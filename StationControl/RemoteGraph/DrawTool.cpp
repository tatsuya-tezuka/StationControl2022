// DrawTool.cpp : 実装ファイル
//

#include "stdafx.h"

#include "GraphWindow.h"

#include "DrawObject.h"
#include "DrawTool.h"

// CDrawTool

CPtrList CDrawTool::c_tools;

static CSelectTool selectTool;
static CRectTool rectTool(rect);
static CRectTool lineTool(line);
static CRectTool diffTool(diff);
static CRectTool limitwarning01Tool(limit_warning_01);
static CRectTool limitwarning02Tool(limit_warning_02);
static CRectTool limitwarning03Tool(limit_warning_03);
static CRectTool limitwarning04Tool(limit_warning_04);
static CRectTool limitwarning05Tool(limit_warning_05);
static CRectTool limitwarning06Tool(limit_warning_06);
static CRectTool limitwarning07Tool(limit_warning_07);
static CRectTool limitwarning08Tool(limit_warning_08);
static CRectTool limitwarning09Tool(limit_warning_09);
static CRectTool limitwarning10Tool(limit_warning_10);
static CRectTool limitalarm01Tool(limit_alarm_01);
static CRectTool limitalarm02Tool(limit_alarm_02);
static CRectTool limitalarm03Tool(limit_alarm_03);
static CRectTool limitalarm04Tool(limit_alarm_04);
static CRectTool limitalarm05Tool(limit_alarm_05);
static CRectTool limitalarm06Tool(limit_alarm_06);
static CRectTool limitalarm07Tool(limit_alarm_07);
static CRectTool limitalarm08Tool(limit_alarm_08);
static CRectTool limitalarm09Tool(limit_alarm_09);
static CRectTool limitalarm10Tool(limit_alarm_10);
static CRectTool two1Tool(two1);
static CRectTool two2Tool(two2);

CPoint		CDrawTool::c_down;
UINT		CDrawTool::c_nDownFlags;
CPoint		CDrawTool::c_last;
DrawShape	CDrawTool::c_drawShape = selection;
CPoint		CDrawTool::m_ptActive;

CDrawTool::CDrawTool(DrawShape drawShape)
{
	m_drawShape = drawShape;
	c_tools.AddTail(this);
}

/*============================================================================*/
/*! イメージツール

	-# オブジェクトの検索

	@param
	@retval

*/
/*============================================================================*/
CDrawTool* CDrawTool::FindTool(DrawShape drawShape)
{
	POSITION pos = c_tools.GetHeadPosition();
	while(pos != NULL){
		CDrawTool* pTool = (CDrawTool*)c_tools.GetNext(pos);
		if(pTool->m_drawShape == drawShape)
			return pTool;
	}

	return NULL;
}

void CDrawTool::OnLButtonDown(CGraphWindow* pView, UINT nFlags, const CPoint& point)
{
	pView->SetCapture();
	c_nDownFlags = nFlags;
	c_down = point;
	c_last = point;
}

void CDrawTool::OnLButtonDblClk(CGraphWindow* /*pView*/, UINT /*nFlags*/, const CPoint& /*point*/)
{
}

void CDrawTool::OnLButtonUp(CGraphWindow* /*pView*/, UINT /*nFlags*/, const CPoint& point)
{
	ReleaseCapture();

	// ★
	return;
	if(point == c_down)
		c_drawShape = selection;
}

void CDrawTool::OnMouseMove(CGraphWindow* pView, UINT nFlags, const CPoint& point)
{
	c_last = point;
	//SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	if(c_drawShape == selection && nFlags&MK_LBUTTON){
//		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
#if 00
		pView->m_idMouseCursor = 32649;
#endif
	} else{
//		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
#if 00
		pView->m_idMouseCursor = 32512;
#endif
	}
}

void CDrawTool::OnRButtonDown(CGraphWindow* pView, UINT nFlags, const CPoint& point)
{
	//pView->SetCapture();
	//c_nDownFlags = nFlags;
	//c_down = point;
	//c_last = point;
}

void CDrawTool::OnKeyboardMove(CGraphWindow* pView, const CRect rect)
{
}

void CDrawTool::OnEditProperties(CGraphWindow* /*pView*/)
{
}

void CDrawTool::OnCancel()
{
	c_drawShape = selection;
}


////////////////////////////////////////////////////////////////////////////
// CResizeTool

enum SelectMode
{
	none, netSelect, move, size
};

SelectMode selectMode = none;
int nDragHandle;

CPoint lastPoint;

CSelectTool::CSelectTool()
: CDrawTool(selection)
{
}

