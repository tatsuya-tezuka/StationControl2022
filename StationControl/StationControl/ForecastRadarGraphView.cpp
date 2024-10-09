/*============================================================================*/
/*! ForecastRadarGraphView.cpp

-# 予報値一覧画面　レーダーグラフ
*/
/*============================================================================*/
// ForecastRadarGraphView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ForecastRadarGraphView.h"


//----------------------------------------------------------------------
// 定義

// 時間
#define  INTERVAL_NEW	0   						// 初期(再）表示開始時間（ミリ秒)
//#define  INTERVAL_REAL	6000						// トレンド 表示開始時間（ミリ秒）
#define  INTERVAL_REAL	500							// トレンド 表示開始時間（ミリ秒）

// 表示色
#define  GRPH_CLR    	RGB(236, 233, 216)  		// グラフ領域背景色
#define  FREM_CLR    	RGB(  0,   0,   0)   		// グラフ枠表示色
#define  FREM_CLR2		RGB(255, 255, 255)			// グラフ枠表示色2
#define  E80_CLR    	RGB(255, 215,   0)   		// グラフ枠表示色
#define  GRID_CLR    	RGB(  0,   0,   0)			// グラフ罫線表示色	
#define  GRID_CLR2    	RGB(128, 128, 128)			// グラフ罫線表示色2	
#define  STRTIT_CLR  	RGB(  0,   0,   0)			// タイトル文字表示色
#define  STRCON_CLR  	RGB(  0,   0,   0)			// 固定項目文字表示色
#define  STRVAR_CLR  	RGB(  0,   0,   0)			// 可変項目文字表示色
#define  YOHO_AOS_CLR	RGB(  0,  11, 255)			// グラフ描画色(予報値AOS)
#define  YOHO_LOS_CLR	RGB(  0,   0, 255)			// グラフ描画色(予報値LOS)
#define  YOHO_CLR		RGB(255, 165,   0)			// グラフ描画色(予報値)
#define  TRND_CLR		RGB(  0, 128,   0)			// グラフ描画色(実測値)	
#define  SKYL_CLR		RGB(144, 238, 144)			// グラフ描画色(スカイライン)
//#define  RFTR_CLR		RGB(255, 192, 203)			// グラフ描画色(ＲＦトリップ)
#define  RFTR_CLR		RGB(255,   0,   0)			// グラフ描画色(ＲＦトリップ)
#define  OVER_CLR		RGB(255,   0,   0)			// グラフ描画色(範囲オーバー)

// 線の太さ
#define  LINE_WID_FRAM_BOLD   2						// グラフ枠の線の太さ(太線）
#define  LINE_WID_FRAM_BOLD2  3						// グラフ枠の線の太さ(太線2）
#define  LINE_WID_FRAM   1							// グラフ枠の線の太さ(細線）
#define  LINE_WID_GRID   1							// グリッド罫線の太さ

// ラジアン
#define	RADCV	(3.1415926538/180.0)

// 文字マージン
#define TEXT_MARGIN				16		// テキストマージン
#define ANGLE_MARGIN			28		// 角度マージン
#define ANGLE_MARGIN_4STATION	14		// 角度マージン

// 凡例
#define EXAMPLE_AREA_WIDTH		50		// 凡例テキストエリア幅（１局モード）
#define EXAMPLE_STATIONALL_AREA_WIDTH		150		// 凡例テキストエリア幅（全局モード）
#define EXAMPLE_LINE_WIDTH		8		// 凡例ライン幅
#define EXAMPLE_RIGHT_LINE_POS	50		// 右ライン位置
#define EXAMPLE_AREA_WIDTH2		100			// 凡例テキストエリア幅2（１局モード）
#define EXAMPLE_AREA_WIDTH3		70			// 凡例テキストエリア幅3（計画一覧画面）
#define EXAMPLE_AREA_WIDTH4		100			// 凡例テキストエリア幅4（予報値一覧画面）

#define EXAMPLE_STATIONALL_LEFT_MARGIN		10		// 凡例テキストエリア左マージン（全局モード）
#define EXAMPLE_STATIONALL_TOP_MARGIN		5		// 凡例テキストエリア上マージン（全局モード）
#define RECT_STATIONALL_HEIGHT				50		// 領域エリア上位置（全局モード）
#define RECT_STATION_HEIGHT					40		// 領域エリア上位置（１局モード）
#define RECT_STATION_HEIGHT2				20		// 領域エリア上位置（一覧画面）

// 角度文字 表示最小サイズ設定
#define VALUES_AREA_WIDTH					200		// 値の表示エリアの横サイズ
#define VALUES_AREA_HEGIHT					120		// 値の表示エリアの縦サイズ
#define ANGLEVIEW_MINIMUM_SZ				0		// これより小さい場合は、度数文字を非表示とする
#define EXAMPLEVIEW_MINIMUM_SZ				180		// これより小さい場合は、凡例を非表示とする
#define DIRVIEW_MINIMUM_SZ					140		// これより小さい場合は、方位を非表示とする

// 値の表の位置設定
#define VALUES_AREA_HMARGIN					4		// 値の表の横方向マージン
#define VALUES_AREA_VMARGIN					4		// 値の表の縦方向マージン
#define VALUES_AREA_ROW_H					20		// 値の表の行の高さマージン
#define VALUES_AREA_FORECAST_W				0.2		// 値の表の予報値の幅
#define VALUES_AREA_TIME_W					0.4		// 値の表の予報値の幅
#define VALUES_AREA_AZ_W					0.2		// 値の表の予報値の幅
#define VALUES_AREA_EL_W					0.2		// 値の表の予報値の幅

// データ定義
#define IGNORE_DATA		9999999999999.0		/* 無効データ（クリア用） */


// CForecastRadarGraphView

IMPLEMENT_DYNCREATE(CForecastRadarGraphView, CView)

/*============================================================================*/
/*! CForecastRadarGraphView

-# コンストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CForecastRadarGraphView::CForecastRadarGraphView() 
: m_i_plot_flag(CTrackingRadarCtrlMonData::eOFF)
, m_nViewType(eRadarViewType_Station)
, m_nStationID(eStation_Usuda64)
, m_strPlanID(_T(""))
, m_tPassStart(0)
, m_tPassEnd(0)
, m_dummyCnt(0)
{
	CFont defFont;
	CFont* oldFont = NULL;
	defFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (defFont.GetLogFont(&lf)){
		lf.lfWeight = FW_NORMAL;
		lf.lfHeight = 12;
		m_txtFontNormal.CreateFontIndirect(&lf);

		lf.lfHeight = -8;
		m_txtFontSmall.CreateFontIndirect(&lf);
	}
}

/*============================================================================*/
/*! CForecastRadarGraphView

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CForecastRadarGraphView::~CForecastRadarGraphView()
{
	m_txtFontSmall.DeleteObject();
	m_txtFontNormal.DeleteObject();
}

BEGIN_MESSAGE_MAP(CForecastRadarGraphView, CView)
	ON_WM_MOUSEACTIVATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CForecastRadarGraphView 描画

/*============================================================================*/
/*! CForecastRadarGraphView

-# 描画時に呼ばれる

@param  pDC：ドキュメントのデバイスコンテキストポインタ
@retval なし
*/
/*============================================================================*/
void CForecastRadarGraphView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 描画コードをここに追加してください。
}


// CForecastRadarGraphView 診断

#ifdef _DEBUG
void CForecastRadarGraphView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CForecastRadarGraphView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForecastRadarGraphView メッセージ ハンドラー


