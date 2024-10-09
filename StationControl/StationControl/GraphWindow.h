#pragma once
#include "afxwin.h"

#include "GraphException.h"

using namespace std;

#include "DrawObject.h"
#include "DrawTool.h"
#include "const.h"

#include "SatelliteData.h"

enum eGraphType{
	eGraphType_All = 0,
	eGraphType_Trend,
	eGraphType_OffAll,
	eGraphType_OffTrend,
	eGraphType_Summary,
};

enum eViewID{
	eViewID_1 = 0,
	eViewID_2,
	eViewID_3,
	eViewID_Num,
};

enum eGraphControl
{
	mCtrlTrend = 0x00000100L,
};

enum eMessage
{
	mMessageOpenFile = WM_USER + 100,
};

#define SCROLL_VALUE 1000.0 
#define CTLOFFSET 0
#define WM_NEWHORZLIMITS (CTLOFFSET + WM_USER + 701)
#define WM_NEWVERTLIMITS (CTLOFFSET + WM_USER + 702)

#define HINT_UPDATE_WINDOW      0
#define HINT_UPDATE_DRAWOBJ     1
#define HINT_UPDATE_SELECTION   2
#define HINT_DELETE_SELECTION   3
#define HINT_UPDATE_OLE_ITEMS   4

#define HSCALE_NUM	15

//#define MAXDIGIT	(1 + 9 + 2 + 1)		// マイナス +  整数部 + カンマ + 文字列終わり 
#define MAXDIGIT	(1 + 13 + 4 + 1)		// マイナス +  整数部 + カンマ + 文字列終わり 
#define MAXDIGIT14	(1 + 14 + 4 + 1)		// マイナス +  整数部 + カンマ + 文字列終わり

#define AUTO_V_TOPBOTMARGIN		0.05
#define AUTO_V_TOPMARGIN		(1.0 + AUTO_V_TOPBOTMARGIN)
#define AUTO_V_BOTMARGIN		(1.0 - AUTO_V_TOPBOTMARGIN)

#define PPLine1st()	{ \
		bIsLastMaxValue = TRUE; \
		val = m_TrendDataPP[nID].spaceData[pos].ppPair.min; \
		pnt.first = (double)xpos; pnt.second = val; \
		pointDraw = ConvPoint(pnt, m_dGridScaleLMax, m_dGridScaleLMin); \
		memDC.MoveTo(pointDraw); \
		val = m_TrendDataPP[nID].spaceData[pos].ppPair.max; \
		pnt.second = val; \
		pointDraw = ConvPoint(pnt, m_dGridScaleLMax, m_dGridScaleLMin); \
		memDC.LineTo(pointDraw); \
	}

#define PPLineMinToMax()	{ \
		bIsLastMaxValue = TRUE; \
		val = m_TrendDataPP[nID].spaceData[pos].ppPair.min; \
		pnt.first = (double)xpos; pnt.second = val; \
		pointDraw = ConvPoint(pnt, m_dGridScaleLMax, m_dGridScaleLMin); \
		memDC.LineTo(pointDraw); \
		val = m_TrendDataPP[nID].spaceData[pos].ppPair.max; \
		pnt.second = val; \
		pointDraw = ConvPoint(pnt, m_dGridScaleLMax, m_dGridScaleLMin); \
		memDC.LineTo(pointDraw); \
	}

#define PPLineMaxToMin()	{ \
		bIsLastMaxValue = FALSE; \
		val = m_TrendDataPP[nID].spaceData[pos].ppPair.max; \
		pnt.first = (double)xpos; pnt.second = val; \
		pointDraw = ConvPoint(pnt, m_dGridScaleLMax, m_dGridScaleLMin); \
		memDC.LineTo(pointDraw); \
		val = m_TrendDataPP[nID].spaceData[pos].ppPair.min; \
		pnt.second = val; \
		pointDraw = ConvPoint(pnt, m_dGridScaleLMax, m_dGridScaleLMin); \
		memDC.LineTo(pointDraw); \
	}

