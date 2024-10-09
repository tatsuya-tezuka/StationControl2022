
// OperationTimeLineView.cpp : COperationTimeLineView �N���X�̎���
//

#include "stdafx.h"
// SHARED_HANDLERS �́A�v���r���[�A�k���ŁA����ь����t�B���^�[ �n���h���[���������Ă��� ATL �v���W�F�N�g�Œ�`�ł��A
// ���̃v���W�F�N�g�Ƃ̃h�L�������g �R�[�h�̋��L���\�ɂ��܂��B
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
	// �W������R�}���h
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &COperationTimeLineView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// COperationTimeLineView �R���X�g���N�V����/�f�X�g���N�V����

COperationTimeLineView::COperationTimeLineView()
{
	// TODO: �\�z�R�[�h�������ɒǉ����܂��B

}

COperationTimeLineView::~COperationTimeLineView()
{
}

BOOL COperationTimeLineView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

// COperationTimeLineView �`��

void COperationTimeLineView::OnDraw(CDC* /*pDC*/)
{
	COperationTimeLineDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B
}


// COperationTimeLineView ���


void COperationTimeLineView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL COperationTimeLineView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// ����̈������
	return DoPreparePrinting(pInfo);
}

void COperationTimeLineView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ����O�̓��ʂȏ�����������ǉ����Ă��������B
}

void COperationTimeLineView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �����̌㏈����ǉ����Ă��������B
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


// COperationTimeLineView �f�f

#ifdef _DEBUG
void COperationTimeLineView::AssertValid() const
{
	CView::AssertValid();
}

void COperationTimeLineView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COperationTimeLineDoc* COperationTimeLineView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COperationTimeLineDoc)));
	return (COperationTimeLineDoc*)m_pDocument;
}
#endif //_DEBUG


// COperationTimeLineView ���b�Z�[�W �n���h���[
