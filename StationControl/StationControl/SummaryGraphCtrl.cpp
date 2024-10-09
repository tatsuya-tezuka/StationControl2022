/*============================================================================*/
/*! SummaryGraphCtrl.cpp

-# サマリグラフ
*/
/*============================================================================*/
// SummaryGraphCtrl.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "SummaryGraphCtrl.h"
#include "SummaryPropertiesDlg.h"

// CSummaryGraphCtrl

IMPLEMENT_DYNAMIC(CSummaryGraphCtrl, CWnd)

/*============================================================================*/
/*! CSummaryGraphCtrl

-# コンストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CSummaryGraphCtrl::CSummaryGraphCtrl()
{
	mParent = NULL;
	mInitOver = FALSE;
	mRect.SetRectEmpty();
	m_nListMax = MAX_SUMMARYTDATA;
	m_pSpectrumView = NULL;
}

/*============================================================================*/
/*! CSummaryGraphCtrl

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CSummaryGraphCtrl::~CSummaryGraphCtrl()
{
}


BEGIN_MESSAGE_MAP(CSummaryGraphCtrl, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CSummaryGraphCtrl メッセージ ハンドラー

/*============================================================================*/
/*! CSummaryGraphCtrl
ウィンドウの作成

@param	pParent：親ウィンドウハンドル
@param	rect：領域
@param	nID：ウィンドウID
@retval TRUE：成功 / FALSE：失敗
*/
/*============================================================================*/
BOOL CSummaryGraphCtrl::Create(CWnd *pParent, CRect rect, UINT nID, UINT nStaionID, UINT nParentID/* = 0*/)
{
	if (!RegisterClass())
		return FALSE;

	mParent = pParent;
	mID = nID;
	mStationIndex = nStaionID;

	if (CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParent, nID, NULL)){
		mInitOver = TRUE;
		return TRUE;
	}
	return FALSE;
}

/*============================================================================*/
/*! CSummaryGraphCtrl
ウィンドウクラスを登録

@param	なし
@retval TRUE：成功 / FALSE：失敗
*/
/*============================================================================*/
BOOL CSummaryGraphCtrl::RegisterClass()
{
	return TRUE;
	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = AfxWndProc;
	wc.hInstance = AfxGetInstanceHandle();
	wc.hbrBackground = ::CreateSolidBrush(COLOR_WINDOW);
	wc.hCursor = ::LoadCursor(AfxGetInstanceHandle(), IDC_ARROW);
	wc.lpszClassName = _T("SUMMARYGRAPH");
	return AfxRegisterClass(&wc);
}

/*============================================================================*/
/*! CSummaryGraphCtrl
ウィンドウの作成

@param	lpCreateStruct
@retval	0：成功 / -1：失敗
*/
/*============================================================================*/
int CSummaryGraphCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

/*============================================================================*/
/*! CSummaryGraphCtrl

-# サイズ変更時に呼ばれる

@param  nType
@param	cx
@param	cy
@retval TRUE：成功 / FALSE：失敗
*/
/*============================================================================*/
void CSummaryGraphCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect);
	mRect = rect;
	if (m_Graph.GetSafeHwnd()){
		m_Graph.MoveWindow(mRect);
	}
}


