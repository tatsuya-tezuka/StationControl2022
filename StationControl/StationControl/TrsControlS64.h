#pragma once
#include "ColorButtonEx2.h"
#include "afxwin.h"
#include "MFCTabCtrlEx.h"


// CTrsControlS64 �_�C�A���O

// �Ď����ڃf�[�^�x�[�X�A�N�Z�X�p��`
#define MON_S64_PRIV_SSOC			"CTRL.PRIV_SSOC"				// ���͌��ǐ��䌠�擾���
#define MON_S64_PRIV_STTN			"CTRL.PRIV_STTN"				// �o��ǐ��䌠�擾���
#define MON_S64_RF_ON_ENBL			"S-TXRF.RF_ON_ENABLE"			// S-TX RF ON ENABLE ���
#define MON_S64_RF_OFF_ENBL			"S-TXRF.RF_OFF_ENABLE"			// S-TX RF OFF ENABLE ���
#define MON_S64_UPLINK_CONT			"CTRL.S_TX_TOTAL_REMOTE"		// S-TX CONTROL MODE ���
#define MON_S64_X_TX_DET_ANS		"X-TX.RF_ON_ANS"				// X-TX RF ON  ANSWER ���
#define MON_S64_S_TX_RL				"CTRL.S_TX_TOTAL_REMOTE"		// S-TX �� �����[�g�^���[�J�� ���
#define MON_S64_S_TXSYN_RL			"S-UCSYN.CONT_MODE"				// S-TXSYN �� �����[�g�^���[�J�� ���
#define MON_S64_ALARM				"CTRL.ALARM_00_6"				// �A���[���`�F�b�N
#define MON_S64_RF_POWER			"S-TXRF.IF_RF_OUTPUT_CONT"		// S-TX RF-POWER ���
#define MON_S64_NO_USE				"S-TXRF.IF_NO_USE"				// S-TX NO-USE ���
#define MON_S64_SWEEP_CONT			"S-UCSYN.SWEEP_CONT"			// S-TXSYN SWEEP-CONT ���
#define MON_S64_TR_SWEEP_CONT		"S-UCSYN.TR_SWEEP_CONT"			// S-TXSYN Tri-SWEEP-CONT ���
#define MON_S64_TR_SWEEP_NUM		"S-UCSYN.SWEEP_CYCLE"			// S-TXSYN Tri-SWEEP-NUM  ���
#define MON_S64_CMD_MOD				"SS-TCRMOD.MODULATION"			// CMD-MOD ���
#define MON_S64_RNG_MOD				"S-TX.RNG_MOD"					// RNG-MOD ���
#define MON_S64_SS_RNG				"V-EQU.RNG_SS"					// SS-RNG ���
#define MON_S64_COH_INCOH			"V-EQU.COHI"					// COH/INCOH ���
#define MON_S64_RNGING				"CTRL.RNG_FLG"					// RNGING ���
#define MON_S64_HOLD_TONE			"CMDOUT.HOLDTONE_STAT"			// HOLD TONE ���
#define MON_S64_CMD_MOD_IDX			"SS-TCRMOD.CMD_MOD_INDEX"		// CMD-MOD INDEX
#define MON_S64_RNG_MOD_IDX			"SS-TCRMOD.RNG_MOD_INDEX"		// RNG-MOD INDEX
#define MON_S64_DATA_SOURCE			"SS-TCRMOD.DATA_SOURCE"			// DATA_SOURCE
#define MON_S64_LAN_S_TX			"CTRL.LAN_01_0"					// S-TX LAN �ڑ����
#define MON_S64_LAN_S_TXSYN			"CTRL.LAN_01_2"					// S-TXSYN LAN �ڑ����
#define MON_S64_LAN_V_EQU			"CTRL.LAN_06_6"					// ���z�ݔ� LAN �ڑ����
#define MON_S64_LAN_RNG				"CTRL.LAN_02_5"					// RNG LAN �ڑ����
#define MON_S64_RNG_CONT			"SS-TCRDEM.CONT_MODE"			// RNG CONTROL ���
#define MON_S64_LAN_CMDOUT1			"CTRL.LAN_04_1"					// CMDOUT1 LAN �ڑ����
#define MON_S64_SAT_NAME			"V-EQU.BBE_SATNAME"				// BBE SAT NAME
#define MON_X_TX_DET_ANS			"X-TX.RF_ON_ANS"				// X-TX RF ON  ANSWER ���
#define MON_S64_LAN_X_TX			"CTRL.LAN_04_4"					// X-TX LAN �ڑ����
#define MON_S64_LAN_X_UL_CNT		"CTRL.LAN_05_6"					// X-UL_CNT LAN �ڑ����

