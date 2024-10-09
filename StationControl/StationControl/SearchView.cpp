// SearchView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "SearchView.h"

#include "ChildFrm.h"
#include "MainFrm.h"

#include <iostream>
#include <string>
#include <algorithm>

// CSearchView

IMPLEMENT_DYNCREATE(CSearchView, CFormView)

CSearchView::CSearchView()
	: CFormView(CSearchView::IDD)
	, m_strText(_T(""))
{
	m_bOneStation = false;
	for (int i = 0; i < CBarChild::mStationMax+1; i++)
		m_bStation[i] = TRUE;
	for (int i = 0; i < CBarChild::mImStatusKindMax; i++)
		m_bImStatus[i] = TRUE;
	for (int i = 0; i < eOptionType_Max; i++)
		m_bOption[i] = FALSE;

	m_nControlID = eOperationSearch;
	switch (theApp.GetHistorySearchType()){
	case	eHistory_SearchOperation:
		m_nControlID = eOperationSearch;
		break;
	case	eHistory_SearchControl:
		m_nControlID = eControlSearch;
		break;
	case	eHistory_SearchEquipment:
		m_nControlID = eEquipmentSearch;
		break;
	case	eHistory_SearchEvent:
		m_nControlID = eEventSearch;
		break;
	}
}

CSearchView::~CSearchView()
{
}

static bool procSearchSort(const stSearchHistoryData& left, const stSearchHistoryData& right)
{
	CString str1, str2;
	str1.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"), left.year, left.month, left.day, left.hour, left.minute, left.second);
	str2.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"), right.year, right.month, right.day, right.hour, right.minute, right.second);
	if (str1 < str2)
		return true;
	if (str1 > str2)
		return false;
	return left.station < right.station;
}

void CSearchView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TIME, m_grTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_SDATE, m_StartDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_STIME, m_StartTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_PDATE, m_StopDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_PTIME, m_StopTime);
	DDX_Control(pDX, IDC_BUTTON_SENDER, m_btSender[0]);
	DDX_Control(pDX, IDC_BUTTON_SENDER2, m_btSender[1]);
	DDX_Control(pDX, IDC_BUTTON_SENDER3, m_btSender[2]);
	DDX_Control(pDX, IDC_BUTTON_SENDER4, m_btSender[3]);
	DDX_Control(pDX, IDC_STATIC_STATION, m_grStation);
	DDX_Control(pDX, IDC_STATIC_SENDEQ, m_grSendEq);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_grStatus);
	DDX_Control(pDX, IDC_STATIC_MESSAGE, m_grMessage);
	DDX_Control(pDX, IDC_STATIC_HISTORY, m_grOption);
	DDX_Control(pDX, IDC_STATIC_TEXT, m_grText);
	DDX_Check(pDX, IDC_CHECK_STATION1, m_bStation[0]);
	DDX_Check(pDX, IDC_CHECK_STATION2, m_bStation[1]);
	DDX_Check(pDX, IDC_CHECK_STATION3, m_bStation[2]);
	DDX_Check(pDX, IDC_CHECK_STATION4, m_bStation[3]);
	DDX_Check(pDX, IDC_CHECK_STATION5, m_bStation[4]);
	DDX_Check(pDX, IDC_CHECK_SSTATUS1, m_bImStatus[0]);
	DDX_Check(pDX, IDC_CHECK_SSTATUS2, m_bImStatus[1]);
	DDX_Check(pDX, IDC_CHECK_SSTATUS3, m_bImStatus[2]);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_edMessage);
	DDX_Control(pDX, IDC_CHECK_STATION1, m_btStation[0]);
	DDX_Control(pDX, IDC_CHECK_STATION2, m_btStation[1]);
	DDX_Control(pDX, IDC_CHECK_STATION3, m_btStation[2]);
	DDX_Control(pDX, IDC_CHECK_STATION4, m_btStation[3]);
	DDX_Control(pDX, IDC_CHECK_STATION5, m_btStation[4]);
	DDX_Check(pDX, IDC_CHECK_HISTORY1, m_bOption[0]);
	DDX_Check(pDX, IDC_CHECK_HISTORY2, m_bOption[1]);
	DDX_Check(pDX, IDC_CHECK_HISTORY3, m_bOption[2]);
	DDX_Check(pDX, IDC_CHECK_HISTORY4, m_bOption[3]);
	DDX_Control(pDX, IDC_LIST_HISTORY, m_History);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_btNext);
	DDX_Control(pDX, IDC_BUTTON_PREV, m_btPrev);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_strText);
}

BEGIN_MESSAGE_MAP(CSearchView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL, &CSearchView::OnBnClickedButtonSelectall)
	ON_BN_CLICKED(IDC_BUTTON_EXEC, &CSearchView::OnBnClickedButtonExec)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CSearchView::OnBnClickedButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CSearchView::OnBnClickedButtonPrint)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CSearchView::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_PREV, &CSearchView::OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDC_CHECK_STATION1, &CSearchView::OnBnClickedCheckStation1)
	ON_BN_CLICKED(IDC_CHECK_STATION2, &CSearchView::OnBnClickedCheckStation2)
	ON_BN_CLICKED(IDC_CHECK_STATION3, &CSearchView::OnBnClickedCheckStation3)
	ON_BN_CLICKED(IDC_CHECK_STATION4, &CSearchView::OnBnClickedCheckStation4)
	ON_BN_CLICKED(IDC_CHECK_STATION5, &CSearchView::OnBnClickedCheckStation5)
END_MESSAGE_MAP()


// CSearchView 診断

#ifdef _DEBUG
void CSearchView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSearchView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
CStationControlDoc* CSearchView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CSearchView メッセージ ハンドラー


