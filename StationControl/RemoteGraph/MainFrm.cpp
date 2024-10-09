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

// MainFrm.cpp : CMainFrame �N���X�̎���
//

#include "stdafx.h"
#include "RemoteGraph.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "DataSelectDlg.h"
#include "HookMsgBox.h"
#include "WaitDialog.h"

// timeGetTime�g�p
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_BUTTON_ONLINE, &CMainFrame::OnButtonOnline)
	ON_COMMAND(ID_EDIT_STATION, &CMainFrame::OnButtonStation)
	ON_COMMAND(ID_CHECK_GRAPH1, &CMainFrame::OnCheckGraph1)
	ON_COMMAND(ID_CHECK_GRAPH2, &CMainFrame::OnCheckGraph2)
	ON_COMMAND(ID_CHECK_GRAPH3, &CMainFrame::OnCheckGraph3)
	ON_COMMAND(ID_CHECK_GRAPH4, &CMainFrame::OnCheckGraph4)
	ON_COMMAND(ID_CHECK_GRAPH5, &CMainFrame::OnCheckGraph5)
	ON_COMMAND(ID_CHECK_GRAPH6, &CMainFrame::OnCheckGraph6)
	ON_COMMAND(ID_CHECK_GRAPH7, &CMainFrame::OnCheckGraph7)
	ON_COMMAND(ID_CHECK_GRAPH8, &CMainFrame::OnCheckGraph8)
	ON_COMMAND(ID_CHECK_GRAPH9, &CMainFrame::OnCheckGraph9)
	ON_UPDATE_COMMAND_UI(ID_CHECK_GRAPH1, &CMainFrame::OnUpdateCheckGraph1)
	ON_UPDATE_COMMAND_UI(ID_CHECK_GRAPH2, &CMainFrame::OnUpdateCheckGraph2)
	ON_UPDATE_COMMAND_UI(ID_CHECK_GRAPH3, &CMainFrame::OnUpdateCheckGraph3)
	ON_UPDATE_COMMAND_UI(ID_CHECK_GRAPH4, &CMainFrame::OnUpdateCheckGraph4)
	ON_UPDATE_COMMAND_UI(ID_CHECK_GRAPH5, &CMainFrame::OnUpdateCheckGraph5)
	ON_UPDATE_COMMAND_UI(ID_CHECK_GRAPH6, &CMainFrame::OnUpdateCheckGraph6)
	ON_UPDATE_COMMAND_UI(ID_CHECK_GRAPH7, &CMainFrame::OnUpdateCheckGraph7)
	ON_UPDATE_COMMAND_UI(ID_CHECK_GRAPH8, &CMainFrame::OnUpdateCheckGraph8)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ONLINE, &CMainFrame::OnUpdateButtonOnline)
	ON_UPDATE_COMMAND_UI(ID_EDIT_STATION, &CMainFrame::OnUpdateButtonStation)
	ON_COMMAND(ID_VIEW_PROPERTIES, &CMainFrame::OnViewProperties)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTIES, &CMainFrame::OnUpdateViewProperties)
	ON_COMMAND(ID_BUTTON_ONLINE_STOP, &CMainFrame::OnButtonOnlineStop)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ONLINE_STOP, &CMainFrame::OnUpdateButtonOnlineStop)
	ON_COMMAND(ID_VIEW_DATALIST, &CMainFrame::OnViewDatalist)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DATALIST, &CMainFrame::OnUpdateViewDatalist)
	ON_COMMAND(ID_VIEW_OUTPUT, &CMainFrame::OnViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, &CMainFrame::OnUpdateViewOutput)
	ON_COMMAND(ID_VIEW_LIMIT, &CMainFrame::OnViewLimit)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIMIT, &CMainFrame::OnUpdateViewLimit)
	ON_COMMAND(ID_VIEW_STATUS, &CMainFrame::OnViewStatus)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS, &CMainFrame::OnUpdateViewStatus)
	ON_COMMAND(ID_BTN_GRAPH_CLEAR1, &CMainFrame::OnBtnGraphClear1)
	ON_COMMAND(ID_BTN_GRAPH_CLEAR2, &CMainFrame::OnBtnGraphClear2)
	ON_COMMAND(ID_BTN_GRAPH_CLEAR3, &CMainFrame::OnBtnGraphClear3)
	ON_COMMAND(ID_BTN_GRAPH_CLEAR4, &CMainFrame::OnBtnGraphClear4)
	ON_COMMAND(ID_BTN_GRAPH_CLEAR5, &CMainFrame::OnBtnGraphClear5)
	ON_COMMAND(ID_BTN_GRAPH_CLEAR6, &CMainFrame::OnBtnGraphClear6)
	ON_COMMAND(ID_BTN_GRAPH_CLEAR7, &CMainFrame::OnBtnGraphClear7)
	ON_COMMAND(ID_BTN_GRAPH_CLEAR8, &CMainFrame::OnBtnGraphClear8)
	ON_UPDATE_COMMAND_UI(ID_BTN_GRAPH_CLEAR1, &CMainFrame::OnUpdateBtnGraphClear1)
	ON_UPDATE_COMMAND_UI(ID_BTN_GRAPH_CLEAR2, &CMainFrame::OnUpdateBtnGraphClear2)
	ON_UPDATE_COMMAND_UI(ID_BTN_GRAPH_CLEAR3, &CMainFrame::OnUpdateBtnGraphClear3)
	ON_UPDATE_COMMAND_UI(ID_BTN_GRAPH_CLEAR4, &CMainFrame::OnUpdateBtnGraphClear4)
	ON_UPDATE_COMMAND_UI(ID_BTN_GRAPH_CLEAR5, &CMainFrame::OnUpdateBtnGraphClear5)
	ON_UPDATE_COMMAND_UI(ID_BTN_GRAPH_CLEAR6, &CMainFrame::OnUpdateBtnGraphClear6)
	ON_UPDATE_COMMAND_UI(ID_BTN_GRAPH_CLEAR7, &CMainFrame::OnUpdateBtnGraphClear7)
	ON_UPDATE_COMMAND_UI(ID_BTN_GRAPH_CLEAR8, &CMainFrame::OnUpdateBtnGraphClear8)
	ON_WM_CLOSE()
	ON_COMMAND(ID_BUTTON_CSVOUT, &CMainFrame::OnButtonCsvOut)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CSVOUT, &CMainFrame::OnUpdateButtonCsvOut)
	ON_COMMAND(ID_BUTTON_PRINTOUT, &CMainFrame::OnButtonPrintOut)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_PRINTOUT, &CMainFrame::OnUpdateButtonPrintOut)
	ON_COMMAND(ID_BUTTON_FILE_OPEN, &CMainFrame::OnButtonFileOpen)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_FILE_OPEN, &CMainFrame::OnUpdateButtonFileOpen)
	ON_COMMAND(ID_BUTTON_FILE_SAVE, &CMainFrame::OnButtonFileSave)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_FILE_SAVE, &CMainFrame::OnUpdateButtonFileSave)
END_MESSAGE_MAP()

// CMainFrame �R���X�g���N�V����/�f�X�g���N�V����

CMainFrame::CMainFrame()
{
	// TODO: �����o�[�������R�[�h�������ɒǉ����Ă��������B
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
	m_bOnline = FALSE;
//	m_bGraph[grapf1] = TRUE;
	m_bGraph[grapf1] = FALSE;
	m_bGraph[grapf2] = FALSE;
	m_bGraph[grapf3] = FALSE;
	m_bGraph[grapf4] = FALSE;
	m_bGraph[grapf5] = FALSE;
	m_bGraph[grapf6] = FALSE;
	m_bGraph[grapf7] = FALSE;
	m_bGraph[grapf8] = FALSE;
	m_bGraph[grapf9] = FALSE;
	m_CreatingView = 0;

	m_pCheckDiskThread = NULL;
	m_bCheckDiskStop = false;

//	m_pDlgWait = NULL;
}

