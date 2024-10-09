// CommBase.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CommBase.h"

// timeGetTime使用
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

// CCommBase

CCommBase::CCommBase()
{
	mThreadStop = false;
}

CCommBase::~CCommBase()
{
}

/*============================================================================*/
/*! サーバー/クライアント

-# ソケットサーバーの作成

@param

@retval

*/
/*============================================================================*/
int CCommBase::WaitSelect(SOCKET fd, long lMilliSec)
{
	timeval timeout_sec;
	timeout_sec.tv_sec = (long)lMilliSec / 1000;				//秒
	timeout_sec.tv_usec = ((long)lMilliSec % 1000) * 1000;	//マイクロ秒

	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);


	int iSock = select((int)fd + 1, &rfds, NULL, NULL, &timeout_sec);
	TRACE("WaitSelect : Error. errno = %d\n", GetLastError());
	if (iSock == 0)    //Timeout
	{
		TRACE("WaitSelect : Timeout\n");
		return -1;
	}
	else if (iSock < 0)
	{
		//待ち異常
		TRACE("WaitSelect : Error\n");
		return -2;
	}

	return (int)iSock;

}

/*============================================================================*/
/*! サーバー/クライアント

-# データ送信

@param		fd			ソケット
@param		pcDataBuf	送信データ
@param		uiDataSize	送信サイズ

@retval

*/
/*============================================================================*/
bool CCommBase::SendData(SOCKET fd, char* pcDataBuf, unsigned int uiDataSize)
{
	if (fd == INVALID_SOCKET) {
		TRACE("SendData : ConnectFd is INVALID_SOCKET\n");
		return false;
	}

	unsigned int uiSndSz = 0;
	unsigned int uiSndSum = 0;
	while (uiSndSum < uiDataSize) {
		uiSndSz = send(fd, pcDataBuf + uiSndSum, uiDataSize - uiSndSum, 0);
		if (uiSndSz == SOCKET_ERROR) {
			TRACE("SendData : Error. errno = %d\n", GetLastError());
			return false;
		}

		uiSndSum += uiSndSz;
	}

	return true;
}

/*============================================================================*/
/*! サーバー/クライアント

-# データ受信

@param		fd			ソケット
@param		pcDataBuf	受信データ
@param		puiSize		受信サイズ
@param		lTimeOut	タイムアウト

@retval

*/
/*============================================================================*/
bool CCommBase::RecvData(SOCKET fd, char* pcDataBuf, unsigned int* puiSize, long lTimeOut)
{
	int iRet;

	clock_t start, end;			//受信開始／完了時刻
	long lTimeLimit = lTimeOut;	//受信の残り時間

	if (fd == INVALID_SOCKET) {
		TRACE("RecvData : ConnectFd is INVALID_SOCKET\n");
		*puiSize = 0;
		return false;
	}

	//受信
	unsigned int uiRcvSz = 0;
	unsigned int uiRevdSz = 0;
	memset(pcDataBuf, 0, *puiSize);

	while (uiRevdSz < *puiSize) {

		if (lTimeLimit <= 0) {
			return false;
		}

		start = clock();

		iRet = WaitSelect(fd, lTimeLimit);
		if (iRet == -1) {	//Timeout
			return false;
		}
		else if (iRet == -2) {
			return false;
		}

		// データ受信
		uiRcvSz = recv(fd, pcDataBuf + uiRevdSz, (*puiSize) - uiRevdSz, 0);
		if (uiRcvSz == SOCKET_ERROR) {	//エラーが発生
			TRACE("RecvData : Error. errno = %d\n", GetLastError());
			*puiSize = uiRevdSz;
			*puiSize = 0;
			return false;
		}
		else if (uiRcvSz == 0) {		//ソケットが切断された 
			*puiSize = uiRevdSz;
			return false;	//connection over
		}

		uiRevdSz += uiRcvSz;

		end = clock();
		lTimeLimit -= (end - start) / (CLOCKS_PER_SEC / 1000);
	}

	*puiSize = uiRevdSz;

	return true;
}

