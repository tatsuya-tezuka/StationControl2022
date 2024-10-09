#pragma once



// CAlarmListThread

class CAlarmListThread : public CWinThread
{
	DECLARE_DYNCREATE(CAlarmListThread)

public:
	CAlarmListThread();           // 動的生成で使用される protected コンストラクター
	virtual ~CAlarmListThread();

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
	bool	m_bStop;			// スレッド終了フラグ

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	StopAlarmThread() { m_bStop = true; }

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};


