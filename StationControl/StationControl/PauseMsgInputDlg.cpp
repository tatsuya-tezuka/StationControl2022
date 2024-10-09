/*============================================================================*/
/*! PauseMsgInputDlg.cpp

-# ��������o�^�E���s		�ꎞ��~�ݒ�
*/
/*============================================================================*/
// PauseMsgInputDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "PauseMsgInputDlg.h"
#include "afxdialogex.h"


// CPauseMsgInputDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CPauseMsgInputDlg, CDialogBase)

/*============================================================================*/
/*! CPauseMsgInputDlg

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CPauseMsgInputDlg::CPauseMsgInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CPauseMsgInputDlg::IDD, pParent)
{

}

/*============================================================================*/
/*! CPauseMsgInputDlg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CPauseMsgInputDlg::~CPauseMsgInputDlg()
{
}

void CPauseMsgInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEQCTRL_PAUSEMSG, m_str);
	DDX_Control(pDX, IDC_EDIT_SEQCTRL_PAUSEMSG, m_cedit);
}


BEGIN_MESSAGE_MAP(CPauseMsgInputDlg, CDialogBase)
	ON_BN_CLICKED(IDOK, &CPauseMsgInputDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPauseMsgInputDlg ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CPauseMsgInputDlg

-# ���s�{�^���������ɌĂяo�����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CPauseMsgInputDlg::OnBnClickedOk()
{
	UpdateData();

	if (m_str.IsEmpty())
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("�������b�Z�[�W����͂��Ă�������"), title, MB_OK);
		return;
	}

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! CPauseMsgInputDlg

-# �ݒ蕶������擾����

@param  �Ȃ�
@retval �ݒ蕶����
*/
/*============================================================================*/
CString CPauseMsgInputDlg::GetString()
{
	return _T("pause,") + m_str;
}


/*============================================================================*/
/*! CPauseMsgInputDlg

-# ���������ɌĂ΂��

@param  �Ȃ�
@retval TRUE:�����@/ FALSE:���s
*/
/*============================================================================*/
BOOL CPauseMsgInputDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_cedit.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
