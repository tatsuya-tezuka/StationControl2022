/*============================================================================*/
/*! TabXrecvView54.cpp

-# �ߑ��󋵃X�e�[�^�X��ʁiX�ю�M�j
*/
/*============================================================================*/
// TabXrecvView54.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabXrecvView54.h"
#include "afxdialogex.h"


// CTabXrecvView54 �_�C�A���O

IMPLEMENT_DYNCREATE(CTabXrecvView54, CDialogEx)

CTabXrecvView54::CTabXrecvView54(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabXrecvView54::IDD, pParent)
{

}

CTabXrecvView54::~CTabXrecvView54()
{
}

void CTabXrecvView54::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabXrecvView54, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DCCTRL, &CTabXrecvView54::OnBnClickedButtonDcctrl)
	ON_BN_CLICKED(IDC_BUTTON_RX, &CTabXrecvView54::OnBnClickedButtonRx)
END_MESSAGE_MAP()


// CTabXrecvView54 ���b�Z�[�W �n���h���[

BOOL CTabXrecvView54::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	theApp.SetMonitorMode(this);

	InitStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! X�ю�M

-# D/C CTRL�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabXrecvView54::OnBnClickedButtonDcctrl()
{
	ControlParamExecute(0);
}


/*============================================================================*/
/*! X�ю�M

-# RX�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabXrecvView54::OnBnClickedButtonRx()
{
	ControlParamExecute(1);
}

/*============================================================================*/
/*! X�ю�M

-# ����p�����^�̎��s

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTabXrecvView54::ControlParamExecute(UINT pos)
{
	CString mControlString = mXrecvParam54[pos].nameS;
	CString str;
	GetDlgItem(mXrecvParam54[pos].id)->GetWindowText(str);

	stControlValue val;
	val.ret = 0;
	val.type = mXrecvParam54[pos].type;
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
		str.Format(_T("%s"), (LPCTSTR)CString(val.buf));
		break;
	case	eControlTime:
		str.Format(_T("%s"), (LPCTSTR)CString(val.buf));
		break;
	}

	theApp.SendControlChkDlg(mControlString, str, _T(""), _T(""));
}


/*============================================================================*/
/*! X�ю�M

-# �ߑ��󋵃X�e�[�^�X�̍X�V

@param
@retval

*/
/*============================================================================*/
void CTabXrecvView54::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mXrecvParam54) / sizeof(mXrecvParam54[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString mControlString = mXrecvParam54[i].nameK;
		CString str;
		GetDlgItem(mXrecvParam54[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mXrecvParam54[i].type;

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

		GetDlgItem(mXrecvParam54[i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! X�ю�M

-# �ߑ��󋵃X�e�[�^�X�̏�����

@param
@retval

*/
/*============================================================================*/
void CTabXrecvView54::InitStatus()
{
	int szControl = sizeof(mXrecvParam54) / sizeof(mXrecvParam54[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mXrecvParam54[i].type;

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

		GetDlgItem(mXrecvParam54[i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! X�ю�M

-# �ߑ��󋵃X�e�[�^�X�̍X�V

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
LRESULT CTabXrecvView54::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
