#include "stdafx.h"
#include "SatelliteData.h"
#include "StationControl.h"
#include "SummaryPropertiesDlg.h"
#include "EquipmentCtrlNum.h"
#include "sys/stat.h"
#include "TimeLine.h"
#include "EngValCnv.h"
#include "EquipmentCtrlStatus.h"
#include "EquipmentCtrlTime.h"
#include "EquipmentCtrlBitRate.h"
#include "..\Utility\StringOprTool.h"
#include "..\Utility\SshShell.h"
#include "EquipmentCtrl2Num.h"
#include "EquipmentCtrlSameNum.h"

#include <ctime>
#include <locale.h>

#include <iostream>
#include <fstream>
#include <mlang.h>

#define ARGMAX     16
#define OPBUFSIZE   80
#define ARGBUFSIZE  16000 
#define SATDATA_BUFF_SIZE 1024

#ifdef _DEBUG_RNGPROPDUMMY
//----------------------------------------------------------------------------------------
// RNG�`���_�~�[�f�[�^�I��
// �P�c64m 0:S/S, 1:X/X�i���n�F�]���j, 2:X/X�i���n�F�Đ��j, 3:X/X�i�V�n�F�]���j, 4:X/X�i�V�n�G�Đ��j
// �P�c54m 3:X/X or X/Ka�i�]���j, 4:X/X or X/Ka�i�Đ��j
// ���V�Y34m 0:S/S, 3:X/X�i�V�n�F�]���j, 4:X/X�i�V�n�G�Đ��j
// ���V�Y20m 0:S/S, 
UINT mRngTransModeDummySelect = 4;
#endif

/* ------------------------------------------------------------------------------------ */
/* RNG�`��
/* ------------------------------------------------------------------------------------ */
CRangePropagation::CRangePropagation()
: m_2WayRange(0.0)
, m_eRngStatus(eStop)
, m_nRngNum(0)
, m_rtt(0.0)
, m_nTransMode(eRngTransMode_SS)
, m_nGeneMode(eRngGeneration_Past)
, m_nStep(eRngStep_1)
, m_nSndCount(0)
{
}


CRangePropagation::~CRangePropagation()
{
	m_queRngSnd.clear();
	m_queRngRcv.clear();
	m_queRngAll.clear();
}


/*============================================================================*/
/*! CRangePropagation

-# �_�~�[�f�[�^����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CRangePropagation::CreateDummy()
{
#ifdef _DEBUG_RNGPROPDUMMY
	time_t t = m_dataTime.GetTime();
	if (t % 1 == 0)
	{
		// �Đ��^�ȊO
		if (mRngTransModeDummySelect < 2 || mRngTransModeDummySelect == 3)
		{
			if (mRngStepDummy[mRngTransModeDummySelect][4].status.Compare(_T("A_CODE")) == 0)
			{
				mRngStepDummy[mRngTransModeDummySelect][4].status = _T("OTHER");
			}
			else
			{
				mRngStepDummy[mRngTransModeDummySelect][4].status = _T("A_CODE");
			}
		}
	}
	if (t % 5 == 0)
	{
		// �Đ��^
		if (mRngTransModeDummySelect == 2 || mRngTransModeDummySelect == 4)
		{
			mRngStepDummy[mRngTransModeDummySelect][4].status = _T("AB_CODE");
		}
	}

	if (t % 40 == 0)
	{
		mRngStepDummy[mRngTransModeDummySelect][1].status = _T("START");
		mRngStepDummy[mRngTransModeDummySelect][3].status = _T("START");
	}
#endif
}


/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# �X�e�[�^�X������

@param
@retval

*/
/*============================================================================*/
void CRangePropagation::InitStatus()
{
	m_nStation = theApp.GetSelectStation();
	m_nTransMode = eRngTransMode_SS;
	m_nGeneMode = eRngGeneration_Past;
	m_nStep = eRngStep_1;
	m_strLastRngToneStatus = _T("");
	m_eRngStatus = eStop;
	m_nRngNum = 0;
	m_2WayRange = 0.0;
	m_nSndCount = 0;
}

/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# �X�e�[�^�X������(54m)

@param
@retval

*/
/*============================================================================*/
void CRangePropagation::InitStatus54()
{
	m_nStation = theApp.GetSelectStation();
	m_nTransMode = eRngTransMode_XX_XKA_54m;
	m_nGeneMode = eRngGeneration_Past;
	m_nStep = eRngStep_1;
	m_strLastRngToneStatus = _T("");
	m_eRngStatus = eStop;
	m_nRngNum = 0;
	m_2WayRange = 0.0;
	m_nSndCount = 0;
}

/*============================================================================*/
/*! CRangePropagation

-# �X�e�b�v�̍X�V

@param �Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CRangePropagation::UpdateStep()
{
	BOOL bStep = FALSE;

	while (TRUE)
	{
		if (m_nStation == eStation_Usuda642)
		{
			// 54m��
			bStep = UpdateStepProc54();
		}
		else
		{
			// ���̑��̋�
			bStep = UpdateStepProc();
		}

		if (bStep == FALSE)
		{
			// �X�e�b�v���ς�Ȃ��ꍇ�̓��[�v�𔲂���
			break;
		}
	}
}


/*============================================================================*/
/*! CRangePropagation

-# �X�e�b�v�̍X�V

@param �Ȃ�
@retval	TRUE:�X�e�b�v���X�V���ꂽ / FALSE:�X�e�b�v���ς��Ȃ�

*/
/*============================================================================*/
BOOL CRangePropagation::UpdateStepProc()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

	CString monitorString;
	CString str;

	UINT nOldStep = m_nStep;

	// �����擾
	theApp.GetCurrentUtcTime(m_dataTime);

	{
		CString strDebug;
		strDebug.Format(_T("Step(%u), m_dataTime(%I64d)"), m_nStep, m_dataTime.GetTime());
		CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStep"), _T("detail"), strDebug, nLogEx::detail);
	}

	switch (m_nStep)
	{
	case eRngStep_1:
		// step1
		InitStatus();
//		if (GetKanshiDouble() == 1)
//			// step2��
//			m_nStep = eRngStep_2;

		monitorString = mRngStep[m_nStation][m_nTransMode][m_nStep].nameK;
		if (!monitorString.IsEmpty())
		{
			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStation, monitorString);
#ifdef _DEBUG_RNGPROPDUMMY
			obs_adr = new CEngValCnv::obsname_t;
			for (int i = 0; i < eRngStep_Num; i++)
			{
				if (monitorString.Compare(mRngStepDummy[mRngTransModeDummySelect][i].nameK) == 0)
				{
					char szStatus[256];
					WideCharToMultiByte(CP_ACP, 0, mRngStepDummy[mRngTransModeDummySelect][i].status, -1, szStatus, sizeof(szStatus), NULL, NULL);
					sprintf_s(obs_adr->sz_sts_name, 64, ("%s"), szStatus);
					obs_adr->l_data = (long)mRngStepDummy[mRngTransModeDummySelect][i].val;
					obs_adr->d_data = mRngStepDummy[mRngTransModeDummySelect][i].val;
					obs_adr->l_inflg = INFLG_ON;
					break;
				}
			}
#endif
			if (obs_adr == NULL)
			{
				break;
			}
			if (obs_adr->l_inflg != INFLG_ON)
			{
				break;
			}
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
#ifdef _DEBUG_RNGPROPDUMMY
			delete obs_adr;
#endif
			if (str.Compare(mRngStep[m_nStation][m_nTransMode][m_nStep].value1) == 0)
			{
				// step2��
				m_nStep = eRngStep_2;

				{
					CString strDebug;
					strDebug.Format(_T("Step to (%u) with Kanshi(%s)"), m_nStep, (LPCTSTR)str);
					CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStep"), _T("debug"), strDebug, nLogEx::debug);
				}
			}
		}
		break;

	case eRngStep_2:
	{
		// step2
		int szControl = sizeof(mRngStep[m_nStation][m_nTransMode]) / sizeof(mRngStep[m_nStation][m_nTransMode][0]);
		for (int i = 0; i < szControl; i++)
		{
			monitorString = mRngStep[m_nStation][i][m_nStep].nameK;
			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStation, monitorString);
#ifdef _DEBUG_RNGPROPDUMMY
			obs_adr = new CEngValCnv::obsname_t;
			for (int j = 0; j < eRngStep_Num; j++)
			{
				if (monitorString.Compare(mRngStepDummy[mRngTransModeDummySelect][j].nameK) == 0)
				{
					char szStatus[256];
					WideCharToMultiByte(CP_ACP, 0, mRngStepDummy[mRngTransModeDummySelect][j].status, -1, szStatus, sizeof(szStatus), NULL, NULL);
					sprintf_s(obs_adr->sz_sts_name, 64, ("%s"), szStatus);
					obs_adr->l_data = (long)mRngStepDummy[mRngTransModeDummySelect][j].val;
					obs_adr->d_data = mRngStepDummy[mRngTransModeDummySelect][j].val;
					obs_adr->l_inflg = INFLG_ON;
					break;
				}
			}
#endif
			if (obs_adr == NULL)
			{
				continue;
			}
			if (obs_adr->l_inflg != INFLG_ON)
			{
#ifdef _DEBUG_RNGPROPDUMMY
				delete obs_adr;
#endif
				continue;
			}
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
#ifdef _DEBUG_RNGPROPDUMMY
			delete obs_adr;
#endif
			if (str.Compare(mRngStep[m_nStation][m_nTransMode][m_nStep].value1) == 0)
			{
				// ���[�h�ݒ�ƁAstep3��
				m_nTransMode = i;
				m_nStep = eRngStep_3;

				{
					CString strDebug;
					strDebug.Format(_T("Step to (%u) with Kanshi(%s)"), m_nStep, (LPCTSTR)str);
					CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStep"), _T("debug"), strDebug, nLogEx::debug);
				}
				break;
			}
		}
		break;
	}

	case eRngStep_3:
		if (m_nTransMode == eRngTransMode_SS)
		{
			m_nStep = eRngStep_4;

			{
				CString strDebug;
				strDebug.Format(_T("Step to (%u) with RngTransMode is SS"), m_nStep);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStep"), _T("debug"), strDebug, nLogEx::debug);
			}
			break;
		}
		str = GetKanshiString();
		if (str.Compare(mRngStep[m_nStation][m_nTransMode][m_nStep].value1) == 0)
		{
			if (m_nTransMode == eRngTransMode_XXOldRegene)
			{
				m_nGeneMode = eRngGeneration_Regene;
			}
			else
			{
				m_nGeneMode = eRngGeneration_Past;
			}
			m_nStep = eRngStep_4;

			{
				CString strDebug;
				strDebug.Format(_T("Step to (%u) with Kanshi(%s), GneneMode(%u)"), m_nStep, (LPCTSTR)str, m_nGeneMode);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStep"), _T("debug"), strDebug, nLogEx::debug);
			}
		}
		else if (!mRngStep[m_nStation][m_nTransMode][m_nStep].value2.IsEmpty() && str.Compare(mRngStep[m_nStation][m_nTransMode][m_nStep].value2) == 0)
		{
			m_nGeneMode = eRngGeneration_Regene;
			m_nStep = eRngStep_4;

			{
				CString strDebug;
				strDebug.Format(_T("Step to (%u) with Kanshi(%s), GneneMode(%u)"), m_nStep, (LPCTSTR)str, m_nGeneMode);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStep"), _T("debug"), strDebug, nLogEx::debug);
			}
		}
		break;

	case eRngStep_4:
		str = GetKanshiString();
		// START���m
		if (str.Compare(mRngStep[m_nStation][m_nTransMode][m_nStep].value1) == 0)
		{
			m_eRngStatus = eStart;
			m_nStep = eRngStep_5;
			InitRngTone();

			{
				CString strDebug;
				strDebug.Format(_T("Step to (%u) with Kanshi(%s), RngStatus(%u)"), m_nStep, (LPCTSTR)str, m_eRngStatus);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStep"), _T("debug"), strDebug, nLogEx::debug);
			}
		}
		break;

	case eRngStep_5:
		str = GetKanshiString(m_nStep - 1);
		// STOP���m
		if (str.Compare(mRngStep[m_nStation][m_nTransMode][m_nStep - 1].value2) == 0)
		{
			m_eRngStatus = eStop;
			m_nStep = eRngStep_6;

			{
				CString strDebug;
				strDebug.Format(_T("Step to (%u) with Kanshi(%s), RngStatus(%u)"), m_nStep, (LPCTSTR)str, m_eRngStatus);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStep"), _T("debug"), strDebug, nLogEx::debug);
			}
			break;
		}

		// �X�e�[�^�X�؂�ւ�莞
		str = GetKanshiString(m_nStep);
		// �]���^�̏ꍇ
		if (m_nGeneMode == eRngGeneration_Past)
		{
			{
				CString strDebug;
				strDebug.Format(_T("Step to (%u) with Kanshi(%s), Jyuraigata, wait Code(%s)"), m_nStep, (LPCTSTR)str, (LPCTSTR)mRngStep[m_nStation][m_nTransMode][m_nStep].value1);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStep"), _T("debug"), strDebug, nLogEx::debug);
			}
			if (str.Compare(m_strLastRngToneStatus) != 0)
			{
				m_strLastRngToneStatus = str;
				{
					CString strDebug;
					strDebug.Format(_T("Step to (%u) with Kanshi(%s), Jyuraigata change string, wait Code(%s)"), m_nStep, (LPCTSTR)str, (LPCTSTR)mRngStep[m_nStation][m_nTransMode][m_nStep].value1);
					CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStep"), _T("debug"), strDebug, nLogEx::debug);
				}
				if (str.Compare(mRngStep[m_nStation][m_nTransMode][m_nStep].value1) == 0)
				{
					{
						CString strDebug;
						strDebug.Format(_T("Step to (%u) with Kanshi(%s), Jyuraigata detect Code(%s)"), m_nStep, (LPCTSTR)str, (LPCTSTR)mRngStep[m_nStation][m_nTransMode][m_nStep].value1);
						CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStep"), _T("debug"), strDebug, nLogEx::debug);
					}
					// �����W�g�[�����o�M���̌��o
					DetectRngTone();
				}
			}
		}
		// �Đ��^�̏ꍇ
		else
		{
			{
				CString strDebug;
				strDebug.Format(_T("Step to (%u) with Kanshi(%s), Saiseigata, wait Code(%s)"), m_nStep, (LPCTSTR)str, (LPCTSTR)mRngStep[m_nStation][m_nTransMode][m_nStep].value1);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStep"), _T("debug"), strDebug, nLogEx::debug);
			}
			if (str.Compare(mRngStep[m_nStation][m_nTransMode][m_nStep].value2) == 0)
			{
				{
					CString strDebug;
					strDebug.Format(_T("Step to (%u) with Kanshi(%s), Saiseigata, detect Code(%s)"), m_nStep, (LPCTSTR)str, (LPCTSTR)mRngStep[m_nStation][m_nTransMode][m_nStep].value1);
					CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStep"), _T("debug"), strDebug, nLogEx::debug);
				}
				// �����W�g�[�����o�M���̌��o
				DetectRngTone();
			}
		}

		// �����W�g�[���ړ�
		MoveRngTone();
		break;

	case eRngStep_6:
		// �����W�g�[���ړ�
		MoveRngTone();
		CGraphMutex::Lock(eRngProp);
		// �����W�g�[�������ׂċA���Ă���܂ő҂�
		if (m_queRngSnd.empty() && m_queRngRcv.empty())
		{
			m_nStep = eRngStep_1;

			{
				CString strDebug;
				strDebug.Format(_T("Step to (%u) with RngSnd and RngRcv completed"), m_nStep);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStep"), _T("debug"), strDebug, nLogEx::debug);
			}
		}
		CGraphMutex::Unlock(eRngProp);
		break;

	default:
		break;
	}

	if (m_nStep == nOldStep)
	{
		// �X�e�b�v���ς�Ȃ�
		return FALSE;
	}

	// �X�e�b�v���ς����ꍇ
	return TRUE;
}

/*============================================================================*/
/*! CRangePropagation

-# �X�e�b�v�̍X�V(54m��)

@param �Ȃ�
@retval	TRUE:�X�e�b�v���X�V���ꂽ / FALSE:�X�e�b�v���ς��Ȃ�

*/
/*============================================================================*/
BOOL CRangePropagation::UpdateStepProc54()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

	CString monitorString;
	CString str;

	UINT nOldStep = m_nStep;

	// �����擾
	theApp.GetCurrentUtcTime(m_dataTime);

	{
		CString strDebug;
		strDebug.Format(_T("Step(%u), m_dataTime(%I64d)"), m_nStep, m_dataTime.GetTime());
		CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStepProc54"), _T("detail"), strDebug, nLogEx::detail);
	}

	switch (m_nStep)
	{
	case eRngStep_1:
		// step1
		InitStatus54();
		//		if (GetKanshiDouble() == 1)
		//			// step2��
		//			m_nStep = eRngStep_2;

		monitorString = mRngStep[m_nStation][m_nTransMode][m_nStep].nameK;
		if (!monitorString.IsEmpty())
		{
			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStation, monitorString);
#ifdef _DEBUG_RNGPROPDUMMY
			obs_adr = new CEngValCnv::obsname_t;
			for (int i = 0; i < eRngStep_Num; i++)
			{
				if (monitorString.Compare(mRngStepDummy[mRngTransModeDummySelect][i].nameK) == 0)
				{
					char szStatus[256];
					WideCharToMultiByte(CP_ACP, 0, mRngStepDummy[mRngTransModeDummySelect][i].status, -1, szStatus, sizeof(szStatus), NULL, NULL);
					sprintf_s(obs_adr->sz_sts_name, 64, ("%s"), szStatus);
					obs_adr->l_data = (long)mRngStepDummy[mRngTransModeDummySelect][i].val;
					obs_adr->d_data = mRngStepDummy[mRngTransModeDummySelect][i].val;
					obs_adr->l_inflg = INFLG_ON;
					break;
				}
			}
#endif
			if (obs_adr == NULL)
			{
				break;
			}
			if (obs_adr->l_inflg != INFLG_ON)
			{
				break;
			}
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
#ifdef _DEBUG_RNGPROPDUMMY
			delete obs_adr;
#endif
			if (str.Compare(mRngStep[m_nStation][m_nTransMode][m_nStep].value1) == 0)
			{
				// step2��
				m_nStep = eRngStep_2;

				{
					CString strDebug;
					strDebug.Format(_T("Step to (%u) with Kanshi(%s)"), m_nStep, (LPCTSTR)str);
					CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStepProc54"), _T("debug"), strDebug, nLogEx::debug);
				}
			}
		}
		break;

	case eRngStep_2:
	{
		// step2
		int szControl = sizeof(mRngStep[m_nStation][m_nTransMode]) / sizeof(mRngStep[m_nStation][m_nTransMode][0]);
		for (int i = 0; i < szControl; i++)
		{
			monitorString = mRngStep[m_nStation][i][m_nStep].nameK;
			if (monitorString.IsEmpty())
			{
				break;
			}

			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStation, monitorString);
#ifdef _DEBUG_RNGPROPDUMMY
			obs_adr = new CEngValCnv::obsname_t;
			for (int j = 0; j < eRngStep_Num; j++)
			{
				if (monitorString.Compare(mRngStepDummy[mRngTransModeDummySelect][j].nameK) == 0)
				{
					char szStatus[256];
					WideCharToMultiByte(CP_ACP, 0, mRngStepDummy[mRngTransModeDummySelect][j].status, -1, szStatus, sizeof(szStatus), NULL, NULL);
					sprintf_s(obs_adr->sz_sts_name, 64, ("%s"), szStatus);
					obs_adr->l_data = (long)mRngStepDummy[mRngTransModeDummySelect][j].val;
					obs_adr->d_data = mRngStepDummy[mRngTransModeDummySelect][j].val;
					obs_adr->l_inflg = INFLG_ON;
					break;
				}
			}
#endif
			if (obs_adr == NULL)
			{
				continue;
			}
			if (obs_adr->l_inflg != INFLG_ON)
			{
#ifdef _DEBUG_RNGPROPDUMMY
				delete obs_adr;
#endif
				continue;
			}
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
#ifdef _DEBUG_RNGPROPDUMMY
			delete obs_adr;
#endif
			if (str.Compare(mRngStep[m_nStation][m_nTransMode][m_nStep].value1) == 0)
			{
				// ���[�h�ݒ�ƁAstep3��
				m_nTransMode = i;
				m_nStep = eRngStep_3;

				{
					CString strDebug;
					strDebug.Format(_T("Step to (%u) with Kanshi(%s)"), m_nStep, (LPCTSTR)str);
					CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStepProc54"), _T("debug"), strDebug, nLogEx::debug);
				}
				break;
			}
		}
		break;
	}

	case eRngStep_3:
		str = GetKanshiString();
		if (str.Compare(mRngStep[m_nStation][m_nTransMode][m_nStep].value1) == 0)
		{
			m_nGeneMode = eRngGeneration_Past;
			m_nStep = eRngStep_4;
			{
				CString strDebug;
				strDebug.Format(_T("Step to (%u) with Kanshi(%s), GneneMode(%u)"), m_nStep, (LPCTSTR)str, m_nGeneMode);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStepProc54"), _T("debug"), strDebug, nLogEx::debug);
			}
		}
		else if (!mRngStep[m_nStation][m_nTransMode][m_nStep].value2.IsEmpty() && str.Compare(mRngStep[m_nStation][m_nTransMode][m_nStep].value2) == 0)
		{
			m_nGeneMode = eRngGeneration_Regene;
			m_nStep = eRngStep_4;
			{
				CString strDebug;
				strDebug.Format(_T("Step to (%u) with Kanshi(%s), GneneMode(%u)"), m_nStep, (LPCTSTR)str, m_nGeneMode);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStepProc54"), _T("debug"), strDebug, nLogEx::debug);
			}
		}
		break;

	case eRngStep_4:
		str = GetKanshiString();
		// START���m
		if (str.Compare(mRngStep[m_nStation][m_nTransMode][m_nStep].value1) == 0)
		{
			m_eRngStatus = eStart;
			m_nStep = eRngStep_5;
			InitRngTone();

			{
				CString strDebug;
				strDebug.Format(_T("Step to (%u) with Kanshi(%s), RngStatus(%u)"), m_nStep, (LPCTSTR)str, m_eRngStatus);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStepProc54"), _T("debug"), strDebug, nLogEx::debug);
			}
		}
		break;

	case eRngStep_5:
		str = GetKanshiString(m_nStep - 1);
		// STOP���m
		if (str.Find(mRngStep[m_nStation][m_nTransMode][m_nStep - 1].value2) == 0)
		{
			// 54m��"STOP"������̐擪����̕�����v������STOP���m
			m_eRngStatus = eStop;
			m_nStep = eRngStep_6;

			{
				CString strDebug;
				strDebug.Format(_T("Step to (%u) with Kanshi(%s), RngStatus(%u)"), m_nStep, (LPCTSTR)str, m_eRngStatus);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStepProc54"), _T("debug"), strDebug, nLogEx::debug);
			}
			break;
		}

		// �X�e�[�^�X�؂�ւ�莞
		str = GetKanshiString(m_nStep);
		// �]���^�̏ꍇ
		if (m_nGeneMode == eRngGeneration_Past)
		{
			{
				CString strDebug;
				strDebug.Format(_T("Step to (%u) with Kanshi(%s), Jyuraigata, wait Code(%s)"), m_nStep, (LPCTSTR)str, (LPCTSTR)mRngStep[m_nStation][m_nTransMode][m_nStep].value1);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStepProc54"), _T("debug"), strDebug, nLogEx::debug);
			}
			if (str.Compare(m_strLastRngToneStatus) != 0)
			{
				m_strLastRngToneStatus = str;
				{
					CString strDebug;
					strDebug.Format(_T("Step to (%u) with Kanshi(%s), Jyuraigata change string, wait Code(%s)"), m_nStep, (LPCTSTR)str, (LPCTSTR)mRngStep[m_nStation][m_nTransMode][m_nStep].value1);
					CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStepProc54"), _T("debug"), strDebug, nLogEx::debug);
				}
				if (str.Compare(mRngStep[m_nStation][m_nTransMode][m_nStep].value1) == 0)
				{
					{
						CString strDebug;
						strDebug.Format(_T("Step to (%u) with Kanshi(%s), Jyuraigata detect Code(%s)"), m_nStep, (LPCTSTR)str, (LPCTSTR)mRngStep[m_nStation][m_nTransMode][m_nStep].value1);
						CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStepProc54"), _T("debug"), strDebug, nLogEx::debug);
					}
					// �����W�g�[�����o�M���̌��o
					DetectRngTone();
				}
			}
		}
		// �Đ��^�̏ꍇ
		else
		{
			{
				CString strDebug;
				strDebug.Format(_T("Step to (%u) with Kanshi(%s), Saiseigata, wait Code(%s)"), m_nStep, (LPCTSTR)str, (LPCTSTR)mRngStep[m_nStation][m_nTransMode][m_nStep].value1);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStepProc54"), _T("debug"), strDebug, nLogEx::debug);
			}
			if (str.Compare(mRngStep[m_nStation][m_nTransMode][m_nStep].value2) == 0)
			{
				{
					CString strDebug;
					strDebug.Format(_T("Step to (%u) with Kanshi(%s), Saiseigata, detect Code(%s)"), m_nStep, (LPCTSTR)str, (LPCTSTR)mRngStep[m_nStation][m_nTransMode][m_nStep].value1);
					CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStepProc54"), _T("debug"), strDebug, nLogEx::debug);
				}
				// �����W�g�[�����o�M���̌��o
				DetectRngTone();
			}
		}

		// �����W�g�[���ړ�
		MoveRngTone();
		break;

	case eRngStep_6:
		// �����W�g�[���ړ�
		MoveRngTone();
		CGraphMutex::Lock(eRngProp);
		// �����W�g�[�������ׂċA���Ă���܂ő҂�
		if (m_queRngSnd.empty() && m_queRngRcv.empty())
		{
			m_nStep = eRngStep_1;

			{
				CString strDebug;
				strDebug.Format(_T("Step to (%u) with RngSnd and RngRcv completed"), m_nStep);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStepProc54"), _T("debug"), strDebug, nLogEx::debug);
			}
		}
		CGraphMutex::Unlock(eRngProp);
		break;

	default:
		break;
	}

	if (m_nStep == nOldStep)
	{
		// �X�e�b�v���ς�Ȃ�
		return FALSE;
	}

	// �X�e�b�v���ς����ꍇ
	return TRUE;
}

/*============================================================================*/
/*! CRangePropagation

-# �����W�g�[���ړ�

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CRangePropagation::MoveRngTone()
{
	CGraphMutex::Lock(eRngProp);

	// �����W�g�[���ړ��v�Z
	deque<CRangePropagation::structPropData>::iterator itr;
	time_t tMove;

//#if 0
//	// ���������Ǝ��Ԏ擾
//	double d2WanRng = Get2WayRng();
//	if (d2WanRng >= 0.0)
//	{
//		m_2WayRange = d2WanRng;
//		m_rtt = m_2WayRange / V_C;
//	}
//	else
//	{
//		return;
//	}
//#endif

	{
		CString strDebug;
		strDebug.Format(_T("m_queRngSnd size(%I64u), m_queRngRcv size(%I64u)"), m_queRngSnd.size(), m_queRngRcv.size());
		CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("MoveRngTone"), _T("debug"), strDebug, nLogEx::debug);
	}

	// �A���e�i->�q���Ԃ̉��ʒu�X�V
	for (itr = m_queRngSnd.begin(); itr != m_queRngSnd.end(); itr++)
	{
		// �����W�g�[�����ړ���������
		tMove = m_dataTime.GetTime() - (*itr).tStart;
		// �o�߂����ʒu�v�Z
		(*itr).pos = (((double)tMove) * V_C);

		{
			CString strDebug;
			strDebug.Format(_T("m_queRngSnd[%u], tMove(%I64d), RTT(%f), pos(%f)"), (*itr).count, tMove, (*itr).rtt, (*itr).pos);
			CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("MoveRngTone"), _T("debug"), strDebug, nLogEx::debug);
		}
	}

	for (itr = m_queRngSnd.begin(); itr != m_queRngSnd.end(); itr++)
	{
		// �q���ɓ��B�����ꍇ
		if ((*itr).pos >= (*itr).d2WayRange / 2)
		{
			{
				CString strDebug;
				strDebug.Format(_T("Arrive Satellite. m_queRngSnd[%u], tStart(%I64d), offset(%f), pos(%f)"), (*itr).count, (*itr).tStart, (*itr).offset, (*itr).pos);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("MoveRngTone"), _T("debug"), strDebug, nLogEx::debug);
			}
			// �A���e�i->�q���ԃL���[����폜���A�A���e�i<-�q���L���[�ɒǉ�
			(*itr).tStart = m_dataTime.GetTime();
			(*itr).offset = (*itr).pos - (*itr).d2WayRange / 2;
			(*itr).pos = (*itr).d2WayRange / 2 - (*itr).offset;
			{
				CString strDebug;
				strDebug.Format(_T("After Arrive Satellite. m_queRngSnd[%u], tStart(%I64d), offset(%f), pos(%f)"), (*itr).count, (*itr).tStart, (*itr).offset, (*itr).pos);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("MoveRngTone"), _T("debug"), strDebug, nLogEx::debug);
			}

			m_queRngRcv.push_back((*itr));
			m_queRngSnd.erase(itr);
//			break;
			if (m_queRngSnd.empty())
				break;
			else
				itr = m_queRngSnd.begin();
		}
	}

	// �A���e�i<-�q���Ԃ̉��ʒu�X�V
	for (itr = m_queRngRcv.begin(); itr != m_queRngRcv.end(); itr++)
	{
		// �����W�g�[�����ړ���������
		tMove = m_dataTime.GetTime() - (*itr).tStart;
		// �o�߂����ʒu�v�Z
		(*itr).pos = (*itr).d2WayRange / 2 - (((double)tMove) * V_C) - (*itr).offset;
	}
	for (itr = m_queRngRcv.begin(); itr != m_queRngRcv.end(); itr++)
	{
		// �A���e�i�ɓ��B�����ꍇ
		if ((*itr).pos <= 0.0)
		{
			// �A���e�i<-�q���ԃL���[����폜
			m_queRngRcv.erase(itr);
			// �L���[����ɂȂ����ꍇ
			if (m_queRngRcv.empty() && m_queRngSnd.empty())
			{
#ifdef _DEBUG_RNGPROPDUMMY
				if (m_nStation == eStation_Usuda642)
				{
					mRngStepDummy[mRngTransModeDummySelect][1].status = _T("STOP(TX&RX)");
					mRngStepDummy[mRngTransModeDummySelect][3].status = _T("STOP(TX&RX)");
				}
				else
				{
					mRngStepDummy[mRngTransModeDummySelect][1].status = _T("STOP");
					mRngStepDummy[mRngTransModeDummySelect][3].status = _T("STOP");
				}
#endif
				// �������Ȃ�
			}
//			break;
			if (m_queRngRcv.empty())
				break;
			else
				itr = m_queRngRcv.begin();
		}
	}

	CGraphMutex::Unlock(eRngProp);
}


/*============================================================================*/
/*! CRangePropagation

-# �Ď��f�[�^��double�Ŏ擾

@param	�Ȃ�
@retval	�Ď��f�[�^

*/
/*============================================================================*/
double CRangePropagation::GetKanshiDouble()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

	CString monitorString;
	double ret;

	monitorString = mRngStep[m_nStation][m_nTransMode][m_nStep].nameK;

	if (monitorString.IsEmpty())
	{
		return 0.0;
	}
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStation, monitorString);
#ifdef _DEBUG_RNGPROPDUMMY
	obs_adr = new CEngValCnv::obsname_t;
	for (int i = 0; i < eRngStep_Num; i++)
	{
		if (monitorString.Compare(mRngStepDummy[mRngTransModeDummySelect][i].nameK) == 0)
		{
			char szStatus[256];
			WideCharToMultiByte(CP_ACP, 0, mRngStepDummy[mRngTransModeDummySelect][i].status, -1, szStatus, sizeof(szStatus), NULL, NULL);
			sprintf_s(obs_adr->sz_sts_name, 64, ("%s"), szStatus);
			obs_adr->l_data = (long)mRngStepDummy[mRngTransModeDummySelect][i].val;
			obs_adr->d_data = mRngStepDummy[mRngTransModeDummySelect][i].val;
			obs_adr->l_inflg = INFLG_ON;
			break;
		}
	}
#endif
	if (obs_adr == NULL)
	{
		return 0.0;
	}
	if (obs_adr->l_inflg != INFLG_ON)
	{
#ifdef _DEBUG_RNGPROPDUMMY
		delete obs_adr;
#endif
		return 0.0;
	}

	ret = obs_adr->d_data;
#ifdef _DEBUG_RNGPROPDUMMY
	delete obs_adr;
#endif

	return ret;
}


/*============================================================================*/
/*! CRangePropagation

-# �Ď��f�[�^��CString�Ŏ擾

@param	step�F�X�e�b�v��
@retval	�Ď��f�[�^

*/
/*============================================================================*/
CString CRangePropagation::GetKanshiString(int step /*= -1*/)
{
	UINT nStep;
	if (step < 0)
	{
		nStep = m_nStep;
	}
	else
	{
		nStep = (UINT)step;
	}

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

	CString monitorString;
	CString str;

	monitorString = mRngStep[m_nStation][m_nTransMode][nStep].nameK;

	if (monitorString.IsEmpty())
	{
		return str;
	}
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStation, monitorString);
#ifdef _DEBUG_RNGPROPDUMMY
	obs_adr = new CEngValCnv::obsname_t;
	for (int i = 0; i < eRngStep_Num; i++)
	{
		if (monitorString.Compare(mRngStepDummy[mRngTransModeDummySelect][i].nameK) == 0)
		{
			char szStatus[256];
			WideCharToMultiByte(CP_ACP, 0, mRngStepDummy[mRngTransModeDummySelect][i].status, -1, szStatus, sizeof(szStatus), NULL, NULL);
			sprintf_s(obs_adr->sz_sts_name, 64, ("%s"), szStatus);
			obs_adr->l_data = (long)mRngStepDummy[mRngTransModeDummySelect][i].val;
			obs_adr->d_data = mRngStepDummy[mRngTransModeDummySelect][i].val;
			obs_adr->l_inflg = INFLG_ON;
			break;
		}
	}
#endif
	if (obs_adr == NULL)
	{
		return str;
	}
	if (obs_adr->l_inflg != INFLG_ON)
	{
#ifdef _DEBUG_RNGPROPDUMMY
		delete obs_adr;
#endif
		return str;
	}

	str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));

#ifdef _DEBUG_RNGPROPDUMMY
	delete obs_adr;
#endif
	return str;
}


/*============================================================================*/
/*! CRangePropagation

-# �����W�g�[�����m

@param
@retval

*/
/*============================================================================*/
void CRangePropagation::DetectRngTone()
{
#ifdef _DEBUG_RNGPROPDUMMY
	m_nRngNum = 20;
#else

	{
		CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("DetectRngTone"), _T("debug"), _T("called"), nLogEx::debug);
	}

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStation, _T("V-EQU.RNG_NUM"));
	if (obs_adr == NULL)
	{
		return;
	}
	if (obs_adr->l_inflg != INFLG_ON)
	{
		return;
	}
	m_nRngNum = (UINT)obs_adr->d_data;
#endif

	// ���������Ǝ��Ԏ擾
	double d2WanRng = Get2WayRng();
	if (d2WanRng >= 0.0)
	{
		m_2WayRange = d2WanRng;
		m_rtt = m_2WayRange / V_C;
		{
			CString strDebug;
			strDebug.Format(_T("m_nRngNum(%u), d2WanRng(%f), set values m_2WayRange(%f), m_rtt(%f)"), m_nRngNum, d2WanRng, m_2WayRange, m_rtt);
			CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("DetectRngTone"), _T("debug"), strDebug, nLogEx::debug);
		}
	}
	else
	{
		{
			CString strDebug;
			strDebug.Format(_T("m_nRngNum(%u), d2WanRng(%f), not set values m_2WayRange(%f), m_rtt(%f)"), m_nRngNum, d2WanRng, m_2WayRange, m_rtt);
			CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("DetectRngTone"), _T("debug"), strDebug, nLogEx::debug);
		}
		return;
	}

	{
		CString strDebug;
		strDebug.Format(_T("compare m_nSndCount(%u) and m_nRngNum(%u)"), m_nSndCount, m_nRngNum);
		CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("DetectRngTone"), _T("debug"), strDebug, nLogEx::debug);
	}

	CGraphMutex::Lock(eRngProp);

	// �]���^�ŃJ�E���g������M�͈͓����A�Đ��^�̏ꍇ
	if ((m_nGeneMode == eRngGeneration_Past && m_nSndCount < m_nRngNum) || (m_nGeneMode == eRngGeneration_Regene))
	{
		// �J�E���g�l����
		if (m_queRngAll.size() >= m_nMaxRngNum)
		{
			// �S���X�g���ő�l���z����ꍇ�A���X�g�擪�̃J�E���g�l�����̃J�E���g�l�ɂ��āA���X�g�擪���폜
			m_nSndCount = m_queRngAll.front().count;
			m_queRngAll.pop_front();
//			TRACE("###### DetectRngTone: pop_front: m_queRngAll size:%04d, m_nSndCount:%04u\n", (int)m_queRngAll.size(), m_nSndCount);

			if (m_queRngRcv.empty() == false)
			{
				// ���H���X�g�Ƀf�[�^������Ε��H�擪�폜
				m_queRngRcv.pop_front();
			}
			else
			{
				// ���H���X�g�Ƀf�[�^���Ȃ���Ή��H�擪�폜
				m_queRngSnd.pop_front();
			}
		}
		else
		{
			// �J�E���g�l���܂��͈͓��̏ꍇ
			m_nSndCount++;
//			TRACE("###### DetectRngTone: normal count up: m_queRngAll size:%04d, m_nSndCount:%04u\n", (int)m_queRngAll.size(), m_nSndCount);
		}

		structPropData stPropData;
		stPropData.count = m_nSndCount;
		// �J�n����
		stPropData.tStart = m_dataTime.GetTime();
		// �I�t�Z�b�g�ʒu
		stPropData.offset = 0;
		// �ʒu
		stPropData.pos = 0;
		// 2WayRng
		stPropData.d2WayRange = m_2WayRange;
		// RTT
		stPropData.rtt = m_rtt;
		m_queRngSnd.push_back(stPropData);
		m_queRngAll.push_back(stPropData);
	}

	CGraphMutex::Unlock(eRngProp);

	{
		CString strDebug;
		strDebug.Format(_T("m_queRngSnd size(%I64u), m_queRngAll size(%I64u)"), m_queRngSnd.size(), m_queRngAll.size());
		CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("DetectRngTone"), _T("debug"), strDebug, nLogEx::debug);
	}
}


/*============================================================================*/
/*! CRangePropagation

-# �q������̉��������擾

@param	�Ȃ�
@retval �q������̉�������

*/
/*============================================================================*/
double CRangePropagation::Get2WayRng()
{
#ifdef _DEBUG_RNGPROPDUMMY
	return 450670.881 * 2 * 1000.0;
#else
	return theApp.GetSatelliteData().GetTrendData().Get2WayRng(m_dataTime.GetTime()) * 1000.0;
#endif
}


/*============================================================================*/
/*! CRangePropagation

-# �����W�g�[��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CRangePropagation::InitRngTone()
{
	m_nSndCount = 0;
	m_queRngSnd.clear();
	m_queRngRcv.clear();
	m_queRngAll.clear();
}


/* ------------------------------------------------------------------------------------ */
/* �T�}���v���p�e�B
/* ------------------------------------------------------------------------------------ */
CSummaryProperties::CSummaryProperties()
{
}

CSummaryProperties::CSummaryProperties(UINT nViewID, UINT nStationID, UINT nParentID, CString strWndName)
	: m_nViewID(nViewID)
	, m_nStationID(nStationID)
	, m_nParentID(nParentID)
	, m_strWndName(strWndName)
{
}

CSummaryProperties::~CSummaryProperties()
{
}

/*============================================================================*/
/*! �T�}���v���p�e�B

-# �O���t�r���[�����X�g�̎擾

@param	listViewName�F�r���[�����X�g
@retval	TRUE:�擾���� / FALSE:�擾���s

*/
/*============================================================================*/
BOOL CSummaryProperties::GetViewNameList(vector<CString>& listViewName)
{
	CString str;

	if (str.LoadString(IDS_TREND_RNGGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	if (str.LoadString(IDS_TREND_RRGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	if (m_nStationID != eStation_Uchinoura20)
	{
		if (str.LoadString(IDS_TREND_XSPGRAPH) == FALSE)
			return FALSE;
		listViewName.push_back(str);
	}

	if (m_nStationID == eStation_Usuda642)
	{
		if (str.LoadString(IDS_TREND_KASPGRAPH) == FALSE)
			return FALSE;
		listViewName.push_back(str);
	}

	if (m_nStationID != eStation_Usuda642)
	{
		if (str.LoadString(IDS_TREND_SAGCGRAPH) == FALSE)
			return FALSE;
		listViewName.push_back(str);
	}

	if (str.LoadString(IDS_TREND_XAGCGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	if (m_nStationID == eStation_Usuda642)
	{
		if (str.LoadString(IDS_TREND_KAAGCGRAPH) == FALSE)
			return FALSE;
		listViewName.push_back(str);
	}

	return TRUE;
}


/* ------------------------------------------------------------------------------------ */
/* RF�g���b�v�p��`�t�@�C��
/* ------------------------------------------------------------------------------------ */
CRFTripAngleFile::CRFTripAngleFile()
{
}

CRFTripAngleFile::~CRFTripAngleFile()
{
	// ���X�g�A�}�b�v�N���A
	m_listPowerInfo.clear();
	m_mapRftripGene.clear();
	m_mapRftripMod.clear();
}


//#if 0
///*============================================================================*/
///*! CRFTripAngleFile
//
//-# RF�g���b�v�p��`�t�@�C���p�X���擾����
//
//@param	strPwr�F���M�d�͒l������
//@param	nStation�F�ǔԍ�
//@retval	RF�g���b�v�p��`�t�@�C���p�X
//
//*/
///*============================================================================*/
//CString CRFTripAngleFile::GetRfTripFilePath(CString strPwr, UINT nStation)
//{
//	UINT nRfTrip_File = eFileType_RFTrip_25w_File;
//
//	// ���M�d�͎擾
//	if (strPwr.Compare(PWR_25W) == 0)
//	{
//		nRfTrip_File = eFileType_RFTrip_25w_File;
//	}
//	else if (strPwr.Compare(PWR_100W) == 0)
//	{
//		nRfTrip_File = eFileType_RFTrip_100w_File;
//	}
//	else if (strPwr.Compare(PWR_200W) == 0)
//	{
//		nRfTrip_File = eFileType_RFTrip_200w_File;
//	}
//	else if (strPwr.Compare(PWR_1KW) == 0)
//	{
//		nRfTrip_File = eFileType_RFTrip_1kw_File;
//	}
//	else if (strPwr.Compare(PWR_2KW) == 0)
//	{
//		nRfTrip_File = eFileType_RFTrip_2kw_File;
//	}
//	else if (strPwr.Compare(PWR_20KW) == 0)
//	{
//		nRfTrip_File = eFileType_RFTrip_20kw_File;
//	}
//
//	return theApp.GetShareFilePath(nRfTrip_File, nStation);
//}
//
//
///*============================================================================*/
///*! CRFTripAngleFile
//
//-# RF�g���b�v�p��`�t�@�C���p�X���擾����
//
//@param	nStation�F�ǔԍ�
//@retval	RF�g���b�v�p��`�t�@�C���p�X
//
//*/
///*============================================================================*/
//CString CRFTripAngleFile::GetRfTripFilePath(UINT nStation)
//{
//	// ���M�d�͂���RF�g���b�v�t�@�C�����擾
//	//	int nRfPower = 0;
//	CString strRfPwrSig;
//	CString strHpa;
//	CString strPwr;
//
//	// �Ǖʂ�RF�g���b�v�t�@�C���I��
//	switch (nStation)
//	{
//	case eStation_Usuda64:
//	case eStation_Usuda642:
//		// HPA�𒲂ׂ�
//
//		if (strHpa.Compare(HPA1_RES))
//		{
//			// HPA-1�̏ꍇ
//			strRfPwrSig = XTX_HPA_RF_PWR_SIG;
//		}
//		else if (strHpa.Compare(HPA2_RES))
//		{
//			// HPA-2�̏ꍇ
//			strRfPwrSig = XTX_HPA2_RF_PWR_SIG;
//		}
//
//		break;
//
//	case eStation_Uchinoura34:
//		// S�т̏ꍇ
//		{
//			strRfPwrSig = STX_RF_POWER_SEL;
//		}
//		// X�т̏ꍇ
//		{
//			strRfPwrSig = XTX_HPA_RF_PWR_CONT;
//		}
//
//		break;
//
//	case eStation_Uchinoura20:
//		strRfPwrSig = STX_RF_POWER_SEL;
//		break;
//
//	default:
//		break;
//	}
//
//
//	// ���M�d�͎擾
//
//
//	// ���M�d�͂���t�@�C���p�X�̎擾
//	return GetRfTripFilePath(strPwr, nStation);
//}
//#endif

/*============================================================================*/
/*! CRFTripAngleFile

-# �J�����g��RF�g���b�v�t�@�C����ǂݍ���

@param	�Ȃ�
@retval	TRUE�F�ǂݍ��ݐ��� / FALSE�F�ǂݍ��ݎ��s

*/
/*============================================================================*/
BOOL CRFTripAngleFile::ReadFile()
{
#ifndef _ENGVALCNV_TREND
	CString strSatellite = _T("HAYABUSA2");
#else
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

	// �J�����g�̉q�������Ď�����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStation, _T(EISEI_NAME));
	if (obs_adr == NULL)
	{
		return FALSE;
	}
	if (obs_adr->l_inflg != INFLG_ON)
	{
		return FALSE;
	}

	CString strSatellite;
	strSatellite.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
	strSatellite.Trim(_T(" "));
	if (strSatellite.IsEmpty())
	{
		return FALSE;
	}
#endif

	// �q���������ɃZ�b�g����Ă��āA���q�����������ꍇ�͉������Ȃ�
	if (m_strSatellite.IsEmpty() == FALSE && m_strSatellite.CompareNoCase(strSatellite) == 0)
	{
		return FALSE;
	}

	// �q�������Z�b�g
	m_strSatellite = strSatellite;

	return ReadFile(strSatellite);
}

/*============================================================================*/
/*! CRFTripAngleFile

-# �w��q������RF�g���b�v�t�@�C����ǂݍ���

@param	strSatellite�F�q����
@retval	TRUE�F�ǂݍ��ݐ��� / FALSE�F�ǂݍ��ݎ��s

*/
/*===========================================================================*/
BOOL CRFTripAngleFile::ReadFile(CString& strSatellite)
{
	// ���X�g�A�}�b�v�N���A
	m_listPowerInfo.clear();
	m_mapRftripGene.clear();
	m_mapRftripMod.clear();

	// �q��������RF�g���b�v�p��`�t�@�C���p�X���쐬
	CString strRfTripFolder = theApp.GetShareFilePath(eFileType_RFTrip_Folder, m_nStation);
	CString strRFTripFileName;
	strRFTripFileName.Format(RFTRIP_FILENAME, (LPCTSTR)(strSatellite.MakeLower()));
	CString strRFTripFilePath;
	strRFTripFilePath.Format(_T("%s%s"), (LPCTSTR)strRfTripFolder, (LPCTSTR)strRFTripFileName);

	TCHAR buf[MAX_PATH];

	// INFO�Z�N�V����
	// ���M�d�͎擾�p�Ď������擾
	if (GetPrivateProfileString(RFTRIP_SECTION_INFO, RFTRIP_KEY_KANSHI_POWER, _T(""), buf, MAX_PATH, strRFTripFilePath) == 0)
		return FALSE;
	(void)CStringOprTool::TrimComment(m_strKanshiPower, _T(';'), CString(buf));
	// �ϒ��`���擾�p�Ď������擾
	if (GetPrivateProfileString(RFTRIP_SECTION_INFO, RFTRIP_KEY_KANSHI_MOD, _T(""), buf, MAX_PATH, strRFTripFilePath) == 0)
		return FALSE;
	(void)CStringOprTool::TrimComment(m_strKanshiMod, _T(';'), CString(buf));
	// ���M�d�̓X�e�[�^�X���擾
	GetPrivateProfileString(RFTRIP_SECTION_INFO, RFTRIP_KEY_MOD_SECTION_DEF, _T(""), buf, MAX_PATH, strRFTripFilePath);
	(void)CStringOprTool::TrimComment(m_strModSectionDef, _T(';'), CString(buf));
	// ���M�d��(STSNAME_POWER)�Ŏ擾�����X�e�[�^�X���擾
	if (GetPrivateProfileString(RFTRIP_SECTION_INFO, RFTRIP_KEY_POWERINFO, _T(""), buf, MAX_PATH, strRFTripFilePath) == 0)
		return FALSE;
	else
	{
		CString strPowerStatusAll;
		CString strPowerStatus;
		(void)CStringOprTool::TrimComment(strPowerStatusAll, _T(';'), CString(buf));
		(void)CStringOprTool::ReplaceMultipleSpace(strPowerStatusAll);

		for (int i = 0;; i++)
		{
			AfxExtractSubString(strPowerStatus, strPowerStatusAll, i, ' ');
			if (strPowerStatus.IsEmpty())
			{
				break;
			}
			// ���M�d�̓��X�g�ɃX�e�[�^�X�ǉ�
			m_listPowerInfo.push_back(strPowerStatus);
			vector<double> listRfTripGene(360);
			vector<double> listRfTripMod(360);
			m_mapRftripGene[strPowerStatus] = listRfTripGene;
			m_mapRftripMod[strPowerStatus] = listRfTripMod;
		}
	}

	CString strKey;

	// �ϒ��`�� �u�ʏ�v�Z�N�V����
	for (int az = 0; az < 360; az++)
	{
		strKey.Format(_T("%d"), az);
		GetPrivateProfileString(RFTRIP_SECTION_RFTRIP_GENE, strKey, _T(""), buf, MAX_PATH, strRFTripFilePath);
		CString strTripAll;
		CString strTrip;
		(void)CStringOprTool::TrimComment(strTripAll, _T(';'), CString(buf));
		(void)CStringOprTool::ReplaceMultipleSpace(strTripAll);

		if (strTripAll.IsEmpty())
		{
			continue;
		}

		for (int i = 0;; i++)
		{
			AfxExtractSubString(strTrip, strTripAll, i, ' ');
			if (strTrip.IsEmpty())
			{
				break;
			}

			// ���M�d�̓X�e�[�^�X�ʂ�RF�g���b�v�ėp�}�b�v�Ƀ��X�g�ǉ�
			CString& strPowerInfo = m_listPowerInfo[i];
			vector<double>& list = m_mapRftripGene[strPowerInfo];
			list[az] = _ttof(strTrip);
		}
	}

	// �ϒ��`�� �u�w��v�Z�N�V����
	CString strSectionMod;
	strSectionMod.Format(_T("%s%s"), RFTRIP_SECTION_RFTRIP_MOD, (LPCTSTR)m_strModSectionDef);
	for (int az = 0; az < 360; az++)
	{
		strKey.Format(_T("%d"), az);
		GetPrivateProfileString(strSectionMod, strKey, _T(""), buf, MAX_PATH, strRFTripFilePath);

		CString strTripAll;
		CString strTrip;
		(void)CStringOprTool::TrimComment(strTripAll, _T(';'), CString(buf));
		(void)CStringOprTool::ReplaceMultipleSpace(strTripAll);

		if (strTripAll.IsEmpty())
		{
			continue;
		}

		for (int i = 0;; i++)
		{
			AfxExtractSubString(strTrip, strTripAll, i, ' ');
			if (strTrip.IsEmpty())
			{
				break;
			}

			// ���M�d�̓X�e�[�^�X�ʂ�RF�g���b�v�w��}�b�v�Ƀ��X�g�ǉ�
			CString& strPowerInfo = m_listPowerInfo[i];
			vector<double>& list = m_mapRftripMod[strPowerInfo];
			list[az] = _ttof(strTrip);
		}
	}

	return TRUE;
}


/*============================================================================*/
/*! CRFTripAngleFile

-# �w��̋ǁA�w��q������RF�g���b�v�t�@�C���ǂݍ���ŁA���͂̑��M�d�͒l�A�ϒ��`���l�ɉ�����AZ, EL���X�g��Ԃ�

@param	nStation�F�ǔԍ�[����]
@param	strSatellite�F�q����[����]
@param	strPower�F���M�d�͎擾�p�Ď���[����]
@param	strMod�F�ϒ��`���擾�p�Ď���[����]
@param	listAzEl�FAZ, EL�̃��X�g[�o��]
@retval	TRUE�F�ǂݍ��ݐ��� / FALSE�F�ǂݍ��ݎ��s

*/
/*===========================================================================*/
BOOL CRFTripAngleFile::ReadFile(UINT nStation, CString& strSatellite, CString& strPowerValue, CString& strModValue, vector<double>& listAzEl)
{
	m_nStation = nStation;
	if (ReadFile(strSatellite))
	{
		return GetAzEl(strPowerValue, strModValue, listAzEl);
	}

	return FALSE;
}


/*============================================================================*/
/*! CRFTripAngleFile

-# �Ď��̑��M�d�́A�ϒ��`���ɉ�����AZ, EL���X�g��Ԃ�

@param	listAzEl�FAZ, EL�̃��X�g�o��
@retval	TRUE�F���X�g���� / FALSE�F���X�g�Ȃ�

*/
/*============================================================================*/
BOOL CRFTripAngleFile::GetAzEl(vector<double>& listAzEl)
{
	CString strPowerValue = _T("");
	CString strModValue = _T("");
#ifndef _ENGVALCNV_TREND
	strPowerValue = _T("HPA1-20kw");
	if (m_strKanshiMod.Compare(_T("*")) != 0)
	{
		strModValue = _T("ABCD_BPSK_123");
	}

#else
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

	// ���M�d�͎擾
	if (m_strKanshiPower.IsEmpty())
	{
		return FALSE;
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStation, m_strKanshiPower);
	if (obs_adr == NULL)
	{
		return FALSE;
	}
	if (obs_adr->l_inflg != INFLG_ON)
	{
		return FALSE;
	}

	strPowerValue.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
	strPowerValue.Trim(_T(" "));
	if (strPowerValue.IsEmpty())
	{
		return FALSE;
	}

	// �ϒ��`�����肪�K�v�ȏꍇ
	if (m_strKanshiMod.Compare(_T("*")) != 0)
	{
		// �ϒ��`���擾
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStation, m_strKanshiMod);
		if (obs_adr == NULL)
		{
			return FALSE;
		}
		if (obs_adr->l_inflg != INFLG_ON)
		{
			return FALSE;
		}

		strModValue.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
		strModValue.Trim(_T(" "));
	}

#endif

	return GetAzEl(strPowerValue, strModValue, listAzEl);
}


/*============================================================================*/
/*! CRFTripAngleFile

-# �Ď��̑��M�d�́A�ϒ��`���ɉ�����AZ, EL���X�g��Ԃ�

@param	strPower�F���M�d�͎擾�p�Ď���[����]
@param	strMod�F�ϒ��`���擾�p�Ď���[����]
@param	listAzEl�FAZ, EL�̃��X�g[�o��]
@retval	TRUE�F���X�g���� / FALSE�F���X�g�Ȃ�

*/
/*============================================================================*/
BOOL CRFTripAngleFile::GetAzEl(CString& strPowerValue, CString& strModValue, vector<double>& listAzEl)
{
	BOOL bRet = FALSE;
	if (strPowerValue.IsEmpty())
	{
		return bRet;
	}

//	if (m_strKanshiMod.Compare(_T("*")) == 0 || strModValue.IsEmpty() || strModValue.Find(m_strModSectionDef) < 0)
	if (strModValue.IsEmpty() || strModValue.Find(m_strModSectionDef) < 0)
	{
		// �ėp�̏ꍇ
		if (m_mapRftripGene.count(strPowerValue) > 0)
		{
			listAzEl = m_mapRftripGene[strPowerValue];
			bRet = TRUE;
		}

	}
	else
	{
		// �ϒ��`���̏ꍇ
		if (m_mapRftripMod.count(strPowerValue) > 0)
		{
			listAzEl = m_mapRftripMod[strPowerValue];
			bRet = TRUE;
		}
	}

	return bRet;
}


/* ------------------------------------------------------------------------------------ */
/* ������[�e�B���e�B
/* ------------------------------------------------------------------------------------ */
CPrintUtility::CPrintUtility()
{
}

CPrintUtility::~CPrintUtility()
{
}

/*============================================================================*/
/*! CPrintUtility

-# SJIS�̃t�@�C�����������

@param	filepath�F�t�@�C���p�X
@retval	�Ȃ�

*/
/*============================================================================*/
BOOL CPrintUtility::PrintSjisFile(CString& filepath, CString title)
{
	BOOL ret = FALSE;
#ifdef _NOTEPAD
	CString strCmd;
	//	strCmd.Format(_T("notepad %s"), filepath);
	strCmd.Format(_T("notepad /p %s"), filepath);

	// �V�����v���Z�X�p�̍\���̂��N������
	STARTUPINFO    stStartUpInfo;
	// �����o�[�̑����͗\�񂳂�Ă��邩���������̂ŁA���ׂẴ����o�[��
	// 0 �ɐݒ肵�Ă���A�K�v�ȃ����o�[���������Z�b�g����
	memset(&stStartUpInfo, 0, sizeof(STARTUPINFO));
	stStartUpInfo.cb = sizeof(STARTUPINFO);			// �\���̂̃T�C�Y
	stStartUpInfo.dwFlags = STARTF_USESHOWWINDOW;	// wShowWindow �����o�[���g��
	//	stStartUpInfo.wShowWindow = SW_SHOWDEFAULT;		// �f�t�H���g�̒l
	stStartUpInfo.wShowWindow = SW_HIDE;			// ��ʂ͔�\��

	// �V�����v���Z�X��ŏ����󂯎��v���Z�X�\����
	PROCESS_INFORMATION stProcessInfo;

	// �V�����v���Z�X�����ƁA�v���Z�X�̓v���C�}���X���b�h�����A
	// �A�v���P�[�V�������N������
	if (CreateProcess(NULL, (LPTSTR)strCmd.GetBuffer(), NULL, NULL, FALSE,
		/*CREATE_SUSPENDED*/NORMAL_PRIORITY_CLASS, NULL,
		NULL, &stStartUpInfo, &stProcessInfo) == 0)
	{
		strCmd.ReleaseBuffer();
		CloseHandle(stProcessInfo.hThread);
		CloseHandle(stProcessInfo.hProcess);
		return FALSE;
	}
	strCmd.ReleaseBuffer();

	// ����I���܂őҋ@
	if (WaitForSingleObject(stProcessInfo.hProcess, INFINITE) != WAIT_FAILED)
	{
		ret = TRUE;
	}

	CloseHandle(stProcessInfo.hThread);
	CloseHandle(stProcessInfo.hProcess);
#else
	// ���O���
	CPrintDialog dlg(FALSE, PD_ALLPAGES | PD_HIDEPRINTTOFILE | PD_NOPAGENUMS | PD_RETURNDC | PD_USEDEVMODECOPIES);
	//dlg.m_pd.Flags |= PD_SELECTION;
	dlg.m_pd.Flags |= PD_NOSELECTION;

	switch (dlg.DoModal())
	{
	case 0:
	case IDCANCEL:
		return ret;
	case IDOK:
		break;
	default:
		ASSERT(FALSE);
		return ret;
	}

	CDC dc;
	dc.Attach(dlg.m_pd.hDC);
	CDetailPrint *printer;
	printer = new CDetailPrint(&dc);

#if 1
	if (title == _T(""))
	{
		title = _T("�ڍו\��");
	}
#else
	CString title = _T("�ڍו\��");
#endif
	printer->SetTitle(title);
	if (printer->StartPrinting())
	{
		FILE *fp = NULL;
		TRY
		{
			if (_wfopen_s(&fp, filepath, _T("r")) != 0 || fp == NULL){
				delete printer;
				return ret;
			}
			char szline[2048];
			CString str, cbuf;
			while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL){
				//cbuf = CServerFile::ConvertUnicode(szline);
				cbuf = CString(szline);
				printer->PrintLine(cbuf);
			}
			printer->EndPrinting();
			ret = TRUE;
		} CATCH_ALL(e){
		}
		END_CATCH_ALL

			if (fp != NULL)
				fclose(fp);
	}
	delete printer;
	::DeleteDC(dc.Detach());

#endif

	return ret;
}

BOOL CPrintUtility::PrintSjisFileList(vector<CString>& fileList, CString title)
{
	BOOL ret = FALSE;
	// ���O���
	CPrintDialog dlg(FALSE, PD_ALLPAGES | PD_HIDEPRINTTOFILE | PD_NOPAGENUMS | PD_RETURNDC | PD_USEDEVMODECOPIES);
	//dlg.m_pd.Flags |= PD_SELECTION;
	dlg.m_pd.Flags |= PD_NOSELECTION;

	switch (dlg.DoModal())
	{
	case 0:
	case IDCANCEL:
		return ret;
	case IDOK:
		break;
	default:
		ASSERT(FALSE);
		return ret;
	}

#if 1
	if (title == _T("OFF"))
	{
		title = _T("");
	}
	else if (title == _T(""))
	{
		title = _T("�ڍו\��");
	}
#else
	CString title = _T("�ڍו\��");
#endif

	CDC dc;
	CDetailPrint *printer;

	int fcnt = (int)fileList.size();
	dc.Attach(dlg.m_pd.hDC);

	for (int i = 0; i < fcnt; ++i)
	{
		printer = new CDetailPrint(&dc);

		CString filepath = fileList[i];
		printer->SetTitle(title);
		if (printer->StartPrinting(true))
		{
			FILE *fp = NULL;
			TRY
			{
				if (_wfopen_s(&fp, filepath, _T("r")) != 0){
					delete printer;
					return ret;
				}
				char szline[2048];
				CString str, cbuf;
				while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL){
					//cbuf = CServerFile::ConvertUnicode(szline);
					cbuf = CString(szline);
					BOOL bHeader = TRUE;
					if (title == _T(""))
					{
						bHeader = FALSE;
					}

					printer->PrintLine(cbuf, bHeader);
				}
				printer->EndPrinting();
				ret = TRUE;
			} CATCH_ALL(e){
			}
			END_CATCH_ALL
			if (fp != NULL)
				fclose(fp);
		}
		delete printer;
	}
	::DeleteDC(dc.Detach());
	return ret;
}



/*============================================================================*/
/*! CPrintUtility

-# UTF8�̃t�@�C����SJIS�ɕϊ���Ɉ������

@param	filepath�F�t�@�C���p�X
@retval	�Ȃ�

*/
/*============================================================================*/
BOOL CPrintUtility::PrintUtf8File(CString& filepath, CString title)
{
	BOOL ret;

	// WORK�t�H���_��ۑ���ɂ���
	CString strFileName = filepath.Right(filepath.GetLength() - filepath.ReverseFind(_T('\\')) - 1);
	CString strWorkFilePath;
	strWorkFilePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppWorkPath(), (LPCTSTR)strFileName);

	// WORK�t�H���_��SJIS�t�@�C���ۑ�����
	ret = CFileUtility::ExchangeFileUtf8toSjis(filepath, strWorkFilePath);

	if (ret)
	{
		// WORK�Ƀt�@�C�����ۑ��ł�����������
		ret = PrintSjisFile(strWorkFilePath, title);
	}

	return ret;
}


/*============================================================================*/
/*! CPrintUtility

-# EUC�̃t�@�C����SJIS�ɕϊ���Ɉ������

@param	filepath�F�t�@�C���p�X
@retval	�Ȃ�

*/
/*============================================================================*/
BOOL CPrintUtility::PrintEucFile(CString& filepath, CString title)
{
	BOOL ret;

	// WORK�t�H���_��ۑ���ɂ���
	CString strFileName = filepath.Right(filepath.GetLength() - filepath.ReverseFind(_T('\\')) - 1);
	CString strWorkFilePath;
	strWorkFilePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppWorkPath(), (LPCTSTR)strFileName);

	// WORK�t�H���_��SJIS�t�@�C���ۑ�����
	ret = CFileUtility::ExchangeFileEuctoSjis(filepath, strWorkFilePath);

	if (ret)
	{
		// WORK�Ƀt�@�C�����ۑ��ł�����������
		ret = PrintSjisFile(strWorkFilePath, title);
	}

	return ret;
}


/*============================================================================*/
/*! CPrintUtility

-# ������EUC�̃t�@�C�����P�ɂ܂Ƃ߁ASJIS�ɕϊ���Ɉ������

@param	filepath�F�t�@�C���p�X
@retval	�Ȃ�

*/
/*============================================================================*/
BOOL CPrintUtility::PrintEucFile(vector<CString>& listFilepath, CString title)
{
	BOOL ret = FALSE;

	if (listFilepath.empty())
	{
		return FALSE;
	}

	// WORK�t�H���_��ۑ���ɂ���
	CString strFileName = listFilepath[0].Right(listFilepath[0].GetLength() - listFilepath[0].ReverseFind(_T('\\')) - 1);
	CString strWorkFilePath;
	strWorkFilePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppWorkPath(), (LPCTSTR)strFileName);

	BOOL bOverWrite = TRUE;
	for (int i = 0; i < listFilepath.size(); i++)
	{
		// WORK�t�H���_��SJIS�t�@�C���ۑ�����
		ret = CFileUtility::ExchangeFileEuctoSjis(listFilepath[i], strWorkFilePath, bOverWrite);
		bOverWrite = FALSE;
	}

	if (ret)
	{
		// WORK�Ƀt�@�C�����ۑ��ł�����������
		ret = PrintSjisFile(strWorkFilePath, title);
	}

	return ret;
}

/*============================================================================*/
/*! �ڍ׈��

-# �ڍ׈���R���X�g���N�^

@param	dc			����p�f�o�C�X�R���e�L�X�g
@retval

*/
/*============================================================================*/
CDetailPrint::CDetailPrint(CDC* dc)
{
	mDc = dc;

	mPageVMargin = 0;
	mPageHMargin = 0;
	mPageHeight = 0;
	mSaveVMargin = 0;
	mLineHeight = 0;
	mPageStarted = FALSE;
	mDocStarted = FALSE;
	mPageNumber = 0;

	mFontHeader = NULL;
	mFontSubHeader = NULL;
	mFontBody = NULL;
}

/*============================================================================*/
/*! �ڍ׈��

-# �ڍ׈���f�X�g���N�^

@param
@retval

*/
/*============================================================================*/
CDetailPrint::~CDetailPrint()
{
	if (mDocStarted)
		EndPrinting();
}

/*============================================================================*/
/*! �ڍ׈��

-# �ڍ׈���J�n

@param
@retval	BOOL

*/
/*============================================================================*/
BOOL CDetailPrint::StartPrinting(bool bDiary/*=false*/)
{
	DOCINFO info;
	::ZeroMemory(&info, sizeof(info));
	info.lpszDocName = AfxGetAppName();

	SetPrinterFont(bDiary);

	mDc->StartDoc(&info);
	mDocStarted = TRUE;

	TEXTMETRIC tm;
	mDc->GetTextMetrics(&tm);
	mLineHeight = tm.tmHeight + tm.tmInternalLeading;
	mPageVMargin = mDc->GetDeviceCaps(LOGPIXELSY) / 2;
	mPageHMargin = mDc->GetDeviceCaps(LOGPIXELSX) / 2;
	mPageHeight = mDc->GetDeviceCaps(VERTRES);
	mPageWidth = mDc->GetDeviceCaps(HORZRES);
	mSaveVMargin = mPageVMargin;

// ADD 2018.06.05 >>>
	CSize size;
	// size�Ɏ��T�C�Y���擾����
	VERIFY(mDc->Escape(GETPHYSPAGESIZE, 0, NULL, (LPVOID)&size));
	mPageHeight = size.cy;
	mPageWidth = size.cx;
	// 1�C���`������̃s�N�Z�������擾
	double dPPIX = (double)mDc->GetDeviceCaps(LOGPIXELSX);
	double dPPIY = (double)mDc->GetDeviceCaps(LOGPIXELSY);

	// mm�P�ʂɒ���
	size.cx = (int)(floor(((double)size.cx / dPPIX) * 25.4 + .5));
	size.cy = (int)(floor(((double)size.cy / dPPIY) * 25.4 + .5));
// ADD 2018.06.05 <<<

	return TRUE;
}

/*============================================================================*/
/*! �ڍ׈��

-# �ڍ׈�� �w�b�_�[���

@param
@retval

*/
/*============================================================================*/
void CDetailPrint::PrintHeader()
{
	//CString s(AfxGetAppName());
	int headingY = mPageVMargin - (3 * mLineHeight) / 2;
	int lineY = mPageVMargin - mLineHeight / 2;
	int saved = mDc->SaveDC();
	mDc->SetTextColor(RGB(0, 0, 0));
	CPen pen(PS_SOLID, mDc->GetDeviceCaps(LOGPIXELSX) / 100, RGB(0, 0, 0));
	mDc->SelectObject(pen);
	mDc->TextOut(mPageHMargin, headingY, mTitle);
	CString page;
	page.Format(_T("%d"), mPageNumber);
	int w = mDc->GetTextExtent(page).cx;
	mDc->TextOut(mPageWidth - mPageHMargin - w, headingY, page);
	mDc->MoveTo(mPageHMargin, lineY);
	mDc->LineTo(mPageWidth - mPageHMargin, lineY);
	mDc->RestoreDC(saved);
}

/*============================================================================*/
/*! �ڍ׈��

-# �ڍ׈�� 1�s���

@param		str		1�s������
@retval

*/
/*============================================================================*/
void CDetailPrint::PrintLine(const CString& str, BOOL bHeader)
{
	if (!mPageStarted || mSaveVMargin > mPageHeight - mPageVMargin)
	{
		// ���y�[�W
		if (mPageStarted){
			mDc->EndPage();
		}
		mDc->StartPage();
		mPageNumber++;
		mSaveVMargin = mPageVMargin;
		mPageStarted = TRUE;
		CString s(AfxGetAppName());
		if (bHeader == TRUE)
		{
			PrintHeader();
		}
	}
// CHG 2018.06.05 >>>
	//mDc->TextOut(mPageHMargin, mSaveVMargin, str);
	//mSaveVMargin += mLineHeight;
	CRect rect(mPageHMargin, mSaveVMargin, mPageWidth - mPageHMargin, mPageHeight);
	int offset = mDc->DrawText(str, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK);
	mSaveVMargin += offset;
// CHG 2018.06.05 <<<
}

/*============================================================================*/
/*! �ڍ׈��

-# �ڍ׈���I��

@param
@retval

*/
/*============================================================================*/
void CDetailPrint::EndPrinting()
{
	mDc->EndPage();
	mDc->EndDoc();
	mDocStarted = FALSE;

	if (mFontHeader != NULL)
		delete mFontHeader;
	if (mFontSubHeader != NULL)
		delete mFontSubHeader;
	if (mFontBody != NULL)
		delete mFontBody;
}

/*============================================================================*/
/*! �ڍ׈��

-# �ڍ׈���I��

@param
@retval

*/
/*============================================================================*/
void CDetailPrint::SetPrinterFont(bool bDiary/* = false*/)
{
	CString strFaceName = _T("");
	if (bDiary == true){
		strFaceName = _T("MS �S�V�b�N");
	}

	mFontHeader = CreateFont(12, FW_BOLD, strFaceName);
	mFontSubHeader = CreateFont(10, FW_NORMAL, strFaceName);
	if (bDiary == true){
		mFontBody = CreateFont(8, FW_NORMAL, strFaceName);
	}
	else{
		mFontBody = CreateFont(10, FW_NORMAL, strFaceName);
	}

	//CFont f;
	//f.CreateStockObject(DEVICE_DEFAULT_FONT);
	mDc->SelectObject(mFontBody);
}

/*============================================================================*/
/*! �ڍ׈��

-# �ڍ׈���p�t�H���g�̍쐬

@param
@retval

*/
/*============================================================================*/
CFont* CDetailPrint::CreateFont(int nPoints, int lfWeight, CString strFaceName/*=_T("")*/)
{
	CFont defFont;
	CFont* oldFont = NULL;
	defFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	defFont.GetLogFont(&lf);

	TEXTMETRIC tm;
	mDc->GetTextMetrics(&tm);
	CFont* pFont = new CFont();
	CSize PaperPixelsPerInch(mDc->GetDeviceCaps(LOGPIXELSX), mDc->GetDeviceCaps(LOGPIXELSY));
	lf.lfWeight = lfWeight;
	lf.lfHeight = -MulDiv(nPoints, PaperPixelsPerInch.cy, 72);
	if (strFaceName.IsEmpty() == false){
		wsprintf(lf.lfFaceName, _T("%s"), (LPCTSTR)strFaceName);
		lf.lfQuality = PROOF_QUALITY;
		lf.lfCharSet = SHIFTJIS_CHARSET;
		lf.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;
		lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	}
	BOOL bResult = pFont->CreateFontIndirect(&lf);
	return pFont;
}






/* ------------------------------------------------------------------------------------ */
/* �t�@�C�����[�e�B���e�B
/* ------------------------------------------------------------------------------------ */
CFileUtility::CFileUtility()
{
}

CFileUtility::~CFileUtility()
{
}

/*============================================================================*/
/*! CFileUtility

-# �������Â��e���|�����t�@�C�����폜����

@param	filepath�F���̃t�@�C���p�X
@param	nDay�F�폜���锻�����
@retval	�Ȃ�

*/
/*============================================================================*/
void CFileUtility::DeleteTmpFlie(CString& filepath, int nDay)
{
	char szCmd[_MAX_PATH];
	sprintf_s(szCmd, "forfiles D -%d /M \"%s.*\" / c \"cmd /c del @file\"", nDay, (LPCSTR)CStringA(filepath));
	system(szCmd);
}


/*============================================================================*/
/*! CFileUtility

-# UTF8�t�@�C�����Ă��SJIS�t�@�C���𐶐�����

@param	srcFilePath		�ǂݍ��݃t�@�C����(UTF8)
@param	dstFilePath		�������݃t�@�C����(SJIS)
@retval true:���� / false:���s

*/
/*============================================================================*/
BOOL CFileUtility::ExchangeFileUtf8toSjis(CString& srcFilePath, CString& dstFilePath)
{
	FILE* rFp;
	FILE* wFp;
	BOOL bRead = FALSE;

	if (_wfopen_s(&rFp, srcFilePath, _T("r, ccs=UTF-8")) != 0 || rFp == NULL)
//	if (_wfopen_s(&rFp, srcFilePath, _T("r")) != 0)
	{
		return FALSE;
	}

	if (_wfopen_s(&wFp, dstFilePath, _T("w")) != 0 || wFp == NULL)
	{
		fclose(rFp);
		return FALSE;
	}

	_wsetlocale(LC_ALL,_T("japanese")); 

	CStdioFile	rFile(rFp);
	CStdioFile	wFile(wFp);
	TRY
	{
		bRead = TRUE;

		CString cbuf;
		BOOL ret = rFile.ReadString(cbuf);

		CString str;
		str.Format(_T("%s"),  (LPCTSTR)cbuf);
		CLogTraceEx::Write(_T(""), _T("CPrintUtility"), _T("ExchangeFileUtf8toSjis"), _T("Info"), str, nLogEx::info);

		while (ret){
			// �t�@�C�����P�s���ǂݏo���A�������݂���
			cbuf += _T("\n");
			wFile.WriteString(cbuf);
			ret = rFile.ReadString(cbuf);

			CString str;
			str.Format(_T("%s"), (LPCTSTR)cbuf);
			CLogTraceEx::Write(_T(""), _T("CPrintUtility"), _T("ExchangeFileUtf8toSjis"), _T("Info"), str, nLogEx::info);
		}
		rFile.Close();
		wFile.Close();
	} CATCH_ALL(e){
		if (bRead == TRUE)
			rFile.Close();
		wFile.Close();
		return FALSE;
	}
	END_CATCH_ALL

	return TRUE;
}


/*============================================================================*/
/*! CFileUtility

-# EUC�t�@�C�����Ă��SJIS�t�@�C���𐶐�����

@param	srcFilePath		�ǂݍ��݃t�@�C����(EUC)
@param	dstFilePath		�������݃t�@�C����(SJIS)
@param  bOverWrite		TRUE = �㏑�� / FALSE = �ǉ�
@retval true:���� / false:���s

*/
/*============================================================================*/
BOOL CFileUtility::ExchangeFileEuctoSjis(CString& srcFilePath, CString& dstFilePath, BOOL bOverWrite /*= TRUE*/)
{
	FILE* rFp;
	FILE* wFp;
	BOOL bRead = FALSE;

	if (_wfopen_s(&rFp, srcFilePath, _T("r")) != 0 || rFp == NULL)
	{
		return FALSE;
	}

	// �㏑��
	if (bOverWrite)
	{
		if (_wfopen_s(&wFp, dstFilePath, _T("w")) != 0 || wFp == NULL)
		{
			fclose(rFp);
			return FALSE;
		}
	}
	// �ǉ���������
	else
	{
		if (_wfopen_s(&wFp, dstFilePath, _T("a")) != 0 || wFp == NULL)
		{
			fclose(rFp);
			return FALSE;
		}
	}

	_wsetlocale(LC_ALL, _T("japanese"));

	CStdioFile	wFile(wFp);
	TRY
	{
		bRead = TRUE;

		char szline[2048];
		CString cbuf;
		while (!feof(rFp))
		{
			/* �P�s�ǂݍ��� */
			if (fgets(szline, static_cast<unsigned>(_countof(szline)), rFp) == NULL) {
				break;
			}
			else {
				cbuf = CServerFile::ConvertUnicode(szline);
//				cbuf += _T("\n");
				wFile.WriteString(cbuf);
				{
					CString str;
					str.Format(_T("%s"), (LPCTSTR)cbuf);
					CLogTraceEx::Write(_T(""), _T("CPrintUtility"), _T("ExchangeFileEuctoSjis"), _T("debug"), str, nLogEx::debug);
				}
			}
		}

		fclose(rFp);
		wFile.Close();
	} CATCH_ALL(e){
		if (bRead == TRUE)
			fclose(rFp);
		wFile.Close();
		return FALSE;
	}
	END_CATCH_ALL

	return TRUE;
}


/*============================================================================*/
/*! CFileUtility

-# EUC��������Ă��SJIS������ɕϊ�����

@param	strEuc		EUC������
@retval SJIS������

*/
/*============================================================================*/
CString CFileUtility::ConvertEuc2Sjis(char& szEuc)
{
	TCHAR	szSjis[4098];

	if (MultiByteToWideChar(20932, MB_PRECOMPOSED, &szEuc, -1, szSjis, 4098) != 0)
		return CString(szSjis);
	else
		return CString(_T(""));
}


/* ------------------------------------------------------------------------------------ */
/* DB�A�N�Z�X�N���X
/* ------------------------------------------------------------------------------------ */
CDBAccess::CDBAccess()
{
	m_eqsatdb_data = NULL;
	m_eqctldb_data = NULL;
	m_rdychktimelist.clear();
	m_plntimelist.clear();
}

CDBAccess::~CDBAccess()
{
	if (m_eqsatdb_data != NULL)
	{
		delete m_eqsatdb_data;
	}

	if (m_eqctldb_data != NULL)
	{
		delete m_eqctldb_data;
	}

	if (m_eqmondb_data != NULL)
	{
		delete m_eqmondb_data;
	}

//	deque<char*>::iterator itr;
//	for (itr = mObList.begin(); itr != mObList.end(); itr++){
//		delete (*itr);
//	}
//	mObList.clear();
}

/*============================================================================*/
/*! CDBAccess

-# �}�N����`�̃p�����[�^���̐擪�A�h���X����A�W�J����ݔ����䍀�ږ��𓾂�B

@param	base_p;      input:�������J�n���郁�����̐擪�A�h���X�B
�i�X�̒�`���̐擪�A�h���X�j
@param	tbl[];       output:���䖼���i�[����e�[�u���̐擪�A�h���X
@param	vec[];       output:������擪�A�h���X���i�[����e�[�u���̐擪�A�h���X
@param	i_argc;      input: �Ăяo�����ŗp�ӂ���tbl[]�̌�(=vec[]�̌�)

@retval				-1:����ID�������ł��Ȃ��B
*/
/*============================================================================*/
int CDBAccess::ud_macstsadr_to_exectbl(char *base_p, char *tbl[], char *vec[], int i_argc)
{
	char   *vec_tmp[VECMAX];
	int    i_ret;
	int    i;

	/*
	�p�����[�^�e�[�u������
	*/
	i_ret = ud_adr_to_resvec(base_p, EXEC_ID, BREAK_ID, ENDOFMAC_ID, vec_tmp, VECMAX);
	if (i_ret > i_argc){
		i_ret = i_argc;
	}

	/*
	�p�����[�^���̎擾
	*/
	if (tbl != (char **)NULL)
	{
		for (i = 0; i < i_ret; i++)
		{
			tbl[i] = ((struct exec_t *)vec_tmp[i])->sz_ctl;
		}
	}

	/*
	�p�����[�^���e�[�u���A�h���X�̊i�[
	*/
	if (vec != (char **)NULL){
		for (i = 0; i < i_ret; i++){
			vec[i] = vec_tmp[i];
		}
	}

	return (i_ret);
}


/*============================================================================*/
/*! CDBAccess

-# �}�N����`�̃p�����[�^�����񂩂�A�p�����[�^���̐擪�A�h���X������
-# ���A�W�J����ݔ����䍀�ږ��𓾂�B


@param	base_p;		input:�������J�n���郁�����̐擪�A�h���X�B
					�i�X�̒�`���̐擪�A�h���X�j
@param	psz_param;	input: �p�����[�^��
@param	tbl[];		output:���s���䖼���i�[����e�[�u���̐擪�A�h���X
@param	vec[];		output:���s������擪�A�h���X���i�[����e�[�u���̐擪�A�h���X
@param	i_argc;input: �Ăяo�����ŗp�ӂ���tbl[]�̌�(=vec[]�̌�)

@retval				-1:����ID�������ł��Ȃ��B
*/
/*============================================================================*/
int CDBAccess::ud_macsts_to_exectbl(char *base_p, char *psz_param, char *tbl[], char *vec[], int i_argc)
{
	char   *vec_tmp[VECMAX];
	int    i_ret;
	int    i;

	/*
	�}�N��������
	*/
	i_ret = ud_adr_to_resvec(base_p, CASE_ID, ENDOFMAC_ID, ENDOFMAC_ID, vec_tmp, VECMAX);
	if (i_ret == ERR_RETURN)
	{
		return (ERR_RETURN);
	}

	for (i = 0; i < i_ret; i++)
	{
		if (strcmp(((struct case_t *)vec_tmp[i])->sz_param, psz_param) == 0)
		{
			break;
		}
	}
	if (i >= i_ret)
	{
		return (ERR_RETURN);
	}

	/*
	�p�����[�^�e�[�u������
	*/
	i_ret = ud_macstsadr_to_exectbl(vec_tmp[i], tbl, vec, i_argc);

	return (i_ret);
}

/*============================================================================*/
/*! CDBAccess

-# ��������̉q���ŗL��񂩂�A�}�N����`�̃p�����[�^�̃X�e�[�^�X������
-# ���������A�X�e�[�^�X������̐擪�A�h���X�A�y�уp�����[�^���̐擪�A�h
-# ���X��z��Ɋi�[���ĕԂ��B

@param	base_p;		input:�������J�n���郁�����̐擪�A�h���X�B
					�X�̒�`���̐擪�A�h���X�j
@param	tbl[];      output:�p�����[�^�����i�[����e�[�u���̐擪�A�h���X
@param	vec[];		output:�p�����[�^���擪�A�h���X���i�[����e�[�u���̐擪�A�h���X
@param	i_argc;		input: �Ăяo�����ŗp�ӂ���tbl[]�̌�(=vec[]�̌�)

@retval
*/
/*============================================================================*/
int CDBAccess::ud_macadr_to_ststbl(char *base_p, char *tbl[], char *vec[], int i_argc)
{
	char   *vec_tmp[VECMAX];
	int    i_ret;
	int    i;


	/*
	�p�����[�^�e�[�u������
	*/
	i_ret = ud_adr_to_resvec(base_p, CASE_ID, ENDOFMAC_ID, ENDOFMACDEF_ID, vec_tmp, VECMAX);
	if (i_ret > i_argc){
		i_ret = i_argc;
	}

	/*
	�p�����[�^���̎擾
	*/
	if (tbl != (char **)NULL)
	{
		for (i = 0; i < i_ret; i++)
		{
			tbl[i] = ((struct case_t *)vec_tmp[i])->sz_param;
		}
	}

	/*
	�p�����[�^���e�[�u���A�h���X�̊i�[
	*/
	if (vec != (char **)NULL)
	{
		for (i = 0; i < i_ret; i++)
		{
			vec[i] = vec_tmp[i];
		}
	}

	return (i_ret);
}

/*============================================================================*/
/*! CDBAccess

-# �}�N�����̂���p�����[�^�̃X�e�[�^�X��������������A�X�e�[�^�X����
-# ��̐擪�A�h���X�A�y�уp�����[�^���̐擪�A�h���X��z��Ɋi�[���ĕԂ��B
-# �P�q���̉q���ŗL����S�Č�������̂ŁA�ᑬ�ł���B
-# �i�����������s���ꍇ�́Aud_satname_to_mactbl()�y��ud_macadr_to_ststbl()
-# ���g�p���鎖�B�j

@param	char *base_p;	input:�������J�n���郁�����̐擪�A�h���X�B
                          �i�X�̒�`���̐擪�A�h���X�j
@param	psz_macname;	input: �}�N����
@param	tbl[];			output:�X�e�[�^�X�����i�[����e�[�u���̐擪�A�h���X
@param	vec[];			output:�X�e�[�^�X���擪�A�h���X���i�[����e�[�u���̐擪�A�h���X
@param	i_argc;			input: �Ăяo�����ŗp�ӂ���tbl[]�̌�(=vec[]�̌�)

@retval		-1:�����}�N�����������ł��Ȃ��B
*/
/*============================================================================*/
int CDBAccess::ud_macname_to_ststbl(char *base_p, char *psz_macname, char *tbl[], char *vec[], int i_argc)
{
	char   *vec_tmp[VECMAX];
//	struct mac_t  *pst_mac;
	int    i_ret;
	int    i;

	/*
	�}�N��������
	*/
	i_ret = ud_adr_to_resvec(base_p, MAC_ID, ENDOFMACDEF_ID, ENDOFMACDEF_ID, vec_tmp, VECMAX);
	if (i_ret == ERR_RETURN)
	{
		return (ERR_RETURN);
	}

	for (i = 0; i < i_ret; i++)
	{
		if (strcmp(((struct mac_t *)vec_tmp[i])->sz_macname, psz_macname) == 0)
		{
			break;
		}
	}
	if (i >= i_ret){
		return (ERR_RETURN);
	}

	/*
	�p�����[�^�e�[�u������
	*/
	i_ret = ud_macadr_to_ststbl(vec_tmp[i], tbl, vec, i_argc);

	return (i_ret);
}

/*============================================================================*/
/*! CDBAccess

-# �X�̐ݔ�������A�q����`���A�܂��́A�ݔ�����`�̐擪�A�h���X����A
-# �f�[�^�h�c�l �̈�v����f�[�^�\���̐擪�A�h���X�����߂�B
-# �i�ݔ�������C�q���ŗL���C���Ɠ����̃f�[�^�\�������e�[�u���̌�����
-# �g����ėp�̃��[�`���j
-# ud_adr_to_res()�Ƃ̈Ⴂ�́A�T���Ă���f�[�^�h�c�Ɉ�v����e�[�u��������
-# ���݂��Ă���ꍇ�́A�����̃e�[�u���̐擪�A�h���X��z��Ɋi�[ ���邱��
-# �ɂ���B

@param	list
@retval
*/
/*============================================================================*/
int CDBAccess::ud_adr_to_resvec(char* base_p, long l_id, long l_limitid, long l_endoftblid, char* vec[], int i_argc)
{
	struct res_t *p;
	int    n = 0;

	for (p = (struct res_t *)base_p; p->l_id != NULL; p = (struct res_t *)((char*)p + p->l_len))
	{
		if (p->l_len == 0)
		{
			break;
		}
		if (p->l_id == l_id)
		{
			vec[n] = (char *)p;
			n++;
			if (n == i_argc)
			{
				break;
			}
		}
		else if ((p->l_id == l_endoftblid) || (p->l_id == l_limitid))
		{
			break;
		}
	}

	return n;
}

/*============================================================================*/
/*! CDBAccess

-# ��������̉q���ŗL���c�a����A�e�q����������̐擪�A�h���X��
-# �z��Ɋi�[���ĕԂ��B

@param	list
@retval 

*/
/*============================================================================*/
int CDBAccess::ud_adr_to_sattbl(vector<CString>& list)
{
	char   *vec[VECMAX];
	int i_ret;
	int i;

	i_ret = ud_adr_to_resvec((char *)(m_eqsatdb_data) + sizeof(long) + sizeof(long), SATINF_ID, NULL, NULL, vec, VECMAX);

	/*
	�q�����擾
	*/
	for (i = 0; i < i_ret; i++)
	{
		CString str = CString(((struct satinf_t *)vec[i])->sz_satname);
		list.push_back(str);
		m_satadrlist[str] = *(__int64*)&vec[i];
	}
	return (i_ret);
}

/*============================================================================*/
/*! CDBAccess

-# �q���ŗL���DB�Ǎ�����

@param	list
@retval

*/
/*============================================================================*/
int CDBAccess::eqsat_db_read()
{
	UINT sta;
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		// �P�ǃ��[�h�́A�Y���ǂ��w��
		sta = theApp.GetSelectStation();
	}
	else
	{
		// �S�ǃ��[�h�́A�q���ŗLDB��34m�̂�S��/X�ѕʂŊĎ����ؑւ�����B
		// 34m�ǂ��w��
		sta = eStation_Uchinoura34;
	}

	CString filename = theApp.GetShareFilePath(eFileType_DB_Eqsat_File, sta);

	try{
		HANDLE hFile = CreateFileA(CStringA(filename), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		
		if (hFile == INVALID_HANDLE_VALUE){
			return -1;
		}
		DWORD mFileSize = GetFileSize(hFile, NULL) + 100;

		CloseHandle(hFile);
		if (m_eqsatdb_data != NULL)
		{
			delete m_eqsatdb_data;
		}
		m_eqsatdb_data = new char[mFileSize];
		//DB���i�[�̈��������
		memset(m_eqsatdb_data, 0, mFileSize);

		//DB�t�@�C����ǂ�ł���
		if (db_read(CStringA(filename).GetBuffer(), m_eqsatdb_data) != 0)
		{
			return -1;
		}

		m_satnamelist.clear();
		m_satadrlist.clear();
		ud_adr_to_sattbl(m_satnamelist);
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}

/*============================================================================*/
/*! CDBAccess

-# ��������̉q���ŗL���c�a����A�e�q����������̐擪�A�h���X��
-# �z��Ɋi�[���ĕԂ��B

@param	list
@retval

*/
/*============================================================================*/
int CDBAccess::ud_adr_to_montbl(vector<CString>& list)
{
	char   *vec[VECMAX*100];
	int i_ret;
	int i;

	i_ret = ud_adr_to_resvec((char *)(m_eqmondb_data)+sizeof(long) + sizeof(long), OBSNAME_ID, NULL, NULL, vec, VECMAX * 100);

	/*
	�q�����擾
	*/
	for (i = 0; i < i_ret; i++)
	{
		CString str = CString(((struct CEngValCnv::obsname_t *)vec[i])->sz_obsname);
		list.push_back(str);
	}
	return (i_ret);
}

/*============================================================================*/
/*! CDBAccess

-# �q���ŗL���DB�Ǎ�����

@param	list
@retval

*/
/*============================================================================*/
int CDBAccess::eqmon_db_read()
{
	CString filename = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, theApp.GetSelectStation());

	try{
		HANDLE hFile = CreateFileA(CStringA(filename), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE){
			return -1;
		}

		DWORD mFileSize = GetFileSize(hFile, NULL) + 100;

		CloseHandle(hFile);
		if (m_eqmondb_data != NULL)
		{
			delete m_eqmondb_data;
		}
		m_eqmondb_data = new char[mFileSize];
		//DB���i�[�̈��������
		memset(m_eqmondb_data, 0, mFileSize);

		//DB�t�@�C����ǂ�ł���
		if (db_read(CStringA(filename).GetBuffer(), m_eqmondb_data) != 0)
		{
			return -1;
		}

		m_monnamelist.clear();
		ud_adr_to_montbl(m_monnamelist);

		delete m_eqmondb_data;	// ��荇�����Ď������X�g���~���������Ȃ̂ō폜�B�i2017/04/14�A�Ό��j
		m_eqmondb_data = NULL;
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}

/*============================================================================*/
/*! CDBAccess

-# ��������̐ݔ�����DB����A�e�q����������̐擪�A�h���X��
-# �z��Ɋi�[���ĕԂ��B

@param	list
@retval

*/
/*============================================================================*/
int CDBAccess::ud_adr_to_ctltbl(vector<CString>& list)
{
	char   *vec[VECMAX*10];
	int i_ret;
	int i;

	i_ret = ud_adr_to_resvec((char *)(m_eqctldb_data) + sizeof(long) + sizeof(long), CTLNAME_ID, NULL, NULL, vec, VECMAX*10);

	/*
	�q�����擾
	*/
	for (i = 0; i < i_ret; i++)
	{
		CString str = CString(((struct ctlname_t *)vec[i])->sz_ctlname);
		list.push_back(str);
		m_ctladrlist[str] = *(__int64*)&vec[i];
	}

	return (i_ret);
}

/*============================================================================*/
/*! CDBAccess

-# �ݔ�����DB�Ǎ�����

@param	list
@retval

*/
/*============================================================================*/
int CDBAccess::eqctl_db_read()
{
	CString filename = theApp.GetShareFilePath(eFileType_DB_Eqctl_File, theApp.GetSelectStation());

	try{
		HANDLE hFile = CreateFileA(CStringA(filename), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE){
			return -1;
		}
		DWORD mFileSize = GetFileSize(hFile, NULL) + 100;

		CloseHandle(hFile);
		if (m_eqctldb_data != NULL)
		{
			delete m_eqctldb_data;
		}
		m_eqctldb_data = new char[mFileSize];
		//DB���i�[�̈��������
		memset(m_eqctldb_data, 0, mFileSize);

		//DB�t�@�C����ǂ�ł���
		if (db_read(CStringA(filename).GetBuffer(), m_eqctldb_data) != 0)
		{
			return -1;
		}

		m_ctlnamelist.clear();
		m_ctladrlist.clear();
		ud_adr_to_ctltbl(m_ctlnamelist);
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}

int CDBAccess::db_read(char* db_file_nam, char *cp_loadadr_p)
{
	int errno;
	struct stat stbuf;
	int    i_readbyte;
	long   l_alldatasize = 0, l_datasize;
	char   sz_work[256] = { 0 };

	unsigned long ul_top_pos; /*�t�@�C���̐擪���烁�������[�h���e�̐擪�܂ł̃o�C�g��*/
	long   l_idnum = 0;
	FILE *fp = NULL;
	char *fname = db_file_nam;

	/*�t�@�C�����Ƃ݂Ȃ�*/
	if (stat(fname, &stbuf) == -1)
	{
		return(500);
	}

	l_datasize = stbuf.st_size;


	if (l_idnum == 0) 
	{
		l_alldatasize += l_datasize;
	}
	else 
	{
		l_alldatasize += (l_datasize * l_idnum);
	}

	errno_t ans;
	/*cp_loadadr_p = cp_loadadr;*/
	if ((ans = fopen_s(&fp, fname, "rb")) != 0 || fp == NULL)
	{
		return(500);
	}

	if (fread(sz_work, sizeof(char), sizeof(sz_work), fp) == -1)
	{
		/*#!HEAD:�����邩�ǂ����ǂ�ł݂�*/
		return(500);
	}
	if (!strncmp(sz_work, "#!HEAD:", 7))
	{
		/*#!HEAD:��ǂݔ�΂�*/
		if ((ul_top_pos = (unsigned long)strchr(sz_work, '\n')) == NULL)
		{
			return(500);
		}
		else
		{
			ul_top_pos = (ul_top_pos - (unsigned long)&sz_work[0]) + 1;
		}
	}
	else
	{
		/*�t�@�C���̐擪����S���e��ǂݍ���*/
		ul_top_pos = 0; /*�t�@�C���̐擪���烁�����Ƀ��[�h������e�̐擪�܂ł̃o�C�g��*/
	}
	if (fseek(fp, ul_top_pos, SEEK_SET) == -1)
	{
		return(500);
	}
	for (;;)
	{
		i_readbyte = (int)fread(cp_loadadr_p, sizeof(char), (l_datasize - ul_top_pos), fp);
		if (i_readbyte <= 0)
		{
			break; 
		}
		if (i_readbyte == (l_datasize - ul_top_pos))
		{
			break; 
		}
		l_datasize -= i_readbyte;

		cp_loadadr_p = cp_loadadr_p + i_readbyte;
	}
	cp_loadadr_p = cp_loadadr_p + (l_datasize - ul_top_pos);
	l_alldatasize -= ul_top_pos; /*#!HEAD:�̕����̃o�C�g����␳*/
	fclose(fp); /*�t�@�C����close����*/

	return 0;
}

/*============================================================================*/
/*! CDBAccess

-# �Ď��f�[�^�i�[

@param	nStation	�F�ǔԍ�
@param	data		�F�H�w�l�ϊ��f�[�^
@retval

*/
/*============================================================================*/
#if 1
void CDBAccess::SetEqMonData(UINT nStation, vector<CEngValCnv::obs_tbl_res_t>& data)
{
	CGraphMutex::Lock(eData);

	int cnt = (int)data.size();
	for (int i = 0; i < cnt; ++i)
	{
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: Add eqmondata(%d): Time(%lf), SigName(%s), SigType(0x%X) dVal(%f), lVal(%d), Sts(%s)"), __FILEW__, __LINE__, nStation, data[i].ld_time_val, (LPCTSTR)CString(data[i].obs_adr.sz_obsname), data[i].obs_adr.l_cnvResType, data[i].obs_adr.d_data, data[i].obs_adr.l_data, (LPCTSTR)CString(data[i].obs_adr.sz_sts_name));
			CLogTraceEx::Write(_T(""), _T("CDBAccess"), _T("SetEqMonData"), _T("detail"), strDebug, nLogEx::detail);
		}
		CEngValCnv::obs_tbl_res_t mon_data = data[i];
		CString obsname = CString(mon_data.obs_adr.sz_obsname);
		m_mondatalist[nStation][obsname] = mon_data.obs_adr;
		theApp.UpdateControlCommand(nStation, obsname, CString(mon_data.obs_adr.sz_sts_name));
	}

	CGraphMutex::Unlock(eData);
}

#else
void CDBAccess::SetEqMonData(UINT nStation, vector<CEngValCnv::obs_tbl_res_t>& data)
{
	CGraphMutex::Lock(eData);

	//vector<CEngValCnv::obsname_t> temp(0);
	//m_mondatalist[nStation].swap(temp);
	//m_mondatalist[nStation].clear();

	vector<CEngValCnv::obs_tbl_res_t>::iterator itro;
	for (itro = data.begin(); itro != data.end(); itro++){
		{
			CString strDebug;
			strDebug.Format(_T("[%d]%s(%d) :: Add eqmondata: Time(%lf), SigName(%s), dVal(%f), lVal(%d), Sts(%s)"), nStation, __FILEW__, __LINE__, (*itro).ld_time_val, CString((*itro).obs_adr.sz_obsname), (*itro).obs_adr.d_data, (*itro).obs_adr.l_data, CString((*itro).obs_adr.sz_sts_name));
			CLogTraceEx::Write(_T(""), _T("CDBAccess"), _T("SetEqMonData"), _T("debug"), strDebug, nLogEx::debug);
		}
		vector<CEngValCnv::obsname_t>::iterator itr;
		for (itr = m_mondatalist[nStation].begin(); itr != m_mondatalist[nStation].end(); itr++){
			if (CString((*itr).sz_obsname) == CString((*itro).obs_adr.sz_obsname)){
				(*itr) = (*itro).obs_adr;
				CGraphMutex::Unlock(eData);
				return;
			}
		}
		m_mondatalist[nStation].push_back((*itro).obs_adr);
	}
	CGraphMutex::Unlock(eData);
}
#endif

/*============================================================================*/
/*! CDBAccess

-# �Ď��f�[�^�擾

@param	nStation	�F�ǔԍ�
@param	strWatch	�F�Ď�����
@retval strct obsname_t �|�C���^

*/
/*============================================================================*/
char* CDBAccess::GetObsname_t(UINT nStation, CString strWatch)
{
	char *obs = NULL;

	CGraphMutex::Lock(eData);

	if (m_mondatalist.count(nStation) != 0)
	{
		map<CString, CEngValCnv::obsname_t> &mon = m_mondatalist[nStation];

		if (mon.count(strWatch) != 0)
		{
			obs = (char*)&mon[strWatch];
		}
	}

#ifdef MON_UT
	if( (obs == NULL) && (!mon_UT.empty()) )
	{
		map<CString, CEngValCnv::obsname_t> &mon_ut = mon_UT;
		if(mon_ut.count(strWatch) != 0)
		{
			obs = (char*)&mon_ut[strWatch];
		}
	}

	if (obs == NULL && strWatch.Find(_T("MON_TIME")) >= 0)
	{
		// �����Ď��̏ꍇ�A���ݎ������X�V���ĕԂ�
		memset(&mon_UT_time, 0, sizeof(mon_UT_time));
		strcpy_s(mon_UT_time.sz_obsname, CTLID_LEN, CStringA(strWatch));
		mon_UT_time.d_data = (double)CTime::GetCurrentTime().GetTime();
		mon_UT_time.l_inflg = INFLG_ON;	//�f�[�^�L���t���O 1=�L��
		obs = (char*)&mon_UT_time;
	}
#endif

	CGraphMutex::Unlock(eData);

	return obs;
}

/*============================================================================*/
/*! CDBAccess

-# �q���ŗLDB���擾�����f�[�^���Y������q���̃C���f�b�N�X���擾����

@param	str :�q����
@retval	�C���f�b�N�X
*/
/*============================================================================*/
int CDBAccess::GetIdxSatelliteEqSatDB(CString str)
{
	int idx = -1;

	struct satinf_t *satinf_adr;
	map<CString, __int64>	&satAdrList = GetSatAdrList();

	for (auto itr = satAdrList.begin(); itr != satAdrList.end(); ++itr)
	{
		CString sat = itr->first;
		if (sat == str)
		{
			satinf_adr = (struct satinf_t *)((__int64*)itr->second);
			idx = satinf_adr->l_satno;
			break;
		}
	}

	return idx;
}

/*============================================================================*/
/*! CDBAccess

-# ���f�B�l�X�`�F�b�N���ԃt�@�C���ǂݍ���

@param
@retval

*/
/*============================================================================*/
int CDBAccess::rdychk_read()
{
	FILE *fp;
	TCHAR sz_buff[64];
	TCHAR sz_ele[2][32];

	bool bRead = false;

	try{
		CString filename = theApp.GetShareFilePath(eFileType_Rdychk_File, theApp.GetSelectStation());
		CString strEle1, strEle2;
		int	val = 0;

		if ((_wfopen_s(&fp, filename, _T("r")) != 0) || (fp == NULL))
		{
			return -1;
		}

		char sz_line[512];
		while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp) != NULL)
		{
			bRead = true;
			wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
			swscanf_s(sz_buff, _T("%s%s;"),
				sz_ele[0], static_cast<unsigned>(_countof(sz_ele[0])),
				sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])));

			strEle1 = CString(sz_ele[0]);
			strEle2 = CString(sz_ele[1]);
			int idx = strEle2.Find(_T(";"));
			if (idx != -1)
			{
				strEle2 = strEle2.Mid(0, idx);
			}

			val = _wtoi(strEle2);

			m_rdychktimelist[strEle1] = val;
		}
		fclose(fp);
	}
	catch (...)
	{
		if (bRead == true)
			return 0;
		return -1;
	}

	return 0;
}

/*============================================================================*/
/*! CDBAccess

-# ���f�B�l�X�`�F�b�N��`�t�@�C�����Y���̎��Ԃ��擾����

@param	str�F�^�[�Q�b�g
@retval

*/
/*============================================================================*/
int CDBAccess::GetRdyChkTimeFile(CString str)
{
	int val = 0;

	if (m_rdychktimelist.find(str) != m_rdychktimelist.end())
	{
		val = m_rdychktimelist[str];
	}

	return val;
}

/*============================================================================*/
/*! CDBAccess

-# �v�掞�ԃt�@�C���ǂݍ���

@param
@retval

*/
/*============================================================================*/
int CDBAccess::plntime_read()
{
	FILE *fp;
	TCHAR sz_buff[64];
	TCHAR sz_ele[2][32];

	bool bRead = false;

	try{
		CString filename = theApp.GetShareFilePath(eFileType_PlnTime_File, theApp.GetSelectStation());
		CString strEle1, strEle2;
		int	val = 0;

		if ((_wfopen_s(&fp, filename, _T("r")) != 0) || (fp == NULL))
		{
			return -1;
		}

		char sz_line[512];
		while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp) != NULL)
		{
			bRead = true;
			wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
			swscanf_s(sz_buff, _T("%s%s;"),
				sz_ele[0], static_cast<unsigned>(_countof(sz_ele[0])),
				sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])));

			strEle1 = CString(sz_ele[0]);
			strEle2 = CString(sz_ele[1]);
			int idx = strEle2.Find(_T(";"));
			if (idx != -1)
			{
				strEle2 = strEle2.Mid(0, idx);
			}

			val = _wtoi(strEle2);

			m_plntimelist[strEle1] = val;
		}
		fclose(fp);
	}
	catch (...)
	{
		if (bRead == true)
			return 0;
		return -1;
	}

	return 0;
}

/*============================================================================*/
/*! CDBAccess

-# �v���`�t�@�C�����Y���̎��Ԃ��擾����

@param	str�F�^�[�Q�b�g
@retval

*/
/*============================================================================*/
int CDBAccess::GetPlnTimeFile(CString str)
{
	int val = -1;

	if (m_plntimelist.find(str) != m_plntimelist.end())
	{
		val = m_plntimelist[str];
	}

	return val;
}

#ifdef MON_UT
/*============================================================================*/
// �Ď����e�[�u���̍X�V
/*============================================================================*/
void CDBAccess::SetMonUT(vector<CEngValCnv::obsname_t>& data)
{
	int cnt = (int)data.size();

	CGraphMutex::Lock(eData);

	mon_UT.clear();
	for(int i=0; i<cnt; ++i)
	{
		CEngValCnv::obsname_t mon_data = data[i];
		CString obsname = CString(mon_data.sz_obsname);
		mon_UT[obsname] = mon_data;
	}

	CGraphMutex::Unlock(eData);
}
#endif


/* ------------------------------------------------------------------------------------ */
/* �Z�b�V�������
/* ------------------------------------------------------------------------------------ */
CSessionInfo::CSessionInfo()
{
	for (int i = 0; i < eSessionType_Num; i++)
	{
		m_sessionStatus[i] = eUnknown;
	}
}

CSessionInfo::~CSessionInfo()
{
}

//#if 0
///*============================================================================*/
///*! CSessionInfo
//
//-# �Z�b�V������Ԃ��X�V����
//
//@param	�Ȃ�
//@retval	�Ȃ�
//
//*/
///*============================================================================*/
//void CSessionInfo::UpdateSessionStatus()
//{
//	// STAT_SDTP�ŕۑ����ꂽ���[�J���̌��ʃt�@�C����ǂݍ��ށB
//	// (�H�����j
//	for (int i = 0; i < eSessionType_Num; i++)
//	{
//		CFileFind find;
//		CString strLockFilePath;
//		strLockFilePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetManagementPath(), STAT_SDTP_FILENAME);
//
//		if (find.FindFile(strLockFilePath))
//		{
//			// �ڑ���Ԃɂ���
//			m_sessionStatus[i] = CSessionInfo::eConnect;
//		}
//		else
//		{
//			// �ؒf��Ԃɂ���
//			m_sessionStatus[i] = CSessionInfo::eDisconnect;
//		}
//	}
//}
//#endif


/*============================================================================*/
/*! CSessionInfo

-# �Z�b�V�����^�C�v������ɊY������C���f�b�N�X��Ԃ�

@param	str�F�Z�b�V�����^�C�v������
@retval	0�ȏ�F�Y������C���f�b�N�X�A-1�F�Y���Ȃ�

*/
/*============================================================================*/
int CSessionInfo::GetSessionIdx(CString& str)
{
	for (int i = 0; i < eSessionType_Num; i++)
	{
		if (str.CompareNoCase(msStrSessionType[i]) == 0)
			return i;
	}

	return -1;
}


/*============================================================================*/
/*! CSessionInfo

-# �Z�b�V�����^�C�v�փZ�b�V������Ԃ��Z�b�g����

@param	str�F�Z�b�V�����^�C�v������
@retval	0�ȏ�F�Y������C���f�b�N�X�A-1�F�Y���Ȃ�

*/
/*============================================================================*/
void CSessionInfo::SetSessionStatus(CString& strSession, CString& strStatus)
{
	for (int i = 0; i < eSessionType_Num; i++)
	{
		int nSession = GetSessionIdx(strSession);
		if (nSession >= 0)
		{
			if (strStatus.CompareNoCase(_T("0")) == 0)
			{
				// �ؒf��ԃZ�b�g
				m_sessionStatus[nSession] = eDisconnect;
			}
			else if (strStatus.CompareNoCase(_T("1")) == 0)
			{
				// �ڑ���ԃZ�b�g
				m_sessionStatus[nSession] = eConnect;
			}
			else if (strStatus.CompareNoCase(_T("9")) == 0)
			{
				// �s����ԃZ�b�g
				m_sessionStatus[nSession] = eUnknown;
			}

			break;
		}
	}
}


/* ------------------------------------------------------------------------------------ */
/* �q���ߑ��f�[�^
/* ------------------------------------------------------------------------------------ */
CTrendData::CTrendData()
{
	for (int i = 0; i < eStation_MAX; i++)
	{
		m_yohoFileTime[i].dwHighDateTime = 0;
		m_yohoFileTime[i].dwLowDateTime = 0;
	}
}


CTrendData::~CTrendData()
{
}


/*============================================================================*/
/*! CTrendData

-# �\��l�t�@�C�����擾

�w��̗\��l�t�@�C�����擾���A�`�n�r�^�k�n�r
�擾�s���͗v����ʖ��ɕs���̑Ή����s���B

@param	�v����ʁi���A���A�ݒ�I���A�f�t�H���g�j
@param	�ǔԍ�
@retval	����i�O�j�^�擾�҂��i�P�j�^�ُ�i���O�j

*/
/*============================================================================*/
int CTrendData::gryohouti(int i_get_type, UINT nStation)
{
	int     	i_ret;

#ifdef _ENGVALCNV_TREND
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT sta = theApp.GetSelectStation();

	// �q�����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(EISEI_NAME));
	if (obs_adr == NULL)
		return -1;
	CString strSatellite = CString(obs_adr->sz_sts_name);

	if (strSatellite.CompareNoCase(_T("VLBI")) != 0)
	{
		// VLBI�łȂ��ꍇ�A�\��l�̎擾��Ԃ𒲂ׂ�

		/*** �\��l�t�@�C���擾�۔��� ***/
		/* �\��l�擾��� �擾 */
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(YOHOU_GET));
		if (obs_adr == NULL)
			return -1;
		CString strYOHOU_GET;
		strYOHOU_GET.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
		strYOHOU_GET.Trim(_T(" "));
		if (strYOHOU_GET.IsEmpty())
		{
			CString str;
			str.Format(_T("%s(%d) :: Fail to get yohochi"), __FILEW__, __LINE__);
			CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("gryohouti"), _T("Error"), str, nLogEx::error);
			return(-1);
		}

		/* �Ď��f�[�^�擾������ */
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_yohouti_get = YOHOGET_MATI;    /* �擾�҂� */
			m_gst_dspinf.i_yohouti_umu = YOHOUTI_NASI;    /* (V2-1)   */

			CString str;
			str.Format(_T("%s(%d) :: yohochi(%s) is invalid"), __FILEW__, __LINE__, (LPCTSTR)CString(YOHOU_GET));
			CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("gryohouti"), _T("Error"), str, nLogEx::error);
			return(1);
		}
		/* �۔��� */
		if (strYOHOU_GET.CompareNoCase(_T("TAKING")) == 0) {	/* �\��l�擾�� */
			m_gst_dspinf.i_yohouti_get = YOHOGET_MATI;    /* �擾�҂� */
			m_gst_dspinf.i_yohouti_umu = YOHOUTI_NASI;    /* (V2-1)   */

			CString str;
			str.Format(_T("%s(%d) :: Can not yohochi, becase is is TAKING"), __FILEW__, __LINE__);
			CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("gryohouti"), _T("info"), str, nLogEx::info);
			return(1);
		}
		else {
			m_gst_dspinf.i_yohouti_get = YOHOGET_SUMI;    /* �擾�ς� */
		}
	}
	else
	{
		// VLBI�̏ꍇ
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(_T("CTRL.PASS_INFO")));
		if (obs_adr != NULL)
		{
			if (obs_adr->l_inflg != INFLG_ON) {
				m_gst_dspinf.i_yohouti_get = YOHOGET_MATI;    /* �擾�҂� */
				m_gst_dspinf.i_yohouti_umu = YOHOUTI_NASI;    /* (V2-1)   */
				return(1);
			}
			CString strPassEnd(obs_adr->sz_sts_name);
			if (strPassEnd.Compare(_T("�^�p�I��")) == 0)
			{
				m_gst_dspinf.i_yohouti_get = YOHOGET_MATI;    /* �擾�҂� */
				m_gst_dspinf.i_yohouti_umu = YOHOUTI_NASI;    /* (V2-1)   */
				return(1);
			}
			else
			{
				m_gst_dspinf.i_yohouti_get = YOHOGET_SUMI;    /* �擾�ς� */
			}
		}
	}

#else
	m_gst_dspinf.i_yohouti_get = YOHOGET_SUMI;
#endif

	/*** �\��l�t�@�C���擾���� ***/
	switch (i_get_type) {
	case GET_REAL:  /* ���A���\�����ł̎擾 */
		if (m_gst_dspinf.i_yohouti_get == YOHOGET_SUMI) {
			i_ret = gryohouti2(0, nStation);	/* <<XUP>> �����Ƀv���b�g���[�h��ǉ� */
		}
		else {
			return(1);
		}
		break;
	case GET_SET:   /* �ۑ��\�����ł̗\��l�I���u�ݒ�v���̎擾 */
	case GET_DEF:   /* �ۑ��\�����ł̗\��l�I���u�f�t�H���g�v���̎擾 */
		if (m_gst_dspinf.i_yohouti_get == YOHOGET_SUMI) {
			i_ret = gryohouti2(1, nStation);	/* <<XUP>> �����Ƀv���b�g���[�h��ǉ� */
		}
		else {
			//			grerrbox(ERR_TITL_YOGET, ERR_MSG_YOGET);
			return(1);
		}
		break;
	default:
		printf(" gryohouti program error(undefine input para)\n");
		return(-2);
	}

	return(i_ret);
}

/*============================================================================*/
/*! CTrendData

-# �\��l�t�@�C�����擾
�w��̉q�����p�X�ԍ��̗\��l�t�@�C�����Q�Ƃ��A
�`�n�r�^�k�n�r�������擾����
�\��l�t�@�C�����Ȃ��ꍇ�́A�ŐV�Ď��f�[�^����������
�`�n�r�����A���ꂩ��Q�S�g����k�n�r�����Ƃ݂Ȃ��B

@param	i_prot_mode�F�v���b�g���[�h
0:���A���f�[�^ �^ 1:�ۑ��f�[�^
@param	�ǔԍ�
@retval	����i�O�j�^���ύX�i�P�j�^�t�@�C�����e���ُ�i�|�P�j

*/
/*============================================================================*/
int CTrendData::gryohouti2(int i_prot_mode, UINT nStation)
{
	int     i;
	char    ca_fpath[SATDATA_BUFF_SIZE];
	char	ca_TIME[26];			/* �����i�t���t�H�[�}�b�g�j		  */
	double	d_aos;
	double	d_los;
	long l_az;

//	// �q�����������̃}�b�v�N���A
//	m_2WayRngMap.clear();

	/*--------------*/
	/* �t�@�C���擾 */
	/*--------------*/

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

	UINT sta = theApp.GetSelectStation();

	/*** �\��l�t�@�C���擾�۔��� ***/
	/* �\��l�擾��� �擾 */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(PRED_FILE_NAME));
	if (obs_adr == NULL)
		return -1;
	CString strYOHOU_OpCtrlFilePath;
	strYOHOU_OpCtrlFilePath.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
	strYOHOU_OpCtrlFilePath.Trim(_T(" "));
	// �g���q��prd�̏ꍇ�Aanp�ɒu��������
	strYOHOU_OpCtrlFilePath.TrimRight(PRED_FILE_TYPE);

	if (strYOHOU_OpCtrlFilePath.IsEmpty())
	{
		CString str;
		str.Format(_T("%s(%d) :: Could not get yohochi file path"), __FILEW__, __LINE__);
		CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("gryohouti2"), _T("debug"), str, nLogEx::debug);
		return(-1);
	}

	strYOHOU_OpCtrlFilePath += ANP_FILE_TYPE;

	// �t�@�C������؂�o��
	CString strFileName = strYOHOU_OpCtrlFilePath.Right(strYOHOU_OpCtrlFilePath.GetLength() - strYOHOU_OpCtrlFilePath.ReverseFind(_T('/')) - 1);
	CString strFileTmp = strYOHOU_OpCtrlFilePath.TrimRight(strFileName);
	strFileTmp = strFileTmp.TrimRight(_T("/"));
	// �q������؂�o��
	CString strSatelliteName = strFileTmp.Right(strFileTmp.GetLength() - strFileTmp.ReverseFind(_T('/')) - 1);
	// �ǃt�H���_���擾
	CString strStationPath;
	strStationPath = theApp.GetShareFilePath(eFileType_Pred_Folder, nStation);
	// �ǃt�H���_������
	CString strFilePath;
	strFilePath.Format(_T("%s%s\\%s"), (LPCTSTR)strStationPath, (LPCTSTR)strSatelliteName, (LPCTSTR)strFileName);

	if (strFilePath.IsEmpty())
		return -1;

//	TRACE("@@@%s\n", CStringA(strFilePath));

	// �\��l�t�@�C�����X�V����Ă��邩�m�F
	HANDLE hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FILETIME yohoFileTime;
	GetFileTime(hFile, NULL, NULL, &yohoFileTime);
	CloseHandle(hFile);
	// �t�@�C�����ƃt�@�C��������r
	if (m_yohoFilePath[nStation].Compare(strFilePath) == 0 &&
		CompareFileTime(&m_yohoFileTime[nStation], &yohoFileTime) == 0)
	{
		// �ύX�Ȃ��Ȃ�Ή������Ȃ�
		return 1;
	}

	/*------------------*/
	/* �t�@�C���n�o�d�m */
	/*------------------*/
	size_t wLen = 0;
	errno_t err = 0;
	err = wcstombs_s(&wLen, ca_fpath, SATDATA_BUFF_SIZE, strFilePath.GetBuffer(), _TRUNCATE);
	strFilePath.ReleaseBuffer();

	CGraphMutex::Lock(eRadar);

	if (err != 0 || load_pred_data(ca_fpath, &d_aos, &d_los) != 0)
	{
		CGraphMutex::Unlock(eRadar);
		return -1;
	}

	m_yohoFilePath[nStation] = strFilePath;
	m_yohoFileTime[nStation] = yohoFileTime;
	m_gst_dspinf.d_AOS_time = d_aos;
	m_gst_dspinf.d_LOS_time = d_los;

	m_gst_dspinf.i_yohouti_umu = YOHOUTI_ARI;

	/*----------------------*/
	/* �c���X�P�[�����擾 */
	/*----------------------*/
	/* �`�n�r�^�k�n�r��񃌃R�[�h�܂Ō��� */
	/*	for( i = 0; i < 5; i++ ) { */

	CTrackingRadarCtrlMonData	*pRadarData = &theApp.GetSatelliteData().m_arrayTrackingRadarCtrlMonData[nStation];
	if (pRadarData == NULL)
	{
		CGraphMutex::Unlock(eRadar);
		return -1;
	}

	// �q�����������̃}�b�v�N���A
	m_2WayRngMap.clear();

	// �\��l���O���t�̃L���[����S�č폜����
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		(&theApp.GetDataCollection())->ClearCollection(theApp.GetSelectStation());
	}

	// EL�\��l��Ԍv�Z
	CSpaceData	ELPredData;
	bool		bPredDiffEna;
	if (theApp.IsPredDiffEnable())
	{
		bPredDiffEna = true;
	}
	else
	{
		bPredDiffEna = false;
	}

	/* <<XUP>> �\��l�J�E���^�N���A */
	m_gla_prd_cont[i_prot_mode] = 0;

#ifndef _ENGVALCNV_TREND
	memset(ca_TIME, (char)NULL, sizeof(ca_TIME));
	bcd7_to_asc19_time(m_gst_predata.predata[0].uca_time, (unsigned char*)ca_TIME);
	CTime cTimeStart = CTime((time_t)CSatelliteData::ub_asc_to_dtime(ca_TIME));

	SYSTEMTIME curSystime;
	GetSystemTime(&curSystime);
	CTime curCTime = curSystime;
	time_t tOffset = curCTime.GetTime() - cTimeStart.GetTime();

	m_gst_dspinf.d_AOS_time = d_aos + (double)tOffset;
	m_gst_dspinf.d_LOS_time = d_los + (double)tOffset;

//	// �O���t�\���̓s����A�b��Ō��ݎ����ɒu��������
//	time_t tPos = 10;
//	//	time_t tPos = 0;
#endif

	for (i = 0;; i++) {

		/* <<XUP>> �\��l�e�[�u���̃X�g�b�p�[�����o�����Ȃ�΁A�I�� */
		if (strcmp(m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].ca_predict_s_tx_freq, END_OF_TABLE) == 0) {
			break;
		}

		// �\��l�f�[�^���q���f�[�^�̃e���|�����ɒǉ�����
		stServerItemData data;
		memset(&data, 0, sizeof(stServerItemData));
		data.kind = 0;

		/* <<XUP>> �\��l�����̌^�ϊ��iBCD��ASCII�j */
		memset(ca_TIME, (char)NULL, sizeof(ca_TIME));
		bcd7_to_asc19_time(m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].uca_time, (unsigned char*)ca_TIME);
//		m_glla_time[i_prot_mode][m_gla_prd_cont[i_prot_mode]] = (time_t)CSatelliteData::ub_asc_to_dtime(ca_TIME);
		CTime ctime = CTime((time_t)CSatelliteData::ub_asc_to_dtime(ca_TIME));

		SYSTEMTIME systime;
		ctime.GetAsSystemTime(systime);

//#ifdef _NOCOMM
#ifdef _ENGVALCNV_TREND
#else
		// �O���t�\���̓s����A�b��Ō��ݎ����ɒu��������
//		GetSystemTime(&systime);
//		ctime = systime;
//		ctime = ctime + tPos;
//		ctime.GetAsSystemTime(systime);
//		tPos += 20;
		ctime = ctime + tOffset;
		ctime.GetAsSystemTime(systime);
#endif
		CCommBase::ConvertTime(&systime, &data.systime);

		// RNG�`�����j�^�p�ɁA�\��l�����Ɖ��������̃}�b�v��p�ӂ���
		time_t tRngTime = ctime.GetTime();
		if (m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].d_predict_2way_rng >= 0.0)
		{
			m_2WayRngMap[tRngTime] = m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].d_predict_2way_rng;
		}
		else
		{
			m_2WayRngMap[tRngTime] = m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].d_predict_uplink_range + m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].d_predict_downlink_range;
		}

		/*
		printf( "�\��l���� : [%02x%02x%02x%02x%02x%02x%02x] [%s] [%ld]\n"
		, m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].uca_time[0]
		, m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].uca_time[1]
		, m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].uca_time[2]
		, m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].uca_time[3]
		, m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].uca_time[4]
		, m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].uca_time[5]
		, m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].uca_time[6]
		, ca_TIME, gla_time[i_prot_mode][m_gla_prd_cont[i_prot_mode]] );
		*/

		/* �_�E�������N�����W�́A���̎����Ɏ�M����M���̉���������1/2�Ȃ̂ŁA���̂܂܎g���Ă悢�B*/
#ifdef NO_USE
		double	d_rtt;					/* �q�s�s�i�b�j                   */
		double	d_downrng;				/* �_�E�������N�����W�i�����j     */
		double	d_downrr;				/* �_�E�������N�����W���[�g�i���^�r�j*/
		double	d_data;
		/* <<XUP>> �q�s�s���l�������_�E�������N�����W�𐶐����� */
		d_data = (double)m_glla_time[i_prot_mode][m_gla_prd_cont[i_prot_mode]];
		d_rtt = lb_rttcal(&m_gst_predata, d_data, RTT_BASE_DOWN, &d_downrng, "+00:00", (double)0.0);

		/* <<XUP>> �q�s�s���l�������_�E�������N�����W���[�g�𐶐����� */
		d_rtt = lb_rttcal_rr(&m_gst_predata, d_data, RTT_BASE_DOWN, &d_downrr, "+00:00", (double)0.0);

		m_gda_dwRNG[i_prot_mode][m_gla_prd_cont[i_prot_mode]] = d_downrng / (double)2.0;
		m_gda_dwRR[i_prot_mode][m_gla_prd_cont[i_prot_mode]] = d_downrr;
#endif
#if(0)  /* �_�E�������N�����W -> 2way�����W��1/2 */
		m_gda_dwRNG[i_prot_mode][m_gla_prd_cont[i_prot_mode]] = m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].d_predict_downlink_range;
#else
		if (m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].d_predict_2way_rng < (double)0.0){
//			m_gda_dwRNG[i_prot_mode][m_gla_prd_cont[i_prot_mode]] = m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].d_predict_downlink_range;
			data.value = m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].d_predict_downlink_range;
			strcpy_s((char*)data.name, _MAX_NAME, "RNG(PRED)");
		}
		else {
//			m_gda_dwRNG[i_prot_mode][m_gla_prd_cont[i_prot_mode]] = m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].d_predict_2way_rng / (double)2.0;
			data.value = m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].d_predict_2way_rng / (double)2.0;
			strcpy_s((char*)data.name, _MAX_NAME, "RNG(PRED)");
		}
#endif

		// RNG�\��l�f�[�^����q���f�[�^����
		(&theApp.GetDataCollection())->CreateForecastData(&data, nStation);

// test >>>
//		strcpy_s((char*)data.name, _MAX_NAME, "UDSC642.RNG(PRED)");
//		(&theApp.GetDataCollection())->CreateForecastData(&data);
//		strcpy_s((char*)data.name, _MAX_NAME, "USC34.PRED");
//		(&theApp.GetDataCollection())->CreateForecastData(&data);
//		strcpy_s((char*)data.name, _MAX_NAME, "USC20.PRED");
//		(&theApp.GetDataCollection())->CreateForecastData(&data);
// test <<<


		if (m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].d_predict_2way_rng < (double)0.0){
//			m_gda_dwRR[i_prot_mode][m_gla_prd_cont[i_prot_mode]] = m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].d_predict_downlink_range_rate;
			data.value = m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].d_predict_downlink_range_rate;
			strcpy_s((char*)data.name, _MAX_NAME, "RR(PRED)");
		}
		else {
//			m_gda_dwRR[i_prot_mode][m_gla_prd_cont[i_prot_mode]] = m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].d_predict_2way_range_rate / (double)2.0;
			data.value = m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].d_predict_2way_range_rate / (double)2.0;
			strcpy_s((char*)data.name, _MAX_NAME, "RR(PRED)");
		}

		// RR�\��l�f�[�^���q���f�[�^����
		(&theApp.GetDataCollection())->CreateForecastData(&data, nStation);

// test >>>
//		strcpy_s((char*)data.name, _MAX_NAME, "UDSC642.RR(PRED)");
//		(&theApp.GetDataCollection())->CreateForecastData(&data);
//		strcpy_s((char*)data.name, _MAX_NAME, "USC34.RR(PRED)");
//		(&theApp.GetDataCollection())->CreateForecastData(&data);
//		strcpy_s((char*)data.name, _MAX_NAME, "USC20.RR(PRED)");
//		(&theApp.GetDataCollection())->CreateForecastData(&data);
// test <<<


		// EL�\��l�f�[�^���q���f�[�^����
		data.value = m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].d_predict_el;
		strcpy_s((char*)data.name, _MAX_NAME, "EL(PRED)");
		(&theApp.GetDataCollection())->CreateForecastData(&data, nStation);

		// EL�\��l���Ԍv�Z�O�Ƀ��X�g�Ɋi�[���Ă���
		if (bPredDiffEna)
		{
			__int64 ELPredDataSize = ELPredData.value.size();
			if (ELPredDataSize == 0 ||
				ELPredDataSize > 0 && CTime(systime).GetTime() > CTime(ELPredData.value[ELPredDataSize - 1].systime).GetTime())	// �������ُ�ȗ\��l�Ή�
			{
				stSpaceData sData;
				memset(&sData, 0, sizeof(stSpaceData));
				sData.systime = systime;
				sData.value = data.value;
				sData.ppPair.max = sData.value;
				sData.ppPair.min = sData.value;
				sData.ppPair.count = 1;
				ELPredData.value.push_back(sData);
			}
			else
			{
				CString str;
				str.Format(_T("%s(%d) :: yohochi file's datetime(%I64d) error(%s)"), __FILEW__, __LINE__, CTime(systime).GetTime(), (LPCTSTR)strFilePath);
				CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("gryohouti2"), _T("Error"), str, nLogEx::error);
			}
		}

// test >>>
//		strcpy_s((char*)data.name, _MAX_NAME, "UDSC642.EL(PRED)");
//		(&theApp.GetDataCollection())->CreateForecastData(&data);
//		strcpy_s((char*)data.name, _MAX_NAME, "USC34.EL(PRED)");
//		(&theApp.GetDataCollection())->CreateForecastData(&data);
//		strcpy_s((char*)data.name, _MAX_NAME, "USC20.EL(PRED)");
//		(&theApp.GetDataCollection())->CreateForecastData(&data);
// test <<<


		// �X�J�C���C���f�[�^���q���f�[�^����
		l_az = (long)m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].d_predict_az;
		if ( l_az >= 0 && l_az < 360 ) {
			if (pRadarData->m_queAzElSkyline.size() > l_az)
			{
				data.value = pRadarData->m_queAzElSkyline[l_az].d_EL;
			}
			else
			{
				data.value = 0.0;
			}
		}
		else {
			data.value = 0.0;
		}
		strcpy_s((char*)data.name, _MAX_NAME, "SKYLINE");
		(&theApp.GetDataCollection())->CreateForecastData(&data, nStation);

// test >>>
//		strcpy_s((char*)data.name, _MAX_NAME, "UDSC642.SKYLINE");
//		(&theApp.GetDataCollection())->CreateForecastData(&data);
//		strcpy_s((char*)data.name, _MAX_NAME, "USC34.SKYLINE");
//		(&theApp.GetDataCollection())->CreateForecastData(&data);
//		strcpy_s((char*)data.name, _MAX_NAME, "USC20.SKYLINE");
//		(&theApp.GetDataCollection())->CreateForecastData(&data);
// test <<<

		// RF�g���b�v�f�[�^���q���f�[�^����
		if ( l_az >= 0 && l_az < 360 ) {
//#if 0
//			if (pRadarData->m_queAzElRftrip.size() > l_az)
//			{
//				data.value = pRadarData->m_queAzElRftrip[l_az].d_EL;
//			}
//#else
			vector<double> listAzEl;
			if (theApp.GetSatelliteData().GetRFTripAngleData(nStation).GetAzEl(listAzEl))
			{
				data.value = listAzEl[l_az];
			}
//#endif
			else
			{
				data.value = 0.0;
			}
		}
		else {
			data.value = 0.0;
		}
		strcpy_s((char*)data.name, _MAX_NAME, "RF-TRIP");
		(&theApp.GetDataCollection())->CreateForecastData(&data, nStation);

// test >>>
//		strcpy_s((char*)data.name, _MAX_NAME, "UDSC642.RF-TRIP");
//		(&theApp.GetDataCollection())->CreateForecastData(&data);
//		strcpy_s((char*)data.name, _MAX_NAME, "USC34.RF-TRIP");
//		(&theApp.GetDataCollection())->CreateForecastData(&data);
//		strcpy_s((char*)data.name, _MAX_NAME, "USC20.RF-TRIP");
//		(&theApp.GetDataCollection())->CreateForecastData(&data);
// test <<<

		m_gla_prd_cont[i_prot_mode]++;

		/* <<2013-TURBO>> 1�b�X�V24���ԗ\��l�ɑΉ����AEOT���Ȃ��ꍇ�̏I�������ǉ� */
		if (m_gla_prd_cont[i_prot_mode] == MAX_PRED_SIZE) {
			break;
		}
	}

	// EL�\��l��Ԍv�Z
	if (bPredDiffEna)
	{
		vector<stPredData>& ELlist = theApp.GetDataCollection().GetELPredInterpolateList(nStation);
		ELlist.clear();
		if (ELPredData.value.size() > 0)
		{
			//		map<time_t, double>& ELMap = theApp.GetDataCollection().GetELPredInterpolateMap(nStation);

			// AOS����LOS�Ԃ̃f�[�^���
			__int64 idx = 0;
			for (double t = d_aos; t <= d_los; t += 1.0)
			{
				//			ELMap[(time_t)t] = InterpolatePrd(t, &idx, &ELPredData);
				stPredData predata;
				predata.t = t;
				predata.data = InterpolatePrd(t, &idx, &ELPredData);
				ELlist.push_back(predata);
//				TRACE(_T("PredELlist: t, data, %f, %f\n"), predata.t, predata.data);
			}
		}
	}

	CGraphMutex::Unlock(eRadar);

	return(0);
}


/*============================================================================*/
/*! CTrendData

-# �\��l�t�@�C����ǂݍ���

@param	szp_file_name�F�t�@�C���p�X

0:���A���f�[�^ �^ 1:�ۑ��f�[�^
@retval	����i�O�j�^�t�@�C�����e���ُ�i�|�P�j

*/
/*============================================================================*/
int CTrendData::load_pred_data(char *szp_file_name, double *d_aos, double *d_los, bool* pNewFormat/* = NULL*/)
{
	int				i_counter;			/* �J�E���^                                   */
	int				i_ret;				/* �֐��Ԓl                                   */
	int				i_tmp;				/* �������^�e���|����                         */
	FILE			*p_fp;				/* �t�@�C���|�C���^                           */
	unsigned char	sz_tmp[512];		/* �\��l�t�@�C���ǂݔ�΂����R�[�h�p�o�b�t�@ */
	unsigned char	sz_read_buff[512];	/* �\��l�t�@�C���ǂݍ��ݗp�o�b�t�@ 1997-03-13*/
	unsigned char	sz_read_tmp01[32];	/* �\��l�t�@�C�������p�o�b�t�@     1997-03-13*/
	unsigned char	sz_read_tmp03[32] = { 0 };	/* �\��l�t�@�C�������p�o�b�t�@     1997-03-13*/
	unsigned char	sz_read_tmp04[32] = { 0 };	/* �\��l�t�@�C�������p�o�b�t�@     1997-03-13*/
	unsigned char	sz_read_tmp05[32] = { 0 };	/* �\��l�t�@�C�������p�o�b�t�@     1997-03-13*/
	unsigned char	sz_read_tmp06[32] = { 0 };	/* �\��l�t�@�C�������p�o�b�t�@     1997-03-13*/
	unsigned char	sz_read_tmp07[32] = { 0 };	/* �\��l�t�@�C�������p�o�b�t�@     1997-03-13*/
	unsigned char	sz_read_tmp08[32] = { 0 };	/* �\��l�t�@�C�������p�o�b�t�@     1997-03-13*/
	unsigned char	sz_read_tmp09[32] = { 0 };	/* �\��l�t�@�C�������p�o�b�t�@     1997-03-13*/
	unsigned char	sz_read_tmp10[32] = { 0 };	/* �\��l�t�@�C�������p�o�b�t�@     1997-03-13*/
	unsigned char	sz_read_tmp11[32];	/* �\��l�t�@�C�������p�o�b�t�@     1997-03-13*/

	char			sz_work[256];
	char			sz_name[40];
	char			sz_passno[40];
	char			sz_time[40];
	char			sz_asctime[40];

	double			d_az;
	double			d_el;
	double			d_uprng;

	long			l_rc;

	/* �\��l�e�[�u���N���A */
	memset((unsigned char *)m_gst_predata.predata, (char)NULL, sizeof(one_predata_t)*MAX_PRED_SIZE);

	/* �\��l�t�@�C���I�[�v�� */
	/*------------------*/
	/* �t�@�C���n�o�d�m */
	/*------------------*/
	if (_wfopen_s(&p_fp, CString(szp_file_name), _T("r")) != 0 || p_fp == NULL) {
		CString strTitle;
		CString strMsg;
		CString strKind;
		if (strTitle.LoadString(IDS_FILE_READ) == FALSE)
			return (-1);
		if (strMsg.LoadString(IDS_FILE_DOSENOT_OPEN) == FALSE)
			return (-1);
		if (strKind.LoadString(IDS_FORCASTFILE_SETTING_MSG) == FALSE)
			return (-1);

		/* �\��l�t�@�C���I�[�v���ُ� */
		CString str;
		str.Format(_T("%s(%d) :: Could not open yohochi file(%s)"), __FILEW__, __LINE__, (LPCTSTR)CString(szp_file_name));
		CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("load_pred_data"), _T("debug"), str, nLogEx::debug);
		return -1;
	}

#ifdef _DEBUG
	{
		CString str;
		str.Format(_T("%s(%d) :: Pred file open(%s)"), __FILEW__, __LINE__, (LPCTSTR)CString(szp_file_name));
		CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("load_pred_data"), _T("debug"), str, nLogEx::debug);
	}
#endif

	/* �t�@�C���Ǘ����ǂݔ�΂� */
	if (fgets((char *)sz_tmp, sizeof(sz_tmp), p_fp) == NULL){
		CString str;
		str.Format(_T("%s(%d) ::  Could not read yohochi file(%s)"), __FILEW__, __LINE__, (LPCTSTR)CString(szp_file_name));
		CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("load_pred_data"), _T("Error"), str, nLogEx::error);
		fclose(p_fp);
		return -1;
	}

	/* �p�X��񕔓ǂݔ�΂� */
	if (fgets((char *)sz_tmp, sizeof(sz_tmp), p_fp) == NULL){
		CString str;
		str.Format(_T("%s(%d) ::  Could not read yohochi file(%s) pass info"), __FILEW__, __LINE__, (LPCTSTR)CString(szp_file_name));
		CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("load_pred_data"), _T("Error"), str, nLogEx::error);
		fclose(p_fp);
		return -1;
	}

	/* �`�n�r��񕔓ǂݔ�΂� */
	if (fgets((char *)sz_tmp, sizeof(sz_tmp), p_fp) == NULL){
		CString str;
		str.Format(_T("%s(%d) ::  Could not read yohochi file(%s) AOS info"), __FILEW__, __LINE__, (LPCTSTR)CString(szp_file_name));
		CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("load_pred_data"), _T("Error"), str, nLogEx::error);
		fclose(p_fp);
		return -1;
	}
	else {
		l_rc = sscanf_s((char *)sz_tmp, "%s%s%s%lf%lf%lf%s",
			sz_name, 40, sz_passno, 40, sz_time, 40, &d_az, &d_el, &d_uprng, sz_work, 256);

		CSatelliteData::ub_asc_to_asctime(sz_time, sz_asctime);
		*d_aos = CSatelliteData::ub_asc_to_dtime(sz_asctime);
	}

	/* �k�n�r��񕔓ǂݔ�΂� */
	if (fgets((char *)sz_tmp, sizeof(sz_tmp), p_fp) == NULL){
		CString str;
		str.Format(_T("%s(%d) ::  Could not read yohochi file(%s) LOS info"), __FILEW__, __LINE__, (LPCTSTR)CString(szp_file_name));
		CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("load_pred_data"), _T("Error"), str, nLogEx::error);
		fclose(p_fp);
		return -1;
	}
	else {
		l_rc = sscanf_s((char *)sz_tmp, "%s%s%s%lf%lf%lf%s",
			sz_name, 40, sz_passno, 40, sz_time, 40, &d_az, &d_el, &d_uprng, sz_work, 256);

		CSatelliteData::ub_asc_to_asctime(sz_time, sz_asctime);
		*d_los = CSatelliteData::ub_asc_to_dtime(sz_asctime);
	}

	/* �l�`�w��񕔓ǂݔ�΂� */
	if (fgets((char *)sz_tmp, sizeof(sz_tmp), p_fp) == NULL){
		CString str;
		str.Format(_T("%s(%d) ::  Could not read yohochi file(%s) MAX info"), __FILEW__, __LINE__, (LPCTSTR)CString(szp_file_name));
		CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("load_pred_data"), _T("Error"), str, nLogEx::error);
		fclose(p_fp);
		return -1;
	}

	/* �J�E���^�N���A */
	i_counter = 0;

	/* �\��l�t�@�C���ǂݍ��� */
	while (1){
		if (fgets((char *)sz_read_buff, 512, p_fp) == NULL){
			if (ferror(p_fp)){
				CString str;
				str.Format(_T("%s(%d) ::  Could not read yohochi file(%s)"), __FILEW__, __LINE__, (LPCTSTR)CString(szp_file_name));
				CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("load_pred_data"), _T("Error"), str, nLogEx::error);
				continue;
			}
			if (feof(p_fp)){
				CString str;
				str.Format(_T("%s(%d) ::  Could not read yohochi file(%s) EOF"), __FILEW__, __LINE__, (LPCTSTR)CString(szp_file_name));
				CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("load_pred_data"), _T("debug"), str, nLogEx::debug);
				/* �t�@�C���ǂݍ��ݏI���i�d�n�e�j */
				break;
			}
		}

		/* ���ڕ��� */
		i_ret = sscanf_s((char *)sz_read_buff, "%s %s %s %s %s %s %s %s %s %s",
			sz_read_tmp01, 32, sz_read_tmp03, 32, sz_read_tmp04, 32, sz_read_tmp05, 32,
			sz_read_tmp06, 32, sz_read_tmp07, 32, sz_read_tmp08, 32,
			sz_read_tmp09, 32, sz_read_tmp10, 32, sz_read_tmp11, 32);	/* <<2013-TURBO>> �[�F���p�V�t�H�[�}�b�g�ɑΉ� */

		CSatelliteData::ub_asc_to_asctime((char*)sz_read_tmp01, (char*)sz_tmp);

		/* (YYYY-MM-DD HH:MM:DD)���V�F�A�[�h�������֎����i�a�b�c�FYYYYMMDDHHMMSS00�j�ݒ� */
		i_tmp = 10 + 1 + 8;
		asc19_to_bcd7_time(sz_tmp, m_gst_predata.predata[i_counter].uca_time);
		m_gst_predata.predata[i_counter].uca_time[7] = 0x00;

		/* (999.9999)���V�F�A�[�h�������ւ`���ifloat�j�ݒ� */
		m_gst_predata.predata[i_counter].d_predict_az = atof((char *)sz_read_tmp03);

		/* ( 99.9999)���V�F�A�[�h�������ւd���ifloat�j�ݒ� */
		m_gst_predata.predata[i_counter].d_predict_el = atof((char *)sz_read_tmp04);

		/* (9999999999999.999)���V�F�A�[�h�������փA�b�v�����N�����W�idouble�j�ݒ� */
		m_gst_predata.predata[i_counter].d_predict_uplink_range = atof((char *)sz_read_tmp05);

		/* (9999999999999.999)���V�F�A�[�h�������փ_�E�������N�����W�idouble�j�ݒ� */
		m_gst_predata.predata[i_counter].d_predict_downlink_range = atof((char *)sz_read_tmp07);

		/* (999999.999)���V�F�A�[�h�������փA�b�v�����N�����W���[�g�ifloat�j�ݒ� */
		m_gst_predata.predata[i_counter].d_predict_uplink_range_rate = atof((char *)sz_read_tmp06);

		/* (999999.999)���V�F�A�[�h�������փ_�E�������N�����W���[�g�ifloat�j�ݒ� */
		m_gst_predata.predata[i_counter].d_predict_downlink_range_rate = atof((char *)sz_read_tmp08);

		/* �V�F�A�[�h�������֗\�����M���g��(9999999999.99999)�ݒ� */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_s_tx_freq, PRED_FREQ_LEN, "0.0");

		/* �V�F�A�[�h�������ւr�тP�v�`�x�\����M���g��(9999999999.99999)�ݒ� */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_s_rx_1way_freq, PRED_FREQ_LEN, "0.0");

		/* �V�F�A�[�h�������ւw�тP�v�`�x�\����M���g��(9999999999.99999)�ݒ� */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_x_rx_1way_freq, PRED_FREQ_LEN, "0.0");

		/* �V�F�A�[�h�������ւr�тQ�v�`�x�\����M���g��(9999999999.99999)�ݒ� */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_s_rx_2way_freq, PRED_FREQ_LEN, "0.0");

		/* �V�F�A�[�h�������ւw�тQ�v�`�x�\����M���g��(9999999999.99999)�ݒ� */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_x_rx_2way_freq, PRED_FREQ_LEN, "0.0");

		/* (lg�^)���V�F�A�[�h�������ւr�тP�v�`�x�\�������v���[�g(999999.99999)�ݒ� */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_s_rx_1way_ramprate, PRED_RAMP_LEN, "0.0");

		/* (lg�^)���V�F�A�[�h�������ւw�тP�v�`�x�\�������v���[�g(999999.99999)�ݒ� */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_x_rx_1way_ramprate, PRED_RAMP_LEN, "0.0");

		/* (lg�^)���V�F�A�[�h�������ւr�тQ�v�`�x�\�������v���[�g(999999.99999)�ݒ� */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_s_rx_2way_ramprate, PRED_RAMP_LEN, "0.0");

		/* (lg�^)���V�F�A�[�h�������ւw�тQ�v�`�x�\�������v���[�g(999999.99999)�ݒ� */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_x_rx_2way_ramprate, PRED_RAMP_LEN, "0.0");

		/* <<2013-TURBO>> �[�F���p�V�t�H�[�}�b�g�ɑΉ� */
		if (i_ret == 10) {		/* �V�t�H�[�}�b�g */

			/* �\���Q�v�`�x�����W���V�F�A�[�h�������ցidouble�j�ݒ� */
			m_gst_predata.predata[i_counter].d_predict_2way_rng = atof((char *)sz_read_tmp09);

			/* �\���Q�v�`�x�����W���[�g���V�F�A�[�h�������ցidouble�j�ݒ� */
			m_gst_predata.predata[i_counter].d_predict_2way_range_rate = atof((char *)sz_read_tmp10);

			/* �\���R�v�`�x�����W���[�g���V�F�A�[�h�������ցidouble�j�ݒ� */
			m_gst_predata.predata[i_counter].d_predict_3way_range_rate = atof((char *)sz_read_tmp11);

			if (pNewFormat != NULL)
				*pNewFormat = 1;
		}
		else {					/* �]���t�H�[�}�b�g */

			/* �\���Q�v�`�x�����W�Ƀ_�~�[�f�[�^���Z�b�g */
			m_gst_predata.predata[i_counter].d_predict_2way_rng = (double)-1.0;

			/* �\���Q�v�`�x�����W���[�g�Ƀ_�~�[�f�[�^���Z�b�g */
			m_gst_predata.predata[i_counter].d_predict_2way_range_rate = (double)-1.0;

			/* �\���R�v�`�x�����W���[�g�Ƀ_�~�[�f�[�^���Z�b�g */
			m_gst_predata.predata[i_counter].d_predict_3way_range_rate = (double)-1.0;

			if (pNewFormat != NULL)
				*pNewFormat = 0;
		}

		/* �J�E���^�t�o */
		i_counter++;
	}

	m_PredItemCount = i_counter;

	if (i_counter < (MAX_PRED_SIZE - 1)){

		/* �\��l�e�[�u���ɃX�g�b�p�ݒ� */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_s_tx_freq, PRED_FREQ_LEN, END_OF_TABLE);
	}

	if (fclose(p_fp) == EOF){
		CString str;
		str.Format(_T("%s(%d) ::  Could not close yohochi file(%s)"), __FILEW__, __LINE__, (LPCTSTR)CString(szp_file_name));
		CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("load_pred_data"), _T("Error"), str, nLogEx::error);
	}

	return 0;
}


/*============================================================================*/
/*! CTrendData

-# �������s�w�s(YYYY - MM - DD HH : MM : SS)����c�b�a(YYYYMMDDHHMMSS)�֕ϊ�����

@param	psz_asc19_time�F�ϊ����s�w�s����(YYYY-MM-DD HH:MM:SS)�擪�A�h���X
@param	psz_bcd7_time�F�ϊ���a�b�c����(YYYYMMDDHHMMSS)�擪�A�h���X
@retval	�Ȃ�

*/
/*============================================================================*/
void	CTrendData::asc19_to_bcd7_time(unsigned char *psz_asc19_time, unsigned char *psz_bcd7_time)
{
	/* �`�r�b�h�h�̏�ʂx�x���a�b�c�̂x�x�֕ϊ� */
	*(psz_bcd7_time + 0) = (*(psz_asc19_time + 0) & 0x0f) << 4;
	*(psz_bcd7_time + 0) = *(psz_bcd7_time + 0) | (*(psz_asc19_time + 1) & 0x0f);

	/* �`�r�b�h�h�̉��ʂx�x���a�b�c�̂x�x�֕ϊ� */
	*(psz_bcd7_time + 1) = (*(psz_asc19_time + 2) & 0x0f) << 4;
	*(psz_bcd7_time + 1) = *(psz_bcd7_time + 1) | (*(psz_asc19_time + 3) & 0x0f);

	/* �`�r�b�h�h�̂l�l���a�b�c�̂l�l�֕ϊ� */
	*(psz_bcd7_time + 2) = (*(psz_asc19_time + 5) & 0x0f) << 4;
	*(psz_bcd7_time + 2) = *(psz_bcd7_time + 2) | (*(psz_asc19_time + 6) & 0x0f);

	/* �`�r�b�h�h�̂c�c���a�b�c�̂c�c�֕ϊ� */
	*(psz_bcd7_time + 3) = (*(psz_asc19_time + 8) & 0x0f) << 4;
	*(psz_bcd7_time + 3) = *(psz_bcd7_time + 3) | (*(psz_asc19_time + 9) & 0x0f);

	/* �`�r�b�h�h�̂g�g���a�b�c�̂g�g�֕ϊ� */
	*(psz_bcd7_time + 4) = (*(psz_asc19_time + 11) & 0x0f) << 4;
	*(psz_bcd7_time + 4) = *(psz_bcd7_time + 4) | (*(psz_asc19_time + 12) & 0x0f);

	/* �a�b�c�̂l�l���`�r�b�h�h�̂l�l�֕ϊ� */
	*(psz_bcd7_time + 5) = (*(psz_asc19_time + 14) & 0x0f) << 4;
	*(psz_bcd7_time + 5) = *(psz_bcd7_time + 5) | (*(psz_asc19_time + 15) & 0x0f);

	/* �a�b�c�̂r�r���`�r�b�h�h�̂r�r�֕ϊ� */
	*(psz_bcd7_time + 6) = (*(psz_asc19_time + 17) & 0x0f) << 4;
	*(psz_bcd7_time + 6) = *(psz_bcd7_time + 6) | (*(psz_asc19_time + 18) & 0x0f);
}


/*============================================================================*/
/*! CTrendData

-# �������c�b�a(YYYYMMDDHHMMSS)����s�w�s(YYYY - MM - DD HH : MM : SS)�֕ϊ�����

@param	psz_bcd7_time�F�ϊ����a�b�c����(YYYYMMDDHHMMSS)�擪�A�h���X
@param	psz_asc19_time�F�ϊ���s�w�s����(YYYY-MM-DD HH:MM:SS)�擪�A�h���X
@retval	�Ȃ�

*/
/*============================================================================*/
void	CTrendData::bcd7_to_asc19_time(unsigned char* psz_bcd7_time, unsigned char* psz_asc19_time)
{

	/* �a�b�c�̏�ʂx�x���`�r�b�h�h�̂x�x�֕ϊ� */
	*(psz_asc19_time + 0) = ((*(psz_bcd7_time + 0) >> 4) & 0x0f) | 0x30;
	*(psz_asc19_time + 1) = ((*(psz_bcd7_time + 0)) & 0x0f) | 0x30;

	/* �a�b�c�̉��ʂx�x���`�r�b�h�h�̂x�x�֕ϊ� */
	*(psz_asc19_time + 2) = ((*(psz_bcd7_time + 1) >> 4) & 0x0f) | 0x30;
	*(psz_asc19_time + 3) = ((*(psz_bcd7_time + 1)) & 0x0f) | 0x30;

	/* "-"�ǉ� */
	*(psz_asc19_time + 4) = 0x2d;

	/* �a�b�c�̂l�l���`�r�b�h�h�̂l�l�֕ϊ� */
	*(psz_asc19_time + 5) = ((*(psz_bcd7_time + 2) >> 4) & 0x0f) | 0x30;
	*(psz_asc19_time + 6) = ((*(psz_bcd7_time + 2)) & 0x0f) | 0x30;

	/* "-"�ǉ� */
	*(psz_asc19_time + 7) = 0x2d;

	/* �a�b�c�̂c�c���`�r�b�h�h�̂c�c�֕ϊ� */
	*(psz_asc19_time + 8) = ((*(psz_bcd7_time + 3) >> 4) & 0x0f) | 0x30;
	*(psz_asc19_time + 9) = ((*(psz_bcd7_time + 3)) & 0x0f) | 0x30;

	/* " "�ǉ� */
	*(psz_asc19_time + 10) = 0x20;

	/* �a�b�c�̂g�g���`�r�b�h�h�̂g�g�֕ϊ� */
	*(psz_asc19_time + 11) = ((*(psz_bcd7_time + 4) >> 4) & 0x0f) | 0x30;
	*(psz_asc19_time + 12) = ((*(psz_bcd7_time + 4)) & 0x0f) | 0x30;

	/* ":"�ǉ� */
	*(psz_asc19_time + 13) = 0x3a;

	/* �a�b�c�̂l�l���`�r�b�h�h�̂l�l�֕ϊ� */
	*(psz_asc19_time + 14) = ((*(psz_bcd7_time + 5) >> 4) & 0x0f) | 0x30;
	*(psz_asc19_time + 15) = ((*(psz_bcd7_time + 5)) & 0x0f) | 0x30;

	/* ":"�ǉ� */
	*(psz_asc19_time + 16) = 0x3a;

	/* �a�b�c�̂r�r���`�r�b�h�h�̂r�r�֕ϊ� */
	*(psz_asc19_time + 17) = ((*(psz_bcd7_time + 6) >> 4) & 0x0f) | 0x30;
	*(psz_asc19_time + 18) = ((*(psz_bcd7_time + 6)) & 0x0f) | 0x30;

	/* �m�t�k�k�ǉ� */
	*(psz_asc19_time + 19) = NULL;


}

//#if 0
///*============================================================================*/
///*! CTrendData
//
//-# �q�s�s�v�Z�@�\
//
//�w�莞���̗\��l���������A���������l�A�q�s�s���Z�o����B
//
//�_�E�������N�x�[�X���I�����ꂽ�ꍇ
//�P.�w�肳�ꂽ�����̃_�E�������N�����W�𒊏o����B
//�Q.���o���ꂽ�_�E�������N�����W����_�E�������N���Ԃ��Z�o����B
//�R.�_�E�������N���Ԃ��Q�{���A�A�b�v�����N�����o���ꂽ����(�ߎ��l)��
//�Z�o����B
//�S.�A�b�v�����N���o�����ł̗\��l����A�b�v�����N�����W�𒊏o����B
//�T.���o���ꂽ�A�b�v�����N�����W����A�b�v�����N���Ԃ��Z�o����B
//�U.�A�b�v�����N�^�_�E�������N�����W���牝�������l���Z�o����B
//�V.�A�b�v�����N�^�_�E�������N���Ԃ���q�s�s���Z�o����B
//<<2013-TURBO>>
//�\��l�̐V�t�H�[�}�b�g���ɔ����ADSN�t�H�[�}�b�g�̏ꍇ��2-WAY RNG��
//�p����RTT�����߂�B
//
//�A�b�v�����N�x�[�X���I�����ꂽ�ꍇ
//�P.�w�肳�ꂽ�����̃A�b�v�����N�����W�𒊏o����B
//�Q.���o���ꂽ�A�b�v�����N�����W����A�b�v�����N���Ԃ��Z�o����B
//�R.�A�b�v�����N���Ԃ��Q�{���A�_�E�������N���A�����鎞��(�ߎ��l)��
//�Z�o����B
//�S.�_�E�������N�A�������ł̗\��l����_�E�������N�����W�𒊏o����B
//�T.���o���ꂽ�_�E�������N�����W����_�E�������N���Ԃ��Z�o����B
//�U.�A�b�v�����N�^�_�E�������N�����W���牝�������l���Z�o����B
//�V.�A�b�v�����N�^�_�E�������N���Ԃ���q�s�s���Z�o����B
//<<2013-TURBO>>
//�\��l�t�H�[�}�b�g�̉��߂�����Ă���A�A�b�v�����N�����W�͓��Y����
//�ɑ��M�����M�����q���Ő܂�Ԃ�A�n��ɓ��B����܂ł̋����̂P�^�Q��
//���邱�Ƃ��������̂ŁA�A�b�v�����N�����W���Q�{���ċ��߂邱�ƂƂ���B
//
//���l     : XUP�Ή�
//
//@param *stp_predict;      I : �W�J�����\��l�e�[�u���̃|�C���^
//@param d_pred_t;          I : ���߂�\��l�̎����i�ώZ�b�j
//@param i_cal_flag;        I : RTT�v�Z���@�w��t���O
//RTT_BASE_UP   = �A�b�v�����N�x�[�X
//RTT_BASE_DOWN = �_�E�������N�x�[�X
//@param *dp_range;       �@O :�����`�d�����i�����j
//@param *psz_offset_time;  I :�ݔ��Ď��e�[�u�������I�t�Z�b�g�l�A�h���X
//@param d_offset_freq;     I :���g���I�t�Z�b�g�l
//@retval	�P�ȏ�F�q�s�s���ԁi�b�j
//�O�@�@�F�q�s�s���P�����̏ꍇ�A�܂��̓G���[�̏ꍇ
//*/
///*============================================================================*/
///* �f�o�b�O�p�}�N���錾 */
///* #define		DEBUG1		/* �G���[�\�� */
///* #define		DEBUG2		/* �������[�g */
///* #define		DEBUG3		/* �ڍ׃f�[�^ */
//double CTrendData::lb_rttcal(predata_t* stp_predict, double d_pred_t, int i_cal_flag, double* dp_range, char* psz_offset_time, double d_offset_freq)
//{
//
//	int         i_ret;       /* �֐��߂�l�o�b�t�@                   */
//	double      d_rng_up;       /* �A�b�v�����N�����W�i�����j           */
//	double      d_rng_down;       /* �_�E�������N�����W�i�����j           */
//	double      d_rng;       /* ���������l�i�����j                   */
//	double      d_rtt_up;       /* �A�b�v�����N���ԁi�b�j               */
//	double      d_rtt_down;       /* �_�E�������N���ԁi�b�j               */
//	double      d_rtt;       /* �q�s�s���ԁi�b�j                     */
//	double      d_c;       /* �����i�j���^�r�j                     */
//	one_predata_t   st_predict;       /* �\��l���                           */
//
//
//	d_c = V_C / 1000;                            /* �����i�j���^�r�j */
//
//	/* �w�莞���̗\��l��\��l�e�[�u�����猟�� */
//	i_ret = lb_prdcal(&st_predict, stp_predict, d_pred_t, 1, psz_offset_time, d_offset_freq);
//
//	if (i_ret != -1) {                         /* ���������ꍇ */
//
//		switch (i_cal_flag) {
//
//		case RTT_BASE_UP:                   /* �A�b�v�����N�x�[�X */
//#ifdef NO_USE	/* <<2013-TURBO>> */
//			/* �A�b�v�����N�����W�擾 */
//			d_rng_up = st_predict.d_predict_uplink_range;
//			d_rtt_up = d_rng_up / d_c;         /* �A�b�v�����N���ԎZ�o */
//
//			/* �_�E�������N�A�������̗\��l��\��l�e�[�u�����猟�� */
//			i_ret = lb_prdcal(&st_predict, stp_predict, d_pred_t + (d_rtt_up * 2), 1, psz_offset_time, d_offset_freq);
//			if (i_ret == -1) {                 /* ���o�G���[ */
//				d_rng = 0.0;
//				d_rtt = 0.0;
//			}
//			else {                              /* �_�E�������N�����W�擾 */
//				d_rng_down = st_predict.d_predict_downlink_range;
//				d_rtt_down = d_rng_down / d_c; /* �_�E�������N���ԎZ�o */
//
//				d_rng = d_rng_up + d_rng_down; /* �q�s�s�Z�o */
//				d_rtt = d_rtt_up + d_rtt_down;
//			}
//#endif
//
//			/* <<2013-TURBO>> �v�Z���@�ύX */
//			d_rng = st_predict.d_predict_uplink_range * (double)2.0;
//			d_rtt = d_rng / d_c;
//
//			break;
//
//		case RTT_BASE_DOWN:                   /* �_�E�������N�x�[�X */
//
//			/* <<2013-TURBO>> ���t�H�[�}�b�g�̏ꍇ�͏]���ʂ� */
//			if (st_predict.d_predict_2way_rng < (double)0.0) {
//
//				/* �_�E�������N�����W�擾 */
//				d_rng_down = st_predict.d_predict_downlink_range;
//				d_rtt_down = d_rng_down / d_c;     /* �_�E�������N���ԎZ�o */
//
//				/* �A�b�v�����N���M�����̗\��l��\��l�e�[�u�����猟�� */
//				i_ret = lb_prdcal(&st_predict, stp_predict, d_pred_t - (d_rtt_down * 2), 1, psz_offset_time, d_offset_freq);
//				if (i_ret == -1) {                 /* ���o�G���[ */
//					d_rng = 0.0;
//					d_rtt = 0.0;
//				}
//				else {                              /* �A�b�v�����N�����W�擾 */
//					d_rng_up = st_predict.d_predict_uplink_range;
//					d_rtt_up = d_rng_up / d_c;     /* �A�b�v�����N���ԎZ�o */
//
//					d_rng = d_rng_up + d_rng_down; /* �q�s�s�Z�o */
//					d_rtt = d_rtt_up + d_rtt_down;
//				}
//			}
//			else {	/* <<2013-TURBO>> �V�t�H�[�}�b�g�̏ꍇ�͂Q�|�v�`�x�����W���g�p���� */
//				d_rng = st_predict.d_predict_2way_rng;
//				d_rtt = d_rng / d_c;
//			}
//
//			break;
//		}
//	}
//	else {
//		d_rng = 0.0;
//		d_rtt = 0.0;
//	}
//
//	*dp_range = d_rng;                         /* �����ݒ�i�����`�d�����j */
//
//	return(d_rtt);
//}
//
//
///*============================================================================*/
///*! CTrendData
//
//-# �q�s�s�v�Z�@�\�i�q�q�o�̓o�[�W�����j
//
//�w�莞���̗\��l���������A���������l�A�q�s�s���Z�o����B
//
//�_�E�������N�x�[�X���I�����ꂽ�ꍇ
//�P.�w�肳�ꂽ�����̃_�E�������N�����W�𒊏o����B
//�Q.���o���ꂽ�_�E�������N�����W����_�E�������N���Ԃ��Z�o����B
//�R.�_�E�������N���Ԃ��Q�{���A�A�b�v�����N�����o���ꂽ����(�ߎ��l)��
//�Z�o����B
//�S.�A�b�v�����N���o�����ł̗\��l����A�b�v�����N�����W�𒊏o����B
//�T.���o���ꂽ�A�b�v�����N�����W����A�b�v�����N���Ԃ��Z�o����B
//�U.�A�b�v�����N�^�_�E�������N�����W���牝�������l���Z�o����B
//�V.�A�b�v�����N�^�_�E�������N���Ԃ���q�s�s���Z�o����B
//<<2013-TURBO>>
//�\��l�̐V�t�H�[�}�b�g���ɔ����ADSN�t�H�[�}�b�g�̏ꍇ��2-WAY RNG��
//�p����RTT�����߂�B
//
//�A�b�v�����N�x�[�X���I�����ꂽ�ꍇ
//�P.�w�肳�ꂽ�����̃A�b�v�����N�����W�𒊏o����B
//�Q.���o���ꂽ�A�b�v�����N�����W����A�b�v�����N���Ԃ��Z�o����B
//�R.�A�b�v�����N���Ԃ��Q�{���A�_�E�������N���A�����鎞��(�ߎ��l)��
//�Z�o����B
//�S.�_�E�������N�A�������ł̗\��l����_�E�������N�����W�𒊏o����B
//�T.���o���ꂽ�_�E�������N�����W����_�E�������N���Ԃ��Z�o����B
//�U.�A�b�v�����N�^�_�E�������N�����W���牝�������l���Z�o����B
//�V.�A�b�v�����N�^�_�E�������N���Ԃ���q�s�s���Z�o����B
//<<2013-TURBO>>
//�\��l�t�H�[�}�b�g�̉��߂�����Ă���A�A�b�v�����N�����W�͓��Y����
//�ɑ��M�����M�����q���Ő܂�Ԃ�A�n��ɓ��B����܂ł̋����̂P�^�Q��
//���邱�Ƃ��������̂ŁA�A�b�v�����N�����W���Q�{���ċ��߂邱�ƂƂ���B
//
//���l     : XUP�Ή�
//
//@param *stp_predict;      I : �W�J�����\��l�e�[�u���̃|�C���^
//@param d_pred_t;          I : ���߂�\��l�̎����i�ώZ�b�j
//@param i_cal_flag;        I : RTT�v�Z���@�w��t���O
//RTT_BASE_UP   = �A�b�v�����N�x�[�X
//RTT_BASE_DOWN = �_�E�������N�x�[�X
//@param *dp_range_rate;    O : �����`�d�����ω����i���^���j
//@param *psz_offset_time;  I :�����I�t�Z�b�g�l�i�[�ʒu�|�C���^
//@param d_offset_freq;     I :���g���I�t�Z�b�g�l
//@retval	�P�ȏ�F�q�s�s���ԁi�b�j
//�O�@�@�F�q�s�s���P�����̏ꍇ�A�܂��̓G���[�̏ꍇ
//*/
///*============================================================================*/
///* �f�o�b�O�p�}�N���錾 */
///* #define		DEBUG1		/* �G���[�\�� */
///* #define		DEBUG2		/* �������[�g */
///* #define		DEBUG3		/* �ڍ׃f�[�^ */
//double CTrendData::lb_rttcal_rr(predata_t* stp_predict, double d_pred_t, int i_cal_flag, double* dp_range_rate, char* psz_offset_time, double d_offset_freq)
//
//
//{
//
//	int         i_ret;			/* �֐��߂�l�o�b�t�@                   */
//	double      d_rng_up;		/* �A�b�v�����N�����W�i�����j           */
//	double      d_rng_down;		/* �_�E�������N�����W�i�����j           */
//	double      d_rr_up;		/* �A�b�v�����N�����W���[�g�i���^���j   */
//	double      d_rr_down;		/* �_�E�������N�����W���[�g�i���^���j   */
//	double      d_rr;			/* ���������l�ω����i���^���j           */
//	double      d_rtt_up;		/* �A�b�v�����N���ԁi�b�j               */
//	double      d_rtt_down;		/* �_�E�������N���ԁi�b�j               */
//	double      d_rtt;			/* �q�s�s���ԁi�b�j                     */
//	double      d_c;			/* �����i�j���^�r�j                     */
//	one_predata_t   st_predict; /* �\��l���                           */
//
//	d_c = V_C / 1000;                            /* �����i�j���^�r�j */
//
//	/* �w�莞���̗\��l��\��l�e�[�u�����猟�� */
//	i_ret = lb_prdcal(&st_predict, stp_predict, d_pred_t, 1, psz_offset_time, d_offset_freq);
//
//	if (i_ret != -1) {                         /* ���������ꍇ */
//
//		switch (i_cal_flag) {
//
//		case RTT_BASE_UP:                   /* �A�b�v�����N�x�[�X */
//			/* �A�b�v�����N�����W�擾 */
//			d_rng_up = st_predict.d_predict_uplink_range;
//			d_rr_up = st_predict.d_predict_uplink_range_rate;
//			d_rtt_up = d_rng_up / d_c;         /* �A�b�v�����N���ԎZ�o */
//
//			/* �_�E�������N�A�������̗\��l��\��l�e�[�u�����猟�� */
//			i_ret = lb_prdcal(&st_predict, stp_predict, d_pred_t + (d_rtt_up * 2.0), 1, psz_offset_time, d_offset_freq);
//			if (i_ret == -1) {                 /* ���o�G���[ */
//				d_rr = 0.0;
//				d_rtt = 0.0;
//			}
//			else {                              /* �_�E�������N�����W�擾 */
//				/*                d_rng_down = st_predict.d_predict_downlink_range ; *//* <<2013-TURBO>> �폜 */
//				d_rr_down = st_predict.d_predict_downlink_range_rate;
//				/*                d_rtt_down = d_rng_down / d_c ; *//* �_�E�������N���ԎZ�o *//* <<2013-TURBO>> �폜 */
//
//				d_rr = (d_rr_up + d_rr_down) / (double)2.0;
//				/*                d_rtt = d_rtt_up + d_rtt_down ; *//* <<2013-TURBO>> �폜 */
//			}
//
//			/* <<2013-TURBO>> �v�Z���@�ύX */
//			d_rtt = st_predict.d_predict_uplink_range * (double)2.0 / d_c;
//
//			break;
//
//		case RTT_BASE_DOWN:                   /* �_�E�������N�x�[�X */
//
//			/* <<2013-TURBO>> ���t�H�[�}�b�g�̏ꍇ�͏]���ʂ� */
//			if (st_predict.d_predict_2way_rng < (double)0.0) {
//
//				/* �_�E�������N�����W�擾 */
//				d_rng_down = st_predict.d_predict_downlink_range;
//				d_rr_down = st_predict.d_predict_downlink_range_rate;
//				d_rtt_down = d_rng_down / d_c;     /* �_�E�������N���ԎZ�o */
//
//				/* �A�b�v�����N���M�����̗\��l��\��l�e�[�u�����猟�� */
//				i_ret = lb_prdcal(&st_predict, stp_predict, d_pred_t - (d_rtt_down * 2), 1, psz_offset_time, d_offset_freq);
//				if (i_ret == -1) {                 /* ���o�G���[ */
//					d_rr = 0.0;
//					d_rtt = 0.0;
//				}
//				else {                              /* �A�b�v�����N�����W�擾 */
//					d_rng_up = st_predict.d_predict_uplink_range;
//					d_rr_up = st_predict.d_predict_uplink_range_rate;
//					d_rtt_up = d_rng_up / d_c;     /* �A�b�v�����N���ԎZ�o */
//
//					d_rr = (d_rr_up + d_rr_down) / (double)2.0;
//					d_rtt = d_rtt_up + d_rtt_down;
//				}
//			}
//			else {	/* <<2013-TURBO>> �V�t�H�[�}�b�g�̏ꍇ�͂Q�|�v�`�x�����W�������W���[�g���g�p���� */
//				d_rr = st_predict.d_predict_2way_range_rate / (double)2.0;
//				d_rtt = st_predict.d_predict_2way_rng / d_c;
//			}
//
//			break;
//		}
//	}
//	else {
//		d_rtt = 0.0;
//	}
//
//	*dp_range_rate = d_rr;                         /* �����ݒ�i�����`�d�����ω����^�Q�j */
//
//	return(d_rtt);
//}
//
//
///*============================================================================*/
///*! CTrendData
//
//-# �\��l��������
//
//�T�v�@�@ : �W�J�����\��l�e�[�u�����狁�߂����\��l���o�͂���B�W�J�����\��l�e�[
//�u�����ŋ��߂�\��l�������ꍇ�́A������Ԃ��s���B
//�܂��A���߂�\��l���W�J�����\��l�e�[�u�������ߋ��▢���̏ꍇ�A�W�J�����\��l�e�[�u����
//�ŏ��i�ߋ��̏ꍇ�j��Ō�i�����̏ꍇ�j���o�͂���B
//�����ɂ͓񕪒T���@��K�p����B
//
//@param	st_predict;     :���߂��\��l�̍\���̂̃|�C���^
//@param	gpst_predict;   :�W�J�����\��l�e�[�u���̃|�C���^
//@param	d_pred_t;        :���߂�\��l�̎����i�ώZ�b�j
//@param	i_sx;            :�ш�i���с��P�A���с��Q�A�����с��R�j
//@param	szp_time_offset :�l�������鎞���I�t�Z�b�g�l������
//�����FSMM:SS
//||| ||
//||| ++-- �b���E�l�A�󂢂��Ƃ���
//|||      �͂O�Ŗ��߂�
//|||
//|++----- �����E�l�A�󂢂��Ƃ���
//|        �͂O�Ŗ��߂�
//|
//+------- �����A"+"�͏ȗ��B
//�ȗ����̓u�����N��
//����邱�ƁB
//@param	d_freq_offset    :�l����������g���I�t�Z�b�g�iHz�j
//
//@retval	�O�ȏ� : ���o�����\��l�e�[�u���̔z��ʒu�B
//��Ԃ����ꍇ�͉ߋ����̃|�C���g��ݒ肷��B
//-1     : �G���[
//*/
///*============================================================================*/
//int CTrendData::lb_prdcal(one_predata_t* st_predict, predata_t* gpst_predict, double d_pred_t, int i_sx, char* szp_time_offset, double d_freq_offset)
//{
//	//	int i, i_j, i_cmp;
//	int i, i_cmp;
//	//	char sz_date1[50], sz_date2[50];
//	char sz_date1[50];
//	char sz_asctime[50], sz_asctime1[50];
//	unsigned char uca_pred_t[10];
//	/*	float f_bfr_data,f_aft_data; 2001-02-14 double �ɕύX�̂��ߕs�v */
//	double d_aft_data = 0.0;
//	double d_time_offset = 0.0;
//
//	int		j;
//	int		i_StartPoint;									/* ��ԊJ�n�ʒu					*/
//	int		i_SampleNum;									/* ��ԂɎg�p����T���v����		*/
//	//	int		i_Ret;
//
//	/* 2013-TURBO                   */
//	int     i_binsrch_num_total_pred;                    /* �\��l�e�[�u���G���g����     */
//	int     i_binsrch_min;                    /* �񕪒T���ŏ��l               */
//	int     i_binsrch_max;                    /* �񕪒T���ő�l               */
//	int     i_binsrch_mid;                    /* �񕪒T�������l               */
//	/*
//	printf( "DEBUG �\��l���� [%02x]\n", gpst_predict->predata[MAX_PRED_SIZE-1].ca_predict_s_tx_freq[0] ) ;
//	*/
//	/* 2013-TURBO                   */
//	/* �\��l�e�[�u���G���g�����擾 */
//	if (gpst_predict->predata[MAX_PRED_SIZE - 1].ca_predict_s_tx_freq[0] != (char)NULL) {
//		i_binsrch_num_total_pred = 86400;
//
//		/*
//		printf( "DEBUG �\��l���� i_binsrch_num_total_pred = [86400]\n" ) ;
//		*/
//
//
//
//	}
//	else {
//		i_binsrch_num_total_pred = (int)gpst_predict->predata[MAX_PRED_SIZE - 1].d_predict_2way_rng;
//
//		/*
//		printf( "DEBUG �\��l���� i_binsrch_num_total_pred = [%d]\n", i_binsrch_num_total_pred ) ;
//		*/
//
//	}
//
//	/*
//	i_binsrch_num_total_pred = 86400 ;
//	*/
//	i_binsrch_min = 1;
//	i_binsrch_max = i_binsrch_num_total_pred;
//
//
//	/* �ш�w��`�F�b�N */
//	if (i_sx != 1 && i_sx != 2 && i_sx != 3) {
//		/*
//		printf( "[%s](%d) :: �ш�w��`�F�b�NNG(%d)\n", __FUNCTION__, __LINE__, i_sx );
//		*/
//		return -1;
//	}
//
//	/* <<XUP>> �����I�t�Z�b�g�������ݒ肳��Ă���ꍇ�́A�����I�t�Z�b�g��ʎZ�b�ɕϊ� */
//	d_time_offset = 0.0;
//	if (szp_time_offset != NULL) {
//		d_time_offset = (double)(((szp_time_offset[1] - 0x30) * 10 + (szp_time_offset[2] - 0x30)) * 60 +
//			(szp_time_offset[4] - 0x30) * 10 + (szp_time_offset[5] - 0x30));
//		if (szp_time_offset[0] == '-') d_time_offset *= -1.0;
//	}
//
//	/* �w�莞���a�b�c����`�r�b�h�h�t�H�[�}�b�g�ɕϊ� <<XUP>> �����I�t�Z�b�g���l�� */
//	CSatelliteData::ub_dtime_to_asc((d_pred_t - d_time_offset), sz_asctime);
//	sz_asctime[19] = '\0';
//	//	i_Ret = asc19_to_bcd7_time((unsigned char*)sz_asctime, uca_pred_t);
//	asc19_to_bcd7_time((unsigned char*)sz_asctime, uca_pred_t);
//
//	/*
//	printf( "DEBUG �擪�\��l����(BCD):[%02x%02x%02x%02x%02x%02x%02x]\n",gpst_predict->predata[0].uca_time[0],gpst_predict->predata[0].uca_time[1],
//	gpst_predict->predata[0].uca_time[2],gpst_predict->predata[0].uca_time[3],
//	gpst_predict->predata[0].uca_time[4],gpst_predict->predata[0].uca_time[5],
//	gpst_predict->predata[0].uca_time[6]) ;
//	printf( "DEBUG �ڕW�\��l����(BCD):[%02x%02x%02x%02x%02x%02x%02x]\n",uca_pred_t[0],uca_pred_t[1],uca_pred_t[2],uca_pred_t[3],uca_pred_t[4],uca_pred_t[5],uca_pred_t[6] ) ;
//	*/
//
//
//
//
//	/* ���߂�\��l�����ƓW�J�\��l�i�P���ځj�̎�����r */
//	i_cmp = memcmp(uca_pred_t, gpst_predict->predata[0].uca_time, 7);
//
//
//
//
//	/* ���߂�\��l�������A�W�J�����\��l�����i�P���ځj�����O�̏ꍇ */
//	/* �� MIN|-----------�\��l--------------------------------|MAX   */
//	if (i_cmp < 0){
//
//
//
//		/*
//		printf( "DEBUG ���߂�\��l�������A�W�J�����\��l�����i�P���ځj�����ߋ�������\n" ) ;
//		*/
//
//
//
//		memcpy((unsigned char *)st_predict, (unsigned char *)&(gpst_predict->predata[0]), sizeof(one_predata_t));
//		/* �I�����ꂽ�����v���[�g�l���O�ɂ��� */
//		lb_ramp0(st_predict, i_sx);
//
//		/* �I�� <<XUP>> �߂�l�Ɍ��o�����z��ʒu��ݒ� */
//		/*
//		printf( "[%s](%d) :: �I�� <<XUP>> �߂�l�Ɍ��o�����z��ʒu��ݒ�\n", __FUNCTION__, __LINE__ );
//		*/
//		return 0;
//	}
//
//	/* ���߂�\��l�������A�W�J�����\��l�����i�P���ځj�ƈ�v */
//	/*  MIN��-----------�\��l--------------------------------|MAX   */
//	if (i_cmp == 0){
//
//
//
//		/*
//		printf( "DEBUG ���߂�\��l�������A�W�J�����\��l�����i�P���ځj�ƈ�v\n" ) ;
//		*/
//
//
//
//		lb_prdcal_hit(0, st_predict, gpst_predict, d_pred_t, i_sx, d_freq_offset);
//
//		/* �I�����ꂽ�����v���[�g�l���O�ɂ��� */
//		lb_ramp0(st_predict, i_sx);
//
//		/* �I�� <<XUP>> �߂�l�Ɍ��o�����z��ʒu��ݒ� */
//		/*
//		printf( "[%s](%d) :: �I�� <<XUP>> �߂�l�Ɍ��o�����z��ʒu��ݒ�\n", __FUNCTION__, __LINE__ );
//		*/
//		return 0;
//	}
//
//
//	/* ���߂�\��l�����ƓW�J�\��l�i�ŏI���ځj�̎�����r 2013-TURBO */
//	i_cmp = memcmp(uca_pred_t, gpst_predict->predata[i_binsrch_max - 1].uca_time, 7);
//
//	/*
//	printf( "DEBUG �ŏI�\��l����(BCD):[%02x%02x%02x%02x%02x%02x%02x]\n",gpst_predict->predata[i_binsrch_max-1].uca_time[0],gpst_predict->predata[i_binsrch_max-1].uca_time[1],
//	gpst_predict->predata[i_binsrch_max-1].uca_time[2],gpst_predict->predata[i_binsrch_max-1].uca_time[3],
//	gpst_predict->predata[i_binsrch_max-1].uca_time[4],gpst_predict->predata[i_binsrch_max-1].uca_time[5],
//	gpst_predict->predata[i_binsrch_max-1].uca_time[6]) ;
//	*/
//
//	/* ���߂�\��l�������A�W�J�����\��l�����i�ŏI���ځj�ƈ�v */
//	/*  MIN|-----------�\��l--------------------------------��MAX   */
//	if (i_cmp == 0){
//
//
//
//		/*
//		printf( "DEBUG ���߂�\��l�������A�W�J�����\��l�����i�ŏI���ځj�ƈ�v\n" ) ;
//		*/
//
//
//
//		lb_prdcal_hit((i_binsrch_max - 1), st_predict, gpst_predict, d_pred_t, i_sx, d_freq_offset);
//
//		/* �I�����ꂽ�����v���[�g�l���O�ɂ��� */
//		lb_ramp0(st_predict, i_sx);
//
//		/* �I�� <<XUP>> �߂�l�Ɍ��o�����z��ʒu��ݒ� */
//		/*
//		printf( "[%s](%d) :: �I�� <<XUP>> �߂�l�Ɍ��o�����z��ʒu��ݒ�\n", __FUNCTION__, __LINE__ );
//		*/
//		return 0;
//	}
//
//
//	/* ���߂�\��l�������A�W�J�����\��l�����i�ŏI���ځj���������̏ꍇ */
//	/*    MIN|-----------�\��l--------------------------------|MAX ��    */
//	if (i_cmp > 0){
//
//
//
//		/*
//		printf( "DEBUG ���߂�\��l�������A�W�J�����\��l�����i�ŏI���ځj��������������\n" ) ;
//		*/
//
//
//
//		/* ���߂�\��l�������A�W�J�����\��l����������̏ꍇ */
//		memcpy((unsigned char *)st_predict, (unsigned char *)&(gpst_predict->predata[i_binsrch_max - 1]), sizeof(one_predata_t));
//		/* �I�����ꂽ�����v���[�g�l���O�ɂ��� */
//		lb_ramp0(st_predict, i_sx);
//		/* �I�� <<XUP>> �߂�l�Ɍ��o�����z��ʒu��ݒ� */
//		/*
//		printf( "[%s](%d) :: �I�� <<XUP>> �߂�l�Ɍ��o�����z��ʒu��ݒ�(%d)\n", __FUNCTION__, __LINE__, i_binsrch_max-1 );
//		*/
//		return (i_binsrch_max - 1);
//	}
//
//
//
//
//
//	/* ���߂�\��l�������A�W�J�����\��l�����i�P���ځj������̏ꍇ       */
//	/* 2013-TURBO -- �񕪒T���J�n ------------------------------------------*/
//
//
//
//	/*
//	printf( "DEBUG \n" ) ;
//	printf( "DEBUG �񕪒T���J�n\n" ) ;
//	printf( "DEBUG \n" ) ;
//	*/
//
//
//
//	while (i_binsrch_min <= i_binsrch_max) {
//
//
//		/* �����l�Z�o                                                         */
//		/*       |---MIN|-------------�����l----------|MAX---------|          */
//		i_binsrch_mid = (i_binsrch_min + i_binsrch_max) / 2;
//
//
//		/*
//		printf( "DEBUG \n" ) ;
//		printf( "DEBUG \n" ) ;
//		printf( "DEBUG �����l�Z�o:i_binsrch_mid:[%d]\n",i_binsrch_mid ) ;
//		*/
//
//
//
//		/* ���߂�\��l�����ƓW�J�\��l�i�����l�j�̎�����r */
//		i_cmp = memcmp(uca_pred_t, gpst_predict->predata[i_binsrch_mid - 1].uca_time, 7);
//
//
//
//		/*
//		printf( "DEBUG �����\��l����(BCD):[%02x%02x%02x%02x%02x%02x%02x]\n",gpst_predict->predata[i_binsrch_mid-1].uca_time[0],gpst_predict->predata[i_binsrch_mid-1].uca_time[1],
//		gpst_predict->predata[i_binsrch_mid-1].uca_time[2],gpst_predict->predata[i_binsrch_mid-1].uca_time[3],
//		gpst_predict->predata[i_binsrch_mid-1].uca_time[4],gpst_predict->predata[i_binsrch_mid-1].uca_time[5],
//		gpst_predict->predata[i_binsrch_mid-1].uca_time[6]) ;
//		printf( "DEBUG �ڕW�\��l����(BCD):[%02x%02x%02x%02x%02x%02x%02x]\n",uca_pred_t[0],uca_pred_t[1],uca_pred_t[2],uca_pred_t[3],uca_pred_t[4],uca_pred_t[5],uca_pred_t[6] ) ;
//		*/
//
//
//
//		/* ���߂�\��l�������A�W�J�����\��l�����i�����l�j�Ɠ����ꍇ         */
//		/*       |---MIN|-------------�����l----------|MAX---------|          */
//		if (i_cmp == 0){
//
//			i = (i_binsrch_mid - 1);
//
//
//			/*
//			printf( "DEBUG �ڕW�\��l�����ƒ����l�\��l��������v:[%d]\n",i ) ;
//			*/
//
//
//			lb_prdcal_hit(i, st_predict, gpst_predict, d_pred_t, i_sx, d_freq_offset);
//
//			if (strlen((char *)gpst_predict->predata[i + 1].uca_time) == 0){
//				/* �I�����ꂽ�����v���[�g�l���O�ɂ��� */
//				lb_ramp0(st_predict, i_sx);
//			}
//
//			/* �I�� <<XUP>> �߂�l�Ɍ��o�����z��ʒu��ݒ� */
//			/*
//			printf( "[%s](%d) :: �I�� <<XUP>> �߂�l�Ɍ��o�����z��ʒu��ݒ�(%d)\n", __FUNCTION__, __LINE__, i );
//			*/
//			return i;
//
//
//
//
//		}
//		/* ���߂�\��l�������A�W�J�����\��l�����i�����l�j���������ɂ���ꍇ */
//		/*       |------MIN|----------�����l---��-----|MAX---------|            */
//		else if (i_cmp > 0){
//
//
//			/*
//			printf( "DEBUG �ڕW�\��l�����������l�\��l������薢���ɂ���\n" ) ;
//			*/
//
//
//			/* �����l�ƍő�l���ׂ荇����(�����l�ƍő�l�̊ԂɖڕW����������) */
//			/*       |------MIN|----------�����l��|MAX-----------------|      */
//			if (i_binsrch_max == (i_binsrch_mid + 1)) {
//				i = (i_binsrch_max - 1);
//
//
//				/*
//				printf( "DEBUG �����l�ƍő�l���ׂ荇����(�����l�ƍő�l�̊ԂɖڕW����������)\n" ) ;
//				printf( "DEBUG �ő�l:[%d]�����Ƃ��ă��[�v���E\n", i ) ;
//				*/
//
//
//				break;
//			}
//			else {
//				/* �����l�ƍő�l���ׂ荇���ĂȂ����ߌp�����Č���                 */
//				/*       |---------|-------MIN|------------|MAX---------|         */
//				i_binsrch_min = i_binsrch_mid;
//
//
//				/*
//				printf( "DEBUG �V���ȍŏ��l:i_binsrch_min:[%d]\n", i_binsrch_min ) ;
//				*/
//
//
//			}
//		}
//		/* ���߂�\��l�������A�W�J�����\��l�����i�����l�j�����ߋ��ɂ���ꍇ */
//		/*       |---MIN|------��------�����l---------|MAX---------|            */
//		else if (i_cmp < 0){
//
//
//			/*
//			printf( "DEBUG �ڕW�\��l�����������l�\��l�������ߋ��ɂ���\n" ) ;
//			*/
//
//
//			/* �����l�ƍŏ��l���ׂ荇����(�����l�ƍŏ��l�̊ԂɖڕW����������) */
//			/*       |------MIN|�������l----------|MAX-----------------|      */
//			if (i_binsrch_min == (i_binsrch_mid - 1)) {
//				i = (i_binsrch_mid - 1);
//
//
//				/*
//				printf( "DEBUG �����l�ƍŏ��l���ׂ荇����(�����l�ƍŏ��l�̊ԂɖڕW����������)\n" ) ;
//				printf( "DEBUG �����l:[%d]�����Ƃ��ă��[�v���E\n", i ) ;
//				*/
//
//
//				break;
//			}
//			else {
//				/* �����l�ƍŏ��l���ׂ荇���ĂȂ����ߌp�����Č���                 */
//				/*       |------MIN|----------|MAX-----|-------------------|      */
//				i_binsrch_max = i_binsrch_mid;
//
//
//				/*
//				printf( "DEBUG �V���ȍő�l:i_binsrch_max:[%d]\n", i_binsrch_max ) ;
//				*/
//
//
//			}
//		}
//	}
//	/* 2013-TURBO -- �񕪒T���I�� ------------------------------------------*/
//
//	/*
//	printf( "DEBUG �񕪒T���I�� ��:[%d]\n", i ) ;
//	*/
//	/* �O��ɏ\���f�[�^������ꍇ */
//	if (i >= (SAMPLING_NUM / 2)) {
//
//		/* ��ԊJ�n�ʒu�ݒ� */
//		i_StartPoint = i - SAMPLING_NUM / 2;
//	}
//
//	/* �O�����Ȃ��ꍇ */
//	else if (i < (SAMPLING_NUM / 2)) {
//
//		/* ��ԊJ�n�ʒu�ݒ�i�擪��ݒ�j */
//		i_StartPoint = 0;
//	}
//
//	/* ���O�����W�F��ԗp�z��ݒ� */
//	/* ���̈ʒu���擾 */
//	for (j = i_StartPoint; j < (i + SAMPLING_NUM); j++) {
//		if (strlen((char *)gpst_predict->predata[j].uca_time) == 0) break;
//		bcd7_to_asc19_time(gpst_predict->predata[j].uca_time, (unsigned char*)sz_date1);
//		if (CSatelliteData::ub_asc_to_asctime(sz_date1, sz_asctime1) < 0){
//			fprintf(stderr, "%s\n", "can not change sz_date1 to asctime \n");
//			/*
//			printf( "[%s](%d) :: ub_asc_to_asctime -1 [%s][%s]\n", __FUNCTION__, __LINE__, sz_date1, sz_asctime1 );
//			*/
//			return -1;
//		}
//		m_lla_time[j] = (time_t)CSatelliteData::ub_asc_to_dtime(sz_asctime1);
//		m_da_predict_az[j] = gpst_predict->predata[j].d_predict_az;
//		m_da_predict_el[j] = gpst_predict->predata[j].d_predict_el;
//		m_da_predict_uplink_range[j] = gpst_predict->predata[j].d_predict_uplink_range;
//		m_da_predict_downlink_range[j] = gpst_predict->predata[j].d_predict_downlink_range;
//		m_da_predict_uplink_range_rate[j] = gpst_predict->predata[j].d_predict_uplink_range_rate;
//		m_da_predict_downlink_range_rate[j] = gpst_predict->predata[j].d_predict_downlink_range_rate;
//		m_da_predict_2way_rng[j] = gpst_predict->predata[j].d_predict_2way_rng;		/* <<2013-TURBO>> */
//		m_da_predict_2way_range_rate[j] = gpst_predict->predata[j].d_predict_2way_range_rate;	/* <<2013-TURBO>> */
//
//		sscanf_s(gpst_predict->predata[j].ca_predict_s_tx_freq, "%lf", &m_da_predict_s_tx_freq[j]);
//		sscanf_s(gpst_predict->predata[j].ca_predict_s_rx_1way_freq, "%lf", &m_da_predict_s_rx_1way_freq[j]);
//		sscanf_s(gpst_predict->predata[j].ca_predict_x_rx_1way_freq, "%lf", &m_da_predict_x_rx_1way_freq[j]);
//		sscanf_s(gpst_predict->predata[j].ca_predict_s_rx_2way_freq, "%lf", &m_da_predict_s_rx_2way_freq[j]);
//		sscanf_s(gpst_predict->predata[j].ca_predict_x_rx_2way_freq, "%lf", &m_da_predict_x_rx_2way_freq[j]);
//		sscanf_s(gpst_predict->predata[j].ca_predict_s_rx_1way_ramprate, "%lf", &m_da_predict_s_rx_1way_ramprate[j]);
//		sscanf_s(gpst_predict->predata[j].ca_predict_x_rx_1way_ramprate, "%lf", &m_da_predict_x_rx_1way_ramprate[j]);
//		sscanf_s(gpst_predict->predata[j].ca_predict_s_rx_2way_ramprate, "%lf", &m_da_predict_s_rx_2way_ramprate[j]);
//		sscanf_s(gpst_predict->predata[j].ca_predict_x_rx_2way_ramprate, "%lf", &m_da_predict_x_rx_2way_ramprate[j]);
//	}
//
//	/***
//	��ԂɎg�p����z������߂�B
//	�ʏ�͖ڕW�����̑O��2�|�C���g���g�p���邪�A���ꂪ���Ȃ��ꍇ�͑O��ɃV�t�g����B
//	***/
//
//	/* �O��ɏ\���f�[�^������ꍇ */
//	if (j >= (i + SAMPLING_NUM)) {
//
//		/* �T���v�����ݒ� */
//		i_SampleNum = SAMPLING_NUM;
//	}
//
//	/* ��낪���Ȃ��ꍇ�i���̏ꍇ[j]�͔z��̏I�[+1���w���Ă���͂��j */
//	else {
//
//		/* �T���v�����ݒ�i����Ȃ��ꍇ�j */
//		i_SampleNum = j - i + 1;
//	}
//
//	/*�A�W�}�X*/
//	st_predict->d_predict_az = lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_az);
//
//	/*�G���x�[�V����*/
//	st_predict->d_predict_el = lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_el);
//
//	/*�A�b�v�����N�����W*/
//	st_predict->d_predict_uplink_range = lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_uplink_range);
//
//	/*�_�E�������N�����W*/
//	st_predict->d_predict_downlink_range = lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_downlink_range);
//
//	/*�A�b�v�����N�����W���[�g* 2001-02-10 ���̃f�[�^�͕�Ԃ̕K�v�Ȃ� */
//	/*                          2001-10-04 ����ς蕜�������邱�Ƃɂ��� */
//	st_predict->d_predict_uplink_range_rate = lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_uplink_range_rate);
//	/*	st_predict->d_predict_uplink_range_rate = gpst_predict->predata[i-1].d_predict_uplink_range_rate;
//	*/
//	/*�_�E�������N�����W���[�g* 2001-02-10 ���̃f�[�^�͕�Ԃ̕K�v�Ȃ� */
//	/*                          2001-10-04 ����ς蕜�������邱�Ƃɂ��� */
//	st_predict->d_predict_downlink_range_rate = lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_downlink_range_rate);
//	/*	st_predict->d_predict_downlink_range_rate = gpst_predict->predata[i-1].d_predict_downlink_range_rate;
//	*/
//
//	/* �Q�v�`�x�����W <<2013-TURBO>> */
//	st_predict->d_predict_2way_rng = lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_2way_rng);
//
//	/* �Q�v�`�x�����W���[�g <<2013-TURBO>> */
//	st_predict->d_predict_2way_range_rate = lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_2way_range_rate);
//
//	/*�\�����M���g��*/
//	/* <<XUP>> ���g���I�t�Z�b�g���l�� */
//	sprintf_s(st_predict->ca_predict_s_tx_freq, PRED_FREQ_LEN, "%.5lf", lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_s_tx_freq));
//
//	if (i_sx == 1){
//		/*1WAY�\����M���g��(s-band)*/
//		/* <<XUP>> ���g���I�t�Z�b�g���l�� */
//		sprintf_s(st_predict->ca_predict_s_rx_1way_freq, PRED_FREQ_LEN, "%.5lf", lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_s_rx_1way_freq));
//		/*1WAY�\����M���g��(x-band)*/
//		st_predict->ca_predict_x_rx_1way_freq[0] = '\0';
//
//		/*2WAY�\����M���g��(s-band)*/
//		/* <<XUP>> ���g���I�t�Z�b�g���l�� */
//		sprintf_s(st_predict->ca_predict_s_rx_2way_freq, PRED_FREQ_LEN, "%.5lf", lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_s_rx_2way_freq));
//		/*2WAY�\����M���g��(x-band)*/
//		st_predict->ca_predict_x_rx_2way_freq[0] = '\0';
//
//		/*1WAY�����v���[�g(S-BAND)*/
//		strcpy_s(st_predict->ca_predict_s_rx_1way_ramprate, PRED_RAMP_LEN, gpst_predict->predata[i - 1].ca_predict_s_rx_1way_ramprate);
//		/*1WAY�����v���[�g(X-BAND)*/
//		st_predict->ca_predict_x_rx_1way_ramprate[0] = '\0';
//
//		/*2WAY�����v���[�g(S-BAND)*/
//		strcpy_s(st_predict->ca_predict_s_rx_2way_ramprate, PRED_RAMP_LEN, gpst_predict->predata[i - 1].ca_predict_s_rx_2way_ramprate);
//		/*2WAY�����v���[�g(X-BAND)*/
//		st_predict->ca_predict_x_rx_2way_ramprate[0] = '\0';
//	}
//	else if (i_sx == 2){
//		/*1WAY�\����M���g��(s-band)*/
//		st_predict->ca_predict_s_rx_1way_freq[0] = '\0';
//		/*1WAY�\����M���g��(x-band)*/
//		/* <<XUP>> ���g���I�t�Z�b�g���l�� */
//		sprintf_s(st_predict->ca_predict_x_rx_1way_freq, PRED_FREQ_LEN, "%.5lf", lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_x_rx_1way_freq));
//
//		/*2WAY�\����M���g��(s-band)*/
//		st_predict->ca_predict_s_rx_2way_freq[0] = '\0';
//		/*2WAY�\����M���g��(x-band)*/
//		/* <<XUP>> ���g���I�t�Z�b�g���l�� */
//		sprintf_s(st_predict->ca_predict_x_rx_2way_freq, PRED_FREQ_LEN, "%.5lf", lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_x_rx_2way_freq));
//
//		/*1WAY�����v���[�g(S-BAND)*/
//		st_predict->ca_predict_s_rx_1way_ramprate[0] = '\0';
//		/*1WAY�����v���[�g(X-BAND)*/
//		strcpy_s(st_predict->ca_predict_x_rx_1way_ramprate, PRED_RAMP_LEN, gpst_predict->predata[i - 1].ca_predict_x_rx_1way_ramprate);
//		/*2WAY�����v���[�g(S-BAND)*/
//		st_predict->ca_predict_s_rx_2way_ramprate[0] = '\0';
//		/*2WAY�����v���[�g(X-BAND)*/
//		strcpy_s(st_predict->ca_predict_x_rx_2way_ramprate, PRED_RAMP_LEN, gpst_predict->predata[i - 1].ca_predict_x_rx_2way_ramprate);
//	}
//	else if (i_sx == 3){
//		/*1WAY�\����M���g��(s-band)*/
//		/* <<XUP>> ���g���I�t�Z�b�g���l�� */
//		sprintf_s(st_predict->ca_predict_s_rx_1way_freq, PRED_FREQ_LEN, "%.5lf", lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_s_rx_1way_freq));
//
//		/*1WAY�\����M���g��(x-band)*/
//		/* <<XUP>> ���g���I�t�Z�b�g���l�� */
//		sprintf_s(st_predict->ca_predict_x_rx_1way_freq, PRED_FREQ_LEN, "%.5lf", lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_x_rx_1way_freq));
//
//		/*2WAY�\����M���g��(s-band)*/
//		/* <<XUP>> ���g���I�t�Z�b�g���l�� */
//		sprintf_s(st_predict->ca_predict_s_rx_2way_freq, PRED_FREQ_LEN, "%.5lf", lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_s_rx_2way_freq));
//
//		/*2WAY�\����M���g��(x-band)*/
//		/* <<XUP>> ���g���I�t�Z�b�g���l�� */
//		sprintf_s(st_predict->ca_predict_x_rx_2way_freq, PRED_FREQ_LEN, "%.5lf", lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_x_rx_2way_freq));
//
//		/*1WAY�����v���[�g(S-BAND)*/
//		strcpy_s(st_predict->ca_predict_s_rx_1way_ramprate, PRED_RAMP_LEN, gpst_predict->predata[i - 1].ca_predict_s_rx_1way_ramprate);
//		/*1WAY�����v���[�g(X-BAND)*/
//		strcpy_s(st_predict->ca_predict_x_rx_1way_ramprate, PRED_RAMP_LEN, gpst_predict->predata[i - 1].ca_predict_x_rx_1way_ramprate);
//		/*2WAY�����v���[�g(S-BAND)*/
//		strcpy_s(st_predict->ca_predict_s_rx_2way_ramprate, PRED_RAMP_LEN, gpst_predict->predata[i - 1].ca_predict_s_rx_2way_ramprate);
//		/*2WAY�����v���[�g(X-BAND)*/
//		strcpy_s(st_predict->ca_predict_x_rx_2way_ramprate, PRED_RAMP_LEN, gpst_predict->predata[i - 1].ca_predict_x_rx_2way_ramprate);
//	}
//	/* �I�� <<XUP>> �߂�l�Ɍ��o�����z��ʒu��ݒ� */
//	/*
//	printf( "DEBUG �߂�l�Ɍ��o�����z��ʒu��ݒ�:[%d]\n", (i-1) ) ;
//	*/
//	printf("[%s](%d) :: �߂�l�Ɍ��o�����z��ʒu��ݒ�:[%d]\n", __FUNCTION__, __LINE__, i - 1);
//	return (i - 1);
//
//}
//
//
///*============================================================================*/
///*! CTrendData
//
//-# �����v���[�g������������
//
//@param	st_predict
//@param	i_sx
//@retval	�Ȃ�
//
//*/
///*============================================================================*/
//void CTrendData::lb_ramp0(one_predata_t* st_predict, int i_sx)
//{
//	if (i_sx == 1){
//		sprintf_s(st_predict->ca_predict_s_rx_1way_ramprate, PRED_RAMP_LEN, "0.00000"); /*1WAY�����v���[�g(S-BAND)*/
//		st_predict->ca_predict_x_rx_1way_ramprate[0] = '\0';            /*1WAY�����v���[�g(X-BAND)*/
//		sprintf_s(st_predict->ca_predict_s_rx_2way_ramprate, PRED_RAMP_LEN, "0.00000"); /*2WAY�����v���[�g(S-BAND)*/
//		st_predict->ca_predict_x_rx_2way_ramprate[0] = '\0';            /*2WAY�����v���[�g(X-BAND)*/
//	}
//	else if (i_sx == 2){
//		st_predict->ca_predict_s_rx_1way_ramprate[0] = '\0';            /*1WAY�����v���[�g(S-BAND)*/
//		sprintf_s(st_predict->ca_predict_x_rx_1way_ramprate, PRED_RAMP_LEN, "0.00000"); /*1WAY�����v���[�g(X-BAND)*/
//		st_predict->ca_predict_s_rx_2way_ramprate[0] = '\0';            /*2WAY�����v���[�g(S-BAND)*/
//		sprintf_s(st_predict->ca_predict_x_rx_2way_ramprate, PRED_RAMP_LEN, "0.00000"); /*2WAY�����v���[�g(X-BAND)*/
//	}
//	else if (i_sx == 3){
//		sprintf_s(st_predict->ca_predict_s_rx_1way_ramprate, PRED_RAMP_LEN, "0.00000"); /*1WAY�����v���[�g(S-BAND)*/
//		sprintf_s(st_predict->ca_predict_x_rx_1way_ramprate, PRED_RAMP_LEN, "0.00000"); /*1WAY�����v���[�g(X-BAND)*/
//		sprintf_s(st_predict->ca_predict_s_rx_2way_ramprate, PRED_RAMP_LEN, "0.00000"); /*2WAY�����v���[�g(S-BAND)*/
//		sprintf_s(st_predict->ca_predict_x_rx_2way_ramprate, PRED_RAMP_LEN, "0.00000"); /*2WAY�����v���[�g(X-BAND)*/
//	}
//}
//
//
///*============================================================================*/
///*! CTrendData
//
//-#
//
//@param	i
//@param	st_predict
//@param	gpst_predict
//@param	d_pred_t
//@param	i_sx
//@param	d_freq_offset
//@retval	�ݒ肵���z��ʒu(i)
//
//*/
///*============================================================================*/
//int CTrendData::lb_prdcal_hit(int i, one_predata_t* st_predict, predata_t* gpst_predict, double d_pred_t, int i_sx, double d_freq_offset)
//{
//
//	double d_aft_data = 0.0;
//	char sz_asctime[50];
//	//	int		i_Ret;
//
//	/* �W�J�����\��l���o�� */
//	memcpy((unsigned char *)st_predict, (unsigned char *)&(gpst_predict->predata[i]), sizeof(one_predata_t));
//
//	/* <<XUP>> �����I�t�Z�b�g�l���O�̎����ɏ��������� */
//	CSatelliteData::ub_dtime_to_asc(d_pred_t, sz_asctime);
//	sz_asctime[19] = '\0';
//	//	i_Ret = asc19_to_bcd7_time((unsigned char*)sz_asctime, st_predict->uca_time);
//	asc19_to_bcd7_time((unsigned char*)sz_asctime, st_predict->uca_time);
//
//	/* <<XUP>> �\�����M���g���Ɏ��g���I�t�Z�b�g���l�� */
//	sscanf_s(gpst_predict->predata[i].ca_predict_s_tx_freq, "%lf", &d_aft_data);
//	sprintf_s(st_predict->ca_predict_s_tx_freq, PRED_FREQ_LEN, "%.5lf", (d_aft_data + d_freq_offset));
//
//	if (i_sx == 1){
//		/*1WAY�\����M���g��(x-band)*/
//		st_predict->ca_predict_x_rx_1way_freq[0] = '\0';
//		/*2WAY�\����M���g��(x-band)*/
//		st_predict->ca_predict_x_rx_2way_freq[0] = '\0';
//		/*1WAY�����v���[�g(X-BAND)*/
//		st_predict->ca_predict_x_rx_1way_ramprate[0] = '\0';
//		/*2WAY�����v���[�g(X-BAND)*/
//		st_predict->ca_predict_x_rx_2way_ramprate[0] = '\0';
//
//		/*1WAY�\����M���g��(s-band) <<XUP>> ���g���I�t�Z�b�g���l�� */
//		sscanf_s(gpst_predict->predata[i].ca_predict_s_rx_1way_freq, "%lf", &d_aft_data);
//		sprintf_s(st_predict->ca_predict_s_rx_1way_freq, PRED_FREQ_LEN, "%.5lf", (d_aft_data + d_freq_offset));
//		/*2WAY�\����M���g��(s-band) <<XUP>> ���g���I�t�Z�b�g���l�� */
//		sscanf_s(gpst_predict->predata[i].ca_predict_s_rx_2way_freq, "%lf", &d_aft_data);
//		sprintf_s(st_predict->ca_predict_s_rx_2way_freq, PRED_FREQ_LEN, "%.5lf", (d_aft_data + d_freq_offset));
//	}
//	else if (i_sx == 2){
//		/*1WAY�\����M���g��(s-band)*/
//		st_predict->ca_predict_s_rx_1way_freq[0] = '\0';
//		/*2WAY�\����M���g��(s-band)*/
//		st_predict->ca_predict_s_rx_2way_freq[0] = '\0';
//		/*1WAY�����v���[�g(S-BAND)*/
//		st_predict->ca_predict_s_rx_1way_ramprate[0] = '\0';
//		/*2WAY�����v���[�g(S-BAND)*/
//		st_predict->ca_predict_s_rx_2way_ramprate[0] = '\0';
//
//		/*1WAY�\����M���g��(x-band) <<XUP>> ���g���I�t�Z�b�g���l�� */
//		sscanf_s(gpst_predict->predata[i].ca_predict_x_rx_1way_freq, "%lf", &d_aft_data);
//		sprintf_s(st_predict->ca_predict_x_rx_1way_freq, PRED_FREQ_LEN, "%.5lf", (d_aft_data + d_freq_offset));
//		/*2WAY�\����M���g��(x-band) <<XUP>> ���g���I�t�Z�b�g���l�� */
//		sscanf_s(gpst_predict->predata[i].ca_predict_x_rx_2way_freq, "%lf", &d_aft_data);
//		sprintf_s(st_predict->ca_predict_x_rx_2way_freq, PRED_FREQ_LEN, "%.5lf", (d_aft_data + d_freq_offset));
//	}
//	else if (i_sx == 3){	/* <<XUP>> S/X�̃P�[�X��ǉ� */
//		/*1WAY�\����M���g��(s-band) <<XUP>> ���g���I�t�Z�b�g���l�� */
//		sscanf_s(gpst_predict->predata[i].ca_predict_s_rx_1way_freq, "%lf", &d_aft_data);
//		sprintf_s(st_predict->ca_predict_s_rx_1way_freq, PRED_FREQ_LEN, "%.5lf", (d_aft_data + d_freq_offset));
//		/*2WAY�\����M���g��(s-band) <<XUP>> ���g���I�t�Z�b�g���l�� */
//		sscanf_s(gpst_predict->predata[i].ca_predict_s_rx_2way_freq, "%lf", &d_aft_data);
//		sprintf_s(st_predict->ca_predict_s_rx_2way_freq, PRED_FREQ_LEN, "%.5lf", (d_aft_data + d_freq_offset));
//
//		/*1WAY�\����M���g��(x-band) <<XUP>> ���g���I�t�Z�b�g���l�� */
//		sscanf_s(gpst_predict->predata[i].ca_predict_x_rx_1way_freq, "%lf", &d_aft_data);
//		sprintf_s(st_predict->ca_predict_x_rx_1way_freq, PRED_FREQ_LEN, "%.5lf", (d_aft_data + d_freq_offset));
//		/*2WAY�\����M���g��(x-band) <<XUP>> ���g���I�t�Z�b�g���l�� */
//		sscanf_s(gpst_predict->predata[i].ca_predict_x_rx_2way_freq, "%lf", &d_aft_data);
//		sprintf_s(st_predict->ca_predict_x_rx_2way_freq, PRED_FREQ_LEN, "%.5lf", (d_aft_data + d_freq_offset));
//	}
//
//	/*
//	printf( "[%s](%d) :: �߂�l�Ɍ��o�����z��ʒu��ݒ�:[%d]\n", __FUNCTION__, __LINE__, i );
//	*/
//	return i;
//}
//
//
//
//
///*============================================================================*/
///*! CTrendData
//
//-# �����ɂ��w�肳�ꂽ�_���ŁA��ԓ_���v�Z����B
//
//@param	i_dynamic_counter  �F�z���̃T���v���|�C���g�J�n�ʒu
//@param	n                  �F�T���v�����O��
//@param	t                  �F��Ԃɂ�苁�߂����ʒu(X)
//@param	i_in_time[]        �F��Ԍ��̃f�[�^�z��(X)
//@param	d_input_parameter[]�F��Ԍ��̃f�[�^�z��(Y)
//@retval	                   �F��Ԍ���(Y)
//
//*/
///*============================================================================*/
//double CTrendData::lb_lagrange(int i_dynamic_counter, int n, double t, time_t i_in_time[], double d_input_parameter[])
//{
//	int	ii, jj;               /* ���[�v�J�E���^�[ */
//	double	d_sum;            /*  */
//	double	d_prod;           /* �W�� */
//
//	d_sum = 0;
//
//	for (ii = i_dynamic_counter; ii < i_dynamic_counter + n; ii++){
//		d_prod = d_input_parameter[ii];
//		for (jj = i_dynamic_counter; jj < i_dynamic_counter + n; jj++){
//			if (ii != jj){
//				d_prod *= (t - (double)i_in_time[jj]) / (double)(i_in_time[ii] - i_in_time[jj]);
//			}
//		}
//		d_sum = d_sum + d_prod;
//	}
//	return d_sum;
//}
//#endif


/*============================================================================*/
/*! CTrendData

-# �����ɂ��w�肳�ꂽ�_���ŁA��ԓ_���v�Z����B

@param	i_dynamic_counter  �F�z���̃T���v���|�C���g�J�n�ʒu
@param	n                  �F�T���v�����O��
@param	t                  �F��Ԃɂ�苁�߂����ʒu(X)
@param	i_in_time[]        �F��Ԍ��̃f�[�^�z��(X)
@param	d_input_parameter[]�F��Ԍ��̃f�[�^�z��(Y)
@retval	                   �F��Ԍ���(Y)

*/
/*============================================================================*/
double CTrendData::CalcLagrange(int i_dynamic_counter, int n, double t, time_t i_in_time[], double d_input_parameter[])
{
	int	ii, jj;               /* ���[�v�J�E���^�[ */
	double	d_sum;            /*  */
	double	d_prod;           /* �W�� */

	d_sum = 0;

	for (ii = i_dynamic_counter; ii < i_dynamic_counter + n; ii++){
		d_prod = d_input_parameter[ii];
		for (jj = i_dynamic_counter; jj < i_dynamic_counter + n; jj++){
			if (ii != jj){
				d_prod *= (t - (double)i_in_time[jj]) / (double)(i_in_time[ii] - i_in_time[jj]);
			}
		}
		d_sum = d_sum + d_prod;
	}
	return d_sum;
}


/*============================================================================*/
/*! CTrendData

-# �\��l�f�[�^��ԏ���

@param	t:���ݎ���
@param	idx:�C���f�b�N�X�ʒu
@param	datalist:�f�[�^���X�g
@retval
/*============================================================================*/
double CTrendData::InterpolatePrd(double t, __int64* idx, CSpaceData* data)
{
	//	// ������AOS�O�̏ꍇ�A�ŏ��̒l���g��
	//	// ������LOS��̏ꍇ�A�ŋ��̒l���g��
	// ������AOS�`LOS�̊Ԃ̏ꍇ�A�\��l�f�[�^�e�[�u������Ώۃf�[�^�̃C���f�b�N�X��T��
	double search_t;
	__int64 ii, data_pos, size;
	size = (__int64)data->value.size();

	for (ii = *idx; ii < size; ii++)
	{
		search_t = (double)CTime(data->value[ii].systime).GetTime();
		if (search_t >= t)
		{
//			TRACE(_T("InterpolatePrd: search_t:%f, t:%f, ii:%I64d\n"), search_t, t, ii);
			break;
		}
	}
	// �C���f�b�N�X�X�V
	*idx = ii;

	// �C���f�b�N�X�̑O��Q�����Ԍ��f�[�^�Ƃ���
	if (ii < 2)
	{
		// ���X�g�擪�t��
		// �C���f�b�N�X��2��菬�����ꍇ�́A�擪�Q�̃C���f�b�N�X���g��
		data_pos = 0;
	}
	else if (ii > size - 1 - 2)
	{
		// ���X�g�I�[�t��
		// �C���f�b�N�X���ŏI�ʒu�Q�O���傫���ꍇ�́A�ŏI�C���f�b�N�X����S���g��
		if (size > 4)
		{
			// �\��l��4�����傫���ꍇ
			data_pos = size - 4;
		}
		else
		{
			// �\��l��4�ȉ�
			data_pos = 0;
		}
	}
	else
	{
		// ����ȊO�̓C���f�b�N�X�ʒu����2�O���g��
		if (ii >= 2)
		{
			data_pos = ii - 2;
		}
		else
		{
			data_pos = 0;
		}
	}

	time_t	i_in_time[LGR_SMP_NUM];
	double	d_input_parameter[LGR_SMP_NUM];

	// ��ԗp�f�[�^�̃T���v�����O����
	for (_int64 i = 0; i < LGR_SMP_NUM; i++)
	{
		i_in_time[i] = CTime(data->value[data_pos + i].systime).GetTime();
		d_input_parameter[i] = data->value[data_pos + i].value;
	}

	// �T���v�����O�f�[�^���g�p���A��Ԃ���
	return CalcLagrange(0, LGR_SMP_NUM, t, i_in_time, d_input_parameter);
}


/*============================================================================*/
/*! CTrendData

-# �q���܂ł̉��������𒼐���Ԃ��ă}�b�v�ɒǉ�����

@param	t1
@param	rng1
@param	t2
@param	rng2
@retval	�Ȃ�

*/
/*============================================================================*/
double CTrendData::GetLinearInterpolation(time_t t1, double dRng1, time_t t2, double dRng2, time_t t)
{
//	return m_2WayRngMap[t1] + (t - t1) / (t2 - t1) * (m_2WayRngMap[t2] - m_2WayRngMap[t1]);
	double a = m_2WayRngMap[t1];
	double b = (double)(t - t1) / (double)(t2 - t1);
	double c = m_2WayRngMap[t2] - m_2WayRngMap[t1];

	double res = a + b * c;

	return res;
}


/*============================================================================*/
/*! CTrendData

-# �q���܂ł̉��������̎擾
�\��l����A�w�莞���̉q���܂ł̉����������擾����B
�\��l�t�@�C���ɂ��傤�ǂ̎����������ꍇ�́A������Ԃ��ċ��߂�

@param	t�F�w�莞��
@retval	��������

*/
/*============================================================================*/
double CTrendData::Get2WayRng(time_t t)
{
	{
		CString strDebug;
		strDebug.Format(_T("time(%I64d)"), t);
		CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("Get2WayRng"), _T("debug"), strDebug, nLogEx::debug);
	}

	auto itr = m_2WayRngMap.lower_bound(t);
	// �͈͂ɂ���ꍇ
	if (itr != m_2WayRngMap.end())
	{
		if ((*itr).first == t)
		{
			// �\��l�}�b�v�Ɏ���������ꍇ�́A�}�b�v�̒l��Ԃ�
			return m_2WayRngMap[t];
		}
		else if (itr == m_2WayRngMap.begin())
		{
			// �P�Ԗڂ��O�̂��߁A�P�ԖڂƂQ�Ԗڂ̗\��l�ŕ��
			auto itr1 = itr;
			itr++;
			auto itr2 = itr;
			return GetLinearInterpolation((*itr1).first, (*itr1).second, (*itr2).first, (*itr2).second, t);
		}
		else
		{
			// �ЂƂO�̗\��l�ƌ��������\��l�ŕ��
			auto itr2 = itr;
			itr--;
			auto itr1 = itr;
			return GetLinearInterpolation((*itr1).first, (*itr1).second, (*itr2).first, (*itr2).second, t);
		}
	}
	// �͈͂ɖ����ꍇ
	else if (m_2WayRngMap.size() >= 2)
	{
		// �Ō���P�O�̗\��l�ƍŌ�̗\��l�ŕ��
		itr--;
		auto itr2 = itr;
		itr--;
		auto itr1 = itr;
		return GetLinearInterpolation((*itr1).first, (*itr1).second, (*itr2).first, (*itr2).second, t);
	}

	return -1.0;
}


CSpectrumViewData::BandWidth_t spectrum_gast_bandw[] = {    /* "SPEC_MEASURE_BW" */
	{ _T("32MHz"), _T("-16[MHz]"), _T("+16[MHz]"), -16000000.0, 16000000.0 },
	{ _T("16MHz"), _T("-8[MHz]"), _T("+8[MHz]"), -8000000.0, 8000000.0 },
	{ _T("8MHz"), _T("-4[MHz]"), _T("+4[MHz]"), -4000000.0, 4000000.0 },
	{ _T("4MHz"), _T("-2[MHz]"), _T("+2[MHz]"), -2000000.0, 2000000.0 },
	{ _T("2MHz"), _T("-1[MHz]"), _T("+1[MHz]"), -1000000.0, 1000000.0 },
	{ _T("1MHz"), _T("-500[KHz]"), _T("+500[KHz]"), -500000.0, 500000.0 },
	{ _T("500kHz"), _T("-250[KHz]"), _T("+250[KHz]"), -250000.0, 250000.0 },
	{ _T("250kHz"), _T("-125[KHz]"), _T("+125[KHz]"), -125000.0, 125000.0 },
	{ _T("125kHz"), _T("-62.5[KHz]"), _T("+62.5[KHz]"), -62500.0, 62500.0 },
	{ _T("62.5kHz"), _T("-31.25[KHz]"), _T("+31.25[KHz]"), -31250.0, 31250.0 },
	{ _T("31.25kHz"), _T("-15.625[KHz]"), _T("+15.625[KHz]"), -15625.0, 15625.0 },
	{ _T("15.63kHz"), _T("-7.815[KHz]"), _T("+7.815[KHz]"), -7815.0, 7815.0 },
	{ _T("7.813kHz"), _T("-3.9065[KHz]"), _T("+3.9065[KHz]"), -3906.5, 3906.5 },
	{ _T("3.910kHz"), _T("-1.955[KHz]"), _T("+1.955[KHz]"), -1955.0, 1955.0 },
	{ _T("1.953kHz"), _T("-0.9765[KHz]"), _T("+0.9765[KHz]"), -976.5, 976.5 },
	{ _T("977Hz"), _T("-488.5[Hz]"), _T("+488.5[Hz]"), -488.5, 488.5 },
	{ _T("488Hz"), _T("-244[Hz]"), _T("+244[Hz]"), -244.0, 244.0 },
	{ _T("244Hz"), _T("-122[Hz]"), _T("+122[Hz]"), -122.0, 122.0 },
	{ _T("122Hz"), _T("-61[Hz]"), _T("+61[Hz]"), -61.0, 61.0 },
	{ _T("61Hz"), _T("-30.5[Hz]"), _T("+30.5[Hz]"), -30.5, 30.5 },
	{ _T("31Hz"), _T("-15.5[Hz]"), _T("+15.5[Hz]"), -15.5, 15.5 },
	{ _T("15Hz"), _T("-7.5[Hz]"), _T("+7.5[Hz]"), -7.5, 7.5 },
	{ _T("8Hz"), _T("-4[Hz]"), _T("+4[Hz]"), -4.0, 4.0 },
	{ _T(""), _T(""), _T(""), 0.0, 0.0 } /* end of data */
};

CString    spectrum_gsz_dbm_str[] = { _T("[dBm]") };

CString spectrum_gpsz_moni_point_str[3] = {
	_T("70M_IF"),
	_T("70M+PREDICTION"),
	_T("CARR_LOOP_AID"),
};

CString spectrum_gpsz_bandw_str[23] = {
	_T("32MHz(RBW:31250Hz)"),
	_T("16MHz(RBW:15625Hz)"),
	_T("8MHz(RBW:7812.5Hz)"),
	_T("4MHz(RBW:3906.25Hz)"),
	_T("2MHz(RBW:1953.125Hz)"),
	_T("1MHz(RBW:976.526Hz)"),
	_T("500kHz(RBW:488.281Hz)"),
	_T("250kHz(RBW:244.141Hz)"),
	_T("125kHz(RBW:122.070Hz"),
	_T("62.5kHz(RBW:61.0351Hz)"),
	_T("31.25kHz(RBW:30.5176Hz)"),
	_T("15.63kHz(RBW:15.2637Hz)"),
	_T("7.813kHz(RBW:7.01465Hz)"),
	_T("3.910kHz(RBW:3.81836Hz)"),
	_T("1.953kHz(RBW:1.90723Hz)"),
	_T("977Hz(RBW:0.954102Hz)"),
	_T("488Hz(RBW:0.476563Hz)"),
	_T("244Hz(RBW:0.238281Hz)"),
	_T("122Hz(RBW:0.119141Hz)"),
	_T("61Hz(RBW:0.0595703Hz)"),
	_T("31Hz(RBW:0.0302734Hz)"),
	_T("15Hz(RBW:0.0146484Hz)"),
	_T("8Hz(RBW:0.0078125Hz)")
};

//----------------------------------------------------------------------
// �F�ݒ�
COLORREF Spectrum_colText[5] = {
	RGB(0, 0, 0),		// ��
	RGB(255, 165, 0),	// �I�����W
	RGB(0, 0, 255),		// ��
	RGB(255, 0, 0),		// ��
	RGB(0, 128, 0),		// ��
};

// �P��
CString Spectrum_strFreqUnit[3] = {
	_T("[Hz]"),
	_T("[kHz]"),
	_T("[MHz]"),
};

// ���S���g��
CString gsz_center_str[1] = { _T("70MHz") };


/* ------------------------------------------------------------------------------------ */
/* �X�y�N�g�����\���f�[�^
/* ------------------------------------------------------------------------------------ */
CSpectrumViewData::CSpectrumViewData()
{
	m_stVScale.bAutoScale = TRUE;
	m_stVScale.gl_y_grid_max = 0;
	m_stVScale.gl_y_grid_min = -200;
	m_stVScale.gl_y_grid_val = 20;
	m_stVScale.gd_y_offset_val = 0;

	m_nStation = 0;
	m_nRetSpectrumData = ERR_RETURN;
//	m_dummy_moni_point = 0;
//	m_dummy_bandw = 0;
//	m_dummy_freq_ofs = 0.0;
//	m_dummy_center_freq = 0.0;
}


CSpectrumViewData::~CSpectrumViewData()
{
}


/*============================================================================*/
/*! CSpectrumViewData

-# �X�y�N�g�����f�[�^���������ɌĂ΂��

@param	nStationID�F�q��ID
@param	nViewID�F�r���[ID
@param	nGraphType�F�O���t�^�C�v
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumViewData::InitializeData(UINT nGraphType)
{
	// �ݒ菉����
	m_stVScale.bAutoScale = TRUE;
	m_stVScale.gl_y_grid_max = 0;
	m_stVScale.gl_y_grid_min = -200;
	if (nGraphType == eSpectrumGraphType_Normal)
	{
		m_stVScale.gl_y_grid_val = 20;
	}
	else
	{
		m_stVScale.gl_y_grid_val = 50;
	}
	m_stVScale.gd_y_offset_val = 0;

	m_gl_moni_point_sel = SPECTRUM_MONIP_70MIF_SEL;
	m_gl_bw_sel = 0;
	m_gd_freq_ofs_in = 0.0;
	m_gd_peak_freq = 0.0;
	m_gd_peak_level = 0.0;

//	m_gsz_time_tag = _T("123 12:34:56");
//	m_gsz_bandw_sts = _T("32MHz");
//	m_gsz_moni_point = _T("70M_IF");
	m_gsz_time_tag = _T("");
	m_gsz_bandw_sts = _T("");
	m_gsz_moni_point = _T("");
	m_gd_freq_ofs = 0.0;
	m_gd_center_freq = 0.0;

	double d_low = 0.0;
	double d_upp = 0.0;
	for (int i = 0; spectrum_gast_bandw[i].sz_sts[0] != NULL; i++){
		if (spectrum_gast_bandw[i].sz_sts.Compare(m_gsz_bandw_sts) == 0) {
			d_low = spectrum_gast_bandw[i].d_low;
			d_upp = spectrum_gast_bandw[i].d_upp;
			break;
		}
	}

	for (int i = SPECTRUM_NUM_MIN; i < SPECTRUM_NUM; i++)
	{
		m_gda_spectrum_data[i] = 0.0;
	}

	int nOffset = (int)((m_gd_freq_ofs_in - d_low) / ((d_upp - d_low) / (double)SPECTRUM_NUM)) - SPECTRUM_NUM / 2;
	m_nDummyOffset = m_nDummyOffset + nOffset;
}


/*============================================================================*/
/*! CSpectrumView

-# �f�[�^�t�@�C���ǂݍ��ݏ���

@param  fp�t�@�C���|�C���^
@param  nStationID�F�q��ID
@param  nViewID�F�r���[ID
@param  nGraphType�F�O���t�^�C�v
@retval 0�F����I�� / -1�F�ُ�I��
*/
/*============================================================================*/
int CSpectrumViewData::FileRead(FILE *fp, UINT nStationID, UINT nViewID, UINT nGraphType)
{
	double   d_val;
	long    l_val;
	int     i_ret;

	CStringA strHeaderA;

	if (nGraphType == eSpectrumGraphType_Normal)
		strHeaderA.Format(("%s Spectrum Graph"), SPECTRUM_HEADER);
	else
		strHeaderA.Format(("%s %s Graph-%d"), SPECTRUM_HEADER, (LPCSTR)CStringA(mStationSimpleString[nStationID]), nViewID + 1);
	CString strHeader = CString(strHeaderA);

	// �w�b�_��T��
	int szStr = strHeaderA.GetLength() + 1;
	LPTSTR lpsz = new TCHAR[szStr];
	wcscpy_s(lpsz, szStr, strHeader);
	i_ret = CSatelliteData::MovetHeaderInFile(fp, lpsz);
	delete [] lpsz;

	if (i_ret <= 0)
	{
		return -1;
	}

	CString str;

	/* �I�[�g�X�P�[�� */
	i_ret = CSatelliteData::GetConfFromFile(fp, SPECTRUM_FILE_AUTO_SCALE, &l_val);
	if (i_ret <= 0){
		//		fclose(fp);
		//		return(ERR_RETURN);
		m_stVScale.bAutoScale = FALSE;
	}
	else {
		m_stVScale.bAutoScale = (BOOL)l_val;
	}

	/* �O���b�h�ő�l */
	//	i_ret = CSatelliteData::ub_fgetconf(SPECTRUM_FILE, FILE_GRID_MAX, "%i", (char*)&l_val);
	i_ret = CSatelliteData::GetConfFromFile(fp, SPECTRUM_FILE_GRID_MAX, &l_val);
	if (i_ret <= 0){
		//		sprintf(gsz_ErrStr, "[id=%s,i_ret=%d]", FILE_GRID_MAX, i_ret);
		fclose(fp);
		return(ERR_RETURN);
	}
	else {
		m_stVScale.gl_y_grid_max = l_val;
		//		m_strVMax.Format(_T("%d"), m_gl_y_grid_max);
		str.Format(_T("%d"), m_stVScale.gl_y_grid_max);
		//		m_CEditMax.SetWindowTextW(str);
	}
	/* �O���b�h�ŏ��l */
	//	i_ret = CSatelliteData::ub_fgetconf(SPECTRUM_FILE, FILE_GRID_MIN, "%i", (char*)&l_val);
	i_ret = CSatelliteData::GetConfFromFile(fp, SPECTRUM_FILE_GRID_MIN, &l_val);
	if (i_ret <= 0){
		//		sprintf(gsz_ErrStr, "[id=%s,i_ret=%d]", FILE_GRID_MIN, i_ret);
		fclose(fp);
		return(ERR_RETURN);
	}
	else {
		m_stVScale.gl_y_grid_min = l_val;
		//		m_strVMin.Format(_T("%d"), m_gl_y_grid_min);
		str.Format(_T("%d"), m_stVScale.gl_y_grid_min);
		//		m_CEditMin.SetWindowTextW(str);
	}
	/* �O���b�h�l���� */
	//	i_ret = CSatelliteData::ub_fgetconf(SPECTRUM_FILE, FILE_GRID_VAL, "%i", (char*)&l_val);
	i_ret = CSatelliteData::GetConfFromFile(fp, SPECTRUM_FILE_GRID_VAL, &l_val);
	if (i_ret <= 0){
		//		sprintf(gsz_ErrStr, "[id=%s,i_ret=%d]", FILE_GRID_VAL, i_ret);
		fclose(fp);
		return(ERR_RETURN);
	}
	else {
		m_stVScale.gl_y_grid_val = l_val;
		//		m_strVGrid.Format(_T("%d"), m_gl_y_grid_val);
		str.Format(_T("%d"), m_stVScale.gl_y_grid_val);
		//		m_CEditGrid.SetWindowTextW(str);
	}
	/* �O���b�h�I�t�Z�b�g�l���� */
	//	i_ret = CSatelliteData::ub_fgetconf(SPECTRUM_FILE, FILE_GRID_OFS, "%f", (char*)&d_val);
	i_ret = CSatelliteData::GetConfFromFile(fp, SPECTRUM_FILE_GRID_OFS, &d_val);
	if (i_ret <= 0){
		//		sprintf(gsz_ErrStr, "[id=%s,i_ret=%d]", FILE_GRID_OFS, i_ret);
		fclose(fp);
		return(ERR_RETURN);
	}
	else {
		m_stVScale.gd_y_offset_val = d_val;
	}

	m_gd_y_offset_att = 0.0;

	//++ dummy >>>
	m_dDummyVOffset = m_stVScale.gd_y_offset_val;
	//++ dummy <<<

	return (0);
}


/*============================================================================*/
/*! CSpectrumView

-# �f�[�^�t�@�C���������ݏ���

@param  fp�F�t�@�C���|�C���^
@param  nStationID�F�q��ID
@param  nViewID�F�r���[ID
@param  nGraphType�F�O���t�^�C�v
@retval 0�F����I�� / -1�F�ُ�I��
*/
/*============================================================================*/
int CSpectrumViewData::FileWrite(FILE* fp, UINT nStationID, UINT nViewID, UINT nGraphType)
{
	/*------------------*/
	/* �t�@�C�����擾   */
	/*------------------*/
	CStringA strHeaderA;

	if (nGraphType == eSpectrumGraphType_Normal)
		strHeaderA.Format(("%s Spectrum Graph"), SPECTRUM_HEADER);
	else
		strHeaderA.Format(("%s %s Graph-%d"), SPECTRUM_HEADER, (LPCSTR)CStringA(mStationSimpleString[nStationID]), nViewID + 1);
	m_stVScale.strHeader = CString(strHeaderA);

	fwprintf(fp, _T("%s\n"), (LPCTSTR)m_stVScale.strHeader);
	fwprintf(fp, _T("%-10s %d;\n"), SPECTRUM_FILE_AUTO_SCALE, m_stVScale.bAutoScale);
	fwprintf(fp, _T("%-10s %d;\n"), SPECTRUM_FILE_GRID_MAX, m_stVScale.gl_y_grid_max);
	fwprintf(fp, _T("%-10s %d;\n"), SPECTRUM_FILE_GRID_MIN, m_stVScale.gl_y_grid_min);
	fwprintf(fp, _T("%-10s %d;\n"), SPECTRUM_FILE_GRID_VAL, m_stVScale.gl_y_grid_val);
	fwprintf(fp, _T("%-10s %.2f;\n\n"), SPECTRUM_FILE_GRID_OFS, m_stVScale.gd_y_offset_val);

	return 0;
}


/*============================================================================*/
/*! CSpectrumView

-# �Ď��f�[�^�l�擾

@param	�Ȃ�
@retval	1�F�擾���� / 0�F�擾���s

*/
/*============================================================================*/
int CSpectrumViewData::GetSpectrumData()
{
#ifdef _ENGVALCNV_TREND

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();
	int ret = 0;

	UINT sta;
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		// �P�ǉ�ʂ̏ꍇ�́A���ǔԍ�
		sta = theApp.GetSelectStation();
	}
	else
	{
		// ����ȊO�̏ꍇ�́A�w��ǔ�
		sta = m_nStation;
	}

	/* TIME TAG */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_TIME_TAG_NAME));
	if (obs_adr == NULL)
	{
		m_gsz_time_tag = _T("");
		ret = -1;
	}
	else
	{
		m_gsz_time_tag.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
	}

	/* MONI_POINT */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_MONI_POINT_NAME));
	if (obs_adr == NULL)
	{
		m_gsz_moni_point = _T("");
		ret = -1;
	}
	else
	{
		m_gsz_moni_point.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
	}

	/* BAND WIDTH */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_MEASURE_BW_NAME));
	if (obs_adr == NULL)
	{
		m_gsz_bandw_sts = _T("");
		ret = -1;
	}
	else
	{
		m_gsz_bandw_sts.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
	}

	/* FREQ_OFFSET */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_FREQ_OFFSET_NAME));
	if (obs_adr == NULL)
	{
		m_gd_freq_ofs = 0.0;
		m_gd_freq_ofs_in = 0.0;
		ret = -1;
	}
	else
	{
		m_gd_freq_ofs = obs_adr->d_data;
		m_gd_freq_ofs_in = m_gd_freq_ofs;
	}

	/* CENTER_FREQ */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_CENTER_FREQ_NAME));
	if (obs_adr == NULL)
	{
		m_gd_center_freq = 0.0;
		ret = -1;
	}
	else
	{
		m_gd_center_freq = obs_adr->d_data;
	}

	/* SPECTRUM DATA */
	m_gd_spectrum_peak = SPECTRUM_PEAK_DEF;
	for (int i = SPECTRUM_NUM_MIN; i < SPECTRUM_NUM; i++)
	{
		CString strSpectrumData;
		strSpectrumData.Format(_T("DSNTCR_SPE.SPECTRUM_%d"), i);
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, strSpectrumData);
		if (obs_adr == NULL)
		{
			m_gda_spectrum_data[i] = 0.0;
			ret = -1;
		}
		else
		{
			m_gda_spectrum_data[i] = obs_adr->d_data;

			if (m_gd_spectrum_peak <= m_gda_spectrum_data[i]) {
				m_gd_spectrum_peak = m_gda_spectrum_data[i];
			}
		}
	}

//#ifdef _DEBUG
//	{
//		SYSTEMTIME time;
//		GetSystemTime(&time);
//		CTime	curTime(time);
//		m_gsz_time_tag = curTime.Format(_T("%d %H:%M:%S"));
//		m_gsz_moni_point = spectrum_gpsz_moni_point_str[m_dummy_moni_point];
//		m_gsz_bandw_sts = spectrum_gast_bandw[m_dummy_bandw].sz_sts;
//		m_gd_freq_ofs = m_dummy_freq_ofs;
//		m_gd_freq_ofs_in = m_dummy_freq_ofs;
//		m_gd_center_freq = m_dummy_center_freq;
//		return 0;
//	}
//#endif

	m_nRetSpectrumData = ret;

	return ret;


#else
	// �_�~�[�f�[�^���Z�b�g
	CSpectrumViewData* pCSectrumData = &theApp.GetSatelliteData().GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return FALSE;
	}

	// �_�~�[�f�[�^����
	CreateDummyData();

	/* SPECTRUM DATA */
	int i_err = 0;
	pCSectrumData->m_gd_spectrum_peak = SPECTRUM_PEAK_DEF;
	for (int i = SPECTRUM_NUM_MIN; i < SPECTRUM_NUM; i++){
		if (pCSectrumData->m_gd_spectrum_peak <= pCSectrumData->m_gda_spectrum_data[i]) {
			pCSectrumData->m_gd_spectrum_peak = pCSectrumData->m_gda_spectrum_data[i];
		}
	}
	if (i_err != 0){
		return(ERR_RETURN);
	}

	return 1;
#endif /* _ENGVALCNV_TREND */
}


/*============================================================================*/
/*! CSpectrumView

-# �_�~�[�f�[�^�𐶐�����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumViewData::CreateDummyData()
{
	static int nDummyCnt = 0;

	double spectrum_data[SPECTRUM_NUM * 2];

	for (int i = 0; i < SPECTRUM_NUM * 2; i++)
	{
		double peek;
		if (i > 360 * 2 && i < 360 * 3)
			peek = 2.0;
		else
			peek = 1.0;
		spectrum_data[i] = sin(SPECTRUM_RADCV * i) * (50.0 + (double)(nDummyCnt % 10)) * peek / 2.0 - 90.0 - 120.0 / 2.0 - m_dDummyVOffset;
	}

	for (int i = SPECTRUM_NUM_MIN; i < SPECTRUM_NUM; i++)
	{
		m_gda_spectrum_data[i] = spectrum_data[i + m_nDummyOffset];
	}

	nDummyCnt++;
	if (nDummyCnt == SPECTRUM_NUM)
	{
		nDummyCnt = 0;
	}
}


/*============================================================================*/
/*! CSpectrumView

-# �_�~�[�I�t�Z�b�g�𐶐�����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumViewData::CreateDummyOffset()
{
	m_gd_freq_ofs = m_gd_freq_ofs_in;
	m_gd_center_freq = m_gd_center_freq + m_gd_freq_ofs_in;

	double d_low = 0.0;
	double d_upp = 0.0;
	for (int i = 0; spectrum_gast_bandw[i].sz_sts[0] != NULL; i++){
		if (spectrum_gast_bandw[i].sz_sts.Compare(m_gsz_bandw_sts) == 0) {
			d_low = spectrum_gast_bandw[i].d_low;
			d_upp = spectrum_gast_bandw[i].d_upp;
			break;
		}
	}

	int nOffset = (int)((m_gd_freq_ofs_in - d_low) / ((d_upp - d_low) / (double)SPECTRUM_NUM)) - SPECTRUM_NUM / 2;
	m_nDummyOffset = m_nDummyOffset + nOffset;
}


/* ------------------------------------------------------------------------------------ */
/* �ǔ����[�_�[����Ď��f�[�^�N���X
/* ------------------------------------------------------------------------------------ */
CTrackingRadarCtrlMonData::CTrackingRadarCtrlMonData()
	: m_i_into_flag(eOFF)
	, m_l_latest_time(eUNYOU_TIME + 1)
	, m_nViewID(-1)
	, m_nStationID(-1)
	, m_bPassEnd(true)
{
	m_yohoFileTime.dwHighDateTime = 0;
	m_yohoFileTime.dwLowDateTime = 0;
	memset(m_ca_yohouget, 0, 30);

	m_queAzElSkyline.clear();
	m_queAzElSkyline.shrink_to_fit();
	m_queAzElForecast.clear();
	m_queAzElForecast.shrink_to_fit();
	m_queAzElActual.clear();
	m_queAzElActual.shrink_to_fit();
	m_yohoFilePath = _T("");
}


CTrackingRadarCtrlMonData::~CTrackingRadarCtrlMonData()
{
}


/*============================================================================*/
/*! CTrackingRadarCtrlMonData

-# �f�[�^�X�V���ɌĂ΂��

@param	nIDEvent�F�C�x���gID
@retval	�Ȃ�

*/
/*============================================================================*/
void CTrackingRadarCtrlMonData::UpdateData()
{
//	int a = m_nViewID;
	if (m_nStationID < 0)
		return;

	/*--------------------------*/
	/*  ���b�ω�������̎擾  */
	/*--------------------------*/
	// �T�}����ʂŋǂ�����ꍇ�͊Ď��f�[�^�擾
	if (m_nViewID < eStation_MAX)
	{
		/* �^�p��(�Ď��f�[�^�j�̎擾������ */
		if (GetKanshiData() != 0)
		{
			// �擾���s
			return;
		}
	}

	/*----------------------------*/
	/*  �v���Z�X�N�����̏����\��  */
	/*----------------------------*/
	if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
		Gtinit();
		m_gst_dspinf.i_dsp_type = TYPE_TRND;
	}

	/*--------------------------*/
	/*  �g�����h�i�p���j�\����  */
	/*--------------------------*/
	else {
		/*** �Ď��f�[�^�擾�󋵂𔻒f ***/
		if (m_gst_dspinf.i_mondat_NGcnt <= 0) {
			/*** �\���ؑւ蔭���܂��͗\��l�擾�҂�������
			���\��l�擾�\���A�͍ĕ`�悷�� ***/
			if ((m_gst_dspinf.i_dsp_flg == eDSP_ON) &&
				(m_gst_dspinf.i_yohouti_get != eYOHOGET_MATI)) {
				Gtinit();
			}
		}
	}
}


///*============================================================================*/
///*! CTrackingRadarCtrlMonData
//
//-# �^�p�󋵔��ʏ��� 
//
//@param	�Ȃ�
//@retval	�Ȃ�
//
//*/
///*============================================================================*/
//int CTrackingRadarCtrlMonData::Gtunyou()
//{
//	// �T�}����ʂŋǂ�����ꍇ�͊Ď��f�[�^�擾
//	if (m_nViewID < eStation_MAX)
//	{
//		return GetKanshiData();
//	}
//	// �\��l�ꗗ��v��ꗗ�̏ꍇ�́A�Ď��f�[�^�͎g�킸�ɒ��ڃt�@�C������擾
//	else
//	{
//		return GetFileData();
//	}
//}


/*============================================================================*/
/*! CTrackingRadarCtrlMonData

-# �Ď��f�[�^�擾

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
int CTrackingRadarCtrlMonData::GetKanshiData()
{
	int			i_unyou;			/* �p�X�^�p�� 				  */
	long		d_time;				/* �p�X�^�p�J�n�܂ł̎��� 		  */
	char		ca_stime[30];		/* �p�X�J�n����                   */
	char		ca_etime[30];		/* �p�X�I������                   */
	TCHAR		ca_fpath[1024];		/* �\��l�t�@�C�����i�t���p�X�j   */
	char		ca_ftime[30];		/* �\��l�t�@�C���X�V����		  */
	char		ca_plnno[30];		/* �v��ԍ�                       */
	char		ca_eisei[100];		/* �q����       			      */
	char		ca_AOS_time[20];	/* �`�n�r����                     */
	char		ca_AOS_AZ[9];		/* �`�n�r�� �`�y�l                */
	char		ca_AOS_EL[8];		/* �`�n�r�� �d�k�l                */
	char		ca_LOS_time[20];	/* �k�n�r����                     */
	char		ca_LOS_AZ[9];		/* �k�n�r�� �`�y�l                */
	char		ca_LOS_EL[8];		/* �k�n�r�� �d�k�l                */
	char		ca_MEL_time[20];	/* �l�`�w�|�d�k����               */
	char		ca_MEL_AZ[9];		/* �l�`�w�|�d�k�� �`�y�l          */
	char		ca_MEL_EL[8];		/* �l�`�w�|�d�k�� �d�k�l          */
	char		ca_VISIBLE[9];		/* �u�h�r�h�a�k�d����             */
//	char        ca_yohouget[30];    /* �\��l�t�@�C���擾��         */
	double		db_realAZ;					// AZ�����l
	double		db_realEL;					// EL�����l
	double		db_end_time;				// �ŏI�f�[�^����
	BOOL		b_readAZ_EN;
	BOOL		b_readEL_EN;

	int			i_cmp1, i_cmp2, i_cmp3;
	int			i_cmp5;

	if (m_nStationID < 0)
		return -1;

	i_unyou = 0;
	d_time = 0;
	memset(ca_stime, 0, 30);
	memset(ca_etime, 0, 30);
	MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, "", -1, ca_fpath, _MAX_PATH);
	memset(ca_ftime, 0, 30);
	memset(ca_plnno, 0, 30);
	memset(ca_eisei, 0, 100);
	memset(ca_AOS_time, 0, 20);
	memset(ca_AOS_AZ, 0, 9);
	memset(ca_AOS_EL, 0, 8);
	memset(ca_LOS_time, 0, 20);
	memset(ca_LOS_AZ, 0, 9);
	memset(ca_LOS_EL, 0, 8);
	memset(ca_MEL_time, 0, 20);
	memset(ca_MEL_AZ, 0, 9);
	memset(ca_MEL_EL, 0, 8);
	memset(ca_VISIBLE, 0, 9);
	db_realAZ = 0.0;
	db_realEL = 0.0;
	db_end_time = 0.0;

	b_readAZ_EN = FALSE;
	b_readEL_EN = FALSE;

	/*------------------------*/
	/* �^�p�󋵊Ď��f�[�^�擾 */
	/*------------------------*/
	/* ����p�Ď��f�[�^�����J�E���^������ */
	m_gst_dspinf.i_mondat_NGcnt = 0;

	/*** ����p�Ď����ڎ擾 ***/

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

//	UINT sta = theApp.GetSelectStation();

	CString str;

	/* �Ď��̎��� */
#ifdef _DEBUG
	str.Format(_T("%s(%d) ::Get kanshi time"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(CTRL_MON_TIME));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: Get kanshi time"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else {
			db_end_time = (double)obs_adr->ld_data;
		}
	}

	/* �p�X�^�p�󋵁i�p�X��ԃt���O�j */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Pass operation (Pass status flag)"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(UNYOU_FLAG));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: Pass operation (Pass status flag)"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else {
			//		i_unyou = (int)obs_adr->d_data;
			CString strPassFlag(obs_adr->sz_sts_name);
			if (strPassFlag.Compare(_T("�p�X��")) == 0)
			{
				i_unyou = 1;
			}
			else
			{
				i_unyou = 0;
			}
		}
	}

	/* �p�X�^�p�J�n�܂ł̎��Ԏ擾�i�^�p�J�n�܂ł̎��ԁj */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Get pass operation time (Operation start time)"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(TILL_TIME));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: Get pass operation time (Operation start time)"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else {
			d_time = (long)obs_adr->d_data;
		}
	}

	/* �\��l�t�@�C���t���p�X���i�\��l�t�@�C�����j */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Pred file full path name(Pred file name)"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(PREDFILE_NAME));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: Pred file full path name(Pred file name)"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			if (MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, obs_adr->sz_sts_name, -1, ca_fpath, _MAX_PATH) != 0)
			{
				CString strFilePath = CString(ca_fpath);

				strFilePath.Trim(_T(" "));
				// �g���q��prd�̏ꍇ�Aanp�ɒu��������
				strFilePath.TrimRight(PRED_FILE_TYPE);
				if (strFilePath.IsEmpty())
				{
					m_gst_dspinf.i_mondat_NGcnt++;
#ifdef _DEBUG
					str.Format(_T("%s(%d) :: Could not get Pred file full path name"), __FILEW__, __LINE__);
					CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
				}
				else
				{

					strFilePath += ANP_FILE_TYPE;

					// �t�@�C������؂�o��
					CString strFileName = strFilePath.Right(strFilePath.GetLength() - strFilePath.ReverseFind(_T('/')) - 1);
					strFilePath = strFilePath.TrimRight(strFileName);
					strFilePath = strFilePath.TrimRight(_T("/"));
					// �q������؂�o��
					CString strSatelliteName = strFilePath.Right(strFilePath.GetLength() - strFilePath.ReverseFind(_T('/')) - 1);
					// �ǃt�H���_������
					CString strPredFolder = theApp.GetShareFilePath(eFileType_Pred_Folder, m_nStationID);
					CString strPredFilePath;
					strPredFilePath.Format(_T("%s%s\\%s"), (LPCTSTR)strPredFolder, (LPCTSTR)strSatelliteName, (LPCTSTR)strFileName);
					wcscpy_s(ca_fpath, 1024, strPredFilePath);
				}
			}
		}
	}

	/* �\��l�t�@�C���쐬�����i�\��l�쐬�����j */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Pred file make time(Pred make time)"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(PREDFILE_TIME));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: Pred file make time(Pred make time)"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			strcpy_s(ca_ftime, 30, obs_adr->sz_sts_name);
		}
	}

	/* �p�X�^�p�J�n�����i�p�X�^�p�J�n�����j */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Pass operation start time"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(START_TIME));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: Pass operation start time"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			strcpy_s(ca_stime, 30, obs_adr->sz_sts_name);
		}
	}

	/* �\��l�擾��� �擾 */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Get getting pred status"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(YOHOU_GET));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: Get getting pred status"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			strcpy_s(m_ca_yohouget, 30, obs_adr->sz_sts_name);
		}
	}

	/*** �\���p�Ď��f�[�^�擾 ***/

	/* �p�X�^�p�I�������i�p�X�^�p�I�������j */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Pass operation end tile"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(STOP_TIME));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: Pass operation end tile"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			strcpy_s(ca_etime, 30, obs_adr->sz_sts_name);
		}
	}

	/* �v��ԍ� */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Pass ID"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(PLAN_NO));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: Pass ID"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			strcpy_s(ca_plnno, 30, obs_adr->sz_sts_name);
		}
	}

	/* �q�����擾�i�q�����́j */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Get satellite name(Satellite name)"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(EISEI_NAME));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: Get satellite name(Satellite name)"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			strcpy_s(ca_eisei, 100, obs_adr->sz_sts_name);
		}
	}

	/* �`�n�r�����i�`�n�r�����j */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: AOS time(AOS time)"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(AOS_TIME));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: AOS time(AOS time)"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			strcpy_s(ca_AOS_time, 20, obs_adr->sz_sts_name);
		}
	}

	/* �`�n�r���`�y�l�i�`�n�r���`�y�l�j */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: AOS AZ(AOS AZ)"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(AOS_AZ));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: AOS AZ(AOS AZ)"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			strcpy_s(ca_AOS_AZ, 9, obs_adr->sz_sts_name);
		}
	}

	/* �`�n�r���d�k�l�i�`�n�r���d�k�l�j */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: AOS EL(AOS EL)"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(AOS_EL));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: AOS EL(AOS EL)"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			strcpy_s(ca_AOS_EL, 8, obs_adr->sz_sts_name);
		}
	}

	/* �k�n�r�����i�k�n�r�����j */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: LOS time(LOS time)"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(LOS_TIME));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: LOS time(LOS time)"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			strcpy_s(ca_LOS_time, 20, obs_adr->sz_sts_name);
		}
	}

	/* �k�n�r���`�y�l�i�k�n�r���`�y�l�j */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: LOS AZ(LOS AZ)"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(LOS_AZ));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: LOS AZ(LOS AZ)"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			strcpy_s(ca_LOS_AZ, 9, obs_adr->sz_sts_name);
		}
	}

	/* �k�n�r���d�k�l�i�k�n�r���d�k�l�j */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: LOS EL(LOS EL)"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(LOS_EL));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: LOS EL(LOS EL)"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			strcpy_s(ca_LOS_EL, 8, obs_adr->sz_sts_name);
		}
	}

	/* �l�`�w�|�d�k�����i�l�`�w�|�d�k�����j */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: MAX-EL time(MAX-EL time)"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(MAXEL_TIME));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: MAX-EL time(MAX-EL time)"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			strcpy_s(ca_MEL_time, 20, obs_adr->sz_sts_name);
		}
	}

	/* �l�`�w�|�d�k���`�y�l�i�l�`�w�|�d�k���`�y�l�j */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: MAX-EL AZ(MAX-EL AZ)"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(MAXEL_AZ));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: MAX-EL AZ(MAX-EL AZ)"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			strcpy_s(ca_MEL_AZ, 9, obs_adr->sz_sts_name);
		}
	}

	/* �l�`�w�|�d�k���d�k�l�i�l�`�w�|�d�k���d�k�l�j */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: MAX-EL EL(MAX-EL EL"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(MAXEL_EL));
	if (obs_adr != NULL)
	{

		//FILE *fp = NULL;
		//if ((fopen_s(&fp, "c:\\MEL_EL.dat", "wb") == 0) && (fp != NULL)){
		//	fwrite((char*)obs_adr, sizeof(CEngValCnv::obsname_t), 1, fp);
		//	fclose(fp);
		//}

		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: MAX-EL EL(MAX-EL EL"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			strcpy_s(ca_MEL_EL, 8, obs_adr->sz_sts_name);
		}
	}

	/* �u�h�r�h�a�k�d����(�u�h�r�h�a�k�d���� ) */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: VISIBLE time(VISIBLE time)"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(VISIBLE));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: VISIBLE time(VISIBLE time)"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			strcpy_s(ca_VISIBLE, 9, obs_adr->sz_sts_name);
		}
	}

	/* AZ�����l */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Real AZ"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(REAL_AZ));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: Real AZ"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			db_realAZ = obs_adr->d_data;
			b_readAZ_EN = TRUE;
		}
	}

	/* EL�����l */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Real EL"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(REAL_EL));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: Real EL"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else
		{
			db_realEL = obs_adr->d_data;
			b_readEL_EN = TRUE;
		}
	}

	/*** �Ď��f�[�^�擾�󋵂𔻒f ***/
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Check to get monitor data status"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	if (m_gst_dspinf.i_mondat_NGcnt > 0) {
		return(0);
	}

	/*--------------*/
	/* �^�p�󋵔��f */
	/*--------------*/
	/* �p�X�^�p�J�n���� �L�����`�F�b�N */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Check enable with pass operation start time"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	i_cmp5 = 1;
	if (strcmp(ca_stime, "") == 0) {
		i_cmp5 = 0;
	}
	else {
		i_cmp5 = strncmp(ca_stime, "                   ", 19);
	}

	/* �^�p�󋵔��� */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Check operation status"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	if (m_i_unyou == 1) {	/* �^�p�� */
		m_gst_dspinf.i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_IN;
		m_i_into_flag = CTrackingRadarCtrlMonData::eOFF;
	}
	else { 					/* �^�p���łȂ� */
		if (i_cmp5 != 0) {     /* �L���� */
			if ((d_time > 0) && (d_time <= CTrackingRadarCtrlMonData::eUNYOU_TIME)) {
				m_gst_dspinf.i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_START;	/* �^�p�J�n�O */
				if (m_l_latest_time > CTrackingRadarCtrlMonData::eUNYOU_TIME) {
					m_i_into_flag = CTrackingRadarCtrlMonData::eON;
#ifdef _DEBUG
					str.Format(_T("i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_START , m_i_into_flag = CTrackingRadarCtrlMonData::eON   , d_time = %ld\n"), d_time);
					CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
				}
				else {
					m_i_into_flag = CTrackingRadarCtrlMonData::eOFF;
#ifdef _DEBUG
					str.Format(_T("i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_START , m_i_into_flag = CTrackingRadarCtrlMonData::eOFF , d_time = %ld\n"), d_time);
					CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
				}
			}
			else {
				m_gst_dspinf.i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_OUT;	/* �^�p�O     */
				m_i_into_flag = CTrackingRadarCtrlMonData::eOFF;
#ifdef _DEBUG
				str.Format(_T("i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_OUT, m_i_into_flag = CTrackingRadarCtrlMonData::eOFF, d_time = %ld\n"), d_time);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
			}
		}
		else {				/* ������ */
			m_gst_dspinf.i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_OUT;	/* �^�p�O     */
			m_i_into_flag = CTrackingRadarCtrlMonData::eOFF;
#ifdef _DEBUG
			str.Format(_T("i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_OUT   , m_i_into_flag = CTrackingRadarCtrlMonData::eOFF , d_time = %ld\n"), d_time);
			CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
		}
	}

	/* �J�E���g�_�E���l���o�b�t�@�ɑҔ� */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Backup count down value to buffer"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	m_l_latest_time = d_time;

	/* �p�X�^�p�󋵁i�p�X��ԃt���O�j */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Pass Info"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(m_nStationID, CString(_T("CTRL.PASS_INFO")));
	if (obs_adr != NULL)
	{
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_mondat_NGcnt++;
			if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
				str.Format(_T("%s(%d) :: Pass Info"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("Error"), str, nLogEx::error);
			}
		}
		else {
			CString strPassEnd(obs_adr->sz_sts_name);
			if (strPassEnd.Compare(_T("�^�p�I��")) == 0)
			{
				m_bPassEnd = true;
			}
			else
			{
				m_bPassEnd = false;
			}
		}
	}

	/*--------------------------------------*/
	/* �\��l�t�@�C���擾�����������o       */
	/*--------------------------------------*/
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Detect cancel to get pred file"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	if (m_gst_dspinf.i_yohouti_get == CTrackingRadarCtrlMonData::eYOHOGET_MATI) {
		if (strcmp(ca_eisei, "VLBI") != 0) {
			// VLBI�ȊO�̏ꍇ
			if (strcmp(m_ca_yohouget, "TAKING") == 0) {
				m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_MATI;    /* �擾�҂��̂܂� */
			}
			else {
				m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_OK;      /* �擾�\��� */
			}
		}
		else {
			// VLBI�̏ꍇ
			if (m_bPassEnd) {
				m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_MATI;    /* �擾�҂��̂܂� */
			}
			else {
				m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_OK;      /* �擾�\��� */
			}
		}
	}

	/* <<XUP>> �J�E���g�_�E���l���}�C�i�X�̏ꍇ�͈ڍs���Ԃ̂��߁A�ȉ��̃`�F�b�N�͍s��Ȃ� */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: No check, while count down value is under zero"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	if (d_time < 0) {
		return(0);
	}

	/*------------------------------*/
	/* �^�p�󋵊Ď��f�[�^�ۑ�       */
	/*------------------------------*/
	/* �O��̏�Ԃ�ۑ�  */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Save last time status"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	if (i_cmp5 != 0) {		/* �L���� */
		if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {	/* �N�����̏���\���� */
			wcscpy_s(m_gst_dspinf.ca_fpath_s, 1024, _T(""));
			strcpy_s(m_gst_dspinf.ca_ftime_s, 30, "");
			strcpy_s(m_gst_dspinf.ca_stime_s, 30, "");
		}
		else {										/* �g�����h�p���\���� */
			/*m_gst_dspinf.i_unyou_mode_s = m_gst_dspinf.i_unyou_mode;*/
			wcscpy_s(m_gst_dspinf.ca_fpath_s, 1024, m_gst_dspinf.ca_fpath);
			strcpy_s(m_gst_dspinf.ca_ftime_s, 30, m_gst_dspinf.ca_ftime);
			strcpy_s(m_gst_dspinf.ca_stime_s, 30, m_gst_dspinf.ca_stime);
		}
	}
	/* ����̏�Ԃ�ۑ� */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Save this time status"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	if (i_cmp5 != 0) {		/* �L���� */
		if (d_time > 0 || strcmp(ca_eisei, "VLBI") == 0) {
			wcscpy_s(m_gst_dspinf.ca_fpath, 1024, ca_fpath);
			strcpy_s(m_gst_dspinf.ca_ftime, 30, ca_ftime);
			strcpy_s(m_gst_dspinf.ca_stime, 30, ca_stime);
		}

		strcpy_s(m_gst_dspinf.ca_etime, 30, ca_etime);
		strcpy_s(m_gst_dspinf.ca_plnno, 30, ca_plnno);
		strcpy_s(m_gst_dspinf.ca_eisei, 100, ca_eisei);
		strcpy_s(m_gst_dspinf.ca_AOS_time, 20, ca_AOS_time);
		strcpy_s(m_gst_dspinf.ca_AOS_AZ, 9, ca_AOS_AZ);
		strcpy_s(m_gst_dspinf.ca_AOS_EL, 8, ca_AOS_EL);
		strcpy_s(m_gst_dspinf.ca_LOS_time, 20, ca_LOS_time);
		strcpy_s(m_gst_dspinf.ca_LOS_AZ, 9, ca_LOS_AZ);
		strcpy_s(m_gst_dspinf.ca_LOS_EL, 8, ca_LOS_EL);
		strcpy_s(m_gst_dspinf.ca_MEL_time, 20, ca_MEL_time);
		strcpy_s(m_gst_dspinf.ca_MEL_AZ, 9, ca_MEL_AZ);
		strcpy_s(m_gst_dspinf.ca_MEL_EL, 8, ca_MEL_EL);
		strcpy_s(m_gst_dspinf.ca_visible, 9, ca_VISIBLE);

//#ifdef _DEBUG
//		db_end_time += 1000;
//#endif

//		m_gst_dspinf.d_end_time = db_end_time;

		char	  full_stime[40];				/* �p�X�^�p�J�n�����i�t���j   */
		char	  full_etime[40];				/* �p�X�^�p�I�������i�t���j   */

		/* �p�X�J�n�����i�ώZ�b�j�Z�o */
		CSatelliteData::ub_asc_to_asctime(m_gst_dspinf.ca_stime, full_stime);
		double d_Stime = CSatelliteData::ub_asc_to_dtime(full_stime);

		/* �p�X�I�������i�ώZ�b�j�Z�o */
		CSatelliteData::ub_asc_to_asctime(m_gst_dspinf.ca_etime, full_etime);
		double d_Etime = CSatelliteData::ub_asc_to_dtime(full_etime);

		/* �p�X���ŁAAZ, EL�f�[�^���擾�L���ŁA�p�X�^�p�J�n����I���̊Ԃ̂ݕ`�� */
		if ((i_unyou == 1) &&
			(b_readAZ_EN == TRUE && b_readEL_EN == TRUE) && 
			(db_end_time >= 0.0 && d_Stime >= 0.0 && d_Etime >= 0.0) &&
			(db_end_time >= d_Stime && db_end_time <= d_Etime))
		{
			Az_El	azel;
			azel.d_AZ = db_realAZ;
			azel.d_EL = db_realEL;
			azel.d_time = db_end_time;

//#ifdef _DEBUG
//			if (m_queAzElActual.size() > 0)
//			{
//				__int64 nLastQueAzELIndex = (__int64)m_queAzElActual.size() - 1;
//				azel.d_AZ = m_queAzElActual[nLastQueAzELIndex].d_AZ + 1.0;
//				azel.d_EL = m_queAzElActual[nLastQueAzELIndex].d_EL + 1.0;
//				if (azel.d_AZ < 0 || azel.d_AZ >= 360.0)
//				{
//					azel.d_AZ = 0.0;
//				}
//				if (azel.d_EL < 0 || azel.d_EL > 90.0)
//				{
//					azel.d_EL = 0.0;
//				}
//			}
//#endif

			if (m_queAzElActual.size() > 0)
			{
				// �O��l�ƍ��ق���������f�[�^�ǉ�
				__int64 nLastQueAzELIndex = (__int64)m_queAzElActual.size() - 1;
				if (azel.d_AZ != m_queAzElActual[nLastQueAzELIndex].d_AZ || azel.d_EL != m_queAzElActual[nLastQueAzELIndex].d_EL)
				{
					m_queAzElActual.push_back(azel);
				}
			}
			else
			{
				// �L���[����Ȃ�f�[�^�ǉ�
				m_queAzElActual.push_back(azel);
			}
		}
	}

	/*--------------------*/
	/* �p�X�ؑ֔���       */
	/*--------------------*/
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Detect pass exchange"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	if (i_cmp5 != 0) { /* �L���� */
		i_cmp1 = wcscmp(m_gst_dspinf.ca_fpath_s, m_gst_dspinf.ca_fpath);
		m_gi_cmp1 = i_cmp1;
		i_cmp2 = strcmp(m_gst_dspinf.ca_ftime_s, m_gst_dspinf.ca_ftime);
		m_gi_cmp2 = i_cmp2;
		/* �v��J�n�����̕ύX�𒲂ׂ邱�ƂŁA�\��l�֘A�̏�񂪍X�V�����O�ɉ�ʂ�`�悵�Ă��܂��P�[�X���������邽�߁A
		�@ �v��J�n�����͒��ׂȂ����Ƃɂ���B
		  �@ �{��ʂ͗\��l�֘A�̏�񂵂��\�����Ă��Ȃ����߁A�v��֘A��񂪍X�V����Ă��A�\��l�֘A��񂪍X�V����Ȃ���΁A
			�@ ��ʂ��ĕ`�悷��K�v�͂Ȃ����߁A����Ŗ��Ȃ��B*/
		/*		i_cmp3 = strcmp( m_gst_dspinf.ca_stime_s, m_gst_dspinf.ca_stime ); */
	}
	else {			/* ������ */
		i_cmp1 = 0; i_cmp2 = 0; i_cmp3 = 0;
	}

	/*---------------------------------------*/
	/* �ĕ\���̗v�ۂ̔��f�̔���              */
	/*---------------------------------------*/
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Detect review necessity"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	/*	if( ((i_cmp1 != 0)||(i_cmp2 != 0)||(i_cmp3 != 0)) && */
	if (((i_cmp1 != 0) || (i_cmp2 != 0)) &&
		((m_gst_dspinf.i_unyou_mode == CTrackingRadarCtrlMonData::eUNYOU_IN) || (m_gst_dspinf.i_unyou_mode == CTrackingRadarCtrlMonData::eUNYOU_START))) {
		m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_ON;	/* �\���؂�ւ� */
	}
	else if (m_i_into_flag == CTrackingRadarCtrlMonData::eON) {
		m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_ON;  /* �\���؂�ւ� */
	}
	else if ((m_gst_dspinf.i_yohouti_get == CTrackingRadarCtrlMonData::eYOHOGET_OK) && (m_gst_dspinf.i_dsp_flg == CTrackingRadarCtrlMonData::eDSP_OFF) &&
		((m_gst_dspinf.i_unyou_mode == CTrackingRadarCtrlMonData::eUNYOU_IN) || (m_gst_dspinf.i_unyou_mode == CTrackingRadarCtrlMonData::eUNYOU_START))) {

		if ((i_cmp1 != 0) || (i_cmp2 != 0)) {
			/* �\��l�֘A�̏�񂪍X�V���ꂽ�ꍇ�A�\��l�擾 */
			m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_SUMI;
			m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_ON;      /* �\���ؑցi�ĕ`��K�v�j */
		}
		else {
			m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_SUMI;
		}
		//m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_ON;  	/* �\���ؑցi�ĕ`��K�v�j */
	}
	else {
		m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_OFF; 	/* �\���p���i�ĕ`��s�v�j */
	}

	if (m_gst_dspinf.i_mondat_NGcnt == 0)
	{
		m_l_inflg = INFLG_ON;
	}
	else
	{
		m_l_inflg = INFLG_OFF;
	}

	return(0);
}


//#if 0
///*============================================================================*/
///*! CTrackingRadarCtrlMonData
//
//-# �t�@�C������f�[�^�擾
//
//@param	�Ȃ�
//@retval	�Ȃ�
//
//*/
///*============================================================================*/
//int CTrackingRadarCtrlMonData::GetFileData()
//{
//	int			i_unyou;			/* �p�X�^�p�� 				  */
//	long long	d_time;				/* �p�X�^�p�J�n�܂ł̎��� 		  */
//	char		ca_stime[30];		/* �p�X�J�n����                   */
//	char		ca_etime[30];		/* �p�X�I������                   */
//	TCHAR		ca_fpath[1024];		/* �\��l�t�@�C�����i�t���p�X�j   */
//	char		ca_ftime[30];		/* �\��l�t�@�C���X�V����		  */
//	char		ca_plnno[30];		/* �v��ԍ�                       */
//	char		ca_eisei[100];		/* �q����       			      */
//	char		ca_AOS_time[20];	/* �`�n�r����                     */
//	char		ca_AOS_AZ[9];		/* �`�n�r�� �`�y�l                */
//	char		ca_AOS_EL[8];		/* �`�n�r�� �d�k�l                */
//	char		ca_LOS_time[20];	/* �k�n�r����                     */
//	char		ca_LOS_AZ[9];		/* �k�n�r�� �`�y�l                */
//	char		ca_LOS_EL[8];		/* �k�n�r�� �d�k�l                */
//	char		ca_MEL_time[20];	/* �l�`�w�|�d�k����               */
//	char		ca_MEL_AZ[9];		/* �l�`�w�|�d�k�� �`�y�l          */
//	char		ca_MEL_EL[8];		/* �l�`�w�|�d�k�� �d�k�l          */
//	char		ca_VISIBLE[9];		/* �u�h�r�h�a�k�d����             */
//	char        ca_yohouget[30];    /* �\��l�t�@�C���擾��         */
//
//	long		l_inflg;
//	int			i_cmp1, i_cmp2, i_cmp3;
//	int			i_cmp5;
//
//	/*------------------------*/
//	/* �^�p�󋵊Ď��f�[�^�擾 */
//	/*------------------------*/
//	/* ����p�Ď��f�[�^�����J�E���^������ */
//	m_gst_dspinf.i_mondat_NGcnt = 0;
//
//	l_inflg = m_l_inflg;
//
//	if (l_inflg != INFLG_ON)
//	{
//		m_gst_dspinf.i_mondat_NGcnt++;
//		return -1;
//	}
//
//	/*** ����p�Ď����ڎ擾 ***/
//
//	CString str;
//
//	/* �p�X�^�p�󋵁i�p�X��ԃt���O�j */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Pass operation (Pass status flag)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	if (l_inflg != INFLG_ON)
//	{
//	}
//	else {
//		i_unyou = m_i_unyou;
//	}
//
//	/* �p�X�^�p�J�n�܂ł̎��Ԏ擾�i�^�p�J�n�܂ł̎��ԁj */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Get pass operation time (Operation start time)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	if (l_inflg != INFLG_ON)
//	{
//	}
//	else {
//		d_time = m_d_time;
//	}
//
//	/* �\��l�t�@�C���t���p�X���i�\��l�t�@�C�����j */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Pred file full path name(Pred file name)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	wcscpy_s(ca_fpath, 1024, m_ca_fpath);
//
//	/* �\��l�t�@�C���쐬�����i�\��l�쐬�����j */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Pred file make time(Pred make time)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_ftime, 30, m_ca_ftime);
//
//	/* �p�X�^�p�J�n�����i�p�X�^�p�J�n�����j */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Pass operation start time"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_stime, 30, m_ca_stime);
//
//	/* �\��l�擾��� �擾 */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Get getting pred status"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_yohouget, 30, m_ca_yohouget);
//
//
//	/*** �\���p�Ď��f�[�^�擾 ***/
//
//	/* �p�X�^�p�I�������i�p�X�^�p�I�������j */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Pass operation end tile"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_etime, 30, m_ca_etime);
//
//	/* �v��ԍ� */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Pass ID"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_plnno, 30, m_ca_plnno);
//
//	/* �q�����擾�i�q�����́j */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Get satellite name(Satellite name)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_eisei, 100, m_ca_eisei);
//
//	/* �`�n�r�����i�`�n�r�����j */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: AOS time(AOS time)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_AOS_time, 20, m_ca_AOS_time);
//
//	/* �`�n�r���`�y�l�i�`�n�r���`�y�l�j */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: AOS AZ(AOS AZ)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_AOS_AZ, 9, m_ca_AOS_AZ);
//
//	/* �`�n�r���d�k�l�i�`�n�r���d�k�l�j */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: AOS EL(AOS EL)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_AOS_EL, 8, m_ca_AOS_EL);
//
//	/* �k�n�r�����i�k�n�r�����j */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: LOS time(LOS time)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_LOS_time, 20, m_ca_LOS_time);
//
//	/* �k�n�r���`�y�l�i�k�n�r���`�y�l�j */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: LOS AZ(LOS AZ)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_LOS_AZ, 9, m_ca_LOS_AZ);
//
//	/* �k�n�r���d�k�l�i�k�n�r���d�k�l�j */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: LOS EL(LOS EL)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_LOS_EL, 8, m_ca_LOS_EL);
//
//	/* �l�`�w�|�d�k�����i�l�`�w�|�d�k�����j */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: MAX-EL time(MAX-EL time)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_MEL_time, 20, m_ca_MEL_time);
//
//	/* �l�`�w�|�d�k���`�y�l�i�l�`�w�|�d�k���`�y�l�j */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: MAX-EL AZ(MAX-EL AZ)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_MEL_AZ, 9, m_ca_MEL_AZ);
//
//	/* �l�`�w�|�d�k���d�k�l�i�l�`�w�|�d�k���d�k�l�j */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: MAX-EL EL(MAX-EL EL"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_MEL_EL, 8, m_ca_MEL_EL);
//
//	/* �u�h�r�h�a�k�d����(�u�h�r�h�a�k�d���� ) */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: VISIBLE time(VISIBLE time)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_VISIBLE, 9, m_ca_VISIBLE);
//
//
//	/*** �Ď��f�[�^�擾�󋵂𔻒f ***/
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Check to get monitor data status"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	if (m_gst_dspinf.i_mondat_NGcnt > 0) {
//		return(0);
//	}
//
//	/*--------------*/
//	/* �^�p�󋵔��f */
//	/*--------------*/
//	/* �p�X�^�p�J�n���� �L�����`�F�b�N */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Check enable with pass operation start time"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	i_cmp5 = 1;
//	if (strcmp(ca_stime, "") == 0) {
//		i_cmp5 = 0;
//	}
//	else {
//		i_cmp5 = strncmp(ca_stime, "                   ", 19);
//	}
//
//	/* �^�p�󋵔��� */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Check operation status"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	if (i_unyou == 1) {	/* �^�p�� */
//		m_gst_dspinf.i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_IN;
//		m_i_into_flag = CTrackingRadarCtrlMonData::eOFF;
//	}
//	else { 					/* �^�p���łȂ� */
//		if (i_cmp5 != 0) {     /* �L���� */
//			if ((d_time > 0) && (d_time <= CTrackingRadarCtrlMonData::eUNYOU_TIME)) {
//				m_gst_dspinf.i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_START;	/* �^�p�J�n�O */
//				if (m_l_latest_time > CTrackingRadarCtrlMonData::eUNYOU_TIME) {
//					m_i_into_flag = CTrackingRadarCtrlMonData::eON;
//#ifdef _DEBUG
//					str.Format(_T("i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_START , m_i_into_flag = CTrackingRadarCtrlMonData::eON   , d_time = %lld\n"), m_d_time);
//					CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//				}
//				else {
//					m_i_into_flag = CTrackingRadarCtrlMonData::eOFF;
//#ifdef _DEBUG
//					str.Format(_T("i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_START , m_i_into_flag = CTrackingRadarCtrlMonData::eOFF , d_time = %lld\n"), m_d_time);
//					CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//				}
//			}
//			else {
//				m_gst_dspinf.i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_OUT;	/* �^�p�O     */
//				m_i_into_flag = CTrackingRadarCtrlMonData::eOFF;
//#ifdef _DEBUG
//				str.Format(_T("i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_OUT, m_i_into_flag = CTrackingRadarCtrlMonData::eOFF, d_time = %lld\n"), m_d_time);
//				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//			}
//		}
//		else {				/* ������ */
//			m_gst_dspinf.i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_OUT;	/* �^�p�O     */
//			m_i_into_flag = CTrackingRadarCtrlMonData::eOFF;
//#ifdef _DEBUG
//			str.Format(_T("i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_OUT   , m_i_into_flag = CTrackingRadarCtrlMonData::eOFF , d_time = %lld\n"), m_d_time);
//			CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//		}
//	}
//
//	/* �J�E���g�_�E���l���o�b�t�@�ɑҔ� */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Backup count down value to buffer"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	m_l_latest_time = d_time;
//
//	/*--------------------------------------*/
//	/* �\��l�t�@�C���擾�����������o       */
//	/*--------------------------------------*/
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Detect cancel to get pred file"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	if (m_gst_dspinf.i_yohouti_get == CTrackingRadarCtrlMonData::eYOHOGET_MATI) {
//		if (strcmp(m_ca_yohouget, "TAKING") == 0) {
//			m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_MATI;    /* �擾�҂��̂܂� */
//		}
//		else {
//			m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_OK;      /* �擾�\��� */
//		}
//	}
//
//	/* <<XUP>> �J�E���g�_�E���l���}�C�i�X�̏ꍇ�͈ڍs���Ԃ̂��߁A�ȉ��̃`�F�b�N�͍s��Ȃ� */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: No check, while count down value is under zero"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	if (d_time < 0) {
//		return(0);
//	}
//
//
//	/*------------------------------*/
//	/* �^�p�󋵊Ď��f�[�^�ۑ�       */
//	/*------------------------------*/
//	/* �O��̏�Ԃ�ۑ�  */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Save last time status"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	if (i_cmp5 != 0) {		/* �L���� */
//		if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {	/* �N�����̏���\���� */
//			wcscpy_s(m_gst_dspinf.ca_fpath_s, 1024, _T(""));
//			strcpy_s(m_gst_dspinf.ca_ftime_s, 30, "");
//			strcpy_s(m_gst_dspinf.ca_stime_s, 30, "");
//		}
//		else {										/* �g�����h�p���\���� */
//			/*m_gst_dspinf.i_unyou_mode_s = m_gst_dspinf.i_unyou_mode;*/
//			wcscpy_s(m_gst_dspinf.ca_fpath_s, 1024, m_gst_dspinf.ca_fpath);
//			strcpy_s(m_gst_dspinf.ca_ftime_s, 30, m_gst_dspinf.ca_ftime);
//			strcpy_s(m_gst_dspinf.ca_stime_s, 30, m_gst_dspinf.ca_stime);
//		}
//	}
//	/* ����̏�Ԃ�ۑ� */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Save this time status"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	if (i_cmp5 != 0) {		/* �L���� */
//		if (d_time > 0) {
//			wcscpy_s(m_gst_dspinf.ca_fpath, 1024, ca_fpath);
//			strcpy_s(m_gst_dspinf.ca_ftime, 30, ca_ftime);
//			strcpy_s(m_gst_dspinf.ca_stime, 30, ca_stime);
//		}
//
//		strcpy_s(m_gst_dspinf.ca_etime, 30, ca_etime);
//		strcpy_s(m_gst_dspinf.ca_plnno, 30, ca_plnno);
//		strcpy_s(m_gst_dspinf.ca_eisei, 100, ca_eisei);
//		strcpy_s(m_gst_dspinf.ca_AOS_time, 20, ca_AOS_time);
//		strcpy_s(m_gst_dspinf.ca_AOS_AZ, 9, ca_AOS_AZ);
//		strcpy_s(m_gst_dspinf.ca_AOS_EL, 8, ca_AOS_EL);
//		strcpy_s(m_gst_dspinf.ca_LOS_time, 20, ca_LOS_time);
//		strcpy_s(m_gst_dspinf.ca_LOS_AZ, 9, ca_LOS_AZ);
//		strcpy_s(m_gst_dspinf.ca_LOS_EL, 8, ca_LOS_EL);
//		strcpy_s(m_gst_dspinf.ca_MEL_time, 20, ca_MEL_time);
//		strcpy_s(m_gst_dspinf.ca_MEL_AZ, 9, ca_MEL_AZ);
//		strcpy_s(m_gst_dspinf.ca_MEL_EL, 8, ca_MEL_EL);
//		strcpy_s(m_gst_dspinf.ca_visible, 9, ca_VISIBLE);
//	}
//
//	/*--------------------*/
//	/* �p�X�ؑ֔���       */
//	/*--------------------*/
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Detect pass exchange"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	if (i_cmp5 != 0) { /* �L���� */
//		i_cmp1 = wcscmp(m_gst_dspinf.ca_fpath_s, m_gst_dspinf.ca_fpath);
//		m_gi_cmp1 = i_cmp1;
//		i_cmp2 = strcmp(m_gst_dspinf.ca_ftime_s, m_gst_dspinf.ca_ftime);
//		m_gi_cmp2 = i_cmp2;
//		/* �v��J�n�����̕ύX�𒲂ׂ邱�ƂŁA�\��l�֘A�̏�񂪍X�V�����O�ɉ�ʂ�`�悵�Ă��܂��P�[�X���������邽�߁A
//		�@ �v��J�n�����͒��ׂȂ����Ƃɂ���B
//		  �@ �{��ʂ͗\��l�֘A�̏�񂵂��\�����Ă��Ȃ����߁A�v��֘A��񂪍X�V����Ă��A�\��l�֘A��񂪍X�V����Ȃ���΁A
//			�@ ��ʂ��ĕ`�悷��K�v�͂Ȃ����߁A����Ŗ��Ȃ��B*/
//		/*		i_cmp3 = strcmp( m_gst_dspinf.ca_stime_s, m_gst_dspinf.ca_stime ); */
//	}
//	else {			/* ������ */
//		i_cmp1 = 0; i_cmp2 = 0; i_cmp3 = 0;
//	}
//
//	/*---------------------------------------*/
//	/* �ĕ\���̗v�ۂ̔��f�̔���              */
//	/*---------------------------------------*/
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Detect review necessity"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	/*	if( ((i_cmp1 != 0)||(i_cmp2 != 0)||(i_cmp3 != 0)) && */
//	if (((i_cmp1 != 0) || (i_cmp2 != 0)) &&
//		((m_gst_dspinf.i_unyou_mode == CTrackingRadarCtrlMonData::eUNYOU_IN) || (m_gst_dspinf.i_unyou_mode == CTrackingRadarCtrlMonData::eUNYOU_START))) {
//		m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_ON;	/* �\���؂�ւ� */
//	}
//	else if (m_i_into_flag == CTrackingRadarCtrlMonData::eON) {
//		m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_ON;  /* �\���؂�ւ� */
//	}
//	else if ((m_gst_dspinf.i_yohouti_get == CTrackingRadarCtrlMonData::eYOHOGET_OK) && (m_gst_dspinf.i_dsp_flg == CTrackingRadarCtrlMonData::eDSP_OFF) &&
//		((m_gst_dspinf.i_unyou_mode == CTrackingRadarCtrlMonData::eUNYOU_IN) || (m_gst_dspinf.i_unyou_mode == CTrackingRadarCtrlMonData::eUNYOU_START))) {
//
//		if ((i_cmp1 != 0) || (i_cmp2 != 0)) {
//			/* �\��l�֘A�̏�񂪍X�V���ꂽ�ꍇ�A�\��l�擾 */
//			m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_SUMI;
//			m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_ON;      /* �\���ؑցi�ĕ`��K�v�j */
//		}
//		else {
//			m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_SUMI;
//		}
//		//m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_ON;  	/* �\���ؑցi�ĕ`��K�v�j */
//	}
//	else {
//		m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_OFF; 	/* �\���p���i�ĕ`��s�v�j */
//	}
//
//	return(0);
//}
//#endif

/*============================================================================*/
/*! CTrackingRadarCtrlMonData

-# �L���[�N���A

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CTrackingRadarCtrlMonData::ClearQeue()
{
	if (m_nViewID >= eStation_MAX)
	{
		// �\��l�ꗗ�A�v��ꗗ��ʂ̏ꍇ�̓X�J�C���C���N���A
		m_queAzElSkyline.clear();
		m_queAzElSkyline.shrink_to_fit();
	}
//	m_queAzElRftrip.clear();
//	m_queAzElRftrip.shrink_to_fit();
	m_queAzElForecast.clear();
	m_queAzElForecast.shrink_to_fit();
	// �����l�L���[�̃N���A
	m_queAzElActual.clear();
	m_queAzElActual.shrink_to_fit();

	m_yohoFilePath = _T("");
}


/*============================================================================*/
/*! CTrackingRadarCtrlMonData

-# �ǔ��󋵏���������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CTrackingRadarCtrlMonData::Gtinit()
{
	// �����l�L���[�̃N���A
	m_queAzElActual.clear();
	m_queAzElActual.shrink_to_fit();

	/*--------------------------------------*/
	/* �X�J�C���C���E�q�e�g���b�v�f�[�^�擾   */
	/*--------------------------------------*/
	Gtgetskyl();
//#if 0
//	Gtgetrftr();
//#endif
	/*** �Ď��f�[�^�擾�󋵂𔻒f ***/
	if (m_gst_dspinf.i_mondat_NGcnt <= 0) {

		/*------------------------*/
		/* �\��l�f�[�^�擾 */
		/*------------------------*/
		if (m_gst_dspinf.i_unyou_mode != CTrackingRadarCtrlMonData::eUNYOU_OUT) {
			(void)Gtgetyoho();
			//#if _NOCOMM
			//			// �_�~�[�����l����
			//			AddDummyData();
			//#endif
		}
	}
}


/*============================================================================*/
/*! CTrackingRadarCtrlMonData

-# �X�J�C���C���f�[�^�擾

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CTrackingRadarCtrlMonData::Gtgetskyl()
{
	FILE    *fp;					/* �t�@�C���|�C���^               */
	char    ca_get[SATDATA_BUFF_SIZE];	    /* ���R�[�h�ǂݍ��݃o�b�t�@       */
	int     i_ret;

	if (m_nStationID < 0)
		return;

	/*------------------*/
	/* �t�@�C�����擾   */
	/*------------------*/
	CString strFilePath;
	strFilePath = theApp.GetShareFilePath(eFileType_Skyline_File, m_nStationID);

	/*------------------*/
	/* �t�@�C���n�o�d�m */
	/*------------------*/
	if (_wfopen_s(&fp, strFilePath, _T("r")) != 0 || fp == NULL) {
		return;
	}

	m_queAzElSkyline.clear();

	/*--------------------------*/
	/* �X�J�C���C�����R�[�h���� */
	/*--------------------------*/
	for (;;) {
		if (feof(fp)) {
			break;
		}
		/* �P�s�ǂݍ��� */
		if (fgets(ca_get, SATDATA_BUFF_SIZE, fp) == NULL) {
			break;
		}
		else {
			if (ca_get[0] != '#') {
				Az_El	azel;
				GetAzEl(ca_get, &azel);
				m_queAzElSkyline.push_back(azel);
			}
		}
	}

	/*--------------------*/
	/* �t�@�C���b�k�n�r�d */
	/*--------------------*/
	i_ret = fclose(fp);
}

//#if 0
///*============================================================================*/
///*! CTrackingRadarCtrlMonData
//
//-# �q�e�g���b�v�f�[�^�擾
//
//@param	�Ȃ�
//@retval	�Ȃ�
//
//*/
///*============================================================================*/
//void CTrackingRadarCtrlMonData::Gtgetrftr()
//{
//	FILE    *fp;					/* �t�@�C���|�C���^               */
//	char    ca_get[SATDATA_BUFF_SIZE];	    /* ���R�[�h�ǂݍ��݃o�b�t�@       */
//	int     i_ret;
//
//	/*------------------*/
//	/* �t�@�C�����擾   */
//	/*------------------*/
//	// ���M�d�͂���RF�g���b�v�t�@�C�����擾
//	CString strFilePath = CRFTripAngleFile::GetRfTripFilePath(m_nStationID);
//
//	/*------------------*/
//	/* �t�@�C���n�o�d�m */
//	/*------------------*/
//	if (_wfopen_s(&fp, strFilePath, _T("r")) != 0 || fp == NULL) {
//		return;
//	}
//
//	m_queAzElRftrip.clear();
//
//	/*--------------------------*/
//	/* �q�e�g���b�v���R�[�h���� */
//	/*--------------------------*/
//	for (;;) {
//		if (feof(fp)) {
//			break;
//		}
//		/* �P�s�ǂݍ��� */
//		if (fgets(ca_get, SATDATA_BUFF_SIZE, fp) == NULL) {
//			break;
//		}
//		else {
//			if (ca_get[0] != '#') {
//				Az_El	azel;
//				GetAzEl(ca_get, &azel);
//				m_queAzElRftrip.push_back(azel);
//			}
//		}
//	}
//	/*--------------------*/
//	/* �t�@�C���b�k�n�r�d */
//	/*--------------------*/
//	i_ret = fclose(fp);
//}
//#endif

/*============================================================================*/
/*! CTrackingRadarCtrlMonData

-# �\��l�f�[�^�擾

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
int CTrackingRadarCtrlMonData::Gtgetyoho()
{
	FILE    *fp;					/* �t�@�C���|�C���^               */
	char    ca_get[SATDATA_BUFF_SIZE];	    /* ���R�[�h�ǂݍ��݃o�b�t�@       */
	int     i;
	int     i_ret;
	int     i_type;

//	char    ca_yohouget[30];		/* �\��l�t�@�C���擾��   */

	if ((m_gi_cmp1 == 0) && (m_gi_cmp2 == 0)) {
		// �\��l��񂪍X�V����Ă��Ȃ��̂ő����^�[��
		return(0);
	}

	/*-----------------------------------*/
	/* �\��l�t�@�C���擾�۔���        */
	/*-----------------------------------*/
	/* �Ď��f�[�^�擾������ */
	if (m_l_inflg != INFLG_ON) {
		m_gst_dspinf.i_yohouti_get = eYOHOGET_MATI;    /* �擾�҂� */
		m_gst_dspinf.i_yohouti_umu = eYOHOUTI_NASI;
		return(0);
	}
	/* �۔��� */
	if (strcmp(m_gst_dspinf.ca_eisei, "VLBI") != 0) {
		// VLBI�ȊO�̏ꍇ
		if (strcmp(m_ca_yohouget, "TAKING") == 0) {  /* �\��l�擾�� */
			m_gst_dspinf.i_yohouti_get = eYOHOGET_MATI;    /* �擾�҂� */
			m_gst_dspinf.i_yohouti_umu = eYOHOUTI_NASI;
			return(0);
		}
		else {
			m_gst_dspinf.i_yohouti_get = eYOHOGET_OK;
		}
	}
	else {
		// VLBI�̏ꍇ
		if (m_bPassEnd) {
			m_gst_dspinf.i_yohouti_get = eYOHOGET_MATI;    /* �擾�҂� */
			m_gst_dspinf.i_yohouti_umu = eYOHOUTI_NASI;
		}
		else {
			m_gst_dspinf.i_yohouti_get = eYOHOGET_OK;
		}
	}

	/*------------------*/
	/* �t�@�C���n�o�d�m */
	/*------------------*/
	CString strFilePath = CString(m_gst_dspinf.ca_fpath);

	if (strFilePath.IsEmpty())
	{
		CString str;
		str.Format(_T("%s(%d) :: Pred file name is empty"), __FILEW__, __LINE__);
		CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtgetyoho"), _T("debug"), str, nLogEx::debug);
		/* �\��l���\���ł��Ȃ��Ă������𑱂��� */
		m_gst_dspinf.i_yohouti_umu = CTrackingRadarCtrlMonData::eYOHOUTI_NASI;
		return(0);
	}

	// �\��l�t�@�C�����X�V����Ă��邩�m�F
	HANDLE hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FILETIME yohoFileTime;
	GetFileTime(hFile, NULL, NULL, &yohoFileTime);
	CloseHandle(hFile);
	// �t�@�C�����ƃt�@�C��������r
	if (m_yohoFilePath.Compare(strFilePath) == 0 &&
		CompareFileTime(&m_yohoFileTime, &yohoFileTime) == 0)
	{
		// �ύX�Ȃ��Ȃ�Ή������Ȃ�
		return 0;
	}

	if (_wfopen_s(&fp, strFilePath, _T("r")) != 0 || fp == NULL) {
		CString str;
		str.Format(_T("%s(%d) :: Pred file open error"), __FILEW__, __LINE__);
		CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtgetyoho"), _T("debug"), str, nLogEx::debug);
		/* �\��l���\���ł��Ȃ��Ă������𑱂��� */
		m_gst_dspinf.i_yohouti_umu = CTrackingRadarCtrlMonData::eYOHOUTI_NASI;

		wcscpy_s(m_gst_dspinf.ca_fpath_s, 1024, _T(""));
		strcpy_s(m_gst_dspinf.ca_ftime_s, 30, "");
		strcpy_s(m_gst_dspinf.ca_stime_s, 30, "");
		wcscpy_s(m_gst_dspinf.ca_fpath, 1024, _T(""));
		strcpy_s(m_gst_dspinf.ca_ftime, 30, "");
		strcpy_s(m_gst_dspinf.ca_stime, 30, "");

		return(0);
	}

	m_yohoFilePath = strFilePath;
	m_yohoFileTime = yohoFileTime;

#ifdef _DEBUG
	{
		CString str;
		str.Format(_T("%s(%d) :: Pred file open(%s)"), __FILEW__, __LINE__, (LPCTSTR)CString(strFilePath));
		CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtgetyoho"), _T("debug"), str, nLogEx::debug);
	}
#endif
	/*--------------------------------------*/
	/* �w�b�_�y�ъǗ���񃌃R�[�h�ǂݔ�΂� */
	/*--------------------------------------*/
	for (i = 0; i<5; i++) {
		if (feof(fp)) {
			CString str;
			str.Format(_T("%s(%d) :: Pred file header error"), __FILEW__, __LINE__);
			CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtgetyoho"), _T("Error"), str, nLogEx::error);
			return(-1);
		}
		/* �P�s�ǂݍ��� */
		if (fgets(ca_get, SATDATA_BUFF_SIZE, fp) == NULL) {
			CString str;
			str.Format(_T("%s(%d) :: Pred file format error"), __FILEW__, __LINE__);
			CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtgetyoho"), _T("Error"), str, nLogEx::error);
			return(-1);
		}
	}

	m_queAzElForecast.clear();

	/*--------------------*/
	/* �\��l���R�[�h���� */
	/*--------------------*/
	i_type = 0;
	for (i = 0;; i++) {
		if (feof(fp)) {
			break;
		}
		/* �P�s�ǂݍ��� */
		if (fgets(ca_get, SATDATA_BUFF_SIZE, fp) == NULL) {
			break;
		}
		else {
			Forecast_Az_El	azelForecast;
			GetForecastAzEl(i_type, ca_get, &azelForecast, i);
			m_queAzElForecast.push_back(azelForecast);
			i_type = 1;
		}
	}

	/*--------------------*/
	/* �t�@�C���b�k�n�r�d */
	/*--------------------*/
	i_ret = fclose(fp);

	m_gst_dspinf.i_yohouti_umu = CTrackingRadarCtrlMonData::eYOHOUTI_ARI;

	return(0);
}


/*============================================================================*/
/*! CTrackingRadarCtrlMonData

-# AZ, EL�f�[�^�擾

@param	pc_rec�F���R�[�h�i�[�o�b�t�@
@param�@az_el�FAZ EL�f�[�^
@retval	�Ȃ�

*/
/*============================================================================*/
void CTrackingRadarCtrlMonData::GetAzEl(char* pc_rec, Az_El* az_el)
{
	char	ca_AZ[9];				/* ���ʊp						  */
	char	ca_EL[8];				/* �p							  */
	int     i, j;

	/*** �����f�[�^���f�d�s ***/
	for (i = 0, j = 0;; i++, j++) {	/* �`�y�Q�b�g */
		if (pc_rec[i] == ':') {
			ca_AZ[j] = NULL;
			i++;
			break;
		}
		ca_AZ[j] = pc_rec[i];
	}
	for (j = 0;; i++, j++) {		/* �d�k�Q�b�g*/
		if ((pc_rec[i] == ' ') || (pc_rec[i] == '\n')) {
			ca_EL[j] = NULL;
			break;
		}
		ca_EL[j] = pc_rec[i];
	}

	/*** �`�y�A�d�k�f�[�^���f�d�s ***/
	az_el->d_AZ = atof(ca_AZ);
	az_el->d_EL = atof(ca_EL);
}


/*============================================================================*/
/*! CTrackingRadarCtrlMonData

-# �\��l��AZ, EL�f�[�^�擾

@param  i_type�F
@param	pc_rec�F���R�[�h�i�[�o�b�t�@
@param�@az_el�FAZ EL�f�[�^
@param  i_roop_cnt�F���[�v�J�E���^
@retval	�Ȃ�

*/
/*============================================================================*/
void CTrackingRadarCtrlMonData::GetForecastAzEl(int i_type, char* pc_rec, Forecast_Az_El* az_el, int i_roop_cnt)
{
	char	ca_time[15];			/* ����							  */
	//	char	ca_TIME[26];			/* �����i�t���t�H�[�}�b�g�j		  */
	char	ca_TIME[40];			/* �����i�t���t�H�[�}�b�g�j		  */
	char	ca_AZ[9] = { 0 };		/* ���ʊp						  */
	char	ca_EL[8] = { 0 };		/* �p							  */
	char	ca_upRNG[18];			/* �q�m�f�i�A�b�v�����N�j		  */
	char	ca_upRR[12];			/* �q�q  �i�A�b�v�����N�j		  */
	char	ca_dwRNG[18];			/* �q�m�f�i�_�E�������N�j		  */
	char	ca_dwRR[12];			/* �q�q  �i�_�E�������N�j		  */
	static double	d_time, d_Stime, d_Etime;
	int     i_ret;

	/*** �����f�[�^���f�d�s ***/
	sscanf_s(pc_rec, "%s %s %s %s %s %s %s", ca_time, 15, ca_AZ, 9, ca_EL, 8, ca_upRNG, 18, ca_upRR, 12, ca_dwRNG, 18, ca_dwRR, 12);

	/*** �����f�[�^��ώZ�b�ɕϊ� ***/
	i_ret = CSatelliteData::ub_asc_to_asctime(ca_time, ca_TIME);
	d_time = CSatelliteData::ub_asc_to_dtime(ca_TIME);

	az_el->d_AZ = atof(ca_AZ);
	az_el->d_EL = atof(ca_EL);
	az_el->d_time = d_time;

	if (i_type == 0) {
		i_ret = CSatelliteData::ub_asc_to_asctime(m_gst_dspinf.ca_AOS_time, ca_TIME);
		d_Stime = CSatelliteData::ub_asc_to_dtime(ca_TIME);
		i_ret = CSatelliteData::ub_asc_to_asctime(m_gst_dspinf.ca_LOS_time, ca_TIME);
		d_Etime = CSatelliteData::ub_asc_to_dtime(ca_TIME);
	}
	az_el->d_aos = (d_time - d_Stime) / (d_Etime - d_Stime);
}


/* ------------------------------------------------------------------------------------ */
/* �Z���f�f��ʃf�[�^�Ǘ��N���X
/* ------------------------------------------------------------------------------------ */
CRdnsChkData::CRdnsChkData()
{
}


CRdnsChkData::~CRdnsChkData()
{
}

/*============================================================================*/
/*! CRdnsChkData

-# ������̃r�b�g���[�g��double�^(bps)�ɕϊ�����B

@param	str�F�r�b�g���[�g�i������j
@retval bps
*/
/*============================================================================*/
double CRdnsChkData::BitrateStrToDouble(CString str)
{
	int idx = -1;
	double br = 0;
	if ((idx = str.Find(_T("Mbps"))) != -1)
	{
		br = _wtof(str.Mid(0, idx)) * 1000000;
	}
	else if ((idx = str.Find(_T("kbps"))) != -1)
	{
		br = _wtof(str.Mid(0, idx)) * 1000;
	}
	else if ((idx = str.Find(_T("bps"))) != -1)
	{
		br = _wtof(str.Mid(0, idx));
	}
	return br;
}



/* ------------------------------------------------------------------------------------ */
/* �v�旧�ĉ�ʃf�[�^�Ǘ��N���X
/* ------------------------------------------------------------------------------------ */

/*============================================================================*/
/*! COprPlnData

-# ANT�҂������w���ʒm

@ brief ��������Ή�����C���f�b�N�X�����߂�

@param  �ݒ蕶����
@retval �C���f�b�N�X
*/
/*============================================================================*/
int COprPlnData::GetAntStandbyIdx(CString str)
{
	int idx = -1;
	for (int i = 0; strAriNasi[i] != _T(""); ++i)
	{
		if (strAriNasi[i] == str)
		{
			idx = i;
			break;
		}
	}
	return idx;
}

/*============================================================================*/
/*! COprPlnData

-# ANT�V���u���L��

@ brief ��������Ή�����C���f�b�N�X�����߂�

@param  �ݒ蕶����
@retval �C���f�b�N�X
*/
/*============================================================================*/
int COprPlnData::GetAntZenithIdx(CString str)
{
	int idx = -1;
	for (int i = 0; strANTZenith[i] != _T(""); ++i)
	{
		if (strANTZenith[i] == str)
		{
			idx = i;
			break;
		}
	}
	return idx;
}

/*============================================================================*/
/*! COprPlnData

-# �^�p�I�v�V����-�����ݒ�

@ brief ��������Ή�����C���f�b�N�X�����߂�

@param  �ݒ蕶����
@retval �C���f�b�N�X
*/
/*============================================================================*/
int COprPlnData::GetOpInitIdx(CString str)
{
	int idx = -1;
	for (int i = 0; strAriNasi[i] != _T(""); ++i)
	{
		if (strAriNasi[i] == str)
		{
			idx = i;
			break;
		}
	}
	return idx;
}

/*============================================================================*/
/*! COprPlnData

-# �^�p�I�v�V����-���M�@�h�b�v���⏞

@ brief ��������Ή�����C���f�b�N�X�����߂�

@param  �ݒ蕶����
@retval �C���f�b�N�X
*/
/*============================================================================*/
int COprPlnData::GetOpTDPIdx(CString str)
{
	int idx = -1;
	for (int i = 0; strTxDoppler[i] != _T(""); ++i)
	{
		if (strTxDoppler[i] == str)
		{
			idx = i;
			break;
		}
	}
	return idx;
}

/*============================================================================*/
/*! COprPlnData

-# �^�p�I�v�V����-���M�@�h�b�v���⏞

@ brief ��������Ή�����C���f�b�N�X�����߂�

@param  �ݒ蕶����
@retval �C���f�b�N�X
*/
/*============================================================================*/
int COprPlnData::GetOpTDPIdx2(CString str)
{
	int idx = TXDOPPLER_NASI_AVE;
	for (int i = 0; strTxDoppler2[i] != _T(""); ++i)
	{
		if (strTxDoppler2[i] == str)
		{
			idx = i;
			break;
		}
	}
	return idx;
}

/*============================================================================*/
/*! COprPlnData

-# �^�p�I�v�V����-��M�@�h�b�v���⏞

@ brief ��������Ή�����C���f�b�N�X�����߂�

@param  �ݒ蕶����
@retval �C���f�b�N�X
*/
/*============================================================================*/
int COprPlnData::GetOpRDPIdx(CString str)
{
	int idx = -1;

	if(theApp.GetSelectStation() == eStation_Usuda642)
	{
		for(int i=0; strRxDoppler54[i] != _T(""); ++i)
		{
			if(strRxDoppler54[i] == str)
			{
				idx = i;
				break;
			}
		}
		return idx;
	}

	for (int i = 0; strRxDoppler[i] != _T(""); ++i)
	{
		if (strRxDoppler[i] == str)
		{
			idx = i;
			break;
		}
	}
	return idx;
}

/*============================================================================*/
/*! COprPlnData

-# �^�p�I�v�V����-���M�o��

@ brief ��������Ή�����C���f�b�N�X�����߂�

@param  �ݒ蕶����
@retval �C���f�b�N�X
*/
/*============================================================================*/
int COprPlnData::GetOpTOutIdx(CString str, int type)
{
	int idx = -1;

	int s = theApp.GetSelectStation();
	if (s == 0)	// �P�c64m
	{
		for (int i = 0; strTrsPwr64[i] != _T(""); ++i)
		{
			if (strTrsPwr64[i] == str)
			{
				idx = i;
				break;
			}
		}
	}
	else if (s == 1)	// �P�c54m
	{
		for (int i = 0; strTrsPwr64[i] != _T(""); ++i)
		{
			if (strTrsPwr64[i] == str)
			{
				idx = i;
				break;
			}
		}
	}
	else if (s == 2)	// ���V�Y34m
	{
		if (type == TRSPWRX_X)
		{
			for (int i = 0; strTrsPwrX34[i] != _T(""); ++i)
			{
				if (strTrsPwrX34[i] == str)
				{
					idx = i;
					break;
				}
			}
		}
		else if (type == TRSPWRX_S)
		{
			for (int i = 0; strTrsPwrS34[i] != _T(""); ++i)
			{
				if (strTrsPwrS34[i] == str)
				{
					idx = i;
					break;
				}
			}
		}
	}
	else if (s == 3)	// ���V�Y20m
	{
		for (int i = 0; strTrsPwr20[i] != _T(""); ++i)
		{
			if (strTrsPwr20[i] == str)
			{
				idx = i;
				break;
			}
		}
	}

	return idx;
}

/*============================================================================*/
/*! COprPlnData

-# �v��t�@�C������������

@brief  VLBI,MAINT�p
@param  �Ȃ�
@retval TRUE�F����I��/FALSE�F�ُ�I��
*/
/*============================================================================*/
int COprPlnData::WritePlanFile(CString sz_satname, CString sz_passno, stPlandata stpd, CString comment)
{
	FILE *fp_smpl;
	FILE *fp_plan;
	CString sz_s_file;
	CString sz_p_folder;
	CString sz_p_file;

	TCHAR sz_s_start[50];
	TCHAR sz_buff[512];
	TCHAR sz_time[50];
	TCHAR sz_ele[10][100];
	TCHAR sz_work[256];
	TCHAR sz_date[50];

	long j;
	long k;
	long l;
	long m;
	long l_ncount;

	long l_hour;
	long l_min;
	long l_sec;

	__int64 d_s_start;
	__int64 d_ntime[10];
	__int64 d_time;
	__int64 d_start;

	int count = 0;
	int t_rdy = 0;

	memset(&sz_ele[0], 0, sizeof(sz_ele));

	/*----- ���`���v��t�@�C���̃p�X���쐬 -----*/
	CSatelliteData& sd = theApp.GetSatelliteData();

	CString filenamep = theApp.GetShareFilePath(eFileType_PlanOut_Folder, theApp.GetSelectStation());
	sz_p_file.Format(_T("%s%s\\%s.pln"), (LPCTSTR)filenamep, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	sz_p_folder.Format(_T("%s%s"), (LPCTSTR)filenamep, (LPCTSTR)sz_satname.MakeLower());
	if (PathFileExists(sz_p_folder) == FALSE)
	{
		CreateDirectory(sz_p_folder, NULL);
	}

	CString filenames = theApp.GetShareFilePath(eFileType_Form_Folder, theApp.GetSelectStation());
	sz_s_file.Format(_T("%s%s.pln"), (LPCTSTR)filenames, (LPCTSTR)sz_satname);
	sz_s_start[0] = '\0';


	if (_wfopen_s(&fp_smpl, sz_s_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("�v�搗�`�t�@�C���J�����Ƃ��ł��܂���B\n%s"), (LPCTSTR)sz_s_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	/*---------- �J�n�����R�����g�p�v����e�t�@�C���o�͏��� ----------*/
	d_s_start = 0;
	for (k = 0; k < 10; k++)
	{
		d_ntime[k] = 0;
	}

	/*---------- �R�����g�p�v����e�t�@�C���o�͏��� ----------*/

	char sz_line[512];
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_smpl) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			continue;
		}

		/*---------- �v����e�쐬���� ----------*/

		l = 0;
		if (sz_buff[0] == 'N')
		{
			if (sz_buff[1] >= 0x30 && sz_buff[1] <= 0x39)
			{
				l_ncount = sz_buff[1] - 0x30;
				k = 2;
			}
			else
			{
				l_ncount = 0;
				k = 1;
			}
		}
		else
		{
			l_ncount = -1;
			k = 0;
		}
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] != ' ')
			{
				sz_time[l] = sz_buff[k];
				l++;
			}
			else
			{
				if (l > 0)
				{
					sz_time[l] = '\0';
					break;
				}
			}
		}

		j = 0;
		l = 0;
		sz_ele[2][0] = '\0';
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] == ',') {
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_buff[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				sz_ele[j][l] = '\0';
				l = 0;
				break;
			}
			else if (sz_buff[k] != ' ')
			{
				sz_ele[j][l] = sz_buff[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			continue;
		}

		for (k = 0; sz_c_ele64[k] != _T(""); ++k)
		{
			if (sz_c_ele64[k].Find(CString(sz_ele[0])) >= 0)
			{
				break;
			}
		}

		m = swscanf_s(sz_time, _T("%d:%d:%d%s"),
			&l_hour, &l_min, &l_sec,
			sz_work, static_cast<unsigned>(_countof(sz_work)));

		if (m != 3)
		{
			if (sz_c_ele64[k] == _T(""))
			{
				continue;
			}
			fclose(fp_smpl);
			return -1;
		}
		d_time = (l_hour * 3600) + (l_min * 60) + l_sec;
		if (sz_time[0] == '-')
		{
			d_time = -d_time;
		}
		if (k < 2)
		{
			/*----- ���f�B�l�X�`�F�b�N���� -----*/
			l = (stpd.l_rdychk >> k) % 2;
			if (l == 0)
			{
				continue;
			}
		}
		/*----- �����ݒ菈�� -----*/
		if (k == 2)
		{
			if (stpd.l_init == 0)
			{
				continue;
			}
		}
		if (l_ncount >= 0)
		{
			if (d_ntime[l_ncount] > d_time)
			{
				d_ntime[l_ncount] = d_time;
			}
		}
	}

	TCHAR sz_lwork[50];
	swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s"), (LPCTSTR)stpd.sz_start);
	d_start = CSatelliteData::StrTimeToCTime(CString(sz_lwork)).GetTime();
	for (k = 0; k < 9; ++k)
	{
		d_ntime[k + 1] += d_ntime[k];
	}

	fclose(fp_smpl);


	if (_wfopen_s(&fp_smpl, sz_s_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("�v�搗�`�t�@�C���J�����Ƃ��ł��܂���B\n%s"), (LPCTSTR)sz_s_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	if (_wfopen_s(&fp_plan, sz_p_file, _T("wb")) != 0)
	{
		CString mes;
		mes.Format(_T("�v��t�@�C���J�����Ƃ��ł��܂���B\n%s"), (LPCTSTR)sz_p_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		fclose(fp_smpl);
		return -1;
	}

	/*---------- �R�����g�p�v����e�t�@�C���o�͏��� ----------*/

	TCHAR szwline[2048];
	CStringA cbuf;
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_smpl) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			if (wcsncmp(sz_buff, SOS_HEADER_ID, wcslen(SOS_HEADER_ID)) == 0)
			{
#if 1	// UTC
				SYSTEMTIME timeSys;
				GetSystemTime(&timeSys);
				CTime time = CTime(timeSys);

				CTime timegm(CTime::GetCurrentTime());
				CString str_gm = timegm.Format(_T("%Y-%m-%d %H:%M:%S"));
#else
				CTime time(CTime::GetCurrentTime());
#endif
				CString str_date = time.Format(_T("%Y-%m-%d %H:%M:%S"));

				sz_date[19] = '\0';
				wsprintf(szwline, _T("%s PLAN %s     1 %2d %s %s\n"),
					SOS_HEADER_ID,
					(LPCTSTR)CString(str_date),
					0,												// 0 �Œ�
					(LPCTSTR)CString(sz_satname).MakeUpper(),
					_T(""));										// �\��l����
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# �q����=%s �p�X�ԍ�=%s RDYCHK=%s\n"),
					(LPCTSTR)CString(sz_satname).MakeUpper(),
					(LPCTSTR)CString(sz_passno),
					(LPCTSTR)sz_rdychk[stpd.l_rdychk]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele64[2], (LPCTSTR)sz_on[stpd.l_init],
					(LPCTSTR)sz_c_ele64[3], (LPCTSTR)stpd.sz_s_bitrate,
					(LPCTSTR)sz_c_ele64[4], (LPCTSTR)stpd.sz_x_bitrate,
					(LPCTSTR)sz_c_ele64[17], (LPCTSTR)stpd.sz_x_qpsk);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				if (stpd.l_mod_sel == 0)
				{
					wsprintf(szwline, _T("# %s=%s %s=%s %s=%s\n"),
						(LPCTSTR)sz_c_ele64[5], (LPCTSTR)stpd.sz_cmd_bit_rate,
						(LPCTSTR)sz_c_ele64[12], (LPCTSTR)stpd.sz_start,
						(LPCTSTR)sz_c_ele64[13], (LPCTSTR)stpd.sz_end);
					cbuf = CServerFile::ConvertEuc(szwline);
					fputs(cbuf, fp_plan);
				}
				else if (stpd.l_mod_sel == 1)
				{
					wsprintf(szwline, _T("# %s=%s %s=%s %s=%s\n"),
						(LPCTSTR)sz_c_ele64[36], (LPCTSTR)stpd.sz_x_cmd_bit_rate,
						(LPCTSTR)sz_c_ele64[12], (LPCTSTR)stpd.sz_start,
						(LPCTSTR)sz_c_ele64[13], (LPCTSTR)stpd.sz_end);
					cbuf = CServerFile::ConvertEuc(szwline);
					fputs(cbuf, fp_plan);
				}

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele64[16], (LPCTSTR)sz_on[stpd.l_auto_acq],
					(LPCTSTR)sz_c_ele64[11], (LPCTSTR)sz_on[stpd.l_offset],
					(LPCTSTR)sz_c_ele64[14], (LPCTSTR)sz_on[stpd.l_a_flag],
					(LPCTSTR)sz_c_ele64[15], (LPCTSTR)sz_on[stpd.l_l_flag],
					(LPCTSTR)sz_c_ele64[18], (LPCTSTR)sz_term_ant[stpd.l_term_ant]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele64[6], (LPCTSTR)sz_auto[stpd.l_cmd_mod],
					(LPCTSTR)sz_c_ele64[7], (LPCTSTR)sz_auto[stpd.l_rng_mod],
					(LPCTSTR)sz_c_ele64[8], (LPCTSTR)stpd.sz_rng_band,
					(LPCTSTR)sz_c_ele64[9], (LPCTSTR)stpd.sz_rng_start,
					(LPCTSTR)sz_c_ele64[10], (LPCTSTR)stpd.sz_rng_end);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%d\n"),
					(LPCTSTR)sz_c_ele64[19], (LPCTSTR)sz_tx_ramp[stpd.l_tx_ramp],
					(LPCTSTR)sz_c_ele64[20], (LPCTSTR)sz_rx_ramp[stpd.l_rx_ramp],
					(LPCTSTR)sz_c_ele64[21], stpd.l_acq_route + 1);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%d %s=%d %s=%s\n"),
					(LPCTSTR)sz_c_ele64[22], (LPCTSTR)sz_on_off[stpd.l_pl_sel],
					(LPCTSTR)sz_c_ele64[23], (LPCTSTR)sz_on_off[stpd.l_top_mon],
					(LPCTSTR)sz_c_ele64[24], stpd.l_ent_angle,
					(LPCTSTR)sz_c_ele64[25], stpd.l_ext_angle,
					(LPCTSTR)sz_c_ele64[33], (LPCTSTR)sz_delay_sel[stpd.l_delay_sel]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele64[47], (LPCTSTR)stpd.sz_cid,								// CID(CID)
					(LPCTSTR)sz_c_ele64[48], mStationSimpleString[theApp.GetSelectStation()],	// �ǖ�(ST_NAME)
					(LPCTSTR)sz_c_ele64[49], _T("MASTER"),										// ��]��(PRI):MASTER�Œ�
					(LPCTSTR)sz_c_ele64[50], _T(""));							// �^�p���[�h(OPE_MODE)
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s\n"),
					(LPCTSTR)sz_c_ele64[61], _T(""));											// Auto
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				continue;
			}
			cbuf = CServerFile::ConvertEuc(sz_buff);
			fputs(cbuf, fp_plan);
			continue;
		}

		/*---------- �v����e�쐬���� ----------*/

		l = 0;
		if (sz_buff[0] == 'N')
		{
			if (sz_buff[1] >= 0x30 && sz_buff[1] <= 0x39)
			{
				l_ncount = sz_buff[1] - 0x30;
				k = 2;
			}
			else
			{
				l_ncount = 0;
				k = 1;
			}
		}
		else
		{
			l_ncount = -1;
			k = 0;
		}
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] != ' ') {
				sz_time[l] = sz_buff[k];
				l++;
			}
			else
			{
				if (l > 0)
				{
					sz_time[l] = '\0';
					break;
				}
			}
		}

		j = 0;
		l = 0;
		sz_ele[2][0] = '\0';
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] == ',')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_buff[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				sz_ele[j][l] = '\0';
				l = 0;
				break;
			}
			else if (sz_buff[k] != ' ')
			{
				sz_ele[j][l] = sz_buff[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			cbuf = CServerFile::ConvertEuc(sz_buff);
			fputs(cbuf, fp_plan);
			continue;
		}

		for (k = 0; sz_c_ele64[k] != _T(""); ++k)
		{
			if (sz_c_ele64[k] == CString(sz_ele[0]))
			{
				break;
			}
		}

		if (k == 12)
		{
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)stpd.sz_start);
		}
		else if (k == 13)
		{
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)stpd.sz_end);
		}
		else
		{
			m = swscanf_s(sz_time, _T("%d:%d:%d%s"), &l_hour, &l_min, &l_sec, sz_work, static_cast<unsigned>(_countof(sz_work)));
			if (m != 3)
			{
				if (sz_c_ele64[k] == _T(""))
				{
					cbuf = CServerFile::ConvertEuc(sz_buff);
					fputs(cbuf, fp_plan);
					continue;
				}
				fclose(fp_smpl);
				fclose(fp_plan);
				return -1;
			}
			d_time = (l_hour * 3600) + (l_min * 60) + l_sec;
			if (sz_time[0] == '-')
			{
				d_time = -d_time;
			}
			if (l_ncount >= 0)
			{
				d_time = d_ntime[l_ncount];
			}
			d_time += d_start;
			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';
		}

		switch (k)
		{
		case 30:
			if (wcscmp(sz_ele[1], _T("ANT")) == 0)
			{
				d_time = d_start - (stpd.l_ant_time * 60);
				if (stpd.l_antwait_flg == 0)
				{
					d_time -= 2;
				}
				CTime time(d_time);
				swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
				sz_date[19] = '\0';
			}
			break;

		case 41: //�A���e�i�҂��󂯎w���@
		{
			if (stpd.l_antwait_flg == 1)
			{
				d_time = d_start - (stpd.l_ant_time * 60);
				d_time = d_time + (5 * 60); //�A���e�i�҂��󂯒ʒm�̂T����
				CTime time(d_time);
				swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
				sz_date[19] = '\0';
			}
			else
			{
				continue;
			}
		}
		break;
		}

		wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, sz_ele[0], sz_ele[1], sz_ele[2]);
		cbuf = CServerFile::ConvertEuc(szwline);
		fputs(cbuf, fp_plan);

		/* �ŏ��̈ꌏ�͖������ŃR�s�[ */
		if (sz_s_start[0] == '\0')
		{
			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
		}
		else
		{
			if (wcscmp(sz_s_start, sz_date) > 0)
			{
				wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			}
		}
	}

	// �R�����g�o��
	if (comment != _T(""))
	{
		fputs("\n", fp_plan);

		comment.Replace(_T("\r\n"), _T("\\"));

		int i = 0;
		CString cItem;
		while (AfxExtractSubString(cItem, comment, i++, '\\'))
		{
			wsprintf(szwline, _T("# COMMENT=%s\n"), (LPCTSTR)cItem);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);
		}
	}

	fclose(fp_plan);
	fclose(fp_smpl);

	return 0;
}

/*============================================================================*/
/*! COprPlnData

-# �v��t�@�C������������

@brief  RDYCHK�p
@param  �Ȃ�
@retval TRUE�F����I��/FALSE�F�ُ�I��
*/
/*============================================================================*/
int COprPlnData::WriteRdyChkPlanFile(CString sz_satname, long satno, CString sz_passno, stPlandata stpd, vector<stCalibPlan>& agclist, vector<stCalibPlan>& tlmlist, vector<stCalibPlan>& cmdlist)
{
	//------------------------------------------------------
	// ���f�B�l�X�`�F�b�N�v��o�^�O�̎��ԃ`�F�b�N
	//------------------------------------------------------

	// UTC���ݎ��Ԏ擾
	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	CTime time = CTime(timeSys);
	__int64 d_timeCurrent = time.GetTime();
	CString str_date = time.Format(_T("%Y-%m-%d %H:%M:%S"));

	// �J�n�����A�I�������擾
	__int64 d_timeStart = CSatelliteData::StrTimeToCTime(CString(stpd.sz_start)).GetTime() + stpd.l_offset_val;
	__int64 d_timeEnd = CSatelliteData::StrTimeToCTime(CString(stpd.sz_end)).GetTime() + stpd.l_offset_val;

	// �v�掞���@���@���ݎ��� 
	if (d_timeStart < d_timeCurrent + START_OFFSET)
	{
		CString mes = _T("");
		mes.Format(_T("���f�B�l�X�`�F�b�N�v��̊J�n�����܂łɎ��Ԃ̗]�T���Ȃ��̂Ōv���o�^�ł��܂���B\n\n �v��J�n���� :%s\n �v��I������ :%s\n\n ���ݎ���       :%s\n �K�[�h�^�C�� :%d�b")
			, (LPCTSTR)stpd.sz_start, (LPCTSTR)stpd.sz_end, (LPCTSTR)str_date, (int)START_OFFSET);

		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
		return -1;
	}

	// �J�n�����@���@�I�������@
	if (d_timeStart > d_timeEnd)
	{
		CString mes = _T("");
		mes.Format(_T("���f�B�l�X�`�F�b�N�v��̏I���������J�n���������ߋ��̂��ߌv���o�^�ł��܂���B\n\n �v��J�n���� :%s\n �v��I������ :%s")
			, (LPCTSTR)stpd.sz_start, (LPCTSTR)stpd.sz_end);

		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
		return -1;
	}
	//------------------------------------------------------

	FILE *fp_plan;
	CString sz_p_folder;
	CString sz_p_file;

	TCHAR sz_ele[10][100];
	TCHAR sz_date[50];

	long k;

	__int64 d_s_start;
	__int64 d_ntime[10];

	int count = 0;
	int t_rdy = 0;

	memset(&sz_ele[0], 0, sizeof(sz_ele));

	/*----- �v��t�@�C���̃p�X���쐬 -----*/
	CSatelliteData& sd = theApp.GetSatelliteData();

	CString filenamep = theApp.GetShareFilePath(eFileType_PlanOut_Folder, theApp.GetSelectStation());
	sz_p_file.Format(_T("%s%s\\%s.pln"), (LPCTSTR)filenamep, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	sz_p_folder.Format(_T("%s%s"), (LPCTSTR)filenamep, (LPCTSTR)sz_satname.MakeLower());
	if (PathFileExists(sz_p_folder) == FALSE)
	{
		CreateDirectory(sz_p_folder, NULL);
	}

	/*---------- �J�n�����R�����g�p�v����e�t�@�C���o�͏��� ----------*/
	d_s_start = 0;
	for (k = 0; k < 10; k++)
	{
		d_ntime[k] = 0;
	}

	if (_wfopen_s(&fp_plan, sz_p_file, _T("wb")) != 0)
	{
		CString mes;
		mes.Format(_T("�v��t�@�C���J�����Ƃ��ł��܂���B\n%s"), (LPCTSTR)sz_p_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	/*---------- �R�����g�p�v����e�t�@�C���o�͏��� ----------*/

	TCHAR szwline[2048];
	CStringA cbuf;

	sz_date[19] = '\0';
	wsprintf(szwline, _T("%s RDYCHK %s     1 %2d %s %s\n"),
		SOS_HEADER_ID,
		(LPCTSTR)CString(str_date),
		satno,
		(LPCTSTR)CString(sz_satname).MakeUpper(),
		_T(""));										// �\��l����
	cbuf = CServerFile::ConvertEuc(szwline);
	fputs(cbuf, fp_plan);

	wsprintf(szwline, _T("# �q����=%s �p�X�ԍ�=%s RDYCHK=%s\n"),
		(LPCTSTR)CString(sz_satname).MakeUpper(),
		(LPCTSTR)CString(sz_passno),
		(LPCTSTR)sz_rdychk[stpd.l_rdychk]);
	cbuf = CServerFile::ConvertEuc(szwline);
	fputs(cbuf, fp_plan);

	wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s\n"),
		(LPCTSTR)sz_c_ele64[2], (LPCTSTR)sz_on[stpd.l_init],
		(LPCTSTR)sz_c_ele64[3], (LPCTSTR)stpd.sz_s_bitrate,
		(LPCTSTR)sz_c_ele64[4], (LPCTSTR)stpd.sz_x_bitrate,
		(LPCTSTR)sz_c_ele64[17], (LPCTSTR)stpd.sz_x_qpsk);
	cbuf = CServerFile::ConvertEuc(szwline);
	fputs(cbuf, fp_plan);

	if (stpd.l_mod_sel == 0)
	{
		wsprintf(szwline, _T("# %s=%s %s=%s %s=%s\n"),
			(LPCTSTR)sz_c_ele64[5], (LPCTSTR)stpd.sz_cmd_bit_rate,
			(LPCTSTR)sz_c_ele64[12], (LPCTSTR)stpd.sz_start,
			(LPCTSTR)sz_c_ele64[13], (LPCTSTR)stpd.sz_end);
		cbuf = CServerFile::ConvertEuc(szwline);
		fputs(cbuf, fp_plan);
	}
	else if (stpd.l_mod_sel == 1)
	{
		wsprintf(szwline, _T("# %s=%s %s=%s %s=%s\n"),
			(LPCTSTR)sz_c_ele64[36], (LPCTSTR)stpd.sz_x_cmd_bit_rate,
			(LPCTSTR)sz_c_ele64[12], (LPCTSTR)stpd.sz_start,
			(LPCTSTR)sz_c_ele64[13], (LPCTSTR)stpd.sz_end);
		cbuf = CServerFile::ConvertEuc(szwline);
		fputs(cbuf, fp_plan);
	}

	wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s %s=%s\n"),
		(LPCTSTR)sz_c_ele64[16], (LPCTSTR)sz_on[stpd.l_auto_acq],
		(LPCTSTR)sz_c_ele64[11], (LPCTSTR)sz_on[stpd.l_offset],
		(LPCTSTR)sz_c_ele64[14], (LPCTSTR)sz_on[stpd.l_a_flag],
		(LPCTSTR)sz_c_ele64[15], (LPCTSTR)sz_on[stpd.l_l_flag],
		(LPCTSTR)sz_c_ele64[18], (LPCTSTR)sz_term_ant[stpd.l_term_ant]);
	cbuf = CServerFile::ConvertEuc(szwline);
	fputs(cbuf, fp_plan);

	wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s %s=%s\n"),
		(LPCTSTR)sz_c_ele64[6], (LPCTSTR)sz_auto[stpd.l_cmd_mod],
		(LPCTSTR)sz_c_ele64[7], (LPCTSTR)sz_auto[stpd.l_rng_mod],
		(LPCTSTR)sz_c_ele64[8], (LPCTSTR)stpd.sz_rng_band,
		(LPCTSTR)sz_c_ele64[9], (LPCTSTR)stpd.sz_rng_start,
		(LPCTSTR)sz_c_ele64[10], (LPCTSTR)stpd.sz_rng_end);
	cbuf = CServerFile::ConvertEuc(szwline);
	fputs(cbuf, fp_plan);

	wsprintf(szwline, _T("# %s=%s %s=%s %s=%d\n"),
		(LPCTSTR)sz_c_ele64[19], (LPCTSTR)sz_tx_ramp[stpd.l_tx_ramp],
		(LPCTSTR)sz_c_ele64[20], (LPCTSTR)sz_rx_ramp[stpd.l_rx_ramp],
		(LPCTSTR)sz_c_ele64[21], stpd.l_acq_route + 1);
	cbuf = CServerFile::ConvertEuc(szwline);
	fputs(cbuf, fp_plan);

	wsprintf(szwline, _T("# %s=%s %s=%s %s=%d %s=%d %s=%s\n"),
		(LPCTSTR)sz_c_ele64[22], (LPCTSTR)sz_on_off[stpd.l_pl_sel],
		(LPCTSTR)sz_c_ele64[23], (LPCTSTR)sz_on_off[stpd.l_top_mon],
		(LPCTSTR)sz_c_ele64[24], stpd.l_ent_angle,
		(LPCTSTR)sz_c_ele64[25], stpd.l_ext_angle,
		(LPCTSTR)sz_c_ele64[33], (LPCTSTR)sz_delay_sel[stpd.l_delay_sel]);
	cbuf = CServerFile::ConvertEuc(szwline);
	fputs(cbuf, fp_plan);

	wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s\n"),
		(LPCTSTR)sz_c_ele64[47], (LPCTSTR)stpd.sz_cid,								// CID(CID)
		(LPCTSTR)sz_c_ele64[48], mStationSimpleString[theApp.GetSelectStation()],	// �ǖ�(ST_NAME)
		(LPCTSTR)sz_c_ele64[49], _T("MASTER"),										// ��]��(PRI):MASTER�Œ�
		(LPCTSTR)sz_c_ele64[50], _T(""));											// �^�p���[�h(OPE_MODE)
	cbuf = CServerFile::ConvertEuc(szwline);
	fputs(cbuf, fp_plan);

	wsprintf(szwline, _T("# %s=%s\n"),
		(LPCTSTR)sz_c_ele64[61], _T(""));											// Auto
	cbuf = CServerFile::ConvertEuc(szwline);
	fputs(cbuf, fp_plan);


	/*---------- �v����e�쐬���� ----------*/
	fputs("\n", fp_plan);

	wsprintf(szwline, _T("#****************************************************************************#\n"));
	cbuf = CServerFile::ConvertEuc(szwline);
	fputs(cbuf, fp_plan);

	wsprintf(szwline, _T("# ���f�B�l�X�`�F�b�N\n"));
	cbuf = CServerFile::ConvertEuc(szwline);
	fputs(cbuf, fp_plan);

	wsprintf(szwline, _T("#****************************************************************************#\n"));
	cbuf = CServerFile::ConvertEuc(szwline);
	fputs(cbuf, fp_plan);


	CTime t_start = CSatelliteData::StrTimeToCTime(stpd.sz_start);
	CTimeSpan span;
	CString strTime = CString(stpd.sz_start);

	// AGC,Delay
	for (int i = 0; i < (int)agclist.size(); ++i)
	{
		strTime = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));

		wsprintf(szwline, _T("%s %s\n"), (LPCTSTR)strTime, (LPCTSTR)agclist[i].ctrl);
		cbuf = CServerFile::ConvertEuc(szwline);
		fputs(cbuf, fp_plan);

		span = CTimeSpan(0, 0, 0, agclist[i].time);
		t_start += span;
	}

	// TLM
	for (int i = 0; i < (int)tlmlist.size(); ++i)
	{
		strTime = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));

		wsprintf(szwline, _T("%s %s\n"), (LPCTSTR)strTime, (LPCTSTR)tlmlist[i].ctrl);
		cbuf = CServerFile::ConvertEuc(szwline);
		fputs(cbuf, fp_plan);

		span = CTimeSpan(0, 0, 0, tlmlist[i].time);
		t_start += span;
	}

	// CMD
	for (int i = 0; i < (int)cmdlist.size(); ++i)
	{
		strTime = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));

		wsprintf(szwline, _T("%s %s\n"), (LPCTSTR)strTime, (LPCTSTR)cmdlist[i].ctrl);
		cbuf = CServerFile::ConvertEuc(szwline);
		fputs(cbuf, fp_plan);

		span = CTimeSpan(0, 0, 0, cmdlist[i].time);
		t_start += span;
	}

	// P-END
	wsprintf(szwline, _T("%s P-END,,%s;\n"), (LPCTSTR)stpd.sz_end, (LPCTSTR)sz_satname.MakeUpper());
	cbuf = CServerFile::ConvertEuc(szwline);
	fputs(cbuf, fp_plan);

	fclose(fp_plan);

	return 0;
}

/*============================================================================*/
/*! COprPlnData

-# �v��t�@�C����Ǎ�

@brief  RDYCHK�p
@param  �Ȃ�
@retval TRUE�F����I��/FALSE�F�ُ�I��
*/
/*============================================================================*/
int COprPlnData::ReadRdyChkPlanFile(CString sz_satname, CString sz_passno, stPlandata& stpd, vector<stCalibPlan>& agclist, vector<stCalibPlan>& tlmlist, vector<stCalibPlan>& cmdlist)
{
	FILE	*fp_plan;
	CString sz_p_file;

	TCHAR sz_s_start[50];
	TCHAR sz_buff[512];
	TCHAR sz_work[256];
	TCHAR sz_date[50] = { 0 };
	TCHAR sz_time[50] = { 0 };
	//	TCHAR sz_txrfpwr[3][20];

	TCHAR sz_work2[256];
	TCHAR sz_work3[256];
	TCHAR sz_work4[256];
	TCHAR sz_work5[256];
	TCHAR sz_work6[256];
	TCHAR sz_work7[256];
	TCHAR sz_work8[256];

	long i;
	long j;
	long l;
	long k;
	long l_rc;

	__int64 d_s_start;
	__int64 d_time;

	CString str;
	CString sz_txcomp;
	CString strTime;

	TCHAR sz_ele[10][100];
	memset(&sz_ele[0], 0, static_cast<unsigned>(_countof(sz_ele)));

	CSatelliteData& sd = theApp.GetSatelliteData();

	CString filename = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
	sz_p_file.Format(_T("%ssttn\\%s\\%s.pln"), (LPCTSTR)filename, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	sz_s_start[0] = '\0';

	if (_wfopen_s(&fp_plan, sz_p_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("�v��t�@�C�����J�����Ƃ��ł��܂���B\n%s"), (LPCTSTR)sz_p_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	stpd.sz_start = _T("");
	stpd.sz_end = _T("");
	stpd.sz_s_bitrate = _T("");
	stpd.sz_x_bitrate = _T("");
	stpd.sz_x_qpsk = _T("");
	stpd.sz_cmd_bit_rate = _T("");
	stpd.sz_x_cmd_bit_rate = _T("");
	stpd.l_cmd_mod = 0;
	stpd.l_rng_mod = 0;
	stpd.sz_rng_band = _T("");
	stpd.sz_rng_start = _T("");
	stpd.sz_rng_end = _T("");
	stpd.l_rdychk = 0;
	stpd.l_init = 0;
	stpd.l_auto_acq = 0;
	stpd.l_offset = 0;
	stpd.l_a_flag = 0;
	stpd.l_l_flag = 0;
	stpd.l_term_ant = 1;
	stpd.l_tx_ramp = 0;
	stpd.l_rx_ramp = 0;
	stpd.l_acq_route = 0;
	stpd.l_pl_sel = 0;
	stpd.l_top_mon = 0;
	stpd.l_ent_angle = 0;
	stpd.l_ext_angle = 0;
	stpd.sz_level_s = _T("");
	stpd.sz_level_x = _T("");
	stpd.sz_ant_s = _T("");
	stpd.sz_ant_x = _T("");
	stpd.l_delay_sel = 0;
	stpd.l_tx_pwr_sel = 0;
	stpd.l_mod_sel = 0;
	stpd.l_tx_uc_sel = -1;
	stpd.l_antwait_flg = 0;
	stpd.l_cmd_index = 0;
	stpd.l_tlm_index = 0;
	stpd.l_agc_index = 0;
	stpd.l_rng_index = 0;
	stpd.l_range_band = 0;
	stpd.l_ant_stow = 2;
	stpd.sz_cid = _T("");
	stpd.sz_station = _T("");
	stpd.sz_priority = _T("");
	stpd.sz_ope_mode = _T("");
	stpd.l_uplink = 0;
	stpd.sz_uplink_s_timelst.clear();
	stpd.sz_uplink_e_timelst.clear();
	stpd.l_rng = 0;
	stpd.sz_rng_s_timelst.clear();
	stpd.sz_rng_e_timelst.clear();
	stpd.l_cmd = 0;
	stpd.sz_cmd_s_timelst.clear();
	stpd.sz_cmd_e_timelst.clear();
	stpd.l_tlms = 0;
	stpd.sz_tlms_s_timelst.clear();
	stpd.sz_tlms_e_timelst.clear();
	stpd.l_tlmx = 0;
	stpd.sz_tlmx_s_timelst.clear();
	stpd.sz_tlmx_e_timelst.clear();
	stpd.l_tlmka = 0;
	stpd.sz_tlmka_s_timelst.clear();
	stpd.sz_tlmka_e_timelst.clear();

	d_s_start = 0;
	d_time = 0;

	char sz_line[512];
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_plan) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			if (wcsncmp(sz_buff, SOS_HEADER_ID, wcslen(SOS_HEADER_ID)) == 0)
			{
				swscanf_s(sz_buff, _T("%s%s%s%s%s%s%s%s"),
					sz_work, static_cast<unsigned>(_countof(sz_work)),
					sz_work2, static_cast<unsigned>(_countof(sz_work2)),
					sz_work3, static_cast<unsigned>(_countof(sz_work3)),
					sz_work4, static_cast<unsigned>(_countof(sz_work4)),
					sz_work5, static_cast<unsigned>(_countof(sz_work5)),
					sz_work6, static_cast<unsigned>(_countof(sz_work6)),
					sz_work7, static_cast<unsigned>(_countof(sz_work7)),
					sz_work8, static_cast<unsigned>(_countof(sz_work8)));
			}

			l_rc = swscanf_s(&sz_buff[1], _T("%s%s%s%s%s%s%s%s%s%s"),
				sz_ele[0], static_cast<unsigned>(_countof(sz_ele[0])),
				sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])),
				sz_ele[2], static_cast<unsigned>(_countof(sz_ele[2])),
				sz_ele[3], static_cast<unsigned>(_countof(sz_ele[3])),
				sz_ele[4], static_cast<unsigned>(_countof(sz_ele[4])),
				sz_ele[5], static_cast<unsigned>(_countof(sz_ele[5])),
				sz_ele[6], static_cast<unsigned>(_countof(sz_ele[6])),
				sz_ele[7], static_cast<unsigned>(_countof(sz_ele[7])),
				sz_ele[8], static_cast<unsigned>(_countof(sz_ele[8])),
				sz_ele[9], static_cast<unsigned>(_countof(sz_ele[9])));

			CString day;
			CString time;
			for (i = 0; i < l_rc; ++i)
			{
				str = CString(sz_ele[i]);

				/*---------- CID ----------*/
				if (str.Find(sz_c_ele64[47]) >= 0)
				{
					j = sz_c_ele64[47].GetLength() + 1;
					stpd.sz_cid = CString(&sz_ele[i][j]);
				}

				/*---------- P-START ----------*/
				if (str.Find(sz_c_ele64[12]) >= 0)
				{
					j = sz_c_ele64[12].GetLength() + 1;
					day = CString(&sz_ele[i][j]);
					time = CString(&sz_ele[i + 1][0]);
					stpd.sz_start.Format(_T("%s %s"), (LPCTSTR)day, (LPCTSTR)time);
				}

				/*---------- P-END ----------*/
				if (str.Find(sz_c_ele64[13]) >= 0)
				{
					j = sz_c_ele64[13].GetLength() + 1;
					day = CString(&sz_ele[i][j]);
					time = CString(&sz_ele[i + 1][0]);
					stpd.sz_end.Format(_T("%s %s"), (LPCTSTR)day, (LPCTSTR)time);
				}
			}
			continue;
		}

		swscanf_s(sz_buff, _T("%s%s%s"),
			sz_date, static_cast<unsigned>(_countof(sz_date)),
			sz_time, static_cast<unsigned>(_countof(sz_time)),
			sz_work, static_cast<unsigned>(_countof(sz_work)));

		j = 0;
		l = 0;
		for (k = 0; sz_work[k] != '\0'; ++k)
		{
			if (sz_work[k] == ',')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_work[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
				break;
			}
			else if (sz_work[k] != ' ')
			{
				sz_ele[j][l] = sz_work[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			continue;
		}

		str = CString(sz_ele[0]);

		for (k = 0; sz_c_ele64[k] != _T(""); ++k)
		{
			if ((k == 2) || (k == 4) || (k == 35))
			{	/* ����P�[�X��ǉ� */
				if (sz_c_ele64[k].Find(str) >= 0)
				{
					break;
				}
			}
			else
			{
				if (str == sz_c_ele64[k])
				{
					break;
				}
			}
		}

		if (sz_c_ele64[k] == _T(""))
		{
			continue;
		}

		swprintf_s(sz_work, static_cast<unsigned>(_countof(sz_work)), _T("%s %s"), sz_date, sz_time);
		if (d_s_start == 0.0 || d_s_start > d_time)
		{
			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), _T(" "));
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_time);
			d_s_start = d_time;
		}

		CString item;
		stCalibPlan st;
		switch (k)
		{
		case 37: //���f�B�l�X�`�F�b�N�iAGC)
			AfxExtractSubString(item, CString(sz_ele[1]), 0, '@');
			st.ctrl = item;
			if (AfxExtractSubString(item, CString(sz_ele[1]), 3, '@'))
			{
				// ���[�v�o���h������ꍇ
				st.loopband = item;
			}
			else
			{
				// ���[�v�o���h�������ꍇ
				st.loopband = _T("");
			}
			agclist.push_back(st);

			break;

		case 38: //���f�B�l�X�`�F�b�N�iRNG)
			AfxExtractSubString(item, CString(sz_ele[1]), 0, '@');
			st.ctrl = item;
			agclist.push_back(st);
			break;

		case 39: //���f�B�l�X�`�F�b�N�i�e�����g���`���j
			AfxExtractSubString(item, CString(sz_ele[1]), 0, '@');
			st.ctrl = item;
			tlmlist.push_back(st);

			break;

		case 40: //���f�B�l�X�`�F�b�N�i�R�}���h�`���j
			AfxExtractSubString(item, CString(sz_ele[1]), 0, '@');
			st.ctrl = item;
			cmdlist.push_back(st);

			break;
		}
	}

	fclose(fp_plan);

	return 0;
}
	
/*============================================================================*/
/*! COprPlnData

-# �v��t�@�C���o�^�v���iNEW�j

@param  �Ȃ�
@retval TRUE�F����I��/FALSE�F�ُ�I��
*/
/*============================================================================*/
int COprPlnData::RegistPlanNew(CString satname, CString passno, BOOL bNew)
{
	// �v��o�^�v��
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = REGIST_PLAN_SH;
	CString arg;

	//	arg.Format(_T("%s %s %s"), mStationSimpleString[theApp.GetSelectStation()], m_strSatellite, pid);
	if (bNew == TRUE)
	{
		arg.Format(_T("%s new %s sttn %s.pln"), (LPCTSTR)CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower(), (LPCTSTR)satname.MakeLower(), (LPCTSTR)passno);
	}
	else
	{
		arg.Format(_T("%s renew %s sttn %s.pln"), (LPCTSTR)CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower(), (LPCTSTR)satname.MakeLower(), (LPCTSTR)passno);
	}
	stParam.strRemShellParam.Format(arg);

	int err = CSshShell::ExecShell(stParam);
	if (err == 0)
	{
		AfxMessageBox(_T("�v��o�^�ɐ������܂����B"), MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		AfxMessageBox(_T("�v��o�^�Ɏ��s���܂����B"), MB_OK | MB_ICONWARNING);
	}

	return 0;
}

/* ------------------------------------------------------------------------------------ */
/* ���g�������ʃf�[�^�Ǘ��N���X
/* ------------------------------------------------------------------------------------ */
CFreqCtrlData::CFreqCtrlData()
{
}


CFreqCtrlData::~CFreqCtrlData()
{
}



/* ------------------------------------------------------------------------------------ */
/* ���M�����ʃf�[�^�Ǘ��N���X
/* ------------------------------------------------------------------------------------ */
CTrnCtrlData::CTrnCtrlData()
{
}


CTrnCtrlData::~CTrnCtrlData()
{
}



/* ------------------------------------------------------------------------------------ */
/* �����Ɩ�������ʃf�[�^�Ǘ��N���X
/* ------------------------------------------------------------------------------------ */
CBsnsDiaryData::CBsnsDiaryData()
{
}


CBsnsDiaryData::~CBsnsDiaryData()
{
}



/* ------------------------------------------------------------------------------------ */
/* CID�I����ʃf�[�^�Ǘ��N���X
/* ------------------------------------------------------------------------------------ */
CCIDSlctData::CCIDSlctData()
{
}


CCIDSlctData::~CCIDSlctData()
{
}

/*============================================================================*/
/*! CCIDSlctData

-# �q������CID���X�g�擾

@param  sat		�q����
@retval CID���X�g�ivector<CString>�j
*/
/*============================================================================*/
vector<CString> CCIDSlctData::GetCidList(CString sat)
{
	vector<CString> cidlist;

	for (auto itr = m_CidParamList.begin(); itr != m_CidParamList.end(); ++itr)
	{
		if (sat == itr->first)
		{
			map<CString, stCIDParam> &list = itr->second;

			for (auto itr2 = list.begin(); itr2 != list.end(); ++itr2)
			{
				cidlist.push_back(itr2->first);
			}
		}
	}

	return cidlist;
}

/*============================================================================*/
/*! CCIDSlctData

-# CID���疳��������(CID_INVALID)����

@param  stCid	����CID�p�����[�^
@retval st		�o��CID�p�����[�^
*/
/*============================================================================*/
stCIDParam CCIDSlctData::TrimInvalidString(stCIDParam stCid)
{
	stCIDParam st;
	(stCid.CID != CID_INVALID) ? st.CID = stCid.CID : st.CID = _T("");
	(stCid.TlmBitRateS != CID_INVALID) ? st.TlmBitRateS = stCid.TlmBitRateS : st.TlmBitRateS = _T("");
	(stCid.TlmBitRateX != CID_INVALID) ? st.TlmBitRateX = stCid.TlmBitRateX : st.TlmBitRateX = _T("");
	(stCid.TlmBitRateHX != CID_INVALID) ? st.TlmBitRateHX = stCid.TlmBitRateHX : st.TlmBitRateHX = _T("");
	(stCid.TlmBitRateKa != CID_INVALID) ? st.TlmBitRateKa = stCid.TlmBitRateKa : st.TlmBitRateKa = _T("");
	(stCid.CmdBitRateS != CID_INVALID) ? st.CmdBitRateS = stCid.CmdBitRateS : st.CmdBitRateS = _T("");
	(stCid.CmdBitRateX != CID_INVALID) ? st.CmdBitRateX = stCid.CmdBitRateX : st.CmdBitRateX = _T("");
	(stCid.TrsPower != CID_INVALID) ? st.TrsPower = stCid.TrsPower : st.TrsPower = _T("");
	(stCid.CarrLoopBandS != CID_INVALID) ? st.CarrLoopBandS = stCid.CarrLoopBandS : st.CarrLoopBandS = _T("");
	(stCid.CarrLoopBandX != CID_INVALID) ? st.CarrLoopBandX = stCid.CarrLoopBandX : st.CarrLoopBandX = _T("");
	(stCid.CarrLoopBandKa != CID_INVALID) ? st.CarrLoopBandKa = stCid.CarrLoopBandKa : st.CarrLoopBandKa = _T("");
	(stCid.UpLinkSweepShape != CID_INVALID) ? st.UpLinkSweepShape = stCid.UpLinkSweepShape : st.UpLinkSweepShape = _T("");
	(stCid.UpLinkSweepWidth != CID_INVALID) ? st.UpLinkSweepWidth = stCid.UpLinkSweepWidth : st.UpLinkSweepWidth = _T("");
	(stCid.UpLinkSweepTime != CID_INVALID) ? st.UpLinkSweepTime = stCid.UpLinkSweepTime : st.UpLinkSweepTime = _T("");
	(stCid.RngMesMode != CID_INVALID) ? st.RngMesMode = stCid.RngMesMode : st.RngMesMode = _T("");
	(stCid.RngMesSmplRate != CID_INVALID) ? st.RngMesSmplRate = stCid.RngMesSmplRate : st.RngMesSmplRate = _T("");
	(stCid.RngMesSmplItgTime != CID_INVALID) ? st.RngMesSmplItgTime = stCid.RngMesSmplItgTime : st.RngMesSmplItgTime = _T("");
	(stCid.RngMesSmplMod != CID_INVALID) ? st.RngMesSmplMod = stCid.RngMesSmplMod : st.RngMesSmplMod = _T("");
	(stCid.Modulation != CID_INVALID) ? st.Modulation = stCid.Modulation : st.Modulation = _T("");
	(stCid.ModulationS != CID_INVALID) ? st.ModulationS = stCid.ModulationS : st.ModulationS = _T("");
	(stCid.ModulationX != CID_INVALID) ? st.ModulationX = stCid.ModulationX : st.ModulationX = _T("");
	(stCid.CmdMod != CID_INVALID) ? st.CmdMod = stCid.CmdMod : st.CmdMod = _T("");
	(stCid.CmdSubCarrFreq != CID_INVALID) ? st.CmdSubCarrFreq = stCid.CmdSubCarrFreq : st.CmdSubCarrFreq = _T("");
	(stCid.TransDoppler != CID_INVALID) ? st.TransDoppler = stCid.TransDoppler : st.TransDoppler = _T("");
	(stCid.AntAutoTrack != CID_INVALID) ? st.AntAutoTrack = stCid.AntAutoTrack : st.AntAutoTrack = _T("");
	(stCid.AntAosPosn != CID_INVALID) ? st.AntAosPosn = stCid.AntAosPosn : st.AntAosPosn = _T("");
	(stCid.AntWaitPoint != CID_INVALID) ? st.AntWaitPoint = stCid.AntWaitPoint : st.AntWaitPoint = _T("");

	return st;
}

//#if 0
///* ------------------------------------------------------------------------------------ */
///* �g�����h�v���p�e�B�������}�b�v�f�[�^
///* ------------------------------------------------------------------------------------ */
//CTrendPropertiesMemMapData::CTrendPropertiesMemMapData(void)
//{
//	ClearParameter();
//}
//
//CTrendPropertiesMemMapData::~CTrendPropertiesMemMapData(void)
//{
//}
//
///*============================================================================*/
///*! �g�����h�v���p�e�B�������}�b�v�f�[�^
//
//-# �������}�b�v�f�[�^�t�@�C�����}�b�s���O
//
//@param	nMMDataMapView	�������}�b�v�f�[�^�t�@�C���}�b�v�r���[�̃T�C�Y
//@retval	bool	����
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapData::MappingUpdateMMDataFile(DWORD nMMDataMapView)
//{
//	LPSTR	pCur;
//	bool	bRes = true;
//
//	DWORD   cnt = 0;
//
//	// �������}�b�v�f�[�^�t�@�C���}�b�v�r���[���m�F
//	if ((pCur = (LPSTR)m_pMMDataMapView) != NULL)
//	{
//		DWORD nData = sizeof(stTrendPropiesMemData);
//		while (nMMDataMapView)
//		{
//			// CSpaceData�i�[���̃T�C�Y���m�F
//			if (nMMDataMapView < nData)
//			{
//				bRes = false;
//				break;
//			}
//
//			// �f�[�^�̐擪�|�C���^�����X�g�ɒǉ�
//			m_listMMDataMap.push_back(pCur);
//
//			// �|�C���^�ƃT�C�Y���X�V
//			pCur += nData;
//			nMMDataMapView -= nData;
//			++cnt;
//		}
//		m_nPropertiesDataCnt = cnt;
//	}
//
//	return bRes;
//}
//
///*============================================================================*/
///*! �g�����h�v���p�e�B�������}�b�v�f�[�^
//
//-# �������}�b�v�f�[�^�t�@�C���̃I�[�v��
//
//@param	flgRead		���[�h�I�[�v���t���O
//@retval	bool	����
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapData::OpenMMDataFile(bool flgRead/* = true*/)
//{
//	// �������}�b�v�f�[�^�t�@�C�������m�F
//	if (m_strMMDataFile.IsEmpty())
//		return false;
//
//	// �������}�b�v�f�[�^�t�@�C���n���h�����m�F
//	if (m_hMMDataFile != INVALID_HANDLE_VALUE)
//		return false;
//
//	TRY
//	{
//		CStringA	strMMDataFile = (CT2A)m_strMMDataFile;
//
//		// �������}�b�v�f�[�^�t�@�C�����I�[�v��
//		if (flgRead)
//			m_hMMDataFile = ::CreateFileA(strMMDataFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//		else
//			m_hMMDataFile = ::CreateFileA(strMMDataFile, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//		if (m_hMMDataFile == INVALID_HANDLE_VALUE)
//		{
//			CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapData"), _T("OpenMMDataFile"), _T("Error"), _T("CreateFileA error."), nLogEx::error);
//			AfxThrowUserException();
//		}
//
//		if (flgRead)
//		{
//			DWORD	dwLow, dwHigh;
//
//			// �t�@�C���T�C�Y���擾
//			dwLow = ::GetFileSize(m_hMMDataFile, &dwHigh);
//			if ((dwLow == -1) && (GetLastError() != NO_ERROR))
//			{
//				CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapData"), _T("OpenMMDataFile"), _T("Error"), _T("GetFileSize error."), nLogEx::error);
//				AfxThrowUserException();
//			}
//
//			// �t�@�C���}�b�s���O�I�u�W�F�N�g���쐬
//			m_hMMDataObject = ::CreateFileMapping(m_hMMDataFile, NULL, PAGE_READONLY, 0, dwLow, NULL);
//			if (m_hMMDataObject == NULL)
//			{
//				CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapData"), _T("OpenMMDataFile"), _T("Error"), _T("CreateFileMapping error."), nLogEx::error);
//				AfxThrowUserException();
//			}
//
//			// �A�h���X��ԂɃt�@�C���̃r���[���}�b�s���O
//			m_pMMDataMapView = ::MapViewOfFile(m_hMMDataObject, FILE_MAP_READ, 0, 0, dwLow);
//			if (m_pMMDataMapView == NULL)
//			{
//				CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapData"), _T("OpenMMDataFile"), _T("Error"), _T("MapViewOfFile error."), nLogEx::error);
//				AfxThrowUserException();
//			}
//
//			// �������}�b�v�f�[�^�t�@�C�����}�b�s���O
//			if (!MappingUpdateMMDataFile(dwLow))
//			{
//				CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapData"), _T("OpenMMDataFile"), _T("Error"), _T("MappingUpdateMMDataFile error."), nLogEx::error);
//				AfxThrowUserException();
//			}
//		}
//	}
//		CATCH_ALL(e)
//	{
//		CString	sError;
//		sError.Format(_T("GetLastError = %d"), ::GetLastError());
//		CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapData"), _T("OpenMMDataFile"), _T("Exception"), sError, nLogEx::error);
//
//		// �������}�b�v�f�[�^�}�b�v���X�g���폜
//		if (m_listMMDataMap.size() > 0)
//		{
//			vector<LPVOID>	tmp_listMMDataMap(0);
//
//			m_listMMDataMap.swap(tmp_listMMDataMap);
//
//			m_listMMDataMap.clear();
//		}
//
//		// �A�h���X��Ԃ̃t�@�C���r���[���A���}�b�v
//		if (m_pMMDataMapView != NULL)
//		{
//			::UnmapViewOfFile(m_pMMDataMapView);
//
//			m_pMMDataMapView = NULL;
//		}
//
//		// �t�@�C���}�b�s���O�I�u�W�F�N�g���N���[�Y
//		if (m_hMMDataObject != NULL)
//		{
//			::CloseHandle(m_hMMDataObject);
//
//			m_hMMDataObject = NULL;
//		}
//
//		// �������}�b�v�f�[�^�t�@�C�����N���[�Y
//		if (m_hMMDataFile != INVALID_HANDLE_VALUE)
//		{
//			::CloseHandle(m_hMMDataFile);
//
//			m_hMMDataFile = INVALID_HANDLE_VALUE;
//		}
//
//		return false;
//	}
//	END_CATCH_ALL
//
//		return true;
//}
//
///*============================================================================*/
///*! �g�����h�v���p�e�B�������}�b�v�f�[�^
//
//-# �������}�b�v�f�[�^�t�@�C���̃N���[�Y
//
//@param	�Ȃ�
//@retval	�Ȃ�
//
//*/
///*============================================================================*/
//void CTrendPropertiesMemMapData::CloseMMDataFile()
//{
//	// �������}�b�v�f�[�^�}�b�v���X�g���폜
//	if (m_listMMDataMap.size() > 0)
//	{
//		vector<LPVOID>	tmp_listMMDataMap(0);
//
//		m_listMMDataMap.swap(tmp_listMMDataMap);
//
//		m_listMMDataMap.clear();
//	}
//
//	// �A�h���X��Ԃ̃t�@�C���r���[���A���}�b�v
//	if (m_pMMDataMapView != NULL)
//	{
//		::UnmapViewOfFile(m_pMMDataMapView);
//
//		m_pMMDataMapView = NULL;
//	}
//
//	// �t�@�C���}�b�s���O�I�u�W�F�N�g���N���[�Y
//	if (m_hMMDataObject != NULL)
//	{
//		::CloseHandle(m_hMMDataObject);
//
//		m_hMMDataObject = NULL;
//	}
//
//	// �������}�b�v�f�[�^�t�@�C�����N���[�Y
//	if (m_hMMDataFile != INVALID_HANDLE_VALUE)
//	{
//		::CloseHandle(m_hMMDataFile);
//
//		m_hMMDataFile = INVALID_HANDLE_VALUE;
//	}
//}
//
///*============================================================================*/
///*! �g�����h�v���p�e�B�������}�b�v�f�[�^
//
//-# �f�[�^���������}�b�v�f�[�^�t�@�C���ɏ�������
//
//@param	propertiesData	�v���p�e�B�f�[�^
//@retval	bool			����
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapData::WriteMMDataFile(CPropertiesData& propertiesData)
//{
//	// �������}�b�v�f�[�^�t�@�C���n���h�����m�F
//	if (m_hMMDataFile == INVALID_HANDLE_VALUE)
//		return false;
//
//	TRY
//	{
//		stTrendPropiesMemData stData;
//		stData.wndIdx = propertiesData.wndIdx;
//		_tcscpy_s(stData.wndName, propertiesData.wndName);
//		stData.nGraphIdx = propertiesData.nGraphIdx;
//		stData.isRealTime = propertiesData.isRealTime;
//		stData.startTime = propertiesData.startTime;
//		stData.trandVScale = propertiesData.trandVScale;
//		for (int i = 0; i < MAX_HSCALE; i++)
//		{
//			stData.trandHScale[i] = propertiesData.trandHScale[i];
//		}
//		for (int i = 0; i < MAX_TDATA; i++)
//		{
//			_tcscpy_s(stData.dataName[i], propertiesData.dataName[i]);
//			_tcscpy_s(stData.dataGName[i], propertiesData.dataGName[i]);
//			_tcscpy_s(stData.dataUnit[i], propertiesData.dataUnit[i]);
//			stData.nDecDidits[i] = propertiesData.nDecDidits[i];
//			stData.isViewMode[i] = propertiesData.isViewMode[i];
//			stData.drawMode[i] = propertiesData.drawMode[i];
//			stData.color[i] = propertiesData.color[i];
//			stData.lineWidth[i] = propertiesData.lineWidth[i];
//			stData.dotSize[i] = propertiesData.dotSize[i];
//			stData.vscaleKind[i] = propertiesData.vscaleKind[i];
//			_tcscpy_s(stData.comment[i], propertiesData.comment[i]);
//			stData.isComment[i] = propertiesData.isComment[i];
//			stData.isAlarm[i] = propertiesData.isAlarm[i];
//			stData.alarm1Major[i] = propertiesData.alarm1Major[i];
//			stData.alarm1Minor[i] = propertiesData.alarm1Minor[i];
//			stData.alarm2Major[i] = propertiesData.alarm2Major[i];
//			stData.alarm2Minor[i] = propertiesData.alarm2Minor[i];
//		}
//
//		unsigned long	nWritten;
//		// propertiesData
//		if (::WriteFile(m_hMMDataFile, &stData, sizeof(stTrendPropiesMemData), &nWritten, NULL) == 0)
//			AfxThrowUserException();
//	}
//		CATCH_ALL(e)
//	{
//		CString	sError;
//		sError.Format(_T("GetLastError = %d"), ::GetLastError());
//		CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapData"), _T("WriteMMDataFile"), _T("Exception"), sError, nLogEx::error);
//
//		// �������}�b�v�f�[�^�t�@�C�����N���[�Y
//		if (m_hMMDataFile != INVALID_HANDLE_VALUE)
//		{
//			::CloseHandle(m_hMMDataFile);
//
//			m_hMMDataFile = INVALID_HANDLE_VALUE;
//		}
//
//		return false;
//	}
//	END_CATCH_ALL
//
//		return true;
//}
//
///*============================================================================*/
///*! �g�����h�v���p�e�B�������}�b�v�f�[�^
//
//-# �����h�v���p�e�B�f�[�^���������}�b�v�f�[�^�t�@�C������ǂݍ���
//
//@param	idx				�擾����f�[�^�̒ʂ��ԍ�
//@param	propertiesData	�f�[�^�̊i�[�A�h���X�ւ̎Q��
//@retval	bool			����
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapData::ReadMMDataFile(DWORD idx, CPropertiesData& propertiesData)
//{
//	LPSTR	pCur;
//
//	// �������}�b�v�f�[�^�t�@�C���n���h�����m�F
//	if (m_hMMDataFile == INVALID_HANDLE_VALUE)
//	{
//		CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapData"), _T("ReadMMDataFile"), _T("Error"), _T("File handle is none."), nLogEx::error);
//		return false;
//	}
//
//	// �������}�b�v�f�[�^�}�b�v���X�g���m�F
//	if (m_listMMDataMap.size() <= 0)
//	{
//		CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapData"), _T("ReadMMDataFile"), _T("Error"), _T("Map list is none."), nLogEx::error);
//		return false;
//	}
//
//	// �w�肳�ꂽ�ԍ��̃f�[�^�ւ̃|�C���^���擾
//	if ((pCur = (LPSTR)m_listMMDataMap[(UINT)(idx)]) == NULL)
//	{
//		CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapData"), _T("ReadMMDataFile"), _T("Error"), _T("Pointer is null."), nLogEx::error);
//		return false;
//	}
//
//	stTrendPropiesMemData& stData = *((stTrendPropiesMemData *)pCur);
//	propertiesData.wndIdx = stData.wndIdx;
//	propertiesData.wndName = stData.wndName;
//	propertiesData.nGraphIdx = stData.nGraphIdx;
//	propertiesData.isRealTime = stData.isRealTime;
//	propertiesData.startTime = stData.startTime;
//	propertiesData.trandVScale = stData.trandVScale;
//	for (int i = 0; i < MAX_HSCALE; i++)
//	{
//		propertiesData.trandHScale[i] = stData.trandHScale[i];
//	}
//	for (int i = 0; i < MAX_TDATA; i++)
//	{
//		propertiesData.dataName[i] = stData.dataName[i];
//		propertiesData.dataGName[i] = stData.dataGName[i];
//		propertiesData.dataUnit[i] = stData.dataUnit[i];
//		propertiesData.nDecDidits[i] = stData.nDecDidits[i];
//		propertiesData.isViewMode[i] = stData.isViewMode[i];
//		propertiesData.drawMode[i] = stData.drawMode[i];
//		propertiesData.color[i] = stData.color[i];
//		propertiesData.lineWidth[i] = stData.lineWidth[i];
//		propertiesData.dotSize[i] = stData.dotSize[i];
//		propertiesData.vscaleKind[i] = stData.vscaleKind[i];
//		propertiesData.comment[i] = stData.comment[i];
//		propertiesData.isComment[i] = stData.isComment[i];
//		propertiesData.isAlarm[i] = stData.isAlarm[i];
//		propertiesData.alarm1Major[i] = stData.alarm1Major[i];
//		propertiesData.alarm1Minor[i] = stData.alarm1Minor[i];
//		propertiesData.alarm2Major[i] = stData.alarm2Major[i];
//		propertiesData.alarm2Minor[i] = stData.alarm2Minor[i];
//	}
//
//	propertiesData.m_bSetted = TRUE;
//
//	return true;
//}
//
//
///* ------------------------------------------------------------------------------------ */
///* �g�����h�v���p�e�B�������}�b�v���X�g
///* ------------------------------------------------------------------------------------ */
//CTrendPropertiesMemMapList::CTrendPropertiesMemMapList(void)
//{
//	ClearParameter();
//}
//
//CTrendPropertiesMemMapList::~CTrendPropertiesMemMapList(void)
//{
//	DeleteMemMapList();
//}
//
///*============================================================================*/
///*! �g�����h�v���p�e�B�������}�b�v���X�g
//
//-# �������}�b�v���X�g�̍폜
//
//@param			�Ȃ�
//@retval	bool	����
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapList::DeleteMemMapList()
//{
//	vector<CTrendPropertiesMemMapData>	tmp_listMMData(0);
//
//	// �������}�b�v�f�[�^�t�@�C�����N���[�Y
//	for (auto itr = m_MapMMData.begin(); itr != m_MapMMData.end(); ++itr)
//	{
//		CString strKey = (CString)(itr->first);
//		CTrendPropertiesMemMapData *mmData = (CTrendPropertiesMemMapData*)(&itr->second);
//		mmData->CloseMMDataFile();
//	}
//
//	// �������}�b�v�f�[�^���X�g�����S�ɍ폜���邽�߃��[�J�����X�g�Ɍ������č폜
//	m_listMMData.swap(tmp_listMMData);
//
//	// �p�����[�^���N���A
//	ClearParameter();
//
//	return true;
//}
//
///*============================================================================*/
///*! �g�����h�v���p�e�B�������}�b�v���X�g
//
//-# �ݒ�f�[�^�ۑ��̊J�n����
//
//@param	�Ȃ�
//@retval	bool		����
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapList::WriteInitialization(CString strFileName)
//{
//	// �������}�b�v���X�g���폜
//	if (!DeleteMemMapList())
//	{
//		CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapList"), _T("WriteInitialization"), _T("Error"), _T("DeleteMemMapList failed."), nLogEx::error);
//		return false;
//	}
//
//	// �������}�b�v���t�@�C������p�����[�^��ݒ�
//	LoadMMInfoFile();
//
//	// �������}�b�v�f�[�^���쐬
//	CString	strMMDataFile;
//	CString strFName;
//	strFName = strFileName.Left(strFileName.GetLength() - 4);
//
//	// �������}�b�v�f�[�^�t�@�C�������쐬
//	CTrendPropertiesMemMapData	newMMData;
//	// �������}�b�v�f�[�^�t�@�C������ݒ�
//	newMMData.SetMMDataFileName(strFileName);
//
//	// �V�����������}�b�v�f�[�^�����X�g�ɒǉ�
//	m_MapMMData[strFName] = newMMData;
//
//	// �X�e�[�^�X���X�V
//	m_eMMListStatus = eMMListStatus_WriteOpen;
//
//	return true;
//}
//
///*============================================================================*/
///*! �g�����h�v���p�e�B�������}�b�v���X�g
//
//-# ����f�[�^�ۑ��̏I������
//
//@param	strFileName	�t�@�C����
//@retval	bool		����
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapList::WriteFinalization()
//{
//	for (auto itr = m_MapMMData.begin(); itr != m_MapMMData.end(); ++itr)
//	{
//		CString strKey = (CString)(itr->first);
//		CTrendPropertiesMemMapData *mmData = (CTrendPropertiesMemMapData*)(&itr->second);
//		mmData->CloseMMDataFile();
//	}
//
//	// �X�e�[�^�X���N���A
//	m_eMMListStatus = eMMListStatus_ReadOpen;	// �����ǂ߂��ԂȂ̂�
//
//	return true;
//}
//
///*============================================================================*/
///*! �g�����h�v���p�e�B�������}�b�v���X�g
//
//-# �f�[�^�̏�������
//
//@param	strFileName		�t�@�C����
//@param	propertiesdataList  �v���p�e�B�f�[�^���X�g
//@retval	bool			����
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapList::WriteData(CString strFileName, vector<CPropertiesData>& propertiesdataList)
//{
//	// �X�e�[�^�X���m�F
//	if (m_eMMListStatus != eMMListStatus_WriteOpen)
//	{
//		CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapList"), _T("WriteData"), _T("Error"), _T("Status is no read open."), nLogEx::error);
//		return false;
//	}
//
//	CString mapkey;
//	mapkey = strFileName.Left(strFileName.GetLength() - 4);
//
//	// �t�@�C���I�[�v��
//	CTrendPropertiesMemMapData& tmpMMData = m_MapMMData[mapkey];
//	if (!tmpMMData.IsOpenMMDataFile())
//	{
//		if (!tmpMMData.OpenMMDataFile(false))
//		{
//			CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapList"), _T("WriteData"), _T("Error"), _T("Open error."), nLogEx::error);
//			return false;
//		}
//	}
//
//	for (int i = 0; i < propertiesdataList.size(); i++)
//	{
//		// �f�[�^����������
//		if (!tmpMMData.WriteMMDataFile(propertiesdataList[i]))
//		{
//			CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteData"), _T("Error"), _T("Write error."), nLogEx::error);
//			return false;
//		}
//	}
//
//	// �t�@�C���N���[�Y
//	if (tmpMMData.IsOpenMMDataFile())
//	{
//		tmpMMData.CloseMMDataFile();
//	}
//
//	return true;
//}
//
//
///*============================================================================*/
///*! �g�����h�v���p�e�B�������}�b�v���X�g
//
//-# �ݒ�f�[�^�W�J�̊J�n����
//
//@param	strFileName	�t�@�C����
//@retval	bool		����
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapList::ReadInitialization(CString strFileName)
//{
//	// �������}�b�v���X�g���폜
//	if (!DeleteMemMapList())
//	{
//		CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapList"), _T("ReadInitialization"), _T("Error"), _T("DeleteMemMapList failed."), nLogEx::error);
//		return false;
//	}
//
//	// �������}�b�v���t�@�C������p�����[�^��ݒ�
//	LoadMMInfoFile();
//
//	// �������}�b�v�f�[�^���쐬
//	CString	strMMDataFile;
//	CString strFName;
//	strFName = strFileName.Left(strFileName.GetLength() - 4);
//
//	// �������}�b�v�f�[�^�t�@�C�������쐬
//	CTrendPropertiesMemMapData	newMMData;
//	// �������}�b�v�f�[�^�t�@�C������ݒ�
//	newMMData.SetMMDataFileName(strFileName);
//
//	// �V�����������}�b�v�f�[�^�����X�g�ɒǉ�
//	m_MapMMData[strFName] = newMMData;
//
//	// �X�e�[�^�X���X�V
//	m_eMMListStatus = eMMListStatus_ReadOpen;
//
//	return true;
//}
//
///*============================================================================*/
///*! �g�����h�v���p�e�B�������}�b�v���X�g
//
//-# �ݒ�f�[�^�W�J�̏I������
//
//@param
//@retval	bool		����
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapList::ReadFinalization()
//{
//	// �X�e�[�^�X���m�F
//	if (m_eMMListStatus != eMMListStatus_ReadOpen)
//	{
//		CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapList"), _T("ReadFinalization"), _T("Error"), _T("Status is no read open."), nLogEx::error);
//		return false;
//	}
//
//	// �������}�b�v�f�[�^�t�@�C�����N���[�Y
//	for (auto itr = m_MapMMData.begin(); itr != m_MapMMData.end(); ++itr)
//	{
//		CString strKey = (CString)(itr->first);
//		CTrendPropertiesMemMapData *mmData = (CTrendPropertiesMemMapData*)(&itr->second);
//		mmData->CloseMMDataFile();
//	}
//
//	// �X�e�[�^�X���N���A
//	m_eMMListStatus = eMMListStatus_ReadOpen;	// �����ǂ߂��ԂȂ̂�
//
//	return true;
//}
//
///*============================================================================*/
///*! �g�����h�v���p�e�B�������}�b�v���X�g
//
//-# �f�[�^�̓ǂݍ���
//
//@param	strFileName			�t�@�C����
//@param	propertiesdataList	�f�[�^�̊i�[�A�h���X���X�g�ւ̎Q��
//@retval	bool			����
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapList::ReadData(CString strFileName, vector<CPropertiesData>& propertiesdataList)
//{
//	// �X�e�[�^�X���m�F
//	if (m_eMMListStatus != eMMListStatus_ReadOpen)
//	{
//		CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapList"), _T("ReadData"), _T("Error"), _T("Status is no read open."), nLogEx::error);
//		return false;
//	}
//
//	CString mapkey;
//	mapkey = strFileName.Left(strFileName.GetLength() - 4);
//
//	// �t�@�C���I�[�v��
//	CTrendPropertiesMemMapData& tmpMMData = m_MapMMData[mapkey];
//	if (!tmpMMData.IsOpenMMDataFile())
//	{
//		if (!tmpMMData.OpenMMDataFile(true))
//		{
//			CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapList"), _T("ReadWaveData"), _T("Error"), _T("Open error."), nLogEx::error);
//			return false;
//		}
//	}
//
//	// �f�[�^��ǂݍ���
//	for (DWORD i = 0; i < tmpMMData.GetPropertiesDataCnt(); ++i)
//	{
//		if (!tmpMMData.ReadMMDataFile(i, propertiesdataList[i]))
//		{
//			CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapList"), _T("ReadData"), _T("Error"), _T("Read error."), nLogEx::error);
//			return false;
//		}
//	}
//
//	// �t�@�C���N���[�Y
//	if (tmpMMData.IsOpenMMDataFile())
//	{
//		tmpMMData.CloseMMDataFile();
//	}
//
//	return true;
//}
//#endif

/* ------------------------------------------------------------------------------------ */
/* �ǐ��f�[�^�Ǘ��N���X
/* ------------------------------------------------------------------------------------ */
CSatelliteData::CSatelliteData()
{
	for (int i = 0; i < eStation_MAX; i++){
		mGetSharePredFileTime[i] = CTime(0);
		mGetSharePlanFileTime[i] = CTime(0);
	}
}


CSatelliteData::~CSatelliteData()
{
}


/*============================================================================*/
/*! CSatelliteData

-# �����������ʎZ�b�ɂ���

@param  sz_time�F�t���t�H�[�}�b�g���͎���������
@retval	�ώZ�b

*/
/*============================================================================*/
double CSatelliteData::ub_asc_to_dtime(char* sz_time)
{
	int i;
	struct tm t;
	double d_sec, d_0sec;
	time_t ll_uttime;

	if ((i = sscanf_s(sz_time, "%d-%d-%d %d:%d:%lf", &t.tm_year, &t.tm_mon
		, &t.tm_mday, &t.tm_hour, &t.tm_min, &d_sec)) < 6)
	{
		return (-1.0);
	}

	if ((d_sec < 0.0) || (d_sec >= 60.0)){ return (-1.0); }
	t.tm_sec = (int)floor(d_sec);
	d_0sec = d_sec - t.tm_sec;
	if ((t.tm_mon  < 1) || (t.tm_mon  > 12)){ return (-1.0); }
	if ((t.tm_mday < 1) || (t.tm_mday > 31)){ return (-1.0); }
	if ((t.tm_hour < 0) || (t.tm_hour > 23)){ return (-1.0); }
	if ((t.tm_min  < 0) || (t.tm_min  > 59)){ return (-1.0); }
	if (t.tm_year < 70){ t.tm_year += 2000; }; /* yyyy == 00 --> 2000 */
	if (t.tm_year > 1900){ t.tm_year -= 1900; }; /* yyyy == 1970 --> 70 */
	t.tm_mon -= 1;  /* mon == 1..12 --> 0..11 */
	t.tm_isdst = 0; /* daylight flg off */
	if ((ll_uttime = mktime(&t)) == -1){
		return (-1.0);
	}

	return((double)ll_uttime + d_0sec);
}

/*============================================================================*/
/*! CSatelliteData

-# �ȗ��t�H�[�}�b�g�̎����������full-format�̎���������ɕϊ�

@param  sz_time�F���͎���������
@            1999-07-01 11:22:33 �̏ȗ��`�Ƃ��Ă͈ȉ��̌`��������
@            1999-07-01 11:22
@            1999-07-01 112233
@            1999-07-01 1122
@            1999-07-01 11
@                      .11
@                      .1122
@                      .112233
@                    01.112233
@                    01.1122
@                    01.11
@                  0701.112233
@                990701.112233
@              19990701.112233
@              19990701112233
@param	output_time:�t���t�H�[�}�b�g�o�͎���������
@retval	5 : yy-mm-dd hh:mm
@       6 : yy-mm-dd hh:mm:ss.s
@       7 :
@       1 : �ُ�I��

*/
/*============================================================================*/
int CSatelliteData::ub_asc_to_asctime(char sz_input_time[], char sz_output_time[])
{
	int i, j;
	int year, mon, mday, hour, min, sec, i_100usec;
	int i_hhmmss, i_yymmdd;
	char sz_time[40], sz_work[40], sz_dmy[40];
	char sz_yymmdd[40], sz_hhmmss[40], sz_0sec[40];
	double d_sec, d_hhmmss, d_yymmdd;
	struct tm stT;
	time_t l_time;
	int sz = 40;

	for (i = 0, sz_time[0] = NULL; sz_input_time[i] != NULL; i++){
		if (sz_input_time[i] == ' '){ continue; } /*�擪�X�y�[�X�T�v���X*/
		strcpy_s(sz_time, sz, &sz_input_time[i]);
		break;
	};
	if (sz_time[0] == NULL){
		return(-1);
	};

	for (i = (int)strlen(sz_time) - 1, j = 0; i > 0; i--){
		if ((j == 0) && (sz_time[i] == ' ')){
			sz_time[i] = NULL; j = i; /*���̃X�y�[�X�T�v���X*/
		}
		else if (sz_time[i] != ' '){
			j = i;
		}
	}

	if ((i = sscanf_s(sz_time, "%d-%d-%d %d:%d:%lf%s"
		, &year, &mon, &mday, &hour, &min, &d_sec, sz_dmy, sz)) == 6){
		sec = (int)floor(d_sec);
		i_100usec = (int)((d_sec - (double)sec)*10000.0);
		if (year < 70){
			year += 2000;
		}
		else if (year < 1900){
			year += 1900;
		}
		sprintf_s(sz_output_time, sz, "%04d-%02d-%02d %02d:%02d:%02d.%04d"
			, year, mon, mday, hour, min, sec, i_100usec);
		return(6); /* yy-mm-dd hh:mm:ss.s */
	}
	else if ((i = sscanf_s(sz_time, "%d-%d-%d %d:%d%s"
		, &year, &mon, &mday, &hour, &min, sz_dmy, sz)) == 5){
		if (year < 70){
			year += 2000;
		}
		else if (year < 1900){
			year += 1900;
		}
		sprintf_s(sz_output_time, sz, "%04d-%02d-%02d %02d:%02d:%02d.0000"
			, year, mon, mday, hour, min, 0);
		return(5); /* yy-mm-dd hh:mm */
	}
	else if ((i = sscanf_s(sz_time, "%d-%d-%d %lf%s"
		, &year, &mon, &mday, &d_hhmmss, sz_dmy, sz)) == 4){
		sscanf_s(sz_time, "%d-%d-%d %s", &year, &mon, &mday, sz_hhmmss, sz);
		if (sz_hhmmss[0] == '.'){
			sprintf_s(sz_yymmdd, sz, "%02d%02d%02d%s", year, mon, mday, sz_hhmmss);
		}
		else{
			sprintf_s(sz_yymmdd, sz, "%02d%02d%02d.%s", year, mon, mday, sz_hhmmss);
		}
		goto yymmdd_hhmmss_0sec; /* yy-mm-dd hhmmss */
	}
	else if (((i = sscanf_s(sz_time, "%lf%s", &d_yymmdd, sz_dmy, sz)) == 1)
		&& (d_yymmdd >= 0101000000.0)){ /* 000101=2000-01-01 */
		i_yymmdd = (int)floor(d_yymmdd / 1000000.0);
		i_hhmmss = (int)fmod(d_yymmdd, 1000000.0);
		if (sscanf_s(sz_time, "%[^.].%s", sz_yymmdd, sz, sz_0sec, sz) == 2){
			sprintf_s(sz_yymmdd, sz, "%06d.%06d.%s", i_yymmdd, i_hhmmss, sz_0sec);
		}
		else{
			sprintf_s(sz_yymmdd, sz, "%06d.%06d.0000", i_yymmdd, i_hhmmss);
		}
		goto yymmdd_hhmmss_0sec; /* yymmddhhmmss */
	}
	else if ((sz_time[0] == '.') && ((i = sscanf_s(sz_time + 1, "%lf%s", &d_hhmmss, sz_dmy, sz)) == 1)){
		sprintf_s(sz_yymmdd, sz, "00%s", sz_time);
		goto yymmdd_hhmmss_0sec; /* .hhmmss */
	}
	else if ((i = sscanf_s(sz_time, "%[^.].%lf%s", sz_work, sz, &d_hhmmss, sz_dmy, sz)) == 2){
		strcpy_s(sz_yymmdd, sz, sz_time);
		goto yymmdd_hhmmss_0sec; /* yymmdd.hhmmss */
	}
	else{

		strcpy_s(sz_output_time, sz, sz_time);
		return(-1);
	};

yymmdd_hhmmss_0sec:

	if (sscanf_s(sz_yymmdd, "%[^.].%[^.].%s", sz_dmy, sz, sz_hhmmss, sz, sz_0sec, sz) == 3){

		if (strlen(sz_hhmmss) != 6){
			strcpy_s(sz_output_time, sz, sz_time);
			return(-1);
		};
		strcat_s(sz_0sec, sz, "000000");
		sz_0sec[4] = NULL;
	}
	else{
		strcpy_s(sz_0sec, sz, "0000");

		switch (strlen(sz_hhmmss)){
		case 2: case 4: case 6:
			strcat_s(sz_hhmmss, sz, "000000");
			sz_hhmmss[6] = NULL;
			break;
		default: strcpy_s(sz_output_time, sz, sz_time);
			return(-1);
		};
	};

	l_time = time(NULL);
	if (localtime_s(&stT, &l_time) != 0)
	{
		return(1);
	}
	sscanf_s(sz_dmy, "%d", &i_yymmdd);
	if ((mday = i_yymmdd % 100) == 0){
		mday = stT.tm_mday; /* 1..31*/
	}
	if ((mon = (i_yymmdd / 100) % 100) == 0){
		mon = stT.tm_mon + 1; /* 0..11*/
	}
	if ((year = i_yymmdd / 10000) == 0){
		year = stT.tm_year; /* n -1900*/
	}

	switch (strlen(sz_dmy)){
	case 6: case 8:/* yymmdd */
		year = i_yymmdd / 10000;
		break;
	case 2: case 4:/* ????dd ??mmdd */
		break;
	default: strcpy_s(sz_output_time, sz, sz_time);
		return(-1);
	};
	sscanf_s(sz_hhmmss, "%d", &i_hhmmss);
	sec = i_hhmmss % 100;
	min = (i_hhmmss / 100) % 100;
	hour = i_hhmmss / 10000;

	if (year < 70){
		year += 2000;
	}
	else if (year < 1900){
		year += 1900;
	}

	sprintf_s(sz_output_time, sz, "%02d-%02d-%02d %02d:%02d:%02d.%s"
		, year, mon, mday, hour, min, sec, sz_0sec);

	return(7);
}


/*============================================================================*/
/*! CSatelliteData

-# �ȗ��t�H�[�}�b�g�̎����������full-format�̎���������ɕϊ�

@param  sz_time�F���͎���������
@            1999-07-01 11:22:33 �̏ȗ��`�Ƃ��Ă͈ȉ��̌`��������
@            1999-07-01 11:22
@            1999-07-01 112233
@            1999-07-01 1122
@            1999-07-01 11
@                      .11
@                      .1122
@                      .112233
@                    01.112233
@                    01.1122
@                    01.11
@                  0701.112233
@                990701.112233
@              19990701.112233
@              19990701112233
@param	output_time:�t���t�H�[�}�b�g�o�͎���������
@retval	5 : yy-mm-dd hh:mm
@       6 : yy-mm-dd hh:mm:ss.s
@       7 :
@       1 : �ُ�I��

*/
/*============================================================================*/
int CSatelliteData::ub_asc_to_asctime(TCHAR sz_input_time[], TCHAR sz_output_time[])
{
	int i, j;
	int year, mon, mday, hour, min, sec, i_100usec;
	int i_hhmmss, i_yymmdd;
	TCHAR sz_time[40], sz_work[40], sz_dmy[40];
	TCHAR sz_yymmdd[40], sz_hhmmss[40], sz_0sec[40];
	double d_sec, d_hhmmss, d_yymmdd;
	struct tm stT;
	time_t l_time;
	int sz = 40;

	for (i = 0, sz_time[0] = NULL; sz_input_time[i] != NULL; i++){
		if (sz_input_time[i] == ' '){ continue; } /*�擪�X�y�[�X�T�v���X*/
		wcscpy_s(sz_time, sz, &sz_input_time[i]);
		break;
	};
	if (sz_time[0] == NULL){
		return(-1);
	};

	for (i = (int)wcslen(sz_time) - 1, j = 0; i > 0; i--){
		if ((j == 0) && (sz_time[i] == ' ')){
			sz_time[i] = NULL; j = i; /*���̃X�y�[�X�T�v���X*/
		}
		else if (sz_time[i] != ' '){
			j = i;
		}
	}

	if ((i = swscanf_s(sz_time, L"%d-%d-%d %d:%d:%lf%s"
		, &year, &mon, &mday, &hour, &min, &d_sec, sz_dmy, sz)) == 6){
		sec = (int)floor(d_sec);
		i_100usec = (int)((d_sec - (double)sec)*10000.0);
		if (year < 70){
			year += 2000;
		}
		else if (year < 1900){
			year += 1900;
		}
		swprintf_s(sz_output_time, sz, L"%04d-%02d-%02d %02d:%02d:%02d.%04d"
			, year, mon, mday, hour, min, sec, i_100usec);
		return(6); /* yy-mm-dd hh:mm:ss.s */
	}
	else if ((i = swscanf_s(sz_time, L"%d-%d-%d %d:%d%s"
		, &year, &mon, &mday, &hour, &min, sz_dmy, sz)) == 5){
		if (year < 70){
			year += 2000;
		}
		else if (year < 1900){
			year += 1900;
		}
		swprintf_s(sz_output_time, sz, L"%04d-%02d-%02d %02d:%02d:%02d.0000"
			, year, mon, mday, hour, min, 0);
		return(5); /* yy-mm-dd hh:mm */
	}
	else if ((i = swscanf_s(sz_time, L"%d-%d-%d %lf%s"
		, &year, &mon, &mday, &d_hhmmss, sz_dmy, sz)) == 4){
		swscanf_s(sz_time, L"%d-%d-%d %s", &year, &mon, &mday, sz_hhmmss, sz);
		if (sz_hhmmss[0] == '.'){
			swprintf_s(sz_yymmdd, sz, L"%02d%02d%02d%s", year, mon, mday, sz_hhmmss);
		}
		else{
			swprintf_s(sz_yymmdd, sz, L"%02d%02d%02d.%s", year, mon, mday, sz_hhmmss);
		}
		goto yymmdd_hhmmss_0sec; /* yy-mm-dd hhmmss */
	}
	else if (((i = swscanf_s(sz_time, L"%lf%s", &d_yymmdd, sz_dmy, sz)) == 1)
		&& (d_yymmdd >= 0101000000.0)){ /* 000101=2000-01-01 */
		i_yymmdd = (int)floor(d_yymmdd / 1000000.0);
		i_hhmmss = (int)fmod(d_yymmdd, 1000000.0);
		if (swscanf_s(sz_time, L"%[^.].%s", sz_yymmdd, sz, sz_0sec, sz) == 2){
			swprintf_s(sz_yymmdd, sz, L"%06d.%06d.%s", i_yymmdd, i_hhmmss, sz_0sec);
		}
		else{
			swprintf_s(sz_yymmdd, sz, L"%06d.%06d.0000", i_yymmdd, i_hhmmss);
		}
		goto yymmdd_hhmmss_0sec; /* yymmddhhmmss */
	}
	else if ((sz_time[0] == '.') && ((i = swscanf_s(sz_time + 1, L"%lf%s", &d_hhmmss, sz_dmy, sz)) == 1)){
		swprintf_s(sz_yymmdd, sz, L"00%s", sz_time);
		goto yymmdd_hhmmss_0sec; /* .hhmmss */
	}
	else if ((i = swscanf_s(sz_time, L"%[^.].%lf%s", sz_work, sz, &d_hhmmss, sz_dmy, sz)) == 2){
		wcscpy_s(sz_yymmdd, sz, sz_time);
		goto yymmdd_hhmmss_0sec; /* yymmdd.hhmmss */
	}
	else{

		wcscpy_s(sz_output_time, sz, sz_time);
		return(-1);
	};

yymmdd_hhmmss_0sec:

	if (swscanf_s(sz_yymmdd, L"%[^.].%[^.].%s", sz_dmy, sz, sz_hhmmss, sz, sz_0sec, sz) == 3){

		if (wcslen(sz_hhmmss) != 6){
			wcscpy_s(sz_output_time, sz, sz_time);
			return(-1);
		};
		wcscat_s(sz_0sec, sz, L"000000");
		sz_0sec[4] = NULL;
	}
	else{
		wcscpy_s(sz_0sec, sz, L"0000");

		switch (wcslen(sz_hhmmss)){
		case 2: case 4: case 6:
			wcscat_s(sz_hhmmss, sz, L"000000");
			sz_hhmmss[6] = NULL;
			break;
		default: wcscpy_s(sz_output_time, sz, sz_time);
			return(-1);
		};
	};

	l_time = time(NULL);
	if (localtime_s(&stT, &l_time) != 0)
	{
		return(1);
	}
	swscanf_s(sz_dmy, L"%d", &i_yymmdd);
	if ((mday = i_yymmdd % 100) == 0){
		mday = stT.tm_mday; /* 1..31*/
	}
	if ((mon = (i_yymmdd / 100) % 100) == 0){
		mon = stT.tm_mon + 1; /* 0..11*/
	}
	if ((year = i_yymmdd / 10000) == 0){
		year = stT.tm_year; /* n -1900*/
	}

	switch (wcslen(sz_dmy)){
	case 6: case 8:/* yymmdd */
		year = i_yymmdd / 10000;
		break;
	case 2: case 4:/* ????dd ??mmdd */
		break;
	default: wcscpy_s(sz_output_time, sz, sz_time);
		return(-1);
	};
	swscanf_s(sz_hhmmss, L"%d", &i_hhmmss);
	sec = i_hhmmss % 100;
	min = (i_hhmmss / 100) % 100;
	hour = i_hhmmss / 10000;

	if (year < 70){
		year += 2000;
	}
	else if (year < 1900){
		year += 1900;
	}

	swprintf_s(sz_output_time, sz, L"%02d-%02d-%02d %02d:%02d:%02d.%s"
		, year, mon, mday, hour, min, sec, sz_0sec);

	return(7);
}


/*============================================================================*/
/*! CSatelliteData

-# ut ��������YYYY-MM-DD HH:MM:SS.ssss������ɕϊ�����B

@param  d_uttime�F�������
@param	sz_time:YYYY���i�[����̈�
@retval	5 : yy-mm-dd hh:mm
@       6 : yy-mm-dd hh:mm:ss.s
@       7 :
@       1 : �ُ�I��

*/
/*============================================================================*/
void CSatelliteData::ub_dtime_to_asc(double d_uttime, char* sz_time)
{ 
	time_t l_uttime,l_100usec;
	struct  tm t;
	long    l_yyyy,l_mm;
//#if 0
//	if( d_uttime <= -2147483648.0 ){ /* 0x80000000..0x7fffffff �͈̔̓`�F�b�N */ 
//		d_uttime = -2147483648.0;    /* �����integer�ϊ�overflow-FPE����������*/
//	}else if( d_uttime >=  2147483646.0 ){ /*���Ƃ�����邽�߂ł���B*/ 
//		d_uttime =  2147483646.0;          /* 1998-03-18 */
//	}
//#else
	if (d_uttime <= -(double)LONG64_MAX){ /* 0x80000000..0x7fffffff �͈̔̓`�F�b�N */
		d_uttime = -(double)LONG64_MAX;    /* �����integer�ϊ�overflow-FPE����������*/
	}
	else if (d_uttime >= (double)(LONG64_MAX - 2)){ /*���Ƃ�����邽�߂ł���B*/
		d_uttime = (double)(LONG64_MAX - 2);          /* 1998-03-18 */
	}
//#endif
	l_uttime = (time_t)d_uttime;
	l_100usec = (time_t)( d_uttime - ((double)l_uttime) + 0.00005 ) * 10000; /* 100usec */
	if( l_100usec >= 10000 ){ /* 0.99995 �� 1.00000 �ɂȂ� */
		l_100usec-=10000;
		l_uttime++;           /* �b�̌���␳ */
	}
	if (localtime_s(&t, &l_uttime) != 0)
		return;

	l_yyyy = t.tm_year + 1900;  /* 70 --> 1970 */ 
	l_mm   = t.tm_mon + 1;      /* 0..11 -> 1..12 */
	sprintf_s(sz_time, 30, "%04d-%02d-%02d %02d:%02d:%02d.%04d", l_yyyy
		,l_mm,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec,(int)l_100usec);
	return;
}

//#if 0
///*============================================================================*/
///*! CSatelliteData
//
//-# �������r
//��������r����B��r���Ƀ��C���h�J�[�h��������g�p���邱�Ƃ��\
//�g�p�ł��郏�C���g�J�[�h�����ɂ͈ȉ��̂��̂�����B
//�@�@�@"*":�C�ӕ�����ƈ�v����B
//   "?":�C�ӂ̂P�����ƈ�v����B
//   ���K�\���i[a-z]����regexp�Œ�`���ꂽ�p�^�[���w��j���L��
//
//   @param	string�F���ׂ���������
//   @param	pattern�F"*.*"���̃p�^�[��������
//   @retval	0:��v���Ȃ� / 1:��v����
//
//   */
///*============================================================================*/
//int CSatelliteData::ua_strmatch(char* string, char* pattern)
//#if 1
//{
//	if (!strcmp(string, pattern)){
//		return(1);
//	}
//	else{
//		return(0);
//	}
//}
//#else
//{
//	int i, j;
//	regex_t re;
//	char buf[256], patwork[512];
//	regmatch_t pm;
//
//	for (i = 0, j = 0; pattern[i] != NULL; i++, j++){
//		if (j >= sizeof(patwork)){ break; }
//		switch (pattern[i]){
//		case '*':
//			patwork[j] = '.'; j++;
//			patwork[j] = '*';
//			break;
//		case '?':
//			patwork[j] = '.';
//			break;
//		case '.':
//			patwork[j] = '\\'; j++;
//			patwork[j] = '.';
//			break;
//		default:
//			patwork[j] = pattern[i];
//			break;
//		}
//	}
//	patwork[j] = NULL;
//	i = regcomp(&re, patwork, REG_EXTENDED);
//	if (i != 0) {
//
//		/*#ifdef DEBUG
//		(void)regerror(i,&re,buf,sizeof buf);
//		fprintf(stderr,"ua_strmatch(%s,%s):regcomp=%s\n",string,pattern,buf); fflush(stderr);
//		#endif*/
//		return(0);                       /* report error */
//	}
//	i = regexec(&re, string, 1, &pm, 0);
//	regfree(&re);
//	if (i != 0) {
//
//		/*#ifdef DEBUG
//		(void)regerror(i,&re,buf,sizeof buf);
//		fprintf(stderr,"ua_strmatch(%s,%s):regexec=%s:rm_so=%d,rm_eo=%d\n",string,pattern,buf,pm.rm_so,pm.rm_eo); fflush(stderr);
//		#endif*/
//		return(0);                       /* report error */
//	}
//	if ((pm.rm_so == 0) && (pm.rm_eo == strlen(string))){
//		return(1);
//	}
//	else{
//
//		/*#ifdef DEBUG
//		fprintf(stderr,"regexec�͈�v�������ǈ�v���Ȃ�:ua_strmatch(%s,%s):rm_so=%d,rm_eo=%d\n",string,pattern,pm.rm_so,pm.rm_eo); fflush(stderr);
//		#endif*/
//		return(0);
//	}
//}
//#endif
//
///*============================================================================*/
///*! CSatelliteData
//
//-#
//�E�uop arg1,arg2,argN;�v�`���ŋL�q���ꂽ�e�L�X�g�t�@�C����ǂݍ���ŁA
//op/arg1/arg2/argN �𕪗�����B(�s�́u;�v���\���܂ŕ����s��
//�܂������Ă��ǂ�)
//
//�E�u/,#,*�v�̂ǂꂩ�Ŏn�܂�s�̓R�����g�s�Ƃ��ēǂݔ�΂�(���͂���Ȃ�)
//�u;�v�̌�낪�u/,#,*�v�̂ǂꂩ�̏ꍇ���u;�v�̌�납����s�R�[�h�܂�
//���R�����g�s�Ƃ��ēǂݔ�΂��B
//
//�E���s�R�[�h�݂̂̍s�^�X�y�[�X�����݂̂̍s�͓ǂݔ�΂��B
//
//�E�X�y�[�X�����Atab�R�[�h�̓T�v���X����(�X�y�[�X�R�[�h�Ɓu,�v�����
//�������ꍇ�́u"�v�ň͂ށB�u"�v����͂������ꍇ�́u\"�v�ƋL�q����B)
//
//�E�A�[�M�������g���ȗ����ꂽ�ꍇ(op,,,;)�̓A�[�M�������g������Ƃ���
//NULL������("")�ւ̃|�C���^��Ԃ��B
//
//�E�u# n "filename"�v�`���̃R�����g��cpp�ɂ��include�W�J���ꂽ�e�L�X�g
//�@�@�@��ǂݍ��񂾏ꍇ���l������filename�̃t�@�C�������݂��Ă���΁A�s�ԍ���
//   n�ɐݒ肷��
//
//   �m���Ӂn
//   �Eop/arg�̎��̂͂��̃��[�`������static�f�[�^�Ƃ��đ��݂���B
//   �]���āA���̃��[�`���𕡐���Ăяo���ꍇ��(�K�v�ɉ�����)�Ăяo������
//   op/arg�̓��e��ۑ�����K�v������B(���̃��[�`�����ł͌Ăяo����閈��
//   ����static�f�[�^�G���A���g���B)
//
//   �Eop����;�܂ł̊Ԃɂ̓R�����g�͏����܂���̂Œ��ӂ��Ă�������
//
//   �m�g�p��n
//   char *argv[ARGMAX+1];
//   long line;
//   FILE *fp;
//   fp = fopen(.....);    // �uop arg1,arg2,...,argN;�v�`���t�@�C����open
//   line = 1;             // ���čs�ԍ������������܂�
//   while((n=ub_getln(fp,&line,argv,ARGMAX))!=-1) {
//   if (!strcmp(argv[0],"op") {  // argv[0]��op�̕����̕�����̐擪
//   // �A�h���X���ݒ肳��܂�
//   sscanf(argv[1],"%d",&i_arg1); // argv[1]�ɑ�1�A�[�M�������g������
//   // �̐擪�A�h���X���ݒ肳��܂�
//   // argv[...]�ɐݒ肳��镶����
//   // �̃A�h���X�͂��̃��[�`������static
//   // �f�[�^�G���A���w���Ă��鎖�𒍈�
//   ..........
//   if (!strcmp(argv[2],"")) {    // �A�[�M�������g���ȗ����ꂽ�ꍇ��
//   i_arg2 = DEFALT;          // NULL������ւ̃A�ރ��X��Ԃ�
//   }
//   ..........
//
//   @param	fp�F���̓t�@�C����fopen()�œ���FILE*�l
//   @param	lp_line�F���̓t�@�C���̍s�ԍ�(���̃��[�`������
//   ���s�R�[�h���ɃJ�E���g�A�b�v����)
//   @param	argv�F�������ꂽ������(argv[0]==op ;
//   argv[1]==arg1,..,argv[N]==argN
//   @paran	argc�F�Ăяo�����ŗp�ӂ���argv[]�̌�
//   @retval	-2 : op�̒�����80�����ȏ゠��/arg�̒����̍��v��16000������
//   ����(���̃��[�`���̓�����static�G���A�ɋN�����鐧��)
//   -1 : �t�@�C���̏I��(EOF)�܂œǂݍ���
//   0 : �uop;�v�`���̃e�L�X�g(argv[0]��op��Ԃ���)
//   1 : �uop arg1;�v�`���̃e�L�X�g(argv[0]��op��argv[1]��arg1��
//   �Ԃ���)
//   n : �uop arg1,arg2,arg3,...,argN�v�`���̃e�L�X�g(n��arg��)
//
//   */
///*============================================================================*/
//long CSatelliteData::ub_getln(FILE* fp, long* lp_line, char* argv[], long argc)
///*
//�֐��l:   -2 : op�̒�����80�����ȏ゠��/arg�̒����̍��v��16000������
//����(���̃��[�`���̓�����static�G���A�ɋN�����鐧��)
//-1 : �t�@�C���̏I��(EOF)�܂œǂݍ���
//0 : �uop;�v�`���̃e�L�X�g(argv[0]��op��Ԃ���)
//1 : �uop arg1;�v�`���̃e�L�X�g(argv[0]��op��argv[1]��arg1��
//�Ԃ���)
//n : �uop arg1,arg2,arg3,...,argN�v�`���̃e�L�X�g(n��arg��)
//*/
//{
//	struct work_t {
//		char op[OPBUFSIZE];
//		char arg[ARGBUFSIZE];
//	};
//	static struct work_t buf;
//	static char sz_buf[256], sz_filename[256], sz_dmy[256];
//	struct stat stat_buf;
//
//	int  i;
//	int  c;
//	long l_argcnt, l_pos, l_supflg, l_ajst;
//
//	/*
//	**begin**
//	*/
//#ifdef SOL86
//	for (i = 0; i <argc; i++){ argv[i] = ""; }; /* CLR return argv[] */
//#else
//	for (i = 0; i <argc; i++){ argv[i] = NULL; }; /* CLR return argv[] */
//#endif
//
//START:
//	while ((c = fgetc(fp)) != EOF){
//		switch (c){
//		case ' ': case '\t':  continue;              /* space skip */
//		case '#':            /* comment�܂���cpp�̓W�J���� skip(LF�܂�) */
//			if (fgets(sz_buf, sizeof(sz_buf), fp) == NULL){
//				return(-1);
//			}
//			(*lp_line)++;
//			/*  # 2 "xxxxxxx" �̏ꍇ��*/
//			if (sscanf_s(sz_buf, "%d%1s%[^\"]\"", &i, sz_dmy, 256, sz_filename, 256) == 3){
//				if (stat(sz_filename, &stat_buf) != -1){
//					*lp_line = i;
//				}
//			}
//			goto START;
//		case '*': case '/':                /* comment skip(LF�܂�) */
//			while ((c = fgetc(fp)) != EOF){
//				if (c == '\n'){
//					(*lp_line)++; goto START;
//				}
//			};
//			return (-1);
//		case '\n':                                   /* LF skip */
//			(*lp_line)++; goto START;
//		default:
//			goto OP_FOUND;
//		}/*end switch*/
//	}/*end while*/
//	return (-1);
//
//OP_FOUND:
//	l_argcnt = 0; argv[l_argcnt] = &buf.op[0];
//	for (buf.op[0] = c, l_pos = 1; (c = fgetc(fp)) != EOF; l_pos++){
//		if (c <= ' '){                       /* OP ���� */
//			if (c == '\n'){ (*lp_line)++; }
//			buf.op[l_pos] = NULL;
//			goto ARGMENT;
//		}
//		else if (c == ';'){
//			buf.op[l_pos] = NULL;
//			return (0);                        /*non-argment*/
//		}
//		else if (c == ','){
//			buf.op[l_pos] = NULL;
//			l_argcnt++;
//			argv[l_argcnt] = ""; /* 1st-argment == null-strings */
//		}
//		if (l_pos >= OPBUFSIZE){ return (-2); }
//		buf.op[l_pos] = c;
//
//	}
//	buf.op[l_pos] = NULL;
//	return (0); /*non-argment*/
//
//ARGMENT:
//	l_pos = 0; l_supflg = 0;
//	l_argcnt++; argv[l_argcnt] = &buf.arg[0]; l_ajst = 1;
//	while ((c = fgetc(fp)) != EOF){
//		if (c == '"'){ l_supflg = (l_supflg + 1) & 1; continue; };
//		if (c == '\\'){
//			if ((c = fgetc(fp)) != EOF){ break; }; /* \" �̏��� */
//		};
//		if (l_supflg == 0){
//			if (c == ';'){                  /* END OF CODE */
//				buf.arg[l_pos] = NULL;
//				return (l_argcnt - l_ajst); /* return argmemt count*/
//			}
//			else if (c == ','){            /* argment���ڰ� */
//				buf.arg[l_pos] = NULL;
//				l_pos++;
//				l_argcnt++;
//				argv[l_argcnt] = &buf.arg[l_pos]; l_ajst = 1;
//			}
//			else if (c > ' '){             /* non-space strings */
//				l_ajst = 0;
//				buf.arg[l_pos] = c;
//				l_pos++;
//			}
//		}
//		else{                   /* " "�̓����ł͂��̂܂� */
//			l_ajst = 0;
//			buf.arg[l_pos] = c;
//			l_pos++;
//		};
//		if (c == '\n'){
//			l_supflg = 0;
//			(*lp_line)++;
//		}
//		if (l_pos >= ARGBUFSIZE){ return (-2); }
//	}
//	buf.arg[l_pos] = NULL;
//	return (l_argcnt - l_ajst); /* return argmemt count*/
//}
//
//
///*============================================================================*/
///*! CSpectrumView
//
//-# config��`�t�@�C�������`��������
//
//@param  sz_filename�Fconfig��`�t�@�C���̃t�@�C����
//@param  keyword�Fconfig���ɑΉ�����L�[���[�h��������w��
//@param  formatstring�F�L�[���[�h������ɑΉ�����config����ǂݍ��ލۂ�scanf()�p�̃t�H�[�}�b�g��������w��
//@param	arg1�`arg8�F�o�͒�`���
//@retval   n:�������ꂽ��`���ڂ�sscanf()�Ŏ�荞�񂾏ꍇ��sscanf()��
//�߂�l�A���Ȃ킿�Aformatstring�Ŏw�肵�����͍��ڐ���Ԃ�
//-1:�L�[���[�h�������ł��Ȃ������A�܂��́Asscanf()�ŃL�[���[
//�h�ɑΉ������`�l��ǂݍ��߂Ȃ������B
//-2:config�t�@�C����OPEN�ł��Ȃ��A�܂��͂��̑���System Call
//�֘A�̃G���[�����������B�i�G���[�ڍׂɂ��Ă�errno�ɐݒ肵���j
//*/
///*============================================================================*/
//int CSatelliteData::ub_fgetconf(TCHAR* sz_filename, char* keyword, char* formatstring, 
//	char* arg1 /*= NULL*/, char* arg2 /*= NULL*/, char* arg3 /*= NULL*/, char* arg4 /*= NULL*/, char* arg5 /*= NULL*/, char* arg6 /*= NULL*/, char* arg7 /*= NULL*/, char* arg8 /*= NULL*/)
//{
//	FILE *fp;
//	int  i, j;
//	char sz_arg[8192], sz_fmt[256];
//	char sz_hostname[40];
//	//	char *cpa_argv[ARGMAX], *szp_tlmdbdir, *szp_opelogdir, *szp_dir;
//	char *cpa_argv[ARGMAX];
//	long l_config_line;
//	char  *cp_keyword;
//	char *psz_conma;
//	char *psz_fmt;
//	char *cpa_argFmt[ARGMAX];
//	int  iArg;
//
//	static long l_fgetconf_seek_ptr = 0;
//	static TCHAR sz_beforefilename[256] = { NULL };
//
//	gethostname(sz_hostname, sizeof(sz_hostname));
//
//	//	if ((fp = fopen(sz_filename, "r")) == NULL){
//	/*------------------*/
//	/* �t�@�C�����擾   */
//	/*------------------*/
//	CString strAppPath = theApp.GetAppPath();
//	CString strFilePath;
//	strFilePath.Format(_T("%s%s"), strAppPath, CString(sz_filename));
//
//	/*------------------*/
//	/* �t�@�C���n�o�d�m */
//	/*------------------*/
//	if (_wfopen_s(&fp, strFilePath, _T("r")) != 0) {
//		//		uc_errlog(0x0011, __FILEW__, __LINE__);
//		return -2;
//	};
//
//	if (_tcscmp(sz_beforefilename, sz_filename) != 0){/*����Ă����*/
//		l_fgetconf_seek_ptr = 0; /*�擪����X�L�����̂�蒼��*/
//	}
//	_tcscpy_s(sz_beforefilename, sizeof(sz_beforefilename), sz_filename);
//	if (fseek(fp, l_fgetconf_seek_ptr, SEEK_SET) == -1){
//		//		uc_errlog(0x0011, __FILEW__, __LINE__);
//	};
//	l_config_line = 1;
//	while ((i = ub_getln(fp, &l_config_line, cpa_argv, ARGMAX - 1)) >= 0){
//		if (!strcmp(cpa_argv[0], keyword)){ /*�L�[���[�h����v���邩�H*/
//			goto found;
//		}
//		else{ /*�v�Z�@�ŗL�p�����[�^�ł��邩�ǂ�������ׂ�*/
//			if ((cp_keyword = strrchr(cpa_argv[0], '.')) != NULL){ /*�v�Z�@host���w�肪����ꍇ*/
//				*cp_keyword = NULL;
//				if (!strcmp(cp_keyword + 1, keyword)){ /*�L�[���[�h��v*/
//					if (ua_strmatch(sz_hostname, cpa_argv[0]) != NULL){/*host����v*/
//						goto found;
//					}/*end if hostname*/
//				}/*end if keyword */
//			}
//		}
//	}/*end while getln */
//	fclose(fp);
//	if (l_fgetconf_seek_ptr == 0){
//		return(-1);
//	}
//	else{
//		l_fgetconf_seek_ptr = 0;
//		return(ub_fgetconf(sz_filename, keyword, formatstring, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));
//	}
//
//found:/*�L�[���[�h����v*/
//	strcpy_s(sz_fmt, 256, "%s");
//	for (j = 1; j<i; j++){ strcat_s(sz_fmt, ",%s"); }
//	sprintf_s(sz_arg, 8192, sz_fmt, cpa_argv[1], 16, cpa_argv[2], 16, cpa_argv[3], 16, cpa_argv[4], 16
//		, cpa_argv[5], 16, cpa_argv[6], 16, cpa_argv[7], 16, cpa_argv[8], 16);
//	/*
//	printf( "cpa_argv[1] : [%s]\n", cpa_argv[1] );
//	printf( "cpa_argv[2] : [%s]\n", cpa_argv[2] );
//	printf( "cpa_argv[3] : [%s]\n", cpa_argv[3] );
//	printf( "cpa_argv[4] : [%s]\n", cpa_argv[4] );
//	printf( "cpa_argv[5] : [%s]\n", cpa_argv[5] );
//	printf( "cpa_argv[6] : [%s]\n", cpa_argv[6] );
//	printf( "cpa_argv[7] : [%s]\n", cpa_argv[7] );
//	printf( "cpa_argv[8] : [%s]\n", cpa_argv[8] );
//	printf( "sz_arg      : [%s]\n", sz_arg );
//	*/
//	cpa_argFmt[1] = arg1;
//	cpa_argFmt[2] = arg2;
//	cpa_argFmt[3] = arg3;
//	cpa_argFmt[4] = arg4;
//	cpa_argFmt[5] = arg5;
//	cpa_argFmt[6] = arg6;
//	cpa_argFmt[7] = arg7;
//	cpa_argFmt[8] = arg8;
//
//	iArg = 1;
//	psz_fmt = formatstring;
//	while ((psz_conma = strchr(psz_fmt, ',')) != NULL) {
//
//		/* �J���}��NULL�ɒu������ */
//		*psz_conma = (char)NULL;
//
//		/* �����^�ҏW�q's'��{�� */
//		if (strchr(psz_fmt, 's') != NULL) {	/* �����^�̏ꍇ */
//			if (cpa_argFmt[iArg] != NULL) {
//				strcpy_s(cpa_argFmt[iArg], 16, cpa_argv[iArg]);
//			}
//		}
//		else {									/* ���l�^�̏ꍇ */
//			if (cpa_argFmt[iArg] != NULL) {
//				sscanf_s(cpa_argv[iArg], psz_fmt, cpa_argFmt[iArg]);
//			}
//		}
//
//		/* �J�E���^�C���N�������g */
//		iArg++;
//
//		/* �����ʒu�̍X�V */
//		psz_fmt = psz_conma + 1;
//	}
//
//	/* �����^�ҏW�q's'��{�� */
//	if (strchr(psz_fmt, 's') != NULL) {	/* �����^�̏ꍇ */
//		if (cpa_argFmt[iArg] != NULL) {
//			strcpy_s(cpa_argFmt[iArg], 16, cpa_argv[iArg]);
//		}
//	}
//	else {									/* ���l�^�̏ꍇ */
//		if (cpa_argFmt[iArg] != NULL) {
//			sscanf_s(cpa_argv[iArg], psz_fmt, cpa_argFmt[iArg]);
//		}
//	}
//
//#ifdef DEBUGSSCANF
//	fprintf(stderr, "sscanf=%d:str=<%s>:format=<%s>:\n", i, sz_arg, formatstring);
//	fflush(stderr);
//#endif
//	if ((l_fgetconf_seek_ptr = ftell(fp)) == -1){
//		//		uc_errlog(0x0011, __FILEW__, __LINE__);
//		l_fgetconf_seek_ptr = 0;
//	};
//	fclose(fp);
//	return(iArg);
//}
//#endif


/*============================================================================*/
/*! CSatelliteData

-# config��`�t�@�C������w�b�_�s�ʒu���������A�s�ړ�����

@param	fp�F�t�@�C���|�C���^
@param	strHeader�F�w�b�_��
@retval	1�F���� / -1�F���s

*/
/*============================================================================*/
int CSatelliteData::MovetHeaderInFile(FILE* fp, TCHAR* strHeader)
{
	char ca_get[SATDATA_BUFF_SIZE];	    /* �ǂݍ��݃o�b�t�@       */
	int ret = -1;

	(void)fseek(fp, 0, SEEK_SET);

	for (;;) {
		if (feof(fp)) {
			break;
		}
		/* �P�s�ǂݍ��� */
		if (fgets(ca_get, SATDATA_BUFF_SIZE, fp) == NULL) {
			break;
		}
		else {
			CString str;
			str = ca_get;
			if (str.Find(strHeader) >= 0)
			{
				ret = 1;
				break;
			}
		}
	}

	return ret;
}


/*============================================================================*/
/*! CSatelliteData

-# config��`�t�@�C�������`�����������Along�^�̒l�Ƃ��ēǂݍ���

@param	fp�F�t�@�C���|�C���^
@param	strId�F����ID��
@param	val�Flong�^�̓ǂݍ��ݒl
@retval	1�F���� / -1�F���s

*/
/*============================================================================*/
int CSatelliteData::GetConfFromFile(FILE* fp, TCHAR* strId, long* val)
{
	char ca_get[SATDATA_BUFF_SIZE];	    /* �ǂݍ��݃o�b�t�@       */
	int ret = -1;

//	(void)fseek(fp, 0, SEEK_SET);

	for (;;) {
		if (feof(fp)) {
			break;
		}
		/* �P�s�ǂݍ��� */
		if (fgets(ca_get, SATDATA_BUFF_SIZE, fp) == NULL) {
			break;
		}
		else {
			CString str;
			str = ca_get;
			CString str2nd = str;
			if (str2nd.Replace(strId, _T("")) == 0)
				continue;

			str2nd.Trim();
			str2nd.Trim(_T(";\n"));

			*val = _ttol(str2nd);
			ret = 1;

			break;
		}
	}

	return ret;
}


/*============================================================================*/
/*! CSatelliteData

-# config��`�t�@�C�������`�����������Adouble�^�̒l�Ƃ��ēǂݍ���

@param	fp�F�t�@�C���|�C���^
@param	strId�F����ID��
@param	val�Fdouble�^�̓ǂݍ��ݒl
@retval	1�F���� / -1�F���s

*/
/*============================================================================*/
int CSatelliteData::GetConfFromFile(FILE* fp, TCHAR* strId, double* val)
{
	char ca_get[SATDATA_BUFF_SIZE];	    /* �ǂݍ��݃o�b�t�@       */
	int ret = -1;

//	(void)fseek(fp, 0, SEEK_SET);

	for (;;) {
		if (feof(fp)) {
			break;
		}
		/* �P�s�ǂݍ��� */
		if (fgets(ca_get, SATDATA_BUFF_SIZE, fp) == NULL) {
			break;
		}
		else {
			CString str;
			str = ca_get;
			CString str2nd = str;
			if (str2nd.Replace(strId, _T("")) == 0)
				continue;

			str2nd.Trim();
			str2nd.Trim(_T(";\n"));

			*val = _ttof(str2nd);
			ret = 1;

			break;
		}
	}

	return ret;
}

//#if 0
///*============================================================================*/
///*! CSatelliteData
//
//-# �\��l�ꗗ�t�@�C����ǂݍ��݃f�[�^�擾
//
//@param	�Ȃ�
//@retval	1�F���� / -1�F���s
//
//*/
///*============================================================================*/
//BOOL CSatelliteData::SetPredListDataList()
//{
//	FILE *fp;
//
//	CString strFilePath;
//	strFilePath.Format(_T("%s%s"), theApp.GetAppPath(), PRED_LIST_FILE);
//
//	char sz_pred_line[300];
//	char sz_aos[50];
//	char sz_aos_date[50];
//	char sz_los[50];
//	char sz_los_date[50];
//	char sz_max[50];
//	char sz_max_date[50];
//
//	char sz_stationname[50];
//	char sz_satname[50];
//	char sz_t_flag[50];
//	char sz_p_flag[50];
//	char sz_pass[50];
//	char sz_a_date[50];
//	char sz_a_time[50];
//	char sz_a_az[50];
//	char sz_a_el[50];
//	char sz_a_rng[50];
//	char sz_l_date[50];
//	char sz_l_time[50];
//	char sz_l_az[50];
//	char sz_l_el[50];
//	char sz_l_rng[50];
//	char sz_m_date[50];
//	char sz_m_time[50];
//	char sz_m_az[50];
//	char sz_m_el[50];
//	char sz_m_rng[50];
//	char sz_date[50];
//	char sz_time[50];
//	char sz_pred_path[50];
//	char sz_offset[50];
//
//	char sz_format[50];
//	char sz_get_from[50];
//
//	int nCol;
//
//	CTime	cTime;
//
//
//	if (_wfopen_s(&fp, strFilePath, _T("r")) != 0)
//	{
//		//		fprintf(stderr, "%s\n", "�\��l�ꗗ�t�@�C���̃I�[�v���Ɏ��s���܂����B");
//		//		uc_opelog(NULL, NULL, BASE_OPE_NO + 14, NULL, NULL);
//		return FALSE;
//	}
//
////	vector<CPredListData>		m_listPredList;				// �\��l�ꗗ�f�[�^
//
//	m_listPredList.clear();
//#if 1
//	CTime now(CTime::GetCurrentTime());
//	CTimeSpan tspan, tspan2;
//	CString sat;
//	CTime bs, be;
//
//#endif
//	CString str;
//
//	while (fgets(sz_pred_line, sizeof(sz_pred_line), fp) != NULL)
//	{
//		stPredListData pld;
//
//		// �V�t�H�[�}�b�g�ǂݍ���
//		nCol = sscanf_s(sz_pred_line, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
//			sz_stationname, sizeof(sz_stationname), 
//			sz_satname, sizeof(sz_satname), sz_t_flag, sizeof(sz_t_flag), sz_p_flag, sizeof(sz_p_flag), sz_pass, sizeof(sz_pass),
//			sz_a_date, sizeof(sz_a_date), sz_a_time, sizeof(sz_a_time), sz_a_az, sizeof(sz_a_az), sz_a_el, sizeof(sz_a_el), sz_a_rng, sizeof(sz_a_rng),
//			sz_l_date, sizeof(sz_l_date), sz_l_time, sizeof(sz_l_time), sz_l_az, sizeof(sz_l_az), sz_l_el, sizeof(sz_l_el), sz_l_rng, sizeof(sz_l_rng),
//			sz_m_date, sizeof(sz_m_date), sz_m_time, sizeof(sz_m_time), sz_m_az, sizeof(sz_m_az), sz_m_el, sizeof(sz_m_el), sz_m_rng, sizeof(sz_m_rng),
//			sz_date, sizeof(sz_date), sz_time, sizeof(sz_time), sz_pred_path, sizeof(sz_pred_path), sz_offset, sizeof(sz_offset),
//			sz_format, sizeof(sz_format), sz_get_from, sizeof(sz_get_from));
//
//		if (nCol != 26)
//		{
//			// �V�t�H�[�}�b�g�ǂݍ��݁i�^�C�v�Ȃ��j
//			nCol = sscanf_s(sz_pred_line, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
//				sz_stationname, sizeof(sz_stationname),
//				sz_satname, sizeof(sz_satname), sz_t_flag, sizeof(sz_t_flag), sz_p_flag, sizeof(sz_p_flag), sz_pass, sizeof(sz_pass),
//				sz_a_date, sizeof(sz_a_date), sz_a_time, sizeof(sz_a_time), sz_a_az, sizeof(sz_a_az), sz_a_el, sizeof(sz_a_el), sz_a_rng, sizeof(sz_a_rng),
//				sz_l_date, sizeof(sz_l_date), sz_l_time, sizeof(sz_l_time), sz_l_az, sizeof(sz_l_az), sz_l_el, sizeof(sz_l_el), sz_l_rng, sizeof(sz_l_rng),
//				sz_m_date, sizeof(sz_m_date), sz_m_time, sizeof(sz_m_time), sz_m_az, sizeof(sz_m_az), sz_m_el, sizeof(sz_m_el), sz_m_rng, sizeof(sz_m_rng),
//				sz_date, sizeof(sz_date), sz_time, sizeof(sz_time), sz_pred_path, sizeof(sz_pred_path), sz_offset, sizeof(sz_offset),
//				sz_get_from, sizeof(sz_get_from));
//		}
//
//		if (nCol < 25)
//		{
//			// ���t�H�[�}�b�g�ǂݍ���
//			sscanf_s(sz_pred_line, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
//				sz_satname, sizeof(sz_satname), sz_t_flag, sizeof(sz_t_flag), sz_p_flag, sizeof(sz_p_flag), sz_pass, sizeof(sz_pass),
//				sz_a_date, sizeof(sz_a_date), sz_a_time, sizeof(sz_a_time), sz_a_az, sizeof(sz_a_az), sz_a_el, sizeof(sz_a_el), sz_a_rng, sizeof(sz_a_rng),
//				sz_l_date, sizeof(sz_l_date), sz_l_time, sizeof(sz_l_time), sz_l_az, sizeof(sz_l_az), sz_l_el, sizeof(sz_l_el), sz_l_rng, sizeof(sz_l_rng),
//				sz_m_date, sizeof(sz_m_date), sz_m_time, sizeof(sz_m_time), sz_m_az, sizeof(sz_m_az), sz_m_el, sizeof(sz_m_el), sz_m_rng, sizeof(sz_m_rng),
//				sz_date, sizeof(sz_date), sz_time, sizeof(sz_time), sz_pred_path, sizeof(sz_pred_path), sz_offset, sizeof(sz_offset));
//		}
//
//		{
//			if (nCol >= 25)
//			{
//				// �ǖ�
//				pld.strStationName = CString(sz_stationname);
//			}
//
//			// �q����
//			pld.strSatellite = CString(sz_satname);
//
//			// �p�XID
//			pld.strPassId = CString(sz_pass);
//
//			// AOS�����̕ϊ�
//			sprintf_s(sz_aos, sizeof(sz_aos), "%s %s%s", sz_a_date, sz_a_time, ".0000");
//			if (ub_asc_to_asctime(sz_aos, sz_aos_date) < 0) {
//				fclose(fp);
//				return FALSE;
//			}
//			cTime = StrTimeToCTime(CString(sz_aos_date));
//#if 1	// �_�~�[
//			CTime ts = cTime;
//			tspan = now - cTime;
//			cTime += tspan;
//
//			if (sat == CString(sz_satname))
//			{
//				tspan = ts - bs;
//				cTime += tspan;
//			}
//			bs = cTime;
//#endif
//
//			pld.tAOS = cTime.GetTime();
//
//			// AOS AZ
//			str = CString(sz_a_az);
//			pld.dAosAz = _ttof(str.GetBuffer());
//			str.ReleaseBuffer();
//
//			// AOS EL
//			str = CString(sz_a_el);
//			pld.dAosEl = _ttof(str.GetBuffer());
//			str.ReleaseBuffer();
//
//			// LOS�����̕ϊ�
//			sprintf_s(sz_los, sizeof(sz_aos), "%s %s%s", sz_l_date, sz_l_time, ".0000");
//			if (CSatelliteData::ub_asc_to_asctime(sz_los, sz_los_date) < 0) {
//				fclose(fp);
//				return FALSE;
//			}
//			cTime = StrTimeToCTime(CString(sz_los_date));
//
//#if 1	// �_�~�[
//			CTime te = cTime;
//			tspan = now - cTime;
//			tspan2 = te - ts;
//			cTime += (tspan + tspan2);
//
//			if (sat == CString(sz_satname))
//			{
//				tspan = te - be;
//				cTime += tspan;
//			}
//			be = cTime;
//#endif
//
//			pld.tLOS = cTime.GetTime();
//
//			// LOS AZ
//			str = CString(sz_l_az);
//			pld.dLosAz = _ttof(str.GetBuffer());
//			str.ReleaseBuffer();
//
//			// LOS EL
//			str = CString(sz_l_el);
//			pld.dLosEl = _ttof(str.GetBuffer());
//			str.ReleaseBuffer();
//
//			// MAX EL�����̕ϊ�
//			sprintf_s(sz_max, sizeof(sz_aos), "%s %s%s", sz_m_date, sz_m_time, ".0000");
//			if (CSatelliteData::ub_asc_to_asctime(sz_max, sz_max_date) < 0) {
//				fclose(fp);
//				return FALSE;
//			}
//			cTime = StrTimeToCTime(CString(sz_max_date));
//
//			pld.tMaxEl = cTime.GetTime();
//
//			// MAX EL AZ
//			str = CString(sz_m_az);
//			pld.dMaxAz = _ttof(str.GetBuffer());
//			str.ReleaseBuffer();
//
//			// MAX EL EL
//			str = CString(sz_m_el);
//			pld.dMaxEl = _ttof(str.GetBuffer());
//			str.ReleaseBuffer();
//
//			// �\��l�t�@�C����
//			pld.strPredPath = CString(sz_pred_path);
//
//			// �I�t�Z�b�g
//			str = CString(sz_offset);
//			// �����𒊏o
//			long nSign = 1;
//			if (str.Left(1) == _T('-'))
//			{
//				nSign = -1;
//				str.Remove(_T('-'));
//			}
//			// �E����Q���b
//			CString strSS = str.Right(2);
//			// �c�肪����":"�Ȃ̂ŕb��":"�̕����������O���A�����o��
//			long nLength = str.GetLength();
//			CString strMM = str.Left(nLength - (2 + 1));
//			long nMM = _ttoi(strMM);
//			long nSS = _ttoi(strSS);
//			pld.tOffset = nSign * (nMM * 60 + nSS);
//
//			if (nCol >= 25)
//			{
//				if (nCol == 25)
//				{
//					// �^�C�v
//					pld.strFormat = _T("");
//				}
//				else
//				{
//					// �^�C�v
//					pld.strFormat = CString(sz_format);
//				}
//
//				// �擾��
//				pld.strGetFrom = CString(sz_get_from).MakeUpper();
//			}
//
//			m_listPredList.push_back(pld);
//
//#if 1
//			sat = CString(sz_satname);
//#endif
//		}
//	}
//
//	fclose(fp);
//
//#ifdef _NOCOMM
//
//	SYSTEMTIME sys;
//	GetSystemTime(&sys);
//	sys.wSecond = 0;
//	CTime local = CTime(sys);
//	int pos = 0;
//	for (int i = 0; i < sizeof(mStationString) / sizeof(mStationString[0]); i++){
//		if (m_listPredList.size() == 0){
//			break;
//		}
//		stPredListData pld = m_listPredList[0];
//		const TCHAR* mSpaceString[]{
//			_T("ASTRO-E2"),
//			_T("HAYABUSA2"),
//			_T("IKAROS"),
//			_T("ASTRO-H"),
//			_T("GEOTAIL"),
//		};
//		for (int j = 0; j < sizeof(mSpaceString) / sizeof(mSpaceString[0]); j++){
//			// �ǖ�
//			pld.strStationName = mStationString[i];
//			// �q����
//			pld.strSatellite = mSpaceString[j];
//			// �p�XID
//			pld.strPassId.Format(_T("170200-0%d%d0"), i, j);
//			// AOS
//			pld.tAOS = (local + CTimeSpan(0, i, 10 * (i + 1), 0)).GetTime();
//			// LOS
//			pld.tLOS = (local + CTimeSpan(0, i+1, 20 * (i + 1), 0)).GetTime();
//			// �\��l�t�@�C����
//			pld.strPredPath = _T("/CONFIG/prd/planet-c/1601100100.prd");
//
//			m_listPredList.push_back(pld);
//
//			// �p�XID
//			pld.strPassId.Format(_T("170200-9%d%d0"), i, j);
//			// AOS
//			pld.tAOS = (local + CTimeSpan(0, i + 1, 40 * (i + 1), 0)).GetTime();
//			// LOS
//			pld.tLOS = (local + CTimeSpan(0, i + 2, 50 * (i + 1), 0)).GetTime();
//			//m_listPredList.push_back(pld);
//
//
//			pos++;
//		}
//	}
//#endif
//
//	return TRUE;
//}
//#endif
//
//#if 0
///*============================================================================*/
///*! CSatelliteData
//
//-# �\��l�ꗗ�t�@�C����ǂݍ��݃f�[�^�擾
//
//@param	�Ȃ�
//@retval	1�F���� / -1�F���s
//
//*/
///*============================================================================*/
//BOOL CSatelliteData::SetPredList_Utf8()
//{
//	BOOL ret = TRUE;
//
//	m_listPredList.clear();
//
//	// 1�ǃ��[�h
//	if (theApp.GetSelectStationMode() == eStation_1)
//	{
//		ret = SetPredList_Utf8(theApp.GetSelectStation());
//	}
//	// 4�ǃ��[�h
//	else
//	{
//		for (UINT i = 0; i < eStation_MAX; i++)
//		{
//			if (SetPredList_Utf8(i) == FALSE)
//			{
//				ret = FALSE;
//			}
//		}
//	}
//
//	return ret;
//}
//
//
///*============================================================================*/
///*! CSatelliteData
//
//-# �w��ǂ̗\��l�ꗗ�t�@�C����ǂݍ��݃f�[�^�擾(UTF8�Ή�)
//
//@param	nStation�F�ǔԍ�
//@retval	1�F���� / -1�F���s
//
//*/
///*============================================================================*/
//BOOL CSatelliteData::SetPredList_Utf8(UINT nStation)
//{
//	FILE *fp;
//
////	CString strFilePath;
////	strFilePath.Format(_T("%s%s"), theApp.GetAppPath(), PRED_LIST_FILE);
//	CString strFilePath = theApp.GetShareFilePath(eFileType_PredList_File, nStation);
//
//	TCHAR sz_pred_line[300];
//	TCHAR sz_aos[50];
//	TCHAR sz_aos_date[50];
//	TCHAR sz_los[50];
//	TCHAR sz_los_date[50];
//	TCHAR sz_max[50];
//	TCHAR sz_max_date[50];
//
//	TCHAR sz_stationname[50];
//	TCHAR sz_satname[50];
//	TCHAR sz_t_flag[50];
//	TCHAR sz_p_flag[50];
//	TCHAR sz_pass[50];
//	TCHAR sz_a_date[50];
//	TCHAR sz_a_time[50];
//	TCHAR sz_a_az[50];
//	TCHAR sz_a_el[50];
//	TCHAR sz_a_rng[50];
//	TCHAR sz_l_date[50];
//	TCHAR sz_l_time[50];
//	TCHAR sz_l_az[50];
//	TCHAR sz_l_el[50];
//	TCHAR sz_l_rng[50];
//	TCHAR sz_m_date[50];
//	TCHAR sz_m_time[50];
//	TCHAR sz_m_az[50];
//	TCHAR sz_m_el[50];
//	TCHAR sz_m_rng[50];
//	TCHAR sz_date[50];
//	TCHAR sz_time[50];
//	TCHAR sz_pred_path[50];
//	TCHAR sz_offset[50];
//
//	TCHAR sz_format[50];
//	TCHAR sz_get_from[50];
//
//	int nCol;
//
//
//	// �t�@�C���X�V�����̎擾
//	HANDLE hFile;
//	hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
//	if (hFile == INVALID_HANDLE_VALUE){
//		return FALSE;
//	}
//	FILETIME cfTime, afTime, wfTime;
//	GetFileTime(hFile, &cfTime, &afTime, &wfTime);
//	CloseHandle(hFile);
//
//	CTime time(wfTime);
//	if (time <= mGetSharePredFileTime[nStation]){
//		// �t�@�C���^�C���ɕω��Ȃ�
//		return FALSE;
//	}
//	mGetSharePredFileTime[nStation] = time;
//
//
//	CTime	cTime;
//
//	if (_wfopen_s(&fp, strFilePath, _T("r, ccs=UTF-8")) != 0)
//	{
//		//		fprintf(stderr, "%s\n", "�\��l�ꗗ�t�@�C���̃I�[�v���Ɏ��s���܂����B");
//		//		uc_opelog(NULL, NULL, BASE_OPE_NO + 14, NULL, NULL);
//		return FALSE;
//	}
//
//#if 1
//	SYSTEMTIME systime;
//	GetSystemTime(&systime);
//	//CTime now(CTime::GetCurrentTime());
//	CTime now(systime);
//	//CTime now(CTime::GetCurrentTime());
//	CTimeSpan tspan, tspan2;
//	CString sat;
//	CTime bs, be;
//
//#endif
//	CString str;
//
//	while (fgetws(sz_pred_line, static_cast<unsigned>(_countof(sz_pred_line)), fp) != NULL)
//	{
//		stPredListData pld;
//
//		// �V�t�H�[�}�b�g�ǂݍ���
//		nCol = swscanf_s(sz_pred_line, L"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
//			sz_stationname, static_cast<unsigned>(_countof(sz_stationname)),
//			sz_satname, static_cast<unsigned>(_countof(sz_satname)),
//			sz_t_flag, static_cast<unsigned>(_countof(sz_t_flag)),
//			sz_p_flag, static_cast<unsigned>(_countof(sz_p_flag)),
//			sz_pass, static_cast<unsigned>(_countof(sz_pass)),
//			sz_a_date, static_cast<unsigned>(_countof(sz_a_date)),
//			sz_a_time, static_cast<unsigned>(_countof(sz_a_time)),
//			sz_a_az, static_cast<unsigned>(_countof(sz_a_az)),
//			sz_a_el, static_cast<unsigned>(_countof(sz_a_el)),
//			sz_a_rng, static_cast<unsigned>(_countof(sz_a_rng)),
//			sz_l_date, static_cast<unsigned>(_countof(sz_l_date)),
//			sz_l_time, static_cast<unsigned>(_countof(sz_l_time)),
//			sz_l_az, static_cast<unsigned>(_countof(sz_l_az)),
//			sz_l_el, static_cast<unsigned>(_countof(sz_l_el)),
//			sz_l_rng, static_cast<unsigned>(_countof(sz_l_rng)),
//			sz_m_date, static_cast<unsigned>(_countof(sz_m_date)),
//			sz_m_time, static_cast<unsigned>(_countof(sz_m_time)),
//			sz_m_az, static_cast<unsigned>(_countof(sz_m_az)),
//			sz_m_el, static_cast<unsigned>(_countof(sz_m_el)),
//			sz_m_rng, static_cast<unsigned>(_countof(sz_m_rng)),
//			sz_date, static_cast<unsigned>(_countof(sz_date)),
//			sz_time, static_cast<unsigned>(_countof(sz_time)),
//			sz_pred_path, static_cast<unsigned>(_countof(sz_pred_path)),
//			sz_offset, static_cast<unsigned>(_countof(sz_offset)),
//			sz_format, static_cast<unsigned>(_countof(sz_format)),
//			sz_get_from, static_cast<unsigned>(_countof(sz_get_from)));
//
//		if (nCol != 26)
//		{
//			// �V�t�H�[�}�b�g�ǂݍ��݁i�^�C�v�Ȃ��j
//			nCol = swscanf_s(sz_pred_line, L"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
//				sz_stationname, static_cast<unsigned>(_countof(sz_stationname)),
//				sz_satname, static_cast<unsigned>(_countof(sz_satname)),
//				sz_t_flag, static_cast<unsigned>(_countof(sz_t_flag)),
//				sz_p_flag, static_cast<unsigned>(_countof(sz_p_flag)),
//				sz_pass, static_cast<unsigned>(_countof(sz_pass)),
//				sz_a_date, static_cast<unsigned>(_countof(sz_a_date)),
//				sz_a_time, static_cast<unsigned>(_countof(sz_a_time)),
//				sz_a_az, static_cast<unsigned>(_countof(sz_a_az)),
//				sz_a_el, static_cast<unsigned>(_countof(sz_a_el)),
//				sz_a_rng, static_cast<unsigned>(_countof(sz_a_rng)),
//				sz_l_date, static_cast<unsigned>(_countof(sz_l_date)),
//				sz_l_time, static_cast<unsigned>(_countof(sz_l_time)),
//				sz_l_az, static_cast<unsigned>(_countof(sz_l_az)),
//				sz_l_el, static_cast<unsigned>(_countof(sz_l_el)),
//				sz_l_rng, static_cast<unsigned>(_countof(sz_l_rng)),
//				sz_m_date, static_cast<unsigned>(_countof(sz_m_date)),
//				sz_m_time, static_cast<unsigned>(_countof(sz_m_time)),
//				sz_m_az, static_cast<unsigned>(_countof(sz_m_az)),
//				sz_m_el, static_cast<unsigned>(_countof(sz_m_el)),
//				sz_m_rng, static_cast<unsigned>(_countof(sz_m_rng)),
//				sz_date, static_cast<unsigned>(_countof(sz_date)),
//				sz_time, static_cast<unsigned>(_countof(sz_time)),
//				sz_pred_path, static_cast<unsigned>(_countof(sz_pred_path)),
//				sz_offset, static_cast<unsigned>(_countof(sz_offset)),
//				sz_get_from, static_cast<unsigned>(_countof(sz_get_from)));
//		}
//
//		if (nCol < 25)
//		{
//			// ���t�H�[�}�b�g�ǂݍ���
//			swscanf_s(sz_pred_line, L"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
//				sz_satname, static_cast<unsigned>(_countof(sz_satname)), 
//				sz_t_flag, static_cast<unsigned>(_countof(sz_t_flag)), 
//				sz_p_flag, static_cast<unsigned>(_countof(sz_p_flag)), 
//				sz_pass, static_cast<unsigned>(_countof(sz_pass)),
//				sz_a_date, static_cast<unsigned>(_countof(sz_a_date)), 
//				sz_a_time, static_cast<unsigned>(_countof(sz_a_time)), 
//				sz_a_az, static_cast<unsigned>(_countof(sz_a_az)), 
//				sz_a_el, static_cast<unsigned>(_countof(sz_a_el)), 
//				sz_a_rng, static_cast<unsigned>(_countof(sz_a_rng)),
//				sz_l_date, static_cast<unsigned>(_countof(sz_l_date)), 
//				sz_l_time, static_cast<unsigned>(_countof(sz_l_time)), 
//				sz_l_az, static_cast<unsigned>(_countof(sz_l_az)), 
//				sz_l_el, static_cast<unsigned>(_countof(sz_l_el)), 
//				sz_l_rng, static_cast<unsigned>(_countof(sz_l_rng)),
//				sz_m_date, static_cast<unsigned>(_countof(sz_m_date)), 
//				sz_m_time, static_cast<unsigned>(_countof(sz_m_time)), 
//				sz_m_az, static_cast<unsigned>(_countof(sz_m_az)), 
//				sz_m_el, static_cast<unsigned>(_countof(sz_m_el)), 
//				sz_m_rng, static_cast<unsigned>(_countof(sz_m_rng)),
//				sz_date, static_cast<unsigned>(_countof(sz_date)), 
//				sz_time, static_cast<unsigned>(_countof(sz_time)), 
//				sz_pred_path, static_cast<unsigned>(_countof(sz_pred_path)), 
//				sz_offset, static_cast<unsigned>(_countof(sz_offset)));
//		}
//
//		{
//			if (nCol >= 25)
//			{
//				// �ǖ�
//				pld.strStationName = CString(sz_stationname);
//			}
//
//			// �q����
//			pld.strSatellite = CString(sz_satname);
//
//			// �p�XID
//			pld.strPassId = CString(sz_pass);
//
//			// AOS�����̕ϊ�
//			swprintf_s(sz_aos, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_a_date, sz_a_time, L".0000");
//			if (ub_asc_to_asctime(sz_aos, sz_aos_date) < 0) {
//				fclose(fp);
//				return FALSE;
//			}
//			cTime = StrTimeToCTime(CString(sz_aos_date));
//#if 1	// �_�~�[
//			CTime ts = cTime;
//			tspan = now - cTime;
//			cTime += tspan;
//
//			if (sat == CString(sz_satname))
//			{
//				tspan = ts - bs;
//				cTime += tspan;
//			}
//			bs = cTime;
//#endif
//
//			pld.tAOS = cTime.GetTime();
//
//			// AOS AZ
//			str = CString(sz_a_az);
//			pld.dAosAz = _ttof(str.GetBuffer());
//			str.ReleaseBuffer();
//
//			// AOS EL
//			str = CString(sz_a_el);
//			pld.dAosEl = _ttof(str.GetBuffer());
//			str.ReleaseBuffer();
//
//			// LOS�����̕ϊ�
//			swprintf_s(sz_los, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_l_date, sz_l_time, L".0000");
//			if (ub_asc_to_asctime(sz_los, sz_los_date) < 0) {
//				fclose(fp);
//				return FALSE;
//			}
//			cTime = StrTimeToCTime(CString(sz_los_date));
//
//#if 1	// �_�~�[
//			CTime te = cTime;
//			tspan = now - cTime;
//			tspan2 = te - ts;
//			cTime += (tspan + tspan2);
//
//			if (sat == CString(sz_satname))
//			{
//				tspan = te - be;
//				cTime += tspan;
//			}
//			be = cTime;
//#endif
//
//			pld.tLOS = cTime.GetTime();
//
//			// LOS AZ
//			str = CString(sz_l_az);
//			pld.dLosAz = _ttof(str.GetBuffer());
//			str.ReleaseBuffer();
//
//			// LOS EL
//			str = CString(sz_l_el);
//			pld.dLosEl = _ttof(str.GetBuffer());
//			str.ReleaseBuffer();
//
//			// MAX EL�����̕ϊ�
//			swprintf_s(sz_max, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_m_date, sz_m_time, L".0000");
//			if (ub_asc_to_asctime(sz_max, sz_max_date) < 0) {
//				fclose(fp);
//				return FALSE;
//			}
//			cTime = StrTimeToCTime(CString(sz_max_date));
//
//			pld.tMaxEl = cTime.GetTime();
//
//			// MAX EL AZ
//			str = CString(sz_m_az);
//			pld.dMaxAz = _ttof(str.GetBuffer());
//			str.ReleaseBuffer();
//
//			// MAX EL EL
//			str = CString(sz_m_el);
//			pld.dMaxEl = _ttof(str.GetBuffer());
//			str.ReleaseBuffer();
//
//			// �\��l�t�@�C����
//			pld.strPredPath = CString(sz_pred_path);
//
//			// �I�t�Z�b�g
//			str = CString(sz_offset);
//			// �����𒊏o
//			long nSign = 1;
//			if (str.Left(1) == _T('-'))
//			{
//				nSign = -1;
//				str.Remove(_T('-'));
//			}
//			// �E����Q���b
//			CString strSS = str.Right(2);
//			// �c�肪����":"�Ȃ̂ŕb��":"�̕����������O���A�����o��
//			long nLength = str.GetLength();
//			CString strMM = str.Left(nLength - (2 + 1));
//			long nMM = _ttoi(strMM);
//			long nSS = _ttoi(strSS);
//			pld.tOffset = nSign * (nMM * 60 + nSS);
//
//			if (nCol >= 25)
//			{
//				if (nCol == 25)
//				{
//					// �^�C�v
//					pld.strFormat = _T("");
//				}
//				else
//				{
//					// �^�C�v
//					pld.strFormat = CString(sz_format);
//				}
//
//				// �擾��
//				pld.strGetFrom = CString(sz_get_from).MakeUpper();
//			}
//
//			m_listPredList.push_back(pld);
//
//#if 1
//			sat = CString(sz_satname);
//#endif
//		}
//	}
//
//	fclose(fp);
//
//#ifdef _NOCOMM
//
//	SYSTEMTIME sys;
//	GetSystemTime(&sys);
//	sys.wSecond = 0;
//	CTime local = CTime(sys);
//	int pos = 0;
////	for (int i = 0; i < sizeof(mStationString) / sizeof(mStationString[0]); i++)
//	{
//		if (m_listPredList.size() == 0){
//			return FALSE;
//		}
//		int i = nStation;
//		stPredListData pld = m_listPredList[0];
//		const TCHAR* mSpaceString[]{
//			_T("ASTRO-E2"),
//				_T("HAYABUSA2"),
//				_T("IKAROS"),
//				_T("ASTRO-H"),
//				_T("GEOTAIL"),
//		};
//		//for (int j = 0; j < sizeof(mSpaceString) / sizeof(mSpaceString[0]); j++)
//		{
//			int j = 0;
//			// �ǖ�
//			pld.strStationName = mStationString[i];
//			// �q����
//			pld.strSatellite = mSpaceString[j];
//			// �p�XID
//			pld.strPassId.Format(_T("170200-0%d%d0"), i, j);
//			// AOS
//			pld.tAOS = (local + CTimeSpan(0, i, 10 * (i + 1), 0)).GetTime();
//			// LOS
//			pld.tLOS = (local + CTimeSpan(0, i + 1, 20 * (i + 1), 0)).GetTime();
//			// �\��l�t�@�C����
//			pld.strPredPath = _T("/CONFIG/prd/planet-c/1601100100.prd");
//
//			m_listPredList.push_back(pld);
//
//			// �p�XID
//			pld.strPassId.Format(_T("170200-9%d%d0"), i, j);
//			// AOS
//			pld.tAOS = (local + CTimeSpan(0, i + 1, 40 * (i + 1), 0)).GetTime();
//			// LOS
//			pld.tLOS = (local + CTimeSpan(0, i + 2, 50 * (i + 1), 0)).GetTime();
//
//			// �\��l�t�@�C����(ishi)
//			pld.strPredPath.Format(_T("/CONFIG/prd/%s/1702009%d%d0.prd"), mSpaceString[j], i, j);
//
//			m_listPredList.push_back(pld);
//
//
//			pos++;
//		}
//	}
//#endif
//
//	return TRUE;
//}
//#endif

/*============================================================================*/
/*! CSatelliteData

-# �\��l�ꗗ�t�@�C����ǂݍ��݃f�[�^�擾

@param	�Ȃ�
@retval	1�F���� / -1�F���s

*/
/*============================================================================*/
BOOL CSatelliteData::SetPredList_Euc()
{
	BOOL ret = FALSE;

	m_listPredList.clear();

	// 1�ǃ��[�h
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		UINT nStation = theApp.GetSelectStation();
		// �\��l�ꗗ�t�@�C�����X�V����Ă��邩���ׂ�
		if (IsPredListFileUpdated(nStation) == TRUE)
		{
			// �\��l�ꗗ�t�@�C�����X�V����Ă���ꍇ�A�\��l�ꗗ�t�@�C���ǂݍ���
			ret = SetPredList_Euc(nStation);
		}
	}
	// 4�ǃ��[�h
	else
	{
		BOOL bUpdated = FALSE;
		// �\��l�ꗗ�t�@�C�����X�V����Ă��邩���ׂ�
		for (UINT i = 0; i < eStation_MAX; i++)
		{
			if (IsPredListFileUpdated(i) == TRUE)
			{
				bUpdated = TRUE;
			}
		}

		// �\��l�ꗗ�t�@�C�����X�V����Ă���ꍇ�A�\��l�ꗗ�t�@�C���ǂݍ���
		if (bUpdated == TRUE)
		{
			for (UINT i = 0; i < eStation_MAX; i++)
			{
				if (SetPredList_Euc(i) == TRUE)
				{
					ret = TRUE;
				}
			}
		}
	}

	return ret;
}


/*============================================================================*/
/*! CSatelliteData

-# �w��ǂ̗\��l�ꗗ�t�@�C����ǂݍ��݃f�[�^�擾(EUC�Ή�)

@param	nStation�F�ǔԍ�
@retval	1�F���� / -1�F���s

*/
/*============================================================================*/
BOOL CSatelliteData::SetPredList_Euc(UINT nStation)
{
	FILE *fp;

	CString strFilePath = theApp.GetShareFilePath(eFileType_PredList_File, nStation);

	TCHAR sz_aos[50];
	TCHAR sz_aos_date[50];
	TCHAR sz_los[50];
	TCHAR sz_los_date[50];
	TCHAR sz_max[50];
	TCHAR sz_max_date[50];

	TCHAR sz_stationname[50] = { 0 };
	TCHAR sz_satname[50] = { 0 };
	TCHAR sz_t_flag[50];
	TCHAR sz_p_flag[50];
	TCHAR sz_pass[50] = { 0 };
	TCHAR sz_a_date[50];
	TCHAR sz_a_time[50];
	TCHAR sz_a_az[50] = { 0 };
	TCHAR sz_a_el[50] = { 0 };
	TCHAR sz_a_rng[50];
	TCHAR sz_l_date[50];
	TCHAR sz_l_time[50];
	TCHAR sz_l_az[50] = { 0 };
	TCHAR sz_l_el[50] = { 0 };
	TCHAR sz_l_rng[50];
	TCHAR sz_m_date[50];
	TCHAR sz_m_time[50];
	TCHAR sz_m_az[50] = { 0 };
	TCHAR sz_m_el[50] = { 0 };
	TCHAR sz_m_rng[50];
	TCHAR sz_date[50];
	TCHAR sz_time[50];
	TCHAR sz_pred_path[50] = { 0 };
	TCHAR sz_offset[50] = { 0 };

	TCHAR sz_format[50] = { 0 };
	TCHAR sz_get_from[50] = { 0 };

	int nCol;


	// �t�@�C���X�V�����̎擾
	HANDLE hFile;
	hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE){
		mGetSharePredFileTime[nStation] = CTime(0);
		return FALSE;
	}
	FILETIME cfTime, afTime, wfTime;
	GetFileTime(hFile, &cfTime, &afTime, &wfTime);
	CloseHandle(hFile);

	CTime time(wfTime);
//	if (time <= mGetSharePredFileTime[nStation]){
//		// �t�@�C���^�C���ɕω��Ȃ�
//		return FALSE;
//	}
//	mGetSharePredFileTime[nStation] = time;

	CTime	cTime;

	if (_wfopen_s(&fp, strFilePath, _T("r")) != 0 || fp == NULL)
	{
		//		fprintf(stderr, "%s\n", "�\��l�ꗗ�t�@�C���̃I�[�v���Ɏ��s���܂����B");
		//		uc_opelog(NULL, NULL, BASE_OPE_NO + 14, NULL, NULL);
		return FALSE;
	}

#if 1
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	//CTime now(CTime::GetCurrentTime());
	CTime now(systime);
	//CTime now(CTime::GetCurrentTime());
	CTimeSpan tspan, tspan2;
	CString sat;
	CTime bs, be;

#endif

	char szline[2048];
	CString str, cbuf;
	while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL)
	{
		cbuf = CServerFile::ConvertUnicode(szline);
		stPredListData pld;

		// �V�t�H�[�}�b�g�ǂݍ���
		nCol = swscanf_s(cbuf, L"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
			sz_stationname, static_cast<unsigned>(_countof(sz_stationname)),
			sz_satname, static_cast<unsigned>(_countof(sz_satname)),
			sz_t_flag, static_cast<unsigned>(_countof(sz_t_flag)),
			sz_p_flag, static_cast<unsigned>(_countof(sz_p_flag)),
			sz_pass, static_cast<unsigned>(_countof(sz_pass)),
			sz_a_date, static_cast<unsigned>(_countof(sz_a_date)),
			sz_a_time, static_cast<unsigned>(_countof(sz_a_time)),
			sz_a_az, static_cast<unsigned>(_countof(sz_a_az)),
			sz_a_el, static_cast<unsigned>(_countof(sz_a_el)),
			sz_a_rng, static_cast<unsigned>(_countof(sz_a_rng)),
			sz_l_date, static_cast<unsigned>(_countof(sz_l_date)),
			sz_l_time, static_cast<unsigned>(_countof(sz_l_time)),
			sz_l_az, static_cast<unsigned>(_countof(sz_l_az)),
			sz_l_el, static_cast<unsigned>(_countof(sz_l_el)),
			sz_l_rng, static_cast<unsigned>(_countof(sz_l_rng)),
			sz_m_date, static_cast<unsigned>(_countof(sz_m_date)),
			sz_m_time, static_cast<unsigned>(_countof(sz_m_time)),
			sz_m_az, static_cast<unsigned>(_countof(sz_m_az)),
			sz_m_el, static_cast<unsigned>(_countof(sz_m_el)),
			sz_m_rng, static_cast<unsigned>(_countof(sz_m_rng)),
			sz_date, static_cast<unsigned>(_countof(sz_date)),
			sz_time, static_cast<unsigned>(_countof(sz_time)),
			sz_pred_path, static_cast<unsigned>(_countof(sz_pred_path)),
			sz_offset, static_cast<unsigned>(_countof(sz_offset)),
			sz_format, static_cast<unsigned>(_countof(sz_format)),
			sz_get_from, static_cast<unsigned>(_countof(sz_get_from)));

		if (nCol < 0)
			continue;

		if (nCol != 26)
		{
			// �V�t�H�[�}�b�g�ǂݍ��݁i�^�C�v�Ȃ��j
			nCol = swscanf_s(cbuf, L"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
				sz_stationname, static_cast<unsigned>(_countof(sz_stationname)),
				sz_satname, static_cast<unsigned>(_countof(sz_satname)),
				sz_t_flag, static_cast<unsigned>(_countof(sz_t_flag)),
				sz_p_flag, static_cast<unsigned>(_countof(sz_p_flag)),
				sz_pass, static_cast<unsigned>(_countof(sz_pass)),
				sz_a_date, static_cast<unsigned>(_countof(sz_a_date)),
				sz_a_time, static_cast<unsigned>(_countof(sz_a_time)),
				sz_a_az, static_cast<unsigned>(_countof(sz_a_az)),
				sz_a_el, static_cast<unsigned>(_countof(sz_a_el)),
				sz_a_rng, static_cast<unsigned>(_countof(sz_a_rng)),
				sz_l_date, static_cast<unsigned>(_countof(sz_l_date)),
				sz_l_time, static_cast<unsigned>(_countof(sz_l_time)),
				sz_l_az, static_cast<unsigned>(_countof(sz_l_az)),
				sz_l_el, static_cast<unsigned>(_countof(sz_l_el)),
				sz_l_rng, static_cast<unsigned>(_countof(sz_l_rng)),
				sz_m_date, static_cast<unsigned>(_countof(sz_m_date)),
				sz_m_time, static_cast<unsigned>(_countof(sz_m_time)),
				sz_m_az, static_cast<unsigned>(_countof(sz_m_az)),
				sz_m_el, static_cast<unsigned>(_countof(sz_m_el)),
				sz_m_rng, static_cast<unsigned>(_countof(sz_m_rng)),
				sz_date, static_cast<unsigned>(_countof(sz_date)),
				sz_time, static_cast<unsigned>(_countof(sz_time)),
				sz_pred_path, static_cast<unsigned>(_countof(sz_pred_path)),
				sz_offset, static_cast<unsigned>(_countof(sz_offset)),
				sz_get_from, static_cast<unsigned>(_countof(sz_get_from)));
		}

		if (nCol < 25)
		{
			// ���t�H�[�}�b�g�ǂݍ���
			swscanf_s(cbuf, L"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
				sz_satname, static_cast<unsigned>(_countof(sz_satname)),
				sz_t_flag, static_cast<unsigned>(_countof(sz_t_flag)),
				sz_p_flag, static_cast<unsigned>(_countof(sz_p_flag)),
				sz_pass, static_cast<unsigned>(_countof(sz_pass)),
				sz_a_date, static_cast<unsigned>(_countof(sz_a_date)),
				sz_a_time, static_cast<unsigned>(_countof(sz_a_time)),
				sz_a_az, static_cast<unsigned>(_countof(sz_a_az)),
				sz_a_el, static_cast<unsigned>(_countof(sz_a_el)),
				sz_a_rng, static_cast<unsigned>(_countof(sz_a_rng)),
				sz_l_date, static_cast<unsigned>(_countof(sz_l_date)),
				sz_l_time, static_cast<unsigned>(_countof(sz_l_time)),
				sz_l_az, static_cast<unsigned>(_countof(sz_l_az)),
				sz_l_el, static_cast<unsigned>(_countof(sz_l_el)),
				sz_l_rng, static_cast<unsigned>(_countof(sz_l_rng)),
				sz_m_date, static_cast<unsigned>(_countof(sz_m_date)),
				sz_m_time, static_cast<unsigned>(_countof(sz_m_time)),
				sz_m_az, static_cast<unsigned>(_countof(sz_m_az)),
				sz_m_el, static_cast<unsigned>(_countof(sz_m_el)),
				sz_m_rng, static_cast<unsigned>(_countof(sz_m_rng)),
				sz_date, static_cast<unsigned>(_countof(sz_date)),
				sz_time, static_cast<unsigned>(_countof(sz_time)),
				sz_pred_path, static_cast<unsigned>(_countof(sz_pred_path)),
				sz_offset, static_cast<unsigned>(_countof(sz_offset)));
		}

		if (nCol < 16)
			continue;

		{
			if (nCol >= 25)
			{
				// �ǖ�
				pld.strStationName = CString(sz_stationname);
			}

			// �q����
			pld.strSatellite = CString(sz_satname);

			// �p�XID
			pld.strPassId = CString(sz_pass);

			// AOS�����̕ϊ�
			swprintf_s(sz_aos, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_a_date, sz_a_time, L".0000");
			if (ub_asc_to_asctime(sz_aos, sz_aos_date) < 0) {
				fclose(fp);
				return FALSE;
			}
			cTime = StrTimeToCTime(CString(sz_aos_date));
//#if 0	// �_�~�[
//			CTime ts = cTime;
//			tspan = now - cTime;
//			cTime += tspan;
//
//			if (sat == CString(sz_satname))
//			{
//				tspan = ts - bs;
//				cTime += tspan;
//			}
//			bs = cTime;
//#endif

			pld.tAOS = cTime.GetTime();

			// AOS AZ
			str = CString(sz_a_az);
			pld.dAosAz = _ttof(str.GetBuffer());
			str.ReleaseBuffer();

			// AOS EL
			str = CString(sz_a_el);
			pld.dAosEl = _ttof(str.GetBuffer());
			str.ReleaseBuffer();

			// LOS�����̕ϊ�
			swprintf_s(sz_los, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_l_date, sz_l_time, L".0000");
			if (ub_asc_to_asctime(sz_los, sz_los_date) < 0) {
				fclose(fp);
				return FALSE;
			}
			cTime = StrTimeToCTime(CString(sz_los_date));

//#if 0	// �_�~�[
//			CTime te = cTime;
//			tspan = now - cTime;
//			tspan2 = te - ts;
//			cTime += (tspan + tspan2);
//
//			if (sat == CString(sz_satname))
//			{
//				tspan = te - be;
//				cTime += tspan;
//			}
//			be = cTime;
//#endif

			pld.tLOS = cTime.GetTime();

			// LOS AZ
			str = CString(sz_l_az);
			pld.dLosAz = _ttof(str.GetBuffer());
			str.ReleaseBuffer();

			// LOS EL
			str = CString(sz_l_el);
			pld.dLosEl = _ttof(str.GetBuffer());
			str.ReleaseBuffer();

			// MAX EL�����̕ϊ�
			swprintf_s(sz_max, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_m_date, sz_m_time, L".0000");
			if (ub_asc_to_asctime(sz_max, sz_max_date) < 0) {
				fclose(fp);
				return FALSE;
			}
			cTime = StrTimeToCTime(CString(sz_max_date));

			pld.tMaxEl = cTime.GetTime();

			// MAX EL AZ
			str = CString(sz_m_az);
			pld.dMaxAz = _ttof(str.GetBuffer());
			str.ReleaseBuffer();

			// MAX EL EL
			str = CString(sz_m_el);
			pld.dMaxEl = _ttof(str.GetBuffer());
			str.ReleaseBuffer();

			// �\��l�t�@�C����
			pld.strPredPath = CString(sz_pred_path);

			// �I�t�Z�b�g
			str = CString(sz_offset);
			// �����𒊏o
			long nSign = 1;
			if (str.Left(1) == _T('-'))
			{
				nSign = -1;
				str.Remove(_T('-'));
			}
			// �E����Q���b
			CString strSS = str.Right(2);
			// �c�肪����":"�Ȃ̂ŕb��":"�̕����������O���A�����o��
			long nLength = str.GetLength();
			CString strMM = str.Left(nLength - (2 + 1));
			long nMM = _ttoi(strMM);
			long nSS = _ttoi(strSS);
			pld.tOffset = nSign * (nMM * 60 + nSS);

			if (nCol >= 25)
			{
				if (nCol == 25)
				{
					// �^�C�v
					pld.strFormat = _T("");
				}
				else
				{
					// �^�C�v
					pld.strFormat = CString(sz_format);
				}

				// �擾��
				pld.strGetFrom = CString(sz_get_from).MakeUpper();
			}

			m_listPredList.push_back(pld);

#if 1
			sat = CString(sz_satname);
#endif
		}
	}

	fclose(fp);

#ifdef _CUTNOCOMM

	SYSTEMTIME sys;
	GetSystemTime(&sys);
	sys.wSecond = 0;
	CTime local = CTime(sys);
	int pos = 0;
	//	for (int i = 0; i < sizeof(mStationString) / sizeof(mStationString[0]); i++)
	{
		if (m_listPredList.size() == 0){
			return FALSE;
		}
		int i = nStation;
		stPredListData pld = m_listPredList[0];
		const TCHAR* mSpaceString[]{
			_T("ASTRO-E2"),
				_T("HAYABUSA2"),
				_T("IKAROS"),
				_T("ASTRO-H"),
				_T("GEOTAIL"),
		};
		//for (int j = 0; j < sizeof(mSpaceString) / sizeof(mSpaceString[0]); j++)
		{
			int j = 0;
			// �ǖ�
			pld.strStationName = mStationString[i];
			// �q����
			pld.strSatellite = mSpaceString[j];
			// �p�XID
			pld.strPassId.Format(_T("170200-0%d%d0"), i, j);
			// AOS
			pld.tAOS = (local + CTimeSpan(0, i, 10 * (i + 1), 0)).GetTime();
			// LOS
			pld.tLOS = (local + CTimeSpan(0, i + 1, 20 * (i + 1), 0)).GetTime();
			// �\��l�t�@�C����
			pld.strPredPath = _T("/CONFIG/prd/planet-c/1601100100.prd");

			m_listPredList.push_back(pld);

			// �p�XID
			pld.strPassId.Format(_T("170200-9%d%d0"), i, j);
			// AOS
			pld.tAOS = (local + CTimeSpan(0, i + 1, 40 * (i + 1), 0)).GetTime();
			// LOS
			pld.tLOS = (local + CTimeSpan(0, i + 2, 50 * (i + 1), 0)).GetTime();

			// �\��l�t�@�C����(ishi)
			pld.strPredPath.Format(_T("/CONFIG/prd/%s/1702009%d%d0.prd"), mSpaceString[j], i, j);

			m_listPredList.push_back(pld);


			pos++;
		}
	}
#endif

	// �\��l�ꗗ�t�@�C�������X�V
	mGetSharePredFileTime[nStation] = time;

	return TRUE;
}


/*============================================================================*/
/*! CSatelliteData

-# �w��ǂ̗\��l�ꗗ�t�@�C�����X�V����Ă邩���ׂ�

@param  nStation�F�ǔԍ�
@retval	TRUE�F�X�V����Ă��� / FALSE�F�X�V����Ă��Ȃ�
*/
/*============================================================================*/
BOOL CSatelliteData::IsPredListFileUpdated(UINT nStation)
{
	CString strFilePath = theApp.GetShareFilePath(eFileType_PredList_File, nStation);

	// �t�@�C���X�V�����̎擾
	HANDLE hFile;
	hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE){
		mGetSharePredFileTime[nStation] = CTime(0);
		return FALSE;
	}
	FILETIME cfTime, afTime, wfTime;
	GetFileTime(hFile, &cfTime, &afTime, &wfTime);
	CloseHandle(hFile);

	CTime time(wfTime);
	if (time == mGetSharePredFileTime[nStation]){
		// �t�@�C���^�C���ɕω��Ȃ�
		return FALSE;
	}

	// �t�@�C���^�C���ɕω�����
	return TRUE;
}


/*============================================================================*/
/*! CSatelliteData

-# �e�L�X�g�^��Time��CTime�^�ɕϊ�����


@param  �Ȃ�
@retval ����(CTime)
*/
/*============================================================================*/
CTime CSatelliteData::StrTimeToCTime(CString strTime)
{
	int dot = strTime.Find(_T("."));
	if (dot != -1)
	{
		strTime = strTime.Mid(0, dot);
	}

	COleDateTime oleTime;
	if (oleTime.ParseDateTime(strTime) == false){
		return CTime(0);
	}
	int yy = oleTime.GetYear();
	if (yy <= 1970){
		return CTime(0);
	}

	CTime cTime(oleTime.GetYear(), oleTime.GetMonth(), oleTime.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());

	return cTime;
}

//#if 0
///*============================================================================*/
///*! CSatelliteData
//
//-# �v��ꗗ���擾����
//
//
//@param  �Ȃ�
//@retval �Ȃ�
//*/
///*============================================================================*/
//BOOL CSatelliteData::SetPlanList()
//{
//	FILE *fp;
//	char sz_buff[500];
//	char sz_stationname[50];
//	char sz_satname[50];
//	char sz_planned_pass[50];
//	char sz_planned_stime1[50];
//	char sz_planned_stime2[50];
//	char sz_planned_etime1[50];
//	char sz_planned_etime2[50];
//	char sz_planned_itime1[50];
//	char sz_planned_itime2[50];
//	char sz_pred_name[50];
//
//	char sz_pre[50];
//	char sz_pre_date[50];
//	char sz_aos[50];
//	char sz_aos_date[50];
//	char sz_los[50];
//	char sz_los_date[50];
//
//	char sz_get_from[50];
//	char sz_plan_type[50];
//	char sz_result_type[50];
//
//	int	i_valid_flag;
//	int	i_ofs_flag;
//	int	i_offset;
//
//	int nCol;
//
//	CTime	cTime;
//
//	CString strFilePath;
//	strFilePath.Format(_T("%s%s"), theApp.GetAppPath(), PLAN_LIST_FILE);
//
//	if (_wfopen_s(&fp, strFilePath, _T("r")) != 0)
//	{
//		//		fprintf(stderr, "%s\n", "�v��ꗗ�t�@�C���̃I�[�v���Ɏ��s���܂����B");
//		//		uc_opelog(NULL, NULL, BASE_OPE_NO + 15, NULL, NULL);
//		return FALSE;
//	}
//
//	m_listPlanList.clear();
//
//	fgets(sz_buff, sizeof(sz_buff), fp);                /*�v��ꗗ�t�@�C���̃w�b�_�[���̓ǂݔ�΂�*/
//	while (fgets(sz_buff, sizeof(sz_buff), fp) != 0){    /*�v��ꗗ�t�@�C���̓ǂݍ���*/
//		stPlanListData pld;
//		// �V�t�H�[�}�b�g�ǂݍ���
//		nCol = sscanf_s(sz_buff, "%s%s%s%s%s%s%s%s%s%s%d%d%d%s%s%s",
//			sz_stationname, sizeof(sz_stationname), sz_satname, sizeof(sz_satname), sz_planned_pass, sizeof(sz_planned_pass),
//			sz_planned_stime1, sizeof(sz_planned_stime1), sz_planned_stime2, sizeof(sz_planned_stime2),
//			sz_planned_etime1, sizeof(sz_planned_etime1), sz_planned_etime2, sizeof(sz_planned_etime2),
//			sz_planned_itime1, sizeof(sz_planned_itime1), sz_planned_itime2, sizeof(sz_planned_itime2),
//			sz_pred_name, sizeof(sz_pred_name), &i_valid_flag, &i_ofs_flag, &i_offset,
//			sz_get_from, sizeof(sz_get_from), sz_plan_type, sizeof(sz_plan_type), sz_result_type, sizeof(sz_result_type));
//
//		if (nCol < 16)
//		{
//			// ���t�H�[�}�b�g�ǂݍ���
//			sscanf_s(sz_buff, "%s%s%s%s%s%s%s%s%s%d%d%d",
//				sz_satname, sizeof(sz_satname), sz_planned_pass, sizeof(sz_planned_pass),
//				sz_planned_stime1, sizeof(sz_planned_stime1), sz_planned_stime2, sizeof(sz_planned_stime2),
//				sz_planned_etime1, sizeof(sz_planned_etime1), sz_planned_etime2, sizeof(sz_planned_etime2),
//				sz_planned_itime1, sizeof(sz_planned_itime1), sz_planned_itime2, sizeof(sz_planned_itime2),
//				sz_pred_name, sizeof(sz_pred_name), &i_valid_flag, &i_ofs_flag, &i_offset);
//		}
//
//		if (nCol == 16)
//		{
//			// �ǖ�
//			pld.strStationName = CString(sz_stationname).MakeUpper();
//		}
//
//		// �q����
//		pld.strSatelliteName = CString(sz_satname);
//
//		// �p�XID
//		pld.strPassId = CString(sz_planned_pass);
//
//		// �v���p�X�J�n����
//		sprintf_s(sz_pre, sizeof(sz_aos), "%s %s%s", sz_planned_itime1, sz_planned_itime2, ".0000");
//		if (ub_asc_to_asctime(sz_pre, sz_pre_date) < 0) {
//			fclose(fp);
//			return FALSE;
//		}
//		cTime = StrTimeToCTime(CString(sz_pre_date));
////		pld.tPrePassStart = cTime.GetTime() + i_offset;
//		pld.tPrePassStart = cTime.GetTime();
//
//		// �p�X�J�n����
//		sprintf_s(sz_aos, sizeof(sz_aos), "%s %s%s", sz_planned_stime1, sz_planned_stime2, ".0000");
//		if (ub_asc_to_asctime(sz_aos, sz_aos_date) < 0) {
//			fclose(fp);
//			return FALSE;
//		}
//		cTime = StrTimeToCTime(CString(sz_aos_date));
////		pld.tPassStart = cTime.GetTime() + i_offset;
//		pld.tPassStart = cTime.GetTime();
//
//		// �p�X�I������
//		sprintf_s(sz_los, sizeof(sz_los), "%s %s%s", sz_planned_etime1, sz_planned_etime2, ".0000");
//		if (ub_asc_to_asctime(sz_los, sz_los_date) < 0) {
//			fclose(fp);
//			return FALSE;
//		}
//		cTime = StrTimeToCTime(CString(sz_los_date));
////		pld.tPassEnd = cTime.GetTime() + i_offset;
//		pld.tPassEnd = cTime.GetTime();
//
//		// �\��l�t�@�C����
//		pld.strPredName = CString(sz_pred_name);
//
//		// �`�F�b�N���ʃt���O
//		pld.lValidFlag = i_valid_flag;
//
//		// �����I�t�Z�b�g�Ǐ]�����t���O
//		pld.lOfsFlag = i_ofs_flag;
//
//		// �����I�t�Z�b�g
//		pld.tOffset = i_offset;
//
//		if (nCol == 16)
//		{
//			// �擾��
//			CString strGetFrom = CString(sz_get_from);
//			if (strGetFrom.Compare(_T("enp")) == 0)
//			{
//				pld.strGetFrom = CString(_T("ENP"));
//			}
//			else if (strGetFrom.Compare(_T("sttr")) == 0)
//			{
//				pld.strGetFrom = CString(_T("�Ǌǐ�"));
//			}
//
//			// �^�p���
//			CString strPlanType = CString(sz_plan_type);
//			if (strPlanType.Compare(_T("maint")) == 0)
//			{
//				pld.strPlanType = CString(_T("�ǐ�L"));
//			}
//			else if (strPlanType.Compare(_T("plan")) == 0)
//			{
//				pld.strPlanType = CString(_T("���^�p"));
//			}
//			else if (strPlanType.Compare(_T("vlbi")) == 0)
//			{
//				pld.strPlanType = CString(_T("VLBI"));
//			}
//			else if (strPlanType.Compare(_T("rdychk")) == 0)
//			{
//				pld.strPlanType = CString(_T("���f�B�l�X�`�F�b�N"));
//			}
//
//			// ���s����
//			CString strResultype = CString(sz_result_type);
//			if (strResultype.Compare(_T("ok")) == 0)
//			{
//				pld.strResult = CString(_T("����I��"));
//			}
//			else if (strResultype.Compare(_T("ng")) == 0)
//			{
//				pld.strResult = CString(_T("�ُ�I��"));
//			}
//			else if (strResultype.Compare(_T("unexec")) == 0)
//			{
//				pld.strResult = CString(_T("--------"));
//			}
//		}
//
//		// ���X�g�ɒǉ�
//		m_listPlanList.push_back(pld);
//	}
//
//	fclose(fp);
//
//#ifdef _NOCOMM
//	SYSTEMTIME sys;
//	GetSystemTime(&sys);
//	sys.wSecond = 0;
//	CTime local = CTime(sys);
//	int pos = 0;
//	for (int i = 0; i < sizeof(mStationString) / sizeof(mStationString[0]); i++){
//		stPlanListData pld;
//		const TCHAR* mSpaceString[]{
//			_T("ASTRO-E2"),
//				_T("HAYABUSA2"),
//				_T("IKAROS"),
//				_T("ASTRO-H"),
//				_T("GEOTAIL"),
//		};
//		for (int j = 0; j < sizeof(mSpaceString) / sizeof(mSpaceString[0]); j++){
//			// �ǖ�
//			pld.strStationName = mStationString[i];
//			// �q����
//			pld.strSatelliteName = mSpaceString[j];
//			// �p�XID
//			pld.strPassId.Format(_T("170200-0%d%d%d"), i, j, j + 1);
//			// �p�X�J�n����
//			pld.tPassStart = (local + CTimeSpan(0, 0, 10 * (j + 1), 0)).GetTime();
//			// �p�X�I������
//			pld.tPassEnd = (local + CTimeSpan(0, 1, 10 * (j + 1), 0)).GetTime();
//			// �v���p�X�J�n����
//			pld.tPrePassStart = pld.tPassStart;
//			if (i == eStation_Usuda64 || i == eStation_Usuda642)
//				pld.tPrePassStart -= OperationTime_AntennaWait;
//			else
//				pld.tPrePassStart -= OperationTime_AntennaWait2;
//			// �\��l�t�@�C����
//			pld.strPredName = _T("/CONFIG/prd/planet-c/1601100100.prd");
//			// �`�F�b�N���ʃt���O
//			pld.lValidFlag = 0;
//			// �����I�t�Z�b�g�Ǐ]�����t���O
//			pld.lOfsFlag = 0;
//			// �����I�t�Z�b�g
//			pld.tOffset = 0;
//			// �擾��
//			pld.strGetFrom = (pos % 2 == 0) ? _T("ENP") : _T("�Ǌǐ�");
//			// �^�p���
//			if (pos % 3 == 0)	pld.strPlanType = _T("���^�p");
//			if (pos % 3 == 1)	pld.strPlanType = _T("�ǐ�L");
//			if (pos % 3 == 2)	pld.strPlanType = _T("VLBI");
//			// ���s����
//			if (pos % 3 == 0)	pld.strResult = _T("����I��");
//			if (pos % 3 == 1)	pld.strResult = _T("�ُ�I��");
//			if (pos % 3 == 2)	pld.strResult = _T("--------");
//			pld.strResult = _T("--------");
//
//			m_listPlanList.push_back(pld);
//
//			// �p�XID
//			pld.strPassId.Format(_T("170201-0%d%d%d"), i, j, j + 1);
//			// �p�X�J�n����
//			pld.tPassStart = (local + CTimeSpan(0, 0, 15 * (j + 1), 0)).GetTime();
//			// �p�X�I������
//			pld.tPassEnd = (local + CTimeSpan(0, 1, 15 * (j + 1), 0)).GetTime();
//			// �v���p�X�J�n����
//			pld.tPrePassStart = pld.tPassStart;
//			if (i == eStation_Usuda64 || i == eStation_Usuda642)
//				pld.tPrePassStart -= OperationTime_AntennaWait;
//			else
//				pld.tPrePassStart -= OperationTime_AntennaWait2;
//			m_listPlanList.push_back(pld);
//
//			if (i == 0 && j == 0){
//				// �p�XID
//				pld.strPassId.Format(_T("179999-0%d%d%d"), i, j, j + 1);
//				// �p�X�J�n����
//				pld.tPassStart = (local + CTimeSpan(0, 0, 20 * (j + 1), 0)).GetTime();
//				// �p�X�I������
//				pld.tPassEnd = (local + CTimeSpan(0, 1, 20 * (j + 1), 0)).GetTime();
//				// �v���p�X�J�n����
//				pld.tPrePassStart = pld.tPassStart;
//				if (i == eStation_Usuda64 || i == eStation_Usuda642)
//					pld.tPrePassStart -= OperationTime_AntennaWait;
//				else
//					pld.tPrePassStart -= OperationTime_AntennaWait2;
//				m_listPlanList.push_back(pld);
//			}
//
//			pos++;
//		}
//	}
//#endif
//
//	return TRUE;
//}
//#endif
//
//#if 0
///*============================================================================*/
///*! CSatelliteData
//
//-# �v��ꗗ���擾����
//
//@param	�Ȃ�
//@retval	1�F���� / -1�F���s
//
//*/
///*============================================================================*/
//BOOL CSatelliteData::SetPlanList_Utf8()
//{
//	BOOL ret = TRUE;
//
//	m_listPlanList.clear();
//
//	// 1�ǃ��[�h
//	if (theApp.GetSelectStationMode() == eStation_1)
//	{
//		ret = SetPlanList_Utf8(theApp.GetSelectStation());
//	}
//	// 4�ǃ��[�h
//	else
//	{
//		for (UINT i = 0; i < eStation_MAX; i++)
//		{
//			if (SetPlanList_Utf8(i) == FALSE)
//			{
//				ret = FALSE;
//			}
//		}
//	}
//
//	return ret;
//}
//
//
///*============================================================================*/
///*! CSatelliteData
//
//-# �w��ǂ̌v��ꗗ���擾����(UTF8�Ή��j
//
//@param  nStation�F�ǔԍ�
//@retval	1�F���� / -1�F���s
//*/
///*============================================================================*/
//BOOL CSatelliteData::SetPlanList_Utf8(UINT nStation)
//{
//	FILE *fp;
//	TCHAR sz_buff[500];
//	TCHAR sz_stationname[50];
//	TCHAR sz_satname[50];
//	TCHAR sz_planned_pass[50];
//	TCHAR sz_planned_stime1[50];
//	TCHAR sz_planned_stime2[50];
//	TCHAR sz_planned_etime1[50];
//	TCHAR sz_planned_etime2[50];
//	TCHAR sz_planned_itime1[50];
//	TCHAR sz_planned_itime2[50];
//	TCHAR sz_pred_name[50];
//
//	TCHAR sz_pre[50];
//	TCHAR sz_pre_date[50];
//	TCHAR sz_aos[50];
//	TCHAR sz_aos_date[50];
//	TCHAR sz_los[50];
//	TCHAR sz_los_date[50];
//
//	TCHAR sz_get_from[50];
//	TCHAR sz_plan_type[50];
//
//	int	i_valid_flag;
//	int	i_ofs_flag;
//	int	i_offset;
//
//	int nCol;
//
//	//	CString strFilePath;
//	//	strFilePath.Format(_T("%s%s"), theApp.GetAppPath(), PLAN_LIST_FILE);
//	CString strFilePath = theApp.GetShareFilePath(eFileType_PlanList_File, nStation);
//
//
//	// �t�@�C���X�V�����̎擾
//	HANDLE hFile;
//	hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
//	if (hFile == INVALID_HANDLE_VALUE){
//		return FALSE;
//	}
//	FILETIME cfTime, afTime, wfTime;
//	GetFileTime(hFile, &cfTime, &afTime, &wfTime);
//	CloseHandle(hFile);
//
//	CTime time(wfTime);
//	if (time <= mGetSharePlanFileTime[nStation]){
//		// �t�@�C���^�C���ɕω��Ȃ�
//		return FALSE;
//	}
//	mGetSharePlanFileTime[nStation] = time;
//
//
//	CTime	cTime;
//
//	if (_wfopen_s(&fp, strFilePath, _T("r, ccs=UTF-8")) != 0)
//	{
//		//		fprintf(stderr, "%s\n", "�v��ꗗ�t�@�C���̃I�[�v���Ɏ��s���܂����B");
//		//		uc_opelog(NULL, NULL, BASE_OPE_NO + 15, NULL, NULL);
//		return FALSE;
//	}
//
//	fgetws(sz_buff, static_cast<unsigned>(_countof(sz_buff)), fp);                /*�v��ꗗ�t�@�C���̃w�b�_�[���̓ǂݔ�΂�*/
//	while (fgetws(sz_buff, static_cast<unsigned>(_countof(sz_buff)), fp) != 0){    /*�v��ꗗ�t�@�C���̓ǂݍ���*/
//		stPlanListData pld;
//
//		// �V�t�H�[�}�b�g�ǂݍ���
//		nCol = swscanf_s(sz_buff, L"%s%s%s%s%s%s%s%s%s%s%d%d%d%s%s",
//			sz_stationname, static_cast<unsigned>(_countof(sz_stationname)), 
//			sz_satname, static_cast<unsigned>(_countof(sz_satname)), 
//			sz_planned_pass, static_cast<unsigned>(_countof(sz_planned_pass)),
//			sz_planned_stime1, static_cast<unsigned>(_countof(sz_planned_stime1)),
//			sz_planned_stime2, static_cast<unsigned>(_countof(sz_planned_stime2)),
//			sz_planned_etime1, static_cast<unsigned>(_countof(sz_planned_etime1)),
//			sz_planned_etime2, static_cast<unsigned>(_countof(sz_planned_etime2)),
//			sz_planned_itime1, static_cast<unsigned>(_countof(sz_planned_itime1)),
//			sz_planned_itime2, static_cast<unsigned>(_countof(sz_planned_itime2)),
//			sz_pred_name, static_cast<unsigned>(_countof(sz_pred_name)), 
//			&i_valid_flag, &i_ofs_flag, &i_offset,
//			sz_get_from, static_cast<unsigned>(_countof(sz_get_from)),
//			sz_plan_type, static_cast<unsigned>(_countof(sz_plan_type)));
//
//		if (nCol < 15)
//		{
//			// ���t�H�[�}�b�g�ǂݍ���
//			swscanf_s(sz_buff, L"%s%s%s%s%s%s%s%s%s%d%d%d",
//				sz_satname, static_cast<unsigned>(_countof(sz_satname)),
//				sz_planned_pass, static_cast<unsigned>(_countof(sz_planned_pass)),
//				sz_planned_stime1, static_cast<unsigned>(_countof(sz_planned_stime1)),
//				sz_planned_stime2, static_cast<unsigned>(_countof(sz_planned_stime2)),
//				sz_planned_etime1, static_cast<unsigned>(_countof(sz_planned_etime1)),
//				sz_planned_etime2, static_cast<unsigned>(_countof(sz_planned_etime2)),
//				sz_planned_itime1, static_cast<unsigned>(_countof(sz_planned_itime1)),
//				sz_planned_itime2, static_cast<unsigned>(_countof(sz_planned_itime2)),
//				sz_pred_name, static_cast<unsigned>(_countof(sz_pred_name)),
//				&i_valid_flag, &i_ofs_flag, &i_offset);
//		}
//
//		if (nCol == 15)
//		{
//			// �ǖ�
//			pld.strStationName = CString(sz_stationname);
//		}
//
//		// �q����
//		pld.strSatelliteName = CString(sz_satname);
//
//		// �p�XID
//		pld.strPassId = CString(sz_planned_pass);
//
//		// �v���p�X�J�n����
//		swprintf_s(sz_pre, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_planned_itime1, sz_planned_itime2, L".0000");
//		if (ub_asc_to_asctime(sz_pre, sz_pre_date) < 0) {
//			fclose(fp);
//			return FALSE;
//		}
//		cTime = StrTimeToCTime(CString(sz_pre_date));
//		pld.tPrePassStart = cTime.GetTime() + i_offset;
//
//		// �p�X�J�n����
//		swprintf_s(sz_aos, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_planned_stime1, sz_planned_stime2, L".0000");
//		if (ub_asc_to_asctime(sz_aos, sz_aos_date) < 0) {
//			fclose(fp);
//			return FALSE;
//		}
//		cTime = StrTimeToCTime(CString(sz_aos_date));
//		pld.tPassStart = cTime.GetTime() + i_offset;
//
//		// �p�X�I������
//		swprintf_s(sz_los, static_cast<unsigned>(_countof(sz_los)), L"%s %s%s", sz_planned_etime1, sz_planned_etime2, L".0000");
//		if (ub_asc_to_asctime(sz_los, sz_los_date) < 0) {
//			fclose(fp);
//			return FALSE;
//		}
//		cTime = StrTimeToCTime(CString(sz_los_date));
//		pld.tPassEnd = cTime.GetTime() + i_offset;
//
//		// �\��l�t�@�C����
//		pld.strPredName = CString(sz_pred_name);
//
//		// �v��t�@�C����
//		CString strPlanPath = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
//		pld.strPlanName = strPlanPath + CString(sz_satname) + _T("\\") + CString(sz_planned_pass) + _T(".pln");
//
//		// �`�F�b�N���ʃt���O
//		pld.lValidFlag = i_valid_flag;
//
//		// �����I�t�Z�b�g�Ǐ]�����t���O
//		pld.lOfsFlag = i_ofs_flag;
//
//		// �����I�t�Z�b�g
//		pld.tOffset = i_offset;
//
//		if (nCol == 15)
//		{
//			pld.strGetFrom = CString(sz_get_from).MakeUpper();
//			pld.strPlanType = CString(sz_plan_type).MakeUpper();
//		}
//
//		// ���X�g�ɒǉ�
//		m_listPlanList.push_back(pld);
//	}
//
//	fclose(fp);
//
//	return TRUE;
//}
//#endif


/*============================================================================*/
/*! CSatelliteData

-# �v��ꗗ���擾����

@param	�Ȃ�
@retval	1�F���� / -1�F���s

*/
/*============================================================================*/
BOOL CSatelliteData::SetPlanList_Euc()
{
	BOOL ret = FALSE;

	m_listPlanList.clear();

	// 1�ǃ��[�h
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		UINT nStation = theApp.GetSelectStation();
		// �v��ꗗ�t�@�C�����X�V����Ă��邩���ׂ�
		if (IsPlanListFileUpdated(nStation) == TRUE)
		{
			// �v��ꗗ�t�@�C�����X�V����Ă���ꍇ�A�v��ꗗ�t�@�C���ǂݍ���
			ret = SetPlanList_Euc(nStation);
		}
	}
	// 4�ǃ��[�h
	else
	{
		BOOL bUpdated = FALSE;
		// �v��ꗗ�t�@�C�����X�V����Ă��邩���ׂ�
		for (UINT i = 0; i < eStation_MAX; i++)
		{
			if (IsPlanListFileUpdated(i) == TRUE)
			{
				bUpdated = TRUE;
			}
		}

		// �v��ꗗ�t�@�C�����X�V����Ă���ꍇ�A�v��ꗗ�t�@�C���ǂݍ���
		if (bUpdated == TRUE)
		{
			for (UINT i = 0; i < eStation_MAX; i++)
			{
				if (SetPlanList_Euc(i) == TRUE)
				{
					ret = TRUE;
				}
			}
		}
	}

	return ret;
}


/*============================================================================*/
/*! CSatelliteData

-# �w��ǂ̌v��ꗗ���擾����(EUC�Ή��j

@param  nStation�F�ǔԍ�
@retval	1�F���� / -1�F���s
*/
/*============================================================================*/
BOOL CSatelliteData::SetPlanList_Euc(UINT nStation)
{
	FILE *fp;
	TCHAR sz_stationname[50] = { 0 };
	TCHAR sz_satname[50] = { 0 };
	TCHAR sz_planned_pass[50] = { 0 };
	TCHAR sz_planned_stime1[50];
	TCHAR sz_planned_stime2[50];
	TCHAR sz_planned_etime1[50];
	TCHAR sz_planned_etime2[50];
	TCHAR sz_planned_itime1[50];
	TCHAR sz_planned_itime2[50];
	TCHAR sz_pred_name[50] = { 0 };

	TCHAR sz_pre[50];
	TCHAR sz_pre_date[50];
	TCHAR sz_aos[50];
	TCHAR sz_aos_date[50];
	TCHAR sz_los[50];
	TCHAR sz_los_date[50];

	TCHAR sz_get_from[50] = { 0 };
	TCHAR sz_plan_type[50] = { 0 };

	int	i_valid_flag;
	int	i_ofs_flag;
	int	i_offset;

	int nCol;

	CString strFilePath = theApp.GetShareFilePath(eFileType_PlanList_File, nStation);

	// �t�@�C���X�V�����̎擾
	HANDLE hFile;
	hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE){
		mGetSharePlanFileTime[nStation] = CTime(0);
		return FALSE;
	}
	FILETIME cfTime, afTime, wfTime;
	GetFileTime(hFile, &cfTime, &afTime, &wfTime);
	CloseHandle(hFile);

	CTime time(wfTime);
//	if (time <= mGetSharePlanFileTime[nStation]){
//		// �t�@�C���^�C���ɕω��Ȃ�
//		return FALSE;
//	}
//	mGetSharePlanFileTime[nStation] = time;

	CTime	cTime;

	if (_wfopen_s(&fp, strFilePath, _T("r")) != 0 || fp == NULL)
	{
		//		fprintf(stderr, "%s\n", "�v��ꗗ�t�@�C���̃I�[�v���Ɏ��s���܂����B");
		//		uc_opelog(NULL, NULL, BASE_OPE_NO + 15, NULL, NULL);
		return FALSE;
	}

	char szline[2048];
	CString cbuf;
	fgets(szline, static_cast<unsigned>(_countof(szline)), fp);                /*�v��ꗗ�t�@�C���̃w�b�_�[���̓ǂݔ�΂�*/
	while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != 0){    /*�v��ꗗ�t�@�C���̓ǂݍ���*/
		cbuf = CServerFile::ConvertUnicode(szline);
		stPlanListData pld;

		// �V�t�H�[�}�b�g�ǂݍ���
		nCol = swscanf_s(cbuf, L"%s%s%s%s%s%s%s%s%s%s%d%d%d%s%s",
			sz_stationname, static_cast<unsigned>(_countof(sz_stationname)),
			sz_satname, static_cast<unsigned>(_countof(sz_satname)),
			sz_planned_pass, static_cast<unsigned>(_countof(sz_planned_pass)),
			sz_planned_stime1, static_cast<unsigned>(_countof(sz_planned_stime1)),
			sz_planned_stime2, static_cast<unsigned>(_countof(sz_planned_stime2)),
			sz_planned_etime1, static_cast<unsigned>(_countof(sz_planned_etime1)),
			sz_planned_etime2, static_cast<unsigned>(_countof(sz_planned_etime2)),
			sz_planned_itime1, static_cast<unsigned>(_countof(sz_planned_itime1)),
			sz_planned_itime2, static_cast<unsigned>(_countof(sz_planned_itime2)),
			sz_pred_name, static_cast<unsigned>(_countof(sz_pred_name)),
			&i_valid_flag, &i_ofs_flag, &i_offset,
			sz_get_from, static_cast<unsigned>(_countof(sz_get_from)),
			sz_plan_type, static_cast<unsigned>(_countof(sz_plan_type)));

		if (nCol < 9)
			continue;

		if (nCol < 15)
		{
			// ���f�B�l�X�`�F�b�N�v��ł̗\��l�������ꍇ��z��
			swscanf_s(cbuf, L"%s%s%s%s%s%s%s%s%s%d%d%d%s%s",
				sz_stationname, static_cast<unsigned>(_countof(sz_stationname)),
				sz_satname, static_cast<unsigned>(_countof(sz_satname)),
				sz_planned_pass, static_cast<unsigned>(_countof(sz_planned_pass)),
				sz_planned_stime1, static_cast<unsigned>(_countof(sz_planned_stime1)),
				sz_planned_stime2, static_cast<unsigned>(_countof(sz_planned_stime2)),
				sz_planned_etime1, static_cast<unsigned>(_countof(sz_planned_etime1)),
				sz_planned_etime2, static_cast<unsigned>(_countof(sz_planned_etime2)),
				sz_planned_itime1, static_cast<unsigned>(_countof(sz_planned_itime1)),
				sz_planned_itime2, static_cast<unsigned>(_countof(sz_planned_itime2)),
				&i_valid_flag, &i_ofs_flag, &i_offset,
				sz_get_from, static_cast<unsigned>(_countof(sz_get_from)),
				sz_plan_type, static_cast<unsigned>(_countof(sz_plan_type)));

		}

//		if (nCol == 15)
		{
			// �ǖ�
			pld.strStationName = CString(sz_stationname);
		}

		// �q����
		pld.strSatelliteName = CString(sz_satname);

		// �p�XID
		pld.strPassId = CString(sz_planned_pass);

		// �v���p�X�J�n����
		swprintf_s(sz_pre, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_planned_itime1, sz_planned_itime2, L".0000");
		if (ub_asc_to_asctime(sz_pre, sz_pre_date) < 0) {
			fclose(fp);
			return FALSE;
		}
		cTime = StrTimeToCTime(CString(sz_pre_date));
		pld.tPrePassStart = cTime.GetTime() + i_offset;

		// �p�X�J�n����
		swprintf_s(sz_aos, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_planned_stime1, sz_planned_stime2, L".0000");
		if (ub_asc_to_asctime(sz_aos, sz_aos_date) < 0) {
			fclose(fp);
			return FALSE;
		}
		cTime = StrTimeToCTime(CString(sz_aos_date));
		pld.tPassStart = cTime.GetTime() + i_offset;

		// �p�X�I������
		swprintf_s(sz_los, static_cast<unsigned>(_countof(sz_los)), L"%s %s%s", sz_planned_etime1, sz_planned_etime2, L".0000");
		if (ub_asc_to_asctime(sz_los, sz_los_date) < 0) {
			fclose(fp);
			return FALSE;
		}
		cTime = StrTimeToCTime(CString(sz_los_date));
		pld.tPassEnd = cTime.GetTime() + i_offset;

		// �\��l�t�@�C����
		if (nCol == 15)
		{
			pld.strPredName = CString(sz_pred_name);
		}
		else
		{
			pld.strPredName = _T("");
		}

		// �`�F�b�N���ʃt���O
		pld.lValidFlag = i_valid_flag;

		// �����I�t�Z�b�g�Ǐ]�����t���O
		pld.lOfsFlag = i_ofs_flag;

		// �����I�t�Z�b�g
		pld.tOffset = i_offset;

//		if (nCol == 15)
		{
			pld.strGetFrom = CString(sz_get_from).MakeUpper();
			pld.strPlanType = CString(sz_plan_type).MakeUpper();
		}

		// �v��t�@�C����
		CString strPlanPath = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
		if (pld.strGetFrom.CompareNoCase(_T("ENP")) == 0)
		{
			pld.strPlanName = strPlanPath + _T("enp\\")  + CString(sz_satname) + _T("\\") + CString(sz_planned_pass) + _T(".pln");
		}
		else
		{
			pld.strPlanName = strPlanPath + _T("sttn\\") + CString(sz_satname) + _T("\\") + CString(sz_planned_pass) + _T(".pln");
		}

		// ���X�g�ɒǉ�
		m_listPlanList.push_back(pld);
	}

	fclose(fp);

	// �v��ꗗ�t�@�C�������X�V
	mGetSharePlanFileTime[nStation] = time;

	return TRUE;
}


/*============================================================================*/
/*! CSatelliteData

-# �w��ǂ̌v��ꗗ�t�@�C�����X�V����Ă邩���ׂ�

@param  nStation�F�ǔԍ�
@retval	TRUE�F�X�V����Ă��� / FALSE�F�X�V����Ă��Ȃ�
*/
/*============================================================================*/
BOOL CSatelliteData::IsPlanListFileUpdated(UINT nStation)
{
	CString strFilePath = theApp.GetShareFilePath(eFileType_PlanList_File, nStation);

	// �t�@�C���X�V�����̎擾
	HANDLE hFile;
	hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE){
		mGetSharePlanFileTime[nStation] = CTime(0);
		return FALSE;
	}
	FILETIME cfTime, afTime, wfTime;
	GetFileTime(hFile, &cfTime, &afTime, &wfTime);
	CloseHandle(hFile);

	CTime time(wfTime);
	if (time == mGetSharePlanFileTime[nStation]){
		// �t�@�C���^�C���ɕω��Ȃ�
		return FALSE;
	}

	// �t�@�C���^�C���ɕω�����
	return TRUE;
}


/*============================================================================*/
/*! CSatelliteData

-# �v��ꗗ�ɗ\��l�t�@�C�����g���Ă��邩���ׂ�

@param	strFilePath�F�\��l�t�@�C��
@retval TRUE: �g���Ă���B FALSE: �g���Ă��Ȃ��B

*/
/*============================================================================*/
BOOL CSatelliteData::IsUsePredFile(CString& strFilePath)
{
	CGraphMutex::Lock(eFile);
	vector<stPlanListData>::iterator itr;
	vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();

	for (itr = listPlan.begin(); itr != listPlan.end(); itr++)
	{
		if ((*itr).strPredName.Compare(strFilePath) == 0)
		{
			CGraphMutex::Unlock(eFile);
			return TRUE;
		}
	}
	CGraphMutex::Unlock(eFile);

	return FALSE;
}


/*============================================================================*/
/*! CSatelliteData

-# �ǖ��i�ȒP�j�����ID���擾����


@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
UINT CSatelliteData::GetSimpleStationID(CString name)
{
	UINT pos;
	for (pos = 0; pos < sizeof(mStationSimpleString) / sizeof(mStationSimpleString[0]); pos++){
		if (name == mStationSimpleString[pos]){
			return pos;
		}
	}
	return 0xffffffff;
}
/*============================================================================*/
/*! CSatelliteData

-# �ǖ��i�ʏ�j�����ID���擾����


@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
UINT CSatelliteData::GetStationID(CString name)
{
	UINT pos;
	for (pos = 0; pos < sizeof(mStationString) / sizeof(mStationString[0]); pos++){
		if (name == mStationString[pos]){
			return pos;
		}
	}
	return 0xffffffff;
}


/*============================================================================*/
/*! CSatelliteData

-# �T�}���v���p�e�B�̃f�[�^�𐶐�����


@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CSatelliteData::CreateSummaryPropertiesData()
{
#ifdef _NOCOMM
	int nStationMax = eStation_MAX;
	int nListMax = MAX_SUMMARYTDATA;
	int nGraphIdx;

	for (int nStation = 0; nStation < nStationMax; ++nStation)
	{
		vector<CString> listViewName;
		if (nStation == eStation_Uchinoura20)
		{
			CSummaryPropertiesDlg::GetViewTrendNameList(listViewName);
		}
		else
		{
			CSummaryPropertiesDlg::GetViewNameList(listViewName);
		}

		for (int nView = 0; nView < MAX_SUMMARY_VIEW; ++nView)
		{
			CPropertiesData prop = GetSummaryPropertiesData(nStation, nView);

			// �E�B���h�E�|�C���^���R�s�[
			prop.wnd = (LPARAM)this;

			if (nStation == eStation_Usuda642 && nView == eSummaryWndSetType_KASP)
			{
				nGraphIdx = eSummaryWndSetType_SAGC;
			}
			else
			{
				nGraphIdx = nView;
			}
			// �O���t�ԍ����R�s�[
			prop.nGraphIdx = nGraphIdx;

			if (!prop.m_bSetted)
			{
				prop.wndIdx = nView;									// �E�B���h�[�ԍ�
				prop.wndName = listViewName[nGraphIdx];						// �E�B���h�[����

				prop.isRealTime = true;									// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
				prop.trandVScale.Auto = true;							// �c���� �I�[�g�X�P�[��
				prop.trandVScale.VScalLMax = initSummaryPropData.VScalLMax;	// �c���� �ő�l
				prop.trandVScale.VScalLMin = initSummaryPropData.VScalLMin;	// �c���� �ŏ��l
				prop.trandVScale.VSCalItvl = initSummaryPropData.VSCalItvl;	// �c���ڐ���̊Ԋu

				prop.trandHScale[0] = m1;			// �����X�P�[��

				CString str = _T("");
				for (int nSigNo = 0; nSigNo < nListMax; ++nSigNo)
				{
					prop.dataName[nSigNo] = mListSummarySig[nStation][nGraphIdx][nSigNo].strSigName;			// �f�[�^����
					prop.dataGName[nSigNo] = mListSummarySig[nStation][nGraphIdx][nSigNo].strSimpleSigName;	// �ȈՃf�[�^����
					prop.dataUnit[nSigNo] = mListSummarySig[nStation][nGraphIdx][nSigNo].strUnit;			// �P��
					prop.isViewMode[nSigNo] = true;		// �\��On/Off
					prop.drawMode[nSigNo] = Line;		// �`�惂�[�h�iLine�ADots�AL&D�j
					prop.color[nSigNo] = colordef[nSigNo];	// �\���F
					prop.lineWidth[nSigNo] = 1;			// ����
					prop.dotSize[nSigNo] = Small;		// �h�b�g�T�C�Y
					prop.vscaleKind[nSigNo] = LScale;	// �c���X�P�[���I��
				}
			}
			SetSummaryPropertiesData(nStation, nView, prop);
		}
	}

#endif

}


/*============================================================================*/
/*! CSatelliteData

-# �ݔ��ڍא���@�\

@param  strCtrl		:�ݔ�����
@param  strParam	:�p�����[�^
@retval ���䑗�M�l�B�L�����Z�����͋󕶎���Ԃ��B
*/
/*============================================================================*/
CString CSatelliteData::opctproc(CString strCtrl, CString strParam, CString val/* = _T("")*/)
{
	CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("opctproc"), strCtrl+","+strParam, _T(""), nLogEx::info);

	if (strParam != _T("") && strParam != _T("-nd"))
	{
		/* ����ʒm�𑗐M */
		theApp.SendControl(strCtrl, strParam, _T(""), _T(""));
		return strParam;
	}
	else if (IsBitRateCtrl(strCtrl))
	{
		/* �r�b�g���[�g�ύX */
		return CtrlBitRate(strCtrl);
	}
	else
	{
		/* �X�e�[�^�X/���l�E����/�m�F�ɐU�蕪�� */
		CString str = _T("");
		stControlValue cval;
		cval.fval = _wtof(val);
		int ret = param_chk(strCtrl, &cval);

		// ����ʒm�𑗐M
		switch (ret){
		case	eControlStatus:
			str.Format(_T("%s"), (LPCTSTR)CString(cval.buf));
			break;
		case	eControlNumeric:
			str.Format(_T("%d"), cval.val);
			break;
		case	eControlDouble:
			str.Format(_T("%f"), cval.fval);
			break;
		case	eControlTime:
			str.Format(_T("%s"), (LPCTSTR)CString(cval.buf));
			break;
		default:
			// �����ʕs��v�̏ꍇ�A���䑗�M���Ȃ�
			return _T("");
		}

		theApp.SendControl(strCtrl, str, _T(""), _T(""));
		return str;
	}

	// ���䑗�M�L�����Z��
	return _T("");
}

/*============================================================================*/
/*! CSatelliteData

-# ���䂪�R�}���h�r�b�g���[�g���m�F

@param  strCtrl		:���䖼
@retval TRUE�F�r�b�g���[�g�AFALSE�F���̑�
*/
/*============================================================================*/
BOOL CSatelliteData::IsCmdBitRateCtrl(CString strCtrl)
{
	if (theApp.GetSelectStation() == eStation_Usuda64)
	{
		// �P�c64
		if ((strCtrl == C_BIT)
			|| (strCtrl == C_BIT2)
			|| (strCtrl == C_BIT3)
			|| (strCtrl == C_BIT4))
		{
			return TRUE;
		}
	}
	else if (theApp.GetSelectStation() == eStation_Usuda642)
	{
		// �P�c54
		if (    strCtrl.CompareNoCase(C_BIT4) == 0
			|| (strCtrl == C_BIT)
			|| (strCtrl == C_BIT_X))
		{
			return TRUE;
		}
	}
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
	{
		// ���V�Y34
		if ((strCtrl == C_BIT)
			|| (strCtrl == C_BIT_X2))
		{
			return TRUE;
		}
	}
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
	{
		// ���V�Y20
		if (strCtrl == C_BIT)
		{
			return TRUE;
		}
	}

	return FALSE;
}
/*============================================================================*/
/*! CSatelliteData

-# ���䂪�R�}���h�r�b�g���[�g���m�F

@param  strCtrl		:���䖼
@retval TRUE�F�r�b�g���[�g�AFALSE�F���̑�
*/
/*============================================================================*/
BOOL CSatelliteData::IsBitRateCtrl(CString strCtrl)
{
	if (theApp.GetSelectStation() == eStation_Usuda64)
	{
		// �P�c64
		if ((strCtrl == BIT_S)
			|| (strCtrl == BIT_X)
			|| (strCtrl == BIT_X2)
			|| (strCtrl == BIT_XQPSK)
			|| (strCtrl == C_BIT)
			|| (strCtrl == C_BIT2)
			|| (strCtrl == C_BIT3)
			|| (strCtrl == C_BIT4))
		{
			return TRUE;
		}
	}
	else if (theApp.GetSelectStation() == eStation_Usuda642)
	{
		// �P�c54
		if (   strCtrl.CompareNoCase(BIT_X2) == 0
			|| strCtrl.CompareNoCase(BIT_KA) == 0
			|| strCtrl.CompareNoCase(C_BIT4) == 0
			|| (strCtrl == BIT_S)
			|| (strCtrl == BIT_X)
			|| (strCtrl == BIT_KA)
			|| (strCtrl == BIT_XQPSK)
			|| (strCtrl == C_BIT)
			|| (strCtrl == C_BIT_X))
		{
			return TRUE;
		}
	}
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
	{
		// ���V�Y34
		if ((strCtrl == BIT_S)
			|| (strCtrl == BIT_X)
			|| (strCtrl == BIT_X2)
			|| (strCtrl == X_CMD2_BITRATE)
			|| (strCtrl == S_ACQ_BITRATE)
			|| (strCtrl == X_ACQ_BITRATE)
			|| (strCtrl == S_TRKRX_BITRATE)
			|| (strCtrl == BIT_XQPSK)
			|| (strCtrl == BIT_XQPSK20)
			|| (strCtrl == BIT_TRKRX)
			|| (strCtrl == C_BIT)
			|| (strCtrl == C_BIT_X))
		{
			return TRUE;
		}
	}
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
	{
		// ���V�Y20
#if 1
		if ((strCtrl == BIT_S)
			|| (strCtrl == BIT_X)
			|| (strCtrl == BIT_XQPSK20)
			|| (strCtrl == C_BIT)
			|| (strCtrl == X_HRX_DATARATE))
#else
		if ((strCtrl == BIT_S)
			|| (strCtrl == BIT_X)
			|| (strCtrl == BIT_XQPSK20)
			|| (strCtrl == C_BIT)
			|| (strCtrl == S_ACQT_BITRATE)
			|| (strCtrl == X_HRX_DATARATE)
			|| (strCtrl == SS_BITRATE))
#endif
		{
			return TRUE;
		}
	}

	return FALSE;
}


/*============================================================================*/
/*! CSatelliteData

-# ���䂪PLL BW���m�F

@param  strCtrl		:���䖼
@retval TRUE:PLL BW����@�^ FALSE:PLL BW����łȂ�
*/
/*============================================================================*/
BOOL CSatelliteData::IsPllBwCtrl(CString strCtrl)
{
	if (theApp.GetSelectStation() == eStation_Usuda64)
	{
		// �P�c64
		if ((strCtrl == SRCV_PLLBW)
			|| (strCtrl == XRCV_PLLBW)
			|| (strCtrl == XRCVOLD_PLLBW))
		{
			return TRUE;
		}
	}
	else if (theApp.GetSelectStation() == eStation_Usuda642)
	{
		// �P�c54
		if ((strCtrl == SRCV_PLLBW)
			|| (strCtrl == XRCV_PLLBW)
			|| (strCtrl == XRCVOLD_PLLBW))
		{
			return TRUE;
		}
	}
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
	{
		// ���V�Y34
		if ((strCtrl == SRCV34_PLLBW)
			|| (strCtrl == XRCV34_PLLBW)
			|| (strCtrl == SCAP34_PLLBW)
			|| (strCtrl == XCAP34_PLLBW)
			|| (strCtrl == STRACK34_PLLBW)
			|| (strCtrl == XTRACK34_PLLBW))
		{
			return TRUE;
		}
	}
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
	{
		// ���V�Y20
	}

	return FALSE;
}


/*============================================================================*/
/*! CSatelliteData

-# �ݔ��ڍא���@�\

@param  strCtrl		:�ݔ�����
@param  strParam	:�p�����[�^
@param  strCtrl2    :�ݔ�����2(�f�t�H���g�͋�)
@retval �Ȃ�
*/
/*============================================================================*/
void CSatelliteData::SendControlSelect(CString strCtrl, stControlValue* pval, CString strCtrl2 /*= _T("")*/)
{
	if (IsBitRateCtrl(strCtrl))
	{
		/* �r�b�g���[�g�ύX */
		CtrlBitRate(strCtrl);

		// �r�b�g���[�g�̐���́ACtrlBitRate�֐����ő��M���Ă���B
		// �߂��Ő���𑗐M���Ȃ��ׂ�0��ݒ肷��B
		pval->ret = 0;
	}
	else if (IsPllBwCtrl(strCtrl))
	{
		/* �X�e�[�^�X/���l�E����/�m�F�ɐU�蕪��(PLL BW) */
		param_chk(strCtrl, pval, CURRENT_LBW);
	}
	else
	{
		/* �X�e�[�^�X/���l�E����/�m�F�ɐU�蕪�� */
		param_chk(strCtrl, pval, _T(""), FALSE, strCtrl2);
	}
}


/*============================================================================*/
/*! CSatelliteData

-# ����I��o�^

@param  strCtrl		:�ݔ�����
@param  pval		:�p�����[�^
@retval ������
*/
/*============================================================================*/
int CSatelliteData::RegistControlSelect(CString strCtrl, stControlValue* pval)
{
	int ret = -1;
	if (IsBitRateCtrl(strCtrl))
	{
		/* �r�b�g���[�g�ύX */
		if (CtrlBitRate(strCtrl, pval, TRUE) != _T(""))
			ret = eControlNumeric;
			
		// �r�b�g���[�g�̐���́ACtrlBitRate�֐����ő��M���Ă���B
		// �߂��Ő���𑗐M���Ȃ��ׂ�0��ݒ肷��B
		pval->ret = 0;
	}
	else if (IsPllBwCtrl(strCtrl))
	{
		/* �X�e�[�^�X/���l�E����/�m�F�ɐU�蕪��(PLL BW) */
		ret = param_chk(strCtrl, pval, CURRENT_LBW, TRUE);
	}
	else
	{
		/* �X�e�[�^�X/���l�E����/�m�F�ɐU�蕪�� */
		ret = param_chk(strCtrl, pval, _T(""), TRUE);
	}

	return ret;
}


/*============================================================================*/
/*! CSatelliteData

-# �ڍא�����-�r�b�g���[�g�ύX����

@param  strCtrl		:�ݔ�����
@param  pval		:�p�����[�^
@param	bSeqCtrl:TRUE:��������ł��� / FALSE:��������łȂ��i�f�t�H���g�j
@retval ���M��������p�����[�^�i���M���s��������p�����[�^��Ԃ��j�B�L�����Z�����͋󕶎��B
*/
/*============================================================================*/
CString CSatelliteData::CtrlBitRate(CString strCtrl, stControlValue* pval /*= NULL*/, BOOL bSeqCtrl /*= FALSE*/)
{
	CEquipmentCtrlBitRate dlg;
	if (bSeqCtrl)
	{
		dlg.SetWindowTitle(_T("����o�^"));
	}

	dlg.SetBitRateCtrl(strCtrl);

	// CMD-BITRATE�̏ꍇ�́A�������͂��o���Ȃ��悤�ɂ���B
	BOOL bCmdBitRate = FALSE;
	if (IsCmdBitRateCtrl(strCtrl))
	{
		bCmdBitRate = TRUE;
	}
	dlg.SetCmdBitRateFlg(bCmdBitRate);

	// �_�C�A���O�N��
	if (dlg.DoModal() == IDCANCEL)
	{
		return _T("");
	}

	CEquipmentCtrlBitRate::stBitRate st;
	st = dlg.GetBitRateSetting();
	CString time;
	CString msg = _T("");
	if (!bSeqCtrl)
	{
		// ��������łȂ��ꍇ�A���M����
		if (st.bTime == TRUE)
		{
			// �����ݒ�`�F�b�N�{�b�N�X�Ƀ`�F�b�N���t���Ă���ꍇ
			time = st.time.Format("%Y-%m-%d %H:%M:%S");
			msg.Format(_T("%s,%s,%s"), (LPCTSTR)st.bitrate, (LPCTSTR)st.satellite, (LPCTSTR)time);
			theApp.SendControlNoUI(strCtrl, msg, _T(""), _T(""));
		}
		else if (!bCmdBitRate)
		{
			// �w�����w��x�͊����Ȃ̂ŃJ���}�͕K�v
			msg.Format(_T("%s,%s,"), (LPCTSTR)st.bitrate, (LPCTSTR)st.satellite);
			theApp.SendControlNoUI(strCtrl, msg, _T(""), _T(""));
		}
		else
		{
			// �w�����w��x�͔񊈐��Ȃ̂ŃJ���}�͕s�v
			msg.Format(_T("%s,%s"), (LPCTSTR)st.bitrate, (LPCTSTR)st.satellite);
			theApp.SendControlNoUI(strCtrl, msg, _T(""), _T(""));
		}
	}

	if (pval != NULL)
	{
		CString ctrl = strCtrl;
		if (st.bitrate != _T(""))
		{
			ctrl += _T(" ") + st.bitrate;
			if (st.satellite != _T(""))
			{
				ctrl += _T(" ") + st.satellite;
				if (st.bTime == TRUE)
				{
					ctrl += _T(" ") + time;
				}
			}
		}
		sprintf_s(pval->buf, 80, "%s", (LPCSTR)CStringA(ctrl));
	}
	return msg;
}


/*============================================================================*/
/*! CSatelliteData

-# �ݔ��ڍא���@�\

@param  strCtrl		 :�ݔ�����
@param  strParam	 :�p�����[�^
@param	strAddStatus :�ǉ��X�e�[�^�X������i�f�t�H���g�͋�j
@param	bSeqCtrl:TRUE:��������ł��� / FALSE:��������łȂ��i�f�t�H���g�j
@param  strCtrl2     :�ݔ�����2(�f�t�H���g�͋�)
@retval ������

*/
/*============================================================================*/
int CSatelliteData::param_chk(CString strCtrl, stControlValue* pval/*=NULL*/, CString strAddStatus /*= _T("")*/, BOOL bSeqCtrl /*= FALSE*/, CString strCtrl2 /*= _T("")*/)
{
	struct ctlname_t *ctrl_adr;
	struct paramtbl_t *param_adr[1];
	struct val_t *val_adr[1];
	struct sts_t *sts_adr[33];

	char sz_work[256];
	char sz_work2[256] = { 0 };

	int ret = -1;
	int l_rc;
	long l_hex;
	long long ll_hex;

	map<CString, __int64> &ctlAdrList = m_dbaccess.GetCtlAdrList();
	ctrl_adr = (struct ctlname_t *)((__int64*)ctlAdrList[strCtrl]);

	if (ctrl_adr == NULL){
		//=====================================================//
		//������������������������ Log ������������������������//
		//		CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("param_chk"), _T("ControlName None"), _T(""), nLogEx::error);
		CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("param_chk"), _T("ControlName None"), strCtrl, nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
		pval->ret = false;
		return ret;
	}

	/*----- �ݔ����䍀�ڃX�e�[�^�X�`�F�b�N ----*/
	l_rc = m_dbaccess.ud_adr_to_resvec((char*)ctrl_adr, PARAMTBL_ID, ENDOFCTL_ID, ENDOFPARAM_ID, (char**)param_adr, 1);

	if (l_rc < 1)
	{
		//=====================================================//
		//������������������������ Log ������������������������//
		CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("param_chk"), _T("Control Error"), strCtrl, nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
		pval->ret = false;
		return ret;
	}

	if (param_adr[0]->l_intype == IN_S_ID && param_adr[0]->l_cvttype == CVT_S_ID)
	{
		l_rc = m_dbaccess.ud_adr_to_resvec((char*)ctrl_adr, STS_ID, ENDOFCTL_ID, ENDOFPARAM_ID, (char**)sts_adr, 33);

		// �����ʂƃp�����^���قȂ��Ă���ꍇ�͂Ȃɂ����Ȃ�
		if (pval->type != 0 || l_rc <= 0){
			//=====================================================//
			//������������������������ Log ������������������������//
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("param_chk"), _T("ControlType Mismatch"), strCtrl, nLogEx::error);
			//������������������������ Log ������������������������//
			//=====================================================//
			pval->ret = false;
			return ret;
		}

		CEquipmentCtrlStatus dlg;
		if (bSeqCtrl)
		{
			// ��������̏ꍇ�A�E�B���h�E�^�C�g���ύX
			dlg.SetWindowTitle(_T("����o�^"));
		}
		dlg.mControlName = strCtrl;
		dlg.mCurrentName = pval->buf;
		if (!strAddStatus.IsEmpty())
		{
			dlg.mStatusList.push_back(strAddStatus);
		}
		for (int i = 0; i < l_rc; i++){
			dlg.mStatusList.push_back(CString(sts_adr[i]->sz_sts));
		}
		if (bSeqCtrl && dlg.mCurrentName.IsEmpty())
		{
			dlg.mCurrentName = sts_adr[0]->sz_sts;
		}

		if (dlg.DoModal() == IDOK)
		{
			pval->ret = true;
			sprintf_s(pval->buf, 80, "%s", (LPCSTR)CStringA(dlg.mCurrentName));
			ret = eControlStatus;
		}
	}
	/*----- �\��l�t�@�C�����ڃ`�F�b�N -----*/
	else if (param_adr[0]->l_intype == IN_PRED_ID)
	{
	}
	/*----- �ݔ����䍀�ڃ`�F�b�N ----*/
	else if (param_adr[0]->l_intype == IN_S_ID && param_adr[0]->l_cvttype == CVT_STR_ID)
	{
		// �����`��������(MM:SS)
		CEquipmentCtrlTime dlg;
		if (bSeqCtrl)
		{
			// ��������̏ꍇ�A�E�B���h�E�^�C�g���ύX
			dlg.SetWindowTitle(_T("����o�^"));
		}
		dlg.m_strControlName = strCtrl;
		UINT nType;
		if (param_adr[0]->l_incolint == 5)
		{
			nType = CEquipmentCtrlTime::eTimeType_MSS;
			dlg.SetTimeType(nType);
			CString strTime(pval->buf);
			if (strTime.IsEmpty())
				strTime.Format(_T(" 0:00"));
			dlg.SetTimeString(strTime);
		}
		else
		{
			nType = CEquipmentCtrlTime::eTimeType_MMSS;
			dlg.SetTimeType(nType);
			CString strTime(pval->buf);
			if (strTime.IsEmpty())
				strTime.Format(_T(" 00:00"));
			dlg.SetTimeString(strTime);
		}

		if (dlg.DoModal() == IDOK)
		{
			pval->ret = true;
			sprintf_s(pval->buf, 80, "%s", (LPCSTR)CStringA(dlg.GetTimeString()));
			ret = eControlTime;
		}
	}
	/* �����`��������(HH:MM:SS) */
	else if (param_adr[0]->l_intype == IN_TIME_ID)
	{
		CEquipmentCtrlTime dlg;
		if (bSeqCtrl)
		{
			// ��������̏ꍇ�A�E�B���h�E�^�C�g���ύX
			dlg.SetWindowTitle(_T("����o�^"));
		}
		dlg.m_strControlName = strCtrl;
		dlg.SetTimeType(CEquipmentCtrlTime::eTimeType_HHMMSS);
		CString strTime(pval->buf);
		if (bSeqCtrl && strTime.IsEmpty())
			strTime.Format(_T(" 00:00:00"));
		dlg.SetTimeString(strTime);
		if (dlg.DoModal() == IDOK)
		{
			pval->ret = true;
			sprintf_s(pval->buf, 80, "%s", (LPCSTR)CStringA(dlg.GetTimeString()));
			ret = eControlTime;
		}
	}
	/* �ʎZ���`��������(DDD-HH:MM:SS) */
	else if (param_adr[0]->l_intype == IN_DATE_ID)
	{
		CEquipmentCtrlTime dlg;
		if (bSeqCtrl)
		{
			// ��������̏ꍇ�A�E�B���h�E�^�C�g���ύX
			dlg.SetWindowTitle(_T("����o�^"));
		}
		dlg.m_strControlName = strCtrl;
		dlg.SetTimeType(CEquipmentCtrlTime::eTimeType_DDDHHMMSS);
		CString strTime(pval->buf);
		if (bSeqCtrl && strTime.IsEmpty())
		{
			CTime time;
			theApp.GetCurrentUtcTime(time);
			strTime = time.Format("%j %H:%M:%S");
		}
		dlg.SetTimeString(strTime);
		if (dlg.DoModal() == IDOK)
		{
			pval->ret = true;
			sprintf_s(pval->buf, 80, "%s", (LPCSTR)CStringA(dlg.GetTimeString()));
			ret = eControlTime;
		}
	}
	/* ���t�`��������(YYYY-MM-DD) */
	else if (param_adr[0]->l_intype == IN_YEAR_ID)
	{
		CEquipmentCtrlTime dlg;
		if (bSeqCtrl)
		{
			// ��������̏ꍇ�A�E�B���h�E�^�C�g���ύX
			dlg.SetWindowTitle(_T("����o�^"));
		}
		dlg.m_strControlName = strCtrl;
		dlg.SetTimeType(CEquipmentCtrlTime::eTimeType_YYYYmmdd);
		CString strTime(pval->buf);
		if (bSeqCtrl && strTime.IsEmpty())
		{
			CTime time;
			theApp.GetCurrentUtcTime(time);
			strTime = time.Format("%Y-%m-%d");
		}
		dlg.SetTimeString(strTime);
		if (dlg.DoModal() == IDOK)
		{
			pval->ret = true;
			sprintf_s(pval->buf, 80, "%s", (LPCSTR)CStringA(dlg.GetTimeString()));
			ret = eControlTime;
		}
	}
	/* �p�����[�^�Ȃ� */
	else if (param_adr[0]->l_intype == IN_NONE_ID)
	{
	}
	/* ���̑� */
	else
	{
		l_rc = m_dbaccess.ud_adr_to_resvec((char*)ctrl_adr, VAL_ID, ENDOFCTL_ID, ENDOFPARAM_ID, (char**)val_adr, 1);
		pval->ret = false;

		if (l_rc < 1)
		{
			val_adr[0] = NULL;
		}

		/* �͈͕\�� */
		if (val_adr[0] == NULL) 
		{
			strcpy_s(sz_work, sizeof(sz_work), "�͈͒l����");
			val_adr[0] = NULL;
		}
		else if (param_adr[0]->l_intype == IN_X_ID) 
		{
			l_hex = (unsigned long)val_adr[0]->d_min;
			ll_hex = (unsigned long long)val_adr[0]->d_min;
			sprintf_s(sz_work, "%llx", ll_hex);
			l_hex = (unsigned long)val_adr[0]->d_max;
			ll_hex = (unsigned long long)val_adr[0]->d_max;
			sprintf_s(sz_work2, "%llx", ll_hex);
			strcat_s(sz_work, sizeof(sz_work), " �` ");
			strcat_s(sz_work, sizeof(sz_work), sz_work2);
		}
		else 
		{
			sprintf_s(sz_work2, "%f", val_adr[0]->d_min);
			l_rc = opctcheck(param_adr[0], NULL, sz_work2, 0);
			strcpy_s(sz_work, sizeof(sz_work), sz_work2);
			sprintf_s(sz_work2, "%f", val_adr[0]->d_max);
			l_rc = opctcheck(param_adr[0], NULL, sz_work2, 0);
			strcat_s(sz_work, sizeof(sz_work), " �` ");
			strcat_s(sz_work, sizeof(sz_work), sz_work2);

			sprintf_s(sz_work2, "%f", pval->fval);
			l_rc = opctcheck(param_adr[0], NULL, sz_work2, 0);
		}

		if (val_adr[0] != NULL)
		{
			if (strCtrl2.IsEmpty())
			{
				CEquipmentCtrlNum dlg;
				if (bSeqCtrl)
				{
					// ��������̏ꍇ�A�E�B���h�E�^�C�g���ύX
					dlg.SetWindowTitle(_T("����o�^"));
				}
				CString val = CString(sz_work2).TrimLeft(_T(" "));
				dlg.SetEquipment(strCtrl, param_adr[0]->l_intype);
				dlg.SetRangeText(CString(sz_work));
				dlg.SetRang(val, val_adr[0]->d_min, val_adr[0]->d_max);
				dlg.m_CtrlValueEdit.SetRoundPlaceValue(param_adr[0]->l_incolflt);
				if (dlg.DoModal() == IDOK)
				{
					pval->ret = true;

					if (param_adr[0]->l_intype == IN_U_ID)
					{
						pval->val = _wtoi(dlg.m_RangeCur);
						ret = eControlNumeric;
						pval->type = eControlNumeric;
					}
					else
					{
						pval->fval = _wtof(dlg.m_RangeCur);
						ret = eControlDouble;
						pval->type = eControlDouble;

						int signflag = 0;
						if ((strCtrl.CompareNoCase(_T("ANT.AZ_OFFSET")) == 0)
							|| ((strCtrl.CompareNoCase(_T("ANT.EL_OFFSET")) == 0))
							|| ((strCtrl.CompareNoCase(_T("ANT.RA_OFFSET")) == 0))
							|| ((strCtrl.CompareNoCase(_T("ANT.DEC_OFFSET")) == 0))
							)
						{
							signflag = 1;
						}
						sprintf_s(sz_work2, "%f", pval->fval);
						l_rc = opctcheck(param_adr[0], val_adr[0], sz_work2, signflag);
						strcpy_s(pval->buf, sizeof(pval->buf), sz_work2);
					}
				}
			}
			else
			{
				CEquipmentCtrlSameNum dlg;
				if (bSeqCtrl)
				{
					// ��������̏ꍇ�A�E�B���h�E�^�C�g���ύX
					dlg.SetWindowTitle(_T("����o�^"));
				}
				CString val = CString(sz_work2).TrimLeft(_T(" "));
				dlg.SetEquipment(strCtrl, param_adr[0]->l_intype);
				dlg.SetEquipment2(strCtrl2);
				dlg.SetRangeText(CString(sz_work));
				dlg.SetRang(val, val_adr[0]->d_min, val_adr[0]->d_max);
				dlg.m_CtrlValueEdit.SetRoundPlaceValue(param_adr[0]->l_incolflt);
				if (dlg.DoModal() == IDOK)
				{
					pval->ret = true;

					if (param_adr[0]->l_intype == IN_U_ID)
					{
						pval->val = _wtoi(dlg.m_RangeCur);
						ret = eControlNumeric;
						pval->type = eControlNumeric;
					}
					else
					{
						pval->fval = _wtof(dlg.m_RangeCur);
						ret = eControlDouble;
						pval->type = eControlDouble;

						int signflag = 0;
						if ((strCtrl.CompareNoCase(_T("ANT.AZ_OFFSET")) == 0)
							|| ((strCtrl.CompareNoCase(_T("ANT.EL_OFFSET")) == 0))
							|| ((strCtrl.CompareNoCase(_T("ANT.RA_OFFSET")) == 0))
							|| ((strCtrl.CompareNoCase(_T("ANT.DEC_OFFSET")) == 0))
							)
						{
							signflag = 1;
						}
						sprintf_s(sz_work2, "%f", pval->fval);
						l_rc = opctcheck(param_adr[0], val_adr[0], sz_work2, signflag);
						strcpy_s(pval->buf, sizeof(pval->buf), sz_work2);
					}
				}

			}
		}
	}

	return ret;
}	


/*============================================================================*/
/*! CSatelliteData

-# �ݔ��ڍא���@�\

@param  strCtrl1	 :�ݔ�����1
@param  strParam1	 :�p�����[�^1
@param  strCtrl2	 :�ݔ�����2
@param  strParam2	 :�p�����[�^2
@retval ������

*/
/*============================================================================*/
int CSatelliteData::param_chk2(CString strCtrl1, stControlValue* pval1, CString strCtrl2, stControlValue* pval2)
{
	struct paramtbl_t *param_adr1[1], *param_adr2[1];
	struct val_t *val_adr1[1], *val_adr2[1];

	int ret = -1;
	int l_rc;

	stEquipment2Ctrl	stEquip[2];
	CString strCtrl;

	pval1->ret = false;
	pval2->ret = false;

	bool success = false;

	for (UINT i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			strCtrl = strCtrl1;
			success = GetEquipmentParam(strCtrl, param_adr1, val_adr1, &stEquip[i], pval1->fval);
		}
		else
		{
			strCtrl = strCtrl2;
			success = GetEquipmentParam(strCtrl, param_adr2, val_adr2, &stEquip[i], pval2->fval);
		}

		if (success == false)
		{
			//=====================================================//
			//������������������������ Log ������������������������//
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("param_chk2"), _T("ControlName None"), strCtrl, nLogEx::error);
			//������������������������ Log ������������������������//
			//=====================================================//
			return ret;
		}
	}

	if (stEquip[0].strEquipment.IsEmpty() == false && stEquip[1].strEquipment.IsEmpty() == false)
	{
		CEquipmentCtrl2Num dlg;
		// ��������̏ꍇ�A�E�B���h�E�^�C�g���ύX
		dlg.SetWindowTitle(_T("�ݔ��ڍא��� RA/DEC OFFSET"));		
		dlg.SetEquipment(0, stEquip[0]);
		dlg.SetEquipment(1, stEquip[1]);

		if (dlg.DoModal() == IDOK)
		{
			pval1->ret = true;
			pval2->ret = true;

			CString strEquipmentVal = dlg.GetEquipmentVal(0);
			if (stEquip[0].nType == IN_U_ID)
			{
				pval1->val = _wtoi(strEquipmentVal);
				ret = eControlNumeric;
				pval1->type = eControlNumeric;
			}
			else
			{
				pval1->fval = _wtof(strEquipmentVal);
				ret = eControlDouble;
				pval1->type = eControlDouble;

				int signflag = 0;
				if (strCtrl.CompareNoCase(RA_OFFSET) == 0 ||
					strCtrl.CompareNoCase(DEC_OFFSET) == 0)
				{
					signflag = 1;
				}
				char sz_work2[256] = { 0 };
				sprintf_s(sz_work2, "%f", pval1->fval);
				l_rc = opctcheck(param_adr1[0], val_adr1[0], sz_work2, signflag);
				strcpy_s(pval1->buf, sizeof(pval1->buf), sz_work2);
			}

			strEquipmentVal = dlg.GetEquipmentVal(1);
			if (stEquip[1].nType == IN_U_ID)
			{
				pval2->val = _wtoi(strEquipmentVal);
				ret = eControlNumeric;
				pval2->type = eControlNumeric;
			}
			else
			{
				pval2->fval = _wtof(strEquipmentVal);
				ret = eControlDouble;
				pval2->type = eControlDouble;

				int signflag = 0;
				if (strCtrl.CompareNoCase(RA_OFFSET) == 0 ||
					strCtrl.CompareNoCase(DEC_OFFSET) == 0)
				{
					signflag = 1;
				}
				char sz_work2[256] = { 0 };
				sprintf_s(sz_work2, "%f", pval2->fval);
				l_rc = opctcheck(param_adr2[0], val_adr2[0], sz_work2, signflag);
				strcpy_s(pval2->buf, sizeof(pval2->buf), sz_work2);
			}
		}
	}

	return ret;
}

bool CSatelliteData::GetEquipmentParam(CString &strCtrl, struct paramtbl_t *param_adr[], struct val_t *val_adr[], stEquipment2Ctrl *stEquip, double fval)
{
	struct ctlname_t *ctrl_adr;

	char sz_work[256];
	char sz_work2[256] = { 0 };

	bool ret = false;
	int l_rc;
	long l_hex;
	long long ll_hex;

	stEquip->strEquipment = _T("");

	map<CString, __int64> &ctlAdrList = m_dbaccess.GetCtlAdrList();

	ctrl_adr = (struct ctlname_t *)((__int64*)ctlAdrList[strCtrl]);

	if (ctrl_adr == NULL){
		//=====================================================//
		//������������������������ Log ������������������������//
		CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("GetEquipmentParam"), _T("ControlName None"), strCtrl, nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
		return ret;
	}

	/*----- �ݔ����䍀�ڃX�e�[�^�X�`�F�b�N ----*/
	l_rc = m_dbaccess.ud_adr_to_resvec((char*)ctrl_adr, PARAMTBL_ID, ENDOFCTL_ID, ENDOFPARAM_ID, (char**)param_adr, 1);

	if (l_rc < 1)
	{
		//=====================================================//
		//������������������������ Log ������������������������//
		CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("GetEquipmentParam"), _T("Control Error"), strCtrl, nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
		return ret;
	}

	if (param_adr[0]->l_intype == IN_S_ID && param_adr[0]->l_cvttype == CVT_S_ID)
	{
	}
	/*----- �\��l�t�@�C�����ڃ`�F�b�N -----*/
	else if (param_adr[0]->l_intype == IN_PRED_ID)
	{
	}
	/*----- �ݔ����䍀�ڃ`�F�b�N ----*/
	else if (param_adr[0]->l_intype == IN_S_ID && param_adr[0]->l_cvttype == CVT_STR_ID)
	{
	}
	/* �����`��������(HH:MM:SS) */
	else if (param_adr[0]->l_intype == IN_TIME_ID)
	{
	}
	/* �ʎZ���`��������(DDD-HH:MM:SS) */
	else if (param_adr[0]->l_intype == IN_DATE_ID)
	{
	}
	/* ���t�`��������(YYYY-MM-DD) */
	else if (param_adr[0]->l_intype == IN_YEAR_ID)
	{
	}
	/* �p�����[�^�Ȃ� */
	else if (param_adr[0]->l_intype == IN_NONE_ID)
	{
	}
	/* ���̑� */
	else
	{
		l_rc = m_dbaccess.ud_adr_to_resvec((char*)ctrl_adr, VAL_ID, ENDOFCTL_ID, ENDOFPARAM_ID, (char**)val_adr, 1);

		if (l_rc < 1)
		{
			val_adr[0] = NULL;
		}

		/* �͈͕\�� */
		if (val_adr[0] == NULL)
		{
			strcpy_s(sz_work, sizeof(sz_work), "�͈͒l����");
			val_adr[0] = NULL;
		}
		else if (param_adr[0]->l_intype == IN_X_ID)
		{
			l_hex = (unsigned long)val_adr[0]->d_min;
			ll_hex = (unsigned long long)val_adr[0]->d_min;
			sprintf_s(sz_work, "%llx", ll_hex);
			l_hex = (unsigned long)val_adr[0]->d_max;
			ll_hex = (unsigned long long)val_adr[0]->d_max;
			sprintf_s(sz_work2, "%llx", ll_hex);
			strcat_s(sz_work, sizeof(sz_work), " �` ");
			strcat_s(sz_work, sizeof(sz_work), sz_work2);
		}
		else
		{

			int i_signflag = 0;

			sprintf_s(sz_work2, "%f", val_adr[0]->d_min);
			l_rc = opctcheck(param_adr[0], NULL, sz_work2, i_signflag);
			strcpy_s(sz_work, sizeof(sz_work), sz_work2);

			sprintf_s(sz_work2, "%f", val_adr[0]->d_max);

			l_rc = opctcheck(param_adr[0], NULL, sz_work2, i_signflag);
			strcat_s(sz_work, sizeof(sz_work), " �` ");
			strcat_s(sz_work, sizeof(sz_work), sz_work2);

			sprintf_s(sz_work2, "%f", fval);
			l_rc = opctcheck(param_adr[0], NULL, sz_work2, i_signflag);
		}

		stEquip->strEquipment = strCtrl;
		stEquip->nType = param_adr[0]->l_intype;
		stEquip->strRang = sz_work;
		CString val = CString(sz_work2).TrimLeft(_T(" "));
		stEquip->dRangeCur = val;
		if (val_adr[0] != NULL)
		{
			stEquip->dRangeMin = val_adr[0]->d_min;
			stEquip->dRangeMax = val_adr[0]->d_max;
		}
		stEquip->l_incolflt = param_adr[0]->l_incolflt;
		ret = true;
	}

	return ret;
}

int CSatelliteData::get_paramlist(CString strCtrl, vector<CString> &list)
{
	struct ctlname_t *ctrl_adr;
	struct paramtbl_t *param_adr[1];
	struct sts_t *sts_adr[33];

	char sz_work2[256] = { 0 };

	int ret = -1;
	int l_rc;

	map<CString, __int64> &ctlAdrList = m_dbaccess.GetCtlAdrList();
	ctrl_adr = (struct ctlname_t *)((__int64*)ctlAdrList[strCtrl]);

	if (ctrl_adr == NULL)
	{
		CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("param_chk"), _T("ControlName None"), strCtrl, nLogEx::error);
		return ret;
	}

	/*----- �ݔ����䍀�ڃX�e�[�^�X�`�F�b�N ----*/
	l_rc = m_dbaccess.ud_adr_to_resvec((char*)ctrl_adr, PARAMTBL_ID, ENDOFCTL_ID, ENDOFPARAM_ID, (char**)param_adr, 1);

	if (l_rc < 1)
	{
		CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("param_chk"), _T("Control Error"), strCtrl, nLogEx::error);
		return ret;
	}

	if (param_adr[0]->l_intype == IN_S_ID && param_adr[0]->l_cvttype == CVT_S_ID)
	{
		l_rc = m_dbaccess.ud_adr_to_resvec((char*)ctrl_adr, STS_ID, ENDOFCTL_ID, ENDOFPARAM_ID, (char**)sts_adr, 33);

		// �����ʂƃp�����^���قȂ��Ă���ꍇ�͂Ȃɂ����Ȃ�
		if (l_rc <= 0)
		{
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("param_chk"), _T("ControlType Mismatch"), strCtrl, nLogEx::error);
			return ret;
		}

		for (int i = 0; i < l_rc; i++)
		{
			list.push_back(CString(sts_adr[i]->sz_sts));
		}
		ret = 1;
	}

	return ret;
}


/*============================================================================*/
/*! CSatelliteData

-# �ݔ��ڍא���@�\

@param  val_adr;		�㉺���l�`�F�b�N�e�[�u��
@param  sz_indata[];	���͕�����
@param  i_signflag;		�����t���t���O
@retval �Ȃ�
*/
/*============================================================================*/
int CSatelliteData::opctcheck(struct paramtbl_t *param_adr, struct val_t *val_adr, char sz_indata[], int i_signflag)
{

	long      l_dt;
	unsigned  long l_hex;
	unsigned long long ll_hex;
	double    d_data;
//	double    d_work;
	long      l_rc;
//	long      i;
	char      sz_format[20];
	char      sz_work[256];
	char      *ca_exp;
	long      l_tsize;
	long      l_year;
	long      l_mon;
	long      l_day;
	long      l_tday;
	long      l_hou;
	long      l_min;
	long      l_sec;

	/*----- ���v���Z�X�̂h�o�b�L�[���擾 -----*/

	if (strlen(sz_indata) == 0) {
		return -1;
	}
	if (param_adr->l_intype == IN_X_ID) {
		l_rc = sscanf_s(sz_indata, "%llx%s", &ll_hex, sz_work, (int)sizeof(sz_work));
		if (l_rc != 1) {
			l_rc = -1;
		}
	}
	else if (param_adr->l_intype == IN_S_ID) {
		l_rc = 0;
		l_dt = (long)strlen(sz_indata);
		d_data = l_dt;
	}
	else {
		l_rc = uc_valchk(sz_indata, (int *)&l_hex, &d_data, 0);
		l_rc = sscanf_s(sz_indata, "%lld%s", &ll_hex, sz_work, (int)sizeof(sz_work));
		l_rc = uc_valchk(sz_indata, (int *)&l_dt, &d_data, 0);
	}
	if (l_rc < 0) {
		l_rc = sscanf_s(sz_indata, "%lg%s", &d_data, sz_work, (int)sizeof(sz_work));
		if (l_rc == 1) {
			l_rc = 2;
		}
		else {
			l_rc = -1;
		}
	}
	switch (param_adr->l_intype) {
		/*-------------------------------------------------*/
		/*-- �����f�[�^����                              --*/
		/*-------------------------------------------------*/
	case IN_D_ID:
	case IN_I_ID:
	case IN_U_ID:
		if (l_rc != 1) {
//			return -1;
		}
		if (param_adr->l_intype == IN_D_ID) {
			d_data = l_dt;
		}
		else {
			d_data = (double)ll_hex;
		}
		if (val_adr != NULL) {
			if (d_data < val_adr->d_min ||
				d_data > val_adr->d_max) {
				return -2;
			}
		}
		if (param_adr->l_intype == IN_D_ID) {
			sprintf_s(sz_indata, 256, "%d", l_dt);
		}
		else {
			sprintf_s(sz_indata, 256, "%llu", ll_hex);
		}
		if (strlen(sz_indata) > param_adr->l_incolint) {
			return -3;
		}
		break;
		/*-------------------------------------------------*/
		/*-- �P�U�i������                                --*/
		/*-------------------------------------------------*/
	case IN_X_ID:
		if (val_adr != NULL) {
			d_data = (double)ll_hex;
			if (d_data < val_adr->d_min ||
				d_data > val_adr->d_max) {
				return -2;
			}
		}
		sprintf_s(sz_indata, 256, "0x%llx", ll_hex);
		if (strlen(sz_indata) > param_adr->l_incolint) {
			return -3;
		}
		break;
		/*-------------------------------------------------*/
		/*-- ���������                                  --*/
		/*-------------------------------------------------*/
	case IN_S_ID:
		if (strlen(sz_indata) > param_adr->l_incolint) {
			return -3;
		}
		break;
		/*-------------------------------------------------*/
		/*-- �t���[�e�B���O����                          --*/
		/*-------------------------------------------------*/
	case IN_F_ID:
		if (l_rc < 1) {
			return -1;
		}
		if (l_rc == 1) {
			d_data = l_dt;
		}
		if (val_adr != NULL) {
			if (d_data < val_adr->d_min ||
				d_data > val_adr->d_max) {
				return -2;
			}
		}
		if (param_adr->l_incolexp == 0) {
			if (param_adr->l_incolflt == 0) {
				l_tsize = param_adr->l_incolint;
				sprintf_s(sz_format, sizeof(sz_format), "%%%df", l_tsize);
			}
			else {
				if (i_signflag == 1) {
					l_tsize = param_adr->l_incolint + param_adr->l_incolflt + 1;
					sprintf_s(sz_format, sizeof(sz_format), "%%+%d.%df", l_tsize, param_adr->l_incolflt);
				}
				else if (i_signflag == 2) { /* #2010 2011-02-25 �A���e�i�p�x�ݒ萧��p�@���ꏈ��(�[���T�v���X) */
					l_tsize = param_adr->l_incolint + param_adr->l_incolflt + 1;
					sprintf_s(sz_format, sizeof(sz_format), "%%0%d.%df", l_tsize, param_adr->l_incolflt);
				}
				else {
					l_tsize = param_adr->l_incolint + param_adr->l_incolflt + 1;
					sprintf_s(sz_format, sizeof(sz_format), "%%%d.%df", l_tsize, param_adr->l_incolflt);
				}
			}
			sprintf_s(sz_indata, 256, sz_format, d_data);
			printf("[%s] -> [%s]\n", sz_format, sz_indata);
			if (strlen(sz_indata) > l_tsize) {
				return -3;
			}
		}
		else {
			l_tsize = param_adr->l_incolint + param_adr->l_incolflt + 7;
			sprintf_s(sz_format, sizeof(sz_format), "%%%d.%de", l_tsize, param_adr->l_incolflt);
			sprintf_s(sz_indata, 256, sz_format, d_data);
			if (strlen(sz_indata) > l_tsize) {
				return -3;
			}
			ca_exp = strchr(sz_indata, 'e');
			ca_exp++;
			sscanf_s(ca_exp, "%d", &l_dt);
			sprintf_s(ca_exp, sizeof(ca_exp), "%d", l_dt);
			if (param_adr->l_incolexp < strlen(ca_exp)) {
				return -3;
			}
		}
		break;
		/*-------------------------------------------------*/
		/*-- �������͏��� (HH:MM:SS)                     --*/
		/*-------------------------------------------------*/
	case IN_TIME_ID:
		l_hou = -1;
		l_min = -1;
		l_sec = -1;
		l_rc = sscanf_s(sz_indata, "%d:%d:%d%s", &l_hou, &l_min, &l_sec, sz_work, (int)sizeof(sz_work));
		if (l_rc == 1) {
			l_sec = l_hou % 100;
			l_hou /= 100;
			l_min = l_hou % 100;
			l_hou /= 100;
		}
		if (l_rc != 1 && l_rc != 3) {
			return -1;
		}
		if (l_hou < 0 || l_hou > 23) {
			return -2;
		}
		if (l_min < 0 || l_min > 59) {
			return -2;
		}
		if (l_sec < 0 || l_sec > 59) {
			return -2;
		}
		sprintf_s(sz_indata, 256, "%02d:%02d:%02d", l_hou, l_min, l_sec);
		break;
		/*-------------------------------------------------*/
		/*-- �������͏���  (DDD HH:MM:SS)                --*/
		/*-------------------------------------------------*/
	case IN_DATE_ID:
		l_tday = -1;
		l_hou = -1;
		l_min = -1;
		l_sec = -1;
		l_rc = sscanf_s(sz_indata, "%d %d:%d:%d%s", &l_tday, &l_hou, &l_min, &l_sec, sz_work, (int)sizeof(sz_work));
		if (l_rc == 1) {
			l_sec = l_tday % 100;
			l_tday /= 100;
			l_min = l_tday % 100;
			l_tday /= 100;
			l_hou = l_tday % 100;
			l_tday /= 100;
		}
		if (l_rc != 1 && l_rc != 4) {
			return -1;
		}
		if (l_tday < 1 || l_hou > 366) {
			return -2;
		}
		if (l_hou < 0 || l_hou > 23) {
			return -2;
		}
		if (l_min < 0 || l_min > 59) {
			return -2;
		}
		if (l_sec < 0 || l_sec > 59) {
			return -2;
		}
		sprintf_s(sz_indata, 256, "%03d %02d:%02d:%02d", l_tday, l_hou, l_min, l_sec);
		break;
		/*-------------------------------------------------*/
		/*-- �������͏���  (YYYY-MM-DD)                  --*/
		/*-------------------------------------------------*/
	case IN_YEAR_ID:
		l_tday = -1;
		l_hou = -1;
		l_min = -1;
		l_sec = -1;
		l_rc = sscanf_s(sz_indata, "%d-%d-%d%s", &l_year, &l_mon, &l_day, sz_work, (int)sizeof(sz_work));
		if (l_rc == 1) {
			l_day = l_year % 100;
			l_year /= 100;
			l_mon = l_year % 100;
			l_year /= 100;
		}
		if (l_rc != 1 && l_rc != 3) {
			return -1;
		}
		if (l_year < 70) {
			l_year += 2000;
		}
		if (l_year < 1900) {
			l_year += 1900;
		}
		if (l_mon < 1 || l_mon > 12) {
			return -2;
		}
		if (l_year % 4 == 0 && l_mon == 2) {
			if (l_day > 29) {
				return -2;
			}
		}
		else {
			if (l_day > l_chkday[l_mon]) {
				return -2;
			}
		}
		sprintf_s(sz_indata, 256, "%04d-%02d-%02d", l_year, l_mon, l_day);
		break;
	}
	return 0;
}


/*<a name="uc_valchk"></a>
@function : uc_valchk()
@name     : ���l������`�F�b�N���o�C�i���l�ϊ�����
@���藚�� : 92-05-29 Ver1.0 1st coding  mika
@���藚�� : 92-07-03 Ver1.1   mika
@���藚�� : 92-08-17 Ver1.2   mika
@���藚�� : 97-01-16 Ver2.0   T.S.
@���藚�� : 97-12-17 Ver2.1   T.S. (K/M�Ή�)
@���藚�� : 98-04-09 Ver2.2   T.S. (8086�Ή�)
@���藚�� : 2000-08-23 Ver2.4 �W�i��(0t777,0T777)�Ή�
@�����T�v : �ϊ����镶�����^�����l������`�F�b�N�ƃo�C�i���l�ϊ����s��
@            1) s        ---<  input : sz_ :  n-byte : >---
@                             �ϊ����镶����
@            2)i_p       ---<  output : long  :  1  :  >---
@                             �ϊ����ꂽ4�o�C�g�̃o�C�i���f�[�^
@                             (i_p=NULL�̏ꍇ�͌��ʂ͖߂��Ȃ�)
@            3)d_p       ---<  output  : double : 1  : >---
@                             �ϊ����ꂽ8�o�C�g��double�f�[�^
@                             (d_p=NULL�̏ꍇ�͌��ʂ͖߂��Ȃ�)

@                             ('0x'+16�i��, '0b'+2�i��, '0t'+8�i����
@                              �ꍇ�͕����Ȃ��f�[�^�Ƃ݂Ȃ��̂Œ���)

@                             ���̃|�C���^�l�� -1 �̏ꍇ�͑�S�p��
@                             ���[�^(l_opt)���L���ɂȂ�

@            4)l_opt     ---<  input  :  long  :  1  : >---
@                             1000 �܂��́A1024 ���w�肷��ƁA���l
@                             �̌��� K M ���t������Ă����ꍇ��
@                             �ϊ��l�Ƃ��āAK:1000�{�܂���1024�{
@                             M:1000*1000�{�܂���1024*1024�{�̒l
@                             ��Ԃ�
@
@�߂�l   :    (�`�F�b�N/�ϊ����ꂽ�f�[�^�̌^������)
@              -1 : ���l�ł͂Ȃ�
@                          (�ϊ����ʂ͂��ǂ��Ȃ�����)
@               0 : s��NULL�|�C���^�ł�����
@                          (�ϊ����ʂ͂��ǂ��Ȃ�����)
@               1 : long�^�̐��l�Ƃ��ĕϊ�����
@               (10�i��, '0x'+16�i��, '0b'+2�i���̏ꍇ)
@               (8�i��, '0t'+8�i���̏ꍇ)
@
@               2 : double�^�̐��l�Ƃ��ĕϊ�����
@                   (999.99, 99.9e99�`���̏ꍇ)
@                   (long�ł̓I�[�o�[�t���[����ꍇ)
@
@�⑫     :
*/
//char    *s;                             /* �ϊ����镶���� */
//INT32bit *i_p;                           /* �ϊ����ꂽ4�o�C�g�̃o�C�i���f�[�^ */
//double  *d_p;                           /* �ϊ����ꂽ8�o�C�g��double�f�[�^   */
//INT32bit l_opt;                          /* d_p==-1�̎��̂ݗL��*/

int CSatelliteData::uc_valchk(char *s, int *i_p, double *d_p, int l_opt)
{
	int     i = 2;                            /* ������J�E���^ */
	int     j = 0;                            /* �J�E���^ */
	char    sz_dmy[512];                    /* sscanf�_�~�[�o�b�t�@ */

	long    l_bitbuf = 0;                     /* �r�b�g�V�t�g�o�b�t�@ */
	double  d_bitbuf = 0.0;                   /* 32bit�ȏ�̏ꍇ */
	long    l_data;                         /* �v�Z���ʃf�[�^  */
	double  d_data;

	union   umoji  mojibuf;                 /* ������擪2�����̃o�b�t�@ */



	/*********************************************************************/
	/*                             PROCEDURE                             */
	/*********************************************************************/

	/* s��NULL���H */
	if (s == NULL)  return(0);

	/* ������擪2�����̎擾 */
	//strncpy(mojibuf.c_moji, 2, s);
	//strncpy_s(mojibuf.c_moji, 2, s, 2);
	memcpy(mojibuf.c_moji, s, 2);

	/* ������擪2�����ɂ�菈����I�� */
	switch (mojibuf.s_moji) {

	case LX: /* 0X */
	case SX: /* 0x */
		while (s[i] != iNULL) {  /* 2005-1-7 */
			l_bitbuf = l_bitbuf << 4;    /* ����4�r�b�g�V�t�g */
			d_bitbuf = d_bitbuf * 16;
			if (0x30 <= s[i] && 0x39 >= s[i]) {
				l_data = (long)(s[i] - 0x30);
				l_bitbuf = l_bitbuf | l_data; /* OR */
				d_bitbuf = d_bitbuf + l_data;
			}
			else  if (0x61 <= (s[i] = tolower(s[i])) && 0x66 >= (s[i] = tolower(s[i]))) {
				l_data = (long)(s[i] - 0x57);
				l_bitbuf = l_bitbuf | l_data;  /* OR */
				d_bitbuf = d_bitbuf + l_data;
			}
			else {
				return(-1);
			}
			i++;
		}
		if (i_p != NULL){ *i_p = l_bitbuf; }
		if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_bitbuf; }
		return(1);
		break;

	case LB: /* 0B */
	case SB: /* 0b */
		while (s[i] != iNULL) { /* 2005-1-7 */
			l_bitbuf = l_bitbuf << 1;    /* ����1�r�b�g�V�t�g */
			d_bitbuf = d_bitbuf * 2;
			switch (s[i]) {
			case '0': break;
			case '1': l_bitbuf = l_bitbuf + 1;
				d_bitbuf = d_bitbuf + 1;
				break;
			default:  return(-1);
			}
			i++;
		}
		if (i_p != NULL){ *i_p = l_bitbuf; }
		if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_bitbuf; }
		return(1);
		break;

	case LT: /* 0T */
	case ST: /* 0t */
		while (s[i] != iNULL) {  /* 2005-1-7 */
			l_bitbuf = l_bitbuf << 3;    /* ����3�r�b�g�V�t�g */
			d_bitbuf = d_bitbuf * 8;
			if ((s[i] >= '0') && (s[i] <= '7')){
				l_bitbuf = l_bitbuf + (s[i] - '0'); /* 0..7 */
				d_bitbuf = d_bitbuf + (s[i] - '0'); /* 0..7 */
			}
			else{
				return(-1);
			}
			i++;
		}
		if (i_p != NULL){ *i_p = l_bitbuf; }
		if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_bitbuf; }
		return(1);
		break;

	default:
		if ((int)d_p == -1){
			//j = sscanf(s, "%lf%s", &d_data, sz_dmy);
			j = sscanf_s(s, "%lf%s", &d_data, sz_dmy, (int)sizeof(sz_dmy));
			if (j == 1){
				; /*d_data�����̂܂ܕԂ�*/
			}
			else if (j == 2){
				if (!strcmp(sz_dmy, "K")){
					d_data = d_data * l_opt;
				}
				else if (!strcmp(sz_dmy, "M")){
					d_data = d_data * l_opt * l_opt;
				}
				else{
					return(-1); /*ERROR*/
				}
			}
			else{
				return(-1); /*ERROR*/
			}
			if (i_p != NULL){ *i_p = (int)d_data; }
			return(1); /*�����l*/
		}
		//if (sscanf(s, "%lf%s", &d_data, sz_dmy) != 1){ return(-1); }
		if (sscanf_s(s, "%lf%s", &d_data, sz_dmy, (int)sizeof(sz_dmy)) != 1){ return(-1); }
		if ((d_data >  2147483647.0) || (d_data < -2147483648.0)){
			if (i_p != NULL){ *i_p = 0x80000000; }
			if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_data; }
			return(2); /*integer���I�[�o�[�t���[����̂Ŏ����l*/
		}
		else{
			/* �����l? */
			if ((strchr(s, '.') == NULL) && (strchr(s, 'e') == NULL) && (strchr(s, 'E') == NULL)){
				if (i_p != NULL){ *i_p = (int)d_data; }
				if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_data; }
				return(1); /*�����l*/
			}
			else{
				if (i_p != NULL){ *i_p = (int)d_data; }
				if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_data; }
				return(2);
			}
		}
	}/*end switch*/
}


/*============================================================================*/
/*! CSatelliteData

-# �ݔ��ڍא���@�\

@param  strCtrl		:�ݔ�����
@param  strParam	:�p�����[�^
@retval �Ȃ�
*/
/*============================================================================*/
void CSatelliteData::opctproc2(CString strCtrl, CString strParam)
{
	CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("opctproc2"), strCtrl + "," + strParam, _T(""), nLogEx::info);

	if (strParam != _T("") && strParam != _T("-nd"))
	{
		/* ����ʒm�𑗐M */
		theApp.SendControlNoUI(strCtrl, strParam, _T(""), _T(""));
	}
	else
	{
		/* ����ʒm�𑗐M */
		theApp.SendControlNoUI(strCtrl, _T(""), _T(""), _T(""));
	}
}

/*============================================================================*/
/*! CSatelliteData

-# �ǉ^�p�󋵃e�[�u���A�N�Z�X

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
int CSatelliteData::stn_tbl_access()
{
	m_dbaccess.eqctl_db_read();
	return 0;
}

/*============================================================================*/
/*! CSatelliteData

-# CID�t�@�C���Ǎ�

@param  sz_watch[]		�Ď����ږ��� [IN]
BOOL ReadCIDFile();
@retval -1�F�ُ�I��/ 0�F����I��
*/
/*============================================================================*/
BOOL CSatelliteData::ReadCIDFile()
{
	const CString mEmptyString = CID_INVALID;

	FILE *fp;
	TCHAR sz_buff[500];

	CString strFilePath = theApp.GetShareFilePath(eFileType_CID_Folder, theApp.GetSelectStation());
	CString tempPath = strFilePath + _T("*");

	// �Ώۃt�H���_���̃t�@�C�������擾����
	// �w�肳�ꂽ�t�@�C�����Ɉ�v����t�@�C�����A�f�B���N�g�����Ō�������
	WIN32_FIND_DATA ffdata;
	HANDLE hFindFile = FindFirstFile(tempPath, &ffdata);
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	UINT item = 0;
	do
	{
		TRY
		{
			CString fname;
			if (ffdata.cFileName[0] == '\0')
			{
				AfxThrowUserException();
			}
			if (ffdata.cFileName[0] == '.')
			{
				AfxThrowUserException();
			}
			if (ffdata.cFileName[0] == '[')
			{
				AfxThrowUserException();
			}

			// �f�B���N�g�����擾
			if (ffdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				CString file = strFilePath + _T("\\") + CString(ffdata.cFileName);
				m_CIDSlctData.GetSatelliteNameList().push_back(CString(ffdata.cFileName).MakeUpper());
			}
		}
		CATCH(CUserException, e)
		{
		}
		AND_CATCH(CException, e)
		{
		}
		END_CATCH
	} while (FindNextFile(hFindFile, &ffdata)); // �t�@�C�����Ȃ��Ȃ�܂ő�����


	map<CString, stCIDPrmEle> &cidEleList = theApp.GetSatelliteData().m_CIDSlctData.GetCidElementList();
	cidEleList.clear();

	map<CString, map<CString, stCIDParam>> &cidlist = theApp.GetSatelliteData().m_CIDSlctData.GetCidParamList();
	cidlist.clear();
		
	map<CString, stCIDPrmFlg> &cidPrmFlglist = theApp.GetSatelliteData().m_CIDSlctData.GetCidPrmFlgList();
	cidPrmFlglist.clear();


	bool bRead = false;

	// �q������CID�t�@�C���Ǎ�
	vector<CString>& slist = m_CIDSlctData.GetSatelliteNameList();
	for (auto itr = slist.begin(); itr != slist.end(); ++itr)
	{
		CString sat = (CString)(*itr);	// �q����

		if (sat.CompareNoCase(_T("rdb_cid")) == 0)
		{
			continue;
		}

//		CString file = strFilePath + sat + _T("\\") + CID_FILE;
		CString file = strFilePath + sat + _T("\\") + sat + CID_EXT;

//#define _CHK_CIDFILE	1
#ifndef _CHK_CIDFILE
		if ((_wfopen_s(&fp, file, _T("r")) != 0) || (fp == NULL))
		{
			// ���̃t�H���_�𒲂ׂ�
			continue;
		}
//#if 0	// �w�b�_�[�s���L��ꍇ�́A�L���ɂ��ׂ�
//		// �w�b�_�[���ǂݔ�΂�
//		fgetws(sz_buff, static_cast<unsigned>(_countof(sz_buff)), fp);
//#endif
		stCIDPrmEle	stele;
		stCIDParam	stcid;
		stCIDPrmFlg	stpflg;
		stCIDPrmFlg	stpflgSat;
		memset(&stpflg, 0, sizeof(stpflg));
		memset(&stpflgSat, 0, sizeof(stpflgSat));

		char sz_line[512];

		while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp) != NULL)
		{
			bRead = true;

//#if 0
//			CString line(sz_line);
//#else
			wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
			CString line(sz_buff);
//#endif
			BOOL isLineRead = TRUE;
			int	 iStart = 0;
			int  iFind = 0;

			int iCount = 0;
			int prmIdx = 0;

			while (isLineRead)
			{
				iFind = line.Find(_T(","), iStart);
				CString item = _T("");

				if (iFind == -1)
				{
					iFind = line.Find(_T("\n"), iStart);
					isLineRead = FALSE;
				}

				item = line.Mid(iStart, iFind - iStart);

				switch (iCount)
				{
				case 0:		// CID�ԍ�
					stcid.CID = item;
					AddDataVector(stele.CID, item);
					break;

				case 1:		// �p�����[�^�ԍ�
					prmIdx = _wtoi(item);
					break;

				case 2:		// �p�����[�^��
					break;

				case 3:		// �p�����[�^�L���t���O
					{
						int nflg = _wtoi(item);
						switch (prmIdx)
						{
						case 1:	// S�уe�����g���r�b�g���[�g
							stpflg.TlmBitRateS = nflg;
							break;
						case 2:	// X�уe�����g���r�b�g���[�g
							stpflg.TlmBitRateX = nflg;
							break;
						case 3:	// X�����e�����g���r�b�g���[�g
							stpflg.TlmBitRateHX = nflg;
							break;
						case 4:	// Ka�уe�����g���r�b�g���[�g 
							stpflg.TlmBitRateKa = nflg;
							break;
						case 5:	// S�уR�}���h�r�b�g���[�g
							stpflg.CmdBitRateS = nflg;
							break;
						case 6:	// X�уR�}���h�r�b�g���[�g
							stpflg.CmdBitRateX = nflg;
							break;
						case 7:	// ���M�d��
							stpflg.TrsPower = nflg;
							break;
						case 8:	// �A�b�v�����N�|���p�����[�^
							stpflg.UpLinkSweepShape = nflg;
							break;
						case 9:	// �����W�v���p�����[�^ ���[�h
							stpflg.RngMesMode = nflg;
							break;
						case 10:// S�уL�����A��M���[�v�o���h
							stpflg.CarrLoopBandS = nflg;
							break;
						case 11:// X�уL�����A��M���[�v�o���h
							stpflg.CarrLoopBandX = nflg;
							break;
						case 12:// Ka�уL�����A��M���[�v�o���h
							stpflg.CarrLoopBandKa = nflg;
							break;
						case 13:// �R�}���h�ϒ��x
							stpflg.CmdMod = nflg;
							break;
						case 14:// �R�}���h�T�u�L�����A���g��
							stpflg.CmdSubCarrFreq = nflg;
							break;
						case 15:// ���M�h�b�v���⏞�̗L��
							stpflg.TransDoppler = nflg;
							break;
						case 16:// �A���e�i�����ǔ�
							stpflg.AntAutoTrack = nflg;
							break;
						case 17:// �ϒ�����
							stpflg.Modulation = nflg;
							break;
						case 18:// �A���e�i�҂��󂯎w���̗L��
							stpflg.AntAosPosn = nflg;
							break;
						case 19:// �A���e�i�ҋ@�ʒu�w���̗L��
							stpflg.AntWaitPoint = nflg;

							// �q���ʂ̃p�����[�^�����񊈐�����t�����Ă���
							stpflgSat.TlmBitRateS |= stpflg.TlmBitRateS;				// �e�����g���r�b�g���[�g S��
							stpflgSat.TlmBitRateX |= stpflg.TlmBitRateX;				// �e�����g���r�b�g���[�g X��
							stpflgSat.TlmBitRateHX |= stpflg.TlmBitRateHX;				// �e�����g���r�b�g���[�g X����
							stpflgSat.TlmBitRateKa |= stpflg.TlmBitRateKa;				// �e�����g���r�b�g���[�g Ka��
							stpflgSat.CmdBitRateS |= stpflg.CmdBitRateS;				// �R�}���h�r�b�g���[�g S��
							stpflgSat.CmdBitRateX |= stpflg.CmdBitRateX;				// �R�}���h�r�b�g���[�g X��
							stpflgSat.TrsPower |= stpflg.TrsPower;						// ���M�d��
							stpflgSat.CarrLoopBandS |= stpflg.CarrLoopBandS;			// �L�����A��M���[�v�o���h S��
							stpflgSat.CarrLoopBandX |= stpflg.CarrLoopBandX;			// �L�����A��M���[�v�o���h X��
							stpflgSat.CarrLoopBandKa |= stpflg.CarrLoopBandKa;			// �L�����A��M���[�v�o���h Ka��
							stpflgSat.UpLinkSweepShape |= stpflg.UpLinkSweepShape;		// �A�b�v�����N�|���p�����[�^ �`��
							stpflgSat.UpLinkSweepWidth |= stpflg.UpLinkSweepWidth;		// �A�b�v�����N�|���p�����[�^ ��
							stpflgSat.UpLinkSweepTime |= stpflg.UpLinkSweepTime;		// �A�b�v�����N�|���p�����[�^ ����
							stpflgSat.RngMesMode |= stpflg.RngMesMode;					// �����W�v���p�����[�^ ���[�h
							stpflgSat.Modulation |= stpflg.Modulation;					// �ϒ����� S��
							stpflgSat.ModulationS |= stpflg.ModulationS;				// �ϒ����� S��
							stpflgSat.ModulationX |= stpflg.ModulationX;				// �ϒ����� X��
							stpflgSat.CmdMod |= stpflg.CmdMod;							// �R�}���h�ϒ��x
							stpflgSat.CmdSubCarrFreq |= stpflg.CmdSubCarrFreq;			// �R�}���h�T�u�L�����A���g��
							stpflgSat.TransDoppler |= stpflg.TransDoppler;				// ���M�h�b�v��
							stpflgSat.AntAutoTrack |= stpflg.AntAutoTrack;				// �A���e�i�����ǔ�
							stpflgSat.AntAosPosn |= stpflg.AntAosPosn;					// �A���e�i�҂��󂯎w��
							stpflgSat.AntWaitPoint |= stpflg.AntWaitPoint;				// �A���e�i�ҋ@�ʒu�w��
							break;
						}
					}
					break;

				case 4:		// �p�����[�^�l
					{
						switch (prmIdx)
						{
						case 1:	// S�уe�����g���r�b�g���[�g
							if (stpflg.TlmBitRateS == 0)
							{
								item = mEmptyString;
							}
							stcid.TlmBitRateS = item;
							AddDataVector(stele.TlmBitRateS, item);
							break;
						case 2:	// X�уe�����g���r�b�g���[�g
							if (stpflg.TlmBitRateX == 0)
							{
								item = mEmptyString;
							}
							stcid.TlmBitRateX = item;
							AddDataVector(stele.TlmBitRateX, item);
							break;
						case 3:	// X�����e�����g���r�b�g���[�g
							if (stpflg.TlmBitRateHX == 0)
							{
								item = mEmptyString;
							}
							stcid.TlmBitRateHX = item;
							AddDataVector(stele.TlmBitRateHX, item);
							break;
						case 4:	// Ka�уe�����g���r�b�g���[�g 
							if (stpflg.TlmBitRateKa == 0)
							{
								item = mEmptyString;
							}
							stcid.TlmBitRateKa = item;
							AddDataVector(stele.TlmBitRateKa, item);
							break;
						case 5:	// S�уR�}���h�r�b�g���[�g
							if (stpflg.CmdBitRateS == 0)
							{
								item = mEmptyString;
							}
							stcid.CmdBitRateS = item;
							AddDataVector(stele.CmdBitRateS, item);
							break;
						case 6:	// X�уR�}���h�r�b�g���[�g
							if (stpflg.CmdBitRateX == 0)
							{
								item = mEmptyString;
							}
							stcid.CmdBitRateX = item;
							AddDataVector(stele.CmdBitRateX, item);
							break;
						case 7:	// ���M�d��
							if (stpflg.TrsPower == 0)
							{
								item = mEmptyString;
							}
							stcid.TrsPower = item;
							AddDataVector(stele.TrsPower, item);
							break;
						case 8:	// �A�b�v�����N�|���p�����[�^
							if (stpflg.UpLinkSweepShape == 0)
							{
								item = mEmptyString;
							}
							stcid.UpLinkSweepShape = item;
							AddDataVector(stele.UpLinkSweepShape, item);
							break;
						case 9:	// �����W�v���p�����[�^ ���[�h
							if (stpflg.RngMesMode == 0)
							{
								item = mEmptyString;
							}
							stcid.RngMesMode = item;
							AddDataVector(stele.RngMesMode, item);
							break;
						case 10:// S�уL�����A��M���[�v�o���h
							if (stpflg.CarrLoopBandS == 0)
							{
								item = mEmptyString;
							}
							stcid.CarrLoopBandS = item;
							AddDataVector(stele.CarrLoopBandS, item);
							break;
						case 11:// X�уL�����A��M���[�v�o���h
							if (stpflg.CarrLoopBandX == 0)
							{
								item = mEmptyString;
							}
							stcid.CarrLoopBandX = item;
							AddDataVector(stele.CarrLoopBandX, item);
							break;
						case 12:// Ka�уL�����A��M���[�v�o���h
							if (stpflg.CarrLoopBandKa == 0)
							{
								item = mEmptyString;
							}
							stcid.CarrLoopBandKa = item;
							AddDataVector(stele.CarrLoopBandKa, item);
							break;
						case 13:// �R�}���h�ϒ��x
							if (stpflg.CmdMod == 0)
							{
								item = mEmptyString;
							}
							stcid.CmdMod = item;
							AddDataVector(stele.CmdMod, item);
							break;
						case 14:// �R�}���h�T�u�L�����A���g��
							if (stpflg.CmdSubCarrFreq == 0)
							{
								item = mEmptyString;
							}
							stcid.CmdSubCarrFreq = item;
							AddDataVector(stele.CmdSubCarrFreq, item);
							break;
						case 15:// ���M�h�b�v���⏞�̗L��
							if (stpflg.TransDoppler == 0)
							{
								item = mEmptyString;
							}
							stcid.TransDoppler = item;
							AddDataVector(stele.TransDoppler, item);
							break;
						case 16:// �A���e�i�����ǔ�
							if (stpflg.AntAutoTrack == 0)
							{
								item = mEmptyString;
							}
							stcid.AntAutoTrack = item;
							AddDataVector(stele.AntAutoTrack, item);
							break;
						case 17:// �ϒ�����
							if (stpflg.Modulation == 0)
							{
								item = mEmptyString;
							}
							stcid.Modulation = item;
							AddDataVector(stele.Modulation, item);
							break;
						case 18:// �A���e�i�҂��󂯎w���̗L��
							if (stpflg.AntAosPosn == 0)
							{
								item = mEmptyString;
							}
							stcid.AntAosPosn = item;
							AddDataVector(stele.AntAosPosn, item);
							break;
						case 19:// �A���e�i�ҋ@�ʒu�w���̗L��
							if (stpflg.AntWaitPoint == 0)
							{
								item = mEmptyString;
							}
							stcid.AntWaitPoint = item;
							AddDataVector(stele.AntWaitPoint, item);

							cidlist[sat.MakeUpper()][stcid.CID] = stcid;

							memset(&stpflg, 0, sizeof(stpflg));		// CID���ς�邽�тɃN���A
							break;
						}
					}
					break;
				}

				iStart = iFind + 1;
				iCount++;

			}
		}

		// �q���ʂ̃p�����[�^�����񊈐��o�^
		cidPrmFlglist[sat.MakeUpper()] = stpflgSat;
		cidEleList[sat.MakeUpper()] = stele;
		fclose(fp);

#ifdef _DEBUG
		{
			CString strLog;

			strLog.Format(_T("%s, %s, CID(%s), 1:TlmBitRateS(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.TlmBitRateS);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 2:TlmBitRateX(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.TlmBitRateX);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 3:TlmBitRateHX(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.TlmBitRateHX);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 4:TlmBitRateKa(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.TlmBitRateKa);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 5:CmdBitRateS(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.CmdBitRateS);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 6:CmdBitRateX(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.CmdBitRateX);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 7:TrsPower(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.TrsPower);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 10:CarrLoopBandS(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.CarrLoopBandS);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 11:CarrLoopBandX(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.CarrLoopBandX);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 12:CarrLoopBandKa(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.CarrLoopBandKa);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 8:UpLinkSweepShape(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.UpLinkSweepShape);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), -:UpLinkSweepWidth(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.UpLinkSweepWidth);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), -:UpLinkSweepTime(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.UpLinkSweepTime);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 9:RngMesMode(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.RngMesMode);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 17:Modulation(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.Modulation);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), -:ModulationS(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.ModulationS);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), -:ModulationX(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.ModulationX);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 13:CmdMod(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.CmdMod);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 14:CmdSubCarrFreq(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.CmdSubCarrFreq);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 15:TransDoppler(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.TransDoppler);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 16:AntAutoTrack(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.AntAutoTrack);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 18:AntAosPosn(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.AntAosPosn);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 19:AntWaitPoint(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.AntWaitPoint);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);
		}
#endif /* _DEBUG */
#else
		// CID�Ńp�^�[���ʂɊ����񊈐����قȂ邩�`�F�b�N
		stCIDPrmFlg	stpflgSat;
		memset(&stpflgSat, 0, sizeof(stpflgSat));
	
		for (int nFreadCnt = 0; nFreadCnt < 2; nFreadCnt++)
		{
			if ((_wfopen_s(&fp, file, _T("r")) != 0) || (fp == NULL))
			{
				if (bRead == true)
					return TRUE;
				else
					return FALSE;
			}
			stCIDPrmEle	stele;
			stCIDParam	stcid;
			stCIDPrmFlg	stpflg;
		
			char sz_line[512];
			memset(&stpflg, 0, sizeof(stpflg));
	
			while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp) != NULL)
			{
				bRead = true;
	//			memset(&stpflg, 0, sizeof(stpflg));
		
				wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
				CString line(sz_buff);
				BOOL isLineRead = TRUE;
				int	 iStart = 0;
				int  iFind = 0;
		
				int iCount = 0;
				int prmIdx = 0;
		
				while (isLineRead)
				{
					iFind = line.Find(_T(","), iStart);
					CString item = _T("");
		
					if (iFind == -1)
					{
						iFind = line.Find(_T("\n"), iStart);
						isLineRead = FALSE;
					}
		
					item = line.Mid(iStart, iFind - iStart);
		
					switch (iCount)
					{
					case 0:		// CID�ԍ�
						stcid.CID = item;
						AddDataVector(stele.CID, item);
						break;
		
					case 1:		// �p�����[�^�ԍ�
						prmIdx = _wtoi(item);
						break;
		
					case 2:		// �p�����[�^��
						break;
		
					case 3:		// �p�����[�^�L���t���O
					{
						int nflg = _wtoi(item);
						switch (prmIdx)
						{
						case 1:	// S�уe�����g���r�b�g���[�g
							stpflg.TlmBitRateS = nflg;
							break;
						case 2:	// X�уe�����g���r�b�g���[�g
							stpflg.TlmBitRateX = nflg;
							break;
						case 3:	// X�����e�����g���r�b�g���[�g
							stpflg.TlmBitRateHX = nflg;
							break;
						case 4:	// Ka�уe�����g���r�b�g���[�g 
							stpflg.TlmBitRateKa = nflg;
							break;
						case 5:	// S�уR�}���h�r�b�g���[�g
							stpflg.CmdBitRateS = nflg;
							break;
						case 6:	// X�уR�}���h�r�b�g���[�g
							stpflg.CmdBitRateX = nflg;
							break;
						case 7:	// ���M�d��
							stpflg.TrsPower = nflg;
							break;
						case 8:	// �A�b�v�����N�|���p�����[�^
							stpflg.UpLinkSweepShape = nflg;
							break;
						case 9:	// �����W�v���p�����[�^ ���[�h
							stpflg.RngMesMode = nflg;
							break;
						case 10:// S�уL�����A��M���[�v�o���h
							stpflg.CarrLoopBandS = nflg;
							break;
						case 11:// X�уL�����A��M���[�v�o���h
							stpflg.CarrLoopBandX = nflg;
							break;
						case 12:// Ka�уL�����A��M���[�v�o���h
							stpflg.CarrLoopBandKa = nflg;
							break;
						case 13:// �R�}���h�ϒ��x
							stpflg.CmdMod = nflg;
							break;
						case 14:// �R�}���h�T�u�L�����A���g��
							stpflg.CmdSubCarrFreq = nflg;
							break;
						case 15:// ���M�h�b�v���⏞�̗L��
							stpflg.TransDoppler = nflg;
							break;
						case 16:// �A���e�i�����ǔ�
							stpflg.AntAutoTrack = nflg;
							break;
						case 17:// �ϒ�����
							stpflg.Modulation = nflg;
							break;
						case 18:// �A���e�i�҂��󂯎w���̗L��
							stpflg.AntAosPosn = nflg;
							break;
						case 19:// �A���e�i�ҋ@�ʒu�w���̗L��
							stpflg.AntWaitPoint = nflg;
		
							//							cidPrmFlglist[sat.MakeUpper()] = stpflg;
							if (nFreadCnt == 0)
							{
								memcpy(&stpflgSat, &stpflg, sizeof(stpflgSat));
							}
							else
							{
								// �ŏI�p�^�[���̉q���ʂ̃p�����[�^�����񊈐����ƌ��݃p�^�[�����r
								CString strLog;
								strLog.Format(_T("%s, %s, CID(%s), different active "), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID);
								if (stpflgSat.TlmBitRateS == FALSE && stpflgSat.TlmBitRateS != stpflg.TlmBitRateS) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(1:TlmBitRateS)"), nLogEx::debug);
								}
								if (stpflgSat.TlmBitRateX == FALSE && stpflgSat.TlmBitRateX != stpflg.TlmBitRateX) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(2:TlmBitRateX)"), nLogEx::debug);
								}
								if (stpflgSat.TlmBitRateHX == FALSE && stpflgSat.TlmBitRateHX != stpflg.TlmBitRateHX) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(3:TlmBitRateHX)"), nLogEx::debug);
								}
								if (stpflgSat.TlmBitRateKa == FALSE && stpflgSat.TlmBitRateKa != stpflg.TlmBitRateKa) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(4:TlmBitRateKa)"), nLogEx::debug);
								}
								if (stpflgSat.CmdBitRateS == FALSE && stpflgSat.CmdBitRateS != stpflg.CmdBitRateS) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(5:CmdBitRateS)"), nLogEx::debug);
								}
								if (stpflgSat.CmdBitRateX == FALSE && stpflgSat.CmdBitRateX != stpflg.CmdBitRateX) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(6:CmdBitRateX)"), nLogEx::debug);
								}
								if (stpflgSat.TrsPower == FALSE && stpflgSat.TrsPower != stpflg.TrsPower) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(7:TrsPower)"), nLogEx::debug);
								}
								if (stpflgSat.CarrLoopBandS == FALSE && stpflgSat.CarrLoopBandS != stpflg.CarrLoopBandS) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(10:CarrLoopBandS)"), nLogEx::debug);
								}
								if (stpflgSat.CarrLoopBandX == FALSE && stpflgSat.CarrLoopBandX != stpflg.CarrLoopBandX) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(11:CarrLoopBandX)"), nLogEx::debug);
								}
								if (stpflgSat.CarrLoopBandKa == FALSE && stpflgSat.CarrLoopBandKa != stpflg.CarrLoopBandKa) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(12:CarrLoopBandKa)"), nLogEx::debug);
								}
								if (stpflgSat.UpLinkSweepShape == FALSE && stpflgSat.UpLinkSweepShape != stpflg.UpLinkSweepShape) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(8:UpLinkSweepShape)"), nLogEx::debug);
								}
								if (stpflgSat.UpLinkSweepWidth == FALSE && stpflgSat.UpLinkSweepWidth != stpflg.UpLinkSweepWidth) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(-:UpLinkSweepWidth)"), nLogEx::debug);
								}
								if (stpflgSat.UpLinkSweepTime == FALSE && stpflgSat.UpLinkSweepTime != stpflg.UpLinkSweepTime) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(-:UpLinkSweepTime)"), nLogEx::debug);
								}
								if (stpflgSat.RngMesMode == FALSE && stpflgSat.RngMesMode != stpflg.RngMesMode) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(9:RngMesMode)"), nLogEx::debug);
								}
								if (stpflgSat.Modulation == FALSE && stpflgSat.Modulation != stpflg.Modulation) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(17:Modulation)"), nLogEx::debug);
								}
								if (stpflgSat.ModulationS == FALSE && stpflgSat.ModulationS != stpflg.ModulationS) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(-:ModulationS)"), nLogEx::debug);
								}
								if (stpflgSat.ModulationX == FALSE && stpflgSat.ModulationX != stpflg.ModulationX) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(-:ModulationX)"), nLogEx::debug);
								}
								if (stpflgSat.CmdMod == FALSE && stpflgSat.CmdMod != stpflg.CmdMod) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(13:CmdMod)"), nLogEx::debug);
								}
								if (stpflgSat.CmdSubCarrFreq == FALSE && stpflgSat.CmdSubCarrFreq != stpflg.CmdSubCarrFreq) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(14:CmdSubCarrFreq)"), nLogEx::debug);
								}
								if (stpflgSat.TransDoppler == FALSE && stpflgSat.TransDoppler != stpflg.TransDoppler) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(15:TransDoppler)"), nLogEx::debug);
								}
								if (stpflgSat.AntAutoTrack == FALSE && stpflgSat.AntAutoTrack != stpflg.AntAutoTrack) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(16:AntAutoTrack)"), nLogEx::debug);
								}
								if (stpflgSat.AntAosPosn == FALSE && stpflgSat.AntAosPosn != stpflg.AntAosPosn) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(18:AntAosPosn)"), nLogEx::debug);
								}
								if (stpflgSat.AntWaitPoint == FALSE && stpflgSat.AntWaitPoint != stpflg.AntWaitPoint) {
									CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog + _T("param(19:AntWaitPoint)"), nLogEx::debug);
								}
							}
							break;
						}
					}
					break;
		
					case 4:		// �p�����[�^�l
					{
						switch (prmIdx)
						{
						case 1:	// S�уe�����g���r�b�g���[�g
							if (stpflg.TlmBitRateS == 0)
							{
								item = mEmptyString;
							}
							stcid.TlmBitRateS = item;
							AddDataVector(stele.TlmBitRateS, item);
							break;
						case 2:	// X�уe�����g���r�b�g���[�g
							if (stpflg.TlmBitRateX == 0)
							{
								item = mEmptyString;
							}
							stcid.TlmBitRateX = item;
							AddDataVector(stele.TlmBitRateX, item);
							break;
						case 3:	// X�����e�����g���r�b�g���[�g
							if (stpflg.TlmBitRateHX == 0)
							{
								item = mEmptyString;
							}
							stcid.TlmBitRateHX = item;
							AddDataVector(stele.TlmBitRateHX, item);
							break;
						case 4:	// Ka�уe�����g���r�b�g���[�g 
							if (stpflg.TlmBitRateKa == 0)
							{
								item = mEmptyString;
							}
							stcid.TlmBitRateKa = item;
							AddDataVector(stele.TlmBitRateKa, item);
							break;
						case 5:	// S�уR�}���h�r�b�g���[�g
							if (stpflg.CmdBitRateS == 0)
							{
								item = mEmptyString;
							}
							stcid.CmdBitRateS = item;
							AddDataVector(stele.CmdBitRateS, item);
							break;
						case 6:	// X�уR�}���h�r�b�g���[�g
							if (stpflg.CmdBitRateX == 0)
							{
								item = mEmptyString;
							}
							stcid.CmdBitRateX = item;
							AddDataVector(stele.CmdBitRateX, item);
							break;
						case 7:	// ���M�d��
							if (stpflg.TrsPower == 0)
							{
								item = mEmptyString;
							}
							stcid.TrsPower = item;
							AddDataVector(stele.TrsPower, item);
							break;
						case 8:	// �A�b�v�����N�|���p�����[�^
							if (stpflg.UpLinkSweepShape == 0)
							{
								item = mEmptyString;
							}
							stcid.UpLinkSweepShape = item;
							AddDataVector(stele.UpLinkSweepShape, item);
							break;
						case 9:	// �����W�v���p�����[�^ ���[�h
							if (stpflg.RngMesMode == 0)
							{
								item = mEmptyString;
							}
							stcid.RngMesMode = item;
							AddDataVector(stele.RngMesMode, item);
							break;
						case 10:// S�уL�����A��M���[�v�o���h
							if (stpflg.CarrLoopBandS == 0)
							{
								item = mEmptyString;
							}
							stcid.CarrLoopBandS = item;
							AddDataVector(stele.CarrLoopBandS, item);
							break;
						case 11:// X�уL�����A��M���[�v�o���h
							if (stpflg.CarrLoopBandX == 0)
							{
								item = mEmptyString;
							}
							stcid.CarrLoopBandX = item;
							AddDataVector(stele.CarrLoopBandX, item);
							break;
						case 12:// Ka�уL�����A��M���[�v�o���h
							if (stpflg.CarrLoopBandKa == 0)
							{
								item = mEmptyString;
							}
							stcid.CarrLoopBandKa = item;
							AddDataVector(stele.CarrLoopBandKa, item);
							break;
						case 13:// �R�}���h�ϒ��x
							if (stpflg.CmdMod == 0)
							{
								item = mEmptyString;
							}
							stcid.CmdMod = item;
							AddDataVector(stele.CmdMod, item);
							break;
						case 14:// �R�}���h�T�u�L�����A���g��
							if (stpflg.CmdSubCarrFreq == 0)
							{
								item = mEmptyString;
							}
							stcid.CmdSubCarrFreq = item;
							AddDataVector(stele.CmdSubCarrFreq, item);
							break;
						case 15:// ���M�h�b�v���⏞�̗L��
							if (stpflg.TransDoppler == 0)
							{
								item = mEmptyString;
							}
							stcid.TransDoppler = item;
							AddDataVector(stele.TransDoppler, item);
							break;
						case 16:// �A���e�i�����ǔ�
							if (stpflg.AntAutoTrack == 0)
							{
								item = mEmptyString;
							}
							stcid.AntAutoTrack = item;
							AddDataVector(stele.AntAutoTrack, item);
							break;
						case 17:// �ϒ�����
							if (stpflg.Modulation == 0)
							{
								item = mEmptyString;
							}
							stcid.Modulation = item;
							AddDataVector(stele.Modulation, item);
							break;
						case 18:// �A���e�i�҂��󂯎w���̗L��
							if (stpflg.AntAosPosn == 0)
							{
								item = mEmptyString;
							}
							stcid.AntAosPosn = item;
							AddDataVector(stele.AntAosPosn, item);
							break;
						case 19:// �A���e�i�ҋ@�ʒu�w���̗L��
							if (stpflg.AntWaitPoint == 0)
							{
								item = mEmptyString;
							}
							stcid.AntWaitPoint = item;
							AddDataVector(stele.AntWaitPoint, item);
		
							cidlist[sat.MakeUpper()][stcid.CID] = stcid;
							memset(&stpflg, 0, sizeof(stpflg));		// CID���ς�邽�тɃN���A
							break;
						}
					}
					break;
					}
		
					iStart = iFind + 1;
					iCount++;
		
				}
			}
		
			// �q���ʂ̃p�����[�^�����񊈐��o�^
			cidPrmFlglist[sat.MakeUpper()] = stpflgSat;
			cidEleList[sat.MakeUpper()] = stele;
			fclose(fp);
		}
#endif	/* _CHK_CIDFILE */

	}

	if (bRead == true)
		return TRUE;
	else
		return FALSE;
}

/*============================================================================*/
/*! CSatelliteData

-# vector<CString> �ɒl��ǉ�����B�A�����j�[�N�B

@param  v		vector<CString>
@param  str		������
@retval �Ȃ�
*/
/*============================================================================*/
void CSatelliteData::AddDataVector(vector<CString> &v, CString str)
{
	for (auto itr = v.begin(); itr != v.end(); ++itr)
	{
		if (str == *itr)
		{
			return;
		}
	}

	v.push_back(str);
}

/*============================================================================*/
/*! CSatelliteData

-# �T�[�o�[�̊e��t�@�C�����擾����

@param	�Ȃ�
@retval	1�F���� / -1�F���s

*/
/*============================================================================*/
void CSatelliteData::GetShareFile(UINT val/*=0xffffffff*/)
{
	bool bUpdatePred = false;
	bool bUpdatePlan = false;

	if (val & eShareFile_PredList){
		CGraphMutex::Lock(eFile);
		vector<stPredListData> back;
		m_listPredList.swap(back);
		// �\��l�ꗗ�t�@�C�����f�[�^�擾
		if (SetPredList_Euc() == TRUE){
			if (back.size() != m_listPredList.size()){
				bUpdatePred = true;
			}
			else  if (m_listPredList.size() == 0){
				bUpdatePred = true;
			}
			else{
				UINT size = (UINT)m_listPredList.size();
				for (UINT i = 0; i < size; i++){
					if (CompareData(m_listPredList[i], back[i]) == true){
						bUpdatePred = true;
						break;
					}
				}
			}
		}
		else{
			m_listPredList.swap(back);
		}
		CGraphMutex::Unlock(eFile);
	}
	if (val & eShareFile_PlanList){
		CGraphMutex::Lock(eFile);
		vector<stPlanListData> back;
		m_listPlanList.swap(back);
		// �v��ꗗ�t�@�C�����f�[�^�擾(UTF8�Ή�)
		if (SetPlanList_Euc() == TRUE){
			if (back.size() != m_listPlanList.size()){
				bUpdatePlan = true;
			}
			else if (m_listPlanList.size() == 0){
				bUpdatePlan = true;
			}
			else{
				UINT size = (UINT)m_listPlanList.size();
				for (UINT i = 0; i < size; i++){
					if (CompareData(m_listPlanList[i], back[i]) == true){
						bUpdatePlan = true;
						break;
					}
				}
			}
		}
		else{
			m_listPlanList.swap(back);
		}
		CGraphMutex::Unlock(eFile);
	}
	if (bUpdatePred == true || bUpdatePlan == true){
		LPARAM val = (bUpdatePred == true) ? eShareFile_PredList : 0;
		val |= (bUpdatePlan == true) ? eShareFile_PlanList : 0;
		CWnd* p = (CWnd*)theApp.GetWindowHandle(IDD_DLG_FORECASTLIST);
		if (p != NULL){
			p->PostMessage(eMessage_UpdateShareFile, 0, val);
		}
		CView* pv = theApp.GetView(eViewTypeTool_TimeLine);
		if (pv != NULL){
			pv->PostMessage(eMessage_UpdateShareFile, 0, val);
		}

		CWnd* pfreq;
		if (theApp.GetSelectStation() == eStation_Usuda642)
		{
			pfreq = (CWnd*)theApp.GetWindowHandle(IDD_DLG_FREQ_CTRL54);
		}
		else
		{
			pfreq = (CWnd*)theApp.GetWindowHandle(IDD_DLG_FREQ_CTRL);
		}
		if (pfreq != NULL)
		{
			pfreq->PostMessage(eMessage_UpdateShareFile, 0, val);
		}

		CWnd* psatellite;
		if (theApp.GetSelectStation() == eStation_Usuda642)
		{
			psatellite = (CWnd*)theApp.GetWindowHandle(IDD_DLG_SATELLITECAPTURE54);	//�q���ߑ�������
		}
		else
		{
			psatellite = (CWnd*)theApp.GetWindowHandle(IDD_DLG_SATELLITECAPTURE);	//�q���ߑ�������
		}
		if (psatellite != NULL)
		{
			psatellite->PostMessage(eMessage_UpdateShareFile, 0, val);
		}
	}
}

/*============================================================================*/
/*! CSatelliteData

-# ���l�ɃJ���}��ǉ�

@param	pos�F�J���}��t���������l
@param	pointKeta�F�����_�ȉ��̌��� (�����_0�`6)
@param	singFlag�F�����t���O�i0�ȊO�Œl�����̂Ƃ�'+'��t��
@retval	�J���}��t������̐��l

*/
/*============================================================================*/
CString CSatelliteData::AddCanma(double pos, int pointKeta, int singFlag /*= 0*/)
{
	int	i, j, keta, temp;
	int sign = 0;
	double tempPos = pos;
	char str[MAXDIGIT14];

	if (pos >= 100000000000000.0 || pos <= -100000000000000.0)
	{
		return CString(_T("###,###,###,###,###.###"));
	}

	if (isnan(pos)){
		// NaN (��)�̏ꍇ�͂O�ɂ���
		tempPos = 0.0;
	}

	// 0�ɋ߂��ꍇ�͂܂�߂�
	switch (pointKeta)
	{
	case 0:
		if (tempPos > -0.1 && tempPos < 0.1)
		{
			tempPos = 0.0;
		}
		break;
	case 1:
		if (tempPos > -0.01 && tempPos < 0.01)
		{
			tempPos = 0.0;
		}
		break;
	case 2:
		if (tempPos > -0.001 && tempPos < 0.001)
		{
			tempPos = 0.0;
		}
		break;
	case 3:
		if (tempPos > -0.0001 && tempPos < 0.0001)
		{
			tempPos = 0.0;
		}
		break;
	case 4:
		if (tempPos > -0.00001 && tempPos < 0.00001)
		{
			tempPos = 0.0;
		}
		break;
	case 5:
		if (tempPos > -0.000001 && tempPos < 0.000001)
		{
			tempPos = 0.0;
		}
		break;
	case 6:
		if (tempPos > -0.0000001 && tempPos < 0.0000001)
		{
			tempPos = 0.0;
		}
		break;
	case 7:
		if (tempPos > -0.00000001 && tempPos < 0.00000001)
		{
			tempPos = 0.0;
		}
		break;
	case 8:
		if (tempPos > -0.000000001 && tempPos < 0.000000001)
		{
			tempPos = 0.0;
		}
		break;
	case 9:
		if (tempPos > -0.0000000001 && tempPos < 0.0000000001)
		{
			tempPos = 0.0;
		}
		break;
	default:
		break;
	}

	__int64 num = (__int64)tempPos;

	double dnum = fabs(tempPos - (double)num);
	CString strSyosuu;

	switch (pointKeta)
	{
	case 0:
		strSyosuu.Format(_T(""));
		break;
	case 1:
		strSyosuu.Format(_T("%.1f"), dnum);
		break;
	case 2:
		strSyosuu.Format(_T("%.2f"), dnum);
		break;
	case 3:
		strSyosuu.Format(_T("%.3f"), dnum);
		break;
	case 4:
		strSyosuu.Format(_T("%.4f"), dnum);
		break;
	case 5:
		strSyosuu.Format(_T("%.5f"), dnum);
		break;
	case 6:
		strSyosuu.Format(_T("%.6f"), dnum);
		break;
	case 7:
		strSyosuu.Format(_T("%.7f"), dnum);
		break;
	case 8:
		strSyosuu.Format(_T("%.8f"), dnum);
		break;
	case 9:
		strSyosuu.Format(_T("%.9f"), dnum);
		break;
	default:
		strSyosuu.Format(_T(""));
		break;
	}

	__int64 kuriage = 0;
	if (strSyosuu.Left(1).Compare(_T("1")) == 0)
	{
		kuriage = 1;
	}

	strSyosuu.Delete(0);

	// �����𒲂ׂ�
	if (tempPos < 0)
	{
		sign = 1;
		num = -num;
	}

	// �J�グ�𑫂�
	num = num + kuriage;

	// �������ɃJ���}��t����
	i = 0;
	keta = 0;

	do
	{
		str[i++] = num % 10 + '0';
		keta++;
		num /= 10;
		if (keta % 3 == 0 && num != 0)
		{
			str[i++] = ',';
		}
	} while (num != 0 && i < sizeof(str));

	if (i >= MAXDIGIT14)
	{
		return CString(_T("###,###,###,###,###.######"));
	}
	if (sign == 1)
	{
		// ���̂Ƃ�
		if (i + 1 >= MAXDIGIT14)
		{
			return CString(_T("###,###,###,###,###.######"));
		}
		str[i++] = '-';
	}
	else if (pos > 0.0)
	{
		// ���̂Ƃ�
		if (singFlag)
		{
			if (i + 1 >= MAXDIGIT14)
			{
				return CString(_T("###,###,###,###,###.######"));
			}
			str[i++] = '+';
		}
	}

	str[i] = '\0';

	j = i - 1;
	for (i = 0; j > i; i++, j--)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
	}

	return CString(str) + strSyosuu;
}


//#if 0
///*============================================================================*/
///*! CSatelliteData
//
//-# �X�y�N�g�����f�[�^����������
//
//@param  nStationID�F�q��ID
//@retval 0�F����I�� / -1�F�ُ�I��
//*/
///*============================================================================*/
//void CSatelliteData::SpectrumInitialize()
//{
//	if (theApp.GetSelectStationMode() == eStation_1)
//	{
//		// 1�ǉ��
//		UINT nStationID = theApp.GetSelectStation();
//		// ������
//		SpectrumInitialize(nStationID);
//		// �t�@�C���ǂݍ���
//		if (SpectrumFileRead(nStationID) < 0)
//		{
//			// �t�@�C�������������A�܂��̓t�H�[�}�b�g�Ⴂ�̏ꍇ�A�t�@�C������蒼��
//			SpectrumFileWrite(nStationID);
//			SpectrumFileRead(nStationID);
//		}
//	}
//	else
//	{
//		// 4�ǉ��
//		for (int i = 0; i < eStation_MAX; i++)
//		{
//			// ������
//			SpectrumInitialize(i);
//			// �t�@�C���ǂݍ���
//			if (SpectrumFileRead(i) < 0)
//			{
//				// �t�@�C�������������A�܂��̓t�H�[�}�b�g�Ⴂ�̏ꍇ�A�t�@�C������蒼��
//				SpectrumFileWrite(i);
//				SpectrumFileRead(i);
//			}
//		}
//	}
//
//}
//
//
///*============================================================================*/
///*! CSatelliteData
//
//-# �X�y�N�g�����f�[�^����������
//
//@param  nStationID�F�q��ID
//@retval 0�F����I�� / -1�F�ُ�I��
//*/
///*============================================================================*/
//void CSatelliteData::SpectrumInitialize(UINT nStationID)
//{
//	// �~�j�X�y�N�g����������
//	for (int i = 0; i < MAX_SUMMARY_VIEW; i++)
//	{
//		GetSummarySpectrumViewData(nStationID, i).InitializeData(eSpectrumGraphType_Sammary);
//	}
//
//	// �X�y�N�g����������
//	GetSpectrumViewData().InitializeData(eSpectrumGraphType_Normal);
//}
//
//
///*============================================================================*/
///*! CSatelliteData
//
//-# �X�y�N�g�����f�[�^�t�@�C���ǂݍ��ݏ���
//
//@param  nStationID�F�q��ID
//@retval 0�F����I�� / -1�F�ُ�I��
//*/
///*============================================================================*/
//int CSatelliteData::SpectrumFileRead(UINT nStationID)
//{
//	FILE*	fp;
//	int ret;
//
//	/*------------------*/
//	/* �t�@�C�����擾   */
//	/*------------------*/
//	CString strAppPath = theApp.GetAppPath();
//	CString strFilePath;
//	strFilePath.Format(_T("%s%s_%s.%s"), strAppPath, CString(SPECTRUM_FILE), CString(mStationSimpleString[nStationID]), CString(SPECTRUM_FILE_TYPE));
//
//	CStringA strHeaderA;
//
//	if (_wfopen_s(&fp, strFilePath, _T("r+")) != 0) {
//		return(ERR_RETURN);
//	}
//
//	// �~�j�X�y�N�g����
//	for (int i = 0; i < MAX_SUMMARY_VIEW; i++)
//	{
//		ret = m_arraySummarySpectrumData[nStationID][i].FileRead(fp, nStationID, i, eSpectrumGraphType_Sammary);
//		if (ret < 0)
//		{
//			fclose(fp);
//			return ret;
//		}
//	}
//
//	// �X�y�N�g����
//	ret = m_spectrumData.FileRead(fp, nStationID, 0, eSpectrumGraphType_Normal);
//
//	fclose(fp);
//
//	return ret;
//}
//
//
///*============================================================================*/
///*! CSatelliteData
//
//-# �X�y�N�g�����f�[�^�t�@�C���������ݏ���
//
//@param  nStationID�F�q��ID
//@retval 0�F����I�� / -1�F�ُ�I��
//*/
///*============================================================================*/
//int CSatelliteData::SpectrumFileWrite(UINT nStationID)
//{
//	FILE*	fp;
//	int ret = ERR_RETURN;
//
//	/*------------------*/
//	/* �t�@�C�����擾   */
//	/*------------------*/
//	CString strAppPath = theApp.GetAppPath();
//	CString strFilePath;
//	strFilePath.Format(_T("%s%s_%s.%s"), strAppPath, CString(SPECTRUM_FILE), CString(mStationSimpleString[nStationID]), CString(SPECTRUM_FILE_TYPE));
//
//	if (_wfopen_s(&fp, strFilePath, _T("w")) != 0) {
//		return(ERR_RETURN);
//	}
//
//	// �~�j�X�y�N�g����
//	for (int i = 0; i < MAX_SUMMARY_VIEW; i++)
//	{
//		ret = m_arraySummarySpectrumData[nStationID][i].FileWrite(fp, nStationID, i, eSpectrumGraphType_Sammary);
//		if (ret < 0)
//		{
//			fclose(fp);
//			return ret;
//		}
//	}
//
//	// �X�y�N�g����
//	ret = m_spectrumData.FileWrite(fp, nStationID, 0, eSpectrumGraphType_Normal);
//
//	fclose(fp);
//
//	return ret;
//}
//
//
///*============================================================================*/
///*! CSatelliteData
//
//-# �g�����h�v���p�e�B�f�[�^����������
//
//@param  nStationID�F�q��ID
//@retval 0�F����I�� / -1�F�ُ�I��
//*/
///*============================================================================*/
//void CSatelliteData::TrendPropInitialize()
//{
//	if (theApp.GetSelectStationMode() == eStation_1)
//	{
//		// 1�ǉ��
//		UINT nStationID = theApp.GetSelectStation();
//		// �t�@�C���ǂݍ���
//		(void)TrendPropReadData(nStationID);
////		if (!TrendPropReadData(nStationID))
////		{
////			// �t�@�C�������������A�܂��̓t�H�[�}�b�g�Ⴂ�̏ꍇ�A�t�@�C������蒼��
////			TrendPropWriteData(nStationID);
////		}
//	}
//	else
//	{
//		// 4�ǉ��
//		for (int i = 0; i < eStation_MAX; i++)
//		{
//			// �t�@�C���ǂݍ���
//			(void)TrendPropReadData(i);
////			if (!TrendPropReadData(i))
////			{
////				// �t�@�C�������������A�܂��̓t�H�[�}�b�g�Ⴂ�̏ꍇ�A�t�@�C������蒼��
////				TrendPropWriteData(i);
////			}
//		}
//	}
//
//}
//
//
//#if 0
///*============================================================================*/
///*! CSatelliteData
//
//-# �g�����h�v���p�e�B�f�[�^�ۑ��̊J�n����
//
//@param	�Ȃ�
//@return	bool		����
//
//*/
///*============================================================================*/
//bool CSatelliteData::TrendPropWriteInitialization()
//{
//	return m_trendPropMemMapList.WriteInitialization();
//}
//
///*============================================================================*/
///*! CSatelliteData
//
//-#	�g�����h�v���p�e�B�f�[�^�ۑ��̏I������
//@param	�Ȃ�
//@return	bool		����
//
//*/
///*============================================================================*/
//bool CSatelliteData::TrendPropWriteFinalization()
//{
//	return m_trendPropMemMapList.WriteFinalization();
//}
//#endif
//
///*============================================================================*/
///*! CSatelliteData
//
//-# �g�����h�v���p�e�B�f�[�^�̏�������
//
//@param	nStationID		��ID
//@return	bool			����
//
//*/
///*============================================================================*/
//bool CSatelliteData::TrendPropWriteData(UINT nStationID)
//{
//	bool ret = true;
//	/*------------------*/
//	/* �t�@�C�����擾   */
//	/*------------------*/
//	CString strAppPath = theApp.GetAppPath();
//	CString strFilePath;
//	strFilePath.Format(_T("%s%s_%s.%s"), strAppPath, CString(TRENDPROP_FILE), CString(mStationSimpleString[nStationID]), CString(TRENDPROP_FILE_TYPE));
//
//	vector<CPropertiesData> propertiesdataList;
//	// �q���ߑ��O���t�ݒ�
//	for (int i = 0; i < eTrendWndType_Num; i++)
//	{
//		propertiesdataList.push_back(m_arrayTrendPropertiesData[i]);
//	}
//	// �T�}���O���t�ݒ�
//	for (int i = 0; i < MAX_SUMMARY_VIEW; i++)
//	{
//		propertiesdataList.push_back(m_arraySummaryPropertiesData[nStationID][i]);
//	}
//
//	// ������
//	if (!m_trendPropMemMapList.WriteInitialization(strFilePath))
//		return false;
//
//	// �ۑ�
//	if (!m_trendPropMemMapList.WriteData(strFilePath, propertiesdataList))
//		ret = false;
//
//	// �ۑ��I��
//	if (!m_trendPropMemMapList.WriteFinalization())
//		ret = false;
//
//	return ret;
//}
//
//#if 0
///*============================================================================*/
///*! CSatelliteData
//
//-#	�g�����h�v���p�e�B�f�[�^�W�J�̊J�n����
//
//@param	�Ȃ�
//@return	bool		����
//
//*/
///*============================================================================*/
//bool CSatelliteData::TrendPropReadInitialization()
//{
//	return m_trendPropMemMapList.ReadInitialization();
//}
//
///*============================================================================*/
///*! CSatelliteData
//
//
//-#	�g�����h�v���p�e�B�f�[�^�W�J�̏I������
//@param
//@return	bool		����
//
//*/
///*============================================================================*/
//bool CSatelliteData::TrendPropReadFinalization()
//{
//	return m_trendPropMemMapList.ReadFinalization();
//}
//#endif
//
///*============================================================================*/
///*! CSatelliteData
//
//-#	�g�����h�v���p�e�B�f�[�^�̓ǂݍ���
//
//@param	nStationID		��ID
//@return	bool			����
//
//*/
///*============================================================================*/
//bool CSatelliteData::TrendPropReadData(UINT nStationID)
//{
//	bool ret = true;
//	/*------------------*/
//	/* �t�@�C�����擾   */
//	/*------------------*/
//	CString strAppPath = theApp.GetAppPath();
//	CString strFilePath;
//	strFilePath.Format(_T("%s%s_%s.%s"), strAppPath, CString(TRENDPROP_FILE), CString(mStationSimpleString[nStationID]), CString(TRENDPROP_FILE_TYPE));
//
//	vector<CPropertiesData> propertiesdataList;
//	// �q���ߑ��O���t�ݒ�
//	for (int i = 0; i < eTrendWndType_Num; i++)
//	{
//		propertiesdataList.push_back(m_arrayTrendPropertiesData[i]);
//	}
//	// �T�}���O���t�ݒ�
//	for (int i = 0; i < MAX_SUMMARY_VIEW; i++)
//	{
//		propertiesdataList.push_back(m_arraySummaryPropertiesData[nStationID][i]);
//	}
//
//	// ������
//	if (!m_trendPropMemMapList.ReadInitialization(strFilePath))
//		return false;
//
//	// �ۑ�
//	if (!m_trendPropMemMapList.ReadData(strFilePath, propertiesdataList))
//		ret = false;
//
//	// �O���t�ݒ�ɔ��f
//	// �q���ߑ��O���t�ݒ�
//	for (int i = 0; i < eTrendWndType_Num; i++)
//	{
//		m_arrayTrendPropertiesData[i] = propertiesdataList[i];
//	}
//	// �T�}���O���t�ݒ�
//	for (int i = 0; i < MAX_SUMMARY_VIEW; i++)
//	{
//		m_arraySummaryPropertiesData[nStationID][i] = propertiesdataList[i + eTrendWndType_Num];
//	}
//
//	// �ۑ��I��
//	if (!m_trendPropMemMapList.ReadFinalization())
//		ret = false;
//
//	return ret;
//
//}
//#endif

/*============================================================================*/
/*! �f�[�^�Ǘ�

-# ���C�A�E�g�ǂݍ��݁i�ݒ�ǂݍ��݁j

@param  �Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
bool CSatelliteData::Load(CArchive& ar)
{
	try
	{
		// �g�����h�f�[�^
		for (int item = 0; item < eTrendWndType_Num; item++)
		{
			ar >> m_arrayTrendPropertiesData[item].wndIdx;						// �E�B���h�[�ԍ�
			ar >> m_arrayTrendPropertiesData[item].wndName;					// �E�B���h�[����
			ar >> m_arrayTrendPropertiesData[item].wnd;						// CView
			ar >> m_arrayTrendPropertiesData[item].nGraphIdx;					// �O���t�ԍ�
			ar >> m_arrayTrendPropertiesData[item].isRealTime;					// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			ar >> m_arrayTrendPropertiesData[item].startTime;					// �J�n����
			//stVScale	trandVScale;				// �c���X�P�[��[0:���A1:�E]
			ar >> m_arrayTrendPropertiesData[item].trandVScale.Auto;
			ar >> m_arrayTrendPropertiesData[item].trandVScale.VScalLMax;
			ar >> m_arrayTrendPropertiesData[item].trandVScale.VScalLMin;
			ar >> m_arrayTrendPropertiesData[item].trandVScale.VScalRMax;
			ar >> m_arrayTrendPropertiesData[item].trandVScale.VScalRMin;
			ar >> m_arrayTrendPropertiesData[item].trandVScale.VSCalItvl;
			//HScale		trandHScale[MAX_HSCALE];	// �����X�P�[��[0:�g�����h�O���t�r���[1�A1:�g�����h�O���t�r���[2]
			for (int i = 0; i < MAX_HSCALE; i++){
				int val;
				ar >> val;
				m_arrayTrendPropertiesData[item].trandHScale[i] = (HScale)val;
			}
			//CString		dataName[MAX_TDATA];		// �f�[�^����
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].dataName[i];
			}
			//CString		dataGName[MAX_TDATA];		// �f�[�^����(�O���t�p��)
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].dataGName[i];
			}
			//CString		dataUnit[MAX_TDATA];		// �P��
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].dataUnit[i];
			}
			//int			nDecDidits[MAX_TDATA];		// �����_�ȉ��̌���
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].nDecDidits[i];
			}
			//bool		isViewMode[MAX_TDATA];		// �\��On/Off
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].isViewMode[i];
			}
			//DrawMode	drawMode[MAX_TDATA];		// �`�惂�[�h�iLine�ADots�AL&D�j
			for (int i = 0; i < MAX_TDATA; i++){
				int val;
				ar >> val;
				m_arrayTrendPropertiesData[item].drawMode[i] = (DrawMode)val;
			}
			//COLORREF	color[MAX_TDATA];			// �\���F
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].color[i];
			}
			//int			lineWidth[MAX_TDATA];		// ����
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].lineWidth[i];
			}
			//DotSize		dotSize[MAX_TDATA];			// �h�b�g�T�C�Y
			for (int i = 0; i < MAX_TDATA; i++){
				int val;
				ar >> val;
				m_arrayTrendPropertiesData[item].dotSize[i] = (DotSize)val;
			}
			//VScaleKind	vscaleKind[MAX_TDATA];		// �c���X�P�[���I���i�E/���j
			for (int i = 0; i < MAX_TDATA; i++){
				int val;
				ar >> val;
				m_arrayTrendPropertiesData[item].vscaleKind[i] = (VScaleKind)val;
			}
			//CString		comment[MAX_TDATA];			// �R�����g
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].comment[i];
			}
			//bool		isComment[MAX_TDATA];		// �R�����g�\��
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].isComment[i];
			}
			//bool		isAlarm[MAX_TDATA];			// �A���[���\��
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].isAlarm[i];
			}
			//double		alarm1Major[MAX_TDATA];		// �A���[��1 ���
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].alarm1Major[i];
			}
			//double		alarm1Minor[MAX_TDATA];		// �A���[��1 ���� 
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].alarm1Minor[i];
			}
			//double		alarm2Major[MAX_TDATA];		// �A���[��2 ���
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].alarm2Major[i];
			}
			//double		alarm2Minor[MAX_TDATA];		// �A���[��2 ����
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].alarm2Minor[i];
			}
			ar >> m_arrayTrendPropertiesData[item].m_bSetted;
		}
		// �X�y�N�g�����f�[�^
		ar >> m_spectrumData.m_stVScale.strHeader;
		ar >> m_spectrumData.m_stVScale.bAutoScale;
		ar >> m_spectrumData.m_stVScale.gl_y_grid_max;
		ar >> m_spectrumData.m_stVScale.gl_y_grid_min;
		ar >> m_spectrumData.m_stVScale.gl_y_grid_val;
		ar >> m_spectrumData.m_stVScale.gd_y_offset_val;
		for (int item = 0; item < eStation_MAX; item++){
			for (int sitem = 0; sitem < MAX_SUMMARY_VIEW; sitem++){
				ar >> m_arraySummarySpectrumData[item][sitem].m_stVScale.strHeader;
				ar >> m_arraySummarySpectrumData[item][sitem].m_stVScale.bAutoScale;
				ar >> m_arraySummarySpectrumData[item][sitem].m_stVScale.gl_y_grid_max;
				ar >> m_arraySummarySpectrumData[item][sitem].m_stVScale.gl_y_grid_min;
				ar >> m_arraySummarySpectrumData[item][sitem].m_stVScale.gl_y_grid_val;
				ar >> m_arraySummarySpectrumData[item][sitem].m_stVScale.gd_y_offset_val;
			}
		}
		// �T�}���f�[�^
		for (int item = 0; item < eStation_MAX; item++){
			for (int sitem = 0; sitem < MAX_SUMMARY_VIEW; sitem++){
				ar >> m_arraySummaryPropertiesData[item][sitem].wndIdx;						// �E�B���h�[�ԍ�
				ar >> m_arraySummaryPropertiesData[item][sitem].wndName;					// �E�B���h�[����
				ar >> m_arraySummaryPropertiesData[item][sitem].wnd;						// CView
				ar >> m_arraySummaryPropertiesData[item][sitem].nGraphIdx;					// �O���t�ԍ�
				ar >> m_arraySummaryPropertiesData[item][sitem].isRealTime;					// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
				ar >> m_arraySummaryPropertiesData[item][sitem].startTime;					// �J�n����
				//stVScale	trandVScale;				// �c���X�P�[��[0:���A1:�E]
				ar >> m_arraySummaryPropertiesData[item][sitem].trandVScale.Auto;
				ar >> m_arraySummaryPropertiesData[item][sitem].trandVScale.VScalLMax;
				ar >> m_arraySummaryPropertiesData[item][sitem].trandVScale.VScalLMin;
				ar >> m_arraySummaryPropertiesData[item][sitem].trandVScale.VScalRMax;
				ar >> m_arraySummaryPropertiesData[item][sitem].trandVScale.VScalRMin;
				ar >> m_arraySummaryPropertiesData[item][sitem].trandVScale.VSCalItvl;
				//HScale		trandHScale[MAX_HSCALE];	// �����X�P�[��[0:�g�����h�O���t�r���[1�A1:�g�����h�O���t�r���[2]
				for (int i = 0; i < MAX_HSCALE; i++){
					int val;
					ar >> val;
					m_arraySummaryPropertiesData[item][sitem].trandHScale[i] = (HScale)val;
				}
				//CString		dataName[MAX_TDATA];		// �f�[�^����
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].dataName[i];
				}
				//CString		dataGName[MAX_TDATA];		// �f�[�^����(�O���t�p��)
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].dataGName[i];
				}
				//CString		dataUnit[MAX_TDATA];		// �P��
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].dataUnit[i];
				}
				//int			nDecDidits[MAX_TDATA];		// �����_�ȉ��̌���
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].nDecDidits[i];
				}
				//bool		isViewMode[MAX_TDATA];		// �\��On/Off
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].isViewMode[i];
				}
				//DrawMode	drawMode[MAX_TDATA];		// �`�惂�[�h�iLine�ADots�AL&D�j
				for (int i = 0; i < MAX_TDATA; i++){
					int val;
					ar >> val;
					m_arraySummaryPropertiesData[item][sitem].drawMode[i] = (DrawMode)val;
				}
				//COLORREF	color[MAX_TDATA];			// �\���F
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].color[i];
				}
				//int			lineWidth[MAX_TDATA];		// ����
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].lineWidth[i];
				}
				//DotSize		dotSize[MAX_TDATA];			// �h�b�g�T�C�Y
				for (int i = 0; i < MAX_TDATA; i++){
					int val;
					ar >> val;
					m_arraySummaryPropertiesData[item][sitem].dotSize[i] = (DotSize)val;
				}
				//VScaleKind	vscaleKind[MAX_TDATA];		// �c���X�P�[���I���i�E/���j
				for (int i = 0; i < MAX_TDATA; i++){
					int val;
					ar >> val;
					m_arraySummaryPropertiesData[item][sitem].vscaleKind[i] = (VScaleKind)val;
				}
				//CString		comment[MAX_TDATA];			// �R�����g
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].comment[i];
				}
				//bool		isComment[MAX_TDATA];		// �R�����g�\��
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].isComment[i];
				}
				//bool		isAlarm[MAX_TDATA];			// �A���[���\��
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].isAlarm[i];
				}
				//double		alarm1Major[MAX_TDATA];		// �A���[��1 ���
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].alarm1Major[i];
				}
				//double		alarm1Minor[MAX_TDATA];		// �A���[��1 ���� 
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].alarm1Minor[i];
				}
				//double		alarm2Major[MAX_TDATA];		// �A���[��2 ���
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].alarm2Major[i];
				}
				//double		alarm2Minor[MAX_TDATA];		// �A���[��2 ����
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].alarm2Minor[i];
				}
				ar >> m_arraySummaryPropertiesData[item][sitem].m_bSetted;
			}
		}
	}
	catch (CArchiveException* e)
	{
		e->Delete();
		return false;
	}
	return true;
}

/*============================================================================*/
/*! �f�[�^�Ǘ�

-# ���C�A�E�g�ۑ��i�ݒ�ۑ��j

@param  �Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
bool CSatelliteData::Save(CArchive& ar)
{
	try
	{
		// �g�����h�f�[�^
		for (int item = 0; item < eTrendWndType_Num; item++)
		{
			ar << m_arrayTrendPropertiesData[item].wndIdx;						// �E�B���h�[�ԍ�
			ar << m_arrayTrendPropertiesData[item].wndName;					// �E�B���h�[����
			ar << m_arrayTrendPropertiesData[item].wnd;						// CView
			ar << m_arrayTrendPropertiesData[item].nGraphIdx;					// �O���t�ԍ�
			ar << m_arrayTrendPropertiesData[item].isRealTime;					// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			ar << m_arrayTrendPropertiesData[item].startTime;					// �J�n����
			//stVScale	trandVScale;				// �c���X�P�[��[0:���A1:�E]
			ar << m_arrayTrendPropertiesData[item].trandVScale.Auto;
			ar << m_arrayTrendPropertiesData[item].trandVScale.VScalLMax;
			ar << m_arrayTrendPropertiesData[item].trandVScale.VScalLMin;
			ar << m_arrayTrendPropertiesData[item].trandVScale.VScalRMax;
			ar << m_arrayTrendPropertiesData[item].trandVScale.VScalRMin;
			ar << m_arrayTrendPropertiesData[item].trandVScale.VSCalItvl;
			//HScale		trandHScale[MAX_HSCALE];	// �����X�P�[��[0:�g�����h�O���t�r���[1�A1:�g�����h�O���t�r���[2]
			for (int i = 0; i < MAX_HSCALE; i++){
				ar << (int)m_arrayTrendPropertiesData[item].trandHScale[i];
			}
			//CString		dataName[MAX_TDATA];		// �f�[�^����
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].dataName[i];
			}
			//CString		dataGName[MAX_TDATA];		// �f�[�^����(�O���t�p��)
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].dataGName[i];
			}
			//CString		dataUnit[MAX_TDATA];		// �P��
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].dataUnit[i];
			}
			//int			nDecDidits[MAX_TDATA];		// �����_�ȉ��̌���
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].nDecDidits[i];
			}
			//bool		isViewMode[MAX_TDATA];		// �\��On/Off
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].isViewMode[i];
			}
			//DrawMode	drawMode[MAX_TDATA];		// �`�惂�[�h�iLine�ADots�AL&D�j
			for (int i = 0; i < MAX_TDATA; i++){
				ar << (int)m_arrayTrendPropertiesData[item].drawMode[i];
			}
			//COLORREF	color[MAX_TDATA];			// �\���F
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].color[i];
			}
			//int			lineWidth[MAX_TDATA];		// ����
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].lineWidth[i];
			}
			//DotSize		dotSize[MAX_TDATA];			// �h�b�g�T�C�Y
			for (int i = 0; i < MAX_TDATA; i++){
				ar << (int)m_arrayTrendPropertiesData[item].dotSize[i];
			}
			//VScaleKind	vscaleKind[MAX_TDATA];		// �c���X�P�[���I���i�E/���j
			for (int i = 0; i < MAX_TDATA; i++){
				ar << (int)m_arrayTrendPropertiesData[item].vscaleKind[i];
			}
			//CString		comment[MAX_TDATA];			// �R�����g
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].comment[i];
			}
			//bool		isComment[MAX_TDATA];		// �R�����g�\��
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].isComment[i];
			}
			//bool		isAlarm[MAX_TDATA];			// �A���[���\��
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].isAlarm[i];
			}
			//double		alarm1Major[MAX_TDATA];		// �A���[��1 ���
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].alarm1Major[i];
			}
			//double		alarm1Minor[MAX_TDATA];		// �A���[��1 ���� 
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].alarm1Minor[i];
			}
			//double		alarm2Major[MAX_TDATA];		// �A���[��2 ���
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].alarm2Major[i];
			}
			//double		alarm2Minor[MAX_TDATA];		// �A���[��2 ����
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].alarm2Minor[i];
			}
			ar << m_arrayTrendPropertiesData[item].m_bSetted;
		}
		// �X�y�N�g�����f�[�^
		ar << m_spectrumData.m_stVScale.strHeader;
		ar << m_spectrumData.m_stVScale.bAutoScale;
		ar << m_spectrumData.m_stVScale.gl_y_grid_max;
		ar << m_spectrumData.m_stVScale.gl_y_grid_min;
		ar << m_spectrumData.m_stVScale.gl_y_grid_val;
		ar << m_spectrumData.m_stVScale.gd_y_offset_val;
		for (int item = 0; item < eStation_MAX; item++){
			for (int sitem = 0; sitem < MAX_SUMMARY_VIEW; sitem++){
				ar << m_arraySummarySpectrumData[item][sitem].m_stVScale.strHeader;
				ar << m_arraySummarySpectrumData[item][sitem].m_stVScale.bAutoScale;
				ar << m_arraySummarySpectrumData[item][sitem].m_stVScale.gl_y_grid_max;
				ar << m_arraySummarySpectrumData[item][sitem].m_stVScale.gl_y_grid_min;
				ar << m_arraySummarySpectrumData[item][sitem].m_stVScale.gl_y_grid_val;
				ar << m_arraySummarySpectrumData[item][sitem].m_stVScale.gd_y_offset_val;
			}
		}
		// �T�}���f�[�^
		for (int item = 0; item < eStation_MAX; item++){
			for (int sitem = 0; sitem < MAX_SUMMARY_VIEW; sitem++){
				ar << m_arraySummaryPropertiesData[item][sitem].wndIdx;						// �E�B���h�[�ԍ�
				ar << m_arraySummaryPropertiesData[item][sitem].wndName;					// �E�B���h�[����
				ar << m_arraySummaryPropertiesData[item][sitem].wnd;						// CView
				ar << m_arraySummaryPropertiesData[item][sitem].nGraphIdx;					// �O���t�ԍ�
				ar << m_arraySummaryPropertiesData[item][sitem].isRealTime;					// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
				ar << m_arraySummaryPropertiesData[item][sitem].startTime;					// �J�n����
				//stVScale	trandVScale;				// �c���X�P�[��[0:���A1:�E]
				ar << m_arraySummaryPropertiesData[item][sitem].trandVScale.Auto;
				ar << m_arraySummaryPropertiesData[item][sitem].trandVScale.VScalLMax;
				ar << m_arraySummaryPropertiesData[item][sitem].trandVScale.VScalLMin;
				ar << m_arraySummaryPropertiesData[item][sitem].trandVScale.VScalRMax;
				ar << m_arraySummaryPropertiesData[item][sitem].trandVScale.VScalRMin;
				ar << m_arraySummaryPropertiesData[item][sitem].trandVScale.VSCalItvl;
				//HScale		trandHScale[MAX_HSCALE];	// �����X�P�[��[0:�g�����h�O���t�r���[1�A1:�g�����h�O���t�r���[2]
				for (int i = 0; i < MAX_HSCALE; i++){
					ar << (int)m_arraySummaryPropertiesData[item][sitem].trandHScale[i];
				}
				//CString		dataName[MAX_TDATA];		// �f�[�^����
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].dataName[i];
				}
				//CString		dataGName[MAX_TDATA];		// �f�[�^����(�O���t�p��)
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].dataGName[i];
				}
				//CString		dataUnit[MAX_TDATA];		// �P��
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].dataUnit[i];
				}
				//int			nDecDidits[MAX_TDATA];		// �����_�ȉ��̌���
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].nDecDidits[i];
				}
				//bool		isViewMode[MAX_TDATA];		// �\��On/Off
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].isViewMode[i];
				}
				//DrawMode	drawMode[MAX_TDATA];		// �`�惂�[�h�iLine�ADots�AL&D�j
				for (int i = 0; i < MAX_TDATA; i++){
					ar << (int)m_arraySummaryPropertiesData[item][sitem].drawMode[i];
				}
				//COLORREF	color[MAX_TDATA];			// �\���F
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].color[i];
				}
				//int			lineWidth[MAX_TDATA];		// ����
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].lineWidth[i];
				}
				//DotSize		dotSize[MAX_TDATA];			// �h�b�g�T�C�Y
				for (int i = 0; i < MAX_TDATA; i++){
					ar << (int)m_arraySummaryPropertiesData[item][sitem].dotSize[i];
				}
				//VScaleKind	vscaleKind[MAX_TDATA];		// �c���X�P�[���I���i�E/���j
				for (int i = 0; i < MAX_TDATA; i++){
					ar << (int)m_arraySummaryPropertiesData[item][sitem].vscaleKind[i];
				}
				//CString		comment[MAX_TDATA];			// �R�����g
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].comment[i];
				}
				//bool		isComment[MAX_TDATA];		// �R�����g�\��
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].isComment[i];
				}
				//bool		isAlarm[MAX_TDATA];			// �A���[���\��
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].isAlarm[i];
				}
				//double		alarm1Major[MAX_TDATA];		// �A���[��1 ���
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].alarm1Major[i];
				}
				//double		alarm1Minor[MAX_TDATA];		// �A���[��1 ���� 
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].alarm1Minor[i];
				}
				//double		alarm2Major[MAX_TDATA];		// �A���[��2 ���
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].alarm2Major[i];
				}
				//double		alarm2Minor[MAX_TDATA];		// �A���[��2 ����
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].alarm2Minor[i];
				}
				ar << m_arraySummaryPropertiesData[item][sitem].m_bSetted;
			}
		}
	}
	catch (CArchiveException* e)
	{
		e->Delete();
		return false;
	}
	return true;
}


/*============================================================================*/
/*! CSatelliteData

-# STAT_SDTP�ŕۑ����ꂽ���[�J���̃Z�b�V������ԃt�@�C����ǂݍ���

@param	�Ȃ�
@retval	TRUE�F�ǂݍ��ݐ��� / FALSE�F�ǂݍ��ݎ��s

*/
/*============================================================================*/
BOOL CSatelliteData::ReadSessionStatusFile()
{
	// �Z�b�V������ԃ��b�N�t�@�C�������邩���ׂ�
	CString strFilePath;
	strFilePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppWorkPath(), STAT_SDTP_LOCKFILENAME);
	if (PathFileExists(strFilePath))
		return FALSE;

	FILE* fp;
	strFilePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppWorkPath(), STAT_SDTP_FILENAME);

	// �\��l�t�@�C�����X�V����Ă��邩�m�F
	HANDLE hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FILETIME fileTime;
	GetFileTime(hFile, NULL, NULL, &fileTime);
	CloseHandle(hFile);
	// �t�@�C��������r
	if (CompareFileTime(&m_SessionFileTime, &fileTime) == 0)
	{
		// �ύX�Ȃ��Ȃ�Ή������Ȃ�
		return FALSE;
	}

	TRY
	{
		if (_wfopen_s(&fp, strFilePath, _T("r")) != 0 || fp == NULL)
		{
			return FALSE;
		}

		m_SessionFileTime = fileTime;
		char	szline[2048];
		CString cbuf, strStationSession, strStatus, strStation, strSession;
		BOOL	bStation1 = TRUE;
		while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL)
		{
			cbuf = CString(szline);
			AfxExtractSubString(strStationSession, cbuf, 0, '=');
			AfxExtractSubString(strStatus, cbuf, 1, '=');

			AfxExtractSubString(strStation, strStationSession, 0, '_');
			AfxExtractSubString(strSession, strStationSession, 1, '_');
			strStatus.Replace(_T("\n"), _T(""));

			// 1�ǃ��[�h
			if (theApp.GetSelectStationMode() == eStation_1)
			{
				UINT nStation = theApp.GetSelectStation();
				// �ǂ̈�v�𒲂ׂ�
				if (strStation.CompareNoCase(CString(mStationSimpleString[nStation]).MakeLower()) == 0)
				{
					// ���ǂ̃Z�b�V������Ԃ��Z�b�g
					m_sessionInfo[nStation].SetSessionStatus(strSession, strStatus);
				}
			}
			// 4�ǃ��[�h
			else
			{
				for (int i = 0; i < eStation_MAX; i++)
				{
					// �ǂ̈�v�𒲂ׂ�
					if (strStation.CompareNoCase(CString(mStationSimpleString[i]).MakeLower()) == 0)
					{
						// ��v�ǂ̃Z�b�V������Ԃ��Z�b�g
						m_sessionInfo[i].SetSessionStatus(strSession, strStatus);
					}
				}
			}
		}
	} CATCH_ALL(e){
	}
	END_CATCH_ALL
		if (fp != NULL)
			fclose(fp);

	CWnd* pWnd = (CWnd*)theApp.GetWindowHandle(IDD_DLG_SESSIONSTATUS);
	if (pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
	{
		pWnd->PostMessage(eMessage_UpdateSessionStatus, 0, 0);
	}

	return TRUE;
}


/*============================================================================*/
/*! CSatelliteData

-# STAT_SDTP�ŕۑ����ꂽ���[�J���̃Z�b�V������ԃt�@�C�����폜����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSatelliteData::DeleteSessionStatusFile()
{
	CString strFilePath;
	strFilePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppWorkPath(), STAT_SDTP_FILENAME);

	(void)DeleteFile(strFilePath);
}


/*============================================================================*/
/*! CSatelliteData

-# RF�g���b�v�p��`�t�@�C���ǂݍ���

@param	�Ȃ�
@retval	1�F���� / -1�F���s

*/
/*============================================================================*/
void CSatelliteData::ReadRFTripFile()
{
	// 1�ǃ��[�h
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		UINT nStation = theApp.GetSelectStation();
		m_arrayRFTripData[nStation].SetStation(nStation);
		(void)m_arrayRFTripData[nStation].ReadFile();
	}
	// 4�ǃ��[�h
	else
	{
		for (UINT i = 0; i < eStation_MAX; i++)
		{
			m_arrayRFTripData[i].SetStation(i);
			(void)m_arrayRFTripData[i].ReadFile();
		}
	}
}


/*============================================================================*/
/*! CSatelliteData

-# �v���p�e�B�t�@�C���Ǎ�

@param	strSat : �q����
@param	stProp : �v���p�e�B�f�[�^�i�[�̈�
@retval	1�F���� / -1�F���s

*/
/*============================================================================*/
int  CSatelliteData::ReadPropertyFile(CString strSat, stOpeProp &stProp)
{
	CString dir = theApp.GetShareFilePath(eFileType_OperationProp_Folder, theApp.GetSelectStation());
	CString fname = dir + _T("Property.dat");

	TCHAR buf[MAX_PATH];

	//--------------------------
	// ���ʕ�
	//--------------------------

	GetPrivateProfileString(_T("COMMON"), _T("LEAP_YEAR"), _T("OFF"), buf, MAX_PATH, fname);
	if (CString(buf) == _T("OFF"))
		stProp.init.leapyear = 0;
	else
		stProp.init.leapyear = 1;
	GetPrivateProfileString(_T("COMMON"), _T("LEAP_SEC"), _T("OFF"), buf, MAX_PATH, fname);
	stProp.init.leapsec = _wtoi(buf);
	GetPrivateProfileString(_T("COMMON"), _T("GAIN"), _T("0"), buf, MAX_PATH, fname);
	stProp.init.gain = _wtof(buf);

	//----------------------------
	// �q���ŗL��
	//----------------------------

	GetPrivateProfileString(strSat, _T("EVENT_CALC"), _T("OFF"), buf, MAX_PATH, fname);
	if (CString(buf) == _T("OFF"))
		stProp.event_autocalc = 0;
	else
		stProp.event_autocalc = 1;
	GetPrivateProfileString(strSat, _T("ZENITH_IN"), _T("0"), buf, MAX_PATH, fname);
	stProp.even_sunset = _wtoi(buf);
	GetPrivateProfileString(strSat, _T("ZENITH_OUT"), _T("0"), buf, MAX_PATH, fname);
	stProp.event_sunrise = _wtoi(buf);
	GetPrivateProfileString(strSat, _T("MARGIN_UP_ON"), _T("0"), buf, MAX_PATH, fname);
	stProp.mg_ULon = _wtoi(buf);
	GetPrivateProfileString(strSat, _T("MARGIN_UP_OFF"), _T("0"), buf, MAX_PATH, fname);
	stProp.mg_ULoff = _wtoi(buf);
	GetPrivateProfileString(strSat, _T("MARGIN_DOWN"), _T("0"), buf, MAX_PATH, fname);
	stProp.mg_DL = _wtoi(buf);
	GetPrivateProfileString(strSat, _T("OFFSET_CMD_END"), _T("0"), buf, MAX_PATH, fname);
	stProp.of_CMD = _wtoi(buf);
	GetPrivateProfileString(strSat, _T("OFFSET_RNG_END"), _T("0"), buf, MAX_PATH, fname);
	stProp.of_RNG = _wtoi(buf);
	GetPrivateProfileString(strSat, _T("DEFAULT_CID"), _T("0"), buf, MAX_PATH, fname);
	sprintf_s(stProp.cid, 30, "%s", (LPCSTR)CStringA(buf));
	GetPrivateProfileString(strSat, _T("SAME_TIME_MOD"), _T("OFF"), buf, MAX_PATH, fname);
	if (CString(buf) == _T("OFF"))
		stProp.opespace = 0;
	else
		stProp.opespace = 1;
	GetPrivateProfileString(strSat, _T("EXPECT_LEVEL"), _T("FIXED"), buf, MAX_PATH, fname);
	if (CString(buf) == _T("FIXED"))
		stProp.init.autocalc = 1;
	else
		stProp.init.autocalc = 0;
	GetPrivateProfileString(strSat, _T("EXPECT_LEVEL_FIXED_S"), _T("0"), buf, MAX_PATH, fname);
	stProp.init.reclevels = _wtof(buf);
	GetPrivateProfileString(strSat, _T("EXPECT_LEVEL_FIXED_X"), _T("0"), buf, MAX_PATH, fname);
	stProp.init.reclevelx = _wtof(buf);
	GetPrivateProfileString(strSat, _T("EIRP"), _T("0"), buf, MAX_PATH, fname);
	stProp.init.eirp = _wtof(buf);
	GetPrivateProfileString(strSat, _T("SUBSARR_TLM"), _T("0"), buf, MAX_PATH, fname);
	stProp.init.tlmflag = true;
	stProp.init.tlm = _wtof(buf);
	GetPrivateProfileString(strSat, _T("SUBSARR_CMD"), _T("0"), buf, MAX_PATH, fname);
	stProp.init.cmdflag = true;
	stProp.init.cmd = _wtof(buf);

	return 1;
}

/*============================================================================*/
/*! CSatelliteData

-# �v�撠�[�o�͗p�e���|�����t�@�C���쐬

@param  fname :�v��t�@�C�����i�t���p�X�j���p�X��؂��"\"��z��
@retval ���[�p�e���|�����t�@�C����
*/
/*============================================================================*/
#define PLNOUT_EXT _T("vrxp")
CString CSatelliteData::CreatePlnPrintFile(CString fname)
{
	CString strRet = _T("");
	CString strOutFName;
	CString strDName;
	CString strFName;

	FILE *fpin;
	FILE *fpout;
	TCHAR sz_buff[512];
	TCHAR sz_work[256];
	TCHAR sz_work2[256];
	TCHAR sz_work3[256];
	TCHAR sz_work4[256];
	TCHAR sz_work5[256];
	TCHAR sz_work6[256];
	TCHAR sz_work7[256];
	TCHAR sz_work8[256];
	TCHAR sz_ele[10][100];
	long l_rc;
	char sz_line[512];
	int i;
	CString strLine;
	CString strEle;
	CString strEventName;

	vector<CString> headerList;
	vector<stPlnoutDetail> detailList;

	CString strSatellite;
	CString strOpeSDate;
	CString strOpeEDate;

	//---------------------------------------------
	// �t�@�C�����݃`�F�b�N
	if (PathFileExists(fname) == FALSE)
	{
		return strRet;
	}

	//---------------------------------------------
	// �v��t�@�C���Ǎ�
	CString mes;
	if (_wfopen_s(&fpin, fname, _T("r")) != 0)
	{
		mes.Format(_T("�v��t�@�C�����J�����Ƃ��ł��܂���B\n%s"), (LPCTSTR)fname);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return strRet;
	}

	BOOL bHeader = TRUE;
	BOOL bPreEvent = FALSE;
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fpin) != NULL)
	{
		stPlnoutDetail st;
		strLine = CServerFile::ConvertUnicode(sz_line);
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)strLine);

		if (strLine == _T("\n"))
		{
			if (bHeader == TRUE)
			{
				bHeader = FALSE;
			}
			continue;
		}

		if (sz_buff[0] == '#')
		{
			if (wcsncmp(sz_buff, SOS_HEADER_ID, wcslen(SOS_HEADER_ID)) == 0)
			{
				swscanf_s(sz_buff, _T("%s%s%s%s%s%s%s%s"),
					sz_work, static_cast<unsigned>(_countof(sz_work)),
					sz_work2, static_cast<unsigned>(_countof(sz_work2)),
					sz_work3, static_cast<unsigned>(_countof(sz_work3)),
					sz_work4, static_cast<unsigned>(_countof(sz_work4)),
					sz_work5, static_cast<unsigned>(_countof(sz_work5)),
					sz_work6, static_cast<unsigned>(_countof(sz_work6)),
					sz_work7, static_cast<unsigned>(_countof(sz_work7)),
					sz_work8, static_cast<unsigned>(_countof(sz_work8)));
			}

			l_rc = swscanf_s(&sz_buff[1], _T("%s%s%s%s%s%s%s%s%s%s"),
				sz_ele[0], static_cast<unsigned>(_countof(sz_ele[0])),
				sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])),
				sz_ele[2], static_cast<unsigned>(_countof(sz_ele[2])),
				sz_ele[3], static_cast<unsigned>(_countof(sz_ele[3])),
				sz_ele[4], static_cast<unsigned>(_countof(sz_ele[4])),
				sz_ele[5], static_cast<unsigned>(_countof(sz_ele[5])),
				sz_ele[6], static_cast<unsigned>(_countof(sz_ele[6])),
				sz_ele[7], static_cast<unsigned>(_countof(sz_ele[7])),
				sz_ele[8], static_cast<unsigned>(_countof(sz_ele[8])),
				sz_ele[9], static_cast<unsigned>(_countof(sz_ele[9])));

			// �w�b�_�[���擾
			if ((strLine.Find(_T(";")) == -1)
				&& (strLine.Find(_T("=")) >= 0)
				&& (bHeader == TRUE))
			{
				for (i = 0; i < l_rc; ++i)
				{
					strEle = CString(sz_ele[i]);

					if (strEle.Find(_T("P-START")) >= 0
						|| strEle.Find(_T("P-END")) >= 0
						|| strEle.Find(_T("R-START")) >= 0
						|| strEle.Find(_T("R-END")) >= 0)
					{
						CString prm;
						AfxExtractSubString(prm, strEle, 1, '=');
						if (prm.IsEmpty() == FALSE)
						{
							i++;
							CString tmp = CString(sz_ele[i]);
							strEle += _T(" ") + tmp;
						}
					}
					headerList.push_back(strEle);


					if (strEle.Find(_T("�q����")) >= 0)
					{
						// �q����
						AfxExtractSubString(strSatellite, strEle, 1, '=');
					}
					else if (strEle.Find(_T("P-START")) >= 0)
					{
						// �^�p�J�n�����擾
						AfxExtractSubString(strOpeSDate, strEle, 1, '=');
					}
					else if (strEle.Find(_T("P-END")) >= 0)
					{
						// �^�p�I�������擾
						AfxExtractSubString(strOpeEDate, strEle, 1, '=');
					}
				}
			}

			// �C�x���g���擾
			if (bHeader == FALSE)
			{
				if (strLine.Find(_T("#*")) >= 0)
				{
					bPreEvent = TRUE;
					continue;
				}

				if (bPreEvent == TRUE)
				{
					CString eTmp;
					if (strLine.Find(_T(";")) == -1)
					{
						eTmp = strLine.Mid(2, strLine.GetLength() - (2 + 1));
						if (strLine.Find(_T("���O")) >= 0)
						{
							int idx = eTmp.Find(_T("("));
							strEventName = eTmp.Mid(0, idx);
						}
						else
						{
							strEventName = eTmp;
						}
					}
					bPreEvent = FALSE;
				}
			}
		}
		else
		{
			if (bPreEvent == TRUE)
			{
				bPreEvent = FALSE;
			}

			// �C�x���g�擾
			CString tmp1, tmp2, tmp3;
			AfxExtractSubString(tmp1, strLine, 0, ' ');
			AfxExtractSubString(tmp2, strLine, 1, ' ');
			AfxExtractSubString(tmp3, strLine, 2, ' ');

			// ����
			CString strDate = tmp1 + _T(" ") + tmp2;
			st.strDate = strDate;
			st.tDate = CSatelliteData::StrTimeToCTime(strDate);

			// �C�x���g��
			st.strEventName = strEventName;

			// ����
			CString strCtrl, strPrm;
			AfxExtractSubString(strCtrl, tmp3, 0, ',');
			AfxExtractSubString(strPrm, tmp3, 1, ',');
			st.strCtrl = strCtrl;
			st.strPrm = strPrm;
			detailList.push_back(st);
		}
	}
	fclose(fpin);

	//---------------------------------------------
	// ���[�o�͗p�e���|�����t�@�C���쐬
	char szBuf[2046];
	strDName = fname.Left(fname.ReverseFind(_T('\\')));
	strFName = fname.Right(fname.GetLength() - fname.ReverseFind(_T('\\')) - 1);
	strOutFName = strDName + _T("\\") + strFName.Mid(0, strFName.Find('.')) + _T(".") + PLNOUT_EXT;

	if ((_wfopen_s(&fpout, strOutFName, _T("w")) != 0) || (fpout == NULL))
	{
		mes.Format(_T("�v�撠�[�p�t�@�C�����J�����Ƃ��ł��܂���B\n%s"), (LPCTSTR)strOutFName);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return strRet;
	}

	// �\�����o��
	sprintf_s(szBuf, "#%s\n", "HEADER");
	fputs(szBuf, fpout);
	sprintf_s(szBuf, "SATELLITE=%s\n", (LPCSTR)CStringA(strSatellite));
	fputs(szBuf, fpout);
	sprintf_s(szBuf, "P-START=%s\n", (LPCSTR)CStringA(strOpeSDate));
	fputs(szBuf, fpout);
	sprintf_s(szBuf, "P-END=%s\n", (LPCSTR)CStringA(strOpeEDate));
	fputs(szBuf, fpout);

	// �T�}���[���o��
	sprintf_s(szBuf, "\n#%s\n", "SUMMARY");
	fputs(szBuf, fpout);
	for (int i = 0; i < headerList.size(); ++i)
	{
		sprintf_s(szBuf, "%s\n", (LPCSTR)CStringA(headerList[i]));
		fputs(szBuf, fpout);
	}

	// �ڍ׏��o��

	// �\�[�g
	int size = (int)detailList.size();
	for (int i = 1; i < size; ++i)
	{
		for (int j = size - 1; j > i; --j)
		{
			stPlnoutDetail st1 = detailList[j];
			stPlnoutDetail st2 = detailList[j - 1];
			if (st1.tDate < st2.tDate)
			{
				detailList[j] = st2;
				detailList[j - 1] = st1;
			}
		}
	}

	sprintf_s(szBuf, "\n#%s\n", "DETAILS");
	fputs(szBuf, fpout);
	for (int i = 0; i < detailList.size(); ++i)
	{
		stPlnoutDetail &st = detailList[i];
		sprintf_s(szBuf, "%s,\"%s\",%s,%s\n", (LPCSTR)CStringA(st.strDate), (LPCSTR)CStringA(st.strEventName), (LPCSTR)CStringA(st.strCtrl), (LPCSTR)CStringA(st.strPrm));
		fputs(szBuf, fpout);
	}

	fclose(fpout);

	return strOutFName;
}


/*============================================================================*/
/*! CSatelliteData

-# ���[�o��

@param  fname :�v��t�@�C�����i�t���p�X�j���p�X��؂��"\"��z��
@retval ���[�p�e���|�����t�@�C����
*/
/*============================================================================*/
void CSatelliteData::ExcelOutPut(HWND hwnd, CString param)
{
	CString	strExePath, strParam;
	strExePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppPath(), (LPCTSTR)CString(CYOHYO_APPNAME));
	strParam = param;
	::ShellExecute(hwnd, _T(""), strExePath, strParam, NULL, SW_NORMAL);
}

/*============================================================================*/
/*! CSatelliteData

-# ���[�o�̓v���Z�X

@param  hwnd�F���g�p
@param	param�F���[�p�����[�^
@param	bWaitComplete�FTRUE:�I����҂i�f�t�H���g�j / FALSE:�I����҂��Ȃ�
@retval ���[���s����(0�F�����A���̑��F���s�j
*/
/*============================================================================*/
int CSatelliteData::ExcelOutPutProc(HWND hwnd, CString param, BOOL bWaitComplete /*= TRUE*/)
{
	CWaitCursor wait;

	(void)hwnd;

	int ret = -1;
	CString	strExePath;
	strExePath.Format(_T("%s%s %s"), (LPCTSTR)theApp.GetAppPath(), (LPCTSTR)CString(CYOHYO_APPNAME), (LPCTSTR)param);
	TRACE("%s\n", CStringA(strExePath));

	// �V�����v���Z�X�p�̍\���̂��N������
	STARTUPINFO    stStartUpInfo;
	// �����o�[�̑����͗\�񂳂�Ă��邩���������̂ŁA���ׂẴ����o�[��
	// 0 �ɐݒ肵�Ă���A�K�v�ȃ����o�[���������Z�b�g����
	memset(&stStartUpInfo, 0, sizeof(STARTUPINFO));
	stStartUpInfo.cb = sizeof(STARTUPINFO);			// �\���̂̃T�C�Yls
	stStartUpInfo.dwFlags = STARTF_USESHOWWINDOW;	// wShowWindow �����o�[���g��
	stStartUpInfo.wShowWindow = SW_HIDE;			// ��ʂ͔�\��

	// �V�����v���Z�X��ŏ����󂯎��v���Z�X�\����
	PROCESS_INFORMATION stProcessInfo;

	// �V�����v���Z�X�����ƁA�v���Z�X�̓v���C�}���X���b�h�����A
	// �A�v���P�[�V�������N������
	if (CreateProcess(NULL, (LPTSTR)strExePath.GetBuffer(), NULL, NULL, FALSE,
		/*CREATE_SUSPENDED*/NORMAL_PRIORITY_CLASS, NULL,
		NULL, &stStartUpInfo, &stProcessInfo) == 0)
	{
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: CreateProcess(%s) failed, gle(%d)"), __FILEW__, __LINE__, (LPCTSTR)CString(CYOHYO_APPNAME), GetLastError());
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ExcelOutPutProc"), _T("error"), strDebug, nLogEx::error);
		}
		strExePath.ReleaseBuffer();
		return -1;
	}
	strExePath.ReleaseBuffer();

	// ���[�I����҂ꍇ
	if (bWaitComplete)
	{
		// ���[�I���܂őҋ@
		if (WaitForSingleObject(stProcessInfo.hProcess, CYOHO_WAITTIME) == WAIT_OBJECT_0)
		{
			DWORD rc;
			GetExitCodeProcess(stProcessInfo.hProcess, &rc);
			{
				CString strDebug;
				strDebug.Format(_T("%s(%d) :: CreateProcess(%s), terminate(%lu)"), __FILEW__, __LINE__, (LPCTSTR)CString(CYOHYO_APPNAME), rc);
				CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ExcelOutPutProc"), _T("debug"), strDebug, nLogEx::debug);
			}

			ret = (int)rc;
		}
		else
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: CreateProcess(%s), wait failed"), __FILEW__, __LINE__, (LPCTSTR)CString(CYOHYO_APPNAME));
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ExcelOutPutProc"), _T("debug"), strDebug, nLogEx::debug);
		}
	}
	// ���[�I����҂��Ȃ��ꍇ
	else
	{
		ret = 0;
	}

	CloseHandle(stProcessInfo.hThread);
	CloseHandle(stProcessInfo.hProcess);

	return ret;
}


/*============================================================================*/
/*! CTrendData

-# �\��l����i�A�b�v�����N�����A�_�E�������N�����A�QWAY�����j���擾����

@param	t		�w�莞��	tye
@param	type	0=Uplink 1=Downlink 2;2way
@retval

*/
/*============================================================================*/
double CTrendData::GetUpDownLinkRange(time_t t, int type)
{
	char ca_TIME[26];			/* �����i�t���t�H�[�}�b�g�j		  */
	long m_gla_prd_cont = 0;
	m_UpDownLinkRangeMap.clear();
	if (m_UpDownLinkRangeMap.size() == 0){
		// map�̍쐬
		/* <<XUP>> �\��l�J�E���^�N���A */
		m_gla_prd_cont = 0;
		for (m_gla_prd_cont = 0; m_gla_prd_cont<m_PredItemCount; m_gla_prd_cont++) {

			/* <<XUP>> �\��l�e�[�u���̃X�g�b�p�[�����o�����Ȃ�΁A�I�� */
			if (strcmp(m_gst_predata.predata[m_gla_prd_cont].ca_predict_s_tx_freq, END_OF_TABLE) == 0) {
				break;
			}

			// �\��l�f�[�^���q���f�[�^�̃e���|�����ɒǉ�����
			stServerItemData data;
			memset(&data, 0, sizeof(stServerItemData));
			data.kind = 0;

			/* <<XUP>> �\��l�����̌^�ϊ��iBCD��ASCII�j */
			memset(ca_TIME, (char)NULL, sizeof(ca_TIME));
			bcd7_to_asc19_time(m_gst_predata.predata[m_gla_prd_cont].uca_time, (unsigned char*)ca_TIME);
			CTime ctime = CTime((time_t)CSatelliteData::ub_asc_to_dtime(ca_TIME));

			SYSTEMTIME systime;
			ctime.GetAsSystemTime(systime);
			CCommBase::ConvertTime(&systime, &data.systime);

			// RNG�`�����j�^�p�ɁA�\��l�����Ɖ��������̃}�b�v��p�ӂ���
			time_t tRngTime = ctime.GetTime();
			switch (type){
			case	0/*�A�b�v�����N����*/:
				m_UpDownLinkRangeMap[tRngTime] = m_gst_predata.predata[m_gla_prd_cont].d_predict_uplink_range;
				break;
			case	1/*�_�E�������N����*/:
				m_UpDownLinkRangeMap[tRngTime] = m_gst_predata.predata[m_gla_prd_cont].d_predict_downlink_range;
				break;
			case	2/*�QWay����*/:
				m_UpDownLinkRangeMap[tRngTime] = m_gst_predata.predata[m_gla_prd_cont].d_predict_2way_rng;
				break;
			}

			/* <<2013-TURBO>> 1�b�X�V24���ԗ\��l�ɑΉ����AEOT���Ȃ��ꍇ�̏I�������ǉ� */
			if (m_gla_prd_cont == MAX_PRED_SIZE) {
				break;
			}
		}
	}

	auto itr = m_UpDownLinkRangeMap.lower_bound(t);
	// �͈͂ɂ���ꍇ
	if (itr != m_UpDownLinkRangeMap.end())
	{
		if ((*itr).first == t)
		{
			// �\��l�}�b�v�Ɏ���������ꍇ�́A�}�b�v�̒l��Ԃ�
			return m_UpDownLinkRangeMap[t];
		}
		else if (itr == m_UpDownLinkRangeMap.begin())
		{
			// �P�Ԗڂ��O�̂��߁A�P�ԖڂƂQ�Ԗڂ̗\��l�ŕ��
			auto itr1 = itr;
			itr++;
			auto itr2 = itr;
			return GetLinearInterpolationRange((*itr1).first, (*itr1).second, (*itr2).first, (*itr2).second, t);
		}
		else
		{
			// �ЂƂO�̗\��l�ƌ��������\��l�ŕ��
			auto itr2 = itr;
			itr--;
			auto itr1 = itr;
			return GetLinearInterpolationRange((*itr1).first, (*itr1).second, (*itr2).first, (*itr2).second, t);
		}
	}
	// �͈͂ɖ����ꍇ
	else if (m_UpDownLinkRangeMap.size() >= 2)
	{
		// �Ō���P�O�̗\��l�ƍŌ�̗\��l�ŕ��
		itr--;
		auto itr2 = itr;
		itr--;
		auto itr1 = itr;
		return GetLinearInterpolationRange((*itr1).first, (*itr1).second, (*itr2).first, (*itr2).second, t);
	}

	return -1.0;
}

/*============================================================================*/
/*! CTrendData

-# �q���܂ł̉��������𒼐���Ԃ��ă}�b�v�ɒǉ�����

@param	t1
@param	rng1
@param	t2
@param	rng2
@retval	�Ȃ�

*/
/*============================================================================*/
double CTrendData::GetLinearInterpolationRange(time_t t1, double dRng1, time_t t2, double dRng2, time_t t)
{
	double fval = m_UpDownLinkRangeMap[t1];
	fval = m_UpDownLinkRangeMap[t2];
	if ((t2 - t1) == 0)
		return m_UpDownLinkRangeMap[t1] + (t - t1);
	if ((m_UpDownLinkRangeMap[t2] - m_UpDownLinkRangeMap[t1]) == 0)
		return m_UpDownLinkRangeMap[t1] + (t - t1);
	return m_UpDownLinkRangeMap[t1] + (t - t1) / (t2 - t1) * (m_UpDownLinkRangeMap[t2] - m_UpDownLinkRangeMap[t1]);
}

/*============================================================================*/
/*! CTrendData

-# �\��l����i�A�b�v�����N�����ω����A�_�E�������N�����ω����A�QWAY�����ω����j���擾����

@param	t		�w�莞��	tye
@param	type	0=Uplink 1=Downlink 2;2way
@retval

*/
/*============================================================================*/
double CTrendData::GetUpDownLinkRate(time_t t, int type)
{
	char ca_TIME[26];			/* �����i�t���t�H�[�}�b�g�j		  */
	long m_gla_prd_cont = 0;
	m_UpDownLinkRateMap.clear();
	if (m_UpDownLinkRateMap.size() == 0){
		// map�̍쐬
		/* <<XUP>> �\��l�J�E���^�N���A */
		m_gla_prd_cont = 0;
		for (m_gla_prd_cont = 0; m_gla_prd_cont<m_PredItemCount; m_gla_prd_cont++) {

			/* <<XUP>> �\��l�e�[�u���̃X�g�b�p�[�����o�����Ȃ�΁A�I�� */
			if (strcmp(m_gst_predata.predata[m_gla_prd_cont].ca_predict_s_tx_freq, END_OF_TABLE) == 0) {
				break;
			}

			// �\��l�f�[�^���q���f�[�^�̃e���|�����ɒǉ�����
			stServerItemData data;
			memset(&data, 0, sizeof(stServerItemData));
			data.kind = 0;

			/* <<XUP>> �\��l�����̌^�ϊ��iBCD��ASCII�j */
			memset(ca_TIME, (char)NULL, sizeof(ca_TIME));
			bcd7_to_asc19_time(m_gst_predata.predata[m_gla_prd_cont].uca_time, (unsigned char*)ca_TIME);
			CTime ctime = CTime((time_t)CSatelliteData::ub_asc_to_dtime(ca_TIME));

			SYSTEMTIME systime;
			ctime.GetAsSystemTime(systime);
			CCommBase::ConvertTime(&systime, &data.systime);

			// RNG�`�����j�^�p�ɁA�\��l�����Ɖ��������̃}�b�v��p�ӂ���
			time_t tRngTime = ctime.GetTime();
			switch (type){
			case	0/*�A�b�v�����N����*/:
				m_UpDownLinkRateMap[tRngTime] = m_gst_predata.predata[m_gla_prd_cont].d_predict_uplink_range_rate;
				break;
			case	1/*�_�E�������N����*/:
				m_UpDownLinkRateMap[tRngTime] = m_gst_predata.predata[m_gla_prd_cont].d_predict_downlink_range_rate;
				break;
			case	2/*�QWay����*/:
				m_UpDownLinkRateMap[tRngTime] = m_gst_predata.predata[m_gla_prd_cont].d_predict_2way_range_rate;
				break;
			case	3/*�RWay����*/:
				m_UpDownLinkRateMap[tRngTime] = m_gst_predata.predata[m_gla_prd_cont].d_predict_3way_range_rate;
				break;
			}

			/* <<2013-TURBO>> 1�b�X�V24���ԗ\��l�ɑΉ����AEOT���Ȃ��ꍇ�̏I�������ǉ� */
			if (m_gla_prd_cont == MAX_PRED_SIZE) {
				break;
			}
		}
	}

	auto itr = m_UpDownLinkRateMap.lower_bound(t);
	// �͈͂ɂ���ꍇ
	if (itr != m_UpDownLinkRateMap.end())
	{
		if ((*itr).first == t)
		{
			// �\��l�}�b�v�Ɏ���������ꍇ�́A�}�b�v�̒l��Ԃ�
			return m_UpDownLinkRateMap[t];
		}
		else if (itr == m_UpDownLinkRateMap.begin())
		{
			// �P�Ԗڂ��O�̂��߁A�P�ԖڂƂQ�Ԗڂ̗\��l�ŕ��
			auto itr1 = itr;
			itr++;
			auto itr2 = itr;
			return GetLinearInterpolationRate((*itr1).first, (*itr1).second, (*itr2).first, (*itr2).second, t);
		}
		else
		{
			// �ЂƂO�̗\��l�ƌ��������\��l�ŕ��
			auto itr2 = itr;
			itr--;
			auto itr1 = itr;
			return GetLinearInterpolationRate((*itr1).first, (*itr1).second, (*itr2).first, (*itr2).second, t);
		}
	}
	// �͈͂ɖ����ꍇ
	else if (m_UpDownLinkRateMap.size() >= 2)
	{
		// �Ō���P�O�̗\��l�ƍŌ�̗\��l�ŕ��
		itr--;
		auto itr2 = itr;
		itr--;
		auto itr1 = itr;
		return GetLinearInterpolationRate((*itr1).first, (*itr1).second, (*itr2).first, (*itr2).second, t);
	}

	return -1.0;
}

/*============================================================================*/
/*! CTrendData

-# �q���܂ł̉��������𒼐���Ԃ��ă}�b�v�ɒǉ�����

@param	t1
@param	rng1
@param	t2
@param	rng2
@retval	�Ȃ�

*/
/*============================================================================*/
double CTrendData::GetLinearInterpolationRate(time_t t1, double dRng1, time_t t2, double dRng2, time_t t)
{
	double fval = m_UpDownLinkRateMap[t1];
	fval = m_UpDownLinkRateMap[t2];
	if ((t2 - t1) == 0)
		return m_UpDownLinkRateMap[t1] + (t - t1);
	if ((m_UpDownLinkRateMap[t2] - m_UpDownLinkRateMap[t1]) == 0)
		return m_UpDownLinkRateMap[t1] + (t - t1);
	return m_UpDownLinkRateMap[t1] + (t - t1) / (t2 - t1) * (m_UpDownLinkRateMap[t2] - m_UpDownLinkRateMap[t1]);
}
