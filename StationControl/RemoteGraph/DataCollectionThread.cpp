// DataCollectionThread.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RemoteGraph.h"
#include "DataCollectionThread.h"
#include "DataCollection.h"
#include "MainFrm.h"

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
#ifdef _PIPE_COLLECTION
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("Start"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// 受信監視データを初期化
	((CDataCollection*)m_ThreadParam.parent)->DeleteSpaceRecv();

	// DATAフォルダ内のファイルを削除
	theApp.DeleteFileFromDataFolder(-1);

	// リアルタイムモードの時刻情報を初期化
	theApp.SetRealTimeStart(CTime(0));
	theApp.SetRealTimeLast(CTime(0));

	for( int i = 0; i < (MAX_VIEW * MAX_TDATA); i++ )
		theApp.SetRealTimeWriteData(i, CTime(0));
	theApp.SetRealTimeWriteMax(CTime(0));

	theApp.SetStartFile(0);

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("WriteInitialization"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// リアルモード測定データ保存を初期化
	CGraphMutex::Lock(eRealTimeData);
	for( int i = 0; i < (MAX_VIEW * MAX_TDATA); i++ )
		((CDataCollection*)m_ThreadParam.parent)->GetSaveRealTime(i).WriteInitialization(-1, i, theApp.GetStartFile());
	CGraphMutex::Unlock(eRealTimeData);

	// 最小タイマ分解能を設定(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();
	dwStart -= dwStart % 1000;

	((CDataCollection*)m_ThreadParam.parent)->RequestStartSignal();

#ifdef DemoStepData
	static const CHAR* SigName[] = {
		"SS-TCRAGC.AGC_A_AVG",
		"DSNTCR_AGSH.AGC_A_AVG",
		"SS-TCRAGC.AGC_A_AVG",
		"SS-TCRAGC.AGC_A_AVG",
	};
#endif /* DemoStepData */

	try{
		while (m_bStop == false){
			// データ収集
			BYTE	data[mMaxBufferSize];
			LPBYTE  lpData = data;
			DWORD   dwSize = sizeof(data);
			DWORD   dwBytes=0;

			// 測定開始からの経過時間（ミリ秒）の計算
			DWORD	dwCurrent = timeGetTime();
			DWORD dwElapsed = (dwCurrent - dwStart);

			if (dwElapsed >= 1000)
			{

#ifdef DemoStepData
				{
					// 監視デバッグ用
					CGraphMutex::Lock(eRealTimeData);
					vector<CEngValCnv::obs_tbl_res_t> graphDataList;
					CEngValCnv::obs_tbl_res_t graphData;
					memset(&graphData, 0, sizeof(graphData));
					graphData.ld_time_val = (double)CTime::GetCurrentTime().GetTime();
					for (UINT i = 0; i < sizeof(SigName) / sizeof(SigName[0]); i++)
					{
						strcpy_s(graphData.obs_adr.sz_obsname, CTLID_LEN, CStringA(SigName[i]));
						graphData.obs_adr.l_inflg = 1;
						graphDataList.push_back(graphData);
						theApp.GetDataCollection().ConvertTrendData(i, graphDataList);
					}
					CGraphMutex::Unlock(eRealTimeData);
				}
#endif /* DemoStepData */

				if (((CDataCollection*)m_ThreadParam.parent)->GetSpaceTemp().empty()){
					Sleep(10);
					continue;
				}
				//=====================================================//
				//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
				CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("GetData"), _T(""), nLogEx::debug);
				//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
				//=====================================================//
				dwStart = dwCurrent;
			}
			else
			{
				Sleep(5);
				continue;
			}

//			TRACE("CDataCollectionThread::Run eMessage_ThreadNormal\n");
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CString	msg;
			msg.Format(_T("mSpaceTemp size = %I64u"), ((CDataCollection*)m_ThreadParam.parent)->GetSpaceTemp().size());
			CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("eMessage_ThreadNormal"), msg, nLogEx::debug);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
			m_ThreadParam.callback(eMessage_ThreadNormal, 0/*取得分だけMin/Maxを更新*/, 0);

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

//	TRACE("CDataCollectionThread::Run RequestStopSignal Start\n");
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("RequestStopSignal"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	((CDataCollection*)m_ThreadParam.parent)->RequestStopSignal();

//	TRACE("CDataCollectionThread::Run RequestStopSignal End\n");
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("SpaceTempEmpty"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// 取得したデータが処理されているか確認
	if( !((CDataCollection*)m_ThreadParam.parent)->IsSpaceTempEmpty() )
	{
		// 処理されるまでの猶予
		Sleep(100);

		// 存在していたら削除
		((CDataCollection*)m_ThreadParam.parent)->DeleteSpaceTemp();
	}

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("ExistSaveRealTimeDataQue"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// 取得したデータが保存されているか確認
	if( ((CDataCollection*)m_ThreadParam.parent)->IsExistSaveRealTimeDataQue() )
	{
		// 保存されるまでの猶予
		Sleep(100);

		// 存在していたら削除
		((CDataCollection*)m_ThreadParam.parent)->DeleteSaveRealTimeDataQue();
	}

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("WriteFinalization"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// リアルモード測定データ保存を終了
	CGraphMutex::Lock(eRealTimeData);
	for( int i = 0; i < (MAX_VIEW * MAX_TDATA); i++ )
		((CDataCollection*)m_ThreadParam.parent)->GetSaveRealTime(i).WriteFinalization();
	CGraphMutex::Unlock(eRealTimeData);

	// 受信監視データをクリア
	((CDataCollection*)m_ThreadParam.parent)->DeleteSpaceRecv();

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("End"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

#else //#ifdef _PIPE_COLLECTION

//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("Start"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// 受信監視データを初期化
	((CDataCollection*)m_ThreadParam.parent)->DeleteSpaceRecv();

	// DATAフォルダ内のファイルを削除
	theApp.DeleteFileFromDataFolder(-1);

	// リアルタイムモードの時刻情報を初期化
	theApp.SetRealTimeStart(CTime(0));
	theApp.SetRealTimeLast(CTime(0));

	for( int i = 0; i < (MAX_VIEW * MAX_TDATA); i++ )
		theApp.SetRealTimeWriteData(i, CTime(0));
	theApp.SetRealTimeWriteMax(CTime(0));

	theApp.SetStartFile(0);

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("WriteInitialization"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// リアルモード測定データ保存を初期化
	CGraphMutex::Lock(eRealTimeData);
	for (int i = 0; i < (MAX_VIEW * MAX_TDATA); i++)
		((CDataCollection*)m_ThreadParam.parent)->GetSaveRealTime(i).WriteInitialization(-1, i, theApp.GetStartFile());
	CGraphMutex::Unlock(eRealTimeData);

	// 最小タイマ分解能を設定(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();
	dwStart -= dwStart % 1000;

#ifdef _PIPE_COLLECTION
	((CDataCollection*)m_ThreadParam.parent)->RequestStartSignal();
#else
#ifdef _NOCOMM
	for (int i = 0; i < sizeof(m_angle) / sizeof(m_angle[0]); ++i) {
		m_angle[i] = rand() * 360 / RAND_MAX;
	}
#else
#endif
#endif

#ifdef HighSpeedDemo
	mRealTimeStart = CTime(0);
	CTime	nextUpdate = CTime(0);
#endif // HighSpeedDemo

	try{
		while (m_bStop == false){
			// データ収集
			BYTE	data[mMaxBufferSize];
			LPBYTE  lpData = data;
			DWORD   dwSize = sizeof(data);
			DWORD   dwBytes = 0;
#ifdef _NOCOMM
			// 測定開始からの経過時間（ミリ秒）の計算
			DWORD	dwCurrent = timeGetTime();
			DWORD dwElapsed = (dwCurrent - dwStart);

#ifdef HighSpeedDemo
			if (dwElapsed >= 10)
#else
			if (dwElapsed >= 1000)
#endif // HighSpeedDemo
			{
#ifndef HighSpeedDemo
				//=====================================================//
				//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
				CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("GetData"), _T(""), nLogEx::debug);
				//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
				//=====================================================//
#endif // HighSpeedDemo
				dwStart = dwCurrent;
#ifdef _PIPE_COLLECTION
				dwBytes = 100;
#else
				dwBytes = GetData(lpData, dwSize);
#endif
			}
			else
			{
				Sleep(1);
				continue;
			}

			if (dwBytes == 0 || dwBytes == (DWORD)-1L){
				Sleep(1);
				continue;
			}

#ifndef HighSpeedDemo
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("CreateSpaceData"), _T(""), nLogEx::debug);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
#endif // HighSpeedDemo

#ifdef _PIPE_COLLECTION
#else
			// データ取得が成功したので、ここでデータを格納する
			if (!((CDataCollection*)m_ThreadParam.parent)->CreateSpaceData(lpData, dwBytes))
				continue;
#endif

#ifndef HighSpeedDemo
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CString	msg;
			msg.Format(_T("mSpaceTemp size = %d"), ((CDataCollection*)m_ThreadParam.parent)->GetSpaceTemp().size());
			CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("eMessage_ThreadNormal"), msg, nLogEx::info);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
#endif // HighSpeedDemo
#ifdef HighSpeedDemo
			bool	bUpdate = false;
			if ((nextUpdate == CTime(0)) || (mRealTimeStart >= nextUpdate))
			{
				bUpdate = true;
				nextUpdate = mRealTimeStart + CTimeSpan(0, 0, 0, 10);
			}
			m_ThreadParam.callback(eMessage_ThreadNormal, 0/*取得分だけMin/Maxを更新*/, (LPARAM)bUpdate);
#else
			m_ThreadParam.callback(eMessage_ThreadNormal, 0/*取得分だけMin/Maxを更新*/, 0);
#endif // HighSpeedDemo

#else
			dwBytes = ((CDataCollection*)m_ThreadParam.parent)->GetData(lpData, dwSize);
			if (dwBytes == 0 || dwBytes == (DWORD)-1L){
				Sleep(1);
				continue;
			}
			// データ取得が成功したので、ここでデータを格納する
			((CDataCollection*)m_ThreadParam.parent)->CreateSpaceData(lpData, dwBytes);
			m_ThreadParam.callback(eMessage_ThreadNormal, 0/*取得分だけMin/Maxを更新*/, 0);
#endif
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

#ifdef _PIPE_COLLECTION
	((CDataCollection*)m_ThreadParam.parent)->RequestStopSignal();
#endif

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("SpaceTempEmpty"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// 取得したデータが処理されているか確認
	if (!((CDataCollection*)m_ThreadParam.parent)->IsSpaceTempEmpty())
	{
		// 処理されるまでの猶予
		Sleep(100);

		// 存在していたら削除
		((CDataCollection*)m_ThreadParam.parent)->DeleteSpaceTemp();
	}

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("ExistSaveRealTimeDataQue"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// 取得したデータが保存されているか確認
	if (((CDataCollection*)m_ThreadParam.parent)->IsExistSaveRealTimeDataQue())
	{
		// 保存されるまでの猶予
		Sleep(100);

		// 存在していたら削除
		((CDataCollection*)m_ThreadParam.parent)->DeleteSaveRealTimeDataQue();
	}

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("WriteFinalization"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// リアルモード測定データ保存を終了
	CGraphMutex::Lock(eRealTimeData);
	for (int i = 0; i < (MAX_VIEW * MAX_TDATA); i++)
		((CDataCollection*)m_ThreadParam.parent)->GetSaveRealTime(i).WriteFinalization();
	CGraphMutex::Unlock(eRealTimeData);

	// 受信監視データをクリア
	((CDataCollection*)m_ThreadParam.parent)->DeleteSpaceRecv();

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CDataCollectionThread"), _T("Run"), _T("End"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
#endif //#ifdef _PIPE_COLLECTION

	return 0;
}

#ifdef _NOCOMM
DWORD CDataCollectionThread::GetData(LPBYTE lpData, DWORD dwSize)
{
	int i, j, k;
	for (i = 0; i < sizeof(m_angle) / sizeof(m_angle[0]); ++i) {
		m_angle[i] = (m_angle[i] + 2 * (i + 1)) % 360;
	}

	SYSTEMTIME	time;
#ifdef HighSpeedDemo
	if( mRealTimeStart == CTime(0) )
	{
		GetSystemTime(&time);
		mRealTimeStart = time;
	}
	else
	{
		mRealTimeStart += CTimeSpan(0, 0, 0, 1);
		mRealTimeStart.GetAsSystemTime(time);
	}
#else
	GetSystemTime(&time);
#endif // HighSpeedDemo

	bool	bLeapSec = false;

#ifdef	_TEST_LEAP
	static	bool	bLeapExist = false;
	if( bLeapExist )
	{
		if( time.wSecond == 0 )
		{
			CTime	tmpTime = time;
			tmpTime -= CTimeSpan(0, 0, 0, 1);
			tmpTime.GetAsSystemTime(time);
		}
		else
			time.wSecond--;
	}
	else
	{
		if( time.wSecond == 0 )
		{
			bLeapExist = true;

			bLeapSec = true;
		}
	}
#endif	// _TEST_LEAP

	LPBYTE lpTemp = lpData;
	stCommandHeader header;
	memset(&header, 0, sizeof(stCommandHeader));
	memcpy(lpTemp, (char*)&header, sizeof(stCommandHeader));
	lpTemp += sizeof(stCommandHeader);

	DWORD   dwBytes = sizeof(stCommandHeader);

	vector<stSelectData>&	seldatalist = theApp.GetDataCollection().GetSelectDataList();
	int pos = 0;
	for( UINT idx = 0; idx < seldatalist.size(); idx++ )
	{
		// データ取得対象フラグを確認
		if( !seldatalist[idx].valid )
			continue;

		DWORD	id = seldatalist[idx].id;

		if( i != ((id >> 28) & 0xF) )
			pos = 0;

		i = (id >> 28) & 0xF;
		j = (id >> 16) & 0xFFF;
		k = (id >>  0) & 0xFFFF;

		stServerItemData data;
		memset(&data, 0, sizeof(stServerItemData));

		((CDataCollection*)m_ThreadParam.parent)->ConvertTime(&time, &data.systime);
		data.kind = 0;
		data.status = SDS_VALID | (bLeapSec ? SDS_LEAP : 0x0);

		int nextPos = pos + j;
		if (nextPos > sizeof(m_angle))
		{
			nextPos = pos;
		}
		if (i == 0)
		{
			data.value = (double)((sin(m_angle[pos] * 3.1415926538 / 180.0) + 1) / 2 * (200 - 0) - 100);
		}
		else if (i == 1)
		{
			data.value = (double)((sin(m_angle[pos] * 3.1415926538 / 180.0) + 1) / 2 * (100 - 0) - 0) +
				(double)((sin(m_angle[nextPos] * 3.1415926538 / 180.0) + 1) / 2 * (100 - 0) - 0);
		}
		else if (i == 2)
		{
			data.value = (double)((sin(m_angle[pos] * 3.1415926538 / 180.0) + 1) / 2 * (100 - 0) - 100);
		}
		else
		{
			data.value = (double)((sin(m_angle[pos] * 3.1415926538 / 180.0) + 1) / 2 * (200 - 0) - 100) +
				(double)((sin(m_angle[nextPos] * 3.1415926538 / 180.0) + 1) / 2 * (200 - 0) - 100);
		}
		sprintf_s((char*)data.name, _MAX_NAME, "%s", (LPCSTR)CStringA(seldatalist[idx].name));

		memcpy(lpTemp, &data, sizeof(stServerItemData));
		lpTemp += sizeof(stServerItemData);
		dwBytes += sizeof(stServerItemData);
		pos++;
	}

	return dwBytes;
}
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

// CSaveRealTimeDataThread

IMPLEMENT_DYNCREATE(CSaveRealTimeDataThread, CWinThread)

CSaveRealTimeDataThread::CSaveRealTimeDataThread()
: m_bStop(false)
{
}

CSaveRealTimeDataThread::~CSaveRealTimeDataThread()
{
}

BOOL CSaveRealTimeDataThread::InitInstance()
{
	m_bAutoDelete = FALSE;

	return TRUE;
}

int CSaveRealTimeDataThread::ExitInstance()
{
	// TODO:    スレッドごとの後処理をここで実行します。
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSaveRealTimeDataThread, CWinThread)
END_MESSAGE_MAP()


// CSaveRealTimeDataThread メッセージ ハンドラー


int CSaveRealTimeDataThread::Run()
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CSaveRealTimeDataThread"), _T("Run"), _T("Start"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	try
	{
		vector<stSpaceData>	save_data;

		while( m_bStop == false )
		{
			// 先頭のキューを取得
			if( !((CDataCollection*)m_ThreadParam.parent)->GetSaveRealTimeDataQue(save_data) )
			{
				Sleep(10);	// データが無い
				continue;	
			}

#ifndef HighSpeedDemo
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CString msg;
			msg.Format(_T("data size = %I64u"), save_data.size());
			CLogTraceEx::Write(_T(""), _T("CSaveRealTimeDataThread"), _T("Run"), _T("PopSaveRealTimeDataQue"), msg, nLogEx::debug);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
#endif // HighSpeedDemo

			// リアルモード測定データをファイルに書き込む
			for( int i = 0; i < save_data.size(); i++ )
			{
				// 空データを確認
				if( CTime(save_data[i].systime) == CTime(0) )
					continue;

				((CDataCollection*)m_ThreadParam.parent)->GetSaveRealTime(i).WriteData(save_data[i]);

				// １ファイルあたりのデータ数に達したらファイルを変更
				if( ((CDataCollection*)m_ThreadParam.parent)->GetSaveRealTime(i).GetDataRWPos() >= MAX_SIZE_PER_MMDATA )
				{
					((CDataCollection*)m_ThreadParam.parent)->GetSaveRealTime(i).WriteFinalization();
					((CDataCollection*)m_ThreadParam.parent)->GetSaveRealTime(i).WriteInitialization(-1, i, ((CDataCollection*)m_ThreadParam.parent)->GetSaveRealTime(i).GetStartFile() + 1);
				}
			}

			// キューをポップ
			((CDataCollection*)m_ThreadParam.parent)->PopSaveRealTimeDataQue();

			save_data.clear();
		}
	}
	catch( CGraphException ex )
	{
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CSaveRealTimeDataThread"), _T("Run"), _T("Exception"), ex.GetErrorMessage(), nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		m_ThreadParam.callback(eMessage_ThreadException, 0, 0);
	}

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CSaveRealTimeDataThread"), _T("Run"), _T("End"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

//	return CWinThread::Run();
	return 0;
}

// CSaveAnalysisDataThread

IMPLEMENT_DYNCREATE(CSaveAnalysisDataThread, CWinThread)

CSaveAnalysisDataThread::CSaveAnalysisDataThread()
: m_bStop(false)
{
	m_bFinish = false;
}

CSaveAnalysisDataThread::~CSaveAnalysisDataThread()
{
}

BOOL CSaveAnalysisDataThread::InitInstance()
{
	m_bAutoDelete = FALSE;

	return TRUE;
}

int CSaveAnalysisDataThread::ExitInstance()
{
	// TODO:    スレッドごとの後処理をここで実行します。
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSaveAnalysisDataThread, CWinThread)
END_MESSAGE_MAP()


// CSaveAnalysisDataThread メッセージ ハンドラー

void CSaveAnalysisDataThread::CompAnalysisData(int tbl, vector<stSpaceData>& list, stSpaceData& data)
{
	// 圧縮データが存在しないなら初期化
	if( list.empty() )
	{
		list.push_back(data);

		return;
	}

	// 最終位置のデータ数が圧縮数に達しているか確認
	if( tblRealScaleInfo[tbl].cmp_rate <= list.back().mmPair.cnt )
	{
		list.push_back(data);

		return;
	}

	// 圧縮データを更新
	stSpaceData&	cmp_value = list.back();
	stSpaceData&	src_value = data;
	cmp_value.mmPair.cnt += src_value.mmPair.cnt;

#if 0
	cmp_value.mmPair.max = __max(cmp_value.mmPair.max, src_value.mmPair.max);
	cmp_value.mmPair.min = __min(cmp_value.mmPair.min, src_value.mmPair.min);
	if( (cmp_value.status == SDS_INVALID) && (src_value.status & SDS_VALID) )
		cmp_value.status = src_value.status;	// 有効データに変更
#else
	if( (cmp_value.status & SDS_VALID) && (src_value.status & SDS_VALID) )	// 有効⇔有効
	{
		cmp_value.mmPair.max = __max(cmp_value.mmPair.max, src_value.mmPair.max);
		cmp_value.mmPair.min = __min(cmp_value.mmPair.min, src_value.mmPair.min);
	}
	else if( (cmp_value.status == SDS_INVALID) && (src_value.status & SDS_VALID) )	// 無効⇔有効
	{
		cmp_value.mmPair.max = src_value.mmPair.max;
		cmp_value.mmPair.min = src_value.mmPair.min;
		cmp_value.status = src_value.status;	// 有効データに変更
	}

	if( cmp_value.status == SDS_INVALID )
	{
		cmp_value.mmPair.max = 0.0;
		cmp_value.mmPair.min = 0.0;
	}
#endif
}

int CSaveAnalysisDataThread::Run()
{
	CString	msg;
	msg.Format(_T("Run(view = %d)"), m_nView + 1);

	CDataCollection	*p_col = (CDataCollection *)m_ThreadParam.parent;

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CSaveAnalysisDataThread"), msg, _T("Start"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// DATAフォルダ内のファイルを削除
	theApp.DeleteFileFromDataFolder(m_nView);

	try
	{
		stQueData	save_data;

		CPropertiesData&	prop = theApp.GetPropertiesData(m_nView);

		CTime	s_time = p_col->GetAnalysisStart(m_nView);
		CTime	e_time = p_col->GetAnalysisEnd(m_nView);

		stSpaceData	invalid_data;
		memset(&invalid_data, 0, sizeof(stSpaceData));
		invalid_data.status = SDS_INVALID;
		invalid_data.mmPair.cnt = 1;
		invalid_data.mmPair.max = 0.0;
		invalid_data.mmPair.min = 0.0;

		// ノンリアルモード測定データ保存を初期化
		CGraphMutex::Lock_Analysis(m_nView);
		for( int i = 0; i < MAX_TDATA; i++ )
			p_col->GetSaveAnalysis(m_nView, i).WriteInitialization(m_nView, i, 0);
		CGraphMutex::Unlock_Analysis(m_nView);

		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CSaveAnalysisDataThread"), msg, _T("Save file start"), _T(""), nLogEx::debug);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//

		while( m_bStop == false )
		{
			// キューをポップ
			if( !p_col->GetSaveAnalysisDataQue(m_nView, save_data) )
			{
				Sleep(10);	// データが無い
				continue;	
			}

//			//=====================================================//
//			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
//			CLogTraceEx::Write(_T(""), _T("CSaveAnalysisDataThread"), _T("Run"), _T("PopSaveAnalysisDataQue"), _T(""), nLogEx::debug);
//			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
//			//=====================================================//

			// 無効データを確認
			if( save_data.data.status == SDS_INVALID )
			{
				// キューをポップ
				p_col->PopSaveAnalysisDataQue(m_nView);

				continue;
			}

			// 時刻を確認
			CTime	c_time = CTime(save_data.data.systime);
			if( (c_time.GetTime() < s_time.GetTime()) || (e_time.GetTime() < c_time.GetTime()) )
			{
				// キューをポップ
				p_col->PopSaveAnalysisDataQue(m_nView);

				CLogTraceEx::Write(_T(""), _T("CSaveAnalysisDataThread"), _T("Run"), _T("Time Range Over"), _T(""), nLogEx::info);

				continue;
			}

			// 監視データの時刻からデータ位置を取得
			unsigned __int64	c_pos = c_time.GetTime() - s_time.GetTime();

			CGraphMutex::Lock_Analysis(m_nView);

			// ノンリアルモード測定データを処理
			for( int i = 0; i < MAX_TDATA; i++ )
			{
				// 監視データ名から検出
				if( prop.item[i].dataName != save_data.name )
					continue;

				// 無効データをファイルに書き込む
				unsigned __int64	t_pos;
				while( (t_pos = p_col->GetSaveAnalysis(m_nView, i).GetDataRWPos()) < c_pos )
				{
					CTime(s_time.GetTime() + (__time64_t)t_pos).GetAsSystemTime(invalid_data.systime);
					p_col->GetSaveAnalysis(m_nView, i).WriteData(invalid_data);
				}

				// ノンリアルモード測定データをファイルに書き込む
				p_col->GetSaveAnalysis(m_nView, i).WriteData(save_data.data);
			}

			CGraphMutex::Unlock_Analysis(m_nView);

			// キューをポップ
			p_col->PopSaveAnalysisDataQue(m_nView);
		}

		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CSaveAnalysisDataThread"), msg, _T("Save file end"), _T(""), nLogEx::debug);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//

		// 終了時刻からデータ位置を取得
		unsigned __int64	e_pos = e_time.GetTime() - s_time.GetTime();

		CGraphMutex::Lock_Analysis(m_nView);

		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CSaveAnalysisDataThread"), msg, _T("Save file remain data start"), _T(""), nLogEx::debug);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//

		vector<stSpaceData>	t_data;
		t_data.resize(1);

		// 読み込まれなかったデータを保存
		for( int i = 0; i < MAX_TDATA; i++ )
		{
			// 監視データ名から検出
			if( p_col->GetSelectDataId(prop.item[i].dataName) != 0xffffffff )
			{
				// 無効データをファイルに書き込む
				unsigned __int64	t_pos;
				while( (t_pos = p_col->GetSaveAnalysis(m_nView, i).GetDataRWPos()) <= e_pos )
				{
					CTime(s_time.GetTime() + (__time64_t)t_pos).GetAsSystemTime(invalid_data.systime);
					p_col->GetSaveAnalysis(m_nView, i).WriteData(invalid_data);
				}
			}

			// ノンリアルモード測定データ保存を終了
			p_col->GetSaveAnalysis(m_nView, i).WriteFinalization();
		}

		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CSaveAnalysisDataThread"), msg, _T("Save file remain data end"), _T(""), nLogEx::debug);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//

		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CSaveAnalysisDataThread"), msg, _T("Create All data start"), _T(""), nLogEx::debug);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//

		for( int i = 0; i < MAX_TDATA; i++ )
		{
			// 監視データ名から検出
			if( p_col->GetSelectDataId(prop.item[i].dataName) == 0xffffffff )
				continue;

			// ALL用の描画データを作成
			if( tblRealScaleInfo[p_col->GetTrendAllTblNo(m_nView)].cmp_rate > 1 )
			{
				vector<stSpaceData>	t_list;

				t_list.reserve(tblRealScaleInfo[p_col->GetTrendAllTblNo(m_nView)].dsp_data);

				for( unsigned __int64 pos = 0; pos <= e_pos; pos++ )
				{
					// ファイルからデータを取得
					p_col->GetTrendZoomMemMap(m_nView, i).ReadData(pos, pos, t_data);

					// データを圧縮
					CompAnalysisData(p_col->GetTrendAllTblNo(m_nView), t_list, t_data[0]);
				}

				// データをコピー
				memcpy((stSpaceData *)&(p_col->GetTrendAllData(m_nView, i)[0]), (stSpaceData *)&(t_list[0]), sizeof(stSpaceData) * t_list.size());

				t_list.clear();
			}
			else
			{
				for( unsigned __int64 pos = 0; pos <= e_pos; pos++ )
				{
					// ファイルからデータを取得
					p_col->GetTrendZoomMemMap(m_nView, i).ReadData(pos, pos, t_data);

					// データをコピー
					p_col->GetTrendAllData(m_nView, i).push_back(t_data[0]);
				}
			}
		}

		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CSaveAnalysisDataThread"), msg, _T("Create All data end"), _T(""), nLogEx::debug);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//

		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CSaveAnalysisDataThread"), msg, _T("Update Zoom data start"), _T(""), nLogEx::debug);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//

		// ZOOMの描画位置／データを更新
		p_col->UpdateTrendZoomTblNo(m_nView, m1, false);
		p_col->UpdateTrendZoomPos(m_nView, 0/* 初期値(0：最初のデータ番号) */, false);
		p_col->UpdateTrendZoomData(m_nView, false);

		// カーソルのデータ番号／データを更新
		p_col->UpdateTrendCursorPos(m_nView, 0/* 初期値(0：最初のデータ番号) */, false);
		p_col->UpdateTrendCursorData(m_nView, false);

		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CSaveAnalysisDataThread"), msg, _T("Update Zoom data end"), _T(""), nLogEx::debug);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//

		// ビューにグラフ更新を通知
		CWnd	*pWnd = ((CMainFrame *)theApp.m_pMainWnd)->FindChildListWnd(m_nView);
		if( pWnd != NULL )
			theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 1, (LPARAM)m_nView);

		CGraphMutex::Unlock_Analysis(m_nView);
	}
	catch( CGraphException ex )
	{
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CSaveAnalysisDataThread"), msg, _T("Exception"), ex.GetErrorMessage(), nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		m_ThreadParam.callback(eMessage_ThreadException, 0, 0);
	}

	// スレッド完了フラグをセット
	m_bFinish = true;

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CSaveAnalysisDataThread"), msg, _T("End"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

//	return CWinThread::Run();
	return 0;
}
