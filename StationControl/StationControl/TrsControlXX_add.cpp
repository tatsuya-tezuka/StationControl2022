/*============================================================================*/
/*! TrsControlXX.cpp

-# X帯送信制御画面 (54m用)
*/
/*============================================================================*/
// TrsControlXX.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrsControlXX.h"
#include "afxdialogex.h"
#include "TrsControlMng.h"
#include "HookMsgBox.h"

/*============================================================================*/
// 送信制御画面状態表示設定 (54m)
/*============================================================================*/
void CTrsControlXX::show_opsc_condition54()
{
	//TRACE(_T("show_opsc_condition\n"));

	char sz_buff[64];
	CString str1, str2;

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 制御権は常に有り
	m_i_priv_flag = 1;

	/*-- TX CONTROL ----------------------------------------------------------------------*/

	/*** リモート／ローカル 状態取得 ******************************************************/

	// S-TXSYN リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_DSNTCR_RL), CString(m_sz_dsntcr_rl), nLogEx::debug);
	}

	// X-UC リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_X_UC_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_uc_rl, sizeof(m_sz_x_uc_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_X_UC_RL), CString(m_sz_x_uc_rl), nLogEx::debug);
	}

	// X-TX_IF リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_X_TX_IF_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_tx_if_rl, sizeof(m_sz_x_tx_if_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX54_X_TX_IF_RL), CString(m_sz_x_tx_if_rl), nLogEx::debug);
	}

	/*** アラームチェック  <<XUP>> X-TX用に変更 *******************************************/
#if 1
	strcpy_s(m_sz_alarm_x, sizeof(m_sz_alarm_x), "OFF");
#else
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_ALARM_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_alarm_x, sizeof(m_sz_alarm_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("X-TX.TOTAL_FAULT"), CString(m_sz_alarm_x), nLogEx::debug);
	}
