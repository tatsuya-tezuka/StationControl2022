// SelectMasterSlave.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "SelectMasterSlave.h"
#include "afxdialogex.h"


// CSelectMasterSlave �_�C�A���O

IMPLEMENT_DYNAMIC(CSelectMasterSlave, CDialogEx)

CSelectMasterSlave::CSelectMasterSlave(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectMasterSlave::IDD, pParent)
	, mSelectMode(0)
{

}

CSelectMasterSlave::~CSelectMasterSlave()
{
}

void CSelectMasterSlave::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, mSelectMode);
}


BEGIN_MESSAGE_MAP(CSelectMasterSlave, CDialogEx)
END_MESSAGE_MAP()


// CSelectMasterSlave ���b�Z�[�W �n���h���[


BOOL CSelectMasterSlave::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CSelectMasterSlave::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	CDialogEx::OnOK();
}
