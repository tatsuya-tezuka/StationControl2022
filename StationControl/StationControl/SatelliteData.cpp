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
// RNG伝搬ダミーデータ選択
// 臼田64m 0:S/S, 1:X/X（旧系：従来）, 2:X/X（旧系：再生）, 3:X/X（新系：従来）, 4:X/X（新系；再生）
// 臼田54m 3:X/X or X/Ka（従来）, 4:X/X or X/Ka（再生）
// 内之浦34m 0:S/S, 3:X/X（新系：従来）, 4:X/X（新系；再生）
// 内之浦20m 0:S/S, 
UINT mRngTransModeDummySelect = 4;
#endif

/* ------------------------------------------------------------------------------------ */
/* RNG伝搬
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

-# ダミーデータ生成

@param	なし
@retval	なし

*/
/*============================================================================*/
void CRangePropagation::CreateDummy()
{
#ifdef _DEBUG_RNGPROPDUMMY
	time_t t = m_dataTime.GetTime();
	if (t % 1 == 0)
	{
		// 再生型以外
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
		// 再生型
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

-# ステータス初期化

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

-# ステータス初期化(54m)

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

-# ステップの更新

@param なし
@retval	なし

*/
/*============================================================================*/
void CRangePropagation::UpdateStep()
{
	BOOL bStep = FALSE;

	while (TRUE)
	{
		if (m_nStation == eStation_Usuda642)
		{
			// 54m局
			bStep = UpdateStepProc54();
		}
		else
		{
			// その他の局
			bStep = UpdateStepProc();
		}

		if (bStep == FALSE)
		{
			// ステップが変らない場合はループを抜ける
			break;
		}
	}
}


/*============================================================================*/
/*! CRangePropagation

-# ステップの更新

@param なし
@retval	TRUE:ステップが更新された / FALSE:ステップが変わらない

*/
/*============================================================================*/
BOOL CRangePropagation::UpdateStepProc()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	CString monitorString;
	CString str;

	UINT nOldStep = m_nStep;

	// 時刻取得
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
//			// step2へ
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
				// step2へ
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
				// モード設定と、step3へ
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
		// START検知
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
		// STOP検知
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

		// ステータス切り替わり時
		str = GetKanshiString(m_nStep);
		// 従来型の場合
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
					// レンジトーン送出信号の検出
					DetectRngTone();
				}
			}
		}
		// 再生型の場合
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
				// レンジトーン送出信号の検出
				DetectRngTone();
			}
		}

		// レンジトーン移動
		MoveRngTone();
		break;

	case eRngStep_6:
		// レンジトーン移動
		MoveRngTone();
		CGraphMutex::Lock(eRngProp);
		// レンジトーンがすべて帰ってくるまで待つ
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
		// ステップが変らない
		return FALSE;
	}

	// ステップが変った場合
	return TRUE;
}

/*============================================================================*/
/*! CRangePropagation

-# ステップの更新(54m局)

@param なし
@retval	TRUE:ステップが更新された / FALSE:ステップが変わらない

*/
/*============================================================================*/
BOOL CRangePropagation::UpdateStepProc54()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	CString monitorString;
	CString str;

	UINT nOldStep = m_nStep;

	// 時刻取得
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
		//			// step2へ
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
				// step2へ
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
				// モード設定と、step3へ
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
		// START検知
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
		// STOP検知
		if (str.Find(mRngStep[m_nStation][m_nTransMode][m_nStep - 1].value2) == 0)
		{
			// 54mは"STOP"文字列の先頭からの部分一致したらSTOP検知
			m_eRngStatus = eStop;
			m_nStep = eRngStep_6;

			{
				CString strDebug;
				strDebug.Format(_T("Step to (%u) with Kanshi(%s), RngStatus(%u)"), m_nStep, (LPCTSTR)str, m_eRngStatus);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("UpdateStepProc54"), _T("debug"), strDebug, nLogEx::debug);
			}
			break;
		}

		// ステータス切り替わり時
		str = GetKanshiString(m_nStep);
		// 従来型の場合
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
					// レンジトーン送出信号の検出
					DetectRngTone();
				}
			}
		}
		// 再生型の場合
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
				// レンジトーン送出信号の検出
				DetectRngTone();
			}
		}

		// レンジトーン移動
		MoveRngTone();
		break;

	case eRngStep_6:
		// レンジトーン移動
		MoveRngTone();
		CGraphMutex::Lock(eRngProp);
		// レンジトーンがすべて帰ってくるまで待つ
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
		// ステップが変らない
		return FALSE;
	}

	// ステップが変った場合
	return TRUE;
}

/*============================================================================*/
/*! CRangePropagation

-# レンジトーン移動

@param	なし
@retval	なし

*/
/*============================================================================*/
void CRangePropagation::MoveRngTone()
{
	CGraphMutex::Lock(eRngProp);

	// レンジトーン移動計算
	deque<CRangePropagation::structPropData>::iterator itr;
	time_t tMove;

//#if 0
//	// 往復距離と時間取得
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

	// アンテナ->衛星間の横位置更新
	for (itr = m_queRngSnd.begin(); itr != m_queRngSnd.end(); itr++)
	{
		// レンジトーンが移動した時間
		tMove = m_dataTime.GetTime() - (*itr).tStart;
		// 経過した位置計算
		(*itr).pos = (((double)tMove) * V_C);

		{
			CString strDebug;
			strDebug.Format(_T("m_queRngSnd[%u], tMove(%I64d), RTT(%f), pos(%f)"), (*itr).count, tMove, (*itr).rtt, (*itr).pos);
			CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("MoveRngTone"), _T("debug"), strDebug, nLogEx::debug);
		}
	}

	for (itr = m_queRngSnd.begin(); itr != m_queRngSnd.end(); itr++)
	{
		// 衛星に到達した場合
		if ((*itr).pos >= (*itr).d2WayRange / 2)
		{
			{
				CString strDebug;
				strDebug.Format(_T("Arrive Satellite. m_queRngSnd[%u], tStart(%I64d), offset(%f), pos(%f)"), (*itr).count, (*itr).tStart, (*itr).offset, (*itr).pos);
				CLogTraceEx::Write(_T(""), _T("CRangePropagation"), _T("MoveRngTone"), _T("debug"), strDebug, nLogEx::debug);
			}
			// アンテナ->衛星間キューから削除し、アンテナ<-衛星キューに追加
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

	// アンテナ<-衛星間の横位置更新
	for (itr = m_queRngRcv.begin(); itr != m_queRngRcv.end(); itr++)
	{
		// レンジトーンが移動した時間
		tMove = m_dataTime.GetTime() - (*itr).tStart;
		// 経過した位置計算
		(*itr).pos = (*itr).d2WayRange / 2 - (((double)tMove) * V_C) - (*itr).offset;
	}
	for (itr = m_queRngRcv.begin(); itr != m_queRngRcv.end(); itr++)
	{
		// アンテナに到達した場合
		if ((*itr).pos <= 0.0)
		{
			// アンテナ<-衛星間キューから削除
			m_queRngRcv.erase(itr);
			// キューが空になった場合
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
				// 何もしない
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

-# 監視データをdoubleで取得

@param	なし
@retval	監視データ

*/
/*============================================================================*/
double CRangePropagation::GetKanshiDouble()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
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

-# 監視データをCStringで取得

@param	step：ステップ数
@retval	監視データ

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

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
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

-# レンジトーン検知

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

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
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

	// 往復距離と時間取得
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

	// 従来型でカウント数が受信範囲内か、再生型の場合
	if ((m_nGeneMode == eRngGeneration_Past && m_nSndCount < m_nRngNum) || (m_nGeneMode == eRngGeneration_Regene))
	{
		// カウント値生成
		if (m_queRngAll.size() >= m_nMaxRngNum)
		{
			// 全リストが最大値を越える場合、リスト先頭のカウント値を次のカウント値にして、リスト先頭を削除
			m_nSndCount = m_queRngAll.front().count;
			m_queRngAll.pop_front();
//			TRACE("###### DetectRngTone: pop_front: m_queRngAll size:%04d, m_nSndCount:%04u\n", (int)m_queRngAll.size(), m_nSndCount);

			if (m_queRngRcv.empty() == false)
			{
				// 復路リストにデータがあれば復路先頭削除
				m_queRngRcv.pop_front();
			}
			else
			{
				// 復路リストにデータがなければ往路先頭削除
				m_queRngSnd.pop_front();
			}
		}
		else
		{
			// カウント値がまだ範囲内の場合
			m_nSndCount++;
//			TRACE("###### DetectRngTone: normal count up: m_queRngAll size:%04d, m_nSndCount:%04u\n", (int)m_queRngAll.size(), m_nSndCount);
		}

		structPropData stPropData;
		stPropData.count = m_nSndCount;
		// 開始時間
		stPropData.tStart = m_dataTime.GetTime();
		// オフセット位置
		stPropData.offset = 0;
		// 位置
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

-# 衛星からの往復距離取得

@param	なし
@retval 衛星からの往復距離

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

-# レンジトーン

@param	なし
@retval	なし

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
/* サマリプロパティ
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
/*! サマリプロパティ

-# グラフビュー名リストの取得

@param	listViewName：ビュー名リスト
@retval	TRUE:取得成功 / FALSE:取得失敗

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
/* RFトリップ角定義ファイル
/* ------------------------------------------------------------------------------------ */
CRFTripAngleFile::CRFTripAngleFile()
{
}

CRFTripAngleFile::~CRFTripAngleFile()
{
	// リスト、マップクリア
	m_listPowerInfo.clear();
	m_mapRftripGene.clear();
	m_mapRftripMod.clear();
}


//#if 0
///*============================================================================*/
///*! CRFTripAngleFile
//
//-# RFトリップ角定義ファイルパスを取得する
//
//@param	strPwr：送信電力値文字列
//@param	nStation：局番号
//@retval	RFトリップ角定義ファイルパス
//
//*/
///*============================================================================*/
//CString CRFTripAngleFile::GetRfTripFilePath(CString strPwr, UINT nStation)
//{
//	UINT nRfTrip_File = eFileType_RFTrip_25w_File;
//
//	// 送信電力取得
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
//-# RFトリップ角定義ファイルパスを取得する
//
//@param	nStation：局番号
//@retval	RFトリップ角定義ファイルパス
//
//*/
///*============================================================================*/
//CString CRFTripAngleFile::GetRfTripFilePath(UINT nStation)
//{
//	// 送信電力からRFトリップファイルを取得
//	//	int nRfPower = 0;
//	CString strRfPwrSig;
//	CString strHpa;
//	CString strPwr;
//
//	// 局別にRFトリップファイル選択
//	switch (nStation)
//	{
//	case eStation_Usuda64:
//	case eStation_Usuda642:
//		// HPAを調べる
//
//		if (strHpa.Compare(HPA1_RES))
//		{
//			// HPA-1の場合
//			strRfPwrSig = XTX_HPA_RF_PWR_SIG;
//		}
//		else if (strHpa.Compare(HPA2_RES))
//		{
//			// HPA-2の場合
//			strRfPwrSig = XTX_HPA2_RF_PWR_SIG;
//		}
//
//		break;
//
//	case eStation_Uchinoura34:
//		// S帯の場合
//		{
//			strRfPwrSig = STX_RF_POWER_SEL;
//		}
//		// X帯の場合
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
//	// 送信電力取得
//
//
//	// 送信電力からファイルパスの取得
//	return GetRfTripFilePath(strPwr, nStation);
//}
//#endif

/*============================================================================*/
/*! CRFTripAngleFile

-# カレントのRFトリップファイルを読み込む

@param	なし
@retval	TRUE：読み込み成功 / FALSE：読み込み失敗

*/
/*============================================================================*/
BOOL CRFTripAngleFile::ReadFile()
{
#ifndef _ENGVALCNV_TREND
	CString strSatellite = _T("HAYABUSA2");
#else
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	// カレントの衛星名を監視から取得
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

	// 衛星名が既にセットされていて、かつ衛星名が同じ場合は何もしない
	if (m_strSatellite.IsEmpty() == FALSE && m_strSatellite.CompareNoCase(strSatellite) == 0)
	{
		return FALSE;
	}

	// 衛星名をセット
	m_strSatellite = strSatellite;

	return ReadFile(strSatellite);
}

/*============================================================================*/
/*! CRFTripAngleFile

-# 指定衛星名のRFトリップファイルを読み込む

@param	strSatellite：衛星名
@retval	TRUE：読み込み成功 / FALSE：読み込み失敗

*/
/*===========================================================================*/
BOOL CRFTripAngleFile::ReadFile(CString& strSatellite)
{
	// リスト、マップクリア
	m_listPowerInfo.clear();
	m_mapRftripGene.clear();
	m_mapRftripMod.clear();

	// 衛星名からRFトリップ角定義ファイルパスを作成
	CString strRfTripFolder = theApp.GetShareFilePath(eFileType_RFTrip_Folder, m_nStation);
	CString strRFTripFileName;
	strRFTripFileName.Format(RFTRIP_FILENAME, (LPCTSTR)(strSatellite.MakeLower()));
	CString strRFTripFilePath;
	strRFTripFilePath.Format(_T("%s%s"), (LPCTSTR)strRfTripFolder, (LPCTSTR)strRFTripFileName);

	TCHAR buf[MAX_PATH];

	// INFOセクション
	// 送信電力取得用監視名を取得
	if (GetPrivateProfileString(RFTRIP_SECTION_INFO, RFTRIP_KEY_KANSHI_POWER, _T(""), buf, MAX_PATH, strRFTripFilePath) == 0)
		return FALSE;
	(void)CStringOprTool::TrimComment(m_strKanshiPower, _T(';'), CString(buf));
	// 変調形式取得用監視名を取得
	if (GetPrivateProfileString(RFTRIP_SECTION_INFO, RFTRIP_KEY_KANSHI_MOD, _T(""), buf, MAX_PATH, strRFTripFilePath) == 0)
		return FALSE;
	(void)CStringOprTool::TrimComment(m_strKanshiMod, _T(';'), CString(buf));
	// 送信電力ステータスを取得
	GetPrivateProfileString(RFTRIP_SECTION_INFO, RFTRIP_KEY_MOD_SECTION_DEF, _T(""), buf, MAX_PATH, strRFTripFilePath);
	(void)CStringOprTool::TrimComment(m_strModSectionDef, _T(';'), CString(buf));
	// 送信電力(STSNAME_POWER)で取得したステータスを取得
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
			// 送信電力リストにステータス追加
			m_listPowerInfo.push_back(strPowerStatus);
			vector<double> listRfTripGene(360);
			vector<double> listRfTripMod(360);
			m_mapRftripGene[strPowerStatus] = listRfTripGene;
			m_mapRftripMod[strPowerStatus] = listRfTripMod;
		}
	}

	CString strKey;

	// 変調形式 「通常」セクション
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

			// 送信電力ステータス別のRFトリップ汎用マップにリスト追加
			CString& strPowerInfo = m_listPowerInfo[i];
			vector<double>& list = m_mapRftripGene[strPowerInfo];
			list[az] = _ttof(strTrip);
		}
	}

	// 変調形式 「指定」セクション
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

			// 送信電力ステータス別のRFトリップ指定マップにリスト追加
			CString& strPowerInfo = m_listPowerInfo[i];
			vector<double>& list = m_mapRftripMod[strPowerInfo];
			list[az] = _ttof(strTrip);
		}
	}

	return TRUE;
}


/*============================================================================*/
/*! CRFTripAngleFile

-# 指定の局、指定衛星名のRFトリップファイル読み込んで、入力の送信電力値、変調形式値に応じたAZ, ELリストを返す

@param	nStation：局番号[入力]
@param	strSatellite：衛星名[入力]
@param	strPower：送信電力取得用監視名[入力]
@param	strMod：変調形式取得用監視名[入力]
@param	listAzEl：AZ, ELのリスト[出力]
@retval	TRUE：読み込み成功 / FALSE：読み込み失敗

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

-# 監視の送信電力、変調形式に応じたAZ, ELリストを返す

@param	listAzEl：AZ, ELのリスト出力
@retval	TRUE：リストあり / FALSE：リストなし

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
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	// 送信電力取得
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

	// 変調形式判定が必要な場合
	if (m_strKanshiMod.Compare(_T("*")) != 0)
	{
		// 変調形式取得
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

-# 監視の送信電力、変調形式に応じたAZ, ELリストを返す

@param	strPower：送信電力取得用監視名[入力]
@param	strMod：変調形式取得用監視名[入力]
@param	listAzEl：AZ, ELのリスト[出力]
@retval	TRUE：リストあり / FALSE：リストなし

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
		// 汎用の場合
		if (m_mapRftripGene.count(strPowerValue) > 0)
		{
			listAzEl = m_mapRftripGene[strPowerValue];
			bRet = TRUE;
		}

	}
	else
	{
		// 変調形式の場合
		if (m_mapRftripMod.count(strPowerValue) > 0)
		{
			listAzEl = m_mapRftripMod[strPowerValue];
			bRet = TRUE;
		}
	}

	return bRet;
}


/* ------------------------------------------------------------------------------------ */
/* 印刷ユーティリティ
/* ------------------------------------------------------------------------------------ */
CPrintUtility::CPrintUtility()
{
}

CPrintUtility::~CPrintUtility()
{
}

/*============================================================================*/
/*! CPrintUtility

-# SJISのファイルを印刷する

@param	filepath：ファイルパス
@retval	なし

*/
/*============================================================================*/
BOOL CPrintUtility::PrintSjisFile(CString& filepath, CString title)
{
	BOOL ret = FALSE;
#ifdef _NOTEPAD
	CString strCmd;
	//	strCmd.Format(_T("notepad %s"), filepath);
	strCmd.Format(_T("notepad /p %s"), filepath);

	// 新しいプロセス用の構造体を起動する
	STARTUPINFO    stStartUpInfo;
	// メンバーの多くは予約されているか無視されるので、すべてのメンバーを
	// 0 に設定してから、必要なメンバーだけをリセットする
	memset(&stStartUpInfo, 0, sizeof(STARTUPINFO));
	stStartUpInfo.cb = sizeof(STARTUPINFO);			// 構造体のサイズ
	stStartUpInfo.dwFlags = STARTF_USESHOWWINDOW;	// wShowWindow メンバーを使う
	//	stStartUpInfo.wShowWindow = SW_SHOWDEFAULT;		// デフォルトの値
	stStartUpInfo.wShowWindow = SW_HIDE;			// 画面は非表示

	// 新しいプロセス上で情報を受け取るプロセス構造体
	PROCESS_INFORMATION stProcessInfo;

	// 新しいプロセスを作ると、プロセスはプライマリスレッドを作り、
	// アプリケーションを起動する
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

	// 印刷終了まで待機
	if (WaitForSingleObject(stProcessInfo.hProcess, INFINITE) != WAIT_FAILED)
	{
		ret = TRUE;
	}

	CloseHandle(stProcessInfo.hThread);
	CloseHandle(stProcessInfo.hProcess);
#else
	// 自前印刷
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
		title = _T("詳細表示");
	}
#else
	CString title = _T("詳細表示");
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
	// 自前印刷
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
		title = _T("詳細表示");
	}
#else
	CString title = _T("詳細表示");
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

-# UTF8のファイルをSJISに変換後に印刷する

@param	filepath：ファイルパス
@retval	なし

*/
/*============================================================================*/
BOOL CPrintUtility::PrintUtf8File(CString& filepath, CString title)
{
	BOOL ret;

	// WORKフォルダを保存先にする
	CString strFileName = filepath.Right(filepath.GetLength() - filepath.ReverseFind(_T('\\')) - 1);
	CString strWorkFilePath;
	strWorkFilePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppWorkPath(), (LPCTSTR)strFileName);

	// WORKフォルダにSJISファイル保存する
	ret = CFileUtility::ExchangeFileUtf8toSjis(filepath, strWorkFilePath);

	if (ret)
	{
		// WORKにファイルが保存できたら印刷する
		ret = PrintSjisFile(strWorkFilePath, title);
	}

	return ret;
}


/*============================================================================*/
/*! CPrintUtility

-# EUCのファイルをSJISに変換後に印刷する

@param	filepath：ファイルパス
@retval	なし

*/
/*============================================================================*/
BOOL CPrintUtility::PrintEucFile(CString& filepath, CString title)
{
	BOOL ret;

	// WORKフォルダを保存先にする
	CString strFileName = filepath.Right(filepath.GetLength() - filepath.ReverseFind(_T('\\')) - 1);
	CString strWorkFilePath;
	strWorkFilePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppWorkPath(), (LPCTSTR)strFileName);

	// WORKフォルダにSJISファイル保存する
	ret = CFileUtility::ExchangeFileEuctoSjis(filepath, strWorkFilePath);

	if (ret)
	{
		// WORKにファイルが保存できたら印刷する
		ret = PrintSjisFile(strWorkFilePath, title);
	}

	return ret;
}


/*============================================================================*/
/*! CPrintUtility

-# 複数のEUCのファイルを１つにまとめ、SJISに変換後に印刷する

@param	filepath：ファイルパス
@retval	なし

*/
/*============================================================================*/
BOOL CPrintUtility::PrintEucFile(vector<CString>& listFilepath, CString title)
{
	BOOL ret = FALSE;

	if (listFilepath.empty())
	{
		return FALSE;
	}

	// WORKフォルダを保存先にする
	CString strFileName = listFilepath[0].Right(listFilepath[0].GetLength() - listFilepath[0].ReverseFind(_T('\\')) - 1);
	CString strWorkFilePath;
	strWorkFilePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppWorkPath(), (LPCTSTR)strFileName);

	BOOL bOverWrite = TRUE;
	for (int i = 0; i < listFilepath.size(); i++)
	{
		// WORKフォルダにSJISファイル保存する
		ret = CFileUtility::ExchangeFileEuctoSjis(listFilepath[i], strWorkFilePath, bOverWrite);
		bOverWrite = FALSE;
	}

	if (ret)
	{
		// WORKにファイルが保存できたら印刷する
		ret = PrintSjisFile(strWorkFilePath, title);
	}

	return ret;
}

/*============================================================================*/
/*! 詳細印刷

-# 詳細印刷コンストラクタ

@param	dc			印刷用デバイスコンテキスト
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
/*! 詳細印刷

-# 詳細印刷デストラクタ

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
/*! 詳細印刷

-# 詳細印刷開始

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
	// sizeに紙サイズを取得する
	VERIFY(mDc->Escape(GETPHYSPAGESIZE, 0, NULL, (LPVOID)&size));
	mPageHeight = size.cy;
	mPageWidth = size.cx;
	// 1インチあたりのピクセル数を取得
	double dPPIX = (double)mDc->GetDeviceCaps(LOGPIXELSX);
	double dPPIY = (double)mDc->GetDeviceCaps(LOGPIXELSY);

	// mm単位に直す
	size.cx = (int)(floor(((double)size.cx / dPPIX) * 25.4 + .5));
	size.cy = (int)(floor(((double)size.cy / dPPIY) * 25.4 + .5));
// ADD 2018.06.05 <<<

	return TRUE;
}

/*============================================================================*/
/*! 詳細印刷

-# 詳細印刷 ヘッダー印刷

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
/*! 詳細印刷

-# 詳細印刷 1行印刷

@param		str		1行文字列
@retval

*/
/*============================================================================*/
void CDetailPrint::PrintLine(const CString& str, BOOL bHeader)
{
	if (!mPageStarted || mSaveVMargin > mPageHeight - mPageVMargin)
	{
		// 改ページ
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
/*! 詳細印刷

-# 詳細印刷終了

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
/*! 詳細印刷

-# 詳細印刷終了

@param
@retval

*/
/*============================================================================*/
void CDetailPrint::SetPrinterFont(bool bDiary/* = false*/)
{
	CString strFaceName = _T("");
	if (bDiary == true){
		strFaceName = _T("MS ゴシック");
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
/*! 詳細印刷

-# 詳細印刷用フォントの作成

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
/* ファイルユーティリティ
/* ------------------------------------------------------------------------------------ */
CFileUtility::CFileUtility()
{
}

CFileUtility::~CFileUtility()
{
}

/*============================================================================*/
/*! CFileUtility

-# 日数が古いテンポラリファイルを削除する

@param	filepath：元のファイルパス
@param	nDay：削除する判定日数
@retval	なし

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

-# UTF8ファイルを呼んでSJISファイルを生成する

@param	srcFilePath		読み込みファイル名(UTF8)
@param	dstFilePath		書き込みファイル名(SJIS)
@retval true:成功 / false:失敗

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
			// ファイルを１行ずつ読み出し、書き込みする
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

-# EUCファイルを呼んでSJISファイルを生成する

@param	srcFilePath		読み込みファイル名(EUC)
@param	dstFilePath		書き込みファイル名(SJIS)
@param  bOverWrite		TRUE = 上書き / FALSE = 追加
@retval true:成功 / false:失敗

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

	// 上書き
	if (bOverWrite)
	{
		if (_wfopen_s(&wFp, dstFilePath, _T("w")) != 0 || wFp == NULL)
		{
			fclose(rFp);
			return FALSE;
		}
	}
	// 追加書き込み
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
			/* １行読み込み */
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

-# EUC文字列を呼んでSJIS文字列に変換する

@param	strEuc		EUC文字列
@retval SJIS文字列

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
/* DBアクセスクラス
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

-# マクロ定義のパラメータ情報の先頭アドレスから、展開する設備制御項目名を得る。

@param	base_p;      input:検索を開始するメモリの先頭アドレス。
（個々の定義情報の先頭アドレス）
@param	tbl[];       output:制御名を格納するテーブルの先頭アドレス
@param	vec[];       output:制御情報先頭アドレスを格納するテーブルの先頭アドレス
@param	i_argc;      input: 呼び出し側で用意したtbl[]の個数(=vec[]の個数)

@retval				-1:検索IDが発見できない。
*/
/*============================================================================*/
int CDBAccess::ud_macstsadr_to_exectbl(char *base_p, char *tbl[], char *vec[], int i_argc)
{
	char   *vec_tmp[VECMAX];
	int    i_ret;
	int    i;

	/*
	パラメータテーブル検索
	*/
	i_ret = ud_adr_to_resvec(base_p, EXEC_ID, BREAK_ID, ENDOFMAC_ID, vec_tmp, VECMAX);
	if (i_ret > i_argc){
		i_ret = i_argc;
	}

	/*
	パラメータ名称取得
	*/
	if (tbl != (char **)NULL)
	{
		for (i = 0; i < i_ret; i++)
		{
			tbl[i] = ((struct exec_t *)vec_tmp[i])->sz_ctl;
		}
	}

	/*
	パラメータ情報テーブルアドレスの格納
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

-# マクロ定義のパラメータ文字列から、パラメータ情報の先頭アドレスを検索
-# し、展開する設備制御項目名を得る。


@param	base_p;		input:検索を開始するメモリの先頭アドレス。
					（個々の定義情報の先頭アドレス）
@param	psz_param;	input: パラメータ名
@param	tbl[];		output:実行制御名を格納するテーブルの先頭アドレス
@param	vec[];		output:実行制御情報先頭アドレスを格納するテーブルの先頭アドレス
@param	i_argc;input: 呼び出し側で用意したtbl[]の個数(=vec[]の個数)

@retval				-1:検索IDが発見できない。
*/
/*============================================================================*/
int CDBAccess::ud_macsts_to_exectbl(char *base_p, char *psz_param, char *tbl[], char *vec[], int i_argc)
{
	char   *vec_tmp[VECMAX];
	int    i_ret;
	int    i;

	/*
	マクロ名検索
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
	パラメータテーブル検索
	*/
	i_ret = ud_macstsadr_to_exectbl(vec_tmp[i], tbl, vec, i_argc);

	return (i_ret);
}

/*============================================================================*/
/*! CDBAccess

-# メモリ上の衛星固有情報から、マクロ定義のパラメータのステータス文字列
-# を検索し、ステータス文字列の先頭アドレス、及びパラメータ情報の先頭アド
-# レスを配列に格納して返す。

@param	base_p;		input:検索を開始するメモリの先頭アドレス。
					個々の定義情報の先頭アドレス）
@param	tbl[];      output:パラメータ名を格納するテーブルの先頭アドレス
@param	vec[];		output:パラメータ情報先頭アドレスを格納するテーブルの先頭アドレス
@param	i_argc;		input: 呼び出し側で用意したtbl[]の個数(=vec[]の個数)

@retval
*/
/*============================================================================*/
int CDBAccess::ud_macadr_to_ststbl(char *base_p, char *tbl[], char *vec[], int i_argc)
{
	char   *vec_tmp[VECMAX];
	int    i_ret;
	int    i;


	/*
	パラメータテーブル検索
	*/
	i_ret = ud_adr_to_resvec(base_p, CASE_ID, ENDOFMAC_ID, ENDOFMACDEF_ID, vec_tmp, VECMAX);
	if (i_ret > i_argc){
		i_ret = i_argc;
	}

	/*
	パラメータ名称取得
	*/
	if (tbl != (char **)NULL)
	{
		for (i = 0; i < i_ret; i++)
		{
			tbl[i] = ((struct case_t *)vec_tmp[i])->sz_param;
		}
	}

	/*
	パラメータ情報テーブルアドレスの格納
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

-# マクロ名称からパラメータのステータス文字列を検索し、ステータス文字
-# 列の先頭アドレス、及びパラメータ情報の先頭アドレスを配列に格納して返す。
-# １衛星の衛星固有情報を全て検索するので、低速である。
-# （高速検索を行う場合は、ud_satname_to_mactbl()及びud_macadr_to_ststbl()
-# を使用する事。）

@param	char *base_p;	input:検索を開始するメモリの先頭アドレス。
                          （個々の定義情報の先頭アドレス）
@param	psz_macname;	input: マクロ名
@param	tbl[];			output:ステータス名を格納するテーブルの先頭アドレス
@param	vec[];			output:ステータス情報先頭アドレスを格納するテーブルの先頭アドレス
@param	i_argc;			input: 呼び出し側で用意したtbl[]の個数(=vec[]の個数)

@retval		-1:検索マクロ名が発見できない。
*/
/*============================================================================*/
int CDBAccess::ud_macname_to_ststbl(char *base_p, char *psz_macname, char *tbl[], char *vec[], int i_argc)
{
	char   *vec_tmp[VECMAX];
//	struct mac_t  *pst_mac;
	int    i_ret;
	int    i;

	/*
	マクロ名検索
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
	パラメータテーブル検索
	*/
	i_ret = ud_macadr_to_ststbl(vec_tmp[i], tbl, vec, i_argc);

	return (i_ret);
}

/*============================================================================*/
/*! CDBAccess

-# 個々の設備制御情報、衛星定義情報、または、設備情報定義の先頭アドレスから、
-# データＩＤ値 の一致するデータ構造の先頭アドレスを求める。
-# （設備制御情報，衛星固有情報，等と同等のデータ構造を持つテーブルの検索に
-# 使える汎用のルーチン）
-# ud_adr_to_res()との違いは、探しているデータＩＤに一致するテーブルが複数
-# 存在している場合は、これらのテーブルの先頭アドレスを配列に格納 すること
-# にある。

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

-# メモリ上の衛星固有情報ＤＢから、各衛星名文字列の先頭アドレスを
-# 配列に格納して返す。

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
	衛星名取得
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

-# 衛星固有情報DB読込処理

@param	list
@retval

*/
/*============================================================================*/
int CDBAccess::eqsat_db_read()
{
	UINT sta;
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		// １局モードは、該当局を指定
		sta = theApp.GetSelectStation();
	}
	else
	{
		// ４局モードは、衛星固有DBは34mのみS帯/X帯別で監視名切替がある。
		// 34m局を指定
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
		//DB情報格納領域を初期化
		memset(m_eqsatdb_data, 0, mFileSize);

		//DBファイルを読んでくる
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

-# メモリ上の衛星固有情報ＤＢから、各衛星名文字列の先頭アドレスを
-# 配列に格納して返す。

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
	衛星名取得
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

-# 衛星固有情報DB読込処理

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
		//DB情報格納領域を初期化
		memset(m_eqmondb_data, 0, mFileSize);

		//DBファイルを読んでくる
		if (db_read(CStringA(filename).GetBuffer(), m_eqmondb_data) != 0)
		{
			return -1;
		}

		m_monnamelist.clear();
		ud_adr_to_montbl(m_monnamelist);

		delete m_eqmondb_data;	// 取り合えず監視名リストが欲しいだけなので削除。（2017/04/14、石原）
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

-# メモリ上の設備制御DBから、各衛星名文字列の先頭アドレスを
-# 配列に格納して返す。

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
	衛星名取得
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

-# 設備制御DB読込処理

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
		//DB情報格納領域を初期化
		memset(m_eqctldb_data, 0, mFileSize);

		//DBファイルを読んでくる
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

	unsigned long ul_top_pos; /*ファイルの先頭からメモリロード内容の先頭までのバイト数*/
	long   l_idnum = 0;
	FILE *fp = NULL;
	char *fname = db_file_nam;

	/*ファイル名とみなす*/
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
		/*#!HEAD:があるかどうか読んでみる*/
		return(500);
	}
	if (!strncmp(sz_work, "#!HEAD:", 7))
	{
		/*#!HEAD:を読み飛ばす*/
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
		/*ファイルの先頭から全内容を読み込む*/
		ul_top_pos = 0; /*ファイルの先頭からメモリにロードする内容の先頭までのバイト数*/
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
	l_alldatasize -= ul_top_pos; /*#!HEAD:の部分のバイト数を補正*/
	fclose(fp); /*ファイルをcloseする*/

	return 0;
}

/*============================================================================*/
/*! CDBAccess

-# 監視データ格納

@param	nStation	：局番号
@param	data		：工学値変換データ
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

-# 監視データ取得

@param	nStation	：局番号
@param	strWatch	：監視名称
@retval strct obsname_t ポインタ

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
		// 時刻監視の場合、現在時刻を更新して返す
		memset(&mon_UT_time, 0, sizeof(mon_UT_time));
		strcpy_s(mon_UT_time.sz_obsname, CTLID_LEN, CStringA(strWatch));
		mon_UT_time.d_data = (double)CTime::GetCurrentTime().GetTime();
		mon_UT_time.l_inflg = INFLG_ON;	//データ有効フラグ 1=有効
		obs = (char*)&mon_UT_time;
	}
#endif

	CGraphMutex::Unlock(eData);

	return obs;
}

/*============================================================================*/
/*! CDBAccess

-# 衛星固有DBより取得したデータより該当する衛星のインデックスを取得する

@param	str :衛星名
@retval	インデックス
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

-# レディネスチェック時間ファイル読み込み

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

-# レディネスチェック定義ファイルより該当の時間を取得する

@param	str：ターゲット
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

-# 計画時間ファイル読み込み

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

-# 計画定義ファイルより該当の時間を取得する

@param	str：ターゲット
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
// 監視情報テーブルの更新
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
/* セッション情報
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
//-# セッション状態を更新する
//
//@param	なし
//@retval	なし
//
//*/
///*============================================================================*/
//void CSessionInfo::UpdateSessionStatus()
//{
//	// STAT_SDTPで保存されたローカルの結果ファイルを読み込む。
//	// (工事中）
//	for (int i = 0; i < eSessionType_Num; i++)
//	{
//		CFileFind find;
//		CString strLockFilePath;
//		strLockFilePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetManagementPath(), STAT_SDTP_FILENAME);
//
//		if (find.FindFile(strLockFilePath))
//		{
//			// 接続状態にする
//			m_sessionStatus[i] = CSessionInfo::eConnect;
//		}
//		else
//		{
//			// 切断状態にする
//			m_sessionStatus[i] = CSessionInfo::eDisconnect;
//		}
//	}
//}
//#endif


/*============================================================================*/
/*! CSessionInfo

-# セッションタイプ文字列に該当するインデックスを返す

@param	str：セッションタイプ文字列
@retval	0以上：該当するインデックス、-1：該当なし

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

-# セッションタイプへセッション状態をセットする

@param	str：セッションタイプ文字列
@retval	0以上：該当するインデックス、-1：該当なし

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
				// 切断状態セット
				m_sessionStatus[nSession] = eDisconnect;
			}
			else if (strStatus.CompareNoCase(_T("1")) == 0)
			{
				// 接続状態セット
				m_sessionStatus[nSession] = eConnect;
			}
			else if (strStatus.CompareNoCase(_T("9")) == 0)
			{
				// 不明状態セット
				m_sessionStatus[nSession] = eUnknown;
			}

			break;
		}
	}
}


/* ------------------------------------------------------------------------------------ */
/* 衛星捕捉データ
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

-# 予報値ファイル情報取得

指定の予報値ファイルを取得し、ＡＯＳ／ＬＯＳ
取得不可時は要求種別毎に不可時の対応を行う。

@param	要求種別（リアル、設定選択、デフォルト）
@param	局番号
@retval	正常（０）／取得待ち（１）／異常（＜０）

*/
/*============================================================================*/
int CTrendData::gryohouti(int i_get_type, UINT nStation)
{
	int     	i_ret;

#ifdef _ENGVALCNV_TREND
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT sta = theApp.GetSelectStation();

	// 衛星名取得
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(EISEI_NAME));
	if (obs_adr == NULL)
		return -1;
	CString strSatellite = CString(obs_adr->sz_sts_name);

	if (strSatellite.CompareNoCase(_T("VLBI")) != 0)
	{
		// VLBIでない場合、予報値の取得状態を調べる

		/*** 予報値ファイル取得可否判別 ***/
		/* 予報値取得状態 取得 */
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

		/* 監視データ取得無効時 */
		if (obs_adr->l_inflg != INFLG_ON) {
			m_gst_dspinf.i_yohouti_get = YOHOGET_MATI;    /* 取得待ち */
			m_gst_dspinf.i_yohouti_umu = YOHOUTI_NASI;    /* (V2-1)   */

			CString str;
			str.Format(_T("%s(%d) :: yohochi(%s) is invalid"), __FILEW__, __LINE__, (LPCTSTR)CString(YOHOU_GET));
			CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("gryohouti"), _T("Error"), str, nLogEx::error);
			return(1);
		}
		/* 可否判別 */
		if (strYOHOU_GET.CompareNoCase(_T("TAKING")) == 0) {	/* 予報値取得中 */
			m_gst_dspinf.i_yohouti_get = YOHOGET_MATI;    /* 取得待ち */
			m_gst_dspinf.i_yohouti_umu = YOHOUTI_NASI;    /* (V2-1)   */

			CString str;
			str.Format(_T("%s(%d) :: Can not yohochi, becase is is TAKING"), __FILEW__, __LINE__);
			CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("gryohouti"), _T("info"), str, nLogEx::info);
			return(1);
		}
		else {
			m_gst_dspinf.i_yohouti_get = YOHOGET_SUMI;    /* 取得済み */
		}
	}
	else
	{
		// VLBIの場合
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(_T("CTRL.PASS_INFO")));
		if (obs_adr != NULL)
		{
			if (obs_adr->l_inflg != INFLG_ON) {
				m_gst_dspinf.i_yohouti_get = YOHOGET_MATI;    /* 取得待ち */
				m_gst_dspinf.i_yohouti_umu = YOHOUTI_NASI;    /* (V2-1)   */
				return(1);
			}
			CString strPassEnd(obs_adr->sz_sts_name);
			if (strPassEnd.Compare(_T("運用終了")) == 0)
			{
				m_gst_dspinf.i_yohouti_get = YOHOGET_MATI;    /* 取得待ち */
				m_gst_dspinf.i_yohouti_umu = YOHOUTI_NASI;    /* (V2-1)   */
				return(1);
			}
			else
			{
				m_gst_dspinf.i_yohouti_get = YOHOGET_SUMI;    /* 取得済み */
			}
		}
	}

#else
	m_gst_dspinf.i_yohouti_get = YOHOGET_SUMI;
#endif

	/*** 予報値ファイル取得処理 ***/
	switch (i_get_type) {
	case GET_REAL:  /* リアル表示中での取得 */
		if (m_gst_dspinf.i_yohouti_get == YOHOGET_SUMI) {
			i_ret = gryohouti2(0, nStation);	/* <<XUP>> 引数にプロットモードを追加 */
		}
		else {
			return(1);
		}
		break;
	case GET_SET:   /* 保存表示中での予報値選択「設定」時の取得 */
	case GET_DEF:   /* 保存表示中での予報値選択「デフォルト」時の取得 */
		if (m_gst_dspinf.i_yohouti_get == YOHOGET_SUMI) {
			i_ret = gryohouti2(1, nStation);	/* <<XUP>> 引数にプロットモードを追加 */
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

-# 予報値ファイル情報取得
指定の衛星名パス番号の予報値ファイルを参照し、
ＡＯＳ／ＬＯＳ時刻を取得する
予報値ファイルがない場合は、最新監視データ発生時刻を
ＡＯＳ時刻、それから２４Ｈ後をＬＯＳ時刻とみなす。

@param	i_prot_mode：プロットモード
0:リアルデータ ／ 1:保存データ
@param	局番号
@retval	正常（０）／未変更（１）／ファイル内容が異常（－１）

*/
/*============================================================================*/
int CTrendData::gryohouti2(int i_prot_mode, UINT nStation)
{
	int     i;
	char    ca_fpath[SATDATA_BUFF_SIZE];
	char	ca_TIME[26];			/* 時刻（フルフォーマット）		  */
	double	d_aos;
	double	d_los;
	long l_az;

//	// 衛星往復距離のマップクリア
//	m_2WayRngMap.clear();

	/*--------------*/
	/* ファイル取得 */
	/*--------------*/

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	UINT sta = theApp.GetSelectStation();

	/*** 予報値ファイル取得可否判別 ***/
	/* 予報値取得状態 取得 */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(PRED_FILE_NAME));
	if (obs_adr == NULL)
		return -1;
	CString strYOHOU_OpCtrlFilePath;
	strYOHOU_OpCtrlFilePath.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
	strYOHOU_OpCtrlFilePath.Trim(_T(" "));
	// 拡張子がprdの場合、anpに置き換える
	strYOHOU_OpCtrlFilePath.TrimRight(PRED_FILE_TYPE);

	if (strYOHOU_OpCtrlFilePath.IsEmpty())
	{
		CString str;
		str.Format(_T("%s(%d) :: Could not get yohochi file path"), __FILEW__, __LINE__);
		CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("gryohouti2"), _T("debug"), str, nLogEx::debug);
		return(-1);
	}

	strYOHOU_OpCtrlFilePath += ANP_FILE_TYPE;

	// ファイル名を切り出す
	CString strFileName = strYOHOU_OpCtrlFilePath.Right(strYOHOU_OpCtrlFilePath.GetLength() - strYOHOU_OpCtrlFilePath.ReverseFind(_T('/')) - 1);
	CString strFileTmp = strYOHOU_OpCtrlFilePath.TrimRight(strFileName);
	strFileTmp = strFileTmp.TrimRight(_T("/"));
	// 衛星名を切り出す
	CString strSatelliteName = strFileTmp.Right(strFileTmp.GetLength() - strFileTmp.ReverseFind(_T('/')) - 1);
	// 局フォルダを取得
	CString strStationPath;
	strStationPath = theApp.GetShareFilePath(eFileType_Pred_Folder, nStation);
	// 局フォルダをつける
	CString strFilePath;
	strFilePath.Format(_T("%s%s\\%s"), (LPCTSTR)strStationPath, (LPCTSTR)strSatelliteName, (LPCTSTR)strFileName);

	if (strFilePath.IsEmpty())
		return -1;

//	TRACE("@@@%s\n", CStringA(strFilePath));

	// 予報値ファイルが更新されているか確認
	HANDLE hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FILETIME yohoFileTime;
	GetFileTime(hFile, NULL, NULL, &yohoFileTime);
	CloseHandle(hFile);
	// ファイル名とファイル時刻比較
	if (m_yohoFilePath[nStation].Compare(strFilePath) == 0 &&
		CompareFileTime(&m_yohoFileTime[nStation], &yohoFileTime) == 0)
	{
		// 変更なしならば何もしない
		return 1;
	}

	/*------------------*/
	/* ファイルＯＰＥＮ */
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
	/* 縦軸スケール情報取得 */
	/*----------------------*/
	/* ＡＯＳ／ＬＯＳ情報レコードまで検索 */
	/*	for( i = 0; i < 5; i++ ) { */

	CTrackingRadarCtrlMonData	*pRadarData = &theApp.GetSatelliteData().m_arrayTrackingRadarCtrlMonData[nStation];
	if (pRadarData == NULL)
	{
		CGraphMutex::Unlock(eRadar);
		return -1;
	}

	// 衛星往復距離のマップクリア
	m_2WayRngMap.clear();

	// 予報値をグラフのキューから全て削除する
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		(&theApp.GetDataCollection())->ClearCollection(theApp.GetSelectStation());
	}

	// EL予報値補間計算
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

	/* <<XUP>> 予報値カウンタクリア */
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

//	// グラフ表示の都合上、暫定で現在時刻に置き換える
//	time_t tPos = 10;
//	//	time_t tPos = 0;
#endif

	for (i = 0;; i++) {

		/* <<XUP>> 予報値テーブルのストッパーを検出したならば、終了 */
		if (strcmp(m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].ca_predict_s_tx_freq, END_OF_TABLE) == 0) {
			break;
		}

		// 予報値データを衛星データのテンポラリに追加する
		stServerItemData data;
		memset(&data, 0, sizeof(stServerItemData));
		data.kind = 0;

		/* <<XUP>> 予報値時刻の型変換（BCD→ASCII） */
		memset(ca_TIME, (char)NULL, sizeof(ca_TIME));
		bcd7_to_asc19_time(m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].uca_time, (unsigned char*)ca_TIME);
//		m_glla_time[i_prot_mode][m_gla_prd_cont[i_prot_mode]] = (time_t)CSatelliteData::ub_asc_to_dtime(ca_TIME);
		CTime ctime = CTime((time_t)CSatelliteData::ub_asc_to_dtime(ca_TIME));

		SYSTEMTIME systime;
		ctime.GetAsSystemTime(systime);

//#ifdef _NOCOMM
#ifdef _ENGVALCNV_TREND
#else
		// グラフ表示の都合上、暫定で現在時刻に置き換える
//		GetSystemTime(&systime);
//		ctime = systime;
//		ctime = ctime + tPos;
//		ctime.GetAsSystemTime(systime);
//		tPos += 20;
		ctime = ctime + tOffset;
		ctime.GetAsSystemTime(systime);
#endif
		CCommBase::ConvertTime(&systime, &data.systime);

		// RNG伝搬モニタ用に、予報値時刻と往復距離のマップを用意する
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
		printf( "予報値時刻 : [%02x%02x%02x%02x%02x%02x%02x] [%s] [%ld]\n"
		, m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].uca_time[0]
		, m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].uca_time[1]
		, m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].uca_time[2]
		, m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].uca_time[3]
		, m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].uca_time[4]
		, m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].uca_time[5]
		, m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].uca_time[6]
		, ca_TIME, gla_time[i_prot_mode][m_gla_prd_cont[i_prot_mode]] );
		*/

		/* ダウンリンクレンジは、その時刻に受信する信号の往復距離の1/2なので、そのまま使ってよい。*/
