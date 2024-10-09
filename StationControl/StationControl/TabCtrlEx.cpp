// TabCtrlEx.cpp : 実装ファイル
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



// CTabCtrlEx メッセージ ハンドラー

LRESULT CTabCtrlEx::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	mpMessageWnd->PostMessage(AFX_WM_PROPERTY_CHANGED, wParam, lParam);
	return 0;
}
