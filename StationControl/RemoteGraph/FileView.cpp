// ���� MFC �T���v�� �\�[�X �R�[�h�ł́AMFC Microsoft Office Fluent ���[�U�[ �C���^�[�t�F�C�X 
// ("Fluent UI") �̎g�p���@�������܂��B���̃R�[�h�́AMFC C++ ���C�u���� �\�t�g�E�F�A�� 
// ��������Ă��� Microsoft Foundation Class ���t�@�����X����ъ֘A�d�q�h�L�������g��
// �⊮���邽�߂̎Q�l�����Ƃ��Ē񋟂���܂��B
// Fluent UI �𕡐��A�g�p�A�܂��͔z�z���邽�߂̃��C�Z���X�����͌ʂɗp�ӂ���Ă��܂��B
// Fluent UI ���C�Z���X �v���O�����̏ڍׂɂ��ẮAWeb �T�C�g
// http://go.microsoft.com/fwlink/?LinkId=238214 ���Q�Ƃ��Ă��������B
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "RemoteGraph.h"
#include "WaitDialog.h"

// timeGetTime�g�p
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static const CString mRootName = _T("�Ď��f�[�^�t�@�C��");

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
	m_nActiveNo = 0;
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_OPEN, &CFileView::OnUpdateOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar ���b�Z�[�W �n���h���[

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// �r���[�̍쐬:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("�t�@�C�� �r���[���쐬�ł��܂���ł���\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}

	// �r���[�̃C���[�W�̓ǂݍ���:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ���b�N����Ă��܂�*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// ���ׂẴR�}���h���A�e�t���[���o�R�ł͂Ȃ����̃R���g���[���o�R�œn����܂�:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// �ÓI�c���[ �r���[ �f�[�^ (�_�~�[ �R�[�h) ����͂��܂�
	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView()
{
	CWaitCursor wait;

	if (m_wndFileView.GetSafeHwnd() == NULL)
	{
		return;
	}
	m_wndFileView.DeleteAllItems();
	HTREEITEM hRoot = m_wndFileView.InsertItem(mRootName, 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	m_MonDataFileList.clear();

	vector<stSelectData>& datalist = theApp.GetDataCollection().GetSelectDataList();
	vector<stSelectData>::iterator itr;
	UINT mask = 0;
	HTREEITEM hStation[eStation_MAX];
	CString str;
	for (itr = datalist.begin(); itr != datalist.end(); itr++){
		AfxExtractSubString(str, (*itr).name, 0, '.');
		str.MakeUpper();
		int stno = GetStationNo(str);
		if (stno >= 0 && !(mask&(1 << stno))){
			mask |= (1<<stno);
			hStation[stno] = m_wndFileView.InsertItem(str, hRoot, TVI_LAST);
			m_wndFileView.SetItemData(hStation[stno], (((DWORD_PTR)stno+1)<<16));
			CString pathMon = theApp.GetShareFilePath(eFileType_Equipment_Folder, stno);
			CreateServerFileList(hStation[stno], pathMon, stno);
		}
	}

	ExpandAll(hRoot);
}

/*============================================================================*/
/*! �f�[�^���W

-# �S�A�C�e����W�J����

@param		nItem	�c���[�n���h��
@retval

*/
/*============================================================================*/
void CFileView::ExpandAll(HTREEITEM hItem)
{
	m_wndFileView.Expand(hItem, TVE_EXPAND);
	HTREEITEM hNextItem = m_wndFileView.GetChildItem(hItem);
	while (hNextItem != NULL){
		m_wndFileView.Expand(hNextItem, TVE_EXPAND);
		ExpandAll(hNextItem);
		hNextItem = m_wndFileView.GetNextItem(hNextItem, TVGN_NEXT);
	}
}

/*============================================================================*/
/*! �f�[�^���W

-# �f�[�^���̂���ǔԍ����擾����

@param		hRoot	�e�c���[�n���h��
@param		path	�e�f�B���N�g����
@param		flag	��ʃt���O(0x01:�N 0x02:�� 0x04:�� 0x08:��)
@retval

*/
/*============================================================================*/
int CFileView::GetStationNo(CString& name)
{
	for (int i = 0; i < sizeof(mStationSimpleString) / sizeof(mStationSimpleString[0]); i++){
		if (name == mStationSimpleString[i]){
			return i;
		}
	}
	return -1;
}

/*============================================================================*/
/*! �f�[�^���W

-# �f�[�^���̂���ǔԍ����擾����

@param		hRoot	�e�c���[�n���h��
@param		path	�e�f�B���N�g����
@param		flag	��ʃt���O(0x01:�N 0x02:�� 0x04:�� 0x08:��)
@retval

*/
/*============================================================================*/
HTREEITEM CFileView::FindItemText(HTREEITEM hRoot, CString& name)
{
	HTREEITEM hItem = m_wndFileView.GetChildItem(hRoot);

	while (hItem != NULL){
		if (m_wndFileView.GetItemText(hItem).CompareNoCase(name) == 0){
			return hItem;
		}
		hItem = m_wndFileView.GetNextSiblingItem(hItem);
	}
	return NULL;
}

/*============================================================================*/
/*! �f�[�^���W

-# �T�[�o�[�t�@�C���ꗗ�̍쐬

@param		hRoot	�e�c���[�n���h��
@param		path	�e�f�B���N�g����
@param		flag	��ʃt���O(0x01:�N 0x02:�� 0x04:�� 0x08:��)
@retval

*/
/*============================================================================*/
void CFileView::CreateServerFileList(HTREEITEM hRoot, CString path, int nStationNo)
{
	CString tempPath = path;

	tempPath += _T("*.*");
	// �Ώۃt�H���_���̃t�@�C�������擾����
	// �w�肳�ꂽ�t�@�C�����Ɉ�v����t�@�C�����A�f�B���N�g�����Ō�������
	WIN32_FIND_DATA ffdata;
	HANDLE hFindFile = FindFirstFile(tempPath, &ffdata);
	if (hFindFile == INVALID_HANDLE_VALUE){
		return;
	}

	// �t�H���_����������
	UINT item = 0;
	do{
		TRY
		{
			if (ffdata.cFileName[0] == '\0')
				AfxThrowUserException();
			if (ffdata.cFileName[0] == '.')
				AfxThrowUserException();
			if (ffdata.cFileName[0] == '[')
				AfxThrowUserException();

			// �f�B���N�g�����擾
			if (!(ffdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				AfxThrowUserException();

			CString name, str;
			name = CString(ffdata.cFileName);

			// ���t�t�H���_�����݂����̂ŁA�t�H���_���ɑΏۊĎ��������݂��邩�`�F�b�N���s��
			// ���݂����ꍇ�́AoutFileList�Ƀt�@�C���t���p�X���i�[�����
			if (FindMonitorData(name, nStationNo, m_MonDataFileList) == false){
				AfxThrowUserException();
			}

			HTREEITEM hYear, hMonth, hDay;
			// �N�̃A�C�e���쐬
			AfxExtractSubString(str, name, 0, '-');
			hYear = FindItemText(hRoot, str);
			if (hYear == NULL){
				hYear = m_wndFileView.InsertItem(str, hRoot, TVI_LAST);
				m_wndFileView.SetItemData(hYear, 1);
			}
			// ���̃A�C�e���쐬
			AfxExtractSubString(str, name, 1, '-');
			hMonth = FindItemText(hYear, str);
			if (hMonth == NULL){
				hMonth = m_wndFileView.InsertItem(str, hYear, TVI_LAST);
				m_wndFileView.SetItemData(hMonth, 2);
			}
			// ���̃A�C�e���쐬
			AfxExtractSubString(str, name, 2, '-');
			hDay = FindItemText(hMonth, str);
			if (hDay == NULL){
				hDay = m_wndFileView.InsertItem(str, hMonth, TVI_LAST);
				m_wndFileView.SetItemData(hDay, 3);
			}
		}
		CATCH(CUserException, e)
		{
		}
		AND_CATCH(CException, e)
		{
		}
		END_CATCH
	} while (FindNextFile(hFindFile, &ffdata)); // �t�@�C�����Ȃ��Ȃ�܂ő�����
}

/*============================================================================*/
/*! �f�[�^���W

-# �w��t�@�C���Ƀf�[�^�o�^���ꂽ�Ď��������邩�̑��݃`�F�b�N

@param		hRoot		�e�c���[�n���h��
@param		path		�e�f�B���N�g����
@param		outFileList	�Ώۃt�@�C��
@retval

*/
/*============================================================================*/
bool CFileView::FindMonitorData(CString fname, int nStationNo, vector<CString>& outFileList)
{
	CString strPath = theApp.GetShareFilePath(eFileType_Equipment_Folder, nStationNo);

	strPath += fname;
	strPath += _T("\\");
	int size = (int)outFileList.size();
	GetDataFileList(strPath, outFileList);

	if ((int)outFileList.size() == size)
		return false;

	return true;
}
/*============================================================================*/
/*! �Ď��f�[�^�A�N�Z�X

-# �w��t�H���_�̃t�@�C���ꗗ���擾����

@param	path	�t�H���_��
@param	outFileList	�Ď��t�@�C�����X�g(�o��)

@retval

*/
/*============================================================================*/
void CFileView::GetDataFileList(CString& path, vector<CString>& outFileList)
{
	// �Ώۃt�H���_���̃t�@�C�������擾����
	// �w�肳�ꂽ�t�@�C�����Ɉ�v����t�@�C�����A�f�B���N�g�����Ō�������
	WIN32_FIND_DATA ffdata;
	HANDLE hFindFile = FindFirstFile(path + _T("*"), &ffdata);
	if (hFindFile == INVALID_HANDLE_VALUE){
		return;
	}

	// �Ď��f�[�^�ꗗ
	vector<stSelectData>& datalist = theApp.GetDataCollection().GetSelectDataList();

	UINT item = 0;
	do{
		TRY
		{
			CString fname;
			if (ffdata.cFileName[0] == '\0')
			{
				AfxThrowUserException();
			}
			if (ffdata.cFileName[0] == '.')
			{
				AfxThrowUserException();
			}
			if (ffdata.cFileName[0] == '[')
			{
				AfxThrowUserException();
			}

			// �t�@�C�����擾
			if (ffdata.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
			{
				CString name = CString(ffdata.cFileName);
				if (name.Find(_T(".")) >= 0){
					continue;
				}
				if (name.Left(3).CompareNoCase(_T("NON")) == 0){
					continue;
				}
				// �t�@�C��������Ď����𔲂��o��
				CString tag;
				AfxExtractSubString(tag, name, 0, '_');

				vector<stSelectData>::iterator itr;
				CString str;
				for (itr = datalist.begin(); itr != datalist.end(); itr++){
					AfxExtractSubString(str, (*itr).name, 1, '.');
					AfxExtractSubString(str, str, 0, '_');
					if (tag.CompareNoCase(str) == 0)
						break;
				}
				if(itr != datalist.end())
					outFileList.push_back(path + name);
			}
		}
			CATCH(CUserException, e)
		{
		}
		AND_CATCH(CException, e)
		{
		}
		END_CATCH
	} while (FindNextFile(hFindFile, &ffdata));

	FindClose(hFindFile);
}
/*============================================================================*/
/*! �v���Z�X�ԒʐM

-# AP�w����̃��b�Z�[�W��M

@param

@retval
*/
/*============================================================================*/
void CALLBACK CFileView::receiveCallBack(TCHAR* monName, time_t monTime, xobs_tbl_res_t* monData, bool bLeapSec)
{
//	TRACE("### monName:%s monTime:%I64d monData:%d leapSec=%d\n", CStringA(monName), monTime, monData->obs_adr.l_id, bLeapSec);

	if( monTime == 0 )
	{
		CLogTraceEx::Write(_T(""), _T("CFileView"), _T("receiveCallBack"), _T("monTime"), _T("Data Time Zero"), nLogEx::info);

		return;
	}

	CTime	t = monTime;

	// �N�����t�@�C���r���[�̂��̂ɒu������
	CTime	s_time = theApp.GetDataCollection().GetAnalysisStart(((CMainFrame *)theApp.m_pMainWnd)->GetFileView().GetActiveNo());
	if( t.GetYear() != s_time.GetYear() )
		t = (t.GetTime() - CTime(t.GetYear(), 1, 1, 0, 0, 0).GetTime()) + CTime(s_time.GetYear(), 1, 1, 0, 0, 0).GetTime();

#ifdef	_TEST_LEAP
	if( t.GetSecond() == 0 )
	{
		stQueData	que_data;
		que_data.name.Format(_T("%s.%s"), mStationSimpleString[((CMainFrame *)theApp.m_pMainWnd)->GetFileView().GetHitItemPath().stno], monName);
		memset(&que_data.data, 0, sizeof(stSpaceData));
		t.GetAsSystemTime(que_data.data.systime);
		que_data.data.status = SDS_VALID | (1 ? SDS_LEAP : 0x0);
		que_data.data.mmPair.cnt = 1;

		// �f�[�^�l���擾
		if( (monData->obs_adr.l_cnvResType & (1 << eCnvResType_StatusStr)) || (monData->obs_adr.l_cnvResType & (1 << eCnvResType_Long)) )
			que_data.data.mmPair.max = que_data.data.mmPair.min = 10.0;
		else
			que_data.data.mmPair.max = que_data.data.mmPair.min = 10.0;

		// �ǂݍ��݊Ď��f�[�^���L���[�Ƀv�b�V��
		theApp.GetDataCollection().PushAnalysisData(((CMainFrame *)theApp.m_pMainWnd)->GetFileView().GetActiveNo(), que_data);
	}
#endif	// _TEST_LEAP

	stQueData	que_data;
	que_data.name.Format(_T("%s.%s"), mStationSimpleString[((CMainFrame *)theApp.m_pMainWnd)->GetFileView().GetHitItemPath().stno], monName);
	memset(&que_data.data, 0, sizeof(stSpaceData));
	t.GetAsSystemTime(que_data.data.systime);
	que_data.data.status = SDS_VALID | (bLeapSec ? SDS_LEAP : 0x0);
	que_data.data.mmPair.cnt = 1;

	// �f�[�^�l���擾
#if 0
	if( (monData->obs_adr.l_cnvResType & (1 << eCnvResType_StatusStr)) || (monData->obs_adr.l_cnvResType & (1 << eCnvResType_Long)) )
		que_data.data.mmPair.max = que_data.data.mmPair.min = (double)monData->obs_adr.l_data;
	else
		que_data.data.mmPair.max = que_data.data.mmPair.min = monData->obs_adr.d_data;
#else
	if( monData->obs_adr.l_cnvResType & (1 << eCnvResType_StatusStr) )
	{
		// ������
		que_data.data.mmPair.max = que_data.data.mmPair.min = (double)monData->obs_adr.l_data;
	}
	else if( monData->obs_adr.l_cnvResType & (1 << eCnvResType_Long) )
	{
		// long�l
		que_data.data.mmPair.max = que_data.data.mmPair.min = (double)monData->obs_adr.l_data;
	}
	else if( monData->obs_adr.l_cnvResType & (1 << eCnvResType_Double) )
	{
		// double�l
		que_data.data.mmPair.max = que_data.data.mmPair.min = (double)monData->obs_adr.d_data;
	}
	else if( monData->obs_adr.l_cnvResType & (1 << eCnvResType_LongDouble) )
	{
		// long double�l
		que_data.data.mmPair.max = que_data.data.mmPair.min = (double)monData->obs_adr.ld_data;
	}
	else
	{
		// long�l
		que_data.data.mmPair.max = que_data.data.mmPair.min = (double)monData->obs_adr.l_data;
	}
#endif

	// �ǂݍ��݊Ď��f�[�^���L���[�Ƀv�b�V��
	theApp.GetDataCollection().PushAnalysisData(((CMainFrame *)theApp.m_pMainWnd)->GetFileView().GetActiveNo(), que_data);
}

/*============================================================================*/
/*! �f�[�^���W

-# �T�[�o�[�t�@�C���ꗗ�̍쐬

@param		hRoot	�e�c���[�n���h��
@param		path	�e�f�B���N�g����
@retval

*/
/*============================================================================*/
bool CFileView::GetRangeTimeMonitorData(SYSTEMTIME& stdate, SYSTEMTIME& spdate, int nStationNo)
{
	CDlgWaitCmd waitDlg;

	waitDlg.Open(_T("�f�[�^�ǂݍ���"), _T("�t�@�C������f�[�^��ǂݍ���ł��܂��B\n\n���΂炭���҂����������B"), false);

#ifdef _ENGDLL
	CString strDbName = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, nStationNo);
	CString strPath = theApp.GetShareFilePath(eFileType_Equipment_Folder, nStationNo);
	CString str, tag;
	CEngValCnv engcnv;
	// DB���̂̐ݒ�
	TCHAR buf[_MAX_PATH];
	wsprintf(buf, _T("%s"), strDbName);
	engcnv.SetEqMonDbFileName(buf);
	// �Ď��f�[�^�i�[�t�H���_���̂̐ݒ�
	wsprintf(buf, _T("%s"), strPath);
	engcnv.SetMonDataPath(buf);

	CTime stime, ptime;
	stime = CTime(stdate);
	ptime = CTime(spdate);
	CEngValCnv::TL_TIME timeTarget;
	timeTarget.bMode = (BYTE)1;
	timeTarget.sttime = stime.GetTime();
	timeTarget.sptime = ptime.GetTime();

	// �Ď��������o��
#ifdef _ALLMON
	vector<stSelectData>& datalist = theApp.GetDataCollection().GetSelectDataList();
	vector<stSelectData>::iterator itr;
	for (itr = datalist.begin(); itr != datalist.end(); itr++){
		AfxExtractSubString(str, (*itr).name, 0, '.');
		wsprintf(buf, _T("%s"), ((*itr).name).Mid(str.GetLength() + 1));
		engcnv.AddMonitorName(buf);
	}
#else
	CPropertiesData& prop = theApp.GetPropertiesData(m_nActiveNo);
	for (int i = 0; i < MAX_TDATA; i++){
		tag = prop.item[i].dataName;
		AfxExtractSubString(tag, prop.item[i].dataName, 0, '.');
		CString name = CString(prop.item[i].dataName);
		wsprintf(buf, _T("%s"), name.Mid(tag.GetLength() + 1));
		engcnv.AddMonitorName(buf);
	}
#endif

	// �H�w�l�ϊ��Ώۂ̃t�@�C���ꗗ���R�s�[����
	vector<CString>::iterator fitr;
	engcnv.ClearDataFileList();
	for (fitr = m_MonDataFileList.begin(); fitr != m_MonDataFileList.end(); fitr++){
		// �p�X������Ώۓ��t�����肷��
		CString temp, str = (*fitr);
		str = str.Mid(strPath.GetLength());
		SYSTEMTIME st;
		memset(&st, 0, sizeof(SYSTEMTIME));
		AfxExtractSubString(temp, str, 0, '-');
		st.wYear = _wtoi(temp);
		AfxExtractSubString(temp, str, 1, '-');
		st.wMonth = _wtoi(temp);
		AfxExtractSubString(temp, str, 2, '-');
		st.wDay = _wtoi(temp);
		CTime tm;
		tm = CTime(st);
		if (tm.GetTime() < timeTarget.sttime || tm.GetTime() > timeTarget.sptime)
			continue;

		wsprintf(buf, _T("%s"), (*fitr));
		engcnv.AddDataFileList(buf);
	}

	// �ŏ��^�C�}����\��ݒ�(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();

	// ���Ԕ͈͂ł̍H�w�l�ϊ�
	engcnv.GetMonitorDataEx(&timeTarget);

	DWORD dwElapsed = (timeGetTime() - dwStart);
	TRACE("### GetRangeTimeMonitorData = %.3lf(sec)\n", (double)((double)dwElapsed / 1000.0));
	timeEndPeriod(1);

	map< CString, vector<CEngValCnv::obs_tbl_res_t> >& list = engcnv.GetOutDataList();

	vector<CEngValCnv::obs_tbl_res_t>& data = engcnv.GetAllVal();

	map< CString, vector<CEngValCnv::obs_tbl_res_t> >::iterator itrd;
	for (itrd = engcnv.mOutDataList.begin(); itrd != engcnv.mOutDataList.end(); itrd++){
		if ((*itrd).second.size() != 0)
			return true;
	}
	return false;
#else
	CString strDbName = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, nStationNo);
	CString strPath = theApp.GetShareFilePath(eFileType_Equipment_Folder, nStationNo);
	CString str, sttag, eqtag, name;
	// DB���̂̐ݒ�
	TCHAR buf[_MAX_PATH];
	wsprintf(buf, _T("%s"), (LPCTSTR)strDbName);
	Dll_SetEqMonDbFileName(buf);

	// �Ď��f�[�^�i�[�t�H���_���̂̐ݒ�
	wsprintf(buf, _T("%s"), (LPCTSTR)strPath);
	Dll_SetMonDataPath(buf);

	CTime stime, ptime;
	stime = CTime(stdate);
	ptime = CTime(spdate);
	xTL_TIME timeTarget;
	timeTarget.bMode = (BYTE)1;
	timeTarget.sttime = stime.GetTime();
	timeTarget.sptime = ptime.GetTime();

	// �Ď��������o��
#ifdef _ALLMON
	vector<stSelectData>& datalist = theApp.GetDataCollection().GetSelectDataList();
	vector<stSelectData>::iterator itr;
	for (itr = datalist.begin(); itr != datalist.end(); itr++){
		AfxExtractSubString(str, (*itr).name, 0, '.');
		wsprintf(buf, _T("%s"), ((*itr).name).Mid(str.GetLength() + 1));
		engcnv.AddMonitorName(buf);
	}
#else
	Dll_ClearMonitorName();
	CPropertiesData& prop = theApp.GetPropertiesData(m_nActiveNo);
	for (int i = 0; i < MAX_TDATA; i++){
		name = prop.item[i].dataName;
		AfxExtractSubString(sttag, name, 0, '.');	// �ǖ����o��
		AfxExtractSubString(eqtag, name, 1, '.');	// �ݔ������o��

		// ���ۂ̊Ď�����o�^����
		if( mStationSimpleString[nStationNo] == sttag )
		{
			wsprintf(buf, _T("%s"), (LPCTSTR)name.Mid(sttag.GetLength() + 1));
			Dll_AddMonitorName(buf);
		}
		else
			Dll_AddMonitorName(_T(""));
	}
#endif

	// �H�w�l�ϊ��Ώۂ̃t�@�C���ꗗ���R�s�[����
	vector<CString>::iterator fitr; 
	Dll_ClearDataFileList();
	for (fitr = m_MonDataFileList.begin(); fitr != m_MonDataFileList.end(); fitr++){
		// �p�X������Ώۓ��t�����肷��
		CString temp, str = (*fitr);
		str = str.Mid(strPath.GetLength());
		SYSTEMTIME st;
		memset(&st, 0, sizeof(SYSTEMTIME));
		AfxExtractSubString(temp, str, 0, '-');
		st.wYear = _wtoi(temp);
		AfxExtractSubString(temp, str, 1, '-');
		st.wMonth = _wtoi(temp);
		AfxExtractSubString(temp, str, 2, '-');
		st.wDay = _wtoi(temp);
		CTime tm;
		tm = CTime(st);
		if (tm.GetTime() < timeTarget.sttime || tm.GetTime() > timeTarget.sptime)
			continue;

		wsprintf(buf, _T("%s"), (LPCTSTR)(*fitr));
		Dll_AddDataFileList(buf);
	}

	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CFileView"), _T("GetRangeTimeMonitorData"), _T("Dll_SetCallBack"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//

	// DLL�ɃR�[���o�b�N�֐��ƒʒm����
	Dll_SetCallBack(receiveCallBack);

	// �ŏ��^�C�}����\��ݒ�(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();

	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CFileView"), _T("GetRangeTimeMonitorData"), _T("DeleteSpaceRead"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//

	// �ǂݍ��݊Ď��f�[�^��������
	theApp.GetDataCollection().DeleteSpaceRead();

	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CFileView"), _T("GetRangeTimeMonitorData"), _T("CreateAnalysisDataFromFile"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//

	// ��͂Ŏg�p����f�[�^���Ď��f�[�^�t�@�C������쐬���J�n
	theApp.GetDataCollection().CreateAnalysisDataFromFile(m_nActiveNo, stime, ptime);

	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CFileView"), _T("GetRangeTimeMonitorData"), _T("Dll_GetMonitorDataEx"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//

	// ���Ԕ͈͂ł̍H�w�l�ϊ�
	Dll_GetMonitorDataEx(&timeTarget);

	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CFileView"), _T("GetRangeTimeMonitorData"), _T("PushAnalysisDataFinish"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//

	// �c��̓ǂݍ��݊Ď��f�[�^���L���[�Ƀv�b�V��
	theApp.GetDataCollection().PushAnalysisDataFinish(m_nActiveNo);

	// �擾�����f�[�^���ۑ�����Ă��邩�m�F
	while( theApp.GetDataCollection().IsExistSaveAnalysisDataQue(m_nActiveNo) )
		Sleep(5);

	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CFileView"), _T("GetRangeTimeMonitorData"), _T("FinishAnalysisDataFromFile"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//

	// ��͂Ŏg�p����f�[�^���Ď��f�[�^�t�@�C������쐬���I��
	theApp.GetDataCollection().FinishAnalysisDataFromFile(m_nActiveNo);

	DWORD dwElapsed = (timeGetTime() - dwStart);
	TRACE("### GetRangeTimeMonitorData = %.3lf(sec)\n", (double)((double)dwElapsed / 1000.0));
	timeEndPeriod(1);

	waitDlg.Close();

#if 0
	map< CString, vector<xobs_tbl_res_t> >& list = Dll_GetOutDataList();
	vector<xobs_tbl_res_t>& data = Dll_GetAllVal();
	map< CString, vector<xobs_tbl_res_t> >::iterator itrd;
	for (itrd = list.begin(); itrd != list.end(); itrd++){
		if ((*itrd).second.size() != 0)
			return true;
	}
	return false;
#else
	return true;
#endif
#endif
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// �N���b�N���ꂽ���ڂ̑I��:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			// �I������Ă���A�C�e����������擾����
			pWndTree->SelectItem(hTreeItem);
			SetHitItemName(pWndTree->GetItemText(hTreeItem));
			// �I������Ă���A�C�e������t�H���_�������߂�
			m_stHitItemPath.stno = -1;
			m_stHitItemPath.year.Empty();
			m_stHitItemPath.month.Empty();
			m_stHitItemPath.day.Empty();
			DWORD dw = (DWORD)pWndTree->GetItemData(hTreeItem);
			switch (dw){
			case	3:/*Day*/
				m_stHitItemPath.day = pWndTree->GetItemText(hTreeItem);
				hTreeItem = pWndTree->GetParentItem(hTreeItem);
			case	2:/*Month*/
				m_stHitItemPath.month = pWndTree->GetItemText(hTreeItem);
				hTreeItem = pWndTree->GetParentItem(hTreeItem);
			case	1:/*Year*/
				m_stHitItemPath.year = pWndTree->GetItemText(hTreeItem);
				hTreeItem = pWndTree->GetParentItem(hTreeItem);
				dw = (DWORD)pWndTree->GetItemData(hTreeItem);
				if ((dw >> 16) != 0){
					// �ǖ�
					m_stHitItemPath.stno = (dw >> 16) - 1;
				}
			default:
				break;
			}
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
//	AfxMessageBox(_T("�v���p�e�B..."));
}

void CFileView::OnFileOpen()
{
	CWaitCursor wait;

	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CFileView"), _T("OnFileOpen"), _T("Start"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//

//	vector<CSpaceData> spacedataList;
	CString name = GetHitItemName();
	// �I�����ꂽ�A�C�e���̔N�������擾����
	stSelectPath& stpath = GetHitItemPath();

	if (m_stHitItemPath.stno < 0)
		return;

	SYSTEMTIME stdate, spdate;

	// �H�w�l�ϊ����s���J�n�I�����������߂�
	stdate.wHour = 0;
	stdate.wMinute = 0;
	stdate.wSecond = 0;
	stdate.wMilliseconds = 0;
	spdate.wHour = 23;
	spdate.wMinute = 59;
	spdate.wSecond = 59;
	spdate.wMilliseconds = 0;

	stdate.wYear = _wtoi(m_stHitItemPath.year);
	spdate.wYear = stdate.wYear;
	if (m_stHitItemPath.month.IsEmpty() == true){
		// �w��N�̂P�N��
		stdate.wMonth = 1;
		spdate.wMonth = 12;
	}
	else{
		stdate.wMonth = _wtoi(m_stHitItemPath.month);
		spdate.wMonth = stdate.wMonth;
	}

	if (m_stHitItemPath.day.IsEmpty() == true){
		// �w�茎�̂P����
		stdate.wDay = 1;
		if (spdate.wMonth == 12){
			spdate.wYear++;
			spdate.wMonth = 1;
		}
		else{
			spdate.wMonth = stdate.wMonth + 1;
		}
		spdate.wDay = 1;
		CTime tm = CTime(spdate);
		tm -= CTimeSpan(1, 0, 0, 0);
		tm.GetAsSystemTime(spdate);
	}
	else{
		stdate.wDay = _wtoi(m_stHitItemPath.day);
		spdate.wDay = stdate.wDay;
	}

	// �A�v���������\�ȍő�f�[�^�����m�F
	CTime	t1 = stdate;
	CTime	t2 = spdate;
	if( ((t2 - t1).GetTotalSeconds() + 1) > MAX_ALLDATA_SIZE )
	{
		t2 = t1 + CTimeSpan(MAX_USEDATA_DAYS, 0, 0, 0) - CTimeSpan(0, 0, 0, 1);
		t2.GetAsSystemTime(spdate);
	}

	CString	msg;
	msg.Format(_T("Start Time = %s %s, End Time = %s %s, Data Count = %I64d"), (LPCTSTR)(t1.Format("%Y/%m/%d")), (LPCTSTR)(t1.Format("%H:%M:%S")), (LPCTSTR)(t2.Format("%Y/%m/%d")), (LPCTSTR)(t2.Format("%H:%M:%S")), (t2 - t1).GetTotalSeconds() + 1);

	TRACE("### %s\n", CStringA(msg));

	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CFileView"), _T("OnFileOpen"), _T("Get Data"), msg, nLogEx::info);
	//������������������������ Log ������������������������//
	//=====================================================//

	// �w����Ԃ̃f�[�^���擾����
	GetRangeTimeMonitorData(stdate, spdate, m_stHitItemPath.stno);

#if 0
	deque<CSpaceData>& spdata = theApp.GetDataCollection().GetSpaceData();
	spdata.clear();

	deque<CSpaceData>& data = theApp.GetDataCollection().GetSpaceTemp();
	data.clear();
	vector<CSpaceData>::iterator itr;
	for (itr = spacedataList.begin(); itr != spacedataList.end(); itr++){
		data.push_back((*itr));
	}
#endif

	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CFileView"), _T("OnFileOpen"), _T("End"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//

	return;
}

void CFileView::OnUpdateOpen(CCmdUI *pCmdUI)
{
//	// ���A���^�C����~���ɗL��
//	pCmdUI->Enable(!((CMainFrame*)theApp.GetMainWnd())->GetOnline());

	CPropertiesData& prop = theApp.GetPropertiesData(m_nActiveNo);
	pCmdUI->Enable(((m_stHitItemPath.stno < 0) || prop.isRealTime) ? FALSE : TRUE);
}

void CFileView::OnFileOpenWith()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂�
}

void CFileView::OnDummyCompile()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂�
}

void CFileView::OnEditCut()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂�
}

void CFileView::OnEditCopy()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂�
}

void CFileView::OnEditClear()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂�
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ���b�N����܂���*/);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("�r�b�g�}�b�v��ǂݍ��߂܂���ł���: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}




