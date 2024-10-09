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

//#define MAXDIGIT	(1 + 9 + 2 + 1)		// �}�C�i�X +  ������ + �J���} + ������I��� 
#define MAXDIGIT	(1 + 13 + 4 + 1)		// �}�C�i�X +  ������ + �J���} + ������I��� 
#define MAXDIGIT14	(1 + 14 + 4 + 1)		// �}�C�i�X +  ������ + �J���} + ������I���

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
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
	typedef struct  {
		CString		strName;		// �f�[�^��
		CString		strGName;		// �f�[�^���i�O���t�\�����j
		CString		strUnit;		// �P��
		int			nDecDidits;		// �����_�ȉ��̌���
		DWORD		nDataId;		// �f�[�^ID
		int			nListIdx;		// ���X�g�C���f�b�N�X
		int			nDrawKind;		// �`�惂�[�h
		COLORREF	crColor;		// ���F
		int			nStyle;			// ���X�^�C��
		int			nWidth;			// ����
		int			nDotSize;		// �h�b�g�T�C�Y
		BOOL		bIsVisible;		// ���\���̗L��
		double		nMaxValue;		// �f�[�^�̍ő�l
		double		nMinValue;		// �f�[�^�̍ŏ��l
		UINT		nGraphKind;		// �O���t���
		VScaleKind	nVScaleKind;	// �c���X�P�[���I���i�E / ���j
	} structLine;	// ���C�������\����

	typedef struct {
		structLine* stLine;
		double max;
		double min;
//		double curMax;
//		double curMin;
	} structPPMinMax;	// PP���k�@�ŏ��ő�\����

	typedef struct {
		deque<stSpaceData>	spaceData;		// �q���f�[�^
		int					srcType;		// �f�[�^�� 0:�H�w�l�ϊ���f�[�^, 1:�t�@�C��
	} structTrendDataPP;	// �g�����hPP���k�f�[�^�\����

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
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CDrawObjList	m_selection;
	CDrawObjList	m_objects;

protected:
	// ������
	DRAWLINE			m_DrawLines;		// ��ID���̏��map
	COLORREF			m_ColorBk;			// �w�i�F
	COLORREF			m_ColorGrid;		// �O���b�h�F
	COLORREF			m_ColorText;		// �e�L�X�g�F
	COLORREF			m_ColorLighttGrid;	// �����O���b�h�F
	BOOL				m_bLabel;			// ���x���\��
	UINT				m_nHorzDiv;			// ��������
	BOOL				m_bUseVAxis[MAX_VSCALE];	// �c���g�p�̗L��

	CWnd				*m_pParentWnd;		// �e�E�B���h�E
	CFont				m_txtFontSmall;		// �e�L�X�g�t�H���g�i�X���[���j
	CFont				m_txtFontNormal;	// �e�L�X�g�t�H���g�i�m�[�}���j

	UINT				m_nGraphType;		// �O���t��ށi�I�����C���E�I�t���C���`�掯�ʂ̂��߂̃O���t�^�C�v�j
	UINT				m_nViewID;			// �r���[ID�i���b�Z�[�W���ʂ̂��߂̃r���[�ԍ��j
	UINT				m_nPropertiesID;	// �ݒ�ID�iAGC/RNG/RR/EL���ʂ̂��߂̐ݒ�ԍ��j
	CString				m_strViewName;		// �r���[��
	int					m_nGrid;			// �O���b�h���
	int					m_CursorPos;		// �J�[�\���ʒu
	CPoint				m_ZoomPos;			// �]�[���J�[�\���ʒu

	stVScale			m_nStvScale;		// �c���X�P�[��

	// ���g�����h�O���t
	int					m_nTrendInc;		// �g�����h�����l
	double				m_pointTop;			// �`��̈�̐擪�ʒu
	double				m_dbMaxHzVal;		// �S�f�[�^�̍ő�l
	double				m_dbMinHzVal;		// �S�f�[�^�̍ŏ��l
	double				m_dbHzRange;		// �`��̈�̃f�[�^�����W�F�f�t�H���g0-1000
	double				m_dbTmpHzRange;		// �`��̈�̃f�[�^�����W�e���|����
	UINT				m_nTrandHScale;		// �g�����h�����X�P�[�����

	CPoint				m_ptZoom[2];

	double				m_dTotalScaleLMax;	// ���c���Ɋ��蓖�Ă�ꂽ�f�[�^�̍ő�
	double				m_dTotalScaleLMin;	// ���c���Ɋ��蓖�Ă�ꂽ�f�[�^�̍ŏ�
	double				m_dGridScaleLMax;	// �O���b�h�X�P�[���ő�
	double				m_dGridScaleLMin;	// �O���b�h�X�P�[���ŏ�

	// ����f�[�^
	structTrendDataPP	m_TrendDataPP[MAX_TDATA];	// �g�����hPP�f�[�^
	
	__int64			m_nTrendDataPos;				// �g�����h�f�[�^���k�ʒu
