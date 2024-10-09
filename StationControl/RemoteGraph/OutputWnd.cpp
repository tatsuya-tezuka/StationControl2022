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

#include "OutputWnd.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
	for( int i = 0; i < MAX_VIEW; i++ )
		m_listDisplayTab[i] = false;

	memset(m_listDataId, 0xff, sizeof(m_listDataId));
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
	//m_wndTabs.AddTab(&m_wndOutputBuild, strTabName, (UINT)0);
	//bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
	//ASSERT(bNameValid);
	//m_wndTabs.AddTab(&m_wndOutputDebug, strTabName, (UINT)1);
	//bNameValid = strTabName.LoadString(IDS_FIND_TAB);
	//ASSERT(bNameValid);
	//m_wndTabs.AddTab(&m_wndOutputFind, strTabName, (UINT)2);

	// �o�̓^�u�Ƀ_�~�[ �e�L�X�g����͂��܂�
	//FillBuildWindow();
	//FillDebugWindow();
	//FillFindWindow();

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// �^�u �R���g���[���́A�N���C�A���g�̈�S�̂��J�o�[����K�v������܂�:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
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

void COutputWnd::FillBuildWindow()
{
	m_wndOutputBuild.AddString(_T("�ő�l    �F99999"));
	m_wndOutputBuild.AddString(_T("�ŏ��l    �F99999"));
	m_wndOutputBuild.AddString(_T("���ϒl    �F99999"));
	m_wndOutputBuild.AddString(_T("�J�[�\���l�F99999"));
}

void COutputWnd::FillDebugWindow()
{
	m_wndOutputDebug.AddString(_T("�f�o�b�O�o�̓f�[�^�������ɕ\������܂��B"));
	m_wndOutputDebug.AddString(_T("�o�̓f�[�^�̓��X�g �r���[�̊e�s�ɕ\������܂�"));
	m_wndOutputDebug.AddString(_T("�\�����@��ύX���邱�Ƃ��ł��܂�..."));
}

void COutputWnd::FillFindWindow()
{
	m_wndOutputFind.AddString(_T("�����o�̓f�[�^�������ɕ\������܂��B"));
	m_wndOutputFind.AddString(_T("�o�̓f�[�^�̓��X�g �r���[�̊e�s�ɕ\������܂�"));
	m_wndOutputFind.AddString(_T("�\�����@��ύX���邱�Ƃ��ł��܂�..."));
}

void COutputWnd::UpdateFonts()
{
	//m_wndOutputBuild.SetFont(&afxGlobalData.fontRegular);
	//m_wndOutputDebug.SetFont(&afxGlobalData.fontRegular);
	//m_wndOutputFind.SetFont(&afxGlobalData.fontRegular);
}


/*============================================================================*/
/*! �o�̓E�B���h�E

-# �^�u�̒ǉ�

@param		pos		�E�B���h�E�C���f�b�N�X

@retval

*/
/*============================================================================*/
void COutputWnd::AddTab(UINT pos)
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
	if (m_ListOutput[pos].GetSafeHwnd() == NULL){
		const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | LVS_REPORT;
		if (!m_ListOutput[pos].Create(dwStyle, m_rectList, &m_wndTabs, pos+100))
		{
			TRACE0("�o�̓E�B���h�E���쐬�ł��܂���ł���\n");
			return;      // �쐬�ł��Ȃ��ꍇ
		}
		m_ListOutput[pos].SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
//		m_ListOutput[pos].InsertColumn(0, _T("No."), LVCFMT_LEFT, 100);
//		m_ListOutput[pos].InsertColumn(0, _T("Time"), LVCFMT_LEFT, 150);
		for (int i = 0; i < MAX_TDATA; i++){
			m_ListOutput[pos].InsertColumn(i + 0, prop.item[i].dataGName, LVCFMT_LEFT, 120);
//			m_ListOutput[pos].SetItemText(0, i + 1, _T("XXXX"));
		}
		m_ListOutput[pos].InsertItem(0, _T(""));
		m_ListOutput[pos].InsertItem(1, _T(""));
		m_ListOutput[pos].InsertItem(2, _T(""));
		m_ListOutput[pos].InsertItem(3, _T(""));
		m_ListOutput[pos].InsertItem(4, _T(""));
		m_ListOutput[pos].m_pos = pos;
	}

	// �f�[�^ID���X�V
	UpdateDataId(pos);

