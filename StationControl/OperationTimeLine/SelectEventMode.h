#pragma once


// CSelectEventMode �_�C�A���O

class CSelectEventMode : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectEventMode)

public:
	CSelectEventMode(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSelectEventMode();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_EVENTMODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nEventMode;
	afx_msg void OnBnClickedOk();
};