#define PPEllipse()	{ \
		r = (*itr).second.nDotSize / 2; \
		val = m_TrendDataPP[nID].spaceData[pos].ppPair.min; \
		pnt.first = (double)xpos; pnt.second = val; \
		pointDraw = ConvPoint(pnt, m_dGridScaleLMax, m_dGridScaleLMin); \
		memDC.Ellipse(pointDraw.x - r, pointDraw.y - r, pointDraw.x + r, pointDraw.y + r); \
		val = m_TrendDataPP[nID].spaceData[pos].ppPair.max; \
		pnt.second = val; \
		pointDraw = ConvPoint(pnt, m_dGridScaleLMax, m_dGridScaleLMin); \
		memDC.Ellipse(pointDraw.x - r, pointDraw.y - r, pointDraw.x + r, pointDraw.y + r); \
	}

class CGraphWindow : public CWnd
{
public:
	CGraphWindow();
	virtual ~CGraphWindow();

public:
	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
	typedef struct  {
		CString		strName;		// データ名
		CString		strGName;		// データ名（グラフ表示名）
		CString		strUnit;		// 単位
		int			nDecDidits;		// 少数点以下の桁数
		DWORD		nDataId;		// データID
		int			nListIdx;		// リストインデックス
		int			nDrawKind;		// 描画モード
		COLORREF	crColor;		// 線色
		int			nStyle;			// 線スタイル
		int			nWidth;			// 線幅
		int			nDotSize;		// ドットサイズ
		BOOL		bIsVisible;		// 線表示の有無
		double		nMaxValue;		// データの最大値
		double		nMinValue;		// データの最小値
		UINT		nGraphKind;		// グラフ種別
		VScaleKind	nVScaleKind;	// 縦軸スケール選択（右 / 左）
	} structLine;	// ライン属性構造体

	typedef struct {
		structLine* stLine;
		double max;
		double min;
//		double curMax;
//		double curMin;
	} structPPMinMax;	// PP圧縮　最小最大構造体

	typedef struct {
		deque<stSpaceData>	spaceData;		// 衛星データ
		int					srcType;		// データ元 0:工学値変換後データ, 1:ファイル
	} structTrendDataPP;	// トレンドPP圧縮データ構造体

protected:
	typedef map<UINT, structLine> DRAWLINE;

	static const int mRightMargin = 10;
	static const int mTopMargin = 5;
	static const int mBottomMargin = 5;
	static const UINT mZoomWidth = 5;
	static const COLORREF mZoomColor = RGB(0, 255, 0);

	static const UINT mGridMarginLeft = 90;
//	static const UINT mGridMarginRight = 216;
	static const UINT mGridMarginRight = 220;
	static const UINT mGridMarginTop = 28;
	static const UINT mGridMarginBottom = 28;
	static const UINT mDataNameMargine = 10;
	static const UINT mLabelLeftMargine = 2;
	static const UINT mLabelRightMargine = 2;
	static const UINT mTimeLabel2ndMargine = 10;
	static const UINT mHStrDistance = 2;
	static const UINT mLegendLineStrDistance = 8;
	static const UINT mVStrDistance = 2;
	static const UINT mViewWindowIDMarginTop = 2;
	static const UINT mAutoStrLeftMargin = 5;
	static const int mMaxRange = 200 * 24* 3600;
	static const UINT mSigStrHeight = 18;
	static const UINT mLegendWeidth = 10;

	static const UINT mBtnWidth = 60;
	static const UINT mBtnHeight = 20;
	static const UINT mChkBtnWidth = 100;
	static const UINT mChkBtnHeight = 20;
	static const UINT mChkBtnMarginRight = 5;
	static const UINT mBtnSettingMarginRight = 2;
	static const UINT mBtnSettingMarginTop = 2;
	static const UINT mBtnIntgMarginLeft = 5;
	static const UINT mBtnIntgMarginRight = 20;
	static const UINT mBtnIntgMarginBottom = 2;
	static const int mIntgStrMarginWidth = 20;
	static const int mUnitMarginLeft = 10;
	static const int mUnitMarginTop = 24;

	static const UINT mSizePP = 60;

	static const int mDrawDetailWidth = 300;
	static const int mDrawDetailHeight = 200;

	static const int mLegendWidth = 65;

	static const int mCurValueWidth = 92;
	static const int mCurValueHeight = 16;

