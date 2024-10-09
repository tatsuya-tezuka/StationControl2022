
// MainFrm.cpp : CMainFrame クラスの実装
//

#include "stdafx.h"
#include "OperationTimeLine.h"

#include "MainFrm.h"
#include "OperationTimeLineDoc.h"
#include "TimeLineView.h"
#include "../StationControl/HookMsgBox.h"

#ifdef MON_UT
#include "MonitorInfo.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CEngValCnv*	CMainFrame::m_pEngval = NULL;
CEngValCnv* CMainFrame::GetEngval()
{
	return m_pEngval;
}
void CMainFrame::SetEngval(CEngValCnv* eval)
{
	m_pEngval = eval;
}
void CMainFrame::DeleteEngval()
{
	TRY
	{
		delete m_pEngval;
		m_pEngval = NULL;
	} CATCH_ALL(e){
		m_pEngval = NULL;
	}
	END_CATCH_ALL
}

BOOL CALLBACK CMonitors::MonitorNumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	CMonitors* pmon = (CMonitors*)dwData;

	// モニタ情報の取得
	MONITORINFOEX mInfo;
	mInfo.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor, &mInfo);

	// モニタ領域の取得
	CRect rect(lprcMonitor);
	pmon->AddMonitor(hMonitor, rect, &mInfo);

	return TRUE;
}

CMonitors::CMonitors()
{
	// マルチモニタ対応
	EnumDisplayMonitors(NULL, NULL, MonitorNumProc, (LPARAM)this);
}

CMonitors::~CMonitors()
{
}

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_MS_PLANNING, &CMainFrame::OnMsPlanning)
	ON_COMMAND(ID_TIMEOFFSET_SET, &CMainFrame::OnTimeoffsetSet)
	ON_COMMAND(ID_OP_STARTTIME_CHG, &CMainFrame::OnOpStarttimeChg)
	ON_COMMAND(ID_OP_ENDTIME_CHG, &CMainFrame::OnOpEndtimeChg)
	ON_COMMAND(ID_FORCED_OP_STOP, &CMainFrame::OnForcedOpStop)
	ON_COMMAND(ID_MP_PLANLIST, &CMainFrame::OnMpPlanlist)
	ON_COMMAND(ID_MP_FORECASTLIST, &CMainFrame::OnMpForecastlist)
	ON_COMMAND(ID_MC_CALIBDIAGNS, &CMainFrame::OnMcCalibdiagns)
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_PASSSTATUS, &CMainFrame::OnViewPassstatus)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PASSSTATUS, &CMainFrame::OnUpdateViewPassstatus)
	ON_COMMAND(ID_VIEW_EQUIPMENT, &CMainFrame::OnViewEquipment)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EQUIPMENT, &CMainFrame::OnUpdateViewEquipment)
	ON_COMMAND(ID_VIEW_EVENT, &CMainFrame::OnViewEvent)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EVENT, &CMainFrame::OnUpdateViewEvent)
	ON_COMMAND(ID_VIEW_LIMITERROR, &CMainFrame::OnViewLimiterror)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIMITERROR, &CMainFrame::OnUpdateViewLimiterror)
	ON_COMMAND(ID_WINDOW_TILE_HORZ, &CMainFrame::OnWindowTileHorz)
	ON_COMMAND(ID_HCOPY_NORMAL, &CMainFrame::OnHcopyNormal)
	ON_UPDATE_COMMAND_UI(ID_HCOPY_NORMAL, &CMainFrame::OnUpdateHcopyNormal)
	ON_COMMAND(ID_HCOPY_REVERSE, &CMainFrame::OnHcopyReverse)
	ON_UPDATE_COMMAND_UI(ID_HCOPY_REVERSE, &CMainFrame::OnUpdateHcopyReverse)
	ON_COMMAND(ID_MH_MANUAL, &CMainFrame::OnMhManual)
	ON_WM_CLOSE()
#ifdef MON_UT
	ON_COMMAND(ID_MON_UT, &CMainFrame::OnMonUt)
	ON_UPDATE_COMMAND_UI(ID_MON_UT, &CMainFrame::OnUpdateMonUt)
#endif
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケーター
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame コンストラクション/デストラクション

CMainFrame::CMainFrame()
{
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);

	mReadThread = NULL;
	mThreadEnd = false;
	m_nPlayStatusSound = eStatusSound_Stop;
	mCloseMessage = true;
	m_pMessageThread = NULL;

	mStatusSoundId = 0;

	for (UINT type = 0; type < eStation_MAX; type++){
		m_bCreatePipeService[type] = false;
	}
}

CMainFrame::~CMainFrame()
{
	if (mCaptionFont.GetSafeHandle())
		mCaptionFont.DeleteObject();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndCaption.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, 1500, -1))
	{
		TRACE0("Failed to create caption bar\n");
		return FALSE;
	}

	m_wndCaption.SetFlatBorder();
	m_wndCaption.SetFont(&afxGlobalData.fontBold);
	m_wndCaption.m_clrBarText = RGB(0, 0, 0);
	CString str = mStationString[theApp.GetSelectStation()];
	m_wndCaption.m_clrBarBackground = mTimeLineStationColor[theApp.GetSelectStation()];
	m_wndCaption.SetText(str, CMFCCaptionBar::ALIGN_CENTER);

	CFont defFont;
	defFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (defFont.GetLogFont(&lf)){
		lf.lfWeight = FW_BOLD;
		lf.lfHeight = 30;
		mCaptionFont.CreateFontIndirect(&lf);
	}
	m_wndCaption.SetFont(&mCaptionFont);

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("メニュー バーを作成できませんでした\n");
		return -1;      // 作成できませんでした。
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// アクティブになったときメニュー バーにフォーカスを移動しない
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	//{
	//	TRACE0("ツール バーの作成に失敗しました。\n");
	//	return -1;      // 作成できませんでした。
	//}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	//m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	//m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	//// ユーザー定義のツール バーの操作を許可します:
	//InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("ステータス バーの作成に失敗しました。\n");
	//	return -1;      // 作成できない場合
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: ツール バーおよびメニュー バーをドッキング可能にしない場合は、この 5 つの行を削除します
	//m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	//DockPane(&m_wndToolBar);


	// Visual Studio 2005 スタイルのドッキング ウィンドウ動作を有効にします
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 スタイルのドッキング ウィンドウの自動非表示動作を有効にします
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	// 固定値に基づいてビジュアル マネージャーと visual スタイルを設定します
	OnApplicationLook(theApp.m_nAppLook);

	//// 拡張ウィンドウ管理ダイアログ ボックスを有効にします
	//EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	//// ツール バーとドッキング ウィンドウ メニューの配置変更を有効にします
	//EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	//// ツール バーのクイック (Alt キーを押しながらドラッグ) カスタマイズを有効にします
	//CMFCToolBar::EnableQuickCustomization();

	//if (CMFCToolBar::GetUserImages() == NULL)
	//{
	//	// ユーザー定義のツール バー イメージを読み込みます
	//	if (m_UserImages.Load(_T(".\\UserImages.bmp")))
	//	{
	//		CMFCToolBar::SetUserImages(&m_UserImages);
	//	}
	//}

	// メニューのパーソナル化 (最近使用されたコマンド) を有効にします
	// TODO: ユーザー固有の基本コマンドを定義し、各メニューをクリックしたときに基本コマンドが 1 つ以上表示されるようにします。
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);

	//CMFCToolBar::SetBasicCommands(lstBasicCommands);

	// メニューを固定する
	DWORD style;
	style = m_wndMenuBar.GetPaneStyle();
	style &= ~CBRS_GRIPPER;
	m_wndMenuBar.SetPaneStyle(style);
	//style = m_wndToolBar.GetPaneStyle();
	//style &= ~CBRS_GRIPPER;
	//m_wndToolBar.SetPaneStyle(style);

	mMem.Create();

	// タイマー起動
	SetTimer(0, 1000, NULL);

