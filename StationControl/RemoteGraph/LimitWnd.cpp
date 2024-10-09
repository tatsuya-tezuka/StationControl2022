// ���� MFC �T���v�� �\�[�X �R�[�h�ł́AMFC Microsoft Office Fluent ���[�U�[ �C���^�[�t�F�C�X 
// ("Fluent UI") �̎g�p���@�������܂��B���̃R�[�h�́AMFC C++ ���C�u���� �\�t�g�E�F�A�� 
// ��������Ă��� Microsoft Foundation Class ���t�@�����X����ъ֘A�d�q�h�L�������g��
// �⊮���邽�߂̎Q�l�����Ƃ��Ē񋟂���܂��B
// Fluent UI �𕡐��A�g�p�A�܂��͔z�z���邽�߂̃��C�Z���X�����͌ʂɗp�ӂ���Ă��܂��B
// Fluent UI ���C�Z���X �v���O�����̏ڍׂɂ��ẮAWeb �T�C�g
// http://go.microsoft.com/fwlink/?LinkId=238214 ���Q�Ƃ��Ă��������B
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#include "stdafx.h"
#include "RemoteGraph.h"

#include "LimitWnd.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLimitBar

CLimitWnd::CLimitWnd()
{
	for( int i = 0; i < MAX_VIEW; i++ )
		m_listDisplayTab[i] = false;

	memset(m_listDataId, 0xff, sizeof(m_listDataId));
}

CLimitWnd::~CLimitWnd()
{
}

