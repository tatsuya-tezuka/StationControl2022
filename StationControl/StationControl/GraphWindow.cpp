/*============================================================================*/
/*! CGraphWindow.cpp

-# 衛星捕捉画面
*/
/*============================================================================*/
#include "stdafx.h"
#include "StationControl.h"
#include "GraphWindow.h"
#include "TrendPropertiesDlg.h"
#include "TrendGraphView.h"
#include "IntgTimeDlg.h"

#include <math.h>
#include <float.h>

#define ID_SETTINGBTN		(WM_USER+8100)
#define ID_HGRIDONCHKBTN	(WM_USER+8101)
#define ID_SIGONCHKBTN		(WM_USER+8102)
#define ID_INTGBTN			(ID_SIGONCHKBTN + MAX_TDATA)

#define _GRAPHDRAW_KAIZEN 1

UINT CGraphWindow::mHScale[] = {
	60,
	300,
	600,
	1200,
	1800,
	3600,
	7200,
	14400,
	28800,
	43200,
	57600,
	72000,
	86400,
	60,
	60,
};

/*============================================================================*/
/*! CGraphWindow

-# コンストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CGraphWindow::CGraphWindow()
: m_pointTop(0)
, m_dbMaxHzVal(1000)
, m_dbMinHzVal(0)
, m_dbHzRange(100)
, m_dbTmpHzRange(m_dbHzRange)
, m_nHorzDiv(mHDivNormal)
, m_DrawLines()
, m_ColorBk(RGB(236, 233, 216))
, m_ColorGrid(RGB(0, 0, 0))
, m_ColorText(RGB(0, 0, 0))
, m_ColorLighttGrid(RGB(128, 128, 128))
, m_nGraphType(eGraphType_Trend)
, m_nGrid(eGridType_NONE)
, m_nTrendInc(1)
, m_CursorPos(0)
, m_nTrandHScale(0)
, m_dTotalScaleLMax(-DBL_MAX)
, m_dTotalScaleLMin(DBL_MAX)
, m_dGridScaleLMax(0.0)
, m_dGridScaleLMin(0.0)
, m_nTrendDataPos(0)
, m_bViewDetail(TRUE)
, m_bFitYohochi(FALSE)
, m_bExitData(FALSE)
, m_bStopDraw(FALSE)
, m_StartTime(0)
, m_EndTime(0)
{
	// 初期化
	m_pParentWnd = NULL;
	m_nViewNo = 0;

	CFont defFont;
	CFont* oldFont = NULL;
	defFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (defFont.GetLogFont(&lf)){
		lf.lfWeight = FW_NORMAL;
		lf.lfHeight = 12;
		m_txtFontNormal.CreateFontIndirect(&lf);

//		lf.lfHeight = -10;
		m_txtFontSmall.CreateFontIndirect(&lf);
	}

	ClearCursor();
}

/*============================================================================*/
/*! CGraphWindow

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CGraphWindow::~CGraphWindow()
{
	m_txtFontSmall.DeleteObject();
	m_txtFontNormal.DeleteObject();
	
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
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(ID_SETTINGBTN, &CGraphWindow::OnBnClickedSettingBtn)
	ON_BN_CLICKED(ID_HGRIDONCHKBTN, &CGraphWindow::OnBnClickedHGridOnBtn)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 0, &CGraphWindow::OnBnClickedSigOnBtn0)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 1, &CGraphWindow::OnBnClickedSigOnBtn1)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 2, &CGraphWindow::OnBnClickedSigOnBtn2)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 3, &CGraphWindow::OnBnClickedSigOnBtn3)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 4, &CGraphWindow::OnBnClickedSigOnBtn4)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 5, &CGraphWindow::OnBnClickedSigOnBtn5)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 6, &CGraphWindow::OnBnClickedSigOnBtn6)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 7, &CGraphWindow::OnBnClickedSigOnBtn7)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 8, &CGraphWindow::OnBnClickedSigOnBtn8)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 9, &CGraphWindow::OnBnClickedSigOnBtn9)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 10, &CGraphWindow::OnBnClickedSigOnBtn10)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 11, &CGraphWindow::OnBnClickedSigOnBtn11)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 12, &CGraphWindow::OnBnClickedSigOnBtn12)
	ON_BN_CLICKED(ID_INTGBTN, &CGraphWindow::OnBnClickedIntgTimeBtn)
END_MESSAGE_MAP()


/*============================================================================*/
/*! グラフ描画

-# グラフウィンドウの作成

@param	dwStyle		ウィンドウスタイル
@param	rect		ウィンドウデフォルト領域
@param	pParentWnd	親ウィンドウ
@param	nID			ウィンドウ識別子
@param	bVisibleSettingBtn	設定ボタン表示
@retval

*/
/*============================================================================*/
BOOL CGraphWindow::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, BOOL bVisibleSettingBtn /*= FALSE*/)
{
	BOOL result = FALSE;
	m_pParentWnd = pParentWnd;

	dwStyle |= WS_CHILD;
	if (m_nGraphType == eGraphType_OffTrend){
		dwStyle |= WS_HSCROLL;
	}

	// ウィンドウの作成
	result = CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID, NULL);
	// 初期化
	m_nGrid = eGridType_NONE;	// グリッド有無
	m_bLabel = FALSE;			// ラベル表示
	m_dbMaxHzVal = m_dbHzRange;
	if (bVisibleSettingBtn || theApp.GetSelectStationMode() == eStation_1)
		m_nHorzDiv = mHDivNormal;			// 分割数
	else
		m_nHorzDiv = mHDivSmall;			// 分割数

	m_bUseVAxis[LScale] = TRUE;			// 端軸使用
	m_bUseVAxis[RScale] = TRUE;			// 端軸使用
	m_nStvScale.Auto = TRUE;			// 端軸の自動スケール
	m_nStvScale.VScalLMax = 50.0;		// 左端軸の最大値
	m_nStvScale.VScalLMin = -50.0;		// 左端軸の最小値
	m_nStvScale.VScalRMax = 50.0;		// 右端軸の最大値
	m_nStvScale.VScalRMin = -50.0;		// 右端軸の最小値
	m_nStvScale.VSCalItvl = 5;			// 端軸分割数

	if (m_nGraphType == eGraphType_OffTrend)
	{
		UpdateScrollInfo();
	}

	m_nTrendDataPos = 0;				// トレンドの開始時刻からの時間位置

//	SetHzRange(mHScale[m1]);			// 横時刻ビュー１デフォルト
	SetHzRange(mHScale[m1] + 1);			// 横時刻ビュー１デフォルト

	m_nTrandHScale = 0;					// 縦軸スケールの種類
	m_dTotalScaleLMax = -DBL_MAX;		// 左縦軸スケール上限
	m_dTotalScaleLMin = DBL_MAX;		// 左縦軸スケール下限

	InitSatelliteDataSetting();

	// 横罫線表示ON
	m_bHGridOn = TRUE;

	// 設定ボタン表示の場合は、通常の衛星捕捉グラフ
	if (bVisibleSettingBtn)
	{
		// 設定ボタン表示
		CRect rcBtn = rect;
		rcBtn.right -= mBtnSettingMarginRight;
		rcBtn.left = rcBtn.right - mBtnWidth;
		rcBtn.top += mBtnSettingMarginTop;
		rcBtn.bottom = rcBtn.top + mBtnHeight;

		CString strBtn;
		if (strBtn.LoadStringW(IDS_TREND_SETTINGBTN) == FALSE)
			return FALSE;
		m_btnSetting.Create(strBtn, BS_PUSHBUTTON | WS_VISIBLE, rcBtn, this, ID_SETTINGBTN);
		m_btnSetting.SetFont(&m_txtFontNormal);

		// 横罫線表示ON/OFFチェックボックス表示
		rcBtn.left = rcBtn.right - mGridMarginRight + mChkBtnMarginRight;
		rcBtn.right = rcBtn.left + mChkBtnWidth;
		rcBtn.top += mBtnSettingMarginTop;
		rcBtn.bottom = rcBtn.top + mChkBtnHeight;
		strBtn;
		if (strBtn.LoadStringW(IDS_TREND_HGRID_ON) == FALSE)
			return FALSE;
		m_btnHGridChk.Create(strBtn, BS_CHECKBOX | BS_AUTOCHECKBOX | WS_VISIBLE, rcBtn, this, ID_HGRIDONCHKBTN);
		m_btnHGridChk.SetFont(&m_txtFontNormal);
		m_btnHGridChk.SetCheck(1);

		CRect rectGraphArea;
		rectGraphArea.SetRect(
			rect.left,
			rect.top + mGridMarginTop,
			rect.right,
			rect.bottom - mGridMarginBottom + 1);

		// 信号表示チェックボタン表示
		for (int i = 0; i < MAX_TDATA; i++)
		{
			CRect rcSigBtn = rectGraphArea;
			CString strSigBtn;
			CPoint ptSigName;
			ptSigName.x = rect.right - mGridMarginRight + mRightMargin + mLegendWeidth + mLegendLineStrDistance;
			ptSigName.y = rect.top + mTopMargin + mSigStrHeight * i;
			rcSigBtn.left = ptSigName.x;
			rcSigBtn.right = rcSigBtn.left + mChkBtnWidth - mHStrDistance;
			rcSigBtn.top = ptSigName.y;
			rcSigBtn.bottom = rcSigBtn.top + 12;

			if (m_DrawLines.size() > i)
			{
				strSigBtn = m_DrawLines[i].strGName;			
			}

			if (m_StationIndex == eStation_Usuda642)
			{
				// 54m
				m_txtSigName[i].Create(strSigBtn, SS_LEFTNOWORDWRAP, rcSigBtn, this, ID_SIGONCHKBTN + i);
				m_txtSigName[i].SetFont(&m_txtFontNormal);
			}
			else
			{
				// 54m以外
				m_chkBtnViewSig[i].Create(strSigBtn, BS_CHECKBOX | BS_AUTOCHECKBOX, rcSigBtn, this, ID_SIGONCHKBTN + i);
				m_chkBtnViewSig[i].SetFont(&m_txtFontNormal);
				m_chkBtnViewSig[i].SetCheck(1);
			}
		}

	}
	// サマリグラフ
	else
	{
		// 背景色を黒色に変更
		m_ColorBk = RGB(0, 0, 0);
		m_ColorGrid = RGB(128, 128, 128);
		m_ColorText = RGB(128, 128, 128);
		m_ColorLighttGrid = RGB(128, 128, 128);
	}

//	m_nIntgTime = 300;
	m_nIntgTime = -1;
	m_bExitData = FALSE;
	m_StartTime = 0;
	m_EndTime = 0;

	UpdateIntegKanshi();

	m_rectCurValArea = CRect(0, 0, 0, 0);

	ResetCursor(-1);

	AjustLayout();

	return result;
}


/*============================================================================*/
/*! グラフ描画

-# 監視から積分時間の取得更新

@param	なし
@retval	なし

*/
/*============================================================================*/
void CGraphWindow::UpdateIntegKanshi()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT sta = theApp.GetSelectStation();

	int nIntgTime;

	CString strKanshi;
	UINT nTermNo = theApp.GetTermNo();
	if (nTermNo > 0)
		nTermNo = nTermNo - 1;

	int nAreaNo = theApp.GetClientTerminal();
//	strKanshi.Format(_T("%s%02u"), NOTIFY_INTEG_MSG, nTermNo);
	strKanshi.Format(_T("%s%s%02u"), NOTIFY_INTEG_MSG, mAreaNameString[nAreaNo], nTermNo);
	CString str;
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, strKanshi);
	if (obs_adr == NULL)
	{
		nIntgTime = 0;
		str.Format(_T(""));
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		nIntgTime = (int)obs_adr->d_data;
		str.Format(_T("%d"), nIntgTime);
	}
	else
	{
		nIntgTime = 0;
		str.Format(_T(""));
	}

	if (m_nIntgTime != nIntgTime && m_btnIntg.GetSafeHwnd() != NULL)
	{
		m_nIntgTime = nIntgTime;
		m_btnIntg.SetWindowTextW(str);
		m_btnIntg.UpdateData(FALSE);
	}
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

//#ifdef _DEBUG
//	{
//		CString str;
//		str.Format(_T("%s(%d) :: Draw Graph GraphType = %u, ViewID = %u, "), __FILEW__, __LINE__, m_nGraphType, m_nPropertiesID);
//		CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("OnPaint"), _T("debug"), str, nLogEx::debug);
//	}
//#endif

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
	//	memDC.SetBkColor(m_ColorBk);
	COLORREF colBk = GetSysColor(COLOR_3DFACE);
	memDC.SetBkColor(colBk);
	// デバイス コンテキストのビューポートの原点を設定
	memDC.SetViewportOrg(0, 0);

	memDC.SelectObject(&m_txtFontNormal);

	CBrush brush(colBk);
	// 全体を塗りつぶし
	memDC.FillRect(rectClient, &brush);

	// 目盛りを表示する領域が確保できるか調べる
	m_bViewDetail = IsViewDetail(rectClient);

	if (m_bViewDetail)
	{
		// 詳細表示時の軸領域
		rectGrid.SetRect(
			rectClient.left + mGridMarginLeft,
			rectClient.top + mGridMarginTop,
			rectClient.right - mGridMarginRight,
			rectClient.bottom - mGridMarginBottom);
		// 横グリッドを詳細表示
		m_nHorzDiv = mHDivNormal;
	}
	else
	{
		// 簡易表示時の軸領域
		rectGrid = rectClient;
		if (theApp.GetSelectStationMode() == eStation_5)
		{
			// 横グリッドを小さい領域用表示
			m_nHorzDiv = mHDivSmall;
		}
	}

	if (m_nGrid != eGridType_NONE)
	{
		if (m_nGraphType != eGraphType_Summary){
			DrawTrendName(memDC, rectClient);
		}
		if (DrawGrid(memDC, rectGrid) == TRUE)
		{
			// グラフ線の描画
			if (!m_DrawLines.empty()){
				DrawGraph(memDC);
			}
		}
	}

	dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0, 0, SRCCOPY);

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

	if (m_bViewDetail)
	{
		// 軸領域と同じにサイズ変更
		rectClient.SetRect(
			rectClient.left + mGridMarginLeft,
			rectClient.top + mGridMarginTop,
			rectClient.right - mGridMarginRight,
			rectClient.bottom - mGridMarginBottom);
	}

	// トレンド特殊処理（最大＝最小＝値）
	if (m_nGraphType == eGraphType_Trend || m_nGraphType == eGraphType_Summary){
		if (max == min && max == Y){
			if (m_bViewDetail)
				nX = (int)((rectClient.Width() * (X - m_pointTop)) / (m_dbHzRange - 1.0) + mGridMarginLeft);
			else
				nX = (int)((rectClient.Width() * (X - m_pointTop)) / (m_dbHzRange - 1.0));

			return CPoint(nX, rectClient.top + rectClient.Height() / 2);
		}
	}
	nY = (int)((rectClient.Height() * (Y - min)) / (max - min) + 0.5);
	if (m_bViewDetail)
		nX = (int)((rectClient.Width() * (X - m_pointTop)) / (m_dbHzRange - 1.0) + mGridMarginLeft);
	else
		nX = (int)((rectClient.Width() * (X - m_pointTop)) / (m_dbHzRange - 1.0));

	if (m_bViewDetail)
		return CPoint(nX, rectClient.Height() - nY + mGridMarginTop);
	else
		return CPoint(nX, rectClient.Height() - nY);
}
CPoint CGraphWindow::ConvPoint(double X)
{
	int nX;
	CRect rectClient;
	GetClientRect(rectClient);

	if (m_bViewDetail)
	{
		// 軸領域と同じにサイズ変更
		rectClient.SetRect(
			rectClient.left + mGridMarginLeft,
			rectClient.top + mGridMarginTop,
			rectClient.right - mGridMarginRight,
			rectClient.bottom - mGridMarginBottom);
	}

	switch (m_nGraphType)
	{
	case eGraphType_All:
	case eGraphType_OffAll:
		if (m_bViewDetail)
		{
			nX = (int)((rectClient.Width() * (X - m_pointTop)) / m_dbTmpHzRange + mGridMarginLeft);
		}
		else
		{
			nX = (int)((rectClient.Width() * (X - m_pointTop)) / m_dbTmpHzRange);
		}
		break;
	case eGraphType_Trend:
	case eGraphType_OffTrend:
	case eGraphType_Summary:
	default:
		if (m_bViewDetail)
		{
			nX = (int)((rectClient.Width() * (X - m_pointTop)) / m_dbTmpHzRange + mGridMarginLeft);
		}
		else
		{
			nX = (int)((rectClient.Width() * (X - m_pointTop)) / m_dbTmpHzRange);
		}
		break;
	}
	return CPoint(nX, rectClient.Height());
}