/*============================================================================*/
/*! サーバー/クライアント

-# Unix日付からWindows日付への変換

@param		unixTime	Unix時間
@param		winTime		Windows時間

@retval

*/
/*============================================================================*/
void CCommBase::ConvertTime(const stServerTime* unixTime, SYSTEMTIME* winTime)
{
	CTime time(unixTime->wYear, 1, 1, 0, 0, 0);
	CTimeSpan ts(unixTime->wDay - 1/*0=1月1日なので*/, 0, 0, 0);
	CTime at = time + ts;
	//CString s = at.Format(_T("%Y-%m-%d"));
	at.GetAsSystemTime(*winTime);
	winTime->wHour = unixTime->wHour;
	winTime->wMinute = unixTime->wMinute;
	winTime->wSecond = unixTime->wSecond;
	winTime->wMilliseconds = unixTime->wMilliseconds;
}
/*============================================================================*/
/*! サーバー/クライアント

-# Windows日付からUnix日付への変換

@param		winTime		Windows時間
@param		unixTime	Unix時間

@retval

*/
/*============================================================================*/
void CCommBase::ConvertTime(const SYSTEMTIME* winTime, stServerTime* unixTime)
{
	CTime time1(winTime->wYear, 1, 1, 0, 0, 0);
	CTime time2(*winTime);
	CTimeSpan ts = time2 - time1;
	LONGLONG days = ts.GetDays() + 1;
	unixTime->wYear = winTime->wYear;
	unixTime->wDay = (WORD)days;
	unixTime->wHour = winTime->wHour;
	unixTime->wMinute = winTime->wMinute;
	unixTime->wSecond = winTime->wSecond;
	unixTime->wMilliseconds = winTime->wMilliseconds;
}

/*============================================================================*/
/*! サーバー/クライアント

-# ロカールアドレスの取得

@param		strAddress	格納バッファ
@param		nSize		格納バッファサイズ

@retval

*/
/*============================================================================*/
bool CCommBase::GetLocalAddress(char* strAddress, UINT nSize)
{
	if (strAddress != NULL && nSize > 0){
		char strHost[mMaxHostName] = { 0 };

		// ホスト名の取得
		if (SOCKET_ERROR != gethostname(strHost, sizeof(strHost))){
			struct addrinfo hints;
			struct addrinfo *result;
			struct in_addr addr;
			memset(&hints, 0, sizeof(hints));
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_family = AF_INET;
			if (getaddrinfo(strHost, NULL, &hints, &result) == 0){
				addr.s_addr = ((struct sockaddr_in *)(result->ai_addr))->sin_addr.s_addr;

				sprintf_s(strHost, mMaxHostName, "%u.%u.%u.%u",
					(UINT)((PBYTE)addr.S_un.S_un_b.s_b1),
					(UINT)((PBYTE)addr.S_un.S_un_b.s_b2),
					(UINT)((PBYTE)addr.S_un.S_un_b.s_b3),
					(UINT)((PBYTE)addr.S_un.S_un_b.s_b4));

				if (strlen(strHost) > nSize){
					SetLastError(ERROR_INSUFFICIENT_BUFFER);
					return false;
				}
				sprintf_s(strAddress, mMaxHostName, "%s", strHost);
				return true;
			}
		}
	}
	else{
		SetLastError(ERROR_INVALID_PARAMETER);
	}
	return false;
}
/*============================================================================*/
/*! サーバー/クライアント

-# ロカール名称の取得

@param		strName		格納バッファ
@param		nSize		格納バッファサイズ

@retval

*/
/*============================================================================*/
bool CCommBase::GetLocalName(char* strName, UINT nSize)
{
	if (strName != NULL && nSize > 0){
		char strHost[mMaxHostName] = { 0 };

		// ホスト名称の取得
		if (SOCKET_ERROR != gethostname(strHost, sizeof(strHost))){

			if (strlen(strHost) > nSize){
				SetLastError(ERROR_INSUFFICIENT_BUFFER);
				return false;
			}

			sprintf_s(strName, mMaxHostName, "%s", strHost);
			return true;
		}
	}
	else{
		SetLastError(ERROR_INVALID_PARAMETER);
	}

	return false;
}
/*============================================================================*/
/*! サーバー/クライアント

-# ポート番号の取得

@param		strServiceName	格納バッファ

@retval

*/
/*============================================================================*/
USHORT CCommBase::GetPortNumber(char* strServiceName)
{
	LPSERVENT   lpservent;
	USHORT      nPortNumber = 0;

	if (_istdigit(strServiceName[0])) {
		nPortNumber = (USHORT)atoi(strServiceName);
	}
	else {
		char* pstrService = strServiceName;
		if ((lpservent = getservbyname(pstrService, NULL)) != NULL)
			nPortNumber = ntohs(lpservent->s_port);
	}

	return nPortNumber;
}

