#pragma once


// CControlManagementInputPassword �_�C�A���O

class CControlManagementInputPassword : public CDialogEx
{
	DECLARE_DYNAMIC(CControlManagementInputPassword)

public:
	CControlManagementInputPassword(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CControlManagementInputPassword();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_MANAGEMENT_INPUTPASSWORD };

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
	CString mInputPassword;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
