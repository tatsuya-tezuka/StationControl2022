#pragma once

const UINT mMaxTimer = 8;

// CChangeControlDlg ダイアログ

class CChangeControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChangeControlDlg)

public:
	CChangeControlDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CChangeControlDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CONTROL_CHANGE };

	stControl	mKControl[mMaxType];
	stControl	mSControl[mMaxType];

	void	EnableControl();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_Status;
	DWORD m_Timer[mMaxTimer];
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedRadio10();
	afx_msg void OnBnClickedRadio11();
	afx_msg void OnBnClickedRadio12();
	afx_msg void OnBnClickedRadio13();
	afx_msg void OnBnClickedRadio14();
	afx_msg void OnBnClickedRadio15();
	afx_msg void OnBnClickedRadio16();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
