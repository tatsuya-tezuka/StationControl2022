// MessageThread.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "MessageThread.h"
#include "MainFrm.h"

// timeGetTime使用
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

// CMessageThread

IMPLEMENT_DYNCREATE(CMessageThread, CWinThread)

CMessageThread::CMessageThread()
: m_bStop(false)
{
}

CMessageThread::~CMessageThread()
{
}

BOOL CMessageThread::InitInstance()
{
	m_bAutoDelete = FALSE;

	return TRUE;
}

int CMessageThread::ExitInstance()
{
	// TODO:    スレッドごとの後処理をここで実行します。
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMessageThread, CWinThread)
END_MESSAGE_MAP()


// CMessageThread メッセージ ハンドラー


int CMessageThread::Run()
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CMessageThread"), _T("Run"), _T("Start"), _T(""), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// 最小タイマ分解能を設定(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();

	try{
		// ディスパッチャからのデータをテンポラリから取得して
		// 工学値変換、各領域への設定を行う
		while (m_bStop==false){

			while (m_bStop == false)
			{
				//for (int i = 0; i < eStation_MAX; i++){
				//	CGraphMutex::Lock(eMessage);
				//	deque<PIPEMONITORACK>& data = theApp.GetDataCollection().GetMessageTemp(i);
				//	if (data.empty() == false){
				//		PIPEMONITORACK newData = data.front();
				//		data.pop_front();
				//		CGraphMutex::Unlock(eMessage);
				//		MessageProc(&newData);
				//	}
				//	else{
				//		CGraphMutex::Unlock(eMessage);
				//		break;
				//	}
				//}
			}

			Sleep(1);
		}
	}
	catch (CGraphException ex){
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CMessageThread"), _T("Run"), _T("Exception"), ex.GetErrorMessage(), nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
	}
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CMessageThread"), _T("Run"), _T("Stop"), _T(""), nLogEx::info);
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
void CMessageThread::MessageProc(PIPEMONITORACK* pbuf)
{
	if (CMainFrame::GetEngval(0) == NULL){
		// 工学値変換用のクラスを作成
		for (int i = 0; i < eStation_MAX; i++)
		{
			CString strFilePath = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, i);
			char szFilePath[_MAX_PATH];
			WideCharToMultiByte(CP_ACP, 0, strFilePath, -1, szFilePath, sizeof(szFilePath), NULL, NULL);

			CMainFrame::SetEngval(i, new CEngValCnv(szFilePath));
		}
	}

	CWnd* pMain = CMainFrame::GetPipeMessageWnd();
	if (pMain == NULL)
		return;

	int err;
	size_t	size;

	BOOL	bAddTrend = FALSE;
	UINT nStation = 0;
	if (pbuf->header.station == 1) nStation = 0;
	if (pbuf->header.station == 2) nStation = 2;
	if (pbuf->header.station == 3) nStation = 3;
	if (pbuf->header.station == 4) nStation = 1;

	// StationControlアプリの場合
	switch (pbuf->header.command)
	{
	case ePipeCommand_monitorget:
	{
		if (theApp.GetPipeIgnore() & 0x01)
			return;

		DWORD	dwStart = timeGetTime();
		UINT nSelectStationMode = theApp.GetSelectStationMode();
		UINT nSelectStation = theApp.GetSelectStation();
		// 監視生データ
		// bufをAppのdequeに登録
		// 周波数制御では、自局以外の送信周波数を取得するため、全局の監視データを取得する
		if (pbuf->header.station >= eStation_Usuda64 + 1 && pbuf->header.station <= eStation_MAX){
			if (CMainFrame::GetEngval(nStation) != NULL){
#ifdef _DEBUG
				DWORD	dwStart = timeGetTime();
#endif

				err = CMainFrame::GetEngval(nStation)->CalcFromMsg((CEngValCnv::sdtp_msg_ex_t*)&(pbuf->buf[0]));

#ifdef _DEBUG
//				TRACE("### MessageProc(%d)\n", timeGetTime() - dwStart);
#endif

				if (err == 0){
					vector<CEngValCnv::obs_tbl_res_t>& data = CMainFrame::GetEngval(nStation)->GetAllVal();
					size = data.size();

					if (size > 0){
						// 衛星データキューに登録
						theApp.GetDataCollection().AddCollection(nStation, data);
						theApp.GetSatelliteData().GetDBAccessCls().SetEqMonData(nStation, data);
						bAddTrend = TRUE;
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
		theApp.GetDataCollection().CreateDataToHistory(pbuf);
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
		if (theApp.GetSelectStation() == nStation){
			::PostMessage(pMain->m_hWnd, eMessage_ThreadNormal, 0, 0);
		}
		else{
		}
	}
	else{
		::PostMessage(pMain->m_hWnd, eMessage_ThreadNormal, 0, 0);
	}
}
