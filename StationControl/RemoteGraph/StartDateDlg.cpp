// StartDateDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RemoteGraph.h"
#include "StartDateDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CStartDateDlg ダイアログ

IMPLEMENT_DYNAMIC(CStartDateDlg, CDialogEx)

CStartDateDlg::CStartDateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStartDateDlg::IDD, pParent)
{
}

CStartDateDlg::~CStartDateDlg()
{
}

void CStartDateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DateTimePicker1);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_DateTimePicker2);
}


BEGIN_MESSAGE_MAP(CStartDateDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CStartDateDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CStartDateDlg メッセージ ハンドラー


BOOL CStartDateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CTime now, before;
	CTimeSpan span;

	now = CTime::GetCurrentTime();
	span = CTimeSpan(180, 0, 0, 0);
	before = now - span;

	m_DateTimePicker1.SetRange(&before, &now);
	m_DateTimePicker2.SetRange(&before, &now);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CStartDateDlg::OnBnClickedOk()
{
	UpdateData();

	CTime time1, time2;
	m_DateTimePicker1.GetTime(time1);
	m_DateTimePicker2.GetTime(time2);
	CString startday = time1.Format(_T("%Y/%m/%d"));
	CString starttime = time2.Format(_T("%H:%M:%S"));

	startday += _T(" ") + starttime;

	((CMainFrame*)(theApp.m_pMainWnd))->GetPropertiesWnd().SetStartDate(startday);

	CDialogEx::OnOK();
}
