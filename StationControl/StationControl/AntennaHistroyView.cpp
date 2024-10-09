// AntennaHistroyView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "AntennaHistroyView.h"
#include "afxdialogex.h"


// CAntennaHistroyView �_�C�A���O

IMPLEMENT_DYNAMIC(CAntennaHistroyView, CDialogBase)

CAntennaHistroyView::CAntennaHistroyView(CWnd* pParent /*=NULL*/)
: CDialogBase(CAntennaHistroyView::IDD, pParent)
	, m_strSelectFile(_T(""))
	, mSearchText(_T(""))
{
	mTitleText.Empty();
	mType = eAntHistory_Operation;
	mSelectFileName.Empty();
	mSaveCx = mSaveCy = 0;
}

CAntennaHistroyView::~CAntennaHistroyView()
{
}

void CAntennaHistroyView::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_FILENAME, m_strSelectFile);
	DDX_Control(pDX, IDC_LIST_FILE, m_ListFile);
	DDX_Text(pDX, IDC_EDIT_SEARCH, mSearchText);
}


BEGIN_MESSAGE_MAP(CAntennaHistroyView, CDialogBase)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_ANTHIST_OPEN, &CAntennaHistroyView::OnMenuOpen)
	ON_COMMAND(ID_ANTHIST_CLOSE, &CAntennaHistroyView::OnMenuClose)
	ON_COMMAND(ID_ANTHIST_PRINT, &CAntennaHistroyView::OnMenuPrint)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CAntennaHistroyView::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH2, &CAntennaHistroyView::OnBnClickedButtonSearch2)
END_MESSAGE_MAP()


// CAntennaHistroyView ���b�Z�[�W �n���h���[


BOOL CAntennaHistroyView::OnInitDialog()
{
	CDialogBase::OnInitDialog();


	mSaveCx = mSaveCy = 0;
	mControl.clear();

	SetControlInfo(IDC_STATIC_FILE, DVANCHORE_LEFTTOP);
	SetControlInfo(IDC_STATIC_FILENAME, DVANCHORE_LEFTTOP);
	SetControlInfo(IDC_EDIT_SEARCH, DVANCHORE_LEFTTOP);
	SetControlInfo(IDC_BUTTON_SEARCH, DVANCHORE_LEFTTOP);
	SetControlInfo(IDC_LIST_FILE, DVANCHORE_LEFTTOP | DVRESIZE_BOTH);

	ModifyStyleEx(0, WS_EX_DLGMODALFRAME);

	if (mTitleText.IsEmpty() == false){
		SetWindowText(mTitleText);
	}

	CFont defFont;
	defFont.CreateStockObject(SYSTEM_FIXED_FONT);
	LOGFONT lf;
	if (defFont.GetLogFont(&lf)){
		//lf.lfWeight = FW_NORMAL;
		//lf.lfHeight = 24;

		if (mListFont.GetSafeHandle()){
			mListFont.DeleteObject();
		}
		mListFont.CreateFontIndirect(&lf);
	}

	// �N���C�A���g�̈�̎擾
	CRect rectClient;
	GetClientRect(rectClient);

	// �g���X�^�C���̎擾
	DWORD  dwStyle = ListView_GetExtendedListViewStyle(m_ListFile.m_hWnd);
	//  �g���X�^�C���̃Z�b�g
	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= LVS_EX_FULLROWSELECT/*|LVS_EX_GRIDLINES*/ | 0x00010000/*LVS_EX_DOUBLEBUFFER*/;
	//  �V�����g���X�^�C����ݒ�
	ListView_SetExtendedListViewStyle(m_ListFile.m_hWnd, dwStyle);
	m_ListFile.InsertColumn(0, _T("99999:"), LVCFMT_LEFT, 50);
	m_ListFile.InsertColumn(1, _T("WWWWWWWWWW"), LVCFMT_LEFT, 100);
	m_ListFile.SetBkColor(RGB(255, 255, 255));
	if (mListFont.GetSafeHandle()){
		m_ListFile.SetFont(&mListFont);
	}

	UpdateData(FALSE);
	AutoSizeColumns();

	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! CAntennaHistroyView

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CAntennaHistroyView::UpdateDlg()
{
	// ��ʏ�����
	if (mTitleText.IsEmpty() == false){
		SetWindowText(mTitleText);
	}

	m_strSelectFile = _T("");
	mSearchText = _T("");
	m_ListFile.DeleteAllItems();
	AutoSizeColumns();
	UpdateData(FALSE);

	return TRUE;
}

/*============================================================================*/
/*! �ڍו\��

-# �J�����̎����T�C�Y����

@param	col		�J�����ʒu�̃J�����T�C�Y�������ɒ������܂�
@retval

*/
/*============================================================================*/
void CAntennaHistroyView::AutoSizeColumns(int col /*=-1*/)
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)m_ListFile.GetDlgItem(0);

	m_ListFile.SetRedraw(false);
	int mincol = col < 0 ? 0 : col;
	int maxcol = col < 0 ? pHeader->GetItemCount() - 1 : col;
	for (col = mincol; col <= maxcol; col++){
		m_ListFile.SetColumnWidth(col, LVSCW_AUTOSIZE);
		int wc1 = m_ListFile.GetColumnWidth(col);
		m_ListFile.SetColumnWidth(col, LVSCW_AUTOSIZE_USEHEADER);
		int wc2 = m_ListFile.GetColumnWidth(col);
		int wc = MAX(50, MAX(wc1, wc2));
		m_ListFile.SetColumnWidth(col, wc);
	}
	m_ListFile.SetRedraw(true);
}

