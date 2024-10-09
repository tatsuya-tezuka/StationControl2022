#pragma once
#include "afxwin.h"
#include "ColorButtonEx2.h"
#include "MFCTabCtrlEx.h"


// CTrsControlX ダイアログ


// 監視項目データベースアクセス用定義
#define MON_X_COH_INCOH				"V-EQU.COHI"						// COH/INCOH 状態
#define MON_X_RNGING				"CTRL.RNG_FLG"						// RNGING 状態
#define MON_X_X_TX_RL				"X-TX.TX_TOTAL_CONT"				// X-TX の リモート／ローカル 状態 
#define MON_X_X_TXSYN_RL			"X-TXSYN.CONT_MODE"					// X-TXSYN の リモート／ローカル 状態
#define MON_X_UL_CNT_RL				"X-UL_CNT.X-TX_TOTAL_REMOTE"		// X-UL_CNT の リモート／ローカル 状態
#define MON_X_RF_IF_RL				"X-RF_IF.CONTROL_MODE"				// X-RF_IF の リモート／ローカル 状態
#define MON_X_ALARM_X				"X-TX.TOTAL_FAULT"					// アラームチェック
#define MON_X_RF_POWER_X			"X-UL_CNT.RF_ON_CONT"				// RF-POWER 状態
#define MON_X34_RF_POWER_X			"X-TX.RF_ON_ANS"					// RF-POWER 状態
#define MON_X_NO_USE_X				"X-UL_CNT.RF_TRIP_NO_USE"			// NO-USE 状態
#define MON_X34_NO_USE_X			"X-TX.RF_TRIP_NO_USE"				// NO-USE 状態
#define MON_X_SWEEP_CONT_X			"X-TXSYN.SWEEP_CONT"				// SWEEP-CONT 状態
#define MON_X_RATE_AID_X			"X-TXSYN.RATE_AID"					// RATE AID 状態
#define MON_X_CMD_MOD_X				"X-TX.CMD_MOD"						// CMD-MOD 状態
#define MON_X_RNG_MOD_X				"X-TX.RNG_MOD"						// RNG-MOD 状態
#define MON_X_CONT_FLAG_X			"XX-RARR.DEEP_NEAR"					// CONTINUATION"表示有無状態
#define MON_X_XX_RARR_CONT			"XX-RARR.CONT_MODE"					// XX-RARR CONT MODE 状態
#define MON_X_XX_RNG_CNT			"CTRL.XX_RNG_GOT_CNT"				// XX-RNG 数値状態（左側）
#define MON_X_XX_RNG_NUM			"V-EQU.RNG_NUM"						// XX-RNG 数値状態（右側）
#define MON_X_P_FLG					"CTRL.PASS_FLAG"					// パス運用中フラグ
#define MON_X_RF_ON_ENBL_X			"X-UL_CNT.RF_ON_ENABLE"				// RF POWER ON ENABLE 状態
#define MON_X34_RF_ON_ENBL_X		"X-TX.RF_ON_ENBL"					// RF POWER ON ENABLE 状態
#define MON_X_NO_USE_ENBL_X			"X-UL_CNT.RF_TRIP_NO_USE_ENA"		// RF-TRIP NOUSE ON ENABLE 状態
#define MON_X34_NO_USE_ENBL_X		"X-TX.NO_USE_ENBL"					// RF-TRIP NOUSE ON ENABLE 状態
#define MON_X_UPLINK_CONT_X			"X-UL_CNT.UP_LINK_CONT_REMOTE"		// UP-LINK CONTROL 状態
#define MON_X34_UPLINK_CONT_X		"X-TX.UPLINK_CONT"					// UP-LINK CONTROL 状態
#define MON_X_RF_ON_DET_ANS_X		"X-UL_CNT.RF_DET"					// RF POWER ON DETECT ANSWER 状態 (X-TX)
#define MON_X34_RF_ON_DET_ANS_X		"X-TX.SSPA_RF_ON_DET"				// RF POWER ON DETECT ANSWER 状態 (X-TX)
#define MON_X_RF_ON_DET_ANS_S		"S-TXRF.IF_RF_OUTPUT_CONT"			// RF POWER ON DETECT ANSWER 状態 (S-TX)
#define MON_X34_RF_ON_DET_ANS_S		"S-TX.RF_ON_DET_ANS"				// RF POWER ON DETECT ANSWER 状態 (S-TX)
#define MON_X_HPA_SEL				"X-TX.HPA_ANS"						// HPA SEL 状態 (X-TX)
#define MON_X_RF_ON_DET_ANS_X_H2	"X-UL_CNT.HPA2_RF_DET"				// RF POWER ON DETECT ANSWER 状態 (X-TX)
#define MON_X_LAN_X_TX				"CTRL.LAN_03_0"						// X-TX LAN 接続状態
#define MON_X34_LAN_X_TX			"CTRL.LAN_04_4"						// X-TX LAN 接続状態
#define MON_X_LAN_S_TX				"CTRL.LAN_01_0"						// S-TX LAN 接続状態
#define MON_X34_LAN_S_TX			"CTRL.LAN_00_6"						// S-TX LAN 接続状態
#define MON_X_LAN_X_TXSYN			"CTRL.LAN_03_2"						// X-TXSYN LAN 接続状態
#define MON_X34_LAN_X_TXSYN			"CTRL.LAN_04_3"						// X-TXSYN LAN 接続状態
#define MON_X_LAN_XX_RARR			"CTRL.LAN_03_7"						// XX-RARR LAN 接続状態
#define MON_X34_LAN_XX_RARR			"CTRL.LAN_04_2"						// XX-RARR LAN 接続状態
#define MON_X_LAN_V_EQU				"CTRL.LAN_06_6"						// 仮想設備 LAN 接続状態
#define MON_X34_LAN_V_EQU			"CTRL.LAN_05_0"						// 仮想設備 LAN 接続状態
#define MON_X_LAN_X_UL_CNT			"CTRL.LAN_05_6"						// X-UL_CNT LAN 接続状態
#define MON_X_LAN_X_RF_IF			"CTRL.LAN_07_2"						// X-RF_IF LAN 接続状態
#define MON_X_PRIV_SSOC				"CTRL.PRIV_SSOC"					// 相模原局制御権取得状態
#define MON_X_PRIV_STTN				"CTRL.PRIV_STTN"					// 出先局制御権取得状態
#define MON_X_NORM_REP				"XX-RRNG.DIST_RNG_VIDEO"			// 信号分配盤切替設定（従来型/再生型）
#define MON_X_LAN_XX_RRNG			"CTRL.LAN_04_2"						// XX-RRNG LAN 接続状態
#define MON_X34_LAN_XX_RRNG			"CTRL.LAN_04_6"						// XX-RRNG LAN 接続状態
#define MON_X_XX_RRNG_CONT			"XX-RRNG.CONT_MODE"					// XX-RRNG CONT MODE 状態
#define MON_X_CTRL_SAT_NAME			"CTRL.SAT_NAME"						// 衛星名称
#define MON_X_XX_TCRMOD_RL			"XX-TCRMOD.CONT_MODE"				// XX-TCRMOD の リモート／ローカル 状態
#define MON_X_XX_TCRMOD_STS			"XX-TCRMOD.MODULATION"				// XX-TCRMOD MODULATION 状態
#define MON_X_LAN_XX_TCRMOD			"CTRL.LAN_07_5"						// XX-TCRMOD LAN 接続状態
#define MON_X34_LAN_XX_TCRMOD		"CTRL.LAN_05_4"						// XX-TCRMOD LAN 接続状態
#define MON_X_LAN_X_CMDOUT			"CTRL.LAN_07_4"						// XX-TCRMOD LAN 接続状態
#define MON_X34_LAN_X_CMDOUT		"CTRL.LAN_05_2"						// XX-TCRMOD LAN 接続状態
#define MON_X_MOD_IN_SEL			"V-EQU.DSN_TCRMOD_SEL"				// MOD/TCRMOD 選択状態
#define MON_X34_MOD_IN_SEL			"X-IFSW.IF_CH_INPUT_SEL_ANS"		// MOD/TCRMOD 選択状態
#define MON_X_MOD_OUT_SEL			"X-IFSW.IF_CH_RNG_OUTPUT_SEL_ANS"	// MOD/TCRMOD 選択状態
#define MON_X_XPA_SEL				"X-TX.XPA_SEL_SSPA_HPA_SEL_ANS"		// HPA/SSPA 選択状態
#define MON_X_HPA_RF_POWER_X		"X-TX.HPA_RF_ON_CONT_ANS"			// HPA RF-POWER 状態
#define MON_X_HPA_RF_ON_DET_ANS_X	"X-TX.RF_ON_DET_ANS"				// HPA RF POWER ON DETECT ANSWER 状態
#define MON_X_HPA_RF_ON_ENBL_X		"X-TX.RF_ON_ENBL"					// RF POWER ON ENABLE 状態
#define MON_X_HOLD_TONE_STS_X		"CMDOUT3.HOLD_TONE"					// X-CMDOUT HOLD TONE 状態
#define MON_X34_HOLD_TONE_STS_X		"X-CMDOUT.HOLD_TONE"				// X-CMDOUT HOLD TONE 状態
#define MON_X_RX_RNG_CNT			"CTRL.RX_RNG_GOT_CNT"				// RX-RNG 数値状態（左側）

