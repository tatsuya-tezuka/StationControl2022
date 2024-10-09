#pragma once
#include "resource.h"
#include "afxdtctl.h"
// CStartDateDlg �_�C�A���O

class CStartDateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartDateDlg)

public:
	CStartDateDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CStartDateDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_STARTDATEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CDateTimeCtrl m_DateTimePicker1;
	CDateTimeCtrl m_DateTimePicker2;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
