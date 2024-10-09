// HistroyView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "HistroyView.h"

// CHistroyView

CCriticalSection	CHistroyView::mSection;

IMPLEMENT_DYNCREATE(CHistroyView, CView)

CHistroyView::CHistroyView()
{
	mFilterData.station = 0xffff;
	for (int i = 0; i < eStation_MAX; i++)
		mFilterData.sender[i] = 0;
	mFilterData.imstatus = 0xff;
	mFilterData.status = 0xff;
	mFilterData.message = 0;
	mHistoryData.clear();
	m_nTabItem = 0;
	m_nHistoryType = 0;
	mHistoryHoldData.clear();
}

CHistroyView::~CHistroyView()
{
}

CHistroyView*	CHistroyView::m_pInstance[eHistory_Max] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
CHistroyView*	CHistroyView::Instance(UINT type)
{
	if (m_pInstance[type] == NULL){
		static CHistroyView mInitializerInstance;
		m_pInstance[type] = &mInitializerInstance;
	}
	return m_pInstance[type];
}

BEGIN_MESSAGE_MAP(CHistroyView, CView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CHistroyView 描画

void CHistroyView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 描画コードをここに追加してください。
}


// CHistroyView 診断

#ifdef _DEBUG
void CHistroyView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CHistroyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
CStationControlDoc* CHistroyView::GetDocument() const // デバッグ以外のバージョンはインラインです。
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
void CALLBACK CHistroyView::messageProc(stFilterData& param, UINT type)
{
	switch (type){
	case	eHistory_Operation:
	case	eHistory_Control:
	case	eHistory_Equipment:
	case	eHistory_Event:
	case	eHistory_Alarm:
		CHistroyView::Instance(type)->SetDisplayFilter(param);
		break;
	case	eHistory_SearchOperation:
	case	eHistory_SearchControl:
	case	eHistory_SearchEquipment:
	case	eHistory_SearchEvent:
	case	eHistory_SearchAlarm:
		CHistroyView::Instance(type)->SetDisplaySearch(type, param);
		break;
	}
}


// CHistroyView メッセージ ハンドラー

void CHistroyView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_pInstance[m_nHistoryType] = this;

	// 履歴リストの作成
	CreateHistroyList();

	// タブ型バーの作成
	CRect rectClient;
	GetClientRect(rectClient);
	if (m_nTabItem & eTabItem_Filter){
		//if (theApp.GetSelectStationMode() != eStation_1)
		{
			m_TabBar.Create(CMFCTabCtrl::STYLE_3D, rectClient, this, m_nControlID + 1, CMFCTabCtrl::LOCATION_TOP);
			if (m_nControlID == eEquipmentHistory){
				m_FilterDlgEq.SetOneStation(true);
				m_FilterDlgEq.SetHistoryType(m_nHistoryType);
				m_FilterDlgEq.SetCallback(messageProc, m_nHistoryType);
				m_FilterDlgEq.Create(IDD_DIALOG_FILTER_EQ, &m_TabBar);
				m_FilterDlgEq.SetCallback(messageProc, m_nHistoryType);
				m_TabBar.AddBar(&m_FilterDlgEq, _T("リアルタイム"));
			}
			else{
				m_FilterDlg.SetOneStation(true);
				m_FilterDlg.SetHistoryType(m_nHistoryType);
				m_FilterDlg.Create(IDD_DIALOG_FILTER, &m_TabBar);
				m_FilterDlg.SetCallback(messageProc, m_nHistoryType);
				m_TabBar.AddBar(&m_FilterDlg, _T("リアルタイム"));
			}
		}
	}
	if (m_nTabItem & eTabItem_Search){
		m_TabBar.Create(CMFCTabCtrl::STYLE_3D, rectClient, this, m_nControlID + 1, CMFCTabCtrl::LOCATION_TOP);
		if (m_nControlID == eEquipmentSearch){
			m_SearchDlgEq.SetOneStation(true);
			m_SearchDlgEq.SetHistoryType(m_nHistoryType);
			m_SearchDlgEq.Create(IDD_DIALOG_SEARCH_EQ, &m_TabBar);
			m_SearchDlgEq.SetCallback(messageProc, m_nHistoryType);
			m_TabBar.AddBar(&m_SearchDlgEq, _T("検索"));
		}
		else{
			m_SearchDlg.SetOneStation(true);
			m_SearchDlg.SetHistoryType(m_nHistoryType);
			m_SearchDlg.Create(IDD_DIALOG_SEARCH, &m_TabBar);
			m_SearchDlg.SetCallback(messageProc, m_nHistoryType);
			m_TabBar.AddBar(&m_SearchDlg, _T("検索"));
		}
	}
}

