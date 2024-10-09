// MFCCaptionBarEx.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "MFCCaptionBarEx.h"


// CMFCCaptionBarEx

// CMFCCaptionBarEx

IMPLEMENT_DYNAMIC(CMFCCaptionBarEx, CMFCCaptionBar)

CMFCCaptionBarEx::CMFCCaptionBarEx()
{
	mChildCaption = false;
	mStationNo = 0;
}

CMFCCaptionBarEx::~CMFCCaptionBarEx()
{
}


BEGIN_MESSAGE_MAP(CMFCCaptionBarEx, CMFCCaptionBar)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMFCCaptionBarEx メッセージ ハンドラー

void CMFCCaptionBarEx::OnPaint()
{
	CPaintDC dcPaint(this);
	CMemDC memDC(dcPaint, this);
	CDC& dc = memDC.GetDC();

	CRect rectClient;
	GetClientRect(rectClient);

	if(m_clrBarBackground == (COLORREF)-1)
		OnDrawBackground(&dc, rectClient);
	else{
		CDrawingManager dm(dc);
		dm.FillGradient(rectClient, m_clrBarBackground, m_clrBarBackground, TRUE);
	}

	int nOldBkMode = dc.SetBkMode(TRANSPARENT);
	COLORREF clrOldText = dc.SetTextColor(m_clrBarText != (COLORREF)-1 ? m_clrBarText : CMFCVisualManager::GetInstance()->GetCaptionBarTextColor(this));

	CFont* pOldFont = dc.SelectObject(m_hFont == NULL ? &(GetGlobalData()->fontRegular) : CFont::FromHandle(m_hFont));

	if (mChildCaption == false){
		OnDrawButton(&dc, m_rectButton, m_strBtnText, m_bBtnEnabled);
	}

	if (theApp.GetSelectStationMode() == eStation_1 && theApp.GetExecuteMode() == eExecuteMode_Monitor){
		// 監視モードの描画
		CString strMode = _T("【監視モード】");
		CRect rect = m_rectDrawText;
		CSize sz = dc.GetTextExtent(m_strText);
		rect.left -= (sz.cx*2);
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
	if (mChildCaption == false){
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
		if (theApp.GetSelectStationMode() == eStation_1 && theApp.GetExecuteMode() == eExecuteMode_Control){
			CRect erect = rectClient;
			CRect srect = rectClient;
			// ボタンの再配置
			erect.right = left;
//			erect.right -= 5;// OFFSET_EMGBUTTON_RIGHT;
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

	if (mChildCaption == false){
		if (theApp.GetSelectStationMode() == eStation_1 && theApp.GetExecuteMode() == eExecuteMode_Control){
			// アンテナ強制停止ボタン作成
			mButtonAnt.Create(_T("EMERGENCY STOP\n[アンテナ]"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_NOTIFY, CRect(0, 0, 1, 1), this, eCtrl_AntEmg);
			mButtonAnt.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
			mButtonAnt.m_bButtonNormal = false;
			mButtonAnt.m_bDrawFocus = false;
			mButtonAnt.SetTextColor(RGB(255, 255, 0));
			mButtonAnt.m_bTransparent = false;
			mButtonAnt.SetFaceColor(RGB(255, 0, 0));
			mButtonAnt.CreateColors();
			// 送信機停止ボタン作成
			mButtonSend.Create(_T("EMERGENCY STOP\n[送信機]"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_NOTIFY, CRect(0, 0, 1, 1), this, eCtrl_SndStop);
			mButtonSend.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
			mButtonSend.m_bButtonNormal = false;
			mButtonSend.m_bDrawFocus = false;
			mButtonSend.SetTextColor(RGB(255, 255, 0));
			mButtonSend.m_bTransparent = false;
			mButtonSend.SetFaceColor(RGB(255, 0, 0));
			mButtonSend.CreateColors();
		}
		// スクリーンショットボタンの作成
		mButtonShot.Create(_T(""), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_NOTIFY, CRect(0, 0, 1, 1), this, eCtrl_SnapShot);
		mButtonShot.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		mButtonShot.m_bDrawFocus = false;
		mButtonShot.m_bTransparent = (theApp.GetSelectStationMode() == eStation_1)?false:true;
		mButtonShot.m_bDontUseWinXPTheme = true;
		HICON icon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SNAP), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
		mButtonShot.SetIcon(icon);
		// サウンドボタンの作成
		mButtonSound.Create(_T(""), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_NOTIFY, CRect(0, 0, 1, 1), this, eCtrl_Sound);
		mButtonSound.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		mButtonSound.m_bDrawFocus = false;
		mButtonSound.m_bTransparent = (theApp.GetSelectStationMode() == eStation_1) ? false : true;
		mButtonSound.m_bDontUseWinXPTheme = true;
		icon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SOUND), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
		mButtonSound.SetIcon(icon);
		// ミュートボタンの作成
		mButtonMute.Create(_T(""), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_NOTIFY, CRect(0, 0, 1, 1), this, eCtrl_Mute);
		mButtonMute.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		mButtonMute.m_bDrawFocus = false;
		mButtonMute.m_bTransparent = (theApp.GetSelectStationMode() == eStation_1) ? false : true;
		mButtonMute.m_bDontUseWinXPTheme = true;
		icon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_MUTE), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
		mButtonMute.SetIcon(icon);
		// ヘルプボタンの作成
		mButtonHelp.Create(_T(""), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_NOTIFY, CRect(0, 0, 1, 1), this, eCtrl_Help);
		mButtonHelp.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		mButtonHelp.m_bDrawFocus = false;
		mButtonHelp.m_bTransparent = (theApp.GetSelectStationMode() == eStation_1) ? false : true;
		mButtonHelp.m_bDontUseWinXPTheme = true;
		icon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_HELP), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
		mButtonHelp.SetIcon(icon);

		if (theApp.GetSelectStationMode() == eStation_1){
			mButtonShot.SetFaceColor(mStationColor[theApp.GetSelectStation()]);
			mButtonSound.SetFaceColor(mStationColor[theApp.GetSelectStation()]);
			mButtonMute.SetFaceColor(mStationColor[theApp.GetSelectStation()]);
			mButtonHelp.SetFaceColor(mStationColor[theApp.GetSelectStation()]);
		}
	}

	return 0;
}

void CMFCCaptionBarEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (theApp.GetSelectStationMode() == eStation_1){
		// １局モード時は何もしない
		CMFCCaptionBar::OnLButtonDown(nFlags, point);
		return;
	}

	const UINT mEventId[] = {
		ID_STATION_USUDA64,
		ID_STATION_USUDA2,
		ID_STATION_UCHINOURA34,
		ID_STATION_UCHINOURA20,
	};

	// ５局モード時の処理
	if (nFlags & MK_CONTROL){
		(theApp.m_pMainWnd)->PostMessage(WM_COMMAND, mEventId[mStationNo], 0);
	}

	CMFCCaptionBar::OnLButtonDown(nFlags, point);
}