/*============================================================================*/
/*! CSummaryGraphCtrl

-# 描画時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CSummaryGraphCtrl::OnPaint()
{
	CPaintDC dc(this);

	return;
}


/*============================================================================*/
/*! CSummaryGraphCtrl

-# ビューにパラメータを追加する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSummaryGraphCtrl::AddViewParam()
{
	if (m_Graph.GetSafeHwnd() == NULL)
		return;

	int nViewID = mID - eSummaryCtrl;

	CPropertiesData* pParam = &theApp.GetSatelliteData().GetSummaryPropertiesData(mStationIndex, nViewID);

//	m_Graph.SetStationIndex(mStationIndex);
//
// test >>>
//	if (mStationIndex == eStation_Uchinoura20 && nViewID == 3)
//	{
//		int a = 0;
//	}
// test <<<

	m_Graph.SetRealTime(pParam->isRealTime);
	(void)m_Graph.AddAxis(pParam->trandVScale, pParam->trandHScale[0]);

	CString	strName;

	// 各データのパラメータを追加
	for (int i = 0; i < m_nListMax; ++i)
	{
		switch (pParam->drawMode[i])
		{
		case Line:
			(void)m_Graph.AddLine(i, pParam, 0, PS_SOLID);
			break;
		case Dots:
			(void)m_Graph.AddDot(i, pParam, 0);
			break;
		case LineAndDots:
			(void)m_Graph.AddLineAndDot(i, pParam, 0, PS_SOLID);
			break;
		default:
			// 何もしない
			break;
		}
	}

	// グラフ更新
	UpdateGraph(TRUE);
}


/*============================================================================*/
/*! CSummaryGraphCtrl

-# サマリウィンドウ初期化処理

@param  type：ウィンドウセットタイプ 0:AGC / 1:RNG,RR / 2:AGC,RNG,EL
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
void CSummaryGraphCtrl::InitTrendWnd(eSummaryWndSetType type)
{
	if (m_pSpectrumView != NULL && m_pSpectrumView->GetSafeHwnd() != NULL)
	{
		m_pSpectrumView->ShowWindow(SW_HIDE);
	}

	if (m_Graph.GetSafeHwnd() != NULL)
	{
//		m_Graph.ShowWindow(SW_SHOW);
//		return;
		m_Graph.SendMessage(WM_CLOSE);
	}

	KillTimer(1);

	SetWndTypeSet(type);

	// 各グラフ表示
	CRect rect;
	GetClientRect(&rect);

	int nViewID = mID - eSummaryCtrl;
	m_Graph.SetViewID(nViewID);
	m_Graph.SetGraphType(eGraphType_Summary);
	m_Graph.SetStationIndex(mStationIndex);
	m_Graph.Create(WS_VISIBLE, rect, this, mID + 4);
	m_Graph.SetGridType(CGraphWindow::eGridType_DOT);

	// ビューにパラメータ追加
	AddViewParam();

	PostMessage(WM_SIZE);
}


/*============================================================================*/
/*! CSummaryGraphCtrl

-# スペクトラムウィンドウ初期化処理

@param  なし
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
void CSummaryGraphCtrl::InitSpectrumWnd(UINT nViewID)
{
	CRect rectLast;
	if (m_Graph.GetSafeHwnd() != NULL)
	{
		m_Graph.ShowWindow(SW_HIDE);
	}

	// 各グラフ表示
	CRect rect;
	GetClientRect(&rect);

	CRuntimeClass* pClass = RUNTIME_CLASS(CSpectrumGraphView);
	if (pClass == NULL)
	{
		return;
	}
	m_pSpectrumView = (CSpectrumGraphView*)(pClass->CreateObject());
	if (m_pSpectrumView != NULL)
	{
		m_pSpectrumView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, mID + 4, NULL);
		m_pSpectrumView->InitializeView(mStationIndex, nViewID, eSpectrumGraphType_Sammary);
		m_pSpectrumView->SendMessage(WM_INITIALUPDATE);
	}

	PostMessage(WM_SIZE);

	// タイマ登録
	SetTimer(1, SPECTRUM_INTERVAL, NULL);
}


/*============================================================================*/
/*! CTrendGraphView

-# ビューに更新を通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CSummaryGraphCtrl::UpdateGraph(BOOL bAll /*= FALSE*/)
{
	if (m_Graph.GetSafeHwnd() != NULL)
	{
		// 更新データを追加
		m_Graph.AddSatelliteData();	// 予報値設定を考慮

		// グラフ間の時刻を取得
		time_t t_old, t_new;
		t_old = m_Graph.GetMostOldTime();
		t_new = m_Graph.GetMostNewTime();
		if (t_old != 0)
		{
			m_MostOldTime = t_old;
			m_MostNewTime = t_new;
		}

		// 初回のもっとも古い時刻よりも時刻が古くならないようにする
		if (m_MostOldTime < m_MostOldTime1st)
		{
			m_MostOldTime = m_MostOldTime1st;
		}

		time_t t_start;
		t_start = m_Graph.FitHRangeAndPPDataSize(m_MostOldTime, m_MostNewTime);
		// 初回のもっとも古い時刻よりも時刻が古くならないようにする
		if (t_start < m_MostOldTime1st)
		{
			t_start = m_MostOldTime1st;
		}

		// 古いデータを削除
		m_Graph.SetStartTime(t_start);
		m_Graph.DeletePPData();

		// グラフビュー更新
		m_Graph.UpdateGraph(bAll);
	}
}


