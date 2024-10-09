#pragma once

#include <mmsystem.h>

// CMessageStationThread

class CMessageStationThread : public CWinThread
{
	DECLARE_DYNCREATE(CMessageStationThread)

public:
	CMessageStationThread();           // 動的生成で使用される protected コンストラクター
	virtual ~CMessageStationThread();

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
	UINT			m_nStation;			// 局番号
	eMutex			m_nMutex;			// セマフォ番号

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	StopThread() { m_bStop = true; }
	void	SetStation(UINT nStation)		{ 
		m_nStation = nStation;
		switch (m_nStation)
		{
		case eStation_Usuda64:
			m_nMutex = eMessageUdsc64;
			break;

		case eStation_Usuda642:
			m_nMutex = eMessageUdsc54;
			break;

		case eStation_Uchinoura34:
			m_nMutex = eMessageUsc34;
			break;

		case eStation_Uchinoura20:
			m_nMutex = eMessageUsc20;
			break;

		default:
			break;
		}
	}

protected:
	void	MessageProc(PIPEMONITORACK* pbuf);

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual int Run();
	/* ------------------------------------------------------------------------------------ */
};