#ifdef _DEBUG
	SetTimer(9, 1000, NULL);
#endif

	return 0;
}

/*============================================================================*/
/*! アプリケーション

-# メニューバー切替

@brief	メニューバーを表示モードにより切り替える。

@param
@retval	なし

*/
/*============================================================================*/
void CMainFrame::SetStationMenuBar()
{
#ifdef MON_UT
	m_wndMenuBar.SetDefaultMenuResId(IDR_PASSSTATUS_MON_UT);
#else
	m_wndMenuBar.SetDefaultMenuResId(IDR_PASSSTATUS);
#endif /* MON_UT */
	m_wndMenuBar.RestoreOriginalState();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return TRUE;
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


// CMainFrame メッセージ ハンドラー

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* メニューをスキャンします*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
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
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// 基底クラスが実際の動作を行います。

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// すべてのユーザー定義ツール バーのボタンのカスタマイズを有効にします
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIFrameWndEx::OnSize(nType, cx, cy);

	if (nType == SIZE_MAXIMIZED){
		RECT rect;
		MONITORINFOEX info;
		if (mMonitor.GetCount() != 0 && mMonitor.GetMonitor(1, &rect, &info) == true){
			MoveWindow(&rect);
		}
	}
}


LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
// ADD 2021.11.02 >>>
		//=====================================================================
		//	サウンド
		//=====================================================================
	case	MM_MCINOTIFY:
		if (wParam == MCI_NOTIFY_SUCCESSFUL){
			if (mStatusSoundId == (MCIDEVICEID)lParam)
				mciSendString(_T("play serror from 0 notify"), NULL, 0, m_hWnd);
			else
				mciSendString(_T("play sevent from 0 notify"), NULL, 0, m_hWnd);
		}
		break;
// ADD 2021.11.02 <<<
	case eMessage_TimeLineHandle:
		break;
	case eMessage_TimeLineFinish:
		if (wParam == 0){
			// Stationから終了依頼
			mCloseMessage = false;

			map<CString, CWnd*> mp = theApp.GetDlgMap();
			for (auto itr = mp.begin(); itr != mp.end(); ++itr)
			{
				if (itr->second != NULL)
					itr->second->SendMessage(WM_CLOSE);
			}

			PostMessage(WM_CLOSE, 0, 0);
		}
		break;
	case eMessage_TimeLinePlan:
		break;
	case eMessage_TimeLineForcast:
		break;
	case eMessage_TimeLineEmg:
	case eMessage_TimeLineStop:
		break;
	case ID_PIPE_RECEIVE:
	case ID_PIPE_SEND:
	case ID_PIPE_CREATE:
	case ID_PIPE_FINISH:
	case ID_TIMELINE_MESSAGE:
		messagePipe(message, wParam, lParam);
		break;
	case ID_PIPE_SERVER:
	{
		UINT status;
		status = theApp.AddHistory(message, wParam, lParam);
		if ((theApp.GetView(eTimeViewType_Event))->GetSafeHwnd())		(theApp.GetView(eTimeViewType_Event))->PostMessage(ID_PIPE_RECEIVE, wParam, lParam);
		if ((theApp.GetView(eTimeViewType_LimitError))->GetSafeHwnd())	(theApp.GetView(eTimeViewType_LimitError))->PostMessage(ID_PIPE_RECEIVE, wParam, lParam);
		PlayStatusSound(status);
		break;
	}
		//=====================================================================
		//	コントロール
		//=====================================================================
	case eTimeLineCtrl_AntEmg:
		// アンテナ強制停止
		sendPipe(eANTEMG);
		break;
	case eTimeLineCtrl_SndStop:
		// 送信機停止
		sendPipe(eSENDSTOP);
		break;
	case eTimeLineCtrl_Sound:
		// エラー時のサウンドOFF
		if (lParam != 9){
			sendPipe(eALARMSTOP);
		}
		StopStatusSound();
		break;
	case eTimeLineCtrl_Mute:
		// エラー時のサウンドをミュート
// CHG 2021.11.02 >>>
		//if (lParam != 9){
		//	if (lParam == 0)
		//		sendPipe(eALARMMUTEOFF);
		//	if (lParam == 1)
		//		sendPipe(eALARMMUTEON);
		//}
		//MuteStatusSound();
		if (lParam != 9){
			::SendMessage(theApp.GetCallWnd(), eTIMELINE_SET_MUTE, 0, (LPARAM)lParam);
		}
		else{
			if (lParam != 9){
				if (lParam == 0)
					sendPipe(eALARMMUTEOFF);
				if (lParam == 1)
					sendPipe(eALARMMUTEON);
			}
			//MuteStatusSound();
		}
		MuteStatusSound();
