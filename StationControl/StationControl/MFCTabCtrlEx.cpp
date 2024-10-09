#include "stdafx.h"
#include "StationControl.h"
#include "MFCTabCtrlEx.h"


// CMFCTabCtrlEx

IMPLEMENT_DYNAMIC(CMFCTabCtrlEx, CMFCTabCtrl)

CMFCTabCtrlEx::CMFCTabCtrlEx()
{
	m_bFlat = true;
	m_strFilterText.Empty();
}

CMFCTabCtrlEx::~CMFCTabCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CMFCTabCtrlEx, CMFCTabCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
//	ON_WM_NCPAINT()
END_MESSAGE_MAP()



// CMFCTabCtrlEx メッセージ ハンドラ

int CMFCTabCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;


	m_nTabShow = -1;
	GetWindowRect(m_rectDefault);
	GetParent()->ScreenToClient(m_rectDefault);
	mBarChildList.clear();

	return 0;
}

void CMFCTabCtrlEx::AddBar(CBarChild* pBar, CString str)
{
	mBarChildList.push_back(pBar);
	AddTab(pBar, str, (int)mBarChildList.size() - 1, TRUE);
	pBar->ShowWindow(SW_HIDE);

	RecalcLayout();
	RedrawWindow();

	m_nTabShow = -1;
	GetWindowRect(m_rectDefault);
	GetParent()->ScreenToClient(m_rectDefault);

	RecalcLayout();
	RedrawWindow();

	HideInactiveWindow(TRUE);
	EnableCustomToolTips(TRUE);
}

void CMFCTabCtrlEx::Resize(int cx)
{
	CRect rect;
	GetWindowRect(rect);
	rect.right = rect.left + cx;
	ScreenToClient(rect);
	MoveWindow(rect);

	vector<CBarChild*>::iterator itr;
	for (itr = mBarChildList.begin(); itr != mBarChildList.end(); itr++){
		(*itr)->ShowWindow(SW_HIDE);
	}
	RecalcLayout();
	RedrawWindow();
}


void CMFCTabCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	int pos = GetTabFromPoint(point);

	if (pos < (int)mBarChildList.size()){
		if (m_nTabShow == pos){
			TabChange(-1);
		}
		else{
			CMFCTabCtrl::OnLButtonDown(nFlags, point);
			TabChange(pos);
		}
	}
	else{
		TabChange(-1);
	}
}

/*============================================================================*/
/*! 拡張タブコントロール
タブの切り替え

@param[in]  pos			アクティブタブ番号
@param[in]  bShow		タブウィンドウの表示/非表示

@retval なし
*/
/*============================================================================*/
void CMFCTabCtrlEx::TabChange(int pos, bool bShow)
{
	if (pos >= 0){
		CRect rectWnd;
		GetWindowRect(rectWnd);
		GetParent()->ScreenToClient(rectWnd);
		CRect rect = m_rectDefault;
		rect.top += GetTabsHeight();
		rect.right = rect.left + GetMaxWidth();
		CWnd *pWndActive;
		rect.bottom = rect.top + mBarChildList[pos]->GetMaxHeight();
		pWndActive = mBarChildList[pos];
		rect.OffsetRect(-rectWnd.left, -rectWnd.top);
		ClientToScreen(rect);
		pWndActive->SetWindowPos(&wndTop,
			rect.left,
			rect.top,
			rect.Width(),
			rect.Height(),
			(bShow) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW);
	}
	else{
		vector<CBarChild*>::iterator itr;
		for (itr = mBarChildList.begin(); itr != mBarChildList.end(); itr++){
			(*itr)->ShowWindow(SW_HIDE);
		}
	}

	m_nTabShow = pos;
}

/*============================================================================*/
/*! 拡張タブコントロール
マウス位置から選択タブ番号を取得する

@param[in]  pt			マウス位置

@retval なし
*/
/*============================================================================*/
int CMFCTabCtrlEx::GetTabFromPointEx(CPoint& pt)
{
	int pos = GetTabFromPoint(pt);
	return pos;
}

