/*============================================================================*/
/*! RangePropagationView.cpp

-# レンジ伝搬状況モニタ画面　伝搬グラフビュー
*/
/*============================================================================*/
// RangePropagationView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "RangePropagationView.h"
#include "RangePropagationStatusMonitor.h"

//----------------------------------------------------------------------
// 定義

#define YOFFSET_RIGHT_ARROW		30
#define YOFFSET_LEFT_ARROW		56
#define XOFFSET_ARROW			20
#define YOFFSET_RIGHT_ARROW_STR	10
#define YOFFSET_LEFT_ARROW_STR	66

POINT   ptArrow[3]    = { 0, 0, 20, 7, 0, 14 };
POINT   ptArrowRvs[3] = { 20, 0, 0, 7, 20, 14 };

// CRangePropagationView

IMPLEMENT_DYNCREATE(CRangePropagationView, CView)

/*============================================================================*/
/*! CRangePropagationView

-# コンストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CRangePropagationView::CRangePropagationView()
: m_colorStop(RGB(128, 128, 128))
, m_colorStart(RGB(0, 191, 255))
, m_colorText(RGB(0, 0, 0))
{
	m_txtFont.CreateStockObject(DEFAULT_GUI_FONT);
}

/*============================================================================*/
/*! CRangePropagationView

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CRangePropagationView::~CRangePropagationView()
{
}

BEGIN_MESSAGE_MAP(CRangePropagationView, CView)
	ON_WM_PAINT()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CRangePropagationView 描画

/*============================================================================*/
/*! CRangePropagationView

-# 描画時に呼ばれる

@param  pDC：ドキュメントのデバイスコンテキストポインタ
@retval なし
*/
/*============================================================================*/
void CRangePropagationView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 描画コードをここに追加してください。
}


// CRangePropagationView 診断

#ifdef _DEBUG
void CRangePropagationView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CRangePropagationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRangePropagationView メッセージ ハンドラー

/*============================================================================*/
/*! CRangePropagationView

-# グリッド線の描画

@param	memDC		ドキュメント
@param	rectClient	描画領域
@retval	TRUE:グリッド線を描画した / FALSE:グリッド線を描画しなかった

*/
/*============================================================================*/

BOOL CRangePropagationView::DrawArrow(CDC& memDC, CRect& rectClient)
{
	CPen penSolid;
	CPen penArrow;
	CBrush brush;
	CPen* oldPen;
	CBrush* oldBrush;
	COLORREF	color;
	POINT   points[3];

	CGraphMutex::Lock(eRngProp);

	// 伝搬データを取得
	CRangePropagation& rangePropagation = theApp.GetSatelliteData().GetRangePropagation();
	deque<CRangePropagation::structPropData>& dequeRngSnd = rangePropagation.GetRngSndData();
	deque<CRangePropagation::structPropData>& dequeRngRcv = rangePropagation.GetRngRcvData();
	UINT nStatus = rangePropagation.GetRngStatus();

	memDC.SetBkMode(TRANSPARENT);

	// 送り
	if (nStatus == CRangePropagation::eStart && dequeRngSnd.empty() == false)
	{
		color = m_colorStart;
	}
	else
	{
		color = m_colorStop;
	}
	penSolid.CreatePen(PS_SOLID, 5, color);
	penArrow.CreatePen(PS_SOLID, 1, color);
	brush.CreateSolidBrush(color);
	oldPen = memDC.SelectObject(&penSolid);
	oldBrush = memDC.SelectObject(&brush);
	memDC.MoveTo(rectClient.left, rectClient.top + YOFFSET_RIGHT_ARROW);
	memDC.LineTo(rectClient.right - XOFFSET_ARROW, rectClient.top + YOFFSET_RIGHT_ARROW);
	memDC.SelectObject(&penArrow);
	for (int i = 0; i < 3; ++i)
	{
		points[i].x = ptArrow[i].x + rectClient.right - XOFFSET_ARROW;
		points[i].y = ptArrow[i].y + YOFFSET_RIGHT_ARROW - 7;
	}
	memDC.Polygon(points, 3);
	penSolid.DeleteObject();
	penArrow.DeleteObject();
	brush.DeleteObject();

	// 戻り
	if (nStatus == CRangePropagation::eStart && dequeRngRcv.empty() == false)
	{
		color = m_colorStart;
	}
	else
	{
		color = m_colorStop;
	}
	penSolid.CreatePen(PS_SOLID, 5, color);
	penArrow.CreatePen(PS_SOLID, 1, color);
	brush.CreateSolidBrush(color);
	memDC.SelectObject(&penSolid);
	memDC.SelectObject(&brush);
	memDC.MoveTo(rectClient.left + XOFFSET_ARROW, rectClient.top + YOFFSET_LEFT_ARROW);
	memDC.LineTo(rectClient.right, rectClient.top + YOFFSET_LEFT_ARROW);
	memDC.SelectObject(&penArrow);
	for (int i = 0; i < 3; ++i)
	{
		points[i].x = ptArrowRvs[i].x + rectClient.left;
		points[i].y = ptArrowRvs[i].y + YOFFSET_LEFT_ARROW - 7;
	}
	memDC.Polygon(points, 3);
	penSolid.DeleteObject();
	penArrow.DeleteObject();
	brush.DeleteObject();
	memDC.SelectObject(oldPen);
	memDC.SelectObject(oldBrush);

	CGraphMutex::Unlock(eRngProp);

	return TRUE;
}