#ifdef NO_USE
		double	d_rtt;					/* ＲＴＴ（秒）                   */
		double	d_downrng;				/* ダウンリンクレンジ（ｋｍ）     */
		double	d_downrr;				/* ダウンリンクレンジレート（ｍ／Ｓ）*/
		double	d_data;
		/* <<XUP>> ＲＴＴを考慮したダウンリンクレンジを生成する */
		d_data = (double)m_glla_time[i_prot_mode][m_gla_prd_cont[i_prot_mode]];
		d_rtt = lb_rttcal(&m_gst_predata, d_data, RTT_BASE_DOWN, &d_downrng, "+00:00", (double)0.0);

		/* <<XUP>> ＲＴＴを考慮したダウンリンクレンジレートを生成する */
		d_rtt = lb_rttcal_rr(&m_gst_predata, d_data, RTT_BASE_DOWN, &d_downrr, "+00:00", (double)0.0);

		m_gda_dwRNG[i_prot_mode][m_gla_prd_cont[i_prot_mode]] = d_downrng / (double)2.0;
		m_gda_dwRR[i_prot_mode][m_gla_prd_cont[i_prot_mode]] = d_downrr;
#endif
#if(0)  /* ダウンリンクレンジ -> 2wayレンジの1/2 */
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

		// RNG予報値データから衛星データ生成
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

		// RR予報値データより衛星データ生成
		(&theApp.GetDataCollection())->CreateForecastData(&data, nStation);

