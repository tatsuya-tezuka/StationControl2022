/*============================================================================*/
/*! RdnsAgcDetail.cpp

-# �v�惌�f�B�l�X�`�F�b�N-AGC�Z���ڍאݒ���
*/
/*============================================================================*/
// RdnsAgcDetail.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "RdnsAgcDetail.h"
#include "afxdialogex.h"


// CRdnsAgcDetail �_�C�A���O

IMPLEMENT_DYNAMIC(CRdnsAgcDetail, CDialogBase)

/*============================================================================*/
/*! CRdnsAgcDetail

-# �R���X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CRdnsAgcDetail::CRdnsAgcDetail(CWnd* pParent /*=NULL*/)
: CDialogBase(CRdnsAgcDetail::IDD, pParent)
, m_nLoopBand(0)
{

}

/*============================================================================*/
/*! CRdnsAgcDetail

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CRdnsAgcDetail::~CRdnsAgcDetail()
{
}

void CRdnsAgcDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RDNS_AGC_1KHZ, m_nLoopBand);
}


BEGIN_MESSAGE_MAP(CRdnsAgcDetail, CDialogBase)
	ON_BN_CLICKED(IDOK, &CRdnsAgcDetail::OnBnClickedOk)
END_MESSAGE_MAP()


// CRdnsAgcDetail ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CRdnsAgcDetail

-# ���[�v�o���h�ݒ�

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsAgcDetail::SetLoopBand(CString strLB)
{
	for (int i = 0; strAGCLoopBand[i] != _T(""); ++i)
	{
		if (strLB == strAGCLoopBand[i])
		{
			m_nLoopBand = i;
			return;
		}
	}
}


/*============================================================================*/
/*! CRdnsAgcDetail

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CRdnsAgcDetail::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CRdnsAgcDetail

-# �ݒ�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsAgcDetail::OnBnClickedOk()
{
	OnOK();
}
