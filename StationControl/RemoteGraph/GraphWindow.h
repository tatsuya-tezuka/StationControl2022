#pragma once
#include "afxwin.h"

#include "GraphException.h"

using namespace std;

#include "DrawObject.h"
#include "DrawTool.h"
#include "const.h"

enum eGraphType{
	eGraphType_All,
	eGraphType_Trend,
	eGraphType_OffAll,
	eGraphType_OffTrend,
};

enum eViewID{
	eViewID_1,
	eViewID_2,
	eViewID_Num,
};

enum eGraphControl
{
	mCtrlTrend = 0x00000100L,
};

enum eMessage
{
	mMessageStartInitialize = WM_USER + 100,
	mMessageOpenFile,
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

#define HSCALE_NUM	14

//-- 20181022 グラフ縦軸の数値化け修正 >>>
//#define MAXDIGIT	(1 + 9 + 3 + 1)		// マイナス +  整数部 + カンマ + 文字列終わり 
//-- 20181022 グラフ縦軸の数値化け修正 <<<
//++ 20181022 グラフ縦軸の数値化け修正 >>>
#define MAXDIGIT	(1 + 13 + 4 + 1)		// マイナス +  整数部 + カンマ + 文字列終わり 
//++ 20181022 グラフ縦軸の数値化け修正 <<<

#define AUTO_V_TOPBOTMARGIN		0.05
#define AUTO_V_TOPMARGIN		(1.0 + AUTO_V_TOPBOTMARGIN)
#define AUTO_V_BOTMARGIN		(1.0 - AUTO_V_TOPBOTMARGIN)

#define NLine1st()	{ \
		bIsLastMaxValue = TRUE; \
		pnt.first = (double)xpos; pnt.second = draw_data.mmPair.min; \
		(*itr_pos) = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		memDC.MoveTo((*itr_pos)); \
		itr_pos++;	m_drawPosCount[(*itr).first]++;	\
		pnt.second = draw_data.mmPair.max; \
		pointDraw = (*itr_pos) = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		memDC.LineTo((*itr_pos)); \
		itr_pos++;	m_drawPosCount[(*itr).first]++;	\
	}

#define NLineMinToMax()	{ \
		bIsLastMaxValue = TRUE; \
		pnt.first = (double)xpos; pnt.second = draw_data.mmPair.min; \
		(*itr_pos) = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		memDC.LineTo((*itr_pos)); \
		itr_pos++;	m_drawPosCount[(*itr).first]++;	\
		pnt.second = draw_data.mmPair.max; \
		pointDraw = (*itr_pos) = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		memDC.LineTo((*itr_pos)); \
		itr_pos++;	m_drawPosCount[(*itr).first]++;	\
	}

#define NLineMaxToMin()	{ \
		bIsLastMaxValue = FALSE; \
		pnt.first = (double)xpos; pnt.second = draw_data.mmPair.max; \
		(*itr_pos) = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		memDC.LineTo((*itr_pos)); \
		itr_pos++;	m_drawPosCount[(*itr).first]++;	\
		pnt.second = draw_data.mmPair.min; \
		pointDraw = (*itr_pos) = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		memDC.LineTo((*itr_pos)); \
		itr_pos++;	m_drawPosCount[(*itr).first]++;	\
	}

#define NEllipse()	{ \
		r = (*itr).second.nDotSize / 2; \
		pnt.first = (double)xpos; pnt.second = draw_data.mmPair.min; \
		(*itr_pos) = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		if( draw_data.status & SDS_LEAPMIN ) \
			memDC.Rectangle((*itr_pos).x - r, (*itr_pos).y - r, (*itr_pos).x + r, (*itr_pos).y + r); \
		else \
			memDC.Ellipse((*itr_pos).x - r, (*itr_pos).y - r, (*itr_pos).x + r, (*itr_pos).y + r); \
		itr_pos++;	m_drawPosCount[(*itr).first]++;	\
		pnt.second = draw_data.mmPair.max; \
		pointDraw = (*itr_pos) = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		if( draw_data.status & SDS_LEAPMAX ) \
			memDC.Rectangle((*itr_pos).x - r, (*itr_pos).y - r, (*itr_pos).x + r, (*itr_pos).y + r); \
		else \
			memDC.Ellipse((*itr_pos).x - r, (*itr_pos).y - r, (*itr_pos).x + r, (*itr_pos).y + r); \
		itr_pos++;	m_drawPosCount[(*itr).first]++;	\
	}