// CHG 2021.11.02 <<<
		break;
		// ADD 2021.12.02 >>>
	case eTIMELINE_SET_MUTE:
	{
		// エラー時のサウンドをミュート
		CMFCButtonEx& btn = m_wndCaption.GetMuteBtnCtrl();
		btn.ExecuteClicked(true, (int)lParam);
		break;
	}
	// ADD 2021.12.02 <<<
	case eTimeLineCtrl_SnapShot:
		// 画面印刷
		OnDirectPrint();
		break;
	case eTimeLineCtrl_Help:
	{
		PostMessage(WM_COMMAND, ID_MH_MANUAL, 0);
	}
	break;

	/**********************************************************************************************
	これ以降、StationControl.exeを呼び出して、情報を取得する
	*/
	case	eTIMELINE_GET_SATNAME:
	{
		TCHAR* p = (TCHAR*)lParam;
		if (theApp.GetCallWnd() == NULL){
			p[0] = '\0';
			return TRUE;
		}
		::SendMessage(theApp.GetCallWnd(), eTIMELINE_GET_SATNAME, 0, (LPARAM)0);
		wsprintf(p, _T("%s"), (LPCTSTR)CString(mMem.Pop()));
		break;
	} //case	eTIMELINE_GET_SATNAME:
	case	eTIMELINE_GET_XS:
	{
		TCHAR* p = (TCHAR*)lParam;
		if (theApp.GetCallWnd() == NULL){
			p[0] = '\0';
			return TRUE;
		}
		// 衛星名を共有メモリにPush
		mMem.Push(CStringA(p));
		::SendMessage(theApp.GetCallWnd(), eTIMELINE_GET_XS, 0, (LPARAM)0);
		// X帯、S帯送信設備使用有無情報取得
		wsprintf(p, _T("%s"), (LPCTSTR)CString(mMem.Pop()));
		break;
	} //case	eTIMELINE_GET_XS:
	case	eTIMELINE_GET_PRED:
	{
		TCHAR* p = (TCHAR*)lParam;
		if (theApp.GetCallWnd() == NULL){
			p[0] = '\0';
			return TRUE;
		}
		// 衛星名を共有メモリにPush
		mMem.Push(CStringA(p));
		::SendMessage(theApp.GetCallWnd(), eTIMELINE_GET_PRED, 0, (LPARAM)0);
		break;
	} //case	eTIMELINE_GET_PRED:
	case	eMessage_UpdateForecastFilePath:
	{
		// 予報値ファイル名を取得
		TCHAR p[_MAX_PATH];
		wsprintf(p, _T("%s"), (LPCTSTR)CString(mMem.Pop()));
		//MessageBox(p);
		if (theApp.GetView(eTimeViewType_OperatioTimeline) != NULL){
			(theApp.GetView(eTimeViewType_OperatioTimeline))->SendMessage(eMessage_UpdateForecastFilePath, 0, (LPARAM)p);
		}
		break;
	}
	/*
	ここまで
	************************************************************************************************/


	default:
		return CMDIFrameWndEx::WindowProc(message, wParam, lParam);
	}
	return TRUE;
}

/*============================================================================*/
/*! 運用タイムライン

-# 共有メモリ受信処理

@param

@retval
*/
/*============================================================================*/
UINT ReadLoopThread(LPVOID pParam)
{
	CMainFrame* pWnd = (CMainFrame*)pParam;

	UINT	mCount = 0;
	while (pWnd->GetThreadEnd() == false){
		if ((pWnd->mMem).Count() != 0){
			pWnd->SendMessage(ID_TIMELINE_MESSAGE, 0, 0);
		}
		Sleep(10);
	}

	return 0;
}

/*============================================================================*/
/*! 運用タイムライン

-# AP層からのメッセージ受信

@param

@retval
*/
/*============================================================================*/
void WINAPI CMainFrame::receiveServerCallBack(LPPIPEBUFFER pbuf)
{
	::SendMessage(theApp.m_pMainWnd->m_hWnd, ID_PIPE_RECEIVE, 1, (LPARAM)pbuf);
}

