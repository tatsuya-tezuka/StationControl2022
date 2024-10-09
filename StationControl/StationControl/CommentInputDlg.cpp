/*============================================================================*/
/*! CommentInputDlg.cpp

-# ��������o�^�E���s		�w��b�ҋ@�ݒ�
*/
/*============================================================================*/
// CommentInputDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "CommentInputDlg.h"
#include "afxdialogex.h"


// CCommentInputDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CCommentInputDlg, CDialogBase)

/*============================================================================*/
/*! CCommentInputDlg

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CCommentInputDlg::CCommentInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CCommentInputDlg::IDD, pParent)
{

}

/*============================================================================*/
/*! CCommentInputDlg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CCommentInputDlg::~CCommentInputDlg()
{
}

void CCommentInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEQCTRL_COMMENT, m_str);
	DDX_Control(pDX, IDC_EDIT_SEQCTRL_COMMENT, m_cedit);
}


BEGIN_MESSAGE_MAP(CCommentInputDlg, CDialogBase)
	ON_BN_CLICKED(IDOK, &CCommentInputDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCommentInputDlg ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CCommentInputDlg

-# ���s�{�^���������ɌĂяo�����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCommentInputDlg::OnBnClickedOk()
{
	UpdateData();

	if (m_str.IsEmpty())
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("�R�����g����͂��Ă�������"), title, MB_OK);
		return;
	}

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! CCommentInputDlg

-# �ݒ蕶������擾����

@param  �Ȃ�
@retval �ݒ蕶����
*/
/*============================================================================*/
CString CCommentInputDlg::GetString()
{
	return _T("#,") + m_str;
}


/*============================================================================*/
/*! CCommentInputDlg

-# ���������ɌĂ΂��

@param  �Ȃ�
@retval TRUE:�����@/ FALSE:���s
*/
/*============================================================================*/
BOOL CCommentInputDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_cedit.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
