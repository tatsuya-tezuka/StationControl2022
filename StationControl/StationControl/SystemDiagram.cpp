/*============================================================================*/
/*! �n���}

-#

@param
@retval

*/
/*============================================================================*/
// SystemDiagram.cpp : �����t�@�C��
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


// CSystemDiagram �`��

void CSystemDiagram::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
}


// CSystemDiagram �f�f

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
CStationControlDoc* CSystemDiagram::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CSystemDiagram ���b�Z�[�W �n���h���[


void CSystemDiagram::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	GetDocument()->SetTitleEx();
}
