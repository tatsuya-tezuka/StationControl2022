// TrsControlS20.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrsControlS20.h"
#include "afxdialogex.h"
#include "TrsControlMng.h"
#include "HookMsgBox.h"


// CTrsControlS20 �_�C�A���O

IMPLEMENT_DYNAMIC(CTrsControlS20, CDialogBase)

CTrsControlS20::CTrsControlS20(CWnd* pParent /*=NULL*/)
: CDialogBase(CTrsControlS20::IDD, pParent)
, m_strTxPwrPower(_T(""))
{
	m_i_sweep_flag = 0;
}

CTrsControlS20::~CTrsControlS20()
{
}

void CTrsControlS20::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRSCTRL_RF_OFF_BTN, m_RfPwrOffBtn);
	DDX_Control(pDX, IDC_TRSCTRL_RF_ON_BTN, m_RfPwrOnBtn);
	DDX_Control(pDX, IDC_TRSCTRL_NOUSE_OFF_BTN, m_NoUseOffBtn);
	DDX_Control(pDX, IDC_TRSCTRL_NOUSE_ON_BTN, m_NoUseOnBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEET_STOP_BTN, m_SweepContStopBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEET_INC_BTN, m_SweepContIncBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEET_INCDEC_BTN, m_SweepContIncDecBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEET_DEC_BTN, m_SweepContDecBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SWEET_DECINC_BTN, m_SweepContDecIncBtn);
	DDX_Control(pDX, IDC_TRSCTRL20_SSRNG_OFF_BTN, m_SSRngOffBtn);
	DDX_Control(pDX, IDC_TRSCTRL20_SSRNG_ON_BTN, m_SSRngOnBtn);
	DDX_Control(pDX, IDC_TRSCTRL20_COHINCOH_COH_BTN, m_CohIncohCohBtn);
	DDX_Control(pDX, IDC_TRSCTRL20_COHINCOH_INCOH_BTN, m_CohIncohInCohBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SXRNG_OFF_BTN, m_SXRngOffBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SXRNG_ON_BTN, m_SXRngOnBtn);
	DDX_Control(pDX, IDC_TRSCTRL_RANGE_STOP_BTN, m_RangeStopBtn);
	DDX_Control(pDX, IDC_TRSCTRL_RANGE_START_BTN, m_RangeStartBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SS_MDL_OFF_BTN, m_SSModulationOffBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SS_MDL_ON_BTN, m_SSModulationOnBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SS_HLDT_OFF_BTN, m_SSHoldToneOffBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SS_HLDT_ON_BTN, m_SSHoldToneOnBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SX_MDL_OFF_BTN, m_SXModulationOffBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SX_MDL_ON_BTN, m_SXModulationOnBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SX_HLDT_OFF_BTN, m_SXHoldToneOffBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SX_HLDT_ON_BTN, m_SXHoldToneOnBtn);
	DDX_Control(pDX, IDC_TRSCTRL_TXPR_P_BTN, m_TxPwrPowerBtn);
	DDX_Text(pDX, IDC_TRSCTRL_TXPR_P_EDIT, m_strTxPwrPower);
	DDX_Control(pDX, IDC_TRSCTRL_SS_TCR_CHECK, m_TcrCmdOut1ChkBtn);
	DDX_Control(pDX, IDC_TRSCTRL_SX_TCR_CHECK, m_TcrCmdOut2ChkBtn);
	DDX_Control(pDX, IDC_TRSCTRL_RF_STATIC, m_grRfPower);
	DDX_Control(pDX, IDC_TRSCTRL_NOUSE_STATIC, m_grNoUse);
	DDX_Control(pDX, IDC_TRSCTRL_SWEET_STATIC, m_grSweepCont);
	DDX_Control(pDX, IDC_TRSCTRL_TXPR_STATIC, m_grTxPwr);
	DDX_Control(pDX, IDC_TRSCTRL_TX_STATIC, m_grTxControl);
	DDX_Control(pDX, IDC_TRSCTRL_RNGCTRL_STATIC, m_grRngControl);
	DDX_Control(pDX, IDC_TRSCTRL_SSRNG_STATIC, m_grSSRng);
	DDX_Control(pDX, IDC_TRSCTRL_COHINCOH_STATIC, m_grCohIncoh);
	DDX_Control(pDX, IDC_TRSCTRL_SXRNG_STATIC, m_grSXRng);
	DDX_Control(pDX, IDC_TRSCTRL_RANGE_STATIC, m_grRange);
	DDX_Control(pDX, IDC_TRSCTRL_SS_STATIC, m_grSS);
	DDX_Control(pDX, IDC_TRSCTRL_SS_MDL_STATIC, m_grSSModulation);
	DDX_Control(pDX, IDC_TRSCTRL_SS_HLDT_STATIC, m_grSSHoldTone);
	DDX_Control(pDX, IDC_TRSCTRL_SX_STATIC3, m_grSX);
	DDX_Control(pDX, IDC_TRSCTRL_SX_MDL_STATIC, m_grSXModulation);
	DDX_Control(pDX, IDC_TRSCTRL_SX_HLDT_STATIC, m_grSXHoldTone);
}


BEGIN_MESSAGE_MAP(CTrsControlS20, CDialogBase)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CTrsControlS20::OnMenuClose)
	ON_BN_CLICKED(IDC_TRSCTRL_TXPR_P_BTN, &CTrsControlS20::OnBnClickedTrsctrlTxprPBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SS_TCR_CHECK, &CTrsControlS20::OnBnClickedTrsctrlSsTcrCheck)
	ON_BN_CLICKED(IDC_TRSCTRL_SX_TCR_CHECK, &CTrsControlS20::OnBnClickedTrsctrlSxTcrCheck)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TRSCTRL_RF_OFF_BTN, &CTrsControlS20::OnBnClickedTrsctrlRfOffBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_RF_ON_BTN, &CTrsControlS20::OnBnClickedTrsctrlRfOnBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEET_STOP_BTN, &CTrsControlS20::OnBnClickedTrsctrlSweetStopBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEET_INC_BTN, &CTrsControlS20::OnBnClickedTrsctrlSweetIncBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEET_INCDEC_BTN, &CTrsControlS20::OnBnClickedTrsctrlSweetIncdecBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEET_DEC_BTN, &CTrsControlS20::OnBnClickedTrsctrlSweetDecBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SWEET_DECINC_BTN, &CTrsControlS20::OnBnClickedTrsctrlSweetDecincBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_NOUSE_OFF_BTN, &CTrsControlS20::OnBnClickedTrsctrlNouseOffBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_NOUSE_ON_BTN, &CTrsControlS20::OnBnClickedTrsctrlNouseOnBtn)
	ON_BN_CLICKED(IDC_TRSCTRL20_SSRNG_OFF_BTN, &CTrsControlS20::OnBnClickedTrsctrl20SsrngOffBtn)
	ON_BN_CLICKED(IDC_TRSCTRL20_SSRNG_ON_BTN, &CTrsControlS20::OnBnClickedTrsctrl20SsrngOnBtn)
	ON_BN_CLICKED(IDC_TRSCTRL20_COHINCOH_COH_BTN, &CTrsControlS20::OnBnClickedTrsctrl20CohincohCohBtn)
	ON_BN_CLICKED(IDC_TRSCTRL20_COHINCOH_INCOH_BTN, &CTrsControlS20::OnBnClickedTrsctrl20CohincohIncohBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SXRNG_OFF_BTN, &CTrsControlS20::OnBnClickedTrsctrlSxrngOffBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SXRNG_ON_BTN, &CTrsControlS20::OnBnClickedTrsctrlSxrngOnBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_RANGE_STOP_BTN, &CTrsControlS20::OnBnClickedTrsctrlRangeStopBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_RANGE_START_BTN, &CTrsControlS20::OnBnClickedTrsctrlRangeStartBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SS_MDL_OFF_BTN, &CTrsControlS20::OnBnClickedTrsctrlSsMdlOffBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SS_MDL_ON_BTN, &CTrsControlS20::OnBnClickedTrsctrlSsMdlOnBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SS_HLDT_OFF_BTN, &CTrsControlS20::OnBnClickedTrsctrlSsHldtOffBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SS_HLDT_ON_BTN, &CTrsControlS20::OnBnClickedTrsctrlSsHldtOnBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SX_MDL_OFF_BTN, &CTrsControlS20::OnBnClickedTrsctrlSxMdlOffBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SX_MDL_ON_BTN, &CTrsControlS20::OnBnClickedTrsctrlSxMdlOnBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SX_HLDT_OFF_BTN, &CTrsControlS20::OnBnClickedTrsctrlSxHldtOffBtn)
	ON_BN_CLICKED(IDC_TRSCTRL_SX_HLDT_ON_BTN, &CTrsControlS20::OnBnClickedTrsctrlSxHldtOnBtn)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTrsControlS20 ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CTrsControlS20

