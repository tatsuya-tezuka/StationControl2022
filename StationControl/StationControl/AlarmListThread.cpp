// AlarmListThread.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "AlarmListThread.h"


// CAlarmListThread

IMPLEMENT_DYNCREATE(CAlarmListThread, CWinThread)

CAlarmListThread::CAlarmListThread()
: m_bStop(false)
{
}

CAlarmListThread::~CAlarmListThread()
{
}

BOOL CAlarmListThread::InitInstance()
{
	m_bAutoDelete = FALSE;
	return TRUE;
}

int CAlarmListThread::ExitInstance()
{
	// TODO:    スレッドごとの後処理をここで実行します。
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CAlarmListThread, CWinThread)
END_MESSAGE_MAP()


// CAlarmListThread メッセージ ハンドラー


int CAlarmListThread::Run()
{
	////=====================================================//
	////↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	//CLogTraceEx::Write(_T(""), _T("CAlarmListThread"), _T("Run"), _T("Start"), _T(""), nLogEx::info);
	////↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	////=====================================================//
	//CShareAlarmIf& mAlarmIf = theApp.GetAlarmIf();
	//vector<CShareAlarmIf::stAlarmItem>& mAlarmList = theApp.GetAlarmList();
	//
	//while (m_bStop){
	//	CGraphMutex::Lock(eAlarm);
	//	vector<CShareAlarmIf::stAlarmItem> temp(0);
	//	temp.clear();
	//	temp.resize(CShareAlarmIf::mAlarmItemMax*CShareAlarmIf::mAlarmStationMax);
	//	bool ret = mAlarmIf.Recv((CShareAlarmIf::stAlarmItem*)&temp.at(0), (UINT)temp.size());
	//	if (ret == true){
	//		//=====================================================//
	//		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	//		CLogTraceEx::Write(_T(""), _T("CAlarmListThread"), _T("Run"), _T("Update Alarm List"), _T(""), nLogEx::debug);
	//		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//		//=====================================================//
	//		mAlarmList.swap(temp);
	//		mAlarmList.resize(CShareAlarmIf::mAlarmItemMax*CShareAlarmIf::mAlarmStationMax);
	//	}
	//	CGraphMutex::Unlock(eAlarm);
	//
	//	Sleep(1000);
	//}
	//
	////=====================================================//
	////↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	//CLogTraceEx::Write(_T(""), _T("CAlarmListThread"), _T("Run"), _T("Stop"), _T(""), nLogEx::info);
	////↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	////=====================================================//

	return 0;
}
