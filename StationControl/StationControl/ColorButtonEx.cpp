/*============================================================================*/
/*! ColorButtonEx.cpp

-# �J�X�^���{�^���i�{�^���̔w�i�F�ύX�j
-# �w�肳�ꂽ�F�ɏ]���ĕ\������B
*/
/*============================================================================*/
// ColorButtonEx.cpp : �����t�@�C��
//


#include "stdafx.h"
#include "StationControl.h"
#include "ColorButtonEx.h"


// CColorButtonEx

IMPLEMENT_DYNAMIC(CColorButtonEx, CButton)

/*============================================================================*/
/*! CColorButtonEx

-# �R���X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
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

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
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



// CColorButtonEx ���b�Z�[�W �n���h���[



/*============================================================================*/
/*! CColorButtonEx

-# �J�X�^���h���[

@param  pNMHDR	�FNMHDR�\���̃|�C���^�[�B
@param  pResult	�F���b�Z�[�W���������ꂽ�Ƃ����ʂ��i�[����R�[�h���� LRESULT �̕ϐ��ւ̃|�C���^�[�B
@retval �Ȃ�
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
				// �񊈐�
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
					// ����
					color2 = m_ColorNormal2;
				}
			}
#else
			int nCheck = GetCheck();
			if (nCheck == 1)
			{
				if (IsWindowEnabled() == FALSE)
				{
					// �񊈐�
					color = COLOR_D_REMOTES;
				}
				else
				{
					if (m_ColorNormal1 == COLOR_CTRLIN)
					{
						// ���䒆
						color = COLOR_CTRLIN;
					}
					else
					{
						// �I��
						color = m_ColorCheck;
					}
				}
			}
			else
			{
				if (IsWindowEnabled() == FALSE)
				{
					// �񊈐�
					color = COLOR_D_REMOTE;
				}
				else
				{
					// �ʏ�
					color = m_ColorNormal1;
				}
				if (m_ColorNormal2 != NULL)
				{
					// ����
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
				pDC->Rectangle(rcItemHalfL);	// ������

				CBrush brushR(color2);
				pDC->SelectObject(&brushR);
				pDC->Rectangle(rcItemHalfR);	// �E����
			}
			else
			{
				CBrush brush(color);
				oldbrush = (CBrush *)pDC->SelectObject(&brush);

				pDC->SetBkMode(TRANSPARENT);
//				pDC->RoundRect(rcItem, CPoint(5, 5));
				pDC->Rectangle(rcItem);
			}

			// Text�\��
			if (m_ColorNormal1 == COLOR_CTRLIN)
			{
				// ���䒆
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

-# Ctrl�Ashift �L�[�����`�F�b�N

@param  nFlags	�F�L�[���
@retval TRUE	�F�N���b�N�AFALSE�F�m�[�N���b�N
*/
/*============================================================================*/
BOOL CColorButtonEx::CheckKeyClick(UINT nFlags)
{
	BOOL ret = FALSE;

	// Ctrl�L�[��������Ă��邩
	BOOL bCtrlStatus = FALSE;
	if (m_bCtrlButton == TRUE)	// Ctrl �L�[�����������K�{�̏ꍇ
	{
		if (nFlags & MK_CONTROL)
		{
			// Ctrl �L�[��������Ă���
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

	// Shift�L�[��������Ă��邩
	BOOL bShiftStatus = FALSE;
	if (m_bShiftButton == TRUE)	// Shift �L�[�����������K�{�̏ꍇ
	{
		if (nFlags & MK_SHIFT)
		{
			// Shift �L�[��������Ă���
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
