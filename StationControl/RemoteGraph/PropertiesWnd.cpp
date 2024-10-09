// この MFC サンプル ソース コードでは、MFC Microsoft Office Fluent ユーザー インターフェイス 
// ("Fluent UI") の使用方法を示します。このコードは、MFC C++ ライブラリ ソフトウェアに 
// 同梱されている Microsoft Foundation Class リファレンスおよび関連電子ドキュメントを
// 補完するための参考資料として提供されます。
// Fluent UI を複製、使用、または配布するためのライセンス条項は個別に用意されています。
// Fluent UI ライセンス プログラムの詳細については、Web サイト
// http://go.microsoft.com/fwlink/?LinkId=238214 を参照してください。
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
/*! プロパティデータ

-# プロパティデータを保存

@param
@retval

*/
/*============================================================================*/
void CPropertiesData::WritePropertiesData(CString szFilePath, int pos)
{
	CString	sec, key, str;

	sec.Format(_T("%s%d"), (LPCTSTR)SECTION_PROPDATA, pos + 1);

	// リアルタイム／ノンリアルタイム
//	str.Format(_T("%d"), isRealTime ? 1 : 0);
//	WritePrivateProfileString(sec, KEY_REALTIME, str, szFilePath);

	// 縦軸スケール
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

	// 縦軸スケール
	key.Format(KEY_HSCL_NUM, 1);
	str.Format(_T("%d"), trandHScale[0]);
	WritePrivateProfileString(sec, key, str, szFilePath);
	key.Format(KEY_HSCL_NUM, 2);
	str.Format(_T("%d"), trandHScale[1]);
	WritePrivateProfileString(sec, key, str, szFilePath);

	// データ
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		key.Format(KEY_DATA_NAME, i + 1);
		WritePrivateProfileString(sec, key, item[i].dataName, szFilePath);
		key.Format(KEY_DATA_GNAME, i + 1);
		WritePrivateProfileString(sec, key, item[i].dataGName, szFilePath);
		key.Format(KEY_DATA_UNIT, i + 1);
		WritePrivateProfileString(sec, key, item[i].dataUnit, szFilePath);
	}

	// グラフ
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

	// コメント
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		key.Format(KEY_COM_DISP, i + 1);
		str.Format(_T("%d"), item[i].isComment ? 1 : 0);
		WritePrivateProfileString(sec, key, str, szFilePath);
		key.Format(KEY_COM_STR, i + 1);
		WritePrivateProfileString(sec, key, item[i].comment, szFilePath);
	}

	// リミット
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
/*! プロパティデータ

-# プロパティデータを復元

@param
@retval

*/
/*============================================================================*/
void CPropertiesData::ReadPropertiesData(CString szFilePath, int pos)
{
	CString	sec, key, str;
	TCHAR	wkbuf[1024];

	sec.Format(_T("%s%d"), (LPCTSTR)SECTION_PROPDATA, pos + 1);

	// リアルタイム／ノンリアルタイム
//	isRealTime = GetPrivateProfileInt(sec, KEY_REALTIME, 1, szFilePath) ? true : false;
	isRealTime = true;	// 強制的にデフォルトをリアルタイム

	// 縦軸スケール
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

	// 縦軸スケール
	key.Format(KEY_HSCL_NUM, 1);
#ifdef	_TEST_MAXDATA
	trandHScale[0] = (HScale)GetPrivateProfileInt(sec, key, h3, szFilePath);
#else	// 	_TEST_MAXDATA
	trandHScale[0] = (HScale)GetPrivateProfileInt(sec, key, h24, szFilePath);
#endif	// 	_TEST_MAXDATA
	key.Format(KEY_HSCL_NUM, 2);
	trandHScale[1] = (HScale)GetPrivateProfileInt(sec, key, h1, szFilePath);

	// データ
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

	// グラフ
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

	// コメント
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		key.Format(KEY_COM_DISP, i + 1);
		item[i].isComment = GetPrivateProfileInt(sec, key, 0, szFilePath) ? true : false;
		key.Format(KEY_COM_STR, i + 1);
		GetPrivateProfileString(sec, key, _T(""), wkbuf, _countof(wkbuf), szFilePath);
		item[i].comment = CString(wkbuf);
	}

	// リミット
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
// CPropertiesWnd メッセージ ハンドラー

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

	// コンボ ボックスの作成:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, ID_PROPERTIES_CB))
	{
		TRACE0("プロパティ コンボ ボックスを作成できませんでした\n");
		return -1;      // 作成できない場合
	}

	m_wndObjectCombo.SetFont(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));

	CRect rectCombo;
	m_wndObjectCombo.GetClientRect(&rectCombo);
	m_nComboHeight = rectCombo.Height();

	// タブの作成
	if (!m_wndTabs.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, ID_PROPERTIES_TAB))
	{
		TRACE0("プロパティ タブを作成できませんでした\n");
		return -1;      // 作成できない場合
	}

	m_wndTabs.InsertItem(0, _T("詳細設定"));
	m_wndTabs.InsertItem(1, _T("簡易設定"));

	// プロパティシート作成（タブ上）
	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rectDummy, &m_wndTabs, 2))
	{
		TRACE0("プロパティ グリッドを作成できませんでした\n");
		return -1;      // 作成できない場合
	}

	// リストの作成（タブ上）
	if (!m_wndList.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP | LVS_REPORT, rectDummy, &m_wndTabs, ID_PROPERTIES_LIST))
	{
		TRACE0("プロパティ リストを作成できませんでした\n");
		return -1;      // 作成できない場合
	}


	m_wndList.SetParent(this);

	DWORD dwStyle = m_wndList.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES;
	m_wndList.SetExtendedStyle(dwStyle);


	LVCOLUMN	lvc;
	lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;    // 有効フラグ
	TCHAR       caption[][32] = { _T("表示"), _T("描画モード") };
	const int   clmNum = sizeof caption / sizeof caption[0];

	for (int c = 0; c < clmNum; ++c)
	{
		lvc.iSubItem = c;			// サブアイテム番号
		lvc.pszText = caption[c];	// 見出しテキスト
		lvc.cx = 100;				// 横幅
		int id = m_wndList.InsertColumn(c, &lvc);
	}

