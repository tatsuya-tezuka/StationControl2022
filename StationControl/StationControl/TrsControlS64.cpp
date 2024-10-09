// TrsControlS64.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrsControlS64.h"
#include "afxdialogex.h"
#include "TrsControlMng.h"
#include "HookMsgBox.h"


// CTrsControlS64 ダイアログ

IMPLEMENT_DYNAMIC(CTrsControlS64, CDialogBase)

CTrsControlS64::CTrsControlS64(CWnd* pParent /*=NULL*/)
: CDialogBase(CTrsControlS64::IDD, pParent)
, m_bSweepContTriangle(FALSE)
, m_strIndexCmd(_T(""))
, m_strIndexRng(_T(""))
, m_strIndexCR(_T(""))
{
	m_i_priv_flag = -1;
	m_i_sweep_flag = 0;
}

CTrsControlS64::~CTrsControlS64()
{
}

void CTrsControlS64::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRSCTRL_RF_OFF_BTN, m_RfPwrOffBtn);
	DDX_Control(pDX, IDC_TRSCTRL_RF_ON_BTN, m_RfPwrOnBtn);
	DDX_Control(pDX, IDC_TRSCTRL_NOUSE_OFF_BTN, m_NoUseOffBtn);
	DDX_Control(pDX, IDC_TRSCTRL_NOUSE_ON_BTN, m_NoUseOnBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEEPN_STOP_BTN, m_SweepContNStopBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEEPN_INC_BTN, m_SweepContNIncBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEEPN_INCDEC_BTN, m_SweepContNIncDecBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEEPN_DEC_BTN, m_SweepContNDecBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEEPN_DECINC_BTN, m_SweepContNDecIncBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEEPT_STOP_BTN, m_SweepContTStopBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEEPT_INC_BTN, m_SweepContTIncBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEEPT_INCDEC_BTN, m_SweepContTIncDecBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEEPT_DEC_BTN, m_SweepContTDecBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEEPT_DECINC_BTN, m_SweepContTDecIncBtn);
	DDX_Control(pDX, IDC_TRSCTRL_MOD_OFF_BTN, m_ModOffBtn);
	DDX_Control(pDX, IDC_TRSCTRL_MOD_ON_BTN, m_ModOnBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SSRNG_OFF_BTN, m_SSRngOffBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SSRNG_ON_BTN, m_SSRngOnBtn);
	DDX_Control(pDX, IDC_TRSCTRL_COHINCOH_COH_BTN, m_CohIncohCohBtn);
	DDX_Control(pDX, IDC_TRSCTRL_COHINCOH_INCOH_BTN, m_CohIncohInCohBtn);
	DDX_Control(pDX, IDC_TRSCTRL_RNGING_STOP_BTN, m_RngingStopBtn);
	DDX_Control(pDX, IDC_TRSCTRL_RNGING_START_BTN, m_RngingStartBtn);
	DDX_Control(pDX, IDC_TRSCTRL_HOLD_OFF_BTN, m_HoldToneOffBtn);
	DDX_Control(pDX, IDC_TRSCTRL_HOLD_ON_BTN, m_HoldToneOnBtn);
	DDX_Check(pDX, IDC_TRSCTRL_SWEEPT_CHECK, m_bSweepContTriangle);
	DDX_Text(pDX, IDC_TRSCTRL_IDX_CMD_EDIT, m_strIndexCmd);
	DDX_Text(pDX, IDC_TRSCTRL_IDX_RNG_EDIT, m_strIndexRng);
	DDX_Text(pDX, IDC_TRSCTRL_IDX_CR_EDIT, m_strIndexCR);
	DDX_Control(pDX, IDC_TRSCTRL_IDX_CMD_BTN, m_IndexCmdBtn);
	DDX_Control(pDX, IDC_TRSCTRL_IDX_RNG_BTN, m_IndexRngBtn);
	DDX_Control(pDX, IDC_TRSCTRL_IDX_CR_BTN, m_IndexCRBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEEPNUM_BTN, m_SweepNumBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEEPT_CHECK, m_SweepContTriChkBtn);
	DDX_Control(pDX, IDC_TRSCTRL_TX_STATIC, m_grTxControl);
	DDX_Control(pDX, IDC_TRSCTRL_RF_STATIC, m_grRfPower);
	DDX_Control(pDX, IDC_TRSCTRL_NOUSE_STATIC, m_grNoUse);
	DDX_Control(pDX, IDC_TRSCTRL_SWEEPN_STATIC, m_grSweepContNormal);
	DDX_Control(pDX, IDC_TRSCTRL_SWEEPT_STATIC, m_grSweepContTriangle);
	DDX_Control(pDX, IDC_TRSCTRL_MOD_STATIC, m_grMod);
	DDX_Control(pDX, IDC_TRSCTRL_IDX_STATIC, m_grIndex);
	DDX_Control(pDX, IDC_TRSCTRL_RNGCTRL_STATIC, m_grRngControl);
	DDX_Control(pDX, IDC_TRSCTRL_SSRNG_STATIC, m_grSSRng);
	DDX_Control(pDX, IDC_TRSCTRL_COHINCOH_STATIC, m_grCohIncoh);
	DDX_Control(pDX, IDC_TRSCTRL_RNGING_STATIC, m_grRnging);
	DDX_Control(pDX, IDC_TRSCTRL_HOLD_STATIC, m_grHoldTone);
}


BEGIN_MESSAGE_MAP(CTrsControlS64, CDialogBase)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CTrsControlS64::OnMenuClose)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TRSCTRL_SWEEPT_CHECK, &CTrsControlS64::OnBnClickedTrsctrlSweeptCheck)
	ON_BN_CLICKED(IDC_TRSCTRL_IDX_CMD_BTN, &CTrsControlS64::OnBnClickedTrsctrlIdxCmdBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_IDX_RNG_BTN, &CTrsControlS64::OnBnClickedTrsctrlIdxRngBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_IDX_CR_BTN, &CTrsControlS64::OnBnClickedTrsctrlIdxCrBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEEPNUM_BTN, &CTrsControlS64::OnBnClickedTrsctrlSweepnumBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_RF_OFF_BTN, &CTrsControlS64::OnBnClickedTrsctrlRfOffBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_RF_ON_BTN, &CTrsControlS64::OnBnClickedTrsctrlRfOnBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_NOUSE_OFF_BTN, &CTrsControlS64::OnBnClickedTrsctrlNouseOffBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_NOUSE_ON_BTN, &CTrsControlS64::OnBnClickedTrsctrlNouseOnBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEEPN_STOP_BTN, &CTrsControlS64::OnBnClickedTrsctrlSweepnStopBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEEPN_INC_BTN, &CTrsControlS64::OnBnClickedTrsctrlSweepnIncBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEEPN_INCDEC_BTN, &CTrsControlS64::OnBnClickedTrsctrlSweepnIncdecBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEEPN_DEC_BTN, &CTrsControlS64::OnBnClickedTrsctrlSweepnDecBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEEPN_DECINC_BTN, &CTrsControlS64::OnBnClickedTrsctrlSweepnDecincBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEEPT_STOP_BTN, &CTrsControlS64::OnBnClickedTrsctrlSweeptStopBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEEPT_INC_BTN, &CTrsControlS64::OnBnClickedTrsctrlSweeptIncBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEEPT_INCDEC_BTN, &CTrsControlS64::OnBnClickedTrsctrlSweeptIncdecBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEEPT_DEC_BTN, &CTrsControlS64::OnBnClickedTrsctrlSweeptDecBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEEPT_DECINC_BTN, &CTrsControlS64::OnBnClickedTrsctrlSweeptDecincBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_MOD_OFF_BTN, &CTrsControlS64::OnBnClickedTrsctrlModOffBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_MOD_ON_BTN, &CTrsControlS64::OnBnClickedTrsctrlModOnBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SSRNG_OFF_BTN, &CTrsControlS64::OnBnClickedTrsctrlSsrngOffBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SSRNG_ON_BTN, &CTrsControlS64::OnBnClickedTrsctrlSsrngOnBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_COHINCOH_COH_BTN, &CTrsControlS64::OnBnClickedTrsctrlCohincohCohBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_COHINCOH_INCOH_BTN, &CTrsControlS64::OnBnClickedTrsctrlCohincohIncohBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_RNGING_STOP_BTN, &CTrsControlS64::OnBnClickedTrsctrlRngingStopBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_RNGING_START_BTN, &CTrsControlS64::OnBnClickedTrsctrlRngingStartBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_HOLD_OFF_BTN, &CTrsControlS64::OnBnClickedTrsctrlHoldOffBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_HOLD_ON_BTN, &CTrsControlS64::OnBnClickedTrsctrlHoldOnBtn)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTrsControlS64 メッセージ ハンドラー