CMainFrame::~CMainFrame()
{
}

UINT CheckDiskThread(LPVOID pParam)
{
	CMainFrame	*pWnd = (CMainFrame *)pParam;

	TRACE(_T("=== CheckDiskThread Start\n"));
	CLogTraceEx::Write(_T(""), _T("CheckDiskThread"), _T("Thread"), _T("Thread Start"), _T(""), nLogEx::debug);

	// �ŏ��^�C�}����\��ݒ�(1ms)
	timeBeginPeriod(1);

	// �N�����͏�Ƀ`�F�b�N
	while( !pWnd->m_bCheckDiskStop )
	{
		bool	bSpaceOk = true;

		// �h���C�u���𒊏o
		int index = theApp.GetAppDataPath().Find(_T(":"));
		if( index == -1 )
			continue;

		CString	szDrive = theApp.GetAppDataPath().Left(index + 1) + _T("\\");

		// �f�B�X�N�����擾
		ULARGE_INTEGER	nFree;
		ULARGE_INTEGER	nAvail;
		ULARGE_INTEGER	nTotal;

		if( GetDiskFreeSpaceEx(szDrive, &nFree, &nTotal, &nAvail) != 0 )
		{
			// �󂫗e�ʂ��m�F
			if( nTotal.QuadPart != 0 )
			{
				if( ((nAvail.QuadPart * 100) / nTotal.QuadPart) < DISK_FREE_SPACE )
					bSpaceOk = false;

				// �f�B�X�N�󂫗e�ʃt���O��ݒ�
				theApp.SetDiskFreeSpace(bSpaceOk);
			}
		}

		//Sleep(20000);
		const UINT mSleepTime = 20000;
		DWORD dwStart = timeGetTime();
		DWORD dwElapsed = 0;
		while (!pWnd->m_bCheckDiskStop){
			dwElapsed = (timeGetTime() - dwStart);
			if (dwElapsed >= mSleepTime)
				break;
			Sleep(100);
		}
	}

	TRACE(_T("=== CheckDiskThread End\n"));
	CLogTraceEx::Write(_T(""), _T("CheckDiskThread"), _T("Thread"), _T("Thread End"), _T(""), nLogEx::debug);

	return 0;
}

#if 0
/*
UINT CsvOutThread(LPVOID pParam)
{
	CMainFrame	*pWnd = (CMainFrame *)pParam;

	TRACE(_T("=== CsvOutThread Start\n"));
	CLogTraceEx::Write(_T(""), _T("CsvOutThread"), _T("Thread"), _T("Thread Start"), _T(""), nLogEx::debug);

	// CSV�t�@�C���ɏo��
	theApp.GetDataCollection().OutputCsvFile(pWnd->GetActiveGraph(), pWnd->m_szCsvFile);

	// �_�C�A���O�̏I����ʒm
//	pWnd->PostMessage(eMessage_CloseWaitDialog, 0, 0);

	TRACE(_T("=== CsvOutThread End\n"));
	CLogTraceEx::Write(_T(""), _T("CsvOutThread"), _T("Thread"), _T("Thread End"), _T(""), nLogEx::debug);

	return 0;
}
*/
#endif

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	// �L���v�`���ɑΉ�����܂Ŕ�\��
	{
		CMFCRibbonCategory	*pCategory = m_wndRibbonBar.GetCategory(0);
		if( pCategory != NULL )
		{
			CMFCRibbonPanel	*pPanel = pCategory->GetPanel(2);
			if( pPanel != NULL )
				pPanel->Remove(5);
		}
	}
//#ifndef	_DEBUG	// ���{���ɕ\������ꍇ�͗L����!!
	// ���A���^�C���̊J�n�����^�I���������\��
	{
		CMFCRibbonCategory	*pCategory = m_wndRibbonBar.GetCategory(0);
		if( pCategory != NULL )
		{
			CMFCRibbonPanel	*pPanel = pCategory->GetPanel(3);
			if( pPanel != NULL )
			{
				pPanel->Remove(4);
				pPanel->Remove(3);
			}
		}
	}
//#endif	// DEBUG

#if 0
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("�X�e�[�^�X �o�[�̍쐬�Ɏ��s���܂����B\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}
#endif

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);

#if 0
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);
#endif

	// Visual Studio 2005 �X�^�C���̃h�b�L���O �E�B���h�E�����L���ɂ��܂�
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 �X�^�C���̃h�b�L���O �E�B���h�E�̎�����\�������L���ɂ��܂�
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ���j���[���ڃC���[�W (�ǂ̕W���c�[�� �o�[�ɂ��Ȃ��C���[�W) ��ǂݍ��݂܂�:
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// �h�b�L���O �E�B���h�E���쐬���܂�
	if (!CreateDockingWindows())
	{
		TRACE0("�h�b�L���O �E�B���h�E���쐬�ł��܂���ł���\n");
		return -1;
	}

	// �e��E�B���h�E���h�b�L���O
	CDockablePane* pTabbedBar = NULL;

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);

	m_wndStatusView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndStatusView);

	m_wndLimit.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndLimit);

	m_wndOutput.DockToWindow(&m_wndStatusView, CBRS_ALIGN_LEFT);
	m_wndLimit.AttachToTabWnd(&m_wndStatusView, DM_SHOW, FALSE, &pTabbedBar);

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);

	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);

	CRect	rc;

	// �e��E�B���h�E�̃��C�A�E�g�𒲐�
	m_wndFileView.GetWindowRect(rc);
	m_wndFileView.SetWindowPos(NULL, 0, 0, 300, rc.Height(), SWP_NOMOVE | SWP_NOOWNERZORDER);

	m_wndProperties.GetWindowRect(rc);
	m_wndProperties.SetWindowPos(NULL, 0, 0, 300, rc.Height(), SWP_NOMOVE | SWP_NOOWNERZORDER);

	m_wndOutput.GetWindowRect(rc);
	m_wndOutput.SetWindowPos(NULL, 0, 0, rc.Width(), 205, SWP_NOMOVE | SWP_NOOWNERZORDER);

	// �Œ�l�Ɋ�Â��ăr�W���A�� �}�l�[�W���[�� visual �X�^�C����ݒ肵�܂�
	OnApplicationLook(theApp.m_nAppLook);

	// �g���E�B���h�E�Ǘ��_�C�A���O �{�b�N�X��L���ɂ��܂�
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// �f�B�X�N�󂫗e�ʃX���b�h���N��
	m_bCheckDiskStop = false;
	m_pCheckDiskThread = AfxBeginThread(CheckDiskThread, this);

	return 0;
}

void CMainFrame::OnClose()
{
	if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("�I�����܂��B\n��낵���ł����H"), theApp.m_pszAppName, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return;

	// �X���b�h���I��
	if (m_bOnline == TRUE && (theApp.GetDataCollection().GetSelectDataList().size() != 0))
	{
		theApp.GetDataCollection().StopCollection();
		m_bOnline = FALSE;
	}
	theApp.GetDataCollection().StopAllAnalysisDataThread();

	// INI�t�@�C���ɐݒ�󋵂�ۑ�
	SaveConfigFile(theApp.GetAppInifileName());

	// �f�B�X�N�󂫗e�ʃX���b�h���~
	if( m_pCheckDiskThread != NULL )
	{
		m_bCheckDiskStop = true;

		while( 1 )
		{
			if( WaitForSingleObject(m_pCheckDiskThread->m_hThread, 100) != WAIT_TIMEOUT )
				break;
		}

		m_pCheckDiskThread = NULL;
		m_bCheckDiskStop = false;
	}

	CMDIFrameWndEx::OnClose();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

#if 0
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;
#endif
	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

#if 0
	// �N���X �r���[���쐬���܂�
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("�N���X �r���[ �E�B���h�E���쐬�ł��܂���ł���\n");
		return FALSE; // �쐬�ł��܂���ł���
	}
