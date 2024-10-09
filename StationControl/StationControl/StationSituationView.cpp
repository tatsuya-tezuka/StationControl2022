/*============================================================================*/
/*! 衛星捕捉状況

-#

@param
@retval

*/
/*============================================================================*/
// StationSituationView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "StationSituationView.h"


// CStationSituationView

IMPLEMENT_DYNCREATE(CStationSituationView, CView)

CStationSituationView::CStationSituationView()
{

}

CStationSituationView::~CStationSituationView()
{
}

BEGIN_MESSAGE_MAP(CStationSituationView, CView)
END_MESSAGE_MAP()


// CStationSituationView 描画

void CStationSituationView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 描画コードをここに追加してください。
}


// CStationSituationView 診断

#ifdef _DEBUG
void CStationSituationView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CStationSituationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
CStationControlDoc* CStationSituationView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CStationSituationView メッセージ ハンドラー


void CStationSituationView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	GetDocument()->SetTitleEx();
}
