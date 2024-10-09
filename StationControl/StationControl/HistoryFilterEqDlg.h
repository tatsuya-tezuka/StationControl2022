#pragma once

#include "MFCTabCtrlEx.h"
#include "SenderButton.h"

// CHistoryFilterEqDlg ダイアログ

class CHistoryFilterEqDlg : public CBarChild
{
	DECLARE_DYNAMIC(CHistoryFilterEqDlg)

public:
	CHistoryFilterEqDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CHistoryFilterEqDlg();

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
protected:
	bool				m_bOneStation;
	SETUPDLGCALLBACK	m_Callback;
	UINT				m_nHistoryType;

public:

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
protected:

public:
	void	SetOneStation(bool val) { m_bOneStation = val; }
	void	SetHistoryType(UINT val) { m_nHistoryType = val; }
	void	SetCallback(SETUPDLGCALLBACK callback, UINT val) { m_Callback = callback; m_nHistoryType = val; }
	//-----------------------------------------------------------------------------

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FILTER_EQ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CGroupBoxEx m_grStation;
	CGroupBoxEx m_grSendEq;
	CGroupBoxEx m_grImStatus;
	CGroupBoxEx m_grStatus;
	CGroupBoxEx m_grMessage;
	afx_msg void OnBnClickedButtonSelectall();
	afx_msg void OnBnClickedButtonExec();
	int m_nSender[mStationMax];
	BOOL m_bStation[mStationMax];
	BOOL m_bImStatus[mImStatusKindMax];
	BOOL m_bStatus[mStatusKindMax];
	CEdit m_edMessage;
	CSenderButton m_btSender[mStationMax];
	CColorCheckButton m_btStation[mStatusFlagMax];
	afx_msg void OnBnClickedCheckStation1();
	afx_msg void OnBnClickedCheckStation2();
	afx_msg void OnBnClickedCheckStation3();
	afx_msg void OnBnClickedCheckStation4();
	afx_msg void OnBnClickedCheckImstatus1();
	afx_msg void OnBnClickedCheckImstatus2();
	afx_msg void OnBnClickedCheckImstatus3();
	afx_msg void OnBnClickedCheckStatus1();
	afx_msg void OnBnClickedCheckStatus2();
	afx_msg void OnBnClickedCheckStatus3();
	afx_msg void OnBnClickedCheckStatus4();
	afx_msg void OnBnClickedCheckStatus5();
	afx_msg void OnBnClickedCheckStatus6();
	afx_msg void OnEnChangeEditMessage();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