// 監視項目結果待ち各項目タイムリミット設定
#define RF_POWER_TIME_X_LIMIT	10		// RF-POWER タイムリミット(s)設定
#define NO_USE_TIME_X_LIMIT		300		// NO-USE タイムリミット(s)設定
#define SWEEP_CONT_TIME_X_LIMIT	10		// SWEEP-CONT タイムリミット(s)設定
#define CMD_MOD_TIME_X_LIMIT	10		// CMD-MOD タイムリミット(s)設定
#define RNG_MOD_TIME_X_LIMIT	10		// RNG-MOD タイムリミット(s)設定
#define SS_RNG_TIME_X_LIMIT		10		// SS-RNG タイムリミット(s)設定
#define SX_RNG_TIME_X_LIMIT		10		// SX-RNG タイムリミット(s)設定
#define COH_INCOH_TIME_X_LIMIT	10		// COH/INCOH タイムリミット(s)設定
#define RNGING_TIME_X_LIMIT		3600	// RNGING タイムリミット(s)設定 (従来型)
#define REP_RNGING_TIME_X_LIMIT	3600	// RNGING タイムリミット(s)設定 (再生型)
#define RAMP_CONT_TIME_X_LIMIT	10		// RAMP CONT タイムリミット(s)設定
#define MODULATION_TIME_X_LIMIT	10		// TCRMOD MODULATION タイムリミット(s)設定
#define HOLD_TONE_TIME_X_LIMIT	10		// X-CMDOUT HOLD TONEタイムリミット(s)設定

