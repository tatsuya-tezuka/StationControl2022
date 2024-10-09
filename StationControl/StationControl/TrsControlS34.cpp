/*============================================================================*/
/*! TrsControl.cpp

-# S�ё��M������
*/
/*============================================================================*/
// TrsControlS.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrsControlS34.h"
#include "afxdialogex.h"
#include "TrsControlMng.h"
#include "HookMsgBox.h"

// CTrsControlS34 �_�C�A���O

IMPLEMENT_DYNAMIC(CTrsControlS34, CDialogBase)

/*============================================================================*/
/*! CTrsControlS34

-# �R���X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
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

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
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


// CTrsControlS34 ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CTrsControlS34

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CTrsControlS34::OnInitDialog()
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

	// CMD-MOD
	m_CmdModOffBtn.SetColor(COLOR_REMOTES);
	m_CmdModOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_CmdModOnBtn.SetColor(COLOR_REMOTE);
	m_CmdModOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// HOLD-TONE
	m_HoldToneOffBtn.SetColor(COLOR_REMOTES);
	m_HoldToneOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_HoldToneOnBtn.SetColor(COLOR_REMOTE);
	m_HoldToneOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// COH-INCOH
	m_CohIncohCohBtn.SetColor(COLOR_REMOTES);
	m_CohIncohCohBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_CohIncohInCohBtn.SetColor(COLOR_REMOTES);
	m_CohIncohInCohBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// RNGING
	m_RngingStopBtn.SetColor(COLOR_REMOTES);
	m_RngingStopBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_RngingStartBtn.SetColor(COLOR_REMOTE);
	m_RngingStartBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// INDEX-CMD
	m_IndexCmdBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_IndexCmdBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// INDEX-RNG
	m_IndexRngBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_IndexRngBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// INDEX-C+R
	m_IndexCRBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_IndexCRBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// PWR/ROUTE
	m_TxPwrRouteBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_TxPwrRouteBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// �O���[�v�{�b�N�X
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
/*! CTrsControlS34

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CTrsControlS34::UpdateDlg()
{
	// ���M�����ʏ�ԕ\���^�C���A�E�g�R�[���o�b�N���[�`���N��
	show_opsc_condition();

	// �J�E���g�_�E���^�C���A�E�g�R�[���o�b�N���[�`���N��
	count_down();

	return TRUE;
}


/*============================================================================*/
/*! CTrsControlS34

-# �t�@�C��-���郁�j���[����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnMenuClose()
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
/*! CTrsControlS34

-# �wTX CONTROL�x-�wRF-POWER�x-�wOFF�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlRfOffBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_RfPwrOffBtn.SetColor(COLOR_CTRLIN);
	m_RfPwrOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("S-TX.RF_CONT"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_rf_power_off_cntdwn = RF_POWER_TIME_S34_LIMIT;

	// �X�V
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# �wTX CONTROL�x-�wRF-POWER�x-�wON�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlRfOnBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_RfPwrOnBtn.SetColor(COLOR_CTRLIN);
	m_RfPwrOffBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("S-TX.RF_CONT"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_rf_power_on_cntdwn = RF_POWER_TIME_S34_LIMIT;

	// �X�V
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# �wTX CONTROL�x-�wNO-USER�x-�wOFF�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlNouseOffBtn()
{
	COLORREF col = m_NoUseOffBtn.GetColor();

	if (col == COLOR_REMOTE)	// �����[�g�i��I���j
	{
		// �{�^���𐧌䒆�F�ɂ���
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("S-TX.RF_TRIP_NO_USE"), _T("NO_USE_RESET"));

		// �J�E���g�_�E���J�n
		m_l_no_use_off_cntdwn = NO_USE_TIME_S34_LIMIT;
	}
	else if (col == COLOR_CTRLIN)	// ���䒆
	{
		// �{�^���𐧌䒆�F�ɂ���
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("S-TX.RF_TRIP_NO_USE"), _T("NO_USE_RESET"));

		// �J�E���g�_�E���J�n
		m_l_no_use_off_cntdwn = NO_USE_TIME_S34_LIMIT;

		// �����̃J�E���g�_�E���I��
		m_l_no_use_on_cntdwn = -1;
	}
	else if (col == COLOR_REMOTES)	// �����[�g�i�I���j
	{
		// �{�^���𐧌䒆�F�ɂ���
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("S-TX.RF_TRIP_NO_USE"), _T("NO_USE_RESET"));

		// �J�E���g�_�E���J�n
		m_l_no_use_off_cntdwn = NO_USE_TIME_S34_LIMIT;

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
/*! CTrsControlS34

-# �wTX CONTROL�x-�wNO-USER�x-�wON�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlNouseOnBtn()
{
	COLORREF col = m_NoUseOnBtn.GetColor();

	if (col == COLOR_REMOTE)	// �����[�g�i��I���j
	{
		// �{�^���𐧌䒆�F�ɂ���
		m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("S-TX.RF_TRIP_NO_USE"), _T("NO_USE"));

		// �J�E���g�_�E���J�n
		m_l_no_use_on_cntdwn = NO_USE_TIME_S34_LIMIT;
	}
	else if (col == COLOR_CTRLIN)	// ���䒆
	{
		// �{�^���𐧌䒆�F�ɂ���
		m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("S-TX.RF_TRIP_NO_USE"), _T("NO_USE"));

		// �J�E���g�_�E���J�n
		m_l_no_use_on_cntdwn = NO_USE_TIME_S34_LIMIT;

		// �����̃J�E���g�_�E���I��
		m_l_no_use_off_cntdwn = -1;
	}
	else if (col == COLOR_REMOTES)	// �����[�g�i�I���j
	{
		// �{�^���𐧌䒆�F�ɂ���
		m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("S-TX.RF_TRIP_NO_USE"), _T("NO_USE"));

		// �J�E���g�_�E���J�n
		m_l_no_use_on_cntdwn = NO_USE_TIME_S34_LIMIT;

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
/*! CTrsControlS34

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wSTOP�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlSweetStopBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_SweepContStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CONT"), _T("OFF"));
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CMD"), _T("STOP"));

	// �J�E���g�_�E���J�n
	m_l_sweep_cont_stop_cntdwn = SWEEP_CONT_TIME_S34_LIMIT;

	// �X�V
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wINC�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlSweetIncBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_SweepContStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CONT"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_sweep_cont_inc_cntdwn = SWEEP_CONT_TIME_S34_LIMIT;
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// �X�V
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wINCDEC�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlSweetIncdecBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_SweepContStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CONT"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_sweep_cont_incdec_cntdwn = SWEEP_CONT_TIME_S34_LIMIT;
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// �X�V
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wDEC�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlSweetDecBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_SweepContStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CONT"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_sweep_cont_dec_cntdwn = SWEEP_CONT_TIME_S34_LIMIT;
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// �X�V
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wDECINC�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlSweetDecincBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_SweepContStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepContIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepContDecIncBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.SWEEP_CONT"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_sweep_cont_decinc_cntdwn = SWEEP_CONT_TIME_S34_LIMIT;
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// �X�V
	m_SweepContStopBtn.Invalidate();
	m_SweepContIncBtn.Invalidate();
	m_SweepContIncDecBtn.Invalidate();
	m_SweepContDecBtn.Invalidate();
	m_SweepContDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# �wTX CONTROL�x-�wCMD-MOD�x-�wOFF�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlCmdOffBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_CmdModOffBtn.SetColor(COLOR_CTRLIN);
	m_CmdModOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.MODULATION"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_cmd_mod_off_cntdwn = CMD_MOD_TIME_S34_LIMIT;

	// �X�V
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# �wTX CONTROL�x-�wCMD-MOD�x-�wON�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlCmdOnBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_CmdModOffBtn.SetColor(COLOR_CTRLIN);
	m_CmdModOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("SS-TCRMOD.MODULATION"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_cmd_mod_on_cntdwn = CMD_MOD_TIME_S34_LIMIT;

	// �X�V
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# �wTX CONTROL�x-�wHOLD-TONE�x-�wOFF�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlHoldOffBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CMDOUT.HOLD_TONE"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_holdtone_off_cntdwn = HOLDTONE_TIME_S34_LIMIT;

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# �wTX CONTROL�x-�wHOLD-TONE�x-�wON�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlHoldOnBtn()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CMDOUT.HOLD_TONE"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_holdtone_on_cntdwn = HOLDTONE_TIME_S34_LIMIT;

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# �wTX CONTROL�x-�wINDEX�x-�wCMD�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlIdxCmdBtn()
{
	CString strCmd;
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	// �����[�g��
	if ((strcmp(m_sz_mod_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_ss_tcrmod, "�ڑ�") == 0) &&
		(strlen(m_sz_sat_name) > 0) &&
		(m_i_priv_flag == 1))
	{
		// �ݔ��ڍא���v���Z�X(opctproc)�N��
		strCmd.Format(_T("CMD,%s"), (LPCTSTR)CString(m_sz_sat_name));
		theApp.GetSatelliteData().opctproc2(_T("CTRL.MOD_INDEX"), strCmd);
	}
}

/*============================================================================*/
/*! CTrsControlS34

-# �wTX CONTROL�x-�wINDEX�x-�wRNG�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlIdxRngBtn()
{
	CString strCmd;
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();


	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	// �����[�g��
	if ((strcmp(m_sz_mod_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_ss_tcrmod, "�ڑ�") == 0) &&
		(strlen(m_sz_sat_name) > 0) &&
		(m_i_priv_flag == 1))
	{
		// �ݔ��ڍא���v���Z�X(opctproc)�N��
		strCmd.Format(_T("RNG,%s"), (LPCTSTR)CString(m_sz_sat_name));
		theApp.GetSatelliteData().opctproc2(_T("CTRL.MOD_INDEX"), strCmd);
	}
}

/*============================================================================*/
/*! CTrsControlS34

-# �wTX CONTROL�x-�wINDEX�x-�wC+R�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlIdxCrBtn()
{
	CString strCmd;
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();


	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	// �����[�g��
	if ((strcmp(m_sz_mod_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_ss_tcrmod, "�ڑ�") == 0) &&
		(strlen(m_sz_sat_name) > 0) &&
		(m_i_priv_flag == 1))
	{
		// �ݔ��ڍא���v���Z�X(opctproc)�N��
		strCmd.Format(_T("CMD+RNG,%s"), (LPCTSTR)CString(m_sz_sat_name));
		theApp.GetSatelliteData().opctproc2(_T("CTRL.MOD_INDEX"), strCmd);
	}
}

/*============================================================================*/
/*! CTrsControlS34

-# �wTX CONTROL�x-�wTX PWR/ROUTE�x-�wPOWER/ROUTE�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlTxprPrBtn()
{
	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CTRL.S_TX_RF_PWR"), _T(""));
}


/*============================================================================*/
/*! CTrsControlS34

-# �wRNG CONTROL�x-�wCHO/INCOH�x-�wCOH�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlCohincohCohBtn2()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_CohIncohCohBtn.SetColor(COLOR_CTRLIN);
	m_CohIncohInCohBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("COHI"));

	// �J�E���g�_�E���J�n
	m_l_coh_incoh_coh_cntdwn = COH_INCOH_TIME_S34_LIMIT;

	// �X�V
	m_CohIncohCohBtn.Invalidate();
	m_CohIncohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# �wRNG CONTROL�x-�wCHO/INCOH�x-�wINCOH�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlCohincohIncohBtn2()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_CohIncohCohBtn.SetColor(COLOR_CTRLIN);
	m_CohIncohInCohBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("INCOHI"));

	// �J�E���g�_�E���J�n
	m_l_coh_incoh_incoh_cntdwn = COH_INCOH_TIME_S34_LIMIT;

	// �X�V
	m_CohIncohCohBtn.Invalidate();
	m_CohIncohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# �wRNG CONTROL�x-�wRNGING�x-�wSTOP�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlRngingStopBtn2()
{
	// �{�^���𐧌䒆�F�ɂ���
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING_CANCEL"), _T(","));

	// �J�E���g�_�E���J�n
	m_l_rnging_stop_cntdwn = RNGING_TIME_S34_LIMIT;

	// �X�V
	m_RngingStopBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# �wRNG CONTROL�x-�wRNGING�x-�wSTART�x�{�^����������

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnBnClickedTrsctrlRngingStartBtn2()
{
	CString str;

	// �{�^���𐧌䒆�F�ɂ���
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);
	m_RngingStartBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	str.Format(_T("S-%02d"), 99);
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str);

	// �J�E���g�_�E���J�n
	m_l_rnging_start_cntdwn = RNGING_TIME_S34_LIMIT;

	// �X�V
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# OnTimer

@param  nIDEvent:�^�C�}�[�̎��ʎq
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::OnTimer(UINT_PTR nIDEvent)
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
/*! CTrsControlS34

-# ���M�����ʏ�ԕ\���ݒ�

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::show_opsc_condition()
{
	TRACE(_T("show_opsc_condition\n"));

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	char sz_buff[64] = { 0 };
	char sz_misc1[32] = { 0 };
	char sz_misc2[32] = { 0 };

	KillTimer(TID_CONDITION);

	CSatelliteData& sd = theApp.GetSatelliteData();

	CString msg = _T("");

	// ���䌠�͏�ɗL��
	m_i_priv_flag = 1;

	/*-- TX CONTROL ----------------------------------------------------------------------*/

	// ���M�����ʂ̃��W�b�N�������ɂ��ǉ� 

	// RF POWER ON ENABLE ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_RF_ON_ENBL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl, sizeof(m_sz_rf_on_enbl), obs_adr->sz_sts_name);
	}

	// RF-TRIP NOUSE ON ENABLE ��� 
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_NO_USE_ENBL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl, sizeof(m_sz_no_use_enbl), obs_adr->sz_sts_name);
	}

	// UP-LINK CONTROL ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_UPLINK_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_uplink_cont, sizeof(m_sz_uplink_cont), obs_adr->sz_sts_name);
	}

	// S-TX RF POWER ON DETECT ANSWER ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_RF_ON_DET_ANS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans, sizeof(m_sz_rf_on_det_ans), obs_adr->sz_sts_name);
	}

	// X-TX RF POWER ON DETECT ANSWER ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_X_TX_DET_ANS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_tx_det_ans, sizeof(m_sz_x_tx_det_ans), obs_adr->sz_sts_name);
	}

	/*** �����[�g�^���[�J�� ��Ԏ擾 ******************************************************/

	// S-TX �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_S_TX_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_s_tx_rl, sizeof(m_sz_s_tx_rl), obs_adr->sz_sts_name);
	}

	// S-TXSYN �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_S_TXSYN_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_s_txsyn_rl, sizeof(m_sz_s_txsyn_rl), obs_adr->sz_sts_name);
	}

	/*** �A���[���`�F�b�N *****************************************************************/
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_ALARM));
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
		m_grCmdMod.SetBackColor(COLOR_ALARM);
	}
	// �A���[������
	else
	{
		// �g�̐F�����ɖ߂�
		m_grRfPower.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grNoUse.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepCont.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grCmdMod.SetBackColor(GetSysColor(COLOR_3DFACE));
	}

	/*** RF-POWER *************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_RF_POWER));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_power, sizeof(m_sz_rf_power), obs_adr->sz_sts_name);
	}

	// RF-POWER ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if (m_l_rf_power_on_cntdwn >= 0 && strcmp(m_sz_rf_power, "ON") == 0)
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
	if (m_l_rf_power_off_cntdwn >= 0 && strcmp(m_sz_rf_power, "OFF") == 0)
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
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_NO_USE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use, sizeof(m_sz_no_use), obs_adr->sz_sts_name);
	}

	// NO-USE ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if (m_l_no_use_on_cntdwn >= 0 && strcmp(m_sz_no_use, "ON") == 0)
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
	if (m_l_no_use_off_cntdwn >= 0 && strcmp(m_sz_no_use, "ON") != 0)
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
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;
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
	else if ((m_l_sweep_cont_inc_cntdwn >= 0) && (strcmp(m_sz_sweep_cmd, "INC") != 0))
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
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

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
		// �J�E���g�_�E���I��
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


	/*** CMD-MOD **************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_CMD_MOD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_cmd_mod, sizeof(m_sz_cmd_mod), obs_adr->sz_sts_name);
	}

	// CMD-MOD OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_cmd_mod_off_cntdwn >= 0 && strcmp(m_sz_cmd_mod, "OFF") == 0)
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
	if (m_l_cmd_mod_on_cntdwn >= 0 && strcmp(m_sz_cmd_mod, "ON") == 0)
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


	/*** HOLD TONE ************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_HOLD_TONE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hold_tone, sizeof(m_sz_hold_tone), obs_adr->sz_sts_name);
	}

	// HOLD TONE ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if (m_l_holdtone_on_cntdwn >= 0 && strcmp(m_sz_hold_tone, "ON") == 0)
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
	if (m_l_holdtone_off_cntdwn >= 0 && strcmp(m_sz_hold_tone, "OFF") == 0)
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


	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	/*** SS-RNG / SX-RNG ******************************************************************/
	/*** ���l�\�����ŐV�f�[�^�\�� *********************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
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
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_COH_INCOH));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_coh_incoh, sizeof(m_sz_coh_incoh), obs_adr->sz_sts_name);
	}

	// COH/INCOH COH �J�E���g�_�E�����Ƀf�[�^�� COH �ɂȂ����ꍇ
	if (m_l_coh_incoh_coh_cntdwn >= 0 && strcmp(m_sz_coh_incoh, "COHI") == 0)
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
	if (m_l_coh_incoh_incoh_cntdwn >= 0 && strcmp(m_sz_coh_incoh, "INCOHI") == 0)
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
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
	}

	// RNGING STOP �J�E���g�_�E�����Ƀf�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_rnging_stop_cntdwn >= 0 && strcmp(m_sz_rnging, "�I��") == 0)
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
	if (m_l_rnging_start_cntdwn >= 0 && strcmp(m_sz_rnging, "�J�n") == 0)
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


	/*** CMD MOD INDEX ********************************************************************/
	/*** ���l�\�����ŐV�f�[�^�\�� *********************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_CMD_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_cmd_mod_idx = obs_adr->d_data;

		// ���l�\�������X�V
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_cmd_mod_idx);
		m_strIndexCmd = CString(sz_buff);

	}

	/*** RNG MOD INDEX ********************************************************************/
	/*** ���l�\�����ŐV�f�[�^�\�� *********************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_RNG_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_rng_mod_idx = obs_adr->d_data;

		// ���l�\�������X�V
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_rng_mod_idx);
		m_strIndexRng = CString(sz_buff);
	}

	/*** DATA SOURCE **********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_DATA_SOURCE));
	if (obs_adr != NULL)
	{
		sprintf_s(m_sz_data_src, sizeof(m_sz_data_src), "%-10s", obs_adr->sz_sts_name);

		// ��ԕ\�������X�V
		m_strIndexCR = CString(m_sz_data_src);
	}


	/*** S��RNG�v�����[�g *****************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_S_RNG_ROUTE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_data_src, sizeof(m_sz_data_src), obs_adr->sz_sts_name);
	}

	// ��ԕ\�������X�V
	m_strRngRoute = CString(m_sz_data_src);

//	int id = 0;
	CString id;
	if (strcmp(m_sz_rng_route, "S/S") == 0)
	{
		// S/S��RNG�v�����̏ꍇ

		/*** �ǐՃf�[�^�z�M��� ***********************************************************/
		// ���݂̏�Ԃ��Ď��c�a����擾
		id = CString(MON_S34_SS_TRK_STS);
	}
	else
	{
		// S/X��RNG�v�����̏ꍇ

		/*** �ǐՃf�[�^�z�M��� ***********************************************************/
		// ���݂̏�Ԃ��Ď��c�a����擾
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

	// ��ԕ\�������X�V
	m_strTrkData = CString(m_sz_data_src);

	/*** �e�����g���f�[�^�z�M��� *********************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾

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

		// ��ԕ\�������X�V
		m_strTlmData = CString(m_sz_data_src);
	}


	// ���݂̏�Ԃ��Ď��c�a����擾
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
		// �ǂ��炩�̃X�e�[�^�X�ɕύX��������
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

-# �J�E���g�_�E���^�C���A�E�g

@param  �Ȃ�
@retval �Ȃ�
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
	if (m_l_sweep_cont_stop_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT STOP �J�E���g�_�E��:%d\n"), m_l_sweep_cont_stop_cntdwn);
		m_l_sweep_cont_stop_cntdwn--;
	}

	// SWEEP-CONT NOMAL INC
	if (m_l_sweep_cont_inc_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT INC �J�E���g�_�E��:%d\n"), m_l_sweep_cont_inc_cntdwn);
		m_l_sweep_cont_inc_cntdwn--;
	}

	// SWEEP-CONT NOMAL INC+DEC
	if (m_l_sweep_cont_incdec_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT INC+DEC �J�E���g�_�E��:%d\n"), m_l_sweep_cont_incdec_cntdwn);
		m_l_sweep_cont_incdec_cntdwn--;
	}

	// SWEEP-CONT NOMAL DEC
	if (m_l_sweep_cont_dec_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT DEC �J�E���g�_�E��:%d\n"), m_l_sweep_cont_dec_cntdwn);
		m_l_sweep_cont_dec_cntdwn--;
	}

	// SWEEP-CONT NOMAL DEC+INC
	if (m_l_sweep_cont_decinc_cntdwn > 0)
	{
		TRACE(_T("SWEEP-CONT DEC+INC �J�E���g�_�E��:%d\n"), m_l_sweep_cont_decinc_cntdwn);
		m_l_sweep_cont_decinc_cntdwn--;
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

	// SX-RNG OFF
	if (m_l_sx_rng_off_cntdwn > 0)
	{
		TRACE(_T("SX-RNG OFF �J�E���g�_�E��:%d\n"), m_l_sx_rng_off_cntdwn);
		m_l_sx_rng_off_cntdwn--;
	}

	// SX-RNG ON
	if (m_l_sx_rng_on_cntdwn > 0)
	{
		TRACE(_T("SX-RNG ON �J�E���g�_�E��:%d\n"), m_l_sx_rng_on_cntdwn);
		m_l_sx_rng_on_cntdwn--;
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
/*! CTrsControlS34

-# RF-POWER OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::show_rf_power_off()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	// OFF ��
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
		if ((strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") == 0) &&
			(strcmp(m_sz_lan_x_uc, "�ڑ�") == 0) &&
			(strcmp(m_sz_lan_x_rf_if, "�ڑ�") == 0))
		{

			// X-TX RF DET ANS ���m�F����
			if ((strcmp(m_sz_x_tx_det_ans, "OFF") == 0) &&
				(strcmp(m_sz_x_tx_hpa_det_ans, "OFF") == 0))
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
/*! CTrsControlS34

-# RF-POWER ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::show_rf_power_on()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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
	
	// �Δ�������
	if (strcmp(m_sz_lan_s_tx, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);		// ON �{�^��
	}
	else if (((strcmp(m_sz_s_tx_rl, "LOCAL") == 0) || (strcmp(m_sz_rf_on_enbl, "OFF") == 0)) &&
		(strcmp(m_sz_rf_on_det_ans, "OFF") == 0))
	{
		// �����[�g�i�I���j/ ���[�J���\��
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES, COLOR_LOCAL, TRUE);		// ON �{�^��
	}
	else if (strcmp(m_sz_rf_on_det_ans, "OFF") == 0)
	{
		// �����[�g�i�I���j/ �����[�g�i��I���j
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES, COLOR_REMOTE, TRUE);		// ON �{�^��
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
/*! CTrsControlS34

-# NO-USE OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::show_no_use_off()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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
	if ((strcmp(m_sz_s_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) &&
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
/*! CTrsControlS34

-# NO-USE ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::show_no_use_on()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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
	if ((strcmp(m_sz_s_tx_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) &&
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
/*! CTrsControlS34

-# SWEEP-CONT NORMAL STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::show_sweep_cont_stop()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_ss_tcrmod, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_SweepContStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j�\��
		m_SweepContStopBtn.SetColor(COLOR_REMOTES);
	}

	// �����[�g��
	if ((strcmp(m_sz_mod_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_ss_tcrmod, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j�\��
		m_SweepContIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecIncBtn.SetColor(COLOR_REMOTE);
	}
	// ���[�J����
	else
	{
		// ���[�J���\��
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
/*! CTrsControlS34

-# SWEEP-CONT NORMAL INC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::show_sweep_cont_inc()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRDEM));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_s_txsyn, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_SweepContIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j�\��
		m_SweepContIncBtn.SetColor(COLOR_REMOTES);
	}

	// �����[�g��
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j�\��
		m_SweepContStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecIncBtn.SetColor(COLOR_REMOTE);
	}
	// ���[�J����
	else
	{
		// ���[�J���\��
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
/*! CTrsControlS34

-# SWEEP-CONT NORMAL INCDEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::show_sweep_cont_incdec()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRDEM));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_s_txsyn, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_SweepContIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j�\��
		m_SweepContIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// �����[�g��
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j�\��
		m_SweepContStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecIncBtn.SetColor(COLOR_REMOTE);
	}
	// ���[�J����
	else
	{
		// ���[�J���\��
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
/*! CTrsControlS34

-# SWEEP-CONT NORMAL DEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::show_sweep_cont_dec()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRDEM));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_s_txsyn, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_SweepContDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j�\��
		m_SweepContDecBtn.SetColor(COLOR_REMOTES);
	}

	// �����[�g��
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j�\��
		m_SweepContStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecIncBtn.SetColor(COLOR_REMOTE);
	}
	// ���[�J����
	else
	{
		// ���[�J���\��
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
/*! CTrsControlS34

-# SWEEP-CONT NORMAL DECINC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::show_sweep_cont_decinc()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRDEM));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_s_txsyn, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_SweepContDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j�\��
		m_SweepContDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// �����[�g��
	if ((strcmp(m_sz_s_txsyn_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j�\��
		m_SweepContStopBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncBtn.SetColor(COLOR_REMOTE);
		m_SweepContIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepContDecBtn.SetColor(COLOR_REMOTE);
	}
	// ���[�J����
	else
	{
		// ���[�J���\��
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
/*! CTrsControlS34

-# CMD-MOD OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::show_cmd_mod_off()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_s_tx, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_CmdModOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j�\��
		m_CmdModOffBtn.SetColor(COLOR_REMOTES);
	}

	// �����[�g��
	if ((strcmp(m_sz_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j�\��
		m_CmdModOnBtn.SetColor(COLOR_REMOTE);
	}
	// ���[�J����
	else
	{
		// ���[�J���\��
		m_CmdModOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# CMD-MOD ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::show_cmd_mod_on()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_s_tx, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_CmdModOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j�\��
		m_CmdModOnBtn.SetColor(COLOR_REMOTES);
	}

	// �����[�g��
	if ((strcmp(m_sz_uplink_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j�\��
		m_CmdModOffBtn.SetColor(COLOR_REMOTE);
	}
	// ���[�J����
	else
	{
		// ���[�J���\��
		m_CmdModOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_CmdModOffBtn.Invalidate();
	m_CmdModOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# HOLD TONE OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::show_holdtone_off()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_CMDOUT1));
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

	// �����[�g��
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
/*! CTrsControlS34

-# HOLD TONE ON ��ԉ�ʕ\����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::show_holdtone_on()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_CMDOUT1));
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
	// �����[�g��
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
/*! CTrsControlS34

-# COH/INCOH COH ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::show_coh_incoh_coh()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRDEM));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_ss_rarr, "�ڑ�") != 0)
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
/*! CTrsControlS34

-# COH/INCOH INCOH ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::show_coh_incoh_incoh()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_S34_LAN_SS_TCRDEM));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_txsyn, sizeof(m_sz_lan_s_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_ss_rarr, "�ڑ�") != 0)
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
/*! CTrsControlS34

-# RNGING STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::show_rnging_stop()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
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

	if (strcmp(m_sz_lan_ss_rarr, "�ڑ�") != 0)
	{
		// ���[�J���\��
		m_RngingStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j�\��
		m_RngingStopBtn.SetColor(COLOR_REMOTES);
	}

	// SS-RNG �� SX-RNG �� OFF �{�^�����ǂ�����Γ_���̏ꍇ�� ���ɂ���
	if ((strcmp(m_sz_pass_flag, "�p�X��") == 0) &&
		(strcmp(m_sz_lan_ss_rarr, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j�\��
		m_RngingStartBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J���\��
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlS34

-# RNGING START ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::show_rnging_start()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q�� 
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
	strcpy_s(m_sz_lan_ss_rarr, sizeof(m_sz_lan_ss_rarr), "�ڑ�");
#endif

	// SS-RNG �� SX-RNG �� OFF �{�^�����ǂ�����Γ_���̏ꍇ�� ���ɂ���
	if (strcmp(m_sz_lan_ss_rarr, "�ڑ�") == 0)
	{
		// �����[�g�i�I���j�\��
		m_RngingStartBtn.SetColor(COLOR_REMOTES);
	}
	else
	{
		// ���[�J���\��
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}

	if ((strcmp(m_sz_lan_ss_rarr, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j�\��
		m_RngingStopBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J���\��
		m_RngingStopBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlS34

-# HOLD TONE ON ��ԉ�ʕ\����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlS34::printlg_endtime(CString str)
{
	// �Ď��f�[�^�擾

	// ���ݎ����擾

	// �X�C�[�v�����\�莞���Z�o

	// opelog�o��
}

void CTrsControlS34::OnClose()
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

void CTrsControlS34::CloseDlg()
{
	KillTimer(TID_CONDITION);
	KillTimer(TID_COUNTDOWN);

	// �E�B���h�E����郁�b�Z�[�W
	theApp.m_pMainWnd->PostMessageW(eMessage_WindowClose, 0, (LPARAM)this);
}
