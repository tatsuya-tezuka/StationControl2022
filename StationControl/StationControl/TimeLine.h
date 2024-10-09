#pragma once

#include <vector>
#include "../StationControl/SatelliteData.h"
#include "DetailViewDlg.h"

using namespace std;

#define _TIMEBREAK	6
#define _FIXEDLABEL_WIDTH	200
#define _BACKHEIGHT_RATE	3

enum{
	eTimeScale_3,
	eTimeScale_6,
	eTimeScale_12,
	eTimeScale_24,
};

static const DWORD mTimeScale[] = {
	3, 8, 12, 24, 48, 72
};
static const DWORD mTimeRange[] = {
	30, 80, 120, 240, 480, 720
};

typedef struct {
	COLORREF	color;
	CPen		pen;
} stTimeLineObject;

typedef struct {
	stTimeLineObject base;			// 基本カラー
	stTimeLineObject light;			// 基本カラーの+50
	stTimeLineObject lighter;		// 基本カラーの+100
	stTimeLineObject lightest;		// 基本カラーの+150
	stTimeLineObject dark;			// 基本カラーの-50
	stTimeLineObject darker;		// 基本カラーの-100
	stTimeLineObject darkShadow;	// 3D オブジェクトの暗い影の色
	stTimeLineObject liteShadow;	// 3D オブジェクトの影の色 (光源の反対方向の縁用)
	stTimeLineObject shadow;		// 影の色 (縁用)
} stTimeLineColor;

enum{
	eColor_Background,		// 背景
	eColor_FixedCell,		// 固定セル背景
	eColor_Grid,			// グリッド
	eColor_GridCell,		// グリッド

	eColor_Forecast,		// 予報値
	eColor_ForecastUsed,	// 予報値(使用中)
	eColor_OpePlan,			// 運用計画
	eColor_OpePlanExecute,	// 運用計画実行中
	eColor_OpePlanNormal,	// 運用計画正常終了
	eColor_OpePlanAbnormal,	// 運用計画異常終了

	eColor_PrePass,			// プリパス
	eColor_PostPass,		// ポストパス

	eColor_OpePlanEnp,		// パス（ENP）
	eColor_OpePlanCalib,	// 計画校正診断
	eColor_OpeStationOccupy,// 局占有
	eColor_OpeVlbi,			// VLBI

	eColor_Base,

	eColor_MAX,
};
static const COLORREF mOpeColorTable[] = {
	RGB(255, 255, 255),
	RGB(200, 255, 255),// RGB(255, 210, 180),
	RGB(0, 0, 0),
	RGB(192, 192, 192),

	RGB(192, 192, 192),
	RGB(128, 0, 128),
	RGB(247, 189, 172),
	RGB(0, 0, 255),
	RGB(50, 255, 50),
	RGB(255, 50, 50),

	RGB(255, 255, 0),
	RGB(255, 255, 0),

	RGB(153, 217, 234),
	RGB(136, 0, 21),
	RGB(0, 0, 0),
	RGB(80, 80, 80),

	RGB(192, 192, 192),
};

enum{
	ePoint_Forecast = 0x01,
	ePoint_Operation = 0x02,
};

enum{
	ePlanStatus_None,
	ePlanStatus_PlanRegist,
	ePlanStatus_PlanInit,
	ePlanStatus_Execute,
	ePlanStatus_NormalEnd,
	ePlanStatus_AbnormalEnd,
};

enum{
	eKanshiPlanStatus_OutPass,
	eKanshiPlanStatus_InPass,
	eKanshiPlanStatus_PlanRegist,
	eKanshiPlanStatus_PlanInit,
};

enum{
	ePassKind_Pred,
	ePassKind_Pass,
	ePassKind_PrePass,
	ePassKind_PostPass,
};

enum{
	eLineType_Run,			// 運用計画パス中
	eLineType_Plan,			// 運用計画登録前
	eLineType_PlanRegist,	// 運用計画登録中
	eLineType_PlanInit,		// 運用計画初期設定中
	eLineType_Pred,			// 予報値
	eLineType_Overlap,		// 実行中運用計画と同じ衛星、かつ時間帯が重なる予報値
	eLineType_UnexecRdsn,	// 未実行校正診断
	eLineType_execRdsn,		// 実行中校正診断
	eLineType_Occu,			// 局占有登録前
	eLineType_OccuRegist,	// 局占有登録中
	eLineType_OccuInit,		// 局占有初期設定中
	eLineType_OccuRun,		// 局占有パス中
	eLineType_Vlbi,			// VLBI登録前
	eLineType_VlbiRegist,	// VLBI登録中
	eLineType_VlbiInit,		// VLBI初期設定中
	eLineType_VlbiRun,		// VLBIパス中
	eLineType_Empty,		// 空きエリア
};