	static const UINT mIntegFrameTopOffsetUdsc64 = 76;
	static const UINT mIntegFrameTopOffsetUsc34 = 40;
	static const UINT mIntegFrameBotOffset = 2;

public:
	enum eGridType{
		eGridType_DOT,
		eGridType_LINE,
		eGridType_NONE
	};

	enum eDotSize{
		eDotSize_SMALL = 4,
		eDotSize_MEDIUM = 6,
		eDotSize_LARGE = 8
	};

	static UINT mHScale[HSCALE_NUM];

	static const UINT mHDivNormal = 10;
	static const UINT mHDivSmall = 5;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CDrawObjList	m_selection;
	CDrawObjList	m_objects;

protected:
	// ◆共通
	DRAWLINE			m_DrawLines;		// 線ID毎の情報map
	COLORREF			m_ColorBk;			// 背景色
	COLORREF			m_ColorGrid;		// グリッド色
	COLORREF			m_ColorText;		// テキスト色
	COLORREF			m_ColorLighttGrid;	// 薄いグリッド色
	BOOL				m_bLabel;			// ラベル表示
	UINT				m_nHorzDiv;			// 横分割数
	BOOL				m_bUseVAxis[MAX_VSCALE];	// 縦軸使用の有無

	CWnd				*m_pParentWnd;		// 親ウィンドウ
	CFont				m_txtFontSmall;		// テキストフォント（スモール）
	CFont				m_txtFontNormal;	// テキストフォント（ノーマル）

	UINT				m_nGraphType;		// グラフ種類（オンライン・オフライン描画識別のためのグラフタイプ）
	UINT				m_nViewID;			// ビューID（メッセージ識別のためのビュー番号）
	UINT				m_nPropertiesID;	// 設定ID（AGC/RNG/RR/EL識別のための設定番号）
	CString				m_strViewName;		// ビュー名
	int					m_nGrid;			// グリッド種別
	int					m_CursorPos;		// カーソル位置
	CPoint				m_ZoomPos;			// ゾームカーソル位置

	stVScale			m_nStvScale;		// 縦軸スケール

	// ◆トレンドグラフ
	int					m_nTrendInc;		// トレンド増分値
	double				m_pointTop;			// 描画領域の先頭位置
	double				m_dbMaxHzVal;		// 全データの最大値
	double				m_dbMinHzVal;		// 全データの最小値
	double				m_dbHzRange;		// 描画領域のデータレンジ：デフォルト0-1000
	double				m_dbTmpHzRange;		// 描画領域のデータレンジテンポラリ
	UINT				m_nTrandHScale;		// トレンド横軸スケール種類

	CPoint				m_ptZoom[2];

	double				m_dTotalScaleLMax;	// 左縦軸に割り当てられたデータの最大
	double				m_dTotalScaleLMin;	// 左縦軸に割り当てられたデータの最小
	double				m_dGridScaleLMax;	// グリッドスケール最大
	double				m_dGridScaleLMin;	// グリッドスケール最小

	// 測定データ
	structTrendDataPP	m_TrendDataPP[MAX_TDATA];	// トレンドPPデータ
	
	__int64			m_nTrendDataPos;				// トレンドデータ圧縮位置
//	__int64			m_nTrendDelPos;					// トレンドデータ削除位置
//	__int64			m_nTrendPPDelPos;				// トレンドPP圧縮データ削除位置
	__int64			m_nQueuLastTotalSize[MAX_TDATA];// 直近のキュートータルサイズ

	__int64			m_PPNumber[MAX_TDATA];		// PP圧縮数
	__int64			m_PPTotalCount[MAX_TDATA];	// PP圧縮合計カウント
	structPPMinMax	m_stPPMinMax;				// PP圧縮最小値最大値

	BOOL			m_bIsRealTime;				// リアルタイム
//	BOOL			m_bDelPPHead;				// PP領域の先頭を削除
//	BOOL			m_bDelPPHeadAfter;			// PP領域の先頭を削除

	BOOL			m_bFirstDraw;				// 初回描画
	BOOL			m_bIsDrawOn[MAX_TDATA];		// 描画ON

	CPoint			m_ptTrendMove;
	UINT			m_nViewNo;

	CButton			m_btnSetting;				// 設定ボタン
	BOOL			m_bViewDetail;				// 詳細表示

