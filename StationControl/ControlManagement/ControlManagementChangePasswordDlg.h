#pragma once
#include "afxwin.h"


// CControlManagementChangePasswordDlg �_�C�A���O

class CControlManagementChangePasswordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CControlManagementChangePasswordDlg)

public:
	CControlManagementChangePasswordDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CControlManagementChangePasswordDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_MANAGEMENT_CHANGEPASSWORD };

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
protected:
	CString		mManPassword;
public:

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
protected:

public:

	//-----------------------------------------------------------------------------

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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
