/*============================================================================*/
/*! SpectrumVerticalSetCommonDlg.cpp

-# スペクトラム縦軸設定共通画面
*/
/*============================================================================*/
// SpectrumVerticalSetCommonDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "SpectrumVerticalSetCommonDlg.h"
#include "afxdialogex.h"
#include "HookMsgBox.h"

extern COLORREF Spectrum_colText[5];

// CSpectrumVerticalSetCommonDlg ダイアログ

IMPLEMENT_DYNAMIC(CSpectrumVerticalSetCommonDlg, CDialogBase)

/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# コンストラクタ

@param	IDD：IDD
@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CSpectrumVerticalSetCommonDlg::CSpectrumVerticalSetCommonDlg(UINT IDD, CWnd* pParent /*=NULL*/)
: CDialogBase(IDD, pParent)
{

}

/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CSpectrumVerticalSetCommonDlg::~CSpectrumVerticalSetCommonDlg()
{
}

void CSpectrumVerticalSetCommonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSpectrumVerticalSetCommonDlg, CDialogBase)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSpectrumVerticalSetCommonDlg メッセージ ハンドラー


/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# ダイアログ初期化処理

@param  nStationID：局ID
@param  nViewID：ビューID
@param  nGraphType：グラフタイプ
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
BOOL CSpectrumVerticalSetCommonDlg::InitializeDlg(UINT nStationID, UINT nViewID, UINT nGraphType)
{
	m_nStationID = nStationID;
	m_nViewID = nViewID;
	m_nGraphType = nGraphType;

	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return FALSE;
	}

	for (int i = 0; i < eColorNum; i++)
	{
		m_colText[i] = Spectrum_colText[i];
	}

	// コンボボックス初期化
	m_bLastAutoScale = pCSectrumData->GetAutoScale();

	if (m_bLastAutoScale)
		m_CCBoxAutoScaleOnOff.SetCurSel(SPECTRUM_AUTOSCALE_ON);
	else
		m_CCBoxAutoScaleOnOff.SetCurSel(SPECTRUM_AUTOSCALE_OFF);
	m_CCBoxAutoScaleOnOff.SetSelectedTextColor(m_colText[eBlack]);

	// エディットボックス, スピンコントロール初期化
	m_CEditMax.DisableMask();
	m_CEditMax.SetValidChars(_T("0123456789,-"));
	m_CEditMin.DisableMask();
	m_CEditMin.SetValidChars(_T("0123456789,-"));
	m_CEditGrid.DisableMask();
	m_CEditGrid.SetValidChars(_T("0123456789,"));

	CString str;

	m_gl_last_y_grid_max = pCSectrumData->GetVerticalMax();
	str.Format(_T("%d"), m_gl_last_y_grid_max);
	m_CEditMax.SetWindowTextW(str);

	m_gl_last_y_grid_min = pCSectrumData->GetVerticalMin();
	str.Format(_T("%d"), m_gl_last_y_grid_min);
	m_CEditMin.SetWindowTextW(str);

	m_gl_last_y_grid_val = pCSectrumData->GetVerticalGrid();
	str.Format(_T("%d"), m_gl_last_y_grid_val);
	m_CEditGrid.SetWindowTextW(str);

	SetEnableVAxis(!m_bLastAutoScale);

	// 色設定
	m_colEditMax = m_colText[eBlack];
	m_colEditMin = m_colText[eBlack];
	m_colEditGrid = m_colText[eBlack];

	return TRUE;
}

