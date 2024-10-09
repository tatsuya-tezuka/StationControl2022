/*============================================================================*/
/*! MessageStationThread.cpp

-#局別メッセージスレッド

@param
@retval

*/
/*============================================================================*/
// MessageStationThread.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "MessageStationThread.h"
#include "MainFrm.h"

// timeGetTime使用
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

// CMessageStationThread

IMPLEMENT_DYNCREATE(CMessageStationThread, CWinThread)

CMessageStationThread::CMessageStationThread()
: m_bStop(false)
, m_nStation(eStation_Usuda64)
, m_nMutex(eMessageUdsc64)
{
}

CMessageStationThread::~CMessageStationThread()
{
}

BOOL CMessageStationThread::InitInstance()
{
	m_bAutoDelete = FALSE;

	return TRUE;
}

int CMessageStationThread::ExitInstance()
{
	// TODO:    スレッドごとの後処理をここで実行します。
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMessageStationThread, CWinThread)
END_MESSAGE_MAP()


// CMessageStationThread メッセージ ハンドラー

int CMessageStationThread::Run()
{
	CString strLog;
	strLog.Format(_T("Run Station(%u)"), m_nStation);

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CMessageStationThread"), strLog, _T("Start"), _T(""), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// 最小タイマ分解能を設定(1ms)
	timeBeginPeriod(1);

	if (CMainFrame::GetEngval(m_nStation) == NULL){
		// 工学値変換用のクラスを作成
		CString strFilePath = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, m_nStation);
		char szFilePath[_MAX_PATH];
		WideCharToMultiByte(CP_ACP, 0, strFilePath, -1, szFilePath, sizeof(szFilePath), NULL, NULL);

		CMainFrame::SetEngval(m_nStation, new CEngValCnv(szFilePath));
	}

	try{
		// ディスパッチャからのデータをテンポラリから取得して
		// 工学値変換、各領域への設定を行う
		while (m_bStop == false){

			while (m_bStop == false)
			{
//++ 20181023 UUN-18-074確認用 >>>
#ifndef _DEBUG_STEP_TEST_WAVE
				CGraphMutex::Lock(m_nMutex);
				deque<PIPEMONITORACK>& data = theApp.GetDataCollection().GetMessageTemp(m_nStation);
				if (data.empty() == false){
					PIPEMONITORACK newData = data.front();
					data.pop_front();
					//TRACE("##########[%d] SIZE=%d\n", m_nStation, data.size());
					CGraphMutex::Unlock(m_nMutex);
					MessageProc(&newData);
				}
				else{
					CGraphMutex::Unlock(m_nMutex);
					break;
				}
#else
				if (m_nStation == 0)
				{
					CWnd* pMain = CMainFrame::GetPipeMessageWnd();
					if (pMain == NULL)
						return 0;
					// 衛星データキューに登録
					theApp.GetDataCollection().AddCollection(NULL, NULL, NULL);

					static DWORD mCallbackProcStart = 0;
					DWORD mDrawInterval = theApp.GetDrawUpdateInterval(false);
					DWORD dwElaps = timeGetTime() - mCallbackProcStart;
					if (dwElaps < mDrawInterval){
						continue;
					}
					mCallbackProcStart = timeGetTime();

					// 画面更新は対象局のみとする
					if (theApp.GetSelectStationMode() == eStation_1){
						if (theApp.GetSelectStation() == m_nStation){
							::PostMessage(pMain->m_hWnd, eMessage_ThreadNormal, 0, 0);
						}
						else{
						}
					}
					else{
						::PostMessage(pMain->m_hWnd, eMessage_ThreadNormal, 0, 0);
					}

				}
#endif /* _DEBUG_STEP_TEST_WAVE */
//++ 20181023 UUN-18-074確認用 <<<
			}

			Sleep(1);
		}
	}
	catch (CGraphException ex){
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CMessageStationThread"), strLog, _T("Exception"), ex.GetErrorMessage(), nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
	}
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CMessageStationThread"), strLog, _T("Stop"), _T(""), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	return 0;
}
/*============================================================================*/
/*! プロセス間通信

-# AP層からのメッセージ受信

@param

@retval
*/
/*============================================================================*/
void CMessageStationThread::MessageProc(PIPEMONITORACK* pbuf)
{
	CWnd* pMain = CMainFrame::GetPipeMessageWnd();
	if (pMain == NULL)
		return;

	int err;
	size_t	size;

	UINT nStation = 0;
	if (pbuf->header.station == 1) nStation = 0;
#ifdef _DEBUG_PIPEUSE64M
	if (pbuf->header.station == 1) nStation = 1;
#endif
	if (pbuf->header.station == 2) nStation = 2;
#ifdef _DEBUG_PIPEUSE34M
	if (pbuf->header.station == 2) nStation = 1;
#endif
	if (pbuf->header.station == 3) nStation = 3;
	if (pbuf->header.station == 4) nStation = 1;
	UINT nSelectStationMode = theApp.GetSelectStationMode();

	// StationControlアプリの場合
	switch (pbuf->header.command)
	{
	case ePipeCommand_monitorget:
	{
		if (theApp.GetPipeIgnore() & 0x01)
			return;

		DWORD	dwStart = timeGetTime();
		// 監視生データ
		// bufをAppのdequeに登録
		// 1局モードの場合は自局、または４局モード時のみ登録
		if (nStation == m_nStation){
			if (CMainFrame::GetEngval(m_nStation) != NULL){
#ifdef _DEBUG
				DWORD	dwStart = timeGetTime();
//				TRACE("### Station(%d) MessageProc(%d) \n", m_nStation, timeGetTime() - dwStart);
#endif
				err = CMainFrame::GetEngval(m_nStation)->CalcFromMsg((CEngValCnv::sdtp_msg_ex_t*)&(pbuf->buf[0]));

#ifdef _DEBUG
				//				TRACE("### MessageProc(%d)\n", timeGetTime() - dwStart);
#endif

				if (err == 0){
					vector<CEngValCnv::obs_tbl_res_t>& data = CMainFrame::GetEngval(m_nStation)->GetAllVal();
					size = data.size();

					if (size > 0){
						// 衛星データキューに登録
						theApp.GetDataCollection().AddCollection(m_nStation, data);
						theApp.GetSatelliteData().GetDBAccessCls().SetEqMonData(m_nStation, data);
					}
				}
			}
		}
		DWORD dwElapsed = (timeGetTime() - dwStart);
		//		TRACE("### ePipeCommand_monitorget Elapsed(%d)\n", dwElapsed);

	}
	break;
	case	ePipeCommand_history:	// 履歴データ通知
		if (theApp.GetPipeIgnore() & 0x02)
			return;
//		theApp.GetDataCollection().CreateDataToHistory(pbuf);
		// 局識別子が100または101の場合は１局モードは自局、４局モードは臼田のみ
		if (pbuf->header.station == eTkscLog_Ope || pbuf->header.station == eTkscLog_Ctl)
		{
			if ((nSelectStationMode == eStation_1 && theApp.GetSelectStation() == m_nStation) ||
				(nSelectStationMode == eStation_5 && eStation_Usuda64 == m_nStation))
			{
				// 履歴生成
				theApp.GetDataCollection().CreateDataToHistory(pbuf);
			}
			else
			{
				// 履歴破棄のため何もしない
				return;
			}
		}
		else
		{
			// 履歴生成
			theApp.GetDataCollection().CreateDataToHistory(pbuf);
		}
		::PostMessage(pMain->m_hWnd, eMessage_ThreadNormal, 0, 0);
		return;
		break;
	default:
		return;
		break;
	}

	static DWORD mCallbackProcStart = 0;
	DWORD mDrawInterval = theApp.GetDrawUpdateInterval(false);
	DWORD dwElaps = timeGetTime() - mCallbackProcStart;
	if (dwElaps < mDrawInterval){
		return;
	}
	mCallbackProcStart = timeGetTime();

	// 画面更新は対象局のみとする
	if (theApp.GetSelectStationMode() == eStation_1){
		if (theApp.GetSelectStation() == m_nStation){
			::PostMessage(pMain->m_hWnd, eMessage_ThreadNormal, 0, 0);
		}
		else{
		}
	}
	else{
		::PostMessage(pMain->m_hWnd, eMessage_ThreadNormal, 0, 0);
	}
}
