/*============================================================================*/
/*! TrendPropertiesDlg.cpp

-# 追尾状況グラフ設定画面
*/
/*============================================================================*/
// TrendPropertiesDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrendPropertiesDlg.h"
#include "afxdialogex.h"
#include "HookMsgBox.h"

/////////////////////////////////////////////////////////////////////////////
// CTrendPropertiesDlg ダイアログ

IMPLEMENT_DYNAMIC(CTrendPropertiesDlg, CPropertiesCommonDlg)

/*============================================================================*/
/*! CTrendPropertiesDlg

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CTrendPropertiesDlg::CTrendPropertiesDlg(CWnd* pParent /*=NULL*/)
: CPropertiesCommonDlg(CTrendPropertiesDlg::IDD, pParent)
{
	m_PropertiesType = ePropertiesType_Trend;
	m_nListMax = MAX_TDATA;
	m_bChgGraphPropDlg = FALSE;
	m_bModified = FALSE;
}

/*============================================================================*/
/*! CTrendPropertiesDlg

-# コンストラクタ

@param  pParent：親ウィンドウ
@param	nViewID：グラフビューID
@param	listViewName：グラフビュー名リスト
@param	nPropertiesID：設定ID（SatelliteDataクラス内の配列インデックスID）
@param  nStaionID：衛星ID
@param  nParentID：親ウィンドウID
@param  nPropertiesType：設定タイプ（トレンド/サマリ）
@param  strWndName：ウィンドウタイトル
@param	pSummaryProp：サマリプロパティ
@retval なし
*/
/*============================================================================*/
CTrendPropertiesDlg::CTrendPropertiesDlg(CWnd* pParent, UINT nViewID, vector<CString>& listViewName, UINT nPropertiesID, UINT nStaionID, UINT nParentID /*= 0*/, 
	UINT nPropertiesType /*= ePropertiesType_Trend*/, CString strWndName /*= _T("")*/, CWnd* pSummaryProp /*= NULL*/, BOOL bChgDlg /*= FALSE*/)
	: CPropertiesCommonDlg(CTrendPropertiesDlg::IDD, pParent, nViewID, listViewName, nPropertiesID)
{
	m_nStationID = nStaionID;
	m_nParentID = nParentID;
	m_PropertiesType = nPropertiesType;
	m_strWndName = strWndName;
	if (m_PropertiesType == ePropertiesType_Trend)
	{
		m_nListMax = MAX_TDATA;
	}
	else
	{
		m_nListMax = MAX_SUMMARYTDATA;
	}
	m_pSummaryProp = pSummaryProp;
	m_bChgDlg = bChgDlg;
	m_bModified = FALSE;
}

