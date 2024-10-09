#pragma once

#include <mmsystem.h>

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

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
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


