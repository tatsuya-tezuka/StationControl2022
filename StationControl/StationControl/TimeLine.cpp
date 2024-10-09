// TimeLine.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TimeLine.h"
#include "InputOccupancy.h"
#include "InputVlbi.h"
#include "..\Utility\SshShell.h"


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
	mAutoScrol = true;
	mForward = false;
	mNext = false;
	mLabelCelTopPos = 0;
	mHeader = false;
	mRedrawCursorPos = 0;
	mTimeBreak = true;
	memset(&mStartTime, 0, sizeof(SYSTEMTIME));
	mTimeLineMaxHeight = 0;
	mSelectBase = 0;
	mSyncTimeLine = NULL;
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

	if (m_DetailDlg.GetSafeHwnd() != NULL){
		m_DetailDlg.DestroyWindow();
	}
}


BEGIN_MESSAGE_MAP(CTimeLine, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CTimeLine メッセージ ハンドラー
#include <algorithm>
static bool procSort(const stForecastData& left, const stForecastData& right)
{
	CString str1(left.satellite);
	CString str2(right.satellite);
	return str1.CompareNoCase(str2) < 0;
}
static bool procStationSort(const stTimeLine& left, const stTimeLine& right)
{
	return left.stationID < right.stationID;
}
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

	//// 予報値からタイムラインデータを作成する
	//mForecastDataList.clear();
	//stForecastData data;
	//vector<stPredListData>& predlist = theApp.GetSatelliteData().GetPredList();
	//vector<stPredListData>::iterator itr;
	//for (itr = predlist.begin(); itr != predlist.end(); itr++){
	//	wsprintf(data.satellite, _T("%s"), (*itr).strSatellite);
	//	wsprintf(data.passId, _T("%s"), (*itr).strPassId);
	//	data.aos = (*itr).tAOS;
	//	data.los = (*itr).tLOS;
	//	data.maxelTime = (*itr).tMaxEl;
	//	data.maxel = (*itr).dMaxEl;
	//	UINT id = theApp.GetSatelliteData().GetStationID((*itr).strStationName);
	//	if (id == 0xffffffff)
	//		continue;
	//	if (theApp.GetSelectStationMode() == eStation_1){
	//		if (id != theApp.GetSelectStation())
	//			continue;
	//	}
	//	mForecastDataList.push_back(data);
	//}
	//std::sort(mForecastDataList.begin(), mForecastDataList.end(), procSort);

	// 文字描画用フォントの作成
	CFont defFont;
	defFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (defFont.GetLogFont(&lf)){
		if ((mHeader == true)){
			lf.lfWeight = FW_BOLD;
			lf.lfHeight = 12;
		}
		else{
			lf.lfWeight = FW_NORMAL;
			lf.lfHeight = 12;
		}
		mTxtFont.DeleteObject();
		mTxtFont.CreateFontIndirect(&lf);
		lf.lfWeight = FW_NORMAL;
		lf.lfHeight = 12;
		mTimeFont.DeleteObject();
		mTimeFont.CreateFontIndirect(&lf);
	}

	// スクロール情報の更新
	//UpdateScrollInfo();

	return result;
}

/*============================================================================*/
/*! グラフ描画

-# 予報値データのソート

@param	なし
@retval	なし

*/
/*============================================================================*/
void CTimeLine::SortForecastData()
{
	std::sort(mForecastDataList.begin(), mForecastDataList.end(), procSort);
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

	// 描画領域縦の算出
	CSize txtSize = memDC.GetTextExtent(_T("W"));
	int maxheight = rectClient.Height() + m_scrollPos.cy;
	if (mTimeDataList.size() == 0){
		mTimeLineMaxHeight = rectClient.Height() * _BACKHEIGHT_RATE;
	}
	else{
		mTimeLineMaxHeight = rectClient.Height() * _BACKHEIGHT_RATE;
		vector<stTimeData>::iterator itr;
		int h = 0;
		for (itr = mTimeDataList.begin(); itr != mTimeDataList.end(); itr++){
			map< __int64, vector<stTimeLine> >::iterator itrm;
			for (itrm = (*itr).item.begin(); itrm != (*itr).item.end(); itrm++){
				h += (txtSize.cy * 2);
			}
		}
		mTimeLineMaxHeight = (mTimeLineMaxHeight<h) ? h : mTimeLineMaxHeight;
	}

	if (mTimeLineMaxHeight < maxheight)
	{
		mTimeLineMaxHeight = maxheight;
	}
	rectBack.bottom = mTimeLineMaxHeight;

	// 描画用ビットマップの作成
	mBitmap.CreateCompatibleBitmap(&dc, rectBack.Width(), rectBack.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&mBitmap);

	// バックグラウンド モードを設定
	memDC.SetBkMode(TRANSPARENT);
	// 背景色の設定
	memDC.SetBkColor(m_ColorBk);

	// 全体を塗りつぶし
	memDC.FillSolidRect(rectBack, m_ColorBk);

	CTime ctime = mCurrentTime;
	if (mForward == true){
		mForward = false;
		CTimeSpan sp(0, 0, 0, mTimeScale[mTimeScaleIndex] * 60 * 60 / 2);
		mCurrentTime -= sp;
		mCurrentTime.GetAsSystemTime(mStartTime);
	}
	else if (mNext == true){
		mNext = false;
		CTimeSpan sp(0, 0, 0, mTimeScale[mTimeScaleIndex] * 60 * 60 / 2);
		mCurrentTime += sp;
		mCurrentTime.GetAsSystemTime(mStartTime);
	}

#ifdef _TIMELINE_OLD // ★
	drawTimeLine(memDC);
#else
	int save = m_displaySize.cy;
	drawTimeLine(memDC);
	if (save != m_displaySize.cy){
		m_scrollPos.cy = 0;
	}
#endif

	dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0, m_scrollPos.cy/*0*/, SRCCOPY);

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
	const COLORREF mAutoOnColor = RGB(255, 0, 0);
	const COLORREF mAutoOffColor = RGB(50, 50, 255);

	CRect rectClient, rect;
	GetClientRect(rectClient);

	// 文字描画用フォントの作成
	CFont *oldFont = NULL;
	oldFont = memDC.SelectObject(&mTxtFont);

	// 現在のフォントから文字サイズを取得する
	CSize txtSize = memDC.GetTextExtent(_T("WWWWWWWWWWWWWWWWWWWW"));
	CSize timeSize = memDC.GetTextExtent(_T("WWWWWWWWWW"));

	int labelWidth = _FIXEDLABEL_WIDTH;// txtSize.cx;
	int cellHeight = txtSize.cy * 2;

	if (mHeader == true){
		cellHeight = rectClient.Height();
	}
	else{
		rectClient.bottom = mTimeLineMaxHeight;
	}

	rect = rectClient;
	rect.left += labelWidth;

	int timeWidth = rect.Width() / (MAX_TIMEDIV - 1);
	int offsetWidth = (rect.Width() - (timeWidth * (MAX_TIMEDIV - 1)));

	// 現在時刻を求める
	CTime curtime = mCurrentTime;

	CTime ctime(mStartTime.wYear, mStartTime.wMonth, mStartTime.wDay, mStartTime.wHour, (mStartTime.wMinute / mTimeRange[mTimeScaleIndex]) * mTimeRange[mTimeScaleIndex], 0);
	CTime startTime, stopTime;
	CTimeSpan spTime(0, 0, mTimeRange[mTimeScaleIndex], 0);

	// 現在時刻から開始時刻を求める
	ctime -= spTime;
	startTime = ctime;
	CRect startRect = CRect(rect.left + labelWidth, rect.top, rect.left + (timeWidth * (MAX_TIMEDIV - 1)) - offsetWidth, rect.bottom);

	rect.right = rect.left + (timeWidth * (MAX_TIMEDIV - 1));
	int x = CalculateTimeToX(startTime, rect, curtime);
	if (x < 0)
		return -1;

	int retTimePos;
	retTimePos = x;

	if (retTimePos >= mRedrawCursorPos){
		// 80%までカーソルがきたら移動を行う
		mTimeBreak = true;
	}

	if (mHeader == false){
		CPen		pen;
		COLORREF	cf = mAutoOnColor;
		COLORREF	cfs = RGB(150, 150, 150);
		int nPenStyle = PS_SOLID;
		if (mAutoScrol == false){
			cf = mAutoOffColor;
			nPenStyle = PS_DOT;
		}
		if (pen.CreatePen(nPenStyle, 1, cfs)){
			CPen* pOldPen = memDC.SelectObject(&pen);
			memDC.MoveTo(x + 1, rectClient.top);
			memDC.LineTo(x + 1, rectClient.bottom);
			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}
		if (pen.CreatePen(nPenStyle, 1, cf)){
			CPen* pOldPen = memDC.SelectObject(&pen);
			memDC.MoveTo(x, rectClient.top);
			memDC.LineTo(x, rectClient.bottom);
			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}
	}

	// 三角形描画
	CPoint pts[4];
	pts[0].x = x;
	pts[0].y = rectClient.bottom - 1;// top + (cellHeight * 2);
	pts[1].x = x + 3;
	pts[1].y = rectClient.bottom - 4;// top + (cellHeight * 2) - 5;
	pts[2].x = x - 3;
	pts[2].y = rectClient.bottom - 4;// top + (cellHeight * 2) - 5;
	pts[3].x = x;
	pts[3].y = rectClient.bottom - 1;// top + (cellHeight * 2);

	CPen penOn(PS_SOLID, 1, mAutoOnColor);
	CPen penOff(PS_SOLID, 1, mAutoOffColor);
	CBrush	brushOn(mAutoOnColor);
	CBrush	brushOff(mAutoOffColor);
	CPen* oldPen = memDC.SelectObject(&penOn);
	if (mAutoScrol == false){
		memDC.SelectObject(&penOff);
	}
	CBrush *oldBrush = memDC.SelectObject(&brushOn);
	if (mAutoScrol == false){
		memDC.SelectObject(&brushOff);
	}
	if (mHeader == true){
		memDC.Polygon(pts, 4);
	}
	if (oldBrush != NULL){
		memDC.SelectObject(oldBrush);
	}
	brushOn.DeleteObject();
	brushOff.DeleteObject();
	if (oldPen != NULL){
		memDC.SelectObject(oldPen);
	}
	penOn.DeleteObject();
	penOff.DeleteObject();
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

	const int mTimeRectdef = 5;

	// 文字描画用フォントの作成
	CFont *oldFont = NULL;
	oldFont = memDC.SelectObject(&mTxtFont);

	// 線描画用ペンの作成
	CPen *oldPen = NULL;
	oldPen = memDC.SelectObject(&mColors[eColor_Grid].base.pen);

	// 現在のフォントから文字サイズを取得する
	CSize txtSize = memDC.GetTextExtent(_T("WWWWWWWWWWWWWWWWWWWW"));
	CSize timeSize = memDC.GetTextExtent(_T("WWWWWWWWWW"));

	int labelWidth = _FIXEDLABEL_WIDTH;// txtSize.cx;
	int cellHeight = txtSize.cy * 2;

	if (mHeader == true){
		cellHeight = rectClient.Height();
	}
	else{
		rectClient.bottom = mTimeLineMaxHeight;
	}
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

	int timeWidth = rect.Width() / (MAX_TIMEDIV - 1);
	int timeDrawWidth = timeWidth;
	int offsetWidth = (rect.Width() - (timeWidth * (MAX_TIMEDIV - 1)));

	CTime ctime(mStartTime.wYear, mStartTime.wMonth, mStartTime.wDay, mStartTime.wHour, (mStartTime.wMinute / mTimeRange[mTimeScaleIndex]) * mTimeRange[mTimeScaleIndex], 0);

	CTime startTime, stopTime;
	CTimeSpan spTime(0, 0, mTimeRange[mTimeScaleIndex], 0);
	CTimeSpan spHalfTime(0, 0, mTimeRange[mTimeScaleIndex] / 2, 0);
	//TRACE("## BaseTime : %s\n", CStringA(ctime.Format(_T("%H:%M:%S"))));

	// 現在時刻から開始時刻を求める
	ctime -= spTime;
	startTime = ctime;
	//TRACE("## StartTime : %s\n", CStringA(startTime.Format(_T("%H:%M:%S"))));
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
	if (mHeader == true){
		rect = rectClient;
		rect.right = rect.left + labelWidth;
		rect.bottom = rect.top + (cellHeight);
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
		if (item == _TIMEBREAK)
			mRedrawCursorPos = rect.left;

		if (mHeader == true){
			// 時間文字列の描画
			CString sdate = ctime.Format(_T("%Y-%m-%d"));
			CString stime = ctime.Format(_T("%H:%M:%S"));
			CRect r = rect;
			r.left = r.left - (timeDrawWidth / 2);
			r.right = r.left + timeDrawWidth;
			r.bottom = r.top + (cellHeight / 2);
			memDC.DrawText(sdate, r, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			r.top = r.bottom;
			r.bottom = rect.top + cellHeight;
			memDC.DrawText(stime, r, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		}
		ctime += spTime;
		rect.left = rect.left + timeDrawWidth;
	}

	CRect rectLabel, rectTemp;
	if (mHeader == true){
		// 時間セルの全体領域
		rectTime.left = rectClient.left + labelWidth;
		rectTime.right = rectTime.left + (timeWidth * (MAX_TIMEDIV - 1));
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
		return;
	}

	cellHeight += 10;

	memDC.SelectObject(&mColors[eColor_GridCell].base.pen);
	// 文字用ペンの作成
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	memDC.SelectObject(&pen);
	memDC.SelectObject(&mTimeFont);
	UINT row = 0;
	stDrawRect drect;

	if (theApp.GetSelectStationMode() == eStation_5){
		// ■4局モード時の描画
		drawTimeLineBaee(memDC);
		return;
	}

	//==========================================================
	// 監視の取得
	const CString mPLAN_NO = _T("CTRL.PASS_NO");// 計画番号
	const CString mEISEI_NAME = _T("CTRL.SAT_NAME"); //衛星名
	const CString mPLAN_STATUS = _T("CTRL.PASS_FLAG"); //運用状態
	const CString mPLAN_STATUS_DETAIL = _T("CTRL.PASS_INFO"); //運用状態詳細

	const CString mINPASS_STATUS = _T("パス中"); //計画登録
	const CString mOUTPASS_STATUS = _T("パス外"); //計画登録
	const CString mPLAN_REGIST = _T("計画登録"); //計画登録
	const CString mPLAN_INIT = _T("初期設定"); //初期設定
	const CString mRDNS_RNG = _T("ＲＮＧ校正");		// ＲＮＧ校正
	const CString mRDNS_AGC = _T("ＡＧＣ校正");		// ＡＧＣ校正
	const CString mRDNS_TLM = _T("テレメトリ伝送チェック");		// テレメトリ伝送チェック
	const CString mRDNS_CMD = _T("コマンド伝送チェック");		// コマンド伝送チェック
	const CString mOCCU_STATUS = _T("局設備占有時間");		// 局設備占有時間
	const CString mOCCU_STATION = _T("MAINT");		// 局占有

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	// 監視データから衛星名、計画IDを取得する
	CSatelliteData& sd = theApp.GetSatelliteData();
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), mEISEI_NAME);
	CString strCurrentSatName = _T("");
	if (obs_adr == NULL){
		// 衛星名が取得できない
#ifdef _DEBUG
		strCurrentSatName = _T("GEOTAIL");
#endif
	}
	else{
		strCurrentSatName.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), mPLAN_NO);
	CString strCurrentPlanID = _T("");
	if (obs_adr == NULL){
		// 計画IDが取得できない
#ifdef _DEBUG
		strCurrentPlanID = _T("200313-9999");
#endif
	}
	else{
		strCurrentPlanID.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), mPLAN_STATUS);
	UINT nCurPlanStatus = eKanshiPlanStatus_OutPass;	// パス外
	if (obs_adr == NULL){
		// 運用状態が取得できない
#ifdef _DEBUG
		nCurPlanStatus = eKanshiPlanStatus_InPass;	// パス中
#endif
	}
	else{
		CString strStatus = CString(obs_adr->sz_sts_name);
		if (strStatus.Compare(mINPASS_STATUS) == 0)
		{
			nCurPlanStatus = eKanshiPlanStatus_InPass;	// パス中
		}
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), mPLAN_STATUS_DETAIL);
	if (obs_adr == NULL){
		// 運用状態が取得できない
#ifdef _DEBUG
		//		nCurPlanStatus = eKanshiPlanStatus_PlanInit;
#endif
	}
	else{
		CString strStatus = CString(obs_adr->sz_sts_name);
		if (strStatus.Compare(mPLAN_REGIST) == 0)
		{
			nCurPlanStatus = eKanshiPlanStatus_PlanRegist;	// 計画登録
		}
		else if (strStatus.Compare(mPLAN_INIT) == 0)
		{
			nCurPlanStatus = eKanshiPlanStatus_PlanInit;	// 初期設定
		}
		else if (strStatus.Compare(mRDNS_RNG) == 0)
		{
			nCurPlanStatus = eKanshiPlanStatus_InPass;	// ＲＮＧ校正のため、パス中あつかい
		}
		else if (strStatus.Compare(mRDNS_AGC) == 0)
		{
			nCurPlanStatus = eKanshiPlanStatus_InPass;	// ＡＧＣ校正のため、パス中あつかい
		}
		else if (strStatus.Compare(mRDNS_TLM) == 0)
		{
			nCurPlanStatus = eKanshiPlanStatus_InPass;	// テレメトリ伝送チェックのため、パス中あつかい
		}
		else if (strStatus.Compare(mRDNS_CMD) == 0)
		{
			nCurPlanStatus = eKanshiPlanStatus_InPass;	// コマンド伝送チェックのため、パス中あつかい
		}

		// 局占有の場合で、かつ局設備占有時間
		if (strCurrentSatName == mOCCU_STATION && strStatus == mOCCU_STATUS)
		{
			nCurPlanStatus = eKanshiPlanStatus_InPass;
		}
	}

	//==========================================================

	// 現在時刻を求める
	CTime curtime = mCurrentTime;
	// ツールツップを削除する
	ClearToolTip();

	rectLabel = rectClient;

	CString saveSName = _T("");
	vector<stForecastData>::iterator itrf;
	rectLabel.top = rectLabel.top - cellHeight;

	rectLabel.bottom = rectLabel.top + cellHeight; // ★

	CRect recTemp;
	mLabelCelTopPos = -1;
	mOperationRect.clear();
	mForecastRect.clear();
	mSatelliteRect.clear();

	CGraphMutex::Lock(eTimeline);

	CRect rectTextCell = rectLabel;
	CRect outTime;
