/*============================================================================*/
/*! TabDsnModemView54.cpp

-# 捕捉状況ステータス画面（DSN-MODEM）
*/
/*============================================================================*/
// TabDsnModemView54.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabDsnModemView54.h"
#include "afxdialogex.h"


// CTabDsnModemView54 ダイアログ

IMPLEMENT_DYNCREATE(CTabDsnModemView54, CDialogEx)

CTabDsnModemView54::CTabDsnModemView54(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabDsnModemView54::IDD, pParent)
{

}

CTabDsnModemView54::~CTabDsnModemView54()
{
}

void CTabDsnModemView54::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CARRVAL, mCarrVal);
	DDX_Control(pDX, IDC_STATIC_SUBCARRVAL, mSubCarrVal);
	DDX_Control(pDX, IDC_STATIC_VITERBIVAL, mViterbiVal);
	DDX_Control(pDX, IDC_STATIC_BITVAL, mBitVal);
	DDX_Control(pDX, IDC_STATIC_FRAMEVAL, mFrameVal);
	DDX_Control(pDX, IDC_STATIC_AGC, mAgcVal);
	DDX_Control(pDX, IDC_STATIC_EB, mEbVal);
	DDX_Control(pDX, IDC_STATIC_RTT, mTimeVal);
	DDX_Control(pDX, IDC_STATIC_TXFREQ, mTxFreqVal);
	DDX_Control(pDX, IDC_STATIC_RXFREQ, mRxFreqVal);
}


BEGIN_MESSAGE_MAP(CTabDsnModemView54, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PLLBW, &CTabDsnModemView54::OnBnClickedButtonPllbw)
	ON_BN_CLICKED(IDC_BUTTON_AGCBW, &CTabDsnModemView54::OnBnClickedButtonAgcbw)
	ON_BN_CLICKED(IDC_BUTTON_BITRATE, &CTabDsnModemView54::OnBnClickedButtonBitrate)
	ON_BN_CLICKED(IDC_BUTTON_RXROUTE, &CTabDsnModemView54::OnBnClickedButtonRxroute)
	ON_BN_CLICKED(IDC_BUTTON_CMDSC, &CTabDsnModemView54::OnBnClickedButtonCmdsc)
	ON_BN_CLICKED(IDC_BUTTON_TLMSC, &CTabDsnModemView54::OnBnClickedButtonTlmsc)
	ON_BN_CLICKED(IDC_BUTTON_REJECT, &CTabDsnModemView54::OnBnClickedButtonReject)
	ON_BN_CLICKED(IDC_BUTTON_XCMDOUT, &CTabDsnModemView54::OnBnClickedButtonXcmdout)
	ON_BN_CLICKED(IDC_BUTTON_XTLMIN, &CTabDsnModemView54::OnBnClickedButtonXtlmin)
	ON_BN_CLICKED(IDC_BUTTON_BITRATE_KA, &CTabDsnModemView54::OnBnClickedButtonBitrateKa)
END_MESSAGE_MAP()


// CTabDsnModemView54 メッセージ ハンドラー

BOOL CTabDsnModemView54::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	mCarrVal.SetGradient(false);
	mCarrVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mCarrVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
	mCarrVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mSubCarrVal.SetGradient(false);
	mSubCarrVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mSubCarrVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
	mSubCarrVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mViterbiVal.SetGradient(false);
	mViterbiVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mViterbiVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
	mViterbiVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mBitVal.SetGradient(false);
	mBitVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mBitVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
	mBitVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mFrameVal.SetGradient(false);
	mFrameVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mFrameVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
	mFrameVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mAgcVal.SetGradient(false);
	mAgcVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mAgcVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mAgcVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mEbVal.SetGradient(false);
	mEbVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mEbVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mEbVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mTimeVal.SetGradient(false);
	mTimeVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mTimeVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mTimeVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mTxFreqVal.SetGradient(false);
	mTxFreqVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mTxFreqVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mTxFreqVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mRxFreqVal.SetGradient(false);
	mRxFreqVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mRxFreqVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mRxFreqVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	theApp.SetMonitorMode(this);

	InitStatus();

	GetDlgItem(IDC_STATIC_XCMDOUT2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_XTLMIN2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_XCMDOUT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_XTLMIN)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! DSN MODEM

