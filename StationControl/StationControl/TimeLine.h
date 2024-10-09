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
	stTimeLineObject base;			// ��{�J���[
	stTimeLineObject light;			// ��{�J���[��+50
	stTimeLineObject lighter;		// ��{�J���[��+100
	stTimeLineObject lightest;		// ��{�J���[��+150
	stTimeLineObject dark;			// ��{�J���[��-50
	stTimeLineObject darker;		// ��{�J���[��-100
	stTimeLineObject darkShadow;	// 3D �I�u�W�F�N�g�̈Â��e�̐F
	stTimeLineObject liteShadow;	// 3D �I�u�W�F�N�g�̉e�̐F (�����̔��Ε����̉��p)
	stTimeLineObject shadow;		// �e�̐F (���p)
} stTimeLineColor;

enum{
	eColor_Background,		// �w�i
	eColor_FixedCell,		// �Œ�Z���w�i
	eColor_Grid,			// �O���b�h
	eColor_GridCell,		// �O���b�h

	eColor_Forecast,		// �\��l
	eColor_ForecastUsed,	// �\��l(�g�p��)
	eColor_OpePlan,			// �^�p�v��
	eColor_OpePlanExecute,	// �^�p�v����s��
	eColor_OpePlanNormal,	// �^�p�v�搳��I��
	eColor_OpePlanAbnormal,	// �^�p�v��ُ�I��

	eColor_PrePass,			// �v���p�X
	eColor_PostPass,		// �|�X�g�p�X

	eColor_OpePlanEnp,		// �p�X�iENP�j
	eColor_OpePlanCalib,	// �v��Z���f�f
	eColor_OpeStationOccupy,// �ǐ�L
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
	eLineType_Run,			// �^�p�v��p�X��
	eLineType_Plan,			// �^�p�v��o�^�O
	eLineType_PlanRegist,	// �^�p�v��o�^��
	eLineType_PlanInit,		// �^�p�v�揉���ݒ蒆
	eLineType_Pred,			// �\��l
	eLineType_Overlap,		// ���s���^�p�v��Ɠ����q���A�����ԑт��d�Ȃ�\��l
	eLineType_UnexecRdsn,	// �����s�Z���f�f
	eLineType_execRdsn,		// ���s���Z���f�f
	eLineType_Occu,			// �ǐ�L�o�^�O
	eLineType_OccuRegist,	// �ǐ�L�o�^��
	eLineType_OccuInit,		// �ǐ�L�����ݒ蒆
	eLineType_OccuRun,		// �ǐ�L�p�X��
	eLineType_Vlbi,			// VLBI�o�^�O
	eLineType_VlbiRegist,	// VLBI�o�^��
	eLineType_VlbiInit,		// VLBI�����ݒ蒆
	eLineType_VlbiRun,		// VLBI�p�X��
	eLineType_Empty,		// �󂫃G���A
};

#define SCROLL_VALUE 1000.0 
#define CTLOFFSET 0
#define WM_NEWHORZLIMITS (CTLOFFSET + WM_USER + 701)
#define WM_NEWVERTLIMITS (CTLOFFSET + WM_USER + 702)
#define MAX_TIMEDIV		9

#define		OperationTime_Forecast			12		// �\��l�F�i���ݓ�������12���ԑO�j�_�~�[
#define		OperationTime_Initialize		120		// �����ݒ�F120�b
#define		OperationTime_AntennaWait		1500	// ANT�҂��󂯎w���@1500(�P�c) 60+360(���V�Y)
#define		OperationTime_AntennaWait2		420		// ANT�҂��󂯎w���@1500(�P�c) 60+360(���V�Y)
#define		OperationTime_AntennaZenith		900		// ANT�V���w�� 900(�P�c) 60(���V�Y)
#define		OperationTime_AntennaZenith2	60		// ANT�V���w�� 900(�P�c) 60(���V�Y)
#define		OperationTime_Agc				420		// AGC�Z�� 420
#define		OperationTime_Rng				120		// RNG�Z�� 120
#define		OperationTime_Tlm				300		// TLM�`�� 300
#define		OperationTime_Cmd				300		// CMD�`�� 300

typedef struct  {
	TCHAR		satellite[30];	// �q����
	TCHAR		predid[20];		// �\��lID
	TCHAR		cid[20];		// CID
	TCHAR		planid[20];		// �v��ID
	__int64		start;			// �J�n����
	__int64		end;			// �I������
	COLORREF	textcolor;		// �����F
	COLORREF	color;			// �F
	UINT		stationID;		// ��ID
	UINT		getkind;		// �擾
	UINT		opekind;		// �^�p���
	UINT		kind;			// �p�X���
	UINT		code;			// �I���R�[�h
	LPARAM		getform;		// �擾�� 
	CString		maintcomment;	// MAINT�p�R�����g
} stTimeLine;

typedef struct  {
	TCHAR		satellite[30];					// �q����
	map< __int64, vector<stTimeLine> > item;	// �^�p�^�C�����C���A�C�e��(�L�[�F�v��ID)
} stTimeData;

typedef struct  {
	TCHAR		satellite[30];					// �q����
	TCHAR		passId[20];						// �p�XID
	__int64		aos;							// �\��l�J�n����
	__int64		los;							// �\��l�I������
	__int64		maxelTime;						// Max EL ����
	double		maxel;							// Max EL
	UINT		station;						// �ǔԍ�
	UINT		flag;							// 0x01=VLBI 0x02=MAINT
} stForecastData;

typedef struct  {
	UINT		id;
	TCHAR		satellite[30];					// �q����
} stSatelliteData;

typedef struct  {
	TCHAR		satellite[30];					// �q����
	TCHAR		passId[20];						// �p�XID
	RECT		rect;
	RECT		itemrect;
	UINT		type;							// �^�p�^�C�v
	DWORD		getform;						// �擾��
	UINT		station;						// �ǔԍ�
} stDrawRect;

enum{
	eBaseSpace,
	eBaseStation,
};
static const CString mButtonString[] = {
	_T("�q�����x�[�X"),
	_T("�ǖ��x�[�X"),
};

// CTimeLine

class CTimeLine : public CWnd
{
	DECLARE_DYNAMIC(CTimeLine)

public:
	CTimeLine();
	virtual ~CTimeLine();

	// �X�N���[��
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
	/* �����o�ϐ�                                                                           */
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

	// �ڍו\�����
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
	/* �����o�֐�                                                                           */
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
//!< �g���O���[�v�{�b�N�X�N���X
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
