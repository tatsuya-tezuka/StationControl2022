// この MFC サンプル ソース コードでは、MFC Microsoft Office Fluent ユーザー インターフェイス 
// ("Fluent UI") の使用方法を示します。このコードは、MFC C++ ライブラリ ソフトウェアに 
// 同梱されている Microsoft Foundation Class リファレンスおよび関連電子ドキュメントを
// 補完するための参考資料として提供されます。
// Fluent UI を複製、使用、または配布するためのライセンス条項は個別に用意されています。
// Fluent UI ライセンス プログラムの詳細については、Web サイト
// http://go.microsoft.com/fwlink/?LinkId=238214 を参照してください。
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#include "stdafx.h"
#include "RemoteGraph.h"

#include "LimitWnd.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLimitBar

CLimitWnd::CLimitWnd()
{
	for( int i = 0; i < MAX_VIEW; i++ )
		m_listDisplayTab[i] = false;

	memset(m_listDataId, 0xff, sizeof(m_listDataId));
}

CLimitWnd::~CLimitWnd()
{
}

BEGIN_MESSAGE_MAP(CLimitWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CLimitWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_rectList.SetRectEmpty();

	// タブ付きウィンドウの作成:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, m_rectList, this, 1))
	{
		TRACE0("タブ付き出力ウィンドウを作成できませんでした\n");
		return -1;      // 作成できない場合
	}

	//UpdateFonts();

	//CString strTabName;
	//BOOL bNameValid;

	// 一覧ウィンドウをタブに割り当てます:
	//bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
	//ASSERT(bNameValid);
	//strTabName = _T("数値");
	//m_wndTabs.AddTab(&m_wndLimitBuild, strTabName, (UINT)0);
	//bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
	//ASSERT(bNameValid);
	//m_wndTabs.AddTab(&m_wndLimitDebug, strTabName, (UINT)1);
	//bNameValid = strTabName.LoadString(IDS_FIND_TAB);
	//ASSERT(bNameValid);
	//m_wndTabs.AddTab(&m_wndLimitFind, strTabName, (UINT)2);

	// 出力タブにダミー テキストを入力します
	//FillBuildWindow();
	//FillDebugWindow();
	//FillFindWindow();

	return 0;
}

void CLimitWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// タブ コントロールは、クライアント領域全体をカバーする必要があります:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void CLimitWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, (int)dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void CLimitWnd::FillBuildWindow()
{
	m_wndLimitBuild.AddString(_T("最大値    ：99999"));
	m_wndLimitBuild.AddString(_T("最小値    ：99999"));
	m_wndLimitBuild.AddString(_T("平均値    ：99999"));
	m_wndLimitBuild.AddString(_T("カーソル値：99999"));
}

void CLimitWnd::FillDebugWindow()
{
	m_wndLimitDebug.AddString(_T("デバッグ出力データがここに表示されます。"));
	m_wndLimitDebug.AddString(_T("出力データはリスト ビューの各行に表示されます"));
	m_wndLimitDebug.AddString(_T("表示方法を変更することもできます..."));
}

void CLimitWnd::FillFindWindow()
{
	m_wndLimitFind.AddString(_T("検索出力データがここに表示されます。"));
	m_wndLimitFind.AddString(_T("出力データはリスト ビューの各行に表示されます"));
	m_wndLimitFind.AddString(_T("表示方法を変更することもできます..."));
}

void CLimitWnd::UpdateFonts()
{
	//m_wndLimitBuild.SetFont(&afxGlobalData.fontRegular);
	//m_wndLimitDebug.SetFont(&afxGlobalData.fontRegular);
	//m_wndLimitFind.SetFont(&afxGlobalData.fontRegular);
}