#define NEllipse_NotSave()	{ \
		r = (*itr).second.nDotSize / 2; \
		pnt.first = (double)xpos; pnt.second = draw_data.mmPair.min; \
		pointDraw = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		if( draw_data.status & SDS_LEAPMIN ) \
			memDC.Rectangle(pointDraw.x - r, pointDraw.y - r, pointDraw.x + r, pointDraw.y + r); \
		else \
			memDC.Ellipse(pointDraw.x - r, pointDraw.y - r, pointDraw.x + r, pointDraw.y + r); \
		pnt.second = draw_data.mmPair.max; \
		pointDraw = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		if( draw_data.status & SDS_LEAPMAX ) \
			memDC.Rectangle(pointDraw.x - r, pointDraw.y - r, pointDraw.x + r, pointDraw.y + r); \
		else \
			memDC.Ellipse(pointDraw.x - r, pointDraw.y - r, pointDraw.x + r, pointDraw.y + r); \
	}

#define MMLine1st()	{ \
		bIsLastMaxValue = TRUE; \
		pnt.first = (double)xpos; pnt.second = draw_data.mmPair.min; \
		(*itr_pos) = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		memDC.MoveTo((*itr_pos)); \
		itr_pos++;	m_drawPosCount[(*itr).first]++;	\
		pnt.second = draw_data.mmPair.max; \
		pointDraw = (*itr_pos) = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		memDC.LineTo((*itr_pos)); \
		itr_pos++;	m_drawPosCount[(*itr).first]++;	\
	}

#define MMLineMinToMax()	{ \
		bIsLastMaxValue = TRUE; \
		pnt.first = (double)xpos; pnt.second = draw_data.mmPair.min; \
		(*itr_pos) = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		memDC.LineTo((*itr_pos)); \
		itr_pos++;	m_drawPosCount[(*itr).first]++;	\
		pnt.second = draw_data.mmPair.max; \
		pointDraw = (*itr_pos) = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		memDC.LineTo((*itr_pos)); \
		itr_pos++;	m_drawPosCount[(*itr).first]++;	\
	}

#define MMLineMaxToMin()	{ \
		bIsLastMaxValue = FALSE; \
		pnt.first = (double)xpos; pnt.second = draw_data.mmPair.max; \
		(*itr_pos) = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		memDC.LineTo((*itr_pos)); \
		itr_pos++;	m_drawPosCount[(*itr).first]++;	\
		pnt.second = draw_data.mmPair.min; \
		pointDraw = (*itr_pos) = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		memDC.LineTo((*itr_pos)); \
		itr_pos++;	m_drawPosCount[(*itr).first]++;	\
	}

#define MMEllipse()	{ \
		r = (*itr).second.nDotSize / 2; \
		pnt.first = (double)xpos; pnt.second = draw_data.mmPair.min; \
		(*itr_pos) = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		if( draw_data.status & SDS_LEAPMIN ) \
			memDC.Rectangle((*itr_pos).x - r, (*itr_pos).y - r, (*itr_pos).x + r, (*itr_pos).y + r); \
		else \
			memDC.Ellipse((*itr_pos).x - r, (*itr_pos).y - r, (*itr_pos).x + r, (*itr_pos).y + r); \
		itr_pos++;	m_drawPosCount[(*itr).first]++;	\
		pnt.second = draw_data.mmPair.max; \
		pointDraw = (*itr_pos) = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		if( draw_data.status & SDS_LEAPMAX ) \
			memDC.Rectangle((*itr_pos).x - r, (*itr_pos).y - r, (*itr_pos).x + r, (*itr_pos).y + r); \
		else \
			memDC.Ellipse((*itr_pos).x - r, (*itr_pos).y - r, (*itr_pos).x + r, (*itr_pos).y + r); \
		itr_pos++;	m_drawPosCount[(*itr).first]++;	\
	}

