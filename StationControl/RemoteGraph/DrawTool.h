#pragma once

// CDrawTool コマンド ターゲット

class CGraphWindow;

enum DrawShape
{
	selection,
	rect,
	line,
	diff,
	limit_warning_01, limit_warning_02, limit_warning_03, limit_warning_04, limit_warning_05, limit_warning_06, limit_warning_07, limit_warning_08, limit_warning_09, limit_warning_10,
	limit_alarm_01, limit_alarm_02, limit_alarm_03, limit_alarm_04, limit_alarm_05, limit_alarm_06, limit_alarm_07, limit_alarm_08, limit_alarm_09, limit_alarm_10,
	two1, two2
};

enum ObjectShape
{
	objrect,
	objline,
	objdiff,
	objlimit_warning_01, objlimit_warning_02, objlimit_warning_03, objlimit_warning_04, objlimit_warning_05, objlimit_warning_06, objlimit_warning_07, objlimit_warning_08, objlimit_warning_09, objlimit_warning_10,
	objlimit_alarm_01, objlimit_alarm_02, objlimit_alarm_03, objlimit_alarm_04, objlimit_alarm_05, objlimit_alarm_06, objlimit_alarm_07, objlimit_alarm_08, objlimit_alarm_09, objlimit_alarm_10,
	objtwo1, objtwo2
};

static const DrawShape	tblLimitDrawShape[MAX_TDATA][LimitKind_Max] =
{
	{ limit_warning_01, limit_alarm_01},
	{ limit_warning_02, limit_alarm_02},
	{ limit_warning_03, limit_alarm_03},
	{ limit_warning_04, limit_alarm_04},
	{ limit_warning_05, limit_alarm_05},
	{ limit_warning_06, limit_alarm_06},
	{ limit_warning_07, limit_alarm_07},
	{ limit_warning_08, limit_alarm_08},
	{ limit_warning_09, limit_alarm_09},
	{ limit_warning_10, limit_alarm_10}
};

static const ObjectShape	tblLimitObjectShape[MAX_TDATA][LimitKind_Max] =
{
	{ objlimit_warning_01, objlimit_alarm_01},
	{ objlimit_warning_02, objlimit_alarm_02},
	{ objlimit_warning_03, objlimit_alarm_03},
	{ objlimit_warning_04, objlimit_alarm_04},
	{ objlimit_warning_05, objlimit_alarm_05},
	{ objlimit_warning_06, objlimit_alarm_06},
	{ objlimit_warning_07, objlimit_alarm_07},
	{ objlimit_warning_08, objlimit_alarm_08},
	{ objlimit_warning_09, objlimit_alarm_09},
	{ objlimit_warning_10, objlimit_alarm_10}
};

class CDrawTool
{
public:
	CDrawTool(DrawShape nDrawShape);

	virtual void MoveLineCursor(CGraphWindow* pView, const CRect& rect){};
	virtual void MoveTwoCursor(UINT obj, CGraphWindow* pView, const CPoint& point){};
	virtual void MoveLimit(CGraphWindow* pView, const CRect& rect, ObjectShape obj, bool bInvalidate = true){};
	virtual void MoveDiffer(CGraphWindow* pView, bool bInvalidate = true){};
	virtual void OnLButtonDown(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnEditProperties(CGraphWindow* pView);
	virtual void OnCancel();
	virtual void Create(CGraphWindow* pView, const CRect& rc, bool change = true){};
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
	virtual void MoveLineCursor(CGraphWindow* pView, const CRect& rect){};
	virtual void MoveTwoCursor(UINT obj, CGraphWindow* pView, const CPoint& point){};
	virtual void MoveLimit(CGraphWindow* pView, const CRect& rect, ObjectShape obj, bool bInvalidate = true){};
	virtual void MoveDiffer(CGraphWindow* pView, bool bInvalidate = true){};
	virtual void OnLButtonDown(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void Create(CGraphWindow* pView, const CRect& rc, bool change = true){};
	virtual void OnRButtonDown(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnKeyboardMove(CGraphWindow* pView, const CRect rect);
};

class CRectTool : public CDrawTool
{
// Constructors
public:
	CRectTool(DrawShape drawShape);

// Implementation
	virtual void MoveLineCursor(CGraphWindow* pView, const CRect& rect);
	virtual void MoveTwoCursor(UINT obj, CGraphWindow* pView, const CPoint& point);
	virtual void MoveLimit(CGraphWindow* pView, const CRect& rect, ObjectShape obj, bool bInvalidate = true);
	virtual void MoveDiffer(CGraphWindow* pView, bool bInvalidate = true);
	virtual void OnLButtonDown(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void Create(CGraphWindow* pView, const CRect& rc, bool change = true);
	virtual void OnRButtonDown(CGraphWindow* pView, UINT nFlags, const CPoint& point);
	virtual void OnKeyboardMove(CGraphWindow* pView, const CRect rect);
};
