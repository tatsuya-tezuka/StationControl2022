/*============================================================================*/
/*! 設備・運用状況

-#

@param
@retval

*/
/*============================================================================*/
// EquipmentOperationView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "EquipmentOperationView.h"


// CEquipmentOperationView

IMPLEMENT_DYNCREATE(CEquipmentOperationView, CView)

CEquipmentOperationView::CEquipmentOperationView()
{
}

CEquipmentOperationView::~CEquipmentOperationView()
{
}

BEGIN_MESSAGE_MAP(CEquipmentOperationView, CView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CEquipmentOperationView 描画

void CEquipmentOperationView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 描画コードをここに追加してください。
}


// CEquipmentOperationView 診断

#ifdef _DEBUG
void CEquipmentOperationView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CEquipmentOperationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
CStationControlDoc* CEquipmentOperationView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CEquipmentOperationView メッセージ ハンドラー


void CEquipmentOperationView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	GetDocument()->SetTitleEx();

	// クライアント領域取得
	CRect rect;
	GetClientRect(rect);

	// 設備状態の作成
	int w = rect.Width() / 2;
	int h = rect.Height() / 3;
	mGridEquipment.SetTitle(CString(mTitleEquipment));
	mGridEquipment.SetGridNum(CPoint(5,4));
#ifdef _DEBUG
	for (int x = 0; x < 5; x++){
		for (int y = 0; y < 4; y++){
			mGridEquipment.AddItem(x, y, mEqString[y][x], RGB(0, 128, 255));
		}
	}
#endif
	mGridEquipment.Create(this, rect, eGridCtrl_Equipment);

	// 運用状態の作成
	mGridOperation.SetTitle(CString(mTitleOperation));
	mGridOperation.SetGridNum(CPoint(5, 4));
#ifdef _DEBUG
	for (int x = 0; x < 5; x++){
		for (int y = 0; y < 4; y++){
			mGridOperation.AddItem(x, y, mOpeString[y][x], RGB(0,128,255));
		}
	}
#endif
	mGridOperation.Create(this, rect, eGridCtrl_Operation);

	// 追尾グラフの作成
	//mTrack.Create(this, rect, eTrackingCtrl);
	CRuntimeClass* pClass = RUNTIME_CLASS(CForecastRadarGraphView);
	if (pClass == NULL)
	{
		return;
	}

	mpTrackView = (CForecastRadarGraphView*)(pClass->CreateObject());
	if (mpTrackView != NULL)
	{
		mpTrackView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, 0, NULL);
		mpTrackView->SendMessage(WM_INITIALUPDATE);
	}

	// サマリグラフの作成
	for (int i = 0; i < mSummaryGraphMax / 2; i++){
		for (int j = 0; j < mSummaryGraphMax / 2; j++){
			mSummary[i*(mSummaryGraphMax / 2) + j].Create(this, rect, theApp.GetSelectStation(), eSummaryCtrl + (i*(mSummaryGraphMax / 2) + j));
		}
	}

}


void CEquipmentOperationView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// クライアント領域の取得
	CRect rect, client;
	GetClientRect(client);
	
	int w, h;
	w = client.Width();
	h = client.Height() / 3;

	if (mGridEquipment.GetSafeHwnd()){
		// 幅：全体
		// 高さ：全体高さの1/3
		rect = client;
		w = rect.Width();
		h = rect.Height() / 3;
		rect.bottom = rect.top + h;
		mGridEquipment.MoveWindow(rect);
	}
	if (mGridOperation.GetSafeHwnd()){
		// 幅：全体
		// 高さ：全体高さの1/3
		rect = client;
		w = rect.Width();
		h = rect.Height() / 3;
		rect.top = rect.top + h;
		rect.bottom = rect.top + h;
		mGridOperation.MoveWindow(rect);
	}
	//if (mTrack.GetSafeHwnd()){
	//	// 幅：全体幅の1/2
	//	// 高さ：全体高さの1/3
	//	rect = client;
	//	w = rect.Width() / 2;
	//	h = rect.Height() / 3;
	//	rect.right = rect.left + w;
	//	rect.top = rect.top + (h * 2);
	//	rect.bottom = rect.top + h;
	//	mTrack.MoveWindow(rect);
	//}
	if (mpTrackView->GetSafeHwnd()){
		// 幅：全体幅の1/2
		// 高さ：全体高さの1/3
		rect = client;
		w = rect.Width() / 2;
		h = rect.Height() / 3;
		rect.right = rect.left + w;
		rect.top = rect.top + (h * 2);
		rect.bottom = rect.top + h;
		mpTrackView->MoveWindow(rect);
	}
	if (mSummary[0].GetSafeHwnd()){
		// 幅：全体幅の1/2
		// 高さ：全体高さの1/3
		rect = client;
		CRect srect = rect;
		srect.left = srect.left + w;
		srect.right = srect.left + w;
		srect.top = srect.top + (h * 2);
		srect.bottom = srect.top + h;
		int sw = w / 2;
		int sh = h / 2;
		for (int i = 0; i < mSummaryGraphMax / 2; i++){
			rect = srect;
			if (i == 0){
				rect.right = rect.left + sw;
			}
			else{
				rect.left = rect.left + sw;
			}
			for (int j = 0; j < mSummaryGraphMax / 2; j++){
				if (j == 0){
					rect.bottom = rect.top + sh;
				}
				else{
					rect.top = rect.bottom;
					rect.bottom = rect.top + sh;
				}
				mSummary[i*(mSummaryGraphMax / 2) + j].MoveWindow(rect);
			}
		}

	}
}


LRESULT CEquipmentOperationView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_ThreadNormal:
	{
		// 監視データが更新されたため、ビューに更新を通知
		for (int i = 0; i < mSummaryGraphMax; i++){
			mSummary[i].UpdateGraph();
		}
		break;
	}
	default:
		return CView::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}