-# �t�@�C��-���郁�j���[����

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnMenuClose()
{
	// �����ꂩ�̃{�^�������s���̏ꍇ
	if (m_RfPwrOffBtn.IsColorIgnore2() ||
		m_RfPwrOnBtn.IsColorIgnore2() ||
		m_NoUseOffBtn.IsColorIgnore2() ||
		m_NoUseOnBtn.IsColorIgnore2() ||
		m_SweepContStopBtn.IsColorIgnore2() ||
		m_SweepContIncBtn.IsColorIgnore2() ||
		m_SweepContIncDecBtn.IsColorIgnore2() ||
		m_SweepContDecBtn.IsColorIgnore2() ||
		m_SweepContDecIncBtn.IsColorIgnore2() ||
		m_TxPwrPowerBtn.IsColorIgnore2() ||
		m_SSRngOffBtn.IsColorIgnore2() ||
		m_SSRngOnBtn.IsColorIgnore2() ||
		m_CohIncohCohBtn.IsColorIgnore2() ||
		m_CohIncohInCohBtn.IsColorIgnore2() ||
		m_SXRngOffBtn.IsColorIgnore2() ||
		m_SXRngOnBtn.IsColorIgnore2() ||
		m_RangeStopBtn.IsColorIgnore2() ||
		m_RangeStartBtn.IsColorIgnore2() ||
		m_SSModulationOffBtn.IsColorIgnore2() ||
		m_SSModulationOnBtn.IsColorIgnore2() ||
		m_SSHoldToneOffBtn.IsColorIgnore2() ||
		m_SSHoldToneOnBtn.IsColorIgnore2() ||
		m_SXModulationOffBtn.IsColorIgnore2() ||
		m_SXModulationOnBtn.IsColorIgnore2() ||
		m_SXHoldToneOffBtn.IsColorIgnore2() ||
		m_SXHoldToneOnBtn.IsColorIgnore2())
	{
		CString title;
		GetWindowText(title);

		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("�I�����܂��B\n��낵���ł����H"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			return;
		}
	}

	CloseDlg();
	//	OnOK();
	ShowWindow(SW_HIDE);
}


/*============================================================================*/
/*! CTrsControlS20

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CTrsControlS20::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// RF-POWER
	m_RfPwrOffBtn.SetColor(COLOR_REMOTES);
	m_RfPwrOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_RfPwrOnBtn.SetColor(COLOR_REMOTES);
	m_RfPwrOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// NO-USE
	m_NoUseOffBtn.SetColor(COLOR_REMOTES);
	m_NoUseOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_NoUseOnBtn.SetColor(COLOR_REMOTE);
	m_NoUseOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// SWEEP-CONT
	m_SweepContStopBtn.SetColor(COLOR_REMOTES);
	m_SweepContStopBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepContIncBtn.SetColor(COLOR_REMOTE);
	m_SweepContIncBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepContIncDecBtn.SetColor(COLOR_REMOTE);
	m_SweepContIncDecBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepContDecBtn.SetColor(COLOR_REMOTE);
	m_SweepContDecBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepContDecIncBtn.SetColor(COLOR_REMOTE);
	m_SweepContDecIncBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// SS-RNG
	m_SSRngOffBtn.SetColor(COLOR_REMOTES);
	m_SSRngOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SSRngOnBtn.SetColor(COLOR_REMOTE);
	m_SSRngOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// COH/INCOH
	m_CohIncohCohBtn.SetColor(COLOR_REMOTES);
	m_CohIncohCohBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_CohIncohInCohBtn.SetColor(COLOR_REMOTE);
	m_CohIncohInCohBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// SX-RNG
	m_SXRngOffBtn.SetColor(COLOR_REMOTES);
	m_SXRngOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SXRngOnBtn.SetColor(COLOR_REMOTE);
	m_SXRngOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// RANGE
	m_RangeStopBtn.SetColor(COLOR_REMOTES);
	m_RangeStopBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_RangeStartBtn.SetColor(COLOR_REMOTE);
	m_RangeStartBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// S/S MODULATION
	m_SSModulationOffBtn.SetColor(COLOR_REMOTES);
	m_SSModulationOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SSModulationOnBtn.SetColor(COLOR_REMOTE);
	m_SSModulationOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// S/S HOLD-TONE
	m_SSHoldToneOffBtn.SetColor(COLOR_REMOTES);
	m_SSHoldToneOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SSHoldToneOnBtn.SetColor(COLOR_REMOTE);
	m_SSHoldToneOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// S/X MODULATION
	m_SXModulationOffBtn.SetColor(COLOR_REMOTES);
	m_SXModulationOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SXModulationOnBtn.SetColor(COLOR_REMOTE);
	m_SXModulationOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// S/X HOLD-TONE
	m_SXHoldToneOffBtn.SetColor(COLOR_REMOTES);
	m_SXHoldToneOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SXHoldToneOnBtn.SetColor(COLOR_REMOTE);
	m_SXHoldToneOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// TX PWR
	m_TxPwrPowerBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_TxPwrPowerBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// S/X����Disable
	m_TcrCmdOut1ChkBtn.SetCheck(1);
	m_TcrCmdOut2ChkBtn.SetCheck(0);
	m_SXModulationOffBtn.EnableWindow(FALSE);
	m_SXModulationOnBtn.EnableWindow(FALSE);
	m_SXHoldToneOffBtn.EnableWindow(FALSE);
	m_SXHoldToneOnBtn.EnableWindow(FALSE);


	// �O���[�v�{�b�N�X
	m_grTxControl.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grTxControl.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRfPower.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRfPower.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grNoUse.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grNoUse.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grSweepCont.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grSweepCont.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grTxPwr.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grTxPwr.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRngControl.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRngControl.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grSSRng.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grSSRng.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grCohIncoh.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grCohIncoh.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grSXRng.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grSXRng.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRange.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRange.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grSS.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grSS.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grSSModulation.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grSSModulation.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grSSHoldTone.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grSSHoldTone.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grSX.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grSX.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grSXModulation.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grSXModulation.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grSXHoldTone.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grSXHoldTone.SetGroupType(CGroupBoxEx::eGroupType_Round);


	// �^�C���A�E�g�J�E���g�_�E���l�����ݒ�
	m_l_rf_power_off_cntdwn = -1;
	m_l_rf_power_on_cntdwn = -1;
	m_l_no_use_off_cntdwn = -1;
	m_l_no_use_on_cntdwn = -1;
	m_l_sweep_cont_stop_cntdwn = -1;
	m_l_sweep_cont_inc_cntdwn = -1;
	m_l_sweep_cont_incdec_cntdwn = -1;
	m_l_sweep_cont_dec_cntdwn = -1;
	m_l_sweep_cont_decinc_cntdwn = -1;
	m_l_ss_rng_off_cntdwn = -1;
	m_l_ss_rng_on_cntdwn = -1;
	m_l_sx_rng_off_cntdwn = -1;
	m_l_sx_rng_on_cntdwn = -1;
	m_l_coh_incoh_coh_cntdwn = -1;
	m_l_coh_incoh_incoh_cntdwn = -1;
	m_l_rnging_stop_cntdwn = -1;
	m_l_rnging_start_cntdwn = -1;
	m_l_mod_off_cntdwn1 = -1;
	m_l_mod_on_cntdwn1 = -1;
	m_l_mod_off_cntdwn2 = -1;
	m_l_mod_on_cntdwn2 = -1;
	m_l_hold_tone_off_cntdwn1 = -1;
	m_l_hold_tone_on_cntdwn1 = -1;
	m_l_hold_tone_off_cntdwn2 = -1;
	m_l_hold_tone_on_cntdwn2 = -1;


	// �z�X�g���擾
	memset(m_sz_hostname, (char)NULL, sizeof(m_sz_hostname));
	gethostname(m_sz_hostname, sizeof(m_sz_hostname));

#if 0
	// ���M�����ʏ�ԕ\���^�C���A�E�g�R�[���o�b�N���[�`���N��
	show_opsc_condition();

	// �J�E���g�_�E���^�C���A�E�g�R�[���o�b�N���[�`���N��
	count_down();
#endif


	CenterWindowEx();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CTrsControlS20

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CTrsControlS20::UpdateDlg()
{
	// ���M�����ʏ�ԕ\���^�C���A�E�g�R�[���o�b�N���[�`���N��
	show_opsc_condition();

	// �J�E���g�_�E���^�C���A�E�g�R�[���o�b�N���[�`���N��
	count_down();

	return TRUE;
}


/*============================================================================*/
/*! CTrsControlS20

-# �wTX CONTROL�x-�wRF-POWER�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlRfOffBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_RfPwrOffBtn.SetColor(COLOR_CTRLIN);
	m_RfPwrOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_RF_OUTPUT_CONT"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_rf_power_off_cntdwn = RF_POWER_TIME_S20_LIMIT;

	// �X�V
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wTX CONTROL�x-�wRF-POWER�x-�wON�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlRfOnBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_RfPwrOffBtn.SetColor(COLOR_CTRLIN);
	m_RfPwrOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_RF_OUTPUT_CONT"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_rf_power_on_cntdwn = RF_POWER_TIME_S20_LIMIT;

	// �X�V
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wTX CONTROL�x-�wNO-USE�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlNouseOffBtn()
{
	COLORREF col = m_NoUseOffBtn.GetColor();

	if (col == COLOR_REMOTE)
	{
		// �����[�g�i��I���j

		// �{�^���𐧌䒆�ɂ���
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_NO_USE"), _T("OFF"));

		// �J�E���g�_�E���J�n
		m_l_no_use_off_cntdwn = NO_USE_TIME_S20_LIMIT;
	}
	else if (col == COLOR_CTRLIN)
	{
		// ���䒆

		// �{�^���𐧌䒆�ɂ���
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_NO_USE"), _T("OFF"));

		// �J�E���g�_�E���J�n
		m_l_no_use_off_cntdwn = NO_USE_TIME_S20_LIMIT;

		// �����̃J�E���g�_�E���I��
		m_l_no_use_on_cntdwn = -1;
	}
	else if (col == COLOR_REMOTES)
	{
		// �����[�g�i�I���j

		// �{�^���𐧌䒆�ɂ���
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_NO_USE"),  _T("OFF"));

		// �J�E���g�_�E���J�n
		m_l_no_use_off_cntdwn = NO_USE_TIME_S20_LIMIT;

		// �����̃J�E���g�_�E���I��
		m_l_no_use_on_cntdwn = -1;
	}

	// �X�V
	m_NoUseOffBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wTX CONTROL�x-�wNO-USE�x-�wON�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlNouseOnBtn()
{
	COLORREF col = m_NoUseOnBtn.GetColor();

	if (col == COLOR_REMOTE)
	{
		// �����[�g�i��I���j

		// �{�^���𐧌䒆�ɂ���
		m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_NO_USE"), _T("ON"));

		// �J�E���g�_�E���J�n
		m_l_no_use_on_cntdwn = NO_USE_TIME_S20_LIMIT;
	}
	else if (col == COLOR_CTRLIN)
	{
		// ���䒆

		// �{�^���𐧌䒆�ɂ���
		m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_NO_USE"), _T("ON"));

		// �J�E���g�_�E���J�n
		m_l_no_use_on_cntdwn = NO_USE_TIME_S20_LIMIT;

		// �����̃J�E���g�_�E���I��
		m_l_no_use_off_cntdwn = -1;
	}
	else if (col == COLOR_REMOTES)
	{
		// �����[�g�i�I���j

		// �{�^���𐧌䒆�ɂ���
		m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_NO_USE"), _T("ON"));

		// �J�E���g�_�E���J�n
		m_l_no_use_on_cntdwn = NO_USE_TIME_S20_LIMIT;

		// �����̃J�E���g�_�E���I��
		m_l_no_use_off_cntdwn = -1;
	}

	// �X�V
	m_NoUseOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wSTOP�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlSweetStopBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SweepContStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CONT"), _T("OFF"));
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CMD"), _T("STOP"));

	// �J�E���g�_�E���J�n
	m_l_sweep_cont_stop_cntdwn = SWEEP_CONT_TIME_S20_LIMIT;

	// �X�V
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wINC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlSweetIncBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SweepContStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CONT"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_sweep_cont_inc_cntdwn = SWEEP_CONT_TIME_S20_LIMIT;
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE; /* 1shot-sweep */

	// �X�V
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wINCDEC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlSweetIncdecBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SweepContStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CONT"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_sweep_cont_incdec_cntdwn = SWEEP_CONT_TIME_S20_LIMIT;
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE; /* 1shot-sweep */

	// �X�V
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wDEC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlSweetDecBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SweepContStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CONT"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_sweep_cont_dec_cntdwn = SWEEP_CONT_TIME_S20_LIMIT;
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE; /* 1shot-sweep */

	// �X�V
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wDECINC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlSweetDecincBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SweepContStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CONT"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_sweep_cont_decinc_cntdwn = SWEEP_CONT_TIME_S20_LIMIT;
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE; /* 1shot-sweep */

	// �X�V
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wTX CONTROL�x-�wTX PWR�x-�wPOWER�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlTxprPBtn()
{
	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CTRL.S_TX_RF_PWR"), _T(""));
}


