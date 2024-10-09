/*============================================================================*/
/*! SummarySpectrumPropertiesDlg.cpp

-# サマリスペクトラム設定画面
*/
/*============================================================================*/
// SummarySpectrumPropertiesDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "SummarySpectrumPropertiesDlg.h"
#include "afxdialogex.h"
#include "HookMsgBox.h"

extern CString spectrum_gpsz_moni_point_str[3];
extern CString spectrum_gpsz_bandw_str[23];
extern CString spectrum_gsz_dbm_str[1];
extern CSpectrumViewData::BandWidth_t spectrum_gast_bandw[];

extern COLORREF Spectrum_colText[5];
extern CString Spectrum_strFreqUnit[3];
extern CString gsz_center_str[1];

// CSummarySpectrumPropertiesDlg ダイアログ

IMPLEMENT_DYNAMIC(CSummarySpectrumPropertiesDlg, CDialogBase)

/*============================================================================*/
/*! SummarySpectrumPropertiesDlg

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CSummarySpectrumPropertiesDlg::CSummarySpectrumPropertiesDlg(CWnd* pParent /*=NULL*/)
: CDialogBase(CSummarySpectrumPropertiesDlg::IDD, pParent)
{

}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# コンストラクタ

@param  pParent：親ウィンドウ
@param	nViewID：グラフビューID
@param	listViewName：グラフビュー名リスト
@param  nStaionID：衛星ID
@param  nParentID：親ウィンドウID
@param  strWndName：ウィンドウタイトル
@param	pSummaryProp：サマリプロパティ
@retval なし
*/
/*============================================================================*/
CSummarySpectrumPropertiesDlg::CSummarySpectrumPropertiesDlg(CWnd* pParent, UINT nViewID, vector<CString> listViewName, UINT nStaionID, 
	UINT nParentID /*= 0*/, CString strWndName /*= _T("")*/, CWnd* pSummaryProp /*= NULL*/, BOOL bChgDlg /*= FALSE*/)
	: CDialogBase(CSummarySpectrumPropertiesDlg::IDD, pParent)
	, m_ActiveView(nViewID)
	, m_listViewName(listViewName)
	, m_nStationID(nStaionID)
	, m_nParentID(nParentID)
	, m_strWndName(strWndName)
	, m_pSummaryProp(pSummaryProp)
	, m_bChgGraphPropDlg(FALSE)
	, m_bChgDlg(bChgDlg)
{
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CSummarySpectrumPropertiesDlg::~CSummarySpectrumPropertiesDlg()
{
}


void CSummarySpectrumPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUMVIEW_MONPOINT_COMBO, m_spectrumSet.m_CCBoxMonitorPoint);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUMVIEW_BANDWIDTH_COMBO, m_spectrumSet.m_CCBoxBandWidth);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUMVIEW_CENTERFREQOFFSET_EDIT, m_spectrumSet.m_CEditCenterFreqOffset);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUMVIEW_CENTERFREQOFFSET_SPIN, m_spectrumSet.m_CSpinCenterFreqOffset);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUM_INPUTEXCHGEDIT, m_spectrumSet.m_CEditInputExchg);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUM_ATTEDIT, m_spectrumSet.m_CEditAtt);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUM_INPUTEXCHGSPIN, m_spectrumSet.m_CSpinInputExchg);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUM_ATTSPIN, m_spectrumSet.m_CSpinAtt);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUM_VERTICALSET_COMBO, m_verticalSet.m_CCBoxAutoScaleOnOff);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUMVIEW_YMAX_EDIT, m_verticalSet.m_CEditMax);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUMVIEW_YMIN_EDIT, m_verticalSet.m_CEditMin);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUMVIEW_YGRID_EDIT, m_verticalSet.m_CEditGrid);
	DDX_Control(pDX, IDC_SUMMARYGRAPTH_SELECTGRAPH_COMBO, m_wndObjectCombo);
}


