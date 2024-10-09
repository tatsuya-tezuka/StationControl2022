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

#include "PropertiesWnd.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define ID_PROPERTIES_CB			(WM_USER +311)
#define ID_PROPERTIES_TAB           (WM_USER +500)
#define ID_PROPERTIES_LIST          (WM_USER +501)

/////////////////////////////////////////////////////////////////////////////
// CPropertiesData

/*============================================================================*/
/*! �v���p�e�B�f�[�^

-# �v���p�e�B�f�[�^��ۑ�

@param
@retval

*/
/*============================================================================*/
void CPropertiesData::WritePropertiesData(CString szFilePath, int pos)
{
	CString	sec, key, str;

	sec.Format(_T("%s%d"), (LPCTSTR)SECTION_PROPDATA, pos + 1);

	// ���A���^�C���^�m�����A���^�C��
//	str.Format(_T("%d"), isRealTime ? 1 : 0);
//	WritePrivateProfileString(sec, KEY_REALTIME, str, szFilePath);

	// �c���X�P�[��
	str.Format(_T("%d"), trandVScale.Auto ? 1 : 0);
	WritePrivateProfileString(sec, KEY_VSCL_AUTO, str, szFilePath);
	str.Format(_T("%f"), trandVScale.VScalLMax);
	WritePrivateProfileString(sec, KEY_VSCL_LMAX, str, szFilePath);
	str.Format(_T("%f"), trandVScale.VScalLMin);
	WritePrivateProfileString(sec, KEY_VSCL_LMIN, str, szFilePath);
	str.Format(_T("%f"), trandVScale.VScalRMax);
	WritePrivateProfileString(sec, KEY_VSCL_RMAX, str, szFilePath);
	str.Format(_T("%f"), trandVScale.VScalRMin);
	WritePrivateProfileString(sec, KEY_VSCL_RMIN, str, szFilePath);
	str.Format(_T("%d"), trandVScale.VSCalItvl);
	WritePrivateProfileString(sec, KEY_VSCL_ITVL, str, szFilePath);

	// �c���X�P�[��
	key.Format(KEY_HSCL_NUM, 1);
	str.Format(_T("%d"), trandHScale[0]);
	WritePrivateProfileString(sec, key, str, szFilePath);
	key.Format(KEY_HSCL_NUM, 2);
	str.Format(_T("%d"), trandHScale[1]);
	WritePrivateProfileString(sec, key, str, szFilePath);

	// �f�[�^
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		key.Format(KEY_DATA_NAME, i + 1);
		WritePrivateProfileString(sec, key, item[i].dataName, szFilePath);
		key.Format(KEY_DATA_GNAME, i + 1);
		WritePrivateProfileString(sec, key, item[i].dataGName, szFilePath);
		key.Format(KEY_DATA_UNIT, i + 1);
		WritePrivateProfileString(sec, key, item[i].dataUnit, szFilePath);
	}

	// �O���t
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		key.Format(KEY_GRAPH_VMODE, i + 1);
		str.Format(_T("%d"), item[i].isViewMode ? 1 : 0);
		WritePrivateProfileString(sec, key, str, szFilePath);
		key.Format(KEY_GRAPH_DMODE, i + 1);
		str.Format(_T("%d"), item[i].drawMode);
		WritePrivateProfileString(sec, key, str, szFilePath);
		key.Format(KEY_GRAPH_COL, i + 1);
		str.Format(_T("%d"), item[i].color);
		WritePrivateProfileString(sec, key, str, szFilePath);
		key.Format(KEY_GRAPH_DOTSZ, i + 1);
		str.Format(_T("%d"), item[i].dotSize);
		WritePrivateProfileString(sec, key, str, szFilePath);
		key.Format(KEY_GRAPH_VSKIND, i + 1);
		str.Format(_T("%d"), item[i].vscaleKind);
		WritePrivateProfileString(sec, key, str, szFilePath);
	}

	// �R�����g
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		key.Format(KEY_COM_DISP, i + 1);
		str.Format(_T("%d"), item[i].isComment ? 1 : 0);
		WritePrivateProfileString(sec, key, str, szFilePath);
		key.Format(KEY_COM_STR, i + 1);
		WritePrivateProfileString(sec, key, item[i].comment, szFilePath);
	}

	// ���~�b�g
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		key.Format(KEY_LMT_WAR_U_V, i + 1);
		str.Format(_T("%d"), item[i].isLimitWarningUpper ? 1 : 0);
		WritePrivateProfileString(sec, key, str, szFilePath);
		key.Format(KEY_LMT_WAR_U, i + 1);
		str.Format(_T("%f"), item[i].limitWarningUpper);
		WritePrivateProfileString(sec, key, str, szFilePath);

		key.Format(KEY_LMT_WAR_L_V, i + 1);
		str.Format(_T("%d"), item[i].isLimitWarningLower ? 1 : 0);
		WritePrivateProfileString(sec, key, str, szFilePath);
		key.Format(KEY_LMT_WAR_L, i + 1);
		str.Format(_T("%f"), item[i].limitWarningLower);
		WritePrivateProfileString(sec, key, str, szFilePath);

		key.Format(KEY_LMT_ALM_U_V, i + 1);
		str.Format(_T("%d"), item[i].isLimitAlarmUpper ? 1 : 0);
		WritePrivateProfileString(sec, key, str, szFilePath);
		key.Format(KEY_LMT_ALM_U, i + 1);
		str.Format(_T("%f"), item[i].limitAlarmUpper);
		WritePrivateProfileString(sec, key, str, szFilePath);

		key.Format(KEY_LMT_ALM_L_V, i + 1);
		str.Format(_T("%d"), item[i].isLimitAlarmLower ? 1 : 0);
		WritePrivateProfileString(sec, key, str, szFilePath);
		key.Format(KEY_LMT_ALM_L, i + 1);
		str.Format(_T("%f"), item[i].limitAlarmLower);
		WritePrivateProfileString(sec, key, str, szFilePath);
	}
}

/*============================================================================*/
/*! �v���p�e�B�f�[�^

-# �v���p�e�B�f�[�^�𕜌�

@param
@retval

*/
/*============================================================================*/
void CPropertiesData::ReadPropertiesData(CString szFilePath, int pos)
{
	CString	sec, key, str;
	TCHAR	wkbuf[1024];

	sec.Format(_T("%s%d"), (LPCTSTR)SECTION_PROPDATA, pos + 1);

	// ���A���^�C���^�m�����A���^�C��
//	isRealTime = GetPrivateProfileInt(sec, KEY_REALTIME, 1, szFilePath) ? true : false;
	isRealTime = true;	// �����I�Ƀf�t�H���g�����A���^�C��

	// �c���X�P�[��
	trandVScale.Auto = GetPrivateProfileInt(sec, KEY_VSCL_AUTO, 1, szFilePath) ? true : false;
	GetPrivateProfileString(sec, KEY_VSCL_LMAX, _T("100"), wkbuf, _countof(wkbuf), szFilePath);
	trandVScale.VScalLMax = _wtof(wkbuf);
	GetPrivateProfileString(sec, KEY_VSCL_LMIN, _T("-100"), wkbuf, _countof(wkbuf), szFilePath);
	trandVScale.VScalLMin = _wtof(wkbuf);
	GetPrivateProfileString(sec, KEY_VSCL_RMAX, _T("50"), wkbuf, _countof(wkbuf), szFilePath);
	trandVScale.VScalRMax = _wtof(wkbuf);
	GetPrivateProfileString(sec, KEY_VSCL_RMIN, _T("-50"), wkbuf, _countof(wkbuf), szFilePath);
	trandVScale.VScalRMin = _wtof(wkbuf);
	trandVScale.VSCalItvl = GetPrivateProfileInt(sec, KEY_VSCL_ITVL, 5, szFilePath);

	// �c���X�P�[��
	key.Format(KEY_HSCL_NUM, 1);
#ifdef	_TEST_MAXDATA
	trandHScale[0] = (HScale)GetPrivateProfileInt(sec, key, h3, szFilePath);
#else	// 	_TEST_MAXDATA
	trandHScale[0] = (HScale)GetPrivateProfileInt(sec, key, h24, szFilePath);
#endif	// 	_TEST_MAXDATA
	key.Format(KEY_HSCL_NUM, 2);
	trandHScale[1] = (HScale)GetPrivateProfileInt(sec, key, h1, szFilePath);

	// �f�[�^
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		key.Format(KEY_DATA_NAME, i + 1);
		GetPrivateProfileString(sec, key, _T(""), item[i].dataName, _countof(item[i].dataName), szFilePath);
		key.Format(KEY_DATA_GNAME, i + 1);
		str.Format(_T("Data%02d"), i + 1);
		GetPrivateProfileString(sec, key, str, item[i].dataGName, _countof(item[i].dataGName), szFilePath);
		key.Format(KEY_DATA_UNIT, i + 1);
		GetPrivateProfileString(sec, key, _T(""), item[i].dataUnit, _countof(item[i].dataUnit), szFilePath);
	}

	// �O���t
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		key.Format(KEY_GRAPH_VMODE, i + 1);
		item[i].isViewMode = GetPrivateProfileInt(sec, key, 1, szFilePath) ? true : false;
		key.Format(KEY_GRAPH_DMODE, i + 1);
		item[i].drawMode = (DrawMode)GetPrivateProfileInt(sec, key, Line, szFilePath);
		key.Format(KEY_GRAPH_COL, i + 1);
		item[i].color = (COLORREF)GetPrivateProfileInt(sec, key, colordef[i], szFilePath);
		key.Format(KEY_GRAPH_DOTSZ, i + 1);
		item[i].dotSize = (DotSize)GetPrivateProfileInt(sec, key, Small, szFilePath);
		key.Format(KEY_GRAPH_VSKIND, i + 1);
		item[i].vscaleKind = (VScaleKind)GetPrivateProfileInt(sec, key, LScale, szFilePath);
	}

	// �R�����g
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		key.Format(KEY_COM_DISP, i + 1);
		item[i].isComment = GetPrivateProfileInt(sec, key, 0, szFilePath) ? true : false;
		key.Format(KEY_COM_STR, i + 1);
		GetPrivateProfileString(sec, key, _T(""), wkbuf, _countof(wkbuf), szFilePath);
		item[i].comment = CString(wkbuf);
	}

	// ���~�b�g
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		key.Format(KEY_LMT_WAR_U_V, i + 1);
		item[i].isLimitWarningUpper = GetPrivateProfileInt(sec, key, 0, szFilePath) ? true : false;
		key.Format(KEY_LMT_WAR_U, i + 1);
		GetPrivateProfileString(sec, key, _T("0"), wkbuf, _countof(wkbuf), szFilePath);
		item[i].limitWarningUpper = _wtof(wkbuf);

		key.Format(KEY_LMT_WAR_L_V, i + 1);
		item[i].isLimitWarningLower = GetPrivateProfileInt(sec, key, 0, szFilePath) ? true : false;
		key.Format(KEY_LMT_WAR_L, i + 1);
		GetPrivateProfileString(sec, key, _T("0"), wkbuf, _countof(wkbuf), szFilePath);
		item[i].limitWarningLower = _wtof(wkbuf);

		key.Format(KEY_LMT_ALM_U_V, i + 1);
		item[i].isLimitAlarmUpper = GetPrivateProfileInt(sec, key, 0, szFilePath) ? true : false;
		key.Format(KEY_LMT_ALM_U, i + 1);
		GetPrivateProfileString(sec, key, _T("0"), wkbuf, _countof(wkbuf), szFilePath);
		item[i].limitAlarmUpper = _wtof(wkbuf);

		key.Format(KEY_LMT_ALM_L_V, i + 1);
		item[i].isLimitAlarmLower = GetPrivateProfileInt(sec, key, 0, szFilePath) ? true : false;
		key.Format(KEY_LMT_ALM_L, i + 1);
		GetPrivateProfileString(sec, key, _T("0"), wkbuf, _countof(wkbuf), szFilePath);
		item[i].limitAlarmLower = _wtof(wkbuf);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCustomDlgProp

CCustomDlgProp::CCustomDlgProp(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData) : CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData)
{
}

void CCustomDlgProp::OnClickButton(CPoint /*point*/)
{
	CStartDateDlg dlg;
	if (dlg.DoModal() == IDCANCEL)
		return;

	CString date = ((CPropertiesWnd*)GetParent())->GetStartDate();
	SetValue(date);
#if 0
	CTime time1, time2;
	dlg.m_DateTimePicker1.GetTime(time1);
	dlg.m_DateTimePicker2.GetTime(time2);
	CString startday = time1.Format(_T("%y/%m/%d"));
	CString starttime = time2.Format(_T("%H:%M:%S"));

	startday += _T("") + starttime;

#endif

}


/////////////////////////////////////////////////////////////////////////////
// CPropertiesWnd

CPropertiesWnd::CPropertiesWnd()
{
	m_nComboHeight = 0;
	m_ActiveView = 0;

	for (int i = 0; i < MAX_VIEW; ++i)
	{
		m_AutoScale[i] = FALSE;
	}

}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_CBN_SELCHANGE(ID_PROPERTIES_CB, OnComboBoxSelChange)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_NOTIFY(TCN_SELCHANGE, ID_PROPERTIES_TAB, OnTcnSelchangeTab)
	ON_NOTIFY(LVN_ITEMCHANGED, ID_PROPERTIES_LIST, OnItemchangedList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertiesWnd ���b�Z�[�W �n���h���[

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient, rectCombo, rectTab;

	GetClientRect(rectClient);
	m_wndObjectCombo.GetWindowRect(&rectCombo);

//	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
	int cyTlb = 0;
	int cyCmb = rectCombo.Size().cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
//	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() - (cyCmb + cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);

	m_wndTabs.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() - (m_nComboHeight + cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndTabs.GetWindowRect(&rectTab);
	m_wndTabs.AdjustRect(FALSE, rectTab);
	m_wndTabs.ScreenToClient(rectTab);
	m_wndTabs.SetMessageWnd(this);

	m_wndList.MoveWindow(rectTab);
	m_wndPropList.MoveWindow(rectTab);

	OnTcnSelchangeTab(0, 0);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// �R���{ �{�b�N�X�̍쐬:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, ID_PROPERTIES_CB))
	{
		TRACE0("�v���p�e�B �R���{ �{�b�N�X���쐬�ł��܂���ł���\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}

	m_wndObjectCombo.SetFont(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));

	CRect rectCombo;
	m_wndObjectCombo.GetClientRect(&rectCombo);
	m_nComboHeight = rectCombo.Height();

	// �^�u�̍쐬
	if (!m_wndTabs.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, ID_PROPERTIES_TAB))
	{
		TRACE0("�v���p�e�B �^�u���쐬�ł��܂���ł���\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}

	m_wndTabs.InsertItem(0, _T("�ڍאݒ�"));
	m_wndTabs.InsertItem(1, _T("�ȈՐݒ�"));

	// �v���p�e�B�V�[�g�쐬�i�^�u��j
	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rectDummy, &m_wndTabs, 2))
	{
		TRACE0("�v���p�e�B �O���b�h���쐬�ł��܂���ł���\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}

	// ���X�g�̍쐬�i�^�u��j
	if (!m_wndList.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP | LVS_REPORT, rectDummy, &m_wndTabs, ID_PROPERTIES_LIST))
	{
		TRACE0("�v���p�e�B ���X�g���쐬�ł��܂���ł���\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}


	m_wndList.SetParent(this);

	DWORD dwStyle = m_wndList.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES;
	m_wndList.SetExtendedStyle(dwStyle);


	LVCOLUMN	lvc;
	lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;    // �L���t���O
	TCHAR       caption[][32] = { _T("�\��"), _T("�`�惂�[�h") };
	const int   clmNum = sizeof caption / sizeof caption[0];

	for (int c = 0; c < clmNum; ++c)
	{
		lvc.iSubItem = c;			// �T�u�A�C�e���ԍ�
		lvc.pszText = caption[c];	// ���o���e�L�X�g
		lvc.cx = 100;				// ����
		int id = m_wndList.InsertColumn(c, &lvc);
	}

#if 0
	LVITEM       lvi;
	CString      str;
	for (int r = 0; r < 10; ++r)
	{
		// �f�[�^
		lvi.lParam = (LPARAM)r;
		lvi.iItem = r;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT;
		str.Format(_T("�f�[�^%02d"), r + 1);
		lvi.pszText = str.GetBuffer();
		m_wndList.InsertItem(&lvi);

		// �`�惂�[�h
		lvi.lParam = (LPARAM)r;
		lvi.iItem = r;
		lvi.iSubItem = 1;
		lvi.mask = LVIF_TEXT;
		str.Format(_T("�`�惂�[�h%02d"), r + 1);
		lvi.pszText = str.GetBuffer();
		m_wndList.SetItem(&lvi);
	}
#endif

	InitPropList();

	AdjustLayout();

	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂�
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO:�����ɃR�}���h�X�V UI �n���h���[ �R�[�h��ǉ����܂�
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂�
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO:�����ɃR�}���h�X�V UI �n���h���[ �R�[�h��ǉ����܂�
}

void CPropertiesWnd::InitPropList()
{
	int i = 0;
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties(0);	// �v���p�e�B�ύX�㑾���\������

	//------------------------------------------------------
	// �\���ݒ�
	CMFCPropertyGridProperty* pPropView = new CMFCPropertyGridProperty(_T("�\���ݒ�"));

	// �\���^�C�v
	CMFCPropertyGridProperty* pPropViewMode = new CMFCPropertyGridProperty(_T("�\���^�C�v"), (variant_t)initPropData.ViewMode, _T("���A���^�C��/�m�����A���^�C��"), idViewMode);
	pPropViewMode->AddOption(_T("���A���^�C��"));
	pPropViewMode->AddOption(_T("�m�����A���^�C��"));
	pPropViewMode->AllowEdit(FALSE);
	pPropView->AddSubItem(pPropViewMode);

	// �J�n����
//	CCustomDlgProp* pPropViewDate = new CCustomDlgProp(_T("�J�n����"), (variant_t)_T(""), _T("�f�[�^�\���J�n����"), idViewDate);
//	pPropView->AddSubItem(pPropViewDate);
//	pPropViewDate->Enable(FALSE);
//	pPropViewDate->SetParent(this);

	m_wndPropList.AddProperty(pPropView);

	//------------------------------------------------------
	// �c���ݒ�
	CMFCPropertyGridProperty* pPropV = new CMFCPropertyGridProperty(_T("�c���ݒ�"));

	// �c���ݒ�-�\���X�P�[��(��)
	CMFCPropertyGridProperty* pPropVL = new CMFCPropertyGridProperty(_T("�\���X�P�[��(��)"));
	pPropV->AddSubItem(pPropVL);

	// �c���ݒ�-�\���X�P�[��(��)-�I�[�g�X�P�[��
	CMFCPropertyGridProperty* pPropVLA = new CMFCPropertyGridProperty(_T("�I�[�g�X�P�[��"), (variant_t)initPropData.Auto, _T("�I�[�g�X�P�[����ON/OFF"), idVLAxisAutoRange);
	pPropVLA->AddOption(_T("On"));
	pPropVLA->AddOption(_T("Off"));
	pPropVLA->AllowEdit(FALSE);
	pPropVL->AddSubItem(pPropVLA);

	// �c���ݒ�-�\���X�P�[��(��)-�ő�l
	CMFCPropertyGridProperty* pPropVLMax = new CMFCPropertyGridProperty(_T("�ő�l"), initPropData.VScalLMax, _T("�ő�l"), idVLAxisMaxRange);
//	pPropVLMax->EnableSpinControl(TRUE, -100, 100);
	pPropVLMax->Enable(FALSE);
	pPropVL->AddSubItem(pPropVLMax);

	// �c���ݒ�-�\���X�P�[��(��)-�ŏ��l
	CMFCPropertyGridProperty* pPropVLMin = new CMFCPropertyGridProperty(_T("�ŏ��l"), initPropData.VScalLMin, _T("�ŏ��l"), idVLAxisMinRange);
//	pPropVLMin->EnableSpinControl(TRUE, -100, 100);
	pPropVLMin->Enable(FALSE);
	pPropVL->AddSubItem(pPropVLMin);

	// �c���ݒ�-�\���X�P�[��(�E)
	CMFCPropertyGridProperty* pPropVR = new CMFCPropertyGridProperty(_T("�\���X�P�[��(�E)"));
	pPropV->AddSubItem(pPropVR);

	// �c���ݒ�-�\���X�P�[��(�E)-�c���ݒ�-�ő�l
	CMFCPropertyGridProperty* pPropVRMax = new CMFCPropertyGridProperty(_T("�ő�l"), initPropData.VScalRMax, _T("�ő�l"), idVRAxisMaxRange);
//	pPropVRMax->EnableSpinControl(TRUE, -100, 100);
	pPropVR->AddSubItem(pPropVRMax);

	// �c���ݒ�-�\���X�P�[��(�E)-�ŏ��l
	CMFCPropertyGridProperty* pPropVRMin = new CMFCPropertyGridProperty(_T("�ŏ��l"), initPropData.VScalRMin, _T("�ŏ��l"), idVRAxisMinRange);
//	pPropVRMin->EnableSpinControl(TRUE, -100, 100);
	pPropVR->AddSubItem(pPropVRMin);

	// �c���ݒ�-�ڐ���̊Ԋu
	CMFCPropertyGridProperty* pPropVM = new CMFCPropertyGridProperty(_T("�ڐ���̕�����"), initPropData.VSCalItvl, _T("�ڐ���̕�����"), idVAxisScaleInterval);
	pPropVM->EnableSpinControl(TRUE, AXIS_SCALE_MIN, AXIS_SCALE_MAX);
	pPropV->AddSubItem(pPropVM);

	m_wndPropList.AddProperty(pPropV);


	//------------------------------------------------------
	// �����ݒ�
	CMFCPropertyGridProperty* pPropH = new CMFCPropertyGridProperty(_T("�����ݒ�"));

	// �����ݒ�-�\���X�P�[��(��)
	CMFCPropertyGridProperty* pPropHSU = new CMFCPropertyGridProperty(_T("�\���X�P�[��(A)"), (variant_t)initPropData.HScaleU, _T("�g�����h�O���t�r���[1�̕\���X�P�[��"), idHUAxisScale);
	for (i = 0; strHScale[i] != _T(""); ++i)
	{
		if( !tblRealScaleInfo[i].flg_auto )
			pPropHSU->AddOption(strHScale[i]);
	}
	pPropHSU->AddOption(strHScale[autoscale]);
	pPropHSU->AllowEdit(FALSE);
	pPropH->AddSubItem(pPropHSU);

	// �����ݒ�-�\���X�P�[��(��)
	CMFCPropertyGridProperty* pPropHSD = new CMFCPropertyGridProperty(_T("�\���X�P�[��(B)"), (variant_t)initPropData.HScaleL, _T("�g�����h�O���t�r���[2�̕\���X�P�[��"), idHLAxisScale);
	for (i = 0; strHScale[i] != _T(""); ++i)
	{
		if( !tblRealScaleInfo[i].flg_auto )
			pPropHSD->AddOption(strHScale[i]);
	}
	pPropHSD->AddOption(strHScale[autoscale]);
	pPropHSD->AllowEdit(FALSE);
	pPropH->AddSubItem(pPropHSD);

	m_wndPropList.AddProperty(pPropH);

	//-------------------------------------------------------------------------------------------------
	// �f�[�^�ݒ�
	for (int i = 0; i < MAX_TDATA; ++i){
		// �f�[�^X
		CString str;
		str.Format(_T("�f�[�^%02d"), i + 1);
		CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(str);

		// �f�[�^�I��
		CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(_T("�f�[�^�I��"), _T(""), _T("�f�[�^�̑I��"), ((idMax - idAxisMax)*i) + idSelectItem);
		pItem->AddOption(_T(""));
		pItem->AllowEdit(FALSE);
		pGroup->AddSubItem(pItem);

		// ����
		str.Format(_T("Data%02d"), i + 1);
		pItem = new CMFCPropertyGridProperty(_T("����"), str, _T("�f�[�^����"), ((idMax - idAxisMax)*i) + idItemName);
		pGroup->AddSubItem(pItem);

		// �P��
		pItem = new CMFCPropertyGridProperty(_T("�P��"), (COleVariant)_T("100"), _T("�f�[�^�̒P��"), ((idMax - idAxisMax)*i) + idItemUnit);
//		pItem->Enable(FALSE);
		pGroup->AddSubItem(pItem);

		// �\��
		pItem = new CMFCPropertyGridProperty(_T("�\��"), (variant_t)initPropData.Display, _T("�\����ON/OFF"), ((idMax - idAxisMax)*i) + idDisplay);
		pItem->AddOption(_T("On"));
		pItem->AddOption(_T("Off"));
		pItem->AllowEdit(FALSE);
		pGroup->AddSubItem(pItem);

		// �`�惂�[�h
		pItem = new CMFCPropertyGridProperty(_T("�`�惂�[�h"), (variant_t)initPropData.DrawKind, _T("Line/Dots/L&D"), ((idMax - idAxisMax)*i) + idDrawKind);
		pItem->AddOption(_T("Line"));
		pItem->AddOption(_T("Dots"));
		pItem->AddOption(_T("L&D"));
		pItem->AllowEdit(FALSE);
		pGroup->AddSubItem(pItem);

		// �\���F
		CMFCPropertyGridColorProperty* pGColorProp = new CMFCPropertyGridColorProperty(_T("�\���F"), 0, NULL, _T("�\���F"), ((idMax - idAxisMax)*i) + idDrawColor);
		pGColorProp->EnableAutomaticButton(_T("����l"), colordef[i]);
		pGColorProp->EnableOtherButton(_T("���̑�..."));
		pGColorProp->SetColor(colordef[i]);

		pGroup->AddSubItem(pGColorProp);

		// �h�b�g�T�C�Y
		pItem = new CMFCPropertyGridProperty(_T("�h�b�g�T�C�Y"), (variant_t)initPropData.DotSize, _T("Large/Medium/Small"), ((idMax - idAxisMax)*i) + idDotSize);
		pItem->AddOption(_T("Small"));
		pItem->AddOption(_T("Medium"));
		pItem->AddOption(_T("Large"));
		pItem->Enable(false);
		pItem->AllowEdit(FALSE);
		pGroup->AddSubItem(pItem);

		// �c���X�P�[���I��
		pItem = new CMFCPropertyGridProperty(_T("�c���X�P�[���I��"), (variant_t)initPropData.SelVScale, _T("�c���̃X�P�[���I��(�� or �E)"), ((idMax - idAxisMax)*i) + idVScale);
		pItem->AddOption(_T("��"));
		pItem->AddOption(_T("�E"));
		pItem->AllowEdit(FALSE);
		pGroup->AddSubItem(pItem);

		//------------------------------------------------------
		// �R�����g
		CMFCPropertyGridProperty* pGroupCm = new CMFCPropertyGridProperty(_T("�R�����g"));

		// �R�����g�\��
		pItem = new CMFCPropertyGridProperty(_T("�\��"), (variant_t)_T("Off"), _T("�R�����g��ON/OFF"), ((idMax - idAxisMax)*i) + idCommentEnable);
		pItem->AddOption(_T("On"));
		pItem->AddOption(_T("Off"));
		pItem->AllowEdit(FALSE);
		pGroupCm->AddSubItem(pItem);

		// �R�����g
		pItem = new CMFCPropertyGridProperty(_T("�R�����g"), (COleVariant)_T(""), _T("�O���t��̃f�[�^�R�����g"), ((idMax - idAxisMax)*i) + idComment);
		pGroupCm->AddSubItem(pItem);

		pGroupCm->Expand(FALSE);
		pGroup->AddSubItem(pGroupCm);

		//------------------------------------------------------
		// ���~�b�g
		CMFCPropertyGridProperty* pGroupAl = new CMFCPropertyGridProperty(_T("���~�b�g"));

		// ���[�j���O������\�����Ď�
		pItem = new CMFCPropertyGridProperty(_T("���[�j���O��� - �\��"), (variant_t)_T("Off"), _T("���[�j���O������\�����Ď���ON/OFF"), ((idMax - idAxisMax)*i) + idLimitWarningUpperEnable);
		pItem->AddOption(_T("On"));
		pItem->AddOption(_T("Off"));
		pItem->AllowEdit(FALSE);
		pGroupAl->AddSubItem(pItem);

		// ���[�j���O���
		pItem = new CMFCPropertyGridProperty(_T("���[�j���O���"), initPropData.LimitWarningUpper, _T("���[�j���O���"), ((idMax - idAxisMax)*i) + idLimitWarningUpper);
		pItem->Enable(FALSE);
		pGroupAl->AddSubItem(pItem);

		// ���[�j���O�������\�����Ď�
		pItem = new CMFCPropertyGridProperty(_T("���[�j���O���� - �\��"), (variant_t)_T("Off"), _T("���[�j���O�������\�����Ď���ON/OFF"), ((idMax - idAxisMax)*i) + idLimitWarningLowerEnable);
		pItem->AddOption(_T("On"));
		pItem->AddOption(_T("Off"));
		pItem->AllowEdit(FALSE);
		pGroupAl->AddSubItem(pItem);

		// ���[�j���O����
		pItem = new CMFCPropertyGridProperty(_T("���[�j���O����"), initPropData.LimitWarningLower, _T("���[�j���O����"), ((idMax - idAxisMax)*i) + idLimitWarningLower);
		pItem->Enable(FALSE);
		pGroupAl->AddSubItem(pItem);

		// �A���[��������\�����Ď�
		pItem = new CMFCPropertyGridProperty(_T("�A���[����� - �\��"), (variant_t)_T("Off"), _T("�A���[��������\�����Ď���ON/OFF"), ((idMax - idAxisMax)*i) + idLimitAlarmUpperEnable);
		pItem->AddOption(_T("On"));
		pItem->AddOption(_T("Off"));
		pItem->AllowEdit(FALSE);
		pGroupAl->AddSubItem(pItem);

		// �A���[�����
		pItem = new CMFCPropertyGridProperty(_T("�A���[�����"), initPropData.LimitAlarmUpper, _T("�A���[�����"), ((idMax - idAxisMax)*i) + idLimitAlarmUpper);
		pItem->Enable(FALSE);
		pGroupAl->AddSubItem(pItem);

		// �A���[���������\�����Ď�
		pItem = new CMFCPropertyGridProperty(_T("�A���[������ - �\��"), (variant_t)_T("Off"), _T("�A���[���������\�����Ď���ON/OFF"), ((idMax - idAxisMax)*i) + idLimitAlarmLowerEnable);
		pItem->AddOption(_T("On"));
		pItem->AddOption(_T("Off"));
		pItem->AllowEdit(FALSE);
		pGroupAl->AddSubItem(pItem);

		// �A���[������
		pItem = new CMFCPropertyGridProperty(_T("�A���[������"), initPropData.LimitAlarmLower, _T("�A���[������"), ((idMax - idAxisMax)*i) + idLimitAlarmLower);
		pItem->Enable(FALSE);
		pGroupAl->AddSubItem(pItem);

		pGroupAl->Expand(FALSE);

		pGroup->AddSubItem(pGroupAl);


		m_wndPropList.AddProperty(pGroup);
	}
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
	m_wndTabs.SetFont(&m_fntPropList);
	m_wndList.SetFont(&m_fntPropList);
}

void CPropertiesWnd::OnComboBoxSelChange()
{
	int pos = -1;
	CString name;
	m_wndObjectCombo.GetWindowTextW(name);
	CPropertiesData prop;
	for (int i = 0; i < 10; ++i)
	{
		prop = theApp.GetPropertiesData(i);

		if (name == prop.wndName)
		{
			pos = prop.wndIdx;
			break;
		}
	}

	if (pos == -1)
		return;

	// �I�����ꂽView���A�N�e�B�u�ɂ���
	((CMDIChildWnd*)prop.wnd)->MDIActivate();
}

void CPropertiesWnd::ChangeViewMode()
{
	bool	bPropetyChange = true;

	// �v���p�e�B�f�[�^�擾
	CPropertiesData&	prop = theApp.GetPropertiesData(m_ActiveView);

	// �\���ݒ�
	CMFCPropertyGridProperty	*pView = m_wndPropList.GetProperty(idViewGr);

	// �\���ݒ�-�\���^�C�v
	CMFCPropertyGridProperty	*pViewMode = pView->GetSubItem(idViewGrMode);
	CString	str = (CString(pViewMode->GetValue().bstrVal).Compare(_T("���A���^�C��")) == 0) ? _T("�m�����A���^�C��") : _T("���A���^�C��");

	// �\���^�C�v��؂�ւ�
	pViewMode->SetValue(str);
	SetViewMode(prop, str);

	if( str.Compare(_T("�m�����A���^�C��")) == 0 )
	{
		if( ((CMainFrame *)theApp.m_pMainWnd)->GetOnline() )
			bPropetyChange = false;
	}
		
	// �v���p�e�B�f�[�^�Z�b�g
	theApp.SetPropertiesData(m_ActiveView, prop);

	// �f�[�^�擾�Ώۃt���O��ݒ�
	theApp.GetDataCollection().SetSelectDataValid();

	// �ύX��ʒm
	if( bPropetyChange )
		theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 0, (LPARAM)m_ActiveView);
}

void CPropertiesWnd::SetViewMode(CPropertiesData& prop, CString str)
{
	if (str.Compare(_T("���A���^�C��")) == 0)
	{
		prop.isRealTime = true;

		// ��̓f�[�^���폜
		theApp.GetDataCollection().DeleteAnalysisData(m_ActiveView);
	}
	else
	{
		prop.isRealTime= false;

		if( ((CMainFrame *)theApp.m_pMainWnd)->GetOnline() )
		{
			// ��͈ڍs�t���O���Z�b�g
			theApp.SetAnalysisMigration(m_ActiveView);

			// �f�[�^���擾����Ă��Ȃ����ł��m�����A���^�C���Ɉڍs���邽�߃��b�Z�[�W�𔭍s
			(theApp.m_pMainWnd)->PostMessage(eMessage_ThreadNormal, 0/*�擾������Min/Max���X�V*/, 0);
		}
		else
		{
			// ��̓f�[�^���쐬
			if( (theApp.GetRealTimeStart() != CTime(0)) && (theApp.GetRealTimeLast() != CTime(0)) )
				theApp.GetDataCollection().CreateAnalysisDataFromRealTime(m_ActiveView);
		}
	}
}

LRESULT CPropertiesWnd::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	bool	bPropetyChange = true;

	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;

	CString		str = _T("");
	COLORREF	color = 0;
	double		dval = 0;
	long		lval = 0;
	int			ival = 0;

	int pos = 0;
	int data = (int)pProp->GetData();

	if (data > idAxisMax)
	{
		pos = ((int)pProp->GetData() - idAxisMax) / (idMax - idAxisMax);
		data = (((int)pProp->GetData() - idAxisMax) % (idMax - idAxisMax)) + idAxisMax;
	}

	CString item = pProp->GetName();
	CPropertiesData prop = theApp.GetPropertiesData(m_ActiveView);

	switch (data){
	case idViewMode:	// �\���^�C�v
		str = pProp->GetValue().bstrVal;

		SetViewMode(prop, str);

		if( str.Compare(_T("�m�����A���^�C��")) == 0 )
		{
			if( ((CMainFrame *)theApp.m_pMainWnd)->GetOnline() )
				bPropetyChange = false;
		}
		break;

//	case idViewDate:	// �J�n����
//	{
//		str = pProp->GetValue().bstrVal;
//
//		COleDateTime oleTime;
//		oleTime.ParseDateTime(str);
//		CTime t(oleTime.GetYear(), oleTime.GetMonth(), oleTime.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());
//		prop.startTime = t;
//	}
//		break;

	case idVLAxisAutoRange:	// �c�����I�[�g�X�P�[��
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("On")) == 0)
		{
			prop.trandVScale.Auto = true;

			// �ő�l�A�ŏ��l��񊈐��ɂ���B
			CMFCPropertyGridProperty* pMax = pProp->GetParent()->GetSubItem(idVAxisGrScalLGrMax);
			pMax->Enable(FALSE);
			CMFCPropertyGridProperty* pMin = pProp->GetParent()->GetSubItem(idVAxisGrScalLGrMin);
			pMin->Enable(FALSE);

			m_AutoScale[m_ActiveView] = TRUE;
		}
		else
		{
			prop.trandVScale.Auto = false;

			// �ő�l�A�ŏ��l�������ɂ���B
			CMFCPropertyGridProperty* pMax = pProp->GetParent()->GetSubItem(idVAxisGrScalLGrMax);
			pMax->Enable(TRUE);
			CMFCPropertyGridProperty* pMin = pProp->GetParent()->GetSubItem(idVAxisGrScalLGrMin);
			pMin->Enable(TRUE);

			m_AutoScale[m_ActiveView] = FALSE;
		}
		break;

	case idVLAxisMaxRange:	// �c�����ő�l
		dval = pProp->GetValue().dblVal;
		if (dval >= AXIS_MINMAX_RANGE)
		{
			dval = AXIS_MINMAX_VALUE;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		else if (dval <= -AXIS_MINMAX_RANGE)
		{
			dval = -AXIS_MINMAX_VALUE;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		if( dval <= prop.trandVScale.VScalLMin )
		{
			dval = prop.trandVScale.VScalLMax;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		prop.trandVScale.VScalLMax = dval;
		break;

	case idVLAxisMinRange:	// �c�����ŏ��l
		dval = pProp->GetValue().dblVal;
		if (dval >= AXIS_MINMAX_RANGE)
		{
			dval = AXIS_MINMAX_VALUE;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		else if (dval <= -AXIS_MINMAX_RANGE)
		{
			dval = -AXIS_MINMAX_VALUE;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		if( dval >= prop.trandVScale.VScalLMax )
		{
			dval = prop.trandVScale.VScalLMin;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		prop.trandVScale.VScalLMin = dval;
		break;

	case idVRAxisMaxRange:	// �c���E�ő�l
		dval = pProp->GetValue().dblVal;
		if (dval > AXIS_MINMAX_RANGE)
		{
			dval = AXIS_MINMAX_VALUE;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		else if (dval <= -AXIS_MINMAX_RANGE)
		{
			dval = -AXIS_MINMAX_VALUE;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		if( dval <= prop.trandVScale.VScalRMin )
		{
			dval = prop.trandVScale.VScalRMax;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		prop.trandVScale.VScalRMax = dval;
		break;

	case idVRAxisMinRange:	// �c���E�ŏ��l
		dval = pProp->GetValue().dblVal;
		if (dval >= AXIS_MINMAX_RANGE)
		{
			dval = AXIS_MINMAX_VALUE;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		else if (dval <= -AXIS_MINMAX_RANGE)
		{
			dval = -AXIS_MINMAX_VALUE;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		if( dval >= prop.trandVScale.VScalRMax )
		{
			dval = prop.trandVScale.VScalRMin;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		prop.trandVScale.VScalRMin = dval;
		break;

	case idVAxisScaleInterval:	// �c���ڐ���̊Ԋu
		lval = pProp->GetValue().lVal;
		if( lval < AXIS_SCALE_MIN )
		{
			lval = AXIS_SCALE_MIN;
			pProp->SetValue(lval);
			pProp->Redraw();
		}
		else if( lval > AXIS_SCALE_MAX )
		{
			lval = AXIS_SCALE_MAX;
			pProp->SetValue(lval);
			pProp->Redraw();
		}
		prop.trandVScale.VSCalItvl = lval;
		break;

	case idHUAxisScale:		// ������X�P�[��
		str = pProp->GetValue().bstrVal;
		prop.trandHScale[0] = (HScale)prop.getHScal(str);
		break;

	case idHLAxisScale:		// �������X�P�[��
		str = pProp->GetValue().bstrVal;
		prop.trandHScale[1] = (HScale)prop.getHScal(str);
		break;

	case idSelectItem:	// �f�[�^�I��
		str = pProp->GetValue().bstrVal;
		//prop.item[pos].dataName = str;
		swprintf_s(prop.item[pos].dataName, CPropertiesDataItem::SIZE_DATANAME, _T("%s"), (LPCTSTR)(str.Left(CPropertiesDataItem::SIZE_DATANAME - 1)));
		pProp->SetValue(prop.item[pos].dataName);
		pProp->Redraw();

#if 0
		CGraphMutex::Lock(eRealData);

		// ���k�f�[�^���č쐬
		theApp.GetDataCollection().CompSpaceData(theApp.GetDataCollection().ConvertNameToId(str), 0, true);

		CGraphMutex::Unlock(eRealData);
#endif

		// Auto�X�P�[�����N���A
		theApp.m_pMainWnd->PostMessage(eMessage_AutoClear, 1, m_ActiveView);

		break;

	case idItemName:	// ����
		str = pProp->GetValue().bstrVal;
		swprintf_s(prop.item[pos].dataGName, CPropertiesDataItem::SIZE_DATAGNAME, _T("%s"), (LPCTSTR)(str.Left(CPropertiesDataItem::SIZE_DATAGNAME - 1)));
		pProp->SetValue(prop.item[pos].dataGName);
		pProp->Redraw();
		break;

	case idItemUnit:	// �P��
		str = pProp->GetValue().bstrVal;
		swprintf_s(prop.item[pos].dataUnit, CPropertiesDataItem::SIZE_DATAUNIT, _T("%s"), (LPCTSTR)(str.Left(CPropertiesDataItem::SIZE_DATAUNIT - 1)));
		pProp->SetValue(prop.item[pos].dataUnit);
		pProp->Redraw();
		break;

	case idDisplay:	// �\��
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("On")) == 0)
		{
			prop.item[pos].isViewMode = true;
		}
		else
		{
			prop.item[pos].isViewMode = false;
		}
		break;

	case idDrawKind:	// �`�惂�[�h
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("Line")) == 0)
		{
			prop.item[pos].drawMode = Line;

			// �h�b�g�T�C�Y �ύX�s��
			CMFCPropertyGridProperty* pDotProp = pProp->GetParent()->GetSubItem(idDataGrDotSiz);
			pDotProp->Enable(FALSE);
		}
		else if (str.Compare(_T("Dots")) == 0)
		{
			prop.item[pos].drawMode = Dots;

			// �h�b�g�T�C�Y �ύX��
			CMFCPropertyGridProperty* pDotProp = pProp->GetParent()->GetSubItem(idDataGrDotSiz);
			pDotProp->Enable(TRUE);
		}
		else if (str.Compare(_T("L&D")) == 0)
		{
			prop.item[pos].drawMode = LineAndDots;

			// �h�b�g�T�C�Y �ύX��
			CMFCPropertyGridProperty* pDotProp = pProp->GetParent()->GetSubItem(idDataGrDotSiz);
			pDotProp->Enable(TRUE);
		}

		break;

	case idDrawColor:	// �\���F
	{
		CMFCPropertyGridColorProperty* pColor = (CMFCPropertyGridColorProperty*)pProp;
		color = pColor->GetColor();
		if (color == -1)
		{
			prop.item[pos].color = colordef[pos];
			pColor->SetColor(prop.item[pos].color);
		}
		else
		{
			prop.item[pos].color = color;
		}
	}
	break;

	case idDotSize:		// �h�b�g�T�C�Y
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("Small")) == 0)
		{
			prop.item[pos].dotSize = Small;
		}
		else if (str.Compare(_T("Medium")) == 0)
		{
			prop.item[pos].dotSize = Medium;
		}
		else if (str.Compare(_T("Large")) == 0)
		{
			prop.item[pos].dotSize = Large;
		}
		break;

	case idVScale:		// �����X�P�[���I��
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("��")) == 0)
		{
			prop.item[pos].vscaleKind = LScale;
		}
		else
		{
			prop.item[pos].vscaleKind = RScale;
		}
		break;
		
	case idCommentEnable:	// �R�����g�\��
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("On")) == 0)
		{
			prop.item[pos].isComment = true;
		}
		else
		{
			prop.item[pos].isComment = false;
		}
		break;

	case idComment:			// �R�����g
		str = pProp->GetValue().bstrVal;
		prop.item[pos].comment = str;
		break;

	case idLimitWarningUpperEnable:		// ���[�j���O��� - �\��
		str = pProp->GetValue().bstrVal;
		prop.item[pos].isLimitWarningUpper = (str.Compare(_T("On")) == 0) ? true : false;
		pProp->GetParent()->GetSubItem(idDataGrLimitWarningUpper)->Enable(prop.item[pos].isLimitWarningUpper ? TRUE : FALSE);
		break;

	case idLimitWarningLowerEnable:		// ���[�j���O���� - �\��
		str = pProp->GetValue().bstrVal;
		prop.item[pos].isLimitWarningLower = (str.Compare(_T("On")) == 0) ? true : false;
		pProp->GetParent()->GetSubItem(idDataGrLimitWarningLower)->Enable(prop.item[pos].isLimitWarningLower ? TRUE : FALSE);
		break;

	case idLimitAlarmUpperEnable:		// �A���[����� - �\��
		str = pProp->GetValue().bstrVal;
		prop.item[pos].isLimitAlarmUpper = (str.Compare(_T("On")) == 0) ? true : false;
		pProp->GetParent()->GetSubItem(idDataGrLimitAlarmUpper)->Enable(prop.item[pos].isLimitAlarmUpper ? TRUE : FALSE);
		break;

	case idLimitAlarmLowerEnable:		// �A���[������ - �\��
		str = pProp->GetValue().bstrVal;
		prop.item[pos].isLimitAlarmLower = (str.Compare(_T("On")) == 0) ? true : false;
		pProp->GetParent()->GetSubItem(idDataGrLimitAlarmLower)->Enable(prop.item[pos].isLimitAlarmLower ? TRUE : FALSE);
		break;

	case idLimitWarningUpper:	// ���[�j���O���
		dval = pProp->GetValue().dblVal;
		if (dval > LIMIT_MINMAX_RANGE)
		{
			dval = LIMIT_MINMAX_VALUE;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		else if (dval < -LIMIT_MINMAX_RANGE)
		{
			dval = -LIMIT_MINMAX_VALUE;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		if( dval <= prop.item[pos].limitWarningLower )
		{
			dval = prop.item[pos].limitWarningUpper;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		prop.item[pos].limitWarningUpper = dval;
		break;

	case idLimitWarningLower:	// ���[�j���O����
		dval = pProp->GetValue().dblVal;
		if (dval > LIMIT_MINMAX_RANGE)
		{
			dval = LIMIT_MINMAX_VALUE;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		else if (dval < -LIMIT_MINMAX_RANGE)
		{
			dval = -LIMIT_MINMAX_VALUE;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		if( dval >= prop.item[pos].limitWarningUpper )
		{
			dval = prop.item[pos].limitWarningLower;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		prop.item[pos].limitWarningLower = dval;
		break;

	case idLimitAlarmUpper:	// �A���[�����
		dval = pProp->GetValue().dblVal;
		if (dval > LIMIT_MINMAX_RANGE)
		{
			dval = LIMIT_MINMAX_VALUE;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		else if (dval < -LIMIT_MINMAX_RANGE)
		{
			dval = -LIMIT_MINMAX_VALUE;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		if( dval <= prop.item[pos].limitAlarmLower )
		{
			dval = prop.item[pos].limitAlarmUpper;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		prop.item[pos].limitAlarmUpper = dval;
		break;

	case idLimitAlarmLower:	// �A���[������
		dval = pProp->GetValue().dblVal;
		if (dval > LIMIT_MINMAX_RANGE)
		{
			dval = LIMIT_MINMAX_VALUE;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		else if (dval < -LIMIT_MINMAX_RANGE)
		{
			dval = -LIMIT_MINMAX_VALUE;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		if( dval >= prop.item[pos].limitAlarmUpper )
		{
			dval = prop.item[pos].limitAlarmLower;
			pProp->SetValue(dval);
			pProp->Redraw();
		}
		prop.item[pos].limitAlarmLower = dval;
		break;
	}

		
	// �v���p�e�B�f�[�^�Z�b�g
	theApp.SetPropertiesData(m_ActiveView, prop);

	// �f�[�^�擾�Ώۃt���O��ݒ�
	theApp.GetDataCollection().SetSelectDataValid();

	// �ύX��ʒm
	if( bPropetyChange )
		theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 0, (LPARAM)m_ActiveView);

	return 0;
}

/*============================================================================*/
/*! �v���p�e�B�E�B���h�E�f�[�^�V�K�쐬

-# �O���t�E�B���h�E���쐬���ꂽ���ɁA�v���p�e�B�f�[�^���쐬����B

@param
@retval

*/
/*============================================================================*/
void CPropertiesWnd::SetNewPropertyData(int pos, LPARAM wnd)
{
	// �A�N�e�B�u�E�B���h�E�ݒ�
	m_ActiveView = pos;

	// �E�B���h�E���擾
	CString name;
	((CMDIChildWnd*)wnd)->GetWindowTextW(name);

	// �h��ʑI���h�p���X�g�X�V
	m_wndObjectCombo.AddString(name);
	m_wndObjectCombo.SelectString(0, name);

#if 0
#ifdef _NOCOMM
	CPropertiesData dummy = theApp.GetPropertiesData(m_ActiveView);
#endif
	prop.wndIdx = pos;					// �E�B���h�[�ԍ�
	prop.wndName = name;				// �E�B���h�[����
	prop.wnd = wnd;

	prop.isRealTime = true;									// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
	prop.trandVScale.Auto = true;							// �c���� �I�[�g�X�P�[��
	m_AutoScale[pos] = TRUE;
	prop.trandVScale.VScalLMax = initPropData.VScalLMax;	// �c���� �ő�l
	prop.trandVScale.VScalLMin = initPropData.VScalLMin;	// �c���� �ŏ��l
	prop.trandVScale.VScalRMax = initPropData.VScalRMax;	// �c���E �ő�l
	prop.trandVScale.VScalRMin = initPropData.VScalRMin;	// �c���E �ŏ��l
	prop.trandVScale.VSCalItvl = initPropData.VSCalItvl;	// �c���ڐ���̊Ԋu

	prop.trandHScale[0] = h24;			// �����X�P�[��(��)
	prop.trandHScale[1] = m60;			// �����X�P�[��(��)

	CString str = _T("");
	for (int i = 0; i < MAX_TDATA; ++i)
	{
#ifdef _NOCOMM
		prop.dataName[i] = dummy.dataName[i];		// �f�[�^����
#else
		prop.dataName[i] = _T("");		// �f�[�^����
#endif
		str.Format(_T("Data%02d"), i + 1);
		prop.dataGName[i] = str;		// �f�[�^����
		prop.dataUnit[i] = _T("");		// �f�[�^�P��
		prop.isViewMode[i] = true;		// �\��On/Off
		prop.drawMode[i] = Line;		// �`�惂�[�h�iLine�ADots�AL&D�j
		prop.color[i] = colordef[i];	// �\���F
		prop.dotSize[i] = Small;		// �h�b�g�T�C�Y
		prop.vscaleKind[i] = LScale;	// �c���X�P�[���I��
	}
#else
	CPropertiesData& prop = theApp.GetPropertiesData(pos);
	prop.wndIdx = pos;					// �E�B���h�[�ԍ�
	prop.wndName.Empty();
	prop.wndName = name;				// �E�B���h�[����
	prop.wnd = wnd;

	m_AutoScale[pos] = prop.trandVScale.Auto ? TRUE : FALSE;
#endif

	// �v���p�e�B�f�[�^�Z�b�g
	//theApp.SetPropertiesData(pos, prop);

	// �v���p�e�B�ύX
	ChangePropaties();

	// �ȈՃ��X�g��ύX
	if( m_wndTabs.GetCurSel() == 1 )
		OnTcnSelchangeTab(0, 0);

	// �ύX��ʒm
	theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 1, (LPARAM)m_ActiveView);
	return;
}


/*============================================================================*/
/*! �v���p�e�B�\�����A�N�e�B�u�E�B���h�E�p�ɕύX

-# �\������Ă���E�B���h�E���ύX���ꂽ��A�v���p�e�B�[�\����ύX����B

@param
@retval

*/
/*============================================================================*/
void CPropertiesWnd::ChangeView(int pos)
{
	CPropertiesData prop = theApp.GetPropertiesData(pos);
	m_wndObjectCombo.SelectString(0, prop.wndName);
	m_ActiveView = pos;

	// �v���p�e�B�ύX
	ChangePropaties();

	// �ȈՃ��X�g��ύX
	if( m_wndTabs.GetCurSel() == 1 )
		OnTcnSelchangeTab(0, 0);
}


/*============================================================================*/
/*! �v���p�e�B�E�B���h�E�f�[�^�폜

-# �E�B���h�E���I�����ꂽ���ɁA�v���p�e�B��ʑI������폜����B

@param
@retval

*/
/*============================================================================*/
void CPropertiesWnd::CloseView(int pos)
{
	// �h��ʑI���h�p���X�g�X�V
	CPropertiesData prop = theApp.GetPropertiesData(pos);
	m_wndObjectCombo.DeleteString(m_wndObjectCombo.FindString(0, prop.wndName));
}

/*============================================================================*/
/*! ��ʑI��ύX

-# ��ʑI�����ύX���ꂽ���ɁA�v���p�e�B�����̉�ʗp�ɍX�V����B

@param
@retval

*/
/*============================================================================*/
void CPropertiesWnd::ChangePropaties(BOOL bUpdate)
{
	CString str = _T("");

	// �v���p�e�B�f�[�^�擾
	CPropertiesData& prop = theApp.GetPropertiesData(m_ActiveView);

	// �\���ݒ�
	CMFCPropertyGridProperty* pView = m_wndPropList.GetProperty(idViewGr);

	// �\���ݒ�-�\���^�C�v
	CMFCPropertyGridProperty* pViewMode = pView->GetSubItem(idViewGrMode);
	str = pViewMode->GetName();
	if (prop.isRealTime)
	{
		pViewMode->SetValue(_T("���A���^�C��"));
	}
	else
	{
		pViewMode->SetValue(_T("�m�����A���^�C��"));
	}

	// �c���ݒ�
	CMFCPropertyGridProperty* pVAxis = m_wndPropList.GetProperty(idVAxisGr);

	// �c���ݒ�-�\���X�P�[��(��)-�I�[�g�X�P�[��
	CMFCPropertyGridProperty* pVAxisLAuto = pVAxis->GetSubItem(idVAxisGrScalLGr)->GetSubItem(idVAxisGrScalLGrAuto);
	if (prop.trandVScale.Auto)
	{
		pVAxisLAuto->SetValue(_T("On"));
	}
	else
	{
		pVAxisLAuto->SetValue(_T("Off"));
	}

	// �c���ݒ�-�\���X�P�[��(��)-�ő�l
	CMFCPropertyGridProperty* pVAxisLMax = pVAxis->GetSubItem(idVAxisGrScalLGr)->GetSubItem(idVAxisGrScalLGrMax);
	pVAxisLMax->SetValue((double)prop.trandVScale.VScalLMax);
	pVAxisLMax->Enable(prop.trandVScale.Auto ? FALSE : TRUE);

	// �c���ݒ�-�\���X�P�[��(��)-�ŏ��l
	CMFCPropertyGridProperty* pVAxisLMin = pVAxis->GetSubItem(idVAxisGrScalLGr)->GetSubItem(idVAxisGrScalLGrMin);
	pVAxisLMin->SetValue((double)prop.trandVScale.VScalLMin);
	pVAxisLMin->Enable(prop.trandVScale.Auto ? FALSE : TRUE);

	// �c���ݒ�-�\���X�P�[��(�E)-�ő�l
	CMFCPropertyGridProperty* pVAxisRMax = pVAxis->GetSubItem(idVAxisGrScalRGr)->GetSubItem(idVAxisGrScalRGrMax);
	pVAxisRMax->SetValue((double)prop.trandVScale.VScalRMax);

	// �c���ݒ�-�\���X�P�[��(�E)-�ŏ��l
	CMFCPropertyGridProperty* pVAxisRMin = pVAxis->GetSubItem(idVAxisGrScalRGr)->GetSubItem(idVAxisGrScalRGrMin);
	pVAxisRMin->SetValue((double)prop.trandVScale.VScalRMin);

	// �c���ݒ�-�ڐ���̊Ԋu
	CMFCPropertyGridProperty* pVAxisScaleItvl = pVAxis->GetSubItem(idVAxisGrScalInvl);
	pVAxisScaleItvl->SetValue((long)prop.trandVScale.VSCalItvl);


	// �����ݒ�
	CMFCPropertyGridProperty* pHAxis = m_wndPropList.GetProperty(idHAxisGr);

	// �����ݒ�-�\���X�P�[��(��)
	CMFCPropertyGridProperty* pHAxisScaleU = pHAxis->GetSubItem(idHAxisGrScaleU);
	pHAxisScaleU->SetValue((variant_t)(strHScale[prop.trandHScale[0]]));

	// �����ݒ�-�\���X�P�[��(��)
	CMFCPropertyGridProperty* pHAxisScaleL = pHAxis->GetSubItem(idHAxisGrScaleL);
	pHAxisScaleL->SetValue((variant_t)(strHScale[prop.trandHScale[1]]));


	// �f�[�^

	for (int i = 0; i < MAX_TDATA; ++i)
	{
		CMFCPropertyGridProperty* pData = m_wndPropList.GetProperty(idDataGr + i);

		// �f�[�^����
		CMFCPropertyGridProperty* pItem = pData->GetSubItem(idDataGrDataSelect);
		pItem->SetValue(prop.item[i].dataName);
		str = pItem->GetName();

		// �f�[�^���́i�O���t�p�j
		pItem = pData->GetSubItem(idDataGrName);
		pItem->SetValue(prop.item[i].dataGName);
		str = pItem->GetName();

		// �P��
		pItem = pData->GetSubItem(idDataGrItemUnit);
		pItem->SetValue(prop.item[i].dataUnit);
		str = pItem->GetName();
			
		// �\��
		pItem = pData->GetSubItem(idDataGrDisplay);
		str = pItem->GetName();
		if (prop.item[i].isViewMode)
		{
			pItem->SetValue(_T("On"));
		}
		else
		{
			pItem->SetValue(_T("Off"));
		}

		// �`�惂�[�h
		pItem = pData->GetSubItem(idDataGrDrawKind);
		str = pItem->GetName();
		if (prop.item[i].drawMode == Line)
		{
			pItem->SetValue(_T("Line"));
		}
		else if (prop.item[i].drawMode == Dots)
		{
			pItem->SetValue(_T("Dots"));
		}
		else if (prop.item[i].drawMode == LineAndDots)
		{
			pItem->SetValue(_T("L&D"));
		}

		// �\���F
		CMFCPropertyGridColorProperty* pGColorProp = (CMFCPropertyGridColorProperty*)pData->GetSubItem(idDataGrColor);
		pGColorProp->SetColor(prop.item[i].color);
		str = pGColorProp->GetName();

		// �h�b�g�T�C�Y
		pItem = pData->GetSubItem(idDataGrDotSiz);
		str = pItem->GetName();
		if (prop.item[i].dotSize == Small)
		{
			pItem->SetValue(_T("Small"));
		}
		else if (prop.item[i].dotSize == Medium)
		{
			pItem->SetValue(_T("Medium"));
		}
		else if (prop.item[i].dotSize == Large)
		{
			pItem->SetValue(_T("Large"));
		}

		if (prop.item[i].drawMode == Line)
		{
			// �񊈐�
			pItem->Enable(FALSE);
		}
		else
		{
			// ����
			pItem->Enable(TRUE);
		}

		// �c���X�P�[���I��
		pItem = pData->GetSubItem(idDataGrSelScale);
		str = pItem->GetName();
		if (prop.item[i].vscaleKind == LScale)
		{
			pItem->SetValue(_T("��"));
		}
		else
		{
			pItem->SetValue(_T("�E"));
		}

		CMFCPropertyGridProperty* pComment = pData->GetSubItem(idDataGrComment);

		// �R�����g�\��
		pItem = pComment->GetSubItem(idDataGrCommentEnable);
		if (prop.item[i].isComment)
		{
			pItem->SetValue(_T("On"));
		}
		else
		{
			pItem->SetValue(_T("Off"));
		}

		// �R�����g
		pItem = pComment->GetSubItem(idDataGrCommentStr);
		str = pItem->GetName();
		pItem->SetValue(prop.item[i].comment);

		CMFCPropertyGridProperty* pLimit = pData->GetSubItem(idDataGrLimit);

		// ���[�j���O��� - �\��
		pItem = pLimit->GetSubItem(idDataGrLimitWarningUpperEnable);
		if (prop.item[i].isLimitWarningUpper)
		{
			pItem->SetValue(_T("On"));
		}
		else
		{
			pItem->SetValue(_T("Off"));
		}

		// ���[�j���O���
		pItem = pLimit->GetSubItem(idDataGrLimitWarningUpper);
		pItem->SetValue((double)prop.item[i].limitWarningUpper);
		pItem->Enable(prop.item[i].isLimitWarningUpper ? TRUE : FALSE);

		// ���[�j���O���� - �\��
		pItem = pLimit->GetSubItem(idDataGrLimitWarningLowerEnable);
		if (prop.item[i].isLimitWarningLower)
		{
			pItem->SetValue(_T("On"));
		}
		else
		{
			pItem->SetValue(_T("Off"));
		}

		// ���[�j���O����
		pItem = pLimit->GetSubItem(idDataGrLimitWarningLower);
		pItem->SetValue((double)prop.item[i].limitWarningLower);
		pItem->Enable(prop.item[i].isLimitWarningLower ? TRUE : FALSE);

		// �A���[����� - �\��
		pItem = pLimit->GetSubItem(idDataGrLimitAlarmUpperEnable);
		if (prop.item[i].isLimitAlarmUpper)
		{
			pItem->SetValue(_T("On"));
		}
		else
		{
			pItem->SetValue(_T("Off"));
		}

		// �A���[�����
		pItem = pLimit->GetSubItem(idDataGrLimitAlarmUpper);
		pItem->SetValue((double)prop.item[i].limitAlarmUpper);
		pItem->Enable(prop.item[i].isLimitAlarmUpper ? TRUE : FALSE);

		// �A���[������ - �\��
		pItem = pLimit->GetSubItem(idDataGrLimitAlarmLowerEnable);
		if (prop.item[i].isLimitAlarmLower)
		{
			pItem->SetValue(_T("On"));
		}
		else
		{
			pItem->SetValue(_T("Off"));
		}

		// �A���[������
		pItem = pLimit->GetSubItem(idDataGrLimitAlarmLower);
		pItem->SetValue((double)prop.item[i].limitAlarmLower);
		pItem->Enable(prop.item[i].isLimitAlarmLower ? TRUE : FALSE);
	}

	// �v���p�e�B�f�[�^�Z�b�g
	if (bUpdate)
	{
		theApp.SetPropertiesData(m_ActiveView, prop);
	}
}

/*============================================================================*/
/*! �f�[�^�I��p���X�g�X�V

-# �f�[�^�I��p���X�g���X�V����B

@param
@retval

*/
/*============================================================================*/
void CPropertiesWnd::UpdatePropatyDataList(BOOL bDataName/* = TRUE*/, BOOL bAutoAssign/* = TRUE*/)
{
	vector<stSelectData>& data = theApp.GetDataCollection().GetSelectDataList();
	vector<stSelectData>::iterator itr;

	m_DataNameList.RemoveAll();
	for (auto itr = data.begin(); itr != data.end(); ++itr)
	{
		stSelectData st = (stSelectData)*itr;
		m_DataNameList.AddTail(st.name);
	}


	for (int i = 0; i < MAX_TDATA; ++i)
	{
		// �f�[�^�I���R���g���[���擾
		CMFCPropertyGridProperty* pData = m_wndPropList.GetProperty(idDataGr + i);
		CMFCPropertyGridProperty* pItem = pData->GetSubItem(idDataGrDataSelect);

		// �R���{�{�b�N�X������
		CString sName = _T("");
		sName = pItem->GetValue().bstrVal;
		pItem->RemoveAllOptions();
		pItem->AddOption(_T(""));

		BOOL bFind = FALSE;
		POSITION pos = m_DataNameList.GetHeadPosition();
		while (pos != NULL)
		{
			// �f�[�^�I�����X�g�č\�z
			CString name = m_DataNameList.GetNext(pos);
			pItem->AddOption(name);

			if ( !bFind && (name == sName) )
			{
				bFind = TRUE;
			}
		}

		// ���ݑI�𒆂̃f�[�^�����X�g�ɖ����̂ŁA�\�����폜����B
		if ( !bFind )
		{
			pItem->SetValue(_T(""));
		}
	}

#ifdef _NOCOMM
	if( bDataName )
	{
		// �v���p�e�B�f�[�^�擾
		if( bAutoAssign )
		{
			// �v���p�e�B�Ƀf�[�^�o�^�̊Ď��f�[�^����������U��
			for (int i = 0; i < MAX_VIEW; ++i)
			{
				CPropertiesData prop = theApp.GetPropertiesData(i);
				for (int n = 0; n < MAX_TDATA; ++n)
				{
					//prop.item[n].dataName = GetDataSelectNameAt((i * MAX_TDATA) + n);
					swprintf_s(prop.item[n].dataName, CPropertiesDataItem::SIZE_DATANAME, _T("%s"), (LPCTSTR)(GetDataSelectNameAt((i * MAX_TDATA) + n)));
				}
				theApp.SetPropertiesData(i, prop);
			}
		}
		else
		{
			// �f�[�^�o�^�ɂȂ��Ď��f�[�^���v���p�e�B����폜
			for (int i = 0; i < MAX_VIEW; ++i)
			{
				CPropertiesData prop = theApp.GetPropertiesData(i);

				for (int j = 0; j < MAX_TDATA; ++j)
				{
					bool	res = false;

					for( int k = 0; k < (MAX_VIEW * MAX_TDATA); k++ )
					{
						CString	sDataName = GetDataSelectNameAt(k);

						if( (!sDataName.IsEmpty()) && (prop.item[j].dataName == sDataName) )
							res = true;
					}

					if (!res){
						//prop.item[j].dataName.Empty();
						wmemset(prop.item[j].dataName, 0, CPropertiesDataItem::SIZE_DATANAME);
					}
				}

				theApp.SetPropertiesData(i, prop);
			}
		}
	}

	// �v���p�e�B�ύX
	ChangePropaties();
#endif


}

/*============================================================================*/
/*! �f�[�^���X�g���w�肳�ꂽ�C���f�b�N�X�̃f�[�^�����擾����

-# �f�[�^���X�g���w�肳�ꂽ�C���f�b�N�X�̃f�[�^�����擾����

@param
	idx�F�C���f�b�N�X
@retval

*/
/*============================================================================*/
CString CPropertiesWnd::GetDataSelectNameAt(int idx)
{
	CString strData = _T("");
	BOOL bFind = FALSE;
	POSITION pos = m_DataNameList.GetHeadPosition();

	int cnt = 0;
	while (pos != NULL)
	{
		CString name = m_DataNameList.GetNext(pos);

		if (idx == cnt)
		{
			strData = name;
			break;
		}
		cnt++;
	}
	return strData;
}

/*============================================================================*/
/*! ��ʑI���R���{�{�b�N�X���X�g�Đݒ�

-# ��ʑI���R���{�{�b�N�X���X�g�Đݒ�

@param
	vBiew	�F�\�����
	name	�F�\������Ă���O���t��
@retval

*/
/*============================================================================*/
void CPropertiesWnd::ExchangeViewCombo(BOOL *vBiew, CString name)
{
	// ��ʑI�� �A�C�e���č쐬
	int nCmb = m_wndObjectCombo.GetCount();
	m_wndObjectCombo.ResetContent();

	nCmb = m_wndObjectCombo.GetCount();


	CPropertiesData prop;
	CString strView = _T("");
	for (int i = 0; i < MAX_VIEW; ++i)
	{
		prop = theApp.GetPropertiesData(i);
		if (vBiew[i])
		{
			m_wndObjectCombo.AddString(prop.wndName);
		}
	}
	
	m_wndObjectCombo.SelectString(0, name);
	m_wndObjectCombo.UpdateWindow();
}

/*============================================================================*/
/*! �ڍאݒ�/�ȈՐݒ�^�u�؂�ւ�

-# �^�O�؂�ւ�������

@param
@retval

*/
/*============================================================================*/
void CPropertiesWnd::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// �^�u�ɂ��\���ؑ�
	int tab = m_wndTabs.GetCurSel();
	if (tab == 0)
	{
		m_wndPropList.ShowWindow(SW_SHOW);
		m_wndList.ShowWindow(SW_HIDE);
		ChangePropaties(FALSE);
	}
	else if (tab == 1)
	{
		vector<COLORREF>	colorList;
		vector<CString>		dataList;

		CPropertiesData prop = theApp.GetPropertiesData(m_ActiveView);
		for (int i = 0; i < MAX_TDATA; ++i)
		{
			// �f�[�^���擾
			dataList.push_back(prop.item[i].dataName);

			// �f�[�^�\���F�擾
			colorList.push_back(prop.item[i].color);
		}
		m_wndList.SetIsDataSetting(0, dataList); 
		m_wndList.SetCelColor(0, colorList);

		int cnt = 0;
		for (int i = 0; i < MAX_TDATA; ++i)
		{
			if (CString(prop.item[i].dataName) != _T(""))
			{
				// �`�惂�[�h�ݒ�
				m_wndList.SetItemText(cnt, 1, strDrawMode[prop.item[i].drawMode]);

				// �f�[�^�\���擾
				m_wndList.SetCheck(cnt, prop.item[i].isViewMode);
				++cnt;
			}
		}

		m_wndPropList.ShowWindow(SW_HIDE);
		m_wndList.ShowWindow(SW_SHOW);
	}
	else
	{
		// nothing!
	}

	//	*pResult = 0;
	if (pResult)
		*pResult = 0;
}

/*============================================================================*/
/*! �ȈՐݒ�^�u�A�\���`�F�b�N�؂�ւ�

-# �\���`�F�b�N�؂�ւ�����

@param
@retval

*/
/*============================================================================*/
void CPropertiesWnd::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// �`�F�b�N�{�b�N�X��Ԏ擾
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView)
	{
		UINT uOldImage = pNMListView->uOldState & LVIS_STATEIMAGEMASK;
		UINT uNewImage = pNMListView->uNewState & LVIS_STATEIMAGEMASK;
		if (uNewImage != uOldImage)
		{
			CPropertiesData prop = theApp.GetPropertiesData(m_ActiveView);
			if (uNewImage == INDEXTOSTATEIMAGEMASK(1))
			{
				// Off
				prop.item[pNMListView->lParam].isViewMode = false;
			}
			else if (uNewImage == INDEXTOSTATEIMAGEMASK(2))
			{
				// On
				prop.item[pNMListView->lParam].isViewMode = true;
			}
			theApp.SetPropertiesData(m_ActiveView, prop);

			// �ύX��ʒm
			if (theApp.m_pMainWnd != NULL)
			{
				theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 0, (LPARAM)m_ActiveView);
			}
		}
	}

	*pResult = 0;
}

/*============================================================================*/
/*! �ȈՐݒ�^�u�A�`�惂�[�h�擾

-# �`�惂�[�h��String����enum�l�ɕϊ�����B

@param
@retval

*/
/*============================================================================*/
int CPropertiesWnd::GetDrawMode(CString str)
{
	for (int i = 0; strDrawMode[i] != _T(""); ++i)
	{
		if (str == strDrawMode[i])
		{
			return i;
		}
	}
	return -1;
}

LRESULT CPropertiesWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case eMessage_PropetyCmbChange:
		{
			CPropertiesData prop = theApp.GetPropertiesData(m_ActiveView);
			CString data = m_wndList.GetItemText((int)wParam, 1);

			prop.item[(int)lParam].drawMode = (DrawMode)GetDrawMode(data);
			theApp.SetPropertiesData(m_ActiveView, prop);

			// �ύX��ʒm
			theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 0, (LPARAM)m_ActiveView);
		}
		break;
	}

	return CDockablePane::WindowProc(message, wParam, lParam);
}
