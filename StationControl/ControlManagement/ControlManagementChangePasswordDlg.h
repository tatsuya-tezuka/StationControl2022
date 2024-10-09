#pragma once
#include "afxwin.h"


// CControlManagementChangePasswordDlg ダイアログ

class CControlManagementChangePasswordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CControlManagementChangePasswordDlg)

public:
	CControlManagementChangePasswordDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CControlManagementChangePasswordDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_MANAGEMENT_CHANGEPASSWORD };

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
	CString mCurrentPass;
	CString mNewPass;
	CString mRenewPass;
	afx_msg void OnBnClickedOk();
	CEdit mEdCurrentPass;
	CEdit mEdNewPass;
	CEdit mEdRenewPass;
};
