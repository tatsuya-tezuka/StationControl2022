/*============================================================================*/
/*! RdnsChk54.cpp

-# 計画レディネスチェック54m画面 (校正診断画面)
*/
/*============================================================================*/
// RdnsChk54.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "RdnsChk54.h"
#include "afxdialogex.h"
#include "HookMsgBox.h"
#include "LoopBandSelectDlg.h"

// timeGetTime使用
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )


#define CID_NA	_T("N/A")
#define MAC_MAX     256

// AGC校正
#define CTRL_AGC_X					_T("CTRL.RDYCHK_AGC DSN-MODEM_X_A@%s@%s@%s,%s;")	// X
#define CTRL_AGC_X_CANCEL			_T("CTRL.RDYCHK_AGC_CANCEL DSN-MODEM_X_A,%s;")

#define CTRL_AGC_KA					_T("CTRL.RDYCHK_AGC DSN-MODEM_KA_A@%s@%s@%s,%s;")	// Ka
#define CTRL_AGC_KA_CANCEL			_T("CTRL.RDYCHK_AGC_CANCEL DSN-MODEM_KA_A,%s;")

// 局内ディレイ値計測
#define CTRL_RNG_XX					_T("CTRL.RDYCHK_RNG DXN@%s@%s,%s;")				// X/X
#define CTRL_RNG_XX_CANCEL			_T("CTRL.RDYCHK_RNG_CANCEL DXN,%s;")

#define CTRL_RNG_RX					_T("CTRL.RDYCHK_RNG DXR@%s@%s,%s;")				// X/X(再生型)
#define CTRL_RNG_RX_CANCEL			_T("CTRL.RDYCHK_RNG_CANCEL DXR,%s;")

#define CTRL_RNG_XKA				_T("CTRL.RDYCHK_RNG DKAN@%s@%s,%s;")			// X/Ka
#define CTRL_RNG_XKA_CANCEL			_T("CTRL.RDYCHK_RNG_CANCEL DKAN,%s;")

#define CTRL_RNG_RXKA				_T("CTRL.RDYCHK_RNG DKAR@%s@%s,%s;")			// X/Ka(再生型)
#define CTRL_RNG_RXKA_CANCEL		_T("CTRL.RDYCHK_RNG_CANCEL DKAR,%s;")

// テレメトリ伝送チェック
#define CTRL_TLM_X_TLMIN			_T("CTRL.RDYCHK_TLM X@%s@%s@%d@%s@%s,%s;")		// X
#define CTRL_TLM_X_TLMIN_CANCEL		_T("CTRL.RDYCHK_TLM_CANCEL X,%s;")

#define CTRL_TLM_KA_TLMIN			_T("CTRL.RDYCHK_TLM KA@%s@%s@%d@%s@%s,%s;")		// Ka
#define CTRL_TLM_KA_TLMIN_CANCEL	_T("CTRL.RDYCHK_TLM_CANCEL KA,%s;")

// コマンド伝送チェック
#define CTRL_CMD_X_CMDOUT			_T("CTRL.RDYCHK_CMD 1@%s@%s@%d@%s@%s,%s;")		// X
#define CTRL_CMD_X_CMDOUT_CANCEL	_T("CTRL.RDYCHK_CMD_CANCEL 1,%s;")

// EXCTR ROUTE選択制御
#define CMD_EXCTR_ROUTE				_T("X-SSPA.EXCTR_ROUTE")

#define MON_AGC_X_COND				_T("CTRL.AGC_X_COND")
#define MON_AGC_KA_COND				_T("CTRL.AGC_Ka_COND")
#define MON_RNG_COND				_T("CTRL.RNG_COND")
#define MON_TLM_X_COND				_T("CTRL.TLMCHK_X_COND")
#define MON_TLM_KA_COND				_T("CTRL.TLMCHK_Ka_COND")
#define MON_CMD_COND				_T("CTRL.CMDCHK_COND")

#define RES_TITLE					_T("校正診断結果")
#define RES_FILE_AGCX				_T("AGC_X_RES.txt")
#define RES_FILE_AGCKA				_T("AGC_KA_RES.txt")
#define RES_FILE_RNG				_T("DELAY_RES.txt")
#define RES_FILE_TLM				_T("TLM_RES.txt")
#define RES_FILE_CMD				_T("CMD_RES.txt")

#define X_RX_LBW					_T("DSNTCR.CARR_LOOP_BAND")
#define KA_RX_LBW					_T("DSNTCR.CARR_LOOP_BAND")

// 校正結果取得用監視名
#define RESULT_AGCX					_T("CTRL.RDYCHK_XAGC_RES")
#define RESULT_AGCKA				_T("CTRL.RDYCHK_KaAGC_RES")
#define RESULT_RNG					_T("CTRL.RDYCHK_DELAY_RES")
#define RESULT_TLM					_T("CTRL.RDYCHK_TLM_RES")
#define RESULT_CMD					_T("CTRL.RDYCHK_CMD_RES")

#define AGC_FILENAME_X				_T("xx_tcr_a");
#define AGC_FILENAME_KA				_T("xka_tcr_a");

//タイマー
#define TID_RDNSSTATE		1
#define TID_RES_AGCKA		2
#define TID_RES_AGCX		3
#define TID_RES_RNG			4
#define TID_RES_TLM			5
#define TID_RES_CMD			6

#define INTERVAL_RES		1000
#define INTERVAL_RDNSSTATE	5000

// CRdnsChk54 ダイアログ

IMPLEMENT_DYNAMIC(CRdnsChk54, CDialogBase)

/*============================================================================*/
/*! CRdnsChk54

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CRdnsChk54::CRdnsChk54(CWnd* pParent /*=NULL*/)
: CDialogBase(CRdnsChk54::IDD, pParent)
	, m_AgcChkValue(FALSE)
	, m_DlyChkValue(FALSE)
	, m_DlyXXChkValue(FALSE)
	, m_DlyXXRChkValue(FALSE)
	, m_DlyXKaTCRChkValue(FALSE)
	, m_DlyXKaTCRRChkValue(FALSE)
	, m_TLMChkValue(FALSE)
	, m_TLMXChkValue(FALSE)
	, m_CmdChkValue(FALSE)
	, m_CmdXChkValue(FALSE)
	, m_PeriodExecStartDay(0)
	, m_PeriodExecStartTime(0)
	, m_PeriodExecEndDay(0)
	, m_PeriodExecEndTime(0)
	, m_strXRX2LoopBand(_T(""))
	, m_strTlmBRX(_T(""))
	, m_strTlmBRKa(_T(""))
	, m_strCmdBRX(_T(""))
	, m_strKaTCRLoopBand(_T(""))
	, m_AgcXRX2ChkValue(FALSE)
	, m_AgcKaTCRChkValue(FALSE)
	, m_TLMKaChkValue(FALSE)
{
	m_LogDlg = NULL;
	m_strSatellite = _T("");
	m_hThread = NULL;
	m_TimeSpan = 0;
	m_ThreadAbort = false;
	m_ThreadRun = false;
	m_PassID = _T("");
	m_strTlmPassID = _T("");
	m_strCmdPassID = _T("");
	m_bNewPlan = TRUE;
	m_ResultDlg = NULL;
	m_RdnsChkAbort = false;
}

/*============================================================================*/
/*! CRdnsChk54

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CRdnsChk54::~CRdnsChk54()
{
}

void CRdnsChk54::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RDNS54_SSEL_CMB, m_SatelliteSelCmb);
	DDX_Control(pDX, IDC_RDNS54_SDISTANCE_EDIT, m_SatelliteDistanceEdit);
	DDX_Control(pDX, IDC_RDNS54_TLM_PATH_EDIT, m_TlmPathNoEdit);
	DDX_Control(pDX, IDC_RDNS54_CMD_PATH_EDIT, m_CmdPathNoEdit);
	DDX_Check(pDX, IDC_RDNS54_AGC_CHKBTN, m_AgcChkValue);
	DDX_Check(pDX, IDC_RDNS54_DELAY_CHKBTN, m_DlyChkValue);
	DDX_Check(pDX, IDC_RDNS54_D_SREQPM_XX_CHKBTN, m_DlyXXChkValue);
	DDX_Check(pDX, IDC_RDNS54_D_SREQPM_XXR_CHKBTN, m_DlyXXRChkValue);
	DDX_Check(pDX, IDC_RDNS54_D_SREQPM_XXKA_CHKBTN, m_DlyXKaTCRChkValue);
	DDX_Check(pDX, IDC_RDNS54_D_SREQPM_XXKAR_CHKBTN, m_DlyXKaTCRRChkValue);
	DDX_Check(pDX, IDC_RDNS54_TLM_CHKBTN, m_TLMChkValue);
	DDX_Check(pDX, IDC_RDNS54_TLM_R_XTLMIN, m_TLMXChkValue);
	DDX_Check(pDX, IDC_RDNS54_CMD_CHKBTN, m_CmdChkValue);
	DDX_Check(pDX, IDC_RDNS54_CMD_CMD_X_CHKBTN, m_CmdXChkValue);
	DDX_DateTimeCtrl(pDX, IDC_RDNS54_PERIOD_STIME_DAY_DP, m_PeriodExecStartDay);
	DDX_DateTimeCtrl(pDX, IDC_RDNS54_PERIOD_STIME_TIME_DP, m_PeriodExecStartTime);
	DDX_DateTimeCtrl(pDX, IDC_RDNS54_PERIOD_ETIME_DAY_DP, m_PeriodExecEndDay);
	DDX_DateTimeCtrl(pDX, IDC_RDNS54_PERIOD_ETIME_TIME_DP, m_PeriodExecEndTime);
	DDX_Text(pDX, IDC_RDNS54_XRX2_LB_EDIT, m_strXRX2LoopBand);
	DDX_Text(pDX, IDC_RDNS54_TLM_BR_X_EDIT, m_strTlmBRX);
	DDX_Text(pDX, IDC_RDNS54_TLM_BR_KA_EDIT, m_strTlmBRKa);
	DDX_Text(pDX, IDC_RDNS54_CMD_BR_X_EDIT, m_strCmdBRX);
	DDX_Text(pDX, IDC_RDNS54_KATCR_LB_EDIT, m_strKaTCRLoopBand);
	DDX_Check(pDX, IDC_RDNS54_XRX2_CHKBTN, m_AgcXRX2ChkValue);
	DDX_Check(pDX, IDC_RDNS54_KATCR_CHKBTN, m_AgcKaTCRChkValue);
	DDX_Control(pDX, IDC_RDNS54_PERIOD_STIME_DAY_DP, m_RdnsChkStartDTCtrl);
	DDX_Control(pDX, IDC_RDNS54_PERIOD_ETIME_DAY_DP, m_RdnsChkEndDTCtrl);
	DDX_Control(pDX, IDC_RDNS54_CID_BTN, m_CidSelBtn);
	DDX_Control(pDX, IDC_RDNS54_XRX2_CHKBTN, m_AGC_X_RX2Ctrl);
	DDX_Control(pDX, IDC_RDNS54_XRX2_LB_EDIT, m_AGC_X_RX2_LBCtrl);
	DDX_Control(pDX, IDC_RDNS54_XRX2_BTN, m_AGC_X_RX2_LBBCtrl);
	DDX_Control(pDX, IDC_RDNS54_XRX2_STEP_BTN, m_AGC_X_RX2_SBCtrl);
	DDX_Control(pDX, IDC_RDNS54_KATCR_CHKBTN, m_AGC_Ka_TCRCtrl);
	DDX_Control(pDX, IDC_RDNS54_KATCR_LB_EDIT, m_AGC_Ka_TCR_LBCtrl);
	DDX_Control(pDX, IDC_RDNS54_KA_LOOP_BTN, m_AGC_Ka_TCR_LBBCtrl);
	DDX_Control(pDX, IDC_RDNS54_KATCR_STEP_BTN, m_AGC_Ka_TCR_SBCtrl);
	DDX_Control(pDX, IDC_RDNS54_D_SREQPM_XX_CHKBTN, m_Delay_XXCtrl);
	DDX_Control(pDX, IDC_RDNS54_D_SREQPM_XXR_CHKBTN, m_Delay_XXRCtrl);
	DDX_Control(pDX, IDC_RDNS54_D_SREQPM_XXKA_CHKBTN, m_Delay_XKaTCRCtrl);
	DDX_Control(pDX, IDC_RDNS54_D_SREQPM_XXKAR_CHKBTN, m_Delay_XKaTCRRCtrl);
	DDX_Control(pDX, IDC_RDNS54_TLM_R_XTLMIN, m_TLM_XCtrl);
	DDX_Control(pDX, IDC_RDNS54_CMD_CMD_X_CHKBTN, m_CMD_XCtrl);
	DDX_Control(pDX, IDC_RDNS54_TLM_R_KATLMIN, m_TLM_KaCtrl);
	DDX_Check(pDX, IDC_RDNS54_TLM_R_KATLMIN, m_TLMKaChkValue);
	DDX_Control(pDX, IDC_RDNS54_REALTIME_STOPBTN, m_AbortBtnCtr);
	DDX_Control(pDX, IDC_RDNS54_PERIOD_STIME_TIME_DP, m_RdnsChkStartTTCtrl);
	DDX_Control(pDX, IDC_RDNS54_PERIOD_ETIME_TIME_DP, m_RdnsChkEndTTCtrl);
	DDX_Control(pDX, IDC_RDNS54_CMD_EXCTR_COMBO, m_ExctrRouteCombo);
}


BEGIN_MESSAGE_MAP(CRdnsChk54, CDialogBase)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CRdnsChk54::OnMenuClose)
	ON_BN_CLICKED(IDC_RDNS54_XRX2_BTN, &CRdnsChk54::OnBnClickedRdns54Xrx2Btn)
	ON_BN_CLICKED(IDC_RDNS54_KA_LOOP_BTN, &CRdnsChk54::OnBnClickedRdns54KaLoopBtn)
	ON_BN_CLICKED(IDC_RDNS54_REALTIME_RUNBTN, &CRdnsChk54::OnBnClickedRdns54RealtimeRunbtn)
	ON_BN_CLICKED(IDC_RDNS54_REALTIME_STOPBTN, &CRdnsChk54::OnBnClickedRdns54RealtimeStopbtn)
	ON_BN_CLICKED(IDC_RDNS54_PERIOD_ENTRYBTN, &CRdnsChk54::OnBnClickedRdns54PeriodEntrybtn)
	ON_BN_CLICKED(IDC_RDNS54_CID_BTN, &CRdnsChk54::OnBnClickedRdns54CidBtn)
	ON_BN_CLICKED(IDC_RDNS54_XRX2_STEP_BTN, &CRdnsChk54::OnBnClickedRdns54Xrx2StepBtn)
	ON_BN_CLICKED(IDC_RDNS54_KATCR_STEP_BTN, &CRdnsChk54::OnBnClickedRdns54KaStepBtn)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_RDNS54_SSEL_CMB, &CRdnsChk54::OnCbnSelchangeRdns54SselCmb)
	ON_BN_CLICKED(IDC_RDNS54_XRX2_CHKBTN, &CRdnsChk54::OnBnClickedRdns54Xrx2Chkbtn)
	ON_BN_CLICKED(IDC_RDNS54_KATCR_CHKBTN, &CRdnsChk54::OnBnClickedRdns54KaChkbtn)
	ON_BN_CLICKED(IDC_RDNS54_DELAY_CHKBTN, &CRdnsChk54::OnBnClickedRdns54DelayChkbtn)
	ON_BN_CLICKED(IDC_RDNS54_D_SREQPM_XX_CHKBTN, &CRdnsChk54::OnBnClickedRdns54DSreqpmXxChkbtn)
	ON_BN_CLICKED(IDC_RDNS54_D_SREQPM_XXR_CHKBTN, &CRdnsChk54::OnBnClickedRdns54DSreqpmXxrChkbtn)
	ON_BN_CLICKED(IDC_RDNS54_D_SREQPM_XXKA_CHKBTN, &CRdnsChk54::OnBnClickedRdns54DSreqpmXKaChkbtn)
	ON_BN_CLICKED(IDC_RDNS54_D_SREQPM_XXKAR_CHKBTN, &CRdnsChk54::OnBnClickedRdns54DSreqpmXKarChkbtn)
	ON_BN_CLICKED(IDC_RDNS54_TLM_CHKBTN, &CRdnsChk54::OnBnClickedRdns54TlmChkbtn)
	ON_BN_CLICKED(IDC_RDNS54_TLM_R_XTLMIN, &CRdnsChk54::OnBnClickedRdns54TlmRXtlmin)
	ON_BN_CLICKED(IDC_RDNS54_TLM_R_KATLMIN, &CRdnsChk54::OnBnClickedRdns54TlmRKatlmin)
	ON_BN_CLICKED(IDC_RDNS54_CMD_CHKBTN, &CRdnsChk54::OnBnClickedRdns54CmdChkbtn)
	ON_BN_CLICKED(IDC_RDNS54_CMD_CMD_X_CHKBTN, &CRdnsChk54::OnBnClickedRdns54CmdCmdXChkbtn)
	ON_BN_CLICKED(IDC_RDNS54_AGC_CHKBTN, &CRdnsChk54::OnBnClickedRdns54AgcChkbtn)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_RDNS54_PERIOD_STIME_DAY_DP, &CRdnsChk54::OnDtnDatetimechangeRdns54PeriodStimeDayDp)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_RDNS54_PERIOD_STIME_TIME_DP, &CRdnsChk54::OnDtnDatetimechangeRdns54PeriodStimeTimeDp)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRdnsChk54 メッセージ ハンドラー

/*============================================================================*/
/*! CRdnsChk54

-# ファイル-閉じるメニュー処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnMenuClose()
{
	CString title;
	GetWindowText(title);

	if (m_ThreadRun == true)
	{
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("校正診断実行中です。終了してよろしいですか？\n実行中の校正診断は強制終了します。"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			return;
		}
		m_ThreadAbort = true;
	}

	CloseLogDlg();
	InitTimer();
	ShowWindow(SW_HIDE);
}


/*============================================================================*/
/*! CRdnsChk54

-# 閉じる処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnClose()
{
	CString title;
	GetWindowText(title);

	if (m_ThreadRun == true)
	{
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("校正診断実行中です。終了してよろしいですか？\n実行中の校正診断は強制終了します。"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			return;
		}

		m_ThreadAbort = true;
	}
	else
	{
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("終了します。\nよろしいですか？"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			return;
		}
	}

	CloseLogDlg();
	InitTimer();
	ShowWindow(SW_HIDE);
}


/*============================================================================*/
/*! CRdnsChk54

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CRdnsChk54::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// 衛星選択
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	vector<CString>			&saNameList = dba.GetSatNameList();
	int cnt = (int)saNameList.size();
	for (int i = 0; i < cnt; i++)
	{
		/*--------------------------*/
		/* 衛星本体情報アドレス取得 */
		CString satname = saNameList[i];

		if (satname == _T("DEFAULT") || satname == _T("ＤＥＦＡＵＬＴ"))
		{
			continue;
		}
		m_SatelliteSelCmb.InsertString(-1, satname);
	}

	// 衛星までの距離
	m_SatelliteDistanceEdit.SetRoundPlaceValue(3);	// 小数点3桁
	m_SatelliteDistanceEdit.SetWindowTextW(_T("10000.000"));


	//---------------------------------------
	// AGC校正
	m_strXRX2LoopBand = CURRENT_LBW;
	m_strKaTCRLoopBand = CURRENT_LBW;

	// Xのループバンド選択肢取得
	m_X_RX2LBList.clear();
	m_X_RX2LBList.push_back(CURRENT_LBW);
	theApp.GetSatelliteData().get_paramlist(X_RX_LBW, m_X_RX2LBList);

	// Kaのループバンド選択肢取得
	m_Ka_LBList.clear();
	m_Ka_LBList.push_back(CURRENT_LBW);
	theApp.GetSatelliteData().get_paramlist(KA_RX_LBW, m_Ka_LBList);

	// 日付フォーマット
	m_RdnsChkStartDTCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_RdnsChkStartTTCtrl.SetFormat(_T("HH:mm:00"));

	m_RdnsChkEndDTCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_RdnsChkEndTTCtrl.SetFormat(_T("HH:mm:00"));

	// EXCTR ROUTEコンボ作成
	CreateExctrRouteCombo();

	UpdateData(FALSE);

	CenterWindowEx();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CRdnsChk54

