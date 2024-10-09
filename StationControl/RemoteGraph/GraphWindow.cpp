
#include "stdafx.h"
#include "RemoteGraph.h"
#include "GraphWindow.h"

#include "MainFrm.h"
#include "ChildFrm.h"

#define	DEF_SAME_RANGE	50.0
#define _GRAPHDRAW_KAIZEN 1

CGraphWindow::CGraphWindow()
: m_pointTop(0)
, m_dbMaxHzVal(1000)
, m_dbMinHzVal(0)
, m_dbHzRange(100)
, m_dbTmpHzRange(m_dbHzRange)
, m_nHorzDiv(10)
, m_DrawLines()
, m_ColorBk(RGB(0, 0, 0))
, m_ColorGrid(RGB(128, 128, 128))
, m_ColorText(RGB(0, 0, 0))
, m_nGraphType(eGraphType_All)
, m_nGrid(eGridType_NONE)
, m_nTrendInc(1)
, m_CursorPos(0)
, m_nTrandHScale(0)
, m_nTotalScaleLMax(-DBL_MAX)
, m_nTotalScaleLMin(DBL_MAX)
, m_nTrendDataPos(0)
{
	// 初期化
	m_pParentWnd = NULL;
	m_nWindowNo = 0;
	m_nViewNo = 0;

	// テキスト用のフォント作成
	m_txtFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (m_txtFont.GetLogFont(&lf)){
		lf.lfHeight = -10;
		wcscpy_s(lf.lfFaceName, LF_FACESIZE, _T("Arial"));
		m_txtFont.DeleteObject();
		m_txtFont.CreateFontIndirect(&lf);
	}

	m_preMousePoint = CPoint(-1, -1);

	MutexLock();
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		m_drawPos[i].resize(tblRealScaleInfo[NUM_REAL_SCALE_INFO_TABLE - 1].dsp_data << 1/* Min/Max分なので２倍 */);
		m_drawPosCount[i] = 0;
	}
	MutexUnlock();

	m_pToolTipCtrl = NULL;
	m_sToolTipCtrl.Empty();
	m_ptToolTipCtrl = CPoint(0, 0);

	m_nSelDifferNo = 0;
	m_ptSelMouse = CPoint(0, 0);
	m_nDifferMode = DifferNone;
	m_nDifferDataNo[0] = 0;
	m_nDifferDataNo[1] = 0;
	m_nDifferDataPos[0] = 0;
	m_nDifferDataPos[1] = 0;
	m_ptDifferData[0] = CPoint(0, 0);
	m_ptDifferData[1] = CPoint(0, 0);

	m_pCommentDataPosThread = NULL;
	m_bCommentDataPosStop = false;
	m_pDifferDataPosThread = NULL;
	m_bDifferDataPosStop = false;

	ClearCursor();
}

CGraphWindow::~CGraphWindow()
{
	RemoveCommentDataPosThread();
	RemoveDifferDataPosThread();

	if( m_pToolTipCtrl != NULL )
	{
		delete m_pToolTipCtrl;
		m_pToolTipCtrl = NULL;
	}

	MutexLock();
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		m_drawPos[i].clear();
		m_drawPosCount[i] = 0;
	}
	MutexUnlock();

	m_txtFont.DeleteObject();

	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL)
		delete m_objects.GetNext(pos);

}

BEGIN_MESSAGE_MAP(CGraphWindow, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
//	ON_WM_HSCROLL()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


/*============================================================================*/
/*! グラフ描画

-# グラフウィンドウの作成

@param	dwStyle		ウィンドウスタイル
@param	rect		ウィンドウデフォルト領域
@param	pParentWnd	親ウィンドウ
@param	nID			ウィンドウ識別子
@retval

*/
/*============================================================================*/
BOOL CGraphWindow::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, UINT nWindowNo)
{
	BOOL result = FALSE;
	m_pParentWnd = pParentWnd;

	// ウィンドウ番号
	m_nWindowNo = nWindowNo;

	dwStyle |= WS_CHILD;
	//if (m_nGraphType == eGraphType_OffTrend){
	//	dwStyle |= WS_HSCROLL;
	//}

	// ウィンドウの作成
	result = CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID, NULL);
	// 初期化
	m_nGrid = eGridType_NONE;	// グリッド有無
	m_bLabel = FALSE;			// ラベル表示
	m_dbMaxHzVal = m_dbHzRange;
	m_nHorzDiv = 10;			// 分割数

	m_bUseVAxis[LScale] = TRUE;			// 端軸使用
	m_bUseVAxis[RScale] = TRUE;			// 端軸使用
	m_nStvScale.Auto = TRUE;			// 端軸の自動スケール
	m_nStvScale.VScalLMax = 50.0;		// 左端軸の最大値
	m_nStvScale.VScalLMin = -50.0;		// 左端軸の最小値
	m_nStvScale.VScalRMax = 50.0;		// 右端軸の最大値
	m_nStvScale.VScalRMin = -50.0;		// 右端軸の最小値
	m_nStvScale.VSCalItvl = 5;			// 端軸分割数

	if( m_pToolTipCtrl != NULL )
	{
		delete m_pToolTipCtrl;
		m_pToolTipCtrl = NULL;
	}

	// ツールチップコントロールを作成
	if( (m_pToolTipCtrl = new CToolTipCtrl) == NULL )
		return FALSE;

	m_pToolTipCtrl->Create(this, TTS_ALWAYSTIP);
	m_pToolTipCtrl->AddTool(this, _T(""));
	m_pToolTipCtrl->SetDelayTime(TTDT_INITIAL, 1000);
	m_pToolTipCtrl->Activate(TRUE);

	//if (m_nGraphType == eGraphType_OffTrend)
//	{
//		UpdateScrollInfo();
//	}

	m_nTrendDataPos = 0;				// トレンドの開始時刻からの時間位置

	if (m_nViewID == eViewID_1)
	{
#ifdef	_TEST_MAXDATA
		m_dbHzComp = tblRealScaleInfo[h3].cmp_rate;
		SetHzRange(tblRealScaleInfo[h3].dsp_data * m_dbHzComp);	// 横時刻ビュー１デフォルト
#else	// 	_TEST_MAXDATA
		m_dbHzComp = tblRealScaleInfo[h24].cmp_rate;
		SetHzRange(tblRealScaleInfo[h24].dsp_data * m_dbHzComp);	// 横時刻ビュー１デフォルト
#endif	// 	_TEST_MAXDATA
	}
	else
	{
		m_dbHzComp = tblRealScaleInfo[h1].cmp_rate;
		SetHzRange(tblRealScaleInfo[h1].dsp_data * m_dbHzComp);		// 横時刻ビュー２デフォルト
	}

	m_nTrandHScale = 0;					// 縦軸スケールの種類
	m_nTotalScaleLMax = -DBL_MAX;		// 左縦軸スケール上限
	m_nTotalScaleLMin = DBL_MAX;		// 左縦軸スケール下限

	InitSatelliteDataSetting();

	ResetCursor(-1, true);

	for( int i = 0; i < MAX_TDATA; i++ )
	{
		for( int j = 0; j < LimitKind_Max; j++ )
			ResetLimit(i, j, -1, -1, true);
	}

//	EnableToolTips(TRUE);

	return result;
}

/*============================================================================*/
/*! グラフ描画

-# グラフウィンドウの描画

@param	なし
@retval	なし

*/
/*============================================================================*/
void CGraphWindow::OnPaint()
{
	CRect rectClient;
	CRect rectGrid;
	CPaintDC dc(this);

	// クライアント領域の取得
	GetClientRect(rectClient);

	CDC memDC;
	CBitmap bitm;
	// 描画ＤＣの作成
	memDC.CreateCompatibleDC(&dc);

	// 描画用ビットマップの作成
	bitm.CreateCompatibleBitmap(&dc, rectClient.Width(), rectClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitm);

	// バックグラウンド モードを設定
	memDC.SetBkMode(OPAQUE);
	// 背景色の設定
	memDC.SetBkColor(m_ColorBk);
	// デバイス コンテキストのビューポートの原点を設定
	memDC.SetViewportOrg(0, 0);

	CBrush brush(m_ColorBk);
	// 全体を塗りつぶし
	memDC.FillRect(rectClient, &brush);

	// 軸の描画
	rectGrid.SetRect(
		rectClient.left + mGridMarginLeft,
		rectClient.top + mGridMarginTop,
		rectClient.right - mGridMarginRight,
		rectClient.bottom - mGridMarginBottom);

	if (m_nGrid != eGridType_NONE)
	{
		if (DrawGrid(memDC, rectGrid) == TRUE)
		{
			// グラフ線の描画
			if (!m_DrawLines.empty()){
				//if (m_bLabel == TRUE)
				//	DrawText(memDC);
				DrawGraph(memDC);
			}
		}
	}

	//DrawCursor(memDC);

	dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0, 0, SRCCOPY);

	//DrawCursor(dc);

	bool	bRealTime = ((m_bIsRealTime == FALSE) && (theApp.IsAnalysisMigration(m_nWindowNo) == false)) ? false : true;

	// ノンリアルかつALLならカーソル類を描画
//	if( (!bRealTime) && (m_nViewID == eViewID_1) && (theApp.GetDataCollection().GetAnalysisCount(m_nWindowNo) != 0) )
	if( bRealTime )
		DrawObj(&dc, false, false, true, false);
	else
	{
		if( theApp.GetDataCollection().GetAnalysisCount(m_nWindowNo) != 0 )
			DrawObj(&dc, true, true, false, true);
	}

	memDC.SelectObject(pOldBitmap);
	bitm.DeleteObject(); // ADD 2011.08.25
	memDC.DeleteDC(); // ADD 2011.08.25
	brush.DeleteObject();
}

/*============================================================================*/
/*! グラフ描画

-# 論理単位をデバイス単位に変換

@param X X値
@param Y Y値
@retval

*/
/*============================================================================*/
CPoint CGraphWindow::ConvPoint(double X, double Y, double max, double min)
{
	int nY;
	int nX;
	CRect rectClient;
	GetClientRect(rectClient);
	
	// 軸領域と同じにサイズ変更
	rectClient.SetRect(
		rectClient.left + mGridMarginLeft,
		rectClient.top + mGridMarginTop,
		rectClient.right - mGridMarginRight,
		rectClient.bottom - mGridMarginBottom);

	// トレンド特殊処理（最大＝最小＝値）
	if (m_nGraphType == eGraphType_Trend){
		if (max == min && max == Y){
			nX = (int)((rectClient.Width() * (X - m_pointTop)) / m_dbHzRange);
			return CPoint(nX, rectClient.top + rectClient.Height() / 2);
		}
	}
	nY = ((max - min) == 0.0) ? 0 : ((int)((rectClient.Height() * (Y - min)) / (max - min) + 0.5));
	nX = (int)((rectClient.Width() * (X - m_pointTop)) / (m_dbHzRange - m_dbHzComp) + mGridMarginLeft);
	return CPoint(nX, rectClient.Height() - nY + mGridMarginTop);
}
CPoint CGraphWindow::ConvPoint(double X)
{
	int nX;
	CRect rectClient;
	GetClientRect(rectClient);

	// 軸領域と同じにサイズ変更
	rectClient.SetRect(
		rectClient.left + mGridMarginLeft,
		rectClient.top + mGridMarginTop,
		rectClient.right - mGridMarginRight,
		rectClient.bottom - mGridMarginBottom);

	switch (m_nGraphType)
	{
	case eGraphType_All:
	case eGraphType_OffAll:
		nX = (int)((rectClient.Width() * (X - m_pointTop)) / m_dbTmpHzRange + mGridMarginLeft);
		break;
	case eGraphType_Trend:
	case eGraphType_OffTrend:
	default:
		nX = (int)((rectClient.Width() * (X - m_pointTop)) / (m_dbHzRange - m_dbHzComp) + mGridMarginLeft);
		break;
	}
	return CPoint(nX, rectClient.Height());
}

/*============================================================================*/
/*! グラフ描画

-# 監視データ番号から表示座標に変換

@param nPurePos	監視データ番号
@retval

*/
/*============================================================================*/
LONG CGraphWindow::ConvNoToX(unsigned __int64 nPurePos)
{
	CRect rectClient;
	GetClientRect(rectClient);
	
	// 軸領域と同じにサイズ変更
	rectClient.SetRect(
		rectClient.left + mGridMarginLeft,
		rectClient.top + mGridMarginTop,
		rectClient.right - mGridMarginRight,
		rectClient.bottom - mGridMarginBottom);

	return (LONG)((rectClient.Width() * (nPurePos - m_pointTop)) / (m_dbHzRange - m_dbHzComp));
}

/*============================================================================*/
/*! グラフ描画

-# 表示座標から監視データ番号に変換

@param nDispPos	表示座標
@retval

*/
/*============================================================================*/
unsigned __int64 CGraphWindow::ConvXToNo(LONG nDispPos)
{
	CRect rectClient;
	GetClientRect(rectClient);
	
	// 軸領域と同じにサイズ変更
	rectClient.SetRect(
		rectClient.left + mGridMarginLeft,
		rectClient.top + mGridMarginTop,
		rectClient.right - mGridMarginRight,
		rectClient.bottom - mGridMarginBottom);

	if( nDispPos < rectClient.left )
		nDispPos = rectClient.left;
	if( nDispPos > rectClient.right )
		nDispPos = rectClient.right;

	return (unsigned __int64)(((m_dbHzRange - m_dbHzComp) * (nDispPos - mGridMarginLeft)) / rectClient.Width());
}

