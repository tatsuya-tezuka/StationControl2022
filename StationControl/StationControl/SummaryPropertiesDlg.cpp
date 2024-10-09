/*============================================================================*/
/*! SummaryPropertiesDlg.cpp

-# サマリグラフ設定画面
*/
/*============================================================================*/
// SummaryPropertiesDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "SummaryPropertiesDlg.h"
#include "MFCEqTabCtrl.h"
#include "afxdialogex.h"


/////////////////////////////////////////////////////////////////////////////
// CSummaryPropertiesDlg ダイアログ

IMPLEMENT_DYNAMIC(CSummaryPropertiesDlg, CDialogBase)

/*============================================================================*/
/*! CTrendPropertiesDlg

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CSummaryPropertiesDlg::CSummaryPropertiesDlg(CWnd* pParent /*=NULL*/)
: CDialogBase(CSummaryPropertiesDlg::IDD, pParent)
{
}


/*============================================================================*/
/*! CSummaryPropertiesDlg

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CSummaryPropertiesDlg::~CSummaryPropertiesDlg()
{
}

void CSummaryPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_SUMMARYGRAPTH_PROP_COMBO, m_wndObjectCombo);
//	DDX_Control(pDX, IDC_SUMMARYGRAPTH_PROP_BUTTON, m_btnSetting);
}


BEGIN_MESSAGE_MAP(CSummaryPropertiesDlg, CDialogEx)
//	ON_WM_CREATE()
//	ON_CBN_SELCHANGE(IDC_SUMMARYGRAPTH_PROP_COMBO, &CSummaryPropertiesDlg::OnComboBoxSelChange)
//	ON_BN_CLICKED(IDC_SUMMARYGRAPTH_PROP_BUTTON, &CSummaryPropertiesDlg::OnBnClickedSummarygrapthPropButton)
END_MESSAGE_MAP()


// CSummaryPropertiesDlg メッセージ ハンドラー


/*============================================================================*/
/*! CSummaryPropertiesDlg

-# ダイアログ生成時に呼ばれる

@param	pParentWnd：親ウインドウ
@param  nIDTemplate：ID
@param	nViewID：ビューID
@param	nStaionID：局ID
@param	nParentID：親ウィンドウID
@retval 0：生成成功 / 1：生成失敗
*/
/*============================================================================*/
BOOL CSummaryPropertiesDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nViewID, UINT nStaionID, UINT nParentID /*= 0*/)
{
//	m_nStationID = nStaionID;
//	m_nViewID = nViewID;
//	m_nParentID = nParentID;
//	GetViewNameList(m_listViewName);
	return CDialogBase::Create(nIDTemplate, pParentWnd);
}


/*============================================================================*/
/*! CSummaryPropertiesDlg

-# ダイアログ生成時に呼ばれる

@param  lpCreateStruct：作成するオブジェクトに関する情報
@retval 0：生成成功 / 1：生成失敗
*/
/*============================================================================*/
int CSummaryPropertiesDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBase::OnCreate(lpCreateStruct) == -1)
		return -1;

//	CRect rectDummy;
//	rectDummy.SetRectEmpty();
//
//	// コンボ ボックスの作成:
//	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
//
//	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, ID_PROPERTIES_CB))
//	{
//		TRACE0("プロパティ コンボ ボックスを作成できませんでした\n");
//		return -1;      // 作成できない場合
//	}
//
//	m_wndObjectCombo.SetFont(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));

	return 0;
}


