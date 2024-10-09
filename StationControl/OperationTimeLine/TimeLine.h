#pragma once

using namespace std;

#define _FIXEDLABEL_WIDTH	200

enum{
	eTimeScale_3,
	eTimeScale_6,
	eTimeScale_12,
	eTimeScale_24,
};

static const DWORD mTimeScale[] = {
	3, 6, 12, 24,
};
static const DWORD mTimeRange[] = {
	30, 60, 120, 240,
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
	eColor_Pass,			// パス運用
	eColor_Forecast,		// 予報値

	eColor_ReadinessCheck,	// レディネスチェック
	eColor_EqInitialize,	// 設備初期化設定
	eColor_Antenna,			// アンテナ待ち受け指向
	eColor_DownlinkRecv,	// ダウンリンク受信
	eColor_PostPass,		// ポストパス

	eColor_UplinkkSend,		// アップリンク送信
	eColor_Command,			// コマンド運用
	eColor_Range,			// レンジ運用
	eColor_UsedForecast,	// 使用中予報値

	eColor_MAX,
};
static const COLORREF mOpeColorTable[] = {
	RGB(255, 255, 255),
	RGB(200, 255, 255),
	RGB(0, 0, 0),
	RGB(192, 192, 192),
	RGB(217, 245, 80),
	RGB(217, 217, 217),

	RGB(152, 208, 222),
	RGB(146, 208, 80),
	RGB(242, 130, 38),
	RGB(217, 217, 217),
	RGB(112, 48, 160),

	RGB(255, 255, 0),
	RGB(0, 176, 240),
	RGB(255, 192, 0),
	RGB(176, 66, 219),
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

enum{
	eTimeKind_Master,
	eTimeKind_DownLink,
	eTimeKind_UpLink,
	eTimeKind_Max,
};

static const CString mTimeLabel[] = {
	_T("マスター計画"), _T("ダウンリンク"), _T("アップリンク"),
};


typedef struct  {
	CString		label;
	CTime		start;		// 開始日時
	CTime		end;		// 終了日時
	COLORREF	color;
	bool		updown;		// [初期設定]と[ANT待ち受け指向通知]がONの場合にtrue
} stTimeLine;

static const CString mEventMode[] = {
	_T("自動実行"),
	_T("手動"),
};

static const CString mOptSendDoppler[] = {
	_T("なし（センター）"),
	_T("なし（ベストロック）"),
	_T("なし（ノミナル）"),
	_T("あり"),
};
static const CString mOptSendOutput[] = {
	_T("HPA-20kw"),
	_T("HPA-2kw"),
	_T("HPA-200w"),
	_T("SSPA-100w"),
	_T("SSPA-10w"),
};
static const CString mOptANTZenith[] = {
	_T("なし"),
	_T("あり（LOCKあり）"),
	_T("あり（LOCKなし）"),
};
static const CString mCIDOptANTZenith[] = {
	_T("なし"),
	_T("天頂(LOCK)"),
	_T("天頂(UNLOCK)"),
	_T("天頂"),
};
static const CString mControlOptANTZenith[] = {
	_T("DISABLE"),
	_T("STOW_LOCK"),
	_T("STOW_UNLOCK"),
};
static const CString mMasterSlabe[] = {
	_T("主局"),
	_T("従局"),
};
static const CString mMasterSlabePlan[] = {
	_T("MASTER"),
	_T("SLAVE"),
};

static const CString mPlanKind[] = {
	_T("待機中"),
	_T("プリパス中"),
	_T("パス中"),
	_T("ポストパス中"),
};
enum{
	ePlanKind_None,
	ePlanKind_PrePass,
	ePlanKind_Pass,
	ePlanKind_PostPass,
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

	void	SetDisplaySize(int displayWidth, int displayHeight);
	const CSize& GetDisplaySize() const;
	const CSize& GetScrollPos() const;
	const CSize& GetPageSize() const;
	void GetClientRectScroll(CRect& rect);
	int GetScrollPosEx(int bar, CScrollBar* pScrollBar);

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	CWnd	*m_pParentWnd;
	CFont	mTxtFont, mTimeFont;
	CBitmap	mBitmap;
	int		mTimeScaleIndex;
	bool	mTimeBreak;
	//bool	mAutoScrol, mForward, mNext;
	CTime	mCurrentTime;
	SYSTEMTIME	mStartTime;
	int		mItemPos;
	int		mItemMaxPoint;
	int		mLabelCelTopPos;
	int		mLabelCelHeight;
	int		mItemMaxCount;
	bool	mHeader;
	int		mRedrawCursorPos;
	COprPlnData		mTimelineData;
	COprPlnData		mClearTimelineData;
	CTime	mForecastStartTime, mForecastStopTime;
	UINT	mForecastSelect;
	UINT	mUplinkSelect;
	UINT	mCmdSelect;
	UINT	mRngSelect;

	stTimeLineColor	mColors[eColor_MAX];

public:
	// 予報値一覧データ
	vector<stPredListData>		mListPredData;
	// 計画一覧データ
	vector<stPlanListData>		mListPlanData;
	// 運用計画データ
	vector<COprPlnData>			mListTimeData;

	int m_nUplnkEvent;			// アップリンク 自動:0 / 手動:1
	int m_nCmdEvent;			// CMD 自動:0 / 手動:1
	int m_nRngEvent;			// RNG 自動:0 / 手動:1

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	int			drawCurrentTime(CDC& memDC);
	void		drawTimeLine(CDC& memDC);
	void		drawForecast(CDC& memDC);
	void		createColors(stTimeLineColor &colors);
	COLORREF	lightenColor(const COLORREF crColor, BYTE blend);
	COLORREF	darkenColor(const COLORREF crColor, BYTE blend);
	void		createPens(stTimeLineColor &colors);
	void		deletePens(stTimeLineColor &colors);
	void		drawTimeline(CDC& memDC, const CRect rect, COLORREF color);
	int			CalculateTimeToX(CTime startTime, CRect startRect, CTime time);
	void		UpdateScrollInfo();
	void		SendNewLimits();

public:
	BOOL	Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void	SetTimeScaleIndex(int val)	{ mTimeScaleIndex = val; Invalidate(); }
	void	UpdateTime() { Invalidate();}
	void	SetCurrentTime(CTime time)
	{
		mCurrentTime = time;
		mCurrentTime.GetAsSystemTime(mStartTime);
		if (mTimeBreak == true || mStartTime.wYear == 0){
			mTimeBreak = false;
			mCurrentTime.GetAsSystemTime(mStartTime);
		}
	}
	//void	AutoScrol(bool val)
	//{
	//	mAutoScrol = val;
	//	if (mAutoScrol == false)
	//		GetLocalTime(&mStartTime);
	//	Invalidate();
	//}
	//void	Forward()		{ mForward = true; Invalidate(); }
	//void	Next()			{ mNext = true; Invalidate(); }
	void	SetHeader()		{ mHeader = true; }
	void	SetTimeLineData(COprPlnData data);
	void	ClearTimeLineData();
	void	SetForecastTime(CTime fstart, CTime fend, UINT nSelect)
	{
		mForecastStartTime = fstart;
		mForecastStopTime = fend;
		mForecastSelect = nSelect;
	};
	void	SetSelectForecast(UINT nSelect)
	{
		mForecastSelect = nSelect;
		if (mBitmap.GetSafeHandle()){
			mBitmap.DeleteObject();
		}
		Invalidate();
		UpdateScrollInfo();
	};
	UINT	GetSelectUplink() { return mUplinkSelect; }
	void	SetSelectUplink(UINT nSelect, bool bDraw = false)
	{
		mUplinkSelect = nSelect;
		if (bDraw == false)
			return;
		if (mBitmap.GetSafeHandle()){
			mBitmap.DeleteObject();
		}
		Invalidate();
		UpdateScrollInfo();
	};
	UINT	GetSelectCmd() { return mCmdSelect; }
	void	SetSelectCmd(UINT nSelect, bool bDraw = false)
	{
		mCmdSelect = nSelect;
		if (bDraw == false)
			return;
		if (mBitmap.GetSafeHandle()){
			mBitmap.DeleteObject();
		}
		Invalidate();
		UpdateScrollInfo();
	};
	UINT	GetSelectRng() { return mRngSelect; }
	void	SetSelectRng(UINT nSelect, bool bDraw = false)
	{
		mRngSelect = nSelect;
		if (bDraw == false)
			return;
		if (mBitmap.GetSafeHandle()){
			mBitmap.DeleteObject();
		}
		Invalidate();
		UpdateScrollInfo();
	};
	void	RedrawGraph()
	{
		if (mBitmap.GetSafeHandle()){
			mBitmap.DeleteObject();
		}
		Invalidate();
		UpdateScrollInfo();
	};

	void	ResetScroll()
	{
		m_scrollPos.cy = 0;
		SetScrollPos(SB_VERT, m_scrollPos.cy, TRUE);
	};

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};



//=============================================================================
// CTimeGroupBoxEx
//=============================================================================
//!< 拡張グループボックスクラス
class CTimeGroupBoxEx : public CButton
{
	DECLARE_DYNAMIC(CTimeGroupBoxEx)

public:
	CTimeGroupBoxEx();
	virtual ~CTimeGroupBoxEx();

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