/*============================================================================*/
/*! 出力ウィンドウ

-# タブの追加

@param		pos		ウィンドウインデックス

@retval

*/
/*============================================================================*/
void CLimitWnd::AddTab(UINT pos)
{
	// プロパティデータ
	CPropertiesData& prop = theApp.GetPropertiesData(pos);

	CString strTabName;
	BOOL bNameValid;
	// 一覧ウィンドウをタブに割り当てます:
	bNameValid = strTabName.LoadString(IDS_DATA_TAB_1+pos);
	ASSERT(bNameValid);
	strTabName = prop.wndName;

	// 出力リストの作成
	if (m_ListLimit[pos].GetSafeHwnd() == NULL){
		const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | LVS_REPORT;
		if (!m_ListLimit[pos].Create(dwStyle, m_rectList, &m_wndTabs, pos+100))
		{
			TRACE0("出力ウィンドウを作成できませんでした\n");
			return;      // 作成できない場合
		}
		m_ListLimit[pos].SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
//		m_ListLimit[pos].InsertColumn(0, _T("No."), LVCFMT_LEFT, 100);
//		m_ListLimit[pos].InsertColumn(0, _T("Time"), LVCFMT_LEFT, 150);
		for (int i = 0; i < MAX_TDATA; i++){
			m_ListLimit[pos].InsertColumn(i + 0, prop.item[i].dataGName, LVCFMT_LEFT, 230);
//			m_ListLimit[pos].SetItemText(0, i + 1, _T("XXXX"));
		}
		for( int i = 0; i < MAX_LIMIT_HISTORY; i++ )
			m_ListLimit[pos].InsertItem(i, _T(""));
		m_ListLimit[pos].m_pos = pos;
	}

	// データIDを更新
	UpdateDataId(pos);

//	m_wndTabs.AddTab(&m_ListLimit[pos], strTabName, (UINT)pos);
	UINT	insert = 0;
	for( UINT i = 0; i < MAX_VIEW; i++ )
	{
		if( m_listDisplayTab[i] && (i > pos) )
			break;

		if( m_listDisplayTab[i] )
			insert++;
	}

	m_listDisplayTab[pos] = true;

	m_wndTabs.InsertTab(&m_ListLimit[pos], strTabName, insert, (UINT)pos);

	// 追加したタブにアクティブに変更
	UpdateTab(pos);
}

/*============================================================================*/
/*! 出力ウィンドウ

-# データIDを更新

@param		pos		ウィンドウインデックス

@retval

*/
/*============================================================================*/
void CLimitWnd::UpdateDataId(UINT pos)
{
	if (m_ListLimit[pos].GetSafeHwnd() == NULL)
		return;

	CPropertiesData& prop = theApp.GetPropertiesData(pos);

	for (int i = 0; i < MAX_TDATA; i++)
	{
		LVCOLUMN	col;
		col.mask = LVCF_TEXT;
//		m_ListLimit[pos].GetColumn(i + 1, &col);
		col.pszText = LPTSTR((LPCTSTR)prop.item[i].dataGName);
		m_ListLimit[pos].SetColumn(i + 0, &col);

		m_listDataId[pos][i] = theApp.GetDataCollection().GetSelectDataId(prop.item[i].dataName);
	}
}

/*============================================================================*/
/*! 出力ウィンドウ

-# タブの追加

@param		pos		ウィンドウインデックス

@retval

*/
/*============================================================================*/
void CLimitWnd::UpdateTab(UINT pos)
{
	// 既にアクティブならスルー
	CMFCLimitListCtrlEx	*pWnd = (CMFCLimitListCtrlEx *)m_wndTabs.GetTabWnd(m_wndTabs.GetActiveTab());
	if( pWnd != NULL && pWnd->m_pos == pos )
		return;
	
	// プロパティデータ
	CPropertiesData& prop = theApp.GetPropertiesData(pos);

	CString strTabName;
	BOOL bNameValid;

	// 一覧ウィンドウをタブに割り当てます:
	bNameValid = strTabName.LoadString(IDS_DATA_TAB_1 + pos);
	ASSERT(bNameValid);

	// 該当タブをアクティブに変更
	m_wndTabs.SetActiveTab(SearchTabIndex(prop.wndName));
}

/*============================================================================*/
/*! 出力ウィンドウ

-# タブの追加

@param		pos		ウィンドウインデックス

@retval

*/
/*============================================================================*/
void CLimitWnd::DeleteTab(UINT pos)
{
	// プロパティデータ
	CPropertiesData& prop = theApp.GetPropertiesData(pos);

	// 該当タブを削除
	m_wndTabs.RemoveTab(SearchTabIndex(prop.wndName));

	m_listDisplayTab[pos] = false;
}

