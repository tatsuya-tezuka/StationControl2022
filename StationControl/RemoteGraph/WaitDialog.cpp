// WaitDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "RemoteGraph.h"
#include "WaitDialog.h"
#include "afxdialogex.h"

static bool mWaitAbortFlag = false;

/////////////////////////////////////////////////////////////////////////////
// CDlgWaitThread

IMPLEMENT_DYNCREATE(CDlgWaitThread, CWinThread)

CDlgWaitThread::CDlgWaitThread()
{
	m_strTitle.Empty();
	m_strMessage.Empty();
	m_bAbort = false;
	m_pParent = NULL;
	mEvent = NULL;
	mEventName.Empty();
}

CDlgWaitThread::~CDlgWaitThread()
{
}

BOOL CDlgWaitThread::InitInstance()
{
	CWaitDialog	dlg(m_strTitle, m_strMessage, m_bAbort, m_pParent);
	m_pMainWnd = &dlg;
	mEvent = new CEvent(FALSE, TRUE, mEventName);
	dlg.SetWaitThreadPtr(this);
	dlg.SetParent(m_pParent);
	dlg.SetTitle(m_strTitle);
	dlg.SetMessage(m_strMessage);
	dlg.SetAbort(m_bAbort);
	dlg.DoModal();
	delete mEvent;
	mEvent = NULL;
	return FALSE;
}

int CDlgWaitThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CDlgWaitThread, CWinThread)
	//{{AFX_MSG_MAP(CDlgWaitThread)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWaitThread message handlers


/////////////////////////////////////////////////////////////////////////////
// CDlgWaitCmd

IMPLEMENT_DYNCREATE(CDlgWaitCmd, CCmdTarget)

CDlgWaitCmd::CDlgWaitCmd()
{
	m_strTitle.Empty();
	m_strMessage.Empty();
	m_bAbort = false;
	m_pParent = NULL;
	mEvent = NULL;
	mEventName.Empty();
	mThread = NULL;
}

CDlgWaitCmd::~CDlgWaitCmd()
{
}


BEGIN_MESSAGE_MAP(CDlgWaitCmd, CCmdTarget)
	//{{AFX_MSG_MAP(cWaitDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWaitCmd message handlers

void CDlgWaitCmd::Open(CString strTitle, CString strMessage, bool bAbort, CWnd* pParent)
{
	TRACE("===== CDlgWaitCmd::Open\n");

	// ���~�t���O��������
	mWaitAbortFlag = false;

	m_strTitle = strTitle;
	m_strMessage = strMessage;
	m_bAbort = bAbort;
	m_pParent = pParent;

	mEventName.Format(_T("STATIONCONTROLWAIT"));
	mEvent = CreateEvent(NULL, TRUE, FALSE, mEventName);
	mThread = (CDlgWaitThread*)AfxBeginThread(RUNTIME_CLASS(CDlgWaitThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	mThread->SetParent(pParent);
	mThread->SetTitle(m_strTitle);
	mThread->SetMessage(m_strMessage);
	mThread->SetAbort(m_bAbort);
	mThread->SetEventName(mEventName);
	mThread->ResumeThread();
}

void CDlgWaitCmd::Close()
{
	TRACE("===== CDlgWaitCmd::Close\n");

	// ���~�t���O���N���A
	mWaitAbortFlag = false;

	mThread->GetEvent()->SetEvent();
	CloseHandle(mEvent);
	mEvent = NULL;
}

// CWaitDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CWaitDialog, CDialogEx)

CWaitDialog::CWaitDialog(CString strTitle, CString strMessage, bool bAbort, CWnd* pParent /*=NULL*/)
	: CDialogEx(CWaitDialog::IDD, pParent)
{
	m_strTitle = strTitle;;
	m_strMessage = strMessage;
	m_bAbort = bAbort;
	m_pParent = pParent;
	mWaitAbortFlag = false;
	mThread = NULL;
}

CWaitDialog::~CWaitDialog()
{
}

void CWaitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_MESSAGE, m_strMessage);
}


BEGIN_MESSAGE_MAP(CWaitDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ABORT, &CWaitDialog::OnBnClickedButtonAbort)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CWaitDialog ���b�Z�[�W �n���h���[

BOOL CWaitDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CProgressCtrl *)GetDlgItem(IDC_PROGRESS_WAIT))->ModifyStyle( 0, PBS_MARQUEE );
	((CProgressCtrl *)GetDlgItem(IDC_PROGRESS_WAIT))->SetMarquee(TRUE, 25);

	GetDlgItem(IDC_BUTTON_ABORT)->ShowWindow((m_bAbort)?SW_SHOW:SW_HIDE);

	SetWindowText(m_strTitle);

	UpdateData(FALSE);

	SetTimer(1, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! �����҂��_�C�A���O

-# �u���~�v�{�^����������

@param
@retval
*/
/*============================================================================*/
void CWaitDialog::OnBnClickedButtonAbort()
{
	// ���~�t���O���Z�b�g
	mWaitAbortFlag = true;
}


void CWaitDialog::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);

	if (nIDEvent == 1){
		CSingleLock lock(mThread->GetEvent(), FALSE);
		if (lock.Lock(0)){
			EndDialog(IDOK);
		}
		else{
			SetTimer(1, 100, NULL);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}