void CSearchView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// 履歴リストの作成
	CreateHistroyList();
	m_nHistoryType = theApp.GetHistorySearchType();

	// ウィンドウ幅を合わせる
	GetParentFrame()->RecalcLayout();
	int	nMapMode;
	GetDeviceScrollSizes(nMapMode, mSizeTotal, mSizePage, mSizeLine);
	ResizeParentToFit(FALSE);

	m_StartDate.SetFormat(_T("yyyy-MM-dd"));
	m_StartTime.SetFormat(_T("HH:mm:ss"));
	m_StopDate.SetFormat(_T("yyyy-MM-dd"));
	m_StopTime.SetFormat(_T("HH:mm:ss"));

	if (theApp.GetSelectStationMode() == eStation_1){
		// １局モード時は局選択は必要ないので削除する
		CRect rect1, rect2;
		GetDlgItem(IDC_STATIC_STATION)->GetWindowRect(rect1);
		ScreenToClient(rect1);
		GetDlgItem(IDC_STATIC_SENDEQ)->GetWindowRect(rect2);
		ScreenToClient(rect2);
		int move = rect2.left - rect1.left;

		if (m_nHistoryType == eHistory_Event || m_nHistoryType == eHistory_SearchEvent){
			// イベント関連の場合は「送信元情報」は必要ない
			GetDlgItem(IDC_STATIC_STATUS)->GetWindowRect(rect2);
			ScreenToClient(rect2);
			move = rect2.left - rect1.left;

			// ※実際には仕様が決まってから行う
			const UINT mHideCtl[] = {
				IDC_STATIC_STATION,
				IDC_CHECK_STATION1,
				IDC_CHECK_STATION2,
				IDC_CHECK_STATION3,
				IDC_CHECK_STATION4,
				IDC_CHECK_STATION5,
				IDC_STATIC_SENDEQ,
				IDC_BUTTON_SENDER,
				IDC_BUTTON_SENDER2,
				IDC_BUTTON_SENDER3,
				IDC_BUTTON_SENDER4,
			};
			const UINT mNomoveCtl[] = {
				IDC_STATIC_TIME,
				IDC_STATIC_START2,
				IDC_STATIC_STOP2,
				IDC_DATETIMEPICKER_SDATE,
				IDC_DATETIMEPICKER_STIME,
				IDC_DATETIMEPICKER_PDATE,
				IDC_DATETIMEPICKER_PTIME,
			};
			CWnd *pwnd = GetWindow(GW_CHILD);
			while (pwnd != NULL){
				bool ret = false;
				for (int i = 0; i < sizeof(mHideCtl) / sizeof(mHideCtl[0]); ++i) {
					if (GetDlgItem(mHideCtl[i]) == pwnd){
						ret = true;
						pwnd->ShowWindow(SW_HIDE);
						break;
					}
				}
				if (ret == false){
					ret = false;
					for (int i = 0; i < sizeof(mNomoveCtl) / sizeof(mNomoveCtl[0]); ++i) {
						if (GetDlgItem(mNomoveCtl[i]) == pwnd){
							ret = true;
							break;
						}
					}
					if (ret == false){
						// 移動
						CRect rc;
						pwnd->GetWindowRect(rc);
						rc.left -= move;
						rc.right -= move;
						ScreenToClient(rc);
						pwnd->MoveWindow(rc);
					}
				}
				pwnd = pwnd->GetWindow(GW_HWNDNEXT);
			}
		}
		else{
			// ※実際には仕様が決まってから行う
			const UINT mHideCtl[] = {
				IDC_STATIC_STATION,
				IDC_CHECK_STATION1,
				IDC_CHECK_STATION2,
				IDC_CHECK_STATION3,
				IDC_CHECK_STATION4,
				IDC_CHECK_STATION5,
				IDC_BUTTON_SENDER2,
				IDC_BUTTON_SENDER3,
				IDC_BUTTON_SENDER4,
			};
			const UINT mNomoveCtl[] = {
				IDC_STATIC_TIME,
				IDC_STATIC_START2,
				IDC_STATIC_STOP2,
				IDC_DATETIMEPICKER_SDATE,
				IDC_DATETIMEPICKER_STIME,
				IDC_DATETIMEPICKER_PDATE,
				IDC_DATETIMEPICKER_PTIME,
			};
			CWnd *pwnd = GetWindow(GW_CHILD);
			while (pwnd != NULL){
				bool ret = false;
				for (int i = 0; i < sizeof(mHideCtl) / sizeof(mHideCtl[0]); ++i) {
					if (GetDlgItem(mHideCtl[i]) == pwnd){
						ret = true;
						pwnd->ShowWindow(SW_HIDE);
						break;
					}
				}
				if (ret == false){
					ret = false;
					for (int i = 0; i < sizeof(mNomoveCtl) / sizeof(mNomoveCtl[0]); ++i) {
						if (GetDlgItem(mNomoveCtl[i]) == pwnd){
							ret = true;
							break;
						}
					}
					if (ret == false){
						// 移動
						CRect rc;
						pwnd->GetWindowRect(rc);
						rc.left -= move;
						rc.right -= move;
						ScreenToClient(rc);
						pwnd->MoveWindow(rc);
					}
				}
				pwnd = pwnd->GetWindow(GW_HWNDNEXT);
			}
		}
	}
	else{
		if (m_nHistoryType == eHistory_Event || m_nHistoryType == eHistory_SearchEvent){
			CRect rect1, rect2;
			GetDlgItem(IDC_STATIC_STATION)->GetWindowRect(rect1);
			ScreenToClient(rect1);
			GetDlgItem(IDC_STATIC_STATUS)->GetWindowRect(rect2);
			ScreenToClient(rect2);
			int move = rect2.left - rect1.left;

			// ※実際には仕様が決まってから行う
			const UINT mHideCtl[] = {
				IDC_STATIC_SENDEQ,
				IDC_BUTTON_SENDER,
				IDC_BUTTON_SENDER2,
				IDC_BUTTON_SENDER3,
				IDC_BUTTON_SENDER4,
			};
			const UINT mNomoveCtl[] = {
				IDC_STATIC_TIME,
				IDC_STATIC_START2,
				IDC_STATIC_STOP2,
				IDC_DATETIMEPICKER_SDATE,
				IDC_DATETIMEPICKER_STIME,
				IDC_DATETIMEPICKER_PDATE,
				IDC_DATETIMEPICKER_PTIME,
				IDC_STATIC_STATION,
				IDC_CHECK_STATION1,
				IDC_CHECK_STATION2,
				IDC_CHECK_STATION3,
				IDC_CHECK_STATION4,
				IDC_CHECK_STATION5,
			};
			CWnd *pwnd = GetWindow(GW_CHILD);
			while (pwnd != NULL){
				bool ret = false;
				for (int i = 0; i < sizeof(mHideCtl) / sizeof(mHideCtl[0]); ++i) {
					if (GetDlgItem(mHideCtl[i]) == pwnd){
						ret = true;
						pwnd->ShowWindow(SW_HIDE);
						break;
					}
				}
				if (ret == false){
					ret = false;
					for (int i = 0; i < sizeof(mNomoveCtl) / sizeof(mNomoveCtl[0]); ++i) {
						if (GetDlgItem(mNomoveCtl[i]) == pwnd){
							ret = true;
							break;
						}
					}
					if (ret == false){
						// 移動
						CRect rc;
						pwnd->GetWindowRect(rc);
						rc.left -= move;
						rc.right -= move;
						ScreenToClient(rc);
						pwnd->MoveWindow(rc);
					}
				}
				pwnd = pwnd->GetWindow(GW_HWNDNEXT);
			}
		}
		if (m_nHistoryType != eHistory_SearchOperation){
			CWnd *pwnd = GetWindow(GW_CHILD);
			while (pwnd != NULL){
				if (GetDlgItem(IDC_CHECK_STATION5) == pwnd){
					pwnd->ShowWindow(SW_HIDE);
				}
				pwnd = pwnd->GetWindow(GW_HWNDNEXT);
			}
		}
	}

	// 送信元コントロールの作成
	for (int i = 0; i < CBarChild::mStationMax; i++)
		m_btSender[i].Create();

	m_btStation[0].SetStation(eStation_Usuda64);
	m_btStation[1].SetStation(eStation_Uchinoura34);
	m_btStation[2].SetStation(eStation_Uchinoura20);
	m_btStation[3].SetStation(eStation_Usuda642);
	m_btStation[4].SetStation(eStation_MAX);

	HICON icon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICONBT_NEXT), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	m_btNext.SetIcon(icon);
	icon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICONBT_PREV), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	m_btPrev.SetIcon(icon);

	const TCHAR* mSearchTitle[]{
		_T("運用履歴検索"),
		_T("制御履歴検索"),
		_T("設備状態履歴検索"),
		_T("イベント履歴検索"),
	};

	m_strTitle.Empty();
	switch (m_nHistoryType){
	case	eHistory_SearchOperation:
		m_bOption[0] = TRUE;
		GetDlgItem(IDC_CHECK_HISTORY1)->EnableWindow(FALSE);
		GetDocument()->SetTitleEx(mSearchTitle[0]);
		m_strTitle = mSearchTitle[0];
		break;
	case	eHistory_SearchControl:
		m_bOption[1] = TRUE;
		GetDlgItem(IDC_CHECK_HISTORY2)->EnableWindow(FALSE);
		GetDocument()->SetTitleEx(mSearchTitle[1]);
		m_strTitle = mSearchTitle[1];
		break;
	case	eHistory_SearchEquipment:
		m_bOption[2] = TRUE;
		GetDlgItem(IDC_CHECK_HISTORY3)->EnableWindow(FALSE);
		GetDocument()->SetTitleEx(mSearchTitle[2]);
		m_strTitle = mSearchTitle[2];
		break;
	case	eHistory_SearchEvent:
		m_bOption[3] = TRUE;
		GetDlgItem(IDC_CHECK_HISTORY4)->EnableWindow(FALSE);
		GetDocument()->SetTitleEx(mSearchTitle[3]);
		m_strTitle = mSearchTitle[3];
		break;
	}

	if (m_History.GetItemCount() > 0)
	{
		GetDlgItem(IDC_BUTTON_FILE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PRINT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_FILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PRINT)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);
}

