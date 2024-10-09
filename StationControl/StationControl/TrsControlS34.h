#pragma once
#include "afxwin.h"
#include "ColorButtonEx2.h"
#include "MFCTabCtrlEx.h"

// CTrsControlS34 ダイアログ

// 監視項目データベースアクセス用定義
#define MON_S34_S_TXSYN_RL			"SS-TCRMOD.CONT_MODE"			// S-TXSYN の リモート／ローカル 状態
#define MON_S34_CONT_FLAG			"SS-RARR.DEEP_NEAR"				// CONTINUATION"表示有無状態
#define MON_S34_S_TX_RL				"S-TX.TX_TOTAL_CONT"			// S-TX の リモート／ローカル 状態 
#define MON_S34_ALARM				"S-TX.TOTAL_FAULT"				// アラームチェック
#define MON_S34_RF_POWER			"S-TX.RF_ON_ANS"				// S-TX RF-POWER 状態
#define MON_S34_NO_USE				"S-TX.RF_TRIP_NO_USE"			// S-TX NO-USE 状態
#define MON_S34_CMD_MOD				"SS-TCRMOD.MODULATION"			// CMD-MOD 状態
#define MON_S34_RNG_MOD				"S-TX.RNG_MOD"					// RNG-MOD 状態
#define MON_S34_SS_RNG_CNT			"CTRL.SS_RNG_GOT_CNT"			// SS-RNG 数値状態（左側）
#define MON_S34_SS_RNG_NUM			"V-EQU.RNG_NUM"					// SS-RNG 数値状態（右側）
#define MON_S34_SS_RNG				"V-EQU.RNG_SS"					// SS-RNG 状態
#define MON_S34_SX_RNG_CNT			"CTRL.SX_RNG_GOT_CNT"			// SX-RNG 数値状態（左側）
#define MON_S34_SX_RNG_NUM			"V-EQU.RNG_NUM"					// SX-RNG 数値状態（右側）
#define MON_S34_SX_RNG				"V-EQU.RNG_SX"					// SX-RNG 状態
#define MON_S34_COH_INCOH			"V-EQU.COHI"					// COH/INCOH 状態
#define MON_S34_RNGING				"CTRL.RNG_FLG"					// RNGING 状態
#define MON_S34_P_FLG				"CTRL.PASS_FLAG"				// パス運用中フラグ
#define MON_S34_LAN_S_TXSYN			"CTRL.LAN_03_6"					// S-TXSYN LAN 接続状態
#define MON_S34_LAN_SS_RARR			"CTRL.LAN_00_1"					// SS-RARR LAN 接続状態
#define MON_S34_LAN_SX_RARR			"CTRL.LAN_00_0"					// SX-RARR LAN 接続状態
#define MON_S34_NO_USE_ENBL			"S-TX.NO_USE_ENBL"				// S-TX RF-TRIP NOUSE ON ENABLE 状態
#define MON_S34_RF_ON_DET_ANS		"S-TX.RF_ON_DET_ANS"			// S-TX RF ON DETECT ANSWER 状態
#define MON_S34_LAN_X_TX			"CTRL.LAN_04_4"					// X-TX LAN 接続状態
#define MON_S34_X_TX_DET_ANS		"X-UL_CNT.SSPA_RF_ON_DET_ANS"	// X-TX RF ON  ANSWER 状態
#define MON_S34_RF_ON_ENBL			"S-TX.RF_ON_ENBL"				// S-TX RF ON ENABLE 状態
#define MON_S34_UPLINK_CONT			"S-TX.UPLINK_CONT"				// S-TX UP-LINK CONTROL 状態
#define MON_S34_LAN_S_TX			"CTRL.LAN_00_6"					// S-TX LAN 接続状態
#define MON_S34_LAN_V_EQU			"CTRL.LAN_05_0"					// 仮想設備 LAN 接続状態
#define MON_S34_PRIV_SSOC			"CTRL.PRIV_SSOC"				// 相模原局制御権取得状態
#define MON_S34_PRIV_STTN			"CTRL.PRIV_STTN"				// 出先局制御権取得状態 
#define MON_S34_HPA_RF_ON_DET_ANS_X	"X-UL_CNT.RF_DET"				// HPA RF POWER ON DETECT ANSWER 状態
#define MON_S34_CMD_MOD_IDX			"SS-TCRMOD.CMD_MOD_INDEX"		// CMD-MOD INDEX
#define MON_S34_RNG_MOD_IDX			"SS-TCRMOD.RNG_MOD_INDEX"		// RNG-MOD INDEX
#define MON_S34_DATA_SOURCE			"SS-TCRMOD.DATA_SOURCE"			// DATA_SOURCE
#define MON_S34_SAT_NAME			"V-EQU.BBE_SATNAME"				// BBE SAT NAME
#define MON_S34_MOD_UPLINK_CONT		"SS-TCRMOD.CONT_MODE"			// SS-TCRMOD 状態
#define MON_S34_DEM_UPLINK_CONT		"SS-TCRDEM.CONT_MODE"			// SS-TCRMOD 状態
#define MON_S34_HOLD_TONE			"CMDOUT.HOLDTONE_STAT"			// HOLD TONE 状態
#define MON_S34_LAN_SS_TCRMOD		"CTRL.LAN_06_1"					// SS-TCRMOD LAN 接続状態
#define MON_S34_LAN_SS_TCRDEM		"CTRL.LAN_06_2"					// SS-TCRDEM LAN 接続状態
#define MON_S34_LAN_CMDOUT1			"CTRL.LAN_01_4"					// CMDOUT1 LAN 接続状態
#define MON_S34_S_RNG_ROUTE			"CTRL.S_RNG_ROUTE"				// S帯RNG計測ルート
#define MON_S34_SS_TRK_STS			"CTRL.SS_TRK_STS"				// S/S帯追跡データ配信状況
#define MON_S34_SX_TRK_STS			"CTRL.SX_TRK_STS"				// S/X帯追跡データ配信状況
#define MON_S34_S_TLM_STS			"CTRL.S_TLM_STS"				// S帯TLM配信状況
#define MON_S34_NOR_SWEEP_CONT		"SS-TCRMOD.SWEEP_ANS"			// S-TXSYN SWEEP-CONT 状態
#define MON_S34_NOR_SWEEP_CMD		"SS-TCRMOD.SWEEP_CMD_ANS"		// S-TXSYN SWEEP-CMD  状態
#define MON_S34_NOR_SWEEP_STS		"SS-TCRMOD.SWEEP_STS"			// S-TXSYN SWEEP-CONT 状態
#define	MON_S34_POWER_SEL			"S-TX.RF_POWER_SEL"				// 送送信出力-パワー
#define	MON_S34_UC_ROUTE			"S-TX.RFSW_UC_ROUTE"			// U/Cルート
#define MON_S34_LAN_X_UL_CNT		"CTRL.LAN_04_4"					// X-UL_CNT LAN 接続状態
#define MON_S34_LAN_X_UC			"CTRL.LAN_04_3"					// X-UC LAN 接続状態
#define MON_S34_LAN_X_RF_IF			"CTRL.LAN_04_2"					// X-RF_IF LAN 接続状態

