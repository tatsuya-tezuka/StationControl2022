#pragma once


// CCancelControlDlg �_�C�A���O

class CCancelControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCancelControlDlg)

public:
	CCancelControlDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CCancelControlDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_CONTROL_CANCEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	int m_nType;
};
