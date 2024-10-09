// SelectEventMode.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "SelectEventMode.h"
#include "afxdialogex.h"
#include "../StationControl/HookMsgBox.h"

// CSelectEventMode �_�C�A���O

IMPLEMENT_DYNAMIC(CSelectEventMode, CDialogEx)

CSelectEventMode::CSelectEventMode(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectEventMode::IDD, pParent)
	, m_nEventMode(0)
{

}

CSelectEventMode::~CSelectEventMode()
{
}

void CSelectEventMode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nEventMode);
}


BEGIN_MESSAGE_MAP(CSelectEventMode, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelectEventMode::OnBnClickedOk)
END_MESSAGE_MAP()


// CSelectEventMode ���b�Z�[�W �n���h���[


BOOL CSelectEventMode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CSelectEventMode::OnBnClickedOk()
{
	UpdateData();

//	if (m_nEventMode == 1)
//	{
//		// �蓮���[�h
//		if (IDCANCEL == AfxMessageBoxHooked(_T("�o�^�ς݂̃C�x���g���폜����܂����A��낵���ł��傤���H"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
//		{
//			return;
//		}
//	}

	CDialogEx::OnOK();
}