-# ログダイアログ表示処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CRdnsChk54::UpdateRdnsChkDlg()
{
	if (m_ThreadRun == true)
	{
		return FALSE;
	}

	// 現在時刻取得
	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	__int64 d_timeCurrent = CTime(timeSys).GetTime();
	CTime mintime = CTime((time_t)((double)d_timeCurrent + START_OFFSET));
	CTime time;
	if (m_bNewPlan == TRUE && m_StartTime == 0)
	{
		// 新規計画でタイムライン指定時刻でない場合
		time = mintime;
	}
	else
	{
		// 計画変更, もしくは新規計画でタイムラインの開始時刻を使う
		time = m_StartTime;
	}

	// UTC現在時間取得し、開始日と終了日を制限
	COleDateTime oletime1 = COleDateTime(time.GetTime());
	COleDateTime oletime2;
	oletime2.SetStatus(COleDateTime::null);
	m_RdnsChkStartDTCtrl.SetRange(&oletime1, &oletime2);
	m_RdnsChkEndDTCtrl.SetRange(&oletime1, &oletime2);

	m_strDay = time.Format(_T("%y%m%d"));
	CString passId = _T("");
	passId.Format(_T("%s-9999"), (LPCTSTR)m_strDay);

	// 校正診断更新時計画ファイル読込
	stPlandata stpd;
	vector<stCalibPlan> agclist;
	agclist.clear();
	vector<stCalibPlan> tlmlist;
	tlmlist.clear();
	vector<stCalibPlan> cmdlist;
	cmdlist.clear();

	CString cid = _T("");

	if (m_bNewPlan == FALSE)
	{
		// 計画変更
		if (COprPlnData::ReadRdyChkPlanFile(m_strSatellite, m_PassID, stpd, agclist, tlmlist, cmdlist) != -1)
		{
			time = CSatelliteData::StrTimeToCTime(CString(stpd.sz_start));
			passId = m_PassID;
			cid = stpd.sz_cid;
		}
	}

	//---------------------------------------
	// テレメトリ伝送チェック
	// パス番号
	CString strEntry;
	strEntry.Format(_T("______-____"));

	m_TlmPathNoEdit.EnableMask(_T("DDDDDD DDDD"),	// The mask string
		strEntry,								// Literal, "_" char = character entry
		_T(' '));								// Default char
	m_TlmPathNoEdit.SetValidChars(NULL); // Valid string characters
	m_TlmPathNoEdit.SetWindowTextW(passId);
	m_strTlmPassID = passId;

	//---------------------------------------
	// コマンド伝送チェック
	// パス番号
	m_CmdPathNoEdit.EnableMask(_T("DDDDDD DDDD"),	// The mask string
		strEntry,								// Literal, "_" char = character entry
		_T(' '));								// Default char
	m_CmdPathNoEdit.SetValidChars(NULL); // Valid string characters
	m_CmdPathNoEdit.SetWindowTextW(passId);
	m_strCmdPassID = passId;

	// 校正診断開始/終了時刻
	m_PeriodExecStartDay = time;
	m_PeriodExecStartTime = time;
	m_PeriodExecEndDay = time;
	m_PeriodExecEndTime = time;
	
	UpdateData(FALSE);

	// 衛星名、CID設定
	vector<CString> &sList = theApp.GetSatelliteData().m_CIDSlctData.GetSatelliteNameList();
	map<CString, map<CString, stCIDParam>> &cidlist = theApp.GetSatelliteData().m_CIDSlctData.GetCidParamList();

#if 0
//	if (sList.size() == 0)
//	{
//		m_SatelliteSelCmb.SetCurSel(0);
//		m_CidSelBtn.SetWindowTextW(CID_NA);
//		m_CidSelBtn.EnableWindow(FALSE);
//		return FALSE;
//	}
#endif

	stCIDParam stCid;
	CString satname = _T("");
	BOOL bCid = TRUE;

	if ((m_strSatellite.CompareNoCase(_T("VLBI")) == 0)
		|| (m_strSatellite.CompareNoCase(_T("MAINT")) == 0))
	{
		m_strSatellite = _T("");
	}

	if (m_strSatellite == _T(""))
	{
		m_SatelliteSelCmb.SetCurSel(0);
		m_SatelliteSelCmb.EnableWindow(TRUE);
		m_SatelliteSelCmb.GetLBText(0, satname);

		if (sList.size() == 0)
		{
			m_CidSelBtn.SetWindowTextW(CID_NA);
			m_CidSelBtn.EnableWindow(FALSE);
			bCid = FALSE;
		}
		else
		{
			vector<CString> &cList = theApp.GetSatelliteData().m_CIDSlctData.GetCidList(satname);
			if (cList.size() == 0)
			{
				m_CidSelBtn.SetWindowTextW(CID_NA);
				m_CidSelBtn.EnableWindow(FALSE);
				bCid = FALSE;
			}
			else
			{
				stOpeProp stProp;
				theApp.GetSatelliteData().ReadPropertyFile(satname, stProp);
				CString strCid = CString(stProp.cid);
				auto itrl = std::find(cList.begin(), cList.end(), strCid);
				if (itrl == cList.end())
				{
					// デフォルトCIDがCIDファイルに無い場合、一番最初のCIDをセット。
					strCid = cList[0];
				}

				m_CidSelBtn.EnableWindow(TRUE);
				m_CidSelBtn.SetWindowTextW(strCid);
				stCid = CCIDSlctData::TrimInvalidString(cidlist[satname][strCid]);
			}
		}
	}
	else
	{
		m_SatelliteSelCmb.SelectStringEx(-1, m_strSatellite);
		m_SatelliteSelCmb.EnableWindow(FALSE);
		satname = m_strSatellite;

		vector<CString> &cList = theApp.GetSatelliteData().m_CIDSlctData.GetCidList(m_strSatellite);
		if (cList.size() == 0)
		{
			m_CidSelBtn.SetWindowTextW(CID_NA);
			m_CidSelBtn.EnableWindow(FALSE);
			bCid = FALSE;
		}
		else
		{
			stOpeProp stProp;
			theApp.GetSatelliteData().ReadPropertyFile(m_strSatellite, stProp);
			CString strCid = CString(stProp.cid);
			auto itrl = std::find(cList.begin(), cList.end(), strCid);
			if (itrl == cList.end())
			{
				// デフォルトCIDがCIDファイルに無い場合、一番最初のCIDをセット。
				strCid = cList[0];
			}

			if (cid.IsEmpty() == TRUE)
			{
				m_CidSelBtn.EnableWindow(TRUE);
				m_CidSelBtn.SetWindowTextW(strCid);
				stCid = CCIDSlctData::TrimInvalidString(cidlist[m_strSatellite][strCid]);
			}
			else
			{
				auto itrl = std::find(cList.begin(), cList.end(), cid);
				if (itrl != cList.end())
				{
					m_CidSelBtn.EnableWindow(TRUE);
					m_CidSelBtn.SetWindowTextW(cid);
					stCid = CCIDSlctData::TrimInvalidString(cidlist[m_strSatellite][cid]);
				}
				else
				{
					m_CidSelBtn.EnableWindow(TRUE);
					m_CidSelBtn.SetWindowTextW(strCid);
					stCid = CCIDSlctData::TrimInvalidString(cidlist[m_strSatellite][strCid]);
				}
			}
		}
	}

	// 衛星固有情報取得
	GetEqSatDBData(satname);

	if (bCid)
	{
		// テレメトリビットレート
		m_strTlmBRX = stCid.TlmBitRateX;
		m_strTlmBRKa = stCid.TlmBitRateKa;

		// コマンドビットレート
		m_strCmdBRX = stCid.CmdBitRateX;
	}
	else
	{
		// テレメトリビットレート
		m_strTlmBRX = _T("");
		m_strTlmBRKa = _T("");

		// コマンドビットレート
		m_strCmdBRX = _T("");
	}

	// 校正診断用コントロール設定
	SetWindowEnable(satname);

	// 計画ファイル更新時の画面設定
	SetSetting(agclist, tlmlist, cmdlist);

	// CURRENT_LWB取得
	GetCurrentLWB();

	// 強制終了ボタン非活性
	m_AbortBtnCtr.EnableWindow(FALSE);

	m_ThreadRun = false;

	UpdateData(FALSE);

	// 終了予定時刻変更
	ChangeRdyChkEndTime();

	return TRUE;
}


