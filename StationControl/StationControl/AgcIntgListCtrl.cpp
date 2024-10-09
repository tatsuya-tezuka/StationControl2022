/*============================================================================*/
/*! AgcIntgListCtrl.cpp

-# 計画レディネスチェック、AGC校正ステップ数/積分時間設定画面のリスト用
*/
/*============================================================================*/
// AgcIntgListCtrl.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "AgcIntgListCtrl.h"

#define ID_LIST_CBXBTN	(WM_USER+2000)
#define eMessage_CmbChange (WM_USER+3000)
#define eMessage_EditEnd (WM_USER+3500)




//=============================================================================
// CListEdit
//=============================================================================
CListEdit::CListEdit(int iItem, int iSubItem, CString sInitText)
	:m_sInitText(sInitText)
{
	m_iItem = iItem;
	m_iSubItem = iSubItem;
	m_bESC = FALSE;
	m_bKeyReturn = FALSE;
	m_bKeyShift = FALSE;
	m_nDecimal = 0;
	m_nNumberLimit = 10;
	m_bNotify = FALSE;
	m_nEditType = eListEditType_Float;
	m_fMin = 0.0f;
	m_fMax = 0.0f;
	m_bInputError = FALSE;
}

CListEdit::~CListEdit()
{
}

BEGIN_MESSAGE_MAP(CListEdit, CEdit)
	//{{AFX_MSG_MAP(CListEdit)
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListEdit message handlers

