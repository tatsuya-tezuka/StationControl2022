#pragma once


// CControlManPassword �_�C�A���O

class CControlManPassword : public CDialogEx
{
	DECLARE_DYNAMIC(CControlManPassword)

public:
	CControlManPassword(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CControlManPassword();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_CONTROL_MANPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
