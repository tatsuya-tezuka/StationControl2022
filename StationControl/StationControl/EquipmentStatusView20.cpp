// EquipmentStatusView34.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "EquipmentStatusView20.h"
#include "afxdialogex.h"


// CEquipmentStatusView20 ダイアログ

IMPLEMENT_DYNCREATE(CEquipmentStatusView20, CFormView)

CEquipmentStatusView20::CEquipmentStatusView20()
: CFormView(CEquipmentStatusView20::IDD)
{

}

CEquipmentStatusView20::~CEquipmentStatusView20()
{
	for (int i = 0; i < eTabMax; i++){
		CSummaryProperties* pProp = mTabList[i].tab.GetProperties();
		if (pProp != NULL){
			delete pProp;
			pProp = NULL;
		}
	}

	vector<CWnd*>::iterator itr;
	for (itr = mControlList.begin(); itr != mControlList.end(); itr++){
		if ((*itr)->GetSafeHwnd()){
			(*itr)->DestroyWindow();
		}
		delete (*itr);
	}
}

void CEquipmentStatusView20::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANTENNA_LOC, mLocAntenna);
	DDX_Control(pDX, IDC_XSEND_LOC, mLocXsend);
	DDX_Control(pDX, IDC_XRECVOLD_LOC, mLocXrecvOld);
	DDX_Control(pDX, IDC_XMEAS_LOC, mLocXmeas);
	DDX_Control(pDX, IDC_STATUS_LOC, mLocStatus);
	DDX_Control(pDX, IDC_TARGET_LOC, mLocTarget);
	DDX_Control(pDX, IDC_GRAPH1_LOC, mLocGraph1);
	DDX_Control(pDX, IDC_GRAPH2_LOC, mLocGraph2);
	DDX_Control(pDX, IDC_GRAPH3_LOC, mLocGraph3);
	DDX_Control(pDX, IDC_GRAPH4_LOC, mLocGraph4);
}

BEGIN_MESSAGE_MAP(CEquipmentStatusView20, CFormView)
END_MESSAGE_MAP()


// CEquipmentStatusView20 診断

#ifdef _DEBUG
void CEquipmentStatusView20::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CEquipmentStatusView20::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
CStationControlDoc* CEquipmentStatusView20::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CEquipmentStatusView20 メッセージ ハンドラー


void CEquipmentStatusView20::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// システムコマンドSC_CLOSEの使用禁止
	CMenu* pSysMenu = GetParent()->GetSystemMenu(FALSE);
	if (pSysMenu)
		pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

	GetDocument()->SetTitleEx();

	CRect rect;
	GetClientRect(rect);

	// ウィンドウ幅を合わせる
	GetParentFrame()->RecalcLayout();
	int	nMapMode;
	GetDeviceScrollSizes(nMapMode, mSizeTotal, mSizePage, mSizeLine);
	ResizeParentToFit(FALSE);

	if (theApp.GetSelectStation() == eStation_Usuda64)
		createTab();
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
		createTab34();
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
		createTab20();
	else
		createTab();
}