/*============================================================================*/
/*! CTrsControlS20

-# �wRNG CONTROL�x-�wSS-RNG�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrl20SsrngOffBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SSRngOffBtn.SetColor(COLOR_CTRLIN);
	m_SSRngOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("V-EQU.RNG_SS OFF"), _T(""));

	// �J�E���g�_�E���J�n
	m_l_ss_rng_off_cntdwn = SS_RNG_TIME_S20_LIMIT;

	// �X�V
	m_SSRngOffBtn.Invalidate();
	m_SSRngOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS20

-# �wRNG CONTROL�x-�wSS-RNG�x-�wON�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrl20SsrngOnBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SSRngOnBtn.SetColor(COLOR_CTRLIN);
	m_SSRngOffBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("V-EQU.RNG_SX OFF"), _T(""));
	theApp.GetSatelliteData().opctproc(_T("V-EQU.RNG_SS ON"), _T(""));
	theApp.GetSatelliteData().opctproc(_T("S-TXFCONV.IF_LINE_SEL"), _T("S-S"));

	// �J�E���g�_�E���J�n
	m_l_ss_rng_on_cntdwn = SS_RNG_TIME_S20_LIMIT;

	// �X�V
	m_SSRngOffBtn.Invalidate();
	m_SSRngOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wRNG CONTROL�x-�wSX-RNG�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlSxrngOffBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SXRngOffBtn.SetColor(COLOR_CTRLIN);
	m_SXRngOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("V-EQU.RNG_SX"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_sx_rng_off_cntdwn = SX_RNG_TIME_S20_LIMIT;

	// �X�V
	m_SXRngOffBtn.Invalidate();
	m_SXRngOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wRNG CONTROL�x-�wSX-RNG�x-�wON�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlSxrngOnBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SXRngOnBtn.SetColor(COLOR_CTRLIN);
	m_SXRngOffBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("V-EQU.RNG_SS"), _T("OFF"));
	theApp.GetSatelliteData().opctproc(_T("V-EQU.RNG_SX"), _T("ON"));
	theApp.GetSatelliteData().opctproc(_T("S-TXFCONV.IF_LINE_SEL"), _T("S-X"));

	// �J�E���g�_�E���J�n
	m_l_sx_rng_on_cntdwn = SX_RNG_TIME_S20_LIMIT;

	// �X�V
	m_SXRngOffBtn.Invalidate();
	m_SXRngOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wRNG CONTROL�x-�wCOH/INCOH�x-�wOCH�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrl20CohincohCohBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_CohIncohCohBtn.SetColor(COLOR_CTRLIN);
	m_CohIncohInCohBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("COH"));

	// �J�E���g�_�E���J�n
	m_l_coh_incoh_coh_cntdwn = COH_INCOH_TIME_S20_LIMIT;

	// �X�V
	m_CohIncohCohBtn.Invalidate();
	m_CohIncohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wRNG CONTROL�x-�wCOH/INCOH�x-�wINOCH�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrl20CohincohIncohBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_CohIncohInCohBtn.SetColor(COLOR_CTRLIN);
	m_CohIncohCohBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("INCOH"));

	// �J�E���g�_�E���J�n
	m_l_coh_incoh_incoh_cntdwn = COH_INCOH_TIME_S20_LIMIT;

	// �X�V
	m_CohIncohCohBtn.Invalidate();
	m_CohIncohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wRNG CONTROL�x-�wRANGE�x-�wSTOP�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlRangeStopBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_RangeStopBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING_CANCEL"), _T(","));

	// �J�E���g�_�E���J�n
	m_l_rnging_stop_cntdwn = RNGING_TIME_S20_LIMIT;

	// �X�V
	m_RangeStopBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wRNG CONTROL�x-�wRANGE�x-�wSTART�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlRangeStartBtn()
{
	CString str;
	// �{�^���𐧌䒆�ɂ���
	m_RangeStopBtn.SetColor(COLOR_CTRLIN);
	m_RangeStartBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	COLORREF colSSRngOn = m_SSRngOnBtn.GetColor();
	COLORREF colSXRngOn = m_SXRngOnBtn.GetColor();

	if (colSSRngOn == COLOR_REMOTES && colSXRngOn != COLOR_REMOTES)
	{
		// SS-RNG ON / SX-RNG OFF �̏ꍇ
		str.Format(_T("S-%02d"), SS_RNG_NUM_DEFAULT);
		theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str);
	}
	else if (colSSRngOn != COLOR_REMOTES && colSXRngOn == COLOR_REMOTES)
	{
		// SS-RNG OFF / SX-RNG ON �̏ꍇ
		str.Format(_T("X-%02d"), SS_RNG_NUM_DEFAULT);
		theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str);
	}
	else if (colSSRngOn == COLOR_REMOTES &&	colSXRngOn == COLOR_REMOTES)
	{
		// SS-RNG ON / SX-RNG ON �̏ꍇ
		str.Format(_T("SX%02d"), SS_RNG_NUM_DEFAULT);
		theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str);
	}

	// �J�E���g�_�E���J�n
	m_l_rnging_start_cntdwn = RNGING_TIME_S20_LIMIT;

	// �X�V
	m_RangeStopBtn.Invalidate();
	m_RangeStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wRNG CONTROL�x-�wS/S�x-�wTCR/CMDOUT1�x�`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlSsTcrCheck()
{
	// S/S �� Enable
	m_SSModulationOffBtn.EnableWindow(TRUE);
	m_SSModulationOnBtn.EnableWindow(TRUE);
	m_SSHoldToneOffBtn.EnableWindow(TRUE);
	m_SSHoldToneOnBtn.EnableWindow(TRUE);

	// S/X �� Disable
	m_TcrCmdOut2ChkBtn.SetCheck(0);
	m_SXModulationOffBtn.EnableWindow(FALSE);
	m_SXModulationOnBtn.EnableWindow(FALSE);
	m_SXHoldToneOffBtn.EnableWindow(FALSE);
	m_SXHoldToneOnBtn.EnableWindow(FALSE);
}


/*============================================================================*/
/*! CTrsControlS20

-# �wRNG CONTROL�x-�wS/S�x-�wMODULATION�x-�wOFF�x�`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlSsMdlOffBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SSModulationOffBtn.SetColor(COLOR_CTRLIN);
	m_SSModulationOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.MODULATION"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_mod_off_cntdwn1 = MOD_TIME_S20_LIMIT;

	// �X�V
	m_SSModulationOffBtn.Invalidate();
	m_SSModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wRNG CONTROL�x-�wS/S�x-�wMODULATION�x-�wON�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlSsMdlOnBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SSModulationOnBtn.SetColor(COLOR_CTRLIN);
	m_SSModulationOffBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.MODULATION"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_mod_on_cntdwn1 = MOD_TIME_S20_LIMIT;

	// �X�V
	m_SSModulationOffBtn.Invalidate();
	m_SSModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wRNG CONTROL�x-�wS/S�x-�wHOLD-TONE�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlSsHldtOffBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SSHoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_SSHoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CMDOUT1.HOLD_TONE"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_hold_tone_off_cntdwn1 = HOLD_TONE_TIME_S20_LIMIT;

	// �X�V
	m_SSHoldToneOffBtn.Invalidate();
	m_SSHoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wRNG CONTROL�x-�wS/S�x-�wHOLD-TONE�x-�wON�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlSsHldtOnBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SSHoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_SSHoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CMDOUT1.HOLD_TONE"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_hold_tone_on_cntdwn1 = HOLD_TONE_TIME_S20_LIMIT;

	// �X�V
	m_SSHoldToneOffBtn.Invalidate();
	m_SSHoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wRNG CONTROL�x-�wS/X�x-�wTCR/CMDOUT2�x�`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlSxTcrCheck()
{
	// S/S �� Disable
	m_TcrCmdOut1ChkBtn.SetCheck(0);
	m_SSModulationOffBtn.EnableWindow(FALSE);
	m_SSModulationOnBtn.EnableWindow(FALSE);
	m_SSHoldToneOffBtn.EnableWindow(FALSE);
	m_SSHoldToneOnBtn.EnableWindow(FALSE);

	// S/X �� Enable
	m_SXModulationOffBtn.EnableWindow(TRUE);
	m_SXModulationOnBtn.EnableWindow(TRUE);
	m_SXHoldToneOffBtn.EnableWindow(TRUE);
	m_SXHoldToneOnBtn.EnableWindow(TRUE);
}


/*============================================================================*/
/*! CTrsControlS20

-# �wRNG CONTROL�x-�wS/X�x-�wMODULATION�x-�wOFF�x�`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlSxMdlOffBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SXModulationOffBtn.SetColor(COLOR_CTRLIN);
	m_SXModulationOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("SX-TCRMOD.MODULATION"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_mod_off_cntdwn2 = MOD_TIME_S20_LIMIT;

	// �X�V
	m_SXModulationOffBtn.Invalidate();
	m_SXModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wRNG CONTROL�x-�wS/X�x-�wMODULATION�x-�wON�x�`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlSxMdlOnBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SXModulationOffBtn.SetColor(COLOR_CTRLIN);
	m_SXModulationOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("SX-TCRMOD.MODULATION"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_mod_on_cntdwn2 = MOD_TIME_S20_LIMIT;

	// �X�V
	m_SXModulationOffBtn.Invalidate();
	m_SXModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wRNG CONTROL�x-�wS/X�x-�wHOLD-TONE�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlSxHldtOffBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SXHoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_SXHoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CMDOUT2.HOLD_TONE"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_hold_tone_off_cntdwn2 = HOLD_TONE_TIME_S20_LIMIT;

	// �X�V
	m_SXHoldToneOffBtn.Invalidate();
	m_SXHoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# �wRNG CONTROL�x-�wS/X�x-�wHOLD-TONE�x-�wON�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnBnClickedTrsctrlSxHldtOnBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SXHoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_SXHoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CMDOUT2.HOLD_TONE"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_hold_tone_on_cntdwn2 = HOLD_TONE_TIME_S20_LIMIT;

	// �X�V
	m_SXHoldToneOffBtn.Invalidate();
	m_SXHoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# OnTimer

@param  nIDEvent:�^�C�}�[�̎��ʎq
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_CONDITION)
	{
		// ���M�����ʏ�ԕ\���ݒ�
		show_opsc_condition();
	}
	else if (nIDEvent == TID_COUNTDOWN)
	{
		// �J�E���g�_�E���^�C���A�E�g
		count_down();
	}

	CDialogBase::OnTimer(nIDEvent);
}


/*============================================================================*/
/*! CTrsControlS20

-# ���M�����ʏ�ԕ\���ݒ�

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_opsc_condition()
{
	TRACE(_T("show_opsc_condition\n"));
	KillTimer(TID_CONDITION);

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X

	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT sta = theApp.GetSelectStation();

	CString msg = _T("");

#if 1
	// ���䌠�͏�ɗL��
	m_i_priv_flag = 1;
#else
	// ���䌠�������J������Ă��Ȃ����m�F
	CString strHost = CString(m_sz_hostname);
	if (strHost.Left(5).CompareNoCase(mHostNameString[1]) == 0)
	{
		// ���͌��ǂ̏ꍇ
		CLogTraceEx::Write(_T(""), _T("CTrsControlS20"), _T("show_opsc_condition"), _T("���͌���"), _T(""), nLogEx::debug);

		memset(m_sz_priv_ssoc, (char)NULL, sizeof(m_sz_priv_ssoc));

		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_PRIV_SSOC));
		if (obs_adr != NULL)
		{
			strcpy_s(m_sz_priv_ssoc, sizeof(m_sz_priv_ssoc), obs_adr->sz_sts_name);

			CLogTraceEx::Write(_T(""), _T("CTrsControlS20"), _T("show_opsc_condition"), _T("CTRL.PRIV_SSOC"), CString(m_sz_priv_ssoc), nLogEx::debug);

			if (strcmp(m_sz_priv_ssoc, "OFF") == 0)
			{
				// ���䌠���擾���Ă��Ȃ��ꍇ
				m_i_priv_flag = 0;
			}
			else
			{
				// ���䌠���擾���Ă���ꍇ
				m_i_priv_flag = 1;
			}
		}
		else
		{
			// ���䌠���擾���Ă��Ȃ��ꍇ
			m_i_priv_flag = 0;
		}
	}
	else
	{
		// �o��ǂ̏ꍇ
		CLogTraceEx::Write(_T(""), _T("CTrsControlS20"), _T("show_opsc_condition"), _T("�o���"), _T(""), nLogEx::debug);

		memset(m_sz_priv_sttn, (char)NULL, sizeof(m_sz_priv_sttn));
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_PRIV_STTN));
		if (obs_adr != NULL)
		{
			strcpy_s(m_sz_priv_sttn, sizeof(m_sz_priv_sttn), obs_adr->sz_sts_name);

			CLogTraceEx::Write(_T(""), _T("CTrsControlS20"), _T("show_opsc_condition"), _T("CTRL.PRIV_STTN"), CString(m_sz_priv_sttn), nLogEx::debug);

			if (strcmp(m_sz_priv_sttn, "OFF") == 0)
			{
				// ���䌠���擾���Ă��Ȃ��ꍇ
				m_i_priv_flag = 0;
			}
			else
			{
				// ���䌠���擾���Ă���ꍇ
				m_i_priv_flag = 1;
			}
		}
		else
		{
			// ���䌠���擾���Ă��Ȃ��ꍇ
			m_i_priv_flag = 0;
		}
	}
#endif

	/*-- TX CONTROL ----------------------------------------------------------------------*/

	// RF POWER ON ENABLE ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_RF_ON_ENBL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_enbl, sizeof(m_sz_rf_on_enbl), obs_adr->sz_sts_name);
	}


	// RF-TRIP NOUSE ON ENABLE ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_NO_USE_ENBL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_no_use_enbl, sizeof(m_sz_no_use_enbl), obs_adr->sz_sts_name);
	}

	// UP-LINK CONTROL ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_UPLINK_CONT));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), obs_adr->sz_sts_name);
	}

	// S-TX RF POWER ON DETECT ANSWER ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_RF_ON_DET_ANS));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_det_ans, sizeof(m_sz_rf_on_det_ans), obs_adr->sz_sts_name);
	}

	/*** �����[�g�^���[�J�� ��Ԏ擾 ******************************************************/

	// S-TX �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_S_TX_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_s_tx_rl, sizeof(m_sz_s_tx_rl), obs_adr->sz_sts_name);
	}

	// S-TXSYN �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_S_TXSYN_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_s_txsyn_rl, sizeof(m_sz_s_txsyn_rl), obs_adr->sz_sts_name);
	}


	/*** �A���[���`�F�b�N *****************************************************************/
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_ALARM));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_alarm, sizeof(m_sz_alarm), obs_adr->sz_sts_name);
	}

	// �A���[����
	if (strcmp(m_sz_alarm, "ON") == 0) 
	{
		// �g��Ԃ�����
		m_grRfPower.SetBackColor(COLOR_ALARM);
		m_grNoUse.SetBackColor(COLOR_ALARM);
		m_grSweepCont.SetBackColor(COLOR_ALARM);
	}
	// �A���[������
	else 
	{
		// �g�̐F�����ɖ߂�
		m_grRfPower.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grNoUse.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepCont.SetBackColor(GetSysColor(COLOR_3DFACE));
	}


	/*** RF-POWER *************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_RF_POWER));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_power, sizeof(m_sz_rf_power), obs_adr->sz_sts_name);
	}
	// RF-POWER ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if (m_l_rf_power_on_cntdwn >= 0 &&
		strcmp(m_sz_rf_power, "ON") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_rf_power_on_cntdwn = -1;
	}
	// RF-POWER ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rf_power_on_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_rf_power_on_cntdwn = -1;
	}

	// RF-POWER OFF �J�E���g�_�E�����ɊĎ��f�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_rf_power_off_cntdwn >= 0 &&
		strcmp(m_sz_rf_power, "OFF") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_rf_power_off_cntdwn = -1;
	}
	// RF-POWER OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rf_power_off_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_rf_power_off_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_rf_power_off_cntdwn == -1 && m_l_rf_power_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_rf_power, "ON") == 0) 
		{
			show_rf_power_on();
		}
		// OFF �̏ꍇ
		else 
		{
			/*** ���ꏈ������ *************************************************************/
			show_rf_power_off();
		}
	}


	/*** NO-USE ***************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_NO_USE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use, sizeof(m_sz_no_use), obs_adr->sz_sts_name);
	}
	// NO-USE ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if (m_l_no_use_on_cntdwn >= 0 &&
		strcmp(m_sz_no_use, "ON") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_no_use_on_cntdwn = -1;
	}
	// NO-USE ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_no_use_on_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_no_use_on_cntdwn = -1;
	}

	// NO-USE OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_no_use_off_cntdwn >= 0 &&
		strcmp(m_sz_no_use, "ON") != 0) 
	{
		// �J�E���g�_�E���I��
		m_l_no_use_off_cntdwn = -1;
	}
	// NO-USE OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_no_use_off_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_no_use_off_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_no_use_off_cntdwn == -1 && m_l_no_use_on_cntdwn == -1) 
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_no_use, "ON") == 0) 
		{
			show_no_use_on();
		}
		// OFF �̏ꍇ
		else 
		{
			show_no_use_off();
		}
	}


	/*** SWEEP-CONT ***********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_SWEEP_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cont, sizeof(m_sz_sweep_cont), obs_adr->sz_sts_name);
	}
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_SWEEP_CMD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cmd, sizeof(m_sz_sweep_cmd), obs_adr->sz_sts_name);
	}
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_SWEEP_STS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_sts, sizeof(m_sz_sweep_sts), obs_adr->sz_sts_name);
	}
	// SWEEP-CONT STOP �J�E���g�_�E�����ɊĎ��f�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_sweep_cont_stop_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "OFF") == 0 ||
		strcmp(m_sz_sweep_cmd, "STOP") == 0 ||
		strcmp(m_sz_sweep_sts, "STOP") == 0)) 
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_stop_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP���s�t���OOFF
		m_i_sweep_flag = 0;
	}
	// SWEEP-CONT STOP �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sweep_cont_stop_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_stop_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}

	// SWEEP-CONT INC �J�E���g�_�E�����Ƀf�[�^�� INC �ɂȂ����ꍇ
	if (m_l_sweep_cont_inc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_cmd, "INC") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0)) 
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_inc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT INC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sweep_cont_inc_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_inc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_sweep_cont_inc_cntdwn >= 0) &&
		(strcmp(m_sz_sweep_cmd, "INC") != 0)) 
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE) 
		{
			theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CMD"), _T("INC"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT INC+DEC �J�E���g�_�E�����Ƀf�[�^�� INC+DEC �ɂȂ����ꍇ
	if (m_l_sweep_cont_incdec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_cmd, "INCDEC") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0)) 
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_incdec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT INC+DEC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sweep_cont_incdec_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_incdec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_sweep_cont_incdec_cntdwn >= 0) &&
		(strcmp(m_sz_sweep_cmd, "INCDEC") != 0)) 
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE) 
		{
			theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CMD"), _T("INCDEC"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT DEC �J�E���g�_�E�����Ƀf�[�^�� DEC �ɂȂ����ꍇ
	if (m_l_sweep_cont_dec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_cmd, "DEC") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0)) 
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_dec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT DEC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sweep_cont_dec_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_dec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_sweep_cont_dec_cntdwn >= 0) &&
		(strcmp(m_sz_sweep_cmd, "DEC") != 0)) 
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE) 
		{
			theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CMD"), _T("DEC"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT DEC+INC �J�E���g�_�E�����Ƀf�[�^�� DEC+INC �ɂȂ����ꍇ
	if (m_l_sweep_cont_decinc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_cmd, "DECINC") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0)) 
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_decinc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT DEC+INC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sweep_cont_decinc_cntdwn == 0) 
	{
		/* �J�E���g�_�E���I��                                                             */
		m_l_sweep_cont_decinc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_sweep_cont_decinc_cntdwn >= 0) &&
		(strcmp(m_sz_sweep_cmd, "DECINC") != 0)) 
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE) 
		{
			theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CMD"), _T("DECINC"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_sweep_cont_stop_cntdwn == -1 && m_l_sweep_cont_inc_cntdwn == -1 &&
		m_l_sweep_cont_incdec_cntdwn == -1 && m_l_sweep_cont_dec_cntdwn == -1 &&
		m_l_sweep_cont_decinc_cntdwn == -1) 
	{
		// INC �̏ꍇ
		if (strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_cmd, "INC") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) 
		{
			show_sweep_cont_inc();
		}

		// INC+DEC �̏ꍇ
		else if (strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_cmd, "INCDEC") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) 
		{
			show_sweep_cont_incdec();
		}

		// DEC �̏ꍇ
		else if (strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_cmd, "DEC") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) 
		{
			show_sweep_cont_dec();
		}

		// DEC+INC �̏ꍇ
		else if (strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_cmd, "DECINC") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) 
		{
			show_sweep_cont_decinc();
		}

		// STOP �̏ꍇ
		else 
		{
			// CONT��ON��STS��RUNNING�ȊO�Ȃ�΁ACONT��OFF����
			if (strcmp(m_sz_sweep_cont, "ON") == 0 &&
				strcmp(m_sz_sweep_sts, "RUNNING") != 0 &&
				m_i_sweep_flag == 1)
			{
				theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CONT"), _T("OFF"));
				theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CMD"), _T("STOP"));

				// SWEEP���s�t���OOFF
				m_i_sweep_flag = 0;
			}

			// ���S�ɒ�~���Ă���ASTOP�\���ɂ���
			if (strcmp(m_sz_sweep_cont, "OFF") == 0 &&
				strcmp(m_sz_sweep_cmd, "STOP") == 0 &&
				strcmp(m_sz_sweep_sts, "INITIALIZE") == 0) 
			{

				show_sweep_cont_stop();
			}
		}
	}


	/*** TX PWRL **************************************************************************/
	/* RF_POWER_SEL */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_POWER_SEL));
	if (obs_adr != NULL)
	{
		if (strcmp(m_sz_data_txpower, obs_adr->sz_sts_name) != 0)
		{
			strcpy_s(m_sz_data_txpower, sizeof(m_sz_data_txpower), obs_adr->sz_sts_name);
			m_strTxPwrPower = CString(m_sz_data_txpower);
		}
	}

	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	/*** SS-RNG / SX-RNG ******************************************************************/
	// ���l�\�����ŐV�f�[�^�\��
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_SS_RNG_CNT));
	if (obs_adr != NULL)
	{
		m_d_ss_rng_cnt = obs_adr->d_data;
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_SS_RNG_NUM));
	if (obs_adr != NULL)
	{
		m_d_ss_rng_num = obs_adr->d_data;
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_SX_RNG_CNT));
	if (obs_adr != NULL)
	{
		m_d_sx_rng_cnt = obs_adr->d_data;
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_SX_RNG_NUM));
	if (obs_adr != NULL)
	{
		m_d_sx_rng_num = obs_adr->d_data;
	}


	/*** SS-RNG ***************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_SS_RNG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_ss_rng, sizeof(m_sz_ss_rng), obs_adr->sz_sts_name);
	}

	// SS-RNG OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_ss_rng_off_cntdwn >= 0 &&
		strcmp(m_sz_ss_rng, "OFF") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_ss_rng_off_cntdwn = -1;
	}
	// SS-RNG OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_ss_rng_off_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_ss_rng_off_cntdwn = -1;
	}

	// SS-RNG ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_ss_rng_on_cntdwn >= 0 &&
		strcmp(m_sz_ss_rng, "ON") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_ss_rng_on_cntdwn = -1;
	}
	// SS-RNG ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ 
	else if (m_l_ss_rng_on_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_ss_rng_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_ss_rng_off_cntdwn == -1 && m_l_ss_rng_on_cntdwn == -1) 
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_ss_rng, "ON") == 0) 
		{
			show_ss_rng_on();
		}
		// OFF �̏ꍇ
		else 
		{
			show_ss_rng_off();
		}
	}


	/*** SX-RNG ***************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_SX_RNG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sx_rng, sizeof(m_sz_sx_rng), obs_adr->sz_sts_name);
	}
	// SX-RNG OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_sx_rng_off_cntdwn >= 0 &&
		strcmp(m_sz_sx_rng, "OFF") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_sx_rng_off_cntdwn = -1;
	}
	// SX-RNG OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sx_rng_off_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_sx_rng_off_cntdwn = -1;
	}

	// SX-RNG ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_sx_rng_on_cntdwn >= 0 &&
		strcmp(m_sz_sx_rng, "ON") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_sx_rng_on_cntdwn = -1;
	}
	// SX-RNG ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sx_rng_on_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_sx_rng_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\ 
	if (m_l_sx_rng_off_cntdwn == -1 && m_l_sx_rng_on_cntdwn == -1) 
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_sx_rng, "ON") == 0) 
		{
			show_sx_rng_on();
		}
		// OFF �̏ꍇ
		else 
		{
			show_sx_rng_off();
		}
	}


	/*** COH/INCOH ************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_COH_INCOH));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_coh_incoh, sizeof(m_sz_coh_incoh), obs_adr->sz_sts_name);
	}

	// COH/INCOH COH �J�E���g�_�E�����Ƀf�[�^�� COH �ɂȂ����ꍇ
	if (m_l_coh_incoh_coh_cntdwn >= 0 &&
		strcmp(m_sz_coh_incoh, "COH") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_coh_incoh_coh_cntdwn = -1;
	}
	// COH/INCOH COH �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_coh_incoh_coh_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_coh_incoh_coh_cntdwn = -1;
	}

	// COH/INCOH INCOH �J�E���g�_�E�����Ƀf�[�^�� INCOH �ɂȂ����ꍇ
	if (m_l_coh_incoh_incoh_cntdwn >= 0 &&
		strcmp(m_sz_coh_incoh, "INCOH") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_coh_incoh_incoh_cntdwn = -1;
	}
	// SX-RNG ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_coh_incoh_incoh_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_coh_incoh_incoh_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_coh_incoh_coh_cntdwn == -1 && m_l_coh_incoh_incoh_cntdwn == -1) 
	{
		// INCOH �̏ꍇ
		if (strcmp(m_sz_coh_incoh, "INCOH") == 0) 
		{
			show_coh_incoh_incoh();
		}
		// COH �̏ꍇ
		else 
		{
			show_coh_incoh_coh();
		}
	}


	/*** RNGING ***************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
	}

	// RNGING STOP �J�E���g�_�E�����Ƀf�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_rnging_stop_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "�I��") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_rnging_stop_cntdwn = -1;
	}
	// RNGING STOP �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rnging_stop_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_rnging_stop_cntdwn = -1;
	}

	// RNGING START �J�E���g�_�E�����Ƀf�[�^�� START �ɂȂ����ꍇ
	if (m_l_rnging_start_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "�J�n") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_rnging_start_cntdwn = -1;
	}
	// RNGING START �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rnging_start_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_rnging_start_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_rnging_stop_cntdwn == -1 && m_l_rnging_start_cntdwn == -1) 
	{
		// START �̏ꍇ
		if (strcmp(m_sz_rnging, "�J�n") == 0) 
		{
			show_rnging_start();
		}
		// STOP �̏ꍇ
		else 
		{
			show_rnging_stop();
		}
	}


	/*** S/S MODULATION *******************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_MOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_mod, sizeof(m_sz_mod), obs_adr->sz_sts_name);
	}

	// MOD OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_mod_off_cntdwn1 >= 0 &&
		strcmp(m_sz_mod, "OFF") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_mod_off_cntdwn1 = -1;
	}
	// MOD OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_mod_off_cntdwn1 == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_mod_off_cntdwn1 = -1;
	}

	// MOD ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_mod_on_cntdwn1 >= 0 &&
		strcmp(m_sz_mod, "ON") == 0) 
	{

		// �J�E���g�_�E���I��
		m_l_mod_on_cntdwn1 = -1;
	}
	// MOD ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_mod_on_cntdwn1 == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_mod_on_cntdwn1 = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_mod_off_cntdwn1 == -1 && m_l_mod_on_cntdwn1 == -1) 
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_mod, "ON") == 0) 
		{
			show_ss_modulation_on();
		}
		// OFF �̏ꍇ
		else 
		{
			show_ss_modulation_off();
		}
	}


	/*** S/S HOLD-TONE ********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_HOLD_TONE1));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hold_tone1, sizeof(m_sz_hold_tone1), obs_adr->sz_sts_name);
	}

	// HOLD-TONE OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_hold_tone_off_cntdwn1 >= 0 &&
		strcmp(m_sz_hold_tone1, "OFF") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_off_cntdwn1 = -1;
	}
	// HOLD-TONE OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_hold_tone_off_cntdwn1 == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_off_cntdwn1 = -1;
	}

	// HOLD-TONE ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_hold_tone_on_cntdwn1 >= 0 &&
		strcmp(m_sz_hold_tone1, "ON") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_on_cntdwn1 = -1;
	}
	// HOLD-TONE ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_hold_tone_on_cntdwn1 == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_on_cntdwn1 = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_hold_tone_off_cntdwn1 == -1 && m_l_hold_tone_on_cntdwn1 == -1) 
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_hold_tone1, "ON") == 0) 
		{
			show_ss_hold_tone_on();
		}
		// OFF �̏ꍇ
		else 
		{
			show_ss_hold_tone_off();
		}
	}


	/*** S/X MODULATION *******************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_MOD_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_mod_x, sizeof(m_sz_mod_x), obs_adr->sz_sts_name);
	}

	// MOD OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_mod_off_cntdwn2 >= 0 &&
		strcmp(m_sz_mod_x, "OFF") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_mod_off_cntdwn2 = -1;
	}
	// MOD OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_mod_off_cntdwn2 == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_mod_off_cntdwn2 = -1;
	}

	// MOD ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_mod_on_cntdwn2 >= 0 &&
		strcmp(m_sz_mod_x, "ON") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_mod_on_cntdwn2 = -1;
	}
	// MOD ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_mod_on_cntdwn2 == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_mod_on_cntdwn2 = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_mod_off_cntdwn2 == -1 && m_l_mod_on_cntdwn2== -1) 
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_mod_x, "ON") == 0) 
		{
			show_sx_modulation_on();
		}
		//  OFF �̏ꍇ
		else 
		{
			show_sx_modulation_off();
		}
	}


	/*** S/X HOLD-TONE ********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_HOLD_TONE2));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hold_tone2, sizeof(m_sz_hold_tone2), obs_adr->sz_sts_name);
	}

	// HOLD-TONE OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_hold_tone_off_cntdwn2 >= 0 &&
		strcmp(m_sz_hold_tone2, "OFF") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_off_cntdwn2 = -1;
	}
	// HOLD-TONE OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_hold_tone_off_cntdwn2 == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_off_cntdwn2 = -1;
	}

	// HOLD-TONE ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_hold_tone_on_cntdwn2 >= 0 &&
		strcmp(m_sz_hold_tone2, "ON") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_on_cntdwn2 = -1;
	}
	// HOLD-TONE ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ 
	else if (m_l_hold_tone_on_cntdwn2 == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_on_cntdwn2 = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_hold_tone_off_cntdwn2 == -1 && m_l_hold_tone_on_cntdwn2 == -1) 
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_hold_tone2, "ON") == 0) 
		{
			show_sx_hold_tone_on();
		}
		// OFF �̏ꍇ
		else 
		{
			show_sx_hold_tone_off();
		}
	}

	UpdateData(FALSE);

	SetTimer(TID_CONDITION, INTERVAL_CONDITION, 0);
}


/*============================================================================*/
/*! CTrsControlS20

-# �J�E���g�_�E���^�C���A�E�g

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::count_down()
{
	TRACE(_T("count_down\n"));
	KillTimer(TID_COUNTDOWN);

	// 0 �ȏ�̓J�E���g�_�E��

	/*-- TX CONTROL ----------------------------------------------------------------------*/

	// RF-POWER OFF
	if (m_l_rf_power_off_cntdwn > 0) 
	{
		printf("RF-POWER OFF �J�E���g�_�E��:%d\n", m_l_rf_power_off_cntdwn);
		m_l_rf_power_off_cntdwn--;
	}

	// RF-POWER ON
	if (m_l_rf_power_on_cntdwn > 0) 
	{
		printf("RF-POWER ON �J�E���g�_�E��:%d\n", m_l_rf_power_on_cntdwn);
		m_l_rf_power_on_cntdwn--;
	}

	// NO-USE OFF
	if (m_l_no_use_off_cntdwn > 0) 
	{
		printf("NO-USE OFF �J�E���g�_�E��:%d\n", m_l_no_use_off_cntdwn);
		m_l_no_use_off_cntdwn--;
	}

	// NO-USE ON
	if (m_l_no_use_on_cntdwn > 0) 
	{
		printf("NO-USE ON �J�E���g�_�E��:%d\n", m_l_no_use_on_cntdwn);
		m_l_no_use_on_cntdwn--;
	}

	// SWEEP-CONT STOP
	if (m_l_sweep_cont_stop_cntdwn > 0) 
	{
		printf("SWEEP-CONT STOP �J�E���g�_�E��:%d\n", m_l_sweep_cont_stop_cntdwn);
		m_l_sweep_cont_stop_cntdwn--;
	}

	// SWEEP-CONT INC
	if (m_l_sweep_cont_inc_cntdwn > 0) 
	{
		printf("SWEEP-CONT INC �J�E���g�_�E��:%d\n", m_l_sweep_cont_inc_cntdwn);
		m_l_sweep_cont_inc_cntdwn--;
	}

	// SWEEP-CONT INC+DEC
	if (m_l_sweep_cont_incdec_cntdwn > 0) 
	{
		printf("SWEEP-CONT INC+DEC �J�E���g�_�E��:%d\n", m_l_sweep_cont_incdec_cntdwn);
		m_l_sweep_cont_incdec_cntdwn--;
	}

	// SWEEP-CONT DEC
	if (m_l_sweep_cont_dec_cntdwn > 0) 
	{
		printf("SWEEP-CONT DEC �J�E���g�_�E��:%d\n", m_l_sweep_cont_dec_cntdwn);
		m_l_sweep_cont_dec_cntdwn--;
	}

	// SWEEP-CONT DEC+INC
	if (m_l_sweep_cont_decinc_cntdwn > 0) 
	{
		printf("SWEEP-CONT DEC+INC �J�E���g�_�E��:%d\n", m_l_sweep_cont_decinc_cntdwn);
		m_l_sweep_cont_decinc_cntdwn--;
	}


	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	// SS-RNG OFF
	if (m_l_ss_rng_off_cntdwn > 0) 
	{
		printf("SS-RNG OFF �J�E���g�_�E��:%d\n", m_l_ss_rng_off_cntdwn);
		m_l_ss_rng_off_cntdwn--;
	}

	// SS-RNG ON
	if (m_l_ss_rng_on_cntdwn > 0) 
	{
		printf("SS-RNG ON �J�E���g�_�E��:%d\n", m_l_ss_rng_on_cntdwn);
		m_l_ss_rng_on_cntdwn--;
	}

	// SX-RNG OFF
	if (m_l_sx_rng_off_cntdwn > 0) 
	{
		printf("SX-RNG OFF �J�E���g�_�E��:%d\n", m_l_sx_rng_off_cntdwn);
		m_l_sx_rng_off_cntdwn--;
	}

	// SX-RNG ON
	if (m_l_sx_rng_on_cntdwn > 0) 
	{
		printf("SX-RNG ON �J�E���g�_�E��:%d\n", m_l_sx_rng_on_cntdwn);
		m_l_sx_rng_on_cntdwn--;
	}

	// COH/INCOH COH
	if (m_l_coh_incoh_coh_cntdwn > 0) 
	{
		printf("COH/INCOH COH �J�E���g�_�E��:%d\n", m_l_coh_incoh_coh_cntdwn);
		m_l_coh_incoh_coh_cntdwn--;
	}

	// COH/INCOH INCOH
	if (m_l_coh_incoh_incoh_cntdwn > 0) 
	{
		printf("COH/INCOH INCOH �J�E���g�_�E��:%d\n", m_l_coh_incoh_incoh_cntdwn);
		m_l_coh_incoh_incoh_cntdwn--;
	}

	// RNGING STOP
	if (m_l_rnging_stop_cntdwn > 0) 
	{
		printf("RNGING STOP �J�E���g�_�E��:%d\n", m_l_rnging_stop_cntdwn);
		m_l_rnging_stop_cntdwn--;
	}

	// RNGING START
	if (m_l_rnging_start_cntdwn > 0) 
	{
		printf("RNGING START �J�E���g�_�E��:%d\n", m_l_rnging_start_cntdwn);
		m_l_rnging_start_cntdwn--;
	}

	// SS MOD OFF
	if (m_l_mod_off_cntdwn1 > 0) 
	{
		printf ("MOD OFF �J�E���g�_�E��:%d\n", m_l_mod_off_cntdwn1);
		m_l_mod_off_cntdwn1--;
	}

	// SS MOD ON
	if (m_l_mod_on_cntdwn1 > 0) 
	{
		printf ("MOD ON �J�E���g�_�E��:%d\n", m_l_mod_on_cntdwn1);
		m_l_mod_on_cntdwn1--;
	}

	// SX MOD OFF
	if (m_l_mod_off_cntdwn2 > 0) 
	{
		printf("MOD OFF �J�E���g�_�E��:%d\n", m_l_mod_off_cntdwn2);
		m_l_mod_off_cntdwn2--;
	}

	// SX MOD ON
	if (m_l_mod_on_cntdwn2 > 0) 
	{
		printf("MOD ON �J�E���g�_�E��:%d\n", m_l_mod_on_cntdwn2);
		m_l_mod_on_cntdwn2--;
	}

	// SS HOLD-TONE OFF
	if (m_l_hold_tone_off_cntdwn1 > 0) 
	{
		printf("HOLD-TONE1 OFF �J�E���g�_�E��:%d\n", m_l_hold_tone_off_cntdwn1);
		m_l_hold_tone_off_cntdwn1--;
	}

	// SS HOLD-TONE ON
	if (m_l_hold_tone_on_cntdwn1> 0) 
	{
		printf("HOLD-TONE1 ON �J�E���g�_�E��:%d\n", m_l_hold_tone_on_cntdwn1);
		m_l_hold_tone_on_cntdwn1--;
	}

	// SX HOLD-TONE OFF
	if (m_l_hold_tone_off_cntdwn2 > 0) 
	{
		printf("HOLD-TONE2 OFF �J�E���g�_�E��:%d\n", m_l_hold_tone_off_cntdwn2);
		m_l_hold_tone_off_cntdwn2--;
	}

	// SX HOLD-TONE ON
	if (m_l_hold_tone_on_cntdwn2> 0) 
	{
		printf("HOLD-TONE2 ON �J�E���g�_�E��:%d\n", m_l_hold_tone_on_cntdwn2);
		m_l_hold_tone_on_cntdwn2--;
	}

	SetTimer(TID_COUNTDOWN, INTERVAL_COUNTDOWN, 0);
}


