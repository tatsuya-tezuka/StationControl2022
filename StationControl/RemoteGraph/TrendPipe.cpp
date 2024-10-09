// TrendPipe.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RemoteGraph.h"
#include "TrendPipe.h"


// CTrendPipe

/*============================================================================*/
/*! プロセス間通信：パイプクラス

-# コンストラクタ

@param

@retval
*/
/*============================================================================*/
CTrendPipe::CTrendPipe()
{
	m_pParam = NULL;
	m_strPipeName = _T("");
	m_strFullPipeName = _T("\\\\.\\PIPE\\");

	m_hOutPipe = NULL;
	m_hInPipe = NULL;
	m_hListner = NULL;
	mPipeCount = 1;
}

/*============================================================================*/
/*! プロセス間通信：パイプクラス

-# デストラクタ

@param

@retval
*/
/*============================================================================*/
CTrendPipe::~CTrendPipe()
{
	Delete();
}

/*============================================================================*/
/*! プロセス間通信：パイプクラス

-# パイプの作成

@param	bServer		trueの場合サーバー、falseの場合クライアント
@param	fCallBack	取得スレッドファンクション
@param	type		1固定
@param	ptr			呼び出し元

@retval
*/
/*============================================================================*/
bool CTrendPipe::Create(bool bServer, void (WINAPI *fCallBack)(PIPEMONITORACK* pbuf), UINT type, DWORD threadNotify)
{
	m_bServer = bServer;
	if (fCallBack == NULL)
		return false;

	if (bServer){
		m_hOutPipe = CreateNamedPipe(GetRealPipeName(true),
			PIPE_ACCESS_OUTBOUND,
			PIPE_WAIT,
			1,
			sizeof(PIPEACK),
			sizeof(PIPEACK),
			mPipeTimeout,
			NULL);
		if (m_hOutPipe == NULL || m_hOutPipe == INVALID_HANDLE_VALUE)
			return false;
		m_hInPipe = CreateNamedPipe(GetRealPipeName(false),
			PIPE_ACCESS_INBOUND,
			PIPE_WAIT,
			1,
			sizeof(PIPEACK),
			sizeof(PIPEACK),
			mPipeTimeout,
			NULL);
		if (m_hInPipe == NULL || m_hInPipe == INVALID_HANDLE_VALUE)
			return false;
	}
	else{
		m_hOutPipe = CreateFile(GetRealPipeName(true),
			GENERIC_WRITE,
			0, // No Share
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (m_hOutPipe == NULL || m_hOutPipe == INVALID_HANDLE_VALUE)
			return false;
		m_hInPipe = CreateFile(GetRealPipeName(false),
			GENERIC_READ,
			0, // No Share
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (m_hInPipe == NULL || m_hInPipe == INVALID_HANDLE_VALUE)
			return false;
	}

	// スレッドの作成
	m_pParam = new ListnerParam;
	m_pParam->funcCallBack = fCallBack;
	m_pParam->hPipe = m_hInPipe;
	m_pParam->type = type;
	m_pParam->threadid = threadNotify;
	m_pParam->thread = NULL;
	m_hListner = CreateThread(NULL,
								0,
								&ListnerProc,
								(LPVOID)m_pParam,
								0,
								&m_dwListnerThreadId);
	if (m_hListner == NULL || m_hListner == INVALID_HANDLE_VALUE)
		return false;

	m_pParam->thread = m_hListner;

	return true;
}

static bool mListnerStop[eStation_MAX] = {false,false,false,false};

/*============================================================================*/
/*! プロセス間通信：パイプクラス

-# パイプの削除

@param

@retval
*/
/*============================================================================*/
bool CTrendPipe::Delete()
{
//	TRACE("CTrendPipe::Delete Start\n");

	DWORD dwTemp;
//	TRACE("CTrendPipe::Delete GetExitCodeThread Start\n");
	if (GetExitCodeThread(m_hListner, &dwTemp)){
//		TRACE("CTrendPipe::Delete GetExitCodeThread End\n");
		if (dwTemp == STILL_ACTIVE){
			//TerminateThread(m_hListner, 3);
			//DisconnectNamedPipe(m_hInPipe);
		}
//		TRACE("CTrendPipe::Delete GetExitCodeThread TerminateThread\n");
	}
	if (m_hListner != NULL){
		mListnerStop[m_pParam->type] = true;
		WaitForSingleObject(m_hListner, 10000);
		mListnerStop[m_pParam->type] = false;
	}
	if (m_hListner != NULL/* && m_hListner != INVALID_HANDLE_VALUE*/){
//		TRACE("CTrendPipe::Delete CloseHandle(m_hListner)\n");
		CloseHandle(m_hListner);
		m_hListner = NULL;
	}

	if (m_hOutPipe != NULL && m_hOutPipe != INVALID_HANDLE_VALUE){
//		TRACE("CTrendPipe::Delete CloseHandle(m_hOutPipe)\n");
		CloseHandle(m_hOutPipe);
		m_hOutPipe = NULL;
	}
	if (m_hInPipe != NULL && m_hInPipe != INVALID_HANDLE_VALUE){
//		TRACE("CTrendPipe::Delete CloseHandle(m_hInPipe)\n");
		CloseHandle(m_hInPipe);
		m_hInPipe = NULL;
	}

	if (m_pParam != NULL){
//		TRACE("CTrendPipe::Delete delete\n");
		delete m_pParam;
		m_pParam = NULL;
	}

//	TRACE("CTrendPipe::Delete End\n");

	return true;
}
/*============================================================================*/
/*! プロセス間通信：パイプクラス

-# スレッドファンクション

@param	lParam		スレッドパラメタ

@retval
*/
/*============================================================================*/
DWORD WINAPI CTrendPipe::ListnerProc(LPVOID lParam)
{
	ListnerParam* pParam = (ListnerParam*)lParam;
	if (pParam == NULL)
		return 1;

	bool	bConnect = false;

	DWORD dwRetVal = 0;
	DWORD dwRead;
	BOOL ret;

	for (bool bContinue = true; bContinue && dwRetVal == 0;/* Sleep(5)*/){
		PIPEMONITORACK buf;
		ret = ReadFile(pParam->hPipe, &buf, sizeof(PIPEMONITORACK), &dwRead, NULL);
//		TRACE("##size=%d\n", dwRead);
		
		// 終了コマンドに対する応答
		if (buf.header.command == ePipeCommand_stop)
			break;
		if (mListnerStop[pParam->type] == true)
			break;

		if (dwRead == 0){
			if (bConnect == true){
				buf.header.command = ePipeCommand_finish;
				buf.header.station = pParam->threadid;
				pParam->funcCallBack(&buf);
				break;
			}
			continue;
		}
		if (!ret)
			dwRetVal = 2;

		bConnect = true;

		pParam->funcCallBack(&buf);
	}

	return dwRetVal;
}


/*============================================================================*/
/*! プロセス間通信：パイプクラス

-# パイプ名称の設定

@param	lParam		スレッドパラメタ

@retval
*/
/*============================================================================*/
void CTrendPipe::SetPipeName(CString strName, CString strHost)
{
	m_strPipeName = strName;
	m_strFullPipeName = _T("\\\\");
	m_strFullPipeName += strHost;
	m_strFullPipeName += "\\PIPE\\";
	m_strFullPipeName += m_strPipeName;
}
/*============================================================================*/
/*! プロセス間通信：パイプクラス

-# パイプ名称の取得

@param	lParam		スレッドパラメタ

@retval
*/
/*============================================================================*/
CString CTrendPipe::GetRealPipeName(bool bServer)
{
	CString strRetVal = m_strFullPipeName;
	strRetVal += (bServer == true) ? _T("_IN") : _T("_OUT");
	return strRetVal;
}
/*============================================================================*/
/*! プロセス間通信：パイプクラス

-# コマンド送信

@param	lParam		strMsg	送信文字列

@retval
*/
/*============================================================================*/
bool CTrendPipe::Send(LPPIPEBUFFER pbuf)
{
	DWORD dw;
	if (m_hOutPipe == NULL)
		return false;

	int len = sizeof(DWORD) + sizeof(DWORD) + sizeof(WPARAM) + sizeof(LPARAM);
	if (pbuf->bufsize == 0)
		len = sizeof(PIPEBUFFER);
	else
		len += sizeof(UINT) + (int)pbuf->bufsize;

	BOOL ret = WriteFile(m_hOutPipe, pbuf, len/*sizeof(PIPEBUFFER)*/, &dw, NULL);

	return true;
}

bool CTrendPipe::Send(PIPESTART* pbuf)
{
	DWORD dw;
	if (m_hOutPipe == NULL)
		return false;

	pbuf->header.matching = mPipeCount++;
	BOOL ret = WriteFile(m_hOutPipe, pbuf, sizeof(PIPESTART), &dw, NULL);

	return (ret == TRUE) ? true : false;
}

bool CTrendPipe::Send(PIPESTOP* pbuf)
{
	DWORD dw;
	if (m_hOutPipe == NULL)
		return false;

	// 終了通知
	if (pbuf->header.command == ePipeCommand_stop){
		mListnerStop[m_pParam->type] = true;
	}

	pbuf->header.matching = mPipeCount++;
	BOOL ret = WriteFile(m_hOutPipe, pbuf, sizeof(PIPESTOP), &dw, NULL);

	return (ret == TRUE) ? true : false;
}
