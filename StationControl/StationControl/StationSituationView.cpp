/*============================================================================*/
/*! �q���ߑ���

-#

@param
@retval

*/
/*============================================================================*/
// StationSituationView.cpp : �����t�@�C��
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


// CStationSituationView �`��

void CStationSituationView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
}


// CStationSituationView �f�f

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
CStationControlDoc* CStationSituationView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CStationSituationView ���b�Z�[�W �n���h���[


void CStationSituationView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	GetDocument()->SetTitleEx();
}
