// MFCCaptionBarEx.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "MFCCaptionBarEx.h"


IMPLEMENT_DYNAMIC(CMFCButtonEx, CMFCButton)

CMFCButtonEx::CMFCButtonEx()
{
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	m_crEnable = RGB(255, 255, 255);
	m_crDisable = ::GetSysColor(COLOR_GRAYTEXT);
	m_crHot = RGB(0, 0, 0);

	m_gradientAngle = 90;
	m_bGradient = false;
	m_bButtonNormal = true;
}

CMFCButtonEx::~CMFCButtonEx()
{
	GdiplusShutdown(m_gdiplusToken);
}

BEGIN_MESSAGE_MAP(CMFCButtonEx, CMFCButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CMFCButtonEx::OnBnClicked)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


void CMFCButtonEx::PreSubclassWindow()
{
	CMFCButton::PreSubclassWindow();
}

void CMFCButtonEx::OnRButtonDown(UINT nFlags, CPoint point)
{
	int id = GetDlgCtrlID();
	if (id == eCtrl_SnapShot){
		CMenu		pPopup;
		CMenu*		pPopupSub;
		if (!pPopup.LoadMenu(IDR_MENU_HCOPY))
			return;
		pPopupSub = pPopup.GetSubMenu(0);
		if (pPopupSub == NULL){
			pPopup.DestroyMenu();
			return;
		}
		CPoint		pos;
		GetCursorPos(&pos);
		pPopupSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pos.x, pos.y, theApp.m_pMainWnd);
		pPopup.DestroyMenu();
		return;
	}

	CMFCButton::OnRButtonDown(nFlags, point);
}

void CMFCButtonEx::OnBnClicked()
{
	ExecuteClicked(false, GetCheck());
}

void CMFCButtonEx::ExecuteClicked(bool bPipe, int status)
{
	int id = GetDlgCtrlID();
	if (id == eTimeLineCtrl_AntEmg || id == eTimeLineCtrl_SndStop){
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000){
			//CTRLキーが押されている
			TRACE("CTRL\n");
		}
		else{
			return;
		}
	}
	else if (id == eTimeLineCtrl_Sound)
	{
		SetFaceColor(mStationColor[theApp.GetSelectStation()]);
	}
	else if (id == eTimeLineCtrl_Mute)
	{
		if (bPipe == true){
			SetCheck(status);
		}
		int bChk = GetCheck();
		if (bChk)
		{
			SetFaceColor(RGB(255, 255, 0));
		}
		else
		{
			SetFaceColor(mStationColor[theApp.GetSelectStation()]);
		}
	}

	(theApp.m_pMainWnd)->PostMessage(id, 0, (bPipe == false) ? GetCheck() : 9);
}

void CMFCButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	if (m_bButtonNormal == true){
		CMFCButton::DrawItem(lpDIS);
		return;
		CDC* pDCPaint = CDC::FromHandle(lpDIS->hDC);
		ASSERT_VALID(pDCPaint);

		CMemDC memDC(*pDCPaint, this);
		CDC* pDC = &memDC.GetDC();

		CRect rectClient = lpDIS->rcItem;

		OnFillBackground(pDC, rectClient);
		OnDrawBorder(pDC, rectClient, lpDIS->itemState);

		// Draw button content:
		OnDraw(pDC, rectClient, lpDIS->itemState);
		//CMFCButton::DrawItem(lpDIS);
		return;
	}

	UINT				state(lpDIS->itemState);
	CString				btnText;
	Graphics			graphics(lpDIS->hDC);
	RECT				rctBgContent;
	Rect				clientRect;
	CRect				txtRect(0, 0, 0, 0);
	LinearGradientBrush *plinGrBrush = NULL;

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	Color	clrDisabled = Color::MakeARGB(255, GetRValue(RGB(192, 192, 192)),
		GetGValue(RGB(192, 192, 192)),
		GetBValue(RGB(192, 192, 192)));

	UINT partID;
	UINT stateID;
	partID = DFC_BUTTON;
	stateID = (state & ODS_SELECTED) ? DFCS_BUTTONPUSH | DFCS_FLAT | DFCS_PUSHED : DFCS_BUTTONPUSH | DFCS_FLAT;
	DrawFrameControl(lpDIS->hDC, &lpDIS->rcItem, partID, stateID);
	memcpy(&rctBgContent, &lpDIS->rcItem, sizeof(RECT));
	InflateRect(&rctBgContent, -GetSystemMetrics(SM_CXEDGE), -GetSystemMetrics(SM_CYEDGE));

	Color	*pclrLower = NULL;
	clientRect.X = rctBgContent.left;
	clientRect.Y = rctBgContent.top;
	clientRect.Width = rctBgContent.right - rctBgContent.left;
	clientRect.Height = rctBgContent.bottom - rctBgContent.top;

	if (state & ODS_SELECTED)
	{
		pclrLower = &m_crLower;
		plinGrBrush = new LinearGradientBrush(clientRect, *pclrLower, m_crUpper, m_gradientAngle);

	}
	else if (state & ODS_DISABLED)
	{
		pclrLower = &clrDisabled;
		plinGrBrush = new LinearGradientBrush(clientRect, m_crUpper, *pclrLower, m_gradientAngle);
	}
	else
	{
		pclrLower = &m_crLower;
		plinGrBrush = new LinearGradientBrush(clientRect, m_crUpper, *pclrLower, m_gradientAngle);
	}

	graphics.FillRectangle(plinGrBrush, clientRect);

	// メニュー文字列フォントの作成
	CFont defFont, txtFont;
	CFont* oldFont = NULL;
	LOGFONT lf;
	if (((CFont*)&CMFCMenuBar::GetMenuFont())->GetLogFont(&lf)){
		lf.lfWeight = FW_NORMAL;
		lf.lfHeight = 14;
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
		else
			clrTxt = m_clrRegular;// m_crEnable;

		DrawText(lpDIS->hDC, btnText, btnText.GetLength(), &txtRect, DT_CALCRECT | DT_CENTER);

		int txtWidth = txtRect.Width();
		int txtHight = txtRect.Height();

		txtRect.left = clientRect.X + (clientRect.Width - txtWidth) / 2;
		txtRect.right = txtRect.left + txtWidth;
		txtRect.top = clientRect.Y + (clientRect.Height - txtHight) / 2;
		txtRect.bottom = txtRect.top + txtHight;

		txtRect.IntersectRect(&rctBgContent, &txtRect);

		clrOrg = ::SetTextColor(lpDIS->hDC, clrTxt);
		bkModeOld = SetBkMode(lpDIS->hDC, TRANSPARENT);

		DrawText(lpDIS->hDC, btnText, btnText.GetLength(), &txtRect, DT_CENTER);

		SetBkMode(lpDIS->hDC, bkModeOld);
		::SetTextColor(lpDIS->hDC, clrOrg);
	}

	delete plinGrBrush;

	if (oldFont != NULL){
		pDC->SelectObject(oldFont);
		txtFont.DeleteObject();
	}
}

void CMFCButtonEx::OnFillBackground(CDC* pDC, const CRect& rectClient)
{
	if (m_bTransparent)
	{
		// Copy background from the parent window
		GetGlobalData()->DrawParentBackground(this, pDC);
	}
	else
	{
		int id = GetDlgCtrlID();
		COLORREF color = m_clrFace;
		pDC->FillSolidRect(rectClient, color);
	}

	if (m_bChecked && m_bHighlightChecked && !(m_bPushed && m_bHighlighted))
	{
		CDrawingManager dm(*pDC);
		dm.HighlightRect(rectClient);
	}
}

