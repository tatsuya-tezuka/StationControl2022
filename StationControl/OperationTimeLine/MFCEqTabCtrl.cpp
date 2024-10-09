// MFCEqTabCtrl.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "MFCEqTabCtrl.h"


// CMFCEqTabCtrl

IMPLEMENT_DYNAMIC(CMFCEqTabCtrl, CMFCTabCtrl)

CMFCEqTabCtrl::CMFCEqTabCtrl()
{

}

CMFCEqTabCtrl::~CMFCEqTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CMFCEqTabCtrl, CMFCTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMFCEqTabCtrl メッセージ ハンドラー

/*============================================================================*/
/*! 設備状況ビュー

-# タブコントロールの初期化

@param  なし
@retval なし
*/
/*============================================================================*/
void CMFCEqTabCtrl::Initialize()
{
	ModifyTabStyle(CMFCTabCtrl::STYLE_3D);
	//ModifyTabStyle(CMFCTabCtrl::STYLE_FLAT);
	SetLocation(CMFCTabCtrl::LOCATION_TOP);
	for (int i = 0; i < GetTabsNum(); i++){
		if (i == 0){
			GetTabWnd(i)->ShowWindow(SW_SHOW);
			//SetTabBkColor(i, RGB(121, 210, 231));
		}
		else{
			GetTabWnd(i)->ShowWindow(SW_HIDE);
			//SetTabBkColor(i, RGB(192, 192, 192));
		}
	}
	SetActiveTab(0);
	RecalcLayout();
	RedrawWindow();
}


void CMFCEqTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	int pos = GetTabFromPoint(point);
	if (pos < 0){
		CMFCTabCtrl::OnLButtonDown(nFlags, point);
		return;
	}

	for (int i = 0; i < GetTabsNum(); i++){
		if (i == pos){
			GetTabWnd(i)->ShowWindow(SW_SHOW);
			//SetTabBkColor(i, RGB(121, 210, 231));
		}
		else{
			GetTabWnd(i)->ShowWindow(SW_HIDE);
			//SetTabBkColor(i, RGB(192, 192, 192));
		}
	}

	SetActiveTab(pos);
	RecalcLayout();
	RedrawWindow();
}
