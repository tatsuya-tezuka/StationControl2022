// DataCollectionThread.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "DataCollectionThread.h"
#include "CommBase.h"
#include "DataCollection.h"

// timeGetTime使用
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

// CDataCollectionThread

IMPLEMENT_DYNCREATE(CDataCollectionThread, CWinThread)

CDataCollectionThread::CDataCollectionThread()
: m_bStop(false)
{
}

CDataCollectionThread::~CDataCollectionThread()
{
}

BOOL CDataCollectionThread::InitInstance()
{
	m_bAutoDelete = FALSE;

	return TRUE;
}

int CDataCollectionThread::ExitInstance()
{
	// TODO:    スレッドごとの後処理をここで実行します。
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CDataCollectionThread, CWinThread)
END_MESSAGE_MAP()


// CDataCollectionThread メッセージ ハンドラー


int CDataCollectionThread::Run()
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("Start"), _T(""), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// 最小タイマ分解能を設定(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();

#ifdef _NOCOMM
	for (int i = 0; i < sizeof(m_angle) / sizeof(m_angle[0]); ++i) {
		m_angle[i] = rand() * 360 / RAND_MAX;
	}
#endif

//	BOOL bSig1st = TRUE;

//++ 20181023 UUN-18-074確認用 >>>
#ifdef _DEBUG_STEP_TEST_WAVE
	DWORD nTanSyukiTime = 100;
	DWORD nTanSyukiLastCnt = 1000000;
	DWORD nTanSyukiCurCnt = 0;

	mFp = NULL;
	CString fname;
	fname.Format(_T("%sGraphStepData.csv"), (LPCTSTR)theApp.GetAppPath());
	if ((_wfopen_s(&mFp, fname, _T("w")) != 0) || (mFp == NULL))
	{
		return -1;
	}