//--------------------------------------------------------
// �Ď����ڌ��ʑ҂��e���ڃ^�C�����~�b�g�ݒ�
#define RF_POWER_TIME_S64_LIMIT			10		// RF-POWER �^�C�����~�b�g(s)�ݒ�
#define NO_USE_TIME_S64_LIMIT			300		// NO-USE �^�C�����~�b�g(s)�ݒ�
#define SWEEP_CONT_TIME_S64_LIMIT		10		// SWEEP-CONT �^�C�����~�b�g(s)�ݒ�
#define TR_SWEEP_CONT_TIME_S64_LIMIT	10		// Tri-SWEEP-CONT �^�C�����~�b�g(s)�ݒ�
#define TR_SWEEP_NUM_TIME_S64_LIMIT		10		// Tri-SWEEP-NUM �^�C�����~�b�g(s)�ݒ�
#define CMD_MOD_TIME_S64_LIMIT			10		// CMD-MOD �^�C�����~�b�g(s)�ݒ�
#define RNG_MOD_TIME_S64_LIMIT			10		// RNG-MOD �^�C�����~�b�g(s)�ݒ�
#define SS_RNG_TIME_S64_LIMIT			10		// SS-RNG �^�C�����~�b�g(s)�ݒ�
#define SX_RNG_TIME_S64_LIMIT			10		// SX-RNG �^�C�����~�b�g(s)�ݒ�
#define COH_INCOH_TIME_S64_LIMIT		10		// COH/INCOH �^�C�����~�b�g(s)�ݒ�
#define RNGING_TIME_S64_LIMIT			10		// RNGING �^�C�����~�b�g(s)�ݒ�
#define HOLDTONE_TIME_S64_LIMIT			10		// HOLD TONE �^�C�����~�b�g(s)�ݒ�
#define CMD_MOD_IDX_TIME_S64_LIMIT		10		// CMD MOD INDEX �^�C�����~�b�g(s)�ݒ�
#define RNG_MOD_IDX_TIME_S64_LIMIT		10		// RNG MOD INDEX �^�C�����~�b�g(s)�ݒ�
#define CAR_MOD_IDX_TIME_S64_LIMIT		10		// CMD+RNG MOD INDEX �^�C�����~�b�g(s)�ݒ�
#define RNG_ROUTE_TIME_S64_LIMIT		10		// RNG ROUTE�؂�ւ� �^�C�����~�b�g(s)�ݒ�
#define TRK_DATA_TIME_S64_LIMIT			10		// �ǐՃf�[�^�z�M�؂�ւ� �^�C�����~�b�g(s)�ݒ�
#define TLM_DATA_TIME_S64_LIMIT			10		// �e�����g���f�[�^�z�M�؂�ւ� �^�C�����~�b�g(s)�ݒ�

class CTrsControlS64 : public CDialogBase
{
	DECLARE_DYNAMIC(CTrsControlS64)

public:
	CTrsControlS64(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CTrsControlS64();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_TRSCTRL_S64 };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
protected:
	long    m_l_inflg;                  // �Ď����ڗL���t���O
	int		m_i_priv_flag;				// ���䌠��ԃt���O
	int		m_i_sweep_flag;				// SWEEP������s�t���O�ǉ�
	int		m_i_nor_sweep_cont_mode;
	int		m_i_tri_sweep_cont_mode;
	long	m_l_tri_sweep_flag;			// �O�p�X�C�[�v���䊮���t���O
	double	m_d_tri_sweep_num;          // S-TXSYN Tri-SWEEP NUM

