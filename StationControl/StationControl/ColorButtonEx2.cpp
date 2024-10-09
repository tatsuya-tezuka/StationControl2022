/*============================================================================*/
/*! ColorButtonEx2.cpp

-# カスタムボタン（ボタンの背景色変更）
-# 指定された色に従って表示する。
-# Ctrlボタン押下時に実行しない色は２つの無効色で判定する。
*/
/*============================================================================*/
// ColorButtonEx2.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ColorButtonEx2.h"


// CColorButtonEx2

IMPLEMENT_DYNAMIC(CColorButtonEx2, CColorButtonEx)

CColorButtonEx2::CColorButtonEx2()
{
	m_ignoreColor1 = COLOR_LOCAL;		// 無効色1:デフォルト黒色
	m_ignoreColor2 = COLOR_CTRLIN;		// 無効色2:デフォルト灰色
}

CColorButtonEx2::~CColorButtonEx2()
{
}


BEGIN_MESSAGE_MAP(CColorButtonEx2, CColorButtonEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CColorButtonEx2 メッセージ ハンドラー


void CColorButtonEx2::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ((m_ColorNormal1 != m_ignoreColor1) && (m_ColorNormal1 != m_ignoreColor2) && CheckKeyClick(nFlags))
	{
		CButton::OnLButtonDown(nFlags, point);
		return;
	}
	else
	{
		return;
	}

	CButton::OnLButtonDown(nFlags, point);
}


void CColorButtonEx2::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if ((m_ColorNormal1 != m_ignoreColor1) && (m_ColorNormal1 != m_ignoreColor2) && CheckKeyClick(nFlags))
	{
		CButton::OnLButtonDblClk(nFlags, point);
		return;
	}
	else
	{
		return;
	}

	CButton::OnLButtonDblClk(nFlags, point);
}