/*============================================================================*/
/*! 運用タイムライン

-# メイン画面からのメッセージ受信

@param

@retval
*/
/*============================================================================*/
void WINAPI CMainFrame::receiveClientCallBack(LPPIPEBUFFER pbuf)
{
	::SendMessage(theApp.m_pMainWnd->m_hWnd, ID_PIPE_RECEIVE, 0, (LPARAM)pbuf);
}
/*============================================================================*/
/*! プロセス間通信

-# AP層からのメッセージ受信

@param

@retval
*/
/*============================================================================*/
void WINAPI CMainFrame::receiveServerServiceCallBack01(PIPEMONITORACK* pbuf)
{
	// UDSC64
	CGraphMutex::Lock(eMessage);
	deque<PIPEMONITORACK>& data = theApp.GetMessageTemp(0);
	data.push_back(*pbuf);
	CGraphMutex::Unlock(eMessage);
}
void WINAPI CMainFrame::receiveServerServiceCallBack02(PIPEMONITORACK* pbuf)
{
	// USC34
	CGraphMutex::Lock(eMessage);
	deque<PIPEMONITORACK>& data = theApp.GetMessageTemp(2);
	data.push_back(*pbuf);
	CGraphMutex::Unlock(eMessage);
}
void WINAPI CMainFrame::receiveServerServiceCallBack03(PIPEMONITORACK* pbuf)
{
	// USC20
	CGraphMutex::Lock(eMessage);
	deque<PIPEMONITORACK>& data = theApp.GetMessageTemp(3);
	data.push_back(*pbuf);
	CGraphMutex::Unlock(eMessage);
}
void WINAPI CMainFrame::receiveServerServiceCallBack04(PIPEMONITORACK* pbuf)
{
	// UDSC54
	CGraphMutex::Lock(eMessage);
	deque<PIPEMONITORACK>& data = theApp.GetMessageTemp(1);
	data.push_back(*pbuf);
	CGraphMutex::Unlock(eMessage);
}
/*============================================================================*/
/*! プロセス間通信

-# AP層からのメッセージ受信

@param

@retval
*/
/*============================================================================*/
void WINAPI CMainFrame::receiveServerServiceCallBack(PIPEMONITORACK* pbuf)
{
	//if (theApp.GetMessageThread() == 1){
	//	CGraphMutex::Lock(eMessage);
	//	deque<PIPEMONITORACK>& data = theApp.GetMessageTemp();
	//	data.push_back(*pbuf);
	//	CGraphMutex::Unlock(eMessage);
	//	if (data.size() >= 100)
	//		Sleep(10);
	//	return;
	//}

	if (CMainFrame::GetEngval() == NULL){
		// 工学値変換用のクラスを作成
		CString strFilePath = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, theApp.GetSelectStation());
		char szFilePath[_MAX_PATH];
		WideCharToMultiByte(CP_ACP, 0, strFilePath, -1, szFilePath, sizeof(szFilePath), NULL, NULL);
		CMainFrame::SetEngval(new CEngValCnv(szFilePath));
	}


	int err;
	size_t	size;

	BOOL	bAddTrend = FALSE;
	UINT nStation = 0;
	if (pbuf->header.station == 1) nStation = 0;
	if (pbuf->header.station == 2) nStation = 2;
	if (pbuf->header.station == 3) nStation = 3;
	if (pbuf->header.station == 4) nStation = 1;

	// 工学値変換＆画面更新は対象局のみとする
	if (nStation != theApp.GetSelectStation())
		return;

	// StationControlアプリの場合
	switch (pbuf->header.command)
	{
	case ePipeCommand_monitorget:
	{
		DWORD	dwStart = timeGetTime();
		// 監視生データ
		// bufをAppのdequeに登録
		// 局番号が範囲内の場合
		if (pbuf->header.station >= eStation_Usuda64 + 1 && pbuf->header.station <= eStation_MAX)
		{
			if (CMainFrame::GetEngval() != NULL)
			{
				err = CMainFrame::GetEngval()->CalcFromMsg((CEngValCnv::sdtp_msg_ex_t*)&(pbuf->buf[0]));

				if (err == 0)
				{

					vector<CEngValCnv::obs_tbl_res_t>& data = CMainFrame::GetEngval()->GetAllVal();
					size = data.size();

					if (size > 0)
					{
						// 監視データ登録
						theApp.SetEqMonData(nStation, data);
					}
				}
			}
			else{
			}
		}
		DWORD dwElapsed = (timeGetTime() - dwStart);
		TRACE("### ePipeCommand_monitorget Elapsed(%d)\n", dwElapsed);
	}
	break;
	case	ePipeCommand_history:	// 履歴データ通知
		theApp.CreateDataToHistory(pbuf);
		::PostMessage(theApp.m_pMainWnd->m_hWnd, ID_PIPE_SERVER, 9, (LPARAM)pbuf);
		break;
	default:
		return;
		break;
	}

	// 履歴は取得したら直ぐ更新
	// 監視は取得しても更新しないで、CTimeLineView::OnTimerでの1秒更新とする

	//static DWORD mCallbackProcStart = 0;
	//DWORD mDrawInterval = theApp.GetDrawUpdateInterval(false);
	//DWORD dwElaps = timeGetTime() - mCallbackProcStart;
	//if (dwElaps < mDrawInterval)
	//	return;
	//mCallbackProcStart = timeGetTime();

	//::PostMessage(theApp.m_pMainWnd->m_hWnd, ID_PIPE_SERVER, 9, (LPARAM)pbuf);
}

/*============================================================================*/
/*! プロセス間通信

-# メイン画面からのメッセージ受信

@param

@retval
*/
/*============================================================================*/
void WINAPI CMainFrame::receiveClientServiceCallBack(PIPEMONITORACK* pbuf)
{
	::PostMessage(theApp.m_pMainWnd->m_hWnd, ID_PIPE_RECEIVE, 9, (LPARAM)pbuf);
}
/*============================================================================*/
/*! 運用タイムライン

-# プロセス間通信を行うためのパイプ作成

@param

@retval
*/
/*============================================================================*/
void CMainFrame::createPipe()
{
	m_bCreatePipe = false;
	mPipeSend.SetPipeName(mPipeName_TIMELINE);
	mPipeRecv.SetPipeName(mPipeName_TIMELINE + mPipeName_STATION);
	bool ret;
	ret = mPipeSend.Create(false, &receiveServerCallBack, ePIPETYPE_TIMELINE, this);
	if (ret == true){
		ret = mPipeRecv.Create(false, &receiveClientCallBack, ePIPETYPE_TIMELINE, this);
		if (ret == true){
			m_bCreatePipe = true;
		}
		else{
			mPipeSend.Delete();
		}
	}
	sendPipe(eTIMELINESTART);

	// メッセージ収集用スレッドの作成
	CreateMessageThread();

	// 監視、履歴データ取得用パイプの作成
// CHG 2017.10.16 >>>
//	m_bCreatePipeService = false;
//	mPipeServiceSend.SetPipeName(mPipeNameString[ePIPETYPE_STATION]);
//	mPipeServiceRecv.SetPipeName(mPipeNameString[ePIPETYPE_STATION]);
//	ret = mPipeServiceSend.CreateService(false, &receiveServerServiceCallBack, ePIPETYPE_STATION, this);
//	if (ret == true){
//		m_bCreatePipeService = true;
//		SendServicePipe(eSTATIONSTART);
//	}
	m_bCreatePipeService[theApp.GetPipeStationIndex()] = false;
	mPipeServiceSend[theApp.GetPipeStationIndex()].SetPipeName(mPipeNameString[ePIPETYPE_STATION + theApp.GetPipeStationIndex()]);
	mPipeServiceRecv[theApp.GetPipeStationIndex()].SetPipeName(mPipeNameString[ePIPETYPE_STATION + theApp.GetPipeStationIndex()]);
	switch (theApp.GetPipeStationIndex()){
	case	0: ret = mPipeServiceSend[0].CreateService(false, &receiveServerServiceCallBack01, ePIPETYPE_STATION, this); break;
	case	1: ret = mPipeServiceSend[1].CreateService(false, &receiveServerServiceCallBack02, ePIPETYPE_STATION, this); break;
	case	2: ret = mPipeServiceSend[2].CreateService(false, &receiveServerServiceCallBack03, ePIPETYPE_STATION, this); break;
	case	3: ret = mPipeServiceSend[3].CreateService(false, &receiveServerServiceCallBack04, ePIPETYPE_STATION, this); break;
	}
	if (ret == true){
		m_bCreatePipeService[theApp.GetPipeStationIndex()] = true;
		SendServicePipe(eSTATIONSTART, theApp.GetPipeStationIndex());
	}
// CHG 2017.10.16 <<<
}

