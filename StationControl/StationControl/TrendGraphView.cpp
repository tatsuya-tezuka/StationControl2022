/*============================================================================*/
/*! TrendGraphView.cpp

-# トレンドグラフ
*/
/*============================================================================*/
// TrendGraphView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "TrendGraphView.h"
#include "MainFrm.h"
#include "ChildFrm.h"

//#define _ALLWAYS_AUTO_SCALE 1

// CTrendGraphView

IMPLEMENT_DYNCREATE(CTrendGraphView, CView)

/*============================================================================*/
/*! TrendGraphView

-# コンストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CTrendGraphView::CTrendGraphView()
: m_nWndTypeSet(eTrendWndSetType_AGC_RNG_EL)
, m_bIsInitialized(FALSE)
, m_MostOldTime1st(0)
, m_MostOldTime(0)
, m_MostNewTime(0)
{

}

/*============================================================================*/
/*! TrendGraphView

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CTrendGraphView::~CTrendGraphView()
{
}

BEGIN_MESSAGE_MAP(CTrendGraphView, CView)
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CTrendGraphView 描画

/*============================================================================*/
/*! TrendGraphView

-# 描画時に呼ばれる

@param  pDC：ドキュメントのデバイスコンテキストポインタ
@retval なし
*/
/*============================================================================*/
void CTrendGraphView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 描画コードをここに追加してください。
}


// CTrendGraphView 診断

#ifdef _DEBUG
void CTrendGraphView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CTrendGraphView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
CStationControlDoc* CTrendGraphView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CTrendGraphView メッセージ ハンドラー