/*============================================================================*/
/*! 設備状況ビュー

-# 各種タブコントロールの作成

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentStatusView20::createTab()
{
	mControlList.clear();
	CRect rectTab;
	int pos = 0;
	for (int i = 0; i < eTabMax; i++){
		if (mTabItemID[i][0].id == 0)
			break;
		GetDlgItem(mTabLocationID[i])->GetWindowRect(&rectTab);
		ScreenToClient(&rectTab);
		mTabList[i].tab.Create(CMFCTabCtrl::STYLE_3D, rectTab, this, 1, CMFCTabCtrl::LOCATION_TOP);

		UINT item = 0;
		mTabList[i].itemCount = 0;
		for (item = 0; item < MAX_TABITEM; item++){
			if (mTabItemID[i][item].id == 0)
				break;
			switch (mTabItemID[i][item].type){
			case	eViewType_Dialog:
				if (mTabItemID[i][item].pset != NULL){
					CDialogEx *pdlg = (CDialogEx*)(mTabItemID[i][item].pset->CreateObject());
					if (!pdlg->Create(mTabItemID[i][item].id, &mTabList[i].tab)){
						TRACE("Failed view creation\n");
						continue;
					}
					mTabList[i].tab.AddTab(pdlg, mTabItemID[i][item].name, 0, FALSE);
					mTabList[i].itemCount++;
					mControlList.push_back(pdlg);
				}
				else{
					if (!mTabList[i].dlg[item].Create(mTabItemID[i][item].id, &mTabList[i].tab)){
						TRACE("Failed view creation\n");
						continue;
					}
					mTabList[i].tab.AddTab(&mTabList[i].dlg[item], mTabItemID[i][item].name, 0, FALSE);
					mTabList[i].itemCount++;
				}
				break;
			case	eViewType_StatusGridView:
			{
				CRuntimeClass* pClass = RUNTIME_CLASS(CEqStatusGridView);
				if (pClass != NULL){
					mEqStatusView = (CEqStatusGridView*)(pClass->CreateObject());
					if (mEqStatusView != NULL){
						mEqStatusView->Create(NULL, NULL, WS_VISIBLE | WS_CLIPCHILDREN | WS_CHILD, rectTab, &mTabList[i].tab, 0, NULL);
						mEqStatusView->SendMessage(WM_INITIALUPDATE);
						mTabList[i].pview[item] = mEqStatusView;
						mTabList[i].tab.AddTab(mTabList[i].pview[item], mTabItemID[i][item].name, 0, FALSE);
						mTabList[i].itemCount++;
					}
				}
				break;
			}
			case	eViewType_TrackView:
			{
				CRuntimeClass* pClass = RUNTIME_CLASS(CForecastRadarGraphView);
				if (pClass != NULL){
					mTrackView = (CForecastRadarGraphView*)(pClass->CreateObject());
					if (mTrackView != NULL){
						mTrackView->SetViewtype(eRadarViewType_Station);
						mTrackView->SetStationID(theApp.GetSelectStation());
						mTrackView->Create(NULL, NULL, WS_VISIBLE | WS_CLIPCHILDREN | WS_CHILD, rectTab, &mTabList[i].tab, 0, NULL);
						mTrackView->SendMessage(WM_INITIALUPDATE);
						mTabList[i].pview[item] = mTrackView;
						mTabList[i].tab.AddTab(mTabList[i].pview[item], mTabItemID[i][item].name, 0, FALSE);
						mTabList[i].itemCount++;
					}
				}
				break;
			}
			case	eViewType_SummaryGraph:
				if (!mSummary[mTabItemID[i][item].id - 1].Create(&mTabList[i].tab, rectTab, eSummaryCtrl + (mTabItemID[i][item].id - 1), theApp.GetSelectStation(), eViewType1_EquipmentStatus)){
					TRACE("Failed view creation\n");
					continue;
				}
				mTabList[i].pwnd[item] = &mSummary[mTabItemID[i][item].id - 1];
				mTabList[i].tab.AddTab(mTabList[i].pwnd[item], mTabItemID[i][item].name, 0, FALSE);
				mTabList[i].itemCount++;
				mSummary[mTabItemID[i][item].id - 1].PostMessage(eMessage_InitSummaryWnd);
				break;

			case	eViewType_SummaryGraphProperties:
				if (!mSummaryProperties[mTabItemID[i][item].id - 1].Create(&mTabList[i].tab, CSummaryPropertiesDlg::IDD, mTabItemID[i][item].id - 1, theApp.GetSelectStation(), eViewType1_EquipmentStatus)){
					TRACE("Failed view creation\n");
					continue;
				}
				CSummaryProperties* pProp = new CSummaryProperties(mTabItemID[i][item].id - 1, theApp.GetSelectStation(), eViewType1_EquipmentStatus, mTabItemID[i][0].name);
				if (pProp == NULL) {
					TRACE("Failed properties creation\n");
					continue;
				}
				mTabList[i].pwnd[item] = &mSummaryProperties[mTabItemID[i][item].id - 1];
				mTabList[i].tab.AddTab(mTabList[i].pwnd[item], mTabItemID[i][item].name, 0, FALSE);
				mTabList[i].tab.SetTabType(CMFCEqTabCtrl::eTabType_SummaryGraph);
				mTabList[i].tab.SetProperties(pProp);
				mTabList[i].itemCount++;
				break;
			}
			pos++;
		}
		mTabList[i].tab.Initialize();
	}
}
void CEquipmentStatusView20::createTab34()
{
	mControlList.clear();
	CRect rectTab;
	int pos = 0;
	for (int i = 0; i < eTabMax; i++){
		if (mTabItemID34[i][0].id == 0)
			break;
		GetDlgItem(mTabLocationID[i])->GetWindowRect(&rectTab);
		ScreenToClient(&rectTab);
		mTabList[i].tab.Create(CMFCTabCtrl::STYLE_3D, rectTab, this, 1, CMFCTabCtrl::LOCATION_TOP);

		UINT item = 0;
		mTabList[i].itemCount = 0;
		for (item = 0; item < MAX_TABITEM; item++){
			if (mTabItemID34[i][item].id == 0)
				break;
			switch (mTabItemID34[i][item].type){
			case	eViewType_Dialog:
				if (mTabItemID34[i][item].pset != NULL){
					CDialogEx *pdlg = (CDialogEx*)(mTabItemID34[i][item].pset->CreateObject());
					if (!pdlg->Create(mTabItemID34[i][item].id, &mTabList[i].tab)){
						TRACE("Failed view creation\n");
						continue;
					}
					mTabList[i].tab.AddTab(pdlg, mTabItemID34[i][item].name, 0, FALSE);
					mTabList[i].itemCount++;
					mControlList.push_back(pdlg);
				}
				else{
					if (!mTabList[i].dlg[item].Create(mTabItemID34[i][item].id, &mTabList[i].tab)){
						TRACE("Failed view creation\n");
						continue;
					}
					mTabList[i].tab.AddTab(&mTabList[i].dlg[item], mTabItemID34[i][item].name, 0, FALSE);
					mTabList[i].itemCount++;
				}
				break;
			case	eViewType_StatusGridView:
			{
				CRuntimeClass* pClass = RUNTIME_CLASS(CEqStatusGridView);
				if (pClass != NULL){
					mEqStatusView = (CEqStatusGridView*)(pClass->CreateObject());
					if (mEqStatusView != NULL){
						mEqStatusView->Create(NULL, NULL, WS_VISIBLE | WS_CLIPCHILDREN | WS_CHILD, rectTab, &mTabList[i].tab, 0, NULL);
						mEqStatusView->SendMessage(WM_INITIALUPDATE);
						mTabList[i].pview[item] = mEqStatusView;
						mTabList[i].tab.AddTab(mTabList[i].pview[item], mTabItemID34[i][item].name, 0, FALSE);
						mTabList[i].itemCount++;
					}
				}
				break;
			}
			case	eViewType_TrackView:
			{
				CRuntimeClass* pClass = RUNTIME_CLASS(CForecastRadarGraphView);
				if (pClass != NULL){
					mTrackView = (CForecastRadarGraphView*)(pClass->CreateObject());
					if (mTrackView != NULL){
						mTrackView->SetViewtype(eRadarViewType_Station);
						mTrackView->SetStationID(theApp.GetSelectStation());
						mTrackView->Create(NULL, NULL, WS_VISIBLE | WS_CLIPCHILDREN | WS_CHILD, rectTab, &mTabList[i].tab, 0, NULL);
						mTrackView->SendMessage(WM_INITIALUPDATE);
						mTabList[i].pview[item] = mTrackView;
						mTabList[i].tab.AddTab(mTabList[i].pview[item], mTabItemID34[i][item].name, 0, FALSE);
						mTabList[i].itemCount++;
					}
				}
				break;
			}
			case	eViewType_SummaryGraph:
				if (!mSummary[mTabItemID34[i][item].id - 1].Create(&mTabList[i].tab, rectTab, eSummaryCtrl + (mTabItemID34[i][item].id - 1), theApp.GetSelectStation(), eViewType1_EquipmentStatus)){
					TRACE("Failed view creation\n");
					continue;
				}
				mTabList[i].pwnd[item] = &mSummary[mTabItemID34[i][item].id - 1];
				mTabList[i].tab.AddTab(mTabList[i].pwnd[item], mTabItemID34[i][item].name, 0, FALSE);
				mTabList[i].itemCount++;
				mSummary[mTabItemID34[i][item].id - 1].PostMessage(eMessage_InitSummaryWnd);
				break;

			case	eViewType_SummaryGraphProperties:
				if (!mSummaryProperties[mTabItemID34[i][item].id - 1].Create(&mTabList[i].tab, CSummaryPropertiesDlg::IDD, mTabItemID34[i][item].id - 1, theApp.GetSelectStation(), eViewType1_EquipmentStatus)){
					TRACE("Failed view creation\n");
					continue;
				}
				CSummaryProperties* pProp = new CSummaryProperties(mTabItemID34[i][item].id - 1, theApp.GetSelectStation(), eViewType1_EquipmentStatus, mTabItemID34[i][0].name);
				if (pProp == NULL) {
					TRACE("Failed properties creation\n");
					continue;
				}
				mTabList[i].pwnd[item] = &mSummaryProperties[mTabItemID34[i][item].id - 1];
				mTabList[i].tab.AddTab(mTabList[i].pwnd[item], mTabItemID34[i][item].name, 0, FALSE);
				mTabList[i].tab.SetTabType(CMFCEqTabCtrl::eTabType_SummaryGraph);
				mTabList[i].tab.SetProperties(pProp);
				mTabList[i].itemCount++;
				break;
			}
			pos++;
		}
		mTabList[i].tab.Initialize();
	}
}
void CEquipmentStatusView20::createTab20()
{
	mControlList.clear();
	CRect rectTab;
	int pos = 0;
	for (int i = 0; i < eTabMax; i++){
		if (mTabItemID20[i][0].id == 0)
			break;
		GetDlgItem(mTabLocationID[i])->GetWindowRect(&rectTab);
		ScreenToClient(&rectTab);
		mTabList[i].tab.Create(CMFCTabCtrl::STYLE_3D, rectTab, this, 1, CMFCTabCtrl::LOCATION_TOP);

		UINT item = 0;
		mTabList[i].itemCount = 0;
		for (item = 0; item < MAX_TABITEM; item++){
			if (mTabItemID20[i][item].id == 0)
				break;
			switch (mTabItemID20[i][item].type){
			case	eViewType_Dialog:
				if (mTabItemID20[i][item].pset != NULL){
					CDialogEx *pdlg = (CDialogEx*)(mTabItemID20[i][item].pset->CreateObject());
					if (!pdlg->Create(mTabItemID20[i][item].id, &mTabList[i].tab)){
						TRACE("Failed view creation\n");
						continue;
					}
					mTabList[i].tab.AddTab(pdlg, mTabItemID20[i][item].name, 0, FALSE);
					mTabList[i].itemCount++;
					mControlList.push_back(pdlg);
				}
				else{
					if (!mTabList[i].dlg[item].Create(mTabItemID20[i][item].id, &mTabList[i].tab)){
						TRACE("Failed view creation\n");
						continue;
					}
					mTabList[i].tab.AddTab(&mTabList[i].dlg[item], mTabItemID20[i][item].name, 0, FALSE);
					mTabList[i].itemCount++;
				}
				break;
			case	eViewType_StatusGridView:
			{
				CRuntimeClass* pClass = RUNTIME_CLASS(CEqStatusGridView);
				if (pClass != NULL){
					mEqStatusView = (CEqStatusGridView*)(pClass->CreateObject());
					if (mEqStatusView != NULL){
						mEqStatusView->Create(NULL, NULL, WS_VISIBLE | WS_CLIPCHILDREN | WS_CHILD, rectTab, &mTabList[i].tab, 0, NULL);
						mEqStatusView->SendMessage(WM_INITIALUPDATE);
						mTabList[i].pview[item] = mEqStatusView;
						mTabList[i].tab.AddTab(mTabList[i].pview[item], mTabItemID20[i][item].name, 0, FALSE);
						mTabList[i].itemCount++;
					}
				}
				break;
			}
			case	eViewType_TrackView:
			{
				CRuntimeClass* pClass = RUNTIME_CLASS(CForecastRadarGraphView);
				if (pClass != NULL){
					mTrackView = (CForecastRadarGraphView*)(pClass->CreateObject());
					if (mTrackView != NULL){
						mTrackView->SetViewtype(eRadarViewType_Station);
						mTrackView->SetStationID(theApp.GetSelectStation());
						mTrackView->Create(NULL, NULL, WS_VISIBLE | WS_CLIPCHILDREN | WS_CHILD, rectTab, &mTabList[i].tab, 0, NULL);
						mTrackView->SendMessage(WM_INITIALUPDATE);
						mTabList[i].pview[item] = mTrackView;
						mTabList[i].tab.AddTab(mTabList[i].pview[item], mTabItemID20[i][item].name, 0, FALSE);
						mTabList[i].itemCount++;
					}
				}
				break;
			}
			case	eViewType_SummaryGraph:
				if (!mSummary[mTabItemID20[i][item].id - 1].Create(&mTabList[i].tab, rectTab, eSummaryCtrl + (mTabItemID20[i][item].id - 1), theApp.GetSelectStation(), eViewType1_EquipmentStatus)){
					TRACE("Failed view creation\n");
					continue;
				}
				mTabList[i].pwnd[item] = &mSummary[mTabItemID20[i][item].id - 1];
				mTabList[i].tab.AddTab(mTabList[i].pwnd[item], mTabItemID20[i][item].name, 0, FALSE);
				mTabList[i].itemCount++;
				mSummary[mTabItemID20[i][item].id - 1].PostMessage(eMessage_InitSummaryWnd);
				break;

			case	eViewType_SummaryGraphProperties:
				if (!mSummaryProperties[mTabItemID20[i][item].id - 1].Create(&mTabList[i].tab, CSummaryPropertiesDlg::IDD, mTabItemID20[i][item].id - 1, theApp.GetSelectStation(), eViewType1_EquipmentStatus)){
					TRACE("Failed view creation\n");
					continue;
				}
				CSummaryProperties* pProp = new CSummaryProperties(mTabItemID20[i][item].id - 1, theApp.GetSelectStation(), eViewType1_EquipmentStatus, mTabItemID20[i][0].name);
				if (pProp == NULL) {
					TRACE("Failed properties creation\n");
					continue;
				}
				mTabList[i].pwnd[item] = &mSummaryProperties[mTabItemID20[i][item].id - 1];
				mTabList[i].tab.AddTab(mTabList[i].pwnd[item], mTabItemID20[i][item].name, 0, FALSE);
				mTabList[i].tab.SetTabType(CMFCEqTabCtrl::eTabType_SummaryGraph);
				mTabList[i].tab.SetProperties(pProp);
				mTabList[i].itemCount++;
				break;
			}
			pos++;
		}
		mTabList[i].tab.Initialize();
	}
}


LRESULT CEquipmentStatusView20::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_ThreadNormal:
	{
		// 監視データが更新されたため、ビューに更新を通知
		for (int i = 0; i < mSummaryGraphMax; i++){
			mSummary[i].PostMessage(message, wParam, lParam);
		}
		mEqStatusView->PostMessage(message, wParam, lParam);
		vector<CWnd*>::iterator itr;		mControlList;
		for (itr = mControlList.begin(); itr != mControlList.end(); itr++)
		{
			(*itr)->PostMessage(message, wParam, lParam);
		}
		break;
	}
	case eMessage_SummaryPropetyChange:
	{
		// サマリプロパティ変更通知
		if (mSummary[lParam].GetSafeHwnd() != NULL)
		{
			mSummary[lParam].PostMessage(message, wParam, lParam);
		}
		break;
	}
	case eMessage_InitSummaryWnd:
	{
		// サマリグラフ初期化通知
		if (mSummary[lParam].GetSafeHwnd() != NULL)
		{
			mSummary[lParam].PostMessage(message, wParam, lParam);
		}
		break;
	}
	case eMessage_NewEngValData:
	{
		// 新しい工学値変換を受けた通知
		if (mSummary[lParam].GetSafeHwnd() != NULL)
		{
			mSummary[lParam].PostMessage(message, wParam, lParam);
		}
		break;
	}
	default:
		return CFormView::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}

/*============================================================================*/
/*! サマリグラフ

-# レイアウト読み込み（設定読み込み）

@param  なし
@retval	なし

*/
/*============================================================================*/
bool CEquipmentStatusView20::Load(CArchive& ar)
{
	for (int i = 0; i < mSummaryGraphMax; i++){
		mSummary[i].Load(ar);
	}
	return true;
}

/*============================================================================*/
/*! サマリグラフ

-# レイアウト保存（設定保存）

@param  なし
@retval	なし

*/
/*============================================================================*/
bool CEquipmentStatusView20::Save(CArchive& ar)
{
	for (int i = 0; i < mSummaryGraphMax; i++){
		mSummary[i].Save(ar);
	}
	return true;
}
