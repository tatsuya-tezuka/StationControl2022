#pragma once


// CControlLogin �_�C�A���O

class CControlLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CControlLogin)

public:
	CControlLogin(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CControlLogin();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_CONTROL_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