/*============================================================================*/
/*! グラフ描画

-# グリッド線の描画

@param	memDC		ドキュメント
@param	RectClient	描画領域
@retval	TRUE:グリッド線を描画した / FALSE:グリッド線を描画しなかった

*/
/*============================================================================*/
BOOL CGraphWindow::DrawGrid(CDC& memDC, CRect& RectClient)
{
	// グリッド線の描画
	CPen penLine;
	CPen penSolid;
	penLine.CreatePen((m_nGrid == eGridType_DOT) ? PS_DOT : PS_SOLID, 1, m_ColorGrid);
	penSolid.CreatePen(PS_SOLID, 1, m_ColorGrid);
	CPen* oldPen = memDC.SelectObject(&penLine);
	CFont* oldFont = memDC.SelectObject(&m_txtFont);

	bool	bRealTime = ((m_bIsRealTime == FALSE) && (theApp.IsAnalysisMigration(m_nWindowNo) == false)) ? false : true;

	// 縦横の軸を表示する領域が確保できるか調べる
	int height = RectClient.Height();
	double oldYPos = -1.0;
	BOOL bRangeValid = TRUE;
	for (UINT n = 0; n <= (UINT)m_nStvScale.VSCalItvl; n++)
	{
		double yPos = (double)n * (height - 1) / m_nStvScale.VSCalItvl;
		if (yPos < oldYPos)
		{
			bRangeValid = FALSE;
		}
		oldYPos = yPos;
	}
	int nOldInipoint = -1;
	double dEndN = m_pointTop + m_dbHzRange + (m_dbHzRange / (double)m_nHorzDiv / 2);
	if (bRangeValid == TRUE)
	{
		for (double n1 = 0.0, n2 = 0.0;
			n1 < dEndN;
			n1 += (m_dbHzRange / (double)m_nHorzDiv), n2 += (m_dbTmpHzRange / (double)m_nHorzDiv))
		{

			int nInipoint = ConvPoint((double)n2).x;
			if (nInipoint < nOldInipoint)
			{
				bRangeValid = FALSE;
			}
			nOldInipoint = nInipoint;
		}
	}

	// トレンドグラフビュー番号の表示
	CString strDigit;
	if (m_nViewID == eViewID_2)
		strDigit = bRealTime ? _T("< B >") : _T("< ZOOM >");
	else
		strDigit = bRealTime ? _T("< A >") : _T("< ALL >");

	memDC.SetTextColor(m_ColorGrid);
	memDC.SetBkMode(TRANSPARENT);
	memDC.TextOutW(mViewWindowIDMarginLeft, RectClient.bottom + mViewWindowIDMarginBottom, strDigit);

	if (m_nStvScale.VSCalItvl > 0 && bRangeValid == TRUE)
	{
		// 横線描画
		double dVLRange;
		double dVLPos;
		double dVRRange;
		double dVRPos;
		double dVLStep;
		double dVRStep;
//++ 1ドットずれ処置 >>>
		double dVLMin;
//++ 1ドットずれ処置 <<<
		if (m_nStvScale.Auto)
		{
			double range;
#ifndef _GRAPHDRAW_KAIZEN			
			range = fabs(m_nTotalScaleLMax - m_nTotalScaleLMin);
			if (range == 0.0)
				range = fabs(m_nTotalScaleLMax);
			if (range == 0.0)
				range = 100.0;
			double	max = Ceil3Keta(m_nTotalScaleLMax + range * (double)AUTO_V_TOPBOTMARGIN, 1);
			double	min = Ceil3Keta(m_nTotalScaleLMin - range * (double)AUTO_V_TOPBOTMARGIN, 0);
#else
			// 最大最小が初期値でない場合
			if (m_nTotalScaleLMax != -DBL_MAX && m_nTotalScaleLMin != DBL_MAX)
			{
				// 範囲値をセット
				range = fabs(m_nTotalScaleLMax - m_nTotalScaleLMin);
			}
			else
			{
				// デフォルトで範囲値100
				range = 100.0;
			}
			if (range == 0.0)
			{
				// 値が１値しかない場合の範囲値セット
				range = fabs(m_nTotalScaleLMax) / 100.0;
			}
			if (range == 0.0)
			{
				// どうしても範囲値が0になる場合
				range = 100.0;
			}

			double rangeMargin = range * (double)AUTO_V_TOPBOTMARGIN;
			double rangeWidth = range + (double)AUTO_V_TOPBOTMARGIN * 2;
			double max = CeilKeta(m_nTotalScaleLMax + rangeMargin, 1, rangeWidth);
			double min = CeilKeta(m_nTotalScaleLMin - rangeMargin, 0, rangeWidth);

#endif	/* _GRAPHDRAW_KAIZEN */
			if (max == min)
			{
				if( m_nTotalScaleLMax == m_nTotalScaleLMin )
				{
					max = m_nTotalScaleLMax + fabs((DEF_SAME_RANGE) * (double)AUTO_V_TOPBOTMARGIN);
					min = m_nTotalScaleLMin - fabs((DEF_SAME_RANGE) * (double)AUTO_V_TOPBOTMARGIN);
				}
				else
				{
					max = m_nTotalScaleLMax + fabs((m_nTotalScaleLMax - m_nTotalScaleLMin) * (double)AUTO_V_TOPBOTMARGIN);
					min = m_nTotalScaleLMin - fabs((m_nTotalScaleLMax - m_nTotalScaleLMin) * (double)AUTO_V_TOPBOTMARGIN);
				}
			}
			if (m_nTotalScaleLMax >= m_nTotalScaleLMin)
			{
				dVLRange = max - min;
				dVLPos = max;
//++ 1ドットずれ処置 >>>
				dVLMin = min;
//++ 1ドットずれ処置 <<<
			}
			else
			{
				dVLRange = 0.0;
				dVLPos = 0.0;
//++ 1ドットずれ処置 >>>
				dVLMin = 0.0;
//++ 1ドットずれ処置 <<<
			}
		}
		else
		{
			dVLRange = m_nStvScale.VScalLMax - m_nStvScale.VScalLMin;
			dVLPos = m_nStvScale.VScalLMax;
//++ 1ドットずれ処置 >>>
			dVLMin = m_nStvScale.VScalLMin;
//++ 1ドットずれ処置 <<<
		}
		dVRRange = m_nStvScale.VScalRMax - m_nStvScale.VScalRMin;
		dVRPos = m_nStvScale.VScalRMax;
		dVLStep = fabs(dVLRange / m_nStvScale.VSCalItvl);
		dVRStep = fabs(dVRRange / m_nStvScale.VSCalItvl);
		int nStrLPos1 = -1;
		int nStrLPos2 = -1;
		int nStrRPos1 = -1;
		int nStrRPos2 = -1;

		CString strDigitMax = _T(" -###,###,###.###### ");
		CSize szDigitMax = memDC.GetTextExtent(strDigitMax);

		
		// 画面上部から横軸を描画
		for (UINT n = 0; n <= (UINT)m_nStvScale.VSCalItvl; n++, dVLPos -= dVLStep, dVRPos -= dVRStep)
		{
			if (n == 0 || n == m_nStvScale.VSCalItvl)
				memDC.SelectObject(&penSolid);
			else
				memDC.SelectObject(&penLine);

//-- 1ドットずれ処置 >>>
//			double yPos = (double)n * height / m_nStvScale.VSCalItvl;
//			memDC.MoveTo(RectClient.left, RectClient.top + (int)yPos);//double to int
//			memDC.LineTo(RectClient.right, RectClient.top + (int)yPos);//double to int
//-- 1ドットずれ処置 <<<
//++ 1ドットずれ処置 >>>
			// 分割数と左軸の設定値を使用してY値計算
			double Y = (double)(m_nStvScale.VSCalItvl - n) * dVLStep + dVLMin;
			// Y軸レンジが0の場合はY値がないため、縦幅のみから計算する。
			// それ以外の場合は、ConvPoint関数のY座標計算と同じ処理
			int nY = (dVLRange == 0.0) ? ((int)((double)(m_nStvScale.VSCalItvl - n) * height / m_nStvScale.VSCalItvl + 0.5)) : ((int)((height * (Y - dVLMin)) / dVLRange + 0.5));
			int yPos = height - nY + RectClient.top;
			// 横軸を引く
			memDC.MoveTo(RectClient.left, yPos);
			memDC.LineTo(RectClient.right, yPos);
//++ 1ドットずれ処置 <<<

			// Auto時、左縦軸上にAuto文字の表示
			if (m_nStvScale.Auto && n == 0)
			{
				long nVPos = (long)ceil(dVLPos);

				CString strDigit = _T("Auto");
				CSize sz = memDC.GetTextExtent(strDigit);

				// 左縦軸数値文字の間にAuto文字を書く
				memDC.SetTextColor(m_ColorGrid);
				memDC.SetBkMode(TRANSPARENT);
//-- 1ドットずれ処置 >>>
//				memDC.TextOutW(RectClient.left + 5, RectClient.top + (int)(yPos + 0.5) - sz.cy, strDigit);
//-- 1ドットずれ処置 <<<
//++ 1ドットずれ処置 >>>
				memDC.TextOutW(RectClient.left + 5, yPos - sz.cy, strDigit);
//++ 1ドットずれ処置 <<<
			}

			// 左縦軸の値表示
			if (dVLStep != 0.0)
			{
				//			long nVPos = (long)ceil(dVLPos);
				CString strDigit;
				//			strDigit.Format(_T("% 8d"), nVPos);
				strDigit = addCanma(dVLPos, 6);

				CSize sz = memDC.GetTextExtent(strDigit);

//-- 1ドットずれ処置 >>>
//				nStrLPos1 = RectClient.top + (int)(yPos + 0.5) - sz.cy;
//-- 1ドットずれ処置 <<<
//++ 1ドットずれ処置 >>>
				nStrLPos1 = yPos - sz.cy;
//++ 1ドットずれ処置 <<<
				if (nStrLPos2 < 0 ||
					nStrLPos1 > nStrLPos2 + sz.cy)
				{
					nStrLPos2 = nStrLPos1;
					memDC.SetTextColor(m_ColorGrid);
					memDC.SetBkMode(TRANSPARENT);
					memDC.TextOutW(RectClient.left - sz.cx - mLabelLeftMargine, nStrLPos2, strDigit);
				}
			}

			// 右縦軸の値表示
			if( bRealTime )
			{
				if( m_bUseVAxis[RScale] == TRUE )
				{
					CString strDigit;
					strDigit = addCanma(dVRPos, 6);

					CSize sz = memDC.GetTextExtent(strDigit);

//-- 1ドットずれ処置 >>>
//					nStrRPos1 = RectClient.top + (int)(yPos + 0.5) - sz.cy;
//-- 1ドットずれ処置 <<<
//++ 1ドットずれ処置 >>>
					nStrRPos1 = yPos - sz.cy;
//++ 1ドットずれ処置 <<<
					if (nStrRPos2 < 0 ||
						nStrRPos1 > nStrRPos2 + sz.cy)
					{
						nStrRPos2 = nStrRPos1;
						memDC.SetTextColor(m_ColorGrid);
						memDC.SetBkMode(TRANSPARENT);
						memDC.TextOutW(RectClient.right + mLabelRightMargine + szDigitMax.cx - sz.cx, nStrRPos2, strDigit);
					}
				}
			}
			else
			{
				CString strDigit;
				strDigit = addCanma(dVLPos, 6);

				CSize sz = memDC.GetTextExtent(strDigit);

//-- 1ドットずれ処置 >>>
//				nStrRPos1 = RectClient.top + (int)(yPos + 0.5) - sz.cy;
//-- 1ドットずれ処置 <<<
//++ 1ドットずれ処置 >>>
				nStrRPos1 = yPos - sz.cy;
//++ 1ドットずれ処置 <<<
				if (nStrRPos2 < 0 ||
					nStrRPos1 > nStrRPos2 + sz.cy)
				{
					nStrRPos2 = nStrRPos1;
					memDC.SetTextColor(m_ColorGrid);
					memDC.SetBkMode(TRANSPARENT);
					memDC.TextOutW(RectClient.right + mLabelRightMargine + szDigitMax.cx - sz.cx, nStrRPos2, strDigit);
				}
			}
		}
	}
	if (m_nHorzDiv > 0 && bRangeValid == TRUE)
	{
		if( bRealTime )
			CGraphMutex::Lock(eRealTimeData);
		else
			CGraphMutex::Lock_Analysis(m_nWindowNo);

		// 現在の横位置から、時刻を監視データリストから取得する。
		deque<CSpaceData>*	spaceDataList = &theApp.GetDataCollection().GetSpaceData();

		CTime		t;
		CTime		prev_t;

		SYSTEMTIME timeSys;
		GetSystemTime(&timeSys);
		t = timeSys;

		// 監視データサイズの初期化
		unsigned __int64	size = 0;

		// 前回からのデータ増分
		if( bRealTime )
		{
			if( spaceDataList->empty() == FALSE )
			{
				deque<CSpaceData>::iterator trenditr = spaceDataList->begin();

				// 監視データのサイズを調べる
				size = (int)(*trenditr).totalSize;
			}
		}
		else
		{
			// 解析用監視データのサイズを取得
			for( int i = 0; i < MAX_TDATA; i++ )
			{
				if( m_nViewID == eViewID_1 )
				{
					if( (size = theApp.GetDataCollection().GetTrendAllData(m_nWindowNo, i).size()) > 0 )
						break;
				}
				else
				{
					if( (size = theApp.GetDataCollection().GetTrendZoomData(m_nWindowNo, i).size()) > 0 )
						break;
				}
			}
		}

		if (size > 0)
		{
			deque<stSpaceData>	*sData = NULL;
			vector<stSpaceData>	*analysis_data = NULL;

			DRAWLINE::iterator itr;

			// プロパティの設定データ数分をループ
			for( itr = m_DrawLines.begin(); itr != m_DrawLines.end(); itr++ )
			{
				// 監視名が無い場合は表示しない
				if ((*itr).second.strName.IsEmpty() == TRUE)
					continue;

				// データインデックス(0～9)とライン属性情報を取得
				UINT	nID = (*itr).first;
				structLine data = (*itr).second;

				// 非表示設定ならば表示しない
				if (data.bIsVisible == FALSE)
				{
					continue;
				}

				if( bRealTime )
				{
					int nListIdx = (int)((*itr).second.nListIdx);

					// リストインデックスに値がある場合
					if (nListIdx >= 0)
					{
						// インデックス位置のデータを使う
						if( nListIdx < (*spaceDataList).size() )
							sData = &(*spaceDataList)[nListIdx].value;
					}
					else
					{
						deque<CSpaceData>::iterator	trenditr2;
						int nSearchIdx;

						// 監視データIDが一致する監視データを探す
						for (trenditr2 = spaceDataList->begin(), nSearchIdx = 0; trenditr2 != spaceDataList->end(); trenditr2++, nSearchIdx++)
						{
							// 監視データIDの一致を調べる
							if ((*trenditr2).dataid == (*itr).second.nDataId)
							{
								// 監視データID一致のデータを使う
								sData = &(*trenditr2).value;
								(*itr).second.nListIdx = nSearchIdx;
								break;
							}
						}
					}
				}
				else
				{
					if( m_nViewID == eViewID_1 )
						analysis_data = &theApp.GetDataCollection().GetTrendAllData(m_nWindowNo, nID);
					else
						analysis_data = &theApp.GetDataCollection().GetTrendZoomData(m_nWindowNo, nID);
				}

				// 表示する監視データがリストから見つからない場合
				if( (bRealTime && (sData == NULL)) || ((!bRealTime) && (analysis_data == NULL)) )
				{
					// 表示しない
					continue;
				}

				// 表示する監視データのサイズを取得
				if( (__int64)(bRealTime ? sData->size() : analysis_data->size()) == 0 )
				{
					sData = NULL;
					analysis_data = NULL;

					// 表示しない
					continue;
				}

				break;
			}

			SYSTEMTIME timeSys;
			GetSystemTime(&timeSys);
			t = timeSys;

			if( (bRealTime && (sData != NULL)) || ((!bRealTime) && (analysis_data != NULL)) )
			{
				// 選択スケールがAUTOなら、表示するスケールを検出
				UINT	nCurScale = m_nTrandHScale;

				if( bRealTime )
				{
					if( m_nTrandHScale == autoscale )
					{
						for( int t = 0; t < NUM_REAL_SCALE_INFO_TABLE; t++ )
						{
							unsigned __int64	data_size = size;

							if( tblRealScaleInfo[t].cmp_rate > 1 )
								data_size = theApp.GetDataCollection().GetCompData((*itr).second.nDataId, t).size();

							if( (data_size > 0) && (tblRealScaleInfo[t].dsp_data > data_size) )
							{
								nCurScale = t;
								break;
							}
						}
					}
				}
				else
				{
					if( m_nViewID == eViewID_1 )
						nCurScale = theApp.GetDataCollection().GetTrendAllTblNo(m_nWindowNo);
					else
						nCurScale = theApp.GetDataCollection().GetTrendZoomTblNo(m_nWindowNo);
				}

				stDrawInfo&	draw_info = theApp.GetDataCollection().GetDrawInfo((*itr).second.nDataId, nCurScale);
				__int64 pos = (!bRealTime) ? 0 : ((m_nTrandHScale == autoscale) ? 0 : draw_info.start_pos);

				if( bRealTime )
				{
					if( tblRealScaleInfo[nCurScale].cmp_rate <= 1 )
					{
						if( (pos >= 0) && (pos < (__int64)sData->size()) )
							t = (*sData)[pos].systime;
					}
					else
					{
						deque<stSpaceData>&	cmp_data = theApp.GetDataCollection().GetCompData((*itr).second.nDataId, nCurScale);

						if( (pos >= 0) && (pos < (__int64)cmp_data.size()) )
							t = cmp_data[pos].systime;
					}
				}
				else
				{
					if( (pos >= 0) && (pos < (__int64)analysis_data->size()) )
						t = (*analysis_data)[pos].systime;
				}
			}

			prev_t = t;
		}

		if( bRealTime )
			CGraphMutex::Unlock(eRealTimeData);
		else
			CGraphMutex::Unlock_Analysis(m_nWindowNo);

		// 縦線描画
		memDC.SelectObject(&penLine);

		// 全トレンド縦軸（縦軸の横ずれ防止）
		int nStrPos1 = -1;
		int nStrPos2 = -1;
		for (double n1 = 0.0, n2 = 0.0;
			n1 < dEndN;
			n1 += ((m_dbHzRange - m_dbHzComp) / (double)m_nHorzDiv), n2 += (m_dbTmpHzRange / (double)m_nHorzDiv), prev_t = t, t += (time_t)((m_dbHzRange - m_dbHzComp) / (double)m_nHorzDiv))
		{

			int nInipoint = ConvPoint((double)n2).x;
			memDC.MoveTo(nInipoint, RectClient.top);
			memDC.LineTo(nInipoint, RectClient.bottom);

			// 横軸の時刻表示
			CString strYmd;
			CString strHMS;
			CSize szYmd;
			CSize szHMS;
			// 左端の時刻もしくは、年月日が変わるとき、年月日を表示
			if (prev_t == t || prev_t.GetYear() != t.GetYear())
			{
				strYmd = t.Format("%Y/%m/%d");
			}
			else if (prev_t.GetMonth() != t.GetMonth() || prev_t.GetDay() != t.GetDay())
			{
				strYmd = t.Format("     %m/%d");
			}
			szYmd = memDC.GetTextExtent(strYmd);

			// 時分秒
			strHMS = t.Format("%H:%M:%S");
			szHMS = memDC.GetTextExtent(strHMS);

			// 大きい文字列のサイズを取得
			int xRange, xYmd;
			CString strYmdDef = _T("XXXX/XX/XX");
			CSize szYmdDef = memDC.GetTextExtent(strYmdDef);

			if (szYmdDef.cx > szYmd.cx)
			{
				xYmd = szYmdDef.cx;
			}
			else
			{
				xYmd = szYmd.cx;
			}

			if (xYmd > szHMS.cx)
			{
				xRange = xYmd;
			}
			else
			{
				xRange = szHMS.cx;
			}

			nStrPos1 = nInipoint - xRange / 2;
			if (nStrPos2 < 0 ||
				nStrPos1 > nStrPos2 + xRange + (long)mHStrDistance)
			{
				nStrPos2 = nStrPos1;
				memDC.SetTextColor(m_ColorGrid);
				memDC.SetBkMode(TRANSPARENT);
				memDC.TextOutW(nStrPos2, RectClient.bottom, strYmd);
				memDC.TextOutW(nStrPos2, RectClient.bottom + mTimeLabel2ndMargine, strHMS);
			}
		}
	}

	memDC.SelectObject(oldPen);
	memDC.SelectObject(oldFont);
	penLine.DeleteObject();
	penSolid.DeleteObject();

	return bRangeValid;
}