/*============================================================================*/
/*! CRdnsChk54

-# 読み込んだ計画ファイルに従い画面を設定する。

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::SetSetting(vector<stCalibPlan>& agclist, vector<stCalibPlan>& tlmlist, vector<stCalibPlan>& cmdlist)
{
	// AGC,RNG 設定
	BOOL bAGC = FALSE;
	BOOL bRNG = FALSE;
	for (int i = 0; i < agclist.size(); ++i)
	{
		CString item = agclist[i].ctrl;
		if (item.CompareNoCase(_T("DSN-MODEM_X_A")) == 0)
		{
			m_AgcXRX2ChkValue = TRUE;		//AGC校正 X チェックボタン
			if (agclist[i].loopband == _T(""))
			{
				m_strXRX2LoopBand = CURRENT_LBW;
			}
			else
			{
				m_strXRX2LoopBand = agclist[i].loopband;
			}
			bAGC = TRUE;
		}
		else if (item.CompareNoCase(_T("DSN-MODEM_KA_A")) == 0)
		{
			m_AgcKaTCRChkValue = TRUE;		//AGC校正 Ka チェックボタン
			if (agclist[i].loopband == _T(""))
			{
				m_strKaTCRLoopBand = CURRENT_LBW;
			}
			else
			{
				m_strKaTCRLoopBand = agclist[i].loopband;
			}
			bAGC = TRUE;
		}
		else if (item.CompareNoCase(_T("DXN")) == 0)
		{
			m_DlyXXChkValue = TRUE;			//局内ディレイ値計測 X/X チェックボタン
			bRNG = TRUE;
		}
		else if (item.CompareNoCase(_T("DXR")) == 0)
		{
			m_DlyXXRChkValue = TRUE;		//局内ディレイ値計測 X/X(再生型) チェックボタン
			bRNG = TRUE;
		}
		else if (item.CompareNoCase(_T("DKAN")) == 0)
		{
			m_DlyXKaTCRChkValue = TRUE;		//局内ディレイ値計測 X/Ka チェックボタン
			bRNG = TRUE;
		}
		else if (item.CompareNoCase(_T("DKAR")) == 0)
		{
			m_DlyXKaTCRRChkValue = TRUE;	//局内ディレイ値計測 X/Ka(再生型) チェックボタン
			bRNG = TRUE;
		}
	}

	if (bAGC == TRUE)
	{
		m_AgcChkValue = TRUE;
	}

	if (bRNG == TRUE)
	{
		m_DlyChkValue = TRUE;
	}


	// TLM 設定
	BOOL bTLM = FALSE;
	for (int i = 0; i < tlmlist.size(); ++i)
	{
		CString item = tlmlist[i].ctrl;
		if (item.CompareNoCase(_T("X")) == 0)
		{
			m_TLMXChkValue = TRUE;		//テレメトリ伝送チェック X チェックボタン
			bTLM = TRUE;
		}
		else if (item.CompareNoCase(_T("KA")) == 0)
		{
			m_TLMKaChkValue = TRUE;		//テレメトリ伝送チェック Ka チェックボタン
			bTLM = TRUE;
		}
	}
	if (bTLM == TRUE)
	{
		m_TLMChkValue = TRUE;
	}

	// CMD 設定
	BOOL bCMD = FALSE;
	for (int i = 0; i < cmdlist.size(); ++i)
	{
		CString item = cmdlist[i].ctrl;
		if (item.CompareNoCase(_T("1")) == 0)
		{
			m_CmdXChkValue = TRUE;		//コマンド伝送チェック X チェックボタン
			bCMD = TRUE;
		}
	}

	if (bCMD == TRUE)
	{
		m_CmdChkValue = TRUE;
	}

	UpdateData(FALSE);
}
	
/*============================================================================*/
/*! CRdnsChk54

-# AGC校正 チェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54AgcChkbtn()
{
	// 終了予定時刻変更
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk54

-# AGC校正 X チェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54Xrx2Chkbtn()
{
	// 終了予定時刻変更
	ChangeRdyChkEndTime();
}

/*============================================================================*/
/*! CRdnsChk54

-# X ループバンド変更ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54Xrx2Btn()
{
	// AGC校正 ループバンド画面表示
#if 1
	UpdateData();
	CLoopBandSelectDlg dlg;
	dlg.SetLBCmbList(m_X_RX2LBList);
	dlg.SetLoopBand(m_strXRX2LoopBand);
	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	m_strXRX2LoopBand = dlg.m_strLoopBand;
	UpdateData(FALSE);

	if (dlg.m_strLoopBand != CURRENT_LBW)
	{
		// 局名
		CString station = CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower();

		// 衛星名
		CString strSat = _T("");
		m_SatelliteSelCmb.GetLBText(m_SatelliteSelCmb.GetCurSel(), strSat);
		CString satellite = strSat;

		// AGC校正名
		CString agcItem = AGC_FILENAME_X;

		// CUST
		CString cust = _T("");
		cust.Format(_T("agc_%s.tbl_%s_%s"), (LPCTSTR)agcItem, (LPCTSTR)m_strXRX2LoopBand, _T("CUST"));

		// DFLT
		CString dflt = _T("");
		dflt.Format(_T("agc_%s.tbl_%s_%s"), (LPCTSTR)agcItem, (LPCTSTR)m_strXRX2LoopBand, _T("DFLT"));

		theApp.GetCalib(station, satellite, cust, dflt);
	}

#else
//	UpdateData();
//	CRdnsAgcDetail dlg;
//
//	dlg.SetLoopBand(m_strXRX2LoopBand);
//	if (dlg.DoModal() == IDCANCEL)
//	{
//		return;
//	}
//
//	m_strXRX2LoopBand = strAGCLoopBand[dlg.m_nLoopBand];
//	UpdateData(FALSE);
//
//	// ファイル要求（DFLT、CUST）
//	if (dlg.m_nLoopBand != AGCLB_CURRENT_LBW)
//	{
//		// 局名
//		CString station = CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower();
//
//		// 衛星名
//		CString strSat = _T("");
//		m_SatelliteSelCmb.GetLBText(m_SatelliteSelCmb.GetCurSel(), strSat);
//		CString satellite = strSat;
//
//		// AGC校正名
//		CString agcItem = AGC_FILENAME_X;
//		CString cust = _T("");
//		CString dflt = _T("");
//
//		// ループバンド
//		CString strLb = _T("");
//		if (m_strXRX2LoopBand == strAGCLoopBand[AGCLB_CURRENT_LBW])
//		{
//			strLb = m_xrx2_lbw;
//		}
//		else
//		{
//			strLb = m_strXRX2LoopBand;
//		}
//
//		// CUST
//		cust.Format(_T("agc_%s.tbl_%s_%s"), agcItem, strLb, _T("CUST"));
//		// DFLT
//		dflt.Format(_T("agc_%s.tbl_%s_%s"), agcItem, strLb, _T("DFLT"));
//		theApp.GetCalib(station, satellite, cust, dflt);
//	}
#endif
}

/*============================================================================*/
/*! CRdnsChk54

-# X ステップ数/積分時間変更ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54Xrx2StepBtn()
{
	// ステップ数/積分時間画面表示
	CRdnsAgcDetailStep dlg;

	// 局名
	CString station = CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower();
	dlg.m_Station = station;

	// 衛星名
	CString satellite = _T("");
	m_SatelliteSelCmb.GetLBText(m_SatelliteSelCmb.GetCurSel(), satellite);
	dlg.m_Satellite = satellite;

	// AGC校正名
	CString agcItem = AGC_FILENAME_X;
	dlg.m_AgcItem = agcItem;

	// ループバンド
	CString strLb = _T("");
	if (m_strXRX2LoopBand == CURRENT_LBW)
	{
		if (m_xrx2_lbw.IsEmpty() == TRUE)
		{
			CString title;
			GetWindowText(title);
			MessageBox(_T("CURRENT_LBW値の取得ができませんでした。\nステップ数/積分時間変更はできません。"), title, MB_OK | MB_ICONWARNING);
			return;
		}
		strLb = m_xrx2_lbw;
	}
	else
	{
		strLb = m_strXRX2LoopBand;
	}

	dlg.m_LoopBand = strLb;

	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}

#if 0
//	// ファイル転送
//	CString file;
//	file.Format(_T("agc_%s.tbl_%s_%s"), agcItem, strLb, _T("CUST"));
//	theApp.RequestCalib(station, satellite, file);
//
//	file.Format(_T("agc_%s.tbl_%s"), agcItem, strLb);
//	theApp.RequestCalib(station, satellite, file);
#endif
}

/*============================================================================*/
/*! CRdnsChk54

-# AGC校正 Ka チェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54KaChkbtn()
{
	// 終了予定時刻変更
	ChangeRdyChkEndTime();
}

/*============================================================================*/
/*! CRdnsChk54

-# Ka ループバンド変更ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54KaLoopBtn()
{
	// AGC校正 ループバンド画面表示
#if 1
	UpdateData();
	CLoopBandSelectDlg dlg;
	dlg.SetLBCmbList(m_Ka_LBList);
	dlg.SetLoopBand(m_strKaTCRLoopBand);
	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	m_strKaTCRLoopBand = dlg.m_strLoopBand;
	UpdateData(FALSE);

	if (dlg.m_strLoopBand != CURRENT_LBW)
	{
		// 局名
		CString station = CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower();

		// 衛星名
		CString satellite = _T("");
		m_SatelliteSelCmb.GetLBText(m_SatelliteSelCmb.GetCurSel(), satellite);

		// AGC校正名
		CString agcItem = AGC_FILENAME_KA;

		// CUST
		CString cust = _T("");
		cust.Format(_T("agc_%s.tbl_%s_%s"), (LPCTSTR)agcItem, (LPCTSTR)m_strKaTCRLoopBand, _T("CUST"));

		// DFLT
		CString dflt = _T("");
		dflt.Format(_T("agc_%s.tbl_%s_%s"), (LPCTSTR)agcItem, (LPCTSTR)m_strKaTCRLoopBand, _T("DFLT"));

		theApp.GetCalib(station, satellite, cust, dflt);
	}
#else
//	UpdateData();
//	CRdnsAgcDetail dlg;
//
//	dlg.SetLoopBand(m_strKaTCRLoopBand);
//	if (dlg.DoModal() == IDCANCEL)
//	{
//		return;
//	}
//
//	m_strKaTCRLoopBand = strAGCLoopBand[dlg.m_nLoopBand];
//	UpdateData(FALSE);
//
//	// ファイル要求（DFLT、CUST）
//	if (dlg.m_nLoopBand != AGCLB_CURRENT_LBW)
//	{
//		// 局名
//		CString station = CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower();
//
//		// 衛星名
//		CString satellite = _T("");
//		m_SatelliteSelCmb.GetLBText(m_SatelliteSelCmb.GetCurSel(), satellite);
//
//		// AGC校正名
//		CString agcItem = AGC_FILENAME_KA;
//		CString cust = _T("");
//		CString dflt = _T("");
//
//		// ループバンド
//		CString strLb = _T("");
//		if (m_strKaTCRLoopBand == strAGCLoopBand[AGCLB_CURRENT_LBW])
//		{
//			strLb = m_xx_tcr_lbw;
//		}
//		else
//		{
//			strLb = m_strKaTCRLoopBand;
//		}
//
//		// CUST
//		cust.Format(_T("agc_%s.tbl_%s_%s"), agcItem, strLb, _T("CUST"));
//		// DFLT
//		dflt.Format(_T("agc_%s.tbl_%s_%s"), agcItem, strLb, _T("DFLT"));
//		theApp.GetCalib(station, satellite, cust, dflt);
//	}
#endif
}

/*============================================================================*/
/*! CRdnsChk54

-# Ka ステップ数/積分時間変更ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54KaStepBtn()
{
	CRdnsAgcDetailStep dlg;

	// 局名
	CString station = CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower();
	dlg.m_Station = station;

	// 衛星名
	CString satellite = _T("");
	m_SatelliteSelCmb.GetLBText(m_SatelliteSelCmb.GetCurSel(), satellite);
	dlg.m_Satellite = satellite;

	// AGC校正名
	CString agcItem = AGC_FILENAME_KA;
	dlg.m_AgcItem = agcItem;

	// ループバンド
	CString strLb = _T("");
	if (m_strKaTCRLoopBand == CURRENT_LBW)
	{
		if (m_strKa_lbw.IsEmpty() == TRUE)
		{
			CString title;
			GetWindowText(title);
			MessageBox(_T("CURRENT_LBW値の取得ができませんでした。\nステップ数/積分時間変更はできません。"), title, MB_OK | MB_ICONWARNING);
			return;
		}
		strLb = m_strKa_lbw;
	}
	else
	{
		strLb = m_strKaTCRLoopBand;
	}

	dlg.m_LoopBand = strLb;

	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}

#if 0
//	// ファイル転送
//	CString file;
//	file.Format(_T("agc_%s.tbl_%s_%s"), agcItem, strLb, _T("CUST"));
//	theApp.RequestCalib(station, satellite, file);
//
//	file.Format(_T("agc_%s.tbl_%s"), agcItem, strLb);
//	theApp.RequestCalib(station, satellite, file);
#endif
}

/*============================================================================*/
/*! CRdnsChk54

-# 局内ディレイ値計測 チェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54DelayChkbtn()
{
	// 終了予定時刻変更
	ChangeRdyChkEndTime();
}

/*============================================================================*/
/*! CRdnsChk54

-# 局内ディレイ値計測 X/X チェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54DSreqpmXxChkbtn()
{
	// 終了予定時刻変更
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk54

-# 局内ディレイ値計測 X/X(再生型) チェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54DSreqpmXxrChkbtn()
{
	// 終了予定時刻変更
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk54

-# 局内ディレイ値計測 X/Ka チェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54DSreqpmXKaChkbtn()
{
	// 終了予定時刻変更
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk54

-# 局内ディレイ値計測 X/Ka(再生型) チェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54DSreqpmXKarChkbtn()
{
	// 終了予定時刻変更
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk54

-# テレメトリ伝送チェック チェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54TlmChkbtn()
{
	// 終了予定時刻変更
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk54

-# テレメトリ伝送チェック X チェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54TlmRXtlmin()
{
	// 終了予定時刻変更
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk54

-# テレメトリ伝送チェック Ka チェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54TlmRKatlmin()
{
	// 終了予定時刻変更
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk54

-# コマンド伝送チェック チェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54CmdChkbtn()
{
	// 終了予定時刻変更
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk54

-# コマンド伝送チェック X チェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54CmdCmdXChkbtn()
{
	// 終了予定時刻変更
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk54

-# 実行ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54RealtimeRunbtn()
{
	if (m_ThreadRun == true)
	{
		return;
	}

	CString title;
	GetWindowText(title);

	m_CtrlList.clear();
	GetSetting();

	if (m_CtrlList.size() > 0)
	{
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("校正診断を実行します。\nよろしいですか？"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return;

		HANDLE hFile;
		FILETIME cfTime, afTime, wfTime;

		int flg = 0;
		m_AgcXResFileTime = 0;
		m_AgcKaResFileTime = 0;
		m_DelayResFileTime = 0;
		m_TlmResFileTime = 0;
		m_CmdResFileTime = 0;

		CString dir = theApp.GetShareFilePath(eFileType_Calib_Result_Folder, theApp.GetSelectStation());
		CString strFilePath = _T("");

//#ifdef _DEBUG
//		CString strTime;
//#endif
		for (int i = 0; i < m_CtrlList.size(); ++i)
		{
			stSendCtrlMsg st = m_CtrlList[i];

			if (((flg & 0x01) == 0) && st.type == TYPE_AGC_X)
			{
				strFilePath = dir + RES_FILE_AGCX;
				hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
				if (hFile != INVALID_HANDLE_VALUE)
				{
					GetFileTime(hFile, &cfTime, &afTime, &wfTime);
					CloseHandle(hFile);
					m_AgcXResFileTime = CTime(wfTime);
//#ifdef _DEBUG
//					strTime = m_AgcXResFileTime.Format("%Y-%m-%d %H:%M:%S");
//#endif
				}
				flg |= 0x01;
			}
			else if (((flg & 0x02) == 0) && st.type == TYPE_AGC_KA)
			{
				strFilePath = dir + RES_FILE_AGCKA;
				hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
				if (hFile != INVALID_HANDLE_VALUE)
				{
					GetFileTime(hFile, &cfTime, &afTime, &wfTime);
					CloseHandle(hFile);
					m_AgcKaResFileTime = CTime(wfTime);
//#ifdef _DEBUG
//					strTime = m_AgcKaResFileTime.Format("%Y-%m-%d %H:%M:%S");
//#endif
				}
				flg |= 0x02;
			}
			else if (((flg & 0x04) == 0) && st.type == TYPE_RNG)
			{
				strFilePath = dir + RES_FILE_RNG;
				hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
				if (hFile != INVALID_HANDLE_VALUE)
				{
					GetFileTime(hFile, &cfTime, &afTime, &wfTime);
					CloseHandle(hFile);
					m_DelayResFileTime = CTime(wfTime);
//#ifdef _DEBUG
//					strTime = m_DelayResFileTime.Format("%Y-%m-%d %H:%M:%S");
//#endif
				}
				flg |= 0x04;
			}
			else if (((flg & 0x08) == 0) && (st.type == TYPE_TLM_X || st.type == TYPE_TLM_KA))
			{
				strFilePath = dir + RES_FILE_TLM;
				hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
				if (hFile != INVALID_HANDLE_VALUE)
				{
					GetFileTime(hFile, &cfTime, &afTime, &wfTime);
					CloseHandle(hFile);
					m_TlmResFileTime = CTime(wfTime);
//#ifdef _DEBUG
//					strTime = m_TlmResFileTime.Format("%Y-%m-%d %H:%M:%S");
//#endif
				}
				flg |= 0x08;
			}
			else if (((flg & 0x10) == 0) && st.type == TYPE_CMD)
			{
				strFilePath = dir + RES_FILE_CMD;
				hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
				if (hFile != INVALID_HANDLE_VALUE)
				{
					GetFileTime(hFile, &cfTime, &afTime, &wfTime);
					CloseHandle(hFile);
					m_CmdResFileTime = CTime(wfTime);
//#ifdef _DEBUG
//					strTime = m_CmdResFileTime.Format("%Y-%m-%d %H:%M:%S");
//#endif
				}
				flg |= 0x10;
			}
		}

		InitTimer();

		m_ThreadAbort = false;
		m_ThreadRun = false;
		m_RdnsChkAbort = false;

		// 結果表示用ダイアログ表示
		if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
		{
			m_ResultDlg->DestroyWindow();
		}

		m_ResultDlg = new CRdnsChkResult();
		if (m_ResultDlg != NULL)
		{
			m_ResultDlg->SetRdnsCheckList(m_CtrlList);
			if (m_ResultDlg->Create(IDD_DLG_RDNSCHK_RESULT, this))
			{
				m_ResultDlg->ShowWindow(SW_SHOW);
			}
		}

		// 制御送信用スレッドの作成
		UINT uiThreadId;
		m_hThread = (HANDLE)_beginthreadex(NULL,				// SECURITY_ATTRIBUTES 構造体へのポインタ
			0,					// 新しいスレッドのスタック サイズ
			SendCtrlThread,		// 新しいスレッドの実行を開始するルーチンの開始アドレス
			this,				// スレッドパラメタ
			CREATE_SUSPENDED,	// 初期状態フラグ
			&uiThreadId);		// スレッド識別子を受け取る 32 ビット変数へのポインタ

		if (m_hThread != NULL)
		{
			ResumeThread(m_hThread);
		}

		// 実行ボタン非活性化
		GetDlgItem(IDC_RDNS54_REALTIME_RUNBTN)->EnableWindow(FALSE);

		// 登録ボタン非活性化
		GetDlgItem(IDC_RDNS54_PERIOD_ENTRYBTN)->EnableWindow(FALSE);

		// 強制終了ボタン活性化
		m_AbortBtnCtr.EnableWindow(TRUE);

		// 衛星選択コンボボックス非活性化
		m_SatelliteSelCmb.EnableWindow(FALSE);

		// CID選択ボタン非活性化
		m_CidSelBtn.EnableWindow(FALSE);

//#ifdef _DEBUG
//		SetTimer(TID_RDNSSTATE, INTERVAL_RDNSSTATE, 0);
//#endif
	}
	else
	{
		MessageBox(_T("校正診断の対象が選択されていません。"), title, MB_OK | MB_ICONWARNING);
	}
}

/*============================================================================*/
/*! CRdnsChk54

-# 画面設定取得

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::GetSetting()
{
	UpdateData();

	CString strCtrl = _T("");
	CString strCancelCtrl = _T("");

	m_CtrlList.clear();
	stSendCtrlMsg st;

	// ホスト名の取得
	CString strHost = CString(mCtrlNameString[theApp.GetSelectStation()]);

	// 局名取得
	CString strStation = CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower();

	// 衛星名取得
	CString strSatellite = _T("");
	m_SatelliteSelCmb.GetLBText(m_SatelliteSelCmb.GetCurSel(), strSatellite);

	// 衛星番号取得
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	int nSatellite = dba.GetIdxSatelliteEqSatDB(strSatellite);

	// CID番号取得
	CString strCID;
	m_CidSelBtn.GetWindowTextW(strCID);

	CString file;

	memset(&m_VResult, 0, sizeof(m_VResult));

	// AGC校正 チェック状態取得
	if (m_AgcChkValue)
	{
		// X
		if (m_AgcXRX2ChkValue)
		{
			// AGC校正名
			CString agcItem = AGC_FILENAME_X;

			// ループバンド
			CString strLb = m_strXRX2LoopBand;
			if (m_strXRX2LoopBand == CURRENT_LBW)
			{
				strLb = m_xrx2_lbw;
			}

			if (strLb.IsEmpty() == FALSE)
			{
				file.Format(_T("agc_%s.tbl_%s_%s"), (LPCTSTR)agcItem, (LPCTSTR)strLb, _T("CUST"));

				strCtrl.Format(CTRL_AGC_X, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strLb, (LPCTSTR)strSatellite);
				strCancelCtrl.Format(CTRL_AGC_X_CANCEL, (LPCTSTR)strSatellite);
				st.type = TYPE_AGC_X;
				st.satellite = strSatellite;
				st.Msg1 = strCtrl;
				st.state = STATE_PRE;
				st.MonString = MON_AGC_X_COND;
				st.ShellType = SHELL_CALIB;
				st.Msg2 = file;
				st.time = dba.GetRdyChkTimeFile(_T("AGC_X")) * 60;
				st.MsgCancel = strCancelCtrl;
				st.RdnsItem = _T("X　");	//m_ItemPropList[]内でユニークにするため
				m_CtrlList.push_back(st);

				m_VResult.agc_x.count++;
			}
			else
			{
				CLogTraceEx::Write(_T(""), _T("CRdnsChk54"), _T("X"), _T("CURRENT_LBW is nothing !!!"), _T(""), nLogEx::info);
			}
		}

		if (m_VResult.agc_x.count > 0)
		{
			m_VResult.agc_x.bResult = TRUE;
		}

		// Ka
		if (m_AgcKaTCRChkValue)
		{
			// AGC校正名
			CString agcItem = AGC_FILENAME_KA;

			// ループバンド
			CString strLb = m_strKaTCRLoopBand;
			if (m_strKaTCRLoopBand == CURRENT_LBW)
			{
				strLb = m_strKa_lbw;
			}

			if (strLb.IsEmpty() == FALSE)
			{
				file.Format(_T("agc_%s.tbl_%s_%s"), (LPCTSTR)agcItem, (LPCTSTR)strLb, _T("CUST"));

				strCtrl.Format(CTRL_AGC_KA, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strLb, (LPCTSTR)strSatellite);
				strCancelCtrl.Format(CTRL_AGC_KA_CANCEL, (LPCTSTR)strSatellite);
				st.type = TYPE_AGC_KA;
				st.satellite = strSatellite;
				st.Msg1 = strCtrl;
				st.state = STATE_PRE;
				st.MonString = MON_AGC_KA_COND;
				st.ShellType = SHELL_CALIB;
				st.Msg2 = file;
				st.time = dba.GetRdyChkTimeFile(_T("AGC_KA")) * 60;
				st.MsgCancel = strCancelCtrl;
				st.RdnsItem = _T("Ka　");	//m_ItemPropList[]内でユニークにするため
				m_CtrlList.push_back(st);

				m_VResult.agc_ka.count++;
			}
			else
			{
				CLogTraceEx::Write(_T(""), _T("CRdnsChk54"), _T("Ka"), _T("CURRENT_LBW is nothing !!!"), _T(""), nLogEx::info);
			}
		}

		if (m_VResult.agc_ka.count > 0)
		{
			m_VResult.agc_ka.bResult = TRUE;
		}
	}

	// 局内ディレイ値計測
	if (m_DlyChkValue)
	{
		// X/X
		if (m_DlyXXChkValue)
		{
			strCtrl.Format(CTRL_RNG_XX, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_RNG_XX_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_RNG;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_RNG_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("RNG_XX")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("X/X");
			m_CtrlList.push_back(st);

			m_VResult.delay.count++;
		}

		// X/X(再生型)
		if (m_DlyXXRChkValue)
		{
			strCtrl.Format(CTRL_RNG_RX, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_RNG_RX_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_RNG;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_RNG_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("RNG_RX")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("X/X(再生型)");
			m_CtrlList.push_back(st);

			m_VResult.delay.count++;
		}

		// X/Ka
		if (m_DlyXKaTCRChkValue)
		{
			strCtrl.Format(CTRL_RNG_XKA, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_RNG_XKA_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_RNG;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_RNG_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("RNG_XKA")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("X/Ka");
			m_CtrlList.push_back(st);

			m_VResult.delay.count++;
		}

		// X/Ka(再生型)
		if (m_DlyXKaTCRRChkValue)
		{
			strCtrl.Format(CTRL_RNG_RXKA, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_RNG_RXKA_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_RNG;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_RNG_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("RNG_RXKA")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("X/Ka(再生型)");
			m_CtrlList.push_back(st);

			m_VResult.delay.count++;
		}

		if (m_VResult.delay.count > 0)
		{
			m_VResult.delay.bResult = TRUE;
		}
	}

	// テレメトリ伝送チェック
	if (m_TLMChkValue)
	{
		// パス番号取得
		CString strTlmPass = _T("");
		m_TlmPathNoEdit.GetWindowTextW(strTlmPass);
		strTlmPass = strTlmPass.Mid(0, 6) + _T("-") + strTlmPass.Mid(6);	// XXXXXXXXXX をXXXXXX-XXXX 形式変換

		// X
		if (m_TLMXChkValue)
		{
			if (m_strTlmBRX != _T(""))
			{
				strCtrl.Format(CTRL_TLM_X_TLMIN, (LPCTSTR)strTlmPass, (LPCTSTR)m_strTlmBRX, nSatellite, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
				strCancelCtrl.Format(CTRL_TLM_X_TLMIN_CANCEL, (LPCTSTR)strSatellite);
				st.type = TYPE_TLM_X;
				st.satellite = strSatellite;
				st.Msg1 = strCtrl;
				st.state = STATE_PRE;
				st.MonString = MON_TLM_X_COND;
				st.ShellType = SHELL_CTRL;
				st.time = dba.GetRdyChkTimeFile(_T("TLM_DX")) * 60;
				st.MsgCancel = strCancelCtrl;
				st.RdnsItem = _T("X　　");	//m_ItemPropList[]内でユニークにするため
				m_CtrlList.push_back(st);

				m_VResult.tlm.count++;
			}
		}

		// Ka
		if (m_TLMKaChkValue)
		{
			if (m_strTlmBRKa != _T(""))
			{
				strCtrl.Format(CTRL_TLM_KA_TLMIN, (LPCTSTR)strTlmPass, (LPCTSTR)m_strTlmBRKa, nSatellite, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
				strCancelCtrl.Format(CTRL_TLM_KA_TLMIN_CANCEL, (LPCTSTR)strSatellite);
				st.type = TYPE_TLM_KA;
				st.satellite = strSatellite;
				st.Msg1 = strCtrl;
				st.state = STATE_PRE;
				st.MonString = MON_TLM_KA_COND;
				st.ShellType = SHELL_CTRL;
				st.time = dba.GetRdyChkTimeFile(_T("TLM_DKA")) * 60;
				st.MsgCancel = strCancelCtrl;
				st.RdnsItem = _T("Ka　　");	//m_ItemPropList[]内でユニークにするため
				m_CtrlList.push_back(st);

				m_VResult.tlm.count++;
			}
		}

		if (m_VResult.tlm.count > 0)
		{
			m_VResult.tlm.bResult = TRUE;
		}
	}

	// コマンド伝送チェック
	if (m_CmdChkValue)
	{
		// パス番号
		CString strCmdPass;
		m_CmdPathNoEdit.GetWindowTextW(strCmdPass);
		strCmdPass = strCmdPass.Mid(0, 6) + _T("-") + strCmdPass.Mid(6);	// XXXXXXXXXX をXXXXXX-XXXX 形式変換

		// X
		if (m_CmdXChkValue)
		{
			if (m_strCmdBRX != _T(""))
			{
				strCtrl.Format(CTRL_CMD_X_CMDOUT, (LPCTSTR)strCmdPass, (LPCTSTR)m_strCmdBRX, nSatellite, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
				strCancelCtrl.Format(CTRL_CMD_X_CMDOUT_CANCEL, (LPCTSTR)strSatellite);
				st.type = TYPE_CMD;
				st.satellite = strSatellite;
				st.Msg1 = strCtrl;
				st.state = STATE_PRE;
				st.MonString = MON_CMD_COND;
				st.ShellType = SHELL_CTRL;
				st.time = dba.GetRdyChkTimeFile(_T("CMD_DX")) * 60;
				st.MsgCancel = strCancelCtrl;
				st.RdnsItem = _T("X　　　");	//m_ItemPropList[]内でユニークにするため
				m_CtrlList.push_back(st);

				m_VResult.cmd.count++;
			}
		}

		if (m_VResult.cmd.count > 0)
		{
			m_VResult.cmd.bResult = TRUE;
		}
	}
}


/*============================================================================*/
/*! CRdnsChk54

-# 強制終了ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54RealtimeStopbtn()
{
	CString title;
	GetWindowText(title);

#if 1
	if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("実行中の校正診断を中止します。\nよろしいですか？"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		return;
	}

	if (m_hThread == NULL)
	{
		return;
	}

	m_ThreadAbort = true;

#else
//
//	if (WaitForSingleObject(m_hThread, 1000L) == WAIT_TIMEOUT)
//	{
//		TerminateThread(m_hThread, 1L);
//	}
//	CloseHandle(m_hThread);
//	m_hThread = NULL;
//
//	BOOL bAGC = FALSE;
//	BOOL bRNG = FALSE;
//	BOOL bTLM = FALSE;
//	BOOL bCMD = FALSE;
//	CString strCtrl = _T("");
//
//	int cnt = (int)m_CtrlList.size();
//	for (int i = 0; i < cnt; ++i)
//	{
//		stSendCtrlMsg &st = m_CtrlList[i];
//#if 0
//		if (st.bRun)
//		{
//			if (st.type == TYPE_AGC && bAGC == FALSE)
//			{
//				strCtrl.Format(CTRL_AGC_CANCEL, (LPCTSTR)st.satellite);
//				theApp.SendControl(strCtrl, _T(""), _T(""), _T(""));
//				bAGC = TRUE;
//			}
//
//			if (st.type == TYPE_RNG && bRNG == FALSE)
//			{
//				strCtrl.Format(CTRL_RNG_CANCEL, (LPCTSTR)st.satellite);
//				theApp.SendControl(strCtrl, _T(""), _T(""), _T(""));
//				bRNG = TRUE;
//			}
//
//			if (st.type == TYPE_TLM && bTLM == FALSE)
//			{
//				strCtrl.Format(CTRL_TLM_CANCEL, (LPCTSTR)st.satellite);
//				theApp.SendControl(strCtrl, _T(""), _T(""), _T(""));
//				bTLM = TRUE;
//			}
//
//			if (st.type == TYPE_CMD && bCMD == FALSE)
//			{
//				strCtrl.Format(CTRL_CMD_CANCEL, (LPCTSTR)st.satellite);
//				theApp.SendControl(strCtrl, _T(""), _T(""), _T(""));
//				bCMD = TRUE;
//			}
//		}
//#endif
//	}
//
//	// 強制終了ボタン非活性化
//	m_AbortBtnCtr.EnableWindow(FALSE);
//
#endif
}


/*============================================================================*/
/*! CRdnsChk54

-# 登録ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54PeriodEntrybtn()
{
	// TLMパスID, CMDパスIDの照合有りで校正診断計画登録実行
	if (RegistPlan(true) == false)
	{
		// 上記関数処理が途中で、TLMパスID, CMDパスIDを合わせる場合は、パスを合わせた後にID照合無しで登録実行
		RegistPlan(false);
	}
}

/*============================================================================*/
/*! CRdnsChk54

-# 校正診断計画登録

@param  bChkPassId： true：TLMパスID、CMDパスIDとAGC校正・局内ディレイ値計画IDを照合する
                     false：計画IDを照合しない
@retval true：終了 / false：途中
*/
/*============================================================================*/
bool CRdnsChk54::RegistPlan(bool bChkPassId)
{
	// 設定値取得
	m_CtrlList.clear();
	GetSetting();

	if (m_CtrlList.size() == 0)
	{
		CString title;
		GetWindowText(title);
		MessageBox(_T("校正診断の対象が選択されていません。"), title, MB_OK | MB_ICONWARNING);
		return true;
	}

	// AGC と DELAYは同ファイル
	//	CString passIdAGC = m_strDay + _T("-9999");
	CString passIdAGC;
	if (m_bNewPlan)
	{
		// 新規計画時は、開始日時からAGC校正・局内ディレイ値計画IDを生成
		passIdAGC = m_PeriodExecStartDay.Format(_T("%y%m%d")) + _T("-9999");
	}
	else
	{
		// 計画変更時は、元の計画IDをAGC校正・局内ディレイ値計画IDに使う
		passIdAGC = m_PassID;
	}

	// 衛星名取得
	CString strSatellite = _T("");
	m_SatelliteSelCmb.GetLBText(m_SatelliteSelCmb.GetCurSel(), strSatellite);

	// TLMのパスID取得
	CString passIdTLM;
	m_TlmPathNoEdit.GetWindowTextW(passIdTLM);
	passIdTLM = passIdTLM.Mid(0, 6) + _T("-") + passIdTLM.Mid(6, 4);

	// CMDのパスID取得
	CString passIdCMD;
	m_CmdPathNoEdit.GetWindowTextW(passIdCMD);
	passIdCMD = passIdCMD.Mid(0, 6) + _T("-") + passIdCMD.Mid(6, 4);

	// CID番号取得
	CString strCID;
	m_CidSelBtn.GetWindowTextW(strCID);

	// 各所要時刻取得
	// AGC＋DELAY
	m_AgcDelayPlanList.clear();
	int time_agc = GetClibTime(0);

	// TLM
	bool bChgPassId = false;
	m_TlmPlanList.clear();
	int time_tlm = GetClibTime(1);
	if (bChkPassId == true && m_TlmPlanList.size() > 0)
	{
		// 新規計画で、TLM IDとAGC校正・局内ディレイ値計画IDが異なる場合
		if (m_bNewPlan == TRUE && passIdTLM != passIdAGC)
		{
			// TLM IDをAGC校正・局内ディレイ値計画IDに合わせるか確認メッセージ
			CString title;
			GetWindowText(title);
			CString strMsg;
			strMsg.Format(_T("以下のパス番号設定に差異があります。\n\n 計画開始日時パス番号 : %s\n テレメトリ伝送チェックパス番号 : %s\n\nテレメトリ伝送チェックのパス番号を計画開始日時パス番号に合わせますか？"), passIdAGC, passIdTLM);
			if (IDYES == MessageBoxHooked(this->m_hWnd, strMsg, title, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2))
			{
				// 計画IDを合わせる
				passIdTLM = passIdAGC;
				m_TlmPathNoEdit.SetWindowTextW(passIdTLM);
				bChgPassId = true;
			}
		}
	}

	// CMD
	m_CmdPlanList.clear();
	int time_cmd = GetClibTime(2);
	if (bChkPassId == true && m_CmdPlanList.size() > 0)
	{
		// 新規計画で、CMD IDとAGC校正・局内ディレイ値計画IDが異なる場合
		if (m_bNewPlan == TRUE && passIdCMD != passIdAGC)
		{
			// CMD IDをAGC校正・局内ディレイ値計画IDに合わせるか確認メッセージ
			CString title;
			GetWindowText(title);
			CString strMsg;
			strMsg.Format(_T("以下のパス番号設定に差異があります。\n\n 計画開始日時パス番号 : %s\n コマンド伝送チェックパス番号 : %s\n\nコマンド伝送チェックのパス番号を計画開始日時パス番号に合わせますか？"), passIdAGC, passIdCMD);
			if (IDYES == MessageBoxHooked(this->m_hWnd, strMsg, title, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2))
			{
				// 計画IDを合わせる
				passIdCMD = passIdAGC;
				m_CmdPathNoEdit.SetWindowTextW(passIdCMD);
				bChgPassId = true;
			}
		}
	}

	if (bChgPassId)
	{
		// TLMかCMDの計画IDを変更する場合は関数を途中終了
		return false;
	}

	// Dummy
	vector<stCalibPlan> dmylist;
	dmylist.clear();

	stPlandata stpd;
	CTime t_start;
	CTime t_end;
	CTimeSpan span;

	stpd.sz_cid = strCID;
	stpd.l_rdychk = 0;
	stpd.l_init = 0;
	stpd.l_mod_sel = 0;
	stpd.l_auto_acq = 0;
	stpd.l_offset = 0;
	stpd.l_a_flag = 0;
	stpd.l_l_flag = 0;
	stpd.l_term_ant = 1;
	stpd.l_cmd_mod = 0;
	stpd.l_rng_mod = 0;
	stpd.l_tx_ramp = 4;
	stpd.l_rx_ramp = 4;
	stpd.l_acq_route = 7;
	stpd.l_pl_sel = 1;
	stpd.l_top_mon = 1;
	stpd.l_ent_angle = 0;
	stpd.l_ext_angle = 0;
	stpd.l_delay_sel = 2;
	stpd.l_offset_val = 0;

	UpdateData();

	t_start = CTime(m_PeriodExecStartDay.GetYear(), m_PeriodExecStartDay.GetMonth(), m_PeriodExecStartDay.GetDay(), m_PeriodExecStartTime.GetHour(), m_PeriodExecStartTime.GetMinute(), 0);

	if ((passIdAGC == passIdTLM) && (passIdAGC == passIdCMD))
	{
		/* AGC(DELAY) と TLM、CMDが同パスIDの場合 */
		// 計画ファイルは1つ。

		// AGC(Delay) + TLM + CMD
		if (m_AgcDelayPlanList.size() > 0 || m_TlmPlanList.size() > 0 || m_CmdPlanList.size() > 0)
		{
			span = CTimeSpan(0, 0, 0, time_agc + time_tlm + time_cmd);
			t_end = t_start + span;

			stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
			stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

			if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdAGC, stpd, m_AgcDelayPlanList, m_TlmPlanList, m_CmdPlanList) != -1)
			{
				// 計画ファイル登録
				COprPlnData::RegistPlanNew(strSatellite, passIdAGC, (m_PassID == passIdAGC) ? FALSE : TRUE);
			}
		}
	}
	else if ((passIdAGC == passIdTLM) && (passIdAGC != passIdCMD))
	{
		/* AGC(DELAY) と TLM が同じパスID、CMDが異なるパスIDの場合 */
		// 計画ファイルは2つ。

		// AGC(Delay) + TLM
		if (m_AgcDelayPlanList.size() > 0 || m_TlmPlanList.size() > 0)
		{
			span = CTimeSpan(0, 0, 0, time_agc + time_tlm);
			t_end = t_start + span;
			stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
			stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

			if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdAGC, stpd, m_AgcDelayPlanList, m_TlmPlanList, dmylist) != -1)
			{
				// 計画ファイル登録
				COprPlnData::RegistPlanNew(strSatellite, passIdAGC, (m_PassID == passIdAGC) ? FALSE : TRUE);
			}
			t_start = t_end;
		}

		// CMD
		if (m_CmdPlanList.size() > 0)
		{
			span = CTimeSpan(0, 0, 0, time_cmd);
			t_end = t_start + span;
			stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
			stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

			if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdCMD, stpd, dmylist, dmylist, m_CmdPlanList) != -1)
			{
				// 計画ファイル登録
				COprPlnData::RegistPlanNew(strSatellite, passIdCMD, (m_PassID == passIdCMD) ? FALSE : TRUE);
			}
		}
	}
	else if ((passIdAGC == passIdCMD) && (passIdAGC != passIdTLM))
	{
		/* AGC(DELAY) と CMD が同じパスID、TLMが異なるパスIDの場合 */
		// 計画ファイルは2つ。

		// AGC(Delay) + CMD
		if (m_AgcDelayPlanList.size() > 0 || m_CmdPlanList.size() > 0)
		{
			span = CTimeSpan(0, 0, 0, time_agc + time_cmd);
			t_end = t_start + span;
			stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
			stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

			if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdAGC, stpd, m_AgcDelayPlanList, dmylist, m_CmdPlanList) != -1)
			{
				// 計画ファイル登録
				COprPlnData::RegistPlanNew(strSatellite, passIdAGC, (m_PassID == passIdAGC) ? FALSE : TRUE);
			}
			t_start = t_end;
		}

		// TLM
		if (m_TlmPlanList.size() > 0)
		{
			span = CTimeSpan(0, 0, 0, time_tlm);
			t_end = t_start + span;
			stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
			stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

			if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdTLM, stpd, dmylist, m_TlmPlanList, dmylist) != -1)
			{
				// 計画ファイル登録
				COprPlnData::RegistPlanNew(strSatellite, passIdTLM, (m_PassID == passIdTLM) ? FALSE : TRUE);
			}
		}
	}
	else if ((passIdAGC != passIdTLM) && (passIdAGC != passIdCMD))
	{
		if (passIdTLM == passIdCMD)
		{
			/* AGC(DELAY) と TLM 、CMDのパスIDが異なり、TLMとCMDが同じパスIDの場合 */
			// 計画ファイルは2つ。

			// AGC(Delay)
			if (m_AgcDelayPlanList.size() > 0)
			{
				span = CTimeSpan(0, 0, 0, time_agc);
				t_end = t_start + span;
				stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
				stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

				if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdAGC, stpd, m_AgcDelayPlanList, dmylist, dmylist) != -1)
				{
					// 計画ファイル登録
					COprPlnData::RegistPlanNew(strSatellite, passIdAGC, (m_PassID == passIdAGC) ? FALSE : TRUE);
				}
				t_start = t_end;
			}

			// TLM + CMD
			if (m_TlmPlanList.size() > 0 || m_CmdPlanList.size() > 0)
			{
				span = CTimeSpan(0, 0, 0, time_tlm + time_cmd);
				t_end = t_start + span;
				stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
				stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

				if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdTLM, stpd, dmylist, m_TlmPlanList, m_CmdPlanList) != -1)
				{
					// 計画ファイル登録
					COprPlnData::RegistPlanNew(strSatellite, passIdTLM, (m_PassID == passIdTLM) ? FALSE : TRUE);
				}
			}
		}
		else
		{
			/* AGC(DELAY) と TLM とCMDが異なるパスIDの場合 */
			// 計画ファイルは3つ。

			// AGC(Delay)
			if (m_AgcDelayPlanList.size() > 0)
			{
				span = CTimeSpan(0, 0, 0, time_agc);
				t_end = t_start + span;
				stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
				stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

				if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdAGC, stpd, m_AgcDelayPlanList, dmylist, dmylist) != -1)
				{
					// 計画ファイル登録
					COprPlnData::RegistPlanNew(strSatellite, passIdAGC, (m_PassID == passIdAGC) ? FALSE : TRUE);
				}
				t_start = t_end;
			}

			// TLM
			if (m_TlmPlanList.size() > 0)
			{
				span = CTimeSpan(0, 0, 0, time_tlm);
				t_end = t_start + span;
				stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
				stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

				if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdTLM, stpd, dmylist, m_TlmPlanList, dmylist) != -1)
				{
					// 計画ファイル登録
					COprPlnData::RegistPlanNew(strSatellite, passIdTLM, (m_PassID == passIdTLM) ? FALSE : TRUE);
				}
				t_start = t_end;
			}

			// CMD
			if (m_CmdPlanList.size() > 0)
			{
				span = CTimeSpan(0, 0, 0, time_cmd);
				t_end = t_start + span;
				stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
				stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

				if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdCMD, stpd, dmylist, dmylist, m_CmdPlanList) != -1)
				{
					// 計画ファイル登録
					COprPlnData::RegistPlanNew(strSatellite, passIdCMD, (m_PassID == passIdCMD) ? FALSE : TRUE);
				}
			}
		}
	}

	return true;
}