#if 0
	LVITEM       lvi;
	CString      str;
	for (int r = 0; r < 10; ++r)
	{
		// データ
		lvi.lParam = (LPARAM)r;
		lvi.iItem = r;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT;
		str.Format(_T("データ%02d"), r + 1);
		lvi.pszText = str.GetBuffer();
		m_wndList.InsertItem(&lvi);

		// 描画モード
		lvi.lParam = (LPARAM)r;
		lvi.iItem = r;
		lvi.iSubItem = 1;
		lvi.mask = LVIF_TEXT;
		str.Format(_T("描画モード%02d"), r + 1);
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
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します
}

void CPropertiesWnd::InitPropList()
{
	int i = 0;
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties(0);	// プロパティ変更後太字表示無し

	//------------------------------------------------------
	// 表示設定
	CMFCPropertyGridProperty* pPropView = new CMFCPropertyGridProperty(_T("表示設定"));

	// 表示タイプ
	CMFCPropertyGridProperty* pPropViewMode = new CMFCPropertyGridProperty(_T("表示タイプ"), (variant_t)initPropData.ViewMode, _T("リアルタイム/ノンリアルタイム"), idViewMode);
	pPropViewMode->AddOption(_T("リアルタイム"));
	pPropViewMode->AddOption(_T("ノンリアルタイム"));
	pPropViewMode->AllowEdit(FALSE);
	pPropView->AddSubItem(pPropViewMode);

	// 開始日時
//	CCustomDlgProp* pPropViewDate = new CCustomDlgProp(_T("開始日時"), (variant_t)_T(""), _T("データ表示開始日時"), idViewDate);
//	pPropView->AddSubItem(pPropViewDate);
//	pPropViewDate->Enable(FALSE);
//	pPropViewDate->SetParent(this);

	m_wndPropList.AddProperty(pPropView);

	//------------------------------------------------------
	// 縦軸設定
	CMFCPropertyGridProperty* pPropV = new CMFCPropertyGridProperty(_T("縦軸設定"));

	// 縦軸設定-表示スケール(左)
	CMFCPropertyGridProperty* pPropVL = new CMFCPropertyGridProperty(_T("表示スケール(左)"));
	pPropV->AddSubItem(pPropVL);

	// 縦軸設定-表示スケール(左)-オートスケール
	CMFCPropertyGridProperty* pPropVLA = new CMFCPropertyGridProperty(_T("オートスケール"), (variant_t)initPropData.Auto, _T("オートスケールのON/OFF"), idVLAxisAutoRange);
	pPropVLA->AddOption(_T("On"));
	pPropVLA->AddOption(_T("Off"));
	pPropVLA->AllowEdit(FALSE);
	pPropVL->AddSubItem(pPropVLA);

	// 縦軸設定-表示スケール(左)-最大値
	CMFCPropertyGridProperty* pPropVLMax = new CMFCPropertyGridProperty(_T("最大値"), initPropData.VScalLMax, _T("最大値"), idVLAxisMaxRange);
//	pPropVLMax->EnableSpinControl(TRUE, -100, 100);
	pPropVLMax->Enable(FALSE);
	pPropVL->AddSubItem(pPropVLMax);

	// 縦軸設定-表示スケール(左)-最小値
	CMFCPropertyGridProperty* pPropVLMin = new CMFCPropertyGridProperty(_T("最小値"), initPropData.VScalLMin, _T("最小値"), idVLAxisMinRange);
//	pPropVLMin->EnableSpinControl(TRUE, -100, 100);
	pPropVLMin->Enable(FALSE);
	pPropVL->AddSubItem(pPropVLMin);

	// 縦軸設定-表示スケール(右)
	CMFCPropertyGridProperty* pPropVR = new CMFCPropertyGridProperty(_T("表示スケール(右)"));
	pPropV->AddSubItem(pPropVR);

	// 縦軸設定-表示スケール(右)-縦軸設定-最大値
	CMFCPropertyGridProperty* pPropVRMax = new CMFCPropertyGridProperty(_T("最大値"), initPropData.VScalRMax, _T("最大値"), idVRAxisMaxRange);
//	pPropVRMax->EnableSpinControl(TRUE, -100, 100);
	pPropVR->AddSubItem(pPropVRMax);

	// 縦軸設定-表示スケール(右)-最小値
	CMFCPropertyGridProperty* pPropVRMin = new CMFCPropertyGridProperty(_T("最小値"), initPropData.VScalRMin, _T("最小値"), idVRAxisMinRange);
//	pPropVRMin->EnableSpinControl(TRUE, -100, 100);
	pPropVR->AddSubItem(pPropVRMin);

	// 縦軸設定-目盛りの間隔
	CMFCPropertyGridProperty* pPropVM = new CMFCPropertyGridProperty(_T("目盛りの分割数"), initPropData.VSCalItvl, _T("目盛りの分割数"), idVAxisScaleInterval);
	pPropVM->EnableSpinControl(TRUE, AXIS_SCALE_MIN, AXIS_SCALE_MAX);
	pPropV->AddSubItem(pPropVM);

	m_wndPropList.AddProperty(pPropV);


	//------------------------------------------------------
	// 横軸設定
	CMFCPropertyGridProperty* pPropH = new CMFCPropertyGridProperty(_T("横軸設定"));

	// 横軸設定-表示スケール(上)
	CMFCPropertyGridProperty* pPropHSU = new CMFCPropertyGridProperty(_T("表示スケール(A)"), (variant_t)initPropData.HScaleU, _T("トレンドグラフビュー1の表示スケール"), idHUAxisScale);
	for (i = 0; strHScale[i] != _T(""); ++i)
	{
		if( !tblRealScaleInfo[i].flg_auto )
			pPropHSU->AddOption(strHScale[i]);
	}
	pPropHSU->AddOption(strHScale[autoscale]);
	pPropHSU->AllowEdit(FALSE);
	pPropH->AddSubItem(pPropHSU);

	// 横軸設定-表示スケール(下)
	CMFCPropertyGridProperty* pPropHSD = new CMFCPropertyGridProperty(_T("表示スケール(B)"), (variant_t)initPropData.HScaleL, _T("トレンドグラフビュー2の表示スケール"), idHLAxisScale);
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
	// データ設定
	for (int i = 0; i < MAX_TDATA; ++i){
		// データX
		CString str;
		str.Format(_T("データ%02d"), i + 1);
		CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(str);

		// データ選択
		CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(_T("データ選択"), _T(""), _T("データの選択"), ((idMax - idAxisMax)*i) + idSelectItem);
		pItem->AddOption(_T(""));
		pItem->AllowEdit(FALSE);
		pGroup->AddSubItem(pItem);

		// 名称
		str.Format(_T("Data%02d"), i + 1);
		pItem = new CMFCPropertyGridProperty(_T("名称"), str, _T("データ名称"), ((idMax - idAxisMax)*i) + idItemName);
		pGroup->AddSubItem(pItem);

		// 単位
		pItem = new CMFCPropertyGridProperty(_T("単位"), (COleVariant)_T("100"), _T("データの単位"), ((idMax - idAxisMax)*i) + idItemUnit);
//		pItem->Enable(FALSE);
		pGroup->AddSubItem(pItem);

		// 表示
		pItem = new CMFCPropertyGridProperty(_T("表示"), (variant_t)initPropData.Display, _T("表示のON/OFF"), ((idMax - idAxisMax)*i) + idDisplay);
		pItem->AddOption(_T("On"));
		pItem->AddOption(_T("Off"));
		pItem->AllowEdit(FALSE);
		pGroup->AddSubItem(pItem);

		// 描画モード
		pItem = new CMFCPropertyGridProperty(_T("描画モード"), (variant_t)initPropData.DrawKind, _T("Line/Dots/L&D"), ((idMax - idAxisMax)*i) + idDrawKind);
		pItem->AddOption(_T("Line"));
		pItem->AddOption(_T("Dots"));
		pItem->AddOption(_T("L&D"));
		pItem->AllowEdit(FALSE);
		pGroup->AddSubItem(pItem);

		// 表示色
		CMFCPropertyGridColorProperty* pGColorProp = new CMFCPropertyGridColorProperty(_T("表示色"), 0, NULL, _T("表示色"), ((idMax - idAxisMax)*i) + idDrawColor);
		pGColorProp->EnableAutomaticButton(_T("既定値"), colordef[i]);
		pGColorProp->EnableOtherButton(_T("その他..."));
		pGColorProp->SetColor(colordef[i]);

		pGroup->AddSubItem(pGColorProp);

		// ドットサイズ
		pItem = new CMFCPropertyGridProperty(_T("ドットサイズ"), (variant_t)initPropData.DotSize, _T("Large/Medium/Small"), ((idMax - idAxisMax)*i) + idDotSize);
		pItem->AddOption(_T("Small"));
		pItem->AddOption(_T("Medium"));
		pItem->AddOption(_T("Large"));
		pItem->Enable(false);
		pItem->AllowEdit(FALSE);
		pGroup->AddSubItem(pItem);

		// 縦軸スケール選択
		pItem = new CMFCPropertyGridProperty(_T("縦軸スケール選択"), (variant_t)initPropData.SelVScale, _T("縦軸のスケール選択(左 or 右)"), ((idMax - idAxisMax)*i) + idVScale);
		pItem->AddOption(_T("左"));
		pItem->AddOption(_T("右"));
		pItem->AllowEdit(FALSE);
		pGroup->AddSubItem(pItem);

		//------------------------------------------------------
		// コメント
		CMFCPropertyGridProperty* pGroupCm = new CMFCPropertyGridProperty(_T("コメント"));

		// コメント表示
		pItem = new CMFCPropertyGridProperty(_T("表示"), (variant_t)_T("Off"), _T("コメントのON/OFF"), ((idMax - idAxisMax)*i) + idCommentEnable);
		pItem->AddOption(_T("On"));
		pItem->AddOption(_T("Off"));
		pItem->AllowEdit(FALSE);
		pGroupCm->AddSubItem(pItem);

		// コメント
		pItem = new CMFCPropertyGridProperty(_T("コメント"), (COleVariant)_T(""), _T("グラフ上のデータコメント"), ((idMax - idAxisMax)*i) + idComment);
		pGroupCm->AddSubItem(pItem);

		pGroupCm->Expand(FALSE);
		pGroup->AddSubItem(pGroupCm);

		//------------------------------------------------------
		// リミット
		CMFCPropertyGridProperty* pGroupAl = new CMFCPropertyGridProperty(_T("リミット"));

		// ワーニング上限線表示＆監視
		pItem = new CMFCPropertyGridProperty(_T("ワーニング上限 - 表示"), (variant_t)_T("Off"), _T("ワーニング上限線表示＆監視のON/OFF"), ((idMax - idAxisMax)*i) + idLimitWarningUpperEnable);
		pItem->AddOption(_T("On"));
		pItem->AddOption(_T("Off"));
		pItem->AllowEdit(FALSE);
		pGroupAl->AddSubItem(pItem);

		// ワーニング上限
		pItem = new CMFCPropertyGridProperty(_T("ワーニング上限"), initPropData.LimitWarningUpper, _T("ワーニング上限"), ((idMax - idAxisMax)*i) + idLimitWarningUpper);
		pItem->Enable(FALSE);
		pGroupAl->AddSubItem(pItem);

		// ワーニング下限線表示＆監視
		pItem = new CMFCPropertyGridProperty(_T("ワーニング下限 - 表示"), (variant_t)_T("Off"), _T("ワーニング下限線表示＆監視のON/OFF"), ((idMax - idAxisMax)*i) + idLimitWarningLowerEnable);
		pItem->AddOption(_T("On"));
		pItem->AddOption(_T("Off"));
		pItem->AllowEdit(FALSE);
		pGroupAl->AddSubItem(pItem);

		// ワーニング下限
		pItem = new CMFCPropertyGridProperty(_T("ワーニング下限"), initPropData.LimitWarningLower, _T("ワーニング下限"), ((idMax - idAxisMax)*i) + idLimitWarningLower);
		pItem->Enable(FALSE);
		pGroupAl->AddSubItem(pItem);

		// アラーム上限線表示＆監視
		pItem = new CMFCPropertyGridProperty(_T("アラーム上限 - 表示"), (variant_t)_T("Off"), _T("アラーム上限線表示＆監視のON/OFF"), ((idMax - idAxisMax)*i) + idLimitAlarmUpperEnable);
		pItem->AddOption(_T("On"));
		pItem->AddOption(_T("Off"));
		pItem->AllowEdit(FALSE);
		pGroupAl->AddSubItem(pItem);

		// アラーム上限
		pItem = new CMFCPropertyGridProperty(_T("アラーム上限"), initPropData.LimitAlarmUpper, _T("アラーム上限"), ((idMax - idAxisMax)*i) + idLimitAlarmUpper);
		pItem->Enable(FALSE);
		pGroupAl->AddSubItem(pItem);

		// アラーム下限線表示＆監視
		pItem = new CMFCPropertyGridProperty(_T("アラーム下限 - 表示"), (variant_t)_T("Off"), _T("アラーム下限線表示＆監視のON/OFF"), ((idMax - idAxisMax)*i) + idLimitAlarmLowerEnable);
		pItem->AddOption(_T("On"));
		pItem->AddOption(_T("Off"));
		pItem->AllowEdit(FALSE);
		pGroupAl->AddSubItem(pItem);

		// アラーム下限
		pItem = new CMFCPropertyGridProperty(_T("アラーム下限"), initPropData.LimitAlarmLower, _T("アラーム下限"), ((idMax - idAxisMax)*i) + idLimitAlarmLower);
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

	// 選択されたViewをアクティブにする
	((CMDIChildWnd*)prop.wnd)->MDIActivate();
}

void CPropertiesWnd::ChangeViewMode()
{
	bool	bPropetyChange = true;

	// プロパティデータ取得
	CPropertiesData&	prop = theApp.GetPropertiesData(m_ActiveView);

	// 表示設定
	CMFCPropertyGridProperty	*pView = m_wndPropList.GetProperty(idViewGr);

	// 表示設定-表示タイプ
	CMFCPropertyGridProperty	*pViewMode = pView->GetSubItem(idViewGrMode);
	CString	str = (CString(pViewMode->GetValue().bstrVal).Compare(_T("リアルタイム")) == 0) ? _T("ノンリアルタイム") : _T("リアルタイム");

	// 表示タイプを切り替え
	pViewMode->SetValue(str);
	SetViewMode(prop, str);

	if( str.Compare(_T("ノンリアルタイム")) == 0 )
	{
		if( ((CMainFrame *)theApp.m_pMainWnd)->GetOnline() )
			bPropetyChange = false;
	}
		
	// プロパティデータセット
	theApp.SetPropertiesData(m_ActiveView, prop);

	// データ取得対象フラグを設定
	theApp.GetDataCollection().SetSelectDataValid();

	// 変更を通知
	if( bPropetyChange )
		theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 0, (LPARAM)m_ActiveView);
}

