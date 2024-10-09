/*============================================================================*/
/*! CSpectrumCommonDlg.cpp

-# スペクトラム設定共通画面
*/
/*============================================================================*/
// SpectrumCommonDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "SpectrumCommonDlg.h"
#include "afxdialogex.h"
#include "HookMsgBox.h"
#include "..\Utility\SshShell.h"

extern CString spectrum_gpsz_moni_point_str[3];
extern CString spectrum_gpsz_bandw_str[23];
extern CString spectrum_gsz_dbm_str[1];
extern CSpectrumViewData::BandWidth_t spectrum_gast_bandw[];

extern COLORREF Spectrum_colText[5];
extern CString Spectrum_strFreqUnit[3];
extern CString gsz_center_str[1];

// CSpectrumCommonDlg ダイアログ

IMPLEMENT_DYNAMIC(CSpectrumCommonDlg, CDialogBase)

/*============================================================================*/
/*! CSpectrumCommonDlg

-# コンストラクタ

@param	IDD：IDD
@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CSpectrumCommonDlg::CSpectrumCommonDlg(UINT IDD, CWnd* pParent /*=NULL*/)
: CDialogBase(IDD, pParent)
, m_strPeakLevelUnit(_T(""))
, m_strPeakFreqUnit(_T(""))
, m_strHLValue(_T(""))
, m_strHCValue(_T(""))
, m_strHRValue(_T(""))
, m_strDataTime(_T(""))
, m_strPeakLevel(_T(""))
, m_strPeakFreq(_T(""))
, m_pView(NULL)
, m_bMPModified(FALSE)
, m_bBWModified(FALSE)
, m_bCFOfstModified(FALSE)
, m_bMPMListOpen(FALSE)
, m_bBWMListOpen(FALSE)
, m_bCFOfstMListOpen(FALSE)
{

}

/*============================================================================*/
/*! CSpectrumCommonDlg

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CSpectrumCommonDlg::~CSpectrumCommonDlg()
{
}

void CSpectrumCommonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSpectrumCommonDlg, CDialogBase)
END_MESSAGE_MAP()


// CSpectrumCommonDlg メッセージ ハンドラー


/*============================================================================*/
/*! CSpectrumCommonDlg

-# ダイアログ初期化処理

@param  nStationID：局ID
@param  nViewID：ビューID
@param  nGraphType：グラフタイプ
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
BOOL CSpectrumCommonDlg::InitializeDlg(UINT nStationID, UINT nViewID, UINT nGraphType)
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

	// 設定初期化
	pCSectrumData->m_gl_moni_point_sel = SPECTRUM_MONIP_70MIF_SEL;
	pCSectrumData->m_gl_bw_sel = 0;
	pCSectrumData->m_gd_freq_ofs_in = 0.0;
	pCSectrumData->m_gd_peak_freq = 0.0;
	pCSectrumData->m_gd_peak_level = 0.0;

#ifdef _ENGVALCNV_TREND
	pCSectrumData->GetSpectrumData();
#else
	// ダミーデータ生成
	pCSectrumData->m_gsz_time_tag = _T("123 12:34:56");
	pCSectrumData->m_gsz_bandw_sts = _T("32MHz");
	pCSectrumData->m_gsz_moni_point = _T("70M_IF");
	pCSectrumData->m_gd_freq_ofs = 0.0;
	pCSectrumData->m_gd_center_freq = 0.0;

	pCSectrumData->m_nDummyOffset = 0;

	// ダミーオフセット生成
	pCSectrumData->CreateDummyOffset();
#endif /* _ENGVALCNV_TREND */

	//	// エディットマスク
	m_CEditCenterFreqOffset.DisableMask();
	m_CEditCenterFreqOffset.SetValidChars(_T("0123456789,.-"));
	m_CEditInputExchg.DisableMask();
	m_CEditInputExchg.SetValidChars(_T("0123456789,.-"));
	m_CEditAtt.DisableMask();
	m_CEditAtt.SetValidChars(_T("0123456789,.-"));

	CString str;
	// 入力換算設定
	str = CSatelliteData::AddCanma(pCSectrumData->m_stVScale.gd_y_offset_val, 2);
	m_CEditInputExchg.SetWindowTextW(str);
	str = CSatelliteData::AddCanma(pCSectrumData->m_gd_y_offset_att, 2);
	m_CEditAtt.SetWindowTextW(str);
	// 色設定
