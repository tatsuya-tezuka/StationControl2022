
// MainFrm.cpp : CMainFrame クラスの実装
//

#include "stdafx.h"
#include "StationControl.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "DialogBase.h"
#include "ImageTool.h"
#include "TrendGraphView.h"
#include "SummaryPropertiesDlg.h"
#include "SpectrumVerticalSetDlg.h"
#include "SummarySpectrumPropertiesDlg.h"
#include "TimeLineView.h"
#include "AntennaHistroyView.h"
#include "..\Utility\SshShell.h"
#include "HookMsgBox.h"
#include "CidSelect.h"

#include "EquipmentStatusView.h"
#include "EquipmentStatusView54.h"
#include "EquipmentStatusView34.h"
#include "EquipmentStatusView20.h"
#include "EquipmentStationStatusView.h"

#include "TrsControlXX54.h"
#include "FrequencyCtrl54.h"
#include "SatelliteCapture54.h"

#include "SelectLNADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWnd* CMainFrame::m_pPipeMessageWnd = NULL;
CWnd* CMainFrame::GetPipeMessageWnd()
{
	return m_pPipeMessageWnd;
}
void CMainFrame::SetPipeMessageWnd(CWnd* p)
{
	m_pPipeMessageWnd = p;
}

CEngValCnv*	CMainFrame::m_pEngval[eStation_MAX] = { NULL, NULL, NULL, NULL };
CEngValCnv* CMainFrame::GetEngval(int pos)
{
	return m_pEngval[pos];
}
void CMainFrame::SetEngval(int pos, CEngValCnv* eval)
{
	m_pEngval[pos] = eval;
}
void CMainFrame::DeleteEngval(int pos)
{
	TRY
	{
		if (m_pEngval[pos] != NULL)
		{
			delete m_pEngval[pos];
			m_pEngval[pos] = NULL;
		}
	} CATCH_ALL(e){
		m_pEngval[pos] = NULL;
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

static UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケーター
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
	//ID_INDICATOR_TIME,
};

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_WINDOW_TILE_HORZ, &CMainFrame::OnWindowTileHorz)
	ON_COMMAND(ID_MS_PLANNING, &CMainFrame::OnMsPlanning)
	ON_COMMAND(ID_MS_ORBITDATA, &CMainFrame::OnMsOrbitdata)
	ON_COMMAND(ID_MS_SESSIONSTATE4, &CMainFrame::OnMsSessionstate4)
	ON_COMMAND(ID_TIMEOFFSET_SET, &CMainFrame::OnMsTimeOffsetSetting)
	ON_COMMAND(ID_OP_STARTTIME_CHG, &CMainFrame::OnMsStartTimeChange)
	ON_COMMAND(ID_OP_ENDTIME_CHG, &CMainFrame::OnMsEndTimeChange)
	ON_COMMAND(ID_FORCED_OP_STOP, &CMainFrame::OnMsForcedOperationStop)
	ON_COMMAND(ID_MC_CALIBDIAGNS_STOP, &CMainFrame::OnMsCalibStpDel)
	ON_COMMAND(ID_VIEW_TREND, &CMainFrame::OnViewTrend)
	ON_COMMAND(ID_VIEW_OPEDETAIL, &CMainFrame::OnViewOpeDetail)
	ON_COMMAND(ID_VIEW_STATIONSIT, &CMainFrame::OnViewStationSit)
	ON_COMMAND(ID_VIEW_EQOPERATION, &CMainFrame::OnViewEqOperation)
	ON_COMMAND(ID_VIEW_EQOPERATION2, &CMainFrame::OnViewEqOperation2)
	ON_COMMAND(ID_VIEW_EQOPERATION3, &CMainFrame::OnViewEqOperation3)
	ON_COMMAND(ID_VIEW_EQOPERATION4, &CMainFrame::OnViewEqOperation4)
	ON_COMMAND(ID_VIEW_EQOPERATION5, &CMainFrame::OnViewEqOperation5)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TREND, &CMainFrame::OnUpdateViewTrend)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OPEDETAIL, &CMainFrame::OnUpdateViewOpeDetail)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATIONSIT, &CMainFrame::OnUpdateViewStationSit)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EQOPERATION, &CMainFrame::OnUpdateViewEqOperation)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EQOPERATION2, &CMainFrame::OnUpdateViewEqOperation2)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EQOPERATION3, &CMainFrame::OnUpdateViewEqOperation3)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EQOPERATION4, &CMainFrame::OnUpdateViewEqOperation4)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EQOPERATION5, &CMainFrame::OnUpdateViewEqOperation5)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//ON_COMMAND(ID_INDICATOR_TIME, &CMainFrame::OnCommandStatusTime)
	//ON_UPDATE_COMMAND_UI(ID_INDICATOR_TIME, &CMainFrame::OnUpdateStatusTime)
	ON_COMMAND(ID_DIRECT_PRINT, &CMainFrame::OnDirectPrint)
	ON_COMMAND(ID_MS_OPETIMELINE, &CMainFrame::OnViewOpeTimeLine)
	ON_UPDATE_COMMAND_UI(ID_MS_OPETIMELINE, &CMainFrame::OnUpdateViewOpeTimeLine)
//	ON_COMMAND(ID_MG_TRENDPROPERTIES, &CMainFrame::OnMgTrendProparties)
	ON_COMMAND(ID_MG_AGC, &CMainFrame::OnMgAgcGraph)
	ON_COMMAND(ID_MG_RNG, &CMainFrame::OnMgRngRRGraph)
	ON_COMMAND(ID_MG_EL, &CMainFrame::OnMgAGCRNGELGraph)
	ON_COMMAND(ID_MG_TREND, &CMainFrame::OnMgTrendGraph)
	ON_COMMAND(ID_MG_SPECTRUM, &CMainFrame::OnMgSpectrumGraph)
	ON_COMMAND(ID_MG_RNGMONITOR, &CMainFrame::OnMgRngMonitor)
	ON_COMMAND(ID_MO_MANUALINIT, &CMainFrame::OnMoManualinitialSetting)
	ON_COMMAND(ID_MO_TCTRL, &CMainFrame::OnMoTransmissionCtrl)
	ON_COMMAND(ID_MO_SEQUENCE, &CMainFrame::OnMoSequentialCtrl)
	ON_COMMAND(ID_MP_PLANLIST, &CMainFrame::OnMpPlanningList)
	ON_COMMAND(ID_MP_FORECASTLIST, &CMainFrame::OnMpForecastList)
	ON_COMMAND(ID_MP_FORECASTLIST_SELBTN, &CMainFrame::OnMpForecastListSelBtn)
	ON_COMMAND(ID_MD_DIARY, &CMainFrame::OnMdDiary)
	ON_COMMAND(ID_MC_CALIBDIAGNS, &CMainFrame::OnMcCalibDiagns)
	ON_COMMAND(ID_ME_MDATAOUT, &CMainFrame::OnMeMntrDataDmpOutput)
	ON_COMMAND(ID_ME_DUMPFILE, &CMainFrame::OnMeDumpFileSel)
	ON_COMMAND(ID_ME_ANTEQIPOPE, &CMainFrame::OnMeAntEqipOpeHis)
	ON_COMMAND(ID_ME_ANTEQIPDRV, &CMainFrame::OnMeAntEqipDrvHis)
	ON_COMMAND(ID_ME_OPE_REALTIME, &CMainFrame::OnMeOpeHisRealtime)
	ON_COMMAND(ID_ME_OPE_SEARCH, &CMainFrame::OnMeOpeHisSearch)
	ON_COMMAND(ID_ME_CTRL_REALTIME, &CMainFrame::OnMeCtrlHisRealtime)
	ON_COMMAND(ID_ME_CTRL_SEARCH, &CMainFrame::OnMeCtrlHisSearch)
	ON_COMMAND(ID_ME_EQIP_REALTIME, &CMainFrame::OnMeEqipHisRealtime)
	ON_COMMAND(ID_ME_EQIP_SEARCH, &CMainFrame::OnMeEqipHisSearch)
	ON_COMMAND(ID_ME_EVENT_REALTIME, &CMainFrame::OnMeEventHisRealtime)
	ON_COMMAND(ID_ME_EVENT_SEARCH, &CMainFrame::OnMeEventHisSearch)
	ON_COMMAND(ID_MI_SESSION, &CMainFrame::OnMiSessionSet)
	ON_COMMAND(ID_MR_FREQCTRL, &CMainFrame::OnMrFreqCtrl)
	ON_COMMAND(ID_MR_SATLCAP, &CMainFrame::OnMrSatlCap)
	ON_COMMAND(ID_MT_AGCTBL_READ, &CMainFrame::OnMtAgcTblRead)
	ON_COMMAND(ID_MT_AGCTBL_LNA_SELECT, &CMainFrame::OnMtAgcTblSelectLNA)
	ON_COMMAND(ID_MT_AGCTBL_FILECPY, &CMainFrame::OnMtAgcTblFileCpy)
	ON_COMMAND(ID_MT_AGCTBL_FILERST, &CMainFrame::OnMtAgcTblFileRst)
	ON_COMMAND(ID_MT_STATBL_FILECPY, &CMainFrame::OnMtStaTblFileCpy)
	ON_COMMAND(ID_MT_STATBL_FILERST, &CMainFrame::OnMtStaTblFileRst)
	ON_COMMAND(ID_MT_LAYOUT_SAVE, &CMainFrame::OnMtLayoutSave)
	ON_COMMAND(ID_MT_LAYOUT_RESTOR, &CMainFrame::OnMtLayoutRestor)
	ON_WM_DESTROY()
	//ON_WM_NCACTIVATE()
	//ON_WM_NCPAINT()
//	ON_UPDATE_COMMAND_UI(ID_MG_TRENDPROPERTIES, &CMainFrame::OnUpdateMgTrendProparties)
	ON_UPDATE_COMMAND_UI(ID_MG_AGC, &CMainFrame::OnUpdateMgAgcGraph)
	ON_UPDATE_COMMAND_UI(ID_MG_RNG, &CMainFrame::OnUpdateMgRngRRGraph)
	ON_UPDATE_COMMAND_UI(ID_MG_EL, &CMainFrame::OnUpdateMgAGCRNGELGraph)
	ON_UPDATE_COMMAND_UI(ID_MG_TREND, &CMainFrame::OnUpdateMgTrendGraph)
	ON_UPDATE_COMMAND_UI(ID_MG_SPECTRUM, &CMainFrame::OnUpdateMgSpectrumGraph)
	ON_UPDATE_COMMAND_UI(ID_MG_RNGMONITOR, &CMainFrame::OnUpdateMgRngMonitor)
	ON_UPDATE_COMMAND_UI(ID_MO_MANUALINIT, &CMainFrame::OnUpdateMoManualinitialSetting)
	ON_UPDATE_COMMAND_UI(ID_MO_TCTRL, &CMainFrame::OnUpdateMoTransmissionCtrl)
	ON_UPDATE_COMMAND_UI(ID_MO_SEQUENCE, &CMainFrame::OnUpdateMoSequentialCtrl)
	ON_UPDATE_COMMAND_UI(ID_MP_PLANLIST, &CMainFrame::OnUpdateMpPlanningList)
	ON_UPDATE_COMMAND_UI(ID_MP_FORECASTLIST, &CMainFrame::OnUpdateMpForecastList)
	ON_UPDATE_COMMAND_UI(ID_MD_DIARY, &CMainFrame::OnUpdateMdDiary)
	ON_UPDATE_COMMAND_UI(ID_MC_CALIBDIAGNS, &CMainFrame::OnUpdateMcCalibDiagns)
	ON_UPDATE_COMMAND_UI(ID_ME_MDATAOUT, &CMainFrame::OnUpdateMeMntrDataDmpOutput)
	ON_UPDATE_COMMAND_UI(ID_ME_DUMPFILE, &CMainFrame::OnUpdateMeDumpFileSel)
	ON_UPDATE_COMMAND_UI(ID_ME_ANTEQIPOPE, &CMainFrame::OnUpdateMeAntEqipOpeHis)
	ON_UPDATE_COMMAND_UI(ID_ME_ANTEQIPDRV, &CMainFrame::OnUpdateMeAntEqipDrvHis)
	ON_UPDATE_COMMAND_UI(ID_ME_OPE_REALTIME, &CMainFrame::OnUpdateMeOpeHisRealtime)
	ON_UPDATE_COMMAND_UI(ID_ME_OPE_SEARCH, &CMainFrame::OnUpdateMeOpeHisSearch)
	ON_UPDATE_COMMAND_UI(ID_ME_CTRL_REALTIME, &CMainFrame::OnUpdateMeCtrlHisRealtime)
	ON_UPDATE_COMMAND_UI(ID_ME_CTRL_SEARCH, &CMainFrame::OnUpdateMeCtrlHisSearch)
	ON_UPDATE_COMMAND_UI(ID_ME_EQIP_REALTIME, &CMainFrame::OnUpdateMeEqipHisRealtime)
	ON_UPDATE_COMMAND_UI(ID_ME_EQIP_SEARCH, &CMainFrame::OnUpdateMeEqipHisSearch)
	ON_UPDATE_COMMAND_UI(ID_ME_EVENT_REALTIME, &CMainFrame::OnUpdateMeEventHisRealtime)
	ON_UPDATE_COMMAND_UI(ID_ME_EVENT_SEARCH, &CMainFrame::OnUpdateMeEventHisSearch)
	ON_UPDATE_COMMAND_UI(ID_MI_SESSION, &CMainFrame::OnUpdateMiSessionSet)
	ON_UPDATE_COMMAND_UI(ID_MS_SESSIONSTATE4, &CMainFrame::OnUpdateMiSessionSet4)
	ON_UPDATE_COMMAND_UI(ID_MR_FREQCTRL, &CMainFrame::OnUpdateMrFreqCtrl)
	ON_UPDATE_COMMAND_UI(ID_MR_SATLCAP, &CMainFrame::OnUpdateMrSatlCap)
	ON_UPDATE_COMMAND_UI(ID_MT_AGCTBL_READ, &CMainFrame::OnUpdateMtAgcTblRead)
	ON_UPDATE_COMMAND_UI(ID_MT_AGCTBL_LNA_SELECT, &CMainFrame::OnUpdateMtAgcTblSelectLNA)
	ON_UPDATE_COMMAND_UI(ID_MT_AGCTBL_FILECPY, &CMainFrame::OnUpdateMtAgcTblFileCpy)
	ON_UPDATE_COMMAND_UI(ID_MT_AGCTBL_FILERST, &CMainFrame::OnUpdateMtAgcTblFileRst)
	ON_UPDATE_COMMAND_UI(ID_MT_STATBL_FILECPY, &CMainFrame::OnUpdateMtStaTblFileCpy)
	ON_UPDATE_COMMAND_UI(ID_MT_STATBL_FILERST, &CMainFrame::OnUpdateMtStaTblFileRst)
	ON_COMMAND(ID_ME_EQIPSTATUSOUT, &CMainFrame::OnMeEqipStatusOut)
	ON_UPDATE_COMMAND_UI(ID_ME_EQIPSTATUSOUT, &CMainFrame::OnUpdateMeEqipStatusOut)
	ON_COMMAND(ID_STATION_10, &CMainFrame::OnStation10)
	ON_UPDATE_COMMAND_UI(ID_STATION_10, &CMainFrame::OnUpdateStation10)
	ON_COMMAND(ID_STATION_USUDA64, &CMainFrame::OnStationUsuda64)
	ON_UPDATE_COMMAND_UI(ID_STATION_USUDA64, &CMainFrame::OnUpdateStationUsuda64)
	ON_COMMAND(ID_STATION_USUDA2, &CMainFrame::OnStationUsuda2)
	ON_UPDATE_COMMAND_UI(ID_STATION_USUDA2, &CMainFrame::OnUpdateStationUsuda2)
	ON_COMMAND(ID_STATION_UCHINOURA34, &CMainFrame::OnStationUchinoura34)
	ON_UPDATE_COMMAND_UI(ID_STATION_UCHINOURA34, &CMainFrame::OnUpdateStationUchinoura34)
	ON_COMMAND(ID_STATION_UCHINOURA20, &CMainFrame::OnStationUchinoura20)
	ON_UPDATE_COMMAND_UI(ID_STATION_UCHINOURA20, &CMainFrame::OnUpdateStationUchinoura20)
	ON_COMMAND(ID_MP_TIMELINE, &CMainFrame::OnMpTimeline)
	ON_UPDATE_COMMAND_UI(ID_MP_TIMELINE, &CMainFrame::OnUpdateMpTimeline)
	ON_COMMAND(ID_HCOPY_NORMAL, &CMainFrame::OnHcopyNormal)
	ON_UPDATE_COMMAND_UI(ID_HCOPY_NORMAL, &CMainFrame::OnUpdateHcopyNormal)
	ON_COMMAND(ID_HCOPY_REVERSE, &CMainFrame::OnHcopyReverse)
	ON_UPDATE_COMMAND_UI(ID_HCOPY_REVERSE, &CMainFrame::OnUpdateHcopyReverse)
	ON_COMMAND(ID_MP_FORECAST_DETAIL, &CMainFrame::OnMpForecastDetail)
	ON_COMMAND(ID_MO_EQUIPSTATUS, &CMainFrame::OnMoEquipstatus)
	ON_UPDATE_COMMAND_UI(ID_MO_EQUIPSTATUS, &CMainFrame::OnUpdateMoEquipstatus)
	ON_COMMAND(ID_MH_MANUAL, &CMainFrame::OnMhManual)
	ON_COMMAND(ID_ME_EQIP_ALARM, &CMainFrame::OnMeEqipAlarm)
	ON_UPDATE_COMMAND_UI(ID_ME_EQIP_ALARM, &CMainFrame::OnUpdateMeEqipAlarm)
#ifdef MON_UT
	ON_COMMAND(ID_MON_UT, &CMainFrame::OnMonUt)
	ON_UPDATE_COMMAND_UI(ID_MON_UT, &CMainFrame::OnUpdateMonUt)
#endif
	END_MESSAGE_MAP()

// CMainFrame コンストラクション/デストラクション

CMainFrame::CMainFrame()
{
	// TODO: メンバー初期化コードをここに追加してください。
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);

	m_pTimeLineThreadsEvent = new CEvent(FALSE, TRUE);
	ASSERT_VALID(m_pTimeLineThreadsEvent);
	for (UINT type = 0; type<ePIPETYPE_MAX; type++){
		m_bCreatePipe[type] = false;
	}

	for (UINT type = 0; type < eStation_MAX; type++){
		m_pStationThreadsEvent[type] = new CEvent(FALSE, TRUE);
		theApp.SetLaunchStation(type, false);
		m_bCreatePipeService[type] = false;
	}
	m_nPlayStatusSound = eStatusSound_Stop;
	m_pTimelineThread = NULL;

//	m_pMessageThread = NULL;
	for (int i = 0; i < eStation_MAX; i++){
		m_pMessageStationThread[i] = NULL;
	}

	mStatusSoundId = 0;

	m_EventSoundId = 0;
	m_dwEventStatus = 0;
	m_dwEventStart = 0;

	m_bRdnsChkPlanForceStop = false;

//	m_nLastSizeType = 0;
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
	if (theApp.GetSelectStationMode() == eStation_1){
		CString str = mStationString[theApp.GetSelectStation()];
		m_wndCaption.m_clrBarBackground = mStationColor[theApp.GetSelectStation()];
		m_wndCaption.SetText(str, CMFCCaptionBar::ALIGN_CENTER);
	}
	else if (theApp.GetSelectStationMode() == eStation_5){
		m_wndCaption.SetText(mStationModeString[eStation_5], CMFCCaptionBar::ALIGN_CENTER);
	}
	else{
		m_wndCaption.m_clrBarBackground = ::GetSysColor(COLOR_BTNFACE);
		m_wndCaption.SetText(_T(""), CMFCCaptionBar::ALIGN_CENTER);
	}

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

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE);

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

	// ユーザー定義のツール バーの操作を許可します:
	//InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("ステータス バーの作成に失敗しました。\n");
	//	return -1;      // 作成できない場合
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	//m_wndStatusBar.SetPaneTextColor(m_wndStatusBar.CommandToIndex(ID_INDICATOR_TIME), RGB(0, 0, 0));

	// TODO: ツール バーおよびメニュー バーをドッキング可能にしない場合は、この 5 つの行を削除します
	//m_wndMenuBar.EnableDocking(CBRS_ALIGN_TOP);
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_TOP);
	DockPane(&m_wndMenuBar);
	//DockPane(&m_wndToolBar);


	// Visual Studio 2005 スタイルのドッキング ウィンドウ動作を有効にします
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 スタイルのドッキング ウィンドウの自動非表示動作を有効にします
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	// 固定値に基づいてビジュアル マネージャーと visual スタイルを設定します
	OnApplicationLook(theApp.m_nAppLook);

	// 拡張ウィンドウ管理ダイアログ ボックスを有効にします
	//EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// ツール バーとドッキング ウィンドウ メニューの配置変更を有効にします
	//EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// ツール バーのクイック (Alt キーを押しながらドラッグ) カスタマイズを有効にします
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

	// タイマー起動
	SetTimer(0, 1000, NULL);
	// タイマー起動
	SetTimer(100, 1000, NULL);

#ifdef _DEBUG
//	SetTimer(9, 1000, NULL);
#endif

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// ■プロセス間通信
	// デフォルトでアプリ起動時に運用タイムラインを起動する
	CMainFrame::SetPipeMessageWnd(this);
	if (theApp.GetSelectStationMode() == eStation_1){
		OnViewOpeTimeLine();
	}

	return CMDIFrameWndEx::OnCreateClient(lpcs, pContext);
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


//-----------------------------------------------------------------
// 各画面呼び出し
//-----------------------------------------------------------------

// 計画立案画面呼び出し
void CMainFrame::OnMsPlanning()
{
	CString str1, str2;
	LPARAM val3;
	UINT val4;
	CTime val5;
	theApp.GetTimeLinePlan(str1, str2, val3, val4, val5);
	UINT bNew = (val4 & 0x01) ? 1 : 0;

	COperationPlan* pDlg = (COperationPlan*)theApp.GetWindowHandle(IDD_DLG_OPERATIONPLAN);
	if (pDlg != NULL){
		pDlg->ClearForceClosed();
		if (str1.IsEmpty() == false && str2.IsEmpty() == false){
			pDlg->SetSatelliteName(str1);
			pDlg->SetPassId(str2);
			pDlg->SetGetForm((DWORD)val3);
			pDlg->SetType(bNew);
		}
		pDlg->UpdateOperationPlanDlg();
		pDlg->ShowWindow(SW_SHOW);
		return;
	}

	pDlg = new COperationPlan();
	if (pDlg != NULL)
	{
		pDlg->ClearForceClosed();
		if (str1.IsEmpty() == false && str2.IsEmpty() == false){
			pDlg->SetSatelliteName(str1);
			pDlg->SetPassId(str2);
			pDlg->SetGetForm((DWORD)val3);
			pDlg->SetType(bNew);
		}
		if (pDlg->Create(IDD_DLG_OPERATIONPLAN, this))
		{
			pDlg->UpdateOperationPlanDlg();
			pDlg->ShowWindow(SW_SHOW);
			theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_OPERATIONPLAN);
		}
	}
}


// 軌道データ生成画面呼び出し
void CMainFrame::OnMsOrbitdata()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


// セッション画面(4局)呼び出し
void CMainFrame::OnMsSessionstate4()
{
//	TCHAR szMsg[256];

	CSessionStatus* pDlg = (CSessionStatus*)theApp.GetWindowHandle(IDD_DLG_SESSIONSTATUS);
	if (pDlg != NULL)
	{
		pDlg->ShowWindow(SW_SHOW);
		return;
	}

	pDlg = new CSessionStatus();
	if (pDlg != NULL)
	{
		for (int i = 0; i < eStation_MAX; i++)
		{
			pDlg->AddStation(i, mStationString[i]);
		}
		// ★表示インデックスを内之浦20m, 内之浦34m, 臼田64m, 臼田54mの順にする
		pDlg->SetStation(eStation_Uchinoura20);

		if (pDlg->Create(IDD_DLG_SESSIONSTATUS, this))
		{
			pDlg->ShowWindow(SW_SHOW);
			theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_SESSIONSTATUS);
		}
	}
}

void CMainFrame::OnUpdateMiSessionSet4(CCmdUI *pCmdUI)
{
	CWnd* pWnd = theApp.GetWindowHandle(IDD_DLG_SESSIONSTATUS);
	if (pWnd == NULL)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
	}
}

// 時刻オフセット設定画面呼び出し
void CMainFrame::OnMsTimeOffsetSetting()
{
	COperationPlanTimeOffsetSet dlg;
	dlg.SetCaller(COperationPlanTimeOffsetSet::TYPE_TIMELINE);
	dlg.SetStationName(CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower());
	if (dlg.DoModal() == IDCLOSE)
	{
		return;
	}
}

// 運用開始時刻変更画面呼び出し
void CMainFrame::OnMsStartTimeChange()
{
	COperationStartTimeChg dlg;
	if (dlg.DoModal() == IDCLOSE)
	{
		return;
	}
}

// 運用終了時刻変更画面呼び出し
void CMainFrame::OnMsEndTimeChange()
{
	COperationEndTimeChg dlg;
	if (dlg.DoModal() == IDCLOSE)
	{
		return;
	}
}

