#pragma once

#include <list>
#include <map>
#include <vector>
using namespace std;

static const CString mBaseStationString[] = {
	_T("UDSC64"),
	_T("USC20"),
	_T("USC34"),
	_T("UDSC54"),
};
static const CString mEquipmentString[] = {
	_T("ANT"),
	_T("BITRATE"),
	_T("CMD"),
	_T("CTRL"),
};
static const CString mDataNameString[] = {
	_T("AZ_DRIVE"),
	_T("AZ_OFFSET"),
	_T("DRIVE_MODE"),
	_T("EL_DRIVE"),
	_T("EL_OFFSET"),
};
static const CString mDataUnitString[] = {
	_T("Hz"),
	_T("V"),
	_T("A"),
	_T("W"),
	_T("℃"),
};

//=============================================================================
//	監視データ
//=============================================================================
#pragma pack(1)
#define _MAX_NAME	64
#define _MAX_UNIT	8

enum{
	eCommand_ReqSignal = 0x01,
	eCommand_ReqStart = 0x02,
	eCommand_ReqStop = 0x03,
	eCommand_ReqDataList = 0x04,
	eCommand_ReqGetData = 0x05,
	eCommand_SendData = 0x10,
};

enum{
	eMessage_ClientConnection = (WM_USER + 1000),
	eMessage_ClientDisConnection,
	eMessage_ClientCommand,
};

//=============================================================================
//	ヘッダー
//=============================================================================
typedef struct  {
	DWORD		size;				// ヘッダーを含むデータサイズ
	BYTE		command;			// コマンドID
	BYTE		reserved[3];		// 予約
} stCommandHeader;

//=============================================================================
//	監視データ登録要求コマンド（eCommand_ReqSignal）
//=============================================================================
typedef struct  {
	stCommandHeader	header;			// コマンドヘッダー
	BYTE			name[_MAX_NAME];// 監視データ名称（複数有り）
} stReqSignal;
typedef struct  {
	BYTE			name[_MAX_NAME];// 監視データ名称（複数有り）
} stReqSignalName;

//=============================================================================
//	監視データ送信開始要求コマンド（eCommand_ReqStart）
//=============================================================================
typedef struct  {
	stCommandHeader	header;			// コマンドヘッダー
} stReqStart;

//=============================================================================
//	監視データ送信停止要求コマンド（eCommand_ReqStop）
//=============================================================================
typedef struct  {
	stCommandHeader	header;			// コマンドヘッダー
} stReqStop;

//=============================================================================
//	監視データ名一覧取得要求コマンド（eCommand_ReqDataList）
//=============================================================================
typedef struct  {
	stCommandHeader	header;			// コマンドヘッダー
} stReqDataList;

//=============================================================================
//	監視データ送信コマンド（eCommand_SendData）
//=============================================================================
typedef struct  {
	WORD			wYear;				// 年
	WORD			wDay;				// １年間の通算日
	WORD			wHour;				// 時間
	WORD			wMinute;			// 分
	WORD			wSecond;			// 秒
	WORD			wMilliseconds;		// ミリ秒
} stServerTime;
typedef struct  {
	stServerTime	systime;			// 時刻
	BYTE			name[_MAX_NAME];	// データ名称
	DWORD			kind;				// データ種別
	double			value;				// データ
	BYTE			status[8];			// ステータス
	BYTE			reserved[32];
} stServerItemData;
typedef struct  {
	stCommandHeader		header;				// コマンドヘッダー
	stServerItemData	item;				// アイテム
} stServerData;

//=============================================================================
//	応答
//=============================================================================
typedef struct  {
	DWORD		size;				// ヘッダーを含むデータサイズ
	BYTE		command;			// コマンドID
	BYTE		status;				// Ack/Nack
	BYTE		reserved[2];		// 予約
} stAckNack;
typedef struct  {
	BYTE		name[_MAX_NAME];	// 信号名称
	BYTE		unit[_MAX_UNIT];	// 信号単位
} stDataList;
typedef struct  {
	stAckNack	header;				// コマンドヘッダー
	stDataList	data[1];			// 信号リスト
} stSendDataList;
//=============================================================================
typedef struct  {
	BOOL	start;
	BOOL	requestData;
	HANDLE	thread;
	ULONG	ipClient;
	SOCKET	sockClient;
} stThreadData;
//=============================================================================
#pragma pack()


// CCommBase コマンド ターゲット

class CCommBase
{
public:
	CCommBase();
	virtual ~CCommBase();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	static const UINT mCtrlPort = 50001;
	static const UINT mDataPort = 50002;
	static const DWORD mDefaultTimeout = 100L;
	static const DWORD mDefaultCommTimeout = 1000L;
	static const DWORD mMaxHostName = MAX_PATH;
	static const DWORD mMaxData = 80;
	static const DWORD mMaxBufferSize = sizeof(stCommandHeader) + (sizeof(stServerItemData)*mMaxData);

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	bool			mThreadStop;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	static int		WaitSelect(SOCKET fd, long lMilliSec);
	static bool		SendData(SOCKET fd, char* pcDataBuf, unsigned int uiDataSize);
	static bool		RecvData(SOCKET fd, char* pcDataBuf, unsigned int* puiSize, long lTimeOut);
	bool			GetThreadStop()	{ return mThreadStop; }