BEGIN_MESSAGE_MAP(CSummarySpectrumPropertiesDlg, CDialogBase)
	ON_CBN_SELCHANGE(IDC_SUMMARYSPECTRUM_VERTICALSET_COMBO, &CSummarySpectrumPropertiesDlg::OnCbnSelchangeSummaryspectrumVerticalsetCombo)
	ON_BN_CLICKED(IDC_SUMMARYSPECTRUM_GRIDPROP_EXEC_BUTTON, &CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumGridpropExecButton)
	ON_BN_CLICKED(IDC_SUMMARYSPECTRUM_GRIDPROP_RESET_BUTTON, &CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumGridpropResetButton)
	ON_BN_CLICKED(IDC_SUMMARYSPECTRUMVIEW_CENTER_BUTTON, &CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumviewCenterButton)
	ON_BN_CLICKED(IDC_SUMMARYSPECTRUM_CTRL_EXEC_BUTTON, &CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumCtrlExecButton)
	ON_BN_CLICKED(IDC_SUMMARYSPECTRUM_CTRL_RESET_BUTTON, &CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumCtrlResetButton)
	ON_BN_CLICKED(IDC_SUMMARYSPECTRUM_INPUTATT_EXEC_BUTTON, &CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumInputattExecButton)
	ON_BN_CLICKED(IDC_SUMMARYSPECTRUM_INPUTATT_RESET_BUTTON, &CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumInputattResetButton)
	ON_BN_CLICKED(IDOK, &CSummarySpectrumPropertiesDlg::OnBnClickedOk)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SUMMARYSPECTRUMVIEW_CENTERFREQOFFSET_SPIN, &CSummarySpectrumPropertiesDlg::OnDeltaposSummaryspectrumviewCenterfreqoffsetSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SUMMARYSPECTRUM_INPUTEXCHGSPIN, &CSummarySpectrumPropertiesDlg::OnDeltaposSummaryspectrumInputexchgspin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SUMMARYSPECTRUM_ATTSPIN, &CSummarySpectrumPropertiesDlg::OnDeltaposSummaryspectrumAttspin)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_SUMMARYSPECTRUMVIEW_YMAX_EDIT, &CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumviewYmaxEdit)
	ON_EN_CHANGE(IDC_SUMMARYSPECTRUMVIEW_YMIN_EDIT, &CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumviewYminEdit)
	ON_EN_CHANGE(IDC_SUMMARYSPECTRUMVIEW_YGRID_EDIT, &CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumviewYgridEdit)
	ON_EN_CHANGE(IDC_SUMMARYSPECTRUMVIEW_CENTERFREQOFFSET_EDIT, &CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumviewCenterfreqoffsetEdit)
	ON_EN_CHANGE(IDC_SUMMARYSPECTRUM_INPUTEXCHGEDIT, &CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumInputexchgedit)
	ON_EN_CHANGE(IDC_SUMMARYSPECTRUM_ATTEDIT, &CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumAttedit)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_SUMMARYGRAPTH_SELECTGRAPH_COMBO, &CSummarySpectrumPropertiesDlg::OnCbnSelchangeSummarygrapthSelectgraphCombo)
	ON_CBN_SELCHANGE(IDC_SUMMARYSPECTRUMVIEW_MONPOINT_COMBO, &CSummarySpectrumPropertiesDlg::OnCbnSelchangeSummaryspectrumviewMonpointCombo)
	ON_CBN_SELCHANGE(IDC_SUMMARYSPECTRUMVIEW_BANDWIDTH_COMBO, &CSummarySpectrumPropertiesDlg::OnCbnSelchangeSummaryspectrumviewBandwidthCombo)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CSummarySpectrumPropertiesDlg::OnMenuClose)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSummarySpectrumPropertiesDlg メッセージ ハンドラー


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# オートスケール選択変更時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnCbnSelchangeSummaryspectrumVerticalsetCombo()
{
	m_verticalSet.OnCbnSelchangeSpectrumverticalsetCombo();
	if (this->GetSafeHwnd())
	{
		Invalidate();
	}

	// グラフ再描画通知
	theApp.m_pMainWnd->PostMessage(eMessage_SummaryPropetyChange, (WPARAM)m_nParentID, (LPARAM)m_ActiveView);
}

/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# 縦軸グリッド実行ボタンクリック時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumGridpropExecButton()
{
	m_verticalSet.OnBnClickedOk();
	if (this->GetSafeHwnd())
	{
		Invalidate();
	}

	// グラフ再描画通知
	theApp.m_pMainWnd->PostMessage(eMessage_SummaryPropetyChange, (WPARAM)m_nParentID, (LPARAM)m_ActiveView);
}