#define MMEllipse_NotSave()	{ \
		r = (*itr).second.nDotSize / 2; \
		pnt.first = (double)xpos; pnt.second = draw_data.mmPair.min; \
		pointDraw = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		if( draw_data.status & SDS_LEAPMIN ) \
			memDC.Rectangle(pointDraw.x - r, pointDraw.y - r, pointDraw.x + r, pointDraw.y + r); \
		else \
			memDC.Ellipse(pointDraw.x - r, pointDraw.y - r, pointDraw.x + r, pointDraw.y + r); \
		pnt.second = draw_data.mmPair.max; \
		pointDraw = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin); \
		if( draw_data.status & SDS_LEAPMAX ) \
			memDC.Rectangle(pointDraw.x - r, pointDraw.y - r, pointDraw.x + r, pointDraw.y + r); \
		else \
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
		double curMax;
		double curMin;
	} structMinMax;	// 最小最大構造体

protected:
	typedef map<UINT, structLine> DRAWLINE;

	static const int mRightMargin = 5;
	static const int mBottomMargin = 5;
	static const UINT mZoomWidth = 5;
	static const COLORREF mZoomColor = RGB(0, 255, 0);

	static const UINT mGridMarginLeft = 100;
	static const UINT mGridMarginRight = 124;
	static const UINT mGridMarginTop = 10;
	static const UINT mGridMarginBottom	= 30;
	static const UINT mDataNameMargine = 10;
	static const UINT mLabelLeftMargine = 2;
	static const UINT mLabelRightMargine = 2;
	static const UINT mTimeLabel2ndMargine = 10;
	static const UINT mHStrDistance = 2;
	static const UINT mViewWindowIDMarginLeft = 4;
	static const UINT mViewWindowIDMarginBottom = 20;
	static const UINT mAutoStrLeftMargin = 5;
	static const int mMaxRange = 200 * 24* 3600;
//	static const int mMaxRange = 300;

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

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CDrawObjList	m_selection;
	CDrawObjList	m_objects;

	UINT	m_resDrawObject;

public:
	CWinThread	*m_pCommentDataPosThread;
	bool	m_bCommentDataPosStop;
	CWinThread	*m_pDifferDataPosThread;
	bool	m_bDifferDataPosStop;

protected:
	// ◆共通
	DRAWLINE			m_DrawLines;		// 線ID毎の情報map
	COLORREF			m_ColorBk;			// 背景色
	COLORREF			m_ColorGrid;		// グリッド色
	COLORREF			m_ColorText;		// テキスト色
	BOOL				m_bLabel;			// ラベル表示
	UINT				m_nHorzDiv;			// 横分割数
	BOOL				m_bUseVAxis[MAX_VSCALE];	// 縦軸使用の有無

	CWnd				*m_pParentWnd;		// 親ウィンドウ
	CFont				m_txtFont;			// テキストフォント

	UINT				m_nGraphType;		// グラフ種類
	UINT				m_nViewID;			// ビューID
	int					m_nGrid;			// グリッド種別
	int					m_CursorPos;		// カーソル位置
	CPoint				m_ZoomPos;			// ゾームカーソル位置

	stVScale			m_nStvScale;		// 縦軸スケール

	UINT				m_nWindowNo;		// ウィンドウ番号

	// ◆トレンドグラフ
	int					m_nTrendInc;		// トレンド増分値
	double				m_pointTop;			// 描画領域の先頭位置
	double				m_dbMaxHzVal;		// 全データの最大値
	double				m_dbMinHzVal;		// 全データの最小値
	double				m_dbHzRange;		// 描画領域のデータレンジ
	double				m_dbHzComp;			// 描画領域のデータレンジ圧縮率
	double				m_dbTmpHzRange;		// 描画領域のデータレンジテンポラリ
	UINT				m_nTrandHScale;		// トレンド横軸スケール種類

	CPoint				m_ptZoom[2];

	double				m_nTotalScaleLMax;	// 左縦軸に割り当てられたデータの最大
	double				m_nTotalScaleLMin;	// 左縦軸に割り当てられたデータの最小

	// 測定データ
	__int64			m_nTrendDataPos;				// トレンドデータ圧縮位置

	structMinMax	m_stMinMax[VScaleKind_Max];		// 最小値最大値

	BOOL			m_bIsRealTime;				// リアルタイム

	BOOL			m_bIsDrawOn[MAX_TDATA];		// 描画ON

	CPoint			m_ptTrendMove;
	UINT			m_nViewNo;

	CPoint			m_preMousePoint;
	CPoint			m_relZoomLeft;
	CPoint			m_absZoomStart;