/*============================================================================*/
/*! CRdnsChk54

-# 校正診断種別所要時刻取得

@param  type = 0:AGC+Delay、1:TLM、2：CMD
@retval なし
*/
/*============================================================================*/
int CRdnsChk54::GetClibTime(int type)
{
	int time = 0;

	stCalibPlan st;
	CString ctrl, prm;

	for (int i = 0; i < (int)m_CtrlList.size(); ++i)
	{
		if (type == 0)
		{
			if( (m_CtrlList[i].type == TYPE_AGC_X)  ||
				(m_CtrlList[i].type == TYPE_AGC_KA) ||
				(m_CtrlList[i].type == TYPE_RNG) )
			{
				time += m_CtrlList[i].time;

				st.time = m_CtrlList[i].time;
#if 1
				AfxExtractSubString(ctrl, m_CtrlList[i].Msg1, 0, ' ');
				AfxExtractSubString(prm, m_CtrlList[i].Msg1, 1, ' ');
				st.ctrl = ctrl + _T(",") + prm;
#else
//				st.ctrl = m_CtrlList[i].Msg1;
#endif

				m_AgcDelayPlanList.push_back(st);
			}
		}
		else if (type == 1)
		{
			if( (m_CtrlList[i].type == TYPE_TLM_X) ||
				(m_CtrlList[i].type == TYPE_TLM_KA) )
			{
				time += m_CtrlList[i].time;

				st.time = m_CtrlList[i].time;
#if 1
				AfxExtractSubString(ctrl, m_CtrlList[i].Msg1, 0, ' ');
				AfxExtractSubString(prm, m_CtrlList[i].Msg1, 1, ' ');
				st.ctrl = ctrl + _T(",") + prm;
#else
//				st.ctrl = m_CtrlList[i].Msg1;
#endif
				m_TlmPlanList.push_back(st);
			}
		}
		else if (type == 2)
		{
			if (m_CtrlList[i].type == TYPE_CMD)
			{
				time += m_CtrlList[i].time;

				st.time = m_CtrlList[i].time;
#if 1
				AfxExtractSubString(ctrl, m_CtrlList[i].Msg1, 0, ' ');
				AfxExtractSubString(prm, m_CtrlList[i].Msg1, 1, ' ');
				st.ctrl = ctrl + _T(",") + prm;
#else
//				st.ctrl = m_CtrlList[i].Msg1;
#endif
				m_CmdPlanList.push_back(st);
			}
		}
	}

	return time;
}

