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

// RemoteGraph.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "RemoteGraph.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "RemoteGraphDoc.h"
#include "RemoteGraphView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRemoteGraphApp

BEGIN_MESSAGE_MAP(CRemoteGraphApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CRemoteGraphApp::OnAppAbout)
	// �W���̃t�@�C����{�h�L�������g �R�}���h
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// CRemoteGraphApp �R���X�g���N�V����

CRemoteGraphApp::CRemoteGraphApp()
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
	SetAppID(_T("RemoteGraph.AppID.NoVersion"));

//	for(int i=0; i<MAX_VIEW; i++)
//		m_TrendScrollMode[i] = false;

	m_nSelectEquipment = 0;

	m_bAnalysisMigration = 0x0;

	m_bDiskFreeSpace = true;
	m_bCheckDiskFreeSpace = true;
}

// �B��� CRemoteGraphApp �I�u�W�F�N�g�ł��B

CRemoteGraphApp theApp;


// CRemoteGraphApp ������

BOOL CRemoteGraphApp::InitInstance()
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

	//============================================================================
	// �R�}���h���C���̉��
	ParseCommandLine(m_cmdInfo);

	// �R�}���h���C����͂̌��ʁA�u1�ǃ��[�h����SELECT�v�̏ꍇ�͋ǑI���_�C�A���O��\������
	m_nDefSelectMode = m_nSelectMode = m_cmdInfo.mStationMode;
	m_nDefSelectStation = m_nSelectStation = m_cmdInfo.mStation;
	m_strFileServerName = m_cmdInfo.mServerName;
	//============================================================================

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
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_RemoteGraphTYPE,
		RUNTIME_CLASS(CRemoteGraphDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CRemoteGraphView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// �A�v���P�[�V�����̏�����
	_InitializeAppl();

	// DATA�t�H���_���̃t�@�C�����폜
	DeleteFileFromDataFolderAll();

	m_DataCollection.RequestDataList();

	// ���C�� MDI �t���[�� �E�B���h�E���쐬���܂��B
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// DDE�Afile open �ȂǕW���̃V�F�� �R�}���h�̃R�}���h ���C������͂��܂��B
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

#if 1 // �� �N�����̐V�����q�E�B���h�E�̍쐬���s��Ȃ�
	// Don't display a new MDI child window during startup
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
#endif

	// �R�}���h ���C���Ŏw�肳�ꂽ�f�B�X�p�b�` �R�}���h�ł��B�A�v���P�[�V������
	// /RegServer�A/Register�A/Unregserver �܂��� /Unregister �ŋN�����ꂽ�ꍇ�AFalse ��Ԃ��܂��B
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// ���C�� �E�B���h�E�����������ꂽ�̂ŁA�\���ƍX�V���s���܂��B
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	// �A�v���P�[�V������������
	m_pMainWnd->PostMessage(eMessage_InitializeApplication, 0);

	{
		CString	sMsg;
		sMsg.Format(_T("MODE=%d, STATION=%d"), m_nSelectMode, m_nSelectStation);
		TRACE(_T("RemoteGraph %s"), sMsg);
		CLogTraceEx::Write(_T(""), _T("CRemoteGraphApp"), _T("InitInstance"), _T("RemoteGraph"), sMsg, nLogEx::info);
	}

	return TRUE;
}