/*============================================================================*/
/*! 運用履歴

-# 履歴リストの作成

@param		rectClient		クライアント領域
@retval

*/
/*============================================================================*/
void CSearchView::CreateHistroyList()
{
	// クライアント領域の取得
	CRect rectClient;
	GetClientRect(rectClient);

	DWORD  dwStyle = ListView_GetExtendedListViewStyle(m_History.m_hWnd);
	//  拡張スタイルのセット
	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= LVS_EX_FULLROWSELECT/*|LVS_EX_GRIDLINES*/ | 0x00010000/*LVS_EX_DOUBLEBUFFER*/;
	//  新しい拡張スタイルを設定
	ListView_SetExtendedListViewStyle(m_History.m_hWnd, dwStyle);
	m_History.SetBkColor(RGB(0, 0, 0));
	if (m_nControlID == eEquipmentSearch){
		m_History.InsertColumn(eSearchitem_Station, _T("局名          "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(eSearchitem_Time, _T("日時                      "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(eSearchitem_Equipment, _T("送信元       "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(eSearchitem_ImStatus, _T("重要度"), LVCFMT_CENTER, 100);
		m_History.InsertColumn(eSearchitem_Status, _T("ステータス  "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(eSearchitem_MessageId, _T("番号"), LVCFMT_LEFT, 100);
		m_History.InsertColumn(eSearchitem_Message, _T("メッセージ"), LVCFMT_LEFT, 100);
	}
	else if (m_nControlID == eEventSearch){
		m_History.InsertColumn(eSearchitem_Station, _T("局名          "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(eSearchitem_Time, _T("日時                      "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(eSearchitem_Equipment, _T("送信元       "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(eSearchitem_ImStatus, _T("重要度"), LVCFMT_CENTER, 100);
		m_History.InsertColumn(eSearchitem_Status, _T("ステータス  "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(eSearchitem_MessageId, _T("番号"), LVCFMT_LEFT, 100);
		m_History.InsertColumn(eSearchitem_Message, _T("メッセージ"), LVCFMT_LEFT, 100);
	}
	else{
		m_History.InsertColumn(eSearchitem_Station, _T("局名          "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(eSearchitem_Time, _T("日時                      "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(eSearchitem_Equipment, _T("送信元           "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(eSearchitem_ImStatus, _T("重要度"), LVCFMT_CENTER, 100);
		m_History.InsertColumn(eSearchitem_Status, _T("ステータス  "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(eSearchitem_MessageId, _T("番号"), LVCFMT_LEFT, 100);
		m_History.InsertColumn(eSearchitem_Message, _T("メッセージ"), LVCFMT_LEFT, 100);
	}
	AutoSizeColumns();
}
/*============================================================================*/
/*! 運用履歴

-# カラムの自動サイズ調整

@param	col		カラム位置のカラムサイズを自動に調整します
@retval

*/
/*============================================================================*/
void CSearchView::AutoSizeColumns(int col /*=-1*/)
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)m_History.GetDlgItem(0);

	// create dummy
	//int dummycol = pHeader->GetItemCount();
	//m_History.InsertColumn(dummycol, _T("dummy"), LVCFMT_LEFT, 10);

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
	//m_History.DeleteColumn(dummycol);
	m_History.SetRedraw(true);
}

/*============================================================================*/
/*! 運用履歴

-# 履歴項目の登録

@param	nItem		アイテム番号
@param	nSubItem	サブアイテム番号
@param	strItem		設定文字列
@param	dwParam		ユーザ値
@retval

*/
/*============================================================================*/
int CSearchView::AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam/*=0*/)
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

void CSearchView::OnBnClickedButtonSelectall()
{
	UpdateData(TRUE);
	for (int i = 0; i < CBarChild::mStationMax; i++)
		m_nSender[i] = 0;
	for (int i = 0; i < CBarChild::mStationMax+1; i++)
		m_bStation[i] = TRUE;
	for (int i = 0; i < CBarChild::mImStatusKindMax; i++)
		m_bImStatus[i] = TRUE;
	for (int i = 0; i < eOptionType_Max; i++)
		m_bOption[i] = TRUE;

	m_edMessage.SetSel(0, -1);
	m_edMessage.Clear();
	for (int i = 0; i < CBarChild::mStationMax; i++)
		m_btSender[i].SetStatus(0);

	for (int i = 0; i < CBarChild::mStationMax; i++)
		m_btSender[i].EnableWindow(m_bStation[i]);

	UpdateData(FALSE);
}


void CSearchView::OnBnClickedButtonExec()
{
	UpdateData(TRUE);

	// 検索開始時刻、終了時刻チェック
	SYSTEMTIME date, time;
	m_StartDate.GetTime(&date);
	m_StartTime.GetTime(&time);
	CTime timeStart = CTime(date.wYear, date.wMonth, date.wDay, time.wHour, time.wMinute, time.wSecond);
	m_StopDate.GetTime(&date);
	m_StopTime.GetTime(&time);
	CTime timeStop = CTime(date.wYear, date.wMonth, date.wDay, time.wHour, time.wMinute, time.wSecond);
	if (timeStart > timeStop)
	{
		CString mes = _T("");
		CString strStartTime = timeStart.Format(_T("%Y-%m-%d %H:%M:%S"));
		CString strEndTime = timeStop.Format(_T("%Y-%m-%d %H:%M:%S"));
		mes.Format(_T("終了時刻が開始時刻よりも過去のため、検索できません。\n\n 開始時刻 :%s\n 終了時刻 :%s"), (LPCTSTR)strStartTime, (LPCTSTR)strEndTime);
		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
		return;
	}

	m_SearchData.filter = false;
	m_SearchData.station = 0;
	//for (int i = 0; i < CBarChild::mStationMax; i++){
	//	m_SearchData.station |= (m_bStation[i] == TRUE) ? (1 << i) : (0 << i);
	//}
	m_SearchData.station = 0;
	if (m_bStation[0] == TRUE)	m_SearchData.station |= 0x01;
	if (m_bStation[1] == TRUE)	m_SearchData.station |= 0x04;
	if (m_bStation[2] == TRUE)	m_SearchData.station |= 0x08;
	if (m_bStation[3] == TRUE)	m_SearchData.station |= 0x02;
	if (m_bStation[4] == TRUE)	m_SearchData.station |= 0x10;
	if (theApp.GetSelectStationMode() == eStation_1){
		m_SearchData.station = (1 << theApp.GetSelectStation());
	}
	m_SearchData.imstatus = 0;
	for (int i = 0; i < CBarChild::mImStatusKindMax; i++){
		m_SearchData.imstatus |= (m_bImStatus[i] == TRUE) ? (1 << i) : (0 << i);
	}
	m_SearchData.status = 0xff;
	if (theApp.GetSelectStationMode() == eStation_1){
		m_SearchData.sender[theApp.GetSelectStation()] = m_btSender[0].GetStatus();
	}
	else{
		m_SearchData.sender[0] = m_btSender[0].GetStatus();
		m_SearchData.sender[2] = m_btSender[1].GetStatus();
		m_SearchData.sender[3] = m_btSender[2].GetStatus();
		m_SearchData.sender[1] = m_btSender[3].GetStatus();
		//for (int i = 0; i < CBarChild::mStationMax; i++){
		//	m_SearchData.sender[i] = m_btSender[i].GetStatus();
		//}
	}
	CString str;
	m_edMessage.GetWindowText(str);
	if (str.IsEmpty() == true)
		m_SearchData.message = 0;
	else
		m_SearchData.message = _wtoi(str);

	m_StartDate.GetTime(&date);
	m_StartTime.GetTime(&time);
	m_SearchData.stime.wYear = date.wYear;
	m_SearchData.stime.wMonth = date.wMonth;
	m_SearchData.stime.wDay = date.wDay;
	m_SearchData.stime.wHour = time.wHour;
	m_SearchData.stime.wMinute = time.wMinute;
	m_SearchData.stime.wSecond = time.wSecond;
	m_StopDate.GetTime(&date);
	m_StopTime.GetTime(&time);
	m_SearchData.ptime.wYear = date.wYear;
	m_SearchData.ptime.wMonth = date.wMonth;
	m_SearchData.ptime.wDay = date.wDay;
	m_SearchData.ptime.wHour = time.wHour;
	m_SearchData.ptime.wMinute = time.wMinute;
	m_SearchData.ptime.wSecond = time.wSecond;
	m_SearchData.searchtype = 0;
	for (int i = 0; i < eOptionType_Max; i++){
		m_SearchData.searchtype |= (m_bOption[i] << i);
	}

	SetDisplaySearch(m_nHistoryType, m_SearchData);
}

void CSearchView::OnBnClickedButtonFile()
{
	m_History.CsvOutput(m_strTitle);
}

void CSearchView::OnBnClickedButtonPrint()
{
	CString strFilter=_T("");
	m_History.PrintOutput(m_strTitle, strFilter, m_nHistoryType, m_SearchData);
}

/*============================================================================*/
/*! 運用履歴

-# 履歴検索の設定

@param	data		履歴情報
@retval

*/
/*============================================================================*/
void CSearchView::SetDisplaySearch(int nFilterType, stFilterData& data)
{
	CWaitCursor wait;

	//// 対象チェック
	//switch (nFilterType){
	//case	eHistory_SearchOperation:
	//case	eHistory_SearchControl:
	//case	eHistory_SearchEquipment:
	//case	eHistory_SearchEvent:
	//case	eHistory_SearchAlarm:
	//	break;
	//default:
	//	return;
	//}

	m_History.SetRedraw(false);
	// 一旦検索結果をクリアする
	m_History.DeleteAllItems();
	mHistoryData.clear();

	UINT nStation = theApp.GetSelectStation();

	FILE *fp = NULL;
	CString str, path, fname;
	vector<stSearchHistoryData> datatemp;

	CTime stime, ptime, time;
	CTime start, stop;
	CTimeSpan sp;
	stime = CTime(data.stime);
	ptime = CTime(data.ptime);
	start = __min(stime, ptime);
	stop = __max(stime, ptime);

	stime = CTime(start.GetYear(), start.GetMonth(), start.GetDay(), 0, 0, 0);
	ptime = CTime(stop.GetYear(), stop.GetMonth(), stop.GetDay(), 0, 0, 0);

	sp = ptime - stime;
	time = start;
	TRACE("### Days=%d\n", sp.GetDays());
	datatemp.clear();
	for (UINT i = 0; i <= sp.GetDays(); i++, time += CTimeSpan(1, 0, 0, 0)){
		// 「筑波局運用管制装置」を考慮して局＋１繰り返す
		for (int st = 0; st < eStation_MAX+1; st++){
			if (st != eStation_MAX){
				if (theApp.GetSelectStationMode() == eStation_1){
					if (st != nStation)
						continue;
				}
				else{
					if (!(data.station & (1 << st)))
						continue;
					nStation = st;
				}
			}
			for (int i = 0; i < eOptionType_Max; i++){
				int nType = 0;
				if (data.searchtype&(1 << i)){
					switch (i){
					case	0:
						nType = eHistory_SearchOperation;
						break;
					case	1:
						nType = eHistory_SearchControl;
						break;
					case	2:
						nType = eHistory_SearchEquipment;
						break;
					case	3:
						nType = eHistory_SearchEvent;
						break;
					default:
						continue;
					}
				}

				// 対象ファイル名取得
				if (nType != eHistory_SearchOperation && st == eStation_MAX)
					continue;
				switch (nType){
				case	eHistory_SearchOperation:
					// 「筑波局運用管制装置」を考慮して局＋１繰り返す
					if (st == eStation_MAX){
						CString strFilePath;
						strFilePath.Format(_T("\\\\%sshare\\log\\opr\\"), (LPCTSTR)theApp.GetFileServerName());
						if (theApp.GetFileServerName().IsEmpty() == true){
							strFilePath.Format(_T("C:\\share\\log\\opr\\"));
						}
						path = strFilePath;
						fname.Format(_T("%s%4d%02d%02d"), (LPCTSTR)path, time.GetYear(), time.GetMonth(), time.GetDay());
					}
					else{
						path = theApp.GetShareFilePath(eFileType_OpHistory_Folder, nStation);
						fname.Format(_T("%s%4d%02d%02d"), (LPCTSTR)path, time.GetYear(), time.GetMonth(), time.GetDay());
					}
					fname += _T(".opl");
					break;
				case	eHistory_SearchControl:
					path = theApp.GetShareFilePath(eFileType_CtrlHistory_Folder, nStation);
					fname.Format(_T("%s%4d%02d%02d"), (LPCTSTR)path, time.GetYear(), time.GetMonth(), time.GetDay());
					fname += _T(".ctrl");
					break;
				case	eHistory_SearchEquipment:
					path = theApp.GetShareFilePath(eFileType_EquipHistory_Folder, nStation);
					fname.Format(_T("%s%4d%02d%02d"), (LPCTSTR)path, time.GetYear(), time.GetMonth(), time.GetDay());
					fname += _T(".moni");
					break;
				case	eHistory_SearchEvent:
					//path = theApp.GetShareFilePath(eFileType_EventHistory_Folder, nStation);
					//fname.Format(_T("%s%4d%02d%02d"), (LPCTSTR)path, time.GetYear(), time.GetMonth(), time.GetDay());
					//fname += _T(".event");
					path = theApp.GetShareFilePath(eFileType_OpHistory_Folder, nStation);
					fname.Format(_T("%s%4d%02d%02d"), (LPCTSTR)path, time.GetYear(), time.GetMonth(), time.GetDay());
					fname += _T(".opl");
					break;
				case	eHistory_SearchLimit:
				case	eHistory_SearchAlarm:
				default:
					continue;
				}

				TRACE("### Fname=%s\n", CStringA(fname));
				// 対象ファイルをオープンする
				if (_wfopen_s(&fp, fname, _T("r")) != 0){
					continue;
				}

				TRY
				{
					char szline[2048];
					CString cbuf;
					while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL){
						cbuf = CServerFile::ConvertUnicode(szline);
						if (cbuf.Find(_T("#!HEAD")) >= 0)
							continue;
						stSearchHistoryData hdata;
						if (_GetDate(hdata, cbuf) == false)
							continue;
						if (_GetTime(hdata, cbuf) == false)
							continue;
						if (theApp.GetSelectStationMode() == eStation_5){
							hdata.station = nStation;
						}
						else{
							if (_GetStation(hdata, cbuf) == false)
								continue;
						}
						if (_GetEquipment(hdata, cbuf) == false)
							continue;
						if (_GetImStatus(hdata, cbuf) == false)
							continue;
						if (nType == eHistory_SearchEquipment){
							if (_GetStatus(hdata, cbuf) == false)
								continue;
						}
						else{
							hdata.status = 0x00;
						}
						if (_GetMessageID(hdata, cbuf) == false)
							continue;
						if (_GetMessageString(hdata, cbuf) == false)
							continue;

						// イベント履歴の場合は運用履歴内のメッセージ文字列よりフィルター文字列をチェックする
						if (nType == eHistory_SearchEvent){
							if (IsMessageEvent(hdata.msgstring) == false)
								continue;
						}

						CTime dtime = CTime(hdata.year, hdata.month, hdata.day, hdata.hour, hdata.minute, hdata.second);
						if (dtime < start || dtime > stop)
							continue;

						// ここまできたら、開始、終了間のデータを取得した
						hdata.searchtype = nType;
						if (datatemp.size() == MAX_HISTORYSEARCH_DATA)
						{
							CString strMsg;
							strMsg.Format(_T("検索件数が%d件を超えました。"), MAX_HISTORYSEARCH_DATA);
							AfxMessageBox((LPCTSTR)strMsg, MB_OK | MB_ICONINFORMATION);
							if (fp != NULL)
								fclose(fp);
							goto SEARCH_END_LABEL;
						}
						datatemp.push_back(hdata);
					}
				}CATCH_ALL(e){
				}
				END_CATCH_ALL

				if (fp != NULL)
					fclose(fp);
			}
		}
	}

SEARCH_END_LABEL:

	// ここで検索フィルタのデータを適用する
	vector<stSearchHistoryData>::iterator itr;
	for (itr = datatemp.begin(); itr != datatemp.end(); itr++){

		// ４局画面でかつ送信元が「筑波局運用管制装置」の場合の特別処理
		if (theApp.GetSelectStationMode() == eStation_5 && (*itr).equipment == eSenderItemMax){
			if (!(data.station & 0x10))
				continue;
		}
		// 上記以外の局フィルタ処理
		else
		{
			if (!(data.station & (1 << (*itr).station)))
				continue;
			nStation = (*itr).station;

			if (data.sender[nStation] != 0){
				// 送信元が「筑波局運用管制装置」以外の処理
				if ((*itr).equipment != eSenderItemMax){
					if (!(data.sender[nStation] & (1 << (*itr).equipment)))
						continue;
				}
			}
		}

		if (!(data.imstatus & (1 << (*itr).imstatus)))
			continue;
		if (!(data.status & (1 << (*itr).status)))
			continue;
		if (data.message != 0){
			if (data.message != (*itr).message)
				continue;
		}
//#ifdef _DEBUG
//		if (theApp.GetSelectStationMode() == eStation_5){
//			if ((BYTE)(rand() % (eSenderItemMax + 1)) == eSenderItemMax){
//				(*itr).equipment = eSenderItemMax;
//				if (!(data.station & (1 << eStation_MAX)))
//					continue;
//			}
//		}
//#endif
		mHistoryData.push_back((*itr));
	}

	mHistoryData.sort(procSearchSort);

	int item=0, subitem=0;
	CHeaderCtrl* pHeader = (CHeaderCtrl*)m_History.GetDlgItem(0);
	int maxcol = pHeader->GetItemCount();

	list<stSearchHistoryData>::iterator itrh;
	for (itrh = mHistoryData.begin(); itrh != mHistoryData.end(); itrh++){
		for (int col = 0; col <= maxcol; col++){
			subitem = col;
			CString str;
			switch (col){
			case eSearchitem_Station:
				str.Format(_T("%s"), mStationString[(*itrh).station]);
				// 送信元が「筑波局運用管制装置」の場合の特別処理
				if ((*itrh).equipment == eSenderItemMax){
					str.Format(_T("%s"), (LPCTSTR)mStationStringEx);
				}
				break;
			case eSearchitem_Time:
				str.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"), (*itrh).year, (*itrh).month, (*itrh).day, (*itrh).hour, (*itrh).minute, (*itrh).second);
				break;
			case eSearchitem_Equipment:
				if ((*itrh).equipment == eSenderItemMax || (*itrh).searchtype == eHistory_SearchEvent)
					str.Format(_T(""));
				else
					str.Format(_T("%s"), (LPCTSTR)mHistEquipmentString[(*itrh).equipment]);
				break;
			case eSearchitem_ImStatus:
				if ((*itrh).searchtype != eHistory_SearchEquipment)
					str.Format(_T("%s"), (LPCTSTR)mHistStatusString[(*itrh).imstatus]);
				else
					str.Format(_T("%s"), (LPCTSTR)mHistImStatusString[(*itrh).imstatus]);
				break;
			case eSearchitem_Status:
				if ((*itrh).searchtype != eHistory_SearchEquipment)
					str.Format(_T(""));
				else
					str.Format(_T("%s"), (LPCTSTR)mEqHistStatusString[(*itrh).status]);
				break;
			case eSearchitem_MessageId:
				str.Format(_T("%6d"), (*itrh).message);
				break;
			case eSearchitem_Message:
				str.Format(_T("%s"), (*itrh).msgstring);
				break;
			default:
				continue;
			}
			if ((*itrh).searchtype == eHistory_SearchEquipment)
				AddItem(item, subitem, str, 0x80000000 | (*itrh).imstatus);
			else
				AddItem(item, subitem, str, (*itrh).imstatus);
		}
		item++;
	}

	AutoSizeColumns();

	m_History.SetRedraw(true);

	if (m_History.GetItemCount() > 0)
	{
		GetDlgItem(IDC_BUTTON_FILE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PRINT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_FILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PRINT)->EnableWindow(FALSE);
	}
}

/*============================================================================*/
/*! データ収集

-# メッセージ内容からイベント履歴かのチェックを行う

@param	pmsg	メッセージ文字列

@retval bool
*/
/*============================================================================*/
bool CSearchView::IsMessageEvent(TCHAR* pmsg)
{
	const CString mMessageEvent[] = {
		{ _T("[イベント実行]") },
		{ _T("[シーケンス管理]") },
		{ _T("[TLM運用]") },
		{ _T("[CMD運用]") },
		{ _T("[RNG運用]") },
		{ _T("[変調制御管理]") },
		{ _T("[UPLINK運用]") },
		{ _T("[制御出力サブ]") },
		{ _T("[S-DL捕捉]") },
		{ _T("[X-DL捕捉]") },
		{ _T("[Ka-DL捕捉]") },
	};
	CString msg = CString(pmsg);
	for (int i = 0; i < sizeof(mMessageEvent) / sizeof(mMessageEvent[0]); i++){
		if (msg.Find(mMessageEvent[i]) >= 0){
			// イベント履歴の文字列が見つかった
			return true;
		}
	}
	return false;
}

bool CSearchView::_GetDate(stSearchHistoryData& hdata, CString cbuf)
{
	CString str, temp;
	AfxExtractSubString(str, cbuf, eSearchFileType_Date, ' ');
	int i = 0;
	while (AfxExtractSubString(temp, str, i, '-')){
		switch (i){
		case	0: hdata.year = _wtoi(temp); break;
		case	1: hdata.month = _wtoi(temp); break;
		case	2: hdata.day = _wtoi(temp); break;
		default:
			return false;
		}
		i++;
	}
	return true;
}

bool CSearchView::_GetTime(stSearchHistoryData& hdata, CString cbuf)
{
	CString str, temp;
	AfxExtractSubString(str, cbuf, eSearchFileType_Time, ' ');
	int i = 0;
	while (AfxExtractSubString(temp, str, i, ':')){
		switch (i){
		case	0: hdata.hour = _wtoi(temp); break;
		case	1: hdata.minute = _wtoi(temp); break;
		case	2: hdata.second = _wtoi(temp); break;
		default:
			return false;
		}
		i++;
	}
	return true;
}

bool CSearchView::_GetStation(stSearchHistoryData& hdata, CString cbuf)
{
	UINT nStation = theApp.GetSelectStation();
	hdata.station = nStation;
	return true;

	//CString str, temp;
	//AfxExtractSubString(str, cbuf, eFileType_Station, ',');
	//for (int i = 0; i < sizeof(mStationString) / sizeof(mStationString[0]); i++){
	//	if (str == mStationString[i]){
	//		hdata.station = (BYTE)i;
	//		return true;
	//	}
	//}
	//return false;
}

bool CSearchView::_GetEquipment(stSearchHistoryData& hdata, CString cbuf)
{
	// 送信元
	typedef struct {
		CString		name;
		UINT		code;
	} stSender;
	const stSender mSender[] = {
		{ _T("e1"), eSenderItem1 },	// 出先局側局運用管制装置
		{ _T("e2"), eSenderItem1 },	// 相模原側局運用管制装置
		{ _T("e3"), eSenderItemMax },	// 筑波局運用管制装置
		{ _T("c0"), eSenderItem2 },	// 設備制御装置
		{ _T("85"), eSenderItem3 },	// S帯TLM入力装置
		{ _T("86"), eSenderItem3 },	// X帯TLM入力装置
		{ _T("87"), eSenderItem4 },	// S帯コマンド出力装置
	};

	CString str, temp;
	AfxExtractSubString(str, cbuf, eSearchFileType_Equipment, ' ');
	for (int i = 0; i < sizeof(mSender) / sizeof(mSender[0]); i++){
		if (str == mSender[i].name){
			hdata.equipment = (BYTE)mSender[i].code;
			return true;
		}
	}
	return false;
}

bool CSearchView::_GetImStatus(stSearchHistoryData& hdata, CString cbuf)
{
	// 送信元
	typedef struct {
		CString		name;
		UINT		code;
	} stImStatus;
	const stImStatus mStatus[] = {
		{ _T("di"), eHistStatus_Normal },	// 情報
		{ _T("dw"), eHistStatus_Warning },	// 警告
		{ _T("ds"), eHistStatus_Fault },	// 異常
		{ _T("dg"), eHistStatus_Normal },	// 情報
	};
	CString str, temp;
	AfxExtractSubString(str, cbuf, eSearchFileType_Status, ' ');
	for (int i = 0; i < sizeof(mStatus) / sizeof(mStatus[0]); i++){
		if (str == mStatus[i].name){
			hdata.imstatus = (BYTE)mStatus[i].code;
			return true;
		}
	}
	return false;
}

bool CSearchView::_GetStatus(stSearchHistoryData& hdata, CString cbuf)
{
	// 送信元
	typedef struct {
		CString		name;
		UINT		code;
	} stStatus;
	const stStatus mStatus[] = {
		{ _T("STATUS"), eEqHistStatus_Status },
		{ _T("WARNING"), eEqHistStatus_Warning },
		{ _T("FAULT"), eEqHistStatus_Fault },
		{ _T("REMOTE/LOCAL"), eEqHistStatus_RemoteLocal },
		{ _T("POWER ON/OFF"), eEqHistStatus_PowerOnOff },
	};
	CString str;
	stSearchHistoryData temp;
	_GetMessageString(temp, cbuf);
	str = temp.msgstring;
	for (int i = 0; i < sizeof(mStatus) / sizeof(mStatus[0]); i++){
		if (str.Find(mStatus[i].name) >= 0){
			hdata.status = (BYTE)mStatus[i].code;
			return true;
		}
	}
	return false;
}

bool CSearchView::_GetMessageID(stSearchHistoryData& hdata, CString cbuf)
{
	CString str, temp;
	AfxExtractSubString(temp, cbuf, eSearchFileType_MessageID, ' ');
	str = temp.Mid(1, temp.GetLength() - 1);
	hdata.message = _wtoi(str);
	return true;
}

bool CSearchView::_GetMessageString(stSearchHistoryData& hdata, CString cbuf)
{
	CStringA org = CStringA(cbuf);
	char *ptr = (char*)(const char*)org;
	CString str;
	CStringA temp;

	UINT sepcount = 0;
	temp.Empty();
	while (1){
		if (sepcount >= eSearchFileType_MessageString){
			// ここにきたらメッセージ本文の先頭
			if (*ptr == 0x0A)
				break;
			temp += CStringA(char(*ptr));
		}
		else{
			if (*ptr == 0x20)
				sepcount++;
		}
		ptr++;
	}

	//AfxExtractSubString(str, cbuf, eSearchFileType_MessageString, 0x0A);
	str = CString(temp);
	wsprintf(hdata.msgstring, _T("%s"), (LPCTSTR)str);
	return true;
}

void CSearchView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if (m_History.GetSafeHwnd()){
		CRect rectCtrl;
		m_History.GetWindowRect(&rectCtrl);
		ScreenToClient(rectCtrl);
		CRect rect;
		GetClientRect( &rect );
		//rect.left = rectCtrl.left;
		rect.top = rectCtrl.top;
		m_History.MoveWindow(&rect);
	}
}


BOOL CSearchView::OnEraseBkgnd(CDC* pDC)
{
	return CFormView::OnEraseBkgnd(pDC);
}


void CSearchView::OnBnClickedButtonNext()
{
	UpdateData(TRUE);
	CString text = m_strText;
	text.MakeLower();
	TCHAR* pwild = (TCHAR*)(const TCHAR*)text;

	// 現在選択されているアイテム番号を取得する
	POSITION pos = m_History.GetFirstSelectedItemPosition();
	int nItem = 0;
	if (pos != NULL)
		nItem = m_History.GetNextSelectedItem(pos) + 1;

	CString str;
	int nColumnCount = m_History.GetHeaderCtrl()->GetItemCount();
	for (; nItem<m_History.GetItemCount(); nItem++){
		for (int i = 0; i<nColumnCount; i++){
			str = m_History.GetItemText(nItem, i);
			str.MakeLower();
			if (text.Find(_T("*")) < 0){
				if (str.Find(text) >= 0){
					m_History.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
					m_History.EnsureVisible(nItem, FALSE);
					return;
				}
			}
			else{
				TCHAR* pstr = (TCHAR*)(const TCHAR*)str;
				if (WildCard(pwild, pstr) == true){
					// 見つかった
					m_History.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
					m_History.EnsureVisible(nItem, FALSE);
					return;
				}
			}
		}
	}
	MessageBox(_T("指定文字列は見つかりませんでした"), MB_OK);
}

void CSearchView::OnBnClickedButtonPrev()
{
	UpdateData(TRUE);
	CString text = m_strText;
	text.MakeLower();
	TCHAR* pwild = (TCHAR*)(const TCHAR*)text;

	// 現在選択されているアイテム番号を取得する
	POSITION pos = m_History.GetFirstSelectedItemPosition();
	int nItem = 0;
	if (pos != NULL)
		nItem = m_History.GetNextSelectedItem(pos) - 1;

	CString str;
	int nColumnCount = m_History.GetHeaderCtrl()->GetItemCount();
	//for (; nItem<m_History.GetItemCount(); nItem++){
	for (; nItem>=0; nItem--){
		for (int i = 0; i<nColumnCount; i++){
			str = m_History.GetItemText(nItem, i);
			str.MakeLower();
			if (text.Find(_T("*")) < 0){
				if (str.Find(text) >= 0){
					m_History.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
					m_History.EnsureVisible(nItem, FALSE);
					return;
				}
			}
			else{
				TCHAR* pstr = (TCHAR*)(const TCHAR*)str;
				if (WildCard(pwild, pstr) == true){
					// 見つかった
					m_History.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
					m_History.EnsureVisible(nItem, FALSE);
					return;
				}
			}
		}
	}
	MessageBox(_T("指定文字列は見つかりませんでした"), MB_OK);
}

/*============================================================================*/
/*! CMonSearchDlg

-# リスト更新処理（ワイルドカード検索）

@param  なし
@retval なし
*/
/*============================================================================*/
bool CSearchView::WildCard(TCHAR* pwild, TCHAR* pstr)
{
	enum State {
		Exact,
		AnyRepeat,
	};

	const TCHAR *s = pstr;
	const TCHAR *w = pwild;
	const TCHAR *q = 0;
	int state = 0;

	bool match = true;
	while (match && *w)
	{
		if (*w == '*')
		{
			state = AnyRepeat;
			q = w + 1;
		}
		else
			state = Exact;

		if (*s == 0)
			break;

		switch (state)
		{
		case Exact:
			match = *s == *w;
			s++;
			w++;
			break;

		case AnyRepeat:
			match = true;
			s++;

			if (*s == *q)
				w++;
			break;
		}
	}

	if (state == AnyRepeat)
		return (*s == *q);
	else
		return match && (*s == *w);
}

void CSearchView::OnBnClickedCheckStation1()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON_SENDER)->EnableWindow(m_bStation[0]);
}
void CSearchView::OnBnClickedCheckStation2()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON_SENDER2)->EnableWindow(m_bStation[1]);
}
void CSearchView::OnBnClickedCheckStation3()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON_SENDER3)->EnableWindow(m_bStation[2]);
}
void CSearchView::OnBnClickedCheckStation4()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON_SENDER4)->EnableWindow(m_bStation[3]);
}
void CSearchView::OnBnClickedCheckStation5()
{
	UpdateData(TRUE);
}
