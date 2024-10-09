/*============================================================================*/
/*! TrsControl.cpp

-# X�ё��M������
*/
/*============================================================================*/
// TrsControlX.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrsControlX.h"
#include "afxdialogex.h"
#include "TrsControlMng.h"
#include "HookMsgBox.h"


// CTrsControlX �_�C�A���O

IMPLEMENT_DYNAMIC(CTrsControlX, CDialogBase)

/*============================================================================*/
/*! CTrsControlX

-# �R���X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
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

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
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


// CTrsControlX ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! CTrsControlX

-# �t�@�C��-���郁�j���[����

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::OnMenuClose()
{
	// �����ꂩ�̃{�^�������s���̏ꍇ
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
/*! CTrsControlX

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CTrsControlX::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// RF-POWER
	m_RFPWROffBtn.SetColor(COLOR_REMOTES);
	m_RFPWROffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_RFPWROnBtn.SetColor(COLOR_REMOTES);
	m_RFPWROnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// NO-USE
	m_NoUseOffBtn.SetColor(COLOR_REMOTES);
	m_NoUseOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_NoUseOnBtn.SetColor(COLOR_REMOTE);
	m_NoUseOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// SWEEP-CONT
	m_SweepCountStopBtn.SetColor(COLOR_REMOTES);
	m_SweepCountStopBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepCountIncBtn.SetColor(COLOR_REMOTE);
	m_SweepCountIncBtn.SetCtrlButton(TRUE);			// Ctrl�L�[ +
	m_SweepCountIncDecBtn.SetColor(COLOR_REMOTE);
	m_SweepCountIncDecBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepCountDecBtn.SetColor(COLOR_REMOTE);
	m_SweepCountDecBtn.SetCtrlButton(TRUE);			// Ctrl�L�[ +
	m_SweepCountDecIncBtn.SetColor(COLOR_REMOTE);
	m_SweepCountDecIncBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// RAMP-CONT
	m_RampContOffBtn.SetColor(COLOR_REMOTES);
	m_RampContOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_RampContOnBtn.SetColor(COLOR_REMOTE);
	m_RampContOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// CMD-MOD
	m_CmdModOffBtn.SetColor(COLOR_REMOTES);
	m_CmdModOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_CmdModOnBtn.SetColor(COLOR_REMOTE);
	m_CmdModOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// RNG-MOD
	m_RngModOffBtn.SetColor(COLOR_REMOTES);
	m_RngModOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_RngModOnBtn.SetColor(COLOR_REMOTE);
	m_RngModOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// MODULATION
	m_ModulationOffBtn.SetColor(COLOR_REMOTES);
	m_ModulationOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_ModulationOnBtn.SetColor(COLOR_REMOTES);
	m_ModulationOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// HOLD-TONE
	m_HoldToneOffBtn.SetColor(COLOR_REMOTES);
	m_HoldToneOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_HoldToneOnBtn.SetColor(COLOR_REMOTE);
	m_HoldToneOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// X/X-COH
	m_XXCohCohBtn.SetColor(COLOR_REMOTES);
	m_XXCohCohBtn.SetCtrlButton(TRUE);			// Ctrl�L�[ +
	m_XXCohIncohBtn.SetColor(COLOR_REMOTE);
	m_XXCohIncohBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// RNGING
	m_RngingStopBtn.SetColor(COLOR_REMOTES);
	m_RngingStopBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_RngingStartBtn.SetColor(COLOR_REMOTES);
	m_RngingStartBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// �Đ��^
	m_RRngStopBtn.SetColor(COLOR_REMOTES);
	m_RRngStopBtn.SetShiftButton(TRUE);			// Shift�L�[ +
	m_RRngStartBtn.SetColor(COLOR_REMOTE);
	m_RRngStartBtn.SetShiftButton(TRUE);		// Shift�L�[ +

	// X/X-RNG CNT
	m_XXRngNumBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_XXRngNumBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +


	// �O���[�v�{�b�N�X
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
	// �z�X�g���擾
	memset(m_sz_hostname, (char)NULL, sizeof(m_sz_hostname));
	gethostname(m_sz_hostname, sizeof(m_sz_hostname));

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
/*! CTrsControlX

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CTrsControlX::UpdateDlg()
{
	// ���M�����ʏ�ԕ\���^�C���A�E�g�R�[���o�b�N���[�`���N��
	show_opsc_condition();

	// �J�E���g�_�E���^�C���A�E�g�R�[���o�b�N���[�`���N��
	count_down();

	return TRUE;
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wRF-POWER�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRfOffBtn()
{
	int station = theApp.GetSelectStation();

	// �{�^���𐧌䒆�ɂ���
	m_RFPWROffBtn.SetColor(COLOR_CTRLIN);
	m_RFPWROnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	if (station == 0)
	{
		// �P�c64m
		theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_ON/OFF_CONT"), _T("OFF"));
	}
	else if (station == 2)
	{
		// ���V�Y34m
		theApp.GetSatelliteData().opctproc(_T("X-TX.RF_CONT"), _T("OFF"));
	}

	// �J�E���g�_�E���J�n
	m_l_rf_power_off_cntdwn = RF_POWER_TIME_X_LIMIT;

	// �X�V
	m_RFPWROffBtn.Invalidate();
	m_RFPWROnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wRF-POWER�x-�wON�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRfOnBtn()
{
	int station = theApp.GetSelectStation();

	// �{�^���𐧌䒆�ɂ���
	m_RFPWROnBtn.SetColor(COLOR_CTRLIN);
	m_RFPWROffBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	if (station == 0)
	{
		// �P�c64m
		theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_ON/OFF_CONT"), _T("ON"));
	}
	else if (station == 2)
	{
		// ���V�Y34m
		theApp.GetSatelliteData().opctproc(_T("X-TX.RF_CONT"), _T("ON"));
	}

	// �J�E���g�_�E���J�n
	m_l_rf_power_on_cntdwn = RF_POWER_TIME_X_LIMIT;

	// �X�V
	m_RFPWROffBtn.Invalidate();
	m_RFPWROnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wNO-USE�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlNouseOffBtn()
{
	COLORREF col = m_NoUseOffBtn.GetColor();

	int station = theApp.GetSelectStation();
	if (station == 0)
	{
		// �P�c64m
		if (col == COLOR_REMOTE)
		{
			// �����[�g�i��I���j

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE_RESET"));

			// �J�E���g�_�E���J�n
			m_l_no_use_off_cntdwn = NO_USE_TIME_X_LIMIT;
		}
		else if (col == COLOR_CTRLIN)
		{
			// ���䒆

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE_RESET"));

			// �J�E���g�_�E���J�n
			m_l_no_use_off_cntdwn = NO_USE_TIME_X_LIMIT;

			// �����̃J�E���g�_�E���I��
			m_l_no_use_on_cntdwn = -1;
		}
		else
		{
			;	// nothing!
		}
	}
	else if (station == 2)
	{
		// ���V�Y34m
		if (col == COLOR_REMOTE)
		{
			// �����[�g�i��I���j

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-TX.RF_TRIP_NO_USE"), _T("NO_USE_RESET"));

			// �J�E���g�_�E���J�n
			m_l_no_use_off_cntdwn = NO_USE_TIME_X_LIMIT;
		}
		else if (col == COLOR_CTRLIN)
		{
			// ���䒆

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-TX.RF_TRIP_NO_USE"), _T("NO_USE_RESET"));

			// �J�E���g�_�E���J�n
			m_l_no_use_off_cntdwn = NO_USE_TIME_X_LIMIT;

			// �����̃J�E���g�_�E���I��
			m_l_no_use_on_cntdwn = -1;
		}
		else if (col == COLOR_REMOTES)
		{
			// �����[�g�i�I���j

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-TX.RF_TRIP_NO_USE"), _T("NO_USE_RESET"));

			// �J�E���g�_�E���J�n
			m_l_no_use_off_cntdwn = NO_USE_TIME_X_LIMIT;

			// �����̃J�E���g�_�E���I��
			m_l_no_use_on_cntdwn = -1;
		}
		else
		{
			;	// nothing!
		}
	}

	// �X�V
	m_NoUseOffBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wNO-USE�x-�wON�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlNouseOnBtn()
{
	COLORREF col = m_NoUseOnBtn.GetColor();
	int station = theApp.GetSelectStation();
	if (station == 0)
	{
		// �P�c64m
		if (col == COLOR_REMOTE)
		{
			// �����[�g�i��I���j

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE"));

			// �J�E���g�_�E���J�n
			m_l_no_use_on_cntdwn = NO_USE_TIME_X_LIMIT;
		}
		else if (col == COLOR_CTRLIN)
		{
			// ���䒆

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE"));

			// �J�E���g�_�E���J�n
			m_l_no_use_on_cntdwn = NO_USE_TIME_X_LIMIT;

			// �����̃J�E���g�_�E���I��
			m_l_no_use_off_cntdwn = -1;
		}
		else
		{
			;	// nothing!
		}
	}
	else if (station == 2)
	{
		// ���V�Y34m
		if (col == COLOR_REMOTE)
		{
			// �����[�g�i��I���j

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-TX.RF_TRIP_NO_USE"), _T("NO_USE"));

			// �J�E���g�_�E���J�n
			m_l_no_use_on_cntdwn = NO_USE_TIME_X_LIMIT;
		}
		else if (col == COLOR_CTRLIN)
		{
			// ���䒆

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-TX.RF_TRIP_NO_USE"), _T("NO_USE"));

			// �J�E���g�_�E���J�n
			m_l_no_use_on_cntdwn = NO_USE_TIME_X_LIMIT;

			// �����̃J�E���g�_�E���I��
			m_l_no_use_off_cntdwn = -1;
		}
		else if (col == COLOR_REMOTES)
		{
			// �����[�g�i�I���j

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-TX.RF_TRIP_NO_USE"), _T("NO_USE"));

			// �J�E���g�_�E���J�n
			m_l_no_use_on_cntdwn = NO_USE_TIME_X_LIMIT;

			// �����̃J�E���g�_�E���I��
			m_l_no_use_off_cntdwn = -1;
		}
		else
		{
			;	// nothing!
		}
	}

	// �X�V
	m_NoUseOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wSTOP�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlSweetStopBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SweepCountStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("X-TXSYN.SWEEP_CONT"), _T("STOP"));

	// �J�E���g�_�E���J�n
	m_l_sweep_cont_stop_cntdwn = SWEEP_CONT_TIME_X_LIMIT;

	// �X�V
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wINC�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlSweetIncBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SweepCountStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("X-TXSYN.SWEEP_CONT"), _T("INC"));

	// �J�E���g�_�E���J�n
	m_l_sweep_cont_inc_cntdwn = SWEEP_CONT_TIME_X_LIMIT;

	// �X�V
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wINCDEC�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlSweetIncdecBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SweepCountStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("X-TXSYN.SWEEP_CONT"), _T("INC+DEC"));

	// �J�E���g�_�E���J�n
	m_l_sweep_cont_incdec_cntdwn = SWEEP_CONT_TIME_X_LIMIT;

	// �X�V
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wDEC�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlSweetDecBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SweepCountStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("X-TXSYN.SWEEP_CONT"), _T("DEC"));

	// �J�E���g�_�E���J�n
	m_l_sweep_cont_dec_cntdwn = SWEEP_CONT_TIME_X_LIMIT;

	// �X�V
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wDECINC�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlSweetDecincBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_SweepCountStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("X-TXSYN.SWEEP_CONT"), _T("DEC+INC"));

	// �J�E���g�_�E���J�n
	m_l_sweep_cont_decinc_cntdwn = SWEEP_CONT_TIME_X_LIMIT;

	// �X�V
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wRAMP-CONT�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRampcontOffBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_RampContOffBtn.SetColor(COLOR_CTRLIN);
	m_RampContOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("X-TXSYN.RATE_AID"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_ramp_cont_off_cntdwn = RAMP_CONT_TIME_X_LIMIT;

	// �X�V
	m_RampContOffBtn.Invalidate();
	m_RampContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wRAMP-CONT�x-�wON�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRampcontOnBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_RampContOffBtn.SetColor(COLOR_CTRLIN);
	m_RampContOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("X-TXSYN.RATE_AID"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_ramp_cont_on_cntdwn = RAMP_CONT_TIME_X_LIMIT;

	// �X�V
	m_RampContOffBtn.Invalidate();
	m_RampContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wMOD�x-�wCMD-MOD�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlCmdOffBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_CmdModOffBtn.SetColor(COLOR_CTRLIN);
	m_CmdModOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CTRL.X_CMD_MOD"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_cmd_mod_off_cntdwn = CMD_MOD_TIME_X_LIMIT;

	// �X�V
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wMOD�x-�wCMD-MOD�x-�wON�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlCmdOnBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_CmdModOffBtn.SetColor(COLOR_CTRLIN);
	m_CmdModOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CTRL.X_CMD_MOD"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_cmd_mod_on_cntdwn = CMD_MOD_TIME_X_LIMIT;

	// �X�V
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wMOD�x-�wRNG-MOD�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRngOffBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_RngModOffBtn.SetColor(COLOR_CTRLIN);
	m_RngModOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CTRL.X_RNG_MOD"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_rng_mod_off_cntdwn = RNG_MOD_TIME_X_LIMIT;

	// �X�V
	m_RngModOffBtn.Invalidate();
	m_RngModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wMOD�x-�wRNG-MOD�x-�wON�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRngOnBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_RngModOffBtn.SetColor(COLOR_CTRLIN);
	m_RngModOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CTRL.X_RNG_MOD"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_rng_mod_on_cntdwn = RNG_MOD_TIME_X_LIMIT;

	// �X�V
	m_RngModOffBtn.Invalidate();
	m_RngModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wTCRMOD�x-�wMODULATION�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlModulationOffBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_ModulationOffBtn.SetColor(COLOR_CTRLIN);
	m_ModulationOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("XX-TCRMOD.MODULATION"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_modulation_off_cntdwn = MODULATION_TIME_X_LIMIT;

	// �X�V
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wTCRMOD�x-�wMODULATION�x-�wON�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlModulationOnBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_ModulationOffBtn.SetColor(COLOR_CTRLIN);
	m_ModulationOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("XX-TCRMOD.MODULATION"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_modulation_on_cntdwn = MODULATION_TIME_X_LIMIT;

	// �X�V
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wTCRMOD�x-�wHOLD-TONE�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlHoldtoneOffBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	int station = theApp.GetSelectStation();
	if (station == 0)
	{
		// �P�c64m
		theApp.GetSatelliteData().opctproc(_T("CMDOUT3.HOLD_TONE"), _T("OFF"));
	}
	else if (station == 2)
	{
		// ���V�Y34m
		theApp.GetSatelliteData().opctproc(_T("X-CMDOUT.HOLD_TONE"), _T("OFF"));
	}

	// �J�E���g�_�E���J�n
	m_l_hold_tone_off_cntdwn = HOLD_TONE_TIME_X_LIMIT;

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wTX CONTROL�x-�wTCRMOD�x-�wHOLD-TONE�x-�wON�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlHoldtoneOnBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	int station = theApp.GetSelectStation();
	if (station == 0)
	{
		// �P�c64m
		theApp.GetSatelliteData().opctproc(_T("CMDOUT3.HOLD_TONE"), _T("ON"));
	}
	else if (station == 2)
	{
		// ���V�Y34m
		theApp.GetSatelliteData().opctproc(_T("X-CMDOUT.HOLD_TONE"), _T("ON"));
	}

	// �J�E���g�_�E���J�n
	m_l_hold_tone_on_cntdwn = HOLD_TONE_TIME_X_LIMIT;

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wRNG CONTROL�x-�wX/X-RNG�x-�wNum�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlXxrngCBtn()
{
	// ����ʒm���M
	UpdateData();
	theApp.GetSatelliteData().opctproc(_T("V-EQU.RNG_NUM"), _T(""), m_strXXRng);
}


/*============================================================================*/
/*! CTrsControlX

-# �wRNG CONTROL�x-�wX/X-COH�x-�wCOH�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlXxcohCohBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_XXCohCohBtn.SetColor(COLOR_CTRLIN);
	m_XXCohIncohBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("COHI"));

	// �J�E���g�_�E���J�n
	m_l_coh_incoh_coh_cntdwn = COH_INCOH_TIME_X_LIMIT;

	// �X�V
	m_XXCohCohBtn.Invalidate();
	m_XXCohIncohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wRNG CONTROL�x-�wX/X-COH�x-�wINCOH�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlXxcohIncohBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_XXCohCohBtn.SetColor(COLOR_CTRLIN);
	m_XXCohIncohBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("INCOHI"));

	// �J�E���g�_�E���J�n
	m_l_coh_incoh_incoh_cntdwn = COH_INCOH_TIME_X_LIMIT;

	// �X�V
	m_XXCohCohBtn.Invalidate();
	m_XXCohIncohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wRNG CONTROL�x-�wRNGING�x-�wSTOP�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRngingStopBtn()
{
	// �{�^���𐧌䒆�ɂ��� STOP �{�^���̂�
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING_CANCEL"), _T(","));

	// �J�E���g�_�E���J�n
	m_l_rnging_stop_cntdwn = RNGING_TIME_X_LIMIT;

	// RNG��ʃt���O���N���A����
	m_l_rng_type = RNG_TYPE_NON;

	// �X�V
	m_RngingStopBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wRNG CONTROL�x-�wRNGING�x-�wSTART�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRngingStartBtn()
{
	CString str1, str2;

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("XX-RRNG.DIST_RNG_VIDEO"), _T("NORMAL"));

	// �Đ��^�{�^�����C���Z���V�e�B�u�ɂ���
	if (m_RRngStartBtn.IsWindowEnabled() == TRUE)
	{
		m_RRngStopBtn.EnableWindow(FALSE);
		m_RRngStartBtn.EnableWindow(FALSE);
	}

	// �{�^���𐧌䒆�ɂ���
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);
	m_RngingStartBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M X/X-RNG�̂ݍs��
	m_XXRngNumBtn.GetWindowTextW(str2);
	str1.Format(_T("XX%02d"), _wtoi(str2));
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str1);

	// �J�E���g�_�E���J�n
	m_l_rnging_start_cntdwn = RNGING_TIME_X_LIMIT;

	// RNG��ʃt���O���]���^�ɐݒ�
	m_l_rng_type = RNG_TYPE_NOR;

	// �X�V
	m_RngingStartBtn.Invalidate();
	m_RngingStopBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wRNG CONTROL�x-�w�Đ��^�x-�wSTOP�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRgntypeStopBtn()
{
	// �{�^���𐧌䒆�ɂ��� STOP �{�^���̂�
	m_RRngStopBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING_CANCEL"), _T(","));

	// �J�E���g�_�E���J�n
	m_l_rep_rnging_stop_cntdwn = REP_RNGING_TIME_X_LIMIT;

	// RNG��ʃt���O�ݒ�
	m_l_rng_type = RNG_TYPE_NON;

	// �X�V
	m_RRngStopBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# �wRNG CONTROL�x-�w�Đ��^�x-�wSTART�x�{�^����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CTrsControlX::OnBnClickedTrxctrlRgntypeStartBtn()
{
	CString str1, str2;

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("XX-RRNG.DIST_RNG_VIDEO"), _T("REPLAY"));

	// �]���^�{�^�����C���Z���V�e�B�u�ɂ���
	if (m_RngingStartBtn.IsWindowEnabled() == TRUE)
	{
		m_RngingStopBtn.EnableWindow(FALSE);
		m_RngingStartBtn.EnableWindow(FALSE);
	}

	// �{�^���𐧌䒆�ɂ���
	m_RRngStopBtn.SetColor(COLOR_CTRLIN);
	m_RRngStartBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M X/X-RNG�̂ݍs��
	m_XXRngNumBtn.GetWindowTextW(str2);
	str1.Format(_T("RX%02d"), _wtoi(str2));
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str1);

	// �J�E���g�_�E���J�n
	m_l_rep_rnging_start_cntdwn = REP_RNGING_TIME_X_LIMIT;

	// RNG��ʃt���O���Đ��^�ɐݒ�
	m_l_rng_type = RNG_TYPE_REP;

	// �X�V
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# OnTimer

@param  nIDEvent:�^�C�}�[�̎��ʎq
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::OnTimer(UINT_PTR nIDEvent)
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
/*! CTrsControlX

-# ���M�����ʏ�ԕ\���ݒ�

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_opsc_condition()
{
	TRACE(_T("show_opsc_condition\n"));
	KillTimer(TID_CONDITION);

	int station = theApp.GetSelectStation();
	if (station == 0)
	{
		// �P�c64m
		show_opsc_condition64();
	}
	else if (station == 2)
	{
		// ���V�Y34m
		show_opsc_condition34();
	}

	SetTimer(TID_CONDITION, INTERVAL_CONDITION, 0);
}

/*============================================================================*/
/*! CTrsControlX

-# ���M�����ʏ�ԕ\���ݒ�

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_opsc_condition64()
{
	TRACE(_T("show_opsc_condition\n"));

	char sz_buff[64];

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// ���䌠�͏�ɗL��
	m_i_priv_flag = 1;

	/*-- TX CONTROL ----------------------------------------------------------------------*/

	// RF POWER ON ENABLE ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_RF_ON_ENBL_X), CString(m_sz_rf_on_enbl_x), nLogEx::debug);
	}

	// RF-TRIP NOUSE ON ENABLE ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_NO_USE_ENBL_X), CString(m_sz_no_use_enbl_x), nLogEx::debug);
	}

	// UP-LINK CONTROL ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_UPLINK_CONT_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_uplink_cont_x, sizeof(m_sz_uplink_cont_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_UPLINK_CONT_X), CString(m_sz_uplink_cont_x), nLogEx::debug);
	}

	// RF POWER ON DETECT ANSWER ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RF_ON_DET_ANS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans_x, sizeof(m_sz_rf_on_det_ans_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_RF_ON_DET_ANS_X), CString(m_sz_rf_on_det_ans_x), nLogEx::debug);
	}

	// RF POWER ON DETECT ANSWER ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RF_ON_DET_ANS_S));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans_s, sizeof(m_sz_rf_on_det_ans_s), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_RF_ON_DET_ANS_S), CString(m_sz_rf_on_det_ans_s), nLogEx::debug);
	}

	// HPA-2���ݑΉ�
	// HPA SEL ���
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


	/*** �����[�g�^���[�J�� ��Ԏ擾 ******************************************************/

