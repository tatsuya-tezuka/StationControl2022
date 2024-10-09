#pragma once
#include "afxwin.h"
#include "ColorButtonEx2.h"
#include "MFCTabCtrlEx.h"


// CTrsControlS20 �_�C�A���O

#define MON_S20_PRIV_SSOC			"CTRL.PRIV_SSOC"			// ���͌��ǐ��䌠�擾���
#define MON_S20_PRIV_STTN			"CTRL.PRIV_STTN"			// �o��ǐ��䌠�擾���
#define MON_S20_RF_ON_ENBL			"S-TXRF.IF_RF_ENABLE"		// S-TX RF ON ENABLE ���
#define MON_S20_NO_USE_ENBL			"S-TXRF.IF_NO_USE_ENABLE"	// S-TX RF-TRIP NOUSE ON ENABLE ���
#define MON_S20_UPLINK_CONT			"CTRL.S_TX_TOTAL_REMOTE"		// S-TX UP-LINK CONTROL ���
#define MON_S20_RF_ON_DET_ANS		"S-TXRF.IF_RF_OUTPUT_CONT"	// S-TX RF ON DETECT ANSWER ���
#define MON_S20_X_TX_DET_ANS		"X-TX.SSPA_RF_ON_DET"		// X-TX RF ON  ANSWER ���
#define MON_S20_S_TX_RL				"CTRL.S_TX_TOTAL_REMOTE"	// S-TX �� �����[�g�^���[�J�� ���
#define MON_S20_S_TXSYN_RL			"SS-TCRMOD.CONT_MODE"		// S-TXSYN �� �����[�g�^���[�J�� ���
#define MON_S20_ALARM				"CTRL.S_TX_TOTAL_FAULT"		// �A���[���`�F�b�N
#define MON_S20_RF_POWER			"S-TXRF.IF_RF_OUTPUT_CONT"	// S-TX RF-POWER ���
#define MON_S20_NO_USE				"S-TXRF.IF_NO_USE"			// S-TX NO-USE ���
#define MON_S20_SWEEP_CONT			"SS-TCRMOD.SWEEP_ANS"		// S-TXSYN SWEEP-CONT ���
#define MON_S20_SWEEP_CMD			"SS-TCRMOD.SWEEP_CMD_ANS"	// S-TXSYN SWEEP-CMD  ���
#define MON_S20_SWEEP_STS			"SS-TCRMOD.SWEEP_STS"		// S-TXSYN SWEEP-CONT ���
#define	MON_S20_POWER_SEL			"S-TXRF.RF_POWER_SEL"		// �����M�o��-�p���[
#define MON_S20_SS_RNG_CNT			"CTRL.SS_RNG_GOT_CNT"		// SS-RNG ���l��ԁi�����j
#define MON_S20_SS_RNG_NUM			"V-EQU.RNG_NUM"				// SS-RNG ���l��ԁi�E���j
#define MON_S20_SX_RNG_CNT			"CTRL.SX_RNG_GOT_CNT"		// SX-RNG ���l��ԁi�����j
#define MON_S20_SX_RNG_NUM			"V-EQU.RNG_NUM"				// SX-RNG ���l��ԁi�E���j
#define MON_S20_SS_RNG				"V-EQU.RNG_SS"				// SS-RNG ���
#define MON_S20_SX_RNG				"V-EQU.RNG_SX"				// SX-RNG ���
#define MON_S20_COH_INCOH			"V-EQU.COHI"				// COH/INCOH ���
#define MON_S20_RNGING				"CTRL.RNG_FLG"				// RNGING ���
#define MON_S20_LAN_S_TX			"CTRL.LAN_01_0"				// S-TX LAN �ڑ����
#define MON_S20_LAN_X_TX			"CTRL.LAN_04_4"				// X-TX LAN �ڑ����
#define MON_S20_HPA_RF_ON_DET_ANS_X	"X-TX.RF_ON_DET_ANS"		// HPA RF POWER ON DETECT ANSWER ���
#define MON_S20_LAN_SS_TCRMOD		"CTRL.LAN_06_1"				// SS-TCRMOD LAN �ڑ����
#define MON_S20_MOD_UPLINK_CONT		"SS-TCRMOD.CONT_MODE"		// SS-TCRMOD ���
#define MON_S20_LAN_S_TXSYN			"CTRL.LAN_02_4"				// S-TXSYN LAN �ڑ����
#define MON_S20_LAN_SS_RARR			"CTRL.LAN_02_3"				// SS-RARR LAN �ڑ����
#define MON_S20_LAN_SX_RARR			"CTRL.LAN_03_3"				// SX-RARR LAN �ڑ����
#define MON_S20_P_FLG				"CTRL.PASS_FLAG"			// �p�X�^�p���t���O
#define MON_S20_LAN_CMDOUT1			"CTRL.LAN_07_0"				// CMDOUT1 LAN �ڑ����
#define MON_S20_LAN_CMDOUT2			"CTRL.LAN_07_1"				// CMDOUT2 LAN �ڑ����
#define MON_S20_MOD					"SS-TCRMOD.MODULATION"		// MOD ���
#define MON_S20_MOD_X				"SX-TCRMOD.MODULATION"		// MOD ���
#define MON_S20_HOLD_TONE1			"CMDOUT1.HOLDTONE_STAT"		// HOLD-TONE ���
#define MON_S20_HOLD_TONE2			"CMDOUT2.HOLD_TONE"			// HOLD-TONE ���

