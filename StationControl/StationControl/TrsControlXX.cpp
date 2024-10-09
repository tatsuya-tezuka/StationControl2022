/*============================================================================*/
/*! TrsControlXX.cpp

-# X帯送信制御画面
*/
/*============================================================================*/
// TrsControlXX.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrsControlXX.h"
#include "afxdialogex.h"
#include "TrsControlMng.h"
#include "HookMsgBox.h"


// CTrsControlXX ダイアログ

IMPLEMENT_DYNAMIC(CTrsControlXX, CDialogBase)

/*============================================================================*/
/*! CTrsControlXX

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CTrsControlXX::CTrsControlXX(CWnd* pParent /*=NULL*/)
: CDialogBase(CTrsControlXX::IDD, pParent)
	, m_strIndexCmd(_T(""))
	, m_strIndexRng(_T(""))
	, m_strIndexCpr(_T(""))
	, m_strXXRng(_T(""))
	, m_strXXRngCont(_T(""))
{

}

/*============================================================================*/
/*! CTrsControlXX

-# コンストラクタ

@param	IDD：IDD
@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CTrsControlXX::CTrsControlXX(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialogBase(IDD, pParent)
{

}

/*============================================================================*/
/*! CTrsControlXX

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CTrsControlXX::~CTrsControlXX()
{
}

void CTrsControlXX::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TRXXCTRL_IDX_CMD_EDIT, m_strIndexCmd);
	DDX_Text(pDX, IDC_TRXXCTRL_IDX_RNG_EDIT, m_strIndexRng);
	DDX_Text(pDX, IDC_TRXXCTRL_IDX_CR_EDIT, m_strIndexCpr);
	DDX_Text(pDX, IDC_TRXXCTRL_XXRNG_EDIT, m_strXXRng);
	DDX_Text(pDX, IDC_TRXXCTRL_XXRNG_CONT_STATIC, m_strXXRngCont);
	DDX_Control(pDX, IDC_TRXXCTRL_RF_OFF_BTN, m_RfPwrOffBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_RF_ON_BTN, m_RfPwrOnBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_NOUSE_OFF_BTN, m_NoUseOffBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_NOUSE_ON_BTN, m_NoUseOnBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_STOP_BTN, m_SweepCountMStopBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_INC_BTN, m_SweepCountMIncBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_INCDEC_BTN, m_SweepCountMIncDecBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_DEC_BTN, m_SweepCountMDecBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_DECINC_BTN, m_SweepCountMDecIncBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_SUB_STOP_BTN, m_SweepCountSStopBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_SUB_INC_BTN, m_SweepCountSIncBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_SUB_INCDEC_BTN, m_SweepCountSIncDecBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_SUB_DEC_BTN, m_SweepCountSDecBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_SUB_DECINC_BTN, m_SweepCountSDecIncBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_MDL_OFF_BTN, m_ModulationOffBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_MDL_ON_BTN, m_ModulationOnBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_HOLD_OFF_BTN, m_HoldToneOffBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_HOLD_ON_BTN, m_HoldToneOnBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_DC_OFF_BTN, m_DoppContOffBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_DC_ON_BTN, m_DoppContOnBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_XXCOH_COH_BTN, m_XXCohCohBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_XXCOH_INCOH_BTN, m_XXCohInCohBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_RNGING_STOP_BTN, m_RngingStopBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_RNGING_START_BTN, m_RngingStartBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_RGNTYPE_STOP_BTN, m_RRngStopBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_RGNTYPE_START_BTN, m_RRngStartBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_IDX_CMD_BTN, m_IndexCmdBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_IDX_RNG_BTN, m_IndexRngBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_IDX_CR_BTN, m_IndexCRBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_XXRNG_C_BTN, m_XXRngNumBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SN_BTN, m_SweepNumBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_SUB_CHECK, m_SweepContSubChkBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_NOR_CHECK, m_SweepContNorChkBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_TRI_CHECK, m_SweepContTriChkBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_SUB_NOR_CHECK, m_SweepContSubNorChkBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_SUB_TRI_CHECK, m_SweepContSubTriChkBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_TX_STATIC, m_grTxControl);
	DDX_Control(pDX, IDC_TRXXCTRL_RF_STATIC, m_grRfPower);
	DDX_Control(pDX, IDC_TRXXCTRL_NOUSE_STATIC, m_grNoUse);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_STATIC, m_grSweepCont);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_SUB_STATIC, m_grSweepContSub);
	DDX_Control(pDX, IDC_TRXXCTRL_DC_STATIC, m_grDoppCont);
	DDX_Control(pDX, IDC_TRXXCTRL_MDL_STATIC, m_grModulation);
	DDX_Control(pDX, IDC_TRXXCTRL_HOLD_STATIC, m_grHoldTone);
	DDX_Control(pDX, IDC_TRXXCTRL_IDX_STATIC, m_grIndex);
	DDX_Control(pDX, IDC_TRXXCTRL_RNGCTRL_STATIC, m_grRngControl);
	DDX_Control(pDX, IDC_TRXXCTRL_XXRNG_STATIC, m_grXXRng);
	DDX_Control(pDX, IDC_TRXXCTRL_XXCOH_STATIC, m_grXXCoh);
	DDX_Control(pDX, IDC_TRXCTRL_RNGING_STATIC, m_grRnging);
	DDX_Control(pDX, IDC_TRXXCTRL_RGNTYPE_STATIC, m_grRRng);
}


BEGIN_MESSAGE_MAP(CTrsControlXX, CDialogBase)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CTrsControlXX::OnMenuCloseClose)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_NOR_CHECK, &CTrsControlXX::OnBnClickedTrxxctrlSweetNorCheck)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_TRI_CHECK, &CTrsControlXX::OnBnClickedTrxxctrlSweetTriCheck)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_CHECK, &CTrsControlXX::OnBnClickedTrxxctrlSweetSubCheck)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_NOR_CHECK, &CTrsControlXX::OnBnClickedTrxxctrlSweetSubNorCheck)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_TRI_CHECK, &CTrsControlXX::OnBnClickedTrxxctrlSweetSubTriCheck)
	ON_BN_CLICKED(IDC_TRXXCTRL_SN_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSnBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_IDX_CMD_BTN, &CTrsControlXX::OnBnClickedTrxxctrlIdxCmdBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_IDX_RNG_BTN, &CTrsControlXX::OnBnClickedTrxxctrlIdxRngBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_IDX_CR_BTN, &CTrsControlXX::OnBnClickedTrxxctrlIdxCrBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_XXRNG_C_BTN, &CTrsControlXX::OnBnClickedTrxxctrlXxrngCBtn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TRXXCTRL_RF_OFF_BTN, &CTrsControlXX::OnBnClickedTrxxctrlRfOffBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RF_ON_BTN, &CTrsControlXX::OnBnClickedTrxxctrlRfOnBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_NOUSE_OFF_BTN, &CTrsControlXX::OnBnClickedTrxxctrlNouseOffBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_NOUSE_ON_BTN, &CTrsControlXX::OnBnClickedTrxxctrlNouseOnBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_STOP_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetStopBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_INC_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetIncBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_INCDEC_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetIncdecBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_DEC_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetDecBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_DECINC_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetDecincBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_STOP_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetSubStopBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_INC_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetSubIncBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_INCDEC_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetSubIncdecBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_DEC_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetSubDecBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_DECINC_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetSubDecincBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_MDL_OFF_BTN, &CTrsControlXX::OnBnClickedTrxxctrlMdlOffBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_MDL_ON_BTN, &CTrsControlXX::OnBnClickedTrxxctrlMdlOnBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_DC_OFF_BTN, &CTrsControlXX::OnBnClickedTrxxctrlDcOffBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_DC_ON_BTN, &CTrsControlXX::OnBnClickedTrxxctrlDcOnBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_HOLD_OFF_BTN, &CTrsControlXX::OnBnClickedTrxxctrlHoldOffBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_HOLD_ON_BTN, &CTrsControlXX::OnBnClickedTrxxctrlHoldOnBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_XXCOH_COH_BTN, &CTrsControlXX::OnBnClickedTrxxctrlXxcohCohBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_XXCOH_INCOH_BTN, &CTrsControlXX::OnBnClickedTrxxctrlXxcohIncohBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RNGING_STOP_BTN, &CTrsControlXX::OnBnClickedTrxxctrlRngingStopBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RNGING_START_BTN, &CTrsControlXX::OnBnClickedTrxxctrlRngingStartBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RGNTYPE_STOP_BTN, &CTrsControlXX::OnBnClickedTrxxctrlRgntypeStopBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RGNTYPE_START_BTN, &CTrsControlXX::OnBnClickedTrxxctrlRgntypeStartBtn)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTrsControlXX メッセージ ハンドラー


/*============================================================================*/
/*! CTrsControlXX

-# ファイル-閉じるメニュー処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnMenuCloseClose()
{
	// いずれかのボタンが実行中の場合
	if (m_RfPwrOffBtn.IsColorIgnore2() ||
		m_RfPwrOnBtn.IsColorIgnore2() ||
		m_NoUseOffBtn.IsColorIgnore2() ||
		m_NoUseOnBtn.IsColorIgnore2() ||
		m_SweepCountMStopBtn.IsColorIgnore2() ||
		m_SweepCountMIncBtn.IsColorIgnore2() ||
		m_SweepCountMIncDecBtn.IsColorIgnore2() ||
		m_SweepCountMDecBtn.IsColorIgnore2() ||
		m_SweepCountMDecIncBtn.IsColorIgnore2() ||
		m_SweepCountSStopBtn.IsColorIgnore2() ||
		m_SweepCountSIncBtn.IsColorIgnore2() ||
		m_SweepCountSIncDecBtn.IsColorIgnore2() ||
		m_SweepCountSDecBtn.IsColorIgnore2() ||
		m_SweepCountSDecIncBtn.IsColorIgnore2() ||
		m_SweepNumBtn.IsColorIgnore2() ||
		m_DoppContOffBtn.IsColorIgnore2() ||
		m_DoppContOnBtn.IsColorIgnore2() ||
		m_ModulationOffBtn.IsColorIgnore2() ||
		m_ModulationOnBtn.IsColorIgnore2() ||
		m_HoldToneOffBtn.IsColorIgnore2() ||
		m_HoldToneOnBtn.IsColorIgnore2() ||
		m_IndexCmdBtn.IsColorIgnore2() ||
		m_IndexRngBtn.IsColorIgnore2() ||
		m_IndexCRBtn.IsColorIgnore2() ||
		m_XXRngNumBtn.IsColorIgnore2() ||
		m_XXCohCohBtn.IsColorIgnore2() ||
		m_XXCohInCohBtn.IsColorIgnore2() ||
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
/*! CTrsControlXX

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CTrsControlXX::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	// RF-POWER
	m_RfPwrOffBtn.SetColor(COLOR_REMOTES);
	m_RfPwrOffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_RfPwrOnBtn.SetColor(COLOR_REMOTES);
	m_RfPwrOnBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// NO-USE
	m_NoUseOffBtn.SetColor(COLOR_REMOTES);
	m_NoUseOffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_NoUseOnBtn.SetColor(COLOR_REMOTES);
	m_NoUseOnBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// SWEEP-CONT
	m_SweepContNorChkBtn.SetCheck(1);
	m_SweepCountMStopBtn.SetColor(COLOR_REMOTES);
	m_SweepCountMStopBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepCountMIncBtn.SetColor(COLOR_REMOTE);
	m_SweepCountMIncBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTE);
	m_SweepCountMIncDecBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepCountMDecBtn.SetColor(COLOR_REMOTE);
	m_SweepCountMDecBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTE);
	m_SweepCountMDecIncBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// SWEEP-CONT(SubCarr)
	m_SweepContSubNorChkBtn.SetCheck(1);
	m_SweepContSubNorChkBtn.EnableWindow(FALSE);
	m_SweepContSubTriChkBtn.EnableWindow(FALSE);
	m_SweepCountSStopBtn.SetColor(COLOR_REMOTES);
	m_SweepCountSStopBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepCountSStopBtn.EnableWindow(FALSE);
	m_SweepCountSIncBtn.SetColor(COLOR_REMOTE);
	m_SweepCountSIncBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepCountSIncBtn.EnableWindow(FALSE);
	m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTE);
	m_SweepCountSIncDecBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepCountSIncDecBtn.EnableWindow(FALSE);
	m_SweepCountSDecBtn.SetColor(COLOR_REMOTE);
	m_SweepCountSDecBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepCountSDecBtn.EnableWindow(FALSE);
	m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTE);
	m_SweepCountSDecIncBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_SweepCountSDecIncBtn.EnableWindow(FALSE);

	// MODULATION
	m_ModulationOffBtn.SetColor(COLOR_REMOTES);
	m_ModulationOffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_ModulationOnBtn.SetColor(COLOR_REMOTE);
	m_ModulationOnBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// HOLD-TONE
	m_HoldToneOffBtn.SetColor(COLOR_REMOTES);
	m_HoldToneOffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_HoldToneOnBtn.SetColor(COLOR_REMOTE);
	m_HoldToneOnBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// DOPP-CONT
	m_DoppContOffBtn.SetColor(COLOR_REMOTES);
	m_DoppContOffBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_DoppContOnBtn.SetColor(COLOR_REMOTE);
	m_DoppContOnBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// X/X-COH
	m_XXCohCohBtn.SetColor(COLOR_REMOTES);
	m_XXCohCohBtn.SetCtrlButton(TRUE);			// Ctrlキー +
	m_XXCohInCohBtn.SetColor(COLOR_REMOTE);
	m_XXCohInCohBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// RNGING
	m_RngingStopBtn.SetColor(COLOR_REMOTES);
	m_RngingStopBtn.SetCtrlButton(TRUE);		// Ctrlキー +
	m_RngingStartBtn.SetColor(COLOR_REMOTE);
	m_RngingStartBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// 再生型
	m_RRngStopBtn.SetColor(COLOR_REMOTES);
	m_RRngStopBtn.SetShiftButton(TRUE);			// Shiftキー +
	m_RRngStartBtn.SetColor(COLOR_REMOTE);
	m_RRngStartBtn.SetShiftButton(TRUE);		// Shiftキー +

	// SWEEP-NUM
	m_SweepNumBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_SweepNumBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// INDEX-CMD
	m_IndexCmdBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_IndexCmdBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// INDEX-RNG
	m_IndexRngBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_IndexRngBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// INDEX-C+R
	m_IndexCRBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_IndexCRBtn.SetCtrlButton(TRUE);		// Ctrlキー +

	// X/X-RNG Cnt
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
	m_grSweepContSub.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grSweepContSub.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grDoppCont.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grDoppCont.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grModulation.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grModulation.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grHoldTone.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grHoldTone.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grIndex.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grIndex.SetGroupType(CGroupBoxEx::eGroupType_Round);
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
	m_l_main_sweep_cont_stop_cntdwn = -1;
	m_l_main_sweep_cont_inc_cntdwn = -1;
	m_l_main_sweep_cont_incdec_cntdwn = -1;
	m_l_main_sweep_cont_dec_cntdwn = -1;
	m_l_main_sweep_cont_decinc_cntdwn = -1;
	m_l_sub_sweep_cont_stop_cntdwn = -1;
	m_l_sub_sweep_cont_inc_cntdwn = -1;
	m_l_sub_sweep_cont_incdec_cntdwn = -1;
	m_l_sub_sweep_cont_dec_cntdwn = -1;
	m_l_sub_sweep_cont_decinc_cntdwn = -1;
	m_l_sweep_num_cntdwn = -1;
	m_l_dopp_cont_off_cntdwn = -1;
	m_l_dopp_cont_on_cntdwn = -1;
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
	m_l_cmd_mod_idx_cntdwn = -1;
	m_l_rng_mod_idx_cntdwn = -1;
	m_l_car_mod_idx_cntdwn = -1;
	m_l_rnging_stop_cntdwn = -1;
	m_l_rnging_start_cntdwn = -1;
	m_l_rep_rnging_stop_cntdwn = -1;
	m_l_rep_rnging_start_cntdwn = -1;
	m_l_rng_type = -1;
	m_l_dsnrng_turn_start_flag = -1;
	m_l_dsnrng_regen_start_flag = -1;
	m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 連続SWEEP回数内部バッファを監視データで初期化する
	// 現在の状態を監視ＤＢから取得 
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SWEEP_NUM));
	if (obs_adr != NULL)
	{
		m_l_sweep_num_set = (long)obs_adr->d_data;
	}

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
/*! CTrsControlXX

-# ダイアログ更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CTrsControlXX::UpdateDlg()
{
	// 送信制御画面状態表示タイムアウトコールバックルーチン起動
	show_opsc_condition();

	// カウントダウンタイムアウトコールバックルーチン起動
	count_down();

	return TRUE;
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『RF-POWER』-『OFF』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlRfOffBtn()
{
	// ボタンを制御中にする
	m_RfPwrOffBtn.SetColor(COLOR_CTRLIN);
	m_RfPwrOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_ON/OFF_CONT"), _T("OFF"));

	// カウントダウン開始
	m_l_rf_power_off_cntdwn = RF_POWER_TIME_XX_LIMIT;

	// 更新
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『RF-POWER』-『ON』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlRfOnBtn()
{
	// ボタンを制御中にする
	m_RfPwrOffBtn.SetColor(COLOR_CTRLIN);
	m_RfPwrOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_ON/OFF_CONT"), _T("ON"));

	// カウントダウン開始
	m_l_rf_power_on_cntdwn = RF_POWER_TIME_XX_LIMIT;

	// 更新
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『NO-USE』-『OFF』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlNouseOffBtn()
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

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE_RESET"));

			// カウントダウン開始
			m_l_no_use_off_cntdwn = NO_USE_TIME_XX_LIMIT;
		}
		else if (col == COLOR_CTRLIN)
		{
			// 制御中

			// ボタンを制御中にする
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE_RESET"));

			// カウントダウン開始
			m_l_no_use_off_cntdwn = NO_USE_TIME_XX_LIMIT;

			// 他方のカウントダウン終了
			m_l_no_use_on_cntdwn = -1;
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

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE_RESET"));

			// カウントダウン開始
			m_l_no_use_off_cntdwn = NO_USE_TIME_XX_LIMIT;
		}
		else if (col == COLOR_CTRLIN)
		{
			// 制御中

			// ボタンを制御中にする
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE_RESET"));

			// カウントダウン開始
			m_l_no_use_off_cntdwn = NO_USE_TIME_XX_LIMIT;

			// 他方のカウントダウン終了
			m_l_no_use_on_cntdwn = -1;
		}
		else if (col == COLOR_REMOTES)
		{
			// リモート（選択）

			// ボタンを制御中にする
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE_RESET"));

			// カウントダウン開始
			m_l_no_use_off_cntdwn = NO_USE_TIME_XX_LIMIT;

			// 他方のカウントダウン終了
			m_l_no_use_on_cntdwn = -1;
		}
	}

	// 更新
	m_NoUseOffBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『NO-USE』-『ON』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlNouseOnBtn()
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
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE"));

			// カウントダウン開始
			m_l_no_use_on_cntdwn = NO_USE_TIME_XX_LIMIT;
		}
		else if (col == COLOR_CTRLIN)
		{
			// 制御中

			// ボタンを制御中にする
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE"));

			// カウントダウン開始
			m_l_no_use_on_cntdwn = NO_USE_TIME_XX_LIMIT;

			// 他方のカウントダウン終了
			m_l_no_use_off_cntdwn = -1;
		}
	}
	else if (station == 2)
	{
		// 内之浦34m
		if (col == COLOR_REMOTE)
		{
			// リモート（非選択）

			// ボタンを制御中にする
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE"));

			// カウントダウン開始
			m_l_no_use_on_cntdwn = NO_USE_TIME_XX_LIMIT;
		}
		else if (col == COLOR_CTRLIN)
		{
			// 制御中

			// ボタンを制御中にする
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE"));

			// カウントダウン開始
			m_l_no_use_on_cntdwn = NO_USE_TIME_XX_LIMIT;

			// 他方のカウントダウン終了
			m_l_no_use_off_cntdwn = -1;
		}
		else if (col == COLOR_REMOTES)
		{
			//　リモート（選択）

			// ボタンを制御中にする
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// 制御通知送信
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE"));

			// カウントダウン開始
			m_l_no_use_on_cntdwn = NO_USE_TIME_XX_LIMIT;

			// 他方のカウントダウン終了
			m_l_no_use_off_cntdwn = -1;
		}
	}

	// 更新
	m_NoUseOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『SWEEP-CONT』-『NOR』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetNorCheck()
{
	// NOR と TRI を排他制御
	m_SweepContNorChkBtn.SetCheck(1);
	m_SweepContTriChkBtn.SetCheck(0);
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『SWEEP-CONT』-『TRI』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetTriCheck()
{
	// NOR と TRI を排他制御
	m_SweepContTriChkBtn.SetCheck(1);
	m_SweepContNorChkBtn.SetCheck(0);
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『SWEEP-CONT』-『STOP』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetStopBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("OFF"));
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("STOP"));

	// カウントダウン開始
	m_l_main_sweep_cont_stop_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountMStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecIncBtn.SetColor(COLOR_CTRLIN);
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『SWEEP-CONT』-『INC』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetIncBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("ON"));

	// カウントダウン開始
	m_l_main_sweep_cont_inc_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountMStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecIncBtn.SetColor(COLOR_CTRLIN);
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『SWEEP-CONT』-『INCDEC』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetIncdecBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("ON"));

	// カウントダウン開始
	m_l_main_sweep_cont_incdec_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountMStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecIncBtn.SetColor(COLOR_CTRLIN);
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『SWEEP-CONT』-『DEC』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetDecBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("ON"));

	// カウントダウン開始
	m_l_main_sweep_cont_dec_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountMStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecIncBtn.SetColor(COLOR_CTRLIN);
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『SWEEP-CONT』-『DECINC』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetDecincBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("ON"));

	// カウントダウン開始
	m_l_main_sweep_cont_decinc_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountMStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecIncBtn.SetColor(COLOR_CTRLIN);
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『SWEEP-CONT(SubCarr)』-チェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetSubCheck()
{
	if (m_SweepContSubChkBtn.GetCheck())
	{
		// 活性
		m_SweepCountSStopBtn.EnableWindow(TRUE);
		m_SweepCountSIncBtn.EnableWindow(TRUE);
		m_SweepCountSIncDecBtn.EnableWindow(TRUE);
		m_SweepCountSDecBtn.EnableWindow(TRUE);
		m_SweepCountSDecIncBtn.EnableWindow(TRUE);

		m_SweepContSubNorChkBtn.EnableWindow(TRUE);
		m_SweepContSubTriChkBtn.EnableWindow(TRUE);
	}
	else
	{
		// 非活性
		m_SweepCountSStopBtn.EnableWindow(FALSE);
		m_SweepCountSIncBtn.EnableWindow(FALSE);
		m_SweepCountSIncDecBtn.EnableWindow(FALSE);
		m_SweepCountSDecBtn.EnableWindow(FALSE);
		m_SweepCountSDecIncBtn.EnableWindow(FALSE);

		m_SweepContSubNorChkBtn.EnableWindow(FALSE);
		m_SweepContSubTriChkBtn.EnableWindow(FALSE);
	}
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『SWEEP-CONT(SubCarr)』-『NOR』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetSubNorCheck()
{
	// NOR と TRI を排他制御
	m_SweepContSubNorChkBtn.SetCheck(1);
	m_SweepContSubTriChkBtn.SetCheck(0);
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『SWEEP-CONT(SubCarr)』-『TRI』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetSubTriCheck()
{
	// NOR と TRI を排他制御
	m_SweepContSubTriChkBtn.SetCheck(1);
	m_SweepContSubNorChkBtn.SetCheck(0);
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『SWEEP-CONT(SubCarr)』-『STOP』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetSubStopBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContSubNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("OFF"));
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("STOP"));

	// カウントダウン開始
	m_l_sub_sweep_cont_stop_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountSStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecIncBtn.SetColor(COLOR_CTRLIN);

	m_SweepContSubChkBtn.EnableWindow(FALSE);
	m_SweepContSubNorChkBtn.EnableWindow(FALSE);
	m_SweepContSubTriChkBtn.EnableWindow(FALSE);

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『SWEEP-CONT(SubCarr)』-『INC』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetSubIncBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContSubNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("ON"));

	// カウントダウン開始
	m_l_sub_sweep_cont_inc_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountSStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecIncBtn.SetColor(COLOR_CTRLIN);

	m_SweepContSubChkBtn.EnableWindow(FALSE);
	m_SweepContSubNorChkBtn.EnableWindow(FALSE);
	m_SweepContSubTriChkBtn.EnableWindow(FALSE);

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『SWEEP-CONT(SubCarr)』-『INCDEC』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetSubIncdecBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContSubNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("ON"));

	// カウントダウン開始
	m_l_sub_sweep_cont_incdec_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountSStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecIncBtn.SetColor(COLOR_CTRLIN);

	m_SweepContSubChkBtn.EnableWindow(FALSE);
	m_SweepContSubNorChkBtn.EnableWindow(FALSE);
	m_SweepContSubTriChkBtn.EnableWindow(FALSE);

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『SWEEP-CONT(SubCarr)』-『DEC』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetSubDecBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContSubNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("ON"));

	// カウントダウン開始
	m_l_sub_sweep_cont_dec_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountSStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecIncBtn.SetColor(COLOR_CTRLIN);

	m_SweepContSubChkBtn.EnableWindow(FALSE);
	m_SweepContSubNorChkBtn.EnableWindow(FALSE);
	m_SweepContSubTriChkBtn.EnableWindow(FALSE);

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『SWEEP-CONT(SubCarr)』-『DECINC』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetSubDecincBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContSubNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("ON"));

	// カウントダウン開始
	m_l_sub_sweep_cont_decinc_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountSStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecIncBtn.SetColor(COLOR_CTRLIN);

	m_SweepContSubChkBtn.EnableWindow(FALSE);
	m_SweepContSubNorChkBtn.EnableWindow(FALSE);
	m_SweepContSubTriChkBtn.EnableWindow(FALSE);

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『SWEEP-NUM』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSnBtn()
{
	CString str1,str2;

	// リモート時
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (m_i_priv_flag == 1)) 
	{
		// 制御通知送信
		m_SweepNumBtn.GetWindowTextW(str2);

		str1.Format(_T("%d"), _wtoi(str2));
		CString str3 = theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_CYCLE"), _T(""), str1);
		if (str3 == _T(""))
		{
			// キャンセル時は、次の制御を送らない
			return;
		}

		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_CYCLE"), str3);

		// カウントダウン開始
		m_l_sweep_num_cntdwn = SWEEP_NUM_TIME_XX_LIMIT;
	}
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『DOPP-CONT』-『OFF』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlDcOffBtn()
{
	// ボタンを制御中にする
	m_DoppContOffBtn.SetColor(COLOR_CTRLIN);
	m_DoppContOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
//	theApp.GetSatelliteData().opctproc(_T("XX-DSNTCRFC.TX_DOPP_AID"), _T("OFF"));
	theApp.GetSatelliteData().opctproc(_T("CTRL.AID_UL"), _T("OFF"));

	// カウントダウン開始
	m_l_dopp_cont_off_cntdwn = DOPP_CONT_TIME_XX_LIMIT;

	// 更新
	m_DoppContOffBtn.Invalidate();
	m_DoppContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『DOPP-CONT』-『ON』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlDcOnBtn()
{
	// ボタンを制御中にする
	m_DoppContOffBtn.SetColor(COLOR_CTRLIN);
	m_DoppContOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
//	theApp.GetSatelliteData().opctproc(_T("XX-DSNTCRFC.TX_DOPP_AID"), _T("ON"));
	theApp.GetSatelliteData().opctproc(_T("CTRL.AID_UL"), _T("ON"));

	// カウントダウン開始
	m_l_dopp_cont_on_cntdwn = DOPP_CONT_TIME_XX_LIMIT;

	// 更新
	m_DoppContOffBtn.Invalidate();
	m_DoppContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『MODULATION』-『OFF』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlMdlOffBtn()
{
#if 0	// 現行ソースがコメントアウト！
	// ボタンを制御中にする
	m_ModulationOffBtn.SetColor(COLOR_CTRLIN);
	m_ModulationOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("XX-TCRMOD.MODULATION"), _T("OFF"));

	// カウントダウン開始
	m_l_modulation_off_cntdwn = MODULATION_TIME_XX_LIMIT;

	// 更新
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
#else	// CMD-MOD

	// ボタンを制御中にする
	m_ModulationOffBtn.SetColor(COLOR_CTRLIN);
	m_ModulationOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_IF_MODULATION"), _T("OFF"));

	// カウントダウン開始
	m_l_modulation_off_cntdwn = MODULATION_TIME_XX_LIMIT;

	// 更新
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
#endif
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『MODULATION』-『ON』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlMdlOnBtn()
{
#if 0	// 現行ソースがコメントアウト！
	// ボタンを制御中にする
	m_ModulationOffBtn.SetColor(COLOR_CTRLIN);
	m_ModulationOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("XX-TCRMOD.MODULATION"), _T("ON"));

	// カウントダウン開始
	m_l_modulation_on_cntdwn = MODULATION_TIME_XX_LIMIT;

	// 更新
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
#else	// CMD-MOD
	// ボタンを制御中にする
	m_ModulationOffBtn.SetColor(COLOR_CTRLIN);
	m_ModulationOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_IF_MODULATION"), _T("ON"));

	// カウントダウン開始
	m_l_modulation_on_cntdwn = MODULATION_TIME_XX_LIMIT;

	// 更新
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
#endif
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『HOLD-TONE』-『OFF』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlHoldOffBtn()
{
	// ボタンを制御中にする
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("X-CMDOUT2.HOLD_TONE"), _T("OFF"));

	// カウントダウン開始
	m_l_hold_tone_off_cntdwn = HOLD_TONE_TIME_XX_LIMIT;

	// 更新
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『HOLD-TONE』-『ON』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlHoldOnBtn()
{
	// ボタンを制御中にする
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("X-CMDOUT2.HOLD_TONE"), _T("ON"));

	// カウントダウン開始
	m_l_hold_tone_on_cntdwn = HOLD_TONE_TIME_XX_LIMIT;

	// 更新
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『INDEX』-『CMD』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlIdxCmdBtn()
{
	CString str;
	// リモート時
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (m_i_priv_flag == 1)) 
	{
		// 制御通知送信
		str.Format(_T("%s"), (LPCTSTR)m_strIndexCmd);
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_MOD_INDEX"), _T(""), str);
	}
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『INDEX』-『RNG』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlIdxRngBtn()
{
	CString str;

	// リモート時
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (m_i_priv_flag == 1)) 
	{
		// 制御通知送信
		str.Format(_T("%s"), (LPCTSTR)m_strIndexRng);
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.RNG_MOD_INDEX"), _T(""), str);
	}
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『INDEX』-『C+R』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlIdxCrBtn()
{
	CString str;

	// リモート時
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (m_i_priv_flag == 1)) 
	{
		// 制御通知送信
		str.Format(_T("%s"), (LPCTSTR)m_strIndexCmd);
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_MOD_INDEX"), str);
		str.Format(_T("%s"), (LPCTSTR)m_strIndexRng);
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.RNG_MOD_INDEX"), str);
	}
}


/*============================================================================*/
/*! CTrsControlXX

-# 『RX CONTROL』-『X/X-RNG』-『NUM』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlXxrngCBtn()
{
	// 制御通知送信
	UpdateData();
	theApp.GetSatelliteData().opctproc(_T("V-EQU.RNG_NUM"), _T(""), m_strXXRng);
}


/*============================================================================*/
/*! CTrsControlXX

-# 『RX CONTROL』-『X/X-COH』-『COH』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlXxcohCohBtn()
{
	// ボタンを制御中にする
	m_XXCohCohBtn.SetColor(COLOR_CTRLIN);
	m_XXCohInCohBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	int station = theApp.GetSelectStation();
	if (station == 0)
	{
		// 臼田64m
		theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("COH"));
	}
	else if (station == 2)
	{
		// 内之浦34m
		theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("COHI"));
	}

	// カウントダウン開始
	m_l_coh_incoh_coh_cntdwn = COH_INCOH_TIME_XX_LIMIT;

	// 更新
	m_XXCohCohBtn.Invalidate();
	m_XXCohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『RX CONTROL』-『X/X-COH』-『INCOH』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlXxcohIncohBtn()
{
	int station = theApp.GetSelectStation();

	// ボタンを制御中にする
	m_XXCohCohBtn.SetColor(COLOR_CTRLIN);
	m_XXCohInCohBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	if (station == 0)
	{
		// 臼田64m
		theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("INCOH"));
	}
	else if (station == 2)
	{
		// 内之浦34m
		theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("INCOHI"));
	}

	// カウントダウン開始
	m_l_coh_incoh_incoh_cntdwn = COH_INCOH_TIME_XX_LIMIT;

	// 更新
	m_XXCohCohBtn.Invalidate();
	m_XXCohInCohBtn.Invalidate();
}



/*============================================================================*/
/*! CTrsControlXX

-# 『RX CONTROL』-『RNGING』-『STOP』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlRngingStopBtn()
{
	int station = theApp.GetSelectStation();

	// ボタンを制御中にする
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING_CANCEL"), _T(","));

	// カウントダウン開始
	if (station == 0)
	{
		// 臼田64m
		m_l_rnging_stop_cntdwn = RNGING_TIME_XX64_LIMIT;
	}
	else if (station == 2)
	{
		// 内之浦34m
		m_l_rnging_stop_cntdwn = RNGING_TIME_XX34_LIMIT;
	}

	// RNG種別フラグをクリアする
	m_l_rng_type = RNG_TYPE_NON;

	// 更新
	m_RngingStopBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『RX CONTROL』-『RNGING』-『START』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlRngingStartBtn()
{
	// ボタンを制御中にする
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);
	m_RngingStartBtn.SetColor(COLOR_CTRLIN);

	// 再生型ボタンをインセンシティブにする
	if (m_RngingStartBtn.IsWindowEnabled() == TRUE)
	{
		m_RRngStopBtn.EnableWindow(FALSE);
		m_RRngStartBtn.EnableWindow(FALSE);
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CTRL.SEL_DSN_RNG_MODE"), _T("TURNAROUND"));
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.RNG_MODE"), _T("TURNAROUND"));

	// 従来型切り替え制御実行中を設定
	m_l_dsnrng_turn_start_flag = 1;

	// 更新
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}



/*============================================================================*/
/*! CTrsControlXX

-# 『RX CONTROL』-『再生型』-『STOP』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlRgntypeStopBtn()
{
	int station = theApp.GetSelectStation();

	// 再生型ボタンをインセンシティブにする
	m_RRngStopBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING_CANCEL"), _T(","));

	// カウントダウン開始
	if (station == 0)
	{
		// 臼田64m
		m_l_rep_rnging_stop_cntdwn = REP_RNGING_TIME_XX64_LIMIT;
	}
	else if (station == 2)
	{
		// 内之浦34m
		m_l_rep_rnging_stop_cntdwn = REP_RNGING_TIME_XX34_LIMIT;
	}

	// RNG種別フラグをクリアする
	m_l_rng_type = RNG_TYPE_NON;

	// 更新
	m_RRngStopBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『RX CONTROL』-『再生型』-『START』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlRgntypeStartBtn()
{
	// ボタンを制御中にする
	m_RRngStopBtn.SetColor(COLOR_CTRLIN);
	m_RRngStartBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CTRL.SEL_DSN_RNG_MODE"), _T("REGEN"));
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.RNG_MODE"), _T("REGEN"));

	// 従来型ボタンをインセンシティブにする
	if (m_RngingStopBtn.IsWindowEnabled() == TRUE)
	{
		m_RngingStopBtn.EnableWindow(FALSE);
		m_RngingStartBtn.EnableWindow(FALSE);
	}

	// 再生型切り替え制御実行中を設定
	m_l_dsnrng_regen_start_flag = 1;

	// 更新
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# OnTimer

@param  nIDEvent:タイマーの識別子
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::OnTimer(UINT_PTR nIDEvent)
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
/*! CTrsControlXX

-# 送信制御画面状態表示設定

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_opsc_condition()
{
	TRACE(_T("show_opsc_condition\n"));
	KillTimer(TID_CONDITION);

	int station = theApp.GetSelectStation();
	if (station == eStation_Usuda64)
	{
		// 臼田64m
		show_opsc_condition64();
	}
	else if (station == eStation_Usuda642)
	{
		// 臼田54m
		show_opsc_condition54();
	}
	else if (station == eStation_Uchinoura34)
	{
		// 内之浦34m
		show_opsc_condition34();
	}

	SetTimer(TID_CONDITION, INTERVAL_CONDITION, 0);
}

/*============================================================================*/
/*! CTrsControlXX

-# 送信制御画面状態表示設定

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_opsc_condition64()
{
	TRACE(_T("show_opsc_condition\n"));

	char sz_buff[64];
	CString str1, str2;

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 制御権は常に有り
	m_i_priv_flag = 1;

	/*-- TX CONTROL ----------------------------------------------------------------------*/
	// RF POWER ON ENABLE 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_RF_ON_ENBL_X), CString(m_sz_rf_on_enbl_x), nLogEx::debug);
	}

	// RF-TRIP NOUSE ON ENABLE 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_NO_USE_ENBL_X), CString(m_sz_no_use_enbl_x), nLogEx::debug);
	}

	// UP-LINK CONTROL 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_UPLINK_CONT_X));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_uplink_cont_x, sizeof(m_sz_uplink_cont_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_UPLINK_CONT_X), CString(m_sz_uplink_cont_x), nLogEx::debug);
	}

	// RF POWER ON DETECT ANSWER 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_DET_ANS_X));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_det_ans_x, sizeof(m_sz_rf_on_det_ans_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_RF_ON_DET_ANS_X), CString(m_sz_rf_on_det_ans_x), nLogEx::debug);
	}

	// RF POWER ON DETECT ANSWER 状態 <<XUP>> S-TX用に変更
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_DET_ANS_S));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_det_ans_s, sizeof(m_sz_rf_on_det_ans_s), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_RF_ON_DET_ANS_S), CString(m_sz_rf_on_det_ans_s), nLogEx::debug);
	}

	// HPA-2増設対応
	// HPA SEL 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_HPA_SEL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_hpa_sel, sizeof(m_sz_hpa_sel), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_HPA_SEL), CString(m_sz_hpa_sel), nLogEx::debug);
	}

	// RF POWER ON DETECT ANSWER 状態 (HPA-2)
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_DET_ANS_X_H2));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_det_ans_x_h2, sizeof(m_sz_rf_on_det_ans_x_h2), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_RF_ON_DET_ANS_X_H2), CString(m_sz_rf_on_det_ans_x_h2), nLogEx::debug);
	}


	/*** リモート／ローカル 状態取得 ******************************************************/

