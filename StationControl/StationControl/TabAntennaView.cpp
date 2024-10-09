/*============================================================================*/
/*! TabAntennaView.cpp

-# �ߑ��󋵃X�e�[�^�X��ʁi�A���e�i�j
*/
/*============================================================================*/
// TabAntennaView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabAntennaView.h"
#include "afxdialogex.h"


// CTabAntennaView �_�C�A���O

IMPLEMENT_DYNCREATE(CTabAntennaView, CDialogEx)

CTabAntennaView::CTabAntennaView(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabAntennaView::IDD, pParent)
{

}

CTabAntennaView::~CTabAntennaView()
{
}

void CTabAntennaView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_AZ_ANGLE, mAzAngle);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_EL_ANGLE, mElAngle);
}


BEGIN_MESSAGE_MAP(CTabAntennaView, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_AZ_DRIVE, &CTabAntennaView::OnBnClickedButtonAzDrive)
	ON_BN_CLICKED(IDC_BUTTON_EL_DRIVE, &CTabAntennaView::OnBnClickedButtonElDrive)
	ON_BN_CLICKED(IDC_BUTTON_AZ_OFFSET, &CTabAntennaView::OnBnClickedButtonAzOffset)
	ON_BN_CLICKED(IDC_BUTTON_EL_OFFSET, &CTabAntennaView::OnBnClickedButtonElOffset)
	ON_BN_CLICKED(IDC_BUTTON_MODE, &CTabAntennaView::OnBnClickedButtonMode)
	ON_BN_CLICKED(IDC_BUTTON_TIME, &CTabAntennaView::OnBnClickedButtonTime)
END_MESSAGE_MAP()


// CTabAntennaView ���b�Z�[�W �n���h���[

BOOL CTabAntennaView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	mAzAngle.SetGradient(false);
	mAzAngle.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mAzAngle.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mAzAngle.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mElAngle.SetGradient(false);
	mElAngle.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mElAngle.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mElAngle.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	theApp.SetMonitorMode(this);

	InitStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! �A���e�i

-# AZ DRIVE�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabAntennaView::OnBnClickedButtonAzDrive()
{
	ControlParamExecute(0);
}



/*============================================================================*/
/*! �A���e�i

-# EL DRIVE�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabAntennaView::OnBnClickedButtonElDrive()
{
	ControlParamExecute(1);
}



/*============================================================================*/
/*! �A���e�i

-# AZ OFFSET�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabAntennaView::OnBnClickedButtonAzOffset()
{
	ControlParamExecute(2);
}


/*============================================================================*/
/*! �A���e�i

-# EL OFFSET�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabAntennaView::OnBnClickedButtonElOffset()
{
	ControlParamExecute(3);
}

/*============================================================================*/
/*! �A���e�i

-# MODE�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabAntennaView::OnBnClickedButtonMode()
{
	ControlParamExecute(4);
}


/*============================================================================*/
/*! �A���e�i

-# TIME OFFSET�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabAntennaView::OnBnClickedButtonTime()
{
	ControlParamExecute(5);
}


/*============================================================================*/
/*! �A���e�i

-# ����p�����^�̎��s

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabAntennaView::ControlParamExecute(UINT pos)
{
	CString mControlString = mAntennaParam[pos].nameS;
	CString str;
	GetDlgItem(mAntennaParam[pos].id)->GetWindowText(str);
	stControlValue val;
	val.ret = 0;
	val.type = mAntennaParam[pos].type;
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
/*! �A���e�i

-# �ߑ��󋵃X�e�[�^�X�̍X�V

@param
@retval

*/
/*============================================================================*/
void CTabAntennaView::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mAntennaParam) / sizeof(mAntennaParam[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString mControlString = mAntennaParam[i].nameK;
		CString str;
		GetDlgItem(mAntennaParam[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mAntennaParam[i].type;

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
			if (mAntennaParam[i].id == IDC_MFCMASKEDEDIT_AZ_ANGLE ||
				mAntennaParam[i].id == IDC_MFCMASKEDEDIT_EL_ANGLE ||
				mAntennaParam[i].id == IDC_BUTTON_AZ_OFFSET ||
				mAntennaParam[i].id == IDC_BUTTON_EL_OFFSET){
				str = CSatelliteData::AddCanma(obs_adr->d_data, 4);
			}
			else{
				str = CSatelliteData::AddCanma(obs_adr->d_data, 3);
			}
			break;
		case	eControlTime:
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		}

		if (mAntennaParam[i].id == IDC_MFCMASKEDEDIT_AZ_ANGLE ||
			mAntennaParam[i].id == IDC_MFCMASKEDEDIT_EL_ANGLE){
			((CColorStatic*)GetDlgItem(mAntennaParam[i].id))->SetText(str);
		}
		else{
			GetDlgItem(mAntennaParam[i].id)->SetWindowText(str);
		}
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! �A���e�i

-# �ߑ��󋵃X�e�[�^�X�̏�����

@param
@retval

*/
/*============================================================================*/
void CTabAntennaView::InitStatus()
{
	int szControl = sizeof(mAntennaParam) / sizeof(mAntennaParam[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mAntennaParam[i].type;

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

		if (mAntennaParam[i].id == IDC_MFCMASKEDEDIT_AZ_ANGLE ||
			mAntennaParam[i].id == IDC_MFCMASKEDEDIT_EL_ANGLE){
			if (str.IsEmpty())
				str.Format(_T(" "));
			((CColorStatic*)GetDlgItem(mAntennaParam[i].id))->SetText(str);
		}
		else{
			GetDlgItem(mAntennaParam[i].id)->SetWindowText(str);
		}
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! �A���e�i

-# �ߑ��󋵃X�e�[�^�X�̍X�V

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
LRESULT CTabAntennaView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
