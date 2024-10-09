#pragma once

#include "MFCTabCtrlEx.h"
#include "SenderButton.h"

// CAlarmFilterDlg ダイアログ

class CAlarmFilterDlg : public CBarChild
{
	DECLARE_DYNAMIC(CAlarmFilterDlg)

public:
	CAlarmFilterDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CAlarmFilterDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FILTER_AL };

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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CGroupBoxEx m_grStation;
	CGroupBoxEx m_grImStatus;
	CGroupBoxEx m_grStatus;
	afx_msg void OnBnClickedButtonSelectall();
	afx_msg void OnBnClickedButtonExec();
	BOOL m_bStation[mStationMax];
	BOOL m_bImStatus[mImStatusKindMax];
	BOOL m_bStatus[mStatusKindMax];
	CColorCheckButton m_btStation[mStatusFlagMax];
};