// 強制運用停止画面呼び出し
void CMainFrame::OnMsForcedOperationStop()
{
	TCHAR szTitle[256];
	TCHAR szMsg[256];

	memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	// 監視データから衛星名を取得する
	const CString mEISEI_NAME = _T("CTRL.SAT_NAME"); //衛星名
	CSatelliteData& sd = theApp.GetSatelliteData();
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), mEISEI_NAME);
	CString strCurrentSatName = _T("");
	if (obs_adr == NULL){
		// 衛星名が取得できない
#ifdef _DEBUG
//		strCurrentSatName = _T("VLBI");
#endif
	}

	if (strCurrentSatName.CompareNoCase(_T("VLBI")) == 0)
	{
		// VLBI強制停止
		if (!LoadString(GetModuleHandle(NULL), IDS_FORCED_OP_STOP_TITLE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
			return;
		if (!LoadString(GetModuleHandle(NULL), IDS_FORCED_OP_VLBI_STOP_MSG, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
			return;
	}
	else if (strCurrentSatName.CompareNoCase(_T("MAINT")) == 0)
	{
		// 局占有強制停止
		if (!LoadString(GetModuleHandle(NULL), IDS_FORCED_OP_STOP_TITLE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
			return;
		if (!LoadString(GetModuleHandle(NULL), IDS_FORCED_OP_OCCU_STOP_MSG, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
			return;
	}
	else
	{
		// 運用計画強制停止
		if (!LoadString(GetModuleHandle(NULL), IDS_FORCED_OP_STOP_TITLE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
			return;
		if (!LoadString(GetModuleHandle(NULL), IDS_FORCED_OP_STOP_MSG, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
			return;
	}

	if (MessageBoxHooked(this->m_hWnd, szMsg, szTitle, MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) == IDOK)
	{
		// 計画変更要求
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = SEND_NOTICE_SH;
		CString arg;

		int nStation = theApp.GetSelectStation();
		arg.Format(_T("%s %d %d"),
			(LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(),	// 局
			INFO_ID_REQ_PLN_MO,												// 計画変更要求
			3);																// 強制運用停止

		stParam.strRemShellParam.Format(arg);

		int err = CSshShell::ExecShell(stParam, FALSE);		// シェル待ちしない
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnMenuResend"), _T("debug"), strDebug, nLogEx::debug);
		}

		if (err != 0) // 異常終了
		{
			// T.B.D
		}
	}
}

#define _GREAT_RDNSCHKPLAN_DEL	1

// 校正診断強制終了
void CMainFrame::OnMsCalibStpDel()
{
	TCHAR szTitle[256];
	TCHAR szMsg[256];

	memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	const CString COND_AGC_S = _T("CTRL.AGC_S_COND");
	const CString COND_AGC_X = _T("CTRL.AGC_X_COND");
	const CString COND_AGC_KA = _T("CTRL.AGC_Ka_COND");
	const CString COND_RNG = _T("CTRL.RNG_COND");
	const CString COND_TLM_S = _T("CTRL.TLMCHK_S_COND");
	const CString COND_TLM_X = _T("CTRL.TLMCHK_X_COND");
	const CString COND_TLM_KA = _T("CTRL.TLMCHK_Ka_COND");
	const CString COND_CMD = _T("CTRL.CMDCHK_COND");

	if (!LoadString(GetModuleHandle(NULL), IDS_FORCED_OP_STOP_TITLE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
		return;
	if (!LoadString(GetModuleHandle(NULL), IDS_MC_CALIBDIAGNS_STOP, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;

#ifdef  _GREAT_RDNSCHKPLAN_DEL
	if (m_bRdnsChkPlanForceStop)
	{
		// すでに校正診断強制終了中は何もしない
		MessageBox(_T("校正診断強制終了中です。\n"), szTitle, MB_OK | MB_ICONEXCLAMATION);
		return;
	}
#endif /* _GREAT_RDNSCHKPLAN_DEL */

	if (MessageBoxHooked(this->m_hWnd, szMsg, szTitle, MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) != IDOK)
	{
		// キャンセル時
		return;
	}

	//----------------------------------------------------
	// 校正診断 - 実行中診断キャンセル
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT nStation = theApp.GetSelectStation();
	CString cond = _T("");

	CString strSatName = _T("");
#ifdef _DEBUG
	//		strSatName = _T("PLANET-C");
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), _T("CTRL.SAT_NAME"));
	if (obs_adr != NULL)
	{
		strSatName = CString(obs_adr->sz_sts_name);
	}
	else
	{
		// カレント衛星がない時
		TCHAR szTitle[256];
		memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
		if (!LoadString(GetModuleHandle(NULL), IDS_FORCED_OP_STOP_TITLE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
			return;

		MessageBox(_T("カレントの衛星がないため、校正診断強制終了処理を中止します。\n"), szTitle, MB_OK | MB_ICONERROR);
		return;
	}

	vector<CString> cancellist;
	CString ctrl = _T("");;
	if (nStation == eStation_Usuda64)
	{
		// AGC S帯
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_AGC_S);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel64AgcSTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel64AgcSTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel64AgcSTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}
		// AGC X帯
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_AGC_X);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel64AgcXTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel64AgcXTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel64AgcXTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}

		// RNG
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_RNG);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel64RngTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel64RngTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel64RngTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}

		// TLM S帯
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_TLM_S);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel64TlmSTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel64TlmSTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel64TlmSTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}

		// TLM X帯
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_TLM_X);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel64TlmXTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel64TlmXTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel64TlmXTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}

		// CMD
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_CMD);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel64CmdTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel64CmdTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel64CmdTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}
	}
	if (nStation == eStation_Usuda642)
	{
		// AGC X帯
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_AGC_X);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel54AgcXTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel54AgcXTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel54AgcXTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}
		// AGC Ka帯
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_AGC_KA);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel54AgcKaTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel54AgcKaTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel54AgcKaTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}

		// RNG
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_RNG);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel54RngTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel54RngTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel54RngTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}

		// TLM X帯
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_TLM_X);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel54TlmXTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel54TlmXTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel54TlmXTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}

		// TLM Ka帯
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_TLM_KA);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel54TlmKaTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel54TlmKaTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel54TlmKaTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}

		// CMD
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_CMD);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel54CmdTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel54CmdTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel54CmdTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}
	}
	else if (nStation == eStation_Uchinoura34)
	{
		// AGC S帯
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_AGC_S);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel34AgcSTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel34AgcSTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel34AgcSTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}
		// AGC X帯
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_AGC_X);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel34AgcXTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel34AgcXTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel34AgcXTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}

		// RNG
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_RNG);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel34RngTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel34RngTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel34RngTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}

		// TLM S帯
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_TLM_S);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel34TlmSTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel34TlmSTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel34TlmSTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}

		// TLM X帯
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_TLM_X);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel34TlmXTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel34TlmXTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel34TlmXTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}

		// CMD
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_CMD);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel34CmdTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel34CmdTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel34CmdTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}
	}
	else if (nStation == eStation_Uchinoura20)
	{
		// AGC S帯
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_AGC_S);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel20AgcSTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel20AgcSTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel20AgcSTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}
		// AGC X帯
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_AGC_X);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel20AgcXTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel20AgcXTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel20AgcXTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}

		// RNG
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_RNG);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel20RngTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel20RngTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel20RngTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}

		// TLM S帯
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_TLM_S);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel20TlmSTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel20TlmSTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel20TlmSTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}

		// TLM X帯
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_TLM_X);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel20TlmXTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel20TlmXTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel20TlmXTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}

		// CMD
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_CMD);
		if (obs_adr != NULL)
		{
			cond = CString(obs_adr->sz_sts_name);
			if (cond.CompareNoCase(_T("STOP")) != 0)
			{
				for (int i = 0; m_RdnsCancel20CmdTbl[i].param != _T(""); ++i)
				{
					if (cond.CompareNoCase(m_RdnsCancel20CmdTbl[i].param) == 0)
					{
						CString tmp = m_RdnsCancel20CmdTbl[i].ctrl;
						ctrl.Format(tmp, strSatName);
						cancellist.push_back(ctrl);
					}
				}
			}
		}
	}

	// キャンセル制御送信
	int size = (int)cancellist.size();
	for (int i = 0; i < size; ++i)
	{
		theApp.SendControlNoUI(cancellist[i], _T(""), _T(""), _T(""));
		Sleep(10);
	}

#ifndef  _GREAT_RDNSCHKPLAN_DEL
	// 状態が全てSTOPになるまで待機
	BOOL bAGCsSTOP = FALSE;
	BOOL bAGCxSTOP = FALSE;
	BOOL bAGCkaSTOP = FALSE;
	BOOL bRNGSTOP = FALSE;
	BOOL bTLMsSTOP = FALSE;
	BOOL bTLMxSTOP = FALSE;
	BOOL bTLMkaSTOP = FALSE;
	BOOL bCMDSTOP = FALSE;
	BOOL bAllStop = FALSE;

	if (nStation != eStation_Usuda642)
	{
		// 54m以外
		while (1)
		{
			// AGC S帯
			if (bAGCsSTOP == FALSE)
			{
				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_AGC_S);
				if (obs_adr != NULL)
				{
					cond = CString(obs_adr->sz_sts_name);
					if (cond.CompareNoCase(_T("STOP")) == 0)
					{
						bAGCsSTOP = TRUE;
					}
				}
			}
			// AGC X帯
			if (bAGCxSTOP == FALSE)
			{
				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_AGC_X);
				if (obs_adr != NULL)
				{
					cond = CString(obs_adr->sz_sts_name);
					if (cond.CompareNoCase(_T("STOP")) == 0)
					{
						bAGCxSTOP = TRUE;
					}
				}
			}
			// RNG
			if (bRNGSTOP == FALSE)
			{
				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_RNG);
				if (obs_adr != NULL)
				{
					cond = CString(obs_adr->sz_sts_name);
					if (cond.CompareNoCase(_T("STOP")) == 0)
					{
						bRNGSTOP = TRUE;
					}
				}
			}
			// TLM S帯
			if (bTLMsSTOP == FALSE)
			{
				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_TLM_S);
				if (obs_adr != NULL)
				{
					cond = CString(obs_adr->sz_sts_name);
					if (cond.CompareNoCase(_T("STOP")) == 0)
					{
						bTLMsSTOP = TRUE;
					}
				}
			}
			// TLM X帯
			if (bTLMxSTOP == FALSE)
			{
				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_TLM_X);
				if (obs_adr != NULL)
				{
					cond = CString(obs_adr->sz_sts_name);
					if (cond.CompareNoCase(_T("STOP")) == 0)
					{
						bTLMxSTOP = TRUE;
					}
				}
			}
			// CMD
			if (bCMDSTOP == FALSE)
			{
				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_CMD);
				if (obs_adr != NULL)
				{
					cond = CString(obs_adr->sz_sts_name);
					if (cond.CompareNoCase(_T("STOP")) == 0)
					{
						bCMDSTOP = TRUE;
					}
				}
			}

			if (bAGCsSTOP == TRUE && bAGCxSTOP == TRUE && bRNGSTOP == TRUE && bTLMsSTOP == TRUE && bTLMxSTOP == TRUE && bCMDSTOP == TRUE)
			{
				bAllStop = TRUE;
			}

			if (bAllStop == TRUE)
			{
				break;
			}
			Sleep(100);
		}
	}
	else
	{
		// 54m
		while (1)
		{
			// AGC X帯
			if (bAGCxSTOP == FALSE)
			{
				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_AGC_X);
				if (obs_adr != NULL)
				{
					cond = CString(obs_adr->sz_sts_name);
					if (cond.CompareNoCase(_T("STOP")) == 0)
					{
						bAGCxSTOP = TRUE;
					}
				}
			}
			// AGC Ka帯
			if (bAGCkaSTOP == FALSE)
			{
				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_AGC_KA);
				if (obs_adr != NULL)
				{
					cond = CString(obs_adr->sz_sts_name);
					if (cond.CompareNoCase(_T("STOP")) == 0)
					{
						bAGCkaSTOP = TRUE;
					}
				}
			}
			// RNG
			if (bRNGSTOP == FALSE)
			{
				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_RNG);
				if (obs_adr != NULL)
				{
					cond = CString(obs_adr->sz_sts_name);
					if (cond.CompareNoCase(_T("STOP")) == 0)
					{
						bRNGSTOP = TRUE;
					}
				}
			}
			// TLM X帯
			if (bTLMxSTOP == FALSE)
			{
				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_TLM_X);
				if (obs_adr != NULL)
				{
					cond = CString(obs_adr->sz_sts_name);
					if (cond.CompareNoCase(_T("STOP")) == 0)
					{
						bTLMxSTOP = TRUE;
					}
				}
			}
			// TLM Ka帯
			if (bTLMkaSTOP == FALSE)
			{
				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_TLM_KA);
				if (obs_adr != NULL)
				{
					cond = CString(obs_adr->sz_sts_name);
					if (cond.CompareNoCase(_T("STOP")) == 0)
					{
						bTLMkaSTOP = TRUE;
					}
				}
			}
			// CMD
			if (bCMDSTOP == FALSE)
			{
				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_CMD);
				if (obs_adr != NULL)
				{
					cond = CString(obs_adr->sz_sts_name);
					if (cond.CompareNoCase(_T("STOP")) == 0)
					{
						bCMDSTOP = TRUE;
					}
				}
			}

			if (bAGCxSTOP == TRUE && bAGCkaSTOP == TRUE && bRNGSTOP == TRUE && bTLMxSTOP == TRUE && bTLMkaSTOP == TRUE && bCMDSTOP == TRUE)
			{
				bAllStop = TRUE;
			}

			if (bAllStop == TRUE)
			{
				break;
			}
			Sleep(100);
		}

	}

	//----------------------------------------------------
	// 校正診断 - 計画強制停止通知

//#if 1
	// 計画削除要求
	CString sat, id, prd;
	LPARAM getform;
	UINT val4;
	CTime val5;
	theApp.GetTimeLinePlan(sat, id, getform, val4, val5);

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = REGIST_PLAN_SH;
	CString arg;
	CString strGetFrom;
	if (getform == 1)
	{
		strGetFrom = _T("enp");
	}
	else
	{
		strGetFrom = _T("sttn");
	}

	//	arg.Format(_T("%s %s %s"), mStationSimpleString[theApp.GetSelectStation()], m_strSatellite, pid);
	// 削除
	arg.Format(_T("%s del %s %s %s.pln"), (LPCTSTR)CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower(), (LPCTSTR)sat.MakeLower(), (LPCTSTR)strGetFrom, (LPCTSTR)id);

	stParam.strRemShellParam.Format(arg);

	int err = CSshShell::ExecShell(stParam);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CTimeLine"), _T("WindowProc"), _T("debug"), strDebug, nLogEx::debug);
	}
//#else
//	// 計画変更要求
//	CSshShell::structSshParam stParam;
//	stParam.strBatchPath = theApp.GetSshBatchPath();
//	stParam.strUserName = theApp.GetSshUserName();
//	stParam.strServerName = theApp.GetSshServerName();
//	stParam.strKeyFilePath = theApp.GetSshKeyPath();
//	stParam.strRemShellName = SEND_NOTICE_SH;
//	CString arg;
//
//	arg.Format(_T("%s %d %d"),
//		(LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(),	// 局
//		INFO_ID_REQ_PLN_MO,												// 計画変更要求
//		3);																// 強制運用停止
//
//	stParam.strRemShellParam.Format(arg);
//
//	int err = CSshShell::ExecShell(stParam, FALSE);		// シェル待ちしない
//	{
//		CString strDebug;
//		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
//		CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnMenuResend"), _T("debug"), strDebug, nLogEx::debug);
//	}
//
//	if (err != 0) // 異常終了
//	{
//		// T.B.D
//	}
//#endif

#else

	// タイムアウト時刻計算
	CTime ctime;
	theApp.GetCurrentUtcTime(ctime);
	m_tRdnsPlanStopTimeout = ctime.GetTime() + (RDNSCHKPLAN_FORCESTOP_TIMEOUT);

	// 強制終了タイマー開始
	m_bRdnsChkPlanForceStop = true;
	SetTimer(200, 800, NULL);

#endif /* _GREAT_RDNSCHKPLAN_DEL */
}


//-----------------------------------------------------------------


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
	switch (theApp.GetSelectStationMode()){
	case	eStation_1:
		OnTileHorz_1();
		break;
	case	eStation_5:
		OnTileHorz_5();
		break;
	default:
		return;
	}
}

/*============================================================================*/
/*! アプリケーション

-# １局用のウィンドウの並べて表示を行う

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnTileHorz_1()
{
	CView* pview;
	CWnd* pwnd;
	CRect rc, rect[eViewType1_Max], maxRect, workArea;

	// +---------------------------++---------------------------+
	// + 衛星捕捉グラフ            ++ 設備状況                  +
	// +                           ++                           +
	// +                           ++                           +
	// +                           ++                           +
	// +---------------------------++---------------------------+
	// + 運用履歴                  ++ 制御履歴                  +
	// +                           ++                           +
	// +                           ++                           +
	// +---------------------------++---------------------------+

	// MDIの作業領域を取得する
	//GetDlgItem(AFX_IDW_PANE_FIRST)->GetWindowRect(rc);
	GetClientRect(rc);
	CWnd* pWnd = CWnd::FromHandle(m_hWndMDIClient);
	pWnd->GetClientRect(rc);
	workArea = rc;

	//MDITile(MDITILE_HORIZONTAL);
	maxRect = workArea;

	UINT	width, height;
	// 衛星捕捉グラフ
	width = maxRect.Width() / 8 * 3 +(27);
	height = maxRect.Height() / 5 * 4 -(30);
	pview = theApp.GetView(eViewType1_Trend);
	if (pview){
		pwnd = pview->GetParent();
		rect[eViewType1_Trend].left = 0;
		rect[eViewType1_Trend].top = 0;
		rect[eViewType1_Trend].right = rect[eViewType1_Trend].left + width;
		rect[eViewType1_Trend].bottom = rect[eViewType1_Trend].top + height;
		pwnd->SetWindowPos(&wndTop, rect[eViewType1_Trend].left, rect[eViewType1_Trend].top, rect[eViewType1_Trend].Width(), rect[eViewType1_Trend].Height(), SWP_NOZORDER);
		UINT heightMin = height;
		switch (((CTrendGraphView*)pview)->GetWndTypeSet())
		{
		case eTrendWndSetType_AGC:
			heightMin = height * 4 / (4 + 3 + 3);
			break;
		case eTrendWndSetType_RNG_RR:
			heightMin = height * (4 + 3) / (4 + 3 + 3);
			break;
		case eTrendWndSetType_AGC_RNG_EL:
		default:
			break;
		}
		((CChildFrame*)pwnd)->SetFrameMaxSize(width, heightMin);
		//pwnd->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
	}
	// 運用詳細
	width = maxRect.Width() / 8 * 5 -(27);
	height = maxRect.Height() / 5 * 4 - (30);
	pview = theApp.GetView(eViewType1_EquipmentStatus);
	if (pview){
		pwnd = pview->GetParent();
		rect[eViewType1_EquipmentStatus].left = rect[eViewType1_Trend].right;
		rect[eViewType1_EquipmentStatus].top = 0;
		rect[eViewType1_EquipmentStatus].right = maxRect.right;// rect[eViewType1_EquipmentStatus].left + width;
		rect[eViewType1_EquipmentStatus].bottom = rect[eViewType1_EquipmentStatus].top + height;
		pwnd->SetWindowPos(&wndTop, rect[eViewType1_EquipmentStatus].left, rect[eViewType1_EquipmentStatus].top, rect[eViewType1_EquipmentStatus].Width(), rect[eViewType1_EquipmentStatus].Height(), SWP_NOZORDER);
		((CChildFrame*)pwnd)->SetFrameMaxSize(rect[eViewType1_EquipmentStatus].Width(), rect[eViewType1_EquipmentStatus].Height());
	}

	int sheight = GetSystemMetrics(SM_CYHSCROLL);

	// 運用履歴
	width = maxRect.Width() / 2 * 1;
	height = maxRect.Height() / 5 * 1 + (30);
	pview = theApp.GetView(eViewType1_OperationHist);
	if (pview){
		pwnd = pview->GetParent();
		rect[eViewType1_OperationHist] = maxRect;
		rect[eViewType1_OperationHist].left = 0;
		rect[eViewType1_OperationHist].top = maxRect.Height() - height;
		rect[eViewType1_OperationHist].right = rect[eViewType1_OperationHist].left + width;
		rect[eViewType1_OperationHist].bottom = maxRect.bottom;
		pwnd->SetWindowPos(&wndTop, rect[eViewType1_OperationHist].left, rect[eViewType1_OperationHist].top, rect[eViewType1_OperationHist].Width(), rect[eViewType1_OperationHist].Height(), SWP_NOZORDER);
	}
	// 制御履歴
	width = maxRect.Width() / 2 * 1;
	height = maxRect.Height() / 5 * 1 + (30);
	pview = theApp.GetView(eViewType1_ControlHist);
	if (pview){
		pwnd = pview->GetParent();
		rect[eViewType1_ControlHist] = maxRect;
		rect[eViewType1_ControlHist].left = rect[eViewType1_OperationHist].right;
		rect[eViewType1_ControlHist].top = maxRect.Height() - height;
		rect[eViewType1_ControlHist].right = maxRect.right;// rect[eViewType1_ControlHist].left + width;
		rect[eViewType1_ControlHist].bottom = maxRect.bottom;
		pwnd->SetWindowPos(&wndTop, rect[eViewType1_ControlHist].left, rect[eViewType1_ControlHist].top, rect[eViewType1_ControlHist].Width(), rect[eViewType1_ControlHist].Height(), SWP_NOZORDER);
	}
}

/*============================================================================*/
/*! アプリケーション

-# ５局用のウィンドウの並べて表示を行う

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnTileHorz_5()
{
	CView* pview;
	CWnd* pwnd;
	CRect rc, rect[eViewType5_Max], maxRect, workArea;

	// +------------------------++-----------------++-----------++----------+
	// + 設備・運用 ++ 設備・運用 ++ 設備・運用 ++ 設備・運用 ++ 設備・運用 + 
	// +            ++            ++            ++            ++            +
	// +            ++            ++            ++            ++            +
	// +            ++            ++            ++            ++            +
	// +--------------------------------++----------------------------------+
	// + 運用履歴                       ++ 制御履歴                         +
	// +                                ++                                  +
	// +                                ++                                  +
	// +--------------------------------++----------------------------------+

	// MDIの作業領域を取得する
	//GetDlgItem(AFX_IDW_PANE_FIRST)->GetWindowRect(rc);
	GetClientRect(rc);
	workArea = rc;

	//MDITile(MDITILE_HORIZONTAL);
	maxRect = workArea;

	UINT	width, height;
	UINT	max = theApp.GetStationMax();

	int offset = 57;

	// 設備運用状態
	width = maxRect.Width() / max * 1;
	height = maxRect.Height() / 5 * 4 - (offset);
	int pos = 0;
	UINT count = 0;

	UINT nInternalIdx, nViewIdx;
	for (int i = eViewType5_EqOperation1; i < eViewType5_OperationHist; i++){
		if (count >= max)
			break;

		// ★表示インデックスを内之浦20m, 内之浦34m, 臼田64m, 臼田54mの順にする
		nInternalIdx = i - eViewType5_EqOperation1;
		nViewIdx = mStationViewOrder[nInternalIdx] + eViewType5_EqOperation1;

		pview = theApp.GetView(nViewIdx);
		if (pview){
			pwnd = pview->GetParent();
			rect[nViewIdx].left = pos;
			rect[nViewIdx].top = 0;
			rect[nViewIdx].right = rect[nViewIdx].left + width;
			rect[nViewIdx].bottom = rect[nViewIdx].top + height;
			pwnd->SetWindowPos(&wndTop, rect[nViewIdx].left, rect[nViewIdx].top, rect[nViewIdx].Width(), rect[nViewIdx].Height(), SWP_NOZORDER);
			((CChildFrame*)pwnd)->SetFrameMaxSize(rect[nViewIdx].Width(), rect[nViewIdx].Height());
			//pwnd->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
		}
		pos += width;
		count++;
	}

	// 運用履歴
	width = maxRect.Width() / 2 * 1;
	height = maxRect.Height() / 5 * 1 + (offset);
	pview = theApp.GetView(eViewType5_OperationHist);
	if (pview){
		pwnd = pview->GetParent();
		rect[eViewType5_OperationHist] = maxRect;
		rect[eViewType5_OperationHist].left = 0;
		rect[eViewType5_OperationHist].top = maxRect.Height() - height;
		rect[eViewType5_OperationHist].right = rect[eViewType5_OperationHist].left + width;
		rect[eViewType5_OperationHist].bottom = rect[eViewType5_OperationHist].top + height - 60;
		pwnd->SetWindowPos(&wndTop, rect[eViewType5_OperationHist].left, rect[eViewType5_OperationHist].top, rect[eViewType5_OperationHist].Width(), rect[eViewType5_OperationHist].Height(), SWP_NOZORDER);
	}
	// 制御履歴
	width = maxRect.Width() / 2 * 1;
	height = maxRect.Height() / 5 * 1 + (offset);
	pview = theApp.GetView(eViewType5_ControlHist);
	if (pview){
		pwnd = pview->GetParent();
		rect[eViewType5_ControlHist] = maxRect;
		rect[eViewType5_ControlHist].left = rect[eViewType5_OperationHist].right;
		rect[eViewType5_ControlHist].top = maxRect.Height() - height;
		rect[eViewType5_ControlHist].right = rect[eViewType5_ControlHist].left + width;
		rect[eViewType5_ControlHist].bottom = rect[eViewType5_ControlHist].top + height - 60;
		pwnd->SetWindowPos(&wndTop, rect[eViewType5_ControlHist].left, rect[eViewType5_ControlHist].top, rect[eViewType5_ControlHist].Width(), rect[eViewType5_ControlHist].Height(), SWP_NOZORDER);
	}
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
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
		//=====================================================================
		//	コントロール
		//=====================================================================
	case eCtrl_AntEmg:
		// アンテナ強制停止
		Emergency(eANTEMG);
		break;
	case eCtrl_SndStop:
		// 送信機停止
		Emergency(eSENDSTOP);
		break;
	case eCtrl_Sound:
		// エラー時のサウンドOFF
		if (lParam != 9){
			sendPipe(ePIPETYPE_TIMELINE, eALARMSTOP);
		}
		StopStatusSound();
		break;
	case eCtrl_Mute:
		// エラー時のサウンドをミュート
// CHG 2021.11.02 >>>
		//if (lParam != 9){
		//	if (lParam == 0)
		//		sendPipe(ePIPETYPE_TIMELINE, eALARMMUTEOFF);
		//	if (lParam == 1)
		//		sendPipe(ePIPETYPE_TIMELINE, eALARMMUTEON);
		//}
		//MuteStatusSound();
		if (lParam != 9){
			::SendMessage(theApp.GetTimeLineHandle(), eTIMELINE_SET_MUTE, 0, (LPARAM)lParam);
		}
		else{
			if (lParam != 9){
				if (lParam == 0)
					sendPipe(ePIPETYPE_TIMELINE, eALARMMUTEOFF);
				if (lParam == 1)
					sendPipe(ePIPETYPE_TIMELINE, eALARMMUTEON);
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
		CMFCCaptionBarEx& cap = GetWndCaption();
		CMFCButtonEx& btn = cap.GetMuteBtnCtrl();
		btn.ExecuteClicked(true, (int)lParam);
		break;
	}
// ADD 2021.12.02 <<<
	case eCtrl_SnapShot:
		// 画面印刷
		OnDirectPrint();
		break;
	case eCtrl_Help:
		{
			PostMessage(WM_COMMAND, ID_MH_MANUAL, 0);
		}
		break;
		//=====================================================================
		//	スレッド
		//=====================================================================
	case eMessage_ThreadNormal:
		if (theApp.GetSelectStationMode() == eStation_1){
#ifndef _ENGVALCNV_TREND
			if (theApp.GetDataCollection().AddCollection(message, wParam, lParam) == true)
#endif
			{
				if ((theApp.GetView(eViewType1_Trend))->GetSafeHwnd())				(theApp.GetView(eViewType1_Trend))->PostMessage(message, wParam, lParam);
				if ((theApp.GetView(eViewType1_EquipmentStatus))->GetSafeHwnd())	(theApp.GetView(eViewType1_EquipmentStatus))->PostMessage(message, wParam, lParam);
			}
			UINT status;
			status = theApp.GetDataCollection().AddHistory(message, wParam, lParam);
			if ((theApp.GetView(eViewType1_OperationHist))->GetSafeHwnd())	(theApp.GetView(eViewType1_OperationHist))->PostMessage(message, wParam, lParam);
			if ((theApp.GetView(eViewType1_ControlHist))->GetSafeHwnd())	(theApp.GetView(eViewType1_ControlHist))->PostMessage(message, wParam, lParam);
			if ((theApp.GetView(eViewType1_EquipmentHist))->GetSafeHwnd())	(theApp.GetView(eViewType1_EquipmentHist))->PostMessage(message, wParam, lParam);
			if ((theApp.GetView(eViewType1_EventHist))->GetSafeHwnd())		(theApp.GetView(eViewType1_EventHist))->PostMessage(message, wParam, lParam);
			PlayStatusSound(status);
			SendPipeHistory();
		}
		if (theApp.GetSelectStationMode() == eStation_5){
#ifndef _ENGVALCNV_TREND
			if (theApp.GetDataCollection().AddCollection(message, wParam, lParam) == true)
#endif
			{
				if ((theApp.GetView(eViewType1_Trend))->GetSafeHwnd())			(theApp.GetView(eViewType1_Trend))->PostMessage(message, wParam, lParam);
				if ((theApp.GetView(eViewType5_EqOperation1))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation1))->PostMessage(message, wParam, lParam);
				if ((theApp.GetView(eViewType5_EqOperation2))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation2))->PostMessage(message, wParam, lParam);
				if ((theApp.GetView(eViewType5_EqOperation3))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation3))->PostMessage(message, wParam, lParam);
				if ((theApp.GetView(eViewType5_EqOperation4))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation4))->PostMessage(message, wParam, lParam);
				if ((theApp.GetView(eViewType5_EqOperation5))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation5))->PostMessage(message, wParam, lParam);
			}
			UINT status;
			status = theApp.GetDataCollection().AddHistory(message, wParam, lParam);
			if ((theApp.GetView(eViewType5_OperationHist))->GetSafeHwnd())	(theApp.GetView(eViewType5_OperationHist))->PostMessage(message, wParam, lParam);
			if ((theApp.GetView(eViewType5_ControlHist))->GetSafeHwnd())	(theApp.GetView(eViewType5_ControlHist))->PostMessage(message, wParam, lParam);
			if ((theApp.GetView(eViewType5_EquipmentHist))->GetSafeHwnd())	(theApp.GetView(eViewType5_EquipmentHist))->PostMessage(message, wParam, lParam);
			if ((theApp.GetView(eViewType5_EventHist))->GetSafeHwnd())		(theApp.GetView(eViewType5_EventHist))->PostMessage(message, wParam, lParam);
			PlayStatusSound(status);
		}
		break;

	case eMessage_ThreadError:
	case eMessage_ThreadFail:
	case eMessage_ThreadException:
		// ここでエラー時の対応を行う
		theApp.GetDataCollection().StopCollection();
		break;
	case eMessage_ThreadEvent:
		if (wParam == 0)
			PlayEventSound((UINT)lParam);
		else
			StopEventSound();
		break;

	case eMessage_DirectPrint:
			DirectPrint(this);
			break;
	case eMessage_TimeLineHandle:
		if (wParam == 1){
			// タイムラインが起動された
			theApp.SetTimeLineHandle((HWND)lParam);
			// 監視データを通知するためのスレッドを起動する
			CreateTimelineThread();
		}
		break;
	case eMessage_TimeLineFinish:
		if (wParam == 1){
			// タイムライン側から終了通知を受け取った
			theApp.SetTimeLineHandle(NULL);
		}
		break;
	case eMessage_TimeLinePlan:
		if (wParam == 1){
			PostMessage(WM_COMMAND, ID_MS_PLANNING, 0);
		}
		break;
	case eMessage_TimeLineForcast:
		{
			theApp.m_pMainWnd->SendMessage(eMessage_OpenForecastListDlg, (WPARAM)wParam, (LPARAM)lParam);
		}
		//if (wParam == 1){
		//	PostMessage(WM_COMMAND, ID_MP_FORECASTLIST, 0);
		//}
		break;
	case eMessage_TimeLineEmg:
	case eMessage_TimeLineStop:
		break;
	case ID_PIPE_RECEIVE:
	case ID_PIPE_SEND:
	case ID_PIPE_CREATE:
	case ID_TIMELINE_MESSAGE:
	case ID_PIPE_FINISH:
		// ■プロセス間通信
		messagePipe(message, wParam, lParam);
		break;

	/**********************************************************************************************
	これ以降、StationControl.exeを呼び出して、情報を取得する
	*/
	case	eTIMELINE_GET_SATNAME:
	{
		// 衛星名取得
		CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
		CString sat = _T("");
		UINT sta = theApp.GetSelectStation();
		obs_adr = (CEngValCnv::obsname_t *)theApp.GetSatelliteData().GetDBAccessCls().GetObsname_t(sta, CTRL_P_SAT);
		if (obs_adr != NULL)
		{
			// 監視データ内にデフォルト衛星名が存在した
			sat = CString(obs_adr->sz_sts_name);
		}
		else{
			sat = _T("NONE");
		}
		mMemoryTimeLine.Push(CStringA(sat));
		break;
	} //case	eTIMELINE_GET_SATNAME:
	case	eTIMELINE_GET_XS:
	{
		// X帯、S帯送信設備使用有無情報取得
		// タイムラインでPushされた衛星名を取得する
		CString sat = _T("");
		sat.Format(_T("%s"), (LPCTSTR)CString(mMemoryTimeLine.Pop()));
		TRACE("##SM:%s\n", CStringA(sat));
		CCidSelect dlg;
		int type = dlg.GetSendType(sat);
		sat.Format(_T("%d"), type);
		mMemoryTimeLine.Push(CStringA(sat));
		break;
	} //case	eTIMELINE_GET_SATNAME:
	case	eTIMELINE_GET_PRED:
	{
		// 予報値ファイル名を取得
		CString sat = _T("");
		sat.Format(_T("%s"), (LPCTSTR)CString(mMemoryTimeLine.Pop()));
		if (sat.Left(1) == _T("-"))
			sat.Empty();
		SendMessage(eMessage_OpenForecastListDlg, (WPARAM)0, (LPARAM)&sat);
		break;
	} //case	eTIMELINE_GET_PRED:
	/*
	ここまで
	************************************************************************************************/

	case	ID_PIPE_SERVER:
		PostMessage(eMessage_ThreadNormal, 0, 0);
		break;

		//=====================================================================
		//	トレンドグラフ　変更通知
		//=====================================================================
	case eMessage_PropetyChange:
		// プロパティ変更通知
		{
			CView* pView = theApp.GetView(eViewType1_Trend);
			if (pView != NULL)
			{
				pView->PostMessage(eMessage_PropetyChange, wParam, lParam);
			}
		}
		break;
	case eMessage_InitTrendWnd:
		// トレンドグラフウィンドウ初期化通知
		{
			CView* pView = theApp.GetView(eViewType1_Trend);
			if (pView != NULL)
			{
				pView->PostMessage(eMessage_InitTrendWnd, wParam, lParam);
			}
		}
		break;
	case eMessage_OpenTrendSetDlg:
		// トレンドグラフ設定ウィンドウ呼び出し通知
		{
			CTrendGraphView* pWnd = (CTrendGraphView*)lParam;
			if (pWnd != NULL)
			{
				vector<CString> listViewName;
				pWnd->GetViewNameList(listViewName);
				UINT nPropertiesID = (UINT)wParam;
				UINT nViewID = pWnd->ExchgPropertiesID2ViewID(nPropertiesID);
				CTrendPropertiesDlg dlg(this, nViewID, listViewName, nPropertiesID, theApp.GetSelectStation(), 0, CTrendPropertiesDlg::ePropertiesType_Trend);
				dlg.DoModal();
			}
		}
		break;
	case eMessage_NewEngValData:
		// 新しい工学値変換を受けた通知
		if (theApp.GetSelectStationMode() == eStation_1){
			if ((theApp.GetView(eViewType1_Trend))->GetSafeHwnd())				(theApp.GetView(eViewType1_Trend))->PostMessage(message, wParam, lParam);
			if ((theApp.GetView(eViewType1_EquipmentStatus))->GetSafeHwnd())	(theApp.GetView(eViewType1_EquipmentStatus))->PostMessage(message, wParam, lParam);
		}
		if (theApp.GetSelectStationMode() == eStation_5){
			if ((theApp.GetView(eViewType1_Trend))->GetSafeHwnd())			(theApp.GetView(eViewType1_Trend))->PostMessage(message, wParam, lParam);
			if ((theApp.GetView(eViewType5_EqOperation1))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation1))->PostMessage(message, wParam, lParam);
			if ((theApp.GetView(eViewType5_EqOperation2))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation2))->PostMessage(message, wParam, lParam);
			if ((theApp.GetView(eViewType5_EqOperation3))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation3))->PostMessage(message, wParam, lParam);
			if ((theApp.GetView(eViewType5_EqOperation4))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation4))->PostMessage(message, wParam, lParam);
			if ((theApp.GetView(eViewType5_EqOperation5))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation5))->PostMessage(message, wParam, lParam);
		}
		break;

		//=====================================================================
		//	サマリグラフ　変更通知
		//=====================================================================
	case eMessage_OpenSummaryTrendSetDlg:
		// サマリトレンドグラフ設定ウィンドウ呼び出し通知
		{
			CSummaryProperties* pProp = (CSummaryProperties*)wParam;
			if (pProp != NULL)
			{
				vector<CString> listViewName;
				pProp->GetViewNameList(listViewName);
				UINT nViewID = pProp->GetViewID();
				UINT nProperitesID = nViewID;
				CTrendPropertiesDlg dlg(this, nViewID, listViewName, nProperitesID, pProp->GetStationID(), pProp->GetParentID(),
					CTrendPropertiesDlg::ePropertiesType_Summary, pProp->GetWndName(), (CWnd*)pProp, FALSE);
				dlg.DoModal();
			}
		}
		break;

	case eMessage_OpenSummaryTrendSetDlgChg:
		// サマリトレンドグラフ設定ウィンドウ呼び出し通知（ダイアログをスペクトラム設定からトレンド設定に変更）
		{
			CSummaryProperties* pProp = (CSummaryProperties*)wParam;
			if (pProp != NULL)
			{
				vector<CString> listViewName;
				pProp->GetViewNameList(listViewName);
				UINT nViewID = pProp->GetViewID();
				UINT nProperitesID = nViewID;
				CTrendPropertiesDlg dlg(this, nViewID, listViewName, nProperitesID, pProp->GetStationID(), pProp->GetParentID(),
					CTrendPropertiesDlg::ePropertiesType_Summary, pProp->GetWndName(), (CWnd*)pProp, TRUE);
				dlg.DoModal();
			}
		}
		break;

	case eMessage_OpenSummarySpectrumSetDlg:
		// サマリスペクトラムグラフ設定ウィンドウ呼び出し通知
		{
			CSummaryProperties* pProp = (CSummaryProperties*)wParam;
			if (pProp != NULL)
			{
				vector<CString> listViewName;
				pProp->GetViewNameList(listViewName);
				CSummarySpectrumPropertiesDlg dlg(this, pProp->GetViewID(), listViewName, pProp->GetStationID(), pProp->GetParentID(), 
					pProp->GetWndName(), (CWnd*)pProp, FALSE);
				dlg.DoModal();
			}
		}
		break;
	
	case eMessage_OpenSummarySpectrumSetDlgChg:
		// サマリスペクトラムグラフ設定ウィンドウ呼び出し通知（ダイアログをトレンド設定からスペクトラム設定に変更）
		{
			CSummaryProperties* pProp = (CSummaryProperties*)wParam;
			if (pProp != NULL)
			{
				vector<CString> listViewName;
				pProp->GetViewNameList(listViewName);
				CSummarySpectrumPropertiesDlg dlg(this, pProp->GetViewID(), listViewName, pProp->GetStationID(), pProp->GetParentID(),
					pProp->GetWndName(), (CWnd*)pProp, TRUE);
				dlg.DoModal();
			}
		}
		break;

	case eMessage_SummaryPropetyChange:
		// サマリプロパティ変更通知
		{
			CView* pView = theApp.GetView((UINT)wParam);
			if (pView != NULL)
			{
				pView->PostMessage(eMessage_SummaryPropetyChange, wParam, lParam);
			}
		}
		break;

	case eMessage_InitSummaryWnd:
		// サマリグラフ初期化通知
		{
			CView* pView = theApp.GetView((UINT)wParam);
			if (pView != NULL)
			{
				pView->PostMessage(eMessage_InitSummaryWnd, wParam, lParam);
			}
		}
		break;

	case eMessage_UpdateForecastData:
		// 予報値データ更新
		if (theApp.GetSelectStationMode() == eStation_1){
			if (theApp.GetDataCollection().AddCollection(message, wParam, lParam) == true){
				if ((theApp.GetView(eViewType1_Trend))->GetSafeHwnd())			(theApp.GetView(eViewType1_Trend))->PostMessage(message, wParam, lParam);
//				if ((theApp.GetView(eViewType1_EquipmentStatus))->GetSafeHwnd())	(theApp.GetView(eViewType1_EquipmentStatus))->PostMessage(message, wParam, lParam);
			}
		}
