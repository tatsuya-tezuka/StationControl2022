/*============================================================================*/
/*! TabXrecvOldView.cpp

-# �ߑ��󋵃X�e�[�^�X��ʁiX�ю�M(���n)�j
*/
/*============================================================================*/
// TabXrecvOldView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabXrecvOldView.h"
#include "afxdialogex.h"


// CTabXrecvOldView �_�C�A���O

IMPLEMENT_DYNCREATE(CTabXrecvOldView, CDialogEx)

CTabXrecvOldView::CTabXrecvOldView(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabXrecvOldView::IDD, pParent)
{

}

CTabXrecvOldView::~CTabXrecvOldView()
{
}

void CTabXrecvOldView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CARRVAL, mCarrVal);
	DDX_Control(pDX, IDC_STATIC_SUBCARRVAL, mSubCarrVal);
	DDX_Control(pDX, IDC_STATIC_VITERBIVAL, mViterbiVal);
	DDX_Control(pDX, IDC_STATIC_BITVAL, mBitVal);
	DDX_Control(pDX, IDC_STATIC_FRAMEVAL, mFrameVal);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_AGC, mAgcVal);
}


BEGIN_MESSAGE_MAP(CTabXrecvOldView, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_REJECT, &CTabXrecvOldView::OnBnClickedButtonReject)
	ON_BN_CLICKED(IDC_BUTTON_PLLBW, &CTabXrecvOldView::OnBnClickedButtonPllbw)
	ON_BN_CLICKED(IDC_BUTTON_AGCBW, &CTabXrecvOldView::OnBnClickedButtonAgcbw)
	ON_BN_CLICKED(IDC_BUTTON_BITRATE, &CTabXrecvOldView::OnBnClickedButtonBitrate)
	ON_BN_CLICKED(IDC_BUTTON_DCCTRL, &CTabXrecvOldView::OnBnClickedButtonDcctrl)
	ON_BN_CLICKED(IDC_BUTTON_RX, &CTabXrecvOldView::OnBnClickedButtonRx)
	ON_BN_CLICKED(IDC_BUTTON_DC, &CTabXrecvOldView::OnBnClickedButtonDc)
END_MESSAGE_MAP()


// CTabXrecvOldView ���b�Z�[�W �n���h���[

BOOL CTabXrecvOldView::OnInitDialog()
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

	theApp.SetMonitorMode(this);

	InitStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! X�ю�M(���n)

-# REJECT�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabXrecvOldView::OnBnClickedButtonReject()
{
	ControlParamExecute(0);
}


