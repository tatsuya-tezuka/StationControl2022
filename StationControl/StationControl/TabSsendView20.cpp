// TabSsendView34.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabSsendView20.h"
#include "afxdialogex.h"


// CTabSsendView20 ダイアログ

IMPLEMENT_DYNCREATE(CTabSsendView20, CDialogEx)

CTabSsendView20::CTabSsendView20(CWnd* pParent /*=NULL*/)
: CDialogEx(CTabSsendView20::IDD, pParent)
{

}

CTabSsendView20::~CTabSsendView20()
{
}

void CTabSsendView20::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RNGVAL, mRngVal);
	DDX_Control(pDX, IDC_STATIC_RFVAL, mRfVal);
	DDX_Control(pDX, IDC_STATIC_RFDUMMY, mDummyVal);
}


BEGIN_MESSAGE_MAP(CTabSsendView20, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RF, &CTabSsendView20::OnBnClickedButtonRf)
	ON_BN_CLICKED(IDC_BUTTON_MOD, &CTabSsendView20::OnBnClickedButtonMod)
	ON_BN_CLICKED(IDC_BUTTON_UCCTRL, &CTabSsendView20::OnBnClickedButtonUcctrl)
	ON_BN_CLICKED(IDC_BUTTON_TX, &CTabSsendView20::OnBnClickedButtonTx)
	ON_BN_CLICKED(IDC_BUTTON_UC, &CTabSsendView20::OnBnClickedButtonUc)
	ON_BN_CLICKED(IDC_BUTTON_SWEEP, &CTabSsendView20::OnBnClickedButtonSweep)
	ON_BN_CLICKED(IDC_BUTTON_HOLDTONE, &CTabSsendView20::OnBnClickedButtonHoldtone)
	ON_BN_CLICKED(IDC_BUTTON_BITRATE, &CTabSsendView20::OnBnClickedButtonBitrate)
END_MESSAGE_MAP()


// CTabSsendView20 メッセージ ハンドラー


BOOL CTabSsendView20::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	mRngVal.SetGradient(false);
	mRngVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mRngVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
	mRngVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mRfVal.SetGradient(false);
	mRfVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mRfVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
	mRfVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mDummyVal.SetGradient(false);
	mDummyVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mDummyVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
	mDummyVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	theApp.SetMonitorMode(this);

	InitStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! S帯送信

-# RFボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabSsendView20::OnBnClickedButtonRf()
{
	ControlParamExecute(0);
}

/*============================================================================*/
/*! S帯送信

-# MODボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabSsendView20::OnBnClickedButtonMod()
{
	ControlParamExecute(1);
}


/*============================================================================*/
/*! S帯送信

-# U/C CTRLボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabSsendView20::OnBnClickedButtonUcctrl()
{
	ControlParamExecute(5);
}


/*============================================================================*/
/*! S帯送信

-# TXボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabSsendView20::OnBnClickedButtonTx()
{
	ControlParamExecute(6);
}


/*============================================================================*/
/*! S帯送信

-# U/Cボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabSsendView20::OnBnClickedButtonUc()
{
	ControlParamExecute(7);
}


/*============================================================================*/
/*! S帯送信

-# SWEEPボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabSsendView20::OnBnClickedButtonSweep()
{
	ControlParamExecute(8);
}


/*============================================================================*/
/*! S帯送信

-# HOLD TONEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabSsendView20::OnBnClickedButtonHoldtone()
{
	ControlParamExecute(9);
}


/*============================================================================*/
/*! S帯送信

-# BIT RATEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabSsendView20::OnBnClickedButtonBitrate()
{
	ControlParamExecute(10);
}


/*============================================================================*/
/*! S帯送信

-# 制御パラメタの実行

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabSsendView20::ControlParamExecute(UINT pos)
{
	CString mControlString = mSSend20Param[pos].nameS;
	CString str;
	GetDlgItem(mSSend20Param[pos].id)->GetWindowText(str);

	if (mSSend20Param[pos].id == IDC_BUTTON_SWEEP)
	{
		if (str.CompareNoCase(_T("INC+DEC")) == 0)
		{
			str = _T("INCDEC");
		}
		else if (str.CompareNoCase(_T("DEC+INC")) == 0)
		{
			str = _T("DECINC");
		}
	}

	stControlValue val;
	val.ret = 0;
	val.type = mSSend20Param[pos].type;
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
	}

	if (mSSend20Param[pos].id == IDC_BUTTON_RF){
		mControlString = _T("CTRL.S_TX_RF_PWR");
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
/*! S帯送信

-# 捕捉状況ステータスの更新

@param
@retval

*/
/*============================================================================*/
void CTabSsendView20::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mSSend20Param) / sizeof(mSSend20Param[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString mControlString = mSSend20Param[i].nameK;
		CString str;
		GetDlgItem(mSSend20Param[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mSSend20Param[i].type;

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

		if (mSSend20Param[i].id == IDC_STATIC_RFVAL)
		{
			if (str.CompareNoCase(_T("ON")) == 0)
			{
				mRfVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				mRfVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
			}
		}

		if (mSSend20Param[i].id == IDC_STATIC_RFDUMMY)
		{
			if (str.CompareNoCase(_T("ANT")) == 0)
			{
				mDummyVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				mDummyVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
			}
		}

		if (mSSend20Param[i].id == IDC_STATIC_RNGVAL)
		{
			if (str.CompareNoCase(_T("START")) == 0)
			{
				mRngVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				mRngVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
			}
		}

		if (mSSend20Param[i].id == IDC_BUTTON_SWEEP)
		{
			if (str.CompareNoCase(_T("INCDEC")) == 0)
			{
				str = _T("INC+DEC");
			}
			else if (str.CompareNoCase(_T("DECINC")) == 0)
			{
				str = _T("DEC+INC");
			}
		}

		if (mSSend20Param[i].id == IDC_STATIC_RFVAL ||
			mSSend20Param[i].id == IDC_STATIC_RFDUMMY ||
			mSSend20Param[i].id == IDC_STATIC_RNGVAL)
			((CColorStatic*)GetDlgItem(mSSend20Param[i].id))->SetText(str);
		else
			GetDlgItem(mSSend20Param[i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! S帯送信

-# 捕捉状況ステータスの初期化

@param
@retval

*/
/*============================================================================*/
void CTabSsendView20::InitStatus()
{
	int szControl = sizeof(mSSend20Param) / sizeof(mSSend20Param[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mSSend20Param[i].type;

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

		if (mSSend20Param[i].id == IDC_STATIC_RFVAL)
		{
			mRfVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mSSend20Param[i].id == IDC_STATIC_RFDUMMY)
		{
			mDummyVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mSSend20Param[i].id == IDC_STATIC_RNGVAL)
		{
			mRngVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mSSend20Param[i].id == IDC_STATIC_RFVAL ||
			mSSend20Param[i].id == IDC_STATIC_RFDUMMY ||
			mSSend20Param[i].id == IDC_STATIC_RNGVAL){
			if (str.IsEmpty())
				str.Format(_T(" "));
			((CColorStatic*)GetDlgItem(mSSend20Param[i].id))->SetText(str);
		}
		else
			GetDlgItem(mSSend20Param[i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! S帯送信

-# 捕捉状況ステータスの更新

@param  なし
@retval なし
*/
/*============================================================================*/
LRESULT CTabSsendView20::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
