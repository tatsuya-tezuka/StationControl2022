/*============================================================================*/
/*! ListBoxEx.cpp

-# CListBoxの拡張クラス（スクロールバークリックを親ウィンドウに通知）
*/
/*============================================================================*/
// ListBoxEx.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ListBoxEx.h"


// CListBoxEx

IMPLEMENT_DYNAMIC(CListBoxEx, CListBox)

/*============================================================================*/
/*! CListBoxEx

-# コンストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CListBoxEx::CListBoxEx()
{

}

/*============================================================================*/
/*! CListBoxEx

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CListBoxEx::~CListBoxEx()
{
}


BEGIN_MESSAGE_MAP(CListBoxEx, CListBox)
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()



// CListBoxEx メッセージ ハンドラー


/*============================================================================*/
/*! CListBoxEx

-# スクロールバークリック時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CListBoxEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);

	CWnd* pWnd = GetParent();
	if (pWnd != NULL && pWnd->GetSafeHwnd())
	{
		pWnd->PostMessage(eCListBoxMsg_VScroll, (WPARAM)this, 0);
	}
}
