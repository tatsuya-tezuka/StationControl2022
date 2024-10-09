/*============================================================================*/
/*! TblSatelliteSelection.cpp

-# �q���I����ʋ���

-#  AGC�Z���e�[�u���Ǎ��E�q���I�����
-#  AGC�Z���e�[�u���t�@�C���R�s�[�E�q���I�����
-#  AGC�Z���e�[�u���t�@�C�������E�q���I�����
-#  �Ǔ��f�B���C�l�e�[�u���t�@�C���R�s�[�E�q���I�����
-#  �Ǔ��f�B���C�l�e�[�u���t�@�C�������E�q���I�����
*/
/*============================================================================*/
// TblSatelliteSelection.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "TblSatelliteSelection.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"


// CTblSatelliteSelection �_�C�A���O

IMPLEMENT_DYNAMIC(CTblSatelliteSelection, CDialogEx)

CTblSatelliteSelection::CTblSatelliteSelection(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTblSatelliteSelection::IDD, pParent)
{
}

CTblSatelliteSelection::~CTblSatelliteSelection()
{
}

void CTblSatelliteSelection::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SATELLITE_CMB, m_SatCmb);
}


BEGIN_MESSAGE_MAP(CTblSatelliteSelection, CDialogEx)
	ON_BN_CLICKED(IDC_TBL_SATSEL_BTN, &CTblSatelliteSelection::OnBnClickedTblSatselBtn)
END_MESSAGE_MAP()


// CTblSatelliteSelection ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CTblSatelliteSelection

-# �_�C�A���O����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CTblSatelliteSelection::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �q���I��
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	vector<CString>			&saNameList = dba.GetSatNameList();
	int cnt = (int)saNameList.size();
	for (int i = 0; i < cnt; i++)
	{
		/*--------------------------*/
		/* �q���{�̏��A�h���X�擾 */
		CString satname = saNameList[i];

		if (satname == _T("DEFAULT") || satname == _T("�c�d�e�`�t�k�s"))
		{
			continue;
		}
		m_SatCmb.InsertString(-1, satname);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! CTblSatelliteSelection

-# �w���s�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTblSatelliteSelection::OnBnClickedTblSatselBtn()
{
	CString sat;
	int idx = m_SatCmb.GetCurSel();

	if (idx == CB_ERR)
	{
		return;
	}

	m_SatCmb.GetLBText(idx, sat);

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = OPTION_AGCTBL_LOAD_SH;

	if (theApp.GetSelectStation() == eStation_Usuda64)
		stParam.strRemShellParam.Format(_T("udsc64 %s"), (LPCTSTR)sat);
	else if (theApp.GetSelectStation() == eStation_Usuda642)
		stParam.strRemShellParam.Format(_T("udsc54 %s"), (LPCTSTR)sat);
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
		stParam.strRemShellParam.Format(_T("usc34 %s"), (LPCTSTR)sat);
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
		stParam.strRemShellParam.Format(_T("usc20 %s"), (LPCTSTR)sat);

	CString msg;
	msg.Format(_T("GetCalib(%s)!!!"), (LPCTSTR)stParam.strRemShellName);
	CLogTraceEx::Write(_T(""), _T("CTblSatelliteSelection"), _T("GetCalib"), msg, _T(""), nLogEx::debug);

	int err = CSshShell::ExecShell(stParam);

	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CTblSatelliteSelection"), _T("GetCalib"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err == 0)
	{
		AfxMessageBox(_T("AGC�Z���e�[�u���t�@�C���̓ǂݍ��݂ɐ������܂����B"), MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		AfxMessageBox(_T("AGC�Z���e�[�u���t�@�C���̓ǂݍ��݂Ɏ��s���܂����B"), MB_OK | MB_ICONWARNING);
	}

	CDialogEx::OnOK();
}