/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# 縦軸のエディット変更時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
BOOL CSpectrumVerticalSetCommonDlg::SetVerticalValues()
{
	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return FALSE;
	}

	if (m_CEditMax.GetSafeHwnd() == NULL ||
		m_CEditMin.GetSafeHwnd() == NULL ||
		m_CEditGrid.GetSafeHwnd() == NULL)
		return FALSE;

	if (this->GetSafeHwnd() != NULL)
		UpdateData(TRUE);

	CString str;
	long l_y_grid_max, l_y_grid_min, l_y_grid_val;

	// グリッド最大値チェック
	m_CEditMax.GetWindowText(str);
	str.Trim();
	str.Remove(_T(','));
	l_y_grid_max = _ttol(str);

	if (l_y_grid_max > m_VMax)
	{
		// エラーダイアログ表示
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return FALSE;
		if (strMsg.LoadString(IDS_YGRID_MAX_ILLEGAL) == FALSE)
			return FALSE;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// グリッド最小値チェック
	m_CEditMin.GetWindowText(str);
	str.Trim();
	str.Remove(_T(','));
	l_y_grid_min = _ttol(str);

	if (l_y_grid_min < m_VMin)
	{
		// エラーダイアログ表示
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return FALSE;
		if (strMsg.LoadString(IDS_YGRID_MIN_ILLEGAL) == FALSE)
			return FALSE;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
		return FALSE;
	}
	else if (l_y_grid_max <= l_y_grid_min)
	{
		// エラーダイアログ表示
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return FALSE;
		if (strMsg.LoadString(IDS_YGRID_MIN_TOO_LARGE) == FALSE)
			return FALSE;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// グリッド値チェック
	m_CEditGrid.GetWindowText(str);
	str.Trim();
	str.Remove(_T(','));
	l_y_grid_val = _ttol(str);

	if (l_y_grid_val < 1)
	{
		// エラーダイアログ表示
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return FALSE;
		if (strMsg.LoadString(IDS_YGRID_VALUE_TOO_SMALL) == FALSE)
			return FALSE;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
		return FALSE;
	}
	else if (l_y_grid_max - l_y_grid_min < l_y_grid_val)
	{
		// エラーダイアログ表示
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return FALSE;
		if (strMsg.LoadString(IDS_YGRID_VALUE_TOO_LARGE) == FALSE)
			return FALSE;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// 確認ダイアログ表示
	{
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("実行します。\nよろしいですか？"), _T("グリッド設定"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return FALSE;
	}

	pCSectrumData->SetVerticalMax(l_y_grid_max);
	pCSectrumData->SetVerticalMin(l_y_grid_min);
	pCSectrumData->SetVerticalGrid(l_y_grid_val);

	return TRUE;
}


/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# 縦軸設定の有効無効切り替え時に呼ばれる

@param  bEnable：TRUE:有効 / FALSE:無効
@retval なし

*/
/*============================================================================*/
void CSpectrumVerticalSetCommonDlg::SetEnableVAxis(BOOL bEnable)
{
	m_CEditMax.EnableWindow(bEnable);
	m_CEditMin.EnableWindow(bEnable);
}


/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# スペクトラム設定データ取得

@param	なし
@retval	スペクトラム設定データ

*/
/*============================================================================*/
CSpectrumViewData* CSpectrumVerticalSetCommonDlg::GetSpectrumViewData()
{
	CSpectrumViewData* pCSectrumData;
	if (m_nGraphType == eSpectrumGraphType_Normal)
	{
		pCSectrumData = &theApp.GetSatelliteData().GetSpectrumViewData();
	}
	else
	{
		pCSectrumData = &theApp.GetSatelliteData().GetSummarySpectrumViewData(m_nStationID, m_nViewID);
	}

	return pCSectrumData;
}


/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# オートスケール選択変更時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
void CSpectrumVerticalSetCommonDlg::OnCbnSelchangeSpectrumverticalsetCombo()
{
	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return;
	}

	int nCurSel = m_CCBoxAutoScaleOnOff.GetCurSel();
//	m_CCBoxAutoScaleOnOff.SetSelectedTextColor(m_colText[eOrange]);
	BOOL	bAutoScale;
	if (nCurSel == SPECTRUM_AUTOSCALE_ON)
		bAutoScale = TRUE;
	else
		bAutoScale = FALSE;

	pCSectrumData->SetAutoScale(bAutoScale);

	SetEnableVAxis(!bAutoScale);
}


/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# OKボタンクリック時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
void CSpectrumVerticalSetCommonDlg::OnBnClickedOk()
{
	OnCbnSelchangeSpectrumverticalsetCombo();
	(void)SetVerticalValues();

	m_CCBoxAutoScaleOnOff.SetSelectedTextColor(m_colText[eBlack]);
	m_colEditMax = m_colText[eBlack];
	m_colEditMin = m_colText[eBlack];
	m_colEditGrid = m_colText[eBlack];
}


/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# キャンセルボタンクリック時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
void CSpectrumVerticalSetCommonDlg::OnBnClickedCancel()
{
	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return;
	}

	pCSectrumData->SetAutoScale(m_bLastAutoScale);

	pCSectrumData->SetVerticalMax(m_gl_last_y_grid_max);
	pCSectrumData->SetVerticalMin(m_gl_last_y_grid_min);
	pCSectrumData->SetVerticalGrid(m_gl_last_y_grid_val);

	m_CCBoxAutoScaleOnOff.SetSelectedTextColor(m_colText[eBlack]);
	m_colEditMax = m_colText[eBlack];
	m_colEditMin = m_colText[eBlack];
	m_colEditGrid = m_colText[eBlack];
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# 最大値のエディット変更時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
void CSpectrumVerticalSetCommonDlg::OnEnChangeVMax()
{
//	m_colEditMax = m_colText[eOrange];
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# 最小値のエディット変更時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
void CSpectrumVerticalSetCommonDlg::OnEnChangeVMin()
{
//	m_colEditMin = m_colText[eOrange];
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# グリッド値のエディット変更時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
void CSpectrumVerticalSetCommonDlg::OnEnChangeVGrid()
{
//	m_colEditGrid = m_colText[eOrange];
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# 描画時に呼ばれ、指定コントロールの背景を塗りつぶすブラシのハンドルを返す

@param	pDC：子ウィンドウのディスプレイコンテキストのポインタ
@param	pWnd：色を格納されたコントロールのポインタ
@param	nCtrlColor：コントロールの種類
@retval	コントロールの背景を塗りつぶすブラシのハンドル

*/
/*============================================================================*/
HBRUSH CSpectrumVerticalSetCommonDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogBase::OnCtlColor(pDC, pWnd, nCtlColor);

	CtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# 描画時に呼ばれ、指定コントロールの背景を塗りつぶす

@param	pDC：子ウィンドウのディスプレイコンテキストのポインタ
@param	pWnd：色を格納されたコントロールのポインタ
@param	nCtrlColor：コントロールの種類
@retval	なし

*/
/*============================================================================*/
void CSpectrumVerticalSetCommonDlg::CtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_EDIT)
	{
		if (*pWnd == m_CEditMax)
		{
			pDC->SetTextColor(m_colEditMax);
		}
		else if (*pWnd == m_CEditMin)
		{
			pDC->SetTextColor(m_colEditMin);
		}
		else if (*pWnd == m_CEditGrid)
		{
			pDC->SetTextColor(m_colEditGrid);
		}
	}
}

