#pragma once


// CControlGetPassword �_�C�A���O

class CControlGetPassword : public CDialogEx
{
	DECLARE_DYNAMIC(CControlGetPassword)

public:
	CControlGetPassword(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CControlGetPassword();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_CONTROL_GETPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