/*============================================================================*/
/*! 運用タイムライン

-# プロセス間通信を行うためのパイプ削除

@param

@retval
*/
/*============================================================================*/
void CMainFrame::deletePipe()
{
	if (m_bCreatePipe == true){
		sendPipe(eTIMELINEEND);
		// パイプの削除
		mPipeSend.Delete();
		mPipeRecv.Delete();
	}
	m_bCreatePipe = false;
	if (mReadThread != NULL){
		mThreadEnd = true;
		WaitForSingleObject(mReadThread, 10000);
		mReadThread = NULL;
	}
	mMem.Delete();

// CHG 2017.10.16 >>>	
//	if (m_bCreatePipeService == true){
//		SendServicePipe(eSTATIONEND);
//		// パイプの削除
//		mPipeServiceSend.Delete();
//		mPipeServiceRecv.Delete();
//		if (CMainFrame::GetEngval() != NULL){
//			CMainFrame::DeleteEngval();
//		}
//	}
//	m_bCreatePipeService = false;
	if (m_bCreatePipeService[theApp.GetPipeStationIndex()] == true){
		SendServicePipe(eSTATIONEND, theApp.GetPipeStationIndex());
		// パイプの削除
		mPipeServiceSend[theApp.GetPipeStationIndex()].Delete();
		mPipeServiceRecv[theApp.GetPipeStationIndex()].Delete();
		if (CMainFrame::GetEngval() != NULL){
			CMainFrame::DeleteEngval();
		}
	}
	m_bCreatePipeService[theApp.GetPipeStationIndex()] = false;
// CHG 2017.10.16 >>>
}
/*============================================================================*/
/*! プロセス間通信

-# サービスモジュールへのパイプ通信

@param

@retval
*/
/*============================================================================*/
void CMainFrame::SendServicePipe(DWORD command, int nStation)
{
	switch (command)
	{
	case	eSTATIONSTART:
	{
		return;
		PIPESTART buf;
		memset(&buf, 0, sizeof(PIPESTART));
		buf.header.size = sizeof(PIPESTART) - sizeof(PIPEHEADER);
		buf.header.command = ePipeCommand_start;
		if (theApp.GetSelectStation() == eStation_Usuda64)
			buf.header.station = ePipeStation_udsc64;
		if (theApp.GetSelectStation() == eStation_Usuda642)
			buf.header.station = ePipeStation_xdsc54;
		if (theApp.GetSelectStation() == eStation_Uchinoura34)
			buf.header.station = ePipeStation_usc34;
		if (theApp.GetSelectStation() == eStation_Uchinoura20)
			buf.header.station = ePipeStation_usc20;
		DWORD ip = theApp.GetHostIpInfo();
		buf.val[0] = ((BYTE)ip & 0xff);
		buf.val[1] = ((BYTE)(ip >> 8) & 0xff);
		buf.val[2] = ((BYTE)(ip >> 16) & 0xff);
		buf.val[3] = ((BYTE)(ip >> 24) & 0xff);
		mPipeServiceSend[nStation].Send(&buf);
		break;
	}
	case	eSTATIONEND:
	{
		PIPESTART buf;
		memset(&buf, 0, sizeof(PIPESTART));
		buf.header.size = sizeof(PIPESTART) - sizeof(PIPEHEADER);
		buf.header.command = ePipeCommand_stop;
		if (theApp.GetSelectStation() == eStation_Usuda64)
			buf.header.station = ePipeStation_udsc64;
		if (theApp.GetSelectStation() == eStation_Usuda642)
			buf.header.station = ePipeStation_xdsc54;
		if (theApp.GetSelectStation() == eStation_Uchinoura34)
			buf.header.station = ePipeStation_usc34;
		if (theApp.GetSelectStation() == eStation_Uchinoura20)
			buf.header.station = ePipeStation_usc20;
		DWORD ip = theApp.GetHostIpInfo();
		buf.val[0] = ((BYTE)ip & 0xff);
		buf.val[1] = ((BYTE)(ip >> 8) & 0xff);
		buf.val[2] = ((BYTE)(ip >> 16) & 0xff);
		buf.val[3] = ((BYTE)(ip >> 24) & 0xff);
		mPipeServiceSend[nStation].Send(&buf);
		break;
	}
	default:
		return;
	}
}