void CPropertiesWnd::SetViewMode(CPropertiesData& prop, CString str)
{
	if (str.Compare(_T("リアルタイム")) == 0)
	{
		prop.isRealTime = true;

		// 解析データを削除
		theApp.GetDataCollection().DeleteAnalysisData(m_ActiveView);
	}
	else
	{
		prop.isRealTime= false;

		if( ((CMainFrame *)theApp.m_pMainWnd)->GetOnline() )
		{
			// 解析移行フラグをセット
			theApp.SetAnalysisMigration(m_ActiveView);

			// データが取得されていない時でもノンリアルタイムに移行するためメッセージを発行
			(theApp.m_pMainWnd)->PostMessage(eMessage_ThreadNormal, 0/*取得分だけMin/Maxを更新*/, 0);
		}
		else
		{
			// 解析データを作成
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
	case idViewMode:	// 表示タイプ
		str = pProp->GetValue().bstrVal;

		SetViewMode(prop, str);

		if( str.Compare(_T("ノンリアルタイム")) == 0 )
		{
			if( ((CMainFrame *)theApp.m_pMainWnd)->GetOnline() )
				bPropetyChange = false;
		}
		break;

//	case idViewDate:	// 開始日時
//	{
//		str = pProp->GetValue().bstrVal;
//
//		COleDateTime oleTime;
//		oleTime.ParseDateTime(str);
//		CTime t(oleTime.GetYear(), oleTime.GetMonth(), oleTime.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());
//		prop.startTime = t;
//	}
//		break;

	case idVLAxisAutoRange:	// 縦軸左オートスケール
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("On")) == 0)
		{
			prop.trandVScale.Auto = true;

			// 最大値、最小値を非活性にする。
			CMFCPropertyGridProperty* pMax = pProp->GetParent()->GetSubItem(idVAxisGrScalLGrMax);
			pMax->Enable(FALSE);
			CMFCPropertyGridProperty* pMin = pProp->GetParent()->GetSubItem(idVAxisGrScalLGrMin);
			pMin->Enable(FALSE);

			m_AutoScale[m_ActiveView] = TRUE;
		}
		else
		{
			prop.trandVScale.Auto = false;

			// 最大値、最小値を活性にする。
			CMFCPropertyGridProperty* pMax = pProp->GetParent()->GetSubItem(idVAxisGrScalLGrMax);
			pMax->Enable(TRUE);
			CMFCPropertyGridProperty* pMin = pProp->GetParent()->GetSubItem(idVAxisGrScalLGrMin);
			pMin->Enable(TRUE);

			m_AutoScale[m_ActiveView] = FALSE;
		}
		break;

	case idVLAxisMaxRange:	// 縦軸左最大値
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

	case idVLAxisMinRange:	// 縦軸左最小値
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

	case idVRAxisMaxRange:	// 縦軸右最大値
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

	case idVRAxisMinRange:	// 縦軸右最小値
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

	case idVAxisScaleInterval:	// 縦軸目盛りの間隔
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

	case idHUAxisScale:		// 横軸上スケール
		str = pProp->GetValue().bstrVal;
		prop.trandHScale[0] = (HScale)prop.getHScal(str);
		break;

	case idHLAxisScale:		// 横軸下スケール
		str = pProp->GetValue().bstrVal;
		prop.trandHScale[1] = (HScale)prop.getHScal(str);
		break;

	case idSelectItem:	// データ選択
		str = pProp->GetValue().bstrVal;
		//prop.item[pos].dataName = str;
		swprintf_s(prop.item[pos].dataName, CPropertiesDataItem::SIZE_DATANAME, _T("%s"), (LPCTSTR)(str.Left(CPropertiesDataItem::SIZE_DATANAME - 1)));
		pProp->SetValue(prop.item[pos].dataName);
		pProp->Redraw();

#if 0
		CGraphMutex::Lock(eRealData);

		// 圧縮データを再作成
		theApp.GetDataCollection().CompSpaceData(theApp.GetDataCollection().ConvertNameToId(str), 0, true);

		CGraphMutex::Unlock(eRealData);
#endif

		// Autoスケールをクリア
		theApp.m_pMainWnd->PostMessage(eMessage_AutoClear, 1, m_ActiveView);

		break;

	case idItemName:	// 名称
		str = pProp->GetValue().bstrVal;
		swprintf_s(prop.item[pos].dataGName, CPropertiesDataItem::SIZE_DATAGNAME, _T("%s"), (LPCTSTR)(str.Left(CPropertiesDataItem::SIZE_DATAGNAME - 1)));
		pProp->SetValue(prop.item[pos].dataGName);
		pProp->Redraw();
		break;

	case idItemUnit:	// 単位
		str = pProp->GetValue().bstrVal;
		swprintf_s(prop.item[pos].dataUnit, CPropertiesDataItem::SIZE_DATAUNIT, _T("%s"), (LPCTSTR)(str.Left(CPropertiesDataItem::SIZE_DATAUNIT - 1)));
		pProp->SetValue(prop.item[pos].dataUnit);
		pProp->Redraw();
		break;

	case idDisplay:	// 表示
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

	case idDrawKind:	// 描画モード
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("Line")) == 0)
		{
			prop.item[pos].drawMode = Line;

			// ドットサイズ 変更不可
			CMFCPropertyGridProperty* pDotProp = pProp->GetParent()->GetSubItem(idDataGrDotSiz);
			pDotProp->Enable(FALSE);
		}
		else if (str.Compare(_T("Dots")) == 0)
		{
			prop.item[pos].drawMode = Dots;

			// ドットサイズ 変更可
			CMFCPropertyGridProperty* pDotProp = pProp->GetParent()->GetSubItem(idDataGrDotSiz);
			pDotProp->Enable(TRUE);
		}
		else if (str.Compare(_T("L&D")) == 0)
		{
			prop.item[pos].drawMode = LineAndDots;

			// ドットサイズ 変更可
			CMFCPropertyGridProperty* pDotProp = pProp->GetParent()->GetSubItem(idDataGrDotSiz);
			pDotProp->Enable(TRUE);
		}

		break;

	case idDrawColor:	// 表示色
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

	case idDotSize:		// ドットサイズ
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

	case idVScale:		// 横軸スケール選択
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("左")) == 0)
		{
			prop.item[pos].vscaleKind = LScale;
		}
		else
		{
			prop.item[pos].vscaleKind = RScale;
		}
		break;
		
	case idCommentEnable:	// コメント表示
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

	case idComment:			// コメント
		str = pProp->GetValue().bstrVal;
		prop.item[pos].comment = str;
		break;

	case idLimitWarningUpperEnable:		// ワーニング上限 - 表示
		str = pProp->GetValue().bstrVal;
		prop.item[pos].isLimitWarningUpper = (str.Compare(_T("On")) == 0) ? true : false;
		pProp->GetParent()->GetSubItem(idDataGrLimitWarningUpper)->Enable(prop.item[pos].isLimitWarningUpper ? TRUE : FALSE);
		break;

	case idLimitWarningLowerEnable:		// ワーニング下限 - 表示
		str = pProp->GetValue().bstrVal;
		prop.item[pos].isLimitWarningLower = (str.Compare(_T("On")) == 0) ? true : false;
		pProp->GetParent()->GetSubItem(idDataGrLimitWarningLower)->Enable(prop.item[pos].isLimitWarningLower ? TRUE : FALSE);
		break;

	case idLimitAlarmUpperEnable:		// アラーム上限 - 表示
		str = pProp->GetValue().bstrVal;
		prop.item[pos].isLimitAlarmUpper = (str.Compare(_T("On")) == 0) ? true : false;
		pProp->GetParent()->GetSubItem(idDataGrLimitAlarmUpper)->Enable(prop.item[pos].isLimitAlarmUpper ? TRUE : FALSE);
		break;

	case idLimitAlarmLowerEnable:		// アラーム下限 - 表示
		str = pProp->GetValue().bstrVal;
		prop.item[pos].isLimitAlarmLower = (str.Compare(_T("On")) == 0) ? true : false;
		pProp->GetParent()->GetSubItem(idDataGrLimitAlarmLower)->Enable(prop.item[pos].isLimitAlarmLower ? TRUE : FALSE);
		break;

	case idLimitWarningUpper:	// ワーニング上限
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

	case idLimitWarningLower:	// ワーニング下限
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

	case idLimitAlarmUpper:	// アラーム上限
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

	case idLimitAlarmLower:	// アラーム下限
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

		
	// プロパティデータセット
	theApp.SetPropertiesData(m_ActiveView, prop);

	// データ取得対象フラグを設定
	theApp.GetDataCollection().SetSelectDataValid();

	// 変更を通知
	if( bPropetyChange )
		theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 0, (LPARAM)m_ActiveView);

	return 0;
}

