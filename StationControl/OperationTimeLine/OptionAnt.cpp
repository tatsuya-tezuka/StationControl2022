// OptionAnt.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "OptionAnt.h"
#include "afxdialogex.h"


// COptionAnt �_�C�A���O

IMPLEMENT_DYNAMIC(COptionAnt, CDialogEx)

COptionAnt::COptionAnt(CWnd* pParent /*=NULL*/)
	: CDialogEx(COptionAnt::IDD, pParent)
	, m_nAntMode(0)
{

}

COptionAnt::~COptionAnt()
{
}

void COptionAnt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nAntMode);
}


BEGIN_MESSAGE_MAP(COptionAnt, CDialogEx)
END_MESSAGE_MAP()


// COptionAnt ���b�Z�[�W �n���h���[


BOOL COptionAnt::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void COptionAnt::OnOK()
{
	UpdateData(TRUE);

	CDialogEx::OnOK();
}