	char	m_sz_hostname[128];			// �z�X�g��
	char	m_sz_priv_ssoc[32];
	char	m_sz_priv_sttn[32];
	char    m_sz_rf_on_enbl[30];		// RF POWER ON ENABLE ���
	char    m_sz_rf_off_enbl[30];		// RF POWER OFF ENABLE ���
	char    m_sz_no_use_enbl[30];		// RF-TRIP NOUSE ON ENABLE ���
	char    m_sz_uplink_cont[30];		// UP-LINK CONTROL ���
	char    m_sz_rf_on_det_ans[30];		// RF POWER ON DETECT ANSWER ��� (S-TX)
	char    m_sz_x_tx_det_ans[30];		// RF POWER ON DETECT ANSWER ��� (X-TX)
	char    m_sz_s_tx_rl[30];			// S-TX �� �����[�g�^���[�J�� ���
	char    m_sz_s_txsyn_rl[30];		// S-TXSYN �� �����[�g�^���[�J�� ���
	char    m_sz_alarm[30];				// �A���[���`�F�b�N
	char    m_sz_rf_power[30];			// RF-POWER ���
	char    m_sz_no_use[30];			// NO-USE ���
	char    m_sz_nor_sweep_cont[30];	// SWEEP-CONT ���
	char    m_sz_nor_sweep_cmd[30];		// SWEEP-CONT ���
	char    m_sz_nor_sweep_sts[30];		// SWEEP-CONT ���
	char    m_sz_tri_sweep_cont[30];	// SWEEP-CONT ���
	char    m_sz_tri_sweep_cmd[30];		// SWEEP-CONT ���
	char    m_sz_tri_sweep_sts[30];		// SWEEP-CONT ���
	char    m_sz_cmd_mod[30];			// CMD-MOD ���
	char    m_sz_rng_mod[30];			// RNG-MOD ���
	char	m_sz_ss_rng[30];			// SS-RNG ���
	char    m_sz_lan_vequ[30];
	char    m_sz_lan_rng[30];
	char	m_sz_rng_cont[30];			// RNG CONT ���


	// RF-POWER �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long	m_l_rf_power_off_cntdwn;
	long	m_l_rf_power_on_cntdwn;

	// NO-USE �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long 	m_l_no_use_off_cntdwn;
	long 	m_l_no_use_on_cntdwn;

	// SWEEP-CONT �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long 	m_l_nor_sweep_cont_stop_cntdwn;
	long 	m_l_nor_sweep_cont_inc_cntdwn;
	long 	m_l_nor_sweep_cont_incdec_cntdwn;
	long 	m_l_nor_sweep_cont_dec_cntdwn;
	long 	m_l_nor_sweep_cont_decinc_cntdwn;

	// Tri-SWEEP-CONT �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long 	m_l_tri_sweep_cont_stop_cntdwn;
	long 	m_l_tri_sweep_cont_inc_cntdwn;
	long 	m_l_tri_sweep_cont_incdec_cntdwn;
	long 	m_l_tri_sweep_cont_dec_cntdwn;
	long 	m_l_tri_sweep_cont_decinc_cntdwn;

	// Tri-SWEEP-NUM �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long	m_l_tri_sweep_num_cntdwn;
	long	m_l_tri_sweep_num_set;

	// CMD-MOD �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long 	m_l_cmd_mod_off_cntdwn;
	long 	m_l_cmd_mod_on_cntdwn;

	// SS-RNG �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long 	m_l_ss_rng_off_cntdwn;
	long 	m_l_ss_rng_on_cntdwn;

	// SX-RNG �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long 	m_l_sx_rng_off_cntdwn;
	long 	m_l_sx_rng_on_cntdwn;

	// COH/INCOH �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long 	m_l_coh_incoh_coh_cntdwn;
	long 	m_l_coh_incoh_incoh_cntdwn;

	// RNGING �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long 	m_l_rnging_stop_cntdwn;
	long 	m_l_rnging_start_cntdwn;

	// HOLD TONE �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long 	m_l_holdtone_off_cntdwn;
	long 	m_l_holdtone_on_cntdwn;

	// MOD INDEX �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long 	m_l_cmd_mod_idx_cntdwn;
	long 	m_l_rng_mod_idx_cntdwn;
	long 	m_l_car_mod_idx_cntdwn;