//++ タイムライン画面デバッグ >>>
//	int maxBottom;
//++ タイムライン画面デバッグ <<<
	for (itrf = mForecastDataList.begin(); itrf != mForecastDataList.end(); itrf++){
		//if (IsStationSatellite(theApp.GetSelectStation(), CString((*itrf).satellite)) == false)
		//	continue;
//++ タイムライン画面デバッグ >>>
//		maxBottom = -1;
//++ タイムライン画面デバッグ <<<
		if (saveSName != CString((*itrf).satellite)){
			rectLabel.top = rectLabel.top + cellHeight;
			rectLabel.bottom = rectLabel.top + cellHeight;
			rectTime.top = rectLabel.top;
			rectTime.bottom = rectTime.top + cellHeight;
			rectTime.DeflateRect(0, mTimeRectdef);
			saveSName = CString((*itrf).satellite);
			// 予報値の描画
			// VLBI,MAINTの特別処理
			if ((*itrf).flag == 0){
				// VLBI,MAINTではない通常の運用計画
				outTime = rectLabel;
				DrawForecast(memDC, (*itrf).satellite, _T(""), startTime, rectTime, outTime);
			}
			rectTextCell = rectLabel;

			// 運用計画
			if (mLabelCelTopPos < 0)
				mLabelCelTopPos = rectLabel.top;

			vector<stTimeData>::iterator itr;
			bool bDraw = false;
			for (itr = mTimeDataList.begin(); itr != mTimeDataList.end(); itr++){
				if (CString((*itr).satellite) != CString((*itrf).satellite))
					continue;
				map< __int64, vector<stTimeLine> >::iterator itrm;
//				UINT current = 0;
				for (itrm = (*itr).item.begin(); itrm != (*itr).item.end(); itrm++){

					//#rectTemp = rectTime;
					if ((*itrm).second.size() == 0){
						rectTemp = rectTime;
					}
					else{
						rectTemp = GetPredRect((*itrf).satellite, (*itrm).second[0].predid, (*itrm).second[0].stationID);
						if (rectTemp.IsRectEmpty()){
							rectTemp = rectTime;
						}
					}

//					if (current == 0){
//						rectLabel.top = rectTemp.top;
//						rectLabel.bottom = rectTemp.bottom;
//					}

					if (bDraw == true/* && IsDuplicatePlanTime((*itr).satellite, (*itrm).first, current) == true*/){
						__int64 pstart = 0, pstop = 0;
						pstop = GetPlanTotalTime((*itr).satellite, (*itrm).first, pstart);
						CTime starttime(pstart);
						CTime stoptime(pstop);
						int sx = CalculateTimeToX(startTime, rectTime, starttime);
						int ex = CalculateTimeToX(startTime, rectTime, stoptime);
						if (sx < 0)
							sx = rectTime.left;

						rectTemp.left = sx;
						rectTemp.right = ex;

						// 改行処理
						rectTemp = GetDrawPlanRect(rectTemp, cellHeight);
						/*
						rectLabel.top = rectLabel.top + cellHeight;
						rectLabel.bottom = rectLabel.top + cellHeight;
						rectTime.top = rectLabel.top;
						rectTime.bottom = rectTime.top + cellHeight;
						rectTime.DeflateRect(0, mTimeRectdef);
						rectTemp = rectTime;
						rectTextCell.bottom = rectLabel.bottom;
						*/
						bDraw = false;
					}
					// タイムラインの１行描画
					// 各部品ごとに描画する
					vector<stTimeLine>::iterator itrp;
					bool bText = true;
					__int64 pstart = 0, pstop = 0;
					pstop = GetPlanTotalTime((*itr).satellite, (*itrm).first, pstart);
					CString strDrawPlanID = _T("");
					CRect rectDrawPlanID;
					rectDrawPlanID.SetRectEmpty();
					COLORREF colorPlanID = RGB(255, 255, 255);
					for (itrp = (*itrm).second.begin(); itrp != (*itrm).second.end(); itrp++){
						UINT status = (*itrp).code;
//						UINT nStatusFromPlnFile = eKanshiPlanStatus_OutPass;
						{
							if ((*itrp).satellite == strCurrentSatName && (*itrp).planid == strCurrentPlanID){
								switch (nCurPlanStatus)
								{
								case eKanshiPlanStatus_InPass:
									// 実行中
									status = ePlanStatus_Execute;
									break;

								case eKanshiPlanStatus_PlanRegist:
									// 計画登録
									status = ePlanStatus_PlanRegist;
									break;

								case eKanshiPlanStatus_PlanInit:
									// 初期設定
									status = ePlanStatus_PlanInit;
									break;

								case eKanshiPlanStatus_OutPass:
									// パス外
								default:
									break;
								}
							}
							// 運用状態の更新
							(*itrp).code = status;

//							if (pstart <= curtime.GetTime()){
//								// 計画の全時間でチェック
//								nStatusFromPlnFile = ePlanStatus_Execute;
//							}
						}

						CTime starttime((*itrp).start);
						int sx = CalculateTimeToX(startTime, rectTime, starttime);
						if (sx < 0)
							sx = rectTime.left;
						rectTemp.left = sx;
						CTime stoptime((*itrp).end);
						int px = CalculateTimeToX(startTime, rectTime, stoptime);
						if (sx < 0 && px < 0)
							continue;
						if (px < 0)
							continue;
						rectTemp.right = px;

						// 運用計画矩形の描画
						bDraw = true;
						drawTimeline(memDC, rectTemp, (*itrp).color, status);
						//drawTimeline(memDC, rectTemp, mStationColor[(*itrp).stationID], status);

						// ツールチップ情報の登録
						CRect rectTool = rectTemp;
						rectTool.top -= m_scrollPos.cy;
						rectTool.bottom -= m_scrollPos.cy;
						AddToolTip((*itrp), rectTool);

						wsprintf(drect.satellite, _T("%s"), (*itr).satellite);
						wsprintf(drect.passId, _T("%s"), (*itrp).planid);
						drect.rect = rectTemp;

						drect.type = getOperationType((*itrp).opekind);

						if (status == ePlanStatus_Execute)
						{
							// 実行中の場合
							switch (drect.type)
							{
							case eLineType_Occu:
								// 局占有
								drect.type = eLineType_OccuRun;
								break;

							case eLineType_Vlbi:
								// VLBI
								drect.type = eLineType_VlbiRun;
								break;

							case eLineType_UnexecRdsn:
								// 校正診断
								drect.type = eLineType_execRdsn;
								break;

							default:
								// その他の運用計画
								drect.type = eLineType_Run;
								break;
							}
						}
						else if (status == ePlanStatus_PlanRegist)
						{
							// 計画登録の場合
							switch (drect.type)
							{
							case eLineType_Occu:
								// 局占有
								drect.type = eLineType_OccuRegist;
								break;

							case eLineType_Vlbi:
								// VLBI
								drect.type = eLineType_VlbiRegist;
								break;

							case eLineType_UnexecRdsn:
								// 校正診断
								// 何もしない
								break;

							default:
								// その他の運用計画
								drect.type = eLineType_PlanRegist;
								break;
							}
						}
						else if (status == ePlanStatus_PlanInit)
						{
							// 初期設定の場合
							switch (drect.type)
							{
							case eLineType_Occu:
								// 局占有
								drect.type = eLineType_OccuInit;
								break;

							case eLineType_Vlbi:
								// VLBI
								drect.type = eLineType_VlbiInit;
								break;

							case eLineType_UnexecRdsn:
								// 校正診断
								// 何もしない
								break;

							default:
								// その他の運用計画
								drect.type = eLineType_PlanInit;
								break;
							}
						}

//						if (drect.type == eLineType_UnexecRdsn && nStatusFromPlnFile == ePlanStatus_Execute)
//						{
//							// 校正診断で実行中の場合
//							drect.type = eLineType_execRdsn;
//						}

						drect.getform = (DWORD)(*itrp).getform;
						mOperationRect.push_back(drect);

						// 計画ID描画領域の更新
						if (rectDrawPlanID.IsRectEmpty()){
							rectDrawPlanID = rectTemp;
						}
						else{
							rectDrawPlanID.top = rectTemp.top;
							rectDrawPlanID.bottom = rectTemp.bottom;
							rectDrawPlanID.left = __min(rectTemp.left, rectDrawPlanID.left);
							rectDrawPlanID.right = __max(rectTemp.right, rectDrawPlanID.right);
						}
						if (bText == true){
							// 計画ID作成
							strDrawPlanID.Format(_T("%06d-%04d"), (int)((*itrm).first >> 32), (int)((*itrm).first & 0xffffffff));
							colorPlanID = (*itrp).textcolor;
							//COLORREF back = memDC.SetTextColor((*itrp).textcolor);
							//memDC.DrawText(str, rectTemp, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
							//memDC.SetTextColor(back);
							bText = false;
						}
					}
					if (strDrawPlanID.IsEmpty() == false){
						COLORREF back = memDC.SetTextColor(colorPlanID);
						memDC.DrawText(strDrawPlanID, rectDrawPlanID, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
						memDC.SetTextColor(back);
					}

//++ タイムライン画面デバッグ >>>
//					if (rectTemp.bottom > maxBottom)
//					{
//						// 一番下の行を記憶しておく
//						maxBottom = rectTemp.bottom;
//					}
//++ タイムライン画面デバッグ <<<
//					current++;
				}
			}//	for (itr = mTimeDataList.begin(); itr != mTimeDataList.end(); itr++){

			// 計画の描画領域拡張
			if (outTime.bottom < rectTemp.bottom + 5)
			{
				outTime.bottom = rectTemp.bottom + 5;
			}
//++ タイムライン画面デバッグ >>>
//			if (maxBottom < 0)
//			{
//				maxBottom = rectTemp.bottom;
//			}
//			if (outTime.bottom < maxBottom + 5)
//			{
//				outTime.bottom = maxBottom + 5;
//			}
//++ タイムライン画面デバッグ <<<

			// 予報値の描画領域と計画の描画領域を比較する
			if (rectLabel.bottom < outTime.bottom){
				rectLabel.bottom = outTime.bottom;
				rectLabel.top = outTime.bottom - cellHeight;
				rectTextCell.bottom = rectLabel.bottom;
			}

			// 衛星名の描画
			rectTextCell.right = rectTextCell.left + labelWidth;
			if (theApp.GetSelectStationMode() == eStation_1){
				memDC.DrawText(CString((*itrf).satellite), rectTextCell, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
				wsprintf(drect.satellite, _T("%s"), (LPCTSTR)CString((*itrf).satellite));
				drect.rect = rectTextCell;
				mSatelliteRect.push_back(drect);
			}
			else{
				rectTextCell.right = rectTextCell.left + rectTextCell.Width() / 2;
				memDC.DrawText(CString((*itrf).satellite), rectTextCell, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			}

			memDC.MoveTo(rectClient.left, rectLabel.bottom);
			memDC.LineTo(rectClient.right, rectLabel.bottom);
		}
	}
	CGraphMutex::Unlock(eTimeline);

	SetDisplaySize(rectClient.right, rectLabel.bottom);

	pen.DeleteObject();

	if (oldPen != NULL){
		memDC.SelectObject(oldPen);
	}

	if (oldFont != NULL){
		memDC.SelectObject(oldFont);
	}
}

/*============================================================================*/
/*! グラフ描画

-# タイムライン描画（ベース）

@param	memDC		描画用デバイスコンテキスト
@retval	なし

*/
/*============================================================================*/
void CTimeLine::drawTimeLineBaee(CDC& memDC)
{
	CRect rectClient, rect;
	GetClientRect(rectClient);

	const int mTimeRectdef = 5;

	// 文字描画用フォントの作成
	CFont *oldFont = NULL;
	oldFont = memDC.SelectObject(&mTxtFont);

	// 線描画用ペンの作成
	CPen *oldPen = NULL;
	oldPen = memDC.SelectObject(&mColors[eColor_Grid].base.pen);

	// 現在のフォントから文字サイズを取得する
	CSize txtSize = memDC.GetTextExtent(_T("WWWWWWWWWWWWWWWWWWWW"));
	CSize timeSize = memDC.GetTextExtent(_T("WWWWWWWWWW"));

	int labelWidth = _FIXEDLABEL_WIDTH;// txtSize.cx;
	int cellHeight = txtSize.cy * 2;

	if (mHeader == true){
		cellHeight = rectClient.Height();
	}
	else{
		rectClient.bottom = mTimeLineMaxHeight;
	}
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

	int timeWidth = rect.Width() / (MAX_TIMEDIV - 1);
	int timeDrawWidth = timeWidth;
	int offsetWidth = (rect.Width() - (timeWidth * (MAX_TIMEDIV - 1)));

	CTime ctime(mStartTime.wYear, mStartTime.wMonth, mStartTime.wDay, mStartTime.wHour, (mStartTime.wMinute / mTimeRange[mTimeScaleIndex]) * mTimeRange[mTimeScaleIndex], 0);

	CTime startTime, stopTime;
	CTimeSpan spTime(0, 0, mTimeRange[mTimeScaleIndex], 0);
	CTimeSpan spHalfTime(0, 0, mTimeRange[mTimeScaleIndex] / 2, 0);
	//TRACE("## BaseTime : %s\n", CStringA(ctime.Format(_T("%H:%M:%S"))));

	// 現在時刻から開始時刻を求める
	ctime -= spTime;
	startTime = ctime;
	//TRACE("## StartTime : %s\n", CStringA(startTime.Format(_T("%H:%M:%S"))));
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
		if (item == _TIMEBREAK)
			mRedrawCursorPos = rect.left;

		if (mHeader == true){
			// 時間文字列の描画
			CString sdate = ctime.Format(_T("%Y-%m-%d"));
			CString stime = ctime.Format(_T("%H:%M:%S"));
			CRect r = rect;
			r.left = r.left - (timeDrawWidth / 2);
			r.right = r.left + timeDrawWidth;
			r.bottom = r.top + (cellHeight / 2);
			memDC.DrawText(sdate, r, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			r.top = r.bottom;
			r.bottom = rect.top + cellHeight;
			memDC.DrawText(stime, r, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		}
		ctime += spTime;
		rect.left = rect.left + timeDrawWidth;
	}

	CRect rectLabel, rectTemp;
	if (mHeader == true){
		// 時間セルの全体領域
		rectTime.left = rectClient.left + labelWidth;
		rectTime.right = rectTime.left + (timeWidth * (MAX_TIMEDIV - 1));
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
		return;
	}

	mOperationRect.clear();
	mForecastRect.clear();
	mSatelliteRect.clear();

	cellHeight += 10;

	memDC.SelectObject(&mColors[eColor_GridCell].base.pen);
	// 文字用ペンの作成
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	memDC.SelectObject(&pen);
	memDC.SelectObject(&mTimeFont);
	CRect rectTextCell = rectLabel;
	CRect rectStationCell = rectTextCell;

	// ◆局名ベース
	// 局名ベース時は、運用計画一覧をベースに描画する
	if (mSelectBase == eBaseStation){
		// 現在時刻を求める
		CTime curtime = mCurrentTime;
		// ツールツップを削除する
		ClearToolTip();

		// rectLabelは最終的に描画領域全体を示し、スクロールバーの制御として使う
		rectLabel = rectClient;

		CRect recTemp;
		mLabelCelTopPos = -1;
		mOperationRect.clear();

		rectLabel.bottom = rectLabel.top + cellHeight;

		// 局名領域の初期化
		rectStationCell = rectLabel;
		rectStationCell.bottom = rectStationCell.top + cellHeight;
		rectStationCell.right = rectStationCell.left + (labelWidth / 2);

		// 衛星名領域の初期化
		rectTextCell = rectStationCell;
		rectTextCell.bottom = rectTextCell.top + cellHeight;
		rectTextCell.right = rectStationCell.left + labelWidth;
		rectTextCell.left = rectTextCell.right - rectTextCell.Width() / 2;

		// タイムライン領域の初期化
		rectTime.top = rectLabel.top;
		rectTime.bottom = rectTime.top + cellHeight;
		rectTime.DeflateRect(0, mTimeRectdef);

		// 局毎→衛星名毎にタイムラインデータを描画する
		for (int nStationNo = 0; nStationNo < eStation_MAX; nStationNo++){
			vector<stForecastData>::iterator itrf;
			CString saveSName = _T("");
			bool bDraw = false;
			CRect outTime;
			for (itrf = mForecastDataList.begin(); itrf != mForecastDataList.end(); itrf++){
				if (saveSName != CString((*itrf).satellite) && (*itrf).station == nStationNo){
					// 衛星名が変わったので全予報値と局毎のタイムラインデータを描画する
					// 予報値の描画
					bool bDrawFocast = false;
					bool bDrawTlmData = false;
					outTime = rectLabel;
					// VLBI,MAINTの特別処理
					if ((*itrf).flag == 0){
						bDrawFocast = DrawForecastStation(memDC, (*itrf).satellite, _T(""), startTime, rectTime, nStationNo, outTime);
					}
					// タイムラインデータの描画
					CRect outPlanTime = rectLabel;
					bDrawTlmData = DrawOperationStation(memDC, (*itrf).satellite, _T(""), startTime, rectTime, nStationNo, outPlanTime);

					if (bDrawFocast == true || bDrawTlmData == true)
					{
						// 領域の更新
						// 予報値の描画領域と計画の描画領域を比較する
						if (outTime.bottom < outPlanTime.bottom){
							rectLabel.bottom = outPlanTime.bottom;
							rectTextCell.bottom = outPlanTime.bottom;
							rectStationCell.bottom = outPlanTime.bottom;
						}
						else{
							rectLabel.bottom = outTime.bottom;
							rectTextCell.bottom = outTime.bottom;
							rectStationCell.bottom = outTime.bottom;
						}

						rectLabel.top = rectLabel.bottom;
						rectLabel.bottom = rectLabel.top + cellHeight;
						rectTime.top = rectLabel.top;
						rectTime.bottom = rectTime.top + cellHeight;
						rectTime.DeflateRect(0, mTimeRectdef);

						// 一つの衛星が終わったので衛星名をラベルに描画する
						if (theApp.GetSelectStationMode() == eStation_1){
							memDC.DrawText(CString((*itrf).satellite), rectTextCell, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
						}
						else{
							memDC.DrawText(CString((*itrf).satellite), rectTextCell, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
						}
						// 衛星名分割線の描画
						memDC.MoveTo(rectTextCell.left, rectTextCell.bottom);
						memDC.LineTo(rectTextCell.right, rectTextCell.bottom);
						rectStationCell.bottom = rectTextCell.bottom;

						// 衛星名領域の更新
						rectTextCell.top = rectTextCell.bottom;
						rectTextCell.bottom = rectTextCell.top + cellHeight;
					}
					else{
						TRACE("## NoSatellite:%s Station:%d\n", CStringA((*itrf).satellite), nStationNo);
					}

					saveSName = CString((*itrf).satellite);

					bDraw |= (bDrawFocast | bDrawTlmData);
				}
			}

			if (bDraw == true){
				// 一つの局が終わったので局名をラベルに描画する
				memDC.DrawText(mStationString[nStationNo], rectStationCell, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
				// 局分割線の描画
				memDC.MoveTo(rectStationCell.left, rectStationCell.bottom);
				memDC.LineTo(rectStationCell.right, rectStationCell.bottom);
				memDC.MoveTo(rectStationCell.right, rectStationCell.top);
				memDC.LineTo(rectStationCell.right, rectStationCell.bottom);

				// 局名領域の更新
				rectStationCell.top = rectStationCell.bottom;
				rectStationCell.bottom = rectStationCell.top + cellHeight;
			}
		}

		SetDisplaySize(rectClient.right, rectLabel.bottom);

		pen.DeleteObject();

		if (oldPen != NULL){
			memDC.SelectObject(oldPen);
		}

		if (oldFont != NULL){
			memDC.SelectObject(oldFont);
		}
		return;
	}

	// ◆衛星名ベース
	// 衛星名ベース時は、予報値一覧をベースに描画する
	// 予報値は衛星名順に並んでいるので、衛星名でブレークするまで

	// 現在時刻を求める
	CTime curtime = mCurrentTime;
	// ツールツップを削除する
	ClearToolTip();

	// rectLabelは最終的に描画領域全体を示し、スクロールバーの制御として使う
	rectLabel = rectClient;

	CString saveSName = _T("");

	CRect recTemp;
	mLabelCelTopPos = -1;
	mOperationRect.clear();

	rectLabel.bottom = rectLabel.top + cellHeight;

	// 衛星名領域の初期化
	rectTextCell = rectLabel;
	rectTextCell.bottom = rectTextCell.top + cellHeight;
	rectTextCell.right = rectTextCell.left + (labelWidth / 2);

	// 局名領域の初期化
	rectStationCell = rectTextCell;
	rectStationCell.bottom = rectStationCell.top + cellHeight;
	rectStationCell.right = rectTextCell.left + labelWidth;
	rectStationCell.left = rectStationCell.right - rectStationCell.Width() / 2;

	// タイムライン領域の初期化
	rectTime.top = rectLabel.top;
	rectTime.bottom = rectTime.top + cellHeight;
	rectTime.DeflateRect(0, mTimeRectdef);

	// 衛星名毎→局毎にタイムラインデータを描画する
	vector<stForecastData>::iterator itrf;
	CRect outTime;
	UINT saveStationNo = 0xffffffff;
	CString satTmp = _T("");
	CRect rectTmp;
	for (itrf = mForecastDataList.begin(); itrf != mForecastDataList.end(); itrf++){
		if (saveSName != CString((*itrf).satellite))
		{
			// 衛星名が変わったので局番をクリアする
			saveStationNo = 0xffffffff;
		}

		// 予報値の描画
		bool bDraw = false;
		for (int nStationNo = 0; nStationNo < eStation_MAX; nStationNo++){
			// 局が変わったので全予報値と局毎のタイムラインデータを描画する
			// VLBI, MAINTは局判定除外する
			if (saveStationNo != (*itrf).station && (*itrf).station == nStationNo){
				bool bDrawFocast = false;
				bool bDrawTlmData = false;
				outTime = rectLabel;
				// VLBI,MAINTの特別処理
				if ((*itrf).flag == 0){
					bDrawFocast = DrawForecastStation(memDC, (*itrf).satellite, _T(""), startTime, rectTime, nStationNo, outTime);
				}
				// タイムラインデータの描画
				CRect outPlanTime = rectLabel;
				bDrawTlmData = DrawOperationStation(memDC, (*itrf).satellite, _T(""), startTime, rectTime, nStationNo, outPlanTime);

				if (bDrawFocast == true || bDrawTlmData == true)
				{
					// 領域の更新
					// 予報値の描画領域と計画の描画領域を比較する
					if (outTime.bottom < outPlanTime.bottom){
						rectLabel.bottom = outPlanTime.bottom;
						rectTextCell.bottom = outPlanTime.bottom;
						rectStationCell.bottom = outPlanTime.bottom;
					}
					else{
						rectLabel.bottom = outTime.bottom;
						rectTextCell.bottom = outTime.bottom;
						rectStationCell.bottom = outTime.bottom;
					}

					rectLabel.top = rectLabel.bottom;
					rectLabel.bottom = rectLabel.top + cellHeight;
					rectTime.top = rectLabel.top;
					rectTime.bottom = rectTime.top + cellHeight;
					rectTime.DeflateRect(0, mTimeRectdef);

					// 一つの局が終わったので局名をラベルに描画する
					memDC.DrawText(mStationString[nStationNo], rectStationCell, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
					// 局分割線の描画
					memDC.MoveTo(rectStationCell.left, rectStationCell.bottom);
					memDC.LineTo(rectStationCell.right, rectStationCell.bottom);
					rectTextCell.bottom = rectStationCell.bottom;

					// 局名領域の更新
					rectStationCell.top = rectStationCell.bottom;
					rectStationCell.bottom = rectStationCell.top + cellHeight;
				}
				else{
					TRACE("## NoSatellite:%s Station:%d\n", CStringA((*itrf).satellite), nStationNo);
				}

				saveStationNo = (*itrf).station;

				bDraw |= (bDrawFocast | bDrawTlmData);
			}
		}

		if (satTmp.IsEmpty() == false && satTmp != CString((*itrf).satellite))
		{
			// 衛星名が変わったので全予報値と局毎のタイムラインデータを描画する
			// 一つの衛星が終わったので衛星名をラベルに描画する
			if (theApp.GetSelectStationMode() == eStation_1){
				memDC.DrawText(satTmp, rectTmp, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			}
			else{
				memDC.DrawText(satTmp, rectTmp, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			}
			// 衛星名分割線の描画
			memDC.MoveTo(rectTmp.left, rectTmp.bottom);
			memDC.LineTo(rectTmp.right, rectTmp.bottom);
			memDC.MoveTo(rectTmp.right, rectTmp.top);
			memDC.LineTo(rectTmp.right, rectTmp.bottom);

			// 衛星名領域の高さ更新
			rectTextCell.top = rectTmp.bottom;

			satTmp = _T("");
		}

		if (bDraw == true)
		{
			// 描画情報がある場合、衛星領域のデータをテンポラリに保持する
			rectTmp = rectTextCell;
			satTmp = CString((*itrf).satellite);
		}

		saveSName = CString((*itrf).satellite);
	}

	if (satTmp.IsEmpty() == false)
	{
		// 最後の衛星の描画
		// 衛星名をラベルに描画する
		if (theApp.GetSelectStationMode() == eStation_1){
			memDC.DrawText(satTmp, rectTmp, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		}
		else{
			memDC.DrawText(satTmp, rectTmp, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		}
		// 衛星名分割線の描画
		memDC.MoveTo(rectTmp.left, rectTmp.bottom);
		memDC.LineTo(rectTmp.right, rectTmp.bottom);
		memDC.MoveTo(rectTmp.right, rectTmp.top);
		memDC.LineTo(rectTmp.right, rectTmp.bottom);

		satTmp = _T("");
	}

	SetDisplaySize(rectClient.right, rectLabel.bottom);

	pen.DeleteObject();

	if (oldPen != NULL){
		memDC.SelectObject(oldPen);
	}

	if (oldFont != NULL){
		memDC.SelectObject(oldFont);
	}
}

/*============================================================================*/
/*! グラフ描画

-# タイムライン描画（衛星順）

@param	memDC			描画用デバイスコンテキスト
@param	rectForecast	予報値描画領域
@param	satname			衛星名
@param	stationno		局番号
@retval	なし

*/
/*============================================================================*/
void CTimeLine::drawTimeLineBaeeSalellite(CDC& memDC, CRect rectForecast, CString satname, UINT stationno)
{
}
bool operator<(const stDrawRect& left, const stDrawRect& right)
{
	return left.rect.top < right.rect.top;
}

bool operator>(const stDrawRect& left, const stDrawRect& right)
{
	return left.rect.top > right.rect.top;
}
/*============================================================================*/
/*! グラフ描画

-# 予報値の描画

@param	memDC		デバイスコンテキスト
@param	satellite	衛星名
@param	passId		パスID
@param	startTime	開始時間
@param	rectTime	時間矩形領域
@param	outTime		予報値描画の最後の位置
@retval	なし

*/
/*============================================================================*/
void CTimeLine::DrawForecast(CDC& memDC, const CString satellite, const CString passId, const CTime startTime, const CRect rectTime, CRect& outTime)
{
	CRect rectClient;
	GetClientRect(rectClient);
	const int mTimeRectdef = 5;

	// 予報値の描画
	stDrawRect drect;
	vector<stForecastData>::iterator itrf;
	CString saveName = _T("");
	CRect rectPred = rectTime;
	int cellHeight = mLabelCelHeight + 10;

	const CString mEISEI_NAME = _T("CTRL.SAT_NAME"); //衛星名
	const CString mPREDFILE_NAME = _T("CTRL.PRED_FILE"); //予報値ファイルパス名
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	// 監視データから衛星名を取得する
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), mEISEI_NAME);
	CString strCurrentSatName = _T("");
	if (obs_adr == NULL){
		// 衛星名が取得できない
#ifdef _DEBUG
		strCurrentSatName = _T("GEOTAIL");
#endif
	}
	else{
		strCurrentSatName.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
	}

	// 監視データから予報値ファイル名を取得する
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), CString(mPREDFILE_NAME));
	CString strPredFile = _T("");
	CString strPredName = _T("");
	if (obs_adr == NULL){
		// 予報値ファイル名が取得できない

#ifdef _DEBUG
		//		mPredFile = _T("C:/share/udsc64/ctrl/prd/geotail/1710020100.prd");//###
		//		mPredFile = _T("C:/share/udsc64/ctrl/prd/vlbi/1710259990.prd");//###
		//strPredFile = _T("C:/share/udsc54/ctrl/prd/hayabusa2/1901310001.prd");//###
		//		mPredFile = _T("C:/share/udsc64/ctrl/prd/geotail/1712159999.prd");//###
		//		mPredFile = _T("C:/share/udsc34/ctrl/prd/planet-c/1712050001.prd");//###
		strPredFile = _T("C:/share/usc34/ctrl/prd/geotail/2003139999.prd");//###
#endif /* _DEBUG */
	}
	else{
		strPredFile.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
	}

	if (strPredFile.Left(1) == _T(" ") || strPredFile.IsEmpty())
	{
	}
	else
	{
		// ファイル名切り出し
		CString strFileName = strPredFile.Right(strPredFile.GetLength() - strPredFile.ReverseFind(_T('/')) - 1);
		// 拡張子削除
		CString strPredId = strFileName.Left(strFileName.ReverseFind(_T('.')));
		// カレント予報値名セット
		strPredName = strPredId.Left(6) + _T("-") + strPredId.Mid(6, 4);
	}

#if 1
	//=========================================================================
	// 予報値の部品を作成する
	//=========================================================================
	CRect curRect = outTime;
	vector<stDrawRect> partdata;
	for (itrf = mForecastDataList.begin(); itrf != mForecastDataList.end(); itrf++){
		CRect partRect = curRect;
		if (CString((*itrf).satellite) != satellite)
			continue;
		if (passId.IsEmpty() == false && passId != (*itrf).passId)
			continue;

		CTime starttime((*itrf).aos);
		int sx = CalculateTimeToX(startTime, rectTime, starttime);
		CTime stoptime((*itrf).los);
		int ex = CalculateTimeToX(startTime, rectTime, stoptime);
		if (sx < 0 && ex < 0)
			continue;
		if (ex < 0)
			continue;
		if (sx > rectClient.right)
			continue;

		if (sx < 0)
			sx = rectTime.left;
		partRect.left = sx;
		partRect.right = ex;

		int nSamePlanCount = GetDuplicatePlan(satellite, (*itrf).passId);
		if (nSamePlanCount != 0){
			// 一つ前の予報値には重複している計画が存在しているので、計画の改行を考慮して予報値の改行を更新する
			partRect.bottom = partRect.top + (cellHeight*(nSamePlanCount + 1));
		}

		wsprintf(drect.satellite, _T("%s"), (*itrf).satellite);
		wsprintf(drect.passId, _T("%s"), (*itrf).passId);
		drect.itemrect = partRect;
		drect.station = (*itrf).station;
		partdata.push_back(drect);
	}

	//=========================================================================
	// 予報値の部品を配置する
	//=========================================================================
	vector<stDrawRect>::iterator itrpart;
	vector<stDrawRect>::iterator itrfrect;
	vector<stDrawRect> temp;
	for (itrpart = partdata.begin(); itrpart != partdata.end(); itrpart++){

		// 既に描画している予報値と重ならない領域を取得する
		(*itrpart).itemrect = GetDrawPredRect(temp, CString((*itrpart).satellite), CString((*itrpart).passId), (*itrpart).itemrect, cellHeight);

		wsprintf(drect.satellite, _T("%s"), (*itrpart).satellite);
		wsprintf(drect.passId, _T("%s"), (*itrpart).passId);
		drect.itemrect = (*itrpart).itemrect;
		drect.rect = (*itrpart).itemrect;
		drect.rect.bottom = drect.rect.top + cellHeight;
		temp.push_back(drect);

		CRect rectDraw = CRect(drect.rect);
		outTime.bottom = __max(outTime.bottom, (*itrpart).itemrect.bottom);
		rectDraw.DeflateRect(0, mTimeRectdef);

		// 予報値バーを描画
		//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
		// 本来はここで、
		// 使用中の予報値ならば、使用中の予報値の色で描画する。
		// 使用中でない予報値は、使用中でない予報値の色で描画する。
		//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
		if (strCurrentSatName.IsEmpty() == false && CString((*itrpart).satellite).CompareNoCase(strCurrentSatName) == 0 &&
			strPredName.IsEmpty() == false && CString((*itrpart).passId) == strPredName)
		{
			// 衛星が一致し、予報値IDが一致の場合、使用中の予報値
			drawTimeline(memDC, rectDraw, mOpeColorTable[eColor_ForecastUsed]);
		}
		else
		{
			// それ以外の予報値
			drawTimeline(memDC, rectDraw, mOpeColorTable[eColor_Forecast]);
		}

#ifdef _DEBUG
//		memDC.DrawText(drect.passId, rectDraw, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
#endif
		drect.rect = rectDraw;
		drect.station = (*itrpart).station;
		mForecastRect.push_back(drect);

		// ツールチップの登録
		for (itrf = mForecastDataList.begin(); itrf != mForecastDataList.end(); itrf++){
			if ((*itrf).station == (*itrpart).station &&
				CString((*itrf).satellite) == CString((*itrpart).satellite) && 
				CString((*itrf).passId) == CString((*itrpart).passId)){
				CRect rectTool = rectDraw;
				rectTool.top -= m_scrollPos.cy;
				rectTool.bottom -= m_scrollPos.cy;
				AddToolTipPred((*itrf), rectTool);
				break;
			}
		}
	}

#else

	outTime.top -= cellHeight;
	outTime.bottom -= cellHeight;


	CRect rectTemp = rectPred;//#
	UINT nSamePlanCount = 0;
	for (itrf = mForecastDataList.begin(); itrf != mForecastDataList.end(); itrf++){
		if (CString((*itrf).satellite) != satellite)
			continue;
		if (passId.IsEmpty() == false && passId != (*itrf).passId)
			continue;

		//#rectTemp = rectPred;

		CTime starttime((*itrf).aos);
		int sx = CalculateTimeToX(startTime, rectTime, starttime);
		CTime stoptime((*itrf).los);
		int ex = CalculateTimeToX(startTime, rectTime, stoptime);
		if (sx < 0 && ex < 0)
			continue;
		if (ex < 0)
			continue;
		if (sx > rectClient.right)
			continue;

		// 改行処理
		CRect backoutTime = outTime;
		outTime.top += cellHeight;
		outTime.bottom = outTime.top + cellHeight;
		rectTemp.top = outTime.top;
		rectTemp.bottom = outTime.top + cellHeight;
		rectTemp.DeflateRect(0, mTimeRectdef);

		if (nSamePlanCount != 0){
			// 一つ前の予報値には重複している計画が存在しているので、計画の改行を考慮して予報値の改行を更新する
			outTime.top = outTime.top + (cellHeight*(nSamePlanCount));
			outTime.bottom = outTime.top + cellHeight;
			rectTemp.top = outTime.top;
			rectTemp.bottom = outTime.top + cellHeight;
			rectTemp.DeflateRect(0, mTimeRectdef);
			nSamePlanCount = 0;
		}

		if (sx < 0)
			sx = rectTime.left;
		rectTemp.left = sx;
		rectTemp.right = ex;

		// 予報値バーを描画
		drawTimeline(memDC, rectTemp, mOpeColorTable[eColor_Forecast]);

		wsprintf(drect.satellite, _T("%s"), (*itrf).satellite);
		wsprintf(drect.passId, _T("%s"), (*itrf).passId);
#ifdef _DEBUG
		memDC.DrawText(drect.passId, rectTemp, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
#endif
		drect.rect = rectTemp;
		mForecastRect.push_back(drect);

		// ツールチップの登録
		CRect rectTool = rectTemp;
		rectTool.top -= m_scrollPos.cy;
		rectTool.bottom -= m_scrollPos.cy;
		AddToolTipPred((*itrf), rectTool);
		nSamePlanCount = GetDuplicatePlan(satellite, (*itrf).passId);
	}
	if (nSamePlanCount != 0){
		// 一つ前の予報値には重複している計画が存在しているので、計画の改行を考慮して予報値の改行を更新する
		outTime.top = outTime.top + (cellHeight*(nSamePlanCount));
		outTime.bottom = outTime.top + cellHeight;
		nSamePlanCount = 0;
	}
#endif
}

/*============================================================================*/
/*! グラフ描画

-# 予報値の描画

@param	memDC		デバイスコンテキスト
@param	satellite	衛星名
@param	passId		パスID
@param	startTime	開始時間
@param	rectTime	時間矩形領域
@param	station		局番号
@param	outTime		予報値描画の最後の位置
@retval	なし

*/
/*============================================================================*/
bool CTimeLine::DrawForecastStation(CDC& memDC, const CString satellite, const CString passId, const CTime startTime, const CRect rectTime, UINT station, CRect& outTime)
{
	CRect rectClient;
	GetClientRect(rectClient);
	const int mTimeRectdef = 5;

	// 予報値の描画
	stDrawRect drect;
	vector<stForecastData>::iterator itrf;
	CString saveName = _T("");
	CRect rectPred = rectTime;
	int cellHeight = mLabelCelHeight + 10;

	const CString mEISEI_NAME = _T("CTRL.SAT_NAME"); //衛星名
	const CString mPREDFILE_NAME = _T("CTRL.PRED_FILE"); //予報値ファイルパス名
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	// 監視データから衛星名を取得する
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(station, mEISEI_NAME);
	CString strCurrentSatName = _T("");
	if (obs_adr == NULL){
		// 衛星名が取得できない
#ifdef _DEBUG
		strCurrentSatName = _T("HAYABUSA2");
#endif
	}
	else{
		strCurrentSatName.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
	}

	// 監視データから予報値ファイル名を取得する
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(station, CString(mPREDFILE_NAME));
	CString strPredFile = _T("");
	CString strPredName = _T("");
	if (obs_adr == NULL){
		// 予報値ファイル名が取得できない
#ifdef _DEBUG
		strPredFile = _T("C:/share/udsc54/ctrl/prd/hayabusa2/1902010002.prd");//###
#endif /* _DEBUG */
	}
	else{
		strPredFile.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
	}

	if (strPredFile.Left(1) == _T(" ") || strPredFile.IsEmpty())
	{
	}
	else
	{
		// ファイル名切り出し
		CString strFileName = strPredFile.Right(strPredFile.GetLength() - strPredFile.ReverseFind(_T('/')) - 1);
		// 拡張子削除
		CString strPredId = strFileName.Left(strFileName.ReverseFind(_T('.')));
		// カレント予報値名セット
		strPredName = strPredId.Left(6) + _T("-") + strPredId.Mid(6, 4);
	}

//#if 1
	bool ret = false; // 描画できたらtrue
	//=========================================================================
	// 予報値の部品を作成する
	//=========================================================================
	CRect curRect = outTime;
	vector<stDrawRect> partdata;
	for (itrf = mForecastDataList.begin(); itrf != mForecastDataList.end(); itrf++){
		CRect partRect = curRect;
		if (CString((*itrf).satellite) != satellite)
			continue;
		if (passId.IsEmpty() == false && passId != (*itrf).passId)
			continue;
		if (station != (*itrf).station)
			continue;

		CTime starttime((*itrf).aos);
		int sx = CalculateTimeToX(startTime, rectTime, starttime);
		CTime stoptime((*itrf).los);
		int ex = CalculateTimeToX(startTime, rectTime, stoptime);
		if (sx < 0 && ex < 0)
			continue;
		if (ex < 0)
			continue;
		if (sx > rectClient.right)
			continue;

		if (sx < 0)
			sx = rectTime.left;
		partRect.left = sx;
		partRect.right = ex;

		int nSamePlanCount = GetDuplicatePlan(satellite, (*itrf).passId, station);
		if (nSamePlanCount != 0){
			// 一つ前の予報値には重複している計画が存在しているので、計画の改行を考慮して予報値の改行を更新する
			partRect.bottom = partRect.top + (cellHeight*(nSamePlanCount + 1));
		}

		wsprintf(drect.satellite, _T("%s"), (*itrf).satellite);
		wsprintf(drect.passId, _T("%s"), (*itrf).passId);
		drect.itemrect = partRect;
		drect.station = station;
		partdata.push_back(drect);
	}

	//=========================================================================
	// 予報値の部品を配置する
	//=========================================================================
	vector<stDrawRect>::iterator itrpart;
	vector<stDrawRect>::iterator itrfrect;
	vector<stDrawRect> temp;
	for (itrpart = partdata.begin(); itrpart != partdata.end(); itrpart++){

		// 既に描画している予報値と重ならない領域を取得する
		(*itrpart).itemrect = GetDrawPredRect(temp, CString((*itrpart).satellite), CString((*itrpart).passId), (*itrpart).itemrect, cellHeight);

		wsprintf(drect.satellite, _T("%s"), (*itrpart).satellite);
		wsprintf(drect.passId, _T("%s"), (*itrpart).passId);
		drect.itemrect = (*itrpart).itemrect;
		drect.rect = (*itrpart).itemrect;
		drect.rect.bottom = drect.rect.top + cellHeight;
		temp.push_back(drect);

		CRect rectDraw = CRect(drect.rect);
		outTime.bottom = __max(outTime.bottom, (*itrpart).itemrect.bottom);
		rectDraw.DeflateRect(0, mTimeRectdef);

		ret = true;

		if ((*itrpart).station == station &&
			strCurrentSatName.IsEmpty() == false && CString((*itrpart).satellite).CompareNoCase(strCurrentSatName) == 0 &&
			strPredName.IsEmpty() == false && CString((*itrpart).passId) == strPredName)
		{
			// 局が一致し、衛星が一致し、予報値IDが一致の場合、使用中の予報値
			drawTimeline(memDC, rectDraw, mOpeColorTable[eColor_ForecastUsed]);
		}
		else
		{
			// 予報値バーを描画
			drawTimeline(memDC, rectDraw, mOpeColorTable[eColor_Forecast]);
		}

#ifdef _DEBUG
//		memDC.DrawText(drect.passId, rectDraw, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
#endif
		drect.rect = rectDraw;
		drect.station = (*itrpart).station;
		mForecastRect.push_back(drect);

		// ツールチップの登録
		for (itrf = mForecastDataList.begin(); itrf != mForecastDataList.end(); itrf++){
			if ((*itrf).station == (*itrpart).station &&
				CString((*itrf).satellite) == CString((*itrpart).satellite) &&
				CString((*itrf).passId) == CString((*itrpart).passId)){
				CRect rectTool = rectDraw;
				rectTool.top -= m_scrollPos.cy;
				rectTool.bottom -= m_scrollPos.cy;
				AddToolTipPred((*itrf), rectTool);
				break;
			}
		}
	}

//#else
//
//	outTime.top -= cellHeight;
//	outTime.bottom -= cellHeight;
//
//	CRect rectTemp = rectPred;//#
//	bool ret = false;
//	UINT nSamePlanCount = 0;
//	for (itrf = mForecastDataList.begin(); itrf != mForecastDataList.end(); itrf++){
//		if (CString((*itrf).satellite) != satellite)
//			continue;
//		if (passId.IsEmpty() == false && passId != CString((*itrf).passId))
//			continue;
//		if (station != (*itrf).station)
//			continue;
//
//		//#rectTemp = rectPred;
//
//		CTime starttime((*itrf).aos);
//		int sx = CalculateTimeToX(startTime, rectTime, starttime);
//		CTime stoptime((*itrf).los);
//		int ex = CalculateTimeToX(startTime, rectTime, stoptime);
//		if (sx < 0 && ex < 0)
//			continue;
//		if (ex < 0)
//			continue;
//
//		ret = true;
//
//		// 改行処理
//		outTime.top += cellHeight;
//		outTime.bottom = outTime.top + cellHeight;
//		rectTemp.top = outTime.top;
//		rectTemp.bottom = outTime.top + cellHeight;
//		rectTemp.DeflateRect(0, mTimeRectdef);
//
//		if (nSamePlanCount != 0){
//			// 一つ前の予報値には重複している計画が存在しているので、計画の改行を考慮して予報値の改行を更新する
//			outTime.top = outTime.top + (cellHeight*nSamePlanCount);
//			outTime.bottom = outTime.top + cellHeight;
//			rectTemp.top = outTime.top;
//			rectTemp.bottom = outTime.top + cellHeight;
//			rectTemp.DeflateRect(0, mTimeRectdef);
//		}
//
//		if (sx < 0)
//			sx = rectTime.left;
//		rectTemp.left = sx;
//		rectTemp.right = ex;
//		drawTimeline(memDC, rectTemp, mOpeColorTable[eColor_Forecast]);
//		//wsprintf(drect.satellite, _T("%s"), satellite);
//		//wsprintf(drect.passId, _T("%s"), passId);
//		wsprintf(drect.satellite, _T("%s"), (*itrf).satellite);
//		wsprintf(drect.passId, _T("%s"), (*itrf).passId);
//#ifdef _DEBUG
//		memDC.DrawText(drect.passId, rectTemp, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
//#endif
//		drect.rect = rectTemp;
//		mForecastRect.push_back(drect);
//
//		// ツールチップの登録
//		CRect rectTool = rectTemp;
//		rectTool.top -= m_scrollPos.cy;
//		rectTool.bottom -= m_scrollPos.cy;
//		AddToolTipPred((*itrf), rectTool);
//		nSamePlanCount = GetDuplicatePlan(satellite, (*itrf).passId, station);
//	}
//#endif
	return ret;
}

/*============================================================================*/
/*! グラフ描画

-# 予報値の描画

@param	memDC		デバイスコンテキスト
@param	satellite	衛星名
@param	passId		パスID
@param	startTime	開始時間
@param	rectTime	時間矩形領域
@param	station		局番号
@retval	なし

*/
/*============================================================================*/
bool CTimeLine::DrawOperationStation(CDC& memDC, const CString satellite, const CString passId, const CTime startTime, const CRect rectTime, UINT station, CRect& outTime)
{

	//==========================================================
	// 監視の取得
	const CString mPLAN_NO = _T("CTRL.PASS_NO");// 計画番号
	const CString mEISEI_NAME = _T("CTRL.SAT_NAME"); //衛星名
	const CString mPLAN_STATUS = _T("CTRL.PASS_FLAG"); //運用状態
	const CString mPLAN_STATUS_DETAIL = _T("CTRL.PASS_INFO"); //運用状態詳細

	const CString mINPASS_STATUS = _T("パス中"); //計画登録
	const CString mOUTPASS_STATUS = _T("パス外"); //計画登録
	const CString mPLAN_REGIST = _T("計画登録"); //計画登録
	const CString mPLAN_INIT = _T("初期設定"); //初期設定
	const CString mRDNS_RNG = _T("ＲＮＧ校正");		// ＲＮＧ校正
	const CString mRDNS_AGC = _T("ＡＧＣ校正");		// ＡＧＣ校正
	const CString mRDNS_TLM = _T("テレメトリ伝送チェック");		// テレメトリ伝送チェック
	const CString mRDNS_CMD = _T("コマンド伝送チェック");		// コマンド伝送チェック
	const CString mOCCU_STATUS = _T("局設備占有時間");		// 局設備占有時間
	const CString mOCCU_STATION = _T("MAINT");		// 局占有

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	// 監視データから衛星名、計画IDを取得する
	CSatelliteData& sd = theApp.GetSatelliteData();
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(station, mEISEI_NAME);
	CString strCurrentSatName = _T("");
	if (obs_adr == NULL){
		// 衛星名が取得できない
#ifdef _DEBUG
		strCurrentSatName = _T("HAYABUSA2");
#endif
	}
	else{
		strCurrentSatName.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(station, mPLAN_NO);
	CString strCurrentPlanID = _T("");
	if (obs_adr == NULL){
		// 計画IDが取得できない
#ifdef _DEBUG
		strCurrentPlanID = _T("190201-0002");
//		if (station == 2)	strCurrentPlanID = _T("170327-9999");
#endif
	}
	else{
		strCurrentPlanID.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(station, mPLAN_STATUS);
//	double dbCurrentPlanStatus = 0.0;
//	if (obs_adr == NULL){
//		// 運用状態が取得できない
//#ifdef _DEBUG
//		dbCurrentPlanStatus = 1.0;
//#endif
//	}
//	else{
//		dbCurrentPlanStatus = obs_adr->d_data;
//	}
	UINT nCurPlanStatus = eKanshiPlanStatus_OutPass;	// パス外
	if (obs_adr == NULL){
		// 運用状態が取得できない
#ifdef _DEBUG
		if (station == 1)
			nCurPlanStatus = eKanshiPlanStatus_InPass;	// パス中
#endif
	}
	else{
		CString strStatus = CString(obs_adr->sz_sts_name);
		if (strStatus.Compare(mINPASS_STATUS) == 0)
		{
			nCurPlanStatus = eKanshiPlanStatus_InPass;	// パス中
		}
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(station, mPLAN_STATUS_DETAIL);
	if (obs_adr == NULL){
		// 運用状態が取得できない
#ifdef _DEBUG
#endif
	}
	else{
		CString strStatus = CString(obs_adr->sz_sts_name);
		if (strStatus.Compare(mPLAN_REGIST) == 0)
		{
			nCurPlanStatus = eKanshiPlanStatus_PlanRegist;	// 計画登録
		}
		else if (strStatus.Compare(mPLAN_INIT) == 0)
		{
			nCurPlanStatus = eKanshiPlanStatus_PlanInit;	// 初期設定
		}
		else if (strStatus.Compare(mRDNS_RNG) == 0)
		{
			nCurPlanStatus = eKanshiPlanStatus_InPass;	// ＲＮＧ校正のため、パス中あつかい
		}
		else if (strStatus.Compare(mRDNS_AGC) == 0)
		{
			nCurPlanStatus = eKanshiPlanStatus_InPass;	// ＡＧＣ校正のため、パス中あつかい
		}
		else if (strStatus.Compare(mRDNS_TLM) == 0)
		{
			nCurPlanStatus = eKanshiPlanStatus_InPass;	// テレメトリ伝送チェックのため、パス中あつかい
		}
		else if (strStatus.Compare(mRDNS_CMD) == 0)
		{
			nCurPlanStatus = eKanshiPlanStatus_InPass;	// コマンド伝送チェックのため、パス中あつかい
		}

		// 局占有の場合で、かつ局設備占有時間
		if (strCurrentSatName == mOCCU_STATION && strStatus == mOCCU_STATUS)
		{
			nCurPlanStatus = eKanshiPlanStatus_InPass;
		}
	}

	//==========================================================

	const int mTimeRectdef = 5;
	// 現在時刻を求める
	CTime curtime = mCurrentTime;
	CRect rectClient;
	GetClientRect(rectClient);
	// 予報値の描画
	stDrawRect drect;
	vector<stTimeData>::iterator itr;
	CString saveName = _T("");
	int cellHeight = mLabelCelHeight + 10;
	CRect rectTemp = rectTime;
	CRect rectLabel = outTime;
	bool ret = false;
	bool bDraw = false;
//++ タイムライン画面デバッグ >>>
//	int maxBottom;
//	maxBottom = -1;
//++ タイムライン画面デバッグ <<<
	for (itr = mTimeDataList.begin(); itr != mTimeDataList.end(); itr++){
		if (CString((*itr).satellite) != satellite)
			continue;

		// 同一衛星名が存在する
		__int64 saveStart(0), saveEnd(0);
//		UINT current = 0;
		map< __int64, vector<stTimeLine> >::iterator itrm;
		for (itrm = (*itr).item.begin(); itrm != (*itr).item.end(); itrm++){
			if ((*itrm).second.size() == 0)
				continue;

			// 先頭の対象局を探す
			vector<stTimeLine>::iterator itrtop;
			int posStation = 0;
			for (itrtop = (*itrm).second.begin(); itrtop != (*itrm).second.end(); itrtop++){
				if ((*itrtop).stationID != station){
					posStation++;
					continue;
				}
				break;
			}
			if (itrtop == (*itrm).second.end())
				continue;

			CRect rect = GetPredRect((*itr).satellite, (*itrm).second[posStation].predid, (*itrm).second[posStation].stationID);
			if (rect.IsRectEmpty() == FALSE){
				rectTemp = rect;
			}

			if (bDraw == true/* && IsDuplicatePlanTime((*itr).satellite, (*itrm).first, current, station) == true*/){
				__int64 pstart = 0, pstop = 0;
				pstop = GetPlanTotalTime((*itr).satellite, (*itrm).first, pstart);
				CTime starttime(pstart);
				CTime stoptime(pstop);
				int sx = CalculateTimeToX(startTime, rectTime, starttime);
				int ex = CalculateTimeToX(startTime, rectTime, stoptime);
				if (sx < 0)
					sx = rectTime.left;

				rectTemp.left = sx;
				rectTemp.right = ex;

				// 改行処理
				rectTemp = GetDrawPlanRect(rectTemp, cellHeight);
				/*
				rectLabel.top = rectLabel.top + cellHeight;
				rectLabel.bottom = rectLabel.top + cellHeight;
				rectTime.top = rectLabel.top;
				rectTime.bottom = rectTime.top + cellHeight;
				rectTime.DeflateRect(0, mTimeRectdef);
				rectTemp = rectTime;
				rectTextCell.bottom = rectLabel.bottom;
				*/
				bDraw = false;

				/*
				// 改行処理
				outTime.bottom += cellHeight;
				rectLabel.top += cellHeight;
				rectLabel.bottom = rectLabel.top + cellHeight;
				rectTemp.top = rectLabel.top;
				rectTemp.bottom = rectTemp.top + cellHeight;
				rectTemp.DeflateRect(0, mTimeRectdef);
				bDraw = false;
				*/
			}

			// タイムラインの１行描画
			// 各部品ごとに描画する
			vector<stTimeLine>::iterator itrp;
			__int64 pstart = 0, pstop = 0;
			pstop = GetPlanTotalTime((*itr).satellite, (*itrm).first, pstart, station);
			CString strDrawPlanID = _T("");
			CRect rectDrawPlanID;
			rectDrawPlanID.SetRectEmpty();
			COLORREF colorPlanID = RGB(255, 255, 255);
			for (itrp = (*itrm).second.begin(); itrp != (*itrm).second.end(); itrp++){
				if ((*itrp).stationID != station)
					continue;
				UINT status = (*itrp).code;
//				UINT nStatusFromPlnFile = eKanshiPlanStatus_OutPass;
				//if (mAutoScrol == true)
				{
					if ((*itrp).satellite == strCurrentSatName && (*itrp).planid == strCurrentPlanID){
//						// 監視で実行中かをチェック
//						if (dbCurrentPlanStatus == 1.0){
//							// 実行中
//							status = ePlanStatus_Execute;
//						}
//						else{
//							// 計画の全時間でチェック
//							if (pstart <= curtime.GetTime() && pstop >= curtime.GetTime()){
//								// 実行中
//								status = ePlanStatus_Execute;
//							}
//						}

						switch (nCurPlanStatus)
						{
						case eKanshiPlanStatus_InPass:
							// 実行中
							status = ePlanStatus_Execute;
							break;

						case eKanshiPlanStatus_PlanRegist:
							// 計画登録
							status = ePlanStatus_PlanRegist;
							break;

						case eKanshiPlanStatus_PlanInit:
							// 初期設定
							status = ePlanStatus_PlanInit;
							break;

						case eKanshiPlanStatus_OutPass:
							// パス外
						default:
							break;
						}
					}

//					if (pstart <= curtime.GetTime()){
//						// 計画の全時間でチェック
//						nStatusFromPlnFile = ePlanStatus_Execute;
//					}

					//if (status < ePlanStatus_NormalEnd){
					//	if (pstart <= curtime.GetTime() && pstop >= curtime.GetTime()){
					//		// 実行中
					//		status = ePlanStatus_Execute;
					//	}
					//}
					// 運用状態の更新
					(*itrp).code = status;
				}

				CTime starttime((*itrp).start);
				int sx = CalculateTimeToX(startTime, rectTime, starttime);
				if (sx < 0)
					sx = rectTime.left;
				rectTemp.left = sx;
				CTime stoptime((*itrp).end);
				int px = CalculateTimeToX(startTime, rectTime, stoptime);
				if (sx < 0 && px < 0)
					continue;
				if (px < 0)
					continue;
				rectTemp.right = px;

				if (saveStart == 0){
					saveStart = (*itrp).start;
					saveEnd = (*itrp).end;
				}
				else{
					saveStart = __min(saveStart, (*itrp).start);
					saveEnd = __max(saveEnd, (*itrp).end);
				}

				// 運用計画矩形の描画
				bDraw = true;
				drawTimeline(memDC, rectTemp, (*itrp).color, status);
				if (rectTemp.left >= rectTime.left && rectTemp.right <= rectTime.right)
				{
					ret = true;
				}
				//drawTimeline(memDC, rectTemp, mStationColor[(*itrp).stationID], status);
				if (rectDrawPlanID.IsRectEmpty()){
					rectDrawPlanID = rectTemp;
				}
				else{
					rectDrawPlanID.top = rectTemp.top;
					rectDrawPlanID.bottom = rectTemp.bottom;
					rectDrawPlanID.left = __min(rectTemp.left, rectDrawPlanID.left);
					rectDrawPlanID.right = __max(rectTemp.right, rectDrawPlanID.right);
				}

				// ツールチップ情報の登録
				CRect rectTool = rectTemp;
				rectTool.top -= m_scrollPos.cy;
				rectTool.bottom -= m_scrollPos.cy;
				AddToolTip((*itrp), rectTool);

				wsprintf(drect.satellite, _T("%s"), (*itr).satellite);
				wsprintf(drect.passId, _T("%s"), (*itrp).planid);
				drect.rect = rectTemp;
				drect.type = getOperationType((*itrp).opekind);

				if (status == ePlanStatus_Execute)
				{
					// 実行中の場合
					switch (drect.type)
					{
					case eLineType_Occu:
						// 局占有
						drect.type = eLineType_OccuRun;
						break;

					case eLineType_Vlbi:
						// VLBI
						drect.type = eLineType_VlbiRun;
						break;

					case eLineType_UnexecRdsn:
						// 校正診断
						drect.type = eLineType_execRdsn;
						break;

					default:
						// その他の運用計画
						drect.type = eLineType_Run;
						break;
					}
				}
				else if (status == ePlanStatus_PlanRegist)
				{
					// 計画登録の場合
					switch (drect.type)
					{
					case eLineType_Occu:
						// 局占有
						drect.type = eLineType_OccuRegist;
						break;

					case eLineType_Vlbi:
						// VLBI
						drect.type = eLineType_VlbiRegist;
						break;

					case eLineType_UnexecRdsn:
						// 校正診断
						// 何もしない
						break;

					default:
						// その他の運用計画
						drect.type = eLineType_PlanRegist;
						break;
					}
				}
				else if (status == ePlanStatus_PlanInit)
				{
					// 初期設定の場合
					switch (drect.type)
					{
					case eLineType_Occu:
						// 局占有
						drect.type = eLineType_OccuInit;
						break;

					case eLineType_Vlbi:
						// VLBI
						drect.type = eLineType_VlbiInit;
						break;

					case eLineType_UnexecRdsn:
						// 校正診断
						// 何もしない
						break;

					default:
						// その他の運用計画
						drect.type = eLineType_PlanInit;
						break;
					}
				}

//				if (drect.type == eLineType_UnexecRdsn && nStatusFromPlnFile == ePlanStatus_Execute)
//				{
//					// 校正診断で実行中の場合
//					drect.type = eLineType_execRdsn;
//				}

				drect.getform = (DWORD)(*itrp).getform;
				mOperationRect.push_back(drect);
				if ((*itrp).kind == ePassKind_Pass){
					// 計画ID作成
					strDrawPlanID.Format(_T("%06d-%04d"), (int)((*itrm).first >> 32), (int)((*itrm).first & 0xffffffff));
					colorPlanID = (*itrp).textcolor;
				}
			}
			if (strDrawPlanID.IsEmpty() == false){
				COLORREF back = memDC.SetTextColor(colorPlanID);
				memDC.DrawText(strDrawPlanID, rectDrawPlanID, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
				memDC.SetTextColor(back);
			}
//++ タイムライン画面デバッグ >>>
//			if (rectTemp.bottom > maxBottom)
//			{
//				// 一番下の行を記憶しておく
//				maxBottom = rectTemp.bottom;
//			}
//++ タイムライン画面デバッグ >>>
//			current++;
		}
	}

	// 計画の描画領域を返す
	outTime = rectTemp;
	outTime.bottom += 5;

//++ タイムライン画面デバッグ >>>
//	if (maxBottom < 0)
//	{
//		maxBottom = rectTemp.bottom;
//	}
//	outTime = rectTemp;
//	outTime.bottom = maxBottom + 5;
//++ タイムライン画面デバッグ >>>

	return ret;
}

/*============================================================================*/
/*! グラフ描画

-# 同一衛星名に対して、異なった計画IDと計画の重なりをチェックする

@param	satellite	衛星名
@param	cid			対象計画ID
@param	current		カレント番号
@retval	なし

*/
/*============================================================================*/
CRect CTimeLine::GetPredRect(const CString satellite, const CString predID, UINT nStation)
{
	vector<stDrawRect>::iterator itr;
	CRect rect;
	rect.SetRectEmpty();
	for (itr = mForecastRect.begin(); itr != mForecastRect.end(); itr++){
		if (CString((*itr).satellite) != satellite)
			continue;
		if (predID != (*itr).passId)
			continue;
		if (nStation != (*itr).station)
			continue;
		rect = (*itr).rect;
		break;
	}
	return rect;
}

/*============================================================================*/
/*! グラフ描画

-# 同一衛星名に対して、異なった計画IDと計画の重なりをチェックする

@param	satellite	衛星名
@param	predID		予報値ID
@retval	なし

*/
/*============================================================================*/
UINT CTimeLine::GetDuplicatePlan(const CString satellite, const CString predID, UINT nStationID/* = 0xffffffff*/)
{
	UINT count = 0;
	UINT current = 0;

	typedef struct  {
		__int64		start;
		__int64		stop;
		UINT		count;
	} stPlanTime;

	map<UINT, stPlanTime> data;

	vector<stTimeData>::iterator itr;
	for (itr = mTimeDataList.begin(); itr != mTimeDataList.end(); itr++){
		if ((*itr).satellite != satellite)
			continue;
		map< __int64, vector<stTimeLine> >::iterator itrm;
		UINT pos = 0;
		__int64 pstart = 0, pstop = 0;
		for (itrm = (*itr).item.begin(); itrm != (*itr).item.end(); itrm++){
			if ((*itrm).second.size() == 0)
				continue;
			if (nStationID == 0xffffffff){
				if (predID != (*itrm).second[0].predid)
					continue;
				if (nStationID != 0xffffffff && nStationID != (*itrm).second[0].stationID)
					continue;
			}
			else{
				vector<stTimeLine>::iterator itrtop;
				int posStation = 0;
				for (itrtop = (*itrm).second.begin(); itrtop != (*itrm).second.end(); itrtop++){
					if ((*itrtop).stationID != nStationID){
						posStation++;
						continue;
					}
					break;
				}
				if (itrtop == (*itrm).second.end())
					continue;
				if (predID != (*itrm).second[posStation].predid)
					continue;
				if (nStationID != (*itrm).second[posStation].stationID)
					continue;
			}
			__int64 pstart = 0, pstop = 0;
			pstop = GetPlanTotalTime((*itr).satellite, (*itrm).first, pstart, nStationID);
			map<UINT, stPlanTime>::iterator itrd;
			bool bUpdate = false;
			for (itrd = data.begin(); itrd != data.end(); itrd++){
				if ((*itrd).second.start > pstop){
					// 重なりなし
				}
				else if ((*itrd).second.stop < pstart){
					// 重なりなし
				}
				else{
					// 重なりあり
					(*itrd).second.count++;
					bUpdate = true;
					break;
				}
			}
			if (bUpdate == false){
				stPlanTime d;
				d.count = 0;
				d.start = pstart;
				d.stop = pstop;
				data.insert(map<UINT, stPlanTime>::value_type((UINT)data.size(), d));
			}
		}
	}

	if (data.size() == 0)
		return 0;

	map<UINT, stPlanTime>::iterator itrd;
	for (itrd = data.begin(); itrd != data.end(); itrd++){
		if (count < (*itrd).second.count){
			count = (*itrd).second.count;
		}
	}

	return count;
}

/*============================================================================*/
/*! グラフ描画

-# 該当の予報値の描画領域を求める

@param	rectDefault	デフォルトの描画領域
@param	cellHeight	バーのセル高さ
@retval	なし

*/
/*============================================================================*/
CRect CTimeLine::GetDrawPredRect(vector<stDrawRect> temp, CString satellite, CString predid, CRect rectDefault, int cellHeight)
{
	vector<stDrawRect>::iterator itrfrect;
	CRect rectTemp = rectDefault;
	bool bFindEmpty = false;
	while (bFindEmpty == false){
		bFindEmpty = true;
		for (itrfrect = temp.begin(); itrfrect != temp.end(); itrfrect++){
			CString sname = CString((*itrfrect).satellite);
			CString pname = CString((*itrfrect).passId);
			if (sname != satellite)
				continue;
			if (pname == predid)
				continue;
			CRect rect1 = rectTemp;
			CRect rect2 = CRect((*itrfrect).itemrect);
			CRect rectInter;
			BOOL ret = rectInter.IntersectRect(rect1, rect2);
			if (ret == TRUE){
				// 重なっている
				rectTemp.top += cellHeight;
				rectTemp.bottom += cellHeight;
				bFindEmpty = false;
				break;
			}
		}
	}

	return rectTemp;
}

/*============================================================================*/
/*! グラフ描画

-# 該当の計画の描画領域を求める

@param	rectDefault	デフォルトの描画領域
@param	cellHeight	バーのセル高さ
@retval	なし

*/
/*============================================================================*/
CRect CTimeLine::GetDrawPlanRect(CRect rectDefault, int cellHeight)
{
	CRect rectTemp = rectDefault;
	vector<stDrawRect>::iterator itrchk;
	bool bFindEmpty = false;
	while (bFindEmpty == false){
		bFindEmpty = true;
		for (itrchk = mOperationRect.begin(); itrchk != mOperationRect.end(); itrchk++){
			CRect rect1 = rectTemp;
			CRect rect2 = CRect((*itrchk).rect);
			CRect rectInter;
			BOOL ret = rectInter.IntersectRect(rect1, rect2);
			if (ret == TRUE){
				// 重なっている
				rectTemp.top += cellHeight;
				rectTemp.bottom += cellHeight;
				bFindEmpty = false;
				break;
			}
		}
	}

	return rectTemp;
}

/*============================================================================*/
/*! グラフ描画

-# 同一衛星名に対して、異なった計画IDと計画の重なりをチェックする

@param	satellite	衛星名
@param	cid			対象計画ID
@param	current		カレント番号
@retval	なし

*/
/*============================================================================*/
bool CTimeLine::IsDuplicatePlanTime(const CString satellite, __int64 cid, UINT current, UINT nStationID/* = 0xffffffff*/)
{
	vector<stTimeData>::iterator itr;
	for (itr = mTimeDataList.begin(); itr != mTimeDataList.end(); itr++){
		if ((*itr).satellite != satellite)
			continue;
		map< __int64, vector<stTimeLine> >::iterator itrm;
		UINT pos = 0;
		__int64 start = 0, stop = 0;
		__int64 pstart = 0, pstop = 0;
		stop = GetPlanTotalTime(satellite, cid, start, nStationID);
		if (start == 0 || stop == 0)
			continue;
		for (itrm = (*itr).item.begin(); itrm != (*itr).item.end(); itrm++){
			CString str;
			str.Format(_T("%06d-%04d"), (int)((*itrm).first >> 32), (int)((*itrm).first & 0xffffffff));
			if ((*itrm).first == cid)
				continue;
			if ((*itrm).second.size() == 0)
				continue;
			if (nStationID != 0xffffffff && nStationID != (*itrm).second[0].stationID)
				continue;
			if (pos >= current)
				continue;
			pstop = GetPlanTotalTime(satellite, (*itrm).first, pstart, nStationID);
			if (pstart < start && pstop < start){
			}
			else if (pstart > stop && pstop > stop){
			}
			else{
				CString str1, str2;
				str1.Format(_T("%I64d-%I64d"), cid >> 32, cid & 0xffffffff);
				str2.Format(_T("%I64d-%I64d"), (*itrm).first >> 32, (*itrm).first & 0xffffffff);
				//TRACE("## %s : %s=%s\n", CStringA(satellite), CStringA(str1), CStringA(str2));
				return true;
			}
			pos++;
		}
	}
	return false;
}

bool CTimeLine::IsDuplicatePlanTime(const CString satellite, vector<stTimeLine>& list, __int64 saveStart, __int64 saveEnd)
{
	if (saveStart == 0 || saveEnd == 0)
		return false;

	vector<stTimeLine>::iterator itr;
	__int64 start = 0, end = 0;
	for (itr = list.begin(); itr != list.end(); itr++){
		if (start == 0){
			start = (*itr).start;
			end = (*itr).end;
		}
		else{
			start = __min(start, (*itr).start);
			end = __max(end, (*itr).end);
		}
	}
	if (saveEnd < start)
		return false;
	if (saveStart > end)
		return false;

	return true;
}

/*============================================================================*/
/*! グラフ描画

-# タイムライン描画（ベース）

@param	memDC		描画用デバイスコンテキスト
@retval	なし

*/
/*============================================================================*/
UINT CTimeLine::getOperationType(UINT opeKind)
{
	UINT flag = eLineType_Empty;
	switch (opeKind){
	case	eOperationKind_StationOccupancy:
		flag = eLineType_Occu;
		break;
	case	eOperationKind_Plan:
		flag = eLineType_Plan;
		break;
	case	eOperationKind_Vlbi:
		flag = eLineType_Vlbi;
		break;
	case	eOperationKind_Rdychk:
		flag = eLineType_UnexecRdsn;
		break;
	}
	return flag;
}

/*============================================================================*/
/*! グラフ描画

-# 運用計画のTotal時刻の取得

@param	satellite	衛星名
@param	cid			対象計画ID
@param	start		開始時刻(out)
@retval	終了時刻

*/
/*============================================================================*/
__int64 CTimeLine::GetPlanTotalTime(const CString satellite, __int64 cid, __int64& start, UINT nStationID/* = 0xffffffff*/)
{
	__int64 stop = 0;
	start = 0;
	vector<stTimeData>::iterator itr;
	for (itr = mTimeDataList.begin(); itr != mTimeDataList.end(); itr++){
		if ((*itr).satellite != satellite)
			continue;
		map< __int64, vector<stTimeLine> >::iterator itrm;
		for (itrm = (*itr).item.begin(); itrm != (*itr).item.end(); itrm++){
			if ((*itrm).first != cid)
				continue;
			vector<stTimeLine>::iterator itrp;
			bool bFirst = true;
			for (itrp = (*itrm).second.begin(); itrp != (*itrm).second.end(); itrp++){
				if (nStationID != 0xffffffff && nStationID != (*itrp).stationID)
					continue;
				if (bFirst == true){
					bFirst = false;
					start = (*itrp).start;
					stop = (*itrp).end;
				}
				start = __min(start, (*itrp).start);
				stop = __max(stop, (*itrp).end);
			}
		}
		//break;
	}
	return stop;
}

/*============================================================================*/
/*! グラフ描画

-# 運用計画内に指定衛星が存在するかのチェック

@param	stationID	局ID
@param	satellite	衛星名
@retval	bool

*/
/*============================================================================*/
bool CTimeLine::IsStationSatellite(const UINT stationID, const CString satellite)
{
	return true;
	vector<stTimeData>::iterator itr;
	for (itr = mTimeDataList.begin(); itr != mTimeDataList.end(); itr++){
		if ((*itr).satellite != satellite)
			continue;
		map< __int64, vector<stTimeLine> >::iterator itrm;
		for (itrm = (*itr).item.begin(); itrm != (*itr).item.end(); itrm++){
			vector<stTimeLine>::iterator itrp;
			for (itrp = (*itrm).second.begin(); itrp != (*itrm).second.end(); itrp++){
				if (stationID == eStation_MAX){
					return true;
				}
				else{
					if ((*itrp).stationID == stationID){
						return true;
					}
				}
			}
		}
	}
	return false;
}

/*============================================================================*/
/*! グラフ描画

-# 時間から現在の描画位置を計算する

@param
@retval

*/
/*============================================================================*/
int CTimeLine::CalculateTimeToX(CTime startTime, CRect startRect, CTime time)
{
	CRect rectClient;
	GetClientRect(rectClient);

	if (startTime > time)
		return -1;

#ifdef _DEBUG
	CString temps = startTime.Format("%Y%m%d%H%M%S");
	CString tempc = time.Format("%Y%m%d%H%M%S");
	//TRACE("## StartTime=%s CurrentTime=%s StartRect.left=%d\n", CStringA(temps), CStringA(tempc), startRect.left);
#endif

	CTimeSpan sp = time - startTime;
	DWORD totalsec = (DWORD)((sp.GetDays() * 24 * 60 * 60) + (sp.GetHours() * 60 * 60) + (sp.GetMinutes() * 60) + sp.GetSeconds());
	//DWORD rangesec = (((mTimeScale[mTimeScaleIndex] / 3) + mTimeScale[mTimeScaleIndex]) * 60 * 60);
	DWORD rangesec = mTimeRange[mTimeScaleIndex] * 60 * (MAX_TIMEDIV - 1);
	DWORD totalwidth = startRect.Width();
	DWORD totalheight = startRect.Height();

	int nX = (int)(((float)totalwidth * (float)totalsec) / (float)rangesec);

	return (startRect.left + nX);
}


void CTimeLine::OnSize(UINT nType, int cx, int cy)
{
	//if (mBitmap.GetSafeHandle()){
	//	mBitmap.DeleteObject();
	//}
	CWnd::OnSize(nType, cx, cy);
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
@param		status	運用計画の状態
@retval

*/
/*============================================================================*/
void CTimeLine::drawTimeline(CDC& memDC, const CRect rect, COLORREF color, UINT status/* = ePlanStatus_None*/)
{
	if (rect.Width() == 0)
		return;

#if 1
	COLORREF penColor = mOpeColorTable[eColor_Grid];
	int	nPenWidth = 1;
	switch (status){
	case	ePlanStatus_Execute:
		penColor = mOpeColorTable[eColor_OpePlanExecute];
		nPenWidth = 2;
		break;
	case	ePlanStatus_NormalEnd:
		penColor = mOpeColorTable[eColor_OpePlanNormal];
		nPenWidth = 2;
		break;
	case	ePlanStatus_AbnormalEnd:
		penColor = mOpeColorTable[eColor_OpePlanAbnormal];
		nPenWidth = 2;
		break;
	}
	CPen pen(PS_SOLID, nPenWidth, penColor);
	CBrush brush(color);

	CPen *oldPen = memDC.SelectObject(&pen);
	CBrush *oldBrush = memDC.SelectObject(&brush);
	memDC.Rectangle(rect);
	memDC.SelectObject(oldPen);
	memDC.SelectObject(oldBrush);

	//memDC.FillSolidRect(rect, color);
#else
	CDrawingManager dm(memDC);
	COLORREF dcolor = darkenColor(color, 50);
	COLORREF lcolor = lightenColor(color, 50);
	CRect rc = rect;
	rc.InflateRect(0, 1);
	dm.FillGradient(rc, dcolor, lcolor, TRUE);

	CBrush brush(RGB(0, 0, 0));
	memDC.FrameRect(&rect, &brush);
#endif // 0
}

void CTimeLine::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (mHeader == false){
		CMenu		pPopup;
		CMenu*		pPopupSub;

		theApp.SetTimeLinePlan(_T(""), _T(""), 0, 0, 0);

		CString satellite = _T(""), planid = _T("");

		// 監視モードの場合はメニュー表示は行わない
		if (theApp.GetExecuteMode() != eExecuteMode_Control){
			return;
		}

		// 制限つき制御モードの場合はメニュー表示は行わない
		if (theApp.GetLimitedMode() == eLimitedMode_Enable){
			return;
		}

		if (point.x < _FIXEDLABEL_WIDTH)
			return;

		UINT flag = 0;
		LPARAM getform = 0;
		CTime time;
		UINT ret = HitTestEx(point, satellite, planid, flag, getform, time);

		//端末時刻と局サーバ時刻に差があると、選択できないケースあるかも
		//CTime ctime;
		//theApp.GetCurrentUtcTime(ctime);
		//time_t curTime = ctime.GetTime();
		//time_t mouseTime = time.GetTime();
		//time_t centerTime = mCurrentTime.GetTime();
		//if (mouseTime < curTime && centerTime < curTime)
		//{
		//	// マウス位置とタイムラインの中心位置が現在時刻よりも過去の場合
		//	if (ret == 0)
		//	{
		//		// マウス右クリック箇所が何も無い場所では、メニュー表示しない
		//		return;
		//	}
		//	// マウス右クリック箇所が予報値や計画の場合
		//	if (IsHitTestFuture(point, curTime) == false)
		//	{
		//		// 衛星、計画ID、種別、取得先から、予報値・計画の終了時刻が実際には過去にならば、メニュー表示しない
		//		return;
		//	}
		//}

		if (theApp.GetSelectStationMode() == eStation_1){
			switch (flag){
			case	eLineType_Run:
				if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_RUN))
					return;
				break;
			case	eLineType_Plan:
				if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_PLAN))
					return;
				break;
			case	eLineType_PlanRegist:
				if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_PLAN_REGIST))
					return;
				break;
			case	eLineType_PlanInit:
				if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_PLAN_INIT))
					return;
				break;
			case	eLineType_Pred:
				if (satellite.CompareNoCase(_T("VLBI")) == 0)	// VLBIの予報値
				{
					if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_PRED_VLBI))
						return;
				}
				else
				{
					if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_PRED))
						return;
				}
				break;
			case	eLineType_Overlap:
				if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_OVERLAP))
					return;
				break;
			case	eLineType_UnexecRdsn:
				if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_UNEXEC))
					return;
				break;
			case	eLineType_Occu:
				if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_OCCU))
					return;
				break;
			case	eLineType_OccuRegist:
				if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_OCCU))
					return;
				break;
			case	eLineType_OccuInit:
				if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_OCCU_RUN))
					return;
				break;
			case	eLineType_OccuRun:
				if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_OCCU_RUN))
					return;
				break;
			case	eLineType_Vlbi:
				if ((ret & ePoint_Forecast) == ePoint_Forecast)
				{
					// 予報値あり
					if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_PLAN))
						return;
				}
				else
				{
					// 予報値なし
					if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_VLBI))
						return;
				}
				break;
			case	eLineType_VlbiRegist:
				if ((ret & ePoint_Forecast) == ePoint_Forecast)
				{
					// 予報値あり
					if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_PLAN_REGIST))
						return;
				}
				else
				{
					// 予報値なし
					if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_VLBI))
						return;
				}
				break;
			case	eLineType_VlbiInit:
				if ((ret & ePoint_Forecast) == ePoint_Forecast)
				{
					// 予報値あり
					if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_PLAN_INIT))
						return;
				}
				else
				{
					// 予報値なし
					if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_VLBI_RUN))
						return;
				}
				break;
			case	eLineType_VlbiRun:
				if ((ret & ePoint_Forecast) == ePoint_Forecast)
				{
					// 予報値あり
					if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_RUN))
						return;
				}
				else
				{
					// 予報値なし
					if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_VLBI_RUN))
						return;
				}
				break;
			case	eLineType_execRdsn:
				if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_UNEXEC_RUN))
					return;
				break;

			case	eLineType_Empty:		// メニューなし
				if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE_EMPTY))
					return;
				break;
			}
		}
		else{
			return;
			if (!pPopup.LoadMenu(IDR_POPUP_TIMELINE5))
				return;
		}

		pPopupSub = pPopup.GetSubMenu(0);
		if (pPopupSub == NULL){
			pPopup.DestroyMenu();
			return;
		}
		CPoint		pos;
		GetCursorPos(&pos);