#endif /* _DEBUG_STEP_TEST_WAVE */
//++ 20181023 UUN-18-074確認用 <<<

	try{
		// データ構造
		typedef struct
		{
			BYTE*	data;
			LPBYTE  lpData;
			DWORD   dwSize;
			DWORD   dwBytes;
		} structData;

		// 局別データの一時的入れ物
		structData stData[eStation_MAX];
		for (int i = 0; i < eStation_MAX; i++)
		{
			switch (i)
			{
			case eStation_Usuda64:
				stData[i].data = new BYTE[CCommBase::mUdsc64BufferSize];
				break;
			case eStation_Usuda642:
				stData[i].data = new BYTE[CCommBase::mXdsc54MaxBufferSize];
				break;
			case eStation_Uchinoura34:
				stData[i].data = new BYTE[CCommBase::mUsc34BufferSize];
				break;
			case eStation_Uchinoura20:
				stData[i].data = new BYTE[CCommBase::mUsc20BufferSize];
				break;
			default:
				break;
			}

			m_nCntWaitReadForecast[i] = 0;

			stData[i].lpData = stData[i].data;
			stData[i].dwSize = sizeof(stData[i].data);
			stData[i].dwBytes = 0;
		}

		// アプリ動作中はループ
		while (m_bStop == false){

#ifdef _NOCOMM
			BOOL bUpdateForecast = FALSE;
			// 測定開始からの経過時間（ミリ秒）の計算
			DWORD dwElapsed = (timeGetTime() - dwStart);

//#if 0
//			if (dwElapsed >= 500 && dwElapsed < 1000){
//				if (bSig1st)
//				{
//					// 衛星捕捉データ取得
//					for (int i = 0; i < eStation_MAX; i++)
//					{
//#ifndef _ENGVALCNV_TREND
//						stData[i].dwBytes = GetData2(stData[i].lpData, stData[i].dwSize, bSig1st, i);
//#endif
//					}
//					TRACE("bSig1st is TRUE\n");
//					bSig1st = FALSE;
//				}
//			}
//#endif

//++ 20181023 UUN-18-074確認用 >>>
#ifdef _DEBUG_STEP_TEST_WAVE
			nTanSyukiCurCnt = dwElapsed / nTanSyukiTime;
			if (nTanSyukiCurCnt != nTanSyukiLastCnt)
			{
				nTanSyukiLastCnt = nTanSyukiCurCnt;
				stData[0].dwBytes = GetData3(stData[0].lpData, stData[0].dwSize);
				((CDataCollection*)m_ThreadParam.parent)->CreateSpaceData(stData[0].lpData, stData[0].dwBytes, 0);
			}
#endif /* _DEBUG_STEP_TEST_WAVE */
//++ 20181023 UUN-18-074確認用 <<<

			if (dwElapsed >= 1000){
				dwStart = timeGetTime();
				// 描画更新間隔時間の取得
				theApp.GetDrawUpdateInterval(true);
				// サーバーの共有フォルダよりデータを取得する
				theApp.GetSatelliteData().GetShareFile();
//#if 1
				// 衛星捕捉データ取得
				for (int i = 0; i < eStation_MAX; i++)
				{
#ifndef _ENGVALCNV_TREND
					stData[i].dwBytes = GetData(stData[i].lpData, stData[i].dwSize, i);
#endif /* _ENGVALCNV_TREND */
#ifdef MON_UT
					// 監視デバッグ用
					CGraphMutex::Lock(eData);
					map<CString, CEngValCnv::obsname_t> mon_UT = theApp.GetSatelliteData().GetDBAccessCls().GetMonUT();
					if (mon_UT.size() > 0)
					{
						vector<CEngValCnv::obs_tbl_res_t> graphDataList;
						CEngValCnv::obs_tbl_res_t graphData;
						memset(&graphData, 0, sizeof(graphData));
						CTime	curTime;
						theApp.GetCurrentUtcTime(curTime);
						graphData.ld_time_val = (double)curTime.GetTime();
						for (auto itr = mon_UT.begin(); itr != mon_UT.end(); ++itr)
						{
							memcpy(&graphData.obs_adr, &itr->second, sizeof(graphData.obs_adr));
							graphDataList.push_back(graphData);
						}
						theApp.GetDataCollection().AddCollection(theApp.GetSelectStation(), graphDataList);
					}
					CGraphMutex::Unlock(eData);
#endif /* MON_UT */
				}
//#else
//				if (bSig1st == FALSE)
//				{
//					// 衛星捕捉データ取得
//					for (int i = 0; i < eStation_MAX; i++)
//					{
//#ifndef _ENGVALCNV_TREND
//						stData[i].dwBytes = GetData2(stData[i].lpData, stData[i].dwSize, bSig1st, i);
//#endif
//					}
//					TRACE("bSig1st is FALSE\n");
//					bSig1st = TRUE;
//				}
//#endif

				// RFトリップ角定義ファイル読み込み
				theApp.GetSatelliteData().ReadRFTripFile();
				// １局時
				if (theApp.GetSelectStationMode() == eStation_1)
				{
					// 衛星捕捉グラフの予報値ファイルデータ取得
					if (theApp.GetSatelliteData().m_trendData.gryohouti(GET_REAL, theApp.GetSelectStation()) == 0)
					{
						bUpdateForecast = TRUE;
					}
#ifndef _ENGVALCNV_TREND
					// RNG伝搬モニタのダミーデータ作成
					theApp.GetSatelliteData().GetRangePropagation().CreateDummy();
#endif
					// RNG伝搬モニタのステップ実行
					theApp.GetSatelliteData().GetRangePropagation().UpdateStep();
				}
				// セッション状態ファイル読み込み
				(void)theApp.GetSatelliteData().ReadSessionStatusFile();
			}
			else{
				Sleep(1);
				continue;
			}

//			if (stData[0].dwBytes == 0 || stData[0].dwBytes == (DWORD)-1L){
//				Sleep(1);
//				continue;
//			}
			// データ取得が成功したので、ここでデータを格納する
			//			((CDataCollection*)m_ThreadParam.parent)->CreateSpaceData(lpData, dwSize);
//#ifndef _ENGVALCNV_TREND
#if !defined(_ENGVALCNV_TREND) || defined(MON_UT)
			for (int i = 0; i < eStation_MAX; i++)
			{
				if (stData[i].dwBytes == 0 || stData[i].dwBytes == (DWORD)-1L){
					continue;
				}
				((CDataCollection*)m_ThreadParam.parent)->CreateSpaceData(stData[i].lpData, stData[i].dwBytes, i);
			}
#endif /* !defined(_ENGVALCNV_TREND) || defined(MON_UT) */
#ifndef _ENGVALCNV_TREND
			((CDataCollection*)m_ThreadParam.parent)->CreateOperationHistory();
			((CDataCollection*)m_ThreadParam.parent)->CreateControlHistory();
			((CDataCollection*)m_ThreadParam.parent)->CreateEquipmentHistory();
			((CDataCollection*)m_ThreadParam.parent)->CreateEventHistory();
#endif /* _ENGVALCNV_TREND */
#ifdef _DEBUG
#ifdef MON_UT
			m_ThreadParam.callback(eMessage_ThreadNormal, 0, 0);
#endif
#endif
			if (bUpdateForecast)
			{
				// 予報値変更のため、更新通知
				m_ThreadParam.callback(eMessage_UpdateForecastData, 0, 0);
			}
#else
#endif
		}

		// 局別データ入れ物削除
		for (int i = 0; i < eStation_MAX; i++)
		{
			delete[] stData[i].data;
		}
	}
	catch (CGraphException ex){
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("Exception"), ex.GetErrorMessage(), nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		m_ThreadParam.callback(eMessage_ThreadException, 0, 0);
	}
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("Stop"), _T(""), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