/*============================================================================*/
/*! プロパティウィンドウデータ新規作成

-# グラフウィンドウが作成された時に、プロパティデータを作成する。

@param
@retval

*/
/*============================================================================*/
void CPropertiesWnd::SetNewPropertyData(int pos, LPARAM wnd)
{
	// アクティブウィンドウ設定
	m_ActiveView = pos;

	// ウィンドウ名取得
	CString name;
	((CMDIChildWnd*)wnd)->GetWindowTextW(name);

	// ”画面選択”用リスト更新
	m_wndObjectCombo.AddString(name);
	m_wndObjectCombo.SelectString(0, name);

#if 0
#ifdef _NOCOMM
	CPropertiesData dummy = theApp.GetPropertiesData(m_ActiveView);
#endif
	prop.wndIdx = pos;					// ウィンドー番号
	prop.wndName = name;				// ウィンドー名称
	prop.wnd = wnd;

	prop.isRealTime = true;									// リアルタイム（true）/ ノンリアルタイム（false）
	prop.trandVScale.Auto = true;							// 縦軸左 オートスケール
	m_AutoScale[pos] = TRUE;
	prop.trandVScale.VScalLMax = initPropData.VScalLMax;	// 縦軸左 最大値
	prop.trandVScale.VScalLMin = initPropData.VScalLMin;	// 縦軸左 最小値
	prop.trandVScale.VScalRMax = initPropData.VScalRMax;	// 縦軸右 最大値
	prop.trandVScale.VScalRMin = initPropData.VScalRMin;	// 縦軸右 最小値
	prop.trandVScale.VSCalItvl = initPropData.VSCalItvl;	// 縦軸目盛りの間隔

	prop.trandHScale[0] = h24;			// 横軸スケール(上)
	prop.trandHScale[1] = m60;			// 横軸スケール(下)

	CString str = _T("");
	for (int i = 0; i < MAX_TDATA; ++i)
	{
#ifdef _NOCOMM
		prop.dataName[i] = dummy.dataName[i];		// データ名称
#else
		prop.dataName[i] = _T("");		// データ名称
#endif
		str.Format(_T("Data%02d"), i + 1);
		prop.dataGName[i] = str;		// データ名称
		prop.dataUnit[i] = _T("");		// データ単位
		prop.isViewMode[i] = true;		// 表示On/Off
		prop.drawMode[i] = Line;		// 描画モード（Line、Dots、L&D）
		prop.color[i] = colordef[i];	// 表示色
		prop.dotSize[i] = Small;		// ドットサイズ
		prop.vscaleKind[i] = LScale;	// 縦軸スケール選択
	}
#else
	CPropertiesData& prop = theApp.GetPropertiesData(pos);
	prop.wndIdx = pos;					// ウィンドー番号
	prop.wndName.Empty();
	prop.wndName = name;				// ウィンドー名称
	prop.wnd = wnd;

	m_AutoScale[pos] = prop.trandVScale.Auto ? TRUE : FALSE;
#endif

	// プロパティデータセット
	//theApp.SetPropertiesData(pos, prop);

	// プロパティ変更
	ChangePropaties();

	// 簡易リストを変更
	if( m_wndTabs.GetCurSel() == 1 )
		OnTcnSelchangeTab(0, 0);

	// 変更を通知
	theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 1, (LPARAM)m_ActiveView);
	return;
}


