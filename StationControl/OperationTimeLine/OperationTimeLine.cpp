
// OperationTimeLine.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "OperationTimeLine.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "OperationTimeLineDoc.h"
#include "OperationTimeLineView.h"
#include "TimeLineView.h"
#include "EquipmentSystem.h"
#include "TimeEvenHistoryView.h"
#include "TimeLimitErrorHistoryView.h"
#include "EquipmentSystemForm.h"
#include <locale.h>
#include "..\Utility\SshShell.h"

#include "sys/stat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COperationTimeLineApp

BEGIN_MESSAGE_MAP(COperationTimeLineApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &COperationTimeLineApp::OnAppAbout)
	// �W���̃t�@�C����{�h�L�������g �R�}���h
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// �W���̈���Z�b�g�A�b�v �R�}���h
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// COperationTimeLineApp �R���X�g���N�V����

COperationTimeLineApp::COperationTimeLineApp()
{
	m_bHiColorIcons = TRUE;

	// �ċN���}�l�[�W���[���T�|�[�g���܂�
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// �A�v���P�[�V���������ʌ��ꃉ���^�C�� �T�|�[�g (/clr) ���g�p���č쐬����Ă���ꍇ:
	//     1) ���̒ǉ��ݒ�́A�ċN���}�l�[�W���[ �T�|�[�g������ɋ@�\���邽�߂ɕK�v�ł��B
	//     2) �쐬����ɂ́A�v���W�F�N�g�� System.Windows.Forms �ւ̎Q�Ƃ�ǉ�����K�v������܂��B
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ���̃A�v���P�[�V���� ID ���������ӂ� ID ������Œu�����܂��B���������
	// ������̌`���� CompanyName.ProductName.SubProduct.VersionInformation �ł�
	SetAppID(_T("OperationTimeLine.AppID.NoVersion"));

	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
	m_nHcopyType = ID_HCOPY_REVERSE;
}

// �B��� COperationTimeLineApp �I�u�W�F�N�g�ł��B

COperationTimeLineApp theApp;


// COperationTimeLineApp ������

BOOL COperationTimeLineApp::InitInstance()
{
	// �A�v���P�[�V���� �}�j�t�F�X�g�� visual �X�^�C����L���ɂ��邽�߂ɁA
	// ComCtl32.dll Version 6 �ȍ~�̎g�p���w�肷��ꍇ�́A
	// Windows XP �� InitCommonControlsEx() ���K�v�ł��B�����Ȃ���΁A�E�B���h�E�쐬�͂��ׂĎ��s���܂��B
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// �A�v���P�[�V�����Ŏg�p���邷�ׂẴR���� �R���g���[�� �N���X���܂߂�ɂ́A
	// �����ݒ肵�܂��B
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// OLE ���C�u���������������܂��B
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit �R���g���[�����g�p����ɂ� AfxInitRichEdit2() ���K�v�ł�	
	// AfxInitRichEdit2();

	// �R�}���h���C���̉��
	ParseCommandLine(m_cmdInfo);
	m_nSelectStation = m_cmdInfo.mSelectStation;
	m_nExecuteMode = m_cmdInfo.mExecuteMode;

	m_hCallWnd = m_cmdInfo.mCallWindow;
#ifndef _DEBUG
	if (m_hCallWnd == NULL){
		return FALSE;
	}
#endif

	// �W��������
	// �����̋@�\���g�킸�ɍŏI�I�Ȏ��s�\�t�@�C����
	// �T�C�Y���k���������ꍇ�́A�ȉ�����s�v�ȏ�����
	// ���[�`�����폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ��Ж��܂��͑g�D���Ȃǂ̓K�؂ȕ������
	// ���̕������ύX���Ă��������B
	SetRegistryKey(_T("�A�v���P�[�V���� �E�B�U�[�h�Ő������ꂽ���[�J�� �A�v���P�[�V����"));
	LoadStdProfileSettings(4);  // �W���� INI �t�@�C���̃I�v�V���������[�h���܂� (MRU ���܂�)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// �A�v���P�[�V�����p�̃h�L�������g �e���v���[�g��o�^���܂��B�h�L�������g �e���v���[�g
	//  �̓h�L�������g�A�t���[�� �E�B���h�E�ƃr���[���������邽�߂ɋ@�\���܂��B

	// ���W�X�g�������폜����
	Unregister();

	// �e��t�@�C���E�t�H���_�̍쐬
	TCHAR	szFullPath[_MAX_PATH];
	TCHAR	szDir[_MAX_DIR];
	TCHAR	szDrive[_MAX_DRIVE];
	TCHAR	szFname[_MAX_FNAME];

	if (::GetModuleFileName(NULL, szFullPath, _MAX_PATH) == 0)
	{
		return FALSE;
	}

	_wsplitpath_s(szFullPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, NULL, 0);
	m_strAppPath.Format(_T("%s%s"), szDrive, szDir);
	//m_strAppInifileName.Format(_T("%s%s%s.ini"), szDrive, szDir, szFname);
	m_strAppInifileName.Format(_T("%s%sStationControl.ini"), szDrive, szDir);
	m_strAppDataPath.Format(_T("%s%sDATA"), szDrive, szDir);
	m_strAppLogPath.Format(_T("%s%sLOG\\"), szDrive, szDir);
	m_strAppFaPanelPath.Format(_T("C:\\FA-Panel\\PROJECT\\"));
	CreateDirectory(m_strAppDataPath, NULL);	//�@�t�H���_�쐬
	CreateDirectory(m_strAppLogPath, NULL);	//�@�t�H���_�쐬

	GetDrawUpdateInterval(true);

	TCHAR buf[MAX_PATH];
	GetPrivateProfileString(SECTION_SYSTEM, KEY_FILESERVER_NAME, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strServerName = buf;
	// �l�b�g���[�N�h���C�u�̊m��
	//CreateNetworkDrive();
	if (m_strServerName.IsEmpty() == false){
		m_strServerName += _T("\\");
	}

	// ���C�� MDI �t���[�� �E�B���h�E���쐬���܂��B
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// �e�́uOperationControl�v�Ɏ��n���h����������
	PostMessage(m_cmdInfo.mCallWindow, eMessageTime_TimeLineHandle, 1, (LPARAM)m_pMainWnd->m_hWnd);

	// DDE�Afile open �ȂǕW���̃V�F�� �R�}���h�̃R�}���h ���C������͂��܂��B
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// �R�}���h ���C���Ŏw�肳�ꂽ�f�B�X�p�b�` �R�}���h�ł��B�A�v���P�[�V������
	// /RegServer�A/Register�A/Unregserver �܂��� /Unregister �ŋN�����ꂽ�ꍇ�AFalse ��Ԃ��܂��B
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_nLogLevel = GetPrivateProfileInt(SECTION_SYSTEM, KEY_LOGLEVEL, nLogEx::fatal, m_strAppInifileName);
	CLogTraceEx::Create(m_strAppLogPath, _T("OperationTimeLine"), m_nLogLevel, nLogEx::text);
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("COperationTimeLineApp"), _T("InitializeAppl"), _T("OperationTimeLine"), _T("Start"), nLogEx::info);
	//������������������������ Log ������������������������//
	//=====================================================//

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHSERVER_NAME, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strSshServerName = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHUSER_NAME, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strSshUserName = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHKEY_PATH, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strSshKeyPath = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHBATCH_PATH, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strBatchPath = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SERVICE_MODE, _T(""), buf, MAX_PATH, m_strAppInifileName);
	CString strServiceIni = buf;
	mServiceMode = eServiceMode_user;	// �T�[�r�X�̃f�t�H���g�̓��[�U���[�h
	for (int i = 0; i < eServiceMode_max; i++)
	{
		if (strServiceIni.CompareNoCase(mServiceModeString[i]) == 0)
		{
			mServiceMode = i;
			break;
		}
	}

	// 54m �������ʔԍ��ݒ�
	GetPrivateProfileString(SECTION_SYSTEM, KEY_HISTID54, DEFAULT_HISTID54, buf, MAX_PATH, m_strAppInifileName);
	m_nHistID54 = ::_tcstol(buf, NULL, 16);

	m_nMessageThread = GetPrivateProfileInt(SECTION_SYSTEM, KEY_MESSAGETHREAD, 1, m_strAppInifileName);

	SetLimitedMode();	//���������䃂�[�h�̐ݒ�

	if (CreateDocTemplate() == FALSE)
		return FALSE;

	for (int i = eTimeViewType_OperatioTimeline; i < eTimeViewType_Max; i++){
		OpenView(i);
	}
	((CMainFrame*)pMainFrame)->SetStationMenuBar();

	// ���C�� �E�B���h�E�����������ꂽ�̂ŁA�\���ƍX�V���s���܂��B
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED/*m_nCmdShow*/);
	pMainFrame->UpdateWindow();

	((CMainFrame*)pMainFrame)->OnTileHorz();

	// ���S�Ƀ��C���t���[�����쐬���ꂽ��łȂ��Ƃ���
	::SendMessage(m_pMainWnd->m_hWnd, ID_PIPE_CREATE, 0, 0);

	return TRUE;
}
/*============================================================================*/
/*! �A�v���P�[�V����

-# �h�b�L���O�E�B���h�E�̍쐬

@param
@retval BOOL

*/
/*============================================================================*/
BOOL COperationTimeLineApp::CreateDocTemplate()
{
	for (int i = eTimeViewType_OperatioTimeline; i < eTimeViewType_Max; i++){
		CRuntimeClass* pViewClass;
		switch (i){
		case	eTimeViewType_OperatioTimeline:		pViewClass = RUNTIME_CLASS(CTimeLineView); break;
			//case	eTimeViewType_EquipmentSystem:		pViewClass = RUNTIME_CLASS(CEquipmentSystem); break;
		case	eTimeViewType_EquipmentSystem:		pViewClass = RUNTIME_CLASS(CEquipmentSystemForm); break;
		case	eTimeViewType_Event:				pViewClass = RUNTIME_CLASS(CTimeEvenHistoryView); break;
		case	eTimeViewType_LimitError:			pViewClass = RUNTIME_CLASS(CTimeLimitErrorHistoryView); break;
		default:	return TRUE;
		}
#ifdef MON_UT
		UINT nIDResource = IDR_PASSSTATUS_MON_UT;
#else
		UINT nIDResource = IDR_PASSSTATUS;
#endif /* MON_UT */
		switch (i){
		case	eTimeViewType_OperatioTimeline:	nIDResource = IDI_ICON_TIMELINE; break;
		case	eTimeViewType_EquipmentSystem:	nIDResource = IDI_ICON_STATUS; break;
		case	eTimeViewType_Event:			nIDResource = IDI_ICON_HISTORY; break;
		case	eTimeViewType_LimitError:		nIDResource = IDI_ICON_HISTORY; break;
		default:	return TRUE;
		}

		m_pDocTemplate[i] = new CMultiDocTemplate(nIDResource,
			RUNTIME_CLASS(COperationTimeLineDoc),
			RUNTIME_CLASS(CChildFrame), // custom MDI child frame
			pViewClass);
		if (!m_pDocTemplate[i])
			return FALSE;
		AddDocTemplate(m_pDocTemplate[i]);
	}

	return TRUE;
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# �w�肳�ꂽ�r���[���J��

@param	val			�E�B���h�E���
@retval

*/
/*============================================================================*/
void COperationTimeLineApp::OpenView(UINT val)
{
	// �w��̃E�B���h�E���J��
	COperationTimeLineDoc* pdoc = (COperationTimeLineDoc*)m_pDocTemplate[val]->OpenDocumentFile(NULL);
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# �w�肳�ꂽ�r���[�����

@param	val			�E�B���h�E���
@retval

*/
/*============================================================================*/
void COperationTimeLineApp::CloseView(UINT val)
{
	// �E�B���h�E���
	// �w��̃E�B���h�E�����
	if (m_pDocTemplate[val] != NULL){
		POSITION pt = m_pDocTemplate[val]->GetFirstDocPosition();
		while (pt){
			CDocument* pdoc = m_pDocTemplate[val]->GetNextDoc(pt);
			if (pdoc == NULL)
				break;
			POSITION pt2 = pdoc->GetFirstViewPosition();
			while (pt2){
				CView *pview = pdoc->GetNextView(pt2);
				CWnd *p;
				p = pview->GetParent();
				p->SendMessage(WM_CLOSE, 0, 0L);
				break;
			}
		}
	}
}
/*============================================================================*/
/*! �A�v���P�[�V����

-# �w�肳�ꂽ�r���[���擾����

@param	val			�E�B���h�E���
@retval

*/
/*============================================================================*/
CView* COperationTimeLineApp::GetView(UINT val)
{
	if (m_pDocTemplate[val] != NULL){
		POSITION pt = m_pDocTemplate[val]->GetFirstDocPosition();
		while (pt){
			CDocument* pdoc = m_pDocTemplate[val]->GetNextDoc(pt);
			if (pdoc == NULL)
				break;
			POSITION pt2 = pdoc->GetFirstViewPosition();
			while (pt2){
				CView *pview = pdoc->GetNextView(pt2);
				return pview;
			}
		}
	}
	return NULL;
}

int COperationTimeLineApp::ExitInstance()
{
	//=====================================================//
	//������������������������ Log ������������������������//
	if (m_nLogLevel < nLogEx::none){
		CLogTraceEx::Write(_T(""), _T("COperationTimeLineApp"), _T("ExitInstance"), _T("OperationTimeLine"), _T("Stop"), nLogEx::info);
		CLogTraceEx::Close();
		CLogTraceEx::Delete();
	}
	//������������������������ Log ������������������������//
	//=====================================================//

	AfxOleTerm(FALSE);

	// �e�́uOperationControl�v�ɏI����������
	PostMessage(m_cmdInfo.mCallWindow, eMessageTime_TimeLineFinish, 1, 0);

	return CWinAppEx::ExitInstance();
}

// COperationTimeLineApp ���b�Z�[�W �n���h���[


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �_�C�A���O�����s���邽�߂̃A�v���P�[�V���� �R�}���h
void COperationTimeLineApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// COperationTimeLineApp �̃J�X�^�}�C�Y���ꂽ�ǂݍ��݃��\�b�h�ƕۑ����\�b�h

void COperationTimeLineApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void COperationTimeLineApp::LoadCustomState()
{
}

void COperationTimeLineApp::SaveCustomState()
{
}

// COperationTimeLineApp ���b�Z�[�W �n���h���[





BOOL COperationTimeLineApp::PreTranslateMessage(MSG* pMsg)
{
	CView* p;
	if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_NCLBUTTONDOWN){
		for (int i = 0; i < eTimeViewType_Max; i++){
			if (i == eTimeViewType_Event || i == eTimeViewType_LimitError){
				p = GetView(i);
				if (p != NULL){
					if (i == eTimeViewType_Event)
						((CTimeEvenHistoryView*)p)->CloseChildWindow(pMsg);
					if (i == eTimeViewType_LimitError)
						((CTimeLimitErrorHistoryView*)p)->CloseChildWindow(pMsg);
				}
			}
		}
	}

	return CWinAppEx::PreTranslateMessage(pMsg);
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# �t�@�C���p�X�̎擾

@brief ���L�t�H���_�̃t�@�C���p�X�܂��̓t�H���_�p�X���擾����

@param	fileType	�t�@�C���^�C�v
@param	nStation	�ǔԍ��i�ȗ����́A�P�ǉ�ʂŏȗ����͎��ǂ��A�T�ǉ�ʂł͉P�c64m�ǂ�Ԃ��j
@retval �t�@�C���p�X�܂��̓t�H���_�p�X
*/
/*============================================================================*/
CString COperationTimeLineApp::GetShareFilePath(UINT fileType, UINT nStation/* = eStation_MAX*/)
{
	// �ǔԍ��̈����ȗ���
	if (nStation == eStation_MAX)
	{
		nStation = m_nSelectStation;
	}

	// �ǔԍ��ƃt�@�C���^�C�v����Ǖʃt�@�C���E�t�H���_�p�X���擾
	CString strFilePath;
	strFilePath.Format(_T("\\\\%sshare\\%s\\%s"), (LPCTSTR)m_strServerName, mStationFolder[nStation], mSvFileFolder[fileType]);

	if (m_strServerName.IsEmpty() == true){
		strFilePath.Format(_T("C:\\share\\%s\\%s"), mStationFolder[nStation], mLocalFileFolder[fileType]);
	}

	return strFilePath;
}

/*============================================================================*/
/*! CDBAccess

-# �Ď��f�[�^�i�[

@param	nStation	�F�ǔԍ�
@param	data		�F�H�w�l�ϊ��f�[�^
@retval

*/
/*============================================================================*/
void COperationTimeLineApp::SetEqMonData(UINT nStation, vector<CEngValCnv::obs_tbl_res_t>& data)
{
	CGraphMutex::Lock(eData);

	int cnt = (int)data.size();
	for (int i = 0; i < cnt; ++i)
	{
		CEngValCnv::obs_tbl_res_t mon_data = data[i];
		CString obsname = CString(mon_data.obs_adr.sz_obsname);
		m_mondatalist[nStation][obsname] = mon_data.obs_adr;
	}

	CGraphMutex::Unlock(eData);
}
/*============================================================================*/
/*! CDBAccess

-# �Ď��f�[�^�擾

@param	nStation	�F�ǔԍ�
@param	strWatch	�F�Ď�����
@retval strct obsname_t �|�C���^

*/
/*============================================================================*/
char* COperationTimeLineApp::GetObsname_t(UINT nStation, CString strWatch)
{
	char *obs = NULL;

	CGraphMutex::Lock(eData);

	if (m_mondatalist.count(nStation) != 0)
	{
		map<CString, CEngValCnv::obsname_t> &mon = m_mondatalist[nStation];

		if (mon.count(strWatch) != 0)
		{
			obs = (char*)&mon[strWatch];
		}
	}

#ifdef MON_UT
	if ((obs == NULL) && (!mon_UT.empty()))
	{
		map<CString, CEngValCnv::obsname_t> &mon_ut = mon_UT;
		if (mon_ut.count(strWatch) != 0)
		{
			obs = (char*)&mon_ut[strWatch];
		}
	}
#endif

	CGraphMutex::Unlock(eData);

	return obs;
}

#ifdef MON_UT
/*============================================================================*/
// �Ď����e�[�u���̍X�V
/*============================================================================*/
void COperationTimeLineApp::SetMonUT(vector<CEngValCnv::obsname_t>& data)
{
	int cnt = (int)data.size();

	CGraphMutex::Lock(eData);

	mon_UT.clear();
	for (int i = 0; i<cnt; ++i)
	{
		CEngValCnv::obsname_t mon_data = data[i];
		CString obsname = CString(mon_data.sz_obsname);
		mon_UT[obsname] = mon_data;
	}

	CGraphMutex::Unlock(eData);
}
#endif

/*============================================================================*/
/*! �A�v���P�[�V����

-#  ��IP�A�h���X�̎擾

@param

@retval

*/
/*============================================================================*/
DWORD COperationTimeLineApp::GetHostIpInfo()
{
	char 		host_name[256];				// �z�X�g�̖��O
	memset(host_name, 0, sizeof(host_name));
	DWORD	ipAddress = 0;

	// �z�X�g���̎擾
	if (gethostname(host_name, 256) == 0){
		struct addrinfo hints, *res;
		struct in_addr addr;
		int err;

		memset(&hints, 0, sizeof(hints));
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_family = AF_INET;

		if ((err = getaddrinfo(host_name, NULL, &hints, &res)) != 0) {
			printf("error %d\n", err);
			return 1;
		}

		addr.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;
		ipAddress = addr.S_un.S_addr;
	}
	return ipAddress;
}

/*============================================================================*/
/*! �f�[�^���W

-# �������̍쐬

@param	pUtf8Str			UTF-8

@retval	CString
*/
/*============================================================================*/
CString COperationTimeLineApp::UTF8_3CharsToSJis(char *pUtf8Str)
{
	wchar_t pUtfMsg[4];

	if (pUtf8Str[0] >= 0x00 && pUtf8Str[0] <= 0x7f){
		// 1�o�C�g����
		return _T("");
	}

	// UTF8 �̂R�o�C�g�����C�h�L�����N�^�֍���
	pUtfMsg[0] = ((pUtf8Str[0] & 0x0f) << 12)
		| ((pUtf8Str[1] & 0x3f) << 6)
		| (pUtf8Str[2] & 0x3f);
	pUtfMsg[1] = 0x0000;

	size_t wLen = 0;
	errno_t err = 0;

	//���P�[���w��
	char* back = setlocale(LC_ALL, "japanese");
	//�ϊ�
	char	sjis[10];
	err = wcstombs_s(&wLen, sjis, 10, pUtfMsg, _TRUNCATE);
#ifndef _DEBUG
	setlocale(LC_ALL, back);
#endif
	if (err != 0)
	{
		//=====================================================//
		//������������������������ Log ������������������������//
		CString msg;
		msg.Format(_T("Code Error : err(%d), 0x%02X 0x%02X 0x%02X"), err, pUtf8Str[0] & 0xFF, pUtf8Str[1] & 0xFF, pUtf8Str[2] & 0xFF);
		CLogTraceEx::Write(_T(""), _T("COperationTimeLineApp"), _T("UTF8_3CharsToSJis"), msg, _T("detail"), nLogEx::detail);
		//������������������������ Log ������������������������//
		//=====================================================//
		return _T("");
	}

	CString str(sjis);
	return str;
}
#ifdef _DEBUG
//#define _DEBUG_HISTORY 1		// �����f�o�b�O�f�[�^�\��
#endif
/*============================================================================*/
/*! �f�[�^���W

-# �������̍쐬

@param	pbuf		�T�[�o�[�����f�[�^

@retval
*/
/*============================================================================*/
void COperationTimeLineApp::CreateDataToHistory(PIPEMONITORACK* pbuf)
{
	DWORD	dwStart = timeGetTime();

	stHistoryData data;
	memset(&data, 0, sizeof(stHistoryData));

	char* ptr = (char*)pbuf->buf;

	// �Y���ǂ𒲂ׂ�
	ptr += 0x0C;	// ���M������
	DWORD nStation = *((DWORD*)ptr);
	if (nStation == 0x010a0102) // UDSC64
		data.station = 0;
	else if (nStation == m_nHistID54) // XDSC54
		data.station = 1;
	else if (nStation == 0x010a0203) // USC34
		data.station = 2;
	else if (nStation == 0x010a0103) // USC20
		data.station = 3;
	else{
		if (nStation == 0) // �}�g�Ǔ�
			data.station = (BYTE)theApp.GetSelectStation();
	}

	ptr = (char*)pbuf->buf;
	CEngValCnv::sdtp_msg_ex_t* pmsg = (CEngValCnv::sdtp_msg_ex_t*)pbuf->buf;

#ifdef _DEBUG_HISTORY
	const char sdtp[109] = {
		/*0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x89, 0x00, 0x00, 0x00, 0xB0, 0x30, 0x00, 0x01,
		0x01, 0x00, 0x00, 0x00, 0x09, 0x80, 0x05, 0x00,
		0x04, 0x00, 0x02, 0x00,*/ 0x41, 0x00, 0x00, 0x00,
		0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B,
		0x02, 0x01, 0x0A, 0x01, 0x00, 0x00, 0x02, 0x01,
		0x04, 0x01, 0x00, 0x00, 0x10, 0x03, 0x6D, 0x00,
		0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00,
		0x32, 0x30, 0x31, 0x37, 0x2D, 0x30, 0x33, 0x2D,
		0x31, 0x30, 0x20, 0x30, 0x35, 0x3A, 0x34, 0x31,
		0x3A, 0x33, 0x32, 0x20, 0x43, 0x30, 0x20, 0x28,
		0x30, 0x30, 0x30, 0x30, 0x39, 0x29, 0x20, 0x64,
		0x69, 0x20, 0xE3, 0x83, 0x86, 0xE3, 0x82, 0xB9,
		0xE3, 0x83, 0x88, 0xE3, 0x83, 0xA1, 0xE3, 0x83,
		0x83, 0xE3, 0x82, 0xBB, 0xE3, 0x83, 0xBC, 0xE3,
		0x82, 0xB8, 0xE3, 0x81, 0xA7, 0xE3, 0x81, 0x99,
		0x00,
	};
	memcpy(ptr, (char*)&sdtp[0], 109);
#endif

	CString strDate, strTime, strSend, strMsgid, strLevel, strMsg;

	ptr += 0x2C;

//	// 54m�ǈȊO�̏ꍇ�i�����[�g���j
//	if (data.station != eStation_Usuda642)
//	{
//		CString buf;
//		int item = 0;
//		CString temp;
//		buf.Empty();
//		while (1){
//			if (*ptr == 0x00){
//				strMsg = buf;
//				break;
//			}
//			if (*ptr == 0x20 && item < 5){
//				// ��̃A�C�e��������
//				switch (item){
//				case	0:
//					strDate = buf;
//					break;
//				case	1:
//					strTime = buf;
//					break;
//				case	2:
//					strSend = buf;
//					break;
//				case	3:
//					strMsgid = buf;
//					break;
//				case	4:
//					strLevel = buf;
//					break;
//				case	5:
//					strMsg = buf;
//					break;
//				}
//				ptr++;
//				item++;
//				buf.Empty();
//				continue;
//			}
//			if (*ptr >= 0x00 && *ptr <= 0x7F){
//				// ASCII�R�[�h
//				buf += CString(char(*ptr));
//				ptr++;
//			}
//			else if ((*ptr & 0xE0) == 0xE0){
//				// ����
//				buf += UTF8_3CharsToSJis(ptr);
//				ptr += 3;
//			}
//			else{
//				// ���̑�
//				//=====================================================//
//				//������������������������ Log ������������������������//
//				CString msg;
//				msg.Format(_T("Code Error : 0x%02X"), *ptr & 0xFF);
//				CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("detail"), nLogEx::detail);
//				//������������������������ Log ������������������������//
//				//=====================================================//
//				return;
//			}
//		}
//	}
//	else
	{
		//�d����胁�b�Z�[�W�����擾����(4ByteLSB) �o�b�t�@��16384BYTE�������̂ŃJ�b�g
		unsigned int msglen = ((BYTE)ptr[0x28] + ((BYTE)ptr[0x29] << 8) + ((BYTE)ptr[0x2A] << 16) + ((BYTE)ptr[0x2B] << 24)) % 0x4000;

		std::string stdbuf;
		int item = 0;
		CString temp;
		// ���b�Z�[�W���������b�Z�[�W���ɂȂ�܂ŌJ��Ԃ�
		// ���b�Z�[�W�������F2015-08-23 00:25:41 u6opr1 (5052) di CTRL.PRIV_STTN,ON,,ANS:OK
		// �@���t�@�A���ԁ@�B�ݔ��@�C���b�Z�[�WID�@�D��ԁ@�E���b�Z�[�W������
		while (1){
			if (msglen < 1 || *ptr == 0x00){
				// �f�[�^�t�B�[���h���I������̂ŕϊ���������Z�b�g���ďI��
				// ���b�Z�[�W������̃R�[�h�ϊ�
				strMsg = ConvUtf8Unicode(stdbuf);
				break;
			}
			// ��؂蕶���u0x20�v����������A�ϊ�������������A�C�e�����ɃZ�b�g����
			if (*ptr == 0x20 && item < 5){
				// ��̃A�C�e��������
				switch (item){
				case	0:	strDate = stdbuf.c_str();		break;
				case	1:	strTime = stdbuf.c_str();		break;
				case	2:	strSend = stdbuf.c_str();		break;
				case	3:	strMsgid = stdbuf.c_str();		break;
				case	4:	strLevel = stdbuf.c_str();		break;
				}
				ptr++;
				msglen--;
				item++;
				stdbuf = "";
				continue;
			}
			// ������A������
			stdbuf += char(*ptr);
			ptr++;
			msglen--;
		}
	}

	CString str;
	int i = 0;
	// ���t
	i = 0;
	AfxExtractSubString(str, strDate, i++, '-');
	data.year = _wtoi(str);
	AfxExtractSubString(str, strDate, i++, '-');
	data.month = _wtoi(str);
	AfxExtractSubString(str, strDate, i++, '-');
	data.day = _wtoi(str);
	// ����
	i = 0;
	AfxExtractSubString(str, strTime, i++, ':');
	data.hour = _wtoi(str);
	AfxExtractSubString(str, strTime, i++, ':');
	data.minute = _wtoi(str);
	AfxExtractSubString(str, strTime, i++, ':');
	data.second = _wtoi(str);

	// ���M��
	typedef struct {
		CString		name;
		UINT		code;
	} stSender;
	const stSender mSender[] = {
		{ _T("e1"), eSenderItem1 },	// �o��Ǒ��ǉ^�p�ǐ����u
		{ _T("e2"), eSenderItem1 },	// ���͌����ǉ^�p�ǐ����u
		{ _T("e3"), eSenderItemMax },	// �}�g�ǉ^�p�ǐ����u
		{ _T("c0"), eSenderItem2 },	// �ݔ����䑕�u
		{ _T("85"), eSenderItem3 },	// S��TLM���͑��u
		{ _T("86"), eSenderItem3 },	// X��TLM���͑��u
		{ _T("87"), eSenderItem4 },	// S�уR�}���h�o�͑��u
	};
#ifdef _DEBUG_HISTORY
	data.equipment = (BYTE)(rand() % (eSenderItemMax + 1));
#else
	strSend.MakeLower();
	data.equipment = eSenderItemMax + 1;
	for (int i = 0; i < sizeof(mSender) / sizeof(mSender[0]); i++){
		if (strSend == mSender[i].name){
			data.equipment = mSender[i].code;
			break;
		}
	}
	if (data.equipment == (eSenderItemMax + 1)){
		//=====================================================//
		//������������������������ Log ������������������������//
		CString msg;
		msg.Format(_T("%s %s %s %s %s %s"), strDate, strTime, strSend, strMsgid, strLevel, strMsg);
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::debug);
		//������������������������ Log ������������������������//
		//=====================================================//
		return;
	}
#endif
	// ���b�Z�[�WID
#ifdef _DEBUG_HISTORY
	switch (rand() % 3){
	case	0:
		data.message = (DWORD)((rand() % 10000) + 30000);
		break;
	case	1:
		data.message = (DWORD)((rand() % 10000) + 10000);
		break;
	case	2:
		data.message = (DWORD)(30000);
		break;
	}
#else
	data.message = _wtoi(strMsgid.Mid(1, strMsgid.GetLength() - 2));
#endif

	// ���b�Z�[�W������
	wsprintf(data.msgstring, _T("%s"), (LPCTSTR)strMsg);
#ifdef _DEBUG_HISTORY
	if ((rand() % 5) == 0){
		wsprintf(data.msgstring, _T("[�C�x���g���s]%s"), (LPCTSTR)strMsg);
	}

#endif

	bool bLimit = false;
	if (data.message >= 30000 && data.message <= 39999 ||
		data.message >= 40000 && data.message <= 49999){
		bLimit = IsMessageLimit(data.message);
	}

	// �X�e�[�^�X
	if (data.message >= 30000 && data.message <= 39999 ||
		data.message >= 40000 && data.message <= 49999)
	{
		bool bStatus = false;
		// ��ԑJ�ڗ���
		// ���~�b�g�G���[����
		switch (data.message % 10){
		case	1:/*POWER ON/OFF*/
			data.status = 4;
			break;
		case	2:/*REMOTE/LOCAL*/
			data.status = 3;
			break;
		case	3:/*MAINT/NORMAL*/
			data.status = 3;		// REMOTE/LOCAL�Ɋ܂߂�
			break;
		case	4:/*WARNING*/
			data.status = 1;
			break;
		case	5:/*FAULT*/
			data.status = 2;
			break;
		case	6:/*FAULT*/
			data.status = 2;
			break;
		case	7:/*STATUS*/
			data.status = 0;
			break;
		case	8:/*����*/
			data.status = 0;
			data.imstatus = 0;
			bStatus = true;
			break;
		default:
			if (bLimit == true){
				if ((data.message % 10) == 0)
				{
					data.status = 0;	// STATUS
					data.imstatus = 0;	// ����
				}
				else if ((data.message % 10) == 9)
				{
					data.status = 2;	// FAULT
					data.imstatus = 2;	// �ُ�
				}
				bStatus = true;
				break;
			}
			//=====================================================//
			//������������������������ Log ������������������������//
			CString msg;
			msg.Format(_T("%s %s %s %s %s %s"), (LPCTSTR)strDate, (LPCTSTR)strTime, (LPCTSTR)strSend, (LPCTSTR)strMsgid, (LPCTSTR)strLevel, (LPCTSTR)strMsg);
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::debug);
			//������������������������ Log ������������������������//
			//=====================================================//
			return;
		}
		if (bStatus == false)
		{
#ifdef _DEBUG_HISTORY
			data.imstatus = (BYTE)(rand() % 3);
#else
			if (strLevel == _T("di"))
				data.imstatus = 0;
			else if (strLevel == _T("dw"))
				data.imstatus = 1;
			else if (strLevel == _T("ds"))
				data.imstatus = 2;
			else if (strLevel == _T("dg"))
				data.imstatus = 0;
			else{// if (strLevel == _T("lo")){
				//=====================================================//
				//������������������������ Log ������������������������//
				CString msg;
				msg.Format(_T("%s %s %s %s %s %s"), strDate, strTime, strSend, strMsgid, strLevel, strMsg);
				CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::debug);
				//������������������������ Log ������������������������//
				//=====================================================//
				return;
			}
#endif
		}
	}
	else
	{
		// ��L�ȊO
		data.imstatus = 0;
#ifdef _DEBUG_HISTORY
		data.status = (BYTE)(rand() % 3);
#else
		if (strLevel == _T("di"))
			data.status = 0;
		else if (strLevel == _T("dw"))
			data.status = 1;
		else if (strLevel == _T("ds"))
			data.status = 2;
		else if (strLevel == _T("dg"))
			data.status = 0;
		else{// if (strLevel == _T("lo")){
			//=====================================================//
			//������������������������ Log ������������������������//
			CString msg;
			msg.Format(_T("%s %s %s %s %s %s"), strDate, strTime, strSend, strMsgid, strLevel, strMsg);
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::debug);
			//������������������������ Log ������������������������//
			//=====================================================//
			return;
		}