void CMFCButtonEx::OnDraw(CDC* pDC, const CRect& rect, UINT uiState)
{
	CRect rectText = rect;
	CRect rectImage = rect;

	CString strText;
	GetWindowText(strText);

	if (m_sizeImage.cx != 0)
	{
		if (!strText.IsEmpty())
		{
			if (m_bTopImage)
			{
				rectImage.bottom = rectImage.top + m_sizeImage.cy + GetVertMargin();
				rectText.top = rectImage.bottom;
				rectText.bottom -= GetVertMargin();
			}
			else if (m_bRightImage)
			{
				rectText.right -= m_sizeImage.cx + GetImageHorzMargin() / 2;
				rectImage.left = rectText.right;
				rectImage.right -= GetImageHorzMargin() / 2;
			}
			else
			{
				rectText.left += m_sizeImage.cx + GetImageHorzMargin() / 2;
				rectImage.left += GetImageHorzMargin() / 2;
				rectImage.right = rectText.left;
			}
		}

		// Center image:
		rectImage.DeflateRect((rectImage.Width() - m_sizeImage.cx) / 2, max(0, (rectImage.Height() - m_sizeImage.cy) / 2));
	}
	else
	{
		rectImage.SetRectEmpty();
	}

	// Draw text:
	CFont* pOldFont = SelectFont(pDC);
	ENSURE(pOldFont != NULL);

	pDC->SetBkMode(TRANSPARENT);
	COLORREF clrText = m_clrRegular;

	//if (m_bHighlighted && m_clrHover != clrDefault)
	//{
	//	clrText = m_clrHover;
	//}

	UINT uiDTFlags = DT_END_ELLIPSIS;
	BOOL bIsSingleLine = FALSE;

	if (strText.Find(_T('\n')) < 0)
	{
		uiDTFlags |= DT_VCENTER | DT_SINGLELINE;
		bIsSingleLine = TRUE;
	}
	else
	{
		rectText.DeflateRect(0, GetVertMargin() / 2);
	}

	switch (m_nAlignStyle)
	{
	case ALIGN_LEFT:
		uiDTFlags |= DT_LEFT;
		rectText.left += GetImageHorzMargin() / 2;
		break;

	case ALIGN_RIGHT:
		uiDTFlags |= DT_RIGHT;
		rectText.right -= GetImageHorzMargin() / 2;
		break;

	case ALIGN_CENTER:
		uiDTFlags |= DT_CENTER;
	}

	if (GetExStyle() & WS_EX_LAYOUTRTL)
	{
		uiDTFlags |= DT_RTLREADING;
	}

	if ((uiState & ODS_DISABLED) && m_bGrayDisabled)
	{
		pDC->SetTextColor(GetGlobalData()->clrBtnHilite);

		CRect rectShft = rectText;
		rectShft.OffsetRect(1, 1);
		OnDrawText(pDC, rectShft, strText, uiDTFlags, uiState);

		clrText = GetGlobalData()->clrGrayedText;
	}

	pDC->SetTextColor(clrText);

	if (m_bDelayFullTextTooltipSet)
	{
		BOOL bIsFullText = pDC->GetTextExtent(strText).cx <= rectText.Width();
		SetTooltip(bIsFullText || !bIsSingleLine ? NULL : (LPCTSTR)strText);
		m_bDelayFullTextTooltipSet = FALSE;
	}

	OnDrawText(pDC, rectText, strText, uiDTFlags, uiState);

	// Draw image:
	if (!rectImage.IsRectEmpty())
	{
		if (m_nStdImageId != (CMenuImages::IMAGES_IDS) - 1)
		{
			CMenuImages::IMAGES_IDS id = m_nStdImageId;

			if ((uiState & ODS_DISABLED) && m_bGrayDisabled && m_nStdImageDisabledId != 0)
			{
				id = m_nStdImageDisabledId;
			}

			CMenuImages::Draw(pDC, id, rectImage.TopLeft(), m_StdImageState);
		}
		else
		{
			BOOL bIsDisabled = (uiState & ODS_DISABLED) && m_bGrayDisabled;

			CMFCToolBarImages& imageChecked = (bIsDisabled && m_ImageCheckedDisabled.GetCount() != 0) ? m_ImageCheckedDisabled :
				(m_bHighlighted && m_ImageCheckedHot.GetCount() != 0) ? m_ImageCheckedHot : m_ImageChecked;

			CMFCToolBarImages& image = (bIsDisabled && m_ImageDisabled.GetCount() != 0) ? m_ImageDisabled :
				(m_bHighlighted && m_ImageHot.GetCount() != 0) ? m_ImageHot : m_Image;

			if (m_bChecked && imageChecked.GetCount() != 0)
			{
				CAfxDrawState ds;

				imageChecked.PrepareDrawImage(ds);
				imageChecked.Draw(pDC, rectImage.left, rectImage.top, 0, FALSE, bIsDisabled && m_ImageCheckedDisabled.GetCount() == 0);
				imageChecked.EndDrawImage(ds);
			}
			else if (image.GetCount() != 0)
			{
				CAfxDrawState ds;

				image.PrepareDrawImage(ds);
				image.Draw(pDC, rectImage.left, rectImage.top, 0, FALSE, bIsDisabled && m_ImageDisabled.GetCount() == 0);
				image.EndDrawImage(ds);
			}
		}
	}

	pDC->SelectObject(pOldFont);
}