/*============================================================================*/
/*! 出力ウィンドウ

-# データの更新

@param		pos		ウィンドウインデックス
@param		time	データ時間
@param		list	最大データ数１０の配列（グラフ非表示の場合も）

@retval

*/
/*============================================================================*/
void CLimitWnd::UpdateDataEx(UINT pos)
{
	// プロパティデータ
	CPropertiesData& prop = theApp.GetPropertiesData(pos);

	// 指定プロパティデータのウィンドウ名から該当タブのウィンドウハンドルを取得
	CMFCLimitListCtrlEx	*pWnd = (CMFCLimitListCtrlEx *)m_wndTabs.GetTabWnd(SearchTabIndex(prop.wndName));
	if( pWnd == NULL )
		return;

	CGraphMutex::Lock(eLimitState);

	// 該当する監視データの値を出力
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		deque<stLimitState>&	tmpLimitState = theApp.GetDataCollection().GetLimitState(pos, i);

		for( int j = 0; j < MAX_LIMIT_HISTORY; j++ )
		{
			// 履歴が存在するか？
			if( j < tmpLimitState.size() )
			{
				CString	str;

				str.Format(_T("%s %s %s%02d[%s] %s"),
					(LPCTSTR)(tmpLimitState[j].time.Format("%Y/%m/%d")),
					(LPCTSTR)(tmpLimitState[j].time.Format("%H:%M:%S")),
					(GET_LIMIT_KIND(tmpLimitState[j].state) == LimitWarning) ? _T("w") : _T("a"),
					i + 1,
					(GET_LIMIT_RANGE(tmpLimitState[j].state) == LimitUpper) ? _T("up") : _T("lo"),
					(GET_LIMIT_STATUS(tmpLimitState[j].state) == LimitAbove) ? _T("above") : _T("below"));

				pWnd->SetItemText(j, i, str);
			}
			else
				pWnd->SetItemText(j, i, _T(""));
		}
	}

	CGraphMutex::Unlock(eLimitState);
}

/*============================================================================*/
/*! 出力ウィンドウ

-# リストへのデータ登録

@param		pos			ウィンドウインデックス
@param		nItem		アイテム番号
@param		nSubItem	サブアイテム番号
@param		strItem		文字列
@param		dwAll		データ

@retval		int

*/
/*============================================================================*/
int CLimitWnd::AddItem(int pos, int nItem, int nSubItem, CString strItem, DWORD dwAll/*=0*/)
{
	LV_ITEM lv;
	memset(&lv, 0, sizeof(lv));
	lv.mask = LVIF_TEXT;
	lv.iItem = nItem;
	lv.iSubItem = nSubItem;
	if (nSubItem == 0){
		lv.mask |= LVIF_PARAM;
		lv.lParam = dwAll;
	}
	lv.pszText = (LPTSTR)(LPCTSTR)strItem;
	lv.cchTextMax = strItem.GetLength();

	if (nSubItem == 0)
		return m_ListLimit[pos].InsertItem(&lv);
	return m_ListLimit[pos].SetItem(&lv);
}


/////////////////////////////////////////////////////////////////////////////
// CLimitList1

CLimitList::CLimitList()
{
}

CLimitList::~CLimitList()
{
}

BEGIN_MESSAGE_MAP(CLimitList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_LIMITWND, OnViewLimit)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CLimitList メッセージ ハンドラー

void CLimitList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_LIMIT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void CLimitList::OnEditCopy()
{
	MessageBox(_T("出力データをコピーします"));
}

void CLimitList::OnEditClear()
{
	MessageBox(_T("出力データをクリアします"));
}

void CLimitList::OnViewLimit()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}




CLimitListCtrl::CLimitListCtrl()
{
}

CLimitListCtrl::~CLimitListCtrl()
{
}


BEGIN_MESSAGE_MAP(CLimitListCtrl, CListCtrl)
END_MESSAGE_MAP()


// CMFCLimitListCtrlEx

IMPLEMENT_DYNAMIC(CMFCLimitListCtrlEx, CMFCListCtrl)

CMFCLimitListCtrlEx::CMFCLimitListCtrlEx()
{
	m_pos = -1;
}

CMFCLimitListCtrlEx::~CMFCLimitListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CMFCLimitListCtrlEx, CMFCListCtrl)
END_MESSAGE_MAP()



