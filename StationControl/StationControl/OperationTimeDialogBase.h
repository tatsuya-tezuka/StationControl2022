#pragma once


// COperationTimeDialogBase �_�C�A���O

class COperationTimeDialogBase : public CDialogBase
{
	DECLARE_DYNAMIC(COperationTimeDialogBase)

public:
	COperationTimeDialogBase(UINT IDD, CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~COperationTimeDialogBase();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum {
		TYPE_START = 1,
		TYPE_END
	};
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString			m_strDateBefore;
	CString			m_strTimeBefore;
	CDateTimeCtrl	m_dateAfterCtrl;
	CDateTimeCtrl	m_timeAfterCtrl;
	SYSTEMTIME		m_timeBefore;
	SYSTEMTIME		m_timeAfter;

	time_t			m_tMin;
	time_t			m_tMax;

	int				m_Type;	// �J�n or �I��

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

public:
	void			SetBeforeTime(SYSTEMTIME t)	{ m_timeBefore = t; }
#if 0
	void			SetBeforeTimeT(time_t t)	{
		FileTimeToSystemTime((FILETIME*)&t, &m_timeBefore);
	}
#else
	void			SetBeforeTimeT(time_t t)	{
		CTime ctime = CTime(t);
		ctime.GetAsSystemTime(m_timeBefore);
	}
#endif
	SYSTEMTIME		GetBeforeTime()				{ return m_timeBefore; }
	void			SetAfterTime(SYSTEMTIME t)	{ m_timeAfter = t; }
	SYSTEMTIME		GetAfterTime()				{ return m_timeAfter; }
	time_t			GetAfterTimeT()				{
		time_t t;
		if (SystemTimeToFileTime(&m_timeAfter, (FILETIME*)&t))
			return t;
		else
			return 0;
	}
	void			SetType(int type) { m_Type = type; }

	void			SetMinMaxTime(time_t tMin, time_t tMax) {
		m_tMin = tMin;
		m_tMax = tMax;
	}

protected:
	virtual void UpdatePlanData(UINT nStation, CString& strTime) = 0;
	BOOL	UpdatePlanDataAutoOff(CString& cbuf);

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
