/*============================================================================*/
/*! TrsControl.cpp

-# X帯送信制御画面
*/
/*============================================================================*/
// TrsControlX.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrsControlX.h"
#include "afxdialogex.h"
#include "TrsControlMng.h"
#include "HookMsgBox.h"


// CTrsControlX ダイアログ

IMPLEMENT_DYNAMIC(CTrsControlX, CDialogBase)

/*============================================================================*/
/*! CTrsControlX

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CTrsControlX::CTrsControlX(CWnd* pParent /*=NULL*/)
: CDialogBase(CTrsControlX::IDD, pParent)
	, m_strXXRng(_T(""))
	, m_strXXRngCont(_T(""))
{
	m_i_priv_flag = -1;
	m_l_rng_type = RNG_TYPE_NON;
}

/*============================================================================*/
/*! CTrsControlX

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CTrsControlX::~CTrsControlX()
{
}

void CTrsControlX::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TRXCTRL_XXRNG_EDIT, m_strXXRng);
	DDX_Control(pDX, IDC_TRXCTRL_RF_OFF_BTN, m_RFPWROffBtn);
	DDX_Control(pDX, IDC_TRXCTRL_RF_ON_BTN, m_RFPWROnBtn);
	DDX_Control(pDX, IDC_TRXCTRL_NOUSE_OFF_BTN, m_NoUseOffBtn);
	DDX_Control(pDX, IDC_TRXCTRL_NOUSE_ON_BTN, m_NoUseOnBtn);
	DDX_Control(pDX, IDC_TRXCTRL_SWEET_STOP_BTN, m_SweepCountStopBtn);
	DDX_Control(pDX, IDC_TRXCTRL_SWEET_INC_BTN, m_SweepCountIncBtn);
	DDX_Control(pDX, IDC_TRXCTRL_SWEET_INCDEC_BTN, m_SweepCountIncDecBtn);
	DDX_Control(pDX, IDC_TRXCTRL_SWEET_DEC_BTN, m_SweepCountDecBtn);
	DDX_Control(pDX, IDC_TRXCTRL_SWEET_DECINC_BTN, m_SweepCountDecIncBtn);
	DDX_Control(pDX, IDC_TRXCTRL_RAMPCONT_OFF_BTN, m_RampContOffBtn);
	DDX_Control(pDX, IDC_TRXCTRL_RAMPCONT_ON_BTN, m_RampContOnBtn);
	DDX_Control(pDX, IDC_TRXCTRL_CMD_OFF_BTN, m_CmdModOffBtn);
	DDX_Control(pDX, IDC_TRXCTRL_CMD_ON_BTN, m_CmdModOnBtn);
	DDX_Control(pDX, IDC_TRXCTRL_RNG_OFF_BTN, m_RngModOffBtn);
	DDX_Control(pDX, IDC_TRXCTRL_RNG_ON_BTN, m_RngModOnBtn);
	DDX_Control(pDX, IDC_TRXCTRL_MODULATION_OFF_BTN, m_ModulationOffBtn);
	DDX_Control(pDX, IDC_TRXCTRL_MODULATION_ON_BTN, m_ModulationOnBtn);
	DDX_Control(pDX, IDC_TRXCTRL_HOLDTONE_OFF_BTN, m_HoldToneOffBtn);
	DDX_Control(pDX, IDC_TRXCTRL_HOLDTONE_ON_BTN, m_HoldToneOnBtn);
	DDX_Control(pDX, IDC_TRXCTRL_XXCOH_COH_BTN, m_XXCohCohBtn);
	DDX_Control(pDX, IDC_TRXCTRL_XXCOH_INCOH_BTN, m_XXCohIncohBtn);
	DDX_Control(pDX, IDC_TRXCTRL_RNGING_STOP_BTN, m_RngingStopBtn);
	DDX_Control(pDX, IDC_TRXCTRL_RNGING_START_BTN, m_RngingStartBtn);
	DDX_Control(pDX, IDC_TRXCTRL_RGNTYPE_STOP_BTN, m_RRngStopBtn);
	DDX_Control(pDX, IDC_TRXCTRL_RGNTYPE_START_BTN, m_RRngStartBtn);
	DDX_Control(pDX, IDC_TRXCTRL_XXRNG_C_BTN, m_XXRngNumBtn);
	DDX_Text(pDX, IDC_TRXCTRL_XXRNG_CONT_STATIC, m_strXXRngCont);
	DDX_Control(pDX, IDC_TRXCTRL_TX_STATIC, m_grTxControl);
	DDX_Control(pDX, IDC_TRXCTRL_RF_STATIC, m_grRfPower);
	DDX_Control(pDX, IDC_TRXCTRL_NOUSE_STATIC, m_grNoUse);
	DDX_Control(pDX, IDC_TRXCTRL_SWEET_STATIC, m_grSweepCont);
	DDX_Control(pDX, IDC_TRXCTRL_RAMPCONT_STATIC, m_grRampCont);
	DDX_Control(pDX, IDC_TRXCTRL_MOD_STATIC, m_grMod);
	DDX_Control(pDX, IDC_TRXCTRL_CMD_STATIC, m_grCmdMod);
	DDX_Control(pDX, IDC_TRXCTRL_RNG_STATIC, m_grRngMod);
	DDX_Control(pDX, IDC_TRXCTRL_TCRMOD_STATIC, m_grTcrMod);
	DDX_Control(pDX, IDC_TRXCTRL_MODULATION_STATIC, m_grModulation);
	DDX_Control(pDX, IDC_TRXCTRL_HOLDTONE_STATIC, m_grHoldTone);
	DDX_Control(pDX, IDC_TRXCTRL_RNGCTRL_STATIC, m_grRngControl);
	DDX_Control(pDX, IDC_TRXCTRL_XXRNG_STATIC, m_grXXRng);
	DDX_Control(pDX, IDC_TRXCTRL_XXCOH_STATIC, m_grXXCoh);
	DDX_Control(pDX, IDC_TRXCTRL_RNGING_STATIC, m_grRnging);
	DDX_Control(pDX, IDC_TRXCTRL_RGNTYPE_STATIC, m_grRRng);
}


BEGIN_MESSAGE_MAP(CTrsControlX, CDialogBase)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CTrsControlX::OnMenuClose)
	ON_BN_CLICKED(IDC_TRXCTRL_XXRNG_C_BTN, &CTrsControlX::OnBnClickedTrxctrlXxrngCBtn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TRXCTRL_RF_OFF_BTN, &CTrsControlX::OnBnClickedTrxctrlRfOffBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_RF_ON_BTN, &CTrsControlX::OnBnClickedTrxctrlRfOnBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_NOUSE_OFF_BTN, &CTrsControlX::OnBnClickedTrxctrlNouseOffBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_NOUSE_ON_BTN, &CTrsControlX::OnBnClickedTrxctrlNouseOnBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_SWEET_STOP_BTN, &CTrsControlX::OnBnClickedTrxctrlSweetStopBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_SWEET_INC_BTN, &CTrsControlX::OnBnClickedTrxctrlSweetIncBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_SWEET_INCDEC_BTN, &CTrsControlX::OnBnClickedTrxctrlSweetIncdecBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_SWEET_DEC_BTN, &CTrsControlX::OnBnClickedTrxctrlSweetDecBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_SWEET_DECINC_BTN, &CTrsControlX::OnBnClickedTrxctrlSweetDecincBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_RAMPCONT_OFF_BTN, &CTrsControlX::OnBnClickedTrxctrlRampcontOffBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_RAMPCONT_ON_BTN, &CTrsControlX::OnBnClickedTrxctrlRampcontOnBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_CMD_OFF_BTN, &CTrsControlX::OnBnClickedTrxctrlCmdOffBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_CMD_ON_BTN, &CTrsControlX::OnBnClickedTrxctrlCmdOnBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_RNG_OFF_BTN, &CTrsControlX::OnBnClickedTrxctrlRngOffBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_RNG_ON_BTN, &CTrsControlX::OnBnClickedTrxctrlRngOnBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_MODULATION_OFF_BTN, &CTrsControlX::OnBnClickedTrxctrlModulationOffBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_MODULATION_ON_BTN, &CTrsControlX::OnBnClickedTrxctrlModulationOnBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_HOLDTONE_OFF_BTN, &CTrsControlX::OnBnClickedTrxctrlHoldtoneOffBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_HOLDTONE_ON_BTN, &CTrsControlX::OnBnClickedTrxctrlHoldtoneOnBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_XXCOH_COH_BTN, &CTrsControlX::OnBnClickedTrxctrlXxcohCohBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_XXCOH_INCOH_BTN, &CTrsControlX::OnBnClickedTrxctrlXxcohIncohBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_RNGING_STOP_BTN, &CTrsControlX::OnBnClickedTrxctrlRngingStopBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_RNGING_START_BTN, &CTrsControlX::OnBnClickedTrxctrlRngingStartBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_RGNTYPE_STOP_BTN, &CTrsControlX::OnBnClickedTrxctrlRgntypeStopBtn)
	ON_BN_CLICKED(IDC_TRXCTRL_RGNTYPE_START_BTN, &CTrsControlX::OnBnClickedTrxctrlRgntypeStartBtn)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTrsControlX メッセージ ハンドラー

/*============================================================================*/
/*! CTrsControlX

-# ファイル-閉じるメニュー処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlX::OnMenuClose()
{
	// いずれかのボタンが実行中の場合
	if (m_RFPWROffBtn.IsColorIgnore2() ||
		m_RFPWROnBtn.IsColorIgnore2() ||
		m_NoUseOffBtn.IsColorIgnore2() ||
		m_NoUseOnBtn.IsColorIgnore2() ||
		m_SweepCountStopBtn.IsColorIgnore2() ||
		m_SweepCountIncBtn.IsColorIgnore2() ||
		m_SweepCountIncDecBtn.IsColorIgnore2() ||
		m_SweepCountDecBtn.IsColorIgnore2() ||
		m_SweepCountDecIncBtn.IsColorIgnore2() ||
		m_RampContOffBtn.IsColorIgnore2() ||
		m_RampContOnBtn.IsColorIgnore2() ||
		m_CmdModOffBtn.IsColorIgnore2() ||
		m_CmdModOnBtn.IsColorIgnore2() ||
		m_RngModOffBtn.IsColorIgnore2() ||
		m_RngModOnBtn.IsColorIgnore2() ||
		m_ModulationOffBtn.IsColorIgnore2() ||
		m_ModulationOnBtn.IsColorIgnore2() ||
		m_HoldToneOffBtn.IsColorIgnore2() ||
		m_HoldToneOnBtn.IsColorIgnore2() ||
		m_XXRngNumBtn.IsColorIgnore2() ||
		m_XXCohCohBtn.IsColorIgnore2() ||
		m_XXCohIncohBtn.IsColorIgnore2() ||
		m_RngingStopBtn.IsColorIgnore2() ||
		m_RngingStartBtn.IsColorIgnore2() ||
		m_RRngStopBtn.IsColorIgnore2() ||
		m_RRngStartBtn.IsColorIgnore2())
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
/*! CTrsControlX

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CTrsControlX::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// RF-POWER
	m_RFPWROffBtn.SetColor(COLOR_REMOTES);
	m_RFPWROffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_RFPWROnBtn.SetColor(COLOR_REMOTES);
	m_RFPWROnBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// NO-USE
	m_NoUseOffBtn.SetColor(COLOR_REMOTES);
	m_NoUseOffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_NoUseOnBtn.SetColor(COLOR_REMOTE);
	m_NoUseOnBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// SWEEP-CONT
	m_SweepCountStopBtn.SetColor(COLOR_REMOTES);
	m_SweepCountStopBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepCountIncBtn.SetColor(COLOR_REMOTE);
	m_SweepCountIncBtn.SetCtrlButton(TRUE);			// Ctrlキー +
	m_SweepCountIncDecBtn.SetColor(COLOR_REMOTE);
	m_SweepCountIncDecBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepCountDecBtn.SetColor(COLOR_REMOTE);
	m_SweepCountDecBtn.SetCtrlButton(TRUE);			// Ctrlキー +
	m_SweepCountDecIncBtn.SetColor(COLOR_REMOTE);
	m_SweepCountDecIncBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// RAMP-CONT
	m_RampContOffBtn.SetColor(COLOR_REMOTES);
	m_RampContOffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_RampContOnBtn.SetColor(COLOR_REMOTE);
	m_RampContOnBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// CMD-MOD
	m_CmdModOffBtn.SetColor(COLOR_REMOTES);
	m_CmdModOffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_CmdModOnBtn.SetColor(COLOR_REMOTE);
	m_CmdModOnBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// RNG-MOD
	m_RngModOffBtn.SetColor(COLOR_REMOTES);
	m_RngModOffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_RngModOnBtn.SetColor(COLOR_REMOTE);
	m_RngModOnBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// MODULATION
	m_ModulationOffBtn.SetColor(COLOR_REMOTES);
	m_ModulationOffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_ModulationOnBtn.SetColor(COLOR_REMOTES);
	m_ModulationOnBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// HOLD-TONE
	m_HoldToneOffBtn.SetColor(COLOR_REMOTES);
	m_HoldToneOffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_HoldToneOnBtn.SetColor(COLOR_REMOTE);
	m_HoldToneOnBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// X/X-COH
	m_XXCohCohBtn.SetColor(COLOR_REMOTES);
	m_XXCohCohBtn.SetCtrlButton(TRUE);			// Ctrlキー +
	m_XXCohIncohBtn.SetColor(COLOR_REMOTE);
	m_XXCohIncohBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// RNGING
	m_RngingStopBtn.SetColor(COLOR_REMOTES);
	m_RngingStopBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_RngingStartBtn.SetColor(COLOR_REMOTES);
	m_RngingStartBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// 再生型
	m_RRngStopBtn.SetColor(COLOR_REMOTES);
	m_RRngStopBtn.SetShiftButton(TRUE);			// Shiftキー +
	m_RRngStartBtn.SetColor(COLOR_REMOTE);
	m_RRngStartBtn.SetShiftButton(TRUE);		// Shiftキー +

	// X/X-RNG CNT
	m_XXRngNumBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_XXRngNumBtn.SetCtrlButton(TRUE);		// Ctrlキー +


	// グループボックス
	m_grTxControl.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grTxControl.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRfPower.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRfPower.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grNoUse.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grNoUse.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grSweepCont.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grSweepCont.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRampCont.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRampCont.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grMod.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grMod.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grCmdMod.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grCmdMod.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRngMod.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRngMod.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grTcrMod.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grTcrMod.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grModulation.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grModulation.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grHoldTone.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grHoldTone.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRngControl.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRngControl.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grXXRng.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grXXRng.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grXXCoh.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grXXCoh.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRnging.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRnging.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRRng.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRRng.SetGroupType(CGroupBoxEx::eGroupType_Round);

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
	m_l_ramp_cont_off_cntdwn = -1;
	m_l_ramp_cont_on_cntdwn = -1;
	m_l_cmd_mod_off_cntdwn = -1;
	m_l_cmd_mod_on_cntdwn = -1;
	m_l_rng_mod_off_cntdwn = -1;
	m_l_rng_mod_on_cntdwn = -1;
	m_l_modulation_off_cntdwn = -1;
	m_l_modulation_on_cntdwn = -1;
	m_l_hold_tone_off_cntdwn = -1;
	m_l_hold_tone_on_cntdwn = -1;
	m_l_xx_rng_off_cntdwn = -1;
	m_l_xx_rng_on_cntdwn = -1;
	m_l_coh_incoh_coh_cntdwn = -1;
	m_l_coh_incoh_incoh_cntdwn = -1;
	m_l_rnging_stop_cntdwn = -1;
	m_l_rnging_start_cntdwn = -1;
	m_l_rep_rnging_stop_cntdwn = -1;
	m_l_rep_rnging_start_cntdwn = -1;
	m_l_rng_type = -1;


#if 0
	// ホスト名取得
	memset(m_sz_hostname, (char)NULL, sizeof(m_sz_hostname));
	gethostname(m_sz_hostname, sizeof(m_sz_hostname));

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
/*! CTrsControlX

-# ダイアログ更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CTrsControlX::UpdateDlg()
{
	// 送信制御画面状態表示タイムアウトコールバックルーチン起動
	show_opsc_condition();

	// カウントダウンタイムアウトコールバックルーチン起動
	count_down();

	return TRUE;
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『RF-POWER』-『OFF』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRfOffBtn()
{
	int station = theApp.GetSelectStation();

	// ボタンを制御中にする
	m_RFPWROffBtn.SetColor(COLOR_CTRLIN);
	m_RFPWROnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	if (station == 0)
	{
		// 臼田64m
		theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_ON/OFF_CONT"), _T("OFF"));
	}
	else if (station == 2)
	{
		// 内之浦34m
		theApp.GetSatelliteData().opctproc(_T("X-TX.RF_CONT"), _T("OFF"));
	}

	// カウントダウン開始
	m_l_rf_power_off_cntdwn = RF_POWER_TIME_X_LIMIT;

	// 更新
	m_RFPWROffBtn.Invalidate();
	m_RFPWROnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『RF-POWER』-『ON』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRfOnBtn()
{
	int station = theApp.GetSelectStation();

	// ボタンを制御中にする
	m_RFPWROnBtn.SetColor(COLOR_CTRLIN);
	m_RFPWROffBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	if (station == 0)
	{
		// 臼田64m
		theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_ON/OFF_CONT"), _T("ON"));
	}
	else if (station == 2)
	{
		// 内之浦34m
		theApp.GetSatelliteData().opctproc(_T("X-TX.RF_CONT"), _T("ON"));
	}

	// カウントダウン開始
	m_l_rf_power_on_cntdwn = RF_POWER_TIME_X_LIMIT;

	// 更新
	m_RFPWROffBtn.Invalidate();
	m_RFPWROnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『NO-USE』-『OFF』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlNouseOffBtn()
{
	COLORREF col = m_NoUseOffBtn.GetColor();

	int station = theApp.GetSelectStation();
	if (station == 0)
	{
		// 臼田64m
		if (col == COLOR_REMOTE)
		{
			// リモート（非選択）

			// ボタンを制御中にする
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE_RESET"));

			// カウントダウン開始
			m_l_no_use_off_cntdwn = NO_USE_TIME_X_LIMIT;
		}
		else if (col == COLOR_CTRLIN)
		{
			// 制御中

			// ボタンを制御中にする
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE_RESET"));

			// カウントダウン開始
			m_l_no_use_off_cntdwn = NO_USE_TIME_X_LIMIT;

			// 他方のカウントダウン終了
			m_l_no_use_on_cntdwn = -1;
		}
		else
		{
			;	// nothing!
		}
	}
	else if (station == 2)
	{
		// 内之浦34m
		if (col == COLOR_REMOTE)
		{
			// リモート（非選択）

			// ボタンを制御中にする
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-TX.RF_TRIP_NO_USE"), _T("NO_USE_RESET"));

			// カウントダウン開始
			m_l_no_use_off_cntdwn = NO_USE_TIME_X_LIMIT;
		}
		else if (col == COLOR_CTRLIN)
		{
			// 制御中

			// ボタンを制御中にする
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-TX.RF_TRIP_NO_USE"), _T("NO_USE_RESET"));

			// カウントダウン開始
			m_l_no_use_off_cntdwn = NO_USE_TIME_X_LIMIT;

			// 他方のカウントダウン終了
			m_l_no_use_on_cntdwn = -1;
		}
		else if (col == COLOR_REMOTES)
		{
			// リモート（選択）

			// ボタンを制御中にする
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-TX.RF_TRIP_NO_USE"), _T("NO_USE_RESET"));

			// カウントダウン開始
			m_l_no_use_off_cntdwn = NO_USE_TIME_X_LIMIT;

			// 他方のカウントダウン終了
			m_l_no_use_on_cntdwn = -1;
		}
		else
		{
			;	// nothing!
		}
	}

	// 更新
	m_NoUseOffBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『NO-USE』-『ON』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlNouseOnBtn()
{
	COLORREF col = m_NoUseOnBtn.GetColor();
	int station = theApp.GetSelectStation();
	if (station == 0)
	{
		// 臼田64m
		if (col == COLOR_REMOTE)
		{
			// リモート（非選択）

			// ボタンを制御中にする
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE"));

			// カウントダウン開始
			m_l_no_use_on_cntdwn = NO_USE_TIME_X_LIMIT;
		}
		else if (col == COLOR_CTRLIN)
		{
			// 制御中

			// ボタンを制御中にする
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE"));

			// カウントダウン開始
			m_l_no_use_on_cntdwn = NO_USE_TIME_X_LIMIT;

			// 他方のカウントダウン終了
			m_l_no_use_off_cntdwn = -1;
		}
		else
		{
			;	// nothing!
		}
	}
	else if (station == 2)
	{
		// 内之浦34m
		if (col == COLOR_REMOTE)
		{
			// リモート（非選択）

			// ボタンを制御中にする
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-TX.RF_TRIP_NO_USE"), _T("NO_USE"));

			// カウントダウン開始
			m_l_no_use_on_cntdwn = NO_USE_TIME_X_LIMIT;
		}
		else if (col == COLOR_CTRLIN)
		{
			// 制御中

			// ボタンを制御中にする
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-TX.RF_TRIP_NO_USE"), _T("NO_USE"));

			// カウントダウン開始
			m_l_no_use_on_cntdwn = NO_USE_TIME_X_LIMIT;

			// 他方のカウントダウン終了
			m_l_no_use_off_cntdwn = -1;
		}
		else if (col == COLOR_REMOTES)
		{
			// リモート（選択）

			// ボタンを制御中にする
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-TX.RF_TRIP_NO_USE"), _T("NO_USE"));

			// カウントダウン開始
			m_l_no_use_on_cntdwn = NO_USE_TIME_X_LIMIT;

			// 他方のカウントダウン終了
			m_l_no_use_off_cntdwn = -1;
		}
		else
		{
			;	// nothing!
		}
	}

	// 更新
	m_NoUseOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『SWEEP-CONT』-『STOP』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlSweetStopBtn()
{
	// ボタンを制御中にする
	m_SweepCountStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("X-TXSYN.SWEEP_CONT"), _T("STOP"));

	// カウントダウン開始
	m_l_sweep_cont_stop_cntdwn = SWEEP_CONT_TIME_X_LIMIT;

	// 更新
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『SWEEP-CONT』-『INC』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlSweetIncBtn()
{
	// ボタンを制御中にする
	m_SweepCountStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("X-TXSYN.SWEEP_CONT"), _T("INC"));

	// カウントダウン開始
	m_l_sweep_cont_inc_cntdwn = SWEEP_CONT_TIME_X_LIMIT;

	// 更新
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『SWEEP-CONT』-『INCDEC』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlSweetIncdecBtn()
{
	// ボタンを制御中にする
	m_SweepCountStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("X-TXSYN.SWEEP_CONT"), _T("INC+DEC"));

	// カウントダウン開始
	m_l_sweep_cont_incdec_cntdwn = SWEEP_CONT_TIME_X_LIMIT;

	// 更新
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『SWEEP-CONT』-『DEC』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlSweetDecBtn()
{
	// ボタンを制御中にする
	m_SweepCountStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("X-TXSYN.SWEEP_CONT"), _T("DEC"));

	// カウントダウン開始
	m_l_sweep_cont_dec_cntdwn = SWEEP_CONT_TIME_X_LIMIT;

	// 更新
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『SWEEP-CONT』-『DECINC』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlSweetDecincBtn()
{
	// ボタンを制御中にする
	m_SweepCountStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecIncBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("X-TXSYN.SWEEP_CONT"), _T("DEC+INC"));

	// カウントダウン開始
	m_l_sweep_cont_decinc_cntdwn = SWEEP_CONT_TIME_X_LIMIT;

	// 更新
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『RAMP-CONT』-『OFF』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRampcontOffBtn()
{
	// ボタンを制御中にする
	m_RampContOffBtn.SetColor(COLOR_CTRLIN);
	m_RampContOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("X-TXSYN.RATE_AID"), _T("OFF"));

	// カウントダウン開始
	m_l_ramp_cont_off_cntdwn = RAMP_CONT_TIME_X_LIMIT;

	// 更新
	m_RampContOffBtn.Invalidate();
	m_RampContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『RAMP-CONT』-『ON』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRampcontOnBtn()
{
	// ボタンを制御中にする
	m_RampContOffBtn.SetColor(COLOR_CTRLIN);
	m_RampContOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("X-TXSYN.RATE_AID"), _T("ON"));

	// カウントダウン開始
	m_l_ramp_cont_on_cntdwn = RAMP_CONT_TIME_X_LIMIT;

	// 更新
	m_RampContOffBtn.Invalidate();
	m_RampContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『MOD』-『CMD-MOD』-『OFF』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlCmdOffBtn()
{
	// ボタンを制御中にする
	m_CmdModOffBtn.SetColor(COLOR_CTRLIN);
	m_CmdModOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CTRL.X_CMD_MOD"), _T("OFF"));

	// カウントダウン開始
	m_l_cmd_mod_off_cntdwn = CMD_MOD_TIME_X_LIMIT;

	// 更新
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『MOD』-『CMD-MOD』-『ON』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlCmdOnBtn()
{
	// ボタンを制御中にする
	m_CmdModOffBtn.SetColor(COLOR_CTRLIN);
	m_CmdModOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CTRL.X_CMD_MOD"), _T("ON"));

	// カウントダウン開始
	m_l_cmd_mod_on_cntdwn = CMD_MOD_TIME_X_LIMIT;

	// 更新
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『MOD』-『RNG-MOD』-『OFF』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRngOffBtn()
{
	// ボタンを制御中にする
	m_RngModOffBtn.SetColor(COLOR_CTRLIN);
	m_RngModOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CTRL.X_RNG_MOD"), _T("OFF"));

	// カウントダウン開始
	m_l_rng_mod_off_cntdwn = RNG_MOD_TIME_X_LIMIT;

	// 更新
	m_RngModOffBtn.Invalidate();
	m_RngModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『MOD』-『RNG-MOD』-『ON』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRngOnBtn()
{
	// ボタンを制御中にする
	m_RngModOffBtn.SetColor(COLOR_CTRLIN);
	m_RngModOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CTRL.X_RNG_MOD"), _T("ON"));

	// カウントダウン開始
	m_l_rng_mod_on_cntdwn = RNG_MOD_TIME_X_LIMIT;

	// 更新
	m_RngModOffBtn.Invalidate();
	m_RngModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『TCRMOD』-『MODULATION』-『OFF』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlModulationOffBtn()
{
	// ボタンを制御中にする
	m_ModulationOffBtn.SetColor(COLOR_CTRLIN);
	m_ModulationOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("XX-TCRMOD.MODULATION"), _T("OFF"));

	// カウントダウン開始
	m_l_modulation_off_cntdwn = MODULATION_TIME_X_LIMIT;

	// 更新
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『TCRMOD』-『MODULATION』-『ON』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlModulationOnBtn()
{
	// ボタンを制御中にする
	m_ModulationOffBtn.SetColor(COLOR_CTRLIN);
	m_ModulationOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("XX-TCRMOD.MODULATION"), _T("ON"));

	// カウントダウン開始
	m_l_modulation_on_cntdwn = MODULATION_TIME_X_LIMIT;

	// 更新
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『TCRMOD』-『HOLD-TONE』-『OFF』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlHoldtoneOffBtn()
{
	// ボタンを制御中にする
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	int station = theApp.GetSelectStation();
	if (station == 0)
	{
		// 臼田64m
		theApp.GetSatelliteData().opctproc(_T("CMDOUT3.HOLD_TONE"), _T("OFF"));
	}
	else if (station == 2)
	{
		// 内之浦34m
		theApp.GetSatelliteData().opctproc(_T("X-CMDOUT.HOLD_TONE"), _T("OFF"));
	}

	// カウントダウン開始
	m_l_hold_tone_off_cntdwn = HOLD_TONE_TIME_X_LIMIT;

	// 更新
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『TX CONTROL』-『TCRMOD』-『HOLD-TONE』-『ON』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlHoldtoneOnBtn()
{
	// ボタンを制御中にする
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	int station = theApp.GetSelectStation();
	if (station == 0)
	{
		// 臼田64m
		theApp.GetSatelliteData().opctproc(_T("CMDOUT3.HOLD_TONE"), _T("ON"));
	}
	else if (station == 2)
	{
		// 内之浦34m
		theApp.GetSatelliteData().opctproc(_T("X-CMDOUT.HOLD_TONE"), _T("ON"));
	}

	// カウントダウン開始
	m_l_hold_tone_on_cntdwn = HOLD_TONE_TIME_X_LIMIT;

	// 更新
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『RNG CONTROL』-『X/X-RNG』-『Num』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlXxrngCBtn()
{
	// 制御通知送信
	UpdateData();
	theApp.GetSatelliteData().opctproc(_T("V-EQU.RNG_NUM"), _T(""), m_strXXRng);
}


/*============================================================================*/
/*! CTrsControlX

-# 『RNG CONTROL』-『X/X-COH』-『COH』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlXxcohCohBtn()
{
	// ボタンを制御中にする
	m_XXCohCohBtn.SetColor(COLOR_CTRLIN);
	m_XXCohIncohBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("COHI"));

	// カウントダウン開始
	m_l_coh_incoh_coh_cntdwn = COH_INCOH_TIME_X_LIMIT;

	// 更新
	m_XXCohCohBtn.Invalidate();
	m_XXCohIncohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『RNG CONTROL』-『X/X-COH』-『INCOH』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlXxcohIncohBtn()
{
	// ボタンを制御中にする
	m_XXCohCohBtn.SetColor(COLOR_CTRLIN);
	m_XXCohIncohBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("INCOHI"));

	// カウントダウン開始
	m_l_coh_incoh_incoh_cntdwn = COH_INCOH_TIME_X_LIMIT;

	// 更新
	m_XXCohCohBtn.Invalidate();
	m_XXCohIncohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『RNG CONTROL』-『RNGING』-『STOP』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRngingStopBtn()
{
	// ボタンを制御中にする STOP ボタンのみ
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING_CANCEL"), _T(","));

	// カウントダウン開始
	m_l_rnging_stop_cntdwn = RNGING_TIME_X_LIMIT;

	// RNG種別フラグをクリアする
	m_l_rng_type = RNG_TYPE_NON;

	// 更新
	m_RngingStopBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『RNG CONTROL』-『RNGING』-『START』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRngingStartBtn()
{
	CString str1, str2;

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("XX-RRNG.DIST_RNG_VIDEO"), _T("NORMAL"));

	// 再生型ボタンをインセンシティブにする
	if (m_RRngStartBtn.IsWindowEnabled() == TRUE)
	{
		m_RRngStopBtn.EnableWindow(FALSE);
		m_RRngStartBtn.EnableWindow(FALSE);
	}

	// ボタンを制御中にする
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);
	m_RngingStartBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信 X/X-RNGのみ行う
	m_XXRngNumBtn.GetWindowTextW(str2);
	str1.Format(_T("XX%02d"), _wtoi(str2));
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str1);

	// カウントダウン開始
	m_l_rnging_start_cntdwn = RNGING_TIME_X_LIMIT;

	// RNG種別フラグを従来型に設定
	m_l_rng_type = RNG_TYPE_NOR;

	// 更新
	m_RngingStartBtn.Invalidate();
	m_RngingStopBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『RNG CONTROL』-『再生型』-『STOP』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRgntypeStopBtn()
{
	// ボタンを制御中にする STOP ボタンのみ
	m_RRngStopBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING_CANCEL"), _T(","));

	// カウントダウン開始
	m_l_rep_rnging_stop_cntdwn = REP_RNGING_TIME_X_LIMIT;

	// RNG種別フラグ設定
	m_l_rng_type = RNG_TYPE_NON;

	// 更新
	m_RRngStopBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# 『RNG CONTROL』-『再生型』-『START』ボタン押下処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRgntypeStartBtn()
{
	CString str1, str2;

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("XX-RRNG.DIST_RNG_VIDEO"), _T("REPLAY"));

	// 従来型ボタンをインセンシティブにする
	if (m_RngingStartBtn.IsWindowEnabled() == TRUE)
	{
		m_RngingStopBtn.EnableWindow(FALSE);
		m_RngingStartBtn.EnableWindow(FALSE);
	}

	// ボタンを制御中にする
	m_RRngStopBtn.SetColor(COLOR_CTRLIN);
	m_RRngStartBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信 X/X-RNGのみ行う
	m_XXRngNumBtn.GetWindowTextW(str2);
	str1.Format(_T("RX%02d"), _wtoi(str2));
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str1);

	// カウントダウン開始
	m_l_rep_rnging_start_cntdwn = REP_RNGING_TIME_X_LIMIT;

	// RNG種別フラグを再生型に設定
	m_l_rng_type = RNG_TYPE_REP;

	// 更新
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# OnTimer

@param  nIDEvent:タイマーの識別子
@retval なし
*/
/*============================================================================*/
void CTrsControlX::OnTimer(UINT_PTR nIDEvent)
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
/*! CTrsControlX

-# 送信制御画面状態表示設定

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_opsc_condition()
{
	TRACE(_T("show_opsc_condition\n"));
	KillTimer(TID_CONDITION);

	int station = theApp.GetSelectStation();
	if (station == 0)
	{
		// 臼田64m
		show_opsc_condition64();
	}
	else if (station == 2)
	{
		// 内之浦34m
		show_opsc_condition34();
	}

	SetTimer(TID_CONDITION, INTERVAL_CONDITION, 0);
}

/*============================================================================*/
/*! CTrsControlX

-# 送信制御画面状態表示設定

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_opsc_condition64()
{
	TRACE(_T("show_opsc_condition\n"));

	char sz_buff[64];

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 制御権は常に有り
	m_i_priv_flag = 1;

	/*-- TX CONTROL ----------------------------------------------------------------------*/

	// RF POWER ON ENABLE 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_RF_ON_ENBL_X), CString(m_sz_rf_on_enbl_x), nLogEx::debug);
	}

	// RF-TRIP NOUSE ON ENABLE 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_NO_USE_ENBL_X), CString(m_sz_no_use_enbl_x), nLogEx::debug);
	}

	// UP-LINK CONTROL 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_UPLINK_CONT_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_uplink_cont_x, sizeof(m_sz_uplink_cont_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_UPLINK_CONT_X), CString(m_sz_uplink_cont_x), nLogEx::debug);
	}

	// RF POWER ON DETECT ANSWER 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RF_ON_DET_ANS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans_x, sizeof(m_sz_rf_on_det_ans_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_RF_ON_DET_ANS_X), CString(m_sz_rf_on_det_ans_x), nLogEx::debug);
	}

	// RF POWER ON DETECT ANSWER 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RF_ON_DET_ANS_S));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans_s, sizeof(m_sz_rf_on_det_ans_s), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_RF_ON_DET_ANS_S), CString(m_sz_rf_on_det_ans_s), nLogEx::debug);
	}

	// HPA-2増設対応
	// HPA SEL 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_HPA_SEL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hpa_sel, sizeof(m_sz_hpa_sel), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_HPA_SEL), CString(m_sz_hpa_sel), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RF_ON_DET_ANS_X_H2));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans_x_h2, sizeof(m_sz_rf_on_det_ans_x_h2), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_RF_ON_DET_ANS_X_H2), CString(m_sz_rf_on_det_ans_x_h2), nLogEx::debug);
	}


	/*** リモート／ローカル 状態取得 ******************************************************/