/*============================================================================*/
/*! グラフ描画

-# カーソル描画

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::DrawCursor(CDC& memDC)
{
	CRect rect;
	GetClientRect(rect);

	CPoint	curPoint;

	switch (m_nGraphType)
	{
	case	eGraphType_OffTrend:
		{
			int pos = m_CursorPos - 1;
			DRAWLINE::const_iterator iterLine = m_DrawLines.begin();
			int endpos = (int)((m_dbHzRange + m_pointTop) / m_nTrendInc);
			pair<double, double> pnt;
			pnt.first = (double)((double)pos*m_nTrendInc); pnt.second = (double)0;
			curPoint = ConvPoint(pnt, (*iterLine).second.nMaxValue, (*iterLine).second.nMinValue);
			if (rect.right == curPoint.x){
				// グラフ領域の最右部分とカーソル位置が同じ場合
				// カーソルが見えなくなってしまう対応
				curPoint.x -= 1;
			}

			CPen		pen;
			COLORREF	cf = RGB(0, 255, 255);
			COLORREF	cfs = RGB(0, 0, 0);
			if (pen.CreatePen(PS_SOLID, 1, cfs)){
				CPen* pOldPen = memDC.SelectObject(&pen);
				pen.DeleteObject();
				memDC.MoveTo(curPoint.x + 1, rect.top);
				memDC.LineTo(curPoint.x + 1, rect.bottom);
				memDC.SelectObject(pOldPen);
			}
			if (pen.CreatePen(PS_SOLID, 1, cf)){
				CPen* pOldPen = memDC.SelectObject(&pen);
				memDC.MoveTo(curPoint.x, rect.top);
				memDC.LineTo(curPoint.x, rect.bottom);
				memDC.SelectObject(pOldPen);
				pen.DeleteObject();
			}
			break;
		}
	}
}

/*============================================================================*/
/*! グラフ描画

-# グラフ線の描画

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::DrawGraph(CDC& memDC)
{
	if (m_bIsRealTime == FALSE)
	{
		// 暫定処置。オフラインの場合は描画しない
		//return;
	}

	switch (m_nGraphType){
	case	eGraphType_All:
	case	eGraphType_Trend:
		// トレンドグラフの描画
		DrawTrend(memDC);
		break;
	case	eGraphType_OffAll:
	case	eGraphType_OffTrend:
		// トレンドグラフの描画
		DrawTrend(memDC);
		break;
	default:
		// 上記以外グラフの描画
		return;
	}
}

/*============================================================================*/
/*! グラフ描画

-# トレンドグラフ線の描画

@param	memDC	デバイスコンテキスト
@retval	なし

*/
/*============================================================================*/
void CGraphWindow::DrawTrend(CDC& memDC)
{
	CRect rect;
	GetClientRect(rect);

	// 縦軸の領域内に描画する
	rect.SetRect(
		rect.left,
		rect.top + mGridMarginTop,
		rect.right,
		rect.bottom - mGridMarginBottom + 1);

	HRGN	hRgn = CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
	SelectClipRgn(memDC, hRgn);

	DRAWLINE::iterator itr;
	deque<CSpaceData>::iterator trenditr;

	CPoint pointDraw;
	CPen pen;
	CPen* oldPen;
	CBrush brush;
	CBrush* oldBrush;
	CFont* oldFont;

	UINT nID;
	pair<double, double> pnt;

	CTime		timeTop;
	int mSaveLogData;
	int mLogData;
	unsigned __int64 size;

	m_pointTop = 0.0;

	bool	bRealTime = ((m_bIsRealTime == FALSE) && (theApp.IsAnalysisMigration(m_nWindowNo) == false)) ? false : true;

	// トレンドの描画
	MutexLock();

	if( bRealTime )
		CGraphMutex::Lock(eRealTimeData);
	else
		CGraphMutex::Lock_Analysis(m_nWindowNo);

	// プロパティを取得
	CPropertiesData&	prop = theApp.GetPropertiesData(m_nWindowNo);

	// 監視データリストの取得
	deque<CSpaceData>*	spaceDataList = &theApp.GetDataCollection().GetSpaceData();
	deque<stSpaceData>	*sData;
	vector<stSpaceData>	*analysis_data;

	// プロパティの設定データ数分をループ
	for (itr = m_DrawLines.begin(); itr != m_DrawLines.end(); itr++)
	{
		vector<POINT>::iterator	itr_pos = m_drawPos[(*itr).first].begin();
		m_drawPosCount[(*itr).first] = 0;

		// 監視名が無い場合は表示しない
		if ((*itr).second.strName.IsEmpty() == TRUE)
		{
			continue;
		}

		// データインデックス(0～9)とライン属性情報を取得
		nID = (*itr).first;
		structLine data = (*itr).second;

		// 非表示設定ならば表示しない
		if (data.bIsVisible == FALSE)
		{
			continue;
		}

		mSaveLogData = -1;
		mLogData = -1;

		sData = NULL;
		analysis_data = NULL;
		size = 0;

		if( bRealTime )
		{
			int nListIdx = (int)((*itr).second.nListIdx);

			// リストインデックスに値がある場合
			if (nListIdx >= 0)
			{
				// インデックス位置のデータを使う
				if( nListIdx < (*spaceDataList).size() )
					sData = &(*spaceDataList)[nListIdx].value;
			}
			else
			{
				int nSearchIdx;

				// 監視データIDが一致する監視データを探す
				for (trenditr = spaceDataList->begin(), nSearchIdx = 0; trenditr != spaceDataList->end(); trenditr++, nSearchIdx++)
				{
					// 監視データIDの一致を調べる
					if ((*trenditr).dataid == (*itr).second.nDataId)
					{
						// 監視データID一致のデータを使う
						sData = &(*trenditr).value;
						(*itr).second.nListIdx = nSearchIdx;
						break;
					}
				}
			}
		}
		else
		{
			if( m_nViewID == eViewID_1 )
				analysis_data = &theApp.GetDataCollection().GetTrendAllData(m_nWindowNo, nID);
			else
				analysis_data = &theApp.GetDataCollection().GetTrendZoomData(m_nWindowNo, nID);
		}

		// 表示する監視データがリストから見つからない場合
		if( (bRealTime && (sData == NULL)) || ((!bRealTime) && (analysis_data == NULL)) )
		{
			// 表示しない
			continue;
		}

		// 表示する監視データのサイズを取得
		size = (__int64)(bRealTime ? sData->size() : analysis_data->size());
		if (size == 0)
		{
			// 表示しない
			continue;
		}

		// 選択スケールがAUTOなら、表示するスケールを検出
		UINT	nCurScale = m_nTrandHScale;

		if( bRealTime )
		{
			if( m_nTrandHScale == autoscale )
			{
				for( int t = 0; t < NUM_REAL_SCALE_INFO_TABLE; t++ )
				{
					unsigned __int64	data_size = size;

					if( tblRealScaleInfo[t].cmp_rate > 1 )
						data_size = theApp.GetDataCollection().GetCompData((*itr).second.nDataId, t).size();

					if( (data_size > 0) && (tblRealScaleInfo[t].dsp_data > data_size) )
					{
						nCurScale = t;
						break;
					}
				}

				// 横軸領域を更新
				m_dbHzComp = tblRealScaleInfo[nCurScale].cmp_rate;
				m_dbHzRange = tblRealScaleInfo[nCurScale].dsp_data * m_dbHzComp;
			}
		}
		else
		{
			if( m_nViewID == eViewID_1 )
				nCurScale = theApp.GetDataCollection().GetTrendAllTblNo(m_nWindowNo);
			else
				nCurScale = theApp.GetDataCollection().GetTrendZoomTblNo(m_nWindowNo);

			// 横軸領域を更新
//			m_dbHzComp = tblRealScaleInfo[nCurScale].cmp_rate;
//			m_dbHzRange = tblRealScaleInfo[nCurScale].dsp_data * m_dbHzComp;
		}

		// 選択スケールが圧縮スケールで圧縮データが存在しないならスルー
		deque<stSpaceData>	*cmp_data = NULL;
		if( bRealTime )
		{
			if( tblRealScaleInfo[nCurScale].cmp_rate > 1 )
			{
				cmp_data = &theApp.GetDataCollection().GetCompData((*itr).second.nDataId, nCurScale);

				if( cmp_data == NULL )
					continue;

				if( cmp_data->empty() )
					continue;
				else
					size = cmp_data->size();
			}
		}

		// 描画ONの場合にグラフデータを表示する
		if (m_bIsDrawOn[nID] == TRUE)
		{
			// 表示設定
			// 線色の設定
			pen.CreatePen((*itr).second.nStyle, (*itr).second.nWidth, (*itr).second.crColor);
			oldPen = memDC.SelectObject(&pen);

			// 円色の設定
			brush.CreateSolidBrush((*itr).second.crColor);
			oldBrush = memDC.SelectObject(&brush);
			int r;	// 円（点）の直径

			// フォントの設定
			oldFont = memDC.SelectObject(&m_txtFont);

			//		queitr = spaceDataQueu->begin();
			//		pos = nTrendPos;
			//		size = (__int64)spaceDataQueu->size();
			stDrawInfo&	draw_info = theApp.GetDataCollection().GetDrawInfo((*itr).second.nDataId, nCurScale);
			unsigned __int64 pos = (!bRealTime) ? 0 : ((m_nTrandHScale == autoscale) ? 0 : draw_info.start_pos);

//			TRACE(_T("XXXXX pos=%d size=%d\n"), pos, size);

			if( (pos < 0) || (pos >= size) ){
				memDC.SelectObject(oldPen);
				memDC.SelectObject(oldBrush);
				memDC.SelectObject(oldFont);
				pen.DeleteObject();
				brush.DeleteObject();
				continue;
			}

			int xpos = 0;
			int saveX = -1;
			BOOL bIsLastMaxValue = FALSE;
			bool ppTrend = true;


			if( tblRealScaleInfo[nCurScale].cmp_rate > 1 )
				timeTop = (!bRealTime) ? (*analysis_data)[pos].systime : (*cmp_data)[pos].systime;
			else
				timeTop = (!bRealTime) ? (*analysis_data)[pos].systime : (*sData)[pos].systime;

			// リアルタイムモードなら、リミットの表示位置を設定
			if( bRealTime )
			{
				int	upper, lower;

				upper = -1;
				if( prop.item[nID].isLimitWarningUpper )
				{
					pnt.first = 0.0; pnt.second = prop.item[nID].limitWarningUpper;
					if( (m_stMinMax[(*itr).second.nVScaleKind].curMin <= pnt.second) && (pnt.second <= m_stMinMax[(*itr).second.nVScaleKind].curMax) )
					{
						pointDraw = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin);
						upper = pointDraw.y;
					}
				}
				lower = -1;
				if( prop.item[nID].isLimitWarningLower )
				{
					pnt.first = 0.0; pnt.second = prop.item[nID].limitWarningLower;
					if( (m_stMinMax[(*itr).second.nVScaleKind].curMin <= pnt.second) && (pnt.second <= m_stMinMax[(*itr).second.nVScaleKind].curMax) )
					{
						pointDraw = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin);
						lower = pointDraw.y;
					}
				}

				ResetLimit(nID, LimitWarning, upper, lower, false, false/*位置だけ変更して描画更新は行わない*/);

				upper = -1;
				if( prop.item[nID].isLimitAlarmUpper )
				{
					pnt.first = 0.0; pnt.second = prop.item[nID].limitAlarmUpper;
					if( (m_stMinMax[(*itr).second.nVScaleKind].curMin <= pnt.second) && (pnt.second <= m_stMinMax[(*itr).second.nVScaleKind].curMax) )
					{
						pointDraw = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin);
						upper = pointDraw.y;
					}
				}
				lower = -1;
				if( prop.item[nID].isLimitAlarmLower )
				{
					pnt.first = 0.0; pnt.second = prop.item[nID].limitAlarmLower;
					if( (m_stMinMax[(*itr).second.nVScaleKind].curMin <= pnt.second) && (pnt.second <= m_stMinMax[(*itr).second.nVScaleKind].curMax) )
					{
						pointDraw = ConvPoint(pnt, m_stMinMax[(*itr).second.nVScaleKind].curMax, m_stMinMax[(*itr).second.nVScaleKind].curMin);
						lower = pointDraw.y;
					}
				}

				ResetLimit(nID, LimitAlarm, upper, lower, false, false/*位置だけ変更して描画更新は行わない*/);
			}

			bool	bOutputName = false;

			// 画面のx範囲のみ描画
			while (pos < size)
			{
				// 開始位置のデータから元の監視データから時刻の検索
				CTime	timeCur = (!bRealTime) ? (*analysis_data)[pos].systime : ((tblRealScaleInfo[nCurScale].cmp_rate > 1) ? (*cmp_data)[pos].systime : (*sData)[pos].systime);
				while (xpos < (int)m_dbHzRange && timeTop + xpos < timeCur )
				{
					// 同じ時刻がある対策。時刻が見つかるまでx位置をずらす
					xpos++;
				}

				if (xpos >= (int)m_dbHzRange)
				{
					break;
				}

				// 表示可能状態
				USHORT	status = (!bRealTime) ? (*analysis_data)[pos].status : ((tblRealScaleInfo[nCurScale].cmp_rate > 1) ? (*cmp_data)[pos].status : (*sData)[pos].status);
				if( status & SDS_VALID )
				{
					bOutputName = true;

					// 非圧縮表示
					if( tblRealScaleInfo[nCurScale].cmp_rate <= 1 )
					{
						stSpaceData&	draw_data = ((!bRealTime) ? (*analysis_data)[pos] : (*sData)[pos]);

						switch ((*itr).second.nDrawKind)
						{
						case Line:
							// 線の描画
							if (xpos == 0 || (((!bRealTime) ? (*analysis_data)[pos - 1].status : (*sData)[pos - 1].status) == SDS_INVALID) )
							{
								NLine1st();
							}
							else
							{
								stSpaceData&	pre_draw_data = ((!bRealTime) ? (*analysis_data)[pos - 1] : (*sData)[pos - 1]);

								// 前回が上昇線
								if (bIsLastMaxValue == TRUE)
								{
									if( draw_data.mmPair.max >= pre_draw_data.mmPair.max )
									{
										// 上昇線のためminからmax
										NLineMinToMax();
									}
									else
									{
										// 下降線のためmaxからmin
										NLineMaxToMin();
									}
								}
								// 前回が下降線
								else
								{
									if( draw_data.mmPair.min <= pre_draw_data.mmPair.min )
									{
										// 下降線のためmaxからmin
										NLineMaxToMin();
									}
									else
									{
										// 上昇線のためminからmax
										NLineMinToMax();
									}
								}
							}

							break;

						case Dots:
							// 円の描画
							NEllipse();

							break;

						case LineAndDots:
							// 線の描画
							if (xpos == 0 || (((!bRealTime) ? (*analysis_data)[pos - 1].status : (*sData)[pos - 1].status) == SDS_INVALID) )
							{
								NLine1st();
							}
							else
							{
								stSpaceData&	pre_draw_data = ((!bRealTime) ? (*analysis_data)[pos - 1] : (*sData)[pos - 1]);

								// 前回が上昇線
								if (bIsLastMaxValue == TRUE)
								{
									if( draw_data.mmPair.max >= pre_draw_data.mmPair.max )
									{
										// 上昇線のためminからmax
										NLineMinToMax();
									}
									else
									{
										// 下降線のためmaxからmin
										NLineMaxToMin();
									}
								}
								// 前回が下降線
								else
								{
									if( draw_data.mmPair.min <= pre_draw_data.mmPair.min )
									{
										// 下降線のためmaxからmin
										NLineMaxToMin();
									}
									else
									{
										// 上昇線のためminからmax
										NLineMinToMax();
									}
								}
							}

							// 円の描画
							NEllipse_NotSave();

							break;

						default:
							// ここには来ない。
							break;
						}
					}
					// 圧縮表示				
//					else if( (tblRealScaleInfo[nCurScale].cmp_rate > 1) && (tblRealScaleInfo[nCurScale].cmp_rate <= ((!bRealTime) ? (*analysis_data)[pos].mmPair.cnt : (*cmp_data)[pos].mmPair.cnt)) )
					else // 圧縮途中の点も描画
					{
//						TRACE(_T("Draw Trend scale = %d, pos = %d, size = %d, count = %d\n"), nCurScale, pos, cmp_data.size(), cmp_data[pos].mmPair.count);

						stSpaceData&	draw_data = ((!bRealTime) ? (*analysis_data)[pos] : (*cmp_data)[pos]);

						switch ((*itr).second.nDrawKind)
						{
						case Line:
							// 線の描画
							if (xpos == 0.0 || (((!bRealTime) ? (*analysis_data)[pos - 1].status : (*cmp_data)[pos - 1].status) == SDS_INVALID) )
							{
								MMLine1st();
							}
							else
							{
								stSpaceData&	pre_draw_data = ((!bRealTime) ? (*analysis_data)[pos - 1] : (*cmp_data)[pos - 1]);

								// 前回が上昇線
								if (bIsLastMaxValue == TRUE)
								{
									if( draw_data.mmPair.max >= pre_draw_data.mmPair.max )
									{
										// 上昇線のためminからmax
										MMLineMinToMax();
									}
									else
									{
										// 下降線のためmaxからmin
										MMLineMaxToMin();
									}
								}
								// 前回が下降線
								else
								{
									if( draw_data.mmPair.min <= pre_draw_data.mmPair.min )
									{
										// 下降線のためmaxからmin
										MMLineMaxToMin();
									}
									else
									{
										// 上昇線のためminからmax
										MMLineMinToMax();
									}
								}
							}
							break;

						case Dots:
							// 円の描画
							MMEllipse();
							break;

						case LineAndDots:
							// 線の描画
							if (xpos == 0.0 || (((!bRealTime) ? (*analysis_data)[pos - 1].status : (*cmp_data)[pos - 1].status) == SDS_INVALID) )
							{
								MMLine1st();
							}
							else
							{
								stSpaceData&	pre_draw_data = ((!bRealTime) ? (*analysis_data)[pos - 1] : (*cmp_data)[pos - 1]);

								// 前回が上昇線
								if (bIsLastMaxValue == TRUE)
								{
									if( draw_data.mmPair.max >= pre_draw_data.mmPair.max )
									{
										// 上昇線のためminからmax
										MMLineMinToMax();
									}
									else
									{
										// 下降線のためmaxからmin
										MMLineMaxToMin();
									}
								}
								// 前回が下降線
								else
								{
									if( draw_data.mmPair.min <= pre_draw_data.mmPair.min )
									{
										// 下降線のためmaxからmin
										MMLineMaxToMin();
									}
									else
									{
										// 上昇線のためminからmax
										MMLineMinToMax();
									}
								}
							}
							// 円の描画
							MMEllipse_NotSave();
							break;

						default:
							// ここには来ない。
							break;
						}
					}

					if (saveX >= 0 && saveX != pointDraw.x){
						ppTrend = false;
						//TRACE("### ppTrend!!!\n");
					}
				}
				// 表示不可状態
				else
				{
					(*itr_pos) = CPoint(-1, -1);
					itr_pos++;
					m_drawPosCount[(*itr).first]++;
					(*itr_pos) = CPoint(-1, -1);
					itr_pos++;
					m_drawPosCount[(*itr).first]++;
				}

				saveX = pointDraw.x;
				pos++;
			}

			if (ppTrend == true && size > 1)
			{
				saveX = -1;
			}

			// データ名表示
			if( bOutputName )
			{
				memDC.SetTextColor((*itr).second.crColor);
				memDC.SetBkMode(TRANSPARENT);
				CString	strName;
				if ((*itr).second.strGName.IsEmpty() == TRUE)
				{
					strName = (*itr).second.strName;
				}
				else
				{
					strName = (*itr).second.strGName;
				}

				CSize szName = memDC.GetTextExtent(strName);
				int	yName;
				if (pointDraw.y < rect.Height() / 2 + (int)mGridMarginTop)
				{
					yName = pointDraw.y;
				}
				else
				{
					yName = pointDraw.y - szName.cy;
				}
				memDC.TextOutW(pointDraw.x + mDataNameMargine, yName, strName);
			}

			// 測定データの描画
			memDC.SelectObject(oldPen);
			memDC.SelectObject(oldBrush);
			memDC.SelectObject(oldFont);
			pen.DeleteObject();
			brush.DeleteObject();
		}
		else
		{
			m_bIsDrawOn[nID] = TRUE;
		}
	}

	SelectClipRgn(memDC, NULL);
	DeleteObject(hRgn);

	// ノンリアルタイムモードなら、値差の表示位置を設定
	if( !bRealTime )
	{
		if( (m_nDifferMode != DifferNone) && (m_ptDifferData[0] != m_ptDifferData[1]) )
		{
			bool	bChange = false;

			if( (m_nDifferDataPos[0] < m_drawPosCount[m_nDifferDataNo[0]]) && ((CPoint)m_drawPos[m_nDifferDataNo[0]][m_nDifferDataPos[0]] != CPoint(-1, -1)) )
			{
				if( m_ptDifferData[0] != m_drawPos[m_nDifferDataNo[0]][m_nDifferDataPos[0]] )
					bChange = true;
				m_ptDifferData[0] = m_drawPos[m_nDifferDataNo[0]][m_nDifferDataPos[0]];
			}
			if( (m_nDifferDataPos[1] < m_drawPosCount[m_nDifferDataNo[1]]) && ((CPoint)m_drawPos[m_nDifferDataNo[1]][m_nDifferDataPos[1]] != CPoint(-1, -1)) )
			{
				if( m_ptDifferData[1] != m_drawPos[m_nDifferDataNo[1]][m_nDifferDataPos[1]] )
					bChange = true;
				m_ptDifferData[1] = m_drawPos[m_nDifferDataNo[1]][m_nDifferDataPos[1]];
			}

			if( bChange )
				ResetDiffer(false, false/*位置だけ変更して描画更新は行わない*/);
		}
	}

	if( bRealTime )
		CGraphMutex::Unlock(eRealTimeData);
	else
		CGraphMutex::Unlock_Analysis(m_nWindowNo);

	MutexUnlock();
}

