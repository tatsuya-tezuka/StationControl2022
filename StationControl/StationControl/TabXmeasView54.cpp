/*============================================================================*/
/*! TabXmeasView54.cpp

-# 捕捉状況ステータス画面（測距）
*/
/*============================================================================*/
// TabXmeasView54.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabXmeasView54.h"
#include "afxdialogex.h"


// CTabXmeasView54 ダイアログ

IMPLEMENT_DYNCREATE(CTabXmeasView54, CDialogEx)

CTabXmeasView54::CTabXmeasView54(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabXmeasView54::IDD, pParent)
{

}

CTabXmeasView54::~CTabXmeasView54()
{
}

void CTabXmeasView54::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RNGMESVAL, mRngMesVal);
	DDX_Control(pDX, IDC_STATIC_OCVAL, mOcVal);
	DDX_Control(pDX, IDC_STATIC_RRMESVAL, mRrMesVal);
	DDX_Control(pDX, IDC_STATIC_OC2VAL, mRrOcVal);
	DDX_Control(pDX, IDC_STATIC_SAMPLEVAL, mSampleVal);
	DDX_Control(pDX, IDC_STATIC_INTGT1VAL, mInteg1Val);
	DDX_Control(pDX, IDC_STATIC_INTGT2VAL, mIteng2Val);
	DDX_Control(pDX, IDC_STATIC_RNGNUMVAL, mRngNumVal);
	DDX_Control(pDX, IDC_STATIC_RANGEVAL, mRangeVal);
	DDX_Control(pDX, IDC_STATIC_RNGMODEVAL, mRngModeVal);
	DDX_Control(pDX, IDC_STATIC_COHVAL, mCohVal);
}


BEGIN_MESSAGE_MAP(CTabXmeasView54, CDialogEx)
END_MESSAGE_MAP()


// CTabXmeasView54 メッセージ ハンドラー

BOOL CTabXmeasView54::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	mRngMesVal.SetGradient(false);
	mRngMesVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mRngMesVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mRngMesVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mOcVal.SetGradient(false);
	mOcVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mOcVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mOcVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mRrMesVal.SetGradient(false);
	mRrMesVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mRrMesVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mRrMesVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mRrOcVal.SetGradient(false);
	mRrOcVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mRrOcVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mRrOcVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mSampleVal.SetGradient(false);
	mSampleVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mSampleVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mSampleVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mInteg1Val.SetGradient(false);
	mInteg1Val.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mInteg1Val.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mInteg1Val.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mIteng2Val.SetGradient(false);
	mIteng2Val.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mIteng2Val.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mIteng2Val.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mRngNumVal.SetGradient(false);
	mRngNumVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mRngNumVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mRngNumVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mRangeVal.SetGradient(false);
	mRangeVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mRangeVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mRangeVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mRngModeVal.SetGradient(false);
	mRngModeVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mRngModeVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mRngModeVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mCohVal.SetGradient(false);
	mCohVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mCohVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mCohVal.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	theApp.SetMonitorMode(this);

	InitStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! 測距

-# 捕捉状況ステータスの更新

@param
@retval

