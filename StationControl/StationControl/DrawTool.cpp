// DrawTool.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"

#include "DrawObject.h"
#include "DrawTool.h"

// CDrawTool

CPtrList CDrawTool::c_tools;

static CSelectTool selectTool;
static CRectTool rectTool(rect);
static CRectTool lineTool(line);

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

void CDrawTool::OnMouseMove(CGraphWindow* /*pView*/, UINT nFlags, const CPoint& point)
{
	c_last = point;
	//SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	if(c_drawShape == selection && nFlags&MK_LBUTTON){
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
	} else{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
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
		if(nDragHandle != 0)
			selectMode = SelectMode::size;
	}

	if(selectMode == none){
		pObj = pView->ObjectAt(local);

		if(pObj != NULL){
			selectMode = SelectMode::move;

			if(!pView->IsSelected(pObj))
				pView->Select(pObj, (nFlags & MK_SHIFT) != 0);
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
				SetCursor(pObj->GetHandleCursor(nHandle));
				return;
			}
			if(nHandle == 0){
				SetCursor(pObj->GetHandleCursor(nHandle));
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

		if (selectMode == SelectMode::move){
			position += delta;
			pObj->MoveTo(position, pView);
		}else if(nDragHandle != 0){
			pObj->MoveHandleTo(nDragHandle, local, pView);
		}
	}

	lastPoint = local;

	if(selectMode == SelectMode::size && c_drawShape == selection){
		c_last = point;
		SetCursor(pView->m_selection.GetHead()->GetHandleCursor(nDragHandle));
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

void CRectTool::MoveLineCursor(CGraphWindow* pView, const CPoint& point)
{
	POSITION pos = pView->m_selection.GetHeadPosition();
	while(pos != NULL){
		CDrawObject* pObj = pView->m_selection.GetNext(pos);
		if(pObj != NULL){
			if(pObj->GetShape() == objline){
				CRect position = pObj->m_position;
				position.left = point.x;
				position.right = point.x;
				pObj->MoveTo(position, pView);
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
	}
	pView->Add(pObj);
	pView->Select(pObj);

	selectMode = SelectMode::size;
	nDragHandle = 1;
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

void CRectTool::Create(CGraphWindow* pView, const CRect& rc)
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
	}
	pView->Add(pObj);

	CPoint local = CPoint(rc.left, rc.top);
	pView->Select(pObj);
	selectMode = SelectMode::size;
	nDragHandle = 1;
	lastPoint = local;
}
