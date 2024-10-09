#pragma once


// CControlRegistDlg ダイアログ

class CControlRegistDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CControlRegistDlg)

public:
	CControlRegistDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CControlRegistDlg();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_CONTROLREGIST };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString m_strCtrl;			// 制御名
	CString m_strVal;			// 制御値
	CString m_strValDisp;		// 制御値（表示用）
	CEdit m_cedit;				// 制御名エディットボックス

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	CString GetString();
	void SetCtrlString(CString str)		{ m_strCtrl = str; }
	void SetValString(CString str)		{ m_strVal = str; }

public:
	afx_msg void OnBnClickedButtonCtrlregistSearch();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	/* ------------------------------------------------------------------------------------ */
};
