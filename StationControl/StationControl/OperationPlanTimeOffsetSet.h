#pragma once


// COperationPlanTimeOffsetSet ダイアログ

class COperationPlanTimeOffsetSet : public CDialogBase
{
	DECLARE_DYNAMIC(COperationPlanTimeOffsetSet)

public:
	COperationPlanTimeOffsetSet(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~COperationPlanTimeOffsetSet();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_OPERATIONPLANTIMEOFFSETSET };

	static const int m_timeMMMax = 5;				// 最大値(分）
	static const int m_timeSSMax = 60;				// 最大値(秒）

	static const int m_timeMin = - m_timeMMMax * m_timeSSMax;			// 最小値(合計秒)
	static const int m_timeMax = m_timeMMMax * m_timeSSMax;			// 最大値(合計秒)

	enum {
		TYPE_TIMELINE,
		TYPE_ATHER
	};

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
//	CDateTimeCtrl	m_timeCtrl;
//	SYSTEMTIME		m_timeOffset;
	time_t			m_time;
	CString			m_strStationName;
	CString			m_strFullFilePath;
	CMFCMaskedEdit m_CMaskedEdit;
	CSpinButtonCtrl m_CSpin;
	int				m_Type;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

protected:
	void UpdateOffsetTime();
	BOOL GetOffsetTimeFromEditCtrl(time_t& offsetTime, BOOL bAutoCorrect = FALSE);

public:
	void SetTime(time_t time)		{ m_time = time; }
	time_t GetTime()				{ return m_time; }
	void SetStationName(CString& str)	{ m_strStationName = str; }
	void SetFullFilePath(CString& str)	{ m_strFullFilePath = str; }
	void SetCaller(int type) { m_Type = type; }

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOperationplantimeoffsetsetButton1();
	afx_msg void OnBnClickedOperationplantimeoffsetsetButton2();
	afx_msg void OnBnClickedOperationplantimeoffsetsetButton3();
	afx_msg void OnDeltaposOffsettimeSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	/* ------------------------------------------------------------------------------------ */
};