void CSelectTool::OnLButtonDown(CGraphWindow* pView, UINT nFlags, const CPoint& point)
{
	CPoint local = point;
	pView->ClientToDoc(local);

	m_ptActive = CPoint(0,0);

	CDrawObject* pObj;
	selectMode = none;

	if(pView->m_selection.GetCount() == 1){
		pObj = pView->m_selection.GetHead();

		nDragHandle = pObj->HitTest(local, pView, TRUE);

		if(nDragHandle == 0)
		{
			if(pObj->GetShape() == objtwo1)
			{
//				SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR1)));
#if 00
				pView->m_idMouseCursor = IDC_CURSOR1;
#endif

				pView->m_resDrawObject = 1;
			}
			else if(pObj->GetShape() == objtwo2)
			{
//				SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR2)));
#if 00
				pView->m_idMouseCursor = IDC_CURSOR2;
#endif

				pView->m_resDrawObject = 2;
			}
			else
			{
//				SetCursor(pObj->GetHandleCursor(nDragHandle));
#if 00
				pView->m_idMouseCursor = 32649;
#endif

				//pView->m_resDrawObject = (pObj->GetShape() == objtwo1) ? 1 : 2;	// 結果を返す
				// 結果を返す
				if(pObj->GetShape() == objrect)		pView->m_resDrawObject = 3;
				if(pObj->GetShape() == objline)		pView->m_resDrawObject = 4;
				if(pObj->GetShape() == objdiff)		pView->m_resDrawObject = 8;
			}
		}

		if(nDragHandle != 0)
		{
			// ２点カーソルは新規に作成しない
			if( (pObj->GetShape() == objtwo1) || (pObj->GetShape() == objtwo2) )
			{
				nDragHandle = 0;
				return;
			}

			selectMode = SelectMode::size;
		}
	}

	if(selectMode == none)
	{
		pObj = NULL;

		if( pView->m_selection.GetCount() == 1 )
		{
			CDrawObject	*pTmpObj = pView->m_selection.GetHead();
			//if( (pTmpObj->GetShape() == objtwo1) || (pTmpObj->GetShape() == objtwo2) )
			if( (pTmpObj->GetShape() == objtwo1) || (pTmpObj->GetShape() == objtwo2) ||
				(pTmpObj->GetShape() == objrect) || (pTmpObj->GetShape() == objline) ||  (pTmpObj->GetShape() == objdiff) )
				pObj = pTmpObj;
		}

		if( pObj == NULL )
			pObj = pView->ObjectAt(local);

		if(pObj != NULL){
			selectMode = SelectMode::move;

			if(!pView->IsSelected(pObj))
				pView->Select(pObj, (nFlags & MK_SHIFT) != 0);

			//pView->m_resDrawObject = (pObj->GetShape() == objtwo1) ? 1 : 2;	// 結果を返す
			// 結果を返す
			if(pObj->GetShape() == objtwo1)		pView->m_resDrawObject = 1;
			if(pObj->GetShape() == objtwo2)		pView->m_resDrawObject = 2;
			if(pObj->GetShape() == objrect)		pView->m_resDrawObject = 3;
			if(pObj->GetShape() == objline)		pView->m_resDrawObject = 4;
			if(pObj->GetShape() == objdiff)		pView->m_resDrawObject = 8;
		}
	}

	if(selectMode == none){
		if((nFlags & MK_SHIFT) == 0)
			pView->Select(NULL);

		selectMode = netSelect;

		CClientDC dc(pView);
		CRect rect(point.x, point.y, point.x, point.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);
	}

	lastPoint = local;
	CDrawTool::OnLButtonDown(pView, nFlags, point);
}

void CSelectTool::OnLButtonDblClk(CGraphWindow* pView, UINT nFlags, const CPoint& point)
{
	CDrawTool::OnLButtonDblClk(pView, nFlags, point);
}

void CSelectTool::OnLButtonUp(CGraphWindow* pView, UINT nFlags, const CPoint& point)
{
	if(pView->GetCapture() == pView){
		if (selectMode == netSelect){
			CClientDC dc(pView);
			CRect rect(c_down.x, c_down.y, c_last.x, c_last.y);
			rect.NormalizeRect();
			dc.DrawFocusRect(rect);

			pView->SelectWithinRect(rect, TRUE);
		}else if(selectMode != none){
			//pView->GetDocument()->UpdateAllViews(pView);
		}
	}

	CDrawTool::OnLButtonUp(pView, nFlags, point);
}

