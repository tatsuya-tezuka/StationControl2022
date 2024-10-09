#pragma once
#include "RdnsAgcDetailStep.h"

// CRdnsAgcDetail ダイアログ

class CRdnsAgcDetail : public CDialogBase
{
	DECLARE_DYNAMIC(CRdnsAgcDetail)

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
	CString	m_station;
	CString m_satellite;
	CString m_agcItem;

protected:

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	void SetLoopBand(CString strLB);
protected:

public:
	CRdnsAgcDetail(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CRdnsAgcDetail();

// ダイアログ データ
	enum { IDD = IDD_DLG_RDNS_AGCDETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	int m_nLoopBand;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
