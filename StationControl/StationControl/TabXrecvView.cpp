/*============================================================================*/
/*! TabXrecvView.cpp

-# 捕捉状況ステータス画面（X帯受信(新系)）
*/
/*============================================================================*/
// TabXrecvView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabXrecvView.h"
#include "afxdialogex.h"


// CTabXrecvView ダイアログ

IMPLEMENT_DYNCREATE(CTabXrecvView, CDialogEx)

CTabXrecvView::CTabXrecvView(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabXrecvView::IDD, pParent)
{

}

CTabXrecvView::~CTabXrecvView()
{
}

void CTabXrecvView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CARRVAL, mCarrVal);
	DDX_Control(pDX, IDC_STATIC_SUBCARRVAL, mSubCarrVal);
	DDX_Control(pDX, IDC_STATIC_VITERBIVAL, mViterbiVal);
	DDX_Control(pDX, IDC_STATIC_BITVAL, mBitVal);
	DDX_Control(pDX, IDC_STATIC_FRAMEVAL, mFrameVal);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_AGC, mAgcVal);
	DDX_Control(pDX, IDC_STATIC_EB, mEbVal);
}


BEGIN_MESSAGE_MAP(CTabXrecvView, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_REJECT, &CTabXrecvView::OnBnClickedButtonReject)
	ON_BN_CLICKED(IDC_BUTTON_PLLBW, &CTabXrecvView::OnBnClickedButtonPllbw)
	ON_BN_CLICKED(IDC_BUTTON_AGCBW, &CTabXrecvView::OnBnClickedButtonAgcbw)
	ON_BN_CLICKED(IDC_BUTTON_BITRATE, &CTabXrecvView::OnBnClickedButtonBitrate)
	ON_BN_CLICKED(IDC_BUTTON_DCCTRL, &CTabXrecvView::OnBnClickedButtonDcctrl)
	ON_BN_CLICKED(IDC_BUTTON_RX, &CTabXrecvView::OnBnClickedButtonRx)
	ON_BN_CLICKED(IDC_BUTTON_DC, &CTabXrecvView::OnBnClickedButtonDc)
	ON_BN_CLICKED(IDC_BUTTON_XTLMIN, &CTabXrecvView::OnBnClickedButtonXtlmin)
END_MESSAGE_MAP()


// CTabXrecvView メッセージ ハンドラー


BOOL CTabXrecvView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	mCarrVal.SetGradient(false);
	mCarrVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mCarrVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
	mCarrVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mSubCarrVal.SetGradient(false);
	mSubCarrVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mSubCarrVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
	mSubCarrVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mViterbiVal.SetGradient(false);
	mViterbiVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mViterbiVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
	mViterbiVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mBitVal.SetGradient(false);
	mBitVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mBitVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
	mBitVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mFrameVal.SetGradient(false);
	mFrameVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mFrameVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
	mFrameVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mAgcVal.SetGradient(false);
	mAgcVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mAgcVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mAgcVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mEbVal.SetGradient(false);
	mEbVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mEbVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mEbVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	theApp.SetMonitorMode(this);

	InitStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! X帯受信(新系)

-# REJECTボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXrecvView::OnBnClickedButtonReject()
{
	ControlParamExecute(0);
}