void CSelectTool::OnMouseMove(CGraphWindow* pView, UINT nFlags, const CPoint& point)
{
	if(pView->GetCapture() != pView){
		if(c_drawShape == selection && pView->m_selection.GetCount() == 1){
			CDrawObject* pObj = pView->m_selection.GetHead();
			CPoint local = point;
			pView->ClientToDoc(local);
			int nHandle = pObj->HitTest(local, pView, TRUE);
			if(nHandle >= 0 && nHandle != 0){
//				SetCursor(pObj->GetHandleCursor(nHandle));
#if 00
				pView->m_idMouseCursor = nHandle;
#endif
				return;
			}
			if(nHandle == 0)
			{
				if(pObj->GetShape() == objtwo1)
				{
//					SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR1)));
#if 00
					pView->m_idMouseCursor = IDC_CURSOR1;
#endif

					pView->m_resDrawObject = 1;
				}
				else if(pObj->GetShape() == objtwo2)
				{
//					SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR2)));
#if 00
					pView->m_idMouseCursor = IDC_CURSOR2;
#endif

					pView->m_resDrawObject = 2;
				}
				else
				{
//					SetCursor(pObj->GetHandleCursor(nHandle));
#if 00
					pView->m_idMouseCursor = 32649;
#endif

					//pView->m_resDrawObject = (pObj->GetShape() == objtwo1) ? 1 : 2;	// 結果を返す
					// 結果を返す
					if(pObj->GetShape() == objrect)		pView->m_resDrawObject = 3;
					if(pObj->GetShape() == objline)		pView->m_resDrawObject = 4;
					if(pObj->GetShape() == objdiff)		pView->m_resDrawObject = 8;
				}

				return;
			}
			// ２点カーソルは新規に作成しない
			if( (pObj->GetShape() == objtwo1) || (pObj->GetShape() == objtwo2) )
			{
//				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
#if 00
				pView->m_idMouseCursor = 32512;
#endif
				return;
			}
		}
		if(c_drawShape == selection)
			CDrawTool::OnMouseMove(pView, nFlags, point);
		return;
	}

	if(selectMode == netSelect){
		CClientDC dc(pView);
		CRect rect(c_down.x, c_down.y, c_last.x, c_last.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);
		rect.SetRect(c_down.x, c_down.y, point.x, point.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);

		CDrawTool::OnMouseMove(pView, nFlags, point);
		return;
	}

	CPoint local = point;
	pView->ClientToDoc(local);
	CPoint delta = (CPoint)(local - lastPoint);

	POSITION pos = pView->m_selection.GetHeadPosition();
	while(pos != NULL){
		CDrawObject* pObj = pView->m_selection.GetNext(pos);
		CRect position = pObj->m_position;

		if(selectMode == SelectMode::move){
			position += delta;
			pObj->MoveTo(position, pView);

			//pView->m_resDrawObject = (pObj->GetShape() == objtwo1) ? 1 : 2;	// 結果を返す
			// 結果を返す
			if(pObj->GetShape() == objtwo1)		pView->m_resDrawObject = 1;
			if(pObj->GetShape() == objtwo2)		pView->m_resDrawObject = 2;
			if(pObj->GetShape() == objrect)		pView->m_resDrawObject = 3;
			if(pObj->GetShape() == objline)		pView->m_resDrawObject = 4;
			if(pObj->GetShape() == objdiff)		pView->m_resDrawObject = 8;

		//}else if(nDragHandle != 0){
		}else if(nDragHandle >= 0 && nDragHandle != 0){
			if(pObj->GetShape() == objrect){
				pObj->MoveHandleTo(nDragHandle, local, pView);
			}
//			if(pObj->GetShape() == objdiff){
//				pObj->MoveHandleTo(nDragHandle, local, pView);
//			}
		}
		else
		{
			if(pObj->GetShape() == objdiff){
				pObj->MoveHandleTo(9/*nDragHandle*/, local, pView);
			}
		}
	}

	lastPoint = local;

	if(selectMode == SelectMode::size && c_drawShape == selection){
		c_last = point;
//		SetCursor(pView->m_selection.GetHead()->GetHandleCursor(nDragHandle));
#if 00
		pView->m_idMouseCursor = 32649;
#endif
		return;
	}

	c_last = point;

	if(c_drawShape == selection)
		CDrawTool::OnMouseMove(pView, nFlags, point);
}

void CSelectTool::OnRButtonDown(CGraphWindow* pView, UINT nFlags, const CPoint& point)
{
	CDrawTool::OnRButtonDown(pView, nFlags, point);
}

