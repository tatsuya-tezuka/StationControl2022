/*============================================================================*/
/*! ListCtrlForecast.cpp

-# 予報値一覧、運用計画一覧用のリストコントロール
*/
/*============================================================================*/
// ListCtrlForecast.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ListCtrlForecast.h"
#include "ForecastValue.h"

// CListCtrlForecast

IMPLEMENT_DYNAMIC(CListCtrlForecast, CListCtrl)

/*============================================================================*/
/*! CListCtrlForecast

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CListCtrlForecast::CListCtrlForecast()
{

}

/*============================================================================*/
/*! CListCtrlForecast

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CListCtrlForecast::~CListCtrlForecast()
{
}


BEGIN_MESSAGE_MAP(CListCtrlForecast, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListCtrlForecast::OnNMCustomdraw)
END_MESSAGE_MAP()



// CListCtrlForecast メッセージ ハンドラー

/*============================================================================*/
/*! CForecastValue

-# カスタムドロー時に呼ばれる

@param  strItem：アイテム名
@retval 色
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

-# カスタムドロー時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし
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

	// グループの境界線更新
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
