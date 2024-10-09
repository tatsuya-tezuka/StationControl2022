#pragma once

#include "afxwin.h"
#include "ColorButtonEx2.h"
#include "MFCTabCtrlEx.h"


// CTrsControlXX �_�C�A���O

// �Ď����ڃf�[�^�x�[�X�A�N�Z�X�p��`
#define MON_XX_COH_INCOH				"V-EQU.COHI"						// COH/INCOH ���
#define MON_XX_RNGING					"CTRL.RNG_FLG"						// RNGING ���
#define MON_XX_X_TX_RL					"X-TX.TX_TOTAL_CONT"				// X-TX �� �����[�g�^���[�J�� ���
#define MON_XX_X_TXSYN_RL				"X-TXSYN.CONT_MODE"					// X-TXSYN �� �����[�g�^���[�J�� ���
#define MON_XX34_X_TXSYN_RL				"X-UC.CONTROL_MODE"					// X-TXSYN �� �����[�g�^���[�J�� ���
#define MON_XX_ALARM_X					"X-TX.TOTAL_FAULT"					// �A���[���`�F�b�N
#define MON_XX_RF_POWER_X				"X-UL_CNT.RF_ON_CONT"				// RF-POWER ���
#define MON_XX_NO_USE_X					"X-UL_CNT.RF_TRIP_NO_USE"			// NO-USE ���
#define MON_XX_RATE_AID_X				"X-TXSYN.RATE_AID"					// RATE AID ���
//#define MON_XX_DOPP_PRE					"XX-DSNTCRFC.TX_DOPP_AID"			// DOPP PRE���
#define MON_XX_DOPP_PRE					"CTRL.AID_UL"						// DOPP PRE���
#define MON_XX_CMD_MOD_X				"DSNTCR.TX_IF_MODULATION"			// CMD-MOD ���
#define MON_XX_CMD_IF_X					"DSNTCR.TX_IF_OUTPUT"				// CMD-IF ���
#define MON_XX_RNG_MOD_X				"X-TX.RNG_MOD"						// RNG-MOD ���
#define MON_XX_CONT_FLAG_X				"XX-RARR.DEEP_NEAR"					// CONTINUATION"�\���L�����
#define MON_XX_XX_RARR_CONT				"XX-RARR.CONT_MODE"					// XX-RARR CONT MODE ���
#define MON_XX_XX_RNG_CNT				"DSNTCR_RARR.RNG_GET_DATA_NUM"		// XX-RNG ���l��ԁi�����j
#define MON_XX_XX_RNG_NUM				"V-EQU.RNG_NUM"						// XX-RNG ���l��ԁi�E���j
#define MON_XX_P_FLG					"CTRL.PASS_FLAG"					// �p�X�^�p���t���O
#define MON_XX_RF_ON_ENBL_X				"X-UL_CNT.RF_ON_ENABLE"				// RF POWER ON ENABLE ���
#define MON_XX_NO_USE_ENBL_X			"X-UL_CNT.RF_TRIP_NO_USE_ENA"		// RF-TRIP NOUSE ON ENABLE ���
#define MON_XX_UPLINK_CONT_X			"X-UL_CNT.UP_LINK_CONT_REMOTE"		// UP-LINK CONTROL ���
#define MON_XX_RF_ON_DET_ANS_X			"X-UL_CNT.RF_DET"					// RF POWER ON DETECT ANSWER ��� (X-TX)
#define MON_XX34_RF_ON_DET_ANS_X		"X-UL_CNT.SSPA_RF_ON_DET_ANS"		// RF POWER ON DETECT ANSWER ��� (X-TX)
#define MON_XX_RF_ON_DET_ANS_S			"S-TXRF.IF_RF_OUTPUT_CONT"			// RF POWER ON DETECT ANSWER ��� (S-TX)
#define MON_XX34_RF_ON_DET_ANS_S		"S-TX.RF_ON_DET_ANS"				// RF POWER ON DETECT ANSWER ��� (S-TX)
#define MON_XX_LAN_X_TX					"CTRL.LAN_03_0"						// X-TX LAN �ڑ����
#define MON_XX_LAN_S_TX					"CTRL.LAN_01_0"						// S-TX LAN �ڑ����
#define MON_XX34_LAN_S_TX				"CTRL.LAN_00_6"						// S-TX LAN �ڑ����
#define MON_XX_LAN_X_TXSYN				"CTRL.LAN_00_1"						// DSN-MODEM LAN �ڑ����
#define MON_XX34_LAN_X_TXSYN			"CTRL.LAN_07_2"						// DSN-MODEM LAN �ڑ����
#define MON_XX_LAN_XX_RARR				"CTRL.LAN_00_2"						// DSN-MODEM LAN �ڑ����
#define MON_XX34_LAN_XX_RARR			"CTRL.LAN_07_3"						// DSN-MODEM LAN �ڑ����
#define MON_XX_LAN_V_EQU				"CTRL.LAN_06_6"						// ���z�ݔ� LAN �ڑ����
#define MON_XX34_LAN_V_EQU				"CTRL.LAN_05_0"						// ���z�ݔ� LAN �ڑ����
#define MON_XX_PRIV_SSOC				"CTRL.PRIV_SSOC"					// ���͌��ǐ��䌠�擾���
#define MON_XX_PRIV_STTN				"CTRL.PRIV_STTN"					// �o��ǐ��䌠�擾���
#define MON_XX_CMD_MOD_IDX				"DSNTCR.TX_MOD_INDEX"				// CMD-MOD INDEX
#define MON_XX_RNG_MOD_IDX				"DSNTCR.RNG_MOD_INDEX"				// RNG-MOD INDEX
#define MON_XX_DATA_SOURCE				"DSNTCR.CMD_DATA_SOURCE"			// DATA_SOURCE
#define MON_XX_NORM_REP					"DSNTCR.RNG_MODE"					// �M�����z�Րؑ֐ݒ�i�]���^/�Đ��^�j
#define MON_XX_LAN_XX_RRNG				"CTRL.LAN_00_2"						// DSN-MODEM LAN �ڑ����
#define MON_XX34_LAN_XX_RRNG			"CTRL.LAN_07_3"						// DSN-MODEM LAN �ڑ����
#define MON_XX_XX_RRNG_CONT				"DSNTCR.CONT_MODE"					// DSN-MODEM CONT MODE ���
#define MON_XX_CTRL_SAT_NAME			"CTRL.SAT_NAME"						// �q������
#define MON_XX_XX_TCRMOD_RL				"XX-TCRMOD.CONT_MODE"				// XX-TCRMOD �� �����[�g�^���[�J�� ���
#define MON_XX_XX_TCRMOD_STS			"XX-TCRMOD.MODULATION"				// XX-TCRMOD MODULATION ���
#define MON_XX_LAN_XX_TCRMOD			"CTRL.LAN_05_4"						// XX-TCRMOD LAN �ڑ����
#define MON_XX_LAN_X_CMDOUT				"CTRL.LAN_00_7"						// XX-TCRMOD LAN �ڑ����
#define MON_XX34_LAN_X_CMDOUT			"CTRL.LAN_07_7"						// XX-TCRMOD LAN �ڑ����
#define MON_XX_MOD_IN_SEL				"V-EQU.DSN_TCRMOD_SEL"				// MOD/TCRMOD �I�����
#define MON_XX_MOD_OUT_SEL				"V-EQU.DSN_TCRMOD_SEL"				// MOD/TCRMOD �I�����
#define MON_XX_XPA_SEL					"X-TX.XPA_SEL_SSPA_HPA_SEL_ANS"		// HPA/SSPA �I�����
#define MON_XX34_XPA_SEL				"X-UL_CNT.SSPA_HPA_SEL_ANS"			// HPA/SSPA �I�����
#define MON_XX_HPA_RF_POWER_X			"X-TX.HPA_RF_ON_CONT_ANS"			// HPA RF-POWER ���
#define MON_XX34_HPA_RF_POWER_X			"X-UL_CNT.RF_ON_CONT-HPA"			// HPA RF-POWER ���
#define MON_XX_HPA_RF_ON_DET_ANS_X		"X-TX.RF_ON_DET_ANS"				// HPA RF POWER ON DETECT ANSWER ���
#define MON_XX34_HPA_RF_ON_DET_ANS_X	"X-UL_CNT.RF_DET"					// HPA RF POWER ON DETECT ANSWER ���
#define MON_XX_HPA_RF_ON_ENBL_X			"X-TX.RF_ON_ENBL"					// RF POWER ON ENABLE ���
#define MON_XX34_HPA_RF_ON_ENBL_X		"X-UL_CNT.RF_ON_ENABLE"				// RF POWER ON ENABLE ���
#define MON_XX_HOLD_TONE_STS_X			"X-CMDOUT2.HOLDTONE_STAT"			// X-CMDOUT HOLD TONE ���
#define MON_XX54_HOLD_TONE_STS_X		"X-CMDOUT.HOLDTONE_STAT"			// X-CMDOUT HOLD TONE ���
#define MON_XX_RX_RNG_CNT				"CTRL.RX_RNG_GOT_CNT"				// RX-RNG ���l��ԁi�����j
#define MON_XX_TC_SWEEP_CONT			"DSNTCR.TX_CARR_SWP"				// S-TXSYN SWEEP-CONT ���
#define MON_XX_TC_SWEEP_CMD				"DSNTCR.TX_CARR_SWP_MODE"			// S-TXSYN SWEEP-CMD  ���
#define MON_XX_TC_SWEEP_STS				"DSNTCR.TX_CARR_SWP_STS"			// S-TXSYN SWEEP-CONT ���
#define MON_XX_SC_SWEEP_CONT			"DSNTCR.TX_SC_SWP"					// S-TXSYN SWEEP-CONT ���
#define MON_XX_SC_SWEEP_CMD				"DSNTCR.TX_SC_SWP_MODE"				// S-TXSYN SWEEP-CMD  ���
#define MON_XX_SC_SWEEP_STS				"DSNTCR.TX_SC_SWP_STS"				// S-TXSYN SWEEP-CONT ���
#define MON_XX_DSNTCR_RL				"DSNTCR.CONT_MODE"					// DSN-MODEM �� �����[�g�^���[�J�� ���
#define MON_XX_SWEEP_NUM				"DSNTCR.TX_CARR_SWP_CYCLE"			// SWEEP-NUM  ���
#define MON_XX_HPA_SEL					"X-TX.HPA_ANS"						// HPA SEL ��� (X-TX)
#define MON_XX_RF_ON_DET_ANS_X_H2		"X-UL_CNT.HPA2_RF_DET"				// RF POWER ON DETECT ANSWER ��� (X-TX)
#define MON_XX_X_UL_CNT_RL				"X-UL_CNT.X-TX_TOTAL_REMOTE"		// X-UL_CNT �� �����[�g�^���[�J�� ���
#define MON_XX_X_UC_RL					"X-UC.CONTROL_MODE"					// X-UC �� �����[�g�^���[�J�� ���
#define MON_XX_X_RF_IF_RL				"X-RF_IF.CONTROL_MODE"				// X-RF_IF �� �����[�g�^���[�J�� ���
#define MON_XX_LAN_X_UL_CNT				"CTRL.LAN_05_6"						// X-UL_CNT LAN �ڑ����
#define MON_XX34_LAN_X_UL_CNT			"CTRL.LAN_04_4"						// X-UL_CNT LAN �ڑ����
#define MON_XX_LAN_X_UC					"CTRL.LAN_05_7"						// X-UC LAN �ڑ����
#define MON_XX34_LAN_X_UC				"CTRL.LAN_04_3"						// X-UC LAN �ڑ����
#define MON_XX_LAN_X_RF_IF				"CTRL.LAN_07_2"						// X-RF_IF LAN �ڑ����
#define MON_XX34_LAN_X_RF_IF			"CTRL.LAN_04_2"						// X-RF_IF LAN �ڑ����
#define MON_XX_SAT_NAME					"V-EQU.BBE_SATNAME"					// BBE SAT NAME