/*============================================================================*/
/*! サーバー/クライアント

-# IPアドレスの取得

@param		strServiceName	格納バッファ

@retval

*/
/*============================================================================*/
ULONG CCommBase::GetIPAddress(char* strHostName)
{
	ULONG	uAddr = INADDR_NONE;
	char	strLocal[mMaxHostName] = { 0 };

	if (strHostName == NULL){
		GetLocalName(strLocal, sizeof(strLocal));
		strHostName = strLocal;
	}

	char* strHost = strHostName;

	struct addrinfo hints;
	struct addrinfo *result;
	struct in_addr addr;
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;
	if (getaddrinfo(strHost, NULL, &hints, &result) == 0){
		addr.s_addr = ((struct sockaddr_in *)(result->ai_addr))->sin_addr.s_addr;
		uAddr = addr.s_addr;
	}
	else{
		uAddr = INADDR_NONE;
	}

	return ntohl(uAddr);
}



// CCommServer

CCommServer::CCommServer()
	: mSock(INVALID_SOCKET)
{
	mMessageWnd = NULL;
}

CCommServer::~CCommServer()
{
	Release();
}

/*============================================================================*/
/*! サーバー/クライアント

-# クライアントの接続を待機するスレッド

@param

@retval

*/
/*============================================================================*/
UINT WINAPI CCommServer::WaitClientThread(LPVOID pParam)
{
	CCommServer* pThis = reinterpret_cast<CCommServer*>(pParam);
	_ASSERTE(pThis != NULL);

	pThis->Wait();

	return 1;
}

/*============================================================================*/
/*! サーバー/クライアント

-# クライアント待機処理

@param

@retval

*/
/*============================================================================*/
void CCommServer::Wait()
{
	// クライアントの接続を待機
	ULONG ipClient;
	while (GetThreadStop() == false){
		ipClient = WaitAccept(100000);
		if (ipClient == 0){
			return;
		}

		CADDRLIST::iterator itr;
		itr = mClientSockAddrList.find(ipClient);
		if (itr != mClientSockAddrList.end()){
			// ログメッセージ送信
			if (mMessageWnd){
				mMessageWnd->SendMessage(eMessage_ClientConnection, 0, (LPARAM)ipClient);
			}
		}

		Clear();

		// クライアント通信用スレッドの作成
		UINT uiThreadId = GetThreadId(ipClient);
		if (uiThreadId == 0){
			HANDLE thread = (HANDLE)_beginthreadex(NULL,				// SECURITY_ATTRIBUTES 構造体へのポインタ
				0,					// 新しいスレッドのスタック サイズ
				ClientCommThread,   // 新しいスレッドの実行を開始するルーチンの開始アドレス
				this,				// スレッドパラメタ
				CREATE_SUSPENDED,	// 初期状態フラグ
				&uiThreadId);		// スレッド識別子を受け取る 32 ビット変数へのポインタ

			if (thread != NULL){
				mThreadList.insert(pair<ULONG, DWORD>(ipClient, (DWORD)uiThreadId));
				ResumeThread(thread);
			}
		}
	}
}

/*============================================================================*/
/*! サーバー/クライアント

-# クライアントの接続を待機するスレッド

@param

@retval

*/
/*============================================================================*/
UINT WINAPI CCommServer::ClientCommThread(LPVOID pParam)
{
	CCommServer* pThis = reinterpret_cast<CCommServer*>(pParam);
	_ASSERTE(pThis != NULL);

	DWORD dw = GetCurrentThreadId();
	pThis->Client(dw);

	return 1;
}