#if 1
		UINT val4 = 0;

		if (flag == eLineType_Pred)
		{
			val4 |= 0x01;
		}
		if (flag == eLineType_UnexecRdsn)
		{
			val4 |= 0x02;
		}

		theApp.SetTimeLinePlan(satellite, planid, getform, val4, time);

#else
		theApp.SetTimeLinePlan(satellite, planid, getform, (flag == eLineType_Pred) ? 1 : 0);
#endif
//		theApp.SetTimeLineCall(true);

#if 1
		if ( satellite.IsEmpty() == true)
		{
			// 衛星名が無い場合、『校正診断立案』メニューを非活性にする。
			for (int i = 0; i < pPopupSub->GetMenuItemCount(); i++)
			{
				UINT id = pPopupSub->GetMenuItemID(i);
				if (id == ID_MC_CALIBDIAGNS)
				{
					pPopupSub->EnableMenuItem(id, MF_BYCOMMAND | MF_GRAYED);
				}
			}
		}

		if ((flag == eLineType_Empty) && 
			((satellite.CompareNoCase(_T("VLBI")) == 0) ||
			(satellite.CompareNoCase(_T("MAINT")) == 0)))
		{
			// 空き領域で、衛星名がVLBIの場合、またはMAINTの場合は、『校正診断立案』メニューを非活性にする。
			for (int i = 0; i < pPopupSub->GetMenuItemCount(); i++)
			{
				UINT id = pPopupSub->GetMenuItemID(i);
				if (id == ID_MC_CALIBDIAGNS)
				{
					pPopupSub->EnableMenuItem(id, MF_BYCOMMAND | MF_GRAYED);
				}
			}
		}