/*============================================================================*/
/*! グラフ描画

-# 属性データの追加

@param nID		ラインＩＤ
@param crColor	ライン色
@param nStyle	ラインスタイル
@param nWidth	ライン幅
@param bVisible	ラインの表示/非表示

@retval	bool

*/
/*============================================================================*/
BOOL CGraphWindow::AddLineToMap(UINT nID, structLine line, CString strName, VScaleKind vscaleKind)
{
	bool	bRealTime = ((m_bIsRealTime == FALSE) && (theApp.IsAnalysisMigration(m_nWindowNo) == false)) ? false : true;

	MutexLock();

	// 最大値、最小値を初期化
	if (m_DrawLines.find(nID) == m_DrawLines.end())
	{
		line.nMaxValue = m_nTotalScaleLMax;
		line.nMinValue = m_nTotalScaleLMin;
	}

	if( bRealTime )
		CGraphMutex::Lock(eRealTimeData);
	else
		CGraphMutex::Lock_Analysis(m_nWindowNo);

	bool	b_empty = true;
	if( bRealTime )
	{
		deque<CSpaceData>*	spaceDataList = &theApp.GetDataCollection().GetSpaceData();
		deque<CSpaceData>::iterator trenditr;
		if (spaceDataList->empty() == FALSE)
		{
			trenditr = spaceDataList->begin();
			if( (spaceDataList->empty() == FALSE && (*trenditr).value.empty() == FALSE) )
				b_empty = false;
		}
	}
	else
	{
		// 解析用監視データのサイズを取得
		for( int i = 0; i < MAX_TDATA; i++ )
		{
			if( m_nViewID == eViewID_1 )
			{
				if( theApp.GetDataCollection().GetTrendAllData(m_nWindowNo, i).empty() == FALSE )
				{
					b_empty = false;
					break;
				}
			}
			else
			{
				if( theApp.GetDataCollection().GetTrendZoomData(m_nWindowNo, i).empty() == FALSE )
				{
					b_empty = false;
					break;
				}
			}
		}
	}

	// 固定スケール
	// または自動スケール（非初期値）
	if( (m_DrawLines.find(nID) != m_DrawLines.end()) && (m_nStvScale.Auto == FALSE || (b_empty == false)) )
	{
		line.nMaxValue = m_DrawLines[nID].nMaxValue;
		line.nMinValue = m_DrawLines[nID].nMinValue;
	}
	// 自動スケール（初期値）
	else
	{
		line.nMaxValue = m_nTotalScaleLMax;
		line.nMinValue = m_nTotalScaleLMin;
	}

	{
		line.nListIdx = -1;				// 衛星データリストのインデックス初期化
		line.nMaxValue = -DBL_MAX;		// 最大値クリア
		line.nMinValue = DBL_MAX;		// 最小値クリア

		m_bIsDrawOn[nID] = TRUE;		// 描画ON
	}

	m_DrawLines[nID] = line;

	// 左縦軸の最大値最小値更新
	if (m_DrawLines[nID].nVScaleKind == LScale)
	{
		if (m_DrawLines[nID].nMaxValue > m_nTotalScaleLMax)
		{
			m_nTotalScaleLMax = m_DrawLines[nID].nMaxValue;
		}
		if (m_DrawLines[nID].nMinValue < m_nTotalScaleLMin)
		{
			m_nTotalScaleLMin = m_DrawLines[nID].nMinValue;
		}
	}

	if( bRealTime )
		CGraphMutex::Unlock(eRealTimeData);
	else
		CGraphMutex::Unlock_Analysis(m_nWindowNo);

	MutexUnlock();

	for( int i = 0; i < MAX_TDATA; i++ )
	{
		for( int j = 0; j < LimitKind_Max; j++ )
			ResetLimit(i, j, -1, -1, true);
	}

	return TRUE;
}

/*============================================================================*/
/*! グラフ描画

-# 線属性データの追加

@param nID		ラインＩＤ
@param crColor	ライン色
@param nStyle	ラインスタイル
@param nWidth	ライン幅
@param bVisible	ラインの表示/非表示

@retval	bool

*/
/*============================================================================*/
BOOL CGraphWindow::AddLine(UINT nID, CString strName, CString strGName, UINT nGraphKind, COLORREF crColor, DWORD  nStyle, int nWidth, VScaleKind nVScaleKind, BOOL bVisible)
{
	// 線スタイルが異常の場合はデフォルト設定
	if (!((PS_STYLE_MASK)& nStyle))
		nStyle |= PS_SOLID;

	structLine line;

	line.nDrawKind = Line;
	line.bIsVisible = bVisible;
	line.crColor = crColor;
	line.nStyle = nStyle;
	line.nWidth = nWidth;
	line.nDotSize = 0;
	line.nGraphKind = nGraphKind;
	line.strName = strName;
	line.strGName = strGName;
	line.nVScaleKind = nVScaleKind;
	line.nDataId = theApp.GetDataCollection().GetSelectDataId(line.strName);

	// 線ID毎の情報マップにライン追加
	//m_DrawLines[nID] = line;
	AddLineToMap(nID, line, strName, nVScaleKind);

	return TRUE;
}
/*============================================================================*/
/*! グラフ描画

-# 点属性データの追加

@param nID		ラインＩＤ
@param crColor	ライン色
@param nStyle	ラインスタイル
@param nWidth	ライン幅
@param bVisible	ラインの表示/非表示

@retval	bool

*/
/*============================================================================*/
BOOL CGraphWindow::AddDot(UINT nID, CString strName, CString strGName, UINT nGraphKind, COLORREF crColor, int nDotSize, VScaleKind nVScaleKind, BOOL bVisible)
{
	// 点スタイルが異常の場合はデフォルト設定
	if (!((PS_STYLE_MASK)& nGraphKind))
		nGraphKind |= PS_SOLID;

	structLine line;

	line.nDrawKind = Dots;
	line.bIsVisible = bVisible;
	line.crColor = crColor;
	line.nStyle = PS_NULL;
	line.nWidth = 0;
	line.nDotSize = GetDotSize(nDotSize);
	line.nGraphKind = nGraphKind;
	line.strName = strName;
	line.strGName = strGName;
	line.nVScaleKind = nVScaleKind;
	line.nDataId = theApp.GetDataCollection().GetSelectDataId(line.strName);

	// 線ID毎の情報マップにライン追加
	AddLineToMap(nID, line, strName, nVScaleKind);

	return TRUE;
}
/*============================================================================*/
/*! グラフ描画

-# 線＆点属性データの追加

@param nID		ラインＩＤ
@param crColor	ライン色
@param nStyle	ラインスタイル
@param nWidth	ライン幅
@param bVisible	ラインの表示/非表示

@retval	bool

*/
/*============================================================================*/
BOOL CGraphWindow::AddLineAndDot(UINT nID, CString strName, CString strGName, UINT nGraphKind, COLORREF crColor, DWORD nStyle, int nWidth, int nDotSize, VScaleKind nVScaleKind, BOOL bVisible)
{
	// 線スタイルが異常の場合はデフォルト設定
	if (!((PS_STYLE_MASK)& nStyle))
		nStyle |= PS_SOLID;

	structLine line;

	line.nDrawKind = LineAndDots;
	line.bIsVisible = bVisible;
	line.crColor = crColor;
	line.nStyle = nStyle;
	line.nWidth = nWidth;
	line.nDotSize = GetDotSize(nDotSize);
	line.nGraphKind = nGraphKind;
	line.strName = strName;
	line.strGName = strGName;
	line.nVScaleKind = nVScaleKind;
	line.nDataId = theApp.GetDataCollection().GetSelectDataId(line.strName);

	// 線ID毎の情報マップにライン追加
	AddLineToMap(nID, line, strName, nVScaleKind);

	return TRUE;
}
/*============================================================================*/
/*! グラフ描画

-# 縦軸の追加

@param nScaleKind	縦軸スケール選択（右/左）
@param nStvScale	スケール
@param nVAxisScaleIntvl	メモリの間隔
@retval	bool

*/
/*============================================================================*/
BOOL CGraphWindow::AddAxis(stVScale nStvScale, HScale trandHScale)
{
	// 前回値を控えておく
	HScale		nOldTrandHScale;
	nOldTrandHScale = (HScale)m_nTrandHScale;

	m_nStvScale = nStvScale;
	if (m_nStvScale.Auto == TRUE ||
		m_nStvScale.VScalLMax > m_nStvScale.VScalLMin)
	{
		m_bUseVAxis[LScale] = TRUE;
	}
	else
	{
		m_bUseVAxis[LScale] = FALSE;
	}
	if (m_nStvScale.VScalRMax > m_nStvScale.VScalRMin)
	{
		m_bUseVAxis[RScale] = TRUE;
	}
	else
	{
		m_bUseVAxis[RScale] = FALSE;
	}

	if( m_nTrandHScale != trandHScale )
	{
		m_nTrendDataPos = 0;
	}

	m_nTrandHScale = (UINT)trandHScale;

	// 横軸領域設定
	m_dbHzComp = (trandHScale == autoscale) ? tblRealScaleInfo[0].cmp_rate : tblRealScaleInfo[trandHScale].cmp_rate;
	SetHzRange((trandHScale == autoscale) ? (tblRealScaleInfo[0].dsp_data * m_dbHzComp) : (tblRealScaleInfo[trandHScale].dsp_data * m_dbHzComp));

	// 前回と横軸設定が変わった場合は初期化
	if (nOldTrandHScale != (HScale)m_nTrandHScale)
	{
		InitSatelliteDataSetting();
	}

	return TRUE;
}


/*============================================================================*/
/*! グラフ描画

-# 線＆点属性データの削除

@param nID		ラインＩＤ

@retval

*/
/*============================================================================*/
void CGraphWindow::ClearLineAndDot(UINT nID)
{
	MutexLock();
	if (nID == 0xffffffff){
		// 全削除
		m_DrawLines.clear();
	}
	else{
		if (m_DrawLines.find(nID) != m_DrawLines.end()){
			m_DrawLines.erase(nID);
		}
	}
	MutexUnlock();
}
/*============================================================================*/
/*! グラフ描画

-# グラフの更新

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::UpdateGraph()
{
	ClearCursor();
	Normalize();
//	UpdateScrollInfo();
	Invalidate();

	RedrawWindow();
	//UpdateWindow();
}

/*============================================================================*/
/*! グラフ描画

-# カーソルをリセット

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::ResetCursor(int pos, bool reset/* = false*/)
{
	CRect crect;
	GetClientRect(crect);

	DrawShape pre = CDrawTool::c_drawShape;
	CDrawTool::c_drawShape = line;
	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	if (pTool == NULL){
		CDrawTool::c_drawShape = selection;
		return;
	}

	if( reset )
	{
		Remove(objline);
		CRect r = crect;
		if( pos < 0 )
			r.right = r.left = -1;
		else
			r.right = r.left + mGridMarginLeft + pos;
		pTool->Create(this, r);
		CDrawTool::c_drawShape = selection;
		return;
	}

	Select(objline);

	if( pos < 0 )
		pTool->MoveLineCursor(this, CRect(-1, crect.top, -1, crect.bottom));
	else
		pTool->MoveLineCursor(this, CRect(crect.left + mGridMarginLeft + pos, crect.top, crect.left + mGridMarginLeft + pos, crect.bottom));
	CDrawTool::c_drawShape = pre;
}

/*============================================================================*/
/*! グラフ描画

-# 矩形をリセット

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::ResetRectangle(LONG left, LONG right)
{
	bool	bRectangle = false;

	while( 1 )
	{
		POSITION	pos = m_objects.GetHeadPosition();

		while( pos != NULL )
		{
			CDrawObject	*pObj = m_objects.GetNext(pos);
			CRect position = pObj->m_position;

			if( pObj->GetShape() == CDrawRect::rectangle )
			{
				CRect	crect;
				GetClientRect(crect);

				position.top = crect.top + mGridMarginTop;
				position.bottom = crect.bottom - mGridMarginBottom;
				position.left = left + mGridMarginLeft;
				position.right = right + mGridMarginLeft;

				pObj->MoveTo(position, this);

				bRectangle = true;

				break;
			}
		}

		// 矩形が存在したら終了
		if( bRectangle )
			break;

		CPoint	point1(0, 0);
		CPoint	point2(1, 0);

		{
			// Ctrlキーと一緒のマウスイベントはズーム枠を作成
			// ズームカーソル
			Select(objrect);
			CDrawTool::c_drawShape = selection;
			// マウスカーソル位置がズーム枠内にあるかのチェック

			bool bsel=false;

			if(m_selection.GetCount() != 0){
				CDrawObject* pObj = m_selection.GetHead();
				if(pObj != NULL){
					CPoint local = point1;
					ClientToDoc(local);
//					int nHandle = pObj->HitTest(local, this, TRUE);
//					if(nHandle == 0)
//						bsel = true;
				}
			}

//			if(bsel == false){
				// 既存ズーム枠が選択状態でないので、既存ズーム枠を削除する。
				// その後、新規にズーム枠を作成する
				//RemoveAll(true);
				Select(objrect);
				Remove(objrect);
				CDrawTool::c_drawShape = rect;
//			}

			CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);

			if(pTool != NULL){
				pTool->OnLButtonDown(this, MK_CONTROL, point1);
			}
		}

		{
			CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
			if(pTool != NULL)
				pTool->OnMouseMove(this, MK_CONTROL, point1);
		}

		{
			CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
			if(pTool != NULL)
				pTool->OnMouseMove(this, MK_CONTROL, point2);
		}

		{
			CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
			if(pTool != NULL)
				pTool->OnLButtonUp(this, MK_CONTROL, point2);

			CDrawTool::c_drawShape = selection;
		}

		bRectangle = true;
	}
}

/*============================================================================*/
/*! グラフ描画

-# リミットをリセット

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::ResetLimit(int index, int kind, int upper, int lower, bool reset/* = false*/, bool bInvalidate/* = true*/)
{
	CRect crect;
	GetClientRect(crect);

	DrawShape pre = CDrawTool::c_drawShape;
	CDrawTool::c_drawShape = tblLimitDrawShape[index][kind];
	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	if (pTool == NULL){
		CDrawTool::c_drawShape = selection;
		return;
	}

	if( reset )
	{
		Remove(tblLimitObjectShape[index][kind]);
		CRect r = crect;
		if( (upper < 0) && (lower < 0) )
			r.bottom = r.top = -1;
		else
		{
			r.top = mGridMarginTop + upper;
			r.bottom = mGridMarginTop + lower;
		}
		r.left += mGridMarginLeft;
		r.right -= mGridMarginRight;
		pTool->Create(this, r);
		CDrawTool::c_drawShape = selection;
		return;
	}

	if( bInvalidate )
		Select(tblLimitObjectShape[index][kind]);

	if( (upper < 0) && (lower < 0) )
		pTool->MoveLimit(this, CRect(crect.left + mGridMarginLeft, -1, crect.right - mGridMarginRight + 1, -1), tblLimitObjectShape[index][kind], bInvalidate);
	else
		pTool->MoveLimit(this, CRect(crect.left + mGridMarginLeft, upper, crect.right - mGridMarginRight + 1, lower), tblLimitObjectShape[index][kind], bInvalidate);

	CDrawTool::c_drawShape = pre;
}

