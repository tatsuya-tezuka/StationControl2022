/*============================================================================*/
/*! TabXsendView54.cpp

-# 捕捉状況ステータス画面（X帯送信）
*/
/*============================================================================*/
// TabXsendView54.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabXsendView54.h"
#include "afxdialogex.h"


// CTabXsendView54 ダイアログ

IMPLEMENT_DYNCREATE(CTabXsendView54, CDialogEx)

CTabXsendView54::CTabXsendView54(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabXsendView54::IDD, pParent)
{

}

CTabXsendView54::~CTabXsendView54()
{
}

void CTabXsendView54::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RFVAL, mRfVal);
	DDX_Control(pDX, IDC_STATIC_RFDUMMY, mDummyVal);
	DDX_Control(pDX, IDC_STATIC_RNGVAL, mRngVal);
}


BEGIN_MESSAGE_MAP(CTabXsendView54, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RF, &CTabXsendView54::OnBnClickedButtonRf)
	ON_BN_CLICKED(IDC_BUTTON_CMDSC, &CTabXsendView54::OnBnClickedButtonCmdsc)
	ON_BN_CLICKED(IDC_BUTTON_RNGMOD, &CTabXsendView54::OnBnClickedButtonRngmod)
	ON_BN_CLICKED(IDC_BUTTON_CMDMOD, &CTabXsendView54::OnBnClickedButtonCmdmod)
//	ON_BN_CLICKED(IDC_BUTTON_RNGCTRL, &CTabXsendView54::OnBnClickedButtonRngctrl)
	ON_BN_CLICKED(IDC_BUTTON_UCCTRL, &CTabXsendView54::OnBnClickedButtonUcctrl)
	ON_BN_CLICKED(IDC_BUTTON_TX, &CTabXsendView54::OnBnClickedButtonTx)
	ON_BN_CLICKED(IDC_BUTTON_SWEEP, &CTabXsendView54::OnBnClickedButtonSweep)
	ON_BN_CLICKED(IDC_BUTTON_HOLDTONE, &CTabXsendView54::OnBnClickedButtonHoldtone)
	ON_BN_CLICKED(IDC_BUTTON_BITRATE, &CTabXsendView54::OnBnClickedButtonBitrate)
	ON_BN_CLICKED(IDC_BUTTON_SSPACTRL, &CTabXsendView54::OnBnClickedButtonSspactrl)
END_MESSAGE_MAP()


// CTabXsendView54 メッセージ ハンドラー


BOOL CTabXsendView54::OnInitDialog()
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
/*! X帯送信

-# RFボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView54::OnBnClickedButtonRf()
{
	ControlParamExecute(0);
}


/*============================================================================*/
/*! X帯送信

-# CMD SCボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView54::OnBnClickedButtonCmdsc()
{
	ControlParamExecute(1);
}


/*============================================================================*/
/*! X帯送信

-# RNG MODボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView54::OnBnClickedButtonRngmod()
{
	ControlParamExecute(2);
}


/*============================================================================*/
/*! X帯送信

-# CMD MODボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView54::OnBnClickedButtonCmdmod()
{
	ControlParamExecute(3);
}


///*============================================================================*/
///*! X帯送信
//
//-# RNG CTRLボタン押下処理
//
//@param  なし
//@retval なし
//*/
///*============================================================================*/
//void CTabXsendView54::OnBnClickedButtonRngctrl()
//{
//	ControlParamExecute(4);
//}


