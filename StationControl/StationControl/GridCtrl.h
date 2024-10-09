#pragma once

using namespace std;

static const UINT mDivHMax = 5;
static const UINT mDivVMax = 4;
static const UINT mHeaderHeight = 20;
#define	WM_GRID_MESSAGE		(WM_USER+100)
static const COLORREF mSelectColor = RGB(0, 0, 0);
typedef struct {
	int			x;
	int			y;
	COLORREF	color;
	CString		text;
} stStatusData;

// CGridCtrl

class CGridCtrl : public CWnd
{
	DECLARE_DYNAMIC(CGridCtrl)

public:
	CGridCtrl();
	virtual ~CGridCtrl();

	//-----------------------------------------------------------------------------
	//	ÉÅÉìÉoïœêî
	//-----------------------------------------------------------------------------
protected:
	CWnd*		mParent;
	BOOL		mInitOver;
	CPoint		mNum;
	CPoint		mSel;
	BOOL		mCaptured;
	CRect		mRect;
	UINT		mMessage;
	BOOL		mHideInvalidSel;
	int			mAlignment;
	CString		mTitle;
	map<UINT, stStatusData>	mItemData;

public:

	//-----------------------------------------------------------------------------
	//	ÉÅÉìÉoä÷êî
	//-----------------------------------------------------------------------------
protected:
	BOOL	RegisterClass();
	void	CloseWindow();
	CSize	CalcSize(int nx, int ny);
	BOOL	IsEnableGrid(CPoint point);

public:
	BOOL	Create(CWnd *pParent, CRect rect, UINT nID);
	void	SetTitle(CString val)	{ mTitle = val; }
	void	SetGridNum(CPoint val)	{ mNum = val; }
	void	SetInvalidGridHide(BOOL val)	{ mHideInvalidSel = val; }
	void	SetAlignment(int val)	{ mAlignment = val; }
	UINT	GetMaxGrid();
	void	AddItem(int x, int y, CString strItem, COLORREF color);

	//-----------------------------------------------------------------------------

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
protected:
	virtual void PostNcDestroy();
};