/*============================================================================*/
/*! グラフ描画

-# 値差をリセット

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::ResetDiffer(bool reset/* = false*/, bool bInvalidate/* = true*/)
{
	CRect crect;
	GetClientRect(crect);

	DrawShape pre = CDrawTool::c_drawShape;
	CDrawTool::c_drawShape = diff;
	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	if (pTool == NULL){
		CDrawTool::c_drawShape = selection;
		return;
	}

	if( reset )
	{
		m_nDifferMode = DifferNone;

#if 0
		Remove(objdiff);
		CRect r;
		r.right = r.left = -1;
		r.bottom = r.top = -1;
		pTool->Create(this, r);
#endif
		CDrawTool::c_drawShape = pre;
		return;
	}

	if( bInvalidate )
		Select(objdiff);

	pTool->MoveDiffer(this, bInvalidate);

	CDrawTool::c_drawShape = pre;
}

void CGraphWindow::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// ツールチップコントロールを非表示
	if( m_pToolTipCtrl->GetSafeHwnd() )
		m_pToolTipCtrl->Pop();

	if( m_nGraphType == eGraphType_All )
	{
		// リアルタイムモード⇔ノンリアルタイムモードを切り替え
		((CMainFrame *)theApp.m_pMainWnd)->GetPropertiesWnd().ChangeViewMode();
#if 0
		if (nFlags & MK_CONTROL){
			CRect	rect;
			GetClientRect(rect);
			if (rect.PtInRect(point)){
				if (theApp.GetTrendScrollMode(m_nViewNo) == false){
					// 解析モード突入
					theApp.SetTrendScrollMode(m_nViewNo, true);
					m_ptTrendMove = point;
					//SetCapture();
					//SetCursor(theApp.LoadStandardCursor(IDC_HAND));
				}
				else{
					// 解析モード解除
					//if (this->GetCapture() == this)
					{
						theApp.SetTrendScrollMode(m_nViewNo, false);
						//ReleaseCapture();
						//SetCursor(theApp.LoadStandardCursor(IDC_ARROW));
					}
				}
			}
		}
#endif
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CGraphWindow::OnLButtonDown(UINT nFlags, CPoint point)
{
	bool	bRealTime = ((m_bIsRealTime == FALSE) && (theApp.IsAnalysisMigration(m_nWindowNo) == false)) ? false : true;

	// ツールチップコントロールを非表示
	if( m_pToolTipCtrl->GetSafeHwnd() )
		m_pToolTipCtrl->Pop();

	if( (m_nGraphType == eGraphType_All) && (!bRealTime) && (theApp.GetDataCollection().GetAnalysisCount(m_nWindowNo) != 0) )
	{
		m_preMousePoint = CPoint(-1, -1);

		// ズーム操作
		if( (m_nViewID == eViewID_1) && (nFlags & MK_CONTROL) && (!(nFlags & MK_SHIFT)) )
		{
			m_resDrawObject = 0;

			CDrawTool::c_drawShape = selection;

			// マウスカーソルがズーム枠内にあるか？
			bool	bSelect = false;
			if( m_selection.GetCount() != 0 )
			{
				CDrawObject	*pObj = m_selection.GetHead();

				if( pObj != NULL )
				{
					CPoint	local = point;

					ClientToDoc(local);

					int	nHandle = pObj->HitTest(local, this, TRUE);

					if( nHandle == 0 )
						bSelect = true;
				}
			}

			// 選択状態
			if( bSelect )
			{
				m_relZoomLeft = CPoint(GetZoomLeft() - point.x, point.y);
			}
			// 非選択状態
			else
			{
				CRect	rectClient;
				GetClientRect(rectClient);
	
				// 軸領域を取得
				rectClient.SetRect(
					rectClient.left + mGridMarginLeft,
					rectClient.top + mGridMarginTop,
					rectClient.right - mGridMarginRight,
					rectClient.bottom - mGridMarginBottom);

				// 軸領域の中にマウスカーソルの位置を収納
				if( rectClient.left > point.x )
					point.x = rectClient.left;
				if( rectClient.right < point.x )
					point.x = rectClient.right;

				m_absZoomStart = point;

				Select(objrect);
				Remove(objrect);
				CDrawTool::c_drawShape = rect;
			}

			// ズーム枠を更新
			CDrawTool	*pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
			if( pTool != NULL )
				pTool->OnLButtonDown(this, nFlags, point);
		}
		// 値差操作
		else if( (!(nFlags & MK_CONTROL)) && (nFlags & MK_SHIFT) )
		{
			m_resDrawObject = 0;

			CDrawTool::c_drawShape = selection;

			CRect	rectClient;
			GetClientRect(rectClient);
	
			// 軸領域を取得
			rectClient.SetRect(
				rectClient.left + mGridMarginLeft,
				rectClient.top + mGridMarginTop,
				rectClient.right - mGridMarginRight,
				rectClient.bottom - mGridMarginBottom);

			// 軸領域の中にマウスカーソルの位置を収納
			if( rectClient.left > point.x )
				point.x = rectClient.left;
			if( rectClient.right < point.x )
				point.x = rectClient.right;

			// 値差の始点を検出
			m_nSelDifferNo = 0;
			m_nDifferMode = DifferNone;
			m_nDifferDataNo[m_nSelDifferNo] = -1;

			CreateDifferDataPosThread(point);

			while( !m_bDifferDataPosStop )
				Sleep(5);

			if( m_nDifferDataNo[m_nSelDifferNo] != -1 )
			{
				Select(objdiff);
				Remove(objdiff);
				CDrawTool::c_drawShape = diff;

				// 値差の終点検出を準備
				m_nSelDifferNo = 1;
				m_nDifferDataNo[1] = m_nDifferDataNo[0];
				m_nDifferDataPos[1] = m_nDifferDataPos[0];
				m_ptDifferData[1] = m_ptDifferData[0];

				// 枠を更新
				CDrawTool	*pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
				if( pTool != NULL )
					pTool->OnLButtonDown(this, nFlags, m_ptDifferData[0]);
			}
		}
		// ライン操作
		else if( !(nFlags & MK_CONTROL) )
		{
			CDrawTool::c_drawShape = selection;

			m_resDrawObject = 0;

			Select(objline);
			CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
			if(pTool != NULL)
				pTool->OnLButtonDown(this, nFlags, point);

			// カーソルが選択されていなければ作成する
			if( m_resDrawObject == 0 )
			{
				CChildFrame	*pWnd = (CChildFrame *)GetChildFrame();

				// ラインを移動
				if( pWnd != NULL )
				{
					pWnd->m_nCursor = ConvXToNo(point.x) + ((m_nViewID == eViewID_1) ? 0 : theApp.GetDataCollection().GetTrendZoomStart(m_nWindowNo));
					pWnd->PostMessage(eMessage_MoveCursorLine);
				}

				m_resDrawObject = 4;

				return;
			}
		}

		return;
	}

#if 0
	if (m_nGraphType == eGraphType_OffAll){
		// ズームモード時の処理
		if (nFlags & MK_CONTROL){
			// Ctrlキーと一緒のマウスイベントはズーム枠を作成
			// ズームカーソル
			Select(objrect);
			CDrawTool::c_drawShape = selection;
			// マウスカーソル位置がズーム枠内にあるかのチェック
			bool bsel = false;
			if (m_selection.GetCount() != 0){
				CDrawObject* pObj = m_selection.GetHead();
				if (pObj != NULL){
					CPoint local = point;
					ClientToDoc(local);
					int nHandle = pObj->HitTest(local, this, TRUE);
					if (nHandle == 0)
						bsel = true;
				}
			}
			if (bsel == false){
				// 既存ズーム枠が選択状態でないので、既存ズーム枠を削除する。
				// その後、新規にズーム枠を作成する
				Select(objrect);
				Remove(objrect);
				CDrawTool::c_drawShape = rect;
			}
			CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
			if (pTool != NULL){
				pTool->OnLButtonDown(this, nFlags, point);
			}
			return;
		}
		else{
			// マウスカーソル位置がカーソル枠内にあるかのチェック
			bool bsel = false;
			if (m_selection.GetCount() != 0){
				CDrawObject* pObj = m_selection.GetHead();
				if (pObj != NULL){
					CPoint local = point;
					ClientToDoc(local);
					int nHandle = pObj->HitTest(local, this, TRUE);
					if (nHandle == 0)
						bsel = true;
				}
			}
			if (bsel == false){
				// カーソ作成
				ResetCursor(point.x);
				return;
			}
			CDrawTool::c_drawShape = selection;
			CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
			if (pTool != NULL){
				pTool->OnLButtonDown(this, nFlags, point);
			}
			return;
		}
	}

	if (m_nGraphType == eGraphType_OffTrend){
		if (nFlags & MK_CONTROL && !(nFlags & MK_SHIFT)){
			int inc = GetTrendZoomInc();
			SetTrendZoomInc(inc * 2);
			OnZoom(true);
		}
		else if (nFlags & MK_CONTROL && nFlags & MK_SHIFT){
			int inc = GetTrendZoomInc();
			inc /= 2;
			if (inc == 0)	inc = 1;
			SetTrendZoomInc(inc);
			OnZoom(false);
		}
		else{
			SetFocus();
			int pos = (int)(m_pointTop / m_nTrendInc);
			if (pos != 0)
				pos--;
			int endpos = (int)((m_dbHzRange + m_pointTop) / m_nTrendInc);
			pair<double, double> pnt;
			CPoint pointDraw;
			DRAWLINE::const_iterator iterLine = m_DrawLines.begin();
			m_ZoomPos = CPoint(0, 0);
			while ((pos != endpos + 2) && (pos != (int)m_dbHzRange)){
				pnt.first = (double)((double)pos*m_nTrendInc); pnt.second = (double)0;
				pointDraw = ConvPoint(pnt, (*iterLine).second.nMaxValue, (*iterLine).second.nMinValue);
				if (pointDraw.x >= point.x){
					m_CursorPos = pos;
					if (m_CursorPos < 1)
						m_CursorPos = 1;
					//TRACE("@@@ Trend Cursor[%d:%d]\n", m_CursorPos, pos);
					Invalidate();
					break;
				}
				pos++;
			}
		}
	}
#endif

	CWnd::OnLButtonDown(nFlags, point);
}


void CGraphWindow::OnMouseMove(UINT nFlags, CPoint point)
{
	bool	bRealTime = ((m_bIsRealTime == FALSE) && (theApp.IsAnalysisMigration(m_nWindowNo) == false)) ? false : true;

	if( (m_nGraphType == eGraphType_All) && (!bRealTime) && (theApp.GetDataCollection().GetAnalysisCount(m_nWindowNo) != 0) )
	{
		CRect	rectClient;
		GetClientRect(rectClient);
	
		// 軸領域を取得
		rectClient.SetRect(
			rectClient.left + mGridMarginLeft,
			rectClient.top + mGridMarginTop,
			rectClient.right - mGridMarginRight,
			rectClient.bottom - mGridMarginBottom);

		// 軸領域の中にマウスカーソルの位置を収納
		if( rectClient.left > point.x )
			point.x = rectClient.left;
		if( rectClient.right < point.x )
			point.x = rectClient.right;

		// X軸方向に変動が無ければスルー
		if( point.x == m_preMousePoint.x )
		{
			m_preMousePoint = point;

			CWnd::OnMouseMove(nFlags, point);

			return;
		}
		m_preMousePoint = point;

		// 選択状態
		if( (GetCapture() == this) && (m_resDrawObject != 0) )
		{
			// ズーム操作
			if( (m_nViewID == eViewID_1) && (m_resDrawObject == 3) )
				Select(objrect);
			// ライン操作
			else if( m_resDrawObject == 4 )
				Select(objline);

			CDrawTool	*pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
			if( pTool != NULL )
			{
				pTool->OnMouseMove(this, nFlags, point);

				// ズーム操作
				if( (m_nViewID == eViewID_1) && (m_resDrawObject == 3) )
				{
					CChildFrame	*pWnd = (CChildFrame *)GetChildFrame();

					// ズーム枠を移動
					if( pWnd != NULL )
					{
						pWnd->m_nZoomStart = ConvXToNo(m_relZoomLeft.x + point.x);
						pWnd->PostMessage(eMessage_MoveZoomRectangle);
					}
				}
				// ライン操作
				else if( m_resDrawObject == 4 )
				{
					CChildFrame	*pWnd = (CChildFrame *)GetChildFrame();

					// ラインを移動
					if( pWnd != NULL )
					{
						pWnd->m_nCursor = ConvXToNo(point.x) + ((m_nViewID == eViewID_1) ? 0 : theApp.GetDataCollection().GetTrendZoomStart(m_nWindowNo));
						pWnd->PostMessage(eMessage_MoveCursorLine);
					}
				}
			}
		}
		// 非選択状態
		else
		{
			m_resDrawObject = 0;

			Select(objrect);
			CDrawTool	*pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
			if( pTool != NULL )
				pTool->OnMouseMove(this, nFlags, point);

			// ズーム操作
			if( (m_nViewID == eViewID_1) && (GetCapture() == this) && (pTool != NULL) && (pTool->m_drawShape == rect) )
			{
				CChildFrame	*pWnd = (CChildFrame *)GetChildFrame();

				if( pWnd != NULL )
				{
					// ズーム枠を作成
					if( m_absZoomStart.x <= point.x )
					{
						pWnd->m_nZoomStart = ConvXToNo(m_absZoomStart.x);
						pWnd->m_nZoomTblNo = GetValidTblNo(ConvXToNo(point.x) - pWnd->m_nZoomStart + 1, theApp.GetDataCollection().GetTrendAllTblNo(m_nWindowNo));
					}
					else
					{
						pWnd->m_nZoomStart = ConvXToNo(point.x);
						pWnd->m_nZoomTblNo = GetValidTblNo(ConvXToNo(m_absZoomStart.x) - pWnd->m_nZoomStart + 1, theApp.GetDataCollection().GetTrendAllTblNo(m_nWindowNo));
						if( ConvXToNo(m_absZoomStart.x) < (tblRealScaleInfo[pWnd->m_nZoomTblNo].dsp_data - 1) )
							pWnd->m_nZoomStart = 0;
						else
							pWnd->m_nZoomStart = ConvXToNo(m_absZoomStart.x) - (tblRealScaleInfo[pWnd->m_nZoomTblNo].dsp_data - 1);
					}
					pWnd->PostMessage(eMessage_CreateZoomRectangle);
				}
			}
			else
			{
				if( m_resDrawObject == 0 )
				{
					m_resDrawObject = 0;

					Select(objdiff);
					CDrawTool	*pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
					if( pTool != NULL )
						pTool->OnMouseMove(this, nFlags, m_ptDifferData[m_nSelDifferNo]);

					// 値差操作
					if( (GetCapture() == this) && (pTool != NULL) && (pTool->m_drawShape == diff) )
					{
						// 値差の終点を検出
						CreateDifferDataPosThread(point);
					}
				}
			}

			if( m_resDrawObject == 0 )
			{
				Select(objline);
				pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
				if(pTool != NULL)
					pTool->OnMouseMove(this, nFlags, point);
			}
		}

#if 0
		if (theApp.GetTrendScrollMode(m_nViewNo) == true){
			if (nFlags & MK_LBUTTON){
				CRect	rect;
				GetClientRect(rect);
				if (m_ptTrendMove.x >= point.x){
					// 右方向
					m_nTrendDataPos += 1;
#if 0
					if (size <= m_nTrendDataPos)
						m_nTrendDataPos = (size - 1);
#endif
				}
				else{
					// 左方向
					if ((int)((int)m_nTrendDataPos - 1) < 0)
						m_nTrendDataPos = 0;
					else
						m_nTrendDataPos -= 1;
				}
				UpdateGraph();
				m_ptTrendMove = point;
			}
		}
#endif

		// ツールチップコントロールを表示
		if( ((nFlags & (MK_LBUTTON | MK_RBUTTON)) == 0) && m_pToolTipCtrl->GetSafeHwnd() )
		{
			static	POINT	pt = { 0, 0 };
			MSG	msg;

			msg.hwnd    = GetSafeHwnd();
			msg.message = WM_MOUSEMOVE;
			msg.wParam  = nFlags;
			msg.lParam  = MAKELPARAM(LOWORD(point.x), LOWORD(point.y));
			msg.time    = 0;
			msg.pt.x    = point.x;
			msg.pt.y    = point.y;

			if( CPoint(pt) != point )
			{
				m_pToolTipCtrl->Pop();
				pt = point;
			}

			m_pToolTipCtrl->RelayEvent(&msg);
			m_pToolTipCtrl->UpdateTipText(_T(""), this);

			// データの表示位置からデータを特定するスレッドを起動
			CreateCommentDataPosThread(point);
		}

		return;
	}

#if 0
	CRect	rect;
	GetClientRect(rect);
	if (rect.left > point.x){
		point.x = rect.left;
	}
	if (rect.right < point.x){
		point.x = rect.right;
	}

	if (m_nGraphType == eGraphType_OffAll){
		CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
		if (pTool != NULL)
			pTool->OnMouseMove(this, nFlags, point);
		return;
	}
#endif

	CWnd::OnMouseMove(nFlags, point);
}


void CGraphWindow::OnLButtonUp(UINT nFlags, CPoint point)
{
	bool	bRealTime = ((m_bIsRealTime == FALSE) && (theApp.IsAnalysisMigration(m_nWindowNo) == false)) ? false : true;

	if( (m_nGraphType == eGraphType_All) && (!bRealTime) && (theApp.GetDataCollection().GetAnalysisCount(m_nWindowNo) != 0) )
	{
		// ズーム操作
		if( (m_nViewID == eViewID_1) && (m_resDrawObject == 3) )
		{
			Select(objrect);
			CDrawTool	*pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
			if( pTool != NULL )
				pTool->OnLButtonUp(this, nFlags, point);

			if( (pTool != NULL) && (pTool->c_drawShape == selection) && (pTool->m_drawShape == rect) )
			{
				CChildFrame	*pWnd = (CChildFrame *)GetChildFrame();

				// ズーム枠を移動
				if( pWnd != NULL )
				{
					pWnd->m_nZoomStart = ConvXToNo(m_relZoomLeft.x + point.x);
					pWnd->PostMessage(eMessage_MoveZoomRectangle);
				}
			}
		}
		// ライン操作
		else if( m_resDrawObject == 4 )
		{
			Select(objline);
			CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
			if(pTool != NULL)
				pTool->OnLButtonUp(this, nFlags, point);

			if( (pTool != NULL) && (pTool->c_drawShape == selection) && (pTool->m_drawShape == line) )
			{
				CChildFrame	*pWnd = (CChildFrame *)GetChildFrame();

				// ラインを移動
				if( pWnd != NULL )
				{
					pWnd->m_nCursor = ConvXToNo(point.x) + ((m_nViewID == eViewID_1) ? 0 : theApp.GetDataCollection().GetTrendZoomStart(m_nWindowNo));
					pWnd->PostMessage(eMessage_MoveCursorLine);
				}
			}
		}
		else
		{
			Select(objrect);
			CDrawTool	*pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
			if( pTool != NULL )
				pTool->OnLButtonUp(this, nFlags, point);

			CDrawTool::c_drawShape = selection;

			// ズーム操作
			if( (m_nViewID == eViewID_1) && (pTool != NULL) && (pTool->c_drawShape == selection) && (pTool->m_drawShape == rect) )
			{
				CChildFrame	*pWnd = (CChildFrame *)GetChildFrame();

				if( pWnd != NULL )
				{
					// ズーム枠を作成
					if( m_absZoomStart.x <= point.x )
					{
						pWnd->m_nZoomStart = ConvXToNo(m_absZoomStart.x);
						pWnd->m_nZoomTblNo = GetValidTblNo(ConvXToNo(point.x) - pWnd->m_nZoomStart + 1, theApp.GetDataCollection().GetTrendAllTblNo(m_nWindowNo));
					}
					else
					{
						pWnd->m_nZoomStart = ConvXToNo(point.x);
						pWnd->m_nZoomTblNo = GetValidTblNo(ConvXToNo(m_absZoomStart.x) - pWnd->m_nZoomStart + 1, theApp.GetDataCollection().GetTrendAllTblNo(m_nWindowNo));
						if( ConvXToNo(m_absZoomStart.x) < (tblRealScaleInfo[pWnd->m_nZoomTblNo].dsp_data - 1) )
							pWnd->m_nZoomStart = 0;
						else
							pWnd->m_nZoomStart = ConvXToNo(m_absZoomStart.x) - (tblRealScaleInfo[pWnd->m_nZoomTblNo].dsp_data - 1);
					}
					pWnd->PostMessage(eMessage_CreateZoomRectangle);
				}
			}
			else
			{
				Select(objdiff);
				CDrawTool	*pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
				if( pTool != NULL )
					pTool->OnLButtonUp(this, nFlags, point);

				CDrawTool::c_drawShape = selection;

				RemoveDifferDataPosThread();
			}
		}

		return;
	}

#if 0
	if (m_nGraphType == eGraphType_OffAll){
		CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
		if (pTool != NULL)
			pTool->OnLButtonUp(this, nFlags, point);

		CDrawTool::c_drawShape = selection;
		return;
	}
#endif

	CWnd::OnLButtonUp(nFlags, point);
}

void CGraphWindow::DrawZoom(CDC* pDC, CRect drect)
{
	CRect rect = drect;
	//---------------------------------------------------------------------------
	//	選択された領域の枠をペン描画
	//---------------------------------------------------------------------------
	if (rect.right < 0)
		return;
	rect.DeflateRect(mZoomWidth - 2, mZoomWidth - 2);
	CPen pen;
	COLORREF cf = mZoomColor;
	if (pen.CreatePen(PS_SOLID, mZoomWidth, cf)){
		int old = pDC->SetROP2(R2_XORPEN);
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		if (rect.left >= rect.right - (LONG)mZoomWidth){
			pDC->MoveTo(rect.left, rect.top);
			pDC->LineTo(rect.left, rect.bottom);
		}
		else{
			pDC->MoveTo(rect.left, rect.top);
			pDC->LineTo(rect.left, rect.bottom);
			pDC->MoveTo(rect.right, rect.top);
			pDC->LineTo(rect.right, rect.bottom);

			pDC->MoveTo(rect.left, rect.top);
			pDC->LineTo(rect.right, rect.top);
			pDC->MoveTo(rect.left, rect.top);
			pDC->LineTo(rect.left, rect.top);
			pDC->MoveTo(rect.right, rect.top);
			pDC->LineTo(rect.right, rect.top);

			pDC->MoveTo(rect.left, rect.bottom);
			pDC->LineTo(rect.right, rect.bottom);
			pDC->MoveTo(rect.left, rect.bottom);
			pDC->LineTo(rect.left, rect.bottom);
			pDC->MoveTo(rect.right, rect.bottom);
			pDC->LineTo(rect.right, rect.bottom);
		}
		pDC->SetROP2(old);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}
}


void CGraphWindow::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	ClearCursor();
}


