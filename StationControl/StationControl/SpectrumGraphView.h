#pragma once

/*******************************************************************************
* データプロット関連定義
*******************************************************************************/
#define GRID_MIN_X  (  60 )
#define GRID_MIN_Y  (  15 )
#define GRID_MAX_X  ( rectLine.Width() + GRID_MIN_X )
#define GRID_MAX_Y  ( rectLine.Height() + GRID_MIN_Y )
#define GRID_NUM_X  ( 10 )
#define GRID_NUM_Y  ( 16 )

// CSpectrumGraphView ビュー

class CSpectrumGraphView : public CView
{
	DECLARE_DYNCREATE(CSpectrumGraphView)

public:
	CSpectrumGraphView();
	virtual ~CSpectrumGraphView();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	enum eGridType{
		eGridType_DOT,
		eGridType_LINE,
		eGridType_NONE
	};

	static const int m_nVAxisWidth = 30;
	static const int m_nVAxisHeightMargin = 10;
	double m_dVAxisMaxMinMargin = 0.05;
	double m_dVRange = 1000.0;
	static const int m_nVerDivMax = 25;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	COLORREF    m_gl_bk_color;				/* 背景 */
	COLORREF    m_gl_glid_line_color;		/* グリッド線 */
	COLORREF    m_gl_Y_fig_color;			/* Y軸文字 */
	COLORREF    m_gl_X_fig_color;			/* X軸文字*/
	COLORREF    m_gl_plot_color;			/* プロット */
	COLORREF    m_gl_peak_color;			/* ピークマーク */
	COLORREF    m_gl_time_color;			/* 時刻表示 */

	COLORREF    m_gl_bk_color_b;			/* 背景 */
	COLORREF    m_gl_glid_line_color_b;		/* グリッド線 */
	COLORREF    m_gl_Y_fig_color_b;			/* Y軸文字 */
	COLORREF    m_gl_X_fig_color_b;			/* X軸文字*/
	COLORREF    m_gl_plot_color_b;			/* プロット */
	COLORREF    m_gl_peak_color_b;			/* ピークマーク */
	COLORREF    m_gl_time_color_b;			/* 時刻表示 */

	COLORREF    m_gl_bk_color_w;			/* 背景 */
	COLORREF    m_gl_glid_line_color_w;		/* グリッド線 */
	COLORREF    m_gl_Y_fig_color_w;			/* Y軸文字 */
	COLORREF    m_gl_X_fig_color_w;			/* X軸文字*/
	COLORREF    m_gl_plot_color_w;			/* プロット */
	COLORREF    m_gl_peak_color_w;			/* ピークマーク */
	COLORREF    m_gl_time_color_w;			/* 時刻表示 */

protected:
	UINT		m_nHorzDiv;			// 横分割数
	UINT		m_nVerDiv;			// 縦分割数
	int			m_nGrid;			// グリッド種別

	long		m_gl_x_grid;		/* グリッド間隔X */
	long		m_gl_y_grid;		/* グリッド間隔Y */

	CFont		m_txtFont;			// テキストフォント
	double		m_dAutoVMax;		// オートスケール縦軸最大値
	double		m_dAutoVMin;		// オートスケール縦軸最小値
	double		m_dVMax;			// 縦軸最大値
	double		m_dVMin;			// 縦軸最小値

	UINT		m_nGraphType;		// グラフタイプ
	UINT		m_nStationIdx;		// 局番号
	UINT		m_nViewIdx;			// ビューインデックス

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドされます。
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	void InitializeView(UINT nStationIdx, UINT nViewIdx, UINT nGraphType)	{
		m_nStationIdx = nStationIdx;
		m_nViewIdx = nViewIdx;
		m_nGraphType = nGraphType;
	}
	void SetStationIdx(UINT nStationIdx)	{ m_nStationIdx = nStationIdx; }
	void SetViewIdx(UINT nViewIdx)			{ m_nViewIdx = nViewIdx; }
	void SetGraphType(UINT nGraphType)		{ m_nGraphType = nGraphType; }
	void ClearAutoVMaxMin()
	{
		m_dAutoVMax = -DBL_MAX;
		m_dAutoVMin = DBL_MAX;
	}

public:
	CSpectrumViewData* GetSpectrumViewData();
	UINT GetGraphType()			{ return m_nGraphType; }

protected:
	BOOL DrawGrid(CDC& memDC, CRect& rectClient);
	BOOL DrawSpectrum(CDC& memDC, CRect& rectClient);

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnPaint();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	/* ------------------------------------------------------------------------------------ */
};


