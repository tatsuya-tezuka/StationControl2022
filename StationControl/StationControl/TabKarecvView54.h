#pragma once
#include "afxwin.h"

#ifdef _DEBUG_KANSHI64M
// 64m監視
static const stControlParam mKarecvParam54[] = {
	{ IDC_BUTTON_DCCTRL, _T("S70-DC.SET_FREQ"), _T("S70-DC.SET_FREQ"), eControlDouble },		// D/C CTRL■
	{ IDC_BUTTON_RX, _T("DSNTCR.DOWN_REF_FREQ"), _T("DSNTCR.DOWN_REF_FREQ"), eControlDouble },		// RX■
};
#else
// 54m監視
static const stControlParam mKarecvParam54[] = {
	{ IDC_BUTTON_DCCTRL, _T("X-DC.Ka-RX_FREQUENCY"), _T("X-DC.Ka-RX_FREQUENCY"), eControlDouble },		// D/C CTRL■
	{ IDC_BUTTON_RX, _T("DSNTCR.DOWN_REF_FREQ"), _T("DSNTCR.DOWN_REF_FREQ"), eControlDouble },		// RX■
};
#endif

// CTabKarecvView54 ダイアログ

class CTabKarecvView54 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabKarecvView54)

public:
	CTabKarecvView54(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabKarecvView54();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
// ダイアログ データ
	enum { IDD = IDD_TAB_KARECV54 };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	ControlParamExecute(UINT pos);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

protected:
	void	UpdateStatus();
	void	InitStatus();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDcctrl();
	afx_msg void OnBnClickedButtonRx();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};
