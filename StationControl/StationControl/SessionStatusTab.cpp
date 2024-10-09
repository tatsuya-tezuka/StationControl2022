/*============================================================================*/
/*! SessionStatusTab.cppCSessionInfo::eDisconnect

-# �Z�b�V������ԉ�ʁ@4��/1��
*/
/*============================================================================*/
// SessionStatusTab.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "SessionStatusTab.h"
#include "SessionLogin.h"
#include "afxdialogex.h"
#include "HookMsgBox.h"
#include "..\Utility\SshShell.h"
#include "SessionStatus.h"

// CSessionStatusTab �_�C�A���O

IMPLEMENT_DYNAMIC(CSessionStatusTab, CDialogBase)

/*============================================================================*/
/*! CSessionStatusTab

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CSessionStatusTab::CSessionStatusTab(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSessionStatusTab::IDD, pParent)
	, m_nStationID(0)
{

}

/*============================================================================*/
/*! CSessionStatusTab

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSessionStatusTab::~CSessionStatusTab()
{
}

void CSessionStatusTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SESSION_MONITOR_STATUS, m_csMonitorStatus);
	DDX_Control(pDX, IDC_STATIC_SESSION_CTRL_STATUS, m_csCtrlStatus);
	DDX_Control(pDX, IDC_STATIC_SESSION_NOTIFY_STATUS, m_csNotifyStatus);
	DDX_Control(pDX, IDC_STATIC_SESSION_HISTORY_STATUS, m_csHistoryStatus);
	DDX_Control(pDX, IDC_SESSIONSTATUS_MONITOR_BUTTON, m_buttonMonitor);
	DDX_Control(pDX, IDC_SESSIONSTATUS_CTRL_BUTTON, m_buttonCtrl);
	DDX_Control(pDX, IDC_SESSIONSTATUS_NOTIFY_BUTTON, m_buttonNotify);
	DDX_Control(pDX, IDC_SESSIONSTATUS_HISTORY_BUTTON, m_buttonHistory);
	DDX_Control(pDX, IDC_STATIC_SESSIONSTATUS_CTRL, m_staticCtrlStr);
}


BEGIN_MESSAGE_MAP(CSessionStatusTab, CDialogBase)
	ON_BN_CLICKED(IDC_SESSIONSTATUS_MONITOR_BUTTON, &CSessionStatusTab::OnBnClickedSessionstatusMonitorButton)
	ON_BN_CLICKED(IDC_SESSIONSTATUS_CTRL_BUTTON, &CSessionStatusTab::OnBnClickedSessionstatusCtrlButton)
	ON_BN_CLICKED(IDC_SESSIONSTATUS_NOTIFY_BUTTON, &CSessionStatusTab::OnBnClickedSessionstatusNotifyButton)
	ON_BN_CLICKED(IDC_SESSIONSTATUS_HISTORY_BUTTON, &CSessionStatusTab::OnBnClickedSessionstatusHistoryButton)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSessionStatusTab ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! CSessionStatusTab

-# �_�C�A���O����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
BOOL CSessionStatusTab::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// 1�ǃ��[�h�Ŗ����ꍇ�͐�����f�B�Z�[�u��
	if (theApp.GetSelectStationMode() != eStation_1)
	{
		m_staticCtrlStr.EnableWindow(FALSE);
		m_csCtrlStatus.EnableWindow(FALSE);
		m_buttonCtrl.EnableWindow(FALSE);
	}

	// �Ď����[�h���̓{�^����񊈐��ɂ���
	if (theApp.GetExecuteMode() != eExecuteMode_Control){
		GetDlgItem(IDC_SESSIONSTATUS_MONITOR_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_SESSIONSTATUS_CTRL_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_SESSIONSTATUS_NOTIFY_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_SESSIONSTATUS_HISTORY_BUTTON)->EnableWindow(FALSE);
	}

	TCHAR szMsg[256];
	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONUNKNOWN, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;

	m_csMonitorStatus.SetGradient(false);
	m_csMonitorStatus.SetTextColor(m_colorTextInv, m_colorTextInv);
	m_csMonitorStatus.SetBackColor(m_colorBkInvalid, m_colorBkInvalid);
	m_csMonitorStatus.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	m_csMonitorStatus.SetText(szMsg);
	m_sessionStatus[eSessionType_Monitor] = CSessionInfo::eUnknown;

	m_csCtrlStatus.SetGradient(false);
	m_csCtrlStatus.SetTextColor(m_colorTextInv, m_colorTextInv);
	m_csCtrlStatus.SetBackColor(m_colorBkInvalid, m_colorBkInvalid);
	m_csCtrlStatus.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	m_csCtrlStatus.SetText(szMsg);
	m_sessionStatus[eSessionType_Ctrl] = CSessionInfo::eUnknown;

	m_csNotifyStatus.SetGradient(false);
	m_csNotifyStatus.SetTextColor(m_colorTextInv, m_colorTextInv);
	m_csNotifyStatus.SetBackColor(m_colorBkInvalid, m_colorBkInvalid);
	m_csNotifyStatus.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	m_csNotifyStatus.SetText(szMsg);
	m_sessionStatus[eSessionType_Notify] = CSessionInfo::eUnknown;

	m_csHistoryStatus.SetGradient(false);
	m_csHistoryStatus.SetTextColor(m_colorTextInv, m_colorTextInv);
	m_csHistoryStatus.SetBackColor(m_colorBkInvalid, m_colorBkInvalid);
	m_csHistoryStatus.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	m_csHistoryStatus.SetText(szMsg);
	m_sessionStatus[eSessionType_History] = CSessionInfo::eUnknown;

	// ��ʕ\��
	UpdateSessionDisplay();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CSessionStatusTab

-# �Ď��Z�b�V������Ԃ�\������

@param	�Ȃ�
@retval TRUE�FOK / FALSE�FNG

*/
/*============================================================================*/
void CSessionStatusTab::DispMonitorStatus()
{
	// �Ď��Z�b�V������Ԃ�����
	UINT nStatus = theApp.GetSatelliteData().GetSessionInfo(m_nStationID).GetSessionStatus(eSessionType_Monitor);

	switch (nStatus)
	{
	case CSessionInfo::eDisconnect:
		DispMonitorDisConStatus();
		break;

	case CSessionInfo::eConnect:
		DispMonitorConStatus();
		break;

	case CSessionInfo::eUnknown:
		DispMonitorUnknownStatus();
		break;

	case CSessionInfo::eGettingNow:
		DispMonitorGettingNowStatus();
		break;

	case CSessionInfo::eError:
		DispMonitorErrorStatus();
		break;

	default:
		break;
	}
}


