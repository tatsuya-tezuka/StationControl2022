// TrsControlS64.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrsControlS64.h"
#include "afxdialogex.h"
#include "TrsControlMng.h"
#include "HookMsgBox.h"


// CTrsControlS64 �_�C�A���O

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


// CTrsControlS64 ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! COperationPlan

-# �t�@�C��-���郁�j���[����

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnMenuClose()
{
	// �����ꂩ�̃{�^�������s���̏ꍇ
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
/*! CTrsControlS64

-# �_�C�A���O����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CTrsControlS64::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// RF-POWER
	m_RfPwrOffBtn.SetColor(COLOR_REMOTES);
	m_RfPwrOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_RfPwrOnBtn.SetColor(COLOR_REMOTE);
	m_RfPwrOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// NO-USER
	m_NoUseOffBtn.SetColor(COLOR_REMOTES);
	m_NoUseOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_NoUseOnBtn.SetColor(COLOR_REMOTES);
	m_NoUseOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// SWEEP-CONT(NORMAL)
	m_SweepContNStopBtn.SetColor(COLOR_REMOTES);
	m_SweepContNStopBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepContNIncBtn.SetColor(COLOR_REMOTE);
	m_SweepContNIncBtn.SetCtrlButton(TRUE);			// Ctrl�L�[ +
	m_SweepContNIncDecBtn.SetColor(COLOR_REMOTE);
	m_SweepContNIncDecBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepContNDecBtn.SetColor(COLOR_REMOTE);
	m_SweepContNDecBtn.SetCtrlButton(TRUE);			// Ctrl�L�[ +
	m_SweepContNDecIncBtn.SetColor(COLOR_REMOTE);
	m_SweepContNDecIncBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// SWEEP-CONT(TRIANGLE)
	m_SweepContTStopBtn.SetColor(COLOR_REMOTES);
	m_SweepContTStopBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepContTIncBtn.SetColor(COLOR_REMOTE);
	m_SweepContTIncBtn.SetCtrlButton(TRUE);			// Ctrl�L�[ +
	m_SweepContTIncDecBtn.SetColor(COLOR_REMOTE);
	m_SweepContTIncDecBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepContTDecBtn.SetColor(COLOR_REMOTE);
	m_SweepContTDecBtn.SetCtrlButton(TRUE);			// Ctrl�L�[ +
	m_SweepContTDecIncBtn.SetColor(COLOR_REMOTE);
	m_SweepContTDecIncBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// SWEEP-NUM
	m_SweepNumBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_SweepNumBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// MOD
	m_ModOffBtn.SetColor(COLOR_REMOTES);
	m_ModOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_ModOnBtn.SetColor(COLOR_REMOTE);
	m_ModOnBtn.SetCtrlButton(TRUE);			// Ctrl�L�[ +

	// SS-RNG
	m_SSRngOffBtn.SetColor(COLOR_REMOTES);
	m_SSRngOffBtn.SetCtrlButton(TRUE);			// Ctrl�L�[ +
	m_SSRngOnBtn.SetColor(COLOR_REMOTE);
	m_SSRngOnBtn.SetCtrlButton(TRUE);			// Ctrl�L�[ +

	// COH/INCOH
	m_CohIncohCohBtn.SetColor(COLOR_REMOTES);
	m_CohIncohCohBtn.SetCtrlButton(TRUE);			// Ctrl�L�[ +
	m_CohIncohInCohBtn.SetColor(COLOR_REMOTES);
	m_CohIncohInCohBtn.SetCtrlButton(TRUE);			// Ctrl�L�[ +

	// RNGING
	m_RngingStopBtn.SetColor(COLOR_REMOTES);
	m_RngingStopBtn.SetCtrlButton(TRUE);			// Ctrl�L�[ +
	m_RngingStartBtn.SetColor(COLOR_REMOTE);
	m_RngingStartBtn.SetCtrlButton(TRUE);			// Ctrl�L�[ +

	// HOLD-TONE
	m_HoldToneOffBtn.SetColor(COLOR_REMOTES);
	m_HoldToneOffBtn.SetCtrlButton(TRUE);			// Ctrl�L�[ +
	m_HoldToneOnBtn.SetColor(COLOR_REMOTE);
	m_HoldToneOnBtn.SetCtrlButton(TRUE);			// Ctrl�L�[ +

	// INDEX-CMD
	m_IndexCmdBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_IndexCmdBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// INDEX-RNG
	m_IndexRngBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_IndexRngBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// INDEX-C+R
	m_IndexCRBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_IndexCRBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// �^�C���A�E�g�J�E���g�_�E���l�����ݒ�
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

	// �O���[�v�{�b�N�X
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


	// �z�X�g���擾
	memset(m_sz_hostname, (char)NULL, sizeof(m_sz_hostname));
	gethostname(m_sz_hostname, sizeof(m_sz_hostname));

	// SWEEP-CONT TRIANGLE�`�F�b�N�{�^��
	m_bSweepContTriangle = FALSE;
	if (m_bSweepContTriangle)
	{
		// �{�^������
		m_SweepContTStopBtn.EnableWindow(TRUE);
		m_SweepContTIncBtn.EnableWindow(TRUE);
		m_SweepContTIncDecBtn.EnableWindow(TRUE);
		m_SweepContTDecBtn.EnableWindow(TRUE);
		m_SweepContTDecIncBtn.EnableWindow(TRUE);
	}
	else
	{
		// �{�^���񊈐�
		m_SweepContTStopBtn.EnableWindow(FALSE);
		m_SweepContTIncBtn.EnableWindow(FALSE);
		m_SweepContTIncDecBtn.EnableWindow(FALSE);
		m_SweepContTDecBtn.EnableWindow(FALSE);
		m_SweepContTDecIncBtn.EnableWindow(FALSE);
	}

#if 0
	// ���M�����ʏ�ԕ\���^�C���A�E�g�R�[���o�b�N���[�`���N��
	show_opsc_condition();

	// �J�E���g�_�E���^�C���A�E�g�R�[���o�b�N���[�`���N��
	count_down();
#endif

	CenterWindowEx();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CTrsControlS64

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CTrsControlS64::UpdateDlg()
{
	// ���M�����ʏ�ԕ\���^�C���A�E�g�R�[���o�b�N���[�`���N��
	show_opsc_condition();

	// �J�E���g�_�E���^�C���A�E�g�R�[���o�b�N���[�`���N��
	count_down();

	return TRUE;
}


/*============================================================================*/
/*! CTrsControlS64

-# �wTX CONTROL�x-�wRF-POWER�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlRfOffBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_RfPwrOffBtn.SetColor(COLOR_CTRLIN);
	m_RfPwrOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_RF_OUTPUT_CONT"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_rf_power_off_cntdwn = RF_POWER_TIME_S64_LIMIT;

	// �X�V
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wTX CONTROL�x-�wRF-POWER�x-�wON�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlRfOnBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_RfPwrOnBtn.SetColor(COLOR_CTRLIN);
	m_RfPwrOffBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_RF_OUTPUT_CONT"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_rf_power_on_cntdwn = RF_POWER_TIME_S64_LIMIT;

	// �X�V
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# �wTX CONTROL�x-�wNO-USE�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlNouseOffBtn()
{
	COLORREF col = m_NoUseOffBtn.GetColor();

	if (col == COLOR_REMOTE)	// �����[�g�i��I���j
	{
		// �{�^���𐧌䒆�F�ɂ���
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_NO_USE"), _T("OFF"));

		// �J�E���g�_�E���J�n
		m_l_no_use_off_cntdwn = NO_USE_TIME_S64_LIMIT;
	}
	else if (col == COLOR_CTRLIN)	// ���䒆
	{
		// �{�^���𐧌䒆�F�ɂ���
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_NO_USE"), _T("OFF"));

		// �J�E���g�_�E���J�n
		m_l_no_use_off_cntdwn = NO_USE_TIME_S64_LIMIT;

		// �����̃J�E���g�_�E���I��
		m_l_no_use_on_cntdwn = -1;
	}
	else if (col == COLOR_REMOTES)	// �����[�g�i�I���j
	{
		// �{�^���𐧌䒆�F�ɂ���
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_NO_USE"), _T("OFF"));

		// �J�E���g�_�E���J�n
		m_l_no_use_off_cntdwn = NO_USE_TIME_S64_LIMIT;

		// �����̃J�E���g�_�E���I��
		m_l_no_use_on_cntdwn = -1;
	}
	else
	{
		;	// noting.
	}

	// �X�V
	m_NoUseOffBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wTX CONTROL�x-�wNO-USE�x-�wON�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlNouseOnBtn()
{
	COLORREF col = m_NoUseOnBtn.GetColor();

	if (col == COLOR_REMOTE)	// �����[�g�i��I���j
	{
		// �{�^���𐧌䒆�F�ɂ���
		m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_NO_USE"), _T("ON"));

		// �J�E���g�_�E���J�n
		m_l_no_use_on_cntdwn = NO_USE_TIME_S64_LIMIT;
	}
	else if (col == COLOR_CTRLIN)	// ���䒆
	{
		// �{�^���𐧌䒆�F�ɂ���
		m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("S-TXRF.IF_NO_USE"), _T("ON"));

		// �J�E���g�_�E���J�n
		m_l_no_use_on_cntdwn = NO_USE_TIME_S64_LIMIT;

		// �����̃J�E���g�_�E���I��
		m_l_no_use_off_cntdwn = -1;
	}
	else
	{
		;	// noting.
	}

	// �X�V
	m_NoUseOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wTX CONTROL�x-�wSWEEP-COUNT(NORMAL)�x-�wSTOP�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweepnStopBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_SweepContNStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.SWEEP_CONT"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_nor_sweep_cont_stop_cntdwn = SWEEP_CONT_TIME_S64_LIMIT;

	// �X�V
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wTX CONTROL�x-�wSWEEP-COUNT(NORMAL)�x-�wINC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweepnIncBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_SweepContNStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.SWEEP_CONT"), _T("INC"));

	// �J�E���g�_�E���J�n
	m_l_nor_sweep_cont_inc_cntdwn = SWEEP_CONT_TIME_S64_LIMIT;

	// �X�V
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wTX CONTROL�x-�wSWEEP-COUNT(NORMAL)�x-�wINCDEC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweepnIncdecBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_SweepContNStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.SWEEP_CONT"), _T("INC+DEC"));

	// �J�E���g�_�E���J�n
	m_l_nor_sweep_cont_incdec_cntdwn = SWEEP_CONT_TIME_S64_LIMIT;

	// �X�V
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wTX CONTROL�x-�wSWEEP-COUNT(NORMAL)�x-�wDEC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweepnDecBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_SweepContNStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.SWEEP_CONT"), _T("DEC"));

	// �J�E���g�_�E���J�n
	m_l_nor_sweep_cont_dec_cntdwn = SWEEP_CONT_TIME_S64_LIMIT;

	// �X�V
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wTX CONTROL�x-�wSWEEP-COUNT(NORMAL)�x-�wDECINC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweepnDecincBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_SweepContNStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContNDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.SWEEP_CONT"), _T("DEC+INC"));

	// �J�E���g�_�E���J�n
	m_l_nor_sweep_cont_decinc_cntdwn = SWEEP_CONT_TIME_S64_LIMIT;

	// �X�V
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT (TRIANGLE)�`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweeptCheck()
{
	UpdateData();

	if (m_bSweepContTriangle)
	{
		// �{�^������
		m_SweepContTStopBtn.EnableWindow(TRUE);
		m_SweepContTIncBtn.EnableWindow(TRUE);
		m_SweepContTIncDecBtn.EnableWindow(TRUE);
		m_SweepContTDecBtn.EnableWindow(TRUE);
		m_SweepContTDecIncBtn.EnableWindow(TRUE);
	}
	else
	{
		// �{�^���񊈐�
		m_SweepContTStopBtn.EnableWindow(FALSE);
		m_SweepContTIncBtn.EnableWindow(FALSE);
		m_SweepContTIncDecBtn.EnableWindow(FALSE);
		m_SweepContTDecBtn.EnableWindow(FALSE);
		m_SweepContTDecIncBtn.EnableWindow(FALSE);
	}
}


/*============================================================================*/
/*! CTrsControlS64

-# �wTX CONTROL�x-�wSWEEP-COUNT(TRIANGLE)�x-�wSTOP�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweeptStopBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_SweepContTStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.TR_SWEEP_CONT"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_tri_sweep_cont_stop_cntdwn = TR_SWEEP_CONT_TIME_S64_LIMIT;

	// �X�V
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wTX CONTROL�x-�wSWEEP-COUNT(TRIANGLE)�x-�wINC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweeptIncBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_SweepContTStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.TR_SWEEP_CONT"), _T("INC"));

	// �J�E���g�_�E���J�n
	m_l_tri_sweep_cont_inc_cntdwn = TR_SWEEP_CONT_TIME_S64_LIMIT;

	// �X�V
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wTX CONTROL�x-�wSWEEP-COUNT(TRIANGLE)�x-�wINCDEC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweeptIncdecBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_SweepContTStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.TR_SWEEP_CONT"), _T("INC+DEC"));

	// �J�E���g�_�E���J�n
	m_l_tri_sweep_cont_incdec_cntdwn = TR_SWEEP_CONT_TIME_S64_LIMIT;

	// �X�V
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wTX CONTROL�x-�wSWEEP-COUNT(TRIANGLE)�x-�wDEC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweeptDecBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_SweepContTStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.TR_SWEEP_CONT"), _T("DEC"));

	// �J�E���g�_�E���J�n
	m_l_tri_sweep_cont_dec_cntdwn = TR_SWEEP_CONT_TIME_S64_LIMIT;

	// �X�V
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wTX CONTROL�x-�wSWEEP-COUNT(TRIANGLE)�x-�wDECINC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweeptDecincBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_SweepContTStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContTDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("S-UCSYN.TR_SWEEP_CONT"), _T("DEC+INC"));

	// �J�E���g�_�E���J�n
	m_l_tri_sweep_cont_decinc_cntdwn = TR_SWEEP_CONT_TIME_S64_LIMIT;

	// �X�V
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wTX CONTROL�x-�wSWEEP-NUM�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSweepnumBtn()
{
	CString str1, str2;
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "�ڑ�");

	// �����[�g��
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1)) 
	{
		CString val;
		m_SweepNumBtn.GetWindowTextW(val);
		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("S-UCSYN.SWEEP_CYCLE"), _T(""), val);

		// �J�E���g�_�E���J�n
		m_l_sweep_num_cntdwn = TR_SWEEP_NUM_TIME_S64_LIMIT;
	}
}


/*============================================================================*/
/*! CTrsControlS64

-# �wTX CONTROL�x-�wMOD�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlModOffBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_ModOffBtn.SetColor(COLOR_CTRLIN);
	m_ModOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.MODULATION"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_cmd_mod_off_cntdwn = CMD_MOD_TIME_S64_LIMIT;

	// �X�V
	m_ModOffBtn.Invalidate();
	m_ModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wTX CONTROL�x-�wMOD�x-�wON�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlModOnBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_ModOffBtn.SetColor(COLOR_CTRLIN);
	m_ModOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.MODULATION"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_cmd_mod_on_cntdwn = CMD_MOD_TIME_S64_LIMIT;

	// �X�V
	m_ModOffBtn.Invalidate();
	m_ModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wRNG CONTROL�x-�wSS-RNG�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSsrngOffBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_SSRngOffBtn.SetColor(COLOR_CTRLIN);
	m_SSRngOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("V-EQU.RNG_SS"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_ss_rng_off_cntdwn = SS_RNG_TIME_S64_LIMIT;

	// �X�V
	m_SSRngOffBtn.Invalidate();
	m_SSRngOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wRNG CONTROL�x-�wSS-RNG�x-�wON�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlSsrngOnBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_SSRngOffBtn.SetColor(COLOR_CTRLIN);
	m_SSRngOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("V-EQU.RNG_SS"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_ss_rng_on_cntdwn = SS_RNG_TIME_S64_LIMIT;

	// �X�V
	m_SSRngOffBtn.Invalidate();
	m_SSRngOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wRNG CONTROL�x-�wCHO/INCOH�x-�wCOH�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlCohincohCohBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_CohIncohCohBtn.SetColor(COLOR_CTRLIN);
	m_CohIncohInCohBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("COHI"));

	// �J�E���g�_�E���J�n
	m_l_coh_incoh_coh_cntdwn = COH_INCOH_TIME_S64_LIMIT;

	// �X�V
	m_CohIncohCohBtn.Invalidate();
	m_CohIncohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wRNG CONTROL�x-�wCHO/INCOH�x-�wINCOH�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlCohincohIncohBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_CohIncohCohBtn.SetColor(COLOR_CTRLIN);
	m_CohIncohInCohBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("INCOHI"));

	// �J�E���g�_�E���J�n
	m_l_coh_incoh_incoh_cntdwn = COH_INCOH_TIME_S64_LIMIT;

	// �X�V
	m_CohIncohCohBtn.Invalidate();
	m_CohIncohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wRNG CONTROL�x-�wRNGING�x-�wSTOP�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlRngingStopBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING_CANCEL"), _T(","));

	// �J�E���g�_�E���J�n
	m_l_rnging_stop_cntdwn = RNGING_TIME_S64_LIMIT;

	// �X�V
	m_RngingStopBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wRNG CONTROL�x-�wRNGING�x-�wSTART�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlRngingStartBtn()
{
	CString str;

	// �{�^���𐧌䒆�F�ɂ���
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);
	m_RngingStartBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	str.Format(_T("S-%02d"), 99);
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str);

	// �J�E���g�_�E���J�n
	m_l_rnging_start_cntdwn = RNGING_TIME_S64_LIMIT;

	// �X�V
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wHOLD-TONE�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlHoldOffBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CMDOUT.HOLD_TONE"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_holdtone_off_cntdwn = HOLDTONE_TIME_S64_LIMIT;

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wHOLD-TONE�x-�wON�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlHoldOnBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CMDOUT.HOLD_TONE"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_holdtone_on_cntdwn = HOLDTONE_TIME_S64_LIMIT;

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# �wINDEX�x - �wCMD�x �{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlIdxCmdBtn()
{
	CString strCmd;

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "�ڑ�");
	}

	// �����[�g��
	if ((strcmp(m_sz_s_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) &&
		(strlen(m_sz_sat_name) > 0) && (m_i_priv_flag == 1)) 
	{
		// �ݔ��ڍא���v���Z�X(opctproc)�N��
		strCmd.Format(_T("CMD,%s"), (LPCTSTR)CString(m_sz_sat_name));
		theApp.GetSatelliteData().opctproc2(_T("CTRL.MOD_INDEX"), strCmd);
	}
}


/*============================================================================*/
/*! CTrsControlS64

-# �wINDEX�x - �wRNG�x �{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlIdxRngBtn()
{
	CString strCmd;

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "�ڑ�");
	}

	// �����[�g��
#if 1
	if ((strcmp(m_sz_s_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) &&
		(strlen(m_sz_sat_name) > 0) &&
		(m_i_priv_flag == 1))

#else
	if ((strcmp(m_sz_mod_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_ss_tcrmod, "�ڑ�") == 0) &&
		(strlen(m_sz_sat_name) > 0) &&
		(m_i_priv_flag == 1))
#endif
	{
		// �ݔ��ڍא���v���Z�X(opctproc)�N��
		strCmd.Format(_T("RNG,%s"), (LPCTSTR)CString(m_sz_sat_name));
		theApp.GetSatelliteData().opctproc2(_T("CTRL.MOD_INDEX"), strCmd);
	}
}


/*============================================================================*/
/*! CTrsControlS64

-# �wINDEX�x - �wC+R�x �{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnBnClickedTrsctrlIdxCrBtn()
{
	CString strCmd;

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "�ڑ�");
	}

	// �����[�g��
#if 1
	if ((strcmp(m_sz_s_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) &&
		(strlen(m_sz_sat_name) > 0) &&
		(m_i_priv_flag == 1))
#else
	if ((strcmp(m_sz_mod_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_ss_tcrmod, "�ڑ�") == 0) &&
		(strlen(m_sz_sat_name) > 0) &&
		(m_i_priv_flag == 1))
#endif
	{
		// �ݔ��ڍא���v���Z�X(opctproc)�N��
		strCmd.Format(_T("CMD+RNG,%s"), (LPCTSTR)CString(m_sz_sat_name));
		theApp.GetSatelliteData().opctproc2(_T("CTRL.MOD_INDEX"), strCmd);
	}
}

/*============================================================================*/
/*! CTrsControlS64

-# OnTimer

@param  nIDEvent:�^�C�}�[�̎��ʎq
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::OnTimer(UINT_PTR nIDEvent)
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
/*! CTrsControlS64

-# ���M�����ʏ�ԕ\���ݒ�

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_opsc_condition()
{
	TRACE(_T("show_opsc_condition\n"));

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	char sz_buff[64];

	KillTimer(TID_CONDITION);

	CSatelliteData& sd = theApp.GetSatelliteData();

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
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("���͌���"), _T(""), nLogEx::debug);

		memset(m_sz_priv_ssoc, (char)NULL, sizeof(m_sz_priv_ssoc));
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_PRIV_SSOC));
		if (obs_adr != NULL)
		{
			strcpy_s(m_sz_priv_ssoc, sizeof(m_sz_priv_ssoc), obs_adr->sz_sts_name);

			CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("CTRL.PRIV_SSOC"), CString(m_sz_priv_ssoc), nLogEx::debug);

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
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("�o���"), _T(""), nLogEx::debug);

		memset(m_sz_priv_sttn, (char)NULL, sizeof(m_sz_priv_sttn));
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_PRIV_STTN));
		if (obs_adr != NULL)
		{
			strcpy_s(m_sz_priv_sttn, sizeof(m_sz_priv_sttn), obs_adr->sz_sts_name);

			CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("CTRL.PRIV_STTN"), CString(m_sz_priv_sttn), nLogEx::debug);

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
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_RF_ON_ENBL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_enbl, sizeof(m_sz_rf_on_enbl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("S-TXRF.RF_ON_ENABLE"), CString(m_sz_rf_on_enbl), nLogEx::debug);
	}

	// RF POWER OFF ENABLE ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_RF_OFF_ENBL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_off_enbl, sizeof(m_sz_rf_off_enbl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("S-TXRF.RF_OFF_ENABLE"), CString(m_sz_rf_off_enbl), nLogEx::debug);
	}

	// UP-LINK CONTROL ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_UPLINK_CONT));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("CTRL.S_TX_TOTAL_REMOTE"), CString(m_sz_uplink_cont), nLogEx::debug);
	}

	// X-TX RF POWER ON DETECT ANSWER ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_X_TX_DET_ANS));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_tx_det_ans, sizeof(m_sz_x_tx_det_ans), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("X-TX.RF_ON_ANS"), CString(m_sz_x_tx_det_ans), nLogEx::debug);
	}

	/*** �����[�g�^���[�J�� ��Ԏ擾 ******************************************************/

	// S-TX �����[�g�^���[�J�� ���
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


	// S-TXSYN �����[�g�^���[�J�� ���
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

	/*** �A���[���`�F�b�N *****************************************************************/
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_ALARM));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_alarm, sizeof(m_sz_alarm), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("CTRL.ALARM_00_6"), CString(m_sz_alarm), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	strcpy_s(m_sz_alarm, sizeof(m_sz_alarm), "OFF");
#endif

	// �A���[����
	if (strcmp(m_sz_alarm, "ON") == 0) 
	{
		// �g��Ԃ�����
		m_grRfPower.SetBackColor(COLOR_ALARM);
		m_grNoUse.SetBackColor(COLOR_ALARM);
		m_grSweepContNormal.SetBackColor(COLOR_ALARM);
		m_grSweepContTriangle.SetBackColor(COLOR_ALARM);
		m_grMod.SetBackColor(COLOR_ALARM);
	}

	// �A���[������
	else 
	{
		// �g�̐F�����ɖ߂�
		m_grRfPower.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grNoUse.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepContNormal.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepContTriangle.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grMod.SetBackColor(GetSysColor(COLOR_3DFACE));
	}


	/*** RF-POWER *************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_RF_POWER));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_power, sizeof(m_sz_rf_power), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("S-TXRF.IF_RF_OUTPUT_CONT"), CString(m_sz_rf_power), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	// RF-POWER ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if (m_l_rf_power_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rf_power_on_cntdwn = -1;

		m_DebugRrPower = 1;
	}

	// RF-POWER OFF �J�E���g�_�E�����ɊĎ��f�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_rf_power_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
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
#else
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
#endif


	/*** NO-USE ***************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_NO_USE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use, sizeof(m_sz_no_use), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("S-TXRF.IF_NO_USE"), CString(m_sz_no_use), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	// NO-USE ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if (m_l_no_use_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_no_use_on_cntdwn = -1;

		m_DebugNoUse = 1;
	}

	// NO-USE OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_no_use_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
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
#else
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
#endif


	/*** SWEEP-CONT ***********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_SWEEP_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_nor_sweep_cont, sizeof(m_sz_nor_sweep_cont), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("S-UCSYN.SWEEP_CONT"), CString(m_sz_nor_sweep_cont), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	// SWEEP-CONT STOP �J�E���g�_�E�����ɊĎ��f�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_nor_sweep_cont_stop_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_nor_sweep_cont_stop_cntdwn = -1;

		m_DebugSweepContN = 1;
	}

	// SWEEP-CONT INC �J�E���g�_�E�����Ƀf�[�^�� INC �ɂȂ����ꍇ
	if (m_l_nor_sweep_cont_inc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_nor_sweep_cont_inc_cntdwn = -1;

		m_DebugSweepContN = 2;
	}

	// SWEEP-CONT INC+DEC �J�E���g�_�E�����Ƀf�[�^�� INC+DEC �ɂȂ����ꍇ
	if (m_l_nor_sweep_cont_incdec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_nor_sweep_cont_incdec_cntdwn = -1;

		m_DebugSweepContN = 3;
	}

	// SWEEP-CONT DEC �J�E���g�_�E�����Ƀf�[�^�� DEC �ɂȂ����ꍇ
	if (m_l_nor_sweep_cont_dec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_nor_sweep_cont_dec_cntdwn = -1;

		m_DebugSweepContN = 4;
	}

	// SWEEP-CONT DEC+INC �J�E���g�_�E�����Ƀf�[�^�� DEC �ɂȂ����ꍇ
	if (m_l_nor_sweep_cont_decinc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
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

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_nor_sweep_cont_stop_cntdwn == -1 && m_l_nor_sweep_cont_inc_cntdwn == -1 &&
		m_l_nor_sweep_cont_incdec_cntdwn == -1 && m_l_nor_sweep_cont_dec_cntdwn == -1 &&
		m_l_nor_sweep_cont_decinc_cntdwn == -1)
	{
		// INC �̏ꍇ
		if (strcmp(m_sz_nor_sweep_cont, "INC") == 0)
		{
			show_nor_sweep_cont_inc();
		}

		// INC+DEC �̏ꍇ
		else if (strcmp(m_sz_nor_sweep_cont, "INC+DEC") == 0)
		{
			show_nor_sweep_cont_incdec();
		}
		// DEC �̏ꍇ
		else if (strcmp(m_sz_nor_sweep_cont, "DEC") == 0)
		{
			show_nor_sweep_cont_dec();
		}
		// DEC+INC �̏ꍇ
		else if (strcmp(m_sz_nor_sweep_cont, "DEC+INC") == 0)
		{
			show_nor_sweep_cont_decinc();
		}
		// STOP �̏ꍇ
		else
		{
			show_nor_sweep_cont_stop();
		}
	}

#else

	// SWEEP-CONT STOP �J�E���g�_�E�����ɊĎ��f�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_nor_sweep_cont_stop_cntdwn >= 0 &&
		strcmp(m_sz_nor_sweep_cont, "STOP") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_nor_sweep_cont_stop_cntdwn = -1;
	}
	// SWEEP-CONT STOP �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_nor_sweep_cont_stop_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_nor_sweep_cont_stop_cntdwn = -1;
	}

	// SWEEP-CONT INC �J�E���g�_�E�����Ƀf�[�^�� INC �ɂȂ����ꍇ
	if (m_l_nor_sweep_cont_inc_cntdwn >= 0 &&
		strcmp(m_sz_nor_sweep_cont, "INC") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_nor_sweep_cont_inc_cntdwn = -1;
	}
	// SWEEP-CONT INC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_nor_sweep_cont_inc_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_nor_sweep_cont_inc_cntdwn = -1;
	}

	// SWEEP-CONT INC+DEC �J�E���g�_�E�����Ƀf�[�^�� INC+DEC �ɂȂ����ꍇ
	if (m_l_nor_sweep_cont_incdec_cntdwn >= 0 &&
		strcmp(m_sz_nor_sweep_cont, "INC+DEC") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_nor_sweep_cont_incdec_cntdwn = -1;
	}
	// SWEEP-CONT INC+DEC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_nor_sweep_cont_incdec_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_nor_sweep_cont_incdec_cntdwn = -1;
	}

	// SWEEP-CONT DEC �J�E���g�_�E�����Ƀf�[�^�� DEC �ɂȂ����ꍇ
	if (m_l_nor_sweep_cont_dec_cntdwn >= 0 &&
		strcmp(m_sz_nor_sweep_cont, "DEC") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_nor_sweep_cont_dec_cntdwn = -1;
	}
	// SWEEP-CONT DEC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_nor_sweep_cont_dec_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_nor_sweep_cont_dec_cntdwn = -1;
	}

	// SWEEP-CONT DEC+INC �J�E���g�_�E�����Ƀf�[�^�� DEC �ɂȂ����ꍇ
	if (m_l_nor_sweep_cont_decinc_cntdwn >= 0 &&
		strcmp(m_sz_nor_sweep_cont, "DEC+INC") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_nor_sweep_cont_decinc_cntdwn = -1;
	}
	// SWEEP-CONT DEC+INC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_nor_sweep_cont_decinc_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_nor_sweep_cont_decinc_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_nor_sweep_cont_stop_cntdwn == -1 && m_l_nor_sweep_cont_inc_cntdwn == -1 &&
		m_l_nor_sweep_cont_incdec_cntdwn == -1 && m_l_nor_sweep_cont_dec_cntdwn == -1 &&
		m_l_nor_sweep_cont_decinc_cntdwn == -1) 
	{
		// INC �̏ꍇ
		if (strcmp(m_sz_nor_sweep_cont, "INC") == 0) 
		{
			show_nor_sweep_cont_inc();
		}

		// INC+DEC �̏ꍇ
		else if (strcmp(m_sz_nor_sweep_cont, "INC+DEC") == 0) 
		{
			show_nor_sweep_cont_incdec();
		}
		// DEC �̏ꍇ
		else if (strcmp(m_sz_nor_sweep_cont, "DEC") == 0) 
		{
			show_nor_sweep_cont_dec();
		}
		// DEC+INC �̏ꍇ
		else if (strcmp(m_sz_nor_sweep_cont, "DEC+INC") == 0) 
		{
			show_nor_sweep_cont_decinc();
		}
		// STOP �̏ꍇ
		else 
		{
			show_nor_sweep_cont_stop();
		}
	}
#endif


	/*** Tri-SWEEP-CONT *******************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_TR_SWEEP_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_tri_sweep_cont, sizeof(m_sz_tri_sweep_cont), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("S-UCSYN.TR_SWEEP_CONT"), CString(m_sz_tri_sweep_cont), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	// Tri-SWEEP-CONT STOP �J�E���g�_�E�����ɊĎ��f�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_tri_sweep_cont_stop_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_tri_sweep_cont_stop_cntdwn = -1;

		// ���䂪�I�������̂ŁA�O�p�X�C�[�v�{�^���L��/�����`�F�b�N�{�b�N�X����������B
		m_SweepContTriChkBtn.SetCheck(0);

		// ���䂪�I�������̂ŁA�O�p�X�C�[�v�{�^���𖳌��ɂ���B
		OnBnClickedTrsctrlSweeptCheck();

		// �O�p�X�C�[�v���䊮���t���O�N���A
		m_l_tri_sweep_flag = 0;

		m_DebugSweepContT = 1;
	}

	// Tri-SWEEP-CONT INC �J�E���g�_�E�����Ƀf�[�^�� INC �ɂȂ����ꍇ
	if (m_l_tri_sweep_cont_inc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_tri_sweep_cont_inc_cntdwn = -1;

		// �O�p�X�C�[�v���䊮���t���O�Z�b�g
		m_l_tri_sweep_flag = 1;

		m_DebugSweepContT = 2;
	}

	// Tri-SWEEP-CONT INC+DEC �J�E���g�_�E�����Ƀf�[�^�� INC+DEC �ɂȂ����ꍇ
	if (m_l_tri_sweep_cont_incdec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_tri_sweep_cont_incdec_cntdwn = -1;

		// �O�p�X�C�[�v���䊮���t���O�Z�b�g
		m_l_tri_sweep_flag = 1;

		m_DebugSweepContT = 3;
	}

	// Tri-SWEEP-CONT DEC �J�E���g�_�E�����Ƀf�[�^�� DEC �ɂȂ����ꍇ
	if (m_l_tri_sweep_cont_dec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_tri_sweep_cont_dec_cntdwn = -1;

		// �O�p�X�C�[�v���䊮���t���O�Z�b�g
		m_l_tri_sweep_flag = 1;

		m_DebugSweepContT = 4;
	}

	// Tri-SWEEP-CONT DEC+INC �J�E���g�_�E�����Ƀf�[�^�� DEC �ɂȂ����ꍇ
	if (m_l_tri_sweep_cont_decinc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_tri_sweep_cont_decinc_cntdwn = -1;

		// �O�p�X�C�[�v���䊮���t���O�Z�b�g
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

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_tri_sweep_cont_stop_cntdwn == -1 && m_l_tri_sweep_cont_inc_cntdwn == -1 &&
		m_l_tri_sweep_cont_incdec_cntdwn == -1 && m_l_tri_sweep_cont_dec_cntdwn == -1 &&
		m_l_tri_sweep_cont_decinc_cntdwn == -1) 
	{
		// INC �̏ꍇ
		if (strcmp(m_sz_tri_sweep_cont, "INC") == 0) 
		{
			show_tri_sweep_cont_inc();
		}

		// INC+DEC �̏ꍇ
		else if (strcmp(m_sz_tri_sweep_cont, "INC+DEC") == 0) 
		{
			show_tri_sweep_cont_incdec();
		}
		// DEC �̏ꍇ
		else if (strcmp(m_sz_tri_sweep_cont, "DEC") == 0) 
		{
			show_tri_sweep_cont_dec();
		}
		// DEC+INC �̏ꍇ
		else if (strcmp(m_sz_tri_sweep_cont, "DEC+INC") == 0) 
		{
			show_tri_sweep_cont_decinc();
		}
		// STOP �̏ꍇ
		else {
			show_tri_sweep_cont_stop();

			// �����Ƀ��[�J������STOP�ɕς�����ꍇ�́A�{�^���𖳌��ɂ���
			if (m_l_tri_sweep_flag == 1) 
			{
				// ���䂪�I�������̂ŁA�O�p�X�C�[�v�{�^���L��/�����`�F�b�N�{�b�N�X����������
				m_SweepContTriChkBtn.SetCheck(0);

				// ���䂪�I�������̂ŁA�O�p�X�C�[�v�{�^���𖳌��ɂ���
				OnBnClickedTrsctrlSweeptCheck();

				// �O�p�X�C�[�v���䊮���t���O�N���A
				m_l_tri_sweep_flag = 0;
			}
		}
	}
#else
	// Tri-SWEEP-CONT STOP �J�E���g�_�E�����ɊĎ��f�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_tri_sweep_cont_stop_cntdwn >= 0 &&
		strcmp(m_sz_tri_sweep_cont, "STOP") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_tri_sweep_cont_stop_cntdwn = -1;

		// ���䂪�I�������̂ŁA�O�p�X�C�[�v�{�^���L��/�����`�F�b�N�{�b�N�X����������B
//		XmToggleButtonSetState(SweepContTri_ChkB, False, False);
		m_SweepContTriChkBtn.SetCheck(0);

		// ���䂪�I�������̂ŁA�O�p�X�C�[�v�{�^���𖳌��ɂ���B
//		TriSweepDis_CB_Click((Widget)NULL, (XtPointer)NULL, (XmPushButtonCallbackStruct *)NULL);
		m_SweepContTriChkBtn.EnableWindow(FALSE);

		// �O�p�X�C�[�v���䊮���t���O�N���A
		m_l_tri_sweep_flag = 0;
	}
	// Tri-SWEEP-CONT STOP �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_tri_sweep_cont_stop_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_tri_sweep_cont_stop_cntdwn = -1;
	}

	// Tri-SWEEP-CONT INC �J�E���g�_�E�����Ƀf�[�^�� INC �ɂȂ����ꍇ
	if (m_l_tri_sweep_cont_inc_cntdwn >= 0 &&
		strcmp(m_sz_tri_sweep_cont, "INC") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_tri_sweep_cont_inc_cntdwn = -1;

		// �O�p�X�C�[�v���䊮���t���O�Z�b�g
		m_l_tri_sweep_flag = 1;
	}
	// Tri-SWEEP-CONT INC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_tri_sweep_cont_inc_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_tri_sweep_cont_inc_cntdwn = -1;
	}

	// Tri-SWEEP-CONT INC+DEC �J�E���g�_�E�����Ƀf�[�^�� INC+DEC �ɂȂ����ꍇ
	if (m_l_tri_sweep_cont_incdec_cntdwn >= 0 &&
		strcmp(m_sz_tri_sweep_cont, "INC+DEC") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_tri_sweep_cont_incdec_cntdwn = -1;

		// �O�p�X�C�[�v���䊮���t���O�Z�b�g
		m_l_tri_sweep_flag = 1;
	}
	// Tri-SWEEP-CONT INC+DEC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_tri_sweep_cont_incdec_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_tri_sweep_cont_incdec_cntdwn = -1;
	}

	// Tri-SWEEP-CONT DEC �J�E���g�_�E�����Ƀf�[�^�� DEC �ɂȂ����ꍇ
	if (m_l_tri_sweep_cont_dec_cntdwn >= 0 &&
		strcmp(m_sz_tri_sweep_cont, "DEC") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_tri_sweep_cont_dec_cntdwn = -1;

		// �O�p�X�C�[�v���䊮���t���O�Z�b�g
		m_l_tri_sweep_flag = 1;
	}
	// Tri-SWEEP-CONT DEC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_tri_sweep_cont_dec_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_tri_sweep_cont_dec_cntdwn = -1;
	}

	// Tri-SWEEP-CONT DEC+INC �J�E���g�_�E�����Ƀf�[�^�� DEC �ɂȂ����ꍇ
	if (m_l_tri_sweep_cont_decinc_cntdwn >= 0 &&
		strcmp(m_sz_tri_sweep_cont, "DEC+INC") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_tri_sweep_cont_decinc_cntdwn = -1;

		// �O�p�X�C�[�v���䊮���t���O�Z�b�g
		m_l_tri_sweep_flag = 1;
	}
	// Tri-SWEEP-CONT DEC+INC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_tri_sweep_cont_decinc_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_tri_sweep_cont_decinc_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_tri_sweep_cont_stop_cntdwn == -1 && m_l_tri_sweep_cont_inc_cntdwn == -1 &&
		m_l_tri_sweep_cont_incdec_cntdwn == -1 && m_l_tri_sweep_cont_dec_cntdwn == -1 &&
		m_l_tri_sweep_cont_decinc_cntdwn == -1) 
	{
		// INC �̏ꍇ
		if (strcmp(m_sz_tri_sweep_cont, "INC") == 0) 
		{
			show_tri_sweep_cont_inc();
		}

		// INC+DEC �̏ꍇ
		else if (strcmp(m_sz_tri_sweep_cont, "INC+DEC") == 0) 
		{
			show_tri_sweep_cont_incdec();
		}
		// DEC �̏ꍇ
		else if (strcmp(m_sz_tri_sweep_cont, "DEC") == 0) 
		{
			show_tri_sweep_cont_dec();
		}
		// DEC+INC �̏ꍇ
		else if (strcmp(m_sz_tri_sweep_cont, "DEC+INC") == 0) 
		{
			show_tri_sweep_cont_decinc();
		}
		// STOP �̏ꍇ
		else {
			show_tri_sweep_cont_stop();

			// �����Ƀ��[�J������STOP�ɕς�����ꍇ�́A�{�^���𖳌��ɂ���
			if (m_l_tri_sweep_flag == 1) 
			{
				// ���䂪�I�������̂ŁA�O�p�X�C�[�v�{�^���L��/�����`�F�b�N�{�b�N�X����������
//				XmToggleButtonSetState(SweepContTri_ChkB, False, False);
				m_SweepContTriChkBtn.SetCheck(0);

				// ���䂪�I�������̂ŁA�O�p�X�C�[�v�{�^���𖳌��ɂ���
//				TriSweepDis_CB_Click((Widget)NULL, (XtPointer)NULL, (XmPushButtonCallbackStruct *)NULL);
				m_SweepContTriChkBtn.EnableWindow(FALSE);

				// �O�p�X�C�[�v���䊮���t���O�N���A
				m_l_tri_sweep_flag = 0;
			}
		}
	}
#endif


	/*** S-TXSYN Tri-SWEEP NUM ************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_TR_SWEEP_NUM));
	if (obs_adr != NULL)
	{
		m_d_tri_sweep_num = obs_adr->d_data;
	}

	// ���l�\�������X�V
	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_tri_sweep_num % 100);
	m_SweepNumBtn.SetWindowTextW(CString(sz_buff));

	// Tri-SWEEP-NUM �J�E���g�_�E�����Ƀf�[�^���ݒ�l�ɂȂ����ꍇ
	if (m_l_tri_sweep_num_cntdwn >= 0 &&
		m_l_tri_sweep_num_set == (long)m_d_tri_sweep_num) 
	{
		// �J�E���g�_�E���I��
		m_l_tri_sweep_num_cntdwn = -1;
	}
	// Tri-SWEEP-NUM �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_tri_sweep_num_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_tri_sweep_num_cntdwn = -1;

		sprintf_s(sz_buff, sizeof(sz_buff), "SWEEP-NUM %2d", m_l_tri_sweep_num_set);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), CString(sz_buff), _T(""), nLogEx::debug);
	}


	/*** CMD-MOD **************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_CMD_MOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_cmd_mod, sizeof(m_sz_cmd_mod), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("SS-TCRMOD.MODULATION"), CString(m_sz_cmd_mod), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	// CMD-MOD OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_cmd_mod_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_cmd_mod_off_cntdwn = -1;

		m_DebugCmdMod = 1;
	}

	// CMD-MOD ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_cmd_mod_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
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

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_cmd_mod_off_cntdwn == -1 && m_l_cmd_mod_on_cntdwn == -1) 
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_cmd_mod, "ON") == 0) 
		{
			show_cmd_mod_on();
		}
		// OFF �̏ꍇ
		else 
		{
			show_cmd_mod_off();
		}
	}
#else
	// CMD-MOD OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_cmd_mod_off_cntdwn >= 0 &&
		strcmp(m_sz_cmd_mod, "OFF") == 0) 
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
		strcmp(m_sz_cmd_mod, "ON") == 0) 
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
		if (strcmp(m_sz_cmd_mod, "ON") == 0) 
		{
			show_cmd_mod_on();
		}
		// OFF �̏ꍇ
		else 
		{
			show_cmd_mod_off();
		}
	}
#endif


	/*** CMD MOD INDEX ********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_CMD_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_cmd_mod_idx = obs_adr->d_data;

		// ���l�\�������X�V
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_cmd_mod_idx);
		m_strIndexCmd = CString(sz_buff);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("SS-TCRMOD.CMD_MOD_INDEX"), m_strIndexCmd, nLogEx::debug);
	}

	/*** RNG MOD INDEX ********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_RNG_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_rng_mod_idx = obs_adr->d_data;

		// ���l�\�������X�V
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_rng_mod_idx);
		m_strIndexRng = CString(sz_buff);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("SS-TCRMOD.RNG_MOD_INDEX"), m_strIndexRng, nLogEx::debug);
	}

	/*** DATA SOURCE **********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_DATA_SOURCE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_data_src, sizeof(m_sz_data_src), obs_adr->sz_sts_name);

		// ��ԕ\�������X�V
		m_strIndexCR = CString(m_sz_data_src);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("SS-TCRMOD.DATA_SOURCE"), m_strIndexCR, nLogEx::debug);
	}

	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	/*** SS-RNG ***************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_SS_RNG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_ss_rng, sizeof(m_sz_ss_rng), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("V-EQU.RNG_SS"), CString(m_sz_ss_rng), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	// SS-RNG OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_ss_rng_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_ss_rng_off_cntdwn = -1;

		m_DebugSSRng = 1;
	}

	// SS-RNG ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_ss_rng_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
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

#else
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
#endif


	/*** COH/INCOH ************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_COH_INCOH));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_coh_incoh, sizeof(m_sz_coh_incoh), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("V-EQU.COHI"), CString(m_sz_coh_incoh), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	// COH/INCOH COH �J�E���g�_�E�����Ƀf�[�^�� COH �ɂȂ����ꍇ
	if (m_l_coh_incoh_coh_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_coh_incoh_coh_cntdwn = -1;

		m_DebugCohIncoh = 1;
	}

	// COH/INCOH INCOH �J�E���g�_�E�����Ƀf�[�^�� INCOH �ɂȂ����ꍇ
	if (m_l_coh_incoh_incoh_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
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
#else
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
#endif


	/*** RNGING ***************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("CTRL.RNG_FLG"), CString(m_sz_rnging), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	// RNGING STOP �J�E���g�_�E�����Ƀf�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_rnging_stop_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rnging_stop_cntdwn = -1;

		m_DebugRnging = 1;
	}

	// RNGING START �J�E���g�_�E�����Ƀf�[�^�� START �ɂȂ����ꍇ
	if (m_l_rnging_start_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rnging_start_cntdwn = -1;

		m_DebugRnging = 2;
	}

	if (m_DebugRnging == 1)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), "��~");
	}
	else if (m_DebugRnging == 2)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), "�J�n");
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
#else
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
#endif


	/*** HOLD TONE 2006-12-12 �ǉ� ********************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_HOLD_TONE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hold_tone, sizeof(m_sz_hold_tone), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlS64"), _T("show_opsc_condition"), _T("CMDOUT.HOLDTONE_STAT"), CString(m_sz_hold_tone), nLogEx::debug);
	}
#ifdef _CUTNOCOMM
	// HOLD TONE ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if (m_l_holdtone_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_holdtone_on_cntdwn = -1;

		m_DebugHoldTone = 1;
	}

	// HOLD TONE OFF �J�E���g�_�E�����ɊĎ��f�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_holdtone_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
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

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_holdtone_off_cntdwn == -1 && m_l_holdtone_on_cntdwn == -1) 
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_hold_tone, "ON") == 0) 
		{
			show_holdtone_on();
		}
		// OFF �̏ꍇ
		else
		{
			show_holdtone_off();
		}
	}
#else
	// HOLD TONE ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if (m_l_holdtone_on_cntdwn >= 0 &&
		strcmp(m_sz_hold_tone, "ON") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_holdtone_on_cntdwn = -1;
	}
	// HOLD TONE ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_holdtone_on_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_holdtone_on_cntdwn = -1;
	}

	// HOLD TONE OFF �J�E���g�_�E�����ɊĎ��f�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_holdtone_off_cntdwn >= 0 &&
		strcmp(m_sz_hold_tone, "OFF") == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_holdtone_off_cntdwn = -1;
	}
	// HOLD TONE OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_holdtone_off_cntdwn == 0) 
	{
		// �J�E���g�_�E���I��
		m_l_holdtone_off_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_holdtone_off_cntdwn == -1 && m_l_holdtone_on_cntdwn == -1) 
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_hold_tone, "ON") == 0) 
		{
			show_holdtone_on();
		}
		// OFF �̏ꍇ
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

-# �J�E���g�_�E���^�C���A�E�g

@param  �Ȃ�
@retval �Ȃ�
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

	// SWEEP-CONT NOMAL STOP
	if (m_l_nor_sweep_cont_stop_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT STOP �J�E���g�_�E��:%d\n"), m_l_nor_sweep_cont_stop_cntdwn);
		m_l_nor_sweep_cont_stop_cntdwn--;
	}

	// SWEEP-CONT NOMAL INC
	if (m_l_nor_sweep_cont_inc_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT INC �J�E���g�_�E��:%d\n"), m_l_nor_sweep_cont_inc_cntdwn);
		m_l_nor_sweep_cont_inc_cntdwn--;
	}

	// SWEEP-CONT NOMAL INC+DEC
	if (m_l_nor_sweep_cont_incdec_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT INC+DEC �J�E���g�_�E��:%d\n"), m_l_nor_sweep_cont_incdec_cntdwn);
		m_l_nor_sweep_cont_incdec_cntdwn--;
	}

	// SWEEP-CONT NOMAL DEC
	if (m_l_nor_sweep_cont_dec_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT DEC �J�E���g�_�E��:%d\n"), m_l_nor_sweep_cont_dec_cntdwn);
		m_l_nor_sweep_cont_dec_cntdwn--;
	}

	// SWEEP-CONT NOMAL DEC+INC
	if (m_l_nor_sweep_cont_decinc_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT DEC+INC �J�E���g�_�E��:%d\n"), m_l_nor_sweep_cont_decinc_cntdwn);
		m_l_nor_sweep_cont_decinc_cntdwn--;
	}

	// SWEEP-CONT TRIANGLE STOP
	if (m_l_tri_sweep_cont_stop_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT STOP �J�E���g�_�E��:%d\n"), m_l_tri_sweep_cont_stop_cntdwn);
		m_l_tri_sweep_cont_stop_cntdwn--;
	}

	// SWEEP-CONT TRIANGLE INC
	if (m_l_tri_sweep_cont_inc_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT INC �J�E���g�_�E��:%d\n"), m_l_tri_sweep_cont_inc_cntdwn);
		m_l_tri_sweep_cont_inc_cntdwn--;
	}

	// SWEEP-CONT TRIANGLE INC+DEC
	if (m_l_tri_sweep_cont_incdec_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT INC+DEC �J�E���g�_�E��:%d\n"), m_l_tri_sweep_cont_incdec_cntdwn);
		m_l_tri_sweep_cont_incdec_cntdwn--;
	}

	// SWEEP-CONT TRIANGLE DEC
	if (m_l_tri_sweep_cont_dec_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT DEC �J�E���g�_�E��:%d\n"), m_l_tri_sweep_cont_dec_cntdwn);
		m_l_tri_sweep_cont_dec_cntdwn--;
	}

	// SWEEP-CONT TRIANGLE DEC+INC
	if (m_l_tri_sweep_cont_decinc_cntdwn > 0) {
		TRACE(_T("SWEEP-CONT DEC+INC �J�E���g�_�E��:%d\n"), m_l_tri_sweep_cont_decinc_cntdwn);
		m_l_tri_sweep_cont_decinc_cntdwn--;
	}

	// SWEEP-NUM
	if (m_l_sweep_num_cntdwn > 0)
	{
		TRACE(_T("SWEEP-NUM �J�E���g�_�E��:%d\n"), m_l_sweep_num_cntdwn);
		m_l_sweep_num_cntdwn--;
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

	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	// SS-RNG OFF
	if (m_l_ss_rng_off_cntdwn > 0)
	{
		TRACE(_T("SS-RNG OFF �J�E���g�_�E��:%d\n"), m_l_ss_rng_off_cntdwn);
		m_l_ss_rng_off_cntdwn--;
	}

	// SS-RNG ON
	if (m_l_ss_rng_on_cntdwn > 0)
	{
		TRACE(_T("SS-RNG ON �J�E���g�_�E��:%d\n"), m_l_ss_rng_on_cntdwn);
		m_l_ss_rng_on_cntdwn--;
	}

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

	// RNGING STOP
	if (m_l_rnging_stop_cntdwn > 0)
	{
		TRACE(_T("RNGING STOP �J�E���g�_�E��:%d\n"), m_l_rnging_stop_cntdwn);
		m_l_rnging_stop_cntdwn--;
	}

	// RNGING START
	if (m_l_rnging_start_cntdwn > 0)
	{
		TRACE(_T("RNGING START �J�E���g�_�E��:%d\n"), m_l_rnging_start_cntdwn);
		m_l_rnging_start_cntdwn--;
	}

	// HOLD TONE OFF
	if (m_l_holdtone_off_cntdwn > 0)
	{
		TRACE(_T("HOLD TONE OFF �J�E���g�_�E��:%d\n"), m_l_holdtone_off_cntdwn);
		m_l_holdtone_off_cntdwn--;
	}

	// HOLD TONE ON
	if (m_l_holdtone_on_cntdwn > 0)
	{
		TRACE(_T("HOLD TONE ON �J�E���g�_�E��:%d\n"), m_l_holdtone_on_cntdwn);
		m_l_holdtone_on_cntdwn--;
	}

	SetTimer(TID_COUNTDOWN, INTERVAL_COUNTDOWN, 0);
}

/*============================================================================*/
/*! CTrsControlS64

-# RF-POWER OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_rf_power_off()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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
	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "�ڑ�");
	strcpy_s(m_sz_rf_on_enbl, sizeof(m_sz_rf_on_enbl), "ON");
#endif

	if (strcmp(m_sz_lan_s_tx, "�ڑ�") == 0)
	{
		// �����[�g�i�I���j�\��
		m_RfPwrOffBtn.SetColor(COLOR_REMOTES);	// OFF �{�^��
	}
	else 
	{
		// ���[�J���\��
		m_RfPwrOffBtn.SetColor(COLOR_LOCAL);	// OFF �{�^��
	}

	// ON ���{�^���̐ݒ�
	// �����[�g��
	if ((strcmp(m_sz_s_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_rf_on_enbl, "ON") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// X-TX��LAN�ڑ���Ԃ��m�F����
#if 1
		if (strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") == 0)
		{
#else
		if (strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) 
		{
#endif

			// X-TX RF DET ANS ���m�F����
			if (strcmp(m_sz_x_tx_det_ans, "OFF") == 0)
			{
				// �����[�g�i��I���j�\��
				m_RfPwrOnBtn.SetColor(COLOR_REMOTE);	// ON �{�^��
			}
			else 
			{
				// ���[�J���\��
				m_RfPwrOnBtn.SetColor(COLOR_LOCAL);		// ON �{�^��
			}
		}
		else 
		{
			// �����[�g�i��I���j�\��
			m_RfPwrOnBtn.SetColor(COLOR_REMOTE);	// ON �{�^��
		}
	}
	// ���[�J����
	else 
	{
		// ���[�J���\��
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);		// ON �{�^��
	}

	// �X�V
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# RF-POWER ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_rf_power_on()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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
	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "�ڑ�");
	strcpy_s(m_sz_rf_on_enbl, sizeof(m_sz_rf_on_enbl), "OFF");
#endif

	if (strcmp(m_sz_lan_s_tx, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);		// ON �{�^��
	}
	else 
	{
		// �����[�g�i�I���j�\��
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES);	// ON �{�^��
	}

	// OFF ���{�^���̐ݒ�
	// �����[�g��
	if ((strcmp(m_sz_s_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_rf_on_enbl, "OFF") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j�\��
		m_RfPwrOffBtn.SetColor(COLOR_REMOTE);	// OFF �{�^��
	}
	// ���[�J����
	else 
	{
		// ���[�J���\��
		m_RfPwrOffBtn.SetColor(COLOR_LOCAL);	// OFF �{�^��
	}

	// �X�V
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# NO-USE OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_no_use_off()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_s_tx, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_NoUseOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j�\��
		m_NoUseOffBtn.SetColor(COLOR_REMOTES);
	}

	// �����[�g��
	if ((strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j�\��
		m_NoUseOnBtn.SetColor(COLOR_REMOTE);
	}

	// ���[�J����
	else 
	{
		// ���[�J���\��
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_NoUseOffBtn.Invalidate();
	m_NoUseOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# NO-USE ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_no_use_on()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_s_tx, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j�\��
		m_NoUseOnBtn.SetColor(COLOR_REMOTES);
	}

	// �����[�g��
	if ((strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j�\��
		m_NoUseOffBtn.SetColor(COLOR_REMOTE);
	}

	// ���[�J����
	else 
	{
		// ���[�J���\��
		m_NoUseOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_NoUseOffBtn.Invalidate();
	m_NoUseOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT NORMAL STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_nor_sweep_cont_stop()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_s_txsyn, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_SweepContNStopBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j�\��
		m_SweepContNStopBtn.SetColor(COLOR_REMOTES);
	}

	// �����[�g��
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) &&
		(strcmp(m_sz_tri_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j�\��
		m_SweepContNIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContNIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContNDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContNDecIncBtn.SetColor(COLOR_REMOTE);
	}
	// ���[�J����
	else 
	{
		// ���[�J���\��
		m_SweepContNIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContNIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContNDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContNDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT NORMAL INC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_nor_sweep_cont_inc()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_s_txsyn, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_SweepContNIncBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j�\��
		m_SweepContNIncBtn.SetColor(COLOR_REMOTES);
	}

	// �����[�g��
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) &&
		(strcmp(m_sz_tri_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j�\��
		m_SweepContNStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContNIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContNDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContNDecIncBtn.SetColor(COLOR_REMOTE);
	}
	// ���[�J����
	else 
	{
		// ���[�J���\��
		m_SweepContNStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContNIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContNDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContNDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT NORMAL INCDEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_nor_sweep_cont_incdec()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_s_txsyn, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_SweepContNIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j�\��
		m_SweepContNIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// �����[�g��
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) &&
		(strcmp(m_sz_tri_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j�\��
		m_SweepContNStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContNIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContNDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContNDecIncBtn.SetColor(COLOR_REMOTE);
	}
	// ���[�J����
	else 
	{
		// ���[�J���\��
		m_SweepContNStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContNIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContNDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContNDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT NORMAL DEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_nor_sweep_cont_dec()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_s_txsyn, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_SweepContNDecBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j�\��
		m_SweepContNDecBtn.SetColor(COLOR_REMOTES);
	}

	// �����[�g��
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) &&
		(strcmp(m_sz_tri_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j�\��
		m_SweepContNStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContNIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContNIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContNDecIncBtn.SetColor(COLOR_REMOTE);
	}
	// ���[�J����
	else 
	{
		// ���[�J���\��
		m_SweepContNStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContNIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContNIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContNDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT NORMAL DECINC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_nor_sweep_cont_decinc()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_s_txsyn, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_SweepContNDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j�\��
		m_SweepContNDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// �����[�g��
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) &&
		(strcmp(m_sz_tri_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j�\��
		m_SweepContNStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContNIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContNIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContNDecBtn.SetColor(COLOR_REMOTE);
	}
	// ���[�J����
	else 
	{
		// ���[�J���\��
		m_SweepContNStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContNIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContNIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContNDecBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepContNStopBtn.Invalidate();
	m_SweepContNIncBtn.Invalidate();
	m_SweepContNIncDecBtn.Invalidate();
	m_SweepContNDecBtn.Invalidate();
	m_SweepContNDecIncBtn.Invalidate();
}



/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT TRIANGLE STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_tri_sweep_cont_stop()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "�ڑ�");

	if (strcmp(m_sz_lan_s_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepContTStopBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SweepContTStopBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) &&
		(strcmp(m_sz_nor_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_SweepContTIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContTIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContTDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContTDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_SweepContTIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContTIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContTDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContTDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT TRIANGLE INC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_tri_sweep_cont_inc()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "�ڑ�");

	if (strcmp(m_sz_lan_s_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepContTIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepContTIncBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) &&
		(strcmp(m_sz_nor_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepContTStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContTIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContTDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContTDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepContTStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContTIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContTDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContTDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT TRIANGLE INCDEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_tri_sweep_cont_incdec()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "�ڑ�");

	if (strcmp(m_sz_lan_s_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepContTIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepContTIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) &&
		(strcmp(m_sz_nor_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepContTStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContTIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContTDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContTDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepContTStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContTIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContTDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContTDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT TRIANGLE DEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_tri_sweep_cont_dec()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "�ڑ�");

	if (strcmp(m_sz_lan_s_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepContTDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepContTDecBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) &&
		(strcmp(m_sz_nor_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepContTStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContTIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContTIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContTDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepContTStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContTIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContTIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContTDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SWEEP-CONT TRIANGLE DECINC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_tri_sweep_cont_decinc()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_S_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), "�ڑ�");

	if (strcmp(m_sz_lan_s_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepContTDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepContTDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑�
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) &&
		(strcmp(m_sz_nor_sweep_cont, "OFF") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepContTStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContTIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContTIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContTDecBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepContTStopBtn.SetColor(COLOR_LOCAL);
		m_SweepContTIncBtn.SetColor(COLOR_LOCAL);
		m_SweepContTIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepContTDecBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepContTStopBtn.Invalidate();
	m_SweepContTIncBtn.Invalidate();
	m_SweepContTIncDecBtn.Invalidate();
	m_SweepContTDecBtn.Invalidate();
	m_SweepContTDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# CMD-MOD OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_cmd_mod_off()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "�ڑ�");
#ifdef _CUTNOCOMM
	strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), "REMOTE");
#endif

	if (strcmp(m_sz_lan_s_tx, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_ModOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j�\��
		m_ModOffBtn.SetColor(COLOR_REMOTES);
	}

	// �����[�g��
	if ((strcmp(m_sz_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j�\��
		m_ModOnBtn.SetColor(COLOR_REMOTE);
	}
	// ���[�J����
	else 
	{
		// ���[�J���\��
		m_ModOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_ModOffBtn.Invalidate();
	m_ModOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# CMD-MOD ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_cmd_mod_on()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), "�ڑ�");
#ifdef _CUTNOCOMM
	strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), "REMOTE");
#endif

	if (strcmp(m_sz_lan_s_tx, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_ModOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j�\��
		m_ModOnBtn.SetColor(COLOR_REMOTES);
	}

	// �����[�g��
	if ((strcmp(m_sz_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j�\��
		m_ModOffBtn.SetColor(COLOR_REMOTE);
	}
	// ���[�J����
	else 
	{
		// ���[�J���\��
		m_ModOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_ModOffBtn.Invalidate();
	m_ModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS64

-# SS-RNG OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_ss_rng_off()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_vequ, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_SSRngOffBtn.SetColor(COLOR_LOCAL);
		m_SSRngOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		if (m_i_priv_flag == 1)
		{
			// �����[�g�i��I���j�\��
			m_SSRngOnBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ���[�J���\��
			m_SSRngOnBtn.SetColor(COLOR_LOCAL);
		}
		// �����[�g�i�I���j�\��
		m_SSRngOffBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_SSRngOffBtn.Invalidate();
	m_SSRngOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# SS-RNG ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_ss_rng_on()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_vequ, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_SSRngOnBtn.SetColor(COLOR_LOCAL);
		m_SSRngOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		if (m_i_priv_flag == 1)
		{
			// �����[�g�i��I���j�\��
			m_SSRngOffBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ���[�J���\��
			m_SSRngOffBtn.SetColor(COLOR_LOCAL);
		}
		// �����[�g�i�I���j�\��
		m_SSRngOnBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_SSRngOnBtn.Invalidate();
	m_SSRngOffBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# COH/INCOH COH ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_coh_incoh_coh()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_vequ, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_CohIncohCohBtn.SetColor(COLOR_LOCAL);
		m_CohIncohInCohBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		if (m_i_priv_flag == 1)
		{
			// �����[�g�i��I���j�\��
			m_CohIncohInCohBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ���[�J���\��
			m_CohIncohInCohBtn.SetColor(COLOR_LOCAL);
		}
		// �����[�g�i�I���j�\��
		m_CohIncohCohBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_CohIncohCohBtn.Invalidate();
	m_CohIncohInCohBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# COH/INCOH INCOH ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_coh_incoh_incoh()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

#ifdef _CUTNOCOMM
	strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), "�ڑ�");
#endif

	if (strcmp(m_sz_lan_vequ, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_CohIncohInCohBtn.SetColor(COLOR_LOCAL);
		m_CohIncohCohBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		if (m_i_priv_flag == 1)
		{
			// �����[�g�i��I���j�\��
			m_CohIncohCohBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ���[�J���\��
			m_CohIncohCohBtn.SetColor(COLOR_LOCAL);
		}
		// �����[�g�i�I���j�\��
		m_CohIncohInCohBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_CohIncohCohBtn.Invalidate();
	m_CohIncohInCohBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# RNGING STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_rnging_stop()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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
	strcpy_s(m_sz_lan_rng, sizeof(m_sz_lan_rng), "�ڑ�");
	strcpy_s(m_sz_rng_cont, sizeof(m_sz_rng_cont), "REMOTE");
#endif

	if (strcmp(m_sz_lan_rng, "�ڑ�") != 0)
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
	if ((m_SSRngOnBtn.GetColor() == COLOR_REMOTES) &&
		((strcmp(m_sz_lan_rng, "�ڑ�") == 0) &&
		(strcmp(m_sz_rng_cont, "REMOTE") == 0) &&
		(m_i_priv_flag == 1)))
	{
		// �����[�g�i��I���j
		m_RngingStartBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# RNGING START ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_rnging_start()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q�� 
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
	strcpy_s(m_sz_lan_rng, sizeof(m_sz_lan_rng), "�ڑ�");
	strcpy_s(m_sz_rng_cont, sizeof(m_sz_rng_cont), "REMOTE");
#endif

	if ((m_SSRngOnBtn.GetColor() == COLOR_REMOTES) &&
		(strcmp(m_sz_lan_rng, "�ڑ�") == 0)) 
	{
		// �����[�g�i�I���j
		m_RngingStartBtn.SetColor(COLOR_REMOTES);
	}
	else 
	{
		// ���[�J��
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}

	// STOP ��
	if ((strcmp(m_sz_lan_rng, "�ڑ�") == 0) && (strcmp(m_sz_rng_cont, "REMOTE") == 0) && (m_i_priv_flag == 1))
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
/*! CTrsControlS64

-# HOLD TONE OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_holdtone_off()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_CMDOUT1));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_cmdout1, sizeof(m_sz_lan_cmdout1), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_cmdout1, sizeof(m_sz_lan_cmdout1), "�ڑ�");

	if (strcmp(m_sz_lan_cmdout1, "�ڑ�") == 0)
	{
		// �����[�g�i�I���j�\��
		m_HoldToneOffBtn.SetColor(COLOR_REMOTES);
	}
	else 
	{
		// ���[�J���\��
		m_HoldToneOffBtn.SetColor(COLOR_LOCAL);
	}

	// ON ��
	if ((strcmp(m_sz_lan_cmdout1, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j�\��
		m_HoldToneOnBtn.SetColor(COLOR_REMOTE);
	}

	// ���[�J����
	else 
	{
		// ���[�J���\��
		m_HoldToneOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# HOLD TONE ON ��ԉ�ʕ\����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::show_holdtone_on()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S64_LAN_CMDOUT1));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_cmdout1, sizeof(m_sz_lan_cmdout1), obs_adr->sz_sts_name);
	}

	strcpy_s(m_sz_lan_cmdout1, sizeof(m_sz_lan_cmdout1), "�ڑ�");

	if (strcmp(m_sz_lan_cmdout1, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_HoldToneOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j�\��
		m_HoldToneOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ���{�^���̐ݒ�
	if ((strcmp(m_sz_lan_cmdout1, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j�\��
		m_HoldToneOffBtn.SetColor(COLOR_REMOTE);
	}
	// ���[�J����
	else 
	{
		// ���[�J���\��
		m_HoldToneOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS64

-# HOLD TONE ON ��ԉ�ʕ\����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS64::printlg_endtime(CString str)
{
	// �Ď��f�[�^�擾

	// ���ݎ����擾

	// �X�C�[�v�����\�莞���Z�o

	// opelog�o��
}


void CTrsControlS64::OnClose()
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

void CTrsControlS64::CloseDlg()
{
	KillTimer(TID_CONDITION);
	KillTimer(TID_COUNTDOWN);

	// �E�B���h�E����郁�b�Z�[�W
	theApp.m_pMainWnd->PostMessageW(eMessage_WindowClose, 0, (LPARAM)this);
}
