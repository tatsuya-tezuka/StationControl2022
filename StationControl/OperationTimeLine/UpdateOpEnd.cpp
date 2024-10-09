// UpdateOpEnd.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "UpdateOpEnd.h"
#include "afxdialogex.h"


// CUpdateOpEnd �_�C�A���O

IMPLEMENT_DYNAMIC(CUpdateOpEnd, CDialogEx)

CUpdateOpEnd::CUpdateOpEnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpdateOpEnd::IDD, pParent)
	, m_strTime(_T(""))
{

}

CUpdateOpEnd::~CUpdateOpEnd()
{
}

void CUpdateOpEnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER_SDATE, m_Date);
	DDX_Control(pDX, IDC_DATETIMEPICKER_STIME, m_Time);
	DDX_Text(pDX, IDC_STATIC_BTIME, m_strTime);
}


BEGIN_MESSAGE_MAP(CUpdateOpEnd, CDialogEx)
END_MESSAGE_MAP()


// CUpdateOpEnd ���b�Z�[�W �n���h���[


BOOL CUpdateOpEnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_strTime = mDefTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	m_Date.SetTime(&mUpdateTime);
	m_Time.SetTime(&mUpdateTime);

	m_Date.SetFormat(_T("yyyy-MM-dd"));
	m_Time.SetFormat(_T("HH:mm:ss"));

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CUpdateOpEnd::OnOK()
{
	SYSTEMTIME date, time;
	m_Date.GetTime(&date);
	m_Time.GetTime(&time);
	mUpdateTime = CTime(date.wYear, date.wMonth, date.wDay, time.wHour, time.wMinute, time.wSecond);

	CDialogEx::OnOK();
}