BOOL CListEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN){
		SHORT sKey = GetKeyState(VK_CONTROL);
		if (pMsg->wParam == VK_RETURN
			|| pMsg->wParam == VK_DELETE
			|| pMsg->wParam == VK_ESCAPE
			|| sKey
			)
		{
			if (pMsg->wParam == VK_RETURN)
				m_bKeyReturn = TRUE;
			if (GetKeyState(VK_SHIFT) & 0xff00)
				m_bKeyShift = TRUE;
			else
				m_bKeyShift = FALSE;

			::TranslateMessage(pMsg);
			if (!(GetStyle() & ES_MULTILINE) || pMsg->wParam != VK_ESCAPE){
				::DispatchMessage(pMsg);
			}
			return TRUE;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

/*============================================================================*/
/*! リストコントロール内エディットボックス

-# 小数点以下の桁数を設定する

@param	len		桁数
@retval

*/
/*============================================================================*/
void CListEdit::SetDecimal(UINT len)
{
	m_nDecimal = len;
}
/*============================================================================*/
/*! リストコントロール内エディットボックス

-# 数値の最大桁数を設定する

@param	len		桁数
@retval

*/
/*============================================================================*/
void CListEdit::SetNumberLimit(UINT len)
{
	m_nNumberLimit = len;
}

/*============================================================================*/
/*! リストコントロール内エディットボックス

-# 数値のチェックを行う

@param
@retval

*/
/*============================================================================*/
BOOL CListEdit::CheckDecimal()
{
	int dpos = -1, dlen = 0;
	int	startPos, endPos;
	CString	str;
	GetWindowText(str);
	for (int i = 0; i<str.GetLength(); i++){
		if (dpos >= 0)
			dlen++;

		// 小数点？
		if (str.GetAt(i) == 0x2e/*.*/)
			dpos = i;
	}
	if (dpos < 0)
		return TRUE;

	GetSel(startPos, endPos);
	if (startPos == endPos && startPos > dpos && m_nDecimal <= (UINT)dlen)
		return FALSE;

	return TRUE;
}
/*============================================================================*/
/*! リストコントロール内エディットボックス

-# 入力文字のチェックを行う

@param	nChar	入力文字
@retval

*/
/*============================================================================*/
BOOL CListEdit::CheckInput(UINT nChar)
{
	// 入力中は自由度も持たせるため細かなチェックは行わない（入力可能文字のチェックのみ）
	BOOL result = TRUE;
	switch (m_nEditType)
	{
	case	eListEditType_Float:
		// 入力可能文字：符号、小数点、数字
	{
		if ((nChar < '0' || '9' < nChar) && (nChar != '+' && nChar != '-' && nChar != '.')){
			result = FALSE;
		}
	}
	break;
	case	eListEditType_Exponent:
		// 入力可能文字：符号、小数点、指数文字、数字
	{
		if ((nChar < '0' || '9' < nChar) && (nChar != '+' && nChar != '-') && nChar != '.' && (nChar != 'E' && nChar != 'e')){
			result = FALSE;
		}
	}
	break;
	case	eListEditType_Int:
		// 入力可能文字：符号、数字
	{
		if ((nChar < '0' || '9' < nChar) && (nChar != '+' && nChar != '-')){
			result = FALSE;
		}
	}
	break;
	case	eListEditType_UInt:
		// 入力可能文字：符号、数字
	{
		if (nChar < '0' || '9' < nChar){
			result = FALSE;
		}
	}
	break;
	case	eListEditType_Char:
		// 入力可能文字：キーボードに表示されている文字とスペース
	{
		if ((nChar < '0' || '9' < nChar) && (nChar < 'A' || 'Z' < nChar) && (nChar < 'a' || 'z' < nChar) && nChar != ' '
			&& (nChar < '!' || '/' < nChar) && (nChar < ':' || '@' < nChar) && (nChar < '[' || '`' < nChar) && (nChar < '{' || '~' < nChar)){
			result = FALSE;
		}
	}
	break;
	default:
		break;
	}
	return result;
}
/*============================================================================*/
/*! リストコントロール内エディットボックス

-# 入力文字の範囲チェックを行う

@param	nChar	入力文字
@retval

*/
/*============================================================================*/
BOOL CListEdit::CheckRange(UINT nChar)
{
	BOOL result = TRUE;
	BOOL bErrMsg = FALSE;

	CString	str;
	GetWindowText(str);

	switch (m_nEditType)
	{
	case	eListEditType_Float:
		// 符号の数(1)、小数点の数(1)、符号の位置(先頭)
	{
		int length = str.GetLength();
		CString temp;
		temp = str;
		temp.Replace(_T("+"), _T(""));
		temp.Replace(_T("-"), _T(""));
		int cntSign = length - temp.GetLength();
		if (1 < cntSign){
			result = FALSE;
			break;
		}

		temp = str;
		temp.Replace(_T("."), _T(""));
		int cntPoint = length - temp.GetLength();
		if (1 < cntPoint){
			result = FALSE;
			break;
		}

		int posSign = str.FindOneOf(_T("+-"));
		if (cntSign == 1 && posSign != 0){
			result = FALSE;
			break;
		}
	}
	break;
	case	eListEditType_Exponent:
		// 符号の数(2)、小数点の数(1)、指数文字の数(1)、符号の位置(先頭か指数文字の直後)
	{
		int startPos, endPos;
		GetSel(startPos, endPos);

		int length = str.GetLength();
		CString temp;
		temp = str;
		temp.Replace(_T("+"), _T(""));
		temp.Replace(_T("-"), _T(""));
		int cntSign = length - temp.GetLength();
		if (2 < cntSign){
			result = FALSE;
			break;
		}

		temp = str;
		temp.Replace(_T("."), _T(""));
		int cntPoint = length - temp.GetLength();
		if (1 < cntPoint){
			result = FALSE;
			break;
		}

		temp = str;
		temp.Replace(_T("E"), _T(""));
		temp.Replace(_T("e"), _T(""));
		int cntExp = length - temp.GetLength();
		if (1 < cntExp){
			result = FALSE;
			break;
		}

		int posExp = str.FindOneOf(_T("Ee"));
		int posSignP1 = str.Find(_T("+"), 0);
		int posSignP2 = str.Find(_T("+"), posSignP1 + 1);
		int posSignM1 = str.Find(_T("-"), 0);
		int posSignM2 = str.Find(_T("-"), posSignM1 + 1);
		if (cntSign == 1){
			if (posSignP1 != 0 && posSignM1 != 0 && posSignP1 != (posExp + 1) && posSignM1 != (posExp + 1)){
				result = FALSE;
				break;
			}
		}
		if (cntSign == 2){
			if (!((posSignP1 == 0 && (posSignM1 == (posExp + 1) || posSignP2 == (posExp + 1)))
				|| (posSignM1 == 0 && (posSignP1 == (posExp + 1) || posSignM2 == (posExp + 1))))){
				result = FALSE;
				break;
			}
		}
		int posPoint = str.Find(_T("."), 0);
		if (posExp != -1 && posPoint != -1 && posExp <= posPoint){
			result = FALSE;
			break;
		}
	}
	break;
	case	eListEditType_Int:
		// 符号の数(1)、符号の位置(先頭)
	{
		int length = str.GetLength();
		CString temp;
		temp = str;
		temp.Replace(_T("+"), _T(""));
		temp.Replace(_T("-"), _T(""));
		int cntSign = length - temp.GetLength();
		if (1 < cntSign){
			result = FALSE;
			break;
		}

		int posSign = str.FindOneOf(_T("+-"));
		if (cntSign == 1 && posSign != 0){
			result = FALSE;
			break;
		}
	}
	break;
	case	eListEditType_UInt:
		// 符号の数(1)、符号の位置(先頭)
	{
		long val = _wtoi(str);

		int length = str.GetLength();
		CString temp;
		temp = str;
		temp.Replace(_T("+"), _T(""));
		temp.Replace(_T("-"), _T(""));
		int cntSign = length - temp.GetLength();
		if (1 < cntSign){
			result = FALSE;
			break;
		}

		int posSign = str.FindOneOf(_T("+-"));
		if (cntSign == 1 && posSign != 0){
			result = FALSE;
			break;
		}
		if (m_fMin > val || m_fMax < val){
			result = FALSE;
//			return FALSE;
			bErrMsg = TRUE;
		}
	}
	break;
	case	eListEditType_VectorMag:
		// 符号の数(1)、小数点の数(1)、符号の位置(先頭)
	{
		double val = _wtof(str);

		int length = str.GetLength();
		CString temp;
		temp = str;
		temp.Replace(_T("+"), _T(""));
		temp.Replace(_T("-"), _T(""));
		int cntSign = length - temp.GetLength();
		if (1 < cntSign){
			result = FALSE;
			break;
		}

		temp = str;
		temp.Replace(_T("."), _T(""));
		int cntPoint = length - temp.GetLength();
		if (1 < cntPoint){
			result = FALSE;
			break;
		}

		int posSign = str.FindOneOf(_T("+-"));
		if (cntSign == 1 && posSign != 0){
			result = FALSE;
			break;
		}
		if (m_fMin > val || m_fMax < val){
			result = FALSE;
//			return FALSE;
			bErrMsg = TRUE;
		}
	}
	break;
	default:
		break;
	}

	// エラーメッセ時がある場合
	if (bErrMsg)
	{
		GetParent()->PostMessageW(eMessage_AgcStepOutOFRange, 0, 0);
	}

	if (result == FALSE)
	{
		// 入力エラーならば、元の値に戻す
		SetWindowText(m_strCurVal);
	}
	else
	{
		// 入力エラーでないならば、値の更新
		m_strCurVal = str;
	}

	return result;
}

void CListEdit::OnKillFocus(CWnd* pNewWnd)
{
	if (m_bInputError == TRUE && m_bESC == FALSE){
		SetFocus();
		return;
	}

	CEdit::OnKillFocus(pNewWnd);

	// ENTERキー以外でフォーカスがなくなったときのために、ここでも範囲チェックする
	if (CheckRange() == FALSE)
	{
		MessageBeep((UINT)-1);
	}

	CString str;
	GetWindowText(str);

	if (m_bNotify == TRUE){
		return;
	}
	m_bNotify = TRUE;

//#if 1
	if (m_iItem >= 0 && m_iSubItem >= 0)
	{
		int val = _wtoi(str);
		str.Format(_T("%d"), val);
		((CListCtrl*)GetParent())->SetItemText(m_iItem, m_iSubItem, str);
	}

	GetParent()->PostMessageW(eMessage_EditEnd, 0, 0);
	DestroyWindow();


//#else
//	LV_DISPINFO dispinfo;
//	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
//	dispinfo.hdr.idFrom = GetDlgCtrlID();
//	dispinfo.hdr.code = LVN_ENDLABELEDIT;
//
//	dispinfo.item.mask = LVIF_TEXT;
//	dispinfo.item.iItem = m_iItem;
//	dispinfo.item.iSubItem = m_iSubItem;
//	dispinfo.item.pszText = m_bESC ? NULL : LPTSTR((LPCTSTR)str);
//	dispinfo.item.cchTextMax = m_bESC ? 0 : str.GetLength();
//
//	GetParent()->SetFocus();
//	GetParent()->GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo);
//#endif
}

void CListEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 0x03/*Ctrl+C*/ || nChar == 0x16/*Ctrl+V*/ || nChar == 0x18/*Ctrl+X*/ || nChar == 0x08/*BackSpace*/)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else if (nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		if (nChar == VK_RETURN)
		{
			if (CheckRange() == FALSE)
			{
				MessageBeep((UINT)-1);
				return;
			}
		}
		if (nChar == VK_ESCAPE)
		{
			m_bESC = TRUE;
		}
		GetParent()->SetFocus();
		return;
	}
	else{
//		if (m_nEditType == eListEditType_Float || m_nEditType == eListEditType_Exponent || m_nEditType == eListEditType_Int
//			|| m_nEditType == eListEditType_VectorMag || m_nEditType == eListEditType_Char)
		if (m_nEditType == eListEditType_Float || m_nEditType == eListEditType_Exponent || m_nEditType == eListEditType_Int || m_nEditType == eListEditType_UInt
			|| m_nEditType == eListEditType_VectorMag || m_nEditType == eListEditType_Char)
			{
			if (CheckInput(nChar) == TRUE)
			{
				CEdit::OnChar(nChar, nRepCnt, nFlags);
			}
			else
			{
				MessageBeep((UINT)-1);
			}
		}
		else
		{
			MessageBeep((UINT)-1);
		}
	}
}

int CListEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	CFont* font = GetParent()->GetFont();
	SetFont(font);

	SetWindowText(m_sInitText);
	SetFocus();
	SetSel(0, -1);

//	CString	str;
	GetWindowText(m_strCurVal);

	return 0;
}



//----------------------------------------------------------------------
// CEditInListCtrl

BEGIN_MESSAGE_MAP(CEditInListCtrl, CEdit)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


/*============================================================================*/
/*! CEditInListCtrl

-# フレームワークが入力フォーカスを失う直前のこのメンバー関数が呼び出されます。 

@param  pNewWnd		入力フォーカスを受け取るウィンドウへのポインター
@retval なし
*/
/*============================================================================*/
void CEditInListCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	if (m_row >= 0 && m_col >= 0)
	{
		CString strText;
		GetWindowText(strText);
		((CListCtrl*)GetParent())->SetItemText(m_row, m_col, strText);
	}
	DestroyWindow();
}


//----------------------------------------------------------------------
// CButtonInListCtrl

BEGIN_MESSAGE_MAP(CButtonInListCtrl, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CButtonInListCtrl::OnBnClicked)
END_MESSAGE_MAP()

/*============================================================================*/
/*! CButtonInListCtrl

-# コンストラクタ

@param  pmain		：親(リスト)のウィンドウクラス
@param  iItem		：リストの列番号
@param  iSubItem	：リストの行番号
@param  id			：ボタンID
@retval なし
*/
/*============================================================================*/
CButtonInListCtrl::CButtonInListCtrl(CWnd* pmain, int iItem, int iSubItem, UINT id)
{
	m_pMain = pmain;
	m_iItem = iItem;
	m_iSubItem = iSubItem;
	m_uID = id;
}

