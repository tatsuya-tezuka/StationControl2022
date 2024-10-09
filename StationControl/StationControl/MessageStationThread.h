#pragma once

#include <mmsystem.h>

// CMessageStationThread

class CMessageStationThread : public CWinThread
{
	DECLARE_DYNCREATE(CMessageStationThread)

public:
	CMessageStationThread();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CMessageStationThread();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	bool			m_bStop;			// �X���b�h�I���t���O
	UINT			m_nStation;			// �ǔԍ�
	eMutex			m_nMutex;			// �Z�}�t�H�ԍ�

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
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