#if 0
	// S-TX �����[�g�^���[�J�� ���
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

	// S-TXSYN �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_X_TXSYN_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_txsyn_rl, sizeof(m_sz_x_txsyn_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-TXSYN.CONT_MODE"), CString(m_sz_x_txsyn_rl), nLogEx::debug);
	}

	// X-UL_CNT �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_UL_CNT_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_ul_cnt_rl, sizeof(m_sz_x_ul_cnt_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-UL_CNT.X-TX_TOTAL_REMOTE"), CString(m_sz_x_txsyn_rl), nLogEx::debug);
	}

	// X-RF_IF �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RF_IF_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_rf_if_rl, sizeof(m_sz_x_rf_if_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-RF_IF.CONTROL_MODE"), CString(m_sz_x_txsyn_rl), nLogEx::debug);
	}

	// �A���[���`�F�b�N
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

	// �A���[����
	if (strcmp(m_sz_alarm_x, "ON") == 0)
	{
		// �g��Ԃ�����
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

	// �A���[������
	else
	{
		// �g�̐F�����ɖ߂�
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
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RF_POWER_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_power_x, sizeof(m_sz_rf_power_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_RF_POWER_X), CString(m_sz_rf_power_x), nLogEx::debug);
	}

	// RF-POWER ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if (m_l_rf_power_on_cntdwn >= 0 &&
		strcmp(m_sz_rf_power_x, "ON") == 0)
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
		strcmp(m_sz_rf_power_x, "OFF") == 0)
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
		if (strcmp(m_sz_rf_power_x, "ON") == 0)
		{
			show_rf_power_on64();
		}

		// OFF �̏ꍇ
		else
		{
			/*** ���ꏈ������ *************************************************************/
			show_rf_power_off64();
		}
	}

	/*** NO-USE ***************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾 
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_NO_USE_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_x, sizeof(m_sz_no_use_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_NO_USE_X), CString(m_sz_no_use_x), nLogEx::debug);
	}

	// NO-USE ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if (m_l_no_use_on_cntdwn >= 0 &&
		strcmp(m_sz_no_use_x, "ON") == 0)
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
		strcmp(m_sz_no_use_x, "ON") != 0)
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
		if (strcmp(m_sz_no_use_x, "ON") == 0)
		{
			show_no_use_on64();
		}
		// OFF �̏ꍇ
		else
		{
			show_no_use_off64();
		}
	}


	/*** SWEEP-CONT ***********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_SWEEP_CONT_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cont_x, sizeof(m_sz_sweep_cont_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_SWEEP_CONT_X), CString(m_sz_sweep_cont_x), nLogEx::debug);
	}

	// SWEEP-CONT STOP �J�E���g�_�E�����ɊĎ��f�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_sweep_cont_stop_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "STOP") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_stop_cntdwn = -1;
	}
	// SWEEP-CONT STOP �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sweep_cont_stop_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_stop_cntdwn = -1;
	}

	// SWEEP-CONT INC �J�E���g�_�E�����Ƀf�[�^�� INC �ɂȂ����ꍇ
	if (m_l_sweep_cont_inc_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "INC") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_inc_cntdwn = -1;
	}
	// SWEEP-CONT INC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sweep_cont_inc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_inc_cntdwn = -1;
	}

	// SWEEP-CONT INC+DEC �J�E���g�_�E�����Ƀf�[�^�� INC+DEC �ɂȂ����ꍇ
	if (m_l_sweep_cont_incdec_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "INC+DEC") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_incdec_cntdwn = -1;
	}
	// SWEEP-CONT INC+DEC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sweep_cont_incdec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_incdec_cntdwn = -1;
	}

	// SWEEP-CONT DEC �J�E���g�_�E�����Ƀf�[�^�� DEC �ɂȂ����ꍇ
	if (m_l_sweep_cont_dec_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "DEC") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_dec_cntdwn = -1;
	}
	// SWEEP-CONT DEC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sweep_cont_dec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_dec_cntdwn = -1;
	}

	// SWEEP-CONT DEC+INC �J�E���g�_�E�����Ƀf�[�^�� DEC+INC �ɂȂ����ꍇ
	if (m_l_sweep_cont_decinc_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "DEC+INC") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_decinc_cntdwn = -1;
	}
	// SWEEP-CONT DEC+INC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sweep_cont_decinc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_decinc_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_sweep_cont_stop_cntdwn == -1 && m_l_sweep_cont_inc_cntdwn == -1 &&
		m_l_sweep_cont_incdec_cntdwn == -1 && m_l_sweep_cont_dec_cntdwn == -1 &&
		m_l_sweep_cont_decinc_cntdwn == -1)
	{

		// INC �̏ꍇ
		if (strcmp(m_sz_sweep_cont_x, "INC") == 0)
		{
			show_sweep_cont_inc64();
		}

		// INC+DEC �̏ꍇ
		else if (strcmp(m_sz_sweep_cont_x, "INC+DEC") == 0)
		{
			show_sweep_cont_incdec64();
		}

		// DEC+INC �̏ꍇ
		else if (strcmp(m_sz_sweep_cont_x, "DEC+INC") == 0)
		{
			show_sweep_cont_decinc64();
		}

		// DEC �̏ꍇ
		else if (strcmp(m_sz_sweep_cont_x, "DEC") == 0)
		{
			show_sweep_cont_dec64();
		}

		// STOP �̏ꍇ
		else
		{
			show_sweep_cont_stop64();
		}
	}

	/*** RAMP-CONT <<XUP>> �ǉ� ***********************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RATE_AID_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rate_aid_x, sizeof(m_sz_rate_aid_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-TXSYN.RATE_AID"), CString(m_sz_rate_aid_x), nLogEx::debug);
	}

	// RAMP-CONT OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_ramp_cont_off_cntdwn >= 0 &&
		strcmp(m_sz_rate_aid_x, "OFF") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_ramp_cont_off_cntdwn = -1;
	}
	// RAMP-CONT OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_ramp_cont_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_ramp_cont_off_cntdwn = -1;
	}

	// RAMP-CONT ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_ramp_cont_on_cntdwn >= 0 &&
		strcmp(m_sz_rate_aid_x, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_ramp_cont_on_cntdwn = -1;
	}
	// RAMP-CONT ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_ramp_cont_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_ramp_cont_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_ramp_cont_off_cntdwn == -1 && m_l_ramp_cont_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_rate_aid_x, "ON") == 0)
		{
			show_ramp_cont_on64();
		}
		// OFF �̏ꍇ
		else
		{
			show_ramp_cont_off64();
		}
	}

#if 0	// �@��X�V�ɂ��폜
	/*** CMD-MOD **************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾 <<XUP>> X-TX�p�ɕύX
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_CMD_MOD_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_cmd_mod_x, sizeof(m_sz_cmd_mod_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-TX.CMD_MOD"), CString(m_sz_cmd_mod_x), nLogEx::debug);
	}
	// CMD-MOD OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_cmd_mod_off_cntdwn >= 0 &&
		strcmp(m_sz_cmd_mod_x, "OFF") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_cmd_mod_off_cntdwn = -1;
	}
	// CMD-MOD OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_cmd_mod_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_cmd_mod_off_cntdwn = -1;
	}

	// CMD-MOD ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_cmd_mod_on_cntdwn >= 0 &&
		strcmp(m_sz_cmd_mod_x, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_cmd_mod_on_cntdwn = -1;
	}
	// CMD-MOD ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_cmd_mod_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_cmd_mod_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_cmd_mod_off_cntdwn == -1 && m_l_cmd_mod_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_cmd_mod_x, "ON") == 0)
		{
			show_cmd_mod_on64();
		}
		// OFF �̏ꍇ
		else
		{
			show_cmd_mod_off64();
		}
	}

	/*** RNG-MOD **************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾 <<XUP>> X-TX�p�ɕύX
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RNG_MOD_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rng_mod_x, sizeof(m_sz_rng_mod_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-TX.RNG_MOD"), CString(m_sz_rng_mod_x), nLogEx::debug);
	}

	// RNG-MOD OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_rng_mod_off_cntdwn >= 0 &&
		strcmp(m_sz_rng_mod_x, "OFF") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rng_mod_off_cntdwn = -1;
	}
	// RNG-MOD OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rng_mod_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rng_mod_off_cntdwn = -1;
	}

	// RNG-MOD ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_rng_mod_on_cntdwn >= 0 &&
		strcmp(m_sz_rng_mod_x, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rng_mod_on_cntdwn = -1;
	}
	// RNG-MOD ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rng_mod_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rng_mod_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_rng_mod_off_cntdwn == -1 && m_l_rng_mod_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_rng_mod_x, "ON") == 0)
		{
			show_rng_mod_on64();
		}
		// OFF �̏ꍇ
		else
		{
			show_rng_mod_off64();
		}
	}
#endif

	/*** TCRMOD MODULATION ****************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_TCRMOD_STS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_tcrmod_sts, sizeof(m_sz_xx_tcrmod_sts), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_XX_TCRMOD_STS), CString(m_sz_xx_tcrmod_sts), nLogEx::debug);
	}
	// TCRMOD MODULATION OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_modulation_off_cntdwn >= 0 &&
		strcmp(m_sz_xx_tcrmod_sts, "OFF") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_modulation_off_cntdwn = -1;
	}
	// TCRMOD MODULATION OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_modulation_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_modulation_off_cntdwn = -1;
	}

	// TCRMOD MODULATION ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_modulation_on_cntdwn >= 0 &&
		strcmp(m_sz_xx_tcrmod_sts, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_modulation_on_cntdwn = -1;
	}
	// TCRMOD MODULATION ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_modulation_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_modulation_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_modulation_off_cntdwn == -1 && m_l_modulation_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_xx_tcrmod_sts, "ON") == 0)
		{
			show_modulation_on64();
		}
		// OFF �̏ꍇ
		else
		{
			show_modulation_off64();
		}
	}

	/*** X-CMDOUT HOLD_TONE ***************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_HOLD_TONE_STS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hold_tone_sts_x, sizeof(m_sz_hold_tone_sts_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_HOLD_TONE_STS_X), CString(m_sz_hold_tone_sts_x), nLogEx::debug);
	}

	// X-CMDOUT HOLD_TONE OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_hold_tone_off_cntdwn >= 0 &&
		strcmp(m_sz_hold_tone_sts_x, "OFF") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_off_cntdwn = -1;
	}
	// X-CMDOUT HOLD_TONE OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_hold_tone_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_off_cntdwn = -1;
	}

	// X-CMDOUT HOLD_TONE ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_hold_tone_on_cntdwn >= 0 &&
		strcmp(m_sz_hold_tone_sts_x, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_on_cntdwn = -1;
	}
	// TCRMOD MODULATION ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_hold_tone_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_hold_tone_off_cntdwn == -1 && m_l_hold_tone_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_hold_tone_sts_x, "ON") == 0)
		{
			show_hold_tone_on64();
		}
		// OFF �̏ꍇ
		else
		{
			show_hold_tone_off64();
		}
	}

	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	/*** SS-RNG / SX-RNG ******************************************************************/
	/*** ���l�\�����ŐV�f�[�^�\�� *********************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾 
	// �I�΂�Ă��郌���W���u���]���^���Đ��^����RNG�񐔂̕\������؂�ւ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_NORM_REP));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_NORM_REP), CString(m_sz_norm_rep), nLogEx::debug);
	}

	// �Đ��^���I�΂�Ă���ꍇ
	if (strcmp(m_sz_norm_rep, "REPLAY") == 0)
	{
		// �Đ��^�����񐔂𒊏o
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RX_RNG_CNT));
		if (obs_adr != NULL)
		{
			m_d_xx_rng_cnt = obs_adr->d_data;
		}
	}
	// �]���^���I�΂�Ă���ꍇ
	else if (strcmp(m_sz_norm_rep, "NORMAL") == 0)
	{
		// �]���^�����񐔂𒊏o
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_RNG_CNT));
		if (obs_adr != NULL)
		{
			m_d_xx_rng_cnt = obs_adr->d_data;
		}
	}
	// ����ł��Ȃ��ꍇ�͂O�N���A
	else
	{
		m_d_xx_rng_cnt = 0;
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_RNG_NUM));
	if (obs_adr != NULL)
	{
		m_d_xx_rng_num = obs_adr->d_data;
	}

	// ���l�\�������X�V
	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_xx_rng_cnt % 100);
	m_strXXRng = CString(sz_buff);

	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_xx_rng_num % 100);
	m_XXRngNumBtn.SetWindowTextW(CString(sz_buff));

	/*** "CONTINUATION" �\���L�� ����� �Z���V�e�B�u���� **********************************/
	// ���݂̏�Ԃ��Ď��c�a����擾 
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_CONT_FLAG_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_cont_flag_x, sizeof(m_sz_cont_flag_x), obs_adr->sz_sts_name);
	}

	// �ߒn���q���̏ꍇ
	if (strcmp(m_sz_cont_flag_x, "NEAR") == 0)
	{
		// XX-RNG �̐��l�\������� "CONTINUATION" �ƕ\��
		m_strXXRngCont = _T("CONTINUATION");

		// ���l�\�����̃Z���V�e�B�u�𗎂Ƃ�
		m_XXRngNumBtn.EnableWindow(FALSE);
	}
	// �[�F���q���̏ꍇ
	else
	{
		// XX-RNG �̐��l�\������� "CONTINUATION" �������i����΁j
		m_strXXRngCont = _T("");

		// ���l�\�����̃Z���V�e�B�u��t����
		m_XXRngNumBtn.EnableWindow(TRUE);
	}


	/*** COH/INCOH ************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_COH_INCOH));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_coh_incoh, sizeof(m_sz_coh_incoh), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("V-EQU.COHI"), CString(m_sz_coh_incoh), nLogEx::debug);
	}

	// COH/INCOH COH �J�E���g�_�E�����Ƀf�[�^�� COH �ɂȂ����ꍇ
	if (m_l_coh_incoh_coh_cntdwn >= 0 &&
		strcmp(m_sz_coh_incoh, "COHI") == 0)
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
		strcmp(m_sz_coh_incoh, "INCOHI") == 0)
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
		if (strcmp(m_sz_coh_incoh, "INCOHI") == 0)
		{
			show_coh_incoh_incoh64();
		}
		// COH �̏ꍇ
		else
		{
			show_coh_incoh_coh64();
		}
	}


	/*** RNGING ***************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("CTRL.RNG_FLG"), CString(m_sz_rnging), nLogEx::debug);
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

	// (�Đ��^) RNGING STOP �J�E���g�_�E�����Ƀf�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_rep_rnging_stop_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "�I��") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rep_rnging_stop_cntdwn = -1;
	}
	// (�Đ��^) RNGING STOP �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rep_rnging_stop_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rep_rnging_stop_cntdwn = -1;
	}

	// (�Đ��^) RNGING START �J�E���g�_�E�����Ƀf�[�^�� START �ɂȂ����ꍇ
	if (m_l_rep_rnging_start_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "�J�n") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rep_rnging_start_cntdwn = -1;
	}
	// (�Đ��^) RNGING START �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rep_rnging_start_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rep_rnging_start_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_rnging_stop_cntdwn == -1 && m_l_rnging_start_cntdwn == -1)
	{
		// START �̏ꍇ
		if (strcmp(m_sz_rnging, "�J�n") == 0)
		{
			show_rnging_start64();
		}
		// STOP �̏ꍇ
		else
		{
			show_rnging_stop64();
		}
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_rep_rnging_stop_cntdwn == -1 && m_l_rep_rnging_start_cntdwn == -1)
	{
		// START �̏ꍇ
		if (strcmp(m_sz_rnging, "�J�n") == 0)
		{
			show_rrng_start64();
		}
		// STOP �̏ꍇ
		else
		{
			show_rrng_stop64();
		}
	}

	/*** �ϒ����u�̑I����Ԋm�F ***********************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_MOD_IN_SEL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_mod_in_sel, sizeof(m_sz_mod_in_sel), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-IFSW.IF_CH_INPUT_SEL_ANS"), CString(m_sz_mod_in_sel), nLogEx::debug);
	}

	// TCRMOD���I������Ă���ꍇ
	if (strcmp(m_sz_mod_in_sel, "TCRMOD") == 0)
	{
		// TCRMOD���Z���V�e�B�u�APM MOD���C���Z���V�e�B�u�ɂ���
		m_ModulationOffBtn.EnableWindow(TRUE);
		m_ModulationOnBtn.EnableWindow(TRUE);
		m_HoldToneOffBtn.EnableWindow(TRUE);
		m_HoldToneOnBtn.EnableWindow(TRUE);
		m_CmdModOffBtn.EnableWindow(FALSE);
		m_CmdModOnBtn.EnableWindow(FALSE);
		m_RngModOffBtn.EnableWindow(FALSE);
		m_RngModOnBtn.EnableWindow(FALSE);
	}
	// PM MOD���I������Ă���ꍇ
	else if (strcmp(m_sz_mod_in_sel, "MOD") == 0)
	{
		// TCRMOD���C���Z���V�e�B�u�APM MOD���Z���V�e�B�u�ɂ���
		m_ModulationOffBtn.EnableWindow(FALSE);
		m_ModulationOnBtn.EnableWindow(FALSE);
		m_HoldToneOffBtn.EnableWindow(FALSE);
		m_HoldToneOnBtn.EnableWindow(FALSE);
		m_CmdModOffBtn.EnableWindow(TRUE);
		m_CmdModOnBtn.EnableWindow(TRUE);
		m_RngModOffBtn.EnableWindow(TRUE);
		m_RngModOnBtn.EnableWindow(TRUE);
	}
	// ���Â�̏�Ԃł��Ȃ��ꍇ
	else
	{
		// �ǂ�����C���Z���V�e�B�u�ɂ���
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

-# ���M�����ʏ�ԕ\���ݒ�

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_opsc_condition34()
{
	TRACE(_T("show_opsc_condition\n"));

	char sz_buff[64];
	char szRfPowerSts[32];

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// ���䌠�͏�ɗL��
	m_i_priv_flag = 1;

	/*-- TX CONTROL ----------------------------------------------------------------------*/

	// RF POWER ON ENABLE ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X34_RF_ON_ENBL_X), CString(m_sz_rf_on_enbl_x), nLogEx::debug);
	}

	// RF-TRIP NOUSE ON ENABLE ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X34_NO_USE_ENBL_X), CString(m_sz_no_use_enbl_x), nLogEx::debug);
	}

	// UP-LINK CONTROL ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_UPLINK_CONT_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_uplink_cont_x, sizeof(m_sz_uplink_cont_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X34_UPLINK_CONT_X), CString(m_sz_uplink_cont_x), nLogEx::debug);
	}

	// RF POWER ON DETECT ANSWER ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_RF_ON_DET_ANS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans_x, sizeof(m_sz_rf_on_det_ans_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X34_RF_ON_DET_ANS_X), CString(m_sz_rf_on_det_ans_x), nLogEx::debug);
	}

	// RF POWER ON DETECT ANSWER ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_RF_ON_DET_ANS_S));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans_s, sizeof(m_sz_rf_on_det_ans_s), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X34_RF_ON_DET_ANS_S), CString(m_sz_rf_on_det_ans_s), nLogEx::debug);
	}


	/*** �����[�g�^���[�J�� ��Ԏ擾 ******************************************************/

	// S-TX �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_X_TX_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_tx_rl, sizeof(m_sz_x_tx_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_X_TX_RL), CString(m_sz_x_tx_rl), nLogEx::debug);
	}

	// S-TXSYN �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_X_TXSYN_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_txsyn_rl, sizeof(m_sz_x_txsyn_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_X_TXSYN_RL), CString(m_sz_x_txsyn_rl), nLogEx::debug);
	}

	/*** �A���[���`�F�b�N  X-TX�p�ɕύX *******************************************/
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_ALARM_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_alarm_x, sizeof(m_sz_alarm_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_ALARM_X), CString(m_sz_alarm_x), nLogEx::debug);
	}

	// �A���[����
	if (strcmp(m_sz_alarm_x, "ON") == 0)
	{
		// �g��Ԃ�����
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

	// �A���[������
	else
	{
		// �g�̐F�����ɖ߂�
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
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_RF_POWER_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_power_x, sizeof(m_sz_rf_power_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X34_RF_POWER_X), CString(m_sz_rf_power_x), nLogEx::debug);
	}

	// HPA����RF���
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

	// HPA/SSPA�ŏ����𕪂���
	if (strcmp(m_sz_xpa_sel, "HPA") == 0)
	{
		// HPA���I������Ă���ꍇ
		strcpy_s(szRfPowerSts, sizeof(szRfPowerSts), m_sz_hpa_rf_power_x);
	}
	else
	{
		// SSPA���I������Ă���ꍇ
		strcpy_s(szRfPowerSts, sizeof(szRfPowerSts), m_sz_rf_power_x);
	}

	// RF-POWER ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if (m_l_rf_power_on_cntdwn >= 0 &&
		strcmp(szRfPowerSts, "ON") == 0)
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
		strcmp(szRfPowerSts, "OFF") == 0)
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
		if (strcmp(szRfPowerSts, "ON") == 0)
		{
			show_rf_power_on34();
		}

		// OFF �̏ꍇ
		else
		{
			/*** ���ꏈ������ *************************************************************/
			show_rf_power_off34();
		}
	}

	/*** NO-USE ***************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾 
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_NO_USE_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_x, sizeof(m_sz_no_use_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X34_NO_USE_X), CString(m_sz_no_use_x), nLogEx::debug);
	}

	// NO-USE ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if (m_l_no_use_on_cntdwn >= 0 &&
		strcmp(m_sz_no_use_x, "ON") == 0)
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
		strcmp(m_sz_no_use_x, "ON") != 0)
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
		if (strcmp(m_sz_no_use_x, "ON") == 0)
		{
			show_no_use_on34();
		}
		// OFF �̏ꍇ
		else
		{
			show_no_use_off34();
		}
	}


	/*** SWEEP-CONT ***********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_SWEEP_CONT_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cont_x, sizeof(m_sz_sweep_cont_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_SWEEP_CONT_X), CString(m_sz_sweep_cont_x), nLogEx::debug);
	}

	// SWEEP-CONT STOP �J�E���g�_�E�����ɊĎ��f�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_sweep_cont_stop_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "STOP") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_stop_cntdwn = -1;
	}
	// SWEEP-CONT STOP �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sweep_cont_stop_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_stop_cntdwn = -1;
	}

	// SWEEP-CONT INC �J�E���g�_�E�����Ƀf�[�^�� INC �ɂȂ����ꍇ
	if (m_l_sweep_cont_inc_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "INC") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_inc_cntdwn = -1;
	}
	// SWEEP-CONT INC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sweep_cont_inc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_inc_cntdwn = -1;
	}

	// SWEEP-CONT INC+DEC �J�E���g�_�E�����Ƀf�[�^�� INC+DEC �ɂȂ����ꍇ
	if (m_l_sweep_cont_incdec_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "INC+DEC") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_incdec_cntdwn = -1;
	}
	// SWEEP-CONT INC+DEC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sweep_cont_incdec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_incdec_cntdwn = -1;
	}

	// SWEEP-CONT DEC �J�E���g�_�E�����Ƀf�[�^�� DEC �ɂȂ����ꍇ
	if (m_l_sweep_cont_dec_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "DEC") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_dec_cntdwn = -1;
	}
	// SWEEP-CONT DEC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sweep_cont_dec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_dec_cntdwn = -1;
	}

	// SWEEP-CONT DEC+INC �J�E���g�_�E�����Ƀf�[�^�� DEC+INC �ɂȂ����ꍇ
	if (m_l_sweep_cont_decinc_cntdwn >= 0 &&
		strcmp(m_sz_sweep_cont_x, "DEC+INC") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_decinc_cntdwn = -1;
	}
	// SWEEP-CONT DEC+INC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sweep_cont_decinc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_cont_decinc_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_sweep_cont_stop_cntdwn == -1 && m_l_sweep_cont_inc_cntdwn == -1 &&
		m_l_sweep_cont_incdec_cntdwn == -1 && m_l_sweep_cont_dec_cntdwn == -1 &&
		m_l_sweep_cont_decinc_cntdwn == -1)
	{

		// INC �̏ꍇ
		if (strcmp(m_sz_sweep_cont_x, "INC") == 0)
		{
			show_sweep_cont_inc34();
		}

		// INC+DEC �̏ꍇ
		else if (strcmp(m_sz_sweep_cont_x, "INC+DEC") == 0)
		{
			show_sweep_cont_incdec34();
		}

		// DEC+INC �̏ꍇ
		else if (strcmp(m_sz_sweep_cont_x, "DEC+INC") == 0)
		{
			show_sweep_cont_decinc34();
		}

		// DEC �̏ꍇ
		else if (strcmp(m_sz_sweep_cont_x, "DEC") == 0)
		{
			show_sweep_cont_dec34();
		}

		// STOP �̏ꍇ
		else
		{
			show_sweep_cont_stop34();
		}
	}

	/*** RAMP-CONT <<XUP>> �ǉ� ***********************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RATE_AID_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rate_aid_x, sizeof(m_sz_rate_aid_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_RATE_AID_X), CString(m_sz_rate_aid_x), nLogEx::debug);
	}

	// RAMP-CONT OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_ramp_cont_off_cntdwn >= 0 &&
		strcmp(m_sz_rate_aid_x, "OFF") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_ramp_cont_off_cntdwn = -1;
	}
	// RAMP-CONT OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_ramp_cont_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_ramp_cont_off_cntdwn = -1;
	}

	// RAMP-CONT ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_ramp_cont_on_cntdwn >= 0 &&
		strcmp(m_sz_rate_aid_x, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_ramp_cont_on_cntdwn = -1;
	}
	// RAMP-CONT ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_ramp_cont_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_ramp_cont_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_ramp_cont_off_cntdwn == -1 && m_l_ramp_cont_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_rate_aid_x, "ON") == 0)
		{
			show_ramp_cont_on34();
		}
		// OFF �̏ꍇ
		else
		{
			show_ramp_cont_off34();
		}
	}

	/*** CMD-MOD **************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾 <<XUP>> X-TX�p�ɕύX
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_CMD_MOD_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_cmd_mod_x, sizeof(m_sz_cmd_mod_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-TX.CMD_MOD"), CString(m_sz_cmd_mod_x), nLogEx::debug);
	}
	// CMD-MOD OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_cmd_mod_off_cntdwn >= 0 &&
		strcmp(m_sz_cmd_mod_x, "OFF") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_cmd_mod_off_cntdwn = -1;
	}
	// CMD-MOD OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_cmd_mod_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_cmd_mod_off_cntdwn = -1;
	}

	// CMD-MOD ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_cmd_mod_on_cntdwn >= 0 &&
		strcmp(m_sz_cmd_mod_x, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_cmd_mod_on_cntdwn = -1;
	}
	// CMD-MOD ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_cmd_mod_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_cmd_mod_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_cmd_mod_off_cntdwn == -1 && m_l_cmd_mod_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_cmd_mod_x, "ON") == 0)
		{
			show_cmd_mod_on34();
		}
		// OFF �̏ꍇ
		else
		{
			show_cmd_mod_off34();
		}
	}

	/*** RNG-MOD **************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾 <<XUP>> X-TX�p�ɕύX
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RNG_MOD_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rng_mod_x, sizeof(m_sz_rng_mod_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("X-TX.RNG_MOD"), CString(m_sz_rng_mod_x), nLogEx::debug);
	}

	// RNG-MOD OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_rng_mod_off_cntdwn >= 0 &&
		strcmp(m_sz_rng_mod_x, "OFF") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rng_mod_off_cntdwn = -1;
	}
	// RNG-MOD OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rng_mod_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rng_mod_off_cntdwn = -1;
	}

	// RNG-MOD ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_rng_mod_on_cntdwn >= 0 &&
		strcmp(m_sz_rng_mod_x, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rng_mod_on_cntdwn = -1;
	}
	// RNG-MOD ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rng_mod_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rng_mod_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_rng_mod_off_cntdwn == -1 && m_l_rng_mod_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_rng_mod_x, "ON") == 0)
		{
			show_rng_mod_on34();
		}
		// OFF �̏ꍇ
		else
		{
			show_rng_mod_off34();
		}
	}

	/*** TCRMOD MODULATION ****************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_TCRMOD_STS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_tcrmod_sts, sizeof(m_sz_xx_tcrmod_sts), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_XX_TCRMOD_STS), CString(m_sz_xx_tcrmod_sts), nLogEx::debug);
	}
	// TCRMOD MODULATION OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_modulation_off_cntdwn >= 0 &&
		strcmp(m_sz_xx_tcrmod_sts, "OFF") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_modulation_off_cntdwn = -1;
	}
	// TCRMOD MODULATION OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_modulation_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_modulation_off_cntdwn = -1;
	}

	// TCRMOD MODULATION ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_modulation_on_cntdwn >= 0 &&
		strcmp(m_sz_xx_tcrmod_sts, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_modulation_on_cntdwn = -1;
	}
	// TCRMOD MODULATION ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_modulation_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_modulation_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_modulation_off_cntdwn == -1 && m_l_modulation_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_xx_tcrmod_sts, "ON") == 0)
		{
			show_modulation_on34();
		}
		// OFF �̏ꍇ
		else
		{
			show_modulation_off34();
		}
	}

	/*** X-CMDOUT HOLD_TONE ***************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_HOLD_TONE_STS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hold_tone_sts_x, sizeof(m_sz_hold_tone_sts_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X34_HOLD_TONE_STS_X), CString(m_sz_hold_tone_sts_x), nLogEx::debug);
	}

	// X-CMDOUT HOLD_TONE OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_hold_tone_off_cntdwn >= 0 &&
		strcmp(m_sz_hold_tone_sts_x, "OFF") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_off_cntdwn = -1;
	}
	// X-CMDOUT HOLD_TONE OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_hold_tone_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_off_cntdwn = -1;
	}

	// X-CMDOUT HOLD_TONE ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_hold_tone_on_cntdwn >= 0 &&
		strcmp(m_sz_hold_tone_sts_x, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_on_cntdwn = -1;
	}
	// TCRMOD MODULATION ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_hold_tone_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_hold_tone_off_cntdwn == -1 && m_l_hold_tone_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_hold_tone_sts_x, "ON") == 0)
		{
			show_hold_tone_on34();
		}
		// OFF �̏ꍇ
		else
		{
			show_hold_tone_off34();
		}
	}

	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	/*** SS-RNG / SX-RNG ******************************************************************/
	/*** ���l�\�����ŐV�f�[�^�\�� *********************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾 
	// �I�΂�Ă��郌���W���u���]���^���Đ��^����RNG�񐔂̕\������؂�ւ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_NORM_REP));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X_NORM_REP), CString(m_sz_norm_rep), nLogEx::debug);
	}

	// �Đ��^���I�΂�Ă���ꍇ
	if (strcmp(m_sz_norm_rep, "REPLAY") == 0)
	{
		// �Đ��^�����񐔂𒊏o
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RX_RNG_CNT));
		if (obs_adr != NULL)
		{
			m_d_xx_rng_cnt = obs_adr->d_data;
		}
	}
	// �]���^���I�΂�Ă���ꍇ
	else if (strcmp(m_sz_norm_rep, "NORMAL") == 0)
	{
		// �]���^�����񐔂𒊏o
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_RNG_CNT));
		if (obs_adr != NULL)
		{
			m_d_xx_rng_cnt = obs_adr->d_data;
		}
	}
	// ����ł��Ȃ��ꍇ�͂O�N���A
	else
	{
		m_d_xx_rng_cnt = 0;
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_XX_RNG_NUM));
	if (obs_adr != NULL)
	{
		m_d_xx_rng_num = obs_adr->d_data;
	}

	// ���l�\�������X�V
	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_xx_rng_cnt % 100);
	m_strXXRng = CString(sz_buff);

	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_xx_rng_num % 100);
	m_XXRngNumBtn.SetWindowTextW(CString(sz_buff));

	/*** "CONTINUATION" �\���L�� ����� �Z���V�e�B�u���� **********************************/
	// ���݂̏�Ԃ��Ď��c�a����擾 
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_CONT_FLAG_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_cont_flag_x, sizeof(m_sz_cont_flag_x), obs_adr->sz_sts_name);
	}

	// �ߒn���q���̏ꍇ
	if (strcmp(m_sz_cont_flag_x, "NEAR") == 0)
	{
		// XX-RNG �̐��l�\������� "CONTINUATION" �ƕ\��
		m_strXXRngCont = _T("CONTINUATION");

		// ���l�\�����̃Z���V�e�B�u�𗎂Ƃ�
		m_XXRngNumBtn.EnableWindow(FALSE);
	}
	// �[�F���q���̏ꍇ
	else
	{
		// XX-RNG �̐��l�\������� "CONTINUATION" �������i����΁j
		m_strXXRngCont = _T("");

		// ���l�\�����̃Z���V�e�B�u��t����
		m_XXRngNumBtn.EnableWindow(TRUE);
	}


	/*** COH/INCOH ************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_COH_INCOH));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_coh_incoh, sizeof(m_sz_coh_incoh), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("V-EQU.COHI"), CString(m_sz_coh_incoh), nLogEx::debug);
	}

	// COH/INCOH COH �J�E���g�_�E�����Ƀf�[�^�� COH �ɂȂ����ꍇ
	if (m_l_coh_incoh_coh_cntdwn >= 0 &&
		strcmp(m_sz_coh_incoh, "COHI") == 0)
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
		strcmp(m_sz_coh_incoh, "INCOHI") == 0)
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
		if (strcmp(m_sz_coh_incoh, "INCOHI") == 0)
		{
			show_coh_incoh_incoh34();
		}
		// COH �̏ꍇ
		else
		{
			show_coh_incoh_coh34();
		}
	}


	/*** RNGING ***************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), _T("CTRL.RNG_FLG"), CString(m_sz_rnging), nLogEx::debug);
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

	// (�Đ��^) RNGING STOP �J�E���g�_�E�����Ƀf�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_rep_rnging_stop_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "�I��") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rep_rnging_stop_cntdwn = -1;
	}
	// (�Đ��^) RNGING STOP �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rep_rnging_stop_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rep_rnging_stop_cntdwn = -1;
	}

	// (�Đ��^) RNGING START �J�E���g�_�E�����Ƀf�[�^�� START �ɂȂ����ꍇ
	if (m_l_rep_rnging_start_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "�J�n") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rep_rnging_start_cntdwn = -1;
	}
	// (�Đ��^) RNGING START �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rep_rnging_start_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rep_rnging_start_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_rnging_stop_cntdwn == -1 && m_l_rnging_start_cntdwn == -1)
	{
		// START �̏ꍇ
		if (strcmp(m_sz_rnging, "�J�n") == 0)
		{
			show_rnging_start34();
		}
		// STOP �̏ꍇ
		else
		{
			show_rnging_stop34();
		}
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_rep_rnging_stop_cntdwn == -1 && m_l_rep_rnging_start_cntdwn == -1)
	{
		// START �̏ꍇ
		if (strcmp(m_sz_rnging, "�J�n") == 0)
		{
			show_rrng_start34();
		}
		// STOP �̏ꍇ
		else
		{
			show_rrng_stop34();
		}
	}

	/*** �ϒ����u�̑I����Ԋm�F ***********************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_MOD_IN_SEL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_mod_in_sel, sizeof(m_sz_mod_in_sel), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlX"), _T("show_opsc_condition"), CString(MON_X34_MOD_IN_SEL), CString(m_sz_mod_in_sel), nLogEx::debug);
	}

	// TCRMOD���I������Ă���ꍇ
	if (strcmp(m_sz_mod_in_sel, "TCRMOD") == 0)
	{
		// TCRMOD���Z���V�e�B�u�APM MOD���C���Z���V�e�B�u�ɂ���
		m_ModulationOffBtn.EnableWindow(TRUE);
		m_ModulationOnBtn.EnableWindow(TRUE);
		m_HoldToneOffBtn.EnableWindow(TRUE);
		m_HoldToneOnBtn.EnableWindow(TRUE);
		m_CmdModOffBtn.EnableWindow(FALSE);
		m_CmdModOnBtn.EnableWindow(FALSE);
		m_RngModOffBtn.EnableWindow(FALSE);
		m_RngModOnBtn.EnableWindow(FALSE);
	}
	// PM MOD���I������Ă���ꍇ
	else if (strcmp(m_sz_mod_in_sel, "MOD") == 0)
	{
		// TCRMOD���C���Z���V�e�B�u�APM MOD���Z���V�e�B�u�ɂ���
		m_ModulationOffBtn.EnableWindow(FALSE);
		m_ModulationOnBtn.EnableWindow(FALSE);
		m_HoldToneOffBtn.EnableWindow(FALSE);
		m_HoldToneOnBtn.EnableWindow(FALSE);
		m_CmdModOffBtn.EnableWindow(TRUE);
		m_CmdModOnBtn.EnableWindow(TRUE);
		m_RngModOffBtn.EnableWindow(TRUE);
		m_RngModOnBtn.EnableWindow(TRUE);
	}
	// ���Â�̏�Ԃł��Ȃ��ꍇ
	else
	{
		// �ǂ�����C���Z���V�e�B�u�ɂ���
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

-# �J�E���g�_�E���^�C���A�E�g

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::count_down()
{
	TRACE(_T("count_down\n"));
	KillTimer(TID_COUNTDOWN);

	// 0 �ȏ�̓J�E���g�_�E��

	/*-- TX CONTROL ----------------------------------------------------------------------*/

	// RF-POWER OFF
	if (m_l_rf_power_off_cntdwn > 0) 
	{
		TRACE(_T("RF-POWER OFF �J�E���g�_�E��:%d\n"), m_l_rf_power_off_cntdwn);
		m_l_rf_power_off_cntdwn--;
	}

	// RF-POWER ON
	if (m_l_rf_power_on_cntdwn > 0) 
	{
		TRACE(_T("RF-POWER ON �J�E���g�_�E��:%d\n"), m_l_rf_power_on_cntdwn);
		m_l_rf_power_on_cntdwn--;
	}

	// NO-USE OFF
	if (m_l_no_use_off_cntdwn > 0) 
	{
		TRACE(_T("NO-USE OFF �J�E���g�_�E��:%d\n"), m_l_no_use_off_cntdwn);
		m_l_no_use_off_cntdwn--;
	}

	// NO-USE ON
	if (m_l_no_use_on_cntdwn > 0) 
	{
		TRACE(_T("NO-USE ON �J�E���g�_�E��:%d\n"), m_l_no_use_on_cntdwn);
		m_l_no_use_on_cntdwn--;
	}

	// SWEEP-CONT STOP
	if (m_l_sweep_cont_stop_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT STOP �J�E���g�_�E��:%d\n"), m_l_sweep_cont_stop_cntdwn);
		m_l_sweep_cont_stop_cntdwn--;
	}

	// SWEEP-CONT INC
	if (m_l_sweep_cont_inc_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT INC �J�E���g�_�E��:%d\n"), m_l_sweep_cont_inc_cntdwn);
		m_l_sweep_cont_inc_cntdwn--;
	}

	// SWEEP-CONT INC+DEC
	if (m_l_sweep_cont_incdec_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT INC+DEC �J�E���g�_�E��:%d\n"), m_l_sweep_cont_incdec_cntdwn);
		m_l_sweep_cont_incdec_cntdwn--;
	}

	// SWEEP-CONT DEC
	if (m_l_sweep_cont_dec_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT DEC �J�E���g�_�E��:%d\n"), m_l_sweep_cont_dec_cntdwn);
		m_l_sweep_cont_dec_cntdwn--;
	}

	// SWEEP-CONT DEC+INC
	if (m_l_sweep_cont_decinc_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT DEC+INCC �J�E���g�_�E��:%d\n"), m_l_sweep_cont_decinc_cntdwn);
		m_l_sweep_cont_decinc_cntdwn--;
	}

	// RAMP-CONT OFF
	if (m_l_ramp_cont_off_cntdwn > 0) 
	{
		TRACE(_T("RAMP-CONT OFF �J�E���g�_�E��:%d\n"), m_l_ramp_cont_off_cntdwn);
		m_l_ramp_cont_off_cntdwn--;
	}

	// RAMP-CONT ON
	if (m_l_ramp_cont_on_cntdwn > 0) 
	{
		TRACE(_T("RAMP-CONT ON �J�E���g�_�E��:%d\n"), m_l_ramp_cont_on_cntdwn);
		m_l_ramp_cont_on_cntdwn--;
	}

	// CMD-MOD OFF
	if (m_l_cmd_mod_off_cntdwn > 0) 
	{
		TRACE(_T("CMD-MOD OFF �J�E���g�_�E��:%d\n"), m_l_cmd_mod_off_cntdwn);
		m_l_cmd_mod_off_cntdwn--;
	}

	// CMD-MOD ON
	if (m_l_cmd_mod_on_cntdwn > 0) 
	{
		TRACE(_T("CMD-MOD ON �J�E���g�_�E��:%d\n"), m_l_cmd_mod_on_cntdwn);
		m_l_cmd_mod_on_cntdwn--;
	}

	// RNG-MOD OFF
	if (m_l_rng_mod_off_cntdwn > 0) 
	{
		TRACE(_T("RNG-MOD OFF �J�E���g�_�E��:%d\n"), m_l_rng_mod_off_cntdwn);
		m_l_rng_mod_off_cntdwn--;
	}

	// RNG-MOD ON
	if (m_l_rng_mod_on_cntdwn> 0) 
	{
		TRACE(_T("RNG-MOD ON �J�E���g�_�E��:%d\n"), m_l_rng_mod_on_cntdwn);
		m_l_rng_mod_on_cntdwn--;
	}

	// TCRMOD MODULATION ON
	if (m_l_modulation_on_cntdwn > 0)
	{
		TRACE(_T("TCRMOD MODULATION ON �J�E���g�_�E��:%d\n"), m_l_modulation_on_cntdwn);
		m_l_modulation_on_cntdwn--;
	}

	// TCRMOD MODULATION OFF
	if (m_l_modulation_off_cntdwn > 0)
	{
		TRACE(_T("TCRMOD MODULATION OFF �J�E���g�_�E��:%d\n"), m_l_modulation_off_cntdwn);
		m_l_modulation_off_cntdwn--;
	}

	// X-CMDOUT HOLD TONE ON
	if (m_l_hold_tone_on_cntdwn > 0)
	{
		TRACE(_T("X-CMDOUT HOLD TONE ON �J�E���g�_�E��:%d\n"), m_l_hold_tone_on_cntdwn);
		m_l_hold_tone_on_cntdwn--;
	}

	// X-CMDOUT HOLD TONE OFF
	if (m_l_hold_tone_off_cntdwn > 0) {
		TRACE(_T("X-CMDOUT HOLD TONE OFF �J�E���g�_�E��:%d\n"), m_l_hold_tone_off_cntdwn);
		m_l_hold_tone_off_cntdwn--;
	}

	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	// COH/INCOH COH
	if (m_l_coh_incoh_coh_cntdwn > 0) 
	{
		TRACE(_T("COH/INCOH COH �J�E���g�_�E��:%d\n"), m_l_coh_incoh_coh_cntdwn);
		m_l_coh_incoh_coh_cntdwn--;
	}

	// COH/INCOH INCOH
	if (m_l_coh_incoh_incoh_cntdwn > 0) 
	{
		TRACE(_T("COH/INCOH INCOH �J�E���g�_�E��:%d\n"), m_l_coh_incoh_incoh_cntdwn);
		m_l_coh_incoh_incoh_cntdwn--;
	}

	// RNGING STOP (�]���^)
	if (m_l_rnging_stop_cntdwn > 0) 
	{
		TRACE(_T("RNGING STOP (�]���^) �J�E���g�_�E��:%d\n"), m_l_rnging_stop_cntdwn);
		m_l_rnging_stop_cntdwn--;
	}

	// RNGING START (�]���^)
	if (m_l_rnging_start_cntdwn > 0) 
	{
		TRACE(_T("RNGING START (�]���^) �J�E���g�_�E��:%d\n"), m_l_rnging_start_cntdwn);
		m_l_rnging_start_cntdwn--;
	}

	// RNGING STOP (�Đ��^)
	if (m_l_rep_rnging_stop_cntdwn > 0) 
	{
		TRACE(_T("RNGING STOP (�Đ��^) �J�E���g�_�E��:%d\n"), m_l_rep_rnging_stop_cntdwn);
		m_l_rep_rnging_stop_cntdwn--;
	}

	// RNGING START (�Đ��^)
	if (m_l_rep_rnging_start_cntdwn > 0) 
	{
		TRACE(_T("RNGING START (�Đ��^) �J�E���g�_�E��:%d\n"), m_l_rep_rnging_start_cntdwn);
		m_l_rep_rnging_start_cntdwn--;
	}

	SetTimer(TID_COUNTDOWN, INTERVAL_COUNTDOWN, 0);
}


