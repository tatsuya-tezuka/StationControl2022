/*============================================================================*/
/*! SelectLNADlg.cpp

-# AGC�Z���e�[�u��LNA�ؑ�:LNA�I��
*/
/*============================================================================*/
// SelectLNADlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "SelectLNADlg.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"
#include "HookMsgBox.h"


// CSelectLNADlg �_�C�A���O

IMPLEMENT_DYNAMIC(CSelectLNADlg, CDialogBase)

/*============================================================================*/
/*! CSelectLNADlg

-# �R���X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CSelectLNADlg::CSelectLNADlg(CWnd* pParent /*=NULL*/)
: CDialogBase(CSelectLNADlg::IDD, pParent)
, m_nSelLNA(eLNA1)
{

}

/*============================================================================*/
/*! CSelectLNADlg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSelectLNADlg::~CSelectLNADlg()
{
}

void CSelectLNADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSelectLNADlg, CDialogBase)
	ON_BN_CLICKED(IDC_SELECT_LNA1, &CSelectLNADlg::OnBnClickedSelectLna1)
	ON_BN_CLICKED(IDC_SELECT_LNA2, &CSelectLNADlg::OnBnClickedSelectLna2)
END_MESSAGE_MAP()


// CSelectLNADlg ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CSelectLNADlg

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CSelectLNADlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// �I������Ă���LNA�Ƀ`�F�b�N������
	if (m_nSelLNA == eLNA1)
	{
		((CButton*)GetDlgItem(IDC_SELECT_LNA1))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_SELECT_LNA2))->SetCheck(BST_UNCHECKED);
	}
	else if (m_nSelLNA == eLNA2)
	{
		((CButton*)GetDlgItem(IDC_SELECT_LNA1))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_SELECT_LNA2))->SetCheck(BST_CHECKED);
	}

	// ���s�{�^����񊈐��ɂ��Ă���
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	// �Ď����[�h���͑I���{�^����񊈐��ɂ���
	if (theApp.GetExecuteMode() != eExecuteMode_Control)
	{
		((CButton*)GetDlgItem(IDC_SELECT_LNA1))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_SELECT_LNA2))->EnableWindow(FALSE);
	}

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CSelectLNADlg

-# ���s�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CSelectLNADlg::OnOK()
{
	UpdateData(TRUE);

	CString str = _T("");
	if (((CButton*)GetDlgItem(IDC_SELECT_LNA1))->GetCheck() == BST_CHECKED)
	{
		str = _T("LNA1");
	}
	else if (((CButton*)GetDlgItem(IDC_SELECT_LNA2))->GetCheck() == BST_CHECKED)
	{
		str = _T("LNA2");
	}

	if (str.IsEmpty())
	{
		return;
	}

	CString strMsg;
//	strMsg.Format(_T("%s�ɐ؂�ւ����s���܂����H"), str);
//	if (IDCANCEL == AfxMessageBoxHooked(strMsg, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
//	{
//		return;
//	}

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = AGCTBL_LNASEL_SH;

	if (theApp.GetSelectStation() == eStation_Usuda64)
		stParam.strRemShellParam.Format(_T("udsc64 %s"), (LPCTSTR)str);
	else if (theApp.GetSelectStation() == eStation_Usuda642)
		stParam.strRemShellParam.Format(_T("udsc54 %s"), (LPCTSTR)str);
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
		stParam.strRemShellParam.Format(_T("usc34 %s"), (LPCTSTR)str);
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
		stParam.strRemShellParam.Format(_T("usc20 %s"), (LPCTSTR)str);

	int err = CSshShell::ExecShell(stParam);

	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CSelectLNADlg"), _T("OnOK"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err == 1)
	{
		// �ؑ֐���
		strMsg.Format(_T("%s�ɐ؂�ւ��܂����B"), str);
		AfxMessageBox(strMsg, MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CSelectLNADlg"), _T("OnOK"), _T("Error"), strDebug, nLogEx::error);

		AfxMessageBox(_T("LNA�ؑւɎ��s���܂����B"), MB_OK | MB_ICONWARNING);
	}

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! CSelectLNADlg

-# LNA1���W�I�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CSelectLNADlg::OnBnClickedSelectLna1()
{
	UpdateData(TRUE);
	EnableExecButton();
}


/*============================================================================*/
/*! CSelectLNADlg

-# LNA2���W�I�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CSelectLNADlg::OnBnClickedSelectLna2()
{
	UpdateData(TRUE);
	EnableExecButton();
}


/*============================================================================*/
/*! CSelectLNADlg

-# ���s�{�^�������E�񊈐�����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CSelectLNADlg::EnableExecButton()
{
	// ���łɑI������Ă���LNA�̏ꍇ�́A���s�{�^���͔񊈐��ɂ���
	if (m_nSelLNA == eLNA1)
	{
		if (((CButton*)GetDlgItem(IDC_SELECT_LNA1))->GetCheck() == BST_CHECKED)
		{
			GetDlgItem(IDOK)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDOK)->EnableWindow(TRUE);
		}
	}
	else if (m_nSelLNA == eLNA2)
	{
		if (((CButton*)GetDlgItem(IDC_SELECT_LNA2))->GetCheck() == BST_CHECKED)
		{
			GetDlgItem(IDOK)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDOK)->EnableWindow(TRUE);
		}
	}
}
