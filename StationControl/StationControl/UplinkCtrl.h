#pragma once


// CUplinkCtrl ダイアログ

class CUplinkCtrl : public CDialogBase
{
	DECLARE_DYNAMIC(CUplinkCtrl)

public:
	CUplinkCtrl(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CUplinkCtrl();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_UPLINKCTRL };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString				m_strRfTripMonitor;		// RF TRIP監視文字列
	CString				m_strRealAz;			// REAL AZ文字列
	CString				m_strAzSkyLIne;			// AZ方向のSKYLINE文字列
	CComboBox			m_CCBoxRfTripMonitor;	// RF TRIP監視コンボボックス
	eRfTripMonitorType	m_rfTripMonitor;		// RF TRIP監視

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	BOOL CreateDlgItems();									// ダイアログアイテムを生成

	CString GetRfTripMonitorStr();							// コンボボックスのRF TRIP監視文字列を取得する
	void SetRfTripMonitor(char szRfTripMode[]);				// RF TRIP監視を設定する
	void SetRfTripMonitorNull();							// RF TRIP監視をNULL設定する
	BOOL UpdateKanshiData();
	UINT GetRfTripMonitor()		{ return (UINT)m_rfTripMonitor; }
	unsigned char GetUpLinkBinary();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedUplinkctrlRftripmonitorButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	/* ------------------------------------------------------------------------------------ */
};