/*============================================================================*/
/*! CButtonInListCtrl

-# デストラクタ

@param
@retval

*/
/*============================================================================*/
CButtonInListCtrl::~CButtonInListCtrl()
{

}

/*============================================================================*/
/*! CEditInListCtrl

-# ボタンクリック時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CButtonInListCtrl::OnBnClicked()
{
	int bCk = GetCheck();
	stLISTDATA st;
	st.row = m_iItem;
	st.col = m_iSubItem;
	m_pMain->PostMessageW(eMessage_CmbChange, (WPARAM)&st, (LPARAM)bCk);
}


//----------------------------------------------------------------------
// CAgcIntgListCtrl

IMPLEMENT_DYNAMIC(CAgcIntgListCtrl, CListCtrl)

/*============================================================================*/
/*! CAgcIntgListCtrl

-# コンストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CAgcIntgListCtrl::CAgcIntgListCtrl()
{
	m_ListEdit = NULL;
}

/*============================================================================*/
/*! CAgcIntgListCtrl

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CAgcIntgListCtrl::~CAgcIntgListCtrl()
{

	if (m_ImageList.GetSafeHandle()){
		m_ImageList.DeleteImageList();
	}
}


BEGIN_MESSAGE_MAP(CAgcIntgListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CAgcIntgListCtrl::OnNMCustomdraw)
	ON_WM_LBUTTONDOWN()
	//ON_WM_DRAWITEM()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()



// CAgcIntgListCtrl メッセージ ハンドラー




/*============================================================================*/
/*! CAgcIntgListCtrl

-# コントロール通知のメッセージ マップを処理します。
   ヘッダーを固定にする。

@param  wParam	：メッセージがコントロールからそのメッセージを送信するコントロールを識別します。
@param  lParam	：通知コードと追加情報を含む通知メッセージ (NMHDR) の構造体へのポインター。 
@param  pResult	：メッセージが処理されたとき結果を格納するコードする LRESULT の変数へのポインター。
@retval メッセージを処理した場合は1を返します; それ以外の場合は 0。
*/
/*============================================================================*/
BOOL CAgcIntgListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	HD_NOTIFY *phdn = (HD_NOTIFY *)lParam;
	if (phdn->hdr.code == HDN_BEGINTRACK) {
		// カラムを固定
		if (( phdn->iItem == 0 ) || ( phdn->iItem == 1 ) || ( phdn->iItem == 2 ) || ( phdn->iItem == 3 )) {
			*pResult = TRUE;
			return(1);
		}
	}
	return(0);

