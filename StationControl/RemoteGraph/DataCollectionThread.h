#pragma once

#include "const.h"

using namespace std;

#define _MAX_NAME	64
#define _MAX_UNIT	8

//=============================================================================
//	ヘッダー
//=============================================================================
typedef struct  {
	DWORD		size;				// ヘッダーを含むデータサイズ
	BYTE		command;			// コマンドID
	BYTE		reserved[3];		// 予約
} stCommandHeader;

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
//	BYTE			status[8];			// ステータス
	USHORT			status;				// ステータス
	BYTE			reserved[32];
} stServerItemData;

static const DWORD mMaxData = 80;
static const DWORD mMaxBufferSize = sizeof(stCommandHeader) + (sizeof(stServerItemData)*mMaxData);

//!< スレッド情報
typedef void (CALLBACK *THREADCALLBACK)(UINT message, WPARAM wParam, LPARAM lParam);
typedef struct{
	void*			parent;		// 呼び出し元ハンドル
	THREADCALLBACK	callback;	// CALLBACK関数ポインタ
} stThreadParam;

// CDataCollectionThread

class CDataCollectionThread : public CWinThread
{
	DECLARE_DYNCREATE(CDataCollectionThread)

public:
	CDataCollectionThread();           // 動的生成で使用される protected コンストラクター
	virtual ~CDataCollectionThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

#ifdef _NOCOMM
	int				m_angle[80];

	DWORD	GetData(LPBYTE lpData, DWORD dwSize);
#endif

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
	// サーバーとの通信クラス
	stThreadParam	m_ThreadParam;		// スレッド情報
	bool			m_bStop;			// スレッド終了フラグ

#ifdef HighSpeedDemo
	CTime	mRealTimeStart;
#endif // HighSpeedDemo

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetThreadParam(stThreadParam& param) { m_ThreadParam = param; }
	void	StopCollection()
	{
		m_bStop = true;
	}

protected:
	UINT	_ConvertNameToId(CString name);
	CString	_ConvertIdToName(UINT Id);

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};

// CSaveRealTimeDataThread

class CSaveRealTimeDataThread : public CWinThread
{
	DECLARE_DYNCREATE(CSaveRealTimeDataThread)

public:
	CSaveRealTimeDataThread();           // 動的生成で使用される protected コンストラクター
	virtual ~CSaveRealTimeDataThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	stThreadParam	m_ThreadParam;		// スレッド情報
	bool			m_bStop;			// スレッド終了フラグ

public:
	void	SetThreadParam(stThreadParam& param) { m_ThreadParam = param; }
	void	Stop()	{ m_bStop = true; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};

// CSaveAnalysisDataThread

class CSaveAnalysisDataThread : public CWinThread
{
	DECLARE_DYNCREATE(CSaveAnalysisDataThread)

public:
	CSaveAnalysisDataThread();           // 動的生成で使用される protected コンストラクター
	virtual ~CSaveAnalysisDataThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	stThreadParam	m_ThreadParam;		// スレッド情報
	bool			m_bStop;			// スレッド終了フラグ
	bool			m_bFinish;			// スレッド完了フラグ

	int				m_nView;			// ビュー番号

protected:
	void	CompAnalysisData(int tbl, vector<stSpaceData>& list, stSpaceData& data);

public:
	void	SetThreadParam(stThreadParam& param) { m_ThreadParam = param; }
	void	Stop()	{ m_bStop = true; }
	bool	IsFinish()	{ return m_bFinish; }
	void	ClrFinish()	{ m_bFinish = false; }

	void	SetViewNo(int nView)	{ m_nView = nView; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};

#pragma once
