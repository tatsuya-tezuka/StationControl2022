#include "stdafx.h"
#include "ShareLbwIf.h"

static const CString mShareMemoryString = _T("LbwShareMemory");
static const CString mShareMemoryMutex = _T("LbwShareMutex");
static const CString mShareMemorySend = _T("LbwShareSend");
static const CString mShareMemoryRecv = _T("LbwShareRecv");


CShareLbwIf::CShareLbwIf()
{
	mServer = true;
	m_hMap = NULL;
	m_hMutex = NULL;
	m_hSend = NULL;
	m_hRecv = NULL;
}


CShareLbwIf::~CShareLbwIf()
{
	Delete();
}

/*============================================================================*/
/*! 共有メモリ（LoopBand）

-# 共有メモリの作成

@param	bServer		trueの場合サーバー、falseの場合クライアント

@retval
*/
/*============================================================================*/
bool CShareLbwIf::Create(bool bServer)
{
	if (m_hMap != NULL && mServer == bServer){
		return true;
	}

	if (m_hMap != NULL){
		Delete();
	}

	mServer = bServer;

	// プロセスIDの取得
	m_dwProcessId = ::GetCurrentProcessId();

	// 共有メモリ用ファイルマッピングオブジェクトの作成
	m_hMap = ::CreateFileMapping((HANDLE)INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(stItem)*mItemMax, mShareMemoryString);

	if (mServer == true){
		if (GetLastError() == ERROR_ALREADY_EXISTS){
			//AfxMessageBox(_T("既にアラームサーバーとして起動されています。"));
			return false;
		}
	}
	else{
		if (GetLastError() == ERROR_ALREADY_EXISTS){
			// 既に共有メモリオブジェクトが作成されているので、クライアントとして実行
			m_hMap = ::OpenFileMapping(FILE_MAP_WRITE, FALSE, mShareMemoryString);
//			m_hMutex = ::CreateMutex(NULL, FALSE, mShareMemoryMutex);
		}
		else{
			//AfxMessageBox(_T("アラームサーバーが起動されていません。\n処理を中止します"));
			return false;
		}
	}

	// ここまでくればサーバーあるいはクライアントして起動されている

	// 
	// 呼び出し側プロセスのアドレス空間に、ファイルのビューをマップします
	if (m_hMap != NULL)
		m_pBuf = (char*)::MapViewOfFile(m_hMap, FILE_MAP_WRITE, 0, 0, sizeof(stItem)*mItemMax);
	else
		m_pBuf = NULL;

	m_hSend = ::CreateEvent(NULL, FALSE, FALSE, mShareMemorySend);
	m_hRecv = ::CreateEvent(NULL, FALSE, FALSE, mShareMemoryRecv);

	return true;
}

/*============================================================================*/
/*! 共有メモリ（LoopBand）

-# アラーム共有メモリの削除

@param	bServer		trueの場合サーバー、falseの場合クライアント

@retval
*/
/*============================================================================*/
void CShareLbwIf::Delete()
{
	::CloseHandle(m_hSend);
	::CloseHandle(m_hRecv);
	if (m_hMutex)
		::CloseHandle(m_hMutex);

	::UnmapViewOfFile(m_pBuf);
	::CloseHandle(m_hMap);

	m_hMap = NULL;
	m_hMutex = NULL;
	m_hSend = NULL;
	m_hRecv = NULL;
}

/*============================================================================*/
/*! 共有メモリ（LoopBand）

-# アラーム共有メモリへの書き込み

@param		stAlarmBuf		共有メモリに書き込むデータ
@param		count			stItem構造体のカウント

@retval
*/
/*============================================================================*/
void CShareLbwIf::Send(stItem* stBuf, UINT count)
{
	if (m_pBuf == NULL)
		return;

	// サーバーとの排他制御
//	if (::WaitForSingleObject(m_hMutex, 500) == WAIT_OBJECT_0)
	{
		// 共有メモリバッファに書き込む
		memset(m_pBuf, 0, sizeof(stItem)*mItemMax);
		memcpy(m_pBuf, stBuf, sizeof(stItem)*count);

		// 書き込み終了イベント
		::SetEvent(m_hSend);
		TRACE("### Client Send Finish\n");

		// Wait for server to finish
		if (::WaitForSingleObject(m_hRecv, 500) == WAIT_OBJECT_0) {
			TRACE("### Server Receive Finish\n");
		}
		else {
			TRACE("### Server Receive Timeout\n");
		}
//		::ReleaseMutex(m_hMutex);
	}
}

/*============================================================================*/
/*! 共有メモリ（LoopBand）

-# アラーム共有メモリからの読み込み（受信は全領域を取得する必要がある）

@param		stAlarmBuf		共有メモリに書き込むデータ（最大mItemMax数分）
@param		count			stItem構造体のカウント

@retval
*/
/*============================================================================*/
bool CShareLbwIf::Recv(stItem* stBuf, UINT count)
{
	if (m_pBuf == NULL)
		return false;

	if (::WaitForSingleObject(m_hSend, 50) == WAIT_OBJECT_0) {
		// 共有メモリバッファから読み込む
		memset(stBuf, 0, sizeof(stItem)*count);
		memcpy(stBuf, &m_pBuf[0], sizeof(stItem)*count);
		::SetEvent(m_hRecv);
		TRACE("### Client Receive Finish\n");
		return true;
	}
	else{
		TRACE("### Client Send Timeout\n");
	}
	return false;
}