/*============================================================================*/
/*! COperationPlan

-# ファイル-閉じるメニュー処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnMenuClose()
{
	// いずれかのボタンが実行中の場合
	if (m_RfPwrOffBtn.IsColorIgnore2() ||
		m_RfPwrOnBtn.IsColorIgnore2() ||
		m_NoUseOffBtn.IsColorIgnore2() ||
		m_NoUseOnBtn.IsColorIgnore2() ||
		m_SweepContNStopBtn.IsColorIgnore2() ||
		m_SweepContNIncBtn.IsColorIgnore2() ||
		m_SweepContNIncDecBtn.IsColorIgnore2() ||
		m_SweepContNDecBtn.IsColorIgnore2() ||
		m_SweepContNDecIncBtn.IsColorIgnore2() ||
		m_SweepContTStopBtn.IsColorIgnore2() ||
		m_SweepContTIncBtn.IsColorIgnore2() ||
		m_SweepContTIncDecBtn.IsColorIgnore2() ||
		m_SweepContTDecBtn.IsColorIgnore2() ||
		m_SweepContTDecIncBtn.IsColorIgnore2() ||
		m_SweepNumBtn.IsColorIgnore2() ||
		m_ModOffBtn.IsColorIgnore2() ||
		m_ModOnBtn.IsColorIgnore2() ||
		m_SSRngOffBtn.IsColorIgnore2() ||
		m_SSRngOnBtn.IsColorIgnore2() ||
		m_CohIncohCohBtn.IsColorIgnore2() ||
		m_CohIncohInCohBtn.IsColorIgnore2() ||
		m_RngingStopBtn.IsColorIgnore2() ||
		m_RngingStartBtn.IsColorIgnore2() ||
		m_HoldToneOffBtn.IsColorIgnore2() ||
		m_HoldToneOnBtn.IsColorIgnore2() ||
		m_IndexCmdBtn.IsColorIgnore2() ||
		m_IndexRngBtn.IsColorIgnore2() ||
		m_IndexCRBtn.IsColorIgnore2())
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
/*! CTrsControlS64

-# ダイアログ初期化処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CTrsControlS64::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// RF-POWER
	m_RfPwrOffBtn.SetColor(COLOR_REMOTES);
	m_RfPwrOffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_RfPwrOnBtn.SetColor(COLOR_REMOTE);
	m_RfPwrOnBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// NO-USER
	m_NoUseOffBtn.SetColor(COLOR_REMOTES);
	m_NoUseOffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_NoUseOnBtn.SetColor(COLOR_REMOTES);
	m_NoUseOnBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// SWEEP-CONT(NORMAL)
	m_SweepContNStopBtn.SetColor(COLOR_REMOTES);
	m_SweepContNStopBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepContNIncBtn.SetColor(COLOR_REMOTE);
	m_SweepContNIncBtn.SetCtrlButton(TRUE);			// Ctrlキー +
	m_SweepContNIncDecBtn.SetColor(COLOR_REMOTE);
	m_SweepContNIncDecBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepContNDecBtn.SetColor(COLOR_REMOTE);
	m_SweepContNDecBtn.SetCtrlButton(TRUE);			// Ctrlキー +
	m_SweepContNDecIncBtn.SetColor(COLOR_REMOTE);
	m_SweepContNDecIncBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// SWEEP-CONT(TRIANGLE)
	m_SweepContTStopBtn.SetColor(COLOR_REMOTES);
	m_SweepContTStopBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepContTIncBtn.SetColor(COLOR_REMOTE);
	m_SweepContTIncBtn.SetCtrlButton(TRUE);			// Ctrlキー +
	m_SweepContTIncDecBtn.SetColor(COLOR_REMOTE);
	m_SweepContTIncDecBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepContTDecBtn.SetColor(COLOR_REMOTE);
	m_SweepContTDecBtn.SetCtrlButton(TRUE);			// Ctrlキー +
	m_SweepContTDecIncBtn.SetColor(COLOR_REMOTE);
	m_SweepContTDecIncBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// SWEEP-NUM
	m_SweepNumBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_SweepNumBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// MOD
	m_ModOffBtn.SetColor(COLOR_REMOTES);
	m_ModOffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_ModOnBtn.SetColor(COLOR_REMOTE);
	m_ModOnBtn.SetCtrlButton(TRUE);			// Ctrlキー +

	// SS-RNG
	m_SSRngOffBtn.SetColor(COLOR_REMOTES);
	m_SSRngOffBtn.SetCtrlButton(TRUE);			// Ctrlキー +
	m_SSRngOnBtn.SetColor(COLOR_REMOTE);
	m_SSRngOnBtn.SetCtrlButton(TRUE);			// Ctrlキー +

	// COH/INCOH
	m_CohIncohCohBtn.SetColor(COLOR_REMOTES);
	m_CohIncohCohBtn.SetCtrlButton(TRUE);			// Ctrlキー +
	m_CohIncohInCohBtn.SetColor(COLOR_REMOTES);
	m_CohIncohInCohBtn.SetCtrlButton(TRUE);			// Ctrlキー +

	// RNGING
	m_RngingStopBtn.SetColor(COLOR_REMOTES);
	m_RngingStopBtn.SetCtrlButton(TRUE);			// Ctrlキー +
	m_RngingStartBtn.SetColor(COLOR_REMOTE);
	m_RngingStartBtn.SetCtrlButton(TRUE);			// Ctrlキー +

	// HOLD-TONE
	m_HoldToneOffBtn.SetColor(COLOR_REMOTES);
	m_HoldToneOffBtn.SetCtrlButton(TRUE);			// Ctrlキー +
	m_HoldToneOnBtn.SetColor(COLOR_REMOTE);
	m_HoldToneOnBtn.SetCtrlButton(TRUE);			// Ctrlキー +

	// INDEX-CMD
	m_IndexCmdBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_IndexCmdBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// INDEX-RNG
	m_IndexRngBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_IndexRngBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// INDEX-C+R
	m_IndexCRBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_IndexCRBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// タイムアウトカウントダウン値初期設定
	m_l_rf_power_off_cntdwn = -1;
	m_l_rf_power_on_cntdwn = -1;
	m_l_no_use_off_cntdwn = -1;
	m_l_no_use_on_cntdwn = -1;

	m_l_nor_sweep_cont_stop_cntdwn = -1;
	m_l_nor_sweep_cont_inc_cntdwn = -1;
	m_l_nor_sweep_cont_incdec_cntdwn = -1;
	m_l_nor_sweep_cont_dec_cntdwn = -1;
	m_l_nor_sweep_cont_decinc_cntdwn = -1;
	m_l_tri_sweep_cont_stop_cntdwn = -1;
	m_l_tri_sweep_cont_inc_cntdwn = -1;
	m_l_tri_sweep_cont_incdec_cntdwn = -1;
	m_l_tri_sweep_cont_dec_cntdwn = -1;
	m_l_tri_sweep_cont_decinc_cntdwn = -1;
	m_l_sweep_num_cntdwn = -1;
	m_l_sweep_num_set = -1;
	m_l_cmd_mod_off_cntdwn = -1;
	m_l_cmd_mod_on_cntdwn = -1;
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

	// グループボックス
	m_grTxControl.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grTxControl.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRfPower.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRfPower.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grNoUse.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grNoUse.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grSweepContNormal.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grSweepContNormal.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grSweepContTriangle.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grSweepContTriangle.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grMod.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grMod.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grIndex.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grIndex.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRngControl.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRngControl.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grSSRng.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grSSRng.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grCohIncoh.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grCohIncoh.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRnging.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRnging.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grHoldTone.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grHoldTone.SetGroupType(CGroupBoxEx::eGroupType_Round);


	// ホスト名取得
	memset(m_sz_hostname, (char)NULL, sizeof(m_sz_hostname));
	gethostname(m_sz_hostname, sizeof(m_sz_hostname));

	// SWEEP-CONT TRIANGLEチェックボタン
	m_bSweepContTriangle = FALSE;
	if (m_bSweepContTriangle)
	{
		// ボタン活性
		m_SweepContTStopBtn.EnableWindow(TRUE);
		m_SweepContTIncBtn.EnableWindow(TRUE);
		m_SweepContTIncDecBtn.EnableWindow(TRUE);
		m_SweepContTDecBtn.EnableWindow(TRUE);
		m_SweepContTDecIncBtn.EnableWindow(TRUE);
	}
	else
	{
		// ボタン非活性
		m_SweepContTStopBtn.EnableWindow(FALSE);
		m_SweepContTIncBtn.EnableWindow(FALSE);
		m_SweepContTIncDecBtn.EnableWindow(FALSE);
		m_SweepContTDecBtn.EnableWindow(FALSE);
		m_SweepContTDecIncBtn.EnableWindow(FALSE);
	}

#if 0
	// 送信制御画面状態表示タイムアウトコールバックルーチン起動
	show_opsc_condition();

	// カウントダウンタイムアウトコールバックルーチン起動
	count_down();
#endif

	CenterWindowEx();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CTrsControlS64

-# ダイアログ更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CTrsControlS64::UpdateDlg()
{
	// 送信制御画面状態表示タイムアウトコールバックルーチン起動
	show_opsc_condition();

	// カウントダウンタイムアウトコールバックルーチン起動
	count_down();

	return TRUE;
}


/*============================================================================*/
/*! CTrsControlS64

-# 『TX CONTROL』-『RF-POWER』-『OFF』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlRfOffBtn()
{
	// ボタンを制御中色にする
	m_RfPwrOffBtn.SetColor(COLOR_CTRLIN);
	m_RfPwrOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_RF_OUTPUT_CONT"), _T("OFF"));

	// カウントダウン開始
	m_l_rf_power_off_cntdwn = RF_POWER_TIME_S64_LIMIT;

	// 更新
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『TX CONTROL』-『RF-POWER』-『ON』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlRfOnBtn()
{
	// ボタンを制御中色にする
	m_RfPwrOnBtn.SetColor(COLOR_CTRLIN);
	m_RfPwrOffBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_RF_OUTPUT_CONT"), _T("ON"));

	// カウントダウン開始
	m_l_rf_power_on_cntdwn = RF_POWER_TIME_S64_LIMIT;

	// 更新
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# 『TX CONTROL』-『NO-USE』-『OFF』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlNouseOffBtn()
{
	COLORREF col = m_NoUseOffBtn.GetColor();

	if (col == COLOR_REMOTE)	// リモート（非選択）
	{
		// ボタンを制御中色にする
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// 制御通知送信
		theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_NO_USE"), _T("OFF"));

		// カウントダウン開始
		m_l_no_use_off_cntdwn = NO_USE_TIME_S64_LIMIT;
	}
	else if (col == COLOR_CTRLIN)	// 制御中
	{
		// ボタンを制御中色にする
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// 制御通知送信
		theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_NO_USE"), _T("OFF"));

		// カウントダウン開始
		m_l_no_use_off_cntdwn = NO_USE_TIME_S64_LIMIT;

		// 他方のカウントダウン終了
		m_l_no_use_on_cntdwn = -1;
	}
	else if (col == COLOR_REMOTES)	// リモート（選択）
	{
		// ボタンを制御中色にする
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// 制御通知送信
		theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_NO_USE"), _T("OFF"));

		// カウントダウン開始
		m_l_no_use_off_cntdwn = NO_USE_TIME_S64_LIMIT;

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
/*! CTrsControlS64

-# 『TX CONTROL』-『NO-USE』-『ON』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlNouseOnBtn()
{
	COLORREF col = m_NoUseOnBtn.GetColor();

	if (col == COLOR_REMOTE)	// リモート（非選択）
	{
		// ボタンを制御中色にする
		m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

		// 制御通知送信
		theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_NO_USE"), _T("ON"));

		// カウントダウン開始
		m_l_no_use_on_cntdwn = NO_USE_TIME_S64_LIMIT;
	}
	else if (col == COLOR_CTRLIN)	// 制御中
	{
		// ボタンを制御中色にする
		m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

		// 制御通知送信
		theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_NO_USE"), _T("ON"));

		// カウントダウン開始
		m_l_no_use_on_cntdwn = NO_USE_TIME_S64_LIMIT;

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
/*! CTrsControlS64

-# 『TX CONTROL』-『SWEEP-COUNT(NORMAL)』-『STOP』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweepnStopBtn()
{
	// ボタンを制御中色にする
	m_SweepContNStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.SWEEP_CONT"), _T("OFF"));

	// カウントダウン開始
	m_l_nor_sweep_cont_stop_cntdwn = SWEEP_CONT_TIME_S64_LIMIT;

	// 更新
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『TX CONTROL』-『SWEEP-COUNT(NORMAL)』-『INC』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweepnIncBtn()
{
	// ボタンを制御中色にする
	m_SweepContNStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.SWEEP_CONT"), _T("INC"));

	// カウントダウン開始
	m_l_nor_sweep_cont_inc_cntdwn = SWEEP_CONT_TIME_S64_LIMIT;

	// 更新
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『TX CONTROL』-『SWEEP-COUNT(NORMAL)』-『INCDEC』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweepnIncdecBtn()
{
	// ボタンを制御中色にする
	m_SweepContNStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.SWEEP_CONT"), _T("INC+DEC"));

	// カウントダウン開始
	m_l_nor_sweep_cont_incdec_cntdwn = SWEEP_CONT_TIME_S64_LIMIT;

	// 更新
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『TX CONTROL』-『SWEEP-COUNT(NORMAL)』-『DEC』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweepnDecBtn()
{
	// ボタンを制御中色にする
	m_SweepContNStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.SWEEP_CONT"), _T("DEC"));

	// カウントダウン開始
	m_l_nor_sweep_cont_dec_cntdwn = SWEEP_CONT_TIME_S64_LIMIT;

	// 更新
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『TX CONTROL』-『SWEEP-COUNT(NORMAL)』-『DECINC』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweepnDecincBtn()
{
	// ボタンを制御中色にする
	m_SweepContNStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.SWEEP_CONT"), _T("DEC+INC"));

	// カウントダウン開始
	m_l_nor_sweep_cont_decinc_cntdwn = SWEEP_CONT_TIME_S64_LIMIT;

	// 更新
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT (TRIANGLE)チェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweeptCheck()
{
	UpdateData();

	if (m_bSweepContTriangle)
	{
		// ボタン活性
		m_SweepContTStopBtn.EnableWindow(TRUE);
		m_SweepContTIncBtn.EnableWindow(TRUE);
		m_SweepContTIncDecBtn.EnableWindow(TRUE);
		m_SweepContTDecBtn.EnableWindow(TRUE);
		m_SweepContTDecIncBtn.EnableWindow(TRUE);
	}
	else
	{
		// ボタン非活性
		m_SweepContTStopBtn.EnableWindow(FALSE);
		m_SweepContTIncBtn.EnableWindow(FALSE);
		m_SweepContTIncDecBtn.EnableWindow(FALSE);
		m_SweepContTDecBtn.EnableWindow(FALSE);
		m_SweepContTDecIncBtn.EnableWindow(FALSE);
	}
}


/*============================================================================*/
/*! CTrsControlS64

-# 『TX CONTROL』-『SWEEP-COUNT(TRIANGLE)』-『STOP』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweeptStopBtn()
{
	// ボタンを制御中色にする
	m_SweepContTStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.TR_SWEEP_CONT"), _T("OFF"));

	// カウントダウン開始
	m_l_tri_sweep_cont_stop_cntdwn = TR_SWEEP_CONT_TIME_S64_LIMIT;

	// 更新
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『TX CONTROL』-『SWEEP-COUNT(TRIANGLE)』-『INC』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweeptIncBtn()
{
	// ボタンを制御中色にする
	m_SweepContTStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.TR_SWEEP_CONT"), _T("INC"));

	// カウントダウン開始
	m_l_tri_sweep_cont_inc_cntdwn = TR_SWEEP_CONT_TIME_S64_LIMIT;

	// 更新
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『TX CONTROL』-『SWEEP-COUNT(TRIANGLE)』-『INCDEC』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweeptIncdecBtn()
{
	// ボタンを制御中色にする
	m_SweepContTStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.TR_SWEEP_CONT"), _T("INC+DEC"));

	// カウントダウン開始
	m_l_tri_sweep_cont_incdec_cntdwn = TR_SWEEP_CONT_TIME_S64_LIMIT;

	// 更新
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『TX CONTROL』-『SWEEP-COUNT(TRIANGLE)』-『DEC』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweeptDecBtn()
{
	// ボタンを制御中色にする
	m_SweepContTStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.TR_SWEEP_CONT"), _T("DEC"));

	// カウントダウン開始
	m_l_tri_sweep_cont_dec_cntdwn = TR_SWEEP_CONT_TIME_S64_LIMIT;

	// 更新
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『TX CONTROL』-『SWEEP-COUNT(TRIANGLE)』-『DECINC』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweeptDecincBtn()
{
	// ボタンを制御中色にする
	m_SweepContTStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.TR_SWEEP_CONT"), _T("DEC+INC"));

	// カウントダウン開始
	m_l_tri_sweep_cont_decinc_cntdwn = TR_SWEEP_CONT_TIME_S64_LIMIT;

	// 更新
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『TX CONTROL』-『SWEEP-NUM』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweepnumBtn()
{
	CString str1, str2;
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "接続");

	// リモート時
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "接続") == 0) && (m_i_priv_flag == 1)) 
	{
		CString val;
		m_SweepNumBtn.GetWindowTextW(val);
		// 制御通知送信
		theApp.GetSatelliteData().opctproc(_T("S-UCSYN.SWEEP_CYCLE"), _T(""), val);

		// カウントダウン開始
		m_l_sweep_num_cntdwn = TR_SWEEP_NUM_TIME_S64_LIMIT;
	}
}


/*============================================================================*/
/*! CTrsControlS64

-# 『TX CONTROL』-『MOD』-『OFF』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlModOffBtn()
{
	// ボタンを制御中色にする
	m_ModOffBtn.SetColor(COLOR_CTRLIN);
	m_ModOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.MODULATION"), _T("OFF"));

	// カウントダウン開始
	m_l_cmd_mod_off_cntdwn = CMD_MOD_TIME_S64_LIMIT;

	// 更新
	m_ModOffBtn.Invalidate();
	m_ModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『TX CONTROL』-『MOD』-『ON』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlModOnBtn()
{
	// ボタンを制御中色にする
	m_ModOffBtn.SetColor(COLOR_CTRLIN);
	m_ModOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.MODULATION"), _T("ON"));

	// カウントダウン開始
	m_l_cmd_mod_on_cntdwn = CMD_MOD_TIME_S64_LIMIT;

	// 更新
	m_ModOffBtn.Invalidate();
	m_ModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『RNG CONTROL』-『SS-RNG』-『OFF』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSsrngOffBtn()
{
	// ボタンを制御中色にする
	m_SSRngOffBtn.SetColor(COLOR_CTRLIN);
	m_SSRngOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("V-EQU.RNG_SS"), _T("OFF"));

	// カウントダウン開始
	m_l_ss_rng_off_cntdwn = SS_RNG_TIME_S64_LIMIT;

	// 更新
	m_SSRngOffBtn.Invalidate();
	m_SSRngOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『RNG CONTROL』-『SS-RNG』-『ON』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSsrngOnBtn()
{
	// ボタンを制御中色にする
	m_SSRngOffBtn.SetColor(COLOR_CTRLIN);
	m_SSRngOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("V-EQU.RNG_SS"), _T("ON"));

	// カウントダウン開始
	m_l_ss_rng_on_cntdwn = SS_RNG_TIME_S64_LIMIT;

	// 更新
	m_SSRngOffBtn.Invalidate();
	m_SSRngOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『RNG CONTROL』-『CHO/INCOH』-『COH』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlCohincohCohBtn()
{
	// ボタンを制御中色にする
	m_CohIncohCohBtn.SetColor(COLOR_CTRLIN);
	m_CohIncohInCohBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("COHI"));

	// カウントダウン開始
	m_l_coh_incoh_coh_cntdwn = COH_INCOH_TIME_S64_LIMIT;

	// 更新
	m_CohIncohCohBtn.Invalidate();
	m_CohIncohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『RNG CONTROL』-『CHO/INCOH』-『INCOH』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlCohincohIncohBtn()
{
	// ボタンを制御中色にする
	m_CohIncohCohBtn.SetColor(COLOR_CTRLIN);
	m_CohIncohInCohBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("INCOHI"));

	// カウントダウン開始
	m_l_coh_incoh_incoh_cntdwn = COH_INCOH_TIME_S64_LIMIT;

	// 更新
	m_CohIncohCohBtn.Invalidate();
	m_CohIncohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『RNG CONTROL』-『RNGING』-『STOP』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlRngingStopBtn()
{
	// ボタンを制御中色にする
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING_CANCEL"), _T(","));

	// カウントダウン開始
	m_l_rnging_stop_cntdwn = RNGING_TIME_S64_LIMIT;

	// 更新
	m_RngingStopBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『RNG CONTROL』-『RNGING』-『START』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlRngingStartBtn()
{
	CString str;

	// ボタンを制御中色にする
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);
	m_RngingStartBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	str.Format(_T("S-%02d"), 99);
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str);

	// カウントダウン開始
	m_l_rnging_start_cntdwn = RNGING_TIME_S64_LIMIT;

	// 更新
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『HOLD-TONE』-『OFF』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlHoldOffBtn()
{
	// ボタンを制御中色にする
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CMDOUT.HOLD_TONE"), _T("OFF"));

	// カウントダウン開始
	m_l_holdtone_off_cntdwn = HOLDTONE_TIME_S64_LIMIT;

	// 更新
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『HOLD-TONE』-『ON』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlHoldOnBtn()
{
	// ボタンを制御中色にする
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CMDOUT.HOLD_TONE"), _T("ON"));

	// カウントダウン開始
	m_l_holdtone_on_cntdwn = HOLDTONE_TIME_S64_LIMIT;

	// 更新
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# 『INDEX』 - 『CMD』 ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlIdxCmdBtn()
{
	CString strCmd;

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	memset(m_sz_sat_name, (char)NULL, sizeof(m_sz_sat_name));
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_SAT_NAME));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sat_name, sizeof(m_sz_sat_name), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "接続");
	}

	// リモート時
	if ((strcmp(m_sz_s_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "接続") == 0) &&
		(strlen(m_sz_sat_name) > 0) && (m_i_priv_flag == 1)) 
	{
		// 設備詳細制御プロセス(opctproc)起動
		strCmd.Format(_T("CMD,%s"), (LPCTSTR)CString(m_sz_sat_name));
		theApp.GetSatelliteData().opctproc2(_T("CTRL.MOD_INDEX"), strCmd);
	}
}


/*============================================================================*/
/*! CTrsControlS64

-# 『INDEX』 - 『RNG』 ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlIdxRngBtn()
{
	CString strCmd;

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	memset(m_sz_sat_name, (char)NULL, sizeof(m_sz_sat_name));
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_SAT_NAME));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sat_name, sizeof(m_sz_sat_name), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "接続");
	}

	// リモート時
#if 1
	if ((strcmp(m_sz_s_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "接続") == 0) &&
		(strlen(m_sz_sat_name) > 0) &&
		(m_i_priv_flag == 1))

#else
	if ((strcmp(m_sz_mod_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_ss_tcrmod, "接続") == 0) &&
		(strlen(m_sz_sat_name) > 0) &&
		(m_i_priv_flag == 1))
#endif
	{
		// 設備詳細制御プロセス(opctproc)起動
		strCmd.Format(_T("RNG,%s"), (LPCTSTR)CString(m_sz_sat_name));
		theApp.GetSatelliteData().opctproc2(_T("CTRL.MOD_INDEX"), strCmd);
	}
}


/*============================================================================*/
/*! CTrsControlS64

-# 『INDEX』 - 『C+R』 ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlIdxCrBtn()
{
	CString strCmd;

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	memset(m_sz_sat_name, (char)NULL, sizeof(m_sz_sat_name));
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_SAT_NAME));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sat_name, sizeof(m_sz_sat_name), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "接続");
	}

	// リモート時
#if 1
	if ((strcmp(m_sz_s_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "接続") == 0) &&
		(strlen(m_sz_sat_name) > 0) &&
		(m_i_priv_flag == 1))
#else
	if ((strcmp(m_sz_mod_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_ss_tcrmod, "接続") == 0) &&
		(strlen(m_sz_sat_name) > 0) &&
		(m_i_priv_flag == 1))
#endif
	{
		// 設備詳細制御プロセス(opctproc)起動
		strCmd.Format(_T("CMD+RNG,%s"), (LPCTSTR)CString(m_sz_sat_name));
		theApp.GetSatelliteData().opctproc2(_T("CTRL.MOD_INDEX"), strCmd);
	}
}

/*============================================================================*/
/*! CTrsControlS64

-# OnTimer

@param  nIDEvent:タイマーの識別子
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::OnTimer(UINT_PTR nIDEvent)
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
/*! CTrsControlS64

-# 送信制御画面状態表示設定

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_opsc_condition()
{
	TRACE(_T("show_opsc_condition\n"));

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	char sz_buff[64];

	KillTimer(TID_CONDITION);

	CSatelliteData& sd = theApp.GetSatelliteData();

	CString msg = _T("");

#if 1
	// 制御権は常に有り
	m_i_priv_flag = 1;
#else
	// 制御権が強制開放されていないか確認
	CString strHost = CString(m_sz_hostname);
	if (strHost.Left(5).CompareNoCase(mHostNameString[1]) == 0)
	{
		// 相模原局の場合
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("相模原局"), _T(""), nLogEx::debug);

		memset(m_sz_priv_ssoc, (char)NULL, sizeof(m_sz_priv_ssoc));
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_PRIV_SSOC));
		if (obs_adr != NULL)
		{
			strcpy_s(m_sz_priv_ssoc, sizeof(m_sz_priv_ssoc), obs_adr->sz_sts_name);

			CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("CTRL.PRIV_SSOC"), CString(m_sz_priv_ssoc), nLogEx::debug);

			if (strcmp(m_sz_priv_ssoc, "OFF") == 0)
			{
				// 制御権を取得していない場合
				m_i_priv_flag = 0;
			}
			else
			{
				// 制御権を取得している場合
				m_i_priv_flag = 1;
			}
		}
		else
		{
			// 制御権を取得していない場合
			m_i_priv_flag = 0;
		}
	}
	else
	{
		// 出先局の場合
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("出先局"), _T(""), nLogEx::debug);

		memset(m_sz_priv_sttn, (char)NULL, sizeof(m_sz_priv_sttn));
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_PRIV_STTN));
		if (obs_adr != NULL)
		{
			strcpy_s(m_sz_priv_sttn, sizeof(m_sz_priv_sttn), obs_adr->sz_sts_name);

			CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("CTRL.PRIV_STTN"), CString(m_sz_priv_sttn), nLogEx::debug);

			if (strcmp(m_sz_priv_sttn, "OFF") == 0)
			{
				// 制御権を取得していない場合
				m_i_priv_flag = 0;
			}
			else
			{
				// 制御権を取得している場合
				m_i_priv_flag = 1;
			}
		}
		else
		{
			// 制御権を取得していない場合
			m_i_priv_flag = 0;
		}
	}
#endif

	/*-- TX CONTROL ----------------------------------------------------------------------*/

	// RF POWER ON ENABLE 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_RF_ON_ENBL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_enbl, sizeof(m_sz_rf_on_enbl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("S-TXRF.RF_ON_ENABLE"), CString(m_sz_rf_on_enbl), nLogEx::debug);
	}

	// RF POWER OFF ENABLE 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_RF_OFF_ENBL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_off_enbl, sizeof(m_sz_rf_off_enbl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("S-TXRF.RF_OFF_ENABLE"), CString(m_sz_rf_off_enbl), nLogEx::debug);
	}

	// UP-LINK CONTROL 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_UPLINK_CONT));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("CTRL.S_TX_TOTAL_REMOTE"), CString(m_sz_uplink_cont), nLogEx::debug);
	}

	// X-TX RF POWER ON DETECT ANSWER 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_X_TX_DET_ANS));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_tx_det_ans, sizeof(m_sz_x_tx_det_ans), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("X-TX.RF_ON_ANS"), CString(m_sz_x_tx_det_ans), nLogEx::debug);
	}

	/*** リモート／ローカル 状態取得 ******************************************************/

	// S-TX リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_S_TX_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_s_tx_rl, sizeof(m_sz_s_tx_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("CTRL.S_TX_TOTAL_REMOTE"), CString(m_sz_s_tx_rl), nLogEx::debug);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_s_tx_rl, sizeof(m_sz_s_tx_rl), "REMOTE");