void CHistroyView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	//if (theApp.GetSelectStationMode() != eStation_1 || m_nTabItem & eTabItem_Search)
	{
		if (m_History.GetSafeHwnd()){
			m_History.SetWindowPos(NULL, -1, CMFCTabCtrlEx::mBarHeight, cx + 3, cy - CMFCTabCtrlEx::mBarHeight, /*SWP_NOMOVE | */SWP_NOACTIVATE | SWP_NOZORDER);
		}
		if (m_TabBar.GetSafeHwnd()){
			m_TabBar.SetWindowPos(NULL, -1, -1, cx, CMFCTabCtrlEx::mBarHeight + 10, /*SWP_NOMOVE | */SWP_NOACTIVATE | SWP_NOZORDER);
			m_TabBar.Resize(cx);
		}
	}
	//else{
	//	if (m_History.GetSafeHwnd()){
	//		m_History.SetWindowPos(NULL, -1, -1, cx + 3, cy, /*SWP_NOMOVE | */SWP_NOACTIVATE | SWP_NOZORDER);
	//	}
	//}
}

/*============================================================================*/
/*! 運用履歴

-# 履歴リストの作成

@param		rectClient		クライアント領域
@retval

*/
/*============================================================================*/
void CHistroyView::CreateHistroyList()
{
	// クライアント領域の取得
	CRect rectClient;
	GetClientRect(rectClient);

	m_History.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | /*LVS_NOCOLUMNHEADER |*/ LVS_SHOWSELALWAYS, rectClient, this, m_nControlID);
	// 拡張スタイルの取得
	DWORD  dwStyle = ListView_GetExtendedListViewStyle(m_History.m_hWnd);
	//  拡張スタイルのセット
	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= LVS_EX_FULLROWSELECT/*|LVS_EX_GRIDLINES*/ | 0x00010000/*LVS_EX_DOUBLEBUFFER*/;
	//  新しい拡張スタイルを設定
	ListView_SetExtendedListViewStyle(m_History.m_hWnd, dwStyle);
	m_History.SetBkColor(RGB(0, 0, 0));
	//if (m_nControlID == eEquipmentHistory){
	//	m_History.InsertColumn(eFilterItemEq_Station, _T("内之浦６４ｍ局"), LVCFMT_CENTER, 100);
	//	m_History.InsertColumn(eFilterItemEq_Time, _T("9999/99/99 99:99:99  "), LVCFMT_CENTER, 100);
	//	m_History.InsertColumn(eFilterItemEq_Equipment, _T("設備制御装置９９"), LVCFMT_CENTER, 100);
	//	m_History.InsertColumn(eFilterItemEq_ImStatus, _T("復旧　"), LVCFMT_CENTER, 100);
	//	m_History.InsertColumn(eFilterItemEq_Status, _T("POWER ON/OFF　"), LVCFMT_CENTER, 100);
	//	m_History.InsertColumn(eFilterItemEq_MessageId, _T("９９９９９９"), LVCFMT_LEFT, 100);
	//	m_History.InsertColumn(eFilterItemEq_Message, _T("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"), LVCFMT_CENTER, 100);
	//}
	//else{
	//	m_History.InsertColumn(eFilterItem_Station, _T("内之浦６４ｍ局"), LVCFMT_CENTER, 100);
	//	m_History.InsertColumn(eFilterItem_Time, _T("9999/99/99 99:99:99  "), LVCFMT_CENTER, 100);
	//	m_History.InsertColumn(eFilterItem_Equipment, _T("設備制御装置９９"), LVCFMT_CENTER, 100);
	//	m_History.InsertColumn(eFilterItem_Status, _T("情報　"), LVCFMT_CENTER, 100);
	//	m_History.InsertColumn(eFilterItem_MessageId, _T("９９９９９９"), LVCFMT_LEFT, 100);
	//	m_History.InsertColumn(eFilterItem_Message, _T("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"), LVCFMT_CENTER, 100);
	//}
	int item = 0;
	if (m_nControlID == eEquipmentHistory){
		m_History.InsertColumn(item++, _T("局名          "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(item++, _T("日時                 "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(item++, _T("送信元       "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(item++, _T("重要度"), LVCFMT_CENTER, 100);
		m_History.InsertColumn(item++, _T("ステータス  "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(item++, _T("番号"), LVCFMT_LEFT, 100);
		m_History.InsertColumn(item++, _T("メッセージ"), LVCFMT_LEFT, 100);
	}
	else if (m_nControlID == eEventHistory){
		m_History.InsertColumn(item++, _T("局名          "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(item++, _T("日時                 "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(item++, _T("重要度"), LVCFMT_CENTER, 100);
		m_History.InsertColumn(item++, _T("番号"), LVCFMT_LEFT, 100);
		m_History.InsertColumn(item++, _T("メッセージ"), LVCFMT_LEFT, 100);
	}
	else{
		m_History.InsertColumn(item++, _T("局名          "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(item++, _T("日時                      "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(item++, _T("送信元           "), LVCFMT_CENTER, 100);
		m_History.InsertColumn(item++, _T("重要度"), LVCFMT_CENTER, 100);
		m_History.InsertColumn(item++, _T("番号"), LVCFMT_LEFT, 100);
		m_History.InsertColumn(item++, _T("メッセージ"), LVCFMT_LEFT, 100);
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
void CHistroyView::AutoSizeColumns(int col /*=-1*/)
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
int CHistroyView::AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam/*=0*/)
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
/*! 運用履歴

-# 履歴フィルタの設定

@param	data		履歴情報
@retval

*/
/*============================================================================*/
void CHistroyView::SetDisplayFilter(stFilterData& data)
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

	CString str = CreateFilterString(data);
	m_TabBar.SetFilterText(str);

	m_History.SetRedraw(true);
	mSection.Unlock();
	CGraphMutex::Unlock(eData);
}

/*============================================================================*/
/*! 運用履歴

-# 履歴フィルタの設定

@param	data		履歴情報
@retval

*/
/*============================================================================*/
CString CHistroyView::CreateFilterString(stFilterData& data)
{
	CString str;
	str.Empty();
	if (theApp.GetSelectStationMode() == eStation_1){
		UINT sta = theApp.GetSelectStation();
		// １局モード
		if ((sta == eStation_Usuda64 || sta == eStation_Usuda642) && (data.sender[sta] & 0x0f) == 0x0f){
			// 64m , 54mで全選択時は表示しない
		}
		else if ((sta == eStation_Uchinoura34 || sta == eStation_Uchinoura20) && (data.sender[sta] & 0x03) == 0x03){
			// 34m , 20mで全選択時は表示しない
		}
		else{
			str += _T("送信元装置:");
			if (data.sender[sta] & 0x01){
				str += mHistEquipmentString[0];
				str += _T(" ");
			}
			if (data.sender[sta] & 0x02){
				str += mHistEquipmentString[1];
				str += _T(" ");
			}
			if (data.sender[sta] & 0x04){
				str += mHistEquipmentString[2];
				str += _T(" ");
			}
			if (data.sender[sta] & 0x08){
				str += mHistEquipmentString[3];
				str += _T(" ");
			}
			str += _T(" ");
		}
		if (m_nControlID == eEquipmentHistory){
			if ((data.imstatus & 0x07) == 0x07){
			}
			else{
				str += _T("重要度:");
				if (data.imstatus & 0x01){
					str += mHistStatusString[0];
					str += _T(" ");
				}
				if (data.imstatus & 0x02){
					str += mHistStatusString[1];
					str += _T(" ");
				}
				if (data.imstatus & 0x04){
					str += mHistStatusString[2];
					str += _T(" ");
				}
				str += _T(" ");
			}
			if ((data.status & 0x1F) == 0x1F){
			}
			else{
				str += _T("ステータス:");
				if (data.status & 0x01){
					str += mEqHistStatusString[0];
					str += _T(" ");
				}
				if (data.status & 0x02){
					str += mEqHistStatusString[1];
					str += _T(" ");
				}
				if (data.status & 0x04){
					str += mEqHistStatusString[2];
					str += _T(" ");
				}
				if (data.status & 0x08){
					str += mEqHistStatusString[3];
					str += _T(" ");
				}
				if (data.status & 0x10){
					str += mEqHistStatusString[4];
					str += _T(" ");
				}
				str += _T(" ");
			}
		}
		else{
			if ((data.status & 0x07) == 0x07){
			}
			else{
				str += _T("重要度:");
				if (data.status & 0x01){
					str += mHistStatusString[0];
					str += _T(" ");
				}
				if (data.status & 0x02){
					str += mHistStatusString[1];
					str += _T(" ");
				}
				if (data.status & 0x04){
					str += mHistStatusString[2];
					str += _T(" ");
				}
				str += _T(" ");
			}
		}
	}
	else{
		// ４局モード
		if ((data.station & 0x0f) == 0x0f){
		}
		else{
			str += _T("局:");
			if (data.station & 0x01){
				str += mHistoryStationString[0];	// 臼田64m
				str += _T(" ");
			}
			if (data.station & 0x02){
				str += mHistoryStationString[3];	// 臼田54m
				str += _T(" ");
			}
			if (data.station & 0x04){
				str += mHistoryStationString[1];	// 内之浦34m
				str += _T(" ");
			}
			if (data.station & 0x08){
				str += mHistoryStationString[2];	// 内之浦20m
				str += _T(" ");
			}

			if (m_nHistoryType == eHistory_Operation && data.station & 0x10){
				str += mStationStringEx;			// 筑波局運用管制
				str += _T(" ");
			}
			str += _T(" ");
		}

		bool bset = true;
		for (int i = 0; i < eStation_MAX; i++){
			UINT sta;
			switch (i)
			{
			case eStation_Usuda64:
				sta = 0;
				break;
			case eStation_Usuda642:
				sta = 3;
				break;
			case eStation_Uchinoura34:
				sta = 1;
				break;
			case eStation_Uchinoura20:
				sta = 2;
				break;
			default:
				sta = 0;
				break;
			}

			if (!(data.station & (1 << i))){
				continue;
			}
			if ((data.sender[i] & 0x0f) != 0x0f){
				if (bset == true){
					str += _T("送信元装置:");
					bset = false;
				}
				str += _T("(");
				str += mHistoryStationString[sta];
				str += _T(")");
				if (data.sender[i] & 0x01){
					str += mHistEquipmentString[0];
					str += _T(" ");
				}
				if (data.sender[i] & 0x02){
					str += mHistEquipmentString[1];
					str += _T(" ");
				}
				if (data.sender[i] & 0x04){
					str += mHistEquipmentString[2];
					str += _T(" ");
				}
				if (data.sender[i] & 0x08){
					str += mHistEquipmentString[3];
					str += _T(" ");
				}
				str += _T(" ");
			}
		}
		if (m_nControlID == eEquipmentHistory){
			if ((data.imstatus & 0x07) == 0x07){
			}
			else{
				str += _T("重要度:");
				if (data.imstatus & 0x01){
					str += mHistStatusString[0];
					str += _T(" ");
				}
				if (data.imstatus & 0x02){
					str += mHistStatusString[1];
					str += _T(" ");
				}
				if (data.imstatus & 0x04){
					str += mHistStatusString[2];
					str += _T(" ");
				}
				str += _T(" ");
			}
			if ((data.status & 0x1F) == 0x1F){
			}
			else{
				str += _T("ステータス:");
				if (data.status & 0x01){
					str += mEqHistStatusString[0];
					str += _T(" ");
				}
				if (data.status & 0x02){
					str += mEqHistStatusString[1];
					str += _T(" ");
				}
				if (data.status & 0x04){
					str += mEqHistStatusString[2];
					str += _T(" ");
				}
				if (data.status & 0x08){
					str += mEqHistStatusString[3];
					str += _T(" ");
				}
				if (data.status & 0x10){
					str += mEqHistStatusString[4];
					str += _T(" ");
				}
				str += _T(" ");
			}
		}
		else{
			if ((data.status & 0x07) == 0x07){
			}
			else{
				str += _T("重要度:");
				if (data.status & 0x01){
					str += mHistStatusString[0];
					str += _T(" ");
				}
				if (data.status & 0x02){
					str += mHistStatusString[1];
					str += _T(" ");
				}
				if (data.status & 0x04){
					str += mHistStatusString[2];
					str += _T(" ");
				}
				str += _T(" ");
			}
		}
	}
	return str;
}

/*============================================================================*/
/*! 運用履歴

-# 履歴検索の設定

@param	data		履歴情報
@retval

*/
/*============================================================================*/
void CHistroyView::SetDisplaySearch(int nFilterType, stFilterData& data)
{
	CloseChildWindow(NULL);

	// 対象チェック
	switch (nFilterType){
	case	eHistory_SearchOperation:
	case	eHistory_SearchControl:
	case	eHistory_SearchEquipment:
	case	eHistory_SearchEvent:
	case	eHistory_SearchAlarm:
		break;
	default:
		return;
	}

	m_History.SetRedraw(false);
	// 一旦検索結果をクリアする
	m_History.DeleteAllItems();
	ClearHistroy();

	UINT nStation = theApp.GetSelectStation();
	UINT nHistStation = theApp.GetSelectStation();
	if (nStation == 0)	nHistStation = 0;
	else if (nStation == 1)	nHistStation = 3;
	else if (nStation == 2)	nHistStation = 1;
	else if (nStation == 3)	nHistStation = 2;
	else
		return;
		
	FILE *fp = NULL;
	CString str, path, fname;
	vector<stHistoryData> datatemp;

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
		for (int st = 0; st < eStation_MAX; st++){
			if (theApp.GetSelectStationMode() == eStation_1){
				if (st != nHistStation)
					continue;
			}
			else{
				if (!(data.station & (1 << st)))
					continue;
				nHistStation = st;
				if (nHistStation == 0)	nStation = 0;
				if (nHistStation == 1)	nStation = 2;
				if (nHistStation == 2)	nStation = 3;
				if (nHistStation == 3)	nStation = 1;
			}
		}
		// 対象ファイル名取得
		switch (nFilterType){
		case	eHistory_SearchOperation:
			path = theApp.GetShareFilePath(eFileType_OpHistory_Folder, nStation);
			fname.Format(_T("%s%4d%02d%02d"), (LPCTSTR)path, time.GetYear(), time.GetMonth(), time.GetDay());
			fname += _T(".opl");
			break;
		case	eHistory_SearchControl:
			path = theApp.GetShareFilePath(eFileType_CtrlHistory_Folder, nStation);
			fname.Format(_T("%s%4d%02d%02d"), (LPCTSTR)path, time.GetYear(), time.GetMonth(), time.GetDay());
			fname += _T(".ctl");
			break;
		case	eHistory_SearchEquipment:
			path = theApp.GetShareFilePath(eFileType_EquipHistory_Folder, nStation);
			fname.Format(_T("%s%4d%02d%02d"), (LPCTSTR)path, time.GetYear(), time.GetMonth(), time.GetDay());
			fname += _T(".moni");
			break;
		case	eHistory_SearchEvent:
			//break;
		case	eHistory_SearchLimit:
			//break;
		case	eHistory_SearchAlarm:
			//break;
		default:
			m_History.SetRedraw(true);
			return;
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
				stHistoryData hdata;
				if (_GetDate(hdata, cbuf) == false)
					continue;
				if (_GetTime(hdata, cbuf) == false)
					continue;
				if (_GetStation(hdata, cbuf) == false)
					continue;
				if (_GetEquipment(hdata, cbuf) == false)
					continue;
				if (_GetStatus(hdata, cbuf) == false)
					continue;
				if (_GetMessageID(hdata, cbuf) == false)
					continue;
				if (_GetMessageString(hdata, cbuf) == false)
					continue;
				CTime dtime = CTime(hdata.year, hdata.month, hdata.day, hdata.hour, hdata.minute, hdata.second);
				if (dtime < start || dtime > stop)
					continue;

				// ここまできたら、開始、終了間のデータを取得した
				datatemp.push_back(hdata);
			}
		}CATCH_ALL(e){
		}
		END_CATCH_ALL

		if (fp != NULL)
			fclose(fp);
	}

	// ここで検索フィルタのデータを削除する
	vector<stHistoryData>::iterator itr;
	vector<stHistoryData> datalist;
	datalist.clear();
	for (itr = datatemp.begin(); itr != datatemp.end(); itr++){
		if (!(data.station & (1 << (*itr).station)))
			continue;
		if (data.sender[nStation] != 0){
			if (!(data.sender[nStation] & (1 << (*itr).equipment)))
				continue;
		}
		if (!(data.status & (1 << (*itr).status)))
			continue;
		if (data.message != 0){
			if (data.message != (*itr).message)
				continue;
		}
		datalist.push_back((*itr));
	}

	for (itr = datalist.begin(); itr != datalist.end(); itr++){
		AddHistroy((*itr));
	}

	AutoSizeColumns();

	m_History.SetRedraw(true);
}

bool CHistroyView::_GetDate(stHistoryData& hdata, CString cbuf)
{
	CString str, temp;
	AfxExtractSubString(str, cbuf, eFileType_Date, ' ');
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

bool CHistroyView::_GetTime(stHistoryData& hdata, CString cbuf)
{
	CString str, temp;
	AfxExtractSubString(str, cbuf, eFileType_Time, ' ');
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

bool CHistroyView::_GetStation(stHistoryData& hdata, CString cbuf)
{
	UINT nStation = theApp.GetSelectStation();
	if (theApp.GetSelectStationMode() == eStation_5)
		nStation = 0;
	hdata.station = (BYTE)nStation;
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

bool CHistroyView::_GetEquipment(stHistoryData& hdata, CString cbuf)
{
	// 送信元
	typedef struct {
		CString		name;
		UINT		code;
	} stSender;
	const stSender mSender[] = {
		{ _T("e1"), eSenderItem1 },	// 局運用管制装置
		{ _T("c0"), eSenderItem2 },	// 設備制御装置
		{ _T("e2"), eSenderItem3 },	// TLM入力装置
		{ _T("e3"), eSenderItem4 },	// 出力装置
	};

	CString str, temp;
	AfxExtractSubString(str, cbuf, eFileType_Equipment, ' ');
	for (int i = 0; i < sizeof(mSender) / sizeof(mSender[0]); i++){
		if (str == mSender[i].name){
			hdata.equipment = (BYTE)mSender[i].code;
			return true;
		}
	}
	return false;
}

bool CHistroyView::_GetStatus(stHistoryData& hdata, CString cbuf)
{
	// 送信元
	typedef struct {
		CString		name;
		UINT		code;
	} stStatus;
	const stStatus mStatus[] = {
		{ _T("di"), eHistStatus_Normal },	// 情報
		{ _T("dw"), eHistStatus_Warning },	// 警告
		{ _T("ds"), eHistStatus_Fault },	// 異常
		{ _T("dg"), eHistStatus_Normal },	// 情報
	};
	CString str, temp;
	AfxExtractSubString(str, cbuf, eFileType_Status, ' ');
	for (int i = 0; i < sizeof(mStatus) / sizeof(mStatus[0]); i++){
		if (str == mStatus[i].name){
			hdata.status = (BYTE)mStatus[i].code;
			return true;
		}
	}
	return false;
}

bool CHistroyView::_GetMessageID(stHistoryData& hdata, CString cbuf)
{
	CString str, temp;
	AfxExtractSubString(temp, cbuf, eFileType_MessageID, ' ');
	str = temp.Mid(1, temp.GetLength() - 1);
	hdata.message = _wtoi(str);
	return true;
}

bool CHistroyView::_GetMessageString(stHistoryData& hdata, CString cbuf)
{
	CString str, temp;
	AfxExtractSubString(str, cbuf, eFileType_MessageString, ' ');
	wsprintf(hdata.msgstring, _T("%s"), (LPCTSTR)str);
	return true;
}

/*============================================================================*/
/*! 運用履歴

-# 有効履歴のチェック

@param	nFilterType		タイプ
@param	val				項目
@retval

*/
/*============================================================================*/
bool CHistroyView::IsDisplay(int nFilterType, DWORD val, UINT nStation)
{
	if (m_nControlID == eEquipmentHistory){
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
		case eFilterItemEq_MessageId:
			if (mFilterData.message == 0)
				return true;
			if (mFilterData.message == val)
				return true;
			break;
		default:
			break;
		}
	}
	else if (m_nControlID == eEventHistory){
		switch (nFilterType){
		case eFilterItemEvent_Station:
			if (mFilterData.station & (1 << val))
				return true;
			break;
		case eFilterItemEvent_Status:
			if (mFilterData.status & (1 << val))
				return true;
			break;
		case eFilterItemEvent_MessageId:
			if (mFilterData.message == 0)
				return true;
			if (mFilterData.message == val)
				return true;
			break;
		default:
			break;
		}
	}
	else{
		switch (nFilterType){
		case eFilterItem_Station:
			if (mFilterData.station & (1 << val))
				return true;
			break;
		case eFilterItem_Equipment:
			if (mFilterData.sender[nStation] == 0)
				return true;
			if (mFilterData.sender[nStation] & (1 << val))
				return true;
			break;
		case eFilterItem_Status:
			if (mFilterData.status & (1 << val))
				return true;
			break;
		case eFilterItem_MessageId:
			if (mFilterData.message == 0)
				return true;
			if (mFilterData.message == val)
				return true;
			break;
		default:
			break;
		}
	}
	return false;
}

/*============================================================================*/
/*! 運用履歴

-# 履歴情報の追加

@param
@retval	bool	trueの場合リスト登録処理を行う falseの場合はスクロール停止中でデータ保留が開始された

*/
/*============================================================================*/
bool CHistroyView::AddHistroyEx(stHistoryData& data)
{
	if (_CheckHoldHistroy() == false){
		if (mHistoryHoldData.size() == MAX_HISTORY_DATA){
			mHistoryHoldData.pop_front();
		}
		mHistoryHoldData.push_back(data);
		return false;
	}

	return true;
}
/*============================================================================*/
/*! 運用履歴

-# スクロール停止中に保留されたデータのリストア

@param
@retval	bool

*/
/*============================================================================*/
bool CHistroyView::RestoreHoldData()
{
	// 保留データ数の取得
	UINT size = (UINT)mHistoryHoldData.size();
	TRACE("### Scroll Hold Data Num=%d\n", size);

	// 取得したデータ数分リスト先頭から削除する
	m_History.SetRedraw(false);
	for (UINT i = 0; i < size; i++){
		m_History.DeleteItem(0);
	}
	m_History.SetRedraw(true);

	// 保留されていたデータをすべてリストに追加する
	while (mHistoryHoldData.size()){
		stHistoryData data = mHistoryHoldData.front();
		mHistoryHoldData.pop_front();
		AddHistroy(data);
	}

	return true;
}

/*============================================================================*/
/*! 運用履歴

-# 履歴リストの状態を調べ、データを表示するか否かを判断する
-リストに最大数存在する
　-スクロール停止中の場合
 　　・データキューから削除しない
   　　・リストには登録しない
	 -スクロール中の場合
	 　　・データキューから削除
	   ・リストの先頭削除
	   ・リストの最後尾に追加
	   -リストに最大数存在しない
	   　-スクロール停止中の場合
		-スクロール中の場合
		　・データキューから削除
		 ・リストの先頭削除
		 ・リストの最後尾に追加


	@param
	@retval	bool	trueの場合リスト登録処理を行う falseの場合はスクロール停止中でデータ保留が開始された

*/
/*============================================================================*/
bool CHistroyView::_CheckHoldHistroy()
{
	// リストに最大数登録されているか調べる
	if (m_History.GetItemCount() == MAX_HISTORY_DATA){
		// リストには最大数表示されている
		// リストはスクロール停止中か？
		if (m_History.GetSelectedCount() != 0){
			// 選択されているのでスクロール停止中
			// 最大数でスクロール停止中なのでデータの追加は行わない、しかしデータキューには溜める
			return false;
		}
	}

	// ここまでくれば、スクロール解除中あるいはリストには最大数登録されていない

	return true;
}