void CSelectTool::OnKeyboardMove(CGraphWindow* pView, const CRect rect)
{
	POSITION pos = pView->m_selection.GetHeadPosition();
	while(pos != NULL){
		CDrawObject* pObj = pView->m_selection.GetNext(pos);
		if(pObj != NULL){
			CRect position = pObj->m_position;
			position.left += rect.left;
			position.right += rect.right;
			position.top += rect.top;
			position.bottom += rect.bottom;
			pObj->MoveTo(position, pView);
		}
	}
}


////////////////////////////////////////////////////////////////////////////
// CRectTool(does rectangles, round-rectangles, and ellipses)

CRectTool::CRectTool(DrawShape drawShape)
: CDrawTool(drawShape)
{
}

void CRectTool::MoveLineCursor(CGraphWindow* pView, const CRect& rect)
{
	POSITION pos = pView->m_selection.GetHeadPosition();
	while(pos != NULL){
		CDrawObject* pObj = pView->m_selection.GetNext(pos);
		if(pObj != NULL){
			if( pObj->GetShape() == objline )
			{
				CRect position = pObj->m_position;
				position = rect;
				pObj->MoveTo(position, pView);
			}
		}
	}
}

void CRectTool::MoveTwoCursor(UINT obj, CGraphWindow* pView, const CPoint& point)
{
	POSITION pos = pView->m_selection.GetHeadPosition();
	while(pos != NULL){
		CDrawObject* pObj = pView->m_selection.GetNext(pos);
		if(pObj != NULL){
			if( pObj->GetShape() == obj )
			{
				CRect position = pObj->m_position;
				position.left = point.x;
				position.right = point.x;
				pObj->MoveTo(position, pView);
			}
		}
	}
}

void CRectTool::MoveLimit(CGraphWindow* pView, const CRect& rect, ObjectShape obj, bool bInvalidate/* = true*/)
{
	if (pView == NULL)
		return;

	if( bInvalidate )
	{
		POSITION pos = pView->m_selection.GetHeadPosition();
		while(pos != NULL){
			CDrawObject* pObj = pView->m_selection.GetNext(pos);
			if(pObj != NULL){
				if( pObj->GetShape() == obj )
				{
					CRect position = pObj->m_position;
					position = rect;
					pObj->MoveTo(position, pView);
				}
			}
		}
	}
	else
	{
		POSITION pos = pView->m_objects.GetHeadPosition();
		while(pos != NULL){
			CDrawObject* pObj = pView->m_objects.GetNext(pos);
			if(pObj != NULL){
				if( pObj->GetShape() == obj )
				{
					CRect position = pObj->m_position;
					position = rect;
					pObj->m_position = position;
					if( pView != NULL )
						pView->ClientToDoc(pObj->m_position);
				}
			}
		}
	}
}

void CRectTool::MoveDiffer(CGraphWindow* pView, bool bInvalidate/* = true*/)
{
	if (pView == NULL)
		return;

	if (bInvalidate)
	{
		POSITION pos = pView->m_selection.GetHeadPosition();
		while(pos != NULL){
			CDrawObject* pObj = pView->m_selection.GetNext(pos);
			if(pObj != NULL){
				if( pObj->GetShape() == objdiff )
				{
					LONG	add = DIFFER_ADD;

					CRect position = pObj->m_position;
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

					pObj->MoveTo(position, pView);
				}
			}
		}
	}
	else
	{
		POSITION pos = pView->m_objects.GetHeadPosition();
		while(pos != NULL){
			CDrawObject* pObj = pView->m_objects.GetNext(pos);
			if(pObj != NULL){
				if( pObj->GetShape() == objdiff )
				{
					LONG	add = DIFFER_ADD;

					CRect position = pObj->m_position;
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

					pObj->m_position = position;
					if( pView != NULL )
						pView->ClientToDoc(pObj->m_position);
				}
			}
		}
	}
}

void CRectTool::OnLButtonDown(CGraphWindow* pView, UINT nFlags, const CPoint& point)
{
	CDrawTool::OnLButtonDown(pView, nFlags, point);

	CPoint local = point;
	pView->ClientToDoc(local);

	CDrawRect* pObj = new CDrawRect(CRect(local, CSize(0, 0)));
	switch (m_drawShape)
	{
	default:
		ASSERT(FALSE); // unsuported shape!

	case rect:
		pObj->m_nShape = CDrawRect::rectangle;
		break;

	case line:
		pObj->m_nShape = CDrawRect::line;
		break;

	case diff:
		pObj->m_nShape = CDrawRect::differ;
		break;

	case two1:
		pObj->m_nShape = CDrawRect::two1;
		break;

	case two2:
		pObj->m_nShape = CDrawRect::two2;
		break;
	}
	pView->Add(pObj);
	pView->Select(pObj);

	selectMode = SelectMode::size;
	nDragHandle = (m_drawShape == diff) ? (-1) : 1;
	lastPoint = local;
}

