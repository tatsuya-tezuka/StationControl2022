#pragma once

#include "CommBase.h"
#include "const.h"

using namespace std;

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

//	DWORD	GetData(LPBYTE lpData, DWORD dwSize);
	DWORD	GetData(LPBYTE lpData, DWORD dwSize, UINT nStation = eStation_MAX);
	DWORD	GetData2(LPBYTE lpData, DWORD dwSize, BOOL bSig1st, UINT nStation = eStation_MAX);
//++ 20181023 UUN-18-074確認用 >>>
#ifdef _DEBUG_STEP_TEST_WAVE
	DWORD	GetData3(LPBYTE lpData, DWORD dwSize);
	FILE *mFp;
#endif /* _DEBUG_STEP_TEST_WAVE */
//++ 20181023 UUN-18-074確認用 <<<
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

	int				m_nCntWaitReadForecast[eStation_MAX];	// 予報値読み待ち回数

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