/*============================================================================*/
/*! CRangePropagationView

-# 伝搬データの描画

@param	memDC		ドキュメント
@param	rectClient	描画領域
@retval	TRUE:グリッド線を描画した / FALSE:グリッド線を描画しなかった

*/
/*============================================================================*/
BOOL CRangePropagationView::DrawPropagation(CDC& memDC, CRect& rectClient)
{
	CFont* oldFont = memDC.SelectObject(&m_txtFont);
	memDC.SetTextColor(m_colorText);
	memDC.SetBkMode(TRANSPARENT);

	CGraphMutex::Lock(eRngProp);

	// 伝搬データを取得
	CRangePropagation& rangePropagation = theApp.GetSatelliteData().GetRangePropagation();
	deque<CRangePropagation::structPropData>& dequeRngSnd = rangePropagation.GetRngSndData();
	deque<CRangePropagation::structPropData>& dequeRngRcv = rangePropagation.GetRngRcvData();

	deque<CRangePropagation::structPropData>::iterator itr;

	int xpos;
	CString str;
	UINT nGeneMode = rangePropagation.GetGeneMode();
	UINT nRngNum = rangePropagation.GetRngNum();
	// アンテナ->衛星間の横位置
	for (itr = dequeRngSnd.begin(); itr != dequeRngSnd.end(); itr++)
	{
		// 実際の距離を画面位置に変換する
		xpos = (int)((rectClient.Width() - XOFFSET_ARROW * 3) * (*itr).pos / rangePropagation.GetRange());
		if (nGeneMode == eRngGeneration_Past)
		{
			str.Format(_T("%u>>"), (*itr).count);
		}
		else
		{
			str.Format(_T(">>"));
		}
		memDC.TextOutW(xpos + XOFFSET_ARROW, rectClient.top + YOFFSET_RIGHT_ARROW_STR, str);
	}

	// アンテナ<-衛星間の横位置
	for (itr = dequeRngRcv.begin(); itr != dequeRngRcv.end(); itr++)
	{
		// 実際の距離を画面位置に変換する
		xpos = (int)((rectClient.Width() - XOFFSET_ARROW * 3) * (*itr).pos / rangePropagation.GetRange());
		if (nGeneMode == eRngGeneration_Past)
		{
			str.Format(_T("<<%u"), (*itr).count);
		}
		else
		{
			str.Format(_T("<<"));
		}
		memDC.TextOutW(xpos + XOFFSET_ARROW, rectClient.top + YOFFSET_LEFT_ARROW_STR, str);
	}

	CGraphMutex::Unlock(eRngProp);

	memDC.SelectObject(oldFont);

	return TRUE;
}


/*============================================================================*/
/*! CRangePropagationView

-# 描画時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CRangePropagationView::OnPaint()
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
	COLORREF colorBk = GetSysColor(COLOR_3DFACE);
	memDC.SetBkColor(colorBk);
	CBrush brush(colorBk);
	// 全体を塗りつぶし
	memDC.FillRect(rectClient, &brush);
	// デバイス コンテキストのビューポートの原点を設定
	memDC.SetViewportOrg(0, 0);

	// 矢印の描画
	DrawArrow(memDC, rectClient);

	// 伝搬データの描画
	DrawPropagation(memDC, rectClient);

	dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	bitm.DeleteObject();
	memDC.DeleteDC();
	brush.DeleteObject();
}


/*============================================================================*/
/*! CRangePropagationView

-# 非アクティブ ウィンドウにユーザーがマウス ボタンを押したときに呼び出される

@param  pDesktopWnd：アクティブになるウィンドウのトップレベルの親ウィンドウへのポインタ
@param	nHitTest：カーソル位置テストの領域コード
@param	message：マウスメッセージ番号
@retval ウィンドウアクティブ/破棄結果
*/
/*============================================================================*/
int CRangePropagationView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	return 0;
//	return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

#if 0
/*============================================================================*/
/*! CRangePropagationView

-# 指定数値を参照数値と同じ桁まで0で埋めて文字列を作る(4桁まで対応）

@param  指定数値
@param  参照数値
@param  文字列
@retval なし
*/
/*============================================================================*/
void CRangePropagationView::AddZeroString(UINT val, UINT refVal, CString& str)
{
	if (refVal >= 1000)
	{
		str.Format(_T("%04u>>"), val);
	}
	else if (refVal >= 100)
	{
		str.Format(_T("%03u>>"), val);
	}
	else if (refVal >= 10)
	{
		str.Format(_T("%02u>>"), val);
	}
	else
	{
		str.Format(_T("%01u>>"), val);
	}
}
#endif
