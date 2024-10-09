// DrawObject.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"

#include "DrawObject.h"

// CDrawObject

IMPLEMENT_SERIAL(CDrawObject, CObject, 0)

CDrawObject::CDrawObject()
{

}

CDrawObject::~CDrawObject()
{
}

CDrawObject::CDrawObject(const CRect& position)
{
	TRACE("### (%d,%d) - (%d,%d)\n", position.left, position.top, position.right, position.bottom);
	m_position = position;

	m_bPen = TRUE;
	m_logpen.lopnStyle = PS_INSIDEFRAME;
	m_logpen.lopnWidth.x = 1;
	m_logpen.lopnWidth.y = 1;
	m_logpen.lopnColor = RGB(0, 255, 255);

	m_bBrush = FALSE;
	m_logbrush.lbStyle = BS_SOLID;
	m_logbrush.lbColor = RGB(192, 192, 192);
	m_logbrush.lbHatch = HS_HORIZONTAL;
}

void CDrawObject::Remove()
{
	delete this;
}

/*============================================================================*/
/*! イメージツール

	-# オブジェクトのハンドル（■点）数の取得

	@param
	@retval

*/
/*============================================================================*/
int CDrawObject::GetHandleCount()
{
	ASSERT_VALID(this);
	return 8;
}

/*============================================================================*/
/*! イメージツール

	-# オブジェクトのハンドル（■点）位置の取得

	@param
	@retval

*/
/*============================================================================*/
CPoint CDrawObject::GetHandle(int nHandle)
{
	ASSERT_VALID(this);
	int x, y, xCenter, yCenter;

	xCenter = m_position.left + m_position.Width() / 2;
	yCenter = m_position.top + m_position.Height() / 2;

#if	1
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
		x = m_position.right;
		y = m_position.bottom;
		break;

	case 2:
		x = xCenter;
		y = m_position.bottom;
		break;

	case 3:
		x = m_position.left;
		y = m_position.bottom;
		break;

	case 4:
		x = m_position.left;
		y = yCenter;
		break;

	case 5:
		x = m_position.left;
		y = m_position.top;
		break;

	case 6:
		x = xCenter;
		y = m_position.top;
		break;

	case 7:
		x = m_position.right;
		y = m_position.top;
		break;

	case 8:
		x = m_position.right;
		y = yCenter;
		break;
	}
#else
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
		x = m_position.left;
		y = m_position.top;
		break;

	case 2:
		x = xCenter;
		y = m_position.top;
		break;

	case 3:
		x = m_position.right;
		y = m_position.top;
		break;

	case 4:
		x = m_position.right;
		y = yCenter;
		break;

	case 5:
		x = m_position.right;
		y = m_position.bottom;
		break;

	case 6:
		x = xCenter;
		y = m_position.bottom;
		break;

	case 7:
		x = m_position.left;
		y = m_position.bottom;
		break;

	case 8:
		x = m_position.left;
		y = yCenter;
		break;
	}
#endif
	return CPoint(x, y);
}

/*============================================================================*/
/*! イメージツール

	-# オブジェクトのハンドル（■点）矩形の取得

	@param
	@retval

*/
/*============================================================================*/
CRect CDrawObject::GetHandleRect(int nHandleID, CGraphWindow* pView)
{
	ASSERT_VALID(this);
	ENSURE(pView != NULL);

	CRect rect;
	CPoint point = GetHandle(nHandleID);
	pView->DocToClient(point);
	rect.SetRect(point.x-3, point.y-3, point.x+3, point.y+3);
	pView->ClientToDoc(rect);

	return rect;
}

/*============================================================================*/
/*! イメージツール

	-# オブジェクトのハンドル（■点）カーソルの取得

	@param
	@retval

*/
/*============================================================================*/
HCURSOR CDrawObject::GetHandleCursor(int nHandle)
{
	ASSERT_VALID(this);

	LPCTSTR id;
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
	case 5:
		id = IDC_SIZENWSE;
		break;

	case 2:
	case 6:
		id = IDC_SIZENS;
		break;

	case 3:
	case 7:
		id = IDC_SIZENESW;
		break;

	case 4:
	case 8:
		id = IDC_SIZEWE;
		break;
	}

	return AfxGetApp()->LoadStandardCursor(id);
}