/*============================================================================*/
/*! サーバー/クライアント

-# クライアント待機処理

@param

@retval

*/
/*============================================================================*/
void CCommServer::Client(DWORD threadid)
{
	DWORD   dwBytes = 0L;
	DWORD   dwTimeout = INFINITE;

	ULONG ipClient = GetThreadClient(threadid);
	if (ipClient == 0)
		return;

	CADDRLIST::iterator itr;
	itr = mClientSockAddrList.find(ipClient);

	BYTE bCommand = 0;

	while (GetThreadStop() == false){
		stCommandHeader header;
		UINT size = sizeof(header);

		// 受信待ち
		CCommBase::RecvData((*itr).second.sockClient, (char*)&header, &size, CCommBase::mDefaultCommTimeout);
		if (size == 0){
			// Error?
			TRACE("Client : RecvData Size Zero\n");
			break;
		}
		if (size != sizeof(header)){
			TRACE("Client : RecvData Size Error\n");
			continue;
		}
		if (header.size == 0){
			TRACE("Client : Header Zero\n");
			continue;
		}

		// ログメッセージ送信
		if (mMessageWnd && bCommand != header.command){
			mMessageWnd->PostMessage(eMessage_ClientCommand, (WPARAM)header.command, (LPARAM)ipClient);
		}

		// ここでコマンド毎の処理を行う
		bool ret = true;
		switch (header.command){
		case	eCommand_ReqSignal:
			ret = RequestSignal(ipClient, (*itr).second.sockClient, header);
			break;
		case	eCommand_ReqStart:
			ret = RequestSignalStart(ipClient, (*itr).second.sockClient, header);
			break;
		case	eCommand_ReqStop:
			ret = RequestSignalStop(ipClient, (*itr).second.sockClient, header);
			break;
		case	eCommand_ReqDataList:
			ret = RequestDataList((*itr).second.sockClient, header);
			break;
		case	eCommand_ReqGetData:
			ret = RequestGetDataSignal(ipClient, (*itr).second.sockClient, header);
			break;
		case	eCommand_SendData:
			break;
		default:
			break;
		}
		if (ret == false)
			break;
		bCommand = header.command;
	}

	// クライアント終了
	// ログメッセージ送信
	if (mMessageWnd){
		mMessageWnd->SendMessage(eMessage_ClientDisConnection, 0, (LPARAM)ipClient);
	}
	ResetThreadId(ipClient);
}

/*============================================================================*/
/*! サーバー/クライアント

-# 監視データ登録要求

@param

@retval

*/
/*============================================================================*/
bool CCommServer::RequestSignal(ULONG ipClient, SOCKET sockClient, stCommandHeader& header)
{
	TRACE("Request Singnal Data Add(0x%02x)\n", header.command);
	map< ULONG, vector<stReqSignalName> >& datalist = GetSignalList();
	map< ULONG, vector<stReqSignalName> >::iterator itr;

	itr = datalist.find(ipClient);
	if (itr == datalist.end()){
		// クライアント毎の登録データリストが存在しない
		vector<stReqSignalName> data;
		data.clear();
		datalist.insert(pair< ULONG, vector<stReqSignalName> >(ipClient, data));
	}
	itr = datalist.find(ipClient);
	(*itr).second.clear();

	// 監視データ登録要求のデータ部を読み込む
	// 登録監視データ数の取得
	UINT count = (header.size - sizeof(stCommandHeader)) / _MAX_NAME;
	bool ret = true;
	for (UINT i = 0; i < count; i++){
		stReqSignalName name;
		UINT size = _MAX_NAME;
		CCommBase::RecvData(sockClient, (char*)&name.name, &size, CCommBase::mDefaultCommTimeout);
		if (size == 0){
			ret = false;
			break;
		}
		(*itr).second.push_back(name);
	}

	stAckNack ack;
	memset(&ack, 0, sizeof(stAckNack));
	ack.command = header.command;
	ack.status = (ret==true)?0x00:0x01;
	ack.size = sizeof(stAckNack);

	// ヘッダー部送信
	CCommBase::SendData(sockClient, (char*)&ack, sizeof(stAckNack));

	return true;
}

