/*============================================================================*/
/*! SessionStatusTab.cppCSessionInfo::eDisconnect

-# セッション状態画面　4局/1局
*/
/*============================================================================*/
// SessionStatusTab.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "SessionStatusTab.h"
#include "SessionLogin.h"
#include "afxdialogex.h"
#include "HookMsgBox.h"
#include "..\Utility\SshShell.h"
#include "SessionStatus.h"

// CSessionStatusTab ダイアログ

IMPLEMENT_DYNAMIC(CSessionStatusTab, CDialogBase)

/*============================================================================*/
/*! CSessionStatusTab

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CSessionStatusTab::CSessionStatusTab(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSessionStatusTab::IDD, pParent)
	, m_nStationID(0)
{

}

/*============================================================================*/
/*! CSessionStatusTab

-# デストラクタ

@param  なし
@retval なし
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


// CSessionStatusTab メッセージ ハンドラー

/*============================================================================*/
/*! CSessionStatusTab

-# ダイアログ初期化処理

@param  なし
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
BOOL CSessionStatusTab::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// 1局モードで無い場合は制御をディセーブル
	if (theApp.GetSelectStationMode() != eStation_1)
	{
		m_staticCtrlStr.EnableWindow(FALSE);
		m_csCtrlStatus.EnableWindow(FALSE);
		m_buttonCtrl.EnableWindow(FALSE);
	}

	// 監視モード時はボタンを非活性にする
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

	// 画面表示
	UpdateSessionDisplay();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CSessionStatusTab

-# 監視セッション状態を表示する

@param	なし
@retval TRUE：OK / FALSE：NG

*/
/*============================================================================*/
void CSessionStatusTab::DispMonitorStatus()
{
	// 監視セッション状態を見る
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

-# 制御セッション状態を表示する

@param	なし
@retval TRUE：OK / FALSE：NG

*/
/*============================================================================*/
void CSessionStatusTab::DispCtrlStatus()
{
	// 制御セッション状態を見る
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

-# 通知セッション状態を表示する

@param	なし
@retval TRUE：OK / FALSE：NG

*/
/*============================================================================*/
void CSessionStatusTab::DispNotifyStatus()
{
	// 通知セッション状態を見る
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

-# 履歴セッション状態を表示する

@param	なし
@retval TRUE：OK / FALSE：NG

*/
/*============================================================================*/
void CSessionStatusTab::DispHistoryStatus()
{
	// 履歴セッション状態を見る
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

-# 監視セッションを不明表示する

@param	なし
@retval	なし

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

-# 監視セッションを取得中表示する

@param	なし
@retval	なし

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

-# 監視セッションをエラー表示する

@param	なし
@retval	なし

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

-# 監視セッションを接続表示する

@param	なし
@retval	なし

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

-# 監視セッションを切断表示する

@param	なし
@retval	なし

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

-# 制御セッションを不明表示する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSessionStatusTab::DispCtrlUnknownStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONUNKNOWN, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csCtrlStatus.SetText(szMsg);
	// 1局モードの場合は制御をイネーブル
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

-# 制御セッションを取得中表示する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSessionStatusTab::DispCtrlGettingNowStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONGETTINGNOW, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csCtrlStatus.SetText(szMsg);
	// 1局モードの場合は制御をイネーブル
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

-# 制御セッションをエラー表示する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSessionStatusTab::DispCtrlErrorStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONERROR, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csCtrlStatus.SetText(szMsg);
	// 1局モードの場合は制御をイネーブル
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

-# 制御セッションを接続表示する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSessionStatusTab::DispCtrlConStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONCONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csCtrlStatus.SetText(szMsg);
	// 1局モードの場合は制御をイネーブル
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		m_csCtrlStatus.SetTextColor(m_colorText, m_colorText);
		m_csCtrlStatus.SetBackColor(m_colorBkConnect, m_colorBkConnect);
	}

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_DISCONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	// 1局モードの場合は制御をイネーブル
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		m_buttonCtrl.EnableWindow(TRUE);
	}
	m_buttonCtrl.SetWindowText(szMsg);

	m_sessionStatus[eSessionType_Ctrl] = CSessionInfo::eConnect;
}


/*============================================================================*/
/*! CSessionStatusTab

-# 制御セッションを切断表示する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSessionStatusTab::DispCtrlDisConStatus()
{
	TCHAR szMsg[256];

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_SESSIONDISCONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	m_csCtrlStatus.SetText(szMsg);
	// 1局モードの場合は制御をイネーブル
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		m_csCtrlStatus.SetTextColor(m_colorText, m_colorText);
		m_csCtrlStatus.SetBackColor(m_colorBkDisConnect, m_colorBkDisConnect);
	}

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONSTATUS_CONNECT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return;
	// 1局モードの場合は制御をイネーブル
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		m_buttonCtrl.EnableWindow(TRUE);
	}
	m_buttonCtrl.SetWindowText(szMsg);

	m_sessionStatus[eSessionType_Ctrl] = CSessionInfo::eDisconnect;
}


/*============================================================================*/
/*! CSessionStatusTab

-# 通知セッションを不明表示する

@param	なし
@retval	なし

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

-# 通知セッションを取得中表示する

@param	なし
@retval	なし

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

-# 通知セッションをエラー表示する

@param	なし
@retval	なし

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

-# 通知セッションを接続表示する

@param	なし
@retval	なし

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

-# 通知セッションを切断表示する

@param	なし
@retval	なし

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

-# 履歴セッションを不明表示する

@param	なし
@retval	なし

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

-# 履歴セッションを取得中表示する

@param	なし
@retval	なし

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

-# 履歴セッションをエラー表示する

@param	なし
@retval	なし

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

-# 履歴セッションを接続表示する

@param	なし
@retval	なし

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

-# 履歴セッションを切断表示する

@param	なし
@retval	なし

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

-# 監視セッションを切断する

@param	なし
@retval TRUE：OK / FALSE：NG

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

		// セッション状態を不明にしておく
		theApp.GetSatelliteData().GetSessionInfo(m_nStationID).SetSessionStatus(eSessionType_Monitor, CSessionInfo::eUnknown);
		// リモシェ実行
		int err = CSshShell::ExecShell(stParam);
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CSessionStatusTab"), _T("DisConnectMonitorSession"), _T("debug"), strDebug, nLogEx::debug);
		}
		if (err)
		{
			// 画面表示
			DispMonitorStatus();
		}
		else
		{
			// セッション状態取得
			GetSessionStatus(TRUE);
		}
	}

	return TRUE;
}


/*============================================================================*/
/*! CSessionStatusTab

-# 制御セッションを切断する

@param	なし
@retval TRUE：OK / FALSE：NG

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

		// セッション状態を不明にしておく
		theApp.GetSatelliteData().GetSessionInfo(m_nStationID).SetSessionStatus(eSessionType_Ctrl, CSessionInfo::eUnknown);
		// リモシェ実行
		int err = CSshShell::ExecShell(stParam);
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CSessionStatusTab"), _T("DisConnectCtrlSession"), _T("debug"), strDebug, nLogEx::debug);
		}
		if (err)
		{
			// 画面表示
			DispMonitorStatus();
		}
		else
		{
			// セッション状態取得
			GetSessionStatus(TRUE);
		}
	}

	return FALSE;
}


/*============================================================================*/
/*! CSessionStatusTab

-# 通知セッションを切断する

@param	なし
@retval TRUE：OK / FALSE：NG

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

		// セッション状態を不明にしておく
		theApp.GetSatelliteData().GetSessionInfo(m_nStationID).SetSessionStatus(eSessionType_Notify, CSessionInfo::eUnknown);
		// リモシェ実行
		int err = CSshShell::ExecShell(stParam);
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CSessionStatusTab"), _T("DisConnectNotifySession"), _T("debug"), strDebug, nLogEx::debug);
		}
		if (err)
		{
			// 画面表示
			DispMonitorStatus();
		}
		else
		{
			// セッション状態取得
			GetSessionStatus(TRUE);
		}
	}

	return TRUE;
}


/*============================================================================*/
/*! CSessionStatusTab

-# 履歴セッションを切断する

@param	なし
@retval TRUE：OK / FALSE：NG

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

		// セッション状態を不明にしておく
		theApp.GetSatelliteData().GetSessionInfo(m_nStationID).SetSessionStatus(eSessionType_History, CSessionInfo::eUnknown);
		// リモシェ実行
		int err = CSshShell::ExecShell(stParam);
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CSessionStatusTab"), _T("DisConnectHistorySession"), _T("debug"), strDebug, nLogEx::debug);
		}
		if (err)
		{
			// 画面表示
			DispMonitorStatus();
		}
		else
		{
			// セッション状態取得
			GetSessionStatus(TRUE);
		}
	}

	return TRUE;
}


/*============================================================================*/
/*! CSessionStatusTab

-# 監視セッションを接続する

@param	なし
@retval TRUE：OK / FALSE：NG

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

	// セッション状態を不明にしておく
	theApp.GetSatelliteData().GetSessionInfo(m_nStationID).SetSessionStatus(eSessionType_Monitor, CSessionInfo::eUnknown);
	// リモシェ実行
	int err = CSshShell::ExecShell(stParam);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CSessionStatusTab"), _T("ConnectMonitorSession"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err)
	{
		// 画面表示
		DispMonitorStatus();
	}
	else
	{
		// セッション状態取得
		GetSessionStatus(TRUE);
	}

	return TRUE;
}


/*============================================================================*/
/*! CSessionStatusTab

-# 制御セッションを接続する

@param	なし
@retval TRUE：OK / FALSE：NG

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

	// セッション状態を不明にしておく
	theApp.GetSatelliteData().GetSessionInfo(m_nStationID).SetSessionStatus(eSessionType_Ctrl, CSessionInfo::eUnknown);
	// リモシェ実行
	int err = CSshShell::ExecShell(stParam);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CSessionStatusTab"), _T("ConnectCtrlSession"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err)
	{
		// 画面表示
		DispMonitorStatus();
	}
	else
	{
		// セッション状態取得
		GetSessionStatus(TRUE);
	}

	return TRUE;
}


/*============================================================================*/
/*! CSessionStatusTab

-# 通知セッションを接続する

@param	なし
@retval TRUE：OK / FALSE：NG

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

	// セッション状態を不明にしておく
	theApp.GetSatelliteData().GetSessionInfo(m_nStationID).SetSessionStatus(eSessionType_Notify, CSessionInfo::eUnknown);
	// リモシェ実行
	int err = CSshShell::ExecShell(stParam);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CSessionStatusTab"), _T("ConnectNotifySession"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err)
	{
		// 画面表示
		DispMonitorStatus();
	}
	else
	{
		// セッション状態取得
		GetSessionStatus(TRUE);
	}

	return TRUE;
}


/*============================================================================*/
/*! CSessionStatusTab

-# 履歴セッションを接続する

@param	なし
@retval TRUE：OK / FALSE：NG

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

	// セッション状態を不明にしておく
	theApp.GetSatelliteData().GetSessionInfo(m_nStationID).SetSessionStatus(eSessionType_History, CSessionInfo::eUnknown);
	// リモシェ実行
	int err = CSshShell::ExecShell(stParam);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CSessionStatusTab"), _T("ConnectHistorySession"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err)
	{
		// 画面表示
		DispMonitorStatus();
	}
	else
	{
		// セッション状態取得
		GetSessionStatus(TRUE);
	}

	return TRUE;
}


/*============================================================================*/
/*! CSessionStatusTab

-# 監視セッションの接続切断ボタン押下時に呼ばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSessionStatusTab::OnBnClickedSessionstatusMonitorButton()
{
	// 監視セッション状態を見る
	switch (m_sessionStatus[eSessionType_Monitor])
	{
	case CSessionInfo::eDisconnect:
		// 接続する
		ConnectMonitorSession();
		break;

	case CSessionInfo::eConnect:
		// 切断する
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

-# 制御セッションの接続切断ボタン押下時に呼ばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSessionStatusTab::OnBnClickedSessionstatusCtrlButton()
{
	// 制御セッション状態を見る
	switch (m_sessionStatus[eSessionType_Ctrl])
	{
	case CSessionInfo::eDisconnect:
		// 接続する
		ConnectCtrlSession();
		break;

	case CSessionInfo::eConnect:
		// 切断する
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

-# 通知セッションの接続切断ボタン押下時に呼ばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSessionStatusTab::OnBnClickedSessionstatusNotifyButton()
{
	// 通知セッション状態を見る
	switch (m_sessionStatus[eSessionType_Notify])
	{
	case CSessionInfo::eDisconnect:
		// 接続する
		ConnectNotifySession();
		break;

	case CSessionInfo::eConnect:
		// 切断する
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

-# 履歴セッションの接続切断ボタン押下時に呼ばれる

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSessionStatusTab::OnBnClickedSessionstatusHistoryButton()
{
	// 履歴セッション状態を見る
	switch (m_sessionStatus[eSessionType_History])
	{
	case CSessionInfo::eDisconnect:
		// 接続する
		ConnectHistorySession();
		break;

	case CSessionInfo::eConnect:
		// 切断する
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

-# 描画時に呼ばれ、指定コントロールの背景を塗りつぶすブラシのハンドルを返す

@param	pDC：子ウィンドウのディスプレイコンテキストのポインタ
@param	pWnd：色を格納されたコントロールのポインタ
@param	nCtrlColor：コントロールの種類
@retval	コントロールの背景を塗りつぶすブラシのハンドル

*/
/*============================================================================*/
HBRUSH CSessionStatusTab::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogBase::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
		// ダイアログの背景色
		if (pWnd == this)
		{
			hbr = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
		}
		break;

	default:
		break;
	}

	// TODO: 既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}


