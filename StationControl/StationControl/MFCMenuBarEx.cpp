// MFCMenuBarEx.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "MFCMenuBarEx.h"
#include "MainFrm.h"

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
	if (id == eCtrl_AntEmg || id == eCtrl_SndStop){
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000){
			//CTRLキーが押されている
			TRACE("CTRL\n");
		}
		else{
			return;
		}
	}
	else if (id == eCtrl_Sound)
	{
		if (theApp.GetSelectStationMode() == eStation_1)
		{
			m_bTransparent = false;
			SetFaceColor(mStationColor[theApp.GetSelectStation()]);
		}
		else
		{
			m_bTransparent = true;
			SetFaceColor(RGB(105, 105, 105));
		}
	}
	else if (id == eCtrl_Mute)
	{
		if (bPipe == true){
			SetCheck(status);
		}
		int bChk = GetCheck();
		if (bChk)
		{
			m_bTransparent = false;
			SetFaceColor(RGB(255, 255, 0));
		}
		else
		{
			if (theApp.GetSelectStationMode() == eStation_1)
			{
				m_bTransparent = false;
				SetFaceColor(mStationColor[theApp.GetSelectStation()]);
			}
			else
			{
				m_bTransparent = true;
				SetFaceColor(RGB(105, 105, 105));
			}
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



// CMFCMenuBarEx

IMPLEMENT_DYNAMIC(CMFCMenuBarEx, CMFCMenuBar)

CMFCMenuBarEx::CMFCMenuBarEx()
{
}

CMFCMenuBarEx::~CMFCMenuBarEx()
{
}


BEGIN_MESSAGE_MAP(CMFCMenuBarEx, CMFCMenuBar)
	//ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(eCtrl_AntEmg, &CMFCMenuBarEx::OnCommandEmg)
	ON_COMMAND(eCtrl_SndStop, &CMFCMenuBarEx::OnCommandStop)
	ON_UPDATE_COMMAND_UI(eCtrl_AntEmg, &CMFCMenuBarEx::OnUpdateCommandEmg)
	ON_UPDATE_COMMAND_UI(eCtrl_SndStop, &CMFCMenuBarEx::OnUpdateCommandStop)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CMFCMenuBarEx メッセージ ハンドラー



int CMFCMenuBarEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCMenuBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// アンテナ強制停止ボタン作成
	mButtonAnt.Create(_T("ANT. EMG"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_NOTIFY, CRect(0, 0, 1, 1), this, eCtrl_AntEmg);
	mButtonAnt.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	mButtonAnt.m_bDrawFocus = false;
	mButtonAnt.SetTextColor(RGB(255, 255, 255));
	mButtonAnt.m_bTransparent = false;
	mButtonAnt.SetFaceColor(RGB(255, 0, 0));
	mButtonAnt.CreateColors();
		// 送信機停止ボタン作成
	mButtonSend.Create(_T("SND. STOP"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_NOTIFY, CRect(0, 0, 1, 1), this, eCtrl_SndStop);
	mButtonSend.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	mButtonSend.m_bDrawFocus = false;
	mButtonSend.SetTextColor(RGB(0, 0, 0));
	mButtonSend.m_bTransparent = false;
	mButtonSend.SetFaceColor(RGB(255, 255, 0));
	mButtonSend.CreateColors();
	return 0;
}

void CMFCMenuBarEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	// 描画メッセージで CMFCMenuBar::OnPaint() を呼び出さないでください。
}


BOOL CMFCMenuBarEx::OnEraseBkgnd(CDC* pDC)
{
	//return CMFCMenuBar::OnEraseBkgnd(pDC);
	CRect rectClient;
	GetClientRect(rectClient);

	CPoint ptCursor;
	::GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);

	// メニューバー全体を塗り潰し
	pDC->FillSolidRect(&rectClient, ::GetSysColor(COLOR_MENUBAR));

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

	pDC->SetBkMode(TRANSPARENT);

	UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	const CObList& oblist = GetAllButtons();
	int iButton = 0;
	// メニュー文字列の描画
	for (POSITION pos = oblist.GetHeadPosition(); pos != NULL; iButton++)
	{
		CMFCToolBarButton* pButton = (CMFCToolBarButton*)oblist.GetNext(pos);
		ENSURE(pButton != NULL);
		ASSERT_VALID(pButton);
		CRect rect = pButton->Rect();
		if (rect.PtInRect(ptCursor)){
			pDC->SetTextColor(::GetSysColor(COLOR_MENUHILIGHT));
			pDC->SetBkColor(::GetSysColor(COLOR_MENUHILIGHT));
		}
		else{
			pDC->SetTextColor(::GetSysColor(COLOR_MENUTEXT));
			pDC->SetBkColor(::GetSysColor(COLOR_MENUBAR));
		}

		CString str = pButton->m_strText;
		pDC->DrawText(str, rect, nFormat);
	}

	SYSTEMTIME time;
	GetLocalTime(&time);

	if (oldFont != NULL){
		pDC->SelectObject(oldFont);
		txtFont.DeleteObject();
	}

	lf.lfWeight = FW_BOLD;
	lf.lfHeight = 16;
	txtFont.CreateFontIndirect(&lf);
	oldFont = pDC->SelectObject(&txtFont);

	CRect erect = rectClient;
	CRect srect = rectClient;

	// ボタンの再配置
	erect.right -= OFFSET_EMGBUTTON_RIGHT;
	erect.left = erect.right - MAX_EMGBUTTON_WIDTH;
	erect.DeflateRect(2, 2);
	mButtonAnt.MoveWindow(erect);
	srect.right = erect.left - 2;
	srect.left = srect.right - MAX_EMGBUTTON_WIDTH;
	srect.DeflateRect(2, 2);
	mButtonSend.MoveWindow(srect);

	if (oldFont != NULL){
		pDC->SelectObject(oldFont);
		txtFont.DeleteObject();
	}

	return TRUE;

	return CMFCMenuBar::OnEraseBkgnd(pDC);
}


void CMFCMenuBarEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CMFCMenuBar::OnLButtonDown(nFlags, point);
	SetCursor(theApp.LoadStandardCursor(IDC_ARROW));
}

AFX_DOCK_TYPE CMFCMenuBarEx::GetDockingMode()const
{
	return DT_SMART;
}

void CMFCMenuBarEx::SetPaneStyle(DWORD dwNewStyle)
{
	m_dwStyle = dwNewStyle & ~CBRS_GRIPPER;
}

void CMFCMenuBarEx::OnCommandEmg()
{
}

void CMFCMenuBarEx::OnCommandStop()
{
}

void CMFCMenuBarEx::OnUpdateCommandEmg(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMFCMenuBarEx::OnUpdateCommandStop(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
