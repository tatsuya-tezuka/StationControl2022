/*============================================================================*/
/*! TrsControlXX.cpp

-# X�ё��M������ (54m�p)
*/
/*============================================================================*/
// TrsControlXX.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrsControlXX.h"
#include "afxdialogex.h"
#include "TrsControlMng.h"
#include "HookMsgBox.h"

/*============================================================================*/
// ���M�����ʏ�ԕ\���ݒ� (54m)
/*============================================================================*/
void CTrsControlXX::show_opsc_condition54()
{
	//TRACE(_T("show_opsc_condition\n"));

	char sz_buff[64];
	CString str1, str2;

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// ���䌠�͏�ɗL��
	m_i_priv_flag = 1;

	/*-- TX CONTROL ----------------------------------------------------------------------*/

	/*** �����[�g�^���[�J�� ��Ԏ擾 ******************************************************/

	// S-TXSYN �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_DSNTCR_RL), CString(m_sz_dsntcr_rl), nLogEx::debug);
	}

	// X-UC �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_X_UC_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_uc_rl, sizeof(m_sz_x_uc_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_X_UC_RL), CString(m_sz_x_uc_rl), nLogEx::debug);
	}

	// X-TX_IF �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_X_TX_IF_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_tx_if_rl, sizeof(m_sz_x_tx_if_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX54_X_TX_IF_RL), CString(m_sz_x_tx_if_rl), nLogEx::debug);
	}

	/*** �A���[���`�F�b�N  <<XUP>> X-TX�p�ɕύX *******************************************/
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

	// �A���[����
	if (strcmp(m_sz_alarm_x, "ON") == 0)
	{
		// �g��Ԃ�����
		m_grRfPower.SetBackColor(COLOR_ALARM);
		m_grNoUse.SetBackColor(COLOR_ALARM);
		m_grSweepCont.SetBackColor(COLOR_ALARM);
		m_grSweepContSub.SetBackColor(COLOR_ALARM);
		m_grDoppCont.SetBackColor(COLOR_ALARM);
		m_grModulation.SetBackColor(COLOR_ALARM);
		m_grHoldTone.SetBackColor(COLOR_ALARM);
	}
	// �A���[������
	else
	{
		// �g�̐F�����ɖ߂�
		m_grRfPower.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grNoUse.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepCont.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepContSub.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grDoppCont.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grModulation.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grHoldTone.SetBackColor(GetSysColor(COLOR_3DFACE));
	}


	/*** RF-POWER *************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
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

	// RF-POWER ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if( (m_l_rf_power_on_cntdwn >= 0) &&
		((strcmp(m_sz_rf_power_x, "ON") == 0) && (strcmp(m_sz_rf_power_sspa, "ON") == 0)) )
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
	if( (m_l_rf_power_off_cntdwn >= 0) &&
		((strcmp(m_sz_rf_power_x, "OFF") == 0) || (strcmp(m_sz_rf_power_sspa, "OFF") == 0)) )
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
	if( (m_l_rf_power_off_cntdwn == -1) &&
		(m_l_rf_power_on_cntdwn == -1) )
	{
		// ON �̏ꍇ
		if( (strcmp(m_sz_rf_power_x, "ON") == 0) && (strcmp(m_sz_rf_power_sspa, "ON") == 0) )
		{
			show_rf_power_on54();
		}
		// OFF �̏ꍇ
		else
		{
			/*** ���ꏈ������ *************************************************************/
			show_rf_power_off54();
		}
	}


	/*** NO-USE ***************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_NO_USE_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_x, sizeof(m_sz_no_use_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX54_NO_USE_X), CString(m_sz_no_use_x), nLogEx::debug);
	}

	// NO-USE ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if( (m_l_no_use_on_cntdwn >= 0) &&
		(strcmp(m_sz_no_use_x, "ON") == 0) )
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
	if( (m_l_no_use_off_cntdwn >= 0) &&
		(strcmp(m_sz_no_use_x, "ON") != 0) )
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
	if( (m_l_no_use_off_cntdwn == -1) &&
		( m_l_no_use_on_cntdwn == -1) )
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_no_use_x, "ON") == 0)
		{
			show_no_use_on54();
		}
		// OFF �̏ꍇ
		else
		{
			show_no_use_off54();
		}
	}


	/*** MAIN SWEEP-CONT ***********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
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

	// SWEEP-CONT STOP �J�E���g�_�E�����ɊĎ��f�[�^�� STOP �ɂȂ����ꍇ
	if( (m_l_main_sweep_cont_stop_cntdwn >= 0)       &&
		((strcmp(m_sz_sweep_cont, "SWEEP_OFF") == 0) ||
		 (strcmp(m_sz_sweep_cmd, "STOP") == 0)       ||
		 (strcmp(m_sz_sweep_sts, "STOP") == 0)) )
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_stop_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;

		// SWEEP���s�t���OOFF
		m_i_sweep_flag = 0;
	}
	// SWEEP-CONT STOP �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_main_sweep_cont_stop_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_stop_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}

	// SWEEP-CONT INC �J�E���g�_�E�����Ƀf�[�^�� +SAW/+TRIANGLE �ɂȂ����ꍇ(����1)
	if( (m_l_main_sweep_cont_inc_cntdwn >= 0)      &&
		(strcmp(m_sz_sweep_cont, "SWEEP_ON") == 0) &&
		(strcmp(m_sz_sweep_sts, "RUNNING") == 0)   &&
		((strcmp(m_sz_sweep_cmd, "+SAW") == 0) || (strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0)) )
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_inc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +SAW �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_main_sweep_cont_inc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_inc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if( (m_l_main_sweep_cont_inc_cntdwn >= 0)        &&
			 (m_i_main_sweep_cont_mode == SWEEP_MODE_SAW) &&
			 (strcmp(m_sz_sweep_cmd, "+SAW") != 0) )
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
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
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT INCDEC �J�E���g�_�E�����Ƀf�[�^�� +-SAW/+-TRIANGLE �ɂȂ����ꍇ(����2)
	if( (m_l_main_sweep_cont_incdec_cntdwn >= 0)   &&
		(strcmp(m_sz_sweep_cont, "SWEEP_ON") == 0) &&
		(strcmp(m_sz_sweep_sts, "RUNNING") == 0)   &&
		((strcmp(m_sz_sweep_cmd, "+-SAW") == 0) || (strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0)) )
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_incdec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +-SAW/TRIANGLE �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_main_sweep_cont_incdec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_incdec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if( (m_l_main_sweep_cont_incdec_cntdwn >= 0)     &&
			 (m_i_main_sweep_cont_mode == SWEEP_MODE_SAW) &&
			 (strcmp(m_sz_sweep_cmd, "+-SAW") != 0) )
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
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
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+-TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT DEC �J�E���g�_�E�����Ƀf�[�^�� -SAW/-TRIANGLE �ɂȂ����ꍇ(����3)
	if( (m_l_main_sweep_cont_dec_cntdwn >= 0)      &&
		(strcmp(m_sz_sweep_cont, "SWEEP_ON") == 0) &&
		(strcmp(m_sz_sweep_sts, "RUNNING") == 0)   &&
		((strcmp(m_sz_sweep_cmd, "-SAW") == 0) || (strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0)) )
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_dec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT DEC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_main_sweep_cont_dec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_dec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if( (m_l_main_sweep_cont_dec_cntdwn >= 0)        &&
			 (m_i_main_sweep_cont_mode == SWEEP_MODE_SAW) &&
			 (strcmp(m_sz_sweep_cmd, "-SAW") != 0) )
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
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
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT DECINC �J�E���g�_�E�����Ƀf�[�^�� -+SAW/-+TRIANGLE �ɂȂ����ꍇ(����4)
	if( (m_l_main_sweep_cont_decinc_cntdwn >= 0)   &&
		(strcmp(m_sz_sweep_cont, "SWEEP_ON") == 0) &&
		(strcmp(m_sz_sweep_sts, "RUNNING") == 0)   &&
		((strcmp(m_sz_sweep_cmd, "-+SAW") == 0) || (strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0)) )
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_decinc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT -+SAW �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_main_sweep_cont_decinc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_decinc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if( (m_l_main_sweep_cont_decinc_cntdwn >= 0)     &&
			 (m_i_main_sweep_cont_mode == SWEEP_MODE_SAW) &&
			 (strcmp(m_sz_sweep_cmd, "-+SAW") != 0) )
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
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
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-+TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if( (m_l_main_sweep_cont_stop_cntdwn == -1)   &&
		(m_l_main_sweep_cont_inc_cntdwn == -1)    &&
		(m_l_main_sweep_cont_incdec_cntdwn == -1) &&
		(m_l_main_sweep_cont_dec_cntdwn == -1)    &&
		(m_l_main_sweep_cont_decinc_cntdwn == -1) )
	{
		// INC �̏ꍇ(����1)
		if( (strcmp(m_sz_sweep_cont, "SWEEP_ON") == 0) &&
			(strcmp(m_sz_sweep_sts, "RUNNING") == 0)   &&
			((strcmp(m_sz_sweep_cmd, "+SAW") == 0) || (strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0)) )
		{
			show_main_sweep_cont_inc64();
		}

		// INCDEC �̏ꍇ(����2)
		else if( (strcmp(m_sz_sweep_cont, "SWEEP_ON") == 0) &&
				 (strcmp(m_sz_sweep_sts, "RUNNING") == 0)   &&
				 ((strcmp(m_sz_sweep_cmd, "+-SAW") == 0) || (strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0)) )
		{
			show_main_sweep_cont_incdec64();
		}

		// DEC �̏ꍇ(����3)
		else if( (strcmp(m_sz_sweep_cont, "SWEEP_ON") == 0) &&
				 (strcmp(m_sz_sweep_sts, "RUNNING") == 0)   &&
				 ((strcmp(m_sz_sweep_cmd, "-SAW") == 0) || (strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0)) )
		{
			show_main_sweep_cont_dec64();
		}

		// DECINC �̏ꍇ(����4)
		else if( (strcmp(m_sz_sweep_cont, "SWEEP_ON") == 0) &&
				 (strcmp(m_sz_sweep_sts, "RUNNING") == 0)   &&
				 ((strcmp(m_sz_sweep_cmd, "-+SAW") == 0) || (strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0)) )
		{
			show_main_sweep_cont_decinc64();
		}

		// STOP �̏ꍇ(����5)
		else {
			// CONT��ON��STS��RUNNING�ȊO�Ȃ�΁ACONT��OFF����
			if( (strcmp(m_sz_sweep_cont, "SWEEP_ON") == 0) &&
				(strcmp(m_sz_sweep_sts, "RUNNING") != 0)   &&
				(m_i_sweep_flag == 1) )
			{
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("SWEEP_OFF"));
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("STOP"));

				// SWEEP���s�t���OOFF
				m_i_sweep_flag = 0;
			}

			// ���S�ɒ�~���Ă���ASTOP�\���ɂ���
			if( (strcmp(m_sz_sweep_cont, "SWEEP_OFF") == 0) &&
				(strcmp(m_sz_sweep_cmd, "STOP") == 0)       &&
				((strcmp(m_sz_sweep_sts, "INITIALIZE") == 0) || (strcmp(m_sz_sweep_sts, "STOP") == 0)) )
			{
				show_main_sweep_cont_stop64();
			}
		}
	}


	/*** SUB SWEEP-CONT ***********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
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

	// SWEEP-CONT STOP �J�E���g�_�E�����ɊĎ��f�[�^�� STOP �ɂȂ����ꍇ
	if( (m_l_sub_sweep_cont_stop_cntdwn >= 0)        &&
		((strcmp(m_sz_sweep_sub_cont, "SWEEP_OFF") == 0) ||
		 (strcmp(m_sz_sweep_sub_cmd, "STOP") == 0)       ||
		 (strcmp(m_sz_sweep_sub_sts, "STOP") == 0)) )
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_stop_cntdwn = -1;

		// SWEEP���s�t���OOFF
		m_i_sweep_flag = 0;
	}
	// SWEEP-CONT STOP �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sub_sweep_cont_stop_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_stop_cntdwn = -1;
	}

	// SWEEP-CONT INC �J�E���g�_�E�����Ƀf�[�^�� +SAW/+TRIANGLE �ɂȂ����ꍇ(����1)
	if( (m_l_sub_sweep_cont_inc_cntdwn >= 0)       &&
		(strcmp(m_sz_sweep_sub_cont, "SWEEP_ON") == 0) &&
		(strcmp(m_sz_sweep_sub_sts, "RUNNING") == 0)   &&
		((strcmp(m_sz_sweep_sub_cmd, "+SAW") == 0) || (strcmp(m_sz_sweep_sub_cmd, "+TRIANGLE") == 0)) )
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_inc_cntdwn = -1;

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +SAW �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sub_sweep_cont_inc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
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

	// SWEEP-CONT INCDEC �J�E���g�_�E�����Ƀf�[�^�� +-SAW/+-TRIANGLE �ɂȂ����ꍇ(����2)
	if( (m_l_sub_sweep_cont_incdec_cntdwn >= 0)    &&
		(strcmp(m_sz_sweep_sub_cont, "SWEEP_ON") == 0) &&
		(strcmp(m_sz_sweep_sub_sts, "RUNNING") == 0)   &&
		((strcmp(m_sz_sweep_sub_cmd, "+-SAW") == 0) || (strcmp(m_sz_sweep_sub_cmd, "+-TRIANGLE") == 0)) )
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_incdec_cntdwn = -1;

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +-SAW/TRIANGLE �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sub_sweep_cont_incdec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
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

	// SWEEP-CONT DEC �J�E���g�_�E�����Ƀf�[�^�� -SAW/-TRIANGLE �ɂȂ����ꍇ(����3)
	if( (m_l_sub_sweep_cont_dec_cntdwn >= 0)       &&
		(strcmp(m_sz_sweep_sub_cont, "SWEEP_ON") == 0) &&
		(strcmp(m_sz_sweep_sub_sts, "RUNNING") == 0)   &&
		((strcmp(m_sz_sweep_sub_cmd, "-SAW") == 0) || (strcmp(m_sz_sweep_sub_cmd, "-TRIANGLE") == 0)) )
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_dec_cntdwn = -1;

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT DEC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sub_sweep_cont_dec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
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

	// SWEEP-CONT DECINC �J�E���g�_�E�����Ƀf�[�^�� -+SAW/-+TRIANGLE �ɂȂ����ꍇ(����4)
	if( (m_l_sub_sweep_cont_decinc_cntdwn >= 0)    &&
		(strcmp(m_sz_sweep_sub_cont, "SWEEP_ON") == 0) &&
		(strcmp(m_sz_sweep_sub_sts, "RUNNING") == 0)   &&
		((strcmp(m_sz_sweep_sub_cmd, "-+SAW") == 0) || (strcmp(m_sz_sweep_sub_cmd, "-+TRIANGLE") == 0)) )
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_decinc_cntdwn = -1;

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT -+SAW �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sub_sweep_cont_decinc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
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

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if( (m_l_sub_sweep_cont_stop_cntdwn == -1)   &&
		(m_l_sub_sweep_cont_inc_cntdwn == -1)    &&
		(m_l_sub_sweep_cont_incdec_cntdwn == -1) &&
		(m_l_sub_sweep_cont_dec_cntdwn == -1)    &&
		(m_l_sub_sweep_cont_decinc_cntdwn == -1) )
	{
		// INC �̏ꍇ(����1)
		if( (strcmp(m_sz_sweep_sub_cont, "SWEEP_ON") == 0) &&
			(strcmp(m_sz_sweep_sub_sts, "RUNNING") == 0)   &&
			((strcmp(m_sz_sweep_sub_cmd, "+SAW") == 0) || (strcmp(m_sz_sweep_sub_cmd, "+TRIANGLE") == 0)) )
		{
			show_sub_sweep_cont_inc64();
		}

		// INCDEC �̏ꍇ(����2)
		else if( (strcmp(m_sz_sweep_sub_cont, "SWEEP_ON") == 0) &&
				 (strcmp(m_sz_sweep_sub_sts, "RUNNING") == 0)   &&
				 ((strcmp(m_sz_sweep_sub_cmd, "+-SAW") == 0) || (strcmp(m_sz_sweep_sub_cmd, "+-TRIANGLE") == 0)) )
		{
			show_sub_sweep_cont_incdec64();
		}

		// DEC �̏ꍇ(����3)
		else if( (strcmp(m_sz_sweep_sub_cont, "SWEEP_ON") == 0) &&
				 (strcmp(m_sz_sweep_sub_sts, "RUNNING") == 0)   &&
				 ((strcmp(m_sz_sweep_sub_cmd, "-SAW") == 0) || (strcmp(m_sz_sweep_sub_cmd, "-TRIANGLE") == 0)) )
		{
			show_sub_sweep_cont_dec64();
		}

		// DECINC �̏ꍇ(����4)
		else if( (strcmp(m_sz_sweep_sub_cont, "SWEEP_ON") == 0) &&
				 (strcmp(m_sz_sweep_sub_sts, "RUNNING") == 0)   &&
				 ((strcmp(m_sz_sweep_sub_cmd, "-+SAW") == 0) || (strcmp(m_sz_sweep_sub_cmd, "-+TRIANGLE") == 0)) )
		{
			show_sub_sweep_cont_decinc64();
		}

		// STOP �̏ꍇ(����5)
		else
		{
			// CONT��ON��STS��RUNNING�ȊO�Ȃ�΁ACONT��OFF����
			if( (strcmp(m_sz_sweep_sub_cont, "SWEEP_ON") == 0) &&
				(strcmp(m_sz_sweep_sub_sts, "RUNNING") != 0)   &&
				(m_i_sweep_flag == 1) )
			{
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("SWEEP_OFF"));
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("STOP"));

				// SWEEP���s�t���OOFF
				m_i_sweep_flag = 0;
			}

			// 2013-03-11 ���S�ɒ�~���Ă���ASTOP�\���ɂ���
			if( (strcmp(m_sz_sweep_sub_cont, "SWEEP_OFF") == 0) &&
				(strcmp(m_sz_sweep_sub_cmd, "STOP") == 0)       &&
				((strcmp(m_sz_sweep_sub_sts, "INITIALIZE") == 0) || (strcmp(m_sz_sweep_sub_sts, "STOP") == 0)) )
			{
				show_sub_sweep_cont_stop64();
			}
		}
	}


	/*** SWEEP-NM ************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SWEEP_NUM));
	if (obs_adr != NULL)
	{
		m_d_sweep_num = obs_adr->d_data;
	}

	// ���l�\�������X�V
	sprintf_s(sz_buff, sizeof(sz_buff), "%3d", (int)m_d_sweep_num % 1000);
	m_SweepNumBtn.SetWindowTextW(CString(sz_buff));

	// Tri-SWEEP-NUM �J�E���g�_�E�����Ƀf�[�^���ݒ�l�ɂȂ����ꍇ
	if( (m_l_sweep_num_cntdwn >= 0) &&
		(m_l_sweep_num_set == (long)m_d_sweep_num) )
	{
		// �J�E���g�_�E���I��
		m_l_sweep_num_cntdwn = -1;
	}
	// Tri-SWEEP-NUM �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sweep_num_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_num_cntdwn = -1;

		// �^�p���O�ɕ\��
		sprintf_s(sz_buff, sizeof(sz_buff), "SWEEP-NUM %2d", m_l_sweep_num_set);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(sz_buff), _T(""), nLogEx::debug);
	}


	/*** DOPP-CONT <<XUP>> �ǉ� ***********************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DOPP_PRE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dopp_pre, sizeof(m_sz_dopp_pre), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_DOPP_PRE), CString(m_sz_dopp_pre), nLogEx::debug);
	}
	// DOPP-CONT OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if( (m_l_dopp_cont_off_cntdwn >= 0) &&
		(strcmp(m_sz_dopp_pre, "OFF") == 0) )
	{
		// �J�E���g�_�E���I��
		m_l_dopp_cont_off_cntdwn = -1;
	}
	// DOPP-CONT OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ 
	else if (m_l_dopp_cont_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_dopp_cont_off_cntdwn = -1;
	}

	// DOPP-CONT ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if( (m_l_dopp_cont_on_cntdwn >= 0) &&
		(strcmp(m_sz_dopp_pre, "ON") == 0) )
	{
		// �J�E���g�_�E���I��
		m_l_dopp_cont_on_cntdwn = -1;
	}
	// DOPP-CONT ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_dopp_cont_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_dopp_cont_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if( (m_l_dopp_cont_off_cntdwn == -1) &&
		(m_l_dopp_cont_on_cntdwn == -1) )
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_dopp_pre, "ON") == 0)
		{
			show_dopp_cont_on64();
		}
		// OFF �̏ꍇ
		else
		{
			show_dopp_cont_off64();
		}
	}


	/*** TCRMOD MODULATION ****************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_CMD_MOD_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_cmd_mod_x, sizeof(m_sz_cmd_mod_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_CMD_MOD_X), CString(m_sz_cmd_mod_x), nLogEx::debug);
	}

	// TCRMOD MODULATION OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if( (m_l_modulation_off_cntdwn >= 0) &&
		(strcmp(m_sz_cmd_mod_x, "OFF") == 0) )
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
	if( (m_l_modulation_on_cntdwn >= 0) &&
		(strcmp(m_sz_cmd_mod_x, "ON") == 0) )
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
	if( (m_l_modulation_off_cntdwn == -1) &&
		(m_l_modulation_on_cntdwn == -1) )
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_cmd_mod_x, "ON") == 0)
		{
			show_modulation_on64();
		}
		// OFF �̏ꍇ
		else
		{
			show_modulation_off64();
		}
	}


	/*** X-CMDOUT HOLD_TONE 2008-09-01 X-HPA���ݑΉ��ǉ� **********************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_HOLD_TONE_STS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hold_tone_sts_x, sizeof(m_sz_hold_tone_sts_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX54_HOLD_TONE_STS_X), CString(m_sz_hold_tone_sts_x), nLogEx::debug);
	}

	// X-CMDOUT HOLD_TONE OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if( (m_l_hold_tone_off_cntdwn >= 0) &&
		(strcmp(m_sz_hold_tone_sts_x, "OFF") == 0) )
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
	if( (m_l_hold_tone_on_cntdwn >= 0) &&
		(strcmp(m_sz_hold_tone_sts_x, "ON") == 0) )
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
	if( (m_l_hold_tone_off_cntdwn == -1) &&
		(m_l_hold_tone_on_cntdwn == -1) )
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


	/*** CMD MOD INDEX ********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_CMD_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_cmd_mod_idx = obs_adr->d_data;

		CString tmp;
		tmp.Format(_T("%f"), obs_adr->d_data);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_CMD_MOD_IDX), tmp, nLogEx::debug);

		// ���l�\�������X�V
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_cmd_mod_idx);
		m_strIndexCmd = CString(sz_buff);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("INDEX-CMD"), m_strIndexCmd, nLogEx::debug);
	}

	/*** RNG MOD INDEX ********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RNG_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_rng_mod_idx = obs_adr->d_data;

		CString tmp;
		tmp.Format(_T("%f"), obs_adr->d_data);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_RNG_MOD_IDX), tmp, nLogEx::debug);

		// ���l�\�������X�V
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_rng_mod_idx);
		m_strIndexRng = CString(sz_buff);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("INDEX-RNG"), m_strIndexRng, nLogEx::debug);
	}

	/*** DATA SOURCE **********************************************************************/
	/* ���݂̏�Ԃ��Ď��c�a����擾                                                       */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DATA_SOURCE));
	if (obs_adr != NULL)
	{
		sprintf_s(m_sz_data_src, sizeof(m_sz_data_src), "%s", obs_adr->sz_sts_name);

		// ��ԕ\�������X�V
		m_strIndexCpr = CString(m_sz_data_src);
	}

	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	/*** SS-RNG / SX-RNG ******************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾 
	// �I�΂�Ă��郌���W���u���]���^���Đ��^����RNG�񐔂̕\������؂�ւ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
	if (obs_adr != NULL)
	{
		sprintf_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), "%s", obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_NORM_REP), CString(m_sz_norm_rep), nLogEx::debug);
	}

	// �Đ��^���I�΂�Ă���ꍇ
	if (strcmp(m_sz_norm_rep, "REGEN") == 0)
	{
		// �Đ��^�����񐔂𒊏o
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RNG_CNT));
		if (obs_adr != NULL)
		{
			m_d_xx_rng_cnt = obs_adr->d_data;
		}
	}
	// �]���^���I�΂�Ă���ꍇ
	else if (strcmp(m_sz_norm_rep, "TURNAROUND") == 0)
	{
		// �]���^�����񐔂𒊏o
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RNG_CNT));
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

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RNG_NUM));
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
	// XX-RNG �̐��l�\������� "CONTINUATION" �������i����΁j
	m_strXXRngCont = _T("");


	/*** COH/INCOH ************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_COH_INCOH));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_coh_incoh, sizeof(m_sz_coh_incoh), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("V-EQU.COHI"), CString(m_sz_coh_incoh), nLogEx::debug);
	}

	// COH/INCOH COH �J�E���g�_�E�����Ƀf�[�^�� COH �ɂȂ����ꍇ
	if( (m_l_coh_incoh_coh_cntdwn >= 0) &&
		(strcmp(m_sz_coh_incoh, "COH") == 0) )
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
	if( (m_l_coh_incoh_incoh_cntdwn >= 0) &&
		(strcmp(m_sz_coh_incoh, "INCOH") == 0) )
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
	if( (m_l_coh_incoh_coh_cntdwn == -1) &&
		(m_l_coh_incoh_incoh_cntdwn == -1) )
	{
		// INCOH �̏ꍇ
		if (strcmp(m_sz_coh_incoh, "INCOH") == 0)
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
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("CTRL.RNG_FLG"), CString(m_sz_rnging), nLogEx::debug);
	}

	// RNGING STOP �J�E���g�_�E�����Ƀf�[�^�� STOP �ɂȂ����ꍇ
	if( (m_l_rnging_stop_cntdwn >= 0) &&
		(strcmp(m_sz_rnging, "�I��") == 0) )
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
	if( (m_l_rnging_start_cntdwn >= 0) &&
		(strcmp(m_sz_rnging, "�J�n") == 0) )
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
	if( (m_l_rep_rnging_stop_cntdwn >= 0) &&
		(strcmp(m_sz_rnging, "�I��") == 0) )
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

	// (�]���^) �����W�`���؂�ւ�����̎��s���ʊm�F
	if (m_l_dsnrng_turn_start_flag == 1)
	{
		// �]���^�؂�ւ�������s��
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
		if (obs_adr != NULL)
		{
			strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
		}

		if (strcmp(m_sz_norm_rep, "TURNAROUND") == 0)
		{
			// ����ʒm���M 
			m_XXRngNumBtn.GetWindowTextW(str2);
			str1.Format(_T("DX%02d"), _wtoi(str2));
			theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str1);

			// �J�E���g�_�E���J�n
			m_l_rnging_start_cntdwn = RNGING_TIME_XX64_LIMIT;

			//RNG��ʃt���O���]���^�ɐݒ�
			m_l_rng_type = RNG_TYPE_NOR;

			// �]���^�؂�ւ�������s���t���O��߂�
			m_l_dsnrng_turn_start_flag = 0;
		}
	}

	// (�Đ��^) �����W�`���؂�ւ�����̎��s���ʊm�F
	if (m_l_dsnrng_regen_start_flag == 1)
	{
		// �Đ��^�؂�ւ�������s��
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
		if (obs_adr != NULL)
		{
			strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
		}

		if (strcmp(m_sz_norm_rep, "REGEN") == 0)
		{
			// ����ʒm���M
			m_XXRngNumBtn.GetWindowTextW(str2);
			str1.Format(_T("DX%02d"), _wtoi(str2));
			theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str1);

			// �J�E���g�_�E���J�n
			m_l_rep_rnging_start_cntdwn = REP_RNGING_TIME_XX64_LIMIT;

			// RNG��ʃt���O���Đ��^�ɐݒ�
			m_l_rng_type = RNG_TYPE_REP;

			// �Đ��^�؂�ւ�������s���t���O��߂�
			m_l_dsnrng_regen_start_flag = 0;
		}
	}

	// (�Đ��^) RNGING START �J�E���g�_�E�����Ƀf�[�^�� START �ɂȂ����ꍇ
	if( (m_l_rep_rnging_start_cntdwn >= 0) &&
		(strcmp(m_sz_rnging, "�J�n") == 0) )
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
	if( (m_l_rnging_stop_cntdwn == -1) &&
		(m_l_rnging_start_cntdwn == -1) )
	{
		// START �̏ꍇ
		if (strcmp(m_sz_rnging, "�J�n") == 0)
		{
			show_rnging_start64();
			show_rrng_start64();
		}
		// STOP �̏ꍇ
		else
		{
			show_rnging_stop64();
			show_rrng_stop64();
		}
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if( (m_l_rep_rnging_stop_cntdwn == -1) &&
		(m_l_rep_rnging_start_cntdwn == -1) )
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

	UpdateData(FALSE);
}

/*============================================================================*/
// RF-POWER ON��Ԃɂ����� OFF/ON�{�^���̉�ʕ\��
/*============================================================================*/
void CTrsControlXX::show_rf_power_on54()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	// SSPA LAN �ڑ����
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_LAN_SSPA));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_sspa, sizeof(m_sz_lan_sspa), obs_adr->sz_sts_name);
	}

	// X-UC LAN �ڑ����
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	// X-TX_IF LAN �ڑ����
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_LAN_X_TX_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx_if, sizeof(m_sz_lan_x_tx_if), obs_adr->sz_sts_name);
	}

	// SSPA �� �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_SSPA_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sspa_rl, sizeof(m_sz_sspa_rl), obs_adr->sz_sts_name);
	}

	// X-TX_IF �� �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_X_TX_IF_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_tx_if_rl, sizeof(m_sz_x_tx_if_rl), obs_adr->sz_sts_name);
	}

	//RF POWER ON ENABLE ��Ԃ��`�F�b�N
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

	// ON�{�^���̏�ԕ\��
	if( (strcmp(m_sz_lan_sspa, "�ڑ�") == 0)        &&		// CTRL.LAN_05_6
		(strcmp(m_sz_lan_x_uc, "�ڑ�") == 0)        &&		// CTRL.LAN_05_7
		(strcmp(m_sz_lan_x_tx_if, "�ڑ�") == 0)     &&		// CTRL.LAN_07_2
		(strcmp(m_sz_sspa_rl, "REMOTE") == 0)       &&		// X-SSPA.CONT_MODE
		(strcmp(m_sz_x_tx_if_rl, "REMOTE") == 0)    &&		// X-TX_IF.CONTROL_MODE
		(strcmp(m_sz_rf_on_enbl_x, "NOT_ALM") == 0) &&		// ANT.LOW_EL
#ifdef _GREAT_XSSPA_ISO
		((strcmp(m_sz_rf_on_enbl_sspa, "READY") == 0) || (strcmp(m_sz_rf_on_enbl_sspa, "TX_ON_ENGAGED") == 0)))			// X-SSPA.TX_ON_READY
