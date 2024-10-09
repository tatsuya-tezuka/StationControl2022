#pragma once
#include "afxcmn.h"

#include "DetailViewDlg.h"

enum{
	eAntHistory_Operation,
	eAntHistory_Drive,
};

// CAntennaHistroyView ダイアログ

class CAntennaHistroyView : public CDialogBase
{
	DECLARE_DYNAMIC(CAntennaHistroyView)

public:
	CAntennaHistroyView(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CAntennaHistroyView();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_ANTHISTORY };

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
	int					mSaveCx, mSaveCy;
	vector<DWORD>		mControl;
	CString				mTitleText;
	UINT				mType;
	CString				mSelectFileName;
	CFont				mListFont;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	virtual BOOL UpdateDlg();
	void	SetTitle(CString val, UINT type) { mTitleText = val; mType = type; }
	void	SetSelectFileName(CString val) { mSelectFileName = val; }
protected:
	void	AutoSizeColumns(int col = -1);
	int		AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam = 0);
	void	SetControlInfo(WORD id, WORD anchore);
	bool	ReadFile(CString filename);
	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void OnCancel(){};

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSelectFile;
	CColorListCtrl m_ListFile;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void PostNcDestroy();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnMenuOpen();
	afx_msg void OnMenuClose();
	afx_msg void OnMenuPrint();
	afx_msg void OnMenuSearch();
	CString mSearchText;
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonSearch2();
};