/*-- �Ď����ڌ��ʑ҂��e���ڃ^�C�����~�b�g�ݒ� --------------------------------------------*/
#define RF_POWER_TIME_S20_LIMIT			10		// RF-POWER �^�C�����~�b�g(s)�ݒ�
#define NO_USE_TIME_S20_LIMIT			300		// NO-USE �^�C�����~�b�g(s)�ݒ�
#define SWEEP_CONT_TIME_S20_LIMIT		10		// SWEEP-CONT �^�C�����~�b�g(s)�ݒ�
#define TR_SWEEP_CONT_TIME_S20_LIMIT	10		// Tri-SWEEP-CONT �^�C�����~�b�g(s)�ݒ�
#define TR_SWEEP_NUM_TIME_S20_LIMIT		10		// Tri-SWEEP-NUM �^�C�����~�b�g(s)�ݒ�
#define MOD_TIME_S20_LIMIT				10		// MOD �^�C�����~�b�g(s)�ݒ�
#define HOLD_TONE_TIME_S20_LIMIT		10		// HOLD-TONE �^�C�����~�b�g(s)�ݒ�
#define CMD_MOD_TIME_S20_LIMIT			10		// CMD-MOD �^�C�����~�b�g(s)�ݒ�
#define RNG_MOD_TIME_S20_LIMIT			10		// RNG-MOD �^�C�����~�b�g(s)�ݒ�
#define SS_RNG_TIME_S20_LIMIT			10		// SS-RNG �^�C�����~�b�g(s)�ݒ�
#define SX_RNG_TIME_S20_LIMIT			10		// SX-RNG �^�C�����~�b�g(s)�ݒ�
#define COH_INCOH_TIME_S20_LIMIT		10		// COH/INCOH �^�C�����~�b�g(s)�ݒ�
#define RNGING_TIME_S20_LIMIT			3600	// RNGING �^�C�����~�b�g(s)�ݒ�

#define SS_RNG_NUM_DEFAULT				99		// SS-RNG-NUM�f�t�H���g��`

class CTrsControlS20 : public CDialogBase
{
	DECLARE_DYNAMIC(CTrsControlS20)

public:
	CTrsControlS20(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CTrsControlS20();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_TRSCTRL_S20 };

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
	// �^�C���A�E�g�J�E���g�_�E���ϐ���`
	// RF-POWER �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long m_l_rf_power_off_cntdwn;
	long m_l_rf_power_on_cntdwn;

	// NO-USE �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long m_l_no_use_off_cntdwn;
	long m_l_no_use_on_cntdwn;

	// SWEEP-CONT �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long m_l_sweep_cont_stop_cntdwn;
	long m_l_sweep_cont_inc_cntdwn;
	long m_l_sweep_cont_incdec_cntdwn;
	long m_l_sweep_cont_dec_cntdwn;
	long m_l_sweep_cont_decinc_cntdwn;

	// SS-RNG �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long m_l_ss_rng_off_cntdwn;
	long m_l_ss_rng_on_cntdwn;

	// SX-RNG �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long m_l_sx_rng_off_cntdwn;
	long m_l_sx_rng_on_cntdwn;

	// COH/INCOH �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long m_l_coh_incoh_coh_cntdwn;
	long m_l_coh_incoh_incoh_cntdwn;

	// RNGING �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long m_l_rnging_stop_cntdwn;
	long m_l_rnging_start_cntdwn;

	// MOD �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long m_l_mod_off_cntdwn1;
	long m_l_mod_on_cntdwn1;
	long m_l_mod_off_cntdwn2;
	long m_l_mod_on_cntdwn2;

