#pragma once


// CMFCToolBarEx

class CMFCToolBarEx : public CMFCToolBar
{
	DECLARE_DYNAMIC(CMFCToolBarEx)

public:
	CMFCToolBarEx();
	virtual ~CMFCToolBarEx();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoPaint(CDC* pDC);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};