/*============================================================================*/
/*! CTrsControlS20

-# RF-POWER OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_rf_power_off()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_RF_ON_ENBL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl, sizeof(m_sz_rf_on_enbl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "�ڑ�");
	strcpy_s(m_sz_rf_on_enbl, sizeof(m_sz_rf_on_enbl), "ON");
	strcpy_s(m_sz_x_tx_det_ans, sizeof(m_sz_x_tx_det_ans), "OFF");
	strcpy_s(m_sz_x_tx_hpa_det_ans, sizeof(m_sz_x_tx_hpa_det_ans), "OFF");
#endif

	if (strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) 
	{
		// �����[�g�i�I���j
		m_RfPwrOffBtn.SetColor(COLOR_REMOTES);
	}
	else 
	{
		// ���[�J��
		m_RfPwrOffBtn.SetColor(COLOR_LOCAL);
	}

	// ON ��
	if ((strcmp(m_sz_s_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_rf_on_enbl, "ON") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_RfPwrOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# RF-POWER ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_rf_power_on()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_RF_ON_ENBL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl, sizeof(m_sz_rf_on_enbl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_RF_ON_DET_ANS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans, sizeof(m_sz_rf_on_det_ans), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "�ڑ�");
	strcpy_s(m_sz_rf_on_enbl, sizeof(m_sz_rf_on_enbl), "OFF");
	strcpy_s(m_sz_rf_on_det_ans, sizeof(m_sz_rf_on_det_ans), "ON");
#endif

	if (strcmp(m_sz_lan_s_tx, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_RfPwrOnBtn.SetColor(COLOR_REMOTE);
	}
	else if (((strcmp(m_sz_s_tx_rl, "LOCAL") == 0) || (strcmp(m_sz_rf_on_enbl, "OFF") == 0)) &&
		(strcmp(m_sz_rf_on_det_ans, "OFF") == 0)) 
	{
		// �����[�g�i�I���j/ ���[�J��
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES, COLOR_LOCAL, TRUE);
	}
	else if (strcmp(m_sz_rf_on_det_ans, "OFF") == 0) 
	{
		// �����[�g�i�I���j/ �����[�g�i��I���j
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES, COLOR_REMOTE, TRUE);
	}
	else 
	{
		// �����[�g�i�I���j
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_s_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_rf_on_enbl, "OFF") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_RfPwrOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_RfPwrOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# NO-USE OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_no_use_off()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_NO_USE_ENBL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl, sizeof(m_sz_no_use_enbl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_s_tx, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_NoUseOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_NoUseOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_s_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_NoUseOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_NoUseOffBtn.Invalidate();
	m_NoUseOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# NO-USE ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_no_use_on()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_NO_USE_ENBL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl, sizeof(m_sz_no_use_enbl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_s_tx, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_NoUseOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_s_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_NoUseOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_NoUseOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_NoUseOffBtn.Invalidate();
	m_NoUseOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# SWEEP-CONT STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_sweep_cont_stop()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_ss_tcrmod, sizeof(m_sz_lan_ss_tcrmod), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_ss_tcrmod, sizeof(m_sz_lan_ss_tcrmod), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_ss_tcrmod, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepContStopBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SweepContStopBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_ss_tcrmod, "�ڑ�") == 0) && (m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_SweepContIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecIncBtn.SetColor(COLOR_REMOTE);

	}
	else 
	{
		// ���[�J��
		m_SweepContIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# SWEEP-CONT INC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_sweep_cont_inc()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_s_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepContIncBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SweepContIncBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepContStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecIncBtn.SetColor(COLOR_REMOTE);

	}
	else
	{
		// ���[�J��
		m_SweepContStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# SWEEP-CONT INCDEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_sweep_cont_incdec()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_s_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepContIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SweepContIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepContStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecIncBtn.SetColor(COLOR_REMOTE);

	}
	else
	{
		// ���[�J��
		m_SweepContStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# SWEEP-CONT DEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_sweep_cont_dec()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_s_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepContDecBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SweepContDecBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepContStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecIncBtn.SetColor(COLOR_REMOTE);

	}
	else
	{
		// ���[�J��
		m_SweepContStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# SWEEP-CONT DECINC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_sweep_cont_decinc()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_s_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepContDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SweepContDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepContStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepContStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContDecBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}



/*============================================================================*/
/*! CTrsControlS20

-# SS-RNG OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_ss_rng_off()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_SS_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_ss_rarr, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SSRngOffBtn.SetColor(COLOR_LOCAL);
		m_SSRngOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		if (m_i_priv_flag == 1) 
		{
			// �����[�g�i��I���j
			m_SSRngOnBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ���[�J��
			m_SSRngOnBtn.SetColor(COLOR_LOCAL);
		}
		// �����[�g�i�I���j
		m_SSRngOffBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_SSRngOffBtn.Invalidate();
	m_SSRngOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# SS-RNG ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_ss_rng_on()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_SS_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_ss_rarr, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SSRngOnBtn.SetColor(COLOR_LOCAL);
		m_SSRngOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		if (m_i_priv_flag == 1) 
		{
			// �����[�g�i��I���j
			m_SSRngOffBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ���[�J��
			m_SSRngOffBtn.SetColor(COLOR_LOCAL);
		}
		// �����[�g�i�I���j
		m_SSRngOnBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_SSRngOffBtn.Invalidate();
	m_SSRngOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# SX-RNG OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_sx_rng_off()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_SS_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_SX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_sx_rarr, sizeof(m_sz_lan_sx_rarr), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), "�ڑ�");
	strcpy_s(m_sz_lan_sx_rarr, sizeof(m_sz_lan_sx_rarr), "�ڑ�");
#endif

	if ((strcmp(m_sz_lan_ss_rarr, "�ڑ�") != 0) || (strcmp(m_sz_lan_sx_rarr, "�ڑ�") != 0)) 
	{
		// ���[�J��
		m_SXRngOffBtn.SetColor(COLOR_LOCAL);
		m_SXRngOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		if (m_i_priv_flag == 1) 
		{
			// �����[�g�i��I���j
			m_SXRngOnBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ���[�J��
			m_SXRngOnBtn.SetColor(COLOR_LOCAL);
		}
		// �����[�g�i�I���j
		m_SXRngOffBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_SXRngOffBtn.Invalidate();
	m_SXRngOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# SX-RNG ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_sx_rng_on()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_SS_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_SX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_sx_rarr, sizeof(m_sz_lan_sx_rarr), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), "�ڑ�");
	strcpy_s(m_sz_lan_sx_rarr, sizeof(m_sz_lan_sx_rarr), "�ڑ�");
#endif

	if ((strcmp(m_sz_lan_ss_rarr, "�ڑ�") != 0) || (strcmp(m_sz_lan_sx_rarr, "�ڑ�") != 0)) 
	{
		// ���[�J��
		m_SXRngOnBtn.SetColor(COLOR_LOCAL);
		m_SXRngOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		if (m_i_priv_flag == 1) 
		{
			// �����[�g�i��I���j
			m_SXRngOffBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ���[�J��
			m_SXRngOffBtn.SetColor(COLOR_LOCAL);
		}
		// �����[�g�i�I���j
		m_SXRngOnBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_SXRngOffBtn.Invalidate();
	m_SXRngOnBtn.Invalidate();
}



/*============================================================================*/
/*! CTrsControlS20

-# COH/INCOH COH ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_coh_incoh_coh()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_SS_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_ss_rarr, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_CohIncohCohBtn.SetColor(COLOR_LOCAL);
		m_CohIncohInCohBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		if (m_i_priv_flag == 1) 
		{
			// �����[�g�i��I���j
			m_CohIncohInCohBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ���[�J��
			m_CohIncohInCohBtn.SetColor(COLOR_LOCAL);
		}
		// �����[�g�i�I���j
		m_CohIncohCohBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_CohIncohCohBtn.Invalidate();
	m_CohIncohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# COH/INCOH INCOH ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_coh_incoh_incoh()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_SS_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_ss_rarr, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_CohIncohInCohBtn.SetColor(COLOR_LOCAL);
		m_CohIncohCohBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		if (m_i_priv_flag == 1) 
		{
			// �����[�g�i��I���j
			m_CohIncohCohBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ���[�J��
			m_CohIncohCohBtn.SetColor(COLOR_LOCAL);
		}
		// �����[�g�i�I���j
		m_CohIncohInCohBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_CohIncohCohBtn.Invalidate();
	m_CohIncohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# RANGE STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_rnging_stop()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_SS_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_SX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_sx_rarr, sizeof(m_sz_lan_sx_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_P_FLG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_pass_flag, sizeof(m_sz_pass_flag), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), "�ڑ�");
	strcpy_s(m_sz_lan_sx_rarr, sizeof(m_sz_lan_sx_rarr), "�ڑ�");
	strcpy_s(m_sz_pass_flag, sizeof(m_sz_pass_flag), "�p�X��");
#endif

	if ((strcmp(m_sz_lan_ss_rarr, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_sx_rarr, "�ڑ�") != 0))
	{
		// ���[�J��
		m_RangeStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_RangeStopBtn.SetColor(COLOR_REMOTES);
	}

	// STOP ��
	// SS-RNG �� SX-RNG �� OFF �{�^�����ǂ�����Γ_���̏ꍇ�� ���[�J���ɂ���
	COLORREF colSSRngOn = m_SSRngOnBtn.GetColor();
	COLORREF colSXRngOn = m_SXRngOnBtn.GetColor();
	if ((((colSSRngOn == COLOR_REMOTES) && (strcmp(m_sz_lan_ss_rarr, "�ڑ�") == 0)) ||
		 ((colSXRngOn == COLOR_REMOTES) && (strcmp(m_sz_lan_sx_rarr, "�ڑ�") == 0))) &&
		 (strcmp(m_sz_pass_flag, "�p�X��") == 0) &&
		 (m_i_priv_flag == 1)	) 
	{
		// �����[�g�i��I���j
		m_RangeStartBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_RangeStartBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RangeStopBtn.Invalidate();
	m_RangeStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# RANGE START ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_rnging_start()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_SS_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_SX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_sx_rarr, sizeof(m_sz_lan_sx_rarr), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), "�ڑ�");
	strcpy_s(m_sz_lan_sx_rarr, sizeof(m_sz_lan_sx_rarr), "�ڑ�");
#endif

	// SS-RNG �� SX-RNG �� OFF �{�^�����ǂ�����Γ_���̏ꍇ�� ���ɂ��� 
	COLORREF colSSRngOn = m_SSRngOnBtn.GetColor();
	COLORREF colSXRngOn = m_SXRngOnBtn.GetColor();
	if (((colSSRngOn == COLOR_REMOTES) && (strcmp(m_sz_lan_ss_rarr, "�ڑ�") == 0)) ||
		((colSXRngOn == COLOR_REMOTES) && (strcmp(m_sz_lan_sx_rarr, "�ڑ�") == 0)))
	{
		// �����[�g�i�I���j
		m_RangeStartBtn.SetColor(COLOR_REMOTES);
	}
	else 
	{
		// ���[�J��
		m_RangeStartBtn.SetColor(COLOR_LOCAL);
	}

	// STOP��
	if ((((colSSRngOn == COLOR_REMOTES) && (strcmp(m_sz_lan_ss_rarr, "�ڑ�") == 0)) ||
	 	 ((colSXRngOn == COLOR_REMOTES) && (strcmp(m_sz_lan_sx_rarr, "�ڑ�") == 0))) &&
		 (strcmp(m_sz_pass_flag, "�p�X��") == 0) &&
		 (m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_RangeStopBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_RangeStopBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RangeStopBtn.Invalidate();
	m_RangeStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# S/S MODULATION OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_ss_modulation_off()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_UPLINK_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}
#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "�ڑ�");
	strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), "REMOTE");
#endif

	if (strcmp(m_sz_lan_s_tx, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SSModulationOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SSModulationOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) && (m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_SSModulationOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_SSModulationOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SSModulationOffBtn.Invalidate();
	m_SSModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# S/S MODULATION ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_ss_modulation_on()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_UPLINK_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}
#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "�ڑ�");
	strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), "REMOTE");
#endif

	if (strcmp(m_sz_lan_s_tx, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SSModulationOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SSModulationOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SSModulationOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_SSModulationOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SSModulationOffBtn.Invalidate();
	m_SSModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# S/S HOLD-TONE OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_ss_hold_tone_off()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_CMDOUT1));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_cmdout1, sizeof(m_sz_lan_cmdout1), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_cmdout1, sizeof(m_sz_lan_cmdout1), "�ڑ�");

	if (strcmp(m_sz_lan_cmdout1, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SSHoldToneOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SSHoldToneOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_lan_cmdout1, "�ڑ�") == 0) && (m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_SSHoldToneOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_SSHoldToneOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SSHoldToneOffBtn.Invalidate();
	m_SSHoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# S/S HOLD-TONE ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_ss_hold_tone_on()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_CMDOUT1));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_cmdout1, sizeof(m_sz_lan_cmdout1), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_cmdout1, sizeof(m_sz_lan_cmdout1), "�ڑ�");

	if (strcmp(m_sz_lan_cmdout1, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SSHoldToneOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SSHoldToneOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_lan_cmdout1, "�ڑ�") == 0) && (m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_SSHoldToneOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_SSHoldToneOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SSHoldToneOffBtn.Invalidate();
	m_SSHoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# S/X MODULATION OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_sx_modulation_off()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_UPLINK_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "�ڑ�");
	strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), "REMOTE");
#endif

	if (strcmp(m_sz_lan_s_tx, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SXModulationOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SXModulationOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SXModulationOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SXModulationOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SXModulationOffBtn.Invalidate();
	m_SXModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# S/X MODULATION ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_sx_modulation_on()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_UPLINK_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "�ڑ�");
	strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), "REMOTE");
#endif

	if (strcmp(m_sz_lan_s_tx, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SXModulationOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SXModulationOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) && (m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_SXModulationOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_SXModulationOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SXModulationOffBtn.Invalidate();
	m_SXModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# S/X HOLD-TONE OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_sx_hold_tone_off()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_CMDOUT2));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_cmdout2, sizeof(m_sz_lan_cmdout2), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_cmdout2, sizeof(m_sz_lan_cmdout2), "�ڑ�");

	if (strcmp(m_sz_lan_cmdout2, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SXHoldToneOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SXHoldToneOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_lan_cmdout2, "�ڑ�") == 0) && (m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_SXHoldToneOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_SXHoldToneOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SXHoldToneOffBtn.Invalidate();
	m_SXHoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# S/X HOLD-TONE ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::show_sx_hold_tone_on()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S20_LAN_CMDOUT2));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_cmdout2, sizeof(m_sz_lan_cmdout2), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_cmdout2, sizeof(m_sz_lan_cmdout2), "�ڑ�");

	if (strcmp(m_sz_lan_cmdout2, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SXHoldToneOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SXHoldToneOnBtn.SetColor(COLOR_REMOTES);
	}

	if ((strcmp(m_sz_lan_cmdout2, "�ڑ�") == 0) && (m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_SXHoldToneOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_SXHoldToneOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SXHoldToneOffBtn.Invalidate();
	m_SXHoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS20

-# HOLD TONE ON ��ԉ�ʕ\����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS20::printlg_endtime(CString str)
{
	// �Ď��f�[�^�擾

	// ���ݎ����擾

	// �X�C�[�v�����\�莞���Z�o

	// opelog�o��
}

void CTrsControlS20::OnClose()
{
	CString title;
	GetWindowText(title);

	if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("�I�����܂��B\n��낵���ł����H"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		return;
	}

	CloseDlg();
	//	OnOK();
	ShowWindow(SW_HIDE);
}

void CTrsControlS20::CloseDlg()
{
	KillTimer(TID_CONDITION);
	KillTimer(TID_COUNTDOWN);

	// �E�B���h�E����郁�b�Z�[�W
	theApp.m_pMainWnd->PostMessageW(eMessage_WindowClose, 0, (LPARAM)this);
}
