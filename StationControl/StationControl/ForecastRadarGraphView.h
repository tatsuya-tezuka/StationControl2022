#pragma once

#include "SatelliteData.h"

#define		ON	1
#define		OFF	0

// CForecastRadarGraphView ビュー

class CForecastRadarGraphView : public CView
{
	DECLARE_DYNCREATE(CForecastRadarGraphView)

protected:
	CForecastRadarGraphView();           // 動的生成で使用される protected コンストラクター
	virtual ~CForecastRadarGraphView();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	enum eDrawType
	{
		eDrawType_Before = 0,
		eDrawType_After
	};

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CTrackingRadarCtrlMonData	m_ctrlMonData;		// 運用状況監視データ

	CPoint		m_ptCenter;			// 円の中心
	double		m_r;				// 円の半径
	int			m_i_plot_flag;		// ＡＯＳ色でのプロット実施フラグ

	int			m_i_into_flag = OFF;	/* パス開始３０分以上前から３０分以内に入ったかどうかのフラグ */
	long		m_l_latest_time = UNYOU_TIME + 1;	/* 前回のカウントダウン値 */

	int			m_nViewType;		// 表示タイプ		
	int			m_nStationID;		// 局ID
	int			m_nCtrlMonDataID;	// データID

	int			m_dummyCnt;			// ダミーカウンタ
//	time_t		m_dummyInitTime;	// ダミー初期時刻

	CFont		m_txtFontSmall;		// テキストフォント（スモール）
	CFont		m_txtFontNormal;	// テキストフォント（ノーマル）

	CString		m_strPlanID;		// 計画ID
	__int64		m_tPassStart;		// 運用開始時刻
	__int64		m_tPassEnd;			// 運用終了時刻

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドされます。

	void SetViewtype(int nViewType)			{ m_nViewType = nViewType; }
	void SetStationID(int nStationID)		{ m_nStationID = nStationID; }
	void Init(CString fpath, CString satellite, CString predId);
	void InitGtunyouData(CString fpath, CString& satellite, CString& predId);
	void InitDummyGtunyouData(CString fpath); 
	void SetPlanID(CString strPlanID, __int64 tPassStart, __int64 tPassEnd)		{ m_strPlanID = strPlanID; m_tPassStart = tPassStart; m_tPassEnd = tPassEnd; }

protected:
	BOOL DrawRadarFrame(CDC& memDC, CRect& rectClient);
	BOOL DrawRadarFrame(CDC& memDC, CRect& rectClient, int drawType);
	BOOL DrawSkyline(CDC& memDC, CRect& rectClient);
	BOOL DrawRfTrip(CDC& memDC, CRect& rectClient);
	BOOL DrawForecast(CDC& memDC, CRect& rectClient);
	BOOL DrawTrakingSaveData(CDC& memDC, CRect& rectClient);
	void GtazelToXY(double d_Az, double d_El, double d_CX, double d_CY, double d_RD, int* i_OX, int* i_OY);

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

protected:
	void AddDummyData();
	void UpdateGraphData();

public:
	virtual void OnInitialUpdate();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	/* ------------------------------------------------------------------------------------ */
};


