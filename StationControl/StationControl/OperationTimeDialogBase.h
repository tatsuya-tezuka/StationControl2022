#pragma once


// COperationTimeDialogBase ダイアログ

class COperationTimeDialogBase : public CDialogBase
{
	DECLARE_DYNAMIC(COperationTimeDialogBase)

public:
	COperationTimeDialogBase(UINT IDD, CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~COperationTimeDialogBase();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum {
		TYPE_START = 1,
		TYPE_END
	};
protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
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

	int				m_Type;	// 開始 or 終了

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

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