public:
	vector<POINT>	m_drawPos[MAX_TDATA];
	UINT			m_drawPosCount[MAX_TDATA];

	CToolTipCtrl	*m_pToolTipCtrl;
	CString			m_sToolTipCtrl;
	CPoint			m_ptToolTipCtrl;

	UINT			m_nSelDifferNo;				// 値差の始点／終点
	CPoint			m_ptSelMouse;				// 値差のマウス位置
	UINT			m_nDifferMode;				// 値差のモード
	UINT			m_nDifferDataNo[2];			// 値差のデータ番号
	UINT			m_nDifferDataPos[2];		// 値差のデータ位置
	CPoint			m_ptDifferData[2];			// 値差のデータ座標

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	BOOL	Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, UINT nWindowNo);

	void	SetGraphType(UINT val)	{ m_nGraphType = val; }
	void	SetViewID(UINT val)		{ m_nViewID = val; }
	UINT	GetViewID()		{ return m_nViewID; }
	void	SetBackColor(COLORREF val)	{ m_ColorBk = val; }
	void	SetGridColor(COLORREF val)	{ m_ColorGrid = val; }
	void	SetGridType(int val)	{ m_nGrid = val; }
	void	SetRealTime(BOOL val)	{ m_bIsRealTime = val; }
	BOOL	GetRealTime()	{ return m_bIsRealTime; }

	BOOL	AddLineToMap(UINT nID, structLine line, CString strName, VScaleKind vscaleKind);

	BOOL	AddLine(UINT nID, CString strName, CString strGName, UINT nGraphKind, COLORREF crColor, DWORD nStyle, int nWidth, VScaleKind nVScaleKind, BOOL bVisible);
	BOOL	AddDot(UINT nI, CString strNameD, CString strGName, UINT nGraphKind, COLORREF crColor, int nDotSize, VScaleKind nVScaleKind, BOOL bVisible);
	BOOL	AddLineAndDot(UINT nID, CString strName, CString strGName, UINT nGraphKind, COLORREF crColor, DWORD nStyle, int nWidth, int nDotSize, VScaleKind nVScaleKind, BOOL bVisible);
	BOOL	AddAxis(stVScale nStvScale, HScale trandHScale);

	double	GetVAxisMax();
	double	GetVAxisMin();

	void	ClearLineAndDot(UINT nID = 0xffffffff);
	void	UpdateGraph();

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
	void	DrawObj(CDC* pDC, bool bRectangle, bool bLine, bool bLimit, bool bDiffer);
	void	ResetCursor(int pos, bool reset = false);
	void	ResetRectangle(LONG left, LONG right);
	void	ResetLimit(int index, int kind, int upper, int lower, bool reset = false, bool bInvalidate = true);
	void	ResetDiffer(bool reset = false, bool bInvalidate = true);
	void	CreateMinMaxValue(bool bClear = false);

	LONG	ConvNoToX(unsigned __int64 nPurePos);
	unsigned __int64	ConvXToNo(LONG nDispPos);

	UINT	GetWindowNo()	{ return m_nWindowNo; }
	CFont&	GetFont()	{ return m_txtFont; }

	void	GetMinMaxValue(double& min, double& max)	{ min = m_stMinMax[LScale].curMin; max = m_stMinMax[LScale].curMax; }
	void	GetMinMaxValueEx(double& min, double& max)	{ min = m_stMinMax[LScale].min; max = m_stMinMax[LScale].max; }