/*============================================================================*/
/*! CSessionStatusTab

-# ����Z�b�V������Ԃ�\������

@param	�Ȃ�
@retval TRUE�FOK / FALSE�FNG

*/
/*============================================================================*/
void CSessionStatusTab::DispCtrlStatus()
{
	// ����Z�b�V������Ԃ�����
	UINT nStatus = theApp.GetSatelliteData().GetSessionInfo(m_nStationID).GetSessionStatus(eSessionType_Ctrl);

	switch (nStatus)
	{
	case CSessionInfo::eDisconnect:
		DispCtrlDisConStatus();
		break;

	case CSessionInfo::eConnect:
		DispCtrlConStatus();
		break;

	case CSessionInfo::eUnknown:
		DispCtrlUnknownStatus();
		break;

	case CSessionInfo::eGettingNow:
		DispCtrlGettingNowStatus();
		break;

	case CSessionInfo::eError:
		DispCtrlErrorStatus();
		break;

	default:
		break;
	}
}


/*============================================================================*/
/*! CSessionStatusTab

-# �ʒm�Z�b�V������Ԃ�\������

@param	�Ȃ�
@retval TRUE�FOK / FALSE�FNG

*/
/*============================================================================*/
void CSessionStatusTab::DispNotifyStatus()
{
	// �ʒm�Z�b�V������Ԃ�����
	UINT nStatus = theApp.GetSatelliteData().GetSessionInfo(m_nStationID).GetSessionStatus(eSessionType_Notify);

	switch (nStatus)
	{
	case CSessionInfo::eDisconnect:
		DispNotifyDisConStatus();
		break;

	case CSessionInfo::eConnect:
		DispNotifyConStatus();
		break;

	case CSessionInfo::eUnknown:
		DispNotifyUnknownStatus();
		break;

	case CSessionInfo::eGettingNow:
		DispNotifyGettingNowStatus();
		break;

	case CSessionInfo::eError:
		DispNotifyErrorStatus();
		break;

	default:
		break;
	}
}


/*============================================================================*/
/*! CSessionStatusTab

-# �����Z�b�V������Ԃ�\������

@param	�Ȃ�
@retval TRUE�FOK / FALSE�FNG

*/
/*============================================================================*/
void CSessionStatusTab::DispHistoryStatus()
{
	// �����Z�b�V������Ԃ�����
	UINT nStatus = theApp.GetSatelliteData().GetSessionInfo(m_nStationID).GetSessionStatus(eSessionType_History);

	switch (nStatus)
	{
	case CSessionInfo::eDisconnect:
		DispHistoryDisConStatus();
		break;

	case CSessionInfo::eConnect:
		DispHistoryConStatus();
		break;

	case CSessionInfo::eUnknown:
		DispHistoryUnknownStatus();
		break;

	case CSessionInfo::eGettingNow:
		DispHistoryGettingNowStatus();
		break;

	case CSessionInfo::eError:
		DispHistoryErrorStatus();
		break;

	default:
		break;
	}
}