/*============================================================================*/
/*! �ڍו\��

-# �t�@�C�����ڂ̓o�^

@param	nItem		�A�C�e���ԍ�
@param	nSubItem	�T�u�A�C�e���ԍ�
@param	strItem		�ݒ蕶����
@param	dwParam		���[�U�l
@retval

*/
/*============================================================================*/
int CAntennaHistroyView::AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam/*=0*/)
{
	LV_ITEM lv;
	memset(&lv, 0, sizeof(lv));
	lv.mask = LVIF_TEXT;
	lv.iItem = nItem;
	lv.iSubItem = nSubItem;
	if (nSubItem == 0){
		lv.mask |= LVIF_PARAM;
		lv.lParam = dwParam;
	}
	lv.pszText = (LPTSTR)(LPCTSTR)strItem;
	lv.cchTextMax = strItem.GetLength();

	if (nSubItem == 0)
		return m_ListFile.InsertItem(&lv);
	return m_ListFile.SetItem(&lv);
}

/*============================================================================*/
/*! �ڍו\��

-# ���T�C�Y���̃R���g���[������ݒ肷��

@param	id		�R���g���[��ID
@param	anchore	���T�C�Y���̎��ʎq

@retval void
*/
/*============================================================================*/
void CAntennaHistroyView::SetControlInfo(WORD id, WORD anchore)
{
	if (anchore == DVANCHORE_LEFTTOP)
		return;

	DWORD info = id | (anchore << 16);
	mControl.push_back(info);
}


void CAntennaHistroyView::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType == SIZE_MINIMIZED)
		return;

	if (GetSafeHwnd() == NULL)
		return;

	if (mControl.size() == 0)
		return;

	if (mSaveCx == 0){
		mSaveCx = cx;
		mSaveCy = cy;
		return;
	}

	int dx = cx - mSaveCx;
	int dy = cy - mSaveCy;

	CRect rect, rectClient;
	GetClientRect(rectClient);

	CWnd *pWnd;
	DWORD info;
	WORD anchore;

	vector<DWORD>::iterator itr;
	for (itr = mControl.begin(); itr != mControl.end(); itr++){
		info = (*itr);
		pWnd = GetDlgItem(LOWORD(info));
		if (!pWnd){
			continue;
		}

		if (!HIWORD(info))
			continue;

		anchore = HIWORD(info);
		pWnd->GetWindowRect(rect);
		ScreenToClient(rect);

		if (anchore & DVRESIZE_HOR)
			rect.right += dx;
		else if (anchore & DVANCHORE_RIGHT)
			rect.OffsetRect(dx, 0);

		if (anchore & DVRESIZE_VER)
			rect.bottom += dy;
		else if (anchore & DVANCHORE_BOTTOM)
			rect.OffsetRect(0, dy);

		pWnd->MoveWindow(rect);
	}

	mSaveCx = cx;
	mSaveCy = cy;
}

