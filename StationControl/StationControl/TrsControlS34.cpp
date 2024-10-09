/*============================================================================*/
/*! TrsControl.cpp

-# S帯送信制御画面
*/
/*============================================================================*/
// TrsControlS.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrsControlS34.h"
#include "afxdialogex.h"
#include "TrsControlMng.h"
#include "HookMsgBox.h"

// CTrsControlS34 ダイアログ

IMPLEMENT_DYNAMIC(CTrsControlS34, CDialogBase)

/*============================================================================*/
/*! CTrsControlS34

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CTrsControlS34::CTrsControlS34(CWnd* pParent /*=NULL*/)
: CDialogBase(CTrsControlS34::IDD, pParent)
	, m_strIndexCmd(_T(""))
	, m_strIndexRng(_T(""))
	, m_strIndexCR(_T(""))
	, m_strPower(_T(""))
	, m_strRoute(_T(""))
	, m_strRngRoute(_T(""))
	, m_strTrkData(_T(""))
	, m_strTlmData(_T(""))
{
	m_i_priv_flag = -1;
	m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;
	m_i_sweep_flag = 0;
}

/*============================================================================*/
/*! CTrsControlS34

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CTrsControlS34::~CTrsControlS34()
{
}

void CTrsControlS34::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TRSCTRL_IDX_CMD_EDIT, m_strIndexCmd);
	DDX_Text(pDX, IDC_TRSCTRL_IDX_RNG_EDIT, m_strIndexRng);
	DDX_Text(pDX, IDC_TRSCTRL_IDX_CR_EDIT, m_strIndexCR);
	DDX_Text(pDX, IDC_TRSCTRL_TXPR_P_EDIT, m_strPower);
	DDX_Text(pDX, IDC_TRSCTRL_TXPR_R_EDIT, m_strRoute);
	DDX_Text(pDX, IDC_TRSCTRL_RNGROUTE_EDIT, m_strRngRoute);
	DDX_Text(pDX, IDC_TRSCTRL_TRKDATA_EDIT, m_strTrkData);
	DDX_Text(pDX, IDC_TRSCTRL_TLMDATA_EDIT, m_strTlmData);
	DDX_Control(pDX, IDC_TRSCTRL_RF_OFF_BTN, m_RfPwrOffBtn);
	DDX_Control(pDX, IDC_TRSCTRL_RF_ON_BTN, m_RfPwrOnBtn);
	DDX_Control(pDX, IDC_TRSCTRL_NOUSE_OFF_BTN, m_NoUseOffBtn);
	DDX_Control(pDX, IDC_TRSCTRL_NOUSE_ON_BTN, m_NoUseOnBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEET_STOP_BTN, m_SweepContStopBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEET_INC_BTN, m_SweepContIncBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEET_INCDEC_BTN, m_SweepContIncDecBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEET_DEC_BTN, m_SweepContDecBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEET_DECINC_BTN, m_SweepContDecIncBtn);
	DDX_Control(pDX, IDC_TRSCTRL_CMD_OFF_BTN, m_CmdModOffBtn);
	DDX_Control(pDX, IDC_TRSCTRL_CMD_ON_BTN, m_CmdModOnBtn);
	DDX_Control(pDX, IDC_TRSCTRL_HOLD_OFF_BTN, m_HoldToneOffBtn);
	DDX_Control(pDX, IDC_TRSCTRL_HOLD_ON_BTN, m_HoldToneOnBtn);
	DDX_Control(pDX, IDC_TRSCTRL_COHINCOH_COH_BTN2, m_CohIncohCohBtn);
	DDX_Control(pDX, IDC_TRSCTRL_COHINCOH_INCOH_BTN2, m_CohIncohInCohBtn);
	DDX_Control(pDX, IDC_TRSCTRL_RNGING_STOP_BTN2, m_RngingStopBtn);
	DDX_Control(pDX, IDC_TRSCTRL_RNGING_START_BTN2, m_RngingStartBtn);
	DDX_Control(pDX, IDC_TRSCTRL_IDX_CMD_BTN, m_IndexCmdBtn);
	DDX_Control(pDX, IDC_TRSCTRL_IDX_RNG_BTN, m_IndexRngBtn);
	DDX_Control(pDX, IDC_TRSCTRL_IDX_CR_BTN, m_IndexCRBtn);
	DDX_Control(pDX, IDC_TRSCTRL_TXPR_PR_BTN, m_TxPwrRouteBtn);
	DDX_Control(pDX, IDC_TRSCTRL_TX_STATIC, m_grTxControl);
	DDX_Control(pDX, IDC_TRSCTRL_RF_STATIC, m_grRfPower);
	DDX_Control(pDX, IDC_TRSCTRL_NOUSE_STATIC, m_grNoUse);
	DDX_Control(pDX, IDC_TRSCTRL_SWEET_STATIC, m_grSweepCont);
	DDX_Control(pDX, IDC_TRSCTRL_CMD_STATIC, m_grCmdMod);
	DDX_Control(pDX, IDC_TRSCTRL_HOLD_STATIC, m_grHoldTone);
	DDX_Control(pDX, IDC_TRSCTRL_IDX_STATIC, m_grIndex);
	DDX_Control(pDX, IDC_TRSCTRL_TXPR_STATIC, m_grTxPwrRoute);
	DDX_Control(pDX, IDC_TRSCTRL_RNGCTRL_STATIC, m_grRngControl);
	DDX_Control(pDX, IDC_TRSCTRL_COHINCOH_STATIC, m_grCohIncoh);
	DDX_Control(pDX, IDC_TRSCTRL_RNGING_STATIC, m_grRnging);
}


BEGIN_MESSAGE_MAP(CTrsControlS34, CDialogBase)
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CTrsControlS34::OnMenuClose)
	ON_BN_CLICKED(IDC_TRSCTRL_IDX_CMD_BTN, &CTrsControlS34::OnBnClickedTrsctrlIdxCmdBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_IDX_RNG_BTN, &CTrsControlS34::OnBnClickedTrsctrlIdxRngBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_IDX_CR_BTN, &CTrsControlS34::OnBnClickedTrsctrlIdxCrBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_TXPR_PR_BTN, &CTrsControlS34::OnBnClickedTrsctrlTxprPrBtn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TRSCTRL_RF_OFF_BTN, &CTrsControlS34::OnBnClickedTrsctrlRfOffBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_RF_ON_BTN, &CTrsControlS34::OnBnClickedTrsctrlRfOnBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_NOUSE_OFF_BTN, &CTrsControlS34::OnBnClickedTrsctrlNouseOffBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_NOUSE_ON_BTN, &CTrsControlS34::OnBnClickedTrsctrlNouseOnBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEET_STOP_BTN, &CTrsControlS34::OnBnClickedTrsctrlSweetStopBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEET_INC_BTN, &CTrsControlS34::OnBnClickedTrsctrlSweetIncBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEET_INCDEC_BTN, &CTrsControlS34::OnBnClickedTrsctrlSweetIncdecBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEET_DEC_BTN, &CTrsControlS34::OnBnClickedTrsctrlSweetDecBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEET_DECINC_BTN, &CTrsControlS34::OnBnClickedTrsctrlSweetDecincBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_CMD_OFF_BTN, &CTrsControlS34::OnBnClickedTrsctrlCmdOffBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_CMD_ON_BTN, &CTrsControlS34::OnBnClickedTrsctrlCmdOnBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_HOLD_OFF_BTN, &CTrsControlS34::OnBnClickedTrsctrlHoldOffBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_HOLD_ON_BTN, &CTrsControlS34::OnBnClickedTrsctrlHoldOnBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_COHINCOH_COH_BTN2, &CTrsControlS34::OnBnClickedTrsctrlCohincohCohBtn2)
	ON_BN_CLICKED(IDC_TRSCTRL_COHINCOH_INCOH_BTN2, &CTrsControlS34::OnBnClickedTrsctrlCohincohIncohBtn2)
	ON_BN_CLICKED(IDC_TRSCTRL_RNGING_STOP_BTN2, &CTrsControlS34::OnBnClickedTrsctrlRngingStopBtn2)
	ON_BN_CLICKED(IDC_TRSCTRL_RNGING_START_BTN2, &CTrsControlS34::OnBnClickedTrsctrlRngingStartBtn2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTrsControlS34 メッセージ ハンドラー


/*============================================================================*/
/*! CTrsControlS34

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CTrsControlS34::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// RF-POWER
	m_RfPwrOffBtn.SetColor(COLOR_REMOTES);
	m_RfPwrOffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_RfPwrOnBtn.SetColor(COLOR_REMOTES);
	m_RfPwrOnBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// NO-USE
	m_NoUseOffBtn.SetColor(COLOR_REMOTES);
	m_NoUseOffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_NoUseOnBtn.SetColor(COLOR_REMOTE);
	m_NoUseOnBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// SWEEP-CONT
	m_SweepContStopBtn.SetColor(COLOR_REMOTES);
	m_SweepContStopBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepContIncBtn.SetColor(COLOR_REMOTE);
	m_SweepContIncBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepContIncDecBtn.SetColor(COLOR_REMOTE);
	m_SweepContIncDecBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepContDecBtn.SetColor(COLOR_REMOTE);
	m_SweepContDecBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepContDecIncBtn.SetColor(COLOR_REMOTE);
	m_SweepContDecIncBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// CMD-MOD
	m_CmdModOffBtn.SetColor(COLOR_REMOTES);
	m_CmdModOffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_CmdModOnBtn.SetColor(COLOR_REMOTE);
	m_CmdModOnBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// HOLD-TONE
	m_HoldToneOffBtn.SetColor(COLOR_REMOTES);
	m_HoldToneOffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_HoldToneOnBtn.SetColor(COLOR_REMOTE);
	m_HoldToneOnBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// COH-INCOH
	m_CohIncohCohBtn.SetColor(COLOR_REMOTES);
	m_CohIncohCohBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_CohIncohInCohBtn.SetColor(COLOR_REMOTES);
	m_CohIncohInCohBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// RNGING
	m_RngingStopBtn.SetColor(COLOR_REMOTES);
	m_RngingStopBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_RngingStartBtn.SetColor(COLOR_REMOTE);
	m_RngingStartBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// INDEX-CMD
	m_IndexCmdBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_IndexCmdBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// INDEX-RNG
	m_IndexRngBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_IndexRngBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// INDEX-C+R
	m_IndexCRBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_IndexCRBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// PWR/ROUTE
	m_TxPwrRouteBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_TxPwrRouteBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// グループボックス
	m_grTxControl.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grTxControl.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRfPower.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRfPower.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grNoUse.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grNoUse.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grSweepCont.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grSweepCont.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grCmdMod.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grCmdMod.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grHoldTone.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grHoldTone.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grIndex.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grIndex.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grTxPwrRoute.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grTxPwrRoute.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRngControl.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRngControl.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grCohIncoh.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grCohIncoh.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRnging.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRnging.SetGroupType(CGroupBoxEx::eGroupType_Round);


	// タイムアウトカウントダウン値初期設定
	m_l_rf_power_off_cntdwn = -1;
	m_l_rf_power_on_cntdwn = -1;
	m_l_no_use_off_cntdwn = -1;
	m_l_no_use_on_cntdwn = -1;
	m_l_sweep_cont_stop_cntdwn = -1;
	m_l_sweep_cont_inc_cntdwn = -1;
	m_l_sweep_cont_incdec_cntdwn = -1;
	m_l_sweep_cont_dec_cntdwn = -1;
	m_l_sweep_cont_decinc_cntdwn = -1;
	m_l_cmd_mod_off_cntdwn = -1;
	m_l_cmd_mod_on_cntdwn = -1;
	m_l_rng_mod_off_cntdwn = -1;
	m_l_rng_mod_on_cntdwn = -1;
	m_l_ss_rng_off_cntdwn = -1;
	m_l_ss_rng_on_cntdwn = -1;
	m_l_sx_rng_off_cntdwn = -1;
	m_l_sx_rng_on_cntdwn = -1;
	m_l_coh_incoh_coh_cntdwn = -1;
	m_l_coh_incoh_incoh_cntdwn = -1;
	m_l_rnging_stop_cntdwn = -1;
	m_l_rnging_start_cntdwn = -1;
	m_l_holdtone_off_cntdwn = -1;
	m_l_holdtone_on_cntdwn = -1;
	m_l_cmd_mod_idx_cntdwn = -1;
	m_l_rng_mod_idx_cntdwn = -1;
	m_l_car_mod_idx_cntdwn = -1;
	m_l_rng_route_cntdwn = -1;
	m_l_trk_data_cntdwn = -1;
	m_l_tlm_data_cntdwn = -1;


	// ホスト名取得
	memset(m_sz_hostname, (char)NULL, sizeof(m_sz_hostname));
	gethostname(m_sz_hostname, sizeof(m_sz_hostname));

#if 0
	// 送信制御画面状態表示タイムアウトコールバックルーチン起動
	show_opsc_condition();

	// カウントダウンタイムアウトコールバックルーチン起動
	count_down();
#endif

	CenterWindowEx();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CTrsControlS34

-# ダイアログ更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CTrsControlS34::UpdateDlg()
{
	// 送信制御画面状態表示タイムアウトコールバックルーチン起動
	show_opsc_condition();

	// カウントダウンタイムアウトコールバックルーチン起動
	count_down();

	return TRUE;
}


/*============================================================================*/
/*! CTrsControlS34

-# ファイル-閉じるメニュー処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnMenuClose()
{
	// いずれかのボタンが実行中の場合
	if (m_RfPwrOffBtn.IsColorIgnore2() ||
		m_RfPwrOnBtn.IsColorIgnore2() ||
		m_NoUseOffBtn.IsColorIgnore2() ||
		m_NoUseOnBtn.IsColorIgnore2() ||
		m_SweepContStopBtn.IsColorIgnore2() ||
		m_SweepContIncBtn.IsColorIgnore2() ||
		m_SweepContIncDecBtn.IsColorIgnore2() ||
		m_SweepContDecBtn.IsColorIgnore2() ||
		m_SweepContDecIncBtn.IsColorIgnore2() ||
		m_CmdModOffBtn.IsColorIgnore2() ||
		m_CmdModOnBtn.IsColorIgnore2() ||
		m_HoldToneOffBtn.IsColorIgnore2() ||
		m_HoldToneOnBtn.IsColorIgnore2() ||
		m_CohIncohCohBtn.IsColorIgnore2() ||
		m_CohIncohInCohBtn.IsColorIgnore2() ||
		m_RngingStopBtn.IsColorIgnore2() ||
		m_RngingStartBtn.IsColorIgnore2() ||
		m_IndexCmdBtn.IsColorIgnore2() ||
		m_IndexRngBtn.IsColorIgnore2() ||
		m_IndexCRBtn.IsColorIgnore2() ||
		m_TxPwrRouteBtn.IsColorIgnore2())
	{
		CString title;
		GetWindowText(title);

		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("終了します。\nよろしいですか？"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			return;
		}
	}

	CloseDlg();
	//	OnOK();
	ShowWindow(SW_HIDE);
}

/*============================================================================*/
/*! CTrsControlS34

-# 『TX CONTROL』-『RF-POWER』-『OFF』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlRfOffBtn()
{
	// ボタンを制御中色にする
	m_RfPwrOffBtn.SetColor(COLOR_CTRLIN);
	m_RfPwrOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("S-TX.RF_CONT"), _T("OFF"));

	// カウントダウン開始
	m_l_rf_power_off_cntdwn = RF_POWER_TIME_S34_LIMIT;

	// 更新
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# 『TX CONTROL』-『RF-POWER』-『ON』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlRfOnBtn()
{
	// ボタンを制御中色にする
	m_RfPwrOnBtn.SetColor(COLOR_CTRLIN);
	m_RfPwrOffBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("S-TX.RF_CONT"), _T("ON"));

	// カウントダウン開始
	m_l_rf_power_on_cntdwn = RF_POWER_TIME_S34_LIMIT;

	// 更新
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# 『TX CONTROL』-『NO-USER』-『OFF』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlNouseOffBtn()
{
	COLORREF col = m_NoUseOffBtn.GetColor();

	if (col == COLOR_REMOTE)	// リモート（非選択）
	{
		// ボタンを制御中色にする
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// 制御通知送信
		theApp.GetSatelliteData().opctproc(_T("S-TX.RF_TRIP_NO_USE"), _T("NO_USE_RESET"));

		// カウントダウン開始
		m_l_no_use_off_cntdwn = NO_USE_TIME_S34_LIMIT;
	}
	else if (col == COLOR_CTRLIN)	// 制御中
	{
		// ボタンを制御中色にする
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// 制御通知送信
		theApp.GetSatelliteData().opctproc(_T("S-TX.RF_TRIP_NO_USE"), _T("NO_USE_RESET"));

		// カウントダウン開始
		m_l_no_use_off_cntdwn = NO_USE_TIME_S34_LIMIT;

		// 他方のカウントダウン終了
		m_l_no_use_on_cntdwn = -1;
	}
	else if (col == COLOR_REMOTES)	// リモート（選択）
	{
		// ボタンを制御中色にする
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// 制御通知送信
		theApp.GetSatelliteData().opctproc(_T("S-TX.RF_TRIP_NO_USE"), _T("NO_USE_RESET"));

		// カウントダウン開始
		m_l_no_use_off_cntdwn = NO_USE_TIME_S34_LIMIT;

		// 他方のカウントダウン終了
		m_l_no_use_on_cntdwn = -1;
	}
	else
	{
		;	// noting.
	}

	// 更新
	m_NoUseOffBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# 『TX CONTROL』-『NO-USER』-『ON』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlNouseOnBtn()
{
	COLORREF col = m_NoUseOnBtn.GetColor();

	if (col == COLOR_REMOTE)	// リモート（非選択）
	{
		// ボタンを制御中色にする
		m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

		// 制御通知送信
		theApp.GetSatelliteData().opctproc(_T("S-TX.RF_TRIP_NO_USE"), _T("NO_USE"));

		// カウントダウン開始
		m_l_no_use_on_cntdwn = NO_USE_TIME_S34_LIMIT;
	}
	else if (col == COLOR_CTRLIN)	// 制御中
	{
		// ボタンを制御中色にする
		m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

		// 制御通知送信
		theApp.GetSatelliteData().opctproc(_T("S-TX.RF_TRIP_NO_USE"), _T("NO_USE"));

		// カウントダウン開始
		m_l_no_use_on_cntdwn = NO_USE_TIME_S34_LIMIT;

		// 他方のカウントダウン終了
		m_l_no_use_off_cntdwn = -1;
	}
	else if (col == COLOR_REMOTES)	// リモート（選択）
	{
		// ボタンを制御中色にする
		m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

		// 制御通知送信
		theApp.GetSatelliteData().opctproc(_T("S-TX.RF_TRIP_NO_USE"), _T("NO_USE"));

		// カウントダウン開始
		m_l_no_use_on_cntdwn = NO_USE_TIME_S34_LIMIT;

		// 他方のカウントダウン終了
		m_l_no_use_off_cntdwn = -1;
	}
	else
	{
		;	// noting.
	}

	// 更新
	m_NoUseOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# 『TX CONTROL』-『SWEEP-CONT』-『STOP』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlSweetStopBtn()
{
	// ボタンを制御中色にする
	m_SweepContStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CONT"), _T("OFF"));
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CMD"), _T("STOP"));

	// カウントダウン開始
	m_l_sweep_cont_stop_cntdwn = SWEEP_CONT_TIME_S34_LIMIT;

	// 更新
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# 『TX CONTROL』-『SWEEP-CONT』-『INC』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlSweetIncBtn()
{
	// ボタンを制御中色にする
	m_SweepContStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CONT"), _T("ON"));

	// カウントダウン開始
	m_l_sweep_cont_inc_cntdwn = SWEEP_CONT_TIME_S34_LIMIT;
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// 更新
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# 『TX CONTROL』-『SWEEP-CONT』-『INCDEC』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlSweetIncdecBtn()
{
	// ボタンを制御中色にする
	m_SweepContStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CONT"), _T("ON"));

	// カウントダウン開始
	m_l_sweep_cont_incdec_cntdwn = SWEEP_CONT_TIME_S34_LIMIT;
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// 更新
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# 『TX CONTROL』-『SWEEP-CONT』-『DEC』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlSweetDecBtn()
{
	// ボタンを制御中色にする
	m_SweepContStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CONT"), _T("ON"));

	// カウントダウン開始
	m_l_sweep_cont_dec_cntdwn = SWEEP_CONT_TIME_S34_LIMIT;
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// 更新
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# 『TX CONTROL』-『SWEEP-CONT』-『DECINC』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlSweetDecincBtn()
{
	// ボタンを制御中色にする
	m_SweepContStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CONT"), _T("ON"));

	// カウントダウン開始
	m_l_sweep_cont_decinc_cntdwn = SWEEP_CONT_TIME_S34_LIMIT;
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// 更新
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# 『TX CONTROL』-『CMD-MOD』-『OFF』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlCmdOffBtn()
{
	// ボタンを制御中色にする
	m_CmdModOffBtn.SetColor(COLOR_CTRLIN);
	m_CmdModOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.MODULATION"), _T("OFF"));

	// カウントダウン開始
	m_l_cmd_mod_off_cntdwn = CMD_MOD_TIME_S34_LIMIT;

	// 更新
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# 『TX CONTROL』-『CMD-MOD』-『ON』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlCmdOnBtn()
{
	// ボタンを制御中色にする
	m_CmdModOffBtn.SetColor(COLOR_CTRLIN);
	m_CmdModOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.MODULATION"), _T("ON"));

	// カウントダウン開始
	m_l_cmd_mod_on_cntdwn = CMD_MOD_TIME_S34_LIMIT;

	// 更新
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# 『TX CONTROL』-『HOLD-TONE』-『OFF』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlHoldOffBtn()
{
	// ボタンを制御中色にする
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CMDOUT.HOLD_TONE"), _T("OFF"));

	// カウントダウン開始
	m_l_holdtone_off_cntdwn = HOLDTONE_TIME_S34_LIMIT;

	// 更新
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# 『TX CONTROL』-『HOLD-TONE』-『ON』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlHoldOnBtn()
{
	// ボタンを制御中色にする
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CMDOUT.HOLD_TONE"), _T("ON"));

	// カウントダウン開始
	m_l_holdtone_on_cntdwn = HOLDTONE_TIME_S34_LIMIT;

	// 更新
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# 『TX CONTROL』-『INDEX』-『CMD』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlIdxCmdBtn()
{
	CString strCmd;
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	memset(m_sz_sat_name, (char)NULL, sizeof(m_sz_sat_name));
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_SAT_NAME));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sat_name, sizeof(m_sz_sat_name), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRMOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_ss_tcrmod, sizeof(m_sz_lan_ss_tcrmod), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_MOD_UPLINK_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_mod_uplink_cont, sizeof(m_sz_mod_uplink_cont), obs_adr->sz_sts_name);
	}

	// リモート時
	if ((strcmp(m_sz_mod_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_ss_tcrmod, "接続") == 0) &&
		(strlen(m_sz_sat_name) > 0) &&
		(m_i_priv_flag == 1))
	{
		// 設備詳細制御プロセス(opctproc)起動
		strCmd.Format(_T("CMD,%s"), (LPCTSTR)CString(m_sz_sat_name));
		theApp.GetSatelliteData().opctproc2(_T("CTRL.MOD_INDEX"), strCmd);
	}
}

/*============================================================================*/
/*! CTrsControlS34

-# 『TX CONTROL』-『INDEX』-『RNG』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlIdxRngBtn()
{
	CString strCmd;
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();


	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	memset(m_sz_sat_name, (char)NULL, sizeof(m_sz_sat_name));
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_SAT_NAME));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sat_name, sizeof(m_sz_sat_name), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRMOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_ss_tcrmod, sizeof(m_sz_lan_ss_tcrmod), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_MOD_UPLINK_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_mod_uplink_cont, sizeof(m_sz_mod_uplink_cont), obs_adr->sz_sts_name);
	}

	// リモート時
	if ((strcmp(m_sz_mod_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_ss_tcrmod, "接続") == 0) &&
		(strlen(m_sz_sat_name) > 0) &&
		(m_i_priv_flag == 1))
	{
		// 設備詳細制御プロセス(opctproc)起動
		strCmd.Format(_T("RNG,%s"), (LPCTSTR)CString(m_sz_sat_name));
		theApp.GetSatelliteData().opctproc2(_T("CTRL.MOD_INDEX"), strCmd);
	}
}

/*============================================================================*/
/*! CTrsControlS34

-# 『TX CONTROL』-『INDEX』-『C+R』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlIdxCrBtn()
{
	CString strCmd;
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();


	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	memset(m_sz_sat_name, (char)NULL, sizeof(m_sz_sat_name));
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_SAT_NAME));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sat_name, sizeof(m_sz_sat_name), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRMOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_ss_tcrmod, sizeof(m_sz_lan_ss_tcrmod), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_MOD_UPLINK_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_mod_uplink_cont, sizeof(m_sz_mod_uplink_cont), obs_adr->sz_sts_name);
	}

	// リモート時
	if ((strcmp(m_sz_mod_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_ss_tcrmod, "接続") == 0) &&
		(strlen(m_sz_sat_name) > 0) &&
		(m_i_priv_flag == 1))
	{
		// 設備詳細制御プロセス(opctproc)起動
		strCmd.Format(_T("CMD+RNG,%s"), (LPCTSTR)CString(m_sz_sat_name));
		theApp.GetSatelliteData().opctproc2(_T("CTRL.MOD_INDEX"), strCmd);
	}
}

/*============================================================================*/
/*! CTrsControlS34

-# 『TX CONTROL』-『TX PWR/ROUTE』-『POWER/ROUTE』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlTxprPrBtn()
{
	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CTRL.S_TX_RF_PWR"), _T(""));
}


/*============================================================================*/
/*! CTrsControlS34

-# 『RNG CONTROL』-『CHO/INCOH』-『COH』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlCohincohCohBtn2()
{
	// ボタンを制御中色にする
	m_CohIncohCohBtn.SetColor(COLOR_CTRLIN);
	m_CohIncohInCohBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("COHI"));

	// カウントダウン開始
	m_l_coh_incoh_coh_cntdwn = COH_INCOH_TIME_S34_LIMIT;

	// 更新
	m_CohIncohCohBtn.Invalidate();
	m_CohIncohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# 『RNG CONTROL』-『CHO/INCOH』-『INCOH』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlCohincohIncohBtn2()
{
	// ボタンを制御中色にする
	m_CohIncohCohBtn.SetColor(COLOR_CTRLIN);
	m_CohIncohInCohBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("INCOHI"));

	// カウントダウン開始
	m_l_coh_incoh_incoh_cntdwn = COH_INCOH_TIME_S34_LIMIT;

	// 更新
	m_CohIncohCohBtn.Invalidate();
	m_CohIncohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# 『RNG CONTROL』-『RNGING』-『STOP』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlRngingStopBtn2()
{
	// ボタンを制御中色にする
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING_CANCEL"), _T(","));

	// カウントダウン開始
	m_l_rnging_stop_cntdwn = RNGING_TIME_S34_LIMIT;

	// 更新
	m_RngingStopBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# 『RNG CONTROL』-『RNGING』-『START』ボタン押下処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlRngingStartBtn2()
{
	CString str;

	// ボタンを制御中色にする
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);
	m_RngingStartBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	str.Format(_T("S-%02d"), 99);
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str);

	// カウントダウン開始
	m_l_rnging_start_cntdwn = RNGING_TIME_S34_LIMIT;

	// 更新
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# OnTimer

@param  nIDEvent:タイマーの識別子
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_CONDITION)
	{
		// 送信制御画面状態表示設定
		show_opsc_condition();
	}
	else if (nIDEvent == TID_COUNTDOWN)
	{
		// カウントダウンタイムアウト
		count_down();
	}

	CDialogBase::OnTimer(nIDEvent);
}
/*============================================================================*/
/*! CTrsControlS34

-# 送信制御画面状態表示設定

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::show_opsc_condition()
{
	TRACE(_T("show_opsc_condition\n"));

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	char sz_buff[64] = { 0 };
	char sz_misc1[32] = { 0 };
	char sz_misc2[32] = { 0 };

	KillTimer(TID_CONDITION);

	CSatelliteData& sd = theApp.GetSatelliteData();

	CString msg = _T("");

	// 制御権は常に有り
	m_i_priv_flag = 1;

	/*-- TX CONTROL ----------------------------------------------------------------------*/

	// 送信制御画面のロジック見直しにより追加 

	// RF POWER ON ENABLE 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_RF_ON_ENBL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl, sizeof(m_sz_rf_on_enbl), obs_adr->sz_sts_name);
	}

	// RF-TRIP NOUSE ON ENABLE 状態 
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_NO_USE_ENBL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl, sizeof(m_sz_no_use_enbl), obs_adr->sz_sts_name);
	}

	// UP-LINK CONTROL 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_UPLINK_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), obs_adr->sz_sts_name);
	}

	// S-TX RF POWER ON DETECT ANSWER 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_RF_ON_DET_ANS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans, sizeof(m_sz_rf_on_det_ans), obs_adr->sz_sts_name);
	}

	// X-TX RF POWER ON DETECT ANSWER 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_X_TX_DET_ANS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_tx_det_ans, sizeof(m_sz_x_tx_det_ans), obs_adr->sz_sts_name);
	}

	/*** リモート／ローカル 状態取得 ******************************************************/

	// S-TX リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_S_TX_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_s_tx_rl, sizeof(m_sz_s_tx_rl), obs_adr->sz_sts_name);
	}

	// S-TXSYN リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_S_TXSYN_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_s_txsyn_rl, sizeof(m_sz_s_txsyn_rl), obs_adr->sz_sts_name);
	}

	/*** アラームチェック *****************************************************************/
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_ALARM));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_alarm, sizeof(m_sz_alarm), obs_adr->sz_sts_name);
	}

	// アラーム時
	if (strcmp(m_sz_alarm, "ON") == 0)
	{
		// 枠を赤くする
		m_grRfPower.SetBackColor(COLOR_ALARM);
		m_grNoUse.SetBackColor(COLOR_ALARM);
		m_grSweepCont.SetBackColor(COLOR_ALARM);
		m_grCmdMod.SetBackColor(COLOR_ALARM);
	}
	// アラーム解除
	else
	{
		// 枠の色を元に戻す
		m_grRfPower.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grNoUse.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepCont.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grCmdMod.SetBackColor(GetSysColor(COLOR_3DFACE));
	}

	/*** RF-POWER *************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_RF_POWER));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_power, sizeof(m_sz_rf_power), obs_adr->sz_sts_name);
	}

	// RF-POWER ON カウントダウン中に監視データが ON になった場合
	if (m_l_rf_power_on_cntdwn >= 0 && strcmp(m_sz_rf_power, "ON") == 0)
	{
		// カウントダウン終了
		m_l_rf_power_on_cntdwn = -1;
	}
	// RF-POWER ON カウントダウンタイムリミットになった場合
	else if (m_l_rf_power_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rf_power_on_cntdwn = -1;
	}

	// RF-POWER OFF カウントダウン中に監視データが OFF になった場合
	if (m_l_rf_power_off_cntdwn >= 0 && strcmp(m_sz_rf_power, "OFF") == 0)
	{
		// カウントダウン終了
		m_l_rf_power_off_cntdwn = -1;
	}
	// RF-POWER OFF カウントダウンタイムリミットになった場合
	else if (m_l_rf_power_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rf_power_off_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_rf_power_off_cntdwn == -1 && m_l_rf_power_on_cntdwn == -1)
	{
		// ON の場合
		if (strcmp(m_sz_rf_power, "ON") == 0)
		{
			show_rf_power_on();
		}

		// OFF の場合
		else
		{
			/*** 特殊処理注意 *************************************************************/
			show_rf_power_off();
		}
	}

	/*** NO-USE ***************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_NO_USE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use, sizeof(m_sz_no_use), obs_adr->sz_sts_name);
	}

	// NO-USE ON カウントダウン中に監視データが ON になった場合
	if (m_l_no_use_on_cntdwn >= 0 && strcmp(m_sz_no_use, "ON") == 0)
	{
		// カウントダウン終了
		m_l_no_use_on_cntdwn = -1;
	}
	// NO-USE ON カウントダウンタイムリミットになった場合
	else if (m_l_no_use_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_no_use_on_cntdwn = -1;
	}

	// NO-USE OFF カウントダウン中にデータが OFF になった場合
	if (m_l_no_use_off_cntdwn >= 0 && strcmp(m_sz_no_use, "ON") != 0)
	{
		// カウントダウン終了
		m_l_no_use_off_cntdwn = -1;
	}
	// NO-USE OFF カウントダウンタイムリミットになった場合
	else if (m_l_no_use_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_no_use_off_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_no_use_off_cntdwn == -1 && m_l_no_use_on_cntdwn == -1)
	{
		// ON の場合
		if (strcmp(m_sz_no_use, "ON") == 0)
		{
			show_no_use_on();
		}

		// OFF の場合
		else
		{
			show_no_use_off();
		}
	}

	/*** SWEEP-CONT ***********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_NOR_SWEEP_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cont, sizeof(m_sz_sweep_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_NOR_SWEEP_CMD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cmd, sizeof(m_sz_sweep_cmd), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_NOR_SWEEP_STS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_sts, sizeof(m_sz_sweep_sts), obs_adr->sz_sts_name);
	}

	// SWEEP-CONT STOP カウントダウン中に監視データが STOP になった場合
	if (m_l_sweep_cont_stop_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "OFF") == 0 ||
		strcmp(m_sz_sweep_cmd, "STOP") == 0 ||
		strcmp(m_sz_sweep_sts, "STOP") == 0)) 
	{
		// カウントダウン終了
		m_l_sweep_cont_stop_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP実行フラグOFF
		m_i_sweep_flag = 0;
	}
	// SWEEP-CONT STOP カウントダウンタイムリミットになった場合
	else if (m_l_sweep_cont_stop_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_stop_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;
	}

	// SWEEP-CONT INC カウントダウン中にデータが INC になった場合
	if (m_l_sweep_cont_inc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_cmd, "INC") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0)) 
	{

		// カウントダウン終了
		m_l_sweep_cont_inc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT INC カウントダウンタイムリミットになった場合
	else if (m_l_sweep_cont_inc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_inc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_sweep_cont_inc_cntdwn >= 0) && (strcmp(m_sz_sweep_cmd, "INC") != 0))
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CMD"), _T("INC"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT INC+DEC カウントダウン中にデータが INC+DEC になった場合
	if (m_l_sweep_cont_incdec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_cmd, "INCDEC") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0)) 
	{
		// カウントダウン終了
		m_l_sweep_cont_incdec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT INC+DEC カウントダウンタイムリミットになった場合
	else if (m_l_sweep_cont_incdec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_incdec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_sweep_cont_incdec_cntdwn >= 0) &&
		(strcmp(m_sz_sweep_cmd, "INCDEC") != 0)) 
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CMD"), _T("INCDEC"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT DEC カウントダウン中にデータが DEC になった場合
	if (m_l_sweep_cont_dec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_cmd, "DEC") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0)) 
	{
		// カウントダウン終了
		m_l_sweep_cont_dec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT DEC カウントダウンタイムリミットになった場合
	else if (m_l_sweep_cont_dec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_dec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_sweep_cont_dec_cntdwn >= 0) &&
		(strcmp(m_sz_sweep_cmd, "DEC") != 0)) 
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CMD"), _T("DEC"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT DEC+INC カウントダウン中にデータが DEC+INC になった場合
	if (m_l_sweep_cont_decinc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_cmd, "DECINC") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0)) 
	{
		// カウントダウン終了
		m_l_sweep_cont_decinc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT DEC+INC カウントダウンタイムリミットになった場合
	else if (m_l_sweep_cont_decinc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_decinc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_sweep_cont_decinc_cntdwn >= 0) &&
		(strcmp(m_sz_sweep_cmd, "DECINC") != 0)) 
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CMD"), _T("DECINC"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_sweep_cont_stop_cntdwn == -1 && m_l_sweep_cont_inc_cntdwn == -1 &&
		m_l_sweep_cont_incdec_cntdwn == -1 && m_l_sweep_cont_dec_cntdwn == -1 &&
		m_l_sweep_cont_decinc_cntdwn == -1)
	{
		// INC の場合
		if (strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_cmd, "INC") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) 
		{
			show_sweep_cont_inc();
		}

		// INC+DEC の場合
		else if (strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_cmd, "INCDEC") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) 
		{
			show_sweep_cont_incdec();
		}

		// DEC の場合
		else if (strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_cmd, "DEC") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) 
		{
			show_sweep_cont_dec();
		}

		// DEC+INC の場合
		else if (strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_cmd, "DECINC") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) 
		{
			show_sweep_cont_decinc();
		}

		// STOP の場合
		else 
		{
			// CONTがONでSTSがRUNNING以外ならば、CONTをOFFする
			if (strcmp(m_sz_sweep_cont, "ON") == 0 &&
				strcmp(m_sz_sweep_sts, "RUNNING") != 0 &&
				m_i_sweep_flag == 1)
			{
				theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CONT"), _T("OFF"));
				theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CMD"), _T("STOP"));

				// SWEEP実行フラグOFF
				m_i_sweep_flag = 0;
			}

			// 完全に停止してから、STOP表示にする
			if (strcmp(m_sz_sweep_cont, "OFF") == 0 &&
				strcmp(m_sz_sweep_cmd, "STOP") == 0 &&
				strcmp(m_sz_sweep_sts, "INITIALIZE") == 0) 
			{
				show_sweep_cont_stop();
			}
		}
	}


	/*** CMD-MOD **************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_CMD_MOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_cmd_mod, sizeof(m_sz_cmd_mod), obs_adr->sz_sts_name);
	}

	// CMD-MOD OFF カウントダウン中にデータが OFF になった場合
	if (m_l_cmd_mod_off_cntdwn >= 0 && strcmp(m_sz_cmd_mod, "OFF") == 0)
	{
		// カウントダウン終了
		m_l_cmd_mod_off_cntdwn = -1;
	}
	// CMD-MOD OFF カウントダウンタイムリミットになった場合
	else if (m_l_cmd_mod_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_cmd_mod_off_cntdwn = -1;
	}

	// CMD-MOD ON カウントダウン中にデータが ON になった場合
	if (m_l_cmd_mod_on_cntdwn >= 0 && strcmp(m_sz_cmd_mod, "ON") == 0)
	{
		// カウントダウン終了
		m_l_cmd_mod_on_cntdwn = -1;
	}
	// CMD-MOD ON カウントダウンタイムリミットになった場合
	else if (m_l_cmd_mod_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_cmd_mod_on_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_cmd_mod_off_cntdwn == -1 && m_l_cmd_mod_on_cntdwn == -1)
	{
		// ON の場合
		if (strcmp(m_sz_cmd_mod, "ON") == 0)
		{
			show_cmd_mod_on();
		}

		// OFF の場合
		else
		{
			show_cmd_mod_off();
		}
	}


	/*** HOLD TONE ************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_HOLD_TONE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hold_tone, sizeof(m_sz_hold_tone), obs_adr->sz_sts_name);
	}

	// HOLD TONE ON カウントダウン中に監視データが ON になった場合
	if (m_l_holdtone_on_cntdwn >= 0 && strcmp(m_sz_hold_tone, "ON") == 0)
	{
		// カウントダウン終了
		m_l_holdtone_on_cntdwn = -1;
	}
	// HOLD TONE ON カウントダウンタイムリミットになった場合
	else if (m_l_holdtone_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_holdtone_on_cntdwn = -1;
	}

	// HOLD TONE OFF カウントダウン中に監視データが OFF になった場合
	if (m_l_holdtone_off_cntdwn >= 0 && strcmp(m_sz_hold_tone, "OFF") == 0)
	{
		// カウントダウン終了
		m_l_holdtone_off_cntdwn = -1;
	}
	// HOLD TONE OFF カウントダウンタイムリミットになった場合
	else if (m_l_holdtone_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_holdtone_off_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_holdtone_off_cntdwn == -1 && m_l_holdtone_on_cntdwn == -1)
	{
		// ON の場合
		if (strcmp(m_sz_hold_tone, "ON") == 0)
		{
			show_holdtone_on();
		}

		// OFF の場合
		else
		{
			show_holdtone_off();
		}
	}


	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	/*** SS-RNG / SX-RNG ******************************************************************/
	/*** 数値表示部最新データ表示 *********************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_SS_RNG_CNT));
	if (obs_adr != NULL)
	{
		m_d_ss_rng_cnt = obs_adr->d_data;
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_SS_RNG_NUM));
	if (obs_adr != NULL)
	{
		m_d_ss_rng_num = obs_adr->d_data;
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_SX_RNG_CNT));
	if (obs_adr != NULL)
	{
		m_d_sx_rng_cnt = obs_adr->d_data;
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_SX_RNG_NUM));
	if (obs_adr != NULL)
	{
		m_d_sx_rng_num = obs_adr->d_data;
	}

	/*** COH/INCOH ************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_COH_INCOH));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_coh_incoh, sizeof(m_sz_coh_incoh), obs_adr->sz_sts_name);
	}

	// COH/INCOH COH カウントダウン中にデータが COH になった場合
	if (m_l_coh_incoh_coh_cntdwn >= 0 && strcmp(m_sz_coh_incoh, "COHI") == 0)
	{
		// カウントダウン終了
		m_l_coh_incoh_coh_cntdwn = -1;
	}
	// COH/INCOH COH カウントダウンタイムリミットになった場合
	else if (m_l_coh_incoh_coh_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_coh_incoh_coh_cntdwn = -1;
	}

	// COH/INCOH INCOH カウントダウン中にデータが INCOH になった場合
	if (m_l_coh_incoh_incoh_cntdwn >= 0 && strcmp(m_sz_coh_incoh, "INCOHI") == 0)
	{
		// カウントダウン終了
		m_l_coh_incoh_incoh_cntdwn = -1;
	}
	// SX-RNG ON カウントダウンタイムリミットになった場合
	else if (m_l_coh_incoh_incoh_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_coh_incoh_incoh_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_coh_incoh_coh_cntdwn == -1 && m_l_coh_incoh_incoh_cntdwn == -1)
	{
		// INCOH の場合
		if (strcmp(m_sz_coh_incoh, "INCOHI") == 0)
		{
			show_coh_incoh_incoh();
		}

		// COH の場合
		else
		{
			show_coh_incoh_coh();
		}
	}


	/*** RNGING ***************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
	}

	// RNGING STOP カウントダウン中にデータが STOP になった場合
	if (m_l_rnging_stop_cntdwn >= 0 && strcmp(m_sz_rnging, "終了") == 0)
	{
		// カウントダウン終了
		m_l_rnging_stop_cntdwn = -1;
	}
	// RNGING STOP カウントダウンタイムリミットになった場合
	else if (m_l_rnging_stop_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rnging_stop_cntdwn = -1;
	}

	// RNGING START カウントダウン中にデータが START になった場合
	if (m_l_rnging_start_cntdwn >= 0 && strcmp(m_sz_rnging, "開始") == 0)
	{
		// カウントダウン終了
		m_l_rnging_start_cntdwn = -1;
	}
	// RNGING START カウントダウンタイムリミットになった場合
	else if (m_l_rnging_start_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rnging_start_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能 
	if (m_l_rnging_stop_cntdwn == -1 && m_l_rnging_start_cntdwn == -1)
	{
		// START の場合
		if (strcmp(m_sz_rnging, "開始") == 0)
		{
			show_rnging_start();
		}

		// STOP の場合
		else
		{
			show_rnging_stop();
		}
	}


	/*** CMD MOD INDEX ********************************************************************/
	/*** 数値表示部最新データ表示 *********************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_CMD_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_cmd_mod_idx = obs_adr->d_data;

		// 数値表示部を更新
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_cmd_mod_idx);
		m_strIndexCmd = CString(sz_buff);

	}

	/*** RNG MOD INDEX ********************************************************************/
	/*** 数値表示部最新データ表示 *********************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_RNG_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_rng_mod_idx = obs_adr->d_data;

		// 数値表示部を更新
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_rng_mod_idx);
		m_strIndexRng = CString(sz_buff);
	}

	/*** DATA SOURCE **********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_DATA_SOURCE));
	if (obs_adr != NULL)
	{
		sprintf_s(m_sz_data_src, sizeof(m_sz_data_src), "%-10s", obs_adr->sz_sts_name);

		// 状態表示部を更新
		m_strIndexCR = CString(m_sz_data_src);
	}


	/*** S帯RNG計測ルート *****************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_S_RNG_ROUTE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_data_src, sizeof(m_sz_data_src), obs_adr->sz_sts_name);
	}

	// 状態表示部を更新
	m_strRngRoute = CString(m_sz_data_src);

//	int id = 0;
	CString id;
	if (strcmp(m_sz_rng_route, "S/S") == 0)
	{
		// S/S帯RNG計測中の場合

		/*** 追跡データ配信状態 ***********************************************************/
		// 現在の状態を監視ＤＢから取得
		id = CString(MON_S34_SS_TRK_STS);
	}
	else
	{
		// S/X帯RNG計測中の場合

		/*** 追跡データ配信状態 ***********************************************************/
		// 現在の状態を監視ＤＢから取得
		id = CString(MON_S34_SX_TRK_STS);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, id);
	if (obs_adr != NULL)
	{
		if (strstr(obs_adr->sz_sts_name, "_SEND") != (char *)NULL)
		{
			strcpy_s(m_sz_data_src, sizeof(m_sz_data_src), "SEND");
		}
		else
		{
			strcpy_s(m_sz_data_src, sizeof(m_sz_data_src), "STOP");
		}
	}

	// 状態表示部を更新
	m_strTrkData = CString(m_sz_data_src);

	/*** テレメトリデータ配信状態 *********************************************************/
	// 現在の状態を監視ＤＢから取得

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_S_TLM_STS));
	if (obs_adr != NULL)
	{
		if (strstr(obs_adr->sz_sts_name, "SEND") == (char *)NULL)
		{
			strcpy_s(m_sz_data_src, sizeof(m_sz_data_src), "STOP");
		}
		else
		{
			strcpy_s(m_sz_data_src, sizeof(m_sz_data_src), obs_adr->sz_sts_name);
		}

		// 状態表示部を更新
		m_strTlmData = CString(m_sz_data_src);
	}


	// 現在の状態を監視ＤＢから取得
	// RF_POWER_SEL
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_POWER_SEL));
	if (obs_adr != NULL)
	{
		strcpy_s(sz_misc1, sizeof(sz_misc1), obs_adr->sz_sts_name);
	}

	// RFSW_UC_ROUTE
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_UC_ROUTE));
	if (obs_adr != NULL)
	{
		strcpy_s(sz_misc2, sizeof(sz_misc2), obs_adr->sz_sts_name);
	}

	if (!(strcmp(sz_misc1, m_sz_data_txpower) == 0 && strcmp(sz_misc2, m_sz_data_ucroute) == 0))
	{
		// どちらかのステータスに変更があった
		strcpy_s(m_sz_data_txpower, sizeof(m_sz_data_txpower), sz_misc1);
		strcpy_s(m_sz_data_ucroute, sizeof(m_sz_data_ucroute), sz_misc2);
		m_strPower = CString(m_sz_data_txpower);
		m_strRoute = CString(m_sz_data_ucroute);
	}

	UpdateData(FALSE);

	SetTimer(TID_CONDITION, INTERVAL_CONDITION, 0);
}