//	__int64			m_nTrendDelPos;					// �g�����h�f�[�^�폜�ʒu
//	__int64			m_nTrendPPDelPos;				// �g�����hPP���k�f�[�^�폜�ʒu
	__int64			m_nQueuLastTotalSize[MAX_TDATA];// ���߂̃L���[�g�[�^���T�C�Y

	__int64			m_PPNumber[MAX_TDATA];		// PP���k��
	__int64			m_PPTotalCount[MAX_TDATA];	// PP���k���v�J�E���g
	structPPMinMax	m_stPPMinMax;				// PP���k�ŏ��l�ő�l

	BOOL			m_bIsRealTime;				// ���A���^�C��
//	BOOL			m_bDelPPHead;				// PP�̈�̐擪���폜
//	BOOL			m_bDelPPHeadAfter;			// PP�̈�̐擪���폜

	BOOL			m_bFirstDraw;				// ����`��
	BOOL			m_bIsDrawOn[MAX_TDATA];		// �`��ON

	CPoint			m_ptTrendMove;
	UINT			m_nViewNo;

	CButton			m_btnSetting;				// �ݒ�{�^��
	BOOL			m_bViewDetail;				// �ڍו\��

	CButton			m_btnHGridChk;				// ���r���`�F�b�N�{�^��
	BOOL			m_bHGridOn;					// ���r���\��ON

	CButton			m_btnIntg;					// �ϕ����ԃ{�^��
	int				m_nIntgTime;				// �ϕ�����(1�`300)

	CRect			m_rectIntgBtn;				// �ϕ��{�^���̈�

	time_t			m_StartTime;				// �J�n����
	time_t			m_EndTime;					// �I������
	time_t			m_MostOldTime;				// �����Ƃ��Â�����
	time_t			m_MostNewTime;				// �����Ƃ��V��������
	BOOL			m_bFitYohochi;				// �\��l�ɍ��킹��
	time_t			m_yohoOldTime;				// �Â��\��l�����i�J�n�j
	time_t			m_yohoNEwTime;				// �V�����\��l�����i�I���j

	__int64			m_nTrendDataEndPos;			// �f�[�^�I���ʒu

	double			m_curVal[MAX_TDATA];		// ���ݒl

	int				m_StationIndex;

	CRect			m_rectCurValArea;			// ���ݒl�̈�
	CButton			m_chkBtnViewSig[MAX_TDATA];		// �M���\���`�F�b�N�{�^��(54m�ȊO)
	CStatic			m_txtSigName[MAX_TDATA];		// �M����(54m)

	BOOL			m_bExitData;				// �f�[�^����
	BOOL			m_bStopDraw;				// �`���~

	//-----------------------------------------------------------------------------
	//	�����o�֐�
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


	// �g�����h��̓Y�[���p�̃����o�ϐ����֐�
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

	// �h�b�g�T�C�Y���擾����
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