/*============================================================================*/
/*! CTrendGraphView

-# ビュー初期化処理

@param  なし
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
void CTrendGraphView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	GetDocument()->SetTitleEx();
}

/*============================================================================*/
/*! CTrendGraphView

-# トレンドウィンドウ初期化処理

@param  type：ウィンドウセットタイプ 0:AGC / 1:RNG,RR / 2:AGC,RNG,EL
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
void CTrendGraphView::InitTrendWnd(eTrendWndSetType type)
{
	// 端末番号取得する
	CString strTermNo;
	UINT nTermNo = theApp.GetTermNo();
	if (nTermNo > 0)
		nTermNo = nTermNo - 1;
	strTermNo.Format(_T("%02u"), nTermNo);

	SetWndTypeSet(type);

	for (int gno = 0; gno < eTrendWndType_Num; gno++){
		if (m_Graph[gno].GetSafeHwnd()){
			m_Graph[gno].DestroyWindow();
		}
	}

	// 各グラフ表示
	CRect rect;
	GetClientRect(&rect);

	int nStart, nEnd;
	BOOL bEnaRR;
	GetGraphWndStartEnd(nStart, nEnd, bEnaRR);

	int nStation = theApp.GetSelectStation();

#ifdef _NOCOMM

	// プロパティデータを設定
	// AGC/RNG/RR/EL初期化
	{
		vector<CString> listViewName;
		GetViewAllNameList(listViewName);

		for (int nPropertiesID = nStart; nPropertiesID <= nEnd; nPropertiesID++)
		{
			CPropertiesData prop = theApp.GetSatelliteData().GetTrendPropertiesData(nPropertiesID);

			// ウィンドウポインタをコピー
			prop.wnd = (LPARAM)this;

			if (!prop.m_bSetted)
			{
				// グラフ番号コピー
				prop.nGraphIdx = nPropertiesID;
				UINT nViewID = nPropertiesID;
				prop.wndIdx = nViewID;						// ウィンドー番号
				prop.wndName = listViewName[nViewID];		// ウィンドー名称

				prop.isRealTime = true;									// リアルタイム（true）/ ノンリアルタイム（false）
				prop.trandVScale.Auto = true;							// 縦軸左 オートスケール
				prop.trandVScale.VScalLMax = initPropData.VScalLMax;	// 縦軸左 最大値
				prop.trandVScale.VScalLMin = initPropData.VScalLMin;	// 縦軸左 最小値
				//	prop.trandVScale.VScalRMax = initPropData.VScalRMax;	// 縦軸右 最大値
				//	prop.trandVScale.VScalRMin = initPropData.VScalRMin;	// 縦軸右 最小値
				prop.trandVScale.VSCalItvl = initPropData.VSCalItvl;	// 縦軸目盛りの間隔

				//		prop.trandHScale[0] = h24;			// 横軸スケール(上)
				prop.trandHScale[0] = m1;			// 横軸スケール
				//		prop.trandHScale[1] = m60;			// 横軸スケール(下)

				for (int nSigNo = 0; nSigNo < MAX_TDATA; ++nSigNo)
				{
					if (mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSigName.Compare(UDSC64_AGC_XRX2_INTEG_SIG) == 0 ||
						mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSigName.Compare(UDSC64_AGC_XXTCR_INTEG_SIG) == 0 ||
						mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSigName.Compare(XDSC54_AGC_DSNMODEM_INTEG_SIG) == 0 ||
						mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSigName.Compare(USC34_AGC_INTEG_SIG) == 0)
					{
//						mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSigName = mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSigName + strTermNo;
						mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSigName = theApp.GetDataCollection().ReplaceAreaKanshiName(mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSigName) + strTermNo;
					}

					prop.dataName[nSigNo] = mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSigName;			// データ名称
					prop.dataGName[nSigNo] = mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSimpleSigName;	// 簡易データ名称
					prop.dataUnit[nSigNo] = mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strUnit;			// 単位
					prop.nDecDidits[nSigNo] = mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].nDecDidits;	// 少数点以下の桁数
					prop.isViewMode[nSigNo] = true;		// 表示On/Off
					prop.drawMode[nSigNo] = Line;		// 描画モード（Line、Dots、L&D）
					prop.color[nSigNo] = colordef[nSigNo];	// 表示色
					prop.lineWidth[nSigNo] = 1;			// 線幅
					prop.dotSize[nSigNo] = Small;		// ドットサイズ
					prop.vscaleKind[nSigNo] = LScale;	// 縦軸スケール選択
				}
				prop.m_bSetted = TRUE;
			}
			theApp.GetSatelliteData().SetTrendPropertiesData(nPropertiesID, prop);
		}
	}

	// AGC/RNG/RR/ELのうち、使うグラフセットのビューIDをセット
	{
//		vector<CString> listViewName;
//		GetViewNameList(listViewName);

//		int nStation = theApp.GetSelectStation();

		for (int nPropertiesID = nStart; nPropertiesID <= nEnd; nPropertiesID++)
		{
			if (nPropertiesID == eTrendWndType_RR && bEnaRR == FALSE)
				continue;

			CPropertiesData prop = theApp.GetSatelliteData().GetTrendPropertiesData(nPropertiesID);

			if (!prop.m_bSetted)
			{
				// ビューIDコピー
				UINT nViewID = ExchgPropertiesID2ViewID(nPropertiesID);
				prop.wndIdx = nViewID;						// ウィンドー番号
			}
			theApp.GetSatelliteData().SetTrendPropertiesData(nPropertiesID, prop);
		}
	}
#endif

	// ビュー生成
	for (int nPropertiesID = nStart; nPropertiesID <= nEnd; nPropertiesID++)
	{
		if (nPropertiesID == eTrendWndType_RR && bEnaRR == FALSE)
			continue;

// test >>>
//		if (nPropertiesID != eTrendWndType_AGC)
//			continue;
// test <<<

		UINT nViewID = ExchgPropertiesID2ViewID(nPropertiesID);
		m_Graph[nPropertiesID].SetViewID(nViewID);
		m_Graph[nPropertiesID].SetPropertiesID(nPropertiesID);
		m_Graph[nPropertiesID].SetGraphType(eGraphType_Trend);
		m_Graph[nPropertiesID].SetStationIndex(nStation);
		m_Graph[nPropertiesID].Create(WS_VISIBLE, rect, this, eTrendCtrl, TRUE);
		m_Graph[nPropertiesID].SetGridType(CGraphWindow::eGridType_DOT);
	}

	// ビューにパラメータ追加
	AddViewParam();

	PostMessage(WM_SIZE);

	m_bIsInitialized = TRUE;
}


/*============================================================================*/
/*! CTrendGraphView

-# サイズ変更時に呼ばれる

@param  nType
@param	cx
@param	cy
@retval TRUE：成功 / FALSE：失敗
*/
/*============================================================================*/
void CTrendGraphView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CRect rect, rect4;
	GetClientRect(&rect);
	if (m_nWndTypeSet == eTrendWndSetType_AGC_RNG_EL)
	{
		// ビューサイズをAGC:4, RNG:3, EL:3の割合にする
		rect4 = rect;
		rect4.bottom = rect.bottom * 4 / 10;
		rect.bottom = rect.bottom * 3 / 10;
	}
	else
	{
		// ビューサイズを均等に割り付ける
		rect.bottom = rect.bottom / (m_nWndTypeSet + 1);
	}

	int nStart, nEnd;
	BOOL bEnaRR;
	GetGraphWndStartEnd(nStart, nEnd, bEnaRR);

