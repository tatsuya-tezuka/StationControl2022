/*============================================================================*/
/*! ColorStringComboBox.cpp

-# 文字色を変更するコンボボックス
*/
/*============================================================================*/
// ColorStringComboBox.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ColorStringComboBox.h"


// CColorStringComboBox

IMPLEMENT_DYNAMIC(CColorStringComboBox, CComboBox)

/*============================================================================*/
/*! ColorStringComboBox

-# コンストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CColorStringComboBox::CColorStringComboBox()
: m_colText(RGB(0, 0, 0))
//: m_colSelectedText(RGB(0, 0, 0))
//, m_colOtherText(RGB(0, 0, 0))
{

}

/*============================================================================*/
/*! ColorStringComboBox

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CColorStringComboBox::~CColorStringComboBox()
{
}


BEGIN_MESSAGE_MAP(CColorStringComboBox, CComboBox)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()



// CColorStringComboBox メッセージ ハンドラー


/*============================================================================*/
/*! ColorStringComboBox

-# オーナー描画時に呼ばれる

@param  nIDCtl
@param  lpDrawItemStruct
@retval なし
*/
/*============================================================================*/
void CColorStringComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC	cDC;
	CString	sText;

	if (!cDC.Attach(lpDrawItemStruct->hDC))
		return;

	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		cDC.SetTextColor(m_colText);
		cDC.SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
		cDC.FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_HIGHLIGHT));
	}
	else
	{
		cDC.SetTextColor(m_colText);
		cDC.SetBkColor(GetSysColor(COLOR_3DFACE));
		cDC.FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_WINDOW));
	}

	if (lpDrawItemStruct->itemID != -1)
	{
		this->GetLBText(lpDrawItemStruct->itemID, sText);

		cDC.TextOut(3, lpDrawItemStruct->rcItem.top += 2, sText);
	}

	cDC.Detach();
}


BOOL CColorStringComboBox::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	return CComboBox::OnChildNotify(message, wParam, lParam, pLResult);
}
