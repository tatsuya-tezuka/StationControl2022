// SessionStatusUDSC64m.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "SessionStatusUDSC64m.h"
#include "afxdialogex.h"


// CSessionStatusUDSC64m �_�C�A���O

IMPLEMENT_DYNAMIC(CSessionStatusUDSC64m, CDialogEx)

CSessionStatusUDSC64m::CSessionStatusUDSC64m(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSessionStatusUDSC64m::IDD, pParent)
{

}

CSessionStatusUDSC64m::~CSessionStatusUDSC64m()
{
}

void CSessionStatusUDSC64m::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SESSIONSTATUS_MONITOR_EDIT, m_ceditMonitor);
	DDX_Control(pDX, IDC_SESSIONSTATUS_CTRL_EDIT, m_ceditMonitor);
	DDX_Control(pDX, IDC_SESSIONSTATUS_NOTIFY_EDIT, m_ceditMonitor);
	DDX_Control(pDX, IDC_SESSIONSTATUS_HISTORY_EDIT, m_ceditMonitor);
}


BEGIN_MESSAGE_MAP(CSessionStatusUDSC64m, CDialogEx)
	ON_BN_CLICKED(IDC_SESSIONSTATUS_MONITOR_BUTTON, &CSessionStatusUDSC64m::OnBnClickedSessionstatusMonitorButton)
	ON_BN_CLICKED(IDC_SESSIONSTATUS_CTRL_BUTTON, &CSessionStatusUDSC64m::OnBnClickedSessionstatusCtrlButton)
	ON_BN_CLICKED(IDC_SESSIONSTATUS_NOTIFY_BUTTON, &CSessionStatusUDSC64m::OnBnClickedSessionstatusNotifyButton)
	ON_BN_CLICKED(IDC_SESSIONSTATUS_HISTORY_BUTTON, &CSessionStatusUDSC64m::OnBnClickedSessionstatusHistoryButton)
END_MESSAGE_MAP()


// CSessionStatusUDSC64m ���b�Z�[�W �n���h���[


void CSessionStatusUDSC64m::OnBnClickedSessionstatusMonitorButton()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}


void CSessionStatusUDSC64m::OnBnClickedSessionstatusCtrlButton()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}


void CSessionStatusUDSC64m::OnBnClickedSessionstatusNotifyButton()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}


void CSessionStatusUDSC64m::OnBnClickedSessionstatusHistoryButton()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}