class CTrsControlX : public CDialogBase
{
	DECLARE_DYNAMIC(CTrsControlX)

public:
	CTrsControlX(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTrsControlX();

// ダイアログ データ
	enum { IDD = IDD_DLG_TRSCTRL_X };

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
	int		m_i_priv_flag;					// 制御権状態フラグ
	double	m_d_xx_rng_cnt;					// XX-RNG 数値状態（左側）
	double	m_d_xx_rng_num;					// XX-RNG 数値状態（右側）

	char	m_sz_hostname[128];				// ホスト名
	char	m_sz_priv_ssoc[32];
	char	m_sz_priv_sttn[32];
	char    m_sz_rf_on_enbl_x[30];			// RF POWER ON ENABLE 状態
	char    m_sz_no_use_enbl_x[30];			// RF-TRIP NOUSE ON ENABLE 状態
	char    m_sz_uplink_cont_x[30];			// UP-LINK CONTROL 状態
	char    m_sz_rf_on_det_ans_x[30];		// RF POWER ON DETECT ANSWER 状態
	char    m_sz_rf_on_det_ans_s[30];		// RF POWER ON DETECT ANSWER 状態
	char    m_sz_hpa_sel[30];				// RF POWER ON DETECT ANSWER 状態 (HPA-2)
	char    m_sz_rf_on_det_ans_x_h2[30];	// RF POWER ON DETECT ANSWER 状態 (HPA-2)
	char    m_sz_x_tx_rl[30];				// X-TX の リモート／ローカル 状態
	char    m_sz_x_txsyn_rl[30];			// X-TXSYN の リモート／ローカル 状態
	char    m_sz_x_ul_cnt_rl[30];			// X-UL_CNT の リモート／ローカル 状態
	char    m_sz_x_rf_if_rl[30];			// X-RF_IF の リモート／ローカル 状態
	char    m_sz_alarm_x[30];				// アラームチェック
	char    m_sz_rf_power_x[30];			// RF-POWER 状態
	char    m_sz_hpa_rf_power_x[30];		// HPA RF-POWER 状態
	char    m_sz_xpa_sel[30];				// HPA/SSPA 選択状態
	char    m_sz_no_use_x[30];				// NO-USE 状態
	char    m_sz_sweep_cont_x[30];			// SWEEP-CONT 状態
	char    m_sz_rate_aid_x[30];			// RAMP-CONT 状態
	char    m_sz_cmd_mod_x[30];				// CMD-MOD 状態
	char    m_sz_rng_mod_x[30];				// RNG-MOD 状態
	char	m_sz_xx_tcrmod_sts[32];			// XX-TCRMOD MODULATION 状態
	char    m_sz_hold_tone_sts_x[30];		// X-CMDOUT HOLD TONE 状態
	char	m_sz_norm_rep[30];				// 信号分配盤切替設定（従来型/再生型）
	char	m_sz_cont_flag_x[30];			// CONTINUATION"表示有無状態
	char	m_sz_coh_incoh[30];				// COH/INCOH 状態
	char	m_sz_rnging[30];				// RNGING 状態
	char    m_sz_mod_in_sel[30];			// MOD/TCRMOD 選択状態
	char    m_sz_lan_x_tx[30];
	char    m_sz_lan_s_tx[30];
	char    m_sz_lan_x_ul_cnt[30];			// X-UL_CNT の リモート／ローカル 状態
	char    m_sz_lan_x_rf_if[30];			// X-RF_IF の リモート／ローカル 状態
	char    m_sz_hpa_rf_on_enbl_x[30];		// RF POWER ON ENABLE 状態
	char    m_sz_hpa_rf_on_det_ans_x[30];	// HPA RF POWER ON DETECT ANSWER 状態
	char    m_sz_lan_x_txsyn[30];
	char	m_sz_xx_tcrmod_rl[32];			// XX-TCRMOD の リモート／ローカル 状態
	char    m_sz_lan_xx_tcrmod[30];			// XX-TCRMOD LAN 接続状態
	char    m_sz_lan_x_cmdout[30];
	char	m_sz_lan_vequ[30];
	char    m_sz_lan_xx_rarr[30];
	char	m_sz_xx_rarr_cont[30];			// XX-RARR CONT 状態 
	char	m_sz_pass_flag[30];				// パス運用中フラグ
	char	m_sz_lan_xx_rrng[30];			// XX-RRNG LAN 接続状態
	char	m_sz_xx_rrng_cont[30];			// XX-RRNG CONT MODE 状態
	char    m_sz_sat_name[30];				// 衛星名称


	// タイムアウトカウントダウン値初期設定
	long m_l_rf_power_off_cntdwn;
	long m_l_rf_power_on_cntdwn;
	long m_l_no_use_off_cntdwn;
	long m_l_no_use_on_cntdwn;
    long m_l_sweep_cont_stop_cntdwn;
	long m_l_sweep_cont_inc_cntdwn;
	long m_l_sweep_cont_incdec_cntdwn;
	long m_l_sweep_cont_dec_cntdwn;
	long m_l_sweep_cont_decinc_cntdwn;
	long m_l_ramp_cont_off_cntdwn;
	long m_l_ramp_cont_on_cntdwn;
	long m_l_cmd_mod_off_cntdwn;
	long m_l_cmd_mod_on_cntdwn;
	long m_l_rng_mod_off_cntdwn;
	long m_l_rng_mod_on_cntdwn;
	long m_l_modulation_off_cntdwn;
	long m_l_modulation_on_cntdwn;
	long m_l_hold_tone_off_cntdwn;
	long m_l_hold_tone_on_cntdwn;
	long m_l_xx_rng_off_cntdwn;
	long m_l_xx_rng_on_cntdwn;
	long m_l_coh_incoh_coh_cntdwn;
	long m_l_coh_incoh_incoh_cntdwn;
	long m_l_rnging_stop_cntdwn;
	long m_l_rnging_start_cntdwn;
	long m_l_rep_rnging_stop_cntdwn;
	long m_l_rep_rnging_start_cntdwn;
	long m_l_rng_type;

	// DEBUG用 >>>
	int		m_DebugRrPower;
	int		m_DebugNoUse;
	int		m_DebugSweepCont;
	int		m_DebugRampCont;
	int		m_DebugCmdMod;
	int		m_DebugRngMod;
	int		m_DebugModulation;
	int		m_DebugHoldTone;
	int		m_DebugXXCoh;
	int		m_DebugRnging;
	int		m_DebugRRng;
	// <<< DEBUG用

	//-----------------------------------------------------------------------------
	//	メンバ関数
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
	void show_sweep_cont_stop64();
	void show_sweep_cont_stop34();
	void show_sweep_cont_inc64();
	void show_sweep_cont_inc34();
	void show_sweep_cont_incdec64();
	void show_sweep_cont_incdec34();
	void show_sweep_cont_dec64();
	void show_sweep_cont_dec34();
	void show_sweep_cont_decinc64();
	void show_sweep_cont_decinc34();
	void show_ramp_cont_off64();
	void show_ramp_cont_off34();
	void show_ramp_cont_on64();
	void show_ramp_cont_on34();
	void show_cmd_mod_off64();
	void show_cmd_mod_off34();
	void show_cmd_mod_on64();
	void show_cmd_mod_on34();
	void show_rng_mod_off64();
	void show_rng_mod_off34();
	void show_rng_mod_on64();
	void show_rng_mod_on34();
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

	void CloseDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	// RNG CONTROL X/X-RNG
	CString m_strXXRng;
	CString m_strXXRngCont;
	CColorButtonEx2 m_RFPWROffBtn;
	CColorButtonEx2 m_RFPWROnBtn;
	CColorButtonEx2 m_NoUseOffBtn;
	CColorButtonEx2 m_NoUseOnBtn;
	CColorButtonEx2 m_SweepCountStopBtn;
	CColorButtonEx2 m_SweepCountIncBtn;
	CColorButtonEx2 m_SweepCountIncDecBtn;
	CColorButtonEx2 m_SweepCountDecBtn;
	CColorButtonEx2 m_SweepCountDecIncBtn;
	CColorButtonEx2 m_RampContOffBtn;
	CColorButtonEx2 m_RampContOnBtn;
	CColorButtonEx2 m_CmdModOffBtn;
	CColorButtonEx2 m_CmdModOnBtn;
	CColorButtonEx2 m_RngModOffBtn;
	CColorButtonEx2 m_RngModOnBtn;
	CColorButtonEx2 m_ModulationOffBtn;
	CColorButtonEx2 m_ModulationOnBtn;
	CColorButtonEx2 m_HoldToneOffBtn;
	CColorButtonEx2 m_HoldToneOnBtn;
	CColorButtonEx2 m_XXRngNumBtn;
	CColorButtonEx2 m_XXCohCohBtn;
	CColorButtonEx2 m_XXCohIncohBtn;
	CColorButtonEx2 m_RngingStopBtn;
	CColorButtonEx2 m_RngingStartBtn;
	CColorButtonEx2 m_RRngStopBtn;
	CColorButtonEx2 m_RRngStartBtn;
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuClose();
	afx_msg void OnBnClickedTrxctrlXxrngCBtn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedTrxctrlRfOffBtn();
	afx_msg void OnBnClickedTrxctrlRfOnBtn();
	afx_msg void OnBnClickedTrxctrlNouseOffBtn();
	afx_msg void OnBnClickedTrxctrlNouseOnBtn();
	afx_msg void OnBnClickedTrxctrlSweetStopBtn();
	afx_msg void OnBnClickedTrxctrlSweetIncBtn();
	afx_msg void OnBnClickedTrxctrlSweetIncdecBtn();
	afx_msg void OnBnClickedTrxctrlSweetDecBtn();
	afx_msg void OnBnClickedTrxctrlSweetDecincBtn();
	afx_msg void OnBnClickedTrxctrlRampcontOffBtn();
	afx_msg void OnBnClickedTrxctrlRampcontOnBtn();
	afx_msg void OnBnClickedTrxctrlCmdOffBtn();
	afx_msg void OnBnClickedTrxctrlCmdOnBtn();
	afx_msg void OnBnClickedTrxctrlRngOffBtn();
	afx_msg void OnBnClickedTrxctrlRngOnBtn();
	afx_msg void OnBnClickedTrxctrlModulationOffBtn();
	afx_msg void OnBnClickedTrxctrlModulationOnBtn();
	afx_msg void OnBnClickedTrxctrlHoldtoneOffBtn();
	afx_msg void OnBnClickedTrxctrlHoldtoneOnBtn();
	afx_msg void OnBnClickedTrxctrlXxcohCohBtn();
	afx_msg void OnBnClickedTrxctrlXxcohIncohBtn();
	afx_msg void OnBnClickedTrxctrlRngingStopBtn();
	afx_msg void OnBnClickedTrxctrlRngingStartBtn();
	afx_msg void OnBnClickedTrxctrlRgntypeStopBtn();
	afx_msg void OnBnClickedTrxctrlRgntypeStartBtn();
	CGroupBoxEx m_grTxControl;
	CGroupBoxEx m_grRfPower;
	CGroupBoxEx m_grNoUse;
	CGroupBoxEx m_grSweepCont;
	CGroupBoxEx m_grRampCont;
	CGroupBoxEx m_grMod;
	CGroupBoxEx m_grCmdMod;
	CGroupBoxEx m_grRngMod;
	CGroupBoxEx m_grTcrMod;
	CGroupBoxEx m_grModulation;
	CGroupBoxEx m_grHoldTone;
	CGroupBoxEx m_grRngControl;
	CGroupBoxEx m_grXXRng;
	CGroupBoxEx m_grXXCoh;
	CGroupBoxEx m_grRnging;
	CGroupBoxEx m_grRRng;
	afx_msg void OnClose();
};