	// HOLD-TONE �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long m_l_hold_tone_off_cntdwn1;
	long m_l_hold_tone_on_cntdwn1;
	long m_l_hold_tone_off_cntdwn2;
	long m_l_hold_tone_on_cntdwn2;

	long    m_l_inflg;						// �Ď����ڗL���t���O
	int		m_i_priv_flag;					// ���䌠��ԃt���O
	long	m_l_flg_ctrl_sweep;				// 1shot-sweep
	int		m_i_sweep_flag;					// SWEEP������s�t���O
	double	m_d_ss_rng_cnt;					// SS-RNG ���l��ԁi�����j
	double	m_d_ss_rng_num;					// SS-RNG ���l��ԁi�E���j
	double	m_d_sx_rng_cnt;					// SX-RNG ���l��ԁi�����j
	double	m_d_sx_rng_num;					// SX-RNG ���l��ԁi�E���j


	char	m_sz_hostname[128];				// �z�X�g��
	char	m_sz_priv_ssoc[32];				// ���͌��ǐ��䌠�擾���
	char	m_sz_priv_sttn[32];				// �o��ǐ��䌠�擾���
	char	m_sz_rf_on_enbl[30];			// RF POWER ON ENABLE ���
	char    m_sz_no_use_enbl[30];			// RF-TRIP NOUSE ON ENABLE ���
	char    m_sz_uplink_cont[30];			// UP-LINK CONTROL ���
	char    m_sz_rf_on_det_ans[30];			// RF POWER ON DETECT ANSWER ��� (S-TX)
	char    m_sz_x_tx_det_ans[30];			// RF POWER ON DETECT ANSWER ��� (X-TX)
	char    m_sz_s_tx_rl[30];				// S-TX �� �����[�g�^���[�J�� ���
	char    m_sz_s_txsyn_rl[30];			// S-TXSYN �� �����[�g�^���[�J�� ���
	char    m_sz_alarm[30];					// �A���[���`�F�b�N
	char    m_sz_rf_power[30];				// RF-POWER ���
	char    m_sz_no_use[30];				// NO-USE ���
	char    m_sz_sweep_cont[30];			// SWEEP-CONT ��� 
	char    m_sz_sweep_cmd[30];				// SWEEP-CONT ���
	char    m_sz_sweep_sts[30];				// SWEEP-CONT ���
	char	m_sz_data_txpower[32];			// RF_POWER_SEL ���
	char	m_sz_ss_rng[30];				// SS-RNG ���
	char	m_sz_sx_rng[30];				// SX-RNG ���
	char	m_sz_coh_incoh[30];				// COH/INCOH ���
	char	m_sz_rnging[30];				// RNGING ���
	char    m_sz_lan_s_tx[30];
	char    m_sz_lan_x_tx[30];
	char    m_sz_x_tx_hpa_det_ans[30];
	char    m_sz_lan_ss_tcrmod[30];
	char    m_sz_mod_uplink_cont[30];
	char    m_sz_lan_s_txsyn[30];
	char    m_sz_lan_ss_rarr[30];
	char    m_sz_lan_sx_rarr[30];
	char	m_sz_pass_flag[30];				// �p�X�^�p���t���O
	char    m_sz_lan_cmdout1[30];
	char    m_sz_lan_cmdout2[30];
	char    m_sz_mod[30];					// MOD���
	char    m_sz_mod_x[30];					// MOD���
	char    m_sz_hold_tone1[30];
	char    m_sz_hold_tone2[30];

