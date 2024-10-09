#pragma once


// CCancelControlDlg ダイアログ

class CCancelControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCancelControlDlg)

public:
	CCancelControlDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CCancelControlDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CONTROL_CANCEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	int m_nType;
};
