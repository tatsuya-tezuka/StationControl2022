#pragma once


// CControlChangePasswordDlg ダイアログ

class CControlChangePasswordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CControlChangePasswordDlg)

public:
	CControlChangePasswordDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CControlChangePasswordDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CONTROL_CHANGEPASSWORD };

	void	GetPassword(int type, CString& oldPass);
	bool	SetPassword(int type, CString newPass);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nType;
	CString m_strOldPass;
	CString m_strNewPass;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedRadio10();
	afx_msg void OnBnClickedRadio11();
	afx_msg void OnBnClickedRadio12();
	CString m_strTarget;
};
