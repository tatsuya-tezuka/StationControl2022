/*============================================================================*/
/*! �ݔ��E�^�p��

-#

@param
@retval

*/
/*============================================================================*/
// EquipmentOperationView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "EquipmentOperationView.h"


// CEquipmentOperationView

IMPLEMENT_DYNCREATE(CEquipmentOperationView, CView)

CEquipmentOperationView::CEquipmentOperationView()
{
}

CEquipmentOperationView::~CEquipmentOperationView()
{
}

BEGIN_MESSAGE_MAP(CEquipmentOperationView, CView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CEquipmentOperationView �`��

void CEquipmentOperationView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
}


// CEquipmentOperationView �f�f

#ifdef _DEBUG
void CEquipmentOperationView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CEquipmentOperationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
CStationControlDoc* CEquipmentOperationView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CEquipmentOperationView ���b�Z�[�W �n���h���[


void CEquipmentOperationView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	GetDocument()->SetTitleEx();

	// �N���C�A���g�̈�擾
	CRect rect;
	GetClientRect(rect);

	// �ݔ���Ԃ̍쐬
	int w = rect.Width() / 2;
	int h = rect.Height() / 3;
	mGridEquipment.SetTitle(CString(mTitleEquipment));
	mGridEquipment.SetGridNum(CPoint(5,4));
#ifdef _DEBUG
	for (int x = 0; x < 5; x++){
		for (int y = 0; y < 4; y++){
			mGridEquipment.AddItem(x, y, mEqString[y][x], RGB(0, 128, 255));
		}
	}
#endif
	mGridEquipment.Create(this, rect, eGridCtrl_Equipment);

	// �^�p��Ԃ̍쐬
	mGridOperation.SetTitle(CString(mTitleOperation));
	mGridOperation.SetGridNum(CPoint(5, 4));
#ifdef _DEBUG
	for (int x = 0; x < 5; x++){
		for (int y = 0; y < 4; y++){
			mGridOperation.AddItem(x, y, mOpeString[y][x], RGB(0,128,255));
		}
	}
#endif
	mGridOperation.Create(this, rect, eGridCtrl_Operation);

	// �ǔ��O���t�̍쐬
	//mTrack.Create(this, rect, eTrackingCtrl);
	CRuntimeClass* pClass = RUNTIME_CLASS(CForecastRadarGraphView);
	if (pClass == NULL)
	{
		return;
	}

	mpTrackView = (CForecastRadarGraphView*)(pClass->CreateObject());
	if (mpTrackView != NULL)
	{
		mpTrackView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, 0, NULL);
		mpTrackView->SendMessage(WM_INITIALUPDATE);
	}

	// �T�}���O���t�̍쐬
	for (int i = 0; i < mSummaryGraphMax / 2; i++){
		for (int j = 0; j < mSummaryGraphMax / 2; j++){
			mSummary[i*(mSummaryGraphMax / 2) + j].Create(this, rect, theApp.GetSelectStation(), eSummaryCtrl + (i*(mSummaryGraphMax / 2) + j));
		}
	}

}


void CEquipmentOperationView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// �N���C�A���g�̈�̎擾
	CRect rect, client;
	GetClientRect(client);
	
	int w, h;
	w = client.Width();
	h = client.Height() / 3;

	if (mGridEquipment.GetSafeHwnd()){
		// ���F�S��
		// �����F�S�̍�����1/3
		rect = client;
		w = rect.Width();
		h = rect.Height() / 3;
		rect.bottom = rect.top + h;
		mGridEquipment.MoveWindow(rect);
	}
	if (mGridOperation.GetSafeHwnd()){
		// ���F�S��
		// �����F�S�̍�����1/3
		rect = client;
		w = rect.Width();
		h = rect.Height() / 3;
		rect.top = rect.top + h;
		rect.bottom = rect.top + h;
		mGridOperation.MoveWindow(rect);
	}
	//if (mTrack.GetSafeHwnd()){
	//	// ���F�S�̕���1/2
	//	// �����F�S�̍�����1/3
	//	rect = client;
	//	w = rect.Width() / 2;
	//	h = rect.Height() / 3;
	//	rect.right = rect.left + w;
	//	rect.top = rect.top + (h * 2);
	//	rect.bottom = rect.top + h;
	//	mTrack.MoveWindow(rect);
	//}
	if (mpTrackView->GetSafeHwnd()){
		// ���F�S�̕���1/2
		// �����F�S�̍�����1/3
		rect = client;
		w = rect.Width() / 2;
		h = rect.Height() / 3;
		rect.right = rect.left + w;
		rect.top = rect.top + (h * 2);
		rect.bottom = rect.top + h;
		mpTrackView->MoveWindow(rect);
	}
	if (mSummary[0].GetSafeHwnd()){
		// ���F�S�̕���1/2
		// �����F�S�̍�����1/3
		rect = client;
		CRect srect = rect;
		srect.left = srect.left + w;
		srect.right = srect.left + w;
		srect.top = srect.top + (h * 2);
		srect.bottom = srect.top + h;
		int sw = w / 2;
		int sh = h / 2;
		for (int i = 0; i < mSummaryGraphMax / 2; i++){
			rect = srect;
			if (i == 0){
				rect.right = rect.left + sw;
			}
			else{
				rect.left = rect.left + sw;
			}
			for (int j = 0; j < mSummaryGraphMax / 2; j++){
				if (j == 0){
					rect.bottom = rect.top + sh;
				}
				else{
					rect.top = rect.bottom;
					rect.bottom = rect.top + sh;
				}
				mSummary[i*(mSummaryGraphMax / 2) + j].MoveWindow(rect);
			}
		}

	}
}


LRESULT CEquipmentOperationView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_ThreadNormal:
	{
		// �Ď��f�[�^���X�V���ꂽ���߁A�r���[�ɍX�V��ʒm
		for (int i = 0; i < mSummaryGraphMax; i++){
			mSummary[i].UpdateGraph();
		}
		break;
	}
	default:
		return CView::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}