#if 0
	// S-TX リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_X_TX_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_tx_rl, sizeof(m_sz_x_tx_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("X-TX.TX_TOTAL_CONT"), CString(m_sz_x_tx_rl), nLogEx::debug);
	}
#endif

	// S-TXSYN リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_DSNTCR_RL), CString(m_sz_dsntcr_rl), nLogEx::debug);
	}

	// X-UL_CNT リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_X_UL_CNT_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_ul_cnt_rl, sizeof(m_sz_x_ul_cnt_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_X_UL_CNT_RL), CString(m_sz_x_ul_cnt_rl), nLogEx::debug);
	}

	// X-UC リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_X_UC_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_uc_rl, sizeof(m_sz_x_uc_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_X_UC_RL), CString(m_sz_x_uc_rl), nLogEx::debug);
	}

	// X-RF_IF リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_X_RF_IF_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_rf_if_rl, sizeof(m_sz_x_rf_if_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_X_RF_IF_RL), CString(m_sz_x_rf_if_rl), nLogEx::debug);
	}

	/*** アラームチェック  <<XUP>> X-TX用に変更 *******************************************/
#if 1
	strcpy_s(m_sz_alarm_x, sizeof(m_sz_alarm_x), "OFF");
#else
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_ALARM_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_alarm_x, sizeof(m_sz_alarm_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("X-TX.TOTAL_FAULT"), CString(m_sz_alarm_x), nLogEx::debug);
	}
