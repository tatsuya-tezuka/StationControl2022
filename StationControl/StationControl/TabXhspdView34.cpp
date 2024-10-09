// TabSrecvView34.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabXhspdView34.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"


// CTabXhspdView34 ダイアログ

IMPLEMENT_DYNCREATE(CTabXhspdView34, CDialogEx)

CTabXhspdView34::CTabXhspdView34(CWnd* pParent /*=NULL*/)
: CDialogEx(CTabXhspdView34::IDD, pParent)
{

}

CTabXhspdView34::~CTabXhspdView34()
{
}

void CTabXhspdView34::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CARRVAL, mCarrVal);
	DDX_Control(pDX, IDC_STATIC_VITERBIVAL, mViterbiVal);
	DDX_Control(pDX, IDC_STATIC_FRAMEVAL, mFrameVal);
	DDX_Control(pDX, IDC_STATIC_EB, mEbVal1);
	DDX_Control(pDX, IDC_STATIC_EB2, mEbVal2);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_AGC, mAgcVal);
}


BEGIN_MESSAGE_MAP(CTabXhspdView34, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_REJECT, &CTabXhspdView34::OnBnClickedButtonReject)
	ON_BN_CLICKED(IDC_BUTTON_BITRATE, &CTabXhspdView34::OnBnClickedButtonBitrate)
	ON_BN_CLICKED(IDC_BUTTON_DCCTRL, &CTabXhspdView34::OnBnClickedButtonDcctrl)
	ON_BN_CLICKED(IDC_BUTTON_RX, &CTabXhspdView34::OnBnClickedButtonRx)
	ON_BN_CLICKED(IDC_BUTTON_DC, &CTabXhspdView34::OnBnClickedButtonDc)
END_MESSAGE_MAP()


// CTabXhspdView34 メッセージ ハンドラー

BOOL CTabXhspdView34::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	mCarrVal.SetGradient(false);
	mCarrVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mCarrVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
	mCarrVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mViterbiVal.SetGradient(false);
	mViterbiVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mViterbiVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
	mViterbiVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mFrameVal.SetGradient(false);
	mFrameVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mFrameVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
	mFrameVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mEbVal1.SetGradient(false);
	mEbVal1.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mEbVal1.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mEbVal1.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mEbVal2.SetGradient(false);
	mEbVal2.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mEbVal2.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mEbVal2.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mAgcVal.SetGradient(false);
	mAgcVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mAgcVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mAgcVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

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
void CTabXhspdView34::OnBnClickedButtonReject()
{
	ControlParamExecute(0);
}


/*============================================================================*/
/*! S帯受信

-# BITRATEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXhspdView34::OnBnClickedButtonBitrate()
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
void CTabXhspdView34::OnBnClickedButtonDcctrl()
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
void CTabXhspdView34::OnBnClickedButtonRx()
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
void CTabXhspdView34::OnBnClickedButtonDc()
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
void CTabXhspdView34::ControlParamExecute(UINT pos)
{
	CString mControlString = mXhspd34Param[pos].nameS;
	CString str;
	GetDlgItem(mXhspd34Param[pos].id)->GetWindowText(str);
	stControlValue val;
	val.ret = 0;
	val.type = mXhspd34Param[pos].type;
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

	theApp.SendControlChkDlg(mControlString, str, _T(""), _T(""));
}


/*============================================================================*/
/*! S帯受信

-# 捕捉状況ステータスの更新

@param
@retval

*/
/*============================================================================*/
void CTabXhspdView34::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	//	if (sd.stn_tbl_access() == 1)
	//	{
	//		return;
	//	}

	int szControl = sizeof(mXhspd34Param) / sizeof(mXhspd34Param[0]);

	for (int i = 0; i < szControl; i++)
	{
		if (mXhspd34Param[i].id == IDC_BUTTON_REJECT)
			continue;

		CString mControlString = mXhspd34Param[i].nameK;
		CString str;
		GetDlgItem(mXhspd34Param[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mXhspd34Param[i].type;

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

		if (mXhspd34Param[i].id == IDC_STATIC_CARRVAL){
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
				mCarrVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			else
				mCarrVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
		}
		if (mXhspd34Param[i].id == IDC_STATIC_VITERBIVAL){
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
				mViterbiVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			else
				mViterbiVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
		}
		if (mXhspd34Param[i].id == IDC_STATIC_FRAMEVAL){
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
				mFrameVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			else
				mFrameVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
		}

		if (mXhspd34Param[i].id == IDC_STATIC_CARRVAL ||
			mXhspd34Param[i].id == IDC_STATIC_VITERBIVAL ||
			mXhspd34Param[i].id == IDC_STATIC_FRAMEVAL ||
			mXhspd34Param[i].id == IDC_MFCMASKEDEDIT_AGC ||
			mXhspd34Param[i].id == IDC_STATIC_EB ||
			mXhspd34Param[i].id == IDC_STATIC_EB2){
			((CColorStatic*)GetDlgItem(mXhspd34Param[i].id))->SetText(str);
		}
		else{
			GetDlgItem(mXhspd34Param[i].id)->SetWindowText(str);
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
void CTabXhspdView34::InitStatus()
{
	int szControl = sizeof(mXhspd34Param) / sizeof(mXhspd34Param[0]);

	for (int i = 0; i < szControl; i++)
	{
		if (mXhspd34Param[i].id == IDC_BUTTON_REJECT)
			continue;

		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mXhspd34Param[i].type;

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

		if (mXhspd34Param[i].id == IDC_STATIC_CARRVAL){
			mCarrVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}
		if (mXhspd34Param[i].id == IDC_STATIC_VITERBIVAL){
			mViterbiVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}
		if (mXhspd34Param[i].id == IDC_STATIC_FRAMEVAL){
			mFrameVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXhspd34Param[i].id == IDC_STATIC_CARRVAL ||
			mXhspd34Param[i].id == IDC_STATIC_VITERBIVAL ||
			mXhspd34Param[i].id == IDC_STATIC_FRAMEVAL ||
			mXhspd34Param[i].id == IDC_MFCMASKEDEDIT_AGC ||
			mXhspd34Param[i].id == IDC_STATIC_EB ||
			mXhspd34Param[i].id == IDC_STATIC_EB2){
			if (str.IsEmpty())
				str.Format(_T(" "));
			((CColorStatic*)GetDlgItem(mXhspd34Param[i].id))->SetText(str);
		}
		else{
			GetDlgItem(mXhspd34Param[i].id)->SetWindowText(str);
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
LRESULT CTabXhspdView34::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