//	return CListCtrl::OnNotify(wParam, lParam, pResult);
}


/*============================================================================*/
/*! CAgcIntgListCtrl

-# カスタムドロー

@param  pNMHDR	：NMHDR構造体ポインター。
@param  pResult	：メッセージが処理されたとき結果を格納するコードする LRESULT の変数へのポインター。
@retval なし
*/
/*============================================================================*/
void CAgcIntgListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

	switch (lplvcd->nmcd.dwDrawStage) {
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		{
			int		nItem = (int)lplvcd->nmcd.dwItemSpec;
			int		nSubItem = lplvcd->iSubItem;
			int		nLparam = (int)lplvcd->nmcd.lItemlParam;

			CDC *pDC = CDC::FromHandle(lplvcd->nmcd.hdc);
			CRect rcItem;
			GetCellRect(nItem, nSubItem, LVIR_BOUNDS, rcItem);

			UINT nFormat = DT_CENTER | DT_SINGLELINE;
			if (nSubItem == 0)	// 計測レベル
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetBkColor(RGB(255, 255, 255));
				pDC->SetTextColor(RGB(0, 0, 0));
				pDC->DrawText(GetItemText(nItem, nSubItem), &rcItem, nFormat);
				*pResult = CDRF_SKIPDEFAULT;
			}
			else if (nSubItem == 1)	// デフォルト
			{
				CImageList* pImageList = GetImageList(LVSIL_STATE);
				// デフォルト
				int ofs = rcItem.Width() / 2 - 6;
				pImageList->Draw(pDC, (nLparam & 0x01) ? 2 : 1, CPoint(rcItem.left + ofs, rcItem.top), ILD_TRANSPARENT);
				*pResult = CDRF_SKIPDEFAULT;
			}
			else if (nSubItem == 2)	// カスタム
			{
				CImageList* pImageList = GetImageList(LVSIL_STATE);
				// デフォルト
				int ofs = rcItem.Width() / 2 - 6;
				pImageList->Draw(pDC, (nLparam & 0x02) ? 4 : 3, CPoint(rcItem.left + ofs, rcItem.top), ILD_TRANSPARENT);
				*pResult = CDRF_SKIPDEFAULT;
			}
			else if (nSubItem == 3)		// 積分時間
			{
				LPARAM param = GetItemData(nItem);
				COLORREF textColor;
				if (param & 0x02)
				{
					textColor = GetSysColor(COLOR_WINDOWTEXT);
					lplvcd->clrText = textColor;
				}
				else
				{
					textColor = GetSysColor(COLOR_GRAYTEXT);
					lplvcd->clrText = textColor;
				}
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetBkColor(RGB(255, 255, 255));
				pDC->SetTextColor(textColor);
				pDC->DrawText(GetItemText(nItem, nSubItem), &rcItem, nFormat);
				*pResult = CDRF_SKIPDEFAULT;
			}
		}
		break;
	default:
		*pResult = CDRF_DODEFAULT;
		break;
	}