// test >>>
//	nEnd = 0;
// test <<<

	for (int i = nStart; i <= nEnd; i++)
	{
		if (i == eTrendWndType_RR && bEnaRR == FALSE)
			continue;

		// AGC,RNG,ELグラフを同時表示の場合のAGCグラフ領域
		if (m_nWndTypeSet == eTrendWndSetType_AGC_RNG_EL && i == eTrendWndType_AGC)
		{
			// グラフ領域のリサイズ
			if (m_Graph[i].GetSafeHwnd()){
				m_Graph[i].MoveWindow(&rect4);
				rect.OffsetRect(0, rect4.Height());
			}
		}
		// それ以外
		else
		{
			// グラフ領域のリサイズ
			if (m_Graph[i].GetSafeHwnd()){
				m_Graph[i].MoveWindow(&rect);
				rect.OffsetRect(0, rect.Height());
			}
		}
	}
}


/*============================================================================*/
/*! CTrendGraphView

-# ビューに更新を通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrendGraphView::UpdateGraph(BOOL bAll /*= FALSE*/)
{
	int nStart, nEnd;
	BOOL bEnaRR;
	GetGraphWndStartEnd(nStart, nEnd, bEnaRR);

	BOOL bFirst;
	BOOL bFitYoho;
// test >>>
//		nStart = 0;
//		nEnd = 0;
// test <<<

	// 描画するか調べる
	for (int i = nStart; i <= nEnd; i++)
	{
		if (m_Graph[i].GetSafeHwnd() != NULL)
		{
			if (m_Graph[i].GetStopDraw())
				return;
		}
	}

	// 更新データを追加
	for (int i = nStart; i <= nEnd; i++)
	{
		if (m_Graph[i].GetSafeHwnd() != NULL)
		{
			m_Graph[i].AddSatelliteData();	// 予報値設定を考慮
		}
	}

	// グラフ間の時刻を取得
	time_t t_old, t_new;
	bFirst = TRUE;
	bFitYoho = FALSE;
	for (int i = nStart; i <= nEnd; i++)
	{
		if (m_Graph[i].GetSafeHwnd() != NULL)
		{
			t_old = m_Graph[i].GetMostOldTime();
			if (m_Graph[i].GetFitYohochi())
				bFitYoho = TRUE;
			if (t_old == 0)
				continue;

			if (bFirst)
			{
				bFirst = FALSE;
				m_MostOldTime = t_old;
			}
			// もっとも古い時刻を得る
			if (t_old < m_MostOldTime)
			{
				m_MostOldTime = t_old;
			}
			t_new = m_Graph[i].GetMostNewTime();
			// もっとも新しい時刻を得る
			if (t_new > m_MostNewTime)
			{
				m_MostNewTime = t_new;
			}
		}
	}

	// 初回のもっとも古い時刻よりも時刻が古くならないようにする
	if (m_MostOldTime < m_MostOldTime1st && bFitYoho == FALSE)
	{
		m_MostOldTime = m_MostOldTime1st;
	}


//#ifdef _DEBUG
//	{
//		CString	str;
//		str.Format(_T("m_MostOldTime = %I64d"), m_MostOldTime);
//		CLogTraceEx::Write(_T(""), _T("CTrendGraphView"), _T("UpdateGraph"), _T("debug"), str, nLogEx::debug);
//	}
//#endif

	// 横軸レンジとデータサイズを合わせる
	time_t t_start, t_startTmp;
	t_start = 0;
	bFirst = TRUE;
	for (int i = nStart; i <= nEnd; i++)
	{
		if (m_Graph[i].GetSafeHwnd() != NULL)
		{
			t_startTmp = m_Graph[i].FitHRangeAndPPDataSize(m_MostOldTime, m_MostNewTime);

			if (t_startTmp == 0)
				continue;

			// 初回のもっとも古い時刻よりも時刻が古くならないようにする
			if (t_startTmp < m_MostOldTime1st && bFitYoho == FALSE)
			{
				t_startTmp = m_MostOldTime1st;
			}

			// グラフ間の時刻を合わせる
			if (bFirst)
			{
				bFirst = FALSE;
				t_start = t_startTmp;
			}
			if (t_startTmp < t_start)
			{
				t_start = t_startTmp;
			}

#ifdef _ALLWAYS_AUTO_SCALE
			// 先頭の時刻変更時は最大値最小値を設定し直す
			if (t_startTmp != m_MostOldTime)
			{
				m_Graph[i].UpdateMaxMin();
			}
#endif /* _ALLWAYS_AUTO_SCALE */
		}
	}

	if (bFitYoho == FALSE)
	{
		// 監視の時刻に合わせる
		m_MostOldTime = t_start;
	}
	else
	{
		// 予報値の時刻に合わせる
		t_start = m_MostOldTime;
	}

	for (int i = nStart; i <= nEnd; i++)
	{
		m_Graph[i].SetOldTime(m_MostOldTime);
	}

	if (t_start == 0)
	{
		SYSTEMTIME st_UTC;
		GetSystemTime(&st_UTC);
		t_start = CTime(st_UTC).GetTime();
	}

//#ifdef _DEBUG
//	{
//		CString	str;
//		str.Format(_T("t_start = %I64d"), t_start);
//		CLogTraceEx::Write(_T(""), _T("CTrendGraphView"), _T("UpdateGraph"), _T("debug"), str, nLogEx::debug);
//	}
//#endif

	// 古いデータを削除
	for (int i = nStart; i <= nEnd; i++)
	{
		if (m_Graph[i].GetSafeHwnd() != NULL)
		{
			m_Graph[i].SetStartTime(t_start);
			m_Graph[i].DeletePPData();
		}
	}

	// 監視データが更新されたため、グラフビューを更新
	for (int i = nStart; i <= nEnd; i++)
	{
		if (m_Graph[i].GetSafeHwnd() != NULL)
		{
			// 描画対象でない
			if (i == eTrendWndType_RR && bEnaRR == FALSE)
				continue;

			// グラフビュー更新
			m_Graph[i].UpdateGraph(bAll);
		}
	}
}


