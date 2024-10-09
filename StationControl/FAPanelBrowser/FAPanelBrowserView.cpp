
// FAPanelBrowserView.cpp : CFAPanelBrowserView クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "FAPanelBrowser.h"
#endif

#include "FAPanelBrowserDoc.h"
#include "FAPanelBrowserView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFAPanelBrowserView

IMPLEMENT_DYNCREATE(CFAPanelBrowserView, CView)

BEGIN_MESSAGE_MAP(CFAPanelBrowserView, CView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CFAPanelBrowserView コンストラクション/デストラクション

CFAPanelBrowserView::CFAPanelBrowserView()
{
	// TODO: 構築コードをここに追加します。

}

CFAPanelBrowserView::~CFAPanelBrowserView()
{
}

BOOL CFAPanelBrowserView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CFAPanelBrowserView 描画

void CFAPanelBrowserView::OnDraw(CDC* /*pDC*/)
{
	CFAPanelBrowserDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


// CFAPanelBrowserView 印刷

BOOL CFAPanelBrowserView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CFAPanelBrowserView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CFAPanelBrowserView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}


// CFAPanelBrowserView 診断

#ifdef _DEBUG
void CFAPanelBrowserView::AssertValid() const
{
	CView::AssertValid();
}

void CFAPanelBrowserView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFAPanelBrowserDoc* CFAPanelBrowserView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFAPanelBrowserDoc)));
	return (CFAPanelBrowserDoc*)m_pDocument;
}
#endif //_DEBUG


// CFAPanelBrowserView メッセージ ハンドラー
