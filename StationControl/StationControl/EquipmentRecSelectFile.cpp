/*============================================================================*/
/*! EquipmentRecSelectFile.cpp

-# �ݔ��L�^ �_���v��`�t�@�C���I�����
*/
/*============================================================================*/
// EquipmentRecSelectFile.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "EquipmentRecSelectFile.h"
#include "afxdialogex.h"


// CEquipmentRecSelectFile �_�C�A���O

IMPLEMENT_DYNAMIC(CEquipmentRecSelectFile, CDialogBase)

CEquipmentRecSelectFile::CEquipmentRecSelectFile(CWnd* pParent /*=NULL*/)
: CDialogBase(CEquipmentRecSelectFile::IDD, pParent)
{

}

CEquipmentRecSelectFile::~CEquipmentRecSelectFile()
{
}

void CEquipmentRecSelectFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EQIPREC_SELFILE_LIST, m_FileList);
}


BEGIN_MESSAGE_MAP(CEquipmentRecSelectFile, CDialogBase)
	ON_BN_CLICKED(IDC_EQIPREC_SELFILE_READ_BTN, &CEquipmentRecSelectFile::OnBnClickedEqiprecSelfileReadBtn)
	ON_BN_CLICKED(IDC_EQIPREC_SELFILE_DEL_BTN, &CEquipmentRecSelectFile::OnBnClickedEqiprecSelfileDelBtn)
END_MESSAGE_MAP()


// CEquipmentRecSelectFile ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CEquipmentRecSelectFile

-# �_�C�A���O����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CEquipmentRecSelectFile::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CEquipmentRecSelectFile

-# �w�ǂݍ��݁x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentRecSelectFile::OnBnClickedEqiprecSelfileReadBtn()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}


/*============================================================================*/
/*! CEquipmentRecSelectFile

-# �w�폜�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentRecSelectFile::OnBnClickedEqiprecSelfileDelBtn()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}