/*============================================================================*/
/*! サーバー/クライアント

-# クライアントの接続を待機するスレッド

@param

@retval

*/
/*============================================================================*/
UINT WINAPI CCommServer::ClientDataThread(LPVOID pParam)
{
	CCommServer* pThis = reinterpret_cast<CCommServer*>(pParam);
	_ASSERTE(pThis != NULL);


	map<UINT, stThreadData>::iterator itr;
	map<UINT, stThreadData>& tdata = pThis->GetThreadData();

	UINT id = (UINT)GetCurrentThreadId();

	itr = tdata.find(id);
	if (itr == tdata.end())
		return 0;

	// 最小タイマ分解能を設定(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();

	int angle[80];
	for (int i = 0; i < sizeof(angle) / sizeof(angle[0]); ++i) {
		angle[i] = rand() * 360 / RAND_MAX;
	}

	map< ULONG, vector<stReqSignalName> >& datalist = pThis->GetSignalList();
	map< ULONG, vector<stReqSignalName> >::iterator itrs;
	itrs = datalist.find((*itr).second.ipClient);
	if (itrs == datalist.end()){
		return 0;
	}

	UINT count = (UINT)(*itrs).second.size();
	//TRACE("# Data Count=%d\n", count);

	while ((*itr).second.start == TRUE){
		DWORD dwElapsed = (timeGetTime() - dwStart);
		if (dwElapsed >= 1000){
			dwStart = timeGetTime();
		} else{
			Sleep(1);
			continue;
		}

		if ((*itr).second.requestData == FALSE){
			// データ要求がないので何もしない
			Sleep(1);
			continue;
		}
		(*itr).second.requestData = FALSE;

		for (int i = 0; i < sizeof(angle) / sizeof(angle[0]); ++i) {
			angle[i] = (angle[i] + 2 * (i + 1)) % 360;
		}


		// データをクライアントに送る
		BYTE	buf[CCommBase::mMaxBufferSize];
		LPBYTE  lpData = buf;
		DWORD   dwBytes = 0;

		// ヘッダー部送信
		stAckNack	ack;
		memset(&ack, 0, sizeof(stAckNack));
		ack.command = eCommand_ReqGetData;
		ack.size = (count * sizeof(stServerItemData)) + sizeof(stAckNack);
		memcpy(lpData, (char*)&ack, sizeof(stAckNack));
		lpData += sizeof(stAckNack);
		dwBytes += sizeof(stAckNack);
		//CCommBase::SendData((*itr).second.sockClient, (char*)&header, sizeof(header));
		//TRACE("# Send Header Sizet=%d\n", header.size);

		// データ部送信
		SYSTEMTIME time;
		GetLocalTime(&time);
		for (UINT i = 0; i < count; i++){
			stServerItemData data;
			memset(&data, 0, sizeof(stServerItemData));
			CCommBase::ConvertTime(&time, &data.systime);
			data.kind = 0;
			data.value = (double)((sin(angle[i] * 3.1415926538 / 180.0) + 1) / 2 * (100 - 0) + 0);
			memcpy(data.name, (*itrs).second[i].name, _MAX_NAME);
			memcpy(lpData, (char*)&data, sizeof(stServerItemData));
			lpData += sizeof(stServerItemData);
			dwBytes += sizeof(stServerItemData);
			//CCommBase::SendData((*itr).second.sockClient, (char*)&data, sizeof(stServerItemData));
			//TRACE("# Send Time=%d\n", data.systime.wYear);
		}

		// データをクライアントに送信する
		pThis->SendGetDataSignal((*itr).second.sockClient, buf, dwBytes);
	}

	timeEndPeriod(1);

	return 1;
}

/*============================================================================*/
/*! サーバー/クライアント

-# 監視データ送信開始要求

@param

@retval

*/
/*============================================================================*/
bool CCommServer::RequestSignalStart(ULONG ipClient, SOCKET sockClient, stCommandHeader& header)
{
	TRACE("Request Singnal Data Start(0x%02x)\n", header.command);
	stAckNack ack;
	memset(&ack, 0, sizeof(stAckNack));
	ack.command = header.command;
	ack.status = 0x00;
	ack.size = sizeof(stAckNack);

	// ヘッダー部送信
	CCommBase::SendData(sockClient, (char*)&ack, sizeof(stAckNack));

	// クライアントへのデータ送信用スレッドの作成
	UINT uiThreadId;
	HANDLE thread = (HANDLE)_beginthreadex(NULL,				// SECURITY_ATTRIBUTES 構造体へのポインタ
		0,					// 新しいスレッドのスタック サイズ
		ClientDataThread,   // 新しいスレッドの実行を開始するルーチンの開始アドレス
		this,				// スレッドパラメタ
		CREATE_SUSPENDED,	// 初期状態フラグ
		&uiThreadId);		// スレッド識別子を受け取る 32 ビット変数へのポインタ
	if (thread != NULL){
		stThreadData data;
		data.start = TRUE;
		data.thread = thread;
		data.ipClient = ipClient;
		data.sockClient = sockClient;
		mThreadClientData.insert(pair<UINT, stThreadData>(uiThreadId, data));
		ResumeThread(thread);
	}

	return true;
}