-# PLL BWボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabDsnModemView54::OnBnClickedButtonPllbw()
{
	ControlParamExecute(1);
}


/*============================================================================*/
/*! DSN MODEM

-# AGC BWボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabDsnModemView54::OnBnClickedButtonAgcbw()
{
	ControlParamExecute(2);
}


/*============================================================================*/
/*! DSN MODEM

-# BIT RATE Xボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabDsnModemView54::OnBnClickedButtonBitrate()
{
	ControlParamExecute(3);
}


/*============================================================================*/
/*! DSN MODEM

-# RX ROUTEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabDsnModemView54::OnBnClickedButtonRxroute()
{
	ControlParamExecute(4);
}


/*============================================================================*/
/*! DSN MODEM

-# CMD SCボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabDsnModemView54::OnBnClickedButtonCmdsc()
{
	ControlParamExecute(5);
}


/*============================================================================*/
/*! DSN MODEM

-# TLM SCボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabDsnModemView54::OnBnClickedButtonTlmsc()
{
	ControlParamExecute(6);
}


/*============================================================================*/
/*! DSN MODEM

-# REJECTボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabDsnModemView54::OnBnClickedButtonReject()
{
	ControlParamExecute(0);
}


/*============================================================================*/
/*! DSN MODEM

-# X-CMDOUT2ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabDsnModemView54::OnBnClickedButtonXcmdout()
{
	ControlParamExecute(7);
}


/*============================================================================*/
/*! DSN MODEM

-# X-TLMIN2ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabDsnModemView54::OnBnClickedButtonXtlmin()
{
	ControlParamExecute(8);
}

/*============================================================================*/
/*! DSN MODEM

-# BIT RATE Kaボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabDsnModemView54::OnBnClickedButtonBitrateKa()
{
	ControlParamExecute(9);
}

/*============================================================================*/
/*! X帯受信(新系)

-# 制御パラメタの実行

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabDsnModemView54::ControlParamExecute(UINT pos)
{
	CString mControlString = mDsnModemParam54[pos].nameS;
	CString str;
	GetDlgItem(mDsnModemParam54[pos].id)->GetWindowText(str);

	if (mDsnModemParam54[pos].id == IDC_BUTTON_PLLBW)
	{
		if (str.CompareNoCase(_T("1000Hz")) == 0)
		{
			str = _T("1kHz");
		}
	}

	stControlValue val;
	val.ret = 0;
	val.type = mDsnModemParam54[pos].type;
	switch (val.type){
	case	eControlStatus:
		sprintf_s(val.buf, 80, "%s", (LPCSTR)CStringA(str));
		break;
	case	eControlNumeric:
		str.Remove(',');
		val.val = _wtoi(str);
		break;
	case	eControlDouble:
		str.Remove(',');
		val.fval = _wtof(str);
		break;
	case	eControlTime:
		sprintf_s(val.buf, 80, "%s", (LPCSTR)CStringA(str));
		break;
	case	eControlCommand:
		theApp.SendControlChkDlg(mControlString, _T("REJECT"), _T(""), _T(""));
		return;
		break;
	}

	theApp.GetSatelliteData().SendControlSelect(mControlString, &val);
	if (val.ret == 0)
		return;

	switch (val.type){
	case	eControlStatus:
		str.Format(_T("%s"), (LPCTSTR)CString(val.buf));
		break;
	case	eControlNumeric:
		str.Format(_T("%d"), val.val);
		break;
	case	eControlDouble:
		str.Format(_T("%s"), (LPCTSTR)CString(val.buf));
		break;
	case	eControlTime:
		str.Format(_T("%s"), (LPCTSTR)CString(val.buf));
		break;
	}

	if (pos == 1 /* PLL BW(CURRENT_LBW) */){
		if (str.CompareNoCase(CURRENT_LBW) == 0){
			// テーブルファイルをコピーするシェルのみを送る
			theApp.SendControlDlgCURRENT_LBW(mControlString, _T(""), _T(""), _T(""));
			return;
		}
		else{
			// 制御とテーブルファイルをコピーするシェルを送る
			theApp.SendControlDlgPLL_BW(mControlString, str, _T(""), _T(""));
			return;
		}
	}

	theApp.SendControlChkDlg(mControlString, str, _T(""), _T(""));
}


