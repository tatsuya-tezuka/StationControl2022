// ListCtrlEx.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "RemoteGraph.h"
#include "ListCtrlEx.h"
#include "const.h"


//=============================================================================
// CColorComboBox
//=============================================================================

CColorComboBox::CColorComboBox()
{
	m_color = RGB(0, 0, 0);
	m_colorOther = RGB(0, 0, 0);
	m_colorlist.clear();
}

CColorComboBox::~CColorComboBox()
{
}


BEGIN_MESSAGE_MAP(CColorComboBox, CComboBox)
	//{{AFX_MSG_MAP(CColorComboBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*============================================================================*/
/*! �J���[�R���{�{�b�N�X

-# �R���{�{�b�N�X�ɕ\������J���[���X�g�̓o�^

@param	list	�J���[���X�g
@retval

*/
/*============================================================================*/
void CColorComboBox::SetColorList(vector<CString> list)
{
	vector<CString>::iterator itr;
	m_colorlist.clear();
	for (itr = list.begin(); itr != list.end(); itr++){
		COLORREF col = _wtoi((*itr));
		m_colorlist.push_back(col);
	}
}
void CColorComboBox::SetColorList(vector<COLORREF> list)
{
	vector<COLORREF>::iterator itr;
	m_colorlist.clear();
	for (itr = list.begin(); itr != list.end(); itr++){
		m_colorlist.push_back((*itr));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CColorComboBox ���b�Z�[�W �n���h��

void CColorComboBox::PreSubclassWindow()
{
	for (UINT i = 0; i<m_colorlist.size(); i++){
		AddString(_T(""));
	}

	// �ŏ��ɕ\�������F��I��
	SetCurSel(0);

	CComboBox::PreSubclassWindow();
}

void CColorComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->itemID == 0xffffffff)
		return;

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	CRect rect(&(lpDrawItemStruct->rcItem));
	rect.DeflateRect(2, 2);

	dc.FillSolidRect(rect, m_colorlist[lpDrawItemStruct->itemID]);

	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		dc.DrawFocusRect(rect);

	dc.FrameRect(rect, &CBrush(RGB(0, 0, 0)));

	dc.Detach();
}

/////////////////////////////////////////////////////////////////////////////
// CListComboEx

CListComboEx::CListComboEx(CWnd* pmain, int iItem, int iSubItem, vector<CString>& itemList, int nSel, UINT id)
{
	m_pMain = pmain;
	m_uID = id;
	m_iItem = iItem;
	m_iSubItem = iSubItem;

	m_ItemList = itemList;
	m_nSel = nSel;
	m_bESC = FALSE;
}

CListComboEx::~CListComboEx()
{
	m_pMain->Invalidate(FALSE);
}


BEGIN_MESSAGE_MAP(CListComboEx, CComboBox)
	//{{AFX_MSG_MAP(CListComboEx)
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_NCDESTROY()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListComboEx ���b�Z�[�W �n���h��

int CListComboEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Set the proper font
#if 0
	SetFont(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
#else
	CFont* font = GetParent()->GetFont();
	SetFont(font);
#endif

	vector<CString>::iterator itr;
	for (itr = m_ItemList.begin(); itr != m_ItemList.end(); itr++){
		AddString((LPCTSTR)(*itr));
	}
	SetCurSel(m_nSel);
	SetFocus();
	return 0;
}

BOOL CListComboEx::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN){
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE){
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

void CListComboEx::OnKillFocus(CWnd* pNewWnd)
{
	CComboBox::OnKillFocus(pNewWnd);

	CString str;
	GetWindowText(str);

	// Send Notification to parent of ListView ctrl
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_ENDLABELEDIT;

	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_iItem;
	dispinfo.item.iSubItem = m_iSubItem;
	dispinfo.item.pszText = m_bESC ? NULL : LPTSTR((LPCTSTR)str);
	dispinfo.item.cchTextMax = str.GetLength();
	dispinfo.item.lParam = GetCurSel();

	m_pMain->SendMessage(m_uID, 0, (LPARAM)&dispinfo);

	PostMessage(WM_CLOSE);
}

void CListComboEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE || nChar == VK_RETURN){
		if (nChar == VK_ESCAPE)
			m_bESC = TRUE;
		GetParent()->SetFocus();
		return;
	}

	CComboBox::OnChar(nChar, nRepCnt, nFlags);
}

void CListComboEx::OnNcDestroy()
{
	CComboBox::OnNcDestroy();
	delete this;
}

void CListComboEx::OnCloseup()
{
	GetParent()->SetFocus();
}



// CListCtrlEx
#define ID_LIST_COMBOBOX		(WM_USER+1100)


IMPLEMENT_DYNAMIC(CListCtrlEx, CListCtrl)

CListCtrlEx::CListCtrlEx()
{
	m_pParent = NULL;
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
	CMFCButton::EnableWindowsTheming();
}

CListCtrlEx::~CListCtrlEx()
{
	CMFCVisualManager::DestroyInstance();
}

BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListCtrlEx::OnNMCustomdraw)
END_MESSAGE_MAP()



// CListCtrlEx ���b�Z�[�W �n���h���[




void CListCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListCtrl::OnLButtonDown(nFlags, point);

	int index = 0;
	int colnum = 0;
	if ((index = HitTestEx(point, &colnum)) != -1 && colnum != 0){
		switch (colnum)
		{
			case 1:
			{
				vector<CString> list;
				list.push_back(_T("Line"));
				list.push_back(_T("Dots"));
				list.push_back(_T("L&D"));
				ShowInPlaceList(index, colnum, list, 0);
			}
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
int CListCtrlEx::HitTestEx(CPoint &point, int *col)
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
//============================================================================
/// @brief	���X�g�R���g���[����̃T�u�A�C�e���ɃR���{�{�b�N�X��\������
/// @param1	nItem		�A�C�e���ԍ�
/// @param2	nCol		�T�u�A�C�e���ԍ�
/// @param3	lstItems	�R���{�{�b�N�X�I�����X�g
/// @param4	nSel		���X�g�I���ʒu
/// @return	���������ꍇ��CComboBox�|�C���^��Ԃ�
//============================================================================
template <class T>
CComboBox* CListCtrlEx::ShowInPlaceList(int nItem, int nCol, T& lstItems, int nSel)
{
	// �A�C�e��������Ԃ����`�F�b�N����
	if (EnsureVisible(nItem, TRUE) == FALSE)
		return NULL;

	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	if (nCol >= nColumnCount || GetColumnWidth(nCol) < 10)
		return NULL;

	// �w��J�����̃I�t�Z�b�g���擾
	int offset = 0;
	for (int i = 0; i < nCol; i++)
		offset += GetColumnWidth(i);

	CRect rect;
	GetItemRect(nItem, &rect, LVIR_BOUNDS);

	// �X�N���[���o�[���l������
	CRect rcClient;
	GetClientRect(&rcClient);
	if (offset + rect.left < 0 || offset + rect.left > rcClient.right){
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;   
		Scroll(size);
		rect.left -= size.cx;
	}

	rect.left += offset;
	rect.right = rect.left + GetColumnWidth(nCol);
	int height = rect.bottom - rect.top -3;
	rect.bottom = height;

	DWORD dwStyle = WS_BORDER | WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST;
	CComboBox *pList = NULL;

	if (nCol == 1){
		pList = new CListComboEx(this, nItem, nCol, (vector<CString>&)lstItems, nSel, eMessage_PropetyCmbChange);
		pList->Create(dwStyle, rect, this, eMessage_PropetyCmbChange);

		pList->SetItemHeight(-1, height);
		pList->SetHorizontalExtent(GetColumnWidth(nCol));

		// �R���{�{�b�N�X�I��ݒ�
		pList->SelectString(0, GetItemText(nItem, nCol));
	}

	//m_bInplace = true;
	return pList;
}

LRESULT CListCtrlEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LV_DISPINFO*	pdisp;
	LV_ITEM*		pitem;

	switch (message)
	{
		case eMessage_PropetyCmbChange:
		{
			pdisp = (LV_DISPINFO *)lParam;
			pitem = &pdisp->item;
			if (pitem->pszText != NULL){
				SetItemText(pitem->iItem, pitem->iSubItem, pitem->pszText);
				LPARAM lp = GetItemData(pitem->iItem);
				if (m_pParent != NULL)
				{
					m_pParent->PostMessage(eMessage_PropetyCmbChange, pitem->iItem, lp);
				}
			}
			return TRUE;
		}
		break;

		default:
			return CListCtrl::WindowProc(message, wParam, lParam);
	}
	return NULL;
}

BOOL CListCtrlEx::GetCellRect(int iRow, int iCol, int nArea, CRect &rect)
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

COLORREF CListCtrlEx::GetCelColorAt(int row)
{
	int			cnt = 0;
	COLORREF	color = NULL;

	vector<COLORREF>::iterator itr;
	for (itr = m_ColorList.begin(); itr != m_ColorList.end(); ++itr)
	{
		if (cnt == row)
		{
			color = (COLORREF)(*itr);
		}
		++cnt;
	}
	return color;
}

COLORREF CListCtrlEx::OnGetCellBkColor(int nRow, int nColum)
{
	COLORREF color = NULL;
	if (nColum == 0)
	{
		return GetCelColorAt(nRow);
	}

	return GetBkColor();
}


void CListCtrlEx::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
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

//			COLORREF color = OnGetCellBkColor(nItem, nSubItem);
			COLORREF color = OnGetCellBkColor(nLparam, nSubItem);
			lplvcd->clrTextBk = color;
			lplvcd->clrText		= color ^ 0xFFFFFF;
			if (nSubItem == 0) {
				pDC->FillSolidRect(rcItem, color);
				pDC->FrameRect(rcItem, &CBrush(color));
			}
			*pResult = CDRF_DODEFAULT;
		}
		break;
	default:
		*pResult = CDRF_DODEFAULT;
		break;
	}

//	*pResult = 0;
}

void CListCtrlEx::SetIsDataSetting(int cel, vector<CString>& list)
{
	m_DataNameList = list;


	int nItemCnt = GetItemCount();
	for (int i = 0; i < nItemCnt; ++i)
	{
		DeleteItem(0);
	}

	CString dataName;
	LVITEM       lvi;
	CString      str;
	int cnt = 0;
	int hitCnt = 0;

	vector<CString>::iterator itr;
	for (itr = m_DataNameList.begin(); itr != m_DataNameList.end(); ++itr)
	{
		dataName = (CString)(*itr);
		if (dataName != _T(""))
		{
			// �f�[�^
			lvi.lParam = (LPARAM)cnt;
			lvi.iItem = hitCnt;
			lvi.iSubItem = 0;
			lvi.mask = LVIF_TEXT | LVIF_PARAM;
			str.Format(_T("�f�[�^%02d"), cnt + 1);
			lvi.pszText = str.GetBuffer();
			InsertItem(&lvi);

			// �`�惂�[�h
			lvi.lParam = (LPARAM)cnt;
			lvi.iItem = hitCnt;
			lvi.iSubItem = 1;
			lvi.mask = LVIF_TEXT | LVIF_PARAM;
			str.Format(_T("�`�惂�[�h%02d"), cnt + 1);
			lvi.pszText = str.GetBuffer();
			SetItem(&lvi);
			++hitCnt;
		}
		++cnt;
	}
}
