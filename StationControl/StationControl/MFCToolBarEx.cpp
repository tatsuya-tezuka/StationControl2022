// MFCToolBarEx.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "MFCToolBarEx.h"

// CMFCToolBarEx

IMPLEMENT_DYNAMIC(CMFCToolBarEx, CMFCToolBar)

CMFCToolBarEx::CMFCToolBarEx()
{

}

CMFCToolBarEx::~CMFCToolBarEx()
{
}


BEGIN_MESSAGE_MAP(CMFCToolBarEx, CMFCToolBar)
	//ON_WM_ERASEBKGND()
	//ON_WM_PAINT()
END_MESSAGE_MAP()



// CMFCToolBarEx メッセージ ハンドラー




BOOL CMFCToolBarEx::OnEraseBkgnd(CDC* pDC)
{
	CRect rectClient;
	GetClientRect(rectClient);

	// メニューバー全体を塗り潰し
	pDC->FillSolidRect(&rectClient, ::GetSysColor(COLOR_GRADIENTACTIVECAPTION));

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

	pDC->SetTextColor(::GetSysColor(COLOR_MENUTEXT));
	pDC->SetBkColor(::GetSysColor(COLOR_MENUBAR));
	CString str;
	GetWindowText(str);
	pDC->DrawText(_T("AAAAAAAAAAAAAAAAAAA"), rectClient, nFormat);

	return TRUE;

	//return CMFCToolBar::OnEraseBkgnd(pDC);
}


void CMFCToolBarEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	// 描画メッセージで CMFCToolBar::OnPaint() を呼び出さないでください。
}

