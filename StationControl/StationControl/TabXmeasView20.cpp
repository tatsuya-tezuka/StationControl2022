/*============================================================================*/
/*! TabSmeasView.cpp

-# 捕捉状況ステータス画面（S帯測距）
*/
/*============================================================================*/
// TabSmeasView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabXmeasView20.h"
#include "afxdialogex.h"


// CTabXmeasView20 ダイアログ

IMPLEMENT_DYNCREATE(CTabXmeasView20, CDialogEx)

CTabXmeasView20::CTabXmeasView20(CWnd* pParent /*=NULL*/)
: CDialogEx(CTabXmeasView20::IDD, pParent)
{

}

CTabXmeasView20::~CTabXmeasView20()
{
}

void CTabXmeasView20::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RNGMESVAL, mRngMesVal);
	DDX_Control(pDX, IDC_STATIC_OCVAL, mOcVal);
	DDX_Control(pDX, IDC_STATIC_RRMESVAL, mRrMesVal);
	DDX_Control(pDX, IDC_STATIC_OC2VAL, mRrOcVal);
	DDX_Control(pDX, IDC_STATIC_SAMPLEVAL, mSampleVal);
	DDX_Control(pDX, IDC_STATIC_RANGEVAL, mRangeVal);
	DDX_Control(pDX, IDC_STATIC_COHVAL, mCohVal);
}


BEGIN_MESSAGE_MAP(CTabXmeasView20, CDialogEx)
END_MESSAGE_MAP()


// CTabXmeasView20 メッセージ ハンドラー


BOOL CTabXmeasView20::OnInitDialog()
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

	mRangeVal.SetGradient(false);
	mRangeVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mRangeVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mRangeVal.SetTextStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	mCohVal.SetGradient(false);
	mCohVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mCohVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mCohVal.SetTextStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	theApp.SetMonitorMode(this);

	InitStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! S帯測距

-# 捕捉状況ステータスの更新

@param
@retval

*/
/*============================================================================*/
void CTabXmeasView20::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mXmeas20Param) / sizeof(mXmeas20Param[0]);
	for (int i = 0; i < szControl; i++)
	{
		CString mMonitorString = mXmeas20Param[i].nameK;
		CString str;
		stControlValue val;
		val.type = mXmeas20Param[i].type;

		if (mMonitorString.IsEmpty())
		{
			continue;
		}

		UINT sta = theApp.GetSelectStation();
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

		((CColorStatic*)GetDlgItem(mXmeas20Param[i].id))->SetText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! S帯測距

-# 捕捉状況ステータスの初期化

@param
@retval

*/
/*============================================================================*/
void CTabXmeasView20::InitStatus()
{
	int szControl = sizeof(mXmeas20Param) / sizeof(mXmeas20Param[0]);
	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.type = mXmeas20Param[i].type;

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
		((CColorStatic*)GetDlgItem(mXmeas20Param[i].id))->SetText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! S帯測距

-# 捕捉状況ステータスの更新

@param  なし
@retval なし
*/
/*============================================================================*/
LRESULT CTabXmeasView20::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
