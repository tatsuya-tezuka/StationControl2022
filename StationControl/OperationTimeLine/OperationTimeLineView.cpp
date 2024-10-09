
// OperationTimeLineView.cpp : COperationTimeLineView クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "OperationTimeLine.h"
#endif

#include "OperationTimeLineDoc.h"
#include "OperationTimeLineView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COperationTimeLineView

IMPLEMENT_DYNCREATE(COperationTimeLineView, CView)

BEGIN_MESSAGE_MAP(COperationTimeLineView, CView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &COperationTimeLineView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// COperationTimeLineView コンストラクション/デストラクション

COperationTimeLineView::COperationTimeLineView()
{
	// TODO: 構築コードをここに追加します。

}

COperationTimeLineView::~COperationTimeLineView()
{
}

BOOL COperationTimeLineView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// COperationTimeLineView 描画

void COperationTimeLineView::OnDraw(CDC* /*pDC*/)
{
	COperationTimeLineDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


// COperationTimeLineView 印刷


void COperationTimeLineView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL COperationTimeLineView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void COperationTimeLineView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void COperationTimeLineView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}

void COperationTimeLineView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void COperationTimeLineView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// COperationTimeLineView 診断

#ifdef _DEBUG
void COperationTimeLineView::AssertValid() const
{
	CView::AssertValid();
}

void COperationTimeLineView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COperationTimeLineDoc* COperationTimeLineView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COperationTimeLineDoc)));
	return (COperationTimeLineDoc*)m_pDocument;
}
#endif //_DEBUG


// COperationTimeLineView メッセージ ハンドラー
