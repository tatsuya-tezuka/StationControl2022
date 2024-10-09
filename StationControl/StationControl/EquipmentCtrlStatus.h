#pragma once
#include "afxwin.h"


// CEquipmentCtrlStatus ダイアログ

class CEquipmentCtrlStatus : public CDialogEx
{
	DECLARE_DYNAMIC(CEquipmentCtrlStatus)

public:
	CEquipmentCtrlStatus(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CEquipmentCtrlStatus();

// ダイアログ データ
	enum { IDD = IDD_DLG_EQUIPCTRL_STATUS };

	CString	mControlName;
	CString	mCurrentName;
	vector<CString>	mStatusList;
	CString m_strWindowTitle;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:

	void SetWindowTitle(CString str)	{ m_strWindowTitle = str; }
	virtual BOOL OnInitDialog();
	CComboBox m_cbStatus;
	afx_msg void OnBnClickedOk();
	CString m_strContName;
};
