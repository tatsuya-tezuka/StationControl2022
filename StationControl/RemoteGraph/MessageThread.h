#pragma once


// CMessageThread

class CMessageThread : public CWinThread
{
	DECLARE_DYNCREATE(CMessageThread)

public:
	CMessageThread();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CMessageThread();

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
	int				m_nStation;			// �ǔԍ� 0=���C���@1=UDSC64 2=UDSC54 3:USC34 4:USC20

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
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