#else
		if (satellite.IsEmpty() == true)
		{
			// 衛星名が無い場合『校正診断立案』メニューを非活性にする。
			for (int i = 0; i < pPopupSub->GetMenuItemCount(); i++)
			{
				UINT id = pPopupSub->GetMenuItemID(i);
				if (id == ID_MC_CALIBDIAGNS)
				{
					pPopupSub->EnableMenuItem(id, MF_BYCOMMAND | MF_GRAYED);
				}
			}
		}

		if ((flag == eLineType_Empty) && 
			((satellite.CompareNoCase(_T("VLBI")) == 0) ||
			(satellite.CompareNoCase(_T("MAINT")) == 0)))
		{
			// 衛星名がVLBIまたはMAINTの場合はその時間帯以外ではメニューを出さない。
			pPopup.DestroyMenu();
			return;
		}
#endif

		if (ret == 0)
		{
			for (int i = 0; i < pPopupSub->GetMenuItemCount(); i++){
				UINT id = pPopupSub->GetMenuItemID(i);
				if (id == ID_MS_PLANNING)
					pPopupSub->EnableMenuItem(id, MF_BYCOMMAND | MF_GRAYED);
			}
		}

		pPopupSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pos.x, pos.y, this/*theApp.m_pMainWnd*/);
		pPopup.DestroyMenu();
	}

	CWnd::OnRButtonDown(nFlags, point);
}