*/
/*============================================================================*/
void CTabXmeasView54::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT sta = theApp.GetSelectStation();

	int szControl = sizeof(mXmeasParam54) / sizeof(mXmeasParam54[0]);
	for (int i = 0; i < szControl; i++)
	{
		CString mMonitorString = mXmeasParam54[i].nameK;
		CString str;
		stControlValue val;
		val.type = mXmeasParam54[i].type;

		if (mMonitorString.IsEmpty())
		{
			continue;
		}

		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, mMonitorString);

		if (obs_adr == NULL)
		{
			continue;
		}

		switch (val.type){
		case	eControlStatus:
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			if (mXmeasParam54[i].id == IDC_STATIC_RANGEVAL)
			{
				str.Replace(_T("&"), _T("&&"));
			}
			break;
		case	eControlNumeric:
			str = CSatelliteData::AddCanma((double)obs_adr->l_data, 3);
			break;
		case	eControlDouble:
			if (mXmeasParam54[i].id == IDC_STATIC_SAMPLEVAL ||
				mXmeasParam54[i].id == IDC_STATIC_INTGT1VAL ||
				mXmeasParam54[i].id == IDC_STATIC_INTGT2VAL)
			{
				str = CSatelliteData::AddCanma(obs_adr->d_data, 0);
			}
//			else if (mXmeasParam54[i].id == IDC_STATIC_RNGNUMVAL)
//			{
//				str = CSatelliteData::AddCanma(obs_adr->d_data, 0);
//				// 分母取得
//				CEngValCnv::obsname_t *obs_adr2 = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, _T("DSNTCR.RNG_NUM"));
//				if (obs_adr2 == NULL)
//				{
//					continue;
//				}
//				CString str2 = CSatelliteData::AddCanma(obs_adr2->d_data, 0);
//				str = str + _T("/") + str2;
//			}
			else
			{
				str = CSatelliteData::AddCanma(obs_adr->d_data, 3);
			}
			break;
		case	eControlTime:
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		}

		((CColorStatic*)GetDlgItem(mXmeasParam54[i].id))->SetText(str);
	}

	// X帯,Ka帯,従来型,再生型を調べる
	UINT nBandType = eXmeasBandType_X;
	UINT nRngMode = eXmeasRngMode_Turnaround;
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, JUDGE_BANDTYPE_STR);
	if (obs_adr != NULL)
	{
		CString str = CString(obs_adr->sz_sts_name);
		for (UINT i = 0; i < eXmeasBandType_Num; i++)
		{
			if (str.CompareNoCase(mBandTypeString[i]) == 0)
			{
				nBandType = i;
				break;
			}
		}
	}
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, JUDGE_RNGMODE_STR);
	if (obs_adr != NULL)
	{
		CString str = CString(obs_adr->sz_sts_name);
		for (UINT i = 0; i < eXmeasRngMode_Num; i++)
		{
			if (str.CompareNoCase(mRngModeString[i]) == 0)
			{
				nRngMode = i;
				break;
			}
		}
	}

	// RNG MEAS, O-C, RR MEAS, O-C
	szControl = sizeof(mXmeasParamRngRr54[nBandType]) / sizeof(mXmeasParamRngRr54[nBandType][0]);
	for (int i = 0; i < szControl; i++)
	{
		CString mMonitorString = mXmeasParamRngRr54[nBandType][i].nameK;
		CString str;
		stControlValue val;
		val.type = mXmeasParamRngRr54[nBandType][i].type;

		if (mMonitorString.IsEmpty())
		{
			continue;
		}

		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, mMonitorString);

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

		((CColorStatic*)GetDlgItem(mXmeasParamRngRr54[nBandType][i].id))->SetText(str);
	}

	// RNG NUM
	szControl = sizeof(mXmeasParamRngRr54[nBandType][nRngMode]) / sizeof(mXmeasParamRngNum54[nBandType][nRngMode][0]);
	for (int i = 0; i < szControl; i++)
	{
		CString mMonitorString = mXmeasParamRngNum54[nBandType][nRngMode][i].nameK;
		CString str;
		stControlValue val;
		val.type = mXmeasParamRngNum54[nBandType][nRngMode][i].type;

		if (mMonitorString.IsEmpty())
		{
			continue;
		}

		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, mMonitorString);

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
			if (mXmeasParamRngNum54[nBandType][nRngMode][i].id == IDC_STATIC_RNGNUMVAL)
			{
				str = CSatelliteData::AddCanma(obs_adr->d_data, 0);
				// 分母取得
				CEngValCnv::obsname_t *obs_adr2 = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, _T("DSNTCR.RNG_NUM"));
				if (obs_adr2 == NULL)
				{
					continue;
				}
				CString str2 = CSatelliteData::AddCanma(obs_adr2->d_data, 0);
				str = str + _T("/") + str2;
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

		((CColorStatic*)GetDlgItem(mXmeasParamRngNum54[nBandType][nRngMode][i].id))->SetText(str);
	}

	UpdateData(FALSE);
}


/*============================================================================*/
/*! 測距

-# 捕捉状況ステータスの初期化

@param
@retval

*/
/*============================================================================*/
void CTabXmeasView54::InitStatus()
{
	int szControl = sizeof(mXmeasParam54) / sizeof(mXmeasParam54[0]);
	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.type = mXmeasParam54[i].type;

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

		if (str.IsEmpty())
			str.Format(_T(" "));
		((CColorStatic*)GetDlgItem(mXmeasParam54[i].id))->SetText(str);
	}

	szControl = sizeof(mXmeasParamRngRr54[0]) / sizeof(mXmeasParamRngRr54[0][0]);
	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.type = mXmeasParamRngRr54[0][i].type;

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

		if (str.IsEmpty())
			str.Format(_T(" "));
		((CColorStatic*)GetDlgItem(mXmeasParamRngRr54[0][i].id))->SetText(str);
	}

	szControl = sizeof(mXmeasParamRngNum54[0][0]) / sizeof(mXmeasParamRngNum54[0][0][0]);
	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.type = mXmeasParamRngNum54[0][0][i].type;

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

		if (str.IsEmpty())
			str.Format(_T(" "));
		((CColorStatic*)GetDlgItem(mXmeasParamRngNum54[0][0][i].id))->SetText(str);
	}

	UpdateData(FALSE);
}


/*============================================================================*/
/*! 測距

-# 捕捉状況ステータスの更新

@param  なし
@retval なし
*/
/*============================================================================*/
LRESULT CTabXmeasView54::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

