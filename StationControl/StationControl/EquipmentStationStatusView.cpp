// EquipmentStationStatusView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "EquipmentStationStatusView.h"


// CEquipmentStationStatusView

IMPLEMENT_DYNCREATE(CEquipmentStationStatusView, CFormView)

CEquipmentStationStatusView::CEquipmentStationStatusView()
	: CFormView(CEquipmentStationStatusView::IDD)
{

}

CEquipmentStationStatusView::~CEquipmentStationStatusView()
{
	for (int i = 0; i < eTab5Max; i++){
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

void CEquipmentStationStatusView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATUS_LOC, mLocStatus);
	DDX_Control(pDX, IDC_OPERATION_LOC, mLocOperation);
	DDX_Control(pDX, IDC_TARGET_LOC, mLocTarget);
	DDX_Control(pDX, IDC_GRAPH1_LOC, mLocGraph1);
	DDX_Control(pDX, IDC_GRAPH2_LOC, mLocGraph2);
	DDX_Control(pDX, IDC_GRAPH3_LOC, mLocGraph3);
	DDX_Control(pDX, IDC_GRAPH4_LOC, mLocGraph4);
}

BEGIN_MESSAGE_MAP(CEquipmentStationStatusView, CFormView)
END_MESSAGE_MAP()


// CEquipmentStationStatusView 診断

#ifdef _DEBUG
void CEquipmentStationStatusView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CEquipmentStationStatusView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
CStationControlDoc* CEquipmentStationStatusView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CEquipmentStationStatusView メッセージ ハンドラー

void CEquipmentStationStatusView::OnInitialUpdate()
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

	createTab();
}

/*============================================================================*/
/*! 設備状況ビュー

-# 各種タブコントロールの作成

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentStationStatusView::createTab()
{
	int nWndID = GetViewID(this);
	if (nWndID < 0)
		return;

	mControlList.clear();
	CRect rectTab;
	int pos = 0;
	for (int i = 0; i < eTab5Max; i++){
		if (mTab5ItemID[i][0].id == 0)
			break;
		GetDlgItem(mTab5LocationID[i])->GetWindowRect(&rectTab);
		ScreenToClient(&rectTab);
		mTabList[i].tab.Create(CMFCTabCtrl::STYLE_3D, rectTab, this, 1, CMFCTabCtrl::LOCATION_TOP);

		UINT item = 0;
		mTabList[i].itemCount = 0;
		for (item = 0; item < MAX_TABITEM; item++){
			if (mTab5ItemID[i][item].id == 0)
				break;
			switch (mTab5ItemID[i][item].type){
			case	eViewType_Dialog:
				if (mTab5ItemID[i][item].pset != NULL){
					CDialogEx *pdlg = (CDialogEx*)(mTab5ItemID[i][item].pset->CreateObject());
					if (!pdlg->Create(mTab5ItemID[i][item].id, &mTabList[i].tab)){
						TRACE("Failed view creation\n");
						continue;
					}
					mTabList[i].tab.AddTab(pdlg, mTab5ItemID[i][item].name, 0, FALSE);
					mTabList[i].itemCount++;
					mControlList.push_back(pdlg);
				}
				else{
					if (!mTabList[i].dlg[item].Create(mTab5ItemID[i][item].id, &mTabList[i].tab)){
						TRACE("Failed view creation\n");
						continue;
					}
					mTabList[i].tab.AddTab(&mTabList[i].dlg[item], mTab5ItemID[i][item].name, 0, FALSE);
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
						mTabList[i].tab.AddTab(mTabList[i].pview[item], mTab5ItemID[i][item].name, 0, FALSE);
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
						mTrackView->SetStationID(nWndID - eViewType5_EqOperation1);
						mTrackView->Create(NULL, NULL, WS_VISIBLE | WS_CLIPCHILDREN | WS_CHILD, rectTab, &mTabList[i].tab, 0, NULL);
						mTrackView->SendMessage(WM_INITIALUPDATE);
						mTabList[i].pview[item] = mTrackView;
						mTabList[i].tab.AddTab(mTabList[i].pview[item], mTab5ItemID[i][item].name, 0, FALSE);
						mTabList[i].itemCount++;
					}
				}
				break;
			}
			case	eViewType_SummaryGraph:
				if (!mSummary[mTab5ItemID[i][item].id - 1].Create(&mTabList[i].tab, rectTab, eSummaryCtrl + (mTab5ItemID[i][item].id - 1), nWndID - eViewType5_EqOperation1, nWndID)){
					TRACE("Failed view creation\n");
					continue;
				}
				mTabList[i].pwnd[item] = &mSummary[mTab5ItemID[i][item].id - 1];
				mTabList[i].tab.AddTab(mTabList[i].pwnd[item], mTab5ItemID[i][item].name, 0, FALSE);
				mTabList[i].itemCount++;
				mSummary[mTab5ItemID[i][item].id - 1].PostMessage(eMessage_InitSummaryWnd);
				break;

			case	eViewType_SummaryGraphProperties:
				if (!mSummaryProperties[mTab5ItemID[i][item].id - 1].Create(&mTabList[i].tab, CSummaryPropertiesDlg::IDD, mTab5ItemID[i][item].id - 1, nWndID - eViewType5_EqOperation1, nWndID)){
					TRACE("Failed view creation\n");
					continue;
				}
				CSummaryProperties* pProp = new CSummaryProperties(mTab5ItemID[i][item].id - 1, nWndID - eViewType5_EqOperation1, nWndID, mTab5ItemID[i][0].name);
				if (pProp == NULL) {
					TRACE("Failed properties creation\n");
					continue;
				}
				mTabList[i].pwnd[item] = &mSummaryProperties[mTab5ItemID[i][item].id - 1];
				mTabList[i].tab.AddTab(mTabList[i].pwnd[item], mTab5ItemID[i][item].name, 0, FALSE);
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


LRESULT CEquipmentStationStatusView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
/*! CEquipmentStationStatusView

-# 自分のビューIDを取得する

@param	pView：ビューポインタ
@retval	ウィンドウID

*/
/*============================================================================*/
int CEquipmentStationStatusView::GetViewID(CView* pView)
{
	for (int i = eViewType5_EqOperation1; i <= eViewType5_EqOperation5; i++)
	{
		if (pView == theApp.GetView(i))
		{
			return i;
		}
	}

	return -1;
}


/*============================================================================*/
/*! サマリグラフ

-# レイアウト読み込み（設定読み込み）

@param  なし
@retval	なし

*/
/*============================================================================*/
bool CEquipmentStationStatusView::Load(CArchive& ar)
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
bool CEquipmentStationStatusView::Save(CArchive& ar)
{
	for (int i = 0; i < mSummaryGraphMax; i++){
		mSummary[i].Save(ar);
	}
	return true;
}