/*============================================================================*/
/*! CTrendGraphView

-# グラフ時刻を監視データから取得してセットする

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrendGraphView::SetGraphTime(int nStation)
{
	if (m_MostOldTime1st == 0 || m_MostNewTime == 0)
	{
		time_t t;
		// 監視データリストの取得
		CGraphMutex::Lock(eData);

		CSpaceData*	spaceDataList = theApp.GetDataCollection().GetSpaceData(nStation);
		int size = theApp.GetDataCollection().GetGraphSigSize(nStation);
		for (int i = 0; i < size; i++)
		{
			if (spaceDataList[i].srcType == eSigSrcType_EngExchange && spaceDataList[i].value.empty() == false)
			{
				t = CTime(spaceDataList[i].value[0].systime).GetTime();
				if (t == 0)
					continue;

				if (t < m_MostOldTime1st)
				{
					m_MostOldTime1st = t;
					m_MostOldTime = t;
					m_MostNewTime = t;
				}
			}
		}

		CGraphMutex::Unlock(eData);
	}
}


/*============================================================================*/
/*! CTrendGraphView

-# ビューに更新を通知

@param  message	：メッセージの識別子
@param  wParam	：メッセージの最初のパラメータ
@param  lParam	：メッセージの 2 番目のパラメータ
@retval 処理結果
*/
/*============================================================================*/
LRESULT CTrendGraphView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_ThreadNormal:
		// 監視データ更新、グラフビュー更新
		UpdateGraph();
		break;

	case	eMessage_UpdateForecastData:
		// 初期化
		InitTrendWnd(m_nWndTypeSet);
		break;

	case	eMessage_PropetyChange:
		// パラメータ更新のため、パラメータを読み込みに行く
		AddViewParam();
		break;

	case	eMessage_InitTrendWnd:
		// 初期化
		InitTrendWnd((eTrendWndSetType)wParam);
		break;

	case eMessage_NewEngValData:
		// グラフ時刻を取得してセットする
		SetGraphTime((int)wParam);

	default:
		return CView::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}