/*============================================================================*/
/*! CSessionStatusTab

-# セッション情報を取得する

@param	bWaitComplete：TRUE:シェル終了を待つ（デフォルト） / FALSE:シェル終了を待たない
@retval	なし

*/
/*============================================================================*/
void CSessionStatusTab::GetSessionStatus(BOOL bWaitComplete /*= TRUE*/)
{
	// セッション状態をリモートシェルで取得する
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = STAT_SDTP_SH;
	CString arg;

	if (theApp.GetSelectStationMode() == eStation_1)
	{
		// 1局の場合は局名と全セッション(all)セット
		int nStation = theApp.GetSelectStation();
		arg.Format(_T("%s all"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower());
		stParam.strRemShellParam.Format(arg);
	}
	else
	{
		// 4局の場合はallと全セッション(all)をセット
		stParam.strRemShellParam.Format(_T("all all"));
	}

	// リモシェ実行
	int err = CSshShell::ExecShell(stParam, bWaitComplete);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShellThread"), __FILEW__, __LINE__);
		CLogTraceEx::Write(_T(""), _T("CSessionStatusTab"), _T("ConnectCtrlSession"), _T("debug"), strDebug, nLogEx::debug);
	}
	if (err)
	{
		// 画面表示
		UpdateSessionDisplay();
	}
	else
	{
		// セッション状態を取得中にしておく
		theApp.GetSatelliteData().GetSessionInfo(m_nStationID).SetSessionStatus(eSessionType_Num, CSessionInfo::eGettingNow);
		if (bWaitComplete)
		{
			// セッション状態ファイル読み込み
			if (theApp.GetSatelliteData().ReadSessionStatusFile())
			{
				// 画面表示
				UpdateSessionDisplay();
			}
		}
	}
}


/*============================================================================*/
/*! CSessionStatusTab

-# セッション情報表示を更新する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSessionStatusTab::UpdateSessionDisplay()
{
	// 画面表示
	DispMonitorStatus();
	DispCtrlStatus();
	DispNotifyStatus();
	DispHistoryStatus();

	UpdateData(FALSE);
}