/*============================================================================*/
/*! 拡張タブコントロール
タブウィンドウの非表示

@param[in]  pMsg	ウィンドウメッセージ

@retval なし
*/
/*============================================================================*/
void CMFCTabCtrlEx::CloseChildWindow(MSG* pMsg)
{
	if (pMsg == NULL){
		TabChange(-1);
		return;
	}

	if (pMsg->hwnd == m_hWnd)
		return;

	CWnd *p = CWnd::FromHandle(pMsg->hwnd);
	if (p->GetSafeHwnd() == false)
		return;

	vector<CBarChild*>::iterator itr;
	for (itr = mBarChildList.begin(); itr != mBarChildList.end(); itr++){
		if (pMsg->hwnd == (*itr)->m_hWnd)
			return;

		CRect rect;
		(*itr)->GetWindowRect(rect);
		if (rect.PtInRect(pMsg->pt))
			return;

		//CPoint pos;
		//GetCursorPos(&pos);
		//if (rect.PtInRect(pos))
		//	return;

		if (p->GetOwner() == (*itr))
			return;
		if (p->GetParent() == (*itr))
			return;

		CWnd *pwnd = (*itr)->GetWindow(GW_CHILD);
		while (pwnd != NULL){
			if (pMsg->hwnd == pwnd->m_hWnd)
				return;
			if (p->GetParent() == pwnd)
				return;
			if (p->GetOwner() == pwnd)
				return;
			if (p == pwnd->GetWindow(GW_CHILD))
				return;
			pwnd = pwnd->GetWindow(GW_HWNDNEXT);
		}
	}

	TabChange(-1);
}

/*============================================================================*/
/*! 拡張タブコントロール
タブウィンドウの最大幅の取得

@param[in]

@retval なし
*/
/*============================================================================*/
UINT CMFCTabCtrlEx::GetMaxWidth()
{
	CRect rect;
	GetClientRect(rect);
	return rect.Width();

	UINT width = 0;
	vector<CBarChild*>::iterator itr;
	for (itr = mBarChildList.begin(); itr != mBarChildList.end(); itr++){
		if (width < (*itr)->GetMaxWidth())
			width = (*itr)->GetMaxWidth();
	}
	return width;
}

/*============================================================================*/
/*! 拡張タブコントロール
タブウィンドウの最大高さの取得

@param[in]

@retval なし
*/
/*============================================================================*/
UINT CMFCTabCtrlEx::GetMaxHeight()
{
	UINT height = 0;
	vector<CBarChild*>::iterator itr;
	for (itr = mBarChildList.begin(); itr != mBarChildList.end(); itr++){
		if (height < (*itr)->GetMaxHeight())
			height = (*itr)->GetMaxHeight();
	}
	return height;
}

void CMFCTabCtrlEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CMemDC memDC(dc, this);
	CDC* pDC = &memDC.GetDC();

	dc.GetClipBox(&m_rectCurrClip);

	COLORREF clrDark;
	COLORREF clrBlack;
	COLORREF clrHighlight;
	COLORREF clrFace;
	COLORREF clrDarkShadow;
	COLORREF clrLight;
	CBrush* pbrFace = NULL;
	CBrush* pbrBlack = NULL;

	CMFCVisualManager::GetInstance()->GetTabFrameColors(this, clrDark, clrBlack, clrHighlight, clrFace, clrDarkShadow, clrLight, pbrFace, pbrBlack);

	ASSERT_VALID(pbrFace);
	ASSERT_VALID(pbrBlack);

	CRect rectClient;
	GetClientRect(&rectClient);

	CBrush* pOldBrush = pDC->SelectObject(pbrFace);
	ENSURE(pOldBrush != NULL);

	CPen penDark(PS_SOLID, 1, clrDark);
	CPen penBlack(PS_SOLID, 1, clrBlack);
	CPen penHiLight(PS_SOLID, 1, clrHighlight);

	CPen* pOldPen = (CPen*)pDC->SelectObject(&penDark);
	ENSURE(pOldPen != NULL);

	const int nTabBorderSize = GetTabBorderSize();

	CRect rectTabs = rectClient;

	if (m_location == LOCATION_BOTTOM)
	{
		rectTabs.top = m_rectTabsArea.top;
	}
	else
	{
		rectTabs.bottom = m_rectTabsArea.bottom;
	}

	pDC->ExcludeClipRect(m_rectWndArea);

	BOOL bBackgroundIsReady = CMFCVisualManager::GetInstance()->OnEraseTabsFrame(pDC, rectClient, this);


	if (!m_bDrawFrame && !bBackgroundIsReady)
	{
		pDC->FillRect(rectClient, pbrFace);
	}

	// ここで背景を塗りつぶしている
	// CMFCVisualManagerを呼びだすとVisualスタイルに合わせて背景を塗りつぶしてしまう。
	// OnEraseTabsAreaをWTViewerEfree用にカスタマイズする
	CDrawingManager dm(*pDC);
	COLORREF clr1 = RGB(191, 219, 255);
	COLORREF clr2 = RGB(191, 219, 255);
	dm.FillGradient(rectTabs, clr2, clr1, TRUE);

	CRect rectFrame = rectClient;

	if (nTabBorderSize == 0)
	{
		if (m_location == LOCATION_BOTTOM)
		{
			rectFrame.bottom = m_rectTabsArea.top + 1;
		}
		else
		{
			rectFrame.top = m_rectTabsArea.bottom - 1;
		}

		if (m_bFlat)
		{
			pDC->FrameRect(&rectFrame, pbrBlack);
		}
		else
		{
			pDC->FrameRect(&rectFrame, pbrFace);
		}
	}
	else
	{
		int yLine = m_location == LOCATION_BOTTOM ? m_rectTabsArea.top : m_rectTabsArea.bottom;

		if (!m_bFlat)
		{
			if (m_location == LOCATION_BOTTOM)
			{
				rectFrame.bottom = m_rectTabsArea.top;
			}
			else
			{
				rectFrame.top = m_rectTabsArea.bottom;
			}
		}

		//-----------------------------------------------------
		// Draw wide 3-dimensional frame around the Tabs area:
		//-----------------------------------------------------
		if (m_bFlatFrame)
		{
			CRect rectBorder(rectFrame);

			if (m_bFlat)
			{
				if (m_location == LOCATION_BOTTOM)
				{
					rectBorder.bottom = m_rectTabsArea.top + 1;
				}
				else
				{
					rectBorder.top = m_rectTabsArea.bottom - 1;
				}
			}

			rectFrame.DeflateRect(1, 1);

			if (m_bDrawFrame && !bBackgroundIsReady && rectFrame.Width() > 0 && rectFrame.Height() > 0)
			{
				pDC->PatBlt(rectFrame.left, rectFrame.top, nTabBorderSize, rectFrame.Height(), PATCOPY);
				pDC->PatBlt(rectFrame.left, rectFrame.top, rectFrame.Width(), nTabBorderSize, PATCOPY);
				pDC->PatBlt(rectFrame.right - nTabBorderSize - 1, rectFrame.top, nTabBorderSize + 1, rectFrame.Height(), PATCOPY);
				pDC->PatBlt(rectFrame.left, rectFrame.bottom - nTabBorderSize, rectFrame.Width(), nTabBorderSize, PATCOPY);

				if (m_location == LOCATION_BOTTOM)
				{
					pDC->PatBlt(rectFrame.left, m_rectWndArea.bottom, rectFrame.Width(), rectFrame.bottom - m_rectWndArea.bottom, PATCOPY);
				}
				else
				{
					pDC->PatBlt(rectFrame.left, rectFrame.top, rectFrame.Width(), m_rectWndArea.top - rectFrame.top, PATCOPY);
				}
			}

			if (m_bFlat)
			{
				//---------------------------
				// Draw line below the tabs:
				//---------------------------
				pDC->SelectObject(&penBlack);
				pDC->MoveTo(rectFrame.left + nTabBorderSize, yLine);
				pDC->LineTo(rectFrame.right - nTabBorderSize, yLine);
			}

			pDC->Draw3dRect(&rectBorder, clrFace, clrFace);

			if (GetTabsHeight() == 0)
			{
				pDC->Draw3dRect(&rectBorder, clrFace, clrFace);
			}
			else
			{
				if (m_bDrawFrame)
				{
					pDC->Draw3dRect(&rectBorder, clrDark, clrDark);
				}

				if (!m_bIsOneNoteStyle)
				{
					int xRight = rectBorder.right - 1;

					if (!m_bDrawFrame)
					{
						xRight -= nTabBorderSize;
					}

					if (m_location == LOCATION_BOTTOM)
					{
						pDC->SelectObject(&penBlack);

						pDC->MoveTo(rectBorder.left, rectBorder.bottom - 1);
						pDC->LineTo(xRight, rectBorder.bottom - 1);
					}
					else
					{
						pDC->SelectObject(&penHiLight);

						pDC->MoveTo(rectBorder.left, rectBorder.top);
						pDC->LineTo(xRight, rectBorder.top);
					}
				}
			}
		}
		else
		{
			if (m_bDrawFrame)
			{
				pDC->Draw3dRect(&rectFrame, clrHighlight, clrDarkShadow);

				rectFrame.DeflateRect(1, 1);
				pDC->Draw3dRect(&rectFrame, clrLight, clrDark);

				rectFrame.DeflateRect(1, 1);

				if (!bBackgroundIsReady && rectFrame.Width() > 0 && rectFrame.Height() > 0)
				{
					pDC->PatBlt(rectFrame.left, rectFrame.top, nTabBorderSize, rectFrame.Height(), PATCOPY);
					pDC->PatBlt(rectFrame.left, rectFrame.top, rectFrame.Width(), nTabBorderSize, PATCOPY);
					pDC->PatBlt(rectFrame.right - nTabBorderSize, rectFrame.top, nTabBorderSize, rectFrame.Height(), PATCOPY);
					pDC->PatBlt(rectFrame.left, rectFrame.bottom - nTabBorderSize, rectFrame.Width(), nTabBorderSize, PATCOPY);

					if (m_location == LOCATION_BOTTOM)
					{
						pDC->PatBlt(rectFrame.left, m_rectWndArea.bottom, rectFrame.Width(), rectFrame.bottom - m_rectWndArea.bottom, PATCOPY);
					}
					else
					{
						pDC->PatBlt(rectFrame.left, rectFrame.top, rectFrame.Width(), m_rectWndArea.top - rectFrame.top, PATCOPY);
					}

					if (m_bFlat)
					{
						//---------------------------
						// Draw line below the tabs:
						//---------------------------
						pDC->SelectObject(&penBlack);

						pDC->MoveTo(rectFrame.left + nTabBorderSize, yLine);
						pDC->LineTo(rectFrame.right - nTabBorderSize, yLine);
					}

					if (nTabBorderSize > 2)
					{
						rectFrame.DeflateRect(nTabBorderSize - 2, nTabBorderSize - 2);
					}

					if (rectFrame.Width() > 0 && rectFrame.Height() > 0)
					{
						pDC->Draw3dRect(&rectFrame, clrDarkShadow, clrHighlight);
					}
				}
				else
				{
					rectFrame.DeflateRect(2, 2);
				}
			}
		}
	}

	if (m_bTopEdge && m_location == LOCATION_TOP)
	{
		pDC->SelectObject(&penDark);

		pDC->MoveTo(rectClient.left, m_rectTabsArea.bottom);
		pDC->LineTo(rectClient.left, rectClient.top);
		pDC->LineTo(rectClient.right - 1, rectClient.top);
		pDC->LineTo(rectClient.right - 1, m_rectTabsArea.bottom);
	}

	CFont* pOldFont = pDC->SelectObject(&afxGlobalData.fontRegular);
	ENSURE(pOldFont != NULL);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(afxGlobalData.clrBtnText);

	if (m_rectTabsArea.Width() > 5 && m_rectTabsArea.Height() > 5)
	{
		//-----------
		// Draw tabs:
		//-----------
		CRect rectClip = m_rectTabsArea;
		rectClip.InflateRect(1, nTabBorderSize);

		CRgn rgn;
		rgn.CreateRectRgnIndirect(rectClip);

		for (int i = m_iTabsNum - 1; i >= 0; i--)
		{
			CMFCTabInfo* pTab = (CMFCTabInfo*)m_arTabs[i];
			ASSERT_VALID(pTab);

			m_iCurTab = i;

			if (i != m_iActiveTab) // Draw active tab last
			{
				pDC->SelectClipRgn(&rgn);

				if (m_bFlat)
				{
					pDC->SelectObject(&penBlack);
					DrawFlatTab(pDC, pTab, FALSE);
				}
				else
				{
					Draw3DTab(pDC, pTab, FALSE);
				}
			}
		}

		if (m_iActiveTab >= 0)
		{
			//-----------------
			// Draw active tab:
			//-----------------
			pDC->SetTextColor(afxGlobalData.clrWindowText);

			CMFCTabInfo* pTabActive = (CMFCTabInfo*)m_arTabs[m_iActiveTab];
			ASSERT_VALID(pTabActive);

			m_iCurTab = m_iActiveTab;

			pDC->SelectClipRgn(&rgn);

			{
				if (m_bIsActiveTabBold)
				{
					if (!IsMDITabGroup() || m_bIsActiveInMDITabGroup)
					{
						pDC->SelectObject(&afxGlobalData.fontBold);
					}
				}

				Draw3DTab(pDC, pTabActive, TRUE);
			}
		}

		pDC->SelectClipRgn(NULL);
	}

	// フィルタ文字列の描画
	pDC->TextOut(100, m_rectTabsArea.top+5, m_strFilterText);

	if (!m_rectTabSplitter.IsRectEmpty())
	{
		pDC->FillRect(m_rectTabSplitter, pbrFace);

		CRect rectTabSplitter = m_rectTabSplitter;

		pDC->Draw3dRect(rectTabSplitter, clrDarkShadow, clrDark);
		rectTabSplitter.DeflateRect(1, 1);
		pDC->Draw3dRect(rectTabSplitter, clrHighlight, clrDark);
	}

	if (m_bFlat && m_nTabsHorzOffset > 0)
	{
		pDC->SelectObject(&penDark);

		const int xDivider = m_rectTabsArea.left - 1;

		if (m_location == LOCATION_BOTTOM)
		{
			pDC->MoveTo(xDivider, m_rectTabsArea.top + 1);
			pDC->LineTo(xDivider, m_rectTabsArea.bottom - 2);
		}
		else
		{
			pDC->MoveTo(xDivider, m_rectTabsArea.bottom);
			pDC->LineTo(xDivider, m_rectTabsArea.top + 2);
		}
	}

	if (!m_rectResize.IsRectEmpty())
	{
		pDC->FillRect(m_rectResize, pbrFace);

		pDC->SelectObject(&penDark);

		if (m_ResizeMode == RESIZE_VERT)
		{
			pDC->MoveTo(m_rectResize.left, m_rectResize.top);
			pDC->LineTo(m_rectResize.left, m_rectResize.bottom);
		}
		else
		{
			pDC->MoveTo(m_rectResize.left, m_rectResize.top);
			pDC->LineTo(m_rectResize.right, m_rectResize.top);
		}
	}

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	if (memDC.IsMemDC())
	{
		dc.ExcludeClipRect(m_rectWndArea);
	}
}