#define SCROLL_VALUE 1000.0 
#define CTLOFFSET 0
#define WM_NEWHORZLIMITS (CTLOFFSET + WM_USER + 701)
#define WM_NEWVERTLIMITS (CTLOFFSET + WM_USER + 702)
#define MAX_TIMEDIV		9

#define		OperationTime_Forecast			12		// 予報値：（現在日時から12時間前）ダミー
#define		OperationTime_Initialize		120		// 初期設定：120秒
#define		OperationTime_AntennaWait		1500	// ANT待ち受け指向　1500(臼田) 60+360(内之浦)
#define		OperationTime_AntennaWait2		420		// ANT待ち受け指向　1500(臼田) 60+360(内之浦)
#define		OperationTime_AntennaZenith		900		// ANT天頂指向 900(臼田) 60(内之浦)
#define		OperationTime_AntennaZenith2	60		// ANT天頂指向 900(臼田) 60(内之浦)
#define		OperationTime_Agc				420		// AGC校正 420
#define		OperationTime_Rng				120		// RNG校正 120
#define		OperationTime_Tlm				300		// TLM伝送 300
#define		OperationTime_Cmd				300		// CMD伝送 300

typedef struct  {
	TCHAR		satellite[30];	// 衛星名
	TCHAR		predid[20];		// 予報値ID
	TCHAR		cid[20];		// CID
	TCHAR		planid[20];		// 計画ID
	__int64		start;			// 開始日時
	__int64		end;			// 終了日時
	COLORREF	textcolor;		// 文字色
	COLORREF	color;			// 色
	UINT		stationID;		// 局ID
	UINT		getkind;		// 取得
	UINT		opekind;		// 運用種別
	UINT		kind;			// パス種別
	UINT		code;			// 終了コード
	LPARAM		getform;		// 取得先 
	CString		maintcomment;	// MAINT用コメント
} stTimeLine;

typedef struct  {
	TCHAR		satellite[30];					// 衛星名
	map< __int64, vector<stTimeLine> > item;	// 運用タイムラインアイテム(キー：計画ID)
} stTimeData;

typedef struct  {
	TCHAR		satellite[30];					// 衛星名
	TCHAR		passId[20];						// パスID
	__int64		aos;							// 予報値開始日時
	__int64		los;							// 予報値終了日時
	__int64		maxelTime;						// Max EL 日時
	double		maxel;							// Max EL
	UINT		station;						// 局番号
	UINT		flag;							// 0x01=VLBI 0x02=MAINT
} stForecastData;

typedef struct  {
	UINT		id;
	TCHAR		satellite[30];					// 衛星名
} stSatelliteData;

typedef struct  {
	TCHAR		satellite[30];					// 衛星名
	TCHAR		passId[20];						// パスID
	RECT		rect;
	RECT		itemrect;
	UINT		type;							// 運用タイプ
	DWORD		getform;						// 取得先
	UINT		station;						// 局番号
} stDrawRect;

enum{
	eBaseSpace,
	eBaseStation,
};
static const CString mButtonString[] = {
	_T("衛星名ベース"),
	_T("局名ベース"),
};

// CTimeLine

class CTimeLine : public CWnd
{
	DECLARE_DYNAMIC(CTimeLine)

public:
	CTimeLine();
	virtual ~CTimeLine();

	// スクロール
	CSize  m_pageSize;
	CSize  m_displaySize;
	CSize  m_scrollPos;
	CSize  m_windowSize;