#define MON_XX54_RF_ON_ENBL_X			"ANT.LOW_EL"						// RF POWER ON ENABLE ���
#define MON_XX54_RF_ON_ENBL_SSPA		"X-SSPA.TX_ON_READY"					// RF POWER ON ENABLE ���
#define MON_XX54_NO_USE_ENBL_X			"SSPA.RF_CUT_NO_USE"				// RF-TRIP NOUSE ON ENABLE ���
#define MON_XX54_NO_USE_X				"SSPA.RF_CUT_NO_USE"				// NO-USE ���
#define MON_XX54_X_TX_IF_RL				"X-TX_IF.CONTROL_MODE"				// X-TX_IF �� �����[�g�^���[�J�� ���
#define MON_XX54_RF_POWER_X				"X-TX_IF.TX1_OUTPUT"				// RF-POWER ���
#define MON_XX54_RF_POWER_SSPA			"X-SSPA.TX_ON/OFF"					// RF-POWER ���
#define MON_XX54_LAN_SSPA				"CTRL.LAN_05_6"						// SSPA LAN �ڑ����
#define MON_XX54_LAN_X_TX_IF			"CTRL.LAN_07_2"						// X-TX_IF LAN �ڑ����
#define MON_XX54_SSPA_RL				"X-SSPA.CONT_MODE"					// SSPA �� �����[�g�^���[�J�� ���


// �Ď����ڌ��ʑ҂��e���ڃ^�C�����~�b�g�ݒ�
#define RF_POWER_TIME_XX_LIMIT		10		// RF-POWER �^�C�����~�b�g(s)�ݒ�
#define NO_USE_TIME_XX_LIMIT		300		// NO-USE �^�C�����~�b�g(s)�ݒ�
#define SWEEP_CONT_TIME_XX_LIMIT	10		// SWEEP-CONT �^�C�����~�b�g(s)�ݒ�
#define CMD_MOD_TIME_XX_LIMIT		10		// CMD-MOD �^�C�����~�b�g(s)�ݒ�
#define RNG_MOD_TIME_XX_LIMIT		10		// RNG-MOD �^�C�����~�b�g(s)�ݒ�
#define SS_RNG_TIME_XX_LIMIT		10		// SS-RNG �^�C�����~�b�g(s)�ݒ�
#define SX_RNG_TIME_XX_LIMIT		10		// SX-RNG �^�C�����~�b�g(s)�ݒ�
#define COH_INCOH_TIME_XX_LIMIT		10		// COH/INCOH �^�C�����~�b�g(s)�ݒ�
#define RNGING_TIME_XX64_LIMIT		10		// RNGING �^�C�����~�b�g(s)�ݒ� (�]���^)
#define REP_RNGING_TIME_XX64_LIMIT	10		// RNGING �^�C�����~�b�g(s)�ݒ� (�Đ��^)
#define RNGING_TIME_XX34_LIMIT		3600	// RNGING �^�C�����~�b�g(s)�ݒ� (�]���^)
#define REP_RNGING_TIME_XX34_LIMIT	3600	// RNGING �^�C�����~�b�g(s)�ݒ� (�Đ��^)
#define DOPP_CONT_TIME_XX_LIMIT		10		// DOPP CONT �^�C�����~�b�g(s)�ݒ�
#define MODULATION_TIME_XX_LIMIT	10		// TCRMOD MODULATION �^�C�����~�b�g(s)�ݒ�
#define HOLD_TONE_TIME_XX_LIMIT		10		// X-CMDOUT HOLD TONE�^�C�����~�b�g(s)�ݒ�
#define SWEEP_NUM_TIME_XX_LIMIT		10		// ���M�L�����A�X�C�[�v�T�C�N�����^�C�����~�b�g(s)�ݒ�
#define CMD_MOD_IDX_TIME_XX_LIMIT	10		// CMD MOD INDEX �^�C�����~�b�g(s)�ݒ�
#define RNG_MOD_IDX_TIME_XX_LIMIT	10		// RNG MOD INDEX �^�C�����~�b�g(s)�ݒ�
#define CAR_MOD_IDX_TIME_XX_LIMIT	10		// CMD+RNG MOD INDEX �^�C�����~�b�g(s)�ݒ�

class CTrsControlXX : public CDialogBase
{
	DECLARE_DYNAMIC(CTrsControlXX)

public:
	CTrsControlXX(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	CTrsControlXX(UINT IDD, CWnd* pParent = NULL);
	virtual ~CTrsControlXX();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_TRSCTRL_XX };

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
	//�^�C���A�E�g�J�E���g�_�E���ϐ���` 
	// RF-POWER �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long	m_l_rf_power_off_cntdwn;
	long	m_l_rf_power_on_cntdwn;

	// NO-USE �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long	m_l_no_use_off_cntdwn;
	long	m_l_no_use_on_cntdwn;

	// SWEEP-CONT �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long	m_l_main_sweep_cont_stop_cntdwn;
	long	m_l_main_sweep_cont_inc_cntdwn;
	long	m_l_main_sweep_cont_incdec_cntdwn;
	long	m_l_main_sweep_cont_dec_cntdwn;
	long	m_l_main_sweep_cont_decinc_cntdwn;
	long	m_l_sub_sweep_cont_stop_cntdwn;
	long	m_l_sub_sweep_cont_inc_cntdwn;
	long	m_l_sub_sweep_cont_incdec_cntdwn;
	long	m_l_sub_sweep_cont_dec_cntdwn;
	long	m_l_sub_sweep_cont_decinc_cntdwn;
	int		m_i_main_sweep_cont_mode;
	int		m_i_sub_sweep_cont_mode;