#endif

	// アラーム時
	if (strcmp(m_sz_alarm_x, "ON") == 0)
	{
		// 枠を赤くする
		m_grRfPower.SetBackColor(COLOR_ALARM);
		m_grNoUse.SetBackColor(COLOR_ALARM);
		m_grSweepCont.SetBackColor(COLOR_ALARM);
		m_grSweepContSub.SetBackColor(COLOR_ALARM);
		m_grDoppCont.SetBackColor(COLOR_ALARM);
		m_grModulation.SetBackColor(COLOR_ALARM);
		m_grHoldTone.SetBackColor(COLOR_ALARM);
	}
	// アラーム解除
	else
	{
		// 枠の色を元に戻す
		m_grRfPower.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grNoUse.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepCont.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepContSub.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grDoppCont.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grModulation.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grHoldTone.SetBackColor(GetSysColor(COLOR_3DFACE));
	}


	/*** RF-POWER *************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_RF_POWER_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_power_x, sizeof(m_sz_rf_power_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("X-TX_IF.TX1_OUTPUT"), CString(m_sz_rf_power_x), nLogEx::debug);
	}
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_RF_POWER_SSPA));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_power_sspa, sizeof(m_sz_rf_power_sspa), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("X-SSPA.TX_ON/OFF"), CString(m_sz_rf_power_sspa), nLogEx::debug);
	}

	// RF-POWER ON カウントダウン中に監視データが ON になった場合
	if( (m_l_rf_power_on_cntdwn >= 0) &&
		((strcmp(m_sz_rf_power_x, "ON") == 0) && (strcmp(m_sz_rf_power_sspa, "ON") == 0)) )
	{
		// カウントダウン終了
		m_l_rf_power_on_cntdwn = -1;
	}
	// RF-POWER ON カウントダウンタイムリミットになった場合
	else if (m_l_rf_power_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rf_power_on_cntdwn = -1;
	}

	// RF-POWER OFF カウントダウン中に監視データが OFF になった場合
	if( (m_l_rf_power_off_cntdwn >= 0) &&
		((strcmp(m_sz_rf_power_x, "OFF") == 0) || (strcmp(m_sz_rf_power_sspa, "OFF") == 0)) )
	{
		// カウントダウン終了
		m_l_rf_power_off_cntdwn = -1;
	}
	// RF-POWER OFF カウントダウンタイムリミットになった場合
	else if (m_l_rf_power_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rf_power_off_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if( (m_l_rf_power_off_cntdwn == -1) &&
		(m_l_rf_power_on_cntdwn == -1) )
	{
		// ON の場合
		if( (strcmp(m_sz_rf_power_x, "ON") == 0) && (strcmp(m_sz_rf_power_sspa, "ON") == 0) )
		{
			show_rf_power_on54();
		}
		// OFF の場合
		else
		{
			/*** 特殊処理注意 *************************************************************/
			show_rf_power_off54();
		}
	}


	/*** NO-USE ***************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_NO_USE_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_x, sizeof(m_sz_no_use_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX54_NO_USE_X), CString(m_sz_no_use_x), nLogEx::debug);
	}

	// NO-USE ON カウントダウン中に監視データが ON になった場合
	if( (m_l_no_use_on_cntdwn >= 0) &&
		(strcmp(m_sz_no_use_x, "ON") == 0) )
	{
		// カウントダウン終了
		m_l_no_use_on_cntdwn = -1;
	}
	// NO-USE ON カウントダウンタイムリミットになった場合
	else if (m_l_no_use_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_no_use_on_cntdwn = -1;
	}

	// NO-USE OFF カウントダウン中にデータが OFF になった場合
	if( (m_l_no_use_off_cntdwn >= 0) &&
		(strcmp(m_sz_no_use_x, "ON") != 0) )
	{
		// カウントダウン終了
		m_l_no_use_off_cntdwn = -1;
	}
	// NO-USE OFF カウントダウンタイムリミットになった場合
	else if (m_l_no_use_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_no_use_off_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if( (m_l_no_use_off_cntdwn == -1) &&
		( m_l_no_use_on_cntdwn == -1) )
	{
		// ON の場合
		if (strcmp(m_sz_no_use_x, "ON") == 0)
		{
			show_no_use_on54();
		}
		// OFF の場合
		else
		{
			show_no_use_off54();
		}
	}


	/*** MAIN SWEEP-CONT ***********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_TC_SWEEP_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cont, sizeof(m_sz_sweep_cont), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_TC_SWEEP_CONT), CString(m_sz_sweep_cont), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_TC_SWEEP_CMD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cmd, sizeof(m_sz_sweep_cmd), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_TC_SWEEP_CMD), CString(m_sz_sweep_cmd), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_TC_SWEEP_STS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_sts, sizeof(m_sz_sweep_sts), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_TC_SWEEP_STS), CString(m_sz_sweep_sts), nLogEx::debug);
	}

	// SWEEP-CONT STOP カウントダウン中に監視データが STOP になった場合
	if( (m_l_main_sweep_cont_stop_cntdwn >= 0)       &&
		((strcmp(m_sz_sweep_cont, "SWEEP_OFF") == 0) ||
		 (strcmp(m_sz_sweep_cmd, "STOP") == 0)       ||
		 (strcmp(m_sz_sweep_sts, "STOP") == 0)) )
	{
		// カウントダウン終了
		m_l_main_sweep_cont_stop_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;

		// SWEEP実行フラグOFF
		m_i_sweep_flag = 0;
	}
	// SWEEP-CONT STOP カウントダウンタイムリミットになった場合
	else if (m_l_main_sweep_cont_stop_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_main_sweep_cont_stop_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}

	// SWEEP-CONT INC カウントダウン中にデータが +SAW/+TRIANGLE になった場合(項番1)
	if( (m_l_main_sweep_cont_inc_cntdwn >= 0)      &&
		(strcmp(m_sz_sweep_cont, "SWEEP_ON") == 0) &&
		(strcmp(m_sz_sweep_sts, "RUNNING") == 0)   &&
		((strcmp(m_sz_sweep_cmd, "+SAW") == 0) || (strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0)) )
	{
		// カウントダウン終了
		m_l_main_sweep_cont_inc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +SAW カウントダウンタイムリミットになった場合
	else if (m_l_main_sweep_cont_inc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_main_sweep_cont_inc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if( (m_l_main_sweep_cont_inc_cntdwn >= 0)        &&
			 (m_i_main_sweep_cont_mode == SWEEP_MODE_SAW) &&
			 (strcmp(m_sz_sweep_cmd, "+SAW") != 0) )
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if( (m_l_main_sweep_cont_inc_cntdwn >= 0)        &&
			 (m_i_main_sweep_cont_mode == SWEEP_MODE_TRI) &&
			 (strcmp(m_sz_sweep_cmd, "+TRIANGLE") != 0) )
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT INCDEC カウントダウン中にデータが +-SAW/+-TRIANGLE になった場合(項番2)
	if( (m_l_main_sweep_cont_incdec_cntdwn >= 0)   &&
		(strcmp(m_sz_sweep_cont, "SWEEP_ON") == 0) &&
		(strcmp(m_sz_sweep_sts, "RUNNING") == 0)   &&
		((strcmp(m_sz_sweep_cmd, "+-SAW") == 0) || (strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0)) )
	{
		// カウントダウン終了
		m_l_main_sweep_cont_incdec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +-SAW/TRIANGLE カウントダウンタイムリミットになった場合
	else if (m_l_main_sweep_cont_incdec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_main_sweep_cont_incdec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if( (m_l_main_sweep_cont_incdec_cntdwn >= 0)     &&
			 (m_i_main_sweep_cont_mode == SWEEP_MODE_SAW) &&
			 (strcmp(m_sz_sweep_cmd, "+-SAW") != 0) )
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+-SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if( (m_l_main_sweep_cont_incdec_cntdwn >= 0)     &&
			 (m_i_main_sweep_cont_mode == SWEEP_MODE_TRI) &&
			 (strcmp(m_sz_sweep_cmd, "+-TRIANGLE") != 0) )
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+-TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT DEC カウントダウン中にデータが -SAW/-TRIANGLE になった場合(項番3)
	if( (m_l_main_sweep_cont_dec_cntdwn >= 0)      &&
		(strcmp(m_sz_sweep_cont, "SWEEP_ON") == 0) &&
		(strcmp(m_sz_sweep_sts, "RUNNING") == 0)   &&
		((strcmp(m_sz_sweep_cmd, "-SAW") == 0) || (strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0)) )
	{
		// カウントダウン終了
		m_l_main_sweep_cont_dec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT DEC カウントダウンタイムリミットになった場合
	else if (m_l_main_sweep_cont_dec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_main_sweep_cont_dec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if( (m_l_main_sweep_cont_dec_cntdwn >= 0)        &&
			 (m_i_main_sweep_cont_mode == SWEEP_MODE_SAW) &&
			 (strcmp(m_sz_sweep_cmd, "-SAW") != 0) )
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if( (m_l_main_sweep_cont_dec_cntdwn >= 0)        &&
			 (m_i_main_sweep_cont_mode == SWEEP_MODE_TRI) &&
			 (strcmp(m_sz_sweep_cmd, "-TRIANGLE") != 0) )
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT DECINC カウントダウン中にデータが -+SAW/-+TRIANGLE になった場合(項番4)
	if( (m_l_main_sweep_cont_decinc_cntdwn >= 0)   &&
		(strcmp(m_sz_sweep_cont, "SWEEP_ON") == 0) &&
		(strcmp(m_sz_sweep_sts, "RUNNING") == 0)   &&
		((strcmp(m_sz_sweep_cmd, "-+SAW") == 0) || (strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0)) )
	{
		// カウントダウン終了
		m_l_main_sweep_cont_decinc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT -+SAW カウントダウンタイムリミットになった場合
	else if (m_l_main_sweep_cont_decinc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_main_sweep_cont_decinc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if( (m_l_main_sweep_cont_decinc_cntdwn >= 0)     &&
			 (m_i_main_sweep_cont_mode == SWEEP_MODE_SAW) &&
			 (strcmp(m_sz_sweep_cmd, "-+SAW") != 0) )
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-+SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if( (m_l_main_sweep_cont_decinc_cntdwn >= 0)     &&
			 (m_i_main_sweep_cont_mode == SWEEP_MODE_TRI) &&
			 (strcmp(m_sz_sweep_cmd, "-+TRIANGLE") != 0) )
	{
		/* 1shot-sweep */
		// カウントダウン中には一回のみ制御とオペログを出すようフラグ制御化
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-+TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if( (m_l_main_sweep_cont_stop_cntdwn == -1)   &&
		(m_l_main_sweep_cont_inc_cntdwn == -1)    &&
		(m_l_main_sweep_cont_incdec_cntdwn == -1) &&
		(m_l_main_sweep_cont_dec_cntdwn == -1)    &&
		(m_l_main_sweep_cont_decinc_cntdwn == -1) )
	{
		// INC の場合(項番1)
		if( (strcmp(m_sz_sweep_cont, "SWEEP_ON") == 0) &&
			(strcmp(m_sz_sweep_sts, "RUNNING") == 0)   &&
			((strcmp(m_sz_sweep_cmd, "+SAW") == 0) || (strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0)) )
		{
			show_main_sweep_cont_inc64();
		}

		// INCDEC の場合(項番2)
		else if( (strcmp(m_sz_sweep_cont, "SWEEP_ON") == 0) &&
				 (strcmp(m_sz_sweep_sts, "RUNNING") == 0)   &&
				 ((strcmp(m_sz_sweep_cmd, "+-SAW") == 0) || (strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0)) )
		{
			show_main_sweep_cont_incdec64();
		}

		// DEC の場合(項番3)
		else if( (strcmp(m_sz_sweep_cont, "SWEEP_ON") == 0) &&
				 (strcmp(m_sz_sweep_sts, "RUNNING") == 0)   &&
				 ((strcmp(m_sz_sweep_cmd, "-SAW") == 0) || (strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0)) )
		{
			show_main_sweep_cont_dec64();
		}

		// DECINC の場合(項番4)
		else if( (strcmp(m_sz_sweep_cont, "SWEEP_ON") == 0) &&
				 (strcmp(m_sz_sweep_sts, "RUNNING") == 0)   &&
				 ((strcmp(m_sz_sweep_cmd, "-+SAW") == 0) || (strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0)) )
		{
			show_main_sweep_cont_decinc64();
		}

		// STOP の場合(項番5)
		else {
			// CONTがONでSTSがRUNNING以外ならば、CONTをOFFする
			if( (strcmp(m_sz_sweep_cont, "SWEEP_ON") == 0) &&
				(strcmp(m_sz_sweep_sts, "RUNNING") != 0)   &&
				(m_i_sweep_flag == 1) )
			{
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("SWEEP_OFF"));
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("STOP"));

				// SWEEP実行フラグOFF
				m_i_sweep_flag = 0;
			}

			// 完全に停止してから、STOP表示にする
			if( (strcmp(m_sz_sweep_cont, "SWEEP_OFF") == 0) &&
				(strcmp(m_sz_sweep_cmd, "STOP") == 0)       &&
				((strcmp(m_sz_sweep_sts, "INITIALIZE") == 0) || (strcmp(m_sz_sweep_sts, "STOP") == 0)) )
			{
				show_main_sweep_cont_stop64();
			}
		}
	}


	/*** SUB SWEEP-CONT ***********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SC_SWEEP_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_sub_cont, sizeof(m_sz_sweep_sub_cont), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_SC_SWEEP_CONT), CString(m_sz_sweep_sub_cont), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SC_SWEEP_CMD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_sub_cmd, sizeof(m_sz_sweep_sub_cmd), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_SC_SWEEP_CMD), CString(m_sz_sweep_sub_cmd), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SC_SWEEP_STS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_sub_sts, sizeof(m_sz_sweep_sub_sts), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_SC_SWEEP_STS), CString(m_sz_sweep_sub_sts), nLogEx::debug);
	}

	// SWEEP-CONT STOP カウントダウン中に監視データが STOP になった場合
	if( (m_l_sub_sweep_cont_stop_cntdwn >= 0)        &&
		((strcmp(m_sz_sweep_sub_cont, "SWEEP_OFF") == 0) ||
		 (strcmp(m_sz_sweep_sub_cmd, "STOP") == 0)       ||
		 (strcmp(m_sz_sweep_sub_sts, "STOP") == 0)) )
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_stop_cntdwn = -1;

		// SWEEP実行フラグOFF
		m_i_sweep_flag = 0;
	}
	// SWEEP-CONT STOP カウントダウンタイムリミットになった場合
	else if (m_l_sub_sweep_cont_stop_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_stop_cntdwn = -1;
	}

	// SWEEP-CONT INC カウントダウン中にデータが +SAW/+TRIANGLE になった場合(項番1)
	if( (m_l_sub_sweep_cont_inc_cntdwn >= 0)       &&
		(strcmp(m_sz_sweep_sub_cont, "SWEEP_ON") == 0) &&
		(strcmp(m_sz_sweep_sub_sts, "RUNNING") == 0)   &&
		((strcmp(m_sz_sweep_sub_cmd, "+SAW") == 0) || (strcmp(m_sz_sweep_sub_cmd, "+TRIANGLE") == 0)) )
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_inc_cntdwn = -1;

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +SAW カウントダウンタイムリミットになった場合
	else if (m_l_sub_sweep_cont_inc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_inc_cntdwn = -1;
	}
	else if( (m_l_sub_sweep_cont_inc_cntdwn >= 0)        &&
			 (m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW) &&
			 (strcmp(m_sz_sweep_sub_cmd, "+SAW") != 0) )
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+SAW"));
	}
	else if( (m_l_sub_sweep_cont_inc_cntdwn >= 0)        &&
			 (m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI) &&
			 (strcmp(m_sz_sweep_sub_cmd, "+TRIANGLE") != 0) )
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+TRIANGLE"));
	}

	// SWEEP-CONT INCDEC カウントダウン中にデータが +-SAW/+-TRIANGLE になった場合(項番2)
	if( (m_l_sub_sweep_cont_incdec_cntdwn >= 0)    &&
		(strcmp(m_sz_sweep_sub_cont, "SWEEP_ON") == 0) &&
		(strcmp(m_sz_sweep_sub_sts, "RUNNING") == 0)   &&
		((strcmp(m_sz_sweep_sub_cmd, "+-SAW") == 0) || (strcmp(m_sz_sweep_sub_cmd, "+-TRIANGLE") == 0)) )
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_incdec_cntdwn = -1;

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +-SAW/TRIANGLE カウントダウンタイムリミットになった場合
	else if (m_l_sub_sweep_cont_incdec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_incdec_cntdwn = -1;
	}
	else if( (m_l_sub_sweep_cont_incdec_cntdwn >= 0)     &&
			 (m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW) &&
			 (strcmp(m_sz_sweep_sub_cmd, "+-SAW") != 0) )
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+-SAW"));
	}
	else if( (m_l_sub_sweep_cont_incdec_cntdwn >= 0)     &&
			 (m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI) &&
			 (strcmp(m_sz_sweep_sub_cmd, "+-TRIANGLE") != 0) )
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+-TRIANGLE"));
	}

	// SWEEP-CONT DEC カウントダウン中にデータが -SAW/-TRIANGLE になった場合(項番3)
	if( (m_l_sub_sweep_cont_dec_cntdwn >= 0)       &&
		(strcmp(m_sz_sweep_sub_cont, "SWEEP_ON") == 0) &&
		(strcmp(m_sz_sweep_sub_sts, "RUNNING") == 0)   &&
		((strcmp(m_sz_sweep_sub_cmd, "-SAW") == 0) || (strcmp(m_sz_sweep_sub_cmd, "-TRIANGLE") == 0)) )
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_dec_cntdwn = -1;

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT DEC カウントダウンタイムリミットになった場合
	else if (m_l_sub_sweep_cont_dec_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_dec_cntdwn = -1;
	}
	else if( (m_l_sub_sweep_cont_dec_cntdwn >= 0)        &&
			 (m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW) &&
			 (strcmp(m_sz_sweep_sub_cmd, "-SAW") != 0) )
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-SAW"));
	}
	else if( (m_l_sub_sweep_cont_dec_cntdwn >= 0)        &&
			 (m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI) &&
			 (strcmp(m_sz_sweep_sub_cmd, "-TRIANGLE") != 0) )
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-TRIANGLE"));
	}

	// SWEEP-CONT DECINC カウントダウン中にデータが -+SAW/-+TRIANGLE になった場合(項番4)
	if( (m_l_sub_sweep_cont_decinc_cntdwn >= 0)    &&
		(strcmp(m_sz_sweep_sub_cont, "SWEEP_ON") == 0) &&
		(strcmp(m_sz_sweep_sub_sts, "RUNNING") == 0)   &&
		((strcmp(m_sz_sweep_sub_cmd, "-+SAW") == 0) || (strcmp(m_sz_sweep_sub_cmd, "-+TRIANGLE") == 0)) )
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_decinc_cntdwn = -1;

		// SWEEP実行フラグON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT -+SAW カウントダウンタイムリミットになった場合
	else if (m_l_sub_sweep_cont_decinc_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sub_sweep_cont_decinc_cntdwn = -1;
	}
	else if( (m_l_sub_sweep_cont_decinc_cntdwn >= 0)     &&
			 (m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW) &&
			 (strcmp(m_sz_sweep_sub_cmd, "-+SAW") != 0) )
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-+SAW"));
	}
	else if( (m_l_sub_sweep_cont_decinc_cntdwn >= 0)     &&
			 (m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI) &&
			 (strcmp(m_sz_sweep_sub_cmd, "-+TRIANGLE") != 0) )
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-+TRIANGLE"));
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if( (m_l_sub_sweep_cont_stop_cntdwn == -1)   &&
		(m_l_sub_sweep_cont_inc_cntdwn == -1)    &&
		(m_l_sub_sweep_cont_incdec_cntdwn == -1) &&
		(m_l_sub_sweep_cont_dec_cntdwn == -1)    &&
		(m_l_sub_sweep_cont_decinc_cntdwn == -1) )
	{
		// INC の場合(項番1)
		if( (strcmp(m_sz_sweep_sub_cont, "SWEEP_ON") == 0) &&
			(strcmp(m_sz_sweep_sub_sts, "RUNNING") == 0)   &&
			((strcmp(m_sz_sweep_sub_cmd, "+SAW") == 0) || (strcmp(m_sz_sweep_sub_cmd, "+TRIANGLE") == 0)) )
		{
			show_sub_sweep_cont_inc64();
		}

		// INCDEC の場合(項番2)
		else if( (strcmp(m_sz_sweep_sub_cont, "SWEEP_ON") == 0) &&
				 (strcmp(m_sz_sweep_sub_sts, "RUNNING") == 0)   &&
				 ((strcmp(m_sz_sweep_sub_cmd, "+-SAW") == 0) || (strcmp(m_sz_sweep_sub_cmd, "+-TRIANGLE") == 0)) )
		{
			show_sub_sweep_cont_incdec64();
		}

		// DEC の場合(項番3)
		else if( (strcmp(m_sz_sweep_sub_cont, "SWEEP_ON") == 0) &&
				 (strcmp(m_sz_sweep_sub_sts, "RUNNING") == 0)   &&
				 ((strcmp(m_sz_sweep_sub_cmd, "-SAW") == 0) || (strcmp(m_sz_sweep_sub_cmd, "-TRIANGLE") == 0)) )
		{
			show_sub_sweep_cont_dec64();
		}

		// DECINC の場合(項番4)
		else if( (strcmp(m_sz_sweep_sub_cont, "SWEEP_ON") == 0) &&
				 (strcmp(m_sz_sweep_sub_sts, "RUNNING") == 0)   &&
				 ((strcmp(m_sz_sweep_sub_cmd, "-+SAW") == 0) || (strcmp(m_sz_sweep_sub_cmd, "-+TRIANGLE") == 0)) )
		{
			show_sub_sweep_cont_decinc64();
		}

		// STOP の場合(項番5)
		else
		{
			// CONTがONでSTSがRUNNING以外ならば、CONTをOFFする
			if( (strcmp(m_sz_sweep_sub_cont, "SWEEP_ON") == 0) &&
				(strcmp(m_sz_sweep_sub_sts, "RUNNING") != 0)   &&
				(m_i_sweep_flag == 1) )
			{
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("SWEEP_OFF"));
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("STOP"));

				// SWEEP実行フラグOFF
				m_i_sweep_flag = 0;
			}

			// 2013-03-11 完全に停止してから、STOP表示にする
			if( (strcmp(m_sz_sweep_sub_cont, "SWEEP_OFF") == 0) &&
				(strcmp(m_sz_sweep_sub_cmd, "STOP") == 0)       &&
				((strcmp(m_sz_sweep_sub_sts, "INITIALIZE") == 0) || (strcmp(m_sz_sweep_sub_sts, "STOP") == 0)) )
			{
				show_sub_sweep_cont_stop64();
			}
		}
	}


	/*** SWEEP-NM ************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SWEEP_NUM));
	if (obs_adr != NULL)
	{
		m_d_sweep_num = obs_adr->d_data;
	}

	// 数値表示部を更新
	sprintf_s(sz_buff, sizeof(sz_buff), "%3d", (int)m_d_sweep_num % 1000);
	m_SweepNumBtn.SetWindowTextW(CString(sz_buff));

	// Tri-SWEEP-NUM カウントダウン中にデータが設定値になった場合
	if( (m_l_sweep_num_cntdwn >= 0) &&
		(m_l_sweep_num_set == (long)m_d_sweep_num) )
	{
		// カウントダウン終了
		m_l_sweep_num_cntdwn = -1;
	}
	// Tri-SWEEP-NUM カウントダウンタイムリミットになった場合
	else if (m_l_sweep_num_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_sweep_num_cntdwn = -1;

		// 運用ログに表示
		sprintf_s(sz_buff, sizeof(sz_buff), "SWEEP-NUM %2d", m_l_sweep_num_set);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(sz_buff), _T(""), nLogEx::debug);
	}


	/*** DOPP-CONT <<XUP>> 追加 ***********************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DOPP_PRE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dopp_pre, sizeof(m_sz_dopp_pre), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_DOPP_PRE), CString(m_sz_dopp_pre), nLogEx::debug);
	}
	// DOPP-CONT OFF カウントダウン中にデータが OFF になった場合
	if( (m_l_dopp_cont_off_cntdwn >= 0) &&
		(strcmp(m_sz_dopp_pre, "OFF") == 0) )
	{
		// カウントダウン終了
		m_l_dopp_cont_off_cntdwn = -1;
	}
	// DOPP-CONT OFF カウントダウンタイムリミットになった場合 
	else if (m_l_dopp_cont_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_dopp_cont_off_cntdwn = -1;
	}

	// DOPP-CONT ON カウントダウン中にデータが ON になった場合
	if( (m_l_dopp_cont_on_cntdwn >= 0) &&
		(strcmp(m_sz_dopp_pre, "ON") == 0) )
	{
		// カウントダウン終了
		m_l_dopp_cont_on_cntdwn = -1;
	}
	// DOPP-CONT ON カウントダウンタイムリミットになった場合
	else if (m_l_dopp_cont_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_dopp_cont_on_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if( (m_l_dopp_cont_off_cntdwn == -1) &&
		(m_l_dopp_cont_on_cntdwn == -1) )
	{
		// ON の場合
		if (strcmp(m_sz_dopp_pre, "ON") == 0)
		{
			show_dopp_cont_on64();
		}
		// OFF の場合
		else
		{
			show_dopp_cont_off64();
		}
	}


	/*** TCRMOD MODULATION ****************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_CMD_MOD_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_cmd_mod_x, sizeof(m_sz_cmd_mod_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_CMD_MOD_X), CString(m_sz_cmd_mod_x), nLogEx::debug);
	}

	// TCRMOD MODULATION OFF カウントダウン中にデータが OFF になった場合
	if( (m_l_modulation_off_cntdwn >= 0) &&
		(strcmp(m_sz_cmd_mod_x, "OFF") == 0) )
	{
		// カウントダウン終了
		m_l_modulation_off_cntdwn = -1;
	}
	// TCRMOD MODULATION OFF カウントダウンタイムリミットになった場合
	else if (m_l_modulation_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_modulation_off_cntdwn = -1;
	}

	// TCRMOD MODULATION ON カウントダウン中にデータが ON になった場合
	if( (m_l_modulation_on_cntdwn >= 0) &&
		(strcmp(m_sz_cmd_mod_x, "ON") == 0) )
	{
		// カウントダウン終了
		m_l_modulation_on_cntdwn = -1;
	}
	// TCRMOD MODULATION ON カウントダウンタイムリミットになった場合
	else if (m_l_modulation_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_modulation_on_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if( (m_l_modulation_off_cntdwn == -1) &&
		(m_l_modulation_on_cntdwn == -1) )
	{
		// ON の場合
		if (strcmp(m_sz_cmd_mod_x, "ON") == 0)
		{
			show_modulation_on64();
		}
		// OFF の場合
		else
		{
			show_modulation_off64();
		}
	}


	/*** X-CMDOUT HOLD_TONE 2008-09-01 X-HPA増設対応追加 **********************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_HOLD_TONE_STS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hold_tone_sts_x, sizeof(m_sz_hold_tone_sts_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX54_HOLD_TONE_STS_X), CString(m_sz_hold_tone_sts_x), nLogEx::debug);
	}

	// X-CMDOUT HOLD_TONE OFF カウントダウン中にデータが OFF になった場合
	if( (m_l_hold_tone_off_cntdwn >= 0) &&
		(strcmp(m_sz_hold_tone_sts_x, "OFF") == 0) )
	{
		// カウントダウン終了
		m_l_hold_tone_off_cntdwn = -1;
	}
	// X-CMDOUT HOLD_TONE OFF カウントダウンタイムリミットになった場合
	else if (m_l_hold_tone_off_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_hold_tone_off_cntdwn = -1;
	}

	// X-CMDOUT HOLD_TONE ON カウントダウン中にデータが ON になった場合
	if( (m_l_hold_tone_on_cntdwn >= 0) &&
		(strcmp(m_sz_hold_tone_sts_x, "ON") == 0) )
	{
		// カウントダウン終了
		m_l_hold_tone_on_cntdwn = -1;
	}
	// TCRMOD MODULATION ON カウントダウンタイムリミットになった場合
	else if (m_l_hold_tone_on_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_hold_tone_on_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if( (m_l_hold_tone_off_cntdwn == -1) &&
		(m_l_hold_tone_on_cntdwn == -1) )
	{
		// ON の場合
		if (strcmp(m_sz_hold_tone_sts_x, "ON") == 0)
		{
			show_hold_tone_on64();
		}
		// OFF の場合
		else
		{
			show_hold_tone_off64();
		}
	}


	/*** CMD MOD INDEX ********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_CMD_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_cmd_mod_idx = obs_adr->d_data;

		CString tmp;
		tmp.Format(_T("%f"), obs_adr->d_data);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_CMD_MOD_IDX), tmp, nLogEx::debug);

		// 数値表示部を更新
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_cmd_mod_idx);
		m_strIndexCmd = CString(sz_buff);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("INDEX-CMD"), m_strIndexCmd, nLogEx::debug);
	}

	/*** RNG MOD INDEX ********************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RNG_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_rng_mod_idx = obs_adr->d_data;

		CString tmp;
		tmp.Format(_T("%f"), obs_adr->d_data);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_RNG_MOD_IDX), tmp, nLogEx::debug);

		// 数値表示部を更新
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_rng_mod_idx);
		m_strIndexRng = CString(sz_buff);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("INDEX-RNG"), m_strIndexRng, nLogEx::debug);
	}

	/*** DATA SOURCE **********************************************************************/
	/* 現在の状態を監視ＤＢから取得                                                       */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DATA_SOURCE));
	if (obs_adr != NULL)
	{
		sprintf_s(m_sz_data_src, sizeof(m_sz_data_src), "%s", obs_adr->sz_sts_name);

		// 状態表示部を更新
		m_strIndexCpr = CString(m_sz_data_src);
	}

	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	/*** SS-RNG / SX-RNG ******************************************************************/
	// 現在の状態を監視ＤＢから取得 
	// 選ばれているレンジ装置が従来型か再生型かでRNG回数の表示元を切り替える
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
	if (obs_adr != NULL)
	{
		sprintf_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), "%s", obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_NORM_REP), CString(m_sz_norm_rep), nLogEx::debug);
	}

	// 再生型が選ばれている場合
	if (strcmp(m_sz_norm_rep, "REGEN") == 0)
	{
		// 再生型測距回数を抽出
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RNG_CNT));
		if (obs_adr != NULL)
		{
			m_d_xx_rng_cnt = obs_adr->d_data;
		}
	}
	// 従来型が選ばれている場合
	else if (strcmp(m_sz_norm_rep, "TURNAROUND") == 0)
	{
		// 従来型測距回数を抽出
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RNG_CNT));
		if (obs_adr != NULL)
		{
			m_d_xx_rng_cnt = obs_adr->d_data;
		}
	}
	// 何れでもない場合は０クリア
	else
	{
		m_d_xx_rng_cnt = 0;
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RNG_NUM));
	if (obs_adr != NULL)
	{
		m_d_xx_rng_num = obs_adr->d_data;
	}

	// 数値表示部を更新
	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_xx_rng_cnt % 100);
	m_strXXRng = CString(sz_buff);

	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_xx_rng_num % 100);
	m_XXRngNumBtn.SetWindowTextW(CString(sz_buff));


	/*** "CONTINUATION" 表示有無 および センシティブ操作 **********************************/
	// XX-RNG の数値表示部上の "CONTINUATION" を消す（あれば）
	m_strXXRngCont = _T("");


	/*** COH/INCOH ************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_COH_INCOH));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_coh_incoh, sizeof(m_sz_coh_incoh), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("V-EQU.COHI"), CString(m_sz_coh_incoh), nLogEx::debug);
	}

	// COH/INCOH COH カウントダウン中にデータが COH になった場合
	if( (m_l_coh_incoh_coh_cntdwn >= 0) &&
		(strcmp(m_sz_coh_incoh, "COH") == 0) )
	{
		// カウントダウン終了
		m_l_coh_incoh_coh_cntdwn = -1;
	}
	// COH/INCOH COH カウントダウンタイムリミットになった場合
	else if (m_l_coh_incoh_coh_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_coh_incoh_coh_cntdwn = -1;
	}

	// COH/INCOH INCOH カウントダウン中にデータが INCOH になった場合
	if( (m_l_coh_incoh_incoh_cntdwn >= 0) &&
		(strcmp(m_sz_coh_incoh, "INCOH") == 0) )
	{
		// カウントダウン終了
		m_l_coh_incoh_incoh_cntdwn = -1;
	}
	// SX-RNG ON カウントダウンタイムリミットになった場合
	else if (m_l_coh_incoh_incoh_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_coh_incoh_incoh_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if( (m_l_coh_incoh_coh_cntdwn == -1) &&
		(m_l_coh_incoh_incoh_cntdwn == -1) )
	{
		// INCOH の場合
		if (strcmp(m_sz_coh_incoh, "INCOH") == 0)
		{
			show_coh_incoh_incoh64();
		}
		// COH の場合
		else
		{
			show_coh_incoh_coh64();
		}
	}


	/*** RNGING ***************************************************************************/
	// 現在の状態を監視ＤＢから取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("CTRL.RNG_FLG"), CString(m_sz_rnging), nLogEx::debug);
	}

	// RNGING STOP カウントダウン中にデータが STOP になった場合
	if( (m_l_rnging_stop_cntdwn >= 0) &&
		(strcmp(m_sz_rnging, "終了") == 0) )
	{
		// カウントダウン終了
		m_l_rnging_stop_cntdwn = -1;
	}
	// RNGING STOP カウントダウンタイムリミットになった場合
	else if (m_l_rnging_stop_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rnging_stop_cntdwn = -1;
	}

	// RNGING START カウントダウン中にデータが START になった場合
	if( (m_l_rnging_start_cntdwn >= 0) &&
		(strcmp(m_sz_rnging, "開始") == 0) )
	{
		// カウントダウン終了
		m_l_rnging_start_cntdwn = -1;
	}
	// RNGING START カウントダウンタイムリミットになった場合
	else if (m_l_rnging_start_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rnging_start_cntdwn = -1;
	}

	// (再生型) RNGING STOP カウントダウン中にデータが STOP になった場合
	if( (m_l_rep_rnging_stop_cntdwn >= 0) &&
		(strcmp(m_sz_rnging, "終了") == 0) )
	{
		// カウントダウン終了
		m_l_rep_rnging_stop_cntdwn = -1;
	}
	// (再生型) RNGING STOP カウントダウンタイムリミットになった場合
	else if (m_l_rep_rnging_stop_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rep_rnging_stop_cntdwn = -1;
	}

	// (従来型) レンジ形式切り替え制御の実行結果確認
	if (m_l_dsnrng_turn_start_flag == 1)
	{
		// 従来型切り替え制御実行中
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
		if (obs_adr != NULL)
		{
			strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
		}

		if (strcmp(m_sz_norm_rep, "TURNAROUND") == 0)
		{
			// 制御通知送信 
			m_XXRngNumBtn.GetWindowTextW(str2);
			str1.Format(_T("DX%02d"), _wtoi(str2));
			theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str1);

			// カウントダウン開始
			m_l_rnging_start_cntdwn = RNGING_TIME_XX64_LIMIT;

			//RNG種別フラグを従来型に設定
			m_l_rng_type = RNG_TYPE_NOR;

			// 従来型切り替え制御実行中フラグを戻す
			m_l_dsnrng_turn_start_flag = 0;
		}
	}

	// (再生型) レンジ形式切り替え制御の実行結果確認
	if (m_l_dsnrng_regen_start_flag == 1)
	{
		// 再生型切り替え制御実行中
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
		if (obs_adr != NULL)
		{
			strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
		}

		if (strcmp(m_sz_norm_rep, "REGEN") == 0)
		{
			// 制御通知送信
			m_XXRngNumBtn.GetWindowTextW(str2);
			str1.Format(_T("DX%02d"), _wtoi(str2));
			theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str1);

			// カウントダウン開始
			m_l_rep_rnging_start_cntdwn = REP_RNGING_TIME_XX64_LIMIT;

			// RNG種別フラグを再生型に設定
			m_l_rng_type = RNG_TYPE_REP;

			// 再生型切り替え制御実行中フラグを戻す
			m_l_dsnrng_regen_start_flag = 0;
		}
	}

	// (再生型) RNGING START カウントダウン中にデータが START になった場合
	if( (m_l_rep_rnging_start_cntdwn >= 0) &&
		(strcmp(m_sz_rnging, "開始") == 0) )
	{
		// カウントダウン終了
		m_l_rep_rnging_start_cntdwn = -1;
	}
	// (再生型) RNGING START カウントダウンタイムリミットになった場合
	else if (m_l_rep_rnging_start_cntdwn == 0)
	{
		// カウントダウン終了
		m_l_rep_rnging_start_cntdwn = -1;
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if( (m_l_rnging_stop_cntdwn == -1) &&
		(m_l_rnging_start_cntdwn == -1) )
	{
		// START の場合
		if (strcmp(m_sz_rnging, "開始") == 0)
		{
			show_rnging_start64();
			show_rrng_start64();
		}
		// STOP の場合
		else
		{
			show_rnging_stop64();
			show_rrng_stop64();
		}
	}

	// カウントダウン終了中であればボタンのピックスマップ貼り替え可能
	if( (m_l_rep_rnging_stop_cntdwn == -1) &&
		(m_l_rep_rnging_start_cntdwn == -1) )
	{
		// START の場合
		if (strcmp(m_sz_rnging, "開始") == 0)
		{
			show_rrng_start64();
		}
		// STOP の場合
		else
		{
			show_rrng_stop64();
		}
	}

	UpdateData(FALSE);
}

