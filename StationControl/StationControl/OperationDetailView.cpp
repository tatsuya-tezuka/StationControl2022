/*============================================================================*/
/*! 運用詳細

-#

@param
@retval

*/
/*============================================================================*/
// OperationDetailView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "OperationDetailView.h"


// COperationDetailView

IMPLEMENT_DYNCREATE(COperationDetailView, CView)

COperationDetailView::COperationDetailView()
{

}

COperationDetailView::~COperationDetailView()
{
}

BEGIN_MESSAGE_MAP(COperationDetailView, CView)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// COperationDetailView 描画

void COperationDetailView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 描画コードをここに追加してください。
}


// COperationDetailView 診断

#ifdef _DEBUG
void COperationDetailView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void COperationDetailView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
CStationControlDoc* COperationDetailView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


// COperationDetailView メッセージ ハンドラー


void COperationDetailView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	GetDocument()->SetTitleEx();
}


void COperationDetailView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	// 描画メッセージで CView::OnPaint() を呼び出さないでください。

	return;

	CRect rect;
	GetClientRect(rect);

	CDC memDC;
	CBitmap bitm;
	// 描画ＤＣの作成
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	CBitmap* pOldBitmap = NULL;
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	pOldBitmap = memDC.SelectObject(&bitmap);

	COLORREF color = memDC.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	// バックグラウンド モードを設定
	memDC.SetBkMode(TRANSPARENT);
	// 背景色の設定
	memDC.FillSolidRect(&rect, ::GetSysColor(COLOR_WINDOW));
	// デフォルトフォントの設定
	CFont txtFont;
	txtFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (txtFont.GetLogFont(&lf)){
		lf.lfHeight = 12;
		txtFont.DeleteObject();
		txtFont.CreateFontIndirect(&lf);
	}
	CFont* oldFont = memDC.SelectObject(&txtFont);
	CSize sizeText = memDC.GetTextExtent(_T("W"));

	CRect rc1 = rect;
	CRect rc2 = rect;
	CRect rc3 = rect;

	rc1.left = 10;
	rc2.left = 10;
	rc3.left = 10;
	rc1.top = 10;
	rc2.top = 10;
	rc3.top = 10;

	sizeText.cy += 2;
	rc2.left += memDC.GetTextExtent(_T("制御開始  ")).cx;
	rc3.left += memDC.GetTextExtent(_T("制御開始  :  ")).cx;

	memDC.TextOut(rc1.left, rc1.top, _T("制御開始"));
	memDC.TextOut(rc2.left, rc2.top, _T(":"));
	memDC.TextOut(rc3.left, rc3.top, _T("CCYY/MM/DD hh:mm:ss"));
	rc1.top += sizeText.cy;	rc1.bottom += sizeText.cy;
	rc2.top += sizeText.cy;	rc2.bottom += sizeText.cy;
	rc3.top += sizeText.cy;	rc3.bottom += sizeText.cy;
	memDC.TextOut(rc1.left, rc1.top, _T("運用開始"));
	memDC.TextOut(rc2.left, rc2.top, _T(":"));
	memDC.TextOut(rc3.left, rc3.top, _T("CCYY/MM/DD hh:mm:ss"));
	rc1.top += sizeText.cy;	rc1.bottom += sizeText.cy;
	rc2.top += sizeText.cy;	rc2.bottom += sizeText.cy;
	rc3.top += sizeText.cy;	rc3.bottom += sizeText.cy;
	memDC.TextOut(rc1.left, rc1.top, _T("U/L STA"));
	memDC.TextOut(rc2.left, rc2.top, _T(":"));
	memDC.TextOut(rc3.left, rc3.top, _T("CCYY/MM/DD hh:mm:ss"));
	rc1.top += sizeText.cy;	rc1.bottom += sizeText.cy;
	rc2.top += sizeText.cy;	rc2.bottom += sizeText.cy;
	rc3.top += sizeText.cy;	rc3.bottom += sizeText.cy;
	memDC.TextOut(rc1.left, rc1.top, _T("CMD AOS"));
	memDC.TextOut(rc2.left, rc2.top, _T(":"));
	memDC.TextOut(rc3.left, rc3.top, _T("CCYY/MM/DD hh:mm:ss"));
	rc1.top += sizeText.cy;	rc1.bottom += sizeText.cy;
	rc2.top += sizeText.cy;	rc2.bottom += sizeText.cy;
	rc3.top += sizeText.cy;	rc3.bottom += sizeText.cy;
	memDC.TextOut(rc1.left, rc1.top, _T("CMD LOS"));
	memDC.TextOut(rc2.left, rc2.top, _T(":"));
	memDC.TextOut(rc3.left, rc3.top, _T("CCYY/MM/DD hh:mm:ss"));
	rc1.top += sizeText.cy;	rc1.bottom += sizeText.cy;
	rc2.top += sizeText.cy;	rc2.bottom += sizeText.cy;
	rc3.top += sizeText.cy;	rc3.bottom += sizeText.cy;
	memDC.TextOut(rc1.left, rc1.top, _T("U/L END"));
	memDC.TextOut(rc2.left, rc2.top, _T(":"));
	memDC.TextOut(rc3.left, rc3.top, _T("CCYY/MM/DD hh:mm:ss"));
	rc1.top += sizeText.cy;	rc1.bottom += sizeText.cy;
	rc2.top += sizeText.cy;	rc2.bottom += sizeText.cy;
	rc3.top += sizeText.cy;	rc3.bottom += sizeText.cy;
	memDC.TextOut(rc1.left, rc1.top, _T("運用終了"));
	memDC.TextOut(rc2.left, rc2.top, _T(":"));
	memDC.TextOut(rc3.left, rc3.top, _T("CCYY/MM/DD hh:mm:ss"));

	rc1.top += (sizeText.cy*3);	rc1.bottom += (sizeText.cy*3);
	rc2.top += (sizeText.cy * 3);	rc2.bottom += (sizeText.cy * 3);
	rc3.top += (sizeText.cy * 3);	rc3.bottom += (sizeText.cy * 3);
	memDC.TextOut(rc1.left, rc1.top, _T("LOCK判定"));
	rc1.top += sizeText.cy;	rc1.bottom += sizeText.cy;
	rc2.top += sizeText.cy;	rc2.bottom += sizeText.cy;
	rc3.top += sizeText.cy;	rc3.bottom += sizeText.cy;
	memDC.TextOut(rc1.left, rc1.top, _T("疑似補足"));

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

}