void CMFCButtonEx::OnDrawText(CDC* pDC, const CRect& rect, const CString& strText, UINT uiDTFlags, UINT uiState)
{
	ASSERT_VALID(pDC);

	CRect rectText = rect;
	pDC->DrawText(strText, rectText, uiDTFlags);
}

/*============================================================================*/
/*! グリッドボタン

-# 描画カラー属性の作成（３Ｄ用）

@param
@retval

*/
/*============================================================================*/
void CMFCButtonEx::CreateColors()
{
	COLORREF color;
	m_crLower = Color::MakeARGB(255,
		GetRValue(m_clrFace),
		GetGValue(m_clrFace),
		GetBValue(m_clrFace));
	m_crLowerHot = Color::MakeARGB(255,
		GetRValue(m_clrFace),
		GetGValue(m_clrFace),
		GetBValue(m_clrFace));
	if (m_bGradient == true){
		color = darkenColor(m_clrFace, 100);
		m_crUpper = Color::MakeARGB(255,
			GetRValue(color),
			GetGValue(color),
			GetBValue(color));
	}
	else{
		m_crUpper = Color::MakeARGB(255,
			GetRValue(m_clrFace),
			GetGValue(m_clrFace),
			GetBValue(m_clrFace));
	}
}

/*============================================================================*/
/*! グリッドボタン

-# 描画カラー属性の作成（３Ｄ用）

@param
@retval

*/
/*============================================================================*/
COLORREF CMFCButtonEx::lightenColor(const COLORREF crColor, BYTE blend)
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
COLORREF CMFCButtonEx::darkenColor(const COLORREF crColor, BYTE blend)
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


// CMFCCaptionBarEx

IMPLEMENT_DYNAMIC(CMFCCaptionBarEx, CMFCCaptionBar)

CMFCCaptionBarEx::CMFCCaptionBarEx()
{

}

CMFCCaptionBarEx::~CMFCCaptionBarEx()
{
}


BEGIN_MESSAGE_MAP(CMFCCaptionBarEx, CMFCCaptionBar)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CMFCCaptionBarEx メッセージ ハンドラー

#define OFFSET_EMGBUTTON_RIGHT	50
#define MAX_EMGBUTTON_WIDTH		120