#if 0
	// S-TX リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_X_TX_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_tx_rl, sizeof(m_sz_x_tx_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-TX.TX_TOTAL_CONT"), CString(m_sz_x_tx_rl), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	strcpy_s(m_sz_x_tx_rl, sizeof(m_sz_x_tx_rl), "REMOTE");
#endif
#endif

	// S-TXSYN リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_X_TXSYN_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_txsyn_rl, sizeof(m_sz_x_txsyn_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-TXSYN.CONT_MODE"), CString(m_sz_x_txsyn_rl), nLogEx::debug);
	}

	// X-UL_CNT リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_UL_CNT_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_ul_cnt_rl, sizeof(m_sz_x_ul_cnt_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-UL_CNT.X-TX_TOTAL_REMOTE"), CString(m_sz_x_txsyn_rl), nLogEx::debug);
	}

	// X-RF_IF リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RF_IF_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_rf_if_rl, sizeof(m_sz_x_rf_if_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-RF_IF.CONTROL_MODE"), CString(m_sz_x_txsyn_rl), nLogEx::debug);
	}

	// アラームチェック
#if 1
	strcpy_s(m_sz_alarm_x, sizeof(m_sz_alarm_x), "OFF");
#else
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_ALARM_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_alarm_x, sizeof(m_sz_alarm_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-TX.TOTAL_FAULT"), CString(m_sz_alarm_x), nLogEx::debug);
	}
