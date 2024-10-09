#pragma once
#include "ColorButtonEx2.h"
#include "afxwin.h"
#include "MFCTabCtrlEx.h"


// CTrsControlS64 ダイアログ

// 監視項目データベースアクセス用定義
#define MON_S64_PRIV_SSOC			"CTRL.PRIV_SSOC"				// 相模原局制御権取得状態
#define MON_S64_PRIV_STTN			"CTRL.PRIV_STTN"				// 出先局制御権取得状態
#define MON_S64_RF_ON_ENBL			"S-TXRF.RF_ON_ENABLE"			// S-TX RF ON ENABLE 状態
#define MON_S64_RF_OFF_ENBL			"S-TXRF.RF_OFF_ENABLE"			// S-TX RF OFF ENABLE 状態
#define MON_S64_UPLINK_CONT			"CTRL.S_TX_TOTAL_REMOTE"		// S-TX CONTROL MODE 状態
#define MON_S64_X_TX_DET_ANS		"X-TX.RF_ON_ANS"				// X-TX RF ON  ANSWER 状態
#define MON_S64_S_TX_RL				"CTRL.S_TX_TOTAL_REMOTE"		// S-TX の リモート／ローカル 状態
#define MON_S64_S_TXSYN_RL			"S-UCSYN.CONT_MODE"				// S-TXSYN の リモート／ローカル 状態
#define MON_S64_ALARM				"CTRL.ALARM_00_6"				// アラームチェック
#define MON_S64_RF_POWER			"S-TXRF.IF_RF_OUTPUT_CONT"		// S-TX RF-POWER 状態
#define MON_S64_NO_USE				"S-TXRF.IF_NO_USE"				// S-TX NO-USE 状態
#define MON_S64_SWEEP_CONT			"S-UCSYN.SWEEP_CONT"			// S-TXSYN SWEEP-CONT 状態
#define MON_S64_TR_SWEEP_CONT		"S-UCSYN.TR_SWEEP_CONT"			// S-TXSYN Tri-SWEEP-CONT 状態
#define MON_S64_TR_SWEEP_NUM		"S-UCSYN.SWEEP_CYCLE"			// S-TXSYN Tri-SWEEP-NUM  状態
#define MON_S64_CMD_MOD				"SS-TCRMOD.MODULATION"			// CMD-MOD 状態
#define MON_S64_RNG_MOD				"S-TX.RNG_MOD"					// RNG-MOD 状態
#define MON_S64_SS_RNG				"V-EQU.RNG_SS"					// SS-RNG 状態
#define MON_S64_COH_INCOH			"V-EQU.COHI"					// COH/INCOH 状態
#define MON_S64_RNGING				"CTRL.RNG_FLG"					// RNGING 状態
#define MON_S64_HOLD_TONE			"CMDOUT.HOLDTONE_STAT"			// HOLD TONE 状態
#define MON_S64_CMD_MOD_IDX			"SS-TCRMOD.CMD_MOD_INDEX"		// CMD-MOD INDEX
#define MON_S64_RNG_MOD_IDX			"SS-TCRMOD.RNG_MOD_INDEX"		// RNG-MOD INDEX
#define MON_S64_DATA_SOURCE			"SS-TCRMOD.DATA_SOURCE"			// DATA_SOURCE
#define MON_S64_LAN_S_TX			"CTRL.LAN_01_0"					// S-TX LAN 接続状態
#define MON_S64_LAN_S_TXSYN			"CTRL.LAN_01_2"					// S-TXSYN LAN 接続状態
#define MON_S64_LAN_V_EQU			"CTRL.LAN_06_6"					// 仮想設備 LAN 接続状態
#define MON_S64_LAN_RNG				"CTRL.LAN_02_5"					// RNG LAN 接続状態
#define MON_S64_RNG_CONT			"SS-TCRDEM.CONT_MODE"			// RNG CONTROL 状態
#define MON_S64_LAN_CMDOUT1			"CTRL.LAN_04_1"					// CMDOUT1 LAN 接続状態
#define MON_S64_SAT_NAME			"V-EQU.BBE_SATNAME"				// BBE SAT NAME
#define MON_X_TX_DET_ANS			"X-TX.RF_ON_ANS"				// X-TX RF ON  ANSWER 状態
#define MON_S64_LAN_X_TX			"CTRL.LAN_04_4"					// X-TX LAN 接続状態
#define MON_S64_LAN_X_UL_CNT		"CTRL.LAN_05_6"					// X-UL_CNT LAN 接続状態