	static void		ConvertTime(const stServerTime* unixTime, SYSTEMTIME* winTime);
	static void		ConvertTime(const SYSTEMTIME* winTime, stServerTime* unixTime);

	static bool		GetLocalAddress(char* strAddress, UINT nSize);
	static bool		GetLocalName(char* strName, UINT nSize);
	static USHORT	GetPortNumber(char* strServiceName);
	static ULONG	GetIPAddress(char* strHostName);

protected:

};


typedef struct{
	SOCKET	sockClient;
	char	ipClient[20];
} stClientSockInfo;

typedef std::map<ULONG, stClientSockInfo> CADDRLIST;

// CCommServer コマンド ターゲット

class CCommServer : public CCommBase
{
public:
	CCommServer();
	virtual ~CCommServer();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetMessageWnd(CWnd* val) { mMessageWnd = val; }
	map< ULONG, vector<stReqSignalName> >&	GetSignalList() { return mSignalList; }
	vector<stDataList>&	GetDataList() { return mDataList; }
	map<UINT, stThreadData>&	GetThreadData() { return mThreadClientData; }

protected:
	map< ULONG, vector<stReqSignalName> >	mSignalList;
	vector<stDataList>		mDataList;
	CWnd*					mMessageWnd;
	map<UINT, stThreadData>		mThreadClientData;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	SOCKET			mSock;
	HANDLE			mThread;
	CADDRLIST		mClientSockAddrList;
	map<ULONG, DWORD>	mThreadList;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	bool	Create(int iPort);
	bool	Release();
	ULONG	WaitAccept(long lTimeOut);
	bool	SendData(char* pcDataBuf, unsigned int uiDataSize);
	bool	RecvData(char* pcDataBuf, unsigned int* puiSize, long lTimeOut);
	void	Wait();
	void	Client(DWORD threadid);
	CADDRLIST& GetClientAddrList() { return mClientSockAddrList; }
	bool	SendGetDataSignal(SOCKET sockClient, LPBYTE lpData, DWORD dwSize);

	/* ------------------------------------------------------------------------------------ */
	/* コマンド                                                                             */
	/* ------------------------------------------------------------------------------------ */
	bool	RequestSignal(ULONG ipClient, SOCKET sockClient, stCommandHeader& header);
	bool	RequestSignalStart(ULONG ipClient, SOCKET sockClient, stCommandHeader& header);
	bool	RequestSignalStop(ULONG ipClient, SOCKET sockClient, stCommandHeader& header);
	bool	RequestDataList(SOCKET sockClient, stCommandHeader& header);
	bool	RequestGetDataSignal(ULONG ipClient, SOCKET sockClient, stCommandHeader& header);

	DWORD	GetThreadId(ULONG ipClient)
	{
		map<ULONG, DWORD>::iterator itr = mThreadList.find(ipClient);
		if (itr == mThreadList.end())
			return 0;

		return (*itr).second;
	}
	ULONG	GetThreadClient(DWORD thread)
	{
		map<ULONG, DWORD>::iterator itr;
		for (itr = mThreadList.begin(); itr != mThreadList.end(); itr++){
			if ((*itr).second == thread)
				return (*itr).first;
		}

		return 0;
	}
	void	ResetThreadId(ULONG ipClient)
	{
		map<ULONG, DWORD>::iterator itr = mThreadList.find(ipClient);
		if (itr == mThreadList.end())
			return;

		(*itr).second = 0xffffffff;
	}
	void	Clear()
	{
		map<ULONG, DWORD>::iterator itr;
		bool ret = true;
		while (ret)
		{
			ret = false;
			for (itr = mThreadList.begin(); itr != mThreadList.end(); itr++){
				if ((*itr).second == 0xffffffff){
					mThreadList.erase(itr);
					ret = false;
					break;
				}
			}
		}
	}
protected:
	static UINT WINAPI WaitClientThread(LPVOID pParam);
	static UINT WINAPI ClientCommThread(LPVOID pParam);
	static UINT WINAPI ClientDataThread(LPVOID pParam);
};


// CCommClient コマンド ターゲット

class CCommClient : public CCommBase
{
public:
	CCommClient();
	virtual ~CCommClient();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	SOCKET			mSock;
	bool			mOpen;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	bool	IsOpen()	{ return mOpen; }

	bool	Open(int iPort, char* pServerIP);
	bool	Close();
	bool	SendData(char* pcDataBuf, unsigned int uiDataSize);
	bool	RecvData(char* pcDataBuf, unsigned int* puiSize, long lTimeOut);
protected:

};