/*============================================================================*/
/*! サーバー/クライアント

-# 監視データ送信終了要求

@param

@retval

*/
/*============================================================================*/
bool CCommServer::RequestSignalStop(ULONG ipClient, SOCKET sockClient, stCommandHeader& header)
{
	TRACE("Request Singnal Data Stop(0x%02x)\n", header.command);
	// クライアントへのデータ送信用スレッドの終了
	map<UINT, stThreadData>::iterator itr;
	map<UINT, stThreadData>& tdata = GetThreadData();

	for (itr = tdata.begin(); itr != tdata.end(); itr++){
		if ((*itr).second.ipClient == ipClient){
			(*itr).second.start = FALSE;
			SleepEx(mDefaultTimeout, TRUE);
			if (WaitForSingleObject((*itr).second.thread, 1000L) == WAIT_TIMEOUT)
				TerminateThread((*itr).second.thread, 1L);
			CloseHandle((*itr).second.thread);
			tdata.erase(itr);
			break;
		}
	}

	stAckNack ack;
	memset(&ack, 0, sizeof(stAckNack));
	ack.command = header.command;
	ack.status = 0x00;
	ack.size = sizeof(stAckNack);

	// ヘッダー部送信
	CCommBase::SendData(sockClient, (char*)&ack, sizeof(stAckNack));

	return true;
}

/*============================================================================*/
/*! サーバー/クライアント

-# 監視データ名一覧取得要求

@param

@retval

*/
/*============================================================================*/
bool CCommServer::RequestDataList(SOCKET sockClient, stCommandHeader& header)
{
	TRACE("Request Singnal Data List(0x%02x)\n", header.command);
	vector<stDataList>& datalist = GetDataList();
	vector<stDataList>::iterator itr;
	UINT count = (UINT)datalist.size();

	stSendDataList data;
	memset(&data, 0, sizeof(stSendDataList));
	data.header.command = header.command;
	data.header.status = 0x00;
	data.header.size = count * sizeof(stDataList);

	// ヘッダー部送信
	CCommBase::SendData(sockClient, (char*)&data.header, sizeof(data.header));

	// データ部送信
	for (itr = datalist.begin(); itr != datalist.end(); itr++){
		CCommBase::SendData(sockClient, (char*)&(*itr), sizeof(stDataList));
	}

	return true;
}

/*============================================================================*/
/*! サーバー/クライアント

-# 監視データ要求

@param

@retval

*/
/*============================================================================*/
bool CCommServer::RequestGetDataSignal(ULONG ipClient, SOCKET sockClient, stCommandHeader& header)
{
	TRACE("Request Get Data Signal(0x%02x)\n", header.command);
	map<UINT, stThreadData>::iterator itr;
	for (itr = mThreadClientData.begin(); itr != mThreadClientData.end(); itr++){
		if ((*itr).second.ipClient == ipClient){
			(*itr).second.requestData = TRUE;
			break;
		}
	}

	stAckNack ack;
	memset(&ack, 0, sizeof(stAckNack));
	ack.command = header.command;
	ack.status = 0x00;
	ack.size = sizeof(stAckNack);
	// ヘッダー部送信
	CCommBase::SendData(sockClient, (char*)&ack, sizeof(stAckNack));

	return true;
}

/*============================================================================*/
/*! サーバー/クライアント

-# 監視データ送信

@param

@retval

*/
/*============================================================================*/
bool CCommServer::SendGetDataSignal(SOCKET sockClient, LPBYTE lpData, DWORD dwSize)
{
	CCommBase::SendData(sockClient, (char*)lpData, dwSize);

	return true;
}