/*============================================================================*/
/*! CSessionStatusTab

-# �Ď��Z�b�V������s���\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispMonitorUnknownStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONUNKNOWN, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csMonitorStatus.SetText(szMsg);
	m_csMonitorStatus.SetTextColor(m_colorTextInv, m_colorTextInv);
	m_csMonitorStatus.SetBackColor(m_colorBkInvalid, m_colorBkInvalid);

	m_buttonMonitor.EnableWindow(FALSE);

	m_sessionStatus[eSessionType_Monitor] = CSessionInfo::eUnknown;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �Ď��Z�b�V�������擾���\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispMonitorGettingNowStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONGETTINGNOW, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csMonitorStatus.SetText(szMsg);
	m_csMonitorStatus.SetTextColor(m_colorTextInv, m_colorTextInv);
	m_csMonitorStatus.SetBackColor(m_colorBkInvalid, m_colorBkInvalid);

	m_buttonMonitor.EnableWindow(FALSE);

	m_sessionStatus[eSessionType_Monitor] = CSessionInfo::eGettingNow;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �Ď��Z�b�V�������G���[�\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispMonitorErrorStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONERROR, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csMonitorStatus.SetText(szMsg);
	m_csMonitorStatus.SetTextColor(m_colorTextInv, m_colorTextInv);
	m_csMonitorStatus.SetBackColor(m_colorBkInvalid, m_colorBkInvalid);

	m_buttonMonitor.EnableWindow(FALSE);

	m_sessionStatus[eSessionType_Monitor] = CSessionInfo::eError;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �Ď��Z�b�V������ڑ��\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispMonitorConStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONCONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csMonitorStatus.SetText(szMsg);
	m_csMonitorStatus.SetTextColor(m_colorText, m_colorText);
	m_csMonitorStatus.SetBackColor(m_colorBkConnect, m_colorBkConnect);

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_DISCONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_buttonMonitor.EnableWindow(TRUE);
	m_buttonMonitor.SetWindowText(szMsg);

	m_sessionStatus[eSessionType_Monitor] = CSessionInfo::eConnect;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �Ď��Z�b�V������ؒf�\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispMonitorDisConStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONDISCONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csMonitorStatus.SetText(szMsg);
	m_csMonitorStatus.SetTextColor(m_colorText, m_colorText);
	m_csMonitorStatus.SetBackColor(m_colorBkDisConnect, m_colorBkDisConnect);

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_CONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_buttonMonitor.EnableWindow(TRUE);
	m_buttonMonitor.SetWindowText(szMsg);

	m_sessionStatus[eSessionType_Monitor] = CSessionInfo::eDisconnect;
}


/*============================================================================*/
/*! CSessionStatusTab

-# ����Z�b�V������s���\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispCtrlUnknownStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONUNKNOWN, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csCtrlStatus.SetText(szMsg);
	// 1�ǃ��[�h�̏ꍇ�͐�����C�l�[�u��
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		m_csCtrlStatus.SetTextColor(m_colorTextInv, m_colorTextInv);
		m_csCtrlStatus.SetBackColor(m_colorBkInvalid, m_colorBkInvalid);
		m_buttonCtrl.EnableWindow(FALSE);
	}

	m_sessionStatus[eSessionType_Ctrl] = CSessionInfo::eUnknown;
}


/*============================================================================*/
/*! CSessionStatusTab

-# ����Z�b�V�������擾���\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispCtrlGettingNowStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONGETTINGNOW, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csCtrlStatus.SetText(szMsg);
	// 1�ǃ��[�h�̏ꍇ�͐�����C�l�[�u��
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		m_csCtrlStatus.SetTextColor(m_colorTextInv, m_colorTextInv);
		m_csCtrlStatus.SetBackColor(m_colorBkInvalid, m_colorBkInvalid);
		m_buttonCtrl.EnableWindow(FALSE);
	}

	m_sessionStatus[eSessionType_Ctrl] = CSessionInfo::eGettingNow;
}


/*============================================================================*/
/*! CSessionStatusTab

-# ����Z�b�V�������G���[�\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispCtrlErrorStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONERROR, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csCtrlStatus.SetText(szMsg);
	// 1�ǃ��[�h�̏ꍇ�͐�����C�l�[�u��
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		m_csCtrlStatus.SetTextColor(m_colorTextInv, m_colorTextInv);
		m_csCtrlStatus.SetBackColor(m_colorBkInvalid, m_colorBkInvalid);
		m_buttonCtrl.EnableWindow(FALSE);
	}

	m_sessionStatus[eSessionType_Ctrl] = CSessionInfo::eError;
}


/*============================================================================*/
/*! CSessionStatusTab

-# ����Z�b�V������ڑ��\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispCtrlConStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONCONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csCtrlStatus.SetText(szMsg);
	// 1�ǃ��[�h�̏ꍇ�͐�����C�l�[�u��
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		m_csCtrlStatus.SetTextColor(m_colorText, m_colorText);
		m_csCtrlStatus.SetBackColor(m_colorBkConnect, m_colorBkConnect);
	}

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_DISCONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	// 1�ǃ��[�h�̏ꍇ�͐�����C�l�[�u��
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		m_buttonCtrl.EnableWindow(TRUE);
	}
	m_buttonCtrl.SetWindowText(szMsg);

	m_sessionStatus[eSessionType_Ctrl] = CSessionInfo::eConnect;
}


/*============================================================================*/
/*! CSessionStatusTab

-# ����Z�b�V������ؒf�\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispCtrlDisConStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONDISCONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csCtrlStatus.SetText(szMsg);
	// 1�ǃ��[�h�̏ꍇ�͐�����C�l�[�u��
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		m_csCtrlStatus.SetTextColor(m_colorText, m_colorText);
		m_csCtrlStatus.SetBackColor(m_colorBkDisConnect, m_colorBkDisConnect);
	}

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_CONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	// 1�ǃ��[�h�̏ꍇ�͐�����C�l�[�u��
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		m_buttonCtrl.EnableWindow(TRUE);
	}
	m_buttonCtrl.SetWindowText(szMsg);

	m_sessionStatus[eSessionType_Ctrl] = CSessionInfo::eDisconnect;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �ʒm�Z�b�V������s���\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispNotifyUnknownStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONUNKNOWN, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csNotifyStatus.SetText(szMsg);
	m_csNotifyStatus.SetTextColor(m_colorTextInv, m_colorTextInv);
	m_csNotifyStatus.SetBackColor(m_colorBkInvalid, m_colorBkInvalid);

	m_buttonNotify.EnableWindow(FALSE);

	m_sessionStatus[eSessionType_Notify] = CSessionInfo::eUnknown;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �ʒm�Z�b�V�������擾���\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispNotifyGettingNowStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONGETTINGNOW, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csNotifyStatus.SetText(szMsg);
	m_csNotifyStatus.SetTextColor(m_colorTextInv, m_colorTextInv);
	m_csNotifyStatus.SetBackColor(m_colorBkInvalid, m_colorBkInvalid);

	m_buttonNotify.EnableWindow(FALSE);

	m_sessionStatus[eSessionType_Notify] = CSessionInfo::eGettingNow;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �ʒm�Z�b�V�������G���[�\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispNotifyErrorStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONERROR, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csNotifyStatus.SetText(szMsg);
	m_csNotifyStatus.SetTextColor(m_colorTextInv, m_colorTextInv);
	m_csNotifyStatus.SetBackColor(m_colorBkInvalid, m_colorBkInvalid);

	m_buttonNotify.EnableWindow(FALSE);

	m_sessionStatus[eSessionType_Notify] = CSessionInfo::eError;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �ʒm�Z�b�V������ڑ��\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispNotifyConStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONCONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csNotifyStatus.SetText(szMsg);
	m_csNotifyStatus.SetTextColor(m_colorText, m_colorText);
	m_csNotifyStatus.SetBackColor(m_colorBkConnect, m_colorBkConnect);

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_DISCONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_buttonNotify.EnableWindow(TRUE);
	m_buttonNotify.SetWindowText(szMsg);

	m_sessionStatus[eSessionType_Notify] = CSessionInfo::eConnect;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �ʒm�Z�b�V������ؒf�\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispNotifyDisConStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONDISCONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csNotifyStatus.SetText(szMsg);
	m_csNotifyStatus.SetTextColor(m_colorText, m_colorText);
	m_csNotifyStatus.SetBackColor(m_colorBkDisConnect, m_colorBkDisConnect);

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_CONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_buttonNotify.EnableWindow(TRUE);
	m_buttonNotify.SetWindowText(szMsg);

	m_sessionStatus[eSessionType_Notify] = CSessionInfo::eDisconnect;
}



/*============================================================================*/
/*! CSessionStatusTab

-# �����Z�b�V������s���\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispHistoryUnknownStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONUNKNOWN, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csHistoryStatus.SetText(szMsg);
	m_csHistoryStatus.SetTextColor(m_colorTextInv, m_colorTextInv);
	m_csHistoryStatus.SetBackColor(m_colorBkInvalid, m_colorBkInvalid);

	m_buttonHistory.EnableWindow(FALSE);

	m_sessionStatus[eSessionType_History] = CSessionInfo::eUnknown;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �����Z�b�V�������擾���\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispHistoryGettingNowStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONGETTINGNOW, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csHistoryStatus.SetText(szMsg);
	m_csHistoryStatus.SetTextColor(m_colorTextInv, m_colorTextInv);
	m_csHistoryStatus.SetBackColor(m_colorBkInvalid, m_colorBkInvalid);

	m_buttonHistory.EnableWindow(FALSE);

	m_sessionStatus[eSessionType_History] = CSessionInfo::eGettingNow;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �����Z�b�V�������G���[�\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispHistoryErrorStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONERROR, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csHistoryStatus.SetText(szMsg);
	m_csHistoryStatus.SetTextColor(m_colorTextInv, m_colorTextInv);
	m_csHistoryStatus.SetBackColor(m_colorBkInvalid, m_colorBkInvalid);

	m_buttonNotify.EnableWindow(FALSE);

	m_sessionStatus[eSessionType_History] = CSessionInfo::eError;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �����Z�b�V������ڑ��\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispHistoryConStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONCONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csHistoryStatus.SetText(szMsg);
	m_csHistoryStatus.SetTextColor(m_colorText, m_colorText);
	m_csHistoryStatus.SetBackColor(m_colorBkConnect, m_colorBkConnect);

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_DISCONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_buttonHistory.EnableWindow(TRUE);
	m_buttonHistory.SetWindowText(szMsg);

	m_sessionStatus[eSessionType_History] = CSessionInfo::eConnect;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �����Z�b�V������ؒf�\������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::DispHistoryDisConStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONDISCONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csHistoryStatus.SetText(szMsg);
	m_csHistoryStatus.SetTextColor(m_colorText, m_colorText);
	m_csHistoryStatus.SetBackColor(m_colorBkDisConnect, m_colorBkDisConnect);

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_CONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_buttonHistory.EnableWindow(TRUE);
	m_buttonHistory.SetWindowText(szMsg);

	m_sessionStatus[eSessionType_History] = CSessionInfo::eDisconnect;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �Ď��Z�b�V������ؒf����

@param	�Ȃ�
@retval TRUE�FOK / FALSE�FNG

*/
/*============================================================================*/
BOOL CSessionStatusTab::DisConnectMonitorSession()
{
	TCHAR szTitle[256];
	TCHAR szMsg[256];

	memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONDISCONNECT_TITLE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
		return FALSE;
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_DISCONNECT_MONITOR, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;

	if (IDOK == MessageBoxHooked(this->m_hWnd, szMsg, szTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = DIS_SDTP_SH;
		CString arg;

		arg.Format(_T("%s moni"), (LPCTSTR)CString(mStationSimpleString[m_nStationID]).MakeLower());

		stParam.strRemShellParam.Format(arg);

		// �Z�b�V������Ԃ�s���ɂ��Ă���
		theApp.GetSatelliteData().GetSessionInfo(m_nStationID).SetSessionStatus(eSessionType_Monitor, CSessionInfo::eUnknown);
		// �����V�F���s
		int err = CSshShell::ExecShell(stParam);
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CSessionStatusTab"), _T("DisConnectMonitorSession"), _T("debug"), strDebug, nLogEx::debug);
		}
		if (err)
		{
			// ��ʕ\��
			DispMonitorStatus();
		}
		else
		{
			// �Z�b�V������Ԏ擾
			GetSessionStatus(TRUE);
		}
	}

	return TRUE;
}