/*============================================================================*/
/*! 運用計画タイムライン

-# マウス位置情報からアイテム内容を取得する

@param		point		マウス位置
@param		satellite	衛星名
@param		planid		計画ID
@param		flag		タイムライン種別
@param		getform		取得先
@param		time		マウス位置時刻
@retval		種別 0:なし、1:予報値、2:計画

*/
/*============================================================================*/
UINT CTimeLine::HitTestEx(CPoint& point, CString& satellite, CString& planid, UINT& flag, LPARAM& getform, CTime& time)
{
	flag = eLineType_Empty;

	UINT kind = 0;
	CRect rect;
	vector<stDrawRect>::iterator itr;
	for (itr = mForecastRect.begin(); itr != mForecastRect.end(); itr++){
		rect = CRect((*itr).rect);
		rect.top -= m_scrollPos.cy;
		rect.bottom -= m_scrollPos.cy;
		if (rect.PtInRect(point) == TRUE){
			kind |= ePoint_Forecast;
			satellite = (*itr).satellite;
			planid = (*itr).passId;
			flag = eLineType_Pred;
			getform = 0;
			break;
		}
	}
	for (itr = mOperationRect.begin(); itr != mOperationRect.end(); itr++){
		rect = CRect((*itr).rect);
		rect.top -= m_scrollPos.cy;
		rect.bottom -= m_scrollPos.cy;
		if (rect.PtInRect(point) == TRUE){
			kind |= ePoint_Operation;
			satellite = (*itr).satellite;
			planid = (*itr).passId;
			flag = (*itr).type;
			getform = (*itr).getform;
			break;
		}
	}

	if (flag == eLineType_Empty){
		for (itr = mSatelliteRect.begin(); itr != mSatelliteRect.end(); itr++){
			rect = CRect((*itr).rect);
			rect.top -= m_scrollPos.cy;
			rect.bottom -= m_scrollPos.cy;
			if (rect.top <= point.y && rect.bottom >= point.y){
				satellite = (*itr).satellite;
				planid = _T("");
			}
		}
	}

	// マウス位置から大体の時間を求める
	// 描画位置の先頭時刻を求める
	CTime startTime, stopTime;
	CTime ctime(mStartTime.wYear, mStartTime.wMonth, mStartTime.wDay, mStartTime.wHour, (mStartTime.wMinute / mTimeRange[mTimeScaleIndex]) * mTimeRange[mTimeScaleIndex], 0);
	CTimeSpan spTime(0, 0, mTimeRange[mTimeScaleIndex], 0);
	ctime -= spTime;
	startTime = ctime;
	CTimeSpan spTime2(0, 0, mTimeRange[mTimeScaleIndex] * 8, 0);
	stopTime = startTime + spTime2;
	// 描画領域を求める
	CRect rectClient;
	GetClientRect(rectClient);
	CRect rectTime;
	int labelWidth = _FIXEDLABEL_WIDTH;// txtSize.cx;
	rect = rectClient;
	rect.left += labelWidth;
	int timeWidth = rect.Width() / (MAX_TIMEDIV - 1);
	rectTime = rectClient;
	rectTime.left = rectClient.left + labelWidth;
	rectTime.right = rectTime.left + (timeWidth * (MAX_TIMEDIV - 1));
	SYSTEMTIME syst;
	if (rectTime.PtInRect(point) == TRUE){
		// マウス位置が時間領域にある
		startTime.GetAsSystemTime(syst);
		stopTime.GetAsSystemTime(syst);
		CTimeSpan sp = stopTime - startTime;
		DWORD totalsec = (DWORD)((sp.GetDays() * 24 * 60 * 60) + (sp.GetHours() * 60 * 60) + (sp.GetMinutes() * 60) + sp.GetSeconds());
		DWORD totalwidth = rectTime.Width();
		time_t tx = (time_t)(((time_t)totalsec * (time_t)(point.x - labelWidth)) / (time_t)totalwidth);
		tx += startTime.GetTime();
		time = CTime(tx);
		time.GetAsSystemTime(syst);
	}

	return kind;
}

