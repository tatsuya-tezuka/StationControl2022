
// StationControl.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "StationControl.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "StationControlDoc.h"
#include "StationControlView.h"
#include "ControlHistoryView.h"
#include "EquipmentOperationView.h"
#include "OperationDetailView.h"
#include "OperationHistoryView.h"
#include "StationSituationView.h"
#include "SystemDiagram.h"
#include "TimelineView.h"
#include "TrendGraphView.h"

#include "EquipmentHistoryView.h"
#include "OperationSearchView.h"
#include "ControlSearchView.h"
#include "EquipmentSearchView.h"
#include "EquipmentStatusView.h"
#include "EquipmentStatusView54.h"
#include "EquipmentStatusView34.h"
#include "EquipmentStatusView20.h"
#include "SelectStation.h"
#include "EquipmentStationStatusView.h"
#include "SearchView.h"
#include "SearchEqView.h"

#include "EventHistoryView.h"
#include "EventSearchView.h"
#include "AlarmListView.h"
#include "ControlAcquisition.h"
#include "AlarmListFormView.h"

#include "HookMsgBox.h"

#include "..\Utility\SshShell.h"
#include "WaitDialog.h"
#include "AgcLoopBandSelectDlg.h"

#include <lm.h>
#pragma comment( lib, "Netapi32.lib" )

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStationControlApp

BEGIN_MESSAGE_MAP(CStationControlApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CStationControlApp::OnAppAbout)
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CStationControlApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CStationControlApp コンストラクション

CStationControlApp::CStationControlApp()
{
	m_bHiColorIcons = TRUE;

	// 再起動マネージャーをサポートします
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// アプリケーションが共通言語ランタイム サポート (/clr) を使用して作成されている場合:
	//     1) この追加設定は、再起動マネージャー サポートが正常に機能するために必要です。
	//     2) 作成するには、プロジェクトに System.Windows.Forms への参照を追加する必要があります。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 下のアプリケーション ID 文字列を一意の ID 文字列で置換します。推奨される
	// 文字列の形式は CompanyName.ProductName.SubProduct.VersionInformation です
	SetAppID(_T("StationControl.AppID.NoVersion"));

	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。

	m_nWindowType = 0;
	m_strDefaultPrinterName.Empty();
	m_TimeLineWnd = NULL;
	m_bTimeLineCall = false;
	m_nHcopyType = ID_HCOPY_REVERSE;
	m_nOpenHistroySearch = 0;
	m_nExecuteMode = eExecuteMode_Monitor;
	
	mpAlarmThread = NULL;
}

// 唯一の CStationControlApp オブジェクトです。

CStationControlApp theApp;


// CStationControlApp 初期化

