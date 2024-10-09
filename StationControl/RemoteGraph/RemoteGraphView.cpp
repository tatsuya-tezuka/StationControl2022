// この MFC サンプル ソース コードでは、MFC Microsoft Office Fluent ユーザー インターフェイス 
// ("Fluent UI") の使用方法を示します。このコードは、MFC C++ ライブラリ ソフトウェアに 
// 同梱されている Microsoft Foundation Class リファレンスおよび関連電子ドキュメントを
// 補完するための参考資料として提供されます。
// Fluent UI を複製、使用、または配布するためのライセンス条項は個別に用意されています。
// Fluent UI ライセンス プログラムの詳細については、Web サイト
// http://go.microsoft.com/fwlink/?LinkId=238214 を参照してください。
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// RemoteGraphView.cpp : CRemoteGraphView クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "RemoteGraph.h"
#endif

#include "MainFrm.h"

#include "RemoteGraphDoc.h"
#include "RemoteGraphView.h"

#include <iostream>
#include <string>
#include <fstream> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// なんちゃってデータ用修正
BOOL CRemoteGraphView::m_bUsedClassFirst = TRUE;
int CRemoteGraphView::m_initAngle[];


#if 0
UINT CRemoteGraphViewUpdateThread_Trend(LPVOID pParam)
{
	CRemoteGraphView	*pWnd = (CRemoteGraphView*)pParam;

	while (TRUE)
	{
		WaitForSingleObject(((CMainFrame*)(theApp.m_pMainWnd))->GetUpdateEvent()->m_hObject, INFINITE);

		if (pWnd->m_bUpdateThread == FALSE)
		{
			break;
		}

		// 監視データが更新されたため、ビューに更新を通知
		pWnd->UpdateGraph();
	}

	return 0;
}
#endif

// CRemoteGraphView

IMPLEMENT_DYNCREATE(CRemoteGraphView, CView)

BEGIN_MESSAGE_MAP(CRemoteGraphView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CRemoteGraphView コンストラクション/デストラクション

CRemoteGraphView::CRemoteGraphView()
{
	// TODO: 構築コードをここに追加します。

}

CRemoteGraphView::~CRemoteGraphView()
{
}

BOOL CRemoteGraphView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CRemoteGraphView 描画

void CRemoteGraphView::OnDraw(CDC* /*pDC*/)
{
	CRemoteGraphDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}

void CRemoteGraphView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRemoteGraphView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CRemoteGraphView 診断

#ifdef _DEBUG
void CRemoteGraphView::AssertValid() const
{
	CView::AssertValid();
}

void CRemoteGraphView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRemoteGraphDoc* CRemoteGraphView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRemoteGraphDoc)));
	return (CRemoteGraphDoc*)m_pDocument;
}
#endif //_DEBUG



void CRemoteGraphView::CreateGraph(UINT nViewID, UINT nViewType, UINT nWindowNo)
{
	CRect rect;
	GetClientRect(&rect);
	m_Graph.SetViewID(nViewID);
	//m_nViewType = eGraphType_All;
	m_nViewType = nViewType;
	m_Graph.SetGraphType(m_nViewType);
	m_Graph.Create(WS_VISIBLE, rect, this, mCtrlTrend, nWindowNo);
	m_Graph.SetGridType(CGraphWindow::eGridType_DOT);

#if 0
	// イベントのスレッド作成
	CEvent m_evtUpdate = CEvent(FALSE, FALSE, _T("CRemoteGraphView_UpdateEvent"));
	m_bUpdateThread = TRUE;
	m_pUpdateThread = AfxBeginThread(CRemoteGraphViewUpdateThread_Trend, this);
#endif
}

void CRemoteGraphView::DeleteGraph()
{
#if 0
	m_bUpdateThread = FALSE;
#endif
	m_Graph.DestroyWindow();
#if 0
	delete m_pUpdateThread;
#endif
}

#if 0
void CRemoteGraphView::GetDummyData(int pos, float& data)
{
	if (pos == 0){
		for (int i = 0; i < sizeof(m_angle) / sizeof(m_angle[0]); ++i) {
			m_angle[i] = (m_angle[i] + 2 * (i + 1)) % 360;
			//fprintf_s(m_fp, "View(%d), pos(%d), m_angle[%d][%d]=%d\n", m_nViewType, pos, m_nViewType, i, m_angle[m_nViewType][i]);
		}
	}
	data = (float)((sin(m_angle[pos] * 3.1415926538 / 180.0) + 1) / 2 * (100 - 0) + 0);
	//fprintf_s(m_fp, "View(%d), pos(%d), data=%f\n", m_nViewType, pos, data);

	if (pos == 3){
		data = m_fLogData;
		if (m_fLogData == 0.0f)
			m_fLogData = 10.0f;
		else
			m_fLogData = 0.0f;
	}
}
#endif

void CRemoteGraphView::SetViewType(UINT val)
{
	m_nViewType = val;
	switch (val){
	case	eGraphType_OffAll:
	case	eGraphType_OffTrend:
		m_Graph.SetGraphType(m_nViewType);
		m_Graph.SetHzRange((double)CSatelliteData::mMaxDataCount);
		m_Graph.UpdateGraph();
		break;
	default:
		break;
	}
}


