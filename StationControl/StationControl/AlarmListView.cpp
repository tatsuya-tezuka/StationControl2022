// AlarmListView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "AlarmListView.h"


// CAlarmListView
CCriticalSection	CAlarmListView::mSection;

IMPLEMENT_DYNCREATE(CAlarmListView, CView)

CAlarmListView::CAlarmListView()
{

}

CAlarmListView::~CAlarmListView()
{
}

CAlarmListView*	CAlarmListView::m_pInstance = NULL;
CAlarmListView*	CAlarmListView::Instance()
{
	if (m_pInstance == NULL){
		static CAlarmListView mInitializerInstance;
		m_pInstance = &mInitializerInstance;
	}
	return m_pInstance;
}

BEGIN_MESSAGE_MAP(CAlarmListView, CView)
END_MESSAGE_MAP()


// CAlarmListView 描画

void CAlarmListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 描画コードをここに追加してください。
}


// CAlarmListView 診断

#ifdef _DEBUG
void CAlarmListView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CAlarmListView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
CStationControlDoc* CAlarmListView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


//============================================================================
/// @brief	メッセージコールバック関数
/// @param	param	フィルタデータ
/// @return
//============================================================================
void CALLBACK CAlarmListView::messageProc(stFilterData& param, UINT type)
{
	switch (type){
	case	eHistory_Alarm:
		CAlarmListView::Instance()->SetDisplayFilter(param);
		break;
	}
}

// CAlarmListView メッセージ ハンドラー

void CAlarmListView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_pInstance = this;

	// 履歴リストの作成
	CreateAlarmList();

	// タブ型バーの作成
	CRect rectClient;
	GetClientRect(rectClient);
	m_TabBar.Create(CMFCTabCtrl::STYLE_3D, rectClient, this, eAlarmList+1, CMFCTabCtrl::LOCATION_TOP);
	m_AlarmFilterDlg.SetOneStation(true);
	m_AlarmFilterDlg.SetHistoryType(eHistory_Alarm);
	m_AlarmFilterDlg.SetCallback(messageProc, eHistory_Alarm);
	m_AlarmFilterDlg.Create(IDD_DIALOG_FILTER_AL, &m_TabBar);
	m_AlarmFilterDlg.SetCallback(messageProc, eHistory_Alarm);
	m_TabBar.AddBar(&m_AlarmFilterDlg, _T("リアルタイム"));
}

void CAlarmListView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_History.GetSafeHwnd()){
		m_History.SetWindowPos(NULL, -1, CMFCTabCtrlEx::mBarHeight, cx + 3, cy - CMFCTabCtrlEx::mBarHeight, /*SWP_NOMOVE | */SWP_NOACTIVATE | SWP_NOZORDER);
	}
	if (m_TabBar.GetSafeHwnd()){
		m_TabBar.SetWindowPos(NULL, -1, -1, cx, CMFCTabCtrlEx::mBarHeight + 10, /*SWP_NOMOVE | */SWP_NOACTIVATE | SWP_NOZORDER);
		m_TabBar.Resize(cx);
	}
}

/*============================================================================*/
/*! 設備アラーム一覧

-# 履歴リストの作成

@param		rectClient		クライアント領域
@retval

*/
/*============================================================================*/
void CAlarmListView::CreateAlarmList()
{
	// クライアント領域の取得
	CRect rectClient;
	GetClientRect(rectClient);

	m_History.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_SHOWSELALWAYS, rectClient, this, eAlarmList);
	// 拡張スタイルの取得
	DWORD  dwStyle = ListView_GetExtendedListViewStyle(m_History.m_hWnd);
	//  拡張スタイルのセット
	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= LVS_EX_FULLROWSELECT/*|LVS_EX_GRIDLINES*/ | 0x00010000/*LVS_EX_DOUBLEBUFFER*/;
	//  新しい拡張スタイルを設定
	ListView_SetExtendedListViewStyle(m_History.m_hWnd, dwStyle);
	m_History.SetBkColor(RGB(0, 0, 0));
	m_History.InsertColumn(eFilterItemEq_Station, _T("内之浦６４ｍ局"), LVCFMT_CENTER, 100);
	m_History.InsertColumn(eFilterItemEq_Time, _T("9999/99/99 99:99:99  "), LVCFMT_CENTER, 100);
	m_History.InsertColumn(eFilterItemEq_Equipment, _T("設備制御装置９９"), LVCFMT_CENTER, 100);
	m_History.InsertColumn(eFilterItemEq_ImStatus, _T("復旧　"), LVCFMT_CENTER, 100);
	m_History.InsertColumn(eFilterItemEq_Status, _T("POWER ON/OFF　"), LVCFMT_CENTER, 100);
	m_History.InsertColumn(eFilterItemEq_MessageId, _T("９９９９９９"), LVCFMT_LEFT, 100);
	m_History.InsertColumn(eFilterItemEq_Message, _T("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"), LVCFMT_CENTER, 100);
	AutoSizeColumns();
}
/*============================================================================*/
/*! 設備アラーム一覧

-# カラムの自動サイズ調整

@param	col		カラム位置のカラムサイズを自動に調整します
@retval

*/
/*============================================================================*/
void CAlarmListView::AutoSizeColumns(int col /*=-1*/)
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)m_History.GetDlgItem(0);

	// create dummy
	int dummycol = pHeader->GetItemCount();
	m_History.InsertColumn(dummycol, _T("dummy"), LVCFMT_LEFT, 10);

	m_History.SetRedraw(false);
	int mincol = col < 0 ? 0 : col;
	int maxcol = col < 0 ? pHeader->GetItemCount() - 1 : col;
	for (col = mincol; col <= maxcol; col++){
		m_History.SetColumnWidth(col, LVSCW_AUTOSIZE);
		int wc1 = m_History.GetColumnWidth(col);
		m_History.SetColumnWidth(col, LVSCW_AUTOSIZE_USEHEADER);
		int wc2 = m_History.GetColumnWidth(col);
		int wc = MAX(MINCOLWIDTH, MAX(wc1, wc2));
		m_History.SetColumnWidth(col, wc);
	}
	// delete dummy
	m_History.DeleteColumn(dummycol);
	m_History.SetRedraw(true);
}

