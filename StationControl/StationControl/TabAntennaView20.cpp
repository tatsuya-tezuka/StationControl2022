/*============================================================================*/
/*! TabAntennaView.cpp

-# �ߑ��󋵃X�e�[�^�X��ʁi�A���e�i�j
*/
/*============================================================================*/
// TabAntennaView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabAntennaView20.h"
#include "afxdialogex.h"


// CTabAntennaView20 �_�C�A���O

IMPLEMENT_DYNCREATE(CTabAntennaView20, CDialogEx)

CTabAntennaView20::CTabAntennaView20(CWnd* pParent /*=NULL*/)
: CDialogEx(CTabAntennaView20::IDD, pParent)
{

}

CTabAntennaView20::~CTabAntennaView20()
{
}

void CTabAntennaView20::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_AZ_ANGLE, m_edAzVal);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_EL_ANGLE, m_edElVal);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_AZ_ANGLE2, m_edAzVal2);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_EL_ANGLE2, m_edElVal2);
	DDX_Control(pDX, IDC_STATIC_FASELOCK, mFalseLock);
}


BEGIN_MESSAGE_MAP(CTabAntennaView20, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_AZ_DRIVE, &CTabAntennaView20::OnBnClickedButtonAzDrive)
	ON_BN_CLICKED(IDC_BUTTON_EL_DRIVE, &CTabAntennaView20::OnBnClickedButtonElDrive)
	ON_BN_CLICKED(IDC_BUTTON_AZ_OFFSET, &CTabAntennaView20::OnBnClickedButtonAzOffset)
	ON_BN_CLICKED(IDC_BUTTON_EL_OFFSET, &CTabAntennaView20::OnBnClickedButtonElOffset)
	ON_BN_CLICKED(IDC_BUTTON_MODE, &CTabAntennaView20::OnBnClickedButtonMode)
	ON_BN_CLICKED(IDC_BUTTON_TIME, &CTabAntennaView20::OnBnClickedButtonTime)
	ON_BN_CLICKED(IDC_BUTTON_AUTO, &CTabAntennaView20::OnBnClickedButtonAuto)
END_MESSAGE_MAP()


// CTabAntennaView20 ���b�Z�[�W �n���h���[

BOOL CTabAntennaView20::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	mFalseLock.SetGradient(false);
	mFalseLock.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	mFalseLock.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
	mFalseLock.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

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
void CTabAntennaView20::OnBnClickedButtonAzDrive()
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
void CTabAntennaView20::OnBnClickedButtonElDrive()
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
void CTabAntennaView20::OnBnClickedButtonAzOffset()
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
void CTabAntennaView20::OnBnClickedButtonElOffset()
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
void CTabAntennaView20::OnBnClickedButtonMode()
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
void CTabAntennaView20::OnBnClickedButtonTime()
{
	ControlParamExecute(5);
}


/*============================================================================*/
/*! �A���e�i

-# AUTO INHIBIT�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabAntennaView20::OnBnClickedButtonAuto()
{
	ControlParamExecute(6);
}


/*============================================================================*/
/*! �A���e�i

-# ����p�����^�̎��s

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabAntennaView20::ControlParamExecute(UINT pos)
{
	CString mControlString = mAntenna20Param[pos].nameS;
	CString str;
	GetDlgItem(mAntenna20Param[pos].id)->GetWindowText(str);
	stControlValue val;
	val.ret = 0;
	val.type = mAntenna20Param[pos].type;
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
void CTabAntennaView20::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mAntenna20Param) / sizeof(mAntenna20Param[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString mControlString = mAntenna20Param[i].nameK;
		CString str;
		GetDlgItem(mAntenna20Param[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mAntenna20Param[i].type;

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

		if (mAntenna20Param[i].id == IDC_MFCMASKEDEDIT_AZ_ANGLE ||
			mAntenna20Param[i].id == IDC_MFCMASKEDEDIT_EL_ANGLE ||
			mAntenna20Param[i].id == IDC_MFCMASKEDEDIT_AZ_ANGLE2 ||
			mAntenna20Param[i].id == IDC_MFCMASKEDEDIT_EL_ANGLE2){
			((CColorStatic*)GetDlgItem(mAntenna20Param[i].id))->SetText(str);
		}
		else{
			if (mAntenna20Param[i].id == IDC_STATIC_FASELOCK){
				if (str.CompareNoCase(_T("ON")) == 0)
					mFalseLock.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
				else
					mFalseLock.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
				((CColorStatic*)GetDlgItem(mAntenna20Param[i].id))->SetText(str);
			}
			else {
				GetDlgItem(mAntenna20Param[i].id)->SetWindowText(str);
			}
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
void CTabAntennaView20::InitStatus()
{
	int szControl = sizeof(mAntenna20Param) / sizeof(mAntenna20Param[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mAntenna20Param[i].type;

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

		if (mAntenna20Param[i].id == IDC_MFCMASKEDEDIT_AZ_ANGLE ||
			mAntenna20Param[i].id == IDC_MFCMASKEDEDIT_EL_ANGLE ||
			mAntenna20Param[i].id == IDC_MFCMASKEDEDIT_AZ_ANGLE2 ||
			mAntenna20Param[i].id == IDC_MFCMASKEDEDIT_EL_ANGLE2){
			if (str.IsEmpty())
				str.Format(_T(" "));
			((CColorStatic*)GetDlgItem(mAntenna20Param[i].id))->SetText(str);
		}
		else{
			if (mAntenna20Param[i].id == IDC_STATIC_FASELOCK){
				mFalseLock.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
				if (str.IsEmpty())
					str.Format(_T(" "));
				((CColorStatic*)GetDlgItem(mAntenna20Param[i].id))->SetText(str);
			}
			else {
				GetDlgItem(mAntenna20Param[i].id)->SetWindowText(str);
			}
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
LRESULT CTabAntennaView20::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