// CRemoteGraphView メッセージ ハンドラー

void CRemoteGraphView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// なんちゃって用データの初期データ値はクラス変数で、全トレンド範囲トレンド合わせて一回のみ作成
	if (m_bUsedClassFirst) {
		for (int i = 0; i < sizeof(m_angle) / sizeof(m_angle[0]); ++i) {
			m_initAngle[i] = rand() * 360 / RAND_MAX;
		}
		m_bUsedClassFirst = FALSE;
	}

	for (int i = 0; i < sizeof(m_angle) / sizeof(m_angle[0]); ++i) {
		m_angle[i] = m_initAngle[i];
	}

	m_fLogData = 0.0f;
}


void CRemoteGraphView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CRect rectCtrl;
	CRect rect;

	// グラフ領域のリサイズ
	if (m_Graph.GetSafeHwnd()){
		m_Graph.GetWindowRect(&rectCtrl);
		ScreenToClient(rectCtrl);
		GetClientRect(&rect);
		rect.left = rectCtrl.left;
		rect.top = rectCtrl.top;
		//rect.bottom -= 50;
		m_Graph.MoveWindow(&rect);

		// ズーム枠を更新
		CWnd	*pWnd = GetParent();

		if( pWnd->GetSafeHwnd() != NULL )
		{
			pWnd = pWnd->GetParent();

			if( pWnd->GetSafeHwnd() != NULL )
			{
				// 監視データが更新されたため、ビューに更新を通知
				if( !m_Graph.GetRealTime() )
				{
					pWnd->PostMessage(eMessage_UpdateZoomRectangle, 0);
					pWnd->PostMessage(eMessage_UpdateCursorLine, 1);
				}
			}
		}
	}
}


BOOL CRemoteGraphView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CView::OnEraseBkgnd(pDC);
}

/*============================================================================*/
/*! グラフビュー

-# オンラインならば再描画する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CRemoteGraphView::UpdateGraphWindow(int nViewNo, bool bClear)
{
//	if (((CMainFrame*)theApp.m_pMainWnd)->GetOnline() == TRUE || bForce == TRUE)
	//if (((CMainFrame*)theApp.m_pMainWnd)->GetOnline() == TRUE)
	{
		if (nViewNo >= 0)
			m_Graph.SetViewID(nViewNo);

		// Min/Maxを作成
		m_Graph.CreateMinMaxValue(bClear);

		// 再描画する
		m_Graph.UpdateGraph();
	}
}

/*============================================================================*/
/*! グラフビュー

-# オンラインならば再描画する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CRemoteGraphView::SetRealTime(BOOL val)
{
	m_Graph.SetRealTime(val);
}

BOOL CRemoteGraphView::AddLine(UINT nID, CString strName, CString strGName, UINT nGraphKind, COLORREF crColor, DWORD nStyle, int nWidth, VScaleKind nVScaleKind, BOOL bVisible)
{
	return m_Graph.AddLine(nID, strName, strGName, nGraphKind, crColor, nStyle, nWidth, nVScaleKind, bVisible);
}
BOOL CRemoteGraphView::AddDot(UINT nID, CString strName, CString strGName, UINT nGraphKind, COLORREF crColor, int nDotSize, VScaleKind nVScaleKind, BOOL bVisible)
{
	return m_Graph.AddDot(nID, strName, strGName, nGraphKind, crColor, nDotSize, nVScaleKind, bVisible);
}
BOOL CRemoteGraphView::AddLineAndDot(UINT nID, CString strName, CString strGName, UINT nGraphKind, COLORREF crColor, DWORD nStyle, int nWidth, int nDotSize, VScaleKind nVScaleKind, BOOL bVisible)
{
	return m_Graph.AddLineAndDot(nID, strName, strGName, nGraphKind, crColor, nStyle, nWidth, nDotSize, nVScaleKind, bVisible);
}
BOOL CRemoteGraphView::AddAxis(stVScale nStvScale, HScale trandHScale)
{
	return m_Graph.AddAxis(nStvScale, trandHScale);
}


void CRemoteGraphView::OnClose()
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CView::OnClose();
}

void CRemoteGraphView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch( nChar )
	{
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
	case VK_PRIOR:
	case VK_NEXT:
//		if( GetKeyState(VK_CONTROL) < 0 )
		{
			CWnd	*pWnd = GetParent();

			if( pWnd->GetSafeHwnd() != NULL )
			{
				pWnd = pWnd->GetParent();

				if( pWnd->GetSafeHwnd() != NULL )
					pWnd->PostMessage(eMessage_AnalysisKeyEvent, nChar);
			}
		}
		break;

	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CRemoteGraphView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if( GetKeyState(VK_CONTROL) < 0 )
	{
		CWnd	*pWnd = GetParent();

		if( pWnd->GetSafeHwnd() != NULL )
		{
			pWnd = pWnd->GetParent();

			if( pWnd->GetSafeHwnd() != NULL )
				pWnd->PostMessage(eMessage_AnalysisKeyEvent, (zDelta < 0) ? VK_DOWN : VK_UP);
		}
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