	// DOPP-CONT �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long	m_l_dopp_cont_off_cntdwn;
	long	m_l_dopp_cont_on_cntdwn;

	// CMD-MOD �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long	m_l_cmd_mod_off_cntdwn;
	long	m_l_cmd_mod_on_cntdwn;

	// RNG-MOD �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long	m_l_rng_mod_off_cntdwn;
	long	m_l_rng_mod_on_cntdwn;

	// SS-RNG �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long	m_l_ss_rng_off_cntdwn;
	long	m_l_ss_rng_on_cntdwn;

	// SX-RNG �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long	m_l_sx_rng_off_cntdwn;
	long	m_l_sx_rng_on_cntdwn;

	// XX-RNG �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long	m_l_xx_rng_off_cntdwn;
	long	m_l_xx_rng_on_cntdwn;

	// COH/INCOH �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long	m_l_coh_incoh_coh_cntdwn;
	long	m_l_coh_incoh_incoh_cntdwn;

	// RNGING �^�C���A�E�g�J�E���g�_�E���ϐ���`(�]���^)
	long	m_l_rnging_stop_cntdwn;
	long	m_l_rnging_start_cntdwn;
	long	m_l_dsnrng_turn_start_flag;

	// RNGING �^�C���A�E�g�J�E���g�_�E���ϐ���`(�Đ��^)
	long	m_l_rep_rnging_stop_cntdwn;
	long	m_l_rep_rnging_start_cntdwn;
	long	m_l_dsnrng_regen_start_flag;

	// TCRMOD MODULATION �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long	m_l_modulation_off_cntdwn;
	long	m_l_modulation_on_cntdwn;

	// X-CMDOUT HOLD TONE �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long	m_l_hold_tone_off_cntdwn;
	long	m_l_hold_tone_on_cntdwn;

	// RNG��ʃt���O
	long	m_l_rng_type;

	// ���M�L�����A�X�C�[�v�T�C�N����
	long	m_l_sweep_num_cntdwn;
	long	m_l_sweep_num_set;

	// MOD INDEX �^�C���A�E�g�J�E���g�_�E���ϐ���`
	long	m_l_cmd_mod_idx_cntdwn;
	long	m_l_rng_mod_idx_cntdwn;
	long	m_l_car_mod_idx_cntdwn;

	// �X�C�[�v����ƃI�y���O���P�񂾂��o�͂��邽�߂̃t���O
	long m_l_flg_ctrl_sweep;     // 1shot-sweep


	long    m_l_inflg;						// �Ď����ڗL���t���O
	int		m_i_priv_flag;					// ���䌠�����J���t���O
	int		m_i_sweep_flag = 0;				// SWEEP������s�t���O
	double  m_d_sweep_num;
	double  m_d_cmd_mod_idx;				// CMD MOD INDEX
	double  m_d_rng_mod_idx;				// RNG MOD INDEX
	double	m_d_xx_rng_cnt;					// XX-RNG ���l��ԁi�����j
	double	m_d_xx_rng_num;					// XX-RNG ���l��ԁi�E���j

