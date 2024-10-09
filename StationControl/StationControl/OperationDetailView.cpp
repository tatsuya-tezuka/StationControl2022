/*============================================================================*/
/*! �^�p�ڍ�

-#

@param
@retval

*/
/*============================================================================*/
// OperationDetailView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "OperationDetailView.h"


// COperationDetailView

IMPLEMENT_DYNCREATE(COperationDetailView, CView)

COperationDetailView::COperationDetailView()
{

}

COperationDetailView::~COperationDetailView()
{
}

BEGIN_MESSAGE_MAP(COperationDetailView, CView)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// COperationDetailView �`��

void COperationDetailView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
}


// COperationDetailView �f�f

#ifdef _DEBUG
void COperationDetailView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void COperationDetailView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
CStationControlDoc* COperationDetailView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


// COperationDetailView ���b�Z�[�W �n���h���[


void COperationDetailView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	GetDocument()->SetTitleEx();
}


void COperationDetailView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	// �`�惁�b�Z�[�W�� CView::OnPaint() ���Ăяo���Ȃ��ł��������B

	return;

	CRect rect;
	GetClientRect(rect);

	CDC memDC;
	CBitmap bitm;
	// �`��c�b�̍쐬
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	CBitmap* pOldBitmap = NULL;
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	pOldBitmap = memDC.SelectObject(&bitmap);

	COLORREF color = memDC.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	// �o�b�N�O���E���h ���[�h��ݒ�
	memDC.SetBkMode(TRANSPARENT);
	// �w�i�F�̐ݒ�
	memDC.FillSolidRect(&rect, ::GetSysColor(COLOR_WINDOW));
	// �f�t�H���g�t�H���g�̐ݒ�
	CFont txtFont;
	txtFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (txtFont.GetLogFont(&lf)){
		lf.lfHeight = 12;
		txtFont.DeleteObject();
		txtFont.CreateFontIndirect(&lf);
	}
	CFont* oldFont = memDC.SelectObject(&txtFont);
	CSize sizeText = memDC.GetTextExtent(_T("W"));

	CRect rc1 = rect;
	CRect rc2 = rect;
	CRect rc3 = rect;

	rc1.left = 10;
	rc2.left = 10;
	rc3.left = 10;
	rc1.top = 10;
	rc2.top = 10;
	rc3.top = 10;

	sizeText.cy += 2;
	rc2.left += memDC.GetTextExtent(_T("����J�n  ")).cx;
	rc3.left += memDC.GetTextExtent(_T("����J�n  :  ")).cx;

	memDC.TextOut(rc1.left, rc1.top, _T("����J�n"));
	memDC.TextOut(rc2.left, rc2.top, _T(":"));
	memDC.TextOut(rc3.left, rc3.top, _T("CCYY/MM/DD hh:mm:ss"));
	rc1.top += sizeText.cy;	rc1.bottom += sizeText.cy;
	rc2.top += sizeText.cy;	rc2.bottom += sizeText.cy;
	rc3.top += sizeText.cy;	rc3.bottom += sizeText.cy;
	memDC.TextOut(rc1.left, rc1.top, _T("�^�p�J�n"));
	memDC.TextOut(rc2.left, rc2.top, _T(":"));
	memDC.TextOut(rc3.left, rc3.top, _T("CCYY/MM/DD hh:mm:ss"));
	rc1.top += sizeText.cy;	rc1.bottom += sizeText.cy;
	rc2.top += sizeText.cy;	rc2.bottom += sizeText.cy;
	rc3.top += sizeText.cy;	rc3.bottom += sizeText.cy;
	memDC.TextOut(rc1.left, rc1.top, _T("U/L STA"));
	memDC.TextOut(rc2.left, rc2.top, _T(":"));
	memDC.TextOut(rc3.left, rc3.top, _T("CCYY/MM/DD hh:mm:ss"));
	rc1.top += sizeText.cy;	rc1.bottom += sizeText.cy;
	rc2.top += sizeText.cy;	rc2.bottom += sizeText.cy;
	rc3.top += sizeText.cy;	rc3.bottom += sizeText.cy;
	memDC.TextOut(rc1.left, rc1.top, _T("CMD AOS"));
	memDC.TextOut(rc2.left, rc2.top, _T(":"));
	memDC.TextOut(rc3.left, rc3.top, _T("CCYY/MM/DD hh:mm:ss"));
	rc1.top += sizeText.cy;	rc1.bottom += sizeText.cy;
	rc2.top += sizeText.cy;	rc2.bottom += sizeText.cy;
	rc3.top += sizeText.cy;	rc3.bottom += sizeText.cy;
	memDC.TextOut(rc1.left, rc1.top, _T("CMD LOS"));
	memDC.TextOut(rc2.left, rc2.top, _T(":"));
	memDC.TextOut(rc3.left, rc3.top, _T("CCYY/MM/DD hh:mm:ss"));
	rc1.top += sizeText.cy;	rc1.bottom += sizeText.cy;
	rc2.top += sizeText.cy;	rc2.bottom += sizeText.cy;
	rc3.top += sizeText.cy;	rc3.bottom += sizeText.cy;
	memDC.TextOut(rc1.left, rc1.top, _T("U/L END"));
	memDC.TextOut(rc2.left, rc2.top, _T(":"));
	memDC.TextOut(rc3.left, rc3.top, _T("CCYY/MM/DD hh:mm:ss"));
	rc1.top += sizeText.cy;	rc1.bottom += sizeText.cy;
	rc2.top += sizeText.cy;	rc2.bottom += sizeText.cy;
	rc3.top += sizeText.cy;	rc3.bottom += sizeText.cy;
	memDC.TextOut(rc1.left, rc1.top, _T("�^�p�I��"));
	memDC.TextOut(rc2.left, rc2.top, _T(":"));
	memDC.TextOut(rc3.left, rc3.top, _T("CCYY/MM/DD hh:mm:ss"));

	rc1.top += (sizeText.cy*3);	rc1.bottom += (sizeText.cy*3);
	rc2.top += (sizeText.cy * 3);	rc2.bottom += (sizeText.cy * 3);
	rc3.top += (sizeText.cy * 3);	rc3.bottom += (sizeText.cy * 3);
	memDC.TextOut(rc1.left, rc1.top, _T("LOCK����"));
	rc1.top += sizeText.cy;	rc1.bottom += sizeText.cy;
	rc2.top += sizeText.cy;	rc2.bottom += sizeText.cy;
	rc3.top += sizeText.cy;	rc3.bottom += sizeText.cy;
	memDC.TextOut(rc1.left, rc1.top, _T("�^���⑫"));

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

}
