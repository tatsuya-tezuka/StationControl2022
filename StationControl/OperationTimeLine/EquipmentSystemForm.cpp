// EquipmentSystemForm.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "OperationTimeLineDoc.h"
#include "EquipmentSystemForm.h"


// CEquipmentSystemForm

IMPLEMENT_DYNCREATE(CEquipmentSystemForm, CFormView)

CEquipmentSystemForm::CEquipmentSystemForm()
	: CFormView(CEquipmentSystemForm::IDD)
{

}

CEquipmentSystemForm::~CEquipmentSystemForm()
{
}

void CEquipmentSystemForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FAPANEL1, m_FaBlock);
}

BEGIN_MESSAGE_MAP(CEquipmentSystemForm, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CEquipmentSystemForm �f�f

#ifdef _DEBUG
void CEquipmentSystemForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CEquipmentSystemForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
COperationTimeLineDoc* CEquipmentSystemForm::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COperationTimeLineDoc)));
	return (COperationTimeLineDoc*)m_pDocument;
}
#endif //_DEBUG


// CEquipmentSystemForm ���b�Z�[�W �n���h���[


void CEquipmentSystemForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	GetDocument()->SetTitleEx(_T("�ݔ��n��"));

	CString fname;
	fname.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppFaPanelPath(), mFapanelBlockFile[theApp.GetSelectStation()]);
	m_FaBlock.SetFileName(fname);
}


void CEquipmentSystemForm::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if (m_FaBlock.GetSafeHwnd()){
		CRect rect;
		GetClientRect(rect);
		m_FaBlock.MoveWindow(rect);
	}
}
