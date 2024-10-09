// AlarmListFormView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "AlarmListFormView.h"


// CAlarmListFormView

IMPLEMENT_DYNCREATE(CAlarmListFormView, CFormView)

CAlarmListFormView::CAlarmListFormView()
	: CFormView(CAlarmListFormView::IDD)
	, m_strUpdateTime(_T(""))
{
}

CAlarmListFormView::~CAlarmListFormView()
{
}

void CAlarmListFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_STATION, m_grStation);
	DDX_Check(pDX, IDC_CHECK_STATION1, m_bStation[0]);
	DDX_Check(pDX, IDC_CHECK_STATION2, m_bStation[1]);
	DDX_Check(pDX, IDC_CHECK_STATION3, m_bStation[2]);
	DDX_Check(pDX, IDC_CHECK_STATION4, m_bStation[3]);
	DDX_Control(pDX, IDC_CHECK_STATION1, m_btStation[0]);
	DDX_Control(pDX, IDC_CHECK_STATION2, m_btStation[1]);
	DDX_Control(pDX, IDC_CHECK_STATION3, m_btStation[2]);
	DDX_Control(pDX, IDC_CHECK_STATION4, m_btStation[3]);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_grStatus);
	DDX_Check(pDX, IDC_CHECK_SIMSTATUS1, m_bImStatus[0]);
	DDX_Check(pDX, IDC_CHECK_SIMSTATUS2, m_bImStatus[1]);
	DDX_Check(pDX, IDC_CHECK_SIMSTATUS3, m_bImStatus[2]);
	DDX_Control(pDX, IDC_STATIC_SSTATUS, m_grSStatus);
	DDX_Check(pDX, IDC_CHECK_SSTATUS1, m_bStatus[0]);
	DDX_Check(pDX, IDC_CHECK_SSTATUS2, m_bStatus[1]);
	DDX_Check(pDX, IDC_CHECK_SSTATUS3, m_bStatus[2]);
	DDX_Check(pDX, IDC_CHECK_SSTATUS4, m_bStatus[3]);
	DDX_Control(pDX, IDC_LIST_HISTORY, m_History);
	DDX_Text(pDX, IDC_STATIC_ALMUPDATETIME, m_strUpdateTime);
}

BEGIN_MESSAGE_MAP(CAlarmListFormView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL, &CAlarmListFormView::OnBnClickedButtonSelectall)
	ON_BN_CLICKED(IDC_BUTTON_EXEC, &CAlarmListFormView::OnBnClickedButtonExec)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CAlarmListFormView 診断

#ifdef _DEBUG
void CAlarmListFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAlarmListFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
CStationControlDoc* CAlarmListFormView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CAlarmListFormView メッセージ ハンドラー


void CAlarmListFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// 履歴リストの作成
	CreateHistroyList();

	// ウィンドウ幅を合わせる
	int	nMapMode;
	GetDeviceScrollSizes(nMapMode, mSizeTotal, mSizePage, mSizeLine);
	SetScaleToFitSize(mSizeTotal);

	// 局
	m_btStation[0].SetStation(eStation_Usuda64);
	m_btStation[1].SetStation(eStation_Uchinoura34);
	m_btStation[2].SetStation(eStation_Uchinoura20);
	m_btStation[3].SetStation(eStation_Usuda642);

	OnBnClickedButtonSelectall();
	mAlarmData.station = 0;
	for (int i = 0; i < CShareAlarmIf::mAlarmStationMax; i++)
		mAlarmData.station |= (m_bStation[i] << i);
	for (int i = 0; i < eAlarmImStatus_Max; i++)
		mAlarmData.imstatus |= (m_bImStatus[i] << i);
	for (int i = 0; i < eAlarmStatus_Max; i++)
		mAlarmData.status |= (m_bStatus[i] << i);

	GetDocument()->SetTitleEx(_T("設備アラーム状況一覧"));

	SetTimer(1, 1000, NULL);
}