#endif

	// アラーム時
	if (strcmp(m_sz_alarm_x, "ON") == 0)
	{
		// 枠を赤くする
		m_grRfPower.SetBackColor(COLOR_ALARM);
		m_grNoUse.SetBackColor(COLOR_ALARM);
		m_grSweepCont.SetBackColor(COLOR_ALARM);
		m_grRampCont.SetBackColor(COLOR_ALARM);
		m_grCmdMod.SetBackColor(COLOR_ALARM);
		m_grRngMod.SetBackColor(COLOR_ALARM);
		m_grRampCont.SetBackColor(COLOR_ALARM);
//		m_grModulation.SetBackColor(COLOR_ALARM);
//		m_grHoldTone.SetBackColor(COLOR_ALARM);
	}

	// アラーム解除
	else
	{
		// 枠の色を元に戻す
		m_grRfPower.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grNoUse.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepCont.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grRampCont.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grCmdMod.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grRngMod.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grRampCont.SetBackColor(COLOR_3DFACE);
//		m_grModulation.SetBackColor(GetSysColor(COLOR_3DFACE));
//		m_grHoldTone.SetBackColor(GetSysColor(COLOR_3DFACE));
	}


	/*** RF-POWER *************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RF_POWER_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_power_x, sizeof(m_sz_rf_power_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_RF_POWER_X), CString(m_sz_rf_power_x), nLogEx::debug);
	}

	// RF-POWER ON カウントダウン中に監視データが ON になった場合
	if (m_l_rf_power_on_cntdwn >= 0 &&
		strcmp(m_sz_rf_power_x, "ON") == 0)
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
		strcmp(m_sz_rf_power_x, "OFF") == 0)
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
		if (strcmp(m_sz_rf_power_x, "ON") == 0)
		{
			show_rf_power_on64();
		}

		// OFF の場合
		else
		{
			/*** 特殊処理注意 *************************************************************/
			show_rf_power_off64();
		}
	}

	/*** NO-USE ***************************************************************************/
	// 現在の状態を監視ＤＢから取得 
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_NO_USE_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_x, sizeof(m_sz_no_use_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_NO_USE_X), CString(m_sz_no_use_x), nLogEx::debug);
	}

	// NO-USE ON カウントダウン中に監視データが ON になった場合
	if (m_l_no_use_on_cntdwn >= 0 &&
		strcmp(m_sz_no_use_x, "ON") == 0)
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
		strcmp(m_sz_no_use_x, "ON") != 0)
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
		if (strcmp(m_sz_no_use_x, "ON") == 0)
		{
			show_no_use_on64();
		}
		// OFF の場合
		else
		{
			show_no_use_off64();
		}
	}


	/*** SWEEP-CONT ***********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_SWEEP_CONT_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cont_x, sizeof(m_sz_sweep_cont_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_SWEEP_CONT_X), CString(m_sz_sweep_cont_x), nLogEx::debug);
	}

	// SWEEP-CONT STOP カウントダウン中に監視データが STOP になった場合
	if (m_l_sweep_cont_stop_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "STOP") == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_stop_cntdwn = -1;
	}
	// SWEEP-CONT STOP カウントダウンタイムリミットになった場合
	else if (m_l_sweep_cont_stop_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_stop_cntdwn = -1;
	}

	// SWEEP-CONT INC カウントダウン中にデータが INC になった場合
	if (m_l_sweep_cont_inc_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "INC") == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_inc_cntdwn = -1;
	}
	// SWEEP-CONT INC カウントダウンタイムリミットになった場合
	else if (m_l_sweep_cont_inc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_inc_cntdwn = -1;
	}

	// SWEEP-CONT INC+DEC カウントダウン中にデータが INC+DEC になった場合
	if (m_l_sweep_cont_incdec_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "INC+DEC") == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_incdec_cntdwn = -1;
	}
	// SWEEP-CONT INC+DEC カウントダウンタイムリミットになった場合
	else if (m_l_sweep_cont_incdec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_incdec_cntdwn = -1;
	}

	// SWEEP-CONT DEC カウントダウン中にデータが DEC になった場合
	if (m_l_sweep_cont_dec_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "DEC") == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_dec_cntdwn = -1;
	}
	// SWEEP-CONT DEC カウントダウンタイムリミットになった場合
	else if (m_l_sweep_cont_dec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_dec_cntdwn = -1;
	}

	// SWEEP-CONT DEC+INC カウントダウン中にデータが DEC+INC になった場合
	if (m_l_sweep_cont_decinc_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "DEC+INC") == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_decinc_cntdwn = -1;
	}
	// SWEEP-CONT DEC+INC カウントダウンタイムリミットになった場合
	else if (m_l_sweep_cont_decinc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_decinc_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_sweep_cont_stop_cntdwn == -1 && m_l_sweep_cont_inc_cntdwn == -1 &&
		m_l_sweep_cont_incdec_cntdwn == -1 && m_l_sweep_cont_dec_cntdwn == -1 &&
		m_l_sweep_cont_decinc_cntdwn == -1)
	{

		// INC の場合
		if (strcmp(m_sz_sweep_cont_x, "INC") == 0)
		{
			show_sweep_cont_inc64();
		}

		// INC+DEC の場合
		else if (strcmp(m_sz_sweep_cont_x, "INC+DEC") == 0)
		{
			show_sweep_cont_incdec64();
		}

		// DEC+INC の場合
		else if (strcmp(m_sz_sweep_cont_x, "DEC+INC") == 0)
		{
			show_sweep_cont_decinc64();
		}

		// DEC の場合
		else if (strcmp(m_sz_sweep_cont_x, "DEC") == 0)
		{
			show_sweep_cont_dec64();
		}

		// STOP の場合
		else
		{
			show_sweep_cont_stop64();
		}
	}

	/*** RAMP-CONT <<XUP>> 追加 ***********************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RATE_AID_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rate_aid_x, sizeof(m_sz_rate_aid_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-TXSYN.RATE_AID"), CString(m_sz_rate_aid_x), nLogEx::debug);
	}

	// RAMP-CONT OFF カウントダウン中にデータが OFF になった場合
	if (m_l_ramp_cont_off_cntdwn >= 0 &&
		strcmp(m_sz_rate_aid_x, "OFF") == 0)
	{
		// カウントダウン終了
		m_l_ramp_cont_off_cntdwn = -1;
	}
	// RAMP-CONT OFF カウントダウンタイムリミットになった場合
	else if (m_l_ramp_cont_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_ramp_cont_off_cntdwn = -1;
	}

	// RAMP-CONT ON カウントダウン中にデータが ON になった場合
	if (m_l_ramp_cont_on_cntdwn >= 0 &&
		strcmp(m_sz_rate_aid_x, "ON") == 0)
	{
		// カウントダウン終了
		m_l_ramp_cont_on_cntdwn = -1;
	}
	// RAMP-CONT ON カウントダウンタイムリミットになった場合
	else if (m_l_ramp_cont_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_ramp_cont_on_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_ramp_cont_off_cntdwn == -1 && m_l_ramp_cont_on_cntdwn == -1)
	{
		// ON の場合
		if (strcmp(m_sz_rate_aid_x, "ON") == 0)
		{
			show_ramp_cont_on64();
		}
		// OFF の場合
		else
		{
			show_ramp_cont_off64();
		}
	}

#if 0	// 機器更新により削除
	/*** CMD-MOD **************************************************************************/
	// 現在の状態を監視ＤＢから取得 <<XUP>> X-TX用に変更
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_CMD_MOD_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_cmd_mod_x, sizeof(m_sz_cmd_mod_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-TX.CMD_MOD"), CString(m_sz_cmd_mod_x), nLogEx::debug);
	}
	// CMD-MOD OFF カウントダウン中にデータが OFF になった場合
	if (m_l_cmd_mod_off_cntdwn >= 0 &&
		strcmp(m_sz_cmd_mod_x, "OFF") == 0)
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
		strcmp(m_sz_cmd_mod_x, "ON") == 0)
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
		if (strcmp(m_sz_cmd_mod_x, "ON") == 0)
		{
			show_cmd_mod_on64();
		}
		// OFF の場合
		else
		{
			show_cmd_mod_off64();
		}
	}

	/*** RNG-MOD **************************************************************************/
	// 現在の状態を監視ＤＢから取得 <<XUP>> X-TX用に変更
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RNG_MOD_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rng_mod_x, sizeof(m_sz_rng_mod_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-TX.RNG_MOD"), CString(m_sz_rng_mod_x), nLogEx::debug);
	}

	// RNG-MOD OFF カウントダウン中にデータが OFF になった場合
	if (m_l_rng_mod_off_cntdwn >= 0 &&
		strcmp(m_sz_rng_mod_x, "OFF") == 0)
	{
		// カウントダウン終了
		m_l_rng_mod_off_cntdwn = -1;
	}
	// RNG-MOD OFF カウントダウンタイムリミットになった場合
	else if (m_l_rng_mod_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rng_mod_off_cntdwn = -1;
	}

	// RNG-MOD ON カウントダウン中にデータが ON になった場合
	if (m_l_rng_mod_on_cntdwn >= 0 &&
		strcmp(m_sz_rng_mod_x, "ON") == 0)
	{
		// カウントダウン終了
		m_l_rng_mod_on_cntdwn = -1;
	}
	// RNG-MOD ON カウントダウンタイムリミットになった場合
	else if (m_l_rng_mod_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rng_mod_on_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_rng_mod_off_cntdwn == -1 && m_l_rng_mod_on_cntdwn == -1)
	{
		// ON の場合
		if (strcmp(m_sz_rng_mod_x, "ON") == 0)
		{
			show_rng_mod_on64();
		}
		// OFF の場合
		else
		{
			show_rng_mod_off64();
		}
	}