//		if (theApp.GetSelectStationMode() == eStation_5){
//			if (theApp.GetDataCollection().AddCollection(message, wParam, lParam) == true){
//				if ((theApp.GetView(eViewType1_Trend))->GetSafeHwnd())			(theApp.GetView(eViewType1_Trend))->PostMessage(message, wParam, lParam);
//				if ((theApp.GetView(eViewType5_EqOperation1))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation1))->PostMessage(message, wParam, lParam);
//				if ((theApp.GetView(eViewType5_EqOperation2))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation2))->PostMessage(message, wParam, lParam);
//				if ((theApp.GetView(eViewType5_EqOperation3))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation3))->PostMessage(message, wParam, lParam);
//				if ((theApp.GetView(eViewType5_EqOperation4))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation4))->PostMessage(message, wParam, lParam);
//				if ((theApp.GetView(eViewType5_EqOperation5))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation5))->PostMessage(message, wParam, lParam);
//			}
//		}
		break;

		//=====================================================================
		//	スペクトラム設定ダイアログ通知
		//=====================================================================
	case eMessage_OpenSpectrumSetDlg:
		// トレンドグラフ設定ウィンドウ呼び出し通知
		{
			CSpectrumVerticalSetDlg dlg(this, (CSpectrumView*)wParam);
			dlg.DoModal();
		}
		break;

		//=====================================================================
		//	衛星選択時の予報値一覧を開く通知
		//=====================================================================
	case eMessage_OpenForecastListDlg:
		{
			CForecastList* pDlg = (CForecastList*)theApp.GetWindowHandle(IDD_DLG_FORECASTLIST);
			if (pDlg != NULL)
			{
//				pDlg->SetSatelliteName(*(CString*)lParam);
				pDlg->SetUseSelectBtn(TRUE);
				pDlg->SelectTabAndSatellite(eForecastListType_Forecast, *(CString*)lParam);
				pDlg->ShowWindow(SW_SHOW);
#if 1	// ishi >>> 呼出元保存
				theApp.SetForecastListDlgParent((CWnd*)wParam);
#endif
				break;
			}

			pDlg = new CForecastList();
			if (pDlg != NULL)
			{
				if (pDlg->Create(IDD_DLG_FORECASTLIST, this))
				{
//					pDlg->SetSatelliteName(*(CString*)lParam);
					pDlg->SetUseSelectBtn(TRUE);
					pDlg->SelectTabAndSatellite(eForecastListType_Forecast, *(CString*)lParam);
					pDlg->ShowWindow(SW_SHOW);
#if 1	// ishi >>> 呼出元保存
					theApp.SetForecastListDlgParent((CWnd*)wParam);
#endif
					theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_FORECASTLIST);
				}
			}
		}
		break;

		//=====================================================================
		//	予報値一覧の選択予報値ファイルパス名更新通知
		//=====================================================================
	case eMessage_UpdateForecastFilePath:
		{
#if 1	// ishi >>> 呼出元へメッセージ送信
			CWnd* parent = theApp.GetForecastListDlgParent();
			if(parent != NULL)
			{
				parent->PostMessageW(eMessage_UpdateForecastFilePath, wParam, lParam);
			}
			else
			{
				// NULLの場合はパス状況から呼び出されたと判断する
				CString sat;
				sat.Format(_T("%s"), (LPCTSTR)*(CString*)lParam);
				mMemoryTimeLine.Push(CStringA(sat));
				::PostMessageW(theApp.GetTimeLineHandle(), eMessage_UpdateForecastFilePath, wParam, lParam);
			}
#else
			CSatelliteCapture* pDlg = (CSatelliteCapture*)theApp.GetWindowHandle(IDD_DLG_SATELLITECAPTURE);	//IDD_DLG_SATELLITECAPTURE54も必要
			if (pDlg != NULL)
			{
				pDlg->PostMessageW(eMessage_UpdateForecastFilePath, wParam, lParam);
			}
#endif
		}
		break;

		//=====================================================================
		//	画面を破棄する通知
		//=====================================================================
	case eMessage_WindowClose:
	{
		if (lParam != NULL)
			theApp.RemoveWindowHandle(lParam);
	}
	break;

	//=====================================================================
	//	校正診断リアルタイム実行 タイムアウト通知
	//=====================================================================
	case eMessage_RdnsChkTimeout:
	{
		MessageBox(_T("校正診断リアルタイム実行処理がタイムアウトしました。"), _T("校正診断実行結果"), MB_OK | MB_ICONWARNING);
	}
	break;

	default:
		return CMDIFrameWndEx::WindowProc(message, wParam, lParam);
	}
	return TRUE;
}

void CMainFrame::OnClose()
{
	if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("終了します。\nよろしいですか？"), _T("StationControl"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return;

	m_pTimeLineThreadsEvent->SetEvent();
	delete m_pTimeLineThreadsEvent;

	for (UINT type = 0; type < eStation_MAX; type++){
		m_pStationThreadsEvent[type]->SetEvent();
		delete m_pStationThreadsEvent[type];
	}

	// 画面のスレッド終了
	ExitThread();

	// CheckDialog 終了処理
	theApp.StopCheckDialog();

	CMDIFrameWndEx::OnClose();
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIFrameWndEx::OnSize(nType, cx, cy);

//	switch (nType)
//	{
//	case SIZE_RESTORED:
//		// サイズ変更時
//	case SIZE_MAXIMIZED:
//		// 最大化時
//		if (m_nLastSizeType == SIZE_MINIMIZED)
//		{
//			// 直前のアプリウィンドウサイズが最小の場合はレイアウト読み込み
//			LoadLayout(theApp.GetAppLayoutPath());
//		}
//		break;
//
//	case SIZE_MINIMIZED:
//		// 最小化時
//		// レイアウト保存
//		SaveLayout(theApp.GetAppLayoutPath());
//		break;
//
//	default:
//		break;
//	}
//
//	// 直前のアプリサイズタイプを更新
//	m_nLastSizeType = nType;

	if (nType == SIZE_MAXIMIZED){
		RECT rect;
		MONITORINFOEX info;
		if (mMonitor.GetCount() != 0 && mMonitor.GetMonitor(0, &rect, &info) == true){
			MoveWindow(&rect);
		}
	}

	return;

	//CSize size;
	//if (m_wndMenuBar.GetSafeHwnd()){
	//	size = m_wndMenuBar.CalcFixedLayout(TRUE, TRUE);
	//	CRect rect;
	//	m_wndMenuBar.GetWindowRect(rect);
	//	ScreenToClient(rect);
	//	rect.right = rect.left + size.cx;
	//	rect.bottom = rect.top + size.cy;
	//	m_wndMenuBar.MoveWindow(rect);
	//	rect.left = rect.right - MAX_EMGBUTTON_WIDTH;
	//	rect.DeflateRect(2, 2);
	//	m_wndMenuBar.InvalidateRect(rect);
	//}
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 0){
		//CString str;
		//SYSTEMTIME time;
		//GetLocalTime(&time);
		//str.Format(_T("%04d/%02d/%02d %02d:%02d:%02d\0"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
		//UINT style = m_wndStatusBar.GetPaneStyle(m_wndStatusBar.CommandToIndex(ID_INDICATOR_TIME));
		//style &= ~SBPS_DISABLED;
		//m_wndStatusBar.SetPaneStyle(m_wndStatusBar.CommandToIndex(ID_INDICATOR_TIME), style);
		//m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_TIME), str);
		//m_wndToolBar.RedrawWindow();
		m_wndCaption.RedrawWindow();
		return;
	}
	if (nIDEvent == 1){
		KillTimer(1);
		PostMessage(eMessage_DirectPrint, 0, 0);
		return;
	}
	if (nIDEvent == 9){
		PIPEMONITORACK buf;
		buf.header.size = 137;
		theApp.GetDataCollection().CreateDataToHistory(&buf);
		PostMessage(eMessage_ThreadNormal, 0, 0);
		return;
	}
	if (nIDEvent == 100){
		EventToneProc();
		return;
	}
	if (nIDEvent == 200)
	{
		KillTimer(200);
		ExecRdnsChkPlanForceStop();
	}

	CMDIFrameWndEx::OnTimer(nIDEvent);
}
/*============================================================================*/
/*! プロセス間通信

-# AP層からのメッセージ受信

@param

@retval
*/
/*============================================================================*/
void CMainFrame::EventToneProc()
{
	// 上記テーブルの監視データを取得する
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT station = theApp.GetSelectStation();

	const CTimeSpan sp(0, 0, 0, 10);

	CTime ctime, mtime;
	theApp.GetCurrentUtcTime(ctime);

	// 音は10秒間鳴らすので、10秒経過しているかチェックする
	// パス開始中の場合はパス終了時のみをチェックする
	if (m_dwEventStatus != 0 && !(m_dwEventStatus&(1 << eEvent_PassStart))){
		// 鳴動中
		DWORD elaps = timeGetTime() - m_dwEventStart;
		if (elaps >= 10000){
			// 音を止める
			StopEventSound();
			m_dwEventStart = 0;
			m_dwEventStatus = 0;
		}
		else{
			// 鳴動中なので何もしない
			return;
		}
	}

	// ここにくるときは、鳴動中でないか、パス開始中の場合
	for (int i = 0; i < sizeof(mEventTone) / sizeof(mEventTone[0]); i++){
		if ((m_dwEventStatus&(1 << eEvent_PassStart))){
			// パス開始中の場合はパス終了時のみをチェックする
			if (i != eEvent_PassEnd)
				continue;
		}
		obs_adr = (CEngValCnv::obsname_t *)theApp.GetSatelliteData().GetDBAccessCls().GetObsname_t(station, mEventTone[i].monname);
		if (obs_adr != NULL){
			// 監視名が見つかった
			CString temp = _T("");

			switch (i){
			case	0:// 初期設定開始時
				mtime = CSatelliteData::StrTimeToCTime(CString(obs_adr->sz_sts_name));
				if (ctime <= mtime && ctime + sp >= mtime){
					// 範囲内なので音を鳴らす
				} else{
					// 初期設定開始時刻でないので何もしない
					temp = ctime.Format(_T("%Y-%m-%d %H:%M:%S"));
					temp = mtime.Format(_T("%Y-%m-%d %H:%M:%S"));
					temp = (ctime+sp).Format(_T("%Y-%m-%d %H:%M:%S"));
					continue;
				}
				break;
			case	1:// パス開始時
				mtime = CSatelliteData::StrTimeToCTime(CString(obs_adr->sz_sts_name));
				if (ctime <= mtime && ctime + sp >= mtime){
					// 範囲内なので音を鳴らす
				} else{
					// パス開始時刻でないので何もしない
					temp = ctime.Format(_T("%Y-%m-%d %H:%M:%S"));
					temp = mtime.Format(_T("%Y-%m-%d %H:%M:%S"));
					continue;
				}
				break;
			case	2:// パス終了時
				if (!(m_dwEventStatus&(1 << eEvent_PassStart))){
					// パス開始音は鳴らしていないので何もしない
					continue;
				}
				// 「CTRL.START_TIME」で取得した時刻を経過し、鳴動中
				temp = CString(obs_adr->sz_sts_name);
				temp.TrimLeft();
				temp.TrimRight();
				if (temp.CompareNoCase(_T("運用終了")) == 0 || temp.CompareNoCase(_T("運用終了処理中")) == 0){
					// パス開始音を止める
					StopEventSound();
					// 「CTRL.PASS_INFO」の値が”運用終了”または”運用終了処理中”に変化したならば鳴動する
					// パス終了時刻に達したので音を鳴らす
				}
				else{
					// パス終了時刻でないので何もしない
					continue;
				}
				break;
			}

			// 指定の音を10秒間鳴らす
			PlayEventSound(i);
			// 音を鳴らすのはテーブル内の項目どれか一つ
			m_dwEventStart = timeGetTime();
			m_dwEventStatus = mEventTone[i].code;
			break;
		}
	}
}

void CMainFrame::OnCommandStatusTime()
{
}
void CMainFrame::OnUpdateStatusTime(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}