/*============================================================================*/
/*! プロセス間通信

-# プロセス間通信パイプにコマンドを送る

@param

@retval
*/
/*============================================================================*/
void CMainFrame::sendPipe(DWORD command, CString param/* = _T("")*/)
{
	if (m_bCreatePipe == true){
		PIPEBUFFER buf;
		buf.command = mCommand;
		buf.commandIndex = command;
		memset(buf.buf, 0, mPipeBufferSize + 1);
		if (param.IsEmpty() == false){
			sprintf_s((char*)buf.buf, (mPipeBufferSize + 1), "%s", (LPCSTR)CStringA(param));
		}
		mPipeSend.Send(&buf);
	}
}
/*============================================================================*/
/*! 運用タイムライン

-# プロセス間通信メッセージを処理する

@param		message		メッセージID

@retval
*/
/*============================================================================*/
void CMainFrame::messagePipe(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == ID_PIPE_RECEIVE){
		LPPIPEBUFFER pbuf = (LPPIPEBUFFER)lParam;
		switch (pbuf->commandIndex){
		case	eSTATIONEND:
			mCloseMessage = false;
			PostMessage(WM_CLOSE, 0, 0);
			break;
		case	eHELLO:
			TRACE("## Hello Message\n");
			break;
		case	eSENDMSG:
			TRACE("## Send Message\n");
			break;
		case	eRECVMSG:
			TRACE("## Receive Message\n");
			break;
		case	eANTEMG:
			TRACE("## AntEmg Message\n");
			break;
		case	eSENDSTOP:
			TRACE("## SendStop Message\n");
			break;

		// アラーム関連
		case	eALARMSTOP:
		{
			// タイムライン側から通知
			CMFCButtonEx& btn = m_wndCaption.GetSoundBtnCtrl();
			btn.ExecuteClicked(true, 0);
			break;
		}
		case	eALARMMUTEOFF:
		{
			// タイムライン側から通知
			CMFCButtonEx& btn = m_wndCaption.GetMuteBtnCtrl();
			btn.ExecuteClicked(true, 0);
			break;
		}
		case	eALARMMUTEON:
		{
			// タイムライン側から通知
			CMFCButtonEx& btn = m_wndCaption.GetMuteBtnCtrl();
			btn.ExecuteClicked(true, 1);
			break;
		}

		case	eMONITORDATA:
		{
			// 監視データ通知
			CMonitorDataTemp data;
			CEngValCnv::obs_tbl_res_t val;

			int len = pbuf->bufsize;
			int pos = 0;
			while (len>0){
				memcpy(&val, &pbuf->buf[pos], sizeof(CEngValCnv::obs_tbl_res_t));
				data.mData.push_back(val);
				pos += sizeof(CEngValCnv::obs_tbl_res_t);
				len -= sizeof(CEngValCnv::obs_tbl_res_t);
			}
			int i = 0;
			break;
		}

		case	eHISTORYEQ:
		{
			break;
			CGraphMutex::Lock(eData);
			TRACE("## History Equipment Message\n");
			deque<stHistoryData>& list = theApp.GetEquipmentHistoryData();
			if (list.size() == MAX_HISTORY_DATA){
				list.pop_front();
			}
			stHistoryData data;
			memcpy(&data, pbuf->buf, sizeof(stHistoryData));
			list.push_back(data);
			PlayStatusSound(data.status);
			if ((theApp.GetView(eTimeViewType_LimitError))->GetSafeHwnd())	(theApp.GetView(eTimeViewType_LimitError))->PostMessage(message, 0, 0);
			CGraphMutex::Unlock(eData);
			break;
		}
		case	eHISTORYEVENT:
		{
			break;
			CGraphMutex::Lock(eData);
			TRACE("## History Event Message\n");
			deque<stHistoryData>& list = theApp.GetEventHistoryData();
			if (list.size() == MAX_HISTORY_DATA){
				list.pop_front();
			}
			stHistoryData data;
			memcpy(&data, pbuf->buf, sizeof(stHistoryData));
			list.push_back(data);
			PlayStatusSound(data.status);
			if ((theApp.GetView(eTimeViewType_Event))->GetSafeHwnd())		(theApp.GetView(eTimeViewType_Event))->PostMessage(message, 0, 0);
			CGraphMutex::Unlock(eData);
			break;
		}
		}
		return;

	}
	if (message == ID_PIPE_CREATE){
		createPipe();
		return;
	}
	if (message == ID_TIMELINE_MESSAGE){
		//CStringA str = mMem.Pop();
		//TRACE("### MSG %s\n", str);
		//if (str.GetAt(0) == '0'){
		//	SendMonitorMessage(str);
		//}
		//else{
		//	RecvMonitorMessage(str);
		//}
		return;
	}
}

void CMainFrame::OnDestroy()
{
	// メッセージ収集用スレッドの終了
	RemoveMessageThread();

	deletePipe();

	CMDIFrameWndEx::OnDestroy();
}

void CMainFrame::OnWindowTileHorz()
{
	OnTileHorz();
}