#endif

	// �t�@�C�� �r���[���쐬���܂�
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("�t�@�C�� �r���[ �E�B���h�E���쐬�ł��܂���ł���\n");
		return FALSE; // �쐬�ł��܂���ł���
	}

	// �o�̓E�B���h�E���쐬���܂�
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 500, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("�o�̓E�B���h�E���쐬�ł��܂���ł���\n");
		return FALSE; // �쐬�ł��܂���ł���
	}

	// ���~�b�g�E�B���h�E���쐬���܂�
	CString strLimitWnd;
	bNameValid = strLimitWnd.LoadString(IDS_LIMIT_WND);
	ASSERT(bNameValid);
	if (!m_wndLimit.Create(strLimitWnd, this, CRect(0, 0, 500, 100), TRUE, ID_VIEW_LIMITWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("���~�b�g�E�B���h�E���쐬�ł��܂���ł���\n");
		return FALSE; // �쐬�ł��܂���ł���
	}

	// �v���p�e�B �E�B���h�E���쐬���܂�
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("�v���p�e�B �E�B���h�E���쐬�ł��܂���ł���\n");
		return FALSE; // �쐬�ł��܂���ł���
	}

	// �X�e�[�^�X �r���[���쐬���܂�
	CString strStatusView;
	bNameValid = strStatusView.LoadString(IDS_STATUS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndStatusView.Create(strStatusView, this, CRect(0, 0, 50, 200), TRUE, ID_VIEW_STATUSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("�X�e�[�^�X �r���[ �E�B���h�E���쐬�ł��܂���ł���\n");
		return FALSE; // �쐬�ł��܂���ł���
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

#if 0
	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);
#endif

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hLimitBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_LIMIT_WND_HC : IDI_LIMIT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndLimit.SetIcon(hLimitBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	HICON hStatusBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_STATUS_VIEW_HC : IDI_STATUS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndStatusView.SetIcon(hStatusBarIcon, FALSE);
}

// CMainFrame �f�f

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


void CMainFrame::OpenFile(CString strFileName)
{
	if (m_bOnline == TRUE)
		return;

	CWaitCursor wait;
	//theApp.GetSatelliteData().OpenFile(strFileName);

	BOOL ret = FALSE;
	CMDIChildWndEx* pChild = (CMDIChildWndEx*)MDIGetActive(&ret);
	if (pChild != NULL)
	{
		pChild->PostMessage(mMessageOpenFile, 0, 0);
	}

	//	INT n;
	//	n = 0;
	//	::EnumChildWindows(m_hWndMDIClient, EnumChildProc, (LPARAM)&n);
}

/*============================================================================*/
/*! ���C���t���[��

-# �ݒ����ǂݍ���

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OpenConfigFile(CString szFilePath)
{
	// ��M�Ď��f�[�^��������
	theApp.GetDataCollection().DeleteSpaceRecv();

	// DATA�t�H���_���̃t�@�C�����폜
	theApp.DeleteFileFromDataFolder(-1);

	// ���A���^�C�����[�h�̎�������������
	theApp.SetRealTimeStart(CTime(0));
	theApp.SetRealTimeLast(CTime(0));

	for( int i = 0; i < (MAX_VIEW * MAX_TDATA); i++ )
		theApp.SetRealTimeWriteData(i, CTime(0));
	theApp.SetRealTimeWriteMax(CTime(0));

	theApp.SetStartFile(0);

	// �r���[�֘A�̃f�[�^���폜
	theApp.GetDataCollection().DeleteCollectionAll();

#if 0
	// �v���p�e�B�ݒ�𔽉f
	for( UINT i = 0; i < MAX_VIEW; i++ )
		SendMessage(eMessage_PropetyChange, 1, (LPARAM)i);
#endif

	// �v���p�e�B�f�[�^��ݒ�t�@�C�����畜��
	theApp.ReadAllPropertiesData(szFilePath);

	// �ȈՃ��X�g��ύX
	GetPropertiesWnd().UpdateSimplicityList();

	// �f�[�^�擾�Ώۃt���O��ݒ�
	theApp.GetDataCollection().SetSelectDataValid();

	// �r���[��������
	SendMessage(eMessage_InitializeApplication, 1, (LPARAM)szFilePath.operator LPCWSTR());

#if 1
	// �v���p�e�B�ݒ�𔽉f
	for( UINT i = 0; i < MAX_VIEW; i++ )
	{
		map<UINT, LPARAM>::iterator itr = m_ChildWndList.find(i);
		if( itr != m_ChildWndList.end() )
			SendMessage(eMessage_PropetyChange, 1, (LPARAM)i);
	}
#endif
}

/*============================================================================*/
/*! ���C���t���[��

-# �ݒ����ۑ�

@param
@retval

*/
/*============================================================================*/
void CMainFrame::SaveConfigFile(CString szFilePath)
{
	CString	str;

	// �o�[�W������ۑ�
	str.Format(_T("%d"), FILE_VERSION);
	WritePrivateProfileString(SECTION_SYSTEM, KEY_FILEVERSION, str, szFilePath);

	// �Ď������X�g��ۑ�
	theApp.GetDataCollection().WriteDataNameList(szFilePath);

	// �v���p�e�B�f�[�^��ۑ�
	theApp.WriteAllPropertiesData(szFilePath);
}

/*============================================================================*/
/*! ���C���t���[��

-# �O���t�v���p�e�B�̍X�V

@param		lParam	�E�B���h�E�n���h��
@retval

*/
/*============================================================================*/
void CMainFrame::UpdataeProperties(LPARAM lParam)
{
}

/*============================================================================*/
/*! ���C���t���[��

-# �q�E�B���h�E�̓o�^�폜

@param		message	�E�B���h�E���b�Z�[�W
@param		wParam	�A�N�e�B�u���
@param		lParam	�E�B���h�E�n���h��
@retval

*/
/*============================================================================*/
void CMainFrame::ResetChildList(UINT message, WPARAM wParam, LPARAM lParam)
{
	CString strTitle = _T("");
	map<UINT, LPARAM>::iterator itr;

	for (itr = m_ChildWndList.begin(); itr != m_ChildWndList.end(); itr++){
		if (message == eMessage_ChildActivate){
			// �A�N�e�B�u��Ԃ̕ω�
			if ((*itr).second == lParam){
				if (wParam)	{
					m_nActiveGraph = (*itr).first;

					// �A�N�e�B�u�ȉ�ʂ̃v���p�e�B�ɕύX
					UINT pos = (*itr).first;
					m_wndProperties.ChangeView(pos);

					// �A�N�e�B�u�E�B���h�E�ԍ���ʒm����
					m_wndFileView.SetActiveNo(pos);

					// �A�N�e�B�u�ȉ�ʂ̏o�̓^�u�ɕύX
					m_wndOutput.UpdateTab(pos);

					// �A�N�e�B�u�ȉ�ʂ̃��~�b�g�^�u�ɕύX
					m_wndLimit.UpdateTab(pos);
				}
				return;
			}
		}
#if 0	// �����͒ʂ邱�Ƃ��Ȃ�
		else{
			// �I��
			if ((*itr).second == lParam){
				// ���X�g����폜
				UINT pos = (*itr).first;
				m_ChildWndList.erase(itr);
				if (m_nActiveGraph == pos)
				{
					m_nActiveGraph = 0;
				}

				// Property�폜
				m_wndProperties.CloseView(pos);
				// �o�̓^�u�폜
				m_wndOutput.DeleteTab(pos);
				return;
			}
		}
#endif
	}

#if 0	// �����͒ʂ邱�Ƃ��Ȃ�
	// �����ɂ����ꍇ�́A���X�g�ɓo�^����Ă��Ȃ�
	if (message == eMessage_ChildDestroy){
		// �������Ȃ�
		return;
	}
#endif

	// ��A�N�e�B�u�Ȃ�X���[
	if( !wParam )
		return;

	// �q�E�C���h�E���X�g�ɖ���(�܂菉��)�Ȃ�A���X�g�ɒǉ����ăv���p�e�B�^�^�u��ǉ�

//	UINT count = (int)m_ChildWndList.size();
	UINT count = (int)m_CreatingView;

	// �V�K�o�^
	m_ChildWndList.insert(map<UINT, LPARAM>::value_type(count, lParam));
	m_nActiveGraph = count;

	// Property �f�[�^�V�K�쐬
	m_wndProperties.SetNewPropertyData(count, lParam);

	// �f�[�^�擾�Ώۃt���O��ݒ�
	theApp.GetDataCollection().SetSelectDataValid();

	// �A�N�e�B�u�E�B���h�E�ԍ���ʒm����
	m_wndFileView.SetActiveNo(count);

	// �o�̓^�u��ǉ�
	m_wndOutput.AddTab(count);

	// ���~�b�g�^�u��ǉ�
	m_wndLimit.AddTab(count);
}


/*============================================================================*/
/*! ���C���t���[��

-# �e�q�E�B���h�E�ւ̃��b�Z�[�W�ʒm

@param		message	�E�B���h�E���b�Z�[�W
@param		wParam	�A�N�e�B�u���
@param		lParam	�E�B���h�E�n���h��
@retval		�Ȃ�

*/
/*============================================================================*/
void CMainFrame::PostMessageToChildList(UINT message, WPARAM wParam, LPARAM lParam)
{
	(void)wParam;
	(void)lParam;
	CString strTitle = _T("");
	map<UINT, LPARAM>::iterator itr;

	// �q�E�B���h�E�փ��b�Z�[�W�𑗂�
	for (itr = m_ChildWndList.begin(); itr != m_ChildWndList.end(); itr++)
	{
		((CChildFrame*)(*itr).second)->PostMessage(message, wParam, lParam);

		m_wndOutput.PostMessage(message, (*itr).first);

		m_wndLimit.PostMessage(message, (*itr).first);
	}
}

#if 0
/*============================================================================*/
/*! ���C���t���[��

-# �e�g�����h�r���[�E�B���h�E�ւ̍X�V�C�x���g�ʒm

@param		�Ȃ�
@retval		�Ȃ�

*/
/*============================================================================*/
void CMainFrame::SetTrendViewUpdateEvent()
{
	for (int i = 0; i < m_ChildWndList.size(); ++i)
	{
		for (int j = 0; j < eViewID_Num; ++j)
		{
			m_evtUpdate.SetEvent();
		}
	}
}
#endif

/*============================================================================*/
/*! ���C���t���[��

-# ���A���^�C�����[�h�̑���J�n�^�I��(����)�̎������X�V

@param		�Ȃ�
@retval		�Ȃ�

*/
/*============================================================================*/
void CMainFrame::SetRealTimeRibbon()
{
#if 0	// ���{���ɕ\������ꍇ�͗L����!!
#ifdef	_DEBUG
	CMFCRibbonCategory	*pCategory = m_wndRibbonBar.GetCategory(0);
	if( pCategory == NULL )
		return;

	CMFCRibbonPanel	*pPanel = pCategory->GetPanel(2);
	if( pPanel == NULL )
		return;

	CString	strYmd;
	CString	strHMS;

	CMFCRibbonLabel *pStartLabel = (CMFCRibbonLabel *)pPanel->GetElement(3);
	if( pStartLabel != NULL )
	{
		if( theApp.GetRealTimeStart() == CTime(0) )
		{
//			pStartLabel->SetText((LPCTSTR)_T("----/--/-- --:--:--"));
			pStartLabel->SetText((LPCTSTR)_T("0000/00/00 00:00:00"));
		}
		else
		{
			strYmd = theApp.GetRealTimeStart().Format("%Y/%m/%d");
			strHMS = theApp.GetRealTimeStart().Format("%H:%M:%S");
			pStartLabel->SetText((LPCTSTR)(strYmd + _T(" ") + strHMS));
		}
	}

	CMFCRibbonLabel *pLastLabel = (CMFCRibbonLabel *)pPanel->GetElement(4);
	if( pLastLabel != NULL )
	{
		if( theApp.GetRealTimeLast() == CTime(0) )
		{
//			pLastLabel->SetText((LPCTSTR)_T("----/--/-- --:--:--"));
			pLastLabel->SetText((LPCTSTR)_T("0000/00/00 00:00:00"));
		}
		else
		{
#if 0
			// ��͂ɂĈ����s�\�Ȏ�M����������\��
			strYmd = theApp.GetRealTimeLast().Format("%Y/%m/%d");
			strHMS = theApp.GetRealTimeLast().Format("%H:%M:%S");
#else
			// ��͂ɂĈ����\�ȏ������񂾎�����\��
			CTime	tmp = (theApp.GetRealTimeStart() == theApp.GetRealTimeWrite()) ? theApp.GetRealTimeWrite() : (theApp.GetRealTimeWrite() - CTimeSpan(0, 0, 0, 1));
			strYmd = tmp.Format("%Y/%m/%d");
			strHMS = tmp.Format("%H:%M:%S");
#endif
			pLastLabel->SetText((LPCTSTR)(strYmd + _T(" ") + strHMS));
		}
	}

	m_wndRibbonBar.Invalidate();
#endif
#endif


	m_wndStatusView.UpdateStatus();
}

/*============================================================================*/
/*! ���C���t���[��

-# �q�E�B���h�E�̓o�^��Ԏ擾

@param		pos		�E�B���h�E�C���f�b�N�X
@retval		TRUE	�E�B���h�E�L
			FALSE	�E�B���h�E��

*/
/*============================================================================*/
BOOL CMainFrame::IsExistChildList(int pos)
{
	map<UINT, LPARAM>::iterator itr;

	for (itr = m_ChildWndList.begin(); itr != m_ChildWndList.end(); ++itr)
	{
		if ((*itr).first == pos)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/*============================================================================*/
/*! ���C���t���[��

-# �q�E�B���h�E�̓o�^��Ԏ擾

@param		pWnd	�E�B���h�E�n���h��
@retval		�E�B���h�E�C���f�b�N�X

*/
/*============================================================================*/
int CMainFrame::FindChildListPos(CWnd* pWnd)
{
	map<UINT, LPARAM>::iterator itr;

	for (itr = m_ChildWndList.begin(); itr != m_ChildWndList.end(); ++itr){
		if ((*itr).second == (LPARAM)pWnd){
			return (int)(*itr).first;
		}
	}
	return -1;
}

/*============================================================================*/
/*! ���C���t���[��

-# �q�E�B���h�E�̓o�^��Ԏ擾

@param		pos		�E�B���h�E�C���f�b�N�X
@retval		TRUE	�E�B���h�E�L
			FALSE	�E�B���h�E��

*/
/*============================================================================*/
CWnd *CMainFrame::FindChildListWnd(int pos)
{
	map<UINT, LPARAM>::iterator itr;

	for (itr = m_ChildWndList.begin(); itr != m_ChildWndList.end(); ++itr)
	{
		if ((*itr).first == pos)
		{
			return (CWnd *)(*itr).second;
		}
	}
	return NULL;
}



// CMainFrame ���b�Z�[�W �n���h���[

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	m_wndOutput.UpdateFonts();
	m_wndLimit.UpdateFonts();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
	m_wndLimit.UpdateFonts();
}


LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_InitializeApplication:
		// �Ď������X�g��INI�t�@�C�����畜��
		theApp.GetDataCollection().ReadDataNameList((wParam == 0) ? theApp.GetAppInifileName() : CString((LPCWSTR)lParam));

		theApp.GetDataCollection().RequestAddSignalList();

		// �v���p�e�B�X�V
		m_wndProperties.UpdatePropatyDataList(FALSE);

		// �f�[�^�擾�Ώۃt���O��ݒ�
		theApp.GetDataCollection().SetSelectDataValid();

		// �t�@�C���r���[�X�V
		m_wndFileView.FillFileView();

		// �f�[�^ID���X�V
		for( UINT i = 0; i < MAX_VIEW; i++ )
		{
			m_wndOutput.UpdateDataId(i);
			m_wndLimit.UpdateDataId(i);

//			if( wParam != 0 )
//			{
//				if( m_ChildWndList[i] != NULL )
//					ShowChildFrame(i, FALSE);
//			}
		}

		// ���{���̎������X�V
		SetRealTimeRibbon();

		// �g�����h�r���[�P��\��
		if( wParam == 0 )
			PostMessage(WM_COMMAND, ID_CHECK_GRAPH1);

		break;

	case	eMessage_ChildActivate:
		// �q�E�B���h�E�̃A�N�e�B��Ԃ��������
		ResetChildList(message, wParam, lParam);
		break;

#if 0	// �����͌Ă΂�邱�Ƃ������B
	case	eMessage_ChildDestroy:
		// �q�E�B���h�E���I������
		ResetChildList(message, wParam, lParam);
		break;
#endif

	case eMessage_PropetyChange:
		{
			// �v���p�e�B�ύX�ʒm
			int pos = int(lParam);
			if (m_ChildWndList[pos] != NULL)
			{
				((CChildFrame*)m_ChildWndList[pos])->PostMessage(eMessage_PropetyChange, wParam, 0);

				m_wndOutput.UpdateDataId(pos);
				m_wndOutput.PostMessage(eMessage_UpdateCursorLine, pos, 0);

				m_wndLimit.UpdateDataId(pos);
				m_wndLimit.PostMessage(eMessage_UpdateCursorLine, pos, 0);
			}
		}
		break;

	case eMessage_ChangeActiveTab:
		{
			// �ύX�����^�u�ԍ��̃r���[���A�N�e�B�u�łȂ��Ȃ�Y���̃r���[���A�N�e�B�u�ɐݒ�
			if( FindChildListPos((CWnd *)MDIGetActive()) != (int)lParam )
			{
				CChildFrame	*pWnd = (CChildFrame *)FindChildListWnd((int)lParam);
				if( pWnd != NULL )
					pWnd->MDIActivate();
			}
		}
		break;

	case eMessage_AutoClear:
		{
			// AUTO�N���A���s�t���O���Z�b�g
			theApp.GetDataCollection().SetAutoClear();

			int pos = int(lParam);
			if (m_ChildWndList[pos] != NULL)
			{
				((CChildFrame*)m_ChildWndList[pos])->PostMessage(eMessage_AutoClear, wParam, lParam);
			}
		}
		break;

	//=====================================================================
	//	�X���b�h
	//=====================================================================
	case eMessage_ThreadNormal:
		{
#ifdef HighSpeedDemo
			bool	bUpdate = lParam ? true : false;
			lParam = 0;
#endif // HighSpeedDemo

			if (theApp.GetDataCollection().AddCollection(message, wParam, lParam) == true)
			{
#if 1
#ifdef HighSpeedDemo
				if( bUpdate )
#endif // HighSpeedDemo
				PostMessageToChildList(message, wParam, lParam);
#else
				// �Ď��f�[�^�X�V���C�x���g���M
				SetTrendViewUpdateEvent();
#endif

				// ���{���̎������X�V
				SetRealTimeRibbon();
			}
		}

		break;

	case eMessage_ThreadError:
	case eMessage_ThreadFail:
	case eMessage_ThreadException:
		// �����ŃG���[���̑Ή����s��
		theApp.GetDataCollection().StopCollection();
		break;

//	case eMessage_CloseWaitDialog:
//		if( m_pDlgWait != NULL )
//		{
//			m_pDlgWait->EndDialog(IDOK);
//			m_pDlgWait = NULL;
//		}

		break;

	default:
		return CMDIFrameWndEx::WindowProc(message, wParam, lParam);
	}
	return TRUE;
}