	void	SetDisplaySize(int displayWidth, int displayHeight, bool bSync = false);
	const CSize& GetDisplaySize() const;
	const CSize& GetScrollPos() const;
	const CSize& GetPageSize() const;
	void GetClientRectScroll(CRect& rect);
	int GetScrollPosEx(int bar, CScrollBar* pScrollBar);
	void	SetScrollSize(CSize page, CSize disp, CSize window)
	{
		m_pageSize = page;
		m_displaySize = disp;
		m_windowSize = window;
		UpdateScrollInfo();
	}

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	CWnd	*m_pParentWnd;
	CFont	mTxtFont, mTimeFont;
	int		mTimeScaleIndex;
	CBitmap	mBitmap;
	bool	mAutoScrol, mForward, mNext;
	int		mLabelCelTopPos;
	int		mLabelCelHeight;
	int		mTimeLineMaxHeight;
	bool	mHeader;
	COprPlnData		mTimelineData;
	CTime	mForecastStartTime, mForecastStopTime;
	bool	mTimeBreak;
	CTime	mCurrentTime;
	SYSTEMTIME	mStartTime;
	int		mRedrawCursorPos;
	UINT	mSelectBase;
	CTimeLine	*mSyncTimeLine;

	// 詳細表示画面
	CDetailViewDlg m_DetailDlg;

	stTimeLineColor			mColors[eColor_MAX];
	vector<stTimeData>		mTimeDataList;
	vector<stForecastData>	mForecastDataList;
	vector<stDrawRect>		mForecastRect;
	vector<stDrawRect>		mOperationRect;
	vector<stDrawRect>		mSatelliteRect;
	vector<stSatelliteData>	mSatelliteData;

	//CMFCToolTipCtrl			mToolTip;
	CToolTipCtrl			mToolTip;
	vector<__int64>			mToolTipId;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	int			drawCurrentTime(CDC& memDC);
	void		drawTimeLine(CDC& memDC);
	void		drawTimeLineBaee(CDC& memDC);
	void		drawTimeLineBaeeSalellite(CDC& memDC, CRect rectForecast, CString satname, UINT stationno);

	void		createColors(stTimeLineColor &colors);
	COLORREF	lightenColor(const COLORREF crColor, BYTE blend);
	COLORREF	darkenColor(const COLORREF crColor, BYTE blend);
	void		createPens(stTimeLineColor &colors);
	void		deletePens(stTimeLineColor &colors);
	void		drawTimeline(CDC& memDC, const CRect rect, COLORREF color, UINT status = ePlanStatus_None);
	int			CalculateTimeToX(CTime startTime, CRect startRect, CTime time);
	void		UpdateScrollInfo(bool bSync = false);
	UINT		HitTestEx(CPoint& point, CString& satellite, CString& planid, UINT& flag, LPARAM& getform, CTime& time);
	bool		IsHitTestFuture(CPoint& point, time_t curTime);
	CRect		GetPredRect(const CString satellite, const CString predID, UINT nStation);
	UINT		GetDuplicatePlan(const CString satellite, const CString predID, UINT nStationID = 0xffffffff);
	CRect		GetDrawPredRect(vector<stDrawRect> temp, CString satellite, CString predid, CRect rectDefault, int cellHeight);
	CRect		GetDrawPlanRect(CRect rectDefault, int cellHeight);
	bool		IsDuplicatePlanTime(const CString satellite, __int64 cid, UINT current, UINT nStationID = 0xffffffff);
	bool		IsDuplicatePlanTime(const CString satellite, vector<stTimeLine>& list, __int64 saveStart, __int64 saveEnd);
	__int64		GetPlanTotalTime(const CString satellite, __int64 cid, __int64& start, UINT nStationID = 0xffffffff);
	bool		IsStationSatellite(const UINT stationID, const CString satellite);
	void		DrawForecast(CDC& memDC, const CString satellite, const CString passId, const CTime startTime, const CRect rectTime, CRect& outTime);
	bool		DrawForecastStation(CDC& memDC, const CString satellite, const CString passId, const CTime startTime, const CRect rectTime, UINT station, CRect& outTime);
	bool		DrawOperationStation(CDC& memDC, const CString satellite, const CString passId, const CTime startTime, const CRect rectTime, UINT station, CRect& outTime);

	void		UpdatePrePassTime();

	void		AddToolTip(stTimeLine& data, CRect rect);
	void		AddToolTipPred(stForecastData& data, CRect rect);
	void		ClearToolTip(bool bRemoveAll=false);
	UINT		getOperationType(UINT opeKind);

public:
	void	ClearTimeData()
	{
		ClearToolTip(true);
		mTimeDataList.clear();
	}
	void	ClearForecastData()	{ mForecastDataList.clear(); }
	void	SetForecastData(stForecastData& data)	{ mForecastDataList.push_back(data); }
	void	SortForecastData();
	void	ClearSatelliteData()	{ mSatelliteData.clear(); }
	void	SetSatelliteData(TCHAR* satname);
	UINT	GetSatelliteID(TCHAR* satname);