/*============================================================================*/
/*! CTrendGraphView

-# グラフ時刻を監視データから取得してセットする

@param  なし
@retval なし
*/
/*============================================================================*/
void CSummaryGraphCtrl::SetGraphTime(int nStation)
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
/*! CSummaryGraphCtrl

-# ビューに更新を通知

@param  message	：メッセージの識別子
@param  wParam	：メッセージの最初のパラメータ
@param  lParam	：メッセージの 2 番目のパラメータ
@retval 処理結果
*/
/*============================================================================*/
LRESULT CSummaryGraphCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	int nViewID = mID - eSummaryCtrl;
	CPropertiesData& prop = theApp.GetSatelliteData().GetSummaryPropertiesData(mStationIndex, nViewID);

	bool bSpectrum;
	if (prop.nGraphIdx == eSummaryWndSetType_XSP && mStationIndex != eStation_Uchinoura20 ||
		prop.nGraphIdx == eSummaryWndSetType_KASP && mStationIndex == eStation_Usuda642)
		bSpectrum = true;
	else
		bSpectrum = false;

	switch (message){
	case	eMessage_ThreadNormal:
		if (bSpectrum)
		{
		}
		else
		{
			// 監視データが更新されたため、ビューに更新を通知
			UpdateGraph();
		}
		break;

	case	eMessage_SummaryPropetyChange:
		if (bSpectrum)
		{
			// パラメータ更新のため、パラメータを読み込みに行く
			if (m_pSpectrumView != NULL)
			{
				m_pSpectrumView->ClearAutoVMaxMin();
				m_pSpectrumView->Invalidate();
			}
		}
		else
		{
			// パラメータ更新のため、パラメータを読み込みに行く
			AddViewParam();
		}
		break;

	case	eMessage_InitSummaryWnd:
	{
		if (bSpectrum)
		{
			// ミニスペクトラム初期化
			InitSpectrumWnd(nViewID);
		}
		else
		{
			// ミニトレンド初期化
			InitTrendWnd((eSummaryWndSetType)wParam);
		}
		break;
	}

	case eMessage_NewEngValData:
		// グラフ時刻を取得してセットする
		SetGraphTime((int)wParam);
		break;

	default:
		return CWnd::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}


/*============================================================================*/
/*! CSummaryGraphCtrl

-# タイマイベント発生時に呼び出される

@param	nIDEvent：イベントID
@retval	なし

*/
/*============================================================================*/
void CSummaryGraphCtrl::OnTimer(UINT_PTR nIDEvent)
{
	CSpectrumViewData* pCSectrumData = &theApp.GetSatelliteData().GetSummarySpectrumViewData(mStationIndex, mID - eSummaryCtrl);
	if (pCSectrumData == NULL)
	{
		return;
	}

	/* 監視データ取得 */
	if (pCSectrumData->GetSpectrumData() != ERR_RETURN && m_pSpectrumView != NULL) 
	{
		// ビュー描画
		m_pSpectrumView->Invalidate();
	}

	CWnd::OnTimer(nIDEvent);
}


/*============================================================================*/
/*! CSummaryGraphCtrl

-# ウィンドウ破棄時に呼び出される

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSummaryGraphCtrl::OnDestroy()
{
	CWnd::OnDestroy();

	KillTimer(1);
}

/*============================================================================*/
/*! サマリグラフ

-# レイアウト読み込み（設定読み込み）

@param  なし
@retval	なし

*/
/*============================================================================*/
bool CSummaryGraphCtrl::Load(CArchive& ar)
{
	int nViewID = mID - eSummaryCtrl;
	CPropertiesData& prop = theApp.GetSatelliteData().GetSummaryPropertiesData(mStationIndex, nViewID);
	if (prop.nGraphIdx == eSummaryWndSetType_XSP && mStationIndex != eStation_Uchinoura20 ||
		prop.nGraphIdx == eSummaryWndSetType_KASP && mStationIndex == eStation_Usuda642)
	{
		// パラメータ更新のため、パラメータを読み込みに行く
		if (m_pSpectrumView != NULL)
			m_pSpectrumView->Invalidate();
	}
	else
	{
		// パラメータ更新のため、パラメータを読み込みに行く
		AddViewParam();
	}
	return true;
}

/*============================================================================*/
/*! サマリグラフ

-# レイアウト保存（設定保存）

@param  なし
@retval	なし

*/
/*============================================================================*/
bool CSummaryGraphCtrl::Save(CArchive& ar)
{
	return true;
}
