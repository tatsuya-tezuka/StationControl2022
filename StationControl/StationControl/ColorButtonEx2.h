#pragma once

#include "ColorButtonEx.h"


// CColorButtonEx2

class CColorButtonEx2 : public CColorButtonEx
{
	DECLARE_DYNAMIC(CColorButtonEx2)

public:
	CColorButtonEx2();
	virtual ~CColorButtonEx2();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
protected:
	COLORREF	m_ignoreColor1;		// 無効色1:デフォルト黒色
	COLORREF	m_ignoreColor2;		// 無効色2:デフォルト灰色

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	void SetIgnoreColor1(COLORREF col)	{ m_ignoreColor1 = col; }	// 無効色1設定
	void SetIgnoreColor2(COLORREF col)	{ m_ignoreColor2 = col; }	// 無効色2設定
	// ボタン色が無効色1になっているかを返す。true:無効色になっている。/ false:なっていない。
	bool IsColorIgnore1()	{
		if (m_ColorNormal1 == m_ignoreColor1)
			return true;
		else
			return false;
	}
	// ボタン色が無効色2になっているかを返す。true:無効色になっている。/ false:なっていない。
	bool IsColorIgnore2()	{
		if (m_ColorNormal1 == m_ignoreColor2)
			return true;
		else
			return false;
	}

protected:
	
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


