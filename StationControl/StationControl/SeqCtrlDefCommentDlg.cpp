/*============================================================================*/
/*! SeqCtrlDefCommentDlg.cpp

-# ���������`�t�@�C���ۑ��R�����g���͉��
*/
/*============================================================================*/
// SeqCtrlDefCommentDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "SeqCtrlDefCommentDlg.h"
#include "afxdialogex.h"


// CSeqCtrlDefCommentDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CSeqCtrlDefCommentDlg, CDialogBase)

/*============================================================================*/
/*! CSeqCtrlDefCommentDlg

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CSeqCtrlDefCommentDlg::CSeqCtrlDefCommentDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSeqCtrlDefCommentDlg::IDD, pParent)
{

}

/*============================================================================*/
/*! CSeqCtrlDefCommentDlg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSeqCtrlDefCommentDlg::~CSeqCtrlDefCommentDlg()
{
}

void CSeqCtrlDefCommentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEQCTRLDEFCOMMENT, m_strComment);
}


BEGIN_MESSAGE_MAP(CSeqCtrlDefCommentDlg, CDialogBase)
	ON_BN_CLICKED(IDOK, &CSeqCtrlDefCommentDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSeqCtrlDefCommentDlg ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CSeqCtrlDefCommentDlg

-# ���s���ɌĂяo�����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CSeqCtrlDefCommentDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	CDialogBase::OnOK();
}