int CRemoteGraphApp::ExitInstance()
{
	// DATA�t�H���_���̃t�@�C�����폜
	DeleteFileFromDataFolderAll();

	//=====================================================//
	//������������������������ Log ������������������������//
	if (m_nLogLevel < nLogEx::none){
		CLogTraceEx::Write(_T(""), _T("CRemoteGraphApp"), _T("ExitInstance"), _T("RemoteGraph"), _T("Stop"), nLogEx::info);
		CLogTraceEx::Close();
		CLogTraceEx::Delete();
	}
	//������������������������ Log ������������������������//
	//=====================================================//

	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

/*============================================================================*/
/*! �A�v���P�[�V����

	-# WORK�t�H���_���N���A

	@param	
	@retval 
*/
/*============================================================================*/
void CRemoteGraphApp::DeleteFileFromDataFolder(int nView)
{
	static const CString search_file[] =
	{
		_T("%s\\%s%s*.spd"),
	};

	HANDLE	hFind;
	WIN32_FIND_DATA	wData;

	for( int i = 0; i < sizeof(search_file) / sizeof(search_file[0]); i++ )
	{
		CString	szSearch;

		if( nView == -1 )
			szSearch.Format(search_file[i], theApp.GetAppDataPath(), REALTIME_FNAME, _T(""));
		else
		{
			CString	str;
			str.Format(_T("%d"), nView + 1);
			szSearch.Format(search_file[i], theApp.GetAppDataPath(), ANALYSIS_FNAME, str);
		}

		if( (hFind = FindFirstFile(szSearch, &wData)) == INVALID_HANDLE_VALUE )
			continue;

		do
		{
			if( !(wData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
			{
				CString	str = m_strAppDataPath + _T("\\") + wData.cFileName;
				DeleteFile(str);
			}
		}
		while( FindNextFile(hFind, &wData) );

		FindClose(hFind);
	}
}
void CRemoteGraphApp::DeleteFileFromDataFolderAll()
{
	static const CString search_file[] =
	{
		_T("\\*.spd"),
	};

	HANDLE	hFind;
	WIN32_FIND_DATA	wData;

	for( int i = 0; i < sizeof(search_file) / sizeof(search_file[0]); i++ )
	{
		CString	szSearch = theApp.GetAppDataPath() + search_file[i];

		if( (hFind = FindFirstFile(szSearch, &wData)) == INVALID_HANDLE_VALUE )
			continue;

		do
		{
			if( !(wData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
			{
				CString	str = m_strAppDataPath + _T("\\") + wData.cFileName;
				DeleteFile(str);
			}
		}
		while( FindNextFile(hFind, &wData) );

		FindClose(hFind);
	}
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# �A�v���P�[�V�����̏��������s��

@param
@retval

*/
/*============================================================================*/
void CRemoteGraphApp::_InitializeAppl()
{
	// �e��t�@�C���E�t�H���_�̍쐬
	TCHAR	szFullPath[_MAX_PATH];
	TCHAR	szDir[_MAX_DIR];
	TCHAR	szDrive[_MAX_DRIVE];
	TCHAR	szFname[_MAX_FNAME];

	if( ::GetModuleFileName(NULL, szFullPath, _MAX_PATH) == 0 )
		return;

	_wsplitpath_s(szFullPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, NULL, 0);
	m_strAppPath.Format(_T("%s%s"), szDrive, szDir);
	m_strAppInifileName.Format(_T("%s%s%s.ini"), szDrive, szDir, szFname);

	// D�h���C�u�̑��݂��m�F
	if( PathIsDirectory(_T("D:\\")) )
	{
		// DATA�t�H���_���쐬
		m_strAppDataPath.Format(_T("D:\\%s"), theApp.m_pszAppName);
		CreateDirectory(m_strAppDataPath, NULL);
		m_strAppDataPath.Format(_T("D:\\%s\\DATA"), theApp.m_pszAppName);
		CreateDirectory(m_strAppDataPath, NULL);

		// LOG�t�H���_���쐬
		m_strAppLogPath.Format(_T("D:\\%s\\LOG\\"), theApp.m_pszAppName);
		CreateDirectory(m_strAppLogPath, NULL);
	}
	else
	{
		// DATA�t�H���_���쐬
		m_strAppDataPath.Format(_T("%s%sDATA"), szDrive, szDir);
		CreateDirectory(m_strAppDataPath, NULL);

		// LOG�t�H���_���쐬
		m_strAppLogPath.Format(_T("%s%sLOG\\"), szDrive, szDir);
		CreateDirectory(m_strAppLogPath, NULL);
	}

	TCHAR buf[MAX_PATH];
	// �f�B�X�N�󂫗e�ʃ`�F�b�N�L��
	GetPrivateProfileString(SECTION_SYSTEM, KEY_CHECKDISKFREESPACE, _T("ON"), buf, MAX_PATH, m_strAppInifileName);
	CString strChkDiskSpace = buf;
	if (strChkDiskSpace == _T("OFF"))
	{
		m_bCheckDiskFreeSpace = false;
	}
	else
	{
		m_bCheckDiskFreeSpace = true;
	}

//	char szLocal[MAX_PATH];
//	CCommBase::GetLocalAddress(szLocal, MAX_PATH);
//	CString strLocal(szLocal);

//	TCHAR buf[MAX_PATH];
//	GetPrivateProfileString(SECTION_SYSTEM, KEY_SERVER, strLocal, buf, _countof(buf), m_strAppInifileName);
//	m_strServerAddress = buf;

//	GetPrivateProfileString(SECTION_SYSTEM, KEY_SERVERPATH, m_strAppDataPath, buf, _countof(buf), m_strAppInifileName);
//	m_strServerFilePath = buf;

	m_nLogLevel = GetPrivateProfileInt(SECTION_SYSTEM, KEY_LOGLEVEL, nLogEx::fatal, m_strAppInifileName);
	CLogTraceEx::Create(m_strAppLogPath, _T("RemoteGraph"), m_nLogLevel, nLogEx::text);

	// �v���p�e�B�f�[�^��INI�t�@�C�����畜��
	theApp.ReadAllPropertiesData(theApp.GetAppInifileName());

	// �f�[�^�擾�Ώۃt���O��ݒ�
	theApp.GetDataCollection().SetSelectDataValid();

	//=====================================================//
	//������������������������ Log ������������������������//
	CString	file_name, path_name = m_pszExeName;
	file_name = PathFindFileName(path_name);
	CLogTraceEx::Write(_T(""), _T("CRemoteGraphApp"), _T("_InitializeAppl"), file_name, _T("Start"), nLogEx::info);
	//������������������������ Log ������������������������//
	//=====================================================//
}



// CRemoteGraphApp ���b�Z�[�W �n���h���[


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
void CRemoteGraphApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CRemoteGraphApp �̃J�X�^�}�C�Y���ꂽ�ǂݍ��݃��\�b�h�ƕۑ����\�b�h

void CRemoteGraphApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
//	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CRemoteGraphApp::LoadCustomState()
{
}

void CRemoteGraphApp::SaveCustomState()
{
}

// CRemoteGraphApp ���b�Z�[�W �n���h���[

#if 0

void CRemoteGraphApp::OpenFile(CString strFileName)
{
	((CMainFrame*)m_pMainWnd)->OpenFile(strFileName);
}

#endif // 0

/*============================================================================*/
/*! �A�v���P�[�V����

-# �t�@�C���p�X�̎擾

@brief ���L�t�H���_�̃t�@�C���p�X�܂��̓t�H���_�p�X���擾����

@param	fileType	�t�@�C���^�C�v
@param	nStation	�ǔԍ��i�ȗ����́A�P�ǉ�ʂŏȗ����͎��ǂ��A�T�ǉ�ʂł͉P�c64m�ǂ�Ԃ��j
@retval �t�@�C���p�X�܂��̓t�H���_�p�X
*/
/*============================================================================*/
CString CRemoteGraphApp::GetShareFilePath(UINT fileType, UINT nStation/* = eStation_MAX*/)
{
	// �ǔԍ��̈����ȗ���
	if (nStation == eStation_MAX)
	{
		// �ǃ��[�h�`�F�b�N
		switch (m_nSelectMode)
		{
		case eStation_1:
			// ���ǔԍ����Z�b�g
			nStation = m_nSelectStation;
			break;

		case eStation_5:
			// �P�c64m���Z�b�g
			nStation = eStation_Usuda64;
			break;

		case eStation_10:
			// 10�ǃ��[�h
			nStation = eStation_Usuda64;
			break;

		default:
			nStation = eStation_Usuda64;
			break;
		}
	}

	// �ǔԍ��ƃt�@�C���^�C�v����Ǖʃt�@�C���E�t�H���_�p�X���擾
	CString strFilePath;
	//	strFilePath.Format(_T("%s%s"), mNetworkDrive[nStation], mSvFileFolder[fileType]);
	strFilePath.Format(_T("\\\\%sshare\\%s\\%s"), (LPCTSTR)theApp.GetFileServerName(), mStationFolder[nStation], mSvFileFolder[fileType]);

	if( m_strFileServerName.IsEmpty() == true )
	{
		strFilePath.Format(_T("C:\\share\\%s\\%s"), mStationFolder[nStation], mLocalFileFolder[fileType]);
	}
#if 0 // �t�@�C���̑��݂��`�F�b�N����Ȃ牺�L�R�[�h��L����
	else
	{
		CFile		f;
		CFileStatus	s;
		if( f.GetStatus(strFilePath, s) == 0 )
			strFilePath.Format(_T("C:\\share\\%s\\%s"), mStationFolder[nStation], mLocalFileFolder[fileType]);
	}
#endif

	return strFilePath;
}

/*============================================================================*/
/*! �A�v���P�[�V����

-#  ��IP�A�h���X�̎擾

@param

@retval

*/
/*============================================================================*/
DWORD CRemoteGraphApp::GetHostIpInfo()
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