/*============================================================================*/
/*! CForecastRadarGraphView

-# 非アクティブ ウィンドウにユーザーがマウス ボタンを押したときに呼び出される

@param  pDesktopWnd：アクティブになるウィンドウのトップレベルの親ウィンドウへのポインタ
@param	nHitTest：カーソル位置テストの領域コード
@param	message：マウスメッセージ番号
@retval ウィンドウアクティブ/破棄結果
*/
/*============================================================================*/
int CForecastRadarGraphView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	return 0;
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# 描画時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastRadarGraphView::OnPaint()
{
	CGraphMutex::Lock(eRadar);

	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		CGraphMutex::Unlock(eRadar);
		return;
	}

	CRect rectClient;
	CPaintDC dc(this); // device context for painting

	// クライアント領域の取得
	GetClientRect(rectClient);

	CDC memDC;
	CBitmap bitm;
	// 描画DCの作成
	memDC.CreateCompatibleDC(&dc);

	// 描画用ビットマップの作成
	bitm.CreateCompatibleBitmap(&dc, rectClient.Width(), rectClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitm);

	// バックグラウンド モードを設定
	memDC.SetBkMode(OPAQUE);
	COLORREF colBk = GetSysColor(COLOR_3DFACE);
	memDC.SetBkColor(colBk);

	CFont* oldFont = NULL;
	if (m_nViewType == eRadarViewType_Station && theApp.GetSelectStationMode() != eStation_1)
	{
		oldFont = memDC.SelectObject(&m_txtFontSmall);
	}
	else
	{
		oldFont = memDC.SelectObject(&m_txtFontNormal);
	}

	CBrush brush(colBk);
	// 全体を塗りつぶし
	memDC.FillRect(rectClient, &brush);
	// デバイス コンテキストのビューポートの原点を設定
	memDC.SetViewportOrg(0, 0);

	// レーダー枠描画
	DrawRadarFrame(memDC, rectClient, eDrawType_Before);
	// スカイライン描画
	DrawSkyline(memDC, rectClient);

	// RFトリップはサマリ画面のみ表示
	if (m_nViewType == eRadarViewType_Station)
	{
		// RFトリップ描画
		DrawRfTrip(memDC, rectClient);
	}
	// 予報値描画
	DrawForecast(memDC, rectClient);

	// 追尾状況グラフ保存データ描画処理(予報値一覧は描画しない）
	if (m_nViewType != eRadarViewType_ForecastList &&
		pCtrlMonData->m_gst_dspinf.i_unyou_mode == CTrackingRadarCtrlMonData::eUNYOU_IN) {
		DrawTrakingSaveData(memDC, rectClient);
	}

	// レーダー枠描画
	DrawRadarFrame(memDC, rectClient, eDrawType_After);

	CGraphMutex::Unlock(eRadar);

	dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	bitm.DeleteObject();
	memDC.SelectObject(oldFont);

	memDC.DeleteDC();

	memDC.DeleteDC();
	brush.DeleteObject();
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# レーダーフレーム描画

@param	memDC		ドキュメント
@param	rectClient	描画領域
@retval	TRUE:グリッド線を描画した / FALSE:グリッド線を描画しなかった

*/
/*============================================================================*/
BOOL CForecastRadarGraphView::DrawRadarFrame(CDC& memDC, CRect& rectClient, int drawType)
{
	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		return FALSE;
	}

	CPen penSolidBold;
	if (!penSolidBold.CreatePen(PS_SOLID, LINE_WID_FRAM_BOLD, pCtrlMonData->m_gst_dspinf.frem_color))
		return FALSE;
	CPen penSolidBold2;
	if (!penSolidBold2.CreatePen(PS_SOLID, LINE_WID_FRAM_BOLD2, pCtrlMonData->m_gst_dspinf.frem_color))
		return FALSE;
	CPen penSolid;
	if (!penSolid.CreatePen(PS_SOLID, LINE_WID_FRAM_BOLD, pCtrlMonData->m_gst_dspinf.frem_color2))
		return FALSE;
	CPen* oldPen = memDC.SelectObject(&penSolid);
	CBrush brushBk;
	brushBk.CreateSolidBrush(pCtrlMonData->m_gst_dspinf.grph_color);
	CBrush brush80;
	brush80.CreateSolidBrush(pCtrlMonData->m_gst_dspinf.e80_frem_color);
	CBrush* oldBrush = (CBrush*)memDC.SelectObject(&brushBk);

	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(pCtrlMonData->m_gst_dspinf.strcon_color);

	CRect rcCircle = rectClient;
	CRect rcValues;
	BOOL bViewValues = FALSE;
	BOOL bViewExample = FALSE;
	BOOL bViewStationModeAll = FALSE;

	// 1局サマリの場合