/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# 縦軸グリッドキャンセルボタンクリック時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumGridpropResetButton()
{
	m_verticalSet.OnBnClickedCancel();
	if (this->GetSafeHwnd())
	{
		Invalidate();
	}

	// グラフ再描画通知
	theApp.m_pMainWnd->PostMessage(eMessage_SummaryPropetyChange, (WPARAM)m_nParentID, (LPARAM)m_ActiveView);
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# センターボタン押下時に呼ばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumviewCenterButton()
{
	m_spectrumSet.OnBnClickedSpectrumviewButton1();
	if (this->GetSafeHwnd())
	{
		Invalidate();
	}

	// 再描画は、タイマー処理で監視データ更新後に行う
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# 実行ボタン押下時によばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumCtrlExecButton()
{
	m_spectrumSet.OnBnClickedSpectrumExecButton();
	if (this->GetSafeHwnd())
	{
		Invalidate();
	}

	// 再描画は、タイマー処理で監視データ更新後に行う
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# リセットボタン押下時によばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumCtrlResetButton()
{
	m_spectrumSet.OnBnClickedSpectrumResetButton();
	if (this->GetSafeHwnd())
	{
		Invalidate();
	}

	// 再描画は、タイマー処理で監視データ更新後に行う
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# 入力端換算実行ボタン押下時によばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumInputattExecButton()
{
	m_spectrumSet.OnBnClickedSpectrumInputexchgExecButton();
	if (this->GetSafeHwnd())
	{
		Invalidate();
	}

	// グラフ再描画通知
	theApp.m_pMainWnd->PostMessage(eMessage_SummaryPropetyChange, (WPARAM)m_nParentID, (LPARAM)m_ActiveView);
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# 入力端換算リセットボタン押下時によばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumInputattResetButton()
{
	m_spectrumSet.OnBnClickedSpectrumInputexchgResetButton();
	if (this->GetSafeHwnd())
	{
		Invalidate();
	}

	// グラフ再描画通知
	theApp.m_pMainWnd->PostMessage(eMessage_SummaryPropetyChange, (WPARAM)m_nParentID, (LPARAM)m_ActiveView);
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# メニューからのファイルクローズ時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnBnClickedOk()
{
	if (!m_bChgGraphPropDlg)
	{
		CString title;
		GetWindowText(title);

		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("終了します。\nよろしいですか？"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return;
	}

	OnOK();
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# ダイアログ初期化処理

@param  なし
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
BOOL CSummarySpectrumPropertiesDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	if (!m_strWndName.IsEmpty())
	{
		CString strTitle;
		strTitle.Format(_T("%s設定"), (LPCTSTR)m_strWndName);
		SetWindowText(strTitle);
	}

	m_spectrumSet.InitializeDlg(m_nStationID, m_ActiveView, eSpectrumGraphType_Sammary);
	m_verticalSet.InitializeDlg(m_nStationID, m_ActiveView, eSpectrumGraphType_Sammary);

	// グラフ選択コンボボックス初期化
	for (int i = 0; i < m_listViewName.size(); i++)
	{
		m_wndObjectCombo.InsertString(-1, m_listViewName[i]);
	}
//	m_wndObjectCombo.SetCurSel(eSummaryWndSetType_XSP);
	CPropertiesData& prop = theApp.GetSatelliteData().GetSummaryPropertiesData(m_nStationID, m_ActiveView);
	m_wndObjectCombo.SetCurSel(prop.nGraphIdx);

	// 監視モードの場合は非活性
	if (theApp.GetExecuteMode() != eExecuteMode_Control){
		GetDlgItem(IDC_SUMMARYSPECTRUM_CTRL_EXEC_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_SUMMARYSPECTRUMVIEW_CENTER_BUTTON)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);
	UpdateWindow();

	// タイマ登録
	SetTimer(1, SPECTRUM_INTERVAL, NULL);

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
/*! CSummarySpectrumPropertiesDlg

-# CENTER FREQ OFFSETスピンコントロール変更時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnDeltaposSummaryspectrumviewCenterfreqoffsetSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_spectrumSet.OnDeltaposSpectrumviewSpin4(pNMHDR, pResult);
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# 入力端換算スピンコントロール変更時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnDeltaposSummaryspectrumInputexchgspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_spectrumSet.OnDeltaposSpectrumInputexchgspin(pNMHDR, pResult);
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# ATT値スピンコントロール変更時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnDeltaposSummaryspectrumAttspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_spectrumSet.OnDeltaposSpectrumAttspin(pNMHDR, pResult);
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# 縦スクロールバークリック時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_spectrumSet.VScroll(nSBCode, nPos, pScrollBar);

	CDialogBase::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumviewYmaxEdit()
{
	m_verticalSet.OnEnChangeVMax();
}


void CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumviewYminEdit()
{
	m_verticalSet.OnEnChangeVMin();
}


void CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumviewYgridEdit()
{
	m_verticalSet.OnEnChangeVGrid();
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# 中心周波数オフセットのエディット変更時に呼ばれる

@param	なし
@retval なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumviewCenterfreqoffsetEdit()
{
	m_spectrumSet.OnEnChangeSpectrumviewEdit7();
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# 入力端換算値のエディット変更時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumInputexchgedit()
{
	m_spectrumSet.OnEnChangeSpectrumInputexchgedit();
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# ATT値のエディット変更時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumAttedit()
{
	m_spectrumSet.OnEnChangeSpectrumAttedit();
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# 描画時に呼ばれ、指定コントロールの背景を塗りつぶすブラシのハンドルを返す

@param	pDC：子ウィンドウのディスプレイコンテキストのポインタ
@param	pWnd：色を格納されたコントロールのポインタ
@param	nCtrlColor：コントロールの種類
@retval	コントロールの背景を塗りつぶすブラシのハンドル

*/
/*============================================================================*/
HBRUSH CSummarySpectrumPropertiesDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogBase::OnCtlColor(pDC, pWnd, nCtlColor);

	m_spectrumSet.CtlColor(pDC, pWnd, nCtlColor);
	m_verticalSet.CtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# グラフ選択コンボボックス変更時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnCbnSelchangeSummarygrapthSelectgraphCombo()
{
	CPropertiesData& prop = theApp.GetSatelliteData().GetSummaryPropertiesData(m_nStationID, m_ActiveView);

	int nCurSel = m_wndObjectCombo.GetCurSel();

	if (nCurSel == prop.nGraphIdx)
	{
		// 何もしない
		return;
	}

	// ミニトレンド選択時は、設定ダイアログを変える。
	if (nCurSel != eSummaryWndSetType_XSP && nCurSel != eSummaryWndSetType_KASP)
	{
		CString title;
		GetWindowText(title);
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("トレンド設定ウィンドウに切り替えます。\nよろしいですか？"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return;

		// プロパティ内容をトレンドに変更する
		prop.nGraphIdx = nCurSel;
		for (int i = 0; i < MAX_SUMMARYTDATA; i++)
		{
			prop.dataName[i] = mListSummarySig[m_nStationID][nCurSel][i].strSigName;
			prop.dataGName[i] = mListSummarySig[m_nStationID][nCurSel][i].strSimpleSigName;
			prop.dataUnit[i] = mListSummarySig[m_nStationID][nCurSel][i].strUnit;
		}
		// プロパティデータセット
		theApp.GetSatelliteData().SetSummaryPropertiesData(m_nStationID, m_ActiveView, prop);

		CRect rect;
		GetWindowRect(&rect);
		theApp.SetWindowTmpPoint(rect.left, rect.top);

		// トレンドグラフ切り替え通知
		theApp.m_pMainWnd->PostMessage(eMessage_InitSummaryWnd, (WPARAM)m_nParentID, (LPARAM)m_ActiveView);
		// サマリトレンドプロパティダイアログを開く通知
		theApp.m_pMainWnd->PostMessage(eMessage_OpenSummaryTrendSetDlgChg, (WPARAM)m_pSummaryProp, (LPARAM)this);
	}
	else
	{
		// スペクトラムの場合
		prop.nGraphIdx = nCurSel;
		// プロパティデータセット
		theApp.GetSatelliteData().SetSummaryPropertiesData(m_nStationID, m_ActiveView, prop);

		CRect rect;
		GetWindowRect(&rect);
		theApp.SetWindowTmpPoint(rect.left, rect.top);

		// トレンドグラフ切り替え通知
		theApp.m_pMainWnd->PostMessage(eMessage_InitSummaryWnd, (WPARAM)m_nParentID, (LPARAM)m_ActiveView);
		// サマリスペクトラムプロパティダイアログを開く通知
		theApp.m_pMainWnd->PostMessage(eMessage_OpenSummarySpectrumSetDlgChg, (WPARAM)m_pSummaryProp, (LPARAM)this);
	}
	// トレンド設定ダイアログを閉じる
	m_bChgGraphPropDlg = TRUE;
	PostMessage(WM_CLOSE);
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# MONITOR POINTコンボボックスが選択変更されたときに呼ばれる

@param  なし
@retval	なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnCbnSelchangeSummaryspectrumviewMonpointCombo()
{
	m_spectrumSet.OnCbnSelchangeSpectrumviewCombo1();
	if (this->GetSafeHwnd())
	{
		m_spectrumSet.m_CCBoxMonitorPoint.Invalidate();
	}
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# BAND WIDTHコンボボックスが選択変更されたときに呼ばれる

@param  なし
@retval	なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnCbnSelchangeSummaryspectrumviewBandwidthCombo()
{
	m_spectrumSet.OnCbnSelchangeSpectrumviewCombo2();
	if (this->GetSafeHwnd())
	{
		m_spectrumSet.m_CCBoxBandWidth.Invalidate();
	}
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# メニューからのファイルクローズ時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnMenuClose()
{
	//	OnOK();
	ShowWindow(SW_HIDE);
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# ウィンドウを閉じる時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnClose()
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
/*! CSpectrumView

-# タイマイベント発生時に呼び出される

@param	nIDEvent：イベントID
@retval	なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnTimer(UINT_PTR nIDEvent)
{
	// 制御更新
	m_spectrumSet.UpdateCtrl();

	CDialogBase::OnTimer(nIDEvent);
}


/*============================================================================*/
/*! CSpectrumView

-# ウィンドウ破棄時に呼び出される

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnDestroy()
{
	CDialogBase::OnDestroy();

	KillTimer(1);
}
