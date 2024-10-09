#pragma once

#include <mmsystem.h>

// CMessageThread

class CMessageThread : public CWinThread
{
	DECLARE_DYNCREATE(CMessageThread)

public:
	CMessageThread();           // 動的生成で使用される protected コンストラクター
	virtual ~CMessageThread();

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
	bool			m_bStop;			// スレッド終了フラグ

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	StopThread() { 	m_bStop = true; }
protected:
	void	MessageProc(PIPEMONITORACK* pbuf);

	/* ------------------------------------------------------------------------------------ */

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};


