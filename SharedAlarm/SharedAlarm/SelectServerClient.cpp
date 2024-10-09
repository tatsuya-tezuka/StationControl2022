// SelectServerClient.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "SharedAlarm.h"
#include "SelectServerClient.h"
#include "afxdialogex.h"


// CSelectServerClient �_�C�A���O

IMPLEMENT_DYNAMIC(CSelectServerClient, CDialogEx)

CSelectServerClient::CSelectServerClient(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectServerClient::IDD, pParent)
	, m_nMode(0)
{

}

CSelectServerClient::~CSelectServerClient()
{
}

void CSelectServerClient::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nMode);
}


BEGIN_MESSAGE_MAP(CSelectServerClient, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelectServerClient::OnBnClickedOk)
END_MESSAGE_MAP()


// CSelectServerClient ���b�Z�[�W �n���h���[


BOOL CSelectServerClient::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CSelectServerClient::OnBnClickedOk()
{
	UpdateData(TRUE);

	CDialogEx::OnOK();
}