//	if (theApp.GetSelectStationMode() == eStation_1 && rcCircle.Width() >= VALUES_AREA_WIDTH && rcCircle.Height() >= EXAMPLEVIEW_MINIMUM_SZ + VALUES_AREA_HEGIHT)
	if (theApp.GetSelectStationMode() == eStation_1 && m_nViewType == eRadarViewType_Station)
	{
		bViewValues = TRUE;
		rcCircle.bottom -= VALUES_AREA_HEGIHT;
		rcValues = rectClient;
		rcValues.top = rcCircle.bottom;
	}

	// 局サマリの場合
	if (m_nViewType == eRadarViewType_Station && theApp.GetSelectStationMode() != eStation_1)
	{
		rcCircle.bottom -= RECT_STATIONALL_HEIGHT;
		bViewStationModeAll = TRUE;
	}

	if (bViewStationModeAll || rcCircle.Width() >= EXAMPLEVIEW_MINIMUM_SZ && rcCircle.Height() >= EXAMPLEVIEW_MINIMUM_SZ)
	{
		bViewExample = TRUE;
		// 1局モードまたは一覧画面
		if (!bViewStationModeAll)
		{
			if (m_nViewType >= eRadarViewType_ForecastList)
			{
				// 縦上方向をつめる
				rcCircle.bottom -= RECT_STATION_HEIGHT2;
			}
			else
			{
				// 縦上方向をつめる
				rcCircle.bottom -= RECT_STATION_HEIGHT;
			}
		}
		// 正方形にする
		if (rcCircle.Width() > rcCircle.Height())
		{
			// 横が縦よりも大きい場合は、横を縦にあわせる
			int sz = (rcCircle.Width() - rcCircle.Height()) / 2;
			rcCircle.left += sz;
			rcCircle.right -= sz;
		}
		else
		{
			// 縦が横よりも大きい場合は、縦を横にあわせる
			int sz = (rcCircle.Height() - rcCircle.Width()) / 2;
			rcCircle.top += sz;
			rcCircle.bottom -= sz;
		}
	}

	CRect rc = rcCircle;

	// 円グリッドの描画
	int len = __min(rc.Width(), rc.Height());
	rc.right = rc.left + len;
	rc.bottom = rc.top + len;
	rc.left = rc.left + ((rcCircle.Width() - rc.Width()) / 2);
	rc.top = rc.top + ((rcCircle.Height() - rc.Height()) / 2);
	rc.right = rc.left + len;
	rc.bottom = rc.top + len;

	rc.DeflateRect(1, 1);
	CRect textrect = rc;

	if (rcCircle.Width() >= ANGLEVIEW_MINIMUM_SZ && rcCircle.Height() >= ANGLEVIEW_MINIMUM_SZ)
	{
		if (!bViewStationModeAll)
		{
			rc.DeflateRect(ANGLE_MARGIN, ANGLE_MARGIN);
		}
		else
		{
			rc.DeflateRect(ANGLE_MARGIN_4STATION, ANGLE_MARGIN_4STATION);
		}
	}

	if (drawType == eDrawType_Before)
	{
		memDC.Ellipse(rc);																									// 0度位置塗りつぶし
	}
	else
	{
		memDC.SelectObject(&penSolidBold);
		(CBrush*)memDC.SelectStockObject(NULL_BRUSH);
		memDC.Ellipse(rc);																									// 0度位置
	}
	
	memDC.SelectObject(&penSolid);

	if (drawType == eDrawType_After)
	{
		if (rcCircle.Width() >= ANGLEVIEW_MINIMUM_SZ && rcCircle.Height() >= ANGLEVIEW_MINIMUM_SZ)
		{
			memDC.TextOutW(rc.right, rc.top + (rc.bottom - rc.top) / 2 + 5, _T("0°"));
		}
	}

	CRect linerect = rc;
	rc.InflateRect(-((int)(linerect.Width() / 2.0 * (1.0 / 3.0))), -((int)(linerect.Height() / 2.0 * (1.0 / 3.0))));
	if (drawType == eDrawType_Before)
	{
		memDC.Ellipse(rc);																									// 30度位置
	}
	
	if (drawType == eDrawType_After)
	{
		if (rcCircle.Width() >= ANGLEVIEW_MINIMUM_SZ && rcCircle.Height() >= ANGLEVIEW_MINIMUM_SZ)
		{
			memDC.TextOutW(rc.right, rc.top + (rc.bottom - rc.top) / 2 + 5, _T("30°"));
		}
	}

	rc.InflateRect(-((int)(linerect.Width() / 2.0 * (1.0 / 3.0))), -((int)(linerect.Height() / 2.0 * (1.0 / 3.0))));
	if (drawType == eDrawType_Before)
	{
		memDC.Ellipse(rc);																									// 60度位置
	}
	
	if (drawType == eDrawType_After)
	{
		if (rcCircle.Width() >= ANGLEVIEW_MINIMUM_SZ && rcCircle.Height() >= ANGLEVIEW_MINIMUM_SZ)
		{
			memDC.TextOutW(rc.right, rc.top + (rc.bottom - rc.top) / 2 + 5, _T("60°"));
		}
	}

	rc = linerect;
	rc.InflateRect((int)(linerect.Width() / 2.0 / 9.0 - linerect.Width() / 2.0), (int)(linerect.Height() / 2.0 / 9.0 - linerect.Height() / 2.0));

	if (drawType == eDrawType_Before)
	{
		(CBrush*)memDC.SelectObject(&brush80);
		memDC.Ellipse(rc);																									// 80度位置																								;			
		(CBrush*)memDC.SelectStockObject(NULL_BRUSH);
		memDC.Ellipse(rc);																								;			
	}

	if (drawType == eDrawType_After)
	{
		if (rcCircle.Width() >= ANGLEVIEW_MINIMUM_SZ && rcCircle.Height() >= ANGLEVIEW_MINIMUM_SZ)
		{
			if (!bViewStationModeAll)
			{
				memDC.TextOutW(rc.right, rc.top + (rc.bottom - rc.top) / 2 + 5, _T("80°"));
			}
			else
			{
				memDC.TextOutW(rc.right - 5, rc.top + (rc.bottom - rc.top) / 2 + 5, _T("80°"));
			}
		}
	}

	CPen penLine;
	if (!penLine.CreatePen(PS_SOLID, LINE_WID_GRID, pCtrlMonData->m_gst_dspinf.grid_color))
		return FALSE;
	CPen penLine2;
	if (!penLine2.CreatePen(PS_SOLID, LINE_WID_GRID, pCtrlMonData->m_gst_dspinf.grid_color2))
		return FALSE;
	rc = linerect;
	double angle;
	double step = 30.0;
	double rad;
	double r;
	CPoint pt1, pt2, ptCenter;

	r = (rc.right - rc.left) / 2;
	ptCenter.x = rc.left + (rc.right - rc.left) / 2;
	ptCenter.y = rc.top + (rc.bottom - rc.top) / 2;
	m_r = r;
	m_ptCenter = ptCenter;

	if (drawType == eDrawType_After)
	{
		// 角度グリッド描画
		double r2 = r * 0.9;
		for (angle = 0.0; angle < 360.0; angle += step)
		{
			if (angle == 0.0 || angle == 90.0)
			{
				memDC.SelectObject(&penLine2);
				rad = 3.1415926538 * angle / 180.0;
				pt1.x = (int)(sin(rad) * r) + ptCenter.x;
				pt1.y = -(int)(cos(rad) * r) + ptCenter.y;
				memDC.MoveTo(pt1.x, pt1.y);
				rad = 3.1415926538 * (angle + 180.0) / 180.0;
				pt2.x = (int)(sin(rad) * r) + ptCenter.x;
				pt2.y = -(int)(cos(rad) * r) + ptCenter.y;
				memDC.LineTo(pt2.x, pt2.y);

				memDC.SelectObject(&penSolidBold2);
				rad = 3.1415926538 * angle / 180.0;
				pt1.x = (int)(sin(rad) * r / 18.0) + ptCenter.x;
				pt1.y = -(int)(cos(rad) * r / 18.0) + ptCenter.y;
				memDC.MoveTo(pt1.x, pt1.y);
				rad = 3.1415926538 * (angle + 180.0) / 180.0;
				pt2.x = (int)(sin(rad) * r / 18.0) + ptCenter.x;
				pt2.y = -(int)(cos(rad) * r / 18.0) + ptCenter.y;
				memDC.LineTo(pt2.x, pt2.y);
			}

			{
				memDC.SelectObject(&penLine);
				rad = 3.1415926538 * angle / 180.0;
				pt1.x = (int)(sin(rad) * r) + ptCenter.x;
				pt1.y = -(int)(cos(rad) * r) + ptCenter.y;
				memDC.MoveTo(pt1.x, pt1.y);
				pt2.x = (int)(sin(rad) * r2) + ptCenter.x;
				pt2.y = -(int)(cos(rad) * r2) + ptCenter.y;
				memDC.LineTo(pt2.x, pt2.y);
			}
		}

		if (rcCircle.Width() >= ANGLEVIEW_MINIMUM_SZ && rcCircle.Height() >= ANGLEVIEW_MINIMUM_SZ)
		{
			// 角度数値描画
			rc = textrect;
			if (!bViewStationModeAll)
				rc.InflateRect(-12, -12);
			else
				rc.InflateRect(-6, -6);

			r = (rc.right - rc.left) / 2;
			ptCenter.x = rc.left + (rc.right - rc.left) / 2;
			ptCenter.y = rc.top + (rc.bottom - rc.top) / 2;

			for (angle = 0.0; angle < 360.0; angle += step)
			{
				CString str;
				str.Format(_T("%.0f°"), angle);
				CSize szStr = memDC.GetTextExtent(str);
				rad = 3.1415926538 * angle / 180.0;
				pt1.x = (int)(sin(rad) * r) + ptCenter.x + 5;
				pt1.y = -(int)(cos(rad) * r) + ptCenter.y;
				memDC.TextOutW(pt1.x - szStr.cx / 2, pt1.y - szStr.cy / 2, str);
			}
		}
	}
	
	if (drawType == eDrawType_After)
	{
		if (bViewExample)
		{
			memDC.SelectObject(&m_txtFontNormal);

			// 1局サマリ画面
			if (bViewValues)
			{
				// 凡例描画
				CString strMsg;
				textrect.left += 10;
				textrect.right = textrect.left + EXAMPLE_AREA_WIDTH;
				textrect.top = textrect.bottom + 10;
				textrect.bottom = textrect.top + TEXT_MARGIN;
				rc = textrect;
				CRect exampleLineRc = rc;
				rc.OffsetRect(EXAMPLE_LINE_WIDTH + 2, 0);

				// SKYLINE
				CPen penExampleSkyline;
				if (!penExampleSkyline.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.skyl_color))
					return FALSE;
				memDC.SelectObject(&penExampleSkyline);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				if (strMsg.LoadString(IDS_RADARGRAPH_SKYLINE) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				exampleLineRc.OffsetRect(EXAMPLE_AREA_WIDTH2, 0);

				// RFトリップはサマリ画面のみ表示
				CPen penExampleRftrip;
				if (m_nViewType == eRadarViewType_Station)
				{
					// RF-TRIP
					if (!penExampleRftrip.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.rftr_color))
						return FALSE;
					memDC.SelectObject(&penExampleRftrip);
					memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
					memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
					rc.OffsetRect(EXAMPLE_AREA_WIDTH2, 0);
					if (strMsg.LoadString(IDS_RADARGRAPH_RFTRIP) == FALSE)
						return FALSE;
					memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
					exampleLineRc.OffsetRect(EXAMPLE_AREA_WIDTH2, 0);
				}

				// 予報値
				CPen penExampleForecastValue;
				if (!penExampleForecastValue.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.yoho_color))
					return FALSE;
				memDC.SelectObject(&penExampleForecastValue);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				rc.OffsetRect(EXAMPLE_AREA_WIDTH2, 0);
				if (strMsg.LoadString(IDS_FORECASTLIST_FORECASTVALUE) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				rc = textrect;
				exampleLineRc = rc;
				exampleLineRc.OffsetRect(0, 14);

				// AOS
				CPen penExampleAosValue;
				if (!penExampleAosValue.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.aos_yoho_color))
					return FALSE;
				memDC.SelectObject(&penExampleAosValue);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				rc.OffsetRect(EXAMPLE_LINE_WIDTH + 2, 14);
				if (strMsg.LoadString(IDS_RADARGRAPH_AOS) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				exampleLineRc.OffsetRect(EXAMPLE_AREA_WIDTH2, 0);

				// 実測値
				CPen penExampleActualValue;
				if (!penExampleActualValue.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.trnd_color))
					return FALSE;
				memDC.SelectObject(&penExampleActualValue);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				rc.OffsetRect(EXAMPLE_AREA_WIDTH2, 0);
				if (strMsg.LoadString(IDS_RADARGRAPH_ACTUAL_VALUE) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

				penExampleSkyline.DeleteObject();
				// RFトリップはサマリ画面のみ表示
				if (m_nViewType == eRadarViewType_Station)
				{
					penExampleRftrip.DeleteObject();
				}
				penExampleForecastValue.DeleteObject();
				penExampleAosValue.DeleteObject();
				penExampleActualValue.DeleteObject();
			}
			// 4局サマリ画面
			else if (bViewStationModeAll)
			{
				// 凡例描画
				CString strMsg;
				rc = textrect;
				rc.left = rc.left + EXAMPLE_STATIONALL_LEFT_MARGIN;
				rc.right = rc.left + EXAMPLE_STATIONALL_AREA_WIDTH;
				rc.top = rc.bottom + EXAMPLE_STATIONALL_TOP_MARGIN;
				rc.bottom = rc.top + TEXT_MARGIN;
				CRect exampleLineRc = rc;
				rc.OffsetRect(EXAMPLE_LINE_WIDTH + 2, 0);

				// SKYLINE
				CPen penExampleSkyline;
				if (!penExampleSkyline.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.skyl_color))
					return FALSE;
				memDC.SelectObject(&penExampleSkyline);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				if (strMsg.LoadString(IDS_RADARGRAPH_SKYLINE) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				exampleLineRc.OffsetRect(0, 14);

				// RF-TRIP
				CPen penExampleRftrip;
				if (!penExampleRftrip.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.rftr_color))
					return FALSE;
				memDC.SelectObject(&penExampleRftrip);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				rc.OffsetRect(0, 14);
				if (strMsg.LoadString(IDS_RADARGRAPH_RFTRIP) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				exampleLineRc.OffsetRect(0, 14);

				// 予報値
				CPen penExampleForecastValue;
				if (!penExampleForecastValue.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.yoho_color))
					return FALSE;
				memDC.SelectObject(&penExampleForecastValue);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				rc.OffsetRect(0, 14);
				if (strMsg.LoadString(IDS_FORECASTLIST_FORECASTVALUE) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				exampleLineRc.OffsetRect(0, 14);

				rc = textrect;
				rc.left = rc.left + EXAMPLE_STATIONALL_LEFT_MARGIN;
				rc.right = rc.left + EXAMPLE_STATIONALL_AREA_WIDTH;
				rc.top = rc.bottom + EXAMPLE_STATIONALL_TOP_MARGIN;
				rc.bottom = rc.top + TEXT_MARGIN;
				exampleLineRc = rc;
				rc.OffsetRect(EXAMPLE_LINE_WIDTH + EXAMPLE_STATIONALL_AREA_WIDTH / 2 + 2, 0);
				exampleLineRc.OffsetRect(EXAMPLE_STATIONALL_AREA_WIDTH / 2, 0);

				// AOS
				CPen penExampleAosValue;
				if (!penExampleAosValue.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.aos_yoho_color))
					return FALSE;
				memDC.SelectObject(&penExampleAosValue);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				if (strMsg.LoadString(IDS_RADARGRAPH_AOS) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				exampleLineRc.OffsetRect(0, 14);

				// 実測値
				CPen penExampleActualValue;
				if (!penExampleActualValue.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.trnd_color))
					return FALSE;
				memDC.SelectObject(&penExampleActualValue);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				rc.OffsetRect(0, 14);
				if (strMsg.LoadString(IDS_RADARGRAPH_ACTUAL_VALUE) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

				penExampleSkyline.DeleteObject();
				penExampleRftrip.DeleteObject();
				penExampleForecastValue.DeleteObject();
				penExampleAosValue.DeleteObject();
				penExampleActualValue.DeleteObject();
			}
			// 一覧画面
			else
			{
				int nExapleWidth;
				if (m_nViewType == eRadarViewType_OperationPlan)
				{
					// 計画一覧の場合
					nExapleWidth = EXAMPLE_AREA_WIDTH3;
				}
				else
				{
					// 予報値一覧の場合
					nExapleWidth = EXAMPLE_AREA_WIDTH4;
				}

				// 凡例描画
				CString strMsg;
				textrect.left += 10;
				textrect.right = textrect.left + EXAMPLE_AREA_WIDTH;
				textrect.top = textrect.bottom + 10;
				textrect.bottom = textrect.top + TEXT_MARGIN;
				rc = textrect;
				CRect exampleLineRc = rc;
				rc.OffsetRect(EXAMPLE_LINE_WIDTH + 2, 0);

				// SKYLINE
				CPen penExampleSkyline;
				if (!penExampleSkyline.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.skyl_color))
					return FALSE;
				memDC.SelectObject(&penExampleSkyline);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				if (strMsg.LoadString(IDS_RADARGRAPH_SKYLINE) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				exampleLineRc.OffsetRect(nExapleWidth, 0);

				// RFトリップはサマリ画面のみ表示
				CPen penExampleRftrip;
				if (m_nViewType == eRadarViewType_Station)
				{
					// RF-TRIP
					if (!penExampleRftrip.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.rftr_color))
						return FALSE;
					memDC.SelectObject(&penExampleRftrip);
					memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
					memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
					rc.OffsetRect(nExapleWidth, 0);
					if (strMsg.LoadString(IDS_RADARGRAPH_RFTRIP) == FALSE)
						return FALSE;
					memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
					exampleLineRc.OffsetRect(nExapleWidth, 0);
				}

				// 予報値
				CPen penExampleForecastValue;
				if (!penExampleForecastValue.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.yoho_color))
					return FALSE;
				memDC.SelectObject(&penExampleForecastValue);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				rc.OffsetRect(nExapleWidth, 0);
				if (strMsg.LoadString(IDS_FORECASTLIST_FORECASTVALUE) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				exampleLineRc.OffsetRect(nExapleWidth, 0);

				// AOS
				CPen penExampleAosValue;
				if (!penExampleAosValue.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.aos_yoho_color))
					return FALSE;
				memDC.SelectObject(&penExampleAosValue);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				rc.OffsetRect(nExapleWidth, 0);
				if (strMsg.LoadString(IDS_RADARGRAPH_AOS) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				exampleLineRc.OffsetRect(nExapleWidth, 0);

				// 実測値は計画一覧のみ表示
				CPen penExampleActualValue;
				if (m_nViewType == eRadarViewType_OperationPlan)
				{
					// 実測値
					if (!penExampleActualValue.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.trnd_color))
						return FALSE;
					memDC.SelectObject(&penExampleActualValue);
					memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
					memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
					rc.OffsetRect(nExapleWidth, 0);
					if (strMsg.LoadString(IDS_RADARGRAPH_ACTUAL_VALUE) == FALSE)
						return FALSE;
					memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				}

				penExampleSkyline.DeleteObject();
				// RFトリップはサマリ画面のみ表示
				if (m_nViewType == eRadarViewType_Station)
				{
					penExampleRftrip.DeleteObject();
				}
				penExampleForecastValue.DeleteObject();
				penExampleAosValue.DeleteObject();
				if (m_nViewType == eRadarViewType_OperationPlan)
				{
					penExampleActualValue.DeleteObject();
				}
			}
		}

		// 1局サマリ画面
		if (bViewValues)
		{
			int x, y;

			// 値の表示
			// 表枠描画
			rcValues.DeflateRect(VALUES_AREA_HMARGIN, VALUES_AREA_VMARGIN);
			memDC.SelectObject(&penLine);

			// 横線描画
			for (int i = 0; i < 5; i++)
			{
				y = rcValues.top + i * VALUES_AREA_ROW_H;
				memDC.MoveTo(rcValues.left, y);
				memDC.LineTo(rcValues.right, y);
			}
			int y2 = rcValues.top + 5 * VALUES_AREA_ROW_H;
			memDC.MoveTo(rcValues.left, y2);
			memDC.LineTo(rcValues.left + (int)((double)rcValues.Width() * (VALUES_AREA_FORECAST_W + VALUES_AREA_TIME_W) + 2), y2);

			// 縦線描画
			x = 0;
			memDC.MoveTo(rcValues.left + x, rcValues.top);
			memDC.LineTo(rcValues.left + x, y2);
			x += (int)((double)rcValues.Width() * VALUES_AREA_FORECAST_W + 1);
			memDC.MoveTo(rcValues.left + x, rcValues.top);
			memDC.LineTo(rcValues.left + x, y2);
			x += (int)((double)rcValues.Width() * VALUES_AREA_TIME_W + 1);
			memDC.MoveTo(rcValues.left + x, rcValues.top);
			memDC.LineTo(rcValues.left + x, y2);
			x += (int)((double)rcValues.Width() * VALUES_AREA_AZ_W);
			memDC.MoveTo(rcValues.left + x, rcValues.top);
			memDC.LineTo(rcValues.left + x, y);
			x += (int)((double)rcValues.Width() * VALUES_AREA_EL_W);
			memDC.MoveTo(rcValues.left + x, rcValues.top);
			memDC.LineTo(rcValues.left + x, y);

			CRect rcValuesTmp = rcValues;
			double dTime;
			char caTime[40];
			CString strMsg;
			CSize szStr;
			WCHAR wc[30];
			size_t chgNum;

			// 項目描画
			{
				if (strMsg.LoadString(IDS_FORECASTLIST_FORECASTVALUE) == FALSE)
					return FALSE;
				rcValuesTmp.right = (int)((double)rcValues.Width() * VALUES_AREA_FORECAST_W + 1);
				rcValuesTmp.bottom = rcValuesTmp.top + VALUES_AREA_ROW_H;
				memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				if (strMsg.LoadString(IDS_RADARGRAPH_TIME) == FALSE)
					return FALSE;
				rcValuesTmp.left = rcValuesTmp.right;
				rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_TIME_W + 1);
				memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				if (strMsg.LoadString(IDS_RADARGRAPH_AZ) == FALSE)
					return FALSE;
				rcValuesTmp.left = rcValuesTmp.right;
				rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_AZ_W);
				memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				if (strMsg.LoadString(IDS_RADARGRAPH_EL) == FALSE)
					return FALSE;
				rcValuesTmp.left = rcValuesTmp.right;
				rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_EL_W);
				memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}

			// AOS情報描画
			{
				if (strMsg.LoadString(IDS_RADARGRAPH_AOSINFO) == FALSE)
					return FALSE;
				rcValuesTmp = rcValues;
				rcValuesTmp.right = (int)((double)rcValues.Width() * VALUES_AREA_FORECAST_W + 1);
				rcValuesTmp.top = rcValuesTmp.top + VALUES_AREA_ROW_H;
				rcValuesTmp.bottom = rcValuesTmp.top + VALUES_AREA_ROW_H;
				memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				if (CSatelliteData::ub_asc_to_asctime(pCtrlMonData->m_gst_dspinf.ca_AOS_time, caTime) >= 0)
				{
					dTime = CSatelliteData::ub_asc_to_dtime(caTime);
					CTime t((__time64_t)dTime);
					strMsg = t.Format("%Y-%m-%d %H:%M:%S");
					rcValuesTmp.left = rcValuesTmp.right;
					rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_TIME_W + 1);
					memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

					mbstowcs_s(&chgNum, wc, 30, pCtrlMonData->m_gst_dspinf.ca_AOS_AZ, _TRUNCATE);
					strMsg.Format(_T("%s"), wc);
					rcValuesTmp.left = rcValuesTmp.right;
					rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_AZ_W);
					memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

					mbstowcs_s(&chgNum, wc, 30, pCtrlMonData->m_gst_dspinf.ca_AOS_EL, _TRUNCATE);
					strMsg.Format(_T("%s"), wc);
					rcValuesTmp.left = rcValuesTmp.right;
					rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_EL_W);
					memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
				}
			}

			// LOS情報描画
			{
				if (strMsg.LoadString(IDS_RADARGRAPH_LOSINFO) == FALSE)
					return FALSE;
				rcValuesTmp = rcValues;
				rcValuesTmp.right = (int)((double)rcValues.Width() * VALUES_AREA_FORECAST_W + 1);
				rcValuesTmp.top = rcValuesTmp.top + VALUES_AREA_ROW_H * 2;
				rcValuesTmp.bottom = rcValuesTmp.top + VALUES_AREA_ROW_H;
				memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				if (CSatelliteData::ub_asc_to_asctime(pCtrlMonData->m_gst_dspinf.ca_LOS_time, caTime) >= 0)
				{
					dTime = CSatelliteData::ub_asc_to_dtime(caTime);
					CTime t((__time64_t)dTime);
					strMsg = t.Format("%Y-%m-%d %H:%M:%S");
					rcValuesTmp.left = rcValuesTmp.right;
					rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_TIME_W + 1);
					memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

					mbstowcs_s(&chgNum, wc, 30, pCtrlMonData->m_gst_dspinf.ca_LOS_AZ, _TRUNCATE);
					strMsg.Format(_T("%s"), wc);
					rcValuesTmp.left = rcValuesTmp.right;
					rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_AZ_W);
					memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

					mbstowcs_s(&chgNum, wc, 30, pCtrlMonData->m_gst_dspinf.ca_LOS_EL, _TRUNCATE);
					strMsg.Format(_T("%s"), wc);
					rcValuesTmp.left = rcValuesTmp.right;
					rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_EL_W);
					memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
				}
			}

			// 最大仰角描画
			{
				if (strMsg.LoadString(IDS_RADARGRAPH_MEL) == FALSE)
					return FALSE;
				rcValuesTmp = rcValues;
				rcValuesTmp.right = (int)((double)rcValues.Width() * VALUES_AREA_FORECAST_W + 1);
				rcValuesTmp.top = rcValuesTmp.top + VALUES_AREA_ROW_H * 3;
				rcValuesTmp.bottom = rcValuesTmp.top + VALUES_AREA_ROW_H;
				memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				if (CSatelliteData::ub_asc_to_asctime(pCtrlMonData->m_gst_dspinf.ca_MEL_time, caTime) >= 0)
				{
					dTime = CSatelliteData::ub_asc_to_dtime(caTime);
					CTime t((__time64_t)dTime);
					strMsg = t.Format("%Y-%m-%d %H:%M:%S");
					rcValuesTmp.left = rcValuesTmp.right;
					rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_TIME_W + 1);
					memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

					mbstowcs_s(&chgNum, wc, 30, pCtrlMonData->m_gst_dspinf.ca_MEL_AZ, _TRUNCATE);
					strMsg.Format(_T("%s"), wc);
					rcValuesTmp.left = rcValuesTmp.right;
					rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_AZ_W);
					memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

					mbstowcs_s(&chgNum, wc, 30, pCtrlMonData->m_gst_dspinf.ca_MEL_EL, _TRUNCATE);
					//strMsg.Format(_T("%s"), wc);
					strMsg.Format(_T("%s"), (LPCTSTR)CString(pCtrlMonData->m_gst_dspinf.ca_MEL_EL));
					rcValuesTmp.left = rcValuesTmp.right;
					rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_EL_W);
					memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
				}
			}

			// 可視時間描画
			{
				if (strMsg.LoadString(IDS_RADARGRAPH_VISIBLETIME) == FALSE)
					return FALSE;
				rcValuesTmp = rcValues;
				rcValuesTmp.right = (int)((double)rcValues.Width() * VALUES_AREA_FORECAST_W + 1);
				rcValuesTmp.top = rcValuesTmp.top + VALUES_AREA_ROW_H * 4;
				rcValuesTmp.bottom = rcValuesTmp.top + VALUES_AREA_ROW_H;
				memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				mbstowcs_s(&chgNum, wc, 30, pCtrlMonData->m_gst_dspinf.ca_visible, _TRUNCATE);
				strMsg.Format(_T("%s"), wc);
				szStr = memDC.GetTextExtent(strMsg);
				rcValuesTmp.left = rcValuesTmp.right;
				rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_TIME_W + 1);
				memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
			}
		}
	}

	memDC.SelectObject(oldBrush);
	memDC.SelectObject(oldPen);
	penSolidBold.DeleteObject();
	penSolidBold2.DeleteObject();
	penSolid.DeleteObject();
	penLine.DeleteObject();
	penLine2.DeleteObject();
	brushBk.DeleteObject();
	brush80.DeleteObject();

	return TRUE;
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# 初期化処理