/*============================================================================*/
/*! アプリケーション

-# 衛星捕捉グラフ

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnViewTrend()
{
	if (theApp.GetView(eViewType1_Trend) == NULL)
		theApp.OpenView(eViewType1_Trend);
	else
	{
//		theApp.CloseView(eViewType1_Trend);
		CView *view = theApp.GetView(eViewType1_Trend);
		((CMDIChildWnd*)view->GetParent())->MDIActivate();
	}
}
void CMainFrame::OnUpdateViewTrend(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetView(eViewType1_Trend)!=NULL)?1:0);
}
/*============================================================================*/
/*! アプリケーション

-# 運用詳細

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnViewOpeDetail()
{
	//if (theApp.GetView(eViewType1_OpeDetail) == NULL)
	//	theApp.OpenView(eViewType1_OpeDetail);
	//else
	//	theApp.CloseView(eViewType1_OpeDetail);
}
void CMainFrame::OnUpdateViewOpeDetail(CCmdUI *pCmdUI)
{
	//pCmdUI->SetCheck((theApp.GetView(eViewType1_OpeDetail) != NULL) ? 1 : 0);
}
/*============================================================================*/
/*! アプリケーション

-# 衛星捕捉状況

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnViewStationSit()
{
	//if (theApp.GetView(eViewType1_StationSit) == NULL)
	//	theApp.OpenView(eViewType1_StationSit);
	//else
	//	theApp.CloseView(eViewType1_StationSit);
}
void CMainFrame::OnUpdateViewStationSit(CCmdUI *pCmdUI)
{
	//pCmdUI->SetCheck((theApp.GetView(eViewType1_StationSit) != NULL) ? 1 : 0);
}
/*============================================================================*/
/*! アプリケーション

-# 設備・運用状況

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnViewEqOperation()
{
	//if (theApp.GetSelectStationMode() == eStation_1){
	//	if (theApp.GetView(eViewType1_EqOperation) == NULL)
	//		theApp.OpenView(eViewType1_EqOperation);
	//	else
	//		theApp.CloseView(eViewType1_EqOperation);
	//}
	if (theApp.GetSelectStationMode() == eStation_5){
		if (theApp.GetView(eViewType5_EqOperation1) == NULL)
			theApp.OpenView(eViewType5_EqOperation1);
		else
		{
//			theApp.CloseView(eViewType5_EqOperation1);
			CView *view = theApp.GetView(eViewType5_EqOperation1);
			((CMDIChildWnd*)view->GetParent())->MDIActivate();
		}
	}
}
void CMainFrame::OnUpdateViewEqOperation(CCmdUI *pCmdUI)
{
	if (theApp.GetSelectStationMode() == eStation_1){
		//pCmdUI->SetCheck((theApp.GetView(eViewType1_EqOperation) != NULL) ? 1 : 0);
	}
	else if (theApp.GetSelectStationMode() == eStation_5)
		pCmdUI->SetCheck((theApp.GetView(eViewType5_EqOperation1) != NULL) ? 1 : 0);
}
void CMainFrame::OnViewEqOperation2()
{
	if (theApp.GetView(eViewType5_EqOperation2) == NULL)
		theApp.OpenView(eViewType5_EqOperation2);
	else
	{
//		theApp.CloseView(eViewType5_EqOperation2);
		CView *view = theApp.GetView(eViewType5_EqOperation2);
		((CMDIChildWnd*)view->GetParent())->MDIActivate();
	}
}
void CMainFrame::OnUpdateViewEqOperation2(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetView(eViewType5_EqOperation2) != NULL) ? 1 : 0);
}
void CMainFrame::OnViewEqOperation3()
{
	if (theApp.GetView(eViewType5_EqOperation3) == NULL)
		theApp.OpenView(eViewType5_EqOperation3);
	else
	{
//		theApp.CloseView(eViewType5_EqOperation3);
		CView *view = theApp.GetView(eViewType5_EqOperation3);
		((CMDIChildWnd*)view->GetParent())->MDIActivate();
	}
}
void CMainFrame::OnUpdateViewEqOperation3(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetView(eViewType5_EqOperation3) != NULL) ? 1 : 0);
}
void CMainFrame::OnViewEqOperation4()
{
	if (theApp.GetView(eViewType5_EqOperation4) == NULL)
		theApp.OpenView(eViewType5_EqOperation4);
	else
	{
//		theApp.CloseView(eViewType5_EqOperation4);
		CView *view = theApp.GetView(eViewType5_EqOperation4);
		((CMDIChildWnd*)view->GetParent())->MDIActivate();
	}
}
void CMainFrame::OnUpdateViewEqOperation4(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetView(eViewType5_EqOperation4) != NULL) ? 1 : 0);
}
void CMainFrame::OnViewEqOperation5()
{
	if (theApp.GetView(eViewType5_EqOperation5) == NULL)
		theApp.OpenView(eViewType5_EqOperation5);
	else
	{
//		theApp.CloseView(eViewType5_EqOperation5);
		CView *view = theApp.GetView(eViewType5_EqOperation5);
		((CMDIChildWnd*)view->GetParent())->MDIActivate();
	}
}
void CMainFrame::OnUpdateViewEqOperation5(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetView(eViewType5_EqOperation5) != NULL) ? 1 : 0);
}


/*============================================================================*/
/*! アプリケーション

-# 画面レイアウトの保存

@param	filename	保存ファイル
@retval

*/
/*============================================================================*/
void CMainFrame::SaveLayout(CString filename)
{
	CFile file;
	if (file.Open(filename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) == NULL){
		return;
	}
	CArchive mArc(&file, CArchive::store);

	bool ret;

	try
	{
		// 端末アプリ名の設定
		mArc << m_strAppName;
		// レイアウト対応アプリバージョンの設定
		mArc << m_nVersion;
		// 局モードの設定
		mArc << theApp.GetSelectStationMode();
		// 選択局の設定
		mArc << theApp.GetSelectStation();
		// 制御モードの設定
		mArc << theApp.GetExecuteMode();

		// Z順にウィンドウ位置を保存する
		CWnd *pWnd = GetDesktopWindow()->GetWindow(GW_CHILD);
		UINT type;
		UINT subtype;
		while (pWnd){
			if (pWnd == this){
				CMDIChildWnd* pcli = MDIGetActive();
				while (pcli){
					type = theApp.GetStationWindowType(pcli);
					if (type != 0xFFFFFFFF){
						TRACE("### Open Window : %d\n", type);
						mArc << type;
						ret = ((CChildFrame*)(pcli))->Save(mArc);
						if (ret == false){
							AfxThrowUserException();
						}
					}
					pcli = (CMDIChildWnd*)pcli->GetNextWindow();
				}
			}
			else{
				type = theApp.GetStationWindowType(pWnd);
				if (type != 0xFFFFFFFF){
					if (IsLayoutExcludeDlg(type) == FALSE)
					{
						// レイアウト除外対象ダイアログでない場合
						TRACE("### Open Window : %d\n", type);
						mArc << type;
						subtype = ((CDialogBase*)pWnd)->GetWindowSubCode();
						mArc << subtype;
						ret = ((CDialogBase*)(pWnd))->Save(mArc);
						if (ret == false){
							AfxThrowUserException();
						}
					}
				}
			}
			pWnd = pWnd->GetWindow(GW_HWNDNEXT);
		}

		// ウィンドウの終了
		mArc << (UINT)eViewType_Max;

		// 各種設定情報の保存
		theApp.GetSatelliteData().Save(mArc);

		ret = true;
	}
	catch (CArchiveException* e)
	{
		e->Delete();
		ret = false;
	}
	catch (CFileException* e)
	{
		e->Delete();
		ret = false;
	}
	catch (CMemoryException* e)
	{
		e->Delete();
		ret = false;
	}
	catch (CUserException* e)
	{
		e->Delete();
		ret = false;
	}

	mArc.Flush();
	file.Close();

	if(ret == false){
		DeleteFile(filename);
	}
}

UINT CMainFrame::GetLoadLayoutIndex(UINT type, UINT subtype /*= 0*/)
{
	UINT ret = 0xffffffff;
	for (UINT i = 0; i < sizeof(mLayoutCreateDlg) / sizeof(mLayoutCreateDlg[0]); i++){
		if (mLayoutCreateDlg[i].code == type && mLayoutCreateDlg[i].subcode == subtype){
			ret = i;
			break;
		}
	}
	return ret;
}

BOOL CMainFrame::IsLayoutExcludeDlg(UINT type)
{
	for (UINT i = 0; i < sizeof(mLayoutExcludeDlg) / sizeof(mLayoutExcludeDlg[0]); i++){
		if (mLayoutExcludeDlg[i].code == type){
			return TRUE;
		}
	}
	return FALSE;
}

UINT CMainFrame::GetLoadLayoutOtherViewIndex(UINT type)
{
	UINT ret = 0xffffffff;
	for (UINT i = 0; i < sizeof(mLayoutCreateOtherView) / sizeof(mLayoutCreateOtherView[0]); i++){
		if (mLayoutCreateOtherView[i].code == type){
			ret = i;
			break;
		}
	}
	return ret;
}

/*============================================================================*/
/*! アプリケーション

-# 画面レイアウトの読込

@param	filename	読込ファイル
@retval

*/
/*============================================================================*/
void CMainFrame::LoadLayout(CString filename)
{
	CFile file;
	if (file.Open(filename, CFile::modeRead | CFile::typeBinary) == NULL){
		return;
	}
	CArchive mArc(&file, CArchive::load);

	bool ret;
	UINT type;
	UINT subtype;

	try
	{
		CString strAppName;
		double nVersion;
		UINT nSelectMode;
		UINT nSelectStation;
		UINT nExecuteMode;
		// 端末アプリ名の設定
		mArc >> strAppName;
		// レイアウト対応アプリバージョンの設定
		mArc >> nVersion;

		CString title = _T("");
		GetWindowText(title);

		if (strAppName.CompareNoCase(m_strAppName) != 0 || nVersion < m_nVersion)
		{
			MessageBox(_T("レイアウトファイルでないか、または対応していないバージョンのファイルです。"), title, MB_OK | MB_ICONWARNING);
			AfxThrowUserException();
		}
			// 局モードの設定
		mArc >> nSelectMode;
		// 選択局の設定
		mArc >> nSelectStation;

		if (nSelectMode != theApp.GetSelectStationMode() || nSelectStation != theApp.GetSelectStation()){
			MessageBox(_T("レイアウトファイルが現在の局モードと異なっています。"), title, MB_OK | MB_ICONWARNING);
			AfxThrowUserException();
		}

		// 制御モードの設定
		mArc >> nExecuteMode;
		if (nExecuteMode != theApp.GetExecuteMode()){
			MessageBox(_T("レイアウトファイルが現在の制御モードと異なっています。"), title, MB_OK | MB_ICONWARNING);
			AfxThrowUserException();
		}

		// 画面のスレッド終了
		ExitThread();

		// 一旦全ダイアログを消去する
		theApp.RemoveWindowHandle(0);

		while (1){
			mArc >> type;
			if (type == eViewType_Max)
				break;
			if (type < eViewType_Max){
				// ビュー関連
				if (type >= eViewType1_Trend && type < eViewType1_Max && theApp.GetSelectStationMode() != eStation_1)
					AfxThrowUserException();
				else if (type >= eViewType5_EqOperation1 && type < eViewType5_Max && theApp.GetSelectStationMode() != eStation_5)
					AfxThrowUserException();

				if (theApp.GetView(type) == NULL)
				{
					UINT ret = GetLoadLayoutOtherViewIndex(type);
					if (ret != 0xffffffff){
						// その他ビューの場合はメニューからの関数呼び出し
						(this->*mLayoutCreateOtherView[ret].func)();
					}
					else
					{
						// 通常のビュー
						theApp.OpenView(type);
					}
				}
				((CChildFrame*)(theApp.GetView(type)->GetParent()))->Load(mArc);
			}
			else{
				mArc >> subtype;
				// 設定ダイアログ関連
				UINT ret = GetLoadLayoutIndex(type, subtype);
				if (ret != 0xffffffff){
					(this->*mLayoutCreateDlg[ret].func)();
					CWnd* pdlg = theApp.GetWindowHandle(type);
					if (pdlg != NULL)
						((CDialogBase*)(pdlg))->Load(mArc);
				}
				else if (IsLayoutExcludeDlg(type) == TRUE){
					// 除外ダイアログの場合は復元しない
				}
				else{
					CDialogBase* pdlg = new CDialogBase(type);
					if (pdlg != NULL){
						if (pdlg->Create(type, this)){
							pdlg->ShowWindow(SW_SHOW);
							theApp.AddWindowHandle((LPARAM)pdlg, type);
							((CDialogBase*)(pdlg))->Load(mArc);
						}
					}
				}
			}
		}

		// 各種設定情報の復
		theApp.GetSatelliteData().Load(mArc);

		// トレンドグラフ設定情報の復元
		CTrendGraphView* pTrend = (CTrendGraphView*)theApp.GetView(eViewType1_Trend);
		if (pTrend != NULL){
			pTrend->Load(mArc);
		}

		// スペクトラムグラフ設定情報の復元
		CSpectrumView* pSpecWnd = (CSpectrumView*)theApp.GetWindowHandle(IDD_DLG_SPECTRUMVIEW);
		if (pSpecWnd != NULL){
			pSpecWnd->Load(mArc);
		}

		// サマリグラフの設定情報の保存
		if (theApp.GetSelectStationMode() == eStation_1){
			// １局モード
			switch (theApp.GetSelectStation()){
			case	eStation_Uchinoura34:
			{
				CEquipmentStatusView34* pSummary = (CEquipmentStatusView34*)theApp.GetView(eViewType1_EquipmentStatus);
				if (pSummary != NULL){
					pSummary->Load(mArc);
				}
				break;
			}
			case	eStation_Uchinoura20:
			{
				CEquipmentStatusView20* pSummary = (CEquipmentStatusView20*)theApp.GetView(eViewType1_EquipmentStatus);
				if (pSummary != NULL){
					pSummary->Load(mArc);
				}
				break;
			}
			case	eStation_Usuda642:
			{
				// 54m
				CEquipmentStatusView54* pSummary = (CEquipmentStatusView54*)theApp.GetView(eViewType1_EquipmentStatus);
				if (pSummary != NULL){
					pSummary->Load(mArc);
				}
				break;
			}
			case	eStation_Usuda64:
			default:
			{
				CEquipmentStatusView* pSummary = (CEquipmentStatusView*)theApp.GetView(eViewType1_EquipmentStatus);
				if (pSummary != NULL){
					pSummary->Load(mArc);
				}
				break;
			}
			}
		}
		else{
			// ４局モード
			CEquipmentStationStatusView* pSummary1 = (CEquipmentStationStatusView*)theApp.GetView(eViewType5_EqOperation1);
			if (pSummary1 != NULL){
				pSummary1->Load(mArc);
			}
			CEquipmentStationStatusView* pSummary2 = (CEquipmentStationStatusView*)theApp.GetView(eViewType5_EqOperation2);
			if (pSummary2 != NULL){
				pSummary2->Load(mArc);
			}
			CEquipmentStationStatusView* pSummary3 = (CEquipmentStationStatusView*)theApp.GetView(eViewType5_EqOperation3);
			if (pSummary3 != NULL){
				pSummary3->Load(mArc);
			}
			CEquipmentStationStatusView* pSummary4 = (CEquipmentStationStatusView*)theApp.GetView(eViewType5_EqOperation4);
			if (pSummary4 != NULL){
				pSummary4->Load(mArc);
			}
		}

		ret = true;
	}
	catch (CArchiveException* e)
	{
		e->Delete();
		ret = false;
	}
	catch (CFileException* e)
	{
		e->Delete();
		ret = false;
	}
	catch (CMemoryException* e)
	{
		e->Delete();
		ret = false;
	}
	catch (CUserException* e)
	{
		e->Delete();
		ret = false;
	}

	mArc.Flush();
	file.Close();
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

/*============================================================================*/
/*! アプリケーション

-# 画面ハードコピー

@param		pwnd	ハードコピーを行いたいウィンドウハンドル
@retval

*/
/*============================================================================*/
bool CMainFrame::DirectPrint(CWnd* pwnd)
{
	CDC dc, memDC, printDC;
	HDC hPrint;

	CPrintDialog* printDlg = NULL;
	CString mDefaultPrinterName = theApp.GetDefaultPrintName();
	if (mDefaultPrinterName.IsEmpty() == true){
		printDlg = new CPrintDialog(FALSE, PD_DISABLEPRINTTOFILE);
		printDlg->GetDefaults();
		mDefaultPrinterName = printDlg->GetDeviceName();
		hPrint = printDlg->GetPrinterDC();
		delete printDlg;
	}
	else{
		if (printDC.CreateDC(NULL, mDefaultPrinterName, NULL, NULL) == 0){
			return false;
		}
		hPrint = printDC.m_hDC;
	}

	if (dc.Attach(hPrint) == 0){
			return false;
	}
	dc.m_bPrinting = TRUE;

	CBitmap bitmap;
	if (GeWindowCaptureMeasure(pwnd, bitmap) == false)
		return false;

	// 全画面をキャプチャできたので、選択プリンタにイメージを送る
	DOCINFO di;
	CString title;
	if (title.LoadString(AFX_IDS_APP_TITLE) == FALSE)
	{
		return false;
	}
	::ZeroMemory(&di, sizeof(DOCINFO));
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = title;

	// 印刷の開始
	if (dc.StartDoc(&di) < 0)
		return false;

	int cx = dc.GetDeviceCaps(HORZRES);
	int cy = dc.GetDeviceCaps(VERTRES);

	CRect rect;
	// ウィンドウ矩形を取得
	pwnd->GetWindowRect(rect);

	// 倍率を求める
	float fx = (float)cx / (float)rect.Width();
	float fy = (float)cy / (float)rect.Height();

	memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(&bitmap);
	dc.StretchBlt(0, 0, (fx<fy) ? cx : (int)(fy*(float)rect.Width()), (fx<fy) ? (int)(fx*(float)rect.Height()) : cy, &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	dc.EndPage();
	dc.EndDoc();
	dc.Detach();

	memDC.DeleteDC();

	return true;
}
/*============================================================================*/
/*! アプリケーション

-# 指定ウィンドウのスクリーンショットを取得する

@param	pWnd				ウィンドウハンドル(NULLの場合アプリケーションキャプチャ)
@retval	bool

*/
/*============================================================================*/
bool CMainFrame::GeWindowCaptureMeasure(CWnd* pWnd, CBitmap& bitmap)
{
	CWaitCursor wait;

	try{
		if (pWnd == NULL || pWnd->GetSafeHwnd() == NULL){
			return false;
		}

		WINDOWPLACEMENT	save;

		// 現在ウィンドウ状態を保存する。
		pWnd->GetWindowPlacement(&save);
		::SetCapture(pWnd->m_hWnd);

		CRect rect;
		// ウィンドウ矩形を取得
		pWnd->GetWindowRect(rect);
		pWnd->GetClientRect(rect);
		pWnd->ClientToScreen(rect);
		// 矩形領域のキャプチャ
		CImageTool img;
		if (bitmap.GetSafeHandle()){
			bitmap.DeleteObject();
		}
		bool ret = img.CaptureRect(rect, bitmap);
		if (ret == false)
			return false;

		// ファイルに保存する場合は以下を有効にする
		//ret = img.SaveImageToFile(pWnd, bitmap, _T("capture.bmp"));

		// ウィンドウ状態を元に戻す
		pWnd->SetWindowPlacement(&save);
		::ReleaseCapture();
	}
	catch (...){
		return false;
	}
	return true;
}

/*============================================================================*/
/*! スレッド

-# 起動プロセスの終了

@param
@retval

*/
/*============================================================================*/
BOOL CALLBACK FinishWindowsProc(HWND hWnd, LPARAM lParam)
{
	PROCESS_INFORMATION* pi = (PROCESS_INFORMATION*)lParam;

	// ウインドウを作成したプロセスIDを取得。
	DWORD lpdwProcessId = 0;
	::GetWindowThreadProcessId(hWnd, &lpdwProcessId);

	// CreateProcessで起動したアプリのプロセスIDとメインウィンドウを
	// 作成したプロセスIDが同じ場合、起動したアプリを終了させる。
	if (pi->dwProcessId == lpdwProcessId){
		::PostMessage(hWnd, WM_CLOSE, 0, 0);
		return FALSE;
	}
	return TRUE;
}
/*============================================================================*/
/*! スレッド

-# プロセスの起動

@param
@retval

*/
/*============================================================================*/
BOOL LaunchApp(LPCTSTR pCmdLine, PROCESS_INFORMATION* pProcessInfo)
{
	// 新しいプロセス用の構造体を起動する
	STARTUPINFO    stStartUpInfo;

	// メンバーの多くは予約されているか無視されるので、すべてのメンバーを
	// 0 に設定してから、必要なメンバーだけをリセットする
	memset(&stStartUpInfo, 0, sizeof(STARTUPINFO));

	stStartUpInfo.cb = sizeof(STARTUPINFO);			// 構造体のサイズ
	stStartUpInfo.dwFlags = STARTF_USESHOWWINDOW;	// wShowWindow メンバーを使う
	stStartUpInfo.wShowWindow = SW_SHOWDEFAULT;		// デフォルトの値

	// 新しいプロセスを作ると、プロセスはプライマリスレッドを作り、
	// アプリケーションを起動する
	return CreateProcess(NULL, (LPTSTR)pCmdLine, NULL, NULL, FALSE,
		/*CREATE_SUSPENDED*/NORMAL_PRIORITY_CLASS, NULL,
		NULL, &stStartUpInfo, pProcessInfo);
}
/*============================================================================*/
/*! スレッド

-# アプリケーションの起動監視スレッド

@param
@retval

*/
/*============================================================================*/
UINT AppLaunchAndWait(LPVOID pParam)
{
	// パラメータのアドレスを代入する
	stThreadInfo* pInfo = (stThreadInfo*)pParam;

	CString strPath = theApp.GetAppPath();
	TCHAR szCommandLine[_MAX_PATH];
	// 新しいプロセス上で情報を受け取るプロセス構造体
	PROCESS_INFORMATION stProcessInfo;

	// アプリケーションを起動する
	if (pInfo->bPassStatus == true){
		// 運用計画アプリ起動
		swprintf_s(szCommandLine, _MAX_PATH, _T("%s%s /HWND:%08X /STATION:%d /EXECMODE:%d"), (LPCTSTR)strPath, (LPCTSTR)APP_TIMELINE, (UINT)(pInfo->pWnd)->m_hWnd, (UINT)(pInfo->nStation), (UINT)(pInfo->nExecuteMode));
	}
	else{
		// １局モードアプリ起動
		TCHAR	szFullPath[_MAX_PATH];
		::GetModuleFileName(NULL, szFullPath, _MAX_PATH);
		swprintf_s(szCommandLine, _MAX_PATH, _T("%s /MODE:1 /STATION:%d"), szFullPath, (UINT)(pInfo->nStation + 1));
	}
	SetCurrentDirectory(szCommandLine);

	if (LaunchApp(szCommandLine, &stProcessInfo)){
		if (pInfo->bPassStatus == false){
			theApp.SetLaunchStation(pInfo->nStation, true);
		}
		HANDLE hThreads[2];
		hThreads[0] = pInfo->pTermThreadEvent->m_hObject;
		hThreads[1] = stProcessInfo.hProcess;
		//WORD dwIndex = WaitForSingleObject(stProcessInfo.hProcess, INFINITE);
		DWORD dwIndex = WaitForMultipleObjects(2, hThreads, FALSE, INFINITE);
		// プロセスとメインスレッドへのハンドルを閉じる
		// コールバック関数の呼び出し。
		//EnumWindows(FinishWindowsProc, (LPARAM)&stProcessInfo);

		::SendMessage(theApp.GetTimeLineHandle(), eMessage_TimeLineFinish, 0, (LPARAM)0);

		CloseHandle(stProcessInfo.hThread);
		CloseHandle(stProcessInfo.hProcess);
		if (pInfo->bPassStatus == false){
			theApp.SetLaunchStation(pInfo->nStation, false);
		}
	}

	// メインスレッドが構造体を割り当てたので、
	// これを解放しなければならない
	if (pInfo)
		delete pInfo;

	theApp.SetTimeLineHandle(NULL);
	TRACE("App Finished\n");

	return 0;
}

/*============================================================================*/
/*! アプリケーション

-# 運用タイムライン

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnViewOpeTimeLine()
{
	if (m_bCreatePipe[ePIPETYPE_TIMELINE] == false){
		stThreadInfo* pThreadInfo = new stThreadInfo;
		if (pThreadInfo){
			createPipe(ePIPETYPE_TIMELINE);
			//createPipe(ePIPETYPE_STATION);
			// 構造体のメンバーに代入する
			pThreadInfo->pTermThreadEvent = m_pTimeLineThreadsEvent;
			pThreadInfo->pWnd = this;
			pThreadInfo->bPassStatus = true;
			pThreadInfo->nStation = theApp.GetSelectStation();
			pThreadInfo->nExecuteMode = theApp.GetExecuteMode();
			AfxBeginThread(AppLaunchAndWait, pThreadInfo);
		}
	}
	else{
		deletePipe(ePIPETYPE_TIMELINE);
	}
}
void CMainFrame::OnUpdateViewOpeTimeLine(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_bCreatePipe[ePIPETYPE_TIMELINE] == true) ? 1 : 0);
}

// ■プロセス間通信
/*============================================================================*/
/*! プロセス間通信

-# AP層からのメッセージ受信

@param

@retval
*/
/*============================================================================*/
void WINAPI CMainFrame::receiveServerCallBack(LPPIPEBUFFER pbuf)
{
	::SendMessage(CMainFrame::GetPipeMessageWnd()->m_hWnd, ID_PIPE_RECEIVE, 1, (LPARAM)pbuf);
}

/*============================================================================*/
/*! プロセス間通信

-# メイン画面からのメッセージ受信

@param

@retval
*/
/*============================================================================*/
void WINAPI CMainFrame::receiveClientCallBack(LPPIPEBUFFER pbuf)
{
	::SendMessage(CMainFrame::GetPipeMessageWnd()->m_hWnd, ID_PIPE_RECEIVE, 0, (LPARAM)pbuf);
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
	//TRACE("### UDSC64\n");
	// UDSC64
	CGraphMutex::Lock(eMessageUdsc64);
	deque<PIPEMONITORACK>& data = theApp.GetDataCollection().GetMessageTemp(0);
	data.push_back(*pbuf);
	CGraphMutex::Unlock(eMessageUdsc64);
}
void WINAPI CMainFrame::receiveServerServiceCallBack02(PIPEMONITORACK* pbuf)
{
	//TRACE("### USC34\n");
	// USC34
	CGraphMutex::Lock(eMessageUsc34);
	deque<PIPEMONITORACK>& data = theApp.GetDataCollection().GetMessageTemp(2);
	data.push_back(*pbuf);
	CGraphMutex::Unlock(eMessageUsc34);
}
void WINAPI CMainFrame::receiveServerServiceCallBack03(PIPEMONITORACK* pbuf)
{
	//TRACE("### USC20\n");
	// USC20
	CGraphMutex::Lock(eMessageUsc20);
	deque<PIPEMONITORACK>& data = theApp.GetDataCollection().GetMessageTemp(3);
	data.push_back(*pbuf);
	CGraphMutex::Unlock(eMessageUsc20);
}
void WINAPI CMainFrame::receiveServerServiceCallBack04(PIPEMONITORACK* pbuf)
{
	//TRACE("### UDSC54\n");
	// UDSC54
	CGraphMutex::Lock(eMessageUdsc54);
	deque<PIPEMONITORACK>& data = theApp.GetDataCollection().GetMessageTemp(1);
	data.push_back(*pbuf);
	CGraphMutex::Unlock(eMessageUdsc54);
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
	//	deque<PIPEMONITORACK>& data = theApp.GetDataCollection().GetMessageTemp();
	//	data.push_back(*pbuf);
	//	CGraphMutex::Unlock(eMessage);
	//	if (data.size() >= 100)
	//		Sleep(10);
	//	return;
	//}

#ifdef _DEBUG
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ReadFile, size(%u), command(%u), station(%u), matching(%u)"), __FILEW__, __LINE__, pbuf->header.size, pbuf->header.command, pbuf->header.station, pbuf->header.matching);
		CLogTraceEx::Write(_T(""), _T("CMainFrame"), _T("receiveServerServiceCallBack"), _T("debug"), strDebug, nLogEx::debug);
	}
#endif
	if (CMainFrame::GetEngval(0) == NULL){
		// 工学値変換用のクラスを作成
		for (int i = 0; i < eStation_MAX; i++)
		{
			CString strFilePath = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, i);
			char szFilePath[_MAX_PATH];
			WideCharToMultiByte(CP_ACP, 0, strFilePath, -1, szFilePath, sizeof(szFilePath), NULL, NULL);
#ifdef _DEBUG
			{
				CString strDebug;
				strDebug.Format(_T("%s(%d) :: DB file path(%s)"), __FILEW__, __LINE__, (LPCTSTR)strFilePath);
				CLogTraceEx::Write(_T(""), _T("CMainFrame"), _T("receiveServerServiceCallBack"), _T("debug"), strDebug, nLogEx::debug);
			}
#endif

			CMainFrame::SetEngval(i, new CEngValCnv(szFilePath));
#ifdef _DEBUG
			if (CMainFrame::GetEngval(i) == NULL)
			{
				CString strDebug;
				strDebug.Format(_T("%s(%d) :: Fail new CEngValCnv, station(%d)"), __FILEW__, __LINE__, i);
				CLogTraceEx::Write(_T(""), _T("CMainFrame"), _T("receiveServerServiceCallBack"), _T("debug"), strDebug, nLogEx::debug);
			}
			else
			{
				CString strDebug;
				strDebug.Format(_T("%s(%d) :: Success new CEngValCnv, station(%d)"), __FILEW__, __LINE__, i);
				CLogTraceEx::Write(_T(""), _T("CMainFrame"), _T("receiveServerServiceCallBack"), _T("debug"), strDebug, nLogEx::debug);
			}
#endif
		}
	}


	int err;
	size_t	size;

	BOOL	bAddTrend = FALSE;
	UINT nStation = 0;
	if (pbuf->header.station == 1) nStation = 0;
	if (pbuf->header.station == 2) nStation = 2;
	if (pbuf->header.station == 3) nStation = 3;
	if (pbuf->header.station == 4) nStation = 1;

	// StationControlアプリの場合
	switch (pbuf->header.command)
	{
	case ePipeCommand_monitorget:
	{
		if (theApp.GetPipeIgnore() & 0x01)
			return;

		DWORD	dwStart = timeGetTime();
		UINT nSelectStationMode = theApp.GetSelectStationMode();
		UINT nSelectStation = theApp.GetSelectStation();
		// 監視生データ
		// bufをAppのdequeに登録
		// 1局モードの場合は指定局、4局モードの場合は局番号が範囲内の場合
		//if ((nSelectStationMode == eStation_1 &&
		//		((nSelectStation == 0 && pbuf->header.station == 1) ||
		//		(nSelectStation == 1 && pbuf->header.station == 4) ||
		//		(nSelectStation == 2 && pbuf->header.station == 2) ||
		//		(nSelectStation == 3 && pbuf->header.station == 3))) ||
		//	(nSelectStationMode == eStation_5 && pbuf->header.station >= eStation_Usuda64 + 1 && pbuf->header.station <= eStation_MAX))
		//{
		// 周波数制御では、自局以外の送信周波数を取得するため、全局の監視データを取得する
		if (pbuf->header.station >= eStation_Usuda64 + 1 && pbuf->header.station <= eStation_MAX)
		{
			if (CMainFrame::GetEngval(nStation) != NULL)
			{
				err = CMainFrame::GetEngval(nStation)->CalcFromMsg((CEngValCnv::sdtp_msg_ex_t*)&(pbuf->buf[0]));

				if (err == 0)
				{
					//CString strDebug;
					//strDebug.Format(_T("%s(%d) :: CalcFromMsg no err(%d)"), __FILEW__, __LINE__, err);
					//CLogTraceEx::Write(_T(""), _T("CMainFrame"), _T("receiveServerServiceCallBack"), _T("debug"), strDebug, nLogEx::debug);

					vector<CEngValCnv::obs_tbl_res_t>& data = CMainFrame::GetEngval(nStation)->GetAllVal();
					size = data.size();

// dummy data >>>
//					CEngValCnv::obs_tbl_res_t	newData;
//					memset(&newData, 0, sizeof(CEngValCnv::obs_tbl_res_t));
//					CEngValCnv::obsname_t obs_adr;
//					newData.obs_adr = &obs_adr;
//					strcpy_s(newData.obs_adr->sz_obsname, sizeof(newData.obs_adr->sz_sts_name), "SS-TCRAGC.AGC_A_AVG");
//					memset(newData.obs_adr->sz_sts_name, sizeof(newData.obs_adr->sz_sts_name), 0);
//					SYSTEMTIME tt;
//					GetSystemTime(&tt);
//					CTime cTime(tt);
//					newData.ld_time_val = (long double)cTime.GetTime();
//					newData.obs_adr->d_data = 123.0;
//					data.push_back(newData);
//					size = 1;
// dummy data <<<

					if (size > 0)
					{
						//strDebug.Format(_T("%s(%d) :: CalcFromMsg size(%d)"), __FILEW__, __LINE__, (int)size);
						//CLogTraceEx::Write(_T(""), _T("CMainFrame"), _T("receiveServerServiceCallBack"), _T("debug"), strDebug, nLogEx::debug);
						// 衛星データキューに登録
						theApp.GetDataCollection().AddCollection(nStation, data);
						theApp.GetSatelliteData().GetDBAccessCls().SetEqMonData(nStation, data);
						bAddTrend = TRUE;

						// タイムライン用データの保存
						//theApp.SetMonitorDataList(data);
					}
					//else
					//{
					//	strDebug.Format(_T("%s(%d) :: CalcFromMsg no size(%d)"), __FILEW__, __LINE__, (int)size);
					//	CLogTraceEx::Write(_T(""), _T("CMainFrame"), _T("receiveServerServiceCallBack"), _T("debug"), strDebug, nLogEx::debug);
					//}
				}
				else
				{
					CString strDebug;
					strDebug.Format(_T("%s(%d) :: CalcFromMsg err(%d)"), __FILEW__, __LINE__, err);
					CLogTraceEx::Write(_T(""), _T("CMainFrame"), _T("receiveServerServiceCallBack"), _T("debug"), strDebug, nLogEx::error);
				}
			}
		}
		DWORD dwElapsed = (timeGetTime() - dwStart);
//		TRACE("### ePipeCommand_monitorget Elapsed(%d)\n", dwElapsed);
	}
	break;
	case	ePipeCommand_history:	// 履歴データ通知
		if (theApp.GetPipeIgnore() & 0x02)
			return;
		theApp.GetDataCollection().CreateDataToHistory(pbuf);
		::PostMessage(CMainFrame::GetPipeMessageWnd()->m_hWnd, eMessage_ThreadNormal, 0, 0);
		return;
		break;
	default:
		return;
		break;
	}



	//if (bAddTrend)
	//{
	//	// 衛星捕捉グラフ、追尾グラフデータ更新
	//	if (theApp.GetSelectStationMode() == eStation_1){
	//		if ((theApp.GetView(eViewType1_Trend))->GetSafeHwnd())				(theApp.GetView(eViewType1_Trend))->PostMessage(eMessage_ThreadNormal, 0, 0);
	//		if ((theApp.GetView(eViewType1_EquipmentStatus))->GetSafeHwnd())	(theApp.GetView(eViewType1_EquipmentStatus))->PostMessage(eMessage_ThreadNormal, 0, 0);
	//	}
	//	if (theApp.GetSelectStationMode() == eStation_5){
	//		if ((theApp.GetView(eViewType1_Trend))->GetSafeHwnd())			(theApp.GetView(eViewType1_Trend))->PostMessage(eMessage_ThreadNormal, 0, 0);
	//		if ((theApp.GetView(eViewType5_EqOperation1))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation1))->PostMessage(eMessage_ThreadNormal, 0, 0);
	//		if ((theApp.GetView(eViewType5_EqOperation2))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation2))->PostMessage(eMessage_ThreadNormal, 0, 0);
	//		if ((theApp.GetView(eViewType5_EqOperation3))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation3))->PostMessage(eMessage_ThreadNormal, 0, 0);
	//		if ((theApp.GetView(eViewType5_EqOperation4))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation4))->PostMessage(eMessage_ThreadNormal, 0, 0);
	//		if ((theApp.GetView(eViewType5_EqOperation5))->GetSafeHwnd())	(theApp.GetView(eViewType5_EqOperation5))->PostMessage(eMessage_ThreadNormal, 0, 0);
	//	}
	//}

	static DWORD mCallbackProcStart = 0;
	DWORD mDrawInterval = theApp.GetDrawUpdateInterval(false);
	DWORD dwElaps = timeGetTime() - mCallbackProcStart;
	if (dwElaps < mDrawInterval)
		return;
	mCallbackProcStart = timeGetTime();

	// 画面更新は対象局のみとする
	//::PostMessage(CMainFrame::GetPipeMessageWnd()->m_hWnd, ID_PIPE_SERVER, 9, (LPARAM)pbuf);
	if (theApp.GetSelectStationMode() == eStation_1){
		if (theApp.GetSelectStation() == nStation){
			::PostMessage(CMainFrame::GetPipeMessageWnd()->m_hWnd, eMessage_ThreadNormal, 0, 0);
		}
		else{
		}
	}
	else{
		::PostMessage(CMainFrame::GetPipeMessageWnd()->m_hWnd, eMessage_ThreadNormal, 0, 0);
	}
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
	::PostMessage(CMainFrame::GetPipeMessageWnd()->m_hWnd, ID_PIPE_RECEIVE, 9, (LPARAM)pbuf);
}
/*============================================================================*/
/*! プロセス間通信

-# プロセス間通信を行うためのパイプ作成

@param

@retval
*/
/*============================================================================*/
void CMainFrame::createPipe(UINT type)
{
	switch (type){
	case	ePIPETYPE_TIMELINE:
	{
		m_bCreatePipe[type] = false;
		mPipeSend[type].SetPipeName(mPipeName_TIMELINE + mPipeName_STATION);
		mPipeRecv[type].SetPipeName(mPipeName_TIMELINE);
		bool ret;
		ret = mPipeSend[type].Create(true, &receiveServerCallBack, type, this);
		if (ret == true){
			ret = mPipeRecv[type].Create(true, &receiveClientCallBack, type, this);
			if (ret == true){
				m_bCreatePipe[type] = true;
			}
			else{
				mPipeSend[type].Delete();
			}
		}
		mMemoryTimeLine.Create();
	}
	break;
	case	ePIPETYPE_STATION:
	{
		// メッセージ収集用スレッドの作成
		CreateMessageThread();
		// タイムライン用データ領域のクリア
		theApp.ClearMonitorDataList();
		// 監視、履歴データ取得用パイプの作成
// CHG 2017.10.16 >>>
//		m_bCreatePipeService = false;
//		mPipeServiceSend.SetPipeName(mPipeNameString[type]);
//		mPipeServiceRecv.SetPipeName(mPipeNameString[type]);
//		bool ret;
//		ret = mPipeServiceSend.CreateService(false, &receiveServerServiceCallBack, type, this);
//		if (ret == true){
//			m_bCreatePipeService = true;
//			SendServicePipe(eSTATIONSTART);
//		}
		for (int i = 0; i < eStation_MAX; i++){
			m_bCreatePipeService[i] = false;
			mPipeServiceSend[i].SetPipeName(mPipeNameString[type+i]);
			mPipeServiceRecv[i].SetPipeName(mPipeNameString[type+i]);
			bool ret;
			switch (i){
			case	0: ret = mPipeServiceSend[i].CreateService(false, &receiveServerServiceCallBack01, type, this); break;
			case	1: ret = mPipeServiceSend[i].CreateService(false, &receiveServerServiceCallBack02, type, this); break;
			case	2: ret = mPipeServiceSend[i].CreateService(false, &receiveServerServiceCallBack03, type, this); break;
			case	3: ret = mPipeServiceSend[i].CreateService(false, &receiveServerServiceCallBack04, type, this); break;
			}
			if (ret == true){
				m_bCreatePipeService[i] = true;
				SendServicePipe(eSTATIONSTART, i);
			}
		}
// CHG 2017.10.16 <<<
	}
	break;
	}
}

