/*============================================================================*/
/*! DateTimeCtrlEx.cpp

-# CDateTimeCtrlの拡張クラス
-# フォーカスを失う時に、親ウィンドウに通知する
*/
/*============================================================================*/
// DateTimeCtrlEx.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "DateTimeCtrlEx.h"


// CDateTimeCtrlEx

IMPLEMENT_DYNAMIC(CDateTimeCtrlEx, CDateTimeCtrl)

/*============================================================================*/
/*! CDateTimeCtrlEx

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CDateTimeCtrlEx::CDateTimeCtrlEx()
: m_pPostToWnd(NULL)
{

}

/*============================================================================*/
/*! CDateTimeCtrlEx

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CDateTimeCtrlEx::~CDateTimeCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CDateTimeCtrlEx, CDateTimeCtrl)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CDateTimeCtrlEx メッセージ ハンドラー


/*============================================================================*/
/*! CDateTimeCtrlEx

-# フレームワークが入力フォーカスを失う直前に呼ばれる

@param  pNewWnd：入力フォーカスを受け取るウィンドウへのポインター
@retval なし
*/
/*============================================================================*/
void CDateTimeCtrlEx::OnKillFocus(CWnd* pNewWnd)
{
	CDateTimeCtrl::OnKillFocus(pNewWnd);

	// m_pPostToWndに通知先のウィンドウポインタがセットされている場合は、m_pPostToWndに通知する
	if (m_pPostToWnd != NULL)
	{
		// フォーカスを失う通知をする
		m_pPostToWnd->PostMessage(WM_KILLFOCUS, (WPARAM)(this->GetSafeHwnd()), NULL);
	}
	// それ以外は、親ウィンドウに通知する
	else
	{
		CWnd* pWnd = GetParent();
		if (pWnd != NULL)
		{
			// フォーカスを失う通知をする
			pWnd->PostMessage(WM_KILLFOCUS, (WPARAM)(this->GetSafeHwnd()), NULL);
		}
	}
}