#if 0
/*
/*============================================================================*/
/*! グラフ描画

-# スクロール情報の更新

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::UpdateScrollInfo()
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	TRY
	{
		si.fMask = SIF_ALL;
		si.nPos = (int)m_nTrendDataPos;
		si.nMin = 0;
#if 0
		si.nMax = size;
#endif
		si.nPage = (UINT)tblRealScaleInfo[m_nTrandHScale].dsp_data;
	}CATCH_ALL(e){
		return;
	} END_CATCH_ALL

	SetScrollInfo(SB_HORZ, &si, TRUE);
	SendNewLimits();
}
*/
#endif

/*============================================================================*/
/*! グラフ描画

-# 線端数制限を行う

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::SendNewLimits()
{
	return;
	GetParent()->PostMessage(WM_NEWHORZLIMITS,
		(WPARAM)m_nTrendDataPos,
		(LPARAM)(m_nTrendDataPos + m_dbHzRange));
//	GetParent()->PostMessage(WM_NEWVERTLIMITS,
//		(WPARAM)m_dbMinValue, (LPARAM)m_dbMaxValue);
	GetParent()->PostMessage(WM_NEWVERTLIMITS,
		(WPARAM)GetVAxisMin(), (LPARAM)GetVAxisMax());
}

#if 0
/*
void CGraphWindow::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	switch (nSBCode) {

	case SB_LINELEFT:
		if (m_nTrendDataPos < 1)
			return;
		m_nTrendDataPos -= 1;
		if (m_nTrendDataPos < 0)
			m_nTrendDataPos = 0;
		break;

	case SB_LINERIGHT:
		m_nTrendDataPos += 1;
#if 0
		if (size <= m_nTrendDataPos)
			m_nTrendDataPos = (size - 1);
#endif
		break;

	case SB_PAGEUP:
		if (m_nTrendDataPos < 10)
			return;
		m_nTrendDataPos -= 10;
		if (m_nTrendDataPos < 0)
			m_nTrendDataPos = 0;
		break;

	case SB_PAGERIGHT:
		m_nTrendDataPos += 10;
#if 0
		if (size <= m_nTrendDataPos)
			m_nTrendDataPos = (size - 1);
#endif
		break;

	case SB_THUMBTRACK:
		m_nTrendDataPos = nPos;
#if 0
		if (size <= m_nTrendDataPos)
			m_nTrendDataPos = (size - 1);
#endif
		break;
	default:
		return;
	}

	Invalidate();
	UpdateScrollInfo();
//	UpdateWindow();

	//CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}
*/
#endif

/*============================================================================*/
/*! グラフ描画

-# 正常位置の更新

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::Normalize()
{
	if (m_pointTop < m_dbMinHzVal)
		m_pointTop = m_dbMinHzVal;
	if (m_dbMaxHzVal < m_pointTop + m_dbHzRange)
		m_dbMaxHzVal = m_pointTop + m_dbHzRange;
}

/*============================================================================*/
/*! グラフ描画

-# ズームイベント

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::OnZoom(bool bZoomIn)
{
	if (m_nGraphType != eGraphType_OffTrend){
		return;
	}

	int pos = (int)(m_pointTop / m_nTrendInc);
	int endpos = (int)((m_dbHzRange + m_pointTop) / m_nTrendInc);
	int cpos = m_CursorPos - ((endpos - pos) / 2) + 1;
	m_pointTop = (double)((double)(m_CursorPos - 1)*m_nTrendInc) - (m_dbHzRange / 2.0);
	double dbTrendPos = (double)((double)cpos*m_nTrendInc);
	if (dbTrendPos < m_pointTop || dbTrendPos >(m_dbHzRange + m_pointTop)){
		// カーソルが枠外のためスクロール情報を更新する
		if (dbTrendPos < m_pointTop){
			m_pointTop = dbTrendPos;
		}
		if (dbTrendPos >(m_dbHzRange + m_pointTop)){
			m_pointTop = dbTrendPos - m_dbHzRange;
		}
	}
	if (m_pointTop < 0)
		m_pointTop = 0;

	//m_MaxHzVal = (double)(((size-1)*m_nTrendInc) + m_nHzRange * .1);
	m_dbMaxHzVal = (double)((double)((double)(m_dbHzRange - 1)*m_nTrendInc) + (double)m_dbHzRange * (double).1);
	Normalize();

	Invalidate();
//	UpdateScrollInfo();
//	UpdateWindow();
}



void CGraphWindow::ClientToDoc(CPoint& point)
{
}

void CGraphWindow::ClientToDoc(CRect& rect)
{
	CRect rc;
	GetClientRect(rc);
//	rect.top = rc.top;
//	rect.bottom = rc.bottom;
	if (rc.left > rect.left)
		rect.left = rc.left;
	if (rc.right < rect.right)
		rect.right = rc.right;
}

void CGraphWindow::DocToClient(CPoint& point)
{
}

void CGraphWindow::DocToClient(CRect& rect)
{
	rect.NormalizeRect();
}

void CGraphWindow::InvalObj(CDrawObject* pObj)
{
	CRect rect = pObj->m_position;
	DocToClient(rect);
	rect.InflateRect(1, 1);

	InvalidateRect(rect, FALSE);
}

/*============================================================================*/
/*! イメージツール

-# 項目の選択解除

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::Deselect(CDrawObject* pObj)
{
	POSITION pos = m_selection.Find(pObj);
	if (pos != NULL){
		InvalObj(pObj);
		m_selection.RemoveAt(pos);
	}
}
/*============================================================================*/
/*! イメージツール

-# オブジェクトの取得

@param
@retval

*/
/*============================================================================*/
CDrawObject* CGraphWindow::ObjectAt(const CPoint& point)
{
	CRect rect(point, CSize(1, 1));
	POSITION pos = m_objects.GetTailPosition();
	while (pos != NULL)
	{
		CDrawObject* pObj = m_objects.GetPrev(pos);
		if (pObj->Intersects(rect))
			return pObj;
	}

	return NULL;
}

BOOL CGraphWindow::IsSelected(const CObject* pDocItem) const
{
	CDrawObject* pDrawObj = (CDrawObject*)pDocItem;
	return m_selection.Find(pDrawObj) != NULL;
}

/*============================================================================*/
/*! イメージツール

-# 項目の選択

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::Select(CDrawObject* pObj, BOOL bAdd)
{
	if (!bAdd){
		OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		m_selection.RemoveAll();
	}

	if (pObj == NULL || IsSelected(pObj)){
		return;
	}

	m_selection.AddTail(pObj);
	InvalObj(pObj);
}

void CGraphWindow::Select(ObjectShape type)
{
	POSITION pt = m_objects.GetHeadPosition();
	while (pt != NULL){
		CDrawObject* pObj = m_objects.GetNext(pt);
		if (pObj->GetShape() == type){
			Select(pObj);
		}
	}
}

/*============================================================================*/
/*! イメージツール

-# 矩形による項目の選択

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::SelectWithinRect(CRect rect, BOOL bAdd)
{
	if (!bAdd)
		Select(NULL);

	ClientToDoc(rect);

	CDrawObjList* pObList = &m_objects;
	POSITION posObj = pObList->GetHeadPosition();
	while (posObj != NULL){
		CDrawObject* pObj = pObList->GetNext(posObj);
		if (pObj->Intersects(rect))
			Select(pObj, TRUE);
	}
}

/*============================================================================*/
/*! イメージツール

-# オブジェクトの追加

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::Add(CDrawObject* pObj)
{
	m_objects.AddTail(pObj);
	//pObj->m_pDocument = this;
}

/*============================================================================*/
/*! イメージツール

-# オブジェクトの削除

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::Remove(CDrawObject* pObj)
{
	POSITION pos = m_objects.Find(pObj);
	if (pos != NULL)
		m_objects.RemoveAt(pos);

	pos = m_selection.Find(pObj);
	if (pos != NULL){
		InvalObj(pObj);
		m_selection.RemoveAt(pos);
	}
}

void CGraphWindow::Remove(ObjectShape type)
{
	CDrawTool::c_drawShape = selection;
	if (type == objrect)
		CDrawTool::c_drawShape = rect;
	if (type == objline)
		CDrawTool::c_drawShape = line;
	if (type == objdiff)
		CDrawTool::c_drawShape = diff;

	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	if (pTool == NULL){
		CDrawTool::c_drawShape = selection;
		return;
	}

	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL){
		CDrawObject* pObj = m_objects.GetNext(pos);
		if (pObj->GetShape() == type){
			Remove(pObj);
			pObj->Remove();
		}
	}
}

/*============================================================================*/
/*! イメージツール

-# オブジェクトの全削除

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::RemoveAll(bool bInit)
{
	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL){
		CDrawObject* pObj = m_objects.GetNext(pos);
		if (bInit == false){
			UINT type = ((CDrawRect*)pObj)->GetShape();
			if (((CDrawRect*)pObj)->IsFunctionNone() == FALSE && ((CDrawRect*)pObj)->IsFunction() == FALSE)
				continue;
		}
		Remove(pObj);
		pObj->Remove();
	}
}
void CGraphWindow::OnUpdate(CView*, LPARAM lHint, CObject* pHint)
{
	switch (lHint)
	{
	case HINT_UPDATE_WINDOW:
		Invalidate(FALSE);
		break;

	case HINT_UPDATE_DRAWOBJ:
		InvalObj((CDrawObject*)pHint);
		break;

	case HINT_UPDATE_SELECTION:
	{
								  CDrawObjList* pList = pHint != NULL ?
									  (CDrawObjList*)pHint : &m_selection;
								  POSITION pos = pList->GetHeadPosition();
								  while (pos != NULL)
								  {
									  InvalObj(pList->GetNext(pos));
								  }
	}
		break;

	case HINT_DELETE_SELECTION:
		if (pHint != &m_selection)
		{
			CDrawObjList* pList = (CDrawObjList*)pHint;
			POSITION pos = pList->GetHeadPosition();
			while (pos != NULL)
			{
				CDrawObject* pObj = pList->GetNext(pos);
				InvalObj(pObj);
				Remove(pObj);
			}
		}
		break;

	default:
		ASSERT(FALSE);
		break;
	}
}
void CGraphWindow::DrawObj(CDC* pDC, bool bRectangle, bool bLine, bool bLimit, bool bDiffer)
{
	if( bRectangle )
	{
		POSITION pos = m_objects.GetHeadPosition();
		while (pos != NULL)
		{
			CDrawObject* pObj = m_objects.GetNext(pos);
			if( pObj->GetShape() != objrect )
				continue;
			pObj->Draw(pDC, FALSE);
		}
	}
	if( bLine )
	{
		POSITION pos = m_objects.GetHeadPosition();
		while (pos != NULL)
		{
			CDrawObject* pObj = m_objects.GetNext(pos);
			if( pObj->GetShape() != objline )
				continue;
			pObj->Draw(pDC, FALSE);
		}
	}
	if( bLimit )
	{
		POSITION pos = m_objects.GetHeadPosition();
		while (pos != NULL)
		{
			CDrawObject* pObj = m_objects.GetNext(pos);
			if( (pObj->GetShape() < objlimit_warning_01) || (objlimit_alarm_10 < pObj->GetShape()) )
				continue;
			pObj->Draw(pDC, FALSE);
		}
	}
	if( bDiffer )
	{
		POSITION pos = m_objects.GetHeadPosition();
		while (pos != NULL)
		{
			CDrawObject* pObj = m_objects.GetNext(pos);
			if( pObj->GetShape() != objdiff )
				continue;
			pObj->Draw(pDC, FALSE);
		}
	}
}

// ドットサイズを取得する
int CGraphWindow::GetDotSize(int nDotSize)
{
	switch (nDotSize)
	{
	case Medium:
		return eDotSize_MEDIUM;
	case Large:
		return eDotSize_LARGE;
	case Small:
	default:
		return eDotSize_SMALL;
	}

	return 0;
}

/*============================================================================*/
/*! グラフ描画

-# 縦軸左端と右端で最大値を取得

@param	なし
@retval	縦軸の最大値

*/
/*============================================================================*/
double CGraphWindow::GetVAxisMax()
{
	double ret = m_nStvScale.VScalLMax;

	if (m_bUseVAxis[RScale] == TRUE)
	{
		if (m_nStvScale.VScalLMax > m_nStvScale.VScalRMax)
		{
			ret = m_nStvScale.VScalLMax;
		}
		else
		{
			ret = m_nStvScale.VScalRMax;
		}
	}

	return ret;
}
/*============================================================================*/
/*! グラフ描画

-# 縦軸左端と右端で最小値を取得

@param	なし
@retval	縦軸の最小値

*/
/*============================================================================*/
double CGraphWindow::GetVAxisMin()
{
	double ret = m_nStvScale.VScalLMin;

	if (m_bUseVAxis[RScale] == TRUE)
	{
		if (m_nStvScale.VScalLMin < m_nStvScale.VScalRMin)
		{
			ret = m_nStvScale.VScalLMin;
		}
		else
		{
			ret = m_nStvScale.VScalRMin;
		}
	}

	return ret;
}

