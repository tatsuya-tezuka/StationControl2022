/*============================================================================*/
/*! 系統図

-#

@param
@retval

*/
/*============================================================================*/
// SystemDiagram.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "SystemDiagram.h"


// CSystemDiagram

IMPLEMENT_DYNCREATE(CSystemDiagram, CView)

CSystemDiagram::CSystemDiagram()
{

}

CSystemDiagram::~CSystemDiagram()
{
}

BEGIN_MESSAGE_MAP(CSystemDiagram, CView)
END_MESSAGE_MAP()


// CSystemDiagram 描画

void CSystemDiagram::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 描画コードをここに追加してください。
}


// CSystemDiagram 診断

#ifdef _DEBUG
void CSystemDiagram::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CSystemDiagram::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
CStationControlDoc* CSystemDiagram::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CSystemDiagram メッセージ ハンドラー


void CSystemDiagram::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	GetDocument()->SetTitleEx();
}