#endif
	}

	bool bOpeMsg = false;
	if (data.message >= 0 && data.message <= 29999){
		// �^�p����
		bOpeMsg = true;
	}

#ifdef _DEBUG_HISTORY
	data.station = (BYTE)(rand() % eStation_MAX);
	if (data.equipment == eSenderItemMax){
		data.station = (BYTE)0;
	}
#else
	if (nStation == 0x010a0102) {// UDSC64
		// �֐��`���ŋǔԃZ�b�g�ς�
	}
	else if (nStation == m_nHistID54) {// XDSC54
		// �֐��`���ŋǔԃZ�b�g�ς�
	}
	else if (nStation == 0x010a0203) {// USC34
		// �֐��`���ŋǔԃZ�b�g�ς�
	}
	else if (nStation == 0x010a0103) {// USC20
		// �֐��`���ŋǔԃZ�b�g�ς�
	}
	else{
		if (bOpeMsg == true){
			// �^�p�����̏ꍇ�͒}�g�Ǔ��̃��b�Z�[�W�����݂��A�ǎ��ʎq���O
			// �֐��`���ŋǔԃZ�b�g�ς�
		}
		else{
			//=====================================================//
			//������������������������ Log ������������������������//
			CString msg;
			msg.Format(_T("%s %s %s %s %s 0x%08x %s"), strDate, strTime, strSend, strMsgid, strLevel, nStation, strMsg);
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::error);
			//������������������������ Log ������������������������//
			//=====================================================//
			return;
		}
	}
	if (data.equipment == eSenderItemMax){
		data.station = (BYTE)0;
	}