/*============================================================================*/
/*! 運用計画タイムライン

-# マウス位置情報からアイテム内容の予報値・計画が未来があるか調べる

@param		point		マウス位置
@param		curTime		現在時刻
@retval		true:未来がある, false:未来がない

*/
/*============================================================================*/
bool CTimeLine::IsHitTestFuture(CPoint& point, time_t curTime)
{
	bool ret = false;
	CRect rect;
	UINT station;
	CString satellite, planid, getform;
	vector<stDrawRect>::iterator itr;

	CGraphMutex::Lock(eFile);
	for (itr = mForecastRect.begin(); itr != mForecastRect.end(); itr++){
		rect = CRect((*itr).rect);
		rect.top -= m_scrollPos.cy;
		rect.bottom -= m_scrollPos.cy;
		if (rect.PtInRect(point) == TRUE){
			station = (*itr).station;
			satellite = (*itr).satellite;
			planid = (*itr).passId;

			vector<stPredListData>& predlist = theApp.GetSatelliteData().GetPredList();
			vector<stPredListData>::iterator itrPrd;
			// 予報値検索
			for (itrPrd = predlist.begin(); itrPrd != predlist.end(); itrPrd++)
			{
				if ((*itrPrd).strStationName.CompareNoCase(mStationString[theApp.GetSelectStation()]) == 0 &&
					(*itrPrd).strSatellite.CompareNoCase(satellite) == 0 &&
					(*itrPrd).strPassId.CompareNoCase(planid) == 0)
				{
					if ((*itrPrd).tLOS >= curTime)
					{
						ret = true;			// 未来に続く予報値である
					}
					else
					{
						ret = false;		// 未来に続く予報値でない
					}
					break;
				}
			}
		}
	}
	for (itr = mOperationRect.begin(); itr != mOperationRect.end(); itr++){
		rect = CRect((*itr).rect);
		rect.top -= m_scrollPos.cy;
		rect.bottom -= m_scrollPos.cy;
		if (rect.PtInRect(point) == TRUE){
			satellite = (*itr).satellite;
			planid = (*itr).passId;
			if ((*itr).getform == 1)
			{
				getform = _T("enp");
			}
			else
			{
				getform = _T("sttn");
			}

			vector<stPlanListData>& planlist = theApp.GetSatelliteData().GetPlanList();
			vector<stPlanListData>::iterator itrPln;
			// ENP立案検索
			if (getform == _T("enp"))
			{
				for (itrPln = planlist.begin(); itrPln != planlist.end(); itrPln++)
				{
					if ((*itrPln).strGetFrom.CompareNoCase(_T("enp")) != 0)
						continue;

					if ((*itrPln).strStationName.CompareNoCase(mStationString[theApp.GetSelectStation()]) == 0 &&
						(*itrPln).strSatelliteName.CompareNoCase(satellite) == 0 &&
						(*itrPln).strPassId.CompareNoCase(planid) == 0)
					{
						if ((*itrPln).tPassEnd >= curTime)
						{
							ret = true;		// 未来に続く計画である
						}
						else
						{
							ret = false;	// 未来に続く計画でない
						}
						break;
					}
				}
			}
			// 局立案検索
			else
			{
				for (itrPln = planlist.begin(); itrPln != planlist.end(); itrPln++)
				{
					if ((*itrPln).strGetFrom.CompareNoCase(_T("enp")) == 0)
						continue;

					if ((*itrPln).strStationName.CompareNoCase(mStationString[theApp.GetSelectStation()]) == 0 &&
						(*itrPln).strSatelliteName.CompareNoCase(satellite) == 0 &&
						(*itrPln).strPassId.CompareNoCase(planid) == 0)
					{
						if ((*itrPln).tPassEnd >= curTime)
						{
							ret = true;		// 未来に続く計画である
						}
						else
						{
							ret = false;	// 未来に続く計画でない
						}
						break;
					}
				}
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	return ret;
}

/*============================================================================*/
/*! 運用状況画面

-# 予報値ファイル名から予報値IDへの変換

@param		name	予報値ファイル名
@retval 予報値ID文字列
*/
/*============================================================================*/
CString CTimeLine::ConvertPredfileToId(CString name)
{
	TCHAR	szDir[_MAX_DIR];
	TCHAR	szDrive[_MAX_DRIVE];
	TCHAR	szFname[_MAX_FNAME];
	_wsplitpath_s(name, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, NULL, 0);
	CString str, temp;
	temp = szFname;
	str.Format(_T("%s-%s"), (LPCTSTR)(temp.Left(6)), (LPCTSTR)(temp.Mid(6)));
	return str;
}

/*============================================================================*/
/*! 運用計画タイムライン

-# タイムラインデータの設定

@param		satellite	衛星名
@param		planid		計画ID
@param		data		タイムラインデータ
@retval

*/
/*============================================================================*/
bool CTimeLine::SetTimeLineData(const CString satellite, const CString planid, stPlanListData data, bool bRedraw/*=true*/)
{
	CString usatellite = satellite;
	usatellite.MakeUpper();

	// 局名から局IDを求める
	UINT sid = theApp.GetSatelliteData().GetStationID(data.strStationName);

	// 予報値ファイル名から予報値IDを求める
	CString strTargetPredID = ConvertPredfileToId(data.strPredName);

	vector<stForecastData>::iterator itr;
	bool bFind = false;
	CString predID = _T("");
	// 計画一覧の局、衛星、計画にひもづく予報値IDが、予報値リストに存在する予報値に一致するかのチェック
	for (itr = mForecastDataList.begin(); itr != mForecastDataList.end(); itr++)
	{
		if ((*itr).station == sid)
		{
			CString strSat = (*itr).satellite;
			CString strPlanType = data.strPlanType;
			if (strSat.CompareNoCase(usatellite) == 0){
				if (strPlanType.CompareNoCase(_T("実運用")) == 0)
				{
					if ((*itr).passId == strTargetPredID)
					{
						bFind = true;
						predID = (*itr).passId;
						break;
					}
				}
				else
				{
					bFind = true;
					predID = (*itr).passId;
					break;
				}
			}
		}
	}
	if (bFind == false){
		// 予報値リストに衛星名が存在しない
		return false;
	}

	bFind = false;

	__int64 tpos = 0;
	// タイムラインデータに衛星名が存在するかのチェック
	vector<stTimeData>::iterator itrt;
	for (itrt = mTimeDataList.begin(); itrt != mTimeDataList.end(); itrt++){
		CString strSat = (*itrt).satellite;

		CString dataPred = _T("");
		map< __int64, vector<stTimeLine> >::iterator itrm;
		for (itrm = (*itrt).item.begin(); itrm != (*itrt).item.end(); itrm++){
			vector<stTimeLine>::iterator itrp;
			for (itrp = (*itrm).second.begin(); itrp != (*itrm).second.end(); itrp++){
				dataPred = (*itrp).predid;
				break;
			}
			if (dataPred.IsEmpty() == false)
				break;
		}

		if (strSat.CompareNoCase(usatellite) == 0 && dataPred == strTargetPredID){
			// タイムラインデータに登録するための、衛星と予報値IDがあるか探す
			bFind = true;
			break;
		}
		tpos++;
	}

	CString f, s;
	AfxExtractSubString(f, planid, 0, _T('-')); // 6
	AfxExtractSubString(s, planid, 1, _T('-')); // 4
	__int64 id = (_wtoi64(f) << 32 | (_wtoi64(s)));
	if (bFind == false){
		// タイムラインデータに衛星名が存在しない
		stTimeData data;
		wsprintf(data.satellite, _T("%s"), (LPCTSTR)usatellite);
		data.item.clear();
		// 計画IDを登録する
		vector<stTimeLine> temp;
		temp.clear();
		data.item.insert(map< __int64, vector<stTimeLine> >::value_type(id, temp));
		mTimeDataList.push_back(data);
		tpos = mTimeDataList.size() - 1;
	}
	else{
		map< __int64, vector<stTimeLine> >::iterator itrm = mTimeDataList[tpos].item.find(id);
		if (itrm == mTimeDataList[tpos].item.end()){
			// 計画IDを登録
			vector<stTimeLine> temp;
			temp.clear();
			mTimeDataList[tpos].item.insert(map< __int64, vector<stTimeLine> >::value_type(id, temp));
		}
		else{
			// タイムラインデータに、登録したい衛星＆計画IDの局があるか調べる
			vector<stTimeLine>::iterator itrp;
			for (itrp = (*itrm).second.begin(); itrp != (*itrm).second.end(); itrp++)
			{
				if ((*itrp).stationID == sid)
				{
					// 局立案を先に登録しているため、同じ衛星、同じ予報値、同じ計画IDが既にリストに登録済みの場合はその計画は局立案である
					// 次に同じIDがきた場合は何もしない
					return false;
				}
			}
		}
	}

	// タイムラインデータに、登録したい衛星＆計画ID、そして局がない場合は以降で追加する
	map< __int64, vector<stTimeLine> >::iterator itrm = mTimeDataList[tpos].item.find(id);

	//// 一旦現在のタイムラインデータを削除する
	//(*itrm).second.clear();

	// タイムラインデータを登録する
	stTimeLine tl;
	if (sid == 0xffffffff)
		return false;
	//=================================================================
	// 運用日時
	wsprintf(tl.satellite, _T("%s"), (LPCTSTR)satellite);
	wsprintf(tl.predid, _T("%s"), (LPCTSTR)predID);

	CString cid;
	GetCID(data.strPlanName, cid);
	wsprintf(tl.cid, (LPCTSTR)cid);

	wsprintf(tl.planid, _T("%s"), (LPCTSTR)planid);
	tl.start = data.tPassStart;
	tl.end = data.tPassEnd;
	tl.stationID = sid;
	tl.color = mOpeColorTable[eColor_OpePlan];
	tl.textcolor = RGB(0, 0, 0);
	tl.opekind = eOperationKind_Plan;
	bool bNoPlan = false;
	tl.getform = 0;
	if (data.strPlanType == mOperationKindString[eOperationKind_Plan]){
		tl.opekind = eOperationKind_Plan;
		if (data.strGetFrom.CollateNoCase(mGetFromString[0]) == 0){ // ENP
			tl.color = mOpeColorTable[eColor_OpePlanEnp];
			tl.getkind = eGetFrom_ENP;
			tl.getform = 1;
		}
		if (data.strGetFrom.CompareNoCase(mGetFromString[1]) == 0 || data.strGetFrom.CompareNoCase(mGetFromString[2]) == 0){ // 局管制
			tl.color = mOpeColorTable[eColor_OpePlan];
			tl.getkind = eGetFrom_STATION;
			tl.getform = 0;
		}
	}
	else if (data.strPlanType == mOperationKindString[eOperationKind_StationOccupancy]){
		tl.opekind = eOperationKind_StationOccupancy;
		tl.textcolor = RGB(255, 255, 255);
		tl.color = mOpeColorTable[eColor_OpeStationOccupy];
		bNoPlan = true;
	}
	else if (data.strPlanType == mOperationKindString[eOperationKind_Vlbi]){
		tl.opekind = eOperationKind_Vlbi;
		tl.textcolor = RGB(255, 255, 255);
		tl.color = mOpeColorTable[eColor_OpeVlbi];
		if (data.strGetFrom.CollateNoCase(mGetFromString[0]) == 0){ // ENP
			tl.getform = 1;
		}
		bNoPlan = true;
	}
	else if (data.strPlanType == mOperationKindString[eOperationKind_Rdychk]){
		tl.opekind = eOperationKind_Rdychk;
		tl.color = mOpeColorTable[eColor_OpePlanCalib];
		bNoPlan = true;
	}
	tl.kind = ePassKind_Pass;
	tl.code = ePlanStatus_None;
	tl.maintcomment = data.strMaintComment;

	(*itrm).second.push_back(tl);

	if (bNoPlan == false){
		//=================================================================
		// プリパス（運用計画前の処理）
		// ・レディネスチェック
		// ・初期設定
		// ・アンテナ待ち受け指向
		if (data.tPrePassStart != 0){
			tl.end = data.tPassStart;
			tl.start = data.tPrePassStart;
			tl.stationID = sid;
			tl.color = mOpeColorTable[eColor_PrePass];
			tl.kind = ePassKind_PrePass;
			(*itrm).second.push_back(tl);
		}
		//=================================================================
		// ポストパス（運用計画後の処理）
		// ・アンテナ天頂指向
//		if (data.strPostPass != _T("NA"))
		{
			tl.start = data.tPassEnd;
			//if (sid == eStation_Usuda64 || sid == eStation_Usuda642)
			//{
			//	tl.end = tl.start + OperationTime_AntennaZenith;
			//}
			//else
			{
				tl.end = tl.start + OperationTime_AntennaZenith2;
			}
			tl.stationID = sid;
			tl.color = mOpeColorTable[eColor_PostPass];
			tl.kind = ePassKind_PostPass;
			(*itrm).second.push_back(tl);
		}
	}

	if (bRedraw == true){
		Invalidate();
		UpdateScrollInfo();
	}

	if (bFind == false){
		// 衛星名がみつからない
		return false;
	}
	return true;
}

void CTimeLine::GetCID(CString strPlan, CString& cid)
{
	FILE *fp = NULL;
	if (_wfopen_s(&fp, strPlan, _T("r")) != 0 || fp == NULL)
	{
		return;
	}

	cid.Empty();
	TCHAR sz_buff[512];
	char sz_line[512];
	CString str;
	BOOL bFind = FALSE;
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		// ヘッダー行の『CID』を探す
		str = CString(sz_line);
		CString item;
		if ((str.Find(_T("# ")) >= 0) && (str.Find(_T("CID=")) >= 0))
		{
			int i = 1;
			while (AfxExtractSubString(item, str, i++, ' '))
			{
				item.TrimLeft();
				item.TrimRight();
				if (item.Find(_T("CID")) >= 0)
				{
					AfxExtractSubString(cid, item, 1, '=');
					bFind = TRUE;
					break;
				}
			}
		}

		if (bFind == TRUE)
		{
			break;
		}
	}
	fclose(fp);
}

LRESULT CTimeLine::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	WM_COMMAND:
	{
		switch (wParam){
		case	ID_MS_PLANNING_DETAIL:		// 運用計画詳細
		{
			CString str1, str2;
			LPARAM val3;
			UINT val4;
			CTime val5;
			theApp.GetTimeLinePlan(str1, str2, val3, val4, val5);
			CString fname;
			int nStation = theApp.GetSelectStation();
			fname.Format(_T("%s%s\\%s\\%s.pln"), (LPCTSTR)theApp.GetShareFilePath(eFileType_Plan_Folder, nStation), (val3 == 0) ? _T("sttn") : _T("enp"), (LPCTSTR)str1, (LPCTSTR)str2);
			fname.Replace(_T("/"), _T("\\"));
#if 0
			CDetailViewDlg mDetailDlg;			// 詳細表示画面
			mDetailDlg.SetTitle(_T("運用計画詳細"));
			// 詳細ビュー表示
			mDetailDlg.SetSelectFileName(fname);
			mDetailDlg.DoModal();
#else
			if (fname.IsEmpty() == false){
				m_DetailDlg.SetTitle(_T("運用計画詳細"));
				m_DetailDlg.SetSelectFileName(fname);
				if (m_DetailDlg.GetSafeHwnd() != NULL){
					m_DetailDlg.DestroyWindow();
				}
				m_DetailDlg.Create(IDD_DIALOG_DETAILVIEW, this);
				m_DetailDlg.ShowWindow(SW_SHOW);
			}
#endif
			break;
		}
		case	ID_FORCED_OP_STOP:			// 運用計画強制終了
		case	ID_OP_ENDTIME_CHG:			// 運用計画終了時刻変更
		case	ID_TIMEOFFSET_SET:			// 時刻オフセット設定
		case	ID_MC_CALIBDIAGNS:			// 校正診断
		case	ID_MS_PLANNING:				// 運用計画立案・運用計画変更
			//		case	ID_OCCUPANCY_DELETE:		// 局設備占有時間帯削除
		case	ID_MP_FORECAST_CHANGE:		// 予報値切替
			//		case	ID_MP_FORECAST_DETAIL:		// アンテナ予報値詳細
			//		case	ID_MC_CALIBDIAGNS_DELETE:	// 校正診断削除
			//		case	ID_VLBI_DELETE:				// VLBI運用時間帯削除
		case	ID_OP_STARTTIME_CHG:		// 運用計画開始時刻変更
		case	ID_MC_CALIBDIAGNS_STOP:		// 校正診断強制終了
			if (wParam == ID_MC_CALIBDIAGNS)
			{
				theApp.SetTimeLineCall(true);
			}
			(theApp.m_pMainWnd)->PostMessage(WM_COMMAND, wParam, lParam);
			break;

		case	ID_MC_CALIBDIAGNS_UPDATE:	// 校正診断変更
			wParam = ID_MC_CALIBDIAGNS;
			theApp.SetTimeLineCall(true);
			(theApp.m_pMainWnd)->PostMessage(WM_COMMAND, wParam, lParam);
			break;

		case	ID_OCCUPANCY_ADD:			// 局設備占有時間帯登録
		{
			CString str1, str2;
			LPARAM val3;
			UINT val4;
			CTime val5;
			theApp.GetTimeLinePlan(str1, str2, val3, val4, val5);

			CInputOccupancy dlg;
			dlg.SetTimeLineTime(val5);
			if (dlg.DoModal() == IDCANCEL)
				break;
			// ここでサーバに通知
			break;
		}
		case	ID_VLBI_ADD:				// VLBI運用時間帯登録
		{
			CString str1, str2;
			LPARAM val3;
			UINT val4;
			CTime val5;
			theApp.GetTimeLinePlan(str1, str2, val3, val4, val5);

			CInputVlbi dlg;
			dlg.SetTimeLineTime(val5);
			if (dlg.DoModal() == IDCANCEL)
				break;
			// ここでサーバに通知
			break;
		}
		case	ID_MP_FORECAST_DETAIL:		// アンテナ予報値詳細
		{
			// 予報値ファイルパスの取得
			CString sat, id, prd;
			LPARAM getform;
			UINT val4;
			CTime val5;
			theApp.GetTimeLinePlan(sat, id, getform, val4, val5);
			int idx = id.Find(_T("-"));
			id.Format(_T("%s%s"), (LPCTSTR)id.Mid(0, idx), (LPCTSTR)id.Mid(idx + 1, 4));

			CString filename = theApp.GetShareFilePath(eFileType_Pred_Folder, theApp.GetSelectStation());
			prd.Format(_T("%s%s\\%s.prd"), (LPCTSTR)filename, (LPCTSTR)sat, (LPCTSTR)id);

			// 計画詳細表示 
			m_DetailDlg.SetTitle(_T("予報値詳細表示"));

			if (!prd.IsEmpty())
			{
				// 詳細ビュー表示
				m_DetailDlg.SetSelectFileName(prd);
				if (m_DetailDlg.GetSafeHwnd() != NULL){
					m_DetailDlg.DestroyWindow();
				}
				m_DetailDlg.Create(IDD_DIALOG_DETAILVIEW_PRD, this);
				m_DetailDlg.ShowWindow(SW_SHOW);
			}
			break;
		}
		case	ID_OCCUPANCY_DELETE:		// 局設備占有時間帯削除
		case	ID_VLBI_DELETE:				// VLBI運用時間帯削除
		case	ID_MS_PLAN_DELETE:			// 運用計画削除
		case	ID_MC_CALIBDIAGNS_DELETE:	// 校正診断削除
		{
			// 予報値ファイルパスの取得
			CString sat, id, prd;
			LPARAM getform;
			UINT val4;
			CTime val5;
			theApp.GetTimeLinePlan(sat, id, getform, val4, val5);

			// 計画削除要求
			CSshShell::structSshParam stParam;
			stParam.strBatchPath = theApp.GetSshBatchPath();
			stParam.strUserName = theApp.GetSshUserName();
			stParam.strServerName = theApp.GetSshServerName();
			stParam.strKeyFilePath = theApp.GetSshKeyPath();
			stParam.strRemShellName = REGIST_PLAN_SH;
			CString arg;
			CString strGetFrom;
			if (getform == 1)
			{
				strGetFrom = _T("enp");
			}
			else
			{
				strGetFrom = _T("sttn");
			}

			//	arg.Format(_T("%s %s %s"), mStationSimpleString[theApp.GetSelectStation()], m_strSatellite, pid);
			// 削除
			arg.Format(_T("%s del %s %s %s.pln"), (LPCTSTR)CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower(), (LPCTSTR)sat.MakeLower(), (LPCTSTR)strGetFrom, (LPCTSTR)id);

			stParam.strRemShellParam.Format(arg);

			int err = CSshShell::ExecShell(stParam);
			{
				CString strDebug;
				strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
				CLogTraceEx::Write(_T(""), _T("CTimeLine"), _T("WindowProc"), _T("debug"), strDebug, nLogEx::debug);
			}
			break;
		}
		case	ID_MP_FORECAST_DELETE:		// 予報値削除
		{
			// 予報値ファイルパスの取得
			CString sat, id, prd;
			LPARAM getform;
			UINT val4;
			CTime val5;
			theApp.GetTimeLinePlan(sat, id, getform, val4, val5);
			int idx = id.Find(_T("-"));
			id.Format(_T("%s%s"), (LPCTSTR)id.Mid(0, idx), (LPCTSTR)id.Mid(idx + 1, 4));

			// 予報値IDが計画一覧に含まれているか調べる
			CGraphMutex::Lock(eFile);

			UINT sta = theApp.GetSelectStation();
			vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();
			for (int i = 0; i < listPlan.size(); i++)
			{
				if (listPlan[i].strStationName.CompareNoCase(mStationString[sta]) == 0 &&
					listPlan[i].strSatelliteName.CompareNoCase(sat) == 0 && 
					listPlan[i].strPredName.Find(id) >= 0)
				{
					// 計画で使われている旨を表示
					CString strTitle;
					GetWindowText(strTitle);
					MessageBox(_T("計画で使われている予報値のため削除できません。"), strTitle, MB_OK | MB_ICONWARNING);
					CGraphMutex::Unlock(eFile);
					return TRUE;
				}
			}

			// 予報値ファイルのサーバ側フルパスを取得する
			prd = _T("");
			vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();
			for (int i = 0; i < listPred.size(); i++)
			{
				if (listPred[i].strStationName.CompareNoCase(mStationString[sta]) == 0 &&
					listPred[i].strSatellite.CompareNoCase(sat) == 0 &&
					listPred[i].strPredPath.Find(id) >= 0)
				{
					prd = listPred[i].strPredPath;
					// 拡張子がprdの場合、anpに置き換える
					prd.TrimRight(PRED_FILE_TYPE);
					prd += ANP_FILE_TYPE;
					break;
				}		
			}

			CGraphMutex::Unlock(eFile);

			if (prd.IsEmpty() == false)
			{
				// 予報値削除要求
				CSshShell::structSshParam stParam;
				stParam.strBatchPath = theApp.GetSshBatchPath();
				stParam.strUserName = theApp.GetSshUserName();
				stParam.strServerName = theApp.GetSshServerName();
				stParam.strKeyFilePath = theApp.GetSshKeyPath();
				stParam.strRemShellName = SEND_NOTICE_SH;
				CString arg;

				// 削除
				{
					// 局名から局番号取得
					arg.Format(_T("%s %d %s"), (LPCTSTR)CString(mStationSimpleString[sta]).MakeLower(), INFO_ID_REQ_DELPRE, (LPCTSTR)prd);

					stParam.strRemShellParam.Format(arg);

					int err = CSshShell::ExecShell(stParam, TRUE);
					{
						CString strDebug;
						strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
						CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnBnClickedForecastlistButton1"), _T("debug"), strDebug, nLogEx::debug);
					}
					if (err == 0)
					{
						// 何もしない
					}
				}
			}
			break;
		}

		default:
			return CWnd::WindowProc(message, wParam, lParam);
		}
		break;
	}
	default:
		return CWnd::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}

int CTimeLine::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_POPUP | TTS_ALWAYSTIP | TTS_NOANIMATE | TTS_NOFADE | TTS_NOPREFIX;// | TTS_USEVISUALSTYLE;
	if (mHeader == false){
		mToolTip.Create(this, dwStyle);
		mToolTip.Activate(TRUE);
		mToolTip.SetMaxTipWidth(500);
		mToolTipId.clear();
	}

	return 0;
}

