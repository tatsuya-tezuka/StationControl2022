#pragma once
#include "afxcmn.h"
#include "ListCtrlEx.h"


// COperationAccount ダイアログ

class COperationAccount : public CDialogEx
{
	DECLARE_DYNAMIC(COperationAccount)

public:
	COperationAccount(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~COperationAccount();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_OPERATION_ACCOUNT };

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
protected:
	CImageList			m_ImageList;
	vector<stAccount>	mAccountList;
public:

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
protected:
	void		AutoSizeColumns(int col = -1);
	int			AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam=0);
	HIMAGELIST	CreateImageList(CDC* pdc);

public:

	//-----------------------------------------------------------------------------

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx mList;
	afx_msg void OnHdnDividerdblclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