/*============================================================================*/
/*! CAlarmListFormView

-# 全選択ボタン押下時の処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CAlarmListFormView::OnBnClickedButtonSelectall()
{
	UpdateData(TRUE);
	for (int i = 0; i < CShareAlarmIf::mAlarmStationMax; i++)
		m_bStation[i] = TRUE;
	for (int i = 0; i < eAlarmImStatus_Max; i++)
		m_bImStatus[i] = TRUE;
	for (int i = 0; i < eAlarmStatus_Max; i++)
		m_bStatus[i] = TRUE;
	UpdateData(FALSE);
}

/*============================================================================*/
/*! CAlarmListFormView

-# 実行ボタン押下時の処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CAlarmListFormView::OnBnClickedButtonExec()
{
	UpdateData(TRUE);
	mAlarmData.station = 0;
	for (int i = 0; i < CShareAlarmIf::mAlarmStationMax; i++)
		mAlarmData.station |= (m_bStation[i] << i);
	mAlarmData.imstatus = 0;
	for (int i = 0; i < eAlarmImStatus_Max; i++)
		mAlarmData.imstatus |= (m_bImStatus[i] << i);
	mAlarmData.status = 0;
	for (int i = 0; i < eAlarmStatus_Max; i++)
		mAlarmData.status |= (m_bStatus[i] << i);

	CGraphMutex::Lock(eAlarm);
	UpdateAlarmList();
	CGraphMutex::Unlock(eAlarm);
}

/*============================================================================*/
/*! 運用履歴

-# 履歴リストの作成

@param		rectClient		クライアント領域
@retval

*/
/*============================================================================*/
void CAlarmListFormView::CreateHistroyList()
{
	// クライアント領域の取得
	CRect rectClient;
	GetClientRect(rectClient);

	DWORD  dwStyle = ListView_GetExtendedListViewStyle(m_History.m_hWnd);
	//  拡張スタイルのセット
	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= LVS_EX_FULLROWSELECT/*|LVS_EX_GRIDLINES*/ | LVS_EX_DOUBLEBUFFER;
	//  新しい拡張スタイルを設定
	ListView_SetExtendedListViewStyle(m_History.m_hWnd, dwStyle);
	m_History.SetBkColor(RGB(0, 0, 0));
	m_History.InsertColumn(eAlarmItem_Station, _T("局名          "), LVCFMT_CENTER, 100);
	m_History.InsertColumn(eAlarmItem_Equipment, _T("送信元       "), LVCFMT_CENTER, 100);
	m_History.InsertColumn(eAlarmItem_Status, _T("ステータス  "), LVCFMT_CENTER, 100);
	m_History.InsertColumn(eAlarmItem_MonName, _T("監視項目名"), LVCFMT_LEFT, 100);
	m_History.InsertColumn(eAlarmItem_MonValue, _T("監視項目"), LVCFMT_LEFT, 100);

	AutoSizeColumns();
}
/*============================================================================*/
/*! 運用履歴

-# カラムの自動サイズ調整

@param	col		カラム位置のカラムサイズを自動に調整します
@retval

*/
/*============================================================================*/
void CAlarmListFormView::AutoSizeColumns(int col /*=-1*/)
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
int CAlarmListFormView::AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam/*=0*/)
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


void CAlarmListFormView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		KillTimer(nIDEvent);

		GetAlarmShareData();

		SetTimer(nIDEvent, 1000, NULL);
	}
	CFormView::OnTimer(nIDEvent);
}

/*============================================================================*/
/*! CAlarmListFormView

-# 共有メモリからアラームデータを取得して表示する

@param  なし
@retval なし
*/
/*============================================================================*/
void CAlarmListFormView::GetAlarmShareData()
{
	bool bUpdate = false;

	for (int i = 0; i < CShareAlarmIf::mAlarmStationMax; i++)
	{
		// 設備アラーム状況一覧データの取得
		CShareAlarmIf& alarmIf = theApp.GetAlarmIf(i);
		if (alarmIf.IsValidMap() == false)
		{
			// 共有メモリ領域が有効でない場合、再度生成する
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CString strMsg;
			strMsg.Format(_T("m_hMap[%d] creates again."), i);
			CLogTraceEx::Write(_T(""), _T("CAlarmListFormView"), _T("GetAlarmShareData"), _T("info"), strMsg, nLogEx::info);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			alarmIf.Create(true, i);
		}

		vector<CShareAlarmIf::stAlarmItem> temp(0);
		temp.clear();
		temp.resize(CShareAlarmIf::mAlarmItemMax);
		if (alarmIf.Recv((CShareAlarmIf::stAlarmItem*)&temp.at(0), (UINT)temp.size()))
		{
			bUpdate = true;

			CGraphMutex::Lock(eAlarm);
			vector<CShareAlarmIf::stAlarmItem>& alarmList = theApp.GetAlarmList(i);
			alarmList.swap(temp);
			alarmList.resize(CShareAlarmIf::mAlarmItemMax);
			CGraphMutex::Unlock(eAlarm);
		}
	}

	if (bUpdate)
	{
		UpdateData(TRUE);
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CAlarmListThread"), _T("Run"), _T("Update Alarm List"), _T(""), nLogEx::debug);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		CGraphMutex::Lock(eAlarm);
		UpdateAlarmList();
		CGraphMutex::Unlock(eAlarm);
		UpdateData(FALSE);
	}
}