void CMFCToolBarEx::DoPaint(CDC* pDCPaint)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDCPaint);

	CRect rectClip;
	pDCPaint->GetClipBox(rectClip);

	BOOL bHorz = GetCurrentAlignment() & CBRS_ORIENT_HORZ ? TRUE : FALSE;

	CRect rectClient;
	GetClientRect(rectClient);

	CMemDC memDC(*pDCPaint, this);
	CDC* pDC = &memDC.GetDC();

	if ((GetStyle() & TBSTYLE_TRANSPARENT) == 0)
	{
		CMFCVisualManager::GetInstance()->OnFillBarBackground(pDC, this, rectClient, rectClip);
	}
	else
	{
		//m_Impl.GetBackgroundFromParent(pDC);
	}

	OnFillBackground(pDC);

	pDC->SetTextColor(GetGlobalData()->clrBtnText);
	pDC->SetBkMode(TRANSPARENT);

	CRect rect;
	GetClientRect(rect);

	// Force the full size of the button:
	if (bHorz)
	{
		rect.bottom = rect.top + GetRowHeight();
	}
	else
	{
		rect.right = rect.left + GetColumnWidth();
	}

	BOOL bDontScaleImages = m_bLocked ? m_bDontScaleLocked : m_bDontScaleImages;
	const double dblImageScale = bDontScaleImages ? 1.0 : GetGlobalData()->GetRibbonImageScale();

	CMFCToolBarImages* pImages = GetImageList(m_Images, m_ImagesLocked, m_LargeImages, m_LargeImagesLocked);
	CMFCToolBarImages* pHotImages = pImages;
	CMFCToolBarImages* pColdImages = GetImageList(m_ColdImages, m_ColdImagesLocked, m_LargeColdImages, m_LargeColdImagesLocked);
	CMFCToolBarImages* pDisabledImages = GetImageList(m_DisabledImages, m_DisabledImagesLocked, m_LargeDisabledImages, m_LargeDisabledImagesLocked);
	CMFCToolBarImages* pMenuImages = !m_bLocked ? &m_MenuImages : &m_MenuImagesLocked;
	CMFCToolBarImages* pDisabledMenuImages = !m_bLocked ? &m_DisabledMenuImages : &m_DisabledMenuImagesLocked;

	BOOL bDrawImages = pImages->IsValid();

	pHotImages->SetTransparentColor(GetGlobalData()->clrBtnFace);

	BOOL bFadeInactiveImages = CMFCVisualManager::GetInstance()->IsFadeInactiveImage();

	CSize sizeImageDest = m_bMenuMode ? m_sizeMenuImage : GetImageSize();
	if (dblImageScale != 1.)
	{
		if (m_bMenuMode && sizeImageDest == CSize(-1, -1))
		{
			sizeImageDest = GetImageSize();

			if (dblImageScale > 1. && m_bLargeIconsAreEnbaled)
			{
				sizeImageDest = m_sizeImage;
			}
		}

		sizeImageDest = CSize((int)(.5 + sizeImageDest.cx * dblImageScale), (int)(.5 + sizeImageDest.cy * dblImageScale));
	}

	CAfxDrawState ds;
	if (bDrawImages)
	{
		if (dblImageScale != 1.0 && pHotImages->GetScale() == 1.0)
		{
			pHotImages->SmoothResize(dblImageScale);
		}

		if (!pHotImages->PrepareDrawImage(ds, sizeImageDest, bFadeInactiveImages))
		{
			return;     // something went wrong
		}
	}

	CFont* pOldFont;
	if (bHorz)
	{
		pOldFont = SelectDefaultFont(pDC);
	}
	else
	{
		pOldFont = (CFont*)pDC->SelectObject(&(GetGlobalData()->fontVert));
	}

	if (pColdImages->GetCount() > 0)
	{
		// Disable fade effect for inactive buttons:
		CMFCVisualManager::GetInstance()->SetFadeInactiveImage(FALSE);
	}

	// Draw buttons:
	int iButton = 0;
	for (POSITION pos = m_Buttons.GetHeadPosition(); pos != NULL; iButton++)
	{
		CMFCToolBarButton* pButton = (CMFCToolBarButton*)m_Buttons.GetNext(pos);
		if (pButton == NULL)
		{
			break;
		}

		ASSERT_VALID(pButton);

		rect = pButton->Rect();
		CRect rectInter;

		if (pButton->m_nStyle & TBBS_SEPARATOR)
		{
			BOOL bHorzSeparator = bHorz;
			CRect rectSeparator; rectSeparator.SetRectEmpty();

			OnCalcSeparatorRect(pButton, rectSeparator, bHorz);

			if (pButton->m_bWrap && bHorz)
			{
				bHorzSeparator = FALSE;
			}

			if (rectInter.IntersectRect(rectSeparator, rectClip) && !pButton->IsHidden())
			{
				DrawSeparator(pDC, rectSeparator, bHorzSeparator);
			}

			continue;
		}

		if (!rectInter.IntersectRect(rect, rectClip))
		{
			continue;
		}

		BOOL bHighlighted = IsButtonHighlighted(iButton);
		BOOL bDisabled = (pButton->m_nStyle & TBBS_DISABLED) && !IsCustomizeMode();

		if (pDC->RectVisible(&rect))
		{
			BOOL bDrawDisabledImages = FALSE;

			if (bDrawImages)
			{
				CMFCToolBarImages* pNewImages = NULL;

				if (pButton->m_bUserButton)
				{
					if (pButton->GetImage() >= 0)
					{
						pNewImages = m_pUserImages;
					}
				}
				else
				{
					if (m_bMenuMode)
					{
						if (bDisabled && pDisabledMenuImages->GetCount() > 0)
						{
							bDrawDisabledImages = TRUE;
							pNewImages = pDisabledMenuImages;
						}
						else if (pMenuImages->GetCount() > 0)
						{
							pNewImages = pMenuImages;
						}
						else
						{
							bDrawDisabledImages = (bDisabled && pDisabledImages->GetCount() > 0);
							pNewImages = bDrawDisabledImages ? pDisabledImages : pHotImages;
						}
					}
					else // Toolbar mode
					{
						bDrawDisabledImages = (bDisabled && pDisabledImages->GetCount() > 0);
						pNewImages = bDrawDisabledImages ? pDisabledImages : pHotImages;

						if (!bHighlighted && !bDrawDisabledImages && (pButton->m_nStyle & TBBS_PRESSED) == 0 && pColdImages->GetCount() > 0 && !pButton->IsDroppedDown())
						{
							pNewImages = pColdImages;
						}
					}
				}

				if (bDrawImages && pNewImages != pImages && pNewImages != NULL)
				{
					pImages->EndDrawImage(ds);

					pNewImages->SetTransparentColor(GetGlobalData()->clrBtnFace);
					if (dblImageScale != 1.0 && pNewImages->GetScale() == 1.0)
					{
						pNewImages->SmoothResize(dblImageScale);
					}

					pNewImages->PrepareDrawImage(ds, sizeImageDest, bFadeInactiveImages);

					pImages = pNewImages;
				}
			}

			DrawButton(pDC, pButton, bDrawImages ? pImages : NULL, bHighlighted, bDrawDisabledImages);
		}
	}

	// Highlight selected button in the toolbar customization mode:
	if (m_iSelected >= m_Buttons.GetCount())
	{
		m_iSelected = -1;
	}

	if ((IsCustomizeMode() || m_bAltCustomizeMode) && m_iSelected >= 0 && !m_bLocked && m_pSelToolbar == this)
	{
		CMFCToolBarButton* pSelButton = GetButton(m_iSelected);
		ENSURE(pSelButton != NULL);

		if (pSelButton != NULL && pSelButton->CanBeStored())
		{
			CRect rectDrag1 = pSelButton->Rect();

			pDC->Draw3dRect(&rectDrag1, GetGlobalData()->clrBtnText, GetGlobalData()->clrBtnText);
			rectDrag1.DeflateRect(1, 1);
			pDC->Draw3dRect(&rectDrag1, GetGlobalData()->clrBtnText, GetGlobalData()->clrBtnText);
		}
	}

	if (IsCustomizeMode() && m_iDragIndex >= 0 && !m_bLocked)
	{
		DrawDragCursor(pDC);
	}

	pDC->SelectObject(pOldFont);

	if (bDrawImages)
	{
		pImages->EndDrawImage(ds);
	}

	// ツールバーの右側に時刻を表示
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
	pDC->SetBkColor(::GetSysColor(COLOR_MENUBAR));

	CRect trect, drect;
	pDC->SelectObject(&timeFont);
	str = time.Format(_T("99:99:99 WWW "));
	pDC->DrawText(str, &trect, DT_CALCRECT);
	pDC->SelectObject(&dayFont);
	str = time.Format(_T("9999/99/99(999) "));
	pDC->DrawText(str, &drect, DT_CALCRECT);

	// UTC
	pDC->SelectObject(&timeFont);
	rect = rectClient;
	rect.left = rect.right - __max(trect.Width(), drect.Width());
	rect.bottom = rect.top + trect.Height();
	str = time.FormatGmt(_T("%H:%M:%S UTC "));
	pDC->DrawText(str, rect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	pDC->SelectObject(&dayFont);
	rect = rectClient;
	rect.left = rect.right - __max(trect.Width(), drect.Width());
	rect.top = rect.top + trect.Height();
	rect.bottom = rect.top + drect.Height();
	str = time.FormatGmt(_T("%Y-%m-%d(%j) "));
	pDC->DrawText(str, rect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	// JST
	pDC->SelectObject(&timeFont);
	rect = rectClient;
	rect.right = rect.right - __max(trect.Width(), drect.Width());
	rect.left = rect.right - __max(trect.Width(), drect.Width());
	rect.bottom = rect.top + trect.Height();
	str = time.Format(_T("%H:%M:%S JST "));
	pDC->DrawText(str, rect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	pDC->SelectObject(&dayFont);
	rect = rectClient;
	rect.right = rect.right - __max(trect.Width(), drect.Width());
	rect.left = rect.right - __max(trect.Width(), drect.Width());
	rect.top = rect.top + trect.Height();
	rect.bottom = rect.top + drect.Height();
	str = time.Format(_T("%Y-%m-%d(%j) "));
	pDC->DrawText(str, rect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	if (oldFont != NULL)
		pDC->SelectObject(oldFont);

	CMFCVisualManager::GetInstance()->SetFadeInactiveImage(bFadeInactiveImages);
}
