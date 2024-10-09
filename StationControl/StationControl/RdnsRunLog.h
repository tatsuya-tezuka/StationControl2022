#pragma once
#include "afxwin.h"


// CRdnsRunLog ダイアログ

class CRdnsRunLog : public CDialogBase
{
	DECLARE_DYNAMIC(CRdnsRunLog)

public:
	CRdnsRunLog(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CRdnsRunLog();

// ダイアログ データ
	enum { IDD = IDD_DLG_RDNS_RUNLOG };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	int m_left;
	int m_top;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void setPos(int left, int top) {
		m_left = left; m_top = top;
	}

protected:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuClose();
	CListBox m_RunLogList;
	virtual void OnCancel();
	virtual void PostNcDestroy();
};