#ifdef _DEBUG
//#define _DEBUG_NOAUTH 1
#endif
BOOL CStationControlApp::InitInstance()
{
	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// OLE ライブラリを初期化します。
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit コントロールを使用するには AfxInitRichEdit2() が必要です	
	// AfxInitRichEdit2();

	//============================================================================
	// コマンドラインの解析
	ParseCommandLine(m_cmdInfo);

	if (m_cmdInfo.mLBCtrl.IsEmpty() == FALSE)
	{
		if (m_cmdInfo.mLBStation == 1)	// 臼田64m
		{
			m_nSelectStation = 0;
		}
		else if (m_cmdInfo.mLBStation == 2)	// 内之浦34m
		{
			m_nSelectStation = 2;
		}
		else if (m_cmdInfo.mLBStation == 3)	// 内之浦20m
		{
			m_nSelectStation = 3;
		}
		else if (m_cmdInfo.mLBStation == 4)	// 臼田54m
		{
			m_nSelectStation = 1;
		}
		else
		{
			return TRUE;
		}

		// アプリケーションの初期化
		if (InitializeAppl(true) == false){
			return FALSE;
		}

		// 設備制御情報DB読込
		GetSatelliteData().GetDBAccessCls().eqctl_db_read();

		// CURRENT_LBW 取得用IF作成
		CreateShareLbwReadIF();

		// LoopBand設定ダイアログ起動
		ExecLoopBandSetDlg(m_cmdInfo.mLBCtrl);

		m_cmdInfo.mLBCtrl = _T("");
		return TRUE;
	}

	if (InitializeAppl(true) == false){
		return FALSE;
	}

	mControlAccess = GetOperationAccess();

	// コマンドライン解析の結果、「1局モード且つSELECT」の場合は局選択ダイアログを表示する
	m_nSelectMode = m_cmdInfo.mStationMode;
	m_nSelectStation = m_cmdInfo.mStation;
	if (m_nSelectMode != eStation_1 && m_nSelectStation == 9)
		m_nSelectStation = eStation_Usuda64;
	if (m_nSelectMode == eStation_1 && m_nSelectStation >= 9){
		if (mControlAccess == 0){
			AfxMessageBox(_T("全ての局に対して運用アカウントが設定されていません。\nアプリケーションを終了します。"));
			return FALSE;
		}
		CSelectStation dlgsel;
		dlgsel.mEnableStation = mControlAccess;
		dlgsel.SetSelectStation(m_nSelectStation);
		if (dlgsel.DoModal() == IDABORT)
		{
			AfxMessageBox(_T("選択可能な局に対して運用アカウントが設定されていません。\nアプリケーションを終了します。"));
			return FALSE;
		}
		// ★表示インデックスを内之浦20m, 内之浦34m, 臼田64m, 臼田54mの順にする
		m_nSelectStation = mStationViewOrder[dlgsel.m_nStation];

		bool bAuthDlg = true;
		switch (mServiceMode)
		{
		case eServiceMode_admin:
		{
			// 制御権利ダイアログは開かずに制御権利と局エリア取得
			m_nExecuteMode = eExecuteMode_Control;
			CFileAccess ac;
			mClientTerminal = ac.GetClientTerminal();
			bAuthDlg = false;
			break;
		}
		case eServiceMode_sttn:
		{
			// 制御権利ダイアログは開くが、局エリアは地上局固定
			m_nExecuteMode = eExecuteMode_Control;
			mClientTerminal = eClientSttn;
			bAuthDlg = false;
			break;
		}
		case eServiceMode_ssoc:
		{
			// 制御権利ダイアログは開くが、局エリアは相模原局固定
			m_nExecuteMode = eExecuteMode_Control;
			mClientTerminal = eClientSsoc;
			bAuthDlg = false;
			break;
		}
		case eServiceMode_user:
		default:
			// 通常処理
			break;
		}

		// 制御権利取得
#ifdef _DEBUG_NOAUTH
		m_nExecuteMode = eExecuteMode_Control;
#else
		if (bAuthDlg)
		{
			CControlAcquisition dlgmode;
			INT_PTR mode = dlgmode.DoModal();
			if (mode == IDCANCEL){
				return FALSE;
			}
			if (mode == IDOK){
				m_nExecuteMode = eExecuteMode_Control;
			}
			if (mode == IDABORT){
				m_nExecuteMode = eExecuteMode_Monitor;
			}

			mClientTerminal = dlgmode.GetClientTerminal();
		}
#endif
	}

	SetLimitedMode();	//制限つき制御モードの設定

#if 1
	// 衛星捕捉グラフの監視項目名を設定
	GetDataCollection().SetKanshiName();
#endif

	// 制御モードをFAに教えるため、制御モードをファイル書き込み
	if (m_nSelectMode == eStation_1){
		if (m_nExecuteMode == eExecuteMode_Control){
			WriteExecuteMode2FAFile(1);
		}
	}
	//============================================================================

	mIgnore = m_cmdInfo.mIgnore;


	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));
	LoadStdProfileSettings(4);  // 標準の INI ファイルのオプションをロードします (MRU を含む)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// レジストリ情報を削除する
	Unregister();

	// メイン MDI フレーム ウィンドウを作成します。
	CMainFrame* pMainFrame = new CMainFrame;
	if (theApp.GetExecuteMode() == eExecuteMode_Control)
	{
		if(theApp.GetLimitedMode() == eLimitedMode_Enable)
		{
			if (!pMainFrame || !pMainFrame->LoadFrame(IDR_STATION_1_LIMITED))
			{
				delete pMainFrame;
				return FALSE;
			}
		}
		else
		{
#ifdef MON_UT
			if (!pMainFrame || !pMainFrame->LoadFrame(IDR_STATION_1_MON_UT))
#else
			if (!pMainFrame || !pMainFrame->LoadFrame(IDR_STATION_1))
#endif /* MON_UT */
			{
				delete pMainFrame;
				return FALSE;
			}
		}
	}
	else
	{
		if (!pMainFrame || !pMainFrame->LoadFrame(IDR_STATION_1_DISABLE))
		{
			delete pMainFrame;
			return FALSE;
		}
	}
	m_pMainWnd = pMainFrame;

	// DDE、file open など標準のシェル コマンドのコマンド ラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// コマンド ラインで指定されたディスパッチ コマンドです。アプリケーションが
	// /RegServer、/Register、/Unregserver または /Unregister で起動された場合、False を返します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// アプリケーションの初期化
	if (InitializeAppl() == false){
		return FALSE;
	}

	int dbret;
	// 監視DB読込
	dbret = GetSatelliteData().GetDBAccessCls().eqmon_db_read();
	if (dbret < 0){
		AfxMessageBox(_T("監視DBの取得に失敗しました。\n監視DBを確認してください。"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// 設備制御情報DB読込
	dbret = GetSatelliteData().GetDBAccessCls().eqctl_db_read();
	if (dbret < 0){
		AfxMessageBox(_T("設備制御情報DBの取得に失敗しました。\n設備制御情報DBを確認してください。"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// 衛星固有DB読込
	dbret = GetSatelliteData().GetDBAccessCls().eqsat_db_read();
	if (dbret < 0){
		AfxMessageBox(_T("衛星固有DBの取得に失敗しました。\n衛星固有DBを確認してください。"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// レディネスチェック時間読込
	dbret = GetSatelliteData().GetDBAccessCls().rdychk_read();
	if (dbret < 0){
		//AfxMessageBox(_T("レディネスチェック時間ファイルの取得に失敗しました。\nレディネスチェック時間ファイルを確認してください。"), MB_OK | MB_ICONERROR);
		//return FALSE;
	}

	// スカイラインデータ取得
	CGraphMutex::Lock(eRadar);
	if (m_nSelectMode == eStation_1)
	{
		// 1局モード
		CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(GetSelectStation());
		if (pCtrlMonData != NULL)
		{
			// 情報更新
			pCtrlMonData->SetViewID(GetSelectStation());
			pCtrlMonData->SetStationID(GetSelectStation());
			pCtrlMonData->Gtgetskyl();
		}
	}
	else
	{
		// 4局モード
		for (int i = eStation_Usuda64; i < eStation_MAX; i++)
		{
			CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(i);
			if (pCtrlMonData != NULL)
			{
				// 情報更新
				pCtrlMonData->SetViewID(i);
				pCtrlMonData->SetStationID(i);
				pCtrlMonData->Gtgetskyl();
			}
		}
	}
	CGraphMutex::Unlock(eRadar);

	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。
	if (CreateDocTemplate(m_nSelectMode) == FALSE)
		return FALSE;

	if (m_nSelectMode == eStation_1){
		// 54mの場合は、計画時間ファイル読み込み
		if (GetSelectStation() == eStation_Usuda642)
		{
			// 計画時間読込
			dbret = GetSatelliteData().GetDBAccessCls().plntime_read();
			if (dbret < 0){
				//AfxMessageBox(_T("計画時間ファイルの取得に失敗しました。\n計画時間ファイルを確認してください。"), MB_OK | MB_ICONERROR);
				//return FALSE;
			}
		}

		// １局画面に表示するビューを開く
		for (int i = eViewType1_Trend; i < eViewType1_EquipmentHist; i++){
			OpenView(i);
			// トレンドウィンドウをAGC,RNG,ELで初期化する
			if (i == eViewType1_Trend)
				m_pMainWnd->PostMessage(eMessage_InitTrendWnd, eTrendWndSetType_AGC_RNG_EL, 0);
		}
		((CMainFrame*)pMainFrame)->SetStationMenuBar(eStation_1);

	}
	if (m_nSelectMode == eStation_5){
		// ５局画面に表示するビューを開く
		UINT count = 0;
		for (int i = eViewType5_EqOperation1; i < eViewType5_EquipmentHist; i++){
			if (i >= eViewType5_EqOperation1 && i <= eViewType5_EqOperation5){
				if (count >= m_nStationMax)
					continue;
			}
			OpenView(i);
			// トレンドウィンドウをAGC,RNG,ELで初期化する
			if (i == eViewType1_Trend)
				m_pMainWnd->PostMessage(eMessage_InitTrendWnd, eTrendWndSetType_AGC_RNG_EL, 0);
			count++;
		}
		((CMainFrame*)pMainFrame)->SetStationMenuBar(eStation_5);
	}

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED/*m_nCmdShow*/);
	pMainFrame->UpdateWindow();

	((CMainFrame*)pMainFrame)->OnTileHorz();

	((CMainFrame*)pMainFrame)->createPipe(ePIPETYPE_STATION);

	GetDataCollection().StartCollection();

	// 設備アラーム状況一覧用スレッドの作成
	CreateAlarmThread();

	// CURRENT_LBW 書き込み用IF作成
	CreateShareLbwWriteIF();

	// CheckDialo起動
	ExecCheckDialog();

	return TRUE;
}

int CStationControlApp::ExitInstance()
{
	RemoveWindowHandle(0);

	// 設備アラーム状況一覧用スレッドの終了
	DeleteAlarmThread();

	GetDataCollection().StopCollection();

	if (m_nSelectMode == eStation_1){
		if (m_nExecuteMode == eExecuteMode_Control){
			// 制御取得している場合はフラフをクリアする
			CControlAcquisition ac;
			ac.ResetControlFlag();

			// 制御モードをFAに教えるため、制御モードクリアをファイル書き込み
			WriteExecuteMode2FAFile(0);
		}
	}

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	if (m_nLogLevel < nLogEx::none){
		CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("ExitInstance"), _T("StationControl"), _T("Stop"), nLogEx::info);
		CLogTraceEx::Close();
		CLogTraceEx::Delete();
	}
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

/*============================================================================*/
/*! アプリケーション

-# ドッキングウィンドウの作成

@param
@retval BOOL

*/
/*============================================================================*/
BOOL CStationControlApp::CreateDocTemplate(UINT nStation)
{
	UINT count = 0;
	if (nStation == eStation_1){
		for (int i = eViewType1_Trend; i < eViewType1_Max; i++){
			CRuntimeClass* pViewClass;
			switch (i){
			case	eViewType1_Trend:			pViewClass = RUNTIME_CLASS(CTrendGraphView); break;
			case	eViewType1_EquipmentStatus:
				if (m_nSelectStation == eStation_Usuda64)
					// 64m
					pViewClass = RUNTIME_CLASS(CEquipmentStatusView);
				else if (m_nSelectStation == eStation_Uchinoura34)
					// 34m
					pViewClass = RUNTIME_CLASS(CEquipmentStatusView34);
				else if (m_nSelectStation == eStation_Uchinoura20)
					// 20m
					pViewClass = RUNTIME_CLASS(CEquipmentStatusView20);
				else
					// 54m
					pViewClass = RUNTIME_CLASS(CEquipmentStatusView54);
				break;
			case	eViewType1_OperationHist:	pViewClass = RUNTIME_CLASS(COperationHistoryView); break;
			case	eViewType1_ControlHist:		pViewClass = RUNTIME_CLASS(CControlHistoryView); break;
			case	eViewType1_EquipmentHist:	pViewClass = RUNTIME_CLASS(CEquipmentHistoryView); break;
			case	eViewType1_EventHist:		pViewClass = RUNTIME_CLASS(CEventHistoryView); break;
			case	eViewType1_OperationSearch:	pViewClass = RUNTIME_CLASS(CSearchView); break;
			case	eViewType1_ControlSearch:	pViewClass = RUNTIME_CLASS(CSearchView); break;
			case	eViewType1_EquipmentSearch:	pViewClass = RUNTIME_CLASS(CSearchEqView); break;
			case	eViewType1_EventSearch:		pViewClass = RUNTIME_CLASS(CSearchView); break;
			case	eViewType1_AlarmList:		pViewClass = RUNTIME_CLASS(CAlarmListFormView); break;
			default:	return TRUE;
			}
			UINT nIDResource = IDR_StationControlTYPE;
			switch (i){
			case	eViewType1_Trend:			nIDResource = IDI_ICON_TREND; break;
			case	eViewType1_EquipmentStatus:	nIDResource = IDI_ICON_STATUS; break;
			case	eViewType1_OperationHist:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType1_ControlHist:		nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType1_EquipmentHist:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType1_EventHist:		nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType1_OperationSearch:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType1_ControlSearch:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType1_EquipmentSearch:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType1_EventSearch:		nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType1_AlarmList:		nIDResource = IDI_ICON_HISTORY; break;
			default:	return TRUE;
			}

			m_pDocTemplate[i] = new CMultiDocTemplate(nIDResource,
				RUNTIME_CLASS(CStationControlDoc),
				RUNTIME_CLASS(CChildFrame), // custom MDI child frame
				pViewClass);
			if (!m_pDocTemplate[i])
				return FALSE;
			AddDocTemplate(m_pDocTemplate[i]);
			count++;
		}
	}
	if (nStation == eStation_5){
		for (int i = eViewType5_EqOperation1; i < eViewType5_Max; i++){
			if (i >= eViewType5_EqOperation1 && i <= eViewType5_EqOperation5){
				if (count >= m_nStationMax)
					continue;
			}
			CRuntimeClass* pViewClass;
			switch (i){
			case	eViewType5_EqOperation1:	pViewClass = RUNTIME_CLASS(CEquipmentStationStatusView); break;
			case	eViewType5_EqOperation2:	pViewClass = RUNTIME_CLASS(CEquipmentStationStatusView); break;
			case	eViewType5_EqOperation3:	pViewClass = RUNTIME_CLASS(CEquipmentStationStatusView); break;
			case	eViewType5_EqOperation4:	pViewClass = RUNTIME_CLASS(CEquipmentStationStatusView); break;
			case	eViewType5_EqOperation5:	pViewClass = RUNTIME_CLASS(CEquipmentStationStatusView); break;
			case	eViewType5_OperationHist:	pViewClass = RUNTIME_CLASS(COperationHistoryView); break;
			case	eViewType5_ControlHist:		pViewClass = RUNTIME_CLASS(CControlHistoryView); break;
			case	eViewType5_EquipmentHist:	pViewClass = RUNTIME_CLASS(CEquipmentHistoryView); break;
			case	eViewType5_EventHist:		pViewClass = RUNTIME_CLASS(CEventHistoryView); break;
			case	eViewType5_OperationSearch:	pViewClass = RUNTIME_CLASS(CSearchView); break;
			case	eViewType5_ControlSearch:	pViewClass = RUNTIME_CLASS(CSearchView); break;
			case	eViewType5_EquipmentSearch:	pViewClass = RUNTIME_CLASS(CSearchEqView); break;
			case	eViewType5_EventSearch:		pViewClass = RUNTIME_CLASS(CSearchView); break;
			case	eViewType5_AlarmList:		pViewClass = RUNTIME_CLASS(CAlarmListFormView); break;
			default:	return TRUE;
			}
			UINT nIDResource = IDR_StationControlTYPE;
			switch (i){
			case	eViewType5_EqOperation1:	nIDResource = IDI_ICON_SITUATION; break;
			case	eViewType5_EqOperation2:	nIDResource = IDI_ICON_SITUATION; break;
			case	eViewType5_EqOperation3:	nIDResource = IDI_ICON_SITUATION; break;
			case	eViewType5_EqOperation4:	nIDResource = IDI_ICON_SITUATION; break;
			case	eViewType5_EqOperation5:	nIDResource = IDI_ICON_SITUATION; break;
			case	eViewType5_OperationHist:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType5_ControlHist:		nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType5_EquipmentHist:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType5_EventHist:		nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType5_OperationSearch:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType5_ControlSearch:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType5_EquipmentSearch:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType5_EventSearch:		nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType5_AlarmList:		nIDResource = IDI_ICON_HISTORY; break;
			default:	return TRUE;
			}

			m_pDocTemplate[i] = new CMultiDocTemplate(nIDResource,
				RUNTIME_CLASS(CStationControlDoc),
				RUNTIME_CLASS(CChildFrame), // custom MDI child frame
				pViewClass);
			if (!m_pDocTemplate[i])
				return FALSE;
			AddDocTemplate(m_pDocTemplate[i]);
			count++;
		}
	}

	// 共通ツールウィンドウの作成
	// 運用計画タイムラインの作成
	m_pDocTemplate[eViewTypeTool_TimeLine] = new CMultiDocTemplate(IDI_ICON_TIMELINE,
		RUNTIME_CLASS(CStationControlDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTimeLineView));
	if (!m_pDocTemplate[eViewTypeTool_TimeLine])
		return FALSE;
	AddDocTemplate(m_pDocTemplate[eViewTypeTool_TimeLine]);
	count++;

	return TRUE;
}

/*============================================================================*/
/*! アプリケーション

-# アプリケーションの初期化を行う

@param
@retval

*/
/*============================================================================*/
bool CStationControlApp::InitializeAppl(bool bPathOnly/* = false*/)
{
	// 各種ファイル・フォルダの作成
	TCHAR	szFullPath[_MAX_PATH] = { 0 };
	TCHAR	szDir[_MAX_DIR] = { 0 };
	TCHAR	szDrive[_MAX_DRIVE] = { 0 };
	TCHAR	szFname[_MAX_FNAME] = { 0 };

	DWORD ret = ::GetModuleFileName(NULL, szFullPath, _MAX_PATH);

	if (ret == 0){
		AfxMessageBox(_T("モジュール名の取得に失敗しました。\n再起動してください。"), MB_OK | MB_ICONERROR);
		return false;
	}

	_wsplitpath_s(szFullPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, NULL, 0);
	m_strAppPath.Format(_T("%s%s"), szDrive, szDir);
	m_strAppInifileName.Format(_T("%s%s%s.ini"), szDrive, szDir, szFname);
	m_strAppDataPath.Format(_T("%s%sDATA"), szDrive, szDir);
	m_strAppLogPath.Format(_T("%s%sLOG\\"), szDrive, szDir);
	m_strAppWorkPath.Format(_T("%s%sWORK\\"), szDrive, szDir);
	m_strAppFaPanelPath.Format(_T("C:\\FA-Panel\\PROJECT\\"));
	m_strAppLayoutPath.Format(_T("%s%s.scl"), (LPCTSTR)m_strAppWorkPath, szFname);

	CreateDirectory(m_strAppDataPath, NULL);	//　フォルダ作成
	CreateDirectory(m_strAppLogPath, NULL);	//　フォルダ作成
	CreateDirectory(m_strAppWorkPath, NULL);	//　フォルダ作成

	DeleteFile(m_strAppLayoutPath);				// テンポラリのレイアウトファイルを削除しておく

	GetDrawUpdateInterval(true);

	m_nStationMax = GetPrivateProfileInt(SECTION_SYSTEM, KEY_STATIONMAX, eStation_MAX, m_strAppInifileName);
	TCHAR buf[MAX_PATH];
	GetPrivateProfileString(SECTION_SYSTEM, KEY_FILESERVER_NAME, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strFileServerName = buf;

	m_strManagementPath.Format(_T("\\\\%s\\share\\%s\\"), (LPCTSTR)theApp.GetFileServerName(), (LPCTSTR)mManagementPath);
	if (m_strFileServerName.IsEmpty() == true){
		m_strManagementPath.Format(_T("C:\\share\\%s\\"), (LPCTSTR)mManagementPath);
	}

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHSERVER_NAME, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strSshServerName = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHUSER_NAME, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strSshUserName = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHKEY_PATH, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strSshKeyPath = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHBATCH_PATH, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strBatchPath = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_EVENTTONE_PATH, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strEventTonePath = buf;
	if (m_strEventTonePath.IsEmpty()){
		m_strEventTonePath.Format(_T("%s%sEventTone\\"), szDrive, szDir);
		CreateDirectory(m_strEventTonePath, NULL);	//　フォルダ作成
	}
	if (m_strEventTonePath.Right(1) != _T("\\")){
		m_strEventTonePath += _T("\\");
	}
	CString str;
	str.Format(_T("%s"), (LPCTSTR)m_strEventTonePath);
	WritePrivateProfileString(SECTION_SYSTEM, KEY_EVENTTONE_PATH, str, m_strAppInifileName);

	// ネットワークドライブの確立
	//CreateNetworkDrive();
	if (m_strFileServerName.IsEmpty() == false){
		m_strFileServerName += _T("\\");
	}

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SERVICE_MODE, _T(""), buf, MAX_PATH, m_strAppInifileName);
	CString strServiceIni = buf;
	mServiceMode = eServiceMode_user;	// サービスのデフォルトはユーザモード
	for (int i = 0; i < eServiceMode_max; i++)
	{
		if (strServiceIni.CompareNoCase(mServiceModeString[i]) == 0)
		{
			mServiceMode = i;
			break;
		}
	}

	// PRED DIFF設定
	m_nPredDiffEnable = GetPrivateProfileInt(SECTION_SYSTEM, KEY_PREDDIFF_ENABLE, 1, m_strAppInifileName);
	GetPrivateProfileString(SECTION_SYSTEM, KEY_PREDDIFF_MIN, DEFAULT_PREDDIFFMIN, buf, MAX_PATH, m_strAppInifileName);
	m_dPredDiffMin = _ttof(buf);

	// 54m 履歴識別番号設定
	GetPrivateProfileString(SECTION_SYSTEM, KEY_HISTID54, DEFAULT_HISTID54, buf, MAX_PATH, m_strAppInifileName);
	DWORD nHistID54 = ::_tcstol(buf, NULL, 16);
	theApp.GetDataCollection().SetHistID54(nHistID54);

	if (bPathOnly == true)
		return true;

	m_nMessageThread = GetPrivateProfileInt(SECTION_SYSTEM, KEY_MESSAGETHREAD, 1, m_strAppInifileName);
	str.Format(_T("%d"), m_nMessageThread);
	WritePrivateProfileString(SECTION_SYSTEM, KEY_MESSAGETHREAD, str, m_strAppInifileName);

	m_nLogLevel = GetPrivateProfileInt(SECTION_SYSTEM, KEY_LOGLEVEL, nLogEx::fatal, m_strAppInifileName);
	CLogTraceEx::Create(m_strAppLogPath, _T("StationControl"), m_nLogLevel, nLogEx::text);

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("InitializeAppl"), _T("StationControl"), _T("Start"), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

#if 0
	// 衛星捕捉グラフの監視項目名を設定
	GetDataCollection().SetKanshiName();
#endif

	// サマリ追尾グラフの局設定
	GetSatelliteData().SetSummarySpectrumStation();

	// 予報値一覧ファイルよりデータ取得(EUC対応)
	CGraphMutex::Lock(eFile);
	GetSatelliteData().SetPredList_Euc();
	CGraphMutex::Unlock(eFile);

	// 計画一覧ファイルよりデータ取得(EUC対応)
	CGraphMutex::Lock(eFile);
	GetSatelliteData().SetPlanList_Euc();
	CGraphMutex::Unlock(eFile);

	// CIDファイルよりデータ取得
	if (GetSatelliteData().ReadCIDFile()==FALSE){
		AfxMessageBox(_T("CIDファイルの取得に失敗しました。\nCIDファイルを確認してください。"), MB_OK | MB_ICONERROR);
		return false;
	}

	// サマリグラフのプロパティデータ作成
	GetSatelliteData().CreateSummaryPropertiesData();

	// RNG伝搬モニタの初期化
	GetSatelliteData().GetRangePropagation().InitStatus();

	// 古いセッション情報ファイルを削除する
	GetSatelliteData().DeleteSessionStatusFile();

	// RFトリップ角定義ファイルよりデータ取得
	GetSatelliteData().ReadRFTripFile();

	return true;
}

// CStationControlApp メッセージ ハンドラー


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
public:
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

// ダイアログを実行するためのアプリケーション コマンド
void CStationControlApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CStationControlApp のカスタマイズされた読み込みメソッドと保存メソッド

void CStationControlApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CStationControlApp::LoadCustomState()
{
}

void CStationControlApp::SaveCustomState()
{
}

/*============================================================================*/
/*! アプリケーション

-# 指定されたビューを開く

@param	val			ウィンドウ種別
@retval

*/
/*============================================================================*/
void CStationControlApp::OpenView(UINT val)
{
	// ウィンドウ種別
	SetWindowType(val, true);

	//CStationControlDoc* pdoc = (CStationControlDoc*)m_pDocTemplate[val]->m_pDocClass;

	// 指定のウィンドウを開く
	m_nOpenViewType = val;
	CStationControlDoc* pdoc = (CStationControlDoc*)m_pDocTemplate[val]->OpenDocumentFile(NULL);
}

/*============================================================================*/
/*! アプリケーション

-# 指定されたビューを閉じる

@param	val			ウィンドウ種別
@retval

*/
/*============================================================================*/
void CStationControlApp::CloseView(UINT val)
{
	// ウィンドウ種別
	SetWindowType(val, false);
	// 指定のウィンドウを閉じる
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
/*! アプリケーション

-# 指定されたビューを取得する

@param	val			ウィンドウ種別
@retval

*/
/*============================================================================*/
CView* CStationControlApp::GetView(UINT val)
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

/*============================================================================*/
/*! アプリケーション

-# 指定されたビューを取得する

@param		lParam		各種ダイアログ、ウィンドウハンドル
@param		wParam		各種ダイアログ(IDD)、ウィンドウパラメタ
@retval

*/
/*============================================================================*/
void CStationControlApp::AddWindowHandle(LPARAM lParam, WPARAM wParam/* = 0*/)
{
	if (wParam != 0){
		map<LPARAM, WPARAM>::iterator itr;
		for (itr = mWindowList.begin(); itr != mWindowList.end(); itr++){
			if (wParam == (WPARAM)((*itr).second)){
				RemoveWindowHandle((LPARAM)(*itr).first);
				break;
			}
		}
	}

	map<LPARAM, WPARAM>::iterator itr;
	itr = mWindowList.find(lParam);
	if (itr == mWindowList.end()){
		mWindowList.insert(pair<LPARAM, WPARAM>(lParam, wParam));
	}
}

/*============================================================================*/
/*! アプリケーション

-# 指定されたビューを取得する

@param		lParam		各種ダイアログ、ウィンドウハンドル
@param		wParam		各種ダイアログ(IDD)、ウィンドウパラメタ
@retval

*/
/*============================================================================*/
CWnd* CStationControlApp::GetWindowHandle(WPARAM wParam/* = 0*/)
{
	if (wParam != 0){
		map<LPARAM, WPARAM>::iterator itr;
		for (itr = mWindowList.begin(); itr != mWindowList.end(); itr++){
			if (wParam == (WPARAM)((*itr).second)){
				CWnd* p = (CWnd*)(*itr).first;
				return p;
			}
		}
	}
	return NULL;
}

/*============================================================================*/
/*! アプリケーション

-# 指定されたビューを取得する

@param		lParam		各種ダイアログ、ウィンドウハンドル(0の場合は全て削除)
@retval

*/
/*============================================================================*/
void CStationControlApp::RemoveWindowHandle(LPARAM lParam)
{
	map<LPARAM, WPARAM>::iterator itr;
	if (lParam == 0){
		for (itr = mWindowList.begin(); itr != mWindowList.end(); itr++){
			CWnd* p = (CWnd*)(*itr).first;
			if ((p != NULL) && (p->GetSafeHwnd())){
				p->DestroyWindow();
			}
			delete p;
		}
		mWindowList.clear();
		return;
	}

	itr = mWindowList.find(lParam);
	if (itr != mWindowList.end()){
		CWnd* p = (CWnd*)(*itr).first;
		if (p->GetSafeHwnd()){
			p->DestroyWindow();
		}
		delete p;
		mWindowList.erase(itr);
	}
}

/*============================================================================*/
/*! アプリケーション

-# 指定されたウィンドウの種別を取得する

@param		pWnd	ウィンドウハンドル
@retval

*/
/*============================================================================*/
UINT CStationControlApp::GetStationWindowType(CWnd* pWnd)
{
	// 主要ビューの確認
	if (m_nSelectMode == eStation_1){
		// １局画面に表示するビューを開く
		for (UINT i = eViewType1_Trend; i < eViewType1_Max; i++){
			CView *p = GetView(i);
			if (p != NULL){
				if (p->GetParent() == pWnd)
					return i;
			}
		}
	}
	if (m_nSelectMode == eStation_5){
		// ５局画面に表示するビューを開く
		for (UINT i = eViewType5_EqOperation1; i < eViewType5_Max; i++){
			CView *p = GetView(i);
			if (p != NULL){
				if (p->GetParent() == pWnd)
					return i;
			}
		}
	}

	// 個別ダイアログ等の確認
	map<LPARAM, WPARAM>::iterator itr;
	for (itr = mWindowList.begin(); itr != mWindowList.end(); itr++){
		if (pWnd == (CWnd*)((*itr).first)){
			return (UINT)(*itr).second;
		}
	}

	for (UINT i = eViewTypeTool_TimeLine; i < eViewType_Max; i++){
		CView *p = GetView(i);
		if (p != NULL){
			if (p->GetParent() == pWnd)
				return i;
		}
	}

	return 0xFFFFFFFF;
}

// CStationControlApp メッセージ ハンドラー

BOOL CStationControlApp::PreTranslateMessage(MSG* pMsg)
{
	CView* p;
	if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_NCLBUTTONDOWN){
		if (m_nSelectMode == eStation_1){
			for (int i = eViewType1_Trend; i < eViewType1_Max; i++){
				if (i == eViewType1_OperationHist || i == eViewType1_ControlHist || i == eViewType1_EquipmentHist || i == eViewType1_EventHist){
					p = GetView(i);
					if (p != NULL){
						if (i == eViewType1_OperationHist)
							((COperationHistoryView*)p)->CloseChildWindow(pMsg);
						if (i == eViewType1_ControlHist)
							((CControlHistoryView*)p)->CloseChildWindow(pMsg);
						if (i == eViewType1_EquipmentHist)
							((CEquipmentHistoryView*)p)->CloseChildWindow(pMsg);
						if (i == eViewType1_EventHist)
							((CEventHistoryView*)p)->CloseChildWindow(pMsg);
					}
				}
				if (i == eViewType1_OperationSearch || i == eViewType1_ControlSearch || i == eViewType1_EquipmentSearch || i == eViewType1_EventSearch){
					p = GetView(i);
					if (p != NULL){
						if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == p->m_hWnd){
							continue;
						}
					}
					if (p != NULL){
						//if (i == eViewType1_OperationSearch)
						//	((COperationSearchView*)p)->CloseChildWindow(pMsg);
						//if (i == eViewType1_ControlSearch)
						//	((CControlSearchView*)p)->CloseChildWindow(pMsg);
						//if (i == eViewType1_EquipmentSearch)
						//	((CEquipmentSearchView*)p)->CloseChildWindow(pMsg);
						//if (i == eViewType1_EventSearch)
						//	((CEventSearchView*)p)->CloseChildWindow(pMsg);
					}
				}
			}
		}
		if (m_nSelectMode == eStation_5){
			for (int i = eViewType5_EqOperation1; i < eViewType5_Max; i++){
				if (i == eViewType5_OperationHist || i == eViewType5_ControlHist || i == eViewType5_EquipmentHist || i == eViewType5_EventHist){
					p = GetView(i);
					if (p != NULL){
						if (i == eViewType5_OperationHist)
							((COperationHistoryView*)p)->CloseChildWindow(pMsg);
						if (i == eViewType5_ControlHist)
							((CControlHistoryView*)p)->CloseChildWindow(pMsg);
						if (i == eViewType5_EquipmentHist)
							((CEquipmentHistoryView*)p)->CloseChildWindow(pMsg);
						if (i == eViewType5_EventHist)
							((CEventHistoryView*)p)->CloseChildWindow(pMsg);
					}
				}
				if (i == eViewType5_OperationSearch || i == eViewType5_ControlSearch || i == eViewType5_EquipmentSearch || i == eViewType5_EventSearch){
					p = GetView(i);
					if (p != NULL){
						//if (i == eViewType5_OperationSearch)
						//	((COperationSearchView*)p)->CloseChildWindow(pMsg);
						//if (i == eViewType5_ControlSearch)
						//	((CControlSearchView*)p)->CloseChildWindow(pMsg);
						//if (i == eViewType5_EquipmentSearch)
						//	((CEquipmentSearchView*)p)->CloseChildWindow(pMsg);
						//if (i == eViewType5_EventSearch)
						//	((CEventSearchView*)p)->CloseChildWindow(pMsg);
					}
				}
			}
		}
	}

	return CWinAppEx::PreTranslateMessage(pMsg);
}

void CStationControlApp::OnFilePrintSetup()
{
	CPrintDialog* printDlg = NULL;
	printDlg = new CPrintDialog(FALSE, PD_DISABLEPRINTTOFILE);
	if (printDlg->DoModal() == IDCANCEL){
		delete printDlg;
		return;
	}
	m_strDefaultPrinterName = printDlg->GetDeviceName();
	delete printDlg;
}


/*============================================================================*/
/*! アプリケーション

-# 現在時刻の設定

@brief 現在時刻（UTC）を取得する

@param	time	UTC時間
@retval なし
*/
/*============================================================================*/
void CStationControlApp::GetCurrentUtcTime(CTime& time)
{
	// 現在時刻の描画
	SYSTEMTIME st_UTC;
	GetSystemTime(&st_UTC);
	time = CTime(st_UTC);
}

/*============================================================================*/
/*! アプリケーション

-#  自IPアドレスの取得

@param

@retval

*/
/*============================================================================*/
DWORD CStationControlApp::GetHostIpInfo()
{
	char 		host_name[256];				// ホストの名前
	memset(host_name, 0, sizeof(host_name));
	DWORD	ipAddress = 0;

	// ホスト名の取得
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
/*! アプリケーション

-#  自端末番号の取得

@param　なし
@retval 端末番号

*/
/*============================================================================*/
UINT CStationControlApp::GetTermNo()
{
	char 		host_name[256];				// ホストの名前
	memset(host_name, 0, sizeof(host_name));
	UINT	ret = 0;

	// ホスト名の取得
	if (gethostname(host_name, 256) == 0){
		CString strHostName(host_name);
		CString strRight2 = strHostName.Right(2);
		UINT termNo = (UINT)(_ttol(strRight2));
		if (termNo >= 0 && termNo <= 255)
		{
			if (theApp.GetSelectStation() == eStation_Usuda642)
			{
				if (termNo >= 1 && termNo <= 7 ||
					termNo >= 11 && termNo <= 13)
				// 54m端末番号 末尾01～07:54m局内、末尾11～13:64m局内
				ret = (termNo / 10) * 7 + termNo % 10;
			}
			else
			{
				// 64m, 34m, 20m端末番号
				ret = termNo;
			}
		}
	}

	return ret;
}


/*============================================================================*/
/*! アプリケーション

-# ファイルパスの取得

@brief 共有フォルダのファイルパスまたはフォルダパスを取得する

@param	fileType	ファイルタイプ
@param	nStation	局番号（省略時は、１局画面で省略時は自局を、５局画面では臼田64m局を返す）	
@retval ファイルパスまたはフォルダパス
*/
/*============================================================================*/
CString CStationControlApp::GetShareFilePath(UINT fileType, UINT nStation/* = eStation_MAX*/)
{
	// 局番号の引数省略時
	if (nStation == eStation_MAX)
	{
		// 局モードチェック
		switch (m_nSelectMode)
		{
		case eStation_1:
			// 自局番号をセット
			nStation = m_nSelectStation;
			break;

		case eStation_5:
			// 臼田64mをセット
			nStation = eStation_Usuda64;
			break;

		case eStation_10:
			// 10局モード
			break;

		default:
			break;
		}
	}

	if (nStation >= eStation_MAX)
	{
		// 臼田64mをセット
		nStation = eStation_Usuda64;
	}

	// 局番号とファイルタイプから局別ファイル・フォルダパスを取得
	CString strFilePath;
	//	strFilePath.Format(_T("%s%s"), mNetworkDrive[nStation], mSvFileFolder[fileType]);
	strFilePath.Format(_T("\\\\%sshare\\%s\\%s"), (LPCTSTR)theApp.GetFileServerName(), mStationFolder[nStation], mSvFileFolder[fileType]);

	if (m_strFileServerName.IsEmpty() == true){
		strFilePath.Format(_T("C:\\share\\%s\\%s"), mStationFolder[nStation], mLocalFileFolder[fileType]);
	}

	return strFilePath;
}

/*============================================================================*/
/*! アプリケーション

-# チェックダイアログボックスファイルパスの取得

@brief 共有フォルダのファイルパスまたはフォルダパスを取得する

@param	nStation	局番号（省略時は、１局画面で省略時は自局を、５局画面では臼田64m局を返す）
@retval ファイルパスまたはフォルダパス
*/
/*============================================================================*/
CString CStationControlApp::GetCheckDialogPath(UINT nStation)
{
	// 局番号とファイルタイプから局別ファイル・フォルダパスを取得
	CString strFilePath;
#if 1
	strFilePath.Format(_T("\\\\%sshare\\DialogReq\\"), (LPCTSTR)theApp.GetFileServerName());

	if (m_strFileServerName.IsEmpty() == true)
	{
		strFilePath = _T("C:\\share\\DialogReq\\");
	}
#else
	//	strFilePath.Format(_T("%s%s"), mNetworkDrive[nStation], mSvFileFolder[fileType]);
	strFilePath.Format(_T("\\\\%sshare\\DialogReq\\%s\\"), (LPCTSTR)theApp.GetFileServerName(), mStationFolder[nStation]);

	if (m_strFileServerName.IsEmpty() == true){
		strFilePath.Format(_T("C:\\share\\DialogReq\\%s\\"), mStationFolder[nStation]);
	}
#endif

	return strFilePath;
}


/*============================================================================*/
/*! アプリケーション

-# チェックダイアログボックス起動

@param	なし
@retval なし
*/
/*============================================================================*/
void CStationControlApp::ExecCheckDialog()
{
	CString bat;
	CString stop;

	// 1局モード時のみ実行
	if (m_nSelectMode != eStation_1 || m_nExecuteMode != eExecuteMode_Control)
	{
		return;
	}

	// チェックダイアログ起動ファイル名取得
	CString dir = GetCheckDialogPath(m_nSelectStation);
	CString tmp = CString(mStationSimpleString[m_nSelectStation]).MakeLower();
	bat.Format(_T("C:\\sos\\opr\\prog\\ap\\Dialog\\CheckDialog_%s"), (LPCTSTR)tmp);

	// Stopファイルが存在するなら削除する。
	stop = dir + tmp + _T("\\") + _T("Stop");
	if (PathFileExists(stop) == TRUE)
	{
		DeleteFile(stop);
	}

	// 実行
	ShellExecute(NULL, _T("open"), (LPTSTR)bat.GetBuffer(), NULL, NULL, SW_HIDE);
}


/*============================================================================*/
/*! アプリケーション

-# チェックダイアログボックス終了

@brief Stopファイルを作成する

@param	なし
@retval なし
*/
/*============================================================================*/
void CStationControlApp::StopCheckDialog()
{
	CString stop;

	// 1局モード時のみ実行
	if (m_nSelectMode != eStation_1 || m_nExecuteMode != eExecuteMode_Control)
	{
		return;
	}

	// チェックダイアログ起動ファイル名取得
	CString dir = GetCheckDialogPath(m_nSelectStation);
#if 1
	CString tmp = CString(mStationSimpleString[m_nSelectStation]).MakeLower();
	stop = dir + tmp + _T("\\") + _T("Stop");
#else
	stop = dir + _T("Stop");
#endif

	// 実行
	// STOPファイル作成
	FILE *fp = NULL;
	if (_wfopen_s(&fp, stop, _T("wt")) == 0 && fp != NULL)
	{
		fclose(fp);
	}
}


/*============================================================================*/
/*! アプリケーション

-# 校正診断用AGC校正ファイル取得要求

@param	statin		: 局名
@param	satellite	: 衛星名
@param	file		: 取得要求ファイル名
@param	bWaitComplete：TRUE:シェル終了を待つ（デフォルト） / FALSE:シェル終了を待たない
@retval	なし

*/
/*============================================================================*/
void CStationControlApp::GetCalib(CString station, CString satellite, CString cust, CString dflt, BOOL bWaitComplete /*= TRUE*/)
{
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = AGCCALIB_GET_SH;

	stParam.strRemShellParam.Format(_T("%s %s %s %s"), (LPCTSTR)station, (LPCTSTR)satellite.MakeLower(), (LPCTSTR)cust, (LPCTSTR)dflt);

	CString msg;
	msg.Format(_T("GetCalib(%s)!!!"), (LPCTSTR)stParam.strRemShellName);
	CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("GetCalib"), msg, _T(""), nLogEx::debug);

//	CDlgWaitCmd wait;
//	wait.Open(_T("校正診断"), _T("AGC校正テーブルファイル取得中。\nしばらくお待ちください。"), false, true);
	int err = CSshShell::ExecShell(stParam, bWaitComplete);
//	wait.Close();

	CString strDebug;
	strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
	CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("GetCalib"), _T("debug"), strDebug, nLogEx::debug);

	if (err != 0)
	{
		AfxMessageBox(_T("AGC校正テーブルファイルの読み込みに失敗しました。"), MB_OK | MB_ICONWARNING);
	}
}

/*============================================================================*/
/*! アプリケーション

-# 校正診断用AGC校正ファイル転送要求

@param	statin		: 局名
@param	satellite	: 衛星名
@param	file		: 転送要求ファイル名
@param	bWaitComplete：TRUE:シェル終了を待つ（デフォルト） / FALSE:シェル終了を待たない
@retval	なし

*/
/*============================================================================*/
void CStationControlApp::RequestCalib(CString station, CString satellite, CString file, CString ctrl, BOOL bWaitComplete /*= TRUE*/)
{
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = AGCCALIB_REQ_SH;

	stParam.strRemShellParam.Format(_T("%s %s %s %s"), (LPCTSTR)station, (LPCTSTR)satellite.MakeLower(), (LPCTSTR)file, (LPCTSTR)ctrl);

	CString msg;
	msg.Format(_T("GetCalib(%s)!!!"), (LPCTSTR)stParam.strRemShellName);
	CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("RequestCalib"), msg, _T(""), nLogEx::debug);

	int err = CSshShell::ExecShell(stParam, bWaitComplete);

	CString strDebug;
	strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
	CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("RequestCalib"), _T("debug"), strDebug, nLogEx::debug);

#if 0
	if (err != 0)
	{
		msg.Format(_T("AGC校正テーブルファイルの転送に失敗しました。\n%s"), file);
		AfxMessageBox(msg, MB_OK | MB_ICONWARNING);
	}
#endif
}

/*============================================================================*/
/*! アプリケーション

-# 制御送信

@brief	制御送信

@param	val1: 制御対象
@param	val2~4: パラメータ
@param	bWaitComplete：TRUE:シェル終了を待つ（デフォルト） / FALSE:シェル終了を待たない
@retval	なし

*/
/*============================================================================*/
void CStationControlApp::SendControl(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete /*= TRUE*/)
{
	if (theApp.GetSelectStationMode() == eStation_5)
		return;

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
	CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("SendControl"), msg, _T(""), nLogEx::info);

	int err = CSshShell::ExecShell(stParam, bWaitComplete);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("SendControl"), _T("debug"), strDebug, nLogEx::debug);
	}
}