#endif


	// S-TXSYN リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_S_TXSYN_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_s_txsyn_rl, sizeof(m_sz_s_txsyn_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("S-UCSYN.CONT_MODE"), CString(m_sz_s_txsyn_rl), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	strcpy_s(m_sz_s_txsyn_rl, sizeof(m_sz_s_txsyn_rl), "REMOTE");
#endif

	/*** アラームチェック *****************************************************************/
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_ALARM));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_alarm, sizeof(m_sz_alarm), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("CTRL.ALARM_00_6"), CString(m_sz_alarm), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	strcpy_s(m_sz_alarm, sizeof(m_sz_alarm), "OFF");
#endif

	// アラーム時
	if (strcmp(m_sz_alarm, "ON") == 0) 
	{
		// 枠を赤くする
		m_grRfPower.SetBackColor(COLOR_ALARM);
		m_grNoUse.SetBackColor(COLOR_ALARM);
		m_grSweepContNormal.SetBackColor(COLOR_ALARM);
		m_grSweepContTriangle.SetBackColor(COLOR_ALARM);
		m_grMod.SetBackColor(COLOR_ALARM);
	}

	// アラーム解除
	else 
	{
		// 枠の色を元に戻す
		m_grRfPower.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grNoUse.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepContNormal.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepContTriangle.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grMod.SetBackColor(GetSysColor(COLOR_3DFACE));
	}


	/*** RF-POWER *************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_RF_POWER));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_power, sizeof(m_sz_rf_power), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("S-TXRF.IF_RF_OUTPUT_CONT"), CString(m_sz_rf_power), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	// RF-POWER ON カウントダウン中に監視データが ON になった場合
	if (m_l_rf_power_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rf_power_on_cntdwn = -1;

		m_DebugRrPower = 1;
	}

	// RF-POWER OFF カウントダウン中に監視データが OFF になった場合
	if (m_l_rf_power_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rf_power_off_cntdwn = -1;

		m_DebugRrPower = 2;
	}

	if (m_DebugRrPower == 1)
	{
		strcpy_s(m_sz_rf_power, sizeof(m_sz_rf_power), "ON");
	}
	else if (m_DebugRrPower == 2)
	{
		strcpy_s(m_sz_rf_power, sizeof(m_sz_rf_power), "OFF");
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
#else
	// RF-POWER ON カウントダウン中に監視データが ON になった場合
	if (m_l_rf_power_on_cntdwn >= 0 &&
		strcmp(m_sz_rf_power, "ON") == 0) 
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
	if (m_l_rf_power_off_cntdwn >= 0 &&
		strcmp(m_sz_rf_power, "OFF") == 0) 
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
#endif


	/*** NO-USE ***************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_NO_USE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use, sizeof(m_sz_no_use), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("S-TXRF.IF_NO_USE"), CString(m_sz_no_use), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	// NO-USE ON カウントダウン中に監視データが ON になった場合
	if (m_l_no_use_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_no_use_on_cntdwn = -1;

		m_DebugNoUse = 1;
	}

	// NO-USE OFF カウントダウン中にデータが OFF になった場合
	if (m_l_no_use_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_no_use_off_cntdwn = -1;

		m_DebugNoUse = 2;
	}

	if (m_DebugNoUse == 1)
	{
		strcpy_s(m_sz_no_use, sizeof(m_sz_no_use), "OFF");
	}
	else if (m_DebugNoUse == 2)
	{
		strcpy_s(m_sz_no_use, sizeof(m_sz_no_use), "ON");
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
#else
	// NO-USE ON カウントダウン中に監視データが ON になった場合
	if (m_l_no_use_on_cntdwn >= 0 &&
		strcmp(m_sz_no_use, "ON") == 0) 
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
	if (m_l_no_use_off_cntdwn >= 0 &&
		strcmp(m_sz_no_use, "ON") != 0) 
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
#endif


	/*** SWEEP-CONT ***********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_SWEEP_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_nor_sweep_cont, sizeof(m_sz_nor_sweep_cont), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("S-UCSYN.SWEEP_CONT"), CString(m_sz_nor_sweep_cont), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	// SWEEP-CONT STOP カウントダウン中に監視データが STOP になった場合
	if (m_l_nor_sweep_cont_stop_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_nor_sweep_cont_stop_cntdwn = -1;

		m_DebugSweepContN = 1;
	}

	// SWEEP-CONT INC カウントダウン中にデータが INC になった場合
	if (m_l_nor_sweep_cont_inc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_nor_sweep_cont_inc_cntdwn = -1;

		m_DebugSweepContN = 2;
	}

	// SWEEP-CONT INC+DEC カウントダウン中にデータが INC+DEC になった場合
	if (m_l_nor_sweep_cont_incdec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_nor_sweep_cont_incdec_cntdwn = -1;

		m_DebugSweepContN = 3;
	}

	// SWEEP-CONT DEC カウントダウン中にデータが DEC になった場合
	if (m_l_nor_sweep_cont_dec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_nor_sweep_cont_dec_cntdwn = -1;

		m_DebugSweepContN = 4;
	}

	// SWEEP-CONT DEC+INC カウントダウン中にデータが DEC になった場合
	if (m_l_nor_sweep_cont_decinc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_nor_sweep_cont_decinc_cntdwn = -1;

		m_DebugSweepContN = 5;
	}

	if (m_DebugSweepContN == 0)
	{
		strcpy_s(m_sz_nor_sweep_cont, sizeof(m_sz_nor_sweep_cont), "OFF");
	}
	if (m_DebugSweepContT == 0)
	{
		strcpy_s(m_sz_tri_sweep_cont, sizeof(m_sz_tri_sweep_cont), "OFF");
	}

	if (m_DebugSweepContN == 1)
	{
		strcpy_s(m_sz_nor_sweep_cont, sizeof(m_sz_nor_sweep_cont), "OFF");
	}
	else if (m_DebugSweepContN == 2)
	{
		strcpy_s(m_sz_nor_sweep_cont, sizeof(m_sz_nor_sweep_cont), "INC");
	}
	else if (m_DebugSweepContN == 3)
	{
		strcpy_s(m_sz_nor_sweep_cont, sizeof(m_sz_nor_sweep_cont), "INC+DEC");
	}
	else if (m_DebugSweepContN == 4)
	{
		strcpy_s(m_sz_nor_sweep_cont, sizeof(m_sz_nor_sweep_cont), "DEC");
	}
	else if (m_DebugSweepContN == 5)
	{
		strcpy_s(m_sz_nor_sweep_cont, sizeof(m_sz_nor_sweep_cont), "DEC+INC");
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_nor_sweep_cont_stop_cntdwn == -1 && m_l_nor_sweep_cont_inc_cntdwn == -1 &&
		m_l_nor_sweep_cont_incdec_cntdwn == -1 && m_l_nor_sweep_cont_dec_cntdwn == -1 &&
		m_l_nor_sweep_cont_decinc_cntdwn == -1)
	{
		// INC の場合
		if (strcmp(m_sz_nor_sweep_cont, "INC") == 0)
		{
			show_nor_sweep_cont_inc();
		}

		// INC+DEC の場合
		else if (strcmp(m_sz_nor_sweep_cont, "INC+DEC") == 0)
		{
			show_nor_sweep_cont_incdec();
		}
		// DEC の場合
		else if (strcmp(m_sz_nor_sweep_cont, "DEC") == 0)
		{
			show_nor_sweep_cont_dec();
		}
		// DEC+INC の場合
		else if (strcmp(m_sz_nor_sweep_cont, "DEC+INC") == 0)
		{
			show_nor_sweep_cont_decinc();
		}
		// STOP の場合
		else
		{
			show_nor_sweep_cont_stop();
		}
	}

#else

	// SWEEP-CONT STOP カウントダウン中に監視データが STOP になった場合
	if (m_l_nor_sweep_cont_stop_cntdwn >= 0 &&
		strcmp(m_sz_nor_sweep_cont, "STOP") == 0) 
	{
		// カウントダウン終了
		m_l_nor_sweep_cont_stop_cntdwn = -1;
	}
	// SWEEP-CONT STOP カウントダウンタイムリミットになった場合
	else if (m_l_nor_sweep_cont_stop_cntdwn == 0) 
	{
		// カウントダウン終了
		m_l_nor_sweep_cont_stop_cntdwn = -1;
	}

	// SWEEP-CONT INC カウントダウン中にデータが INC になった場合
	if (m_l_nor_sweep_cont_inc_cntdwn >= 0 &&
		strcmp(m_sz_nor_sweep_cont, "INC") == 0) 
	{
		// カウントダウン終了
		m_l_nor_sweep_cont_inc_cntdwn = -1;
	}
	// SWEEP-CONT INC カウントダウンタイムリミットになった場合
	else if (m_l_nor_sweep_cont_inc_cntdwn == 0) 
	{
		// カウントダウン終了
		m_l_nor_sweep_cont_inc_cntdwn = -1;
	}

	// SWEEP-CONT INC+DEC カウントダウン中にデータが INC+DEC になった場合
	if (m_l_nor_sweep_cont_incdec_cntdwn >= 0 &&
		strcmp(m_sz_nor_sweep_cont, "INC+DEC") == 0) 
	{
		// カウントダウン終了
		m_l_nor_sweep_cont_incdec_cntdwn = -1;
	}
	// SWEEP-CONT INC+DEC カウントダウンタイムリミットになった場合
	else if (m_l_nor_sweep_cont_incdec_cntdwn == 0) 
	{
		// カウントダウン終了
		m_l_nor_sweep_cont_incdec_cntdwn = -1;
	}

	// SWEEP-CONT DEC カウントダウン中にデータが DEC になった場合
	if (m_l_nor_sweep_cont_dec_cntdwn >= 0 &&
		strcmp(m_sz_nor_sweep_cont, "DEC") == 0) 
	{
		// カウントダウン終了
		m_l_nor_sweep_cont_dec_cntdwn = -1;
	}
	// SWEEP-CONT DEC カウントダウンタイムリミットになった場合
	else if (m_l_nor_sweep_cont_dec_cntdwn == 0) 
	{
		// カウントダウン終了
		m_l_nor_sweep_cont_dec_cntdwn = -1;
	}

	// SWEEP-CONT DEC+INC カウントダウン中にデータが DEC になった場合
	if (m_l_nor_sweep_cont_decinc_cntdwn >= 0 &&
		strcmp(m_sz_nor_sweep_cont, "DEC+INC") == 0) 
	{
		// カウントダウン終了
		m_l_nor_sweep_cont_decinc_cntdwn = -1;
	}
	// SWEEP-CONT DEC+INC カウントダウンタイムリミットになった場合
	else if (m_l_nor_sweep_cont_decinc_cntdwn == 0) 
	{
		// カウントダウン終了
		m_l_nor_sweep_cont_decinc_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_nor_sweep_cont_stop_cntdwn == -1 && m_l_nor_sweep_cont_inc_cntdwn == -1 &&
		m_l_nor_sweep_cont_incdec_cntdwn == -1 && m_l_nor_sweep_cont_dec_cntdwn == -1 &&
		m_l_nor_sweep_cont_decinc_cntdwn == -1) 
	{
		// INC の場合
		if (strcmp(m_sz_nor_sweep_cont, "INC") == 0) 
		{
			show_nor_sweep_cont_inc();
		}

		// INC+DEC の場合
		else if (strcmp(m_sz_nor_sweep_cont, "INC+DEC") == 0) 
		{
			show_nor_sweep_cont_incdec();
		}
		// DEC の場合
		else if (strcmp(m_sz_nor_sweep_cont, "DEC") == 0) 
		{
			show_nor_sweep_cont_dec();
		}
		// DEC+INC の場合
		else if (strcmp(m_sz_nor_sweep_cont, "DEC+INC") == 0) 
		{
			show_nor_sweep_cont_decinc();
		}
		// STOP の場合
		else 
		{
			show_nor_sweep_cont_stop();
		}
	}
#endif


	/*** Tri-SWEEP-CONT *******************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_TR_SWEEP_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_tri_sweep_cont, sizeof(m_sz_tri_sweep_cont), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("S-UCSYN.TR_SWEEP_CONT"), CString(m_sz_tri_sweep_cont), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	// Tri-SWEEP-CONT STOP カウントダウン中に監視データが STOP になった場合
	if (m_l_tri_sweep_cont_stop_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_tri_sweep_cont_stop_cntdwn = -1;

		// 制御が終了したので、三角スイープボタン有効/無効チェックボックスを解除する。
		m_SweepContTriChkBtn.SetCheck(0);

		// 制御が終了したので、三角スイープボタンを無効にする。
		OnBnClickedTrsctrlSweeptCheck();

		// 三角スイープ制御完了フラグクリア
		m_l_tri_sweep_flag = 0;

		m_DebugSweepContT = 1;
	}

	// Tri-SWEEP-CONT INC カウントダウン中にデータが INC になった場合
	if (m_l_tri_sweep_cont_inc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_tri_sweep_cont_inc_cntdwn = -1;

		// 三角スイープ制御完了フラグセット
		m_l_tri_sweep_flag = 1;

		m_DebugSweepContT = 2;
	}

	// Tri-SWEEP-CONT INC+DEC カウントダウン中にデータが INC+DEC になった場合
	if (m_l_tri_sweep_cont_incdec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_tri_sweep_cont_incdec_cntdwn = -1;

		// 三角スイープ制御完了フラグセット
		m_l_tri_sweep_flag = 1;

		m_DebugSweepContT = 3;
	}

	// Tri-SWEEP-CONT DEC カウントダウン中にデータが DEC になった場合
	if (m_l_tri_sweep_cont_dec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_tri_sweep_cont_dec_cntdwn = -1;

		// 三角スイープ制御完了フラグセット
		m_l_tri_sweep_flag = 1;

		m_DebugSweepContT = 4;
	}

	// Tri-SWEEP-CONT DEC+INC カウントダウン中にデータが DEC になった場合
	if (m_l_tri_sweep_cont_decinc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_tri_sweep_cont_decinc_cntdwn = -1;

		// 三角スイープ制御完了フラグセット
		m_l_tri_sweep_flag = 1;

		m_DebugSweepContT = 5;
	}

	if (m_DebugSweepContT == 1)
	{
		strcpy_s(m_sz_tri_sweep_cont, sizeof(m_sz_tri_sweep_cont), "OFF");
	}
	else if (m_DebugSweepContT == 2)
	{
		strcpy_s(m_sz_tri_sweep_cont, sizeof(m_sz_tri_sweep_cont), "INC");
	}
	else if (m_DebugSweepContT == 3)
	{
		strcpy_s(m_sz_tri_sweep_cont, sizeof(m_sz_tri_sweep_cont), "INC+DEC");
	}
	else if (m_DebugSweepContT == 4)
	{
		strcpy_s(m_sz_tri_sweep_cont, sizeof(m_sz_tri_sweep_cont), "DEC");
	}
	else if (m_DebugSweepContT == 5)
	{
		strcpy_s(m_sz_tri_sweep_cont, sizeof(m_sz_tri_sweep_cont), "DEC+INC");
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_tri_sweep_cont_stop_cntdwn == -1 && m_l_tri_sweep_cont_inc_cntdwn == -1 &&
		m_l_tri_sweep_cont_incdec_cntdwn == -1 && m_l_tri_sweep_cont_dec_cntdwn == -1 &&
		m_l_tri_sweep_cont_decinc_cntdwn == -1) 
	{
		// INC の場合
		if (strcmp(m_sz_tri_sweep_cont, "INC") == 0) 
		{
			show_tri_sweep_cont_inc();
		}

		// INC+DEC の場合
		else if (strcmp(m_sz_tri_sweep_cont, "INC+DEC") == 0) 
		{
			show_tri_sweep_cont_incdec();
		}
		// DEC の場合
		else if (strcmp(m_sz_tri_sweep_cont, "DEC") == 0) 
		{
			show_tri_sweep_cont_dec();
		}
		// DEC+INC の場合
		else if (strcmp(m_sz_tri_sweep_cont, "DEC+INC") == 0) 
		{
			show_tri_sweep_cont_decinc();
		}
		// STOP の場合
		else {
			show_tri_sweep_cont_stop();

			// 制御後にローカル側でSTOPに変わった場合は、ボタンを無効にする
			if (m_l_tri_sweep_flag == 1) 
			{
				// 制御が終了したので、三角スイープボタン有効/無効チェックボックスを解除する
				m_SweepContTriChkBtn.SetCheck(0);

				// 制御が終了したので、三角スイープボタンを無効にする
				OnBnClickedTrsctrlSweeptCheck();

				// 三角スイープ制御完了フラグクリア
				m_l_tri_sweep_flag = 0;
			}
		}
	}
#else
	// Tri-SWEEP-CONT STOP カウントダウン中に監視データが STOP になった場合
	if (m_l_tri_sweep_cont_stop_cntdwn >= 0 &&
		strcmp(m_sz_tri_sweep_cont, "STOP") == 0) 
	{
		// カウントダウン終了
		m_l_tri_sweep_cont_stop_cntdwn = -1;

		// 制御が終了したので、三角スイープボタン有効/無効チェックボックスを解除する。
//		XmToggleButtonSetState(SweepContTri_ChkB, False, False);
		m_SweepContTriChkBtn.SetCheck(0);

		// 制御が終了したので、三角スイープボタンを無効にする。
//		TriSweepDis_CB_Click((Widget)NULL, (XtPointer)NULL, (XmPushButtonCallbackStruct *)NULL);
		m_SweepContTriChkBtn.EnableWindow(FALSE);

		// 三角スイープ制御完了フラグクリア
		m_l_tri_sweep_flag = 0;
	}
	// Tri-SWEEP-CONT STOP カウントダウンタイムリミットになった場合
	else if (m_l_tri_sweep_cont_stop_cntdwn == 0) 
	{
		// カウントダウン終了
		m_l_tri_sweep_cont_stop_cntdwn = -1;
	}

	// Tri-SWEEP-CONT INC カウントダウン中にデータが INC になった場合
	if (m_l_tri_sweep_cont_inc_cntdwn >= 0 &&
		strcmp(m_sz_tri_sweep_cont, "INC") == 0) 
	{
		// カウントダウン終了
		m_l_tri_sweep_cont_inc_cntdwn = -1;

		// 三角スイープ制御完了フラグセット
		m_l_tri_sweep_flag = 1;
	}
	// Tri-SWEEP-CONT INC カウントダウンタイムリミットになった場合
	else if (m_l_tri_sweep_cont_inc_cntdwn == 0) 
	{
		// カウントダウン終了
		m_l_tri_sweep_cont_inc_cntdwn = -1;
	}

	// Tri-SWEEP-CONT INC+DEC カウントダウン中にデータが INC+DEC になった場合
	if (m_l_tri_sweep_cont_incdec_cntdwn >= 0 &&
		strcmp(m_sz_tri_sweep_cont, "INC+DEC") == 0) 
	{
		// カウントダウン終了
		m_l_tri_sweep_cont_incdec_cntdwn = -1;

		// 三角スイープ制御完了フラグセット
		m_l_tri_sweep_flag = 1;
	}
	// Tri-SWEEP-CONT INC+DEC カウントダウンタイムリミットになった場合
	else if (m_l_tri_sweep_cont_incdec_cntdwn == 0) 
	{
		// カウントダウン終了
		m_l_tri_sweep_cont_incdec_cntdwn = -1;
	}

	// Tri-SWEEP-CONT DEC カウントダウン中にデータが DEC になった場合
	if (m_l_tri_sweep_cont_dec_cntdwn >= 0 &&
		strcmp(m_sz_tri_sweep_cont, "DEC") == 0) 
	{
		// カウントダウン終了
		m_l_tri_sweep_cont_dec_cntdwn = -1;

		// 三角スイープ制御完了フラグセット
		m_l_tri_sweep_flag = 1;
	}
	// Tri-SWEEP-CONT DEC カウントダウンタイムリミットになった場合
	else if (m_l_tri_sweep_cont_dec_cntdwn == 0) 
	{
		// カウントダウン終了
		m_l_tri_sweep_cont_dec_cntdwn = -1;
	}

	// Tri-SWEEP-CONT DEC+INC カウントダウン中にデータが DEC になった場合
	if (m_l_tri_sweep_cont_decinc_cntdwn >= 0 &&
		strcmp(m_sz_tri_sweep_cont, "DEC+INC") == 0) 
	{
		// カウントダウン終了
		m_l_tri_sweep_cont_decinc_cntdwn = -1;

		// 三角スイープ制御完了フラグセット
		m_l_tri_sweep_flag = 1;
	}
	// Tri-SWEEP-CONT DEC+INC カウントダウンタイムリミットになった場合
	else if (m_l_tri_sweep_cont_decinc_cntdwn == 0) 
	{
		// カウントダウン終了
		m_l_tri_sweep_cont_decinc_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_tri_sweep_cont_stop_cntdwn == -1 && m_l_tri_sweep_cont_inc_cntdwn == -1 &&
		m_l_tri_sweep_cont_incdec_cntdwn == -1 && m_l_tri_sweep_cont_dec_cntdwn == -1 &&
		m_l_tri_sweep_cont_decinc_cntdwn == -1) 
	{
		// INC の場合
		if (strcmp(m_sz_tri_sweep_cont, "INC") == 0) 
		{
			show_tri_sweep_cont_inc();
		}

		// INC+DEC の場合
		else if (strcmp(m_sz_tri_sweep_cont, "INC+DEC") == 0) 
		{
			show_tri_sweep_cont_incdec();
		}
		// DEC の場合
		else if (strcmp(m_sz_tri_sweep_cont, "DEC") == 0) 
		{
			show_tri_sweep_cont_dec();
		}
		// DEC+INC の場合
		else if (strcmp(m_sz_tri_sweep_cont, "DEC+INC") == 0) 
		{
			show_tri_sweep_cont_decinc();
		}
		// STOP の場合
		else {
			show_tri_sweep_cont_stop();

			// 制御後にローカル側でSTOPに変わった場合は、ボタンを無効にする
			if (m_l_tri_sweep_flag == 1) 
			{
				// 制御が終了したので、三角スイープボタン有効/無効チェックボックスを解除する
//				XmToggleButtonSetState(SweepContTri_ChkB, False, False);
				m_SweepContTriChkBtn.SetCheck(0);

				// 制御が終了したので、三角スイープボタンを無効にする
//				TriSweepDis_CB_Click((Widget)NULL, (XtPointer)NULL, (XmPushButtonCallbackStruct *)NULL);
				m_SweepContTriChkBtn.EnableWindow(FALSE);

				// 三角スイープ制御完了フラグクリア
				m_l_tri_sweep_flag = 0;
			}
		}
	}
#endif


	/*** S-TXSYN Tri-SWEEP NUM ************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_TR_SWEEP_NUM));
	if (obs_adr != NULL)
	{
		m_d_tri_sweep_num = obs_adr->d_data;
	}

	// 数値表示部を更新
	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_tri_sweep_num % 100);
	m_SweepNumBtn.SetWindowTextW(CString(sz_buff));

	// Tri-SWEEP-NUM カウントダウン中にデータが設定値になった場合
	if (m_l_tri_sweep_num_cntdwn >= 0 &&
		m_l_tri_sweep_num_set == (long)m_d_tri_sweep_num) 
	{
		// カウントダウン終了
		m_l_tri_sweep_num_cntdwn = -1;
	}
	// Tri-SWEEP-NUM カウントダウンタイムリミットになった場合
	else if (m_l_tri_sweep_num_cntdwn == 0) 
	{
		// カウントダウン終了
		m_l_tri_sweep_num_cntdwn = -1;

		sprintf_s(sz_buff, sizeof(sz_buff), "SWEEP-NUM %2d", m_l_tri_sweep_num_set);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), CString(sz_buff), _T(""), nLogEx::debug);
	}


	/*** CMD-MOD **************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_CMD_MOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_cmd_mod, sizeof(m_sz_cmd_mod), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("SS-TCRMOD.MODULATION"), CString(m_sz_cmd_mod), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	// CMD-MOD OFF カウントダウン中にデータが OFF になった場合
	if (m_l_cmd_mod_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_cmd_mod_off_cntdwn = -1;

		m_DebugCmdMod = 1;
	}

	// CMD-MOD ON カウントダウン中にデータが ON になった場合
	if (m_l_cmd_mod_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_cmd_mod_on_cntdwn = -1;

		m_DebugCmdMod = 2;
	}

	if (m_DebugCmdMod == 1)
	{
		strcpy_s(m_sz_cmd_mod, sizeof(m_sz_cmd_mod), "OFF");
	}
	else if (m_DebugCmdMod == 2)
	{
		strcpy_s(m_sz_cmd_mod, sizeof(m_sz_cmd_mod), "ON");
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
#else
	// CMD-MOD OFF カウントダウン中にデータが OFF になった場合
	if (m_l_cmd_mod_off_cntdwn >= 0 &&
		strcmp(m_sz_cmd_mod, "OFF") == 0) 
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
	if (m_l_cmd_mod_on_cntdwn >= 0 &&
		strcmp(m_sz_cmd_mod, "ON") == 0) 
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
#endif


	/*** CMD MOD INDEX ********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_CMD_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_cmd_mod_idx = obs_adr->d_data;

		// 数値表示部を更新
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_cmd_mod_idx);
		m_strIndexCmd = CString(sz_buff);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("SS-TCRMOD.CMD_MOD_INDEX"), m_strIndexCmd, nLogEx::debug);
	}

	/*** RNG MOD INDEX ********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_RNG_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_rng_mod_idx = obs_adr->d_data;

		// 数値表示部を更新
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_rng_mod_idx);
		m_strIndexRng = CString(sz_buff);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("SS-TCRMOD.RNG_MOD_INDEX"), m_strIndexRng, nLogEx::debug);
	}

	/*** DATA SOURCE **********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_DATA_SOURCE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_data_src, sizeof(m_sz_data_src), obs_adr->sz_sts_name);

		// 状態表示部を更新
		m_strIndexCR = CString(m_sz_data_src);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("SS-TCRMOD.DATA_SOURCE"), m_strIndexCR, nLogEx::debug);
	}

	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	/*** SS-RNG ***************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_SS_RNG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_ss_rng, sizeof(m_sz_ss_rng), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("V-EQU.RNG_SS"), CString(m_sz_ss_rng), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	// SS-RNG OFF カウントダウン中にデータが OFF になった場合
	if (m_l_ss_rng_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_ss_rng_off_cntdwn = -1;

		m_DebugSSRng = 1;
	}

	// SS-RNG ON カウントダウン中にデータが ON になった場合
	if (m_l_ss_rng_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_ss_rng_on_cntdwn = -1;

		m_DebugSSRng = 2;
	}

	if (m_DebugSSRng == 1)
	{
		strcpy_s(m_sz_ss_rng, sizeof(m_sz_ss_rng), "OFF");
	}
	else if (m_DebugSSRng == 2)
	{
		strcpy_s(m_sz_ss_rng, sizeof(m_sz_ss_rng), "ON");
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_ss_rng_off_cntdwn == -1 && m_l_ss_rng_on_cntdwn == -1) 
	{
		// ON の場合
		if (strcmp(m_sz_ss_rng, "ON") == 0) 
		{
			show_ss_rng_on();
		}
		// OFF の場合
		else 
		{
			show_ss_rng_off();
		}
	}

#else
	// SS-RNG OFF カウントダウン中にデータが OFF になった場合
	if (m_l_ss_rng_off_cntdwn >= 0 &&
		strcmp(m_sz_ss_rng, "OFF") == 0) 
	{
		// カウントダウン終了
		m_l_ss_rng_off_cntdwn = -1;
	}
	// SS-RNG OFF カウントダウンタイムリミットになった場合
	else if (m_l_ss_rng_off_cntdwn == 0) 
	{
		// カウントダウン終了
		m_l_ss_rng_off_cntdwn = -1;
	}

	// SS-RNG ON カウントダウン中にデータが ON になった場合
	if (m_l_ss_rng_on_cntdwn >= 0 &&
		strcmp(m_sz_ss_rng, "ON") == 0) 
	{
		// カウントダウン終了
		m_l_ss_rng_on_cntdwn = -1;
	}
	// SS-RNG ON カウントダウンタイムリミットになった場合
	else if (m_l_ss_rng_on_cntdwn == 0) 
	{
		// カウントダウン終了
		m_l_ss_rng_on_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_ss_rng_off_cntdwn == -1 && m_l_ss_rng_on_cntdwn == -1) 
	{
		// ON の場合
		if (strcmp(m_sz_ss_rng, "ON") == 0) 
		{
			show_ss_rng_on();
		}
		// OFF の場合
		else 
		{
			show_ss_rng_off();
		}
	}
#endif


	/*** COH/INCOH ************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_COH_INCOH));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_coh_incoh, sizeof(m_sz_coh_incoh), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("V-EQU.COHI"), CString(m_sz_coh_incoh), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	// COH/INCOH COH カウントダウン中にデータが COH になった場合
	if (m_l_coh_incoh_coh_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_coh_incoh_coh_cntdwn = -1;

		m_DebugCohIncoh = 1;
	}

	// COH/INCOH INCOH カウントダウン中にデータが INCOH になった場合
	if (m_l_coh_incoh_incoh_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_coh_incoh_incoh_cntdwn = -1;

		m_DebugCohIncoh = 2;
	}

	if (m_DebugCohIncoh == 1)
	{
		strcpy_s(m_sz_coh_incoh, sizeof(m_sz_coh_incoh), "COH");
	}
	else if (m_DebugCohIncoh == 2)
	{
		strcpy_s(m_sz_coh_incoh, sizeof(m_sz_coh_incoh), "INCOH");
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_coh_incoh_coh_cntdwn == -1 && m_l_coh_incoh_incoh_cntdwn == -1) 
	{
		// INCOH の場合
		if (strcmp(m_sz_coh_incoh, "INCOH") == 0) 
		{
			show_coh_incoh_incoh();
		}
		// COH の場合
		else 
		{
			show_coh_incoh_coh();
		}
	}
#else
	// COH/INCOH COH カウントダウン中にデータが COH になった場合
	if (m_l_coh_incoh_coh_cntdwn >= 0 &&
		strcmp(m_sz_coh_incoh, "COH") == 0) 
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
	if (m_l_coh_incoh_incoh_cntdwn >= 0 &&
		strcmp(m_sz_coh_incoh, "INCOH") == 0) 
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
		if (strcmp(m_sz_coh_incoh, "INCOH") == 0) 
		{
			show_coh_incoh_incoh();
		}
		// COH の場合
		else 
		{
			show_coh_incoh_coh();
		}
	}
#endif


	/*** RNGING ***************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("CTRL.RNG_FLG"), CString(m_sz_rnging), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	// RNGING STOP カウントダウン中にデータが STOP になった場合
	if (m_l_rnging_stop_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rnging_stop_cntdwn = -1;

		m_DebugRnging = 1;
	}

	// RNGING START カウントダウン中にデータが START になった場合
	if (m_l_rnging_start_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rnging_start_cntdwn = -1;

		m_DebugRnging = 2;
	}

	if (m_DebugRnging == 1)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), "停止");
	}
	else if (m_DebugRnging == 2)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), "開始");
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
#else
	// RNGING STOP カウントダウン中にデータが STOP になった場合
	if (m_l_rnging_stop_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "終了") == 0) 
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
	if (m_l_rnging_start_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "開始") == 0) 
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
#endif


	/*** HOLD TONE 2006-12-12 追加 ********************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_HOLD_TONE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hold_tone, sizeof(m_sz_hold_tone), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("CMDOUT.HOLDTONE_STAT"), CString(m_sz_hold_tone), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	// HOLD TONE ON カウントダウン中に監視データが ON になった場合
	if (m_l_holdtone_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_holdtone_on_cntdwn = -1;

		m_DebugHoldTone = 1;
	}

	// HOLD TONE OFF カウントダウン中に監視データが OFF になった場合
	if (m_l_holdtone_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_holdtone_off_cntdwn = -1;

		m_DebugHoldTone = 2;
	}

	if (m_DebugHoldTone == 1)
	{
		strcpy_s(m_sz_hold_tone, sizeof(m_sz_hold_tone), "ON");
	}
	else if (m_DebugHoldTone == 2)
	{
		strcpy_s(m_sz_hold_tone, sizeof(m_sz_hold_tone), "OFF");
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
#else
	// HOLD TONE ON カウントダウン中に監視データが ON になった場合
	if (m_l_holdtone_on_cntdwn >= 0 &&
		strcmp(m_sz_hold_tone, "ON") == 0) 
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
	if (m_l_holdtone_off_cntdwn >= 0 &&
		strcmp(m_sz_hold_tone, "OFF") == 0) 
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
#endif

	UpdateData(FALSE);

	SetTimer(TID_CONDITION, INTERVAL_CONDITION, 0);
}

/*============================================================================*/
/*! CTrsControlS64

-# カウントダウンタイムアウト

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::count_down()
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
	if (m_l_nor_sweep_cont_stop_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT STOP カウントダウン:%d\n"), m_l_nor_sweep_cont_stop_cntdwn);
		m_l_nor_sweep_cont_stop_cntdwn--;
	}

	// SWEEP-CONT NOMAL INC
	if (m_l_nor_sweep_cont_inc_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT INC カウントダウン:%d\n"), m_l_nor_sweep_cont_inc_cntdwn);
		m_l_nor_sweep_cont_inc_cntdwn--;
	}

	// SWEEP-CONT NOMAL INC+DEC
	if (m_l_nor_sweep_cont_incdec_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT INC+DEC カウントダウン:%d\n"), m_l_nor_sweep_cont_incdec_cntdwn);
		m_l_nor_sweep_cont_incdec_cntdwn--;
	}

	// SWEEP-CONT NOMAL DEC
	if (m_l_nor_sweep_cont_dec_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT DEC カウントダウン:%d\n"), m_l_nor_sweep_cont_dec_cntdwn);
		m_l_nor_sweep_cont_dec_cntdwn--;
	}

	// SWEEP-CONT NOMAL DEC+INC
	if (m_l_nor_sweep_cont_decinc_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT DEC+INC カウントダウン:%d\n"), m_l_nor_sweep_cont_decinc_cntdwn);
		m_l_nor_sweep_cont_decinc_cntdwn--;
	}

	// SWEEP-CONT TRIANGLE STOP
	if (m_l_tri_sweep_cont_stop_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT STOP カウントダウン:%d\n"), m_l_tri_sweep_cont_stop_cntdwn);
		m_l_tri_sweep_cont_stop_cntdwn--;
	}

	// SWEEP-CONT TRIANGLE INC
	if (m_l_tri_sweep_cont_inc_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT INC カウントダウン:%d\n"), m_l_tri_sweep_cont_inc_cntdwn);
		m_l_tri_sweep_cont_inc_cntdwn--;
	}

	// SWEEP-CONT TRIANGLE INC+DEC
	if (m_l_tri_sweep_cont_incdec_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT INC+DEC カウントダウン:%d\n"), m_l_tri_sweep_cont_incdec_cntdwn);
		m_l_tri_sweep_cont_incdec_cntdwn--;
	}

	// SWEEP-CONT TRIANGLE DEC
	if (m_l_tri_sweep_cont_dec_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT DEC カウントダウン:%d\n"), m_l_tri_sweep_cont_dec_cntdwn);
		m_l_tri_sweep_cont_dec_cntdwn--;
	}

	// SWEEP-CONT TRIANGLE DEC+INC
	if (m_l_tri_sweep_cont_decinc_cntdwn > 0) {
		TRACE(_T("SWEEP-CONT DEC+INC カウントダウン:%d\n"), m_l_tri_sweep_cont_decinc_cntdwn);
		m_l_tri_sweep_cont_decinc_cntdwn--;
	}

	// SWEEP-NUM
	if (m_l_sweep_num_cntdwn > 0)
	{
		TRACE(_T("SWEEP-NUM カウントダウン:%d\n"), m_l_sweep_num_cntdwn);
		m_l_sweep_num_cntdwn--;
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
/*! CTrsControlS64

-# RF-POWER OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_rf_power_off()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_RF_ON_ENBL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_enbl, sizeof(m_sz_rf_on_enbl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_X_TX_DET_ANS));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_tx_det_ans, sizeof(m_sz_x_tx_det_ans), obs_adr->sz_sts_name);
	}

#if 1
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}
#else
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}
#endif

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "接続");
	strcpy_s(m_sz_rf_on_enbl, sizeof(m_sz_rf_on_enbl), "ON");
#endif

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
#if 1
		if (strcmp(m_sz_lan_x_ul_cnt, "接続") == 0)
		{
#else
		if (strcmp(m_sz_lan_x_tx, "接続") == 0) 
		{
#endif

			// X-TX RF DET ANS を確認する
			if (strcmp(m_sz_x_tx_det_ans, "OFF") == 0)
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
/*! CTrsControlS64

-# RF-POWER ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_rf_power_on()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_RF_ON_ENBL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_enbl, sizeof(m_sz_rf_on_enbl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "接続");
	strcpy_s(m_sz_rf_on_enbl, sizeof(m_sz_rf_on_enbl), "OFF");
#endif

	if (strcmp(m_sz_lan_s_tx, "接続") != 0)
	{
		// ローカル表示
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);		// ON ボタン
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
/*! CTrsControlS64

-# NO-USE OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_no_use_off()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "接続");
#endif

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
	if ((strcmp(m_sz_lan_s_tx, "接続") == 0) &&
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
/*! CTrsControlS64

-# NO-USE ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_no_use_on()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "接続");
#endif

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
	if ((strcmp(m_sz_lan_s_tx, "接続") == 0) &&
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
/*! CTrsControlS64

-# SWEEP-CONT NORMAL STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_nor_sweep_cont_stop()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "接続");
#endif

	if (strcmp(m_sz_lan_s_txsyn, "接続") != 0)
	{
		// ローカル表示
		m_SweepContNStopBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）表示
		m_SweepContNStopBtn.SetColor(COLOR_REMOTES);
	}

	// リモート時
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "接続") == 0) &&
		(strcmp(m_sz_tri_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1)) 
	{
		// リモート（非選択）表示
		m_SweepContNIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContNIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContNDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContNDecIncBtn.SetColor(COLOR_REMOTE);
	}
	// ローカル時
	else 
	{
		// ローカル表示
		m_SweepContNIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContNIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContNDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContNDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT NORMAL INC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_nor_sweep_cont_inc()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "接続");
#endif

	if (strcmp(m_sz_lan_s_txsyn, "接続") != 0)
	{
		// ローカル表示
		m_SweepContNIncBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）表示
		m_SweepContNIncBtn.SetColor(COLOR_REMOTES);
	}

	// リモート時
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "接続") == 0) &&
		(strcmp(m_sz_tri_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）表示
		m_SweepContNStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContNIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContNDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContNDecIncBtn.SetColor(COLOR_REMOTE);
	}
	// ローカル時
	else 
	{
		// ローカル表示
		m_SweepContNStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContNIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContNDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContNDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT NORMAL INCDEC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_nor_sweep_cont_incdec()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "接続");
#endif

	if (strcmp(m_sz_lan_s_txsyn, "接続") != 0)
	{
		// ローカル表示
		m_SweepContNIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）表示
		m_SweepContNIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// リモート時
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "接続") == 0) &&
		(strcmp(m_sz_tri_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）表示
		m_SweepContNStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContNIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContNDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContNDecIncBtn.SetColor(COLOR_REMOTE);
	}
	// ローカル時
	else 
	{
		// ローカル表示
		m_SweepContNStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContNIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContNDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContNDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT NORMAL DEC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_nor_sweep_cont_dec()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "接続");
#endif

	if (strcmp(m_sz_lan_s_txsyn, "接続") != 0)
	{
		// ローカル表示
		m_SweepContNDecBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）表示
		m_SweepContNDecBtn.SetColor(COLOR_REMOTES);
	}

	// リモート時
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "接続") == 0) &&
		(strcmp(m_sz_tri_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1)) 
	{
		// リモート（非選択）表示
		m_SweepContNStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContNIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContNIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContNDecIncBtn.SetColor(COLOR_REMOTE);
	}
	// ローカル時
	else 
	{
		// ローカル表示
		m_SweepContNStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContNIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContNIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContNDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT NORMAL DECINC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_nor_sweep_cont_decinc()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "接続");
#endif

	if (strcmp(m_sz_lan_s_txsyn, "接続") != 0)
	{
		// ローカル表示
		m_SweepContNDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）表示
		m_SweepContNDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// リモート時
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "接続") == 0) &&
		(strcmp(m_sz_tri_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1)) 
	{
		// リモート（非選択）表示
		m_SweepContNStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContNIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContNIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContNDecBtn.SetColor(COLOR_REMOTE);
	}
	// ローカル時
	else 
	{
		// ローカル表示
		m_SweepContNStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContNIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContNIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContNDecBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}



/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT TRIANGLE STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_tri_sweep_cont_stop()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "接続");

	if (strcmp(m_sz_lan_s_txsyn, "接続") != 0) 
	{
		// ローカル
		m_SweepContTStopBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_SweepContTStopBtn.SetColor(COLOR_REMOTES);
	}

	// その他
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "接続") == 0) &&
		(strcmp(m_sz_nor_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_SweepContTIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContTIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContTDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContTDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_SweepContTIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContTIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContTDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContTDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT TRIANGLE INC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_tri_sweep_cont_inc()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "接続");

	if (strcmp(m_sz_lan_s_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepContTIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepContTIncBtn.SetColor(COLOR_REMOTES);
	}

	// その他
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "接続") == 0) &&
		(strcmp(m_sz_nor_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepContTStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContTIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContTDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContTDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepContTStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContTIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContTDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContTDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT TRIANGLE INCDEC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_tri_sweep_cont_incdec()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "接続");

	if (strcmp(m_sz_lan_s_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepContTIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepContTIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// その他
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "接続") == 0) &&
		(strcmp(m_sz_nor_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepContTStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContTIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContTDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContTDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepContTStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContTIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContTDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContTDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT TRIANGLE DEC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_tri_sweep_cont_dec()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "接続");

	if (strcmp(m_sz_lan_s_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepContTDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepContTDecBtn.SetColor(COLOR_REMOTES);
	}

	// その他
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "接続") == 0) &&
		(strcmp(m_sz_nor_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepContTStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContTIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContTIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContTDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepContTStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContTIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContTIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContTDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT TRIANGLE DECINC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_tri_sweep_cont_decinc()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "接続");

	if (strcmp(m_sz_lan_s_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepContTDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepContTDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// その他
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "接続") == 0) &&
		(strcmp(m_sz_nor_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepContTStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContTIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContTIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContTDecBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepContTStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContTIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContTIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContTDecBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# CMD-MOD OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_cmd_mod_off()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_UPLINK_CONT));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "接続");
#ifdef _CUTNOCOMM
	strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), "REMOTE");
#endif

	if (strcmp(m_sz_lan_s_tx, "接続") != 0)
	{
		// ローカル表示
		m_ModOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）表示
		m_ModOffBtn.SetColor(COLOR_REMOTES);
	}

	// リモート時
	if ((strcmp(m_sz_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）表示
		m_ModOnBtn.SetColor(COLOR_REMOTE);
	}
	// ローカル時
	else 
	{
		// ローカル表示
		m_ModOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_ModOffBtn.Invalidate();
	m_ModOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# CMD-MOD ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_cmd_mod_on()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_UPLINK_CONT));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "接続");
#ifdef _CUTNOCOMM
	strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), "REMOTE");
#endif

	if (strcmp(m_sz_lan_s_tx, "接続") != 0)
	{
		// ローカル表示
		m_ModOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）表示
		m_ModOnBtn.SetColor(COLOR_REMOTES);
	}

	// リモート時
	if ((strcmp(m_sz_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）表示
		m_ModOffBtn.SetColor(COLOR_REMOTE);
	}
	// ローカル時
	else 
	{
		// ローカル表示
		m_ModOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_ModOffBtn.Invalidate();
	m_ModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# SS-RNG OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_ss_rng_off()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), "接続");
#endif

	if (strcmp(m_sz_lan_vequ, "接続") != 0)
	{
		// ローカル表示
		m_SSRngOffBtn.SetColor(COLOR_LOCAL);
		m_SSRngOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		if (m_i_priv_flag == 1)
		{
			// リモート（非選択）表示
			m_SSRngOnBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ローカル表示
			m_SSRngOnBtn.SetColor(COLOR_LOCAL);
		}
		// リモート（選択）表示
		m_SSRngOffBtn.SetColor(COLOR_REMOTES);
	}

	// 更新
	m_SSRngOffBtn.Invalidate();
	m_SSRngOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SS-RNG ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_ss_rng_on()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), "接続");
#endif

	if (strcmp(m_sz_lan_vequ, "接続") != 0)
	{
		// ローカル表示
		m_SSRngOnBtn.SetColor(COLOR_LOCAL);
		m_SSRngOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		if (m_i_priv_flag == 1)
		{
			// リモート（非選択）表示
			m_SSRngOffBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ローカル表示
			m_SSRngOffBtn.SetColor(COLOR_LOCAL);
		}
		// リモート（選択）表示
		m_SSRngOnBtn.SetColor(COLOR_REMOTES);
	}

	// 更新
	m_SSRngOnBtn.Invalidate();
	m_SSRngOffBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# COH/INCOH COH 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_coh_incoh_coh()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), "接続");
#endif

	if (strcmp(m_sz_lan_vequ, "接続") != 0)
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
/*! CTrsControlS64

-# COH/INCOH INCOH 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_coh_incoh_incoh()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), "接続");
#endif

	if (strcmp(m_sz_lan_vequ, "接続") != 0)
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
/*! CTrsControlS64

-# RNGING STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_rnging_stop()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_RNG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_rng, sizeof(m_sz_lan_rng), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_RNG_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rng_cont, sizeof(m_sz_rng_cont), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_rng, sizeof(m_sz_lan_rng), "接続");
	strcpy_s(m_sz_rng_cont, sizeof(m_sz_rng_cont), "REMOTE");
#endif

	if (strcmp(m_sz_lan_rng, "接続") != 0)
	{
		// ローカル
		m_RngingStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_RngingStopBtn.SetColor(COLOR_REMOTES);
	}

	// START 側
	if ((m_SSRngOnBtn.GetColor() == COLOR_REMOTES) &&
		((strcmp(m_sz_lan_rng, "接続") == 0) &&
		(strcmp(m_sz_rng_cont, "REMOTE") == 0) &&
		(m_i_priv_flag == 1)))
	{
		// リモート（非選択）
		m_RngingStartBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# RNGING START 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_rnging_start()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照 
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_RNG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_rng, sizeof(m_sz_lan_rng), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_RNG_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rng_cont, sizeof(m_sz_rng_cont), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_rng, sizeof(m_sz_lan_rng), "接続");
	strcpy_s(m_sz_rng_cont, sizeof(m_sz_rng_cont), "REMOTE");
#endif

	if ((m_SSRngOnBtn.GetColor() == COLOR_REMOTES) &&
		(strcmp(m_sz_lan_rng, "接続") == 0)) 
	{
		// リモート（選択）
		m_RngingStartBtn.SetColor(COLOR_REMOTES);
	}
	else 
	{
		// ローカル
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}

	// STOP 側
	if ((strcmp(m_sz_lan_rng, "接続") == 0) && (strcmp(m_sz_rng_cont, "REMOTE") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_RngingStopBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_RngingStopBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# HOLD TONE OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_holdtone_off()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_CMDOUT1));
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

	// ON 側
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
/*! CTrsControlS64

-# HOLD TONE ON 状態画面表示示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::show_holdtone_on()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_CMDOUT1));
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
/*! CTrsControlS64

-# HOLD TONE ON 状態画面表示示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlS64::printlg_endtime(CString str)
{
	// 監視データ取得

	// 現在時刻取得

	// スイープ完了予定時刻算出

	// opelog出力
}


void CTrsControlS64::OnClose()
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

void CTrsControlS64::CloseDlg()
{
	KillTimer(TID_CONDITION);
	KillTimer(TID_COUNTDOWN);

	// ウィンドウを閉じるメッセージ
	theApp.m_pMainWnd->PostMessageW(eMessage_WindowClose, 0, (LPARAM)this);
}