//--------------------------------------------------------
// 監視項目結果待ち各項目タイムリミット設定
#define RF_POWER_TIME_S34_LIMIT		10		// RF-POWER タイムリミット(s)設定
#define NO_USE_TIME_S34_LIMIT		300		// NO-USE タイムリミット(s)設定
#define SWEEP_CONT_TIME_S34_LIMIT	10		// SWEEP-CONT タイムリミット(s)設定
#define CMD_MOD_TIME_S34_LIMIT		10		// CMD-MOD タイムリミット(s)設定
#define RNG_MOD_TIME_S34_LIMIT		10		// RNG-MOD タイムリミット(s)設定
#define SS_RNG_TIME_S34_LIMIT		10		// SS-RNG タイムリミット(s)設定
#define SX_RNG_TIME_S34_LIMIT		10		// SX-RNG タイムリミット(s)設定
#define COH_INCOH_TIME_S34_LIMIT	10		// COH/INCOH タイムリミット(s)設定
#define RNGING_TIME_S34_LIMIT		600		// RNGING タイムリミット(s)設定
#define HOLDTONE_TIME_S34_LIMIT		10		// HOLD TONE タイムリミット(s)設定
#define CMD_MOD_IDX_TIME_S34_LIMIT	10		// CMD MOD INDEX タイムリミット(s)設定
#define RNG_MOD_IDX_TIME_S34_LIMIT	10		// RNG MOD INDEX タイムリミット(s)設定
#define CAR_MOD_IDX_TIME_S34_LIMIT	10		// CMD+RNG MOD INDEX タイムリミット(s)設定
#define RNG_ROUTE_TIME_S34_LIMIT	10		// RNG ROUTE切り替え タイムリミット(s)設定
#define TRK_DATA_TIME_S34_LIMIT		10		// 追跡データ配信切り替え タイムリミット(s)設定
#define TLM_DATA_TIME_S34_LIMIT		10		// テレメトリデータ配信切り替え タイムリミット(s)設定


