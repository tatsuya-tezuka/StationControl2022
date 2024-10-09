// ListCtrlEx.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ControlManagement.h"
#include "ListCtrlEx.h"


// CListCtrlEx

IMPLEMENT_DYNAMIC(CListCtrlEx, CListCtrl)

CListCtrlEx::CListCtrlEx()
{

}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListCtrlEx::OnNMCustomdraw)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CListCtrlEx メッセージ ハンドラー


void CListCtrlEx::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
	switch (lplvcd->nmcd.dwDrawStage) {
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
	{
		CDC *pDC = CDC::FromHandle(lplvcd->nmcd.hdc);
		int		nItem = (int)lplvcd->nmcd.dwItemSpec;
		int		nSubItem = lplvcd->iSubItem;
		CImageList* pImageList = GetImageList(LVSIL_STATE);

		CRect rcItem;
		GetCellRect(nItem, nSubItem, LVIR_BOUNDS, rcItem);
		rcItem.DeflateRect(2, 2);

		pImageList->Draw(pDC, (nSubItem!=0)?2:1, CPoint(rcItem.left, rcItem.top), ILD_TRANSPARENT);
		*pResult = CDRF_DODEFAULT;
		break;
	}
	default:
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
BOOL CListCtrlEx::GetCellRect(int iRow, int iCol, int nArea, CRect &rect)
{
	if (iCol) return GetSubItemRect(iRow, iCol, nArea, rect);

	if (GetHeaderCtrl()->GetItemCount() == 1)
		return GetItemRect(iRow, rect, nArea);

	CRect rCol1;
	if (!GetSubItemRect(iRow, 1, nArea, rCol1))
		return FALSE;

	if (!GetItemRect(iRow, rect, nArea))
		return FALSE;

	rect.right = rCol1.left;

	return TRUE;
}


void CListCtrlEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem(lpDrawItemStruct->rcItem);
	int nItem = lpDrawItemStruct->itemID;

	int nSavedDC = pDC->SaveDC();

	// アイテム情報の取得
	LV_ITEM lvi;
	lvi.mask = LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.stateMask = 0xFFFF;
	GetItem(&lvi);

	// アイテムのハイライト表示の取得
	BOOL bHighlight = ((lvi.state & LVIS_DROPHILITED) ||
		((lvi.state & LVIS_SELECTED) &&
		((GetFocus() == this) ||
		(GetStyle() & LVS_SHOWSELALWAYS))));

	// 各種領域の取得
	CRect rcBounds, rcLabel, rcIcon;
	GetItemRect(nItem, rcBounds, LVIR_BOUNDS);
	GetItemRect(nItem, rcLabel, LVIR_LABEL);
	GetItemRect(nItem, rcIcon, LVIR_ICON);
	CRect rcCol(rcLabel);
	CRect rcHighlight(rcBounds);

	rcCol.left = 0;

	// 文字列の取得
	CString sLabel = GetItemText(nItem, 0);

	// 背景の描画
	if (IsWindowEnabled()){
		rcHighlight.DeflateRect(1, 1);
		if (bHighlight){
			pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
			pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
			pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));
		}
		else{
			pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_WINDOW)));
		}
	}
	else{
		pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_BTNFACE)));
	}

	// 文字色の設定
	// テキスト色の設定
	if (bHighlight)
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	else
		pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));

	BYTE myComputer = (lvi.lParam >>24) & 0xff;
	if (myComputer == 0)
		pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));


	int offset = pDC->GetTextExtent(_T(" "), 1).cx * 2;
	// 先頭アイテムの描画
	rcLabel.left = 3;
	pDC->DrawText(sLabel, -1, rcLabel, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS | DT_NOCLIP);

	// 残りのアイテムの描画
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;

	for (int nColumn = 1; GetColumn(nColumn, &lvc); nColumn++){
		rcCol.left = rcCol.right;
		rcCol.right += lvc.cx;

		// サブアイテム文字列の取得
		sLabel = GetItemText(nItem, nColumn);
		
		int image = 0;
		switch (nColumn){
		case	1:/*usc20*/
			image = (lvi.lParam >> 3) & 0x01;
			break;
		case	2:/*usc34*/
			image = (lvi.lParam >> 2) & 0x01;
			break;
		case	3:/*udsc64*/
			image = (lvi.lParam >> 1) & 0x01;
			break;
		case	4:/*xdsc54*/
			image = (lvi.lParam >> 0) & 0x01;
			break;
		}

		CImageList* pImageList = GetImageList(LVSIL_STATE);
		pImageList->Draw(pDC, image+1, CPoint(rcCol.left+(rcCol.Width()/2)-8, rcCol.top), ILD_TRANSPARENT);
	}

	// ファーカス矩形の描画
	if (lvi.state & LVIS_FOCUSED && (GetFocus() == this)){
		pDC->DrawFocusRect(rcHighlight);
	}

	pDC->RestoreDC(nSavedDC);
}


void CListCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListCtrl::OnLButtonDown(nFlags, point);

	int pos = 0, index;
	int colnum, strpos = 0;
	CString str;
	if ((index = HitTestEx(point, &colnum)) != -1 && colnum != 0){
		UINT flag = LVIS_SELECTED;
		if ((GetItemState(index, flag) & flag) == flag){
			LPARAM lParam = GetItemData(index);
			int image = 0;
			switch (colnum){
			case	1:/*usc20*/
				image = (lParam >> 3) & 0x01;
				if (image == 1)
					lParam &= ~(0x00000008);
				else
					lParam |= (1 << 3);
				break;
			case	2:/*usc34*/
				image = (lParam >> 2) & 0x01;
				if (image == 1)
					lParam &= ~(0x00000004);
				else
					lParam |= (1 << 2);
				break;
			case	3:/*udsc64*/
				image = (lParam >> 1) & 0x01;
				if (image == 1)
					lParam &= ~(0x00000002);
				else
					lParam |= (1 << 1);
				break;
			case	4:/*xdsc54*/
				image = (lParam >> 0) & 0x01;
				if (image == 1)
					lParam &= ~(0x00000001);
				else
					lParam |= (1 << 0);
				break;
			}
			SetItemData(index, lParam);
		}
		else{
			SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}
//============================================================================
/// @brief	マウス位置情報からアイテム番号を取得する
/// @param
/// @return	成功した場合はtrueを返す
//============================================================================
int CListCtrlEx::HitTestEx(CPoint &point, int *col)
{
	int colnum = 0;
	int row = HitTest(point, NULL);
	if (col) *col = 0;

	if ((GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
		return row;

	// 現在表示されている最初のアイテムインデックスを取得する
	row = GetTopIndex();

	// 可視のアイテム数を取得する
	int bottom = row + GetCountPerPage();

	if (bottom > GetItemCount())
		bottom = GetItemCount();

	// Get the number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();

	for (; row <= bottom; row++){
		// アイテム領域の取得
		CRect rect;
		GetItemRect(row, &rect, LVIR_BOUNDS);

		if (rect.PtInRect(point)){
			// Now find the column
			for (colnum = 0; colnum < nColumnCount; colnum++){
				int colwidth = GetColumnWidth(colnum);
				if (point.x >= rect.left && point.x <= (rect.left + colwidth)){
					if (col) *col = colnum;
					return row;
				}
				rect.left += colwidth;
			}
		}
	}
	return -1;
}