/*============================================================================*/
/*! プロセス間通信

-# プロセス間通信を行うためのパイプ削除

@param

@retval
*/
/*============================================================================*/
void CMainFrame::deletePipe(UINT type)
{
	switch (type){
	case	ePIPETYPE_TIMELINE:
	{
		if (m_bCreatePipe[type] == true){
			if (type == ePIPETYPE_TIMELINE){
				// タイムラインを終了させる
				sendPipe(ePIPETYPE_TIMELINE, eSTATIONEND);
			}
			// パイプの削除
			mPipeSend[type].Delete();
			mPipeRecv[type].Delete();
		}
		mMemoryTimeLine.Delete();
		m_bCreatePipe[type] = false;
		break;
	}
	case	ePIPETYPE_STATION:
	{
// CHG 2017.10.16 >>>	
//		if (m_bCreatePipeService == true){
//			SendServicePipe(eSTATIONEND);
//			// パイプの削除
//			mPipeServiceSend.Delete();
//			mPipeServiceRecv.Delete();
//			for (int i = 0; i < eStation_MAX; i++)
//			{
//				if (CMainFrame::GetEngval(i) != NULL){
//					CMainFrame::DeleteEngval(i);
//				}
//			}
//		}
//		m_bCreatePipeService = false;
		for (int i = 0; i < eStation_MAX; i++){
			if (m_bCreatePipeService[i] == true){
				SendServicePipe(i, eSTATIONEND);
				// パイプの削除
				mPipeServiceSend[i].Delete();
				mPipeServiceRecv[i].Delete();
				if (CMainFrame::GetEngval(i) != NULL){
					CMainFrame::DeleteEngval(i);
				}
			}
			m_bCreatePipeService[i] = false;
		}
// CHG 2017.10.16 <<<
		break;
	}
	}
}

/*============================================================================*/
/*! プロセス間通信

-# プロセス間通信パイプにコマンドを送る

@param

@retval
*/
/*============================================================================*/
void CMainFrame::sendPipe(UINT type, DWORD command, char* pbuf/* = NULL*/, UINT size/* = 0*/)
{
	switch (type){
	case	ePIPETYPE_TIMELINE:
	{
		//stHistoryData data;
		if (m_bCreatePipe[type] == true){
			if (type == ePIPETYPE_TIMELINE){
				PIPEBUFFER buf;
				memset(&buf, 0, sizeof(PIPEBUFFER));
				buf.command = mCommand;
				buf.commandIndex = command;
				buf.bufsize = 0;
				if (pbuf != NULL){
					if (mPipeBufferSize < size){
						//=====================================================//
						//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
						CString msg;
						msg.Format(_T("eMONITORDATA : size=%d"), size);
						CLogTraceEx::Write(_T(""), _T("CMainFrame"), _T("sendPipe"), msg, _T("Error"), nLogEx::debug);
						//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
						//=====================================================//
					}
					buf.bufsize = size;
					memcpy(buf.buf, pbuf, size);
					//memcpy(&data, buf.buf, sizeof(stHistoryData));
				}
				if (command == eMONITORDATA){
					//=====================================================//
					//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
					CString msg;
					msg.Format(_T("eMONITORDATA : size=%d"), size);
					CLogTraceEx::Write(_T(""), _T("CMainFrame"), _T("sendPipe"), msg, _T(""), nLogEx::debug);
					//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
					//=====================================================//
				}
				mPipeSend[type].Send(&buf);
			}
		}
		break;
	}
	case	ePIPETYPE_STATION:
	{
// CHG 2017.10.16 >>>
//		if (m_bCreatePipeService == true){
//			SendServicePipe(command);
//		}
		for (int i = 0; i < eStation_MAX; i++){
			if (m_bCreatePipeService[i] == true){
				SendServicePipe(command, i);
			}
		}
// CHG 2017.10.16 <<<
		break;
	}
	}
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
		buf.val[0] = ((BYTE)ip&0xff);
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

-# 履歴情報をサブ画面に送る
メッセージIDの区分は以下のようになります。
0 ～ 29999 運用履歴
30000 ～ 39999 状態遷移履歴
40000 ～ 49999 リミットエラー履歴
50000 ～ 59999 制御履歴
60000 ～ 69999 イベント履歴

@param

@retval
*/
/*============================================================================*/
void CMainFrame::SendPipeHistory()
{
	CGraphMutex::Lock(eData);
	deque<stHistoryData>& data = theApp.GetDataCollection().GetSubEquipmentHistoryData();
	while (!data.empty())
	{
		stHistoryData newData = data.front();
		data.pop_front();
		//sendPipe(ePIPETYPE_TIMELINE, eHISTORYEQ, (char*)&newData, sizeof(stHistoryData));
		TRACE("## EQ : ST=%d ID=%d\n", newData.station, newData.message);
	}

	deque<stHistoryData>& datae = theApp.GetDataCollection().GetSubEventHistoryData();
	while (!datae.empty())
	{
		stHistoryData newData = datae.front();
		datae.pop_front();
		//sendPipe(ePIPETYPE_TIMELINE, eHISTORYEVENT, (char*)&newData, sizeof(stHistoryData));
		TRACE("## EVENT : ST=%d ID=%d\n", newData.station, newData.message);
	}
	CGraphMutex::Unlock(eData);
}
/*============================================================================*/
/*! プロセス間通信

-# プロセス間通信メッセージを処理する

@param		message		メッセージID

@retval
*/
/*============================================================================*/
void CMainFrame::messagePipe(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	ID_PIPE_RECEIVE:
	{
		LPPIPEBUFFER pbuf = (LPPIPEBUFFER)lParam;
		switch (pbuf->commandIndex){
		case	eTIMELINESTART:
			TRACE("*** TimeLine Start\n");
			sendPipe(ePIPETYPE_TIMELINE, eHELLO);
			break;
		case	eTIMELINEEND:
			TRACE("*** TimeLine End\n");
			deletePipe(ePIPETYPE_TIMELINE);
			createPipe(ePIPETYPE_TIMELINE);
			break;
		case	eTIMELINE_FORECASTPLAN:
			// 運用計画一覧画面呼び出し
			theApp.SetTimeLineCall(true);
			PostMessage(WM_COMMAND, ID_MP_PLANLIST, 0);
			break;
		case	eTIMELINE_FORECAST:
			// 予報値一覧画面呼び出し
			theApp.SetTimeLineCall(true);
			PostMessage(WM_COMMAND, ID_MP_FORECASTLIST, 0);
			break;
		case	eTIMELINE_OPEPLAN:
			// 運用計画立案画面呼び出し
			theApp.SetTimeLineCall(true);
			PostMessage(WM_COMMAND, ID_MS_PLANNING, 0);
			break;
		case	eTIMELINE_PLANDELETE:
			// 計画削除確認画面呼び出し
			//theApp.SetTimeLineCall(true);
			//PostMessage(WM_COMMAND, ID_MS_PLANNING, 0);
			break;
		case	eTIMELINE_PLANDETAIL:
			// 計画詳細画面呼び出し
			//theApp.SetTimeLineCall(true);
			//PostMessage(WM_COMMAND, ID_MS_PLANNING, 0);
			break;
		case	eTIMELINE_PLANSTARTTIME:
			// 計画開始時刻変更画面呼び出し
			theApp.SetTimeLineCall(true);
			PostMessage(WM_COMMAND, ID_OP_STARTTIME_CHG, 0);
			break;
		case	eTIMELINE_PLANSTOPTIME:
			// 計画終了時刻変更画面呼び出し
			theApp.SetTimeLineCall(true);
			PostMessage(WM_COMMAND, ID_OP_ENDTIME_CHG, 0);
			break;
		case	eTIMELINE_PLANEMGSTOP:
			// 強制運用停止確認画面呼び出し
			theApp.SetTimeLineCall(true);
			PostMessage(WM_COMMAND, ID_FORCED_OP_STOP, 0);
			break;
		case	eTIMELINE_TIMEOFFSET:
			// 時刻オフセット画面呼び出し
			theApp.SetTimeLineCall(true);
			PostMessage(WM_COMMAND, ID_TIMEOFFSET_SET, 0);
			break;
		case	eTIMELINE_CALIBRATION:
			// 校正診断画面呼び出し
			theApp.SetTimeLineCall(true);
			PostMessage(WM_COMMAND, ID_MC_CALIBDIAGNS, 0);
			break;
		case	eTIMELINE_EQUIPMENTOCC:
			// 局設備占有時間入力画面呼び出し
			//theApp.SetTimeLineCall(true);
			//PostMessage(WM_COMMAND, ID_MC_CALIBDIAGNS, 0);
			break;
		case	eTIMELINE_VLBI:
			// VLBI運用入力画面呼び出し
			//theApp.SetTimeLineCall(true);
			//PostMessage(WM_COMMAND, ID_MC_CALIBDIAGNS, 0);
			break;
		case	eANTEMG:
			Emergency(eANTEMG);
			break;
		case	eSENDSTOP:
			Emergency(eSENDSTOP);
			break;

		case	eALARMSTOP:
		{
			// タイムライン側から通知
			CMFCCaptionBarEx& cap = GetWndCaption();
			CMFCButtonEx& btn = cap.GetSoundBtnCtrl();
			btn.ExecuteClicked(true, 0);
			break;
		}
		case	eALARMMUTEOFF:
		{
			// タイムライン側から通知
			CMFCCaptionBarEx& cap = GetWndCaption();
			CMFCButtonEx& btn = cap.GetMuteBtnCtrl();
			btn.ExecuteClicked(true, 0);
			break;
		}
		case	eALARMMUTEON:
		{
			// タイムライン側から通知
			CMFCCaptionBarEx& cap = GetWndCaption();
			CMFCButtonEx& btn = cap.GetMuteBtnCtrl();
			btn.ExecuteClicked(true, 1);
			break;
		}

		}
		break;
	}
	case	ID_PIPE_FINISH:
		TRACE("*** %d\n", lParam);
		if (lParam == ePIPETYPE_STATION){
			for (int i = 0; i < eStation_MAX; i++)
			{
				if (CMainFrame::GetEngval(i) != NULL){
					CMainFrame::DeleteEngval(i);
				}
			}
		}
		deletePipe((UINT)lParam);
		createPipe((UINT)lParam);
		break;

	}
}