@param	なし
@retval	なし

*/
/*============================================================================*/
void CForecastRadarGraphView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// 初期化処理
	Init(_T(""), _T(""), _T(""));
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# 初期化処理

@param	なし
@retval	なし

*/
/*============================================================================*/
void CForecastRadarGraphView::Init(CString fpath, CString satellite, CString predId)
{
//	if (m_nStationID < 0)
//		return;

	// ビュータイプと局IDから、衛星データを取得
	switch (m_nViewType)
	{
	case eRadarViewType_Station:
		// 局の場合は局IDを監視データIDにする
		m_nCtrlMonDataID = m_nStationID;
		break;

	case eRadarViewType_ForecastList:
		// 予報値一覧の場合は局+1のID値にする
		m_nCtrlMonDataID = eStation_MAX;
		break;

	case eRadarViewType_OperationPlan:
	default:
		// 計画一覧の場合は局+2のID値にする
		m_nCtrlMonDataID = eStation_MAX + 1;
		break;
	}

	CGraphMutex::Lock(eRadar);

	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		CGraphMutex::Unlock(eRadar);
		return;
	}

	// ビューIDと局番号セット
	pCtrlMonData->SetViewID(m_nCtrlMonDataID);
	pCtrlMonData->SetStationID(m_nStationID);
	// キューのクリア
	pCtrlMonData->ClearQeue();

	// 表示色設定
	//	pCtrlMonData->m_gst_dspinf.pixmap_color		= PIXMAP_CLR;
	pCtrlMonData->m_gst_dspinf.grph_color = GRPH_CLR;
	pCtrlMonData->m_gst_dspinf.frem_color = FREM_CLR;
	pCtrlMonData->m_gst_dspinf.frem_color2 = FREM_CLR2;
	pCtrlMonData->m_gst_dspinf.e80_frem_color = E80_CLR;
	pCtrlMonData->m_gst_dspinf.grid_color = GRID_CLR;
	pCtrlMonData->m_gst_dspinf.grid_color2 = GRID_CLR2;
	pCtrlMonData->m_gst_dspinf.strtit_color = STRTIT_CLR;
	pCtrlMonData->m_gst_dspinf.strcon_color = STRCON_CLR;
	pCtrlMonData->m_gst_dspinf.strvar_color = STRVAR_CLR;
	pCtrlMonData->m_gst_dspinf.aos_yoho_color = YOHO_AOS_CLR;
	pCtrlMonData->m_gst_dspinf.los_yoho_color = YOHO_LOS_CLR;
	pCtrlMonData->m_gst_dspinf.yoho_color = YOHO_CLR;
	pCtrlMonData->m_gst_dspinf.trnd_color = TRND_CLR;
	pCtrlMonData->m_gst_dspinf.skyl_color = SKYL_CLR;
	pCtrlMonData->m_gst_dspinf.rftr_color = RFTR_CLR;
	pCtrlMonData->m_gst_dspinf.over_color = OVER_CLR;

