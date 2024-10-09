/*============================================================================*/
// 監視情報テーブル
//   CDBAccess::GetObsname_t()で取得エラーとなる監視をテスト用に実装
/*============================================================================*/
// MonitorInfo.h : 実装ファイル

#pragma once

#include "afxwin.h"
#include "atltime.h"
#include "afxcmn.h"
#include "afxdtctl.h"
#include "afxdialogex.h"

#ifdef MON_UT

// CMonitorInfo リスト
class CMonitorListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMonitorListCtrl)

public:
	CMonitorListCtrl();
	virtual ~CMonitorListCtrl();

	int		AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam = 0);

public:
	DECLARE_MESSAGE_MAP()

};

// CMonitorInfo ダイアログ
class CMonitorInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CMonitorInfo)

public:
	CMonitorInfo(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CMonitorInfo();

// ダイアログ データ
	enum { IDD = IDD_DLG_MON_UT };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString m_FileName;
	CMonitorListCtrl m_MonitorList;
	std::vector<CEngValCnv::obsname_t> m_obsname_list;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	virtual BOOL	UpdateDlg();
	void	DisplayList();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMonUt();
	afx_msg void OnUpdateMonUt(CCmdUI *pCmdUI);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedMonUtReadBtn();
	afx_msg void OnBnClickedMonUtRenewBtn();
};

#endif
