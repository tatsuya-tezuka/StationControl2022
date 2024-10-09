/*============================================================================*/
/*! TabXsendView.cpp

-# 捕捉状況ステータス画面（X帯送信(新系)）
*/
/*============================================================================*/
// TabXsendView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabXsendView.h"
#include "afxdialogex.h"


// CTabXsendView

IMPLEMENT_DYNCREATE(CTabXsendView, CDialogEx)

CTabXsendView::CTabXsendView(CWnd* pParent /*=NULL*/)
: CDialogEx(CTabXsendView::IDD, pParent)
{

}

CTabXsendView::~CTabXsendView()
{
}

void CTabXsendView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RFVAL, mRfVal);
	DDX_Control(pDX, IDC_STATIC_RFDUMMY, mDummyVal);
	DDX_Control(pDX, IDC_STATIC_RNGVAL, mRngVal);
}

BEGIN_MESSAGE_MAP(CTabXsendView, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RF, &CTabXsendView::OnBnClickedButtonRf)
	ON_BN_CLICKED(IDC_BUTTON_CMDSC, &CTabXsendView::OnBnClickedButtonCmdsc)
	ON_BN_CLICKED(IDC_BUTTON_MOD, &CTabXsendView::OnBnClickedButtonMod)
	ON_BN_CLICKED(IDC_BUTTON_INDEXCMD, &CTabXsendView::OnBnClickedButtonIndexcmd)
	ON_BN_CLICKED(IDC_BUTTON_INDEXCMD2, &CTabXsendView::OnBnClickedButtonIndexcmd2)
	ON_BN_CLICKED(IDC_BUTTON_UCCTRL, &CTabXsendView::OnBnClickedButtonUcctrl)
	ON_BN_CLICKED(IDC_BUTTON_TX, &CTabXsendView::OnBnClickedButtonTx)
	ON_BN_CLICKED(IDC_BUTTON_UC, &CTabXsendView::OnBnClickedButtonUc)
	ON_BN_CLICKED(IDC_BUTTON_SWEEP, &CTabXsendView::OnBnClickedButtonSweep)
	ON_BN_CLICKED(IDC_BUTTON_HOLDTONE, &CTabXsendView::OnBnClickedButtonHoldtone)
	ON_BN_CLICKED(IDC_BUTTON_BITRATE, &CTabXsendView::OnBnClickedButtonBitrate)
	ON_BN_CLICKED(IDC_BUTTON_XCMDOUT, &CTabXsendView::OnBnClickedButtonXcmdout)
END_MESSAGE_MAP()

// CTabXsendView メッセージ ハンドラー


BOOL CTabXsendView::OnInitDialog()
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
/*! X帯送信(新系)

-# RFボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView::OnBnClickedButtonRf()
{
	ControlParamExecute(0);
}


/*============================================================================*/
/*! X帯送信(新系)

-# CMD SCボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView::OnBnClickedButtonCmdsc()
{
	ControlParamExecute(1);
}

/*============================================================================*/
/*! X帯送信(新系)

-# MODボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView::OnBnClickedButtonMod()
{
	ControlParamExecute(2);
}

/*============================================================================*/
/*! X帯送信(新系)

-# INDEX CMDボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView::OnBnClickedButtonIndexcmd()
{
	ControlParamExecute(3);
}


/*============================================================================*/
/*! X帯送信(新系)

-# INDEX RNGボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView::OnBnClickedButtonIndexcmd2()
{
	ControlParamExecute(4);
}


/*============================================================================*/
/*! X帯送信(新系)

-# U/C CTRLボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView::OnBnClickedButtonUcctrl()
{
	ControlParamExecute(5);
}


/*============================================================================*/
/*! X帯送信(新系)

-# TXボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView::OnBnClickedButtonTx()
{
	ControlParamExecute(6);
}


/*============================================================================*/
/*! X帯送信(新系)

-# U/Cボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView::OnBnClickedButtonUc()
{
	ControlParamExecute(7);
}


/*============================================================================*/
/*! X帯送信(新系)

-# SWEEPボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView::OnBnClickedButtonSweep()
{
	ControlParamExecute(8);
}


/*============================================================================*/
/*! X帯送信(新系)

-# HOLD TONEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView::OnBnClickedButtonHoldtone()
{
	ControlParamExecute(9);
}


/*============================================================================*/
/*! X帯送信(新系)

-# BIT RATEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView::OnBnClickedButtonBitrate()
{
	ControlParamExecute(10);
}

/*============================================================================*/
/*! X帯送信(新系)

-# X-CMDOUTボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView::OnBnClickedButtonXcmdout()
{
	ControlParamExecute(11);
}

/*============================================================================*/
/*! X帯送信(新系)

-# 制御パラメタの実行

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendView::ControlParamExecute(UINT pos)
{
	CString mControlString = mXsendParam[pos].nameS;
	CString str;
	GetDlgItem(mXsendParam[pos].id)->GetWindowText(str);
	stControlValue val;
	val.ret = 0;
	val.type = mXsendParam[pos].type;
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
/*! X帯送信(新系)

-# 捕捉状況ステータスの更新

@param
@retval

*/
/*============================================================================*/
void CTabXsendView::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	//	if (sd.stn_tbl_access() == 1)
	//	{
	//		return;
	//	}

	int szControl = sizeof(mXsendParam) / sizeof(mXsendParam[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString mControlString = mXsendParam[i].nameK;
		CString str;
		GetDlgItem(mXsendParam[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mXsendParam[i].type;

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
			if (mXsendParam[i].id == IDC_BUTTON_INDEXCMD ||
				mXsendParam[i].id == IDC_BUTTON_INDEXCMD2)
			{
				str = CSatelliteData::AddCanma(obs_adr->d_data, 2);
			}
			else if (mXsendParam[i].id == IDC_BUTTON_BITRATE)
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

		if (mXsendParam[i].id == IDC_STATIC_RFVAL)
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

		if (mXsendParam[i].id == IDC_STATIC_RFDUMMY)
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

		if (mXsendParam[i].id == IDC_STATIC_RNGVAL)
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

		if (mXsendParam[i].id == IDC_STATIC_RFVAL ||
			mXsendParam[i].id == IDC_STATIC_RFDUMMY ||
			mXsendParam[i].id == IDC_STATIC_RNGVAL)
			((CColorStatic*)GetDlgItem(mXsendParam[i].id))->SetText(str);
		else
			GetDlgItem(mXsendParam[i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! X帯送信(新系)

-# 捕捉状況ステータスの初期化

@param
@retval

*/
/*============================================================================*/
void CTabXsendView::InitStatus()
{
	int szControl = sizeof(mXsendParam) / sizeof(mXsendParam[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mXsendParam[i].type;

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

		if (mXsendParam[i].id == IDC_STATIC_RFVAL)
		{
			mRfVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXsendParam[i].id == IDC_STATIC_RFDUMMY)
		{
			mDummyVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXsendParam[i].id == IDC_STATIC_RNGVAL)
		{
			mRngVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXsendParam[i].id == IDC_STATIC_RFVAL ||
			mXsendParam[i].id == IDC_STATIC_RFDUMMY ||
			mXsendParam[i].id == IDC_STATIC_RNGVAL){
			if (str.IsEmpty())
				str.Format(_T(" "));
			((CColorStatic*)GetDlgItem(mXsendParam[i].id))->SetText(str);
		}
		else
			GetDlgItem(mXsendParam[i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! X帯送信(新系)

-# 捕捉状況ステータスの更新

@param  なし
@retval なし
*/
/*============================================================================*/
LRESULT CTabXsendView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
