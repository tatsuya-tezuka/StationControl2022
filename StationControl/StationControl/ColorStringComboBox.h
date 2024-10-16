#pragma once


// CColorStringComboBox

class CColorStringComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CColorStringComboBox)

public:
	CColorStringComboBox();
	virtual ~CColorStringComboBox();

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
//	COLORREF	m_colSelectedText;
//	COLORREF	m_colOtherText;
	COLORREF	m_colText;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
//	void SetSelectedTextColor(COLORREF color)	{ m_colSelectedText = color; }
//	void SetOtherTextColor(COLORREF color)		{ m_colOtherText = color; }
	void SetTextColor(COLORREF color)			{ m_colText = color; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	/* ------------------------------------------------------------------------------------ */
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
};