/*============================================================================*/
/*! CTrsControlS34

-# カウントダウンタイムアウト

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::count_down()
{
	TRACE(_T("count_down\n"));
	KillTimer(TID_COUNTDOWN);

	/*-- TX CONTROL ----------------------------------------------------------------------*/

	// RF-POWER OFF
	if (m_l_rf_power_off_cntdwn > 0)
	{
		TRACE(_T("RF-POWER OFF カウントダウン:%d\n"), m_l_rf_power_off_cntdwn);
		m_l_rf_power_off_cntdwn--;
	}

	// RF-POWER ON
	if (m_l_rf_power_on_cntdwn > 0)
	{
		TRACE(_T("RF-POWER ON カウントダウン:%d\n"), m_l_rf_power_on_cntdwn);
		m_l_rf_power_on_cntdwn--;
	}

	// NO-USE OFF
	if (m_l_no_use_off_cntdwn > 0)
	{
		TRACE(_T("NO-USE OFF カウントダウン:%d\n"), m_l_no_use_off_cntdwn);
		m_l_no_use_off_cntdwn--;
	}

	// NO-USE ON
	if (m_l_no_use_on_cntdwn > 0)
	{
		TRACE(_T("NO-USE ON カウントダウン:%d\n"), m_l_no_use_on_cntdwn);
		m_l_no_use_on_cntdwn--;
	}

	// SWEEP-CONT NOMAL STOP
	if (m_l_sweep_cont_stop_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT STOP カウントダウン:%d\n"), m_l_sweep_cont_stop_cntdwn);
		m_l_sweep_cont_stop_cntdwn--;
	}

	// SWEEP-CONT NOMAL INC
	if (m_l_sweep_cont_inc_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT INC カウントダウン:%d\n"), m_l_sweep_cont_inc_cntdwn);
		m_l_sweep_cont_inc_cntdwn--;
	}

	// SWEEP-CONT NOMAL INC+DEC
	if (m_l_sweep_cont_incdec_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT INC+DEC カウントダウン:%d\n"), m_l_sweep_cont_incdec_cntdwn);
		m_l_sweep_cont_incdec_cntdwn--;
	}

	// SWEEP-CONT NOMAL DEC
	if (m_l_sweep_cont_dec_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT DEC カウントダウン:%d\n"), m_l_sweep_cont_dec_cntdwn);
		m_l_sweep_cont_dec_cntdwn--;
	}

	// SWEEP-CONT NOMAL DEC+INC
	if (m_l_sweep_cont_decinc_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT DEC+INC カウントダウン:%d\n"), m_l_sweep_cont_decinc_cntdwn);
		m_l_sweep_cont_decinc_cntdwn--;
	}

	// CMD-MOD OFF
	if (m_l_cmd_mod_off_cntdwn > 0)
	{
		TRACE(_T("CMD-MOD OFF カウントダウン:%d\n"), m_l_cmd_mod_off_cntdwn);
		m_l_cmd_mod_off_cntdwn--;
	}

	// CMD-MOD ON
	if (m_l_cmd_mod_on_cntdwn > 0)
	{
		TRACE(_T("CMD-MOD ON カウントダウン:%d\n"), m_l_cmd_mod_on_cntdwn);
		m_l_cmd_mod_on_cntdwn--;
	}

	// RNG-MOD OFF
	if (m_l_rng_mod_off_cntdwn > 0)
	{
		TRACE(_T("RNG-MOD OFF カウントダウン:%d\n"), m_l_rng_mod_off_cntdwn);
		m_l_rng_mod_off_cntdwn--;
	}

	// RNG-MOD ON
	if (m_l_rng_mod_on_cntdwn> 0)
	{
		TRACE(_T("RNG-MOD ON カウントダウン:%d\n"), m_l_rng_mod_on_cntdwn);
		m_l_rng_mod_on_cntdwn--;
	}

	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	// SS-RNG OFF
	if (m_l_ss_rng_off_cntdwn > 0)
	{
		TRACE(_T("SS-RNG OFF カウントダウン:%d\n"), m_l_ss_rng_off_cntdwn);
		m_l_ss_rng_off_cntdwn--;
	}

	// SS-RNG ON
	if (m_l_ss_rng_on_cntdwn > 0)
	{
		TRACE(_T("SS-RNG ON カウントダウン:%d\n"), m_l_ss_rng_on_cntdwn);
		m_l_ss_rng_on_cntdwn--;
	}

	// SX-RNG OFF
	if (m_l_sx_rng_off_cntdwn > 0)
	{
		TRACE(_T("SX-RNG OFF カウントダウン:%d\n"), m_l_sx_rng_off_cntdwn);
		m_l_sx_rng_off_cntdwn--;
	}

	// SX-RNG ON
	if (m_l_sx_rng_on_cntdwn > 0)
	{
		TRACE(_T("SX-RNG ON カウントダウン:%d\n"), m_l_sx_rng_on_cntdwn);
		m_l_sx_rng_on_cntdwn--;
	}

	// COH/INCOH COH
	if (m_l_coh_incoh_coh_cntdwn > 0)
	{
		TRACE(_T("COH/INCOH COH カウントダウン:%d\n"), m_l_coh_incoh_coh_cntdwn);
		m_l_coh_incoh_coh_cntdwn--;
	}

	// COH/INCOH INCOH
	if (m_l_coh_incoh_incoh_cntdwn > 0)
	{
		TRACE(_T("COH/INCOH INCOH カウントダウン:%d\n"), m_l_coh_incoh_incoh_cntdwn);
		m_l_coh_incoh_incoh_cntdwn--;
	}

	// RNGING STOP
	if (m_l_rnging_stop_cntdwn > 0)
	{
		TRACE(_T("RNGING STOP カウントダウン:%d\n"), m_l_rnging_stop_cntdwn);
		m_l_rnging_stop_cntdwn--;
	}

	// RNGING START
	if (m_l_rnging_start_cntdwn > 0)
	{
		TRACE(_T("RNGING START カウントダウン:%d\n"), m_l_rnging_start_cntdwn);
		m_l_rnging_start_cntdwn--;
	}

	// HOLD TONE OFF
	if (m_l_holdtone_off_cntdwn > 0)
	{
		TRACE(_T("HOLD TONE OFF カウントダウン:%d\n"), m_l_holdtone_off_cntdwn);
		m_l_holdtone_off_cntdwn--;
	}

	// HOLD TONE ON
	if (m_l_holdtone_on_cntdwn > 0)
	{
		TRACE(_T("HOLD TONE ON カウントダウン:%d\n"), m_l_holdtone_on_cntdwn);
		m_l_holdtone_on_cntdwn--;
	}

	SetTimer(TID_COUNTDOWN, INTERVAL_COUNTDOWN, 0);
}