/*============================================================================*/
/*! X帯送信

-# U/C CTRLボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView54::OnBnClickedButtonUcctrl()
{
	ControlParamExecute(5);
}


/*============================================================================*/
/*! X帯送信

-# TXボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView54::OnBnClickedButtonTx()
{
	ControlParamExecute(6);
}


/*============================================================================*/
/*! X帯送信

-# SWEEPボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView54::OnBnClickedButtonSweep()
{
	ControlParamExecute(7);
}


/*============================================================================*/
/*! X帯送信

-# HOLD TONEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView54::OnBnClickedButtonHoldtone()
{
	ControlParamExecute(8);
}


/*============================================================================*/
/*! X帯送信

-# BIT RATEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView54::OnBnClickedButtonBitrate()
{
	ControlParamExecute(9);
}

/*============================================================================*/
/*! X帯送信

-# SSPA CTRLボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView54::OnBnClickedButtonSspactrl()
{
	ControlParamExecute(12);
}

/*============================================================================*/
/*! X帯送信

-# 制御パラメタの実行

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView54::ControlParamExecute(UINT pos)
{
	CString mControlString = mXsendParam54[pos].nameS;
	CString str;
	GetDlgItem(mXsendParam54[pos].id)->GetWindowText(str);
	stControlValue val;
	val.ret = 0;
	val.type = mXsendParam54[pos].type;
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

	CString strCtrl2 = _T("");
	if (pos == 5 || pos == 12)			
	{
		// U/C CTRLまたはSSPA CTRLボタンの場合
		theApp.GetSatelliteData().SendControlSelect(mXsendParam54[12].nameS, &val, mXsendParam54[5].nameS);
	}
	else
	{
		// それ以外のボタンの場合
		theApp.GetSatelliteData().SendControlSelect(mControlString, &val);
	}
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

	if (pos == 5 || pos == 12)
	{
		// U/C CTRLまたはSSPA CTRLボタンの場合
		// U/C CTRLとSSPA CTRL制御を連続送信
		theApp.SendControlSameNumChkDlg(mXsendParam54[12].nameS, mXsendParam54[5].nameS, str, _T(""), _T(""));
	}
	else
	{
		// それ以外の制御の場合
		theApp.SendControlChkDlg(mControlString, str, _T(""), _T(""));
	}
}


/*============================================================================*/
/*! X帯送信

-# 捕捉状況ステータスの更新

@param
@retval

*/
/*============================================================================*/
void CTabXsendView54::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mXsendParam54) / sizeof(mXsendParam54[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString mControlString = mXsendParam54[i].nameK;
		CString str;
		GetDlgItem(mXsendParam54[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mXsendParam54[i].type;

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
			if (mXsendParam54[i].id == IDC_STATIC_RNGVAL)
			{
				str.Replace(_T("&"), _T("&&"));
			}
			break;
		case	eControlNumeric:
			str = CSatelliteData::AddCanma((double)obs_adr->l_data, 3);
			break;
		case	eControlDouble:
			if (mXsendParam54[i].id == IDC_BUTTON_CMDSC)
			{
				str = CSatelliteData::AddCanma(obs_adr->d_data, 4);
			}
			else if (mXsendParam54[i].id == IDC_BUTTON_BITRATE)
			{
				str = CSatelliteData::AddCanma(obs_adr->d_data, 5);
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

		if (mXsendParam54[i].id == IDC_STATIC_RFVAL)
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

		if (mXsendParam54[i].id == IDC_STATIC_RFDUMMY)
		{
			if (str.CompareNoCase(_T("RHCP")) == 0 || str.CompareNoCase(_T("LHCP")) == 0)
			{
				mDummyVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				mDummyVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
			}
		}

		if (mXsendParam54[i].id == IDC_STATIC_RNGVAL)
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

		if (mXsendParam54[i].id == IDC_STATIC_RFVAL ||
			mXsendParam54[i].id == IDC_STATIC_RFDUMMY ||
			mXsendParam54[i].id == IDC_STATIC_RNGVAL)
			((CColorStatic*)GetDlgItem(mXsendParam54[i].id))->SetText(str);
		else
			GetDlgItem(mXsendParam54[i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! X帯送信

-# 捕捉状況ステータスの初期化

@param
@retval

*/
/*============================================================================*/
void CTabXsendView54::InitStatus()
{
	int szControl = sizeof(mXsendParam54) / sizeof(mXsendParam54[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mXsendParam54[i].type;

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

		if (mXsendParam54[i].id == IDC_STATIC_RFVAL)
		{
			mRfVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXsendParam54[i].id == IDC_STATIC_RFDUMMY)
		{
			mDummyVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXsendParam54[i].id == IDC_STATIC_RNGVAL)
		{
			mRngVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXsendParam54[i].id == IDC_STATIC_RFVAL ||
			mXsendParam54[i].id == IDC_STATIC_RFDUMMY ||
			mXsendParam54[i].id == IDC_STATIC_RNGVAL){
			if (str.IsEmpty())
				str.Format(_T(" "));
			((CColorStatic*)GetDlgItem(mXsendParam54[i].id))->SetText(str);
		}
		else
			GetDlgItem(mXsendParam54[i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! X帯送信

-# 捕捉状況ステータスの更新

@param  なし
@retval なし
*/
/*============================================================================*/
LRESULT CTabXsendView54::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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


