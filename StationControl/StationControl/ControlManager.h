#pragma once


// CControlManager �_�C�A���O

class CControlManager : public CDialogEx
{
	DECLARE_DYNAMIC(CControlManager)

public:
	CControlManager(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CControlManager();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_CONTROL_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