/*============================================================================*/
/*! イメージツール

	-# 線色の設定

	@param
	@retval

*/
/*============================================================================*/
void CDrawObject::SetLineColor(COLORREF color, BOOL bPreview)
{
	ASSERT_VALID(this);

	m_logpen.lopnColor = color;

	Invalidate();
}

/*============================================================================*/
/*! イメージツール

	-# オブジェクト塗り潰し色の設定

	@param
	@retval

*/
/*============================================================================*/
void CDrawObject::SetFillColor(COLORREF color, BOOL bPreview)
{
	ASSERT_VALID(this);

	m_logbrush.lbColor = color;

	Invalidate();
}

void CDrawObject::Draw(CDC* pDC, BOOL bEditMode)
{
}

/*============================================================================*/
/*! イメージツール

	-# オブジェクトの拡大縮小

	@param
	@retval

*/
/*============================================================================*/
void CDrawObject::MoveTo(const CRect& position, CGraphWindow* pView)
{
	ASSERT_VALID(this);

	// ★
	//if(position == m_position)
	//	return;

	if(pView == NULL){
		Invalidate();
		m_position = position;
		Invalidate();
	} else{
		CRect rect;
		pView->GetClientRect(rect);
		if(rect.left > position.left || rect.right < position.right)
			return;
		pView->InvalObj(this);
		m_position = position;
		pView->ClientToDoc(m_position);
		pView->InvalObj(this);
	}
	TRACE("### (%d,%d) - (%d,%d)\n", position.left, position.top, position.right, position.bottom);
}

/*============================================================================*/
/*! イメージツール

	-# 位置情報からハンドル番号を取得する

	@param
	@retval

*/
/*============================================================================*/
int CDrawObject::HitTest(CPoint point, CGraphWindow* pView, BOOL bSelected)
{
	ASSERT_VALID(this);
	ASSERT(pView != NULL);

	if(bSelected){
		CRect rect = m_position;
		pView->DocToClient(rect);
		if(rect.PtInRect(point) == TRUE){
			return 0;
		} else{
			if(rect.left == rect.right && rect.left == point.x){
				return 0;
			}
		}
	} else{
		if(point.x >= m_position.left && point.x < m_position.right && point.y >= m_position.top && point.y < m_position.bottom)
			return 1;
	}
	return -1;
}

/*============================================================================*/
/*! イメージツール

	-# 矩形情報がオブジェクトの矩形と交差しているかを求める

	@param
	@retval

*/
/*============================================================================*/
BOOL CDrawObject::Intersects(const CRect& rect)
{
	ASSERT_VALID(this);

	CRect fixed = m_position;
	fixed.NormalizeRect();
	CRect rectT = rect;
	rectT.NormalizeRect();
	return !(rectT & fixed).IsRectEmpty();
}

/*============================================================================*/
/*! イメージツール

	-# オブジェクトのハンドル（■点）の移動

	@param
	@retval

*/
/*============================================================================*/
void CDrawObject::MoveHandleTo(int nHandle, CPoint point, CGraphWindow* pView)
{
	ASSERT_VALID(this);

	CRect position = m_position;
#if	1
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
		position.right = point.x;
		position.bottom = point.y;
		break;

	case 2:
		position.bottom = point.y;
		break;

	case 3:
		position.left = point.x;
		position.bottom = point.y;
		break;

	case 4:
		position.left = point.x;
		break;

	case 5:
		position.left = point.x;
		position.top = point.y;
		break;

	case 6:
		position.top = point.y;
		break;

	case 7:
		position.right = point.x;
		position.top = point.y;
		break;

	case 8:
		position.right = point.x;
		break;
	}
#else
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
		position.left = point.x;
		position.top = point.y;
		break;

	case 2:
		position.top = point.y;
		break;

	case 3:
		position.right = point.x;
		position.top = point.y;
		break;

	case 4:
		position.right = point.x;
		break;

	case 5:
		position.right = point.x;
		position.bottom = point.y;
		break;

	case 6:
		position.bottom = point.y;
		break;

	case 7:
		position.left = point.x;
		position.bottom = point.y;
		break;

	case 8:
		position.left = point.x;
		break;
	}
#endif
	MoveTo(position, pView);
}

/*============================================================================*/
/*! イメージツール

	-# オブジェクトの再描画

	@param
	@retval

*/
/*============================================================================*/
void CDrawObject::Invalidate()
{
	ASSERT_VALID(this);
}


