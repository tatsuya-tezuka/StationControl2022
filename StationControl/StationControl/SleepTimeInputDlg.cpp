/*============================================================================*/
/*! SleepTimeInputDlg.cpp

-# ��������o�^�E���s		�w��b�ҋ@�ݒ�
*/
/*============================================================================*/
// SleepTimeInputDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "SleepTimeInputDlg.h"
#include "afxdialogex.h"


// CSleepTimeInputDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CSleepTimeInputDlg, CDialogBase)

/*============================================================================*/
/*! SleepTimeInputDlg

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CSleepTimeInputDlg::CSleepTimeInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSleepTimeInputDlg::IDD, pParent)
{

}

/*============================================================================*/
/*! SleepTimeInputDlg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSleepTimeInputDlg::~CSleepTimeInputDlg()
{
}

void CSleepTimeInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEQCTRL_SLEEPTIME, m_str);
	DDX_Control(pDX, IDC_EDIT_SEQCTRL_SLEEPTIME, m_cedit);
}


BEGIN_MESSAGE_MAP(CSleepTimeInputDlg, CDialogBase)
	ON_BN_CLICKED(IDOK, &CSleepTimeInputDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSleepTimeInputDlg ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! SleepTimeInputDlg

-# ���s�{�^���������ɌĂяo�����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CSleepTimeInputDlg::OnBnClickedOk()
{
	UpdateData();

	if (m_str.IsEmpty())
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("Sleep���Ԃ���͂��Ă�������"), title, MB_OK);
		return;
	}

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! SleepTimeInputDlg

-# �ݒ蕶������擾����

@param  �Ȃ�
@retval �ݒ蕶����
*/
/*============================================================================*/
CString CSleepTimeInputDlg::GetString()
{
	return _T("sleep,") + m_str;
}


/*============================================================================*/
/*! SleepTimeInputDlg

-# ���������ɌĂ΂��

@param  �Ȃ�
@retval TRUE:�����@/ FALSE:���s
*/
/*============================================================================*/
BOOL CSleepTimeInputDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_cedit.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