//	m_colEditMax = m_colText[eBlack];
//	m_colEditMin = m_colText[eBlack];
//	m_colEditGrid = m_colText[eBlack];
	m_colEditInputExchg = m_colText[eBlack];
	m_colEditAtt = m_colText[eBlack];

	str = CSatelliteData::AddCanma(pCSectrumData->m_gd_freq_ofs_in, 3);
	m_CEditCenterFreqOffset.SetWindowText(str);
	m_colEditCenterFreqOffset = m_colText[eBlack];

	// スピンコントロール初期化
	m_CSpinCenterFreqOffset.SetRange32((int)(SPECTRUM_FREQ_OFS_MIN), (int)(SPECTRUM_FREQ_OFS_MAX));
	m_CSpinInputExchg.SetRange32(SPECTRUM_INPUTMIN, SPECTRUM_INPUTMAX);
	m_CSpinAtt.SetRange32(SPECTRUM_INPUTMIN, SPECTRUM_INPUTMAX);

	// コンボボックス初期化
	for (int i = 0; i < sizeof(spectrum_gpsz_moni_point_str) / sizeof(spectrum_gpsz_moni_point_str[0]); i++)
	{
		m_CCBoxMonitorPoint.InsertString(-1, spectrum_gpsz_moni_point_str[i]);
	}
	m_CCBoxMonitorPoint.SetCurSel(SPECTRUM_MONIP_70MIF_SEL);
	m_nMonitorPointCurSel = m_CCBoxMonitorPoint.GetCurSel();
	m_CCBoxMonitorPoint.SetSelectedTextColor(m_colText[eBlack]);
	for (int i = 0; i < sizeof(spectrum_gpsz_bandw_str) / sizeof(spectrum_gpsz_bandw_str[0]); i++)
	{
		m_CCBoxBandWidth.InsertString(-1, spectrum_gpsz_bandw_str[i]);
	}
	m_CCBoxBandWidth.SetCurSel(0);
	m_nBandWidthCurSel = m_CCBoxBandWidth.GetCurSel();
	m_CCBoxBandWidth.SetSelectedTextColor(m_colText[eBlack]);

	m_strPeakLevelUnit = spectrum_gsz_dbm_str[edBm];
	m_strPeakFreqUnit = Spectrum_strFreqUnit[eMHz];
	m_strHLValue = spectrum_gast_bandw[0].sz_low;
	m_strHCValue = gsz_center_str[0];
	m_strHRValue = spectrum_gast_bandw[0].sz_upp;

	m_bMPModified = FALSE;
	m_bBWModified = FALSE;
	m_bCFOfstModified = FALSE;
	m_bMPMListOpen = FALSE;
	m_bBWMListOpen = FALSE;
	m_bCFOfstMListOpen = FALSE;

	return TRUE;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# CENTER FREQ OFFSETのスピンコントロール変更時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnDeltaposSpectrumviewSpin4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// 中心周波数オフセット値を変更する
	if (m_CSpinCenterFreqOffset.GetSafeHwnd() == NULL)
	{
		return;
	}

	CString str;
	m_CEditCenterFreqOffset.GetWindowText(str);
	str.Trim();
	str.Remove(_T(','));
	double d_freq = _ttof(str);

	if (d_freq < SPECTRUM_FREQ_OFS_MIN)
	{
		d_freq = SPECTRUM_FREQ_OFS_MIN;
	}
	else if (d_freq > SPECTRUM_FREQ_OFS_MAX)
	{
		d_freq = SPECTRUM_FREQ_OFS_MAX;
	}

	//	m_CSpinCenterFreqOffset.SetPos32((int)round(d_freq * 100.0));
	m_CSpinCenterFreqOffset.SetPos32((int)round(d_freq));

	*pResult = 0;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# 縦スクロールバークリック時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	VScroll(nSBCode, nPos, pScrollBar);

	CDialogBase::OnVScroll(nSBCode, nPos, pScrollBar);
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# 縦スクロールバークリック時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::VScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (nSBCode == SB_THUMBPOSITION)
	{
		if ((m_CSpinCenterFreqOffset.m_hWnd == pScrollBar->m_hWnd))
		{
			CString str;
			int val = m_CSpinCenterFreqOffset.GetPos32();
			str = CSatelliteData::AddCanma((double)val, 3);
			m_CEditCenterFreqOffset.SetWindowText(str);
		}
		else if ((m_CSpinInputExchg.m_hWnd == pScrollBar->m_hWnd))
		{
			CString str;
			int val = m_CSpinInputExchg.GetPos32();
			str = CSatelliteData::AddCanma((double)val, 2);
			m_CEditInputExchg.SetWindowText(str);
		}
		else if ((m_CSpinAtt.m_hWnd == pScrollBar->m_hWnd))
		{
			CString str;
			int val = m_CSpinAtt.GetPos32();
			str = CSatelliteData::AddCanma((double)val, 2);
			m_CEditAtt.SetWindowText(str);
		}
	}
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# センターボタン押下時に呼ばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnBnClickedSpectrumviewButton1()
{
	CString strTitle;
	CString strMsg;
	if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
		return;
	if (strMsg.LoadString(IDS_CHECK_CTRL_MSG) == FALSE)
		return;
	if (IDCANCEL == MessageBoxHooked(this->m_hWnd, strMsg, strTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return;

	// 中心オフセットをリセットする
	ResetCenterFreqOffset();
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# 中心周波数オフセット値を取得する

@param	なし
@retval	グリッド値

*/
/*============================================================================*/
void CSpectrumCommonDlg::ResetCenterFreqOffset()
{
	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return;
	}

	CString str;
	m_CCBoxMonitorPoint.GetLBText(m_nMonitorPointCurSel, str);
	double d_freq;
	if (pCSectrumData->m_gsz_moni_point.Compare(str) == 0)
	{
		if (pCSectrumData->m_gd_peak_freq != 0.0){
#if(1)
			d_freq = pCSectrumData->m_gd_peak_freq - pCSectrumData->m_gd_center_freq;
#else
			d_freq = m_gd_peak_freq - 70000000.0;
#endif
		}
		else {
			d_freq = 0.0;
		}
	}
	else {
		d_freq = pCSectrumData->m_gd_peak_freq;
	}
	d_freq = (double)((long long)(d_freq *  (double)1000.0)) / (double)1000.0;
//	pCSectrumData->m_gd_freq_ofs_in = d_freq;
//	str = CSatelliteData::AddCanma(pCSectrumData->m_gd_freq_ofs_in, 3);
//	m_CEditCenterFreqOffset.SetWindowText(str);
//	m_colEditCenterFreqOffset = m_colText[eBlack];

	// ここで装置に変更したFreqOffsetの制御を送る
	CString strVal;
	if (m_nStationID == eStation_Usuda642)
	{
		// 54mの場合
		strVal = ExchgCtrlValSpecFreOffset54m(d_freq);
	}
	else
	{
		// 54m以外の場合
		strVal.Format(_T("%.3f"), d_freq);
	}
	if (SendShell(CString(CTL_FREQ_OFFSET_NAME), strVal) == 0)
	{
		m_bCFOfstModified = FALSE;
		UpdateCtrl();
	}

//	// ダミーオフセット生成
//	pCSectrumData->CreateDummyOffset();
//
//	// リセットしたオフセットは0に戻す
//	pCSectrumData->m_gd_freq_ofs_in = 0.0;
//	CString str = CSatelliteData::AddCanma(pCSectrumData->m_gd_freq_ofs_in, 3);
//	m_CEditCenterFreqOffset.SetWindowText(str);
//	m_colEditCenterFreqOffset = m_colText[eBlack];
	
	if (this->GetSafeHwnd())
	{
		Invalidate();
	}
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# 周波数値の単位変換

@param	src：変換前の周波数値
@param	dst：変換後の周波数値
@retval	単位 0：Hz / 1：KHz / 2：MHz

*/
/*============================================================================*/
int CSpectrumCommonDlg::ExchangeFreqUnit(double& src, double& dst)
{
	dst = src;
	for (int i = 0; i <= eMHz; i++)
	{
		if (dst > 1000.0)
		{
			dst = dst / 1000.0;
		}
		else
		{
			return i;
		}
	}

	return eMHz;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# シェル送信

@param	strCtrl：設備文字列
@param	strVal：送信データ文字列
@retval	なし

*/
/*============================================================================*/
int CSpectrumCommonDlg::SendShell(CString& strCtrl, CString& strVal)
{
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = SEND_CONTROL_SH;
	stParam.strRemShellParam.Format(_T("%s %s %s"), (LPCTSTR)CString(mStationSimpleString[m_nStationID]).MakeLower(), (LPCTSTR)strCtrl, (LPCTSTR)strVal);

	int err = CSshShell::ExecShell(stParam);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CSpectrumCommonDlg"), _T("SendShell"), _T("debug"), strDebug, nLogEx::debug);
	}

	return err;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# スペクトラム設定データ取得

@param	なし
@retval	スペクトラム設定データ

*/
/*============================================================================*/
CSpectrumViewData* CSpectrumCommonDlg::GetSpectrumViewData()
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
/*! CSpectrumCommonDlg

-# 中心周波数オフセットのエディット変更時に呼ばれる

@param	なし
@retval なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnEnChangeSpectrumviewEdit7()
{
	// 更新した
	m_bCFOfstModified = TRUE;
	m_colEditCenterFreqOffset = m_colText[eOrange];
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# 入力端換算値のエディット変更時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnEnChangeSpectrumInputexchgedit()
{
	m_colEditInputExchg = m_colText[eOrange];
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# ATT値のエディット変更時に呼ばれる

@param  なし
@retval なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnEnChangeSpectrumAttedit()
{
	m_colEditAtt = m_colText[eOrange];
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# メニューからのファイルクローズ時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CSpectrumCommonDlg::OnMenuClose()
{
	//	OnOK();
	ShowWindow(SW_HIDE);
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
HBRUSH CSpectrumCommonDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CSpectrumCommonDlg::CtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_EDIT)
	{
		if (*pWnd == m_CEditCenterFreqOffset)
		{
			pDC->SetTextColor(m_colEditCenterFreqOffset);
		}
		else if (*pWnd == m_CEditInputExchg)
		{
			pDC->SetTextColor(m_colEditInputExchg);
		}
		else if (*pWnd == m_CEditAtt)
		{
			pDC->SetTextColor(m_colEditAtt);
		}
	}
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# 実行ボタン押下時によばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnBnClickedSpectrumExecButton()
{
	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return;
	}

	CString str;
	long l_y_grid_max, l_y_grid_min, l_y_grid_val;
	double d_freq;

	if (m_CCBoxMonitorPoint.GetSafeHwnd() == NULL ||
		m_CCBoxBandWidth.GetSafeHwnd() == NULL ||
		m_CEditCenterFreqOffset.GetSafeHwnd() == NULL)
	{
		return;
	}

	l_y_grid_max = 0;
	l_y_grid_min = 0;
	l_y_grid_val = 0;

	// 中心周波数オフセットチェック
	m_CEditCenterFreqOffset.GetWindowText(str);
	str.Trim();
	str.Remove(_T(','));
	d_freq = _ttof(str);

	if ((d_freq < SPECTRUM_FREQ_OFS_MIN) || (d_freq > SPECTRUM_FREQ_OFS_MAX))
	{
		// エラーダイアログ表示
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return;
		if (strMsg.LoadString(IDS_SPECTRUMVIEW_CENTERFREQOFFSET_ERR) == FALSE)
			return;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
		return;
	}

	// 確認ダイアログ表示
	{
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return;
		if (strMsg.LoadString(IDS_CHECK_CTRL_MSG) == FALSE)
			return;
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, strMsg, strTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return;
	}

	// 設定値の反映
	d_freq = (double)((long long)(d_freq *  (double)1000.0)) / (double)1000.0;

	int nMonitorPointCurSel = m_CCBoxMonitorPoint.GetCurSel();
	int nBandWidthCurSel = m_CCBoxBandWidth.GetCurSel();

//ifdef _DEBUG
//	pCSectrumData->m_dummy_moni_point = nMonitorPointCurSel;
//	pCSectrumData->m_dummy_bandw = nBandWidthCurSel;
//	pCSectrumData->m_dummy_freq_ofs = d_freq;
//	pCSectrumData->m_dummy_center_freq = d_freq;
//endif

	// MonitorPointの制御を送る
	if (SendShell(CString(CTL_MONI_POINT_NAME), spectrum_gpsz_moni_point_str[nMonitorPointCurSel]) == 0)
	{
		m_bMPModified = FALSE;
		UpdateCtrl();
	}

	// BandWidthの制御を送る
	if (SendShell(CString(CTL_MEASURE_BW_NAME), spectrum_gast_bandw[nBandWidthCurSel].sz_sts) == 0)
	{
		m_bBWModified = FALSE;
		UpdateCtrl();
	}

	// FreqOffsetの制御を送る
	CString strVal;
	if (m_nStationID == eStation_Usuda642)
	{
		// 54mの場合
		strVal = ExchgCtrlValSpecFreOffset54m(d_freq);
	}
	else
	{
		// 54m以外の場合
		strVal.Format(_T("%.3f"), d_freq);
	}
	if (SendShell(CString(CTL_FREQ_OFFSET_NAME), strVal) == 0)
	{ 
		m_bCFOfstModified = FALSE;
		UpdateCtrl();
	}

//#ifdef _DEBUG
//	// ダミーオフセット生成
//	pCSectrumData->CreateDummyOffset();
//#endif

	if (this->GetSafeHwnd())
	{
		Invalidate();
	}

//	// ファイル保存
//	theApp.GetSatelliteData().SpectrumFileWrite(m_nStationID);
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# 54mのスペクトラム周波数オフセット値を制御文字列に変換する

@param	freq：周波数
@retval	制御文字列

*/
/*============================================================================*/
CString CSpectrumCommonDlg::ExchgCtrlValSpecFreOffset54m(double freq)
{
	CString strVal;

	// bit40に符号
	int signVal;
	double d_freqAbs = fabs(freq);
	if (freq >= 0)
	{
		signVal = 1;
	}
	else
	{
		signVal = 0;
	}

	// bit12～39が整数部
	int intVal = (int)d_freqAbs;

	// bit0～11が小数部
	double decVal = d_freqAbs - (double)intVal;
	// 小数点以下３桁のみ有効
	decVal *= 1000;

	strVal.Format(_T("0x%01d%07d%03d"), signVal, intVal, (int)round(decVal));

	return strVal;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# リセットボタン押下時によばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnBnClickedSpectrumResetButton()
{
	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return;
	}

	CString str;
	str = CSatelliteData::AddCanma(pCSectrumData->m_gd_freq_ofs_in, 3);
	m_CEditCenterFreqOffset.SetWindowText(str);
	m_colEditCenterFreqOffset = m_colText[eBlack];

	m_CCBoxMonitorPoint.SetCurSel(m_nMonitorPointCurSel);
	m_CCBoxMonitorPoint.SetSelectedTextColor(m_colText[eBlack]);

	m_CCBoxBandWidth.SetCurSel(m_nBandWidthCurSel);
	m_CCBoxBandWidth.SetSelectedTextColor(m_colText[eBlack]);

	m_bMPModified = FALSE;
	m_bBWModified = FALSE;
	m_bCFOfstModified = FALSE;
	m_bMPMListOpen = FALSE;
	m_bBWMListOpen = FALSE;
	m_bCFOfstMListOpen = FALSE;

	if (this->GetSafeHwnd())
	{
		Invalidate();
	}
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# 入力端換算実行ボタン押下時によばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnBnClickedSpectrumInputexchgExecButton()
{
	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return;
	}

	CString str;
	double d_y_offset_val, d_y_offset_att;

	// Y軸オフセット値チェック
	m_CEditInputExchg.GetWindowText(str);
	str.Trim();
	str.Remove(_T(','));
	d_y_offset_val = _ttof(str);

	if (d_y_offset_val > (double)SPECTRUM_INPUTMAX || d_y_offset_val < (double)SPECTRUM_INPUTMIN)
	{
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return;
		if (strMsg.LoadString(IDS_YOFFSET_VALUE_ILLEGAL) == FALSE)
			return;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
		return;
	}

	// ATT値チェック
	m_CEditAtt.GetWindowText(str);
	str.Trim();
	str.Remove(_T(','));
	d_y_offset_att = _ttof(str);

	if (d_y_offset_att > (double)SPECTRUM_INPUTMAX || d_y_offset_att < (double)SPECTRUM_INPUTMIN)
	{
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return;
		if (strMsg.LoadString(IDS_ATT_VALUE_ILLEGAL) == FALSE)
			return;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
		return;
	}

	// 確認ダイアログ表示
	{
		CString strTitle;
		if (strTitle.LoadString(IDS_INPUTEXCHG) == FALSE)
			return;
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("実行します。\nよろしいですか？"), strTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return;
	}

	// 設定値の反映
	d_y_offset_val = (double)((long long)(d_y_offset_val *  (double)100.0)) / (double)100.0;
	pCSectrumData->m_stVScale.gd_y_offset_val = d_y_offset_val;
	d_y_offset_att = (double)((long long)(d_y_offset_att *  (double)100.0)) / (double)100.0;
	pCSectrumData->m_gd_y_offset_att = d_y_offset_att;

	str = CSatelliteData::AddCanma(pCSectrumData->m_stVScale.gd_y_offset_val, 2);
	m_CEditInputExchg.SetWindowText(str);
	str = CSatelliteData::AddCanma(pCSectrumData->m_gd_y_offset_att, 2);
	m_CEditAtt.SetWindowText(str);

	m_colEditInputExchg = m_colText[eBlack];
	m_colEditAtt = m_colText[eBlack];

	if (this->GetSafeHwnd())
	{
		ClearAutoVMaxMin();
		Invalidate();
	}

//	// ファイル保存
//	theApp.GetSatelliteData().SpectrumFileWrite(m_nStationID);
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# 入力端換算リセットボタン押下時によばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnBnClickedSpectrumInputexchgResetButton()
{
	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return;
	}

	CString str;

	str = CSatelliteData::AddCanma(pCSectrumData->m_stVScale.gd_y_offset_val, 2);
	m_CEditInputExchg.SetWindowText(str);
	m_colEditInputExchg = m_colText[eBlack];

	str = CSatelliteData::AddCanma(pCSectrumData->m_gd_y_offset_att, 2);
	m_CEditAtt.SetWindowText(str);
	m_colEditAtt = m_colText[eBlack];
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# 入力端換算スピンコントロール変更時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnDeltaposSpectrumInputexchgspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// 入力端換算値を変更する
	if (m_CSpinInputExchg.GetSafeHwnd() == NULL)
	{
		return;
	}

	CString str;
	m_CEditInputExchg.GetWindowText(str);
	str.Trim();
	str.Remove(_T(','));
	double val = _ttof(str);

	m_CSpinInputExchg.SetPos32((int)round(val));

	*pResult = 0;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# ATT値スピンコントロール変更時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnDeltaposSpectrumAttspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// ATT値を変更する
	if (m_CSpinAtt.GetSafeHwnd() == NULL)
	{
		return;
	}

	CString str;
	m_CEditAtt.GetWindowText(str);
	str.Trim();
	str.Remove(_T(','));
	double val = _ttof(str);

	m_CSpinAtt.SetPos32((int)round(val));

	*pResult = 0;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# MONITOR POINTコンボボックスが選択変更されたときに呼ばれる

@param  なし
@retval	なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnCbnSelchangeSpectrumviewCombo1()
{
	// 更新した
	m_bMPModified = TRUE;
	m_CCBoxMonitorPoint.SetSelectedTextColor(m_colText[eOrange]);

	if (this->GetSafeHwnd())
	{
		m_CCBoxMonitorPoint.Invalidate();
	}
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# BAND WIDTHコンボボックスが選択変更されたときに呼ばれる

@param  なし
@retval	なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnCbnSelchangeSpectrumviewCombo2()
{
	// 更新した
	m_bBWModified = TRUE;
	m_CCBoxBandWidth.SetSelectedTextColor(m_colText[eOrange]);

	if (this->GetSafeHwnd())
	{
		m_CCBoxBandWidth.Invalidate();
	}
}

/*============================================================================*/
/*! CSpectrumCommonDlg

-# レイアウト読み込み（設定読み込み）

@param  なし
@retval	なし

*/
/*============================================================================*/
bool CSpectrumCommonDlg::Load(CArchive& ar)
{
	return true;
}

/*============================================================================*/
/*! CSpectrumCommonDlg

-# レイアウト保存（設定保存）

@param  なし
@retval	なし

*/
/*============================================================================*/
bool CSpectrumCommonDlg::Save(CArchive& ar)
{
	return true;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# 制御更新

@param  なし
@retval	TRUE:監視制御が取得できた／FALSE:制御監視が取得できなかった

*/
/*============================================================================*/
BOOL CSpectrumCommonDlg::UpdateCtrl()
{
	BOOL ret = FALSE;

	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return ret;
	}

	/* 監視データ取得 */
	if (pCSectrumData->GetSpectrumData() != ERR_RETURN)
	{
		CString str;

		// MonitorPointをユーザが変更していない場合
//		if (m_CCBoxMonitorPoint.GetSelectedTextColor() == m_colText[eBlack])
		if (m_bMPModified == FALSE && m_bMPMListOpen == FALSE)
		{
			for (int i = 0; i < sizeof(spectrum_gpsz_moni_point_str) / sizeof(spectrum_gpsz_moni_point_str[0]); i++)
			{
				// 監視文字列比較
				if (pCSectrumData->m_gsz_moni_point.Compare(spectrum_gpsz_moni_point_str[i]) == 0)
				{
					// インデックス位置に差異がある場合描画
					if (m_nMonitorPointCurSel != i)
					{
						// 監視値を表示
						m_nMonitorPointCurSel = i;
						m_CCBoxMonitorPoint.SetCurSel(i);
						m_CCBoxMonitorPoint.SetSelectedTextColor(m_colText[eBlack]);
						m_CCBoxMonitorPoint.Invalidate();
					}
					break;
				}
			}
		}

		// BandWidthをユーザが変更していない場合
//		if (m_CCBoxBandWidth.GetSelectedTextColor() == m_colText[eBlack])
		if (m_bBWModified == FALSE && m_bBWMListOpen == FALSE)
		{
			for (int i = 0; spectrum_gast_bandw[i].sz_sts[0] != NULL; i++)
			{
				// 監視文字列比較
				if (pCSectrumData->m_gsz_bandw_sts.Compare(spectrum_gast_bandw[i].sz_sts) == 0)
				{
					// インデックス位置に差異がある場合描画
					if (m_nBandWidthCurSel != i)
					{
						// 監視値を表示
						m_nBandWidthCurSel = i;
						m_CCBoxBandWidth.SetCurSel(i);
						m_CCBoxBandWidth.SetSelectedTextColor(m_colText[eBlack]);
						m_CCBoxBandWidth.Invalidate();
					}
					break;
				}
			}
		}

		// FreqOffsetをユーザが変更していない場合
//		if (m_colEditCenterFreqOffset == m_colText[eBlack])
		if (m_bCFOfstModified == FALSE)
		{
			// 監視値を表示
			str = CSatelliteData::AddCanma(pCSectrumData->m_gd_freq_ofs_in, 3);
			CString strWndtxt;
			m_CEditCenterFreqOffset.GetWindowText(strWndtxt);
			// 画面の文字列と差異がある場合描画
			bool bUpdate = false;
			if (strWndtxt.Compare(str) != 0)
			{
				m_CEditCenterFreqOffset.SetWindowText(str);
				bUpdate = true;
			}
			if (m_colEditCenterFreqOffset != m_colText[eBlack])
			{
				m_colEditCenterFreqOffset = m_colText[eBlack];
				bUpdate = true;
			}
			m_bCFOfstModified = FALSE;
			if (bUpdate)
			{
				m_CEditCenterFreqOffset.Invalidate();
			}
		}

		ret = TRUE;
	}

	return ret;
}


/*============================================================================*/
/*! CSpectrumView

-# Monitor Pointのプリダウンリストを開いたときに呼ばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnCbnDropdownSpectrumviewCombo1()
{
	// リストが開いている
	m_bMPMListOpen = TRUE;
}


/*============================================================================*/
/*! CSpectrumView

-# Monitor Pointのプリダウンリストを閉じた時に呼ばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnCbnCloseupSpectrumviewCombo1()
{
	// リストが閉じている
	m_bMPMListOpen = FALSE;
}


/*============================================================================*/
/*! CSpectrumView

-# Band Widthのプリダウンリストを開いたときに呼ばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnCbnDropdownSpectrumviewCombo2()
{
	// リストが開いている
	m_bBWMListOpen = TRUE;
}


/*============================================================================*/
/*! CSpectrumView

-# Band Widthのプリダウンリストを閉じた時に呼ばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnCbnCloseupSpectrumviewCombo2()
{
	// リストが閉じている
	m_bBWMListOpen = FALSE;
}


