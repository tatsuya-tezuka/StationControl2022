// EventDel.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "EventDel.h"
#include "afxdialogex.h"


// CEventDel �_�C�A���O

IMPLEMENT_DYNAMIC(CEventDel, CDialogEx)

CEventDel::CEventDel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEventDel::IDD, pParent)
	, m_strText(_T(""))
{

}

CEventDel::~CEventDel()
{
}

void CEventDel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TEXT, m_strText);
}


BEGIN_MESSAGE_MAP(CEventDel, CDialogEx)
END_MESSAGE_MAP()


// CEventDel ���b�Z�[�W �n���h���[


BOOL CEventDel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_strText.Format(mEventDelText[mMode], mTargetNo + 1);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CEventDel::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	CDialogEx::OnOK();
}
