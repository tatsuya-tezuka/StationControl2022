// ControlManagementInputPassword.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "ControlManagement.h"
#include "ControlManagementInputPassword.h"
#include "afxdialogex.h"


// CControlManagementInputPassword �_�C�A���O

IMPLEMENT_DYNAMIC(CControlManagementInputPassword, CDialogEx)

CControlManagementInputPassword::CControlManagementInputPassword(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlManagementInputPassword::IDD, pParent)
	, mInputPassword(_T(""))
{

}

CControlManagementInputPassword::~CControlManagementInputPassword()
{
}

void CControlManagementInputPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, mInputPassword);
}


BEGIN_MESSAGE_MAP(CControlManagementInputPassword, CDialogEx)
	ON_BN_CLICKED(IDOK, &CControlManagementInputPassword::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CControlManagementInputPassword::OnBnClickedCancel)
END_MESSAGE_MAP()


// CControlManagementInputPassword ���b�Z�[�W �n���h���[


BOOL CControlManagementInputPassword::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CFileAccess& ac = theApp.GetFileAccess();
	if (ac.ReadManagementPassword(mManPassword) == false){
		// �p�X���[�h���ݒ肳��Ă��Ȃ��̂ŁA�f�t�H���g�p�X���[�h��ݒ肷��
		mManPassword = mDefaultPassword;
		mManPassword = theApp.ManEncodeDecode(mManPassword);
		ac.WriteManagementPassword(mManPassword);
	}
	mManPassword = theApp.ManEncodeDecode(mManPassword);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CControlManagementInputPassword::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (mInputPassword.GetLength() != mMaxPassword){
		MessageBox(_T("���͂��ꂽ�p�X���[�h��8�����ł͂���܂���B\n�ēx���͂��Ă��������B"), MB_OK);
		return;
	}
	if (mInputPassword != mManPassword){
		MessageBox(_T("���͂��ꂽ�p�X���[�h�Ɍ�肪����܂��B\n�ēx���͂��Ă��������B"), MB_OK);
		return;
	}

	CDialogEx::OnOK();
}


void CControlManagementInputPassword::OnBnClickedCancel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDialogEx::OnCancel();
}