//	// ダミーデータ初期化
//	if (fpath.IsEmpty() && m_nViewType == eRadarViewType_Station)
//	{
//		fpath = theApp.GetShareFilePath(eFileType_Yoho_File, m_nStationID);
//	}
//	// 計画一覧でない場合
//	else if (!fpath.IsEmpty())
	{
		// 拡張子がprdの場合、anpに置き換える
		fpath.TrimRight(PRED_FILE_TYPE);
		fpath += ANP_FILE_TYPE;
	}

	// 運用状況監視データを初期化する
	InitGtunyouData(fpath, satellite, predId);

	CGraphMutex::Unlock(eRadar);

	// グラフデータ更新
	UpdateGraphData();

	// タイマ登録
	SetTimer(1, INTERVAL_REAL, NULL);

#if 0
// test >>>
	CRFTripAngleFile	rftrip;
	vector<double> listAzEl;
//	if (rftrip.ReadFile((UINT)m_nStationID, CString(_T("HAYABUSA2")), CString(_T("HPA1-100kw")), CString(_T("ABCD_BPSK_123")), listAzEl))
	if (rftrip.ReadFile((UINT)m_nStationID, CString(_T("HAYABUSA2")), CString(_T("HPA1-100kw")), CString(_T("")), listAzEl))
	{
		TRACE("Read RfTripFile OK\n");
	}
	else
	{
		TRACE("Read RfTripFile NG\n");
	}