// test >>>
//		strcpy_s((char*)data.name, _MAX_NAME, "UDSC642.RR(PRED)");
//		(&theApp.GetDataCollection())->CreateForecastData(&data);
//		strcpy_s((char*)data.name, _MAX_NAME, "USC34.RR(PRED)");
//		(&theApp.GetDataCollection())->CreateForecastData(&data);
//		strcpy_s((char*)data.name, _MAX_NAME, "USC20.RR(PRED)");
//		(&theApp.GetDataCollection())->CreateForecastData(&data);
// test <<<


		// EL予報値データより衛星データ生成
		data.value = m_gst_predata.predata[m_gla_prd_cont[i_prot_mode]].d_predict_el;
		strcpy_s((char*)data.name, _MAX_NAME, "EL(PRED)");
		(&theApp.GetDataCollection())->CreateForecastData(&data, nStation);

		// EL予報値を補間計算前にリストに格納しておく
		if (bPredDiffEna)
		{
			__int64 ELPredDataSize = ELPredData.value.size();
			if (ELPredDataSize == 0 ||
				ELPredDataSize > 0 && CTime(systime).GetTime() > CTime(ELPredData.value[ELPredDataSize - 1].systime).GetTime())	// 時刻が異常な予報値対応
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


		// スカイラインデータより衛星データ生成
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

		// RFトリップデータより衛星データ生成
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

		/* <<2013-TURBO>> 1秒更新24時間予報値に対応し、EOTがない場合の終了判定を追加 */
		if (m_gla_prd_cont[i_prot_mode] == MAX_PRED_SIZE) {
			break;
		}
	}

	// EL予報値補間計算
	if (bPredDiffEna)
	{
		vector<stPredData>& ELlist = theApp.GetDataCollection().GetELPredInterpolateList(nStation);
		ELlist.clear();
		if (ELPredData.value.size() > 0)
		{
			//		map<time_t, double>& ELMap = theApp.GetDataCollection().GetELPredInterpolateMap(nStation);

			// AOSからLOS間のデータ補間
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

-# 予報値ファイルを読み込む

@param	szp_file_name：ファイルパス

0:リアルデータ ／ 1:保存データ
@retval	正常（０）／ファイル内容が異常（－１）

*/
/*============================================================================*/
int CTrendData::load_pred_data(char *szp_file_name, double *d_aos, double *d_los, bool* pNewFormat/* = NULL*/)
{
	int				i_counter;			/* カウンタ                                   */
	int				i_ret;				/* 関数返値                                   */
	int				i_tmp;				/* ｉｎｔ型テンポラリ                         */
	FILE			*p_fp;				/* ファイルポインタ                           */
	unsigned char	sz_tmp[512];		/* 予報値ファイル読み飛ばしレコード用バッファ */
	unsigned char	sz_read_buff[512];	/* 予報値ファイル読み込み用バッファ 1997-03-13*/
	unsigned char	sz_read_tmp01[32];	/* 予報値ファイル分割用バッファ     1997-03-13*/
	unsigned char	sz_read_tmp03[32] = { 0 };	/* 予報値ファイル分割用バッファ     1997-03-13*/
	unsigned char	sz_read_tmp04[32] = { 0 };	/* 予報値ファイル分割用バッファ     1997-03-13*/
	unsigned char	sz_read_tmp05[32] = { 0 };	/* 予報値ファイル分割用バッファ     1997-03-13*/
	unsigned char	sz_read_tmp06[32] = { 0 };	/* 予報値ファイル分割用バッファ     1997-03-13*/
	unsigned char	sz_read_tmp07[32] = { 0 };	/* 予報値ファイル分割用バッファ     1997-03-13*/
	unsigned char	sz_read_tmp08[32] = { 0 };	/* 予報値ファイル分割用バッファ     1997-03-13*/
	unsigned char	sz_read_tmp09[32] = { 0 };	/* 予報値ファイル分割用バッファ     1997-03-13*/
	unsigned char	sz_read_tmp10[32] = { 0 };	/* 予報値ファイル分割用バッファ     1997-03-13*/
	unsigned char	sz_read_tmp11[32];	/* 予報値ファイル分割用バッファ     1997-03-13*/

	char			sz_work[256];
	char			sz_name[40];
	char			sz_passno[40];
	char			sz_time[40];
	char			sz_asctime[40];

	double			d_az;
	double			d_el;
	double			d_uprng;

	long			l_rc;

	/* 予報値テーブルクリア */
	memset((unsigned char *)m_gst_predata.predata, (char)NULL, sizeof(one_predata_t)*MAX_PRED_SIZE);

	/* 予報値ファイルオープン */
	/*------------------*/
	/* ファイルＯＰＥＮ */
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

		/* 予報値ファイルオープン異常 */
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

	/* ファイル管理部読み飛ばし */
	if (fgets((char *)sz_tmp, sizeof(sz_tmp), p_fp) == NULL){
		CString str;
		str.Format(_T("%s(%d) ::  Could not read yohochi file(%s)"), __FILEW__, __LINE__, (LPCTSTR)CString(szp_file_name));
		CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("load_pred_data"), _T("Error"), str, nLogEx::error);
		fclose(p_fp);
		return -1;
	}

	/* パス情報部読み飛ばし */
	if (fgets((char *)sz_tmp, sizeof(sz_tmp), p_fp) == NULL){
		CString str;
		str.Format(_T("%s(%d) ::  Could not read yohochi file(%s) pass info"), __FILEW__, __LINE__, (LPCTSTR)CString(szp_file_name));
		CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("load_pred_data"), _T("Error"), str, nLogEx::error);
		fclose(p_fp);
		return -1;
	}

	/* ＡＯＳ情報部読み飛ばし */
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

	/* ＬＯＳ情報部読み飛ばし */
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

	/* ＭＡＸ情報部読み飛ばし */
	if (fgets((char *)sz_tmp, sizeof(sz_tmp), p_fp) == NULL){
		CString str;
		str.Format(_T("%s(%d) ::  Could not read yohochi file(%s) MAX info"), __FILEW__, __LINE__, (LPCTSTR)CString(szp_file_name));
		CLogTraceEx::Write(_T(""), _T("CTrendData"), _T("load_pred_data"), _T("Error"), str, nLogEx::error);
		fclose(p_fp);
		return -1;
	}

	/* カウンタクリア */
	i_counter = 0;

	/* 予報値ファイル読み込み */
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
				/* ファイル読み込み終了（ＥＯＦ） */
				break;
			}
		}

		/* 項目分割 */
		i_ret = sscanf_s((char *)sz_read_buff, "%s %s %s %s %s %s %s %s %s %s",
			sz_read_tmp01, 32, sz_read_tmp03, 32, sz_read_tmp04, 32, sz_read_tmp05, 32,
			sz_read_tmp06, 32, sz_read_tmp07, 32, sz_read_tmp08, 32,
			sz_read_tmp09, 32, sz_read_tmp10, 32, sz_read_tmp11, 32);	/* <<2013-TURBO>> 深宇宙用新フォーマットに対応 */

		CSatelliteData::ub_asc_to_asctime((char*)sz_read_tmp01, (char*)sz_tmp);

		/* (YYYY-MM-DD HH:MM:DD)をシェアードメモリへ時刻（ＢＣＤ：YYYYMMDDHHMMSS00）設定 */
		i_tmp = 10 + 1 + 8;
		asc19_to_bcd7_time(sz_tmp, m_gst_predata.predata[i_counter].uca_time);
		m_gst_predata.predata[i_counter].uca_time[7] = 0x00;

		/* (999.9999)をシェアードメモリへＡｚ（float）設定 */
		m_gst_predata.predata[i_counter].d_predict_az = atof((char *)sz_read_tmp03);

		/* ( 99.9999)をシェアードメモリへＥｌ（float）設定 */
		m_gst_predata.predata[i_counter].d_predict_el = atof((char *)sz_read_tmp04);

		/* (9999999999999.999)をシェアードメモリへアップリンクレンジ（double）設定 */
		m_gst_predata.predata[i_counter].d_predict_uplink_range = atof((char *)sz_read_tmp05);

		/* (9999999999999.999)をシェアードメモリへダウンリンクレンジ（double）設定 */
		m_gst_predata.predata[i_counter].d_predict_downlink_range = atof((char *)sz_read_tmp07);

		/* (999999.999)をシェアードメモリへアップリンクレンジレート（float）設定 */
		m_gst_predata.predata[i_counter].d_predict_uplink_range_rate = atof((char *)sz_read_tmp06);

		/* (999999.999)をシェアードメモリへダウンリンクレンジレート（float）設定 */
		m_gst_predata.predata[i_counter].d_predict_downlink_range_rate = atof((char *)sz_read_tmp08);

		/* シェアードメモリへ予測送信周波数(9999999999.99999)設定 */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_s_tx_freq, PRED_FREQ_LEN, "0.0");

		/* シェアードメモリへＳ帯１ＷＡＹ予測受信周波数(9999999999.99999)設定 */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_s_rx_1way_freq, PRED_FREQ_LEN, "0.0");

		/* シェアードメモリへＸ帯１ＷＡＹ予測受信周波数(9999999999.99999)設定 */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_x_rx_1way_freq, PRED_FREQ_LEN, "0.0");

		/* シェアードメモリへＳ帯２ＷＡＹ予測受信周波数(9999999999.99999)設定 */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_s_rx_2way_freq, PRED_FREQ_LEN, "0.0");

		/* シェアードメモリへＸ帯２ＷＡＹ予測受信周波数(9999999999.99999)設定 */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_x_rx_2way_freq, PRED_FREQ_LEN, "0.0");

		/* (lg型)をシェアードメモリへＳ帯１ＷＡＹ予測ランプレート(999999.99999)設定 */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_s_rx_1way_ramprate, PRED_RAMP_LEN, "0.0");

		/* (lg型)をシェアードメモリへＸ帯１ＷＡＹ予測ランプレート(999999.99999)設定 */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_x_rx_1way_ramprate, PRED_RAMP_LEN, "0.0");

		/* (lg型)をシェアードメモリへＳ帯２ＷＡＹ予測ランプレート(999999.99999)設定 */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_s_rx_2way_ramprate, PRED_RAMP_LEN, "0.0");

		/* (lg型)をシェアードメモリへＸ帯２ＷＡＹ予測ランプレート(999999.99999)設定 */
		strcpy_s(m_gst_predata.predata[i_counter].ca_predict_x_rx_2way_ramprate, PRED_RAMP_LEN, "0.0");

		/* <<2013-TURBO>> 深宇宙用新フォーマットに対応 */
		if (i_ret == 10) {		/* 新フォーマット */

			/* 予測２ＷＡＹレンジをシェアードメモリへ（double）設定 */
			m_gst_predata.predata[i_counter].d_predict_2way_rng = atof((char *)sz_read_tmp09);

			/* 予測２ＷＡＹレンジレートをシェアードメモリへ（double）設定 */
			m_gst_predata.predata[i_counter].d_predict_2way_range_rate = atof((char *)sz_read_tmp10);

			/* 予測３ＷＡＹレンジレートをシェアードメモリへ（double）設定 */
			m_gst_predata.predata[i_counter].d_predict_3way_range_rate = atof((char *)sz_read_tmp11);

			if (pNewFormat != NULL)
				*pNewFormat = 1;
		}
		else {					/* 従来フォーマット */

			/* 予測２ＷＡＹレンジにダミーデータをセット */
			m_gst_predata.predata[i_counter].d_predict_2way_rng = (double)-1.0;

			/* 予測２ＷＡＹレンジレートにダミーデータをセット */
			m_gst_predata.predata[i_counter].d_predict_2way_range_rate = (double)-1.0;

			/* 予測３ＷＡＹレンジレートにダミーデータをセット */
			m_gst_predata.predata[i_counter].d_predict_3way_range_rate = (double)-1.0;

			if (pNewFormat != NULL)
				*pNewFormat = 0;
		}

		/* カウンタＵＰ */
		i_counter++;
	}

	m_PredItemCount = i_counter;

	if (i_counter < (MAX_PRED_SIZE - 1)){

		/* 予報値テーブルにストッパ設定 */
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

-# 時刻をＴＸＴ(YYYY - MM - DD HH : MM : SS)からＤＣＢ(YYYYMMDDHHMMSS)へ変換する

@param	psz_asc19_time：変換元ＴＸＴ時刻(YYYY-MM-DD HH:MM:SS)先頭アドレス
@param	psz_bcd7_time：変換後ＢＣＤ時刻(YYYYMMDDHHMMSS)先頭アドレス
@retval	なし

*/
/*============================================================================*/
void	CTrendData::asc19_to_bcd7_time(unsigned char *psz_asc19_time, unsigned char *psz_bcd7_time)
{
	/* ＡＳＣＩＩの上位ＹＹをＢＣＤのＹＹへ変換 */
	*(psz_bcd7_time + 0) = (*(psz_asc19_time + 0) & 0x0f) << 4;
	*(psz_bcd7_time + 0) = *(psz_bcd7_time + 0) | (*(psz_asc19_time + 1) & 0x0f);

	/* ＡＳＣＩＩの下位ＹＹをＢＣＤのＹＹへ変換 */
	*(psz_bcd7_time + 1) = (*(psz_asc19_time + 2) & 0x0f) << 4;
	*(psz_bcd7_time + 1) = *(psz_bcd7_time + 1) | (*(psz_asc19_time + 3) & 0x0f);

	/* ＡＳＣＩＩのＭＭをＢＣＤのＭＭへ変換 */
	*(psz_bcd7_time + 2) = (*(psz_asc19_time + 5) & 0x0f) << 4;
	*(psz_bcd7_time + 2) = *(psz_bcd7_time + 2) | (*(psz_asc19_time + 6) & 0x0f);

	/* ＡＳＣＩＩのＤＤをＢＣＤのＤＤへ変換 */
	*(psz_bcd7_time + 3) = (*(psz_asc19_time + 8) & 0x0f) << 4;
	*(psz_bcd7_time + 3) = *(psz_bcd7_time + 3) | (*(psz_asc19_time + 9) & 0x0f);

	/* ＡＳＣＩＩのＨＨをＢＣＤのＨＨへ変換 */
	*(psz_bcd7_time + 4) = (*(psz_asc19_time + 11) & 0x0f) << 4;
	*(psz_bcd7_time + 4) = *(psz_bcd7_time + 4) | (*(psz_asc19_time + 12) & 0x0f);

	/* ＢＣＤのＭＭをＡＳＣＩＩのＭＭへ変換 */
	*(psz_bcd7_time + 5) = (*(psz_asc19_time + 14) & 0x0f) << 4;
	*(psz_bcd7_time + 5) = *(psz_bcd7_time + 5) | (*(psz_asc19_time + 15) & 0x0f);

	/* ＢＣＤのＳＳをＡＳＣＩＩのＳＳへ変換 */
	*(psz_bcd7_time + 6) = (*(psz_asc19_time + 17) & 0x0f) << 4;
	*(psz_bcd7_time + 6) = *(psz_bcd7_time + 6) | (*(psz_asc19_time + 18) & 0x0f);
}


/*============================================================================*/
/*! CTrendData

-# 時刻をＤＣＢ(YYYYMMDDHHMMSS)からＴＸＴ(YYYY - MM - DD HH : MM : SS)へ変換する

@param	psz_bcd7_time：変換元ＢＣＤ時刻(YYYYMMDDHHMMSS)先頭アドレス
@param	psz_asc19_time：変換後ＴＸＴ時刻(YYYY-MM-DD HH:MM:SS)先頭アドレス
@retval	なし

*/
/*============================================================================*/
void	CTrendData::bcd7_to_asc19_time(unsigned char* psz_bcd7_time, unsigned char* psz_asc19_time)
{

	/* ＢＣＤの上位ＹＹをＡＳＣＩＩのＹＹへ変換 */
	*(psz_asc19_time + 0) = ((*(psz_bcd7_time + 0) >> 4) & 0x0f) | 0x30;
	*(psz_asc19_time + 1) = ((*(psz_bcd7_time + 0)) & 0x0f) | 0x30;

	/* ＢＣＤの下位ＹＹをＡＳＣＩＩのＹＹへ変換 */
	*(psz_asc19_time + 2) = ((*(psz_bcd7_time + 1) >> 4) & 0x0f) | 0x30;
	*(psz_asc19_time + 3) = ((*(psz_bcd7_time + 1)) & 0x0f) | 0x30;

	/* "-"追加 */
	*(psz_asc19_time + 4) = 0x2d;

	/* ＢＣＤのＭＭをＡＳＣＩＩのＭＭへ変換 */
	*(psz_asc19_time + 5) = ((*(psz_bcd7_time + 2) >> 4) & 0x0f) | 0x30;
	*(psz_asc19_time + 6) = ((*(psz_bcd7_time + 2)) & 0x0f) | 0x30;

	/* "-"追加 */
	*(psz_asc19_time + 7) = 0x2d;

	/* ＢＣＤのＤＤをＡＳＣＩＩのＤＤへ変換 */
	*(psz_asc19_time + 8) = ((*(psz_bcd7_time + 3) >> 4) & 0x0f) | 0x30;
	*(psz_asc19_time + 9) = ((*(psz_bcd7_time + 3)) & 0x0f) | 0x30;

	/* " "追加 */
	*(psz_asc19_time + 10) = 0x20;

	/* ＢＣＤのＨＨをＡＳＣＩＩのＨＨへ変換 */
	*(psz_asc19_time + 11) = ((*(psz_bcd7_time + 4) >> 4) & 0x0f) | 0x30;
	*(psz_asc19_time + 12) = ((*(psz_bcd7_time + 4)) & 0x0f) | 0x30;

	/* ":"追加 */
	*(psz_asc19_time + 13) = 0x3a;

	/* ＢＣＤのＭＭをＡＳＣＩＩのＭＭへ変換 */
	*(psz_asc19_time + 14) = ((*(psz_bcd7_time + 5) >> 4) & 0x0f) | 0x30;
	*(psz_asc19_time + 15) = ((*(psz_bcd7_time + 5)) & 0x0f) | 0x30;

	/* ":"追加 */
	*(psz_asc19_time + 16) = 0x3a;

	/* ＢＣＤのＳＳをＡＳＣＩＩのＳＳへ変換 */
	*(psz_asc19_time + 17) = ((*(psz_bcd7_time + 6) >> 4) & 0x0f) | 0x30;
	*(psz_asc19_time + 18) = ((*(psz_bcd7_time + 6)) & 0x0f) | 0x30;

	/* ＮＵＬＬ追加 */
	*(psz_asc19_time + 19) = NULL;


}

//#if 0
///*============================================================================*/
///*! CTrendData
//
//-# ＲＴＴ計算機能
//
//指定時刻の予報値を検索し、往復距離値、ＲＴＴを算出する。
//
//ダウンリンクベースが選択された場合
//１.指定された時刻のダウンリンクレンジを抽出する。
//２.抽出されたダウンリンクレンジからダウンリンク時間を算出する。
//３.ダウンリンク時間を２倍し、アップリンクが送出された時刻(近似値)を
//算出する。
//４.アップリンク送出時刻での予報値からアップリンクレンジを抽出する。
//５.抽出されたアップリンクレンジからアップリンク時間を算出する。
//６.アップリンク／ダウンリンクレンジから往復距離値を算出する。
//７.アップリンク／ダウンリンク時間からＲＴＴを算出する。
//<<2013-TURBO>>
//予報値の新フォーマット化に伴い、DSNフォーマットの場合は2-WAY RNGを
//用いてRTTを求める。
//
//アップリンクベースが選択された場合
//１.指定された時刻のアップリンクレンジを抽出する。
//２.抽出されたアップリンクレンジからアップリンク時間を算出する。
//３.アップリンク時間を２倍し、ダウンリンクが帰着する時刻(近似値)を
//算出する。
//４.ダウンリンク帰着時刻での予報値からダウンリンクレンジを抽出する。
//５.抽出されたダウンリンクレンジからダウンリンク時間を算出する。
//６.アップリンク／ダウンリンクレンジから往復距離値を算出する。
//７.アップリンク／ダウンリンク時間からＲＴＴを算出する。
//<<2013-TURBO>>
//予報値フォーマットの解釈が誤っており、アップリンクレンジは当該時刻
//に送信した信号が衛星で折り返り、地上に到達するまでの距離の１／２で
//あることが解ったので、アップリンクレンジを２倍して求めることとする。
//
//備考     : XUP対応
//
//@param *stp_predict;      I : 展開した予報値テーブルのポインタ
//@param d_pred_t;          I : 求める予報値の時刻（積算秒）
//@param i_cal_flag;        I : RTT計算方法指定フラグ
//RTT_BASE_UP   = アップリンクベース
//RTT_BASE_DOWN = ダウンリンクベース
//@param *dp_range;       　O :往復伝播距離（ｋｍ）
//@param *psz_offset_time;  I :設備監視テーブル時刻オフセット値アドレス
//@param d_offset_freq;     I :周波数オフセット値
//@retval	１以上：ＲＴＴ時間（秒）
//０　　：ＲＴＴが１未満の場合、またはエラーの場合
//*/
///*============================================================================*/
///* デバッグ用マクロ宣言 */
///* #define		DEBUG1		/* エラー表示 */
///* #define		DEBUG2		/* 処理ルート */
///* #define		DEBUG3		/* 詳細データ */
//double CTrendData::lb_rttcal(predata_t* stp_predict, double d_pred_t, int i_cal_flag, double* dp_range, char* psz_offset_time, double d_offset_freq)
//{
//
//	int         i_ret;       /* 関数戻り値バッファ                   */
//	double      d_rng_up;       /* アップリンクレンジ（ｋｍ）           */
//	double      d_rng_down;       /* ダウンリンクレンジ（ｋｍ）           */
//	double      d_rng;       /* 往復距離値（ｋｍ）                   */
//	double      d_rtt_up;       /* アップリンク時間（秒）               */
//	double      d_rtt_down;       /* ダウンリンク時間（秒）               */
//	double      d_rtt;       /* ＲＴＴ時間（秒）                     */
//	double      d_c;       /* 光速（Ｋｍ／Ｓ）                     */
//	one_predata_t   st_predict;       /* 予報値情報                           */
//
//
//	d_c = V_C / 1000;                            /* 光速（Ｋｍ／Ｓ） */
//
//	/* 指定時刻の予報値を予報値テーブルから検索 */
//	i_ret = lb_prdcal(&st_predict, stp_predict, d_pred_t, 1, psz_offset_time, d_offset_freq);
//
//	if (i_ret != -1) {                         /* 成功した場合 */
//
//		switch (i_cal_flag) {
//
//		case RTT_BASE_UP:                   /* アップリンクベース */
//#ifdef NO_USE	/* <<2013-TURBO>> */
//			/* アップリンクレンジ取得 */
//			d_rng_up = st_predict.d_predict_uplink_range;
//			d_rtt_up = d_rng_up / d_c;         /* アップリンク時間算出 */
//
//			/* ダウンリンク帰着時刻の予報値を予報値テーブルから検索 */
//			i_ret = lb_prdcal(&st_predict, stp_predict, d_pred_t + (d_rtt_up * 2), 1, psz_offset_time, d_offset_freq);
//			if (i_ret == -1) {                 /* 抽出エラー */
//				d_rng = 0.0;
//				d_rtt = 0.0;
//			}
//			else {                              /* ダウンリンクレンジ取得 */
//				d_rng_down = st_predict.d_predict_downlink_range;
//				d_rtt_down = d_rng_down / d_c; /* ダウンリンク時間算出 */
//
//				d_rng = d_rng_up + d_rng_down; /* ＲＴＴ算出 */
//				d_rtt = d_rtt_up + d_rtt_down;
//			}
//#endif
//
//			/* <<2013-TURBO>> 計算方法変更 */
//			d_rng = st_predict.d_predict_uplink_range * (double)2.0;
//			d_rtt = d_rng / d_c;
//
//			break;
//
//		case RTT_BASE_DOWN:                   /* ダウンリンクベース */
//
//			/* <<2013-TURBO>> 旧フォーマットの場合は従来通り */
//			if (st_predict.d_predict_2way_rng < (double)0.0) {
//
//				/* ダウンリンクレンジ取得 */
//				d_rng_down = st_predict.d_predict_downlink_range;
//				d_rtt_down = d_rng_down / d_c;     /* ダウンリンク時間算出 */
//
//				/* アップリンク送信時刻の予報値を予報値テーブルから検索 */
//				i_ret = lb_prdcal(&st_predict, stp_predict, d_pred_t - (d_rtt_down * 2), 1, psz_offset_time, d_offset_freq);
//				if (i_ret == -1) {                 /* 抽出エラー */
//					d_rng = 0.0;
//					d_rtt = 0.0;
//				}
//				else {                              /* アップリンクレンジ取得 */
//					d_rng_up = st_predict.d_predict_uplink_range;
//					d_rtt_up = d_rng_up / d_c;     /* アップリンク時間算出 */
//
//					d_rng = d_rng_up + d_rng_down; /* ＲＴＴ算出 */
//					d_rtt = d_rtt_up + d_rtt_down;
//				}
//			}
//			else {	/* <<2013-TURBO>> 新フォーマットの場合は２－ＷＡＹレンジを使用する */
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
//	*dp_range = d_rng;                         /* 引数設定（往復伝播距離） */
//
//	return(d_rtt);
//}
//
//
///*============================================================================*/
///*! CTrendData
//
//-# ＲＴＴ計算機能（ＲＲ出力バージョン）
//
//指定時刻の予報値を検索し、往復距離値、ＲＴＴを算出する。
//
//ダウンリンクベースが選択された場合
//１.指定された時刻のダウンリンクレンジを抽出する。
//２.抽出されたダウンリンクレンジからダウンリンク時間を算出する。
//３.ダウンリンク時間を２倍し、アップリンクが送出された時刻(近似値)を
//算出する。
//４.アップリンク送出時刻での予報値からアップリンクレンジを抽出する。
//５.抽出されたアップリンクレンジからアップリンク時間を算出する。
//６.アップリンク／ダウンリンクレンジから往復距離値を算出する。
//７.アップリンク／ダウンリンク時間からＲＴＴを算出する。
//<<2013-TURBO>>
//予報値の新フォーマット化に伴い、DSNフォーマットの場合は2-WAY RNGを
//用いてRTTを求める。
//
//アップリンクベースが選択された場合
//１.指定された時刻のアップリンクレンジを抽出する。
//２.抽出されたアップリンクレンジからアップリンク時間を算出する。
//３.アップリンク時間を２倍し、ダウンリンクが帰着する時刻(近似値)を
//算出する。
//４.ダウンリンク帰着時刻での予報値からダウンリンクレンジを抽出する。
//５.抽出されたダウンリンクレンジからダウンリンク時間を算出する。
//６.アップリンク／ダウンリンクレンジから往復距離値を算出する。
//７.アップリンク／ダウンリンク時間からＲＴＴを算出する。
//<<2013-TURBO>>
//予報値フォーマットの解釈が誤っており、アップリンクレンジは当該時刻
//に送信した信号が衛星で折り返り、地上に到達するまでの距離の１／２で
//あることが解ったので、アップリンクレンジを２倍して求めることとする。
//
//備考     : XUP対応
//
//@param *stp_predict;      I : 展開した予報値テーブルのポインタ
//@param d_pred_t;          I : 求める予報値の時刻（積算秒）
//@param i_cal_flag;        I : RTT計算方法指定フラグ
//RTT_BASE_UP   = アップリンクベース
//RTT_BASE_DOWN = ダウンリンクベース
//@param *dp_range_rate;    O : 往復伝播距離変化率（ｍ／ｓ）
//@param *psz_offset_time;  I :時刻オフセット値格納位置ポインタ
//@param d_offset_freq;     I :周波数オフセット値
//@retval	１以上：ＲＴＴ時間（秒）
//０　　：ＲＴＴが１未満の場合、またはエラーの場合
//*/
///*============================================================================*/
///* デバッグ用マクロ宣言 */
///* #define		DEBUG1		/* エラー表示 */
///* #define		DEBUG2		/* 処理ルート */
///* #define		DEBUG3		/* 詳細データ */
//double CTrendData::lb_rttcal_rr(predata_t* stp_predict, double d_pred_t, int i_cal_flag, double* dp_range_rate, char* psz_offset_time, double d_offset_freq)
//
//
//{
//
//	int         i_ret;			/* 関数戻り値バッファ                   */
//	double      d_rng_up;		/* アップリンクレンジ（ｋｍ）           */
//	double      d_rng_down;		/* ダウンリンクレンジ（ｋｍ）           */
//	double      d_rr_up;		/* アップリンクレンジレート（ｍ／ｓ）   */
//	double      d_rr_down;		/* ダウンリンクレンジレート（ｍ／ｓ）   */
//	double      d_rr;			/* 往復距離値変化率（ｍ／ｓ）           */
//	double      d_rtt_up;		/* アップリンク時間（秒）               */
//	double      d_rtt_down;		/* ダウンリンク時間（秒）               */
//	double      d_rtt;			/* ＲＴＴ時間（秒）                     */
//	double      d_c;			/* 光速（Ｋｍ／Ｓ）                     */
//	one_predata_t   st_predict; /* 予報値情報                           */
//
//	d_c = V_C / 1000;                            /* 光速（Ｋｍ／Ｓ） */
//
//	/* 指定時刻の予報値を予報値テーブルから検索 */
//	i_ret = lb_prdcal(&st_predict, stp_predict, d_pred_t, 1, psz_offset_time, d_offset_freq);
//
//	if (i_ret != -1) {                         /* 成功した場合 */
//
//		switch (i_cal_flag) {
//
//		case RTT_BASE_UP:                   /* アップリンクベース */
//			/* アップリンクレンジ取得 */
//			d_rng_up = st_predict.d_predict_uplink_range;
//			d_rr_up = st_predict.d_predict_uplink_range_rate;
//			d_rtt_up = d_rng_up / d_c;         /* アップリンク時間算出 */
//
//			/* ダウンリンク帰着時刻の予報値を予報値テーブルから検索 */
//			i_ret = lb_prdcal(&st_predict, stp_predict, d_pred_t + (d_rtt_up * 2.0), 1, psz_offset_time, d_offset_freq);
//			if (i_ret == -1) {                 /* 抽出エラー */
//				d_rr = 0.0;
//				d_rtt = 0.0;
//			}
//			else {                              /* ダウンリンクレンジ取得 */
//				/*                d_rng_down = st_predict.d_predict_downlink_range ; *//* <<2013-TURBO>> 削除 */
//				d_rr_down = st_predict.d_predict_downlink_range_rate;
//				/*                d_rtt_down = d_rng_down / d_c ; *//* ダウンリンク時間算出 *//* <<2013-TURBO>> 削除 */
//
//				d_rr = (d_rr_up + d_rr_down) / (double)2.0;
//				/*                d_rtt = d_rtt_up + d_rtt_down ; *//* <<2013-TURBO>> 削除 */
//			}
//
//			/* <<2013-TURBO>> 計算方法変更 */
//			d_rtt = st_predict.d_predict_uplink_range * (double)2.0 / d_c;
//
//			break;
//
//		case RTT_BASE_DOWN:                   /* ダウンリンクベース */
//
//			/* <<2013-TURBO>> 旧フォーマットの場合は従来通り */
//			if (st_predict.d_predict_2way_rng < (double)0.0) {
//
//				/* ダウンリンクレンジ取得 */
//				d_rng_down = st_predict.d_predict_downlink_range;
//				d_rr_down = st_predict.d_predict_downlink_range_rate;
//				d_rtt_down = d_rng_down / d_c;     /* ダウンリンク時間算出 */
//
//				/* アップリンク送信時刻の予報値を予報値テーブルから検索 */
//				i_ret = lb_prdcal(&st_predict, stp_predict, d_pred_t - (d_rtt_down * 2), 1, psz_offset_time, d_offset_freq);
//				if (i_ret == -1) {                 /* 抽出エラー */
//					d_rr = 0.0;
//					d_rtt = 0.0;
//				}
//				else {                              /* アップリンクレンジ取得 */
//					d_rng_up = st_predict.d_predict_uplink_range;
//					d_rr_up = st_predict.d_predict_uplink_range_rate;
//					d_rtt_up = d_rng_up / d_c;     /* アップリンク時間算出 */
//
//					d_rr = (d_rr_up + d_rr_down) / (double)2.0;
//					d_rtt = d_rtt_up + d_rtt_down;
//				}
//			}
//			else {	/* <<2013-TURBO>> 新フォーマットの場合は２－ＷＡＹレンジ＆レンジレートを使用する */
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
//	*dp_range_rate = d_rr;                         /* 引数設定（往復伝播距離変化率／２） */
//
//	return(d_rtt);
//}
//
//
///*============================================================================*/
///*! CTrendData
//
//-# 予報値検索処理
//
//概要　　 : 展開した予報値テーブルから求めたい予報値を出力する。展開した予報値テー
//ブル内で求める予報値が無い場合は、直線補間を行う。
//また、求める予報値が展開した予報値テーブルよりも過去や未来の場合、展開した予報値テーブルの
//最初（過去の場合）や最後（未来の場合）を出力する。
//検索には二分探索法を適用する。
//
//@param	st_predict;     :求めた予報値の構造体のポインタ
//@param	gpst_predict;   :展開した予報値テーブルのポインタ
//@param	d_pred_t;        :求める予報値の時刻（積算秒）
//@param	i_sx;            :帯域（ｓ帯＝１、ｘ帯＝２、ｓｘ帯＝３）
//@param	szp_time_offset :考慮させる時刻オフセット値文字列
//書式：SMM:SS
//||| ||
//||| ++-- 秒を右詰、空いたところ
//|||      は０で埋める
//|||
//|++----- 分を右詰、空いたところ
//|        は０で埋める
//|
//+------- 符号、"+"は省略可。
//省略時はブランクを
//入れること。
//@param	d_freq_offset    :考慮させる周波数オフセット（Hz）
//
//@retval	０以上 : 検出した予報値テーブルの配列位置。
//補間した場合は過去側のポイントを設定する。
//-1     : エラー
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
//	/*	float f_bfr_data,f_aft_data; 2001-02-14 double に変更のため不要 */
//	double d_aft_data = 0.0;
//	double d_time_offset = 0.0;
//
//	int		j;
//	int		i_StartPoint;									/* 補間開始位置					*/
//	int		i_SampleNum;									/* 補間に使用するサンプル数		*/
//	//	int		i_Ret;
//
//	/* 2013-TURBO                   */
//	int     i_binsrch_num_total_pred;                    /* 予報値テーブルエントリ数     */
//	int     i_binsrch_min;                    /* 二分探索最小値               */
//	int     i_binsrch_max;                    /* 二分探索最大値               */
//	int     i_binsrch_mid;                    /* 二分探索中央値               */
//	/*
//	printf( "DEBUG 予報値件数 [%02x]\n", gpst_predict->predata[MAX_PRED_SIZE-1].ca_predict_s_tx_freq[0] ) ;
//	*/
//	/* 2013-TURBO                   */
//	/* 予報値テーブルエントリ数取得 */
//	if (gpst_predict->predata[MAX_PRED_SIZE - 1].ca_predict_s_tx_freq[0] != (char)NULL) {
//		i_binsrch_num_total_pred = 86400;
//
//		/*
//		printf( "DEBUG 予報値件数 i_binsrch_num_total_pred = [86400]\n" ) ;
//		*/
//
//
//
//	}
//	else {
//		i_binsrch_num_total_pred = (int)gpst_predict->predata[MAX_PRED_SIZE - 1].d_predict_2way_rng;
//
//		/*
//		printf( "DEBUG 予報値件数 i_binsrch_num_total_pred = [%d]\n", i_binsrch_num_total_pred ) ;
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
//	/* 帯域指定チェック */
//	if (i_sx != 1 && i_sx != 2 && i_sx != 3) {
//		/*
//		printf( "[%s](%d) :: 帯域指定チェックNG(%d)\n", __FUNCTION__, __LINE__, i_sx );
//		*/
//		return -1;
//	}
//
//	/* <<XUP>> 時刻オフセット引数が設定されている場合は、時刻オフセットを通算秒に変換 */
//	d_time_offset = 0.0;
//	if (szp_time_offset != NULL) {
//		d_time_offset = (double)(((szp_time_offset[1] - 0x30) * 10 + (szp_time_offset[2] - 0x30)) * 60 +
//			(szp_time_offset[4] - 0x30) * 10 + (szp_time_offset[5] - 0x30));
//		if (szp_time_offset[0] == '-') d_time_offset *= -1.0;
//	}
//
//	/* 指定時刻ＢＣＤからＡＳＣＩＩフォーマットに変換 <<XUP>> 時刻オフセットを考慮 */
//	CSatelliteData::ub_dtime_to_asc((d_pred_t - d_time_offset), sz_asctime);
//	sz_asctime[19] = '\0';
//	//	i_Ret = asc19_to_bcd7_time((unsigned char*)sz_asctime, uca_pred_t);
//	asc19_to_bcd7_time((unsigned char*)sz_asctime, uca_pred_t);
//
//	/*
//	printf( "DEBUG 先頭予報値時刻(BCD):[%02x%02x%02x%02x%02x%02x%02x]\n",gpst_predict->predata[0].uca_time[0],gpst_predict->predata[0].uca_time[1],
//	gpst_predict->predata[0].uca_time[2],gpst_predict->predata[0].uca_time[3],
//	gpst_predict->predata[0].uca_time[4],gpst_predict->predata[0].uca_time[5],
//	gpst_predict->predata[0].uca_time[6]) ;
//	printf( "DEBUG 目標予報値時刻(BCD):[%02x%02x%02x%02x%02x%02x%02x]\n",uca_pred_t[0],uca_pred_t[1],uca_pred_t[2],uca_pred_t[3],uca_pred_t[4],uca_pred_t[5],uca_pred_t[6] ) ;
//	*/
//
//
//
//
//	/* 求める予報値時刻と展開予報値（１件目）の時刻比較 */
//	i_cmp = memcmp(uca_pred_t, gpst_predict->predata[0].uca_time, 7);
//
//
//
//
//	/* 求める予報値時刻が、展開した予報値時刻（１件目）よりも前の場合 */
//	/* ★ MIN|-----------予報値--------------------------------|MAX   */
//	if (i_cmp < 0){
//
//
//
//		/*
//		printf( "DEBUG 求める予報値時刻が、展開した予報値時刻（１件目）よりも過去だった\n" ) ;
//		*/
//
//
//
//		memcpy((unsigned char *)st_predict, (unsigned char *)&(gpst_predict->predata[0]), sizeof(one_predata_t));
//		/* 選択されたランプレート値を０にする */
//		lb_ramp0(st_predict, i_sx);
//
//		/* 終了 <<XUP>> 戻り値に検出した配列位置を設定 */
//		/*
//		printf( "[%s](%d) :: 終了 <<XUP>> 戻り値に検出した配列位置を設定\n", __FUNCTION__, __LINE__ );
//		*/
//		return 0;
//	}
//
//	/* 求める予報値時刻が、展開した予報値時刻（１件目）と一致 */
//	/*  MIN★-----------予報値--------------------------------|MAX   */
//	if (i_cmp == 0){
//
//
//
//		/*
//		printf( "DEBUG 求める予報値時刻が、展開した予報値時刻（１件目）と一致\n" ) ;
//		*/
//
//
//
//		lb_prdcal_hit(0, st_predict, gpst_predict, d_pred_t, i_sx, d_freq_offset);
//
//		/* 選択されたランプレート値を０にする */
//		lb_ramp0(st_predict, i_sx);
//
//		/* 終了 <<XUP>> 戻り値に検出した配列位置を設定 */
//		/*
//		printf( "[%s](%d) :: 終了 <<XUP>> 戻り値に検出した配列位置を設定\n", __FUNCTION__, __LINE__ );
//		*/
//		return 0;
//	}
//
//
//	/* 求める予報値時刻と展開予報値（最終件目）の時刻比較 2013-TURBO */
//	i_cmp = memcmp(uca_pred_t, gpst_predict->predata[i_binsrch_max - 1].uca_time, 7);
//
//	/*
//	printf( "DEBUG 最終予報値時刻(BCD):[%02x%02x%02x%02x%02x%02x%02x]\n",gpst_predict->predata[i_binsrch_max-1].uca_time[0],gpst_predict->predata[i_binsrch_max-1].uca_time[1],
//	gpst_predict->predata[i_binsrch_max-1].uca_time[2],gpst_predict->predata[i_binsrch_max-1].uca_time[3],
//	gpst_predict->predata[i_binsrch_max-1].uca_time[4],gpst_predict->predata[i_binsrch_max-1].uca_time[5],
//	gpst_predict->predata[i_binsrch_max-1].uca_time[6]) ;
//	*/
//
//	/* 求める予報値時刻が、展開した予報値時刻（最終件目）と一致 */
//	/*  MIN|-----------予報値--------------------------------★MAX   */
//	if (i_cmp == 0){
//
//
//
//		/*
//		printf( "DEBUG 求める予報値時刻が、展開した予報値時刻（最終件目）と一致\n" ) ;
//		*/
//
//
//
//		lb_prdcal_hit((i_binsrch_max - 1), st_predict, gpst_predict, d_pred_t, i_sx, d_freq_offset);
//
//		/* 選択されたランプレート値を０にする */
//		lb_ramp0(st_predict, i_sx);
//
//		/* 終了 <<XUP>> 戻り値に検出した配列位置を設定 */
//		/*
//		printf( "[%s](%d) :: 終了 <<XUP>> 戻り値に検出した配列位置を設定\n", __FUNCTION__, __LINE__ );
//		*/
//		return 0;
//	}
//
//
//	/* 求める予報値時刻が、展開した予報値時刻（最終件目）よりも未来の場合 */
//	/*    MIN|-----------予報値--------------------------------|MAX ★    */
//	if (i_cmp > 0){
//
//
//
//		/*
//		printf( "DEBUG 求める予報値時刻が、展開した予報値時刻（最終件目）よりも未来だった\n" ) ;
//		*/
//
//
//
//		/* 求める予報値時刻が、展開した予報値時刻よりも後の場合 */
//		memcpy((unsigned char *)st_predict, (unsigned char *)&(gpst_predict->predata[i_binsrch_max - 1]), sizeof(one_predata_t));
//		/* 選択されたランプレート値を０にする */
//		lb_ramp0(st_predict, i_sx);
//		/* 終了 <<XUP>> 戻り値に検出した配列位置を設定 */
//		/*
//		printf( "[%s](%d) :: 終了 <<XUP>> 戻り値に検出した配列位置を設定(%d)\n", __FUNCTION__, __LINE__, i_binsrch_max-1 );
//		*/
//		return (i_binsrch_max - 1);
//	}
//
//
//
//
//
//	/* 求める予報値時刻が、展開した予報値時刻（１件目）よりも後の場合       */
//	/* 2013-TURBO -- 二分探索開始 ------------------------------------------*/
//
//
//
//	/*
//	printf( "DEBUG \n" ) ;
//	printf( "DEBUG 二分探索開始\n" ) ;
//	printf( "DEBUG \n" ) ;
//	*/
//
//
//
//	while (i_binsrch_min <= i_binsrch_max) {
//
//
//		/* 中央値算出                                                         */
//		/*       |---MIN|-------------中央値----------|MAX---------|          */
//		i_binsrch_mid = (i_binsrch_min + i_binsrch_max) / 2;
//
//
//		/*
//		printf( "DEBUG \n" ) ;
//		printf( "DEBUG \n" ) ;
//		printf( "DEBUG 中央値算出:i_binsrch_mid:[%d]\n",i_binsrch_mid ) ;
//		*/
//
//
//
//		/* 求める予報値時刻と展開予報値（中央値）の時刻比較 */
//		i_cmp = memcmp(uca_pred_t, gpst_predict->predata[i_binsrch_mid - 1].uca_time, 7);
//
//
//
//		/*
//		printf( "DEBUG 中央予報値時刻(BCD):[%02x%02x%02x%02x%02x%02x%02x]\n",gpst_predict->predata[i_binsrch_mid-1].uca_time[0],gpst_predict->predata[i_binsrch_mid-1].uca_time[1],
//		gpst_predict->predata[i_binsrch_mid-1].uca_time[2],gpst_predict->predata[i_binsrch_mid-1].uca_time[3],
//		gpst_predict->predata[i_binsrch_mid-1].uca_time[4],gpst_predict->predata[i_binsrch_mid-1].uca_time[5],
//		gpst_predict->predata[i_binsrch_mid-1].uca_time[6]) ;
//		printf( "DEBUG 目標予報値時刻(BCD):[%02x%02x%02x%02x%02x%02x%02x]\n",uca_pred_t[0],uca_pred_t[1],uca_pred_t[2],uca_pred_t[3],uca_pred_t[4],uca_pred_t[5],uca_pred_t[6] ) ;
//		*/
//
//
//
//		/* 求める予報値時刻が、展開した予報値時刻（中央値）と同じ場合         */
//		/*       |---MIN|-------------中★値----------|MAX---------|          */
//		if (i_cmp == 0){
//
//			i = (i_binsrch_mid - 1);
//
//
//			/*
//			printf( "DEBUG 目標予報値時刻と中央値予報値時刻が一致:[%d]\n",i ) ;
//			*/
//
//
//			lb_prdcal_hit(i, st_predict, gpst_predict, d_pred_t, i_sx, d_freq_offset);
//
//			if (strlen((char *)gpst_predict->predata[i + 1].uca_time) == 0){
//				/* 選択されたランプレート値を０にする */
//				lb_ramp0(st_predict, i_sx);
//			}
//
//			/* 終了 <<XUP>> 戻り値に検出した配列位置を設定 */
//			/*
//			printf( "[%s](%d) :: 終了 <<XUP>> 戻り値に検出した配列位置を設定(%d)\n", __FUNCTION__, __LINE__, i );
//			*/
//			return i;
//
//
//
//
//		}
//		/* 求める予報値時刻が、展開した予報値時刻（中央値）よりも未来にある場合 */
//		/*       |------MIN|----------中央値---★-----|MAX---------|            */
//		else if (i_cmp > 0){
//
//
//			/*
//			printf( "DEBUG 目標予報値時刻が中央値予報値時刻より未来にある\n" ) ;
//			*/
//
//
//			/* 中央値と最大値が隣り合った(中央値と最大値の間に目標時刻がある) */
//			/*       |------MIN|----------中央値★|MAX-----------------|      */
//			if (i_binsrch_max == (i_binsrch_mid + 1)) {
//				i = (i_binsrch_max - 1);
//
//
//				/*
//				printf( "DEBUG 中央値と最大値が隣り合った(中央値と最大値の間に目標時刻がある)\n" ) ;
//				printf( "DEBUG 最大値:[%d]をｉとしてループ離脱\n", i ) ;
//				*/
//
//
//				break;
//			}
//			else {
//				/* 中央値と最大値が隣り合ってないため継続して検索                 */
//				/*       |---------|-------MIN|------------|MAX---------|         */
//				i_binsrch_min = i_binsrch_mid;
//
//
//				/*
//				printf( "DEBUG 新たな最小値:i_binsrch_min:[%d]\n", i_binsrch_min ) ;
//				*/
//
//
//			}
//		}
//		/* 求める予報値時刻が、展開した予報値時刻（中央値）よりも過去にある場合 */
//		/*       |---MIN|------★------中央値---------|MAX---------|            */
//		else if (i_cmp < 0){
//
//
//			/*
//			printf( "DEBUG 目標予報値時刻が中央値予報値時刻より過去にある\n" ) ;
//			*/
//
//
//			/* 中央値と最小値が隣り合った(中央値と最小値の間に目標時刻がある) */
//			/*       |------MIN|★中央値----------|MAX-----------------|      */
//			if (i_binsrch_min == (i_binsrch_mid - 1)) {
//				i = (i_binsrch_mid - 1);
//
//
//				/*
//				printf( "DEBUG 中央値と最小値が隣り合った(中央値と最小値の間に目標時刻がある)\n" ) ;
//				printf( "DEBUG 中央値:[%d]をｉとしてループ離脱\n", i ) ;
//				*/
//
//
//				break;
//			}
//			else {
//				/* 中央値と最小値が隣り合ってないため継続して検索                 */
//				/*       |------MIN|----------|MAX-----|-------------------|      */
//				i_binsrch_max = i_binsrch_mid;
//
//
//				/*
//				printf( "DEBUG 新たな最大値:i_binsrch_max:[%d]\n", i_binsrch_max ) ;
//				*/
//
//
//			}
//		}
//	}
//	/* 2013-TURBO -- 二分探索終了 ------------------------------------------*/
//
//	/*
//	printf( "DEBUG 二分探索終了 ｉ:[%d]\n", i ) ;
//	*/
//	/* 前後に十分データがある場合 */
//	if (i >= (SAMPLING_NUM / 2)) {
//
//		/* 補間開始位置設定 */
//		i_StartPoint = i - SAMPLING_NUM / 2;
//	}
//
//	/* 前が少ない場合 */
//	else if (i < (SAMPLING_NUM / 2)) {
//
//		/* 補間開始位置設定（先頭を設定） */
//		i_StartPoint = 0;
//	}
//
//	/* ラグランジェ補間用配列設定 */
//	/* 次の位置を取得 */
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
//	補間に使用する配列を決める。
//	通常は目標時刻の前後2ポイントを使用するが、これが取れない場合は前後にシフトする。
//	***/
//
//	/* 前後に十分データがある場合 */
//	if (j >= (i + SAMPLING_NUM)) {
//
//		/* サンプル数設定 */
//		i_SampleNum = SAMPLING_NUM;
//	}
//
//	/* 後ろが少ない場合（その場合[j]は配列の終端+1を指しているはず） */
//	else {
//
//		/* サンプル数設定（足りない場合） */
//		i_SampleNum = j - i + 1;
//	}
//
//	/*アジマス*/
//	st_predict->d_predict_az = lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_az);
//
//	/*エレベーション*/
//	st_predict->d_predict_el = lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_el);
//
//	/*アップリンクレンジ*/
//	st_predict->d_predict_uplink_range = lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_uplink_range);
//
//	/*ダウンリンクレンジ*/
//	st_predict->d_predict_downlink_range = lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_downlink_range);
//
//	/*アップリンクレンジレート* 2001-02-10 このデータは補間の必要なし */
//	/*                          2001-10-04 やっぱり復活させることにした */
//	st_predict->d_predict_uplink_range_rate = lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_uplink_range_rate);
//	/*	st_predict->d_predict_uplink_range_rate = gpst_predict->predata[i-1].d_predict_uplink_range_rate;
//	*/
//	/*ダウンリンクレンジレート* 2001-02-10 このデータは補間の必要なし */
//	/*                          2001-10-04 やっぱり復活させることにした */
//	st_predict->d_predict_downlink_range_rate = lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_downlink_range_rate);
//	/*	st_predict->d_predict_downlink_range_rate = gpst_predict->predata[i-1].d_predict_downlink_range_rate;
//	*/
//
//	/* ２ＷＡＹレンジ <<2013-TURBO>> */
//	st_predict->d_predict_2way_rng = lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_2way_rng);
//
//	/* ２ＷＡＹレンジレート <<2013-TURBO>> */
//	st_predict->d_predict_2way_range_rate = lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_2way_range_rate);
//
//	/*予測送信周波数*/
//	/* <<XUP>> 周波数オフセットを考慮 */
//	sprintf_s(st_predict->ca_predict_s_tx_freq, PRED_FREQ_LEN, "%.5lf", lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_s_tx_freq));
//
//	if (i_sx == 1){
//		/*1WAY予測受信周波数(s-band)*/
//		/* <<XUP>> 周波数オフセットを考慮 */
//		sprintf_s(st_predict->ca_predict_s_rx_1way_freq, PRED_FREQ_LEN, "%.5lf", lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_s_rx_1way_freq));
//		/*1WAY予測受信周波数(x-band)*/
//		st_predict->ca_predict_x_rx_1way_freq[0] = '\0';
//
//		/*2WAY予測受信周波数(s-band)*/
//		/* <<XUP>> 周波数オフセットを考慮 */
//		sprintf_s(st_predict->ca_predict_s_rx_2way_freq, PRED_FREQ_LEN, "%.5lf", lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_s_rx_2way_freq));
//		/*2WAY予測受信周波数(x-band)*/
//		st_predict->ca_predict_x_rx_2way_freq[0] = '\0';
//
//		/*1WAYランプレート(S-BAND)*/
//		strcpy_s(st_predict->ca_predict_s_rx_1way_ramprate, PRED_RAMP_LEN, gpst_predict->predata[i - 1].ca_predict_s_rx_1way_ramprate);
//		/*1WAYランプレート(X-BAND)*/
//		st_predict->ca_predict_x_rx_1way_ramprate[0] = '\0';
//
//		/*2WAYランプレート(S-BAND)*/
//		strcpy_s(st_predict->ca_predict_s_rx_2way_ramprate, PRED_RAMP_LEN, gpst_predict->predata[i - 1].ca_predict_s_rx_2way_ramprate);
//		/*2WAYランプレート(X-BAND)*/
//		st_predict->ca_predict_x_rx_2way_ramprate[0] = '\0';
//	}
//	else if (i_sx == 2){
//		/*1WAY予測受信周波数(s-band)*/
//		st_predict->ca_predict_s_rx_1way_freq[0] = '\0';
//		/*1WAY予測受信周波数(x-band)*/
//		/* <<XUP>> 周波数オフセットを考慮 */
//		sprintf_s(st_predict->ca_predict_x_rx_1way_freq, PRED_FREQ_LEN, "%.5lf", lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_x_rx_1way_freq));
//
//		/*2WAY予測受信周波数(s-band)*/
//		st_predict->ca_predict_s_rx_2way_freq[0] = '\0';
//		/*2WAY予測受信周波数(x-band)*/
//		/* <<XUP>> 周波数オフセットを考慮 */
//		sprintf_s(st_predict->ca_predict_x_rx_2way_freq, PRED_FREQ_LEN, "%.5lf", lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_x_rx_2way_freq));
//
//		/*1WAYランプレート(S-BAND)*/
//		st_predict->ca_predict_s_rx_1way_ramprate[0] = '\0';
//		/*1WAYランプレート(X-BAND)*/
//		strcpy_s(st_predict->ca_predict_x_rx_1way_ramprate, PRED_RAMP_LEN, gpst_predict->predata[i - 1].ca_predict_x_rx_1way_ramprate);
//		/*2WAYランプレート(S-BAND)*/
//		st_predict->ca_predict_s_rx_2way_ramprate[0] = '\0';
//		/*2WAYランプレート(X-BAND)*/
//		strcpy_s(st_predict->ca_predict_x_rx_2way_ramprate, PRED_RAMP_LEN, gpst_predict->predata[i - 1].ca_predict_x_rx_2way_ramprate);
//	}
//	else if (i_sx == 3){
//		/*1WAY予測受信周波数(s-band)*/
//		/* <<XUP>> 周波数オフセットを考慮 */
//		sprintf_s(st_predict->ca_predict_s_rx_1way_freq, PRED_FREQ_LEN, "%.5lf", lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_s_rx_1way_freq));
//
//		/*1WAY予測受信周波数(x-band)*/
//		/* <<XUP>> 周波数オフセットを考慮 */
//		sprintf_s(st_predict->ca_predict_x_rx_1way_freq, PRED_FREQ_LEN, "%.5lf", lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_x_rx_1way_freq));
//
//		/*2WAY予測受信周波数(s-band)*/
//		/* <<XUP>> 周波数オフセットを考慮 */
//		sprintf_s(st_predict->ca_predict_s_rx_2way_freq, PRED_FREQ_LEN, "%.5lf", lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_s_rx_2way_freq));
//
//		/*2WAY予測受信周波数(x-band)*/
//		/* <<XUP>> 周波数オフセットを考慮 */
//		sprintf_s(st_predict->ca_predict_x_rx_2way_freq, PRED_FREQ_LEN, "%.5lf", lb_lagrange(i_StartPoint, i_SampleNum, (d_pred_t - d_time_offset), m_lla_time, m_da_predict_x_rx_2way_freq));
//
//		/*1WAYランプレート(S-BAND)*/
//		strcpy_s(st_predict->ca_predict_s_rx_1way_ramprate, PRED_RAMP_LEN, gpst_predict->predata[i - 1].ca_predict_s_rx_1way_ramprate);
//		/*1WAYランプレート(X-BAND)*/
//		strcpy_s(st_predict->ca_predict_x_rx_1way_ramprate, PRED_RAMP_LEN, gpst_predict->predata[i - 1].ca_predict_x_rx_1way_ramprate);
//		/*2WAYランプレート(S-BAND)*/
//		strcpy_s(st_predict->ca_predict_s_rx_2way_ramprate, PRED_RAMP_LEN, gpst_predict->predata[i - 1].ca_predict_s_rx_2way_ramprate);
//		/*2WAYランプレート(X-BAND)*/
//		strcpy_s(st_predict->ca_predict_x_rx_2way_ramprate, PRED_RAMP_LEN, gpst_predict->predata[i - 1].ca_predict_x_rx_2way_ramprate);
//	}
//	/* 終了 <<XUP>> 戻り値に検出した配列位置を設定 */
//	/*
//	printf( "DEBUG 戻り値に検出した配列位置を設定:[%d]\n", (i-1) ) ;
//	*/
//	printf("[%s](%d) :: 戻り値に検出した配列位置を設定:[%d]\n", __FUNCTION__, __LINE__, i - 1);
//	return (i - 1);
//
//}
//
//
///*============================================================================*/
///*! CTrendData
//
//-# ランプレートを初期化する
//
//@param	st_predict
//@param	i_sx
//@retval	なし
//
//*/
///*============================================================================*/
//void CTrendData::lb_ramp0(one_predata_t* st_predict, int i_sx)
//{
//	if (i_sx == 1){
//		sprintf_s(st_predict->ca_predict_s_rx_1way_ramprate, PRED_RAMP_LEN, "0.00000"); /*1WAYランプレート(S-BAND)*/
//		st_predict->ca_predict_x_rx_1way_ramprate[0] = '\0';            /*1WAYランプレート(X-BAND)*/
//		sprintf_s(st_predict->ca_predict_s_rx_2way_ramprate, PRED_RAMP_LEN, "0.00000"); /*2WAYランプレート(S-BAND)*/
//		st_predict->ca_predict_x_rx_2way_ramprate[0] = '\0';            /*2WAYランプレート(X-BAND)*/
//	}
//	else if (i_sx == 2){
//		st_predict->ca_predict_s_rx_1way_ramprate[0] = '\0';            /*1WAYランプレート(S-BAND)*/
//		sprintf_s(st_predict->ca_predict_x_rx_1way_ramprate, PRED_RAMP_LEN, "0.00000"); /*1WAYランプレート(X-BAND)*/
//		st_predict->ca_predict_s_rx_2way_ramprate[0] = '\0';            /*2WAYランプレート(S-BAND)*/
//		sprintf_s(st_predict->ca_predict_x_rx_2way_ramprate, PRED_RAMP_LEN, "0.00000"); /*2WAYランプレート(X-BAND)*/
//	}
//	else if (i_sx == 3){
//		sprintf_s(st_predict->ca_predict_s_rx_1way_ramprate, PRED_RAMP_LEN, "0.00000"); /*1WAYランプレート(S-BAND)*/
//		sprintf_s(st_predict->ca_predict_x_rx_1way_ramprate, PRED_RAMP_LEN, "0.00000"); /*1WAYランプレート(X-BAND)*/
//		sprintf_s(st_predict->ca_predict_s_rx_2way_ramprate, PRED_RAMP_LEN, "0.00000"); /*2WAYランプレート(S-BAND)*/
//		sprintf_s(st_predict->ca_predict_x_rx_2way_ramprate, PRED_RAMP_LEN, "0.00000"); /*2WAYランプレート(X-BAND)*/
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
//@retval	設定した配列位置(i)
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
//	/* 展開した予報値を出力 */
//	memcpy((unsigned char *)st_predict, (unsigned char *)&(gpst_predict->predata[i]), sizeof(one_predata_t));
//
//	/* <<XUP>> 時刻オフセット考慮前の時刻に書き換える */
//	CSatelliteData::ub_dtime_to_asc(d_pred_t, sz_asctime);
//	sz_asctime[19] = '\0';
//	//	i_Ret = asc19_to_bcd7_time((unsigned char*)sz_asctime, st_predict->uca_time);
//	asc19_to_bcd7_time((unsigned char*)sz_asctime, st_predict->uca_time);
//
//	/* <<XUP>> 予測送信周波数に周波数オフセットを考慮 */
//	sscanf_s(gpst_predict->predata[i].ca_predict_s_tx_freq, "%lf", &d_aft_data);
//	sprintf_s(st_predict->ca_predict_s_tx_freq, PRED_FREQ_LEN, "%.5lf", (d_aft_data + d_freq_offset));
//
//	if (i_sx == 1){
//		/*1WAY予測受信周波数(x-band)*/
//		st_predict->ca_predict_x_rx_1way_freq[0] = '\0';
//		/*2WAY予測受信周波数(x-band)*/
//		st_predict->ca_predict_x_rx_2way_freq[0] = '\0';
//		/*1WAYランプレート(X-BAND)*/
//		st_predict->ca_predict_x_rx_1way_ramprate[0] = '\0';
//		/*2WAYランプレート(X-BAND)*/
//		st_predict->ca_predict_x_rx_2way_ramprate[0] = '\0';
//
//		/*1WAY予測受信周波数(s-band) <<XUP>> 周波数オフセットを考慮 */
//		sscanf_s(gpst_predict->predata[i].ca_predict_s_rx_1way_freq, "%lf", &d_aft_data);
//		sprintf_s(st_predict->ca_predict_s_rx_1way_freq, PRED_FREQ_LEN, "%.5lf", (d_aft_data + d_freq_offset));
//		/*2WAY予測受信周波数(s-band) <<XUP>> 周波数オフセットを考慮 */
//		sscanf_s(gpst_predict->predata[i].ca_predict_s_rx_2way_freq, "%lf", &d_aft_data);
//		sprintf_s(st_predict->ca_predict_s_rx_2way_freq, PRED_FREQ_LEN, "%.5lf", (d_aft_data + d_freq_offset));
//	}
//	else if (i_sx == 2){
//		/*1WAY予測受信周波数(s-band)*/
//		st_predict->ca_predict_s_rx_1way_freq[0] = '\0';
//		/*2WAY予測受信周波数(s-band)*/
//		st_predict->ca_predict_s_rx_2way_freq[0] = '\0';
//		/*1WAYランプレート(S-BAND)*/
//		st_predict->ca_predict_s_rx_1way_ramprate[0] = '\0';
//		/*2WAYランプレート(S-BAND)*/
//		st_predict->ca_predict_s_rx_2way_ramprate[0] = '\0';
//
//		/*1WAY予測受信周波数(x-band) <<XUP>> 周波数オフセットを考慮 */
//		sscanf_s(gpst_predict->predata[i].ca_predict_x_rx_1way_freq, "%lf", &d_aft_data);
//		sprintf_s(st_predict->ca_predict_x_rx_1way_freq, PRED_FREQ_LEN, "%.5lf", (d_aft_data + d_freq_offset));
//		/*2WAY予測受信周波数(x-band) <<XUP>> 周波数オフセットを考慮 */
//		sscanf_s(gpst_predict->predata[i].ca_predict_x_rx_2way_freq, "%lf", &d_aft_data);
//		sprintf_s(st_predict->ca_predict_x_rx_2way_freq, PRED_FREQ_LEN, "%.5lf", (d_aft_data + d_freq_offset));
//	}
//	else if (i_sx == 3){	/* <<XUP>> S/Xのケースを追加 */
//		/*1WAY予測受信周波数(s-band) <<XUP>> 周波数オフセットを考慮 */
//		sscanf_s(gpst_predict->predata[i].ca_predict_s_rx_1way_freq, "%lf", &d_aft_data);
//		sprintf_s(st_predict->ca_predict_s_rx_1way_freq, PRED_FREQ_LEN, "%.5lf", (d_aft_data + d_freq_offset));
//		/*2WAY予測受信周波数(s-band) <<XUP>> 周波数オフセットを考慮 */
//		sscanf_s(gpst_predict->predata[i].ca_predict_s_rx_2way_freq, "%lf", &d_aft_data);
//		sprintf_s(st_predict->ca_predict_s_rx_2way_freq, PRED_FREQ_LEN, "%.5lf", (d_aft_data + d_freq_offset));
//
//		/*1WAY予測受信周波数(x-band) <<XUP>> 周波数オフセットを考慮 */
//		sscanf_s(gpst_predict->predata[i].ca_predict_x_rx_1way_freq, "%lf", &d_aft_data);
//		sprintf_s(st_predict->ca_predict_x_rx_1way_freq, PRED_FREQ_LEN, "%.5lf", (d_aft_data + d_freq_offset));
//		/*2WAY予測受信周波数(x-band) <<XUP>> 周波数オフセットを考慮 */
//		sscanf_s(gpst_predict->predata[i].ca_predict_x_rx_2way_freq, "%lf", &d_aft_data);
//		sprintf_s(st_predict->ca_predict_x_rx_2way_freq, PRED_FREQ_LEN, "%.5lf", (d_aft_data + d_freq_offset));
//	}
//
//	/*
//	printf( "[%s](%d) :: 戻り値に検出した配列位置を設定:[%d]\n", __FUNCTION__, __LINE__, i );
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
//-# 引数により指定された点数で、補間点を計算する。
//
//@param	i_dynamic_counter  ：配列上のサンプルポイント開始位置
//@param	n                  ：サンプリング数
//@param	t                  ：補間により求めたい位置(X)
//@param	i_in_time[]        ：補間元のデータ配列(X)
//@param	d_input_parameter[]：補間元のデータ配列(Y)
//@retval	                   ：補間結果(Y)
//
//*/
///*============================================================================*/
//double CTrendData::lb_lagrange(int i_dynamic_counter, int n, double t, time_t i_in_time[], double d_input_parameter[])
//{
//	int	ii, jj;               /* ループカウンター */
//	double	d_sum;            /*  */
//	double	d_prod;           /* 係数 */
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

-# 引数により指定された点数で、補間点を計算する。

@param	i_dynamic_counter  ：配列上のサンプルポイント開始位置
@param	n                  ：サンプリング数
@param	t                  ：補間により求めたい位置(X)
@param	i_in_time[]        ：補間元のデータ配列(X)
@param	d_input_parameter[]：補間元のデータ配列(Y)
@retval	                   ：補間結果(Y)

*/
/*============================================================================*/
double CTrendData::CalcLagrange(int i_dynamic_counter, int n, double t, time_t i_in_time[], double d_input_parameter[])
{
	int	ii, jj;               /* ループカウンター */
	double	d_sum;            /*  */
	double	d_prod;           /* 係数 */

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

-# 予報値データ補間処理

@param	t:現在時刻
@param	idx:インデックス位置
@param	datalist:データリスト
@retval
/*============================================================================*/
double CTrendData::InterpolatePrd(double t, __int64* idx, CSpaceData* data)
{
	//	// 時刻がAOS前の場合、最初の値を使う
	//	// 時刻がLOS後の場合、最魚の値を使う
	// 時刻がAOS～LOSの間の場合、予報値データテーブルから対象データのインデックスを探す
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
	// インデックス更新
	*idx = ii;

	// インデックスの前後２個ずつを補間元データとする
	if (ii < 2)
	{
		// リスト先頭付近
		// インデックスが2より小さい場合は、先頭２つのインデックスを使う
		data_pos = 0;
	}
	else if (ii > size - 1 - 2)
	{
		// リスト終端付近
		// インデックスが最終位置２つ前より大きい場合は、最終インデックスから４つを使う
		if (size > 4)
		{
			// 予報値が4個よりも大きい場合
			data_pos = size - 4;
		}
		else
		{
			// 予報値が4個以下
			data_pos = 0;
		}
	}
	else
	{
		// それ以外はインデックス位置から2つ前を使う
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

	// 補間用データのサンプリング処理
	for (_int64 i = 0; i < LGR_SMP_NUM; i++)
	{
		i_in_time[i] = CTime(data->value[data_pos + i].systime).GetTime();
		d_input_parameter[i] = data->value[data_pos + i].value;
	}

	// サンプリングデータを使用し、補間する
	return CalcLagrange(0, LGR_SMP_NUM, t, i_in_time, d_input_parameter);
}


/*============================================================================*/
/*! CTrendData

-# 衛星までの往復距離を直線補間してマップに追加する

@param	t1
@param	rng1
@param	t2
@param	rng2
@retval	なし

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

-# 衛星までの往復距離の取得
予報値から、指定時刻の衛星までの往復距離を取得する。
予報値ファイルにちょうどの時刻が無い場合は、直線補間して求める

@param	t：指定時刻
@retval	往復距離

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
	// 範囲にある場合
	if (itr != m_2WayRngMap.end())
	{
		if ((*itr).first == t)
		{
			// 予報値マップに時刻がある場合は、マップの値を返す
			return m_2WayRngMap[t];
		}
		else if (itr == m_2WayRngMap.begin())
		{
			// １番目より前のため、１番目と２番目の予報値で補間
			auto itr1 = itr;
			itr++;
			auto itr2 = itr;
			return GetLinearInterpolation((*itr1).first, (*itr1).second, (*itr2).first, (*itr2).second, t);
		}
		else
		{
			// ひとつ前の予報値と検索した予報値で補間
			auto itr2 = itr;
			itr--;
			auto itr1 = itr;
			return GetLinearInterpolation((*itr1).first, (*itr1).second, (*itr2).first, (*itr2).second, t);
		}
	}
	// 範囲に無い場合
	else if (m_2WayRngMap.size() >= 2)
	{
		// 最後より１つ前の予報値と最後の予報値で補間
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
// 色設定
COLORREF Spectrum_colText[5] = {
	RGB(0, 0, 0),		// 黒
	RGB(255, 165, 0),	// オレンジ
	RGB(0, 0, 255),		// 青
	RGB(255, 0, 0),		// 赤
	RGB(0, 128, 0),		// 緑
};

// 単位
CString Spectrum_strFreqUnit[3] = {
	_T("[Hz]"),
	_T("[kHz]"),
	_T("[MHz]"),
};

// 中心周波数
CString gsz_center_str[1] = { _T("70MHz") };


/* ------------------------------------------------------------------------------------ */
/* スペクトラム表示データ
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

-# スペクトラムデータ初期化時に呼ばれる

@param	nStationID：衛星ID
@param	nViewID：ビューID
@param	nGraphType：グラフタイプ
@retval	なし

*/
/*============================================================================*/
void CSpectrumViewData::InitializeData(UINT nGraphType)
{
	// 設定初期化
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

-# データファイル読み込み処理

@param  fpファイルポインタ
@param  nStationID：衛星ID
@param  nViewID：ビューID
@param  nGraphType：グラフタイプ
@retval 0：正常終了 / -1：異常終了
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

	// ヘッダを探す
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

	/* オートスケール */
	i_ret = CSatelliteData::GetConfFromFile(fp, SPECTRUM_FILE_AUTO_SCALE, &l_val);
	if (i_ret <= 0){
		//		fclose(fp);
		//		return(ERR_RETURN);
		m_stVScale.bAutoScale = FALSE;
	}
	else {
		m_stVScale.bAutoScale = (BOOL)l_val;
	}

	/* グリッド最大値 */
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
	/* グリッド最小値 */
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
	/* グリッド値識別 */
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
	/* グリッドオフセット値識別 */
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

-# データファイル書き込み処理

@param  fp：ファイルポインタ
@param  nStationID：衛星ID
@param  nViewID：ビューID
@param  nGraphType：グラフタイプ
@retval 0：正常終了 / -1：異常終了
*/
/*============================================================================*/
int CSpectrumViewData::FileWrite(FILE* fp, UINT nStationID, UINT nViewID, UINT nGraphType)
{
	/*------------------*/
	/* ファイル名取得   */
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

-# 監視データ値取得

@param	なし
@retval	1：取得成功 / 0：取得失敗

*/
/*============================================================================*/
int CSpectrumViewData::GetSpectrumData()
{
#ifdef _ENGVALCNV_TREND

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();
	int ret = 0;

	UINT sta;
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		// １局画面の場合は、自局番号
		sta = theApp.GetSelectStation();
	}
	else
	{
		// それ以外の場合は、指定局番
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
	// ダミーデータをセット
	CSpectrumViewData* pCSectrumData = &theApp.GetSatelliteData().GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return FALSE;
	}

	// ダミーデータ生成
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

-# ダミーデータを生成する

@param	なし
@retval	なし

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

-# ダミーオフセットを生成する

@param	なし
@retval	なし

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
/* 追尾レーダー制御監視データクラス
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

-# データ更新時に呼ばれる

@param	nIDEvent：イベントID
@retval	なし

*/
/*============================================================================*/
void CTrackingRadarCtrlMonData::UpdateData()
{
//	int a = m_nViewID;
	if (m_nStationID < 0)
		return;

	/*--------------------------*/
	/*  毎秒変化する情報の取得  */
	/*--------------------------*/
	// サマリ画面で局がある場合は監視データ取得
	if (m_nViewID < eStation_MAX)
	{
		/* 運用状況(監視データ）の取得＆判別 */
		if (GetKanshiData() != 0)
		{
			// 取得失敗
			return;
		}
	}

	/*----------------------------*/
	/*  プロセス起動時の初期表示  */
	/*----------------------------*/
	if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {
		Gtinit();
		m_gst_dspinf.i_dsp_type = TYPE_TRND;
	}

	/*--------------------------*/
	/*  トレンド（継続）表示時  */
	/*--------------------------*/
	else {
		/*** 監視データ取得状況を判断 ***/
		if (m_gst_dspinf.i_mondat_NGcnt <= 0) {
			/*** 表示切替り発生または予報値取得待ち解除時
			かつ予報値取得可能時、は再描画する ***/
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
//-# 運用状況判別処理 
//
//@param	なし
//@retval	なし
//
//*/
///*============================================================================*/
//int CTrackingRadarCtrlMonData::Gtunyou()
//{
//	// サマリ画面で局がある場合は監視データ取得
//	if (m_nViewID < eStation_MAX)
//	{
//		return GetKanshiData();
//	}
//	// 予報値一覧や計画一覧の場合は、監視データは使わずに直接ファイルから取得
//	else
//	{
//		return GetFileData();
//	}
//}


/*============================================================================*/
/*! CTrackingRadarCtrlMonData

-# 監視データ取得

@param	なし
@retval	なし

*/
/*============================================================================*/
int CTrackingRadarCtrlMonData::GetKanshiData()
{
	int			i_unyou;			/* パス運用状況 				  */
	long		d_time;				/* パス運用開始までの時間 		  */
	char		ca_stime[30];		/* パス開始時刻                   */
	char		ca_etime[30];		/* パス終了時刻                   */
	TCHAR		ca_fpath[1024];		/* 予報値ファイル名（フルパス）   */
	char		ca_ftime[30];		/* 予報値ファイル更新日時		  */
	char		ca_plnno[30];		/* 計画番号                       */
	char		ca_eisei[100];		/* 衛星名       			      */
	char		ca_AOS_time[20];	/* ＡＯＳ時刻                     */
	char		ca_AOS_AZ[9];		/* ＡＯＳ時 ＡＺ値                */
	char		ca_AOS_EL[8];		/* ＡＯＳ時 ＥＬ値                */
	char		ca_LOS_time[20];	/* ＬＯＳ時刻                     */
	char		ca_LOS_AZ[9];		/* ＬＯＳ時 ＡＺ値                */
	char		ca_LOS_EL[8];		/* ＬＯＳ時 ＥＬ値                */
	char		ca_MEL_time[20];	/* ＭＡＸ－ＥＬ時刻               */
	char		ca_MEL_AZ[9];		/* ＭＡＸ－ＥＬ時 ＡＺ値          */
	char		ca_MEL_EL[8];		/* ＭＡＸ－ＥＬ時 ＥＬ値          */
	char		ca_VISIBLE[9];		/* ＶＩＳＩＢＬＥ時刻             */
//	char        ca_yohouget[30];    /* 予報値ファイル取得状況         */
	double		db_realAZ;					// AZ実測値
	double		db_realEL;					// EL実測値
	double		db_end_time;				// 最終データ時刻
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
	/* 運用状況監視データ取得 */
	/*------------------------*/
	/* 制御用監視データ無効カウンタ初期化 */
	m_gst_dspinf.i_mondat_NGcnt = 0;

	/*** 制御用監視項目取得 ***/

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

//	UINT sta = theApp.GetSelectStation();

	CString str;

	/* 監視の時刻 */
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

	/* パス運用状況（パス状態フラグ） */
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
			if (strPassFlag.Compare(_T("パス中")) == 0)
			{
				i_unyou = 1;
			}
			else
			{
				i_unyou = 0;
			}
		}
	}

	/* パス運用開始までの時間取得（運用開始までの時間） */
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

	/* 予報値ファイルフルパス名（予報値ファイル名） */
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
				// 拡張子がprdの場合、anpに置き換える
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

					// ファイル名を切り出す
					CString strFileName = strFilePath.Right(strFilePath.GetLength() - strFilePath.ReverseFind(_T('/')) - 1);
					strFilePath = strFilePath.TrimRight(strFileName);
					strFilePath = strFilePath.TrimRight(_T("/"));
					// 衛星名を切り出す
					CString strSatelliteName = strFilePath.Right(strFilePath.GetLength() - strFilePath.ReverseFind(_T('/')) - 1);
					// 局フォルダをつける
					CString strPredFolder = theApp.GetShareFilePath(eFileType_Pred_Folder, m_nStationID);
					CString strPredFilePath;
					strPredFilePath.Format(_T("%s%s\\%s"), (LPCTSTR)strPredFolder, (LPCTSTR)strSatelliteName, (LPCTSTR)strFileName);
					wcscpy_s(ca_fpath, 1024, strPredFilePath);
				}
			}
		}
	}

	/* 予報値ファイル作成時刻（予報値作成時刻） */
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

	/* パス運用開始時刻（パス運用開始時刻） */
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

	/* 予報値取得状態 取得 */
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

	/*** 表示用監視データ取得 ***/

	/* パス運用終了時刻（パス運用終了時刻） */
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

	/* 計画番号 */
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

	/* 衛星名取得（衛星名称） */
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

	/* ＡＯＳ時刻（ＡＯＳ時刻） */
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

	/* ＡＯＳ時ＡＺ値（ＡＯＳ時ＡＺ値） */
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

	/* ＡＯＳ時ＥＬ値（ＡＯＳ時ＥＬ値） */
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

	/* ＬＯＳ時刻（ＬＯＳ時刻） */
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

	/* ＬＯＳ時ＡＺ値（ＬＯＳ時ＡＺ値） */
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

	/* ＬＯＳ時ＥＬ値（ＬＯＳ時ＥＬ値） */
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

	/* ＭＡＸ－ＥＬ時刻（ＭＡＸ－ＥＬ時刻） */
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

	/* ＭＡＸ－ＥＬ時ＡＺ値（ＭＡＸ－ＥＬ時ＡＺ値） */
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

	/* ＭＡＸ－ＥＬ時ＥＬ値（ＭＡＸ－ＥＬ時ＥＬ値） */
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

	/* ＶＩＳＩＢＬＥ時刻(ＶＩＳＩＢＬＥ時刻 ) */
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

	/* AZ実測値 */
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

	/* EL実測値 */
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

	/*** 監視データ取得状況を判断 ***/
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Check to get monitor data status"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	if (m_gst_dspinf.i_mondat_NGcnt > 0) {
		return(0);
	}

	/*--------------*/
	/* 運用状況判断 */
	/*--------------*/
	/* パス運用開始時刻 有効性チェック */
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

	/* 運用状況判別 */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Check operation status"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	if (m_i_unyou == 1) {	/* 運用中 */
		m_gst_dspinf.i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_IN;
		m_i_into_flag = CTrackingRadarCtrlMonData::eOFF;
	}
	else { 					/* 運用中でない */
		if (i_cmp5 != 0) {     /* 有効時 */
			if ((d_time > 0) && (d_time <= CTrackingRadarCtrlMonData::eUNYOU_TIME)) {
				m_gst_dspinf.i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_START;	/* 運用開始前 */
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
				m_gst_dspinf.i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_OUT;	/* 運用外     */
				m_i_into_flag = CTrackingRadarCtrlMonData::eOFF;
#ifdef _DEBUG
				str.Format(_T("i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_OUT, m_i_into_flag = CTrackingRadarCtrlMonData::eOFF, d_time = %ld\n"), d_time);
				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
			}
		}
		else {				/* 無効時 */
			m_gst_dspinf.i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_OUT;	/* 運用外     */
			m_i_into_flag = CTrackingRadarCtrlMonData::eOFF;
#ifdef _DEBUG
			str.Format(_T("i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_OUT   , m_i_into_flag = CTrackingRadarCtrlMonData::eOFF , d_time = %ld\n"), d_time);
			CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
		}
	}

	/* カウントダウン値をバッファに待避 */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Backup count down value to buffer"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	m_l_latest_time = d_time;

	/* パス運用状況（パス状態フラグ） */
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
			if (strPassEnd.Compare(_T("運用終了")) == 0)
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
	/* 予報値ファイル取得中解除を検出       */
	/*--------------------------------------*/
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Detect cancel to get pred file"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	if (m_gst_dspinf.i_yohouti_get == CTrackingRadarCtrlMonData::eYOHOGET_MATI) {
		if (strcmp(ca_eisei, "VLBI") != 0) {
			// VLBI以外の場合
			if (strcmp(m_ca_yohouget, "TAKING") == 0) {
				m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_MATI;    /* 取得待ちのまま */
			}
			else {
				m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_OK;      /* 取得可能状態 */
			}
		}
		else {
			// VLBIの場合
			if (m_bPassEnd) {
				m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_MATI;    /* 取得待ちのまま */
			}
			else {
				m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_OK;      /* 取得可能状態 */
			}
		}
	}

	/* <<XUP>> カウントダウン値がマイナスの場合は移行期間のため、以下のチェックは行わない */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: No check, while count down value is under zero"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	if (d_time < 0) {
		return(0);
	}

	/*------------------------------*/
	/* 運用状況監視データ保存       */
	/*------------------------------*/
	/* 前回の状態を保存  */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Save last time status"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	if (i_cmp5 != 0) {		/* 有効時 */
		if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {	/* 起動時の初回表示時 */
			wcscpy_s(m_gst_dspinf.ca_fpath_s, 1024, _T(""));
			strcpy_s(m_gst_dspinf.ca_ftime_s, 30, "");
			strcpy_s(m_gst_dspinf.ca_stime_s, 30, "");
		}
		else {										/* トレンド継続表示時 */
			/*m_gst_dspinf.i_unyou_mode_s = m_gst_dspinf.i_unyou_mode;*/
			wcscpy_s(m_gst_dspinf.ca_fpath_s, 1024, m_gst_dspinf.ca_fpath);
			strcpy_s(m_gst_dspinf.ca_ftime_s, 30, m_gst_dspinf.ca_ftime);
			strcpy_s(m_gst_dspinf.ca_stime_s, 30, m_gst_dspinf.ca_stime);
		}
	}
	/* 今回の状態を保存 */
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Save this time status"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	if (i_cmp5 != 0) {		/* 有効時 */
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

		char	  full_stime[40];				/* パス運用開始時刻（フル）   */
		char	  full_etime[40];				/* パス運用終了時刻（フル）   */

		/* パス開始時刻（積算秒）算出 */
		CSatelliteData::ub_asc_to_asctime(m_gst_dspinf.ca_stime, full_stime);
		double d_Stime = CSatelliteData::ub_asc_to_dtime(full_stime);

		/* パス終了時刻（積算秒）算出 */
		CSatelliteData::ub_asc_to_asctime(m_gst_dspinf.ca_etime, full_etime);
		double d_Etime = CSatelliteData::ub_asc_to_dtime(full_etime);

		/* パス中で、AZ, ELデータが取得有効で、パス運用開始から終了の間のみ描画 */
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
				// 前回値と差異があったらデータ追加
				__int64 nLastQueAzELIndex = (__int64)m_queAzElActual.size() - 1;
				if (azel.d_AZ != m_queAzElActual[nLastQueAzELIndex].d_AZ || azel.d_EL != m_queAzElActual[nLastQueAzELIndex].d_EL)
				{
					m_queAzElActual.push_back(azel);
				}
			}
			else
			{
				// キューが空ならデータ追加
				m_queAzElActual.push_back(azel);
			}
		}
	}

	/*--------------------*/
	/* パス切替判別       */
	/*--------------------*/
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Detect pass exchange"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	if (i_cmp5 != 0) { /* 有効時 */
		i_cmp1 = wcscmp(m_gst_dspinf.ca_fpath_s, m_gst_dspinf.ca_fpath);
		m_gi_cmp1 = i_cmp1;
		i_cmp2 = strcmp(m_gst_dspinf.ca_ftime_s, m_gst_dspinf.ca_ftime);
		m_gi_cmp2 = i_cmp2;
		/* 計画開始時刻の変更を調べることで、予報値関連の情報が更新される前に画面を描画してしまうケースが発生するため、
		　 計画開始時刻は調べないことにする。
		  　 本画面は予報値関連の情報しか表示していないため、計画関連情報が更新されても、予報値関連情報が更新されなければ、
			　 画面を再描画する必要はないため、これで問題なし。*/
		/*		i_cmp3 = strcmp( m_gst_dspinf.ca_stime_s, m_gst_dspinf.ca_stime ); */
	}
	else {			/* 無効時 */
		i_cmp1 = 0; i_cmp2 = 0; i_cmp3 = 0;
	}

	/*---------------------------------------*/
	/* 再表示の要否の判断の判別              */
	/*---------------------------------------*/