/*============================================================================*/
/*! アプリケーション

-# 制御送信

@brief	制御送信

@param	val1: 制御対象
@param	val2~4: パラメータ
@param	bWaitComplete：TRUE:シェル終了を待つ（デフォルト） / FALSE:シェル終了を待たない
@retval	なし

*/
/*============================================================================*/
void CStationControlApp::SendControl2(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete /*= TRUE*/)
{
	if (theApp.GetSelectStationMode() == eStation_5)
		return;

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
	CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("SendControl"), msg, _T(""), nLogEx::info);

	int err = CSshShell::ExecShell(stParam, bWaitComplete);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("SendControl"), _T("debug"), strDebug, nLogEx::debug);
	}
}

void CStationControlApp::SendControlNoUI(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete /*= TRUE*/)
{
	if (theApp.GetSelectStationMode() == eStation_5)
		return;

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
	msg.Format(_T("SendControlNoUI(%s)!!!"), (LPCTSTR)stParam.strRemShellName);
	CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("SendControlNoUI"), msg, _T(""), nLogEx::info);

	int err = CSshShell::ExecShell(stParam, bWaitComplete);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("SendControlNoUI"), _T("debug"), strDebug, nLogEx::debug);
	}
}

void CStationControlApp::SendControlAgcTblLoad(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete /*= TRUE*/)
{
	if (theApp.GetSelectStationMode() == eStation_5)
		return;

	CString ctrl = val1;
	if (val2 != _T(""))
	{
		if ( ctrl != _T(""))
		{
			ctrl += _T(" ");
		}
		ctrl += val2;

		if (val3 != _T(""))
		{
			if (ctrl != _T(""))
			{
				ctrl += _T(" ");
			}
			ctrl += val3;

			if (val4 != _T(""))
			{
				if (ctrl != _T(""))
				{
					ctrl += _T(" ");
				}
				ctrl += val4;
			}
		}
	}

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = AGCTBL_LOAD_SH;

	if (theApp.GetSelectStation() == eStation_Usuda64)
	{
		if (ctrl != _T(""))
			stParam.strRemShellParam.Format(_T("udsc64 %s"), (LPCTSTR)ctrl);
		else
			stParam.strRemShellParam.Format(_T("udsc64"));
	}
	else if (theApp.GetSelectStation() == eStation_Usuda642)
	{
		if (ctrl != _T(""))
			stParam.strRemShellParam.Format(_T("udsc54 %s"), (LPCTSTR)ctrl);
		else
			stParam.strRemShellParam.Format(_T("udsc54"));
	}
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
	{
		if (ctrl != _T(""))
			stParam.strRemShellParam.Format(_T("usc34 %s"), (LPCTSTR)ctrl);
		else
			stParam.strRemShellParam.Format(_T("usc34"));
	}
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
	{
		if (ctrl != _T(""))
			stParam.strRemShellParam.Format(_T("usc20 %s"), (LPCTSTR)ctrl);
		else
			stParam.strRemShellParam.Format(_T("usc20"));
	}

	CString msg;
	msg.Format(_T("SendControlAgcTblLoad(%s)!!!"), (LPCTSTR)stParam.strRemShellName);
	CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("SendControlAgcTblLoad"), msg, _T(""), nLogEx::info);

	int err = CSshShell::ExecShell(stParam, bWaitComplete);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("SendControlAgcTblLoad"), _T("debug"), strDebug, nLogEx::debug);
	}
}

