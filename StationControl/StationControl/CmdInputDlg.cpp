/*============================================================================*/
/*! CmdInputDlg.cpp

-# ��������o�^�E���s		�R�}���h�ݒ�
*/
/*============================================================================*/
// CmdInputDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "CmdInputDlg.h"
#include "afxdialogex.h"


// CCmdInputDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CCmdInputDlg, CDialogBase)

/*============================================================================*/
/*! CmdInputDlg

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CCmdInputDlg::CCmdInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CCmdInputDlg::IDD, pParent)
{

}

/*============================================================================*/
/*! CCmdInputDlg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CCmdInputDlg::~CCmdInputDlg()
{
}

void CCmdInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEQCTRL_CMD, m_str);
	DDX_Control(pDX, IDC_EDIT_SEQCTRL_CMD, m_cedit);
}


BEGIN_MESSAGE_MAP(CCmdInputDlg, CDialogBase)
	ON_BN_CLICKED(IDOK, &CCmdInputDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCmdInputDlg ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CCmdInputDlg

-# ���s�{�^���������ɌĂяo�����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCmdInputDlg::OnBnClickedOk()
{
	UpdateData();

	if (m_str.IsEmpty())
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("�R�}���h����͂��Ă�������"), title, MB_OK);
		return;
	}

	CDialogBase::OnOK();
}

/*============================================================================*/
/*! CCmdInputDlg

-# �ݒ蕶������擾����

@param  �Ȃ�
@retval �ݒ蕶����
*/
/*============================================================================*/
CString CCmdInputDlg::GetString()
{
	return _T("command,") + m_str;
}


/*============================================================================*/
/*! CCmdInputDlg

-# ���������ɌĂ΂��

@param  �Ȃ�
@retval TRUE:�����@/ FALSE:���s
*/
/*============================================================================*/
BOOL CCmdInputDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_cedit.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