#endif

	// アラーム時
	if (strcmp(m_sz_alarm_x, "ON") == 0)
	{
		// 枠を赤くする
		m_grRfPower.SetBackColor(COLOR_ALARM);
		m_grNoUse.SetBackColor(COLOR_ALARM);
		m_grSweepCont.SetBackColor(COLOR_ALARM);
		m_grSweepContSub.SetBackColor(COLOR_ALARM);
		m_grDoppCont.SetBackColor(COLOR_ALARM);
		m_grModulation.SetBackColor(COLOR_ALARM);
		m_grHoldTone.SetBackColor(COLOR_ALARM);
	}

	// アラーム解除
	else
	{
		// 枠の色を元に戻す
		m_grRfPower.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grNoUse.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepCont.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepContSub.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grDoppCont.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grModulation.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grHoldTone.SetBackColor(GetSysColor(COLOR_3DFACE));
	}


	/*** RF-POWER *************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_POWER_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_power_x, sizeof(m_sz_rf_power_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("X-TX.RF_ON_ANS"), CString(m_sz_rf_power_x), nLogEx::debug);
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
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NO_USE_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_x, sizeof(m_sz_no_use_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_NO_USE_X), CString(m_sz_no_use_x), nLogEx::debug);
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

	/*** MAIN SWEEP-CONT ***********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_TC_SWEEP_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cont, sizeof(m_sz_sweep_cont), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_TC_SWEEP_CONT), CString(m_sz_sweep_cont), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_TC_SWEEP_CMD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cmd, sizeof(m_sz_sweep_cmd), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_TC_SWEEP_CMD), CString(m_sz_sweep_cmd), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_TC_SWEEP_STS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_sts, sizeof(m_sz_sweep_sts), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_TC_SWEEP_STS), CString(m_sz_sweep_sts), nLogEx::debug);
	}

	// SWEEP-CONT STOP カウントダウン中に監視データが STOP になった場合
	if (m_l_main_sweep_cont_stop_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "OFF") == 0 ||
		strcmp(m_sz_sweep_cmd, "STOP") == 0 ||
		strcmp(m_sz_sweep_sts, "STOP") == 0))
	{
		// カウントダウン終了
		m_l_main_sweep_cont_stop_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;

		// SWEEP実行フラグOFF
		m_i_sweep_flag = 0;
	}
	// SWEEP-CONT STOP カウントダウンタイムリミットになった場合
	else if (m_l_main_sweep_cont_stop_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_main_sweep_cont_stop_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}

	// SWEEP-CONT INC カウントダウン中にデータが +SAW/+TRIANGLE になった場合
	if (m_l_main_sweep_cont_inc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "+SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0))
	{
		// カウントダウン終了
		m_l_main_sweep_cont_inc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +SAW カウントダウンタイムリミットになった場合
	else if (m_l_main_sweep_cont_inc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_main_sweep_cont_inc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_main_sweep_cont_inc_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "+SAW") != 0))
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if ((m_l_main_sweep_cont_inc_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "+TRIANGLE") != 0))
	{

		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT INCDEC カウントダウン中にデータが +-SAW/+-TRIANGLE になった場合
	if (m_l_main_sweep_cont_incdec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "+-SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0))
	{
		// カウントダウン終了
		m_l_main_sweep_cont_incdec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +-SAW/TRIANGLE カウントダウンタイムリミットになった場合
	else if (m_l_main_sweep_cont_incdec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_main_sweep_cont_incdec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_main_sweep_cont_incdec_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "+-SAW") != 0))
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+-SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if ((m_l_main_sweep_cont_incdec_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "+-TRIANGLE") != 0))
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+-TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT DEC カウントダウン中にデータが -SAW/-TRIANGLE になった場合
	if (m_l_main_sweep_cont_dec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "-SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0))
	{
		// カウントダウン終了
		m_l_main_sweep_cont_dec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT DEC カウントダウンタイムリミットになった場合
	else if (m_l_main_sweep_cont_dec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_main_sweep_cont_dec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_main_sweep_cont_dec_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "-SAW") != 0)) {
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if ((m_l_main_sweep_cont_dec_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "-TRIANGLE") != 0))
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT DECINC カウントダウン中にデータが -+SAW/-+TRIANGLE になった場合
	if (m_l_main_sweep_cont_decinc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "-+SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0))
	{
		// カウントダウン終了
		m_l_main_sweep_cont_decinc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT -+SAW カウントダウンタイムリミットになった場合
	else if (m_l_main_sweep_cont_decinc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_main_sweep_cont_decinc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_main_sweep_cont_decinc_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "-+SAW") != 0))
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-+SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if ((m_l_main_sweep_cont_decinc_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "-+TRIANGLE") != 0))
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-+TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_main_sweep_cont_stop_cntdwn == -1 && m_l_main_sweep_cont_inc_cntdwn == -1 &&
		m_l_main_sweep_cont_incdec_cntdwn == -1 && m_l_main_sweep_cont_dec_cntdwn == -1 &&
		m_l_main_sweep_cont_decinc_cntdwn == -1)
	{
		// INC の場合
		if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "+SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0))
		{
			show_main_sweep_cont_inc64();
		}

		// INCDEC の場合
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "+-SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0))
		{
			show_main_sweep_cont_incdec64();
		}

		// DEC の場合
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "-SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0))
		{
			show_main_sweep_cont_dec64();
		}

		// DECINC の場合
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "-+SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0))
		{
			show_main_sweep_cont_decinc64();
		}

		// STOP の場合
		else {
			// CONTがONでSTSがRUNNING以外ならば、CONTをOFFする
			if (strcmp(m_sz_sweep_cont, "ON") == 0 &&
				strcmp(m_sz_sweep_sts, "RUNNING") != 0 &&
				m_i_sweep_flag == 1)
			{
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("OFF"));
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("STOP"));

				// SWEEP実行フラグOFF
				m_i_sweep_flag = 0;
			}

			// 完全に停止してから、STOP表示にする
			if (strcmp(m_sz_sweep_cont, "OFF") == 0 &&
				strcmp(m_sz_sweep_cmd, "STOP") == 0 &&
				(strcmp(m_sz_sweep_sts, "INITIALIZE") == 0 ||
				strcmp(m_sz_sweep_sts, "STOP") == 0))
			{

				show_main_sweep_cont_stop64();
			}
		}
	}

	/*** SUB SWEEP-CONT ***********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SC_SWEEP_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cont, sizeof(m_sz_sweep_cont), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_SC_SWEEP_CONT), CString(m_sz_sweep_cont), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SC_SWEEP_CMD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cmd, sizeof(m_sz_sweep_cmd), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_SC_SWEEP_CMD), CString(m_sz_sweep_cmd), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SC_SWEEP_STS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_sts, sizeof(m_sz_sweep_sts), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_SC_SWEEP_STS), CString(m_sz_sweep_sts), nLogEx::debug);
	}

	// SWEEP-CONT STOP カウントダウン中に監視データが STOP になった場合
	if (m_l_sub_sweep_cont_stop_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "OFF") == 0 ||
		strcmp(m_sz_sweep_cmd, "STOP") == 0 ||
		strcmp(m_sz_sweep_sts, "STOP") == 0))
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_stop_cntdwn = -1;

		// SWEEP実行フラグOFF
		m_i_sweep_flag = 0;
	}
	// SWEEP-CONT STOP カウントダウンタイムリミットになった場合
	else if (m_l_sub_sweep_cont_stop_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_stop_cntdwn = -1;
	}

	// SWEEP-CONT INC カウントダウン中にデータが +SAW/+TRIANGLE になった場合
	if (m_l_sub_sweep_cont_inc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "+SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0))
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_inc_cntdwn = -1;

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +SAW カウントダウンタイムリミットになった場合
	else if (m_l_sub_sweep_cont_inc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_inc_cntdwn = -1;
	}
	else if ((m_l_sub_sweep_cont_inc_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "+SAW") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+SAW"));
	}
	else if ((m_l_sub_sweep_cont_inc_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "+TRIANGLE") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+TRIANGLE"));
	}

	// SWEEP-CONT INCDEC カウントダウン中にデータが +-SAW/+-TRIANGLE になった場合
	if (m_l_sub_sweep_cont_incdec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "+-SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0))
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_incdec_cntdwn = -1;

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +-SAW/TRIANGLE カウントダウンタイムリミットになった場合
	else if (m_l_sub_sweep_cont_incdec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_incdec_cntdwn = -1;
	}
	else if ((m_l_sub_sweep_cont_incdec_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "+-SAW") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+-SAW"));
	}
	else if ((m_l_sub_sweep_cont_incdec_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "+-TRIANGLE") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+-TRIANGLE"));
	}

	// SWEEP-CONT DEC カウントダウン中にデータが -SAW/-TRIANGLE になった場合
	if (m_l_sub_sweep_cont_dec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "-SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0))
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_dec_cntdwn = -1;

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT DEC カウントダウンタイムリミットになった場合
	else if (m_l_sub_sweep_cont_dec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_dec_cntdwn = -1;
	}
	else if ((m_l_sub_sweep_cont_dec_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "-SAW") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-SAW"));
	}
	else if ((m_l_sub_sweep_cont_dec_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "-TRIANGLE") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-TRIANGLE"));
	}

	// SWEEP-CONT DECINC カウントダウン中にデータが -+SAW/-+TRIANGLE になった場合 
	if (m_l_sub_sweep_cont_decinc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "-+SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0))
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_decinc_cntdwn = -1;

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT -+SAW カウントダウンタイムリミットになった場合
	else if (m_l_sub_sweep_cont_decinc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_decinc_cntdwn = -1;
	}
	else if ((m_l_sub_sweep_cont_decinc_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "-+SAW") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-+SAW"));
	}
	else if ((m_l_sub_sweep_cont_decinc_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "-+TRIANGLE") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-+TRIANGLE"));
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_sub_sweep_cont_stop_cntdwn == -1 && m_l_sub_sweep_cont_inc_cntdwn == -1 &&
		m_l_sub_sweep_cont_incdec_cntdwn == -1 && m_l_sub_sweep_cont_dec_cntdwn == -1 &&
		m_l_sub_sweep_cont_decinc_cntdwn == -1)
	{
		// INC の場合
		if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "+SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0))
		{
			show_sub_sweep_cont_inc64();
		}

		// INCDEC の場合
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "+-SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0))
		{
			show_sub_sweep_cont_incdec64();
		}

		// DEC の場合
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "-SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0))
		{
			show_sub_sweep_cont_dec64();
		}

		// DECINC の場合
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "-+SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0))
		{
			show_sub_sweep_cont_decinc64();
		}

		// STOP の場合
		else
		{
			// CONTがONでSTSがRUNNING以外ならば、CONTをOFFする
			if (strcmp(m_sz_sweep_cont, "ON") == 0 &&
				strcmp(m_sz_sweep_sts, "RUNNING") != 0 &&
				m_i_sweep_flag == 1)
			{
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("OFF"));
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("STOP"));

				// SWEEP実行フラグOFF
				m_i_sweep_flag = 0;
			}

			// 2013-03-11 完全に停止してから、STOP表示にする
			if (strcmp(m_sz_sweep_cont, "OFF") == 0 &&
				strcmp(m_sz_sweep_cmd, "STOP") == 0 &&
				(strcmp(m_sz_sweep_sts, "INITIALIZE") == 0 ||
				strcmp(m_sz_sweep_sts, "STOP") == 0))
			{
				show_sub_sweep_cont_stop64();
			}
		}
	}


	/*** SWEEP-NM ************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SWEEP_NUM));
	if (obs_adr != NULL)
	{
		m_d_sweep_num = obs_adr->d_data;
	}

	// 数値表示部を更新
	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_sweep_num % 100);
	m_SweepNumBtn.SetWindowTextW(CString(sz_buff));

	// Tri-SWEEP-NUM カウントダウン中にデータが設定値になった場合
	if (m_l_sweep_num_cntdwn >= 0 &&
		m_l_sweep_num_set == (long)m_d_sweep_num)
	{
		// カウントダウン終了
		m_l_sweep_num_cntdwn = -1;
	}
	// Tri-SWEEP-NUM カウントダウンタイムリミットになった場合
	else if (m_l_sweep_num_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sweep_num_cntdwn = -1;

		// 運用ログに表示
		sprintf_s(sz_buff, sizeof(sz_buff), "SWEEP-NUM %2d", m_l_sweep_num_set);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(sz_buff), _T(""), nLogEx::debug);
	}


	/*** DOPP-CONT <<XUP>> 追加 ***********************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DOPP_PRE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dopp_pre, sizeof(m_sz_dopp_pre), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_DOPP_PRE), CString(m_sz_dopp_pre), nLogEx::debug);
	}
	// DOPP-CONT OFF カウントダウン中にデータが OFF になった場合
	if (m_l_dopp_cont_off_cntdwn >= 0 &&
		strcmp(m_sz_dopp_pre, "OFF") == 0)
	{
		// カウントダウン終了
		m_l_dopp_cont_off_cntdwn = -1;
	}
	// DOPP-CONT OFF カウントダウンタイムリミットになった場合 
	else if (m_l_dopp_cont_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_dopp_cont_off_cntdwn = -1;
	}

	// DOPP-CONT ON カウントダウン中にデータが ON になった場合
	if (m_l_dopp_cont_on_cntdwn >= 0 &&
		strcmp(m_sz_dopp_pre, "ON") == 0)
	{
		// カウントダウン終了
		m_l_dopp_cont_on_cntdwn = -1;
	}
	// DOPP-CONT ON カウントダウンタイムリミットになった場合
	else if (m_l_dopp_cont_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_dopp_cont_on_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_dopp_cont_off_cntdwn == -1 && m_l_dopp_cont_on_cntdwn == -1)
	{
		// ON の場合
		if (strcmp(m_sz_dopp_pre, "ON") == 0)
		{
			show_dopp_cont_on64();
		}
		// OFF の場合
		else
		{
			show_dopp_cont_off64();
		}
	}


	/*** TCRMOD MODULATION ****************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_CMD_MOD_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_cmd_mod_x, sizeof(m_sz_cmd_mod_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_CMD_MOD_X), CString(m_sz_cmd_mod_x), nLogEx::debug);
	}

	// TCRMOD MODULATION OFF カウントダウン中にデータが OFF になった場合
	if (m_l_modulation_off_cntdwn >= 0 &&
		strcmp(m_sz_cmd_mod_x, "OFF") == 0)
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
		strcmp(m_sz_cmd_mod_x, "ON") == 0)
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
		if (strcmp(m_sz_cmd_mod_x, "ON") == 0)
		{
			show_modulation_on64();
		}
		// OFF の場合
		else
		{
			show_modulation_off64();
		}
	}

	/*** X-CMDOUT HOLD_TONE 2008-09-01 X-HPA増設対応追加 **********************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_HOLD_TONE_STS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hold_tone_sts_x, sizeof(m_sz_hold_tone_sts_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_HOLD_TONE_STS_X), CString(m_sz_hold_tone_sts_x), nLogEx::debug);
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

	/*** CMD MOD INDEX ********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_CMD_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_cmd_mod_idx = obs_adr->d_data;

		CString tmp;
		tmp.Format(_T("%f"), obs_adr->d_data);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_CMD_MOD_IDX), tmp, nLogEx::debug);

		// 数値表示部を更新
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_cmd_mod_idx);
		m_strIndexCmd = CString(sz_buff);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("INDEX-CMD"), m_strIndexCmd, nLogEx::debug);
	}

	/*** RNG MOD INDEX ********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RNG_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_rng_mod_idx = obs_adr->d_data;

		CString tmp;
		tmp.Format(_T("%f"), obs_adr->d_data);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_RNG_MOD_IDX), tmp, nLogEx::debug);

		// 数値表示部を更新
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_rng_mod_idx);
		m_strIndexRng = CString(sz_buff);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("INDEX-RNG"), m_strIndexRng, nLogEx::debug);
	}

	/*** DATA SOURCE **********************************************************************/
	/* 現在の状態を監視ＤＢから取得                                                       */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DATA_SOURCE));
	if (obs_adr != NULL)
	{
		sprintf_s(m_sz_data_src, sizeof(m_sz_data_src), "%s", obs_adr->sz_sts_name);

		// 状態表示部を更新
		m_strIndexCpr = CString(m_sz_data_src);
	}

	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	/*** SS-RNG / SX-RNG ******************************************************************/
	// 現在の状態を監視ＤＢから取得 
	// 選ばれているレンジ装置が従来型か再生型かでRNG回数の表示元を切り替える
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
	if (obs_adr != NULL)
	{
		sprintf_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), "%s", obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_NORM_REP), CString(m_sz_norm_rep), nLogEx::debug);
	}

	// 再生型が選ばれている場合
	if (strcmp(m_sz_norm_rep, "REGEN") == 0)
	{
		// 再生型測距回数を抽出
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RNG_CNT));
		if (obs_adr != NULL)
		{
			m_d_xx_rng_cnt = obs_adr->d_data;
		}
	}
	// 従来型が選ばれている場合
	else if (strcmp(m_sz_norm_rep, "TURNAROUND") == 0)
	{
		// 従来型測距回数を抽出
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RNG_CNT));
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

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RNG_NUM));
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
	// XX-RNG の数値表示部上の "CONTINUATION" を消す（あれば）
	m_strXXRngCont = _T("");


	/*** COH/INCOH ************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_COH_INCOH));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_coh_incoh, sizeof(m_sz_coh_incoh), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("V-EQU.COHI"), CString(m_sz_coh_incoh), nLogEx::debug);
	}

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
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("CTRL.RNG_FLG"), CString(m_sz_rnging), nLogEx::debug);
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

	// (従来型) レンジ形式切り替え制御の実行結果確認
	if (m_l_dsnrng_turn_start_flag == 1)
	{
		// 従来型切り替え制御実行中
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
		if (obs_adr != NULL)
		{
			strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
		}

		if (strcmp(m_sz_norm_rep, "TURNAROUND") == 0)
		{
			// 制御通知送信 
			m_XXRngNumBtn.GetWindowTextW(str2);
			str1.Format(_T("DX%02d"), _wtoi(str2));
			theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str1);

			// カウントダウン開始
			m_l_rnging_start_cntdwn = RNGING_TIME_XX64_LIMIT;

			//RNG種別フラグを従来型に設定
			m_l_rng_type = RNG_TYPE_NOR;

			// 従来型切り替え制御実行中フラグを戻す
			m_l_dsnrng_turn_start_flag = 0;
		}
	}

	// (再生型) レンジ形式切り替え制御の実行結果確認
	if (m_l_dsnrng_regen_start_flag == 1)
	{
		// 再生型切り替え制御実行中
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
		if (obs_adr != NULL)
		{
			strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
		}

		if (strcmp(m_sz_norm_rep, "REGEN") == 0)
		{
			// 制御通知送信
			m_XXRngNumBtn.GetWindowTextW(str2);
			str1.Format(_T("DX%02d"), _wtoi(str2));
			theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str1);

			// カウントダウン開始
			m_l_rep_rnging_start_cntdwn = REP_RNGING_TIME_XX64_LIMIT;

			// RNG種別フラグを再生型に設定
			m_l_rng_type = RNG_TYPE_REP;

			// 再生型切り替え制御実行中フラグを戻す
			m_l_dsnrng_regen_start_flag = 0;
		}
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
			show_rrng_start64();
		}
		// STOP の場合
		else
		{
			show_rnging_stop64();
			show_rrng_stop64();
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

	/*** 変調装置の選択状態確認 2008-09-01 X-HPA増設対応追加 ******************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_MOD_IN_SEL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_mod_in_sel, sizeof(m_sz_mod_in_sel), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("X-IFSW.IF_CH_INPUT_SEL_ANS"), CString(m_sz_mod_in_sel), nLogEx::debug);
	}

	// TCRMODが選択されている場合
	if (strcmp(m_sz_mod_in_sel, "TCRMOD") == 0)
	{
		// TCRMODをセンシティブ、PM MODをインセンシティブにする
		m_HoldToneOffBtn.EnableWindow(FALSE);
		m_HoldToneOnBtn.EnableWindow(FALSE);
		m_ModulationOffBtn.EnableWindow(FALSE);
		m_ModulationOnBtn.EnableWindow(FALSE);
	}
	// PM MODが選択されている場合
	else if (strcmp(m_sz_mod_in_sel, "DSN-MODEM") == 0)
	{
		// TCRMODをインセンシティブ、PM MODをセンシティブにする
		m_HoldToneOffBtn.EnableWindow(TRUE);
		m_HoldToneOnBtn.EnableWindow(TRUE);
		m_ModulationOffBtn.EnableWindow(TRUE);
		m_ModulationOnBtn.EnableWindow(TRUE);
	}
	// いづれの状態でもない場合
	else
	{
		// どちらもインセンシティブにする
		m_HoldToneOffBtn.EnableWindow(FALSE);
		m_HoldToneOnBtn.EnableWindow(FALSE);
		m_ModulationOffBtn.EnableWindow(FALSE);
		m_ModulationOnBtn.EnableWindow(FALSE);
	}

	UpdateData(FALSE);
}

/*============================================================================*/
/*! CTrsControlXX

-# 送信制御画面状態表示設定

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_opsc_condition34()
{
	TRACE(_T("show_opsc_condition\n"));

	char sz_buff[64];
	char szRfPowerSts[32];
	CString str1, str2;

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 制御権は常に有り
	m_i_priv_flag = 1;

	/*-- TX CONTROL ----------------------------------------------------------------------*/
	// RF POWER ON ENABLE 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_RF_ON_ENBL_X), CString(m_sz_rf_on_enbl_x), nLogEx::debug);
	}

	// RF-TRIP NOUSE ON ENABLE 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_NO_USE_ENBL_X), CString(m_sz_no_use_enbl_x), nLogEx::debug);
	}

	// UP-LINK CONTROL 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_UPLINK_CONT_X));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_uplink_cont_x, sizeof(m_sz_uplink_cont_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_UPLINK_CONT_X), CString(m_sz_uplink_cont_x), nLogEx::debug);
	}

	// RF POWER ON DETECT ANSWER 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_RF_ON_DET_ANS_X));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_det_ans_x, sizeof(m_sz_rf_on_det_ans_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX34_RF_ON_DET_ANS_X), CString(m_sz_rf_on_det_ans_x), nLogEx::debug);
	}

	// RF POWER ON DETECT ANSWER 状態 <<XUP>> S-TX用に変更
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_RF_ON_DET_ANS_S));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_det_ans_s, sizeof(m_sz_rf_on_det_ans_s), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX34_RF_ON_DET_ANS_S), CString(m_sz_rf_on_det_ans_s), nLogEx::debug);
	}


	/*** リモート／ローカル 状態取得 ******************************************************/

	// S-TXSYN リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_X_TXSYN_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_txsyn_rl, sizeof(m_sz_x_txsyn_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX34_X_TXSYN_RL), CString(m_sz_x_txsyn_rl), nLogEx::debug);
	}

	// DSN-MODEM リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_DSNTCR_RL), CString(m_sz_dsntcr_rl), nLogEx::debug);
	}

	// X-UL_CNT リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_X_UL_CNT_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_ul_cnt_rl, sizeof(m_sz_x_ul_cnt_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_X_UL_CNT_RL), CString(m_sz_x_ul_cnt_rl), nLogEx::debug);
	}

	// X-UC リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_X_UC_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_uc_rl, sizeof(m_sz_x_uc_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_X_UC_RL), CString(m_sz_x_uc_rl), nLogEx::debug);
	}

	// X-RF_IF リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_X_RF_IF_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_rf_if_rl, sizeof(m_sz_x_rf_if_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_X_RF_IF_RL), CString(m_sz_x_rf_if_rl), nLogEx::debug);
	}

	/*** アラームチェック  <<XUP>> X-TX用に変更 *******************************************/
