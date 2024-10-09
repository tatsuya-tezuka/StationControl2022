#pragma once
#include "resource.h"
#include "afxdtctl.h"
// CStartDateDlg ダイアログ

class CStartDateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartDateDlg)

public:
	CStartDateDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CStartDateDlg();

// ダイアログ データ
	enum { IDD = IDD_STARTDATEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CDateTimeCtrl m_DateTimePicker1;
	CDateTimeCtrl m_DateTimePicker2;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