/*============================================================================*/
/*! �ڍו\��

-# �t�@�C���̓ǂݍ���

@param	filename		�ǂݍ��݃t�@�C����
@retval

*/
/*============================================================================*/
bool CAntennaHistroyView::ReadFile(CString filename)
{
	FILE* fp;
	bool bRead = false;
	char szRead[1024];
	CString cbuf;

	if ((_wfopen_s(&fp, filename, _T("r")) != 0) || (fp == NULL) )
	{
		return FALSE;
	}

//#if 1
	TRY
	{
		UINT nStation = theApp.GetSelectStation();
		m_ListFile.DeleteAllItems();

		bRead = true;

		int item = 0;
		int lineNo = 0;
		CString str;
		while (fgets(szRead, static_cast<unsigned>(_countof(szRead)), fp) != NULL)
		{
			if (nStation == eStation_Usuda642)
			{
				// 54m�̃A���e�i�ݔ��쓮�����ƃA���e�i�ݔ��^�p�����̓��ꏈ��
				if (mType == eAntHistory_Operation)
				{
					// 54m�A���e�i�ݔ��^�p�����t�@�C���̏ꍇ�A�ŏ��̂P�s���΂�
					if (lineNo == 0)
					{
						lineNo++;
						continue;
					}
				}
				else
				{
					// 54m�A���e�i�ݔ��쓮�����t�@�C���̏ꍇ�A�ŏ��̂Q�s�ڂ܂ł��΂�
					if (lineNo < 2)
					{
						lineNo++;
						continue;
					}
				}
			}

			cbuf = CServerFile::ConvertUnicode(szRead);
			cbuf.TrimRight(_T("\n"));
			str.Format(_T("%04d:"), item + 1);
			AddItem(item, 0, str);
			AddItem(item, 1, cbuf);

			item++;
			lineNo++;
		}

		fclose(fp);

	} CATCH_ALL(e){
		if (bRead == true)
			fclose(fp);
		return false;
	}
	END_CATCH_ALL

	return true;
//#else
//	CStdioFile	file(fp);
//	TRY
//	{
//		bRead = true;
//
//		CString cbuf, str;
//		BOOL ret = file.ReadString(cbuf);
//		int item = 0;
//		while (ret){
//			str.Format(_T("%04d:"), item + 1);
//			AddItem(item, 0, str);
//			AddItem(item, 1, cbuf);
//			item++;
//			ret = file.ReadString(cbuf);
//		}
//		file.Close();
//	} CATCH_ALL(e){
//		if (bRead == true)
//			file.Close();
//		return false;
//	}
//	END_CATCH_ALL
//
//		return true;
//#endif
}

void CAntennaHistroyView::PostNcDestroy()
{
	//delete this;

	CDialogEx::PostNcDestroy();
}


void CAntennaHistroyView::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 200;
	lpMMI->ptMinTrackSize.y = 200;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CAntennaHistroyView::OnMenuOpen()
{
	UpdateData(TRUE);

	UINT nStation = theApp.GetSelectStation();
	if (theApp.GetSelectStationMode() == eStation_5)
		nStation = 0;

	CString pathname = _T("");
	CString filter = _T("");
	if (mType == eAntHistory_Operation)
	{
		pathname = theApp.GetShareFilePath(eFileType_AntEquipOpHistory_Folder, nStation);
		filter = _T("Antenna History File(uny*.csv)|uny*.csv||");
	}
	else
	{
		pathname = theApp.GetShareFilePath(eFileType_AntEquipDriveHistory_Folder, nStation);
		filter = _T("Antenna History File(kdu*.csv)|kdu*.csv||");
	}

//	const TCHAR BASED_CODE szFilter[] = _T("Antenna History File(*.csv)|ant.csv|All Files (*.*)|*.*||");
//	const TCHAR BASED_CODE szFilter[] = _T("Antenna History File(ant*.csv)|ant*.csv||");
	CFileDialog dlg(TRUE, _T("csv"), pathname, OFN_OVERWRITEPROMPT | OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, filter);
	if (dlg.DoModal() == IDOK)
	{

		mSelectFileName = dlg.GetPathName();
		m_strSelectFile = mSelectFileName;
		if (ReadFile(m_strSelectFile) == false)
		{
			m_strSelectFile = _T("�t�@�C���ǂݍ��݃G���[");
		}
		else
		{
			AutoSizeColumns();
			UpdateData(FALSE);
		}
	}

	// �J���_�C�A���O�\����A���j���[�������Ȃ鎖�ւ̑Ή�
	DrawMenuBar();
}

