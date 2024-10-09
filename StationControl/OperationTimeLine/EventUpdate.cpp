// EventUpdate.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "EventUpdate.h"
#include "afxdialogex.h"


// CEventUpdate ダイアログ

IMPLEMENT_DYNAMIC(CEventUpdate, CDialogEx)

CEventUpdate::CEventUpdate(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEventUpdate::IDD, pParent)
	, m_strText(_T(""))
	, m_strStart(_T(""))
	, m_strEnd(_T(""))
{

}

CEventUpdate::~CEventUpdate()
{
}

void CEventUpdate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TEXT, m_strText);
	DDX_Text(pDX, IDC_STATIC_BSTART, m_strStart);
	DDX_Text(pDX, IDC_STATIC_BEND, m_strEnd);
	DDX_Control(pDX, IDC_DATETIMEPICKER_SDATE, m_StartDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_STIME, m_StartTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_EDATE, m_EndDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_ETIME, m_EndTime);
}


BEGIN_MESSAGE_MAP(CEventUpdate, CDialogEx)
END_MESSAGE_MAP()


// CEventUpdate メッセージ ハンドラー


BOOL CEventUpdate::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_strText = mEventUpdateText[mMode];
	m_strStart = mDefStartTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	m_strEnd = mDefEndTime.Format(_T("%Y-%m-%d %H:%M:%S"));

	m_StartDate.SetFormat(_T("yyyy-MM-dd"));
	m_StartTime.SetFormat(_T("HH:mm:ss"));
	m_EndDate.SetFormat(_T("yyyy-MM-dd"));;
	m_EndTime.SetFormat(_T("HH:mm:ss"));

	m_StartDate.SetTime(&mStartTime);
	m_StartTime.SetTime(&mStartTime);
	m_EndDate.SetTime(&mEndTime);
	m_EndTime.SetTime(&mEndTime);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CEventUpdate::OnOK()
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
		AfxMessageBox(_T("終了日時が開始日時より過去です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	mStartTime = startTime;
	mEndTime = endTime;

	CDialogEx::OnOK();
}
