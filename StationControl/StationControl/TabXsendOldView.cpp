/*============================================================================*/
/*! TabXsendOldView.cpp

-# 捕捉状況ステータス画面（X帯送信(旧系)）
*/
/*============================================================================*/
// TabXsendOldView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabXsendOldView.h"
#include "afxdialogex.h"


// CTabXsendOldView ダイアログ

IMPLEMENT_DYNCREATE(CTabXsendOldView, CDialogEx)

CTabXsendOldView::CTabXsendOldView(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabXsendOldView::IDD, pParent)
{

}

CTabXsendOldView::~CTabXsendOldView()
{
}

void CTabXsendOldView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RNGVAL, mRngVal);
	DDX_Control(pDX, IDC_STATIC_RFVAL, mRfVal);
	DDX_Control(pDX, IDC_STATIC_RFDUMMY, mDummyVal);
}


BEGIN_MESSAGE_MAP(CTabXsendOldView, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RF, &CTabXsendOldView::OnBnClickedButtonRf)
	ON_BN_CLICKED(IDC_BUTTON_CMDSC, &CTabXsendOldView::OnBnClickedButtonCmdsc)
	ON_BN_CLICKED(IDC_BUTTON_MOD, &CTabXsendOldView::OnBnClickedButtonMod)
	ON_BN_CLICKED(IDC_BUTTON_INDEXCMD, &CTabXsendOldView::OnBnClickedButtonIndexcmd)
	ON_BN_CLICKED(IDC_BUTTON_INDEXCMD2, &CTabXsendOldView::OnBnClickedButtonIndexcmd2)
	ON_BN_CLICKED(IDC_BUTTON_UCCTRL, &CTabXsendOldView::OnBnClickedButtonUcctrl)
	ON_BN_CLICKED(IDC_BUTTON_TX, &CTabXsendOldView::OnBnClickedButtonTx)
	ON_BN_CLICKED(IDC_BUTTON_UC, &CTabXsendOldView::OnBnClickedButtonUc)
	ON_BN_CLICKED(IDC_BUTTON_SWEEP, &CTabXsendOldView::OnBnClickedButtonSweep)
	ON_BN_CLICKED(IDC_BUTTON_HOLDTONE, &CTabXsendOldView::OnBnClickedButtonHoldtone)
	ON_BN_CLICKED(IDC_BUTTON_BITRATE, &CTabXsendOldView::OnBnClickedButtonBitrate)
END_MESSAGE_MAP()


// CTabXsendOldView メッセージ ハンドラー


BOOL CTabXsendOldView::OnInitDialog()
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
/*! X帯送信(旧系)

-# RFボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendOldView::OnBnClickedButtonRf()
{
	ControlParamExecute(0);
}


/*============================================================================*/
/*! X帯送信(旧系)

-# CMD SCボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendOldView::OnBnClickedButtonCmdsc()
{
	ControlParamExecute(1);
}

/*============================================================================*/
/*! X帯送信(旧系)

-# MODボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendOldView::OnBnClickedButtonMod()
{
	ControlParamExecute(2);
}

/*============================================================================*/
/*! X帯送信(旧系)

-# INDEX CMDボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendOldView::OnBnClickedButtonIndexcmd()
{
	ControlParamExecute(3);
}


/*============================================================================*/
/*! X帯送信(旧系)

-# INDEX RNGボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendOldView::OnBnClickedButtonIndexcmd2()
{
	ControlParamExecute(4);
}


/*============================================================================*/
/*! X帯送信(旧系)

-# U/C CTRLボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendOldView::OnBnClickedButtonUcctrl()
{
	ControlParamExecute(5);
}


/*============================================================================*/
/*! X帯送信(旧系)

-# TXボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendOldView::OnBnClickedButtonTx()
{
	ControlParamExecute(6);
}


/*============================================================================*/
/*! X帯送信(旧系)

-# U/Cボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendOldView::OnBnClickedButtonUc()
{
	ControlParamExecute(7);
}


/*============================================================================*/
/*! X帯送信(旧系)

-# SWEEPボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendOldView::OnBnClickedButtonSweep()
{
	ControlParamExecute(8);
}


/*============================================================================*/
/*! X帯送信(旧系)

-# HOLD TONEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendOldView::OnBnClickedButtonHoldtone()
{
	ControlParamExecute(9);
}


/*============================================================================*/
/*! X帯送信(旧系)

-# BIT RATEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendOldView::OnBnClickedButtonBitrate()
{
	ControlParamExecute(10);
}

/*============================================================================*/
/*! X帯送信(旧系)

-# 制御パラメタの実行

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXsendOldView::ControlParamExecute(UINT pos)
{
	CString mControlString = mXsendOldParam[pos].nameS;
	CString str;
	GetDlgItem(mXsendOldParam[pos].id)->GetWindowText(str);
	stControlValue val;
	val.ret = 0;
	val.type = mXsendOldParam[pos].type;
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
/*! X帯送信(旧系)

-# 捕捉状況ステータスの更新

@param
@retval

*/
/*============================================================================*/
void CTabXsendOldView::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	//	if (sd.stn_tbl_access() == 1)
	//	{
	//		return;
	//	}

	int szControl = sizeof(mXsendOldParam) / sizeof(mXsendOldParam[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString mControlString = mXsendOldParam[i].nameK;
		CString str;
		GetDlgItem(mXsendOldParam[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mXsendOldParam[i].type;

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
			if (mXsendOldParam[i].id == IDC_BUTTON_INDEXCMD ||
				mXsendOldParam[i].id == IDC_BUTTON_INDEXCMD2)
			{
				str = CSatelliteData::AddCanma(obs_adr->d_data, 2);
			}
			else if (mXsendOldParam[i].id == IDC_BUTTON_BITRATE)
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

		if (mXsendOldParam[i].id == IDC_STATIC_RFVAL)
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

		if (mXsendOldParam[i].id == IDC_STATIC_RFDUMMY)
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

		if (mXsendOldParam[i].id == IDC_STATIC_RNGVAL)
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

		if (mXsendOldParam[i].id == IDC_STATIC_RFVAL ||
			mXsendOldParam[i].id == IDC_STATIC_RFDUMMY ||
			mXsendOldParam[i].id == IDC_STATIC_RNGVAL)
			((CColorStatic*)GetDlgItem(mXsendOldParam[i].id))->SetText(str);
		else
			GetDlgItem(mXsendOldParam[i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! X帯送信(旧系)

-# 捕捉状況ステータスの初期化

@param
@retval

*/
/*============================================================================*/
void CTabXsendOldView::InitStatus()
{
	int szControl = sizeof(mXsendOldParam) / sizeof(mXsendOldParam[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mXsendOldParam[i].type;

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

		if (mXsendOldParam[i].id == IDC_STATIC_RFVAL)
		{
			mRfVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXsendOldParam[i].id == IDC_STATIC_RFDUMMY)
		{
			mDummyVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXsendOldParam[i].id == IDC_STATIC_RNGVAL)
		{
			mRngVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXsendOldParam[i].id == IDC_STATIC_RFVAL ||
			mXsendOldParam[i].id == IDC_STATIC_RFDUMMY ||
			mXsendOldParam[i].id == IDC_STATIC_RNGVAL){
			if (str.IsEmpty())
				str.Format(_T(" "));
			((CColorStatic*)GetDlgItem(mXsendOldParam[i].id))->SetText(str);
		}
		else
			GetDlgItem(mXsendOldParam[i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! X帯送信(旧系)

-# 捕捉状況ステータスの更新

@param  なし
@retval なし
*/
/*============================================================================*/
LRESULT CTabXsendOldView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
