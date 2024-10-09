// GridButton.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "GridButton.h"


// CGridButton

IMPLEMENT_DYNAMIC(CGridButton, CMFCButton)

CGridButton::CGridButton()
{
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	mStatus = eButtonStatus_Close;
	m_crEnable = RGB(0, 0, 0);
	m_crDisable = ::GetSysColor(COLOR_GRAYTEXT);
	m_crHot = RGB(0, 0, 0);
	m_crInvert = RGB(255, 255, 255);
	m_bGradient = false;

	COLORREF color;
	for (int i = 0; i < eButtonStatus_ColMax; i++){
		m_GColor[i].lower = Color::MakeARGB(255,
			GetRValue(mStatusColor[i]),
			GetGValue(mStatusColor[i]),
			GetBValue(mStatusColor[i]));
		m_GColor[i].hot = Color::MakeARGB(255,
			GetRValue(mStatusColor[i]),
			GetGValue(mStatusColor[i]),
			GetBValue(mStatusColor[i]));
		if (m_bGradient == true){
			color = darkenColor(mStatusColor[i], 100);
			m_GColor[i].upper = Color::MakeARGB(255,
				GetRValue(color),
				GetGValue(color),
				GetBValue(color));
		}
		else{
			m_GColor[i].upper = Color::MakeARGB(255,
				GetRValue(mStatusColor[i]),
				GetGValue(mStatusColor[i]),
				GetBValue(mStatusColor[i]));
		}
	}

	m_gradientAngle = 90;
	m_bControl = false;
}

CGridButton::~CGridButton()
{
	GdiplusShutdown(m_gdiplusToken);
}

/*============================================================================*/
/*! グリッドボタン

-# 描画カラー属性の作成（３Ｄ用）

@param
@retval

*/
/*============================================================================*/
COLORREF CGridButton::lightenColor(const COLORREF crColor, BYTE blend)
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
COLORREF CGridButton::darkenColor(const COLORREF crColor, BYTE blend)
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


BEGIN_MESSAGE_MAP(CGridButton, CMFCButton)
	ON_WM_ERASEBKGND()
	//ON_WM_LBUTTONDOWN()
	//ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CGridButton メッセージ ハンドラー

void CGridButton::OnFillBackground(CDC* pDC, const CRect& rectClient)
{
	CMFCButton::OnFillBackground(pDC, rectClient);
}


void CGridButton::OnDrawText(CDC* pDC, const CRect& rect, const CString& strText, UINT uiDTFlags, UINT uiState)
{
	return CMFCButton::OnDrawText(pDC, rect, strText, uiDTFlags, uiState);
}

void CGridButton::PostNcDestroy()
{
	delete this;

	CMFCButton::PostNcDestroy();
}

BOOL CGridButton::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.style |= BS_OWNERDRAW;

	return CMFCButton::PreCreateWindow(cs);
}


void CGridButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	UINT				state(lpDrawItemStruct->itemState);
	CString				btnText;
	Graphics			graphics(lpDrawItemStruct->hDC);
	RECT				rctBgContent;
	Rect				clientRect;
	CRect				txtRect(0, 0, 0, 0);
	LinearGradientBrush *plinGrBrush = NULL;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	Color	clrDisabled = Color::MakeARGB(255, GetRValue(RGB(192,192,192)),
		GetGValue(RGB(192, 192, 192)),
		GetBValue(RGB(192, 192, 192)));

	UINT partID;
	UINT stateID;
	partID = DFC_BUTTON;
	stateID = (state & ODS_SELECTED) ? DFCS_BUTTONPUSH | DFCS_FLAT | DFCS_PUSHED : DFCS_BUTTONPUSH | DFCS_FLAT;
	DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, partID, stateID);
	memcpy(&rctBgContent, &lpDrawItemStruct->rcItem, sizeof(RECT));
	InflateRect(&rctBgContent, -GetSystemMetrics(SM_CXEDGE), -GetSystemMetrics(SM_CYEDGE));

	clientRect.X = rctBgContent.left;
	clientRect.Y = rctBgContent.top;
	clientRect.Width = rctBgContent.right - rctBgContent.left;
	clientRect.Height = rctBgContent.bottom - rctBgContent.top;

	//if (m_bGradient == true)
	{
		Color	*pclrLower = NULL;

		if (state & ODS_SELECTED)
		{
			pclrLower = &m_GColor[mStatus].lower;
			plinGrBrush = new LinearGradientBrush(clientRect, *pclrLower, m_GColor[mStatus].upper, m_gradientAngle);

		}
		else if (state & ODS_DISABLED)
		{
			pclrLower = &clrDisabled;
			plinGrBrush = new LinearGradientBrush(clientRect, clrDisabled, *pclrLower, m_gradientAngle);
		}
		else
		{
			pclrLower = &m_GColor[mStatus].lower;
			plinGrBrush = new LinearGradientBrush(clientRect, m_GColor[mStatus].upper, *pclrLower, m_gradientAngle);
		}

		graphics.FillRectangle(plinGrBrush, clientRect);
	}
	//else{
		//pDC->FillSolidRect(&rctBgContent, mStatusColor[mStatus]);
	//}

	CFont* oldFont = NULL;
	// メニュー文字列フォントの作成
	CFont txtFont;
	LOGFONT lf;
	if (((CFont*)&CMFCMenuBar::GetMenuFont())->GetLogFont(&lf)){
		lf.lfWeight = FW_NORMAL;
		if (theApp.GetSelectStationMode() == eStation_1){
			lf.lfHeight = 12;
		}
		else{
			lf.lfHeight = 12;
		}
		txtFont.CreateFontIndirect(&lf);
		oldFont = pDC->SelectObject(&txtFont);
	}

	// テキスト描画
	GetWindowText(btnText);
	if (!btnText.IsEmpty())
	{
		COLORREF	clrTxt, clrOrg;
		int			bkModeOld;

		if (state & ODS_DISABLED)
			clrTxt = m_crDisable;
		else if (GetRValue(mStatusColor[mStatus]) <= 128 && GetGValue(mStatusColor[mStatus]) <= 128 && GetBValue(mStatusColor[mStatus]) <= 128)
			clrTxt = m_crInvert;
		else
			clrTxt = m_crEnable;

		DrawText(lpDrawItemStruct->hDC, btnText, btnText.GetLength(), &txtRect, DT_CALCRECT | DT_CENTER);

		int txtWidth = txtRect.Width();
		int txtHight = txtRect.Height();

		txtRect.left = clientRect.X + (clientRect.Width - txtWidth) / 2;
		txtRect.right = txtRect.left + txtWidth;
		txtRect.top = clientRect.Y + (clientRect.Height - txtHight) / 2;
		txtRect.bottom = txtRect.top + txtHight;

		txtRect.IntersectRect(&rctBgContent, &txtRect);

		clrOrg = ::SetTextColor(lpDrawItemStruct->hDC, clrTxt);
		bkModeOld = SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);

		DrawText(lpDrawItemStruct->hDC, btnText, btnText.GetLength(), &txtRect, DT_CENTER);

		SetBkMode(lpDrawItemStruct->hDC, bkModeOld);
		::SetTextColor(lpDrawItemStruct->hDC, clrOrg);
	}

	//if (m_bGradient == true)
	{
		delete plinGrBrush;
	}

	if (oldFont != NULL){
		pDC->SelectObject(oldFont);
		txtFont.DeleteObject();
	}
}


BOOL CGridButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CMFCButton::OnEraseBkgnd(pDC);
}


void CGridButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bControl = (nFlags & MK_CONTROL)?true:false;
	TRACE("##OnLButtonDown\n");

	CMFCButton::OnLButtonDown(nFlags, point);
}


void CGridButton::OnMouseMove(UINT nFlags, CPoint point)
{
	return;

	CMFCButton::OnMouseMove(nFlags, point);
}