#endif

	if (theApp.GetSelectStation() != data.station){
		// �Ώۂ̋ǂł͂Ȃ�
		//=====================================================//
		//������������������������ Log ������������������������//
		CString msg;
		msg.Format(_T("Station(%d) is not support"), (int)data.station);
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("debug"), nLogEx::debug);
		//������������������������ Log ������������������������//
		//=====================================================//
		return;
	}


	bool bEvent = IsMessageEvent(data.msgstring);

	CGraphMutex::Lock(eData);
	if (bEvent == true){
		// �C�x���g����
		mEventHistoryTemp.push_back(data);
	}
	else{
		if (data.message >= 30000 && data.message <= 39999){
			if (bLimit == true){
				// ���~�b�g�G���[����
				mEquipmentHistoryTemp.push_back(data);
			}
		}
		if (data.message >= 40000 && data.message <= 49999){
			// ���~�b�g�G���[����
			mEquipmentHistoryTemp.push_back(data);
		}
		if (data.message >= 60000 && data.message <= 69999){
			// �C�x���g����
			mEventHistoryTemp.push_back(data);
		}
	}
	//=====================================================//
	//������������������������ Log ������������������������//
	CString msg;
	msg.Format(_T("%s %s %s %s %s %s"), (LPCTSTR)strDate, (LPCTSTR)strTime, (LPCTSTR)strSend, (LPCTSTR)strMsgid, (LPCTSTR)strLevel, (LPCTSTR)strMsg);
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//

	DWORD dwElapsed = (timeGetTime() - dwStart);
	TRACE("### CreateDataToHistory Elapsed(%d)\n", dwElapsed);
	CGraphMutex::Unlock(eData);
}

