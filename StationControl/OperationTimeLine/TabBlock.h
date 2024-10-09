#pragma once
#include "fapanel1.h"


// CTabBlock ダイアログ

class CTabBlock : public CDialogEx
{
	DECLARE_DYNCREATE(CTabBlock)

public:
	CTabBlock(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabBlock();

// ダイアログ データ
	enum { IDD = IDD_TAB_BLOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CFapanel1 m_FaBlock;
};