/*============================================================================*/
/*! アプリケーション

-# 緊急停止

@brief	緊急停止

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::Emergency(UINT type)
{
	if (type == eANTEMG){
		if (MessageBoxHooked(this->m_hWnd, _T("アンテナの駆動系をDRIVE OFFにします。\nよろしいですか？"), _T(""), MB_ICONQUESTION | MB_OKCANCEL) == IDCANCEL)
			return;
	}
	else{
		if (MessageBoxHooked(this->m_hWnd, _T("送信機のRF OUTPUTを全てOFFにします。\nよろしいですか？"), _T(""), MB_ICONQUESTION | MB_OKCANCEL) == IDCANCEL)
			return;
	}
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = (type == eANTEMG) ? EMERGENCY_ANT_SH : EMERGENCY_TX_SH;
	if (theApp.GetSelectStation() == eStation_Usuda64)
		stParam.strRemShellParam.Format(_T("udsc64"));
	else if (theApp.GetSelectStation() == eStation_Usuda642)
		stParam.strRemShellParam.Format(_T("udsc54"));
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
		stParam.strRemShellParam.Format(_T("usc34"));
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
		stParam.strRemShellParam.Format(_T("usc20"));
	else
		stParam.strRemShellParam.Empty();

	CString msg;
	msg.Format(_T("Emergency Stop(%s)!!!"), (LPCTSTR)stParam.strRemShellName);
	CLogTraceEx::Write(_T(""), _T("CMainFrame"), _T("Emergency"), msg, _T(""), nLogEx::info);

	int err = CSshShell::ExecShell(stParam);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CMainFrame"), _T("Emergency"), _T("debug"), strDebug, nLogEx::debug);
	}
}


void CMainFrame::OnDestroy()
{
	// メッセージ収集用スレッドの終了
	RemoveMessageThread();

	// 監視データを通知するためのスレッドを終了する
	DeleteTimelineThread();

	for (UINT i = 0; i<ePIPETYPE_MAX; i++){
		deletePipe(i);
	}
	deletePipe(ePIPETYPE_STATION);

	CMDIFrameWndEx::OnDestroy();
}


//-----------------------------------------------------------------------------------
// メニュー呼出

/*============================================================================*/
/*! アプリケーション

-# 『グラフ』-『AGCグラフ』メニュー

@brief	AGCグラフ(衛星捕捉グラフ)画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMgAgcGraph()
{
	CTrendGraphView* pView = (CTrendGraphView*)theApp.GetView(eViewType1_Trend);
	if (pView == NULL)
	{
		theApp.OpenView(eViewType1_Trend);
		SetAGCGraphMinSize();
		PostMessage(eMessage_InitTrendWnd, eTrendWndSetType_AGC, 0);
	}
	else if (pView->GetWndTypeSet() == eTrendWndSetType_AGC)
	{
//		theApp.CloseView(eViewType1_Trend);
		CView *view = theApp.GetView(eViewType1_Trend);
		((CMDIChildWnd*)view->GetParent())->MDIActivate();
	}
	else
	{
		//theApp.CloseView(eViewType1_Trend);
		//theApp.OpenView(eViewType1_Trend);
		SetAGCGraphMinSize();
		PostMessage(eMessage_InitTrendWnd, eTrendWndSetType_AGC, 0);
	}
}

void CMainFrame::OnUpdateMgAgcGraph(CCmdUI *pCmdUI)
{
	UINT nCheck = 0;
	CTrendGraphView* pView = (CTrendGraphView*)theApp.GetView(eViewType1_Trend);
	if (pView != NULL)
	{
		if (pView->GetWndTypeSet() == eTrendWndSetType_AGC)
		{
			nCheck = 1;
		}
	}
	pCmdUI->SetCheck(nCheck);
}

/*============================================================================*/
/*! アプリケーション

-# 『グラフ』-『RNG、RRグラフ』メニュー

@brief	RNG、RRグラフ(衛星捕捉グラフ)画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMgRngRRGraph()
{
	CTrendGraphView* pView = (CTrendGraphView*)theApp.GetView(eViewType1_Trend);
	if (pView == NULL)
	{
		theApp.OpenView(eViewType1_Trend);
		SetRNGRRGraphMinSize();
		PostMessage(eMessage_InitTrendWnd, eTrendWndSetType_RNG_RR, 0);
	}
	else if (pView->GetWndTypeSet() == eTrendWndSetType_RNG_RR)
	{
//		theApp.CloseView(eViewType1_Trend);
		CView *view = theApp.GetView(eViewType1_Trend);
		((CMDIChildWnd*)view->GetParent())->MDIActivate();
	}
	else
	{
		//theApp.CloseView(eViewType1_Trend);
		//theApp.OpenView(eViewType1_Trend);
		SetRNGRRGraphMinSize();
		PostMessage(eMessage_InitTrendWnd, eTrendWndSetType_RNG_RR, 0);
	}
}

void CMainFrame::OnUpdateMgRngRRGraph(CCmdUI *pCmdUI)
{
	UINT nCheck = 0;
	CTrendGraphView* pView = (CTrendGraphView*)theApp.GetView(eViewType1_Trend);
	if (pView != NULL)
	{
		if (pView->GetWndTypeSet() == eTrendWndSetType_RNG_RR)
		{
			nCheck = 1;
		}
	}
	pCmdUI->SetCheck(nCheck);
}

/*============================================================================*/
/*! アプリケーション

-# 『グラフ』-『AGC、RNG、ELグラフ』メニュー

@brief	AGC、RNG、ELグラフ(衛星捕捉グラフ)画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMgAGCRNGELGraph()
{
	CTrendGraphView* pView = (CTrendGraphView*)theApp.GetView(eViewType1_Trend);
	if (pView == NULL)
	{
		theApp.OpenView(eViewType1_Trend);
		SetAGCRNGELGraphMinSize();
		PostMessage(eMessage_InitTrendWnd, eTrendWndSetType_AGC_RNG_EL, 0);
	}
	else if (pView->GetWndTypeSet() == eTrendWndSetType_AGC_RNG_EL)
	{
//		theApp.CloseView(eViewType1_Trend);
		CView *view = theApp.GetView(eViewType1_Trend);
		((CMDIChildWnd*)view->GetParent())->MDIActivate();
	}
	else
	{
		//theApp.CloseView(eViewType1_Trend);
		//theApp.OpenView(eViewType1_Trend);
		SetAGCRNGELGraphMinSize();
		PostMessage(eMessage_InitTrendWnd, eTrendWndSetType_AGC_RNG_EL, 0);
	}
}

void CMainFrame::OnUpdateMgAGCRNGELGraph(CCmdUI *pCmdUI)
{
	UINT nCheck = 0;
	CTrendGraphView* pView = (CTrendGraphView*)theApp.GetView(eViewType1_Trend);
	if (pView != NULL)
	{
		if (pView->GetWndTypeSet() == eTrendWndSetType_AGC_RNG_EL)
		{
			nCheck = 1;
		}
	}
	pCmdUI->SetCheck(nCheck);
}

/*============================================================================*/
/*! アプリケーション

-# 『グラフ』-『グラフ(Trend)』メニュー

@brief	トレンドグラフを表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMgTrendGraph()
{
	CString	strExePath, strParam;

	strExePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppPath(), (LPCTSTR)mTrendGraphApp);
	//strParam.Format(_T("/TITLE:%s-%s /XML:%s%s\\%s.xml"), strStation, fname, theApp.GetAppFaPanelPath(), mStationFolder[mstno], fname);

	// 汎用グラフに引き継ぎ情報を渡す
//	strParam.Empty();
	strParam.Format(_T("/MODE:%d /STATION:%d /SERVERNAME:%s"), theApp.GetSelectStationMode() + 1, theApp.GetSelectStation() + 1, (LPCTSTR)theApp.GetFileServerName());

	::ShellExecute(m_hWnd, _T(""), strExePath, strParam, NULL, SW_SHOWMAXIMIZED);
}

void CMainFrame::OnUpdateMgTrendGraph(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


/*============================================================================*/
/*! アプリケーション

-# 『グラフ』-『スペクトラムグラフ』メニュー

@brief	スペクトラム表示画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMgSpectrumGraph()
{
	CSpectrumView* pDlg = (CSpectrumView*)theApp.GetWindowHandle(IDD_DLG_SPECTRUMVIEW);
	if (pDlg != NULL)
	{
		pDlg->ShowWindow(SW_SHOW);
		return;
	}

	pDlg = new CSpectrumView();
	if (pDlg != NULL)
	{
		if (pDlg->Create(IDD_DLG_SPECTRUMVIEW, this))
		{
			pDlg->ShowWindow(SW_SHOW);
			theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_SPECTRUMVIEW);
		}
	}
}

void CMainFrame::OnUpdateMgSpectrumGraph(CCmdUI *pCmdUI)
{
	CWnd* pWnd = theApp.GetWindowHandle(IDD_DLG_SPECTRUMVIEW);
	if (pWnd == NULL)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
	}
}


/*============================================================================*/
/*! アプリケーション

-# 『グラフ』-『RNG伝搬モニタ起動』メニュー

@brief	レンジ伝搬状況モニタ画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMgRngMonitor()
{
	CRangePropagationStatusMonitor* pDlg = (CRangePropagationStatusMonitor*)theApp.GetWindowHandle(IDD_DLG_RANGEPROPAGATIONSTATUSMONITOR);
	if (pDlg != NULL)
	{
		pDlg->ShowWindow(SW_SHOW);
		return;
	}

	pDlg = new CRangePropagationStatusMonitor();
	if (pDlg != NULL)
	{
		if (pDlg->Create(IDD_DLG_RANGEPROPAGATIONSTATUSMONITOR, this))
		{
			pDlg->ShowWindow(SW_SHOW);
			theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_RANGEPROPAGATIONSTATUSMONITOR);
		}
	}
}

void CMainFrame::OnUpdateMgRngMonitor(CCmdUI *pCmdUI)
{
	CWnd* pWnd = theApp.GetWindowHandle(IDD_DLG_RANGEPROPAGATIONSTATUSMONITOR);
	if (pWnd == NULL)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
	}
}


/*============================================================================*/
/*! アプリケーション

-# 『運用制御』-『マニュアル初期設定』メニュー

@brief	初期設定画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMoManualinitialSetting()
{
	CInitSetting dlg;
	if (dlg.DoModal() == IDCLOSE)
	{
		return;
	}
}

void CMainFrame::OnUpdateMoManualinitialSetting(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


/*============================================================================*/
/*! アプリケーション

-# 『運用制御』-『送信制御』メニュー

@brief	送信制御画面選択を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMoTransmissionCtrl()
{
	CTrsControlMng tcm;
	tcm.DisplayTrsControl();
}

void CMainFrame::OnUpdateMoTransmissionCtrl(CCmdUI *pCmdUI)
{
	CWnd* pWnd;
	pWnd = theApp.GetWindowHandle(IDD_DLG_TRSCTRL_X);
	if (pWnd != NULL)
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
		return;
	}
	pWnd = theApp.GetWindowHandle(IDD_DLG_TRSCTRL_XX);
	if (pWnd != NULL)
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
		return;
	}
	pWnd = theApp.GetWindowHandle(IDD_DLG_TRSCTRL_XX54);
	if (pWnd != NULL)
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
		return;
	}
	pWnd = theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S64);
	if (pWnd != NULL)
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
		return;
	}
	pWnd = theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S64);
	if (pWnd != NULL)
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
		return;
	}
	pWnd = theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S34);
	if (pWnd != NULL)
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
		return;
	}
	pWnd = theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S20);
	if (pWnd != NULL)
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
		return;
	}

	pCmdUI->SetCheck(0);
}


/*============================================================================*/
/*! アプリケーション

-# 『運用制御』-『逐次制御』メニュー

@brief	逐次制御画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMoSequentialCtrl()
{
	CSequentialCtrlRegister* pDlg = (CSequentialCtrlRegister*)theApp.GetWindowHandle(IDD_DLG_SEQUENTIALCTRLREGISTER);
	if (pDlg != NULL){
		pDlg->ShowWindow(SW_SHOW);
		return;
	}

	pDlg = new CSequentialCtrlRegister();
	if (pDlg != NULL)
	{
		if (pDlg->Create(IDD_DLG_SEQUENTIALCTRLREGISTER, this))
		{
			pDlg->ShowWindow(SW_SHOW);
			theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_SEQUENTIALCTRLREGISTER);
		}
	}
}

void CMainFrame::OnUpdateMoSequentialCtrl(CCmdUI *pCmdUI)
{
	CWnd* pWnd = theApp.GetWindowHandle(IDD_DLG_SEQUENTIALCTRLREGISTER);
	if (pWnd == NULL)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
	}
}


/*============================================================================*/
/*! アプリケーション

-# 『運用制御』-『設備状態』メニュー

@brief	設備状態画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMoEquipstatus()
{
	if (theApp.GetView(eViewType1_EquipmentStatus) == NULL)
		theApp.OpenView(eViewType1_EquipmentStatus);
	else
	{
//		theApp.CloseView(eViewType1_EquipmentStatus);
		CView *view = theApp.GetView(eViewType1_EquipmentStatus);
		((CMDIChildWnd*)view->GetParent())->MDIActivate();
	}
}


void CMainFrame::OnUpdateMoEquipstatus(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetView(eViewType1_EquipmentStatus) != NULL) ? 1 : 0);
}


/*============================================================================*/
/*! アプリケーション

-# 『予報値/計画』-『計画一覧』メニュー

@brief	予報値一覧・運用計画画面を起動し『運用計画』タブを表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMpPlanningList()
{
	CForecastList* pDlg = (CForecastList*)theApp.GetWindowHandle(IDD_DLG_FORECASTLIST);
	if (pDlg != NULL)
	{
		pDlg->InitCombobox();
		pDlg->SetUseSelectBtn(FALSE);
		pDlg->SelectTabAndInitCombo(eForecastListType_Operation);
		pDlg->ShowWindow(SW_SHOW);
		return;
	}

	pDlg = new CForecastList();
	if (pDlg != NULL)
	{
		if (pDlg->Create(IDD_DLG_FORECASTLIST, this))
		{
			pDlg->SetUseSelectBtn(FALSE);
			pDlg->SelectTabAndInitCombo(eForecastListType_Operation);
			pDlg->ShowWindow(SW_SHOW);
			theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_FORECASTLIST);
		}
	}
}

void CMainFrame::OnUpdateMpPlanningList(CCmdUI *pCmdUI)
{
	CWnd* pWnd = theApp.GetWindowHandle(IDD_DLG_FORECASTLIST);
	if (pWnd == NULL)
	{
		pCmdUI->SetCheck(0);
	}
	else if (pWnd->IsWindowVisible())
	{
		pCmdUI->SetCheck((((CForecastList*)pWnd)->IsOperationList()) ? 1 : 0);
	}
}


/*============================================================================*/
/*! アプリケーション

-# 『予報値/計画』-『予報値一覧』メニュー

@brief	予報値一覧・運用計画画面を起動し『予報値』タブを表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMpForecastList()
{
	CForecastList* pDlg = (CForecastList*)theApp.GetWindowHandle(IDD_DLG_FORECASTLIST);
	if (pDlg != NULL)
	{
		pDlg->InitCombobox();
		pDlg->SetUseSelectBtn(FALSE);
		pDlg->SelectTabAndInitCombo(eForecastListType_Forecast);
		pDlg->ShowWindow(SW_SHOW);
		return;
	}

	pDlg = new CForecastList();
	if (pDlg != NULL)
	{
		if (pDlg->Create(IDD_DLG_FORECASTLIST, this))
		{
			pDlg->SetUseSelectBtn(FALSE);
			pDlg->SelectTabAndInitCombo(eForecastListType_Forecast);
			pDlg->ShowWindow(SW_SHOW);
			theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_FORECASTLIST);
		}
	}
}

void CMainFrame::OnUpdateMpForecastList(CCmdUI *pCmdUI)
{
	CWnd* pWnd = theApp.GetWindowHandle(IDD_DLG_FORECASTLIST);
	if (pWnd == NULL)
	{
		pCmdUI->SetCheck(0);
	}
	else if (pWnd->IsWindowVisible())
	{
		pCmdUI->SetCheck((((CForecastList*)pWnd)->IsForecastList()) ? 1 : 0);
	}
}

/*============================================================================*/
/*! アプリケーション

-# 『予報値/計画』-『予報値一覧』メニュー

@brief	選択ボタンモードの予報値一覧・運用計画画面を起動し『予報値』タブを表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMpForecastListSelBtn()
{
	CForecastList* pDlg = (CForecastList*)theApp.GetWindowHandle(IDD_DLG_FORECASTLIST);
	if (pDlg != NULL)
	{
		pDlg->InitCombobox();
		pDlg->SetUseSelectBtn(TRUE);
		pDlg->SelectTabAndInitCombo(eForecastListType_Forecast);
		pDlg->ShowWindow(SW_SHOW);
		return;
	}

	pDlg = new CForecastList();
	if (pDlg != NULL)
	{
		if (pDlg->Create(IDD_DLG_FORECASTLIST, this))
		{
			pDlg->SetUseSelectBtn(TRUE);
			pDlg->SelectTabAndInitCombo(eForecastListType_Forecast);
			pDlg->ShowWindow(SW_SHOW);
			theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_FORECASTLIST);
		}
	}
}

/*============================================================================*/
/*! アプリケーション

-# 『予報値/計画』-『運用計画タイムライン』メニュー

@brief	運用計画タイムライン画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMpTimeline()
{
	if (theApp.GetView(eViewTypeTool_TimeLine) == NULL)
		theApp.OpenView(eViewTypeTool_TimeLine);
	else
	{
//		theApp.CloseView(eViewTypeTool_TimeLine);
		CView *view = theApp.GetView(eViewTypeTool_TimeLine);
		((CMDIChildWnd*)view->GetParent())->MDIActivate();
	}
}


void CMainFrame::OnUpdateMpTimeline(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetView(eViewTypeTool_TimeLine) != NULL) ? 1 : 0);
}


/*============================================================================*/
/*! アプリケーション

-# 『業務日誌』-『無線業務日誌一覧』メニュー

@brief	無線業務日誌画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMdDiary()
{
	CBusinessDiary dlg;
	dlg.DoModal();
	//CBusinessDiary* pDlg = (CBusinessDiary*)theApp.GetWindowHandle(IDD_DLG_DIARY);
	//if (pDlg != NULL){
	//	pDlg->ShowWindow(SW_SHOW);
	//	return;
	//}

	//pDlg = new CBusinessDiary();
	//if (pDlg != NULL)
	//{
	//	if (pDlg->Create(IDD_DLG_DIARY, this))
	//	{
	//		pDlg->ShowWindow(SW_SHOW);
	//		theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_DIARY);
	//	}
	//}
}

void CMainFrame::OnUpdateMdDiary(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


/*============================================================================*/
/*! アプリケーション

-# 『校正診断』-『校正診断』メニュー

@brief	校正診断画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMcCalibDiagns()
{
	CString satellite, planid;
	LPARAM val3 = 0;
	UINT val4 = 0;
	CTime val5 = 0;
	satellite = _T("");
	planid = _T("");

	UINT sta = theApp.GetSelectStation();
	UINT bNew = 1;
	CTime startTime = 0;

	// タイムラインからの呼出チェック
	bool bTimeLine = theApp.GetTimeLineCall();
	if (bTimeLine)
	{
		theApp.GetTimeLinePlan(satellite, planid, val3, val4, val5);
		theApp.SetTimeLineCall(false);
		startTime = val5;
	}

	bNew = (val4 & 0x02) ? 0 : 1;

	if (sta == eStation_Usuda64)
	{
		// 64m
		CRdnsChk64* pDlg64 = (CRdnsChk64*)theApp.GetWindowHandle(IDD_DLG_RDNSCHK64);
		if (pDlg64 != NULL){

			if (bTimeLine == true &&  pDlg64->IsWindowVisible())
			{
				AfxMessageBox(_T("校正診断画面が起動しています。\n画面を閉じてから起動してください。"), MB_OK | MB_ICONEXCLAMATION);
				return;
			}

			pDlg64->SetSatelliteName(satellite);
			pDlg64->SetType(bNew);
			pDlg64->SetGetForm(DWORD(val3));
			pDlg64->SetPassId(planid);
			pDlg64->SetStartTime(startTime);
			pDlg64->UpdateRdnsChkDlg();
//			if (m_nLastSizeType != SIZE_MINIMIZED)
//			{
//				pDlg64->UpdateRdnsChkDlg();
//			}
			pDlg64->ShowWindow(SW_SHOW);
		}
		else
		{
			pDlg64 = new CRdnsChk64();
			if (pDlg64 != NULL)
			{
				if (pDlg64->Create(IDD_DLG_RDNSCHK64, this))
				{
					pDlg64->SetSatelliteName(satellite);
					pDlg64->SetType(bNew);
					pDlg64->SetGetForm(DWORD(val3));
					pDlg64->SetPassId(planid);
					pDlg64->SetStartTime(startTime);
					pDlg64->UpdateRdnsChkDlg();
//					if (m_nLastSizeType != SIZE_MINIMIZED)
//					{
//						pDlg64->UpdateRdnsChkDlg();
//					}
					pDlg64->ShowWindow(SW_SHOW);
					theApp.AddWindowHandle((LPARAM)pDlg64, IDD_DLG_RDNSCHK64);
				}
			}
		}
	}
	else if (sta == eStation_Usuda642)
	{
		// 54m
		CRdnsChk54* pDlg54 = (CRdnsChk54*)theApp.GetWindowHandle(IDD_DLG_RDNSCHK54);
		if (pDlg54 != NULL){

			if (bTimeLine == true && pDlg54->IsWindowVisible())
			{
				AfxMessageBox(_T("校正診断画面が起動しています。\n画面を閉じてから起動してください。"), MB_OK | MB_ICONEXCLAMATION);
				return;
			}

			pDlg54->SetType(bNew);
			pDlg54->SetGetForm(DWORD(val3));
			pDlg54->SetPassId(planid);
			pDlg54->SetSatelliteName(satellite);
			pDlg54->SetStartTime(startTime);
			pDlg54->UpdateRdnsChkDlg();
//			if (m_nLastSizeType != SIZE_MINIMIZED)
//			{
//				pDlg54->UpdateRdnsChkDlg();
//			}
			pDlg54->ShowWindow(SW_SHOW);
		}
		else
		{
			pDlg54 = new CRdnsChk54();
			if (pDlg54 != NULL)
			{
				if (pDlg54->Create(IDD_DLG_RDNSCHK54, this))
				{
					pDlg54->SetSatelliteName(satellite);
					pDlg54->SetType(bNew);
					pDlg54->SetGetForm(DWORD(val3));
					pDlg54->SetPassId(planid);
					pDlg54->SetStartTime(startTime);
					pDlg54->UpdateRdnsChkDlg();
//					if (m_nLastSizeType != SIZE_MINIMIZED)
//					{
//						pDlg54->UpdateRdnsChkDlg();
//					}
					pDlg54->ShowWindow(SW_SHOW);
					theApp.AddWindowHandle((LPARAM)pDlg54, IDD_DLG_RDNSCHK54);
				}
			}
		}
	}
	else if (sta == eStation_Uchinoura34)
	{
		// 34m
		CRdnsChk34* pDlg34 = (CRdnsChk34*)theApp.GetWindowHandle(IDD_DLG_RDNSCHK34);
		if (pDlg34 != NULL){

			if (bTimeLine == true && pDlg34->IsWindowVisible())
			{
				AfxMessageBox(_T("校正診断画面が起動しています。\n画面を閉じてから起動してください。"), MB_OK | MB_ICONEXCLAMATION);
				return;
			}

			pDlg34->SetSatelliteName(satellite);
			pDlg34->SetType(bNew);
			pDlg34->SetGetForm(DWORD(val3));
			pDlg34->SetPassId(planid);
			pDlg34->SetStartTime(startTime);
			pDlg34->UpdateRdnsChkDlg();
//			if (m_nLastSizeType != SIZE_MINIMIZED)
//			{
//				pDlg34->UpdateRdnsChkDlg();
//			}
			pDlg34->ShowWindow(SW_SHOW);
		}
		else
		{
			pDlg34 = new CRdnsChk34();
			if (pDlg34 != NULL)
			{
				if (pDlg34->Create(IDD_DLG_RDNSCHK34, this))
				{
					pDlg34->SetSatelliteName(satellite);
					pDlg34->SetType(bNew);
					pDlg34->SetGetForm(DWORD(val3));
					pDlg34->SetPassId(planid);
					pDlg34->SetStartTime(startTime);
					pDlg34->UpdateRdnsChkDlg();
//					if (m_nLastSizeType != SIZE_MINIMIZED)
//					{
//						pDlg34->UpdateRdnsChkDlg();
//					}
					pDlg34->ShowWindow(SW_SHOW);
					theApp.AddWindowHandle((LPARAM)pDlg34, IDD_DLG_RDNSCHK34);
				}
			}
		}
	}
	else if (sta == eStation_Uchinoura20)
	{
		// 20m
		CRdnsChk20* pDlg20 = (CRdnsChk20*)theApp.GetWindowHandle(IDD_DLG_RDNSCHK20);
		if (pDlg20 != NULL){

			if (bTimeLine == true && pDlg20->IsWindowVisible())
			{
				AfxMessageBox(_T("校正診断画面が起動しています。\n画面を閉じてから起動してください。"), MB_OK | MB_ICONEXCLAMATION);
				return;
			}

			pDlg20->SetSatelliteName(satellite);
			pDlg20->SetType(bNew);
			pDlg20->SetGetForm(DWORD(val3));
			pDlg20->SetPassId(planid);
			pDlg20->SetStartTime(startTime);
			pDlg20->UpdateRdnsChkDlg();
//			if (m_nLastSizeType != SIZE_MINIMIZED)
//			{
//				pDlg20->UpdateRdnsChkDlg();
//			}
			pDlg20->ShowWindow(SW_SHOW);
		}
		else
		{
			pDlg20 = new CRdnsChk20();
			if (pDlg20 != NULL)
			{
				if (pDlg20->Create(IDD_DLG_RDNSCHK20, this))
				{
					pDlg20->SetSatelliteName(satellite);
					pDlg20->SetType(bNew);
					pDlg20->SetGetForm(DWORD(val3));
					pDlg20->SetPassId(planid);
					pDlg20->SetStartTime(startTime);
					pDlg20->UpdateRdnsChkDlg();
//					if (m_nLastSizeType != SIZE_MINIMIZED)
//					{
//						pDlg20->UpdateRdnsChkDlg();
//					}
					pDlg20->ShowWindow(SW_SHOW);
					theApp.AddWindowHandle((LPARAM)pDlg20, IDD_DLG_RDNSCHK20);
				}
			}
		}
	}
}

void CMainFrame::OnUpdateMcCalibDiagns(CCmdUI *pCmdUI)
{
	CWnd* pWnd;
	pWnd = theApp.GetWindowHandle(IDD_DLG_RDNSCHK64);
	if (pWnd != NULL)
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
		return;
	}
	pWnd = theApp.GetWindowHandle(IDD_DLG_RDNSCHK54);
	if (pWnd != NULL)
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
		return;
	}
	pWnd = theApp.GetWindowHandle(IDD_DLG_RDNSCHK34);
	if (pWnd != NULL)
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
		return;
	}
	pWnd = theApp.GetWindowHandle(IDD_DLG_RDNSCHK20);
	if (pWnd != NULL)
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
		return;
	}
}


/*============================================================================*/
/*! アプリケーション

-# 『設備記録』-『設備状態遷移出力』メニュー

@brief	設備状態遷移出力画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMeEqipStatusOut()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateMeEqipStatusOut(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}

/*============================================================================*/
/*! アプリケーション

-# 『設備記録』-『監視データダンプ出力』-『監視データダンプ出力』メニュー

@brief	監視データダンプ出力画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMeMntrDataDmpOutput()
{
	CEquipmentRecDumpOutput* pDlg = (CEquipmentRecDumpOutput*)theApp.GetWindowHandle(IDD_DLG_EQIPREC_DUMPOUTPUT);
	if (pDlg != NULL){
		pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		pDlg = new CEquipmentRecDumpOutput();
		if (pDlg != NULL)
		{
			if (pDlg->Create(IDD_DLG_EQIPREC_DUMPOUTPUT, this))
			{
				pDlg->ShowWindow(SW_SHOW);
				theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_EQIPREC_DUMPOUTPUT);
			}
		}
	}
}

void CMainFrame::OnUpdateMeMntrDataDmpOutput(CCmdUI *pCmdUI)
{
	CWnd* pWnd = theApp.GetWindowHandle(IDD_DLG_EQIPREC_DUMPOUTPUT);
	if (pWnd == NULL)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
	}
}

/*============================================================================*/
/*! アプリケーション

-# 『設備記録』-『監視データダンプ出力』-『ダンプ定義ファイル選択画面』メニュー

@brief	ダンプ定義ファイル選択画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMeDumpFileSel()
{
	CEquipmentRecSelectFile* pDlg = (CEquipmentRecSelectFile*)theApp.GetWindowHandle(IDD_DLG_EQIPREC_SELFILE);
	if (pDlg != NULL){
		pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		pDlg = new CEquipmentRecSelectFile();
		if (pDlg != NULL)
		{
			if (pDlg->Create(IDD_DLG_EQIPREC_SELFILE, this))
			{
				pDlg->ShowWindow(SW_SHOW);
				theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_EQIPREC_SELFILE);
			}
		}
	}
}

void CMainFrame::OnUpdateMeDumpFileSel(CCmdUI *pCmdUI)
{
	CWnd* pWnd = theApp.GetWindowHandle(IDD_DLG_EQIPREC_SELFILE);
	if (pWnd == NULL)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
	}
}

/*============================================================================*/
/*! アプリケーション

-# 『設備記録』-『アンテナ設備運用履歴』メニュー

@brief	アンテナ設備運用履歴画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMeAntEqipOpeHis()
{
	CAntennaHistroyView* pDlg = (CAntennaHistroyView*)theApp.GetWindowHandle(IDD_DIALOG_ANTHISTORY);
	if (pDlg != NULL){
		pDlg->SetTitle(_T("アンテナ設備運用履歴表示"), eAntHistory_Operation);
		pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		pDlg = new CAntennaHistroyView();
		if (pDlg != NULL)
		{
			if (pDlg->Create(IDD_DIALOG_ANTHISTORY, this))
			{
				pDlg->SetTitle(_T("アンテナ設備運用履歴表示"), eAntHistory_Operation);
				pDlg->ShowWindow(SW_SHOW);
				theApp.AddWindowHandle((LPARAM)pDlg, IDD_DIALOG_ANTHISTORY);
			}
		}
	}
}

void CMainFrame::OnUpdateMeAntEqipOpeHis(CCmdUI *pCmdUI)
{
	CWnd* pWnd = theApp.GetWindowHandle(IDD_DIALOG_ANTHISTORY);
	if (pWnd == NULL)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
	}
}


/*============================================================================*/
/*! アプリケーション

-# 『設備記録』-『アンテナ設備駆動履歴』メニュー

@brief	アンテナ設備駆動履歴画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMeAntEqipDrvHis()
{
	CAntennaHistroyView* pDlg = (CAntennaHistroyView*)theApp.GetWindowHandle(IDD_DIALOG_ANTHISTORY+200);
	if (pDlg != NULL){
		pDlg->SetTitle(_T("アンテナ設備駆動履歴表示"), eAntHistory_Drive);
		pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		pDlg = new CAntennaHistroyView();
		if (pDlg != NULL)
		{
			if (pDlg->Create(IDD_DIALOG_ANTHISTORY, this))
			{
				pDlg->SetTitle(_T("アンテナ設備駆動履歴表示"), eAntHistory_Drive);
				pDlg->ShowWindow(SW_SHOW);
				theApp.AddWindowHandle((LPARAM)pDlg, IDD_DIALOG_ANTHISTORY+200);
			}
		}
	}
}

void CMainFrame::OnUpdateMeAntEqipDrvHis(CCmdUI *pCmdUI)
{
	CWnd* pWnd = theApp.GetWindowHandle(IDD_DIALOG_ANTHISTORY + 200);
	if (pWnd == NULL)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
	}
}


/*============================================================================*/
/*! アプリケーション

-# 『設備記録』-『運用履歴』-『リアルタイム』メニュー

@brief	運用履歴(リアルタイム)画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMeOpeHisRealtime()
{
	if (theApp.GetSelectStationMode() == eStation_1){
		if (theApp.GetView(eViewType1_OperationHist) == NULL)
			theApp.OpenView(eViewType1_OperationHist);
		else
		{
//			theApp.CloseView(eViewType1_OperationHist);
			CView *view = theApp.GetView(eViewType1_OperationHist);
			((CMDIChildWnd*)view->GetParent())->MDIActivate();
		}
	}
	if (theApp.GetSelectStationMode() == eStation_5){
		if (theApp.GetView(eViewType5_OperationHist) == NULL)
			theApp.OpenView(eViewType5_OperationHist);
		else
		{
//			theApp.CloseView(eViewType5_OperationHist);
			CView *view = theApp.GetView(eViewType5_OperationHist);
			((CMDIChildWnd*)view->GetParent())->MDIActivate();
		}
	}
}

void CMainFrame::OnUpdateMeOpeHisRealtime(CCmdUI *pCmdUI)
{
	if (theApp.GetSelectStationMode() == eStation_1)
		pCmdUI->SetCheck((theApp.GetView(eViewType1_OperationHist) != NULL) ? 1 : 0);
	else if (theApp.GetSelectStationMode() == eStation_5)
		pCmdUI->SetCheck((theApp.GetView(eViewType5_OperationHist) != NULL) ? 1 : 0);
}


/*============================================================================*/
/*! アプリケーション

-# 『設備記録』-『運用履歴』-『検索』メニュー

@brief	運用履歴(検索)画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMeOpeHisSearch()
{
	if (theApp.GetSelectStationMode() == eStation_1){
		if (theApp.GetView(eViewType1_OperationSearch) == NULL){
			theApp.SetHistorySearchType(eHistory_SearchOperation);
			theApp.OpenView(eViewType1_OperationSearch);
		}
		else
		{ 
//			theApp.CloseView(eViewType1_OperationSearch);
			CView *view = theApp.GetView(eViewType1_OperationSearch);
			((CMDIChildWnd*)view->GetParent())->MDIActivate();
		}
	}
	if (theApp.GetSelectStationMode() == eStation_5){
		if (theApp.GetView(eViewType5_OperationSearch) == NULL){
			theApp.SetHistorySearchType(eHistory_SearchOperation);
			theApp.OpenView(eViewType5_OperationSearch);
		}
		else
		{
//			theApp.CloseView(eViewType5_OperationSearch);
			CView *view = theApp.GetView(eViewType5_OperationSearch);
			((CMDIChildWnd*)view->GetParent())->MDIActivate();
		}
	}
}

void CMainFrame::OnUpdateMeOpeHisSearch(CCmdUI *pCmdUI)
{
	if (theApp.GetSelectStationMode() == eStation_1)
		pCmdUI->SetCheck((theApp.GetView(eViewType1_OperationSearch) != NULL) ? 1 : 0);
	else if (theApp.GetSelectStationMode() == eStation_5)
		pCmdUI->SetCheck((theApp.GetView(eViewType5_OperationSearch) != NULL) ? 1 : 0);
}


/*============================================================================*/
/*! アプリケーション

-# 『設備記録』-『制御履歴』-『リアルタイム』メニュー

@brief	制御履歴(リアルタイム)画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMeCtrlHisRealtime()
{
	if (theApp.GetSelectStationMode() == eStation_1){
		if (theApp.GetView(eViewType1_ControlHist) == NULL)
			theApp.OpenView(eViewType1_ControlHist);
		else
		{
//			theApp.CloseView(eViewType1_ControlHist);
			CView *view = theApp.GetView(eViewType1_ControlHist);
			((CMDIChildWnd*)view->GetParent())->MDIActivate();
		}
	}
	if (theApp.GetSelectStationMode() == eStation_5){
		if (theApp.GetView(eViewType5_ControlHist) == NULL)
			theApp.OpenView(eViewType5_ControlHist);
		else
		{
//			theApp.CloseView(eViewType5_ControlHist);
			CView *view = theApp.GetView(eViewType5_ControlHist);
			((CMDIChildWnd*)view->GetParent())->MDIActivate();
		}
	}
}

void CMainFrame::OnUpdateMeCtrlHisRealtime(CCmdUI *pCmdUI)
{
	if (theApp.GetSelectStationMode() == eStation_1)
		pCmdUI->SetCheck((theApp.GetView(eViewType1_ControlHist) != NULL) ? 1 : 0);
	else if (theApp.GetSelectStationMode() == eStation_5)
		pCmdUI->SetCheck((theApp.GetView(eViewType5_ControlHist) != NULL) ? 1 : 0);
}


/*============================================================================*/
/*! アプリケーション

-# 『設備記録』-『制御履歴』-『検索』メニュー

@brief	制御履歴(検索)画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMeCtrlHisSearch()
{
	if (theApp.GetSelectStationMode() == eStation_1){
		if (theApp.GetView(eViewType1_ControlSearch) == NULL){
			theApp.SetHistorySearchType(eHistory_SearchControl);
			theApp.OpenView(eViewType1_ControlSearch);
		}
		else
		{
//			theApp.CloseView(eViewType1_ControlSearch);
			CView *view = theApp.GetView(eViewType1_ControlSearch);
			((CMDIChildWnd*)view->GetParent())->MDIActivate();
		}
	}
	if (theApp.GetSelectStationMode() == eStation_5){
		if (theApp.GetView(eViewType5_ControlSearch) == NULL){
			theApp.SetHistorySearchType(eHistory_SearchControl);
			theApp.OpenView(eViewType5_ControlSearch);
		}
		else
		{
//			theApp.CloseView(eViewType5_ControlSearch);
			CView *view = theApp.GetView(eViewType5_ControlSearch);
			((CMDIChildWnd*)view->GetParent())->MDIActivate();
		}
	}
}

void CMainFrame::OnUpdateMeCtrlHisSearch(CCmdUI *pCmdUI)
{
	if (theApp.GetSelectStationMode() == eStation_1)
		pCmdUI->SetCheck((theApp.GetView(eViewType1_ControlSearch) != NULL) ? 1 : 0);
	else if (theApp.GetSelectStationMode() == eStation_5)
		pCmdUI->SetCheck((theApp.GetView(eViewType5_ControlSearch) != NULL) ? 1 : 0);
}


/*============================================================================*/
/*! アプリケーション

-# 『設備記録』-『設備状態履歴』-『リアルタイム』メニュー

@brief	設備状態履歴(リアルタイム)画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMeEqipHisRealtime()
{
	if (theApp.GetSelectStationMode() == eStation_1){
		if (theApp.GetView(eViewType1_EquipmentHist) == NULL)
			theApp.OpenView(eViewType1_EquipmentHist);
		else
		{
//			theApp.CloseView(eViewType1_EquipmentHist);
			CView *view = theApp.GetView(eViewType1_EquipmentHist);
			((CMDIChildWnd*)view->GetParent())->MDIActivate();
		}
	}
	if (theApp.GetSelectStationMode() == eStation_5){
		if (theApp.GetView(eViewType5_EquipmentHist) == NULL)
			theApp.OpenView(eViewType5_EquipmentHist);
		else
		{
//			theApp.CloseView(eViewType5_EquipmentHist);
			CView *view = theApp.GetView(eViewType5_EquipmentHist);
			((CMDIChildWnd*)view->GetParent())->MDIActivate();
		}
	}
}

void CMainFrame::OnUpdateMeEqipHisRealtime(CCmdUI *pCmdUI)
{
	if (theApp.GetSelectStationMode() == eStation_1)
		pCmdUI->SetCheck((theApp.GetView(eViewType1_EquipmentHist) != NULL) ? 1 : 0);
	else if (theApp.GetSelectStationMode() == eStation_5)
		pCmdUI->SetCheck((theApp.GetView(eViewType5_EquipmentHist) != NULL) ? 1 : 0);
}


/*============================================================================*/
/*! アプリケーション

-# 『設備記録』-『イベント履歴』-『リアルタイム』メニュー

@brief	イベント履歴(リアルタイム)画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMeEventHisRealtime()
{
	if (theApp.GetSelectStationMode() == eStation_1){
		if (theApp.GetView(eViewType1_EventHist) == NULL)
			theApp.OpenView(eViewType1_EventHist);
		else
		{
//			theApp.CloseView(eViewType1_EventHist);
			CView *view = theApp.GetView(eViewType1_EventHist);
			((CMDIChildWnd*)view->GetParent())->MDIActivate();
		}
	}
	if (theApp.GetSelectStationMode() == eStation_5){
		if (theApp.GetView(eViewType5_EventHist) == NULL)
			theApp.OpenView(eViewType5_EventHist);
		else
		{
//			theApp.CloseView(eViewType5_EventHist);
			CView *view = theApp.GetView(eViewType5_EventHist);
			((CMDIChildWnd*)view->GetParent())->MDIActivate();
		}
	}
}

void CMainFrame::OnUpdateMeEventHisRealtime(CCmdUI *pCmdUI)
{
	if (theApp.GetSelectStationMode() == eStation_1)
		pCmdUI->SetCheck((theApp.GetView(eViewType1_EventHist) != NULL) ? 1 : 0);
	else if (theApp.GetSelectStationMode() == eStation_5)
		pCmdUI->SetCheck((theApp.GetView(eViewType5_EventHist) != NULL) ? 1 : 0);
}


/*============================================================================*/
/*! アプリケーション

-# 『設備記録』-『設備状態履歴』-『検索』メニュー

@brief	設備状態履歴(検索)画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMeEqipHisSearch()
{
	if (theApp.GetSelectStationMode() == eStation_1){
		if (theApp.GetView(eViewType1_EquipmentSearch) == NULL){
			theApp.SetHistorySearchType(eHistory_SearchEquipment);
			theApp.OpenView(eViewType1_EquipmentSearch);
		}
		else
		{
//			theApp.CloseView(eViewType1_EquipmentSearch);
			CView *view = theApp.GetView(eViewType1_EquipmentSearch);
			((CMDIChildWnd*)view->GetParent())->MDIActivate();
		}
	}
	if (theApp.GetSelectStationMode() == eStation_5){
		if (theApp.GetView(eViewType5_EquipmentSearch) == NULL){
			theApp.SetHistorySearchType(eHistory_SearchEquipment);
			theApp.OpenView(eViewType5_EquipmentSearch);
		}
		else
		{
//			theApp.CloseView(eViewType5_EquipmentSearch);
			CView *view = theApp.GetView(eViewType5_EquipmentSearch);
			((CMDIChildWnd*)view->GetParent())->MDIActivate();
		}
	}
}

void CMainFrame::OnUpdateMeEqipHisSearch(CCmdUI *pCmdUI)
{
	if (theApp.GetSelectStationMode() == eStation_1)
		pCmdUI->SetCheck((theApp.GetView(eViewType1_EquipmentSearch) != NULL) ? 1 : 0);
	else if (theApp.GetSelectStationMode() == eStation_5)
		pCmdUI->SetCheck((theApp.GetView(eViewType5_EquipmentSearch) != NULL) ? 1 : 0);
}


/*============================================================================*/
/*! アプリケーション

-# 『設備記録』-『イベント履歴』-『検索』メニュー

@brief	イベント履歴(検索)画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMeEventHisSearch()
{
	if (theApp.GetSelectStationMode() == eStation_1){
		if (theApp.GetView(eViewType1_EventSearch) == NULL){
			theApp.SetHistorySearchType(eHistory_SearchEvent);
			theApp.OpenView(eViewType1_EventSearch);
		}
		else
		{
//			theApp.CloseView(eViewType1_EventSearch);
			CView *view = theApp.GetView(eViewType1_EventSearch);
			((CMDIChildWnd*)view->GetParent())->MDIActivate();
		}
	}
	if (theApp.GetSelectStationMode() == eStation_5){
		if (theApp.GetView(eViewType5_EventSearch) == NULL){
			theApp.SetHistorySearchType(eHistory_SearchEvent);
			theApp.OpenView(eViewType5_EventSearch);
		}
		else
		{
//			theApp.CloseView(eViewType5_EventSearch);
			CView *view = theApp.GetView(eViewType5_EventSearch);
			((CMDIChildWnd*)view->GetParent())->MDIActivate();
		}
	}
}

void CMainFrame::OnUpdateMeEventHisSearch(CCmdUI *pCmdUI)
{
	if (theApp.GetSelectStationMode() == eStation_1)
		pCmdUI->SetCheck((theApp.GetView(eViewType1_EventSearch) != NULL) ? 1 : 0);
	else if (theApp.GetSelectStationMode() == eStation_5)
		pCmdUI->SetCheck((theApp.GetView(eViewType5_EventSearch) != NULL) ? 1 : 0);
}


/*============================================================================*/
/*! アプリケーション

-# 『設備記録』-『設備アラーム状況一覧』メニュー

@brief	設備アラーム状況一覧画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMeEqipAlarm()
{
	if (theApp.GetSelectStationMode() == eStation_1){
		if (theApp.GetView(eViewType1_AlarmList) == NULL)
			theApp.OpenView(eViewType1_AlarmList);
		else
		{
//			theApp.CloseView(eViewType1_AlarmList);
			CView *view = theApp.GetView(eViewType1_AlarmList);
			((CMDIChildWnd*)view->GetParent())->MDIActivate();
		}
	}
}


void CMainFrame::OnUpdateMeEqipAlarm(CCmdUI *pCmdUI)
{
	if (theApp.GetSelectStationMode() == eStation_1)
		pCmdUI->SetCheck((theApp.GetView(eViewType1_AlarmList) != NULL) ? 1 : 0);
}

/*============================================================================*/
/*! アプリケーション

-# 『セッション』-『セッション接続状態設定』メニュー

@brief	セッション状態画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMiSessionSet()
{
//	TCHAR szMsg[256];

	CSessionStatus* pDlg = (CSessionStatus*)theApp.GetWindowHandle(IDD_DLG_SESSIONSTATUS);
	if (pDlg != NULL)
	{
		pDlg->ShowWindow(SW_SHOW);
		return;
	}

	pDlg = new CSessionStatus();
	if (pDlg != NULL)
	{
		for (int i = 0; i < eStation_MAX; i++)
		{
			pDlg->AddStation(i, mStationString[i]);
		}
		pDlg->SetStation(theApp.GetSelectStation());

		if (pDlg->Create(IDD_DLG_SESSIONSTATUS, this))
		{
			pDlg->ShowWindow(SW_SHOW);
			theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_SESSIONSTATUS);
		}
	}
}

void CMainFrame::OnUpdateMiSessionSet(CCmdUI *pCmdUI)
{
	CWnd* pWnd = theApp.GetWindowHandle(IDD_DLG_SESSIONSTATUS);
	if (pWnd == NULL)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
	}
}


/*============================================================================*/
/*! アプリケーション

-# 『追尾状況』-『周波数制御』メニュー

@brief	周波数制御画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMrFreqCtrl()
{
	if (theApp.GetSelectStation() == eStation_Usuda642)
	{
		// 54m
		CFrequencyCtrl54* pDlg = (CFrequencyCtrl54*)theApp.GetWindowHandle(IDD_DLG_FREQ_CTRL54);
		if (pDlg != NULL){
			pDlg->Initialize();
			pDlg->ShowWindow(SW_SHOW);
			return;
		}

		pDlg = new CFrequencyCtrl54();
		if (pDlg != NULL)
		{
			if (pDlg->Create(IDD_DLG_FREQ_CTRL54, this))
			{
				pDlg->ShowWindow(SW_SHOW);
				theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_FREQ_CTRL54);
			}
		}
	}
	else
	{
		// その他
		CFrequencyCtrl* pDlg = (CFrequencyCtrl*)theApp.GetWindowHandle(IDD_DLG_FREQ_CTRL);
		if (pDlg != NULL){
			pDlg->Initialize();
			pDlg->ShowWindow(SW_SHOW);
			return;
		}

		pDlg = new CFrequencyCtrl();
		if (pDlg != NULL)
		{
			if (pDlg->Create(IDD_DLG_FREQ_CTRL, this))
			{
				pDlg->ShowWindow(SW_SHOW);
				theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_FREQ_CTRL);
			}
		}
	}
}

void CMainFrame::OnUpdateMrFreqCtrl(CCmdUI *pCmdUI)
{
	if (theApp.GetSelectStationMode() == eStation_1 && theApp.GetSelectStation() == eStation_Uchinoura20){
		pCmdUI->Enable(FALSE);
	}
	else{
		pCmdUI->Enable(TRUE);
		CWnd* pWnd;
		if (theApp.GetSelectStation() == eStation_Usuda642)
		{
			// 54m
			pWnd = theApp.GetWindowHandle(IDD_DLG_FREQ_CTRL54);
		}
		else
		{
			// その他
			pWnd = theApp.GetWindowHandle(IDD_DLG_FREQ_CTRL);
		}
		if (pWnd == NULL)
		{
			pCmdUI->SetCheck(0);
		}
		else
		{
			pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
		}
	}
}


/*============================================================================*/
/*! アプリケーション

-# 『追尾状況』-『衛星捕捉』メニュー

@brief	衛星捕捉画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMrSatlCap()
{
	if(theApp.GetSelectStation() == eStation_Usuda642)
	{
		CSatelliteCapture54* pDlg = (CSatelliteCapture54*)theApp.GetWindowHandle(IDD_DLG_SATELLITECAPTURE54);
		if (pDlg != NULL)
		{
			pDlg->ShowWindow(SW_SHOW);
			return;
		}

		pDlg = new CSatelliteCapture54();
		if (pDlg != NULL)
		{
			if (pDlg->Create(IDD_DLG_SATELLITECAPTURE54, this))
			{
				pDlg->ShowWindow(SW_SHOW);
				theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_SATELLITECAPTURE54);
			}
		}
	}
	else
	{
		CSatelliteCapture* pDlg = (CSatelliteCapture*)theApp.GetWindowHandle(IDD_DLG_SATELLITECAPTURE);
		if (pDlg != NULL)
		{
			pDlg->ShowWindow(SW_SHOW);
			return;
		}

		pDlg = new CSatelliteCapture();
		if (pDlg != NULL)
		{
			if (pDlg->Create(IDD_DLG_SATELLITECAPTURE, this))
			{
				pDlg->ShowWindow(SW_SHOW);
				theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_SATELLITECAPTURE);
			}
		}
	}
}

void CMainFrame::OnUpdateMrSatlCap(CCmdUI *pCmdUI)
{
	CWnd* pWnd;
	if(theApp.GetSelectStation() == eStation_Usuda642)
	{
		pWnd = theApp.GetWindowHandle(IDD_DLG_SATELLITECAPTURE54);
	}
	else
	{
		pWnd = theApp.GetWindowHandle(IDD_DLG_SATELLITECAPTURE);
	}
	if (pWnd == NULL)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck((pWnd->IsWindowVisible()) ? 1 : 0);
	}
}


/*============================================================================*/
/*! アプリケーション

-# 『オプション』-『AGC校正テーブル操作』-『AGC校正テーブル読込・衛星選択』メニュー

@brief	AGC校正テーブル読込・衛星選択画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMtAgcTblRead()
{
	CTblSatelliteSelection dlg;
	dlg.DoModal();
}

void CMainFrame::OnUpdateMtAgcTblRead(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


/*============================================================================*/
/*! アプリケーション

-# 『オプション』-『AGC校正テーブルLNA切替』メニュー

@brief	AGC校正テーブルLNA切替画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMtAgcTblSelectLNA()
{
	// あらかじめ、現状のLNA現在値取得
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = AGCTBL_LNASEL_SH;

	if (theApp.GetSelectStation() == eStation_Usuda64)
		stParam.strRemShellParam.Format(_T("udsc64"));
	else if (theApp.GetSelectStation() == eStation_Usuda642)
		stParam.strRemShellParam.Format(_T("udsc54"));
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
		stParam.strRemShellParam.Format(_T("usc34"));
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
		stParam.strRemShellParam.Format(_T("usc20"));

	int err = CSshShell::ExecShell(stParam);

	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CMainFrame"), _T("OnMtAgcTblSelectLNA"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err == 1 || err == 2)
	{
		CSelectLNADlg dlg;
		dlg.SetLNA((UINT)err);
		dlg.DoModal();
	}
	else
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CMainFrame"), _T("OnMtAgcTblSelectLNA"), _T("Error"), strDebug, nLogEx::error);

		AfxMessageBox(_T("AGC校正テーブルLNAの現在値取得に失敗しました。"), MB_OK | MB_ICONWARNING);
	}
}

void CMainFrame::OnUpdateMtAgcTblSelectLNA(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


/*============================================================================*/
/*! アプリケーション

-# 『オプション』-『AGC校正テーブル操作』-『AGC校正テーブルファイルコピー・衛星選択』メニュー

@brief	AGC校正テーブルファイルコピー・衛星選択画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMtAgcTblFileCpy()
{
	CTblSatelliteSelection dlg;
	dlg.DoModal();
}

void CMainFrame::OnUpdateMtAgcTblFileCpy(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}



/*============================================================================*/
/*! アプリケーション

-# 『オプション』-『AGC校正テーブル操作』-『AGC校正テーブルファイル復元・衛星選択』メニュー

@brief	AGC校正テーブルファイルコピー・衛星選択画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMtAgcTblFileRst()
{
	CTblSatelliteSelection dlg;
	dlg.DoModal();
}

void CMainFrame::OnUpdateMtAgcTblFileRst(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


/*============================================================================*/
/*! アプリケーション

-# 『オプション』-『局内ディレイ値テーブル操作』-『局内ディレイ値テーブルファイルコピー・衛星選択』メニュー

@brief	局内ディレイ値テーブルファイルコピー・衛星選択画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMtStaTblFileCpy()
{
	CTblSatelliteSelection dlg;
	dlg.DoModal();
}

void CMainFrame::OnUpdateMtStaTblFileCpy(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}



/*============================================================================*/
/*! アプリケーション

-# 『オプション』-『局内ディレイ値テーブル操作』-『局内ディレイ値テーブルファイル復元・衛星選択』メニュー

@brief	局内ディレイ値テーブルファイル復元・衛星選択画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMtStaTblFileRst()
{
	CTblSatelliteSelection dlg;
	dlg.DoModal();
}

void CMainFrame::OnUpdateMtStaTblFileRst(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


/*============================================================================*/
/*! アプリケーション

-# 『オプション』-『レイアウト』-『レイアウト保存』メニュー

@brief	レイアウト保存画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMtLayoutSave()
{
	const TCHAR BASED_CODE szFilter[] = _T("Station Control Layout(*.scl)|*.scl|All Files (*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("scl"), NULL, OFN_OVERWRITEPROMPT | OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);
	if (dlg.DoModal() != IDOK)
		return;

	CString fileName = dlg.GetPathName();

	SaveLayout(fileName);
}


/*============================================================================*/
/*! アプリケーション

-# 『オプション』-『レイアウト』-『レイアウト復元』メニュー

@brief	レイアウト復元画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMtLayoutRestor()
{
	const TCHAR BASED_CODE szFilter[] = _T("Station Control Layout(*.scl)|*.scl|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("scl"), NULL, OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (dlg.DoModal() != IDOK)
		return;

	CString fileName = dlg.GetPathName();

	LoadLayout(fileName);
}

/*============================================================================*/
/*! アプリケーション

-# 『局選択』-『全局モード』メニュー

@brief	全局モードを表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnStation10()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateStation10(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


/*============================================================================*/
/*! アプリケーション

-# 『局選択』-『臼田64m局』メニュー

@brief	臼田64m局モードを表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnStationUsuda64()
{
	if (theApp.GetLaunchStation(eStation_Usuda64) == false){
		stThreadInfo* pThreadInfo = new stThreadInfo;
		if (pThreadInfo){
			// 構造体のメンバーに代入する
			pThreadInfo->pTermThreadEvent = m_pStationThreadsEvent[eStation_Usuda64];
			pThreadInfo->pWnd = this;
			pThreadInfo->bPassStatus = false;
			pThreadInfo->nStation = eStation_Usuda64;
			AfxBeginThread(AppLaunchAndWait, pThreadInfo);
		}
	}
	else{
		//m_pStationThreadsEvent[eStation_Usuda64]->SetEvent();
	}
}


void CMainFrame::OnUpdateStationUsuda64(CCmdUI *pCmdUI)
{
	if (theApp.GetControlAccess() & (1 << eStation_Usuda64))
		pCmdUI->SetCheck((theApp.GetLaunchStation(eStation_Usuda64) == true) ? 1 : 0);
	else
		pCmdUI->Enable(FALSE);
}


/*============================================================================*/
/*! アプリケーション

-# 『局選択』-『臼田64m局2』メニュー

@brief	臼田64m局2モードを表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnStationUsuda2()
{
	if (theApp.GetLaunchStation(eStation_Usuda642) == false){
		stThreadInfo* pThreadInfo = new stThreadInfo;
		if (pThreadInfo){
			// 構造体のメンバーに代入する
			pThreadInfo->pTermThreadEvent = m_pStationThreadsEvent[eStation_Usuda642];
			pThreadInfo->pWnd = this;
			pThreadInfo->bPassStatus = false;
			pThreadInfo->nStation = eStation_Usuda642;
			AfxBeginThread(AppLaunchAndWait, pThreadInfo);
		}
	}
	else{
		//m_pStationThreadsEvent[eStation_Usuda642]->SetEvent();
	}
}


void CMainFrame::OnUpdateStationUsuda2(CCmdUI *pCmdUI)
{
	if (theApp.GetControlAccess() & (1 << eStation_Usuda642))
		pCmdUI->SetCheck((theApp.GetLaunchStation(eStation_Usuda642) == true) ? 1 : 0);
	else
		pCmdUI->Enable(FALSE);
}


/*============================================================================*/
/*! アプリケーション

-# 『局選択』-『内之浦34m局』メニュー

@brief	内之浦34m局モードを表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnStationUchinoura34()
{
	if (theApp.GetLaunchStation(eStation_Uchinoura34) == false){
		stThreadInfo* pThreadInfo = new stThreadInfo;
		if (pThreadInfo){
			// 構造体のメンバーに代入する
			pThreadInfo->pTermThreadEvent = m_pStationThreadsEvent[eStation_Uchinoura34];
			pThreadInfo->pWnd = this;
			pThreadInfo->bPassStatus = false;
			pThreadInfo->nStation = eStation_Uchinoura34;
			AfxBeginThread(AppLaunchAndWait, pThreadInfo);
		}
	}
	else{
		//m_pStationThreadsEvent[eStation_Uchinoura34]->SetEvent();
	}
}


void CMainFrame::OnUpdateStationUchinoura34(CCmdUI *pCmdUI)
{
	if (theApp.GetControlAccess() & (1 << eStation_Uchinoura34))
		pCmdUI->SetCheck((theApp.GetLaunchStation(eStation_Uchinoura34) == true) ? 1 : 0);
	else
		pCmdUI->Enable(FALSE);
}


/*============================================================================*/
/*! アプリケーション

-# 『局選択』-『内之浦20m局』メニュー

@brief	内之浦20m局モードを表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnStationUchinoura20()
{
	if (theApp.GetLaunchStation(eStation_Uchinoura20) == false){
		stThreadInfo* pThreadInfo = new stThreadInfo;
		if (pThreadInfo){
			// 構造体のメンバーに代入する
			pThreadInfo->pTermThreadEvent = m_pStationThreadsEvent[eStation_Uchinoura20];
			pThreadInfo->pWnd = this;
			pThreadInfo->bPassStatus = false;
			pThreadInfo->nStation = eStation_Uchinoura20;
			AfxBeginThread(AppLaunchAndWait, pThreadInfo);
		}
	}
	else{
		//m_pStationThreadsEvent[eStation_Uchinoura20]->SetEvent();
	}
}

void CMainFrame::OnUpdateStationUchinoura20(CCmdUI *pCmdUI)
{
	if (theApp.GetControlAccess() & (1 << eStation_Uchinoura20))
		pCmdUI->SetCheck((theApp.GetLaunchStation(eStation_Uchinoura20) == true) ? 1 : 0);
	else
		pCmdUI->Enable(FALSE);
}


/*============================================================================*/
/*! アプリケーション

-# 『ヘルプ』-『取扱説明書』メニュー

@brief	取扱説明書を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMhManual()
{
//	AfxMessageBox(_T("DUMMY ！！！取説表示"), MB_OK | MB_ICONEXCLAMATION);
	CString strDocFilePath;
	strDocFilePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppPath(), (LPCTSTR)CString(DOCUMENT_FILENAME));
	// 実行
	ShellExecute(NULL, _T("open"), (LPTSTR)strDocFilePath.GetBuffer(), NULL, NULL, SW_HIDE);
	strDocFilePath.ReleaseBuffer();
}


BOOL CMainFrame::OnNcActivate(BOOL bActive)
{
	BOOL ret = CMDIFrameWndEx::OnNcActivate(bActive);

	paintCaption();

	return ret;
}


void CMainFrame::OnNcPaint()
{
	paintCaption();
}

void CMainFrame::paintCaption()
{
	CString str;
	GetWindowText(str);
	Default();
	//SetWindowText(_T(""));
	return;

	CWindowDC dc(this);
	CRect rect, rectCaption;
	//GetClientRect(rect);
	GetWindowRect(rect);
	rectCaption.left = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXSMICON) + GetSystemMetrics(SM_CXBORDER);
	rectCaption.top = GetSystemMetrics(SM_CYFRAME);
	rectCaption.right = (rect.right - rect.left) - GetSystemMetrics(SM_CXFRAME) - (GetSystemMetrics(SM_CXSIZE) * 3);
	rectCaption.right -= 100;
	rectCaption.bottom = GetSystemMetrics(SM_CYCAPTION);

	COLORREF crText, crCaption;
	crText = GetSysColor((GetActiveWindow()==this) ? COLOR_CAPTIONTEXT : COLOR_INACTIVECAPTIONTEXT);
	crCaption = GetSysColor((GetActiveWindow() == this) ? COLOR_ACTIVECAPTION : COLOR_INACTIVECAPTION);
	//dc.FillSolidRect(&rectCaption, crCaption);
	dc.SetTextColor(crText);
	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(str, rectCaption, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

/*============================================================================*/
/*! アプリケーション

-# メニューバー切替

@brief	メニューバーを表示モードにより切り替える。（10局/5局/1局）

@param	eStation：表示種別
@retval	なし

*/
/*============================================================================*/
void CMainFrame::SetStationMenuBar(UINT eStation)
{
	switch (eStation)
	{
	case eStation_1:
		if (theApp.GetSelectStation() == eStation_Usuda64)
		{
			// 64mの場合
			// 1局用メニューバー表示
			if (theApp.GetExecuteMode() == eExecuteMode_Control)
			{
				if(theApp.GetLimitedMode() == eLimitedMode_Enable)
				{
					m_wndMenuBar.SetDefaultMenuResId(IDR_STATION_1_LIMITED_64M);
				}
				else
				{
#ifdef MON_UT
					m_wndMenuBar.SetDefaultMenuResId(IDR_STATION_1_MON_UT_64M);
#else
					m_wndMenuBar.SetDefaultMenuResId(IDR_STATION_1_64M);
#endif /* MON_UT */
				}
			}
			else
			{
				m_wndMenuBar.SetDefaultMenuResId(IDR_STATION_1_DISABLE_64M);
			}
			m_wndMenuBar.RestoreOriginalState();
		}
		else
		{
			// 64m以外の場合
			// 1局用メニューバー表示
			if (theApp.GetExecuteMode() == eExecuteMode_Control)
			{
				if (theApp.GetLimitedMode() == eLimitedMode_Enable)
				{
					m_wndMenuBar.SetDefaultMenuResId(IDR_STATION_1_LIMITED);
				}
				else
				{
#ifdef MON_UT
					m_wndMenuBar.SetDefaultMenuResId(IDR_STATION_1_MON_UT);
#else
					m_wndMenuBar.SetDefaultMenuResId(IDR_STATION_1);
#endif /* MON_UT */
				}
			}
			else
			{
				m_wndMenuBar.SetDefaultMenuResId(IDR_STATION_1_DISABLE);
			}
			m_wndMenuBar.RestoreOriginalState();

		}
		break;

	case eStation_5:
		// 5局用メニューバー表示
#ifdef MON_UT
		m_wndMenuBar.SetDefaultMenuResId(IDR_STATION_5_MON_UT);
#else
		m_wndMenuBar.SetDefaultMenuResId(IDR_STATION_5);
#endif /* MON_UT */
		m_wndMenuBar.RestoreOriginalState();
		break;

	case eStation_10:
		// 10局用メニューバー表示
		m_wndMenuBar.SetDefaultMenuResId(IDR_STATION_ALL);
		m_wndMenuBar.RestoreOriginalState();
		break;

	default:
		break;
	}
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
				return;
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

	mciSendString(_T("open StatusError.wav type waveaudio alias serror"), NULL, 0, NULL);
	mStatusSoundId = mciGetDeviceID(_T("serror"));
	mciSendString(_T("play serror from 0 notify"), NULL, 0, m_hWnd);
	m_nPlayStatusSound = eStatusSound_Play;

	// ボタンカラー変更
	if (theApp.GetSelectStationMode() == eStation_5)
	{
		m_wndCaption.GetSoundBtnCtrl().m_bTransparent = false;
	}
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

-# イベント毎のサウンド再生を行う

@param	type	イベントタイプ
@retval	なし

*/
/*============================================================================*/
void CMainFrame::PlayEventSound(UINT type)
{
	CString str;
	str.Format(_T("open %s%s type waveaudio alias sevent"), (LPCTSTR)theApp.GetEventTonePath(), (LPCTSTR)mEventTone[type].wavefile);
	mciSendString(str, NULL, 0, NULL);
	m_EventSoundId = mciGetDeviceID(_T("sevent"));
	mciSendString(_T("play sevent from 0 notify"), NULL, 0, m_hWnd);
}

/*============================================================================*/
/*! アプリケーション

-# イベント毎のサウンドを停止する

@param
@retval	なし

*/
/*============================================================================*/
void CMainFrame::StopEventSound()
{
	mciSendString(_T("close sevent"), NULL, 0, NULL);
	m_EventSoundId = 0;
}

/*============================================================================*/
/*! アプリケーション

-# FABrowserの起動

@brief	FABrowserの起動

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnFaBrowser(UINT station, CString fname)
{
	CString	strExePath, strParam, strStation;

	int mstno = 0;
	if (theApp.GetSelectStationMode() == eStation_5){
		mstno = station - eViewType5_EqOperation1;
		switch (mstno)
		{
		case eStation_Usuda64:
			OnStationUsuda64();
			break;
		case eStation_Usuda642:
			OnStationUsuda2();
			break;
		case eStation_Uchinoura34:
			OnStationUchinoura34();
			break;
		case eStation_Uchinoura20:
			OnStationUchinoura20();
			break;
		default:
			break;
		}
		return;
	}
	else{
		mstno = theApp.GetSelectStation();
	}
	strStation = mStationString[mstno];

	strExePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppPath(), (LPCTSTR)mFaBrowserApp);
	fname.Replace(_T("/"), _T("／"));	// ファイル名中の半角"/"を全角"／"に置換。
	strParam.Format(_T("/TITLE:\"%s-%s\" /XML:\"%s%s\\%s\""), (LPCTSTR)strStation, (LPCTSTR)fname, (LPCTSTR)theApp.GetAppFaPanelPath(), mStationSimpleString[mstno], (LPCTSTR)fname);

//	TRACE(_T("CMainFrame::OnFaBrowser %s\n"), strParam);

	::ShellExecute(m_hWnd, _T(""), strExePath, strParam, NULL, SW_SHOWMAXIMIZED);
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

// 
/*============================================================================*/
/*! アプリケーション

-# アンテナ予報値詳細メニュー

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::OnMpForecastDetail()
{
}

static bool	mTimelineThreadEnd = false;
/*============================================================================*/
/*! アプリケーション

-# アプリとタイムラインアプリとのデータ収集スレッドの開始

@param
@retval

*/
/*============================================================================*/
UINT ThreadTimeline(LPVOID pParam)
{
	CMainFrame	*pMain = (CMainFrame *)pParam;

	while (!mTimelineThreadEnd){

		CGraphMutex::Lock(eTimeline);
		deque<CMonitorDataTemp>& data = theApp.GetMonitorDataList();
		if (!data.empty()){
			CMonitorDataTemp newData = data.front();
			data.pop_front();
			CGraphMutex::Unlock(eTimeline);
			//pMain->SendTimelineThread(newData);
		}
		else{
			CGraphMutex::Unlock(eTimeline);
		}

		Sleep(100);
	}

	return 0;
}

/*============================================================================*/
/*! アプリケーション

-# アプリとタイムラインアプリとのデータ収集スレッドの開始

@param
@retval

*/
/*============================================================================*/
void CMainFrame::CreateTimelineThread()
{
	mTimelineThreadEnd = false;
	//m_pTimelineThread = AfxBeginThread(ThreadTimeline, this);
}

/*============================================================================*/
/*! アプリケーション

-# アプリとタイムラインアプリとのデータ収集スレッドの終了

@param
@retval

*/
/*============================================================================*/
void CMainFrame::DeleteTimelineThread()
{
	if (m_pTimelineThread != NULL){
		mTimelineThreadEnd = true;
		WaitForSingleObject(m_pTimelineThread->m_hThread, 10000);
		m_pTimelineThread = NULL;
	}
}

/*============================================================================*/
/*! アプリケーション

-# アプリとタイムラインアプリとのデータ収集

@param
@retval

*/
/*============================================================================*/
void CMainFrame::SendTimelineThread(CMonitorDataTemp& data)
{
	int len = sizeof(CEngValCnv::obs_tbl_res_t);
	vector<CEngValCnv::obs_tbl_res_t>::iterator itr;
	for (itr = data.mData.begin(); itr != data.mData.end(); itr++){
		char* recBuffer = (char*)&(*itr);
		sendPipe(ePIPETYPE_TIMELINE, eMONITORDATA, (char*)recBuffer, (UINT)sizeof(CEngValCnv::obs_tbl_res_t));
	}
	//char* recBuffer = (char*)&(data.mData.at(0));
	//sendPipe(ePIPETYPE_TIMELINE, eMONITORDATA, (char*)recBuffer, (UINT)(data.mData.size()*sizeof(CEngValCnv::obs_tbl_res_t)));
}


/*============================================================================*/
/*! アプリケーション

-# AGCグラフ最小サイズ設定

@brief	AGCグラフ画面の最小サイズを設定する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::SetAGCGraphMinSize()
{
	CRect rectMin;
	CWnd* pWnd = CWnd::FromHandle(m_hWndMDIClient);

	pWnd->GetClientRect(rectMin);

	UINT	width, height;
	width = rectMin.Width() / 8 * 3 + (27);
	height = rectMin.Height() / 5 * 4 - (30);
	CTrendGraphView* pView = (CTrendGraphView*)theApp.GetView(eViewType1_Trend);
	if (pView != NULL)
	{
		CWnd* pwnd = pView->GetParent();
		if (pwnd != NULL)
		{
			CRect rectCur;
			pwnd->GetWindowRect(rectCur);
			pWnd->ScreenToClient(rectCur);
			int heightMin = (int)height * 4 / (4 + 3 + 3);
			if (heightMin > rectCur.Height())
			{
				pwnd->SetWindowPos(&wndTop, rectCur.left, rectCur.top, rectCur.Width(), heightMin, SWP_NOZORDER);
			}
			((CChildFrame*)pwnd)->SetFrameMaxSize(width, heightMin);
		}
	}
}


/*============================================================================*/
/*! アプリケーション

-# RNG, RRグラフ最小サイズ設定

@brief	RNG, RRグラフ画面の最小サイズを設定する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::SetRNGRRGraphMinSize()
{
	CRect rectMin;
	CWnd* pWnd = CWnd::FromHandle(m_hWndMDIClient);

	pWnd->GetClientRect(rectMin);

	UINT	width, height;
	width = rectMin.Width() / 8 * 3 + (27);
	height = rectMin.Height() / 5 * 4 - (30);
	CTrendGraphView* pView = (CTrendGraphView*)theApp.GetView(eViewType1_Trend);
	if (pView != NULL)
	{
		CWnd* pwnd = pView->GetParent();
		if (pwnd != NULL)
		{
			CRect rectCur;
			pwnd->GetWindowRect(rectCur);
			pWnd->ScreenToClient(rectCur);
			int heightMin = (int)height * (4 + 3) / (4 + 3 + 3);
			if (heightMin > rectCur.Height())
			{
				pwnd->SetWindowPos(&wndTop, rectCur.left, rectCur.top, rectCur.Width(), heightMin, SWP_NOZORDER);
			}
			((CChildFrame*)pwnd)->SetFrameMaxSize(width, heightMin);
		}
	}
}


/*============================================================================*/
/*! アプリケーション

-# AGC, RNG, ELグラフ最小サイズ設定

@brief	AGC, RNG, ELグラフ画面の最小サイズを設定する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::SetAGCRNGELGraphMinSize()
{
	CRect rectMin;
	CWnd* pWnd = CWnd::FromHandle(m_hWndMDIClient);

	pWnd->GetClientRect(rectMin);

	UINT	width, height;
	width = rectMin.Width() / 8 * 3 + (27);
	height = rectMin.Height() / 5 * 4 - (30);
	CTrendGraphView* pView = (CTrendGraphView*)theApp.GetView(eViewType1_Trend);
	if (pView != NULL)
	{
		CWnd* pwnd = pView->GetParent();
		if (pwnd != NULL)
		{
			CRect rectCur;
			pwnd->GetWindowRect(rectCur);
			pWnd->ScreenToClient(rectCur);
			int heightMin = (int)height;
			if (heightMin > rectCur.Height())
			{
				pwnd->SetWindowPos(&wndTop, rectCur.left, rectCur.top, rectCur.Width(), heightMin, SWP_NOZORDER);
			}
			((CChildFrame*)pwnd)->SetFrameMaxSize(width, heightMin);
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
void CMainFrame::CreateMessageThread()
{
	if (theApp.GetMessageThread() == 1){
		// 局別メッセージスレッド
		for (int i = 0; i < eStation_MAX; i++){
			if (m_pMessageStationThread[i] == NULL){
				m_pMessageStationThread[i] = (CMessageStationThread*)AfxBeginThread(RUNTIME_CLASS(CMessageStationThread), THREAD_PRIORITY_HIGHEST/*THREAD_PRIORITY_NORMAL*/, 0, CREATE_SUSPENDED);
				m_pMessageStationThread[i]->SetStation(i);
				m_pMessageStationThread[i]->ResumeThread();
			}
		}