#ifdef _DEBUG
	str.Format(_T("%s(%d) :: Detect review necessity"), __FILEW__, __LINE__);
	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("GetKanshiData"), _T("debug"), str, nLogEx::debug);
#endif
	/*	if( ((i_cmp1 != 0)||(i_cmp2 != 0)||(i_cmp3 != 0)) && */
	if (((i_cmp1 != 0) || (i_cmp2 != 0)) &&
		((m_gst_dspinf.i_unyou_mode == CTrackingRadarCtrlMonData::eUNYOU_IN) || (m_gst_dspinf.i_unyou_mode == CTrackingRadarCtrlMonData::eUNYOU_START))) {
		m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_ON;	/* 表示切り替え */
	}
	else if (m_i_into_flag == CTrackingRadarCtrlMonData::eON) {
		m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_ON;  /* 表示切り替え */
	}
	else if ((m_gst_dspinf.i_yohouti_get == CTrackingRadarCtrlMonData::eYOHOGET_OK) && (m_gst_dspinf.i_dsp_flg == CTrackingRadarCtrlMonData::eDSP_OFF) &&
		((m_gst_dspinf.i_unyou_mode == CTrackingRadarCtrlMonData::eUNYOU_IN) || (m_gst_dspinf.i_unyou_mode == CTrackingRadarCtrlMonData::eUNYOU_START))) {

		if ((i_cmp1 != 0) || (i_cmp2 != 0)) {
			/* 予報値関連の情報が更新された場合、予報値取得 */
			m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_SUMI;
			m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_ON;      /* 表示切替（再描画必要） */
		}
		else {
			m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_SUMI;
		}
		//m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_ON;  	/* 表示切替（再描画必要） */
	}
	else {
		m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_OFF; 	/* 表示継続（再描画不要） */
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
//-# ファイルからデータ取得
//
//@param	なし
//@retval	なし
//
//*/
///*============================================================================*/
//int CTrackingRadarCtrlMonData::GetFileData()
//{
//	int			i_unyou;			/* パス運用状況 				  */
//	long long	d_time;				/* パス運用開始までの時間 		  */
//	char		ca_stime[30];		/* パス開始時刻                   */
//	char		ca_etime[30];		/* パス終了時刻                   */
//	TCHAR		ca_fpath[1024];		/* 予報値ファイル名（フルパス）   */
//	char		ca_ftime[30];		/* 予報値ファイル更新日時		  */
//	char		ca_plnno[30];		/* 計画番号                       */
//	char		ca_eisei[100];		/* 衛星名       			      */
//	char		ca_AOS_time[20];	/* ＡＯＳ時刻                     */
//	char		ca_AOS_AZ[9];		/* ＡＯＳ時 ＡＺ値                */
//	char		ca_AOS_EL[8];		/* ＡＯＳ時 ＥＬ値                */
//	char		ca_LOS_time[20];	/* ＬＯＳ時刻                     */
//	char		ca_LOS_AZ[9];		/* ＬＯＳ時 ＡＺ値                */
//	char		ca_LOS_EL[8];		/* ＬＯＳ時 ＥＬ値                */
//	char		ca_MEL_time[20];	/* ＭＡＸ－ＥＬ時刻               */
//	char		ca_MEL_AZ[9];		/* ＭＡＸ－ＥＬ時 ＡＺ値          */
//	char		ca_MEL_EL[8];		/* ＭＡＸ－ＥＬ時 ＥＬ値          */
//	char		ca_VISIBLE[9];		/* ＶＩＳＩＢＬＥ時刻             */
//	char        ca_yohouget[30];    /* 予報値ファイル取得状況         */
//
//	long		l_inflg;
//	int			i_cmp1, i_cmp2, i_cmp3;
//	int			i_cmp5;
//
//	/*------------------------*/
//	/* 運用状況監視データ取得 */
//	/*------------------------*/
//	/* 制御用監視データ無効カウンタ初期化 */
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
//	/*** 制御用監視項目取得 ***/
//
//	CString str;
//
//	/* パス運用状況（パス状態フラグ） */
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
//	/* パス運用開始までの時間取得（運用開始までの時間） */
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
//	/* 予報値ファイルフルパス名（予報値ファイル名） */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Pred file full path name(Pred file name)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	wcscpy_s(ca_fpath, 1024, m_ca_fpath);
//
//	/* 予報値ファイル作成時刻（予報値作成時刻） */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Pred file make time(Pred make time)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_ftime, 30, m_ca_ftime);
//
//	/* パス運用開始時刻（パス運用開始時刻） */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Pass operation start time"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_stime, 30, m_ca_stime);
//
//	/* 予報値取得状態 取得 */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Get getting pred status"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_yohouget, 30, m_ca_yohouget);
//
//
//	/*** 表示用監視データ取得 ***/
//
//	/* パス運用終了時刻（パス運用終了時刻） */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Pass operation end tile"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_etime, 30, m_ca_etime);
//
//	/* 計画番号 */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Pass ID"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_plnno, 30, m_ca_plnno);
//
//	/* 衛星名取得（衛星名称） */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Get satellite name(Satellite name)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_eisei, 100, m_ca_eisei);
//
//	/* ＡＯＳ時刻（ＡＯＳ時刻） */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: AOS time(AOS time)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_AOS_time, 20, m_ca_AOS_time);
//
//	/* ＡＯＳ時ＡＺ値（ＡＯＳ時ＡＺ値） */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: AOS AZ(AOS AZ)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_AOS_AZ, 9, m_ca_AOS_AZ);
//
//	/* ＡＯＳ時ＥＬ値（ＡＯＳ時ＥＬ値） */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: AOS EL(AOS EL)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_AOS_EL, 8, m_ca_AOS_EL);
//
//	/* ＬＯＳ時刻（ＬＯＳ時刻） */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: LOS time(LOS time)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_LOS_time, 20, m_ca_LOS_time);
//
//	/* ＬＯＳ時ＡＺ値（ＬＯＳ時ＡＺ値） */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: LOS AZ(LOS AZ)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_LOS_AZ, 9, m_ca_LOS_AZ);
//
//	/* ＬＯＳ時ＥＬ値（ＬＯＳ時ＥＬ値） */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: LOS EL(LOS EL)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_LOS_EL, 8, m_ca_LOS_EL);
//
//	/* ＭＡＸ－ＥＬ時刻（ＭＡＸ－ＥＬ時刻） */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: MAX-EL time(MAX-EL time)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_MEL_time, 20, m_ca_MEL_time);
//
//	/* ＭＡＸ－ＥＬ時ＡＺ値（ＭＡＸ－ＥＬ時ＡＺ値） */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: MAX-EL AZ(MAX-EL AZ)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_MEL_AZ, 9, m_ca_MEL_AZ);
//
//	/* ＭＡＸ－ＥＬ時ＥＬ値（ＭＡＸ－ＥＬ時ＥＬ値） */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: MAX-EL EL(MAX-EL EL"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_MEL_EL, 8, m_ca_MEL_EL);
//
//	/* ＶＩＳＩＢＬＥ時刻(ＶＩＳＩＢＬＥ時刻 ) */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: VISIBLE time(VISIBLE time)"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	strcpy_s(ca_VISIBLE, 9, m_ca_VISIBLE);
//
//
//	/*** 監視データ取得状況を判断 ***/
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Check to get monitor data status"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	if (m_gst_dspinf.i_mondat_NGcnt > 0) {
//		return(0);
//	}
//
//	/*--------------*/
//	/* 運用状況判断 */
//	/*--------------*/
//	/* パス運用開始時刻 有効性チェック */
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
//	/* 運用状況判別 */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Check operation status"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	if (i_unyou == 1) {	/* 運用中 */
//		m_gst_dspinf.i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_IN;
//		m_i_into_flag = CTrackingRadarCtrlMonData::eOFF;
//	}
//	else { 					/* 運用中でない */
//		if (i_cmp5 != 0) {     /* 有効時 */
//			if ((d_time > 0) && (d_time <= CTrackingRadarCtrlMonData::eUNYOU_TIME)) {
//				m_gst_dspinf.i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_START;	/* 運用開始前 */
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
//				m_gst_dspinf.i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_OUT;	/* 運用外     */
//				m_i_into_flag = CTrackingRadarCtrlMonData::eOFF;
//#ifdef _DEBUG
//				str.Format(_T("i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_OUT, m_i_into_flag = CTrackingRadarCtrlMonData::eOFF, d_time = %lld\n"), m_d_time);
//				CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//			}
//		}
//		else {				/* 無効時 */
//			m_gst_dspinf.i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_OUT;	/* 運用外     */
//			m_i_into_flag = CTrackingRadarCtrlMonData::eOFF;
//#ifdef _DEBUG
//			str.Format(_T("i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_OUT   , m_i_into_flag = CTrackingRadarCtrlMonData::eOFF , d_time = %lld\n"), m_d_time);
//			CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//		}
//	}
//
//	/* カウントダウン値をバッファに待避 */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Backup count down value to buffer"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	m_l_latest_time = d_time;
//
//	/*--------------------------------------*/
//	/* 予報値ファイル取得中解除を検出       */
//	/*--------------------------------------*/
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Detect cancel to get pred file"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	if (m_gst_dspinf.i_yohouti_get == CTrackingRadarCtrlMonData::eYOHOGET_MATI) {
//		if (strcmp(m_ca_yohouget, "TAKING") == 0) {
//			m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_MATI;    /* 取得待ちのまま */
//		}
//		else {
//			m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_OK;      /* 取得可能状態 */
//		}
//	}
//
//	/* <<XUP>> カウントダウン値がマイナスの場合は移行期間のため、以下のチェックは行わない */
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
//	/* 運用状況監視データ保存       */
//	/*------------------------------*/
//	/* 前回の状態を保存  */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Save last time status"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	if (i_cmp5 != 0) {		/* 有効時 */
//		if (m_gst_dspinf.i_dsp_type == TYPE_NEW) {	/* 起動時の初回表示時 */
//			wcscpy_s(m_gst_dspinf.ca_fpath_s, 1024, _T(""));
//			strcpy_s(m_gst_dspinf.ca_ftime_s, 30, "");
//			strcpy_s(m_gst_dspinf.ca_stime_s, 30, "");
//		}
//		else {										/* トレンド継続表示時 */
//			/*m_gst_dspinf.i_unyou_mode_s = m_gst_dspinf.i_unyou_mode;*/
//			wcscpy_s(m_gst_dspinf.ca_fpath_s, 1024, m_gst_dspinf.ca_fpath);
//			strcpy_s(m_gst_dspinf.ca_ftime_s, 30, m_gst_dspinf.ca_ftime);
//			strcpy_s(m_gst_dspinf.ca_stime_s, 30, m_gst_dspinf.ca_stime);
//		}
//	}
//	/* 今回の状態を保存 */
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Save this time status"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	if (i_cmp5 != 0) {		/* 有効時 */
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
//	/* パス切替判別       */
//	/*--------------------*/
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Detect pass exchange"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	if (i_cmp5 != 0) { /* 有効時 */
//		i_cmp1 = wcscmp(m_gst_dspinf.ca_fpath_s, m_gst_dspinf.ca_fpath);
//		m_gi_cmp1 = i_cmp1;
//		i_cmp2 = strcmp(m_gst_dspinf.ca_ftime_s, m_gst_dspinf.ca_ftime);
//		m_gi_cmp2 = i_cmp2;
//		/* 計画開始時刻の変更を調べることで、予報値関連の情報が更新される前に画面を描画してしまうケースが発生するため、
//		　 計画開始時刻は調べないことにする。
//		  　 本画面は予報値関連の情報しか表示していないため、計画関連情報が更新されても、予報値関連情報が更新されなければ、
//			　 画面を再描画する必要はないため、これで問題なし。*/
//		/*		i_cmp3 = strcmp( m_gst_dspinf.ca_stime_s, m_gst_dspinf.ca_stime ); */
//	}
//	else {			/* 無効時 */
//		i_cmp1 = 0; i_cmp2 = 0; i_cmp3 = 0;
//	}
//
//	/*---------------------------------------*/
//	/* 再表示の要否の判断の判別              */
//	/*---------------------------------------*/
//#ifdef _DEBUG
//	str.Format(_T("%s(%d) :: Detect review necessity"), __FILEW__, __LINE__);
//	CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtunyou"), _T("debug"), str, nLogEx::debug);
//#endif
//	/*	if( ((i_cmp1 != 0)||(i_cmp2 != 0)||(i_cmp3 != 0)) && */
//	if (((i_cmp1 != 0) || (i_cmp2 != 0)) &&
//		((m_gst_dspinf.i_unyou_mode == CTrackingRadarCtrlMonData::eUNYOU_IN) || (m_gst_dspinf.i_unyou_mode == CTrackingRadarCtrlMonData::eUNYOU_START))) {
//		m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_ON;	/* 表示切り替え */
//	}
//	else if (m_i_into_flag == CTrackingRadarCtrlMonData::eON) {
//		m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_ON;  /* 表示切り替え */
//	}
//	else if ((m_gst_dspinf.i_yohouti_get == CTrackingRadarCtrlMonData::eYOHOGET_OK) && (m_gst_dspinf.i_dsp_flg == CTrackingRadarCtrlMonData::eDSP_OFF) &&
//		((m_gst_dspinf.i_unyou_mode == CTrackingRadarCtrlMonData::eUNYOU_IN) || (m_gst_dspinf.i_unyou_mode == CTrackingRadarCtrlMonData::eUNYOU_START))) {
//
//		if ((i_cmp1 != 0) || (i_cmp2 != 0)) {
//			/* 予報値関連の情報が更新された場合、予報値取得 */
//			m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_SUMI;
//			m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_ON;      /* 表示切替（再描画必要） */
//		}
//		else {
//			m_gst_dspinf.i_yohouti_get = CTrackingRadarCtrlMonData::eYOHOGET_SUMI;
//		}
//		//m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_ON;  	/* 表示切替（再描画必要） */
//	}
//	else {
//		m_gst_dspinf.i_dsp_flg = CTrackingRadarCtrlMonData::eDSP_OFF; 	/* 表示継続（再描画不要） */
//	}
//
//	return(0);
//}
//#endif

/*============================================================================*/
/*! CTrackingRadarCtrlMonData

-# キュークリア

@param	なし
@retval	なし

*/
/*============================================================================*/
void CTrackingRadarCtrlMonData::ClearQeue()
{
	if (m_nViewID >= eStation_MAX)
	{
		// 予報値一覧、計画一覧画面の場合はスカイラインクリア
		m_queAzElSkyline.clear();
		m_queAzElSkyline.shrink_to_fit();
	}
//	m_queAzElRftrip.clear();
//	m_queAzElRftrip.shrink_to_fit();
	m_queAzElForecast.clear();
	m_queAzElForecast.shrink_to_fit();
	// 実測値キューのクリア
	m_queAzElActual.clear();
	m_queAzElActual.shrink_to_fit();

	m_yohoFilePath = _T("");
}


/*============================================================================*/
/*! CTrackingRadarCtrlMonData

-# 追尾状況初期化処理

@param	なし
@retval	なし

*/
/*============================================================================*/
void CTrackingRadarCtrlMonData::Gtinit()
{
	// 実測値キューのクリア
	m_queAzElActual.clear();
	m_queAzElActual.shrink_to_fit();

	/*--------------------------------------*/
	/* スカイライン・ＲＦトリップデータ取得   */
	/*--------------------------------------*/
	Gtgetskyl();
//#if 0
//	Gtgetrftr();
//#endif
	/*** 監視データ取得状況を判断 ***/
	if (m_gst_dspinf.i_mondat_NGcnt <= 0) {

		/*------------------------*/
		/* 予報値データ取得 */
		/*------------------------*/
		if (m_gst_dspinf.i_unyou_mode != CTrackingRadarCtrlMonData::eUNYOU_OUT) {
			(void)Gtgetyoho();
			//#if _NOCOMM
			//			// ダミー実測値生成
			//			AddDummyData();
			//#endif
		}
	}
}


/*============================================================================*/
/*! CTrackingRadarCtrlMonData

-# スカイラインデータ取得

@param	なし
@retval	なし

*/
/*============================================================================*/
void CTrackingRadarCtrlMonData::Gtgetskyl()
{
	FILE    *fp;					/* ファイルポインタ               */
	char    ca_get[SATDATA_BUFF_SIZE];	    /* レコード読み込みバッファ       */
	int     i_ret;

	if (m_nStationID < 0)
		return;

	/*------------------*/
	/* ファイル名取得   */
	/*------------------*/
	CString strFilePath;
	strFilePath = theApp.GetShareFilePath(eFileType_Skyline_File, m_nStationID);

	/*------------------*/
	/* ファイルＯＰＥＮ */
	/*------------------*/
	if (_wfopen_s(&fp, strFilePath, _T("r")) != 0 || fp == NULL) {
		return;
	}

	m_queAzElSkyline.clear();

	/*--------------------------*/
	/* スカイラインレコード検索 */
	/*--------------------------*/
	for (;;) {
		if (feof(fp)) {
			break;
		}
		/* １行読み込み */
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
	/* ファイルＣＬＯＳＥ */
	/*--------------------*/
	i_ret = fclose(fp);
}

//#if 0
///*============================================================================*/
///*! CTrackingRadarCtrlMonData
//
//-# ＲＦトリップデータ取得
//
//@param	なし
//@retval	なし
//
//*/
///*============================================================================*/
//void CTrackingRadarCtrlMonData::Gtgetrftr()
//{
//	FILE    *fp;					/* ファイルポインタ               */
//	char    ca_get[SATDATA_BUFF_SIZE];	    /* レコード読み込みバッファ       */
//	int     i_ret;
//
//	/*------------------*/
//	/* ファイル名取得   */
//	/*------------------*/
//	// 送信電力からRFトリップファイルを取得
//	CString strFilePath = CRFTripAngleFile::GetRfTripFilePath(m_nStationID);
//
//	/*------------------*/
//	/* ファイルＯＰＥＮ */
//	/*------------------*/
//	if (_wfopen_s(&fp, strFilePath, _T("r")) != 0 || fp == NULL) {
//		return;
//	}
//
//	m_queAzElRftrip.clear();
//
//	/*--------------------------*/
//	/* ＲＦトリップレコード検索 */
//	/*--------------------------*/
//	for (;;) {
//		if (feof(fp)) {
//			break;
//		}
//		/* １行読み込み */
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
//	/* ファイルＣＬＯＳＥ */
//	/*--------------------*/
//	i_ret = fclose(fp);
//}
//#endif

/*============================================================================*/
/*! CTrackingRadarCtrlMonData

-# 予報値データ取得

@param	なし
@retval	なし

*/
/*============================================================================*/
int CTrackingRadarCtrlMonData::Gtgetyoho()
{
	FILE    *fp;					/* ファイルポインタ               */
	char    ca_get[SATDATA_BUFF_SIZE];	    /* レコード読み込みバッファ       */
	int     i;
	int     i_ret;
	int     i_type;

//	char    ca_yohouget[30];		/* 予報値ファイル取得状況   */

	if ((m_gi_cmp1 == 0) && (m_gi_cmp2 == 0)) {
		// 予報値情報が更新されていないので即リターン
		return(0);
	}

	/*-----------------------------------*/
	/* 予報値ファイル取得可否判別        */
	/*-----------------------------------*/
	/* 監視データ取得無効時 */
	if (m_l_inflg != INFLG_ON) {
		m_gst_dspinf.i_yohouti_get = eYOHOGET_MATI;    /* 取得待ち */
		m_gst_dspinf.i_yohouti_umu = eYOHOUTI_NASI;
		return(0);
	}
	/* 可否判別 */
	if (strcmp(m_gst_dspinf.ca_eisei, "VLBI") != 0) {
		// VLBI以外の場合
		if (strcmp(m_ca_yohouget, "TAKING") == 0) {  /* 予報値取得中 */
			m_gst_dspinf.i_yohouti_get = eYOHOGET_MATI;    /* 取得待ち */
			m_gst_dspinf.i_yohouti_umu = eYOHOUTI_NASI;
			return(0);
		}
		else {
			m_gst_dspinf.i_yohouti_get = eYOHOGET_OK;
		}
	}
	else {
		// VLBIの場合
		if (m_bPassEnd) {
			m_gst_dspinf.i_yohouti_get = eYOHOGET_MATI;    /* 取得待ち */
			m_gst_dspinf.i_yohouti_umu = eYOHOUTI_NASI;
		}
		else {
			m_gst_dspinf.i_yohouti_get = eYOHOGET_OK;
		}
	}

	/*------------------*/
	/* ファイルＯＰＥＮ */
	/*------------------*/
	CString strFilePath = CString(m_gst_dspinf.ca_fpath);

	if (strFilePath.IsEmpty())
	{
		CString str;
		str.Format(_T("%s(%d) :: Pred file name is empty"), __FILEW__, __LINE__);
		CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtgetyoho"), _T("debug"), str, nLogEx::debug);
		/* 予報値が表示できなくても処理を続ける */
		m_gst_dspinf.i_yohouti_umu = CTrackingRadarCtrlMonData::eYOHOUTI_NASI;
		return(0);
	}

	// 予報値ファイルが更新されているか確認
	HANDLE hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FILETIME yohoFileTime;
	GetFileTime(hFile, NULL, NULL, &yohoFileTime);
	CloseHandle(hFile);
	// ファイル名とファイル時刻比較
	if (m_yohoFilePath.Compare(strFilePath) == 0 &&
		CompareFileTime(&m_yohoFileTime, &yohoFileTime) == 0)
	{
		// 変更なしならば何もしない
		return 0;
	}

	if (_wfopen_s(&fp, strFilePath, _T("r")) != 0 || fp == NULL) {
		CString str;
		str.Format(_T("%s(%d) :: Pred file open error"), __FILEW__, __LINE__);
		CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtgetyoho"), _T("debug"), str, nLogEx::debug);
		/* 予報値が表示できなくても処理を続ける */
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
	/* ヘッダ及び管理情報レコード読み飛ばし */
	/*--------------------------------------*/
	for (i = 0; i<5; i++) {
		if (feof(fp)) {
			CString str;
			str.Format(_T("%s(%d) :: Pred file header error"), __FILEW__, __LINE__);
			CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtgetyoho"), _T("Error"), str, nLogEx::error);
			return(-1);
		}
		/* １行読み込み */
		if (fgets(ca_get, SATDATA_BUFF_SIZE, fp) == NULL) {
			CString str;
			str.Format(_T("%s(%d) :: Pred file format error"), __FILEW__, __LINE__);
			CLogTraceEx::Write(_T(""), _T("CTrackingRadarCtrlMonData"), _T("Gtgetyoho"), _T("Error"), str, nLogEx::error);
			return(-1);
		}
	}

	m_queAzElForecast.clear();

	/*--------------------*/
	/* 予報値レコード検索 */
	/*--------------------*/
	i_type = 0;
	for (i = 0;; i++) {
		if (feof(fp)) {
			break;
		}
		/* １行読み込み */
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
	/* ファイルＣＬＯＳＥ */
	/*--------------------*/
	i_ret = fclose(fp);

	m_gst_dspinf.i_yohouti_umu = CTrackingRadarCtrlMonData::eYOHOUTI_ARI;

	return(0);
}


/*============================================================================*/
/*! CTrackingRadarCtrlMonData

-# AZ, ELデータ取得

@param	pc_rec：レコード格納バッファ
@param　az_el：AZ ELデータ
@retval	なし

*/
/*============================================================================*/
void CTrackingRadarCtrlMonData::GetAzEl(char* pc_rec, Az_El* az_el)
{
	char	ca_AZ[9];				/* 方位角						  */
	char	ca_EL[8];				/* 仰角							  */
	int     i, j;

	/*** 文字データをＧＥＴ ***/
	for (i = 0, j = 0;; i++, j++) {	/* ＡＺゲット */
		if (pc_rec[i] == ':') {
			ca_AZ[j] = NULL;
			i++;
			break;
		}
		ca_AZ[j] = pc_rec[i];
	}
	for (j = 0;; i++, j++) {		/* ＥＬゲット*/
		if ((pc_rec[i] == ' ') || (pc_rec[i] == '\n')) {
			ca_EL[j] = NULL;
			break;
		}
		ca_EL[j] = pc_rec[i];
	}

	/*** ＡＺ、ＥＬデータをＧＥＴ ***/
	az_el->d_AZ = atof(ca_AZ);
	az_el->d_EL = atof(ca_EL);
}


/*============================================================================*/
/*! CTrackingRadarCtrlMonData

-# 予報値のAZ, ELデータ取得

@param  i_type：
@param	pc_rec：レコード格納バッファ
@param　az_el：AZ ELデータ
@param  i_roop_cnt：ループカウンタ
@retval	なし

*/
/*============================================================================*/
void CTrackingRadarCtrlMonData::GetForecastAzEl(int i_type, char* pc_rec, Forecast_Az_El* az_el, int i_roop_cnt)
{
	char	ca_time[15];			/* 時刻							  */
	//	char	ca_TIME[26];			/* 時刻（フルフォーマット）		  */
	char	ca_TIME[40];			/* 時刻（フルフォーマット）		  */
	char	ca_AZ[9] = { 0 };		/* 方位角						  */
	char	ca_EL[8] = { 0 };		/* 仰角							  */
	char	ca_upRNG[18];			/* ＲＮＧ（アップリンク）		  */
	char	ca_upRR[12];			/* ＲＲ  （アップリンク）		  */
	char	ca_dwRNG[18];			/* ＲＮＧ（ダウンリンク）		  */
	char	ca_dwRR[12];			/* ＲＲ  （ダウンリンク）		  */
	static double	d_time, d_Stime, d_Etime;
	int     i_ret;

	/*** 文字データをＧＥＴ ***/
	sscanf_s(pc_rec, "%s %s %s %s %s %s %s", ca_time, 15, ca_AZ, 9, ca_EL, 8, ca_upRNG, 18, ca_upRR, 12, ca_dwRNG, 18, ca_dwRR, 12);

	/*** 時刻データを積算秒に変換 ***/
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
/* 校正診断画面データ管理クラス
/* ------------------------------------------------------------------------------------ */
CRdnsChkData::CRdnsChkData()
{
}


CRdnsChkData::~CRdnsChkData()
{
}

/*============================================================================*/
/*! CRdnsChkData

-# 文字列のビットレートをdouble型(bps)に変換する。

@param	str：ビットレート（文字列）
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
/* 計画立案画面データ管理クラス
/* ------------------------------------------------------------------------------------ */

/*============================================================================*/
/*! COprPlnData

-# ANT待ちうけ指向通知

@ brief 文字列より対応するインデックスを求める

@param  設定文字列
@retval インデックス
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

-# ANT天頂志向有無

@ brief 文字列より対応するインデックスを求める

@param  設定文字列
@retval インデックス
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

-# 運用オプション-初期設定

@ brief 文字列より対応するインデックスを求める

@param  設定文字列
@retval インデックス
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

-# 運用オプション-送信機ドップラ補償

@ brief 文字列より対応するインデックスを求める

@param  設定文字列
@retval インデックス
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

-# 運用オプション-送信機ドップラ補償

@ brief 文字列より対応するインデックスを求める

@param  設定文字列
@retval インデックス
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

-# 運用オプション-受信機ドップラ補償

@ brief 文字列より対応するインデックスを求める

@param  設定文字列
@retval インデックス
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

-# 運用オプション-送信出力

@ brief 文字列より対応するインデックスを求める

@param  設定文字列
@retval インデックス
*/
/*============================================================================*/
int COprPlnData::GetOpTOutIdx(CString str, int type)
{
	int idx = -1;

	int s = theApp.GetSelectStation();
	if (s == 0)	// 臼田64m
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
	else if (s == 1)	// 臼田54m
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
	else if (s == 2)	// 内之浦34m
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
	else if (s == 3)	// 内之浦20m
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

-# 計画ファイルを書き込む

@brief  VLBI,MAINT用
@param  なし
@retval TRUE：正常終了/FALSE：異常終了
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

	/*----- 雛形＆計画ファイルのパス名作成 -----*/
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
		mes.Format(_T("計画雛形ファイル開くことができません。\n%s"), (LPCTSTR)sz_s_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	/*---------- 開始時刻コメント用計画内容ファイル出力処理 ----------*/
	d_s_start = 0;
	for (k = 0; k < 10; k++)
	{
		d_ntime[k] = 0;
	}

	/*---------- コメント用計画内容ファイル出力処理 ----------*/

	char sz_line[512];
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_smpl) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			continue;
		}

		/*---------- 計画内容作成処理 ----------*/

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
			/*----- レディネスチェック処理 -----*/
			l = (stpd.l_rdychk >> k) % 2;
			if (l == 0)
			{
				continue;
			}
		}
		/*----- 初期設定処理 -----*/
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
		mes.Format(_T("計画雛形ファイル開くことができません。\n%s"), (LPCTSTR)sz_s_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	if (_wfopen_s(&fp_plan, sz_p_file, _T("wb")) != 0)
	{
		CString mes;
		mes.Format(_T("計画ファイル開くことができません。\n%s"), (LPCTSTR)sz_p_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		fclose(fp_smpl);
		return -1;
	}

	/*---------- コメント用計画内容ファイル出力処理 ----------*/

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
					0,												// 0 固定
					(LPCTSTR)CString(sz_satname).MakeUpper(),
					_T(""));										// 予報値無し
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# 衛星名=%s パス番号=%s RDYCHK=%s\n"),
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
					(LPCTSTR)sz_c_ele64[48], mStationSimpleString[theApp.GetSelectStation()],	// 局名(ST_NAME)
					(LPCTSTR)sz_c_ele64[49], _T("MASTER"),										// 主従局(PRI):MASTER固定
					(LPCTSTR)sz_c_ele64[50], _T(""));							// 運用モード(OPE_MODE)
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

		/*---------- 計画内容作成処理 ----------*/

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

		case 41: //アンテナ待ち受け指向　
		{
			if (stpd.l_antwait_flg == 1)
			{
				d_time = d_start - (stpd.l_ant_time * 60);
				d_time = d_time + (5 * 60); //アンテナ待ち受け通知の５分後
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

		/* 最初の一件は無条件でコピー */
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

	// コメント出力
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

-# 計画ファイルを書き込む

@brief  RDYCHK用
@param  なし
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
int COprPlnData::WriteRdyChkPlanFile(CString sz_satname, long satno, CString sz_passno, stPlandata stpd, vector<stCalibPlan>& agclist, vector<stCalibPlan>& tlmlist, vector<stCalibPlan>& cmdlist)
{
	//------------------------------------------------------
	// レディネスチェック計画登録前の時間チェック
	//------------------------------------------------------

	// UTC現在時間取得
	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	CTime time = CTime(timeSys);
	__int64 d_timeCurrent = time.GetTime();
	CString str_date = time.Format(_T("%Y-%m-%d %H:%M:%S"));

	// 開始時刻、終了時刻取得
	__int64 d_timeStart = CSatelliteData::StrTimeToCTime(CString(stpd.sz_start)).GetTime() + stpd.l_offset_val;
	__int64 d_timeEnd = CSatelliteData::StrTimeToCTime(CString(stpd.sz_end)).GetTime() + stpd.l_offset_val;

	// 計画時刻　＞　現在時刻 
	if (d_timeStart < d_timeCurrent + START_OFFSET)
	{
		CString mes = _T("");
		mes.Format(_T("レディネスチェック計画の開始時刻までに時間の余裕がないので計画を登録できません。\n\n 計画開始時刻 :%s\n 計画終了時刻 :%s\n\n 現在時刻       :%s\n ガードタイム :%d秒")
			, (LPCTSTR)stpd.sz_start, (LPCTSTR)stpd.sz_end, (LPCTSTR)str_date, (int)START_OFFSET);

		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
		return -1;
	}

	// 開始時刻　＜　終了時刻　
	if (d_timeStart > d_timeEnd)
	{
		CString mes = _T("");
		mes.Format(_T("レディネスチェック計画の終了時刻が開始時刻よりも過去のため計画を登録できません。\n\n 計画開始時刻 :%s\n 計画終了時刻 :%s")
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

	/*----- 計画ファイルのパス名作成 -----*/
	CSatelliteData& sd = theApp.GetSatelliteData();

	CString filenamep = theApp.GetShareFilePath(eFileType_PlanOut_Folder, theApp.GetSelectStation());
	sz_p_file.Format(_T("%s%s\\%s.pln"), (LPCTSTR)filenamep, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	sz_p_folder.Format(_T("%s%s"), (LPCTSTR)filenamep, (LPCTSTR)sz_satname.MakeLower());
	if (PathFileExists(sz_p_folder) == FALSE)
	{
		CreateDirectory(sz_p_folder, NULL);
	}

	/*---------- 開始時刻コメント用計画内容ファイル出力処理 ----------*/
	d_s_start = 0;
	for (k = 0; k < 10; k++)
	{
		d_ntime[k] = 0;
	}

	if (_wfopen_s(&fp_plan, sz_p_file, _T("wb")) != 0)
	{
		CString mes;
		mes.Format(_T("計画ファイル開くことができません。\n%s"), (LPCTSTR)sz_p_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	/*---------- コメント用計画内容ファイル出力処理 ----------*/

	TCHAR szwline[2048];
	CStringA cbuf;

	sz_date[19] = '\0';
	wsprintf(szwline, _T("%s RDYCHK %s     1 %2d %s %s\n"),
		SOS_HEADER_ID,
		(LPCTSTR)CString(str_date),
		satno,
		(LPCTSTR)CString(sz_satname).MakeUpper(),
		_T(""));										// 予報値無し
	cbuf = CServerFile::ConvertEuc(szwline);
	fputs(cbuf, fp_plan);

	wsprintf(szwline, _T("# 衛星名=%s パス番号=%s RDYCHK=%s\n"),
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
		(LPCTSTR)sz_c_ele64[48], mStationSimpleString[theApp.GetSelectStation()],	// 局名(ST_NAME)
		(LPCTSTR)sz_c_ele64[49], _T("MASTER"),										// 主従局(PRI):MASTER固定
		(LPCTSTR)sz_c_ele64[50], _T(""));											// 運用モード(OPE_MODE)
	cbuf = CServerFile::ConvertEuc(szwline);
	fputs(cbuf, fp_plan);

	wsprintf(szwline, _T("# %s=%s\n"),
		(LPCTSTR)sz_c_ele64[61], _T(""));											// Auto
	cbuf = CServerFile::ConvertEuc(szwline);
	fputs(cbuf, fp_plan);


	/*---------- 計画内容作成処理 ----------*/
	fputs("\n", fp_plan);

	wsprintf(szwline, _T("#****************************************************************************#\n"));
	cbuf = CServerFile::ConvertEuc(szwline);
	fputs(cbuf, fp_plan);

	wsprintf(szwline, _T("# レディネスチェック\n"));
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

-# 計画ファイルを読込

@brief  RDYCHK用
@param  なし
@retval TRUE：正常終了/FALSE：異常終了
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
		mes.Format(_T("計画ファイルを開くことができません。\n%s"), (LPCTSTR)sz_p_file);
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
			{	/* 判定ケースを追加 */
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
		case 37: //レディネスチェック（AGC)
			AfxExtractSubString(item, CString(sz_ele[1]), 0, '@');
			st.ctrl = item;
			if (AfxExtractSubString(item, CString(sz_ele[1]), 3, '@'))
			{
				// ループバンドがある場合
				st.loopband = item;
			}
			else
			{
				// ループバンドが無い場合
				st.loopband = _T("");
			}
			agclist.push_back(st);

			break;

		case 38: //レディネスチェック（RNG)
			AfxExtractSubString(item, CString(sz_ele[1]), 0, '@');
			st.ctrl = item;
			agclist.push_back(st);
			break;

		case 39: //レディネスチェック（テレメトリ伝送）
			AfxExtractSubString(item, CString(sz_ele[1]), 0, '@');
			st.ctrl = item;
			tlmlist.push_back(st);

			break;

		case 40: //レディネスチェック（コマンド伝送）
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

-# 計画ファイル登録要求（NEW）

@param  なし
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
int COprPlnData::RegistPlanNew(CString satname, CString passno, BOOL bNew)
{
	// 計画登録要求
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
		AfxMessageBox(_T("計画登録に成功しました。"), MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		AfxMessageBox(_T("計画登録に失敗しました。"), MB_OK | MB_ICONWARNING);
	}

	return 0;
}

/* ------------------------------------------------------------------------------------ */
/* 周波数制御画面データ管理クラス
/* ------------------------------------------------------------------------------------ */
CFreqCtrlData::CFreqCtrlData()
{
}


CFreqCtrlData::~CFreqCtrlData()
{
}



/* ------------------------------------------------------------------------------------ */
/* 送信制御画面データ管理クラス
/* ------------------------------------------------------------------------------------ */
CTrnCtrlData::CTrnCtrlData()
{
}


CTrnCtrlData::~CTrnCtrlData()
{
}



/* ------------------------------------------------------------------------------------ */
/* 無線業務日誌画面データ管理クラス
/* ------------------------------------------------------------------------------------ */
CBsnsDiaryData::CBsnsDiaryData()
{
}


CBsnsDiaryData::~CBsnsDiaryData()
{
}



/* ------------------------------------------------------------------------------------ */
/* CID選択画面データ管理クラス
/* ------------------------------------------------------------------------------------ */
CCIDSlctData::CCIDSlctData()
{
}


CCIDSlctData::~CCIDSlctData()
{
}

/*============================================================================*/
/*! CCIDSlctData

-# 衛星毎のCIDリスト取得

@param  sat		衛星名
@retval CIDリスト（vector<CString>）
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

-# CIDから無効文字列(CID_INVALID)除去

@param  stCid	入力CIDパラメータ
@retval st		出力CIDパラメータ
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
///* トレンドプロパティメモリマップデータ
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
///*! トレンドプロパティメモリマップデータ
//
//-# メモリマップデータファイルをマッピング
//
//@param	nMMDataMapView	メモリマップデータファイルマップビューのサイズ
//@retval	bool	結果
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
//	// メモリマップデータファイルマップビューを確認
//	if ((pCur = (LPSTR)m_pMMDataMapView) != NULL)
//	{
//		DWORD nData = sizeof(stTrendPropiesMemData);
//		while (nMMDataMapView)
//		{
//			// CSpaceData格納分のサイズを確認
//			if (nMMDataMapView < nData)
//			{
//				bRes = false;
//				break;
//			}
//
//			// データの先頭ポインタをリストに追加
//			m_listMMDataMap.push_back(pCur);
//
//			// ポインタとサイズを更新
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
///*! トレンドプロパティメモリマップデータ
//
//-# メモリマップデータファイルのオープン
//
//@param	flgRead		リードオープンフラグ
//@retval	bool	結果
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapData::OpenMMDataFile(bool flgRead/* = true*/)
//{
//	// メモリマップデータファイル名を確認
//	if (m_strMMDataFile.IsEmpty())
//		return false;
//
//	// メモリマップデータファイルハンドルを確認
//	if (m_hMMDataFile != INVALID_HANDLE_VALUE)
//		return false;
//
//	TRY
//	{
//		CStringA	strMMDataFile = (CT2A)m_strMMDataFile;
//
//		// メモリマップデータファイルをオープン
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
//			// ファイルサイズを取得
//			dwLow = ::GetFileSize(m_hMMDataFile, &dwHigh);
//			if ((dwLow == -1) && (GetLastError() != NO_ERROR))
//			{
//				CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapData"), _T("OpenMMDataFile"), _T("Error"), _T("GetFileSize error."), nLogEx::error);
//				AfxThrowUserException();
//			}
//
//			// ファイルマッピングオブジェクトを作成
//			m_hMMDataObject = ::CreateFileMapping(m_hMMDataFile, NULL, PAGE_READONLY, 0, dwLow, NULL);
//			if (m_hMMDataObject == NULL)
//			{
//				CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapData"), _T("OpenMMDataFile"), _T("Error"), _T("CreateFileMapping error."), nLogEx::error);
//				AfxThrowUserException();
//			}
//
//			// アドレス空間にファイルのビューをマッピング
//			m_pMMDataMapView = ::MapViewOfFile(m_hMMDataObject, FILE_MAP_READ, 0, 0, dwLow);
//			if (m_pMMDataMapView == NULL)
//			{
//				CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapData"), _T("OpenMMDataFile"), _T("Error"), _T("MapViewOfFile error."), nLogEx::error);
//				AfxThrowUserException();
//			}
//
//			// メモリマップデータファイルをマッピング
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
//		// メモリマップデータマップリストを削除
//		if (m_listMMDataMap.size() > 0)
//		{
//			vector<LPVOID>	tmp_listMMDataMap(0);
//
//			m_listMMDataMap.swap(tmp_listMMDataMap);
//
//			m_listMMDataMap.clear();
//		}
//
//		// アドレス空間のファイルビューをアンマップ
//		if (m_pMMDataMapView != NULL)
//		{
//			::UnmapViewOfFile(m_pMMDataMapView);
//
//			m_pMMDataMapView = NULL;
//		}
//
//		// ファイルマッピングオブジェクトをクローズ
//		if (m_hMMDataObject != NULL)
//		{
//			::CloseHandle(m_hMMDataObject);
//
//			m_hMMDataObject = NULL;
//		}
//
//		// メモリマップデータファイルをクローズ
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
///*! トレンドプロパティメモリマップデータ
//
//-# メモリマップデータファイルのクローズ
//
//@param	なし
//@retval	なし
//
//*/
///*============================================================================*/
//void CTrendPropertiesMemMapData::CloseMMDataFile()
//{
//	// メモリマップデータマップリストを削除
//	if (m_listMMDataMap.size() > 0)
//	{
//		vector<LPVOID>	tmp_listMMDataMap(0);
//
//		m_listMMDataMap.swap(tmp_listMMDataMap);
//
//		m_listMMDataMap.clear();
//	}
//
//	// アドレス空間のファイルビューをアンマップ
//	if (m_pMMDataMapView != NULL)
//	{
//		::UnmapViewOfFile(m_pMMDataMapView);
//
//		m_pMMDataMapView = NULL;
//	}
//
//	// ファイルマッピングオブジェクトをクローズ
//	if (m_hMMDataObject != NULL)
//	{
//		::CloseHandle(m_hMMDataObject);
//
//		m_hMMDataObject = NULL;
//	}
//
//	// メモリマップデータファイルをクローズ
//	if (m_hMMDataFile != INVALID_HANDLE_VALUE)
//	{
//		::CloseHandle(m_hMMDataFile);
//
//		m_hMMDataFile = INVALID_HANDLE_VALUE;
//	}
//}
//
///*============================================================================*/
///*! トレンドプロパティメモリマップデータ
//
//-# データをメモリマップデータファイルに書き込む
//
//@param	propertiesData	プロパティデータ
//@retval	bool			結果
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapData::WriteMMDataFile(CPropertiesData& propertiesData)
//{
//	// メモリマップデータファイルハンドルを確認
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
//		// メモリマップデータファイルをクローズ
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
///*! トレンドプロパティメモリマップデータ
//
//-# レンドプロパティデータをメモリマップデータファイルから読み込む
//
//@param	idx				取得するデータの通し番号
//@param	propertiesData	データの格納アドレスへの参照
//@retval	bool			結果
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapData::ReadMMDataFile(DWORD idx, CPropertiesData& propertiesData)
//{
//	LPSTR	pCur;
//
//	// メモリマップデータファイルハンドルを確認
//	if (m_hMMDataFile == INVALID_HANDLE_VALUE)
//	{
//		CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapData"), _T("ReadMMDataFile"), _T("Error"), _T("File handle is none."), nLogEx::error);
//		return false;
//	}
//
//	// メモリマップデータマップリストを確認
//	if (m_listMMDataMap.size() <= 0)
//	{
//		CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapData"), _T("ReadMMDataFile"), _T("Error"), _T("Map list is none."), nLogEx::error);
//		return false;
//	}
//
//	// 指定された番号のデータへのポインタを取得
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
///* トレンドプロパティメモリマップリスト
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
///*! トレンドプロパティメモリマップリスト
//
//-# メモリマップリストの削除
//
//@param			なし
//@retval	bool	結果
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapList::DeleteMemMapList()
//{
//	vector<CTrendPropertiesMemMapData>	tmp_listMMData(0);
//
//	// メモリマップデータファイルをクローズ
//	for (auto itr = m_MapMMData.begin(); itr != m_MapMMData.end(); ++itr)
//	{
//		CString strKey = (CString)(itr->first);
//		CTrendPropertiesMemMapData *mmData = (CTrendPropertiesMemMapData*)(&itr->second);
//		mmData->CloseMMDataFile();
//	}
//
//	// メモリマップデータリストを完全に削除するためローカルリストに交換して削除
//	m_listMMData.swap(tmp_listMMData);
//
//	// パラメータをクリア
//	ClearParameter();
//
//	return true;
//}
//
///*============================================================================*/
///*! トレンドプロパティメモリマップリスト
//
//-# 設定データ保存の開始処理
//
//@param	なし
//@retval	bool		結果
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapList::WriteInitialization(CString strFileName)
//{
//	// メモリマップリストを削除
//	if (!DeleteMemMapList())
//	{
//		CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapList"), _T("WriteInitialization"), _T("Error"), _T("DeleteMemMapList failed."), nLogEx::error);
//		return false;
//	}
//
//	// メモリマップ情報ファイルからパラメータを設定
//	LoadMMInfoFile();
//
//	// メモリマップデータを作成
//	CString	strMMDataFile;
//	CString strFName;
//	strFName = strFileName.Left(strFileName.GetLength() - 4);
//
//	// メモリマップデータファイル名を作成
//	CTrendPropertiesMemMapData	newMMData;
//	// メモリマップデータファイル名を設定
//	newMMData.SetMMDataFileName(strFileName);
//
//	// 新しいメモリマップデータをリストに追加
//	m_MapMMData[strFName] = newMMData;
//
//	// ステータスを更新
//	m_eMMListStatus = eMMListStatus_WriteOpen;
//
//	return true;
//}
//
///*============================================================================*/
///*! トレンドプロパティメモリマップリスト
//
//-# 測定データ保存の終了処理
//
//@param	strFileName	ファイル名
//@retval	bool		結果
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
//	// ステータスをクリア
//	m_eMMListStatus = eMMListStatus_ReadOpen;	// すぐ読める状態なので
//
//	return true;
//}
//
///*============================================================================*/
///*! トレンドプロパティメモリマップリスト
//
//-# データの書き込み
//
//@param	strFileName		ファイル名
//@param	propertiesdataList  プロパティデータリスト
//@retval	bool			結果
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapList::WriteData(CString strFileName, vector<CPropertiesData>& propertiesdataList)
//{
//	// ステータスを確認
//	if (m_eMMListStatus != eMMListStatus_WriteOpen)
//	{
//		CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapList"), _T("WriteData"), _T("Error"), _T("Status is no read open."), nLogEx::error);
//		return false;
//	}
//
//	CString mapkey;
//	mapkey = strFileName.Left(strFileName.GetLength() - 4);
//
//	// ファイルオープン
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
//		// データを書き込む
//		if (!tmpMMData.WriteMMDataFile(propertiesdataList[i]))
//		{
//			CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteData"), _T("Error"), _T("Write error."), nLogEx::error);
//			return false;
//		}
//	}
//
//	// ファイルクローズ
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
///*! トレンドプロパティメモリマップリスト
//
//-# 設定データ展開の開始処理
//
//@param	strFileName	ファイル名
//@retval	bool		結果
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapList::ReadInitialization(CString strFileName)
//{
//	// メモリマップリストを削除
//	if (!DeleteMemMapList())
//	{
//		CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapList"), _T("ReadInitialization"), _T("Error"), _T("DeleteMemMapList failed."), nLogEx::error);
//		return false;
//	}
//
//	// メモリマップ情報ファイルからパラメータを設定
//	LoadMMInfoFile();
//
//	// メモリマップデータを作成
//	CString	strMMDataFile;
//	CString strFName;
//	strFName = strFileName.Left(strFileName.GetLength() - 4);
//
//	// メモリマップデータファイル名を作成
//	CTrendPropertiesMemMapData	newMMData;
//	// メモリマップデータファイル名を設定
//	newMMData.SetMMDataFileName(strFileName);
//
//	// 新しいメモリマップデータをリストに追加
//	m_MapMMData[strFName] = newMMData;
//
//	// ステータスを更新
//	m_eMMListStatus = eMMListStatus_ReadOpen;
//
//	return true;
//}
//
///*============================================================================*/
///*! トレンドプロパティメモリマップリスト
//
//-# 設定データ展開の終了処理
//
//@param
//@retval	bool		結果
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapList::ReadFinalization()
//{
//	// ステータスを確認
//	if (m_eMMListStatus != eMMListStatus_ReadOpen)
//	{
//		CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapList"), _T("ReadFinalization"), _T("Error"), _T("Status is no read open."), nLogEx::error);
//		return false;
//	}
//
//	// メモリマップデータファイルをクローズ
//	for (auto itr = m_MapMMData.begin(); itr != m_MapMMData.end(); ++itr)
//	{
//		CString strKey = (CString)(itr->first);
//		CTrendPropertiesMemMapData *mmData = (CTrendPropertiesMemMapData*)(&itr->second);
//		mmData->CloseMMDataFile();
//	}
//
//	// ステータスをクリア
//	m_eMMListStatus = eMMListStatus_ReadOpen;	// すぐ読める状態なので
//
//	return true;
//}
//
///*============================================================================*/
///*! トレンドプロパティメモリマップリスト
//
//-# データの読み込み
//
//@param	strFileName			ファイル名
//@param	propertiesdataList	データの格納アドレスリストへの参照
//@retval	bool			結果
//
//*/
///*============================================================================*/
//bool CTrendPropertiesMemMapList::ReadData(CString strFileName, vector<CPropertiesData>& propertiesdataList)
//{
//	// ステータスを確認
//	if (m_eMMListStatus != eMMListStatus_ReadOpen)
//	{
//		CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapList"), _T("ReadData"), _T("Error"), _T("Status is no read open."), nLogEx::error);
//		return false;
//	}
//
//	CString mapkey;
//	mapkey = strFileName.Left(strFileName.GetLength() - 4);
//
//	// ファイルオープン
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
//	// データを読み込む
//	for (DWORD i = 0; i < tmpMMData.GetPropertiesDataCnt(); ++i)
//	{
//		if (!tmpMMData.ReadMMDataFile(i, propertiesdataList[i]))
//		{
//			CLogTraceEx::Write(_T(""), _T("CTrendPropertiesMemMapList"), _T("ReadData"), _T("Error"), _T("Read error."), nLogEx::error);
//			return false;
//		}
//	}
//
//	// ファイルクローズ
//	if (tmpMMData.IsOpenMMDataFile())
//	{
//		tmpMMData.CloseMMDataFile();
//	}
//
//	return true;
//}
//#endif

/* ------------------------------------------------------------------------------------ */
/* 管制データ管理クラス
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

-# 時刻文字列を通算秒にする

@param  sz_time：フルフォーマット入力時刻文字列
@retval	積算秒

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

-# 省略フォーマットの時刻文字列をfull-formatの時刻文字列に変換

@param  sz_time：入力時刻文字列
@            1999-07-01 11:22:33 の省略形としては以下の形式を許す
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
@param	output_time:フルフォーマット出力時刻文字列
@retval	5 : yy-mm-dd hh:mm
@       6 : yy-mm-dd hh:mm:ss.s
@       7 :
@       1 : 異常終了

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
		if (sz_input_time[i] == ' '){ continue; } /*先頭スペースサプレス*/
		strcpy_s(sz_time, sz, &sz_input_time[i]);
		break;
	};
	if (sz_time[0] == NULL){
		return(-1);
	};

	for (i = (int)strlen(sz_time) - 1, j = 0; i > 0; i--){
		if ((j == 0) && (sz_time[i] == ' ')){
			sz_time[i] = NULL; j = i; /*後ろのスペースサプレス*/
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

-# 省略フォーマットの時刻文字列をfull-formatの時刻文字列に変換

@param  sz_time：入力時刻文字列
@            1999-07-01 11:22:33 の省略形としては以下の形式を許す
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
@param	output_time:フルフォーマット出力時刻文字列
@retval	5 : yy-mm-dd hh:mm
@       6 : yy-mm-dd hh:mm:ss.s
@       7 :
@       1 : 異常終了

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
		if (sz_input_time[i] == ' '){ continue; } /*先頭スペースサプレス*/
		wcscpy_s(sz_time, sz, &sz_input_time[i]);
		break;
	};
	if (sz_time[0] == NULL){
		return(-1);
	};

	for (i = (int)wcslen(sz_time) - 1, j = 0; i > 0; i--){
		if ((j == 0) && (sz_time[i] == ' ')){
			sz_time[i] = NULL; j = i; /*後ろのスペースサプレス*/
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

-# ut 時刻情報をYYYY-MM-DD HH:MM:SS.ssss文字列に変換する。

@param  d_uttime：時刻情報
@param	sz_time:YYYYを格納する領域
@retval	5 : yy-mm-dd hh:mm
@       6 : yy-mm-dd hh:mm:ss.s
@       7 :
@       1 : 異常終了

*/
/*============================================================================*/
void CSatelliteData::ub_dtime_to_asc(double d_uttime, char* sz_time)
{ 
	time_t l_uttime,l_100usec;
	struct  tm t;
	long    l_yyyy,l_mm;
//#if 0
//	if( d_uttime <= -2147483648.0 ){ /* 0x80000000..0x7fffffff の範囲チェック */ 
//		d_uttime = -2147483648.0;    /* これはinteger変換overflow-FPEが発生する*/
//	}else if( d_uttime >=  2147483646.0 ){ /*ことを避けるためである。*/ 
//		d_uttime =  2147483646.0;          /* 1998-03-18 */
//	}
//#else
	if (d_uttime <= -(double)LONG64_MAX){ /* 0x80000000..0x7fffffff の範囲チェック */
		d_uttime = -(double)LONG64_MAX;    /* これはinteger変換overflow-FPEが発生する*/
	}
	else if (d_uttime >= (double)(LONG64_MAX - 2)){ /*ことを避けるためである。*/
		d_uttime = (double)(LONG64_MAX - 2);          /* 1998-03-18 */
	}
//#endif
	l_uttime = (time_t)d_uttime;
	l_100usec = (time_t)( d_uttime - ((double)l_uttime) + 0.00005 ) * 10000; /* 100usec */
	if( l_100usec >= 10000 ){ /* 0.99995 は 1.00000 になる */
		l_100usec-=10000;
		l_uttime++;           /* 秒の桁を補正 */
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
//-# 文字列比較
//文字列を比較する。比較時にワイルドカード文字列を使用することが可能
//使用できるワイルトカード文字には以下のものがある。
//　　　"*":任意文字列と一致する。
//   "?":任意の１文字と一致する。
//   正規表現（[a-z]当のregexpで定義されたパターン指定）も有効
//
//   @param	string：調べたい文字列
//   @param	pattern："*.*"等のパターン文字列
//   @retval	0:一致しない / 1:一致した
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
//		fprintf(stderr,"regexecは一致したけど一致しない:ua_strmatch(%s,%s):rm_so=%d,rm_eo=%d\n",string,pattern,pm.rm_so,pm.rm_eo); fflush(stderr);
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
//・「op arg1,arg2,argN;」形式で記述されたテキストファイルを読み込んで、
//op/arg1/arg2/argN を分離する。(行は「;」が表れるまで複数行に
//またがっても良い)
//
//・「/,#,*」のどれかで始まる行はコメント行として読み飛ばす(入力されない)
//「;」の後ろが「/,#,*」のどれかの場合も「;」の後ろから改行コードまで
//をコメント行として読み飛ばす。
//
//・改行コードのみの行／スペース文字のみの行は読み飛ばす。
//
//・スペース文字、tabコードはサプレスする(スペースコードと「,」を入力
//したい場合は「"」で囲む。「"」を入力したい場合は「\"」と記述する。)
//
//・アーギュメントが省略された場合(op,,,;)はアーギュメント文字列として
//NULL文字列("")へのポインタを返す。
//
//・「# n "filename"」形式のコメントはcppによるinclude展開されたテキスト
//　　　を読み込んだ場合を考慮してfilenameのファイルが存在していれば、行番号を
//   nに設定する
//
//   ［注意］
//   ・op/argの実体はこのルーチン内にstaticデータとして存在する。
//   従って、このルーチンを複数回呼び出す場合は(必要に応じて)呼び出し側で
//   op/argの内容を保存する必要がある。(このルーチン内では呼び出される毎に
//   同じstaticデータエリアを使う。)
//
//   ・opから;までの間にはコメントは書けませんので注意してください
//
//   ［使用例］
//   char *argv[ARGMAX+1];
//   long line;
//   FILE *fp;
//   fp = fopen(.....);    // 「op arg1,arg2,...,argN;」形式ファイルをopen
//   line = 1;             // して行番号を初期化します
//   while((n=ub_getln(fp,&line,argv,ARGMAX))!=-1) {
//   if (!strcmp(argv[0],"op") {  // argv[0]にopの部分の文字列の先頭
//   // アドレスが設定されます
//   sscanf(argv[1],"%d",&i_arg1); // argv[1]に第1アーギュメント文字列
//   // の先頭アドレスが設定されます
//   // argv[...]に設定される文字列
//   // のアドレスはこのルーチン内のstatic
//   // データエリアを指している事を注意
//   ..........
//   if (!strcmp(argv[2],"")) {    // アーギュメントが省略された場合は
//   i_arg2 = DEFALT;          // NULL文字列へのアﾄﾞレスを返す
//   }
//   ..........
//
//   @param	fp：入力ファイルのfopen()で得たFILE*値
//   @param	lp_line：入力ファイルの行番号(このルーチン内で
//   改行コード毎にカウントアップする)
//   @param	argv：分離された文字列(argv[0]==op ;
//   argv[1]==arg1,..,argv[N]==argN
//   @paran	argc：呼び出し側で用意したargv[]の個数
//   @retval	-2 : opの長さが80文字以上ある/argの長さの合計が16000文字以
//   ある(このルーチンの内部のstaticエリアに起因する制約)
//   -1 : ファイルの終了(EOF)まで読み込んだ
//   0 : 「op;」形式のテキスト(argv[0]にopを返した)
//   1 : 「op arg1;」形式のテキスト(argv[0]にopをargv[1]にarg1を
//   返した)
//   n : 「op arg1,arg2,arg3,...,argN」形式のテキスト(nはarg数)
//
//   */
///*============================================================================*/
//long CSatelliteData::ub_getln(FILE* fp, long* lp_line, char* argv[], long argc)
///*
//関数値:   -2 : opの長さが80文字以上ある/argの長さの合計が16000文字以
//ある(このルーチンの内部のstaticエリアに起因する制約)
//-1 : ファイルの終了(EOF)まで読み込んだ
//0 : 「op;」形式のテキスト(argv[0]にopを返した)
//1 : 「op arg1;」形式のテキスト(argv[0]にopをargv[1]にarg1を
//返した)
//n : 「op arg1,arg2,arg3,...,argN」形式のテキスト(nはarg数)
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
//		case '#':            /* commentまたはcppの展開結果 skip(LFまで) */
//			if (fgets(sz_buf, sizeof(sz_buf), fp) == NULL){
//				return(-1);
//			}
//			(*lp_line)++;
//			/*  # 2 "xxxxxxx" の場合は*/
//			if (sscanf_s(sz_buf, "%d%1s%[^\"]\"", &i, sz_dmy, 256, sz_filename, 256) == 3){
//				if (stat(sz_filename, &stat_buf) != -1){
//					*lp_line = i;
//				}
//			}
//			goto START;
//		case '*': case '/':                /* comment skip(LFまで) */
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
//		if (c <= ' '){                       /* OP 分離 */
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
//			if ((c = fgetc(fp)) != EOF){ break; }; /* \" の処理 */
//		};
//		if (l_supflg == 0){
//			if (c == ';'){                  /* END OF CODE */
//				buf.arg[l_pos] = NULL;
//				return (l_argcnt - l_ajst); /* return argmemt count*/
//			}
//			else if (c == ','){            /* argmentｾﾊﾟﾚｰﾀ */
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
//		else{                   /* " "の内側ではそのまま */
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
//-# config定義ファイルから定義情報を検索
//
//@param  sz_filename：config定義ファイルのファイル名
//@param  keyword：config情報に対応するキーワード文字列を指定
//@param  formatstring：キーワード文字列に対応するconfig情報を読み込む際のscanf()用のフォーマット文字列を指定
//@param	arg1～arg8：出力定義情報
//@retval   n:検索された定義項目をsscanf()で取り込んだ場合のsscanf()の
//戻り値、すなわち、formatstringで指定した入力項目数を返す
//-1:キーワードが発見できなかった、または、sscanf()でキーワー
//ドに対応する定義値を読み込めなかった。
//-2:configファイルがOPENできない、またはその他のSystem Call
//関連のエラーが発生した。（エラー詳細についてはerrnoに設定した）
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
//	/* ファイル名取得   */
//	/*------------------*/
//	CString strAppPath = theApp.GetAppPath();
//	CString strFilePath;
//	strFilePath.Format(_T("%s%s"), strAppPath, CString(sz_filename));
//
//	/*------------------*/
//	/* ファイルＯＰＥＮ */
//	/*------------------*/
//	if (_wfopen_s(&fp, strFilePath, _T("r")) != 0) {
//		//		uc_errlog(0x0011, __FILEW__, __LINE__);
//		return -2;
//	};
//
//	if (_tcscmp(sz_beforefilename, sz_filename) != 0){/*違っていれば*/
//		l_fgetconf_seek_ptr = 0; /*先頭からスキャンのやり直し*/
//	}
//	_tcscpy_s(sz_beforefilename, sizeof(sz_beforefilename), sz_filename);
//	if (fseek(fp, l_fgetconf_seek_ptr, SEEK_SET) == -1){
//		//		uc_errlog(0x0011, __FILEW__, __LINE__);
//	};
//	l_config_line = 1;
//	while ((i = ub_getln(fp, &l_config_line, cpa_argv, ARGMAX - 1)) >= 0){
//		if (!strcmp(cpa_argv[0], keyword)){ /*キーワードが一致するか？*/
//			goto found;
//		}
//		else{ /*計算機固有パラメータであるかどうかしらべる*/
//			if ((cp_keyword = strrchr(cpa_argv[0], '.')) != NULL){ /*計算機host名指定がある場合*/
//				*cp_keyword = NULL;
//				if (!strcmp(cp_keyword + 1, keyword)){ /*キーワード一致*/
//					if (ua_strmatch(sz_hostname, cpa_argv[0]) != NULL){/*host名一致*/
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
//found:/*キーワードが一致*/
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
//		/* カンマをNULLに置き換え */
//		*psz_conma = (char)NULL;
//
//		/* 文字型編集子's'を捜す */
//		if (strchr(psz_fmt, 's') != NULL) {	/* 文字型の場合 */
//			if (cpa_argFmt[iArg] != NULL) {
//				strcpy_s(cpa_argFmt[iArg], 16, cpa_argv[iArg]);
//			}
//		}
//		else {									/* 数値型の場合 */
//			if (cpa_argFmt[iArg] != NULL) {
//				sscanf_s(cpa_argv[iArg], psz_fmt, cpa_argFmt[iArg]);
//			}
//		}
//
//		/* カウンタインクリメント */
//		iArg++;
//
//		/* 検索位置の更新 */
//		psz_fmt = psz_conma + 1;
//	}
//
//	/* 文字型編集子's'を捜す */
//	if (strchr(psz_fmt, 's') != NULL) {	/* 文字型の場合 */
//		if (cpa_argFmt[iArg] != NULL) {
//			strcpy_s(cpa_argFmt[iArg], 16, cpa_argv[iArg]);
//		}
//	}
//	else {									/* 数値型の場合 */
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

-# config定義ファイルからヘッダ行位置を検索し、行移動する

@param	fp：ファイルポインタ
@param	strHeader：ヘッダ名
@retval	1：成功 / -1：失敗

*/
/*============================================================================*/
int CSatelliteData::MovetHeaderInFile(FILE* fp, TCHAR* strHeader)
{
	char ca_get[SATDATA_BUFF_SIZE];	    /* 読み込みバッファ       */
	int ret = -1;

	(void)fseek(fp, 0, SEEK_SET);

	for (;;) {
		if (feof(fp)) {
			break;
		}
		/* １行読み込み */
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

-# config定義ファイルから定義情報を検索し、long型の値として読み込む

@param	fp：ファイルポインタ
@param	strId：識別ID名
@param	val：long型の読み込み値
@retval	1：成功 / -1：失敗

*/
/*============================================================================*/
int CSatelliteData::GetConfFromFile(FILE* fp, TCHAR* strId, long* val)
{
	char ca_get[SATDATA_BUFF_SIZE];	    /* 読み込みバッファ       */
	int ret = -1;

//	(void)fseek(fp, 0, SEEK_SET);

	for (;;) {
		if (feof(fp)) {
			break;
		}
		/* １行読み込み */
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

-# config定義ファイルから定義情報を検索し、double型の値として読み込む

@param	fp：ファイルポインタ
@param	strId：識別ID名
@param	val：double型の読み込み値
@retval	1：成功 / -1：失敗

*/
/*============================================================================*/
int CSatelliteData::GetConfFromFile(FILE* fp, TCHAR* strId, double* val)
{
	char ca_get[SATDATA_BUFF_SIZE];	    /* 読み込みバッファ       */
	int ret = -1;

//	(void)fseek(fp, 0, SEEK_SET);

	for (;;) {
		if (feof(fp)) {
			break;
		}
		/* １行読み込み */
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
//-# 予報値一覧ファイルを読み込みデータ取得
//
//@param	なし
//@retval	1：成功 / -1：失敗
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
//		//		fprintf(stderr, "%s\n", "予報値一覧ファイルのオープンに失敗しました。");
//		//		uc_opelog(NULL, NULL, BASE_OPE_NO + 14, NULL, NULL);
//		return FALSE;
//	}
//
////	vector<CPredListData>		m_listPredList;				// 予報値一覧データ
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
//		// 新フォーマット読み込み
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
//			// 新フォーマット読み込み（タイプなし）
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
//			// 旧フォーマット読み込み
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
//				// 局名
//				pld.strStationName = CString(sz_stationname);
//			}
//
//			// 衛星名
//			pld.strSatellite = CString(sz_satname);
//
//			// パスID
//			pld.strPassId = CString(sz_pass);
//
//			// AOS時刻の変換
//			sprintf_s(sz_aos, sizeof(sz_aos), "%s %s%s", sz_a_date, sz_a_time, ".0000");
//			if (ub_asc_to_asctime(sz_aos, sz_aos_date) < 0) {
//				fclose(fp);
//				return FALSE;
//			}
//			cTime = StrTimeToCTime(CString(sz_aos_date));
//#if 1	// ダミー
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
//			// LOS時刻の変換
//			sprintf_s(sz_los, sizeof(sz_aos), "%s %s%s", sz_l_date, sz_l_time, ".0000");
//			if (CSatelliteData::ub_asc_to_asctime(sz_los, sz_los_date) < 0) {
//				fclose(fp);
//				return FALSE;
//			}
//			cTime = StrTimeToCTime(CString(sz_los_date));
//
//#if 1	// ダミー
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
//			// MAX EL時刻の変換
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
//			// 予報値ファイル名
//			pld.strPredPath = CString(sz_pred_path);
//
//			// オフセット
//			str = CString(sz_offset);
//			// 正負を抽出
//			long nSign = 1;
//			if (str.Left(1) == _T('-'))
//			{
//				nSign = -1;
//				str.Remove(_T('-'));
//			}
//			// 右から２つが秒
//			CString strSS = str.Right(2);
//			// 残りが分と":"なので秒と":"の文字数を除外し、分を出す
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
//					// タイプ
//					pld.strFormat = _T("");
//				}
//				else
//				{
//					// タイプ
//					pld.strFormat = CString(sz_format);
//				}
//
//				// 取得先
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
//			// 局名
//			pld.strStationName = mStationString[i];
//			// 衛星名
//			pld.strSatellite = mSpaceString[j];
//			// パスID
//			pld.strPassId.Format(_T("170200-0%d%d0"), i, j);
//			// AOS
//			pld.tAOS = (local + CTimeSpan(0, i, 10 * (i + 1), 0)).GetTime();
//			// LOS
//			pld.tLOS = (local + CTimeSpan(0, i+1, 20 * (i + 1), 0)).GetTime();
//			// 予報値ファイル名
//			pld.strPredPath = _T("/CONFIG/prd/planet-c/1601100100.prd");
//
//			m_listPredList.push_back(pld);
//
//			// パスID
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
//-# 予報値一覧ファイルを読み込みデータ取得
//
//@param	なし
//@retval	1：成功 / -1：失敗
//
//*/
///*============================================================================*/
//BOOL CSatelliteData::SetPredList_Utf8()
//{
//	BOOL ret = TRUE;
//
//	m_listPredList.clear();
//
//	// 1局モード
//	if (theApp.GetSelectStationMode() == eStation_1)
//	{
//		ret = SetPredList_Utf8(theApp.GetSelectStation());
//	}
//	// 4局モード
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
//-# 指定局の予報値一覧ファイルを読み込みデータ取得(UTF8対応)
//
//@param	nStation：局番号
//@retval	1：成功 / -1：失敗
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
//	// ファイル更新日時の取得
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
//		// ファイルタイムに変化なし
//		return FALSE;
//	}
//	mGetSharePredFileTime[nStation] = time;
//
//
//	CTime	cTime;
//
//	if (_wfopen_s(&fp, strFilePath, _T("r, ccs=UTF-8")) != 0)
//	{
//		//		fprintf(stderr, "%s\n", "予報値一覧ファイルのオープンに失敗しました。");
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
//		// 新フォーマット読み込み
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
//			// 新フォーマット読み込み（タイプなし）
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
//			// 旧フォーマット読み込み
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
//				// 局名
//				pld.strStationName = CString(sz_stationname);
//			}
//
//			// 衛星名
//			pld.strSatellite = CString(sz_satname);
//
//			// パスID
//			pld.strPassId = CString(sz_pass);
//
//			// AOS時刻の変換
//			swprintf_s(sz_aos, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_a_date, sz_a_time, L".0000");
//			if (ub_asc_to_asctime(sz_aos, sz_aos_date) < 0) {
//				fclose(fp);
//				return FALSE;
//			}
//			cTime = StrTimeToCTime(CString(sz_aos_date));
//#if 1	// ダミー
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
//			// LOS時刻の変換
//			swprintf_s(sz_los, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_l_date, sz_l_time, L".0000");
//			if (ub_asc_to_asctime(sz_los, sz_los_date) < 0) {
//				fclose(fp);
//				return FALSE;
//			}
//			cTime = StrTimeToCTime(CString(sz_los_date));
//
//#if 1	// ダミー
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
//			// MAX EL時刻の変換
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
//			// 予報値ファイル名
//			pld.strPredPath = CString(sz_pred_path);
//
//			// オフセット
//			str = CString(sz_offset);
//			// 正負を抽出
//			long nSign = 1;
//			if (str.Left(1) == _T('-'))
//			{
//				nSign = -1;
//				str.Remove(_T('-'));
//			}
//			// 右から２つが秒
//			CString strSS = str.Right(2);
//			// 残りが分と":"なので秒と":"の文字数を除外し、分を出す
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
//					// タイプ
//					pld.strFormat = _T("");
//				}
//				else
//				{
//					// タイプ
//					pld.strFormat = CString(sz_format);
//				}
//
//				// 取得先
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
//			// 局名
//			pld.strStationName = mStationString[i];
//			// 衛星名
//			pld.strSatellite = mSpaceString[j];
//			// パスID
//			pld.strPassId.Format(_T("170200-0%d%d0"), i, j);
//			// AOS
//			pld.tAOS = (local + CTimeSpan(0, i, 10 * (i + 1), 0)).GetTime();
//			// LOS
//			pld.tLOS = (local + CTimeSpan(0, i + 1, 20 * (i + 1), 0)).GetTime();
//			// 予報値ファイル名
//			pld.strPredPath = _T("/CONFIG/prd/planet-c/1601100100.prd");
//
//			m_listPredList.push_back(pld);
//
//			// パスID
//			pld.strPassId.Format(_T("170200-9%d%d0"), i, j);
//			// AOS
//			pld.tAOS = (local + CTimeSpan(0, i + 1, 40 * (i + 1), 0)).GetTime();
//			// LOS
//			pld.tLOS = (local + CTimeSpan(0, i + 2, 50 * (i + 1), 0)).GetTime();
//
//			// 予報値ファイル名(ishi)
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

-# 予報値一覧ファイルを読み込みデータ取得

@param	なし
@retval	1：成功 / -1：失敗

*/
/*============================================================================*/
BOOL CSatelliteData::SetPredList_Euc()
{
	BOOL ret = FALSE;

	m_listPredList.clear();

	// 1局モード
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		UINT nStation = theApp.GetSelectStation();
		// 予報値一覧ファイルが更新されているか調べる
		if (IsPredListFileUpdated(nStation) == TRUE)
		{
			// 予報値一覧ファイルが更新されている場合、予報値一覧ファイル読み込み
			ret = SetPredList_Euc(nStation);
		}
	}
	// 4局モード
	else
	{
		BOOL bUpdated = FALSE;
		// 予報値一覧ファイルが更新されているか調べる
		for (UINT i = 0; i < eStation_MAX; i++)
		{
			if (IsPredListFileUpdated(i) == TRUE)
			{
				bUpdated = TRUE;
			}
		}

		// 予報値一覧ファイルが更新されている場合、予報値一覧ファイル読み込み
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

-# 指定局の予報値一覧ファイルを読み込みデータ取得(EUC対応)

@param	nStation：局番号
@retval	1：成功 / -1：失敗

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


	// ファイル更新日時の取得
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
//		// ファイルタイムに変化なし
//		return FALSE;
//	}
//	mGetSharePredFileTime[nStation] = time;

	CTime	cTime;

	if (_wfopen_s(&fp, strFilePath, _T("r")) != 0 || fp == NULL)
	{
		//		fprintf(stderr, "%s\n", "予報値一覧ファイルのオープンに失敗しました。");
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

		// 新フォーマット読み込み
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
			// 新フォーマット読み込み（タイプなし）
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
			// 旧フォーマット読み込み
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
				// 局名
				pld.strStationName = CString(sz_stationname);
			}

			// 衛星名
			pld.strSatellite = CString(sz_satname);

			// パスID
			pld.strPassId = CString(sz_pass);

			// AOS時刻の変換
			swprintf_s(sz_aos, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_a_date, sz_a_time, L".0000");
			if (ub_asc_to_asctime(sz_aos, sz_aos_date) < 0) {
				fclose(fp);
				return FALSE;
			}
			cTime = StrTimeToCTime(CString(sz_aos_date));
//#if 0	// ダミー
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

			// LOS時刻の変換
			swprintf_s(sz_los, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_l_date, sz_l_time, L".0000");
			if (ub_asc_to_asctime(sz_los, sz_los_date) < 0) {
				fclose(fp);
				return FALSE;
			}
			cTime = StrTimeToCTime(CString(sz_los_date));

//#if 0	// ダミー
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

			// MAX EL時刻の変換
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

			// 予報値ファイル名
			pld.strPredPath = CString(sz_pred_path);

			// オフセット
			str = CString(sz_offset);
			// 正負を抽出
			long nSign = 1;
			if (str.Left(1) == _T('-'))
			{
				nSign = -1;
				str.Remove(_T('-'));
			}
			// 右から２つが秒
			CString strSS = str.Right(2);
			// 残りが分と":"なので秒と":"の文字数を除外し、分を出す
			long nLength = str.GetLength();
			CString strMM = str.Left(nLength - (2 + 1));
			long nMM = _ttoi(strMM);
			long nSS = _ttoi(strSS);
			pld.tOffset = nSign * (nMM * 60 + nSS);

			if (nCol >= 25)
			{
				if (nCol == 25)
				{
					// タイプ
					pld.strFormat = _T("");
				}
				else
				{
					// タイプ
					pld.strFormat = CString(sz_format);
				}

				// 取得先
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
			// 局名
			pld.strStationName = mStationString[i];
			// 衛星名
			pld.strSatellite = mSpaceString[j];
			// パスID
			pld.strPassId.Format(_T("170200-0%d%d0"), i, j);
			// AOS
			pld.tAOS = (local + CTimeSpan(0, i, 10 * (i + 1), 0)).GetTime();
			// LOS
			pld.tLOS = (local + CTimeSpan(0, i + 1, 20 * (i + 1), 0)).GetTime();
			// 予報値ファイル名
			pld.strPredPath = _T("/CONFIG/prd/planet-c/1601100100.prd");

			m_listPredList.push_back(pld);

			// パスID
			pld.strPassId.Format(_T("170200-9%d%d0"), i, j);
			// AOS
			pld.tAOS = (local + CTimeSpan(0, i + 1, 40 * (i + 1), 0)).GetTime();
			// LOS
			pld.tLOS = (local + CTimeSpan(0, i + 2, 50 * (i + 1), 0)).GetTime();

			// 予報値ファイル名(ishi)
			pld.strPredPath.Format(_T("/CONFIG/prd/%s/1702009%d%d0.prd"), mSpaceString[j], i, j);

			m_listPredList.push_back(pld);


			pos++;
		}
	}
#endif

	// 予報値一覧ファイル時刻更新
	mGetSharePredFileTime[nStation] = time;

	return TRUE;
}


/*============================================================================*/
/*! CSatelliteData

-# 指定局の予報値一覧ファイルが更新されてるか調べる

@param  nStation：局番号
@retval	TRUE：更新されている / FALSE：更新されていない
*/
/*============================================================================*/
BOOL CSatelliteData::IsPredListFileUpdated(UINT nStation)
{
	CString strFilePath = theApp.GetShareFilePath(eFileType_PredList_File, nStation);

	// ファイル更新日時の取得
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
		// ファイルタイムに変化なし
		return FALSE;
	}

	// ファイルタイムに変化あり
	return TRUE;
}


/*============================================================================*/
/*! CSatelliteData

-# テキスト型のTimeをCTime型に変換する


@param  なし
@retval 時刻(CTime)
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
//-# 計画一覧を取得する
//
//
//@param  なし
//@retval なし
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
//		//		fprintf(stderr, "%s\n", "計画一覧ファイルのオープンに失敗しました。");
//		//		uc_opelog(NULL, NULL, BASE_OPE_NO + 15, NULL, NULL);
//		return FALSE;
//	}
//
//	m_listPlanList.clear();
//
//	fgets(sz_buff, sizeof(sz_buff), fp);                /*計画一覧ファイルのヘッダー部の読み飛ばし*/
//	while (fgets(sz_buff, sizeof(sz_buff), fp) != 0){    /*計画一覧ファイルの読み込み*/
//		stPlanListData pld;
//		// 新フォーマット読み込み
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
//			// 旧フォーマット読み込み
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
//			// 局名
//			pld.strStationName = CString(sz_stationname).MakeUpper();
//		}
//
//		// 衛星名
//		pld.strSatelliteName = CString(sz_satname);
//
//		// パスID
//		pld.strPassId = CString(sz_planned_pass);
//
//		// プリパス開始日時
//		sprintf_s(sz_pre, sizeof(sz_aos), "%s %s%s", sz_planned_itime1, sz_planned_itime2, ".0000");
//		if (ub_asc_to_asctime(sz_pre, sz_pre_date) < 0) {
//			fclose(fp);
//			return FALSE;
//		}
//		cTime = StrTimeToCTime(CString(sz_pre_date));
////		pld.tPrePassStart = cTime.GetTime() + i_offset;
//		pld.tPrePassStart = cTime.GetTime();
//
//		// パス開始日時
//		sprintf_s(sz_aos, sizeof(sz_aos), "%s %s%s", sz_planned_stime1, sz_planned_stime2, ".0000");
//		if (ub_asc_to_asctime(sz_aos, sz_aos_date) < 0) {
//			fclose(fp);
//			return FALSE;
//		}
//		cTime = StrTimeToCTime(CString(sz_aos_date));
////		pld.tPassStart = cTime.GetTime() + i_offset;
//		pld.tPassStart = cTime.GetTime();
//
//		// パス終了日時
//		sprintf_s(sz_los, sizeof(sz_los), "%s %s%s", sz_planned_etime1, sz_planned_etime2, ".0000");
//		if (ub_asc_to_asctime(sz_los, sz_los_date) < 0) {
//			fclose(fp);
//			return FALSE;
//		}
//		cTime = StrTimeToCTime(CString(sz_los_date));
////		pld.tPassEnd = cTime.GetTime() + i_offset;
//		pld.tPassEnd = cTime.GetTime();
//
//		// 予報値ファイル名
//		pld.strPredName = CString(sz_pred_name);
//
//		// チェック結果フラグ
//		pld.lValidFlag = i_valid_flag;
//
//		// 時刻オフセット追従条件フラグ
//		pld.lOfsFlag = i_ofs_flag;
//
//		// 時刻オフセット
//		pld.tOffset = i_offset;
//
//		if (nCol == 16)
//		{
//			// 取得先
//			CString strGetFrom = CString(sz_get_from);
//			if (strGetFrom.Compare(_T("enp")) == 0)
//			{
//				pld.strGetFrom = CString(_T("ENP"));
//			}
//			else if (strGetFrom.Compare(_T("sttr")) == 0)
//			{
//				pld.strGetFrom = CString(_T("局管制"));
//			}
//
//			// 運用種別
//			CString strPlanType = CString(sz_plan_type);
//			if (strPlanType.Compare(_T("maint")) == 0)
//			{
//				pld.strPlanType = CString(_T("局占有"));
//			}
//			else if (strPlanType.Compare(_T("plan")) == 0)
//			{
//				pld.strPlanType = CString(_T("実運用"));
//			}
//			else if (strPlanType.Compare(_T("vlbi")) == 0)
//			{
//				pld.strPlanType = CString(_T("VLBI"));
//			}
//			else if (strPlanType.Compare(_T("rdychk")) == 0)
//			{
//				pld.strPlanType = CString(_T("レディネスチェック"));
//			}
//
//			// 実行結果
//			CString strResultype = CString(sz_result_type);
//			if (strResultype.Compare(_T("ok")) == 0)
//			{
//				pld.strResult = CString(_T("正常終了"));
//			}
//			else if (strResultype.Compare(_T("ng")) == 0)
//			{
//				pld.strResult = CString(_T("異常終了"));
//			}
//			else if (strResultype.Compare(_T("unexec")) == 0)
//			{
//				pld.strResult = CString(_T("--------"));
//			}
//		}
//
//		// リストに追加
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
//			// 局名
//			pld.strStationName = mStationString[i];
//			// 衛星名
//			pld.strSatelliteName = mSpaceString[j];
//			// パスID
//			pld.strPassId.Format(_T("170200-0%d%d%d"), i, j, j + 1);
//			// パス開始日時
//			pld.tPassStart = (local + CTimeSpan(0, 0, 10 * (j + 1), 0)).GetTime();
//			// パス終了日時
//			pld.tPassEnd = (local + CTimeSpan(0, 1, 10 * (j + 1), 0)).GetTime();
//			// プリパス開始日時
//			pld.tPrePassStart = pld.tPassStart;
//			if (i == eStation_Usuda64 || i == eStation_Usuda642)
//				pld.tPrePassStart -= OperationTime_AntennaWait;
//			else
//				pld.tPrePassStart -= OperationTime_AntennaWait2;
//			// 予報値ファイル名
//			pld.strPredName = _T("/CONFIG/prd/planet-c/1601100100.prd");
//			// チェック結果フラグ
//			pld.lValidFlag = 0;
//			// 時刻オフセット追従条件フラグ
//			pld.lOfsFlag = 0;
//			// 時刻オフセット
//			pld.tOffset = 0;
//			// 取得先
//			pld.strGetFrom = (pos % 2 == 0) ? _T("ENP") : _T("局管制");
//			// 運用種別
//			if (pos % 3 == 0)	pld.strPlanType = _T("実運用");
//			if (pos % 3 == 1)	pld.strPlanType = _T("局占有");
//			if (pos % 3 == 2)	pld.strPlanType = _T("VLBI");
//			// 実行結果
//			if (pos % 3 == 0)	pld.strResult = _T("正常終了");
//			if (pos % 3 == 1)	pld.strResult = _T("異常終了");
//			if (pos % 3 == 2)	pld.strResult = _T("--------");
//			pld.strResult = _T("--------");
//
//			m_listPlanList.push_back(pld);
//
//			// パスID
//			pld.strPassId.Format(_T("170201-0%d%d%d"), i, j, j + 1);
//			// パス開始日時
//			pld.tPassStart = (local + CTimeSpan(0, 0, 15 * (j + 1), 0)).GetTime();
//			// パス終了日時
//			pld.tPassEnd = (local + CTimeSpan(0, 1, 15 * (j + 1), 0)).GetTime();
//			// プリパス開始日時
//			pld.tPrePassStart = pld.tPassStart;
//			if (i == eStation_Usuda64 || i == eStation_Usuda642)
//				pld.tPrePassStart -= OperationTime_AntennaWait;
//			else
//				pld.tPrePassStart -= OperationTime_AntennaWait2;
//			m_listPlanList.push_back(pld);
//
//			if (i == 0 && j == 0){
//				// パスID
//				pld.strPassId.Format(_T("179999-0%d%d%d"), i, j, j + 1);
//				// パス開始日時
//				pld.tPassStart = (local + CTimeSpan(0, 0, 20 * (j + 1), 0)).GetTime();
//				// パス終了日時
//				pld.tPassEnd = (local + CTimeSpan(0, 1, 20 * (j + 1), 0)).GetTime();
//				// プリパス開始日時
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
//-# 計画一覧を取得する
//
//@param	なし
//@retval	1：成功 / -1：失敗
//
//*/
///*============================================================================*/
//BOOL CSatelliteData::SetPlanList_Utf8()
//{
//	BOOL ret = TRUE;
//
//	m_listPlanList.clear();
//
//	// 1局モード
//	if (theApp.GetSelectStationMode() == eStation_1)
//	{
//		ret = SetPlanList_Utf8(theApp.GetSelectStation());
//	}
//	// 4局モード
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
//-# 指定局の計画一覧を取得する(UTF8対応）
//
//@param  nStation：局番号
//@retval	1：成功 / -1：失敗
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
//	// ファイル更新日時の取得
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
//		// ファイルタイムに変化なし
//		return FALSE;
//	}
//	mGetSharePlanFileTime[nStation] = time;
//
//
//	CTime	cTime;
//
//	if (_wfopen_s(&fp, strFilePath, _T("r, ccs=UTF-8")) != 0)
//	{
//		//		fprintf(stderr, "%s\n", "計画一覧ファイルのオープンに失敗しました。");
//		//		uc_opelog(NULL, NULL, BASE_OPE_NO + 15, NULL, NULL);
//		return FALSE;
//	}
//
//	fgetws(sz_buff, static_cast<unsigned>(_countof(sz_buff)), fp);                /*計画一覧ファイルのヘッダー部の読み飛ばし*/
//	while (fgetws(sz_buff, static_cast<unsigned>(_countof(sz_buff)), fp) != 0){    /*計画一覧ファイルの読み込み*/
//		stPlanListData pld;
//
//		// 新フォーマット読み込み
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
//			// 旧フォーマット読み込み
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
//			// 局名
//			pld.strStationName = CString(sz_stationname);
//		}
//
//		// 衛星名
//		pld.strSatelliteName = CString(sz_satname);
//
//		// パスID
//		pld.strPassId = CString(sz_planned_pass);
//
//		// プリパス開始日時
//		swprintf_s(sz_pre, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_planned_itime1, sz_planned_itime2, L".0000");
//		if (ub_asc_to_asctime(sz_pre, sz_pre_date) < 0) {
//			fclose(fp);
//			return FALSE;
//		}
//		cTime = StrTimeToCTime(CString(sz_pre_date));
//		pld.tPrePassStart = cTime.GetTime() + i_offset;
//
//		// パス開始日時
//		swprintf_s(sz_aos, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_planned_stime1, sz_planned_stime2, L".0000");
//		if (ub_asc_to_asctime(sz_aos, sz_aos_date) < 0) {
//			fclose(fp);
//			return FALSE;
//		}
//		cTime = StrTimeToCTime(CString(sz_aos_date));
//		pld.tPassStart = cTime.GetTime() + i_offset;
//
//		// パス終了日時
//		swprintf_s(sz_los, static_cast<unsigned>(_countof(sz_los)), L"%s %s%s", sz_planned_etime1, sz_planned_etime2, L".0000");
//		if (ub_asc_to_asctime(sz_los, sz_los_date) < 0) {
//			fclose(fp);
//			return FALSE;
//		}
//		cTime = StrTimeToCTime(CString(sz_los_date));
//		pld.tPassEnd = cTime.GetTime() + i_offset;
//
//		// 予報値ファイル名
//		pld.strPredName = CString(sz_pred_name);
//
//		// 計画ファイル名
//		CString strPlanPath = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
//		pld.strPlanName = strPlanPath + CString(sz_satname) + _T("\\") + CString(sz_planned_pass) + _T(".pln");
//
//		// チェック結果フラグ
//		pld.lValidFlag = i_valid_flag;
//
//		// 時刻オフセット追従条件フラグ
//		pld.lOfsFlag = i_ofs_flag;
//
//		// 時刻オフセット
//		pld.tOffset = i_offset;
//
//		if (nCol == 15)
//		{
//			pld.strGetFrom = CString(sz_get_from).MakeUpper();
//			pld.strPlanType = CString(sz_plan_type).MakeUpper();
//		}
//
//		// リストに追加
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

-# 計画一覧を取得する

@param	なし
@retval	1：成功 / -1：失敗

*/
/*============================================================================*/
BOOL CSatelliteData::SetPlanList_Euc()
{
	BOOL ret = FALSE;

	m_listPlanList.clear();

	// 1局モード
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		UINT nStation = theApp.GetSelectStation();
		// 計画一覧ファイルが更新されているか調べる
		if (IsPlanListFileUpdated(nStation) == TRUE)
		{
			// 計画一覧ファイルが更新されている場合、計画一覧ファイル読み込み
			ret = SetPlanList_Euc(nStation);
		}
	}
	// 4局モード
	else
	{
		BOOL bUpdated = FALSE;
		// 計画一覧ファイルが更新されているか調べる
		for (UINT i = 0; i < eStation_MAX; i++)
		{
			if (IsPlanListFileUpdated(i) == TRUE)
			{
				bUpdated = TRUE;
			}
		}

		// 計画一覧ファイルが更新されている場合、計画一覧ファイル読み込み
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

-# 指定局の計画一覧を取得する(EUC対応）

@param  nStation：局番号
@retval	1：成功 / -1：失敗
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

	// ファイル更新日時の取得
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
//		// ファイルタイムに変化なし
//		return FALSE;
//	}
//	mGetSharePlanFileTime[nStation] = time;

	CTime	cTime;

	if (_wfopen_s(&fp, strFilePath, _T("r")) != 0 || fp == NULL)
	{
		//		fprintf(stderr, "%s\n", "計画一覧ファイルのオープンに失敗しました。");
		//		uc_opelog(NULL, NULL, BASE_OPE_NO + 15, NULL, NULL);
		return FALSE;
	}

	char szline[2048];
	CString cbuf;
	fgets(szline, static_cast<unsigned>(_countof(szline)), fp);                /*計画一覧ファイルのヘッダー部の読み飛ばし*/
	while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != 0){    /*計画一覧ファイルの読み込み*/
		cbuf = CServerFile::ConvertUnicode(szline);
		stPlanListData pld;

		// 新フォーマット読み込み
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
			// レディネスチェック計画での予報値が無い場合を想定
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
			// 局名
			pld.strStationName = CString(sz_stationname);
		}

		// 衛星名
		pld.strSatelliteName = CString(sz_satname);

		// パスID
		pld.strPassId = CString(sz_planned_pass);

		// プリパス開始日時
		swprintf_s(sz_pre, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_planned_itime1, sz_planned_itime2, L".0000");
		if (ub_asc_to_asctime(sz_pre, sz_pre_date) < 0) {
			fclose(fp);
			return FALSE;
		}
		cTime = StrTimeToCTime(CString(sz_pre_date));
		pld.tPrePassStart = cTime.GetTime() + i_offset;

		// パス開始日時
		swprintf_s(sz_aos, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_planned_stime1, sz_planned_stime2, L".0000");
		if (ub_asc_to_asctime(sz_aos, sz_aos_date) < 0) {
			fclose(fp);
			return FALSE;
		}
		cTime = StrTimeToCTime(CString(sz_aos_date));
		pld.tPassStart = cTime.GetTime() + i_offset;

		// パス終了日時
		swprintf_s(sz_los, static_cast<unsigned>(_countof(sz_los)), L"%s %s%s", sz_planned_etime1, sz_planned_etime2, L".0000");
		if (ub_asc_to_asctime(sz_los, sz_los_date) < 0) {
			fclose(fp);
			return FALSE;
		}
		cTime = StrTimeToCTime(CString(sz_los_date));
		pld.tPassEnd = cTime.GetTime() + i_offset;

		// 予報値ファイル名
		if (nCol == 15)
		{
			pld.strPredName = CString(sz_pred_name);
		}
		else
		{
			pld.strPredName = _T("");
		}

		// チェック結果フラグ
		pld.lValidFlag = i_valid_flag;

		// 時刻オフセット追従条件フラグ
		pld.lOfsFlag = i_ofs_flag;

		// 時刻オフセット
		pld.tOffset = i_offset;

//		if (nCol == 15)
		{
			pld.strGetFrom = CString(sz_get_from).MakeUpper();
			pld.strPlanType = CString(sz_plan_type).MakeUpper();
		}

		// 計画ファイル名
		CString strPlanPath = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
		if (pld.strGetFrom.CompareNoCase(_T("ENP")) == 0)
		{
			pld.strPlanName = strPlanPath + _T("enp\\")  + CString(sz_satname) + _T("\\") + CString(sz_planned_pass) + _T(".pln");
		}
		else
		{
			pld.strPlanName = strPlanPath + _T("sttn\\") + CString(sz_satname) + _T("\\") + CString(sz_planned_pass) + _T(".pln");
		}

		// リストに追加
		m_listPlanList.push_back(pld);
	}

	fclose(fp);

	// 計画一覧ファイル時刻更新
	mGetSharePlanFileTime[nStation] = time;

	return TRUE;
}


/*============================================================================*/
/*! CSatelliteData

-# 指定局の計画一覧ファイルが更新されてるか調べる

@param  nStation：局番号
@retval	TRUE：更新されている / FALSE：更新されていない
*/
/*============================================================================*/
BOOL CSatelliteData::IsPlanListFileUpdated(UINT nStation)
{
	CString strFilePath = theApp.GetShareFilePath(eFileType_PlanList_File, nStation);

	// ファイル更新日時の取得
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
		// ファイルタイムに変化なし
		return FALSE;
	}

	// ファイルタイムに変化あり
	return TRUE;
}


/*============================================================================*/
/*! CSatelliteData

-# 計画一覧に予報値ファイルが使われているか調べる

@param	strFilePath：予報値ファイル
@retval TRUE: 使われている。 FALSE: 使われていない。

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

-# 局名（簡単）から局IDを取得する


@param  なし
@retval なし
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

-# 局名（通常）から局IDを取得する


@param  なし
@retval なし
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

-# サマリプロパティのデータを生成する


@param  なし
@retval なし
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

			// ウィンドウポインタをコピー
			prop.wnd = (LPARAM)this;

			if (nStation == eStation_Usuda642 && nView == eSummaryWndSetType_KASP)
			{
				nGraphIdx = eSummaryWndSetType_SAGC;
			}
			else
			{
				nGraphIdx = nView;
			}
			// グラフ番号をコピー
			prop.nGraphIdx = nGraphIdx;

			if (!prop.m_bSetted)
			{
				prop.wndIdx = nView;									// ウィンドー番号
				prop.wndName = listViewName[nGraphIdx];						// ウィンドー名称

				prop.isRealTime = true;									// リアルタイム（true）/ ノンリアルタイム（false）
				prop.trandVScale.Auto = true;							// 縦軸左 オートスケール
				prop.trandVScale.VScalLMax = initSummaryPropData.VScalLMax;	// 縦軸左 最大値
				prop.trandVScale.VScalLMin = initSummaryPropData.VScalLMin;	// 縦軸左 最小値
				prop.trandVScale.VSCalItvl = initSummaryPropData.VSCalItvl;	// 縦軸目盛りの間隔

				prop.trandHScale[0] = m1;			// 横軸スケール

				CString str = _T("");
				for (int nSigNo = 0; nSigNo < nListMax; ++nSigNo)
				{
					prop.dataName[nSigNo] = mListSummarySig[nStation][nGraphIdx][nSigNo].strSigName;			// データ名称
					prop.dataGName[nSigNo] = mListSummarySig[nStation][nGraphIdx][nSigNo].strSimpleSigName;	// 簡易データ名称
					prop.dataUnit[nSigNo] = mListSummarySig[nStation][nGraphIdx][nSigNo].strUnit;			// 単位
					prop.isViewMode[nSigNo] = true;		// 表示On/Off
					prop.drawMode[nSigNo] = Line;		// 描画モード（Line、Dots、L&D）
					prop.color[nSigNo] = colordef[nSigNo];	// 表示色
					prop.lineWidth[nSigNo] = 1;			// 線幅
					prop.dotSize[nSigNo] = Small;		// ドットサイズ
					prop.vscaleKind[nSigNo] = LScale;	// 縦軸スケール選択
				}
			}
			SetSummaryPropertiesData(nStation, nView, prop);
		}
	}

#endif

}


/*============================================================================*/
/*! CSatelliteData

-# 設備詳細制御機能

@param  strCtrl		:設備名称
@param  strParam	:パラメータ
@retval 制御送信値。キャンセル時は空文字を返す。
*/
/*============================================================================*/
CString CSatelliteData::opctproc(CString strCtrl, CString strParam, CString val/* = _T("")*/)
{
	CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("opctproc"), strCtrl+","+strParam, _T(""), nLogEx::info);

	if (strParam != _T("") && strParam != _T("-nd"))
	{
		/* 制御通知を送信 */
		theApp.SendControl(strCtrl, strParam, _T(""), _T(""));
		return strParam;
	}
	else if (IsBitRateCtrl(strCtrl))
	{
		/* ビットレート変更 */
		return CtrlBitRate(strCtrl);
	}
	else
	{
		/* ステータス/数値・時刻/確認に振り分け */
		CString str = _T("");
		stControlValue cval;
		cval.fval = _wtof(val);
		int ret = param_chk(strCtrl, &cval);

		// 制御通知を送信
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
			// 制御種別不一致の場合、制御送信しない
			return _T("");
		}

		theApp.SendControl(strCtrl, str, _T(""), _T(""));
		return str;
	}

	// 制御送信キャンセル
	return _T("");
}

/*============================================================================*/
/*! CSatelliteData

-# 制御がコマンドビットレートか確認

@param  strCtrl		:制御名
@retval TRUE：ビットレート、FALSE：その他
*/
/*============================================================================*/
BOOL CSatelliteData::IsCmdBitRateCtrl(CString strCtrl)
{
	if (theApp.GetSelectStation() == eStation_Usuda64)
	{
		// 臼田64
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
		// 臼田54
		if (    strCtrl.CompareNoCase(C_BIT4) == 0
			|| (strCtrl == C_BIT)
			|| (strCtrl == C_BIT_X))
		{
			return TRUE;
		}
	}
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
	{
		// 内之浦34
		if ((strCtrl == C_BIT)
			|| (strCtrl == C_BIT_X2))
		{
			return TRUE;
		}
	}
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
	{
		// 内之浦20
		if (strCtrl == C_BIT)
		{
			return TRUE;
		}
	}

	return FALSE;
}
/*============================================================================*/
/*! CSatelliteData

-# 制御がコマンドビットレートか確認

@param  strCtrl		:制御名
@retval TRUE：ビットレート、FALSE：その他
*/
/*============================================================================*/
BOOL CSatelliteData::IsBitRateCtrl(CString strCtrl)
{
	if (theApp.GetSelectStation() == eStation_Usuda64)
	{
		// 臼田64
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
		// 臼田54
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
		// 内之浦34
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
		// 内之浦20
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

-# 制御がPLL BWか確認

@param  strCtrl		:制御名
@retval TRUE:PLL BW制御　／ FALSE:PLL BW制御でない
*/
/*============================================================================*/
BOOL CSatelliteData::IsPllBwCtrl(CString strCtrl)
{
	if (theApp.GetSelectStation() == eStation_Usuda64)
	{
		// 臼田64
		if ((strCtrl == SRCV_PLLBW)
			|| (strCtrl == XRCV_PLLBW)
			|| (strCtrl == XRCVOLD_PLLBW))
		{
			return TRUE;
		}
	}
	else if (theApp.GetSelectStation() == eStation_Usuda642)
	{
		// 臼田54
		if ((strCtrl == SRCV_PLLBW)
			|| (strCtrl == XRCV_PLLBW)
			|| (strCtrl == XRCVOLD_PLLBW))
		{
			return TRUE;
		}
	}
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
	{
		// 内之浦34
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
		// 内之浦20
	}

	return FALSE;
}


/*============================================================================*/
/*! CSatelliteData

-# 設備詳細制御機能

@param  strCtrl		:設備名称
@param  strParam	:パラメータ
@param  strCtrl2    :設備名称2(デフォルトは空)
@retval なし
*/
/*============================================================================*/
void CSatelliteData::SendControlSelect(CString strCtrl, stControlValue* pval, CString strCtrl2 /*= _T("")*/)
{
	if (IsBitRateCtrl(strCtrl))
	{
		/* ビットレート変更 */
		CtrlBitRate(strCtrl);

		// ビットレートの制御は、CtrlBitRate関数内で送信している。
		// 戻り先で制御を送信しない為に0を設定する。
		pval->ret = 0;
	}
	else if (IsPllBwCtrl(strCtrl))
	{
		/* ステータス/数値・時刻/確認に振り分け(PLL BW) */
		param_chk(strCtrl, pval, CURRENT_LBW);
	}
	else
	{
		/* ステータス/数値・時刻/確認に振り分け */
		param_chk(strCtrl, pval, _T(""), FALSE, strCtrl2);
	}
}


/*============================================================================*/
/*! CSatelliteData

-# 制御選択登録

@param  strCtrl		:設備名称
@param  pval		:パラメータ
@retval 制御種別
*/
/*============================================================================*/
int CSatelliteData::RegistControlSelect(CString strCtrl, stControlValue* pval)
{
	int ret = -1;
	if (IsBitRateCtrl(strCtrl))
	{
		/* ビットレート変更 */
		if (CtrlBitRate(strCtrl, pval, TRUE) != _T(""))
			ret = eControlNumeric;
			
		// ビットレートの制御は、CtrlBitRate関数内で送信している。
		// 戻り先で制御を送信しない為に0を設定する。
		pval->ret = 0;
	}
	else if (IsPllBwCtrl(strCtrl))
	{
		/* ステータス/数値・時刻/確認に振り分け(PLL BW) */
		ret = param_chk(strCtrl, pval, CURRENT_LBW, TRUE);
	}
	else
	{
		/* ステータス/数値・時刻/確認に振り分け */
		ret = param_chk(strCtrl, pval, _T(""), TRUE);
	}

	return ret;
}


/*============================================================================*/
/*! CSatelliteData

-# 詳細制御画面-ビットレート変更処理

@param  strCtrl		:設備名称
@param  pval		:パラメータ
@param	bSeqCtrl:TRUE:逐次制御である / FALSE:逐次制御でない（デフォルト）
@retval 送信した制御パラメータ（送信失敗時も制御パラメータを返す）。キャンセル時は空文字。
*/
/*============================================================================*/
CString CSatelliteData::CtrlBitRate(CString strCtrl, stControlValue* pval /*= NULL*/, BOOL bSeqCtrl /*= FALSE*/)
{
	CEquipmentCtrlBitRate dlg;
	if (bSeqCtrl)
	{
		dlg.SetWindowTitle(_T("制御登録"));
	}

	dlg.SetBitRateCtrl(strCtrl);

	// CMD-BITRATEの場合は、時刻入力が出来ないようにする。
	BOOL bCmdBitRate = FALSE;
	if (IsCmdBitRateCtrl(strCtrl))
	{
		bCmdBitRate = TRUE;
	}
	dlg.SetCmdBitRateFlg(bCmdBitRate);

	// ダイアログ起動
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
		// 逐次制御でない場合、送信する
		if (st.bTime == TRUE)
		{
			// 時刻設定チェックボックスにチェックが付いている場合
			time = st.time.Format("%Y-%m-%d %H:%M:%S");
			msg.Format(_T("%s,%s,%s"), (LPCTSTR)st.bitrate, (LPCTSTR)st.satellite, (LPCTSTR)time);
			theApp.SendControlNoUI(strCtrl, msg, _T(""), _T(""));
		}
		else if (!bCmdBitRate)
		{
			// 『日時指定』は活性なのでカンマは必要
			msg.Format(_T("%s,%s,"), (LPCTSTR)st.bitrate, (LPCTSTR)st.satellite);
			theApp.SendControlNoUI(strCtrl, msg, _T(""), _T(""));
		}
		else
		{
			// 『日時指定』は非活性なのでカンマは不要
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

-# 設備詳細制御機能

@param  strCtrl		 :設備名称
@param  strParam	 :パラメータ
@param	strAddStatus :追加ステータス文字列（デフォルトは空）
@param	bSeqCtrl:TRUE:逐次制御である / FALSE:逐次制御でない（デフォルト）
@param  strCtrl2     :設備名称2(デフォルトは空)
@retval 制御種別

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
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		//		CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("param_chk"), _T("ControlName None"), _T(""), nLogEx::error);
		CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("param_chk"), _T("ControlName None"), strCtrl, nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		pval->ret = false;
		return ret;
	}

	/*----- 設備制御項目ステータスチェック ----*/
	l_rc = m_dbaccess.ud_adr_to_resvec((char*)ctrl_adr, PARAMTBL_ID, ENDOFCTL_ID, ENDOFPARAM_ID, (char**)param_adr, 1);

	if (l_rc < 1)
	{
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("param_chk"), _T("Control Error"), strCtrl, nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		pval->ret = false;
		return ret;
	}

	if (param_adr[0]->l_intype == IN_S_ID && param_adr[0]->l_cvttype == CVT_S_ID)
	{
		l_rc = m_dbaccess.ud_adr_to_resvec((char*)ctrl_adr, STS_ID, ENDOFCTL_ID, ENDOFPARAM_ID, (char**)sts_adr, 33);

		// 制御種別とパラメタが異なっている場合はなにもしない
		if (pval->type != 0 || l_rc <= 0){
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("param_chk"), _T("ControlType Mismatch"), strCtrl, nLogEx::error);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
			pval->ret = false;
			return ret;
		}

		CEquipmentCtrlStatus dlg;
		if (bSeqCtrl)
		{
			// 逐次制御の場合、ウィンドウタイトル変更
			dlg.SetWindowTitle(_T("制御登録"));
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
	/*----- 予報値ファイル項目チェック -----*/
	else if (param_adr[0]->l_intype == IN_PRED_ID)
	{
	}
	/*----- 設備制御項目チェック ----*/
	else if (param_adr[0]->l_intype == IN_S_ID && param_adr[0]->l_cvttype == CVT_STR_ID)
	{
		// 時刻形式文字列(MM:SS)
		CEquipmentCtrlTime dlg;
		if (bSeqCtrl)
		{
			// 逐次制御の場合、ウィンドウタイトル変更
			dlg.SetWindowTitle(_T("制御登録"));
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
	/* 時刻形式文字列(HH:MM:SS) */
	else if (param_adr[0]->l_intype == IN_TIME_ID)
	{
		CEquipmentCtrlTime dlg;
		if (bSeqCtrl)
		{
			// 逐次制御の場合、ウィンドウタイトル変更
			dlg.SetWindowTitle(_T("制御登録"));
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
	/* 通算日形式文字列(DDD-HH:MM:SS) */
	else if (param_adr[0]->l_intype == IN_DATE_ID)
	{
		CEquipmentCtrlTime dlg;
		if (bSeqCtrl)
		{
			// 逐次制御の場合、ウィンドウタイトル変更
			dlg.SetWindowTitle(_T("制御登録"));
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
	/* 日付形式文字列(YYYY-MM-DD) */
	else if (param_adr[0]->l_intype == IN_YEAR_ID)
	{
		CEquipmentCtrlTime dlg;
		if (bSeqCtrl)
		{
			// 逐次制御の場合、ウィンドウタイトル変更
			dlg.SetWindowTitle(_T("制御登録"));
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
	/* パラメータなし */
	else if (param_adr[0]->l_intype == IN_NONE_ID)
	{
	}
	/* その他 */
	else
	{
		l_rc = m_dbaccess.ud_adr_to_resvec((char*)ctrl_adr, VAL_ID, ENDOFCTL_ID, ENDOFPARAM_ID, (char**)val_adr, 1);
		pval->ret = false;

		if (l_rc < 1)
		{
			val_adr[0] = NULL;
		}

		/* 範囲表示 */
		if (val_adr[0] == NULL) 
		{
			strcpy_s(sz_work, sizeof(sz_work), "範囲値無し");
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
			strcat_s(sz_work, sizeof(sz_work), " ～ ");
			strcat_s(sz_work, sizeof(sz_work), sz_work2);
		}
		else 
		{
			sprintf_s(sz_work2, "%f", val_adr[0]->d_min);
			l_rc = opctcheck(param_adr[0], NULL, sz_work2, 0);
			strcpy_s(sz_work, sizeof(sz_work), sz_work2);
			sprintf_s(sz_work2, "%f", val_adr[0]->d_max);
			l_rc = opctcheck(param_adr[0], NULL, sz_work2, 0);
			strcat_s(sz_work, sizeof(sz_work), " ～ ");
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
					// 逐次制御の場合、ウィンドウタイトル変更
					dlg.SetWindowTitle(_T("制御登録"));
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
					// 逐次制御の場合、ウィンドウタイトル変更
					dlg.SetWindowTitle(_T("制御登録"));
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

-# 設備詳細制御機能

@param  strCtrl1	 :設備名称1
@param  strParam1	 :パラメータ1
@param  strCtrl2	 :設備名称2
@param  strParam2	 :パラメータ2
@retval 制御種別

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
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("param_chk2"), _T("ControlName None"), strCtrl, nLogEx::error);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
			return ret;
		}
	}

	if (stEquip[0].strEquipment.IsEmpty() == false && stEquip[1].strEquipment.IsEmpty() == false)
	{
		CEquipmentCtrl2Num dlg;
		// 逐次制御の場合、ウィンドウタイトル変更
		dlg.SetWindowTitle(_T("設備詳細制御 RA/DEC OFFSET"));		
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
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("GetEquipmentParam"), _T("ControlName None"), strCtrl, nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return ret;
	}

	/*----- 設備制御項目ステータスチェック ----*/
	l_rc = m_dbaccess.ud_adr_to_resvec((char*)ctrl_adr, PARAMTBL_ID, ENDOFCTL_ID, ENDOFPARAM_ID, (char**)param_adr, 1);

	if (l_rc < 1)
	{
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("GetEquipmentParam"), _T("Control Error"), strCtrl, nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return ret;
	}

	if (param_adr[0]->l_intype == IN_S_ID && param_adr[0]->l_cvttype == CVT_S_ID)
	{
	}
	/*----- 予報値ファイル項目チェック -----*/
	else if (param_adr[0]->l_intype == IN_PRED_ID)
	{
	}
	/*----- 設備制御項目チェック ----*/
	else if (param_adr[0]->l_intype == IN_S_ID && param_adr[0]->l_cvttype == CVT_STR_ID)
	{
	}
	/* 時刻形式文字列(HH:MM:SS) */
	else if (param_adr[0]->l_intype == IN_TIME_ID)
	{
	}
	/* 通算日形式文字列(DDD-HH:MM:SS) */
	else if (param_adr[0]->l_intype == IN_DATE_ID)
	{
	}
	/* 日付形式文字列(YYYY-MM-DD) */
	else if (param_adr[0]->l_intype == IN_YEAR_ID)
	{
	}
	/* パラメータなし */
	else if (param_adr[0]->l_intype == IN_NONE_ID)
	{
	}
	/* その他 */
	else
	{
		l_rc = m_dbaccess.ud_adr_to_resvec((char*)ctrl_adr, VAL_ID, ENDOFCTL_ID, ENDOFPARAM_ID, (char**)val_adr, 1);

		if (l_rc < 1)
		{
			val_adr[0] = NULL;
		}

		/* 範囲表示 */
		if (val_adr[0] == NULL)
		{
			strcpy_s(sz_work, sizeof(sz_work), "範囲値無し");
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
			strcat_s(sz_work, sizeof(sz_work), " ～ ");
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
			strcat_s(sz_work, sizeof(sz_work), " ～ ");
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

	/*----- 設備制御項目ステータスチェック ----*/
	l_rc = m_dbaccess.ud_adr_to_resvec((char*)ctrl_adr, PARAMTBL_ID, ENDOFCTL_ID, ENDOFPARAM_ID, (char**)param_adr, 1);

	if (l_rc < 1)
	{
		CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("param_chk"), _T("Control Error"), strCtrl, nLogEx::error);
		return ret;
	}

	if (param_adr[0]->l_intype == IN_S_ID && param_adr[0]->l_cvttype == CVT_S_ID)
	{
		l_rc = m_dbaccess.ud_adr_to_resvec((char*)ctrl_adr, STS_ID, ENDOFCTL_ID, ENDOFPARAM_ID, (char**)sts_adr, 33);

		// 制御種別とパラメタが異なっている場合はなにもしない
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

-# 設備詳細制御機能

@param  val_adr;		上下限値チェックテーブル
@param  sz_indata[];	入力文字列
@param  i_signflag;		符号付加フラグ
@retval なし
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

	/*----- 自プロセスのＩＰＣキーを取得 -----*/

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
		/*-- 整数データ入力                              --*/
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
		/*-- １６進数入力                                --*/
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
		/*-- 文字列入力                                  --*/
		/*-------------------------------------------------*/
	case IN_S_ID:
		if (strlen(sz_indata) > param_adr->l_incolint) {
			return -3;
		}
		break;
		/*-------------------------------------------------*/
		/*-- フローティング入力                          --*/
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
				else if (i_signflag == 2) { /* #2010 2011-02-25 アンテナ角度設定制御用　特殊処理(ゼロサプレス) */
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
		/*-- 時刻入力処理 (HH:MM:SS)                     --*/
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
		/*-- 時刻入力処理  (DDD HH:MM:SS)                --*/
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
		/*-- 時刻入力処理  (YYYY-MM-DD)                  --*/
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
@name     : 数値文字列チェック＆バイナリ値変換処理
@改定履歴 : 92-05-29 Ver1.0 1st coding  mika
@改定履歴 : 92-07-03 Ver1.1   mika
@改定履歴 : 92-08-17 Ver1.2   mika
@改定履歴 : 97-01-16 Ver2.0   T.S.
@改定履歴 : 97-12-17 Ver2.1   T.S. (K/M対応)
@改定履歴 : 98-04-09 Ver2.2   T.S. (8086対応)
@改定履歴 : 2000-08-23 Ver2.4 ８進数(0t777,0T777)対応
@処理概要 : 変換する文字列を与え数値文字列チェックとバイナリ値変換を行う
@            1) s        ---<  input : sz_ :  n-byte : >---
@                             変換する文字列
@            2)i_p       ---<  output : long  :  1  :  >---
@                             変換された4バイトのバイナリデータ
@                             (i_p=NULLの場合は結果は戻さない)
@            3)d_p       ---<  output  : double : 1  : >---
@                             変換された8バイトのdoubleデータ
@                             (d_p=NULLの場合は結果は戻さない)

@                             ('0x'+16進数, '0b'+2進数, '0t'+8進数の
@                              場合は符号なしデータとみなすので注意)

@                             このポインタ値が -1 の場合は第４パラ
@                             メータ(l_opt)が有効になる

@            4)l_opt     ---<  input  :  long  :  1  : >---
@                             1000 または、1024 を指定すると、数値
@                             の後ろに K M が付加されていた場合に
@                             変換値として、K:1000倍または1024倍
@                             M:1000*1000倍または1024*1024倍の値
@                             を返す
@
@戻り値   :    (チェック/変換されたデータの型を示す)
@              -1 : 数値ではない
@                          (変換結果はもどさなかった)
@               0 : sがNULLポインタであった
@                          (変換結果はもどさなかった)
@               1 : long型の数値として変換した
@               (10進数, '0x'+16進数, '0b'+2進数の場合)
@               (8進数, '0t'+8進数の場合)
@
@               2 : double型の数値として変換した
@                   (999.99, 99.9e99形式の場合)
@                   (longではオーバーフローする場合)
@
@補足     :
*/
//char    *s;                             /* 変換する文字列 */
//INT32bit *i_p;                           /* 変換された4バイトのバイナリデータ */
//double  *d_p;                           /* 変換された8バイトのdoubleデータ   */
//INT32bit l_opt;                          /* d_p==-1の時のみ有効*/

int CSatelliteData::uc_valchk(char *s, int *i_p, double *d_p, int l_opt)
{
	int     i = 2;                            /* 文字列カウンタ */
	int     j = 0;                            /* カウンタ */
	char    sz_dmy[512];                    /* sscanfダミーバッファ */

	long    l_bitbuf = 0;                     /* ビットシフトバッファ */
	double  d_bitbuf = 0.0;                   /* 32bit以上の場合 */
	long    l_data;                         /* 計算結果データ  */
	double  d_data;

	union   umoji  mojibuf;                 /* 文字列先頭2文字のバッファ */



	/*********************************************************************/
	/*                             PROCEDURE                             */
	/*********************************************************************/

	/* sがNULLか？ */
	if (s == NULL)  return(0);

	/* 文字列先頭2文字の取得 */
	//strncpy(mojibuf.c_moji, 2, s);
	//strncpy_s(mojibuf.c_moji, 2, s, 2);
	memcpy(mojibuf.c_moji, s, 2);

	/* 文字列先頭2文字により処理を選択 */
	switch (mojibuf.s_moji) {

	case LX: /* 0X */
	case SX: /* 0x */
		while (s[i] != iNULL) {  /* 2005-1-7 */
			l_bitbuf = l_bitbuf << 4;    /* 左へ4ビットシフト */
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
			l_bitbuf = l_bitbuf << 1;    /* 左へ1ビットシフト */
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
			l_bitbuf = l_bitbuf << 3;    /* 左へ3ビットシフト */
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
				; /*d_dataをそのまま返す*/
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
			return(1); /*整数値*/
		}
		//if (sscanf(s, "%lf%s", &d_data, sz_dmy) != 1){ return(-1); }
		if (sscanf_s(s, "%lf%s", &d_data, sz_dmy, (int)sizeof(sz_dmy)) != 1){ return(-1); }
		if ((d_data >  2147483647.0) || (d_data < -2147483648.0)){
			if (i_p != NULL){ *i_p = 0x80000000; }
			if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_data; }
			return(2); /*integerがオーバーフローするので実数値*/
		}
		else{
			/* 整数値? */
			if ((strchr(s, '.') == NULL) && (strchr(s, 'e') == NULL) && (strchr(s, 'E') == NULL)){
				if (i_p != NULL){ *i_p = (int)d_data; }
				if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_data; }
				return(1); /*整数値*/
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

-# 設備詳細制御機能

@param  strCtrl		:設備名称
@param  strParam	:パラメータ
@retval なし
*/
/*============================================================================*/
void CSatelliteData::opctproc2(CString strCtrl, CString strParam)
{
	CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("opctproc2"), strCtrl + "," + strParam, _T(""), nLogEx::info);

	if (strParam != _T("") && strParam != _T("-nd"))
	{
		/* 制御通知を送信 */
		theApp.SendControlNoUI(strCtrl, strParam, _T(""), _T(""));
	}
	else
	{
		/* 制御通知を送信 */
		theApp.SendControlNoUI(strCtrl, _T(""), _T(""), _T(""));
	}
}

/*============================================================================*/
/*! CSatelliteData

-# 局運用状況テーブルアクセス

@param  なし
@retval なし
*/
/*============================================================================*/
int CSatelliteData::stn_tbl_access()
{
	m_dbaccess.eqctl_db_read();
	return 0;
}

/*============================================================================*/
/*! CSatelliteData

-# CIDファイル読込

@param  sz_watch[]		監視項目名称 [IN]
BOOL ReadCIDFile();
@retval -1：異常終了/ 0：正常終了
*/
/*============================================================================*/
BOOL CSatelliteData::ReadCIDFile()
{
	const CString mEmptyString = CID_INVALID;

	FILE *fp;
	TCHAR sz_buff[500];

	CString strFilePath = theApp.GetShareFilePath(eFileType_CID_Folder, theApp.GetSelectStation());
	CString tempPath = strFilePath + _T("*");

	// 対象フォルダ内のファイル情報を取得する
	// 指定されたファイル名に一致するファイルを、ディレクトリ内で検索する
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

			// ディレクトリ名取得
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
	} while (FindNextFile(hFindFile, &ffdata)); // ファイルがなくなるまで続ける


	map<CString, stCIDPrmEle> &cidEleList = theApp.GetSatelliteData().m_CIDSlctData.GetCidElementList();
	cidEleList.clear();

	map<CString, map<CString, stCIDParam>> &cidlist = theApp.GetSatelliteData().m_CIDSlctData.GetCidParamList();
	cidlist.clear();
		
	map<CString, stCIDPrmFlg> &cidPrmFlglist = theApp.GetSatelliteData().m_CIDSlctData.GetCidPrmFlgList();
	cidPrmFlglist.clear();


	bool bRead = false;

	// 衛星毎のCIDファイル読込
	vector<CString>& slist = m_CIDSlctData.GetSatelliteNameList();
	for (auto itr = slist.begin(); itr != slist.end(); ++itr)
	{
		CString sat = (CString)(*itr);	// 衛星名

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
			// 次のフォルダを調べる
			continue;
		}
//#if 0	// ヘッダー行が有る場合は、有効にすべし
//		// ヘッダー部読み飛ばし
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
				case 0:		// CID番号
					stcid.CID = item;
					AddDataVector(stele.CID, item);
					break;

				case 1:		// パラメータ番号
					prmIdx = _wtoi(item);
					break;

				case 2:		// パラメータ名
					break;

				case 3:		// パラメータ有効フラグ
					{
						int nflg = _wtoi(item);
						switch (prmIdx)
						{
						case 1:	// S帯テレメトリビットレート
							stpflg.TlmBitRateS = nflg;
							break;
						case 2:	// X帯テレメトリビットレート
							stpflg.TlmBitRateX = nflg;
							break;
						case 3:	// X高速テレメトリビットレート
							stpflg.TlmBitRateHX = nflg;
							break;
						case 4:	// Ka帯テレメトリビットレート 
							stpflg.TlmBitRateKa = nflg;
							break;
						case 5:	// S帯コマンドビットレート
							stpflg.CmdBitRateS = nflg;
							break;
						case 6:	// X帯コマンドビットレート
							stpflg.CmdBitRateX = nflg;
							break;
						case 7:	// 送信電力
							stpflg.TrsPower = nflg;
							break;
						case 8:	// アップリンク掃引パラメータ
							stpflg.UpLinkSweepShape = nflg;
							break;
						case 9:	// レンジ計測パラメータ モード
							stpflg.RngMesMode = nflg;
							break;
						case 10:// S帯キャリア受信ループバンド
							stpflg.CarrLoopBandS = nflg;
							break;
						case 11:// X帯キャリア受信ループバンド
							stpflg.CarrLoopBandX = nflg;
							break;
						case 12:// Ka帯キャリア受信ループバンド
							stpflg.CarrLoopBandKa = nflg;
							break;
						case 13:// コマンド変調度
							stpflg.CmdMod = nflg;
							break;
						case 14:// コマンドサブキャリア周波数
							stpflg.CmdSubCarrFreq = nflg;
							break;
						case 15:// 送信ドップラ補償の有無
							stpflg.TransDoppler = nflg;
							break;
						case 16:// アンテナ自動追尾
							stpflg.AntAutoTrack = nflg;
							break;
						case 17:// 変調方式
							stpflg.Modulation = nflg;
							break;
						case 18:// アンテナ待ち受け指向の有無
							stpflg.AntAosPosn = nflg;
							break;
						case 19:// アンテナ待機位置指向の有無
							stpflg.AntWaitPoint = nflg;

							// 衛星別のパラメータ活性非活性情報を付加していく
							stpflgSat.TlmBitRateS |= stpflg.TlmBitRateS;				// テレメトリビットレート S帯
							stpflgSat.TlmBitRateX |= stpflg.TlmBitRateX;				// テレメトリビットレート X帯
							stpflgSat.TlmBitRateHX |= stpflg.TlmBitRateHX;				// テレメトリビットレート X高速
							stpflgSat.TlmBitRateKa |= stpflg.TlmBitRateKa;				// テレメトリビットレート Ka帯
							stpflgSat.CmdBitRateS |= stpflg.CmdBitRateS;				// コマンドビットレート S帯
							stpflgSat.CmdBitRateX |= stpflg.CmdBitRateX;				// コマンドビットレート X帯
							stpflgSat.TrsPower |= stpflg.TrsPower;						// 送信電力
							stpflgSat.CarrLoopBandS |= stpflg.CarrLoopBandS;			// キャリア受信ループバンド S帯
							stpflgSat.CarrLoopBandX |= stpflg.CarrLoopBandX;			// キャリア受信ループバンド X帯
							stpflgSat.CarrLoopBandKa |= stpflg.CarrLoopBandKa;			// キャリア受信ループバンド Ka帯
							stpflgSat.UpLinkSweepShape |= stpflg.UpLinkSweepShape;		// アップリンク掃引パラメータ 形状
							stpflgSat.UpLinkSweepWidth |= stpflg.UpLinkSweepWidth;		// アップリンク掃引パラメータ 幅
							stpflgSat.UpLinkSweepTime |= stpflg.UpLinkSweepTime;		// アップリンク掃引パラメータ 時間
							stpflgSat.RngMesMode |= stpflg.RngMesMode;					// レンジ計測パラメータ モード
							stpflgSat.Modulation |= stpflg.Modulation;					// 変調方式 S帯
							stpflgSat.ModulationS |= stpflg.ModulationS;				// 変調方式 S帯
							stpflgSat.ModulationX |= stpflg.ModulationX;				// 変調方式 X帯
							stpflgSat.CmdMod |= stpflg.CmdMod;							// コマンド変調度
							stpflgSat.CmdSubCarrFreq |= stpflg.CmdSubCarrFreq;			// コマンドサブキャリア周波数
							stpflgSat.TransDoppler |= stpflg.TransDoppler;				// 送信ドップラ
							stpflgSat.AntAutoTrack |= stpflg.AntAutoTrack;				// アンテナ自動追尾
							stpflgSat.AntAosPosn |= stpflg.AntAosPosn;					// アンテナ待ち受け指向
							stpflgSat.AntWaitPoint |= stpflg.AntWaitPoint;				// アンテナ待機位置指向
							break;
						}
					}
					break;

				case 4:		// パラメータ値
					{
						switch (prmIdx)
						{
						case 1:	// S帯テレメトリビットレート
							if (stpflg.TlmBitRateS == 0)
							{
								item = mEmptyString;
							}
							stcid.TlmBitRateS = item;
							AddDataVector(stele.TlmBitRateS, item);
							break;
						case 2:	// X帯テレメトリビットレート
							if (stpflg.TlmBitRateX == 0)
							{
								item = mEmptyString;
							}
							stcid.TlmBitRateX = item;
							AddDataVector(stele.TlmBitRateX, item);
							break;
						case 3:	// X高速テレメトリビットレート
							if (stpflg.TlmBitRateHX == 0)
							{
								item = mEmptyString;
							}
							stcid.TlmBitRateHX = item;
							AddDataVector(stele.TlmBitRateHX, item);
							break;
						case 4:	// Ka帯テレメトリビットレート 
							if (stpflg.TlmBitRateKa == 0)
							{
								item = mEmptyString;
							}
							stcid.TlmBitRateKa = item;
							AddDataVector(stele.TlmBitRateKa, item);
							break;
						case 5:	// S帯コマンドビットレート
							if (stpflg.CmdBitRateS == 0)
							{
								item = mEmptyString;
							}
							stcid.CmdBitRateS = item;
							AddDataVector(stele.CmdBitRateS, item);
							break;
						case 6:	// X帯コマンドビットレート
							if (stpflg.CmdBitRateX == 0)
							{
								item = mEmptyString;
							}
							stcid.CmdBitRateX = item;
							AddDataVector(stele.CmdBitRateX, item);
							break;
						case 7:	// 送信電力
							if (stpflg.TrsPower == 0)
							{
								item = mEmptyString;
							}
							stcid.TrsPower = item;
							AddDataVector(stele.TrsPower, item);
							break;
						case 8:	// アップリンク掃引パラメータ
							if (stpflg.UpLinkSweepShape == 0)
							{
								item = mEmptyString;
							}
							stcid.UpLinkSweepShape = item;
							AddDataVector(stele.UpLinkSweepShape, item);
							break;
						case 9:	// レンジ計測パラメータ モード
							if (stpflg.RngMesMode == 0)
							{
								item = mEmptyString;
							}
							stcid.RngMesMode = item;
							AddDataVector(stele.RngMesMode, item);
							break;
						case 10:// S帯キャリア受信ループバンド
							if (stpflg.CarrLoopBandS == 0)
							{
								item = mEmptyString;
							}
							stcid.CarrLoopBandS = item;
							AddDataVector(stele.CarrLoopBandS, item);
							break;
						case 11:// X帯キャリア受信ループバンド
							if (stpflg.CarrLoopBandX == 0)
							{
								item = mEmptyString;
							}
							stcid.CarrLoopBandX = item;
							AddDataVector(stele.CarrLoopBandX, item);
							break;
						case 12:// Ka帯キャリア受信ループバンド
							if (stpflg.CarrLoopBandKa == 0)
							{
								item = mEmptyString;
							}
							stcid.CarrLoopBandKa = item;
							AddDataVector(stele.CarrLoopBandKa, item);
							break;
						case 13:// コマンド変調度
							if (stpflg.CmdMod == 0)
							{
								item = mEmptyString;
							}
							stcid.CmdMod = item;
							AddDataVector(stele.CmdMod, item);
							break;
						case 14:// コマンドサブキャリア周波数
							if (stpflg.CmdSubCarrFreq == 0)
							{
								item = mEmptyString;
							}
							stcid.CmdSubCarrFreq = item;
							AddDataVector(stele.CmdSubCarrFreq, item);
							break;
						case 15:// 送信ドップラ補償の有無
							if (stpflg.TransDoppler == 0)
							{
								item = mEmptyString;
							}
							stcid.TransDoppler = item;
							AddDataVector(stele.TransDoppler, item);
							break;
						case 16:// アンテナ自動追尾
							if (stpflg.AntAutoTrack == 0)
							{
								item = mEmptyString;
							}
							stcid.AntAutoTrack = item;
							AddDataVector(stele.AntAutoTrack, item);
							break;
						case 17:// 変調方式
							if (stpflg.Modulation == 0)
							{
								item = mEmptyString;
							}
							stcid.Modulation = item;
							AddDataVector(stele.Modulation, item);
							break;
						case 18:// アンテナ待ち受け指向の有無
							if (stpflg.AntAosPosn == 0)
							{
								item = mEmptyString;
							}
							stcid.AntAosPosn = item;
							AddDataVector(stele.AntAosPosn, item);
							break;
						case 19:// アンテナ待機位置指向の有無
							if (stpflg.AntWaitPoint == 0)
							{
								item = mEmptyString;
							}
							stcid.AntWaitPoint = item;
							AddDataVector(stele.AntWaitPoint, item);

							cidlist[sat.MakeUpper()][stcid.CID] = stcid;

							memset(&stpflg, 0, sizeof(stpflg));		// CIDが変わるたびにクリア
							break;
						}
					}
					break;
				}

				iStart = iFind + 1;
				iCount++;

			}
		}

		// 衛星別のパラメータ活性非活性登録
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
		// CIDでパターン別に活性非活性が異なるかチェック
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
					case 0:		// CID番号
						stcid.CID = item;
						AddDataVector(stele.CID, item);
						break;
		
					case 1:		// パラメータ番号
						prmIdx = _wtoi(item);
						break;
		
					case 2:		// パラメータ名
						break;
		
					case 3:		// パラメータ有効フラグ
					{
						int nflg = _wtoi(item);
						switch (prmIdx)
						{
						case 1:	// S帯テレメトリビットレート
							stpflg.TlmBitRateS = nflg;
							break;
						case 2:	// X帯テレメトリビットレート
							stpflg.TlmBitRateX = nflg;
							break;
						case 3:	// X高速テレメトリビットレート
							stpflg.TlmBitRateHX = nflg;
							break;
						case 4:	// Ka帯テレメトリビットレート 
							stpflg.TlmBitRateKa = nflg;
							break;
						case 5:	// S帯コマンドビットレート
							stpflg.CmdBitRateS = nflg;
							break;
						case 6:	// X帯コマンドビットレート
							stpflg.CmdBitRateX = nflg;
							break;
						case 7:	// 送信電力
							stpflg.TrsPower = nflg;
							break;
						case 8:	// アップリンク掃引パラメータ
							stpflg.UpLinkSweepShape = nflg;
							break;
						case 9:	// レンジ計測パラメータ モード
							stpflg.RngMesMode = nflg;
							break;
						case 10:// S帯キャリア受信ループバンド
							stpflg.CarrLoopBandS = nflg;
							break;
						case 11:// X帯キャリア受信ループバンド
							stpflg.CarrLoopBandX = nflg;
							break;
						case 12:// Ka帯キャリア受信ループバンド
							stpflg.CarrLoopBandKa = nflg;
							break;
						case 13:// コマンド変調度
							stpflg.CmdMod = nflg;
							break;
						case 14:// コマンドサブキャリア周波数
							stpflg.CmdSubCarrFreq = nflg;
							break;
						case 15:// 送信ドップラ補償の有無
							stpflg.TransDoppler = nflg;
							break;
						case 16:// アンテナ自動追尾
							stpflg.AntAutoTrack = nflg;
							break;
						case 17:// 変調方式
							stpflg.Modulation = nflg;
							break;
						case 18:// アンテナ待ち受け指向の有無
							stpflg.AntAosPosn = nflg;
							break;
						case 19:// アンテナ待機位置指向の有無
							stpflg.AntWaitPoint = nflg;
		
							//							cidPrmFlglist[sat.MakeUpper()] = stpflg;
							if (nFreadCnt == 0)
							{
								memcpy(&stpflgSat, &stpflg, sizeof(stpflgSat));
							}
							else
							{
								// 最終パターンの衛星別のパラメータ活性非活性情報と現在パターンを比較
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
		
					case 4:		// パラメータ値
					{
						switch (prmIdx)
						{
						case 1:	// S帯テレメトリビットレート
							if (stpflg.TlmBitRateS == 0)
							{
								item = mEmptyString;
							}
							stcid.TlmBitRateS = item;
							AddDataVector(stele.TlmBitRateS, item);
							break;
						case 2:	// X帯テレメトリビットレート
							if (stpflg.TlmBitRateX == 0)
							{
								item = mEmptyString;
							}
							stcid.TlmBitRateX = item;
							AddDataVector(stele.TlmBitRateX, item);
							break;
						case 3:	// X高速テレメトリビットレート
							if (stpflg.TlmBitRateHX == 0)
							{
								item = mEmptyString;
							}
							stcid.TlmBitRateHX = item;
							AddDataVector(stele.TlmBitRateHX, item);
							break;
						case 4:	// Ka帯テレメトリビットレート 
							if (stpflg.TlmBitRateKa == 0)
							{
								item = mEmptyString;
							}
							stcid.TlmBitRateKa = item;
							AddDataVector(stele.TlmBitRateKa, item);
							break;
						case 5:	// S帯コマンドビットレート
							if (stpflg.CmdBitRateS == 0)
							{
								item = mEmptyString;
							}
							stcid.CmdBitRateS = item;
							AddDataVector(stele.CmdBitRateS, item);
							break;
						case 6:	// X帯コマンドビットレート
							if (stpflg.CmdBitRateX == 0)
							{
								item = mEmptyString;
							}
							stcid.CmdBitRateX = item;
							AddDataVector(stele.CmdBitRateX, item);
							break;
						case 7:	// 送信電力
							if (stpflg.TrsPower == 0)
							{
								item = mEmptyString;
							}
							stcid.TrsPower = item;
							AddDataVector(stele.TrsPower, item);
							break;
						case 8:	// アップリンク掃引パラメータ
							if (stpflg.UpLinkSweepShape == 0)
							{
								item = mEmptyString;
							}
							stcid.UpLinkSweepShape = item;
							AddDataVector(stele.UpLinkSweepShape, item);
							break;
						case 9:	// レンジ計測パラメータ モード
							if (stpflg.RngMesMode == 0)
							{
								item = mEmptyString;
							}
							stcid.RngMesMode = item;
							AddDataVector(stele.RngMesMode, item);
							break;
						case 10:// S帯キャリア受信ループバンド
							if (stpflg.CarrLoopBandS == 0)
							{
								item = mEmptyString;
							}
							stcid.CarrLoopBandS = item;
							AddDataVector(stele.CarrLoopBandS, item);
							break;
						case 11:// X帯キャリア受信ループバンド
							if (stpflg.CarrLoopBandX == 0)
							{
								item = mEmptyString;
							}
							stcid.CarrLoopBandX = item;
							AddDataVector(stele.CarrLoopBandX, item);
							break;
						case 12:// Ka帯キャリア受信ループバンド
							if (stpflg.CarrLoopBandKa == 0)
							{
								item = mEmptyString;
							}
							stcid.CarrLoopBandKa = item;
							AddDataVector(stele.CarrLoopBandKa, item);
							break;
						case 13:// コマンド変調度
							if (stpflg.CmdMod == 0)
							{
								item = mEmptyString;
							}
							stcid.CmdMod = item;
							AddDataVector(stele.CmdMod, item);
							break;
						case 14:// コマンドサブキャリア周波数
							if (stpflg.CmdSubCarrFreq == 0)
							{
								item = mEmptyString;
							}
							stcid.CmdSubCarrFreq = item;
							AddDataVector(stele.CmdSubCarrFreq, item);
							break;
						case 15:// 送信ドップラ補償の有無
							if (stpflg.TransDoppler == 0)
							{
								item = mEmptyString;
							}
							stcid.TransDoppler = item;
							AddDataVector(stele.TransDoppler, item);
							break;
						case 16:// アンテナ自動追尾
							if (stpflg.AntAutoTrack == 0)
							{
								item = mEmptyString;
							}
							stcid.AntAutoTrack = item;
							AddDataVector(stele.AntAutoTrack, item);
							break;
						case 17:// 変調方式
							if (stpflg.Modulation == 0)
							{
								item = mEmptyString;
							}
							stcid.Modulation = item;
							AddDataVector(stele.Modulation, item);
							break;
						case 18:// アンテナ待ち受け指向の有無
							if (stpflg.AntAosPosn == 0)
							{
								item = mEmptyString;
							}
							stcid.AntAosPosn = item;
							AddDataVector(stele.AntAosPosn, item);
							break;
						case 19:// アンテナ待機位置指向の有無
							if (stpflg.AntWaitPoint == 0)
							{
								item = mEmptyString;
							}
							stcid.AntWaitPoint = item;
							AddDataVector(stele.AntWaitPoint, item);
		
							cidlist[sat.MakeUpper()][stcid.CID] = stcid;
							memset(&stpflg, 0, sizeof(stpflg));		// CIDが変わるたびにクリア
							break;
						}
					}
					break;
					}
		
					iStart = iFind + 1;
					iCount++;
		
				}
			}
		
			// 衛星別のパラメータ活性非活性登録
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

-# vector<CString> に値を追加する。但しユニーク。

@param  v		vector<CString>
@param  str		文字列
@retval なし
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

-# サーバーの各種ファイルを取得する

@param	なし
@retval	1：成功 / -1：失敗

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
		// 予報値一覧ファイルよりデータ取得
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
		// 計画一覧ファイルよりデータ取得(UTF8対応)
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
			psatellite = (CWnd*)theApp.GetWindowHandle(IDD_DLG_SATELLITECAPTURE54);	//衛星捕捉制御画面
		}
		else
		{
			psatellite = (CWnd*)theApp.GetWindowHandle(IDD_DLG_SATELLITECAPTURE);	//衛星捕捉制御画面
		}
		if (psatellite != NULL)
		{
			psatellite->PostMessage(eMessage_UpdateShareFile, 0, val);
		}
	}
}

/*============================================================================*/
/*! CSatelliteData

-# 数値にカンマを追加

@param	pos：カンマを付けたい数値
@param	pointKeta：少数点以下の桁数 (少数点0～6)
@param	singFlag：符号フラグ（0以外で値が正のとき'+'を付加
@retval	カンマを付けた後の数値

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
		// NaN (非数)の場合は０にする
		tempPos = 0.0;
	}

	// 0に近い場合はまるめる
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

	// 正負を調べる
	if (tempPos < 0)
	{
		sign = 1;
		num = -num;
	}

	// 繰上げを足す
	num = num + kuriage;

	// 整数部にカンマを付ける
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
		// 負のとき
		if (i + 1 >= MAXDIGIT14)
		{
			return CString(_T("###,###,###,###,###.######"));
		}
		str[i++] = '-';
	}
	else if (pos > 0.0)
	{
		// 正のとき
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
//-# スペクトラムデータ初期化処理
//
//@param  nStationID：衛星ID
//@retval 0：正常終了 / -1：異常終了
//*/
///*============================================================================*/
//void CSatelliteData::SpectrumInitialize()
//{
//	if (theApp.GetSelectStationMode() == eStation_1)
//	{
//		// 1局画面
//		UINT nStationID = theApp.GetSelectStation();
//		// 初期化
//		SpectrumInitialize(nStationID);
//		// ファイル読み込み
//		if (SpectrumFileRead(nStationID) < 0)
//		{
//			// ファイルが無かった、またはフォーマット違いの場合、ファイルを作り直す
//			SpectrumFileWrite(nStationID);
//			SpectrumFileRead(nStationID);
//		}
//	}
//	else
//	{
//		// 4局画面
//		for (int i = 0; i < eStation_MAX; i++)
//		{
//			// 初期化
//			SpectrumInitialize(i);
//			// ファイル読み込み
//			if (SpectrumFileRead(i) < 0)
//			{
//				// ファイルが無かった、またはフォーマット違いの場合、ファイルを作り直す
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
//-# スペクトラムデータ初期化処理
//
//@param  nStationID：衛星ID
//@retval 0：正常終了 / -1：異常終了
//*/
///*============================================================================*/
//void CSatelliteData::SpectrumInitialize(UINT nStationID)
//{
//	// ミニスペクトラム初期化
//	for (int i = 0; i < MAX_SUMMARY_VIEW; i++)
//	{
//		GetSummarySpectrumViewData(nStationID, i).InitializeData(eSpectrumGraphType_Sammary);
//	}
//
//	// スペクトラム初期化
//	GetSpectrumViewData().InitializeData(eSpectrumGraphType_Normal);
//}
//
//
///*============================================================================*/
///*! CSatelliteData
//
//-# スペクトラムデータファイル読み込み処理
//
//@param  nStationID：衛星ID
//@retval 0：正常終了 / -1：異常終了
//*/
///*============================================================================*/
//int CSatelliteData::SpectrumFileRead(UINT nStationID)
//{
//	FILE*	fp;
//	int ret;
//
//	/*------------------*/
//	/* ファイル名取得   */
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
//	// ミニスペクトラム
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
//	// スペクトラム
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
//-# スペクトラムデータファイル書き込み処理
//
//@param  nStationID：衛星ID
//@retval 0：正常終了 / -1：異常終了
//*/
///*============================================================================*/
//int CSatelliteData::SpectrumFileWrite(UINT nStationID)
//{
//	FILE*	fp;
//	int ret = ERR_RETURN;
//
//	/*------------------*/
//	/* ファイル名取得   */
//	/*------------------*/
//	CString strAppPath = theApp.GetAppPath();
//	CString strFilePath;
//	strFilePath.Format(_T("%s%s_%s.%s"), strAppPath, CString(SPECTRUM_FILE), CString(mStationSimpleString[nStationID]), CString(SPECTRUM_FILE_TYPE));
//
//	if (_wfopen_s(&fp, strFilePath, _T("w")) != 0) {
//		return(ERR_RETURN);
//	}
//
//	// ミニスペクトラム
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
//	// スペクトラム
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
//-# トレンドプロパティデータ初期化処理
//
//@param  nStationID：衛星ID
//@retval 0：正常終了 / -1：異常終了
//*/
///*============================================================================*/
//void CSatelliteData::TrendPropInitialize()
//{
//	if (theApp.GetSelectStationMode() == eStation_1)
//	{
//		// 1局画面
//		UINT nStationID = theApp.GetSelectStation();
//		// ファイル読み込み
//		(void)TrendPropReadData(nStationID);
////		if (!TrendPropReadData(nStationID))
////		{
////			// ファイルが無かった、またはフォーマット違いの場合、ファイルを作り直す
////			TrendPropWriteData(nStationID);
////		}
//	}
//	else
//	{
//		// 4局画面
//		for (int i = 0; i < eStation_MAX; i++)
//		{
//			// ファイル読み込み
//			(void)TrendPropReadData(i);
////			if (!TrendPropReadData(i))
////			{
////				// ファイルが無かった、またはフォーマット違いの場合、ファイルを作り直す
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
//-# トレンドプロパティデータ保存の開始処理
//
//@param	なし
//@return	bool		結果
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
//-#	トレンドプロパティデータ保存の終了処理
//@param	なし
//@return	bool		結果
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
//-# トレンドプロパティデータの書き込み
//
//@param	nStationID		局ID
//@return	bool			結果
//
//*/
///*============================================================================*/
//bool CSatelliteData::TrendPropWriteData(UINT nStationID)
//{
//	bool ret = true;
//	/*------------------*/
//	/* ファイル名取得   */
//	/*------------------*/
//	CString strAppPath = theApp.GetAppPath();
//	CString strFilePath;
//	strFilePath.Format(_T("%s%s_%s.%s"), strAppPath, CString(TRENDPROP_FILE), CString(mStationSimpleString[nStationID]), CString(TRENDPROP_FILE_TYPE));
//
//	vector<CPropertiesData> propertiesdataList;
//	// 衛星捕捉グラフ設定
//	for (int i = 0; i < eTrendWndType_Num; i++)
//	{
//		propertiesdataList.push_back(m_arrayTrendPropertiesData[i]);
//	}
//	// サマリグラフ設定
//	for (int i = 0; i < MAX_SUMMARY_VIEW; i++)
//	{
//		propertiesdataList.push_back(m_arraySummaryPropertiesData[nStationID][i]);
//	}
//
//	// 初期化
//	if (!m_trendPropMemMapList.WriteInitialization(strFilePath))
//		return false;
//
//	// 保存
//	if (!m_trendPropMemMapList.WriteData(strFilePath, propertiesdataList))
//		ret = false;
//
//	// 保存終了
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
//-#	トレンドプロパティデータ展開の開始処理
//
//@param	なし
//@return	bool		結果
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
//-#	トレンドプロパティデータ展開の終了処理
//@param
//@return	bool		結果
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
//-#	トレンドプロパティデータの読み込み
//
//@param	nStationID		局ID
//@return	bool			結果
//
//*/
///*============================================================================*/
//bool CSatelliteData::TrendPropReadData(UINT nStationID)
//{
//	bool ret = true;
//	/*------------------*/
//	/* ファイル名取得   */
//	/*------------------*/
//	CString strAppPath = theApp.GetAppPath();
//	CString strFilePath;
//	strFilePath.Format(_T("%s%s_%s.%s"), strAppPath, CString(TRENDPROP_FILE), CString(mStationSimpleString[nStationID]), CString(TRENDPROP_FILE_TYPE));
//
//	vector<CPropertiesData> propertiesdataList;
//	// 衛星捕捉グラフ設定
//	for (int i = 0; i < eTrendWndType_Num; i++)
//	{
//		propertiesdataList.push_back(m_arrayTrendPropertiesData[i]);
//	}
//	// サマリグラフ設定
//	for (int i = 0; i < MAX_SUMMARY_VIEW; i++)
//	{
//		propertiesdataList.push_back(m_arraySummaryPropertiesData[nStationID][i]);
//	}
//
//	// 初期化
//	if (!m_trendPropMemMapList.ReadInitialization(strFilePath))
//		return false;
//
//	// 保存
//	if (!m_trendPropMemMapList.ReadData(strFilePath, propertiesdataList))
//		ret = false;
//
//	// グラフ設定に反映
//	// 衛星捕捉グラフ設定
//	for (int i = 0; i < eTrendWndType_Num; i++)
//	{
//		m_arrayTrendPropertiesData[i] = propertiesdataList[i];
//	}
//	// サマリグラフ設定
//	for (int i = 0; i < MAX_SUMMARY_VIEW; i++)
//	{
//		m_arraySummaryPropertiesData[nStationID][i] = propertiesdataList[i + eTrendWndType_Num];
//	}
//
//	// 保存終了
//	if (!m_trendPropMemMapList.ReadFinalization())
//		ret = false;
//
//	return ret;
//
//}
//#endif

/*============================================================================*/
/*! データ管理

-# レイアウト読み込み（設定読み込み）

@param  なし
@retval	なし

*/
/*============================================================================*/
bool CSatelliteData::Load(CArchive& ar)
{
	try
	{
		// トレンドデータ
		for (int item = 0; item < eTrendWndType_Num; item++)
		{
			ar >> m_arrayTrendPropertiesData[item].wndIdx;						// ウィンドー番号
			ar >> m_arrayTrendPropertiesData[item].wndName;					// ウィンドー名称
			ar >> m_arrayTrendPropertiesData[item].wnd;						// CView
			ar >> m_arrayTrendPropertiesData[item].nGraphIdx;					// グラフ番号
			ar >> m_arrayTrendPropertiesData[item].isRealTime;					// リアルタイム（true）/ ノンリアルタイム（false）
			ar >> m_arrayTrendPropertiesData[item].startTime;					// 開始日時
			//stVScale	trandVScale;				// 縦軸スケール[0:左、1:右]
			ar >> m_arrayTrendPropertiesData[item].trandVScale.Auto;
			ar >> m_arrayTrendPropertiesData[item].trandVScale.VScalLMax;
			ar >> m_arrayTrendPropertiesData[item].trandVScale.VScalLMin;
			ar >> m_arrayTrendPropertiesData[item].trandVScale.VScalRMax;
			ar >> m_arrayTrendPropertiesData[item].trandVScale.VScalRMin;
			ar >> m_arrayTrendPropertiesData[item].trandVScale.VSCalItvl;
			//HScale		trandHScale[MAX_HSCALE];	// 横軸スケール[0:トレンドグラフビュー1、1:トレンドグラフビュー2]
			for (int i = 0; i < MAX_HSCALE; i++){
				int val;
				ar >> val;
				m_arrayTrendPropertiesData[item].trandHScale[i] = (HScale)val;
			}
			//CString		dataName[MAX_TDATA];		// データ名称
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].dataName[i];
			}
			//CString		dataGName[MAX_TDATA];		// データ名称(グラフ用名)
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].dataGName[i];
			}
			//CString		dataUnit[MAX_TDATA];		// 単位
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].dataUnit[i];
			}
			//int			nDecDidits[MAX_TDATA];		// 少数点以下の桁数
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].nDecDidits[i];
			}
			//bool		isViewMode[MAX_TDATA];		// 表示On/Off
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].isViewMode[i];
			}
			//DrawMode	drawMode[MAX_TDATA];		// 描画モード（Line、Dots、L&D）
			for (int i = 0; i < MAX_TDATA; i++){
				int val;
				ar >> val;
				m_arrayTrendPropertiesData[item].drawMode[i] = (DrawMode)val;
			}
			//COLORREF	color[MAX_TDATA];			// 表示色
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].color[i];
			}
			//int			lineWidth[MAX_TDATA];		// 線幅
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].lineWidth[i];
			}
			//DotSize		dotSize[MAX_TDATA];			// ドットサイズ
			for (int i = 0; i < MAX_TDATA; i++){
				int val;
				ar >> val;
				m_arrayTrendPropertiesData[item].dotSize[i] = (DotSize)val;
			}
			//VScaleKind	vscaleKind[MAX_TDATA];		// 縦軸スケール選択（右/左）
			for (int i = 0; i < MAX_TDATA; i++){
				int val;
				ar >> val;
				m_arrayTrendPropertiesData[item].vscaleKind[i] = (VScaleKind)val;
			}
			//CString		comment[MAX_TDATA];			// コメント
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].comment[i];
			}
			//bool		isComment[MAX_TDATA];		// コメント表示
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].isComment[i];
			}
			//bool		isAlarm[MAX_TDATA];			// アラーム表示
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].isAlarm[i];
			}
			//double		alarm1Major[MAX_TDATA];		// アラーム1 上限
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].alarm1Major[i];
			}
			//double		alarm1Minor[MAX_TDATA];		// アラーム1 下限 
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].alarm1Minor[i];
			}
			//double		alarm2Major[MAX_TDATA];		// アラーム2 上限
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].alarm2Major[i];
			}
			//double		alarm2Minor[MAX_TDATA];		// アラーム2 下限
			for (int i = 0; i < MAX_TDATA; i++){
				ar >> m_arrayTrendPropertiesData[item].alarm2Minor[i];
			}
			ar >> m_arrayTrendPropertiesData[item].m_bSetted;
		}
		// スペクトラムデータ
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
		// サマリデータ
		for (int item = 0; item < eStation_MAX; item++){
			for (int sitem = 0; sitem < MAX_SUMMARY_VIEW; sitem++){
				ar >> m_arraySummaryPropertiesData[item][sitem].wndIdx;						// ウィンドー番号
				ar >> m_arraySummaryPropertiesData[item][sitem].wndName;					// ウィンドー名称
				ar >> m_arraySummaryPropertiesData[item][sitem].wnd;						// CView
				ar >> m_arraySummaryPropertiesData[item][sitem].nGraphIdx;					// グラフ番号
				ar >> m_arraySummaryPropertiesData[item][sitem].isRealTime;					// リアルタイム（true）/ ノンリアルタイム（false）
				ar >> m_arraySummaryPropertiesData[item][sitem].startTime;					// 開始日時
				//stVScale	trandVScale;				// 縦軸スケール[0:左、1:右]
				ar >> m_arraySummaryPropertiesData[item][sitem].trandVScale.Auto;
				ar >> m_arraySummaryPropertiesData[item][sitem].trandVScale.VScalLMax;
				ar >> m_arraySummaryPropertiesData[item][sitem].trandVScale.VScalLMin;
				ar >> m_arraySummaryPropertiesData[item][sitem].trandVScale.VScalRMax;
				ar >> m_arraySummaryPropertiesData[item][sitem].trandVScale.VScalRMin;
				ar >> m_arraySummaryPropertiesData[item][sitem].trandVScale.VSCalItvl;
				//HScale		trandHScale[MAX_HSCALE];	// 横軸スケール[0:トレンドグラフビュー1、1:トレンドグラフビュー2]
				for (int i = 0; i < MAX_HSCALE; i++){
					int val;
					ar >> val;
					m_arraySummaryPropertiesData[item][sitem].trandHScale[i] = (HScale)val;
				}
				//CString		dataName[MAX_TDATA];		// データ名称
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].dataName[i];
				}
				//CString		dataGName[MAX_TDATA];		// データ名称(グラフ用名)
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].dataGName[i];
				}
				//CString		dataUnit[MAX_TDATA];		// 単位
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].dataUnit[i];
				}
				//int			nDecDidits[MAX_TDATA];		// 少数点以下の桁数
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].nDecDidits[i];
				}
				//bool		isViewMode[MAX_TDATA];		// 表示On/Off
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].isViewMode[i];
				}
				//DrawMode	drawMode[MAX_TDATA];		// 描画モード（Line、Dots、L&D）
				for (int i = 0; i < MAX_TDATA; i++){
					int val;
					ar >> val;
					m_arraySummaryPropertiesData[item][sitem].drawMode[i] = (DrawMode)val;
				}
				//COLORREF	color[MAX_TDATA];			// 表示色
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].color[i];
				}
				//int			lineWidth[MAX_TDATA];		// 線幅
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].lineWidth[i];
				}
				//DotSize		dotSize[MAX_TDATA];			// ドットサイズ
				for (int i = 0; i < MAX_TDATA; i++){
					int val;
					ar >> val;
					m_arraySummaryPropertiesData[item][sitem].dotSize[i] = (DotSize)val;
				}
				//VScaleKind	vscaleKind[MAX_TDATA];		// 縦軸スケール選択（右/左）
				for (int i = 0; i < MAX_TDATA; i++){
					int val;
					ar >> val;
					m_arraySummaryPropertiesData[item][sitem].vscaleKind[i] = (VScaleKind)val;
				}
				//CString		comment[MAX_TDATA];			// コメント
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].comment[i];
				}
				//bool		isComment[MAX_TDATA];		// コメント表示
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].isComment[i];
				}
				//bool		isAlarm[MAX_TDATA];			// アラーム表示
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].isAlarm[i];
				}
				//double		alarm1Major[MAX_TDATA];		// アラーム1 上限
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].alarm1Major[i];
				}
				//double		alarm1Minor[MAX_TDATA];		// アラーム1 下限 
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].alarm1Minor[i];
				}
				//double		alarm2Major[MAX_TDATA];		// アラーム2 上限
				for (int i = 0; i < MAX_TDATA; i++){
					ar >> m_arraySummaryPropertiesData[item][sitem].alarm2Major[i];
				}
				//double		alarm2Minor[MAX_TDATA];		// アラーム2 下限
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
/*! データ管理

-# レイアウト保存（設定保存）

@param  なし
@retval	なし

*/
/*============================================================================*/
bool CSatelliteData::Save(CArchive& ar)
{
	try
	{
		// トレンドデータ
		for (int item = 0; item < eTrendWndType_Num; item++)
		{
			ar << m_arrayTrendPropertiesData[item].wndIdx;						// ウィンドー番号
			ar << m_arrayTrendPropertiesData[item].wndName;					// ウィンドー名称
			ar << m_arrayTrendPropertiesData[item].wnd;						// CView
			ar << m_arrayTrendPropertiesData[item].nGraphIdx;					// グラフ番号
			ar << m_arrayTrendPropertiesData[item].isRealTime;					// リアルタイム（true）/ ノンリアルタイム（false）
			ar << m_arrayTrendPropertiesData[item].startTime;					// 開始日時
			//stVScale	trandVScale;				// 縦軸スケール[0:左、1:右]
			ar << m_arrayTrendPropertiesData[item].trandVScale.Auto;
			ar << m_arrayTrendPropertiesData[item].trandVScale.VScalLMax;
			ar << m_arrayTrendPropertiesData[item].trandVScale.VScalLMin;
			ar << m_arrayTrendPropertiesData[item].trandVScale.VScalRMax;
			ar << m_arrayTrendPropertiesData[item].trandVScale.VScalRMin;
			ar << m_arrayTrendPropertiesData[item].trandVScale.VSCalItvl;
			//HScale		trandHScale[MAX_HSCALE];	// 横軸スケール[0:トレンドグラフビュー1、1:トレンドグラフビュー2]
			for (int i = 0; i < MAX_HSCALE; i++){
				ar << (int)m_arrayTrendPropertiesData[item].trandHScale[i];
			}
			//CString		dataName[MAX_TDATA];		// データ名称
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].dataName[i];
			}
			//CString		dataGName[MAX_TDATA];		// データ名称(グラフ用名)
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].dataGName[i];
			}
			//CString		dataUnit[MAX_TDATA];		// 単位
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].dataUnit[i];
			}
			//int			nDecDidits[MAX_TDATA];		// 少数点以下の桁数
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].nDecDidits[i];
			}
			//bool		isViewMode[MAX_TDATA];		// 表示On/Off
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].isViewMode[i];
			}
			//DrawMode	drawMode[MAX_TDATA];		// 描画モード（Line、Dots、L&D）
			for (int i = 0; i < MAX_TDATA; i++){
				ar << (int)m_arrayTrendPropertiesData[item].drawMode[i];
			}
			//COLORREF	color[MAX_TDATA];			// 表示色
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].color[i];
			}
			//int			lineWidth[MAX_TDATA];		// 線幅
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].lineWidth[i];
			}
			//DotSize		dotSize[MAX_TDATA];			// ドットサイズ
			for (int i = 0; i < MAX_TDATA; i++){
				ar << (int)m_arrayTrendPropertiesData[item].dotSize[i];
			}
			//VScaleKind	vscaleKind[MAX_TDATA];		// 縦軸スケール選択（右/左）
			for (int i = 0; i < MAX_TDATA; i++){
				ar << (int)m_arrayTrendPropertiesData[item].vscaleKind[i];
			}
			//CString		comment[MAX_TDATA];			// コメント
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].comment[i];
			}
			//bool		isComment[MAX_TDATA];		// コメント表示
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].isComment[i];
			}
			//bool		isAlarm[MAX_TDATA];			// アラーム表示
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].isAlarm[i];
			}
			//double		alarm1Major[MAX_TDATA];		// アラーム1 上限
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].alarm1Major[i];
			}
			//double		alarm1Minor[MAX_TDATA];		// アラーム1 下限 
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].alarm1Minor[i];
			}
			//double		alarm2Major[MAX_TDATA];		// アラーム2 上限
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].alarm2Major[i];
			}
			//double		alarm2Minor[MAX_TDATA];		// アラーム2 下限
			for (int i = 0; i < MAX_TDATA; i++){
				ar << m_arrayTrendPropertiesData[item].alarm2Minor[i];
			}
			ar << m_arrayTrendPropertiesData[item].m_bSetted;
		}
		// スペクトラムデータ
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
		// サマリデータ
		for (int item = 0; item < eStation_MAX; item++){
			for (int sitem = 0; sitem < MAX_SUMMARY_VIEW; sitem++){
				ar << m_arraySummaryPropertiesData[item][sitem].wndIdx;						// ウィンドー番号
				ar << m_arraySummaryPropertiesData[item][sitem].wndName;					// ウィンドー名称
				ar << m_arraySummaryPropertiesData[item][sitem].wnd;						// CView
				ar << m_arraySummaryPropertiesData[item][sitem].nGraphIdx;					// グラフ番号
				ar << m_arraySummaryPropertiesData[item][sitem].isRealTime;					// リアルタイム（true）/ ノンリアルタイム（false）
				ar << m_arraySummaryPropertiesData[item][sitem].startTime;					// 開始日時
				//stVScale	trandVScale;				// 縦軸スケール[0:左、1:右]
				ar << m_arraySummaryPropertiesData[item][sitem].trandVScale.Auto;
				ar << m_arraySummaryPropertiesData[item][sitem].trandVScale.VScalLMax;
				ar << m_arraySummaryPropertiesData[item][sitem].trandVScale.VScalLMin;
				ar << m_arraySummaryPropertiesData[item][sitem].trandVScale.VScalRMax;
				ar << m_arraySummaryPropertiesData[item][sitem].trandVScale.VScalRMin;
				ar << m_arraySummaryPropertiesData[item][sitem].trandVScale.VSCalItvl;
				//HScale		trandHScale[MAX_HSCALE];	// 横軸スケール[0:トレンドグラフビュー1、1:トレンドグラフビュー2]
				for (int i = 0; i < MAX_HSCALE; i++){
					ar << (int)m_arraySummaryPropertiesData[item][sitem].trandHScale[i];
				}
				//CString		dataName[MAX_TDATA];		// データ名称
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].dataName[i];
				}
				//CString		dataGName[MAX_TDATA];		// データ名称(グラフ用名)
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].dataGName[i];
				}
				//CString		dataUnit[MAX_TDATA];		// 単位
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].dataUnit[i];
				}
				//int			nDecDidits[MAX_TDATA];		// 少数点以下の桁数
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].nDecDidits[i];
				}
				//bool		isViewMode[MAX_TDATA];		// 表示On/Off
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].isViewMode[i];
				}
				//DrawMode	drawMode[MAX_TDATA];		// 描画モード（Line、Dots、L&D）
				for (int i = 0; i < MAX_TDATA; i++){
					ar << (int)m_arraySummaryPropertiesData[item][sitem].drawMode[i];
				}
				//COLORREF	color[MAX_TDATA];			// 表示色
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].color[i];
				}
				//int			lineWidth[MAX_TDATA];		// 線幅
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].lineWidth[i];
				}
				//DotSize		dotSize[MAX_TDATA];			// ドットサイズ
				for (int i = 0; i < MAX_TDATA; i++){
					ar << (int)m_arraySummaryPropertiesData[item][sitem].dotSize[i];
				}
				//VScaleKind	vscaleKind[MAX_TDATA];		// 縦軸スケール選択（右/左）
				for (int i = 0; i < MAX_TDATA; i++){
					ar << (int)m_arraySummaryPropertiesData[item][sitem].vscaleKind[i];
				}
				//CString		comment[MAX_TDATA];			// コメント
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].comment[i];
				}
				//bool		isComment[MAX_TDATA];		// コメント表示
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].isComment[i];
				}
				//bool		isAlarm[MAX_TDATA];			// アラーム表示
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].isAlarm[i];
				}
				//double		alarm1Major[MAX_TDATA];		// アラーム1 上限
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].alarm1Major[i];
				}
				//double		alarm1Minor[MAX_TDATA];		// アラーム1 下限 
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].alarm1Minor[i];
				}
				//double		alarm2Major[MAX_TDATA];		// アラーム2 上限
				for (int i = 0; i < MAX_TDATA; i++){
					ar << m_arraySummaryPropertiesData[item][sitem].alarm2Major[i];
				}
				//double		alarm2Minor[MAX_TDATA];		// アラーム2 下限
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

