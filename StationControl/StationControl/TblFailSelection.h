#pragma once
#include "afxwin.h"
#include "const.h"


// CTblFailSelection ダイアログ

class CTblFailSelection : public CDialogEx
{
	DECLARE_DYNAMIC(CTblFailSelection)

public:
	CTblFailSelection(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTblFailSelection();

// ダイアログ データ
	enum { IDD = IDD_DLG_TBL_FILESEL };

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

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// ファイルリスト
	CListBox m_FileList;
	// 選択ファイルEdit
	CEdit m_FileEdit;
	// 選択ファイル
	CString m_strFile;
	afx_msg void OnBnClickedButton1();
};