	char	m_sz_hostname[128];				// �z�X�g��
	char	m_sz_priv_ssoc[32];				// ���͌��ǐ��䌠�擾��� 
	char	m_sz_priv_sttn[32];				// �o��ǐ��䌠�擾���
	char    m_sz_rf_on_enbl_x[30];			// RF POWER ON ENABLE ���
	char    m_sz_no_use_enbl_x[30];			// RF-TRIP NOUSE ON ENABLE ���
	char    m_sz_uplink_cont_x[30];			// UP-LINK CONTROL ���
	char    m_sz_rf_on_det_ans_x[30];		// RF POWER ON DETECT ANSWER ���
	char    m_sz_rf_on_det_ans_s[30];		// RF POWER ON DETECT ANSWER ���
	char    m_sz_x_tx_rl[30];				// X-TX �� �����[�g�^���[�J�� ���
	char    m_sz_x_txsyn_rl[30];			// X-TXSYN �� �����[�g�^���[�J�� ���
	char    m_sz_dsntcr_rl[30];				// DSN-MODEM �� �����[�g�^���[�J�� ���
	char    m_sz_alarm_x[30];				// �A���[���`�F�b�N
	char    m_sz_rf_power_x[30];			// RF-POWER ���
	char    m_sz_hpa_rf_power_x[30];		// HPA RF-POWER ���
	char    m_sz_xpa_sel[30];				// HPA/SSPA �I�����
	char    m_sz_no_use_x[30];				// NO-USE ���
	char    m_sz_sweep_cont[30];			// SWEEP-CONT ���
	char    m_sz_sweep_cmd[30];				// SWEEP-CONT ���
	char    m_sz_sweep_sts[30];				// SWEEP-CONT ���
	char    m_sz_dopp_pre[30];				// DOPP-CONT ��� 
	char	m_sz_xx_tcrmod_sts[32];			// XX-TCRMOD MODULATION ���
	char    m_sz_hold_tone_sts_x[30];		// X-CMDOUT HOLD TONE ���
	char    m_sz_data_src[30];				// DATA SOURCE ���
	char	m_sz_norm_rep[30];				// �M�����z�Րؑ֐ݒ�i�]���^/�Đ��^�j
	char	m_sz_coh_incoh[30];				// COH/INCOH ���
	char	m_sz_rnging[30];				// RNGING ���
	char    m_sz_lan_x_tx[30];				
	char    m_sz_lan_s_tx[30];
	char    m_sz_hpa_rf_on_enbl_x[30];		// RF POWER ON ENABLE ���
	char    m_sz_hpa_rf_on_det_ans_x[30];	// HPA RF POWER ON DETECT ANSWER ���
	char    m_sz_lan_x_txsyn[30];
	char	m_sz_xx_tcrmod_rl[32];			// XX-TCRMOD �� �����[�g�^���[�J�� ���
	char    m_sz_lan_xx_tcrmod[30];			// XX-TCRMOD LAN �ڑ����
	char    m_sz_lan_x_cmdout[30];			// XX-TCRMOD LAN �ڑ����
	char	m_sz_lan_vequ[30];
	char    m_sz_lan_xx_rarr[30];
	char	m_sz_pass_flag[30];				// �p�X�^�p���t���O
	char	m_sz_lan_xx_rrng[30];			// DSN-MODEM LAN �ڑ����
	char	m_sz_xx_rrng_cont[30];			// DSN-MODEM CONT MODE ���
	char    m_sz_sat_name[30];				// �q������
	char    m_sz_mod_in_sel[30];			// MOD/TCRMOD �I�����
	char    m_sz_hpa_sel[30];				// RF POWER ON DETECT ANSWER ��� (HPA-2)
	char    m_sz_rf_on_det_ans_x_h2[30];	// RF POWER ON DETECT ANSWER ��� (HPA-2)
	char    m_sz_x_ul_cnt_rl[30];			// X-UL_CNT �� �����[�g�^���[�J�� ���
	char    m_sz_x_uc_rl[30];				// X-UC �� �����[�g�^���[�J�� ���
	char    m_sz_x_rf_if_rl[30];			// X-RF_IF �� �����[�g�^���[�J�� ���
	char    m_sz_cmd_mod_x[30];				// CMD-MOD ���
	char    m_sz_lan_x_ul_cnt[30];
	char    m_sz_lan_x_uc[30];				// X-UC LAN �ڑ����
	char    m_sz_lan_x_rf_if[30];

	// 54m
	char    m_sz_rf_on_enbl_sspa[30];		// RF POWER ON ENABLE ���
	char    m_sz_rf_power_sspa[30];			// RF-POWER ���
	char    m_sz_x_tx_if_rl[30];			// X-TX_IF �� �����[�g�^���[�J�� ���
	char    m_sz_lan_x_tx_if[30];			// X-TX_IF LAN �ڑ����
	char    m_sz_sspa_rl[30];				// SSPA �� �����[�g�^���[�J�� ���
	char	m_sz_lan_sspa[30];				// SSPA LAN �ڑ����
	char    m_sz_sweep_sub_cont[30];		// SWEEP-CONT SubCarr ���
	char    m_sz_sweep_sub_cmd[30];			// SWEEP-CONT SubCarr ���
	char    m_sz_sweep_sub_sts[30];			// SWEEP-CONT SubCarr ���

