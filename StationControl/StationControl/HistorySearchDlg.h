#pragma once

#include "MFCTabCtrlEx.h"
#include "SenderButton.h"

// CHistorySearchDlg ダイアログ

class CHistorySearchDlg : public CBarChild
{
	DECLARE_DYNAMIC(CHistorySearchDlg)

public:
	CHistorySearchDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CHistorySearchDlg();

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
	enum { IDD = IDD_DIALOG_SEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CDateTimeCtrl m_StartDate;
	CDateTimeCtrl m_StartTime;
	CDateTimeCtrl m_StopDate;
	CDateTimeCtrl m_StopTime;
	virtual BOOL OnInitDialog();
	CGroupBoxEx m_grTime;
	CGroupBoxEx m_grStation;
	CGroupBoxEx m_grSendEq;
	CGroupBoxEx m_grStatus;
	CGroupBoxEx m_grMessage;
	afx_msg void OnBnClickedButtonSelectall();
	afx_msg void OnBnClickedButtonExec();
	afx_msg void OnBnClickedButtonFile();
	afx_msg void OnBnClickedButtonPrint();
	int m_nSender[mStationMax];
	BOOL m_bStation[mStationMax];
	BOOL m_bStatus[mStatusFlagMax];
	CEdit m_edMessage;
	CSenderButton m_btSender[mStationMax];
	CColorCheckButton m_btStation[mStatusFlagMax];
	afx_msg void OnBnClickedCheckStation1();
	afx_msg void OnBnClickedCheckStation2();
	afx_msg void OnBnClickedCheckStation3();
	afx_msg void OnBnClickedCheckStation4();
};
