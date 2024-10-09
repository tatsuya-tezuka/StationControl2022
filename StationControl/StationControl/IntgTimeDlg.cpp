/*============================================================================*/
/*! IntegTimeDlg.cpp

-# �ϕ����Ԑݒ���
*/
/*============================================================================*/
// IntegTimeDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "IntgTimeDlg.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"


// CIntgTimeDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CIntgTimeDlg, CDialogBase)

/*============================================================================*/
/*! CIntgTimeDlg

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CIntgTimeDlg::CIntgTimeDlg(CWnd* pParent /*=NULL*/)
: CDialogBase(CIntgTimeDlg::IDD, pParent)
	, m_nIntgTime(1)
{

}

/*============================================================================*/
/*! CIntgTimeDlg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CIntgTimeDlg::~CIntgTimeDlg()
{
}

void CIntgTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INTGTIME_EDIT, m_nIntgTime);
	DDV_MinMaxInt(pDX, m_nIntgTime, 1, 300);
	DDX_Control(pDX, IDC_INTGTIME_EDIT, m_cedit);
}


BEGIN_MESSAGE_MAP(CIntgTimeDlg, CDialogBase)
	ON_BN_CLICKED(IDOK, &CIntgTimeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CIntgTimeDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CIntgTimeDlg ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CIntgTimeDlg

-# ���s���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CIntgTimeDlg::OnBnClickedOk()
{
	// �ϕ����Ԑݒ�������o�ϐ��ɔ��f����
	if (UpdateData(TRUE))
	{
		// �����[�g�V�F���ŋǉ^�p�ǐ��T�[�o�ɒʒm
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = SEND_NOTICE_SH;
		CString arg;
		int nStation = theApp.GetSelectStation();
		UINT nTermNo = theApp.GetTermNo();
		if (nTermNo > 0)
			nTermNo = nTermNo - 1;

//		arg.Format(_T("%s %d %s%02u,%d,;"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_INTEG, NOTIFY_INTEG_MSG, nTermNo, m_nIntgTime);
		int nAreaNo = theApp.GetClientTerminal();
		arg.Format(_T("%s %d %s%s%02u,%d,;"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_INTEG, NOTIFY_INTEG_MSG, mAreaNameString[nAreaNo], nTermNo, m_nIntgTime);

		stParam.strRemShellParam.Format(arg);

		int err = CSshShell::ExecShell(stParam, FALSE);		// �V�F���҂����Ȃ�
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CIntgTimeDlg"), _T("OnBnClickedOk"), _T("debug"), strDebug, nLogEx::debug);
		}
		if (err == 0)
		{
			// �������Ȃ�
		}

		CDialogBase::OnOK();
	}
}


/*============================================================================*/
/*! CIntgTimeDlg

-# �L�����Z�����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CIntgTimeDlg::OnBnClickedCancel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDialogBase::OnCancel();
}


/*============================================================================*/
/*! CIntgTimeDlg

-# ���������ɌĂ΂��

@param  �Ȃ�
@retval TRUE:�����@/ FALSE:���s
*/
/*============================================================================*/
BOOL CIntgTimeDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_cedit.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
