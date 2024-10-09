// TimeLine.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "TimeLine.h"

#define _TEXTOFFSET 2
#define _TIMEOFFSET 7

#define _TIMEBREAK	6
#define _BACKHEIGHT_RATE	3

// CTimeLine

IMPLEMENT_DYNAMIC(CTimeLine, CWnd)

CTimeLine::CTimeLine()
{
	mTimeScaleIndex = 0;

	for (int i = 0; i < eColor_MAX; i++){
		mColors[i].base.color = mOpeColorTable[i];
		createColors(mColors[i]);
		createPens(mColors[i]);
	}
	//mAutoScrol = true;
	//mForward = false;
	//mNext = false;
	memset(&mStartTime, 0, sizeof(SYSTEMTIME));
	mTimeBreak = false;
	mItemPos = 0;
	mLabelCelTopPos = 0;
	mItemMaxPoint = 0;
	mItemMaxCount = 0;
	mHeader = false;

	// 文字描画用フォントの作成
	CFont defFont;
	defFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (defFont.GetLogFont(&lf)){
		lf.lfWeight = FW_NORMAL;
		lf.lfHeight = 11;
		mTxtFont.DeleteObject();
		mTxtFont.CreateFontIndirect(&lf);
		lf.lfWeight = FW_NORMAL;
		lf.lfHeight = 11;
		mTimeFont.DeleteObject();
		mTimeFont.CreateFontIndirect(&lf);
	}
	mRedrawCursorPos = 0;

	mForecastSelect = 0;
	mUplinkSelect = 0;
	mCmdSelect = 0;
	mRngSelect = 0;

	m_nUplnkEvent = 0;
	m_nCmdEvent = 0;
	m_nRngEvent = 0;
}

CTimeLine::~CTimeLine()
{
	mTxtFont.DeleteObject();
	mTimeFont.DeleteObject();

	for (int i = 0; i < eColor_MAX; i++){
		deletePens(mColors[i]);
	}

	if (mBitmap.GetSafeHandle()){
		mBitmap.DeleteObject();
	}
}


BEGIN_MESSAGE_MAP(CTimeLine, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	//ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CTimeLine メッセージ ハンドラー
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
BOOL CTimeLine::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL result = FALSE;
	m_pParentWnd = pParentWnd;

	dwStyle |= WS_CHILD;
	dwStyle |= /*WS_HSCROLL |*/ WS_VSCROLL;
	
	// ウィンドウの作成
	result = CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID, NULL);

	//UpdateScrollInfo();

	return result;
}

void CTimeLine::OnSize(UINT nType, int cx, int cy)
{
	if (mBitmap.GetSafeHandle()){
		mBitmap.DeleteObject();
	}

	CWnd::OnSize(nType, cx, cy);
}

/*============================================================================*/
/*! グラフ描画

-# グラフウィンドウの描画

@param	なし
@retval	なし

*/
/*============================================================================*/
void CTimeLine::OnPaint()
{
	const COLORREF m_ColorBk = RGB(255, 255, 255);

	CRect rectClient;
	CRect rectGrid;
	CPaintDC dc(this);

	// クライアント領域の取得
	GetClientRect(rectClient);
	CRect rectBack = rectClient;
	rectBack.bottom *= _BACKHEIGHT_RATE;

	CDC memDC;
	// 描画ＤＣの作成
	memDC.CreateCompatibleDC(&dc);
	CBitmap* pOldBitmap;

	if (mHeader == true){
		CSize windowSize(rectClient.Width(), rectClient.Height());
		if (windowSize.cy < m_displaySize.cy){
			rectBack.bottom = m_displaySize.cy;
		}
		// 描画用ビットマップの作成
		mBitmap.CreateCompatibleBitmap(&dc, rectBack.Width(), rectBack.Height());
		pOldBitmap = memDC.SelectObject(&mBitmap);

		// バックグラウンド モードを設定
		memDC.SetBkMode(TRANSPARENT);
		// 背景色の設定
		memDC.SetBkColor(m_ColorBk);

		// 全体を塗りつぶし
		memDC.FillSolidRect(rectBack, m_ColorBk);

		drawTimeLine(memDC);
		dc.BitBlt(0, 0, rectBack.Width(), rectBack.Height(), &memDC, 0, m_scrollPos.cy/*0*/, SRCCOPY);
	}
	else{
		CSize windowSize(rectClient.Width(), rectClient.Height());
		if (windowSize.cy < m_displaySize.cy){
			rectBack.bottom = m_displaySize.cy;
		}
		// 描画用ビットマップの作成
		mBitmap.CreateCompatibleBitmap(&dc, rectBack.Width(), rectBack.Height());
		pOldBitmap = memDC.SelectObject(&mBitmap);

		// バックグラウンド モードを設定
		memDC.SetBkMode(TRANSPARENT);
		// 背景色の設定
		memDC.SetBkColor(m_ColorBk);

		// 全体を塗りつぶし
		memDC.FillSolidRect(rectBack, m_ColorBk);

		drawForecast(memDC);
		dc.BitBlt(0, 0, rectBack.Width(), rectBack.Height(), &memDC, 0, m_scrollPos.cy/*0*/, SRCCOPY);
	}

	drawCurrentTime(dc);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	mBitmap.DeleteObject();
}

/*============================================================================*/
/*! グラフ描画

-# タイムライン描画

@param	memDC		描画用デバイスコンテキスト
@retval	なし

*/
/*============================================================================*/
int CTimeLine::drawCurrentTime(CDC& memDC)
{
	CRect rectClient, rect;
	GetClientRect(rectClient);

	// 文字描画用フォントの作成
	CFont *oldFont = NULL;
	oldFont = memDC.SelectObject(&mTxtFont);

	// 現在のフォントから文字サイズを取得する
	CSize txtSize = memDC.GetTextExtent(_T("WWWWWWWWWWWWWWW"));
	CSize timeSize = memDC.GetTextExtent(_T("WWWWWWWWWW"));

	int labelWidth = _FIXEDLABEL_WIDTH;// txtSize.cx;
	int cellHeight = txtSize.cy + _TEXTOFFSET;

	rect = rectClient;
	rect.left += labelWidth;

	int timeWidth = rect.Width() / (MAX_TIMEDIV-1);
	int offsetWidth = (rect.Width() - (timeWidth * (MAX_TIMEDIV-1)));

	int retTimePos;

	CTime ctime(mStartTime.wYear, mStartTime.wMonth, mStartTime.wDay, mStartTime.wHour, (mStartTime.wMinute / mTimeRange[mTimeScaleIndex]) * mTimeRange[mTimeScaleIndex], 0);
	CTime startTime, stopTime;
	CTimeSpan spTime(0, 0, mTimeRange[mTimeScaleIndex], 0);
	//TRACE("## CursorTime : %s\n", CStringA(ctime.Format(_T("%H:%M:%S"))));

	// 現在時刻から開始時刻を求める
	ctime -= spTime;
	startTime = ctime;
	CRect startRect = CRect(rect.left + labelWidth, rect.top, rect.left + (timeWidth * (MAX_TIMEDIV-1)) - offsetWidth, rect.bottom);

	TRACE("### StartTime=%s\n", CStringA(startTime.Format("%Y%m%d%H%M%S")));

	rect.right = rect.left + (timeWidth * (MAX_TIMEDIV-1));
	int x = CalculateTimeToX(startTime, rect, mCurrentTime);
	if (x < 0)
		return -1;

	retTimePos = x;

	if (retTimePos >= mRedrawCursorPos){
		// 80%までカーソルがきたら移動を行う
		mTimeBreak = true;
	}

	CRect currect = rectClient;
	if (mHeader == true){
		currect.top = rectClient.top + (cellHeight * 2);
	}

	//if (mHeader == false)
	{
		CPen		pen;
		COLORREF	cf = RGB(255, 0, 0);
		COLORREF	cfs = RGB(150, 150, 150);
		if (pen.CreatePen(PS_SOLID, 1, cfs)){
			CPen* pOldPen = memDC.SelectObject(&pen);
			memDC.MoveTo(x + 1, currect.top);
			memDC.LineTo(x + 1, currect.bottom);
			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}
		if (pen.CreatePen(PS_SOLID, 1, cf)){
			CPen* pOldPen = memDC.SelectObject(&pen);
			memDC.MoveTo(x, currect.top);
			memDC.LineTo(x, currect.bottom);
			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}
	}

	// 三角形描画
	CPoint pts[4];
	pts[0].x = x;
	pts[0].y = rectClient.top + (cellHeight * 2);
	pts[1].x = x+3;
	pts[1].y = rectClient.top + (cellHeight * 2) - 3;
	pts[2].x = x-3;
	pts[2].y = rectClient.top + (cellHeight * 2) - 3;
	pts[3].x = x;
	pts[3].y = rectClient.top + (cellHeight * 2);

	CPen penRed(PS_SOLID, 1, RGB(255, 0, 0));
	CPen* oldPen = memDC.SelectObject(&penRed);
	CBrush	brush(RGB(255, 0, 0));
	CBrush *oldBrush = memDC.SelectObject(&brush);
	if (mHeader == true){
		memDC.Polygon(pts, 4);
	}
	if (oldBrush != NULL){
		memDC.SelectObject(oldBrush);
		brush.DeleteObject();
	}
	if (oldPen != NULL){
		memDC.SelectObject(oldPen);
	}
	penRed.DeleteObject();

	return retTimePos;
}

