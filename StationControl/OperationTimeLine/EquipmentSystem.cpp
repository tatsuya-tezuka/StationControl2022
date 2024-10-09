// EquipmentSystem.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "OperationTimeLineDoc.h"
#include "EquipmentSystem.h"


// CEquipmentSystem

IMPLEMENT_DYNCREATE(CEquipmentSystem, CFormView)

CEquipmentSystem::CEquipmentSystem()
	: CFormView(CEquipmentSystem::IDD)
{
	mSaveCx = mSaveCy = 0;
	mpDlg = NULL;
}

CEquipmentSystem::~CEquipmentSystem()
{
	vector<CWnd*>::iterator itr;
	for (itr = mControlList.begin(); itr != mControlList.end(); itr++){
		if ((*itr)->GetSafeHwnd()){
			(*itr)->DestroyWindow();
		}
		delete (*itr);
	}
	if (mpDlg != NULL)
		delete mpDlg;
}

void CEquipmentSystem::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEquipmentSystem, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CEquipmentSystem 診断

#ifdef _DEBUG
void CEquipmentSystem::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CEquipmentSystem::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
COperationTimeLineDoc* CEquipmentSystem::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COperationTimeLineDoc)));
	return (COperationTimeLineDoc*)m_pDocument;
}
#endif //_DEBUG


// CEquipmentSystem メッセージ ハンドラー


void CEquipmentSystem::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CRect rect;
	GetClientRect(rect);

	GetParentFrame()->RecalcLayout();

	GetDocument()->SetTitleEx(mTimeViewTitle[eTimeViewType_OperatioTimeline]);

	int	nMapMode;
	GetDeviceScrollSizes(nMapMode, mSizeTotal, mSizePage, mSizeLine);
	//SetScaleToFitSize(mSizeTotal);
	ResizeParentToFit(FALSE);

	// システムコマンドSC_CLOSEのディセーブル
	CMenu* pSysMenu = GetParent()->GetSystemMenu(FALSE);
	if (pSysMenu)	pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

	createTab();
}

/*============================================================================*/
/*! 設備状況ビュー

-# 各種タブコントロールの作成

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentSystem::createTab()
{
	mControlList.clear();
#ifndef _CUT
	CRect rectTab;
	int pos = 0;
	GetDlgItem(IDC_EQSYSTEM_LOC)->GetWindowRect(&rectTab);
	ScreenToClient(&rectTab);
	mTabCtrl.Create(CMFCTabCtrl::STYLE_3D, rectTab, this, 1, CMFCTabCtrl::LOCATION_TOP);
	for (int i = 0; i < eTabMax; i++){
		CDialogEx *pdlg = (CDialogEx*)(mTabItem[i].pset->CreateObject());
		if (!pdlg->Create(mTabItem[i].id, &mTabCtrl)){
			TRACE("Failed view creation\n");
			continue;
		}
		mTabCtrl.AddTab(pdlg, mTabItem[i].name, 0, FALSE);
		mControlList.push_back(pdlg);
	}
	mTabCtrl.Initialize();
#else
	CRect rectTab;
	int pos = 0;
	GetClientRect(rectTab);
	mpDlg = (CDialogEx*)(mTabItem[0].pset->CreateObject());
	if (!mpDlg->Create(mTabItem[0].id, this)){
		TRACE("Failed view creation\n");
		return;
	}
#endif
}


void CEquipmentSystem::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// クライアント領域の取得
	CRect rect, client;
	GetClientRect(client);

#ifndef _CUT
	if (mTabCtrl.GetSafeHwnd()){
		mTabCtrl.MoveWindow(client);
	}
#else
	if (mpDlg->GetSafeHwnd()){
		mpDlg->MoveWindow(client);
	}
#endif
}
