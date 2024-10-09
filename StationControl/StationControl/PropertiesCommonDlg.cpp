/*============================================================================*/
/*! PropertiesCommonDlg.cpp

-# 衛星捕捉グラフ、サマリグラフ共通設定画面
*/
/*============================================================================*/
// PropertiesCommonDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "PropertiesCommonDlg.h"
#include "afxdialogex.h"


/////////////////////////////////////////////////////////////////////////////
// CCustomDlgProp

CCustomDlgProp::CCustomDlgProp(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData) : CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData)
{
}

void CCustomDlgProp::OnClickButton(CPoint /*point*/)
{
}

// CPropertiesCommonDlg ダイアログ

IMPLEMENT_DYNAMIC(CPropertiesCommonDlg, CDialogBase)

/*============================================================================*/
/*! CPropertiesCommonDlg

-# コンストラクタ

@param	IDD：IDD
@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CPropertiesCommonDlg::CPropertiesCommonDlg(UINT IDD, CWnd* pParent /*=NULL*/)
: CDialogBase(IDD, pParent)
{
	m_nComboHeight = 0;
	m_nViewID = 0;
	m_nPropertiesID = 0;
}

/*============================================================================*/
/*! CPropertiesCommonDlg

-# コンストラクタ

@param  pParent：親ウィンドウ
@param	nViewID：グラフビューID
@param	strViewName：グラフビュー名
@retval なし
*/
/*============================================================================*/
CPropertiesCommonDlg::CPropertiesCommonDlg(UINT IDD, CWnd* pParent, UINT nViewID, vector<CString>& listViewName, UINT nPropertiesID)
	: CDialogBase(IDD, pParent)
	, m_nViewID(nViewID)
	, m_listViewName(listViewName)
	, m_nPropertiesID(nPropertiesID)
	, m_nComboHeight(0)
{
}


/*============================================================================*/
/*! CPropertiesCommonDlg

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CPropertiesCommonDlg::~CPropertiesCommonDlg()
{
}

void CPropertiesCommonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropertiesCommonDlg, CDialogBase)
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()


// CPropertiesCommonDlg メッセージ ハンドラー

/*============================================================================*/
/*! CPropertiesCommonDlg

-# プロパティを全展開時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CPropertiesCommonDlg::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

/*============================================================================*/
/*! CPropertiesCommonDlg

-# プロパティの全展開情報更新時に呼ばれる

@param  CCmdUI：ユーザインターフェイスアイテム
@retval なし
*/
/*============================================================================*/
void CPropertiesCommonDlg::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

/*============================================================================*/
/*! CPropertiesCommonDlg

-# プロパティをソート時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CPropertiesCommonDlg::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

/*============================================================================*/
/*! CPropertiesCommonDlg

-# プロパティのソート情報更新時に呼ばれる

@param  CCmdUI：ユーザインターフェイスアイテム
@retval なし
*/
/*============================================================================*/
void CPropertiesCommonDlg::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}


/*============================================================================*/
/*! CPropertiesCommonDlg

-# フォーカス時に呼ばれる

@param  pOldWnd：入力フォーカスを失ったCWndオブジェクト
@retval なし
*/
/*============================================================================*/
void CPropertiesCommonDlg::OnSetFocus(CWnd* pOldWnd)
{
	m_wndPropList.SetFocus();
}


/*============================================================================*/
/*! CPropertiesCommonDlg

-# プロパティリストにフォントを設定する

@param  なし
@retval なし
*/
/*============================================================================*/
void CPropertiesCommonDlg::SetPropListFont()
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
	//	m_wndTabs.SetFont(&m_fntPropList);
	//	m_wndList.SetFont(&m_fntPropList);
}


/*============================================================================*/
/*! CPropertiesCommonDlg

-# プロパティ変更時に呼ばれる

@param  wParam
@param  lParam
@retval なし
*/
/*============================================================================*/
LRESULT CPropertiesCommonDlg::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
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
		str = pProp->GetValue().bstrVal;
		prop.trandHScale[0] = (HScale)prop.getHScal(str);
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

/*============================================================================*/
/*! CPropertiesCommonDlg

-# プロパティウィンドウデータ新規作成
グラフウィンドウが作成された時に、プロパティデータを作成する。

@param	pos：グラフビュー番号
@param	name：グラフビュー名
@param	wnd：ウィンドウハンドラ
@retval	なし

*/
/*============================================================================*/
void CPropertiesCommonDlg::SetNewPropertyData(int pos, CString name, LPARAM wnd)
{
	m_wndObjectCombo.InsertString(-1, name);
	m_wndObjectCombo.SetCurSel(0);

	// プロパティ変更
	ChangePropaties();

//	// 変更を通知
//	SendPropertiesChange();

	return;
}


/*============================================================================*/
/*! CPropertiesCommonDlg

-# プロパティウィンドウデータ削除
ウィンドウが終了された時に、プロパティ画面選択から削除する。

@param	pos：グラフビュー番号
@retval	なし

*/
/*============================================================================*/
void CPropertiesCommonDlg::CloseView(int pos)
{
	// ”画面選択”用リスト更新
	CPropertiesData& prop = GetPropertiesData(pos);
	m_wndObjectCombo.DeleteString(m_wndObjectCombo.FindString(0, prop.wndName));
}


/*============================================================================*/
/*! CPropertiesCommonDlg

-# データリストより指定されたインデックスのデータ名を取得する

@param	idx：インデックス
@retval	データ名

*/
/*============================================================================*/
CString CPropertiesCommonDlg::GetDataSelectNameAt(int idx)
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
/*! CPropertiesCommonDlg

-# 画面選択コンボボックスリスト再設定

@param	vBiew	：表示状態
@param	name	：表示されているグラフ名
@retval	なし

*/
/*============================================================================*/
void CPropertiesCommonDlg::ExchangeViewCombo(BOOL *vBiew, CString name)
{
	// 画面選択 アイテム再作成
	int nCmb = m_wndObjectCombo.GetCount();
	m_wndObjectCombo.ResetContent();

	nCmb = m_wndObjectCombo.GetCount();


	CString strView = _T("");
	for (int i = 0; i < MAX_VIEW; ++i)
	{
		CPropertiesData& prop = GetPropertiesData(i);
		if (vBiew[i])
		{
			m_wndObjectCombo.AddString(prop.wndName);
		}
	}

	m_wndObjectCombo.SelectString(0, name);
	m_wndObjectCombo.UpdateWindow();
}


/*============================================================================*/
/*! CPropertiesCommonDlg

-# 簡易設定タブ、描画モード取得
描画モードのStringからenum値に変換する。

@param	描画モード
@retval	描画モード番号

*/
/*============================================================================*/
int CPropertiesCommonDlg::GetDrawMode(CString str)
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




