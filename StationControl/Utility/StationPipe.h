// WTViewerPipe.h

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

static const DWORD		mPipeTimeout=(10*1000);				//!< 10秒
static const DWORD		mPipeBufferSize=16*1024;			//!< バッファサイズ

static const CString	mPipeName_STATION = _T("STATION");		//!< メイン画面側パイプ名称(Station側)
static const CString	mPipeName_TIMELINE = _T("TIMELINE");	//!< ツール画面側パイプ名称(TimeLine側)

static const CString	mPipeShareFileName = _T("~$$$TIME$$$.DAT");

enum ePIPETYPE{
	ePIPETYPE_TIMELINE,
	ePIPETYPE_STATION,
	ePIPETYPE_STATION01,
	ePIPETYPE_STATION02,
	ePIPETYPE_STATION03,
	ePIPETYPE_STATION04,

	ePIPETYPE_STATION1_UDSC64,
	ePIPETYPE_STATION1_XDSC54,
	ePIPETYPE_STATION1_USC34,
	ePIPETYPE_STATION1_USC20,
	ePIPETYPE_STATION5,

	ePIPETYPE_MAX,
};
static const CString mPipeNameString[] = {
	_T("TIMELINE"),
	_T("STATIONEX01"),
	_T("STATIONEX02"),
	_T("STATIONEX03"),
	_T("STATIONEX04"),

	_T("STATION1UDSC64"),
	_T("STATION1XDSC54"),
	_T("STATION1USC34"),
	_T("STATION1USC20"),
	_T("STATION5"),
};

enum ePIPEMSG{
	ID_PIPE_RECEIVE = WM_USER+9000,
	ID_PIPE_SEND,
	ID_PIPE_CREATE,
	ID_PIPE_FINISH,
	ID_TIMELINE_MESSAGE,
	ID_PIPE_SERVER,

	/*
	これ以降、StationControl.exeを呼び出して、情報を取得する
	*/
	eTIMELINE_GET_SATNAME,			//!< 衛星名の取得（メッセージを送って共有メモリから取得）
	eTIMELINE_GET_XS,				//!< X帯、S帯送信設備使用有無情報取得（メッセージを送って共有メモリから取得）
	eTIMELINE_GET_PRED,				//!< X帯、S帯送信設備使用有無情報取得（メッセージを送って共有メモリから取得）
	/*
	ここまで
	*/
	eTIMELINE_SET_MUTE,				// ADD 2021.12.02 ミュート指示
};

enum ePIPECOMMAND{
	mCommand = 0,

	mFinishCommand = 0xffffffff,
};

enum eCOMMAND{
	eSENDMSG		= 0x0000,		//!< 送信メッセージ
	eRECVMSG,						//!< 受信メッセージ
	eTIMELINESTART,					//!< タイムライン開始
	eTIMELINEEND,					//!< タイムライン終了
	eSTATIONSTART,					//!< Station開始
	eSTATIONEND,					//!< Station終了
	eANTEMG,						//!< アンテナ強制停止
	eSENDSTOP,						//!< 送信機停止
	eALARMSTOP,						//!< アラーム停止
	eALARMMUTEOFF,					//!< アラームミュート
	eALARMMUTEON,					//!< アラームミュート

	eHISTORYEQ,						//!< 履歴情報（設備）
	eHISTORYEVENT,					//!< 履歴情報（イベント）
	eMONITORDATA,					//!< 監視データ

	eTIMELINE_FORECASTPLAN,			//!< 運用計画一覧画面呼び出し
	eTIMELINE_FORECAST,				//!< 予報値一覧画面呼び出し
	eTIMELINE_OPEPLAN,				//!< 運用計画立案画面呼び出し
	eTIMELINE_PLANDELETE,			//!< 計画削除確認画面呼び出し
	eTIMELINE_PLANDETAIL,			//!< 計画詳細画面呼び出し
	eTIMELINE_PLANSTARTTIME,		//!< 計画開始時刻変更画面呼び出し
	eTIMELINE_PLANSTOPTIME,			//!< 計画終了時刻変更画面呼び出し
	eTIMELINE_PLANEMGSTOP,			//!< 強制運用停止確認画面呼び出し
	eTIMELINE_TIMEOFFSET,			//!< 時刻オフセット画面呼び出し
	eTIMELINE_CALIBRATION,			//!< 校正診断画面呼び出し
	eTIMELINE_EQUIPMENTOCC,			//!< 局設備占有時間入力画面呼び出し
	eTIMELINE_VLBI,					//!< VLBI運用入力画面呼び出し

	eHELLO,							//!< その他

	eTERM			= 0xFFFF,		//!< コマンド終端
};

enum eCOMMANDRESULT{
	eRESULT_NORMAL	= 0x00,		//!< 正常終了

	eRESULT_ERROR	= 0xFF,		//!< エラー
};

enum eCOMMANDRESULTDETAIL{
	eRESULTDETAIL_OKGO		= 0x00,		//!< 正常終了（コマンド実行）

	eRESULTDETAIL_ERROR		= 0xFF,		//!< エラー原因不明
};

