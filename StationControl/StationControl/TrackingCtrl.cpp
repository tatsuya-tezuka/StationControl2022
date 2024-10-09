// TrackingCtrl.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrackingCtrl.h"


// CTrackingCtrl

IMPLEMENT_DYNAMIC(CTrackingCtrl, CWnd)

CTrackingCtrl::CTrackingCtrl()
{
	mParent = NULL;
	mInitOver = FALSE;
	mRect.SetRectEmpty();
}

CTrackingCtrl::~CTrackingCtrl()
{
}


BEGIN_MESSAGE_MAP(CTrackingCtrl, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CTrackingCtrl メッセージ ハンドラー
/*============================================================================*/
/*! 分割グリッドコントロール
分割グリッドウィンドウの作成

@param		pParent		親ウィンドウハンドル
@param		rect		領域

@retval BOOL
*/
/*============================================================================*/
BOOL CTrackingCtrl::Create(CWnd *pParent, CRect rect, UINT nID)
{
	if (!RegisterClass())
		return FALSE;

	mParent = pParent;
	if (CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParent, nID, NULL)){
		mInitOver = TRUE;
		return TRUE;
	}
	return FALSE;
}

/*============================================================================*/
/*! 分割グリッドコントロール
ウィンドウクラスを登録

@param
@param

@retval BOOL
*/
/*============================================================================*/
BOOL CTrackingCtrl::RegisterClass()
{
	return TRUE;
	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = AfxWndProc;
	wc.hInstance = AfxGetInstanceHandle();
	wc.hbrBackground = ::CreateSolidBrush(COLOR_WINDOW);
	wc.hCursor = ::LoadCursor(AfxGetInstanceHandle(), IDC_ARROW);
	wc.lpszClassName = _T("TRACKINGWINDOW");
	return AfxRegisterClass(&wc);
}

int CTrackingCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ここに特定な作成コードを追加してください。

	return 0;
}

void CTrackingCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect);
	mRect = rect;
	mRect.InflateRect(-2, -2);
}

BOOL CTrackingCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(&rect, ::GetSysColor(COLOR_WINDOW));
	return TRUE;
}

void CTrackingCtrl::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CDC memDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap = NULL;
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	pOldBitmap = memDC.SelectObject(&bitmap);

	CBrush brFrame(GetSysColor(COLOR_3DFACE));
	CBrush brBack(RGB(0,0,0));
	memDC.FillRect(rect, &brFrame);

	CRect rc = rect;
	int len = __min(rc.Width(), rc.Height());
	rc.right = rc.left + len;
	rc.bottom = rc.top + len;
	rc.left = rc.left + ((rect.Width()-rc.Width()) / 2);
	rc.top = rc.top + ((rect.Height()-rc.Height()) / 2);
	rc.right = rc.left + len;
	rc.bottom = rc.top + len;
	rc.DeflateRect(20, 20);
	memDC.Ellipse(rc);
	CRect linerect = rc;
	rc.InflateRect(-(linerect.Width() / 8), -(linerect.Height() / 8));
	memDC.Ellipse(rc);
	rc = linerect;
	rc.InflateRect(-(linerect.Width() / 4), -(linerect.Height() / 4));
	memDC.Ellipse(rc);
	memDC.MoveTo(linerect.left + linerect.Width() / 2, linerect.top);
	memDC.LineTo(linerect.left + linerect.Width() / 2, linerect.bottom);
	memDC.MoveTo(linerect.left, linerect.top + linerect.Height() / 2);
	memDC.LineTo(linerect.right, linerect.top + linerect.Height() / 2);

	CFont defFont, txtFont;
	CFont* oldFont = NULL;
	defFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (defFont.GetLogFont(&lf)){
		lf.lfWeight = FW_NORMAL;
		lf.lfHeight = 14;
		txtFont.CreateFontIndirect(&lf);
		oldFont = memDC.SelectObject(&txtFont);
	}

	COLORREF color = memDC.SetTextColor(RGB(0, 0, 0));
	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(color);

	rc = linerect;
	rc.left = rc.left + (rc.Width() / 2) - 20;
	rc.right = rc.left + 40;
	rc.bottom = rc.top;
	rc.top = rc.top - 20;
	memDC.DrawText(_T("N"), rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	rc = linerect;
	rc.left = rc.left + (rc.Width() / 2) - 20;
	rc.right = rc.left + 40;
	rc.top = rc.bottom;
	rc.bottom = rc.top + 20;
	memDC.DrawText(_T("S"), rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	rc = linerect;
	rc.right = rc.left;
	rc.left = rc.left - 20;
	rc.top = rc.top + (rc.Height() / 2) - 20;
	rc.bottom = rc.top + 40;
	memDC.DrawText(_T("W"), rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	rc = linerect;
	rc.left = rc.right;
	rc.right = rc.left + 20;
	rc.top = rc.top + (rc.Height() / 2) - 20;
	rc.bottom = rc.top + 40;
	memDC.DrawText(_T("E"), rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SetTextColor(color);
	if (pOldBitmap != NULL)
		memDC.SelectObject(pOldBitmap);

	if (bitmap.m_hObject != NULL)
		bitmap.DeleteObject();

	defFont.DeleteObject();
	if (oldFont != NULL){
		memDC.SelectObject(oldFont);
		txtFont.DeleteObject();
	}
	memDC.DeleteDC();
}

void CTrackingCtrl::PostNcDestroy()
{
	CWnd::PostNcDestroy();
}

/*============================================================================*/
/*! 分割グリッドコントロール
	角度を正常化する

@param	angle
@param

@retval double		正常化した角度
*/
/*============================================================================*/
double CTrackingCtrl::NormalizeAngle(const double angle)
{
	if (angle < 0.0)
		return angle + 360.0;
	if (angle > 359.9)
		return angle - 360.0;

	return angle;
}