class CTrsControlS34 : public CDialogBase
{
	DECLARE_DYNAMIC(CTrsControlS34)

public:
	CTrsControlS34(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTrsControlS34();

// ダイアログ データ
	enum { IDD = IDD_DLG_TRSCTRL_S34 };

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
	int		m_i_priv_flag;				// 制御権状態フラグ
	long	m_l_flg_ctrl_sweep;			// 1shot-sweep
	int		m_i_sweep_flag;				// SWEEP制御実行フラグ追加
	int		m_i_sweep_cont_mode;
	double	m_d_ss_rng_cnt;				// SS-RNG 数値状態（左側）
	double	m_d_ss_rng_num;				// SS-RNG 数値状態（右側）
	double	m_d_sx_rng_cnt;				// SX-RNG 数値状態（左側）
	double	m_d_sx_rng_num;				// SX-RNG 数値状態（右側）
	double  m_d_cmd_mod_idx;			// CMD MOD INDEX
	double  m_d_rng_mod_idx;			// RNG MOD INDEX

	char	m_sz_hostname[128];			// ホスト名
	char	m_sz_priv_ssoc[32];
	char	m_sz_priv_sttn[32];
	char    m_sz_rf_on_enbl[30];		// RF POWER ON ENABLE 状態
	char    m_sz_no_use_enbl[30];		// RF-TRIP NOUSE ON ENABLE 状態
	char    m_sz_uplink_cont[30];		// UP-LINK CONTROL 状態
	char    m_sz_rf_on_det_ans[30];		// RF POWER ON DETECT ANSWER 状態 (S-TX)
	char    m_sz_x_tx_det_ans[30];		// RF POWER ON DETECT ANSWER 状態 (X-TX)
	char    m_sz_s_tx_rl[30];			// S-TX の リモート／ローカル 状態
	char    m_sz_s_txsyn_rl[30];		// S-TXSYN の リモート／ローカル 状態
	char    m_sz_alarm[30];				// アラームチェック
	char    m_sz_rf_power[30];			// RF-POWER 状態
	char    m_sz_no_use[30];			// NO-USE 状態
	char    m_sz_sweep_cont[30];		// SWEEP-CONT 状態
	char    m_sz_sweep_cmd[30];			// SWEEP-CONT 状態
	char    m_sz_sweep_sts[30];			// SWEEP-CONT 状態
	char    m_sz_cmd_mod[30];			// CMD-MOD 状態
	char	m_sz_ss_rng[30];			// SS-RNG 状態
	char	m_sz_rnging[30];			// RNGING 状態
	char	m_sz_coh_incoh[30];			// COH/INCOH 状態
	char    m_sz_hold_tone[30];
	char    m_sz_data_src[30];			// DATA SOURCE 状態
	char    m_sz_rng_route[30];
	char    m_sz_s_tlm_sts[30];
	char	m_sz_data_txpower[32];		// RF_POWER_SEL 状態
	char	m_sz_data_ucroute[32];		// RFSW_UC_ROUTE 状態

	char    m_sz_lan_s_tx[30];
	char    m_sz_lan_x_tx[30];
	char    m_sz_x_tx_hpa_det_ans[30];
	char    m_sz_lan_ss_tcrmod[30];
	char    m_sz_mod_uplink_cont[30];
	char    m_sz_lan_s_txsyn[30];
	char    m_sz_lan_ss_rarr[30];
	char	m_sz_pass_flag[30];			// パス運用中フラグ
	char    m_sz_lan_cmdout1[30];
	char    m_sz_sat_name[30];
	char    m_sz_lan_x_ul_cnt[30];
	char    m_sz_lan_x_uc[30];
	char    m_sz_lan_x_rf_if[30];


	// RF-POWER タイムアウトカウントダウン変数定義
	long	m_l_rf_power_off_cntdwn;
	long	m_l_rf_power_on_cntdwn;

	// NO-USE タイムアウトカウントダウン変数定義
	long 	m_l_no_use_off_cntdwn;
	long 	m_l_no_use_on_cntdwn;

	// SWEEP-CONT タイムアウトカウントダウン変数定義
	long 	m_l_sweep_cont_stop_cntdwn;
	long 	m_l_sweep_cont_inc_cntdwn;
	long 	m_l_sweep_cont_incdec_cntdwn;
	long 	m_l_sweep_cont_dec_cntdwn;
	long 	m_l_sweep_cont_decinc_cntdwn;

	// CMD-MOD タイムアウトカウントダウン変数定義
	long 	m_l_cmd_mod_off_cntdwn;
	long 	m_l_cmd_mod_on_cntdwn;

	// RNG-MOD タイムアウトカウントダウン変数定義
	long 	m_l_rng_mod_off_cntdwn;
	long 	m_l_rng_mod_on_cntdwn;

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

	// DEBUG用 >>>
	int		m_DebugRfPower;
	int		m_DebugNoUse;
	int		m_DebugSweepCont;
	int		m_DebugCmdMod;
	int		m_DebugHoldTone;
	int		m_DebugCohIncoh;
	int		m_DebugRnging;
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
	void show_sweep_cont_stop();
	void show_sweep_cont_inc();
	void show_sweep_cont_incdec();
	void show_sweep_cont_dec();
	void show_sweep_cont_decinc();
	void show_cmd_mod_off();
	void show_cmd_mod_on();
	void show_holdtone_off();
	void show_holdtone_on();
	void show_coh_incoh_coh();
	void show_coh_incoh_incoh();
	void show_rnging_stop();
	void show_rnging_start();

	void printlg_endtime(CString str);

	void CloseDlg();

private:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// INDEX-CMD
	CString m_strIndexCmd;
	// INDEX-RNG
	CString m_strIndexRng;
	// INDEX-C+R
	CString m_strIndexCR;
	// TX PWR/ROUTE - POWER
	CString m_strPower;
	// TX PWR/ROUTE - ROUTE
	CString m_strRoute;
	// RNG ROUTE
	CString m_strRngRoute;
	// TRK DATA
	CString m_strTrkData;
	// TLM DATA
	CString m_strTlmData;
	CColorButtonEx2 m_RfPwrOffBtn;
	CColorButtonEx2 m_RfPwrOnBtn;
	CColorButtonEx2 m_NoUseOffBtn;
	CColorButtonEx2 m_NoUseOnBtn;
	CColorButtonEx2 m_SweepContStopBtn;
	CColorButtonEx2 m_SweepContIncBtn;
	CColorButtonEx2 m_SweepContIncDecBtn;
	CColorButtonEx2 m_SweepContDecBtn;
	CColorButtonEx2 m_SweepContDecIncBtn;
	CColorButtonEx2 m_CmdModOffBtn;
	CColorButtonEx2 m_CmdModOnBtn;
	CColorButtonEx2 m_HoldToneOffBtn;
	CColorButtonEx2 m_HoldToneOnBtn;
	CColorButtonEx2 m_CohIncohCohBtn;
	CColorButtonEx2 m_CohIncohInCohBtn;
	CColorButtonEx2 m_RngingStopBtn;
	CColorButtonEx2 m_RngingStartBtn;
	CColorButtonEx2 m_IndexCmdBtn;
	CColorButtonEx2 m_IndexRngBtn;
	CColorButtonEx2 m_IndexCRBtn;
	CColorButtonEx2 m_TxPwrRouteBtn;
	afx_msg void OnMenuClose();
	afx_msg void OnBnClickedTrsctrlIdxCmdBtn();
	afx_msg void OnBnClickedTrsctrlIdxRngBtn();
	afx_msg void OnBnClickedTrsctrlIdxCrBtn();
	afx_msg void OnBnClickedTrsctrlTxprPrBtn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedTrsctrlRfOffBtn();
	afx_msg void OnBnClickedTrsctrlRfOnBtn();
	afx_msg void OnBnClickedTrsctrlNouseOffBtn();
	afx_msg void OnBnClickedTrsctrlNouseOnBtn();
	afx_msg void OnBnClickedTrsctrlSweetStopBtn();
	afx_msg void OnBnClickedTrsctrlSweetIncBtn();
	afx_msg void OnBnClickedTrsctrlSweetIncdecBtn();
	afx_msg void OnBnClickedTrsctrlSweetDecBtn();
	afx_msg void OnBnClickedTrsctrlSweetDecincBtn();
	afx_msg void OnBnClickedTrsctrlCmdOffBtn();
	afx_msg void OnBnClickedTrsctrlCmdOnBtn();
	afx_msg void OnBnClickedTrsctrlHoldOffBtn();
	afx_msg void OnBnClickedTrsctrlHoldOnBtn();
	afx_msg void OnBnClickedTrsctrlCohincohCohBtn2();
	afx_msg void OnBnClickedTrsctrlCohincohIncohBtn2();
	afx_msg void OnBnClickedTrsctrlRngingStopBtn2();
	afx_msg void OnBnClickedTrsctrlRngingStartBtn2();
	CGroupBoxEx m_grTxControl;
	CGroupBoxEx m_grRfPower;
	CGroupBoxEx m_grNoUse;
	CGroupBoxEx m_grSweepCont;
	CGroupBoxEx m_grCmdMod;
	CGroupBoxEx m_grHoldTone;
	CGroupBoxEx m_grIndex;
	CGroupBoxEx m_grTxPwrRoute;
	CGroupBoxEx m_grRngControl;
	CGroupBoxEx m_grCohIncoh;
	CGroupBoxEx m_grRnging;
	afx_msg void OnClose();
};
