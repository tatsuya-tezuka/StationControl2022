#pragma once

// CDrawTool コマンド ターゲット

class CGraphWindow;

enum DrawShape
{
	selection, rect, line
};

enum ObjectShape
{
	objrect, objline
};

class CDrawTool
{
public:
	CDrawTool(DrawShape nDrawShape);

	virtual void MoveLineCursor(CGraphWindow* pView, const CPoint& point){};
	virtual void OnLButtonDown(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnEditProperties(CGraphWindow* pView);
	virtual void OnCancel();
	virtual void Create(CGraphWindow* pView, const CRect& rc){};
	virtual void OnRButtonDown(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnKeyboardMove(CGraphWindow* pView, const CRect rect);

	DrawShape	m_drawShape;

	static CDrawTool* FindTool(DrawShape drawShape);
	static CPtrList c_tools;
	static CPoint c_down;
	static UINT c_nDownFlags;
	static CPoint c_last;
	static DrawShape c_drawShape;
	static CPoint m_ptActive;
};


class CSelectTool : public CDrawTool
{
// Constructors
public:
	CSelectTool();

// Implementation
	virtual void MoveLineCursor(CGraphWindow* pView, const CPoint& point){};
	virtual void OnLButtonDown(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void Create(CGraphWindow* pView, const CRect& rc){};
	virtual void OnRButtonDown(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnKeyboardMove(CGraphWindow* pView, const CRect rect);
};

class CRectTool : public CDrawTool
{
// Constructors
public:
	CRectTool(DrawShape drawShape);

// Implementation
	virtual void MoveLineCursor(CGraphWindow* pView, const CPoint& point);
	virtual void OnLButtonDown(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void Create(CGraphWindow* pView, const CRect& rc);
	virtual void OnRButtonDown(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnKeyboardMove(CGraphWindow* pView, const CRect rect);
};