/*============================================================================*/
/*! CRdnsChk54

-# CIDボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnBnClickedRdns54CidBtn()
{
	CString satelliete;
	CString cid;
	m_SatelliteSelCmb.GetWindowTextW(satelliete);
	m_CidSelBtn.GetWindowTextW(cid);

	// CID選択画面表示
	CCidSelect dlg;
	dlg.SetSatelliteName(satelliete);
	dlg.SetCID(cid);
	if (dlg.DoModal() == IDOK)
	{
		stCIDParam st = CCIDSlctData::TrimInvalidString(dlg.GetParameter());
		m_CidSelBtn.SetWindowTextW(st.CID);

		// テレメトリビットレート
		m_strTlmBRX = st.TlmBitRateX;
		m_strTlmBRKa = st.TlmBitRateKa;

		// コマンドビットレート
		m_strCmdBRX = st.CmdBitRateX;

		// テレメトリビットレートコントロール設定
		// X
		if (i_bitrate_x2 < 0 || m_strTlmBRX == _T(""))
		{
			m_TLMXChkValue = FALSE;
			m_TLM_XCtrl.EnableWindow(FALSE);
		}
		else
		{
			m_TLM_XCtrl.EnableWindow(TRUE);
		}

		// Ka
		if (i_bitrate_ka < 0 || m_strTlmBRKa == _T(""))
		{
			m_TLMKaChkValue = FALSE;
			m_TLM_KaCtrl.EnableWindow(FALSE);
		}
		else
		{
			m_TLM_KaCtrl.EnableWindow(TRUE);
		}

		UpdateData(FALSE);

		// 終了時刻更新
		ChangeRdyChkEndTime();
	}
}

/*============================================================================*/
/*! CRdnsChk54

-# メッセージ処理

@param  message	：メッセージの識別子
@param  wParam	：メッセージの最初のパラメータ
@param  lParam	：メッセージの 2 番目のパラメータ
@retval 処理結果
*/
/*============================================================================*/
LRESULT CRdnsChk54::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case eMessage_ChildDestroy:
		m_LogDlg = NULL;
		break;
	case eRdnsChk54_ResultOpen:
		ResultFileOpen((int)lParam);
		break;
	default:
		return CDialogBase::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}