// test <<<
#endif
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# 運用状況監視データを初期化する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CForecastRadarGraphView::InitGtunyouData(CString fpath, CString& satellite, CString& predId)
{
	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		return;
	}

	pCtrlMonData->m_l_inflg = 0;	// 無効にしておく
	pCtrlMonData->m_gi_cmp1 = 1;
	pCtrlMonData->m_gi_cmp2 = 1;
	pCtrlMonData->m_gst_dspinf.i_dsp_type = TYPE_NEW;
	pCtrlMonData->m_gst_dspinf.i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_IN;

	// 初期化
	wcscpy_s(pCtrlMonData->m_gst_dspinf.ca_fpath, 1024, fpath.GetBuffer());
	fpath.ReleaseBuffer();

	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_stime, 30, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_etime, 30, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_ftime, 30, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_plnno, 30, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_eisei, 100, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_AOS_time, 20, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_AOS_AZ, 9, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_AOS_EL, 8, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_LOS_time, 20, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_LOS_AZ, 9, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_LOS_EL, 8, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_MEL_time, 20, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_MEL_AZ, 9, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_MEL_EL, 8, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_visible, 9, "");
	strcpy_s(pCtrlMonData->m_ca_yohouget, 30, "");

	if (m_nStationID < 0)
		return;

	// 予報値一覧、計画一覧から取得していた値をセット
	// 局、衛星、予報値ファイル名と一致する予報値を予報値一覧リストから探す
	CGraphMutex::Lock(eFile);
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();
	CTime ctime;
	CStringA strA;
	for (int i = 0; i < (int)listPred.size(); i++)
	{
		if (listPred[i].strStationName.CompareNoCase(mStationString[m_nStationID]) == 0 && 
			listPred[i].strSatellite.CompareNoCase(satellite) == 0 &&
			listPred[i].strPassId.CompareNoCase(predId) == 0)
		{
			pCtrlMonData->m_l_inflg = INFLG_ON;	// データ有効

			// 初期化
			wcscpy_s(pCtrlMonData->m_gst_dspinf.ca_fpath, 1024, fpath.GetBuffer());
			fpath.ReleaseBuffer();

			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_plnno, 30, CStringA(m_strPlanID));							// 計画一覧画面の場合、選択した計画の計画番号をセット
			ctime = CTime(m_tPassStart);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_stime, 30, CStringA(ctime.Format("%Y-%m-%d %H:%M:%S")));	// 計画一覧画面の場合、選択した計画の運用開始時刻をセット（計画一覧リストから取得）
			ctime = CTime(m_tPassEnd);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_etime, 30, CStringA(ctime.Format("%Y-%m-%d %H:%M:%S")));	// 計画一覧画面の場合、選択した計画の運用終了時刻をセット（計画一覧リストから取得）

			// 予報値ファイル更新日時の取得
			HANDLE hFile;
			CTime ftimestamp;
			hFile = CreateFile(fpath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				ftimestamp = CTime(0);
			}
			else
			{
				FILETIME cfTime, afTime, wfTime;
				GetFileTime(hFile, &cfTime, &afTime, &wfTime);
				CloseHandle(hFile);
				ftimestamp = CTime(wfTime);
			}
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_ftime, 30, CStringA(ftimestamp.Format("%Y-%m-%d %H:%M:%S")));	// 選択した予報値ファイルタイムスタンプをセット（ファイルから取得）

			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_eisei, 100, CStringA(satellite));								// 衛星名セット
			ctime = CTime(listPred[i].tAOS);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_AOS_time, 20, CStringA(ctime.Format("%Y-%m-%d %H:%M:%S")));		// AOS時刻セット
			strA.Format("%3.4f", listPred[i].dAosAz);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_AOS_AZ, 9, strA);												// AOS AZセット
			strA.Format("%2.4f", listPred[i].dAosEl);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_AOS_EL, 8, strA);												// AOS ELセット
			ctime = CTime(listPred[i].tLOS);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_LOS_time, 20, CStringA(ctime.Format("%Y-%m-%d %H:%M:%S")));		// LOS時刻セット
			strA.Format("%3.4f", listPred[i].dLosAz);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_LOS_AZ, 9, strA);												// LOS AZセット
			strA.Format("%2.4f", listPred[i].dLosEl);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_LOS_EL, 8, strA);												// LOS ELセット
			ctime = CTime(listPred[i].tMaxEl);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_MEL_time, 20, CStringA(ctime.Format("%Y-%m-%d %H:%M:%S")));		// MAX時刻セット
			strA.Format("%3.4f", listPred[i].dMaxAz);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_MEL_AZ, 9, strA);												// MAX AZセット
			strA.Format("%2.4f", listPred[i].dMaxEl);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_MEL_EL, 8, strA);												// MAX ELセット

			break;
		}
	}
	CGraphMutex::Unlock(eFile);

	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	CTime t = timeSys;
//	m_dummyInitTime = t.GetTime();
	pCtrlMonData->m_i_unyou = 1;

#ifdef _DEBUG
	m_dummyCnt = 0;
#endif
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# ダミー実測値データを作成する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CForecastRadarGraphView::AddDummyData()
{
	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		return;
	}

	CGraphMutex::Lock(eRadar);

	int sz = (int)pCtrlMonData->m_queAzElForecast.size();
	static double offset = 0.0;
	static double inc = 0.2;

	if (m_dummyCnt == sz)
	{
		CGraphMutex::Unlock(eRadar);
		return;
	}

	if (m_dummyCnt < sz / 2)
	{
		offset = offset + inc;
	}
	else
	{
		offset = offset - inc;
	}
	Az_El	azel;
	azel.d_AZ = pCtrlMonData->m_queAzElForecast[m_dummyCnt].d_AZ + offset;
	azel.d_EL = pCtrlMonData->m_queAzElForecast[m_dummyCnt].d_EL + offset;
	pCtrlMonData->m_queAzElActual.push_back(azel);
	m_dummyCnt++;

	CGraphMutex::Unlock(eRadar);
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# タイマイベント発生時に呼び出される