/*============================================================================*/
// RF-POWER ON状態における OFF/ONボタンの画面表示
/*============================================================================*/
void CTrsControlXX::show_rf_power_on54()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	// SSPA LAN 接続状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_LAN_SSPA));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_sspa, sizeof(m_sz_lan_sspa), obs_adr->sz_sts_name);
	}

	// X-UC LAN 接続状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	// X-TX_IF LAN 接続状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_LAN_X_TX_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx_if, sizeof(m_sz_lan_x_tx_if), obs_adr->sz_sts_name);
	}

	// SSPA の リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_SSPA_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sspa_rl, sizeof(m_sz_sspa_rl), obs_adr->sz_sts_name);
	}

	// X-TX_IF の リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_X_TX_IF_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_tx_if_rl, sizeof(m_sz_x_tx_if_rl), obs_adr->sz_sts_name);
	}

	//RF POWER ON ENABLE 状態をチェック
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
	}
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_RF_ON_ENBL_SSPA));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_sspa, sizeof(m_sz_rf_on_enbl_sspa), obs_adr->sz_sts_name);
	}

	// ONボタンの状態表示
	if( (strcmp(m_sz_lan_sspa, "接続") == 0)        &&		// CTRL.LAN_05_6
		(strcmp(m_sz_lan_x_uc, "接続") == 0)        &&		// CTRL.LAN_05_7
		(strcmp(m_sz_lan_x_tx_if, "接続") == 0)     &&		// CTRL.LAN_07_2
		(strcmp(m_sz_sspa_rl, "REMOTE") == 0)       &&		// X-SSPA.CONT_MODE
		(strcmp(m_sz_x_tx_if_rl, "REMOTE") == 0)    &&		// X-TX_IF.CONTROL_MODE
		(strcmp(m_sz_rf_on_enbl_x, "NOT_ALM") == 0) &&		// ANT.LOW_EL
#ifdef _GREAT_XSSPA_ISO
		((strcmp(m_sz_rf_on_enbl_sspa, "READY") == 0) || (strcmp(m_sz_rf_on_enbl_sspa, "TX_ON_ENGAGED") == 0)))			// X-SSPA.TX_ON_READY
