// TabXsendView34.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabXsendView34.h"
#include "afxdialogex.h"


// CTabXsendView34 ダイアログ

IMPLEMENT_DYNCREATE(CTabXsendView34, CDialogEx)

CTabXsendView34::CTabXsendView34(CWnd* pParent /*=NULL*/)
: CDialogEx(CTabXsendView34::IDD, pParent)
{

}

CTabXsendView34::~CTabXsendView34()
{
}

void CTabXsendView34::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RNGVAL, mRngVal);
	DDX_Control(pDX, IDC_STATIC_RFVAL, mRfVal);
	DDX_Control(pDX, IDC_STATIC_RFDUMMY, mDummyVal);
}


BEGIN_MESSAGE_MAP(CTabXsendView34, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RF, &CTabXsendView34::OnBnClickedButtonRf)
	ON_BN_CLICKED(IDC_BUTTON_MOD, &CTabXsendView34::OnBnClickedButtonMod)
	ON_BN_CLICKED(IDC_BUTTON_INDEXCMD, &CTabXsendView34::OnBnClickedButtonIndexcmd)
	ON_BN_CLICKED(IDC_BUTTON_INDEXCMD2, &CTabXsendView34::OnBnClickedButtonIndexcmd2)
	ON_BN_CLICKED(IDC_BUTTON_UCCTRL, &CTabXsendView34::OnBnClickedButtonUcctrl)
	ON_BN_CLICKED(IDC_BUTTON_TX, &CTabXsendView34::OnBnClickedButtonTx)
	ON_BN_CLICKED(IDC_BUTTON_UC, &CTabXsendView34::OnBnClickedButtonUc)
	ON_BN_CLICKED(IDC_BUTTON_SWEEP, &CTabXsendView34::OnBnClickedButtonSweep)
	ON_BN_CLICKED(IDC_BUTTON_HOLDTONE, &CTabXsendView34::OnBnClickedButtonHoldtone)
	ON_BN_CLICKED(IDC_BUTTON_BITRATE, &CTabXsendView34::OnBnClickedButtonBitrate)
	ON_BN_CLICKED(IDC_BUTTON_MODMODE, &CTabXsendView34::OnBnClickedButtonMod2)
END_MESSAGE_MAP()


// CTabXsendView34 メッセージ ハンドラー


BOOL CTabXsendView34::OnInitDialog()
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
void CTabXsendView34::OnBnClickedButtonRf()
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
void CTabXsendView34::OnBnClickedButtonMod()
{
	ControlParamExecute(1);
}

/*============================================================================*/
/*! S帯送信

-# INDEX CMDボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView34::OnBnClickedButtonIndexcmd()
{
	ControlParamExecute(2);
}


/*============================================================================*/
/*! S帯送信

-# INDEX RNGボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView34::OnBnClickedButtonIndexcmd2()
{
	ControlParamExecute(3);
}


/*============================================================================*/
/*! S帯送信

-# U/C CTRLボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView34::OnBnClickedButtonUcctrl()
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
void CTabXsendView34::OnBnClickedButtonTx()
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
void CTabXsendView34::OnBnClickedButtonUc()
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
void CTabXsendView34::OnBnClickedButtonSweep()
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
void CTabXsendView34::OnBnClickedButtonHoldtone()
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
void CTabXsendView34::OnBnClickedButtonBitrate()
{
	ControlParamExecute(10);
}


/*============================================================================*/
/*! S帯送信

-# MOD MODEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView34::OnBnClickedButtonMod2()
{
	ControlParamExecute(4);
}

/*============================================================================*/
/*! S帯送信

-# 制御パラメタの実行

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView34::ControlParamExecute(UINT pos)
{
	CString mControlString = mXSend34Param[pos].nameS;
	CString str;
	GetDlgItem(mXSend34Param[pos].id)->GetWindowText(str);
	stControlValue val;
	val.ret = 0;
	val.type = mXSend34Param[pos].type;
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
void CTabXsendView34::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mXSend34Param) / sizeof(mXSend34Param[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString mControlString = mXSend34Param[i].nameK;
		CString str;
		GetDlgItem(mXSend34Param[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mXSend34Param[i].type;

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
			if (mXSend34Param[i].id == IDC_BUTTON_INDEXCMD ||
				mXSend34Param[i].id == IDC_BUTTON_INDEXCMD2)
			{ 
				str = CSatelliteData::AddCanma(obs_adr->d_data, 2);
			}
			else if (mXSend34Param[i].id == IDC_BUTTON_BITRATE)
			{
				str = CSatelliteData::AddCanma(obs_adr->d_data, 4);
			}
			else
			{
				str = CSatelliteData::AddCanma(obs_adr->d_data, 3);
			}
			break;
		case	eControlTime:
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		}

		if (mXSend34Param[i].id == IDC_STATIC_RFVAL)
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

		if (mXSend34Param[i].id == IDC_STATIC_RFDUMMY)
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

		if (mXSend34Param[i].id == IDC_STATIC_RNGVAL)
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

		if (mXSend34Param[i].id == IDC_STATIC_RFVAL ||
			mXSend34Param[i].id == IDC_STATIC_RFDUMMY ||
			mXSend34Param[i].id == IDC_STATIC_RNGVAL)
			((CColorStatic*)GetDlgItem(mXSend34Param[i].id))->SetText(str);
		else
			GetDlgItem(mXSend34Param[i].id)->SetWindowText(str);

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
void CTabXsendView34::InitStatus()
{
	int szControl = sizeof(mXSend34Param) / sizeof(mXSend34Param[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mXSend34Param[i].type;

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

		if (mXSend34Param[i].id == IDC_STATIC_RFVAL)
		{
			mRfVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXSend34Param[i].id == IDC_STATIC_RFDUMMY)
		{
			mDummyVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXSend34Param[i].id == IDC_STATIC_RNGVAL)
		{
			mRngVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXSend34Param[i].id == IDC_STATIC_RFVAL ||
			mXSend34Param[i].id == IDC_STATIC_RFDUMMY ||
			mXSend34Param[i].id == IDC_STATIC_RNGVAL){
			if (str.IsEmpty())
				str.Format(_T(" "));
			((CColorStatic*)GetDlgItem(mXSend34Param[i].id))->SetText(str);
		}
		else
			GetDlgItem(mXSend34Param[i].id)->SetWindowText(str);

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
LRESULT CTabXsendView34::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