/*============================================================================*/
/*! サーバー/クライアント

-# ソケットサーバーの作成

@param		iPort			ポート番号

@retval

*/
/*============================================================================*/
bool CCommServer::Create(int iPort)
{
	int iRet;

	if (mSock == INVALID_SOCKET) {
		mSock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
		if (mSock == INVALID_SOCKET) {
			TRACE("Server Socket Open Error\n");
			return false;
		}

		SOCKADDR_IN sAddr;
		memset(&sAddr, 0, sizeof(sAddr));
		sAddr.sin_addr.s_addr = INADDR_ANY;
		sAddr.sin_family = AF_INET;
		sAddr.sin_port = htons(iPort);

		// ソケット名の設定
		iRet = bind(mSock, (SOCKADDR*)&sAddr, sizeof(sAddr));
		if (iRet == SOCKET_ERROR) {
			TRACE("Server Socket Bind Error\n");
			closesocket(mSock);
			return false;
		}

		// 要求の待機を通知
		iRet = listen(mSock, SOMAXCONN);
		if (iRet == SOCKET_ERROR) {
			TRACE("Server Socket Listen Error\n");
			closesocket(mSock);
			return false;
		}
	}
	else {
		TRACE("Server Socket Create Error\n");
		return false;
	}

	mThreadStop = false;

	// スレッドが接続されるのをスレッドを起動して待つ
	UINT uiThreadId = 0;
	mThread = (HANDLE)_beginthreadex(NULL,				// SECURITY_ATTRIBUTES 構造体へのポインタ
		0,					// 新しいスレッドのスタック サイズ
		WaitClientThread,   // 新しいスレッドの実行を開始するルーチンの開始アドレス
		this,				// スレッドパラメタ
		CREATE_SUSPENDED,	// 初期状態フラグ
		&uiThreadId);		// スレッド識別子を受け取る 32 ビット変数へのポインタ

	if (mThread != NULL){
		ResumeThread(mThread);
		return true;
	}

	return true;
}

/*============================================================================*/
/*! サーバー/クライアント

-# ソケットサーバーの破棄

@param

@retval

*/
/*============================================================================*/
bool CCommServer::Release()
{
	if (mSock != INVALID_SOCKET) {
		closesocket(mSock);
		mSock = INVALID_SOCKET;
	}

	mThreadStop = true;
	SleepEx(mDefaultTimeout, TRUE);
	if (WaitForSingleObject(mThread, 1000L) == WAIT_TIMEOUT)
		TerminateThread(mThread, 1L);
	CloseHandle(mThread);
	mThread = NULL;

	map<ULONG, DWORD>::iterator itr;
	for (itr = mThreadList.begin(); itr != mThreadList.end(); itr++){
		HANDLE thread = OpenThread(SYNCHRONIZE | STANDARD_RIGHTS_REQUIRED | THREAD_SUSPEND_RESUME | THREAD_SET_INFORMATION, TRUE, (*itr).second);
		if (thread != NULL)
		{
			if (WaitForSingleObject(thread, 1000L) == WAIT_TIMEOUT)
				TerminateThread(thread, 1L);
			CloseHandle(thread);
		}
	}
	mThreadList.clear();

	return true;
}

/*============================================================================*/
/*! サーバー/クライアント

-# 接続待ち

@param

@retval

*/
/*============================================================================*/
ULONG CCommServer::WaitAccept(long lTimeOut)
{
	int iRet;

	iRet = CCommBase::WaitSelect(mSock, lTimeOut);
	if (iRet == -1) {
		return 0;	//Timeout
	}
	else if (iRet == -2) {
		return 0;
	}

	SOCKADDR_IN tAddr;
	int iLen;
	iLen = sizeof(tAddr);

	SOCKET connectFd;
	connectFd = accept(mSock, (SOCKADDR*)&tAddr, &iLen);
	if (connectFd == INVALID_SOCKET) {

		TRACE("Server Socket Accept Error\n");
		return 0;
	}

	stClientSockInfo info;
	info.sockClient = connectFd;

	memset(info.ipClient, 0, 20);
	sprintf_s(info.ipClient, 20, "%u.%u.%u.%u",
		(UINT)((PBYTE)tAddr.sin_addr.S_un.S_un_b.s_b1),
		(UINT)((PBYTE)tAddr.sin_addr.S_un.S_un_b.s_b2),
		(UINT)((PBYTE)tAddr.sin_addr.S_un.S_un_b.s_b3),
		(UINT)((PBYTE)tAddr.sin_addr.S_un.S_un_b.s_b4));

	if (mClientSockAddrList.find(tAddr.sin_addr.S_un.S_addr) == mClientSockAddrList.end()){
		mClientSockAddrList.insert(pair<ULONG, stClientSockInfo>(tAddr.sin_addr.S_un.S_addr, info));
	}
	else{
		mClientSockAddrList[tAddr.sin_addr.S_un.S_addr].sockClient = info.sockClient;
	}

	return tAddr.sin_addr.S_un.S_addr;
}
/*============================================================================*/
/*! サーバー/クライアント

-# データ送信

@param		pcDataBuf	送信データ
@param		uiDataSize	送信サイズ

@retval

*/
/*============================================================================*/
bool CCommServer::SendData(char* pcDataBuf, unsigned int uiDataSize)
{
	return CCommBase::SendData(mSock, pcDataBuf, uiDataSize);
}