void CMFCCaptionBarEx::OnPaint()
{
	CPaintDC dcPaint(this);
	CMemDC memDC(dcPaint, this);
	CDC& dc = memDC.GetDC();

	CRect rectClient;
	GetClientRect(rectClient);

	if (m_clrBarBackground == (COLORREF)-1)
		OnDrawBackground(&dc, rectClient);
	else{
		CDrawingManager dm(dc);
		dm.FillGradient(rectClient, m_clrBarBackground, m_clrBarBackground, TRUE);
	}

	int nOldBkMode = dc.SetBkMode(TRANSPARENT);
	COLORREF clrOldText = dc.SetTextColor(m_clrBarText != (COLORREF)-1 ? m_clrBarText : CMFCVisualManager::GetInstance()->GetCaptionBarTextColor(this));

	CFont* pOldFont = dc.SelectObject(m_hFont == NULL ? &(GetGlobalData()->fontRegular) : CFont::FromHandle(m_hFont));

	OnDrawButton(&dc, m_rectButton, m_strBtnText, m_bBtnEnabled);

	if (theApp.GetExecuteMode() == eExecuteMode_Monitor){
		// 監視モードの描画
		CString strMode = _T("【監視モード】");
		CRect rect = m_rectDrawText;
		CSize sz = dc.GetTextExtent(m_strText);
		rect.left -= (sz.cx * 2);
		sz = dc.GetTextExtent(strMode);
		rect.right = rect.left + sz.cx;
		OnDrawText(&dc, rect, strMode);
	}

	// 局名の描画
	OnDrawText(&dc, m_rectDrawText, m_strText);

	if (!m_rectImage.IsRectEmpty())
	{
		OnDrawImage(&dc, m_rectImage);
	}

	if (!m_rectClose.IsRectEmpty())
	{
		COLORREF clrText = CMFCVisualManager::GetInstance()->OnFillCaptionBarButton(&dc, this, m_rectClose, m_bIsCloseBtnPressed, m_bIsCloseBtnHighlighted, FALSE, FALSE, TRUE);

		CMenuImages::IMAGE_STATE imageState;

		if (GetRValue(clrText) > 192 && GetGValue(clrText) > 192 && GetBValue(clrText) > 192)
		{
			imageState = CMenuImages::ImageWhite;
		}
		else
		{
			imageState = CMenuImages::ImageBlack;
		}

		CMenuImages::Draw(&dc, CMenuImages::IdClose, m_rectClose, imageState);

		CMFCVisualManager::GetInstance()->OnDrawCaptionBarButtonBorder(&dc, this, m_rectClose, m_bIsCloseBtnPressed, m_bIsCloseBtnHighlighted, FALSE, FALSE, TRUE);
	}

	dc.SelectObject(pOldFont);
	dc.SetTextColor(clrOldText);
	dc.SetBkMode(nOldBkMode);


	int left = 0;
	left = OnDrawTime(&dc, true, false);
	CRect brect = rectClient;
	CRect arect = rectClient;
	// ボタンの再配置
	brect.right = left;
	brect.right -= 5;// OFFSET_EMGBUTTON_RIGHT;
	brect.left = brect.right - 32;
	mButtonHelp.MoveWindow(brect);
	arect = brect;
	brect.right = arect.left;
	brect.right -= 5;// OFFSET_EMGBUTTON_RIGHT;
	brect.left = brect.right - 32;
	mButtonMute.MoveWindow(brect);
	arect = brect;
	brect.right = arect.left;
	brect.right -= 5;// OFFSET_EMGBUTTON_RIGHT;
	brect.left = brect.right - 32;
	mButtonSound.MoveWindow(brect);
	arect = brect;
	brect.right = arect.left;
	brect.right -= 5;// OFFSET_EMGBUTTON_RIGHT;
	brect.left = brect.right - 32;
	mButtonShot.MoveWindow(brect);
	arect = brect;
	left = arect.left;
	CRect erect = rectClient;
	CRect srect = rectClient;
	// ボタンの再配置
	if (theApp.GetExecuteMode() == eExecuteMode_Control){
		erect.right = left;
		//	erect.right -= 5;// OFFSET_EMGBUTTON_RIGHT;
		erect.right -= 150;// OFFSET_EMGBUTTON_RIGHT;
		erect.left = erect.right - MAX_EMGBUTTON_WIDTH;
		erect.DeflateRect(2, 2);
		mButtonAnt.MoveWindow(erect);
		srect.right = erect.left - 2;
		srect.left = srect.right - MAX_EMGBUTTON_WIDTH;
		srect.DeflateRect(2, 2);
		mButtonSend.MoveWindow(srect);
	}
}

