#pragma once


// CSelectServerClient �_�C�A���O

class CSelectServerClient : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectServerClient)

public:
	CSelectServerClient(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSelectServerClient();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	int m_nMode;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