	// DEBUG�p >>>
	int		m_DebugRfPower;
	int		m_DebugNoUse;
	int		m_DebugSweepCont;
	int		m_DebugSSRng;
	int		m_DebugSXRng;
	int		m_DebugCohIncoh;
	int		m_DebugRnging;
	int		m_DebugSSModulation;
	int		m_DebugSSHoldTone;
	int		m_DebugSXModulation;
	int		m_DebugSXHoldTone;
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
	void show_sweep_cont_stop();
	void show_sweep_cont_inc();
	void show_sweep_cont_incdec();
	void show_sweep_cont_dec();
	void show_sweep_cont_decinc();
	void show_ss_rng_off();
	void show_ss_rng_on();
	void show_sx_rng_off();
	void show_sx_rng_on();
	void show_coh_incoh_coh();
	void show_coh_incoh_incoh();
	void show_rnging_stop();
	void show_rnging_start();
	void show_ss_modulation_off();
	void show_ss_modulation_on();
	void show_ss_hold_tone_off();
	void show_ss_hold_tone_on();
	void show_sx_modulation_off();
	void show_sx_modulation_on();
	void show_sx_hold_tone_off();
	void show_sx_hold_tone_on();
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
	CColorButtonEx2 m_SweepContStopBtn;
	CColorButtonEx2 m_SweepContIncBtn;
	CColorButtonEx2 m_SweepContIncDecBtn;
	CColorButtonEx2 m_SweepContDecBtn;
	CColorButtonEx2 m_SweepContDecIncBtn;
	CColorButtonEx2 m_TxPwrPowerBtn;
	CColorButtonEx2 m_SSRngOffBtn;
	CColorButtonEx2 m_SSRngOnBtn;
	CColorButtonEx2 m_CohIncohCohBtn;
	CColorButtonEx2 m_CohIncohInCohBtn;
	CColorButtonEx2 m_SXRngOffBtn;
	CColorButtonEx2 m_SXRngOnBtn;
	CColorButtonEx2 m_RangeStopBtn;
	CColorButtonEx2 m_RangeStartBtn;
	CColorButtonEx2 m_SSModulationOffBtn;
	CColorButtonEx2 m_SSModulationOnBtn;
	CColorButtonEx2 m_SSHoldToneOffBtn;
	CColorButtonEx2 m_SSHoldToneOnBtn;
	CColorButtonEx2 m_SXModulationOffBtn;
	CColorButtonEx2 m_SXModulationOnBtn;
	CColorButtonEx2 m_SXHoldToneOffBtn;
	CColorButtonEx2 m_SXHoldToneOnBtn;
	virtual BOOL OnInitDialog();
	CString m_strTxPwrPower;
	afx_msg void OnBnClickedTrsctrlTxprPBtn();
	afx_msg void OnBnClickedTrsctrlSsTcrCheck();
	afx_msg void OnBnClickedTrsctrlSxTcrCheck();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedTrsctrlRfOffBtn();
	afx_msg void OnBnClickedTrsctrlRfOnBtn();
	afx_msg void OnBnClickedTrsctrlSweetStopBtn();
	afx_msg void OnBnClickedTrsctrlSweetIncBtn();
	afx_msg void OnBnClickedTrsctrlSweetIncdecBtn();
	afx_msg void OnBnClickedTrsctrlSweetDecBtn();
	afx_msg void OnBnClickedTrsctrlSweetDecincBtn();
	afx_msg void OnBnClickedTrsctrlNouseOffBtn();
	afx_msg void OnBnClickedTrsctrlNouseOnBtn();
	afx_msg void OnBnClickedTrsctrl20SsrngOffBtn();
	afx_msg void OnBnClickedTrsctrl20SsrngOnBtn();
	afx_msg void OnBnClickedTrsctrl20CohincohCohBtn();
	afx_msg void OnBnClickedTrsctrl20CohincohIncohBtn();
	afx_msg void OnBnClickedTrsctrlSxrngOffBtn();
	afx_msg void OnBnClickedTrsctrlSxrngOnBtn();
	afx_msg void OnBnClickedTrsctrlRangeStopBtn();
	afx_msg void OnBnClickedTrsctrlRangeStartBtn();
	afx_msg void OnBnClickedTrsctrlSsMdlOffBtn();
	afx_msg void OnBnClickedTrsctrlSsMdlOnBtn();
	afx_msg void OnBnClickedTrsctrlSsHldtOffBtn();
	afx_msg void OnBnClickedTrsctrlSsHldtOnBtn();
	afx_msg void OnBnClickedTrsctrlSxMdlOffBtn();
	afx_msg void OnBnClickedTrsctrlSxMdlOnBtn();
	afx_msg void OnBnClickedTrsctrlSxHldtOffBtn();
	afx_msg void OnBnClickedTrsctrlSxHldtOnBtn();
	CButton m_TcrCmdOut1ChkBtn;
	CButton m_TcrCmdOut2ChkBtn;
	CGroupBoxEx m_grTxControl;
	CGroupBoxEx m_grRfPower;
	CGroupBoxEx m_grNoUse;
	CGroupBoxEx m_grSweepCont;
	CGroupBoxEx m_grTxPwr;
	CGroupBoxEx m_grRngControl;
	CGroupBoxEx m_grSSRng;
	CGroupBoxEx m_grCohIncoh;
	CGroupBoxEx m_grSXRng;
	CGroupBoxEx m_grRange;
	CGroupBoxEx m_grSS;
	CGroupBoxEx m_grSSModulation;
	CGroupBoxEx m_grSSHoldTone;
	CGroupBoxEx m_grSX;
	CGroupBoxEx m_grSXModulation;
	CGroupBoxEx m_grSXHoldTone;
	afx_msg void OnClose();
};