////////////////////////////////////////////////////////////////////////////
// CDrawRect

IMPLEMENT_SERIAL(CDrawRect, CDrawObject, 0)

CDrawRect::CDrawRect()
{
}

CDrawRect::CDrawRect(const CRect& position)
: CDrawObject(position)
{
	ASSERT_VALID(this);

	m_nShape		= rectangle;
	m_textColor		= RGB(0,0,0);
	m_backColor		= RGB(128,128,128);
}

/*============================================================================*/
/*! イメージツール

	-# オブジェクトのハンドル（■点）数の取得

	@param
	@retval

*/
/*============================================================================*/
int CDrawRect::GetHandleCount()
{
	ASSERT_VALID(this);

	return CDrawObject::GetHandleCount();
}

/*============================================================================*/
/*! イメージツール

	-# オブジェクトのハンドル（■点）位置の取得

	@param
	@retval

*/
/*============================================================================*/
CPoint CDrawRect::GetHandle(int nHandle)
{
	ASSERT_VALID(this);

	return CDrawObject::GetHandle(nHandle);
}

/*============================================================================*/
/*! イメージツール

	-# オブジェクトのハンドル（■点）カーソルの取得

	@param
	@retval

*/
/*============================================================================*/
HCURSOR CDrawRect::GetHandleCursor(int nHandle)
{
	ASSERT_VALID(this);

	return AfxGetApp()->LoadStandardCursor(IDC_HAND);

	return CDrawObject::GetHandleCursor(nHandle);
}

/*============================================================================*/
/*! イメージツール

	-# 矩形情報がオブジェクトの矩形と交差しているかを求める

	@param
	@retval

*/
/*============================================================================*/
BOOL CDrawRect::Intersects(const CRect& rect)
{
	ASSERT_VALID(this);

	CRect rectT = rect;
	rectT.NormalizeRect();

	CRect fixed = m_position;
	fixed.NormalizeRect();

	//if ((rectT & fixed).IsRectEmpty())
	//	return FALSE;
	if(fixed.left == fixed.right && fixed.left == rectT.left){
	} else{
		if ((rectT & fixed).IsRectEmpty())
			return FALSE;
	}

	CRgn rgn;
	switch (m_nShape)
	{
	case rectangle:
		return TRUE;

	case line:
		return TRUE;
	}
	return rgn.RectInRegion(fixed);
}

/*============================================================================*/
/*! イメージツール

	-# オブジェクトのハンドル（■点）の移動

	@param
	@retval

*/
/*============================================================================*/
void CDrawRect::MoveHandleTo(int nHandle, CPoint point, CGraphWindow* pView)
{
	ASSERT_VALID(this);

	CDrawObject::MoveHandleTo(nHandle, point, pView);
}

/*============================================================================*/
/*! イメージツール

	-# オブジェクト描画

	@param
	@retval

*/
/*============================================================================*/
void CDrawRect::Draw(CDC* pDC, BOOL bEditMode)
{
	ASSERT_VALID(this);

	CBrush brush;
	m_logbrush.lbColor = RGB(0,0,0);
	if (!brush.CreateBrushIndirect(&m_logbrush))
		return;
	CPen pen, pens, penline;
	if (!pen.CreatePenIndirect(&m_logpen))
		return;

	CBrush* pOldBrush;
	CPen* pOldPen;

	if (m_bBrush)
		pOldBrush = pDC->SelectObject(&brush);
	else
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	if (m_bPen)
		pOldPen = pDC->SelectObject(&pen);
	else
		pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);

	CRect rect = m_position;
	switch (m_nShape)
	{
	case rectangle:
		if(rect.left == rect.right){
			pDC->MoveTo(rect.left, rect.top);
			pDC->LineTo(rect.right, rect.bottom);
		} else{
			pDC->Rectangle(rect);
		}
		//rect.DeflateRect(1,1);
		//pens.CreatePen(PS_SOLID, 1, RGB(128,128,128));
		//pDC->SelectObject(&pens);
		//pDC->Rectangle(rect);
		//pens.DeleteObject();
		break;

	case line:
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		penline.CreatePen(PS_SOLID, 1, RGB(255,128,64));
		pOldPen = pDC->SelectObject(&penline);
		pDC->MoveTo(rect.left, rect.top);
		pDC->LineTo(rect.right, rect.bottom);
		penline.DeleteObject();
		break;
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}