	BOOL	Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void	SetTimeScaleIndex(int val)	{ mTimeScaleIndex = val; }
	void	AutoScrol(bool val)
	{
		mAutoScrol = val;
		//if (mAutoScrol == false)
		//	GetLocalTime(&mStartTime);
		//if (mBitmap.GetSafeHandle()){
		//	mBitmap.DeleteObject();
		//}
		ClearToolTip(true);
		Invalidate();
	}
	void	Forward(){
		//if (mBitmap.GetSafeHandle()){
		//	mBitmap.DeleteObject();
		//}
		mForward = true;
		ClearToolTip(true);
		Invalidate();
	}
	void	Next()
	{
		//if (mBitmap.GetSafeHandle()){
		//	mBitmap.DeleteObject();
		//}
		mNext = true;
		ClearToolTip(true);
		Invalidate();
	}
	void	SetHeader()		{ mHeader = true; }
	bool	SetTimeLineData(const CString satellite, const CString planid, stPlanListData data, bool bRedraw = true);
	void	GetCID(CString strPlan, CString& cid);

	CString	ConvertPredfileToId(CString name);
	void	SetForecastTime(CTime fstart, CTime fend)
	{
		mForecastStartTime = fstart;
		mForecastStopTime = fend;
	};
	void	SetCurrentTime(CTime time)
	{
		if (mAutoScrol == false){
			mTimeBreak = true;
			return;
		}
		mCurrentTime = time;
		if (mTimeBreak == true || mStartTime.wYear == 0){
			mTimeBreak = false;
			ClearToolTip(true);
			mCurrentTime.GetAsSystemTime(mStartTime);
		}
	}
	void	RedrawGraph(bool bUpdate = true)
	{
		if (bUpdate == false){
			Invalidate();
			return;
		}
		//if (mBitmap.GetSafeHandle()){
		//	mBitmap.DeleteObject();
		//}
		Invalidate();
		UpdateScrollInfo();
	};

	vector<stTimeData>&		GetTimeDataList()	{ return mTimeDataList; }
	vector<stForecastData>&	GetForecastDataList() { return mForecastDataList; }

	UINT	GetSelectBase()			{ return mSelectBase; }
	void	SetSelectBase(UINT val) { mSelectBase = val; }
	void	SetSyncTimeLine(CTimeLine *val) { mSyncTimeLine = val; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};



//=============================================================================
// CGroupBoxNote
//=============================================================================
//!< 拡張グループボックスクラス
class CGroupBoxNote : public CButton
{
	DECLARE_DYNAMIC(CGroupBoxNote)

public:
	CGroupBoxNote();
	virtual ~CGroupBoxNote();

	enum eGroupType{
		eGroupType_None,
		eGroupType_Normal,
		eGroupType_Round,
		eGroupType_Box,
		eGroupType_BoxLine,
		eGroupType_Ribbon,
		eGroupType_Max,
	};

	void	SetTextColor(COLORREF color)	{ m_colorText = color; }
	void	SetBackColor(COLORREF color)	{ m_colorBack = color; }
	void	SetGroupType(eGroupType type)
	{
		m_typeGroup = type;
		if (type == eGroupType_Normal)
			m_colorBack = ::GetSysColor(COLOR_3DFACE);
	}
	void	SetRoundPoint(CPoint pt)		{ m_pointRound = pt; }
	void	SetFontBold(bool val)			{ m_bFontBold = val; }
	void	UpdateRect();

protected:
	CString		m_strText;
	CRect		m_rect;
	CPoint		m_pointRound;

	COLORREF	m_colorText;
	COLORREF	m_colorBack;
	eGroupType	m_typeGroup;
	bool		m_bFontBold;

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	afx_msg void OnPaint();
};


// CNotesButton

class CNotesButton : public CButton
{
	DECLARE_DYNAMIC(CNotesButton)

public:
	CNotesButton();
	virtual ~CNotesButton();

	void	SetNotesType(int val)	{ mNotesType = val; }

protected:
	int		mNotesType;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};