void CTimeLine::PreSubclassWindow()
{
	//EnableToolTips();

	CWnd::PreSubclassWindow();
}

BOOL CTimeLine::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_MOUSEMOVE:
		if (mHeader == false){
			mToolTip.RelayEvent(pMsg);
		}
		break;
	}
	//if ((pMsg->message >= WM_MOUSEFIRST) && (pMsg->message <= WM_MOUSELAST)) {
	//	if (mHeader == false){
	//		MSG msg = *pMsg;
	//		if (msg.hwnd == m_hWnd) {
	//			mToolTip.RelayEvent(&msg);
	//		}
	//	}
	//}
	//if (pMsg->message == WM_MOUSEMOVE){
	//	POINT pt;
	//	GetCursorPos(&pt);
	//	if (mLastPt.x != 0 && mLastPt.x != pt.x && mLastPt.y != pt.y)
	//		mToolTip.RelayEvent(pMsg);
	//	mLastPt.x = pt.x;
	//	mLastPt.y = pt.y;
	//}

	return CWnd::PreTranslateMessage(pMsg);
}

/*============================================================================*/
/*! 運用計画タイムライン

-# ツールチップの登録

@param		data	運用計画データ
@param		rect	矩形領域
@retval

*/
/*============================================================================*/
void CTimeLine::AddToolTip(stTimeLine& data, CRect rect)
{
	if (mHeader == true){
		return;
	}

	CString buf, str;
	buf.Empty();

	switch (data.opekind){
	case	eOperationKind_Plan:
	{
		/*
		ENP立案または局立案
		パスID
		CID
		現在のアンテナ予報値名
		パス開始日時
		パス終了日時
		*/
		if (data.getkind == eGetFrom_ENP)
			str.Format(_T("ENP立案\r\n"));
		if (data.getkind == eGetFrom_STATION)
			str.Format(_T("局立案\r\n"));
		buf += str;
		str.Format(_T("パスID：%s\r\n"), data.planid);
		buf += str;
		str.Format(_T("CID：%s\r\n"), data.cid);
		buf += str;
		str.Format(_T("予報値：%s\r\n"), data.predid);
		buf += str;
		CTime start = CTime(data.start);
		CTime stop = CTime(data.end);
		str.Format(_T("パス開始日時：%4d-%02d-%02d %02d:%02d:%02d\r\n"), start.GetYear(), start.GetMonth(), start.GetDay(), start.GetHour(), start.GetMinute(), start.GetSecond());
		buf += str;
		str.Format(_T("パス終了日時：%4d-%02d-%02d %02d:%02d:%02d"), stop.GetYear(), stop.GetMonth(), stop.GetDay(), stop.GetHour(), stop.GetMinute(), stop.GetSecond());
		buf += str;

// タイムライン画面デバッグ >>>
//		CString strPassKind;
//		switch (data.kind)
//		{
//		case ePassKind_PrePass:
//			strPassKind = _T("プリパス");
//			break;
//		case ePassKind_PostPass:
//			strPassKind = _T("ポストパス");
//			break;
//		case ePassKind_Pass:
//		default:
//			strPassKind = _T("パス");
//			break;
//		}
//		str.Format(_T("%s開始日時：%4d-%02d-%02d %02d:%02d:%02d\r\n"), (LPCTSTR)strPassKind, start.GetYear(), start.GetMonth(), start.GetDay(), start.GetHour(), start.GetMinute(), start.GetSecond());
//		buf += str;
//		str.Format(_T("%s終了日時：%4d-%02d-%02d %02d:%02d:%02d"), (LPCTSTR)strPassKind, stop.GetYear(), stop.GetMonth(), stop.GetDay(), stop.GetHour(), stop.GetMinute(), stop.GetSecond());
//		buf += str;
// タイムライン画面デバッグ <<<
		break;
	}
	case	eOperationKind_Rdychk:
	{
		/*
		計画校正診断開始日時
		計画校正診断終了予定日時
		*/
		CTime start = CTime(data.start);
		CTime stop = CTime(data.end);
		str.Format(_T("計画校正診断開始日時：%4d-%02d-%02d %02d:%02d:%02d\r\n"), start.GetYear(), start.GetMonth(), start.GetDay(), start.GetHour(), start.GetMinute(), start.GetSecond());
		buf += str;
		str.Format(_T("計画校正診断終了日時：%4d-%02d-%02d %02d:%02d:%02d"), stop.GetYear(), stop.GetMonth(), stop.GetDay(), stop.GetHour(), stop.GetMinute(), stop.GetSecond());
		buf += str;
		break;
	}
	case	eOperationKind_StationOccupancy:
	{
		/*
		開始日時
		終了日時
		*/
		CTime start = CTime(data.start);
		CTime stop = CTime(data.end);
		str.Format(_T("局設備占有開始日時：%4d-%02d-%02d %02d:%02d:%02d\r\n"), start.GetYear(), start.GetMonth(), start.GetDay(), start.GetHour(), start.GetMinute(), start.GetSecond());
		buf += str;
		str.Format(_T("局設備占有終了日時：%4d-%02d-%02d %02d:%02d:%02d"), stop.GetYear(), stop.GetMonth(), stop.GetDay(), stop.GetHour(), stop.GetMinute(), stop.GetSecond());
		buf += str;

		// コメント
		CString comment;
		int i = 0;
		if (data.maintcomment.IsEmpty() == false){
			buf += _T("\r\n[コメント]");
			while (AfxExtractSubString(comment, data.maintcomment, i++, '\t')){
				buf += _T("\n") + comment;
			}
		}

		break;
	}
	case	eOperationKind_Vlbi:
	{
		/*
		開始日時
		終了日時
		*/
		CTime start = CTime(data.start);
		CTime stop = CTime(data.end);
		str.Format(_T("VLBI運用開始日時：%4d-%02d-%02d %02d:%02d:%02d\r\n"), start.GetYear(), start.GetMonth(), start.GetDay(), start.GetHour(), start.GetMinute(), start.GetSecond());
		buf += str;
		str.Format(_T("VLBI運用終了日時：%4d-%02d-%02d %02d:%02d:%02d"), stop.GetYear(), stop.GetMonth(), stop.GetDay(), stop.GetHour(), stop.GetMinute(), stop.GetSecond());
		buf += str;
		break;
	}
	}

	CString f, s;
	AfxExtractSubString(f, data.planid, 0, _T('-')); // 6
	AfxExtractSubString(s, data.planid, 1, _T('-')); // 4
	UINT satid = GetSatelliteID(data.satellite);
	__int64 id = ((__int64)(data.kind & 0xff) << 56) | ((__int64)(data.stationID & 0x0F) << 48) | ((__int64)(satid & 0x0F) << 40) | ((__int64)(_wtoi64(f) & 0xffffff) << 16) | ((__int64)(_wtoi64(s) & 0xffffff));

	vector<__int64>::iterator itr;
	bool bFind = false;
	for (itr = mToolTipId.begin(); itr != mToolTipId.end(); itr++){
		mToolTip.GetText(str, this, (*itr));
		if ((*itr) == id){
			mToolTip.GetText(str, this, id);
			if (str == buf){
				bFind = true;
				break;
			}
		}
	}

	if (bFind == true){
		mToolTip.SetToolRect(this, id, rect);
		mToolTip.UpdateTipText(buf, this, id);
	}
	else{
		mToolTip.AddTool(this, buf, rect, id);
		mToolTipId.push_back(id);
	}
}

