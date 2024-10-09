// MessageThread.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
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
				CGraphMutex::Lock(eMessage);
				deque<PIPEMONITORACK>& data = theApp.GetMessageTemp(theApp.GetSelectStation());
				if (data.empty() == false){
					PIPEMONITORACK newData = data.front();
					data.pop_front();
					CGraphMutex::Unlock(eMessage);
					MessageProc(&newData);
				}
				else{
					CGraphMutex::Unlock(eMessage);
					break;
				}
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
	if (CMainFrame::GetEngval() == NULL){
		// 工学値変換用のクラスを作成
		CString strFilePath = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, theApp.GetSelectStation());
		char szFilePath[_MAX_PATH];
		WideCharToMultiByte(CP_ACP, 0, strFilePath, -1, szFilePath, sizeof(szFilePath), NULL, NULL);
		CMainFrame::SetEngval(new CEngValCnv(szFilePath));
	}


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
		// 工学値変換＆画面更新は対象局のみとする
		if (nStation != theApp.GetSelectStation())
			return;

		DWORD	dwStart = timeGetTime();

		// 監視生データ
		// bufをAppのdequeに登録
		// 局番号が範囲内の場合
		if (pbuf->header.station >= eStation_Usuda64 + 1 && pbuf->header.station <= eStation_MAX)
		{
			if (CMainFrame::GetEngval() != NULL)
			{
				err = CMainFrame::GetEngval()->CalcFromMsg((CEngValCnv::sdtp_msg_ex_t*)&(pbuf->buf[0]));

				if (err == 0)
				{

					vector<CEngValCnv::obs_tbl_res_t>& data = CMainFrame::GetEngval()->GetAllVal();
					size = data.size();

					if (size > 0)
					{
						// 監視データ登録
						theApp.SetEqMonData(nStation, data);
					}
				}
			}
			else{
			}
		}
		DWORD dwElapsed = (timeGetTime() - dwStart);
		TRACE("### ePipeCommand_monitorget Elapsed(%d)\n", dwElapsed);
	}
	break;
	case	ePipeCommand_history:	// 履歴データ通知
		theApp.CreateDataToHistory(pbuf);
		::PostMessage(theApp.m_pMainWnd->m_hWnd, ID_PIPE_SERVER, 9, (LPARAM)pbuf);
		break;
	default:
		return;
		break;
	}

	// 履歴は取得したら直ぐ更新
	// 監視は取得しても更新しないで、CTimeLineView::OnTimerでの1秒更新とする
}
