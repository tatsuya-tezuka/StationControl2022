#pragma once


// CSleepTimeInputDlg ダイアログ

class CSleepTimeInputDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CSleepTimeInputDlg)

public:
	CSleepTimeInputDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSleepTimeInputDlg();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_SLEEPTIMEINPUT };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString m_str;
	CEdit m_cedit;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	CString GetString();
	void SetString(CString str)		{ m_str = str; }

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	/* ------------------------------------------------------------------------------------ */
};