	// DEBUG�p >>>
	int		m_DebugRFPOWER;
	int		m_DebugNOUSE;
	int		m_DebugSweepCont;
	int		m_DebugSweepContSub;
	int		m_DebugDoppCont;
	int		m_DebugModulation;
	int		m_DebugHoldTone;
	int		m_DebugXXCoh;
	int		m_DebugRnging;
	int		m_DebugRRng;
	// <<< DEBUG�p


	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
protected:
	virtual BOOL UpdateDlg();

	void show_opsc_condition();
	void show_opsc_condition64();
	void show_opsc_condition34();
	void count_down();

	void show_rf_power_off64();
	void show_rf_power_off34();
	void show_rf_power_on64();
	void show_rf_power_on34();
	void show_no_use_off64();
	void show_no_use_off34();
	void show_no_use_on64();
	void show_no_use_on34();
	void show_main_sweep_cont_stop64();
	void show_main_sweep_cont_stop34();
	void show_main_sweep_cont_inc64();
	void show_main_sweep_cont_inc34();
	void show_main_sweep_cont_incdec64();
	void show_main_sweep_cont_incdec34();
	void show_main_sweep_cont_dec64();
	void show_main_sweep_cont_dec34();
	void show_main_sweep_cont_decinc64();
	void show_main_sweep_cont_decinc34();
	void show_sub_sweep_cont_stop64();
	void show_sub_sweep_cont_stop34();
	void show_sub_sweep_cont_inc64();
	void show_sub_sweep_cont_inc34();
	void show_sub_sweep_cont_incdec64();
	void show_sub_sweep_cont_incdec34();
	void show_sub_sweep_cont_dec64();
	void show_sub_sweep_cont_dec34();
	void show_sub_sweep_cont_decinc64();
	void show_sub_sweep_cont_decinc34();
	void show_dopp_cont_off64();
	void show_dopp_cont_off34();
	void show_dopp_cont_on64();
	void show_dopp_cont_on34();
	void show_modulation_off64();
	void show_modulation_off34();
	void show_modulation_on64();
	void show_modulation_on34();
	void show_hold_tone_off64();
	void show_hold_tone_off34();
	void show_hold_tone_on64();
	void show_hold_tone_on34();
	void show_coh_incoh_coh64();
	void show_coh_incoh_coh34();
	void show_coh_incoh_incoh64();
	void show_coh_incoh_incoh34();
	void show_rnging_stop64();
	void show_rnging_stop34();
	void show_rnging_start64();
	void show_rnging_start34();
	void show_rrng_stop64();
	void show_rrng_stop34();
	void show_rrng_start64();
	void show_rrng_start34();

	// 54m
	void show_opsc_condition54();
	void show_rf_power_on54();
	void show_rf_power_off54();
	void show_no_use_on54();
	void show_no_use_off54();

	void printlg_endtime(CString str);

	void CloseDlg();

