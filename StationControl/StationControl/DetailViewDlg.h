#pragma once
#include "afxcmn.h"


enum{
	DVANCHORE_LEFTTOP = 0x0000,	// 左上は固定させる
	DVANCHORE_RIGHT = 0x0001,	// 右側は固定させる
	DVANCHORE_BOTTOM = 0x0002,	// 下側は固定させる
	DVRESIZE_HOR = 0x0004,	// 横のリサイズを可能にする
	DVRESIZE_VER = 0x0008,	// 縦のリサイズを可能にする
	DVRESIZE_BOTH = (DVRESIZE_HOR | DVRESIZE_VER),
};

class CColorListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CColorListCtrl)

public:
	CColorListCtrl();
	virtual ~CColorListCtrl();

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

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};



// CDetailViewDlg ダイアログ

class CDetailViewDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CDetailViewDlg)

public:
	CDetailViewDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CDetailViewDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_DETAILVIEW };

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
	CString				mSelectFileName;
	CFont				mListFont;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetTitle(CString val) { mTitleText = val; }
	void	SetSelectFileName(CString val) { mSelectFileName = val; }
protected:
	void	AutoSizeColumns(int col = -1);
	int		AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam=0);
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
	afx_msg void OnClose();
};
