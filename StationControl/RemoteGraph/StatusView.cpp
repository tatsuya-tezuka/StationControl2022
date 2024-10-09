// StatusView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RemoteGraph.h"
#include "StatusView.h"


/////////////////////////////////////////////////////////////////////////////
// CStatusList

IMPLEMENT_DYNAMIC(CStatusList, CMFCListCtrl)

CStatusList::CStatusList()
{
	m_bColor = FALSE;
//	m_bModifyFont = FALSE;
}

CStatusList::~CStatusList()
{
}

BEGIN_MESSAGE_MAP(CStatusList, CMFCListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CStatusList::OnNMCustomdraw)
END_MESSAGE_MAP()


// CStatusList メッセージ ハンドラー

void CStatusList::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
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
		int		lParam = (int)lplvcd->nmcd.lItemlParam;

		CDC *pDC = CDC::FromHandle(lplvcd->nmcd.hdc);
		CRect rcItem;
		GetItemRect(nItem, rcItem, LVIR_BOUNDS);
		bool bSelected = GetItemState(nItem, LVIS_SELECTED) == LVIS_SELECTED;
		CString sLabel = GetItemText(nItem, nSubItem);

		UINT nFormat = DT_LEFT | DT_SINGLELINE;
		pDC->SetBkMode(TRANSPARENT);
		lplvcd->clrText = RGB(0, 0, 0);
		lplvcd->clrTextBk = RGB(255, 255, 255);

		CString	station;
		if( theApp.GetDefSelectStationMode() == eStation_5 )
			station = _T("４局モード");
		else
		{
			UINT	nSelectStation = theApp.GetDefSelectStation();
			if( nSelectStation >= eStation_MAX )
				nSelectStation = 0;
			station.Format(_T("１局モード[%s]"), mStationSimpleString[nSelectStation]);
		}
		if( !theApp.GetFileServerName().IsEmpty() )
			station += _T(" ") + theApp.GetFileServerName();

		const CString startString = _T("開始時間 ");
		const CString stopString = _T("終了時間 ");
		const CString maxString = _T("8888/88/88 88:88:88");
		CSize sizeTitle = pDC->GetTextExtent(startString, (int)wcslen(startString));
		CSize sizeString = pDC->GetTextExtent(maxString, (int)wcslen(maxString));
		rcItem.DeflateRect(2, 2);
		CRect tempRect = rcItem;
		CString	strTime;

		if (nItem == 0){
			pDC->DrawText(station, &tempRect, nFormat);
		}
		if (nItem == 1){
			pDC->DrawText(startString, &tempRect, nFormat);
			tempRect.left += sizeTitle.cx;
			GetRealTimeStartString(strTime);
			pDC->DrawText((LPCTSTR)strTime, &tempRect, nFormat);
		}
		if (nItem == 2){
			pDC->DrawText(stopString, &tempRect, nFormat);
			tempRect.left += sizeTitle.cx;
			GetRealTimeStopString(strTime);
			pDC->DrawText((LPCTSTR)strTime, &tempRect, nFormat);
		}

		*pResult = CDRF_SKIPDEFAULT;
		break;
	}
	default:
		*pResult = CDRF_DODEFAULT;
		break;
	}
}

/*============================================================================*/
/*! ステータスビュー

-# リアルタイム開始時間文字列の取得

@param	str		リアルタイム開始時間文字列
@retval

*/
/*============================================================================*/
void CStatusList::GetRealTimeStartString(CString& str)
{
	str.Empty();
	CString	strYmd;
	CString	strHMS;
	if (theApp.GetRealTimeStart() == CTime(0)){
		str = (LPCTSTR)_T("0000/00/00 00:00:00");
	}
	else{
		strYmd = theApp.GetRealTimeStart().Format("%Y/%m/%d");
		strHMS = theApp.GetRealTimeStart().Format("%H:%M:%S");
		str = (LPCTSTR)(strYmd + _T(" ") + strHMS);
	}
}

/*============================================================================*/
/*! ステータスビュー

-# リアルタイム終了時間文字列の取得

@param	str		リアルタイム終了時間文字列
@retval

*/
/*============================================================================*/
void CStatusList::GetRealTimeStopString(CString& str)
{
	str.Empty();
	CString	strYmd;
	CString	strHMS;
	if (theApp.GetRealTimeLast() == CTime(0)){
		str = (LPCTSTR)_T("0000/00/00 00:00:00");
	}
	else{
		CTime tmp = (theApp.GetRealTimeStart() == theApp.GetRealTimeWriteMax()) ? theApp.GetRealTimeWriteMax() : (theApp.GetRealTimeWriteMax() - CTimeSpan(0, 0, 0, 1));
		strYmd = tmp.Format("%Y/%m/%d");
		strHMS = tmp.Format("%H:%M:%S");
		str = (LPCTSTR)(strYmd + _T(" ") + strHMS);
	}
}

COLORREF CStatusList::OnGetCellTextColor(int nRow, int nColum)
{
	if (!m_bColor)
	{
		return CMFCListCtrl::OnGetCellTextColor(nRow, nColum);
	}

	return(nRow % 2) == 0 ? RGB(128, 37, 0) : RGB(0, 0, 0);
}