void CAntennaHistroyView::OnMenuClose()
{
	//	OnOK();
	ShowWindow(SW_HIDE);
}

void CAntennaHistroyView::OnMenuPrint()
{
	if (m_strSelectFile == _T("") || m_strSelectFile == _T("�t�@�C���ǂݍ��݃G���["))
		return;


//#if 1
	// ���[
	CSatelliteData::ExcelOutPut(m_hWnd, m_strSelectFile);

//#else
//	CString title = _T("");
//	GetWindowText(title);
//	CPrintUtility::PrintEucFile(m_strSelectFile, title);
//#endif
}

/*============================================================================*/
/*! �ڍו\��

-# ��������������

@param	�Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CAntennaHistroyView::OnBnClickedButtonSearch()
{
	UpdateData(TRUE);

	// ���������񂪑��݂��Ȃ��ꍇ�͉������Ȃ�
	if (mSearchText.IsEmpty())
		return;

	// ���ݑI������Ă���A�C�e���ԍ����擾����
	POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
//#if 1
	int nItem = 0;
	if (pos != NULL)
	{
		nItem = m_ListFile.GetNextSelectedItem(pos) + 1;
	}
//#else
//	if (pos == NULL)
//	{
//		m_ListFile.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
//		pos = m_ListFile.GetFirstSelectedItemPosition();
//		if (pos == NULL)
//			return;
//	}
//	int nItem = m_ListFile.GetNextSelectedItem(pos) + 1;
//#endif

	// ���ݑI������Ă���A�C�e���ȍ~����������
	CString str, sbuf;
	sbuf = mSearchText;
	sbuf.MakeLower();
	int nColumnCount = m_ListFile.GetHeaderCtrl()->GetItemCount();
	for (; nItem<m_ListFile.GetItemCount(); nItem++)
	{
//		for (int i = 0; i<nColumnCount; i++)	// �����Ώۂ͖{���̂�
		{
			str = m_ListFile.GetItemText(nItem, 1);
			str.MakeLower();
			if (str.Find(sbuf) >= 0){
				m_ListFile.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				m_ListFile.EnsureVisible(nItem, FALSE);
				m_ListFile.SetFocus();
				return;
			}
		}
	}
	MessageBox(_T("�Y���̕����񂪌�����܂���B"), MB_OK);
	m_ListFile.SetFocus();
}


/*============================================================================*/
/*! �ڍו\��

-# �㌟����������

@param	�Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CAntennaHistroyView::OnBnClickedButtonSearch2()
{
	// ���������񂪑��݂��Ȃ��ꍇ�͉������Ȃ�
	if (mSearchText.IsEmpty())
		return;

	// ���ݑI������Ă���A�C�e���ԍ����擾����
	POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
//#if 1
	int nItem = 0;
	if (pos != NULL)
	{
		nItem = m_ListFile.GetNextSelectedItem(pos) - 1;
	}
//#else
//	if (pos == NULL){
//		m_ListFile.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
//		pos = m_ListFile.GetFirstSelectedItemPosition();
//		if (pos == NULL)
//			return;
//	}
//	int nItem = m_ListFile.GetNextSelectedItem(pos) - 1;
//#endif

	// ���ݑI������Ă���A�C�e���ȍ~����������
	CString str, sbuf;
	sbuf = mSearchText;
	sbuf.MakeLower();
	int nColumnCount = m_ListFile.GetHeaderCtrl()->GetItemCount();
	for (; nItem >= 0; --nItem)
	{
//		for (int i = 0; i<nColumnCount; i++)	// �����Ώۂ͖{���̂�
		{
			str = m_ListFile.GetItemText(nItem, 1);
			str.MakeLower();
			if (str.Find(sbuf) >= 0)
			{
				m_ListFile.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				m_ListFile.EnsureVisible(nItem, FALSE);
				m_ListFile.SetFocus();
				return;
			}
		}
	}
	MessageBox(_T("�Y���̕����񂪌�����܂���B"), MB_OK);
	m_ListFile.SetFocus();
}