//void CMFCTabCtrlEx::OnNcPaint()
//{
//}

void CMFCTabCtrlEx::Draw3DTab(CDC* pDC, CMFCTabInfo* pTab, BOOL bActive)
{
	ASSERT_VALID(pTab);
	ASSERT_VALID(pDC);

	ASSERT_VALID(pTab);
	ASSERT_VALID(pDC);

	if ((m_bIsOneNoteStyle || m_bIsVS2005Style || m_bLeftRightRounded) && pTab->GetRect().left < m_rectTabsArea.left)
	{
		return;
	}

	// 強制的にタブ幅を変更する
	CRect r = pTab->GetRect();
	if (r.Width() > 56){
		r.right = r.left + 56;
		//pTab->SetRect(r);
	}

	//if (pTab->m_bVisible)
	{
		CRect rectInter;
		if (m_rectCurrClip.IsRectEmpty() || rectInter.IntersectRect(pTab->GetRect(), m_rectCurrClip))
		{
			CMFCVisualManager::GetInstance()->OnDrawTab(pDC, pTab->GetRect(), m_iCurTab, bActive, this);


			//// 先頭タブ：Targetは選択されている項目文字列を表示する
			//if(m_iCurTab == 0){
			//	//UINT nFormat = DT_SINGLELINE | DT_CENTER | DT_VCENTER;
			//	CFont* pOldFont = pDC->SelectObject(&afxGlobalData.fontBold);
			//	ASSERT(pOldFont != NULL);
			//	UINT nFormat = DT_SINGLELINE | DT_LEFT;
			//	pDC->SetTextColor(RGB(0,0,0));
			//	UINT act = m_wnd1.GetActiveButton() - IDC_BUTTON_COMMAND01;
			//	CString& str = m_wnd1.GetButtonText(act);
			//	CString strw, stre;	
			//	AfxExtractSubString(strw, str, 0, ',');
			//	AfxExtractSubString(stre, str, 1, ',');
			//	nFormat = DT_SINGLELINE | DT_LEFT|DT_TOP;
			//	pDC->DrawText(_T(" ")+strw, pTab->GetRect(), nFormat);
			//	nFormat = DT_SINGLELINE | DT_RIGHT|DT_BOTTOM;
			//	pDC->DrawText(stre+_T(" "), pTab->GetRect(), nFormat);
			//	pDC->SelectObject(pOldFont);
			//}
		}
	}
}

