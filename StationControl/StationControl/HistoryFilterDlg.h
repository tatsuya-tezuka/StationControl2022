#pragma once

#include "MFCTabCtrlEx.h"
#include "SenderButton.h"

// CHistoryFilterDlg ダイアログ

class CHistoryFilterDlg : public CBarChild
{
	DECLARE_DYNAMIC(CHistoryFilterDlg)

public:
	CHistoryFilterDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CHistoryFilterDlg();

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
	enum { IDD = IDD_DIALOG_FILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CGroupBoxEx m_grStation;
	CGroupBoxEx m_grSendEq;
	CGroupBoxEx m_grStatus;
	CGroupBoxEx m_grMessage;
	afx_msg void OnBnClickedButtonSelectall();
	afx_msg void OnBnClickedButtonExec();
	int m_nSender[mStationMax];
	BOOL m_bStation[mStationMax+1];
	BOOL m_bStatus[mStatusFlagMax];
	CEdit m_edMessage;
	CSenderButton m_btSender[mStationMax];
	CColorCheckButton m_btStation[mStatusFlagMax+1];
	afx_msg void OnBnClickedCheckStation1();
	afx_msg void OnBnClickedCheckStation2();
	afx_msg void OnBnClickedCheckStation3();
	afx_msg void OnBnClickedCheckStation4();
	afx_msg void OnBnClickedCheckStation5();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCheckStatus1();
	afx_msg void OnBnClickedCheckStatus2();
	afx_msg void OnBnClickedCheckStatus3();
	afx_msg void OnEnChangeEditMessage();
};
