// TabCtrlEx.cpp : �����t�@�C��
//

#include "stdafx.h"
//#include "RemoteGraph.h"
#include "TabCtrlEx.h"


// CTabCtrlEx

IMPLEMENT_DYNAMIC(CTabCtrlEx, CTabCtrl)

CTabCtrlEx::CTabCtrlEx()
{

}

CTabCtrlEx::~CTabCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CTabCtrlEx, CTabCtrl)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()



// CTabCtrlEx ���b�Z�[�W �n���h���[

LRESULT CTabCtrlEx::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	mpMessageWnd->PostMessage(AFX_WM_PROPERTY_CHANGED, wParam, lParam);
	return 0;
}