/*============================================================================*/
/*! 運用計画タイムライン

-# ツールチップの登録（予報値）

@param		data	予報値データ
@param		rect	矩形領域
@retval

*/
/*============================================================================*/
void CTimeLine::AddToolTipPred(stForecastData& data, CRect rect)
{
	if (mHeader == true){
		return;
	}

	CString buf, str;
	buf.Empty();

	str.Format(_T("アンテナ予報値：%s\r\n"), data.passId);
	buf += str;
	CTime start = CTime(data.aos);
	CTime stop = CTime(data.los);
	str.Format(_T("AOS日時：%4d-%02d-%02d %02d:%02d:%02d\r\n"), start.GetYear(), start.GetMonth(), start.GetDay(), start.GetHour(), start.GetMinute(), start.GetSecond());
	buf += str;
	str.Format(_T("LOS日時：%4d-%02d-%02d %02d:%02d:%02d\r\n"), stop.GetYear(), stop.GetMonth(), stop.GetDay(), stop.GetHour(), stop.GetMinute(), stop.GetSecond());
	buf += str;
	CTime el = CTime(data.maxelTime);
	str.Format(_T("MAX EL 日時：%4d-%02d-%02d %02d:%02d:%02d\r\n"), el.GetYear(), el.GetMonth(), el.GetDay(), el.GetHour(), el.GetMinute(), el.GetSecond());
	buf += str;
	str.Format(_T("MAX EL 角度：%.2f"), data.maxel);
	buf += str;

	CString f, s;
	AfxExtractSubString(f, data.passId, 0, _T('-')); // 6
	AfxExtractSubString(s, data.passId, 1, _T('-')); // 4
	UINT satid = GetSatelliteID(data.satellite);
	__int64 id = ((__int64)(ePassKind_Pred & 0xff) << 56) | ((__int64)(data.station & 0x0F) << 48) | ((__int64)(satid & 0x0F) << 40) | ((__int64)(_wtoi64(f) & 0xffffff) << 16) | ((__int64)(_wtoi64(s) & 0xffffff));

	vector<__int64>::iterator itr;
	bool bFind = false;
	for (itr = mToolTipId.begin(); itr != mToolTipId.end(); itr++){
		if ((*itr) == id){
			CString str;
			mToolTip.GetText(str, this, id);
			if (str == buf){
				bFind = true;
				break;
			}
		}
	}

	if (bFind == true){
		mToolTip.SetToolRect(this, id, rect);
		mToolTip.UpdateTipText(buf, this, id);
	}
	else{
		mToolTip.AddTool(this, buf, rect, id);
		mToolTipId.push_back(id);
	}
}

void CTimeLine::ClearToolTip(bool bRemoveAll/* = false*/)
{
	if (bRemoveAll == false)
		return;

	vector<__int64>::iterator itr;
	for (itr = mToolTipId.begin(); itr != mToolTipId.end(); itr++){
		mToolTip.DelTool(this, (*itr));
	}
	mToolTipId.clear();
}

int CTimeLine::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	int status = CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);

	SetFocus();

	return status;
}

void CTimeLine::SetDisplaySize(int displayWidth, int displayHeight, bool bSync/* = false*/)
{
	m_displaySize = CSize(displayWidth, displayHeight);

	if (IsWindow(m_hWnd))
		UpdateScrollInfo(bSync);

	if (mSyncTimeLine != NULL){
		mSyncTimeLine->SetScrollSize(m_pageSize, m_displaySize, m_windowSize);
	}
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

void CTimeLine::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CRect rectClient;
	GetClientRect(rectClient);
	if (mHeader == true){
		CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
		return;
	}

	const int lineOffset = 60;
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
	UpdateScrollInfo();
	Invalidate();

	return;



	Invalidate();
	UpdateScrollInfo();

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

/*============================================================================*/
/*! タイムライン

-# スクロール情報の更新

@param
@retval

*/
/*============================================================================*/
void CTimeLine::UpdateScrollInfo(bool bSync/* = false*/)
{
	//if (mHeader == true && bSync==false)
	//	return;

	CRect rect;
	GetClientRectScroll(rect);
	CSize windowSize(rect.Width(), rect.Height());
	if (mHeader == false)
		m_windowSize = windowSize;
	else
		windowSize = m_windowSize;

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
	//ShowScrollBar(SB_VERT, TRUE);
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

void CTimeLine::SetSatelliteData(TCHAR* satname)
{
	vector<stSatelliteData>::iterator itr;
	CString str = satname;
	UINT nextid = (UINT)mSatelliteData.size();
	for (itr = mSatelliteData.begin(); itr != mSatelliteData.end(); itr++){
		CString temp = (*itr).satellite;
		if (str.CompareNoCase(temp) == 0)
			return;
	}
	stSatelliteData data;
	memset(&data, 0, sizeof(stSatelliteData));
	wsprintf(data.satellite, _T("%s"), satname);
	data.id = nextid;
	mSatelliteData.push_back(data);
}

UINT CTimeLine::GetSatelliteID(TCHAR* satname)
{
	vector<stSatelliteData>::iterator itr;
	CString str = satname;
	for (itr = mSatelliteData.begin(); itr != mSatelliteData.end(); itr++){
		CString temp = (*itr).satellite;
		if (str.CompareNoCase(temp) == 0)
			return (*itr).id;
	}
	return (UINT)-1;
}








//=============================================================================
// CGroupBoxNote
//=============================================================================

IMPLEMENT_DYNAMIC(CGroupBoxNote, CButton)

CGroupBoxNote::CGroupBoxNote()
{
	//m_colorText = RGB(0, 0, 0);
	//m_colorBack = RGB(191, 219, 255);
	m_colorText = RGB(0, 0, 0);
	m_colorBack = ::GetSysColor(COLOR_BTNFACE);
	m_typeGroup = eGroupType_BoxLine;
	m_pointRound = CPoint(20, 20);
	m_bFontBold = false;
}

CGroupBoxNote::~CGroupBoxNote()
{
}


BEGIN_MESSAGE_MAP(CGroupBoxNote, CButton)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CGroupBoxNote メッセージ ハンドラ

void CGroupBoxNote::PreSubclassWindow()
{
	GetWindowText(m_strText);
	GetWindowRect(m_rect);
	ScreenToClient(m_rect);

	CButton::PreSubclassWindow();
}

void CGroupBoxNote::UpdateRect()
{
	CRect rect;
	GetClientRect(rect);
	m_rect.right = rect.right;
	m_rect.bottom = rect.bottom;
	// 描画
	Invalidate();
}

void CGroupBoxNote::OnPaint()
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
		ptStart.x = m_rect.left + (MAX_TIMEDIV - 1);
		ptEnd.x = ptStart.x + size.cx;
	}

	// 矩形の描画
	CPen *oldPen;
	if (m_typeGroup == eGroupType_None || m_typeGroup == eGroupType_Normal){
		ptEnd.x = (m_rect.right > ptEnd.x) ? ptEnd.x : m_rect.right;
		dc.DrawEdge(CRect(m_rect.left, ptStart.y, m_rect.right, m_rect.bottom), EDGE_ETCHED, BF_RECT);
		dc.FillSolidRect(CRect(ptStart.x, m_rect.top, ptEnd.x, m_rect.top + size.cy), m_colorBack);
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
			dc.SetBkMode(TRANSPARENT);
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