/*============================================================================*/
/*! �f�[�^���W

-# �����R�[�h�ϊ����s��

@param	src		�@�@UTF-8�e�L�X�g

@retval	Cstring		UNICODE�e�L�X�g
*/
/*============================================================================*/
CString COperationTimeLineApp::ConvUtf8Unicode(std::string& src)
{
	//UTF-16�ϊ���̃T�C�Y���擾
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0U, src.data(), int(src.size()) + 1, nullptr, 0U);
	if (lenghtUnicode <= 0)
	{
		//=====================================================//
		//������������������������ Log ������������������������//
		CString msg;
		msg.Format(_T("Code Error : MultiByteToWideChar Length(%d)"), lenghtUnicode);
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvUtf8Unicode"), msg, _T("detail"), nLogEx::detail);
		//������������������������ Log ������������������������//
		//=====================================================//
		return _T("");
	}

	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];
	//UTF-8����UTF-16�֕ϊ�
	MultiByteToWideChar(CP_UTF8, 0, src.data(), int(src.size()) + 1, bufUnicode, lenghtUnicode);

	CString strUnicode(bufUnicode);
	delete[] bufUnicode;

	return strUnicode;
}

/*============================================================================*/
/*! �f�[�^���W

-# ���b�Z�[�W���e����C�x���g�������̃`�F�b�N���s��

@param	pmsg	���b�Z�[�W������

@retval bool
*/
/*============================================================================*/
bool COperationTimeLineApp::IsMessageEvent(TCHAR* pmsg)
{
	const CString mMessageEvent[] = {
		{ _T("[�C�x���g���s]") },
		{ _T("[�V�[�P���X�Ǘ�]") },
		{ _T("[TLM�^�p]") },
		{ _T("[CMD�^�p]") },
		{ _T("[RNG�^�p]") },
		{ _T("[�ϒ�����Ǘ�]") },
		{ _T("[UPLINK�^�p]") },
		{ _T("[����o�̓T�u]") },
		{ _T("[S-DL�ߑ�]") },
		{ _T("[X-DL�ߑ�]") },
		{ _T("[Ka-DL�ߑ�]") },
	};
	CString msg = CString(pmsg);
	for (int i = 0; i < sizeof(mMessageEvent) / sizeof(mMessageEvent[0]); i++){
		if (msg.Find(mMessageEvent[i]) >= 0){
			// �C�x���g�����̕����񂪌�������
			return true;
		}
	}
	return false;
}