/*============================================================================*/
/*! プロパティ表示をアクティブウィンドウ用に変更

-# 表示されているウィンドウが変更されたら、プロパティー表示を変更する。

@param
@retval

*/
/*============================================================================*/
void CPropertiesWnd::ChangeView(int pos)
{
	CPropertiesData prop = theApp.GetPropertiesData(pos);
	m_wndObjectCombo.SelectString(0, prop.wndName);
	m_ActiveView = pos;

	// プロパティ変更
	ChangePropaties();

	// 簡易リストを変更
	if( m_wndTabs.GetCurSel() == 1 )
		OnTcnSelchangeTab(0, 0);
}


/*============================================================================*/
/*! プロパティウィンドウデータ削除

-# ウィンドウが終了された時に、プロパティ画面選択から削除する。

@param
@retval

*/
/*============================================================================*/
void CPropertiesWnd::CloseView(int pos)
{
	// ”画面選択”用リスト更新
	CPropertiesData prop = theApp.GetPropertiesData(pos);
	m_wndObjectCombo.DeleteString(m_wndObjectCombo.FindString(0, prop.wndName));
}

/*============================================================================*/
/*! 画面選択変更

-# 画面選択が変更された時に、プロパティをその画面用に更新する。

@param
@retval

*/
/*============================================================================*/
void CPropertiesWnd::ChangePropaties(BOOL bUpdate)
{
	CString str = _T("");

	// プロパティデータ取得
	CPropertiesData& prop = theApp.GetPropertiesData(m_ActiveView);

	// 表示設定
	CMFCPropertyGridProperty* pView = m_wndPropList.GetProperty(idViewGr);

	// 表示設定-表示タイプ
	CMFCPropertyGridProperty* pViewMode = pView->GetSubItem(idViewGrMode);
	str = pViewMode->GetName();
	if (prop.isRealTime)
	{
		pViewMode->SetValue(_T("リアルタイム"));
	}
	else
	{
		pViewMode->SetValue(_T("ノンリアルタイム"));
	}

	// 縦軸設定
	CMFCPropertyGridProperty* pVAxis = m_wndPropList.GetProperty(idVAxisGr);

	// 縦軸設定-表示スケール(左)-オートスケール
	CMFCPropertyGridProperty* pVAxisLAuto = pVAxis->GetSubItem(idVAxisGrScalLGr)->GetSubItem(idVAxisGrScalLGrAuto);
	if (prop.trandVScale.Auto)
	{
		pVAxisLAuto->SetValue(_T("On"));
	}
	else
	{
		pVAxisLAuto->SetValue(_T("Off"));
	}

	// 縦軸設定-表示スケール(左)-最大値
	CMFCPropertyGridProperty* pVAxisLMax = pVAxis->GetSubItem(idVAxisGrScalLGr)->GetSubItem(idVAxisGrScalLGrMax);
	pVAxisLMax->SetValue((double)prop.trandVScale.VScalLMax);
	pVAxisLMax->Enable(prop.trandVScale.Auto ? FALSE : TRUE);

	// 縦軸設定-表示スケール(左)-最小値
	CMFCPropertyGridProperty* pVAxisLMin = pVAxis->GetSubItem(idVAxisGrScalLGr)->GetSubItem(idVAxisGrScalLGrMin);
	pVAxisLMin->SetValue((double)prop.trandVScale.VScalLMin);
	pVAxisLMin->Enable(prop.trandVScale.Auto ? FALSE : TRUE);

	// 縦軸設定-表示スケール(右)-最大値
	CMFCPropertyGridProperty* pVAxisRMax = pVAxis->GetSubItem(idVAxisGrScalRGr)->GetSubItem(idVAxisGrScalRGrMax);
	pVAxisRMax->SetValue((double)prop.trandVScale.VScalRMax);

	// 縦軸設定-表示スケール(右)-最小値
	CMFCPropertyGridProperty* pVAxisRMin = pVAxis->GetSubItem(idVAxisGrScalRGr)->GetSubItem(idVAxisGrScalRGrMin);
	pVAxisRMin->SetValue((double)prop.trandVScale.VScalRMin);

	// 縦軸設定-目盛りの間隔
	CMFCPropertyGridProperty* pVAxisScaleItvl = pVAxis->GetSubItem(idVAxisGrScalInvl);
	pVAxisScaleItvl->SetValue((long)prop.trandVScale.VSCalItvl);


	// 横軸設定
	CMFCPropertyGridProperty* pHAxis = m_wndPropList.GetProperty(idHAxisGr);

	// 横軸設定-表示スケール(上)
	CMFCPropertyGridProperty* pHAxisScaleU = pHAxis->GetSubItem(idHAxisGrScaleU);
	pHAxisScaleU->SetValue((variant_t)(strHScale[prop.trandHScale[0]]));

	// 横軸設定-表示スケール(上)
	CMFCPropertyGridProperty* pHAxisScaleL = pHAxis->GetSubItem(idHAxisGrScaleL);
	pHAxisScaleL->SetValue((variant_t)(strHScale[prop.trandHScale[1]]));


	// データ

	for (int i = 0; i < MAX_TDATA; ++i)
	{
		CMFCPropertyGridProperty* pData = m_wndPropList.GetProperty(idDataGr + i);

		// データ名称
		CMFCPropertyGridProperty* pItem = pData->GetSubItem(idDataGrDataSelect);
		pItem->SetValue(prop.item[i].dataName);
		str = pItem->GetName();

		// データ名称（グラフ用）
		pItem = pData->GetSubItem(idDataGrName);
		pItem->SetValue(prop.item[i].dataGName);
		str = pItem->GetName();

		// 単位
		pItem = pData->GetSubItem(idDataGrItemUnit);
		pItem->SetValue(prop.item[i].dataUnit);
		str = pItem->GetName();
			
		// 表示
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

		// 描画モード
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

		// 表示色
		CMFCPropertyGridColorProperty* pGColorProp = (CMFCPropertyGridColorProperty*)pData->GetSubItem(idDataGrColor);
		pGColorProp->SetColor(prop.item[i].color);
		str = pGColorProp->GetName();

		// ドットサイズ
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
			// 非活性
			pItem->Enable(FALSE);
		}
		else
		{
			// 活性
			pItem->Enable(TRUE);
		}

		// 縦軸スケール選択
		pItem = pData->GetSubItem(idDataGrSelScale);
		str = pItem->GetName();
		if (prop.item[i].vscaleKind == LScale)
		{
			pItem->SetValue(_T("左"));
		}
		else
		{
			pItem->SetValue(_T("右"));
		}

		CMFCPropertyGridProperty* pComment = pData->GetSubItem(idDataGrComment);

		// コメント表示
		pItem = pComment->GetSubItem(idDataGrCommentEnable);
		if (prop.item[i].isComment)
		{
			pItem->SetValue(_T("On"));
		}
		else
		{
			pItem->SetValue(_T("Off"));
		}

		// コメント
		pItem = pComment->GetSubItem(idDataGrCommentStr);
		str = pItem->GetName();
		pItem->SetValue(prop.item[i].comment);

		CMFCPropertyGridProperty* pLimit = pData->GetSubItem(idDataGrLimit);

		// ワーニング上限 - 表示
		pItem = pLimit->GetSubItem(idDataGrLimitWarningUpperEnable);
		if (prop.item[i].isLimitWarningUpper)
		{
			pItem->SetValue(_T("On"));
		}
		else
		{
			pItem->SetValue(_T("Off"));
		}

		// ワーニング上限
		pItem = pLimit->GetSubItem(idDataGrLimitWarningUpper);
		pItem->SetValue((double)prop.item[i].limitWarningUpper);
		pItem->Enable(prop.item[i].isLimitWarningUpper ? TRUE : FALSE);

		// ワーニング下限 - 表示
		pItem = pLimit->GetSubItem(idDataGrLimitWarningLowerEnable);
		if (prop.item[i].isLimitWarningLower)
		{
			pItem->SetValue(_T("On"));
		}
		else
		{
			pItem->SetValue(_T("Off"));
		}

		// ワーニング下限
		pItem = pLimit->GetSubItem(idDataGrLimitWarningLower);
		pItem->SetValue((double)prop.item[i].limitWarningLower);
		pItem->Enable(prop.item[i].isLimitWarningLower ? TRUE : FALSE);

		// アラーム上限 - 表示
		pItem = pLimit->GetSubItem(idDataGrLimitAlarmUpperEnable);
		if (prop.item[i].isLimitAlarmUpper)
		{
			pItem->SetValue(_T("On"));
		}
		else
		{
			pItem->SetValue(_T("Off"));
		}

		// アラーム上限
		pItem = pLimit->GetSubItem(idDataGrLimitAlarmUpper);
		pItem->SetValue((double)prop.item[i].limitAlarmUpper);
		pItem->Enable(prop.item[i].isLimitAlarmUpper ? TRUE : FALSE);

		// アラーム下限 - 表示
		pItem = pLimit->GetSubItem(idDataGrLimitAlarmLowerEnable);
		if (prop.item[i].isLimitAlarmLower)
		{
			pItem->SetValue(_T("On"));
		}
		else
		{
			pItem->SetValue(_T("Off"));
		}

		// アラーム下限
		pItem = pLimit->GetSubItem(idDataGrLimitAlarmLower);
		pItem->SetValue((double)prop.item[i].limitAlarmLower);
		pItem->Enable(prop.item[i].isLimitAlarmLower ? TRUE : FALSE);
	}

	// プロパティデータセット
	if (bUpdate)
	{
		theApp.SetPropertiesData(m_ActiveView, prop);
	}
}