#endif

	/*** TCRMOD MODULATION ****************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_TCRMOD_STS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_tcrmod_sts, sizeof(m_sz_xx_tcrmod_sts), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_XX_TCRMOD_STS), CString(m_sz_xx_tcrmod_sts), nLogEx::debug);
	}
	// TCRMOD MODULATION OFF カウントダウン中にデータが OFF になった場合
	if (m_l_modulation_off_cntdwn >= 0 &&
		strcmp(m_sz_xx_tcrmod_sts, "OFF") == 0)
	{
		// カウントダウン終了
		m_l_modulation_off_cntdwn = -1;
	}
	// TCRMOD MODULATION OFF カウントダウンタイムリミットになった場合
	else if (m_l_modulation_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_modulation_off_cntdwn = -1;
	}

	// TCRMOD MODULATION ON カウントダウン中にデータが ON になった場合
	if (m_l_modulation_on_cntdwn >= 0 &&
		strcmp(m_sz_xx_tcrmod_sts, "ON") == 0)
	{
		// カウントダウン終了
		m_l_modulation_on_cntdwn = -1;
	}
	// TCRMOD MODULATION ON カウントダウンタイムリミットになった場合
	else if (m_l_modulation_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_modulation_on_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_modulation_off_cntdwn == -1 && m_l_modulation_on_cntdwn == -1)
	{
		// ON の場合
		if (strcmp(m_sz_xx_tcrmod_sts, "ON") == 0)
		{
			show_modulation_on64();
		}
		// OFF の場合
		else
		{
			show_modulation_off64();
		}
	}

	/*** X-CMDOUT HOLD_TONE ***************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_HOLD_TONE_STS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hold_tone_sts_x, sizeof(m_sz_hold_tone_sts_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_HOLD_TONE_STS_X), CString(m_sz_hold_tone_sts_x), nLogEx::debug);
	}

	// X-CMDOUT HOLD_TONE OFF カウントダウン中にデータが OFF になった場合
	if (m_l_hold_tone_off_cntdwn >= 0 &&
		strcmp(m_sz_hold_tone_sts_x, "OFF") == 0)
	{
		// カウントダウン終了
		m_l_hold_tone_off_cntdwn = -1;
	}
	// X-CMDOUT HOLD_TONE OFF カウントダウンタイムリミットになった場合
	else if (m_l_hold_tone_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_hold_tone_off_cntdwn = -1;
	}

	// X-CMDOUT HOLD_TONE ON カウントダウン中にデータが ON になった場合
	if (m_l_hold_tone_on_cntdwn >= 0 &&
		strcmp(m_sz_hold_tone_sts_x, "ON") == 0)
	{
		// カウントダウン終了
		m_l_hold_tone_on_cntdwn = -1;
	}
	// TCRMOD MODULATION ON カウントダウンタイムリミットになった場合
	else if (m_l_hold_tone_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_hold_tone_on_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_hold_tone_off_cntdwn == -1 && m_l_hold_tone_on_cntdwn == -1)
	{
		// ON の場合
		if (strcmp(m_sz_hold_tone_sts_x, "ON") == 0)
		{
			show_hold_tone_on64();
		}
		// OFF の場合
		else
		{
			show_hold_tone_off64();
		}
	}

	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	/*** SS-RNG / SX-RNG ******************************************************************/
	/*** 数値表示部最新データ表示 *********************************************************/
	// 現在の状態を監視ＤＢから取得 
	// 選ばれているレンジ装置が従来型か再生型かでRNG回数の表示元を切り替える
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_NORM_REP));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_NORM_REP), CString(m_sz_norm_rep), nLogEx::debug);
	}

	// 再生型が選ばれている場合
	if (strcmp(m_sz_norm_rep, "REPLAY") == 0)
	{
		// 再生型測距回数を抽出
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RX_RNG_CNT));
		if (obs_adr != NULL)
		{
			m_d_xx_rng_cnt = obs_adr->d_data;
		}
	}
	// 従来型が選ばれている場合
	else if (strcmp(m_sz_norm_rep, "NORMAL") == 0)
	{
		// 従来型測距回数を抽出
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_RNG_CNT));
		if (obs_adr != NULL)
		{
			m_d_xx_rng_cnt = obs_adr->d_data;
		}
	}
	// 何れでもない場合は０クリア
	else
	{
		m_d_xx_rng_cnt = 0;
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_RNG_NUM));
	if (obs_adr != NULL)
	{
		m_d_xx_rng_num = obs_adr->d_data;
	}

	// 数値表示部を更新
	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_xx_rng_cnt % 100);
	m_strXXRng = CString(sz_buff);

	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_xx_rng_num % 100);
	m_XXRngNumBtn.SetWindowTextW(CString(sz_buff));

	/*** "CONTINUATION" 表示有無 および センシティブ操作 **********************************/
	// 現在の状態を監視ＤＢから取得 
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_CONT_FLAG_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_cont_flag_x, sizeof(m_sz_cont_flag_x), obs_adr->sz_sts_name);
	}

	// 近地球衛星の場合
	if (strcmp(m_sz_cont_flag_x, "NEAR") == 0)
	{
		// XX-RNG の数値表示部上に "CONTINUATION" と表示
		m_strXXRngCont = _T("CONTINUATION");

		// 数値表示部のセンシティブを落とす
		m_XXRngNumBtn.EnableWindow(FALSE);
	}
	// 深宇宙衛星の場合
	else
	{
		// XX-RNG の数値表示部上の "CONTINUATION" を消す（あれば）
		m_strXXRngCont = _T("");

		// 数値表示部のセンシティブを付ける
		m_XXRngNumBtn.EnableWindow(TRUE);
	}


	/*** COH/INCOH ************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_COH_INCOH));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_coh_incoh, sizeof(m_sz_coh_incoh), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("V-EQU.COHI"), CString(m_sz_coh_incoh), nLogEx::debug);
	}

	// COH/INCOH COH カウントダウン中にデータが COH になった場合
	if (m_l_coh_incoh_coh_cntdwn >= 0 &&
		strcmp(m_sz_coh_incoh, "COHI") == 0)
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
		strcmp(m_sz_coh_incoh, "INCOHI") == 0)
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
			show_coh_incoh_incoh64();
		}
		// COH の場合
		else
		{
			show_coh_incoh_coh64();
		}
	}


	/*** RNGING ***************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("CTRL.RNG_FLG"), CString(m_sz_rnging), nLogEx::debug);
	}

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

	// (再生型) RNGING STOP カウントダウン中にデータが STOP になった場合
	if (m_l_rep_rnging_stop_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "終了") == 0)
	{
		// カウントダウン終了
		m_l_rep_rnging_stop_cntdwn = -1;
	}
	// (再生型) RNGING STOP カウントダウンタイムリミットになった場合
	else if (m_l_rep_rnging_stop_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rep_rnging_stop_cntdwn = -1;
	}

	// (再生型) RNGING START カウントダウン中にデータが START になった場合
	if (m_l_rep_rnging_start_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "開始") == 0)
	{
		// カウントダウン終了
		m_l_rep_rnging_start_cntdwn = -1;
	}
	// (再生型) RNGING START カウントダウンタイムリミットになった場合
	else if (m_l_rep_rnging_start_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rep_rnging_start_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_rnging_stop_cntdwn == -1 && m_l_rnging_start_cntdwn == -1)
	{
		// START の場合
		if (strcmp(m_sz_rnging, "開始") == 0)
		{
			show_rnging_start64();
		}
		// STOP の場合
		else
		{
			show_rnging_stop64();
		}
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_rep_rnging_stop_cntdwn == -1 && m_l_rep_rnging_start_cntdwn == -1)
	{
		// START の場合
		if (strcmp(m_sz_rnging, "開始") == 0)
		{
			show_rrng_start64();
		}
		// STOP の場合
		else
		{
			show_rrng_stop64();
		}
	}

	/*** 変調装置の選択状態確認 ***********************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_MOD_IN_SEL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_mod_in_sel, sizeof(m_sz_mod_in_sel), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-IFSW.IF_CH_INPUT_SEL_ANS"), CString(m_sz_mod_in_sel), nLogEx::debug);
	}

	// TCRMODが選択されている場合
	if (strcmp(m_sz_mod_in_sel, "TCRMOD") == 0)
	{
		// TCRMODをセンシティブ、PM MODをインセンシティブにする
		m_ModulationOffBtn.EnableWindow(TRUE);
		m_ModulationOnBtn.EnableWindow(TRUE);
		m_HoldToneOffBtn.EnableWindow(TRUE);
		m_HoldToneOnBtn.EnableWindow(TRUE);
		m_CmdModOffBtn.EnableWindow(FALSE);
		m_CmdModOnBtn.EnableWindow(FALSE);
		m_RngModOffBtn.EnableWindow(FALSE);
		m_RngModOnBtn.EnableWindow(FALSE);
	}
	// PM MODが選択されている場合
	else if (strcmp(m_sz_mod_in_sel, "MOD") == 0)
	{
		// TCRMODをインセンシティブ、PM MODをセンシティブにする
		m_ModulationOffBtn.EnableWindow(FALSE);
		m_ModulationOnBtn.EnableWindow(FALSE);
		m_HoldToneOffBtn.EnableWindow(FALSE);
		m_HoldToneOnBtn.EnableWindow(FALSE);
		m_CmdModOffBtn.EnableWindow(TRUE);
		m_CmdModOnBtn.EnableWindow(TRUE);
		m_RngModOffBtn.EnableWindow(TRUE);
		m_RngModOnBtn.EnableWindow(TRUE);
	}
	// いづれの状態でもない場合
	else
	{
		// どちらもインセンシティブにする
		m_ModulationOffBtn.EnableWindow(FALSE);
		m_ModulationOnBtn.EnableWindow(FALSE);
		m_HoldToneOffBtn.EnableWindow(FALSE);
		m_HoldToneOnBtn.EnableWindow(FALSE);
		m_CmdModOffBtn.EnableWindow(FALSE);
		m_CmdModOnBtn.EnableWindow(FALSE);
		m_RngModOffBtn.EnableWindow(FALSE);
		m_RngModOnBtn.EnableWindow(FALSE);
	}

	UpdateData(FALSE);
}

/*============================================================================*/
/*! CTrsControlX

-# 送信制御画面状態表示設定

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_opsc_condition34()
{
	TRACE(_T("show_opsc_condition\n"));

	char sz_buff[64];
	char szRfPowerSts[32];

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 制御権は常に有り
	m_i_priv_flag = 1;

	/*-- TX CONTROL ----------------------------------------------------------------------*/

	// RF POWER ON ENABLE 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X34_RF_ON_ENBL_X), CString(m_sz_rf_on_enbl_x), nLogEx::debug);
	}

	// RF-TRIP NOUSE ON ENABLE 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X34_NO_USE_ENBL_X), CString(m_sz_no_use_enbl_x), nLogEx::debug);
	}

	// UP-LINK CONTROL 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_UPLINK_CONT_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_uplink_cont_x, sizeof(m_sz_uplink_cont_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X34_UPLINK_CONT_X), CString(m_sz_uplink_cont_x), nLogEx::debug);
	}

	// RF POWER ON DETECT ANSWER 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_RF_ON_DET_ANS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans_x, sizeof(m_sz_rf_on_det_ans_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X34_RF_ON_DET_ANS_X), CString(m_sz_rf_on_det_ans_x), nLogEx::debug);
	}

	// RF POWER ON DETECT ANSWER 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_RF_ON_DET_ANS_S));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans_s, sizeof(m_sz_rf_on_det_ans_s), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X34_RF_ON_DET_ANS_S), CString(m_sz_rf_on_det_ans_s), nLogEx::debug);
	}


	/*** リモート／ローカル 状態取得 ******************************************************/

	// S-TX リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_X_TX_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_tx_rl, sizeof(m_sz_x_tx_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_X_TX_RL), CString(m_sz_x_tx_rl), nLogEx::debug);
	}

	// S-TXSYN リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_X_TXSYN_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_txsyn_rl, sizeof(m_sz_x_txsyn_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_X_TXSYN_RL), CString(m_sz_x_txsyn_rl), nLogEx::debug);
	}

	/*** アラームチェック  X-TX用に変更 *******************************************/
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_ALARM_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_alarm_x, sizeof(m_sz_alarm_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_ALARM_X), CString(m_sz_alarm_x), nLogEx::debug);
	}

	// アラーム時
	if (strcmp(m_sz_alarm_x, "ON") == 0)
	{
		// 枠を赤くする
		m_grRfPower.SetBackColor(COLOR_ALARM);
		m_grNoUse.SetBackColor(COLOR_ALARM);
		m_grSweepCont.SetBackColor(COLOR_ALARM);
		m_grRampCont.SetBackColor(COLOR_ALARM);
		m_grCmdMod.SetBackColor(COLOR_ALARM);
		m_grRngMod.SetBackColor(COLOR_ALARM);
		m_grRampCont.SetBackColor(COLOR_ALARM);
//		m_grModulation.SetBackColor(COLOR_ALARM);
//		m_grHoldTone.SetBackColor(COLOR_ALARM);
	}

	// アラーム解除
	else
	{
		// 枠の色を元に戻す
		m_grRfPower.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grNoUse.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepCont.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grRampCont.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grCmdMod.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grRngMod.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grRampCont.SetBackColor(COLOR_3DFACE);
//		m_grModulation.SetBackColor(GetSysColor(COLOR_3DFACE));
//		m_grHoldTone.SetBackColor(GetSysColor(COLOR_3DFACE));
	}


	/*** RF-POWER *************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_RF_POWER_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_power_x, sizeof(m_sz_rf_power_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X34_RF_POWER_X), CString(m_sz_rf_power_x), nLogEx::debug);
	}

	// HPA側のRF状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_HPA_RF_POWER_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hpa_rf_power_x, sizeof(m_sz_hpa_rf_power_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_HPA_RF_POWER_X), CString(m_sz_hpa_rf_power_x), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XPA_SEL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xpa_sel, sizeof(m_sz_xpa_sel), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_XPA_SEL), CString(m_sz_xpa_sel), nLogEx::debug);
	}

	// HPA/SSPAで処理を分ける
	if (strcmp(m_sz_xpa_sel, "HPA") == 0)
	{
		// HPAが選択されている場合
		strcpy_s(szRfPowerSts, sizeof(szRfPowerSts), m_sz_hpa_rf_power_x);
	}
	else
	{
		// SSPAが選択されている場合
		strcpy_s(szRfPowerSts, sizeof(szRfPowerSts), m_sz_rf_power_x);
	}

	// RF-POWER ON カウントダウン中に監視データが ON になった場合
	if (m_l_rf_power_on_cntdwn >= 0 &&
		strcmp(szRfPowerSts, "ON") == 0)
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
		strcmp(szRfPowerSts, "OFF") == 0)
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
		if (strcmp(szRfPowerSts, "ON") == 0)
		{
			show_rf_power_on34();
		}

		// OFF の場合
		else
		{
			/*** 特殊処理注意 *************************************************************/
			show_rf_power_off34();
		}
	}

	/*** NO-USE ***************************************************************************/
	// 現在の状態を監視ＤＢから取得 
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_NO_USE_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_x, sizeof(m_sz_no_use_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X34_NO_USE_X), CString(m_sz_no_use_x), nLogEx::debug);
	}

	// NO-USE ON カウントダウン中に監視データが ON になった場合
	if (m_l_no_use_on_cntdwn >= 0 &&
		strcmp(m_sz_no_use_x, "ON") == 0)
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
		strcmp(m_sz_no_use_x, "ON") != 0)
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
		if (strcmp(m_sz_no_use_x, "ON") == 0)
		{
			show_no_use_on34();
		}
		// OFF の場合
		else
		{
			show_no_use_off34();
		}
	}


	/*** SWEEP-CONT ***********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_SWEEP_CONT_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cont_x, sizeof(m_sz_sweep_cont_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_SWEEP_CONT_X), CString(m_sz_sweep_cont_x), nLogEx::debug);
	}

	// SWEEP-CONT STOP カウントダウン中に監視データが STOP になった場合
	if (m_l_sweep_cont_stop_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "STOP") == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_stop_cntdwn = -1;
	}
	// SWEEP-CONT STOP カウントダウンタイムリミットになった場合
	else if (m_l_sweep_cont_stop_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_stop_cntdwn = -1;
	}

	// SWEEP-CONT INC カウントダウン中にデータが INC になった場合
	if (m_l_sweep_cont_inc_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "INC") == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_inc_cntdwn = -1;
	}
	// SWEEP-CONT INC カウントダウンタイムリミットになった場合
	else if (m_l_sweep_cont_inc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_inc_cntdwn = -1;
	}

	// SWEEP-CONT INC+DEC カウントダウン中にデータが INC+DEC になった場合
	if (m_l_sweep_cont_incdec_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "INC+DEC") == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_incdec_cntdwn = -1;
	}
	// SWEEP-CONT INC+DEC カウントダウンタイムリミットになった場合
	else if (m_l_sweep_cont_incdec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_incdec_cntdwn = -1;
	}

	// SWEEP-CONT DEC カウントダウン中にデータが DEC になった場合
	if (m_l_sweep_cont_dec_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "DEC") == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_dec_cntdwn = -1;
	}
	// SWEEP-CONT DEC カウントダウンタイムリミットになった場合
	else if (m_l_sweep_cont_dec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_dec_cntdwn = -1;
	}

	// SWEEP-CONT DEC+INC カウントダウン中にデータが DEC+INC になった場合
	if (m_l_sweep_cont_decinc_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "DEC+INC") == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_decinc_cntdwn = -1;
	}
	// SWEEP-CONT DEC+INC カウントダウンタイムリミットになった場合
	else if (m_l_sweep_cont_decinc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sweep_cont_decinc_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_sweep_cont_stop_cntdwn == -1 && m_l_sweep_cont_inc_cntdwn == -1 &&
		m_l_sweep_cont_incdec_cntdwn == -1 && m_l_sweep_cont_dec_cntdwn == -1 &&
		m_l_sweep_cont_decinc_cntdwn == -1)
	{

		// INC の場合
		if (strcmp(m_sz_sweep_cont_x, "INC") == 0)
		{
			show_sweep_cont_inc34();
		}

		// INC+DEC の場合
		else if (strcmp(m_sz_sweep_cont_x, "INC+DEC") == 0)
		{
			show_sweep_cont_incdec34();
		}

		// DEC+INC の場合
		else if (strcmp(m_sz_sweep_cont_x, "DEC+INC") == 0)
		{
			show_sweep_cont_decinc34();
		}

		// DEC の場合
		else if (strcmp(m_sz_sweep_cont_x, "DEC") == 0)
		{
			show_sweep_cont_dec34();
		}

		// STOP の場合
		else
		{
			show_sweep_cont_stop34();
		}
	}

	/*** RAMP-CONT <<XUP>> 追加 ***********************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RATE_AID_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rate_aid_x, sizeof(m_sz_rate_aid_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_RATE_AID_X), CString(m_sz_rate_aid_x), nLogEx::debug);
	}

	// RAMP-CONT OFF カウントダウン中にデータが OFF になった場合
	if (m_l_ramp_cont_off_cntdwn >= 0 &&
		strcmp(m_sz_rate_aid_x, "OFF") == 0)
	{
		// カウントダウン終了
		m_l_ramp_cont_off_cntdwn = -1;
	}
	// RAMP-CONT OFF カウントダウンタイムリミットになった場合
	else if (m_l_ramp_cont_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_ramp_cont_off_cntdwn = -1;
	}

	// RAMP-CONT ON カウントダウン中にデータが ON になった場合
	if (m_l_ramp_cont_on_cntdwn >= 0 &&
		strcmp(m_sz_rate_aid_x, "ON") == 0)
	{
		// カウントダウン終了
		m_l_ramp_cont_on_cntdwn = -1;
	}
	// RAMP-CONT ON カウントダウンタイムリミットになった場合
	else if (m_l_ramp_cont_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_ramp_cont_on_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_ramp_cont_off_cntdwn == -1 && m_l_ramp_cont_on_cntdwn == -1)
	{
		// ON の場合
		if (strcmp(m_sz_rate_aid_x, "ON") == 0)
		{
			show_ramp_cont_on34();
		}
		// OFF の場合
		else
		{
			show_ramp_cont_off34();
		}
	}

	/*** CMD-MOD **************************************************************************/
	// 現在の状態を監視ＤＢから取得 <<XUP>> X-TX用に変更
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_CMD_MOD_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_cmd_mod_x, sizeof(m_sz_cmd_mod_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-TX.CMD_MOD"), CString(m_sz_cmd_mod_x), nLogEx::debug);
	}
	// CMD-MOD OFF カウントダウン中にデータが OFF になった場合
	if (m_l_cmd_mod_off_cntdwn >= 0 &&
		strcmp(m_sz_cmd_mod_x, "OFF") == 0)
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
		strcmp(m_sz_cmd_mod_x, "ON") == 0)
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
		if (strcmp(m_sz_cmd_mod_x, "ON") == 0)
		{
			show_cmd_mod_on34();
		}
		// OFF の場合
		else
		{
			show_cmd_mod_off34();
		}
	}

	/*** RNG-MOD **************************************************************************/
	// 現在の状態を監視ＤＢから取得 <<XUP>> X-TX用に変更
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RNG_MOD_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rng_mod_x, sizeof(m_sz_rng_mod_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-TX.RNG_MOD"), CString(m_sz_rng_mod_x), nLogEx::debug);
	}

	// RNG-MOD OFF カウントダウン中にデータが OFF になった場合
	if (m_l_rng_mod_off_cntdwn >= 0 &&
		strcmp(m_sz_rng_mod_x, "OFF") == 0)
	{
		// カウントダウン終了
		m_l_rng_mod_off_cntdwn = -1;
	}
	// RNG-MOD OFF カウントダウンタイムリミットになった場合
	else if (m_l_rng_mod_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rng_mod_off_cntdwn = -1;
	}

	// RNG-MOD ON カウントダウン中にデータが ON になった場合
	if (m_l_rng_mod_on_cntdwn >= 0 &&
		strcmp(m_sz_rng_mod_x, "ON") == 0)
	{
		// カウントダウン終了
		m_l_rng_mod_on_cntdwn = -1;
	}
	// RNG-MOD ON カウントダウンタイムリミットになった場合
	else if (m_l_rng_mod_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rng_mod_on_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_rng_mod_off_cntdwn == -1 && m_l_rng_mod_on_cntdwn == -1)
	{
		// ON の場合
		if (strcmp(m_sz_rng_mod_x, "ON") == 0)
		{
			show_rng_mod_on34();
		}
		// OFF の場合
		else
		{
			show_rng_mod_off34();
		}
	}

	/*** TCRMOD MODULATION ****************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_TCRMOD_STS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_tcrmod_sts, sizeof(m_sz_xx_tcrmod_sts), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_XX_TCRMOD_STS), CString(m_sz_xx_tcrmod_sts), nLogEx::debug);
	}
	// TCRMOD MODULATION OFF カウントダウン中にデータが OFF になった場合
	if (m_l_modulation_off_cntdwn >= 0 &&
		strcmp(m_sz_xx_tcrmod_sts, "OFF") == 0)
	{
		// カウントダウン終了
		m_l_modulation_off_cntdwn = -1;
	}
	// TCRMOD MODULATION OFF カウントダウンタイムリミットになった場合
	else if (m_l_modulation_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_modulation_off_cntdwn = -1;
	}

	// TCRMOD MODULATION ON カウントダウン中にデータが ON になった場合
	if (m_l_modulation_on_cntdwn >= 0 &&
		strcmp(m_sz_xx_tcrmod_sts, "ON") == 0)
	{
		// カウントダウン終了
		m_l_modulation_on_cntdwn = -1;
	}
	// TCRMOD MODULATION ON カウントダウンタイムリミットになった場合
	else if (m_l_modulation_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_modulation_on_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_modulation_off_cntdwn == -1 && m_l_modulation_on_cntdwn == -1)
	{
		// ON の場合
		if (strcmp(m_sz_xx_tcrmod_sts, "ON") == 0)
		{
			show_modulation_on34();
		}
		// OFF の場合
		else
		{
			show_modulation_off34();
		}
	}

	/*** X-CMDOUT HOLD_TONE ***************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_HOLD_TONE_STS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hold_tone_sts_x, sizeof(m_sz_hold_tone_sts_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X34_HOLD_TONE_STS_X), CString(m_sz_hold_tone_sts_x), nLogEx::debug);
	}

	// X-CMDOUT HOLD_TONE OFF カウントダウン中にデータが OFF になった場合
	if (m_l_hold_tone_off_cntdwn >= 0 &&
		strcmp(m_sz_hold_tone_sts_x, "OFF") == 0)
	{
		// カウントダウン終了
		m_l_hold_tone_off_cntdwn = -1;
	}
	// X-CMDOUT HOLD_TONE OFF カウントダウンタイムリミットになった場合
	else if (m_l_hold_tone_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_hold_tone_off_cntdwn = -1;
	}

	// X-CMDOUT HOLD_TONE ON カウントダウン中にデータが ON になった場合
	if (m_l_hold_tone_on_cntdwn >= 0 &&
		strcmp(m_sz_hold_tone_sts_x, "ON") == 0)
	{
		// カウントダウン終了
		m_l_hold_tone_on_cntdwn = -1;
	}
	// TCRMOD MODULATION ON カウントダウンタイムリミットになった場合
	else if (m_l_hold_tone_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_hold_tone_on_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_hold_tone_off_cntdwn == -1 && m_l_hold_tone_on_cntdwn == -1)
	{
		// ON の場合
		if (strcmp(m_sz_hold_tone_sts_x, "ON") == 0)
		{
			show_hold_tone_on34();
		}
		// OFF の場合
		else
		{
			show_hold_tone_off34();
		}
	}

	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	/*** SS-RNG / SX-RNG ******************************************************************/
	/*** 数値表示部最新データ表示 *********************************************************/
	// 現在の状態を監視ＤＢから取得 
	// 選ばれているレンジ装置が従来型か再生型かでRNG回数の表示元を切り替える
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_NORM_REP));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_NORM_REP), CString(m_sz_norm_rep), nLogEx::debug);
	}

	// 再生型が選ばれている場合
	if (strcmp(m_sz_norm_rep, "REPLAY") == 0)
	{
		// 再生型測距回数を抽出
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RX_RNG_CNT));
		if (obs_adr != NULL)
		{
			m_d_xx_rng_cnt = obs_adr->d_data;
		}
	}
	// 従来型が選ばれている場合
	else if (strcmp(m_sz_norm_rep, "NORMAL") == 0)
	{
		// 従来型測距回数を抽出
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_RNG_CNT));
		if (obs_adr != NULL)
		{
			m_d_xx_rng_cnt = obs_adr->d_data;
		}
	}
	// 何れでもない場合は０クリア
	else
	{
		m_d_xx_rng_cnt = 0;
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_RNG_NUM));
	if (obs_adr != NULL)
	{
		m_d_xx_rng_num = obs_adr->d_data;
	}

	// 数値表示部を更新
	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_xx_rng_cnt % 100);
	m_strXXRng = CString(sz_buff);

	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_xx_rng_num % 100);
	m_XXRngNumBtn.SetWindowTextW(CString(sz_buff));

	/*** "CONTINUATION" 表示有無 および センシティブ操作 **********************************/
	// 現在の状態を監視ＤＢから取得 
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_CONT_FLAG_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_cont_flag_x, sizeof(m_sz_cont_flag_x), obs_adr->sz_sts_name);
	}

	// 近地球衛星の場合
	if (strcmp(m_sz_cont_flag_x, "NEAR") == 0)
	{
		// XX-RNG の数値表示部上に "CONTINUATION" と表示
		m_strXXRngCont = _T("CONTINUATION");

		// 数値表示部のセンシティブを落とす
		m_XXRngNumBtn.EnableWindow(FALSE);
	}
	// 深宇宙衛星の場合
	else
	{
		// XX-RNG の数値表示部上の "CONTINUATION" を消す（あれば）
		m_strXXRngCont = _T("");

		// 数値表示部のセンシティブを付ける
		m_XXRngNumBtn.EnableWindow(TRUE);
	}


	/*** COH/INCOH ************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_COH_INCOH));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_coh_incoh, sizeof(m_sz_coh_incoh), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("V-EQU.COHI"), CString(m_sz_coh_incoh), nLogEx::debug);
	}

	// COH/INCOH COH カウントダウン中にデータが COH になった場合
	if (m_l_coh_incoh_coh_cntdwn >= 0 &&
		strcmp(m_sz_coh_incoh, "COHI") == 0)
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
		strcmp(m_sz_coh_incoh, "INCOHI") == 0)
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
			show_coh_incoh_incoh34();
		}
		// COH の場合
		else
		{
			show_coh_incoh_coh34();
		}
	}


	/*** RNGING ***************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("CTRL.RNG_FLG"), CString(m_sz_rnging), nLogEx::debug);
	}

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

	// (再生型) RNGING STOP カウントダウン中にデータが STOP になった場合
	if (m_l_rep_rnging_stop_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "終了") == 0)
	{
		// カウントダウン終了
		m_l_rep_rnging_stop_cntdwn = -1;
	}
	// (再生型) RNGING STOP カウントダウンタイムリミットになった場合
	else if (m_l_rep_rnging_stop_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rep_rnging_stop_cntdwn = -1;
	}

	// (再生型) RNGING START カウントダウン中にデータが START になった場合
	if (m_l_rep_rnging_start_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "開始") == 0)
	{
		// カウントダウン終了
		m_l_rep_rnging_start_cntdwn = -1;
	}
	// (再生型) RNGING START カウントダウンタイムリミットになった場合
	else if (m_l_rep_rnging_start_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rep_rnging_start_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_rnging_stop_cntdwn == -1 && m_l_rnging_start_cntdwn == -1)
	{
		// START の場合
		if (strcmp(m_sz_rnging, "開始") == 0)
		{
			show_rnging_start34();
		}
		// STOP の場合
		else
		{
			show_rnging_stop34();
		}
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_rep_rnging_stop_cntdwn == -1 && m_l_rep_rnging_start_cntdwn == -1)
	{
		// START の場合
		if (strcmp(m_sz_rnging, "開始") == 0)
		{
			show_rrng_start34();
		}
		// STOP の場合
		else
		{
			show_rrng_stop34();
		}
	}

	/*** 変調装置の選択状態確認 ***********************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_MOD_IN_SEL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_mod_in_sel, sizeof(m_sz_mod_in_sel), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X34_MOD_IN_SEL), CString(m_sz_mod_in_sel), nLogEx::debug);
	}

	// TCRMODが選択されている場合
	if (strcmp(m_sz_mod_in_sel, "TCRMOD") == 0)
	{
		// TCRMODをセンシティブ、PM MODをインセンシティブにする
		m_ModulationOffBtn.EnableWindow(TRUE);
		m_ModulationOnBtn.EnableWindow(TRUE);
		m_HoldToneOffBtn.EnableWindow(TRUE);
		m_HoldToneOnBtn.EnableWindow(TRUE);
		m_CmdModOffBtn.EnableWindow(FALSE);
		m_CmdModOnBtn.EnableWindow(FALSE);
		m_RngModOffBtn.EnableWindow(FALSE);
		m_RngModOnBtn.EnableWindow(FALSE);
	}
	// PM MODが選択されている場合
	else if (strcmp(m_sz_mod_in_sel, "MOD") == 0)
	{
		// TCRMODをインセンシティブ、PM MODをセンシティブにする
		m_ModulationOffBtn.EnableWindow(FALSE);
		m_ModulationOnBtn.EnableWindow(FALSE);
		m_HoldToneOffBtn.EnableWindow(FALSE);
		m_HoldToneOnBtn.EnableWindow(FALSE);
		m_CmdModOffBtn.EnableWindow(TRUE);
		m_CmdModOnBtn.EnableWindow(TRUE);
		m_RngModOffBtn.EnableWindow(TRUE);
		m_RngModOnBtn.EnableWindow(TRUE);
	}
	// いづれの状態でもない場合
	else
	{
		// どちらもインセンシティブにする
		m_ModulationOffBtn.EnableWindow(FALSE);
		m_ModulationOnBtn.EnableWindow(FALSE);
		m_HoldToneOffBtn.EnableWindow(FALSE);
		m_HoldToneOnBtn.EnableWindow(FALSE);
		m_CmdModOffBtn.EnableWindow(FALSE);
		m_CmdModOnBtn.EnableWindow(FALSE);
		m_RngModOffBtn.EnableWindow(FALSE);
		m_RngModOnBtn.EnableWindow(FALSE);
	}

	UpdateData(FALSE);
}

/*============================================================================*/
/*! CTrsControlSX

-# カウントダウンタイムアウト

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::count_down()
{
	TRACE(_T("count_down\n"));
	KillTimer(TID_COUNTDOWN);

	// 0 以上はカウントダウン

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

	// SWEEP-CONT STOP
	if (m_l_sweep_cont_stop_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT STOP カウントダウン:%d\n"), m_l_sweep_cont_stop_cntdwn);
		m_l_sweep_cont_stop_cntdwn--;
	}

	// SWEEP-CONT INC
	if (m_l_sweep_cont_inc_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT INC カウントダウン:%d\n"), m_l_sweep_cont_inc_cntdwn);
		m_l_sweep_cont_inc_cntdwn--;
	}

	// SWEEP-CONT INC+DEC
	if (m_l_sweep_cont_incdec_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT INC+DEC カウントダウン:%d\n"), m_l_sweep_cont_incdec_cntdwn);
		m_l_sweep_cont_incdec_cntdwn--;
	}

	// SWEEP-CONT DEC
	if (m_l_sweep_cont_dec_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT DEC カウントダウン:%d\n"), m_l_sweep_cont_dec_cntdwn);
		m_l_sweep_cont_dec_cntdwn--;
	}

	// SWEEP-CONT DEC+INC
	if (m_l_sweep_cont_decinc_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT DEC+INCC カウントダウン:%d\n"), m_l_sweep_cont_decinc_cntdwn);
		m_l_sweep_cont_decinc_cntdwn--;
	}

	// RAMP-CONT OFF
	if (m_l_ramp_cont_off_cntdwn > 0) 
	{
		TRACE(_T("RAMP-CONT OFF カウントダウン:%d\n"), m_l_ramp_cont_off_cntdwn);
		m_l_ramp_cont_off_cntdwn--;
	}

	// RAMP-CONT ON
	if (m_l_ramp_cont_on_cntdwn > 0) 
	{
		TRACE(_T("RAMP-CONT ON カウントダウン:%d\n"), m_l_ramp_cont_on_cntdwn);
		m_l_ramp_cont_on_cntdwn--;
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

	// TCRMOD MODULATION ON
	if (m_l_modulation_on_cntdwn > 0)
	{
		TRACE(_T("TCRMOD MODULATION ON カウントダウン:%d\n"), m_l_modulation_on_cntdwn);
		m_l_modulation_on_cntdwn--;
	}

	// TCRMOD MODULATION OFF
	if (m_l_modulation_off_cntdwn > 0)
	{
		TRACE(_T("TCRMOD MODULATION OFF カウントダウン:%d\n"), m_l_modulation_off_cntdwn);
		m_l_modulation_off_cntdwn--;
	}

	// X-CMDOUT HOLD TONE ON
	if (m_l_hold_tone_on_cntdwn > 0)
	{
		TRACE(_T("X-CMDOUT HOLD TONE ON カウントダウン:%d\n"), m_l_hold_tone_on_cntdwn);
		m_l_hold_tone_on_cntdwn--;
	}

	// X-CMDOUT HOLD TONE OFF
	if (m_l_hold_tone_off_cntdwn > 0) {
		TRACE(_T("X-CMDOUT HOLD TONE OFF カウントダウン:%d\n"), m_l_hold_tone_off_cntdwn);
		m_l_hold_tone_off_cntdwn--;
	}

	/*-- RNG CONTROL ---------------------------------------------------------------------*/

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

	// RNGING STOP (従来型)
	if (m_l_rnging_stop_cntdwn > 0) 
	{
		TRACE(_T("RNGING STOP (従来型) カウントダウン:%d\n"), m_l_rnging_stop_cntdwn);
		m_l_rnging_stop_cntdwn--;
	}

	// RNGING START (従来型)
	if (m_l_rnging_start_cntdwn > 0) 
	{
		TRACE(_T("RNGING START (従来型) カウントダウン:%d\n"), m_l_rnging_start_cntdwn);
		m_l_rnging_start_cntdwn--;
	}

	// RNGING STOP (再生型)
	if (m_l_rep_rnging_stop_cntdwn > 0) 
	{
		TRACE(_T("RNGING STOP (再生型) カウントダウン:%d\n"), m_l_rep_rnging_stop_cntdwn);
		m_l_rep_rnging_stop_cntdwn--;
	}

	// RNGING START (再生型)
	if (m_l_rep_rnging_start_cntdwn > 0) 
	{
		TRACE(_T("RNGING START (再生型) カウントダウン:%d\n"), m_l_rep_rnging_start_cntdwn);
		m_l_rep_rnging_start_cntdwn--;
	}

	SetTimer(TID_COUNTDOWN, INTERVAL_COUNTDOWN, 0);
}