/*============================================================================*/
/*! CTrendGraphView

-# グラフビュー名リストの取得

@param	listViewName：ビュー名リスト
@retval	TRUE:取得成功 / FALSE:取得失敗

*/
/*============================================================================*/
BOOL CSummaryPropertiesDlg::GetViewNameList(vector<CString>& listViewName)
{
	CString str;

	if (str.LoadString(IDS_TREND_RNGGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	if (str.LoadString(IDS_TREND_RRGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	if (str.LoadString(IDS_TREND_XSPGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	if (str.LoadString(IDS_TREND_SAGCGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	if (str.LoadString(IDS_TREND_XAGCGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	return TRUE;
}


/*============================================================================*/
/*! CTrendGraphView

-# グラフビュー名リストの取得（トレンドのみ）

@param	listViewName：ビュー名リスト
@retval	TRUE:取得成功 / FALSE:取得失敗

*/
/*============================================================================*/
BOOL CSummaryPropertiesDlg::GetViewTrendNameList(vector<CString>& listViewName)
{
	CString str;

	if (str.LoadString(IDS_TREND_RNGGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	if (str.LoadString(IDS_TREND_RRGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	if (str.LoadString(IDS_TREND_SAGCGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	if (str.LoadString(IDS_TREND_XAGCGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	return TRUE;
}


///*============================================================================*/
///*! CSummaryPropertiesDlg
//
//-# プロパティデータを取得する
//
//@param	pos：ビュー位置
//@retval	プロパティデータ
//
//*/
///*============================================================================*/
//CPropertiesData&  CSummaryPropertiesDlg::GetPropertiesData(int pos)
//{
//	return theApp.GetSatelliteData().GetSummaryPropertiesData(m_nStationID, pos);
//}


///*============================================================================*/
///*! CSummaryPropertiesDlg
//
//-# プロパティデータをセットする
//
//@param	pos：ビュー位置
//@param	val：プロパティデータ
//@retval	なし
//
//*/
///*============================================================================*/
//void CSummaryPropertiesDlg::SetPropertiesData(int pos, CPropertiesData& val)
//{
////	theApp.GetSatelliteData().SetSummaryPropertiesData(m_nStationID, pos, val);
//}

///*============================================================================*/
///*! CTrendPropertiesDlg
//
//-# プロパティ変更をグラフに通知する
//
//@param	なし
//@retval	なし
//
//*/
///*============================================================================*/
//void CSummaryPropertiesDlg::SendPropertiesChange()
//{
////	// 変更を通知
////	theApp.m_pMainWnd->PostMessage(eMessage_SummaryPropetyChange, (WPARAM)m_nParentID, (LPARAM)m_nViewID);
//}


///*============================================================================*/
///*! CSummaryPropertiesDlg
//
//-# コンボボックス選択を更新する
//
//@param  なし
//@retval なし
//*/
///*============================================================================*/
//void CSummaryPropertiesDlg::UpdateComboBoxSelect()
//{
////	CPropertiesData& prop = GetPropertiesData(m_nViewID);
////	m_wndObjectCombo.SetCurSel(prop.nGraphIdx);
////	UpdateData(FALSE);
//}


///*============================================================================*/
///*! CSummaryPropertiesDlg
//
//-# コンボボックス選択変更時に呼ばれる
//
//@param  なし
//@retval なし
//*/
///*============================================================================*/
//void CSummaryPropertiesDlg::OnComboBoxSelChange()
//{
//	CPropertiesData& prop = GetPropertiesData(m_nViewID);
//
//	int nCurSel = m_wndObjectCombo.GetCurSel();
//
//	for (int i = 0; i < MAX_SUMMARYTDATA; i++)
//	{
//		prop.dataName[i] = mListSummarySig[m_nStationID][nCurSel][i].strSigName;
//		prop.dataGName[i] = mListSummarySig[m_nStationID][nCurSel][i].strSimpleSigName;
//		prop.dataUnit[i] = mListSummarySig[m_nStationID][nCurSel][i].strUnit;
//	}
//	prop.wndName = m_listViewName[nCurSel];
//	prop.nGraphIdx = nCurSel;
//	SetPropertiesData(m_nViewID, prop);
//
//	// プロパティ変更通知
//	SendPropertiesChange();
//}


///*============================================================================*/
///*! CSummaryPropertiesDlg
//
//-# 設定ボタン押下時に呼ばれる
//
//@param  なし
//@retval なし
//*/
///*============================================================================*/
//void CSummaryPropertiesDlg::OnBnClickedSummarygrapthPropButton()
//{
//	// サマリプロパティダイアログを開く通知
//	theApp.m_pMainWnd->PostMessage(eMessage_OpenSummaryTrendSetDlg, (WPARAM)m_nViewID, (LPARAM)this);
//
//	CMFCEqTabCtrl* pWnd = (CMFCEqTabCtrl*)GetParent();
//	if (pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
//	{
//		pWnd->SetActiveTab(0);
//	}
//}


/*============================================================================*/
/*! CSummaryPropertiesDlg

-# 初期化時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CSummaryPropertiesDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

//	for (int i = 0; i < m_listViewName.size(); i++)
//	{
//		m_wndObjectCombo.InsertString(-1, m_listViewName[i]);
//	}
//	CPropertiesData& prop = GetPropertiesData(m_nViewID);
//	m_wndObjectCombo.SetCurSel(prop.nGraphIdx);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


///*============================================================================*/
///*! CSummaryPropertiesDlg
//
//-# サマリコンボボックス更新時に呼ばれる
//
//@param  なし
//@retval なし
//*/
///*============================================================================*/
//LRESULT CSummaryPropertiesDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message){
//
//	case eMessage_UpdateSummaryTabCombo:
//	{
//		// サマリプロパティコンボ変更通知
//		UpdateComboBoxSelect();
//		break;
//	}
//
//	default:
//		return CDialogBase::WindowProc(message, wParam, lParam);
//	}
//
//	return TRUE;
//}
