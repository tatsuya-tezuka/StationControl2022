#pragma once
#include "afxwin.h"

// CDownlinkCapture ダイアログ

class CDownlinkCapture : public CDialogBase
{
	DECLARE_DYNAMIC(CDownlinkCapture)

public:
	CDownlinkCapture(CWnd* pParent = NULL);   // 標準コンストラクター
	CDownlinkCapture(UINT IDD, CWnd* pParent = NULL);
	virtual ~CDownlinkCapture();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_DOWNLINKCAPTURE };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString				m_strRcvFreqMode;		// 受信周波数モード文字列
	CString				m_strCtrlMode;			// 制御モード文字列
	CComboBox			m_CCBoxRcvFreqMode;		// 受信周波数モードコンボボックス
	CComboBox			m_CCBoxCtrlMode;		// 制御モードコンボボックス
	eRcvFreqModeType	m_rcvFreqMode;			// 受信周波数モード（監視）
	eCtrlModeType		m_ctrlMode;				// 制御モード（監視）

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	BOOL CreateDlgItems();								// ダイアログアイテムを生成
	BOOL CreateDlgRcvFreqMode();						// 受信周波数モードダイアログアイテムを生成
	BOOL CreateDlgCtrlMode();							// 制御モードダイアログアイテムを生成

	CString GetRcvFreqModeComboStr();					// コンボボックスの受信周波数モード文字列を取得する
	void SetRcvFreqMode(char szRcvFreqMode[]);			// 受信周波数モードを設定する

	CString GetCtrlModeComboStr();						// コンボボックスの制御モード文字列を取得する
	void SetCtrlMode(char szCtrlMode[]);				// 制御モードを設定する
	void SetCtrlModeNull();								// 制御モードをNULL設定する

	BOOL UpdateKanshiData();							// 監視データ更新
	unsigned char GetRcvFreqModNotifyData();			// 受信周波数モード通知データ取得
//	unsigned char GetCtrlModNotifyData();				// 制御モード通知データ取得
	unsigned char GetAutoFreqData();					// 自動の場合の周波数データ
	unsigned char GetManualFreqData();					// 手動の場合の周波数データ
	unsigned char GetWayBinary();						// Wayモードのバイナリ値取得
	unsigned char GetDownLinkBinary();

	UINT GetRcvFreqMode()	{ return (UINT)m_rcvFreqMode; }
	UINT GetCtrlMode()		{ return (UINT)m_ctrlMode; }
	unsigned char GetCtrlModeFromCombo();
	virtual void ShowRcvFreqMode();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDownlinkcaptureRcvfreqmodeButton();
	afx_msg void OnBnClickedDownlinkcaptureCtrlmodeButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	/* ------------------------------------------------------------------------------------ */
	afx_msg void OnCbnSelchangeDownlinkcaptureCtrlmodeCombo();
};
