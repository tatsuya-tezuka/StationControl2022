// TabSrecvView34.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabXcaptView34.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"


// CTabXcaptView34 ダイアログ

IMPLEMENT_DYNCREATE(CTabXcaptView34, CDialogEx)

CTabXcaptView34::CTabXcaptView34(CWnd* pParent /*=NULL*/)
: CDialogEx(CTabXcaptView34::IDD, pParent)
{

}

CTabXcaptView34::~CTabXcaptView34()
{
}

void CTabXcaptView34::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CARRVAL, mCarrVal);
	DDX_Control(pDX, IDC_STATIC_CARRVAL2, mCarrVal2);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_AGC, mAgcVal);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_AGC2, mAgcVal2);
}


BEGIN_MESSAGE_MAP(CTabXcaptView34, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_REJECT, &CTabXcaptView34::OnBnClickedButtonReject)
	ON_BN_CLICKED(IDC_BUTTON_PLLBW, &CTabXcaptView34::OnBnClickedButtonPllbw)
	ON_BN_CLICKED(IDC_BUTTON_AGCBW, &CTabXcaptView34::OnBnClickedButtonAgcbw)
	ON_BN_CLICKED(IDC_BUTTON_BITRATE, &CTabXcaptView34::OnBnClickedButtonBitrate)
	ON_BN_CLICKED(IDC_BUTTON_DCCTRL, &CTabXcaptView34::OnBnClickedButtonDcctrl)
	ON_BN_CLICKED(IDC_BUTTON_RX, &CTabXcaptView34::OnBnClickedButtonRx)
	ON_BN_CLICKED(IDC_BUTTON_DC, &CTabXcaptView34::OnBnClickedButtonDc)
END_MESSAGE_MAP()


// CTabXcaptView34 メッセージ ハンドラー

BOOL CTabXcaptView34::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	mCarrVal.SetGradient(false);
	mCarrVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mCarrVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
	mCarrVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mCarrVal2.SetGradient(false);
	mCarrVal2.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mCarrVal2.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
	mCarrVal2.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mAgcVal.SetGradient(false);
	mAgcVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mAgcVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mAgcVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mAgcVal2.SetGradient(false);
	mAgcVal2.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mAgcVal2.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mAgcVal2.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	theApp.SetMonitorMode(this);

	InitStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! S帯受信

-# REJECTボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXcaptView34::OnBnClickedButtonReject()
{
	ControlParamExecute(0);
}


/*============================================================================*/
/*! S帯受信

-# PLL BWボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXcaptView34::OnBnClickedButtonPllbw()
{
	ControlParamExecute(1);
}


/*============================================================================*/
/*! S帯受信

-# AGC BWボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXcaptView34::OnBnClickedButtonAgcbw()
{
	ControlParamExecute(2);
}


/*============================================================================*/
/*! S帯受信

-# BITRATEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXcaptView34::OnBnClickedButtonBitrate()
{
	ControlParamExecute(3);
}


/*============================================================================*/
/*! S帯受信

-# D/C CTRLボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXcaptView34::OnBnClickedButtonDcctrl()
{
	ControlParamExecute(4);
}


/*============================================================================*/
/*! S帯受信

-# RXボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXcaptView34::OnBnClickedButtonRx()
{
	ControlParamExecute(5);
}


/*============================================================================*/
/*! S帯受信

-# D/Cボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXcaptView34::OnBnClickedButtonDc()
{
	ControlParamExecute(6);
}

/*============================================================================*/
/*! S帯受信

-# 制御パラメタの実行

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXcaptView34::ControlParamExecute(UINT pos)
{
	CString mControlString = mXcapt34Param[pos].nameS;
	CString str;
	GetDlgItem(mXcapt34Param[pos].id)->GetWindowText(str);
	stControlValue val;
	val.ret = 0;
	val.type = mXcapt34Param[pos].type;
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
//		theApp.SendControlNoUIChkDlg(mControlString, _T(","), _T(""), _T(""));
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
#if 1
		str.Format(_T("%s"), (LPCTSTR)CString(val.buf));
#else
		str.Format(_T("%f"), val.fval);
#endif
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
/*! S帯受信

-# 捕捉状況ステータスの更新

@param
@retval

*/
/*============================================================================*/
void CTabXcaptView34::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	//	if (sd.stn_tbl_access() == 1)
	//	{
	//		return;
	//	}

	int szControl = sizeof(mXcapt34Param) / sizeof(mXcapt34Param[0]);

	for (int i = 0; i < szControl; i++)
	{
		if (mXcapt34Param[i].id == IDC_BUTTON_REJECT)
			continue;

		CString mControlString = mXcapt34Param[i].nameK;
		CString str;
		GetDlgItem(mXcapt34Param[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mXcapt34Param[i].type;

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
			str = CSatelliteData::AddCanma(obs_adr->d_data, 3);
			break;
		case	eControlTime:
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		}

		if (mXcapt34Param[i].id == IDC_STATIC_CARRVAL){
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
				mCarrVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			else
				mCarrVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
		}
		if (mXcapt34Param[i].id == IDC_STATIC_CARRVAL2){
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
				mCarrVal2.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			else
				mCarrVal2.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
		}

		if (mXcapt34Param[i].id == IDC_STATIC_CARRVAL ||
			mXcapt34Param[i].id == IDC_STATIC_CARRVAL2 ||
			mXcapt34Param[i].id == IDC_MFCMASKEDEDIT_AGC ||
			mXcapt34Param[i].id == IDC_MFCMASKEDEDIT_AGC2){
			((CColorStatic*)GetDlgItem(mXcapt34Param[i].id))->SetText(str);
		}
		else{
			GetDlgItem(mXcapt34Param[i].id)->SetWindowText(str);
		}
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! S帯受信

-# 捕捉状況ステータスの初期化

@param
@retval

*/
/*============================================================================*/
void CTabXcaptView34::InitStatus()
{
	int szControl = sizeof(mXcapt34Param) / sizeof(mXcapt34Param[0]);

	for (int i = 0; i < szControl; i++)
	{
		if (mXcapt34Param[i].id == IDC_BUTTON_REJECT)
			continue;

		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mXcapt34Param[i].type;

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
			str.Format(_T(""));
			break;
		}

		if (mXcapt34Param[i].id == IDC_STATIC_CARRVAL){
			mCarrVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}
		if (mXcapt34Param[i].id == IDC_STATIC_CARRVAL2){
			mCarrVal2.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXcapt34Param[i].id == IDC_STATIC_CARRVAL ||
			mXcapt34Param[i].id == IDC_STATIC_CARRVAL2 ||
			mXcapt34Param[i].id == IDC_MFCMASKEDEDIT_AGC ||
			mXcapt34Param[i].id == IDC_MFCMASKEDEDIT_AGC2){
			if (str.IsEmpty())
				str.Format(_T(" "));
			((CColorStatic*)GetDlgItem(mXcapt34Param[i].id))->SetText(str);
		}
		else{
			GetDlgItem(mXcapt34Param[i].id)->SetWindowText(str);
		}
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! S帯受信

-# 捕捉状況ステータスの更新

@param  なし
@retval なし
*/
/*============================================================================*/
LRESULT CTabXcaptView34::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