/*============================================================================*/
/*! アプリケーション

-# ウィンドウの並べて表示を行う

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnTileHorz()
{
	CView* pview;
	CWnd* pwnd;
	CRect rc, rect[eTimeViewType_Max], maxRect, workArea;

	// +----------------------++----------------------------------+
	// + タイムライン         ++                                  +
	// +                      ++                                  +
	// +                      ++                                  +
	// +                      ++                                  +
	// +                      ++                                  +
	// +                      ++                                  +
	// +                      ++                                  +
	// +                      ++                                  +
	// +----------------------------------------------------------+

	// MDIの作業領域を取得する
	//GetDlgItem(AFX_IDW_PANE_FIRST)->GetWindowRect(rc);
	GetClientRect(rc);
	CWnd* pWnd = CWnd::FromHandle(m_hWndMDIClient);
	pWnd->GetClientRect(rc);
	workArea = rc;

	//MDITile(MDITILE_HORIZONTAL);
	maxRect = workArea;

	UINT	width, height;
	// 運用状況画面
	width = maxRect.Width() / 2 * 1;
	height = maxRect.Height();
	pview = theApp.GetView(eTimeViewType_OperatioTimeline);
	if (pview){
		pwnd = pview->GetParent();
		rect[eTimeViewType_OperatioTimeline].left = 0;
		rect[eTimeViewType_OperatioTimeline].top = 0;
		rect[eTimeViewType_OperatioTimeline].right = rect[eTimeViewType_OperatioTimeline].left + width;
		rect[eTimeViewType_OperatioTimeline].bottom = maxRect.bottom;
		pwnd->SetWindowPos(&wndTop, rect[eTimeViewType_OperatioTimeline].left, rect[eTimeViewType_OperatioTimeline].top, rect[eTimeViewType_OperatioTimeline].Width(), rect[eTimeViewType_OperatioTimeline].Height(), SWP_NOZORDER);
	}
	// 設備状況画面
	width = maxRect.Width() / 2 * 1;
	height = maxRect.Height() / 2 * 1;
	pview = theApp.GetView(eTimeViewType_EquipmentSystem);
	if (pview){
		pwnd = pview->GetParent();
		rect[eTimeViewType_EquipmentSystem].left = rect[eTimeViewType_OperatioTimeline].right;
		rect[eTimeViewType_EquipmentSystem].top = 0;
		rect[eTimeViewType_EquipmentSystem].right = maxRect.right;
		rect[eTimeViewType_EquipmentSystem].bottom = rect[eTimeViewType_EquipmentSystem].top + height;
		pwnd->SetWindowPos(&wndTop, rect[eTimeViewType_EquipmentSystem].left, rect[eTimeViewType_EquipmentSystem].top, rect[eTimeViewType_EquipmentSystem].Width(), rect[eTimeViewType_EquipmentSystem].Height(), SWP_NOZORDER);
	}

	// イベントログ
	width = maxRect.Width() / 2 * 1;
	height = maxRect.Height() / 4 * 1;
	pview = theApp.GetView(eTimeViewType_Event);
	if (pview){
		pwnd = pview->GetParent();
		rect[eTimeViewType_Event].left = rect[eTimeViewType_OperatioTimeline].right;
		rect[eTimeViewType_Event].top = rect[eTimeViewType_EquipmentSystem].bottom;
		rect[eTimeViewType_Event].right = maxRect.right;
		rect[eTimeViewType_Event].bottom = rect[eTimeViewType_Event].top + height;
		pwnd->SetWindowPos(&wndTop, rect[eTimeViewType_Event].left, rect[eTimeViewType_Event].top, rect[eTimeViewType_Event].Width(), rect[eTimeViewType_Event].Height(), SWP_NOZORDER);
	}

	// リミットエラーログ
	width = maxRect.Width() / 2 * 1;
	height = maxRect.Height() / 4 * 1;
	pview = theApp.GetView(eTimeViewType_LimitError);
	if (pview){
		pwnd = pview->GetParent();
		rect[eTimeViewType_LimitError].left = rect[eTimeViewType_OperatioTimeline].right;
		rect[eTimeViewType_LimitError].top = rect[eTimeViewType_Event].bottom;
		rect[eTimeViewType_LimitError].right = maxRect.right;
		rect[eTimeViewType_LimitError].bottom = maxRect.bottom;
		pwnd->SetWindowPos(&wndTop, rect[eTimeViewType_LimitError].left, rect[eTimeViewType_LimitError].top, rect[eTimeViewType_LimitError].Width(), rect[eTimeViewType_LimitError].Height(), SWP_NOZORDER);
	}
}


void CMainFrame::OnMsPlanning()
{
	// 運用計画立案画面呼び出し
	sendPipe(eTIMELINE_OPEPLAN);
}


void CMainFrame::OnTimeoffsetSet()
{
	// 時刻オフセット画面呼び出し
	sendPipe(eTIMELINE_TIMEOFFSET);
}


void CMainFrame::OnOpStarttimeChg()
{
	// 計画開始時刻変更画面呼び出し
	sendPipe(eTIMELINE_PLANSTARTTIME);
}


void CMainFrame::OnOpEndtimeChg()
{
	// 計画終了時刻変更画面呼び出し
	sendPipe(eTIMELINE_PLANSTOPTIME);
}


void CMainFrame::OnForcedOpStop()
{
	// 強制運用停止確認画面呼び出し
	sendPipe(eTIMELINE_PLANEMGSTOP);
}


void CMainFrame::OnMpPlanlist()
{
	// 運用計画一覧画面呼び出し
	sendPipe(eTIMELINE_FORECASTPLAN);
}


void CMainFrame::OnMpForecastlist()
{
	// 予報値一覧画面呼び出し
	sendPipe(eTIMELINE_FORECAST);
}


void CMainFrame::OnMcCalibdiagns()
{
	// 校正診断画面呼び出し
	sendPipe(eTIMELINE_CALIBRATION);
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 0){
		m_wndCaption.RedrawWindow();
		return;
	}

	if (nIDEvent == 9){
		PIPEMONITORACK buf;
		buf.header.size = 137;
		theApp.CreateDataToHistory(&buf);
		PostMessage(ID_PIPE_SERVER, 0, 0);
		return;
	}

	CMDIFrameWndEx::OnTimer(nIDEvent);
}


void CMainFrame::OnViewPassstatus()
{
	if (theApp.GetView(eTimeViewType_OperatioTimeline) == NULL)
		theApp.OpenView(eTimeViewType_OperatioTimeline);
	else
	{
//		theApp.CloseView(eTimeViewType_OperatioTimeline);
		CView *view = theApp.GetView(eTimeViewType_OperatioTimeline);
		((CMDIChildWnd*)view->GetParent())->MDIActivate();
	}
}


void CMainFrame::OnUpdateViewPassstatus(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetView(eTimeViewType_OperatioTimeline) != NULL) ? 1 : 0);
}


void CMainFrame::OnViewEquipment()
{
	if (theApp.GetView(eTimeViewType_EquipmentSystem) == NULL)
		theApp.OpenView(eTimeViewType_EquipmentSystem);
	else
	{
//		theApp.CloseView(eTimeViewType_EquipmentSystem);
		CView *view = theApp.GetView(eTimeViewType_EquipmentSystem);
		((CMDIChildWnd*)view->GetParent())->MDIActivate();
	}
}


void CMainFrame::OnUpdateViewEquipment(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetView(eTimeViewType_EquipmentSystem) != NULL) ? 1 : 0);
}


void CMainFrame::OnViewEvent()
{
	if (theApp.GetView(eTimeViewType_Event) == NULL)
		theApp.OpenView(eTimeViewType_Event);
	else
	{
//		theApp.CloseView(eTimeViewType_Event);
		CView *view = theApp.GetView(eTimeViewType_Event);
		((CMDIChildWnd*)view->GetParent())->MDIActivate();
	}
}


void CMainFrame::OnUpdateViewEvent(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetView(eTimeViewType_Event) != NULL) ? 1 : 0);
}


void CMainFrame::OnViewLimiterror()
{
	if (theApp.GetView(eTimeViewType_LimitError) == NULL)
		theApp.OpenView(eTimeViewType_LimitError);
	else
	{
//		theApp.CloseView(eTimeViewType_LimitError);
		CView *view = theApp.GetView(eTimeViewType_LimitError);
		((CMDIChildWnd*)view->GetParent())->MDIActivate();
	}
}


void CMainFrame::OnUpdateViewLimiterror(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetView(eTimeViewType_LimitError) != NULL) ? 1 : 0);
}

void CMainFrame::OnHcopyNormal()
{
	theApp.SetHcopyType(ID_HCOPY_NORMAL);
}
void CMainFrame::OnUpdateHcopyNormal(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetHcopyType() == ID_HCOPY_NORMAL) ? 1 : 0);
}
void CMainFrame::OnHcopyReverse()
{
	theApp.SetHcopyType(ID_HCOPY_REVERSE);
}
void CMainFrame::OnUpdateHcopyReverse(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetHcopyType() == ID_HCOPY_REVERSE) ? 1 : 0);
}

// 取扱説明書表示
void CMainFrame::OnMhManual()
{
//	AfxMessageBox(_T("DUMMY ！！！取説表示"), MB_OK | MB_ICONEXCLAMATION);
	CString strDocFilePath;
	strDocFilePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppPath(), (LPCTSTR)CString(DOCUMENT_FILENAME));
	// 実行
	ShellExecute(NULL, _T("open"), (LPTSTR)strDocFilePath.GetBuffer(), NULL, NULL, SW_HIDE);
	strDocFilePath.ReleaseBuffer();
}

/*============================================================================*/
/*! アプリケーション

-# リアルタイムの履歴情報から警告、エラー時はサウンド再生を行う

@param	status	履歴ステータス
@retval	なし

*/
/*============================================================================*/
void CMainFrame::PlayStatusSound(UINT status)
{
	// ADD 2021.11.02 >>>
	if (status == eHistStatus_Fault){
		// 鳴動指示なのでボタン状態をチェックする
		if (m_wndCaption.GetMuteBtnCtrl().GetCheck() == 0){
			// Mute_Off状態
			if (m_nPlayStatusSound == eStatusSound_Mute){
				m_nPlayStatusSound = eStatusSound_Stop;
			}
		}
		else if (m_wndCaption.GetMuteBtnCtrl().GetCheck() != 0){
			// Mute_On状態
			if (m_nPlayStatusSound != eStatusSound_Mute){
				MuteStatusSound();
			}
		}
	}
	// ADD 2021.11.02 <<<

	if (m_nPlayStatusSound == eStatusSound_Mute){
		return;
	}
	if (m_nPlayStatusSound != eStatusSound_Stop){
		return;
	}

	// アラーム時のみ鳴動
	if (status != eHistStatus_Fault)
	{
		return;
	}

	//mciSendString(_T("open StatusError.wav type waveaudio alias serror"), NULL, 0, NULL);
	//mStatusSoundId = mciGetDeviceID(_T("serror"));
	//mciSendString(_T("play serror from 0 notify"), NULL, 0, m_hWnd);
	//m_nPlayStatusSound = eStatusSound_Play;

	WCHAR strCurDir[1024];
	GetCurrentDirectory(1024, strCurDir);

	CString strOpenMsg;
	strOpenMsg.Format(_T("open %sStatusError.wav type waveaudio alias serror"), (LPCTSTR)theApp.GetAppPath());
	int err = mciSendString((LPCTSTR)strOpenMsg, NULL, 0, NULL);
	mStatusSoundId = mciGetDeviceID(_T("serror"));
	err = mciSendString(_T("play serror from 0 notify"), NULL, 0, m_hWnd);
	m_nPlayStatusSound = eStatusSound_Play;

	// ボタンカラー変更
	m_wndCaption.GetSoundBtnCtrl().SetFaceColor(RGB(0, 255, 0));
}

