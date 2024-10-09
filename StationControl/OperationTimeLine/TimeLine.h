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
	eColor_Pass,			// �p�X�^�p
	eColor_Forecast,		// �\��l

	eColor_ReadinessCheck,	// ���f�B�l�X�`�F�b�N
	eColor_EqInitialize,	// �ݔ��������ݒ�
	eColor_Antenna,			// �A���e�i�҂��󂯎w��
	eColor_DownlinkRecv,	// �_�E�������N��M
	eColor_PostPass,		// �|�X�g�p�X

	eColor_UplinkkSend,		// �A�b�v�����N���M
	eColor_Command,			// �R�}���h�^�p
	eColor_Range,			// �����W�^�p
	eColor_UsedForecast,	// �g�p���\��l

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

enum{
	eTimeKind_Master,
	eTimeKind_DownLink,
	eTimeKind_UpLink,
	eTimeKind_Max,
};

static const CString mTimeLabel[] = {
	_T("�}�X�^�[�v��"), _T("�_�E�������N"), _T("�A�b�v�����N"),
};


typedef struct  {
	CString		label;
	CTime		start;		// �J�n����
	CTime		end;		// �I������
	COLORREF	color;
	bool		updown;		// [�����ݒ�]��[ANT�҂��󂯎w���ʒm]��ON�̏ꍇ��true
} stTimeLine;

static const CString mEventMode[] = {
	_T("�������s"),
	_T("�蓮"),
};

static const CString mOptSendDoppler[] = {
	_T("�Ȃ��i�Z���^�[�j"),
	_T("�Ȃ��i�x�X�g���b�N�j"),
	_T("�Ȃ��i�m�~�i���j"),
	_T("����"),
};
static const CString mOptSendOutput[] = {
	_T("HPA-20kw"),
	_T("HPA-2kw"),
	_T("HPA-200w"),
	_T("SSPA-100w"),
	_T("SSPA-10w"),
};
static const CString mOptANTZenith[] = {
	_T("�Ȃ�"),
	_T("����iLOCK����j"),
	_T("����iLOCK�Ȃ��j"),
};
static const CString mCIDOptANTZenith[] = {
	_T("�Ȃ�"),
	_T("�V��(LOCK)"),
	_T("�V��(UNLOCK)"),
	_T("�V��"),
};
static const CString mControlOptANTZenith[] = {
	_T("DISABLE"),
	_T("STOW_LOCK"),
	_T("STOW_UNLOCK"),
};
static const CString mMasterSlabe[] = {
	_T("���"),
	_T("�]��"),
};
static const CString mMasterSlabePlan[] = {
	_T("MASTER"),
	_T("SLAVE"),
};

static const CString mPlanKind[] = {
	_T("�ҋ@��"),
	_T("�v���p�X��"),
	_T("�p�X��"),
	_T("�|�X�g�p�X��"),
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

	// �X�N���[��
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
	/* �����o�ϐ�                                                                           */
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
	// �\��l�ꗗ�f�[�^
	vector<stPredListData>		mListPredData;
	// �v��ꗗ�f�[�^
	vector<stPlanListData>		mListPlanData;
	// �^�p�v��f�[�^
	vector<COprPlnData>			mListTimeData;

	int m_nUplnkEvent;			// �A�b�v�����N ����:0 / �蓮:1
	int m_nCmdEvent;			// CMD ����:0 / �蓮:1
	int m_nRngEvent;			// RNG ����:0 / �蓮:1

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
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
//!< �g���O���[�v�{�b�N�X�N���X
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
