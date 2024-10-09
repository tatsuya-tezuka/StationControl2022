#pragma once
#include "afxwin.h"


// CLoopBandSelectDlg ダイアログ

class CLoopBandSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoopBandSelectDlg)

public:
	CLoopBandSelectDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CLoopBandSelectDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_LOOPBAND };

	//-----------------------------------------------------------------------------
	//	定義
	//-----------------------------------------------------------------------------
public:
protected:

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
	CString		m_strLoopBand;

protected:
	vector<CString> m_LBCmbList;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	void SetLBCmbList(vector<CString> list) { m_LBCmbList = list; }
	void SetLoopBand(CString str) { m_strLoopBand = str; }

protected:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_LBCmbCtrl;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