/*============================================================================*/
/*! アプリケーション

-# リアルタイムの履歴情報からの警告、エラー時のサウンドを停止する

@param
@retval	なし

*/
/*============================================================================*/
void CMainFrame::StopStatusSound()
{
	if (m_nPlayStatusSound == eStatusSound_Mute){
		return;
	}
	if (m_nPlayStatusSound == eStatusSound_Play){
		mciSendString(_T("close serror"), NULL, 0, NULL);
		m_nPlayStatusSound = eStatusSound_Stop;
		mStatusSoundId = 0;
	}
}

/*============================================================================*/
/*! アプリケーション

-# リアルタイムの履歴情報から警告、エラー時のサウンド再生を禁止する

@param
@retval	なし

*/
/*============================================================================*/
void CMainFrame::MuteStatusSound()
{
	if (m_nPlayStatusSound == eStatusSound_Mute){
		m_nPlayStatusSound = eStatusSound_Stop;
		return;
	}
	StopStatusSound();
	m_nPlayStatusSound = eStatusSound_Mute;
}

/*============================================================================*/
/*! アプリケーション

-# 画面ハードコピー

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnDirectPrint()
{
	// 一応全画面
	// アクティブウィンドウをハードコピーする場合はアクティブのウィンドウハンドルを渡す
	//SetTimer(1, 100, NULL);
	CString	strExePath, strParam;
	if (theApp.GetHcopyType() == ID_HCOPY_NORMAL)
		strExePath.Format(_T("C:\\HardCopy\\NormalHardCopy.exe"));
	else
		strExePath.Format(_T("C:\\HardCopy\\ReverseHardCopy.exe"));
	strParam.Empty();
	::ShellExecute(m_hWnd, _T(""), strExePath, strParam, NULL, SW_NORMAL);
	//DirectPrint(this);
}




void CMainFrame::OnClose()
{
	if (mCloseMessage == true){
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("終了します。\nよろしいですか？"), _T("OperationTimeLine"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return;
	}

	CMDIFrameWndEx::OnClose();
}

/*============================================================================*/
/*! アプリケーション

-# メッセージ収集スレッドの作成

@param

@retval void
*/
/*============================================================================*/
void CMainFrame::CreateMessageThread()
{
	if (theApp.GetMessageThread() == 1){
		if (m_pMessageThread == NULL){
			m_pMessageThread = (CMessageThread*)AfxBeginThread(RUNTIME_CLASS(CMessageThread), THREAD_PRIORITY_HIGHEST/*THREAD_PRIORITY_NORMAL*/, 0, CREATE_SUSPENDED);
			m_pMessageThread->ResumeThread();
		}
	}
}

/*============================================================================*/
/*! アプリケーション

-# メッセージ収集スレッドの作成

@param

@retval void
*/
/*============================================================================*/
void CMainFrame::RemoveMessageThread()
{
	if (m_pMessageThread != NULL){
		m_pMessageThread->StopThread();
		WaitForSingleObject(m_pMessageThread->m_hThread, 10000);
		delete m_pMessageThread;
		m_pMessageThread = NULL;
	}
}

#ifdef MON_UT
/*============================================================================*/
// 監視情報ダイログ
/*============================================================================*/
void CMainFrame::OnMonUt()
{
	CMonitorInfo dlg;
	dlg.DoModal();
	// TODO: ここにコマンド ハンドラー コードを追加します。
}

/*============================================================================*/
// 監視情報ダイログ
/*============================================================================*/
void CMainFrame::OnUpdateMonUt(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}
#endif