/*============================================================================*/
/*! グラフ描画

-# 数値にカンマを追加

@param	カンマを付けたい数値
@retval	カンマを付けた後の数値

*/
/*============================================================================*/
//-- 20181022 グラフ縦軸の数値化け修正 >>>
//CString CGraphWindow::addCanma(double pos)
//{
//	int	i, j, keta, temp;
//	int sign = 0;
//	double tempPos = pos;
//	char str[MAXDIGIT];
//
//	// 0に近い場合はまるめる
//	if (tempPos > -0.0000001 && tempPos < 0.0000001)
//	{
//		tempPos = 0.0;
//	}
//
//	long num = (long)tempPos;
//
//	// 少数部を6桁にする
//	double dnum = fabs(tempPos - (double)num);
//	CString strSyosuu;
//	strSyosuu.Format(_T("%.6f"), dnum);
//	strSyosuu.Delete(0);
//
//	// 正負を調べる
//	if (tempPos < 0)
//	{
//		sign = 1;
//		num = -num;
//	}
//
//	// 整数部にカンマを付ける
//	i = 0;
//	keta = 0;
//
//	do
//	{
//		str[i++] = num % 10 + '0';
//		keta++;
//		num /= 10;
//		if (keta % 3 == 0 && num != 0)
//		{
//			str[i++] = ',';
//		}
//	} while (num != 0 && i < sizeof(str));
//
//	if (i >= MAXDIGIT)
//	{
//		return CString(_T("#,###,###,###.######"));
//	}
//
//	if (sign == 1)
//	{
//		if (i + 1 >= MAXDIGIT)
//		{
//			return CString(_T("#,###,###,###.######"));
//		}
//		str[i++] = '-';
//	}
//
//	str[i] = '\0';
//
//	j = i - 1;
//	for (i = 0; j > i; i++, j--)
//	{
//		temp = str[i];
//		str[i] = str[j];
//		str[j] = temp;
//	}
//
//	return CString(str) + strSyosuu;
//}
//-- 20181022 グラフ縦軸の数値化け修正 <<<
//++ 20181022 グラフ縦軸の数値化け修正 >>>
/*============================================================================*/
/*! CSatelliteData

-# 数値にカンマを追加

@param	pos：カンマを付けたい数値
@param	pointKeta：少数点以下の桁数 (少数点0～6)
@param	singFlag：符号フラグ（0以外で値が正のとき'+'を付加
@retval	カンマを付けた後の数値

*/
/*============================================================================*/
CString CGraphWindow::addCanma(double pos, int pointKeta, int singFlag /*= 0*/)
{
	int	i, j, keta, temp;
	int sign = 0;
	int pointKetaTmp;
	int shiftMax = 6;
	double tempPos = pos;
	char str[MAXDIGIT];

	if (pos >= 10000000000000.0 || pos <= -10000000000000.0)
	{
		return CString(_T("#,###,###,###,###.###"));
	}
	
	if (isnan(pos)){
		// NaN (非数)の場合は０にする
		tempPos = 0.0;
	}

	if (tempPos >= 1000000000000.0 || pos <= -1000000000000.0)
	{
		shiftMax = 3;
	}
	else if (tempPos >= 100000000000.0 || pos <= -100000000000.0)
	{
		shiftMax = 4;
	}
	else if (tempPos >= 10000000000.0 || pos <= -10000000000.0)
	{
		shiftMax = 5;
	}

	if (pointKeta > shiftMax)
	{
		pointKetaTmp = shiftMax;
	}
	else
	{
		pointKetaTmp = pointKeta;
	}

	// 0に近い場合はまるめる
	switch (pointKetaTmp)
	{
	case 0:
		if (tempPos > -0.1 && tempPos < 0.1)
		{
			tempPos = 0.0;
		}
		break;
	case 1:
		if (tempPos > -0.01 && tempPos < 0.01)
		{
			tempPos = 0.0;
		}
		break;
	case 2:
		if (tempPos > -0.001 && tempPos < 0.001)
		{
			tempPos = 0.0;
		}
		break;
	case 3:
		if (tempPos > -0.0001 && tempPos < 0.0001)
		{
			tempPos = 0.0;
		}
		break;
	case 4:
		if (tempPos > -0.00001 && tempPos < 0.00001)
		{
			tempPos = 0.0;
		}
		break;
	case 5:
		if (tempPos > -0.000001 && tempPos < 0.000001)
		{
			tempPos = 0.0;
		}
		break;
	case 6:
		if (tempPos > -0.0000001 && tempPos < 0.0000001)
		{
			tempPos = 0.0;
		}
		break;
	case 7:
		if (tempPos > -0.00000001 && tempPos < 0.00000001)
		{
			tempPos = 0.0;
		}
		break;
	case 8:
		if (tempPos > -0.000000001 && tempPos < 0.000000001)
		{
			tempPos = 0.0;
		}
		break;
	case 9:
		if (tempPos > -0.0000000001 && tempPos < 0.0000000001)
		{
			tempPos = 0.0;
		}
		break;
	default:
		break;
	}

	__int64 num = (__int64)tempPos;

	double dnum = fabs(tempPos - (double)num);
	CString strSyosuu;

	switch (pointKetaTmp)
	{
	case 0:
		strSyosuu.Format(_T(""));
		break;
	case 1:
		strSyosuu.Format(_T("%.1f"), dnum);
		break;
	case 2:
		strSyosuu.Format(_T("%.2f"), dnum);
		break;
	case 3:
		strSyosuu.Format(_T("%.3f"), dnum);
		break;
	case 4:
		strSyosuu.Format(_T("%.4f"), dnum);
		break;
	case 5:
		strSyosuu.Format(_T("%.5f"), dnum);
		break;
	case 6:
		strSyosuu.Format(_T("%.6f"), dnum);
		break;
	case 7:
		strSyosuu.Format(_T("%.7f"), dnum);
		break;
	case 8:
		strSyosuu.Format(_T("%.8f"), dnum);
		break;
	case 9:
		strSyosuu.Format(_T("%.9f"), dnum);
		break;
	default:
		strSyosuu.Format(_T(""));
		break;
	}

	__int64 kuriage = 0;
	if (strSyosuu.Left(1).Compare(_T("1")) == 0)
	{
		kuriage = 1;
	}

	strSyosuu.Delete(0);

	// 正負を調べる
	if (tempPos < 0)
	{
		sign = 1;
		num = -num;
	}

	// 繰上げを足す
	num = num + kuriage;

	// 整数部にカンマを付ける
	i = 0;
	keta = 0;

	do
	{
		str[i++] = num % 10 + '0';
		keta++;
		num /= 10;
		if (keta % 3 == 0 && num != 0)
		{
			str[i++] = ',';
		}
	} while (num != 0 && i < sizeof(str));

	if (i >= MAXDIGIT)
	{
		return CString(_T("#,###,###,###,###.######"));
	}
	if (sign == 1)
	{
		// 負のとき
		if (i + 1 >= MAXDIGIT)
		{
			return CString(_T("#,###,###,###,###.######"));
		}
		str[i++] = '-';
	}
	else if (pos > 0.0)
	{
		// 正のとき
		if (singFlag)
		{
			if (i + 1 >= MAXDIGIT)
			{
				return CString(_T("#,###,###,###,###.######"));
			}
			str[i++] = '+';
		}
	}

	str[i] = '\0';

	j = i - 1;
	for (i = 0; j > i; i++, j--)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
	}

	return CString(str) + strSyosuu;
}
//++ 20181022 グラフ縦軸の数値化け修正 <<<

/*============================================================================*/
/*! グラフ描画

-# 軸の数値を上から3桁目で切り上げ/切り下げする

@param	入力数値
@param	0:切り下げ / 0以外:切り上げ
@retval 結果数値


*/
/*============================================================================*/
double CGraphWindow::Ceil3Keta(double pos, int dir)
{
	double tempPos = pos * 1000000000;	// 少数点6桁と切り上げの3桁目分をずらす
	long long num = (long long)fabs(tempPos);
	int keta = 0;

	do
	{
		keta++;
		num /= 10;
	} while (num != 0);

	double sft = pow(10, keta - 3);
	tempPos /= sft;

	if (dir != 0)
	{
		tempPos = ceil(tempPos);
	}
	else
	{
		tempPos = floor(tempPos);
	}
	tempPos *= sft;
	tempPos /= 1000000000;

	return tempPos;
}

/*============================================================================*/
/*! グラフ描画

-# 軸の数値を上から範囲値/100の桁数で切り上げ/切り下げする

@param	入力数値
@param	0:切り下げ / 0以外:切り上げ
@param	範囲値
@retval 結果数値

*/
/*============================================================================*/
double CGraphWindow::CeilKeta(double pos, int dir, double range)
{
	double tempPos = pos;
	double tempRange = range;
	double tempShift = 1.0;

	// 範囲値の桁が整数1桁以上の場合
	if (range >= 1.0 || range <= -1.0)
	{
		__int64 nNum = (__int64)fabs(tempRange);
		while (nNum != 0)
		{
			tempShift *= 10.0;		// 最小単位
			nNum /= 10;				// 桁があるか
		}

		tempShift /= 100.0;	// 範囲値/100の桁数を調べる
		double sft = tempPos / tempShift;
		// 切り上げ、切り下げがあるか調べる
		if (dir != 0)
		{
			sft = ceil(sft);
		}
		else
		{
			sft = floor(sft);
		}

		tempPos = sft * tempShift;
	}
	// 範囲値の桁が小数の場合
	else
	{
		double dNum = fabs(tempRange / 100);
		while (dNum < 1)
		{
			tempShift *= 10.0;		// 最小単位(分母）
			dNum *= 10.0;			// 桁があるか
		}

		// 符号をはずして整数部と小数部に分ける
		double upper = fabs((double)((__int64)tempPos));
		double lower = fabs(tempPos) - upper;

		double sft = lower * tempShift;
		// 符号を付加し切り上げ、切り下げがあるか調べる
		if (pos < 0)
		{
			sft = -sft;
		}
		if (dir != 0)
		{
			sft = ceil(sft);
		}
		else
		{
			sft = floor(sft);
		}

		// 整数部と小数部を加え、符号を付加する
		tempPos = fabs(sft) / tempShift + upper;
		if (pos < 0)
		{
			tempPos = -tempPos;
		}
	}

	return tempPos;
}

/*============================================================================*/
/*! CDataCollection

-# 衛星データ設定を初期化する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CGraphWindow::InitSatelliteDataSetting()
{
	// トレンドの描画
	MutexLock();

	for (int i = 0; i < MAX_TDATA; ++i)
	{
		m_bIsDrawOn[i] = TRUE;				// 描画ON
	}

	MutexUnlock();
}

/*============================================================================*/
/*! CDataCollection

-# 最小値最大値を計算する

@param	衛星データ
@param	最小値最大値
@retval	なし

*/
/*============================================================================*/
void CGraphWindow::CalcMinMax(stSpaceData& sData, structMinMax& stMinMax)
{
	// 表示可能状態
	if( sData.status & SDS_VALID )
	{
		// 最大値、最小値更新
		if (sData.mmPair.max > stMinMax.stLine->nMaxValue)
		{
			stMinMax.stLine->nMaxValue = sData.mmPair.max;
			if (stMinMax.stLine->nVScaleKind == LScale &&
				stMinMax.stLine->nMaxValue > m_nTotalScaleLMax)
			{
				m_nTotalScaleLMax = stMinMax.stLine->nMaxValue;
				if (m_nStvScale.Auto == TRUE)
				{
					stMinMax.max = m_nTotalScaleLMax;
				}
			}
		}
		if (sData.mmPair.min < stMinMax.stLine->nMinValue)
		{
			stMinMax.stLine->nMinValue = sData.mmPair.min;
			if (stMinMax.stLine->nVScaleKind == LScale &&
				stMinMax.stLine->nMinValue < m_nTotalScaleLMin)
			{
				m_nTotalScaleLMin = stMinMax.stLine->nMinValue;
				if (m_nStvScale.Auto == TRUE)
				{
					stMinMax.min = m_nTotalScaleLMin;
				}
			}
		}

		// 縦軸自動時はマージンをつける
		if( (stMinMax.stLine->nVScaleKind == LScale) && (m_nStvScale.Auto == TRUE) )
		{
			double	tmp_max = stMinMax.max;
			double	tmp_min = stMinMax.min;
#ifndef _GRAPHDRAW_KAIZEN
			stMinMax.curMax = Ceil3Keta(stMinMax.max + (stMinMax.max - stMinMax.min) * (double)AUTO_V_TOPBOTMARGIN, 1);
			stMinMax.curMin = Ceil3Keta(stMinMax.min - (stMinMax.max - stMinMax.min) * (double)AUTO_V_TOPBOTMARGIN, 0);
#else
			double range;
			// 最大最小が初期値でない場合
			if (tmp_max != -DBL_MAX && tmp_min != DBL_MAX)
			{
				// 範囲値をセット
				range = fabs(tmp_max - tmp_min);
			}
			else
			{
				// デフォルトで範囲値100
				range = 100.0;
			}
			if (range == 0.0)
			{
				// 値が１値しかない場合の範囲値セット
				range = fabs(tmp_max) / 100.0;
			}
			if (range == 0.0)
			{
				// どうしても範囲値が0になる場合
				range = 100.0;
			}
			double rangeMargin = range * (double)AUTO_V_TOPBOTMARGIN;
			double rangeWidth = range + (double)AUTO_V_TOPBOTMARGIN * 2;
			stMinMax.curMax = CeilKeta(tmp_max + rangeMargin, 1, rangeWidth);
			stMinMax.curMin = CeilKeta(tmp_min - rangeMargin, 0, rangeWidth);
#endif
			if( stMinMax.curMax == stMinMax.curMin )
			{
				if( tmp_max == tmp_min )
				{
					stMinMax.curMax = tmp_max + fabs((DEF_SAME_RANGE) * (double)AUTO_V_TOPBOTMARGIN);
					stMinMax.curMin = tmp_min - fabs((DEF_SAME_RANGE) * (double)AUTO_V_TOPBOTMARGIN);
				}
				else
				{
					stMinMax.curMax = tmp_max + fabs((tmp_max - tmp_min) * (double)AUTO_V_TOPBOTMARGIN);
					stMinMax.curMin = tmp_min - fabs((tmp_max - tmp_min) * (double)AUTO_V_TOPBOTMARGIN);
				}
			}
		}
	}
}