/*============================================================================*/
/*! CSessionStatusTab

-# ����Z�b�V������ؒf����

@param	�Ȃ�
@retval TRUE�FOK / FALSE�FNG

*/
/*============================================================================*/
BOOL CSessionStatusTab::DisConnectCtrlSession()
{
	TCHAR szTitle[256];
	TCHAR szMsg[256];

	memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONDISCONNECT_TITLE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
		return FALSE;
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_DISCONNECT_CTRL, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;

	if (IDOK == MessageBoxHooked(this->m_hWnd, szMsg, szTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = DIS_SDTP_SH;
		CString arg;

		arg.Format(_T("%s ctrl"), (LPCTSTR)CString(mStationSimpleString[m_nStationID]).MakeLower());

		stParam.strRemShellParam.Format(arg);

		// �Z�b�V������Ԃ�s���ɂ��Ă���
		theApp.GetSatelliteData().GetSessionInfo(m_nStationID).SetSessionStatus(eSessionType_Ctrl, CSessionInfo::eUnknown);
		// �����V�F���s
		int err = CSshShell::ExecShell(stParam);
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CSessionStatusTab"), _T("DisConnectCtrlSession"), _T("debug"), strDebug, nLogEx::debug);
		}
		if (err)
		{
			// ��ʕ\��
			DispMonitorStatus();
		}
		else
		{
			// �Z�b�V������Ԏ擾
			GetSessionStatus(TRUE);
		}
	}

	return FALSE;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �ʒm�Z�b�V������ؒf����

@param	�Ȃ�
@retval TRUE�FOK / FALSE�FNG

*/
/*============================================================================*/
BOOL CSessionStatusTab::DisConnectNotifySession()
{
	TCHAR szTitle[256];
	TCHAR szMsg[256];

	memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONDISCONNECT_TITLE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
		return FALSE;
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_DISCONNECT_NOTIFY, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;

	if (IDOK == MessageBoxHooked(this->m_hWnd, szMsg, szTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = DIS_SDTP_SH;
		CString arg;

		arg.Format(_T("%s info"), (LPCTSTR)CString(mStationSimpleString[m_nStationID]).MakeLower());

		stParam.strRemShellParam.Format(arg);

		// �Z�b�V������Ԃ�s���ɂ��Ă���
		theApp.GetSatelliteData().GetSessionInfo(m_nStationID).SetSessionStatus(eSessionType_Notify, CSessionInfo::eUnknown);
		// �����V�F���s
		int err = CSshShell::ExecShell(stParam);
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CSessionStatusTab"), _T("DisConnectNotifySession"), _T("debug"), strDebug, nLogEx::debug);
		}
		if (err)
		{
			// ��ʕ\��
			DispMonitorStatus();
		}
		else
		{
			// �Z�b�V������Ԏ擾
			GetSessionStatus(TRUE);
		}
	}

	return TRUE;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �����Z�b�V������ؒf����

@param	�Ȃ�
@retval TRUE�FOK / FALSE�FNG

*/
/*============================================================================*/
BOOL CSessionStatusTab::DisConnectHistorySession()
{
	TCHAR szTitle[256];
	TCHAR szMsg[256];

	memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONDISCONNECT_TITLE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
		return FALSE;
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_DISCONNECT_HISTORY, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;

	if (IDOK == MessageBoxHooked(this->m_hWnd, szMsg, szTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = DIS_SDTP_SH;
		CString arg;

		arg.Format(_T("%s opelog"), (LPCTSTR)CString(mStationSimpleString[m_nStationID]).MakeLower());

		stParam.strRemShellParam.Format(arg);

		// �Z�b�V������Ԃ�s���ɂ��Ă���
		theApp.GetSatelliteData().GetSessionInfo(m_nStationID).SetSessionStatus(eSessionType_History, CSessionInfo::eUnknown);
		// �����V�F���s
		int err = CSshShell::ExecShell(stParam);
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CSessionStatusTab"), _T("DisConnectHistorySession"), _T("debug"), strDebug, nLogEx::debug);
		}
		if (err)
		{
			// ��ʕ\��
			DispMonitorStatus();
		}
		else
		{
			// �Z�b�V������Ԏ擾
			GetSessionStatus(TRUE);
		}
	}

	return TRUE;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �Ď��Z�b�V������ڑ�����

@param	�Ȃ�
@retval TRUE�FOK / FALSE�FNG

*/
/*============================================================================*/
BOOL CSessionStatusTab::ConnectMonitorSession()
{
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = CON_SDTP_SH;
	CString arg;

	arg.Format(_T("%s moni"), (LPCTSTR)CString(mStationSimpleString[m_nStationID]).MakeLower());

	stParam.strRemShellParam.Format(arg);

	// �Z�b�V������Ԃ�s���ɂ��Ă���
	theApp.GetSatelliteData().GetSessionInfo(m_nStationID).SetSessionStatus(eSessionType_Monitor, CSessionInfo::eUnknown);
	// �����V�F���s
	int err = CSshShell::ExecShell(stParam);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CSessionStatusTab"), _T("ConnectMonitorSession"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err)
	{
		// ��ʕ\��
		DispMonitorStatus();
	}
	else
	{
		// �Z�b�V������Ԏ擾
		GetSessionStatus(TRUE);
	}

	return TRUE;
}


/*============================================================================*/
/*! CSessionStatusTab

-# ����Z�b�V������ڑ�����

@param	�Ȃ�
@retval TRUE�FOK / FALSE�FNG

*/
/*============================================================================*/
BOOL CSessionStatusTab::ConnectCtrlSession()
{
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = CON_SDTP_SH;
	CString arg;

	arg.Format(_T("%s ctrl"), (LPCTSTR)CString(mStationSimpleString[m_nStationID]).MakeLower());

	stParam.strRemShellParam.Format(arg);

	// �Z�b�V������Ԃ�s���ɂ��Ă���
	theApp.GetSatelliteData().GetSessionInfo(m_nStationID).SetSessionStatus(eSessionType_Ctrl, CSessionInfo::eUnknown);
	// �����V�F���s
	int err = CSshShell::ExecShell(stParam);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CSessionStatusTab"), _T("ConnectCtrlSession"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err)
	{
		// ��ʕ\��
		DispMonitorStatus();
	}
	else
	{
		// �Z�b�V������Ԏ擾
		GetSessionStatus(TRUE);
	}

	return TRUE;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �ʒm�Z�b�V������ڑ�����

@param	�Ȃ�
@retval TRUE�FOK / FALSE�FNG

*/
/*============================================================================*/
BOOL CSessionStatusTab::ConnectNotifySession()
{
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = CON_SDTP_SH;
	CString arg;

	arg.Format(_T("%s info"), (LPCTSTR)CString(mStationSimpleString[m_nStationID]).MakeLower());

	stParam.strRemShellParam.Format(arg);

	// �Z�b�V������Ԃ�s���ɂ��Ă���
	theApp.GetSatelliteData().GetSessionInfo(m_nStationID).SetSessionStatus(eSessionType_Notify, CSessionInfo::eUnknown);
	// �����V�F���s
	int err = CSshShell::ExecShell(stParam);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CSessionStatusTab"), _T("ConnectNotifySession"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err)
	{
		// ��ʕ\��
		DispMonitorStatus();
	}
	else
	{
		// �Z�b�V������Ԏ擾
		GetSessionStatus(TRUE);
	}

	return TRUE;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �����Z�b�V������ڑ�����

@param	�Ȃ�
@retval TRUE�FOK / FALSE�FNG

*/
/*============================================================================*/
BOOL CSessionStatusTab::ConnectHistorySession()
{
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = CON_SDTP_SH;
	CString arg;

	arg.Format(_T("%s opelog"), (LPCTSTR)CString(mStationSimpleString[m_nStationID]).MakeLower());

	stParam.strRemShellParam.Format(arg);

	// �Z�b�V������Ԃ�s���ɂ��Ă���
	theApp.GetSatelliteData().GetSessionInfo(m_nStationID).SetSessionStatus(eSessionType_History, CSessionInfo::eUnknown);
	// �����V�F���s
	int err = CSshShell::ExecShell(stParam);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CSessionStatusTab"), _T("ConnectHistorySession"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err)
	{
		// ��ʕ\��
		DispMonitorStatus();
	}
	else
	{
		// �Z�b�V������Ԏ擾
		GetSessionStatus(TRUE);
	}

	return TRUE;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �Ď��Z�b�V�����̐ڑ��ؒf�{�^���������ɌĂ΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::OnBnClickedSessionstatusMonitorButton()
{
	// �Ď��Z�b�V������Ԃ�����
	switch (m_sessionStatus[eSessionType_Monitor])
	{
	case CSessionInfo::eDisconnect:
		// �ڑ�����
		ConnectMonitorSession();
		break;

	case CSessionInfo::eConnect:
		// �ؒf����
		DisConnectMonitorSession();
		break;

	case CSessionInfo::eUnknown:
	case CSessionInfo::eGettingNow:
	default:
		break;
	}
}


/*============================================================================*/
/*! CSessionStatusTab

-# ����Z�b�V�����̐ڑ��ؒf�{�^���������ɌĂ΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::OnBnClickedSessionstatusCtrlButton()
{
	// ����Z�b�V������Ԃ�����
	switch (m_sessionStatus[eSessionType_Ctrl])
	{
	case CSessionInfo::eDisconnect:
		// �ڑ�����
		ConnectCtrlSession();
		break;

	case CSessionInfo::eConnect:
		// �ؒf����
		DisConnectCtrlSession();
		break;

	case CSessionInfo::eUnknown:
	case CSessionInfo::eGettingNow:
	default:
		break;
	}
}


/*============================================================================*/
/*! CSessionStatusTab

-# �ʒm�Z�b�V�����̐ڑ��ؒf�{�^���������ɌĂ΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::OnBnClickedSessionstatusNotifyButton()
{
	// �ʒm�Z�b�V������Ԃ�����
	switch (m_sessionStatus[eSessionType_Notify])
	{
	case CSessionInfo::eDisconnect:
		// �ڑ�����
		ConnectNotifySession();
		break;

	case CSessionInfo::eConnect:
		// �ؒf����
		DisConnectNotifySession();
		break;

	case CSessionInfo::eUnknown:
	case CSessionInfo::eGettingNow:
	default:
		break;
	}
}


/*============================================================================*/
/*! CSessionStatusTab

-# �����Z�b�V�����̐ڑ��ؒf�{�^���������ɌĂ΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::OnBnClickedSessionstatusHistoryButton()
{
	// �����Z�b�V������Ԃ�����
	switch (m_sessionStatus[eSessionType_History])
	{
	case CSessionInfo::eDisconnect:
		// �ڑ�����
		ConnectHistorySession();
		break;

	case CSessionInfo::eConnect:
		// �ؒf����
		DisConnectHistorySession();
		break;

	case CSessionInfo::eUnknown:
	case CSessionInfo::eGettingNow:
	default:
		break;
	}
}


/*============================================================================*/
/*! CSessionStatusTab

-# �`�掞�ɌĂ΂�A�w��R���g���[���̔w�i��h��Ԃ��u���V�̃n���h����Ԃ�

@param	pDC�F�q�E�B���h�E�̃f�B�X�v���C�R���e�L�X�g�̃|�C���^
@param	pWnd�F�F���i�[���ꂽ�R���g���[���̃|�C���^
@param	nCtrlColor�F�R���g���[���̎��
@retval	�R���g���[���̔w�i��h��Ԃ��u���V�̃n���h��

*/
/*============================================================================*/
HBRUSH CSessionStatusTab::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogBase::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
		// �_�C�A���O�̔w�i�F
		if (pWnd == this)
		{
			hbr = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
		}
		break;

	default:
		break;
	}

	// TODO: ����l���g�p�������Ȃ��ꍇ�͕ʂ̃u���V��Ԃ��܂��B
	return hbr;
}


/*============================================================================*/
/*! CSessionStatusTab

-# �Z�b�V���������擾����

@param	bWaitComplete�FTRUE:�V�F���I����҂i�f�t�H���g�j / FALSE:�V�F���I����҂��Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::GetSessionStatus(BOOL bWaitComplete /*= TRUE*/)
{
	// �Z�b�V������Ԃ������[�g�V�F���Ŏ擾����
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = STAT_SDTP_SH;
	CString arg;

	if (theApp.GetSelectStationMode() == eStation_1)
	{
		// 1�ǂ̏ꍇ�͋ǖ��ƑS�Z�b�V����(all)�Z�b�g
		int nStation = theApp.GetSelectStation();
		arg.Format(_T("%s all"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower());
		stParam.strRemShellParam.Format(arg);
	}
	else
	{
		// 4�ǂ̏ꍇ��all�ƑS�Z�b�V����(all)���Z�b�g
		stParam.strRemShellParam.Format(_T("all all"));
	}

	// �����V�F���s
	int err = CSshShell::ExecShell(stParam, bWaitComplete);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShellThread"), __FILEW__, __LINE__);
		CLogTraceEx::Write(_T(""), _T("CSessionStatusTab"), _T("ConnectCtrlSession"), _T("debug"), strDebug, nLogEx::debug);
	}
	if (err)
	{
		// ��ʕ\��
		UpdateSessionDisplay();
	}
	else
	{
		// �Z�b�V������Ԃ��擾���ɂ��Ă���
		theApp.GetSatelliteData().GetSessionInfo(m_nStationID).SetSessionStatus(eSessionType_Num, CSessionInfo::eGettingNow);
		if (bWaitComplete)
		{
			// �Z�b�V������ԃt�@�C���ǂݍ���
			if (theApp.GetSatelliteData().ReadSessionStatusFile())
			{
				// ��ʕ\��
				UpdateSessionDisplay();
			}
		}
	}
}


/*============================================================================*/
/*! CSessionStatusTab

-# �Z�b�V�������\�����X�V����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatusTab::UpdateSessionDisplay()
{
	// ��ʕ\��
	DispMonitorStatus();
	DispCtrlStatus();
	DispNotifyStatus();
	DispHistoryStatus();

	UpdateData(FALSE);
}