@param	nIDEvent：イベントID
@retval	なし

*/
/*============================================================================*/
void CForecastRadarGraphView::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(1);

	UpdateGraphData();

	SetTimer(1, INTERVAL_REAL, NULL);

	CView::OnTimer(nIDEvent);
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# グラフデータを定期更新する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CForecastRadarGraphView::UpdateGraphData()
{
	CGraphMutex::Lock(eRadar);

	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		return;
	}

	// 情報更新
	pCtrlMonData->UpdateData();

	CGraphMutex::Unlock(eRadar);

	// ビュー更新
	Invalidate();
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# スカイライン描画

@param	memDC		ドキュメント
@param	rectClient	描画領域
@retval	TRUE:スカイラインを描画した / FALSE:スカイラインを描画しなかった

*/
/*============================================================================*/
BOOL CForecastRadarGraphView::DrawSkyline(CDC& memDC, CRect& rectClient)
{
	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		return FALSE;
	}

	CPen penSolid;
	if (!penSolid.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.skyl_color))
		return FALSE;
	CPen* oldPen = memDC.SelectObject(&penSolid);
	CBrush brush;
	brush.CreateSolidBrush(pCtrlMonData->m_gst_dspinf.skyl_color);
	CBrush* oldBrush = (CBrush*)memDC.SelectObject(&brush);

	int     i_X, i_Y;               /* グラフ描画座標             	  */
	POINT ptLast, ptLastCircle;
	POINT ptCur, ptCurCircle;
	POINT pt[4];
	double rad;

	for (int i = 0; i < pCtrlMonData->m_queAzElSkyline.size(); i++)
	{
		GtazelToXY(pCtrlMonData->m_queAzElSkyline[i].d_AZ, pCtrlMonData->m_queAzElSkyline[i].d_EL, (double)m_ptCenter.x,
			(double)m_ptCenter.y, m_r, &i_X, &i_Y);
		/* グラフ描画 */
//		memDC.SetPixel(i_X, i_Y, pCtrlMonData->m_gst_dspinf.skyl_color);
		// ポリゴン
		ptCur.x = i_X; ptCur.y = i_Y;
//		rad = atan2(-(ptCur.y - m_ptCenter.y) , (ptCur.x - m_ptCenter.x));
		rad = atan2((ptCur.x - m_ptCenter.x), -(ptCur.y - m_ptCenter.y));
//		if (rad < 0)
//		{
//			rad = rad + 2 * 3.1415926538;
//		}

		ptCurCircle.x = (int)(sin(rad) * m_r) + m_ptCenter.x;
		ptCurCircle.y = -(int)(cos(rad) * m_r) + m_ptCenter.y;

//		memDC.SetPixel(ptCurCircle.x, ptCurCircle.y, pCtrlMonData->m_gst_dspinf.skyl_color);

		if (i == 0)
		{
			ptLast = ptCur;
			ptLastCircle = ptCurCircle;
			continue;
		}

		pt[0] = ptLast;
		pt[1] = ptCur;
		pt[2] = ptCurCircle;
		pt[3] = ptLastCircle;

		memDC.Polygon(pt, 4);

		ptLast = ptCur;
		ptLastCircle = ptCurCircle;
	}

	memDC.SelectObject(oldBrush);
	memDC.SelectObject(oldPen);
	penSolid.DeleteObject();
	brush.DeleteObject();

	return TRUE;
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# RFトリップ描画

@param	memDC		ドキュメント
@param	rectClient	描画領域
@retval	TRUE:RFトリップを描画した / FALSE:RFトリップを描画しなかった

*/
/*============================================================================*/
BOOL CForecastRadarGraphView::DrawRfTrip(CDC& memDC, CRect& rectClient)
#if 0
{
	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		return FALSE;
	}

	CPen penSolid;
	if (!penSolid.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.rftr_color))
		return FALSE;
	CPen* oldPen = memDC.SelectObject(&penSolid);

	int     i_X, i_Y;               /* グラフ描画座標             	  */
	int		i_X_0, i_Y_0;
	BOOL	bExistI0 = FALSE;
	for (int i = 0; i < pCtrlMonData->m_queAzElRftrip.size(); i++)
	{
		GtazelToXY(pCtrlMonData->m_queAzElRftrip[i].d_AZ, pCtrlMonData->m_queAzElRftrip[i].d_EL, (double)m_ptCenter.x,
			(double)m_ptCenter.y, m_r, &i_X, &i_Y);
		/* グラフ描画 */
//		memDC.SetPixel(i_X, i_Y, pCtrlMonData->m_gst_dspinf.rftr_color);
		if (i == 0)
		{
			memDC.MoveTo(i_X, i_Y);
			i_X_0 = i_X;
			i_Y_0 = i_Y;
			bExistI0 = TRUE;
		}
		else
		{
			memDC.LineTo(i_X, i_Y);
		}
	}
	if (bExistI0)
		memDC.LineTo(i_X_0, i_Y_0);

	return TRUE;
}
#else
{
	if (m_nStationID < 0)
		return FALSE;

	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		return FALSE;
	}

	CPen penSolid;
	if (!penSolid.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.rftr_color))
		return FALSE;
	CPen* oldPen = memDC.SelectObject(&penSolid);

	int     i_X, i_Y;               /* グラフ描画座標             	  */
	int		i_X_0, i_Y_0;
	BOOL	bExistI0 = FALSE;

#if 0
	vector<Az_El> listAzEl;
	theApp.GetSatelliteData().GetRFTripAngleData(m_nStationID).GetAzEl(listAzEl);

	for (int i = 0; i < listAzEl.size(); i++)
	{
		GtazelToXY(listAzEl[i].d_AZ, listAzEl[i].d_EL, (double)m_ptCenter.x,
			(double)m_ptCenter.y, m_r, &i_X, &i_Y);
		/* グラフ描画 */
		//		memDC.SetPixel(i_X, i_Y, pCtrlMonData->m_gst_dspinf.rftr_color);
		if (i == 0)
		{
			memDC.MoveTo(i_X, i_Y);
			i_X_0 = i_X;
			i_Y_0 = i_Y;
			bExistI0 = TRUE;
		}
		else
		{
			memDC.LineTo(i_X, i_Y);
		}
	}
	if (bExistI0)
		memDC.LineTo(i_X_0, i_Y_0);
#else

	vector<double> listAzEl;
	theApp.GetSatelliteData().GetRFTripAngleData(m_nStationID).GetAzEl(listAzEl);

	for (__int64 i = 0; i < (__int64)listAzEl.size(); i++)
	{
		GtazelToXY((double)i, listAzEl[i], (double)m_ptCenter.x,
			(double)m_ptCenter.y, m_r, &i_X, &i_Y);
		/* グラフ描画 */
		//		memDC.SetPixel(i_X, i_Y, pCtrlMonData->m_gst_dspinf.rftr_color);
		if (i == 0)
		{
			memDC.MoveTo(i_X, i_Y);
			i_X_0 = i_X;
			i_Y_0 = i_Y;
			bExistI0 = TRUE;
		}
		else
		{
			memDC.LineTo(i_X, i_Y);
		}
	}
	if (bExistI0)
		memDC.LineTo(i_X_0, i_Y_0);

#endif

	return TRUE;
}
#endif