#else
		((strcmp(m_sz_rf_on_enbl_sspa, "READY") == 0) || (strcmp(m_sz_rf_on_enbl_sspa, "TX_ON_ENGAGE") == 0)))			// X-SSPA.TX_ON_READY
#endif // _GREAT_XSSPA_ISO
	{
		// �����[�g�i�I���j
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES);
	}
	else
	{ 
		// ���[�J��
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);
	}

	// OFF�{�^���̏�ԕ\��
	if( (strcmp(m_sz_lan_sspa, "�ڑ�") == 0)     &&			// CTRL.LAN_05_6
		(strcmp(m_sz_lan_x_uc, "�ڑ�") == 0)     &&			// CTRL.LAN_05_7
		(strcmp(m_sz_lan_x_tx_if, "�ڑ�") == 0)  &&			// CTRL.LAN_07_2
		(strcmp(m_sz_sspa_rl, "REMOTE") == 0)    &&			// X-SSPA.CONT_MODE
		(strcmp(m_sz_x_tx_if_rl, "REMOTE") == 0) &&			// X-TX_IF.CONTROL_MODE
		(m_i_priv_flag == 1) )
	{
		// �����[�g�i��I���j
		m_RfPwrOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_RfPwrOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RfPwrOnBtn.Invalidate();
	m_RfPwrOffBtn.Invalidate();
}