/*============================================================================*/
/*! CTrsControlS34

-# RF-POWER OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::show_rf_power_off()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_RF_ON_ENBL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl, sizeof(m_sz_rf_on_enbl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_X_TX_DET_ANS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_tx_det_ans, sizeof(m_sz_x_tx_det_ans), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_HPA_RF_ON_DET_ANS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_tx_hpa_det_ans, sizeof(m_sz_x_tx_hpa_det_ans), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// OFF 側
	if (strcmp(m_sz_lan_s_tx, "接続") == 0)
	{
		// リモート（選択）表示
		m_RfPwrOffBtn.SetColor(COLOR_REMOTES);	// OFF ボタン
	}
	else
	{
		// ローカル表示
		m_RfPwrOffBtn.SetColor(COLOR_LOCAL);	// OFF ボタン
	}

	// ON 側ボタンの設定
	// リモート時
	if ((strcmp(m_sz_s_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_rf_on_enbl, "ON") == 0) &&
		(strcmp(m_sz_lan_s_tx, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// X-TXのLAN接続状態を確認する
		if ((strcmp(m_sz_lan_x_ul_cnt, "接続") == 0) &&
			(strcmp(m_sz_lan_x_uc, "接続") == 0) &&
			(strcmp(m_sz_lan_x_rf_if, "接続") == 0))
		{

			// X-TX RF DET ANS を確認する
			if ((strcmp(m_sz_x_tx_det_ans, "OFF") == 0) &&
				(strcmp(m_sz_x_tx_hpa_det_ans, "OFF") == 0))
			{
				// リモート（非選択）表示
				m_RfPwrOnBtn.SetColor(COLOR_REMOTE);	// ON ボタン
			}
			else
			{
				// ローカル表示
				m_RfPwrOnBtn.SetColor(COLOR_LOCAL);		// ON ボタン
			}
		}
		else
		{
			// リモート（非選択）表示
			m_RfPwrOnBtn.SetColor(COLOR_REMOTE);	// ON ボタン
		}
	}
	// ローカル時
	else
	{
		// ローカル表示
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);		// ON ボタン
	}

	// 更新
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# RF-POWER ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::show_rf_power_on()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_RF_ON_ENBL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl, sizeof(m_sz_rf_on_enbl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_RF_ON_DET_ANS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans, sizeof(m_sz_rf_on_det_ans), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}
	
	// 緑半灯条件
	if (strcmp(m_sz_lan_s_tx, "接続") != 0)
	{
		// ローカル表示
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);		// ON ボタン
	}
	else if (((strcmp(m_sz_s_tx_rl, "LOCAL") == 0) || (strcmp(m_sz_rf_on_enbl, "OFF") == 0)) &&
		(strcmp(m_sz_rf_on_det_ans, "OFF") == 0))
	{
		// リモート（選択）/ ローカル表示
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES, COLOR_LOCAL, TRUE);		// ON ボタン
	}
	else if (strcmp(m_sz_rf_on_det_ans, "OFF") == 0)
	{
		// リモート（選択）/ リモート（非選択）
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES, COLOR_REMOTE, TRUE);		// ON ボタン
	}
	else
	{
		// リモート（選択）表示
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES);	// ON ボタン
	}

	// OFF 側ボタンの設定
	// リモート時
	if ((strcmp(m_sz_s_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_rf_on_enbl, "OFF") == 0) &&
		(strcmp(m_sz_lan_s_tx, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）表示
		m_RfPwrOffBtn.SetColor(COLOR_REMOTE);	// OFF ボタン
	}
	// ローカル時
	else
	{
		// ローカル表示
		m_RfPwrOffBtn.SetColor(COLOR_LOCAL);	// OFF ボタン
	}

	// 更新
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# NO-USE OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::show_no_use_off()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_NO_USE_ENBL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl, sizeof(m_sz_no_use_enbl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_s_tx, "接続") != 0)
	{
		// ローカル表示
		m_NoUseOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）表示
		m_NoUseOffBtn.SetColor(COLOR_REMOTES);
	}

	// リモート時
	if ((strcmp(m_sz_s_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）表示
		m_NoUseOnBtn.SetColor(COLOR_REMOTE);
	}

	// ローカル時
	else
	{
		// ローカル表示
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_NoUseOffBtn.Invalidate();
	m_NoUseOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# NO-USE ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::show_no_use_on()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_NO_USE_ENBL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl, sizeof(m_sz_no_use_enbl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_s_tx, "接続") != 0)
	{
		// ローカル表示
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）表示
		m_NoUseOnBtn.SetColor(COLOR_REMOTES);
	}

	// リモート時
	if ((strcmp(m_sz_s_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）表示
		m_NoUseOffBtn.SetColor(COLOR_REMOTE);
	}

	// ローカル時
	else
	{
		// ローカル表示
		m_NoUseOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_NoUseOffBtn.Invalidate();
	m_NoUseOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# SWEEP-CONT NORMAL STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::show_sweep_cont_stop()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRMOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_ss_tcrmod, sizeof(m_sz_lan_ss_tcrmod), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_MOD_UPLINK_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_mod_uplink_cont, sizeof(m_sz_mod_uplink_cont), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_ss_tcrmod, "接続") != 0)
	{
		// ローカル表示
		m_SweepContStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）表示
		m_SweepContStopBtn.SetColor(COLOR_REMOTES);
	}

	// リモート時
	if ((strcmp(m_sz_mod_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_ss_tcrmod, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）表示
		m_SweepContIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecIncBtn.SetColor(COLOR_REMOTE);
	}
	// ローカル時
	else
	{
		// ローカル表示
		m_SweepContIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# SWEEP-CONT NORMAL INC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::show_sweep_cont_inc()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRDEM));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_s_txsyn, "接続") != 0)
	{
		// ローカル表示
		m_SweepContIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）表示
		m_SweepContIncBtn.SetColor(COLOR_REMOTES);
	}

	// リモート時
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）表示
		m_SweepContStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecIncBtn.SetColor(COLOR_REMOTE);
	}
	// ローカル時
	else
	{
		// ローカル表示
		m_SweepContStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# SWEEP-CONT NORMAL INCDEC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::show_sweep_cont_incdec()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRDEM));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_s_txsyn, "接続") != 0)
	{
		// ローカル表示
		m_SweepContIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）表示
		m_SweepContIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// リモート時
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）表示
		m_SweepContStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecIncBtn.SetColor(COLOR_REMOTE);
	}
	// ローカル時
	else
	{
		// ローカル表示
		m_SweepContStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# SWEEP-CONT NORMAL DEC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::show_sweep_cont_dec()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRDEM));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_s_txsyn, "接続") != 0)
	{
		// ローカル表示
		m_SweepContDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）表示
		m_SweepContDecBtn.SetColor(COLOR_REMOTES);
	}

	// リモート時
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）表示
		m_SweepContStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecIncBtn.SetColor(COLOR_REMOTE);
	}
	// ローカル時
	else
	{
		// ローカル表示
		m_SweepContStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# SWEEP-CONT NORMAL DECINC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::show_sweep_cont_decinc()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRDEM));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_s_txsyn, "接続") != 0)
	{
		// ローカル表示
		m_SweepContDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）表示
		m_SweepContDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// リモート時
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）表示
		m_SweepContStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecBtn.SetColor(COLOR_REMOTE);
	}
	// ローカル時
	else
	{
		// ローカル表示
		m_SweepContStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContDecBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# CMD-MOD OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::show_cmd_mod_off()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_UPLINK_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRMOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_s_tx, "接続") != 0)
	{
		// ローカル表示
		m_CmdModOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）表示
		m_CmdModOffBtn.SetColor(COLOR_REMOTES);
	}

	// リモート時
	if ((strcmp(m_sz_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）表示
		m_CmdModOnBtn.SetColor(COLOR_REMOTE);
	}
	// ローカル時
	else
	{
		// ローカル表示
		m_CmdModOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# CMD-MOD ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::show_cmd_mod_on()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_UPLINK_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRMOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_s_tx, "接続") != 0)
	{
		// ローカル表示
		m_CmdModOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）表示
		m_CmdModOnBtn.SetColor(COLOR_REMOTES);
	}

	// リモート時
	if ((strcmp(m_sz_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）表示
		m_CmdModOffBtn.SetColor(COLOR_REMOTE);
	}
	// ローカル時
	else
	{
		// ローカル表示
		m_CmdModOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# HOLD TONE OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::show_holdtone_off()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_CMDOUT1));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_cmdout1, sizeof(m_sz_lan_cmdout1), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_cmdout1, sizeof(m_sz_lan_cmdout1), "接続");

	if (strcmp(m_sz_lan_cmdout1, "接続") == 0)
	{
		// リモート（選択）表示
		m_HoldToneOffBtn.SetColor(COLOR_REMOTES);
	}
	else
	{
		// ローカル表示
		m_HoldToneOffBtn.SetColor(COLOR_LOCAL);
	}

	// リモート時
	if ((strcmp(m_sz_lan_cmdout1, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）表示
		m_HoldToneOnBtn.SetColor(COLOR_REMOTE);
	}

	// ローカル時
	else
	{
		// ローカル表示
		m_HoldToneOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# HOLD TONE ON 状態画面表示示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::show_holdtone_on()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_CMDOUT1));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_cmdout1, sizeof(m_sz_lan_cmdout1), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_cmdout1, sizeof(m_sz_lan_cmdout1), "接続");

	if (strcmp(m_sz_lan_cmdout1, "接続") != 0)
	{
		// ローカル表示
		m_HoldToneOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）表示
		m_HoldToneOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF 側ボタンの設定
	// リモート時
	if ((strcmp(m_sz_lan_cmdout1, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）表示
		m_HoldToneOffBtn.SetColor(COLOR_REMOTE);
	}
	// ローカル時
	else
	{
		// ローカル表示
		m_HoldToneOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# COH/INCOH COH 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::show_coh_incoh_coh()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRDEM));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_ss_rarr, "接続") != 0)
	{
		// ローカル表示
		m_CohIncohCohBtn.SetColor(COLOR_LOCAL);
		m_CohIncohInCohBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		if (m_i_priv_flag == 1)
		{
			// リモート（非選択）表示
			m_CohIncohInCohBtn.SetColor(COLOR_REMOTE);
		}
		else
		{
			// ローカル表示
			m_CohIncohInCohBtn.SetColor(COLOR_LOCAL);
		}
		// リモート（選択）表示
		m_CohIncohCohBtn.SetColor(COLOR_REMOTES);
	}

	// 更新
	m_CohIncohCohBtn.Invalidate();
	m_CohIncohInCohBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# COH/INCOH INCOH 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::show_coh_incoh_incoh()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRDEM));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_ss_rarr, "接続") != 0)
	{
		// ローカル表示
		m_CohIncohInCohBtn.SetColor(COLOR_LOCAL);
		m_CohIncohCohBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		if (m_i_priv_flag == 1)
		{
			// リモート（非選択）表示
			m_CohIncohCohBtn.SetColor(COLOR_REMOTE);
		}
		else
		{
			// ローカル表示
			m_CohIncohCohBtn.SetColor(COLOR_LOCAL);
		}
		// リモート（選択）表示
		m_CohIncohInCohBtn.SetColor(COLOR_REMOTES);
	}

	// 更新
	m_CohIncohCohBtn.Invalidate();
	m_CohIncohInCohBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# RNGING STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::show_rnging_stop()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRDEM));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_P_FLG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_pass_flag, sizeof(m_sz_pass_flag), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_ss_rarr, "接続") != 0)
	{
		// ローカル表示
		m_RngingStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）表示
		m_RngingStopBtn.SetColor(COLOR_REMOTES);
	}

	// SS-RNG と SX-RNG の OFF ボタンがどちらも緑点灯の場合は 黒にする
	if ((strcmp(m_sz_pass_flag, "パス中") == 0) &&
		(strcmp(m_sz_lan_ss_rarr, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）表示
		m_RngingStartBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル表示
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# RNGING START 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::show_rnging_start()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照 
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRDEM));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), obs_adr->sz_sts_name);
	}

//	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRDEM));
//	if (obs_adr != NULL)
//	{
//		strcpy_s(m_sz_lan_sx_rarr, sizeof(m_sz_lan_sx_rarr), obs_adr->sz_sts_name);
//	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), "接続");
#endif

	// SS-RNG と SX-RNG の OFF ボタンがどちらも緑点灯の場合は 黒にする
	if (strcmp(m_sz_lan_ss_rarr, "接続") == 0)
	{
		// リモート（選択）表示
		m_RngingStartBtn.SetColor(COLOR_REMOTES);
	}
	else
	{
		// ローカル表示
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}

	if ((strcmp(m_sz_lan_ss_rarr, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）表示
		m_RngingStopBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル表示
		m_RngingStopBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# HOLD TONE ON 状態画面表示示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS34::printlg_endtime(CString str)
{
	// 監視データ取得

	// 現在時刻取得

	// スイープ完了予定時刻算出

	// opelog出力
}

void CTrsControlS34::OnClose()
{
	CString title;
	GetWindowText(title);

	if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("終了します。\nよろしいですか？"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		return;
	}

	CloseDlg();
	//	OnOK();
	ShowWindow(SW_HIDE);
}

void CTrsControlS34::CloseDlg()
{
	KillTimer(TID_CONDITION);
	KillTimer(TID_COUNTDOWN);

	// ウィンドウを閉じるメッセージ
	theApp.m_pMainWnd->PostMessageW(eMessage_WindowClose, 0, (LPARAM)this);
}