/*============================================================================*/
/*! �f�[�^���W

-# ���b�Z�[�W���e����C�x���g�������̃`�F�b�N���s������

@param	msgno	���b�Z�[�W�ԍ�

@retval	bool
*/
/*============================================================================*/
bool COperationTimeLineApp::IsMessageLimit(DWORD msgno)
{
	CString msg;
	msg.Format(_T("%d"), msgno);
	if (msg.Right(1) == _T("9") || msg.Right(1) == _T("0")){
		return true;
	}
	return false;
}

/*============================================================================*/
// ���������䃂�[�h�̐ݒ�
/*============================================================================*/
void COperationTimeLineApp::SetLimitedMode()
{
	char host_name[256]; //�z�X�g��

	m_nLimitedMode = eLimitedMode_Disable;

	if(theApp.GetSelectStation() != eStation_Usuda642)
	{
		return;		//�A�v��54m�p�ȊO
	}

	if(theApp.GetExecuteMode() != eExecuteMode_Control)
	{
		return;		//���䃂�[�h�ȊO
	}

	if (mServiceMode == eServiceMode_limited)
	{
		// INI�t�@�C���ݒ�ɂ��f�o�b�O�p�̐������[�h�ݒ�
		m_nLimitedMode = eLimitedMode_Enable;
		return;
	}
	else if (mServiceMode == eServiceMode_unlimited)
	{
		// INI�t�@�C���ݒ�ɂ�鐧�����[�h�����ݒ�̂��߁A�������Ȃ�
		return;
	}

	if(gethostname(host_name, sizeof(host_name)) != 0)
	{
		return;
	}

	CString strHost = CString(host_name);
	if(strHost.Left(8).CompareNoCase(mHostNameUsuda) == 0)	//�z�X�g�� �P�c�F����Ԋϑ���(�P�c64m�ǎɓ�)
	{
		m_nLimitedMode = eLimitedMode_Enable;
	}

	return;
}

