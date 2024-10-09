#pragma once


// CSessionStatusUDSC64m ダイアログ

class CSessionStatusUDSC64m : public CDialogEx
{
	DECLARE_DYNAMIC(CSessionStatusUDSC64m)

public:
	CSessionStatusUDSC64m(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSessionStatusUDSC64m();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_SESSIONSTATUSUDSC64M };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CEdit m_ceditMonitor;

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSessionstatusMonitorButton();
	afx_msg void OnBnClickedSessionstatusCtrlButton();
	afx_msg void OnBnClickedSessionstatusNotifyButton();
	afx_msg void OnBnClickedSessionstatusHistoryButton();
	/* ------------------------------------------------------------------------------------ */
};