	CButton			m_btnHGridChk;				// 横罫線チェックボタン
	BOOL			m_bHGridOn;					// 横罫線表示ON

	CButton			m_btnIntg;					// 積分時間ボタン
	int				m_nIntgTime;				// 積分時間(1～300)

	CRect			m_rectIntgBtn;				// 積分ボタン領域

	time_t			m_StartTime;				// 開始時刻
	time_t			m_EndTime;					// 終了時刻
	time_t			m_MostOldTime;				// もっとも古い時刻
	time_t			m_MostNewTime;				// もっとも新しい時刻
	BOOL			m_bFitYohochi;				// 予報値に合わせる
	time_t			m_yohoOldTime;				// 古い予報値時刻（開始）
	time_t			m_yohoNEwTime;				// 新しい予報値時刻（終了）

	__int64			m_nTrendDataEndPos;			// データ終わり位置

	double			m_curVal[MAX_TDATA];		// 現在値

	int				m_StationIndex;

	CRect			m_rectCurValArea;			// 現在値領域
	CButton			m_chkBtnViewSig[MAX_TDATA];		// 信号表示チェックボタン(54m以外)
	CStatic			m_txtSigName[MAX_TDATA];		// 信号名(54m)

	BOOL			m_bExitData;				// データ存在
	BOOL			m_bStopDraw;				// 描画停止

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	BOOL	Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, BOOL bVisibleSettingBtn = FALSE);

	void	SetGraphType(UINT val)	{ m_nGraphType = val; }
	void	SetViewID(UINT val)		{ m_nViewID = val; }
	void	SetPropertiesID(UINT val)	{ m_nPropertiesID = val; }
	void	SetBackColor(COLORREF val)	{ m_ColorBk = val; }
	void	SetGridColor(COLORREF val)	{ m_ColorGrid = val; }
	void	SetGridType(int val)	{ m_nGrid = val; }
	void	SetRealTime(BOOL val)	{ m_bIsRealTime = val; }

	void	SetStationIndex(int index)	{ m_StationIndex = index; }

	BOOL	GetFitYohochi()			{ return m_bFitYohochi; }
	void	SetFitYohochi(BOOL bFitYohochi)	{ m_bFitYohochi = bFitYohochi; }
	BOOL	GetStopDraw()			{ return m_bStopDraw; }

	BOOL	AddLineToMap(UINT nID, structLine line);

	BOOL	AddLine(UINT nID, CPropertiesData* pParam, UINT nGraphKind, DWORD nStyle);
	BOOL	AddDot(UINT nID, CPropertiesData* pParam, UINT nGraphKind);
	BOOL	AddLineAndDot(UINT nID, CPropertiesData* pParam, UINT nGraphKind, DWORD nStyle);
	BOOL	AddAxis(stVScale nStvScale, HScale trandHScale);

	void	AddSatelliteData();
	time_t	FitHRangeAndPPDataSize(time_t t_old, time_t t_new);
	void	DeletePPData();
	time_t	SelectStartTime(time_t t);

	void	UpdateMaxMin();

	double	GetVAxisMax();
	double	GetVAxisMin();

	void	ClearLineAndDot(UINT nID = 0xffffffff);
	void	UpdateGraph(BOOL bAll = FALSE);
	time_t	GetMostOldTime()	{ 
		if (m_bExitData)
			return m_MostOldTime;
		else
			return 0;
	}
	void	SetOldTime(time_t mostOldTime)	{ m_MostOldTime = mostOldTime; }
	time_t	GetMostNewTime()	{ return m_MostNewTime; }
	void	SetStartTime(time_t t)		{ 
		m_StartTime = t; 
		m_EndTime = t + (time_t)m_dbHzRange;
	}


	// トレンド解析ズーム用のメンバ変数＆関数
	void		SetTrendZoomInc(int val)		{ m_nTrendInc = val; }
	int			GetTrendZoomInc()				{ return m_nTrendInc; }
	void		SetHzRange(double val)			{ m_dbHzRange = val; }

	UINT	SetViewNo(int val) { m_nViewNo = (UINT)val; }

	void	DocToClient(CRect& rect);
	void	DocToClient(CPoint& point);
	void	ClientToDoc(CPoint& point);
	void	ClientToDoc(CRect& rect);
	void	InvalObj(CDrawObject* pObj);
	void	Deselect(CDrawObject* pObj);
	CDrawObject*	ObjectAt(const CPoint& point);
	BOOL	IsSelected(const CObject* pDocItem) const;
	void	Select(CDrawObject* pObj, BOOL bAdd = FALSE);
	void	Select(ObjectShape type);
	void	SelectWithinRect(CRect rect, BOOL bAdd = FALSE);
	void	Add(CDrawObject* pObj);
	void	Remove(CDrawObject* pObj);
	void	Remove(ObjectShape type);
	void	RemoveAll(bool bInit = true);
	void	OnUpdate(CView*, LPARAM lHint, CObject* pHint);
	void	DrawObj(CDC* pDC);
	void	ResetCursor(int pos);

