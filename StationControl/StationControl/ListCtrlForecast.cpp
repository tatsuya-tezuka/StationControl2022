/*============================================================================*/
/*! ListCtrlForecast.cpp

-# �\��l�ꗗ�A�^�p�v��ꗗ�p�̃��X�g�R���g���[��
*/
/*============================================================================*/
// ListCtrlForecast.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "ListCtrlForecast.h"
#include "ForecastValue.h"

// CListCtrlForecast

IMPLEMENT_DYNAMIC(CListCtrlForecast, CListCtrl)

/*============================================================================*/
/*! CListCtrlForecast

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CListCtrlForecast::CListCtrlForecast()
{

}

/*============================================================================*/
/*! CListCtrlForecast

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CListCtrlForecast::~CListCtrlForecast()
{
}


BEGIN_MESSAGE_MAP(CListCtrlForecast, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListCtrlForecast::OnNMCustomdraw)
END_MESSAGE_MAP()



// CListCtrlForecast ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! CForecastValue

-# �J�X�^���h���[���ɌĂ΂��

@param  strItem�F�A�C�e����
@retval �F
*/
/*============================================================================*/
COLORREF CListCtrlForecast::GetCelColorAt(CString strItem)
{
	map<CString, COLORREF>::iterator itr;
	for (itr = m_mapColor.begin(); itr != m_mapColor.end(); ++itr)
	{
		if ((*itr).first.Compare(strItem) == 0)
		{
			return (*itr).second;
		}
	}

	return NULL;
}

/*============================================================================*/
/*! CForecastValue

-# �J�X�^���h���[���ɌĂ΂��

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�
*/
/*============================================================================*/
void CListCtrlForecast::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

	switch (lplvcd->nmcd.dwDrawStage) {
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		{
			int		nItem = (int)lplvcd->nmcd.dwItemSpec;
			int		nSubItem = lplvcd->iSubItem;

			if (nSubItem == 0)
			{
				CString	str = GetItemText(nItem, nSubItem);
				lplvcd->clrTextBk = GetCelColorAt(str);
			}
			else
			{
				lplvcd->clrTextBk = GetBkColor();
			}
			*pResult = CDRF_DODEFAULT;
		}
		break;
	default:
		*pResult = CDRF_DODEFAULT;
		break;
	}

	// �O���[�v�̋��E���X�V
	CForecastValue* pWnd = (CForecastValue*)GetParent();
	if (pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
	{
		CForecastValueGroupView* pView = pWnd->GetGroupView();
		if (pView != NULL)
		{
			pView->Invalidate();
		}
	}
}