/*============================================================================*/
/*! グラフ描画

-# タイムライン描画

@param	memDC		描画用デバイスコンテキスト
@retval	なし

*/
/*============================================================================*/
void CTimeLine::drawTimeLine(CDC& memDC)
{
	CRect rectClient, rect;
	GetClientRect(rectClient);

	// 文字描画用フォントの作成
	CFont *oldFont = NULL;
	oldFont = memDC.SelectObject(&mTxtFont);

	// 線描画用ペンの作成
	CPen *oldPen = NULL;
	oldPen = memDC.SelectObject(&mColors[eColor_Grid].base.pen);

	// 現在のフォントから文字サイズを取得する
	CSize txtSize = memDC.GetTextExtent(_T("WWWWWWWWWWWWWWW"));
	CSize timeSize = memDC.GetTextExtent(_T("WWWWWWWWWW"));

	int labelWidth = _FIXEDLABEL_WIDTH;// txtSize.cx;
	int cellHeight = txtSize.cy + _TEXTOFFSET;
	mLabelCelHeight = cellHeight;

		vector<int> celtop;
	int item = 0, subitem = 0;

	// 時間セル、ラベルセルの背景色設定
	CRect back;
	back = rectClient;
	back.right = back.left + labelWidth;
	memDC.FillSolidRect(back, mOpeColorTable[eColor_FixedCell]);
	back = rectClient;
	back.bottom = back.top + cellHeight * 2;
	if (mHeader == true){
		memDC.FillSolidRect(back, mOpeColorTable[eColor_FixedCell]);
	}

	// 外枠の描画
	memDC.SelectStockObject(NULL_BRUSH);
	memDC.Rectangle(rectClient);

	// 縦線の描画
	memDC.SelectObject(&mColors[eColor_Grid].base.pen);
	rect = rectClient;
	rect.left += labelWidth;

	// ラベル列の右側描画
	memDC.MoveTo(rect.left, rect.top);
	memDC.LineTo(rect.left, rect.bottom);

	int timeWidth = rect.Width() / (MAX_TIMEDIV-1);
	int timeDrawWidth = timeWidth;
	int offsetWidth = (rect.Width() - (timeWidth * (MAX_TIMEDIV-1)));

	CTime ctime(mStartTime.wYear, mStartTime.wMonth, mStartTime.wDay, mStartTime.wHour, (mStartTime.wMinute / mTimeRange[mTimeScaleIndex]) * mTimeRange[mTimeScaleIndex], 0);

	CTime startTime, stopTime;
	CTimeSpan spTime(0, 0, mTimeRange[mTimeScaleIndex], 0);
	CTimeSpan spHalfTime(0, 0, mTimeRange[mTimeScaleIndex]/2, 0);

	// 現在時刻から開始時刻を求める
	ctime -= spTime;
	startTime = ctime;
	//TRACE("## StartTime : %s\n", CStringA(startTime.Format(_T("%H:%M:%S"))));
	CRect startRect = CRect(rect.left + labelWidth, rect.top, rect.left + (timeWidth * (MAX_TIMEDIV-1)) - offsetWidth, rect.bottom);

	//============================================================================
	// 計算
	rect = rectClient;
	rect.left += labelWidth;
	rect.right = rect.left + labelWidth;
	rect.bottom = rect.top + (cellHeight * 2);
	CRect calcRect;
	calcRect.SetRectEmpty();
	bool bTextThin = false;		// 間引きフラグ
	bool bTextInsert = false;	// 挿入フラグ
	for (item = 0; item < MAX_TIMEDIV; item++){
		if (item == 0){
			rect.left = rect.left + timeWidth;
			continue;
		}
		// 時間軸の描画
		rect.right = rect.left + timeWidth;

		// 時間文字列の描画
		CString sdate = ctime.Format(_T("%Y-%m-%d"));
		CString stime = ctime.Format(_T("%H:%M:%S"));
		CRect r = rect;
		r.left = r.left - (timeWidth / 2);
		r.right = r.left + timeWidth;
		r.bottom = r.top + cellHeight;
		memDC.DrawText(sdate, r, DT_CALCRECT | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		if (calcRect.IsRectEmpty() == FALSE){
			if (calcRect.right > r.left){
				timeDrawWidth = calcRect.Width();
				bTextThin = true;
			}
			else if ((r.left-calcRect.right) > (calcRect.Width()*3))
				bTextInsert = true;
		}
		calcRect = r;

		r.top = r.bottom;
		r.bottom = r.top + cellHeight;
		rect.left = rect.left + timeWidth;
	}

	CRect rectTime;
	rectTime = rectClient;
	rectTime.left = rectClient.left + labelWidth;
	rectTime.right = rectTime.left + (timeWidth * (MAX_TIMEDIV-1));

	//============================================================================
	// 描画
	if (mHeader == true){
		rect = rectClient;
		rect.right = rect.left + labelWidth;
		rect.bottom = rect.top + (cellHeight*2);
		memDC.DrawText(_T("UTC"), rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
	rect = rectClient;
	rect.left += labelWidth;
	rect.right = rect.left + labelWidth;
	rect.bottom = rect.top + (cellHeight * 2);
	for (item = 0; item < MAX_TIMEDIV; item++){
		// 描画領域拡大時の特別処理
		if (bTextInsert == true){
			CTime half = ctime - spHalfTime;
			int x = CalculateTimeToX(startTime, rectTime, half);
			if (mHeader == false){
				CPen pen;
				if (pen.CreatePen(PS_DOT, 1, mOpeColorTable[eColor_GridCell])){
					CPen* pOldPen = memDC.SelectObject(&pen);
					memDC.MoveTo(x, rectClient.top);// +(cellHeight * 2));
					memDC.LineTo(x, rectClient.bottom);
					memDC.SelectObject(pOldPen);
					pen.DeleteObject();
				}
			}
		}

		if (item == 0 || item == (MAX_TIMEDIV-1)){
			ctime += spTime;
			rect.left = rect.left + timeDrawWidth;
			continue;
		}
		if (bTextThin == true && (item%2) == 1){
			ctime += spTime;
			rect.left = rect.left + timeDrawWidth;
			continue;
		}

		// 時間軸の描画
		rect.right = rect.left + timeDrawWidth;
		if (mHeader == true){
			memDC.MoveTo(rect.left, rectClient.top + (cellHeight * 2));
		}
		else{
			memDC.MoveTo(rect.left, rectClient.top);
		}
		memDC.LineTo(rect.left, rectClient.bottom);

		if (mHeader == true){
			// 時間文字列の描画
			CString sdate = ctime.Format(_T("%Y-%m-%d"));
			CString stime = ctime.Format(_T("%H:%M:%S"));
			CRect r = rect;
			r.left = r.left - (timeDrawWidth / 2);
			r.right = r.left + timeDrawWidth;
			r.bottom = r.top + cellHeight;
			memDC.DrawText(sdate, r, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			r.top = r.bottom;
			r.bottom = r.top + cellHeight;
			memDC.DrawText(stime, r, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			if (item == _TIMEBREAK)
				mRedrawCursorPos = r.left;
		}
		ctime += spTime;
		rect.left = rect.left + timeDrawWidth;
	}

	CRect rectLabel, rectTemp;
	CRect rectFixed;
	rectFixed = rectClient;
	if (mHeader == true){
		// 時間セルの全体領域
		rectTime.left = rectClient.left + labelWidth;
		rectTime.right = rectTime.left + (timeWidth * (MAX_TIMEDIV-1));
		rectLabel = rectClient;

		// 時間横軸描画
		memDC.SelectObject(&mColors[eColor_Grid].base.pen);
		rectLabel.top = rectLabel.top + (cellHeight * 2);
		rectLabel.bottom = rectLabel.top + cellHeight;
		memDC.MoveTo(rectClient.left, rectLabel.top);
		memDC.LineTo(rectClient.right, rectLabel.top);
		if (oldPen != NULL){
			memDC.SelectObject(oldPen);
		}
		if (oldFont != NULL){
			memDC.SelectObject(oldFont);
		}
		rectFixed.top = rectLabel.top;
	}

	cellHeight = txtSize.cy * 2 + _TIMEOFFSET;

	// 運用タイムラインの描画
	memDC.SelectObject(&mColors[eColor_Pass].base.pen);
	mItemMaxCount = 0;

	stTimeLine data;
	vector<stTimeLine>	timeList[eTimeKind_Max];
	CString timetext;

	// データ収集

	timeList[eTimeKind_Master].clear();
	// 開始時刻をセットしておく
	CTime stInitAnt = mTimelineData.m_OpetDay.start;

	// 運用日時
#if 0
	data.updown = false;
	data.start = mTimelineData.m_OpetDay.start;
	data.end = mTimelineData.m_OpetDay.end;
	if (mTimelineData.m_strPlanType.CompareNoCase(_T("RDYCHK")) == 0)
	{
		// レディネスチェックの計画の場合
		data.label = _T("RDYCHK");
		data.color = mOpeColorTable[eColor_ReadinessCheck];
		stInitAnt = data.end;
	}
	else
	{
		// 通常の計画およびその他の計画の場合
		data.label = _T("パス運用");
		data.color = mOpeColorTable[eColor_Pass];
	}
	timeList[eTimeKind_Master].push_back(data);
	timetext = data.start.Format(_T("%Y-%m-%d %H:%M:%S"));
	//TRACE("#1 %s\n", CStringA(timetext));
#else
	if (mTimelineData.m_strPlanType.CompareNoCase(_T("RDYCHK")) == 0)
	{
		// レディネスチェックの計画の場合、初期設定時刻をセットしておく
		stInitAnt = mTimelineData.m_OpetDay.end;
	}
	else
	{
		// 通常の計画およびその他の計画の場合
		data.label = _T("パス運用");
		data.updown = false;
		data.start = mTimelineData.m_OpetDay.start;
		data.end = mTimelineData.m_OpetDay.end;
		data.color = mOpeColorTable[eColor_Pass];
		timeList[eTimeKind_Master].push_back(data);
		timetext = data.start.Format(_T("%Y-%m-%d %H:%M:%S"));
		//TRACE("#1 %s\n", CStringA(timetext));
	}
#endif

	CTimeSpan sall, span;
	sall = CTimeSpan(0, 0, 0, 0);
	// 初期設定
	data.label = _T("INT");
	data.updown = false;
	time_t t_initpass = mTimelineData.m_InitPassTime.start.GetTime();
	time_t t_antwait = mTimelineData.m_AntWaitTime.start.GetTime();
	if (mTimelineData.m_OpInit && t_initpass > 0){
		// 「初期設定」の時刻
		if (mTimelineData.m_AntStandby && (t_antwait > 0)){
			// 「ANT待ち受け指向通知」がONの場合は「ANT待ち受け指向通知」の時間と「初期設定」の時刻から古い時刻を選ぶ
			data.end = mTimelineData.m_OpetDay.start;
			data.start = (t_initpass > t_antwait) ? mTimelineData.m_AntWaitTime.start : mTimelineData.m_InitPassTime.start;
			data.color = mOpeColorTable[eColor_EqInitialize];
			data.updown = true;
			timeList[eTimeKind_Master].push_back(data);
			stInitAnt = data.start;
			timetext = data.start.Format(_T("%Y-%m-%d %H:%M:%S"));
			timetext = data.end.Format(_T("%Y-%m-%d %H:%M:%S"));
		}
		else {
			// 「ANT待ち受け指向通知」がOFFの場合、「初期設定」の時刻を選ぶ
			data.end = mTimelineData.m_OpetDay.start;
			data.start = mTimelineData.m_InitPassTime.start;
			data.color = mOpeColorTable[eColor_EqInitialize];
			data.updown = false;
			timeList[eTimeKind_Master].push_back(data);
			stInitAnt = data.start;
		}
		timetext = data.start.Format(_T("%Y-%m-%d %H:%M:%S"));
		//TRACE("#2 %s\n", CStringA(timetext));
	}
	// ANT待ち受け指向通知
	data.label = _T("ANT");
	if (data.updown == false && mTimelineData.m_AntStandby && t_antwait > 0){
		// 「ANT待ち受け指向通知」がONの場合
		data.end = mTimelineData.m_OpetDay.start;
		data.start = mTimelineData.m_AntWaitTime.start;
		data.color = mOpeColorTable[eColor_Antenna];
		timeList[eTimeKind_Master].push_back(data);
		stInitAnt = data.start;
		if (mTimelineData.m_OpInit == 0){
			// 初期設定が存在しない場合にはレディネスチェック用の時間位置を加算する
			sall += span;
		}
		timetext = data.start.Format(_T("%Y-%m-%d %H:%M:%S"));
		//TRACE("#3 %s\n", CStringA(timetext));
	}

	data.updown = false;

	// ANT天頂指向有無
	data.label = _T("ANT");
	if (mTimelineData.m_AntZenith){
		if (theApp.GetSelectStation() == eStation_Usuda64 || theApp.GetSelectStation() == eStation_Usuda642)
			span = CTimeSpan(0, 0, 0, OperationTime_AntennaZenith);
		else
			span = CTimeSpan(0, 0, 0, OperationTime_AntennaZenith2);
		data.start = mTimelineData.m_OpetDay.end + span;
		data.end = data.start - span;
		data.color = mOpeColorTable[eColor_PostPass];
		timeList[eTimeKind_Master].push_back(data);
		timetext = data.start.Format(_T("%Y-%m-%d %H:%M:%S"));
		//TRACE("#4 %s\n", CStringA(timetext));
	}
#if 0
	// ARTCを個別に描画
	// CMD伝送
	data.label = _T("C");
	if (mTimelineData.m_CmdChkValue){
		span = CTimeSpan(0, 0, 0, OperationTime_Cmd);
		data.end = mTimelineData.m_OpetDay.start - sall;
		data.start = data.end - span;
		data.color = mOpeColorTable[eColor_ReadinessCheck];
		timeList[eTimeKind_Master].push_back(data);
		sall += span;
		timetext = data.start.Format(_T("%Y-%m-%d %H:%M:%S"));
		//TRACE("#5 %s\n", CStringA(timetext));
	}
	// TLM伝送
	data.label = _T("T");
	if (mTimelineData.m_TlmChkValue){
		span = CTimeSpan(0, 0, 0, OperationTime_Tlm);
		data.end = mTimelineData.m_OpetDay.start - sall;
		data.start = data.end - span;
		data.color = mOpeColorTable[eColor_ReadinessCheck];
		timeList[eTimeKind_Master].push_back(data);
		sall += span;
		timetext = data.start.Format(_T("%Y-%m-%d %H:%M:%S"));
		//TRACE("#6 %s\n", CStringA(timetext));
	}
	// RNG校正
	data.label = _T("R");
	if (mTimelineData.m_RndChkValue){
		span = CTimeSpan(0, 0, 0, OperationTime_Rng);
		data.end = mTimelineData.m_OpetDay.start - sall;
		data.start = data.end - span;
		data.color = mOpeColorTable[eColor_ReadinessCheck];
		timeList[eTimeKind_Master].push_back(data);
		sall += span;
		timetext = data.start.Format(_T("%Y-%m-%d %H:%M:%S"));
		//TRACE("#7 %s\n", CStringA(timetext));
	}
	// AGC校正
	data.label = _T("A");
	if (mTimelineData.m_AgcChkValue){
		span = CTimeSpan(0, 0, 0, OperationTime_Agc);
		data.end = mTimelineData.m_OpetDay.start - sall;
		data.start = data.end - span;
		data.color = mOpeColorTable[eColor_ReadinessCheck];
		timeList[eTimeKind_Master].push_back(data);
		sall += span;
		timetext = data.start.Format(_T("%Y-%m-%d %H:%M:%S"));
		//TRACE("#8 %s\n", CStringA(timetext));
	}
#else

#if 0
	// ARTCをまとめて描画
	// CMD伝送
//	data.label = _T("RDYCHK");
	CString strARTC = _T("");
	if (mTimelineData.m_AgcChkValue){
		strARTC = strARTC + _T("A");
	}
	if (mTimelineData.m_RndChkValue){
		strARTC = strARTC + _T("R");
	}
	if (mTimelineData.m_TlmChkValue){
		strARTC = strARTC + _T("T");
	}
	if (mTimelineData.m_CmdChkValue){
		strARTC = strARTC + _T("C");
	}
	data.label = strARTC;
	span = 0;
	if (mTimelineData.m_AgcChkValue || mTimelineData.m_RndChkValue || mTimelineData.m_TlmChkValue || mTimelineData.m_CmdChkValue){
		span += CTimeSpan(0, 0, 0, OperationTime_Agc);
		span += CTimeSpan(0, 0, 0, OperationTime_Rng);
		span += CTimeSpan(0, 0, 0, OperationTime_Tlm);
		span += CTimeSpan(0, 0, 0, OperationTime_Cmd);
	}

	if (span != 0){
		// レディネスチェックが存在する
		timetext = stInitAnt.Format(_T("%Y-%m-%d %H:%M:%S"));
		data.end = stInitAnt;
		data.start = data.end - span;
		data.color = mOpeColorTable[eColor_ReadinessCheck];
		timeList[eTimeKind_Master].push_back(data);
		sall += span;
		timetext = data.start.Format(_T("%Y-%m-%d %H:%M:%S"));
		timetext = data.end.Format(_T("%Y-%m-%d %H:%M:%S"));
		//TRACE("#5 %s\n", CStringA(timetext));
	}
#else
	// ARTCをまとめて描画
	// CMD伝送
	//	data.label = _T("RDYCHK");
	CString strARTC = _T("");
	time_t rdy_t = LONG64_MAX;
	BOOL bRdychk = FALSE;
	if (mTimelineData.m_AgcChkValue){
		strARTC = strARTC + _T("A");
		time_t agc_t = mTimelineData.m_AgcTime.GetTime();
		if (agc_t < rdy_t)
		{
			rdy_t = agc_t;
		}
		bRdychk = TRUE;
	}
	if (mTimelineData.m_RndChkValue){
		strARTC = strARTC + _T("R");
		time_t rng_t = mTimelineData.m_RngTime.GetTime();
		if (rng_t < rdy_t)
		{
			rdy_t = rng_t;
		}
		bRdychk = TRUE;
	}
	if (mTimelineData.m_TlmChkValue){
		strARTC = strARTC + _T("T");
		time_t tlm_t = mTimelineData.m_TlmTime.GetTime();
		if (tlm_t < rdy_t)
		{
			rdy_t = tlm_t;
		}
		bRdychk = TRUE;
	}
	if (mTimelineData.m_CmdChkValue){
		strARTC = strARTC + _T("C");
		time_t cmd_t = mTimelineData.m_CmdTime.GetTime();
		if (cmd_t < rdy_t)
		{
			rdy_t = cmd_t;
		}
		bRdychk = TRUE;
	}

	data.label = strARTC;
	if (bRdychk && rdy_t < LONG64_MAX)
	{
		// レディネスチェックが存在する
		timetext = stInitAnt.Format(_T("%Y-%m-%d %H:%M:%S"));
		if (stInitAnt.GetTime() >= rdy_t)
		{
			data.end = stInitAnt;
			data.start = CTime(rdy_t);
			data.color = mOpeColorTable[eColor_ReadinessCheck];
			timeList[eTimeKind_Master].push_back(data);
			sall += CTimeSpan(rdy_t);
			timetext = data.start.Format(_T("%Y-%m-%d %H:%M:%S"));
			timetext = data.end.Format(_T("%Y-%m-%d %H:%M:%S"));
			//TRACE("#5 %s\n", CStringA(timetext));
		}
	}
#endif

#endif // 0

	timeList[eTimeKind_DownLink].clear();
	// TLM S日時
	if (mTimelineData.m_TlmSChk){
		if (mTimelineData.m_TlmSDayList.size() > 0)
		{
			data.label = _T("TLM S");
			for (int i = 0; i < mTimelineData.m_TlmSDayList.size(); i++){
				//if (mUplinkSelect == i)
				{
					data.start = mTimelineData.m_TlmSDayList[i].start;
					data.end = mTimelineData.m_TlmSDayList[i].end;
					data.color = mOpeColorTable[eColor_DownlinkRecv];
					timeList[eTimeKind_DownLink].push_back(data);
					//break;
				}
			}
		}
	}

	// TLM X日時
	if (mTimelineData.m_TlmXChk){
		if (mTimelineData.m_TlmXDayList.size() > 0)
		{
			data.label = _T("TLM X");
			for (int i = 0; i < mTimelineData.m_TlmXDayList.size(); i++){
				//if (mUplinkSelect == i)
				{
					data.start = mTimelineData.m_TlmXDayList[i].start;
					data.end = mTimelineData.m_TlmXDayList[i].end;
					data.color = mOpeColorTable[eColor_DownlinkRecv];
					timeList[eTimeKind_DownLink].push_back(data);
					//break;
				}
			}
		}
	}

	// TLM Ka日時
	if (mTimelineData.m_TlmKaChk){
		if (mTimelineData.m_TlmKaDayList.size() > 0)
		{
			data.label = _T("TLM Ka");
			for (int i = 0; i < mTimelineData.m_TlmKaDayList.size(); i++){
				//if (mUplinkSelect == i)
				{
					data.start = mTimelineData.m_TlmKaDayList[i].start;
					data.end = mTimelineData.m_TlmKaDayList[i].end;
					data.color = mOpeColorTable[eColor_DownlinkRecv];
					timeList[eTimeKind_DownLink].push_back(data);
					//break;
				}
			}
		}
	}

	timeList[eTimeKind_UpLink].clear();
	// アップリンク
	if (m_nUplnkEvent == 0 && mTimelineData.m_UpLinkChk){
		if (mTimelineData.m_UpLinkCnt > 0)
		{
			data.label = _T("UPLINK");
			for (int i = 0; i < mTimelineData.m_UpLinkCnt; i++){
				//if (mUplinkSelect == i)
				{
					data.start = mTimelineData.m_ULDay[i].start;
					data.end = mTimelineData.m_ULDay[i].end;
					data.color = mOpeColorTable[eColor_UplinkkSend];
					timeList[eTimeKind_UpLink].push_back(data);
					//break;
				}
			}
		}
	}

	// CMD
	if (m_nCmdEvent == 0 && mTimelineData.m_CmdChk){
		if (mTimelineData.m_CmdCnt > 0)
		{
			data.label = _T("CMD");
			for (int i = 0; i < mTimelineData.m_CmdCnt; i++){
				//if (mCmdSelect == i)
				{
					data.start = mTimelineData.m_CmdDayList[i].start;
					data.end = mTimelineData.m_CmdDayList[i].end;
					data.color = mOpeColorTable[eColor_Command];
					timeList[eTimeKind_UpLink].push_back(data);
					//break;
				}
			}
		}
	}

	// RNG
	if (m_nRngEvent == 0 && mTimelineData.m_RngChk){
		if (mTimelineData.m_RngCnt > 0)
		{
			data.label = _T("RNG");
			for (int i = 0; i < mTimelineData.m_RngCnt; i++){
				//if (mRngSelect == i)
				{
					data.start = mTimelineData.m_RngDayList[i].start;
					data.end = mTimelineData.m_RngDayList[i].end;
					data.color = mOpeColorTable[eColor_Range];
					timeList[eTimeKind_UpLink].push_back(data);
					//break;
				}
			}
		}
	}

	// 文字用ペンの作成
	CPen pen(PS_SOLID, 1, RGB(0,0,0));
	memDC.SelectObject(&pen);
	memDC.SelectObject(&mTimeFont);

	rectLabel = rectClient;
	rectLabel = rectFixed;
	mLabelCelTopPos = rectLabel.top;
	rectLabel.bottom = rectLabel.top + cellHeight;

	const UINT mTimeCelOffset = 6;
	const UINT mTimeNextOffset = 2;

	CRect rectNextCel = rectLabel;

	for (int row = 0; row < eTimeKind_Max; row++){
		if (timeList[row].size() == 0){
			// データが存在しないので何もしない
			continue;
		}

		// セル上部の線を引く
		memDC.MoveTo(rectClient.left, rectNextCel.top);
		memDC.LineTo(rectClient.right, rectNextCel.top);

		// 各種別ごとのタイムラインを描画
		vector<stTimeLine>::iterator itr;
		rectTime.top = rectNextCel.top + mTimeCelOffset;
		rectTime.bottom = rectNextCel.top + cellHeight - mTimeCelOffset;
		rectTemp = rectTime;
		int h = rectTemp.Height();
		CString breakLabel = _T("");
		int lnum = 0;
		int idx;
		for (itr = timeList[row].begin(), idx = 0; itr != timeList[row].end(); itr++, idx++){
			if (idx == 0)
			{
				lnum = 1;
			}
			CTime starttime((*itr).start);
			int x = CalculateTimeToX(startTime, rectTime, starttime);
			if (x < 0)
				x = rectTime.left;
			rectTemp.left = x;
			CTime stoptime((*itr).end);
			x = CalculateTimeToX(startTime, rectTime, stoptime);
			if (x < 0){
				if (row == eTimeKind_DownLink){
					if (breakLabel != _T("") && breakLabel != (*itr).label){
						rectTemp.top = rectTemp.bottom + mTimeNextOffset;
						rectTemp.bottom = rectTemp.top + h;
						lnum++;
					}
					breakLabel = (*itr).label;
				}
				continue;
			}
			rectTemp.right = x;

			if ((*itr).updown == true){
				// [初期設定]と[ANT待ち受け指向通知]がONの場合の特別処理
				// 上に[初期設定]
				// 下に[ANT待ち受け指向通知]
				CRect r = rectTemp;
				r.bottom = r.top + (rectTemp.Height() / 2);
				drawTimeline(memDC, r, (*itr).color);
				memDC.DrawText((*itr).label, r, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
				//itr++;
				r = rectTemp;
				r.top = rectTemp.top + (rectTemp.Height() / 2);
				drawTimeline(memDC, r, mOpeColorTable[eColor_Antenna]);
				memDC.DrawText(_T("ANT"), r, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			}
			else{
				if (row == eTimeKind_UpLink || row == eTimeKind_DownLink){
					if (breakLabel != _T("") && breakLabel != (*itr).label){
						rectTemp.top = rectTemp.bottom + mTimeNextOffset;
						rectTemp.bottom = rectTemp.top + h;
						lnum++;
					}
				}
				drawTimeline(memDC, rectTemp, (*itr).color);
				memDC.DrawText((*itr).label, rectTemp, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
				if (row == eTimeKind_UpLink || row == eTimeKind_DownLink){
					breakLabel = (*itr).label;
				}
			}
		}

		if (row < eTimeKind_Max - 1){
			memDC.MoveTo(rectClient.left, rectLabel.bottom);
			memDC.LineTo(rectClient.right, rectLabel.bottom);
		}

		if (row != eTimeKind_Master){
			if (timeList[row].size() != 0){
				rectNextCel.bottom = rectNextCel.top + (lnum*h) + (mTimeCelOffset * 2) + ((lnum - 1)*mTimeNextOffset);
			}
		}
		
		rectTemp = rectNextCel;
		rectTemp.right = rectTemp.left + labelWidth;
		// ラベル名の描画（「マスター計画」「ダウンリンク」「アップリンク」）
		memDC.DrawText(mTimeLabel[row], rectTemp, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

		mItemMaxPoint = rectNextCel.bottom;
		rectNextCel.top = rectNextCel.bottom;
		rectNextCel.bottom += cellHeight;

		mItemMaxCount++;
	}

	pen.DeleteObject();

	if (oldPen != NULL){
		memDC.SelectObject(oldPen);
	}

	if (oldFont != NULL){
		memDC.SelectObject(oldFont);
	}

	UpdateScrollInfo();
}

/*============================================================================*/
/*! グラフ描画

-# タイムライン描画（予報値）

@param	memDC		描画用デバイスコンテキスト
@retval	なし

*/
/*============================================================================*/
void CTimeLine::drawForecast(CDC& memDC)
{
	CRect rectClient, rect;
	GetClientRect(rectClient);
	rectClient.bottom = rectClient.top + rectClient.Height() * 2;

	// 文字描画用フォントの作成
	CFont *oldFont = NULL;
	oldFont = memDC.SelectObject(&mTxtFont);

	// 線描画用ペンの作成
	CPen *oldPen = NULL;
	oldPen = memDC.SelectObject(&mColors[eColor_Grid].base.pen);

	// 現在のフォントから文字サイズを取得する
	CSize txtSize = memDC.GetTextExtent(_T("WWWWWWWWWWWWWWW"));
	CSize timeSize = memDC.GetTextExtent(_T("WWWWWWWWWW"));

	int labelWidth = _FIXEDLABEL_WIDTH;// txtSize.cx;
	int cellHeight = txtSize.cy + _TEXTOFFSET;
	mLabelCelHeight = cellHeight;

	vector<int> celtop;
	int item = 0, subitem = 0;

	// データ収集
	stTimeLine data;
	vector<stTimeLine>	timeList;
	timeList.clear();
	// 予報値リストを検索して登録する
	vector<stPredListData>::iterator itrp;
	for (itrp = mListPredData.begin(); itrp != mListPredData.end(); itrp++){
		data.label = (*itrp).strPassId;
		data.start = CTime((*itrp).tAOS);
		data.end = CTime((*itrp).tLOS);
		data.color = mOpeColorTable[eColor_Forecast];
		timeList.push_back(data);
	}

	vector<stTimeLine>::iterator itrt;

	const UINT mTimeCelOffset = 6;
	const UINT mTimeNextOffset = 2;

	UINT allHeight = 0;
	UINT oneHeight = ((txtSize.cy * 2 + _TIMEOFFSET) - mTimeCelOffset) - (mTimeCelOffset);
	if (timeList.size() != 0){
		int size = (int)timeList.size();
		allHeight = 0 + (size*oneHeight) + (mTimeCelOffset * 2) + ((size - 1)*mTimeNextOffset);
	}
	if (allHeight != 0){
		rectClient.bottom = rectClient.top + allHeight;
	}

	// 時間セル、ラベルセルの背景色設定
	CRect back;
	back = rectClient;
	back.right = back.left + labelWidth;
	memDC.FillSolidRect(back, mOpeColorTable[eColor_FixedCell]);
	back = rectClient;
	back.bottom = back.top + cellHeight * 2;

	// 外枠の描画
	memDC.SelectStockObject(NULL_BRUSH);
	memDC.Rectangle(rectClient);

	// 縦線の描画
	memDC.SelectObject(&mColors[eColor_Grid].base.pen);
	rect = rectClient;
	rect.left += labelWidth;

	// ラベル列の右側描画
	memDC.MoveTo(rect.left, rect.top);
	memDC.LineTo(rect.left, rect.bottom);

	int timeWidth = rect.Width() / (MAX_TIMEDIV - 1);
	int timeDrawWidth = timeWidth;
	int offsetWidth = (rect.Width() - (timeWidth * (MAX_TIMEDIV - 1)));

	CTime ctime(mStartTime.wYear, mStartTime.wMonth, mStartTime.wDay, mStartTime.wHour, (mStartTime.wMinute / mTimeRange[mTimeScaleIndex]) * mTimeRange[mTimeScaleIndex], 0);

	CTime startTime, stopTime;
	CTimeSpan spTime(0, 0, mTimeRange[mTimeScaleIndex], 0);
	CTimeSpan spHalfTime(0, 0, mTimeRange[mTimeScaleIndex] / 2, 0);

	// 現在時刻から開始時刻を求める
	ctime -= spTime;
	startTime = ctime;
	//TRACE("@@ StartTime : %s\n", CStringA(startTime.Format(_T("%H:%M:%S"))));
	CRect startRect = CRect(rect.left + labelWidth, rect.top, rect.left + (timeWidth * (MAX_TIMEDIV - 1)) - offsetWidth, rect.bottom);

	//============================================================================
	// 計算
	rect = rectClient;
	rect.left += labelWidth;
	rect.right = rect.left + labelWidth;
	rect.bottom = rect.top + (cellHeight * 2);
	CRect calcRect;
	calcRect.SetRectEmpty();
	bool bTextThin = false;		// 間引きフラグ
	bool bTextInsert = false;	// 挿入フラグ
	for (item = 0; item < MAX_TIMEDIV; item++){
		if (item == 0){
			rect.left = rect.left + timeWidth;
			continue;
		}
		// 時間軸の描画
		rect.right = rect.left + timeWidth;

		// 時間文字列の描画
		CString sdate = ctime.Format(_T("%Y-%m-%d"));
		CString stime = ctime.Format(_T("%H:%M:%S"));
		CRect r = rect;
		r.left = r.left - (timeWidth / 2);
		r.right = r.left + timeWidth;
		r.bottom = r.top + cellHeight;
		memDC.DrawText(sdate, r, DT_CALCRECT | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		if (calcRect.IsRectEmpty() == FALSE){
			if (calcRect.right > r.left){
				timeDrawWidth = calcRect.Width();
				bTextThin = true;
			}
			else if ((r.left - calcRect.right) > (calcRect.Width() * 3))
				bTextInsert = true;
		}
		calcRect = r;

		r.top = r.bottom;
		r.bottom = r.top + cellHeight;
		rect.left = rect.left + timeWidth;
	}

	CRect rectTime;
	rectTime = rectClient;
	rectTime.left = rectClient.left + labelWidth;
	rectTime.right = rectTime.left + (timeWidth * (MAX_TIMEDIV - 1));

	//============================================================================
	// 描画
	rect = rectClient;
	rect.left += labelWidth;
	rect.right = rect.left + labelWidth;
	rect.bottom = rect.top + (cellHeight * 2);
	for (item = 0; item < MAX_TIMEDIV; item++){
		// 描画領域拡大時の特別処理
		if (bTextInsert == true){
			CTime half = ctime - spHalfTime;
			int x = CalculateTimeToX(startTime, rectTime, half);
			if (mHeader == false){
				CPen pen;
				if (pen.CreatePen(PS_DOT, 1, mOpeColorTable[eColor_GridCell])){
					CPen* pOldPen = memDC.SelectObject(&pen);
					memDC.MoveTo(x, rectClient.top);// +(cellHeight * 2));
					memDC.LineTo(x, rectClient.bottom);
					memDC.SelectObject(pOldPen);
					pen.DeleteObject();
				}
			}
		}

		if (item == 0 || item == (MAX_TIMEDIV - 1)){
			ctime += spTime;
			rect.left = rect.left + timeDrawWidth;
			continue;
		}
		if (bTextThin == true && (item % 2) == 1){
			ctime += spTime;
			rect.left = rect.left + timeDrawWidth;
			continue;
		}

		// 時間軸の描画
		rect.right = rect.left + timeDrawWidth;
		if (mHeader == true){
			memDC.MoveTo(rect.left, rectClient.top + (cellHeight * 2));
		}
		else{
			memDC.MoveTo(rect.left, rectClient.top);
		}
		memDC.LineTo(rect.left, rectClient.bottom);

		// 時間文字列の描画
		CString sdate = ctime.Format(_T("%Y-%m-%d"));
		CString stime = ctime.Format(_T("%H:%M:%S"));
		CRect r = rect;
		r.left = r.left - (timeDrawWidth / 2);
		r.right = r.left + timeDrawWidth;
		r.bottom = r.top + cellHeight;
		//memDC.DrawText(sdate, r, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		r.top = r.bottom;
		r.bottom = r.top + cellHeight;
		//memDC.DrawText(stime, r, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		if (item == _TIMEBREAK)
			mRedrawCursorPos = r.left;
		ctime += spTime;
		rect.left = rect.left + timeDrawWidth;
	}

	CRect rectLabel, rectTemp;

	cellHeight = txtSize.cy * 2 + _TIMEOFFSET;

	// 運用タイムラインの描画
	memDC.SelectObject(&mColors[eColor_Forecast].base.pen);
	mItemMaxCount = 0;

	// 文字用ペンの作成
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	memDC.SelectObject(&pen);
	memDC.SelectObject(&mTimeFont);

	rectLabel = rectClient;
	mLabelCelTopPos = rectLabel.top;

	CRect rectNextCel = rectLabel;

	// セル上部の線を引く
	memDC.MoveTo(rectClient.left, rectNextCel.top);
	memDC.LineTo(rectClient.right, rectNextCel.top);

	// 予報値の描画
	rectTime.top = rectNextCel.top + mTimeCelOffset;
	rectTime.bottom = rectNextCel.top + cellHeight - mTimeCelOffset;
	rectTemp = rectTime;
	int h = rectTemp.Height();
	UINT nDrawCount = 0;
	for (itrt = timeList.begin(); itrt != timeList.end(); itrt++){
		CTime starttime((*itrt).start);
		int x = CalculateTimeToX(startTime, rectTime, starttime);
		if (x < labelWidth)
			x = labelWidth;
		rectTemp.left = x;
		CTime stoptime((*itrt).end);
		x = CalculateTimeToX(startTime, rectTime, stoptime);
		if (x < labelWidth)
			x = labelWidth;
		rectTemp.right = x;

		if (rectTemp.left == rectTemp.right){
			mItemMaxCount++;
			continue;
		}

		if (mItemMaxCount == mForecastSelect)
			drawTimeline(memDC, rectTemp, mOpeColorTable[eColor_UsedForecast]);
		else
			drawTimeline(memDC, rectTemp, (*itrt).color);
		memDC.DrawText((*itrt).label, rectTemp, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

		rectTemp.top = rectTemp.bottom + mTimeNextOffset;
		rectTemp.bottom = rectTemp.top + h;
		mItemMaxCount++;
		nDrawCount++;
		TRACE("## pred [%s] : %s - %s\n", CStringA((*itrt).label), CStringA(starttime.Format("%Y-%m-%d %H:%M:%S")), CStringA(stoptime.Format("%Y-%m-%d %H:%M:%S")));
	}

	if (nDrawCount != 0){
		int size = (int)nDrawCount;
		rectNextCel.bottom = rectNextCel.top + (size*h) + (mTimeCelOffset * 2) + ((size - 1)*mTimeNextOffset);
	}
	else{
		int size = 1;
		rectNextCel.bottom = rectNextCel.top + (size*h) + (mTimeCelOffset * 2) + ((size - 1)*mTimeNextOffset);
	}
	//if (timeList.size() != 0){
	//	int size = (int)timeList.size();
	//	rectNextCel.bottom = rectNextCel.top + (size*h) + (mTimeCelOffset * 2) + ((size - 1)*mTimeNextOffset);
	//}
	//else{
	//	int size = 1;
	//	rectNextCel.bottom = rectNextCel.top + (size*h) + (mTimeCelOffset * 2) + ((size - 1)*mTimeNextOffset);
	//}

	rectTemp = rectNextCel;
	rectTemp.right = rectTemp.left + labelWidth;
	// ラベル名の描画（「予報値」）
	memDC.DrawText(_T("予報値"), rectTemp, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	mItemMaxPoint = rectNextCel.bottom;

	SetDisplaySize(rectClient.right, rectTemp.bottom/*rectLabel.bottom*/);

	pen.DeleteObject();

	if (oldPen != NULL){
		memDC.SelectObject(oldPen);
	}

	if (oldFont != NULL){
		memDC.SelectObject(oldFont);
	}

	//UpdateScrollInfo();
}

int CTimeLine::CalculateTimeToX(CTime startTime, CRect startRect, CTime time)
{
	CRect rectClient;
	GetClientRect(rectClient);

	if (startTime > time)
		return -1;

	CTimeSpan sp = time - startTime;
	DWORD totalsec = (DWORD)((sp.GetDays() * 24 * 60 * 60) + (sp.GetHours() * 60 * 60) + (sp.GetMinutes() * 60) + sp.GetSeconds());
	//DWORD rangesec = (((mTimeScale[mTimeScaleIndex] / 3) + mTimeScale[mTimeScaleIndex]) * 60 * 60);
	DWORD rangesec = mTimeRange[mTimeScaleIndex] * 60 * (MAX_TIMEDIV - 1);
	DWORD totalwidth = startRect.Width();
	DWORD totalheight = startRect.Height();

	int nX = (int)(((float)totalwidth * (float)totalsec) / (float)rangesec);

	return (startRect.left+nX);
}

int CTimeLine::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	int status = CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);

	SetFocus();

	return status;
}

void CTimeLine::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
#if 1
	CRect rectClient;
	GetClientRect(rectClient);
	if (mHeader == true){
		CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
		return;
	}

	const int lineOffset = 20;
	int deltaPos = 0;

	switch (nSBCode) {

	case SB_LINEUP:
		deltaPos = -lineOffset;
		break;

	case SB_LINEDOWN:
		deltaPos = lineOffset;
		break;

	case SB_PAGEUP:
		deltaPos = -m_pageSize.cy;
		break;

	case SB_PAGEDOWN:
		deltaPos = m_pageSize.cy;
		break;

	case SB_THUMBTRACK:
		deltaPos = GetScrollPosEx(SB_VERT, pScrollBar) - m_scrollPos.cy;
		break;

	case SB_THUMBPOSITION:
		deltaPos = GetScrollPosEx(SB_VERT, pScrollBar) - m_scrollPos.cy;
		break;

	default:
		return;
	}

	CRect rect;
	GetClientRectScroll(rect);
	CSize windowSize(rect.Width(), rect.Height());
	if (windowSize.cy >= m_displaySize.cy){
		CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
		return;
	}

	// スクロール位置の算出.
	int newScrollPos = m_scrollPos.cy + deltaPos;

	if (newScrollPos < 0)
		deltaPos = -m_scrollPos.cy;

	int maxScrollPos = m_displaySize.cy - m_pageSize.cy;
	if (newScrollPos > maxScrollPos)
		deltaPos = maxScrollPos - m_scrollPos.cy;

	if (deltaPos != 0){
		m_scrollPos.cy += deltaPos;
		SetScrollPos(SB_VERT, m_scrollPos.cy, TRUE);
		ScrollWindow(0, -deltaPos);
	}

	Invalidate();
	UpdateScrollInfo();
	return;
#else

	CRect rectClient;
	GetClientRect(rectClient);
	if (mHeader == true){
		CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
		return;
	}

	int count = (int)mListPredData.size();
	if (count == 0){
		CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
		return;
	}

	int h = mItemMaxPoint / count;

	switch (nSBCode) {

	case SB_LINEUP:
	case SB_PAGEUP:
		if (mItemPos == 0)
			return;
		mItemPos -= h;
		if (mItemPos < 0)
			mItemPos = 0;
		break;

	case SB_LINEDOWN:
	case SB_PAGEDOWN:
		mItemPos += h;
		if (mItemPos >(mItemMaxPoint - h))
			mItemPos = (mItemMaxPoint-h);
		break;

	case SB_THUMBTRACK:
		mItemPos = nPos;
		if (mItemPos >(mItemMaxPoint - h))
			mItemPos = (mItemMaxPoint - h);
		if (mItemPos < 0)
			mItemPos = 0;
		break;
	default:
		return;
	}

	Invalidate();
	UpdateScrollInfo();

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
#endif // 1
}

/*============================================================================*/
/*! タイムライン

-# スクロール情報の更新

@param
@retval

*/
/*============================================================================*/
void CTimeLine::UpdateScrollInfo()
{
#if 1
	if (mHeader == true)
		return;

	CRect rect;
	GetClientRectScroll(rect);
	CSize windowSize(rect.Width(), rect.Height());

	if (windowSize.cy >= m_displaySize.cy){
		return;
	}

	CSize deltaPos(0, 0);

	int scrollMax = 0;
	deltaPos.cy = 0;
	if (windowSize.cy < m_displaySize.cy)
	{
		scrollMax = m_displaySize.cy - 1;
		if (m_pageSize.cy > 0 && m_scrollPos.cy > 0)
		{
			m_scrollPos.cy = (LONG)(1.0 * m_scrollPos.cy * windowSize.cy / m_pageSize.cy);
		}
		m_pageSize.cy = windowSize.cy;
		m_scrollPos.cy = min(m_scrollPos.cy, m_displaySize.cy - m_pageSize.cy - 1);
		deltaPos = ::GetScrollPos(m_hWnd, SB_VERT) - m_scrollPos.cy;
	}
	else
	{
		m_pageSize.cy = 0;
		m_scrollPos.cy = 0;
		deltaPos = ::GetScrollPos(m_hWnd, SB_VERT);
	}

	SCROLLINFO si;
	memset(&si, 0, sizeof(SCROLLINFO));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	si.nMin = 0;
	si.nMax = scrollMax;
	si.nPage = m_pageSize.cy;
	si.nPos = m_scrollPos.cy;
	SetScrollInfo(SB_VERT, &si, TRUE);

	if (deltaPos.cx != 0 || deltaPos.cy != 0)
	{
		ScrollWindow(deltaPos.cx, deltaPos.cy);
	}
	ShowScrollBar(SB_VERT, TRUE);
	return;
#else

	if (mHeader == true)
		return;

	if (mLabelCelHeight == 0)
		return;

	int count = (int)mListPredData.size();
	if (count == 0)
		return;

	int h = mItemMaxPoint / count;

	CRect rectClient;
	GetClientRect(rectClient);
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	TRY
	{
		si.fMask = SIF_ALL;
		si.nPos = (int)mItemPos;
		si.nMin = 0;
		si.nMax = mItemMaxPoint / h;
		si.nPage = mLabelCelHeight;
		si.nPage = h;
		mItemMaxCount = si.nMax;
	}CATCH_ALL(e){
		return;
	} END_CATCH_ALL

	SetScrollInfo(SB_VERT, &si, TRUE);
	ShowScrollBar(SB_VERT, TRUE);
	SendNewLimits();
#endif // 1
}
/*============================================================================*/
/*! タイムライン

-# 線端数制限を行う

@param
@retval

*/
/*============================================================================*/
void CTimeLine::SendNewLimits()
{
	return;
	GetParent()->PostMessage(WM_NEWVERTLIMITS, (WPARAM)0, (LPARAM)mItemMaxPoint);
}

void CTimeLine::SetDisplaySize(int displayWidth, int displayHeight)
{
	CSize s = m_displaySize;
	m_displaySize = CSize(displayWidth, displayHeight);

	if (s == m_displaySize)
		return;

	if (IsWindow(m_hWnd))
		UpdateScrollInfo();
}

const CSize& CTimeLine::GetDisplaySize() const
{
	return m_displaySize;
}

const CSize& CTimeLine::GetScrollPos() const
{
	return m_scrollPos;
}

const CSize& CTimeLine::GetPageSize() const
{
	return m_pageSize;
}

void CTimeLine::GetClientRectScroll(CRect& rect)
{
	CRect winRect;
	GetWindowRect(&winRect);
	ScreenToClient(&winRect);

	GetClientRect(&rect);

	int cxSB = ::GetSystemMetrics(SM_CXVSCROLL);
	int cySB = ::GetSystemMetrics(SM_CYHSCROLL);

	if (winRect.right >= (rect.right + cxSB))
		rect.right += cxSB;
	if (winRect.bottom >= (rect.bottom + cySB))
		rect.bottom += cySB;
}

int CTimeLine::GetScrollPosEx(int bar, CScrollBar* pScrollBar)
{
	HWND hWndScroll;
	if (pScrollBar == NULL)
		hWndScroll = m_hWnd;
	else
		hWndScroll = pScrollBar->m_hWnd;

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_TRACKPOS;
	::GetScrollInfo(hWndScroll, bar, &si);

	int scrollPos = si.nTrackPos;

	return scrollPos;
}


/*============================================================================*/
/*! タイムライン

-# 描画カラー属性の作成

@param
@retval

*/
/*============================================================================*/
void CTimeLine::createColors(stTimeLineColor &colors)
{
	colors.light.color = lightenColor(colors.base.color, 50);
	colors.lighter.color = lightenColor(colors.base.color, 50);
	colors.lightest.color = lightenColor(colors.base.color, 50);
	colors.dark.color = darkenColor(colors.base.color, 50);
	colors.darker.color = darkenColor(colors.base.color, 50);
	colors.darkShadow.color = ::GetSysColor(COLOR_3DDKSHADOW);
	colors.liteShadow.color = ::GetSysColor(COLOR_3DHIGHLIGHT);

	BYTE byRed3DDkShadow = GetRValue(colors.darkShadow.color);
	BYTE byRed3DLiteShadow = GetRValue(colors.liteShadow.color);
	BYTE byGreen3DDkShadow = GetGValue(colors.darkShadow.color);
	BYTE byGreen3DLiteShadow = GetGValue(colors.liteShadow.color);
	BYTE byBlue3DDkShadow = GetBValue(colors.darkShadow.color);
	BYTE byBlue3DLiteShadow = GetBValue(colors.liteShadow.color);

	colors.shadow.color = RGB(byRed3DLiteShadow + ((byRed3DDkShadow - byRed3DLiteShadow) >> 1),
		byGreen3DLiteShadow + ((byGreen3DDkShadow - byGreen3DLiteShadow) >> 1),
		byBlue3DLiteShadow + ((byBlue3DDkShadow - byBlue3DLiteShadow) >> 1));
}

/*============================================================================*/
/*! タイムライン

-# 描画カラー属性の作成（３Ｄ用）

@param
@retval

*/
/*============================================================================*/
COLORREF CTimeLine::lightenColor(const COLORREF crColor, BYTE blend)
{
	//return crColor;
	WORD byRed = (WORD)GetRValue(crColor);
	WORD byGreen = (WORD)GetGValue(crColor);
	WORD byBlue = (WORD)GetBValue(crColor);

	if ((byRed + blend) <= 255)
		byRed = (byRed + blend);
	if ((byGreen + blend) <= 255)
		byGreen = (byGreen + blend);
	if ((byBlue + blend) <= 255)
		byBlue = (byBlue + blend);

	return RGB((BYTE)byRed, (BYTE)byGreen, (BYTE)byBlue);
}

/*============================================================================*/
/*! タイムライン

-# 描画カラー属性の作成（３Ｄ用）

@param
@retval

*/
/*============================================================================*/
COLORREF CTimeLine::darkenColor(const COLORREF crColor, BYTE blend)
{
	//return crColor;
	WORD byRed = (WORD)GetRValue(crColor);
	WORD byGreen = (WORD)GetGValue(crColor);
	WORD byBlue = (WORD)GetBValue(crColor);

	if (byRed >= blend)
		byRed = (byRed - blend);
	if (byGreen >= blend)
		byGreen = (byGreen - blend);
	if (byBlue >= blend)
		byBlue = (byBlue - blend);

	return RGB((BYTE)byRed, (BYTE)byGreen, (BYTE)byBlue);
}
/*============================================================================*/
/*! タイムライン

-# 描画ペン属性の作成

@param
@retval

*/
/*============================================================================*/
void CTimeLine::createPens(stTimeLineColor &colors)
{
	deletePens(colors);

	colors.base.pen.CreatePen(PS_SOLID, 1, colors.base.color);
	colors.light.pen.CreatePen(PS_SOLID, 1, colors.light.color);
	colors.lighter.pen.CreatePen(PS_SOLID, 1, colors.lighter.color);
	colors.dark.pen.CreatePen(PS_SOLID, 1, colors.dark.color);
	colors.darker.pen.CreatePen(PS_SOLID, 1, colors.darker.color);
	colors.darkShadow.pen.CreatePen(PS_SOLID, 1, colors.darkShadow.color);
	colors.liteShadow.pen.CreatePen(PS_SOLID, 1, colors.liteShadow.color);
	colors.shadow.pen.CreatePen(PS_SOLID, 1, colors.shadow.color);
}

/*============================================================================*/
/*! タイムライン

-# 描画ペン属性の削除

@param
@retval

*/
/*============================================================================*/
void CTimeLine::deletePens(stTimeLineColor &colors)
{
	for (int i = 0; i<2; i++){
		if (colors.base.pen.m_hObject)
			colors.base.pen.DeleteObject();
		if (colors.light.pen.m_hObject)
			colors.light.pen.DeleteObject();
		if (colors.lighter.pen.m_hObject)
			colors.lighter.pen.DeleteObject();
		if (colors.dark.pen.m_hObject)
			colors.dark.pen.DeleteObject();
		if (colors.darker.pen.m_hObject)
			colors.darker.pen.DeleteObject();
		if (colors.darkShadow.pen.m_hObject)
			colors.darkShadow.pen.DeleteObject();
		if (colors.liteShadow.pen.m_hObject)
			colors.liteShadow.pen.DeleteObject();
		if (colors.shadow.pen.m_hObject)
			colors.shadow.pen.DeleteObject();
	}
}

/*============================================================================*/
/*! タイムライン

-# タイムラインの描画

@param		pDC		デバイスコンテキスト
@param		rect	描画領域
@param		coloor	色
@retval

*/
/*============================================================================*/
void CTimeLine::drawTimeline(CDC& memDC, const CRect rect, COLORREF color)
{
	if (rect.Width() == 0)
		return;

#if 1
	memDC.FillSolidRect(rect, color);
#else
	CDrawingManager dm(memDC);
	COLORREF dcolor = darkenColor(color, 50);
	COLORREF lcolor = lightenColor(color, 50);
	CRect rc = rect;
	rc.InflateRect(0, 1);
	dm.FillGradient(rc, dcolor, lcolor, TRUE);
#endif // 0

	CBrush brush(RGB(0,0,0));
	memDC.FrameRect(&rect, &brush);
}

void CTimeLine::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (mHeader == false){
		CMenu		pPopup;
		CMenu*		pPopupSub;
		if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE))
			return;

		pPopupSub = pPopup.GetSubMenu(0);
		if (pPopupSub == NULL){
			pPopup.DestroyMenu();
			return;
		}
		CPoint		pos;
		GetCursorPos(&pos);

		pPopupSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, theApp.m_pMainWnd);
		pPopup.DestroyMenu();
	}

	CWnd::OnRButtonDown(nFlags, point);
}
/*============================================================================*/
/*! 運用計画タイムライン

-# タイムラインデータの設定

@param		data		タイムラインデータ
@retval

*/
/*============================================================================*/
void CTimeLine::SetTimeLineData(COprPlnData data)
{
	if (mBitmap.GetSafeHandle()){
		mBitmap.DeleteObject();
	}
	mTimelineData = data;
	Invalidate();
	UpdateScrollInfo();
}
/*============================================================================*/
/*! 運用計画タイムライン

-# タイムラインデータのクリア

@param
@retval

*/
/*============================================================================*/
void CTimeLine::ClearTimeLineData()
{
	if (mBitmap.GetSafeHandle()){
		mBitmap.DeleteObject();
	}
	mTimelineData = mClearTimelineData;
	Invalidate();
	UpdateScrollInfo();
}


//=============================================================================
// CTimeGroupBoxEx
//=============================================================================

IMPLEMENT_DYNAMIC(CTimeGroupBoxEx, CButton)

CTimeGroupBoxEx::CTimeGroupBoxEx()
{
	//m_colorText = RGB(0, 0, 0);
	//m_colorBack = RGB(191, 219, 255);
	m_colorText = RGB(0, 0, 0);
	m_colorBack = ::GetSysColor(COLOR_BTNFACE);
	m_typeGroup = eGroupType_Normal;
	m_pointRound = CPoint(20, 20);
	m_bFontBold = false;
}

CTimeGroupBoxEx::~CTimeGroupBoxEx()
{
}


BEGIN_MESSAGE_MAP(CTimeGroupBoxEx, CButton)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CTimeGroupBoxEx メッセージ ハンドラ

void CTimeGroupBoxEx::PreSubclassWindow()
{
	GetWindowText(m_strText);
	GetWindowRect(m_rect);
	ScreenToClient(m_rect);

	CButton::PreSubclassWindow();
}

void CTimeGroupBoxEx::UpdateRect()
{
	CRect rect;
	GetClientRect(rect);
	m_rect.right = rect.right;
	m_rect.bottom = rect.bottom;
	// 描画
	Invalidate();
}

void CTimeGroupBoxEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	// 描画メッセージで CButton::OnPaint() を呼び出さないでください。

	int saveDC = dc.SaveDC();

	GetWindowText(m_strText);

	CString str;
	str = m_strText;

	CFont* oldFont = NULL;
	CFont defFont, txtFont;
	if (m_typeGroup != eGroupType_None){
		// テキストの描画
		// 文字フォントの作成
		defFont.CreateStockObject(DEFAULT_GUI_FONT);
		LOGFONT lf;
		if (defFont.GetLogFont(&lf)){
			lf.lfWeight = FW_NORMAL;
			if (m_typeGroup != eGroupType_Normal && m_typeGroup != eGroupType_Ribbon)
				lf.lfHeight = 14;
			if (m_bFontBold == true)
				lf.lfWeight = FW_BOLD;

			txtFont.DeleteObject();
			txtFont.CreateFontIndirect(&lf);
			oldFont = dc.SelectObject(&txtFont);
		}
	}

	CSize size = dc.GetTextExtent(str, str.GetLength());

	// テキストなしの矩形をベースに設定する
	CPoint ptStart, ptEnd;
	ptStart.x = m_rect.left;
	ptStart.y = m_rect.top + (size.cy / 2);
	ptEnd.x = ptStart.x + size.cx;
	ptEnd.y = m_rect.top + (size.cy / 2);
	if (m_typeGroup == eGroupType_Normal || m_typeGroup == eGroupType_Ribbon){
		ptStart.x = m_rect.left + (MAX_TIMEDIV-1);
		ptEnd.x = ptStart.x + size.cx;
	}

	CPen penShadow(PS_SOLID | PS_GEOMETRIC | PS_ENDCAP_SQUARE, 1, ::GetSysColor(COLOR_3DSHADOW));
	CPen penHlight(PS_SOLID | PS_GEOMETRIC | PS_ENDCAP_SQUARE, 1, ::GetSysColor(COLOR_3DHIGHLIGHT));

	// 矩形の描画
	CPen *oldPen;
	if (m_typeGroup == eGroupType_None || m_typeGroup == eGroupType_Normal){
		ptEnd.x = (m_rect.right > ptEnd.x) ? ptEnd.x : m_rect.right;
		dc.FillSolidRect(CRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom), ::GetSysColor(COLOR_3DFACE));
		dc.FillSolidRect(CRect(ptStart.x, m_rect.top, ptEnd.x, m_rect.top + size.cy), m_colorBack);
		dc.DrawEdge(CRect(m_rect.left, ptStart.y, m_rect.right, m_rect.bottom), EDGE_ETCHED, BF_RECT);
	}

	if (m_typeGroup != eGroupType_None){
		// テキストの描画
		// テキスト背景の描画
		CPen	pen(PS_SOLID, 1, m_colorBack);
		CBrush	brush(m_colorBack);
		oldPen = dc.SelectObject(&pen);
		CBrush *oldBrush = dc.SelectObject(&brush);

		if (m_typeGroup == eGroupType_Round){
			dc.FillSolidRect(m_rect, ::GetSysColor(COLOR_3DFACE));
			dc.RoundRect(CRect(m_rect.left, m_rect.top, m_rect.right + 1, m_rect.bottom + 1), m_pointRound);
		}
		else if (m_typeGroup == eGroupType_Box){
			dc.FillSolidRect(CRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom), ::GetSysColor(COLOR_3DFACE));
			dc.FillSolidRect(CRect(m_rect.left, m_rect.top, m_rect.right + 1, m_rect.bottom + 1), m_colorBack);
		}
		else if (m_typeGroup == eGroupType_BoxLine){
			dc.FillSolidRect(CRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom), ::GetSysColor(COLOR_3DFACE));
			dc.FillSolidRect(CRect(m_rect.left, m_rect.top, m_rect.right + 1, m_rect.bottom + 1), m_colorBack);
			dc.DrawEdge(CRect(m_rect.left, ptStart.y, m_rect.right, m_rect.bottom), EDGE_ETCHED, BF_RECT);
		}
		else if (m_typeGroup == eGroupType_Ribbon){
			dc.FillSolidRect(CRect(m_rect.left, m_rect.bottom - 22, m_rect.right - 2, m_rect.bottom - 2), m_colorBack);
			dc.DrawEdge(CRect(m_rect.left, ptStart.y, m_rect.right, m_rect.bottom), EDGE_ETCHED, BF_RECT);
		}

		// テキストの描画
		if (m_typeGroup != eGroupType_Normal && m_typeGroup != eGroupType_Ribbon){
			dc.SetTextColor(m_colorText);
			dc.SetBkColor(m_colorBack);
			ptStart.x = m_rect.left;
			ptEnd.x = m_rect.right;
			dc.DrawText(str, CRect(ptStart, ptEnd), DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_NOCLIP);
		}
		else if (m_typeGroup == eGroupType_Ribbon){
			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor(m_colorText);
			ptStart.x = m_rect.left;
			ptEnd.x = m_rect.right;
			ptStart.y = m_rect.bottom - 22;
			ptEnd.y = m_rect.bottom - 2;
			dc.DrawText(str, CRect(ptStart, ptEnd), DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_NOCLIP);
		}
		else{
			dc.SetTextColor(m_colorText);
			dc.SetBkColor(m_colorBack);
			//dc.SetBkMode(TRANSPARENT);
			ptEnd.x = m_rect.right;
			dc.DrawText(str, CRect(ptStart, ptEnd), DT_VCENTER | DT_LEFT | DT_SINGLELINE | DT_NOCLIP | DT_END_ELLIPSIS);
		}
		dc.SelectObject(oldPen);
		dc.SelectObject(oldBrush);
		pen.DeleteObject();
		brush.DeleteObject();

		if (oldFont != NULL){
			dc.SelectObject(oldFont);
			txtFont.DeleteObject();
		}
	}
	penShadow.DeleteObject();
	penHlight.DeleteObject();

	dc.RestoreDC(saveDC);
}

// CNotesButton

IMPLEMENT_DYNAMIC(CNotesButton, CButton)

CNotesButton::CNotesButton()
{
	mNotesType = eColor_Background;
}

CNotesButton::~CNotesButton()
{
}


BEGIN_MESSAGE_MAP(CNotesButton, CButton)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CNotesButton::OnNMCustomdraw)
END_MESSAGE_MAP()



// CNotesButton メッセージ ハンドラー


void CNotesButton::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	CString str;
	GetWindowText(str);
	CRect rect;
	GetClientRect(rect);

	if (pNMCD->dwDrawStage == CDDS_PREPAINT){
		CDC *pDC = CDC::FromHandle(pNMCD->hdc);
		rect = CRect(pNMCD->rc);
		pDC->SetBkMode(TRANSPARENT);
		//pDC->FillSolidRect(rect, ::GetSysColor(COLOR_BTNFACE));
		rect.left -= 2;
		rect.right = rect.left + rect.Height();
		rect.DeflateRect(2, 2);
		pDC->FillSolidRect(rect, mOpeColorTable[mNotesType]);
		rect = CRect(pNMCD->rc);
		rect.left = rect.Height();
		pDC->DrawText(str, rect, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER | DT_END_ELLIPSIS);
		*pResult = CDRF_SKIPDEFAULT;
	}

	//*pResult = 0;
}