-# STAT_SDTPで保存されたローカルのセッション状態ファイルを読み込む

@param	なし
@retval	TRUE：読み込み成功 / FALSE：読み込み失敗

*/
/*============================================================================*/
BOOL CSatelliteData::ReadSessionStatusFile()
{
	// セッション状態ロックファイルがあるか調べる
	CString strFilePath;
	strFilePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppWorkPath(), STAT_SDTP_LOCKFILENAME);
	if (PathFileExists(strFilePath))
		return FALSE;

	FILE* fp;
	strFilePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppWorkPath(), STAT_SDTP_FILENAME);

	// 予報値ファイルが更新されているか確認
	HANDLE hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FILETIME fileTime;
	GetFileTime(hFile, NULL, NULL, &fileTime);
	CloseHandle(hFile);
	// ファイル時刻比較
	if (CompareFileTime(&m_SessionFileTime, &fileTime) == 0)
	{
		// 変更なしならば何もしない
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

			// 1局モード
			if (theApp.GetSelectStationMode() == eStation_1)
			{
				UINT nStation = theApp.GetSelectStation();
				// 局の一致を調べる
				if (strStation.CompareNoCase(CString(mStationSimpleString[nStation]).MakeLower()) == 0)
				{
					// 自局のセッション状態をセット
					m_sessionInfo[nStation].SetSessionStatus(strSession, strStatus);
				}
			}
			// 4局モード
			else
			{
				for (int i = 0; i < eStation_MAX; i++)
				{
					// 局の一致を調べる
					if (strStation.CompareNoCase(CString(mStationSimpleString[i]).MakeLower()) == 0)
					{
						// 一致局のセッション状態をセット
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

-# STAT_SDTPで保存されたローカルのセッション状態ファイルを削除する

@param	なし
@retval	なし

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

-# RFトリップ角定義ファイル読み込み

@param	なし
@retval	1：成功 / -1：失敗

*/
/*============================================================================*/
void CSatelliteData::ReadRFTripFile()
{
	// 1局モード
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		UINT nStation = theApp.GetSelectStation();
		m_arrayRFTripData[nStation].SetStation(nStation);
		(void)m_arrayRFTripData[nStation].ReadFile();
	}
	// 4局モード
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

-# プロパティファイル読込

@param	strSat : 衛星名
@param	stProp : プロパティデータ格納領域
@retval	1：成功 / -1：失敗

*/
/*============================================================================*/
int  CSatelliteData::ReadPropertyFile(CString strSat, stOpeProp &stProp)
{
	CString dir = theApp.GetShareFilePath(eFileType_OperationProp_Folder, theApp.GetSelectStation());
	CString fname = dir + _T("Property.dat");

	TCHAR buf[MAX_PATH];

	//--------------------------
	// 共通部
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
	// 衛星固有部
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

-# 計画帳票出力用テンポラリファイル作成

@param  fname :計画ファイル名（フルパス）※パス区切りは"\"を想定
@retval 帳票用テンポラリファイル名
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
	// ファイル存在チェック
	if (PathFileExists(fname) == FALSE)
	{
		return strRet;
	}

	//---------------------------------------------
	// 計画ファイル読込
	CString mes;
	if (_wfopen_s(&fpin, fname, _T("r")) != 0)
	{
		mes.Format(_T("計画ファイルを開くことができません。\n%s"), (LPCTSTR)fname);
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

			// ヘッダー部取得
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


					if (strEle.Find(_T("衛星名")) >= 0)
					{
						// 衛星名
						AfxExtractSubString(strSatellite, strEle, 1, '=');
					}
					else if (strEle.Find(_T("P-START")) >= 0)
					{
						// 運用開始時刻取得
						AfxExtractSubString(strOpeSDate, strEle, 1, '=');
					}
					else if (strEle.Find(_T("P-END")) >= 0)
					{
						// 運用終了時刻取得
						AfxExtractSubString(strOpeEDate, strEle, 1, '=');
					}
				}
			}

			// イベント名取得
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
						if (strLine.Find(_T("分前")) >= 0)
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

			// イベント取得
			CString tmp1, tmp2, tmp3;
			AfxExtractSubString(tmp1, strLine, 0, ' ');
			AfxExtractSubString(tmp2, strLine, 1, ' ');
			AfxExtractSubString(tmp3, strLine, 2, ' ');

			// 日時
			CString strDate = tmp1 + _T(" ") + tmp2;
			st.strDate = strDate;
			st.tDate = CSatelliteData::StrTimeToCTime(strDate);

			// イベント名
			st.strEventName = strEventName;

			// 制御
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
	// 帳票出力用テンポラリファイル作成
	char szBuf[2046];
	strDName = fname.Left(fname.ReverseFind(_T('\\')));
	strFName = fname.Right(fname.GetLength() - fname.ReverseFind(_T('\\')) - 1);
	strOutFName = strDName + _T("\\") + strFName.Mid(0, strFName.Find('.')) + _T(".") + PLNOUT_EXT;

	if ((_wfopen_s(&fpout, strOutFName, _T("w")) != 0) || (fpout == NULL))
	{
		mes.Format(_T("計画帳票用ファイルを開くことができません。\n%s"), (LPCTSTR)strOutFName);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return strRet;
	}

	// 表示情報出力
	sprintf_s(szBuf, "#%s\n", "HEADER");
	fputs(szBuf, fpout);
	sprintf_s(szBuf, "SATELLITE=%s\n", (LPCSTR)CStringA(strSatellite));
	fputs(szBuf, fpout);
	sprintf_s(szBuf, "P-START=%s\n", (LPCSTR)CStringA(strOpeSDate));
	fputs(szBuf, fpout);
	sprintf_s(szBuf, "P-END=%s\n", (LPCSTR)CStringA(strOpeEDate));
	fputs(szBuf, fpout);

	// サマリー情報出力
	sprintf_s(szBuf, "\n#%s\n", "SUMMARY");
	fputs(szBuf, fpout);
	for (int i = 0; i < headerList.size(); ++i)
	{
		sprintf_s(szBuf, "%s\n", (LPCSTR)CStringA(headerList[i]));
		fputs(szBuf, fpout);
	}

	// 詳細情報出力

	// ソート
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

-# 帳票出力

@param  fname :計画ファイル名（フルパス）※パス区切りは"\"を想定
@retval 帳票用テンポラリファイル名
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

-# 帳票出力プロセス

@param  hwnd：未使用
@param	param：帳票パラメータ
@param	bWaitComplete：TRUE:終了を待つ（デフォルト） / FALSE:終了を待たない
@retval 帳票実行結果(0：成功、その他：失敗）
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

	// 新しいプロセス用の構造体を起動する
	STARTUPINFO    stStartUpInfo;
	// メンバーの多くは予約されているか無視されるので、すべてのメンバーを
	// 0 に設定してから、必要なメンバーだけをリセットする
	memset(&stStartUpInfo, 0, sizeof(STARTUPINFO));
	stStartUpInfo.cb = sizeof(STARTUPINFO);			// 構造体のサイズls
	stStartUpInfo.dwFlags = STARTF_USESHOWWINDOW;	// wShowWindow メンバーを使う
	stStartUpInfo.wShowWindow = SW_HIDE;			// 画面は非表示

	// 新しいプロセス上で情報を受け取るプロセス構造体
	PROCESS_INFORMATION stProcessInfo;

	// 新しいプロセスを作ると、プロセスはプライマリスレッドを作り、
	// アプリケーションを起動する
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

	// 帳票終了を待つ場合
	if (bWaitComplete)
	{
		// 帳票終了まで待機
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
	// 帳票終了を待たない場合
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

-# 予報値から（アップリンク距離、ダウンリンク距離、２WAY距離）を取得する

@param	t		指定時刻	tye
@param	type	0=Uplink 1=Downlink 2;2way
@retval

*/
/*============================================================================*/
double CTrendData::GetUpDownLinkRange(time_t t, int type)
{
	char ca_TIME[26];			/* 時刻（フルフォーマット）		  */
	long m_gla_prd_cont = 0;
	m_UpDownLinkRangeMap.clear();
	if (m_UpDownLinkRangeMap.size() == 0){
		// mapの作成
		/* <<XUP>> 予報値カウンタクリア */
		m_gla_prd_cont = 0;
		for (m_gla_prd_cont = 0; m_gla_prd_cont<m_PredItemCount; m_gla_prd_cont++) {

			/* <<XUP>> 予報値テーブルのストッパーを検出したならば、終了 */
			if (strcmp(m_gst_predata.predata[m_gla_prd_cont].ca_predict_s_tx_freq, END_OF_TABLE) == 0) {
				break;
			}

			// 予報値データを衛星データのテンポラリに追加する
			stServerItemData data;
			memset(&data, 0, sizeof(stServerItemData));
			data.kind = 0;

			/* <<XUP>> 予報値時刻の型変換（BCD→ASCII） */
			memset(ca_TIME, (char)NULL, sizeof(ca_TIME));
			bcd7_to_asc19_time(m_gst_predata.predata[m_gla_prd_cont].uca_time, (unsigned char*)ca_TIME);
			CTime ctime = CTime((time_t)CSatelliteData::ub_asc_to_dtime(ca_TIME));

			SYSTEMTIME systime;
			ctime.GetAsSystemTime(systime);
			CCommBase::ConvertTime(&systime, &data.systime);

			// RNG伝搬モニタ用に、予報値時刻と往復距離のマップを用意する
			time_t tRngTime = ctime.GetTime();
			switch (type){
			case	0/*アップリンク距離*/:
				m_UpDownLinkRangeMap[tRngTime] = m_gst_predata.predata[m_gla_prd_cont].d_predict_uplink_range;
				break;
			case	1/*ダウンリンク距離*/:
				m_UpDownLinkRangeMap[tRngTime] = m_gst_predata.predata[m_gla_prd_cont].d_predict_downlink_range;
				break;
			case	2/*２Way距離*/:
				m_UpDownLinkRangeMap[tRngTime] = m_gst_predata.predata[m_gla_prd_cont].d_predict_2way_rng;
				break;
			}

			/* <<2013-TURBO>> 1秒更新24時間予報値に対応し、EOTがない場合の終了判定を追加 */
			if (m_gla_prd_cont == MAX_PRED_SIZE) {
				break;
			}
		}
	}

	auto itr = m_UpDownLinkRangeMap.lower_bound(t);
	// 範囲にある場合
	if (itr != m_UpDownLinkRangeMap.end())
	{
		if ((*itr).first == t)
		{
			// 予報値マップに時刻がある場合は、マップの値を返す
			return m_UpDownLinkRangeMap[t];
		}
		else if (itr == m_UpDownLinkRangeMap.begin())
		{
			// １番目より前のため、１番目と２番目の予報値で補間
			auto itr1 = itr;
			itr++;
			auto itr2 = itr;
			return GetLinearInterpolationRange((*itr1).first, (*itr1).second, (*itr2).first, (*itr2).second, t);
		}
		else
		{
			// ひとつ前の予報値と検索した予報値で補間
			auto itr2 = itr;
			itr--;
			auto itr1 = itr;
			return GetLinearInterpolationRange((*itr1).first, (*itr1).second, (*itr2).first, (*itr2).second, t);
		}
	}
	// 範囲に無い場合
	else if (m_UpDownLinkRangeMap.size() >= 2)
	{
		// 最後より１つ前の予報値と最後の予報値で補間
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

-# 衛星までの往復距離を直線補間してマップに追加する

@param	t1
@param	rng1
@param	t2
@param	rng2
@retval	なし

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

-# 予報値から（アップリンク距離変化率、ダウンリンク距離変化率、２WAY距離変化率）を取得する

@param	t		指定時刻	tye
@param	type	0=Uplink 1=Downlink 2;2way
@retval

*/
/*============================================================================*/
double CTrendData::GetUpDownLinkRate(time_t t, int type)
{
	char ca_TIME[26];			/* 時刻（フルフォーマット）		  */
	long m_gla_prd_cont = 0;
	m_UpDownLinkRateMap.clear();
	if (m_UpDownLinkRateMap.size() == 0){
		// mapの作成
		/* <<XUP>> 予報値カウンタクリア */
		m_gla_prd_cont = 0;
		for (m_gla_prd_cont = 0; m_gla_prd_cont<m_PredItemCount; m_gla_prd_cont++) {

			/* <<XUP>> 予報値テーブルのストッパーを検出したならば、終了 */
			if (strcmp(m_gst_predata.predata[m_gla_prd_cont].ca_predict_s_tx_freq, END_OF_TABLE) == 0) {
				break;
			}

			// 予報値データを衛星データのテンポラリに追加する
			stServerItemData data;
			memset(&data, 0, sizeof(stServerItemData));
			data.kind = 0;

			/* <<XUP>> 予報値時刻の型変換（BCD→ASCII） */
			memset(ca_TIME, (char)NULL, sizeof(ca_TIME));
			bcd7_to_asc19_time(m_gst_predata.predata[m_gla_prd_cont].uca_time, (unsigned char*)ca_TIME);
			CTime ctime = CTime((time_t)CSatelliteData::ub_asc_to_dtime(ca_TIME));

			SYSTEMTIME systime;
			ctime.GetAsSystemTime(systime);
			CCommBase::ConvertTime(&systime, &data.systime);

			// RNG伝搬モニタ用に、予報値時刻と往復距離のマップを用意する
			time_t tRngTime = ctime.GetTime();
			switch (type){
			case	0/*アップリンク距離*/:
				m_UpDownLinkRateMap[tRngTime] = m_gst_predata.predata[m_gla_prd_cont].d_predict_uplink_range_rate;
				break;
			case	1/*ダウンリンク距離*/:
				m_UpDownLinkRateMap[tRngTime] = m_gst_predata.predata[m_gla_prd_cont].d_predict_downlink_range_rate;
				break;
			case	2/*２Way距離*/:
				m_UpDownLinkRateMap[tRngTime] = m_gst_predata.predata[m_gla_prd_cont].d_predict_2way_range_rate;
				break;
			case	3/*３Way距離*/:
				m_UpDownLinkRateMap[tRngTime] = m_gst_predata.predata[m_gla_prd_cont].d_predict_3way_range_rate;
				break;
			}

			/* <<2013-TURBO>> 1秒更新24時間予報値に対応し、EOTがない場合の終了判定を追加 */
			if (m_gla_prd_cont == MAX_PRED_SIZE) {
				break;
			}
		}
	}

	auto itr = m_UpDownLinkRateMap.lower_bound(t);
	// 範囲にある場合
	if (itr != m_UpDownLinkRateMap.end())
	{
		if ((*itr).first == t)
		{
			// 予報値マップに時刻がある場合は、マップの値を返す
			return m_UpDownLinkRateMap[t];
		}
		else if (itr == m_UpDownLinkRateMap.begin())
		{
			// １番目より前のため、１番目と２番目の予報値で補間
			auto itr1 = itr;
			itr++;
			auto itr2 = itr;
			return GetLinearInterpolationRate((*itr1).first, (*itr1).second, (*itr2).first, (*itr2).second, t);
		}
		else
		{
			// ひとつ前の予報値と検索した予報値で補間
			auto itr2 = itr;
			itr--;
			auto itr1 = itr;
			return GetLinearInterpolationRate((*itr1).first, (*itr1).second, (*itr2).first, (*itr2).second, t);
		}
	}
	// 範囲に無い場合
	else if (m_UpDownLinkRateMap.size() >= 2)
	{
		// 最後より１つ前の予報値と最後の予報値で補間
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

-# 衛星までの往復距離を直線補間してマップに追加する

@param	t1
@param	rng1
@param	t2
@param	rng2
@retval	なし

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
