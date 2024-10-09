/*============================================================================*/
/*! CSpectrumVerticalSetDlg.cpp

-# スペクトラム縦軸設定画面
*/
/*============================================================================*/
// CSpectrumVerticalSetDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "SpectrumVerticalSetDlg.h"
#include "afxdialogex.h"

// CSpectrumVerticalSetDlg ダイアログ

IMPLEMENT_DYNAMIC(CSpectrumVerticalSetDlg, CSpectrumVerticalSetCommonDlg)

/*============================================================================*/
/*! CSpectrumVerticalSetDlg

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CSpectrumVerticalSetDlg::CSpectrumVerticalSetDlg(CWnd* pParent /*=NULL*/)
: CSpectrumVerticalSetCommonDlg(CSpectrumVerticalSetDlg::IDD, pParent)
	, m_pSpectrumView(NULL)
{

}


/*============================================================================*/
/*! CSpectrumVerticalSetDlg

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CSpectrumVerticalSetDlg::CSpectrumVerticalSetDlg(CWnd* pParent, CSpectrumView* pSpectrumView)
	: CSpectrumVerticalSetCommonDlg(CSpectrumVerticalSetDlg::IDD, pParent)
	, m_pSpectrumView(pSpectrumView)
{
	
}


/*============================================================================*/
/*! CSpectrumVerticalSetDlg

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CSpectrumVerticalSetDlg::~CSpectrumVerticalSetDlg()
{
}

void CSpectrumVerticalSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CSpectrumVerticalSetCommonDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPECTRUMVERTICALSET_COMBO, m_CCBoxAutoScaleOnOff);
	DDX_Control(pDX, IDC_SPECTRUMVIEW_EDIT1, m_CEditMax);
	DDX_Control(pDX, IDC_SPECTRUMVIEW_EDIT2, m_CEditGrid);
	DDX_Control(pDX, IDC_SPECTRUMVIEW_EDIT3, m_CEditMin);
}


BEGIN_MESSAGE_MAP(CSpectrumVerticalSetDlg, CSpectrumVerticalSetCommonDlg)
	ON_CBN_SELCHANGE(IDC_SPECTRUMVERTICALSET_COMBO, &CSpectrumVerticalSetDlg::OnCbnSelchangeSpectrumverticalsetCombo)
	ON_EN_CHANGE(IDC_SPECTRUMVIEW_EDIT1, &CSpectrumVerticalSetDlg::OnEnChangeVMax)
	ON_EN_CHANGE(IDC_SPECTRUMVIEW_EDIT2, &CSpectrumVerticalSetDlg::OnEnChangeVGrid)
	ON_EN_CHANGE(IDC_SPECTRUMVIEW_EDIT3, &CSpectrumVerticalSetDlg::OnEnChangeVMin)
	ON_BN_CLICKED(IDOK, &CSpectrumVerticalSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSpectrumVerticalSetDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSpectrumVerticalSetDlg メッセージ ハンドラー


/*============================================================================*/
/*! CSpectrumVerticalSetDlg

-# オートスケール選択変更時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
void CSpectrumVerticalSetDlg::OnCbnSelchangeSpectrumverticalsetCombo()
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

	if (m_pSpectrumView != NULL)
		m_pSpectrumView->Invalidate();
}


/*============================================================================*/
/*! CSpectrumVerticalSetDlg

-# ダイアログ初期化時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
BOOL CSpectrumVerticalSetDlg::OnInitDialog()
{
	CSpectrumVerticalSetCommonDlg::OnInitDialog();

	if (!InitializeDlg(theApp.GetSelectStation(), 0, eSpectrumGraphType_Normal))
		return FALSE;

	UpdateData(FALSE);
	UpdateWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CSpectrumVerticalSetDlg

-# メッセージ処理

@breif	Enterキー（Escキー）を押しても閉じないようにする。
@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CSpectrumVerticalSetDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			{
				CWnd* pWnd = GetFocus();
				if (pWnd != NULL)
				{
					int nID = pWnd->GetDlgCtrlID();
					if (nID == IDC_SPECTRUMVIEW_EDIT1 ||
						nID == IDC_SPECTRUMVIEW_EDIT3 ||
						nID == IDC_SPECTRUMVIEW_EDIT2)
					{
						if (SetVerticalValues())
						{
							if (m_pSpectrumView != NULL)
								m_pSpectrumView->Invalidate();
						}
					}
				}
			}
			return FALSE;

		default:
			break;
		}
	}

	return CSpectrumVerticalSetCommonDlg::PreTranslateMessage(pMsg);
}


///*============================================================================*/
///*! CSpectrumVerticalSetDlg
//
//-# 縦軸のエディット変更時に呼ばれる
//
//@param  なし
//@retval なし
//
//*/
///*============================================================================*/
//BOOL CSpectrumVerticalSetDlg::SetVerticalValues()
//{
//	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
//	if (pCSectrumData == NULL)
//	{
//		return FALSE;
//	}
//
//	if (m_CEditMax.GetSafeHwnd() == NULL ||
//		m_CEditMin.GetSafeHwnd() == NULL ||
//		m_CEditGrid.GetSafeHwnd() == NULL)
//		return FALSE;
//
//	UpdateData(TRUE);
//
//	CString str;
//	long l_y_grid_max, l_y_grid_min, l_y_grid_val;
//
//	// グリッド最大値チェック
//	m_CEditMax.GetWindowText(str);
//	str.Trim();
//	str.Remove(_T(','));
//	l_y_grid_max = _ttol(str);
//
//	if (l_y_grid_max > m_VMax)
//	{
//		// エラーダイアログ表示
//		CString strTitle;
//		CString strMsg;
//		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
//			return FALSE;
//		if (strMsg.LoadString(IDS_YGRID_MAX_ILLEGAL) == FALSE)
//			return FALSE;
//		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
//		return FALSE;
//	}
//
//	// グリッド最小値チェック
//	m_CEditMin.GetWindowText(str);
//	str.Trim();
//	str.Remove(_T(','));
//	l_y_grid_min = _ttol(str);
//
//	if (l_y_grid_min < m_VMin)
//	{
//		// エラーダイアログ表示
//		CString strTitle;
//		CString strMsg;
//		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
//			return FALSE;
//		if (strMsg.LoadString(IDS_YGRID_MIN_ILLEGAL) == FALSE)
//			return FALSE;
//		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
//		return FALSE;
//	}
//	else if (l_y_grid_max <= l_y_grid_min)
//	{
//		// エラーダイアログ表示
//		CString strTitle;
//		CString strMsg;
//		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
//			return FALSE;
//		if (strMsg.LoadString(IDS_YGRID_MIN_TOO_LARGE) == FALSE)
//			return FALSE;
//		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
//		return FALSE;
//	}
//
//	// グリッド値チェック
//	m_CEditGrid.GetWindowText(str);
//	str.Trim();
//	str.Remove(_T(','));
//	l_y_grid_val = _ttol(str);
//
//	if (l_y_grid_val < 1)
//	{
//		// エラーダイアログ表示
//		CString strTitle;
//		CString strMsg;
//		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
//			return FALSE;
//		if (strMsg.LoadString(IDS_YGRID_VALUE_TOO_SMALL) == FALSE)
//			return FALSE;
//		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
//		return FALSE;
//	}
//	else if (l_y_grid_max - l_y_grid_min < l_y_grid_val)
//	{
//		// エラーダイアログ表示
//		CString strTitle;
//		CString strMsg;
//		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
//			return FALSE;
//		if (strMsg.LoadString(IDS_YGRID_VALUE_TOO_LARGE) == FALSE)
//			return FALSE;
//		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
//		return FALSE;
//	}
//
//	pCSectrumData->SetVerticalMax(l_y_grid_max);
//	pCSectrumData->SetVerticalMin(l_y_grid_min);
//	pCSectrumData->SetVerticalGrid(l_y_grid_val);
//
//	return TRUE;
//}

/*============================================================================*/
/*! CSpectrumVerticalSetDlg

-# OKボタンクリック時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
void CSpectrumVerticalSetDlg::OnBnClickedOk()
{
	OnCbnSelchangeSpectrumverticalsetCombo();
	if (SetVerticalValues())
	{
		if (m_pSpectrumView != NULL)
		{
			m_pSpectrumView->ClearAutoVMaxMin();
			m_pSpectrumView->Invalidate();
		}
//		CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
//		if (pCSectrumData == NULL)
//		{
//			return;
//		}
//
//		pCSectrumData->FileWrite();
//
//		// ファイル保存
//		theApp.GetSatelliteData().SpectrumFileWrite(m_nStationID);

		CSpectrumVerticalSetCommonDlg::OnOK();
	}
}


/*============================================================================*/
/*! CSpectrumVerticalSetDlg

-# キャンセルボタンクリック時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
void CSpectrumVerticalSetDlg::OnBnClickedCancel()
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

	if (m_pSpectrumView != NULL)
		m_pSpectrumView->Invalidate();

	CSpectrumVerticalSetCommonDlg::OnCancel();
}