/*============================================================================*/
/*! CRdnsChk54

-# ログダイアログ終了処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::CloseLogDlg(bool bDestroy/*=TRUE*/)
{
	if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
	{
		m_ResultDlg->DestroyWindow();
		m_ResultDlg = NULL;
	}
}


/*============================================================================*/
/*! CRdnsChk54

-# 衛星選択コンボボックス変更処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::OnCbnSelchangeRdns54SselCmb()
{
	CString satellite;
	int idx = m_SatelliteSelCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}

	m_SatelliteSelCmb.GetLBText(idx, satellite);

	BOOL bCid = TRUE;
	CString strCid;
	if (satellite != _T(""))
	{
		map<CString, map<CString, stCIDParam>> &cidparam = theApp.GetSatelliteData().m_CIDSlctData.GetCidParamList();
		vector<CString> &cList = theApp.GetSatelliteData().m_CIDSlctData.GetCidList(satellite);
		if (cList.size() == 0)
		{
			m_CidSelBtn.SetWindowTextW(CID_NA);
			m_CidSelBtn.EnableWindow(FALSE);
			bCid = FALSE;
		}
		else
		{
			stOpeProp stProp;
			theApp.GetSatelliteData().ReadPropertyFile(satellite, stProp);
			strCid = CString(stProp.cid);
			auto itrl = std::find(cList.begin(), cList.end(), strCid);
			if (itrl == cList.end())
			{
				// デフォルトCIDがCIDファイルに無い場合、一番最初のCIDをセット。
				strCid = cList[0];
			}

			m_CidSelBtn.EnableWindow(TRUE);
			m_CidSelBtn.SetWindowTextW(strCid);
		}

		// 衛星固有情報取得
		GetEqSatDBData(satellite);

		if (bCid)
		{
			stCIDParam stCid = CCIDSlctData::TrimInvalidString(cidparam[satellite][strCid]);

			// テレメトリビットレート
			m_strTlmBRX = stCid.TlmBitRateX;
			m_strTlmBRKa = stCid.TlmBitRateKa;

			// コマンドビットレート
			m_strCmdBRX = stCid.CmdBitRateX;
		}
		else
		{
			// テレメトリビットレート
			m_strTlmBRX = _T("");
			m_strTlmBRKa = _T("");

			// コマンドビットレート
			m_strCmdBRX = _T("");
		}

		// 校正診断用コントロール設定
		SetWindowEnable(satellite);

		// Current LoopBand取得
		GetCurrentLWB();

		UpdateData(FALSE);

		// 終了時刻更新
		ChangeRdyChkEndTime();
	}
}

/*============================================================================*/
/*! CRdnsChk54

-# 衛星固有情報DB情報取得

@param  satname：衛星名
@retval 0：正常終了、0以外：異常終了
*/
/*============================================================================*/
int CRdnsChk54::GetEqSatDBData(CString satname)
{
	struct satinf_t *satinf_adr;
	struct satinf_t *original_adr;
	char *sattbl_adr;

	struct xtx_t *xtx_t_adr;			// X帯送信設備使用有無情報テーブルアドレス
	struct agcrx54_t *agcrx54_t_adr;	// AGC校正対象受信機情報テーブルアドレス
	struct rarrband_t *rarrband_t_adr;	// RARR使用帯域情報テーブルアドレス
	struct mac_t	*mac_t_adr = NULL;

	char *psz_tbl[MAC_MAX];				// マクロ名を格納するテーブルの先頭アドレス
	char *psz_vec[MAC_MAX];				// マクロ情報アドレスを格納するテーブルの

	if (satname == _T(""))
	{
		return -1;
	}

	// DBアクセスクラス
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();

	vector<CString>			&saNameList = dba.GetSatNameList();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();

	if (saNameList.size() == 0 || satAdrList.size() == 0)
	{
		return -1;
	}

	satinf_adr = (struct satinf_t *)((__int64*)satAdrList[satname]);
	original_adr = satinf_adr;
	sattbl_adr = (char *)satinf_adr;

	// 使用周波数帯
	l_useband = satinf_adr->l_useband;

	// 近地球／深宇宙 (STDN,DSN)
	l_level = satinf_adr->l_level;

	// 衛星番号
	l_satno = satinf_adr->l_satno;

	while (1)
	{
		satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
		if (satinf_adr->l_id == ENDOFSAT_ID)
		{
			/*識別ＩＤが終了の場合*/
			break;
		}

		// X帯送信設備使用有無情報
		if (satinf_adr->l_id == XTX_ID)
		{
			xtx_t_adr = (struct xtx_t *)satinf_adr;
			l_x_tx = xtx_t_adr->l_x_tx;
		}

		// AGC校正対象受信機情報
		if (satinf_adr->l_id == AGCRX_ID)
		{
			agcrx54_t_adr = (struct agcrx54_t *)satinf_adr;
			l_x_dsntcr1 = agcrx54_t_adr->l_x_dsntcr1;		// X帯DSN-MODEM1
			l_ka_dsntcr1 = agcrx54_t_adr->l_ka_dsntcr1;		// Ka帯DSN-MODEM1
		}

		// ＸRARR使用帯域情報
		if (satinf_adr->l_id == RARRBAND_ID)
		{
			rarrband_t_adr = (struct rarrband_t *)satinf_adr;
			l_rarr_band = rarrband_t_adr->l_rarr_band;
		}

		sattbl_adr += satinf_adr->l_len;
	}

	satinf_adr = original_adr;				/*衛星本体情報先頭アドレスセット*/

	/*------------------*/
	/* X2 BITRATEの取得 */
	/*------------------*/
	satinf_adr = original_adr;				/*衛星本体情報先頭アドレスセット*/
	i_bitrate_x2 = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(BIT_X2).GetBuffer(), psz_tbl, psz_vec, MAC_MAX);

	/*-----------------*/
	/* Ka BITRATEの取得 */
	/*-----------------*/
	satinf_adr = original_adr;				/*衛星本体情報先頭アドレスセット*/
	i_bitrate_ka = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(BIT_Ka).GetBuffer(), psz_tbl, psz_vec, MAC_MAX);

#if 0 // trial
//	/*-----------------*/
//	/* INIT.SS-TCRDEMの取得 */
//	/*-----------------*/
//	satinf_adr = original_adr;				/*衛星本体情報先頭アドレスセット*/
//	int ret = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(_T("INIT.SS-TCRDEM")).GetBuffer(), psz_tbl, psz_vec, MAC_MAX);
//	if (ret > 0)
//	{
//		int i_ret = dba.ud_macstsadr_to_exectbl((char*)psz_tbl, psz_tbl, psz_vec, MAC_MAX);
//
//		int a = 0;
//	}
//
//	
//	ret = dba.ud_macsts_to_exectbl((char*)satinf_adr, CStringA(_T("INIT.SS-TCRDEM")).GetBuffer(), psz_tbl, psz_vec, MAC_MAX);
//	if (ret > 0)
//	{
//
//		int a = 0;
//	}
#endif

	return 0;
}

/*============================================================================*/
/*! CRdnsChk54

-# 校正診断用コントロール活性/非活性設定

@param  satname ： 衛星名
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::SetWindowEnable(CString satname)
{
	/********************************************/
	/* AGC校正 */
	m_AgcChkValue = FALSE;

	// AGC校正 X
	m_AgcXRX2ChkValue = FALSE;
	if (l_x_dsntcr1 == AGCRX_Y_ID)
	{
		m_AGC_X_RX2Ctrl.EnableWindow(TRUE);
		m_AGC_X_RX2_LBCtrl.EnableWindow(TRUE);
		m_AGC_X_RX2_LBBCtrl.EnableWindow(TRUE);
		m_AGC_X_RX2_SBCtrl.EnableWindow(TRUE);
	}
	else
	{
		m_AGC_X_RX2Ctrl.EnableWindow(FALSE);
		m_AGC_X_RX2_LBCtrl.EnableWindow(FALSE);
		m_AGC_X_RX2_LBBCtrl.EnableWindow(FALSE);
		m_AGC_X_RX2_SBCtrl.EnableWindow(FALSE);
	}

	m_strXRX2LoopBand = CURRENT_LBW;

	// AGC校正 Ka
	m_AgcKaTCRChkValue = FALSE;
	m_AGC_Ka_TCRCtrl.EnableWindow(FALSE);			//Ka チェックボタン
	m_AGC_Ka_TCR_LBCtrl.EnableWindow(FALSE);		//Ka ループバンド テキスト
	m_AGC_Ka_TCR_LBBCtrl.EnableWindow(FALSE);		//Ka ループバンド 変更ボタン
	m_AGC_Ka_TCR_SBCtrl.EnableWindow(FALSE);		//Ka ステップ数/積分時間 変更ボタン
	if (l_ka_dsntcr1 == AGCRX_Y_ID)
	{
		m_AGC_Ka_TCRCtrl.EnableWindow(TRUE);		//Ka チェックボタン
		m_AGC_Ka_TCR_LBCtrl.EnableWindow(TRUE);		//Ka ループバンド テキスト
		m_AGC_Ka_TCR_LBBCtrl.EnableWindow(TRUE);	//Ka ループバンド 変更ボタン
		m_AGC_Ka_TCR_SBCtrl.EnableWindow(TRUE);		//Ka ステップ数/積分時間 変更ボタン
	}

	m_strKaTCRLoopBand = CURRENT_LBW;

	/********************************************/
	/* 局内ディレイ値計測 */
	m_DlyChkValue = FALSE;

	m_DlyXXChkValue = FALSE;
	m_DlyXXRChkValue = FALSE;
	m_DlyXKaTCRChkValue = FALSE;
	m_DlyXKaTCRRChkValue = FALSE;

	m_Delay_XXCtrl.EnableWindow(FALSE);			//X/X チェックボタン
	m_Delay_XXRCtrl.EnableWindow(FALSE);		//X/X(再生型) チェックボタン
	m_Delay_XKaTCRCtrl.EnableWindow(FALSE);		//X/Ka チェックボタン
	m_Delay_XKaTCRRCtrl.EnableWindow(FALSE);	//X/Ka(再生型) チェックボタン

	if (l_rarr_band == RARRBAND_X_ID)
	{
		m_Delay_XXCtrl.EnableWindow(TRUE);		//X/X チェックボタン
	}
	else if (l_rarr_band == RARRBAND_RX_ID)
	{
		m_Delay_XXCtrl.EnableWindow(TRUE);		//X/X チェックボタン
		m_Delay_XXRCtrl.EnableWindow(TRUE);		//X/X(再生型) チェックボタン
	}
	else if (l_rarr_band == RARRBAND_XKA_ID)
	{
		m_Delay_XXCtrl.EnableWindow(TRUE);		//X/X チェックボタン
		m_Delay_XKaTCRCtrl.EnableWindow(TRUE);	//X/Ka チェックボタン
	}
	else if (l_rarr_band == RARRBAND_RXKA_ID)
	{
		m_Delay_XXCtrl.EnableWindow(TRUE);		//X/X チェックボタン
		m_Delay_XXRCtrl.EnableWindow(TRUE);		//X/X(再生型) チェックボタン
		m_Delay_XKaTCRCtrl.EnableWindow(TRUE);	//X/Ka チェックボタン
		m_Delay_XKaTCRRCtrl.EnableWindow(TRUE);	//X/Ka(再生型) チェックボタン
	}

	/********************************************/
	/* テレメトリ伝送チェック */
	m_TLMChkValue = FALSE;

	// X
	m_TLMXChkValue = FALSE;
	if (i_bitrate_x2 < 0 || m_strTlmBRX == _T(""))
	{
		m_TLM_XCtrl.EnableWindow(FALSE);
	}
	else
	{
		m_TLM_XCtrl.EnableWindow(TRUE);
	}

	// Ka
	m_TLMKaChkValue = FALSE;
	if (i_bitrate_ka < 0 || m_strTlmBRKa == _T(""))
	{
		m_TLM_KaCtrl.EnableWindow(FALSE);
	}
	else
	{
		m_TLM_KaCtrl.EnableWindow(TRUE);
	}

	/********************************************/
	/* コマンド伝送チェック */
	m_CmdChkValue = FALSE;

	m_CmdXChkValue = FALSE;

	if (l_x_tx == XTX_Y_ID)
	{
		m_CMD_XCtrl.EnableWindow(TRUE);
	}
	else
	{
		m_CMD_XCtrl.EnableWindow(FALSE);
	}

	UpdateData(FALSE);
}

/*============================================================================*/
/*! CRdnsChk54

-# CURRENT_LWBを監視データより取得する。

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsChk54::GetCurrentLWB()
{
	/* デフォルトのループバンド設定はCURRENT_LWBの為、監視より設定値を取得する*/

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	// 局名
	CString station = CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower();

	// 衛星名
	CString strSat = _T("");
	m_SatelliteSelCmb.GetLBText(m_SatelliteSelCmb.GetCurSel(), strSat);
	CString satellite = strSat;

	CString cust, dflt;
	CString agcItem;

	// X CURRENT_LBW取得(監視データより取得)
	m_xrx2_lbw = _T("");
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), X_RX_LBW);
	if (obs_adr != NULL)
	{
		m_xrx2_lbw = CString(obs_adr->sz_sts_name);

		/* テーブルファイル取得 */
		agcItem = AGC_FILENAME_X;
		// CUST
		cust.Format(_T("agc_%s.tbl_%s_%s"), (LPCTSTR)agcItem, (LPCTSTR)m_xrx2_lbw, _T("CUST"));
		// DFLT
		dflt.Format(_T("agc_%s.tbl_%s_%s"), (LPCTSTR)agcItem, (LPCTSTR)m_xrx2_lbw, _T("DFLT"));

		theApp.GetCalib(station, satellite, cust, dflt);
	}

	// Ka CURRENT_LBW取得(監視データより取得)
	m_strKa_lbw = _T("");
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), KA_RX_LBW);
	if (obs_adr != NULL)
	{
		m_strKa_lbw = CString(obs_adr->sz_sts_name);

		/* テーブルファイル取得 */
		agcItem = AGC_FILENAME_KA;
		// CUST
		cust.Format(_T("agc_%s.tbl_%s_%s"), (LPCTSTR)agcItem, (LPCTSTR)m_strKa_lbw, _T("CUST"));
		// DFLT
		dflt.Format(_T("agc_%s.tbl_%s_%s"), (LPCTSTR)agcItem, (LPCTSTR)m_strKa_lbw, _T("DFLT"));

		theApp.GetCalib(station, satellite, cust, dflt);
	}
}

