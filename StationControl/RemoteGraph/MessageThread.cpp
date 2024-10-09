// MessageThread.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RemoteGraph.h"
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
		if (m_nStation == 0){
			while (m_bStop == false){
				Sleep(10);
			}
		}
		else{
			// ディスパッチャからのデータをテンポラリから取得して
			// 工学値変換、各領域への設定を行う
			while (m_bStop == false){

				while (m_bStop == false)
				{
					CGraphMutex::Lock((eMutex)(eMessage + m_nStation));
					deque<PIPEMONITORACK>& data = theApp.GetMessageTemp(m_nStation-1);
					if (data.empty() == false){
						PIPEMONITORACK newData = data.front();
						data.pop_front();
						CGraphMutex::Unlock((eMutex)(eMessage + m_nStation));
						MessageProc(&newData);
					}
					else{
						CGraphMutex::Unlock((eMutex)(eMessage + m_nStation));
						break;
					}
				}

				Sleep(1);
			}
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
	CString strDebug;

	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("Pipe Command Begin"), _T(""), nLogEx::debug);

	int err;
	size_t	size;
	BOOL	bAddTrend = FALSE;

	deque<CSpaceData>& tempData = theApp.GetDataCollection().GetSpaceTemp();

	switch (pbuf->header.command)
	{
	case ePipeCommand_monitorget:
	{
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("ePipeCommand_monitorget"), _T("Begin"), nLogEx::debug);

		DWORD	dwStart = timeGetTime();
#if 0
		UINT nSelectStationMode = theApp.GetSelectStationMode();
		UINT nSelectStation = theApp.GetSelectStation();
		if (nSelectStation != (m_nStation - 1))
			return;
#endif

		// 監視生データ
		// bufをAppのdequeに登録
#if 0
		// 1局モードの場合は指定局、4局モードの場合は局番号が範囲内の場合
		if ((nSelectStationMode == eStation_1 &&
			((nSelectStation == 0 && pbuf->header.station == 1) ||
			(nSelectStation == 1 && pbuf->header.station == 4) ||
			(nSelectStation == 2 && pbuf->header.station == 2) ||
			(nSelectStation == 3 && pbuf->header.station == 3))) ||
			(nSelectStationMode == eStation_5 && pbuf->header.station >= eStation_Usuda64 + 1 && pbuf->header.station <= eStation_MAX))
#endif
		{
			UINT nStation = 0;
			if (pbuf->header.station == 1) nStation = 0;
			if (pbuf->header.station == 2) nStation = 2;
			if (pbuf->header.station == 3) nStation = 3;
			if (pbuf->header.station == 4) nStation = 1;
			// 自分と同じ局番号のデータのみを扱う
			if (nStation != (m_nStation - 1))
				return;
			if (CDataCollection::GetEngval(nStation) != NULL){
				err = CDataCollection::GetEngval(nStation)->CalcFromMsg((CEngValCnv::sdtp_msg_ex_t*)&(pbuf->buf[0]));
				if (err == 0){
					vector<CEngValCnv::obs_tbl_res_t>& data = CDataCollection::GetEngval(nStation)->GetAllVal();
					size = data.size();
					if (size > 0)
					{
						if (CDataCollection::GetEngval(nStation)->mOutDataList.size() != 0)
						{
							// 衛星データキューに登録
							theApp.GetDataCollection().SetEqMonData(nStation, data);
							CGraphMutex::Lock((eMutex)(eMessage));
							theApp.GetDataCollection().ConvertTrendData(nStation, data);
							CGraphMutex::Unlock((eMutex)(eMessage));
							bAddTrend = TRUE;
						}
						else
						{
							// 流れている監視名を表示(デバッグ用)
							if (theApp.GetLogLevel() == nLogEx::debug)
							{
								if (GetPrivateProfileInt(SECTION_SYSTEM, KEY_DATANAMEOUT, 0, theApp.GetAppInifileName()) != 0)
								{
									for (UINT i = 0; i < data.size(); i++)
									{
										strDebug.Format(_T("%s.%s"), mStationSimpleString[nStation], (LPCTSTR)CString(data[i].obs_adr.sz_obsname));
										CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("data obsname"), strDebug, nLogEx::debug);
									}
								}
							}
						}
					}
					else{
						strDebug.Format(_T("%s(%d) :: CalcFromMsg no size(%d)"), __FILEW__, __LINE__, (int)size);
						CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("debug"), strDebug, nLogEx::debug);
					}
				}
				else{
					strDebug.Format(_T("%s(%d) :: CalcFromMsg err(%d)"), __FILEW__, __LINE__, err);
					CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("debug"), strDebug, nLogEx::debug);
				}
			}
			else
			{
				strDebug.Format(_T("%s(%d) :: mpTrendEng[%d] null"), __FILEW__, __LINE__, nStation);
				CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("debug"), strDebug, nLogEx::debug);
			}
		}
		//		DWORD dwElapsed = (timeGetTime() - dwStart);
		//		TRACE("### ePipeCommand_monitorget Elapsed(%d)\n", dwElapsed);
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("ePipeCommand_monitorget"), _T("End"), nLogEx::debug);
	}
	break;
	default:
		break;
	}
	//::PostMessage(mPipeMessageWnd->m_hWnd, ID_PIPE_SERVER, 9, (LPARAM)pbuf);

	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("End"), _T(""), nLogEx::debug);
}
