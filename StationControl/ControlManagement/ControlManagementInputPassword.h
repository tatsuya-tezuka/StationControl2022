#pragma once


// CControlManagementInputPassword ダイアログ

class CControlManagementInputPassword : public CDialogEx
{
	DECLARE_DYNAMIC(CControlManagementInputPassword)

public:
	CControlManagementInputPassword(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CControlManagementInputPassword();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_MANAGEMENT_INPUTPASSWORD };

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
protected:
	CString		mManPassword;
public:

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
protected:

public:

	//-----------------------------------------------------------------------------

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString mInputPassword;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
