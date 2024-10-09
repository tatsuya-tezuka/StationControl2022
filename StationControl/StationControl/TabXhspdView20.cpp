// TabSrecvView34.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabXhspdView20.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"


// CTabXhspdView20 ダイアログ

IMPLEMENT_DYNCREATE(CTabXhspdView20, CDialogEx)

CTabXhspdView20::CTabXhspdView20(CWnd* pParent /*=NULL*/)
: CDialogEx(CTabXhspdView20::IDD, pParent)
{

}

CTabXhspdView20::~CTabXhspdView20()
{
}

void CTabXhspdView20::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CARRVAL, mCarrVal);
	DDX_Control(pDX, IDC_STATIC_VITERBIVAL, mViterbiVal);
	DDX_Control(pDX, IDC_STATIC_FRAMEVAL, mFrameVal);
	DDX_Control(pDX, IDC_STATIC_EB, mEbVal1);
	DDX_Control(pDX, IDC_STATIC_EB2, mEbVal2);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_AGC, mAgcVal);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_AGC2, mAgcVal2);
}


BEGIN_MESSAGE_MAP(CTabXhspdView20, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_REJECT, &CTabXhspdView20::OnBnClickedButtonReject)
	ON_BN_CLICKED(IDC_BUTTON_BITRATE, &CTabXhspdView20::OnBnClickedButtonBitrate)
	ON_BN_CLICKED(IDC_BUTTON_DCCTRL, &CTabXhspdView20::OnBnClickedButtonDcctrl)
	ON_BN_CLICKED(IDC_BUTTON_RX, &CTabXhspdView20::OnBnClickedButtonRx)
	ON_BN_CLICKED(IDC_BUTTON_DC, &CTabXhspdView20::OnBnClickedButtonDc)
END_MESSAGE_MAP()


// CTabXhspdView20 メッセージ ハンドラー

BOOL CTabXhspdView20::OnInitDialog()
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
void CTabXhspdView20::OnBnClickedButtonReject()
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
void CTabXhspdView20::OnBnClickedButtonBitrate()
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
void CTabXhspdView20::OnBnClickedButtonDcctrl()
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
void CTabXhspdView20::OnBnClickedButtonRx()
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
void CTabXhspdView20::OnBnClickedButtonDc()
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
void CTabXhspdView20::ControlParamExecute(UINT pos)
{
	CString mControlString = mXhspd20Param[pos].nameS;
	CString str;
	GetDlgItem(mXhspd20Param[pos].id)->GetWindowText(str);
	stControlValue val;
	val.ret = 0;
	val.type = mXhspd20Param[pos].type;
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
void CTabXhspdView20::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	//	if (sd.stn_tbl_access() == 1)
	//	{
	//		return;
	//	}

	int szControl = sizeof(mXhspd20Param) / sizeof(mXhspd20Param[0]);

	for (int i = 0; i < szControl; i++)
	{
		if (mXhspd20Param[i].id == IDC_BUTTON_REJECT)
			continue;

		CString mControlString = mXhspd20Param[i].nameK;
		CString str;
		GetDlgItem(mXhspd20Param[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mXhspd20Param[i].type;

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

		if (mXhspd20Param[i].id == IDC_STATIC_CARRVAL)
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

		if (mXhspd20Param[i].id == IDC_STATIC_VITERBIVAL)
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

		if (mXhspd20Param[i].id == IDC_STATIC_FRAMEVAL)
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

		if (mXhspd20Param[i].id == IDC_STATIC_CARRVAL ||
			mXhspd20Param[i].id == IDC_STATIC_VITERBIVAL ||
			mXhspd20Param[i].id == IDC_STATIC_FRAMEVAL ||
			mXhspd20Param[i].id == IDC_MFCMASKEDEDIT_AGC ||
			mXhspd20Param[i].id == IDC_STATIC_EB ||
			mXhspd20Param[i].id == IDC_STATIC_EB2){
			((CColorStatic*)GetDlgItem(mXhspd20Param[i].id))->SetText(str);
		}
		else{
			GetDlgItem(mXhspd20Param[i].id)->SetWindowText(str);
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
void CTabXhspdView20::InitStatus()
{
	int szControl = sizeof(mXhspd20Param) / sizeof(mXhspd20Param[0]);

	for (int i = 0; i < szControl; i++)
	{
		if (mXhspd20Param[i].id == IDC_BUTTON_REJECT)
			continue;

		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mXhspd20Param[i].type;

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

		if (mXhspd20Param[i].id == IDC_STATIC_CARRVAL)
		{
			mCarrVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXhspd20Param[i].id == IDC_STATIC_VITERBIVAL)
		{
			mViterbiVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXhspd20Param[i].id == IDC_STATIC_FRAMEVAL)
		{
			mFrameVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXhspd20Param[i].id == IDC_STATIC_CARRVAL ||
			mXhspd20Param[i].id == IDC_STATIC_VITERBIVAL ||
			mXhspd20Param[i].id == IDC_STATIC_FRAMEVAL ||
			mXhspd20Param[i].id == IDC_MFCMASKEDEDIT_AGC ||
			mXhspd20Param[i].id == IDC_STATIC_EB ||
			mXhspd20Param[i].id == IDC_STATIC_EB2){
			if (str.IsEmpty())
				str.Format(_T(" "));
			((CColorStatic*)GetDlgItem(mXhspd20Param[i].id))->SetText(str);
		}
		else{
			GetDlgItem(mXhspd20Param[i].id)->SetWindowText(str);
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
LRESULT CTabXhspdView20::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