#if 1
	strcpy_s(m_sz_alarm_x, sizeof(m_sz_alarm_x), "OFF");
#else
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_ALARM_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_alarm_x, sizeof(m_sz_alarm_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("X-TX.TOTAL_FAULT"), CString(m_sz_alarm_x), nLogEx::debug);
	}
#endif

	// アラーム時
	if (strcmp(m_sz_alarm_x, "ON") == 0)
	{
		// 枠を赤くする
		m_grRfPower.SetBackColor(COLOR_ALARM);
		m_grNoUse.SetBackColor(COLOR_ALARM);
		m_grSweepCont.SetBackColor(COLOR_ALARM);
		m_grSweepContSub.SetBackColor(COLOR_ALARM);
		m_grDoppCont.SetBackColor(COLOR_ALARM);
		m_grModulation.SetBackColor(COLOR_ALARM);
		m_grHoldTone.SetBackColor(COLOR_ALARM);
	}

	// アラーム解除
	else
	{
		// 枠の色を元に戻す
		m_grRfPower.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grNoUse.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepCont.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepContSub.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grDoppCont.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grModulation.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grHoldTone.SetBackColor(GetSysColor(COLOR_3DFACE));
	}


	/*** RF-POWER *************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_POWER_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_power_x, sizeof(m_sz_rf_power_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_RF_POWER_X), CString(m_sz_rf_power_x), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_HPA_RF_POWER_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hpa_rf_power_x, sizeof(m_sz_hpa_rf_power_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX34_HPA_RF_POWER_X), CString(m_sz_hpa_rf_power_x), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_XPA_SEL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xpa_sel, sizeof(m_sz_xpa_sel), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX34_XPA_SEL), CString(m_sz_xpa_sel), nLogEx::debug);
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
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NO_USE_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_x, sizeof(m_sz_no_use_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_NO_USE_X), CString(m_sz_no_use_x), nLogEx::debug);
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


	/*** MAIN SWEEP-CONT ***********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_TC_SWEEP_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cont, sizeof(m_sz_sweep_cont), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_TC_SWEEP_CONT), CString(m_sz_sweep_cont), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_TC_SWEEP_CMD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cmd, sizeof(m_sz_sweep_cmd), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_TC_SWEEP_CMD), CString(m_sz_sweep_cmd), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_TC_SWEEP_STS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_sts, sizeof(m_sz_sweep_sts), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_TC_SWEEP_STS), CString(m_sz_sweep_sts), nLogEx::debug);
	}
	// SWEEP-CONT STOP カウントダウン中に監視データが STOP になった場合
	if (m_l_main_sweep_cont_stop_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "OFF") == 0 ||
		strcmp(m_sz_sweep_cmd, "STOP") == 0 ||
		strcmp(m_sz_sweep_sts, "STOP") == 0))
	{
		// カウントダウン終了
		m_l_main_sweep_cont_stop_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;

		// SWEEP実行フラグOFF
		m_i_sweep_flag = 0;
	}
	// SWEEP-CONT STOP カウントダウンタイムリミットになった場合
	else if (m_l_main_sweep_cont_stop_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_main_sweep_cont_stop_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}

	// SWEEP-CONT INC カウントダウン中にデータが +SAW/+TRIANGLE になった場合
	if (m_l_main_sweep_cont_inc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "+SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0))
	{
		// カウントダウン終了
		m_l_main_sweep_cont_inc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +SAW カウントダウンタイムリミットになった場合
	else if (m_l_main_sweep_cont_inc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_main_sweep_cont_inc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_main_sweep_cont_inc_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "+SAW") != 0))
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if ((m_l_main_sweep_cont_inc_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "+TRIANGLE") != 0))
	{

		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT INCDEC カウントダウン中にデータが +-SAW/+-TRIANGLE になった場合
	if (m_l_main_sweep_cont_incdec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "+-SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0))
	{
		// カウントダウン終了
		m_l_main_sweep_cont_incdec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +-SAW/TRIANGLE カウントダウンタイムリミットになった場合
	else if (m_l_main_sweep_cont_incdec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_main_sweep_cont_incdec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_main_sweep_cont_incdec_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "+-SAW") != 0))
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+-SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if ((m_l_main_sweep_cont_incdec_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "+-TRIANGLE") != 0))
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+-TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT DEC カウントダウン中にデータが -SAW/-TRIANGLE になった場合
	if (m_l_main_sweep_cont_dec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "-SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0))
	{
		// カウントダウン終了
		m_l_main_sweep_cont_dec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT DEC カウントダウンタイムリミットになった場合
	else if (m_l_main_sweep_cont_dec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_main_sweep_cont_dec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_main_sweep_cont_dec_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "-SAW") != 0)) {
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if ((m_l_main_sweep_cont_dec_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "-TRIANGLE") != 0))
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT DECINC カウントダウン中にデータが -+SAW/-+TRIANGLE になった場合
	if (m_l_main_sweep_cont_decinc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "-+SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0))
	{
		// カウントダウン終了
		m_l_main_sweep_cont_decinc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT -+SAW カウントダウンタイムリミットになった場合
	else if (m_l_main_sweep_cont_decinc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_main_sweep_cont_decinc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_main_sweep_cont_decinc_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "-+SAW") != 0))
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-+SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if ((m_l_main_sweep_cont_decinc_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "-+TRIANGLE") != 0))
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-+TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_main_sweep_cont_stop_cntdwn == -1 && m_l_main_sweep_cont_inc_cntdwn == -1 &&
		m_l_main_sweep_cont_incdec_cntdwn == -1 && m_l_main_sweep_cont_dec_cntdwn == -1 &&
		m_l_main_sweep_cont_decinc_cntdwn == -1)
	{
		// INC の場合
		if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "+SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0))
		{
			show_main_sweep_cont_inc34();
		}

		// INCDEC の場合
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "+-SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0))
		{
			show_main_sweep_cont_incdec34();
		}

		// DEC の場合
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "-SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0))
		{
			show_main_sweep_cont_dec34();
		}

		// DECINC の場合
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "-+SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0))
		{
			show_main_sweep_cont_decinc34();
		}

		// STOP の場合
		else {
			// CONTがONでSTSがRUNNING以外ならば、CONTをOFFする
			if (strcmp(m_sz_sweep_cont, "ON") == 0 &&
				strcmp(m_sz_sweep_sts, "RUNNING") != 0 &&
				m_i_sweep_flag == 1)
			{
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("OFF"));
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("STOP"));

				// SWEEP実行フラグOFF
				m_i_sweep_flag = 0;
			}

			// 完全に停止してから、STOP表示にする
			if (strcmp(m_sz_sweep_cont, "OFF") == 0 &&
				strcmp(m_sz_sweep_sts, "STOP") == 0)
			{

				show_main_sweep_cont_stop34();
			}
		}
	}


	/*** SUB SWEEP-CONT ***********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SC_SWEEP_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cont, sizeof(m_sz_sweep_cont), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_SC_SWEEP_CONT), CString(m_sz_sweep_cont), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SC_SWEEP_CMD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cmd, sizeof(m_sz_sweep_cmd), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_SC_SWEEP_CMD), CString(m_sz_sweep_cmd), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SC_SWEEP_STS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_sts, sizeof(m_sz_sweep_sts), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_SC_SWEEP_STS), CString(m_sz_sweep_sts), nLogEx::debug);
	}

	// SWEEP-CONT STOP カウントダウン中に監視データが STOP になった場合
	if (m_l_sub_sweep_cont_stop_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "OFF") == 0 ||
		strcmp(m_sz_sweep_cmd, "STOP") == 0 ||
		strcmp(m_sz_sweep_sts, "STOP") == 0))
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_stop_cntdwn = -1;

		// SWEEP実行フラグOFF
		m_i_sweep_flag = 0;
	}
	// SWEEP-CONT STOP カウントダウンタイムリミットになった場合
	else if (m_l_sub_sweep_cont_stop_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_stop_cntdwn = -1;
	}

	// SWEEP-CONT INC カウントダウン中にデータが +SAW/+TRIANGLE になった場合
	if (m_l_sub_sweep_cont_inc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "+SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0))
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_inc_cntdwn = -1;

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +SAW カウントダウンタイムリミットになった場合
	else if (m_l_sub_sweep_cont_inc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_inc_cntdwn = -1;
	}
	else if ((m_l_sub_sweep_cont_inc_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "+SAW") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+SAW"));
	}
	else if ((m_l_sub_sweep_cont_inc_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "+TRIANGLE") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+TRIANGLE"));
	}

	// SWEEP-CONT INCDEC カウントダウン中にデータが +-SAW/+-TRIANGLE になった場合
	if (m_l_sub_sweep_cont_incdec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "+-SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0))
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_incdec_cntdwn = -1;

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +-SAW/TRIANGLE カウントダウンタイムリミットになった場合
	else if (m_l_sub_sweep_cont_incdec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_incdec_cntdwn = -1;
	}
	else if ((m_l_sub_sweep_cont_incdec_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "+-SAW") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+-SAW"));
	}
	else if ((m_l_sub_sweep_cont_incdec_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "+-TRIANGLE") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+-TRIANGLE"));
	}

	// SWEEP-CONT DEC カウントダウン中にデータが -SAW/-TRIANGLE になった場合
	if (m_l_sub_sweep_cont_dec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "-SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0))
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_dec_cntdwn = -1;

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT DEC カウントダウンタイムリミットになった場合
	else if (m_l_sub_sweep_cont_dec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_dec_cntdwn = -1;
	}
	else if ((m_l_sub_sweep_cont_dec_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "-SAW") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-SAW"));
	}
	else if ((m_l_sub_sweep_cont_dec_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "-TRIANGLE") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-TRIANGLE"));
	}

	// SWEEP-CONT DECINC カウントダウン中にデータが -+SAW/-+TRIANGLE になった場合 
	if (m_l_sub_sweep_cont_decinc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "-+SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0))
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_decinc_cntdwn = -1;

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT -+SAW カウントダウンタイムリミットになった場合
	else if (m_l_sub_sweep_cont_decinc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_decinc_cntdwn = -1;
	}
	else if ((m_l_sub_sweep_cont_decinc_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "-+SAW") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-+SAW"));
	}
	else if ((m_l_sub_sweep_cont_decinc_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "-+TRIANGLE") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-+TRIANGLE"));
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_sub_sweep_cont_stop_cntdwn == -1 && m_l_sub_sweep_cont_inc_cntdwn == -1 &&
		m_l_sub_sweep_cont_incdec_cntdwn == -1 && m_l_sub_sweep_cont_dec_cntdwn == -1 &&
		m_l_sub_sweep_cont_decinc_cntdwn == -1)
	{
		// INC の場合
		if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "+SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0))
		{
			show_sub_sweep_cont_inc34();
		}

		// INCDEC の場合
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "+-SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0))
		{
			show_sub_sweep_cont_incdec34();
		}

		// DEC の場合
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "-SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0))
		{
			show_sub_sweep_cont_dec34();
		}

		// DECINC の場合
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "-+SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0))
		{
			show_sub_sweep_cont_decinc34();
		}

		// STOP の場合
		else
		{
			// CONTがONでSTSがRUNNING以外ならば、CONTをOFFする
			if (strcmp(m_sz_sweep_cont, "ON") == 0 &&
				strcmp(m_sz_sweep_sts, "RUNNING") != 0 &&
				m_i_sweep_flag == 1)
			{
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("OFF"));
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("STOP"));

				// SWEEP実行フラグOFF
				m_i_sweep_flag = 0;
			}

			// 2013-03-11 完全に停止してから、STOP表示にする
			if (strcmp(m_sz_sweep_cont, "OFF") == 0 &&
				strcmp(m_sz_sweep_sts, "STOP") == 0)
			{
				show_sub_sweep_cont_stop34();
			}
		}
	}


	/*** SWEEP-NM ************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SWEEP_NUM));
	if (obs_adr != NULL)
	{
		m_d_sweep_num = obs_adr->d_data;
	}

	// 数値表示部を更新
	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_sweep_num % 100);
	m_SweepNumBtn.SetWindowTextW(CString(sz_buff));

	// Tri-SWEEP-NUM カウントダウン中にデータが設定値になった場合
	if (m_l_sweep_num_cntdwn >= 0 &&
		m_l_sweep_num_set == (long)m_d_sweep_num)
	{
		// カウントダウン終了
		m_l_sweep_num_cntdwn = -1;
	}
	// Tri-SWEEP-NUM カウントダウンタイムリミットになった場合
	else if (m_l_sweep_num_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sweep_num_cntdwn = -1;

		// 運用ログに表示
		sprintf_s(sz_buff, sizeof(sz_buff), "SWEEP-NUM %2d", m_l_sweep_num_set);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(sz_buff), _T(""), nLogEx::debug);
	}


	/*** DOPP-CONT <<XUP>> 追加 ***********************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DOPP_PRE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dopp_pre, sizeof(m_sz_dopp_pre), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_DOPP_PRE), CString(m_sz_dopp_pre), nLogEx::debug);
	}
	// DOPP-CONT OFF カウントダウン中にデータが OFF になった場合
	if (m_l_dopp_cont_off_cntdwn >= 0 &&
		strcmp(m_sz_dopp_pre, "OFF") == 0)
	{
		// カウントダウン終了
		m_l_dopp_cont_off_cntdwn = -1;
	}
	// DOPP-CONT OFF カウントダウンタイムリミットになった場合 
	else if (m_l_dopp_cont_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_dopp_cont_off_cntdwn = -1;
	}

	// DOPP-CONT ON カウントダウン中にデータが ON になった場合
	if (m_l_dopp_cont_on_cntdwn >= 0 &&
		strcmp(m_sz_dopp_pre, "ON") == 0)
	{
		// カウントダウン終了
		m_l_dopp_cont_on_cntdwn = -1;
	}
	// DOPP-CONT ON カウントダウンタイムリミットになった場合
	else if (m_l_dopp_cont_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_dopp_cont_on_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if (m_l_dopp_cont_off_cntdwn == -1 && m_l_dopp_cont_on_cntdwn == -1)
	{
		// ON の場合
		if (strcmp(m_sz_dopp_pre, "ON") == 0)
		{
			show_dopp_cont_on34();
		}
		// OFF の場合
		else
		{
			show_dopp_cont_off34();
		}
	}


	/*** TCRMOD MODULATION ****************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_CMD_MOD_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_cmd_mod_x, sizeof(m_sz_cmd_mod_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_CMD_MOD_X), CString(m_sz_cmd_mod_x), nLogEx::debug);
	}
	// TCRMOD MODULATION OFF カウントダウン中にデータが OFF になった場合
	if (m_l_modulation_off_cntdwn >= 0 &&
		strcmp(m_sz_cmd_mod_x, "OFF") == 0)
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
		strcmp(m_sz_cmd_mod_x, "ON") == 0)
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
		if (strcmp(m_sz_cmd_mod_x, "ON") == 0)
		{
			show_modulation_on34();
		}
		// OFF の場合
		else
		{
			show_modulation_off34();
		}
	}

	/*** X-CMDOUT HOLD_TONE 2008-09-01 X-HPA増設対応追加 **********************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_HOLD_TONE_STS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hold_tone_sts_x, sizeof(m_sz_hold_tone_sts_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_HOLD_TONE_STS_X), CString(m_sz_hold_tone_sts_x), nLogEx::debug);
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


	/*** CMD MOD INDEX ********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_CMD_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_cmd_mod_idx = obs_adr->d_data;

		// 数値表示部を更新
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_cmd_mod_idx);
		m_strIndexCmd = CString(sz_buff);
	}

	/*** RNG MOD INDEX ********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RNG_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_rng_mod_idx = obs_adr->d_data;

		// 数値表示部を更新
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_rng_mod_idx);
		m_strIndexRng = CString(sz_buff);
	}

	/*** DATA SOURCE **********************************************************************/
	/* 現在の状態を監視ＤＢから取得                                                       */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DATA_SOURCE));
	if (obs_adr != NULL)
	{
		sprintf_s(m_sz_data_src, sizeof(m_sz_data_src), "%-10s", obs_adr->sz_sts_name);

		// 状態表示部を更新
		m_strIndexCpr = CString(m_sz_data_src);
	}

	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	/*** SS-RNG / SX-RNG ******************************************************************/
	// 現在の状態を監視ＤＢから取得 
	// 選ばれているレンジ装置が従来型か再生型かでRNG回数の表示元を切り替える
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
	if (obs_adr != NULL)
	{
		sprintf_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), "%-10s", obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_NORM_REP), CString(m_sz_norm_rep), nLogEx::debug);
	}

	// 再生型が選ばれている場合
	if (strcmp(m_sz_norm_rep, "REGEN") == 0)
	{
		// 再生型測距回数を抽出
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RNG_CNT));
		if (obs_adr != NULL)
		{
			m_d_xx_rng_cnt = obs_adr->d_data;
		}
	}
	// 従来型が選ばれている場合
	else if (strcmp(m_sz_norm_rep, "TURNAROUND") == 0)
	{
		// 従来型測距回数を抽出
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RNG_CNT));
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

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RNG_NUM));
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
	// XX-RNG の数値表示部上の "CONTINUATION" を消す（あれば）
	m_strXXRngCont = _T("");


	/*** COH/INCOH ************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_COH_INCOH));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_coh_incoh, sizeof(m_sz_coh_incoh), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_COH_INCOH), CString(m_sz_coh_incoh), nLogEx::debug);
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
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_RNGING), CString(m_sz_rnging), nLogEx::debug);
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

	// (従来型) レンジ形式切り替え制御の実行結果確認
	if (m_l_dsnrng_turn_start_flag == 1)
	{
		// 従来型切り替え制御実行中
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
		if (obs_adr != NULL)
		{
			strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
		}

		if (strcmp(m_sz_norm_rep, "TURNAROUND") == 0)
		{
			// 制御通知送信 
			m_XXRngNumBtn.GetWindowTextW(str2);
			str1.Format(_T("DX%02d"), _wtoi(str2));
			theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str1);

			// カウントダウン開始
			m_l_rnging_start_cntdwn = RNGING_TIME_XX34_LIMIT;

			//RNG種別フラグを従来型に設定
			m_l_rng_type = RNG_TYPE_NOR;

			// 従来型切り替え制御実行中フラグを戻す
			m_l_dsnrng_turn_start_flag = 0;
		}
	}

	// (再生型) レンジ形式切り替え制御の実行結果確認
	if (m_l_dsnrng_regen_start_flag == 1)
	{
		// 再生型切り替え制御実行中
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
		if (obs_adr != NULL)
		{
			strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
		}

		if (strcmp(m_sz_norm_rep, "REGEN") == 0)
		{
			// 制御通知送信
			m_XXRngNumBtn.GetWindowTextW(str2);
			str1.Format(_T("DX%02d"), _wtoi(str2));
			theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str1);

			// カウントダウン開始
			m_l_rep_rnging_start_cntdwn = REP_RNGING_TIME_XX34_LIMIT;

			// RNG種別フラグを再生型に設定
			m_l_rng_type = RNG_TYPE_REP;

			// 再生型切り替え制御実行中フラグを戻す
			m_l_dsnrng_regen_start_flag = 0;
		}
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

#if 0
	/*** 変調装置の選択状態確認 2008-09-01 X-HPA増設対応追加 ******************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_MOD_IN_SEL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_mod_in_sel, sizeof(m_sz_mod_in_sel), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("X-IFSW.IF_CH_INPUT_SEL_ANS"), CString(m_sz_mod_in_sel), nLogEx::debug);
	}

	// TCRMODが選択されている場合
	if (strcmp(m_sz_mod_in_sel, "TCRMOD") == 0)
	{
		// TCRMODをセンシティブ、PM MODをインセンシティブにする
		m_HoldToneOffBtn.EnableWindow(FALSE);
		m_HoldToneOnBtn.EnableWindow(FALSE);
		m_ModulationOffBtn.EnableWindow(FALSE);
		m_ModulationOnBtn.EnableWindow(FALSE);
	}
	// PM MODが選択されている場合
	else if (strcmp(m_sz_mod_in_sel, "DSN-MODEM") == 0)
	{
		// TCRMODをインセンシティブ、PM MODをセンシティブにする
		m_HoldToneOffBtn.EnableWindow(TRUE);
		m_HoldToneOnBtn.EnableWindow(TRUE);
		m_ModulationOffBtn.EnableWindow(TRUE);
		m_ModulationOnBtn.EnableWindow(TRUE);
	}
	// いづれの状態でもない場合
	else
	{
		// どちらもインセンシティブにする
		m_HoldToneOffBtn.EnableWindow(FALSE);
		m_HoldToneOnBtn.EnableWindow(FALSE);
		m_ModulationOffBtn.EnableWindow(FALSE);
		m_ModulationOnBtn.EnableWindow(FALSE);
	}
#endif
	UpdateData(FALSE);
}


/*============================================================================*/
/*! CTrsControlXX

-# カウントダウンタイムアウト

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::count_down()
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
	if (m_l_main_sweep_cont_stop_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT STOP カウントダウン:%d\n"), m_l_main_sweep_cont_stop_cntdwn);
		m_l_main_sweep_cont_stop_cntdwn--;
	}

	// SWEEP-CONT INC
	if (m_l_main_sweep_cont_inc_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT INC カウントダウン:%d\n"), m_l_main_sweep_cont_inc_cntdwn);
		m_l_main_sweep_cont_inc_cntdwn--;
	}

	// SWEEP-CONT INCDEC
	if (m_l_main_sweep_cont_incdec_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT +-SAW/TRIANGLE カウントダウン:%d\n"), m_l_main_sweep_cont_incdec_cntdwn);
		m_l_main_sweep_cont_incdec_cntdwn--;
	}

	// SWEEP-CONT DEC
	if (m_l_main_sweep_cont_dec_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT DEC カウントダウン:%d\n"), m_l_main_sweep_cont_dec_cntdwn);
		m_l_main_sweep_cont_dec_cntdwn--;
	}

	// SWEEP-CONT DECINC
	if (m_l_main_sweep_cont_decinc_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT -+SAWC カウントダウン:%d\n"), m_l_main_sweep_cont_decinc_cntdwn);
		m_l_main_sweep_cont_decinc_cntdwn--;
	}

	// SWEEP-CONT(SUB) STOP
	if (m_l_sub_sweep_cont_stop_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT STOP カウントダウン:%d\n"), m_l_sub_sweep_cont_stop_cntdwn);
		m_l_sub_sweep_cont_stop_cntdwn--;
	}

	// SWEEP-CONT(SUB) INC
	if (m_l_sub_sweep_cont_inc_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT INC カウントダウン:%d\n"), m_l_sub_sweep_cont_inc_cntdwn);
		m_l_sub_sweep_cont_inc_cntdwn--;
	}

	// SWEEP-CONT(SUB) INCDEC
	if (m_l_sub_sweep_cont_incdec_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT +-SAW/TRIANGLE カウントダウン:%d\n"), m_l_sub_sweep_cont_incdec_cntdwn);
		m_l_sub_sweep_cont_incdec_cntdwn--;
	}

	// SWEEP-CONT(SUB) DEC
	if (m_l_sub_sweep_cont_dec_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT DEC カウントダウン:%d\n"), m_l_sub_sweep_cont_dec_cntdwn);
		m_l_sub_sweep_cont_dec_cntdwn--;
	}

	// SWEEP-CONT(SUB) DECINC
	if (m_l_sub_sweep_cont_decinc_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT -+SAWC カウントダウン:%d\n"), m_l_sub_sweep_cont_decinc_cntdwn);
		m_l_sub_sweep_cont_decinc_cntdwn--;
	}

	// SWEEP-NUM
	if (m_l_sweep_num_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-NUM カウントダウン:%d\n"), m_l_sweep_num_cntdwn);
		m_l_sweep_num_cntdwn--;
	}

	// DOPP-CONT OFF
	if (m_l_dopp_cont_off_cntdwn > 0) 
	{
		TRACE(_T("DOPP-CONT OFF カウントダウン:%d\n"), m_l_dopp_cont_off_cntdwn);
		m_l_dopp_cont_off_cntdwn--;
	}

	// DOPP-CONT ON
	if (m_l_dopp_cont_on_cntdwn > 0) 
	{
		TRACE(_T("DOPP-CONT ON カウントダウン:%d\n"), m_l_dopp_cont_on_cntdwn);
		m_l_dopp_cont_on_cntdwn--;
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
	if (m_l_hold_tone_off_cntdwn > 0)
	{
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
/*! CTrsControlXX

-# RF-POWER OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_rf_power_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// OFF 側
	if ((strcmp(m_sz_lan_x_tx, "接続") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") == 0) &&
		(strcmp(m_sz_lan_x_uc, "接続") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "接続") == 0))
	{
		// リモート（選択）
		m_RfPwrOffBtn.SetColor(COLOR_REMOTES);
	}
	else 
	{
		// ローカル
		m_RfPwrOffBtn.SetColor(COLOR_LOCAL);
	}

	// ON 側
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_uc_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_rf_on_enbl_x, "ON") == 0) &&
		(strcmp(m_sz_lan_x_tx, "接続") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") == 0) &&
		(strcmp(m_sz_lan_x_uc, "接続") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// S-TXのLAN接続状態を確認する
		if (strcmp(m_sz_lan_s_tx, "接続") == 0) 
		{
			// S-TX RF DET ANS を確認する
			if (strcmp(m_sz_rf_on_det_ans_s, "OFF") == 0) 
			{
				// リモート（非選択）
				m_RfPwrOnBtn.SetColor(COLOR_REMOTE);
			}
			else 
			{
				// ローカル
				m_RfPwrOnBtn.SetColor(COLOR_LOCAL);
			}
		}
		else 
		{
			// リモート（非選択）
			m_RfPwrOnBtn.SetColor(COLOR_REMOTE);
		}
	}
	else 
	{
		// ローカル
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# RF-POWER OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_rf_power_off34()
{
	char	szRfOnEnblSts[32];

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_HPA_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hpa_rf_on_enbl_x, sizeof(m_sz_hpa_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
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

	if ((strcmp(m_sz_lan_x_ul_cnt, "接続") == 0) &&
		(strcmp(m_sz_lan_x_uc, "接続") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "接続") == 0))
	{
		// リモート（選択）
		m_RfPwrOffBtn.SetColor(COLOR_REMOTES);
	}
	else
	{
		// ローカル
		m_RfPwrOffBtn.SetColor(COLOR_LOCAL);
	}

	// ON 側
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_uc_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&
		(strcmp(szRfOnEnblSts, "ON") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") == 0) &&
		(strcmp(m_sz_lan_x_uc, "接続") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// S-TXのLAN接続状態を確認する
		if (strcmp(m_sz_lan_s_tx, "接続") == 0)
		{
			// S-TX RF DET ANS を確認する
			if (strcmp(m_sz_rf_on_det_ans_s, "OFF") == 0)
			{
				// リモート（非選択）
				m_RfPwrOnBtn.SetColor(COLOR_REMOTE);
			}
			else
			{
				// ローカル
				m_RfPwrOnBtn.SetColor(COLOR_LOCAL);
			}
		}
		else
		{
			// リモート（非選択）
			m_RfPwrOnBtn.SetColor(COLOR_REMOTE);
		}
	}
	else
	{
		// ローカル
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# RF-POWER ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_rf_power_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_DET_ANS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans_x, sizeof(m_sz_rf_on_det_ans_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	// HPA-2増設対応
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_HPA_SEL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hpa_sel, sizeof(m_sz_hpa_sel), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_DET_ANS_X_H2));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans_x_h2, sizeof(m_sz_rf_on_det_ans_x_h2), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// ON 側
	if ((strcmp(m_sz_lan_x_tx, "接続") != 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") != 0) &&
		(strcmp(m_sz_lan_x_uc, "接続") != 0) &&
		(strcmp(m_sz_lan_x_rf_if, "接続") != 0))
	{ 
		// ローカル
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);
	}
	else if (((strcmp(m_sz_lan_x_ul_cnt, "接続") != 0)) && 
		(((strcmp(m_sz_hpa_sel, "NO1") == 0) && (strcmp(m_sz_rf_on_det_ans_x, "OFF") == 0))
		|| ((strcmp(m_sz_hpa_sel, "NO2") == 0) && (strcmp(m_sz_rf_on_det_ans_x_h2, "OFF") == 0)))) 
	{
		// リモート（選択）/ ローカル
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES, COLOR_LOCAL, TRUE);
	}
	else if (((strcmp(m_sz_hpa_sel, "NO1") == 0) && (strcmp(m_sz_rf_on_det_ans_x, "OFF") == 0))
		|| ((strcmp(m_sz_hpa_sel, "NO2") == 0) && (strcmp(m_sz_rf_on_det_ans_x_h2, "OFF") == 0))) 
	{
		// リモート（選択）/ リモート（非選択）
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES, COLOR_REMOTE, TRUE);
	}
	else 
	{
		// リモート（選択）
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF 側
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_uc_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_rf_on_enbl_x, "OFF") == 0) &&
		(strcmp(m_sz_lan_x_tx, "接続") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") == 0) &&
		(strcmp(m_sz_lan_x_uc, "接続") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_RfPwrOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_RfPwrOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RfPwrOnBtn.Invalidate();
	m_RfPwrOffBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# RF-POWER ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_rf_power_on34()
{
	char	szRfOnEnblSts[32];
	char	szRfOnDetAnsSts[32];

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_RF_ON_DET_ANS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans_x, sizeof(m_sz_rf_on_det_ans_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_HPA_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hpa_rf_on_enbl_x, sizeof(m_sz_hpa_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_HPA_RF_ON_DET_ANS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hpa_rf_on_det_ans_x, sizeof(m_sz_hpa_rf_on_det_ans_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
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

	if ((strcmp(m_sz_lan_x_ul_cnt, "接続") != 0) &&
		(strcmp(m_sz_lan_x_uc, "接続") != 0) &&
		(strcmp(m_sz_lan_x_rf_if, "接続") != 0))
	{
		// ローカル
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);
	}
	else if ((((strcmp(m_sz_x_ul_cnt_rl, "LOCAL") == 0) && (strcmp(m_sz_x_uc_rl, "LOCAL") == 0) && (strcmp(m_sz_x_rf_if_rl, "LOCAL") == 0))) && (strcmp(szRfOnDetAnsSts, "OFF") == 0)) 
	{
		// リモート（選択）/ ローカル
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES, COLOR_LOCAL, TRUE);
	}
	else if (strcmp(szRfOnDetAnsSts, "OFF") == 0)
	{
		// リモート（選択）/ リモート（非選択）
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES, COLOR_REMOTE, TRUE);
	}
	else
	{
		// リモート（選択）
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF 側
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_uc_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&
		(strcmp(szRfOnEnblSts, "OFF") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") == 0) &&
		(strcmp(m_sz_lan_x_uc, "接続") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_RfPwrOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_RfPwrOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RfPwrOnBtn.Invalidate();
	m_RfPwrOffBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# NO-USE OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_no_use_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// OFF 側
	if ((strcmp(m_sz_lan_x_tx, "接続") != 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") != 0) &&
		(strcmp(m_sz_lan_x_uc, "接続") != 0) &&
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
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_uc_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_tx, "接続") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") == 0) &&
		(strcmp(m_sz_lan_x_uc, "接続") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
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
/*! CTrsControlXX

-# NO-USE OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_no_use_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// OFF 側
	if ((strcmp(m_sz_lan_x_ul_cnt, "接続") != 0) &&
		(strcmp(m_sz_lan_x_uc, "接続") != 0) &&
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
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_uc_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") == 0) &&
		(strcmp(m_sz_lan_x_uc, "接続") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
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
/*! CTrsControlXX

-# NO-USE ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_no_use_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// ON 側
	if ((strcmp(m_sz_lan_x_tx, "接続") != 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") != 0) &&
		(strcmp(m_sz_lan_x_uc, "接続") != 0) &&
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
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_uc_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_tx, "接続") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") == 0) &&
		(strcmp(m_sz_lan_x_uc, "接続") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "接続") == 0) &&
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
	m_NoUseOnBtn.Invalidate();
	m_NoUseOffBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# NO-USE ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_no_use_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// ON 側
	if ((strcmp(m_sz_lan_x_ul_cnt, "接続") != 0) &&
		(strcmp(m_sz_lan_x_uc, "接続") != 0) &&
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
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_uc_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "接続") == 0) &&
		(strcmp(m_sz_lan_x_uc, "接続") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "接続") == 0) &&
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
	m_NoUseOnBtn.Invalidate();
	m_NoUseOffBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_stop64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_stop34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) INC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_inc64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) INC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_inc34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) INCDEC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_incdec64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) INCDEC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_incdec34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) DEC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_dec64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) DEC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_dec34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) DECINC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_decinc64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) DECINC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_decinc34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_stop64()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_stop34()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) INC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_inc64()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) INC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_inc34()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) INCDEC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_incdec64()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) INCDEC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_incdec34()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) DEC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_dec64()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) DEC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_dec34()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) DECINC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_decinc64()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) DECINC 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_decinc34()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// その他のボタン
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "接続") == 0) && (m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# DOPP-CONT OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_dopp_cont_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	// OFF 側
	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_DoppContOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_DoppContOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON 側
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "接続") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_DoppContOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_DoppContOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_DoppContOffBtn.Invalidate();
	m_DoppContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# DOPP-CONT OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_dopp_cont_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	// OFF 側
	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_DoppContOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_DoppContOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON 側
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_DoppContOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_DoppContOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_DoppContOffBtn.Invalidate();
	m_DoppContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# DOPP-CONT ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_dopp_cont_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	// ON 側
	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0) 
	{
		// ローカル
		m_DoppContOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_DoppContOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF 側
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "接続") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_DoppContOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ローカル
		m_DoppContOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_DoppContOffBtn.Invalidate();
	m_DoppContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# DOPP-CONT ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_dopp_cont_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	// ON 側
	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
	{
		// ローカル
		m_DoppContOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_DoppContOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF 側
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "接続") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_DoppContOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_DoppContOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_DoppContOffBtn.Invalidate();
	m_DoppContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# MODULATION OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_modulation_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	// OFF 側
	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
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
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "接続") == 0) &&
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
/*! CTrsControlXX

-# MODULATION OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_modulation_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	// OFF 側
	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
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
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "接続") == 0) &&
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
/*! CTrsControlXX

-# MODULATION ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_modulation_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	// ON 側
	if (strcmp (m_sz_lan_x_txsyn, "接続") != 0)
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
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "接続") == 0) &&
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
/*! CTrsControlXX

-# MODULATION ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_modulation_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	// ON 側
	if (strcmp(m_sz_lan_x_txsyn, "接続") != 0)
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
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "接続") == 0) &&
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
/*! CTrsControlXX

-# HOLD-TONE OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_hold_tone_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_CMDOUT));
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
	if ((strcmp(m_sz_lan_x_cmdout, "接続") == 0) &&
		(m_i_priv_flag == 1)) 
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
/*! CTrsControlXX

-# HOLD-TONE OFF 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_hold_tone_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_CMDOUT));
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
	if ((strcmp(m_sz_lan_x_cmdout, "接続") == 0) &&
		(m_i_priv_flag == 1))
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
/*! CTrsControlXX

-# HOLD-TONE ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_hold_tone_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_CMDOUT));
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
	if ((strcmp(m_sz_lan_x_cmdout, "接続") == 0) &&
		(m_i_priv_flag == 1)) 
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
/*! CTrsControlXX

-# HOLD-TONE ON 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_hold_tone_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_CMDOUT));
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
	if ((strcmp(m_sz_lan_x_cmdout, "接続") == 0) &&
		(m_i_priv_flag == 1))
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
/*! CTrsControlXX

-# X/X-COH COH 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_coh_incoh_coh64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_vequ, "接続") != 0) 
	{
		// ローカル
		m_XXCohCohBtn.SetColor(COLOR_LOCAL);
		m_XXCohInCohBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		if (m_i_priv_flag == 1) 
		{
			// リモート（非選択）
			m_XXCohInCohBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ローカル
			m_XXCohInCohBtn.SetColor(COLOR_LOCAL);
		}

		// リモート（選択）
		m_XXCohCohBtn.SetColor(COLOR_REMOTES);
	}

	// 更新
	m_XXCohCohBtn.Invalidate();
	m_XXCohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# X/X-COH COH 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_coh_incoh_coh34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_vequ, "接続") != 0)
	{
		// ローカル
		m_XXCohCohBtn.SetColor(COLOR_LOCAL);
		m_XXCohInCohBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		if (m_i_priv_flag == 1)
		{
			// リモート（非選択）
			m_XXCohInCohBtn.SetColor(COLOR_REMOTE);
		}
		else
		{
			// ローカル
			m_XXCohInCohBtn.SetColor(COLOR_LOCAL);
		}

		// リモート（選択）
		m_XXCohCohBtn.SetColor(COLOR_REMOTES);
	}

	// 更新
	m_XXCohCohBtn.Invalidate();
	m_XXCohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# X/X-COH INCOH 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_coh_incoh_incoh64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_vequ, "接続") != 0) 
	{
		// ローカル
		m_XXCohInCohBtn.SetColor(COLOR_LOCAL);
		m_XXCohCohBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
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
		m_XXCohInCohBtn.SetColor(COLOR_REMOTES);
	}

	// 更新
	m_XXCohCohBtn.Invalidate();
	m_XXCohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# X/X-COH INCOH 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_coh_incoh_incoh34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_vequ, "接続") != 0)
	{
		// ローカル
		m_XXCohInCohBtn.SetColor(COLOR_LOCAL);
		m_XXCohCohBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
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
		m_XXCohInCohBtn.SetColor(COLOR_REMOTES);
	}

	// 更新
	m_XXCohCohBtn.Invalidate();
	m_XXCohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# RNGING STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_rnging_stop64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_XX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rarr, sizeof(m_sz_lan_xx_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	// 切り替え実行中フラグが立っている場合は、制御中にする
	if (m_l_dsnrng_turn_start_flag == 1) 
	{
		m_RngingStopBtn.SetColor(COLOR_CTRLIN);
		m_RngingStartBtn.SetColor(COLOR_CTRLIN);
		return;
	}

	// XX-RARR LAN接続時
	if ((strcmp(m_sz_lan_xx_rarr, "接続") == 0) &&
		(strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
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

	// 更新
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# RNGING STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_rnging_stop34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_XX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rarr, sizeof(m_sz_lan_xx_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_P_FLG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_pass_flag, sizeof(m_sz_pass_flag), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
	}

	// 切り替え実行中フラグが立っている場合は、制御中にする
	if (m_l_dsnrng_turn_start_flag == 1)
	{
		m_RngingStopBtn.SetColor(COLOR_CTRLIN);
		m_RngingStartBtn.SetColor(COLOR_CTRLIN);
		return;
	}

	// XX-RARR LAN接続時
	if ((strcmp(m_sz_lan_xx_rarr, "接続") == 0) &&
		(strcmp(m_sz_pass_flag, "パス中") == 0) &&
		(strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_RngingStartBtn.SetColor(COLOR_REMOTE);
	}
	// それ以外
	else
	{
		// ローカル
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}

	// インセンシティブの場合は、センシティブにする
	if ((m_RngingStartBtn.IsWindowEnabled() == FALSE) && (strcmp(m_sz_rnging, "開始") != 0))
	{
		m_RngingStartBtn.EnableWindow(TRUE);
		m_RngingStopBtn.EnableWindow(TRUE);
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

	// 更新
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# RNGING START 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_rnging_start64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_XX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rarr, sizeof(m_sz_lan_xx_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
	}

	// 従来型が選ばれていなければ、ボタンをインセンシティブにする
	if ((strcmp(m_sz_norm_rep, "TURNAROUND") != 0) && (m_RngingStartBtn.IsWindowEnabled() != FALSE)) 
	{
		m_RngingStartBtn.EnableWindow(FALSE);
		m_RngingStopBtn.EnableWindow(FALSE);
	}
	else if ((strcmp(m_sz_norm_rep, "TURNAROUND") == 0) && (m_RngingStartBtn.IsWindowEnabled() == FALSE) && (m_l_rng_type == RNG_TYPE_NOR))
	{
		m_RngingStartBtn.EnableWindow(TRUE);
		m_RngingStopBtn.EnableWindow(TRUE);
	}
	// XX-RARR LAN接続時
	if ((strcmp(m_sz_lan_xx_rarr, "接続") == 0) &&
		(strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_norm_rep, "TURNAROUND") == 0) &&
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

	if ((strcmp(m_sz_lan_xx_rarr, "接続") != 0) ||
		(strcmp(m_sz_norm_rep, "TURNAROUND") != 0)) 
	{
		// ローカル
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_RngingStartBtn.SetColor(COLOR_REMOTES);
	}

	// 更新
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# RNGING START 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_rnging_start34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_XX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rarr, sizeof(m_sz_lan_xx_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
	}

	// 従来型が選ばれていなければ、ボタンをインセンシティブにする
	if ((strcmp(m_sz_norm_rep, "TURNAROUND") != 0) && (m_RngingStartBtn.IsWindowEnabled() != FALSE))
	{
		m_RngingStartBtn.EnableWindow(FALSE);
		m_RngingStopBtn.EnableWindow(FALSE);
	}
	else if ((strcmp(m_sz_norm_rep, "TURNAROUND") == 0) && (m_RngingStartBtn.IsWindowEnabled() == FALSE) && (m_l_rng_type == RNG_TYPE_NOR))
	{
		m_RngingStartBtn.EnableWindow(TRUE);
		m_RngingStopBtn.EnableWindow(TRUE);
	}

	// XX-RARR LAN接続時
	if ((strcmp(m_sz_lan_xx_rarr, "接続") == 0) &&
		(strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_norm_rep, "TURNAROUND") == 0) &&
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

	if ((strcmp(m_sz_lan_xx_rarr, "接続") != 0) ||
		(strcmp(m_sz_norm_rep, "TURNAROUND") != 0))
	{
		// ローカル
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_RngingStartBtn.SetColor(COLOR_REMOTES);
	}

	// 更新
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 再生型 STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_rrng_stop64()
{
	struct satinf_t *satinf_adr;
	struct satinf_t *original_adr;
	char *sattbl_adr;
	struct rarrband_t *rarrband_t_adr;	// ＲＡＲＲ使用帯域情報テーブルアドレス

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_XX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rarr, sizeof(m_sz_lan_xx_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_P_FLG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_pass_flag, sizeof(m_sz_pass_flag), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SAT_NAME));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sat_name, sizeof(m_sz_sat_name), obs_adr->sz_sts_name);
	}

	// 切り替え実行中フラグが立っている場合は、制御中にする
	if (m_l_dsnrng_regen_start_flag == 1) 
	{
		m_RRngStopBtn.SetColor(COLOR_CTRLIN);
		m_RRngStartBtn.SetColor(COLOR_CTRLIN);
		return;
	}

	// XX-RARR LAN接続時
	if ((strcmp(m_sz_lan_xx_rarr, "接続") == 0) &&
		(strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// リモート（非選択）
		m_RRngStartBtn.SetColor(COLOR_REMOTE);
	}
	// それ以外
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
			m_RRngStopBtn.EnableWindow(TRUE);
			m_RRngStartBtn.EnableWindow(TRUE);
		}
	}
	else 
	{
		// パス中の場合

		/***
		パス中の場合は、運用中の衛星が再生測距に対応している場合のみセンシティブにする
		***/

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
					if( (rarrband_t_adr->l_rarr_band == RARRBAND_RX_ID) ||
						(rarrband_t_adr->l_rarr_band == RARRBAND_RXKA_ID) )
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

	if (strcmp(m_sz_lan_xx_rarr, "接続") != 0)
	{
		// ローカル
		m_RRngStopBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_RRngStopBtn.SetColor(COLOR_REMOTES);
	}

	// 更新
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 再生型 STOP 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_rrng_stop34()
{
	struct satinf_t *satinf_adr;
	struct satinf_t *original_adr;
	char *sattbl_adr;
	struct rarrband_t *rarrband_t_adr;	// ＲＡＲＲ使用帯域情報テーブルアドレス

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_XX_RRNG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rrng, sizeof(m_sz_lan_xx_rrng), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RRNG_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_rrng_cont, sizeof(m_sz_xx_rrng_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_P_FLG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_pass_flag, sizeof(m_sz_pass_flag), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_CTRL_SAT_NAME));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sat_name, sizeof(m_sz_sat_name), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
	}

	// 切り替え実行中フラグが立っている場合は、制御中にする
	if (m_l_dsnrng_regen_start_flag == 1)
	{
		m_RRngStopBtn.SetColor(COLOR_CTRLIN);
		m_RRngStartBtn.SetColor(COLOR_CTRLIN);
		return;
	}

	// XX-RARR LAN接続時
	if ((strcmp(m_sz_lan_xx_rrng, "接続") == 0) &&
		(strcmp(m_sz_pass_flag, "パス中") == 0) &&
		(strcmp(m_sz_xx_rrng_cont, "REMOTE") == 0) &&
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_RRngStartBtn.SetColor(COLOR_REMOTE);
	}
	// それ以外
	else
	{
		// ローカル
		m_RRngStartBtn.SetColor(COLOR_LOCAL);
	}

	// パス中とパス外で処理を分ける
	if (strcmp(m_sz_pass_flag, MON_STR_PASS) != 0)
	{
		// パス外の場合
		if ((m_RRngStartBtn.IsWindowEnabled() == FALSE) && (strcmp(m_sz_rnging, "開始") != 0))
		{
			// インセンシティブならセンシティブにする
			m_RRngStopBtn.EnableWindow(TRUE);
			m_RRngStartBtn.EnableWindow(TRUE);
		}
	}
	else
	{
		// パス中の場合

		/***
		パス中の場合は、運用中の衛星が再生測距に対応している場合のみセンシティブにする
		***/

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
					if ((m_RRngStartBtn.IsWindowEnabled() == FALSE) && (strcmp(m_sz_rnging, "開始") != 0))
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

	// 更新
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 再生型 START 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_rrng_start64()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_XX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rarr, sizeof(m_sz_lan_xx_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
	}

	// 再生型が選ばれていなければ、ボタンをインセンシティブにする
	if ((strcmp(m_sz_norm_rep, "REGEN") != 0) && (m_RRngStartBtn.IsWindowEnabled() != FALSE))
	{
		m_RRngStopBtn.EnableWindow(FALSE);
		m_RRngStartBtn.EnableWindow(FALSE);
	}
	else if ((strcmp(m_sz_norm_rep, "REGEN") == 0) && (m_RRngStartBtn.IsWindowEnabled() == FALSE) && (m_l_rng_type == RNG_TYPE_REP)) 
	{
		m_RRngStopBtn.EnableWindow(TRUE);
		m_RRngStartBtn.EnableWindow(TRUE);
	}

	// XX-RRNG LAN接続時は緑にする
	if ((strcmp(m_sz_lan_xx_rarr, "接続") == 0) &&
		(strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_norm_rep, "REGEN") == 0) &&
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

	if ((strcmp(m_sz_lan_xx_rarr, "接続") != 0) ||
		(strcmp(m_sz_norm_rep, "REGEN") != 0)) 
	{
		// ローカル
		m_RRngStartBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// リモート（選択）
		m_RRngStartBtn.SetColor(COLOR_REMOTES);
	}

	// 更新
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 再生型 START 状態画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::show_rrng_start34()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_XX_RRNG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rrng, sizeof(m_sz_lan_xx_rrng), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RRNG_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_rrng_cont, sizeof(m_sz_xx_rrng_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
	}

	// 再生型が選ばれていなければ、ボタンをインセンシティブにする
	if ((strcmp(m_sz_norm_rep, "REGEN") != 0) && (m_RRngStartBtn.IsWindowEnabled() != FALSE))
	{
		m_RRngStopBtn.EnableWindow(FALSE);
		m_RRngStartBtn.EnableWindow(FALSE);
	}
	else if ((strcmp(m_sz_norm_rep, "REGEN") == 0) && (m_RRngStartBtn.IsWindowEnabled() == FALSE) && (m_l_rng_type == RNG_TYPE_REP))
	{
		m_RRngStopBtn.EnableWindow(TRUE);
		m_RRngStartBtn.EnableWindow(TRUE);
	}

	// XX-RRNG LAN接続時は緑にする
	if ((strcmp(m_sz_lan_xx_rrng, "接続") == 0) &&
		(strcmp(m_sz_xx_rrng_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_norm_rep, "REGEN") == 0) &&
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

	if ((strcmp(m_sz_lan_xx_rrng, "接続") != 0) ||
		(strcmp(m_sz_norm_rep, "REGEN") != 0))
	{
		// ローカル
		m_RRngStartBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// リモート（選択）
		m_RRngStartBtn.SetColor(COLOR_REMOTES);
	}

	// 更新
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# HOLD TONE ON 状態画面表示示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::printlg_endtime(CString str)
{
	// 監視データ取得

	// 現在時刻取得

	// スイープ完了予定時刻算出

	// opelog出力
}


void CTrsControlXX::OnClose()
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

void CTrsControlXX::CloseDlg()
{
	KillTimer(TID_CONDITION);
	KillTimer(TID_COUNTDOWN);

	// ウィンドウを閉じるメッセージ
	theApp.m_pMainWnd->PostMessageW(eMessage_WindowClose, 0, (LPARAM)this);
}

/*============================================================================*/
/*! CTrsControlXX

-# SWEET CTRLのSub Carrを有効化する

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX::EnableSweetCtrlSubCarr()
{
	if (m_SweepContSubChkBtn.IsWindowEnabled() == FALSE)
	{
		m_SweepContSubChkBtn.EnableWindow(TRUE);
		OnBnClickedTrxxctrlSweetSubCheck();
	}
}
