// この MFC サンプル ソース コードでは、MFC Microsoft Office Fluent ユーザー インターフェイス 
// ("Fluent UI") の使用方法を示します。このコードは、MFC C++ ライブラリ ソフトウェアに 
// 同梱されている Microsoft Foundation Class リファレンスおよび関連電子ドキュメントを
// 補完するための参考資料として提供されます。
// Fluent UI を複製、使用、または配布するためのライセンス条項は個別に用意されています。
// Fluent UI ライセンス プログラムの詳細については、Web サイト
// http://go.microsoft.com/fwlink/?LinkId=238214 を参照してください。
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MainFrm.cpp : CMainFrame クラスの実装
//

#include "stdafx.h"
#include "RemoteGraph.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "DataSelectDlg.h"
#include "HookMsgBox.h"
#include "WaitDialog.h"

// timeGetTime使用
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

// CMainFrame コンストラクション/デストラクション

CMainFrame::CMainFrame()
{
	// TODO: メンバー初期化コードをここに追加してください。
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

	// 最小タイマ分解能を設定(1ms)
	timeBeginPeriod(1);

	// 起動中は常にチェック
	while( !pWnd->m_bCheckDiskStop )
	{
		bool	bSpaceOk = true;

		// ドライブ名を抽出
		int index = theApp.GetAppDataPath().Find(_T(":"));
		if( index == -1 )
			continue;

		CString	szDrive = theApp.GetAppDataPath().Left(index + 1) + _T("\\");

		// ディスク情報を取得
		ULARGE_INTEGER	nFree;
		ULARGE_INTEGER	nAvail;
		ULARGE_INTEGER	nTotal;

		if( GetDiskFreeSpaceEx(szDrive, &nFree, &nTotal, &nAvail) != 0 )
		{
			// 空き容量を確認
			if( nTotal.QuadPart != 0 )
			{
				if( ((nAvail.QuadPart * 100) / nTotal.QuadPart) < DISK_FREE_SPACE )
					bSpaceOk = false;

				// ディスク空き容量フラグを設定
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

	// CSVファイルに出力
	theApp.GetDataCollection().OutputCsvFile(pWnd->GetActiveGraph(), pWnd->m_szCsvFile);

	// ダイアログの終了を通知
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

	// キャプチャに対応するまで非表示
	{
		CMFCRibbonCategory	*pCategory = m_wndRibbonBar.GetCategory(0);
		if( pCategory != NULL )
		{
			CMFCRibbonPanel	*pPanel = pCategory->GetPanel(2);
			if( pPanel != NULL )
				pPanel->Remove(5);
		}
	}
//#ifndef	_DEBUG	// リボンに表示する場合は有効に!!
	// リアルタイムの開始時刻／終了時刻を非表示
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
		TRACE0("ステータス バーの作成に失敗しました。\n");
		return -1;      // 作成できない場合
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

	// Visual Studio 2005 スタイルのドッキング ウィンドウ動作を有効にします
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 スタイルのドッキング ウィンドウの自動非表示動作を有効にします
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// メニュー項目イメージ (どの標準ツール バーにもないイメージ) を読み込みます:
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// ドッキング ウィンドウを作成します
	if (!CreateDockingWindows())
	{
		TRACE0("ドッキング ウィンドウを作成できませんでした\n");
		return -1;
	}

	// 各種ウィンドウをドッキング
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

	// 各種ウィンドウのレイアウトを調整
	m_wndFileView.GetWindowRect(rc);
	m_wndFileView.SetWindowPos(NULL, 0, 0, 300, rc.Height(), SWP_NOMOVE | SWP_NOOWNERZORDER);

	m_wndProperties.GetWindowRect(rc);
	m_wndProperties.SetWindowPos(NULL, 0, 0, 300, rc.Height(), SWP_NOMOVE | SWP_NOOWNERZORDER);

	m_wndOutput.GetWindowRect(rc);
	m_wndOutput.SetWindowPos(NULL, 0, 0, rc.Width(), 205, SWP_NOMOVE | SWP_NOOWNERZORDER);

	// 固定値に基づいてビジュアル マネージャーと visual スタイルを設定します
	OnApplicationLook(theApp.m_nAppLook);

	// 拡張ウィンドウ管理ダイアログ ボックスを有効にします
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// ディスク空き容量スレッドを起動
	m_bCheckDiskStop = false;
	m_pCheckDiskThread = AfxBeginThread(CheckDiskThread, this);

	return 0;
}

void CMainFrame::OnClose()
{
	if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("終了します。\nよろしいですか？"), theApp.m_pszAppName, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return;

	// スレッドを終了
	if (m_bOnline == TRUE && (theApp.GetDataCollection().GetSelectDataList().size() != 0))
	{
		theApp.GetDataCollection().StopCollection();
		m_bOnline = FALSE;
	}
	theApp.GetDataCollection().StopAllAnalysisDataThread();

	// INIファイルに設定状況を保存
	SaveConfigFile(theApp.GetAppInifileName());

	// ディスク空き容量スレッドを停止
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
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

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
	// クラス ビューを作成します
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("クラス ビュー ウィンドウを作成できませんでした\n");
		return FALSE; // 作成できませんでした
	}
#endif

	// ファイル ビューを作成します
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("ファイル ビュー ウィンドウを作成できませんでした\n");
		return FALSE; // 作成できませんでした
	}

	// 出力ウィンドウを作成します
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 500, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("出力ウィンドウを作成できませんでした\n");
		return FALSE; // 作成できませんでした
	}

	// リミットウィンドウを作成します
	CString strLimitWnd;
	bNameValid = strLimitWnd.LoadString(IDS_LIMIT_WND);
	ASSERT(bNameValid);
	if (!m_wndLimit.Create(strLimitWnd, this, CRect(0, 0, 500, 100), TRUE, ID_VIEW_LIMITWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("リミットウィンドウを作成できませんでした\n");
		return FALSE; // 作成できませんでした
	}

	// プロパティ ウィンドウを作成します
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("プロパティ ウィンドウを作成できませんでした\n");
		return FALSE; // 作成できませんでした
	}

	// ステータス ビューを作成します
	CString strStatusView;
	bNameValid = strStatusView.LoadString(IDS_STATUS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndStatusView.Create(strStatusView, this, CRect(0, 0, 50, 200), TRUE, ID_VIEW_STATUSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("ステータス ビュー ウィンドウを作成できませんでした\n");
		return FALSE; // 作成できませんでした
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

// CMainFrame 診断

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
/*! メインフレーム

-# 設定情報を読み込み

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OpenConfigFile(CString szFilePath)
{
	// 受信監視データを初期化
	theApp.GetDataCollection().DeleteSpaceRecv();

	// DATAフォルダ内のファイルを削除
	theApp.DeleteFileFromDataFolder(-1);

	// リアルタイムモードの時刻情報を初期化
	theApp.SetRealTimeStart(CTime(0));
	theApp.SetRealTimeLast(CTime(0));

	for( int i = 0; i < (MAX_VIEW * MAX_TDATA); i++ )
		theApp.SetRealTimeWriteData(i, CTime(0));
	theApp.SetRealTimeWriteMax(CTime(0));

	theApp.SetStartFile(0);

	// ビュー関連のデータを削除
	theApp.GetDataCollection().DeleteCollectionAll();

#if 0
	// プロパティ設定を反映
	for( UINT i = 0; i < MAX_VIEW; i++ )
		SendMessage(eMessage_PropetyChange, 1, (LPARAM)i);
#endif

	// プロパティデータを設定ファイルから復元
	theApp.ReadAllPropertiesData(szFilePath);

	// 簡易リストを変更
	GetPropertiesWnd().UpdateSimplicityList();

	// データ取得対象フラグを設定
	theApp.GetDataCollection().SetSelectDataValid();

	// ビューを初期化
	SendMessage(eMessage_InitializeApplication, 1, (LPARAM)szFilePath.operator LPCWSTR());

#if 1
	// プロパティ設定を反映
	for( UINT i = 0; i < MAX_VIEW; i++ )
	{
		map<UINT, LPARAM>::iterator itr = m_ChildWndList.find(i);
		if( itr != m_ChildWndList.end() )
			SendMessage(eMessage_PropetyChange, 1, (LPARAM)i);
	}
#endif
}

/*============================================================================*/
/*! メインフレーム

-# 設定情報を保存

@param
@retval

*/
/*============================================================================*/
void CMainFrame::SaveConfigFile(CString szFilePath)
{
	CString	str;

	// バージョンを保存
	str.Format(_T("%d"), FILE_VERSION);
	WritePrivateProfileString(SECTION_SYSTEM, KEY_FILEVERSION, str, szFilePath);

	// 監視名リストを保存
	theApp.GetDataCollection().WriteDataNameList(szFilePath);

	// プロパティデータを保存
	theApp.WriteAllPropertiesData(szFilePath);
}

/*============================================================================*/
/*! メインフレーム

-# グラフプロパティの更新

@param		lParam	ウィンドウハンドル
@retval

*/
/*============================================================================*/
void CMainFrame::UpdataeProperties(LPARAM lParam)
{
}

/*============================================================================*/
/*! メインフレーム

-# 子ウィンドウの登録削除

@param		message	ウィンドウメッセージ
@param		wParam	アクティブ状態
@param		lParam	ウィンドウハンドル
@retval

*/
/*============================================================================*/
void CMainFrame::ResetChildList(UINT message, WPARAM wParam, LPARAM lParam)
{
	CString strTitle = _T("");
	map<UINT, LPARAM>::iterator itr;

	for (itr = m_ChildWndList.begin(); itr != m_ChildWndList.end(); itr++){
		if (message == eMessage_ChildActivate){
			// アクティブ状態の変化
			if ((*itr).second == lParam){
				if (wParam)	{
					m_nActiveGraph = (*itr).first;

					// アクティブな画面のプロパティに変更
					UINT pos = (*itr).first;
					m_wndProperties.ChangeView(pos);

					// アクティブウィンドウ番号を通知する
					m_wndFileView.SetActiveNo(pos);

					// アクティブな画面の出力タブに変更
					m_wndOutput.UpdateTab(pos);

					// アクティブな画面のリミットタブに変更
					m_wndLimit.UpdateTab(pos);
				}
				return;
			}
		}
#if 0	// ここは通ることがない
		else{
			// 終了
			if ((*itr).second == lParam){
				// リストから削除
				UINT pos = (*itr).first;
				m_ChildWndList.erase(itr);
				if (m_nActiveGraph == pos)
				{
					m_nActiveGraph = 0;
				}

				// Property削除
				m_wndProperties.CloseView(pos);
				// 出力タブ削除
				m_wndOutput.DeleteTab(pos);
				return;
			}
		}
#endif
	}

#if 0	// ここは通ることがない
	// ここにきた場合は、リストに登録されていない
	if (message == eMessage_ChildDestroy){
		// 何もしない
		return;
	}
#endif

	// 非アクティブならスルー
	if( !wParam )
		return;

	// 子ウインドウリストに無い(つまり初回)なら、リストに追加してプロパティ／タブを追加

//	UINT count = (int)m_ChildWndList.size();
	UINT count = (int)m_CreatingView;

	// 新規登録
	m_ChildWndList.insert(map<UINT, LPARAM>::value_type(count, lParam));
	m_nActiveGraph = count;

	// Property データ新規作成
	m_wndProperties.SetNewPropertyData(count, lParam);

	// データ取得対象フラグを設定
	theApp.GetDataCollection().SetSelectDataValid();

	// アクティブウィンドウ番号を通知する
	m_wndFileView.SetActiveNo(count);

	// 出力タブを追加
	m_wndOutput.AddTab(count);

	// リミットタブを追加
	m_wndLimit.AddTab(count);
}


/*============================================================================*/
/*! メインフレーム

-# 各子ウィンドウへのメッセージ通知

@param		message	ウィンドウメッセージ
@param		wParam	アクティブ状態
@param		lParam	ウィンドウハンドル
@retval		なし

*/
/*============================================================================*/
void CMainFrame::PostMessageToChildList(UINT message, WPARAM wParam, LPARAM lParam)
{
	(void)wParam;
	(void)lParam;
	CString strTitle = _T("");
	map<UINT, LPARAM>::iterator itr;

	// 子ウィンドウへメッセージを送る
	for (itr = m_ChildWndList.begin(); itr != m_ChildWndList.end(); itr++)
	{
		((CChildFrame*)(*itr).second)->PostMessage(message, wParam, lParam);

		m_wndOutput.PostMessage(message, (*itr).first);

		m_wndLimit.PostMessage(message, (*itr).first);
	}
}

#if 0
/*============================================================================*/
/*! メインフレーム

-# 各トレンドビューウィンドウへの更新イベント通知

@param		なし
@retval		なし

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
/*! メインフレーム

-# リアルタイムモードの測定開始／終了(現在)の時刻を更新

@param		なし
@retval		なし

*/
/*============================================================================*/
void CMainFrame::SetRealTimeRibbon()
{
#if 0	// リボンに表示する場合は有効に!!
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
			// 解析にて扱い不可能な受信した時刻を表示
			strYmd = theApp.GetRealTimeLast().Format("%Y/%m/%d");
			strHMS = theApp.GetRealTimeLast().Format("%H:%M:%S");
#else
			// 解析にて扱い可能な書き込んだ時刻を表示
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
/*! メインフレーム

-# 子ウィンドウの登録状態取得

@param		pos		ウィンドウインデックス
@retval		TRUE	ウィンドウ有
			FALSE	ウィンドウ無

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
/*! メインフレーム

-# 子ウィンドウの登録状態取得

@param		pWnd	ウィンドウハンドル
@retval		ウィンドウインデックス

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
/*! メインフレーム

-# 子ウィンドウの登録状態取得

@param		pos		ウィンドウインデックス
@retval		TRUE	ウィンドウ有
			FALSE	ウィンドウ無

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



// CMainFrame メッセージ ハンドラー

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
		// 監視名リストをINIファイルから復元
		theApp.GetDataCollection().ReadDataNameList((wParam == 0) ? theApp.GetAppInifileName() : CString((LPCWSTR)lParam));

		theApp.GetDataCollection().RequestAddSignalList();

		// プロパティ更新
		m_wndProperties.UpdatePropatyDataList(FALSE);

		// データ取得対象フラグを設定
		theApp.GetDataCollection().SetSelectDataValid();

		// ファイルビュー更新
		m_wndFileView.FillFileView();

		// データIDを更新
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

		// リボンの時刻を更新
		SetRealTimeRibbon();

		// トレンドビュー１を表示
		if( wParam == 0 )
			PostMessage(WM_COMMAND, ID_CHECK_GRAPH1);

		break;

	case	eMessage_ChildActivate:
		// 子ウィンドウのアクティ状態がかわった
		ResetChildList(message, wParam, lParam);
		break;

#if 0	// ここは呼ばれることが無い。
	case	eMessage_ChildDestroy:
		// 子ウィンドウが終了した
		ResetChildList(message, wParam, lParam);
		break;
#endif

	case eMessage_PropetyChange:
		{
			// プロパティ変更通知
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
			// 変更したタブ番号のビューがアクティブでないなら該当のビューをアクティブに設定
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
			// AUTOクリア実行フラグをセット
			theApp.GetDataCollection().SetAutoClear();

			int pos = int(lParam);
			if (m_ChildWndList[pos] != NULL)
			{
				((CChildFrame*)m_ChildWndList[pos])->PostMessage(eMessage_AutoClear, wParam, lParam);
			}
		}
		break;

	//=====================================================================
	//	スレッド
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
				// 監視データ更新をイベント送信
				SetTrendViewUpdateEvent();
#endif

				// リボンの時刻を更新
				SetRealTimeRibbon();
			}
		}

		break;

	case eMessage_ThreadError:
	case eMessage_ThreadFail:
	case eMessage_ThreadException:
		// ここでエラー時の対応を行う
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
/*! リボン

-# ファイル-CSV出力 押下処理

@param		
@retval		

*/
/*============================================================================*/
void CMainFrame::OnButtonCsvOut()
{
	CPropertiesData&	prop = theApp.GetPropertiesData(m_nActiveGraph);

	// リアルタイムモードならスルー
	if( prop.isRealTime )
		return;

	// データが選択されていないならスルー
	if( theApp.GetDataCollection().GetSelectDataList().size() == 0 )
		return;

	// 出力するデフォルトのファイル名を設定
	CString	szFile;
	SYSTEMTIME	curtime;
	GetLocalTime(&curtime);
	szFile.Format(_T("%s\\Data_%04d%02d%02d%02d%02d%02d.csv"), (LPCTSTR)theApp.GetAppDataPath(), curtime.wYear, curtime.wMonth, curtime.wDay, curtime.wHour, curtime.wMinute, curtime.wSecond);

	// 出力するファイル名を取得
	const TCHAR BASED_CODE szFilter[] = _T("CSV File(*.csv)|*.csv|All Files (*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("csv"), szFile, OFN_OVERWRITEPROMPT | OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);
	if( dlg.DoModal() != IDOK )
		return;
	m_szCsvFile = dlg.GetPathName();

	// CSV出力を開始
//	CDlgWait	wait_dlg(_T("CSVファイルに出力しています。\nしばらくお待ちください。"), this);
//	m_pDlgWait = &wait_dlg;

#if 0
//	CWinThread	*ptr = AfxBeginThread(CsvOutThread, this);
#else
	CDlgWaitCmd waitDlg;

	waitDlg.Open(_T("CSV出力"), _T("CSVファイルにデータを出力しています。\n\nしばらくお待ちください。"), false);

	// CSVファイルに出力
	theApp.GetDataCollection().OutputCsvFile(GetActiveGraph(), m_szCsvFile);

	waitDlg.Close();
#endif

//	wait_dlg.DoModal();
}

/*============================================================================*/
/*! リボン

-# ファイル-CSV出力 更新処理

@param		pCmdUI：コマンドUI
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
/*! リボン

-# ファイル-印刷 押下処理

@param		
@retval		

*/
/*============================================================================*/
void CMainFrame::OnButtonPrintOut()
{
	// 一応全画面
	// アクティブウィンドウをハードコピーする場合はアクティブのウィンドウハンドルを渡す
	//SetTimer(1, 100, NULL);
	CString	strExePath, strParam;
	strExePath.Format(_T("C:\\HardCopy\\NormalHardCopy.exe"));
//	strExePath.Format(_T("C:\\HardCopy\\ReverseHardCopy.exe"));
	strParam.Empty();
	::ShellExecute(m_hWnd, _T(""), strExePath, strParam, NULL, SW_NORMAL);
	//DirectPrint(this);
}

/*============================================================================*/
/*! リボン

-# ファイル-印刷 更新処理

@param		pCmdUI：コマンドUI
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateButtonPrintOut(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

/*============================================================================*/
/*! リボン

-# ファイル-開く 押下処理

@param		
@retval		

*/
/*============================================================================*/
void CMainFrame::OnButtonFileOpen()
{
	// 入力するファイル名を取得
	const TCHAR BASED_CODE szFilter[] = _T("Config File(*.cfg)|*.cfg|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("cfg"), NULL, OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if( dlg.DoModal() != IDOK )
		return;

	// 設定ファイルから設定状況を読み込む
	OpenConfigFile(dlg.GetPathName());
}

/*============================================================================*/
/*! リボン

-# ファイル-開く 更新処理

@param		pCmdUI：コマンドUI
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
/*! リボン

-# ファイル-保存 押下処理

@param		
@retval		

*/
/*============================================================================*/
void CMainFrame::OnButtonFileSave()
{
	// 出力するデフォルトのファイル名を設定
	CString	szFile;
	SYSTEMTIME	curtime;
	GetLocalTime(&curtime);
	szFile.Format(_T("%s\\Auto_%04d%02d%02d%02d%02d%02d.cfg"), (LPCTSTR)theApp.GetAppDataPath(), curtime.wYear, curtime.wMonth, curtime.wDay, curtime.wHour, curtime.wMinute, curtime.wSecond);

	// 出力するファイル名を取得
	const TCHAR BASED_CODE szFilter[] = _T("Config File(*.cfg)|*.cfg|All Files (*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("cfg"), szFile, OFN_OVERWRITEPROMPT | OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);
	if( dlg.DoModal() != IDOK )
		return;

	// 設定ファイルに設定状況を保存
	SaveConfigFile(dlg.GetPathName());
}

/*============================================================================*/
/*! リボン

-# ファイル-保存 更新処理

@param		pCmdUI：コマンドUI
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateButtonFileSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

/*============================================================================*/
/*! リボン

-# リアルタイム-開始ボタン 押下処理

@param		
@retval		

*/
/*============================================================================*/
void CMainFrame::OnButtonOnline()
{
	if (m_bOnline == FALSE && (theApp.GetDataCollection().GetSelectDataList().size() != 0))
	{
		// データを初期化
		theApp.GetDataCollection().StartCollection();

		// プロパティ変更通知
		// 子ウィンドウへメッセージを送る
		map<UINT, LPARAM>::iterator itr;
		for (itr = m_ChildWndList.begin(); itr != m_ChildWndList.end(); itr++)
		{
			((CChildFrame*)(*itr).second)->SendMessage(mMessageStartInitialize, 0, 0);
			((CChildFrame*)(*itr).second)->SendMessage(eMessage_PropetyChange, 0, 0);
		}

//		theApp.GetDataCollection().StartCollection();	// 開始後に初期化していたので最初へ移動
		m_bOnline = TRUE;
	}
}

/*============================================================================*/
/*! リボン

-# リアルタイム-開始ボタン 更新処理

@param		pCmdUI：コマンドUI
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
/*! リボン

-# リアルタイム-停止ボタン 押下処理

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
/*! リボン

-# リアルタイム-開始ボタン 更新処理

@param		pCmdUI：コマンドUI
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
/*! リボン

-# データ登録ボタン 押下処理

@param		
@retval

*/
/*============================================================================*/
void CMainFrame::OnButtonStation()
{
	CDataSelectDlg dlg;
	if (dlg.DoModal() == IDCANCEL)
		return;

	// プロパティ更新
	m_wndProperties.UpdatePropatyDataList(TRUE, (dlg.m_bAutoAssign != false) ? TRUE : FALSE);

	// データ取得対象フラグを設定
	theApp.GetDataCollection().SetSelectDataValid();

	// ファイルビュー更新
	m_wndFileView.FillFileView();

	// データIDを更新
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
/*! メインフレーム

-# リストに登録されている子ウィンドウ数を返す

@param		なし
@retval		リストに登録されている子ウィンドウ数
*/
/*============================================================================*/
UINT CMainFrame::GetChildWndListSize()
{
	return (UINT)m_ChildWndList.size();
}

/*============================================================================*/
/*! リボン

-# グラフ画面の表示/非表示を行う。

@param	pos		：画面インデックス
		bShow	：TRUE 表示、FALSE 非表示

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
		// 表示設定
		((CChildFrame*)m_ChildWndList[pos])->ShowWindow(SW_SHOW);

		// 出力タブを追加
		m_wndOutput.AddTab(pos);

		// リミットタブを追加
		m_wndLimit.AddTab(pos);

		// プロパティウィンドウ、画面選択コンボのアイテム再作成
		m_wndProperties.ExchangeViewCombo(m_bGraph, strView);

		((CChildFrame*)m_ChildWndList[pos])->MDIActivate();
	}
	else
	{
		// 非表示設定
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

		// プロパティウィンドウ、画面選択コンボのアイテム再作成
		m_wndProperties.ExchangeViewCombo(m_bGraph, strNext);

		// 選択されたViewをアクティブにする
		if (next != NULL)
		{
			((CMDIChildWnd*)next)->MDIActivate();
		}

		// ウィンドウ一覧から削除されないので閉じる
		((CChildFrame*)m_ChildWndList[pos])->DestroyWindow();
		m_ChildWndList.erase(pos);

		// データ取得対象フラグを設定
		theApp.GetDataCollection().SetSelectDataValid();

		// 出力タブを削除
		m_wndOutput.DeleteTab(pos);

		// リミットタブを削除
		m_wndLimit.DeleteTab(pos);
	}
}


/*============================================================================*/
/*! リボン

-# グラフ1-表示チェックボックス　押下処理

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
/*! リボン

-# グラフ2-表示チェックボックス　押下処理

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
/*! リボン

-# グラフ3-表示チェックボックス　押下処理

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
/*! リボン

-# グラフ4-表示チェックボックス　押下処理

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
/*! リボン

-# グラフ5-表示チェックボックス　押下処理

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
/*! リボン

-# グラフ6-表示チェックボックス　押下処理

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
/*! リボン

-# グラフ7-表示チェックボックス　押下処理

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
/*! リボン

-# グラフ8-表示チェックボックス　押下処理

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
/*! リボン

-# オフライン用グラフ-表示チェックボックス　押下処理

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
/*! リボン

-# グラフ1-表示チェックボックス　更新処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateCheckGraph1(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGraph[grapf1]);
}


/*============================================================================*/
/*! リボン

-# グラフ2-表示チェックボックス　更新処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateCheckGraph2(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGraph[grapf2]);
}


/*============================================================================*/
/*! リボン

-# グラフ3-表示チェックボックス　更新処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateCheckGraph3(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGraph[grapf3]);
}


/*============================================================================*/
/*! リボン

-# グラフ4-表示チェックボックス　更新処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateCheckGraph4(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGraph[grapf4]);
}


/*============================================================================*/
/*! リボン

-# グラフ5-表示チェックボックス　更新処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateCheckGraph5(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGraph[grapf5]);
}


/*============================================================================*/
/*! リボン

-# グラフ6-表示チェックボックス　更新処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateCheckGraph6(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGraph[grapf6]);
}


/*============================================================================*/
/*! リボン

-# グラフ7-表示チェックボックス　更新処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateCheckGraph7(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGraph[grapf7]);
}


/*============================================================================*/
/*! リボン

-# グラフ8-表示チェックボックス　更新処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateCheckGraph8(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGraph[grapf8]);
}


/*============================================================================*/
/*! リボン

-# 表示-プロパティウィンドウチェックボックス　押下処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnViewProperties()
{
	m_wndProperties.ShowPane(!m_wndProperties.IsVisible(), FALSE, TRUE);
}


/*============================================================================*/
/*! リボン

-# 表示-プロパティウィンドウチェックボックス　更新処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateViewProperties(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndProperties.IsVisible());
}


/*============================================================================*/
/*! リボン

-# 表示-ファイルビューチェックボックス　押下処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnViewDatalist()
{
	m_wndFileView.ShowPane(!m_wndFileView.IsVisible(), FALSE, TRUE);
}


/*============================================================================*/
/*! リボン

-# 表示-ファイルビューチェックボックス　更新処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateViewDatalist(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndFileView.IsVisible());	// Phase1
}


/*============================================================================*/
/*! リボン

-# 表示-出力ウィンドウチェックボックス　押下処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnViewOutput()
{
	m_wndOutput.ShowPane(!m_wndOutput.IsVisible(), FALSE, TRUE);
}


/*============================================================================*/
/*! リボン

-# 表示-出力ウィンドウチェックボックス　更新処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateViewOutput(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndOutput.IsVisible());
}


/*============================================================================*/
/*! リボン

-# 表示-リミットウィンドウチェックボックス　押下処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnViewLimit()
{
	m_wndLimit.ShowPane(!m_wndLimit.IsVisible(), FALSE, TRUE);
}


/*============================================================================*/
/*! リボン

-# 表示-リミットウィンドウチェックボックス　更新処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateViewLimit(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndLimit.IsVisible());
}

/*============================================================================*/
/*! リボン

-# 表示-ステータスウィンドウチェックボックス　押下処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnViewStatus()
{
	m_wndStatusView.ShowPane(!m_wndStatusView.IsVisible(), FALSE, TRUE);
}


/*============================================================================*/
/*! リボン

-# 表示-ステータスウィンドウチェックボックス　更新処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnUpdateViewStatus(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndStatusView.IsVisible());
}

/*============================================================================*/
/*! リボン

-# グラフ1-Autoクリアボタン 押下処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnBtnGraphClear1()
{
	PostMessage(eMessage_AutoClear, 0, (LPARAM)0);
}


/*============================================================================*/
/*! リボン

-# グラフ2-Autoクリアボタン 押下処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnBtnGraphClear2()
{
	PostMessage(eMessage_AutoClear, 0, (LPARAM)1);
}


/*============================================================================*/
/*! リボン

-# グラフ3-Autoクリアボタン 押下処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnBtnGraphClear3()
{
	PostMessage(eMessage_AutoClear, 0, (LPARAM)2);
}


/*============================================================================*/
/*! リボン

-# グラフ4-Autoクリアボタン 押下処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnBtnGraphClear4()
{
	PostMessage(eMessage_AutoClear, 0, (LPARAM)3);
}


/*============================================================================*/
/*! リボン

-# グラフ5-Autoクリアボタン 押下処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnBtnGraphClear5()
{
	PostMessage(eMessage_AutoClear, 0, (LPARAM)4);
}


/*============================================================================*/
/*! リボン

-# グラフ6-Autoクリアボタン 押下処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnBtnGraphClear6()
{
	PostMessage(eMessage_AutoClear, 0, (LPARAM)5);
}


/*============================================================================*/
/*! リボン

-# グラフ7-Autoクリアボタン 押下処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnBtnGraphClear7()
{
	PostMessage(eMessage_AutoClear, 0, (LPARAM)6);
}


/*============================================================================*/
/*! リボン

-# グラフ8-Autoクリアボタン 押下処理

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnBtnGraphClear8()
{
	PostMessage(eMessage_AutoClear, 0, (LPARAM)7);
}


/*============================================================================*/
/*! リボン

-# グラフ1-Autoクリアボタン 更新処理

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
/*! リボン

-# グラフ2-Autoクリアボタン 更新処理

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
/*! リボン

-# グラフ3-Autoクリアボタン 更新処理

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
/*! リボン

-# グラフ4-Autoクリアボタン 更新処理

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
/*! リボン

-# グラフ5-Autoクリアボタン 更新処理

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
/*! リボン

-# グラフ6-Autoクリアボタン 更新処理

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
/*! リボン

-# グラフ7-Autoクリアボタン 更新処理

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
/*! リボン

-# グラフ8-Autoクリアボタン 更新処理

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
