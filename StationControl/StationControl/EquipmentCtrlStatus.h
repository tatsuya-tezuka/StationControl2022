#pragma once
#include "afxwin.h"


// CEquipmentCtrlStatus �_�C�A���O

class CEquipmentCtrlStatus : public CDialogEx
{
	DECLARE_DYNAMIC(CEquipmentCtrlStatus)

public:
	CEquipmentCtrlStatus(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CEquipmentCtrlStatus();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_EQUIPCTRL_STATUS };

	CString	mControlName;
	CString	mCurrentName;
	vector<CString>	mStatusList;
	CString m_strWindowTitle;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:

	void SetWindowTitle(CString str)	{ m_strWindowTitle = str; }
	virtual BOOL OnInitDialog();
	CComboBox m_cbStatus;
	afx_msg void OnBnClickedOk();
	CString m_strContName;
};
