#pragma once


// CSessionStatusTab ダイアログ

class CSessionStatusTab : public CDialogBase
{
	DECLARE_DYNAMIC(CSessionStatusTab)

public:
	CSessionStatusTab(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSessionStatusTab();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_SESSIONSTATUSTAB };

	COLORREF	m_colorText = RGB(0, 0, 0);		// テキストの色
	COLORREF	m_colorBkDisConnect = RGB(255, 0, 0);	// 背景の切断色
	COLORREF	m_colorBkConnect = RGB(0, 128, 0);		// 背景の接続色
	COLORREF	m_colorBkInvalid = RGB(128, 128, 128);	// 背景の無効色
	COLORREF	m_colorTextInv = RGB(255, 255, 255);			// テキストの反転色

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CColorStatic m_csMonitorStatus;		// 監視セッション状態表示
	CColorStatic m_csCtrlStatus;		// 制御セッション状態エディットコントロール
	CColorStatic m_csNotifyStatus;		// 通知セッション状態エディットコントロール
	CColorStatic m_csHistoryStatus;		// 履歴セッション状態エディットコントロール

	CButton m_buttonMonitor;			// 監視セッション状態ボタンコントロール			
	CButton m_buttonCtrl;				// 制御セッション状態ボタンコントロール
	CButton m_buttonNotify;				// 通知セッション状態ボタンコントロール
	CButton m_buttonHistory;			// 履歴セッション状態ボタンコントロール

	CFont		m_txtFont;				// テキストフォント

	CString m_stationName;				// 局名
	UINT m_nStationID;					// 局番号

	CStatic	m_staticCtrlStr;			// 制御セッション文字列
	UINT m_sessionStatus[eSessionType_Num];	// セッション状態

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void SetStationName(CString stationName)	{ m_stationName = stationName; }
	CString GetStationName()					{ return m_stationName; }
	void SetStationID(UINT nStationID)			{ m_nStationID = nStationID; }

	void GetSessionStatus(BOOL bWaitComplete = TRUE);
	void UpdateSessionDisplay();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	void DispMonitorStatus();
	void DispCtrlStatus();
	void DispNotifyStatus();
	void DispHistoryStatus();

	void DispMonitorUnknownStatus();
	void DispMonitorGettingNowStatus();
	void DispMonitorErrorStatus();
	void DispMonitorConStatus();
	void DispMonitorDisConStatus();
	void DispCtrlUnknownStatus();
	void DispCtrlGettingNowStatus();
	void DispCtrlErrorStatus();
	void DispCtrlConStatus();
	void DispCtrlDisConStatus();
	void DispNotifyUnknownStatus();
	void DispNotifyGettingNowStatus();
	void DispNotifyErrorStatus();
	void DispNotifyConStatus();
	void DispNotifyDisConStatus();
	void DispHistoryUnknownStatus();
	void DispHistoryGettingNowStatus();
	void DispHistoryErrorStatus();
	void DispHistoryConStatus();
	void DispHistoryDisConStatus();

	BOOL DisConnectMonitorSession();
	BOOL DisConnectCtrlSession();
	BOOL DisConnectNotifySession();
	BOOL DisConnectHistorySession();

	BOOL ConnectMonitorSession();
	BOOL ConnectCtrlSession();
	BOOL ConnectNotifySession();
	BOOL ConnectHistorySession();

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedSessionstatusMonitorButton();
	afx_msg void OnBnClickedSessionstatusCtrlButton();
	afx_msg void OnBnClickedSessionstatusNotifyButton();
	afx_msg void OnBnClickedSessionstatusHistoryButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	/* ------------------------------------------------------------------------------------ */
};