COLORREF CStatusList::OnGetCellBkColor(int nRow, int nColum)
{
	if (!m_bColor)
	{
		return CMFCListCtrl::OnGetCellBkColor(nRow, nColum);
	}

	if (m_bMarkSortedColumn && nColum == m_iSortedColumn)
	{
		return(nRow % 2) == 0 ? RGB(233, 221, 229) : RGB(176, 218, 234);
	}

	return(nRow % 2) == 0 ? RGB(253, 241, 249) : RGB(196, 238, 254);
}

#if 0
HFONT CStatusList::OnGetCellFont(int nRow, int nColum, DWORD /*dwData* = 0*/)
{
	if (!m_bModifyFont)
	{
		return NULL;
	}

	if (nColum == 0)
	{
		return afxGlobalData.fontDefaultGUIBold;
	}

	return NULL;
}
#endif




// CStatusView

IMPLEMENT_DYNAMIC(CStatusView, CDockablePane)

CStatusView::CStatusView()
{

}

CStatusView::~CStatusView()
{
}

BEGIN_MESSAGE_MAP(CStatusView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CStatusView メッセージ ハンドラー

int CStatusView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

#if 0
	CFont defFont;
	defFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (defFont.GetLogFont(&lf)){
		lf.lfWeight = FW_BOLD;
		lf.lfHeight = 16;
		m_Font.CreateFontIndirect(&lf);
	}
	//m_Font.CreateStockObject(DEFAULT_GUI_FONT);
#endif

	CRect rectClient(0, 0, lpCreateStruct->cx, lpCreateStruct->cy);

	const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_SHOWSELALWAYS | LVS_SINGLESEL;
	if (!m_wndStatus.Create(dwStyle, rectClient, this, 1))
	{
		TRACE0("Failed to create output window\n");
		return -1;      // fail to create
	}
	m_wndStatus.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT/* | LVS_EX_GRIDLINES*/);

	m_wndStatus.InsertColumn(0, _T("note"), LVCFMT_LEFT, 1000);

#if 0
	m_wndStatus.SetFont(&m_Font);
#else
//	m_wndStatus.SetFont(&afxGlobalData.fontRegular);
#endif

	// 固定領域の設定
	// 1行目：局
	// 2行目：開始時間
	// 3行目：終了時間
	AddItem(0, 0, _T(""));
	AddItem(1, 0, _T(""));
	AddItem(2, 0, _T(""));
	AutoSizeColumns();

	return 0;
}

void CStatusView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect(rc);

	m_wndStatus.SetWindowPos(NULL, rc.left + 1, rc.top + 1, rc.Width() - 2, rc.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
	AutoSizeColumns();
}

void CStatusView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndStatus.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CStatusView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndStatus.SetFocus();
}

void CStatusView::FillDebugWindow(CString& str)
{
	const UINT mFixedCount = 2;

	if (m_wndStatus.GetItemCount() < mFixedCount){
		// 固定項目が設定されていない場合は何もしない
		return;
	}
	AddItem(m_wndStatus.GetItemCount(), 0, str);
}

void CStatusView::UpdateStatus()
{
	m_wndStatus.Invalidate();
}

template <class T> inline const T& MAXST(const T& t1, const T& t2) {
	return t1 < t2 ? t2 : t1;
}
/*============================================================================*/
/*! ステータスビュー

-# カラムの自動サイズ調整

@param	col		カラム位置のカラムサイズを自動に調整します
@retval

*/
/*============================================================================*/
void CStatusView::AutoSizeColumns(int col /*=-1*/)
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)m_wndStatus.GetDlgItem(0);

	CRect rect;
	m_wndStatus.GetClientRect(rect);

	m_wndStatus.SetRedraw(false);
	int mincol = col < 0 ? 0 : col;
	int maxcol = col < 0 ? pHeader->GetItemCount() - 1 : col;
	for (col = mincol; col <= maxcol; col++){
		m_wndStatus.SetColumnWidth(col, LVSCW_AUTOSIZE);
		int wc1 = m_wndStatus.GetColumnWidth(col);
		m_wndStatus.SetColumnWidth(col, LVSCW_AUTOSIZE_USEHEADER);
		int wc2 = m_wndStatus.GetColumnWidth(col);
		int wc = MAXST(wc1, wc2);
		wc = MAXST(wc, rect.Width());
		m_wndStatus.SetColumnWidth(col, wc);
	}
	m_wndStatus.SetRedraw(true);
}

/*============================================================================*/
/*! ステータスビュー

-# ステータスの登録

@param	nItem		アイテム番号
@param	nSubItem	サブアイテム番号
@param	strItem		設定文字列
@param	dwParam		ユーザ値
@retval

*/
/*============================================================================*/
int CStatusView::AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam/*=0*/)
{
	LV_ITEM lv;
	memset(&lv, 0, sizeof(lv));
	lv.mask = LVIF_TEXT;
	lv.iItem = nItem;
	lv.iSubItem = nSubItem;
	if (nSubItem == 0){
		lv.mask |= LVIF_PARAM;
		lv.lParam = dwParam;
	}
	lv.pszText = (LPTSTR)(LPCTSTR)strItem;
	lv.cchTextMax = strItem.GetLength();

	if (nSubItem == 0)
		return m_wndStatus.InsertItem(&lv);
	return m_wndStatus.SetItem(&lv);
}
