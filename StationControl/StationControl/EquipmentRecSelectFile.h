#pragma once
#include "afxwin.h"


// CEquipmentRecSelectFile ダイアログ

class CEquipmentRecSelectFile : public CDialogBase
{
	DECLARE_DYNAMIC(CEquipmentRecSelectFile)

public:
	CEquipmentRecSelectFile(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CEquipmentRecSelectFile();

// ダイアログ データ
	enum { IDD = IDD_DLG_EQIPREC_SELFILE };

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
	CListBox m_FileList;
	afx_msg void OnBnClickedEqiprecSelfileReadBtn();
	afx_msg void OnBnClickedEqiprecSelfileDelBtn();
};
