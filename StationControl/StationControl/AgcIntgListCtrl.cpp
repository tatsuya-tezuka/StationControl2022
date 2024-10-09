/*============================================================================*/
/*! AgcIntgListCtrl.cpp

-# �v�惌�f�B�l�X�`�F�b�N�AAGC�Z���X�e�b�v��/�ϕ����Ԑݒ��ʂ̃��X�g�p
*/
/*============================================================================*/
// AgcIntgListCtrl.cpp : �����t�@�C��
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
/*! ���X�g�R���g���[�����G�f�B�b�g�{�b�N�X

-# �����_�ȉ��̌�����ݒ肷��

@param	len		����
@retval

*/
/*============================================================================*/
void CListEdit::SetDecimal(UINT len)
{
	m_nDecimal = len;
}
/*============================================================================*/
/*! ���X�g�R���g���[�����G�f�B�b�g�{�b�N�X

-# ���l�̍ő包����ݒ肷��

@param	len		����
@retval

*/
/*============================================================================*/
void CListEdit::SetNumberLimit(UINT len)
{
	m_nNumberLimit = len;
}

/*============================================================================*/
/*! ���X�g�R���g���[�����G�f�B�b�g�{�b�N�X

-# ���l�̃`�F�b�N���s��

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

		// �����_�H
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
/*! ���X�g�R���g���[�����G�f�B�b�g�{�b�N�X

-# ���͕����̃`�F�b�N���s��

@param	nChar	���͕���
@retval

*/
/*============================================================================*/
BOOL CListEdit::CheckInput(UINT nChar)
{
	// ���͒��͎��R�x���������邽�ߍׂ��ȃ`�F�b�N�͍s��Ȃ��i���͉\�����̃`�F�b�N�̂݁j
	BOOL result = TRUE;
	switch (m_nEditType)
	{
	case	eListEditType_Float:
		// ���͉\�����F�����A�����_�A����
	{
		if ((nChar < '0' || '9' < nChar) && (nChar != '+' && nChar != '-' && nChar != '.')){
			result = FALSE;
		}
	}
	break;
	case	eListEditType_Exponent:
		// ���͉\�����F�����A�����_�A�w�������A����
	{
		if ((nChar < '0' || '9' < nChar) && (nChar != '+' && nChar != '-') && nChar != '.' && (nChar != 'E' && nChar != 'e')){
			result = FALSE;
		}
	}
	break;
	case	eListEditType_Int:
		// ���͉\�����F�����A����
	{
		if ((nChar < '0' || '9' < nChar) && (nChar != '+' && nChar != '-')){
			result = FALSE;
		}
	}
	break;
	case	eListEditType_UInt:
		// ���͉\�����F�����A����
	{
		if (nChar < '0' || '9' < nChar){
			result = FALSE;
		}
	}
	break;
	case	eListEditType_Char:
		// ���͉\�����F�L�[�{�[�h�ɕ\������Ă��镶���ƃX�y�[�X
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
/*! ���X�g�R���g���[�����G�f�B�b�g�{�b�N�X

-# ���͕����͈̔̓`�F�b�N���s��

@param	nChar	���͕���
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
		// �����̐�(1)�A�����_�̐�(1)�A�����̈ʒu(�擪)
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
		// �����̐�(2)�A�����_�̐�(1)�A�w�������̐�(1)�A�����̈ʒu(�擪���w�������̒���)
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
		// �����̐�(1)�A�����̈ʒu(�擪)
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
		// �����̐�(1)�A�����̈ʒu(�擪)
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
		// �����̐�(1)�A�����_�̐�(1)�A�����̈ʒu(�擪)
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

	// �G���[���b�Z��������ꍇ
	if (bErrMsg)
	{
		GetParent()->PostMessageW(eMessage_AgcStepOutOFRange, 0, 0);
	}

	if (result == FALSE)
	{
		// ���̓G���[�Ȃ�΁A���̒l�ɖ߂�
		SetWindowText(m_strCurVal);
	}
	else
	{
		// ���̓G���[�łȂ��Ȃ�΁A�l�̍X�V
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

	// ENTER�L�[�ȊO�Ńt�H�[�J�X���Ȃ��Ȃ����Ƃ��̂��߂ɁA�����ł��͈̓`�F�b�N����
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

-# �t���[�����[�N�����̓t�H�[�J�X���������O�̂��̃����o�[�֐����Ăяo����܂��B 

@param  pNewWnd		���̓t�H�[�J�X���󂯎��E�B���h�E�ւ̃|�C���^�[
@retval �Ȃ�
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

-# �R���X�g���N�^

@param  pmain		�F�e(���X�g)�̃E�B���h�E�N���X
@param  iItem		�F���X�g�̗�ԍ�
@param  iSubItem	�F���X�g�̍s�ԍ�
@param  id			�F�{�^��ID
@retval �Ȃ�
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

-# �f�X�g���N�^

@param
@retval

*/
/*============================================================================*/
CButtonInListCtrl::~CButtonInListCtrl()
{

}

/*============================================================================*/
/*! CEditInListCtrl

-# �{�^���N���b�N���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
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

-# �R���X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CAgcIntgListCtrl::CAgcIntgListCtrl()
{
	m_ListEdit = NULL;
}

/*============================================================================*/
/*! CAgcIntgListCtrl

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
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



// CAgcIntgListCtrl ���b�Z�[�W �n���h���[




/*============================================================================*/
/*! CAgcIntgListCtrl

-# �R���g���[���ʒm�̃��b�Z�[�W �}�b�v���������܂��B
   �w�b�_�[���Œ�ɂ���B

@param  wParam	�F���b�Z�[�W���R���g���[�����炻�̃��b�Z�[�W�𑗐M����R���g���[�������ʂ��܂��B
@param  lParam	�F�ʒm�R�[�h�ƒǉ������܂ޒʒm���b�Z�[�W (NMHDR) �̍\���̂ւ̃|�C���^�[�B 
@param  pResult	�F���b�Z�[�W���������ꂽ�Ƃ����ʂ��i�[����R�[�h���� LRESULT �̕ϐ��ւ̃|�C���^�[�B
@retval ���b�Z�[�W�����������ꍇ��1��Ԃ��܂�; ����ȊO�̏ꍇ�� 0�B
*/
/*============================================================================*/
BOOL CAgcIntgListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	HD_NOTIFY *phdn = (HD_NOTIFY *)lParam;
	if (phdn->hdr.code == HDN_BEGINTRACK) {
		// �J�������Œ�
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

-# �J�X�^���h���[

@param  pNMHDR	�FNMHDR�\���̃|�C���^�[�B
@param  pResult	�F���b�Z�[�W���������ꂽ�Ƃ����ʂ��i�[����R�[�h���� LRESULT �̕ϐ��ւ̃|�C���^�[�B
@retval �Ȃ�
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
			if (nSubItem == 0)	// �v�����x��
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetBkColor(RGB(255, 255, 255));
				pDC->SetTextColor(RGB(0, 0, 0));
				pDC->DrawText(GetItemText(nItem, nSubItem), &rcItem, nFormat);
				*pResult = CDRF_SKIPDEFAULT;
			}
			else if (nSubItem == 1)	// �f�t�H���g
			{
				CImageList* pImageList = GetImageList(LVSIL_STATE);
				// �f�t�H���g
				int ofs = rcItem.Width() / 2 - 6;
				pImageList->Draw(pDC, (nLparam & 0x01) ? 2 : 1, CPoint(rcItem.left + ofs, rcItem.top), ILD_TRANSPARENT);
				*pResult = CDRF_SKIPDEFAULT;
			}
			else if (nSubItem == 2)	// �J�X�^��
			{
				CImageList* pImageList = GetImageList(LVSIL_STATE);
				// �f�t�H���g
				int ofs = rcItem.Width() / 2 - 6;
				pImageList->Draw(pDC, (nLparam & 0x02) ? 4 : 3, CPoint(rcItem.left + ofs, rcItem.top), ILD_TRANSPARENT);
				*pResult = CDRF_SKIPDEFAULT;
			}
			else if (nSubItem == 3)		// �ϕ�����
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
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

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
		case 2:	// �J�X�^��
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

-# ���{�^���A�_�u���N���b�N
   �_�u���N���b�N���ꂽ�Z�����G�f�B�b�g�\�����܂��B

@param  nFlags	�F�L�[��������Ă��邩�ǂ����������܂��B
@param  point	�F�J�[�\���� x ���W�� y ���W���w�肵�܂��B
@retval �Ȃ�
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
/// @brief	�}�E�X�ʒu��񂩂�A�C�e���ԍ����擾����
/// @param
/// @return	���������ꍇ��true��Ԃ�
//============================================================================
int CAgcIntgListCtrl::HitTestEx(CPoint &point, int *col)
{
	int colnum = 0;
	int row = HitTest(point, NULL);
	if (col) *col = 0;

	if ((GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
		return row;

	// ���ݕ\������Ă���ŏ��̃A�C�e���C���f�b�N�X���擾����
	row = GetTopIndex();

	// ���̃A�C�e�������擾����
	int bottom = row + GetCountPerPage();

	if (bottom > GetItemCount())
		bottom = GetItemCount();

	// Get the number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();

	for (; row <= bottom; row++){
		// �A�C�e���̈�̎擾
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

-# �Z���̋�`���擾���܂��B

@param  iRow	�F�s�ԍ�
@param  iCol	�F��ԍ�
@param  nArea	�F�擾���� (���X�g �r���[) �T�u���ڂ̊O�ڂ���l�p�`���������肵�܂��B
@param  rect	�F��`
@retval ����I��(TRUE)�A�ُ�I��(FALSE)
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

-# �w�肳�ꂽ�Z���Ƀ`�F�b�N�{�b�N�X���쐬���܂��B

@param  col	�F�s�ԍ�
@param  max	�F�ő��ԍ�
@retval �Ȃ�
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
/*! �ݒ胊�X�g

-# �c���[�R���g���[���Ŏg�p����C���[�W���X�g�̍쐬

@param		pdc			�f�o�C�X�R���e�L�X�g

@retval	HIMAGELIST	�C���[�W���X�g
*/
/*============================================================================*/
HIMAGELIST CAgcIntgListCtrl::CreateImageList(CDC* pdc)
{
	typedef struct {
		UINT	nType;
		UINT	nStateId;
		UINT	nState;
	} stImageInfo; 
	// �c���[�R���g���[���ł̓`�F�b�N�{�b�N�X�����g�p���Ȃ��̂Ń��W�I�{�^���̍쐬�͍s��Ȃ�
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

	// �C���[�W���X�g�ւ̒ǉ�
	ImageList_Add(hImageList, (HBITMAP)bmp.m_hObject, 0);

	bmp.DeleteObject();
	memDC.DeleteDC();
	return hImageList;
}


#define IDC_EDIT_SUB_ITEM (WM_USER + 1234)
/*============================================================================*/
/*! CAgcIntgListCtrl

-# �w�肳�ꂽ�Z���ɃG�f�B�b�g�{�b�N�X���쐬���܂��B

@param  iItem	�F�s�ԍ�
@param  iSubItem�F��ԍ�
@retval �Ȃ�
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

-# �w�肳�ꂽ�Z���ɃG�f�B�b�g�{�b�N�X���쐬���܂��B

@param  message	�F���b�Z�[�W�̎��ʎq
@param  wParam	�F���b�Z�[�W�̍ŏ��̃p�����[�^
@param  lParam	�F���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
@retval ��������
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
				// �J�X�^�}�C�Y���ύX���ꂽ�B
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

-# �w�肳�ꂽ�X�e�b�v���Ƀ`�F�b�N�{�b�N�X�Ƀ`�F�b�N��t����B

@param  col		�F�s�ԍ�
@param  step	�F�X�e�b�v
@retval �Ȃ�
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
