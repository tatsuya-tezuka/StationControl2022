/*============================================================================*/
/*! ColorButtonEx.cpp

-# カスタムボタン（ボタンの背景色変更）
-# 指定された色に従って表示する。
*/
/*============================================================================*/
// ColorButtonEx.cpp : 実装ファイル
//


#include "stdafx.h"
#include "StationControl.h"
#include "ColorButtonEx.h"


// CColorButtonEx

IMPLEMENT_DYNAMIC(CColorButtonEx, CButton)

/*============================================================================*/
/*! CColorButtonEx

-# コンストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CColorButtonEx::CColorButtonEx()
{
	m_ColorNormal1	= RGB(0, 0, 0);
	m_ColorNormal2	= RGB(0, 0, 0);
	m_bHalf			= FALSE;
	m_bCtrlButton	= FALSE;
	m_bShiftButton	= FALSE;
}

/*============================================================================*/
/*! CColorButtonEx

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CColorButtonEx::~CColorButtonEx()
{
}


BEGIN_MESSAGE_MAP(CColorButtonEx, CButton)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CColorButtonEx::OnNMCustomdraw)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CColorButtonEx メッセージ ハンドラー



/*============================================================================*/
/*! CColorButtonEx

-# カスタムドロー

@param  pNMHDR	：NMHDR構造体ポインター。
@param  pResult	：メッセージが処理されたとき結果を格納するコードする LRESULT の変数へのポインター。
@retval なし
*/
/*============================================================================*/

void CColorButtonEx::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	long nState = pNMCD->uItemState;
	CDC *pDC = CDC::FromHandle(pNMCD->hdc);
	CRect rcItem = pNMCD->rc;
	CRect rcItemHalfL(rcItem.left, rcItem.top, (rcItem.Width() / 2), rcItem.bottom);
	CRect rcItemHalfR((rcItem.Width() / 2), rcItem.top, rcItem.right, rcItem.bottom);

//	UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	UINT nFormat = DT_CENTER | DT_WORDBREAK;

	switch (pNMCD->dwDrawStage)
	{
	case CDDS_PREERASE:
		*pResult = CDRF_NOTIFYPOSTERASE;
		break;

	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYPOSTPAINT;
		break;

//	case CDDS_PREERASE | CDDS_PREPAINT:
//		*pResult = CDRF_SKIPDEFAULT;
//		break;

	case CDDS_POSTPAINT:
		{
			COLORREF color, color2;
			color = RGB(0, 0, 0);
			color2 = RGB(0, 0, 0);

			BOOL bHalf = FALSE;

			CString str;
			GetWindowText(str);
#if 1
			if (IsWindowEnabled() == FALSE)
			{
				// 非活性
				if (m_ColorNormal1 == COLOR_REMOTES)
				{
					color = COLOR_D_REMOTES;
				}
				else if (m_ColorNormal1 == COLOR_REMOTE)
				{
					color = COLOR_D_REMOTE;
				}
				else
				{
					color = RGB(0, 0, 0);
				}
			}
			else
			{
				color = m_ColorNormal1;

				if (m_bHalf == TRUE)
				{
					// 半灯
					color2 = m_ColorNormal2;
				}
			}
#else
			int nCheck = GetCheck();
			if (nCheck == 1)
			{
				if (IsWindowEnabled() == FALSE)
				{
					// 非活性
					color = COLOR_D_REMOTES;
				}
				else
				{
					if (m_ColorNormal1 == COLOR_CTRLIN)
					{
						// 制御中
						color = COLOR_CTRLIN;
					}
					else
					{
						// 選択
						color = m_ColorCheck;
					}
				}
			}
			else
			{
				if (IsWindowEnabled() == FALSE)
				{
					// 非活性
					color = COLOR_D_REMOTE;
				}
				else
				{
					// 通常
					color = m_ColorNormal1;
				}
				if (m_ColorNormal2 != NULL)
				{
					// 半灯
					color2 = m_ColorNormal2;
					bHalf = TRUE;
				}
			}
#endif

			CBrush *oldbrush;
			if (m_bHalf)
			{
				CBrush brushL(color);
				oldbrush = (CBrush *)pDC->SelectObject(&brushL);

				pDC->SetBkMode(TRANSPARENT);
				pDC->Rectangle(rcItemHalfL);	// 左半分

				CBrush brushR(color2);
				pDC->SelectObject(&brushR);
				pDC->Rectangle(rcItemHalfR);	// 右半分
			}
			else
			{
				CBrush brush(color);
				oldbrush = (CBrush *)pDC->SelectObject(&brush);

				pDC->SetBkMode(TRANSPARENT);
//				pDC->RoundRect(rcItem, CPoint(5, 5));
				pDC->Rectangle(rcItem);
			}

			// Text表示
			if (m_ColorNormal1 == COLOR_CTRLIN)
			{
				// 制御中
				pDC->SetTextColor(RGB(0, 0, 0));
			}
			else
			{
				pDC->SetTextColor(color ^ 0xFFFFFF);
			}

		
			CRect rect;
			CRect backupRect(rcItem);
			pDC->DrawText(str, &rect, DT_CALCRECT);
			backupRect.top = (backupRect.bottom - rect.bottom) / 2;
			pDC->DrawText(str, &backupRect, nFormat);

			if (nState & ODS_FOCUS)
			{
				if (nState & ODS_SELECTED)
				{
					pDC->DrawEdge(rcItem, EDGE_SUNKEN, BF_RECT);
				}
				CRect focusRect(rcItem);
				focusRect.left += 2;
				focusRect.right -= 2;
				focusRect.top += 2;
				focusRect.bottom -= 2;
				pDC->DrawFocusRect(focusRect);
			}
			else if (nState & ODS_CHECKED)
			{
				pDC->DrawEdge(rcItem, EDGE_BUMP, BF_RECT);
			}

			pDC->SelectObject(oldbrush);
			*pResult = CDRF_DODEFAULT;
		}
		break;

	default:
		break;
	}

