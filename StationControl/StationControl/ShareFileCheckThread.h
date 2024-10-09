#pragma once



// CShareFileCheckThread

class CShareFileCheckThread : public CWinThread
{
	DECLARE_DYNCREATE(CShareFileCheckThread)

protected:
	CShareFileCheckThread();           // 動的生成で使用される protected コンストラクター
	virtual ~CShareFileCheckThread();

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
	void	StopThread()
	{
		m_bStop = true;
	}

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};