void CMFCTabCtrlEx::OnSize(UINT nType, int cx, int cy)
{
	CMFCTabCtrl::OnSize(nType, cx, cy);
}


//=============================================================================
// CGroupBoxEx
//=============================================================================

IMPLEMENT_DYNAMIC(CGroupBoxEx, CButton)

CGroupBoxEx::CGroupBoxEx()
{
	//m_colorText = RGB(0, 0, 0);
	//m_colorBack = RGB(191, 219, 255);
	m_colorText = RGB(255, 255, 255);
	m_colorBack = RGB(50, 50, 255);
	m_typeGroup = eGroupType_Ribbon;
	m_pointRound = CPoint(20, 20);
	m_bFontBold = false;
}

CGroupBoxEx::~CGroupBoxEx()
{
}


BEGIN_MESSAGE_MAP(CGroupBoxEx, CButton)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CGroupBoxEx メッセージ ハンドラ

void CGroupBoxEx::PreSubclassWindow()
{
	GetWindowText(m_strText);
	GetWindowRect(m_rect);
	ScreenToClient(m_rect);

	CButton::PreSubclassWindow();
}

void CGroupBoxEx::UpdateRect()
{
	CRect rect;
	GetClientRect(rect);
	m_rect.right = rect.right;
	m_rect.bottom = rect.bottom;
	// 描画
	Invalidate();
}

void CGroupBoxEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	// 描画メッセージで CButton::OnPaint() を呼び出さないでください。
	
	int saveDC = dc.SaveDC();

	GetWindowText(m_strText);

	CString str;
	str = m_strText;

	CFont* oldFont = NULL;
	CFont defFont, txtFont;
	if (m_typeGroup == eGroupType_Round){
		CFont* pFont = GetParent()->GetFont();
		oldFont = dc.SelectObject(pFont);
	}
	else if (m_typeGroup != eGroupType_None){
		// テキストの描画
		// 文字フォントの作成
		defFont.CreateStockObject(DEFAULT_GUI_FONT);
		LOGFONT lf;
		if (defFont.GetLogFont(&lf)){
			lf.lfWeight = FW_NORMAL;
			if (m_typeGroup != eGroupType_Normal && m_typeGroup != eGroupType_Ribbon){
				lf.lfHeight = 14;
			}
			if (m_bFontBold == true)
				lf.lfWeight = FW_BOLD;

			txtFont.DeleteObject();
			txtFont.CreateFontIndirect(&lf);
			oldFont = dc.SelectObject(&txtFont);
		}
	}

	CSize size = dc.GetTextExtent(str, str.GetLength());

	// テキストなしの矩形をベースに設定する
	CPoint ptStart, ptEnd;
	ptStart.x = m_rect.left;
	ptStart.y = m_rect.top + (size.cy / 2);
	ptEnd.x = ptStart.x + size.cx;
	ptEnd.y = m_rect.top + (size.cy / 2);
	if (m_typeGroup == eGroupType_Normal || m_typeGroup == eGroupType_Ribbon){
		ptStart.x = m_rect.left + 8;
		ptEnd.x = ptStart.x + size.cx;
	}

	CPen penShadow(PS_SOLID | PS_GEOMETRIC | PS_ENDCAP_SQUARE, 1, ::GetSysColor(COLOR_3DSHADOW));
	CPen penHlight(PS_SOLID | PS_GEOMETRIC | PS_ENDCAP_SQUARE, 1, ::GetSysColor(COLOR_3DHIGHLIGHT));

	// 矩形の描画
	CPen *oldPen;
	if (m_typeGroup == eGroupType_None || m_typeGroup == eGroupType_Normal){
		ptEnd.x = (m_rect.right > ptEnd.x) ? ptEnd.x : m_rect.right;
		dc.DrawEdge(CRect(m_rect.left, ptStart.y, m_rect.right, m_rect.bottom), EDGE_ETCHED, BF_RECT);
		dc.FillSolidRect(CRect(ptStart.x, m_rect.top, ptEnd.x, m_rect.top + size.cy), m_colorBack);
	}

	if (m_typeGroup != eGroupType_None){
		// テキストの描画
		// テキスト背景の描画
		CPen	pen(PS_SOLID, 1, m_colorBack);
		CBrush	brush(m_colorBack);
		//dc.SetBkMode(TRANSPARENT);
		oldPen = dc.SelectObject(&pen);
		CBrush *oldBrush = dc.SelectObject(&brush);

		if (m_typeGroup == eGroupType_Round){
			CRect rc = m_rect;
			dc.FillSolidRect(CRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom), ::GetSysColor(COLOR_3DFACE));
			dc.DrawEdge(CRect(m_rect.left, ptStart.y, m_rect.right, m_rect.bottom), EDGE_ETCHED, BF_RECT);
			rc.DeflateRect(4, 13);
			rc.top += 3;
			dc.FillSolidRect(rc, m_colorBack);
			//dc.FillSolidRect(m_rect, ::GetSysColor(COLOR_3DFACE));
			//dc.RoundRect(CRect(m_rect.left, m_rect.top, m_rect.right + 1, m_rect.bottom + 1), m_pointRound);
		}
		else if (m_typeGroup == eGroupType_Box){
			dc.SetBkMode(TRANSPARENT);
			dc.FillSolidRect(CRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom), ::GetSysColor(COLOR_3DFACE));
			dc.FillSolidRect(CRect(m_rect.left, m_rect.top, m_rect.right + 1, m_rect.bottom + 1), m_colorBack);
		}
		else if (m_typeGroup == eGroupType_BoxLine){
			dc.SetBkMode(TRANSPARENT);
			dc.FillSolidRect(CRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom), ::GetSysColor(COLOR_3DFACE));
			dc.FillSolidRect(CRect(m_rect.left, m_rect.top, m_rect.right + 1, m_rect.bottom + 1), m_colorBack);
			dc.DrawEdge(CRect(m_rect.left, ptStart.y, m_rect.right, m_rect.bottom), EDGE_ETCHED, BF_RECT);
		}
		else if (m_typeGroup == eGroupType_Ribbon){
			dc.SetBkMode(TRANSPARENT);
			dc.FillSolidRect(CRect(m_rect.left, m_rect.bottom - 22, m_rect.right - 2, m_rect.bottom - 2), m_colorBack);
			dc.DrawEdge(CRect(m_rect.left, ptStart.y, m_rect.right, m_rect.bottom), EDGE_ETCHED, BF_RECT);
		}

		// テキストの描画
		if (m_typeGroup != eGroupType_Normal && m_typeGroup != eGroupType_Ribbon){
			dc.SetBkColor(::GetSysColor(COLOR_3DFACE));
			ptStart.x = m_rect.left + 10;
			ptEnd.x = m_rect.right;
			dc.DrawText(str, CRect(ptStart, ptEnd), DT_VCENTER | DT_LEFT | DT_SINGLELINE | DT_NOCLIP | DT_END_ELLIPSIS);
			//dc.SetTextColor(m_colorText);
			//ptStart.x = m_rect.left;
			//ptEnd.x = m_rect.right;
			//dc.DrawText(str, CRect(ptStart, ptEnd), DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_NOCLIP);
		}
		else if (m_typeGroup == eGroupType_Ribbon){
			dc.SetTextColor(m_colorText);
			ptStart.x = m_rect.left;
			ptEnd.x = m_rect.right;
			ptStart.y = m_rect.bottom - 22;
			ptEnd.y = m_rect.bottom - 2;
			dc.DrawText(str, CRect(ptStart, ptEnd), DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_NOCLIP);
		}
		else{
			ptEnd.x = m_rect.right;
			dc.DrawText(str, CRect(ptStart, ptEnd), DT_VCENTER | DT_LEFT | DT_SINGLELINE | DT_NOCLIP | DT_END_ELLIPSIS);
		}
		dc.SelectObject(oldPen);
		dc.SelectObject(oldBrush);
		pen.DeleteObject();
		brush.DeleteObject();

		if (oldFont != NULL){
			dc.SelectObject(oldFont);
			txtFont.DeleteObject();
		}
	}
	penShadow.DeleteObject();
	penHlight.DeleteObject();

	dc.RestoreDC(saveDC);
}


IMPLEMENT_DYNAMIC(CBarChild, CDialog)

CBarChild::CBarChild(UINT IDD, CWnd* pParent /*=NULL*/)
: CDialog(IDD, pParent)
{

}

CBarChild::~CBarChild()
{
}