// CMFCLimitListCtrlEx メッセージ ハンドラー


// CMFCLimitTabCtrlEx

IMPLEMENT_DYNAMIC(CMFCLimitTabCtrlEx, CMFCTabCtrl)

CMFCLimitTabCtrlEx::CMFCLimitTabCtrlEx()
{

}

CMFCLimitTabCtrlEx::~CMFCLimitTabCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CMFCLimitTabCtrlEx, CMFCTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


BOOL CMFCLimitTabCtrlEx::OnCommand(WPARAM wParam, LPARAM lParam)
{
	BOOL bScrollTabs = FALSE;

	int count = m_iTabsNum;

	// OnCommandでボタンイベントを処理する
	if ((HWND)lParam == m_btnScrollLeft.GetSafeHwnd())
	{
		if (m_iActiveTab != 0){
			SetActiveTab(m_iActiveTab - 1);

			// 該当ビューをアクティブに設定
			CMFCLimitListCtrlEx	*pWnd = (CMFCLimitListCtrlEx *)GetTabWnd(GetActiveTab());
			if( pWnd != NULL )
				theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)GetActiveTab(), (LPARAM)pWnd->m_pos);
		}
		// 左ボタン押下
	}
	else if ((HWND)lParam == m_btnScrollRight.GetSafeHwnd())
	{
		// 右ボタン押下
		if (m_iActiveTab != (m_iTabsNum-1)){
			SetActiveTab(m_iActiveTab + 1);

			// 該当ビューをアクティブに設定
			CMFCLimitListCtrlEx	*pWnd = (CMFCLimitListCtrlEx *)GetTabWnd(GetActiveTab());
			if( pWnd != NULL )
				theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)GetActiveTab(), (LPARAM)pWnd->m_pos);
		}
	}
	else if ((HWND)lParam == m_btnScrollFirst.GetSafeHwnd())
	{
		// 最左ボタン押下
		SetActiveTab(0);

		// 該当ビューをアクティブに設定
		CMFCLimitListCtrlEx	*pWnd = (CMFCLimitListCtrlEx *)GetTabWnd(GetActiveTab());
		if( pWnd != NULL )
			theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)GetActiveTab(), (LPARAM)pWnd->m_pos);
	}
	else if ((HWND)lParam == m_btnScrollLast.GetSafeHwnd())
	{
		// 最右ボタン押下
		SetActiveTab((m_iTabsNum - 1));

		// 該当ビューをアクティブに設定
		CMFCLimitListCtrlEx	*pWnd = (CMFCLimitListCtrlEx *)GetTabWnd(GetActiveTab());
		if( pWnd != NULL )
			theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)GetActiveTab(), (LPARAM)pWnd->m_pos);
	}

	return CMFCTabCtrl::OnCommand(wParam, lParam);
}

#if 0
void CMFCLimitTabCtrlEx::FireChangeActiveTab(int nNewTab)
{
	CMFCLimitListCtrlEx	*pWnd = (CMFCLimitListCtrlEx *)GetTabWnd(nNewTab);

	if( pWnd != NULL )
		theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)nNewTab, (LPARAM)pWnd->m_pos);

	return CMFCTabCtrl::FireChangeActiveTab(nNewTab);
}
#endif

void CMFCLimitTabCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMFCTabCtrl::OnLButtonDown(nFlags, point);

	// 該当ビューをアクティブに設定
	CMFCLimitListCtrlEx	*pWnd = (CMFCLimitListCtrlEx *)GetTabWnd(GetActiveTab());
	if( pWnd != NULL )
		theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)GetActiveTab(), (LPARAM)pWnd->m_pos);
}

LRESULT CLimitWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case eMessage_UpdateCursorLine:
		{
			CPropertiesData	*prop = &theApp.GetPropertiesData((UINT)wParam);

			if( !prop->isRealTime )
				UpdateDataEx((UINT)wParam);
		}
		break;

	//=====================================================================
	//	スレッド
	//=====================================================================
	case eMessage_ThreadNormal:
		{
			CPropertiesData	*prop = &theApp.GetPropertiesData((UINT)wParam);

//			if( prop->isRealTime )
				UpdateDataEx((UINT)wParam);
		}
		break;

	default:
		return CDockablePane::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}
