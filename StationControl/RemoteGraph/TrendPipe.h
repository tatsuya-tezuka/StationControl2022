#pragma once

// CTrendPipe コマンド ターゲット

static const DWORD		mPipeBufferSize = 16 * 1024;			//!< バッファサイズ
static const DWORD		mPipeTimeout = (10 * 1000);				//!< 10秒
static const CString	mPipeName_STATION = _T("STATIONEX");		//!< メイン画面側パイプ名称(Station側)

static const CString mPipeNameString[] = {
	_T("STATIONEX01"),
	_T("STATIONEX02"),
	_T("STATIONEX03"),
	_T("STATIONEX04"),
};

enum eCOMMAND{
	eSTATIONSTART,					//!< Station開始
	eSTATIONEND,					//!< Station終了

	eHELLO,							//!< その他

	eTERM = 0xFFFF,					//!< コマンド終端
};

// コマンドID
enum{
	ePipeCommand_start = 501,		// 端末開始要求
	ePipeCommand_stop = 502,		// 端末終了要求
	ePipeCommand_calib = 105,		// 校正診断
	ePipeCommand_monitorget = 201,	// 監視データ要求
	ePipeCommand_history = 301,		// 履歴データ通知
	ePipeCommand_finish = 999,		// パイプ通信終了
};


#pragma pack(1)
typedef struct
{
	DWORD		command;
	DWORD		commandIndex;
	WPARAM		wparam;
	LPARAM		lparam;
	UINT		bufsize;
	BYTE		buf[mPipeBufferSize + 1];
} PIPEBUFFER, *FAR LPPIPEBUFFER;

// 共通ヘッダー
typedef struct
{
	DWORD		size;			// データ部のサイズ
	DWORD		command;		// コマンドID
	DWORD		station;		// 局ID
	DWORD		matching;		// 要求応答ID
} PIPEHEADER;

// 端末開始要求
typedef struct
{
	PIPEHEADER	header;
	BYTE		val[4];			// IPアドレス
} PIPESTART;
typedef struct
{
	PIPEHEADER	header;
	DWORD		code;			// リプライコード
} PIPEACK;

// 端末終了要求
typedef struct
{
	PIPEHEADER	header;
	BYTE		val[4];			// IPアドレス
} PIPESTOP;

// 監視データ要求
#define PIPE_MON_MAX	(32*1024)
typedef struct
{
	PIPEHEADER	header;
	BYTE		buf[16 * 1024];
} PIPEMONITORACK;

#pragma pack()

enum ePIPEMSG{
	ID_PIPE_RECEIVE = WM_USER + 9000,
	ID_PIPE_SEND,
	ID_PIPE_CREATE,
	ID_PIPE_FINISH,

	/*
	ここまで
	*/
};


class CTrendPipe
{
public:
	CTrendPipe();
	virtual ~CTrendPipe();

	//-----------------------------------------------------------------------------
	//	定義
	//-----------------------------------------------------------------------------
public:
	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	bool	Create(bool bServer, void (WINAPI *fCallBack)(PIPEMONITORACK* pbuf), UINT type, DWORD threadNotify);
	bool	Delete();

	static DWORD WINAPI ListnerProc(LPVOID lParam);

	struct ListnerParam{
		HANDLE	hPipe;			// パイプハンドル
		UINT	type;			// AP種別
		DWORD	threadid;		// 通知用スレッドID
		HANDLE	thread;			// スレッドハンドル
		void (WINAPI *funcCallBack)(PIPEMONITORACK* pbuf);	// コマンド取得ファンクション
	};

	void	SetPipeName(CString strName, CString strHost = _T("."));
	CString	GetPipeName()	{ return m_strFullPipeName; }
	CString	GetRealPipeName(bool bServer);

	bool	Send(LPPIPEBUFFER pbuf);
	bool	Send(PIPESTART* pbuf);
	bool	Send(PIPESTOP* pbuf);

protected:
	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
protected:
	ListnerParam*			m_pParam;
	bool					m_bServer;				//!< true=Server false=Client
	CString					m_strPipeName;			//!< パイプ名
	CString					m_strFullPipeName;		//!< 完全パイプ名

	HANDLE					m_hInPipe;				//!< 入力用パイプハンドル
	HANDLE					m_hOutPipe;				//!< 出力用パイプハンドル
	HANDLE					m_hListner;				//!< 取得用ハンドル
	DWORD					m_dwListnerThreadId;
	DWORD					mPipeCount;
};