//		// メッセージ振り分けスレッド
//		if (m_pMessageThread == NULL){
//			m_pMessageThread = (CMessageThread*)AfxBeginThread(RUNTIME_CLASS(CMessageThread), THREAD_PRIORITY_HIGHEST/*THREAD_PRIORITY_NORMAL*/, 0, CREATE_SUSPENDED);
//			m_pMessageThread->ResumeThread();
//		}
	}
}

/*============================================================================*/
/*! アプリケーション

-# メッセージ収集スレッドの削除

@param

@retval void
*/
/*============================================================================*/
void CMainFrame::RemoveMessageThread()
{
//	// メッセージ振り分けスレッド
//	if (m_pMessageThread != NULL){
//		m_pMessageThread->StopThread();
//		WaitForSingleObject(m_pMessageThread->m_hThread, 10000);
//		delete m_pMessageThread;
//		m_pMessageThread = NULL;
//	}

	// 局別メッセージスレッド
	for (int i = 0; i < eStation_MAX; i++){
		if (m_pMessageStationThread[i] != NULL){
			m_pMessageStationThread[i]->StopThread();
			WaitForSingleObject(m_pMessageStationThread[i]->m_hThread, 10000);
			delete m_pMessageStationThread[i];
			m_pMessageStationThread[i] = NULL;
		}
	}
}


