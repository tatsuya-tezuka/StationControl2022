#pragma once

// CDrawObject

class CGraphWindow;

class CDrawObject : public CObject
{
protected:
	DECLARE_SERIAL(CDrawObject);
	CDrawObject();

public:
	CDrawObject(const CRect& position);

// Attributes
	CRect	m_position;

	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	CRect GetHandleRect(int nHandleID, CGraphWindow* pView);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void SetLineColor(COLORREF color, BOOL bPreview = FALSE);
	virtual void SetFillColor(COLORREF color, BOOL bPreview = FALSE);

// Operations
	virtual void Draw(CDC* pDC, BOOL bEditMode);
	virtual void MoveTo(const CRect& positon, CGraphWindow* pView = NULL);
	virtual int HitTest(CPoint point, CGraphWindow* pView, BOOL bSelected);
	virtual BOOL Intersects(const CRect& rect);
	virtual void MoveHandleTo(int nHandle, CPoint point, CGraphWindow* pView = NULL);
	virtual void Remove();
	void Invalidate();

	COLORREF GetLineColor() const
	{
		return m_logpen.lopnColor;
	}

	int GetLineWeight() const
	{
		return m_logpen.lopnWidth.x;
	}

	void SetLineWeight(int nWidth, BOOL bPreview = FALSE);

	COLORREF GetFillColor() const
	{
		return m_logbrush.lbColor;
	}

	BOOL IsEnableFill() const
	{
		return m_bBrush;
	}

	void EnableFill(BOOL bEnable, BOOL bPreview = FALSE);

	BOOL IsEnableLine() const
	{
		return m_bPen;
	}

	void EnableLine(BOOL bEnable, BOOL bPreview = FALSE);

	virtual BOOL CanChangeFillColor() const
	{
		return TRUE;
	}

	virtual BOOL CanChangeLineColor() const
	{
		return TRUE;
	}

	virtual BOOL CanChangeLineWeight() const
	{
		return TRUE;
	}

	virtual BOOL IsImageText() const
	{
		return FALSE;
	}

	virtual BOOL IsFunction() const
	{
		return FALSE;
	}

	virtual BOOL IsFunctionNone() const
	{
		return FALSE;
	}

	virtual UINT GetShape() const
	{
		return 0;
	}

	virtual UINT GetItemNo() const
	{
		return 0;
	}

// Implementation
public:
	virtual ~CDrawObject();
#ifdef _DEBUG
	void AssertValid();
#endif

// implementation data
protected:
	BOOL		m_bPen;
	LOGPEN		m_logpen;
	BOOL		m_bBrush;
	LOGBRUSH	m_logbrush;
};

typedef CTypedPtrList<CObList, CDrawObject*> CDrawObjList;


////////////////////////////////////////////////////////////////////////
// specialized draw objects

class CDrawRect : public CDrawObject
{
protected:
	DECLARE_SERIAL(CDrawRect);
	CDrawRect();

public:
	CDrawRect(const CRect& position);

	enum Shape { rectangle, line };

// Implementation
public:
	virtual void Draw(CDC* pDC, BOOL bEditMode);
	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int nHandle, CPoint point, CGraphWindow* pView = NULL);
	virtual BOOL Intersects(const CRect& rect);


	virtual UINT GetShape() const
	{
		return m_nShape;
	}

protected:
	Shape		m_nShape;
	COLORREF	m_textColor;
	COLORREF	m_backColor;

	friend class CRectTool;
};