/*============================================================================*/
/*! X帯受信(新系)

-# PLL BWボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXrecvView::OnBnClickedButtonPllbw()
{
	ControlParamExecute(1);
}


/*============================================================================*/
/*! X帯受信(新系)

-# AGC BWボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXrecvView::OnBnClickedButtonAgcbw()
{
	ControlParamExecute(2);
}


/*============================================================================*/
/*! X帯受信(新系)

-# BITRATEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXrecvView::OnBnClickedButtonBitrate()
{
	ControlParamExecute(3);
}


/*============================================================================*/
/*! X帯受信(新系)

-# D/C CTRLボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXrecvView::OnBnClickedButtonDcctrl()
{
	ControlParamExecute(4);
}


/*============================================================================*/
/*! X帯受信(新系)

-# RXボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXrecvView::OnBnClickedButtonRx()
{
	ControlParamExecute(5);
}


/*============================================================================*/
/*! X帯受信(新系)

-# D/Cボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXrecvView::OnBnClickedButtonDc()
{
	ControlParamExecute(6);
}

/*============================================================================*/
/*! X帯受信(新系)

-# X-TLMINボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXrecvView::OnBnClickedButtonXtlmin()
{
	ControlParamExecute(7);
}

/*============================================================================*/
/*! X帯受信(新系)

-# 制御パラメタの実行

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabXrecvView::ControlParamExecute(UINT pos)
{
	CString mControlString = mXrecvParam[pos].nameS;
	CString str;
	GetDlgItem(mXrecvParam[pos].id)->GetWindowText(str);

	if (mXrecvParam[pos].id == IDC_BUTTON_PLLBW)
	{
		if (str.CompareNoCase(_T("1000Hz")) == 0)
		{
			str = _T("1kHz");
		}
	}

	stControlValue val;
	val.ret = 0;
	val.type = mXrecvParam[pos].type;
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
/*! X帯受信(新系)

-# 捕捉状況ステータスの更新

@param
@retval

*/
/*============================================================================*/
void CTabXrecvView::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mXrecvParam) / sizeof(mXrecvParam[0]);

	for (int i = 0; i < szControl; i++)
	{
		if (mXrecvParam[i].id == IDC_BUTTON_REJECT)
			continue;

		CString mControlString = mXrecvParam[i].nameK;
		CString str;
		GetDlgItem(mXrecvParam[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mXrecvParam[i].type;

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
			if (mXrecvParam[i].id == IDC_BUTTON_BITRATE)
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

		if (mXrecvParam[i].id == IDC_STATIC_CARRVAL)
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

		if (mXrecvParam[i].id == IDC_STATIC_SUBCARRVAL)
		{
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				mSubCarrVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				mSubCarrVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
			}
		}

		if (mXrecvParam[i].id == IDC_STATIC_VITERBIVAL)
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

		if (mXrecvParam[i].id == IDC_STATIC_BITVAL)
		{
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				mBitVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				mBitVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
			}
		}

		if (mXrecvParam[i].id == IDC_STATIC_FRAMEVAL)
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

		if (mXrecvParam[i].id == IDC_BUTTON_PLLBW)
		{
			if (str.CompareNoCase(_T("1kHz")) == 0)
			{
				str = _T("1000Hz");
			}
		}

		if (mXrecvParam[i].id == IDC_STATIC_EB ||
			mXrecvParam[i].id == IDC_STATIC_CARRVAL ||
			mXrecvParam[i].id == IDC_STATIC_SUBCARRVAL ||
			mXrecvParam[i].id == IDC_STATIC_VITERBIVAL ||
			mXrecvParam[i].id == IDC_STATIC_BITVAL ||
			mXrecvParam[i].id == IDC_STATIC_FRAMEVAL ||
			mXrecvParam[i].id == IDC_MFCMASKEDEDIT_AGC)
			((CColorStatic*)GetDlgItem(mXrecvParam[i].id))->SetText(str);
		else
			GetDlgItem(mXrecvParam[i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! X帯受信(新系)

-# 捕捉状況ステータスの初期化

@param
@retval

*/
/*============================================================================*/
void CTabXrecvView::InitStatus()
{
	int szControl = sizeof(mXrecvParam) / sizeof(mXrecvParam[0]);

	for (int i = 0; i < szControl; i++)
	{
		if (mXrecvParam[i].id == IDC_BUTTON_REJECT)
			continue;

		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mXrecvParam[i].type;

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

		if (mXrecvParam[i].id == IDC_STATIC_CARRVAL)
		{
			mCarrVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXrecvParam[i].id == IDC_STATIC_SUBCARRVAL)
		{
			mSubCarrVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXrecvParam[i].id == IDC_STATIC_VITERBIVAL)
		{
			mViterbiVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXrecvParam[i].id == IDC_STATIC_BITVAL)
		{
			mBitVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXrecvParam[i].id == IDC_STATIC_FRAMEVAL)
		{
			mFrameVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXrecvParam[i].id == IDC_STATIC_EB ||
			mXrecvParam[i].id == IDC_STATIC_CARRVAL ||
			mXrecvParam[i].id == IDC_STATIC_SUBCARRVAL ||
			mXrecvParam[i].id == IDC_STATIC_VITERBIVAL ||
			mXrecvParam[i].id == IDC_STATIC_BITVAL ||
			mXrecvParam[i].id == IDC_STATIC_FRAMEVAL ||
			mXrecvParam[i].id == IDC_MFCMASKEDEDIT_AGC){
			if (str.IsEmpty())
				str.Format(_T(" "));
			((CColorStatic*)GetDlgItem(mXrecvParam[i].id))->SetText(str);
		}
		else
			GetDlgItem(mXrecvParam[i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! X帯受信(新系)

-# 捕捉状況ステータスの更新

@param  なし
@retval なし
*/
/*============================================================================*/
LRESULT CTabXrecvView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
