// DrawObject.cpp : 実装ファイル
//

#include "stdafx.h"

#include "DrawObject.h"
#include "GraphWindow.h"
#include "RemoteGraph.h"

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
	const LONG mPenWidth = 2;//1;	// ペン幅(Pixel)
//	TRACE("### (%d,%d) - (%d,%d)\n", position.left, position.top, position.right, position.bottom);
	m_position = position;

	m_bPen = TRUE;
	m_logpen.lopnStyle = PS_INSIDEFRAME;
	m_logpen.lopnWidth.x = mPenWidth;
	m_logpen.lopnWidth.y = mPenWidth;
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
//		CRect rect;
//		pView->GetClientRect(rect);
//		if(rect.left > position.left || rect.right < position.right)
//			return;
		pView->InvalObj(this);
		m_position = position;
		pView->ClientToDoc(m_position);
		pView->InvalObj(this);
	}
//	TRACE("### (%d,%d) - (%d,%d)\n", position.left, position.top, position.right, position.bottom);
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
#if 0
		if(rect.PtInRect(point) == TRUE){
			return 0;
		} else{
			//if(rect.left == rect.right && rect.left == point.x){
			if(rect.left == rect.right && (rect.left-5 <= point.x && point.x <= rect.left+2)){	// ラインカーソルのつまむ感度を良くする
				return 0;
			}
		}
