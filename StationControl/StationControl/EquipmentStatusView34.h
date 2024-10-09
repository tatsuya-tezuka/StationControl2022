#pragma once

#include <vector>
using namespace std;

#include "MFCEqTabCtrl.h"
#include "TabAntennaView34.h"
#include "TabXsendView.h"
#include "TabXrecvView.h"
#include "TabXmeasView.h"
#include "EqStatusGridView.h"
#include "ForecastRadarGraphView.h"
#include "SummaryGraphCtrl.h"
#include "SummaryPropertiesDlg.h"

// CEquipmentStatusView フォーム ビュー

class CEquipmentStatusView34 : public CFormView
{
	DECLARE_DYNCREATE(CEquipmentStatusView34)

public:
	CEquipmentStatusView34();   // 標準コンストラクター
	virtual ~CEquipmentStatusView34();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_EQSTATUS34 };

	// 属性
public:
	CStationControlDoc* GetDocument() const;

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

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
	CFont				mTabFont;
	SIZE				mSizeTotal, mSizePage, mSizeLine;
	stTabInfo			mTabList[eTabMax];
	CEqStatusGridView*	mEqStatusView;
	CForecastRadarGraphView*	mTrackView;
	CSummaryGraphCtrl	mSummary[mSummaryGraphMax];
	CSummaryPropertiesDlg	mSummaryProperties[mSummaryGraphMax];
	vector<CWnd*>		mControlList;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	// レイアウト読み込み＆保存（設定情報）
	bool	Load(CArchive& ar);
	bool	Save(CArchive& ar);
protected:
	void	createTab();
	void	createTab34();
	void	createTab20();
	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CStatic mLocAntenna;
	CStatic mLocXsend;
	CStatic mLocXrecvNew;
	CStatic mLocXrecvOld;
	CStatic mLocXmeas;
	CStatic mLocStatus;
	CStatic mLocTarget;
	CStatic mLocGraph1;
	CStatic mLocGraph2;
	CStatic mLocGraph3;
	CStatic mLocGraph4;
	virtual void OnInitialUpdate();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

#ifndef _DEBUG  // EquipmentStatusView.cpp のデバッグ バージョン
inline CStationControlDoc* CEquipmentStatusView34::GetDocument() const
{
	return reinterpret_cast<CStationControlDoc*>(m_pDocument);
}
#endif
