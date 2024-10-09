// EquipmentStatusView54.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "EquipmentStatusView54.h"


// CEquipmentStatusView54

IMPLEMENT_DYNCREATE(CEquipmentStatusView54, CFormView)

CEquipmentStatusView54::CEquipmentStatusView54()
	: CFormView(CEquipmentStatusView54::IDD)
{

}

CEquipmentStatusView54::~CEquipmentStatusView54()
{
	for (int i = 0; i < eTab54Max; i++){
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

void CEquipmentStatusView54::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANTENNA_LOC, mLocAntenna);
	DDX_Control(pDX, IDC_XSEND_LOC, mLocXsend);
	DDX_Control(pDX, IDC_XRECV_LOC, mLocXrecv);
	DDX_Control(pDX, IDC_KARECV_LOC, mLocKarecv);
	DDX_Control(pDX, IDC_DSNMODEM_LOC, mLocDsnmodem);
	DDX_Control(pDX, IDC_XMEAS_LOC, mLocXmeas);
	DDX_Control(pDX, IDC_STATUS_LOC, mLocStatus);
	DDX_Control(pDX, IDC_TARGET_LOC, mLocTarget);
	DDX_Control(pDX, IDC_GRAPH1_LOC, mLocGraph1);
	DDX_Control(pDX, IDC_GRAPH2_LOC, mLocGraph2);
	DDX_Control(pDX, IDC_GRAPH3_LOC, mLocGraph3);
	DDX_Control(pDX, IDC_GRAPH4_LOC, mLocGraph4);
}

BEGIN_MESSAGE_MAP(CEquipmentStatusView54, CFormView)
END_MESSAGE_MAP()


// CEquipmentStatusView54 診断

#ifdef _DEBUG
void CEquipmentStatusView54::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CEquipmentStatusView54::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
CStationControlDoc* CEquipmentStatusView54::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CEquipmentStatusView54 メッセージ ハンドラー

void CEquipmentStatusView54::OnInitialUpdate()
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
void CEquipmentStatusView54::createTab()
{
	mControlList.clear();
	CRect rectTab;
	int pos = 0;
	for (int i = 0; i < eTab54Max; i++){
		if (mTabItemID54[i][0].id == 0)
			break;
		GetDlgItem(mTabLocationID54[i])->GetWindowRect(&rectTab);
		ScreenToClient(&rectTab);
		mTabList[i].tab.Create(CMFCTabCtrl::STYLE_3D, rectTab, this, 1, CMFCTabCtrl::LOCATION_TOP);

		UINT item = 0;
		mTabList[i].itemCount = 0;
		for (item = 0; item < MAX_TABITEM; item++){
			if (mTabItemID54[i][item].id == 0)
				break;
			switch (mTabItemID54[i][item].type){
			case	eViewType_Dialog:
				if (mTabItemID54[i][item].pset != NULL){
					CDialogEx *pdlg = (CDialogEx*)(mTabItemID54[i][item].pset->CreateObject());
					if (!pdlg->Create(mTabItemID54[i][item].id, &mTabList[i].tab)){
						TRACE("Failed view creation\n");
						continue;
					}
					mTabList[i].tab.AddTab(pdlg, mTabItemID54[i][item].name, 0, FALSE);
					mTabList[i].itemCount++;
					mControlList.push_back(pdlg);
				}
				else{
					if (!mTabList[i].dlg[item].Create(mTabItemID54[i][item].id, &mTabList[i].tab)){
						TRACE("Failed view creation\n");
						continue;
					}
					mTabList[i].tab.AddTab(&mTabList[i].dlg[item], mTabItemID54[i][item].name, 0, FALSE);
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
						mTabList[i].tab.AddTab(mTabList[i].pview[item], mTabItemID54[i][item].name, 0, FALSE);
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
						mTabList[i].tab.AddTab(mTabList[i].pview[item], mTabItemID54[i][item].name, 0, FALSE);
						mTabList[i].itemCount++;
					}
				}
				break;
			}
			case	eViewType_SummaryGraph:
				if (!mSummary[mTabItemID54[i][item].id - 1].Create(&mTabList[i].tab, rectTab, eSummaryCtrl + (mTabItemID54[i][item].id - 1), theApp.GetSelectStation(), eViewType1_EquipmentStatus)){
					TRACE("Failed view creation\n");
					continue;
				}
				mTabList[i].pwnd[item] = &mSummary[mTabItemID54[i][item].id - 1];
				mTabList[i].tab.AddTab(mTabList[i].pwnd[item], mTabItemID54[i][item].name, 0, FALSE);
				mTabList[i].itemCount++;
				mSummary[mTabItemID54[i][item].id - 1].PostMessage(eMessage_InitSummaryWnd);
				break;

			case	eViewType_SummaryGraphProperties:
				if (!mSummaryProperties[mTabItemID54[i][item].id - 1].Create(&mTabList[i].tab, CSummaryPropertiesDlg::IDD, mTabItemID54[i][item].id - 1, theApp.GetSelectStation(), eViewType1_EquipmentStatus)){
					TRACE("Failed view creation\n");
					continue;
				}
				CSummaryProperties* pProp = new CSummaryProperties(mTabItemID54[i][item].id - 1, theApp.GetSelectStation(), eViewType1_EquipmentStatus, mTabItemID54[i][0].name);
				if (pProp == NULL) {
					TRACE("Failed properties creation\n");
					continue;
				}
				mTabList[i].pwnd[item] = &mSummaryProperties[mTabItemID54[i][item].id - 1];
				mTabList[i].tab.AddTab(mTabList[i].pwnd[item], mTabItemID54[i][item].name, 0, FALSE);
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


LRESULT CEquipmentStatusView54::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
bool CEquipmentStatusView54::Load(CArchive& ar)
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
bool CEquipmentStatusView54::Save(CArchive& ar)
{
	for (int i = 0; i < mSummaryGraphMax; i++){
		mSummary[i].Save(ar);
	}
	return true;
}