/*============================================================================*/
/*! CTrsControlX

-# RF-POWER OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_rf_power_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// OFF側
	if ((strcmp(m_sz_lan_x_tx, "接続") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "接続") == 0)) 
	{
		// リモート（選択）
		m_RFPWROffBtn.SetColor(COLOR_REMOTES);
	}
	else 
	{
		// ローカル
		m_RFPWROffBtn.SetColor(COLOR_LOCAL);
	}

	// ON側
	if ((strcmp (m_sz_x_ul_cnt_rl , "REMOTE") == 0) &&  //追加  2017/05/08  as-04
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&  //追加  2017/05/08  as-04
		(strcmp(m_sz_rf_on_enbl_x, "ON") == 0) &&
		(strcmp(m_sz_lan_x_tx, "接続") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") == 0) &&  //追加  2017/05/08  as-04
		(strcmp(m_sz_lan_x_rf_if, "接続") == 0) &&  //追加  2017/05/08  as-04
		(m_i_priv_flag == 1)) 
	{
		// S-TXのLAN接続状態を確認する
		if (strcmp(m_sz_lan_s_tx, "接続") == 0) 
		{
			// S-TX RF DET ANS を確認する
			if (strcmp(m_sz_rf_on_det_ans_s, "OFF") == 0) 
			{
				// リモート（非選択）
				m_RFPWROnBtn.SetColor(COLOR_REMOTE);
			}
			else 
			{
				// ローカル
				m_RFPWROnBtn.SetColor(COLOR_LOCAL);
			}
		}
		else 
		{
			// リモート（非選択）
			m_RFPWROnBtn.SetColor(COLOR_REMOTE);
		}
	}
	else 
	{
		// ローカル
		m_RFPWROnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RFPWROffBtn.Invalidate();
	m_RFPWROnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlX

-# RF-POWER OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_rf_power_off34()
{
	char	szRfOnEnblSts[32];

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

	// HPA側のRF状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_HPA_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hpa_rf_on_enbl_x, sizeof(m_sz_hpa_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	// HPA/SSPAで処理を分ける
	if (strcmp(m_sz_xpa_sel, "HPA") == 0)
	{
		// HPAが選択されている場合
		strcpy_s(szRfOnEnblSts, sizeof(szRfOnEnblSts), m_sz_hpa_rf_on_enbl_x);
	}
	else
	{
		// SSPAが選択されている場合
		strcpy_s(szRfOnEnblSts, sizeof(szRfOnEnblSts), m_sz_rf_on_enbl_x);
	}

	if (strcmp(m_sz_lan_x_tx, "接続") == 0)
	{
		// リモート（選択） 
		m_RFPWROffBtn.SetColor(COLOR_REMOTES);
	}
	else
	{
		// ローカル
		m_RFPWROffBtn.SetColor(COLOR_LOCAL);
	}

	// ON側
	if ((strcmp(m_sz_x_tx_rl, "REMOTE") == 0) &&
		(strcmp(szRfOnEnblSts, "ON") == 0) &&
		(strcmp(m_sz_lan_x_tx, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// S-TXのLAN接続状態を確認する
		if (strcmp(m_sz_lan_s_tx, "接続") == 0)
		{
			// S-TX RF DET ANS を確認する
			if (strcmp(m_sz_rf_on_det_ans_s, "OFF") == 0)
			{
				// リモート（非選択）
				m_RFPWROnBtn.SetColor(COLOR_REMOTE);
			}
			else
			{
				// ローカル
				m_RFPWROnBtn.SetColor(COLOR_LOCAL);
			}
		}
		else
		{
			// リモート（非選択）
			m_RFPWROnBtn.SetColor(COLOR_REMOTE);
		}
	}
	else
	{
		// ローカル
		m_RFPWROnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RFPWROffBtn.Invalidate();
	m_RFPWROnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RF-POWER ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_rf_power_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RF_ON_DET_ANS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans_x, sizeof(m_sz_rf_on_det_ans_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	// HPA-2 増設対応
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_HPA_SEL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hpa_sel, sizeof(m_sz_hpa_sel), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RF_ON_DET_ANS_X_H2));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans_x_h2, sizeof(m_sz_rf_on_det_ans_x_h2), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// 緑半灯条件
	if ((strcmp(m_sz_lan_x_tx, "接続") != 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") != 0) &&
		(strcmp(m_sz_lan_x_rf_if, "接続") != 0))
	{
		// ローカル
		m_RFPWROnBtn.SetColor(COLOR_LOCAL);
	}
	else if (((strcmp(m_sz_x_ul_cnt_rl, "LOCAL") == 0)) &&
		(((strcmp(m_sz_hpa_sel, "NO1") == 0) && (strcmp(m_sz_rf_on_det_ans_x, "OFF") == 0))
		|| ((strcmp(m_sz_hpa_sel, "NO2") == 0) && (strcmp(m_sz_rf_on_det_ans_x_h2, "OFF") == 0)))) 
	{
		// リモート（選択）／ ローカル
		m_RFPWROnBtn.SetColor(COLOR_REMOTES, COLOR_LOCAL, TRUE);
	}
	else if (((strcmp(m_sz_hpa_sel, "NO1") == 0) && (strcmp(m_sz_rf_on_det_ans_x, "OFF") == 0))
		|| ((strcmp(m_sz_hpa_sel, "NO2") == 0) && (strcmp(m_sz_rf_on_det_ans_x_h2, "OFF") == 0))) 
	{
		// リモート（選択）／ リモート（非選択）
		m_RFPWROnBtn.SetColor(COLOR_REMOTES, COLOR_REMOTE, TRUE);
	}
	else 
	{
		// リモート（選択）
		m_RFPWROnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF 側ボタンの設定
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&  //追加  2017/05/08  as-04
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&  //追加  2017/05/08  as-04
		(strcmp(m_sz_rf_on_enbl_x, "OFF") == 0) &&
		(strcmp(m_sz_lan_x_tx, "接続") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") == 0) &&  //追加  2017/05/08  as-04
		(strcmp(m_sz_lan_x_rf_if, "接続") == 0) &&  //追加  2017/05/08  as-04
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_RFPWROffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_RFPWROffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RFPWROffBtn.Invalidate();
	m_RFPWROnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlX

-# RF-POWER ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_rf_power_on34()
{
	char	szRfOnEnblSts[32];
	char	szRfOnDetAnsSts[32];

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_RF_ON_DET_ANS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans_x, sizeof(m_sz_rf_on_det_ans_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	// HPA側のRF状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_HPA_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hpa_rf_on_enbl_x, sizeof(m_sz_hpa_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_HPA_RF_ON_DET_ANS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hpa_rf_on_det_ans_x, sizeof(m_sz_hpa_rf_on_det_ans_x), obs_adr->sz_sts_name);
	}

	// HPA/SSPAで処理を分ける
	if (strcmp(m_sz_xpa_sel, "HPA") == 0)
	{
		// HPAが選択されている場合
		strcpy_s(szRfOnEnblSts, sizeof(szRfOnEnblSts), m_sz_hpa_rf_on_enbl_x);
		strcpy_s(szRfOnDetAnsSts, sizeof(szRfOnDetAnsSts), m_sz_hpa_rf_on_det_ans_x);
	}
	else
	{
		// SSPAが選択されている場合
		strcpy_s(szRfOnEnblSts, sizeof(szRfOnEnblSts), m_sz_rf_on_enbl_x);
		strcpy_s(szRfOnDetAnsSts, sizeof(szRfOnDetAnsSts), m_sz_rf_on_det_ans_x);
	}

	// 緑半灯条件
	if (strcmp(m_sz_lan_x_tx, "接続") != 0)
	{
		// ローカル
		m_RFPWROnBtn.SetColor(COLOR_LOCAL);
	}
	else if (((strcmp(m_sz_x_tx_rl, "LOCAL") == 0) || (strcmp(szRfOnEnblSts, "OFF") == 0)) && (strcmp(szRfOnDetAnsSts, "OFF") == 0))
	{
		// リモート（選択）／ ローカル
		m_RFPWROnBtn.SetColor(COLOR_REMOTES, COLOR_LOCAL, TRUE);
	}
	else if (strcmp(szRfOnDetAnsSts, "OFF") == 0)
	{
		// リモート（選択）／ リモート（非選択）
		m_RFPWROnBtn.SetColor(COLOR_REMOTES, COLOR_REMOTE, TRUE);
	}
	else
	{
		// リモート（選択）
		m_RFPWROnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF 側ボタンの設定
	if ((strcmp(m_sz_x_tx_rl, "REMOTE") == 0) &&
		(strcmp(szRfOnEnblSts, "OFF") == 0) &&
		(strcmp(m_sz_lan_x_tx, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_RFPWROffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_RFPWROffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RFPWROffBtn.Invalidate();
	m_RFPWROnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlX

-# NO-USE OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_no_use_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// OFF 側
	if ((strcmp(m_sz_lan_x_tx, "接続") != 0)&&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") != 0) &&
		(strcmp(m_sz_lan_x_rf_if, "接続") != 0))
	{
		// ローカル
		m_NoUseOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_NoUseOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON 側
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&  //追加  2017/05/08  as-04
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&  //追加  2017/05/08  as-04
		(strcmp(m_sz_lan_x_tx, "接続") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") == 0) &&  //追加  2017/05/08  as-04
		(strcmp(m_sz_lan_x_rf_if, "接続") == 0) &&  //追加  2017/05/08  as-04
		(m_i_priv_flag == 1))
	{
		// リモート時
		m_NoUseOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル時
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_NoUseOffBtn.Invalidate();
	m_NoUseOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# NO-USE OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_no_use_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_tx, "接続") != 0)
	{
		// ローカル
		m_NoUseOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_NoUseOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON 側
	if ((strcmp(m_sz_x_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_tx, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート時
		m_NoUseOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル時
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_NoUseOffBtn.Invalidate();
	m_NoUseOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# NO-USE ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_no_use_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// OF 側
	if ((strcmp(m_sz_lan_x_tx, "接続") != 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") != 0) &&   //追加  2017/05/08  as-04
		(strcmp(m_sz_lan_x_rf_if, "接続") != 0))
	{
		// ローカル
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_NoUseOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF 側
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&  //追加  2017/05/08  as-04
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&  //追加  2017/05/08  as-04
		(strcmp(m_sz_lan_x_tx, "接続") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") == 0) &&  //追加  2017/05/08  as-04
		(strcmp(m_sz_lan_x_rf_if, "接続") == 0) &&  //追加  2017/05/08  as-04
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_NoUseOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_NoUseOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_NoUseOffBtn.Invalidate();
	m_NoUseOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# NO-USE ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_no_use_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_tx, "接続") != 0)
	{
		// ローカル
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_NoUseOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF 側
	if ((strcmp(m_sz_x_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_tx, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_NoUseOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_NoUseOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_NoUseOffBtn.Invalidate();
	m_NoUseOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_stop64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_SweepCountStopBtn.SetColor(COLOR_REMOTES);
	}

	// その他
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_SweepCountIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_stop34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountStopBtn.SetColor(COLOR_REMOTES);
	}

	// その他
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT INC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_inc64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), "接続");
#endif

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_SweepCountIncBtn.SetColor(COLOR_REMOTES);
	}

	// その他
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_SweepCountStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT INC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_inc34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountIncBtn.SetColor(COLOR_REMOTES);
	}

	// その他
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT INCDEC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_incdec64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// その他
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT INCDEC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_incdec34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// その他
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT DEC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_dec64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), "接続");
#endif

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_SweepCountDecBtn.SetColor(COLOR_REMOTES);
	}

	// その他
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT DEC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_dec34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountDecBtn.SetColor(COLOR_REMOTES);
	}

	// その他
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT DECINC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_decinc64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// その他
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT DECINC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_decinc34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// その他
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RAMP-CONT OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_ramp_cont_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_X_TXSYN_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_txsyn_rl, sizeof(m_sz_x_txsyn_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_RampContOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_RampContOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON 側
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "接続") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_RampContOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_RampContOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RampContOffBtn.Invalidate();
	m_RampContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RAMP-CONT OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_ramp_cont_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_X_TXSYN_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_txsyn_rl, sizeof(m_sz_x_txsyn_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_RampContOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_RampContOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON 側
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_RampContOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_RampContOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RampContOffBtn.Invalidate();
	m_RampContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RAMP-CONT ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_ramp_cont_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_X_TXSYN_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_txsyn_rl, sizeof(m_sz_x_txsyn_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_x_txsyn_rl, sizeof(m_sz_x_txsyn_rl), "REMOTE");
	strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), "接続");
#endif

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_RampContOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_RampContOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF 側
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "接続") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_RampContOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_RampContOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RampContOffBtn.Invalidate();
	m_RampContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RAMP-CONT ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_ramp_cont_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_X_TXSYN_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_txsyn_rl, sizeof(m_sz_x_txsyn_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_RampContOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_RampContOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF 側
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_RampContOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_RampContOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RampContOffBtn.Invalidate();
	m_RampContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# CMD-MOD OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_cmd_mod_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_TCRMOD_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_tcrmod_rl, sizeof(m_sz_xx_tcrmod_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_XX_TCRMOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_tcrmod, sizeof(m_sz_lan_xx_tcrmod), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_x_tx_rl, sizeof(m_sz_x_tx_rl), "REMOTE");
	strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), "接続");
#endif

	// OFF 側
	if (strcmp(m_sz_lan_xx_tcrmod, "接続") != 0)
	{
		// ローカル
		m_CmdModOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_CmdModOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON 側
	if ((strcmp(m_sz_xx_tcrmod_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_xx_tcrmod, "接続") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_CmdModOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_CmdModOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# CMD-MOD OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_cmd_mod_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_X_TX_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_tx_rl, sizeof(m_sz_x_tx_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_x_tx_rl, sizeof(m_sz_x_tx_rl), "REMOTE");
	strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), "接続");
#endif

	if (strcmp(m_sz_lan_x_tx, "接続") != 0)
	{
		// ローカル
		m_CmdModOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_CmdModOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON 側
	if ((strcmp(m_sz_x_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_tx, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_CmdModOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_CmdModOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# CMD-MOD ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_cmd_mod_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_TCRMOD_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_tcrmod_rl, sizeof(m_sz_xx_tcrmod_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_XX_TCRMOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_tcrmod, sizeof(m_sz_lan_xx_tcrmod), obs_adr->sz_sts_name);
	}

	// ON 側
	if (strcmp(m_sz_lan_xx_tcrmod, "接続") != 0)
	{
		// ローカル
		m_CmdModOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_CmdModOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF 側
	if ((strcmp(m_sz_xx_tcrmod_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_xx_tcrmod, "接続") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_CmdModOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_CmdModOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# CMD-MOD ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_cmd_mod_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_X_TX_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_tx_rl, sizeof(m_sz_x_tx_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_tx, "接続") != 0)
	{
		// ローカル
		m_CmdModOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_CmdModOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF 側
	if ((strcmp(m_sz_x_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_tx, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_CmdModOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_CmdModOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNG-MOD OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_rng_mod_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_TCRMOD_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_tcrmod_rl, sizeof(m_sz_xx_tcrmod_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_XX_TCRMOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_tcrmod, sizeof(m_sz_lan_xx_tcrmod), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_xx_tcrmod, "接続") != 0)
	{
		// ローカル
		m_RngModOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（非選択）
		m_RngModOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON 側
	if ((strcmp(m_sz_xx_tcrmod_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_xx_tcrmod, "接続") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_RngModOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_RngModOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RngModOffBtn.Invalidate();
	m_RngModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNG-MOD OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_rng_mod_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_X_TX_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_tx_rl, sizeof(m_sz_x_tx_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_tx, "接続") != 0)
	{
		// ローカル
		m_RngModOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（非選択）
		m_RngModOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON 側
	if ((strcmp(m_sz_x_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_tx, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_RngModOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_RngModOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RngModOffBtn.Invalidate();
	m_RngModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNG-MOD ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_rng_mod_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_TCRMOD_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_tcrmod_rl, sizeof(m_sz_xx_tcrmod_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_XX_TCRMOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_tcrmod, sizeof(m_sz_lan_xx_tcrmod), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_xx_tcrmod, "接続") != 0)
	{
		// ローカル
		m_RngModOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_RngModOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF 側
	if ((strcmp(m_sz_xx_tcrmod_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_xx_tcrmod, "接続") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_RngModOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_RngModOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RngModOffBtn.Invalidate();
	m_RngModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNG-MOD ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_rng_mod_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_X_TX_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_tx_rl, sizeof(m_sz_x_tx_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_tx, "接続") != 0)
	{
		// ローカル
		m_RngModOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_RngModOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF 側
	if ((strcmp(m_sz_x_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_tx, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_RngModOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_RngModOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RngModOffBtn.Invalidate();
	m_RngModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# MODULATION OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_modulation_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_TCRMOD_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_tcrmod_rl, sizeof(m_sz_xx_tcrmod_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_XX_TCRMOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_tcrmod, sizeof(m_sz_lan_xx_tcrmod), obs_adr->sz_sts_name);
	}

	// OFF 側
	if (strcmp(m_sz_lan_xx_tcrmod, "接続") != 0) 
	{
		// ローカル
		m_ModulationOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_ModulationOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON 側
	if ((strcmp(m_sz_xx_tcrmod_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_xx_tcrmod, "接続") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_ModulationOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_ModulationOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# MODULATION OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_modulation_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_TCRMOD_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_tcrmod_rl, sizeof(m_sz_xx_tcrmod_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_XX_TCRMOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_tcrmod, sizeof(m_sz_lan_xx_tcrmod), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_xx_tcrmod, "接続") != 0)
	{
		// ローカル
		m_ModulationOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_ModulationOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON 側
	if ((strcmp(m_sz_xx_tcrmod_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_xx_tcrmod, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_ModulationOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_ModulationOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# MODULATION ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_modulation_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_TCRMOD_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_tcrmod_rl, sizeof(m_sz_xx_tcrmod_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_XX_TCRMOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_tcrmod, sizeof(m_sz_lan_xx_tcrmod), obs_adr->sz_sts_name);
	}

	// ON 側
	if (strcmp(m_sz_lan_xx_tcrmod, "接続") != 0) 
	{
		// ローカル
		m_ModulationOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_ModulationOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF 側
	if ((strcmp(m_sz_xx_tcrmod_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_xx_tcrmod, "接続") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_ModulationOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_ModulationOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# MODULATION ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_modulation_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_TCRMOD_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_tcrmod_rl, sizeof(m_sz_xx_tcrmod_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_XX_TCRMOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_tcrmod, sizeof(m_sz_lan_xx_tcrmod), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_xx_tcrmod, "接続") != 0)
	{
		// ローカル
		m_ModulationOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_ModulationOnBtn.SetColor(COLOR_REMOTES);
	}

	// ON 側
	if ((strcmp(m_sz_xx_tcrmod_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_xx_tcrmod, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_ModulationOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_ModulationOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# HOLD-TONE OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_hold_tone_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_CMDOUT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_cmdout, sizeof(m_sz_lan_x_cmdout), obs_adr->sz_sts_name);
	}

	// OFF 側
	if (strcmp(m_sz_lan_x_cmdout, "接続") != 0) 
	{
		// ローカル
		m_HoldToneOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_HoldToneOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON 側
	if ((strcmp(m_sz_lan_x_cmdout, "接続") == 0) && (m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_HoldToneOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_HoldToneOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# HOLD-TONE OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_hold_tone_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_CMDOUT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_cmdout, sizeof(m_sz_lan_x_cmdout), obs_adr->sz_sts_name);
	}

	// OFF 側
	if (strcmp(m_sz_lan_x_cmdout, "接続") != 0)
	{
		// ローカル
		m_HoldToneOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_HoldToneOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON 側
	if ((strcmp(m_sz_lan_x_cmdout, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_HoldToneOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_HoldToneOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# HOLD-TONE ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_hold_tone_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_CMDOUT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_cmdout, sizeof(m_sz_lan_x_cmdout), obs_adr->sz_sts_name);
	}

	// ON 側
	if (strcmp(m_sz_lan_x_cmdout, "接続") != 0) 
	{
		// ローカル
		m_HoldToneOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_HoldToneOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF 側
	if ((strcmp(m_sz_lan_x_cmdout, "接続") == 0) && (m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_HoldToneOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_HoldToneOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# HOLD-TONE ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_hold_tone_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_CMDOUT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_cmdout, sizeof(m_sz_lan_x_cmdout), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_cmdout, "接続") != 0)
	{
		// ローカル
		m_HoldToneOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_HoldToneOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF 側
	if ((strcmp(m_sz_lan_x_cmdout, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_HoldToneOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_HoldToneOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# COH-INCOH COH 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_coh_incoh_coh64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_vequ, "接続") != 0) 
	{
		// ローカル
		m_XXCohCohBtn.SetColor(COLOR_LOCAL);
		m_XXCohIncohBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		if (m_i_priv_flag == 1) 
		{
			// リモート（非選択）
			m_XXCohIncohBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ローカル
			m_XXCohIncohBtn.SetColor(COLOR_LOCAL);
		}

		// リモート（選択）
		m_XXCohCohBtn.SetColor(COLOR_REMOTES);
	}

	// 更新
	m_XXCohCohBtn.Invalidate();
	m_XXCohIncohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# COH-INCOH COH 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_coh_incoh_coh34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_vequ, "接続") != 0)
	{
		// ローカル
		m_XXCohCohBtn.SetColor(COLOR_LOCAL);
		m_XXCohIncohBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		if (m_i_priv_flag == 1)
		{
			// リモート（非選択）
			m_XXCohIncohBtn.SetColor(COLOR_REMOTE);
		}
		else
		{
			// ローカル
			m_XXCohIncohBtn.SetColor(COLOR_LOCAL);
		}

		// リモート（選択）
		m_XXCohCohBtn.SetColor(COLOR_REMOTES);
	}

	// 更新
	m_XXCohCohBtn.Invalidate();
	m_XXCohIncohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# COH-INCOH INCOH 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_coh_incoh_incoh64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_vequ, "接続") != 0) 
	{
		// ローカル
		m_XXCohIncohBtn.SetColor(COLOR_LOCAL);
		m_XXCohCohBtn.SetColor(COLOR_LOCAL);
	}
	else {
		if (m_i_priv_flag == 1) 
		{
			// リモート（非選択）
			m_XXCohCohBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ローカル
			m_XXCohCohBtn.SetColor(COLOR_LOCAL);
		}

		// リモート（選択）
		m_XXCohIncohBtn.SetColor(COLOR_REMOTES);
	}

	// 更新
	m_XXCohCohBtn.Invalidate();
	m_XXCohIncohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# COH-INCOH INCOH 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_coh_incoh_incoh34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_vequ, "接続") != 0)
	{
		// ローカル
		m_XXCohIncohBtn.SetColor(COLOR_LOCAL);
		m_XXCohCohBtn.SetColor(COLOR_LOCAL);
	}
	else {
		if (m_i_priv_flag == 1)
		{
			// リモート（非選択）
			m_XXCohCohBtn.SetColor(COLOR_REMOTE);
		}
		else
		{
			// ローカル
			m_XXCohCohBtn.SetColor(COLOR_LOCAL);
		}

		// リモート（選択）
		m_XXCohIncohBtn.SetColor(COLOR_REMOTES);
	}

	// 更新
	m_XXCohCohBtn.Invalidate();
	m_XXCohIncohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNGING STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_rnging_stop64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_XX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rarr, sizeof(m_sz_lan_xx_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_RARR_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_rarr_cont, sizeof(m_sz_xx_rarr_cont), obs_adr->sz_sts_name);
	}

	// STOP 側
	if (strcmp(m_sz_lan_xx_rarr, "接続") != 0)
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
	// XX-RARR LAN接続時は緑にする
	if ((strcmp(m_sz_lan_xx_rarr, "接続") == 0) &&
		(strcmp(m_sz_xx_rarr_cont, "REMOTE") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_RngingStartBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}

	// インセンシティブの場合は、センシティブにする
	if (m_RngingStartBtn.IsWindowEnabled() == FALSE)
	{
		m_RngingStartBtn.EnableWindow(TRUE);
		m_RngingStopBtn.EnableWindow(TRUE);
	}

	// 更新
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNGING STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_rnging_stop34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_XX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rarr, sizeof(m_sz_lan_xx_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_RARR_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_rarr_cont, sizeof(m_sz_xx_rarr_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_P_FLG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_pass_flag, sizeof(m_sz_pass_flag), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_xx_rarr, "接続") != 0)
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
	// XX-RARR LAN接続時は緑にする
	if ((strcmp(m_sz_lan_xx_rarr, "接続") == 0) &&
		(strcmp(m_sz_pass_flag, "パス中") == 0) &&
		(strcmp(m_sz_xx_rarr_cont, "REMOTE") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_RngingStartBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}

	// インセンシティブの場合は、センシティブにする
	if ((m_RngingStartBtn.IsWindowEnabled() == FALSE) && (m_l_rng_type != RNG_TYPE_REP))
	{
		m_RngingStartBtn.EnableWindow(TRUE);
		m_RngingStopBtn.EnableWindow(TRUE);
	}

	// 更新
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNGING START 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_rnging_start64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_XX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rarr, sizeof(m_sz_lan_xx_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_RARR_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_rarr_cont, sizeof(m_sz_xx_rarr_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_NORM_REP));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
	}

	// 従来型が選ばれていなければ、ボタンをインセンシティブにする
	if ((strcmp(m_sz_norm_rep, "NORMAL") != 0) && (m_RngingStartBtn.IsWindowEnabled() != FALSE))
	{
		m_RngingStartBtn.EnableWindow(FALSE);
		m_RngingStopBtn.EnableWindow(FALSE);
	}
	else if ((strcmp(m_sz_norm_rep, "NORMAL") == 0) && (m_RngingStartBtn.IsWindowEnabled() == FALSE) && (m_l_rng_type == RNG_TYPE_NOR))
	{
		m_RngingStartBtn.EnableWindow(TRUE);
		m_RngingStopBtn.EnableWindow(TRUE);
	}

	if ((strcmp(m_sz_lan_xx_rarr, "接続") != 0) ||
		(strcmp(m_sz_norm_rep, "NORMAL") != 0))
	{
		// ローカル
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_RngingStartBtn.SetColor(COLOR_REMOTES);
	}

	// STOP 側
	// XX-RARR LAN接続時
	if ((strcmp(m_sz_lan_xx_rarr, "接続") == 0) &&
		(strcmp(m_sz_xx_rarr_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_norm_rep, "NORMAL") == 0) &&
		(m_i_priv_flag == 1)) 
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
/*! CTrsControlX

-# RNGING START 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_rnging_start34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_XX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rarr, sizeof(m_sz_lan_xx_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_RARR_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_rarr_cont, sizeof(m_sz_xx_rarr_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_NORM_REP));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
	}

	// 従来型が選ばれていなければ、ボタンをインセンシティブにする
	if ((strcmp(m_sz_norm_rep, "NORMAL") != 0) && (m_RngingStartBtn.IsWindowEnabled() != FALSE))
	{
		m_RngingStartBtn.EnableWindow(FALSE);
		m_RngingStopBtn.EnableWindow(FALSE);
	}
	else if ((strcmp(m_sz_norm_rep, "NORMAL") == 0) && (m_RngingStartBtn.IsWindowEnabled() == FALSE) && (m_l_rng_type == RNG_TYPE_NOR))
	{
		m_RngingStartBtn.EnableWindow(TRUE);
		m_RngingStopBtn.EnableWindow(TRUE);
	}

	if ((strcmp(m_sz_lan_xx_rarr, "接続") != 0) ||
		(strcmp(m_sz_norm_rep, "NORMAL") != 0))
	{
		// ローカル
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_RngingStartBtn.SetColor(COLOR_REMOTES);
	}

	// STOP 側
	// XX-RARR LAN接続時
	if ((strcmp(m_sz_lan_xx_rarr, "接続") == 0) &&
		(strcmp(m_sz_xx_rarr_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_norm_rep, "NORMAL") == 0) &&
		(m_i_priv_flag == 1))
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
/*! CTrsControlX

-# RNGING 再生型 STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_rrng_stop64()
{
	struct satinf_t *satinf_adr;
	struct satinf_t *original_adr;
	char *sattbl_adr;
	struct rarrband_t *rarrband_t_adr;	// ＲＡＲＲ使用帯域情報テーブルアドレス

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_XX_RRNG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rrng, sizeof(m_sz_lan_xx_rrng), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_RRNG_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_rrng_cont, sizeof(m_sz_xx_rrng_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_P_FLG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_pass_flag, sizeof(m_sz_pass_flag), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_CTRL_SAT_NAME));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sat_name, sizeof(m_sz_sat_name), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_xx_rrng, "接続") != 0)
	{
		// ローカル
		m_RRngStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_RRngStopBtn.SetColor(COLOR_REMOTES);
	}

	// START 側
	// XX-RARR LAN接続時
	if ((strcmp(m_sz_lan_xx_rrng, "接続") == 0) &&
		(strcmp(m_sz_xx_rrng_cont, "REMOTE") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_RRngStartBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_RRngStartBtn.SetColor(COLOR_LOCAL);
	}

	// パス中とパス外で処理を分ける
	if (strcmp(m_sz_pass_flag, MON_STR_PASS) != 0)
	{
		// パス外の場合
		if (m_RRngStartBtn.IsWindowEnabled() == FALSE)
		{
			// インセンシティブならセンシティブにする
			m_RRngStartBtn.EnableWindow(TRUE);
			m_RRngStopBtn.EnableWindow(TRUE);
		}
	}
	else 
	{
		// パス中の場合
		// パス中の場合は、運用中の衛星が再生測距に対応している場合のみセンシティブにする

		// 抽出した衛星名で衛星固有ＤＢ検索

		CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();

		vector<CString>			&saNameList = dba.GetSatNameList();
		map<CString, __int64>	&satAdrList = dba.GetSatAdrList();

		if (saNameList.size() == 0 || satAdrList.size() == 0)
		{
			return ;
		}

		if (CString(m_sz_sat_name).IsEmpty() == FALSE)
		{
			satinf_adr = (struct satinf_t *)((__int64*)satAdrList[CString(m_sz_sat_name)]);
			original_adr = satinf_adr;
			sattbl_adr = (char *)satinf_adr;
			while (1)
			{
				satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
				if (satinf_adr->l_id == ENDOFSAT_ID)
				{
					/*識別ＩＤが終了の場合*/
					break;
				}

				// ＸＲＡＲＲ使用帯域情報
				if (satinf_adr->l_id == RARRBAND_ID)
				{
					rarrband_t_adr = (struct rarrband_t *)satinf_adr;
					if (rarrband_t_adr->l_rarr_band == RARRBAND_RX_ID)
					{
						// インセンシティブの場合はセンシティブにする
						if (m_RRngStartBtn.IsWindowEnabled() == FALSE)
						{
							m_RRngStartBtn.EnableWindow(TRUE);
							m_RRngStopBtn.EnableWindow(TRUE);
						}
					}
					else
					{
						// センシティブの場合はインセンシティブにする
						if (m_RRngStartBtn.IsWindowEnabled() == TRUE)
						{
							m_RRngStartBtn.EnableWindow(FALSE);
							m_RRngStopBtn.EnableWindow(FALSE);
						}
					}
					break;
				}
				sattbl_adr += satinf_adr->l_len;
			}
		}
	}

	// 更新
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNGING 再生型 STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_rrng_stop34()
{
	struct satinf_t *satinf_adr;
	struct satinf_t *original_adr;
	char *sattbl_adr;
	struct rarrband_t *rarrband_t_adr;	// ＲＡＲＲ使用帯域情報テーブルアドレス

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_XX_RRNG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rrng, sizeof(m_sz_lan_xx_rrng), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_RRNG_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_rrng_cont, sizeof(m_sz_xx_rrng_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_P_FLG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_pass_flag, sizeof(m_sz_pass_flag), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_CTRL_SAT_NAME));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sat_name, sizeof(m_sz_sat_name), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_xx_rrng, "接続") != 0)
	{
		// ローカル
		m_RRngStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_RRngStopBtn.SetColor(COLOR_REMOTES);
	}

	// START 側
	// XX-RARR LAN接続時
	if ((strcmp(m_sz_lan_xx_rrng, "接続") == 0) &&
		(strcmp(m_sz_pass_flag, "パス中") == 0) &&
		(strcmp(m_sz_xx_rrng_cont, "REMOTE") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_RRngStartBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_RRngStartBtn.SetColor(COLOR_LOCAL);
	}

	// パス中とパス外で処理を分ける
	if (strcmp(m_sz_pass_flag, MON_STR_PASS) != 0)
	{
		// パス外の場合
		if ((m_RRngStartBtn.IsWindowEnabled() == FALSE) && (m_l_rng_type != RNG_TYPE_NOR))
		{
			// インセンシティブならセンシティブにする
			m_RRngStartBtn.EnableWindow(TRUE);
			m_RRngStopBtn.EnableWindow(TRUE);
		}
	}
	else
	{
		// パス中の場合
		// パス中の場合は、運用中の衛星が再生測距に対応している場合のみセンシティブにする

		// 抽出した衛星名で衛星固有ＤＢ検索

		CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();

		vector<CString>			&saNameList = dba.GetSatNameList();
		map<CString, __int64>	&satAdrList = dba.GetSatAdrList();

		if (saNameList.size() == 0 || satAdrList.size() == 0)
		{
			return;
		}

		satinf_adr = (struct satinf_t *)((__int64*)satAdrList[CString(m_sz_sat_name)]);
		original_adr = satinf_adr;
		sattbl_adr = (char *)satinf_adr;
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}

			// ＸＲＡＲＲ使用帯域情報
			if (satinf_adr->l_id == RARRBAND_ID)
			{
				rarrband_t_adr = (struct rarrband_t *)satinf_adr;
				if (rarrband_t_adr->l_rarr_band == RARRBAND_RX_ID)
				{
					// インセンシティブの場合はセンシティブにする
					if ((m_RRngStartBtn.IsWindowEnabled() == FALSE) && (m_l_rng_type != RNG_TYPE_NOR))
					{
						m_RRngStartBtn.EnableWindow(TRUE);
						m_RRngStopBtn.EnableWindow(TRUE);
					}
				}
				else
				{
					// センシティブの場合はインセンシティブにする
					if (m_RRngStartBtn.IsWindowEnabled() == TRUE)
					{
						m_RRngStartBtn.EnableWindow(FALSE);
						m_RRngStopBtn.EnableWindow(FALSE);
					}
				}
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
	}

	// 更新
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNGING 再生型 START 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_rrng_start64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_XX_RRNG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rrng, sizeof(m_sz_lan_xx_rrng), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_RRNG_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_rrng_cont, sizeof(m_sz_xx_rrng_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_NORM_REP));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_xx_rrng, sizeof(m_sz_lan_xx_rrng), "接続");
	strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), "REPLAY");
	strcpy_s(m_sz_xx_rrng_cont, sizeof(m_sz_xx_rrng_cont), "REMOTE");