//	*pResult = 0;
}

void CAgcIntgListCtrl::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CListCtrl::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CAgcIntgListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListCtrl::OnLButtonDown(nFlags, point);

	int index = 0;
	int colnum = 0;
	if ((index = HitTestEx(point, &colnum)) != -1 && colnum != 0){
		switch (colnum)
		{
		case 2:	// カスタム
		{
			LPARAM param = GetItemData(index);
			param ^= 0x02;
			SetItemData(index, param);
		}
		break;

		default:
			break;
		}
	}
}

/*============================================================================*/
/*! CAgcIntgListCtrl

-# 左ボタン、ダブルクリック
   ダブルクリックされたセルをエディット表示します。

@param  nFlags	：キーが押されているかどうかを示します。
@param  point	：カーソルの x 座標と y 座標を指定します。
@retval なし
*/
/*============================================================================*/
void CAgcIntgListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CListCtrl::OnLButtonDblClk(nFlags, point);

	int index = 0;
	int colnum = 0;
	if ((index = HitTestEx(point, &colnum)) != -1 && colnum != 0){
		switch (colnum)
		{
		case 3:
			BeginEdit(index, colnum);
			break;

		default:
			break;
		}
	}
}

//============================================================================
/// @brief	マウス位置情報からアイテム番号を取得する
/// @param
/// @return	成功した場合はtrueを返す
//============================================================================
int CAgcIntgListCtrl::HitTestEx(CPoint &point, int *col)
{
	int colnum = 0;
	int row = HitTest(point, NULL);
	if (col) *col = 0;

	if ((GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
		return row;

	// 現在表示されている最初のアイテムインデックスを取得する
	row = GetTopIndex();

	// 可視のアイテム数を取得する
	int bottom = row + GetCountPerPage();

	if (bottom > GetItemCount())
		bottom = GetItemCount();

	// Get the number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();

	for (; row <= bottom; row++){
		// アイテム領域の取得
		CRect rect;
		GetItemRect(row, &rect, LVIR_BOUNDS);

		if (rect.PtInRect(point)){
			// Now find the column
			for (colnum = 0; colnum < nColumnCount; colnum++){
				int colwidth = GetColumnWidth(colnum);
				if (point.x >= rect.left && point.x <= (rect.left + colwidth)){
					if (col) *col = colnum;
					return row;
				}
				rect.left += colwidth;
			}
		}
	}
	return -1;
}

/*============================================================================*/
/*! CAgcIntgListCtrl

-# セルの矩形を取得します。

@param  iRow	：行番号
@param  iCol	：列番号
@param  nArea	：取得する (リスト ビュー) サブ項目の外接する四角形部分を決定します。
@param  rect	：矩形
@retval 正常終了(TRUE)、異常終了(FALSE)
*/
/*============================================================================*/
BOOL CAgcIntgListCtrl::GetCellRect(int iRow, int iCol, int nArea, CRect &rect)
{
	if (iCol) return GetSubItemRect(iRow, iCol, nArea, rect);

	if (GetHeaderCtrl()->GetItemCount() == 1)
		return GetItemRect(iRow, rect, nArea);

	CRect rCol1;
	if (!GetSubItemRect(iRow, 1, nArea, rCol1))
		return FALSE;

	if (!GetItemRect(iRow, rect, nArea))
		return FALSE;

	rect.right = rCol1.left;

	return TRUE;
}

/*============================================================================*/
/*! CAgcIntgListCtrl

-# 指定されたセルにチェックボックスを作成します。

@param  col	：行番号
@param  max	：最大列番号
@retval なし
*/
/*============================================================================*/
void CAgcIntgListCtrl::CreateCheckBox()
{
	if (m_ImageList.GetSafeHandle())
		return;
	CDC* pdc = GetDC();
	HIMAGELIST hImageList = CreateImageList(pdc);
	m_ImageList.Attach(hImageList);
	SetImageList(&m_ImageList, TVSIL_STATE);
	ReleaseDC(pdc);

	return;
}


/*============================================================================*/
/*! 設定リスト

-# ツリーコントロールで使用するイメージリストの作成

@param		pdc			デバイスコンテキスト

@retval	HIMAGELIST	イメージリスト
*/
/*============================================================================*/
HIMAGELIST CAgcIntgListCtrl::CreateImageList(CDC* pdc)
{
	typedef struct {
		UINT	nType;
		UINT	nStateId;
		UINT	nState;
	} stImageInfo; 
	// ツリーコントロールではチェックボックスしか使用しないのでラジオボタンの作成は行わない
	static	const stImageInfo	mImageInfo[] = {
		{ 0, 0, 0 },
		{ BP_CHECKBOX, CBS_UNCHECKEDDISABLED, DFCS_BUTTON3STATE | DFCS_FLAT | DFCS_INACTIVE },
		{ BP_CHECKBOX, CBS_CHECKEDDISABLED, DFCS_BUTTON3STATE | DFCS_CHECKED | DFCS_FLAT | DFCS_INACTIVE },
		{ BP_CHECKBOX, CBS_UNCHECKEDNORMAL, DFCS_BUTTONCHECK | DFCS_FLAT },
		{ BP_CHECKBOX, CBS_CHECKEDNORMAL, DFCS_BUTTONCHECK | DFCS_CHECKED | DFCS_FLAT },
		{ BP_CHECKBOX, CBS_MIXEDNORMAL, DFCS_BUTTON3STATE | DFCS_CHECKED | DFCS_FLAT },
	};

	HIMAGELIST hImageList = 0;
	const int nBmpWidth = 16;
	const int nBmpHeight = 16;
	const int nImages = sizeof(mImageInfo) / sizeof(mImageInfo[0]);
	CDC memDC;
	if (memDC.CreateCompatibleDC(pdc) == FALSE)
		return NULL;
	CBitmap bmp;
	if (bmp.CreateCompatibleBitmap(pdc, nBmpWidth * nImages, nBmpHeight) == FALSE){
		memDC.DeleteDC();
		return NULL;
	}
	hImageList = ImageList_Create(nBmpWidth, nBmpHeight, ILC_COLOR32, nImages, 1);
	if (hImageList == NULL){
		bmp.DeleteObject();
		memDC.DeleteDC();
		return NULL;
	}

	RECT rectImage1 = { 0, 0, 15, 15 };

	HTHEME hTheme = OpenThemeData(NULL, L"BUTTON");
	CBitmap* pOldBmp = memDC.SelectObject(&bmp);
	memDC.FillSolidRect(0, 0, nBmpWidth*nImages, nBmpHeight, GetSysColor(COLOR_WINDOW));

	int nImageWidth = nBmpWidth - 2;
	int nImageHeight = nBmpHeight - 2;
	int nImageLeft = (nBmpWidth - nImageWidth) / 2;
	int nImageTop = (nBmpHeight - nImageHeight) / 2;

	RECT rectImage = { nImageLeft,
		nImageTop,
		nImageLeft + nImageWidth,
		nImageTop + nImageHeight };

	for (int i = 0; i<nImages; i++){
		memDC.FillSolidRect(0, 0, 16, 16, GetSysColor(COLOR_WINDOW));
		if (mImageInfo[i].nType == 0){
		}
		else{
			if (hTheme){
				rectImage1.left = rectImage.left - 1;
				rectImage1.right = rectImage1.left + nBmpWidth - 1;
				DrawThemeBackground(hTheme, memDC.m_hDC, mImageInfo[i].nType, mImageInfo[i].nStateId, &rectImage1, NULL);
				DrawThemeEdge(hTheme, memDC.m_hDC, mImageInfo[i].nType, mImageInfo[i].nStateId, &rectImage1, 0, BF_ADJUST | BF_RECT, NULL);
			}
			else{
				memDC.DrawFrameControl(&rectImage1, DFC_BUTTON, mImageInfo[i].nState);
			}
		}
		rectImage.left += nBmpWidth;
		rectImage.right += nBmpWidth;
	}
	if (hTheme){
		CloseThemeData(hTheme);
	}
	memDC.SelectObject(pOldBmp);

	// イメージリストへの追加
	ImageList_Add(hImageList, (HBITMAP)bmp.m_hObject, 0);

	bmp.DeleteObject();
	memDC.DeleteDC();
	return hImageList;
}


#define IDC_EDIT_SUB_ITEM (WM_USER + 1234)
/*============================================================================*/
/*! CAgcIntgListCtrl

-# 指定されたセルにエディットボックスを作成します。

@param  iItem	：行番号
@param  iSubItem：列番号
@retval なし
*/
/*============================================================================*/
void CAgcIntgListCtrl::BeginEdit(int iItem, int iSubItem)
{
	CRect ColumnRect;

	LPARAM param = GetItemData(iItem);
	if ((param & 0x02) == 0)
	{
		return;
	}

	if (iSubItem == 0)
		GetSubItemRect(iItem, iSubItem, LVIR_LABEL, ColumnRect);
	else
		GetSubItemRect(iItem, iSubItem, LVIR_BOUNDS, ColumnRect);

	ColumnRect.DeflateRect(1, 1);
//#if 1
	CString str = GetItemText(iItem, iSubItem);

	m_ListEdit = new CListEdit(iItem, iSubItem, str);
	m_ListEdit->Create(WS_VISIBLE | ES_AUTOHSCROLL | ES_RIGHT, ColumnRect, this, IDC_EDIT_SUB_ITEM);
	m_ListEdit->SetEditType(eListEditType_UInt);
	m_ListEdit->SetMinMax(1.0, 300.0);
	m_ListEdit->SetNumberLimit(3);

//#else
//	m_Edit.Create(WS_VISIBLE | ES_AUTOHSCROLL | ES_RIGHT, ColumnRect, this, IDC_EDIT_SUB_ITEM);
//	CFont* pFont = GetFont();
//	m_Edit.SetFont(pFont);
//	m_Edit.m_row = iItem;
//	m_Edit.m_col = iSubItem;
//	m_Edit.SetWindowText(GetItemText(iItem, iSubItem));
//	m_Edit.SetFocus();
//#endif
}


/*============================================================================*/
/*! CAgcIntgListCtrl

-# 指定されたセルにエディットボックスを作成します。

@param  message	：メッセージの識別子
@param  wParam	：メッセージの最初のパラメータ
@param  lParam	：メッセージの 2 番目のパラメータ
@retval 処理結果
*/
/*============================================================================*/
LRESULT CAgcIntgListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case eMessage_CmbChange:
		{
			stLISTDATA st = *(stLISTDATA*)wParam;
			if (st.col == 2)
			{
				// カスタマイズが変更された。
				int bCk = (int)lParam;
				SetItemText(st.row, 3, GetItemText(st.row, 3));
			}
		}
		break;

	case eMessage_EditEnd:
		{
			if (m_ListEdit != NULL)
			{
				delete m_ListEdit;
				m_ListEdit = NULL;
			}
		}
		break;

	case eMessage_AgcStepOutOFRange:
		GetParent()->PostMessageW(eMessage_AgcStepOutOFRange, 0, 0);
		break;

	default:
		return CListCtrl::WindowProc(message, wParam, lParam);

	}

	return NULL;
//	return CListCtrl::WindowProc(message, wParam, lParam);
}

/*============================================================================*/
/*! CAgcIntgListCtrl

-# 指定されたステップ毎にチェックボックスにチェックを付ける。

@param  col		：行番号
@param  step	：ステップ
@retval なし
*/
/*============================================================================*/
void CAgcIntgListCtrl::CheckBoxCheckStep(int col, int step)
{
	int num = GetItemCount();
	for (int i = 0; i < num; ++i)
	{
		LPARAM param = GetItemData(i);

		if ((i % step) == 0)
		{
			param = (param & ~0x02) | 0x02;
		}
		else
		{
			param = param & ~0x02;
		}
		SetItemData(i, param);
	}
}


void CAgcIntgListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}
