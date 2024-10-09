// EventAdd.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "EventAdd.h"
#include "afxdialogex.h"


// CEventAdd �_�C�A���O

IMPLEMENT_DYNAMIC(CEventAdd, CDialogEx)

CEventAdd::CEventAdd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEventAdd::IDD, pParent)
	, m_strText(_T(""))
{
	mMode = eEventAddModeUplink;
}

CEventAdd::~CEventAdd()
{
}

void CEventAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TEXT, m_strText);
	DDX_Control(pDX, IDC_DATETIMEPICKER_SDATE, m_StartDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_STIME, m_StartTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_EDATE, m_EndDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_ETIME, m_EndTime);
	DDX_Control(pDX, IDC_RNGTYPECMB, m_RngTypeCmb);
	DDX_Control(pDX, IDC_RNG_TYPE, m_RngTypeStatic);
}


BEGIN_MESSAGE_MAP(CEventAdd, CDialogEx)
END_MESSAGE_MAP()


// CEventAdd ���b�Z�[�W �n���h���[


BOOL CEventAdd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_StartDate.SetFormat(_T("yyyy-MM-dd"));
	m_StartTime.SetFormat(_T("HH:mm:ss"));
	m_EndDate.SetFormat(_T("yyyy-MM-dd"));;
	m_EndTime.SetFormat(_T("HH:mm:ss"));

	if (mMode == eEventAddModeRng)
	{
		for (int i = 0; i < (int)m_RngTypeList.size(); ++i)
		{
			m_RngTypeCmb.InsertString(-1, m_RngTypeList[i]);
		}
		m_RngTypeCmb.SetCurSel(0);
	}
	else
	{
		m_RngTypeCmb.ShowWindow(FALSE);
		m_RngTypeStatic.ShowWindow(FALSE);
	}

	m_strText = mEventAddText[mMode];
	m_StartDate.SetTime(&mStartTime);
	m_StartTime.SetTime(&mStartTime);
	m_EndDate.SetTime(&mEndTime);
	m_EndTime.SetTime(&mEndTime);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void CEventAdd::OnOK()
{
	SYSTEMTIME date, time;
	m_StartDate.GetTime(&date);
	m_StartTime.GetTime(&time);
	CTime startTime = CTime(date.wYear, date.wMonth, date.wDay, time.wHour, time.wMinute, time.wSecond);
	m_EndDate.GetTime(&date);
	m_EndTime.GetTime(&time);
	CTime endTime = CTime(date.wYear, date.wMonth, date.wDay, time.wHour, time.wMinute, time.wSecond);

	if (endTime.GetTime() < startTime.GetTime())
	{
		AfxMessageBox(_T("�I���������J�n�������ߋ��ł��B\n�������������Ă��������B"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	mStartTime = startTime;
	mEndTime = endTime;

	CString str;
	m_RngTypeCmb.GetWindowTextW(str);
	mRngType = str;

	CDialogEx::OnOK();
}