void CRectTool::OnLButtonDblClk(CGraphWindow* pView, UINT nFlags, const CPoint& point)
{
	CDrawTool::OnLButtonDblClk(pView, nFlags, point);
}

void CRectTool::OnLButtonUp(CGraphWindow* pView, UINT nFlags, const CPoint& point)
{
	if(point == c_down){
		// オブジェクト作成の中止
		CDrawObject *pObj = pView->m_selection.GetTail();
		pView->Remove(pObj);
		pObj->Remove();
		selectTool.OnLButtonDown(pView, nFlags, point);
		//// ★ラインカーソルとして扱う
		//selectTool.OnMouseMove(pView, nFlags, point);
	}

	selectTool.OnLButtonUp(pView, nFlags, point);
}

void CRectTool::OnMouseMove(CGraphWindow* pView, UINT nFlags, const CPoint& point)
{
	//SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	selectTool.OnMouseMove(pView, nFlags, point);
}

void CRectTool::OnRButtonDown(CGraphWindow* pView, UINT nFlags, const CPoint& point)
{
	CDrawTool::OnRButtonDown(pView, nFlags, point);
}

void CRectTool::OnKeyboardMove(CGraphWindow* pView, const CRect rect)
{
	CDrawTool::OnKeyboardMove(pView, rect);
}

void CRectTool::Create(CGraphWindow* pView, const CRect& rc, bool change/* = true*/)
{
	CDrawRect* pObj = new CDrawRect(rc);
	switch (m_drawShape)
	{
	case rect:
		pObj->m_nShape = CDrawRect::rectangle;
		break;

	case line:
		pObj->m_nShape = CDrawRect::line;
		break;

	case diff:
		pObj->m_nShape = CDrawRect::differ;
		break;

	case limit_warning_01:
		pObj->m_nShape = CDrawRect::limit_warning_01;
		break;

	case limit_warning_02:
		pObj->m_nShape = CDrawRect::limit_warning_02;
		break;

	case limit_warning_03:
		pObj->m_nShape = CDrawRect::limit_warning_03;
		break;

	case limit_warning_04:
		pObj->m_nShape = CDrawRect::limit_warning_04;
		break;

	case limit_warning_05:
		pObj->m_nShape = CDrawRect::limit_warning_05;
		break;

	case limit_warning_06:
		pObj->m_nShape = CDrawRect::limit_warning_06;
		break;

	case limit_warning_07:
		pObj->m_nShape = CDrawRect::limit_warning_07;
		break;

	case limit_warning_08:
		pObj->m_nShape = CDrawRect::limit_warning_08;
		break;

	case limit_warning_09:
		pObj->m_nShape = CDrawRect::limit_warning_09;
		break;

	case limit_warning_10:
		pObj->m_nShape = CDrawRect::limit_warning_10;
		break;

	case limit_alarm_01:
		pObj->m_nShape = CDrawRect::limit_alarm_01;
		break;

	case limit_alarm_02:
		pObj->m_nShape = CDrawRect::limit_alarm_02;
		break;

	case limit_alarm_03:
		pObj->m_nShape = CDrawRect::limit_alarm_03;
		break;

	case limit_alarm_04:
		pObj->m_nShape = CDrawRect::limit_alarm_04;
		break;

	case limit_alarm_05:
		pObj->m_nShape = CDrawRect::limit_alarm_05;
		break;

	case limit_alarm_06:
		pObj->m_nShape = CDrawRect::limit_alarm_06;
		break;

	case limit_alarm_07:
		pObj->m_nShape = CDrawRect::limit_alarm_07;
		break;

	case limit_alarm_08:
		pObj->m_nShape = CDrawRect::limit_alarm_08;
		break;

	case limit_alarm_09:
		pObj->m_nShape = CDrawRect::limit_alarm_09;
		break;

	case limit_alarm_10:
		pObj->m_nShape = CDrawRect::limit_alarm_10;
		break;

	case two1:
		pObj->m_nShape = CDrawRect::two1;
		break;

	case two2:
		pObj->m_nShape = CDrawRect::two2;
		break;
	}
	pView->Add(pObj);

	CPoint local = CPoint(rc.left, rc.top);

	if( change )
	{
		pView->Select(pObj);
		selectMode = SelectMode::size;
		nDragHandle = 1;
		lastPoint = local;
	}
}
