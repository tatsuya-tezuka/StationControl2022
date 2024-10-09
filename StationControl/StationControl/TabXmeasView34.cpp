/*============================================================================*/
/*! TabXmeasView.cpp

-# �ߑ��󋵃X�e�[�^�X��ʁiX�ё����i�V�n�j�j
*/
/*============================================================================*/
// TabXmeasView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabXmeasView34.h"
#include "afxdialogex.h"


// CTabXmeasView34 �_�C�A���O

IMPLEMENT_DYNCREATE(CTabXmeasView34, CDialogEx)

CTabXmeasView34::CTabXmeasView34(CWnd* pParent /*=NULL*/)
: CDialogEx(CTabXmeasView34::IDD, pParent)
{

}

CTabXmeasView34::~CTabXmeasView34()
{
}

void CTabXmeasView34::DoDataExchange(CDataExchange* pDX)
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
	DDX_Control(pDX, IDC_STATIC_RNGMODEVAL, mRngModeVal);
}


BEGIN_MESSAGE_MAP(CTabXmeasView34, CDialogEx)
END_MESSAGE_MAP()


// CTabXmeasView34 ���b�Z�[�W �n���h���[


BOOL CTabXmeasView34::OnInitDialog()
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

	mRngModeVal.SetGradient(false);
	mRngModeVal.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mRngModeVal.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mRngModeVal.SetTextStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	theApp.SetMonitorMode(this);

	InitStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! S�ё���

-# �ߑ��󋵃X�e�[�^�X�̍X�V

@param
@retval

*/
/*============================================================================*/
void CTabXmeasView34::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mXmeas34Param) / sizeof(mXmeas34Param[0]);
	for (int i = 0; i < szControl; i++)
	{
		CString mMonitorString = mXmeas34Param[i].nameK;
		CString str;
		stControlValue val;
		val.type = mXmeas34Param[i].type;

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
			if (mXmeas34Param[i].id == IDC_STATIC_RNGNUMVAL){
				str = CSatelliteData::AddCanma(obs_adr->d_data, 0);
				// ����擾
				CEngValCnv::obsname_t *obs_adr2 = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, RNG_DENOMINATOR_NUM);
				if (obs_adr2 == NULL)
				{
					continue;
				}
				CString str2 = CSatelliteData::AddCanma(obs_adr2->d_data, 0);
				str = str + _T("/") + str2;
			}
			else if (mXmeas34Param[i].id == IDC_STATIC_INTGT1VAL || mXmeas34Param[i].id == IDC_STATIC_INTGT2VAL){
				str = CSatelliteData::AddCanma(obs_adr->d_data, 0);
			}
			else
				str = CSatelliteData::AddCanma(obs_adr->d_data, 3);
			break;
		case	eControlTime:
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		}

		((CColorStatic*)GetDlgItem(mXmeas34Param[i].id))->SetText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! S�ё���

-# �ߑ��󋵃X�e�[�^�X�̏�����

@param
@retval

*/
/*============================================================================*/
void CTabXmeasView34::InitStatus()
{
	int szControl = sizeof(mXmeas34Param) / sizeof(mXmeas34Param[0]);
	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.type = mXmeas34Param[i].type;

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
		((CColorStatic*)GetDlgItem(mXmeas34Param[i].id))->SetText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! S�ё���

-# �ߑ��󋵃X�e�[�^�X�̍X�V

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
LRESULT CTabXmeasView34::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