//	*pResult = 0;
}

void CColorButtonEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ((m_ColorNormal1 != COLOR_LOCAL) && CheckKeyClick(nFlags))
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


void CColorButtonEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if ((m_ColorNormal1 != COLOR_LOCAL) && CheckKeyClick(nFlags))
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

/*============================================================================*/
/*! CColorButtonEx

-# Ctrl、shift キー押下チェック

@param  nFlags	：キー種別
@retval TRUE	：クリック、FALSE：ノークリック
*/
/*============================================================================*/
BOOL CColorButtonEx::CheckKeyClick(UINT nFlags)
{
	BOOL ret = FALSE;

	// Ctrlキーが押されているか
	BOOL bCtrlStatus = FALSE;
	if (m_bCtrlButton == TRUE)	// Ctrl キーが同時押下必須の場合
	{
		if (nFlags & MK_CONTROL)
		{
			// Ctrl キーが押されている
			bCtrlStatus = TRUE;
		}
		else
		{
			bCtrlStatus = FALSE;
		}
	}
	else
	{
		bCtrlStatus = TRUE;
	}

	// Shiftキーが押されているか
	BOOL bShiftStatus = FALSE;
	if (m_bShiftButton == TRUE)	// Shift キーが同時押下必須の場合
	{
		if (nFlags & MK_SHIFT)
		{
			// Shift キーが押されている
			bShiftStatus = TRUE;
		}
		else
		{
			bShiftStatus = FALSE;
		}
	}
	else
	{
		bShiftStatus = TRUE;
	}

	if (bCtrlStatus == TRUE && bShiftStatus == TRUE)
	{
		ret = TRUE;
	}
	else
	{
		ret = FALSE;
	}

	return ret;
}


void CColorButtonEx::SetColor(COLORREF nomalCol1, COLORREF nomalCol2, BOOL bHalf)
{
	m_ColorNormal1 = nomalCol1; 
	m_ColorNormal2 = nomalCol2; 
	m_bHalf = bHalf;
}
