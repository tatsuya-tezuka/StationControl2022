
// StationControlView.cpp : CStationControlView クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "StationControl.h"
#endif

#include "StationControlDoc.h"
#include "StationControlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStationControlView

IMPLEMENT_DYNCREATE(CStationControlView, CView)

BEGIN_MESSAGE_MAP(CStationControlView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CStationControlView コンストラクション/デストラクション

CStationControlView::CStationControlView()
{
	// TODO: 構築コードをここに追加します。

}

CStationControlView::~CStationControlView()
{
}

BOOL CStationControlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CStationControlView 描画

void CStationControlView::OnDraw(CDC* /*pDC*/)
{
	CStationControlDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}

void CStationControlView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CStationControlView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CStationControlView 診断

#ifdef _DEBUG
void CStationControlView::AssertValid() const
{
	CView::AssertValid();
}

void CStationControlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CStationControlDoc* CStationControlView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CStationControlView メッセージ ハンドラー