BEGIN_MESSAGE_MAP(CLimitWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CLimitWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_rectList.SetRectEmpty();

	// �^�u�t���E�B���h�E�̍쐬:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, m_rectList, this, 1))
	{
		TRACE0("�^�u�t���o�̓E�B���h�E���쐬�ł��܂���ł���\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}

	//UpdateFonts();

	//CString strTabName;
	//BOOL bNameValid;

	// �ꗗ�E�B���h�E���^�u�Ɋ��蓖�Ă܂�:
	//bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
	//ASSERT(bNameValid);
	//strTabName = _T("���l");
	//m_wndTabs.AddTab(&m_wndLimitBuild, strTabName, (UINT)0);
	//bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
	//ASSERT(bNameValid);
	//m_wndTabs.AddTab(&m_wndLimitDebug, strTabName, (UINT)1);
	//bNameValid = strTabName.LoadString(IDS_FIND_TAB);
	//ASSERT(bNameValid);
	//m_wndTabs.AddTab(&m_wndLimitFind, strTabName, (UINT)2);

	// �o�̓^�u�Ƀ_�~�[ �e�L�X�g����͂��܂�
	//FillBuildWindow();
	//FillDebugWindow();
	//FillFindWindow();

	return 0;
}

void CLimitWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// �^�u �R���g���[���́A�N���C�A���g�̈�S�̂��J�o�[����K�v������܂�:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void CLimitWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, (int)dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void CLimitWnd::FillBuildWindow()
{
	m_wndLimitBuild.AddString(_T("�ő�l    �F99999"));
	m_wndLimitBuild.AddString(_T("�ŏ��l    �F99999"));
	m_wndLimitBuild.AddString(_T("���ϒl    �F99999"));
	m_wndLimitBuild.AddString(_T("�J�[�\���l�F99999"));
}

void CLimitWnd::FillDebugWindow()
{
	m_wndLimitDebug.AddString(_T("�f�o�b�O�o�̓f�[�^�������ɕ\������܂��B"));
	m_wndLimitDebug.AddString(_T("�o�̓f�[�^�̓��X�g �r���[�̊e�s�ɕ\������܂�"));
	m_wndLimitDebug.AddString(_T("�\�����@��ύX���邱�Ƃ��ł��܂�..."));
}

void CLimitWnd::FillFindWindow()
{
	m_wndLimitFind.AddString(_T("�����o�̓f�[�^�������ɕ\������܂��B"));
	m_wndLimitFind.AddString(_T("�o�̓f�[�^�̓��X�g �r���[�̊e�s�ɕ\������܂�"));
	m_wndLimitFind.AddString(_T("�\�����@��ύX���邱�Ƃ��ł��܂�..."));
}

void CLimitWnd::UpdateFonts()
{
	//m_wndLimitBuild.SetFont(&afxGlobalData.fontRegular);
	//m_wndLimitDebug.SetFont(&afxGlobalData.fontRegular);
	//m_wndLimitFind.SetFont(&afxGlobalData.fontRegular);
}


/*============================================================================*/
/*! �o�̓E�B���h�E

-# �^�u�̒ǉ�

@param		pos		�E�B���h�E�C���f�b�N�X

@retval

*/
/*============================================================================*/
void CLimitWnd::AddTab(UINT pos)
{
	// �v���p�e�B�f�[�^
	CPropertiesData& prop = theApp.GetPropertiesData(pos);

	CString strTabName;
	BOOL bNameValid;
	// �ꗗ�E�B���h�E���^�u�Ɋ��蓖�Ă܂�:
	bNameValid = strTabName.LoadString(IDS_DATA_TAB_1+pos);
	ASSERT(bNameValid);
	strTabName = prop.wndName;

	// �o�̓��X�g�̍쐬
	if (m_ListLimit[pos].GetSafeHwnd() == NULL){
		const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | LVS_REPORT;
		if (!m_ListLimit[pos].Create(dwStyle, m_rectList, &m_wndTabs, pos+100))
		{
			TRACE0("�o�̓E�B���h�E���쐬�ł��܂���ł���\n");
			return;      // �쐬�ł��Ȃ��ꍇ
		}
		m_ListLimit[pos].SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
//		m_ListLimit[pos].InsertColumn(0, _T("No."), LVCFMT_LEFT, 100);
//		m_ListLimit[pos].InsertColumn(0, _T("Time"), LVCFMT_LEFT, 150);
		for (int i = 0; i < MAX_TDATA; i++){
			m_ListLimit[pos].InsertColumn(i + 0, prop.item[i].dataGName, LVCFMT_LEFT, 230);
//			m_ListLimit[pos].SetItemText(0, i + 1, _T("XXXX"));
		}
		for( int i = 0; i < MAX_LIMIT_HISTORY; i++ )
			m_ListLimit[pos].InsertItem(i, _T(""));
		m_ListLimit[pos].m_pos = pos;
	}

	// �f�[�^ID���X�V
	UpdateDataId(pos);

//	m_wndTabs.AddTab(&m_ListLimit[pos], strTabName, (UINT)pos);
	UINT	insert = 0;
	for( UINT i = 0; i < MAX_VIEW; i++ )
	{
		if( m_listDisplayTab[i] && (i > pos) )
			break;

		if( m_listDisplayTab[i] )
			insert++;
	}

	m_listDisplayTab[pos] = true;

	m_wndTabs.InsertTab(&m_ListLimit[pos], strTabName, insert, (UINT)pos);

	// �ǉ������^�u�ɃA�N�e�B�u�ɕύX
	UpdateTab(pos);
}

/*============================================================================*/
/*! �o�̓E�B���h�E

-# �f�[�^ID���X�V

@param		pos		�E�B���h�E�C���f�b�N�X

@retval

*/
/*============================================================================*/
void CLimitWnd::UpdateDataId(UINT pos)
{
	if (m_ListLimit[pos].GetSafeHwnd() == NULL)
		return;

	CPropertiesData& prop = theApp.GetPropertiesData(pos);

	for (int i = 0; i < MAX_TDATA; i++)
	{
		LVCOLUMN	col;
		col.mask = LVCF_TEXT;
//		m_ListLimit[pos].GetColumn(i + 1, &col);
		col.pszText = LPTSTR((LPCTSTR)prop.item[i].dataGName);
		m_ListLimit[pos].SetColumn(i + 0, &col);

		m_listDataId[pos][i] = theApp.GetDataCollection().GetSelectDataId(prop.item[i].dataName);
	}
}

/*============================================================================*/
/*! �o�̓E�B���h�E

-# �^�u�̒ǉ�

@param		pos		�E�B���h�E�C���f�b�N�X

@retval

*/
/*============================================================================*/
void CLimitWnd::UpdateTab(UINT pos)
{
	// ���ɃA�N�e�B�u�Ȃ�X���[
	CMFCLimitListCtrlEx	*pWnd = (CMFCLimitListCtrlEx *)m_wndTabs.GetTabWnd(m_wndTabs.GetActiveTab());
	if( pWnd != NULL && pWnd->m_pos == pos )
		return;
	
	// �v���p�e�B�f�[�^
	CPropertiesData& prop = theApp.GetPropertiesData(pos);

	CString strTabName;
	BOOL bNameValid;

	// �ꗗ�E�B���h�E���^�u�Ɋ��蓖�Ă܂�:
	bNameValid = strTabName.LoadString(IDS_DATA_TAB_1 + pos);
	ASSERT(bNameValid);

	// �Y���^�u���A�N�e�B�u�ɕύX
	m_wndTabs.SetActiveTab(SearchTabIndex(prop.wndName));
}

/*============================================================================*/
/*! �o�̓E�B���h�E

-# �^�u�̒ǉ�

@param		pos		�E�B���h�E�C���f�b�N�X

@retval

*/
/*============================================================================*/
void CLimitWnd::DeleteTab(UINT pos)
{
	// �v���p�e�B�f�[�^
	CPropertiesData& prop = theApp.GetPropertiesData(pos);

	// �Y���^�u���폜
	m_wndTabs.RemoveTab(SearchTabIndex(prop.wndName));

	m_listDisplayTab[pos] = false;
}

/*============================================================================*/
/*! �o�̓E�B���h�E

-# �f�[�^�̍X�V

@param		pos		�E�B���h�E�C���f�b�N�X
@param		time	�f�[�^����
@param		list	�ő�f�[�^���P�O�̔z��i�O���t��\���̏ꍇ���j

@retval

*/
/*============================================================================*/
void CLimitWnd::UpdateDataEx(UINT pos)
{
	// �v���p�e�B�f�[�^
	CPropertiesData& prop = theApp.GetPropertiesData(pos);

	// �w��v���p�e�B�f�[�^�̃E�B���h�E������Y���^�u�̃E�B���h�E�n���h�����擾
	CMFCLimitListCtrlEx	*pWnd = (CMFCLimitListCtrlEx *)m_wndTabs.GetTabWnd(SearchTabIndex(prop.wndName));
	if( pWnd == NULL )
		return;

	CGraphMutex::Lock(eLimitState);

	// �Y������Ď��f�[�^�̒l���o��
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		deque<stLimitState>&	tmpLimitState = theApp.GetDataCollection().GetLimitState(pos, i);

		for( int j = 0; j < MAX_LIMIT_HISTORY; j++ )
		{
			// ���������݂��邩�H
			if( j < tmpLimitState.size() )
			{
				CString	str;

				str.Format(_T("%s %s %s%02d[%s] %s"),
					(LPCTSTR)(tmpLimitState[j].time.Format("%Y/%m/%d")),
					(LPCTSTR)(tmpLimitState[j].time.Format("%H:%M:%S")),
					(GET_LIMIT_KIND(tmpLimitState[j].state) == LimitWarning) ? _T("w") : _T("a"),
					i + 1,
					(GET_LIMIT_RANGE(tmpLimitState[j].state) == LimitUpper) ? _T("up") : _T("lo"),
					(GET_LIMIT_STATUS(tmpLimitState[j].state) == LimitAbove) ? _T("above") : _T("below"));

				pWnd->SetItemText(j, i, str);
			}
			else
				pWnd->SetItemText(j, i, _T(""));
		}
	}

	CGraphMutex::Unlock(eLimitState);
}

/*============================================================================*/
/*! �o�̓E�B���h�E

-# ���X�g�ւ̃f�[�^�o�^

@param		pos			�E�B���h�E�C���f�b�N�X
@param		nItem		�A�C�e���ԍ�
@param		nSubItem	�T�u�A�C�e���ԍ�
@param		strItem		������
@param		dwAll		�f�[�^

@retval		int

*/
/*============================================================================*/
int CLimitWnd::AddItem(int pos, int nItem, int nSubItem, CString strItem, DWORD dwAll/*=0*/)
{
	LV_ITEM lv;
	memset(&lv, 0, sizeof(lv));
	lv.mask = LVIF_TEXT;
	lv.iItem = nItem;
	lv.iSubItem = nSubItem;
	if (nSubItem == 0){
		lv.mask |= LVIF_PARAM;
		lv.lParam = dwAll;
	}
	lv.pszText = (LPTSTR)(LPCTSTR)strItem;
	lv.cchTextMax = strItem.GetLength();

	if (nSubItem == 0)
		return m_ListLimit[pos].InsertItem(&lv);
	return m_ListLimit[pos].SetItem(&lv);
}


/////////////////////////////////////////////////////////////////////////////
// CLimitList1

CLimitList::CLimitList()
{
}

CLimitList::~CLimitList()
{
}

BEGIN_MESSAGE_MAP(CLimitList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_LIMITWND, OnViewLimit)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CLimitList ���b�Z�[�W �n���h���[

void CLimitList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_LIMIT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void CLimitList::OnEditCopy()
{
	MessageBox(_T("�o�̓f�[�^���R�s�[���܂�"));
}

void CLimitList::OnEditClear()
{
	MessageBox(_T("�o�̓f�[�^���N���A���܂�"));
}

void CLimitList::OnViewLimit()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}




CLimitListCtrl::CLimitListCtrl()
{
}

CLimitListCtrl::~CLimitListCtrl()
{
}


BEGIN_MESSAGE_MAP(CLimitListCtrl, CListCtrl)
END_MESSAGE_MAP()


// CMFCLimitListCtrlEx

IMPLEMENT_DYNAMIC(CMFCLimitListCtrlEx, CMFCListCtrl)

CMFCLimitListCtrlEx::CMFCLimitListCtrlEx()
{
	m_pos = -1;
}

CMFCLimitListCtrlEx::~CMFCLimitListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CMFCLimitListCtrlEx, CMFCListCtrl)
END_MESSAGE_MAP()



// CMFCLimitListCtrlEx ���b�Z�[�W �n���h���[


// CMFCLimitTabCtrlEx

IMPLEMENT_DYNAMIC(CMFCLimitTabCtrlEx, CMFCTabCtrl)

CMFCLimitTabCtrlEx::CMFCLimitTabCtrlEx()
{

}

CMFCLimitTabCtrlEx::~CMFCLimitTabCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CMFCLimitTabCtrlEx, CMFCTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


BOOL CMFCLimitTabCtrlEx::OnCommand(WPARAM wParam, LPARAM lParam)
{
	BOOL bScrollTabs = FALSE;

	int count = m_iTabsNum;

	// OnCommand�Ń{�^���C�x���g����������
	if ((HWND)lParam == m_btnScrollLeft.GetSafeHwnd())
	{
		if (m_iActiveTab != 0){
			SetActiveTab(m_iActiveTab - 1);

			// �Y���r���[���A�N�e�B�u�ɐݒ�
			CMFCLimitListCtrlEx	*pWnd = (CMFCLimitListCtrlEx *)GetTabWnd(GetActiveTab());
			if( pWnd != NULL )
				theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)GetActiveTab(), (LPARAM)pWnd->m_pos);
		}
		// ���{�^������
	}
	else if ((HWND)lParam == m_btnScrollRight.GetSafeHwnd())
	{
		// �E�{�^������
		if (m_iActiveTab != (m_iTabsNum-1)){
			SetActiveTab(m_iActiveTab + 1);

			// �Y���r���[���A�N�e�B�u�ɐݒ�
			CMFCLimitListCtrlEx	*pWnd = (CMFCLimitListCtrlEx *)GetTabWnd(GetActiveTab());
			if( pWnd != NULL )
				theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)GetActiveTab(), (LPARAM)pWnd->m_pos);
		}
	}
	else if ((HWND)lParam == m_btnScrollFirst.GetSafeHwnd())
	{
		// �ō��{�^������
		SetActiveTab(0);

		// �Y���r���[���A�N�e�B�u�ɐݒ�
		CMFCLimitListCtrlEx	*pWnd = (CMFCLimitListCtrlEx *)GetTabWnd(GetActiveTab());
		if( pWnd != NULL )
			theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)GetActiveTab(), (LPARAM)pWnd->m_pos);
	}
	else if ((HWND)lParam == m_btnScrollLast.GetSafeHwnd())
	{
		// �ŉE�{�^������
		SetActiveTab((m_iTabsNum - 1));

		// �Y���r���[���A�N�e�B�u�ɐݒ�
		CMFCLimitListCtrlEx	*pWnd = (CMFCLimitListCtrlEx *)GetTabWnd(GetActiveTab());
		if( pWnd != NULL )
			theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)GetActiveTab(), (LPARAM)pWnd->m_pos);
	}

	return CMFCTabCtrl::OnCommand(wParam, lParam);
}

