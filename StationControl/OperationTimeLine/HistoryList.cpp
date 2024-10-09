// HistoryList.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "HistoryList.h"


// CHistoryList

IMPLEMENT_DYNAMIC(CHistoryList, CListCtrl)

CHistoryList::CHistoryList()
{
	mParentWnd = NULL;
}

CHistoryList::~CHistoryList()
{
}


BEGIN_MESSAGE_MAP(CHistoryList, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CHistoryList::OnNMCustomdraw)
	ON_WM_LBUTTONDOWN()
	ON_WM_NCCALCSIZE()
END_MESSAGE_MAP()

// CHistoryList メッセージ ハンドラー

void CHistoryList::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	int nR, nC;
	NMCUSTOMDRAW* pia = (NMCUSTOMDRAW*)pNMHDR;
	NMLVCUSTOMDRAW *plvcd = (NMLVCUSTOMDRAW*)pia;
	DWORD dw;
	bool bEquipment;
	switch (plvcd->nmcd.dwDrawStage) {
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		nR = (int)plvcd->nmcd.dwItemSpec;
		nC = plvcd->iSubItem;
		dw = (DWORD)GetItemData(nR);
		bEquipment = (dw & 0x80000000)?true:false;
		dw &= 0x7fffffff;
		if (dw == eHistStatus_Normal){
			plvcd->clrTextBk = RGB(0, 0, 0);
			if(bEquipment)
				plvcd->clrText = RGB(0, 255, 0);
			else
				plvcd->clrText = RGB(255, 255, 255);
		}
		else if (dw == eHistStatus_Warning){
			plvcd->clrTextBk = RGB(0, 0, 0);
			plvcd->clrText = RGB(255, 255, 0);
		}
		else if (dw == eHistStatus_Fault){
			plvcd->clrTextBk = RGB(0, 0, 0);
			plvcd->clrText = RGB(255, 0, 0);
		}
		else{
			plvcd->clrTextBk = RGB(0, 0, 0);
			plvcd->clrText = RGB(255, 255, 255);
		}
		if (nC == eFilterItem_Station){
			CString str = GetItemText(nR, nC);
			plvcd->clrTextBk = RGB(255, 255, 255);
			for (int i = 0; i < sizeof(mStationString) / sizeof(mStationString[0]); ++i) {
				if (str == mStationString[i]){
					plvcd->clrTextBk = mStationColor[i];
					break;
				}
			}
			plvcd->clrText = RGB(0, 0, 0);
		}
		*pResult = CDRF_DODEFAULT;
		break;
	default:
		*pResult = CDRF_DODEFAULT;
		break;
	}
}

void CHistoryList::OnLButtonDown(UINT nFlags, CPoint point)
{
	int item = HitTest(point);
	UINT val = GetItemState(item, LVIS_SELECTED);
	if (val == LVIS_SELECTED){
		SetItemState(item, 0, LVIS_SELECTED);
		if (mParentWnd)	mParentWnd->PostMessage(eMessageTimeline_History, 0, 0);
		return;
	}
	else{
		if (mParentWnd)	mParentWnd->PostMessage(eMessageTimeline_History, 0, 1);
	}

	CListCtrl::OnLButtonDown(nFlags, point);
}


void CHistoryList::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	//常にスクロールバーを表示させる
	EnableScrollBarCtrl(SB_VERT | SB_HORZ, TRUE);

	CListCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);
}