/*============================================================================*/
/*! CRdnsChk54

-# 制御コマンド送信スレッド

@param
@retval

*/
/*============================================================================*/
UINT WINAPI CRdnsChk54::SendCtrlThread(LPVOID pParam)
{
	CRdnsChk54* pThis = reinterpret_cast<CRdnsChk54*>(pParam);
	_ASSERTE(pThis != NULL);

	UINT nStation = theApp.GetSelectStation();
	CString station = CString(mStationSimpleString[nStation]).MakeLower();

	vector<stSendCtrlMsg>::iterator itr, itr2, itr_tmp;

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();
	CString res;

	// 校正診断実行中フラグON
	pThis->m_ThreadRun = true;

	// 最小タイマ分解能を設定(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = 0;
	bool bTimeout = false;

	while (1/*pThis->ThreadEnd() == false*/)
	{
		bool bRunWait = false;
		// リストに登録されているコマンドが実行中の項目が存在すれば待機する
		for (itr = pThis->m_CtrlList.begin(); itr != pThis->m_CtrlList.end(); ++itr)
		{
			if (pThis->ThreadAbort() == true)
			{
				break;
			}

			if ((*itr).state == STATE_WAIT || (*itr).state == STATE_RUN)
			{
				// コマンド終了待機
				bRunWait = true;
				break;
			}
		}

		if (pThis->ThreadAbort() == true)
		{
			break;
		}

		if (itr != pThis->m_CtrlList.end())
		{
			// 実行中のコマンドが存在したので待機
			while (pThis->ThreadAbort() == false)
			{
				// 終了待機
				// 実行中のコマンドが終了したかをチェックする
				// theAppからコマンドが終了したかを確認する
				if (theApp.GetControlCommand(nStation, mRdnsRunLogItem, (*itr).MonString) == STATE_STP)
				{
					// タイマークリア
					dwStart = 0;

					// コマンドが終了してるので「STATE_STP」を設定する
					(*itr).state = STATE_STP;

					// 結果ファイル表示
					if ((*itr).type == TYPE_AGC_X)
					{
						// 結果取得
						obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), RESULT_AGCX);
						if (obs_adr != NULL)
						{
							res = CString(obs_adr->sz_sts_name);

							// 結果ダイアログへメッセージ送信
							if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
							{
								pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
							}
						}
//#ifdef _DEBUG
//						res = _T("OK");
//
//						// 結果ダイアログへメッセージ送信
//						if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
//						{
//							pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
//						}
//#endif

						if (pThis->m_VResult.agc_x.bResult == TRUE)
						{
							pThis->m_VResult.agc_x.count--;
							if (pThis->m_VResult.agc_x.count <= 0)
							{
								// AGC X 結果ファイル表示
								//pThis->ResultFileOpen(TYPE_AGC_X);
								pThis->SendMessage(eRdnsChk54_ResultOpen, 0, (LPARAM)TYPE_AGC_X);
								pThis->m_VResult.agc_x.bResult = FALSE;
							}
						}
					}
					else if ((*itr).type == TYPE_AGC_KA)
					{
						// 結果取得
						obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), RESULT_AGCKA);
						if (obs_adr != NULL)
						{
							res = CString(obs_adr->sz_sts_name);

							// 結果ダイアログへメッセージ送信
							if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
							{
								pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
							}
						}
//#ifdef _DEBUG
//						res = _T("OK");
//
//						// 結果ダイアログへメッセージ送信
//						if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
//						{
//							pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
//						}
//#endif

						if (pThis->m_VResult.agc_ka.bResult == TRUE)
						{
							pThis->m_VResult.agc_ka.count--;
							if (pThis->m_VResult.agc_ka.count <= 0)
							{
								// AGC X 結果ファイル表示
								//pThis->ResultFileOpen(TYPE_AGC_KA);
								pThis->SendMessage(eRdnsChk54_ResultOpen, 0, (LPARAM)TYPE_AGC_KA);
								pThis->m_VResult.agc_ka.bResult = FALSE;
							}
						}
					}
					else if ((*itr).type == TYPE_RNG)
					{
						// 結果取得
						obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), RESULT_RNG);
						if (obs_adr != NULL)
						{
							res = CString(obs_adr->sz_sts_name);

							// 結果ダイアログへメッセージ送信
							if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
							{
								pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
							}
						}
//#ifdef _DEBUG
//						res = _T("OK");
//
//						// 結果ダイアログへメッセージ送信
//						if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
//						{
//							pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
//						}
//#endif

						if (pThis->m_VResult.delay.bResult == TRUE)
						{
							pThis->m_VResult.delay.count--;
							if (pThis->m_VResult.delay.count <= 0)
							{
								// Delay 結果ファイル表示
								//pThis->ResultFileOpen(TYPE_RNG);
								pThis->SendMessage(eRdnsChk54_ResultOpen, 0, (LPARAM)TYPE_RNG);
								pThis->m_VResult.delay.bResult = FALSE;
							}
						}
					}
					else if (((*itr).type == TYPE_TLM_X) || ((*itr).type == TYPE_TLM_KA))
					{
						// 結果取得
						obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), RESULT_TLM);
						if (obs_adr != NULL)
						{
							res = CString(obs_adr->sz_sts_name);

							// 結果ダイアログへメッセージ送信
							if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
							{
								pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
							}
						}
//#ifdef _DEBUG
//						res = _T("OK");
//
//						// 結果ダイアログへメッセージ送信
//						if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
//						{
//							pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
//						}
//#endif

						if (pThis->m_VResult.tlm.bResult == TRUE)
						{
							pThis->m_VResult.tlm.count--;
							if (pThis->m_VResult.tlm.count <= 0)
							{
								// Tlm 結果ファイル表示
								//pThis->ResultFileOpen(TYPE_TLM_S);
								pThis->SendMessage(eRdnsChk54_ResultOpen, 0, (LPARAM)TYPE_TLM_X);
								pThis->m_VResult.tlm.bResult = FALSE;
							}
						}
					}
					else if ((*itr).type == TYPE_CMD)
					{
						// 結果取得
						obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), RESULT_CMD);
						if (obs_adr != NULL)
						{
							res = CString(obs_adr->sz_sts_name);

							// 結果ダイアログへメッセージ送信
							if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
							{
								pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
							}
						}
//#ifdef _DEBUG
//						res = _T("OK");
//
//						// 結果ダイアログへメッセージ送信
//						if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
//						{
//							pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
//						}
//#endif

						if (pThis->m_VResult.cmd.bResult == TRUE)
						{
							pThis->m_VResult.cmd.count--;
							if (pThis->m_VResult.cmd.count <= 0)
							{
								// Cmd 結果ファイル表示
								//pThis->ResultFileOpen(TYPE_CMD);
								pThis->SendMessage(eRdnsChk54_ResultOpen, 0, (LPARAM)TYPE_CMD);
								pThis->m_VResult.cmd.bResult = FALSE;
							}
						}
					}

					break;
				}

				// 測定開始からの経過時間（ミリ秒）の計算
				if (dwStart != 0 && 
					timeGetTime() - dwStart >= 30000 &&
					theApp.GetControlCommand(nStation, mRdnsRunLogItem, (*itr).MonString) < STATE_RUN)
				{
					// 30秒を超えていても開始になっていない場合は、タイムアウトする
					// メインフレームへメッセージ送信
					theApp.m_pMainWnd->PostMessageW(eMessage_RdnsChkTimeout, 0, 0);

					// スレッド終了
					pThis->m_ThreadAbort = true;
					bTimeout = true;
					break;
				}

#if 0
//				if (pThis->RdnsChkAbort() == true)
//				{
//					theApp.SendControlNoUI((*itr).MsgCancel, _T(""), _T(""), _T(""));
//					pThis->RdnsChkAbortClear();
//				}
#endif

				Sleep(10);
			}

		}
		else
		{
			// 実行中のコマンドが存在しないので先頭から「STATE_PRE」のコマンドを実行する
			for (itr = pThis->m_CtrlList.begin(); itr != pThis->m_CtrlList.end(); ++itr)
			{
				if (pThis->ThreadAbort() == true)
					break;

				if ((*itr).state == STATE_PRE)
				{
					{
						// コマンド実行
						if ((*itr).ShellType == SHELL_CALIB)
						{
							theApp.RequestCalib(station, (*itr).satellite, (*itr).Msg2, (*itr).Msg1);
						}
						else
						{
							if ((*itr).type == TYPE_CMD)
							{
								// コマンド伝送の場合は、EXCTR ROUTE選択制御を先に送る
								CString strExtcrRouteVal;
								pThis->m_ExctrRouteCombo.GetLBText(pThis->m_ExctrRouteCombo.GetCurSel(), strExtcrRouteVal);
								theApp.SendControl(CString(CMD_EXCTR_ROUTE), strExtcrRouteVal, _T(""), _T(""));
							}

							theApp.SendControlNoUI((*itr).Msg1, _T(""), _T(""), _T(""));
						}

						// theAppにコマンド実行したことを通知
						(*itr).item = mRdnsRunLogItem;
						(*itr).state = STATE_WAIT;
						(*itr).station = nStation;
						theApp.SetControlCommand((*itr));

						// タイマーセット
						dwStart = timeGetTime();

						break;
					}
				}
			}

			if (itr == pThis->m_CtrlList.end())
			{
				// 全てのコマンドが実行された。
				break;
			}

			if (pThis->ThreadAbort() == true)
			{
				break;
			}
		}
	}

	if (pThis->ThreadAbort() == true)
	{
		CString strCtrl;
		CString res = _T("NG");
		int flg = 0;
		for (itr = pThis->m_CtrlList.begin(); itr != pThis->m_CtrlList.end(); ++itr)
		{
			if ((*itr).state == STATE_WAIT || (*itr).state == STATE_RUN)
			{
				// 「強制終了」が発行されたので実行中のコマンドを強制終了させる
				// 強制終了シェルを実行
#if 1
				if (pThis->RdnsChkAbort() == false)
				{
					if (bTimeout == false)
					{
						// タイムアウトでない場合は、校正診断キャンセル制御を送信
						theApp.SendControlNoUI((*itr).MsgCancel, _T(""), _T(""), _T(""));
					}

					if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
					{
						pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
					}

					// 結果ファイル表示
					if ((*itr).type == TYPE_AGC_X)
					{
						if (pThis->m_VResult.agc_x.bResult == TRUE)
						{
							// AGC X 結果ファイル表示
							pThis->SendMessage(eRdnsChk54_ResultOpen, 0, (LPARAM)TYPE_AGC_X);
							pThis->m_VResult.agc_x.bResult = FALSE;
						}
					}
					else if ((*itr).type == TYPE_AGC_KA)
					{
						if (pThis->m_VResult.agc_ka.bResult == TRUE)
						{
							// AGC Ka 結果ファイル表示
							pThis->SendMessage(eRdnsChk54_ResultOpen, 0, (LPARAM)TYPE_AGC_KA);
							pThis->m_VResult.agc_ka.bResult = FALSE;
						}
					}
					else if ((*itr).type == TYPE_RNG)
					{
						if (pThis->m_VResult.delay.bResult == TRUE)
						{
							// Delay 結果ファイル表示
							pThis->SendMessage(eRdnsChk54_ResultOpen, 0, (LPARAM)TYPE_RNG);
							pThis->m_VResult.delay.bResult = FALSE;
						}
					}
					else if (((*itr).type == TYPE_TLM_X) || ((*itr).type == TYPE_TLM_KA))
					{
						if (pThis->m_VResult.tlm.bResult == TRUE)
						{
							// Tlm 結果ファイル表示
							pThis->SendMessage(eRdnsChk54_ResultOpen, 0, (LPARAM)TYPE_TLM_X);
							pThis->m_VResult.tlm.bResult = FALSE;
						}
					}
					else if ((*itr).type == TYPE_CMD)
					{
						if (pThis->m_VResult.cmd.bResult == TRUE)
						{
							// Cmd 結果ファイル表示
							pThis->SendMessage(eRdnsChk54_ResultOpen, 0, (LPARAM)TYPE_CMD);
							pThis->m_VResult.cmd.bResult = FALSE;
						}
					}
				}
#else
//				if (((flg & 0x01) == 0) && ((*itr).type == TYPE_AGC_S || (*itr).type == TYPE_AGC_X))
//				{
//					// AGC
//					strCtrl.Format(CTRL_AGC_CANCEL, (LPCTSTR)(*itr).satellite);
//					theApp.SendControlNoUI(strCtrl, _T(""), _T(""), _T(""));
//					flg |= 0x01;
//				}
//				else if (((flg & 0x02) == 0) && ((*itr).type == TYPE_RNG))
//				{
//					// RNG
//					strCtrl.Format(CTRL_RNG_CANCEL, (LPCTSTR)(*itr).satellite);
//					theApp.SendControlNoUI(strCtrl, _T(""), _T(""), _T(""));
//					flg |= 0x02;
//				}
//				else if (((flg & 0x04) == 0) && ((*itr).type == TYPE_TLM_S || (*itr).type == TYPE_TLM_X))
//				{
//					// TLM
//					strCtrl.Format(CTRL_TLM_CANCEL, (LPCTSTR)(*itr).satellite);
//					theApp.SendControlNoUI(strCtrl, _T(""), _T(""), _T(""));
//					flg |= 0x04;
//				}
//				else if (((flg & 0x08) == 0) && ((*itr).type == TYPE_CMD))
//				{
//					// CMD
//					strCtrl.Format(CTRL_CMD_CANCEL, (LPCTSTR)(*itr).satellite);
//					theApp.SendControlNoUI(strCtrl, _T(""), _T(""), _T(""));
//					flg |= 0x08;
//				}
#endif
				// theAppに対して終了処理を行う
				theApp.ClearControlCommand(nStation, mRdnsRunLogItem, (*itr).MonString);
			}
		}
		pThis->m_CtrlList.clear();
	}

	if (pThis->RdnsChkAbort() == false)
	{
		// 実行ボタン活性化
		pThis->GetDlgItem(IDC_RDNS54_REALTIME_RUNBTN)->EnableWindow(TRUE);

		// 登録ボタン活性化
		pThis->GetDlgItem(IDC_RDNS54_PERIOD_ENTRYBTN)->EnableWindow(TRUE);

		// 強制終了ボタン非活性化
		pThis->m_AbortBtnCtr.EnableWindow(FALSE);

		// 衛星選択コンボボックス活性化
		pThis->m_SatelliteSelCmb.EnableWindow(TRUE);

		// CID選択ボタン活性化
		pThis->m_CidSelBtn.EnableWindow(TRUE);

		if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
		{
			pThis->m_ResultDlg->SendMessage(eRdnsChk_End, 0, 0);
		}
	}

	// 校正診断実行中フラグOFF
	pThis->m_ThreadRun = false;

	return 0;
}

