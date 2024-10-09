// EquipmentSystemForm.cpp : 実装ファイル
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


// CEquipmentSystemForm 診断

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
COperationTimeLineDoc* CEquipmentSystemForm::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COperationTimeLineDoc)));
	return (COperationTimeLineDoc*)m_pDocument;
}
#endif //_DEBUG


// CEquipmentSystemForm メッセージ ハンドラー


void CEquipmentSystemForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	GetDocument()->SetTitleEx(_T("設備系統"));

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