/*============================================================================*/
// RF-POWER OFF��Ԃɂ����� OFF/ON�{�^���̉�ʕ\��
/*============================================================================*/
void CTrsControlXX::show_rf_power_off54()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	// SSPA LAN �ڑ����
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_LAN_SSPA));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_sspa, sizeof(m_sz_lan_sspa), obs_adr->sz_sts_name);
	}

	// X-UC LAN �ڑ����
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	// X-TX_IF LAN �ڑ����
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_LAN_X_TX_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx_if, sizeof(m_sz_lan_x_tx_if), obs_adr->sz_sts_name);
	}

	// SSPA �� �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_SSPA_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sspa_rl, sizeof(m_sz_sspa_rl), obs_adr->sz_sts_name);
	}

	// X-TX_IF �� �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_X_TX_IF_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_x_tx_if_rl, sizeof(m_sz_x_tx_if_rl), obs_adr->sz_sts_name);
	}

	//RF POWER ON ENABLE ��Ԃ��`�F�b�N
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

	// OFF�{�^���̏�ԕ\��
	if( (strcmp(m_sz_lan_sspa, "�ڑ�") == 0)    &&			// CTRL.LAN_05_6
		(strcmp(m_sz_lan_x_uc, "�ڑ�") == 0)    &&			// CTRL.LAN_05_7
		(strcmp(m_sz_lan_x_tx_if, "�ڑ�") == 0) &&			// CTRL.LAN_07_2
		(strcmp(m_sz_sspa_rl, "REMOTE") == 0)   &&			// X-SSPA.CONT_MODE
		(strcmp(m_sz_x_tx_if_rl, "REMOTE") == 0) )			// X-TX_IF.CONTROL_MODE
	{
		// �����[�g�i�I���j
		m_RfPwrOffBtn.SetColor(COLOR_REMOTES);
	}
	else
	{
		// ���[�J��
		m_RfPwrOffBtn.SetColor(COLOR_LOCAL);
	}

	// ON�{�^���̏�ԕ\��
	if( (strcmp(m_sz_lan_sspa, "�ڑ�") == 0)        &&		// CTRL.LAN_05_6
		(strcmp(m_sz_lan_x_uc, "�ڑ�") == 0)        &&		// CTRL.LAN_05_7
		(strcmp(m_sz_lan_x_tx_if, "�ڑ�") == 0)     &&		// CTRL.LAN_07_2
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
		// �����[�g�i��I���j
		m_RfPwrOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{ 
		// ���[�J��
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}

/*============================================================================*/
// NO-USE ON��Ԃɂ����� OFF/ON�{�^���̉�ʕ\��
//
// (���g�p�̂��߁A��ʔ�\���j
//
/*============================================================================*/
void CTrsControlXX::show_no_use_on54()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	// X-UC LAN �ڑ����
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	// X-TX_IF LAN �ڑ����
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_LAN_X_TX_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx_if, sizeof(m_sz_lan_x_tx_if), obs_adr->sz_sts_name);
	}

	// SSPA �� �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_SSPA_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sspa_rl, sizeof(m_sz_sspa_rl), obs_adr->sz_sts_name);
	}

	//RF-TRIP NOUSE ON ENABLE ��Ԃ��`�F�b�N
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
	}

	// ON�{�^���̏�ԕ\��
	if( (strcmp(m_sz_lan_x_uc, "�ڑ�") == 0)    &&		// CTRL.LAN_05_7
		(strcmp(m_sz_lan_x_tx_if, "�ڑ�") == 0) &&		// CTRL.LAN_07_2
		(strcmp(m_sz_sspa_rl, "REMOTE") == 0)   &&		// X-SSPA.CONT_MODE
		(strcmp(m_sz_no_use_enbl_x, "OFF") == 0) )		// SSPA.RF_CUT_NO_USE
	{
		// �����[�g�i�I���j
		m_NoUseOnBtn.SetColor(COLOR_REMOTES);
	}
	else
	{
		// ���[�J��
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}

	// OFF�{�^���̏�ԕ\��
	if( (strcmp(m_sz_lan_x_uc, "�ڑ�") == 0)    &&		// CTRL.LAN_05_7
		(strcmp(m_sz_lan_x_tx_if, "�ڑ�") == 0) &&		// CTRL.LAN_07_2
		(strcmp(m_sz_sspa_rl, "REMOTE") == 0)   &&		// X-SSPA.CONT_MODE
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
	m_NoUseOnBtn.Invalidate();
	m_NoUseOffBtn.Invalidate();
}

