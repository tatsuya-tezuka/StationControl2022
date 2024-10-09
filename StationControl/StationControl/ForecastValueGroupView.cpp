/*============================================================================*/
/*! ForecastValueGroupView.cpp

-# 予報値一覧画面　予報値リストのグループビュー
*/
/*============================================================================*/
// ForecastValueGroupView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ForecastValueGroupView.h"
#include "ForecastValue.h"


// CForecastValueGroupView

IMPLEMENT_DYNCREATE(CForecastValueGroupView, CView)

CForecastValueGroupView::CForecastValueGroupView()
: m_colorText(RGB(0, 0, 0))
{
	m_txtFont.CreateStockObject(DEFAULT_GUI_FONT);
}

CForecastValueGroupView::~CForecastValueGroupView()
{
}

BEGIN_MESSAGE_MAP(CForecastValueGroupView, CView)
	ON_WM_PAINT()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CForecastValueGroupView 描画

void CForecastValueGroupView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 描画コードをここに追加してください。
}


// CForecastValueGroupView 診断

#ifdef _DEBUG
void CForecastValueGroupView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CForecastValueGroupView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForecastValueGroupView メッセージ ハンドラー


void CForecastValueGroupView::OnPaint()
{
	CRect rectClient;
	CPaintDC dc(this); // device context for painting

	// クライアント領域の取得
	GetClientRect(rectClient);

	CDC memDC;
	CBitmap bitm;
	// 描画DCの作成
	memDC.CreateCompatibleDC(&dc);

	// 描画用ビットマップの作成
	bitm.CreateCompatibleBitmap(&dc, rectClient.Width(), rectClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitm);

	// バックグラウンド モードを設定
	memDC.SetBkMode(OPAQUE);
	// ウィンドウの背景色を取得
	COLORREF colorBk = GetSysColor(COLOR_WINDOW);
	memDC.SetBkColor(colorBk);
	CBrush brush(colorBk);
	// 全体を塗りつぶし
	memDC.FillRect(rectClient, &brush);
	// デバイス コンテキストのビューポートの原点を設定
	memDC.SetViewportOrg(0, 0);

	// AOS, LOS, MAX ELのグルーピング描画
	DrawGroup(memDC, rectClient);

	dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	bitm.DeleteObject();
	memDC.DeleteDC();
	brush.DeleteObject();
}

/*============================================================================*/
/*! 予報値グループ描画

-# グルーピング描画

@param	memDC		ドキュメント
@param	rectClient	描画領域
@retval	TRUE:グリッド線を描画した / FALSE:グリッド線を描画しなかった

*/
/*============================================================================*/
BOOL CForecastValueGroupView::DrawGroup(CDC& memDC, CRect& rectClient)
{
	CForecastValue* pWnd = (CForecastValue*)GetParent();
	if (pWnd == NULL)
	{
		return FALSE;
	}

	CFont* oldFont = memDC.SelectObject(&m_txtFont);
	memDC.SetTextColor(m_colorText);
	memDC.SetBkMode(TRANSPARENT);

	CForecastValue::structGroupXPos stXPos;
	CString str, strOut;
	CSize sz;

	// 親のリストコントロールからAOSの表示開始位置と終了位置を取得
	pWnd->GetAosXPos(stXPos);
	str.Format(_T("AOS"));
	strOut = _T("");
	sz = memDC.GetTextExtent(str);

	while (sz.cx + m_nLeftOffset + m_nRightOffset < stXPos.x2 - stXPos.x1)
	{
		strOut = str;
		str = _T("-") + str + _T("-");
		sz = memDC.GetTextExtent(str);
	}

	memDC.TextOutW(rectClient.left + m_nLeftOffset + stXPos.x1, rectClient.top, strOut);

	// 親のリストコントロールからAOSの表示開始位置と終了位置を取得
	pWnd->GetLosXPos(stXPos);
	str.Format(_T("LOS"));
	strOut = _T("");
	sz = memDC.GetTextExtent(str);

	while (sz.cx + m_nLeftOffset + m_nRightOffset < stXPos.x2 - stXPos.x1)
	{
		strOut = str;
		str = _T("-") + str + _T("-");
		sz = memDC.GetTextExtent(str);
	}

	memDC.TextOutW(rectClient.left + m_nLeftOffset + stXPos.x1, rectClient.top, strOut);

	// 親のリストコントロールからMAX ELの表示開始位置と終了位置を取得
	pWnd->GetMaxElXPos(stXPos);
	str.Format(_T("MAX EL"));
	strOut = _T("");
	sz = memDC.GetTextExtent(str);

	while (sz.cx + m_nLeftOffset + m_nRightOffset < stXPos.x2 - stXPos.x1)
	{
		strOut = str;
		str = _T("-") + str + _T("-");
		sz = memDC.GetTextExtent(str);
	}

	memDC.TextOutW(rectClient.left + m_nLeftOffset + stXPos.x1, rectClient.top, strOut);

	memDC.SelectObject(oldFont);

	return TRUE;
}


int CForecastValueGroupView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	return 0;
//	return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