#else
		((strcmp(m_sz_rf_on_enbl_sspa, "READY") == 0) || (strcmp(m_sz_rf_on_enbl_sspa, "TX_ON_ENGAGE") == 0)))			// X-SSPA.TX_ON_READY
#endif // _GREAT_XSSPA_ISO
	{
		// リモート（選択）
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES);
	}
	else
	{ 
		// ローカル
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);
	}

	// OFFボタンの状態表示
	if( (strcmp(m_sz_lan_sspa, "接続") == 0)     &&			// CTRL.LAN_05_6
		(strcmp(m_sz_lan_x_uc, "接続") == 0)     &&			// CTRL.LAN_05_7
		(strcmp(m_sz_lan_x_tx_if, "接続") == 0)  &&			// CTRL.LAN_07_2
		(strcmp(m_sz_sspa_rl, "REMOTE") == 0)    &&			// X-SSPA.CONT_MODE
		(strcmp(m_sz_x_tx_if_rl, "REMOTE") == 0) &&			// X-TX_IF.CONTROL_MODE
		(m_i_priv_flag == 1) )
	{
		// リモート（非選択）
		m_RfPwrOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_RfPwrOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RfPwrOnBtn.Invalidate();
	m_RfPwrOffBtn.Invalidate();
}

/*============================================================================*/
// RF-POWER OFF状態における OFF/ONボタンの画面表示
/*============================================================================*/
void CTrsControlXX::show_rf_power_off54()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	// SSPA LAN 接続状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_LAN_SSPA));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_sspa, sizeof(m_sz_lan_sspa), obs_adr->sz_sts_name);
	}

	// X-UC LAN 接続状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	// X-TX_IF LAN 接続状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_LAN_X_TX_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx_if, sizeof(m_sz_lan_x_tx_if), obs_adr->sz_sts_name);
	}

	// SSPA の リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_SSPA_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sspa_rl, sizeof(m_sz_sspa_rl), obs_adr->sz_sts_name);
	}

	// X-TX_IF の リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_X_TX_IF_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_tx_if_rl, sizeof(m_sz_x_tx_if_rl), obs_adr->sz_sts_name);
	}

	//RF POWER ON ENABLE 状態をチェック
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
	}
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_RF_ON_ENBL_SSPA));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_sspa, sizeof(m_sz_rf_on_enbl_sspa), obs_adr->sz_sts_name);
	}

	// OFFボタンの状態表示
	if( (strcmp(m_sz_lan_sspa, "接続") == 0)    &&			// CTRL.LAN_05_6
		(strcmp(m_sz_lan_x_uc, "接続") == 0)    &&			// CTRL.LAN_05_7
		(strcmp(m_sz_lan_x_tx_if, "接続") == 0) &&			// CTRL.LAN_07_2
		(strcmp(m_sz_sspa_rl, "REMOTE") == 0)   &&			// X-SSPA.CONT_MODE
		(strcmp(m_sz_x_tx_if_rl, "REMOTE") == 0) )			// X-TX_IF.CONTROL_MODE
	{
		// リモート（選択）
		m_RfPwrOffBtn.SetColor(COLOR_REMOTES);
	}
	else
	{
		// ローカル
		m_RfPwrOffBtn.SetColor(COLOR_LOCAL);
	}

	// ONボタンの状態表示
	if( (strcmp(m_sz_lan_sspa, "接続") == 0)        &&		// CTRL.LAN_05_6
		(strcmp(m_sz_lan_x_uc, "接続") == 0)        &&		// CTRL.LAN_05_7
		(strcmp(m_sz_lan_x_tx_if, "接続") == 0)     &&		// CTRL.LAN_07_2
		(strcmp(m_sz_sspa_rl, "REMOTE") == 0)       &&		// X-SSPA.CONT_MODE
		(strcmp(m_sz_x_tx_if_rl, "REMOTE") == 0)    &&		// X-TX_IF.CONTROL_MODE
		(strcmp(m_sz_rf_on_enbl_x, "NOT_ALM") == 0) &&		// ANT.LOW_EL
#ifdef _GREAT_XSSPA_ISO
		((strcmp(m_sz_rf_on_enbl_sspa, "READY") == 0) || (strcmp(m_sz_rf_on_enbl_sspa, "TX_ON_ENGAGED") == 0)) &&		// X-SSPA.TX_ON_READY
#else
		((strcmp(m_sz_rf_on_enbl_sspa, "READY") == 0) || (strcmp(m_sz_rf_on_enbl_sspa, "TX_ON_ENGAGE") == 0)) &&		// X-SSPA.TX_ON_READY
#endif // _GREAT_XSSPA_ISO
		(m_i_priv_flag == 1) )
	{
		// リモート（非選択）
		m_RfPwrOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{ 
		// ローカル
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}

/*============================================================================*/
// NO-USE ON状態における OFF/ONボタンの画面表示
//
// (未使用のため、画面非表示）
//
/*============================================================================*/
void CTrsControlXX::show_no_use_on54()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	// X-UC LAN 接続状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	// X-TX_IF LAN 接続状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_LAN_X_TX_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx_if, sizeof(m_sz_lan_x_tx_if), obs_adr->sz_sts_name);
	}

	// SSPA の リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_SSPA_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sspa_rl, sizeof(m_sz_sspa_rl), obs_adr->sz_sts_name);
	}

	//RF-TRIP NOUSE ON ENABLE 状態をチェック
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
	}

	// ONボタンの状態表示
	if( (strcmp(m_sz_lan_x_uc, "接続") == 0)    &&		// CTRL.LAN_05_7
		(strcmp(m_sz_lan_x_tx_if, "接続") == 0) &&		// CTRL.LAN_07_2
		(strcmp(m_sz_sspa_rl, "REMOTE") == 0)   &&		// X-SSPA.CONT_MODE
		(strcmp(m_sz_no_use_enbl_x, "OFF") == 0) )		// SSPA.RF_CUT_NO_USE
	{
		// リモート（選択）
		m_NoUseOnBtn.SetColor(COLOR_REMOTES);
	}
	else
	{
		// ローカル
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}

	// OFFボタンの状態表示
	if( (strcmp(m_sz_lan_x_uc, "接続") == 0)    &&		// CTRL.LAN_05_7
		(strcmp(m_sz_lan_x_tx_if, "接続") == 0) &&		// CTRL.LAN_07_2
		(strcmp(m_sz_sspa_rl, "REMOTE") == 0)   &&		// X-SSPA.CONT_MODE
		(m_i_priv_flag == 1))
	{
		// リモート（非選択）
		m_NoUseOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_NoUseOffBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_NoUseOnBtn.Invalidate();
	m_NoUseOffBtn.Invalidate();
}

