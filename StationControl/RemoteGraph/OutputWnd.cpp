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

#include "OutputWnd.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
	for( int i = 0; i < MAX_VIEW; i++ )
		m_listDisplayTab[i] = false;

	memset(m_listDataId, 0xff, sizeof(m_listDataId));
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
	//m_wndTabs.AddTab(&m_wndOutputBuild, strTabName, (UINT)0);
	//bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
	//ASSERT(bNameValid);
	//m_wndTabs.AddTab(&m_wndOutputDebug, strTabName, (UINT)1);
	//bNameValid = strTabName.LoadString(IDS_FIND_TAB);
	//ASSERT(bNameValid);
	//m_wndTabs.AddTab(&m_wndOutputFind, strTabName, (UINT)2);

	// 出力タブにダミー テキストを入力します
	//FillBuildWindow();
	//FillDebugWindow();
	//FillFindWindow();

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// タブ コントロールは、クライアント領域全体をカバーする必要があります:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
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

void COutputWnd::FillBuildWindow()
{
	m_wndOutputBuild.AddString(_T("最大値    ：99999"));
	m_wndOutputBuild.AddString(_T("最小値    ：99999"));
	m_wndOutputBuild.AddString(_T("平均値    ：99999"));
	m_wndOutputBuild.AddString(_T("カーソル値：99999"));
}

void COutputWnd::FillDebugWindow()
{
	m_wndOutputDebug.AddString(_T("デバッグ出力データがここに表示されます。"));
	m_wndOutputDebug.AddString(_T("出力データはリスト ビューの各行に表示されます"));
	m_wndOutputDebug.AddString(_T("表示方法を変更することもできます..."));
}

void COutputWnd::FillFindWindow()
{
	m_wndOutputFind.AddString(_T("検索出力データがここに表示されます。"));
	m_wndOutputFind.AddString(_T("出力データはリスト ビューの各行に表示されます"));
	m_wndOutputFind.AddString(_T("表示方法を変更することもできます..."));
}

void COutputWnd::UpdateFonts()
{
	//m_wndOutputBuild.SetFont(&afxGlobalData.fontRegular);
	//m_wndOutputDebug.SetFont(&afxGlobalData.fontRegular);
	//m_wndOutputFind.SetFont(&afxGlobalData.fontRegular);
}


/*============================================================================*/
/*! 出力ウィンドウ

-# タブの追加

@param		pos		ウィンドウインデックス

@retval

*/
/*============================================================================*/
void COutputWnd::AddTab(UINT pos)
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
	if (m_ListOutput[pos].GetSafeHwnd() == NULL){
		const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | LVS_REPORT;
		if (!m_ListOutput[pos].Create(dwStyle, m_rectList, &m_wndTabs, pos+100))
		{
			TRACE0("出力ウィンドウを作成できませんでした\n");
			return;      // 作成できない場合
		}
		m_ListOutput[pos].SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
//		m_ListOutput[pos].InsertColumn(0, _T("No."), LVCFMT_LEFT, 100);
//		m_ListOutput[pos].InsertColumn(0, _T("Time"), LVCFMT_LEFT, 150);
		for (int i = 0; i < MAX_TDATA; i++){
			m_ListOutput[pos].InsertColumn(i + 0, prop.item[i].dataGName, LVCFMT_LEFT, 120);
//			m_ListOutput[pos].SetItemText(0, i + 1, _T("XXXX"));
		}
		m_ListOutput[pos].InsertItem(0, _T(""));
		m_ListOutput[pos].InsertItem(1, _T(""));
		m_ListOutput[pos].InsertItem(2, _T(""));
		m_ListOutput[pos].InsertItem(3, _T(""));
		m_ListOutput[pos].InsertItem(4, _T(""));
		m_ListOutput[pos].m_pos = pos;
	}

	// データIDを更新
	UpdateDataId(pos);