/*============================================================================*/
/*! CTrendGraphView

-# 描画時に呼ばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CTrendGraphView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	// 描画メッセージで CView::OnPaint() を呼び出さないでください。
}


/*============================================================================*/
/*! CTrendGraphView

-# 開始グラフウィンドウ、終了グラフウィンドウ取得

@param	なし
@retval	なし

*/
/*============================================================================*/
void CTrendGraphView::GetGraphWndStartEnd(int& nStart, int& nEnd, BOOL& bEnaRR)
{
	switch (m_nWndTypeSet)
	{
	case eTrendWndSetType_AGC:
		// AGC
		nStart = eTrendWndType_AGC;
		nEnd = eTrendWndType_AGC;
		bEnaRR = FALSE;
		break;
	case eTrendWndSetType_RNG_RR:
		// RNG, RR
		nStart = eTrendWndType_RNG;
		nEnd = eTrendWndType_RR;
		bEnaRR = TRUE;
		break;
	case eTrendWndSetType_AGC_RNG_EL:
		// AGC, RNG, EL
	default:
		nStart = eTrendWndType_AGC;
		nEnd = eTrendWndType_EL;
		bEnaRR = FALSE;
		break;
	}
}


/*============================================================================*/
/*! CTrendGraphView

-# グラフビュー名リストの取得

@param	listViewName：ビュー名リスト
@retval	TRUE:取得成功 / FALSE:取得失敗

*/
/*============================================================================*/
BOOL CTrendGraphView::GetViewNameList(vector<CString>& listViewName)
{
	CString str;
	int type = GetWndTypeSet();

	switch (type)
	{
	case eTrendWndSetType_AGC:
		if (str.LoadString(IDS_TREND_AGCGRAPH) == FALSE)
			return FALSE;
		listViewName.push_back(str);
		break;

	case eTrendWndSetType_RNG_RR:
		if (str.LoadString(IDS_TREND_RNGGRAPH) == FALSE)
			return FALSE;
		listViewName.push_back(str);
		if (str.LoadString(IDS_TREND_RRGRAPH) == FALSE)
			return FALSE;
		listViewName.push_back(str);
		break;

	case eTrendWndSetType_AGC_RNG_EL:
		if (str.LoadString(IDS_TREND_AGCGRAPH) == FALSE)
			return FALSE;
		listViewName.push_back(str);
		if (str.LoadString(IDS_TREND_RNGGRAPH) == FALSE)
			return FALSE;
		listViewName.push_back(str);
		if (str.LoadString(IDS_TREND_ELGRAPH) == FALSE)
			return FALSE;
		listViewName.push_back(str);

		break;

	default:
		break;
	}

	return TRUE;
}