/*============================================================================*/
/*! サーバー/クライアント

-# データ受信

@param		pcDataBuf	受信データ
@param		puiSize		受信サイズ
@param		lTimeOut	タイムアウト

@retval

*/
/*============================================================================*/
bool CCommServer::RecvData(char* pcDataBuf, unsigned int* puiSize, long lTimeOut)
{
	return CCommBase::RecvData(mSock, pcDataBuf, puiSize, lTimeOut);
}



// CCommClient

CCommClient::CCommClient()
	: mSock(INVALID_SOCKET)
{
	mOpen = false;
}

CCommClient::~CCommClient()
{
	if (mSock != INVALID_SOCKET) {
		closesocket(mSock);
		mSock = INVALID_SOCKET;
	}
}

/*============================================================================*/
/*! サーバー/クライアント

-# ソケットクライアントのオープン

@param		iPort			ポート番号
@param		pServerIP		サーバーIPアドレス(Ex:192.168.0.1)

@retval

*/
/*============================================================================*/
bool CCommClient::Open(int iPort, char* pServerIP)
{
	int iRet;

	mOpen = false;

	if (mSock != INVALID_SOCKET) {
		TRACE("Client Socket Already Open\n");
		mOpen = true;
		return true;

	}

	mSock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (mSock == INVALID_SOCKET) {
		TRACE("Client Socket Open Error\n");
		return false;
	}

	ULONG uAddr = INADDR_NONE;
	struct addrinfo hints;
	struct addrinfo *result;
	struct in_addr addr;
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;
	if (getaddrinfo(pServerIP, NULL, &hints, &result) == 0){
		addr.s_addr = ((struct sockaddr_in *)(result->ai_addr))->sin_addr.s_addr;
		uAddr = addr.s_addr;
	}
	else{
		uAddr = INADDR_NONE;
	}


	SOCKADDR_IN sAddr;
	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.s_addr = uAddr;
	sAddr.sin_port = htons(iPort);

	iRet = connect(mSock, (SOCKADDR*)&sAddr, sizeof(sAddr));
	if (iRet == SOCKET_ERROR) {
		TRACE("Client Socket Connect Error\n");
		Close();
		return false;
	}

	mOpen = true;

	return true;
}
/*============================================================================*/
/*! サーバー/クライアント

-# ソケットクライアントのクローズ

@param

@retval

*/
/*============================================================================*/
bool CCommClient::Close()
{
	if (mSock != INVALID_SOCKET) {
		closesocket(mSock);
		mSock = INVALID_SOCKET;
	}
	mOpen = false;
	return true;
}

/*============================================================================*/
/*! サーバー/クライアント

-# データ送信

@param		pcDataBuf	送信データ
@param		uiDataSize	送信サイズ

@retval

*/
/*============================================================================*/
bool CCommClient::SendData(char* pcDataBuf, unsigned int uiDataSize)
{
	return CCommBase::SendData(mSock, pcDataBuf, uiDataSize);
}

/*============================================================================*/
/*! サーバー/クライアント

-# データ受信

@param		pcDataBuf	受信データ
@param		puiSize		受信サイズ
@param		lTimeOut	タイムアウト

@retval

*/
/*============================================================================*/
bool CCommClient::RecvData(char* pcDataBuf, unsigned int* puiSize, long lTimeOut)
{
	return CCommBase::RecvData(mSock, pcDataBuf, puiSize, lTimeOut);
}