#if 0
void CMFCLimitTabCtrlEx::FireChangeActiveTab(int nNewTab)
{
	CMFCLimitListCtrlEx	*pWnd = (CMFCLimitListCtrlEx *)GetTabWnd(nNewTab);

	if( pWnd != NULL )
		theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)nNewTab, (LPARAM)pWnd->m_pos);

	return CMFCTabCtrl::FireChangeActiveTab(nNewTab);
}
#endif

void CMFCLimitTabCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMFCTabCtrl::OnLButtonDown(nFlags, point);

	// �Y���r���[���A�N�e�B�u�ɐݒ�
	CMFCLimitListCtrlEx	*pWnd = (CMFCLimitListCtrlEx *)GetTabWnd(GetActiveTab());
	if( pWnd != NULL )
		theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)GetActiveTab(), (LPARAM)pWnd->m_pos);
}

LRESULT CLimitWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case eMessage_UpdateCursorLine:
		{
			CPropertiesData	*prop = &theApp.GetPropertiesData((UINT)wParam);

			if( !prop->isRealTime )
				UpdateDataEx((UINT)wParam);
		}
		break;

	//=====================================================================
	//	�X���b�h
	//=====================================================================
	case eMessage_ThreadNormal:
		{
			CPropertiesData	*prop = &theApp.GetPropertiesData((UINT)wParam);

//			if( prop->isRealTime )
				UpdateDataEx((UINT)wParam);
		}
		break;

	default:
		return CDockablePane::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}