/*============================================================================*/
/*! CForecastRadarGraphView

-# 予報値描画

@param	memDC		ドキュメント
@param	rectClient	描画領域
@retval	TRUE:予報値を描画した / FALSE:予報値を描画しなかった

*/
/*============================================================================*/
BOOL CForecastRadarGraphView::DrawForecast(CDC& memDC, CRect& rectClient)
{
	if (m_nStationID < 0)
		return FALSE;

	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		return FALSE;
	}

	CTrackingRadarCtrlMonData* pStationData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nStationID);
	if (pStationData == NULL)
	{
		return FALSE;
	}

	CPen penAos, penYoho, penYohoBold;
	if (!penAos.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.aos_yoho_color))
		return FALSE;
	if (!penYoho.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.yoho_color))
		return FALSE;
	if (!penYohoBold.CreatePen(PS_SOLID, LINE_WID_FRAM_BOLD2, pCtrlMonData->m_gst_dspinf.yoho_color))
		return FALSE;

	CPen* oldPen = memDC.SelectObject(&penAos);

	int     i_X, i_Y;					/* グラフ描画座標          	  */
	char	  ca_stime[40];				/* パス運用開始時刻（フル）   */
	char	  ca_etime[40];				/* パス運用終了時刻（フル）   */
	double	  d_Stime;					/* パス運用開始時刻（積算秒） */
	double	  d_Etime;					/* パス運用終了時刻（積算秒） */
	double    d_time;                   /* グラフデータ発生時刻       */

	if (m_nViewType == eRadarViewType_OperationPlan || m_nViewType == eRadarViewType_ForecastList)
	{
		// 計画一覧の場合
		/* パス開始時刻（積算秒）算出 */
		CSatelliteData::ub_asc_to_asctime(pCtrlMonData->m_gst_dspinf.ca_stime, ca_stime);
		d_Stime = CSatelliteData::ub_asc_to_dtime(ca_stime);

		/* パス終了時刻（積算秒）算出 */
		CSatelliteData::ub_asc_to_asctime(pCtrlMonData->m_gst_dspinf.ca_etime, ca_etime);
		d_Etime = CSatelliteData::ub_asc_to_dtime(ca_etime);
	}
	else
	{
		// サマリ画面の場合
		/* パス開始時刻（積算秒）算出 */
		CSatelliteData::ub_asc_to_asctime(pStationData->m_gst_dspinf.ca_stime, ca_stime);
		d_Stime = CSatelliteData::ub_asc_to_dtime(ca_stime);

		/* パス終了時刻（積算秒）算出 */
		CSatelliteData::ub_asc_to_asctime(pStationData->m_gst_dspinf.ca_etime, ca_etime);
		d_Etime = CSatelliteData::ub_asc_to_dtime(ca_etime);
	}

	__int64 size = (__int64)pCtrlMonData->m_queAzElForecast.size();

	for (__int64 i = 0; i < size; i++)
	{
		/* データ発生時刻 ＧＥＴ */
		d_time = pCtrlMonData->m_queAzElForecast[i].d_time;

		GtazelToXY(pCtrlMonData->m_queAzElForecast[i].d_AZ, pCtrlMonData->m_queAzElForecast[i].d_EL, (double)m_ptCenter.x,
			(double)m_ptCenter.y, m_r, &i_X, &i_Y);

		/* プロットフラグの初期化 */
		if (i == 0) m_i_plot_flag = CTrackingRadarCtrlMonData::eOFF;
		else {
			if (pCtrlMonData->m_queAzElForecast[i].d_aos >= 0.0 && pCtrlMonData->m_queAzElForecast[i].d_aos < 0.1) {
				memDC.SelectObject(&penAos);
				m_i_plot_flag = CTrackingRadarCtrlMonData::eON;
			}
			else {
				if (m_i_plot_flag == CTrackingRadarCtrlMonData::eOFF) {
					memDC.SelectObject(&penAos);
				}
				else {
					// 計画一覧の追尾グラフの場合
//					if (m_nViewType == eRadarViewType_OperationPlan && d_time >= d_Stime && d_time <= d_Etime)
					if (m_nViewType != eRadarViewType_ForecastList && d_time >= d_Stime && d_time <= d_Etime)
					{
						memDC.SelectObject(&penYohoBold);
					}
					else
					{
						memDC.SelectObject(&penYoho);
					}
				}
				m_i_plot_flag = CTrackingRadarCtrlMonData::eCOMP;
			}
		}

		/* プロットループ１回目以外なら、予報値を線で描画する */
		if (i != 0) {
			memDC.LineTo(i_X, i_Y);
		}
		else
		{
			memDC.MoveTo(i_X, i_Y);
		}
	}

	memDC.SelectObject(oldPen);
	penAos.DeleteObject();
	penYoho.DeleteObject();
	penYohoBold.DeleteObject();

	return TRUE;
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# 追尾グラフ保存データ描画

@param	memDC		ドキュメント
@param	rectClient	描画領域
@retval	TRUE:追尾グラフ保存データを描画した / FALSE:追尾グラフ保存データを描画しなかった

*/
/*============================================================================*/
BOOL CForecastRadarGraphView::DrawTrakingSaveData(CDC& memDC, CRect& rectClient)
{
	if (m_nStationID < 0)
		return FALSE;

	int nCtrlMonDataID;
	if (m_nViewType == eRadarViewType_OperationPlan)
	{
		// 計画一覧の追尾の場合は、予報値ファイルパスが局のカレントのファイルパスと一致し、かつ計画ID一致時に実測値を表示
		CTrackingRadarCtrlMonData* pOprListData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
		CTrackingRadarCtrlMonData* pStationData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nStationID);
		if (pOprListData == NULL || pStationData == NULL)
		{
			return FALSE;
		}

		CString strOprStationPlanID = CString(pStationData->m_gst_dspinf.ca_plnno);
		if (pOprListData->m_yohoFilePath.CompareNoCase(pStationData->m_yohoFilePath) == 0 &&
			m_strPlanID.CompareNoCase(strOprStationPlanID) == 0)
		{
			nCtrlMonDataID = m_nStationID;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		nCtrlMonDataID = m_nCtrlMonDataID;
	}

	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		return FALSE;
	}

//	char	  ca_stime[40];				/* パス運用開始時刻（フル）   */
//	char	  ca_etime[40];				/* パス運用終了時刻（フル）   */
//	double	  d_Stime;					/* パス運用開始時刻（積算秒） */
//	double	  d_Etime;					/* パス運用終了時刻（積算秒） */
	int       i_X, i_Y;                 /* グラフ描画座標             */
//	double    d_time;                   /* グラフデータ発生時刻       */

//	/*----------*/
//	/* 初期処理 */
//	/*----------*/
//	/* パス開始時刻（積算秒）算出 */
//	CSatelliteData::ub_asc_to_asctime(pCtrlMonData->m_gst_dspinf.ca_stime, ca_stime);
//	d_Stime = CSatelliteData::ub_asc_to_dtime(ca_stime);
//
//	/* パス終了時刻（積算秒）算出 */
//	CSatelliteData::ub_asc_to_asctime(pCtrlMonData->m_gst_dspinf.ca_etime, ca_etime);
//	d_Etime = CSatelliteData::ub_asc_to_dtime(ca_etime);

	/*------------*/
	/* グラフ描画 */
	/*------------*/
	CPen penActual;
	if (!penActual.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.trnd_color))
		return FALSE;

	CPen* oldPen = memDC.SelectObject(&penActual);

	__int64 size = (__int64)pCtrlMonData->m_queAzElActual.size();

	for (__int64 i = 0; i < size; i++)
	{
//		/* データ発生時刻 ＧＥＴ */
//		d_time = pCtrlMonData->m_queAzElActual[i].d_time;
//
//#ifndef _DEBUG
//		/* パス運用開始時刻前の場合描画しない */
//		if (d_time < d_Stime || d_time > d_Etime){
//			continue;
//		}
//#endif
		if ((pCtrlMonData->m_queAzElActual[i].d_AZ != IGNORE_DATA) && (pCtrlMonData->m_queAzElActual[i].d_EL != IGNORE_DATA)) {

			/* グラフ描画座標を求める */
			GtazelToXY(pCtrlMonData->m_queAzElActual[i].d_AZ, pCtrlMonData->m_queAzElActual[i].d_EL,
				(double)m_ptCenter.x, (double)m_ptCenter.y,
				m_r, &i_X, &i_Y);
			if (i == 0)
			{
				memDC.MoveTo(i_X, i_Y);
			}
			else
			{
				memDC.LineTo(i_X, i_Y);
			}
		}
	}

	memDC.SelectObject(oldPen);
	penActual.DeleteObject();

	return TRUE;
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# ＡＺ、ＥＬ座標算出関数

@param	d_Az：ＡＺ値
@param	d_El：ＥＬ値
@param	d_CX：円心Ｘ座標
@param	d_CY：円心Ｙ座標
@param	d_RD：円の半径
@param	*i_OX：算出Ｘ座標
@param	*i_OY：算出Ｙ座標
@retval	なし

*/
/*============================================================================*/
void CForecastRadarGraphView::GtazelToXY(double d_Az, double d_El, double d_CX, double d_CY, double d_RD, int* i_OX, int* i_OY)
{
	double		d_X, d_Y;

	d_X = d_CX + ((d_RD * ((90.0 - d_El) / 90.0)) * sin(d_Az * RADCV));
	d_Y = d_CY - ((d_RD * ((90.0 - d_El) / 90.0)) * cos(d_Az * RADCV));

	*i_OX = (int)d_X;
	*i_OY = (int)d_Y;
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# 背景消去時に呼ばれる

@param  pDC：ドキュメントのデバイスコンテキストポインタ
@retval なし
*/
/*============================================================================*/
BOOL CForecastRadarGraphView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