/*============================================================================*/
/*! キャプションバー

-# アプリケーション用の時間テキストを描画

@param	pDC			デバイスコンテキスト
@param	bUtc		UTC時間表示ON/OFF
@param	bJst		JST時間表示ON/OFF
@retval	int			描画後の左位置

*/
/*============================================================================*/
int CMFCCaptionBarEx::OnDrawTime(CDC* pDC, bool bUtc, bool bJst)
{
	CRect rectClient, rect;
	GetClientRect(rectClient);

	// バーの右側に時刻を表示
	CFont defFont, timeFont, dayFont;
	CFont* oldFont = NULL;
	LOGFONT lf;
	if (((CFont*)&CMFCMenuBar::GetMenuFont())->GetLogFont(&lf)){
		lf.lfWeight = FW_BOLD;
		lf.lfHeight = 20;
		timeFont.CreateFontIndirect(&lf);
		lf.lfWeight = FW_BOLD;
		lf.lfHeight = 16;
		dayFont.CreateFontIndirect(&lf);
		oldFont = pDC->SelectObject(&timeFont);
	}

	CString str;
	CTime time(CTime::GetCurrentTime());
	tm t1, t2;
	time.GetLocalTm(&t1);
	time.GetGmtTm(&t2);
	UINT nFormat = DT_RIGHT | DT_VCENTER;// | DT_SINGLELINE;
	pDC->SetTextColor(::GetSysColor(COLOR_MENUTEXT));
	pDC->SetBkColor(GetGlobalData()->clrBarShadow);
	pDC->SetBkMode(TRANSPARENT);

	CRect trect, drect;
	pDC->SelectObject(&timeFont);
	str = time.Format(_T("99:99:99 WWW "));
	pDC->DrawText(str, &trect, DT_CALCRECT);
	pDC->SelectObject(&dayFont);
	str = time.Format(_T("9999-99-99(999) "));
	pDC->DrawText(str, &drect, DT_CALCRECT);

	int left = rectClient.right;

	// UTC
	pDC->SelectObject(&timeFont);
	rect = rectClient;
	rect.left = rect.right - __max(trect.Width(), drect.Width());
	rect.bottom = rect.top + trect.Height();
	str = time.FormatGmt(_T("%H:%M:%S UTC "));
	if (bUtc == true){
		pDC->DrawText(str, rect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	}
	pDC->SelectObject(&dayFont);
	rect = rectClient;
	rect.left = rect.right - __max(trect.Width(), drect.Width());
	rect.top = rect.top + trect.Height();
	rect.bottom = rect.top + drect.Height();
	str = time.FormatGmt(_T("%Y-%m-%d(%j) "));
	if (bUtc == true){
		pDC->DrawText(str, rect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		left = rect.left;
	}
	// JST
	pDC->SelectObject(&timeFont);
	rect = rectClient;
	if (bUtc == true){
		rect.right = rect.right - __max(trect.Width(), drect.Width());
	}
	rect.left = rect.right - __max(trect.Width(), drect.Width());
	rect.bottom = rect.top + trect.Height();
	str = time.Format(_T("%H:%M:%S JST "));
	if (bJst == true){
		pDC->DrawText(str, rect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	}
	pDC->SelectObject(&dayFont);
	rect = rectClient;
	if (bUtc == true){
		rect.right = rect.right - __max(trect.Width(), drect.Width());
	}
	rect.left = rect.right - __max(trect.Width(), drect.Width());
	rect.top = rect.top + trect.Height();
	rect.bottom = rect.top + drect.Height();
	str = time.Format(_T("%Y-%m-%d(%j) "));
	if (bJst == true){
		pDC->DrawText(str, rect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		left = rect.left;
	}

	if (oldFont != NULL)
		pDC->SelectObject(oldFont);

	return left;
}

void CMFCCaptionBarEx::OnDrawButton(CDC* pDC, CRect rect, const CString& strButton, BOOL bEnabled)
{
	const int nMenuArrowWidth = 10;
	const int nButtonHorzMargin = 10;
	const int nButtonVertMargin = 5;
	const int nMessageBarMargin = 4;

	const int nIdToolTipClose = 1;
	const int nIdToolTipText = 2;
	const int nIdToolTipImage = 3;
	const int nIdToolTipButton = 4;

	ASSERT_VALID(pDC);

	COLORREF clrText = CMFCVisualManager::GetInstance()->OnFillCaptionBarButton(pDC, this, rect, m_bIsBtnPressed || m_bIsBtnForcePressed, m_bIsBtnHighlighted, !m_bBtnEnabled, m_bBtnHasDropDownArrow, FALSE);

	CRect rectText = rect;
	rectText.DeflateRect(m_nHorzElementOffset, 0);

	if (m_bIsMessageBarMode)
	{
		rectText.DeflateRect(nButtonHorzMargin, 0);
	}

	if (m_uiBtnID != 0 && bEnabled && m_bBtnHasDropDownArrow)
	{
		rectText.right -= nMenuArrowWidth;
	}

	COLORREF clrTextOld = (COLORREF)-1;
	if (clrText != (COLORREF)-1)
	{
		clrTextOld = pDC->SetTextColor(clrText);
	}

	pDC->DrawText(strButton, rectText, DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);

	if (clrTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor(clrTextOld);
	}

	if (m_uiBtnID != 0 && bEnabled)
	{
		if (m_uiBtnID != 0 && m_bBtnHasDropDownArrow)
		{
			// Draw menu triangle:
			CRect rectArrow = rect;
			rectArrow.bottom -= m_nMargin;
			rectArrow.top = rectArrow.bottom - nMenuArrowWidth;
			rectArrow.left = rectText.right;

			int iXMiddle = rectArrow.left + rectArrow.Width() / 2;

			rectArrow.DeflateRect(0, rectArrow.Height() / 3);
			rectArrow.DeflateRect(rectArrow.Height() / 3, rectArrow.Height() / 3);
			rectArrow.left = iXMiddle - rectArrow.Height() - 1;
			rectArrow.right = iXMiddle + rectArrow.Height() + 1;

			int iHalfWidth = (rectArrow.Width() % 2 != 0) ? (rectArrow.Width() - 1) / 2 : rectArrow.Width() / 2;

			CPoint pts[3];
			pts[0].x = rectArrow.left;
			pts[0].y = rectArrow.top;
			pts[1].x = rectArrow.right;
			pts[1].y = rectArrow.top;
			pts[2].x = rectArrow.left + iHalfWidth;
			pts[2].y = rectArrow.bottom + 1;

			CBrush brArrow(pDC->GetTextColor());

			CPen* pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);
			CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&brArrow);

			pDC->SetPolyFillMode(WINDING);
			pDC->Polygon(pts, 3);

			pDC->SelectObject(pOldBrush);
			pDC->SelectObject(pOldPen);
		}

		CMFCVisualManager::GetInstance()->OnDrawCaptionBarButtonBorder(pDC, this, rect, m_bIsBtnPressed || m_bIsBtnForcePressed, m_bIsBtnHighlighted, !m_bBtnEnabled, m_bBtnHasDropDownArrow, FALSE);
	}
}


int CMFCCaptionBarEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCCaptionBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (theApp.GetExecuteMode() == eExecuteMode_Control){
		// アンテナ強制停止ボタン作成
		mButtonAnt.Create(_T("EMERGENCY STOP\n[アンテナ]"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_NOTIFY, CRect(0, 0, 1, 1), this, eTimeLineCtrl_AntEmg);
		mButtonAnt.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		mButtonAnt.m_bButtonNormal = false;
		mButtonAnt.m_bDrawFocus = false;
		mButtonAnt.SetTextColor(RGB(255, 255, 0));
		mButtonAnt.m_bTransparent = false;
		mButtonAnt.SetFaceColor(RGB(255, 0, 0));
		mButtonAnt.CreateColors();
		// 送信機停止ボタン作成
		mButtonSend.Create(_T("EMERGENCY STOP\n[送信機]"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_NOTIFY, CRect(0, 0, 1, 1), this, eTimeLineCtrl_SndStop);
		mButtonSend.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		mButtonSend.m_bButtonNormal = false;
		mButtonSend.m_bDrawFocus = false;
		mButtonSend.SetTextColor(RGB(255, 255, 0));
		mButtonSend.m_bTransparent = false;
		mButtonSend.SetFaceColor(RGB(255, 0, 0));
		mButtonSend.CreateColors();
	}
	// スクリーンショットボタンの作成
	mButtonShot.Create(_T(""), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_NOTIFY, CRect(0, 0, 1, 1), this, eTimeLineCtrl_SnapShot);
	mButtonShot.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	mButtonShot.m_bDrawFocus = false;
	mButtonShot.m_bTransparent = false;
	mButtonShot.m_bDontUseWinXPTheme = true;
	HICON icon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SNAP), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	mButtonShot.SetIcon(icon);
	// サウンドボタンの作成
	mButtonSound.Create(_T(""), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_NOTIFY, CRect(0, 0, 1, 1), this, eTimeLineCtrl_Sound);
	mButtonSound.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	mButtonSound.m_bDrawFocus = false;
	mButtonSound.m_bTransparent = false;
	mButtonSound.m_bDontUseWinXPTheme = true;
	icon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SOUND), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	mButtonSound.SetIcon(icon);
	// ミュートボタンの作成
	mButtonMute.Create(_T(""), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_NOTIFY, CRect(0, 0, 1, 1), this, eTimeLineCtrl_Mute);
	mButtonMute.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	mButtonMute.m_bDrawFocus = false;
	mButtonMute.m_bTransparent = false;
	mButtonMute.m_bDontUseWinXPTheme = true;
	icon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_MUTE), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	mButtonMute.SetIcon(icon);
	// ヘルプボタンの作成
	mButtonHelp.Create(_T(""), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_NOTIFY, CRect(0, 0, 1, 1), this, eTimeLineCtrl_Help);
	mButtonHelp.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	mButtonHelp.m_bDrawFocus = false;
	mButtonHelp.m_bTransparent = false;
	mButtonHelp.m_bDontUseWinXPTheme = true;
	icon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_HELP), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	mButtonHelp.SetIcon(icon);

	mButtonShot.SetFaceColor(mTimeLineStationColor[theApp.GetSelectStation()]);
	mButtonSound.SetFaceColor(mTimeLineStationColor[theApp.GetSelectStation()]);
	mButtonMute.SetFaceColor(mTimeLineStationColor[theApp.GetSelectStation()]);
	mButtonHelp.SetFaceColor(mTimeLineStationColor[theApp.GetSelectStation()]);

	return 0;
}