/*============================================================================*/
/*! グラフ描画

-# トレンドグラフ名の描画

@param	memDC		ドキュメント
@param	RectClient	描画領域
@retval	TRUE:トレンドグラフ名を描画した / FALSE:トレンドグラフ名を描画しなかった

*/
/*============================================================================*/
BOOL CGraphWindow::DrawTrendName(CDC& memDC, CRect& RectClient)
{
	// トレンドグラフビュー名の表示
	switch (m_nPropertiesID)
	{
	case eTrendWndType_AGC:
		if (m_strViewName.LoadString(IDS_TREND_AGCGRAPH) == FALSE)
			return FALSE;
		break;
	case eTrendWndType_RNG:
		if (m_strViewName.LoadString(IDS_TREND_RNGGRAPH) == FALSE)
			return FALSE;
		break;
	case eTrendWndType_RR:
		if (m_strViewName.LoadString(IDS_TREND_RRGRAPH) == FALSE)
			return FALSE;
		break;
	case eTrendWndType_EL:
		if (m_strViewName.LoadString(IDS_TREND_ELGRAPH) == FALSE)
			return FALSE;
		break;
	default:
		break;
	}

	memDC.SetTextColor(m_ColorGrid);
	memDC.SetBkMode(TRANSPARENT);
	CSize szStr;
	szStr = memDC.GetTextExtent(m_strViewName);
	memDC.TextOutW(RectClient.left + RectClient.Width() / 2 - szStr.cx / 2, RectClient.top + mViewWindowIDMarginTop, m_strViewName);

	if (m_nPropertiesID > eTrendWndType_AGC)
	{
		// ウィンドウ仕切線の描画
		CPen penSolid;
		penSolid.CreatePen(PS_SOLID, 1, m_ColorGrid);
		CPen* oldPen = memDC.SelectObject(&penSolid);

		memDC.MoveTo(RectClient.left, RectClient.top);
		memDC.LineTo(RectClient.right, RectClient.top);

		memDC.SelectObject(oldPen);
		penSolid.DeleteObject();
	}

	return TRUE;
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
	CFont* oldFont = memDC.SelectObject(&m_txtFontSmall);

	// グラフ軸内を塗りつぶし
	memDC.SetBkColor(m_ColorBk);
	CBrush brush(m_ColorBk);
	memDC.FillRect(RectClient, &brush);
	brush.DeleteObject();
	
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

	memDC.SetTextColor(m_ColorGrid);
	memDC.SetBkMode(TRANSPARENT);

	int nDecimalKeta = 0;

// test >>>
//	if (m_nPropertiesID == eTrendWndType_RNG)
//	{
//		int c = 0;
//	}
// test <<<


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
			range = fabs(m_dTotalScaleLMax - m_dTotalScaleLMin);
			if (range == 0.0)
				range = fabs(m_dTotalScaleLMax);
			if (range == 0.0)
				range = 100.0;
			m_dGridScaleLMax = Ceil3Keta(m_dTotalScaleLMax + range * (double)AUTO_V_TOPBOTMARGIN, 1);
			m_dGridScaleLMin = Ceil3Keta(m_dTotalScaleLMin - range * (double)AUTO_V_TOPBOTMARGIN, 0);
#else
			// 最大最小が初期値でない場合
			if (m_dTotalScaleLMax != -DBL_MAX && m_dTotalScaleLMin != DBL_MAX)
			{
				// 範囲値をセット
				range = fabs(m_dTotalScaleLMax - m_dTotalScaleLMin);
			}
			else
			{
				// デフォルトで範囲値100
				range = 100.0;
			}
			if (range == 0.0)
			{
				// 値が１値しかない場合の範囲値セット
				range = fabs(m_dTotalScaleLMax) / 100.0;
			}
			if (range == 0.0)
			{
				// どうしても範囲値が0になる場合
				range = 100.0;
			}

			double rangeMargin = range * (double)AUTO_V_TOPBOTMARGIN;
			double rangeWidth = range + (double)AUTO_V_TOPBOTMARGIN * 2;
			m_dGridScaleLMax = CeilKeta(m_dTotalScaleLMax + rangeMargin, 1, rangeWidth);
			m_dGridScaleLMin = CeilKeta(m_dTotalScaleLMin - rangeMargin, 0, rangeWidth);
#endif /* _GRAPHDRAW_KAIZEN */
			if (m_dGridScaleLMax >= m_dGridScaleLMin)
			{
				dVLRange = m_dGridScaleLMax - m_dGridScaleLMin;
				dVLPos = m_dGridScaleLMax;
//++ 1ドットずれ処置 >>>
				dVLMin = m_dGridScaleLMin;
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
			m_dGridScaleLMax = m_nStvScale.VScalLMax;
			m_dGridScaleLMin = m_nStvScale.VScalLMin;
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
		CString strLastDigit = _T("");
//-- 1ドットずれ処置 >>>
//		double yPos;
//-- 1ドットずれ処置 <<<
//++ 1ドットずれ処置 >>>
		int yPos;
//++ 1ドットずれ処置 <<<
		for (UINT n = 0; n <= (UINT)m_nStvScale.VSCalItvl; n++, dVLPos -= dVLStep, dVRPos -= dVRStep)
		{
			if (n == 0 || n == m_nStvScale.VSCalItvl)
				memDC.SelectObject(&penSolid);
			else
				memDC.SelectObject(&penLine);

			// 横罫線の表示ON
			if (n == 0 || n == m_nStvScale.VSCalItvl || m_bHGridOn)
			{
//-- 1ドットずれ処置 >>>
//				yPos = (double)n * height / m_nStvScale.VSCalItvl;
//				memDC.MoveTo(RectClient.left, RectClient.top + (int)yPos);//double to int
//				memDC.LineTo(RectClient.right, RectClient.top + (int)yPos);//double to int
//-- 1ドットずれ処置 <<<

//++ 1ドットずれ処置 >>>
				// 分割数と左軸の設定値を使用してY値計算
				double Y = (double)(m_nStvScale.VSCalItvl - n) * dVLStep + dVLMin;
				// Y軸レンジが0の場合はY値がないため、縦幅のみから計算する。
				// それ以外の場合は、ConvPoint関数のY座標計算と同じ処理
				int nY = (dVLRange == 0.0) ? ((int)((double)(m_nStvScale.VSCalItvl - n) * height / m_nStvScale.VSCalItvl + 0.5)) : ((int)((height * (Y - dVLMin)) / dVLRange + 0.5));
				yPos = height - nY + RectClient.top;
				memDC.MoveTo(RectClient.left, yPos);
				memDC.LineTo(RectClient.right, yPos);
//++ 1ドットずれ処置 <<<
			}

			if (m_bViewDetail)
			{
				// Auto時、左縦軸上にAuto文字の表示
				if (m_nStvScale.Auto && n == 0)
				{
					long nVPos = (long)ceil(dVLPos);

					CString strDigit = _T("Auto");
					CSize sz = memDC.GetTextExtent(strDigit);

					// 左縦軸数値文字の間にAuto文字を書く
//-- 1ドットずれ処置 >>>
//					memDC.TextOutW(RectClient.left + 5, RectClient.top + (int)(yPos + 0.5) - sz.cy, strDigit);
//-- 1ドットずれ処置 <<<
//++ 1ドットずれ処置 >>>
					memDC.TextOutW(RectClient.left + 5, yPos - sz.cy, strDigit);
//++ 1ドットずれ処置 <<<
				}

				// 縦軸単位を表示
				if (n == 0)
				{
					// プロパティの設定データ数分をループ
					DRAWLINE::iterator itr;
					for (itr = m_DrawLines.begin(); itr != m_DrawLines.end(); itr++)
					{
						if ((*itr).second.strUnit.IsEmpty() == FALSE)
						{
							CString strDigit;
							strDigit.Format(_T("[%s]"), (LPCTSTR)(*itr).second.strUnit);
							CSize sz = memDC.GetTextExtent(strDigit);
							// 左縦軸の上に単位文字を書く
							memDC.TextOutW(RectClient.left - mUnitMarginLeft - sz.cx, RectClient.top - mUnitMarginTop, strDigit);

							// 縦軸の小数点以下の桁をセット
							nDecimalKeta = (*itr).second.nDecDidits;
							break;
						}
					}
				}

				// 横罫線の表示ON
				if (n == 0 || n == m_nStvScale.VSCalItvl || m_bHGridOn)
				{
					// 左縦軸の値表示
					if (dVLStep != 0.0)
					{
						CString strDigit;
						strDigit = addCanma(dVLPos, nDecimalKeta);

						if (strLastDigit.CompareNoCase(strDigit) != 0)
						{
							// 直前の目盛り数値文字列を一致しない場合に新しい目盛り数値表示
							CSize sz = memDC.GetTextExtent(strDigit);
	
//-- 1ドットずれ処置 >>>
//							nStrLPos1 = RectClient.top + (int)(yPos + 0.5) - sz.cy;
//-- 1ドットずれ処置 <<<
//++ 1ドットずれ処置 >>>
							nStrLPos1 = yPos - sz.cy;
//++ 1ドットずれ処置 <<<
							if (nStrLPos2 < 0 ||
								nStrLPos1 > nStrLPos2 + sz.cy)
							{
								nStrLPos2 = nStrLPos1;
								memDC.TextOutW(RectClient.left - sz.cx - mLabelLeftMargine, nStrLPos2, strDigit);
								strLastDigit = strDigit;
							}
						}
					}
				}
			}
		}
	}
	if (m_nHorzDiv > 0 && bRangeValid == TRUE)
	{
		// 縦線描画
		memDC.SelectObject(&penLine);

		// 全トレンド縦軸（縦軸の横ずれ防止）
		int nStrPos1 = -1;
		int nStrPos2 = -1;

		CTime		t = m_StartTime;
		CTime		prev_t;
		CString		strLastYmd;
		strLastYmd.Format(_T(""));
		for (double n1 = 0.0, n2 = 0.0;
			n1 < dEndN;
//			n1 += (m_dbHzRange / (double)m_nHorzDiv), n2 += (m_dbTmpHzRange / (double)m_nHorzDiv), prev_t = t, t += (time_t)(m_dbHzRange / (double)m_nHorzDiv))
			n1 += ((m_dbHzRange - 1)/ (double)m_nHorzDiv), n2 += (m_dbTmpHzRange / (double)m_nHorzDiv), prev_t = t, t += (time_t)((m_dbHzRange - 1)/ (double)m_nHorzDiv))
		{

			int nInipoint = ConvPoint((double)n2).x;
			memDC.MoveTo(nInipoint, RectClient.top);
			memDC.LineTo(nInipoint, RectClient.bottom);

			if (m_bViewDetail)
			{
				// 横軸の時刻表示
				CString strYmd;
				CString strHMS;
				CSize szYmd;
				CSize szHMS;
				// 左端の時刻もしくは、年月日が変わるとき、年月日を表示
				if (prev_t == t || prev_t.GetYear() != t.GetYear())
				{
					strYmd = t.Format("%Y-%m-%d");
				}
				else if (prev_t.GetMonth() != t.GetMonth() || prev_t.GetDay() != t.GetDay())
				{
					strYmd = t.Format("%m-%d");
				}
				else if (!strLastYmd.IsEmpty())
				{
					strYmd = strLastYmd;
					strLastYmd.Format(_T(""));
				}

				szYmd = memDC.GetTextExtent(strYmd);

				// 時分秒
				strHMS = t.Format("%H:%M:%S");
				szHMS = memDC.GetTextExtent(strHMS);

				// 大きい文字列のサイズを取得
				int xRange, xYmd;
				CSize szYmdDef = memDC.GetTextExtent(strYmd);

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

				nStrPos1 = nInipoint;
				if (nStrPos2 < 0 ||
					nStrPos1 > nStrPos2 + xRange + (long)mHStrDistance)
				{
					nStrPos2 = nStrPos1;
					memDC.TextOutW(nInipoint - xYmd, RectClient.bottom + 4, strYmd);
					memDC.TextOutW(nInipoint - szHMS.cx, RectClient.bottom + 4 + mTimeLabel2ndMargine, strHMS);
				}
				else if (!strYmd.IsEmpty())
				{
					// 年月日が表示できなかった場合は、次にまわす
					strLastYmd = strYmd;
				}
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
		// 暫定処置。オンラインの場合は描画しない
		//return;
	}

	switch (m_nGraphType){
	case	eGraphType_All:
	case	eGraphType_Trend:
	case	eGraphType_Summary:
		// トレンドグラフの描画
		DrawTrend(memDC);
		break;
	case	eGraphType_OffAll:
	case	eGraphType_OffTrend:
		// トレンドグラフの描画
		DrawOffTrend(memDC);
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
	CRect rect, rectGrid;
	GetClientRect(rect);

	if (m_bViewDetail)
	{
		rect.SetRect(
			rect.left,
			rect.top + mGridMarginTop,
			rect.right,
			rect.bottom - mGridMarginBottom + 1);

		rectGrid = rect;
		rectGrid.left += mGridMarginLeft,
			rectGrid.right -= mGridMarginRight;
	}
	else
	{
		rectGrid = rect;
	}

	// リージョン生成
	HRGN	hRgn = CreateRectRgn(rectGrid.left, rectGrid.top, rectGrid.right, rectGrid.bottom);
	SelectClipRgn(memDC, hRgn);

	CPoint pointDraw;
	CPen penLine, penCurValue;
	CPen* oldPen;
	CBrush brushDots, brushCurValue;
	CBrush* oldBrush;
	CFont* oldFont;
	CPoint pointLastSigName;

	penCurValue.CreatePen(PS_SOLID, 1, m_ColorGrid);
	brushCurValue.CreateSolidBrush(m_ColorLighttGrid);
	memDC.SetTextColor(m_ColorGrid);

	MutexLock();

	// トレンド描画
	for (int i = eSigSrcType_File; i >= eSigSrcType_EngExchange; i--)
	{
		// データ元がファイルの場合から先にトレンド描画する
		DrawTrendContents(memDC, i);
	}

	// リージョン解除
	SelectClipRgn(memDC, NULL);
	DeleteObject(hRgn);

	// 積分表示
	BOOL bViewInteg = FALSE;
	UINT nStationID = theApp.GetSelectStation();
	// サマリグラフでなく、内之浦24mでなく、かつAGCグラフの場合
	if (m_nGraphType != eGraphType_Summary && nStationID != eStation_Uchinoura20 && m_nPropertiesID == eTrendWndType_AGC)
	{
		bViewInteg = TRUE;
	}

	// 凡例描画
	if (m_bViewDetail)
	{
		// フォントの設定
		oldFont = memDC.SelectObject(&m_txtFontSmall);

		DRAWLINE::iterator itr;
		UINT nID;

		// プロパティの設定データ数分をループ
		BOOL bFirst = TRUE;
		for (itr = m_DrawLines.begin(); itr != m_DrawLines.end(); itr++)
		{
			// データ名が無い場合は表示しない
			if ((*itr).second.strName.IsEmpty() == TRUE)
			{
				continue;
			}

			nID = (*itr).first;
			structLine data = (*itr).second;

			// 表示設定
			// 線色の設定
			penLine.CreatePen((*itr).second.nStyle, 1, (*itr).second.crColor);
			oldPen = memDC.SelectObject(&penLine);

			// 円色の設定
			brushDots.CreateSolidBrush((*itr).second.crColor);
			oldBrush = memDC.SelectObject(&brushDots);
			int r;	// 円（点）の直径

			// データ名表示
			//				memDC.SetTextColor((*itr).second.crColor);
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
			CSize szStr;
			szStr = memDC.GetTextExtent(strName);

			CPoint ptSigName;
			ptSigName.x = rect.right - mGridMarginRight + mRightMargin;
			ptSigName.y = rect.top + mTopMargin + mSigStrHeight * nID;

			// 積分表示でかつ、臼田64m、内之浦34mそれぞれの積分信号位置の場合、信号の間を空ける
//			if (bViewInteg && (nStationID == eStation_Usuda64 && nID >= 1 || nStationID == eStation_Usuda642 && nID >= 1 || nStationID == eStation_Uchinoura34 && nID >= 11))
			if (bViewInteg && (nStationID == eStation_Usuda64 && nID >= 1 || nStationID == eStation_Uchinoura34 && nID >= 11))
			{
				ptSigName.y++;
			}

			pointLastSigName = ptSigName;

			switch ((*itr).second.nDrawKind)
			{
			case Line:
				// 凡例を線で描画
				memDC.MoveTo(ptSigName.x, ptSigName.y + szStr.cy / 2);
				ptSigName.x += mLegendWeidth;
				memDC.LineTo(ptSigName.x, ptSigName.y + szStr.cy / 2);
				break;

			case Dots:
				// 凡例を円で描画
				r = (*itr).second.nDotSize / 2;
				memDC.Ellipse(ptSigName.x + mLegendWeidth / 2 - r, ptSigName.y + szStr.cy / 2 - r, ptSigName.x + mLegendWeidth / 2 + r, ptSigName.y + szStr.cy / 2 + r);
				ptSigName.x += mLegendWeidth;
				break;

			case LineAndDots:
				// 凡例を線と円で描画
				r = (*itr).second.nDotSize / 2;
				memDC.Ellipse(ptSigName.x + mLegendWeidth / 2 - r, ptSigName.y + szStr.cy / 2 - r, ptSigName.x + mLegendWeidth / 2 + r, ptSigName.y + szStr.cy / 2 + r);
				memDC.MoveTo(ptSigName.x, ptSigName.y + szStr.cy / 2);
				ptSigName.x += mLegendWeidth;
				memDC.LineTo(ptSigName.x, ptSigName.y + szStr.cy / 2);
				break;

			default:
				break;
			}

			ptSigName.x += mLegendLineStrDistance;
//			memDC.TextOutW(ptSigName.x, ptSigName.y, strName);

			// 現在値描画
			CRect rectCurValue(rect.right - mHStrDistance - mCurValueWidth, ptSigName.y - mVStrDistance, rect.right - mHStrDistance, ptSigName.y + szStr.cy + mVStrDistance);
			memDC.SelectObject(&penCurValue);
			memDC.FillSolidRect(rectCurValue, COLOR_WHITE);
			memDC.FrameRect(rectCurValue, &brushCurValue);
			if (bFirst)
			{
				bFirst = FALSE;
				m_rectCurValArea = rectCurValue;
			}
			else
			{
				m_rectCurValArea.bottom = ptSigName.y + szStr.cy + mVStrDistance;
			}

			rectCurValue.left += mHStrDistance;
			rectCurValue.right -= mHStrDistance;
			memDC.DrawText(addCanma(m_curVal[nID], (*itr).second.nDecDidits), rectCurValue, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
			//				memDC.DrawText(addCanma(val, 0), rectCurValue, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

			memDC.SelectObject(oldPen);
			memDC.SelectObject(oldBrush);
			penLine.DeleteObject();
			brushDots.DeleteObject();
		}
		memDC.SelectObject(oldFont);
	}

	// 積分表示する場合
	if (bViewInteg && pointLastSigName.x != 0)
	{
		// 積分時間設定ボタンの説明表示
		// フォントの設定
		oldFont = memDC.SelectObject(&m_txtFontSmall);

		CRect rcIntgStr;
		rcIntgStr.left = pointLastSigName.x;
		rcIntgStr.right = rect.right;
		//		rcIntgStr.top = pointLastSigName.y + mSigStrHeight + mTopMargin;
		rcIntgStr.top = pointLastSigName.y + mSigStrHeight;
		rcIntgStr.bottom = rcIntgStr.top + mBtnHeight;
		CString strItegTime(_T("INTEG TIME"));
		memDC.DrawText(strItegTime, rcIntgStr, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

		CRect rectIntgFrame;
		rectIntgFrame = rcIntgStr;

		CSize sz = memDC.GetTextExtent(strItegTime);
		// 積分時間設定ボタン座標計算
		m_rectIntgBtn = rcIntgStr;
		m_rectIntgBtn.left += sz.cx + mBtnIntgMarginRight;
		m_rectIntgBtn.right = m_rectIntgBtn.left + mBtnWidth;
		if (m_btnIntg.GetSafeHwnd() == NULL)
		{
			// 積分時間設定ボタン表示
			CString strBtn;
			if (m_nIntgTime > 0)
				strBtn.Format(_T("%d"), m_nIntgTime);
			m_btnIntg.Create(strBtn, BS_PUSHBUTTON | WS_VISIBLE, m_rectIntgBtn, this, ID_INTGBTN);
			m_btnIntg.SetFont(&m_txtFontNormal);
		}

		rcIntgStr = m_rectIntgBtn;
		rcIntgStr.left = rcIntgStr.right + mHStrDistance * 2;
		rcIntgStr.right += mIntgStrMarginWidth;
		memDC.DrawText(_T("[s]"), rcIntgStr, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

		memDC.SelectObject(oldFont);

		if (m_StationIndex == eStation_Usuda64 || m_StationIndex == eStation_Uchinoura34)
		{
			// 64m, 34mは枠線描画
			rectIntgFrame.left -= mHStrDistance * 2;
			if (nStationID == eStation_Uchinoura34)
			{
				rectIntgFrame.top -= mIntegFrameTopOffsetUsc34;
			}
			else
			{
				rectIntgFrame.top -= mIntegFrameTopOffsetUdsc64;
			}
			rectIntgFrame.bottom += mIntegFrameBotOffset;
			memDC.SelectObject(&penCurValue);
			memDC.FrameRect(rectIntgFrame, &brushCurValue);
		}
	}

	penCurValue.DeleteObject();
	brushCurValue.DeleteObject();

	MutexUnlock();
}

/*============================================================================*/
/*! グラフ描画

-# トレンドグラフ線の描画

@param	memDC：デバイスコンテキスト
@param	nSrcType：データ元タイプ(0:工学値変換後データ / 1:ファイル読み込み）
@retval	なし

*/
/*============================================================================*/
void CGraphWindow::DrawTrendContents(CDC& memDC, int nSrcType)
{
	DRAWLINE::iterator itr;

	CPoint pointDraw;
	CPen penLine;
	CPen* oldPen;
	CBrush brushDots;
	CBrush* oldBrush;
	CFont* oldFont;
	CPoint pointLastSigName;

	double val = 0.0;
	UINT nID;
	pair<double, double> pnt;

	int mSaveLogData;
	int mLogData;
	__int64 size;

	m_pointTop = 0.0;

	// トレンドの描画
	CGraphMutex::Lock(eData);

	// 監視データリストの取得
	CSpaceData*	spaceDataList = theApp.GetDataCollection().GetSpaceData(m_StationIndex);
	CSpaceData*	sData;

	// プロパティの設定データ数分をループ
	for (itr = m_DrawLines.begin(); itr != m_DrawLines.end(); itr++)
	{
		// データ名が無い場合は表示しない
		if ((*itr).second.strName.IsEmpty() == TRUE)
		{
			continue;
		}

		// データインデックスがない場合は表示しない
		int nListIdx = (int)((*itr).second.nListIdx);
		if (nListIdx < 0)
		{
			continue;
		}

		nID = (*itr).first;
		structLine data = (*itr).second;
		// 非表示設定ならば表示しない
		if (data.bIsVisible == FALSE)
		{
			continue;
		}
		
		mSaveLogData = -1;
		mLogData = -1;

		sData = &spaceDataList[nListIdx];
		size = 0;

		// データが見つからない場合
		if (sData == NULL)
		{
			// 表示しない
			continue;
		}

		// データ元一致確認
		if (sData->srcType != nSrcType)
		{
			// 表示しない
			continue;
		}

		// データサイズの取得
		size = (__int64)sData->value.size();
		if (size == 0)
		{
			// 監視データがなければ表示しない
			continue;
		}

		// キューに無い場合は何もしない
		if (m_TrendDataPP[nID].spaceData.empty() == TRUE)
		{
			continue;
		}

// test >>>
//		if (m_nPropertiesID == eTrendWndType_EL)
//		{
//			int c = 0;
//		}
// test <<<


		// 描画ONの場合にグラフデータを表示する
		if (m_bIsDrawOn[nID] == TRUE)
		{
			__int64 pos = m_nTrendDataPos;
			size = (__int64)m_TrendDataPP[nID].spaceData.size();

			// キューサイズより現在値が大きい場合は描画しない
			if (pos >= size)
				continue;

			__int64 xpos = 0;
//			__int64 drawPos = -1;
			int saveX = -1;
			BOOL bIsLastMaxValue = FALSE;
			bool ppTrend = true;

			// 表示設定
			// 線色の設定
			penLine.CreatePen((*itr).second.nStyle, (*itr).second.nWidth, (*itr).second.crColor);
			oldPen = memDC.SelectObject(&penLine);

			// 円色の設定
			brushDots.CreateSolidBrush((*itr).second.crColor);
			oldBrush = memDC.SelectObject(&brushDots);
			int r;	// 円（点）の直径

			// フォントの設定
			oldFont = memDC.SelectObject(&m_txtFontSmall);

			if (size > 0)
			{
				// 開始時刻を探す
				time_t posTime = CTime(m_TrendDataPP[nID].spaceData[pos].systime).GetTime();

#if 0
				// 非圧縮の場合
				if (m_TrendDataPP[nID].spaceData[0].ppPair.count == 1)
				{
					// 開始時刻がキューの指定データ時刻より大きい場合
					if (m_StartTime > posTime)
					{
						// 開始時刻にオフセットを付加
						__int64 offset = m_StartTime - posTime;
						if (pos + offset < size)
						{
							pos += offset;
						}
						else
						{
							xpos -= offset;
						}

						// 開始前の有効データを見つけ、グラフ領域の端から端まで表示
						while (pos > 0)
						{
							if ((char)m_TrendDataPP[nID].spaceData[pos].status[0] >= 0)
							{
								break;
							}
							pos--;
							xpos--;
						}
					}
					// 開始時刻がキューの指定データ時刻よりも小さい場合
					else if (m_StartTime < posTime)
					{
						// 開始時刻にオフセットを付加
						__int64 offset = posTime - m_StartTime;
						if (pos >= offset)
						{
							pos -= offset;
						}
						else
						{
							xpos += offset;
						}
						// 開始前の有効データを見つけ、グラフ領域の端から端まで表示
						while (pos > 0)
						{
							if ((char)m_TrendDataPP[nID].spaceData[pos].status[0] >= 0)
							{
								break;
							}
							pos--;
							xpos--;
						}
					}
					else
					{
						// 開始前の有効データを見つけ、グラフ領域の端から端まで表示
						while (pos > 0)
						{
							if ((char)m_TrendDataPP[nID].spaceData[pos].status[0] >= 0)
							{
								break;
							}
							pos--;
							xpos--;
						}
					}
				}
				// 圧縮の場合
				else
#endif
				{
					time_t queStartTime = CTime(m_TrendDataPP[nID].spaceData[0].systime).GetTime();
					time_t queEndTime = CTime(m_TrendDataPP[nID].spaceData[size - 1].systime).GetTime();

					// 開始時刻がキューの指定データ時刻より大きい場合
					if (m_StartTime > posTime)
					{
						// 開始時刻にオフセットを付加
						__int64 offset = m_StartTime - posTime;
						// キュー範囲内に開始時刻がある場合
						if (m_StartTime <= queEndTime)
						{
							// 開始時刻posを昇順で探す
							for (__int64 i = pos; i < size; i++)
							{
								if ((char)m_TrendDataPP[nID].spaceData[i].status[0] >= 0)
								{
									posTime = CTime(m_TrendDataPP[nID].spaceData[i].systime).GetTime();
									if (posTime > m_StartTime)
									{
										// 開始時刻を越えたキュー位置を見つけた
										break;
									}
									else
									{
										pos = i;
									}
								}
							}
						}
						// キュー範囲内に開始時刻がない場合
						else
						{
							xpos -= offset;
						}

						// 開始前の有効データを見つけ、グラフ領域の端から端まで表示
						while (pos > 0)
						{
							if ((char)m_TrendDataPP[nID].spaceData[pos].status[0] >= 0)
							{
								break;
							}
							pos--;
							xpos--;
						}
					}
					// 開始時刻がキューの指定データ時刻よりも小さい場合
					else if (m_StartTime < posTime)
					{
						// 開始時刻にオフセットを付加
						__int64 offset = posTime - m_StartTime;
						// キュー範囲内に開始時刻がある場合
						if (queStartTime <= m_StartTime)
						{
							// 開始時刻posを昇順で探す
							for (__int64 i = pos; i < size; i++)
							{
								if ((char)m_TrendDataPP[nID].spaceData[i].status[0] >= 0)
								{
									posTime = CTime(m_TrendDataPP[nID].spaceData[i].systime).GetTime();
									if (posTime > m_StartTime)
									{
										// 開始時刻を越えたキュー位置を見つけた
										break;
									}
									else
									{
										pos = i;
									}
								}
							}
						}
						else
						{
							xpos += offset;
						}
						// 開始前の有効データを見つけ、グラフ領域の端から端まで表示
						while (pos > 0)
						{
							if ((char)m_TrendDataPP[nID].spaceData[pos].status[0] >= 0)
							{
								break;
							}
							pos--;
							xpos--;
						}
					}
					else
					{
						// 開始前の有効データを見つけ、グラフ領域の端から端まで表示
						while (pos > 0)
						{
							if ((char)m_TrendDataPP[nID].spaceData[pos].status[0] >= 0)
							{
								break;
							}
							pos--;
							xpos--;
						}
					}
				}
			}

			BOOL bFirst = TRUE;
			BOOL bSetCurVal = FALSE;

			// 直近の値をセットしておく
			val = m_curVal[nID];

			// 画面のx範囲のみ描画
			while (pos < size)
			{
				// 元の監視データから時刻の検索
				time_t cur_t = CTime(m_TrendDataPP[nID].spaceData[pos].systime).GetTime();
				if (m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange)
				{
					while (xpos < (__int64)m_dbHzRange && m_StartTime + xpos < cur_t)
					{
						// 同じ時刻がある対策。時刻が見つかるまでx位置をずらす
						xpos++;
					}
				}
				else
				{
					xpos = cur_t - m_StartTime;
				}

//				// ウィンドウ範囲内を描画したら抜ける
//				//				if (xpos >= (int)m_dbHzRange)
//				if (drawPos >= (__int64)m_dbHzRange)
//				{
//					if (m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange)
//						break;
//				}

				// 表示可能状態
				if ((char)m_TrendDataPP[nID].spaceData[pos].status[0] >= 0)
				{
//					drawPos = xpos;

					// 非圧縮表示
					if (m_TrendDataPP[nID].spaceData[0].ppPair.count == 1 && m_TrendDataPP[nID].spaceData[pos].ppPair.count == 1)
					{
						val = m_TrendDataPP[nID].spaceData[pos].ppPair.max;
						pnt.first = (double)xpos; pnt.second = val;
						pointDraw = ConvPoint(pnt, m_dGridScaleLMax, m_dGridScaleLMin);
						switch ((*itr).second.nDrawKind)
						{
						case Line:
							// 線の描画
							if ((*itr).second.nGraphKind == 1)
							{
								if (xpos == 0 || pos == 0 ||
									m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange && (char)m_TrendDataPP[nID].spaceData[pos - 1].status[0] < 0 ||
									m_TrendDataPP[nID].srcType == eSigSrcType_File && bFirst)
								{
									memDC.MoveTo(pointDraw);
									bFirst = FALSE;
								}
								else
								{
									CPoint pt = pointDraw;
									pt.y = mSaveLogData;
									memDC.LineTo(pt);
									memDC.LineTo(pointDraw);
								}
								mSaveLogData = pointDraw.y;
							}
							else
							{
								if (xpos == 0 || pos == 0 ||
									m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange && (char)m_TrendDataPP[nID].spaceData[pos - 1].status[0] < 0 ||
									m_TrendDataPP[nID].srcType == eSigSrcType_File && bFirst)
								{
									memDC.MoveTo(pointDraw);
									bFirst = FALSE;
								}
								else
								{
									memDC.LineTo(pointDraw);
								}
							}

							break;

						case Dots:
							// 円の描画
							r = (*itr).second.nDotSize / 2;
							memDC.Ellipse(pointDraw.x - r, pointDraw.y - r, pointDraw.x + r, pointDraw.y + r);
							break;

						case LineAndDots:
							// 線の描画
							if ((*itr).second.nGraphKind == 1)
							{
								if (xpos == 0 || pos == 0 ||
									m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange && (char)m_TrendDataPP[nID].spaceData[pos - 1].status[0] < 0 ||
									m_TrendDataPP[nID].srcType == eSigSrcType_File && bFirst)
								{
									memDC.MoveTo(pointDraw);
									bFirst = FALSE;
								}
								else
								{
									CPoint pt = pointDraw;
									pt.y = mSaveLogData;
									memDC.LineTo(pt);
									memDC.LineTo(pointDraw);
								}
								mSaveLogData = pointDraw.y;
							}
							else{
								if (xpos == 0 || pos == 0 ||
									m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange && (char)m_TrendDataPP[nID].spaceData[pos - 1].status[0] < 0 ||
									m_TrendDataPP[nID].srcType == eSigSrcType_File && bFirst)
								{
									memDC.MoveTo(pointDraw);
									bFirst = FALSE;
								}
								else
								{
									memDC.LineTo(pointDraw);
								}
							}

							// 円の描画
							r = (*itr).second.nDotSize / 2;
							memDC.Ellipse(pointDraw.x - r, pointDraw.y - r, pointDraw.x + r, pointDraw.y + r);
							break;

						default:
							// ここには来ない。
							break;
						}
					}
					// 圧縮表示
					else if (m_TrendDataPP[nID].spaceData[pos].ppPair.count > 1)
					{
						switch ((*itr).second.nDrawKind)
						{
						case Line:
							// 線の描画
							if (xpos == 0 || pos == 0 ||
								m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange && (char)m_TrendDataPP[nID].spaceData[pos - 1].status[0] < 0 ||
								m_TrendDataPP[nID].srcType == eSigSrcType_File && bFirst)
							{
								PPLine1st();
								bFirst = FALSE;
							}
							else
							{
								// 前回が上昇線
								if (bIsLastMaxValue == TRUE)
								{
									if (m_TrendDataPP[nID].spaceData[pos].ppPair.max >= m_TrendDataPP[nID].spaceData[pos - 1].ppPair.max)
									{
										// 上昇線のためminからmax
										PPLineMinToMax();
									}
									else
									{
										// 下降線のためmaxからmin
										PPLineMaxToMin();
									}
								}
								// 前回が下降線
								else
								{
									if (m_TrendDataPP[nID].spaceData[pos].ppPair.min <= m_TrendDataPP[nID].spaceData[pos - 1].ppPair.min)
									{
										// 下降線のためmaxからmin
										PPLineMaxToMin();
									}
									else
									{
										// 上昇線のためminからmax
										PPLineMinToMax();
									}
								}
							}
							break;

						case Dots:
							// 円の描画
							PPEllipse();
							break;

						case LineAndDots:
							// 線の描画
							if (xpos == 0 || pos == 0 ||
								m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange && (char)m_TrendDataPP[nID].spaceData[pos - 1].status[0] < 0 ||
								m_TrendDataPP[nID].srcType == eSigSrcType_File && bFirst)
							{
								PPLine1st();
								bFirst = FALSE;
							}
							else
							{
								// 前回が上昇線
								if (bIsLastMaxValue == TRUE)
								{
									if (m_TrendDataPP[nID].spaceData[pos].ppPair.max >= m_TrendDataPP[nID].spaceData[pos - 1].ppPair.max)
									{
										// 上昇線のためminからmax
										PPLineMinToMax();
									}
									else
									{
										// 下降線のためmaxからmin
										PPLineMaxToMin();
									}
								}
								// 前回が下降線
								else
								{
									if (m_TrendDataPP[nID].spaceData[pos].ppPair.min <= m_TrendDataPP[nID].spaceData[pos - 1].ppPair.min)
									{
										// 下降線のためmaxからmin
										PPLineMaxToMin();
									}
									else
									{
										// 上昇線のためminからmax
										PPLineMinToMax();
									}
								}
							}
							// 円の描画
							PPEllipse();
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

					// ファイルタイプのデータから表示用データ値取得
					if (m_TrendDataPP[nID].srcType == eSigSrcType_File && bSetCurVal == FALSE && cur_t >= m_MostNewTime - m_nTrendDataPos)				
					{
						bSetCurVal = TRUE;
						m_curVal[nID] = val;
					}
				}

				saveX = pointDraw.x;
				pos++;
			}

			if (m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange)
			{
				m_curVal[nID] = val;
			}

			if (ppTrend == true && size > 1)
			{
				saveX = -1;
			}

			// 測定データの描画
			memDC.SelectObject(oldPen);
			memDC.SelectObject(oldBrush);
			memDC.SelectObject(oldFont);
			penLine.DeleteObject();
			brushDots.DeleteObject();
		}
		else
		{
			m_bIsDrawOn[nID] = TRUE;
		}
	}

	CGraphMutex::Unlock(eData);
}

/*============================================================================*/
/*! グラフ描画

-# トレンドグラフ線の描画

@param	memDC	デバイスコンテキスト
@retval	なし

*/
/*============================================================================*/
void CGraphWindow::DrawOffTrend(CDC& memDC)
{
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
BOOL CGraphWindow::AddLineToMap(UINT nID, structLine line)
{
//#ifdef _DEBUG
//	{
//		CString str;
//		str.Format(_T("%s(%d) :: Sig[%d], GraphType = %u, ViewID = %u"), __FILEW__, __LINE__, nID, m_nGraphType, m_nPropertiesID);
//		CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("AddLineToMap"), _T("debug"), str, nLogEx::debug);
//	}
//#endif

	MutexLock();
	CGraphMutex::Lock(eData);

	// 最大値、最小値を初期化
	if (m_DrawLines.find(nID) == m_DrawLines.end())
	{
		line.nMaxValue = m_dTotalScaleLMax;
		line.nMinValue = m_dTotalScaleLMin;
	}

	CSpaceData*	spaceDataList = theApp.GetDataCollection().GetSpaceData(m_StationIndex);

	// 固定スケール
	// または自動スケール（非初期値）
	if (m_DrawLines.find(nID) != m_DrawLines.end() &&
		(m_nStvScale.Auto == FALSE ||
		(spaceDataList[0].value.empty() == FALSE)))
	{
		line.nMaxValue = m_DrawLines[nID].nMaxValue;
		line.nMinValue = m_DrawLines[nID].nMinValue;
	}
	// 自動スケール（初期値）
	else
	{
		line.nMaxValue = m_dTotalScaleLMax;
		line.nMinValue = m_dTotalScaleLMin;
	}

//	// 信号名が異なる時にクリア
//	if (m_DrawLines[nID].strGName.Compare(line.strGName) != 0)
	{
//#ifdef _DEBUG
//		{
//			CString str;
//			str.Format(_T("%s(%d) :: Diff Sig[%d], GraphType = %u, ViewID = %u"), __FILEW__, __LINE__, nID, m_nGraphType, m_nPropertiesID);
//			CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("AddLineToMap"), _T("debug"), str, nLogEx::debug);
//		}
//#endif
//		line.nListIdx = -1;				// 衛星データリストのインデックス初期化
		line.nMaxValue = -DBL_MAX;		// 最大値クリア
		line.nMinValue = DBL_MAX;		// 最小値クリア

		m_nQueuLastTotalSize[nID] = 0;	// 直近のキュートータルサイズクリア
		m_PPNumber[nID] = 0;			// 圧縮位置クリア
		m_PPTotalCount[nID] = 0;		// 圧縮合計数クリア
		m_TrendDataPP[nID].spaceData.clear();		// 圧縮リストクリア
		m_TrendDataPP[nID].spaceData.shrink_to_fit();	// 圧縮リストのメモリクリア
		m_bIsDrawOn[nID] = TRUE;		// 描画ON
	}

	m_DrawLines[nID] = line;

	// 左縦軸の最大値最小値更新
	if (m_DrawLines[nID].nVScaleKind == LScale)
	{
		if (m_DrawLines[nID].nMaxValue > m_dTotalScaleLMax)
		{
			m_dTotalScaleLMax = m_DrawLines[nID].nMaxValue;
		}
		if (m_DrawLines[nID].nMinValue < m_dTotalScaleLMin)
		{
			m_dTotalScaleLMin = m_DrawLines[nID].nMinValue;
		}
	}

	m_bExitData = FALSE;

	CGraphMutex::Unlock(eData);
	MutexUnlock();

	CString	str;
	if (m_DrawLines[nID].strGName.IsEmpty() == false && m_DrawLines[nID].strName.IsEmpty() == false)
	{
		str = m_DrawLines[nID].strGName;
	}
	else if (m_DrawLines[nID].strName.IsEmpty() == false)
	{
		str = m_DrawLines[nID].strName;
	}
	else
	{
		str = _T("");
	}

	if (m_StationIndex == eStation_Usuda642)
	{
		// 54m
		if (m_txtSigName[nID].GetSafeHwnd() != NULL)
		{
			if (str.IsEmpty())
			{
				m_txtSigName[nID].ShowWindow(SW_HIDE);
			}
			else
			{
				m_txtSigName[nID].SetWindowTextW(str);
				m_txtSigName[nID].ShowWindow(SW_SHOW);
			}
		}
	}
	else
	{
		// 54m以外
		if (m_chkBtnViewSig[nID].GetSafeHwnd() != NULL)
		{
			if (str.IsEmpty())
			{
				m_chkBtnViewSig[nID].ShowWindow(SW_HIDE);
			}
			else
			{
				m_chkBtnViewSig[nID].SetWindowTextW(str);
				m_chkBtnViewSig[nID].ShowWindow(SW_SHOW);
				CPropertiesData& prop = theApp.GetSatelliteData().GetTrendPropertiesData(m_nPropertiesID);
				if (prop.isViewMode[nID])
				{
					m_chkBtnViewSig[nID].SetCheck(BST_CHECKED);
				}
				else
				{
					m_chkBtnViewSig[nID].SetCheck(BST_UNCHECKED);
				}
			}
		}
	}
	return TRUE;
}

/*============================================================================*/
/*! グラフ描画

-# 線属性データの追加

@param nID		ラインＩＤ
@param pParam	パラメータ
@param nGraphKind	グラフ種別
@param nStyle	ラインスタイル
@retval	bool

*/
/*============================================================================*/
BOOL CGraphWindow::AddLine(UINT nID, CPropertiesData* pParam, UINT nGraphKind, DWORD nStyle)
{
	// 線スタイルが異常の場合はデフォルト設定
	if (!((PS_STYLE_MASK)& nStyle))
		nStyle |= PS_SOLID;

	structLine line;

	line.nDrawKind = Line;
	line.bIsVisible = pParam->isViewMode[nID];
	line.crColor = pParam->color[nID];
	line.nStyle = nStyle;
	line.nWidth = pParam->lineWidth[nID];
	line.nDotSize = 0;
	line.nGraphKind = nGraphKind;
	line.strName = pParam->dataName[nID];
	line.strGName = pParam->dataGName[nID];
	line.strUnit = pParam->dataUnit[nID];
	line.nDecDidits = pParam->nDecDidits[nID];
	line.nVScaleKind = pParam->vscaleKind[nID];
	if (line.strName.IsEmpty())
	{
		line.nListIdx = -1;
	}
	else
	{
		line.nListIdx = theApp.GetDataCollection().GetGraphSigIndex(m_StationIndex, line.strName);
	}

	// 線ID毎の情報マップにライン追加
	AddLineToMap(nID, line);

	return TRUE;
}
/*============================================================================*/
/*! グラフ描画

-# 点属性データの追加

@param nID		ラインＩＤ
@param pParam	パラメータ
@param nGraphKind	グラフ種別
@retval	bool

*/
/*============================================================================*/
BOOL CGraphWindow::AddDot(UINT nID, CPropertiesData* pParam, UINT nGraphKind)
{
	// 点スタイルが異常の場合はデフォルト設定
	if (!((PS_STYLE_MASK)& nGraphKind))
		nGraphKind |= PS_SOLID;

	structLine line;

	line.nDrawKind = Dots;
	line.bIsVisible = pParam->isViewMode[nID];
	line.crColor = pParam->color[nID];
	line.nStyle = PS_NULL;
	line.nWidth = 0;
	line.nDotSize = GetDotSize(pParam->dotSize[nID]);
	line.nGraphKind = nGraphKind;
	line.strName = pParam->dataName[nID];
	line.strGName = pParam->dataGName[nID];
	line.strUnit = pParam->dataUnit[nID];
	line.nDecDidits = pParam->nDecDidits[nID];
	line.nVScaleKind = pParam->vscaleKind[nID];
	if (line.strName.IsEmpty())
	{
		line.nListIdx = -1;
	}
	else
	{
		line.nListIdx = theApp.GetDataCollection().GetGraphSigIndex(m_StationIndex, line.strName);
	}

	// 線ID毎の情報マップにライン追加
	AddLineToMap(nID, line);

	return TRUE;
}
/*============================================================================*/
/*! グラフ描画

-# 線＆点属性データの追加

@param nID		ラインＩＤ
@param pParam	パラメータ
@param nGraphKind	グラフ種別
@param nStyle	ラインスタイル
@retval	bool

*/
/*============================================================================*/
BOOL CGraphWindow::AddLineAndDot(UINT nID, CPropertiesData* pParam, UINT nGraphKind, DWORD nStyle)
{
	// 線スタイルが異常の場合はデフォルト設定
	if (!((PS_STYLE_MASK)& nStyle))
		nStyle |= PS_SOLID;

	structLine line;

	line.nDrawKind = LineAndDots;
	line.bIsVisible = pParam->isViewMode[nID];
	line.crColor = pParam->color[nID];
	line.nStyle = nStyle;
	line.nWidth = pParam->lineWidth[nID];
	line.nDotSize = GetDotSize(pParam->dotSize[nID]);
	line.nGraphKind = nGraphKind;
	line.strName = pParam->dataName[nID];
	line.strGName = pParam->dataGName[nID];
	line.strUnit = pParam->dataUnit[nID];
	line.nDecDidits = pParam->nDecDidits[nID];
	line.nVScaleKind = pParam->vscaleKind[nID];
	if (line.strName.IsEmpty())
	{
		line.nListIdx = -1;
	}
	else
	{
		line.nListIdx = theApp.GetDataCollection().GetGraphSigIndex(m_StationIndex, line.strName);
	}

	// 線ID毎の情報マップにライン追加
	AddLineToMap(nID, line);

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

//	if (trandHScale == autoscale || m_nTrandHScale != trandHScale)
	if (m_nTrandHScale != trandHScale)
	{
		m_nTrendDataPos = 0;
	}

	m_nTrandHScale = (UINT)trandHScale;

	if (m_nTrandHScale == yohochiscale && theApp.GetSatelliteData().m_trendData.m_gst_dspinf.i_yohouti_umu == YOHOUTI_ARI)
	{
		double dbHzRange = theApp.GetSatelliteData().m_trendData.m_gst_dspinf.d_LOS_time - theApp.GetSatelliteData().m_trendData.m_gst_dspinf.d_AOS_time;
		if (dbHzRange > 0)
		{
			// 横軸領域設定
			SetHzRange(dbHzRange + 1);
		}
	}
	else
	{
		// 横軸領域設定
		SetHzRange(mHScale[trandHScale] + 1);
	}

	// 前回と横軸設定が変わった場合は初期化
	if (nOldTrandHScale != (HScale)m_nTrandHScale)
	{
#ifdef _DEBUG
		{
			CString str;
			str.Format(_T("%s(%d) :: Change scale : GraphType = %u, ViewID = %u"), __FILEW__, __LINE__, m_nGraphType, m_nPropertiesID);
			CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("AddAxis"), _T("debug"), str, nLogEx::debug);
		}
#endif
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
//void CGraphWindow::ClearLine(UINT nID)
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

@param	bAll：TRUE 全更新 / FALSE グラフ描画のみ更新
@retval

*/
/*============================================================================*/
void CGraphWindow::UpdateGraph(BOOL bAll /*= FALSE*/)
{
	ClearCursor();
	Normalize();

	if (!bAll)
	{
		// グラフのみ再描画
		CRect rectClient;
		GetClientRect(rectClient);
		CRect rectGrid;
		if (m_bViewDetail)
		{
			// 積分ボタンの監視取得
			UpdateIntegKanshi();

			// 詳細表示時の軸領域
			rectGrid.SetRect(
				rectClient.left,
				rectClient.top,
				rectClient.right - mGridMarginRight,
				rectClient.bottom - mGridMarginBottom);
			InvalidateRect(rectGrid);
			rectClient.top = rectGrid.bottom;
			rectClient.right = rectGrid.right + mRightMargin;
			InvalidateRect(rectClient);
			InvalidateRect(m_rectCurValArea);
		}
		else
		{
			// 簡易表示時の軸領域
			rectGrid = rectClient;
			InvalidateRect(rectGrid);
		}
	}
	else
	{
		// 積分ボタンの監視取得
		UpdateIntegKanshi();

		// 全再描画
		Invalidate();
	}
}


void CGraphWindow::ResetCursor(int pos)
{
	CRect crect;
	GetClientRect(crect);

	CDrawTool::c_drawShape = line;
	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	if (pTool == NULL){
		CDrawTool::c_drawShape = selection;
		return;
	}

	if (pos < 0){
		Remove(objline);
		CRect r = crect;
		r.right = r.left;
		pTool->Create(this, r);
		CDrawTool::c_drawShape = selection;
		return;
	}

	Select(objline);

	pTool->MoveLineCursor(this, CPoint(crect.left + pos, crect.top));
	CDrawTool::c_drawShape = selection;
}

void CGraphWindow::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_nGraphType == eGraphType_All){
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
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CGraphWindow::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_nGraphType == eGraphType_All){
		if (theApp.GetTrendScrollMode(m_nViewNo) == true){
			CRect	rect;
			GetClientRect(rect);
//			if (rect.PtInRect(point) && m_nTrandHScale != autoscale){
			if (rect.PtInRect(point)){
				m_ptTrendMove = point;
			}
		}
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

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

	CWnd::OnLButtonDown(nFlags, point);
}


void CGraphWindow::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_nGraphType == eGraphType_All){
		if (theApp.GetTrendScrollMode(m_nViewNo) == true){
			if (nFlags & MK_LBUTTON){
				CRect	rect;
				GetClientRect(rect);
				int size = (int)GetPPDataMaxSize();
				if (size != 0 && rect.PtInRect(point)){
					if (m_ptTrendMove.x >= point.x){
						// 右方向
						m_nTrendDataPos += 1;
						if (size <= m_nTrendDataPos)
							m_nTrendDataPos = (size - 1);
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
		}
		CWnd::OnMouseMove(nFlags, point);
		return;
	}

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

	CWnd::OnMouseMove(nFlags, point);
}


void CGraphWindow::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_nGraphType == eGraphType_All){
		CWnd::OnLButtonUp(nFlags, point);
		return;
	}
	if (m_nGraphType == eGraphType_OffAll){
		CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
		if (pTool != NULL)
			pTool->OnLButtonUp(this, nFlags, point);

		CDrawTool::c_drawShape = selection;
		return;
	}

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


void CGraphWindow::AjustLayout()
{
	if (m_btnSetting.GetSafeHwnd() != NULL)
	{
		// 詳細表示の場合のみ設定ボタン表示
		if (m_bViewDetail)
		{
			m_btnSetting.ShowWindow(SW_SHOW);
		}
		else
		{
			m_btnSetting.ShowWindow(SW_HIDE);
		}

		// 設定ボタン移動
		CRect rcBtn;
		GetClientRect(rcBtn);
		rcBtn.right -= mBtnSettingMarginRight;
		rcBtn.left = rcBtn.right - mBtnWidth;
		rcBtn.top += mBtnSettingMarginTop;
		rcBtn.bottom = rcBtn.top + mBtnHeight;

		if (m_btnSetting.GetSafeHwnd() != NULL)
			m_btnSetting.MoveWindow(rcBtn);
	}

	if (m_btnHGridChk.GetSafeHwnd() != NULL)
	{
		// 詳細表示の場合のみ横罫線表示ON/OFFチェックボックス表示
		if (m_bViewDetail)
		{
			m_btnHGridChk.ShowWindow(SW_SHOW);
		}
		else
		{
			m_btnHGridChk.ShowWindow(SW_HIDE);
		}

		// 横罫線表示ON/OFFチェックボックス移動
		CRect rcBtn;
		GetClientRect(rcBtn);
		rcBtn.left = rcBtn.right - mGridMarginRight + mChkBtnMarginRight;
		rcBtn.right = rcBtn.left + mChkBtnWidth;
		rcBtn.top += mBtnSettingMarginTop;
		rcBtn.bottom = rcBtn.top + mChkBtnHeight;

		if (m_btnHGridChk.GetSafeHwnd() != NULL)
			m_btnHGridChk.MoveWindow(rcBtn);
	}

	CRect rect;
	GetClientRect(rect);
	rect.SetRect(
		rect.left,
		rect.top + mGridMarginTop,
		rect.right,
		rect.bottom - mGridMarginBottom + 1);

	// 信号表示チェックボタン表示
	if (m_StationIndex == eStation_Usuda642)
	{
		// 54m
		for (int i = 0; i < MAX_TDATA; i++)
		{
			if (m_txtSigName[i].GetSafeHwnd() != NULL)
			{
				// 詳細表示の場合のみ横罫線表示ON/OFFチェックボックス表示
				if (m_bViewDetail && m_DrawLines.size() > i && !m_DrawLines[i].strGName.IsEmpty())
				{
					m_txtSigName[i].ShowWindow(SW_SHOW);
				}
				else
				{
					m_txtSigName[i].ShowWindow(SW_HIDE);
				}

				// 横罫線表示ON/OFFチェックボックス移動
				CRect rcSigBtn = rect;
				CString strSigBtn;
				CPoint ptSigName;
				ptSigName.x = rcSigBtn.right - mGridMarginRight + mRightMargin + mLegendWeidth + mLegendLineStrDistance;
				ptSigName.y = rcSigBtn.top + mTopMargin + mSigStrHeight * i;
				rcSigBtn.left = ptSigName.x;
				rcSigBtn.right = rcSigBtn.left + mChkBtnWidth - mHStrDistance;
				rcSigBtn.top = ptSigName.y;
				rcSigBtn.bottom = rcSigBtn.top + 12;

				if (m_txtSigName[i].GetSafeHwnd() != NULL)
					m_txtSigName[i].MoveWindow(rcSigBtn);
			}
		}
	}
	else
	{
		// 54m以外
		for (int i = 0; i < MAX_TDATA; i++)
		{
			if (m_chkBtnViewSig[i].GetSafeHwnd() != NULL)
			{
				// 詳細表示の場合のみ横罫線表示ON/OFFチェックボックス表示
				if (m_bViewDetail && m_DrawLines.size() > i && !m_DrawLines[i].strGName.IsEmpty())
				{
					m_chkBtnViewSig[i].ShowWindow(SW_SHOW);
				}
				else
				{
					m_chkBtnViewSig[i].ShowWindow(SW_HIDE);
				}

				// 横罫線表示ON/OFFチェックボックス移動
				CRect rcSigBtn = rect;
				CString strSigBtn;
				CPoint ptSigName;
				ptSigName.x = rcSigBtn.right - mGridMarginRight + mRightMargin + mLegendWeidth + mLegendLineStrDistance;
				ptSigName.y = rcSigBtn.top + mTopMargin + mSigStrHeight * i;
				rcSigBtn.left = ptSigName.x;
				rcSigBtn.right = rcSigBtn.left + mChkBtnWidth - mHStrDistance;
				rcSigBtn.top = ptSigName.y;
				rcSigBtn.bottom = rcSigBtn.top + 12;

				if (m_chkBtnViewSig[i].GetSafeHwnd() != NULL)
					m_chkBtnViewSig[i].MoveWindow(rcSigBtn);
			}
		}
	}

	if (m_btnIntg.GetSafeHwnd() != NULL)
	{

		// 詳細表示の場合のみ積分時間ボタン表示
		if (m_bViewDetail)
		{
			m_btnIntg.ShowWindow(SW_SHOW);
		}
		else
		{
			m_btnIntg.ShowWindow(SW_HIDE);
		}

		// 積分時間ボタン移動
		CRect rcBtn;
		GetClientRect(rcBtn);
		m_rectIntgBtn.right = rcBtn.right - mBtnIntgMarginRight;
		m_rectIntgBtn.left = m_rectIntgBtn.right - mBtnWidth;
		if (m_btnIntg.GetSafeHwnd() != NULL)
			m_btnIntg.MoveWindow(m_rectIntgBtn);
	}

	ClearCursor();
}



void CGraphWindow::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	AjustLayout();
}



/*============================================================================*/
/*! グラフ描画

-# スクロール情報の更新

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::UpdateScrollInfo()
{
	//if (m_nGraphType != eGraphType_OffTrend){
	//	return;
	//}

	int size = (int)GetPPDataMaxSize(); //関数化する、データがあるリストのサイズを取得
	if (size == 0)
	{
		return;
	}
#ifdef _DEBUG
	CString	str;
	str.Format(_T("m_nTrendDataPos = %I64d, size = %d"), m_nTrendDataPos, size);
	CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("UpdateScrollInfo"), _T("debug"), str, nLogEx::debug);
#endif

	SCROLLINFO si;

	si.cbSize = sizeof(SCROLLINFO);
	TRY
	{
		si.fMask = SIF_ALL;
		si.nPos = (int)m_nTrendDataPos;
		si.nMin = 0;
		si.nMax = size;
		si.nPage = mHScale[m_nTrandHScale];
	}CATCH_ALL(e){
		return;
	} END_CATCH_ALL

	SetScrollInfo(SB_HORZ, &si, TRUE);
	SendNewLimits();
}

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


void CGraphWindow::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int size = (int)GetPPDataMaxSize();

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
		if (size <= m_nTrendDataPos)
			m_nTrendDataPos = (size - 1);
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
		if (size <= m_nTrendDataPos)
			m_nTrendDataPos = (size - 1);
		break;

	case SB_THUMBTRACK:
		m_nTrendDataPos = nPos;
		if (size <= m_nTrendDataPos)
			m_nTrendDataPos = (size - 1);
		break;
	default:
		return;
	}

	Invalidate();
	UpdateScrollInfo();
//	UpdateWindow();

	//CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}
/*============================================================================*/
/*! グラフ描画

-# 正常位置の更新

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::Normalize()
{
	//if (m_nGraphType != eGraphType_OffTrend){
	//	return;
	//}
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
	UpdateScrollInfo();
//	UpdateWindow();
}



void CGraphWindow::ClientToDoc(CPoint& point)
{
}

void CGraphWindow::ClientToDoc(CRect& rect)
{
	CRect rc;
	GetClientRect(rc);
	rect.top = rc.top;
	rect.bottom = rc.bottom;
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
void CGraphWindow::DrawObj(CDC* pDC)
{
	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObject* pObj = m_objects.GetNext(pos);
		pObj->Draw(pDC, FALSE);
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
/*! CGraphWindow

-# 数値にカンマを追加

@param	pos：カンマを付けたい数値
@param	pointKeta：少数点以下の桁数 (少数点0～6)
@retval	カンマを付けた後の数値

*/
/*============================================================================*/
CString CGraphWindow::addCanma(double pos, int pointKeta)
{
	int	i, j, keta, temp;
	int sign = 0;
	double tempPos = pos;
	char str[MAXDIGIT];

	if (isnan(pos)){
		// NaN (非数)の場合は０にする
		tempPos = 0.0;
	}

	// 0に近い場合はまるめる
	switch (pointKeta)
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
	default:
		break;
	}

	__int64 num = (__int64)tempPos;
	double dnum = fabs(tempPos - (double)num);
	CString strSyosuu;
	switch (pointKeta)
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
		if (i + 1 >= MAXDIGIT)
		{
			return CString(_T("#,###,###,###,###.######"));
		}
		str[i++] = '-';
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

-# PP圧縮変換

@param	トレースデータ番号
@param	トレンド圧縮データキュー
@param	監視データ
@param	最小値最大値
@retval	なし

*/
/*============================================================================*/
void CGraphWindow::ConvertPP(int trace, deque<stSpaceData>& trendPP, stSpaceData newData, structPPMinMax& stPPMinMax)
{
	__int64 number = m_PPNumber[trace];
	// 領域が圧縮境界サイズを超えており、圧縮リストがいっぱいの場合
	if ((m_dbHzRange > MAX_PPDATA_SIZE) && trendPP.size() == MAX_PPDATA_SIZE)
	{
		// 圧縮する必要あり
		if ((number + 1) == MAX_PPDATA_SIZE){
			// 圧縮処理が終端に達した
			// 格納されている最後のデータと新しく追加するデータを圧縮する
			trendPP[number].ppPair.count += newData.ppPair.count;
			if ((char)trendPP[number].status[0] >= 0 && (char)newData.status[0] >= 0)
			{
				trendPP[number].ppPair.max = __max(trendPP[number].ppPair.max, newData.ppPair.max);
				trendPP[number].ppPair.min = __min(trendPP[number].ppPair.min, newData.ppPair.min);
			}

			// ステータスを更新する
			if ((char)trendPP[number].status[0] < 0 && (char)newData.status[0] >= 0)
			{
				trendPP[number].status[0] = newData.status[0];
				trendPP[number].ppPair.max = newData.ppPair.max;
				trendPP[number].ppPair.min = newData.ppPair.min;
			}

			// 最小最大を計算
			CalcPPMinMaxWithCompareCount(trendPP, number, stPPMinMax);
			// 位置を0クリア
			number = 0;
			// PP圧縮キューに入れた監視データサイズ合計
			m_PPTotalCount[trace] += newData.ppPair.count;
		}
		else
		{
			// 圧縮処理が終端に達していない
			if (trendPP[MAX_PPDATA_SIZE - 2].ppPair.count != trendPP[MAX_PPDATA_SIZE - 1].ppPair.count)
			{
				trendPP[MAX_PPDATA_SIZE - 1].ppPair.count += newData.ppPair.count;
				if ((char)trendPP[MAX_PPDATA_SIZE - 1].status[0] >= 0 && (char)newData.status[0] >= 0)
				{
					trendPP[MAX_PPDATA_SIZE - 1].ppPair.max = __max(trendPP[MAX_PPDATA_SIZE - 1].ppPair.max, newData.ppPair.max);
					trendPP[MAX_PPDATA_SIZE - 1].ppPair.min = __min(trendPP[MAX_PPDATA_SIZE - 1].ppPair.min, newData.ppPair.min);
				}

				// ステータスを更新する
				if ((char)trendPP[MAX_PPDATA_SIZE - 1].status[0] < 0 && (char)newData.status[0] >= 0)
				{
					trendPP[MAX_PPDATA_SIZE - 1].status[0] = newData.status[0];
					trendPP[MAX_PPDATA_SIZE - 1].ppPair.max = newData.ppPair.max;
					trendPP[MAX_PPDATA_SIZE - 1].ppPair.min = newData.ppPair.min;
				}

				// 最小最大を計算
				CalcPPMinMaxWithCompareCount(trendPP, MAX_PPDATA_SIZE - 1, stPPMinMax);
				// PP圧縮キューに入れた監視データサイズ合計
				m_PPTotalCount[trace] += newData.ppPair.count;
			}
			else
			{
				trendPP[number].ppPair.count += trendPP[number + 1].ppPair.count;
				if ((char)trendPP[number].status[0] >= 0 && (char)trendPP[number + 1].status[0] >= 0)
				{
					trendPP[number].ppPair.max = __max(trendPP[number].ppPair.max, trendPP[number + 1].ppPair.max);
					trendPP[number].ppPair.min = __min(trendPP[number].ppPair.min, trendPP[number + 1].ppPair.min);
				}
				// ステータスを更新する
				if ((char)trendPP[number].status[0] < 0 && (char)trendPP[number + 1].status[0] >= 0)
				{
					trendPP[number].status[0] = trendPP[number + 1].status[0];
					trendPP[number].ppPair.max =  trendPP[number + 1].ppPair.max;
					trendPP[number].ppPair.min = trendPP[number + 1].ppPair.min;
				}

				number++;

				// 圧縮を行ったので圧縮されたデータを削除する
				int pos = 0;
				deque<stSpaceData>::iterator itr;
				for (itr = trendPP.begin(); itr != trendPP.end(); itr++, pos++)
				{
					if (number == pos)
					{
						trendPP.erase(itr);
						break;
					}
				}
				trendPP.push_back(newData);

				__int64	nTailPos = trendPP.size() - 1;
				// 最小最大を計算
				CalcPPMinMaxWithCompareCount(trendPP, nTailPos, stPPMinMax);
				// PP圧縮キューに入れた監視データサイズ合計
				m_PPTotalCount[trace] += newData.ppPair.count;
			}
		}
	}
	// 圧縮リストから削除後の処理
	else if (trendPP.empty() == FALSE && trendPP[0].ppPair.count > 1)
	{
		// 圧縮カウント境界あり
		if ((number > 0 && trendPP[number - 1].ppPair.count != trendPP[number].ppPair.count) || number == 0)
		{
			// 圧縮リストの末尾の場合
			if (number == trendPP.size() - 1)
			{
				// 末尾に新しいデータを圧縮追加
				trendPP[number].ppPair.count += newData.ppPair.count;
				if ((char)trendPP[number].status[0] >= 0 && (char)newData.status[0] >= 0)
				{
					trendPP[number].ppPair.max = __max(trendPP[number].ppPair.max, newData.ppPair.max);
					trendPP[number].ppPair.min = __min(trendPP[number].ppPair.min, newData.ppPair.min);
				}
				// ステータスを更新する
				if ((char)trendPP[number].status[0] < 0 && (char)newData.status[0] >= 0)
				{
					trendPP[number].status[0] = newData.status[0];
					trendPP[number].ppPair.max = newData.ppPair.max;
					trendPP[number].ppPair.min = newData.ppPair.min;
				}

				// 最小最大を計算
				CalcPPMinMaxWithCompareCount(trendPP, number, stPPMinMax);
				// PP圧縮キューに入れた監視データサイズ合計
				m_PPTotalCount[trace] += newData.ppPair.count;
			}
			// 圧縮リストの末尾でない場合
			else
			{
				// 圧縮リストの末尾でない場合は、圧縮リストの末尾が圧縮済みか判断
				__int64	nEndPos = trendPP.size() - 1;
				if ((nEndPos > 0 && trendPP[nEndPos - 1].ppPair.count != trendPP[nEndPos].ppPair.count) || nEndPos == 0)
				{
					// 末尾に新しいデータを圧縮追加
					trendPP[nEndPos].ppPair.count += newData.ppPair.count;
					if ((char)trendPP[nEndPos].status[0] >= 0 && (char)newData.status[0] >= 0)
					{
						trendPP[nEndPos].ppPair.max = __max(trendPP[nEndPos].ppPair.max, newData.ppPair.max);
						trendPP[nEndPos].ppPair.min = __min(trendPP[nEndPos].ppPair.min, newData.ppPair.min);
					}
					// ステータスを更新する
					if ((char)trendPP[nEndPos].status[0] < 0 && (char)newData.status[0] >= 0)
					{
						trendPP[nEndPos].status[0] = newData.status[0];
						trendPP[nEndPos].ppPair.max = newData.ppPair.max;
						trendPP[nEndPos].ppPair.min = newData.ppPair.min;
					}

					// 最小最大を計算
					CalcPPMinMaxWithCompareCount(trendPP, nEndPos, stPPMinMax);
					// PP圧縮キューに入れた監視データサイズ合計
					m_PPTotalCount[trace] += newData.ppPair.count;
				}
				else
				{
					// 既存データの圧縮
					trendPP[number].ppPair.count += trendPP[number + 1].ppPair.count;
					if ((char)trendPP[number].status[0] >= 0 && (char)trendPP[number + 1].status[0] >= 0)
					{
						trendPP[number].ppPair.max = __max(trendPP[number].ppPair.max, trendPP[number + 1].ppPair.max);
						trendPP[number].ppPair.min = __min(trendPP[number].ppPair.min, trendPP[number + 1].ppPair.min);
					}
					// ステータスを更新する
					if ((char)trendPP[number].status[0] < 0 && (char)trendPP[number + 1].status[0] >= 0)
					{
						trendPP[number].status[0] = trendPP[number + 1].status[0];
						trendPP[number].ppPair.max = trendPP[number + 1].ppPair.max;
						trendPP[number].ppPair.min = trendPP[number + 1].ppPair.min;
					}
					number++;

					// 圧縮を行ったので圧縮されたデータを削除する
					int pos = 0;
					deque<stSpaceData>::iterator itr;
					for (itr = trendPP.begin(); itr != trendPP.end(); itr++, pos++)
					{
						if (number == pos)
						{
							trendPP.erase(itr);
							break;
						}
					}
					// 圧縮リスト末尾に新しいデータを追加
					trendPP.push_back(newData);

					__int64	nTailPos = trendPP.size() - 1;
					// 最小最大を計算
					CalcPPMinMaxWithCompareCount(trendPP, nTailPos, stPPMinMax);
					// PP圧縮キューに入れた監視データサイズ合計
					m_PPTotalCount[trace] += newData.ppPair.count;
				}
			}
		}
		else
		{
			// 圧縮リスト末尾に新しいデータを追加
			trendPP.push_back(newData);

			__int64	nTailPos = trendPP.size() - 1;
			// 最小最大を計算
			CalcPPMinMaxWithCompareCount(trendPP, nTailPos, stPPMinMax);
			// PP圧縮キューに入れた監視データサイズ合計
			m_PPTotalCount[trace] += newData.ppPair.count;
			number++;
		}
	}
	// 圧縮する必要なし
	else
	{
		// 圧縮リスト末尾に新しいデータを追加
		trendPP.push_back(newData);

		__int64	nTailPos = trendPP.size() - 1;
		// 最小最大を計算
		CalcPPMinMaxWithCompareCount(trendPP, nTailPos, stPPMinMax);
		// PP圧縮キューに入れた監視データサイズ合計
		m_PPTotalCount[trace] += newData.ppPair.count;
	}

	m_PPNumber[trace] = number;
}


/*============================================================================*/
/*! CDataCollection

-# PP圧縮データを半分にする

@param	トレースデータ番号
@param	トレンド圧縮データキュー
@retval	なし

*/
/*============================================================================*/
void CGraphWindow::DeleteHalfPP(int trace, deque<stSpaceData>& trendPP)
{
	// 隣同士を圧縮していく
	__int64	number;
	__int64 oldSize = (__int64)trendPP.size();
	for (number = 0; number < (__int64)trendPP.size() - 1;)
	{
		trendPP[number].ppPair.count += trendPP[number + 1].ppPair.count;
		if ((char)trendPP[number].status[0] >= 0 && (char)trendPP[number + 1].status[0] >= 0)
		{
			trendPP[number].ppPair.max = __max(trendPP[number].ppPair.max, trendPP[number + 1].ppPair.max);
			trendPP[number].ppPair.min = __min(trendPP[number].ppPair.min, trendPP[number + 1].ppPair.min);
		}
		// ステータスを更新する
		if ((char)trendPP[number].status[0] < 0 && (char)trendPP[number + 1].status[0] >= 0)
		{
			trendPP[number].status[0] = trendPP[number + 1].status[0];
			trendPP[number].ppPair.max = trendPP[number + 1].ppPair.max;
			trendPP[number].ppPair.min = trendPP[number + 1].ppPair.min;
		}
		number++;

		// 圧縮されたデータを削除する
		int pos = 0;
		deque<stSpaceData>::iterator itr;
		for (itr = trendPP.begin(); itr != trendPP.end(); itr++, pos++)
		{
			if (number == pos){
				trendPP.erase(itr);
				break;
			}
		}
	}

	if (number > 0)
	{
		if (oldSize % 2 == 0)
		{
			m_PPNumber[trace] = number - 1;
		}
		else
		{
			m_PPNumber[trace] = number;
		}
	}
	else
	{
		m_PPNumber[trace] = 0;
	}

	m_nTrendDataPos = 0;
}

/*============================================================================*/
/*! CDataCollection

-# PP圧縮データの頭の部分を消す

@param	トレースデータ番号
@param	トレンド圧縮データキュー
@retval	なし

*/
/*============================================================================*/
void CGraphWindow::DeleteHeadPP(int trace, deque<stSpaceData>& trendPP, int srcType)
{
	if (trendPP.empty() == FALSE)
	{
		// 圧縮を行ったので圧縮されたデータを削除する
		deque<stSpaceData>::iterator itr = trendPP.begin();
		m_PPTotalCount[trace] -= (*itr).ppPair.count;
		trendPP.erase(itr);

		if (m_PPNumber[trace] > 0)
		{
			m_PPNumber[trace] -= 1;
		}

//#ifdef _DEBUG
//	{
//		if (m_TrendDataPP[trace].spaceData.size() > 0)
//		{
//			CString str;
//			str.Format(_T("%s(%d) :: Call delete headPP Sig[%d], GraphType = %u, ViewID = %u, cur_t = %I64d, size = %I64d"), __FILEW__, __LINE__, trace, m_nGraphType, m_nPropertiesID, CTime(m_TrendDataPP[trace].spaceData[m_TrendDataPP[trace].spaceData.size() - 1].systime).GetTime(), m_TrendDataPP[trace].spaceData.size());
//			CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("DeleteHeadPP"), _T("debug"), str, nLogEx::debug);
//
//		}
//	}
//#endif
	}

	m_nTrendDataPos = 0;

//	if (srcType == eSigSrcType_EngExchange)
//	{
//		deque<stSpaceData>::iterator itr = trendPP.begin();
//		if (itr != trendPP.end() && !m_bFitYohochi)
//		{
//			m_StartTime = CTime((*itr).systime).GetTime();
//		}
//	}
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
#ifdef _DEBUG
	{
		CString str;
		str.Format(_T("%s(%d) :: GraphType = %u, ViewID = %u"), __FILEW__, __LINE__, m_nGraphType, m_nPropertiesID);
		CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("InitSatelliteDataSetting"), _T("debug"), str, nLogEx::debug);
	}
#endif

	// トレンドの描画
	MutexLock();

	for (int i = 0; i < MAX_TDATA; ++i)
	{
		m_nQueuLastTotalSize[i] = 0;	// 直近のキュートータルサイズクリア
		m_PPNumber[i] = 0;				// 圧縮位置クリア
		m_PPTotalCount[i] = 0;			// 圧縮合計数クリア
		m_TrendDataPP[i].spaceData.clear();		// 圧縮リストクリア
		m_TrendDataPP[i].spaceData.shrink_to_fit();	// 圧縮リストのメモリクリア
		m_bIsDrawOn[i] = TRUE;				// 描画ON

		m_curVal[i] = 0.0;
	}

	m_bFitYohochi = FALSE;				// 予報値に合わせる
	m_bStopDraw = FALSE;				// 描画停止

	MutexUnlock();
}


/*============================================================================*/
/*! CDataCollection

-# 圧縮位置の要素カウント値とその前の要素のカウント値を比較し、最小値最大値を計算する

@param	トレンド圧縮データキュー
@param	キューの位置
@param	最小値最大値
@retval

*/
/*============================================================================*/
void CGraphWindow::CalcPPMinMaxWithCompareCount(deque<stSpaceData>& trendPP, __int64 number, structPPMinMax& stPPMinMax)
{
	// countが満たされたか
	if (number == 0 || number > 0 && trendPP[number].ppPair.count == trendPP[number - 1].ppPair.count)
	{
		// 最小最大を計算
		CalcPPMinMax(trendPP[number], stPPMinMax);
	}
}


/*============================================================================*/
/*! CDataCollection

-# 最小値最大値を計算する

@param	衛星データ
@param	最小値最大値
@retval	なし

*/
/*============================================================================*/
void CGraphWindow::CalcPPMinMax(stSpaceData& sData, structPPMinMax& stPPMinMax)
{
// test >>>
//	if (m_nPropertiesID == eTrendWndType_RNG)
//	{
//		int c = 0;
//	}
// test <<<


	// 表示可能状態
	if ((char)sData.status[0] >= 0)
	{
		// 最大値、最小値更新
		if (sData.ppPair.max > stPPMinMax.stLine->nMaxValue)
		{
			stPPMinMax.stLine->nMaxValue = sData.ppPair.max;
			if (stPPMinMax.stLine->nVScaleKind == LScale &&
				stPPMinMax.stLine->nMaxValue > m_dTotalScaleLMax)
			{
				m_dTotalScaleLMax = stPPMinMax.stLine->nMaxValue;
				if (m_nStvScale.Auto == TRUE)
				{
					stPPMinMax.max = m_dTotalScaleLMax;
				}
			}
		}
		if (sData.ppPair.min < stPPMinMax.stLine->nMinValue)
		{
			stPPMinMax.stLine->nMinValue = sData.ppPair.min;
			if (stPPMinMax.stLine->nVScaleKind == LScale &&
				stPPMinMax.stLine->nMinValue < m_dTotalScaleLMin)
			{
				m_dTotalScaleLMin = stPPMinMax.stLine->nMinValue;
				if (m_nStvScale.Auto == TRUE)
				{
					stPPMinMax.min = m_dTotalScaleLMin;
				}
			}
		}
	}
}


/*============================================================================*/
/*! CDataCollection

-# PP圧縮データの最大サイズを返す

@param	なし
@retval	PP圧縮データの最大サイズ

*/
/*============================================================================*/
__int64 CGraphWindow::GetPPDataMaxSize()
{
	__int64 size = 0;

	// 存在するもっともサイズが大きい圧縮データを探す
	for (int idx = 0; idx < MAX_TDATA; ++idx)
	{
		if ((__int64)m_TrendDataPP[idx].spaceData.size() > size)
		{
			size = m_TrendDataPP[idx].spaceData.size();
		}
	}

	return size;
}


/*============================================================================*/
/*! CDataCollection

-# 監視データを追加し、縦軸値と監視データの最大最小値を求める

@param	なし
@retval なし

*/
/*============================================================================*/
void CGraphWindow::AddSatelliteData()
{
	DRAWLINE::iterator itr;
	deque<CSpaceData>::iterator trenditr;

	UINT nID;

	__int64 size;
	BOOL bSetFirstTime = TRUE;

	MutexLock();
	CGraphMutex::Lock(eData);

	// 監視データリストの取得
	CSpaceData*	spaceDataList = theApp.GetDataCollection().GetSpaceData(m_StationIndex);
	CSpaceData*	sData;

	time_t mostNewTime = m_MostNewTime;

	// プロパティの設定データ数分をループ
	for (itr = m_DrawLines.begin(); itr != m_DrawLines.end(); itr++)
	{
		// データ名が無い場合は表示しない
		if ((*itr).second.strName.IsEmpty() == TRUE)
		{
			continue;
		}

		// データインデックスがない場合は表示しない
		int nListIdx = (int)((*itr).second.nListIdx);
		if (nListIdx < 0)
		{
			continue;
		}

		nID = (*itr).first;
		structLine data = (*itr).second;
		// 非表示設定ならば表示しない
		if (data.bIsVisible == FALSE)
		{
			continue;
		}

		sData = NULL;
		size = 0;

		sData = &spaceDataList[nListIdx];

		// データが見つからない場合
		if (sData == NULL)
		{
			continue;
		}

		// データサイズの取得
		size = (__int64)sData->value.size();
		if (size == 0)
		{
			continue;
		}

//		double max, min, curMax, curMin;
		double max, min;
		// 軸の最大最小
		// 左縦軸
		if ((*itr).second.nVScaleKind == LScale)
		{
			// 自動スケール時の最大最小
			if (m_nStvScale.Auto == TRUE)
			{
				max = m_dTotalScaleLMax;
				min = m_dTotalScaleLMin;
//				double range = max - min;
//				
//				if (range == 0.0)
//					range = fabs(max);
//				if (range == 0.0)
//					range = 100.0;
//				curMax = Ceil3Keta(max + range * (double)AUTO_V_TOPBOTMARGIN);
//				curMin = Ceil3Keta(min - range * (double)AUTO_V_TOPBOTMARGIN);
			}
			// 固定スケール時の最大最小
			else
			{
				max = m_nStvScale.VScalLMax;
				min = m_nStvScale.VScalLMin;
//				curMax = max;
//				curMin = min;
			}
		}
		// 右縦軸
		else
		{
			// 固定スケール時の最大最小
			max = m_nStvScale.VScalRMax;
			min = m_nStvScale.VScalRMin;
//			curMax = max;
//			curMin = min;
		}

// test >>>
//		if (m_nPropertiesID == eTrendWndType_EL)
//		{
//			int c = 0;
//		}
// test <<<


		// 今回の受信増分の位置を調べる
		__int64 pos;
		// 今までのデータサイズから前回のキューサイズを引いた、新しく受信したデータサイズが圧縮サイズに満たない場合、
		// 新しく受信した生データで最大最小を調べる。
		// それ以外の場合はデータを圧縮破棄しているため、圧縮データの始めから終わりまでで最大最小を調べる。
		if (size >= (sData->totalSize - m_nQueuLastTotalSize[nID]))
		{
			pos = size - (sData->totalSize - m_nQueuLastTotalSize[nID]);
		}
		else
		{
			pos = 0;
		}

#if 0
//		// 横軸が自動スケールでない場合
//		if (m_nTrandHScale != autoscale)
		{
			// 新規受信データが横軸描画範囲より大きい場合
			if (size - pos > m_dbHzRange && sData->srcType == eSigSrcType_EngExchange)
			{
				// 描画範囲以上のデータは不要のため、データの後ろから描画範囲を調べなおす
//				pos = sData->totalSize - (__int64)m_dbHzRange;
				pos = size - (__int64)m_dbHzRange;
				time_t search_t1 = CTime(sData->value[pos].systime).GetTime();
				time_t search_t2;
				// うるう対策のため、前の時刻に同じ時刻が無いか調べる
				while (pos > 0)
				{
					search_t2 = CTime(sData->value[pos - 1].systime).GetTime();
					if (search_t2 < search_t1)
					{
						break;
					}
					pos--;
				}
			}
		}
#else
		// 新規受信データの不要データ検索
		if (sData->srcType == eSigSrcType_EngExchange && pos < size && size > 0)
		{
			time_t	rcv_startpos_t = CTime(sData->value[pos].systime).GetTime();
			time_t	rcv_endpos_t = CTime(sData->value[size - 1].systime).GetTime();
			time_t	rcv_range_t = rcv_endpos_t - rcv_startpos_t;
			time_t	rcv_halfpos_t = rcv_range_t / 2 + rcv_startpos_t;
			time_t	search_t;
//			time_t	start_t = m_EndTime - (time_t)m_dbHzRange;
//			time_t	start_t = m_MostNewTime - (time_t)(m_dbHzRange * 0.9);
			time_t	start_t;
			if (mostNewTime > 0)
			{
				start_t = mostNewTime - (time_t)(m_dbHzRange * 0.9);
			}
			else
			{
				start_t = rcv_endpos_t - (time_t)(m_dbHzRange * 0.9);
			}

			if (start_t < 0)
			{
				start_t = 0;
			}

			// 新規受信データの開始が現状のグラフ描画開始時刻以下で、かつ終了が描画開始時刻以上の場合
			if (rcv_startpos_t <= start_t && rcv_endpos_t >= start_t)
			{
				// 新規受信データの真ん中が、現状のグラフ描画開始時刻以上の場合
				if (rcv_halfpos_t >= start_t)
				{
					// 描画範囲以上のデータは不要のため、データを前から描画範囲を調べなおす
					search_t = rcv_startpos_t;
					while (search_t < start_t && pos < size - 1)
					{
						pos++;
						search_t = CTime(sData->value[pos].systime).GetTime();
					}
				}
				// 新規受信データの真ん中が、現状のグラフ描画開始時刻より小さい場合
				else
				{
					// 描画範囲以上のデータは不要のため、データの末尾から描画範囲を調べなおす
					search_t = rcv_endpos_t;
					pos = size - 1;
					while (search_t > start_t && pos > 0)
					{
						pos--;
						search_t = CTime(sData->value[pos].systime).GetTime();
					}

					// うるう監視と短周期監視対策のため、前の時刻に同じ時刻が無いか調べる
					time_t search_t2;
					while (pos > 0)
					{
						search_t2 = CTime(sData->value[pos - 1].systime).GetTime();
						if (search_t2 < search_t)
						{
							break;
						}
						pos--;
					}
				}
			}
		}
#endif

		m_stPPMinMax.max = max;
		m_stPPMinMax.min = min;
//		m_stPPMinMax.curMax = curMax;
//		m_stPPMinMax.curMin = curMin;
		m_stPPMinMax.stLine = &data;

		m_TrendDataPP[nID].srcType = sData->srcType;

//#ifdef _DEBUG
//		{
//			CString str;
//			str.Format(_T("%s(%d) :: Sig[%d], GraphType = %u, ViewID = %u, pos = %I64d, size = %I64d"), __FILEW__, __LINE__, nID, m_nGraphType, m_nPropertiesID, pos, size);
//			CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("AddSatelliteData"), _T("debug"), str, nLogEx::debug);
//		}
//#endif

		while (pos < size)
		{
			if (size > 1 && m_TrendDataPP[nID].spaceData.empty() == FALSE && m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange)
			{
//#ifdef _DEBUG
//				{
//					CString str;
//					str.Format(_T("%s(%d) :: Check delete harf Sig[%d], GraphType = %u, ViewID = %u, pos = %I64d, size = %I64d"), __FILEW__, __LINE__, nID, m_nGraphType, m_nPropertiesID, pos, m_TrendDataPP[nID].spaceData.size());
//					CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("AddSatelliteData"), _T("debug"), str, nLogEx::debug);
//				}
//#endif
				// データサイズがグラフ右端または圧縮境界を超えている場合
				if (m_TrendDataPP[nID].spaceData.size() >= MAX_PPDATA_SIZE)
				{
					// 圧縮データを半分にする
					DeleteHalfPP(nID, m_TrendDataPP[nID].spaceData);
				}
			}

			// 圧縮データ生成
			ConvertPP(nID, m_TrendDataPP[nID].spaceData, sData->value[pos], m_stPPMinMax);

			m_bExitData = TRUE;

			pos++;
		}

		if (!m_TrendDataPP[nID].spaceData.empty())
		{
			// 直前のサイズを控えておく
			m_nQueuLastTotalSize[nID] = sData->totalSize;
			if (m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange)
			{
				// キューの最後の位置を取得
				m_nTrendDataEndPos = (__int64)m_TrendDataPP[nID].spaceData.size();
			}

			// もっとも古い時刻ともっとも新しい時刻
			time_t t_old = CTime(m_TrendDataPP[nID].spaceData[0].systime).GetTime();
			time_t t_new = CTime(m_TrendDataPP[nID].spaceData[m_TrendDataPP[nID].spaceData.size() - 1].systime).GetTime();

//#ifdef _DEBUG
//			{
//				CString str;
//				str.Format(_T("%s(%d) :: Sig(%d), t_old = %I64d, t_new = %I64d, GraphType = %u, ViewID = %u"), __FILEW__, __LINE__, nID, t_old, t_new, m_nGraphType, m_nPropertiesID);
//				CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("AddSatelliteData"), _T("debug"), str, nLogEx::debug);
//			}
//#endif

			// 監視データまたは、予報値に合わせる設定時の予報値の場合、時刻取得
			if (m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange)
			{
				// グラフで最初のデータは無条件で時刻取得
				if (bSetFirstTime)
				{
					bSetFirstTime = FALSE;
					m_MostOldTime = t_old;
					m_MostNewTime = t_new;
				}
				// 最初以外は大小比較
				else
				{
					if (t_old < m_MostOldTime)
					{
						m_MostOldTime = t_old;
					}
					if (t_new > m_MostNewTime)
					{
						m_MostNewTime = t_new;
					}
				}
			}

		}
	}

	CGraphMutex::Unlock(eData);
	MutexUnlock();

	// 予報値ありの設定では、監視データのAOSが開始時刻になる
	m_MostOldTime = SelectStartTime(m_MostOldTime);

//#ifdef _DEBUG
//	{
//		CString str;
//		str.Format(_T("%s(%d) :: m_MostOldTime = %I64d, m_MostNewTime = %I64d, GraphType = %u, ViewID = %u"), __FILEW__, __LINE__, m_MostOldTime, m_MostNewTime, m_nGraphType, m_nPropertiesID);
//		CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("AddSatelliteData"), _T("debug"), str, nLogEx::debug);
//	}
//#endif

}


/*============================================================================*/
/*! CDataCollection

-# グラフ横レンジとPP圧縮データ数を合わせる

@param	t_old：もっとも古いデータ時刻（衛星捕捉グラフ時に使用）
@param	t_new:もっとも新しい時刻（衛星捕捉グラフ時に使用）
@retval	なし

*/
/*============================================================================*/
time_t CGraphWindow::FitHRangeAndPPDataSize(time_t t_old, time_t t_new)
{
	CGraphMutex::Lock(eData);

	time_t		size = 0;
	deque<stSpaceData>*	spaceDataQueu = &m_TrendDataPP[0].spaceData;
	int	idx = 0;

	// 最新時刻と一致するデータを探す
	BOOL bFind = FALSE;
	for (int nSearchIdx = 0; nSearchIdx < MAX_TDATA; ++nSearchIdx)
	{
		size = m_TrendDataPP[nSearchIdx].spaceData.size();
		// 工学値変換後データ
		if (m_TrendDataPP[nSearchIdx].srcType == eSigSrcType_EngExchange && size > 0) 
		{
			if (t_new == CTime(m_TrendDataPP[nSearchIdx].spaceData[size - 1].systime).GetTime())
			{
				idx = nSearchIdx;
				spaceDataQueu = &m_TrendDataPP[nSearchIdx].spaceData;
				bFind = TRUE;
				break;
			}
		}
	}

	// 監視データサイズの初期化
	m_MostOldTime = t_old;
	m_MostNewTime = t_new;

	if (bFind == FALSE)
	{
		CGraphMutex::Unlock(eData);
		return 0;
	}

	size = m_MostNewTime - m_MostOldTime;

	if (size > 0)
	{
		// 圧縮キューが空でない場合
		if (spaceDataQueu->empty() == FALSE && m_TrendDataPP[idx].srcType == eSigSrcType_EngExchange)
		{
			// 圧縮なしで、かつ右縦軸に到達した場合
			if (size >= m_dbHzRange)
			{
				// 古いデータの10%を削除する位置に更新
				if (theApp.GetTrendScrollMode(m_nViewNo) == false)
				{
					if (m_bFitYohochi)
					{
						m_bStopDraw = TRUE;
						CGraphMutex::Unlock(eData);
						return m_MostOldTime;
					}

					time_t mostOldTime = m_MostOldTime;
					mostOldTime = mostOldTime + size - (__int64)(m_dbHzRange * 0.9);
					// データに記録されている時刻を探す
					__int64 searchPos = 0;
					__int64 lastPos = (__int64)(*spaceDataQueu).size() - 1;
					time_t searchTime = CTime((*spaceDataQueu)[searchPos].systime).GetTime();
					while (TRUE)
					{
						if (searchTime == mostOldTime)
						{
							m_MostOldTime = searchTime;
							break;
						}

						if (searchPos >= lastPos)
						{
							m_MostOldTime = mostOldTime;
							break;
						}

						searchPos++;
						searchTime = CTime((*spaceDataQueu)[searchPos].systime).GetTime();
					}

					m_nTrendDataPos = 0;
#ifdef _DEBUG
					{
						CString	str;
						str.Format(_T("delete 90per : m_MostOldTime = %I64d, size = %I64d, GraphType = %u, ViewID = %u, m_nTrendDataPos = %I64d"), m_MostOldTime, size, m_nGraphType, m_nPropertiesID, m_nTrendDataPos);
						CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("FitHRangeAndPPDataSize"), _T("debug"), str, nLogEx::debug);
					}
#endif
//					if (m_nGraphType != eGraphType_Summary && m_bFitYohochi == TRUE)
//					{
//						m_bFitYohochi = FALSE;
//					}
				}
			}
		}
		// データが無い場合
		else
		{
			CGraphMutex::Unlock(eData);
			return 0;
		}
	}

	CGraphMutex::Unlock(eData);

	return m_MostOldTime;
}


/*============================================================================*/
/*! CDataCollection

-# 開始時刻より古いデータを削除する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CGraphWindow::DeletePPData()
{
	time_t cur_t;
	time_t aos_t = (time_t)theApp.GetSatelliteData().m_trendData.m_gst_dspinf.d_AOS_time;
	time_t los_t = (time_t)theApp.GetSatelliteData().m_trendData.m_gst_dspinf.d_LOS_time;
	for (int i = 0; i < MAX_TDATA; i++)
	{
		if (m_TrendDataPP[i].spaceData.empty())
			continue;

		while (m_TrendDataPP[i].spaceData.empty() == false)
		{
			// 次の時刻
			cur_t = CTime(m_TrendDataPP[i].spaceData[0].systime).GetTime();
			// 予報値の場合
			if (m_TrendDataPP[i].srcType == eSigSrcType_File && theApp.GetSatelliteData().m_trendData.m_gst_dspinf.i_yohouti_umu == YOHOUTI_ARI)
			{
				if (cur_t >= aos_t && m_StartTime <= los_t)
				{
					break;
				}
			}
			// 予報値でない場合
			else
			{
				if (cur_t >= m_StartTime)
				{
					break;
				}
			}

//#ifdef _DEBUG
//			{
//				CString str;
//				str.Format(_T("%s(%d) :: Delete Sig[%d], time = %I64d, GraphType = %u, ViewID = %u"), __FILEW__, __LINE__, i, cur_t, m_nGraphType, m_nPropertiesID);
//				CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("DeletePPData"), _T("debug"), str, nLogEx::debug);
//			}
//#endif
			// 圧縮データの先頭を削除
			DeleteHeadPP(i, m_TrendDataPP[i].spaceData, m_TrendDataPP[i].srcType);
		}
	}
}


/*============================================================================*/
/*! CDataCollection

-# 衛星捕捉グラフ（予報値あり・なし）／サマリグラフに合わせた開始時刻を返す

@param	t：指定時刻
@retval	開始時刻

*/
/*============================================================================*/
time_t CGraphWindow::SelectStartTime(time_t t)
{
	time_t		start_t;
	BOOL		bFitYohoch = FALSE;

	// 衛星捕捉グラフの場合
	if (m_nGraphType != eGraphType_Summary)
	{
		start_t = t;

		// 横を予報値に合わせる場合
		if (m_nTrandHScale == yohochiscale)
		{
			if (theApp.GetSatelliteData().m_trendData.m_gst_dspinf.i_yohouti_umu == YOHOUTI_ARI)
			{
				// 予報値ありの設定では、監視データのAOSが開始時刻になる
//				if (start_t <= (time_t)theApp.GetSatelliteData().m_trendData.m_gst_dspinf.d_LOS_time)
				{
					start_t = (time_t)theApp.GetSatelliteData().m_trendData.m_gst_dspinf.d_AOS_time;
				}
				bFitYohoch = TRUE;

				if (start_t > (time_t)theApp.GetSatelliteData().m_trendData.m_gst_dspinf.d_LOS_time)
					m_bStopDraw = TRUE;
			}
		}
	}
	// サマリグラフの場合
	else
	{
		start_t = m_MostOldTime;
	}

	m_bFitYohochi = bFitYohoch;

	return start_t;
}


/*============================================================================*/
/*! グラフ描画

-# 設定ボタンクリック時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSettingBtn()
{
#ifdef _DEBUG
	{
		CString str;
		str.Format(_T("%s(%d) :: Call Setting Btn, GraphType = %u, ViewID = %u"), __FILEW__, __LINE__, m_nGraphType, m_nPropertiesID);
		CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("OnBnClickedSettingBtn"), _T("debug"), str, nLogEx::debug);
	}
#endif

	if (m_nGraphType != eGraphType_Summary)
	{
		CTrendGraphView* pView = (CTrendGraphView*)GetParent();
		if (pView == NULL)
			return;

		if (m_btnSetting.GetSafeHwnd() == NULL)
			return;

		// トレンドプロパティダイアログを開く通知
		theApp.m_pMainWnd->PostMessage(eMessage_OpenTrendSetDlg, (WPARAM)m_nPropertiesID, (LPARAM)GetParent());
	}
}


/*============================================================================*/
/*! グラフ描画

-# 横罫線表示ONOFF切り替え時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedHGridOnBtn()
{
	if (m_btnHGridChk.GetSafeHwnd() == NULL)
		return;

	m_bHGridOn = m_btnHGridChk.GetCheck();

	// 再描画
	Invalidate();
}


/*============================================================================*/
/*! グラフ描画

-# 信号表示ONOFF切り替え時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CGraphWindow::BnClickedSigOnBtn(UINT nSig)
{
	if (m_nGraphType != eGraphType_Summary && m_StationIndex != eStation_Usuda642)
	{
		if (m_chkBtnViewSig[nSig].GetSafeHwnd() == NULL)
			return;

		CPropertiesData& prop = theApp.GetSatelliteData().GetTrendPropertiesData(m_nPropertiesID);
		if (m_chkBtnViewSig[nSig].GetCheck())
			prop.isViewMode[nSig] = true;
		else
			prop.isViewMode[nSig] = false;

		theApp.GetSatelliteData().SetTrendPropertiesData(m_nPropertiesID, prop);
		// 変更を通知
		theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 0, (LPARAM)m_nViewID);
	}
}

/*============================================================================*/
/*! グラフ描画

-# 信号表示ONOFF切り替え時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn0()
{
	BnClickedSigOnBtn(0);
}

/*============================================================================*/
/*! グラフ描画

-# 信号表示ONOFF切り替え時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn1()
{
	BnClickedSigOnBtn(1);
}

/*============================================================================*/
/*! グラフ描画

-# 信号表示ONOFF切り替え時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn2()
{
	BnClickedSigOnBtn(2);
}

/*============================================================================*/
/*! グラフ描画

-# 信号表示ONOFF切り替え時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn3()
{
	BnClickedSigOnBtn(3);
}


/*============================================================================*/
/*! グラフ描画

-# 信号表示ONOFF切り替え時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn4()
{
	BnClickedSigOnBtn(4);
}


/*============================================================================*/
/*! グラフ描画

-# 信号表示ONOFF切り替え時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn5()
{
	BnClickedSigOnBtn(5);
}


/*============================================================================*/
/*! グラフ描画

-# 信号表示ONOFF切り替え時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn6()
{
	BnClickedSigOnBtn(6);
}


/*============================================================================*/
/*! グラフ描画

-# 信号表示ONOFF切り替え時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn7()
{
	BnClickedSigOnBtn(7);
}


/*============================================================================*/
/*! グラフ描画

-# 信号表示ONOFF切り替え時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn8()
{
	BnClickedSigOnBtn(8);
}


/*============================================================================*/
/*! グラフ描画

-# 信号表示ONOFF切り替え時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn9()
{
	BnClickedSigOnBtn(9);
}


/*============================================================================*/
/*! グラフ描画

-# 信号表示ONOFF切り替え時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn10()
{
	BnClickedSigOnBtn(10);
}


/*============================================================================*/
/*! グラフ描画

-# 信号表示ONOFF切り替え時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn11()
{
	BnClickedSigOnBtn(11);
}


/*============================================================================*/
/*! グラフ描画

-# 信号表示ONOFF切り替え時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn12()
{
	BnClickedSigOnBtn(12);
}


/*============================================================================*/
/*! グラフ描画

-# 積分時間設定ボタンクリック時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedIntgTimeBtn()
{
	CTrendGraphView* pView = (CTrendGraphView*)GetParent();
	if (pView == NULL)
		return;

	if (m_btnSetting.GetSafeHwnd() == NULL)
		return;

	CIntgTimeDlg dlg;
	dlg.SetIntgTime(m_nIntgTime);
	if (dlg.DoModal() == IDOK)
	{
		// 積分設定ボタンの表示更新
//		m_nIntgTime = dlg.GetIntgTime();
		UpdateIntegKanshi();
	}
}


/*============================================================================*/
/*! グラフ描画

-# 詳細表示する領域があるか調べる

@param  rect：領域
@retval TRUE：領域がある / FALSE：領域がない
*/
/*============================================================================*/
BOOL CGraphWindow::IsViewDetail(CRect& rect)
{
	if (rect.Width() > mDrawDetailWidth && rect.Height() > mDrawDetailHeight)
	{
		return TRUE;
	}

	return FALSE;
}


/*============================================================================*/
/*! グラフ描画

-# 最大値最小値を更新する

@param  なし
@retval なし
*/
/*============================================================================*/
void CGraphWindow::UpdateMaxMin()
{
	UINT nID;
	DRAWLINE::iterator itr;
	deque<CSpaceData>::iterator trenditr;

	// 最大値最小値をクリア
	m_dTotalScaleLMax = -DBL_MAX;
	m_dTotalScaleLMin = DBL_MAX;

	for (itr = m_DrawLines.begin(); itr != m_DrawLines.end(); itr++)
	{
		// データ名が無い場合は表示しない
		if ((*itr).second.strName.IsEmpty() == TRUE)
		{
			continue;
		}

		// データインデックスがない場合は表示しない
		int nListIdx = (int)((*itr).second.nListIdx);
		if (nListIdx < 0)
		{
			continue;
		}

		nID = (*itr).first;

		if (m_TrendDataPP[nID].spaceData.size() > 0)
		{
			CalcPPMinMaxWithCompareCount(m_TrendDataPP[nID].spaceData, nID, m_stPPMinMax);
		}
	}
}