/*============================================================================*/
/*! �f�[�^���W

-# �T�����M�����f�[�^��o�^����

@param

@retval bool
*/
/*============================================================================*/
UINT COperationTimeLineApp::AddHistory(UINT message, WPARAM wParam, LPARAM lParam)
{
	CGraphMutex::Lock(eData);
	UINT	status = eHistStatus_Normal;

	try
	{
		// �ꎞ�I�Ɋi�[���ꂽ�f�[�^���P�v�̈�Ɋi�[����
		while (!mEquipmentHistoryTemp.empty())
		{
			stHistoryData newData = mEquipmentHistoryTemp.front();
			mEquipmentHistoryTemp.pop_front();
			if (mEquipmentHistoryData.size() == MAX_HISTORY_DATA){
				mEquipmentHistoryData.pop_front();
			}
			mEquipmentHistoryData.push_back(newData);
			status = __max((BYTE)status, newData.status);
		}

		while (!mEventHistoryTemp.empty())
		{
			stHistoryData newData = mEventHistoryTemp.front();
			mEventHistoryTemp.pop_front();
			if (mEventHistoryData.size() == MAX_HISTORY_DATA){
				mEventHistoryData.pop_front();
			}
			mEventHistoryData.push_back(newData);
			status = __max((BYTE)status, newData.status);
		}
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);

	return status;
}
/*============================================================================*/
/*! �A�v���P�[�V����

-# ���䑗�M

@brief	���䑗�M

@param	val1: ����Ώ�
@param	val2~4: �p�����[�^
@param	bWaitComplete�FTRUE:�V�F���I����҂i�f�t�H���g�j / FALSE:�V�F���I����҂��Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void COperationTimeLineApp::SendControl(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete /*= TRUE*/)
{
	CString ctrl = val1;
	if (val2 != _T(""))
	{
		ctrl += _T(" ") + val2;
		if (val3 != _T(""))
		{
			ctrl += _T(" ") + val3;
			if (val4 != _T(""))
			{
				ctrl += _T(" ") + val4;
			}
		}
	}

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = SEND_CONTROL_SH;

	if (theApp.GetSelectStation() == eStation_Usuda64)
		stParam.strRemShellParam.Format(_T("udsc64 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Usuda642)
		stParam.strRemShellParam.Format(_T("udsc54 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
		stParam.strRemShellParam.Format(_T("usc34 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
		stParam.strRemShellParam.Format(_T("usc20 %s"), (LPCTSTR)ctrl);

	CString msg;
	msg.Format(_T("SendControl(%s)!!!"), (LPCTSTR)stParam.strRemShellName);
	CLogTraceEx::Write(_T(""), _T("COperationTimeLineApp"), _T("SendControl"), msg, _T(""), nLogEx::info);

	int err = CSshShell::ExecShell(stParam, bWaitComplete);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("COperationTimeLineApp"), _T("SendControl"), _T("debug"), strDebug, nLogEx::debug);
	}
}

void COperationTimeLineApp::SendControlNoUI(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete /*= TRUE*/)
{
	CString ctrl = val1;
	if (val2 != _T(""))
	{
		ctrl += _T(" ") + val2;
		if (val3 != _T(""))
		{
			ctrl += _T(" ") + val3;
			if (val4 != _T(""))
			{
				ctrl += _T(" ") + val4;
			}
		}
	}

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = SEND_CONTROL_NOUI_SH;

	if (theApp.GetSelectStation() == eStation_Usuda64)
		stParam.strRemShellParam.Format(_T("udsc64 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Usuda642)
		stParam.strRemShellParam.Format(_T("udsc54 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
		stParam.strRemShellParam.Format(_T("usc34 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
		stParam.strRemShellParam.Format(_T("usc20 %s"), (LPCTSTR)ctrl);

	CString msg;
	msg.Format(_T("SendControl(%s)!!!"), (LPCTSTR)stParam.strRemShellName);
	CLogTraceEx::Write(_T(""), _T("COperationTimeLineApp"), _T("SendControlNoUI"), msg, _T(""), nLogEx::info);

	int err = CSshShell::ExecShell(stParam, bWaitComplete);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("COperationTimeLineApp"), _T("SendControlNoUI"), _T("debug"), strDebug, nLogEx::debug);
	}
}


/*============================================================================*/
/*! �A�v���P�[�V����

-# �ݔ��A���[���󋵈ꗗ�̍폜

@param

@retval void
*/
/*============================================================================*/
UINT COperationTimeLineApp::GetDrawUpdateInterval(bool bRead)
{
	if (bRead == true){
		m_nDrawUpdateInterval = GetPrivateProfileInt(SECTION_SYSTEM, KEY_DRAWUPDATEINTERVAL, 500, m_strAppInifileName);
	}
	return m_nDrawUpdateInterval;
}