protected:
	CPoint	ConvPoint(pair<double, double> pnt, double max, double min) { return ConvPoint(pnt.first, pnt.second, max, min); }
	CPoint	ConvPoint(double X, double Y, double max, double min);
	CPoint	ConvPoint(double X);
	BOOL	DrawGrid(CDC& memDC, CRect& RectClient);
	void	DrawCursor(CDC& memDC);
	void	DrawGraph(CDC& memDC);
	void	DrawTrend(CDC& memDC);

	void		AutoHScaleClear()
	{
		if( m_nStvScale.Auto == TRUE )
		{
			m_nTotalScaleLMax = -DBL_MAX;		// 左縦軸スケール上限
			m_nTotalScaleLMin = DBL_MAX;		// 左縦軸スケール下限
		}
	}

	CWnd	*GetChildFrame()
	{
		CWnd	*pWnd = GetParent();
		if( (pWnd != NULL) && (pWnd->GetSafeHwnd() != NULL) )
		{
			pWnd = pWnd->GetParent();
			if( (pWnd != NULL) && (pWnd->GetSafeHwnd() != NULL) )
			{
				pWnd = pWnd->GetParent();
				if( (pWnd != NULL) && (pWnd->GetSafeHwnd() != NULL) )
				{
					return pWnd;
				}
			}
		}

		return NULL;
	}

	LONG	GetZoomLeft()
	{
		POSITION	pos = m_objects.GetHeadPosition();

		while( pos != NULL )
		{
			CDrawObject	*pObj = m_objects.GetNext(pos);

			if( pObj->GetShape() == CDrawRect::rectangle )
				return pObj->m_position.left;
		}

		return 0;
	}

	int	GetValidTblNo(unsigned __int64	nCount, int nAllTblNo)
	{
		int	nTbl = 0;

		for( int t = 0; t < NUM_REAL_SCALE_INFO_TABLE; t++ )
		{
			if( tblRealScaleInfo[t].cmp_rate > 1 )
			{
				nTbl = t - 1;
				break;
			}

			if( (nAllTblNo != 0) && (nAllTblNo <= t) )
			{
				nTbl = t - 1;
				break;
			}

			if( tblRealScaleInfo[t].dsp_data > nCount )
			{
				nTbl = t;
				break;
			}
		}

		return nTbl;
	}

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

//	void	UpdateScrollInfo();
	void	Normalize();
	void	SendNewLimits();
	void	OnZoom(bool bZoomIn);

	// ドットサイズを取得する
	int GetDotSize(int nDotSize);

//-- 20181022 グラフ縦軸の数値化け修正 >>>
//	CString	addCanma(double pos);
//-- 20181022 グラフ縦軸の数値化け修正 <<<
//++ 20181022 グラフ縦軸の数値化け修正 >>>
	CString addCanma(double pos, int pointKeta, int singFlag = 0);
//++ 20181022 グラフ縦軸の数値化け修正 <<<

	double Ceil3Keta(double pos, int dir);
	double CeilKeta(double pos, int dir, double range);

	void InitSatelliteDataSetting();
	void CalcMinMax(stSpaceData& sData, structMinMax& stMinMax);

	void	RemoveCommentDataPosThread();
	void	CreateCommentDataPosThread(CPoint point);
	void	RemoveDifferDataPosThread();
	void	CreateDifferDataPosThread(CPoint point);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

#pragma once