/*============================================================================*/
/*! アプリケーション

-# X帯送信制御画面

@brief	X帯送信制御画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::TrsCtrlX()
{
	// X帯送信制御画面
	CTrsControlX* pDlg = (CTrsControlX*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_X);
	if (pDlg != NULL){
		pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		pDlg = new CTrsControlX();
		if (pDlg != NULL)
		{
			if (pDlg->Create(IDD_DLG_TRSCTRL_X, (CMainFrame*)theApp.GetMainWnd()))
			{
				pDlg->ShowWindow(SW_SHOW);
				theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_TRSCTRL_X);
			}
		}
	}
}

/*============================================================================*/
/*! アプリケーション

-# X/X帯送信制御画面

@brief	X/X帯送信制御画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::TrsCtrlXX()
{
	// Ｘ-X帯TCR送信制御画面
	CTrsControlXX* pDlg = (CTrsControlXX*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_XX);
	if (pDlg != NULL){
		pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		pDlg = new CTrsControlXX();
		if (pDlg != NULL)
		{
			if (pDlg->Create(IDD_DLG_TRSCTRL_XX, (CMainFrame*)theApp.GetMainWnd()))
			{
				pDlg->ShowWindow(SW_SHOW);
				theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_TRSCTRL_XX);
			}
		}
	}
}

/*============================================================================*/
/*! アプリケーション

-# X/X帯送信制御画面(54m)

@brief	X/X帯送信制御画面(54m)を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::TrsCtrlXX54()
{
	// Ｘ-X帯TCR送信制御画面
	CTrsControlXX54* pDlg = (CTrsControlXX54*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_XX54);
	if (pDlg != NULL){
		pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		pDlg = new CTrsControlXX54();
		if (pDlg != NULL)
		{
			if (pDlg->Create(IDD_DLG_TRSCTRL_XX54, (CMainFrame*)theApp.GetMainWnd()))
			{
				pDlg->ShowWindow(SW_SHOW);
				theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_TRSCTRL_XX54);
			}
		}
	}
}

/*============================================================================*/
/*! アプリケーション

-# S帯64m送信制御画面

@brief	S帯64m送信制御画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::TrsCtrlS64()
{
	CTrsControlS64* pDlg = (CTrsControlS64*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S64);
	if (pDlg != NULL){
		pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		pDlg = new CTrsControlS64();
		if (pDlg != NULL)
		{
			if (pDlg->Create(IDD_DLG_TRSCTRL_S64, (CMainFrame*)theApp.GetMainWnd()))
			{
				pDlg->ShowWindow(SW_SHOW);
				theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_TRSCTRL_S64);
			}
		}
	}
}

/*============================================================================*/
/*! アプリケーション

-# S帯54m送信制御画面

@brief	S帯64m送信制御画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::TrsCtrlS54()
{
	CTrsControlS64* pDlg = (CTrsControlS64*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S64);
	if (pDlg != NULL){
		pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		pDlg = new CTrsControlS64();
		if (pDlg != NULL)
		{
			if (pDlg->Create(IDD_DLG_TRSCTRL_S64, (CMainFrame*)theApp.GetMainWnd()))
			{
				pDlg->ShowWindow(SW_SHOW);
				theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_TRSCTRL_S64);
			}
		}
	}
}

/*============================================================================*/
/*! アプリケーション

-# S帯34m送信制御画面

@brief	S帯34m送信制御画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::TrsCtrlS34()
{
	CTrsControlS34* pDlg = (CTrsControlS34*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S34);
	if (pDlg != NULL){
		pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		pDlg = new CTrsControlS34();
		if (pDlg != NULL)
		{
			if (pDlg->Create(IDD_DLG_TRSCTRL_S34, (CMainFrame*)theApp.GetMainWnd()))
			{
				pDlg->ShowWindow(SW_SHOW);
				theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_TRSCTRL_S34);
			}
		}
	}
}

/*============================================================================*/
/*! アプリケーション

-# S帯20m送信制御画面

@brief	S帯20m送信制御画面を表示する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::TrsCtrlS20()
{
	CTrsControlS20* pDlg20 = (CTrsControlS20*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S20);
	if (pDlg20 != NULL){
		pDlg20->ShowWindow(SW_SHOW);
	}
	else
	{
		pDlg20 = new CTrsControlS20();
		if (pDlg20 != NULL)
		{
			if (pDlg20->Create(IDD_DLG_TRSCTRL_S20, (CMainFrame*)theApp.GetMainWnd()))
			{
				pDlg20->ShowWindow(SW_SHOW);
				theApp.AddWindowHandle((LPARAM)pDlg20, IDD_DLG_TRSCTRL_S20);
			}
		}
	}
}

/*============================================================================*/
/*! スレッド

-# 画面スレッド終了

@brief	画面のスレッドを終了する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CMainFrame::ExitThread()
{
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		// 校正診断を閉じる
		UINT sta = theApp.GetSelectStation();
		if (sta == eStation_Usuda64)
		{
			// 64m
			CRdnsChk64* pDlg64 = (CRdnsChk64*)theApp.GetWindowHandle(IDD_DLG_RDNSCHK64);
			if (pDlg64 != NULL)
			{
				pDlg64->ExitThread();
			}
		}
		else if (sta == eStation_Usuda642)
		{
			// 54m
			CRdnsChk54* pDlg54 = (CRdnsChk54*)theApp.GetWindowHandle(IDD_DLG_RDNSCHK54);
			if (pDlg54 != NULL)
			{
				pDlg54->ExitThread();
			}
		}
		else if (sta == eStation_Uchinoura34)
		{
			// 34m
			CRdnsChk34* pDlg34 = (CRdnsChk34*)theApp.GetWindowHandle(IDD_DLG_RDNSCHK34);
			if (pDlg34 != NULL)
			{
				pDlg34->ExitThread();
			}
		}
		else if (sta == eStation_Uchinoura20)
		{
			// 20m
			CRdnsChk20* pDlg20 = (CRdnsChk20*)theApp.GetWindowHandle(IDD_DLG_RDNSCHK20);
			if (pDlg20 != NULL)
			{
				pDlg20->ExitThread();
			}
		}

		// 逐次制御を閉じる
		CSequentialCtrlRegister* pDlg = (CSequentialCtrlRegister*)theApp.GetWindowHandle(IDD_DLG_SEQUENTIALCTRLREGISTER);
		if (pDlg != NULL)
		{
			pDlg->ExitThread();
		}
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

/*============================================================================*/
/*! アプリケーション

-# 校正診断強制終了

@retval void
*/
/*============================================================================*/
void CMainFrame::ExecRdnsChkPlanForceStop()
{
	const CString COND_AGC_S = _T("CTRL.AGC_S_COND");
	const CString COND_AGC_X = _T("CTRL.AGC_X_COND");
	const CString COND_AGC_KA = _T("CTRL.AGC_Ka_COND");
	const CString COND_RNG = _T("CTRL.RNG_COND");
	const CString COND_TLM_S = _T("CTRL.TLMCHK_S_COND");
	const CString COND_TLM_X = _T("CTRL.TLMCHK_X_COND");
	const CString COND_TLM_KA = _T("CTRL.TLMCHK_Ka_COND");
	const CString COND_CMD = _T("CTRL.CMDCHK_COND");

	//----------------------------------------------------
	// 校正診断 - 実行中診断キャンセル
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT nStation = theApp.GetSelectStation();
	CString cond = _T("");

	// 状態が全てSTOPになるまで待機
	BOOL bAGCsSTOP = FALSE;
	BOOL bAGCxSTOP = FALSE;
	BOOL bAGCkaSTOP = FALSE;
	BOOL bRNGSTOP = FALSE;
	BOOL bTLMsSTOP = FALSE;
	BOOL bTLMxSTOP = FALSE;
	BOOL bTLMkaSTOP = FALSE;
	BOOL bCMDSTOP = FALSE;
	BOOL bAllStop = FALSE;

	if (nStation != eStation_Usuda642)
	{
		// 54m以外	
		// AGC S帯
		if (bAGCsSTOP == FALSE)
		{
			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_AGC_S);
			if (obs_adr != NULL)
			{
				cond = CString(obs_adr->sz_sts_name);
				if (cond.CompareNoCase(_T("STOP")) == 0)
				{
					bAGCsSTOP = TRUE;
				}
			}
		}
		// AGC X帯
		if (bAGCxSTOP == FALSE)
		{
			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_AGC_X);
			if (obs_adr != NULL)
			{
				cond = CString(obs_adr->sz_sts_name);
				if (cond.CompareNoCase(_T("STOP")) == 0)
				{
					bAGCxSTOP = TRUE;
				}
			}
		}
		// RNG
		if (bRNGSTOP == FALSE)
		{
			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_RNG);
			if (obs_adr != NULL)
			{
				cond = CString(obs_adr->sz_sts_name);
				if (cond.CompareNoCase(_T("STOP")) == 0)
				{
					bRNGSTOP = TRUE;
				}
			}
		}
		// TLM S帯
		if (bTLMsSTOP == FALSE)
		{
			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_TLM_S);
			if (obs_adr != NULL)
			{
				cond = CString(obs_adr->sz_sts_name);
				if (cond.CompareNoCase(_T("STOP")) == 0)
				{
					bTLMsSTOP = TRUE;
				}
			}
		}
		// TLM X帯
		if (bTLMxSTOP == FALSE)
		{
			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_TLM_X);
			if (obs_adr != NULL)
			{
				cond = CString(obs_adr->sz_sts_name);
				if (cond.CompareNoCase(_T("STOP")) == 0)
				{
					bTLMxSTOP = TRUE;
				}
			}
		}
		// CMD
		if (bCMDSTOP == FALSE)
		{
			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_CMD);
			if (obs_adr != NULL)
			{
				cond = CString(obs_adr->sz_sts_name);
				if (cond.CompareNoCase(_T("STOP")) == 0)
				{
					bCMDSTOP = TRUE;
				}
			}
		}

		if (bAGCsSTOP == TRUE && bAGCxSTOP == TRUE && bRNGSTOP == TRUE && bTLMsSTOP == TRUE && bTLMxSTOP == TRUE && bCMDSTOP == TRUE)
		{
			bAllStop = TRUE;
		}
	}
	else
	{
		// 54m
		// AGC X帯
		if (bAGCxSTOP == FALSE)
		{
			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_AGC_X);
			if (obs_adr != NULL)
			{
				cond = CString(obs_adr->sz_sts_name);
				if (cond.CompareNoCase(_T("STOP")) == 0)
				{
					bAGCxSTOP = TRUE;
				}
			}
		}
		// AGC Ka帯
		if (bAGCkaSTOP == FALSE)
		{
			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_AGC_KA);
			if (obs_adr != NULL)
			{
				cond = CString(obs_adr->sz_sts_name);
				if (cond.CompareNoCase(_T("STOP")) == 0)
				{
					bAGCkaSTOP = TRUE;
				}
			}
		}
		// RNG
		if (bRNGSTOP == FALSE)
		{
			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_RNG);
			if (obs_adr != NULL)
			{
				cond = CString(obs_adr->sz_sts_name);
				if (cond.CompareNoCase(_T("STOP")) == 0)
				{
					bRNGSTOP = TRUE;
				}
			}
		}
		// TLM X帯
		if (bTLMxSTOP == FALSE)
		{
			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_TLM_X);
			if (obs_adr != NULL)
			{
				cond = CString(obs_adr->sz_sts_name);
				if (cond.CompareNoCase(_T("STOP")) == 0)
				{
					bTLMxSTOP = TRUE;
				}
			}
		}
		// TLM Ka帯
		if (bTLMkaSTOP == FALSE)
		{
			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_TLM_KA);
			if (obs_adr != NULL)
			{
				cond = CString(obs_adr->sz_sts_name);
				if (cond.CompareNoCase(_T("STOP")) == 0)
				{
					bTLMkaSTOP = TRUE;
				}
			}
		}
		// CMD
		if (bCMDSTOP == FALSE)
		{
			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, COND_CMD);
			if (obs_adr != NULL)
			{
				cond = CString(obs_adr->sz_sts_name);
				if (cond.CompareNoCase(_T("STOP")) == 0)
				{
					bCMDSTOP = TRUE;
				}
			}
		}

		if (bAGCxSTOP == TRUE && bAGCkaSTOP == TRUE && bRNGSTOP == TRUE && bTLMxSTOP == TRUE && bTLMkaSTOP == TRUE && bCMDSTOP == TRUE)
		{
			bAllStop = TRUE;
		}
	}

	bool bTimeout = false;
	if (bAllStop == FALSE)
	{
		// 強制終了が終わってない
		CTime ctime;
		theApp.GetCurrentUtcTime(ctime);
		if (ctime > m_tRdnsPlanStopTimeout)
		{
			bTimeout = true;
		}
		else
		{
			// タイマーセットして関数を抜ける
			SetTimer(200, 800, NULL);
			return;
		}
	}

	//----------------------------------------------------
	// 校正診断 - 計画強制停止通知

	// 計画削除要求
	CString sat, id, prd;
	LPARAM getform;
	UINT val4;
	CTime val5;
	theApp.GetTimeLinePlan(sat, id, getform, val4, val5);

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = REGIST_PLAN_SH;
	CString arg;
	CString strGetFrom;
	if (getform == 1)
	{
		strGetFrom = _T("enp");
	}
	else
	{
		strGetFrom = _T("sttn");
	}

	//	arg.Format(_T("%s %s %s"), mStationSimpleString[theApp.GetSelectStation()], m_strSatellite, pid);
	// 削除
	arg.Format(_T("%s del %s %s %s.pln"), (LPCTSTR)CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower(), (LPCTSTR)sat.MakeLower(), (LPCTSTR)strGetFrom, (LPCTSTR)id);

	stParam.strRemShellParam.Format(arg);

	int err = CSshShell::ExecShell(stParam);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CTimeLine"), _T("WindowProc"), _T("debug"), strDebug, nLogEx::debug);
	}

	m_bRdnsChkPlanForceStop = false;

	if (bTimeout)
	{
		// タイムアウト時
		TCHAR szTitle[256];
		memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
		if (!LoadString(GetModuleHandle(NULL), IDS_FORCED_OP_STOP_TITLE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
			return;
	
		// 現在時刻が所定時間を過ぎているならば強制終了をタイムアウト
		MessageBox(_T("校正診断強制終了をタイムアウトしました。\n"), szTitle, MB_OK | MB_ICONERROR);
	}
}
