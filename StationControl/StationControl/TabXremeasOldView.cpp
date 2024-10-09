// TabXremeasOldView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabXremeasOldView.h"
#include "afxdialogex.h"


// CTabXremeasOldView ダイアログ

IMPLEMENT_DYNCREATE(CTabXremeasOldView, CDialogEx)

CTabXremeasOldView::CTabXremeasOldView(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabXremeasOldView::IDD, pParent)
{

}

CTabXremeasOldView::~CTabXremeasOldView()
{
}

void CTabXremeasOldView::DoDataExchange(CDataExchange* pDX)
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
	DDX_Control(pDX, IDC_STATIC_COHVAL, mCohVal);
}


BEGIN_MESSAGE_MAP(CTabXremeasOldView, CDialogEx)
END_MESSAGE_MAP()


// CTabXremeasOldView メッセージ ハンドラー

BOOL CTabXremeasOldView::OnInitDialog()
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
void CTabXremeasOldView::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mXremeasOldParam) / sizeof(mXremeasOldParam[0]);
	for (int i = 0; i < szControl; i++)
	{
		CString mControlString = mXremeasOldParam[i].nameK;
		CString str;
		stControlValue val;
		val.type = mXremeasOldParam[i].type;

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
			if (mXremeasOldParam[i].id == IDC_STATIC_RNGNUMVAL){
				str = CSatelliteData::AddCanma(obs_adr->d_data, 0);
				// 分母取得
				CEngValCnv::obsname_t *obs_adr2 = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, RNG_DENOMINATOR_NUM);
				if (obs_adr2 == NULL)
				{
					continue;
				}
				CString str2 = CSatelliteData::AddCanma(obs_adr2->d_data, 0);
				str = str + _T("/") + str2;
			}
			else if (mXremeasOldParam[i].id == IDC_STATIC_INTGT1VAL || mXremeasOldParam[i].id == IDC_STATIC_INTGT2VAL){
				str = CSatelliteData::AddCanma(obs_adr->d_data, 0);
			}
			else
				str = CSatelliteData::AddCanma(obs_adr->d_data, 3);
			break;
		case	eControlTime:
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		}

		((CColorStatic*)GetDlgItem(mXremeasOldParam[i].id))->SetText(str);

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
void CTabXremeasOldView::InitStatus()
{
	int szControl = sizeof(mXremeasOldParam) / sizeof(mXremeasOldParam[0]);
	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.type = mXremeasOldParam[i].type;

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
		((CColorStatic*)GetDlgItem(mXremeasOldParam[i].id))->SetText(str);

	}
	UpdateData(FALSE);
}


LRESULT CTabXremeasOldView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
