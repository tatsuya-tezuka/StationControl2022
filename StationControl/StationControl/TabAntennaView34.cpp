/*============================================================================*/
/*! TabAntennaView.cpp

-# 捕捉状況ステータス画面（アンテナ）
*/
/*============================================================================*/
// TabAntennaView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabAntennaView34.h"
#include "afxdialogex.h"


// CTabAntennaView34 ダイアログ

IMPLEMENT_DYNCREATE(CTabAntennaView34, CDialogEx)

CTabAntennaView34::CTabAntennaView34(CWnd* pParent /*=NULL*/)
: CDialogEx(CTabAntennaView34::IDD, pParent)
, m_dRaOffset(0.0)
, m_dDecOffset(0.0)
{

}

CTabAntennaView34::~CTabAntennaView34()
{
}

void CTabAntennaView34::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_AZ_ANGLE, m_edAzVal);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_EL_ANGLE, m_edElVal);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_AZ_ANGLE2, m_edAzVal2);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_EL_ANGLE2, m_edElVal2);
	DDX_Control(pDX, IDC_STATIC_FASELOCK, mFalseLock);
	DDX_Control(pDX, IDC_STATIC_CONTDATE, mContTime);
}


BEGIN_MESSAGE_MAP(CTabAntennaView34, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_AZ_DRIVE, &CTabAntennaView34::OnBnClickedButtonAzDrive)
	ON_BN_CLICKED(IDC_BUTTON_EL_DRIVE, &CTabAntennaView34::OnBnClickedButtonElDrive)
	ON_BN_CLICKED(IDC_BUTTON_AZ_OFFSET, &CTabAntennaView34::OnBnClickedButtonAzOffset)
	ON_BN_CLICKED(IDC_BUTTON_EL_OFFSET, &CTabAntennaView34::OnBnClickedButtonElOffset)
	ON_BN_CLICKED(IDC_BUTTON_MODE, &CTabAntennaView34::OnBnClickedButtonMode)
	ON_BN_CLICKED(IDC_BUTTON_TIME, &CTabAntennaView34::OnBnClickedButtonTime)
	ON_BN_CLICKED(IDC_BUTTON_AUTO, &CTabAntennaView34::OnBnClickedButtonAuto)
	ON_BN_CLICKED(IDC_BUTTON_RAOFFSET, &CTabAntennaView34::OnBnClickedButtonRaoffset)
	ON_BN_CLICKED(IDC_BUTTON_DECOFFSET, &CTabAntennaView34::OnBnClickedButtonDecoffset)
END_MESSAGE_MAP()


// CTabAntennaView34 メッセージ ハンドラー

BOOL CTabAntennaView34::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_edAzVal.SetGradient(false);
	m_edAzVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	m_edAzVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	m_edAzVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	m_edElVal.SetGradient(false);
	m_edElVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	m_edElVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	m_edElVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	m_edAzVal2.SetGradient(false);
	m_edAzVal2.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	m_edAzVal2.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	m_edAzVal2.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	m_edElVal2.SetGradient(false);
	m_edElVal2.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	m_edElVal2.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	m_edElVal2.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mFalseLock.SetGradient(false);
	mFalseLock.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mFalseLock.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
	mFalseLock.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mContTime.SetGradient(false);
	mContTime.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mContTime.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mContTime.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	theApp.SetMonitorMode(this);

	InitStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! アンテナ

-# AZ DRIVEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView34::OnBnClickedButtonAzDrive()
{
	ControlParamExecute(0);
}