/*============================================================================*/
/*! X�ю�M(���n)

-# PLL BW�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabXrecvOldView::OnBnClickedButtonPllbw()
{
	ControlParamExecute(1);
}


/*============================================================================*/
/*! X�ю�M(���n)

-# AGC BW�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabXrecvOldView::OnBnClickedButtonAgcbw()
{
	ControlParamExecute(2);
}


/*============================================================================*/
/*! X�ю�M(���n)

-# BITRATE�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabXrecvOldView::OnBnClickedButtonBitrate()
{
	ControlParamExecute(3);
}


/*============================================================================*/
/*! X�ю�M(���n)

-# D/C CTRL�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabXrecvOldView::OnBnClickedButtonDcctrl()
{
	ControlParamExecute(4);
}


/*============================================================================*/
/*! X�ю�M(���n)

-# RX�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabXrecvOldView::OnBnClickedButtonRx()
{
	ControlParamExecute(5);
}


/*============================================================================*/
/*! X�ю�M(���n)

-# D/C�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabXrecvOldView::OnBnClickedButtonDc()
{
	ControlParamExecute(6);
}

/*============================================================================*/
/*! X�ю�M(���n)

-# ����p�����^�̎��s

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabXrecvOldView::ControlParamExecute(UINT pos)
{
	CString mControlString = mXrecvOldParam[pos].nameS;
	CString str;
	GetDlgItem(mXrecvOldParam[pos].id)->GetWindowText(str);
	stControlValue val;
	val.ret = 0;
	val.type = mXrecvOldParam[pos].type;
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
		theApp.SendControlNoUIChkDlg(mControlString, _T(","), _T(""), _T(""));
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
			// �e�[�u���t�@�C�����R�s�[����V�F���݂̂𑗂�
			theApp.SendControlDlgCURRENT_LBW(mControlString, _T(""), _T(""), _T(""));
			return;
		}
		else{
			// ����ƃe�[�u���t�@�C�����R�s�[����V�F���𑗂�
			theApp.SendControlDlgPLL_BW(mControlString, str, _T(""), _T(""));
			return;
		}
	}

	theApp.SendControlChkDlg(mControlString, str, _T(""), _T(""));
}


/*============================================================================*/
/*! X�ю�M(���n)

-# �ߑ��󋵃X�e�[�^�X�̍X�V

@param
@retval

*/
/*============================================================================*/
void CTabXrecvOldView::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

	//	if (sd.stn_tbl_access() == 1)
	//	{
	//		return;
	//	}

	int szControl = sizeof(mXrecvOldParam) / sizeof(mXrecvOldParam[0]);

	for (int i = 0; i < szControl; i++)
	{
		if (mXrecvOldParam[i].id == IDC_BUTTON_REJECT)
			continue;

		CString mControlString = mXrecvOldParam[i].nameK;
		CString str;
		GetDlgItem(mXrecvOldParam[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mXrecvOldParam[i].type;

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
			if (mXrecvOldParam[i].id == IDC_BUTTON_BITRATE)
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

		if (mXrecvOldParam[i].id == IDC_STATIC_CARRVAL)
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

		if (mXrecvOldParam[i].id == IDC_STATIC_SUBCARRVAL)
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

		if (mXrecvOldParam[i].id == IDC_STATIC_VITERBIVAL)
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

		if (mXrecvOldParam[i].id == IDC_STATIC_BITVAL)
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

		if (mXrecvOldParam[i].id == IDC_STATIC_FRAMEVAL)
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

		if (mXrecvOldParam[i].id == IDC_STATIC_CARRVAL ||
			mXrecvOldParam[i].id == IDC_STATIC_SUBCARRVAL ||
			mXrecvOldParam[i].id == IDC_STATIC_VITERBIVAL ||
			mXrecvOldParam[i].id == IDC_STATIC_BITVAL ||
			mXrecvOldParam[i].id == IDC_STATIC_FRAMEVAL ||
			mXrecvOldParam[i].id == IDC_MFCMASKEDEDIT_AGC)
			((CColorStatic*)GetDlgItem(mXrecvOldParam[i].id))->SetText(str);
		else
			GetDlgItem(mXrecvOldParam[i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! X�ю�M(���n)

-# �ߑ��󋵃X�e�[�^�X�̏�����

@param
@retval

*/
/*============================================================================*/
void CTabXrecvOldView::InitStatus()
{
	int szControl = sizeof(mXrecvOldParam) / sizeof(mXrecvOldParam[0]);

	for (int i = 0; i < szControl; i++)
	{
		if (mXrecvOldParam[i].id == IDC_BUTTON_REJECT)
			continue;

		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mXrecvOldParam[i].type;

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

		if (mXrecvOldParam[i].id == IDC_STATIC_CARRVAL)
		{
			mCarrVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXrecvOldParam[i].id == IDC_STATIC_SUBCARRVAL)
		{
			mSubCarrVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXrecvOldParam[i].id == IDC_STATIC_VITERBIVAL)
		{
			mViterbiVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXrecvOldParam[i].id == IDC_STATIC_BITVAL)
		{
			mBitVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXrecvOldParam[i].id == IDC_STATIC_FRAMEVAL)
		{
			mFrameVal.SetBackColor(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		}

		if (mXrecvOldParam[i].id == IDC_STATIC_CARRVAL ||
			mXrecvOldParam[i].id == IDC_STATIC_SUBCARRVAL ||
			mXrecvOldParam[i].id == IDC_STATIC_VITERBIVAL ||
			mXrecvOldParam[i].id == IDC_STATIC_BITVAL ||
			mXrecvOldParam[i].id == IDC_STATIC_FRAMEVAL ||
			mXrecvOldParam[i].id == IDC_MFCMASKEDEDIT_AGC){
			if (str.IsEmpty())
				str.Format(_T(" "));
			((CColorStatic*)GetDlgItem(mXrecvOldParam[i].id))->SetText(str);
		}
		else
			GetDlgItem(mXrecvOldParam[i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! X�ю�M(���n)

-# �ߑ��󋵃X�e�[�^�X�̍X�V

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
LRESULT CTabXrecvOldView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
