#pragma once



// CShareFileCheckThread

class CShareFileCheckThread : public CWinThread
{
	DECLARE_DYNCREATE(CShareFileCheckThread)

protected:
	CShareFileCheckThread();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CShareFileCheckThread();

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