/*============================================================================*/
/*! アンテナ

-# EL DRIVEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView34::OnBnClickedButtonElDrive()
{
	ControlParamExecute(1);
}



/*============================================================================*/
/*! アンテナ

-# AZ OFFSETボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView34::OnBnClickedButtonAzOffset()
{
	ControlParamExecute(2);
}


/*============================================================================*/
/*! アンテナ

-# EL OFFSETボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView34::OnBnClickedButtonElOffset()
{
	ControlParamExecute(3);
}

/*============================================================================*/
/*! アンテナ

-# MODEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView34::OnBnClickedButtonMode()
{
	ControlParamExecute(4);
}


/*============================================================================*/
/*! アンテナ

-# TIME OFFSETボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView34::OnBnClickedButtonTime()
{
	ControlParamExecute(5);
}

/*============================================================================*/
/*! アンテナ

-# AUTO INHIBITボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView34::OnBnClickedButtonAuto()
{
	ControlParamExecute(6);
}

/*============================================================================*/
/*! アンテナ

-# RA OFFSETボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView34::OnBnClickedButtonRaoffset()
{
	ControlParamExecute(7);
}


/*============================================================================*/
/*! アンテナ

-# DEC OFFSETボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView34::OnBnClickedButtonDecoffset()
{
	ControlParamExecute(8);
}


/*============================================================================*/
/*! アンテナ

-# 制御パラメタの実行

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView34::ControlParamExecute(UINT pos)
{
	CString mControlString = mAntenna34Param[pos].nameS;
	CString str;
	GetDlgItem(mAntenna34Param[pos].id)->GetWindowText(str);
	stControlValue val;
	val.ret = 0;
	val.type = mAntenna34Param[pos].type;
	switch (val.type){
	case	eControlStatus:
		sprintf_s(val.buf, 80, "%s", (LPCSTR)CStringA(str));
		break;
	case	eControlNumeric:
		str.Remove(',');
		val.val = _wtoi(str);
		break;
	case	eControlDouble:
		if (mAntenna34Param[pos].id == IDC_BUTTON_RAOFFSET){
			val.fval = m_dRaOffset;
		}
		else if (mAntenna34Param[pos].id == IDC_BUTTON_DECOFFSET){
			val.fval = m_dDecOffset;
		}
		else{
			str.Remove(',');
			val.fval = _wtof(str);
		}
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
		if (mAntenna34Param[pos].id == IDC_BUTTON_RAOFFSET){
			m_dRaOffset = val.fval;
		}
		else if (mAntenna34Param[pos].id == IDC_BUTTON_DECOFFSET){
			m_dDecOffset = val.fval;
		}
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

	if (theApp.SendControlChkDlg(mControlString, str, _T(""), _T("")))
	{
		if (mAntenna34Param[pos].id == IDC_BUTTON_RAOFFSET ||
			mAntenna34Param[pos].id == IDC_BUTTON_DECOFFSET)
		{
			CTime time;
			theApp.GetCurrentUtcTime(time);
			CString strt = time.Format("%m-%d %H:%M:%S");
			mContTime.SetText(strt);
		}
	}
}


/*============================================================================*/
/*! アンテナ

-# 捕捉状況ステータスの更新

@param
@retval

*/
/*============================================================================*/
void CTabAntennaView34::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mAntenna34Param) / sizeof(mAntenna34Param[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString mControlString = mAntenna34Param[i].nameK;
		CString str;
		GetDlgItem(mAntenna34Param[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mAntenna34Param[i].type;

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
		if (mAntenna34Param[i].id == IDC_MFCMASKEDEDIT_AZ_ANGLE ||
			mAntenna34Param[i].id == IDC_MFCMASKEDEDIT_EL_ANGLE ||
			mAntenna34Param[i].id == IDC_MFCMASKEDEDIT_AZ_ANGLE2 ||
			mAntenna34Param[i].id == IDC_MFCMASKEDEDIT_EL_ANGLE2){
			((CColorStatic*)GetDlgItem(mAntenna34Param[i].id))->SetText(str);
		}
		else if (mAntenna34Param[i].id == IDC_BUTTON_RAOFFSET ||
				 mAntenna34Param[i].id == IDC_BUTTON_DECOFFSET){
			// 表示変更なし
		}
		else{
			if (mAntenna34Param[i].id == IDC_STATIC_FASELOCK){
				if (str.CompareNoCase(_T("ON")) == 0)
					mFalseLock.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
				else
					mFalseLock.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
				((CColorStatic*)GetDlgItem(mAntenna34Param[i].id))->SetText(str);
			}
			else{
				GetDlgItem(mAntenna34Param[i].id)->SetWindowText(str);
			}
		}
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! アンテナ

-# 捕捉状況ステータスの初期化

@param
@retval

*/
/*============================================================================*/
void CTabAntennaView34::InitStatus()
{
	m_dRaOffset = 0.0;
	m_dDecOffset = 0.0;

	int szControl = sizeof(mAntenna34Param) / sizeof(mAntenna34Param[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mAntenna34Param[i].type;

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
		if (mAntenna34Param[i].id == IDC_MFCMASKEDEDIT_AZ_ANGLE ||
			mAntenna34Param[i].id == IDC_MFCMASKEDEDIT_EL_ANGLE ||
			mAntenna34Param[i].id == IDC_MFCMASKEDEDIT_AZ_ANGLE2 ||
			mAntenna34Param[i].id == IDC_MFCMASKEDEDIT_EL_ANGLE2){
			if (str.IsEmpty())
				str.Format(_T(" "));
			((CColorStatic*)GetDlgItem(mAntenna34Param[i].id))->SetText(str);
		}
		else if (mAntenna34Param[i].id == IDC_BUTTON_RAOFFSET ||
				 mAntenna34Param[i].id == IDC_BUTTON_DECOFFSET){
			// 表示変更なし
		}
		else{
			if (mAntenna34Param[i].id == IDC_STATIC_FASELOCK){
				mFalseLock.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
				if (str.IsEmpty())
					str.Format(_T(" "));
				((CColorStatic*)GetDlgItem(mAntenna34Param[i].id))->SetText(str);
			}
			else{
				GetDlgItem(mAntenna34Param[i].id)->SetWindowText(str);
			}
		}
	}

	// RA/DEC OFFSETの制御日時を消しておく
	mContTime.SetText(_T(" "));

	UpdateData(FALSE);
}


/*============================================================================*/
/*! アンテナ

-# 捕捉状況ステータスの更新

@param  なし
@retval なし
*/
/*============================================================================*/
LRESULT CTabAntennaView34::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

