#pragma once


// CControlGrant �_�C�A���O

class CControlGrant : public CDialogEx
{
	DECLARE_DYNAMIC(CControlGrant)

public:
	CControlGrant(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CControlGrant();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_CONTROL_GRANT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
