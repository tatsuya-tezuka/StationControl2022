#pragma once


// CTrackingCtrl

class CTrackingCtrl : public CWnd
{
	DECLARE_DYNAMIC(CTrackingCtrl)

public:
	CTrackingCtrl();
	virtual ~CTrackingCtrl();

	//-----------------------------------------------------------------------------
	//	ÉÅÉìÉoïœêî
	//-----------------------------------------------------------------------------
protected:
	CWnd*		mParent;
	BOOL		mInitOver;
	CRect		mRect;

public:

	//-----------------------------------------------------------------------------
	//	ÉÅÉìÉoä÷êî
	//-----------------------------------------------------------------------------
protected:
	BOOL	RegisterClass();
	double	NormalizeAngle(const double angle);

public:
	BOOL	Create(CWnd *pParent, CRect rect, UINT nID);

	//-----------------------------------------------------------------------------

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
protected:
	virtual void PostNcDestroy();
};