/*============================================================================*/
/*! CDataCollection

-# 最大値／最小値を作成

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::CreateMinMaxValue(bool bClear/* = false*/)
{
	DRAWLINE::iterator itr;
	deque<CSpaceData>::iterator trenditr;

	UINT nID;

	unsigned __int64 size;

	bool	bRealTime = ((m_bIsRealTime == FALSE) && (theApp.IsAnalysisMigration(m_nWindowNo) == false)) ? false : true;

	MutexLock();

	if( bRealTime )
		CGraphMutex::Lock(eRealTimeData);
	else
		CGraphMutex::Lock_Analysis(m_nWindowNo);

	// Autoスケールをクリア([AUTOクリア]ボタンによって実行)
	if( bClear )
		AutoHScaleClear();

	// 監視データリストの取得
	deque<CSpaceData>*	spaceDataList = &theApp.GetDataCollection().GetSpaceData();
	deque<stSpaceData>	*sData;
	vector<stSpaceData>	*analysis_data;

	// プロパティの設定データ数分をループ
	for (itr = m_DrawLines.begin(); itr != m_DrawLines.end(); itr++)
	{
		// 監視名が無い場合は表示しない
		if ((*itr).second.strName.IsEmpty() == TRUE)
		{
			continue;
		}

		// データインデックス(0～9)とライン属性情報を取得
		nID = (*itr).first;
		structLine data = (*itr).second;

		// 非表示設定ならば表示しない
		if (data.bIsVisible == FALSE)
		{
			continue;
		}

		sData = NULL;
		analysis_data = NULL;
		size = 0;

		if( bRealTime )
		{
			int nListIdx = (int)((*itr).second.nListIdx);

			// リストインデックスに値がある場合
			if (nListIdx >= 0)
			{
				// インデックス位置のデータを使う
				if( nListIdx < (*spaceDataList).size() )
					sData = &(*spaceDataList)[nListIdx].value;
			}
			else
			{
				int nSearchIdx;
				// 監視データIDが一致する監視データを探す
				for (trenditr = spaceDataList->begin(), nSearchIdx = 0; trenditr != spaceDataList->end(); trenditr++, nSearchIdx++)
				{
					// 監視データIDの一致を調べる
					if ((*trenditr).dataid == (*itr).second.nDataId)
					{
						// 監視データID一致のデータを使う
						sData = &(*trenditr).value;
						(*itr).second.nListIdx = nSearchIdx;
						break;
					}
				}
			}
		}
		else
		{
			if( m_nViewID == eViewID_1 )
				analysis_data = &theApp.GetDataCollection().GetTrendAllData(m_nWindowNo, nID);
			else
				analysis_data = &theApp.GetDataCollection().GetTrendZoomData(m_nWindowNo, nID);
		}

		// 表示する監視データがリストから見つからない場合
		if( (bRealTime && (sData == NULL)) || ((!bRealTime) && (analysis_data == NULL)) )
		{
			continue;
		}

		// 表示する監視データのサイズを取得
		size = (__int64)(bRealTime ? sData->size() : analysis_data->size());
		if (size == 0)
		{
			continue;
		}

		double max, min, curMax, curMin;
		// 軸の最大最小
		// 左縦軸
		if ((*itr).second.nVScaleKind == LScale)
		{
			// 自動スケール時の最大最小
			if (m_nStvScale.Auto == TRUE)
			{
				max = m_nTotalScaleLMax;
				min = m_nTotalScaleLMin;
#ifndef _GRAPHDRAW_KAIZEN			
				curMax = Ceil3Keta(max + (max - min) * (double)AUTO_V_TOPBOTMARGIN, 1);
				curMin = Ceil3Keta(min - (max - min) * (double)AUTO_V_TOPBOTMARGIN, 0);
#else
				double range;
				// 最大最小が初期値でない場合
				if (max != -DBL_MAX && min != DBL_MAX)
				{
					// 範囲値をセット
					range = fabs(max - min);
				}
				else
				{
					// デフォルトで範囲値100
					range = 100.0;
				}
				if (range == 0.0)
				{
					// 値が１値しかない場合の範囲値セット
					range = fabs(max) / 100.0;
				}
				if (range == 0.0)
				{
					// どうしても範囲値が0になる場合
					range = 100.0;
				}
				double rangeMargin = range * (double)AUTO_V_TOPBOTMARGIN;
				double rangeWidth = range + (double)AUTO_V_TOPBOTMARGIN * 2;
				curMax = CeilKeta(max + rangeMargin, 1, rangeWidth);
				curMin = CeilKeta(min - rangeMargin, 0, rangeWidth);
#endif
				if( curMax == curMin )
				{
					if( max == min )
					{
						curMax = max + fabs((DEF_SAME_RANGE) * (double)AUTO_V_TOPBOTMARGIN);
						curMin = min - fabs((DEF_SAME_RANGE) * (double)AUTO_V_TOPBOTMARGIN);
					}
					else
					{
						curMax = max + fabs((max - min) * (double)AUTO_V_TOPBOTMARGIN);
						curMin = min - fabs((max - min) * (double)AUTO_V_TOPBOTMARGIN);
					}
				}
			}
			// 固定スケール時の最大最小
			else
			{
				max = m_nStvScale.VScalLMax;
				min = m_nStvScale.VScalLMin;
				curMax = max;
				curMin = min;
			}
		}
		// 右縦軸
		else
		{
			// 固定スケール時の最大最小
			max = m_nStvScale.VScalRMax;
			min = m_nStvScale.VScalRMin;
			curMax = max;
			curMin = min;
		}

		// 選択スケールがAUTOなら、表示するスケールを検出
		UINT	nCurScale = m_nTrandHScale;

		if( bRealTime )
		{
			if( m_nTrandHScale == autoscale )
			{
				for( int t = 0; t < NUM_REAL_SCALE_INFO_TABLE; t++ )
				{
					unsigned __int64	data_size = size;

					if( tblRealScaleInfo[t].cmp_rate > 1 )
						data_size = theApp.GetDataCollection().GetCompData((*itr).second.nDataId, t).size();

					if( (data_size > 0) && (tblRealScaleInfo[t].dsp_data > data_size) )
					{
						nCurScale = t;
						break;
					}
				}

				// 横軸領域を更新
				m_dbHzComp = tblRealScaleInfo[nCurScale].cmp_rate;
				m_dbHzRange = tblRealScaleInfo[nCurScale].dsp_data * m_dbHzComp;
			}
		}
		else
		{
			if( m_nViewID == eViewID_1 )
				nCurScale = theApp.GetDataCollection().GetTrendAllTblNo(m_nWindowNo);
			else
				nCurScale = theApp.GetDataCollection().GetTrendZoomTblNo(m_nWindowNo);

			// 横軸領域を更新
//			m_dbHzComp = tblRealScaleInfo[nCurScale].cmp_rate;
//			m_dbHzRange = tblRealScaleInfo[nCurScale].dsp_data * m_dbHzComp;
		}

		// 選択スケールが圧縮スケールで圧縮データが存在しないならスルー
		deque<stSpaceData>	*cmp_data = NULL;
		if( bRealTime )
		{
			if( tblRealScaleInfo[nCurScale].cmp_rate > 1 )
			{
				cmp_data = &theApp.GetDataCollection().GetCompData((*itr).second.nDataId, nCurScale);

				if( cmp_data == NULL )
					continue;

				if( cmp_data->empty() )
					continue;
				else
					size = cmp_data->size();
			}
		}

		stDrawInfo&	draw_info = theApp.GetDataCollection().GetDrawInfo((*itr).second.nDataId, nCurScale);
//		unsigned __int64	pos = (!bRealTime) ? 0 : ((bClear || draw_info.change_flg) ? ((m_nTrandHScale == autoscale) ? 0 : draw_info.start_pos) : draw_info.update_pos);	// 100%⇒90%時にMin/Maxを再計算したい場合
		unsigned __int64	pos = (!bRealTime) ? 0 : (bClear ? ((m_nTrandHScale == autoscale) ? 0 : draw_info.start_pos) : draw_info.update_pos);

		m_stMinMax[(*itr).second.nVScaleKind].max = max;
		m_stMinMax[(*itr).second.nVScaleKind].min = min;
		m_stMinMax[(*itr).second.nVScaleKind].curMax = curMax;
		m_stMinMax[(*itr).second.nVScaleKind].curMin = curMin;
		m_stMinMax[(*itr).second.nVScaleKind].stLine = &data;

		// 指定データ範囲のMin／Maxを作成
		while (pos < size)
		{
			// Min／Maxを計算
			if( bRealTime )
			{
				if( tblRealScaleInfo[nCurScale].cmp_rate > 1 )
				{
					if( (0 <= pos) && (pos < (unsigned __int64)cmp_data->size()) )
//					{
//						if( (*cmp_data)[pos].mmPair.cnt >= tblRealScaleInfo[nCurScale].cmp_rate )
							CalcMinMax((*cmp_data)[pos], m_stMinMax[(*itr).second.nVScaleKind]);
//					}
				}
				else
				{
					if( (0 <= pos) && (pos < (unsigned __int64)sData->size()) )
						CalcMinMax((*sData)[pos], m_stMinMax[(*itr).second.nVScaleKind]);
				}
			}
			else
			{
				if( (0 <= pos) && (pos < (unsigned __int64)analysis_data->size()) )
					CalcMinMax((*analysis_data)[pos], m_stMinMax[(*itr).second.nVScaleKind]);
			}

			pos++;
		}
	}

	if( bRealTime )
		CGraphMutex::Unlock(eRealTimeData);
	else
		CGraphMutex::Unlock_Analysis(m_nWindowNo);

	MutexUnlock();
}

/*============================================================================*/
/*! CDataCollection

-# データの表示位置からコメント用のデータを特定

@param
@retval

*/
/*============================================================================*/
UINT CommentDataPosThread(LPVOID pParam)
{
	CGraphWindow	*pWnd = (CGraphWindow *)pParam;

//	TRACE(_T("=== CommentDataPosThread Start\n"));
//	CLogTraceEx::Write(_T(""), _T("CommentDataPosThread"), _T("Thread"), _T("Thread Start"), _T(""), nLogEx::debug);

	LONG	w = 5;
	bool	res = false;

	CGraphMutex::Lock(eDrawTrend);

	for( int i = 0; i < MAX_TDATA; i++ )
	{
		vector<POINT>::iterator	itr = pWnd->m_drawPos[i].begin();

		for( int j = 0; j < (int)pWnd->m_drawPosCount[i]; j++, itr++ )
		{
			if( pWnd->m_bCommentDataPosStop )
			{
				res = true;

				break;
			}

			// 無効データならスルー
			if( CPoint(*itr) == CPoint(-1, -1) )
				continue;

			// 表示位置の範囲内にマウスポインタがあるか判定
			if( (((*itr).x - w) <= pWnd->m_ptToolTipCtrl.x) && (pWnd->m_ptToolTipCtrl.x <= ((*itr).x + w)) &&
				(((*itr).y - w) <= pWnd->m_ptToolTipCtrl.y) && (pWnd->m_ptToolTipCtrl.y <= ((*itr).y + w)) )
			{
				CPropertiesData&	prop = theApp.GetPropertiesData(pWnd->GetWindowNo());

				if( prop.item[i].isComment )
				{
					CGraphMutex::Lock(eDisplayComment);

					pWnd->m_sToolTipCtrl = prop.item[i].comment;
					pWnd->PostMessage(eMessage_CommentChange);

					CGraphMutex::Unlock(eDisplayComment);

					res = true;

					CLogTraceEx::Write(_T(""), _T("CommentDataPosThread"), _T("Thread"), _T("Output Comment"), _T(""), nLogEx::debug);

					break;
				}
			}
		}

		if( res )
			break;
	}

	CGraphMutex::Unlock(eDrawTrend);

//	TRACE(_T("=== CommentDataPosThread End\n"));
//	CLogTraceEx::Write(_T(""), _T("CommentDataPosThread"), _T("Thread"), _T("Thread End"), _T(""), nLogEx::debug);

	return 0;
}
void CGraphWindow::RemoveCommentDataPosThread()
{
	if( m_pCommentDataPosThread != NULL )
	{
		m_bCommentDataPosStop = true;

		while( 1 )
		{
			if( WaitForSingleObject(m_pCommentDataPosThread->m_hThread, 100) != WAIT_TIMEOUT )
				break;
		}

		m_pCommentDataPosThread = NULL;
		m_bCommentDataPosStop = false;
	}
}
void CGraphWindow::CreateCommentDataPosThread(CPoint point)
{
	RemoveCommentDataPosThread();

	m_sToolTipCtrl.Empty();
	m_ptToolTipCtrl = point;

	m_bCommentDataPosStop = false;
	m_pCommentDataPosThread = AfxBeginThread(CommentDataPosThread, this);
}
/*============================================================================*/
/*! CDataCollection

-# データの表示位置から値差用のデータを特定

@param
@retval

*/
/*============================================================================*/
UINT DifferDataPosThread(LPVOID pParam)
{
	CGraphWindow	*pWnd = (CGraphWindow *)pParam;

//	TRACE(_T("=== DifferDataPosThread Start\n"));
//	CLogTraceEx::Write(_T(""), _T("DifferDataPosThread"), _T("Thread"), _T("Thread Start"), _T(""), nLogEx::debug);

	UINT	mode = DifferNone;

	if( pWnd->m_nSelDifferNo != 0 )
	{
		if( abs(pWnd->m_ptSelMouse.x - pWnd->m_ptDifferData[0].x) >= abs(pWnd->m_ptSelMouse.y - pWnd->m_ptDifferData[0].y) )
			mode = DifferSameData;
		else
			mode = DifferSameTime;
	}

	LONG	distance = LONG_MAX;

	UINT	tmp_nDifferDataNo = 0;
	UINT	tmp_nDifferDataPos = 0;
	CPoint	tmp_ptDifferData = CPoint(0, 0);

	bool	res = false, get = false;

	CGraphMutex::Lock(eDrawTrend);

	// XY方向に最も近いデータを検出
	if( mode == DifferNone )
	{
		for( int i = 0; i < MAX_TDATA; i++ )
		{
			vector<POINT>::iterator	itr = pWnd->m_drawPos[i].begin();

			for( int j = 0; j < (int)pWnd->m_drawPosCount[i]; j++, itr++ )
			{
				if( pWnd->m_bDifferDataPosStop )
				{
					res = true;
					get = false;

					break;
				}

				// 無効データならスルー
				if( (CPoint(*itr) == CPoint(-1, -1)) )
					continue;

				LONG	tmp_distance = abs((*itr).x - pWnd->m_ptSelMouse.x) + abs((*itr).y - pWnd->m_ptSelMouse.y);
				if( tmp_distance < distance )
				{
					tmp_nDifferDataNo = i;
					tmp_nDifferDataPos = j;
					tmp_ptDifferData = (*itr);

					distance = tmp_distance;

					get = true;

//					CLogTraceEx::Write(_T(""), _T("DifferDataPosThread"), _T("Thread"), _T("DifferNone Get"), _T(""), nLogEx::debug);
				}
			}

			if( res )
				break;
		}
	}
	// X方向に最も近いデータを検出(＝同一データ検出)
	else if( mode == DifferSameData )
	{
		vector<POINT>::iterator	itr = pWnd->m_drawPos[pWnd->m_nDifferDataNo[0]].begin();

		for( int j = 0; j < (int)pWnd->m_drawPosCount[pWnd->m_nDifferDataNo[0]]; j++, itr++ )
		{
			if( pWnd->m_bDifferDataPosStop )
			{
				res = true;
				get = false;

				break;
			}

			// 無効データならスルー
			if( (CPoint(*itr) == CPoint(-1, -1)) )
				continue;

			LONG	tmp_distance = abs((*itr).x - pWnd->m_ptSelMouse.x);
			if( tmp_distance < distance )
			{
				tmp_nDifferDataNo = pWnd->m_nDifferDataNo[0];
				tmp_nDifferDataPos = j;
				tmp_ptDifferData = (*itr);

				distance = tmp_distance;

				get = true;

//				CLogTraceEx::Write(_T(""), _T("DifferDataPosThread"), _T("Thread"), _T("DifferSameData Get"), _T(""), nLogEx::debug);
			}
		}
	}
	// Y方向に最も近いデータを検出(＝同一時刻検出)
	else if( mode == DifferSameTime )
	{
		for( int i = 0; i < MAX_TDATA; i++ )
		{
			if( pWnd->m_bDifferDataPosStop )
			{
				res = true;
				get = false;

				break;
			}

			if( pWnd->m_nDifferDataPos[0] >= pWnd->m_drawPosCount[i] )
				continue;

			// 無効データならスルー
			if( (CPoint(pWnd->m_drawPos[i][pWnd->m_nDifferDataPos[0]]) == CPoint(-1, -1)) )
				continue;

			LONG	tmp_distance = abs(pWnd->m_drawPos[i][pWnd->m_nDifferDataPos[0]].y - pWnd->m_ptSelMouse.y);
			if( tmp_distance < distance )
			{
				tmp_nDifferDataNo = i;
				tmp_nDifferDataPos = pWnd->m_nDifferDataPos[0];
				tmp_ptDifferData = pWnd->m_drawPos[i][pWnd->m_nDifferDataPos[0]];

				distance = tmp_distance;

				get = true;

//				CLogTraceEx::Write(_T(""), _T("DifferDataPosThread"), _T("Thread"), _T("DifferSameTime Get"), _T(""), nLogEx::debug);
			}
		}
	}

	// データを確定
	if( get )
	{
		pWnd->m_nDifferMode = mode;
		pWnd->m_nDifferDataNo[pWnd->m_nSelDifferNo] = tmp_nDifferDataNo;
		pWnd->m_nDifferDataPos[pWnd->m_nSelDifferNo] = tmp_nDifferDataPos;
		pWnd->m_ptDifferData[pWnd->m_nSelDifferNo] = tmp_ptDifferData;

		// 終点の表示位置を更新
		if( pWnd->m_nSelDifferNo == 1 )
		{
			pWnd->PostMessage(eMessage_DifferChange);

			CLogTraceEx::Write(_T(""), _T("DifferDataPosThread"), _T("Thread"), _T("Output Differ"), _T(""), nLogEx::debug);
		}
	}

	CGraphMutex::Unlock(eDrawTrend);

	pWnd->m_bDifferDataPosStop = true;

//	TRACE(_T("=== DifferDataPosThread End\n"));
//	CLogTraceEx::Write(_T(""), _T("DifferDataPosThread"), _T("Thread"), _T("Thread End"), _T(""), nLogEx::debug);

	return 0;
}
void CGraphWindow::RemoveDifferDataPosThread()
{
	if( m_pDifferDataPosThread != NULL )
	{
		m_bDifferDataPosStop = true;

		while( 1 )
		{
			if( WaitForSingleObject(m_pDifferDataPosThread->m_hThread, 100) != WAIT_TIMEOUT )
				break;
		}

		m_pDifferDataPosThread = NULL;
		m_bDifferDataPosStop = false;
	}
}
void CGraphWindow::CreateDifferDataPosThread(CPoint point)
{
	RemoveDifferDataPosThread();

	m_ptSelMouse = point;

	m_bDifferDataPosStop = false;
	m_pDifferDataPosThread = AfxBeginThread(DifferDataPosThread, this);
}

LRESULT CGraphWindow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
	case eMessage_CommentChange:
		CGraphMutex::Lock(eDisplayComment);

		m_pToolTipCtrl->UpdateTipText(m_sToolTipCtrl, this);

		CGraphMutex::Unlock(eDisplayComment);

	case eMessage_DifferChange:
		m_resDrawObject = 0;

		Select(objdiff);
		CDrawTool	*pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
		if( pTool != NULL )
			pTool->OnMouseMove(this, 0, m_ptDifferData[m_nSelDifferNo]);

		break;
	}

	return CWnd::WindowProc(message, wParam, lParam);
}