	// RNG ROUTE �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long 	m_l_rng_route_cntdwn;
	long 	m_l_trk_data_cntdwn;
	long 	m_l_tlm_data_cntdwn;

	// ���M�L�����A�X�C�[�v�T�C�N����
	long	m_l_sweep_num_cntdwn;
	long	m_l_sweep_num_set;



	double	m_d_ss_rng_cnt;				// SS-RNG ���l��ԁi�����j
	double	m_d_ss_rng_num;				// SS-RNG ���l��ԁi�E���j
	double	m_d_sx_rng_cnt;				// SX-RNG ���l��ԁi�����j
	double	m_d_sx_rng_num;				// SX-RNG ���l��ԁi�E���j
	double  m_d_cmd_mod_idx;			// CMD MOD INDEX
	double  m_d_rng_mod_idx;			// RNG MOD INDEX

	char	m_sz_coh_incoh[30];			// COH/INCOH ���
	char	m_sz_rnging[30];			// RNGING ���
	char	m_sz_pass_flag[30];			// �p�X�^�p���t���O
	char    m_sz_hold_tone[30];
	char    m_sz_data_src[30];			// DATA SOURCE ���
	char    m_sz_rng_route[30];
	char    m_sz_s_tlm_sts[30];
	char	m_sz_data_txpower[32];		// RF_POWER_SEL ���
	char	m_sz_data_ucroute[32];		// RFSW_UC_ROUTE ��� 
	char    m_sz_lan_s_tx[30];
	char    m_sz_lan_x_tx[30];
	char    m_sz_lan_x_ul_cnt[30];
	char    m_sz_x_tx_hpa_det_ans[30];
	char    m_sz_lan_ss_tcrmod[30];
	char    m_sz_mod_uplink_cont[30];
	char    m_sz_lan_s_txsyn[30];
	char    m_sz_lan_ss_rarr[30];
	char    m_sz_lan_sx_rarr[30];
	char    m_sz_lan_cmdout1[30];
	char    m_sz_sat_name[30];
	char    m_sz_dsntcr_rl[30];				// DSN-MODEM �� �����[�g�^���[�J�� ���

	// DEBUG�p >>>
	int		m_DebugRrPower;
	int		m_DebugNoUse;
	int		m_DebugSweepContN;
	int		m_DebugSweepContT;
	int		m_DebugCmdMod;
	int		m_DebugSSRng;
	int		m_DebugCohIncoh;
	int		m_DebugRnging;
	int		m_DebugHoldTone;
	// <<< DEBUG�p

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
protected:
	virtual BOOL UpdateDlg();

	void show_opsc_condition();
	void count_down();

	void show_rf_power_off();
	void show_rf_power_on();
	void show_no_use_off();
	void show_no_use_on();
	void show_nor_sweep_cont_stop();
	void show_nor_sweep_cont_inc();
	void show_nor_sweep_cont_incdec();
	void show_nor_sweep_cont_dec();
	void show_nor_sweep_cont_decinc();
	void show_tri_sweep_cont_stop();
	void show_tri_sweep_cont_inc();
	void show_tri_sweep_cont_incdec();
	void show_tri_sweep_cont_dec();
	void show_tri_sweep_cont_decinc();
	void show_cmd_mod_off();
	void show_cmd_mod_on();
	void show_ss_rng_off();
	void show_ss_rng_on();
	void show_coh_incoh_coh();
	void show_coh_incoh_incoh();
	void show_rnging_stop();
	void show_rnging_start();
	void show_holdtone_off();
	void show_holdtone_on();

	void printlg_endtime(CString str);