/*============================================================================*/
/*! CTrsControlX

-# RF-POWER OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_rf_power_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	// OFF��
	if ((strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") == 0)) 
	{
		// �����[�g�i�I���j
		m_RFPWROffBtn.SetColor(COLOR_REMOTES);
	}
	else 
	{
		// ���[�J��
		m_RFPWROffBtn.SetColor(COLOR_LOCAL);
	}

	// ON��
	if ((strcmp (m_sz_x_ul_cnt_rl , "REMOTE") == 0) &&  //�ǉ�  2017/05/08  as-04
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&  //�ǉ�  2017/05/08  as-04
		(strcmp(m_sz_rf_on_enbl_x, "ON") == 0) &&
		(strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") == 0) &&  //�ǉ�  2017/05/08  as-04
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") == 0) &&  //�ǉ�  2017/05/08  as-04
		(m_i_priv_flag == 1)) 
	{
		// S-TX��LAN�ڑ���Ԃ��m�F����
		if (strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) 
		{
			// S-TX RF DET ANS ���m�F����
			if (strcmp(m_sz_rf_on_det_ans_s, "OFF") == 0) 
			{
				// �����[�g�i��I���j
				m_RFPWROnBtn.SetColor(COLOR_REMOTE);
			}
			else 
			{
				// ���[�J��
				m_RFPWROnBtn.SetColor(COLOR_LOCAL);
			}
		}
		else 
		{
			// �����[�g�i��I���j
			m_RFPWROnBtn.SetColor(COLOR_REMOTE);
		}
	}
	else 
	{
		// ���[�J��
		m_RFPWROnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RFPWROffBtn.Invalidate();
	m_RFPWROnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlX

-# RF-POWER OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_rf_power_off34()
{
	char	szRfOnEnblSts[32];

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	// HPA����RF���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_HPA_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hpa_rf_on_enbl_x, sizeof(m_sz_hpa_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	// HPA/SSPA�ŏ����𕪂���
	if (strcmp(m_sz_xpa_sel, "HPA") == 0)
	{
		// HPA���I������Ă���ꍇ
		strcpy_s(szRfOnEnblSts, sizeof(szRfOnEnblSts), m_sz_hpa_rf_on_enbl_x);
	}
	else
	{
		// SSPA���I������Ă���ꍇ
		strcpy_s(szRfOnEnblSts, sizeof(szRfOnEnblSts), m_sz_rf_on_enbl_x);
	}

	if (strcmp(m_sz_lan_x_tx, "�ڑ�") == 0)
	{
		// �����[�g�i�I���j 
		m_RFPWROffBtn.SetColor(COLOR_REMOTES);
	}
	else
	{
		// ���[�J��
		m_RFPWROffBtn.SetColor(COLOR_LOCAL);
	}

	// ON��
	if ((strcmp(m_sz_x_tx_rl, "REMOTE") == 0) &&
		(strcmp(szRfOnEnblSts, "ON") == 0) &&
		(strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// S-TX��LAN�ڑ���Ԃ��m�F����
		if (strcmp(m_sz_lan_s_tx, "�ڑ�") == 0)
		{
			// S-TX RF DET ANS ���m�F����
			if (strcmp(m_sz_rf_on_det_ans_s, "OFF") == 0)
			{
				// �����[�g�i��I���j
				m_RFPWROnBtn.SetColor(COLOR_REMOTE);
			}
			else
			{
				// ���[�J��
				m_RFPWROnBtn.SetColor(COLOR_LOCAL);
			}
		}
		else
		{
			// �����[�g�i��I���j
			m_RFPWROnBtn.SetColor(COLOR_REMOTE);
		}
	}
	else
	{
		// ���[�J��
		m_RFPWROnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RFPWROffBtn.Invalidate();
	m_RFPWROnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RF-POWER ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_rf_power_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	// HPA-2 ���ݑΉ�
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

	// �Δ�������
	if ((strcmp(m_sz_lan_x_tx, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") != 0))
	{
		// ���[�J��
		m_RFPWROnBtn.SetColor(COLOR_LOCAL);
	}
	else if (((strcmp(m_sz_x_ul_cnt_rl, "LOCAL") == 0)) &&
		(((strcmp(m_sz_hpa_sel, "NO1") == 0) && (strcmp(m_sz_rf_on_det_ans_x, "OFF") == 0))
		|| ((strcmp(m_sz_hpa_sel, "NO2") == 0) && (strcmp(m_sz_rf_on_det_ans_x_h2, "OFF") == 0)))) 
	{
		// �����[�g�i�I���j�^ ���[�J��
		m_RFPWROnBtn.SetColor(COLOR_REMOTES, COLOR_LOCAL, TRUE);
	}
	else if (((strcmp(m_sz_hpa_sel, "NO1") == 0) && (strcmp(m_sz_rf_on_det_ans_x, "OFF") == 0))
		|| ((strcmp(m_sz_hpa_sel, "NO2") == 0) && (strcmp(m_sz_rf_on_det_ans_x_h2, "OFF") == 0))) 
	{
		// �����[�g�i�I���j�^ �����[�g�i��I���j
		m_RFPWROnBtn.SetColor(COLOR_REMOTES, COLOR_REMOTE, TRUE);
	}
	else 
	{
		// �����[�g�i�I���j
		m_RFPWROnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ���{�^���̐ݒ�
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&  //�ǉ�  2017/05/08  as-04
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&  //�ǉ�  2017/05/08  as-04
		(strcmp(m_sz_rf_on_enbl_x, "OFF") == 0) &&
		(strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") == 0) &&  //�ǉ�  2017/05/08  as-04
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") == 0) &&  //�ǉ�  2017/05/08  as-04
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_RFPWROffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_RFPWROffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RFPWROffBtn.Invalidate();
	m_RFPWROnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlX

-# RF-POWER ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_rf_power_on34()
{
	char	szRfOnEnblSts[32];
	char	szRfOnDetAnsSts[32];

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	// HPA����RF���
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

	// HPA/SSPA�ŏ����𕪂���
	if (strcmp(m_sz_xpa_sel, "HPA") == 0)
	{
		// HPA���I������Ă���ꍇ
		strcpy_s(szRfOnEnblSts, sizeof(szRfOnEnblSts), m_sz_hpa_rf_on_enbl_x);
		strcpy_s(szRfOnDetAnsSts, sizeof(szRfOnDetAnsSts), m_sz_hpa_rf_on_det_ans_x);
	}
	else
	{
		// SSPA���I������Ă���ꍇ
		strcpy_s(szRfOnEnblSts, sizeof(szRfOnEnblSts), m_sz_rf_on_enbl_x);
		strcpy_s(szRfOnDetAnsSts, sizeof(szRfOnDetAnsSts), m_sz_rf_on_det_ans_x);
	}

	// �Δ�������
	if (strcmp(m_sz_lan_x_tx, "�ڑ�") != 0)
	{
		// ���[�J��
		m_RFPWROnBtn.SetColor(COLOR_LOCAL);
	}
	else if (((strcmp(m_sz_x_tx_rl, "LOCAL") == 0) || (strcmp(szRfOnEnblSts, "OFF") == 0)) && (strcmp(szRfOnDetAnsSts, "OFF") == 0))
	{
		// �����[�g�i�I���j�^ ���[�J��
		m_RFPWROnBtn.SetColor(COLOR_REMOTES, COLOR_LOCAL, TRUE);
	}
	else if (strcmp(szRfOnDetAnsSts, "OFF") == 0)
	{
		// �����[�g�i�I���j�^ �����[�g�i��I���j
		m_RFPWROnBtn.SetColor(COLOR_REMOTES, COLOR_REMOTE, TRUE);
	}
	else
	{
		// �����[�g�i�I���j
		m_RFPWROnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ���{�^���̐ݒ�
	if ((strcmp(m_sz_x_tx_rl, "REMOTE") == 0) &&
		(strcmp(szRfOnEnblSts, "OFF") == 0) &&
		(strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_RFPWROffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_RFPWROffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RFPWROffBtn.Invalidate();
	m_RFPWROnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlX

-# NO-USE OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_no_use_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	// OFF ��
	if ((strcmp(m_sz_lan_x_tx, "�ڑ�") != 0)&&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") != 0))
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
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&  //�ǉ�  2017/05/08  as-04
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&  //�ǉ�  2017/05/08  as-04
		(strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") == 0) &&  //�ǉ�  2017/05/08  as-04
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") == 0) &&  //�ǉ�  2017/05/08  as-04
		(m_i_priv_flag == 1))
	{
		// �����[�g��
		m_NoUseOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J����
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_NoUseOffBtn.Invalidate();
	m_NoUseOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# NO-USE OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_no_use_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_x_tx, "�ڑ�") != 0)
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
	if ((strcmp(m_sz_x_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g��
		m_NoUseOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J����
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_NoUseOffBtn.Invalidate();
	m_NoUseOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# NO-USE ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_no_use_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	// OF ��
	if ((strcmp(m_sz_lan_x_tx, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") != 0) &&   //�ǉ�  2017/05/08  as-04
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") != 0))
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
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&  //�ǉ�  2017/05/08  as-04
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&  //�ǉ�  2017/05/08  as-04
		(strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") == 0) &&  //�ǉ�  2017/05/08  as-04
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") == 0) &&  //�ǉ�  2017/05/08  as-04
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
/*! CTrsControlX

-# NO-USE ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_no_use_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_x_tx, "�ڑ�") != 0)
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
	if ((strcmp(m_sz_x_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) &&
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
/*! CTrsControlX

-# SWEEP-CONT STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_stop64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SweepCountStopBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_SweepCountIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_stop34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountStopBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT INC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_inc64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SweepCountIncBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_SweepCountStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT INC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_inc34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountIncBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT INCDEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_incdec64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT INCDEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_incdec34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT DEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_dec64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SweepCountDecBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT DEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_dec34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountDecBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT DECINC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_decinc64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# SWEEP-CONT DECINC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_sweep_cont_decinc34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepCountDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountDecBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountDecBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountStopBtn.Invalidate();
	m_SweepCountIncBtn.Invalidate();
	m_SweepCountIncDecBtn.Invalidate();
	m_SweepCountDecBtn.Invalidate();
	m_SweepCountDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RAMP-CONT OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_ramp_cont_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_RampContOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_RampContOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_RampContOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_RampContOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RampContOffBtn.Invalidate();
	m_RampContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RAMP-CONT OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_ramp_cont_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_RampContOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_RampContOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_RampContOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_RampContOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RampContOffBtn.Invalidate();
	m_RampContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RAMP-CONT ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_ramp_cont_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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
	strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_RampContOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_RampContOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_RampContOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_RampContOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RampContOffBtn.Invalidate();
	m_RampContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RAMP-CONT ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_ramp_cont_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_RampContOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_RampContOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_x_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_RampContOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_RampContOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RampContOffBtn.Invalidate();
	m_RampContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# CMD-MOD OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_cmd_mod_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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
	strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), "�ڑ�");
#endif

	// OFF ��
	if (strcmp(m_sz_lan_xx_tcrmod, "�ڑ�") != 0)
	{
		// ���[�J��
		m_CmdModOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_CmdModOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_xx_tcrmod_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_xx_tcrmod, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_CmdModOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_CmdModOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# CMD-MOD OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_cmd_mod_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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
	strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_x_tx, "�ڑ�") != 0)
	{
		// ���[�J��
		m_CmdModOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_CmdModOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_x_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_CmdModOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_CmdModOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# CMD-MOD ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_cmd_mod_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	// ON ��
	if (strcmp(m_sz_lan_xx_tcrmod, "�ڑ�") != 0)
	{
		// ���[�J��
		m_CmdModOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_CmdModOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_xx_tcrmod_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_xx_tcrmod, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_CmdModOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_CmdModOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# CMD-MOD ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_cmd_mod_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_x_tx, "�ڑ�") != 0)
	{
		// ���[�J��
		m_CmdModOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_CmdModOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_x_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_CmdModOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_CmdModOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNG-MOD OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_rng_mod_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_xx_tcrmod, "�ڑ�") != 0)
	{
		// ���[�J��
		m_RngModOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i��I���j
		m_RngModOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_xx_tcrmod_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_xx_tcrmod, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_RngModOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_RngModOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RngModOffBtn.Invalidate();
	m_RngModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNG-MOD OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_rng_mod_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_x_tx, "�ڑ�") != 0)
	{
		// ���[�J��
		m_RngModOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i��I���j
		m_RngModOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_x_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_RngModOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_RngModOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RngModOffBtn.Invalidate();
	m_RngModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNG-MOD ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_rng_mod_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_xx_tcrmod, "�ڑ�") != 0)
	{
		// ���[�J��
		m_RngModOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_RngModOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_xx_tcrmod_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_xx_tcrmod, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_RngModOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_RngModOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RngModOffBtn.Invalidate();
	m_RngModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNG-MOD ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_rng_mod_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_x_tx, "�ڑ�") != 0)
	{
		// ���[�J��
		m_RngModOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_RngModOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_x_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_RngModOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_RngModOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RngModOffBtn.Invalidate();
	m_RngModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# MODULATION OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_modulation_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	// OFF ��
	if (strcmp(m_sz_lan_xx_tcrmod, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_ModulationOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_ModulationOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_xx_tcrmod_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_xx_tcrmod, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_ModulationOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_ModulationOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# MODULATION OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_modulation_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_xx_tcrmod, "�ڑ�") != 0)
	{
		// ���[�J��
		m_ModulationOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_ModulationOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_xx_tcrmod_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_xx_tcrmod, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_ModulationOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_ModulationOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# MODULATION ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_modulation_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	// ON ��
	if (strcmp(m_sz_lan_xx_tcrmod, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_ModulationOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_ModulationOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_xx_tcrmod_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_xx_tcrmod, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_ModulationOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_ModulationOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# MODULATION ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_modulation_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_xx_tcrmod, "�ڑ�") != 0)
	{
		// ���[�J��
		m_ModulationOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_ModulationOnBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_xx_tcrmod_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_xx_tcrmod, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_ModulationOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_ModulationOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# HOLD-TONE OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_hold_tone_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_CMDOUT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_cmdout, sizeof(m_sz_lan_x_cmdout), obs_adr->sz_sts_name);
	}

	// OFF ��
	if (strcmp(m_sz_lan_x_cmdout, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_HoldToneOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_HoldToneOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_lan_x_cmdout, "�ڑ�") == 0) && (m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_HoldToneOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_HoldToneOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# HOLD-TONE OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_hold_tone_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_CMDOUT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_cmdout, sizeof(m_sz_lan_x_cmdout), obs_adr->sz_sts_name);
	}

	// OFF ��
	if (strcmp(m_sz_lan_x_cmdout, "�ڑ�") != 0)
	{
		// ���[�J��
		m_HoldToneOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_HoldToneOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_lan_x_cmdout, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_HoldToneOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_HoldToneOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# HOLD-TONE ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_hold_tone_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_X_CMDOUT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_cmdout, sizeof(m_sz_lan_x_cmdout), obs_adr->sz_sts_name);
	}

	// ON ��
	if (strcmp(m_sz_lan_x_cmdout, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_HoldToneOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_HoldToneOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_lan_x_cmdout, "�ڑ�") == 0) && (m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_HoldToneOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_HoldToneOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# HOLD-TONE ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_hold_tone_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_X_CMDOUT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_cmdout, sizeof(m_sz_lan_x_cmdout), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_cmdout, "�ڑ�") != 0)
	{
		// ���[�J��
		m_HoldToneOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_HoldToneOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_lan_x_cmdout, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_HoldToneOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_HoldToneOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# COH-INCOH COH ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_coh_incoh_coh64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_vequ, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_XXCohCohBtn.SetColor(COLOR_LOCAL);
		m_XXCohIncohBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		if (m_i_priv_flag == 1) 
		{
			// �����[�g�i��I���j
			m_XXCohIncohBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ���[�J��
			m_XXCohIncohBtn.SetColor(COLOR_LOCAL);
		}

		// �����[�g�i�I���j
		m_XXCohCohBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_XXCohCohBtn.Invalidate();
	m_XXCohIncohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# COH-INCOH COH ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_coh_incoh_coh34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_vequ, "�ڑ�") != 0)
	{
		// ���[�J��
		m_XXCohCohBtn.SetColor(COLOR_LOCAL);
		m_XXCohIncohBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		if (m_i_priv_flag == 1)
		{
			// �����[�g�i��I���j
			m_XXCohIncohBtn.SetColor(COLOR_REMOTE);
		}
		else
		{
			// ���[�J��
			m_XXCohIncohBtn.SetColor(COLOR_LOCAL);
		}

		// �����[�g�i�I���j
		m_XXCohCohBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_XXCohCohBtn.Invalidate();
	m_XXCohIncohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# COH-INCOH INCOH ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_coh_incoh_incoh64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_vequ, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_XXCohIncohBtn.SetColor(COLOR_LOCAL);
		m_XXCohCohBtn.SetColor(COLOR_LOCAL);
	}
	else {
		if (m_i_priv_flag == 1) 
		{
			// �����[�g�i��I���j
			m_XXCohCohBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ���[�J��
			m_XXCohCohBtn.SetColor(COLOR_LOCAL);
		}

		// �����[�g�i�I���j
		m_XXCohIncohBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_XXCohCohBtn.Invalidate();
	m_XXCohIncohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# COH-INCOH INCOH ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_coh_incoh_incoh34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_X34_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_vequ, "�ڑ�") != 0)
	{
		// ���[�J��
		m_XXCohIncohBtn.SetColor(COLOR_LOCAL);
		m_XXCohCohBtn.SetColor(COLOR_LOCAL);
	}
	else {
		if (m_i_priv_flag == 1)
		{
			// �����[�g�i��I���j
			m_XXCohCohBtn.SetColor(COLOR_REMOTE);
		}
		else
		{
			// ���[�J��
			m_XXCohCohBtn.SetColor(COLOR_LOCAL);
		}

		// �����[�g�i�I���j
		m_XXCohIncohBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_XXCohCohBtn.Invalidate();
	m_XXCohIncohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNGING STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_rnging_stop64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	// STOP ��
	if (strcmp(m_sz_lan_xx_rarr, "�ڑ�") != 0)
	{
		// ���[�J��
		m_RngingStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_RngingStopBtn.SetColor(COLOR_REMOTES);
	}

	// START ��
	// XX-RARR LAN�ڑ����͗΂ɂ���
	if ((strcmp(m_sz_lan_xx_rarr, "�ڑ�") == 0) &&
		(strcmp(m_sz_xx_rarr_cont, "REMOTE") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_RngingStartBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}

	// �C���Z���V�e�B�u�̏ꍇ�́A�Z���V�e�B�u�ɂ���
	if (m_RngingStartBtn.IsWindowEnabled() == FALSE)
	{
		m_RngingStartBtn.EnableWindow(TRUE);
		m_RngingStopBtn.EnableWindow(TRUE);
	}

	// �X�V
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNGING STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_rnging_stop34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_xx_rarr, "�ڑ�") != 0)
	{
		// ���[�J��
		m_RngingStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_RngingStopBtn.SetColor(COLOR_REMOTES);
	}

	// START ��
	// XX-RARR LAN�ڑ����͗΂ɂ���
	if ((strcmp(m_sz_lan_xx_rarr, "�ڑ�") == 0) &&
		(strcmp(m_sz_pass_flag, "�p�X��") == 0) &&
		(strcmp(m_sz_xx_rarr_cont, "REMOTE") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_RngingStartBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}

	// �C���Z���V�e�B�u�̏ꍇ�́A�Z���V�e�B�u�ɂ���
	if ((m_RngingStartBtn.IsWindowEnabled() == FALSE) && (m_l_rng_type != RNG_TYPE_REP))
	{
		m_RngingStartBtn.EnableWindow(TRUE);
		m_RngingStopBtn.EnableWindow(TRUE);
	}

	// �X�V
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNGING START ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_rnging_start64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	// �]���^���I�΂�Ă��Ȃ���΁A�{�^�����C���Z���V�e�B�u�ɂ���
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

	if ((strcmp(m_sz_lan_xx_rarr, "�ڑ�") != 0) ||
		(strcmp(m_sz_norm_rep, "NORMAL") != 0))
	{
		// ���[�J��
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_RngingStartBtn.SetColor(COLOR_REMOTES);
	}

	// STOP ��
	// XX-RARR LAN�ڑ���
	if ((strcmp(m_sz_lan_xx_rarr, "�ڑ�") == 0) &&
		(strcmp(m_sz_xx_rarr_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_norm_rep, "NORMAL") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_RngingStopBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_RngingStopBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNGING START ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_rnging_start34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	// �]���^���I�΂�Ă��Ȃ���΁A�{�^�����C���Z���V�e�B�u�ɂ���
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

	if ((strcmp(m_sz_lan_xx_rarr, "�ڑ�") != 0) ||
		(strcmp(m_sz_norm_rep, "NORMAL") != 0))
	{
		// ���[�J��
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_RngingStartBtn.SetColor(COLOR_REMOTES);
	}

	// STOP ��
	// XX-RARR LAN�ڑ���
	if ((strcmp(m_sz_lan_xx_rarr, "�ڑ�") == 0) &&
		(strcmp(m_sz_xx_rarr_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_norm_rep, "NORMAL") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_RngingStopBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_RngingStopBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNGING �Đ��^ STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_rrng_stop64()
{
	struct satinf_t *satinf_adr;
	struct satinf_t *original_adr;
	char *sattbl_adr;
	struct rarrband_t *rarrband_t_adr;	// �q�`�q�q�g�p�ш���e�[�u���A�h���X

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_xx_rrng, "�ڑ�") != 0)
	{
		// ���[�J��
		m_RRngStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_RRngStopBtn.SetColor(COLOR_REMOTES);
	}

	// START ��
	// XX-RARR LAN�ڑ���
	if ((strcmp(m_sz_lan_xx_rrng, "�ڑ�") == 0) &&
		(strcmp(m_sz_xx_rrng_cont, "REMOTE") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_RRngStartBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_RRngStartBtn.SetColor(COLOR_LOCAL);
	}

	// �p�X���ƃp�X�O�ŏ����𕪂���
	if (strcmp(m_sz_pass_flag, MON_STR_PASS) != 0)
	{
		// �p�X�O�̏ꍇ
		if (m_RRngStartBtn.IsWindowEnabled() == FALSE)
		{
			// �C���Z���V�e�B�u�Ȃ�Z���V�e�B�u�ɂ���
			m_RRngStartBtn.EnableWindow(TRUE);
			m_RRngStopBtn.EnableWindow(TRUE);
		}
	}
	else 
	{
		// �p�X���̏ꍇ
		// �p�X���̏ꍇ�́A�^�p���̉q�����Đ������ɑΉ����Ă���ꍇ�̂݃Z���V�e�B�u�ɂ���

		// ���o�����q�����ŉq���ŗL�c�a����

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
				satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_t�ɕϊ�*/
				if (satinf_adr->l_id == ENDOFSAT_ID)
				{
					/*���ʂh�c���I���̏ꍇ*/
					break;
				}

				// �w�q�`�q�q�g�p�ш���
				if (satinf_adr->l_id == RARRBAND_ID)
				{
					rarrband_t_adr = (struct rarrband_t *)satinf_adr;
					if (rarrband_t_adr->l_rarr_band == RARRBAND_RX_ID)
					{
						// �C���Z���V�e�B�u�̏ꍇ�̓Z���V�e�B�u�ɂ���
						if (m_RRngStartBtn.IsWindowEnabled() == FALSE)
						{
							m_RRngStartBtn.EnableWindow(TRUE);
							m_RRngStopBtn.EnableWindow(TRUE);
						}
					}
					else
					{
						// �Z���V�e�B�u�̏ꍇ�̓C���Z���V�e�B�u�ɂ���
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

	// �X�V
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNGING �Đ��^ STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_rrng_stop34()
{
	struct satinf_t *satinf_adr;
	struct satinf_t *original_adr;
	char *sattbl_adr;
	struct rarrband_t *rarrband_t_adr;	// �q�`�q�q�g�p�ш���e�[�u���A�h���X

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_xx_rrng, "�ڑ�") != 0)
	{
		// ���[�J��
		m_RRngStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_RRngStopBtn.SetColor(COLOR_REMOTES);
	}

	// START ��
	// XX-RARR LAN�ڑ���
	if ((strcmp(m_sz_lan_xx_rrng, "�ڑ�") == 0) &&
		(strcmp(m_sz_pass_flag, "�p�X��") == 0) &&
		(strcmp(m_sz_xx_rrng_cont, "REMOTE") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_RRngStartBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_RRngStartBtn.SetColor(COLOR_LOCAL);
	}

	// �p�X���ƃp�X�O�ŏ����𕪂���
	if (strcmp(m_sz_pass_flag, MON_STR_PASS) != 0)
	{
		// �p�X�O�̏ꍇ
		if ((m_RRngStartBtn.IsWindowEnabled() == FALSE) && (m_l_rng_type != RNG_TYPE_NOR))
		{
			// �C���Z���V�e�B�u�Ȃ�Z���V�e�B�u�ɂ���
			m_RRngStartBtn.EnableWindow(TRUE);
			m_RRngStopBtn.EnableWindow(TRUE);
		}
	}
	else
	{
		// �p�X���̏ꍇ
		// �p�X���̏ꍇ�́A�^�p���̉q�����Đ������ɑΉ����Ă���ꍇ�̂݃Z���V�e�B�u�ɂ���

		// ���o�����q�����ŉq���ŗL�c�a����

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
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_t�ɕϊ�*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*���ʂh�c���I���̏ꍇ*/
				break;
			}

			// �w�q�`�q�q�g�p�ш���
			if (satinf_adr->l_id == RARRBAND_ID)
			{
				rarrband_t_adr = (struct rarrband_t *)satinf_adr;
				if (rarrband_t_adr->l_rarr_band == RARRBAND_RX_ID)
				{
					// �C���Z���V�e�B�u�̏ꍇ�̓Z���V�e�B�u�ɂ���
					if ((m_RRngStartBtn.IsWindowEnabled() == FALSE) && (m_l_rng_type != RNG_TYPE_NOR))
					{
						m_RRngStartBtn.EnableWindow(TRUE);
						m_RRngStopBtn.EnableWindow(TRUE);
					}
				}
				else
				{
					// �Z���V�e�B�u�̏ꍇ�̓C���Z���V�e�B�u�ɂ���
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

	// �X�V
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNGING �Đ��^ START ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_rrng_start64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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
	strcpy_s(m_sz_lan_xx_rrng, sizeof(m_sz_lan_xx_rrng), "�ڑ�");
	strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), "REPLAY");
	strcpy_s(m_sz_xx_rrng_cont, sizeof(m_sz_xx_rrng_cont), "REMOTE");
#endif

	// �Đ��^���I�΂�Ă��Ȃ���΁A�{�^�����C���Z���V�e�B�u�ɂ���
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

	if ((strcmp(m_sz_lan_xx_rrng, "�ڑ�") != 0) ||
		(strcmp(m_sz_norm_rep, "REPLAY") != 0))
	{
		// ���[�J��
		m_RRngStartBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_RRngStartBtn.SetColor(COLOR_REMOTES);
	}

	// STOP ��
	// XX-RRNG LAN�ڑ���
	if ((strcmp(m_sz_lan_xx_rrng, "�ڑ�") == 0) &&
		(strcmp(m_sz_xx_rrng_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_norm_rep, "REPLAY") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_RRngStopBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_RRngStopBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlX

-# RNGING �Đ��^ START ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlX::show_rrng_start34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	// �Đ��^���I�΂�Ă��Ȃ���΁A�{�^�����C���Z���V�e�B�u�ɂ���
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

	if ((strcmp(m_sz_lan_xx_rrng, "�ڑ�") != 0) ||
		(strcmp(m_sz_norm_rep, "REPLAY") != 0))
	{
		// ���[�J��
		m_RRngStartBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_RRngStartBtn.SetColor(COLOR_REMOTES);
	}

	// STOP ��
	// XX-RRNG LAN�ڑ���
	if ((strcmp(m_sz_lan_xx_rrng, "�ڑ�") == 0) &&
		(strcmp(m_sz_xx_rrng_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_norm_rep, "REPLAY") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_RRngStopBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_RRngStopBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


void CTrsControlX::OnClose()
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

void CTrsControlX::CloseDlg()
{
	KillTimer(TID_CONDITION);
	KillTimer(TID_COUNTDOWN);

	// �E�B���h�E����郁�b�Z�[�W
	theApp.m_pMainWnd->PostMessageW(eMessage_WindowClose, 0, (LPARAM)this);
}
