#pragma once



// CAlarmListThread

class CAlarmListThread : public CWinThread
{
	DECLARE_DYNCREATE(CAlarmListThread)

public:
	CAlarmListThread();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CAlarmListThread();

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
	bool	m_bStop;			// �X���b�h�I���t���O

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
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