/*============================================================================*/
/*! CTrendPropertiesDlg

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CTrendPropertiesDlg::~CTrendPropertiesDlg()
{
}

void CTrendPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertiesCommonDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTrendPropertiesDlg, CPropertiesCommonDlg)
	ON_WM_CREATE()
	ON_COMMAND(ID_EXPAND_ALL, &CPropertiesCommonDlg::OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, &CPropertiesCommonDlg::OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, &CPropertiesCommonDlg::OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, &CPropertiesCommonDlg::OnUpdateSortProperties)
	ON_WM_SETTINGCHANGE()
	ON_CBN_SELCHANGE(ID_PROPERTIES_CB, &CTrendPropertiesDlg::OnComboBoxSelChange)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, &CTrendPropertiesDlg::OnPropertyChanged)
	ON_BN_CLICKED(IDOK, &CTrendPropertiesDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTrendPropertiesDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTrendPropertiesDlg メッセージ ハンドラー


/*============================================================================*/
/*! CTrendPropertiesDlg

-# ダイアログ生成時に呼ばれる

@param  lpCreateStruct：作成するオブジェクトに関する情報
@retval 0：生成成功 / 1：生成失敗
*/
/*============================================================================*/
int CTrendPropertiesDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_strWndName.IsEmpty())
	{
		CString strTitle;
		strTitle.Format(_T("%s設定"), (LPCTSTR)m_strWndName);
		SetWindowText(strTitle);
	}

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

	// プロパティシート作成
	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rectDummy, this, 2))
	{
		TRACE0("プロパティ グリッドを作成できませんでした\n");
		return -1;      // 作成できない場合
	}

	InitPropList();

	if (m_PropertiesType == ePropertiesType_Trend)
	{
		// 衛星捕捉グラフはコンボボックス切り替えで異なるグラフを設定
		for (int i = 0; i < m_listViewName.size(); i++)
		{
			SetNewPropertyData(i, m_listViewName[i], (LPARAM)GetParent());
		}
		// 衛星捕捉グラフ設定
		m_wndObjectCombo.SetCurSel(m_nViewID);
		m_bModified = FALSE;
	}
	else
	{
		// サマリグラフはコンボボックスはリスト全てセットしておくが、プロパティ設定はカレントのみ
		for (int i = 0; i < m_listViewName.size(); i++)
		{
			m_wndObjectCombo.InsertString(-1, m_listViewName[i]);
			m_wndObjectCombo.SetCurSel(0);
		}

		// プロパティ変更
		ChangePropaties();
		// 変更を通知
//		SendPropertiesChange();

		// サマリ設定
		CPropertiesData& prop = GetPropertiesData(m_nPropertiesID);
		m_wndObjectCombo.SetCurSel(prop.nGraphIdx);

		ShowProperties();
		m_bModified = TRUE;
	}

	AdjustLayout();

	UpdatePropatyDataList();

	// プロパティをバックアップする
	BackupPropertiesData();

	return 0;
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# レイアウトを調整する

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrendPropertiesDlg::AdjustLayout()
{
	if (GetSafeHwnd() == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient, rectCombo;

	GetClientRect(rectClient);
	rectClient.bottom -= mOKButtonAreaHeight;
	m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyTlb = 4;
	int cyCmb = rectCombo.Size().cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);

	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() - (m_nComboHeight + cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# プロパティリストを初期化する

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrendPropertiesDlg::InitPropList()
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
	//	CMFCPropertyGridProperty* pPropViewMode = new CMFCPropertyGridProperty(_T("表示タイプ"), (variant_t)initPropData.ViewMode, _T("リアルタイム/ノンリアルタイム"), idViewMode);
	CMFCPropertyGridProperty* pPropViewMode = new CMFCPropertyGridProperty(_T("表示タイプ"), (variant_t)initPropData.ViewMode, _T("リアルタイム"), idViewMode);
	pPropViewMode->AddOption(_T("リアルタイム"));
	//	pPropViewMode->AddOption(_T("ノンリアルタイム"));
	pPropViewMode->AllowEdit(FALSE);
	pPropView->AddSubItem(pPropViewMode);

//	// 開始日時
//	CCustomDlgProp* pPropViewDate = new CCustomDlgProp(_T("開始日時"), (variant_t)_T(""), _T("データ表示開始日時"), idViewDate);
//	pPropView->AddSubItem(pPropViewDate);
//	pPropViewDate->Enable(FALSE);
//	pPropViewDate->SetParent(this);

	m_wndPropList.AddProperty(pPropView);

	//------------------------------------------------------
	// 縦軸設定
	CMFCPropertyGridProperty* pPropV = new CMFCPropertyGridProperty(_T("縦軸設定"));

	// 縦軸設定-表示スケール(左)
	CMFCPropertyGridProperty* pPropVL = new CMFCPropertyGridProperty(_T("表示スケール"));
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

	// 縦軸設定-目盛りの間隔
	CMFCPropertyGridProperty* pPropVM = new CMFCPropertyGridProperty(_T("目盛りの間隔"), initPropData.VSCalItvl, _T("目盛りの間隔"), idVAxisScaleInterval);
	pPropVM->EnableSpinControl(TRUE, AXIS_SCALE_MIN, AXIS_SCALE_MAX);
	pPropV->AddSubItem(pPropVM);

	m_wndPropList.AddProperty(pPropV);


	//------------------------------------------------------
	// 横軸設定
	CMFCPropertyGridProperty* pPropH = new CMFCPropertyGridProperty(_T("横軸設定"));

	// 横軸設定-表示スケール(上)
	CMFCPropertyGridProperty* pPropHSU = new CMFCPropertyGridProperty(_T("表示スケール"), (variant_t)initPropData.HScaleU, _T("トレンドグラフビュー1の表示スケール"), idHUAxisScale);
	for (i = 0; strHScale[i] != _T(""); ++i)
	{
		pPropHSU->AddOption(strHScale[i]);
	}
	pPropHSU->AllowEdit(FALSE);
	pPropH->AddSubItem(pPropHSU);

	m_wndPropList.AddProperty(pPropH);

	//-------------------------------------------------------------------------------------------------
	// データ設定
	for (int i = 0; i < m_nListMax; ++i){
		// データX
		CString str;
		str.Format(_T("データ%02d"), i + 1);
		CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(str);

		// データ選択
		CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(_T("データ選択"), _T(""), _T("データの選択"), ((idMax - idAxisMax)*i) + idSelectItem);
		pItem->AddOption(_T(""));
		pItem->AllowEdit(FALSE);
		if (theApp.GetServiceMode() != eServiceMode_oldremote)
		{
			// 旧リモート化モードで無い場合、監視の変更を不可にする
			pItem->Enable(FALSE);
		}
		pGroup->AddSubItem(pItem);

		// 名称
		str.Format(_T("Data%02d"), i + 1);
		pItem = new CMFCPropertyGridProperty(_T("名称"), str, _T("データ名称"), ((idMax - idAxisMax)*i) + idItemName);
		pGroup->AddSubItem(pItem);

		// 単位
		pItem = new CMFCPropertyGridProperty(_T("単位"), (COleVariant)_T("100"), _T("データの単位"), ((idMax - idAxisMax)*i) + idItemUnit);
		pItem->Enable(TRUE);
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

		// 線幅
		pItem = new CMFCPropertyGridProperty(_T("線幅"), initPropData.lineWidth, _T("1～10"), ((idMax - idAxisMax)*i) + idLineWidth);
		pItem->EnableSpinControl(TRUE, 1, 10);
		pItem->Enable(true);
		pItem->AllowEdit(TRUE);
		pGroup->AddSubItem(pItem);

		// ドットサイズ
		pItem = new CMFCPropertyGridProperty(_T("ドットサイズ"), (variant_t)initPropData.DotSize, _T("Large/Medium/Small"), ((idMax - idAxisMax)*i) + idDotSize);
		pItem->AddOption(_T("Small"));
		pItem->AddOption(_T("Medium"));
		pItem->AddOption(_T("Large"));
		pItem->Enable(false);
		pItem->AllowEdit(FALSE);
		pGroup->AddSubItem(pItem);

//		//------------------------------------------------------
//		// コメント
//		CMFCPropertyGridProperty* pGroupCm = new CMFCPropertyGridProperty(_T("コメント"));
//
//		// コメント
//		pItem = new CMFCPropertyGridProperty(_T("コメント"), (COleVariant)_T(""), _T("グラフ上のデータコメント"), ((idMax - idAxisMax)*i) + idComment);
//		pGroupCm->AddSubItem(pItem);
//
//		// コメント表示
//		pItem = new CMFCPropertyGridProperty(_T("表示"), (variant_t)_T("Off"), _T("コメントのON/OFF"), ((idMax - idAxisMax)*i) + idCommentEnable);
//		pItem->AddOption(_T("On"));
//		pItem->AddOption(_T("Off"));
//		pItem->AllowEdit(FALSE);
//		pGroupCm->AddSubItem(pItem);
//
//		pGroupCm->Expand(FALSE);
//		pGroup->AddSubItem(pGroupCm);
//
//		//------------------------------------------------------
//		// アラーム
//		CMFCPropertyGridProperty* pGroupAl = new CMFCPropertyGridProperty(_T("アラーム"));
//
//		// アラーム線表示
//		pItem = new CMFCPropertyGridProperty(_T("表示"), (variant_t)_T("Off"), _T("アラーム線表示のON/OFF"), ((idMax - idAxisMax)*i) + idAlarmEnable);
//		pItem->AddOption(_T("On"));
//		pItem->AddOption(_T("Off"));
//		pItem->AllowEdit(FALSE);
//		pGroupAl->AddSubItem(pItem);
//
//		// アラーム1 上限
//		pItem = new CMFCPropertyGridProperty(_T("アラーム1上限"), initPropData.Alarm1Major, _T("アラーム値1上限"), ((idMax - idAxisMax)*i) + idAlarm1Major);
//		pItem->Enable(FALSE);
//		pGroupAl->AddSubItem(pItem);
//
//		// アラーム1 下限
//		pItem = new CMFCPropertyGridProperty(_T("アラーム1下限"), initPropData.Alarm1Minor, _T("アラーム1下限"), ((idMax - idAxisMax)*i) + idAlarm1Minor);
//		pItem->Enable(FALSE);
//		pGroupAl->AddSubItem(pItem);
//
//		// アラーム2 上限
//		pItem = new CMFCPropertyGridProperty(_T("アラーム2上限"), initPropData.Alarm2Major, _T("アラーム2上限"), ((idMax - idAxisMax)*i) + idAlarm2Major);
//		pItem->Enable(FALSE);
//		pGroupAl->AddSubItem(pItem);
//
//		// アラーム2 下限
//		pItem = new CMFCPropertyGridProperty(_T("アラーム2下限"), initPropData.Alarm2Minor, _T("アラーム2下限"), ((idMax - idAxisMax)*i) + idAlarm2Minor);
//		pItem->Enable(FALSE);
//		pGroupAl->AddSubItem(pItem);
//
//		pGroupAl->Expand(FALSE);
//
//		pGroup->AddSubItem(pGroupAl);

		m_wndPropList.AddProperty(pGroup);
	}
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# 画面選択変更
画面選択が変更された時に、プロパティをその画面用に更新する。

@param	bUpdate：プロパティアップデートフラグ
@retval	なし

*/
/*============================================================================*/
void CTrendPropertiesDlg::ChangePropaties(BOOL bUpdate)
{
	CString str = _T("");

	// プロパティデータ取得
	CPropertiesData& prop = GetPropertiesData(m_nPropertiesID);

	// 表示設定
	CMFCPropertyGridProperty* pView = m_wndPropList.GetProperty(idViewGr);

	// 表示設定-表示タイプ
	CMFCPropertyGridProperty* pViewMode = pView->GetSubItem(idViewGrMode);
	str = pViewMode->GetName();
	if (prop.isRealTime)
	{
		pViewMode->SetValue(_T("リアルタイム"));
	}
//	else
//	{
//		pViewMode->SetValue(_T("ノンリアルタイム"));
//	}

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

	// 縦軸設定-表示スケール(左)-最小値
	CMFCPropertyGridProperty* pVAxisLMin = pVAxis->GetSubItem(idVAxisGrScalLGr)->GetSubItem(idVAxisGrScalLGrMin);
	pVAxisLMin->SetValue((double)prop.trandVScale.VScalLMin);

	if (prop.trandVScale.Auto)
	{
		pVAxisLMax->Enable(FALSE);
		pVAxisLMin->Enable(FALSE);
	}
	else
	{
		pVAxisLMax->Enable(TRUE);
		pVAxisLMin->Enable(TRUE);
	}
	// 縦軸設定-目盛りの間隔
	CMFCPropertyGridProperty* pVAxisScaleItvl = pVAxis->GetSubItem(idVAxisGrScalInvl);
	pVAxisScaleItvl->SetValue((long)prop.trandVScale.VSCalItvl);


	// 横軸設定
	CMFCPropertyGridProperty* pHAxis = m_wndPropList.GetProperty(idHAxisGr);

	// 横軸設定-表示スケール(上)
	CMFCPropertyGridProperty* pHAxisScaleU = pHAxis->GetSubItem(idHAxisGrScaleU);
	pHAxisScaleU->SetValue((variant_t)(strHScale[prop.trandHScale[0]]));

	// データ
	for (int i = 0; i < m_nListMax; ++i)
	{
		CMFCPropertyGridProperty* pData = m_wndPropList.GetProperty(idDataGr + i);

		// データ名称
		CMFCPropertyGridProperty* pItem = pData->GetSubItem(idDataGrDataSelect);
		pItem->SetValue(prop.dataName[i]);
		str = pItem->GetName();

		// データ名称（グラフ用）
		pItem = pData->GetSubItem(idDataGrName);
		pItem->SetValue(prop.dataGName[i]);
		str = pItem->GetName();

		// 単位
		pItem = pData->GetSubItem(idDataGrItemUnit);
		pItem->SetValue(prop.dataUnit[i]);
		str = pItem->GetName();

		// 表示
		pItem = pData->GetSubItem(idDataGrDisplay);
		str = pItem->GetName();
		if (prop.isViewMode[i])
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
		if (prop.drawMode[i] == Line)
		{
			pItem->SetValue(_T("Line"));
		}
		else if (prop.drawMode[i] == Dots)
		{
			pItem->SetValue(_T("Dots"));
		}
		else if (prop.drawMode[i] == LineAndDots)
		{
			pItem->SetValue(_T("L&D"));
		}

		// 表示色
		CMFCPropertyGridColorProperty* pGColorProp = (CMFCPropertyGridColorProperty*)pData->GetSubItem(idDataGrColor);
		pGColorProp->SetColor(prop.color[i]);
		str = pGColorProp->GetName();

		// 線幅
		CMFCPropertyGridProperty* pGLineWidth = pData->GetSubItem(idDataGrLineWidth);
		pGLineWidth->SetValue((long)prop.lineWidth[i]);

		// ドットサイズ
		pItem = pData->GetSubItem(idDataGrDotSiz);
		str = pItem->GetName();
		if (prop.dotSize[i] == Small)
		{
			pItem->SetValue(_T("Small"));
		}
		else if (prop.dotSize[i] == Medium)
		{
			pItem->SetValue(_T("Medium"));
		}
		else if (prop.dotSize[i] == Large)
		{
			pItem->SetValue(_T("Large"));
		}

		if (prop.drawMode[i] == Line)
		{
			// 非活性
			pItem->Enable(FALSE);
		}
		else
		{
			// 活性
			pItem->Enable(TRUE);
		}

//		// コメント
//		CMFCPropertyGridProperty* pComment = pData->GetSubItem(idDataGrComment);
//
//		pItem = pComment->GetSubItem(idDataGrCommentStr);
//		str = pItem->GetName();
//		pItem->SetValue(prop.comment[i]);
//
//		// コメント表示
//		pItem = pComment->GetSubItem(idDataGrCommentEnable);
//		if (prop.isComment)
//		{
//			pItem->SetValue(_T("On"));
//		}
//		else
//		{
//			pItem->SetValue(_T("Off"));
//		}
//
//		// アラーム表示
//		CMFCPropertyGridProperty* pAlarm = pData->GetSubItem(idDataGrAlarm);
//		pItem = pAlarm->GetSubItem(idDataGrAlarmEnable);
//		if (prop.isAlarm)
//		{
//			pItem->SetValue(_T("On"));
//		}
//		else
//		{
//			pItem->SetValue(_T("Off"));
//		}
//
//		// アラーム値1 上限
//		pItem = pAlarm->GetSubItem(idDataGrAlarm1Major);
//		pItem->SetValue((double)prop.alarm1Major[i]);
//
//		// アラーム値1 下限
//		pItem = pAlarm->GetSubItem(idDataGrAlarm1Minor);
//		pItem->SetValue((double)prop.alarm1Minor[i]);
//
//		// アラーム値2 上限
//		pItem = pAlarm->GetSubItem(idDataGrAlarm2Major);
//		pItem->SetValue((double)prop.alarm2Major[i]);
//
//		// アラーム値2 下限
//		pItem = pAlarm->GetSubItem(idDataGrAlarm2Minor);
//		pItem->SetValue((double)prop.alarm2Minor[i]);
	}

	// プロパティデータセット
	if (bUpdate)
	{
		SetPropertiesData(m_nPropertiesID, prop);
	}
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# データ選択用リスト更新
データ選択用リストを更新する。

@param	なし
@retval	なし

*/
/*============================================================================*/
void CTrendPropertiesDlg::UpdatePropatyDataList()
{
	map<DWORD, CString>& data = theApp.GetDataCollection().GetDataNameList(theApp.GetSelectStation());
	vector<stSelectData>::iterator itr;

	m_DataNameList.RemoveAll();
	for (auto itr = data.begin(); itr != data.end(); ++itr)
	{
		m_DataNameList.AddTail((*itr).second);
	}

	for (int i = 0; i < m_nListMax; ++i)
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

			if (!bFind && (name == sName))
			{
				bFind = TRUE;
			}
		}

		// 現在選択中のデータがリストに無いので、表示を削除する。
		if (!bFind)
		{
			pItem->SetValue(_T(""));
		}
	}

#ifdef _NOCOMM
	// プロパティ変更
	ChangePropaties();
#endif


}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# プロパティデータを取得する

@param	pos：ビュー位置
@retval	プロパティデータ

*/
/*============================================================================*/
CPropertiesData&  CTrendPropertiesDlg::GetPropertiesData(int pos)
{
	if (m_PropertiesType == ePropertiesType_Trend)
		return theApp.GetSatelliteData().GetTrendPropertiesData(pos);
	else
		return theApp.GetSatelliteData().GetSummaryPropertiesData(m_nStationID, pos);
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# プロパティデータをセットする

@param	pos：ビュー位置
@param	val：プロパティデータ
@retval	なし

*/
/*============================================================================*/
void CTrendPropertiesDlg::SetPropertiesData(int pos, CPropertiesData& val)
{
	if (m_PropertiesType == ePropertiesType_Trend)
		theApp.GetSatelliteData().SetTrendPropertiesData(pos, val);
	else
		theApp.GetSatelliteData().SetSummaryPropertiesData(m_nStationID, pos, val);
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# プロパティ変更をグラフに通知する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CTrendPropertiesDlg::SendPropertiesChange()
{
	if (m_PropertiesType == ePropertiesType_Trend)
		// 変更を通知
		theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 0, (LPARAM)m_nViewID);
	else
		// 変更を通知
		theApp.m_pMainWnd->PostMessage(eMessage_SummaryPropetyChange, (WPARAM)m_nParentID, (LPARAM)m_nViewID);
}

/*============================================================================*/
/*! CTrendPropertiesDlg

-# OKボタンクリック時に呼ばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CTrendPropertiesDlg::OnBnClickedOk()
{
	// 確認ダイアログ表示
	CString title;
	GetWindowText(title);

	if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("実行します。\nよろしいですか？"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return;

//	// トレンド設定を保存する
//	theApp.GetSatelliteData().TrendPropWriteData(m_nStationID);

	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_wndPropList.RemoveAll();

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# キャンセルボタンクリック時に呼ばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CTrendPropertiesDlg::OnBnClickedCancel()
{
	if (!m_bChgGraphPropDlg)
		// バックアップしたプロパティを元に戻す
		RestorePropertiesData();

	m_wndPropList.RemoveAll();

	CDialogBase::OnCancel();
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# プロパティデータをバックアップする

@param	なし
@retval	なし

*/
/*============================================================================*/
void CTrendPropertiesDlg::BackupPropertiesData()
{
	// プロパティデータを戻す
	if (m_PropertiesType == ePropertiesType_Trend)
	{
		// プロパティデータ取得B
		for (int i = 0; i < eTrendWndType_Num; i++)
		{
			m_backupPropertiesData[i] = GetPropertiesData(i);
		}
	}
	else
	{
		// プロパティデータ取得
		for (int i = 0; i < MAX_SUMMARY_VIEW; i++)
		{
			m_backupSummaryPropertiesData[i] = GetPropertiesData(i);
		}
	}
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# プロパティデータを復元する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CTrendPropertiesDlg::RestorePropertiesData()
{
	// プロパティデータを復元する
	if (m_PropertiesType == ePropertiesType_Trend)
	{
		for (int i = 0; i < eTrendWndType_Num; i++)
		{
			SetPropertiesData(i, m_backupPropertiesData[i]);
			if (m_bModified)
			{
				// 変更を通知
				theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 0, (LPARAM)i);
			}
		}
	}
	else
	{
		for (int i = 0; i < MAX_SUMMARY_VIEW; i++)
		{
			SetPropertiesData(i, m_backupSummaryPropertiesData[i]);
			if (m_bModified)
			{
				// 変更を通知
				theApp.m_pMainWnd->PostMessage(eMessage_SummaryPropetyChange, (WPARAM)m_nParentID, (LPARAM)i);
			}
		}
	}
	m_bModified = FALSE;
}

/*============================================================================*/
/*! CTrendPropertiesDlg

-# プロパティ表示をアクティブウィンドウ用に変更
表示されているウィンドウが変更されたら、プロパティー表示を変更する。

@param	pos：グラフビュー番号
@retval	なし

*/
/*============================================================================*/
void CTrendPropertiesDlg::ChangeView(int pos)
{
	CPropertiesData& prop = GetPropertiesData(pos);
	m_wndObjectCombo.SelectString(0, prop.wndName);
	m_nPropertiesID = pos;

	// プロパティ変更
	ChangePropaties();
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# コンボボックス選択変更時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrendPropertiesDlg::OnComboBoxSelChange()
{
	// トレンドの時
	if (m_PropertiesType == ePropertiesType_Trend)
	{
		int pos = -1;
		CString name;
		m_wndObjectCombo.GetWindowTextW(name);
		for (int i = 0; i < eTrendWndType_Num; ++i)
		{
			CPropertiesData& prop = GetPropertiesData(i);

			if (name == prop.wndName)
			{
				pos = prop.nGraphIdx;
				break;
			}
		}

		if (pos == -1)
			return;

		// コンボで指定したビューに切り替える
		ChangeView(pos);
	}
	// サマリの時
	else
	{
		m_bModified = TRUE;
		CPropertiesData& prop = GetPropertiesData(m_nPropertiesID);

		int nCurSel = m_wndObjectCombo.GetCurSel();

		// スペクトラム選択時は、設定ダイアログを変える。
		if (nCurSel == eSummaryWndSetType_XSP && m_nStationID != eStation_Uchinoura20 ||
			nCurSel == eSummaryWndSetType_KASP && m_nStationID == eStation_Usuda642)
		{
			CString title;
			GetWindowText(title);
			if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("スペクトラム設定ウィンドウに切り替えます。\nよろしいですか？"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
				return;

			prop.nGraphIdx = nCurSel;

			CRect rect;
			GetWindowRect(&rect);
			theApp.SetWindowTmpPoint(rect.left, rect.top);

			// トレンドグラフ切り替え通知
			theApp.m_pMainWnd->PostMessage(eMessage_InitSummaryWnd, (WPARAM)m_nParentID, (LPARAM)m_nViewID);
			// サマリスペクトラムプロパティダイアログを開く通知
			theApp.m_pMainWnd->PostMessage(eMessage_OpenSummarySpectrumSetDlgChg, (WPARAM)m_pSummaryProp, (LPARAM)this);

			// プロパティデータセット
			SetPropertiesData(m_nPropertiesID, prop);
			ShowProperties();

			// プロパティ変更
			ChangePropaties();
			// プロパティ変更通知
			SendPropertiesChange();
//			// プロパティ保存
//			theApp.GetSatelliteData().TrendPropWriteData(m_nStationID);

			// トレンド設定ダイアログを閉じる
			m_bChgGraphPropDlg = TRUE;
			PostMessage(WM_CLOSE);
		}
		// トレンドの場合はプロパティ変更
		else
		{
			prop.nGraphIdx = nCurSel;
			for (int i = 0; i < MAX_SUMMARYTDATA; i++)
			{
				prop.dataName[i] = mListSummarySig[m_nStationID][nCurSel][i].strSigName;
				prop.dataGName[i] = mListSummarySig[m_nStationID][nCurSel][i].strSimpleSigName;
				prop.dataUnit[i] = mListSummarySig[m_nStationID][nCurSel][i].strUnit;
			}

			// トレンドグラフ切り替え通知
			theApp.m_pMainWnd->PostMessage(eMessage_InitSummaryWnd, (WPARAM)m_nParentID, (LPARAM)m_nViewID);
			// プロパティデータセット
			SetPropertiesData(m_nPropertiesID, prop);
			ShowProperties();

			// プロパティ変更
			ChangePropaties();
			// プロパティ変更通知
			SendPropertiesChange();
		}
	}
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# プロパティを表示する

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrendPropertiesDlg::ShowProperties()
{
	int nSel = m_wndObjectCombo.GetCurSel();
	// ミニスペクトラムの場合
	if (nSel == eSummaryWndSetType_XSP && m_nStationID != eStation_Uchinoura20 ||
		nSel == eSummaryWndSetType_KASP && m_nStationID == eStation_Usuda642)
	{
		// 非表示
		m_wndPropList.ShowWindow(SW_HIDE);
	}
	// それ以外の場合
	else
	{
		// 表示
		m_wndPropList.ShowWindow(SW_SHOW);
	}
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# ウィンドウを閉じる時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrendPropertiesDlg::OnClose()
{
	if (!m_bChgGraphPropDlg)
	{
		CString title;
		GetWindowText(title);

		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("終了します。\nよろしいですか？"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return;
	}

	CDialogEx::OnClose();
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# ダイアログ初期化時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CTrendPropertiesDlg::OnInitDialog()
{
	CPropertiesCommonDlg::OnInitDialog();

	if (m_bChgDlg)
	{
		CPoint pt = theApp.GetWindowTmpPoint();
		SetWindowPos(NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
	else
	{
		CenterWindowEx();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# プロパティ変更時に呼ばれる

@param  wParam
@param  lParam
@retval なし
*/
/*============================================================================*/
LRESULT CTrendPropertiesDlg::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	m_bModified = TRUE;

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
	CPropertiesData& prop = GetPropertiesData(m_nPropertiesID);

	switch (data){
	case idViewMode:	// 表示タイプ
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("リアルタイム")) == 0)
		{
			prop.isRealTime = true;
			CCustomDlgProp* pStartDate = (CCustomDlgProp*)pProp->GetParent()->GetSubItem(idViewGrDate);
			pStartDate->Enable(FALSE);
		}
		break;

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
		}
		else
		{
			prop.trandVScale.Auto = false;

			// 最大値、最小値を活性にする。
			CMFCPropertyGridProperty* pMax = pProp->GetParent()->GetSubItem(idVAxisGrScalLGrMax);
			pMax->Enable(TRUE);
			CMFCPropertyGridProperty* pMin = pProp->GetParent()->GetSubItem(idVAxisGrScalLGrMin);
			pMin->Enable(TRUE);
		}
		break;

	case idVLAxisMaxRange:	// 縦軸左最大値
		dval = pProp->GetValue().dblVal;
		if (dval > 10000000000000)
		{
			dval = 9999999999999.999999;
			pProp->SetValue(dval);
		}
		else if (dval < -10000000000000)
		{
			dval = -9999999999999.999999;
			pProp->SetValue(dval);
		}
		prop.trandVScale.VScalLMax = dval;
		break;

	case idVLAxisMinRange:	// 縦軸左最小値
		dval = pProp->GetValue().dblVal;
		if (dval > 10000000000000)
		{
			dval = 9999999999999.999999;
			pProp->SetValue(dval);
		}
		else if (dval < -10000000000000)
		{
			dval = -9999999999999.999999;
			pProp->SetValue(dval);
		}
		prop.trandVScale.VScalLMin = dval;
		break;

	case idVAxisScaleInterval:	// 縦軸目盛りの間隔
		lval = pProp->GetValue().lVal;
		if (lval < AXIS_SCALE_MIN)
		{
			lval = AXIS_SCALE_MIN;
			pProp->SetValue(lval);
			pProp->Redraw();
		}
		else if (lval > AXIS_SCALE_MAX)
		{
			lval = AXIS_SCALE_MAX;
			pProp->SetValue(lval);
			pProp->Redraw();
		}
		prop.trandVScale.VSCalItvl = lval;
		break;

	case idHUAxisScale:		// 横軸上スケール
		{
			str = pProp->GetValue().bstrVal;
			HScale hscale = (HScale)prop.getHScal(str);
			// トレンドの時
			if (m_PropertiesType == ePropertiesType_Trend)
			{
				// 横軸スケールが変更の場合
				if (prop.trandHScale[0] != hscale)
				{
					// 自プロパティのスケール変更
					prop.trandHScale[0] = hscale;
					// 全てのプロパティの横軸を合わせて変更
					for (int i = 0; i < eTrendWndType_Num; ++i)
					{
						if (i == m_nPropertiesID)
							continue;
	
						CPropertiesData& propOther = GetPropertiesData(i);
						propOther.trandHScale[0] = hscale;
					}
				}
			}
			else
			{
				// 自プロパティのスケール変更
				prop.trandHScale[0] = hscale;
			}
		}
		break;

	case idHLAxisScale:		// 横軸下スケール
		str = pProp->GetValue().bstrVal;
		prop.trandHScale[1] = (HScale)prop.getHScal(str);
		break;

	case idSelectItem:	// データ選択
		str = pProp->GetValue().bstrVal;
		prop.dataName[pos] = str;
		break;

	case idItemName:	// 名称
		str = pProp->GetValue().bstrVal;
		prop.dataGName[pos] = str;
		break;

	case idDisplay:	// 表示
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("On")) == 0)
		{
			prop.isViewMode[pos] = true;
		}
		else
		{
			prop.isViewMode[pos] = false;
		}
		break;

	case idDrawKind:	// 描画モード
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("Line")) == 0)
		{
			prop.drawMode[pos] = Line;

			// 線幅変更可
			CMFCPropertyGridProperty* pLineProp = pProp->GetParent()->GetSubItem(idDataGrLineWidth);
			pLineProp->Enable(TRUE);
			// ドットサイズ 変更不可
			CMFCPropertyGridProperty* pDotProp = pProp->GetParent()->GetSubItem(idDataGrDotSiz);
			pDotProp->Enable(FALSE);
		}
		else if (str.Compare(_T("Dots")) == 0)
		{
			prop.drawMode[pos] = Dots;

			// 線幅変更不可
			CMFCPropertyGridProperty* pLineProp = pProp->GetParent()->GetSubItem(idDataGrLineWidth);
			pLineProp->Enable(FALSE);
			// ドットサイズ 変更可
			CMFCPropertyGridProperty* pDotProp = pProp->GetParent()->GetSubItem(idDataGrDotSiz);
			pDotProp->Enable(TRUE);
		}
		else if (str.Compare(_T("L&D")) == 0)
		{
			prop.drawMode[pos] = LineAndDots;

			// 線幅変更可
			CMFCPropertyGridProperty* pLineProp = pProp->GetParent()->GetSubItem(idDataGrLineWidth);
			pLineProp->Enable(TRUE);
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
			prop.color[pos] = colordef[pos];
			pColor->SetColor(prop.color[pos]);
		}
		else
		{
			prop.color[pos] = color;
		}
	}
	break;

	case idLineWidth:	// 線幅
		lval = pProp->GetValue().lVal;
		if (lval > 10)
		{
			lval = 10;
			pProp->SetValue(lval);
		}
		else if (lval < 1)
		{
			lval = 1;
			pProp->SetValue(lval);
		}
		prop.lineWidth[pos] = lval;
		break;

	case idDotSize:		// ドットサイズ
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("Small")) == 0)
		{
			prop.dotSize[pos] = Small;
		}
		else if (str.Compare(_T("Medium")) == 0)
		{
			prop.dotSize[pos] = Medium;
		}
		else if (str.Compare(_T("Large")) == 0)
		{
			prop.dotSize[pos] = Large;
		}
		break;

	case idVScale:		// 横軸スケール選択
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("左")) == 0)
		{
			prop.vscaleKind[pos] = LScale;
		}
		else
		{
			prop.vscaleKind[pos] = RScale;
		}
		break;

	case idComment:			// コメント
		str = pProp->GetValue().bstrVal;
		prop.comment[pos] = str;
		break;

	case idCommentEnable:	// コメント表示
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("On")) == 0)
		{
			prop.isComment[pos] = true;
		}
		else
		{
			prop.isComment[pos] = false;
		}
		break;

	case idAlarmEnable:		// アラーム表示
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("On")) == 0)
		{
			prop.isAlarm[pos] = true;
		}
		else
		{
			prop.isAlarm[pos] = false;
		}
		break;
	}

	// プロパティデータセット
	SetPropertiesData(m_nPropertiesID, prop);

	// 変更を通知
	SendPropertiesChange();

	return 0;
}
