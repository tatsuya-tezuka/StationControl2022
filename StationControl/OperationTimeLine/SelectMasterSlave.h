#pragma once


// CSelectMasterSlave �_�C�A���O

class CSelectMasterSlave : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectMasterSlave)

public:
	CSelectMasterSlave(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSelectMasterSlave();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_MASTERSLAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	int mSelectMode;
};
