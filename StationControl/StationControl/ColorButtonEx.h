#pragma once


// CColorButtonEx

class CColorButtonEx : public CButton
{
	DECLARE_DYNAMIC(CColorButtonEx)

	/* ------------------------------------------------------------------------------------ */
	/* íËã`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	//-----------------------------------------------------------------------------
	//	ÉÅÉìÉoïœêî
	//-----------------------------------------------------------------------------
public:
protected:
	COLORREF	m_ColorNormal1;
	COLORREF	m_ColorNormal2;
	BOOL		m_bHalf;
	BOOL		m_bCtrlButton;
	BOOL		m_bShiftButton;

	//-----------------------------------------------------------------------------
	//	ÉÅÉìÉoä÷êî
	//-----------------------------------------------------------------------------
public:
	void SetColor(COLORREF nomalCol1, COLORREF nomalCol2 = NULL, BOOL bHalf = FALSE);
	void SetCtrlButton(BOOL val) { m_bCtrlButton = val; }
	void SetShiftButton(BOOL val) { m_bShiftButton = val; }
	COLORREF GetColor() { return m_ColorNormal1; }

protected:
	BOOL CheckKeyClick(UINT nFlags);


public:
	CColorButtonEx();
	virtual ~CColorButtonEx();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