protected:
	CPoint	ConvPoint(pair<double, double> pnt, double max, double min) { return ConvPoint(pnt.first, pnt.second, max, min); }
	CPoint	ConvPoint(double X, double Y, double max, double min);
	CPoint	ConvPoint(double X);
	BOOL	DrawTrendName(CDC& memDC, CRect& RectClient);
	BOOL	DrawGrid(CDC& memDC, CRect& RectClient);
	void	DrawCursor(CDC& memDC);
	void	DrawGraph(CDC& memDC);
	void	DrawTrend(CDC& memDC);
	void	DrawTrendContents(CDC& memDC, int nSrcType);
	void	DrawOffTrend(CDC& memDC);
	BOOL	IsViewDetail(CRect& rect);

	void	MutexLock()
	{
		CGraphMutex::Lock(eDrawTrend);
	}

	void	MutexUnlock()
	{
		CGraphMutex::Unlock(eDrawTrend);
	}

	void	DrawZoom(CDC* pDC, CRect drect);
	void	ClearCursor()
	{
		m_ptZoom[0] = CPoint(-1, -1);
		m_ptZoom[1] = CPoint(-1, -1);
	}

	void	UpdateScrollInfo();
	void	Normalize();
	void	SendNewLimits();
	void	OnZoom(bool bZoomIn);

	// ドットサイズを取得する
	int GetDotSize(int nDotSize);

	CString addCanma(double pos, int pointKeta = 0);
	double Ceil3Keta(double pos, int dir);
	double CeilKeta(double pos, int dir, double range);

	void ConvertPP(int trace, deque<stSpaceData>& trendPP, stSpaceData newData, structPPMinMax& stPPMinMax);
	void ConvertPP(int trace, deque<stSpaceData>& trendPP, stSpaceData newData);
	void DeleteHalfPP(int trace, deque<stSpaceData>& trendPP);
	void DeleteHeadPP(int trace, deque<stSpaceData>& trendPP, int srcType);
	void InitSatelliteDataSetting();
	void CalcPPMinMaxWithCompareCount(deque<stSpaceData>& trendPP, __int64 number, structPPMinMax& stPPMinMax);
	void CalcPPMinMax(stSpaceData& sData, structPPMinMax& stPPMinMax);
	__int64 GetPPDataMaxSize();
	void BnClickedSigOnBtn(UINT nSig);

	void UpdateIntegKanshi();

	void AjustLayout();

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	afx_msg void OnBnClickedSettingBtn();
	afx_msg void OnBnClickedHGridOnBtn();
	afx_msg void OnBnClickedSigOnBtn0();
	afx_msg void OnBnClickedSigOnBtn1();
	afx_msg void OnBnClickedSigOnBtn2();
	afx_msg void OnBnClickedSigOnBtn3();
	afx_msg void OnBnClickedSigOnBtn4();
	afx_msg void OnBnClickedSigOnBtn5();
	afx_msg void OnBnClickedSigOnBtn6();
	afx_msg void OnBnClickedSigOnBtn7();
	afx_msg void OnBnClickedSigOnBtn8();
	afx_msg void OnBnClickedSigOnBtn9();
	afx_msg void OnBnClickedSigOnBtn10();
	afx_msg void OnBnClickedSigOnBtn11();
	afx_msg void OnBnClickedSigOnBtn12();
	afx_msg void OnBnClickedIntgTimeBtn();
};

