// TabSsendView34.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabSsendView34.h"
#include "afxdialogex.h"


// CTabSsendView34 ダイアログ

IMPLEMENT_DYNCREATE(CTabSsendView34, CDialogEx)

CTabSsendView34::CTabSsendView34(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabSsendView34::IDD, pParent)
{

}

CTabSsendView34::~CTabSsendView34()
{
}

void CTabSsendView34::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RNGVAL, mRngVal);
	DDX_Control(pDX, IDC_STATIC_RFVAL, mRfVal);
	DDX_Control(pDX, IDC_STATIC_RFDUMMY, mDummyVal);
}


BEGIN_MESSAGE_MAP(CTabSsendView34, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RF, &CTabSsendView34::OnBnClickedButtonRf)
	ON_BN_CLICKED(IDC_BUTTON_MOD, &CTabSsendView34::OnBnClickedButtonMod)
	ON_BN_CLICKED(IDC_BUTTON_INDEXCMD, &CTabSsendView34::OnBnClickedButtonIndexcmd)
	ON_BN_CLICKED(IDC_BUTTON_INDEXCMD2, &CTabSsendView34::OnBnClickedButtonIndexcmd2)
	ON_BN_CLICKED(IDC_BUTTON_UCCTRL, &CTabSsendView34::OnBnClickedButtonUcctrl)
	ON_BN_CLICKED(IDC_BUTTON_TX, &CTabSsendView34::OnBnClickedButtonTx)
	ON_BN_CLICKED(IDC_BUTTON_UC, &CTabSsendView34::OnBnClickedButtonUc)
	ON_BN_CLICKED(IDC_BUTTON_SWEEP, &CTabSsendView34::OnBnClickedButtonSweep)
	ON_BN_CLICKED(IDC_BUTTON_HOLDTONE, &CTabSsendView34::OnBnClickedButtonHoldtone)
	ON_BN_CLICKED(IDC_BUTTON_BITRATE, &CTabSsendView34::OnBnClickedButtonBitrate)
	ON_BN_CLICKED(IDC_BUTTON_MODMODE, &CTabSsendView34::OnBnClickedButtonMod2)
END_MESSAGE_MAP()


// CTabSsendView34 メッセージ ハンドラー


BOOL CTabSsendView34::OnInitDialog()
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
void CTabSsendView34::OnBnClickedButtonRf()
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
void CTabSsendView34::OnBnClickedButtonMod()
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
void CTabSsendView34::OnBnClickedButtonIndexcmd()
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
void CTabSsendView34::OnBnClickedButtonIndexcmd2()
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
void CTabSsendView34::OnBnClickedButtonUcctrl()
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
void CTabSsendView34::OnBnClickedButtonTx()
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
void CTabSsendView34::OnBnClickedButtonUc()
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
void CTabSsendView34::OnBnClickedButtonSweep()
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
void CTabSsendView34::OnBnClickedButtonHoldtone()
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
void CTabSsendView34::OnBnClickedButtonBitrate()
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
void CTabSsendView34::OnBnClickedButtonMod2()
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
void CTabSsendView34::ControlParamExecute(UINT pos)
{
	CString mControlString = mSSend34Param[pos].nameS;
	CString str;
	GetDlgItem(mSSend34Param[pos].id)->GetWindowText(str);

	if (mSSend34Param[pos].id == IDC_BUTTON_SWEEP)
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
	val.type = mSSend34Param[pos].type;
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
void CTabSsendView34::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mSSend34Param) / sizeof(mSSend34Param[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString mControlString = mSSend34Param[i].nameK;
		CString str;
		GetDlgItem(mSSend34Param[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mSSend34Param[i].type;

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
			if (mSSend34Param[i].id == IDC_BUTTON_INDEXCMD ||
				mSSend34Param[i].id == IDC_BUTTON_INDEXCMD2)
			{
				str = CSatelliteData::AddCanma(obs_adr->d_data, 2);
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

		if (mSSend34Param[i].id == IDC_STATIC_RFVAL)
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

		if (mSSend34Param[i].id == IDC_STATIC_RFDUMMY)
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

		if (mSSend34Param[i].id == IDC_STATIC_RNGVAL)
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

		if (mSSend34Param[i].id == IDC_BUTTON_SWEEP)
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

		if (mSSend34Param[i].id == IDC_STATIC_RFVAL ||
			mSSend34Param[i].id == IDC_STATIC_RFDUMMY ||
			mSSend34Param[i].id == IDC_STATIC_RNGVAL)
			((CColorStatic*)GetDlgItem(mSSend34Param[i].id))->SetText(str);
		else
			GetDlgItem(mSSend34Param[i].id)->SetWindowText(str);

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
void CTabSsendView34::InitStatus()
{
	int szControl = sizeof(mSSend34Param) / sizeof(mSSend34Param[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mSSend34Param[i].type;

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

		if (mSSend34Param[i].id == IDC_STATIC_RFVAL)
		{
			mRfVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mSSend34Param[i].id == IDC_STATIC_RFDUMMY)
		{
			mDummyVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mSSend34Param[i].id == IDC_STATIC_RNGVAL)
		{
			mRngVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mSSend34Param[i].id == IDC_STATIC_RFVAL ||
			mSSend34Param[i].id == IDC_STATIC_RFDUMMY ||
			mSSend34Param[i].id == IDC_STATIC_RNGVAL){
			if (str.IsEmpty())
				str.Format(_T(" "));
			((CColorStatic*)GetDlgItem(mSSend34Param[i].id))->SetText(str);
		}
		else
			GetDlgItem(mSSend34Param[i].id)->SetWindowText(str);

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
LRESULT CTabSsendView34::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