#endif

	// 再生型が選ばれていなければ、ボタンをインセンシティブにする
	if ((strcmp(m_sz_norm_rep, "REPLAY") != 0) && (m_RRngStartBtn.IsWindowEnabled() == TRUE))
	{
		m_RRngStartBtn.EnableWindow(FALSE);
		m_RRngStopBtn.EnableWindow(FALSE);
	}
	else if ((strcmp(m_sz_norm_rep, "REPLAY") == 0) && (m_RRngStartBtn.IsWindowEnabled() == FALSE) && (m_l_rng_type == RNG_TYPE_REP))
	{
		m_RRngStartBtn.EnableWindow(TRUE);
		m_RRngStopBtn.EnableWindow(TRUE);
	}

	if ((strcmp(m_sz_lan_xx_rrng, "接続") != 0) ||
		(strcmp(m_sz_norm_rep, "REPLAY") != 0))
	{
		// ローカル
		m_RRngStartBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_RRngStartBtn.SetColor(COLOR_REMOTES);
	}

	// STOP 側
	// XX-RRNG LAN接続時
	if ((strcmp(m_sz_lan_xx_rrng, "接続") == 0) &&
		(strcmp(m_sz_xx_rrng_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_norm_rep, "REPLAY") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_RRngStopBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_RRngStopBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNGING 再生型 START 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlX::show_rrng_start34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_XX_RRNG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rrng, sizeof(m_sz_lan_xx_rrng), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_RRNG_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_rrng_cont, sizeof(m_sz_xx_rrng_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_NORM_REP));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
	}

	// 再生型が選ばれていなければ、ボタンをインセンシティブにする
	if ((strcmp(m_sz_norm_rep, "REPLAY") != 0) && (m_RRngStartBtn.IsWindowEnabled() == TRUE))
	{
		m_RRngStartBtn.EnableWindow(FALSE);
		m_RRngStopBtn.EnableWindow(FALSE);
	}
	else if ((strcmp(m_sz_norm_rep, "REPLAY") == 0) && (m_RRngStartBtn.IsWindowEnabled() == FALSE) && (m_l_rng_type == RNG_TYPE_REP))
	{
		m_RRngStartBtn.EnableWindow(TRUE);
		m_RRngStopBtn.EnableWindow(TRUE);
	}

	if ((strcmp(m_sz_lan_xx_rrng, "接続") != 0) ||
		(strcmp(m_sz_norm_rep, "REPLAY") != 0))
	{
		// ローカル
		m_RRngStartBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_RRngStartBtn.SetColor(COLOR_REMOTES);
	}

	// STOP 側
	// XX-RRNG LAN接続時
	if ((strcmp(m_sz_lan_xx_rrng, "接続") == 0) &&
		(strcmp(m_sz_xx_rrng_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_norm_rep, "REPLAY") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_RRngStopBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_RRngStopBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


void CTrsControlX::OnClose()
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

void CTrsControlX::CloseDlg()
{
	KillTimer(TID_CONDITION);
	KillTimer(TID_COUNTDOWN);

	// ウィンドウを閉じるメッセージ
	theApp.m_pMainWnd->PostMessageW(eMessage_WindowClose, 0, (LPARAM)this);
}