	void CloseDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuClose();
	CColorButtonEx2 m_RfPwrOffBtn;
	CColorButtonEx2 m_RfPwrOnBtn;
	CColorButtonEx2 m_NoUseOffBtn;
	CColorButtonEx2 m_NoUseOnBtn;
	CColorButtonEx2 m_SweepContNStopBtn;
	CColorButtonEx2 m_SweepContNIncBtn;
	CColorButtonEx2 m_SweepContNIncDecBtn;
	CColorButtonEx2 m_SweepContNDecBtn;
	CColorButtonEx2 m_SweepContNDecIncBtn;
	CColorButtonEx2 m_SweepContTStopBtn;
	CColorButtonEx2 m_SweepContTIncBtn;
	CColorButtonEx2 m_SweepContTIncDecBtn;
	CColorButtonEx2 m_SweepContTDecBtn;
	CColorButtonEx2 m_SweepContTDecIncBtn;
	CColorButtonEx2 m_SweepNumBtn;
	CColorButtonEx2 m_ModOffBtn;
	CColorButtonEx2 m_ModOnBtn;
	CColorButtonEx2 m_SSRngOffBtn;
	CColorButtonEx2 m_SSRngOnBtn;
	CColorButtonEx2 m_CohIncohCohBtn;
	CColorButtonEx2 m_CohIncohInCohBtn;
	CColorButtonEx2 m_RngingStopBtn;
	CColorButtonEx2 m_RngingStartBtn;
	CColorButtonEx2 m_HoldToneOffBtn;
	CColorButtonEx2 m_HoldToneOnBtn;
	CColorButtonEx2 m_IndexCmdBtn;
	CColorButtonEx2 m_IndexRngBtn;
	CColorButtonEx2 m_IndexCRBtn;
	BOOL m_bSweepContTriangle;
	CString m_strIndexCmd;
	CString m_strIndexRng;
	CString m_strIndexCR;
	CButton m_SweepContTriChkBtn;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTrsctrlSweeptCheck();
	afx_msg void OnBnClickedTrsctrlIdxCmdBtn();
	afx_msg void OnBnClickedTrsctrlIdxRngBtn();
	afx_msg void OnBnClickedTrsctrlIdxCrBtn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedTrsctrlSweepnumBtn();
	afx_msg void OnBnClickedTrsctrlRfOffBtn();
	afx_msg void OnBnClickedTrsctrlRfOnBtn();
	afx_msg void OnBnClickedTrsctrlNouseOffBtn();
	afx_msg void OnBnClickedTrsctrlNouseOnBtn();
	afx_msg void OnBnClickedTrsctrlSweepnStopBtn();
	afx_msg void OnBnClickedTrsctrlSweepnIncBtn();
	afx_msg void OnBnClickedTrsctrlSweepnIncdecBtn();
	afx_msg void OnBnClickedTrsctrlSweepnDecBtn();
	afx_msg void OnBnClickedTrsctrlSweepnDecincBtn();
	afx_msg void OnBnClickedTrsctrlSweeptStopBtn();
	afx_msg void OnBnClickedTrsctrlSweeptIncBtn();
	afx_msg void OnBnClickedTrsctrlSweeptIncdecBtn();
	afx_msg void OnBnClickedTrsctrlSweeptDecBtn();
	afx_msg void OnBnClickedTrsctrlSweeptDecincBtn();
	afx_msg void OnBnClickedTrsctrlModOffBtn();
	afx_msg void OnBnClickedTrsctrlModOnBtn();
	afx_msg void OnBnClickedTrsctrlSsrngOffBtn();
	afx_msg void OnBnClickedTrsctrlSsrngOnBtn();
	afx_msg void OnBnClickedTrsctrlCohincohCohBtn();
	afx_msg void OnBnClickedTrsctrlCohincohIncohBtn();
	afx_msg void OnBnClickedTrsctrlRngingStopBtn();
	afx_msg void OnBnClickedTrsctrlRngingStartBtn();
	afx_msg void OnBnClickedTrsctrlHoldOffBtn();
	afx_msg void OnBnClickedTrsctrlHoldOnBtn();
	CGroupBoxEx m_grTxControl;
	CGroupBoxEx m_grRfPower;
	CGroupBoxEx m_grNoUse;
	CGroupBoxEx m_grSweepContNormal;
	CGroupBoxEx m_grSweepContTriangle;
	CGroupBoxEx m_grMod;
	CGroupBoxEx m_grIndex;
	CGroupBoxEx m_grRngControl;
	CGroupBoxEx m_grSSRng;
	CGroupBoxEx m_grCohIncoh;
	CGroupBoxEx m_grRnging;
	CGroupBoxEx m_grHoldTone;
	afx_msg void OnClose();
};