/*============================================================================*/
/*! CRdnsChk54

-# 実行予約-校正診断終了予定日時変更

@param
@retval

*/
/*============================================================================*/
void CRdnsChk54::ChangeRdyChkEndTime()
{
	// チェック有無確認
	UpdateData();

	CDBAccess &dba = theApp.GetSatelliteData().GetDBAccessCls();

	m_TimeSpan = 0;

	// AGC校正
	if (m_AgcChkValue)
	{
		// X
		if (m_AgcXRX2ChkValue)
		{
			m_TimeSpan += dba.GetRdyChkTimeFile(_T("AGC_X")) * 60;
		}

		// Ka
		if (m_AgcKaTCRChkValue)
		{
			m_TimeSpan += dba.GetRdyChkTimeFile(_T("AGC_KA")) * 60;
		}
	}

	// 局内ディレイ値計測
	if (m_DlyChkValue)
	{
		// X/X
		if (m_DlyXXChkValue)
		{
			m_TimeSpan += dba.GetRdyChkTimeFile(_T("RNG_XX")) * 60;
		}

		// X/X(再生型)
		if (m_DlyXXRChkValue)
		{
			m_TimeSpan += dba.GetRdyChkTimeFile(_T("RNG_RX")) * 60;
		}

		// X/Ka
		if (m_DlyXKaTCRChkValue)
		{
			m_TimeSpan += dba.GetRdyChkTimeFile(_T("RNG_XKA")) * 60;
		}

		// X/Ka(再生型)
		if (m_DlyXKaTCRRChkValue)
		{
			m_TimeSpan += dba.GetRdyChkTimeFile(_T("RNG_RXKA")) * 60;
		}
	}

	// テレメトリ伝送チェック
	if (m_TLMChkValue)
	{
		// X
		if (m_TLMXChkValue)
		{
			m_TimeSpan += dba.GetRdyChkTimeFile(_T("TLM_DX")) * 60;
		}

		// Ka
		if (m_TLMKaChkValue)
		{
			m_TimeSpan += dba.GetRdyChkTimeFile(_T("TLM_DKA")) * 60;
		}
	}

	// コマンド伝送チェック
	if (m_CmdChkValue)
	{
		// X
		if (m_CmdXChkValue)
		{
			m_TimeSpan += dba.GetRdyChkTimeFile(_T("CMD_DX")) * 60;
		}
	}

	SetRdyChkEndTime();

	return;
}


/*============================================================================*/
/*! CRdnsChk54

-# 校正診断終了予定日時設定

@param
@retval

*/
/*============================================================================*/
void CRdnsChk54::SetRdyChkEndTime()
{
	UpdateData();

	CTime sTime = CTime(m_PeriodExecStartDay.GetYear(), m_PeriodExecStartDay.GetMonth(), m_PeriodExecStartDay.GetDay(),
						m_PeriodExecStartTime.GetHour(), m_PeriodExecStartTime.GetMinute(), 0);
	CTimeSpan span(0, 0, 0, m_TimeSpan);
	CTime eTime = sTime + span;

	m_PeriodExecEndDay = eTime;
	m_PeriodExecEndTime = eTime;

	UpdateData(FALSE);
}


/*============================================================================*/
/*! CRdnsChk54

-# 校正診断開始日時 年月日 変更通知

@param
@retval

*/
/*============================================================================*/
void CRdnsChk54::OnDtnDatetimechangeRdns54PeriodStimeDayDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	SetRdyChkEndTime();

	if (m_bNewPlan)
	{
		// 新規計画の場合、校正診断計画パス番号を合わせる
		CString strCurPassID = m_PeriodExecStartDay.Format(_T("%y%m%d-9999"));
		CString strTlmPass = _T("");

		m_TlmPathNoEdit.GetWindowTextW(strTlmPass);
		strTlmPass = strTlmPass.Mid(0, 6) + _T("-") + strTlmPass.Mid(6);	// XXXXXXXXXX をXXXXXX-XXXX 形式変換
		if (strTlmPass == m_strTlmPassID)
		{
			// テレメトリ・パス番号を変更していない場合、校正診断計画パス番号に合わせる
			m_strTlmPassID = strCurPassID;
			m_TlmPathNoEdit.SetWindowTextW(m_strTlmPassID);
			m_TlmPathNoEdit.UpdateData(FALSE);
		}
		CString strCmdPass = _T("");
		m_CmdPathNoEdit.GetWindowTextW(strCmdPass);
		strCmdPass = strCmdPass.Mid(0, 6) + _T("-") + strCmdPass.Mid(6);	// XXXXXXXXXX をXXXXXX-XXXX 形式変換
		if (strCmdPass == m_strCmdPassID)
		{
			// コマンド・パス番号を変更していない場合、校正診断計画パス番号に合わせる
			m_strCmdPassID = strCurPassID;
			m_CmdPathNoEdit.SetWindowTextW(m_strCmdPassID);
			m_CmdPathNoEdit.UpdateData(FALSE);
		}
	}

	*pResult = 0;
}


/*============================================================================*/
/*! CRdnsChk54

-# 校正診断開始日時 時刻 変更通知

@param
@retval

*/
/*============================================================================*/
void CRdnsChk54::OnDtnDatetimechangeRdns54PeriodStimeTimeDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	SetRdyChkEndTime();

	*pResult = 0;
}


/*============================================================================*/
/*! CRdnsChk54

-# 校正診断結果ファイル表示

@param	type :校正診断種別
@retval

*/
/*============================================================================*/
void CRdnsChk54::ResultFileOpen(int type)
{
	if (type == TYPE_AGC_X)
	{
		SetTimer(TID_RES_AGCX, INTERVAL_RES, 0);
	}
	else if (type == TYPE_AGC_KA)
	{
		SetTimer(TID_RES_AGCKA, INTERVAL_RES, 0);
	}
	else if (type == TYPE_RNG)
	{
		SetTimer(TID_RES_RNG, INTERVAL_RES, 0);
	}
	else if (type == TYPE_TLM_X)
	{
		SetTimer(TID_RES_TLM, INTERVAL_RES, 0);
	}
	else if (type == TYPE_CMD)
	{
		SetTimer(TID_RES_CMD, INTERVAL_RES, 0);
	}
	else
	{
		return;
	}
}


/*============================================================================*/
/*! CRdnsChk54

-# デバッグ用

@param
@retval

*/
/*============================================================================*/
void CRdnsChk54::MonitoringRdnsRunState()
{
	KillTimer(TID_RDNSSTATE);

#if 0
//	CString strSts = _T("");
//
//	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
//	CSatelliteData& sd = theApp.GetSatelliteData();
//
//	// AGC X
//	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), MON_AGC_X_COND);
//	if (obs_adr != NULL)
//	{
//		strSts = CString(obs_adr->sz_sts_name);
//		TRACE(_T("RdnsChk AGC X = %s\n"), strSts);
//	}
//
//	// RNG
//	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), MON_RNG_COND);
//	if (obs_adr != NULL)
//	{
//		strSts = CString(obs_adr->sz_sts_name);
//		TRACE(_T("RdnsChk RNG = %s\n"), strSts);
//	}
//
//	// TLM X
//	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), MON_TLM_X_COND);
//	if (obs_adr != NULL)
//	{
//		strSts = CString(obs_adr->sz_sts_name);
//		TRACE(_T("RdnsChk TLM X = %s\n"), strSts);
//	}
//
//	// CMD
//	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), MON_CMD_COND);
//	if (obs_adr != NULL)
//	{
//		strSts = CString(obs_adr->sz_sts_name);
//		TRACE(_T("RdnsChk CMD = %s\n"), strSts);
//	}
#endif

	UINT nStation = theApp.GetSelectStation();

	for (auto itr = m_CtrlList.begin(); itr != m_CtrlList.end(); ++itr)
	{
		if (theApp.GetControlCommand(nStation, mRdnsRunLogItem, (*itr).MonString) == STATE_WAIT)
		{
			theApp.UpdateControlCommand(nStation, (*itr).MonString, _T("RUN"));
			break;
		}
		if (theApp.GetControlCommand(nStation, mRdnsRunLogItem, (*itr).MonString) == STATE_RUN)
		{
			theApp.UpdateControlCommand(nStation, (*itr).MonString, _T("STOP"));
			break;
		}
	}

	SetTimer(TID_RDNSSTATE, INTERVAL_RDNSSTATE, 0);
}


void CRdnsChk54::OnTimer(UINT_PTR nIDEvent)
{
	HANDLE hFile;
	FILETIME cfTime, afTime, wfTime;

	CString dir = theApp.GetShareFilePath(eFileType_Calib_Result_Folder, theApp.GetSelectStation());
	CString strFilePath = _T("");

	CTime time = 0;

	if (nIDEvent == TID_RDNSSTATE)
	{
		MonitoringRdnsRunState();
	}
	else if (nIDEvent == TID_RES_AGCX)
	{
		strFilePath = dir + RES_FILE_AGCX;
		hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			GetFileTime(hFile, &cfTime, &afTime, &wfTime);
			CloseHandle(hFile);
			time = CTime(wfTime);

			if (time != m_AgcXResFileTime)
			{
				// 詳細ビュー表示
				if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
				{
					m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_AGC_X);
				}
				KillTimer(TID_RES_AGCX);
			}
		}
//#ifdef _DEBUG
//		// 詳細ビュー表示
//		if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
//		{
//			m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_AGC_X);
//		}
//		KillTimer(TID_RES_AGCX);
//#endif
	}
	else if (nIDEvent == TID_RES_AGCKA)
	{
		strFilePath = dir + RES_FILE_AGCKA;
		hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			GetFileTime(hFile, &cfTime, &afTime, &wfTime);
			CloseHandle(hFile);
			time = CTime(wfTime);

			if (time != m_AgcKaResFileTime)
			{
				// 詳細ビュー表示
				if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
				{
					m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_AGC_KA);
				}
				KillTimer(TID_RES_AGCKA);
			}
		}
//#ifdef _DEBUG
//		// 詳細ビュー表示
//		if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
//		{
//			m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_AGC_KA);
//		}
//		KillTimer(TID_RES_AGCKA);
//#endif
	}
	else if (nIDEvent == TID_RES_RNG)
	{
		strFilePath = dir + RES_FILE_RNG;
		hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			GetFileTime(hFile, &cfTime, &afTime, &wfTime);
			CloseHandle(hFile);
			time = CTime(wfTime);

			if (time != m_DelayResFileTime)
			{
				// 詳細ビュー表示
				if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
				{
					m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_RNG);
				}
				KillTimer(TID_RES_RNG);
			}
		}
//#ifdef _DEBUG
//		// 詳細ビュー表示
//		if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
//		{
//			m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_RNG);
//		}
//		KillTimer(TID_RES_RNG);
//#endif
	}
	else if (nIDEvent == TID_RES_TLM)
	{
		strFilePath = dir + RES_FILE_TLM;
		hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			GetFileTime(hFile, &cfTime, &afTime, &wfTime);
			CloseHandle(hFile);
			time = CTime(wfTime);

			if (time != m_TlmResFileTime)
			{
				// 詳細ビュー表示
				if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
				{
					m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_TLM_S);
				}
				KillTimer(TID_RES_TLM);
			}
		}
//#ifdef _DEBUG
//		// 詳細ビュー表示
//		if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
//		{
//			m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_TLM_S);
//		}
//		KillTimer(TID_RES_TLM);
//#endif
	}
	else if (nIDEvent == TID_RES_CMD)
	{
		strFilePath = dir + RES_FILE_CMD;
		hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			GetFileTime(hFile, &cfTime, &afTime, &wfTime);
			CloseHandle(hFile);
			time = CTime(wfTime);

			if (time != m_CmdResFileTime)
			{
				// 詳細ビュー表示
				if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
				{
					m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_CMD);
				}
				KillTimer(TID_RES_CMD);
			}
		}
//#ifdef _DEBUG
//		// 詳細ビュー表示
//		if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
//		{
//			m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_CMD);
//		}
//		KillTimer(TID_RES_CMD);
//#endif
	}

	CDialogBase::OnTimer(nIDEvent);
}

/*============================================================================*/
/*! CRdnsChk54

-# KillTimer ALL

@param
@retval

*/
/*============================================================================*/
void CRdnsChk54::InitTimer()
{
	KillTimer(TID_RES_AGCKA);
	KillTimer(TID_RES_AGCX);
	KillTimer(TID_RES_RNG);
	KillTimer(TID_RES_TLM);
	KillTimer(TID_RES_CMD);
}

/*============================================================================*/
/*! CRdnsChk54

-# スレッドを抜ける

@param
@retval

*/
/*============================================================================*/
void CRdnsChk54::ExitThread()
{
	m_RdnsChkAbort = true;

	if (m_hThread != NULL)
	{
		if (m_ThreadRun == true)
		{
			m_ThreadAbort = true;
		}

		CloseLogDlg();
		InitTimer();

		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}

/*============================================================================*/
/*! CRdnsChk54

-# EXCTR ROUTE コンボボックス作成

@param
@retval

*/
/*============================================================================*/
void CRdnsChk54::CreateExctrRouteCombo()
{
	struct ctlname_t *ctrl_adr;
	struct paramtbl_t *param_adr[1];
	struct sts_t *sts_adr[33];
	int ret = -1;
	int l_rc;

	CDBAccess& db = theApp.GetSatelliteData().GetDBAccessCls();

	const CString ctrlName = _T("X-SSPA.EXCTR_ROUTE");

	map<CString, __int64> &ctlAdrList = db.GetCtlAdrList();
	ctrl_adr = (struct ctlname_t *)((__int64*)ctlAdrList[ctrlName]);

	if (ctrl_adr == NULL){
//#ifdef _DEBUG
//		if (m_ExctrRouteCombo.GetCount() == 0)
//		{
//			m_ExctrRouteCombo.InsertString(-1, _T("EXCTR1"));
//			m_ExctrRouteCombo.InsertString(-1, _T("EXCTR2"));
//			m_ExctrRouteCombo.SetCurSel(0);
//			return;
//		}
//#endif /* _DEBUG */

		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CRdnsChk54"), _T("CreateExctrRouteCombo"), _T("ControlName None"), ctrlName, nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return;
	}

	/*----- 設備制御項目ステータスチェック ----*/
	l_rc = db.ud_adr_to_resvec((char*)ctrl_adr, PARAMTBL_ID, ENDOFCTL_ID, ENDOFPARAM_ID, (char**)param_adr, 1);

	if (l_rc < 1)
	{
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CRdnsChk54"), _T("CreateExctrRouteCombo"), _T("Control Error"), ctrlName, nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return;
	}

	if (param_adr[0]->l_intype == IN_S_ID && param_adr[0]->l_cvttype == CVT_S_ID)
	{
		l_rc = db.ud_adr_to_resvec((char*)ctrl_adr, STS_ID, ENDOFCTL_ID, ENDOFPARAM_ID, (char**)sts_adr, 33);

		// 制御種別とパラメタが異なっている場合はなにもしない
		if (l_rc <= 0){
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CLogTraceEx::Write(_T(""), _T("CRdnsChk54"), _T("CreateExctrRouteCombo"), _T("ControlType Mismatch"), ctrlName, nLogEx::error);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
			return;
		}

		// コンボボックスのクリア
		m_ExctrRouteCombo.Clear();

		// 制御項目追加
		for (int i = 0; i < l_rc; i++)
		{
			m_ExctrRouteCombo.InsertString(-1, CString(sts_adr[i]->sz_sts));
		}
		m_ExctrRouteCombo.SetCurSel(0);
	}
}