/*============================================================================*/
/*! ���{��

-# �t�@�C��-CSV�o�� ��������

@param		
@retval		

*/
/*============================================================================*/
void CMainFrame::OnButtonCsvOut()
{
	CPropertiesData&	prop = theApp.GetPropertiesData(m_nActiveGraph);

	// ���A���^�C�����[�h�Ȃ�X���[
	if( prop.isRealTime )
		return;

	// �f�[�^���I������Ă��Ȃ��Ȃ�X���[
	if( theApp.GetDataCollection().GetSelectDataList().size() == 0 )
		return;

	// �o�͂���f�t�H���g�̃t�@�C������ݒ�
	CString	szFile;
	SYSTEMTIME	curtime;
	GetLocalTime(&curtime);
	szFile.Format(_T("%s\\Data_%04d%02d%02d%02d%02d%02d.csv"), (LPCTSTR)theApp.GetAppDataPath(), curtime.wYear, curtime.wMonth, curtime.wDay, curtime.wHour, curtime.wMinute, curtime.wSecond);

	// �o�͂���t�@�C�������擾
	const TCHAR BASED_CODE szFilter[] = _T("CSV File(*.csv)|*.csv|All Files (*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("csv"), szFile, OFN_OVERWRITEPROMPT | OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);
	if( dlg.DoModal() != IDOK )
		return;
	m_szCsvFile = dlg.GetPathName();

	// CSV�o�͂��J�n
//	CDlgWait	wait_dlg(_T("CSV�t�@�C���ɏo�͂��Ă��܂��B\n���΂炭���҂����������B"), this);
//	m_pDlgWait = &wait_dlg;

#if 0
//	CWinThread	*ptr = AfxBeginThread(CsvOutThread, this);
#else
	CDlgWaitCmd waitDlg;

	waitDlg.Open(_T("CSV�o��"), _T("CSV�t�@�C���Ƀf�[�^���o�͂��Ă��܂��B\n\n���΂炭���҂����������B"), false);

	// CSV�t�@�C���ɏo��
	theApp.GetDataCollection().OutputCsvFile(GetActiveGraph(), m_szCsvFile);

	waitDlg.Close();
#endif

//	wait_dlg.DoModal();
}

/*============================================================================*/
/*! ���{��

-# �t�@�C��-CSV�o�� �X�V����

@param		pCmdUI�F�R�}���hUI
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateButtonCsvOut(CCmdUI *pCmdUI)
{
	if (theApp.GetDataCollection().GetSelectDataList().size() == 0)
		pCmdUI->Enable(FALSE);
	else
	{
		CPropertiesData&	prop = theApp.GetPropertiesData(m_nActiveGraph);

		pCmdUI->Enable(prop.isRealTime ? FALSE : TRUE);
	}
}

/*============================================================================*/
/*! ���{��

-# �t�@�C��-��� ��������

@param		
@retval		

*/
/*============================================================================*/
void CMainFrame::OnButtonPrintOut()
{
	// �ꉞ�S���
	// �A�N�e�B�u�E�B���h�E���n�[�h�R�s�[����ꍇ�̓A�N�e�B�u�̃E�B���h�E�n���h����n��
	//SetTimer(1, 100, NULL);
	CString	strExePath, strParam;
	strExePath.Format(_T("C:\\HardCopy\\NormalHardCopy.exe"));
//	strExePath.Format(_T("C:\\HardCopy\\ReverseHardCopy.exe"));
	strParam.Empty();
	::ShellExecute(m_hWnd, _T(""), strExePath, strParam, NULL, SW_NORMAL);
	//DirectPrint(this);
}

/*============================================================================*/
/*! ���{��

-# �t�@�C��-��� �X�V����

@param		pCmdUI�F�R�}���hUI
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateButtonPrintOut(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

/*============================================================================*/
/*! ���{��

-# �t�@�C��-�J�� ��������

@param		
@retval		

*/
/*============================================================================*/
void CMainFrame::OnButtonFileOpen()
{
	// ���͂���t�@�C�������擾
	const TCHAR BASED_CODE szFilter[] = _T("Config File(*.cfg)|*.cfg|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("cfg"), NULL, OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if( dlg.DoModal() != IDOK )
		return;

	// �ݒ�t�@�C������ݒ�󋵂�ǂݍ���
	OpenConfigFile(dlg.GetPathName());
}

/*============================================================================*/
/*! ���{��

-# �t�@�C��-�J�� �X�V����

@param		pCmdUI�F�R�}���hUI
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateButtonFileOpen(CCmdUI *pCmdUI)
{
	if (m_bOnline == TRUE)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);;
	}
}

/*============================================================================*/
/*! ���{��

-# �t�@�C��-�ۑ� ��������

@param		
@retval		

*/
/*============================================================================*/
void CMainFrame::OnButtonFileSave()
{
	// �o�͂���f�t�H���g�̃t�@�C������ݒ�
	CString	szFile;
	SYSTEMTIME	curtime;
	GetLocalTime(&curtime);
	szFile.Format(_T("%s\\Auto_%04d%02d%02d%02d%02d%02d.cfg"), (LPCTSTR)theApp.GetAppDataPath(), curtime.wYear, curtime.wMonth, curtime.wDay, curtime.wHour, curtime.wMinute, curtime.wSecond);

	// �o�͂���t�@�C�������擾
	const TCHAR BASED_CODE szFilter[] = _T("Config File(*.cfg)|*.cfg|All Files (*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("cfg"), szFile, OFN_OVERWRITEPROMPT | OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);
	if( dlg.DoModal() != IDOK )
		return;

	// �ݒ�t�@�C���ɐݒ�󋵂�ۑ�
	SaveConfigFile(dlg.GetPathName());
}

/*============================================================================*/
/*! ���{��

-# �t�@�C��-�ۑ� �X�V����

@param		pCmdUI�F�R�}���hUI
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateButtonFileSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

/*============================================================================*/
/*! ���{��

-# ���A���^�C��-�J�n�{�^�� ��������

@param		
@retval		

*/
/*============================================================================*/
void CMainFrame::OnButtonOnline()
{
	if (m_bOnline == FALSE && (theApp.GetDataCollection().GetSelectDataList().size() != 0))
	{
		// �f�[�^��������
		theApp.GetDataCollection().StartCollection();

		// �v���p�e�B�ύX�ʒm
		// �q�E�B���h�E�փ��b�Z�[�W�𑗂�
		map<UINT, LPARAM>::iterator itr;
		for (itr = m_ChildWndList.begin(); itr != m_ChildWndList.end(); itr++)
		{
			((CChildFrame*)(*itr).second)->SendMessage(mMessageStartInitialize, 0, 0);
			((CChildFrame*)(*itr).second)->SendMessage(eMessage_PropetyChange, 0, 0);
		}

//		theApp.GetDataCollection().StartCollection();	// �J�n��ɏ��������Ă����̂ōŏ��ֈړ�
		m_bOnline = TRUE;
	}
}

/*============================================================================*/
/*! ���{��

-# ���A���^�C��-�J�n�{�^�� �X�V����

@param		pCmdUI�F�R�}���hUI
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateButtonOnline(CCmdUI *pCmdUI)
{
	if (theApp.GetDataCollection().GetSelectDataList().size() == 0)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);

		if (m_bOnline == TRUE)
		{
			pCmdUI->SetRadio(TRUE);
		}
		else
		{
			pCmdUI->SetRadio(FALSE);
		}
	}
}

/*============================================================================*/
/*! ���{��

-# ���A���^�C��-��~�{�^�� ��������

@param		
@retval

*/
/*============================================================================*/
void CMainFrame::OnButtonOnlineStop()
{
	if (m_bOnline == TRUE && (theApp.GetDataCollection().GetSelectDataList().size() != 0))
	{
		theApp.GetDataCollection().StopCollection();
		m_bOnline = FALSE;
	}
}

/*============================================================================*/
/*! ���{��

-# ���A���^�C��-�J�n�{�^�� �X�V����

@param		pCmdUI�F�R�}���hUI
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateButtonOnlineStop(CCmdUI *pCmdUI)
{
	if (theApp.GetDataCollection().GetSelectDataList().size() == 0)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);

		if (m_bOnline == TRUE)
		{
			pCmdUI->SetRadio(FALSE);
		}
		else
		{
			pCmdUI->SetRadio(TRUE);
		}
	}
}

/*============================================================================*/
/*! ���{��

-# �f�[�^�o�^�{�^�� ��������

@param		
@retval

*/
/*============================================================================*/
void CMainFrame::OnButtonStation()
{
	CDataSelectDlg dlg;
	if (dlg.DoModal() == IDCANCEL)
		return;

	// �v���p�e�B�X�V
	m_wndProperties.UpdatePropatyDataList(TRUE, (dlg.m_bAutoAssign != false) ? TRUE : FALSE);

	// �f�[�^�擾�Ώۃt���O��ݒ�
	theApp.GetDataCollection().SetSelectDataValid();

	// �t�@�C���r���[�X�V
	m_wndFileView.FillFileView();

	// �f�[�^ID���X�V
	for( UINT i = 0; i < MAX_VIEW; i++ )
	{
		m_wndOutput.UpdateDataId(i);
		m_wndLimit.UpdateDataId(i);
	}
}


void CMainFrame::OnUpdateButtonStation(CCmdUI *pCmdUI)
{
	if (m_bOnline == TRUE)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);;
	}
}

/*============================================================================*/
/*! ���C���t���[��

-# ���X�g�ɓo�^����Ă���q�E�B���h�E����Ԃ�

@param		�Ȃ�
@retval		���X�g�ɓo�^����Ă���q�E�B���h�E��
*/
/*============================================================================*/
UINT CMainFrame::GetChildWndListSize()
{
	return (UINT)m_ChildWndList.size();
}

/*============================================================================*/
/*! ���{��

-# �O���t��ʂ̕\��/��\�����s���B

@param	pos		�F��ʃC���f�b�N�X
		bShow	�FTRUE �\���AFALSE ��\��

@retval

*/
/*============================================================================*/
void CMainFrame::ShowChildFrame(int pos, BOOL bShow)
{
	if( (CChildFrame*)m_ChildWndList[pos] == NULL )
		return;

	CString strView = _T("");
	((CChildFrame*)m_ChildWndList[pos])->GetWindowTextW(strView);

	if (bShow)
	{
		// �\���ݒ�
		((CChildFrame*)m_ChildWndList[pos])->ShowWindow(SW_SHOW);

		// �o�̓^�u��ǉ�
		m_wndOutput.AddTab(pos);

		// ���~�b�g�^�u��ǉ�
		m_wndLimit.AddTab(pos);

		// �v���p�e�B�E�B���h�E�A��ʑI���R���{�̃A�C�e���č쐬
		m_wndProperties.ExchangeViewCombo(m_bGraph, strView);

		((CChildFrame*)m_ChildWndList[pos])->MDIActivate();
	}
	else
	{
		// ��\���ݒ�
		((CChildFrame*)m_ChildWndList[pos])->ShowWindow(SW_HIDE);

		CString strNext;
		BOOL bVisible = FALSE;
		CWnd* next = ((CChildFrame*)m_ChildWndList[pos])->GetParent()->GetTopWindow();
		if (next != NULL)
		{
			((CChildFrame*)next)->GetWindowTextW(strNext);
			if (strView == strNext)
			{
				next = ((CChildFrame*)next)->GetWindow(GW_HWNDNEXT);
				while (next != NULL)
				{
					bVisible = next->IsWindowVisible();

					if (next != NULL)
					{
						((CChildFrame*)next)->GetWindowTextW(strNext);
					}

					if (bVisible)
					{
						break;
					}
					else
					{
						next = ((CChildFrame*)next)->GetWindow(GW_HWNDNEXT);
					}
				}
			}
		}
		else
		{
			next = ((CChildFrame*)m_ChildWndList[pos])->GetWindow(GW_HWNDNEXT);
			if (next != NULL)
			{
				((CChildFrame*)next)->GetWindowTextW(strNext);
			}
		}

		// �v���p�e�B�E�B���h�E�A��ʑI���R���{�̃A�C�e���č쐬
		m_wndProperties.ExchangeViewCombo(m_bGraph, strNext);

		// �I�����ꂽView���A�N�e�B�u�ɂ���
		if (next != NULL)
		{
			((CMDIChildWnd*)next)->MDIActivate();
		}

		// �E�B���h�E�ꗗ����폜����Ȃ��̂ŕ���
		((CChildFrame*)m_ChildWndList[pos])->DestroyWindow();
		m_ChildWndList.erase(pos);

		// �f�[�^�擾�Ώۃt���O��ݒ�
		theApp.GetDataCollection().SetSelectDataValid();

		// �o�̓^�u���폜
		m_wndOutput.DeleteTab(pos);

		// ���~�b�g�^�u���폜
		m_wndLimit.DeleteTab(pos);
	}
}


/*============================================================================*/
/*! ���{��

-# �O���t1-�\���`�F�b�N�{�b�N�X�@��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnCheckGraph1()
{
#if 0
	m_bGraph[grapf1] = !m_bGraph[grapf1];
	ShowChildFrame(grapf1, m_bGraph[grapf1]);
#else
	m_bGraph[grapf1] = !m_bGraph[grapf1];
	if ( IsExistChildList(grapf1) == FALSE)
	{
		m_CreatingView = grapf1;
		theApp.OpenNewView();
	}
	else
	{
		ShowChildFrame(grapf1, m_bGraph[grapf1]);
	}
#endif
}


/*============================================================================*/
/*! ���{��

-# �O���t2-�\���`�F�b�N�{�b�N�X�@��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnCheckGraph2()
{
	m_bGraph[grapf2] = !m_bGraph[grapf2];
	if ( IsExistChildList(grapf2) == FALSE)
	{
		m_CreatingView = grapf2;
		theApp.OpenNewView();
	}
	else
	{
		ShowChildFrame(grapf2, m_bGraph[grapf2]);
	}
}


/*============================================================================*/
/*! ���{��

-# �O���t3-�\���`�F�b�N�{�b�N�X�@��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnCheckGraph3()
{
	m_bGraph[grapf3] = !m_bGraph[grapf3];
//	ShowChildFrame(grapf3, m_bGraph[grapf3]);
	if (IsExistChildList(grapf3) == FALSE)
	{
		m_CreatingView = grapf3;
		theApp.OpenNewView();
	}
	else
	{
		ShowChildFrame(grapf3, m_bGraph[grapf3]);
	}
}


/*============================================================================*/
/*! ���{��

-# �O���t4-�\���`�F�b�N�{�b�N�X�@��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnCheckGraph4()
{
	m_bGraph[grapf4] = !m_bGraph[grapf4];
//	ShowChildFrame(grapf4, m_bGraph[grapf4]);
	if (IsExistChildList(grapf4) == FALSE)
	{
		m_CreatingView = grapf4;
		theApp.OpenNewView();
	}
	else
	{
		ShowChildFrame(grapf4, m_bGraph[grapf4]);
	}
}


/*============================================================================*/
/*! ���{��

-# �O���t5-�\���`�F�b�N�{�b�N�X�@��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnCheckGraph5()
{
	m_bGraph[grapf5] = !m_bGraph[grapf5];
//	ShowChildFrame(grapf5, m_bGraph[grapf5]);
	if (IsExistChildList(grapf5) == FALSE)
	{
		m_CreatingView = grapf5;
		theApp.OpenNewView();
	}
	else
	{
		ShowChildFrame(grapf5, m_bGraph[grapf5]);
	}
}


/*============================================================================*/
/*! ���{��

-# �O���t6-�\���`�F�b�N�{�b�N�X�@��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnCheckGraph6()
{
	m_bGraph[grapf6] = !m_bGraph[grapf6];
//	ShowChildFrame(grapf6, m_bGraph[grapf6]);
	if (IsExistChildList(grapf6) == FALSE)
	{
		m_CreatingView = grapf6;
		theApp.OpenNewView();
	}
	else
	{
		ShowChildFrame(grapf6, m_bGraph[grapf6]);
	}
}


/*============================================================================*/
/*! ���{��

-# �O���t7-�\���`�F�b�N�{�b�N�X�@��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnCheckGraph7()
{
	m_bGraph[grapf7] = !m_bGraph[grapf7];
//	ShowChildFrame(grapf7, m_bGraph[grapf7]);
	if (IsExistChildList(grapf7) == FALSE)
	{
		m_CreatingView = grapf7;
		theApp.OpenNewView();
	}
	else
	{
		ShowChildFrame(grapf7, m_bGraph[grapf7]);
	}
}


/*============================================================================*/
/*! ���{��

-# �O���t8-�\���`�F�b�N�{�b�N�X�@��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnCheckGraph8()
{
	m_bGraph[grapf8] = !m_bGraph[grapf8];
	//	ShowChildFrame(grapf8, m_bGraph[grapf8]);
	if (IsExistChildList(grapf8) == FALSE)
	{
		m_CreatingView = grapf8;
		theApp.OpenNewView();
	}
	else
	{
		ShowChildFrame(grapf8, m_bGraph[grapf8]);
	}
}

/*============================================================================*/
/*! ���{��

-# �I�t���C���p�O���t-�\���`�F�b�N�{�b�N�X�@��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnCheckGraph9()
{
	m_bGraph[grapf9] = !m_bGraph[grapf9];
	//	ShowChildFrame(grapf8, m_bGraph[grapf8]);
	if (IsExistChildList(grapf9) == FALSE)
	{
		m_CreatingView = grapf9;
		theApp.OpenNewView();
	}
	else
	{
		ShowChildFrame(grapf9, m_bGraph[grapf9]);
	}
}


/*============================================================================*/
/*! ���{��

-# �O���t1-�\���`�F�b�N�{�b�N�X�@�X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateCheckGraph1(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGraph[grapf1]);
}


/*============================================================================*/
/*! ���{��

-# �O���t2-�\���`�F�b�N�{�b�N�X�@�X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateCheckGraph2(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGraph[grapf2]);
}


/*============================================================================*/
/*! ���{��

-# �O���t3-�\���`�F�b�N�{�b�N�X�@�X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateCheckGraph3(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGraph[grapf3]);
}


/*============================================================================*/
/*! ���{��

-# �O���t4-�\���`�F�b�N�{�b�N�X�@�X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateCheckGraph4(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGraph[grapf4]);
}


/*============================================================================*/
/*! ���{��

-# �O���t5-�\���`�F�b�N�{�b�N�X�@�X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateCheckGraph5(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGraph[grapf5]);
}


/*============================================================================*/
/*! ���{��

-# �O���t6-�\���`�F�b�N�{�b�N�X�@�X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateCheckGraph6(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGraph[grapf6]);
}


/*============================================================================*/
/*! ���{��

-# �O���t7-�\���`�F�b�N�{�b�N�X�@�X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateCheckGraph7(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGraph[grapf7]);
}


/*============================================================================*/
/*! ���{��

-# �O���t8-�\���`�F�b�N�{�b�N�X�@�X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateCheckGraph8(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGraph[grapf8]);
}


/*============================================================================*/
/*! ���{��

-# �\��-�v���p�e�B�E�B���h�E�`�F�b�N�{�b�N�X�@��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnViewProperties()
{
	m_wndProperties.ShowPane(!m_wndProperties.IsVisible(), FALSE, TRUE);
}


/*============================================================================*/
/*! ���{��

-# �\��-�v���p�e�B�E�B���h�E�`�F�b�N�{�b�N�X�@�X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateViewProperties(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndProperties.IsVisible());
}


/*============================================================================*/
/*! ���{��

-# �\��-�t�@�C���r���[�`�F�b�N�{�b�N�X�@��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnViewDatalist()
{
	m_wndFileView.ShowPane(!m_wndFileView.IsVisible(), FALSE, TRUE);
}


/*============================================================================*/
/*! ���{��

-# �\��-�t�@�C���r���[�`�F�b�N�{�b�N�X�@�X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateViewDatalist(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndFileView.IsVisible());	// Phase1
}


/*============================================================================*/
/*! ���{��

-# �\��-�o�̓E�B���h�E�`�F�b�N�{�b�N�X�@��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnViewOutput()
{
	m_wndOutput.ShowPane(!m_wndOutput.IsVisible(), FALSE, TRUE);
}


/*============================================================================*/
/*! ���{��

-# �\��-�o�̓E�B���h�E�`�F�b�N�{�b�N�X�@�X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateViewOutput(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndOutput.IsVisible());
}


/*============================================================================*/
/*! ���{��

-# �\��-���~�b�g�E�B���h�E�`�F�b�N�{�b�N�X�@��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnViewLimit()
{
	m_wndLimit.ShowPane(!m_wndLimit.IsVisible(), FALSE, TRUE);
}


/*============================================================================*/
/*! ���{��

-# �\��-���~�b�g�E�B���h�E�`�F�b�N�{�b�N�X�@�X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateViewLimit(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndLimit.IsVisible());
}

/*============================================================================*/
/*! ���{��

-# �\��-�X�e�[�^�X�E�B���h�E�`�F�b�N�{�b�N�X�@��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnViewStatus()
{
	m_wndStatusView.ShowPane(!m_wndStatusView.IsVisible(), FALSE, TRUE);
}


/*============================================================================*/
/*! ���{��

-# �\��-�X�e�[�^�X�E�B���h�E�`�F�b�N�{�b�N�X�@�X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateViewStatus(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndStatusView.IsVisible());
}

/*============================================================================*/
/*! ���{��

-# �O���t1-Auto�N���A�{�^�� ��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnBtnGraphClear1()
{
	PostMessage(eMessage_AutoClear, 0, (LPARAM)0);
}


/*============================================================================*/
/*! ���{��

-# �O���t2-Auto�N���A�{�^�� ��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnBtnGraphClear2()
{
	PostMessage(eMessage_AutoClear, 0, (LPARAM)1);
}


/*============================================================================*/
/*! ���{��

-# �O���t3-Auto�N���A�{�^�� ��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnBtnGraphClear3()
{
	PostMessage(eMessage_AutoClear, 0, (LPARAM)2);
}


/*============================================================================*/
/*! ���{��

-# �O���t4-Auto�N���A�{�^�� ��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnBtnGraphClear4()
{
	PostMessage(eMessage_AutoClear, 0, (LPARAM)3);
}


/*============================================================================*/
/*! ���{��

-# �O���t5-Auto�N���A�{�^�� ��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnBtnGraphClear5()
{
	PostMessage(eMessage_AutoClear, 0, (LPARAM)4);
}


/*============================================================================*/
/*! ���{��

-# �O���t6-Auto�N���A�{�^�� ��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnBtnGraphClear6()
{
	PostMessage(eMessage_AutoClear, 0, (LPARAM)5);
}


/*============================================================================*/
/*! ���{��

-# �O���t7-Auto�N���A�{�^�� ��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnBtnGraphClear7()
{
	PostMessage(eMessage_AutoClear, 0, (LPARAM)6);
}


/*============================================================================*/
/*! ���{��

-# �O���t8-Auto�N���A�{�^�� ��������

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnBtnGraphClear8()
{
	PostMessage(eMessage_AutoClear, 0, (LPARAM)7);
}


/*============================================================================*/
/*! ���{��

-# �O���t1-Auto�N���A�{�^�� �X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateBtnGraphClear1(CCmdUI *pCmdUI)
{
	if (m_bGraph[grapf1] && m_wndProperties.GetAutoScaleEnable(0))
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}


/*============================================================================*/
/*! ���{��

-# �O���t2-Auto�N���A�{�^�� �X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateBtnGraphClear2(CCmdUI *pCmdUI)
{
	if (m_bGraph[grapf2] && m_wndProperties.GetAutoScaleEnable(1))
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}


/*============================================================================*/
/*! ���{��

-# �O���t3-Auto�N���A�{�^�� �X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateBtnGraphClear3(CCmdUI *pCmdUI)
{
	if (m_bGraph[grapf3] && m_wndProperties.GetAutoScaleEnable(2))
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}


/*============================================================================*/
/*! ���{��

-# �O���t4-Auto�N���A�{�^�� �X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateBtnGraphClear4(CCmdUI *pCmdUI)
{
	if (m_bGraph[grapf4] && m_wndProperties.GetAutoScaleEnable(3))
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}


/*============================================================================*/
/*! ���{��

-# �O���t5-Auto�N���A�{�^�� �X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateBtnGraphClear5(CCmdUI *pCmdUI)
{
	if (m_bGraph[grapf5] && m_wndProperties.GetAutoScaleEnable(4))
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}


/*============================================================================*/
/*! ���{��

-# �O���t6-Auto�N���A�{�^�� �X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateBtnGraphClear6(CCmdUI *pCmdUI)
{
	if (m_bGraph[grapf6] && m_wndProperties.GetAutoScaleEnable(5))
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}


/*============================================================================*/
/*! ���{��

-# �O���t7-Auto�N���A�{�^�� �X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateBtnGraphClear7(CCmdUI *pCmdUI)
{
	if (m_bGraph[grapf7] && m_wndProperties.GetAutoScaleEnable(6))
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}


/*============================================================================*/
/*! ���{��

-# �O���t8-Auto�N���A�{�^�� �X�V����

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateBtnGraphClear8(CCmdUI *pCmdUI)
{
	if (m_bGraph[grapf8] && m_wndProperties.GetAutoScaleEnable(7))
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}