//--------------------------------------------------------
// 監視項目結果待ち各項目タイムリミット設定
#define RF_POWER_TIME_S64_LIMIT			10		// RF-POWER タイムリミット(s)設定
#define NO_USE_TIME_S64_LIMIT			300		// NO-USE タイムリミット(s)設定
#define SWEEP_CONT_TIME_S64_LIMIT		10		// SWEEP-CONT タイムリミット(s)設定
#define TR_SWEEP_CONT_TIME_S64_LIMIT	10		// Tri-SWEEP-CONT タイムリミット(s)設定
#define TR_SWEEP_NUM_TIME_S64_LIMIT		10		// Tri-SWEEP-NUM タイムリミット(s)設定
#define CMD_MOD_TIME_S64_LIMIT			10		// CMD-MOD タイムリミット(s)設定
#define RNG_MOD_TIME_S64_LIMIT			10		// RNG-MOD タイムリミット(s)設定
#define SS_RNG_TIME_S64_LIMIT			10		// SS-RNG タイムリミット(s)設定
#define SX_RNG_TIME_S64_LIMIT			10		// SX-RNG タイムリミット(s)設定
#define COH_INCOH_TIME_S64_LIMIT		10		// COH/INCOH タイムリミット(s)設定
#define RNGING_TIME_S64_LIMIT			10		// RNGING タイムリミット(s)設定
#define HOLDTONE_TIME_S64_LIMIT			10		// HOLD TONE タイムリミット(s)設定
#define CMD_MOD_IDX_TIME_S64_LIMIT		10		// CMD MOD INDEX タイムリミット(s)設定
#define RNG_MOD_IDX_TIME_S64_LIMIT		10		// RNG MOD INDEX タイムリミット(s)設定
#define CAR_MOD_IDX_TIME_S64_LIMIT		10		// CMD+RNG MOD INDEX タイムリミット(s)設定
#define RNG_ROUTE_TIME_S64_LIMIT		10		// RNG ROUTE切り替え タイムリミット(s)設定
#define TRK_DATA_TIME_S64_LIMIT			10		// 追跡データ配信切り替え タイムリミット(s)設定
#define TLM_DATA_TIME_S64_LIMIT			10		// テレメトリデータ配信切り替え タイムリミット(s)設定