	void EnableSweetCtrlSubCarr();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	// INDEX - CMD
	CString m_strIndexCmd;
	// INDEX - RNG
	CString m_strIndexRng;
	// INDEX - C+R
	CString m_strIndexCpr;
	// RNG CONTROL X/X-RNG
	CString m_strXXRng;
	CString m_strXXRngCont;
	CColorButtonEx2 m_RfPwrOffBtn;
	CColorButtonEx2 m_RfPwrOnBtn;
	CColorButtonEx2 m_NoUseOffBtn;
	CColorButtonEx2 m_NoUseOnBtn;
	CColorButtonEx2 m_SweepCountMStopBtn;
	CColorButtonEx2 m_SweepCountMIncBtn;
	CColorButtonEx2 m_SweepCountMIncDecBtn;
	CColorButtonEx2 m_SweepCountMDecBtn;
	CColorButtonEx2 m_SweepCountMDecIncBtn;
	CColorButtonEx2 m_SweepCountSStopBtn;
	CColorButtonEx2 m_SweepCountSIncBtn;
	CColorButtonEx2 m_SweepCountSIncDecBtn;
	CColorButtonEx2 m_SweepCountSDecBtn;
	CColorButtonEx2 m_SweepCountSDecIncBtn;
	CColorButtonEx2 m_SweepNumBtn;
	CColorButtonEx2 m_DoppContOffBtn;
	CColorButtonEx2 m_DoppContOnBtn;
	CColorButtonEx2 m_ModulationOffBtn;
	CColorButtonEx2 m_ModulationOnBtn;
	CColorButtonEx2 m_HoldToneOffBtn;
	CColorButtonEx2 m_HoldToneOnBtn;
	CColorButtonEx2 m_IndexCmdBtn;
	CColorButtonEx2 m_IndexRngBtn;
	CColorButtonEx2 m_IndexCRBtn;
	CColorButtonEx2 m_XXRngNumBtn;
	CColorButtonEx2 m_XXCohCohBtn;
	CColorButtonEx2 m_XXCohInCohBtn;
	CColorButtonEx2 m_RngingStopBtn;
	CColorButtonEx2 m_RngingStartBtn;
	CColorButtonEx2 m_RRngStopBtn;
	CColorButtonEx2 m_RRngStartBtn;
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuCloseClose();
	afx_msg void OnBnClickedTrxxctrlIdxCmdBtn();
	afx_msg void OnBnClickedTrxxctrlIdxRngBtn();
	afx_msg void OnBnClickedTrxxctrlIdxCrBtn();
	afx_msg void OnBnClickedTrxxctrlXxrngCBtn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedTrxxctrlSnBtn();
	CButton m_SweepContNorChkBtn;
	CButton m_SweepContTriChkBtn;
	CButton m_SweepContSubChkBtn;
	CButton m_SweepContSubNorChkBtn;
	CButton m_SweepContSubTriChkBtn;
	afx_msg void OnBnClickedTrxxctrlRfOffBtn();
	afx_msg void OnBnClickedTrxxctrlRfOnBtn();
	afx_msg void OnBnClickedTrxxctrlNouseOffBtn();
	afx_msg void OnBnClickedTrxxctrlNouseOnBtn();
	afx_msg void OnBnClickedTrxxctrlSweetNorCheck();
	afx_msg void OnBnClickedTrxxctrlSweetTriCheck();
	afx_msg void OnBnClickedTrxxctrlSweetStopBtn();
	afx_msg void OnBnClickedTrxxctrlSweetIncBtn();
	afx_msg void OnBnClickedTrxxctrlSweetIncdecBtn();
	afx_msg void OnBnClickedTrxxctrlSweetDecBtn();
	afx_msg void OnBnClickedTrxxctrlSweetDecincBtn();
	afx_msg void OnBnClickedTrxxctrlSweetSubCheck();
	afx_msg void OnBnClickedTrxxctrlSweetSubNorCheck();
	afx_msg void OnBnClickedTrxxctrlSweetSubTriCheck();
	afx_msg void OnBnClickedTrxxctrlSweetSubStopBtn();
	afx_msg void OnBnClickedTrxxctrlSweetSubIncBtn();
	afx_msg void OnBnClickedTrxxctrlSweetSubIncdecBtn();
	afx_msg void OnBnClickedTrxxctrlSweetSubDecBtn();
	afx_msg void OnBnClickedTrxxctrlSweetSubDecincBtn();
	afx_msg void OnBnClickedTrxxctrlMdlOffBtn();
	afx_msg void OnBnClickedTrxxctrlMdlOnBtn();
	afx_msg void OnBnClickedTrxxctrlDcOffBtn();
	afx_msg void OnBnClickedTrxxctrlDcOnBtn();
	afx_msg void OnBnClickedTrxxctrlHoldOffBtn();
	afx_msg void OnBnClickedTrxxctrlHoldOnBtn();
	afx_msg void OnBnClickedTrxxctrlXxcohCohBtn();
	afx_msg void OnBnClickedTrxxctrlXxcohIncohBtn();
	afx_msg void OnBnClickedTrxxctrlRngingStopBtn();
	afx_msg void OnBnClickedTrxxctrlRngingStartBtn();
	afx_msg void OnBnClickedTrxxctrlRgntypeStopBtn();
	afx_msg void OnBnClickedTrxxctrlRgntypeStartBtn();
	CGroupBoxEx m_grTxControl;
	CGroupBoxEx m_grRfPower;
	CGroupBoxEx m_grNoUse;
	CGroupBoxEx m_grSweepCont;
	CGroupBoxEx m_grSweepContSub;
	CGroupBoxEx m_grDoppCont;
	CGroupBoxEx m_grModulation;
	CGroupBoxEx m_grHoldTone;
	CGroupBoxEx m_grIndex;
	CGroupBoxEx m_grRngControl;
	CGroupBoxEx m_grXXRng;
	CGroupBoxEx m_grXXCoh;
	CGroupBoxEx m_grRnging;
	CGroupBoxEx m_grRRng;
	afx_msg void OnClose();
};
