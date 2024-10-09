#pragma once

#include "..\StationControl\GridButton.h"

// CColorControl

class CColorControl
{
public:
	CColorControl();
	virtual ~CColorControl();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	bool		mGradient;
	COLORREF	mTextColor, mTextHotColor;
	COLORREF	mBackColor, mBackHotColor;
	CBrush		*mpBackpBrush, *mpBackHotBrush, *mpDisableBrush;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	virtual void	SetGradient(const bool val) { mGradient = val; }
	virtual void	SetTextColor(const COLORREF text, const COLORREF texthot);
	virtual void	SetBackColor(const COLORREF back, const COLORREF texthot);
protected:
	COLORREF	lightenColor(const COLORREF crColor, BYTE blend);
	COLORREF	darkenColor(const COLORREF crColor, BYTE blend);

	/* ------------------------------------------------------------------------------------ */
};


// CColorStatic

class CColorStatic : public CStatic, public CColorControl
{
	DECLARE_DYNAMIC(CColorStatic)

public:
	CColorStatic();
	virtual ~CColorStatic();


	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString	mText;
	UINT	mTextStyle;
	UINT	mStyle;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetText(CString val)	{ mText = val; Invalidate(); }
	void	SetTextStyle(UINT val)	{ mTextStyle = val; }
	void	SetStyle(UINT val)
	{ 
		mStyle = val;
		ModifyStyle(0, mStyle);
	}
	void	SetBackColorEx(const COLORREF back, const COLORREF texthot, const bool updatedraw = true);

protected:

	/* ------------------------------------------------------------------------------------ */

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};

// CColorEdit

class CColorEdit : public CMFCMaskedEdit, public CColorControl
{
	DECLARE_DYNAMIC(CColorEdit)

public:
	CColorEdit();
	virtual ~CColorEdit();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
};

// CColorComboBox

class CColorComboBoxEx : public CComboBox, public CColorControl
{
	DECLARE_DYNAMIC(CColorComboBoxEx)
public:
	CColorComboBoxEx();
	virtual ~CColorComboBoxEx();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	bool		m_bComboHot;
	COLORREF	mSelectedTextColor;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetSelectedTextColor(COLORREF val)	{ mSelectedTextColor = val; }
	COLORREF	GetSelectedTextColor()			{ return mSelectedTextColor; }
protected:

	/* ------------------------------------------------------------------------------------ */

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};
