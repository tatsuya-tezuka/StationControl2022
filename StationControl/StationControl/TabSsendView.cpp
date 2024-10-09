/*============================================================================*/
/*! TabSsendView.cpp

-# 捕捉状況ステータス画面（S帯送信）
*/
/*============================================================================*/
// TabSsendView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabSsendView.h"
#include "afxdialogex.h"


// CTabSsendView ダイアログ

IMPLEMENT_DYNCREATE(CTabSsendView, CDialogEx)

CTabSsendView::CTabSsendView(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabSsendView::IDD, pParent)
{

}

CTabSsendView::~CTabSsendView()
{
}

void CTabSsendView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RNGVAL, mRngVal);
	DDX_Control(pDX, IDC_STATIC_RFVAL, mRfVal);
	DDX_Control(pDX, IDC_STATIC_RFDUMMY, mDummyVal);
}


BEGIN_MESSAGE_MAP(CTabSsendView, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RF, &CTabSsendView::OnBnClickedButtonRf)
	ON_BN_CLICKED(IDC_BUTTON_CMDSC, &CTabSsendView::OnBnClickedButtonCmdsc)
	ON_BN_CLICKED(IDC_BUTTON_MOD, &CTabSsendView::OnBnClickedButtonMod)
	ON_BN_CLICKED(IDC_BUTTON_INDEXCMD, &CTabSsendView::OnBnClickedButtonIndexcmd)
	ON_BN_CLICKED(IDC_BUTTON_INDEXCMD2, &CTabSsendView::OnBnClickedButtonIndexcmd2)
	ON_BN_CLICKED(IDC_BUTTON_UCCTRL, &CTabSsendView::OnBnClickedButtonUcctrl)
	ON_BN_CLICKED(IDC_BUTTON_TX, &CTabSsendView::OnBnClickedButtonTx)
	ON_BN_CLICKED(IDC_BUTTON_UC, &CTabSsendView::OnBnClickedButtonUc)
	ON_BN_CLICKED(IDC_BUTTON_SWEEP, &CTabSsendView::OnBnClickedButtonSweep)
	ON_BN_CLICKED(IDC_BUTTON_HOLDTONE, &CTabSsendView::OnBnClickedButtonHoldtone)
	ON_BN_CLICKED(IDC_BUTTON_BITRATE, &CTabSsendView::OnBnClickedButtonBitrate)
END_MESSAGE_MAP()


// CTabSsendView メッセージ ハンドラー


BOOL CTabSsendView::OnInitDialog()
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
void CTabSsendView::OnBnClickedButtonRf()
{
	ControlParamExecute(0);
}


/*============================================================================*/
/*! S帯送信

-# CMD SCボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabSsendView::OnBnClickedButtonCmdsc()
{
	ControlParamExecute(1);
}

/*============================================================================*/
/*! S帯送信

-# MODボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabSsendView::OnBnClickedButtonMod()
{
	ControlParamExecute(2);
}

/*============================================================================*/
/*! S帯送信

-# INDEX CMDボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabSsendView::OnBnClickedButtonIndexcmd()
{
	ControlParamExecute(3);
}


/*============================================================================*/
/*! S帯送信

-# INDEX RNGボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabSsendView::OnBnClickedButtonIndexcmd2()
{
	ControlParamExecute(4);
}


/*============================================================================*/
/*! S帯送信

-# U/C CTRLボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabSsendView::OnBnClickedButtonUcctrl()
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
void CTabSsendView::OnBnClickedButtonTx()
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
void CTabSsendView::OnBnClickedButtonUc()
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
void CTabSsendView::OnBnClickedButtonSweep()
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
void CTabSsendView::OnBnClickedButtonHoldtone()
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
void CTabSsendView::OnBnClickedButtonBitrate()
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
void CTabSsendView::ControlParamExecute(UINT pos)
{
	CString mControlString = mSSendParam[pos].nameS;
	CString str;
	GetDlgItem(mSSendParam[pos].id)->GetWindowText(str);
	stControlValue val;
	val.ret = 0;
	val.type = mSSendParam[pos].type;
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
//	if (mSSendParam[pos].id == IDC_BUTTON_RF){
//		mControlString = _T("S-TX.RF_PWR_SEL_CONT");
//	}
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
void CTabSsendView::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mSSendParam) / sizeof(mSSendParam[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString mControlString = mSSendParam[i].nameK;
		CString str;
		GetDlgItem(mSSendParam[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mSSendParam[i].type;

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
			if (mSSendParam[i].id == IDC_BUTTON_INDEXCMD ||
				mSSendParam[i].id == IDC_BUTTON_INDEXCMD2)
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

		if (mSSendParam[i].id == IDC_STATIC_RFVAL)
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

		if (mSSendParam[i].id == IDC_STATIC_RFDUMMY)
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

		if (mSSendParam[i].id == IDC_STATIC_RNGVAL)
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

		if (mSSendParam[i].id == IDC_STATIC_RFVAL ||
			mSSendParam[i].id == IDC_STATIC_RFDUMMY ||
			mSSendParam[i].id == IDC_STATIC_RNGVAL)
			((CColorStatic*)GetDlgItem(mSSendParam[i].id))->SetText(str);
		else
			GetDlgItem(mSSendParam[i].id)->SetWindowText(str);

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
void CTabSsendView::InitStatus()
{
	int szControl = sizeof(mSSendParam) / sizeof(mSSendParam[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mSSendParam[i].type;

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

		if (mSSendParam[i].id == IDC_STATIC_RFVAL)
		{
			mRfVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mSSendParam[i].id == IDC_STATIC_RFDUMMY)
		{
			mDummyVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mSSendParam[i].id == IDC_STATIC_RNGVAL)
		{
			mRngVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mSSendParam[i].id == IDC_STATIC_RFVAL ||
			mSSendParam[i].id == IDC_STATIC_RFDUMMY ||
			mSSendParam[i].id == IDC_STATIC_RNGVAL){
			if (str.IsEmpty())
				str.Format(_T(" "));
			((CColorStatic*)GetDlgItem(mSSendParam[i].id))->SetText(str);
		}
		else
			GetDlgItem(mSSendParam[i].id)->SetWindowText(str);

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
LRESULT CTabSsendView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