/*============================================================================*/
// NO-USE OFF状態における OFF/ONボタンの画面表示
//
// (未使用のため、画面非表示）
//
/*============================================================================*/
void CTrsControlXX::show_no_use_off54()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// 条件チェック用監視項目を参照
	// X-UC LAN 接続状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	// X-TX_IF LAN 接続状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_LAN_X_TX_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx_if, sizeof(m_sz_lan_x_tx_if), obs_adr->sz_sts_name);
	}

	// SSPA の リモート／ローカル 状態
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_SSPA_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sspa_rl, sizeof(m_sz_sspa_rl), obs_adr->sz_sts_name);
	}

	// OFFボタンの状態表示
	if( (strcmp(m_sz_lan_x_uc, "接続") == 0)    &&
		(strcmp(m_sz_lan_x_tx_if, "接続") == 0) &&
		(strcmp(m_sz_sspa_rl, "REMOTE") == 0) )
	{
		// リモート（選択）
		m_NoUseOffBtn.SetColor(COLOR_REMOTES);
	}
	else
	{
		// ローカル
		m_NoUseOffBtn.SetColor(COLOR_LOCAL);
	}

	// ONボタンの状態表示
	if( (strcmp(m_sz_lan_x_uc, "接続") == 0)    &&
		(strcmp(m_sz_lan_x_tx_if, "接続") == 0) &&
		(strcmp(m_sz_sspa_rl, "REMOTE") == 0)   &&
		(m_i_priv_flag == 1) )
	{
		// リモート（非選択）
		m_NoUseOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ローカル
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}

	// 更新
	m_NoUseOffBtn.Invalidate();
	m_NoUseOnBtn.Invalidate();
}