/*============================================================================*/
/*! CAlarmListFormView

-# 設備アラーム状況一覧の作成

@param  なし
@retval なし
*/
/*============================================================================*/
void CAlarmListFormView::UpdateAlarmList()
{
	static bool bAutoSize = true;
	m_History.SetRedraw(false);

	m_History.DeleteAllItems();

	vector<CShareAlarmIf::stAlarmItem>::iterator itr;
	int item = 0;
	CString str;
	CString time=_T("");
	COleDateTime oleTime, oleTemp;

	for (int i = 0; i < CShareAlarmIf::mAlarmStationMax; i++)
	{
		vector<CShareAlarmIf::stAlarmItem>& list = theApp.GetAlarmList(i);

		for (itr = list.begin(); itr != list.end(); itr++){
			if ((*itr).line.no == 0)
				continue;
			if ((*itr).line.no == (DWORD)-1)
			{
				// 時刻情報のみの場合
				str = CString((*itr).updatetime);
				str.Replace('-', '/');
				oleTemp.ParseDateTime(str);
				if (oleTemp > oleTime){
					time = CString((*itr).updatetime);
					oleTime = oleTemp;
				}
				continue;
			}
			if (!(mAlarmData.station & (1 << (*itr).line.station)))
				continue;
			if (!(mAlarmData.status & (1 << (*itr).line.status)))
				continue;
			if (!(mAlarmData.imstatus & (1 << eAlarmImStatus_Warning))){
				if ((*itr).line.status % 2 == 0)
					continue;
			}
			if (!(mAlarmData.imstatus & (1 << eAlarmImStatus_Error))){
				if ((*itr).line.status % 2 == 1)
					continue;
			}

			str = CString((*itr).updatetime);
			str.Replace('-', '/');
			oleTemp.ParseDateTime(str);
			if (oleTemp > oleTime){
				time = CString((*itr).updatetime);
				oleTime = oleTemp;
			}
			str.Format(_T("%s"), mAlarmStationString[(*itr).line.station]);
			AddItem(item, 0, str, (*itr).line.status % 2 == 0 ? eHistStatus_Warning : eHistStatus_Fault);
			str.Format(_T("%s"), (LPCTSTR)mAlarmEquipmentString[(*itr).line.sender]);
			AddItem(item, 1, str);
			str.Format(_T("%s"), mAlarmStatusString[(*itr).line.status]);
			AddItem(item, 2, str);
			str.Format(_T("%s"), (LPCTSTR)CString((*itr).line.monname));
			AddItem(item, 3, str);
			str.Format(_T("%s"), (LPCTSTR)CString((*itr).line.monval));
			AddItem(item, 4, str);
			item++;
		}
	}

	if (bAutoSize == true){
		if (m_History.GetItemCount() != 0){
			AutoSizeColumns();
			bAutoSize = false;
		}
	}
	m_strUpdateTime = time;

	m_History.SetRedraw(true);
}


void CAlarmListFormView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if (m_History.GetSafeHwnd()){
		CRect rectCtrl;
		m_History.GetWindowRect(&rectCtrl);
		ScreenToClient(rectCtrl);
		CRect rect;
		GetClientRect(&rect);
		//rect.left = rectCtrl.left;
		rect.top = rectCtrl.top;
		m_History.MoveWindow(&rect);
	}
}


BOOL CAlarmListFormView::OnEraseBkgnd(CDC* pDC)
{
	// リージョンエリアを求める
	if (pDC == NULL)
		return TRUE;
	HDC hdc = pDC->GetSafeHdc();
	if (hdc == NULL)
		return TRUE;
	CRect clientRect;
	GetClientRect(clientRect);
	CRect listRect;
	m_History.GetWindowRect(&listRect);
	ScreenToClient(&listRect);
	clientRect.bottom = listRect.top;

	// リージョン生成
	HRGN	hRgn = CreateRectRgn(clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);
	SelectClipRgn(hdc, hRgn);

	// 背景消す
	CFormView::OnEraseBkgnd(pDC);

	// リージョン解除
	SelectClipRgn(hdc, NULL);
	DeleteObject(hRgn);

	return TRUE;
}


void CAlarmListFormView::OnDestroy()
{
	CFormView::OnDestroy();

	KillTimer(1);
}
