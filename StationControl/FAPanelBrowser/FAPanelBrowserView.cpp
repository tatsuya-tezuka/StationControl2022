
// FAPanelBrowserView.cpp : CFAPanelBrowserView �N���X�̎���
//

#include "stdafx.h"
// SHARED_HANDLERS �́A�v���r���[�A�k���ŁA����ь����t�B���^�[ �n���h���[���������Ă��� ATL �v���W�F�N�g�Œ�`�ł��A
// ���̃v���W�F�N�g�Ƃ̃h�L�������g �R�[�h�̋��L���\�ɂ��܂��B
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
	// �W������R�}���h
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CFAPanelBrowserView �R���X�g���N�V����/�f�X�g���N�V����

CFAPanelBrowserView::CFAPanelBrowserView()
{
	// TODO: �\�z�R�[�h�������ɒǉ����܂��B

}

CFAPanelBrowserView::~CFAPanelBrowserView()
{
}

BOOL CFAPanelBrowserView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

// CFAPanelBrowserView �`��

void CFAPanelBrowserView::OnDraw(CDC* /*pDC*/)
{
	CFAPanelBrowserDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B
}


// CFAPanelBrowserView ���

BOOL CFAPanelBrowserView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// ����̈������
	return DoPreparePrinting(pInfo);
}

void CFAPanelBrowserView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ����O�̓��ʂȏ�����������ǉ����Ă��������B
}

void CFAPanelBrowserView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �����̌㏈����ǉ����Ă��������B
}


// CFAPanelBrowserView �f�f

#ifdef _DEBUG
void CFAPanelBrowserView::AssertValid() const
{
	CView::AssertValid();
}

void CFAPanelBrowserView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFAPanelBrowserDoc* CFAPanelBrowserView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFAPanelBrowserDoc)));
	return (CFAPanelBrowserDoc*)m_pDocument;
}
#endif //_DEBUG


// CFAPanelBrowserView ���b�Z�[�W �n���h���[