//	m_wndTabs.AddTab(&m_ListOutput[pos], strTabName, (UINT)pos);
	UINT	insert = 0;
	for( UINT i = 0; i < MAX_VIEW; i++ )
	{
		if( m_listDisplayTab[i] && (i > pos) )
			break;

		if( m_listDisplayTab[i] )
			insert++;
	}

	m_listDisplayTab[pos] = true;

	m_wndTabs.InsertTab(&m_ListOutput[pos], strTabName, insert, (UINT)pos);

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
void COutputWnd::UpdateDataId(UINT pos)
{
	if (m_ListOutput[pos].GetSafeHwnd() == NULL)
		return;

	CPropertiesData& prop = theApp.GetPropertiesData(pos);

	for (int i = 0; i < MAX_TDATA; i++)
	{
		LVCOLUMN	col;
		col.mask = LVCF_TEXT;
//		m_ListOutput[pos].GetColumn(i + 1, &col);
		col.pszText = LPTSTR((LPCTSTR)prop.item[i].dataGName);
		m_ListOutput[pos].SetColumn(i + 0, &col);

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
void COutputWnd::UpdateTab(UINT pos)
{
	// 既にアクティブならスルー
	CMFCOutputListCtrlEx	*pWnd = (CMFCOutputListCtrlEx *)m_wndTabs.GetTabWnd(m_wndTabs.GetActiveTab());
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
void COutputWnd::DeleteTab(UINT pos)
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
void COutputWnd::UpdateDataEx(UINT pos)
{
	// プロパティデータ
	CPropertiesData& prop = theApp.GetPropertiesData(pos);

	// 指定プロパティデータのウィンドウ名から該当タブのウィンドウハンドルを取得
	CMFCOutputListCtrlEx	*pWnd = (CMFCOutputListCtrlEx *)m_wndTabs.GetTabWnd(SearchTabIndex(prop.wndName));
	if( pWnd == NULL )
		return;

	if( prop.isRealTime )
		CGraphMutex::Lock(eRealTimeData);
	else
		CGraphMutex::Lock_Analysis(pos);

	// 監視データリストの取得
	deque<CSpaceData>&	spaceDataList = theApp.GetDataCollection().GetSpaceData();

	// 該当する監視データの値を出力
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		CString	str;
		double	max, min, mmax, mmin;

		bool	b_getdata = false;
		bool	b_mm_getdata = false;
		bool	b_validdata = false;
		
		if( prop.isRealTime )
		{
			int	index = -1;

			// 監視データリストのループ
			for( int j = 0; j < spaceDataList.size(); j++ )
			{
				// 選択中の監視データIDが一致する監視データを検出
				if( m_listDataId[pos][i] != spaceDataList[j].dataid )
					continue;

				index = j;

				break;
			}

			if( index != -1 )
			{
				stSpaceData&	sData = spaceDataList[index].value.back();

				max = sData.mmPair.max;
				min = sData.mmPair.min;
				b_getdata = true;

				mmax = spaceDataList[index].data_max;
				mmin = spaceDataList[index].data_min;
				b_mm_getdata = ((mmax == -DBL_MAX) || (mmin == DBL_MAX)) ? false : true;

				b_validdata = (sData.status == SDS_INVALID) ? false : true;
			}
		}
		else
		{
			vector<stSpaceData>&	data = theApp.GetDataCollection().GetTrendCursorData(pos, i);

			CChildFrame	*pChildFrame = (CChildFrame *)((CMainFrame *)theApp.m_pMainWnd)->FindChildListWnd(pos);

			if( (pChildFrame != NULL) && (data.size() != 0) )
			{
				max = data[0].mmPair.max;
				min = data[0].mmPair.min;
				b_getdata = true;

				stMMPair&	pair = theApp.GetDataCollection().GetTrendZoomMMPair(pos, i);
				mmax = pair.max;
				mmin = pair.min;
//				pChildFrame->GetMinMaxValue(mmin, mmax);	// ZOOM側のMin/Max値を取得

				double	t_max, t_min;
				pChildFrame->GetMinMaxValueEx(t_min, t_max);
				b_mm_getdata = ((mmax == -DBL_MAX) || (mmin == DBL_MAX) || (t_max == -DBL_MAX) || (t_min == DBL_MAX) || (t_max == -FLT_MAX) || (t_min == FLT_MAX)) ? false : true;

				b_validdata = (data[0].status == SDS_INVALID) ? false : true;
			}
		}

		int	offs = 0;

		if( b_getdata )
		{
			// Min/Max値を表示
			if( b_validdata )
			{
				str.Format(_T("%.6f %s"), max, (LPCTSTR)prop.item[i].dataUnit);
				pWnd->SetItemText(0, i + offs, str);
				str.Format(_T("%.6f %s"), min, (LPCTSTR)prop.item[i].dataUnit);
				pWnd->SetItemText(1, i + offs, str);
			}
			else
			{
				pWnd->SetItemText(0, i + offs, _T("---"));
				pWnd->SetItemText(1, i + offs, _T("---"));
			}

			// 統計Min／Maxを表示
			if( b_mm_getdata )
			{
				str.Format(_T("%.6f %s"), mmax, (LPCTSTR)prop.item[i].dataUnit);
				pWnd->SetItemText(2, i + offs, str);
				str.Format(_T("%.6f %s"), mmin, (LPCTSTR)prop.item[i].dataUnit);
				pWnd->SetItemText(3, i + offs, str);
			}
			else
			{
				pWnd->SetItemText(2, i + offs, _T(""));
				pWnd->SetItemText(3, i + offs, _T(""));
			}

			// コメントを表示
			pWnd->SetItemText(4, i + offs, prop.item[i].comment);
		}
		else
		{
			// 監視データが検出されなかったら表示をクリア
			pWnd->SetItemText(0, i + offs, _T(""));
			pWnd->SetItemText(1, i + offs, _T(""));
			pWnd->SetItemText(2, i + offs, _T(""));
			pWnd->SetItemText(3, i + offs, _T(""));
			pWnd->SetItemText(4, i + offs, _T(""));
		}
	}

	if( prop.isRealTime )
		CGraphMutex::Unlock(eRealTimeData);
	else
		CGraphMutex::Unlock_Analysis(pos);
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
int COutputWnd::AddItem(int pos, int nItem, int nSubItem, CString strItem, DWORD dwAll/*=0*/)
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
		return m_ListOutput[pos].InsertItem(&lv);
	return m_ListOutput[pos].SetItem(&lv);
}


/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList メッセージ ハンドラー

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

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

void COutputList::OnEditCopy()
{
	MessageBox(_T("出力データをコピーします"));
}

void COutputList::OnEditClear()
{
	MessageBox(_T("出力データをクリアします"));
}

void COutputList::OnViewOutput()
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




COutputListCtrl::COutputListCtrl()
{
}

COutputListCtrl::~COutputListCtrl()
{
}


BEGIN_MESSAGE_MAP(COutputListCtrl, CListCtrl)
END_MESSAGE_MAP()


// CMFCOutputListCtrlEx

IMPLEMENT_DYNAMIC(CMFCOutputListCtrlEx, CMFCListCtrl)

CMFCOutputListCtrlEx::CMFCOutputListCtrlEx()
{
	m_pos = -1;
}

CMFCOutputListCtrlEx::~CMFCOutputListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CMFCOutputListCtrlEx, CMFCListCtrl)
END_MESSAGE_MAP()



// CMFCOutputListCtrlEx メッセージ ハンドラー


// CMFCOutputTabCtrlEx

IMPLEMENT_DYNAMIC(CMFCOutputTabCtrlEx, CMFCTabCtrl)

CMFCOutputTabCtrlEx::CMFCOutputTabCtrlEx()
{

}

CMFCOutputTabCtrlEx::~CMFCOutputTabCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CMFCOutputTabCtrlEx, CMFCTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


BOOL CMFCOutputTabCtrlEx::OnCommand(WPARAM wParam, LPARAM lParam)
{
	BOOL bScrollTabs = FALSE;

	int count = m_iTabsNum;

	// OnCommandでボタンイベントを処理する
	if ((HWND)lParam == m_btnScrollLeft.GetSafeHwnd())
	{
		if (m_iActiveTab != 0){
			SetActiveTab(m_iActiveTab - 1);

			// 該当ビューをアクティブに設定
			CMFCOutputListCtrlEx	*pWnd = (CMFCOutputListCtrlEx *)GetTabWnd(GetActiveTab());
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
			CMFCOutputListCtrlEx	*pWnd = (CMFCOutputListCtrlEx *)GetTabWnd(GetActiveTab());
			if( pWnd != NULL )
				theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)GetActiveTab(), (LPARAM)pWnd->m_pos);
		}
	}
	else if ((HWND)lParam == m_btnScrollFirst.GetSafeHwnd())
	{
		// 最左ボタン押下
		SetActiveTab(0);

		// 該当ビューをアクティブに設定
		CMFCOutputListCtrlEx	*pWnd = (CMFCOutputListCtrlEx *)GetTabWnd(GetActiveTab());
		if( pWnd != NULL )
			theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)GetActiveTab(), (LPARAM)pWnd->m_pos);
	}
	else if ((HWND)lParam == m_btnScrollLast.GetSafeHwnd())
	{
		// 最右ボタン押下
		SetActiveTab((m_iTabsNum - 1));

		// 該当ビューをアクティブに設定
		CMFCOutputListCtrlEx	*pWnd = (CMFCOutputListCtrlEx *)GetTabWnd(GetActiveTab());
		if( pWnd != NULL )
			theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)GetActiveTab(), (LPARAM)pWnd->m_pos);
	}

	return CMFCTabCtrl::OnCommand(wParam, lParam);
}

#if 0
void CMFCOutputTabCtrlEx::FireChangeActiveTab(int nNewTab)
{
	CMFCOutputListCtrlEx	*pWnd = (CMFCOutputListCtrlEx *)GetTabWnd(nNewTab);

	if( pWnd != NULL )
		theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)nNewTab, (LPARAM)pWnd->m_pos);

	return CMFCTabCtrl::FireChangeActiveTab(nNewTab);
}
#endif

void CMFCOutputTabCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMFCTabCtrl::OnLButtonDown(nFlags, point);

	// 該当ビューをアクティブに設定
	CMFCOutputListCtrlEx	*pWnd = (CMFCOutputListCtrlEx *)GetTabWnd(GetActiveTab());
	if( pWnd != NULL )
		theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)GetActiveTab(), (LPARAM)pWnd->m_pos);
}

LRESULT COutputWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

			if( prop->isRealTime )
				UpdateDataEx((UINT)wParam);
		}
		break;

	default:
		return CDockablePane::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}