//++ 20181023 UUN-18-074確認用 >>>
#ifdef _DEBUG_STEP_TEST_WAVE
	fclose(mFp);
#endif /* _DEBUG_STEP_TEST_WAVE */
//++ 20181023 UUN-18-074確認用 <<<

	return 0;
}

#ifdef _NOCOMM
DWORD CDataCollectionThread::GetData(LPBYTE lpData, DWORD dwSize, UINT nStation /*= eStation_MAX*/)
{
	int i, j, k;
	for (i = 0; i < sizeof(m_angle) / sizeof(m_angle[0]); ++i) {
		m_angle[i] = (m_angle[i] + (i + 1)) % 360;
	}
	SYSTEMTIME time;
	GetSystemTime(&time);

	LPBYTE lpTemp = lpData;
	stCommandHeader header;
	memset(&header, 0, sizeof(stCommandHeader));
	memcpy(lpTemp, (char*)&header, sizeof(stCommandHeader));
	lpTemp += sizeof(stCommandHeader);

	DWORD   dwBytes = sizeof(stCommandHeader);

	// 端末番号取得する
	CString strTermNo;
	UINT nTermNo = theApp.GetTermNo();
	if (nTermNo > 0)
		nTermNo = nTermNo - 1;
	strTermNo.Format(_T("%02u"), nTermNo);

	// 衛星捕捉用データ
	for (i = 0; i < eStation_MAX; i++)
	{
		if (nStation != eStation_MAX && nStation != i)
			continue;

		for (j = 0; j < eTrendWndType_Num; j++)
		{
			int pos = 0;
			for (k = 0; k < MAX_TDATA; k++)
			{
				if (mListSatelliteCaptureSig[i][j][k].strSigName.IsEmpty())
					break;
				stServerItemData data;
				memset(&data, 0, sizeof(stServerItemData));
				CCommBase::ConvertTime(&time, &data.systime);
				//data.systime = time;
				data.kind = 0;
				int nextPos = pos + j;
				if (nextPos > sizeof(m_angle))
				{
					nextPos = pos;
				}

				double a, b;
				switch (j)
				{
				case 0:
					a = 500;
					b = -500;
					break;
				case 1:
					a = 9000000000000;
//					a = 10000;
					b = 0;
					break;
				case 2:
					a = 5000000;
					b = 0;
					break;
				case 3:
					a = 360;
					b = 0;
					break;
				default:
					a = 1000000;
					b = 0;
					break;
				}

				data.value = (double)((sin(m_angle[pos] * 3.1415926538 / 180.0) + 1) / 2 * (a - 0) + b);

				// 予報値、スカイライン、RFトリップの場合はファイルから取得。よってダミーデータ不要。
				if (mListSatelliteCaptureSig[i][j][k].strSigName.Compare(UDSC64_RNG_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(UDSC64_RR_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(UDSC64_EL_EL_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(UDSC64_EL_SKYLINE_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(UDSC64_EL_RF_TRIP_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(XDSC54_RNG_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(XDSC54_RR_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(XDSC54_EL_EL_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(XDSC54_EL_SKYLINE_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(XDSC54_EL_RF_TRIP_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC34_RNG_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC34_RR_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC34_EL_EL_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC34_EL_SKYLINE_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC34_EL_RF_TRIP_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC20_RNG_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC20_RR_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC20_EL_EL_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC20_EL_SKYLINE_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC20_EL_RF_TRIP_SIG) == 0
					)
				{
					pos++;
					continue;
				}

				CString strSigName;
				if (mListSatelliteCaptureSig[i][j][k].strSigName.Compare(UDSC64_AGC_XRX2_INTEG_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(UDSC64_AGC_XXTCR_INTEG_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(XDSC54_AGC_DSNMODEM_INTEG_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC34_AGC_INTEG_SIG) == 0)
				{
					// 局内の場所に応じて監視名の末尾を変える
					strSigName = theApp.GetDataCollection().ReplaceAreaKanshiName(mListSatelliteCaptureSig[i][j][k].strSigName) + strTermNo;
				}
				else
				{
					strSigName = mListSatelliteCaptureSig[i][j][k].strSigName;
				}
				sprintf_s((char*)data.name, _MAX_NAME, "%s", (LPCSTR)CStringA(strSigName));

				memcpy(lpTemp, &data, sizeof(stServerItemData));
				lpTemp += sizeof(stServerItemData);
				dwBytes += sizeof(stServerItemData);
				pos++;
			}
		}
	}

	// サマリ用データ
	if (nStation == eStation_Uchinoura34)
	{
		i = nStation;
		for (j = 0; j < eSummaryWndSetType_Num; j++)
		{
			int pos = 0;
			for (k = 0; k < MAX_TDATA; k++)
			{
				if (mListSummarySig[i][j][k].strSigName.IsEmpty())
					break;

				if (mListSummarySig[i][j][k].strSigName.Compare(USC34_SAGC_TRKRXAGC_A_SIG) != 0 &&
					mListSummarySig[i][j][k].strSigName.Compare(USC34_SAGC_TRKRXAGC_B_SIG) != 0 &&
					mListSummarySig[i][j][k].strSigName.Compare(USC34_XAGC_ACQ_TRKRXAGC_A_SIG) != 0 &&
					mListSummarySig[i][j][k].strSigName.Compare(USC34_XAGC_ACQ_TRKRXAGC_B_SIG) != 0 &&
					mListSummarySig[i][j][k].strSigName.Compare(USC34_XAGC_TRKRXAGC_A_SIG) != 0 &&
					mListSummarySig[i][j][k].strSigName.Compare(USC34_XAGC_TRKRXAGC_B_SIG) != 0 &&
					mListSummarySig[i][j][k].strSigName.Compare(USC34_AGC_XX_TCR_SIG) != 0)
					continue;

				stServerItemData data;
				memset(&data, 0, sizeof(stServerItemData));
				CCommBase::ConvertTime(&time, &data.systime);
				//data.systime = time;
				data.kind = 0;

				int nextPos = pos + j;
				if (nextPos > sizeof(m_angle))
				{
					nextPos = pos;
				}

				double a, b;
				switch (j)
				{
				case 0:
					a = 500;
					b = -500;
					break;
				case 1:
					a = 9000000000000;
//					a = 10000;
					b = 0;
					break;
				case 2:
					a = 5000000;
					b = 0;
					break;
				case 3:
					a = 360;
					b = 0;
					break;
				default:
					a = 1000000;
					b = 0;
					break;
				}

				data.value = (double)((sin(m_angle[pos] * 3.1415926538 / 180.0) + 1) / 2 * (a - 0) + b);
				sprintf_s((char*)data.name, _MAX_NAME, "%s", (LPCSTR)CStringA(mListSummarySig[i][j][k].strSigName));
				memcpy(lpTemp, &data, sizeof(stServerItemData));
				lpTemp += sizeof(stServerItemData);
				dwBytes += sizeof(stServerItemData);
				pos++;
			}
		}
	}
	if (nStation == eStation_Usuda642)
	{
		i = nStation;
		for (j = 0; j < eSummaryWndSetType_Num; j++)
		{
			int pos = 0;
			for (k = 0; k < MAX_TDATA; k++)
			{
				if (mListSummarySig[i][j][k].strSigName.IsEmpty())
					break;

				if (mListSummarySig[i][j][k].strSigName.Compare(XDSC54_AGC_XX_TCR_SIG) != 0)
					continue;

				stServerItemData data;
				memset(&data, 0, sizeof(stServerItemData));
				CCommBase::ConvertTime(&time, &data.systime);
				//data.systime = time;
				data.kind = 0;

				int nextPos = pos + j;
				if (nextPos > sizeof(m_angle))
				{
					nextPos = pos;
				}

				double a, b;
				switch (j)
				{
				case 0:
					a = 500;
					b = -500;
					break;
				case 1:
					a = 9000000000000;
					//					a = 10000;
					b = 0;
					break;
				case 2:
					a = 5000000;
					b = 0;
					break;
				case 3:
					a = 360;
					b = 0;
					break;
				default:
					a = 1000000;
					b = 0;
					break;
				}

				data.value = (double)((sin(m_angle[pos] * 3.1415926538 / 180.0) + 1) / 2 * (a - 0) + b);
				sprintf_s((char*)data.name, _MAX_NAME, "%s", (LPCSTR)CStringA(XDSC54_AGC_XX_TCR_SIG));
				memcpy(lpTemp, &data, sizeof(stServerItemData));
				lpTemp += sizeof(stServerItemData);
				dwBytes += sizeof(stServerItemData);
				pos++;
				goto Label_MakeDataEnd;
			}
		}
	}

Label_MakeDataEnd:

	return dwBytes;
}

DWORD CDataCollectionThread::GetData2(LPBYTE lpData, DWORD dwSize, BOOL bSig1st, UINT nStation /*= eStation_MAX*/)
{
	int i, j, k;
	for (i = 0; i < sizeof(m_angle) / sizeof(m_angle[0]); ++i) {
		m_angle[i] = (m_angle[i] + (i + 1)) % 360;
	}
	SYSTEMTIME time;
	GetSystemTime(&time);

	LPBYTE lpTemp = lpData;
	stCommandHeader header;
	memset(&header, 0, sizeof(stCommandHeader));
	memcpy(lpTemp, (char*)&header, sizeof(stCommandHeader));
	lpTemp += sizeof(stCommandHeader);

	DWORD   dwBytes = sizeof(stCommandHeader);

	// 端末番号取得する
	CString strTermNo;
	UINT nTermNo = theApp.GetTermNo();
	if (nTermNo > 0)
		nTermNo = nTermNo - 1;
	strTermNo.Format(_T("%02u"), nTermNo);


	// 衛星捕捉用データ
	for (i = 0; i < eStation_MAX; i++)
	{
		if (nStation != eStation_MAX && nStation != i)
			continue;

		for (j = 0; j < eTrendWndType_Num; j++)
		{
			int pos = 0;
			for (k = 0; k < MAX_TDATA; k++)
			{
				if (bSig1st == TRUE && k % 2 != 0)
					continue;
				if (bSig1st == FALSE && k % 2 == 0)
					continue;

				if (mListSatelliteCaptureSig[i][j][k].strSigName.IsEmpty())
					break;
				stServerItemData data;
				memset(&data, 0, sizeof(stServerItemData));
				CCommBase::ConvertTime(&time, &data.systime);
				//data.systime = time;
				data.kind = 0;
				int nextPos = pos + j;
				if (nextPos > sizeof(m_angle))
				{
					nextPos = pos;
				}

				double a, b;
				switch (j)
				{
				case 0:
					a = 500;
					b = -500;
					break;
				case 1:
					a = 9000000000000;
					//					a = 10000;
					b = 0;
					break;
				case 2:
					a = 5000000;
					b = 0;
					break;
				case 3:
					a = 360;
					b = 0;
					break;
				default:
					a = 1000000;
					b = 0;
					break;
				}

				data.value = (double)((sin(m_angle[pos] * 3.1415926538 / 180.0) + 1) / 2 * (a - 0) + b);

				// 予報値、スカイライン、RFトリップの場合はファイルから取得。よってダミーデータ不要。
				if (mListSatelliteCaptureSig[i][j][k].strSigName.Compare(UDSC64_RNG_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(UDSC64_RR_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(UDSC64_EL_EL_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(UDSC64_EL_SKYLINE_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(UDSC64_EL_RF_TRIP_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(XDSC54_RNG_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(XDSC54_RR_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(XDSC54_EL_EL_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(XDSC54_EL_SKYLINE_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(XDSC54_EL_RF_TRIP_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC34_RNG_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC34_RR_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC34_EL_EL_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC34_EL_SKYLINE_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC34_EL_RF_TRIP_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC20_RNG_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC20_RR_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC20_EL_EL_PRED_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC20_EL_SKYLINE_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC20_EL_RF_TRIP_SIG) == 0
					)
				{
					pos++;
					continue;
				}

				CString strSigName;
				if (mListSatelliteCaptureSig[i][j][k].strSigName.Compare(UDSC64_AGC_XRX2_INTEG_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(UDSC64_AGC_XXTCR_INTEG_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(XDSC54_AGC_DSNMODEM_INTEG_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC34_AGC_INTEG_SIG) == 0)
				{
					// 局内の場所に応じて監視名の末尾を変える
					strSigName = theApp.GetDataCollection().ReplaceAreaKanshiName(mListSatelliteCaptureSig[i][j][k].strSigName) + strTermNo;
				}
				else
				{
					strSigName = mListSatelliteCaptureSig[i][j][k].strSigName;
				}
				sprintf_s((char*)data.name, _MAX_NAME, "%s", (LPCSTR)CStringA(strSigName));

				memcpy(lpTemp, &data, sizeof(stServerItemData));
				lpTemp += sizeof(stServerItemData);
				dwBytes += sizeof(stServerItemData);
				pos++;
			}
		}
	}

	// サマリ用データ
	if (nStation == eStation_Uchinoura34)
	{
		i = nStation;
		for (j = 0; j < eSummaryWndSetType_Num; j++)
		{
			int pos = 0;
			for (k = 0; k < MAX_TDATA; k++)
			{
				if (bSig1st == TRUE && k % 2 != 0)
					continue;
				if (bSig1st == FALSE && k % 2 == 0)
					continue;

				if (mListSummarySig[i][j][k].strSigName.IsEmpty())
					break;

				if (mListSummarySig[i][j][k].strSigName.Compare(USC34_SAGC_TRKRXAGC_A_SIG) != 0 &&
					mListSummarySig[i][j][k].strSigName.Compare(USC34_SAGC_TRKRXAGC_B_SIG) != 0 &&
					mListSummarySig[i][j][k].strSigName.Compare(USC34_XAGC_ACQ_TRKRXAGC_A_SIG) != 0 &&
					mListSummarySig[i][j][k].strSigName.Compare(USC34_XAGC_ACQ_TRKRXAGC_B_SIG) != 0 &&
					mListSummarySig[i][j][k].strSigName.Compare(USC34_XAGC_TRKRXAGC_A_SIG) != 0 &&
					mListSummarySig[i][j][k].strSigName.Compare(USC34_XAGC_TRKRXAGC_B_SIG) != 0 &&
					mListSummarySig[i][j][k].strSigName.Compare(USC34_AGC_XX_TCR_SIG) != 0)
					continue;

				stServerItemData data;
				memset(&data, 0, sizeof(stServerItemData));
				CCommBase::ConvertTime(&time, &data.systime);
				//data.systime = time;
				data.kind = 0;

				int nextPos = pos + j;
				if (nextPos > sizeof(m_angle))
				{
					nextPos = pos;
				}

				double a, b;
				switch (j)
				{
				case 0:
					a = 500;
					b = -500;
					break;
				case 1:
					a = 9000000000000;
					//					a = 10000;
					b = 0;
					break;
				case 2:
					a = 5000000;
					b = 0;
					break;
				case 3:
					a = 360;
					b = 0;
					break;
				default:
					a = 1000000;
					b = 0;
					break;
				}

				data.value = (double)((sin(m_angle[pos] * 3.1415926538 / 180.0) + 1) / 2 * (a - 0) + b);
				sprintf_s((char*)data.name, _MAX_NAME, "%s", (LPCSTR)CStringA(mListSummarySig[i][j][k].strSigName));
				memcpy(lpTemp, &data, sizeof(stServerItemData));
				lpTemp += sizeof(stServerItemData);
				dwBytes += sizeof(stServerItemData);
				pos++;
			}
		}
	}
	return dwBytes;
}

//++ 20181023 UUN-18-074確認用 >>>
#ifdef _DEBUG_STEP_TEST_WAVE
DWORD CDataCollectionThread::GetData3(LPBYTE lpData, DWORD dwSize)
{
//	static double dRng = 283761600;
	static double dRng = -283761600;
//	double dStep = -26;
	double dStep = 0.01;
//	double dStep = 100000000;

//	static double dRng = -0.5;
//	double dStep = 0.1;

//	static double dRng = 2000.1234;
//	double dStep = -400;
//	double dStep = -0.1;

//	static double dRng = -800000.1234;
//	double dStep = 0.0001;

	//	static double dRng = 20000;
//	static double dRng = 1000000;
//	double dStep = -100;
	static DWORD cnt = 0;
	static DWORD cnt2 = 0;
	cnt++;
//	if (cnt > 1110)
	if (cnt > 100)
	{
		cnt = 0;
#if 0
		dRng += dStep;
#else
		if (cnt2 < 10)
		{
			dRng += dStep;
		}
		else
		{
			dRng += (dStep * -1.0);
		}

		if (cnt2 >= 19)
		{
			cnt2 = 0;
		}
		else
		{
			cnt2++;
		}

		TRACE(_T("cnt=%u\n"), cnt2);
#endif
	}

	SYSTEMTIME time;
	GetSystemTime(&time);

	LPBYTE lpTemp = lpData;
	stCommandHeader header;
	memset(&header, 0, sizeof(stCommandHeader));
	memcpy(lpTemp, (char*)&header, sizeof(stCommandHeader));
	lpTemp += sizeof(stCommandHeader);

	DWORD   dwBytes = sizeof(stCommandHeader);

	// 衛星捕捉用データ
	stServerItemData data;
	memset(&data, 0, sizeof(stServerItemData));
	CCommBase::ConvertTime(&time, &data.systime);
	data.kind = 0;
	data.value = dRng;
	sprintf_s((char*)data.name, _MAX_NAME, "%s", (LPCSTR)CStringA(UDSC64_RNG_XX_TCR_SIG));

	memcpy(lpTemp, &data, sizeof(stServerItemData));
	lpTemp += sizeof(stServerItemData);
	dwBytes += sizeof(stServerItemData);

	if (mFp != NULL)
	{
		char	szBuf[256];
		sprintf_s(szBuf, "\"%04u/%03u %u:%02u:%02u%03u\", %.06f\n", data.systime.wYear, data.systime.wDay, data.systime.wHour, data.systime.wMinute, data.systime.wSecond, data.systime.wMilliseconds, data.value);
		fputs(szBuf, mFp);
	}

	return dwBytes;
}
#endif /* _DEBUG_STEP_TEST_WAVE */
//++ 20181023 UUN-18-074確認用 <<<
#endif

/*============================================================================*/
/*! データ収集

-# データ名称からデータIDを求める

@param		CString	name
@retval		データID

*/
/*============================================================================*/
UINT CDataCollectionThread::_ConvertNameToId(CString name)
{
	return 0;
}
/*============================================================================*/
/*! データ収集

-# データIDからデータ名称を求める

@param		UINT	Id
@retval		データ名称

*/
/*============================================================================*/
CString CDataCollectionThread::_ConvertIdToName(UINT Id)
{
	return _T("");
}