/*============================================================================*/
/*! データ選択用リスト更新

-# データ選択用リストを更新する。

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
		// データ選択コントロール取得
		CMFCPropertyGridProperty* pData = m_wndPropList.GetProperty(idDataGr + i);
		CMFCPropertyGridProperty* pItem = pData->GetSubItem(idDataGrDataSelect);

		// コンボボックス初期化
		CString sName = _T("");
		sName = pItem->GetValue().bstrVal;
		pItem->RemoveAllOptions();
		pItem->AddOption(_T(""));

		BOOL bFind = FALSE;
		POSITION pos = m_DataNameList.GetHeadPosition();
		while (pos != NULL)
		{
			// データ選択リスト再構築
			CString name = m_DataNameList.GetNext(pos);
			pItem->AddOption(name);

			if ( !bFind && (name == sName) )
			{
				bFind = TRUE;
			}
		}

		// 現在選択中のデータがリストに無いので、表示を削除する。
		if ( !bFind )
		{
			pItem->SetValue(_T(""));
		}
	}

#ifdef _NOCOMM
	if( bDataName )
	{
		// プロパティデータ取得
		if( bAutoAssign )
		{
			// プロパティにデータ登録の監視データを自動割り振り
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
			// データ登録にない監視データをプロパティから削除
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

	// プロパティ変更
	ChangePropaties();
#endif


}

/*============================================================================*/
/*! データリストより指定されたインデックスのデータ名を取得する

-# データリストより指定されたインデックスのデータ名を取得する

@param
	idx：インデックス
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
/*! 画面選択コンボボックスリスト再設定

-# 画面選択コンボボックスリスト再設定

@param
	vBiew	：表示状態
	name	：表示されているグラフ名
@retval

*/
/*============================================================================*/
void CPropertiesWnd::ExchangeViewCombo(BOOL *vBiew, CString name)
{
	// 画面選択 アイテム再作成
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
/*! 詳細設定/簡易設定タブ切り替え

-# タグ切り替え時処理

@param
@retval

*/
/*============================================================================*/
void CPropertiesWnd::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// タブによる表示切替
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
			// データ名取得
			dataList.push_back(prop.item[i].dataName);

			// データ表示色取得
			colorList.push_back(prop.item[i].color);
		}
		m_wndList.SetIsDataSetting(0, dataList); 
		m_wndList.SetCelColor(0, colorList);

		int cnt = 0;
		for (int i = 0; i < MAX_TDATA; ++i)
		{
			if (CString(prop.item[i].dataName) != _T(""))
			{
				// 描画モード設定
				m_wndList.SetItemText(cnt, 1, strDrawMode[prop.item[i].drawMode]);

				// データ表示取得
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
/*! 簡易設定タブ、表示チェック切り替え

-# 表示チェック切り替え処理

@param
@retval

*/
/*============================================================================*/
void CPropertiesWnd::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// チェックボックス状態取得
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

			// 変更を通知
			if (theApp.m_pMainWnd != NULL)
			{
				theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 0, (LPARAM)m_ActiveView);
			}
		}
	}

	*pResult = 0;
}

/*============================================================================*/
/*! 簡易設定タブ、描画モード取得

-# 描画モードのStringからenum値に変換する。

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

			// 変更を通知
			theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 0, (LPARAM)m_ActiveView);
		}
		break;
	}

	return CDockablePane::WindowProc(message, wParam, lParam);
}