/*============================================================================*/
/*! X帯受信(新系)

-# 捕捉状況ステータスの更新

@param
@retval

*/
/*============================================================================*/
void CTabDsnModemView54::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mDsnModemParam54) / sizeof(mDsnModemParam54[0]);

	for (int i = 0; i < szControl; i++)
	{
		if (mDsnModemParam54[i].id == IDC_BUTTON_REJECT)
			continue;

		CString mControlString = mDsnModemParam54[i].nameK;
		CString str;
		GetDlgItem(mDsnModemParam54[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mDsnModemParam54[i].type;

		if (mControlString.IsEmpty())
		{
			continue;
		}

		UINT sta = theApp.GetSelectStation();
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, mControlString);

		if (obs_adr == NULL)
		{
			continue;
		}

		switch (val.type){
		case	eControlStatus:
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		case	eControlNumeric:
			str = CSatelliteData::AddCanma((double)obs_adr->l_data, 3);
			break;
		case	eControlDouble:
			if (mDsnModemParam54[i].id == IDC_STATIC_AGC ||
				mDsnModemParam54[i].id == IDC_STATIC_EB)
			{
				str = CSatelliteData::AddCanma(obs_adr->d_data, 3);
			}
			else if (mDsnModemParam54[i].id == IDC_BUTTON_BITRATE || mDsnModemParam54[i].id == IDC_BUTTON_BITRATE_KA)
			{
				str = CSatelliteData::AddCanma(obs_adr->d_data, 4);
			}
			else
			{
				str = CSatelliteData::AddCanma(obs_adr->d_data, 0);
			}
			break;
		case	eControlTime:
			if (mDsnModemParam54[i].id == IDC_STATIC_DOPPLER)
			{
//				time_t t = (time_t)obs_adr->ld_data;
//				CTime ctime = CTime(t);
//				str = _T("DOPPLER[") + ctime.Format("%H:%M:%S") + _T("]");
				str.Format(_T("DOPPLER[%s]"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			}
			else
			{
				str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			}
			break;
		}

		if (mDsnModemParam54[i].id == IDC_STATIC_CARRVAL)
		{
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				mCarrVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				mCarrVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
			}
		}

		if (mDsnModemParam54[i].id == IDC_STATIC_SUBCARRVAL)
		{
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				mSubCarrVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				mSubCarrVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
			}
		}

		if (mDsnModemParam54[i].id == IDC_STATIC_VITERBIVAL)
		{
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				mViterbiVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				mViterbiVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
			}
		}

		if (mDsnModemParam54[i].id == IDC_STATIC_BITVAL)
		{
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				mBitVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				mBitVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
			}
		}

		if (mDsnModemParam54[i].id == IDC_STATIC_FRAMEVAL)
		{
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				mFrameVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				mFrameVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
			}
		}

		if (mDsnModemParam54[i].id == IDC_BUTTON_PLLBW)
		{
			if (str.CompareNoCase(_T("1kHz")) == 0)
			{
				str = _T("1000Hz");
			}
		}

		if (mDsnModemParam54[i].id == IDC_STATIC_EB ||
			mDsnModemParam54[i].id == IDC_STATIC_CARRVAL ||
			mDsnModemParam54[i].id == IDC_STATIC_SUBCARRVAL ||
			mDsnModemParam54[i].id == IDC_STATIC_VITERBIVAL ||
			mDsnModemParam54[i].id == IDC_STATIC_BITVAL ||
			mDsnModemParam54[i].id == IDC_STATIC_FRAMEVAL ||
			mDsnModemParam54[i].id == IDC_STATIC_AGC ||
			mDsnModemParam54[i].id == IDC_STATIC_RTT ||
			mDsnModemParam54[i].id == IDC_STATIC_TXFREQ ||
			mDsnModemParam54[i].id == IDC_STATIC_RXFREQ)
			((CColorStatic*)GetDlgItem(mDsnModemParam54[i].id))->SetText(str);
		else
			GetDlgItem(mDsnModemParam54[i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! X帯受信(新系)

-# 捕捉状況ステータスの初期化

@param
@retval

*/
/*============================================================================*/
void CTabDsnModemView54::InitStatus()
{
	int szControl = sizeof(mDsnModemParam54) / sizeof(mDsnModemParam54[0]);

	for (int i = 0; i < szControl; i++)
	{
		if (mDsnModemParam54[i].id == IDC_BUTTON_REJECT)
			continue;

		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mDsnModemParam54[i].type;

		switch (val.type){
		case	eControlStatus:
			str.Format(_T(""));
			break;
		case	eControlNumeric:
			str.Format(_T(""));
			break;
		case	eControlDouble:
			str.Format(_T(""));
			break;
		case	eControlTime:
			if (mDsnModemParam54[i].id == IDC_STATIC_DOPPLER)
			{
				str = _T("DOPPLER[--- --:--:--]");
				GetDlgItem(mDsnModemParam54[i].id)->SetWindowText(str);
			}
			else
			{
				str.Format(_T(""));
			}
			break;
		}

		if (mDsnModemParam54[i].id == IDC_STATIC_CARRVAL)
		{
			mCarrVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mDsnModemParam54[i].id == IDC_STATIC_SUBCARRVAL)
		{
			mSubCarrVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mDsnModemParam54[i].id == IDC_STATIC_VITERBIVAL)
		{
			mViterbiVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mDsnModemParam54[i].id == IDC_STATIC_BITVAL)
		{
			mBitVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mDsnModemParam54[i].id == IDC_STATIC_FRAMEVAL)
		{
			mFrameVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mDsnModemParam54[i].id == IDC_STATIC_EB ||
			mDsnModemParam54[i].id == IDC_STATIC_CARRVAL ||
			mDsnModemParam54[i].id == IDC_STATIC_SUBCARRVAL ||
			mDsnModemParam54[i].id == IDC_STATIC_VITERBIVAL ||
			mDsnModemParam54[i].id == IDC_STATIC_BITVAL ||
			mDsnModemParam54[i].id == IDC_STATIC_FRAMEVAL ||
			mDsnModemParam54[i].id == IDC_STATIC_AGC ||
			mDsnModemParam54[i].id == IDC_STATIC_RTT ||
			mDsnModemParam54[i].id == IDC_STATIC_TXFREQ ||
			mDsnModemParam54[i].id == IDC_STATIC_RXFREQ)
		{
			if (str.IsEmpty())
				str.Format(_T(" "));
			((CColorStatic*)GetDlgItem(mDsnModemParam54[i].id))->SetText(str);
		}
		else
		{
			GetDlgItem(mDsnModemParam54[i].id)->SetWindowText(str);
		}
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! X帯受信(新系)

-# 捕捉状況ステータスの更新

@param  なし
@retval なし
*/
/*============================================================================*/
LRESULT CTabDsnModemView54::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_ThreadNormal:
		UpdateStatus();
		return TRUE;
	default:
		break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