/*============================================================================*/
/*! 設備アラーム一覧

-# 履歴項目の登録

@param	nItem		アイテム番号
@param	nSubItem	サブアイテム番号
@param	strItem		設定文字列
@param	dwParam		ユーザ値
@retval

*/
/*============================================================================*/
int CAlarmListView::AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam/*=0*/)
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
		return m_History.InsertItem(&lv);
	return m_History.SetItem(&lv);
}

/*============================================================================*/
/*! 設備アラーム一覧

-# 履歴フィルタの設定

@param	data		履歴情報
@retval

*/
/*============================================================================*/
void CAlarmListView::SetDisplayFilter(stFilterData& data)
{
	memcpy(&mFilterData, &data, sizeof(stFilterData));
	CloseChildWindow(NULL);

	CGraphMutex::Lock(eData);
	mSection.Lock();
	m_History.SetRedraw(false);

	m_History.DeleteAllItems();

	deque<stHistoryData>::iterator itr;
	for (itr = mHistoryData.begin(); itr != mHistoryData.end(); itr++){
		// フィルタ表示なので、現在存在するデータから表示するので、第２引数はfalse
		AddHistroy((*itr), false);
	}

	m_History.SetRedraw(true);
	mSection.Unlock();
	CGraphMutex::Unlock(eData);
}

bool CAlarmListView::_GetStation(stHistoryData& hdata, CString cbuf)
{
	CString str, temp;
	AfxExtractSubString(str, cbuf, eAlarmFileType_Station, ',');
	for (int i = 0; i < sizeof(mStationString) / sizeof(mStationString[0]); i++){
		if (str == mStationString[i]){
			hdata.station = (BYTE)i;
			return true;
		}
	}
	return false;
}

bool CAlarmListView::_GetEquipment(stHistoryData& hdata, CString cbuf)
{
	CString str, temp;
	AfxExtractSubString(str, cbuf, eAlarmFileType_Equipment, ',');
	for (int i = 0; i < sizeof(mHistEquipmentString) / sizeof(mHistEquipmentString[0]); i++){
		if (str == mHistEquipmentString[i]){
			hdata.equipment = (BYTE)i;
			return true;
		}
	}
	return false;
}

bool CAlarmListView::_GetImStatus(stHistoryData& hdata, CString cbuf)
{
	CString str, temp;
	AfxExtractSubString(str, cbuf, eAlarmFileType_ImStatus, ',');
	for (int i = 0; i < sizeof(mAlarmImStatusString) / sizeof(mAlarmImStatusString[0]); i++){
		if (str == mAlarmImStatusString[i]){
			hdata.imstatus = (BYTE)i;
			return true;
		}
	}
	return false;
}

bool CAlarmListView::_GetStatus(stHistoryData& hdata, CString cbuf)
{
	CString str, temp;
	AfxExtractSubString(str, cbuf, eAlarmFileType_Status, ',');
	for (int i = 0; i < sizeof(mAlarmStatusString) / sizeof(mAlarmStatusString[0]); i++){
		if (str == mAlarmStatusString[i]){
			hdata.status = (BYTE)i;
			return true;
		}
	}
	return false;
}

/*============================================================================*/
/*! 設備アラーム一覧

-# 有効履歴のチェック

@param	nFilterType		タイプ
@param	val				項目
@retval

*/
/*============================================================================*/
bool CAlarmListView::IsDisplay(int nFilterType, DWORD val, UINT nStation)
{
	switch (nFilterType){
	case eFilterItemEq_Station:
		if (mFilterData.station & (1 << val))
			return true;
		break;
	case eFilterItemEq_Equipment:
		if (mFilterData.sender[nStation] == 0)
			return true;
		if (mFilterData.sender[nStation] & (1 << val))
			return true;
		break;
	case eFilterItemEq_ImStatus:
		if (mFilterData.imstatus & (1 << val))
			return true;
		break;
	case eFilterItemEq_Status:
		if (mFilterData.status & (1 << val))
			return true;
		break;
	default:
		break;
	}
	return false;
}
