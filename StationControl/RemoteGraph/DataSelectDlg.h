#pragma once
#include "afxwin.h"


// CDataSelectDlg ダイアログ

class CDataSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataSelectDlg)

public:
	CDataSelectDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CDataSelectDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_ADDDATA };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	bool	m_bAutoAssign;

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	void	_CreateControl();
	void	_ResetControl(int type);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbStation;
	int m_nStation;
	CComboBox m_cbEquipment;
	int m_nEquipment;
	CListBox m_listAll;
	CListBox m_listPart;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboStation();
	afx_msg void OnCbnSelchangeComboEquipment();
	afx_msg void OnLbnSelchangeList2();
	CButton m_btnAutoAssign;
};
