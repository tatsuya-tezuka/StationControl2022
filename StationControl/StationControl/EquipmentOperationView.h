#pragma once

#include "GridCtrl.h"
#include "TrackingCtrl.h"
#include "SummaryGraphCtrl.h"
#include "ForecastRadarGraphView.h"

// CEquipmentOperationView ビュー

class CEquipmentOperationView : public CView
{
	DECLARE_DYNCREATE(CEquipmentOperationView)

protected:
	CEquipmentOperationView();           // 動的生成で使用される protected コンストラクター
	virtual ~CEquipmentOperationView();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	static	const UINT mSummaryGraphMax = 4;
protected:
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CGridCtrl			mGridEquipment;
	CGridCtrl			mGridOperation;
	CTrackingCtrl		mTrack;
	CForecastRadarGraphView*	mpTrackView;
	CSummaryGraphCtrl	mSummary[mSummaryGraphMax];
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */

	// 属性
public:
	CStationControlDoc* GetDocument() const;

public:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドされます。
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

#ifndef _DEBUG  // StationControlView.cpp のデバッグ バージョン
inline CStationControlDoc* CEquipmentOperationView::GetDocument() const
{
	return reinterpret_cast<CStationControlDoc*>(m_pDocument);
}
#endif
