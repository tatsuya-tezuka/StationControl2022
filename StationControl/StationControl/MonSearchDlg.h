#pragma once
#include "afxwin.h"


// CMonSerchDlg ダイアログ

class CMonSearchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMonSearchDlg)

public:
	CMonSearchDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CMonSearchDlg();

// ダイアログ データ
	enum { IDD = IDD_DLG_MON_SEARCH };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString m_MonNameSearch;

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	void	ResetList(CString name);
	bool	WildCard(TCHAR* pwild, TCHAR* pstr);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strMonName;
	CListBox m_MonNameList;
	afx_msg void OnEnChangeMonnameEdit();
	afx_msg void OnLbnDblclkMonnameList();
	afx_msg void OnBnClickedOk();
};
