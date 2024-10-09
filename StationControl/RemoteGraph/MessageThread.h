#pragma once


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
	int				m_nStation;			// 局番号 0=メイン　1=UDSC64 2=UDSC54 3:USC34 4:USC20

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	StopThread() { 	m_bStop = true; }
	void	SetStation(int val) { m_nStation = val; }
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