//	m_wndTabs.AddTab(&m_ListOutput[pos], strTabName, (UINT)pos);
	UINT	insert = 0;
	for( UINT i = 0; i < MAX_VIEW; i++ )
	{
		if( m_listDisplayTab[i] && (i > pos) )
			break;

		if( m_listDisplayTab[i] )
			insert++;
	}

	m_listDisplayTab[pos] = true;

	m_wndTabs.InsertTab(&m_ListOutput[pos], strTabName, insert, (UINT)pos);

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
void COutputWnd::UpdateDataId(UINT pos)
{
	if (m_ListOutput[pos].GetSafeHwnd() == NULL)
		return;

	CPropertiesData& prop = theApp.GetPropertiesData(pos);

	for (int i = 0; i < MAX_TDATA; i++)
	{
		LVCOLUMN	col;
		col.mask = LVCF_TEXT;
//		m_ListOutput[pos].GetColumn(i + 1, &col);
		col.pszText = LPTSTR((LPCTSTR)prop.item[i].dataGName);
		m_ListOutput[pos].SetColumn(i + 0, &col);

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
void COutputWnd::UpdateTab(UINT pos)
{
	// ���ɃA�N�e�B�u�Ȃ�X���[
	CMFCOutputListCtrlEx	*pWnd = (CMFCOutputListCtrlEx *)m_wndTabs.GetTabWnd(m_wndTabs.GetActiveTab());
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
void COutputWnd::DeleteTab(UINT pos)
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
void COutputWnd::UpdateDataEx(UINT pos)
{
	// �v���p�e�B�f�[�^
	CPropertiesData& prop = theApp.GetPropertiesData(pos);

	// �w��v���p�e�B�f�[�^�̃E�B���h�E������Y���^�u�̃E�B���h�E�n���h�����擾
	CMFCOutputListCtrlEx	*pWnd = (CMFCOutputListCtrlEx *)m_wndTabs.GetTabWnd(SearchTabIndex(prop.wndName));
	if( pWnd == NULL )
		return;

	if( prop.isRealTime )
		CGraphMutex::Lock(eRealTimeData);
	else
		CGraphMutex::Lock_Analysis(pos);

	// �Ď��f�[�^���X�g�̎擾
	deque<CSpaceData>&	spaceDataList = theApp.GetDataCollection().GetSpaceData();

	// �Y������Ď��f�[�^�̒l���o��
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		CString	str;
		double	max, min, mmax, mmin;

		bool	b_getdata = false;
		bool	b_mm_getdata = false;
		bool	b_validdata = false;
		
		if( prop.isRealTime )
		{
			int	index = -1;

			// �Ď��f�[�^���X�g�̃��[�v
			for( int j = 0; j < spaceDataList.size(); j++ )
			{
				// �I�𒆂̊Ď��f�[�^ID����v����Ď��f�[�^�����o
				if( m_listDataId[pos][i] != spaceDataList[j].dataid )
					continue;

				index = j;

				break;
			}

			if( index != -1 )
			{
				stSpaceData&	sData = spaceDataList[index].value.back();

				max = sData.mmPair.max;
				min = sData.mmPair.min;
				b_getdata = true;

				mmax = spaceDataList[index].data_max;
				mmin = spaceDataList[index].data_min;
				b_mm_getdata = ((mmax == -DBL_MAX) || (mmin == DBL_MAX)) ? false : true;

				b_validdata = (sData.status == SDS_INVALID) ? false : true;
			}
		}
		else
		{
			vector<stSpaceData>&	data = theApp.GetDataCollection().GetTrendCursorData(pos, i);

			CChildFrame	*pChildFrame = (CChildFrame *)((CMainFrame *)theApp.m_pMainWnd)->FindChildListWnd(pos);

			if( (pChildFrame != NULL) && (data.size() != 0) )
			{
				max = data[0].mmPair.max;
				min = data[0].mmPair.min;
				b_getdata = true;

				stMMPair&	pair = theApp.GetDataCollection().GetTrendZoomMMPair(pos, i);
				mmax = pair.max;
				mmin = pair.min;
//				pChildFrame->GetMinMaxValue(mmin, mmax);	// ZOOM����Min/Max�l���擾

				double	t_max, t_min;
				pChildFrame->GetMinMaxValueEx(t_min, t_max);
				b_mm_getdata = ((mmax == -DBL_MAX) || (mmin == DBL_MAX) || (t_max == -DBL_MAX) || (t_min == DBL_MAX) || (t_max == -FLT_MAX) || (t_min == FLT_MAX)) ? false : true;

				b_validdata = (data[0].status == SDS_INVALID) ? false : true;
			}
		}

		int	offs = 0;

		if( b_getdata )
		{
			// Min/Max�l��\��
			if( b_validdata )
			{
				str.Format(_T("%.6f %s"), max, (LPCTSTR)prop.item[i].dataUnit);
				pWnd->SetItemText(0, i + offs, str);
				str.Format(_T("%.6f %s"), min, (LPCTSTR)prop.item[i].dataUnit);
				pWnd->SetItemText(1, i + offs, str);
			}
			else
			{
				pWnd->SetItemText(0, i + offs, _T("---"));
				pWnd->SetItemText(1, i + offs, _T("---"));
			}

			// ���vMin�^Max��\��
			if( b_mm_getdata )
			{
				str.Format(_T("%.6f %s"), mmax, (LPCTSTR)prop.item[i].dataUnit);
				pWnd->SetItemText(2, i + offs, str);
				str.Format(_T("%.6f %s"), mmin, (LPCTSTR)prop.item[i].dataUnit);
				pWnd->SetItemText(3, i + offs, str);
			}
			else
			{
				pWnd->SetItemText(2, i + offs, _T(""));
				pWnd->SetItemText(3, i + offs, _T(""));
			}

			// �R�����g��\��
			pWnd->SetItemText(4, i + offs, prop.item[i].comment);
		}
		else
		{
			// �Ď��f�[�^�����o����Ȃ�������\�����N���A
			pWnd->SetItemText(0, i + offs, _T(""));
			pWnd->SetItemText(1, i + offs, _T(""));
			pWnd->SetItemText(2, i + offs, _T(""));
			pWnd->SetItemText(3, i + offs, _T(""));
			pWnd->SetItemText(4, i + offs, _T(""));
		}
	}

	if( prop.isRealTime )
		CGraphMutex::Unlock(eRealTimeData);
	else
		CGraphMutex::Unlock_Analysis(pos);
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
int COutputWnd::AddItem(int pos, int nItem, int nSubItem, CString strItem, DWORD dwAll/*=0*/)
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
		return m_ListOutput[pos].InsertItem(&lv);
	return m_ListOutput[pos].SetItem(&lv);
}


/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList ���b�Z�[�W �n���h���[

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

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

void COutputList::OnEditCopy()
{
	MessageBox(_T("�o�̓f�[�^���R�s�[���܂�"));
}

void COutputList::OnEditClear()
{
	MessageBox(_T("�o�̓f�[�^���N���A���܂�"));
}

void COutputList::OnViewOutput()
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




COutputListCtrl::COutputListCtrl()
{
}

COutputListCtrl::~COutputListCtrl()
{
}


BEGIN_MESSAGE_MAP(COutputListCtrl, CListCtrl)
END_MESSAGE_MAP()


// CMFCOutputListCtrlEx

IMPLEMENT_DYNAMIC(CMFCOutputListCtrlEx, CMFCListCtrl)

CMFCOutputListCtrlEx::CMFCOutputListCtrlEx()
{
	m_pos = -1;
}

CMFCOutputListCtrlEx::~CMFCOutputListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CMFCOutputListCtrlEx, CMFCListCtrl)
END_MESSAGE_MAP()



// CMFCOutputListCtrlEx ���b�Z�[�W �n���h���[


// CMFCOutputTabCtrlEx

IMPLEMENT_DYNAMIC(CMFCOutputTabCtrlEx, CMFCTabCtrl)

CMFCOutputTabCtrlEx::CMFCOutputTabCtrlEx()
{

}

CMFCOutputTabCtrlEx::~CMFCOutputTabCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CMFCOutputTabCtrlEx, CMFCTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


BOOL CMFCOutputTabCtrlEx::OnCommand(WPARAM wParam, LPARAM lParam)
{
	BOOL bScrollTabs = FALSE;

	int count = m_iTabsNum;

	// OnCommand�Ń{�^���C�x���g����������
	if ((HWND)lParam == m_btnScrollLeft.GetSafeHwnd())
	{
		if (m_iActiveTab != 0){
			SetActiveTab(m_iActiveTab - 1);

			// �Y���r���[���A�N�e�B�u�ɐݒ�
			CMFCOutputListCtrlEx	*pWnd = (CMFCOutputListCtrlEx *)GetTabWnd(GetActiveTab());
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
			CMFCOutputListCtrlEx	*pWnd = (CMFCOutputListCtrlEx *)GetTabWnd(GetActiveTab());
			if( pWnd != NULL )
				theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)GetActiveTab(), (LPARAM)pWnd->m_pos);
		}
	}
	else if ((HWND)lParam == m_btnScrollFirst.GetSafeHwnd())
	{
		// �ō��{�^������
		SetActiveTab(0);

		// �Y���r���[���A�N�e�B�u�ɐݒ�
		CMFCOutputListCtrlEx	*pWnd = (CMFCOutputListCtrlEx *)GetTabWnd(GetActiveTab());
		if( pWnd != NULL )
			theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)GetActiveTab(), (LPARAM)pWnd->m_pos);
	}
	else if ((HWND)lParam == m_btnScrollLast.GetSafeHwnd())
	{
		// �ŉE�{�^������
		SetActiveTab((m_iTabsNum - 1));

		// �Y���r���[���A�N�e�B�u�ɐݒ�
		CMFCOutputListCtrlEx	*pWnd = (CMFCOutputListCtrlEx *)GetTabWnd(GetActiveTab());
		if( pWnd != NULL )
			theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)GetActiveTab(), (LPARAM)pWnd->m_pos);
	}

	return CMFCTabCtrl::OnCommand(wParam, lParam);
}

#if 0
void CMFCOutputTabCtrlEx::FireChangeActiveTab(int nNewTab)
{
	CMFCOutputListCtrlEx	*pWnd = (CMFCOutputListCtrlEx *)GetTabWnd(nNewTab);

	if( pWnd != NULL )
		theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)nNewTab, (LPARAM)pWnd->m_pos);

	return CMFCTabCtrl::FireChangeActiveTab(nNewTab);
}
#endif

void CMFCOutputTabCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMFCTabCtrl::OnLButtonDown(nFlags, point);

	// �Y���r���[���A�N�e�B�u�ɐݒ�
	CMFCOutputListCtrlEx	*pWnd = (CMFCOutputListCtrlEx *)GetTabWnd(GetActiveTab());
	if( pWnd != NULL )
		theApp.m_pMainWnd->PostMessage(eMessage_ChangeActiveTab, (WPARAM)GetActiveTab(), (LPARAM)pWnd->m_pos);
}

LRESULT COutputWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

			if( prop->isRealTime )
				UpdateDataEx((UINT)wParam);
		}
		break;

	default:
		return CDockablePane::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}