class CTrsControlS64 : public CDialogBase
{
	DECLARE_DYNAMIC(CTrsControlS64)

public:
	CTrsControlS64(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTrsControlS64();

// ダイアログ データ
	enum { IDD = IDD_DLG_TRSCTRL_S64 };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
protected:
	long    m_l_inflg;                  // 監視項目有効フラグ
	int		m_i_priv_flag;				// 制御権状態フラグ
	int		m_i_sweep_flag;				// SWEEP制御実行フラグ追加
	int		m_i_nor_sweep_cont_mode;
	int		m_i_tri_sweep_cont_mode;
	long	m_l_tri_sweep_flag;			// 三角スイープ制御完了フラグ
	double	m_d_tri_sweep_num;          // S-TXSYN Tri-SWEEP NUM

	char	m_sz_hostname[128];			// ホスト名
	char	m_sz_priv_ssoc[32];
	char	m_sz_priv_sttn[32];
	char    m_sz_rf_on_enbl[30];		// RF POWER ON ENABLE 状態
	char    m_sz_rf_off_enbl[30];		// RF POWER OFF ENABLE 状態
	char    m_sz_no_use_enbl[30];		// RF-TRIP NOUSE ON ENABLE 状態
	char    m_sz_uplink_cont[30];		// UP-LINK CONTROL 状態
	char    m_sz_rf_on_det_ans[30];		// RF POWER ON DETECT ANSWER 状態 (S-TX)
	char    m_sz_x_tx_det_ans[30];		// RF POWER ON DETECT ANSWER 状態 (X-TX)
	char    m_sz_s_tx_rl[30];			// S-TX の リモート／ローカル 状態
	char    m_sz_s_txsyn_rl[30];		// S-TXSYN の リモート／ローカル 状態
	char    m_sz_alarm[30];				// アラームチェック
	char    m_sz_rf_power[30];			// RF-POWER 状態
	char    m_sz_no_use[30];			// NO-USE 状態
	char    m_sz_nor_sweep_cont[30];	// SWEEP-CONT 状態
	char    m_sz_nor_sweep_cmd[30];		// SWEEP-CONT 状態
	char    m_sz_nor_sweep_sts[30];		// SWEEP-CONT 状態
	char    m_sz_tri_sweep_cont[30];	// SWEEP-CONT 状態
	char    m_sz_tri_sweep_cmd[30];		// SWEEP-CONT 状態
	char    m_sz_tri_sweep_sts[30];		// SWEEP-CONT 状態
	char    m_sz_cmd_mod[30];			// CMD-MOD 状態
	char    m_sz_rng_mod[30];			// RNG-MOD 状態
	char	m_sz_ss_rng[30];			// SS-RNG 状態
	char    m_sz_lan_vequ[30];
	char    m_sz_lan_rng[30];
	char	m_sz_rng_cont[30];			// RNG CONT 状態


	// RF-POWER タイムアウトカウントダウン変数定義
	long	m_l_rf_power_off_cntdwn;
	long	m_l_rf_power_on_cntdwn;

	// NO-USE タイムアウトカウントダウン変数定義
	long 	m_l_no_use_off_cntdwn;
	long 	m_l_no_use_on_cntdwn;

	// SWEEP-CONT タイムアウトカウントダウン変数定義
	long 	m_l_nor_sweep_cont_stop_cntdwn;
	long 	m_l_nor_sweep_cont_inc_cntdwn;
	long 	m_l_nor_sweep_cont_incdec_cntdwn;
	long 	m_l_nor_sweep_cont_dec_cntdwn;
	long 	m_l_nor_sweep_cont_decinc_cntdwn;

	// Tri-SWEEP-CONT タイムアウトカウントダウン変数定義
	long 	m_l_tri_sweep_cont_stop_cntdwn;
	long 	m_l_tri_sweep_cont_inc_cntdwn;
	long 	m_l_tri_sweep_cont_incdec_cntdwn;
	long 	m_l_tri_sweep_cont_dec_cntdwn;
	long 	m_l_tri_sweep_cont_decinc_cntdwn;

	// Tri-SWEEP-NUM タイムアウトカウントダウン変数定義
	long	m_l_tri_sweep_num_cntdwn;
	long	m_l_tri_sweep_num_set;

	// CMD-MOD タイムアウトカウントダウン変数定義
	long 	m_l_cmd_mod_off_cntdwn;
	long 	m_l_cmd_mod_on_cntdwn;

	// SS-RNG タイムアウトカウントダウン変数定義
	long 	m_l_ss_rng_off_cntdwn;
	long 	m_l_ss_rng_on_cntdwn;

	// SX-RNG タイムアウトカウントダウン変数定義
	long 	m_l_sx_rng_off_cntdwn;
	long 	m_l_sx_rng_on_cntdwn;

	// COH/INCOH タイムアウトカウントダウン変数定義
	long 	m_l_coh_incoh_coh_cntdwn;
	long 	m_l_coh_incoh_incoh_cntdwn;

	// RNGING タイムアウトカウントダウン変数定義
	long 	m_l_rnging_stop_cntdwn;
	long 	m_l_rnging_start_cntdwn;

	// HOLD TONE タイムアウトカウントダウン変数定義
	long 	m_l_holdtone_off_cntdwn;
	long 	m_l_holdtone_on_cntdwn;

	// MOD INDEX タイムアウトカウントダウン変数定義
	long 	m_l_cmd_mod_idx_cntdwn;
	long 	m_l_rng_mod_idx_cntdwn;
	long 	m_l_car_mod_idx_cntdwn;

	// RNG ROUTE タイムアウトカウントダウン変数定義
	long 	m_l_rng_route_cntdwn;
	long 	m_l_trk_data_cntdwn;
	long 	m_l_tlm_data_cntdwn;

	// 送信キャリアスイープサイクル数
	long	m_l_sweep_num_cntdwn;
	long	m_l_sweep_num_set;



	double	m_d_ss_rng_cnt;				// SS-RNG 数値状態（左側）
	double	m_d_ss_rng_num;				// SS-RNG 数値状態（右側）
	double	m_d_sx_rng_cnt;				// SX-RNG 数値状態（左側）
	double	m_d_sx_rng_num;				// SX-RNG 数値状態（右側）
	double  m_d_cmd_mod_idx;			// CMD MOD INDEX
	double  m_d_rng_mod_idx;			// RNG MOD INDEX

	char	m_sz_coh_incoh[30];			// COH/INCOH 状態
	char	m_sz_rnging[30];			// RNGING 状態
	char	m_sz_pass_flag[30];			// パス運用中フラグ
	char    m_sz_hold_tone[30];
	char    m_sz_data_src[30];			// DATA SOURCE 状態
	char    m_sz_rng_route[30];
	char    m_sz_s_tlm_sts[30];
	char	m_sz_data_txpower[32];		// RF_POWER_SEL 状態
	char	m_sz_data_ucroute[32];		// RFSW_UC_ROUTE 状態 
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
	char    m_sz_dsntcr_rl[30];				// DSN-MODEM の リモート／ローカル 状態

	// DEBUG用 >>>
	int		m_DebugRrPower;
	int		m_DebugNoUse;
	int		m_DebugSweepContN;
	int		m_DebugSweepContT;
	int		m_DebugCmdMod;
	int		m_DebugSSRng;
	int		m_DebugCohIncoh;
	int		m_DebugRnging;
	int		m_DebugHoldTone;
	// <<< DEBUG用

	//-----------------------------------------------------------------------------
	//	メンバ関数
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

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
