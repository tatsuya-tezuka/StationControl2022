// K:\TEZUKA\PROJECTS\StationControl\Utility\ColorControl.cpp : 実装ファイル
//

#include "stdafx.h"
#include "..\Utility\ColorControl.h"


// CColorControl

CColorControl::CColorControl()
{
	mGradient = true;
	mpBackpBrush = NULL;
	mpBackHotBrush = NULL;
	mpDisableBrush = NULL;
	mTextColor = RGB(0, 0, 0);
	mTextHotColor = RGB(0, 0, 0);
	mBackColor = GetSysColor(COLOR_3DFACE);
	mBackHotColor = GetSysColor(COLOR_3DFACE);
	SetBackColor(mBackColor, mBackHotColor);
}

CColorControl::~CColorControl()
{
	if (mpBackpBrush)
		delete mpBackpBrush;
	if (mpBackHotBrush)
		delete mpBackHotBrush;
	if (mpDisableBrush)
		delete mpDisableBrush;
}

/*============================================================================*/
/*! カラーコントロール

-# テキスト色の設定

@param  text	テキスト色
@param  texthot	マウスアクティブテキスト色
@retval なし
*/
/*============================================================================*/
void CColorControl::SetTextColor(const COLORREF text, const COLORREF texthot)
{
	mTextColor = text;
	mTextHotColor = texthot;
}

/*============================================================================*/
/*! カラーコントロール

-# 背景色の設定

@param  back	背景色
@param  backhot	マウスアクティブ背景色
@retval なし
*/
/*============================================================================*/
void CColorControl::SetBackColor(const COLORREF back, const COLORREF backhot)
{
	mBackColor = back;
	mBackHotColor = backhot;

	if (mpBackpBrush)
		delete mpBackpBrush;
	if (mpBackHotBrush)
		delete mpBackHotBrush;
	if (mpDisableBrush)
		delete mpDisableBrush;

	mpBackpBrush = new CBrush(mBackColor);
	mpBackHotBrush = new CBrush(mBackHotColor);
	mpDisableBrush = new CBrush(::GetSysColor(COLOR_BTNFACE));
}

/*============================================================================*/
/*! グリッドボタン

-# 描画カラー属性の作成（３Ｄ用）

@param
@retval

*/
/*============================================================================*/
COLORREF CColorControl::lightenColor(const COLORREF crColor, BYTE blend)
{
	//return crColor;
	WORD byRed = (WORD)GetRValue(crColor);
	WORD byGreen = (WORD)GetGValue(crColor);
	WORD byBlue = (WORD)GetBValue(crColor);

	if ((byRed + blend) <= 255)
		byRed = (byRed + blend);
	if ((byGreen + blend) <= 255)
		byGreen = (byGreen + blend);
	if ((byBlue + blend) <= 255)
		byBlue = (byBlue + blend);

	return RGB((BYTE)byRed, (BYTE)byGreen, (BYTE)byBlue);
}

/*============================================================================*/
/*! グリッドボタン

-# 描画カラー属性の作成（３Ｄ用）

@param
@retval

*/
/*============================================================================*/
COLORREF CColorControl::darkenColor(const COLORREF crColor, BYTE blend)
{
	//return crColor;
	WORD byRed = (WORD)GetRValue(crColor);
	WORD byGreen = (WORD)GetGValue(crColor);
	WORD byBlue = (WORD)GetBValue(crColor);

	if (byRed >= blend)
		byRed = (byRed - blend);
	if (byGreen >= blend)
		byGreen = (byGreen - blend);
	if (byBlue >= blend)
		byBlue = (byBlue - blend);

	return RGB((BYTE)byRed, (BYTE)byGreen, (BYTE)byBlue);
}


// CColorStatic

IMPLEMENT_DYNAMIC(CColorStatic, CStatic)

CColorStatic::CColorStatic()
{
	mText.Empty();
	mStyle = SS_LEFT;
	mTextStyle = 0;// DT_VCENTER | DT_LEFT | DT_SINGLELINE | DT_NOCLIP | DT_END_ELLIPSIS;
}

CColorStatic::~CColorStatic()
{
}


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CColorStatic メッセージ ハンドラ

void CColorStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(rect);

	CFont* pOldFont = NULL;

	COLORREF cl = mTextColor;

	if (mGradient == true){
		pOldFont = dc.SelectObject(&afxGlobalData.fontBold);
		// ここでキャプション背景を塗りつぶしている
		// CMFCVisualManagerを呼びだすとOutlookバースタイルに合わせて背景を塗りつぶしてしまう。
		CDrawingManager dm(dc);
		COLORREF dcolor = darkenColor(mBackColor, 50);
		COLORREF lcolor = lightenColor(mBackColor, 50);

		dm.FillGradient(rect, dcolor, lcolor, TRUE);
	}
	else{
		pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);
		dc.FillSolidRect(rect, mBackColor);
	}

	dc.SetTextColor(cl);
	dc.SetBkMode(TRANSPARENT);

	CString strText;
	GetWindowText(strText);

	if(mText.IsEmpty() == false)
		strText = mText;

	if (mTextStyle == 0){
		mTextStyle = DT_SINGLELINE | DT_NOCLIP | DT_END_ELLIPSIS;
		if (GetStyle() & SS_LEFT)			mTextStyle |= DT_LEFT;
		if (GetStyle() & SS_RIGHT)			mTextStyle |= DT_RIGHT;
		if (GetStyle() & SS_CENTER)			mTextStyle |= DT_CENTER;
		if (GetStyle() & SS_CENTERIMAGE)	mTextStyle |= DT_VCENTER;
	}

	dc.DrawText(strText, rect, mTextStyle);

	dc.SelectObject(pOldFont);
}

/*============================================================================*/
/*! カラーコントロール

-# 背景色の設定

@param  back	背景色
@param  backhot	マウスアクティブ背景色
@param	updatedraw	描画更新
@retval なし
*/
/*============================================================================*/
void CColorStatic::SetBackColorEx(const COLORREF back, const COLORREF backhot, const bool updatedraw /*= true*/)
{
	SetBackColor(back, backhot);
	if (updatedraw)
	{
		Invalidate();
	}
}


// CColorEdit

IMPLEMENT_DYNAMIC(CColorEdit, CMFCMaskedEdit)

CColorEdit::CColorEdit()
{
	mTextColor = GetSysColor(CTLCOLOR_EDIT);
	mTextHotColor = GetSysColor(CTLCOLOR_EDIT);
	mBackColor = GetSysColor(COLOR_WINDOW);
	mBackHotColor = GetSysColor(COLOR_WINDOW);
	SetBackColor(mBackColor, mBackHotColor);
}

CColorEdit::~CColorEdit()
{
}


BEGIN_MESSAGE_MAP(CColorEdit, CMFCMaskedEdit)
END_MESSAGE_MAP()

// CColorEdit メッセージ ハンドラ

BOOL CColorEdit::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
	if (!(message == WM_CTLCOLOREDIT || message == WM_CTLCOLORSTATIC))
		return CMFCMaskedEdit::OnChildNotify(message, wParam, lParam, pLResult);

	CDC *pDC = CDC::FromHandle((HDC)wParam);
	
	ASSERT(GetStyle() & ES_READONLY);

	if (GetStyle() & ES_READONLY){
		pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
		pDC->SetBkColor(::GetSysColor(COLOR_BTNFACE));
		*pLResult = (LRESULT)(mpDisableBrush->GetSafeHandle());
	} else{
		pDC->SetTextColor(mTextColor);
		pDC->SetBkColor(mBackColor);
		*pLResult = (LRESULT)(mpBackpBrush->GetSafeHandle());
	}

	return TRUE;
}


// CColorComboBox

IMPLEMENT_DYNAMIC(CColorComboBoxEx, CComboBox)

CColorComboBoxEx::CColorComboBoxEx()
{
	mTextColor = GetSysColor(CTLCOLOR_BTN);
	mTextHotColor = GetSysColor(CTLCOLOR_BTN);
	mBackColor = GetSysColor(COLOR_WINDOW);
	mBackHotColor = GetSysColor(COLOR_WINDOW);
	SetBackColor(mBackColor, mBackHotColor);
	mSelectedTextColor = GetSysColor(COLOR_BTNTEXT);

	m_bComboHot = false;
}

CColorComboBoxEx::~CColorComboBoxEx()
{
}


BEGIN_MESSAGE_MAP(CColorComboBoxEx, CComboBox)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

// CColorComboBox メッセージ ハンドラ

void CColorComboBoxEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	// 描画メッセージで __super::OnPaint() を呼び出さないでください。
	CRect rect;
	GetClientRect(rect);
	CRect temp = rect;
	CRect rcText;
	rcText = rect;
	rect.left = rect.right - ::GetSystemMetrics(SM_CXHSCROLL);
	rcText.right = rect.left - 1;
	CRect rcDropButton = rect;
	GetClientRect(rect);

	COLORREF clrDisabledBkg = ::GetSysColor(COLOR_BTNFACE);
	COLORREF clrDisabledBorder = ::GetSysColor(COLOR_3DDKSHADOW);
	COLORREF clrDisabledText = ::GetSysColor(COLOR_GRAYTEXT);
	COLORREF clrEnabledBkg = ::GetSysColor(COLOR_WINDOW);
	dc.FillSolidRect(rect, clrDisabledBkg);

	int nDropBtnThemeStyle = 0;
	if(m_bComboHot == true)
		nDropBtnThemeStyle = CBXS_HOT;
	else
		nDropBtnThemeStyle = CBXS_NORMAL;

	if (IsWindowEnabled() == FALSE){
		nDropBtnThemeStyle = CBXS_DISABLED;
	}

	HTHEME mComboTheme = NULL;
	mComboTheme = OpenThemeData(m_hWnd, L"COMBOBOX");
	if (mComboTheme){
		Default();
		DrawThemeBackground(mComboTheme, dc.m_hDC, CP_BACKGROUND, nDropBtnThemeStyle, &rcText, NULL);
		DrawThemeBackground(mComboTheme, dc.m_hDC, CP_READONLY, nDropBtnThemeStyle, &rcText, NULL);
		rcDropButton.left -= 2;
		rcDropButton.right -= 2;
		DrawThemeBackground(mComboTheme, dc.m_hDC, CP_DROPDOWNBUTTON, nDropBtnThemeStyle, &rcDropButton, NULL);
		CloseThemeData(mComboTheme);

		// テキスト描画
		CFont *pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);
		CString str;
		GetWindowText(str);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(mSelectedTextColor);
		rcText.DeflateRect(3, 0);
		dc.DrawText(str, &rcText, DT_SINGLELINE | DT_VCENTER);
		dc.SelectObject(pOldFont);
		return;
	}

	UINT dwBtnStyle = DFCS_SCROLLDOWN | DFCS_PUSHED | DFCS_FLAT;
	dc.DrawFrameControl(rcDropButton, DFC_SCROLL, dwBtnStyle);

	COLORREF clrTopLeft = ::GetSysColor(COLOR_3DSHADOW);
	COLORREF clrBottomRight = ::GetSysColor(COLOR_3DHILIGHT);
	dc.Draw3dRect(&rect, clrTopLeft, clrBottomRight);
	clrTopLeft = ::GetSysColor(COLOR_3DDKSHADOW);
	clrBottomRight = ::GetSysColor(COLOR_3DLIGHT);
	dc.Draw3dRect(&rect, clrTopLeft, clrBottomRight);
	dc.DrawFrameControl(rcDropButton, DFC_SCROLL, dwBtnStyle);

	// テキスト描画
	CFont *pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);
	CString str;
	GetWindowText(str);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(mSelectedTextColor);
	rcText.DeflateRect(3, 0);
	dc.DrawText(str, &rcText, DT_SINGLELINE | DT_VCENTER);
	dc.SelectObject(pOldFont);
}


void CColorComboBoxEx::OnMouseMove(UINT nFlags, CPoint point)
{
	bool bChange = m_bComboHot;
	CRect rect;
	GetClientRect(rect);

	// ドロップダウンリスト
	if ((GetStyle() & CBS_DROPDOWN) && (GetStyle() & CBS_SIMPLE)){
		if (rect.PtInRect(point)){
			m_bComboHot = true;
		} else{
			m_bComboHot = false;
		}
	}

	if (bChange != m_bComboHot){
		InvalidateRect(&rect);
	}

	CComboBox::OnMouseMove(nFlags, point);
}


void CColorComboBoxEx::OnMouseLeave()
{
	bool bChange = m_bComboHot;
	CRect rect;
	GetClientRect(rect);
	m_bComboHot = false;
	if (bChange != m_bComboHot){
		InvalidateRect(&rect);
	}

	CComboBox::OnMouseLeave();
}
