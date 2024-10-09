
// StationControlView.cpp : CStationControlView �N���X�̎���
//

#include "stdafx.h"
// SHARED_HANDLERS �́A�v���r���[�A�k���ŁA����ь����t�B���^�[ �n���h���[���������Ă��� ATL �v���W�F�N�g�Œ�`�ł��A
// ���̃v���W�F�N�g�Ƃ̃h�L�������g �R�[�h�̋��L���\�ɂ��܂��B
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

// CStationControlView �R���X�g���N�V����/�f�X�g���N�V����

CStationControlView::CStationControlView()
{
	// TODO: �\�z�R�[�h�������ɒǉ����܂��B

}

CStationControlView::~CStationControlView()
{
}

BOOL CStationControlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

// CStationControlView �`��

void CStationControlView::OnDraw(CDC* /*pDC*/)
{
	CStationControlDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B
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


// CStationControlView �f�f

#ifdef _DEBUG
void CStationControlView::AssertValid() const
{
	CView::AssertValid();
}

void CStationControlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CStationControlDoc* CStationControlView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CStationControlView ���b�Z�[�W �n���h���[
