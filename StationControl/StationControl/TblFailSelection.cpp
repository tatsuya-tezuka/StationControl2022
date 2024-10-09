/*============================================================================*/
/*! TblFailSelection.cpp

-# �t�@�C���I����ʋ���

-#  AGC�Z���e�[�u���t�@�C���R�s�[�E�t�@�C���I�����
-#  AGC�Z���e�[�u���t�@�C�������E�t�@�C���I�����
-#  �Ǔ��f�B���C�l�e�[�u���t�@�C���R�s�[�E�t�@�C���I�����
-#  �Ǔ��f�B���C�l�e�[�u���t�@�C�������E�t�@�C���I�����
*/
/*============================================================================*/
// TblFailSelection.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "TblFailSelection.h"
#include "afxdialogex.h"


// CTblFailSelection �_�C�A���O

IMPLEMENT_DYNAMIC(CTblFailSelection, CDialogEx)

CTblFailSelection::CTblFailSelection(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTblFailSelection::IDD, pParent)
	, m_strFile(_T(""))
{
}

CTblFailSelection::~CTblFailSelection()
{
}

void CTblFailSelection::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_FileList);
	DDX_Control(pDX, IDC_EDIT1, m_FileEdit);
	DDX_Text(pDX, IDC_EDIT1, m_strFile);
}


BEGIN_MESSAGE_MAP(CTblFailSelection, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTblFailSelection::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTblFailSelection ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CTblFailSelection

-# �_�C�A���O����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CTblFailSelection::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CTblFailSelection

-# �w���s�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTblFailSelection::OnBnClickedButton1()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}