#endif
		if(rect.left == rect.right){
			if(rect.left-5 <= point.x && point.x <= rect.left+2){
				return 0;
			}
		} else{
			if((rect.left-5 <= point.x && point.x <= rect.left+2) || (rect.right-5 <= point.x && point.x <= rect.right+2)){
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

	case 9:	// diff用
		if( (pView->m_nDifferMode != DifferNone) && (pView->m_ptDifferData[0] != pView->m_ptDifferData[1]) )
		{
			LONG	add = DIFFER_ADD;

			position = CRect(pView->m_ptDifferData[0], pView->m_ptDifferData[1]);
			position.NormalizeRect();

			if( pView->m_nDifferMode == DifferSameData )
			{
				position.top -= add;
				position.bottom += add;
				position.left -= DIFFER_TEXT;
				position.right += DIFFER_TEXT;
			}
			else
			{
				position.left -= DIFFER_TEXT;
				position.right += DIFFER_TEXT;
				position.top -= 10;
				position.bottom += 10;
			}
		}
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
	//if(fixed.left == fixed.right && fixed.left == rectT.left){
	if(fixed.left == fixed.right && (fixed.left-5 <= rectT.left && rectT.left <= fixed.left+2)){	// ラインカーソルのつまむ感度を良くする
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

	case differ:
		return TRUE;

	case limit_warning_01:
	case limit_warning_02:
	case limit_warning_03:
	case limit_warning_04:
	case limit_warning_05:
	case limit_warning_06:
	case limit_warning_07:
	case limit_warning_08:
	case limit_warning_09:
	case limit_warning_10:
		return TRUE;

	case limit_alarm_01:
	case limit_alarm_02:
	case limit_alarm_03:
	case limit_alarm_04:
	case limit_alarm_05:
	case limit_alarm_06:
	case limit_alarm_07:
	case limit_alarm_08:
	case limit_alarm_09:
	case limit_alarm_10:
		return TRUE;

	case two1:
		return TRUE;

	case two2:
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
	const int mDeflateY = 2;	// 上下に縮小(Pixel)

	ASSERT_VALID(this);

#if 0
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
#endif

	CRect rect = m_position;
	switch (m_nShape)
	{
	case rectangle:
		{
			CPen	penline;
			COLORREF	col = RGB(0, 255, 255);
#if 0
			penline.CreatePenIndirect(&m_logpen);
#else
			penline.CreatePen(PS_SOLID, 1, col);
#endif
			CPen	*pOldPen = pDC->SelectObject(&penline);
//			rect.DeflateRect(0, mDeflateY);
			if(rect.left == rect.right){
				pDC->MoveTo(rect.left, rect.top);
				pDC->LineTo(rect.right, rect.bottom);
			} else{
#if 0
				pDC->Rectangle(rect);
#else
				pDC->MoveTo(rect.left, rect.top);
				pDC->LineTo(rect.left, rect.bottom);
				pDC->LineTo(rect.right, rect.bottom);
				pDC->LineTo(rect.right, rect.top);
				pDC->LineTo(rect.left, rect.top);
				pDC->MoveTo(rect.left - 1, rect.top);
				pDC->LineTo(rect.left - 1, rect.bottom + 1);
				pDC->MoveTo(rect.left + 1, rect.top);
				pDC->LineTo(rect.left + 1, rect.bottom + 1);
				pDC->MoveTo(rect.right + 1, rect.top);
				pDC->LineTo(rect.right + 1, rect.bottom + 1);
				pDC->MoveTo(rect.right - 1, rect.top);
				pDC->LineTo(rect.right - 1, rect.bottom + 1);
#endif
			}
			pDC->SelectObject(pOldPen);
			penline.DeleteObject();
		}
		//rect.DeflateRect(1,1);
		//pens.CreatePen(PS_SOLID, 1, RGB(128,128,128));
		//pDC->SelectObject(&pens);
		//pDC->Rectangle(rect);
		//pens.DeleteObject();
		break;

	case line:
		if( (rect.left >= 0) && (rect.right >= 0) )
		{
			CPen	penline;
			COLORREF	col = RGB(255,128,64);
//			pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
			penline.CreatePen(PS_SOLID, 1, col);
			CPen	*pOldPen = pDC->SelectObject(&penline);
			pDC->MoveTo(rect.left, rect.top);
			pDC->LineTo(rect.right, rect.bottom);
			vector<stSpaceData>&	data = theApp.GetDataCollection().GetTrendCursorDataMaster(((CGraphWindow *)pDC->GetWindow())->GetWindowNo());
			CTime	time = (data.size() != 0) ? CTime(data[0].systime) : CTime(0);
			CString	str = time.Format("%Y/%m/%d") + _T(" ") + time.Format("%H:%M:%S");
			CRect	rc = CRect(rect.left - 45, rect.bottom - 10, rect.right + 45 + 1, rect.bottom);
			CFont*	oldFont = pDC->SelectObject(&((CGraphWindow *)pDC->GetWindow())->GetFont());
			COLORREF oldText = pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(TRANSPARENT);
			pDC->FillSolidRect(rc, col);
			rc.left += 2;
			rc.right -= 2;
			pDC->TextOutW(rc.left, rc.top, str);
			pDC->SetTextColor(oldText);
			pDC->SelectObject(oldFont);
			pDC->SelectObject(pOldPen);
			penline.DeleteObject();
		}
		break;

	case differ:
		if( (((CGraphWindow *)pDC->GetWindow())->m_nDifferMode != DifferNone) &&
			(((CGraphWindow *)pDC->GetWindow())->m_ptDifferData[0] != ((CGraphWindow *)pDC->GetWindow())->m_ptDifferData[1]) )
		{
			CGraphWindow	*pWnd = (CGraphWindow *)pDC->GetWindow();
			LONG	add = DIFFER_ADD;
			CPen	pensolid, penline;
			COLORREF	col = RGB(255, 255, 255);
			pensolid.CreatePen(PS_SOLID, 1, col);
			penline.CreatePen(PS_DASH, 1, col);
			CPen	*pOldPen = pDC->SelectObject(&penline);
			COLORREF	oldBk = pDC->SetBkColor(RGB(0, 0, 0));
			CRect	rc;
//			if( rect.Width() != 0 )
			if( pWnd->m_nDifferMode == DifferSameData )
			{
//				CRect	tmp_rect = rect;
				CRect	tmp_rect = CRect(pWnd->m_ptDifferData[0], pWnd->m_ptDifferData[1]);
				tmp_rect.NormalizeRect();
				tmp_rect.top -= add;
				tmp_rect.bottom += add;

				rc = tmp_rect;
				rc.left += 1;
				rc.right -= 1;
				rc.top = ((tmp_rect.Height() >> 1) + tmp_rect.top - 5);
				rc.bottom = ((tmp_rect.Height() >> 1) + tmp_rect.top + 5);

				// 同一データ
				pDC->SelectObject(&pensolid);
				pDC->MoveTo(tmp_rect.left, tmp_rect.top);
				pDC->LineTo(tmp_rect.left, tmp_rect.bottom + 1);
				pDC->MoveTo(tmp_rect.right, tmp_rect.top);
				pDC->LineTo(tmp_rect.right, tmp_rect.bottom + 1);
				pDC->SelectObject(&penline);
				pDC->MoveTo(tmp_rect.left, (tmp_rect.Height() >> 1) + tmp_rect.top);
				pDC->LineTo(tmp_rect.right + 1, (tmp_rect.Height() >> 1) + tmp_rect.top);
			}
			else
			{
//				CRect	tmp_rect = rect;
				CRect	tmp_rect = CRect(pWnd->m_ptDifferData[0], pWnd->m_ptDifferData[1]);
				tmp_rect.NormalizeRect();

				rc = tmp_rect;
				rc.left -= DIFFER_TEXT;
				rc.right += DIFFER_TEXT;
				rc.top = ((tmp_rect.Height() >> 1) + tmp_rect.top - 5);
				rc.bottom = ((tmp_rect.Height() >> 1) + tmp_rect.top + 5);

				// 同一時刻
				pDC->SelectObject(&pensolid);
				pDC->MoveTo(tmp_rect.left - add, tmp_rect.top);
				pDC->LineTo(tmp_rect.right + add + 1, tmp_rect.top);
				pDC->MoveTo(tmp_rect.left - add, tmp_rect.bottom);
				pDC->LineTo(tmp_rect.right + add + 1, tmp_rect.bottom);
				pDC->SelectObject(&penline);
				pDC->MoveTo(tmp_rect.left, tmp_rect.top);
				pDC->LineTo(tmp_rect.right, tmp_rect.bottom + 1);
			}
			CGraphMutex::Lock_Analysis(pWnd->GetWindowNo());
			vector<stSpaceData>	*analysis_data1, *analysis_data2;
			if( pWnd->GetViewID() == eViewID_1 )
			{
				analysis_data1 = &theApp.GetDataCollection().GetTrendAllData(pWnd->GetWindowNo(), pWnd->m_nDifferDataNo[0]);
				analysis_data2 = &theApp.GetDataCollection().GetTrendAllData(pWnd->GetWindowNo(), pWnd->m_nDifferDataNo[1]);
			}
			else
			{
				analysis_data1 = &theApp.GetDataCollection().GetTrendZoomData(pWnd->GetWindowNo(), pWnd->m_nDifferDataNo[0]);
				analysis_data2 = &theApp.GetDataCollection().GetTrendZoomData(pWnd->GetWindowNo(), pWnd->m_nDifferDataNo[1]);
			}
			if( ((analysis_data1 != NULL) && (analysis_data2 != NULL)) &&
				(((pWnd->m_nDifferDataPos[0] >> 1) < analysis_data1->size()) && ((pWnd->m_nDifferDataPos[1] >> 1) < analysis_data2->size())) )
			{
				double	value2, value1;
				value2 = (*analysis_data2)[pWnd->m_nDifferDataPos[1] >> 1].mmPair.max;
				value1 = (*analysis_data1)[pWnd->m_nDifferDataPos[0] >> 1].mmPair.min;
				CString	str;
				str.Format(_T("%.6f"), value2 - value1);
				CFont*	oldFont = pDC->SelectObject(&pWnd->GetFont());
				COLORREF oldText = pDC->SetTextColor(col);
				pDC->SetBkMode(TRANSPARENT);
				CRect	tmp_rc = rc;
				pDC->DrawText(str, tmp_rc, DT_CENTER | DT_CALCRECT);
				LONG	x_src = (rc.Width() >> 1) + rc.left;
				LONG	y_src = (rc.Height() >> 1) + rc.top;
				rc.left = x_src - (tmp_rc.Width() >> 1) - 3;
				rc.right = x_src + (tmp_rc.Width() >> 1) + 3;
				rc.top = y_src - (tmp_rc.Height() >> 1);
				rc.bottom = y_src + (tmp_rc.Height() >> 1);
				pDC->FillSolidRect(rc, RGB(0, 0, 0));
				pDC->SelectObject(&pensolid);
				pDC->MoveTo(rc.left, rc.top);
				pDC->LineTo(rc.left, rc.bottom);
				pDC->LineTo(rc.right, rc.bottom);
				pDC->LineTo(rc.right, rc.top);
				pDC->LineTo(rc.left, rc.top);
//				pDC->TextOutW(rc.left, rc.top, str);
				pDC->DrawText(str, rc, DT_CENTER);
				pDC->SetTextColor(oldText);
				pDC->SelectObject(oldFont);
			}
			CGraphMutex::Unlock_Analysis(pWnd->GetWindowNo());
			pDC->SelectObject(pOldPen);
			pensolid.DeleteObject();
			penline.DeleteObject();
			pDC->SetBkColor(oldBk);
		}
		break;

	case limit_warning_01:
	case limit_warning_02:
	case limit_warning_03:
	case limit_warning_04:
	case limit_warning_05:
	case limit_warning_06:
	case limit_warning_07:
	case limit_warning_08:
	case limit_warning_09:
	case limit_warning_10:
		{
			CPropertiesData&	prop = theApp.GetPropertiesData(((CGraphWindow *)pDC->GetWindow())->GetWindowNo());

			int	index = m_nShape - limit_warning_01;

			if( (!prop.item[index].isViewMode) || (CString(prop.item[index].dataName) == _T("")) )
				break;

			CRect	rcClient;
			pDC->GetWindow()->GetClientRect(rcClient);

			if( ((rcClient.top <= rect.top) && (rect.top <= rcClient.bottom)) ||
				((rcClient.top <= rect.bottom) && (rect.bottom <= rcClient.bottom)) )
			{
				LONG	ofs = 3;
				LONG	add = 5;
				CPen	pensolid, penline;
				CString	str;
				COLORREF	col = prop.item[index].color;
				COLORREF	oldBk = pDC->SetBkColor(RGB(0, 0, 0));
				pensolid.CreatePen(PS_SOLID, 1, col);
				penline.CreatePen(PS_DASHDOT, 1, col);
				CPen	*pOldPen = pDC->SelectObject(&penline);
				CFont*	oldFont = pDC->SelectObject(&((CGraphWindow *)pDC->GetWindow())->GetFont());
				COLORREF oldText = pDC->SetTextColor(col);
				pDC->SetBkMode(TRANSPARENT);
				if( prop.item[index].isLimitWarningUpper && ((rcClient.top <= rect.top) && (rect.top <= rcClient.bottom)) )
				{
					pDC->SelectObject(&pensolid);
					pDC->MoveTo(rect.left - ofs - add, rect.top - add);
					pDC->LineTo(rect.left - ofs, rect.top);
					pDC->LineTo(rect.left, rect.top);
					pDC->SelectObject(&penline);
					pDC->LineTo(rect.right, rect.top);
					pDC->SelectObject(&pensolid);
					pDC->MoveTo(rect.right + ofs + add, rect.top - add);
					pDC->LineTo(rect.right + ofs, rect.top);
					pDC->LineTo(rect.right -1, rect.top);
					str.Format(_T("w%02d[up]"), index + 1);
					pDC->TextOutW(rect.right + ofs + add + 3, rect.top - 6, str);
				}
				if( prop.item[index].isLimitWarningLower && ((rcClient.top <= rect.bottom) && (rect.bottom <= rcClient.bottom)) )
				{
					pDC->SelectObject(&pensolid);
					pDC->MoveTo(rect.left - ofs - add, rect.bottom + add);
					pDC->LineTo(rect.left - ofs, rect.bottom);
					pDC->LineTo(rect.left, rect.bottom);
					pDC->SelectObject(&penline);
					pDC->LineTo(rect.right, rect.bottom);
					pDC->SelectObject(&pensolid);
					pDC->MoveTo(rect.right + ofs + add, rect.bottom + add);
					pDC->LineTo(rect.right + ofs, rect.bottom);
					pDC->LineTo(rect.right - 1, rect.bottom);
					str.Format(_T("w%02d[lo]"), index + 1);
					pDC->TextOutW(rect.right + ofs + add + 3, rect.bottom - 6, str);
				}
				pDC->SetTextColor(oldText);
				pDC->SelectObject(oldFont);
				pDC->SelectObject(pOldPen);
				pensolid.DeleteObject();
				penline.DeleteObject();
				pDC->SetBkColor(oldBk);
			}
		}
		break;

	case limit_alarm_01:
	case limit_alarm_02:
	case limit_alarm_03:
	case limit_alarm_04:
	case limit_alarm_05:
	case limit_alarm_06:
	case limit_alarm_07:
	case limit_alarm_08:
	case limit_alarm_09:
	case limit_alarm_10:
		{
			CPropertiesData&	prop = theApp.GetPropertiesData(((CGraphWindow *)pDC->GetWindow())->GetWindowNo());

			int	index = m_nShape - limit_alarm_01;

			if( (!prop.item[index].isViewMode) || (CString(prop.item[index].dataName) == _T("")) )
				break;

			CRect	rcClient;
			pDC->GetWindow()->GetClientRect(rcClient);

			if( ((rcClient.top <= rect.top) && (rect.top <= rcClient.bottom)) ||
				((rcClient.top <= rect.bottom) && (rect.bottom <= rcClient.bottom)) )
			{
				LONG	ofs = 3;
				LONG	add = 5;
				CPen	penline;
				CString	str;
				COLORREF	col = prop.item[index].color;
				penline.CreatePen(PS_SOLID, 1, col);
				CPen	*pOldPen = pDC->SelectObject(&penline);
				CFont*	oldFont = pDC->SelectObject(&((CGraphWindow *)pDC->GetWindow())->GetFont());
				COLORREF oldText = pDC->SetTextColor(col);
				pDC->SetBkMode(TRANSPARENT);
				if( prop.item[index].isLimitAlarmUpper && ((rcClient.top <= rect.top) && (rect.top <= rcClient.bottom)) )
				{
					pDC->MoveTo(rect.left - ofs - add, rect.top - add);
					pDC->LineTo(rect.left - ofs, rect.top);
					pDC->LineTo(rect.right + ofs, rect.top);
					pDC->MoveTo(rect.right + ofs + add, rect.top - add);
					pDC->LineTo(rect.right + ofs - 1, rect.top + 1);
					str.Format(_T("a%02d[up]"), index + 1);
					pDC->TextOutW(rect.right + ofs + add + 3, rect.top - 6, str);
				}
				if( prop.item[index].isLimitAlarmLower && ((rcClient.top <= rect.bottom) && (rect.bottom <= rcClient.bottom)) )
				{
					pDC->MoveTo(rect.left - ofs - add, rect.bottom + add);
					pDC->LineTo(rect.left - ofs, rect.bottom);
					pDC->LineTo(rect.right + ofs, rect.bottom);
					pDC->MoveTo(rect.right + ofs + add, rect.bottom + add);
					pDC->LineTo(rect.right + ofs - 1, rect.bottom - 1);
					str.Format(_T("a%02d[lo]"), index + 1);
					pDC->TextOutW(rect.right + ofs + add + 3, rect.bottom - 6, str);
				}
				pDC->SetTextColor(oldText);
				pDC->SelectObject(oldFont);
				pDC->SelectObject(pOldPen);
				penline.DeleteObject();
			}
		}
		break;

#if 0
	case two1:
	case two2:
		{
			COLORREF	col = RGB(255,255,255);
			pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
			penline.CreatePen(PS_SOLID, 1, col);
			pOldPen = pDC->SelectObject(&penline);

			int width = 6, width_s = 3;
			if( m_nShape == two1 )
			{
				for( int cur = rect.top; cur <= rect.bottom; cur += ((width + width_s) * 4) )
				{
					pDC->MoveTo(rect.left, cur);
					pDC->LineTo(rect.right, cur + (width + width_s) + width);

					pDC->MoveTo(rect.left, cur + (width + width_s) * 2);
					pDC->LineTo(rect.right, cur + (width + width_s) * 2 + width);

					pDC->MoveTo(rect.left, cur + (width + width_s) * 3);
					pDC->LineTo(rect.right, cur + (width + width_s) * 3 + width);
				}
			}
			else
			{
				for( int cur = rect.top; cur <= rect.bottom; cur += (width + width_s) )
				{
					pDC->MoveTo(rect.left, cur);
					pDC->LineTo(rect.right, cur + width);
				}
			}
			penline.DeleteObject();
		}
		break;
#endif
	}

#if 0
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
#endif
}