// コマンドID
enum{
	ePipeCommand_start = 501,		// 端末開始要求
	ePipeCommand_stop = 502,		// 端末終了要求
	ePipeCommand_calib = 105,		// 校正診断
	ePipeCommand_monitorget = 201,	// 監視データ要求
	ePipeCommand_history = 301,		// 履歴データ通知
};

// 局ID
enum{
	ePipeStation_none,
	ePipeStation_udsc64,
	ePipeStation_usc34,
	ePipeStation_usc20,
	ePipeStation_xdsc54,
};
// 要求応答ID
enum{
	ePipeRequest_request,
	ePipeRequest_other,
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
	BYTE		buf[16*1024];
	//long        l_unix_msgtype;    /* メッセージタイプ(64) */
	//long        l_len;             /* 送信バイト数 */
	//long        l_objtype;         /* 要求ＩＤ */
	//long        l_blkcnt;          /* ブロックカウント */
	//long        l_send_ipc;        /* 送信側メッセージキューＩＤ */
	//long        l_recv_ipc;        /* 受信側メッセージキューＩＤ */
	//unsigned char uca_SdtpDummy[26];/* 受信側メッセージキューＩＤ */
	//unsigned char uca_SdtpPDULen[4];/* PDU長 */
	//unsigned char uca_SdtpRecNum[2];/* レコード数 */
	//unsigned char uca_buff[PIPE_MON_MAX];/* データバッファ */
} PIPEMONITORACK;
#pragma pack()

class CStationPipe
{
public:
	CStationPipe();
	virtual ~CStationPipe();

	bool	Create(bool bServer, void (WINAPI *fCallBack)(LPPIPEBUFFER pbuf), UINT type, CWnd* pCallWnd);
	bool	CreateService(bool bServer, void (WINAPI *fCallBack)(PIPEMONITORACK* pbuf), UINT type, CWnd* pCallWnd);
	bool	Delete();

	static DWORD WINAPI ListnerProc(LPVOID lParam);
	static DWORD WINAPI ListnerServiceProc(LPVOID lParam);
	struct ListnerParam{
		HANDLE	hPipe;			// パイプハンドル
		UINT	type;			// AP種別
		CWnd*	pCallWnd;		// メッセージウィンドウ
		void (WINAPI *funcCallBack)(LPPIPEBUFFER pbuf);	// コマンド取得ファンクション
	};
	struct ListnerServiceParam{
		HANDLE	hPipe;			// パイプハンドル
		UINT	type;			// AP種別
		CWnd*	pCallWnd;		// メッセージウィンドウ
		void (WINAPI *funcCallBack)(PIPEMONITORACK* pbuf);	// コマンド取得ファンクション
	};
protected:
	ListnerParam*	m_pParam;
	ListnerServiceParam*	m_pParamService;
	bool	m_bServer;				//!< true=Server false=Client
	CString	m_strPipeName;			//!< パイプ名
	CString	m_strFullPipeName;		//!< 完全パイプ名

	HANDLE	m_hInPipe;				//!< 入力用パイプハンドル
	HANDLE	m_hOutPipe;				//!< 出力用パイプハンドル
	HANDLE	m_hListner;				//!< 取得用ハンドル
	DWORD	m_dwListnerThreadId;
	DWORD	mPipeCount;

	void	CreateDataToHistory(PIPEMONITORACK* pbuf);

public:
	void	SetPipeName(CString strName, CString strHost = _T("."));
	CString	GetPipeName()	{ return m_strFullPipeName; }
	CString	GetRealPipeName(bool bServer);

	bool	Send(LPPIPEBUFFER pbuf);
	bool	Send(PIPESTART* pbuf);
	bool	Send(PIPESTOP* pbuf);
};

static const CString mMemoryName	= _T("STATIONSHARE");
static const CString mMutexName		= _T("Global\\STATIONMUTEX");
static const CString mEventExecName = _T("STATIONEVENTEXEC");
static const CString mEventDoneName = _T("STATIONEVENTDONE");
class CShareMemory
{
public:
	CShareMemory();
	~CShareMemory();
//-----------------------------------------------------------------------------
//	定義
//-----------------------------------------------------------------------------
public:
	struct stManagement{
		UINT	count;		// 格納済カウント
	};
	static const UINT mMemorySize = 2048;
	static const UINT mMemoryCount = 100;
//-----------------------------------------------------------------------------
//	メンバ関数
//-----------------------------------------------------------------------------
public:
	void		Create();
	void		Delete();
	bool		Enable() { if(mFileMap != NULL) return true; else return false; }
	void		Push(CStringA data);
	CStringA	Pop();
	UINT		Count();
protected:
//-----------------------------------------------------------------------------
//	メンバ変数
//-----------------------------------------------------------------------------
public:
protected:
	HANDLE	mFileMap;
	HANDLE	mMutex;
	bool	mApplication;
	char*	mMessagePtr;
	HANDLE	mExec;
	HANDLE	mDone;
};


class CServerFile
{
public:
	CServerFile();
	~CServerFile();
	//-----------------------------------------------------------------------------
	//	定義
	//-----------------------------------------------------------------------------
public:
	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	static CString		ConvertUnicode(char* euc);
	static CStringA		ConvertEuc(WCHAR* utf);
protected:
	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
protected:
};