/*============================================================================*/
/*! CTrendGraphView

-# グラフビュー名全リストの取得

@param	listViewName：ビュー名リスト
@retval	TRUE:取得成功 / FALSE:取得失敗

*/
/*============================================================================*/
BOOL CTrendGraphView::GetViewAllNameList(vector<CString>& listViewName)
{
	CString str;
	if (str.LoadString(IDS_TREND_AGCGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);
	if (str.LoadString(IDS_TREND_RNGGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);
	if (str.LoadString(IDS_TREND_RRGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);
	if (str.LoadString(IDS_TREND_ELGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	return TRUE;
}


/*============================================================================*/
/*! CTrendGraphView

-# ビューにパラメータを追加する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CTrendGraphView::AddViewParam()
{
	int nStart, nEnd;
	BOOL bEnaRR;
	GetGraphWndStartEnd(nStart, nEnd, bEnaRR);

	for (int nPropertiesID = nStart; nPropertiesID <= nEnd; nPropertiesID++)
	{
		if (nPropertiesID == eTrendWndType_RR && bEnaRR == FALSE)
			continue;

// test >>>
//		if (nPropertiesID != eTrendWndType_AGC)
//			continue;
// test <<<

		CPropertiesData* pParam = &theApp.GetSatelliteData().GetTrendPropertiesData(nPropertiesID);

//		m_Graph[nPropertiesID].SetStationIndex(theApp.GetSelectStation());
		m_Graph[nPropertiesID].SetRealTime(pParam->isRealTime);
		(void)m_Graph[nPropertiesID].AddAxis(pParam->trandVScale, pParam->trandHScale[0]);

		CString	strName;

		// 各データのパラメータを追加
		for (int i = 0; i < MAX_TDATA; ++i)
		{
			switch (pParam->drawMode[i])
			{
			case Line:
				(void)m_Graph[nPropertiesID].AddLine(i, pParam, 0, PS_SOLID);
				break;
			case Dots:
				(void)m_Graph[nPropertiesID].AddDot(i, pParam, 0);
				break;
			case LineAndDots:
				(void)m_Graph[nPropertiesID].AddLineAndDot(i, pParam, 0, PS_SOLID);
				break;
			default:
				// 何もしない
				break;
			}
		}

	}

	// カーソル待ち表示
	CWaitCursor wait;

	// グラフ更新
	UpdateGraph(TRUE);
}


/*============================================================================*/
/*! CTrendGraphView

-# ビューIDをウィンドウタイプセットによって、ビュー番号に変換する

@param	type：ビューID
@retval	ビュー番号

*/
/*============================================================================*/
UINT CTrendGraphView::ExchgPropertiesID2ViewID(UINT nPropertiesID)
{
	UINT nViewID = nPropertiesID;

	// グラフをビューの番号に変換
	switch (GetWndTypeSet())
	{
	case eTrendWndSetType_AGC:
		break;
	case eTrendWndSetType_RNG_RR:
		nViewID -= 1;
		break;
	case eTrendWndSetType_AGC_RNG_EL:
		if (nPropertiesID == eTrendWndType_EL)
			nViewID -= 1;
		break;
	default:
		break;
	}

	return nViewID;
}

/*============================================================================*/
/*! 捕捉グラフ

-# レイアウト読み込み（設定読み込み）

@param  なし
@retval	なし

*/
/*============================================================================*/
bool CTrendGraphView::Load(CArchive& ar)
{
	InitTrendWnd(m_nWndTypeSet);
	return true;
}

/*============================================================================*/
/*! 捕捉グラフ

-# レイアウト保存（設定保存）

@param  なし
@retval	なし

*/
/*============================================================================*/
bool CTrendGraphView::Save(CArchive& ar)
{
	return true;
}
