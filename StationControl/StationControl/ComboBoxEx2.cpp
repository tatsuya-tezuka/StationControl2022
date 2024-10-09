/*============================================================================*/
/*! ComboBoxEx2.cpp

-# コンボボックス拡張クラス
-#
-# 指定した文字列をコンボボックスから検索し、完全一致した項目の選択機能を実装する。
-# CComboBox::SelectStringはコンボボックスの項目の先頭からの部分一致のため、
-# 項目の完全一致をするためには、本クラスで実装しているSelectStringExを使用すること。
*/
/*============================================================================*/
// ComboBoxEx2.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ComboBoxEx2.h"


// CComboBoxEx2

IMPLEMENT_DYNAMIC(CComboBoxEx2, CComboBox)

CComboBoxEx2::CComboBoxEx2()
{

}

CComboBoxEx2::~CComboBoxEx2()
{
}


BEGIN_MESSAGE_MAP(CComboBoxEx2, CComboBox)
END_MESSAGE_MAP()



// CComboBoxEx2 メッセージ ハンドラー


/*============================================================================*/
/*! ComboBoxEx2

-# 指定した文字列をコンボボックスから検索し、完全一致した項目を選択する。

@param  nStartAfter	検索開始位置
@param	lpszString	検索文字列
@retval 一致項目の0から始まるインデックス
*/
/*============================================================================*/
int CComboBoxEx2::SelectStringEx(int nStartAfter, LPCTSTR lpszString)
{
	int idx = FindStringExact(nStartAfter, lpszString);
	SetCurSel(idx);

	return idx;
}
