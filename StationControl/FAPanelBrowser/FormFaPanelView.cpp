// FormFaPanelView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "FAPanelBrowser.h"
#include "FAPanelBrowserDoc.h"
#include "FormFaPanelView.h"


// CFormFaPanelView

IMPLEMENT_DYNCREATE(CFormFaPanelView, CFormView)

CFormFaPanelView::CFormFaPanelView()
	: CFormView(CFormFaPanelView::IDD)
{

}

CFormFaPanelView::~CFormFaPanelView()
{
}

void CFormFaPanelView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FAPANEL1, mFaPanelCtrl);
}

BEGIN_MESSAGE_MAP(CFormFaPanelView, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CFormFaPanelView �f�f

#ifdef _DEBUG
void CFormFaPanelView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormFaPanelView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif

CFAPanelBrowserDoc* CFormFaPanelView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFAPanelBrowserDoc)));
	return (CFAPanelBrowserDoc*)m_pDocument;
}
#endif //_DEBUG


// CFormFaPanelView ���b�Z�[�W �n���h���[


void CFormFaPanelView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CRect rect;
	GetClientRect(rect);

	// �E�B���h�E�������킹��
	GetParentFrame()->RecalcLayout();
	int	nMapMode;
	GetDeviceScrollSizes(nMapMode, mSizeTotal, mSizePage, mSizeLine);
	ResizeParentToFit(FALSE);

	// �^�C�g��
	GetDocument()->SetTitle(theApp.GetTitle());
	if (mFaPanelCtrl.GetSafeHwnd()){
		// XML�t�@�C��
		mFaPanelCtrl.SetFileName(theApp.GetXmFile());
	}
}


void CFormFaPanelView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if (mFaPanelCtrl.GetSafeHwnd()){
		CRect rect;
		GetClientRect(rect);
		mFaPanelCtrl.MoveWindow(rect);
	}
}