/*============================================================================*/
// NO-USE OFF��Ԃɂ����� OFF/ON�{�^���̉�ʕ\��
//
// (���g�p�̂��߁A��ʔ�\���j
//
/*============================================================================*/
void CTrsControlXX::show_no_use_off54()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	// X-UC LAN �ڑ����
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	// X-TX_IF LAN �ڑ����
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_LAN_X_TX_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx_if, sizeof(m_sz_lan_x_tx_if), obs_adr->sz_sts_name);
	}

	// SSPA �� �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX54_SSPA_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sspa_rl, sizeof(m_sz_sspa_rl), obs_adr->sz_sts_name);
	}

	// OFF�{�^���̏�ԕ\��
	if( (strcmp(m_sz_lan_x_uc, "�ڑ�") == 0)    &&
		(strcmp(m_sz_lan_x_tx_if, "�ڑ�") == 0) &&
		(strcmp(m_sz_sspa_rl, "REMOTE") == 0) )
	{
		// �����[�g�i�I���j
		m_NoUseOffBtn.SetColor(COLOR_REMOTES);
	}
	else
	{
		// ���[�J��
		m_NoUseOffBtn.SetColor(COLOR_LOCAL);
	}

	// ON�{�^���̏�ԕ\��
	if( (strcmp(m_sz_lan_x_uc, "�ڑ�") == 0)    &&
		(strcmp(m_sz_lan_x_tx_if, "�ڑ�") == 0) &&
		(strcmp(m_sz_sspa_rl, "REMOTE") == 0)   &&
		(m_i_priv_flag == 1) )
	{
		// �����[�g�i��I���j
		m_NoUseOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_NoUseOffBtn.Invalidate();
	m_NoUseOnBtn.Invalidate();
}