/*============================================================================*/
/*! アプリケーション

-# 制御送信

@brief	制御送信

@param	val1: 制御対象
@param	val2~: パラメータ
@retval	TRUE: 制御送信実行 / FALSE: キャンセル

*/
/*============================================================================*/
BOOL CStationControlApp::SendControlChkDlg(CString val1, CString val2, CString val3, CString val4)
{
	CString strText;
	strText.Format(_T("制御名:%s\n制御を実行しますか？"), (LPCTSTR)val1);

	if (IDCANCEL == MessageBoxHooked(m_pMainWnd->m_hWnd, strText, _T("設備詳細制御"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return FALSE;

	SendControl(val1, val2, val3, val4, FALSE);

	return TRUE;
}

/*============================================================================*/
/*! アプリケーション

-# 制御送信

@brief	制御送信（２つの制御に同値を送信）

@param	val1: 制御対象1
@param	val2: 制御対象2
@param	val3~: パラメータ
@retval	TRUE: 制御送信実行 / FALSE: キャンセル

*/
/*============================================================================*/
BOOL CStationControlApp::SendControlSameNumChkDlg(CString val1, CString val2, CString val3, CString val4, CString val5)
{
	CString strText;
	strText.Format(_T("制御名:%s\n制御名:%s\n制御を実行しますか？"), (LPCTSTR)val1, (LPCTSTR)val2);

	if (IDCANCEL == MessageBoxHooked(m_pMainWnd->m_hWnd, strText, _T("設備詳細制御"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return FALSE;

	SendControl(val1, val3, val4, val5, TRUE);
	SendControl(val2, val3, val4, val5, FALSE);

	return TRUE;
}

/*============================================================================*/
/*! アプリケーション

-# 制御送信

@brief	制御送信

@param	val1: 制御対象
@param	val2~: パラメータ
@retval	TRUE: 制御送信実行 / FALSE: キャンセル

*/
/*============================================================================*/
BOOL CStationControlApp::SendControlChkDlg2(CString val1, CString val2, CString val3, CString val4)
{
	CString strText;
	strText.Format(_T("制御名:%s, %s\n制御を実行しますか？"), (LPCTSTR)val1, (LPCTSTR)val3);

	if (IDCANCEL == MessageBoxHooked(m_pMainWnd->m_hWnd, strText, _T("設備詳細制御"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return FALSE;

	SendControl2(val1, val2, val3, val4, FALSE);

	return TRUE;
}


/*============================================================================*/
/*! アプリケーション

-# 制御送信

@brief	制御送信

@param	val1: 制御対象
@param	val2~: パラメータ
@retval	TRUE: 制御送信実行 / FALSE: キャンセル

*/
/*============================================================================*/
BOOL CStationControlApp::SendControlNoUIChkDlg(CString val1, CString val2, CString val3, CString val4)
{
	CString strText;
	strText.Format(_T("制御名:%s\n制御を実行しますか？"), (LPCTSTR)val1);

	if (IDCANCEL == MessageBoxHooked(m_pMainWnd->m_hWnd, strText, _T("設備詳細制御"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return FALSE;

	SendControlNoUI(val1, val2, val3, val4, FALSE);

	return TRUE;
}


/*============================================================================*/
/*! アプリケーション

-# 制御送信(CURRENT_LBW)

@brief	制御送信

@param	val1: 制御対象
@param	val2~: パラメータ
@retval	TRUE: 制御送信実行 / FALSE: キャンセル

*/
/*============================================================================*/
BOOL CStationControlApp::SendControlDlgCURRENT_LBW(CString val1, CString val2, CString val3, CString val4)
{
	CString strText;
	strText.Format(_T("制御名:%s\n制御を実行しますか？"), (LPCTSTR)val1);

	if (IDCANCEL == MessageBoxHooked(m_pMainWnd->m_hWnd, strText, _T("設備詳細制御"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return FALSE;

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();
//	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), CString(EISEI_NAME));
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), CString(VEQU_EISEI_NAME));
	CString strSatellite;
	if (obs_adr == NULL)
	{
		strSatellite = _T("");
	}
	else
	{
		strSatellite = CString(obs_adr->sz_sts_name);
	}

//#ifdef _DEBUG
//	strSatellite = _T("GETAIL");
//	strSatellite = _T("       ");
//#endif
	CString strSatelliteTmp = strSatellite;
	strSatelliteTmp.Trim(_T(" "));
	if (strSatelliteTmp == _T(""))
	{
		AfxMessageBox(_T("カレントの衛星がないため、AGC校正テーブル読み込みを実行できません。"));
		return FALSE;
	}

	if (GetSelectStationMode() == eStation_1 && theApp.GetSelectStation() == eStation_Uchinoura20)
	{
		// 20m
		// テーブルのメモリ展開処理
		SendControlAgcTblLoad(strSatellite, _T(""), val3, val4, TRUE);
	}
	else
	{
		// 20m以外
		// テーブルファイルをコピーするシェルを送る
		SendControlAgcTblLoad(strSatellite, val1, val3, val4, TRUE);
	}

	return TRUE;
}


/*============================================================================*/
/*! アプリケーション

-# 制御送信(PLL BW)

@brief	制御送信

@param	val1: 制御対象
@param	val2~: パラメータ
@retval	TRUE: 制御送信実行 / FALSE: キャンセル

*/
/*============================================================================*/
BOOL CStationControlApp::SendControlDlgPLL_BW(CString val1, CString val2, CString val3, CString val4)
{
	CString strText;
	strText.Format(_T("制御名:%s\n制御を実行しますか？"), (LPCTSTR)val1);

	if (IDCANCEL == MessageBoxHooked(m_pMainWnd->m_hWnd, strText, _T("設備詳細制御"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return FALSE;

	// 制御を送る
	SendControl(val1, val2, val3, val4, TRUE);

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();
//	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), CString(EISEI_NAME));
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), CString(VEQU_EISEI_NAME));
	CString strSatellite;
	if (obs_adr == NULL)
	{
		strSatellite = _T("");
	}
	else
	{
		strSatellite = CString(obs_adr->sz_sts_name);
	}

//#ifdef _DEBUG
//	strSatellite = _T("GETAIL");
//	strSatellite = _T(" ");
//#endif
	CString strSatelliteTmp = strSatellite;
	strSatelliteTmp.Trim(_T(" "));
	if (strSatelliteTmp == _T(""))
	{
		AfxMessageBox(_T("カレントの衛星がないため、AGC校正テーブル読み込みを実行できません。"));
		return FALSE;
	}

	if (GetSelectStationMode() == eStation_1 && theApp.GetSelectStation() == eStation_Uchinoura20)
	{
		// 20m
		// テーブルのメモリ展開処理
		SendControlAgcTblLoad(strSatellite, _T(""), val3, val4, TRUE);
	}
	else
	{
		// 20m以外
		// テーブルファイルをコピーするシェルを送る
		SendControlAgcTblLoad(strSatellite, val1, val3, val4, TRUE);
	}

	return TRUE;
}


/*============================================================================*/
/*! アプリケーション

-# 運用アカウント設定ファイルを読み込み、起動ユーザーの局アクセス状況を取得する

@param
@retval	局アクセス状況

*/
/*============================================================================*/
BYTE CStationControlApp::GetOperationAccess()
{
	TCHAR buf[100];
	DWORD dw = 100;
	GetUserName(buf, &dw);
	CString strLoginUser = buf;

	vector<stAccount> val;
	CFile file;
	CString name;
	val.clear();

	CFileAccess ac;
	ac.ReadOperationAccount(val);

	CString strSid;
	getUserSid(strLoginUser, strSid);

	BYTE ret = 0;
	vector<stAccount>::iterator itr;
	for (itr = val.begin(); itr != val.end(); itr++){
		if ((*itr).enable == false){
			// 削除済なので何もしない
			continue;
		}

		if (strLoginUser.CompareNoCase((*itr).user) == 0 && strSid == (*itr).usersid){
			// ログインユーザの設定が見つかった
			ret |= ((*itr).udsc64 == true) ? 0x01 : 0;
			ret |= ((*itr).xdsc54 == true) ? 0x02 : 0;
			ret |= ((*itr).usc34 == true) ? 0x04 : 0;
			ret |= ((*itr).usc20 == true) ? 0x08 : 0;
			break;
		}
	}

	return ret;
}

/*============================================================================*/
/*! 制御権利管理

-# パスワードのエンコード,デコード

@param
@retval

*/
/*============================================================================*/
CString CStationControlApp::ManEncodeDecode(CString str)
{
	// 8文字固定
	char tobuf[mMaxPassword + 1];
	char frombuf[mMaxPassword + 1];
	sprintf_s((char*)tobuf, mMaxPassword + 1, "%s", (LPCSTR)CStringA(str));
	memset(frombuf, 0, mMaxPassword + 1);

	short* toptr = (short*)tobuf;
	short* fromptr = (short*)frombuf;

	for (int i = 0; i < mMaxPassword / 2; i++){
		*fromptr = htons(*toptr);
		toptr++;
		fromptr++;
	}
	CString ret = CString(frombuf);
	return ret;
}

bool CStationControlApp::getUserSid(CString& strUserName, CString& strSid)
{
	// ローカルアカウントのSIDを取得する
	DWORD			cbDomain = 0;
	DWORD			cbUserSID = 0;
	SID_NAME_USE	snuType;
	TCHAR			*pszDomain = NULL;
	LPVOID			pUserSID = NULL;
	BOOL ret;

	strSid.Empty();

	// 最初の実行で各種サイズを取得する
	ret = ::LookupAccountName(NULL, (TCHAR*)(const TCHAR*)strUserName, pUserSID, &cbUserSID, pszDomain, &cbDomain, &snuType);
	if (ret == FALSE){
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER){
			return false;
		}
	}
	pUserSID = (PSID)malloc(cbUserSID);
	if (pUserSID == NULL){
		// メモリ確保失敗
		return false;
	}

	pszDomain = (TCHAR*)::malloc(cbDomain*sizeof(TCHAR));
	if (NULL == pszDomain){
		// メモリ確保失敗
		free(pUserSID);
		return false;
	}
	memset(pszDomain, 0, cbDomain * sizeof(TCHAR));

	// SID取得
	ret = ::LookupAccountName(NULL, (TCHAR*)(const TCHAR*)strUserName, pUserSID, &cbUserSID, pszDomain, &cbDomain, &snuType);
	if (ret == FALSE){
		free(pUserSID);
		free(pszDomain);
		return false;
	}

	// ここまでくればSIDは正常に取得できた
	PSID_IDENTIFIER_AUTHORITY psia;
	DWORD dwSidSize, dwSubAuthorities;

	if (!IsValidSid(pUserSID)){
		free(pUserSID);
		free(pszDomain);
		return false;
	}

	// コンピュータ名領域のクリア
	free(pszDomain);

	psia = GetSidIdentifierAuthority(pUserSID);
	dwSubAuthorities = *GetSidSubAuthorityCount(pUserSID);
	// S-SID_REVISION- + IdentifierAuthority- + subauthorities- + NULL
	dwSidSize = (15 + 12 + (12 * dwSubAuthorities) + 1) * sizeof(TCHAR);

	//LPTSTR pSID = NULL;
	//DWORD size = dwSidSize;
	//pSID = (LPTSTR)malloc(size);
	//if (pSID){
	//	memset(pSID, 0, size);
	//}
	//else{
	//	free(pUserSID);
	//	free(pszDomain);
	//	return false;
	//}

	CString temp;
	strSid.Empty();
	temp.Format(_T("S-%lu-"), SID_REVISION);
	strSid += temp;
	//dwSidSize = swprintf_s(pSID, dwSidSize * 2, _T("S-%lu-"), SID_REVISION);


	if ((psia->Value[0] != 0) || (psia->Value[1] != 0))
	{
		temp.Format(_T("0x%02hx%02hx%02hx%02hx%02hx%02hx"),
			(USHORT)psia->Value[0],
			(USHORT)psia->Value[1],
			(USHORT)psia->Value[2],
			(USHORT)psia->Value[3],
			(USHORT)psia->Value[4],
			(USHORT)psia->Value[5]);
		strSid += temp;
		//dwSidSize += swprintf_s(pSID + lstrlen(pSID),
		//	size,
		//	_T("0x%02hx%02hx%02hx%02hx%02hx%02hx"),
		//	(USHORT)psia->Value[0],
		//	(USHORT)psia->Value[1],
		//	(USHORT)psia->Value[2],
		//	(USHORT)psia->Value[3],
		//	(USHORT)psia->Value[4],
		//	(USHORT)psia->Value[5]);
	}
	else
	{
		temp.Format(_T("%lu"),
			(ULONG)(psia->Value[5]) +
			(ULONG)(psia->Value[4] << 8) +
			(ULONG)(psia->Value[3] << 16) +
			(ULONG)(psia->Value[2] << 24));
		strSid += temp;
		//dwSidSize += swprintf_s(pSID + lstrlen(pSID),
		//	size,
		//	_T("%lu"),
		//	(ULONG)(psia->Value[5]) +
		//	(ULONG)(psia->Value[4] << 8) +
		//	(ULONG)(psia->Value[3] << 16) +
		//	(ULONG)(psia->Value[2] << 24));
	}

	// Add SID subauthorities to the string
	for (DWORD dwCounter = 0; dwCounter < dwSubAuthorities; dwCounter++)
	{
		temp.Format(_T("-%lu"), *GetSidSubAuthority(pUserSID, dwCounter));
		strSid += temp;
		//dwSidSize += swprintf_s(pSID + dwSidSize,
		//	size,
		//	_T("-%lu"),
		//	*GetSidSubAuthority(pUserSID, dwCounter));
	}

	free(pUserSID);

	return true;
}

void CStationControlApp::SetMonitorMode(CWnd* pMain)
{
	if (GetExecuteMode() == eExecuteMode_Control)
		return;

	TCHAR name[100];
	CWnd *pwnd = pMain->GetWindow(GW_CHILD);
	while (pwnd != NULL){
		GetClassName(pwnd->m_hWnd, name, 100);
		CString str = name;
		if (str.CompareNoCase(_T("Button")) == 0){
			pwnd->EnableWindow(FALSE);
		}
		pwnd = pwnd->GetWindow(GW_HWNDNEXT);
	}
}

/*============================================================================*/
// 制限つき制御モードの設定
/*============================================================================*/
void CStationControlApp::SetLimitedMode()
{
	char host_name[256]; //ホスト名

	m_nLimitedMode = eLimitedMode_Disable;

	if(theApp.GetSelectStation() != eStation_Usuda642)
	{
		return;		//アプリ54m用以外
	}

	if(theApp.GetExecuteMode() != eExecuteMode_Control)
	{
		return;		//制御モード以外
	}

	if (mServiceMode == eServiceMode_limited)
	{
		// INIファイル設定によるデバッグ用の制限モード設定
		m_nLimitedMode = eLimitedMode_Enable;
		return;
	}
	else if (mServiceMode == eServiceMode_unlimited)
	{
		// INIファイル設定による制限モード無し設定のため、何もしない
		return;
	}

	if(gethostname(host_name, sizeof(host_name)) != 0)
	{
		return;
	}

	CString strHost = CString(host_name);
	if(strHost.Left(8).CompareNoCase(mHostNameUsuda) == 0)	//ホスト名 臼田宇宙空間観測所(臼田64m局舎内)
	{
		m_nLimitedMode = eLimitedMode_Enable;
	}

	return;
}

/*============================================================================*/
/*! 設備アラーム状況一覧

-# 設備アラーム状況一覧の作成

@param

@retval void
*/
/*============================================================================*/
void CStationControlApp::CreateAlarmThread()
{
	if (m_nSelectMode == eStation_1){
		for (int i = 0; i < CShareAlarmIf::mAlarmStationMax; i++) {
			mShareAlarmIf[i].Create(true, i);
		}
	}
	//if (mpAlarmThread == NULL){
	//	mpAlarmThread = (CAlarmListThread*)AfxBeginThread(RUNTIME_CLASS(CAlarmListThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	//	mpAlarmThread->ResumeThread();
	//}
}
/*============================================================================*/
/*! 設備アラーム状況一覧

-# 設備アラーム状況一覧の削除

@param

@retval void
*/
/*============================================================================*/
void CStationControlApp::DeleteAlarmThread()
{
	//if (mpAlarmThread != NULL){
	//	mpAlarmThread->StopAlarmThread();
	//	WaitForSingleObject(mpAlarmThread->m_hThread, 5000);
	//	delete mpAlarmThread;
	//	mpAlarmThread = NULL;
	//}
}
/*============================================================================*/
/*! アプリケーション

-# 設備アラーム状況一覧の削除

@param

@retval void
*/
/*============================================================================*/
UINT CStationControlApp::GetDrawUpdateInterval(bool bRead)
{
	if (bRead == true){
		m_nDrawUpdateInterval = GetPrivateProfileInt(SECTION_SYSTEM, KEY_DRAWUPDATEINTERVAL, 500, m_strAppInifileName);
	}
	return m_nDrawUpdateInterval;
}


/*============================================================================*/
/*! アプリケーション

-# ループバンド設定ダイアログ起動

@param

@retval void
*/
/*============================================================================*/
void CStationControlApp::ExecLoopBandSetDlg(CString ctrl)
{
	vector<CString> list;

	list.push_back(CURRENT_LBW);
	theApp.GetSatelliteData().get_paramlist(ctrl, list);

	AgcLoopBandSelectDlg dlg;
	dlg.SetLBCmbList(list);
	dlg.DoModal();

#if 1
	CString strLB = dlg.m_strLoopBand;
	if (strLB.CompareNoCase(CURRENT_LBW) != 0)
	{
		SendControl(ctrl, strLB, _T(""), _T(""));
	}
#else
#if 1
	CString strLB = dlg.m_strLoopBand;

	CString strSat;
	vector<CShareLbwIf::stItem> slist;
	slist.clear();
	slist.resize(CShareLbwIf::mItemMax);
	mShareLbwIf.Recv((CShareLbwIf::stItem*)&slist.at(0), (UINT)slist.size());

	for (int i = 0; i < slist.size(); ++i)
	{
		CString sCtrl = CString(slist[i].ctrl);
		if (sCtrl.CompareNoCase(ctrl) == 0)
		{
			//			strLB = CString(slist[i].value);
			strSat = CString(slist[i].satellite);
		}
	}

	if (strLB.CompareNoCase(CURRENT_LBW) != 0)
	{
		SendControl(ctrl, strLB, _T(""), _T(""));
	}
	SendControlAgcTblLoad(ctrl, strSat, _T(""), _T(""));

#else
	CString strLB = dlg.m_strLoopBand;
	if (strLB.CompareNoCase(CURRENT_LBW) == 0)
	{
		vector<CShareLbwIf::stItem> list;
		list.clear();
		list.resize(CShareLbwIf::mItemMax);
		mShareLbwIf.Recv((CShareLbwIf::stItem*)&list.at(0), (UINT)list.size());

		for (int i = 0; i < list.size(); ++i)
		{
			CString sCtrl = CString(list[i].ctrl);
			if (sCtrl.CompareNoCase(ctrl) == 0)
			{
				strLB = CString(list[i].value);
			}
		}
	}

	SendControl(ctrl, strLB, _T(""), _T(""));
	SendControlAgcTblLoad(ctrl, strLB, _T(""), _T(""));
#endif
#endif

}

/*============================================================================*/
/*! アプリケーション

-# CURRENT_LBW スレッドの作成

@param

@retval void
*/
/*============================================================================*/
void CStationControlApp::CreateShareLbwWriteIF()
{
	if ( (m_nSelectMode == eStation_1)
		&& (m_nExecuteMode == eExecuteMode_Control))
	{
		mShareLbwIf.Create(true);
	}
}

void CStationControlApp::CreateShareLbwReadIF()
{
	mShareLbwIf.Create(false);
}

/*============================================================================*/
/*! アプリケーション

-# CURRENT_LBW スレッドの作成

@param

@retval void
*/
/*============================================================================*/
void CStationControlApp::WriteExecuteMode2FAFile(UINT mode)
{
	// FAフォルダパス取得
	CString fname;
	fname.Format(_T("%s%s\\%s"), (LPCTSTR)theApp.GetAppFaPanelPath(), (LPCTSTR)CString(mStationFolder[theApp.GetSelectStation()]).MakeUpper(), FA_AUTH_FILE);

	FILE* fp;

	if (_wfopen_s(&fp, fname, _T("w")) != 0 || fp == NULL)
	{
		return;
	}

	fwprintf(fp, _T("%u"), mode);

	fclose(fp);
}

