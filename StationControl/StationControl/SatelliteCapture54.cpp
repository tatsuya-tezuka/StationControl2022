/*============================================================================*/
/*! SatelliteCapture54.cpp

-# 衛星捕捉画面(54m)
*/
/*============================================================================*/
// SatelliteCapture54.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "SatelliteCapture54.h"
#include "afxdialogex.h"


// CSatelliteCapture54 ダイアログ

IMPLEMENT_DYNAMIC(CSatelliteCapture54, CSatelliteCapture)

/*============================================================================*/
// コンストラクタ
/*============================================================================*/
CSatelliteCapture54::CSatelliteCapture54(CWnd* pParent /*=NULL*/)
	: CSatelliteCapture(CSatelliteCapture54::IDD, pParent)
{

}

/*============================================================================*/
// デストラクタ
/*============================================================================*/
CSatelliteCapture54::~CSatelliteCapture54()
{
}

void CSatelliteCapture54::DoDataExchange(CDataExchange* pDX)
{
	CSatelliteCapture::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SATELLITECAPTURE_TAB1, m_tabCtrl);
}

BEGIN_MESSAGE_MAP(CSatelliteCapture54, CSatelliteCapture)
	ON_COMMAND(ID_MENU_CLOSE_RC2, &CSatelliteCapture54::OnMenuClose)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SATELLITECAPTURE_TAB1, &CSatelliteCapture54::OnTcnSelchangeSatellitecaptureTab1)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CSatelliteCapture54 メッセージ ハンドラー

/*============================================================================*/
// ファイルメニューの閉じるから呼び出される
/*============================================================================*/
void CSatelliteCapture54::OnMenuClose()
{
	//	OnOK();
	ShowWindow(SW_HIDE);
}

/*============================================================================*/
// ダイアログ初期化処理
/*============================================================================*/
BOOL CSatelliteCapture54::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	TCHAR szMsg[256];
	CRect rect;
	m_tabCtrl.GetClientRect(&rect);
	rect.top += nTabTopOffset;
	rect.left += nTabLeftOffset;

	// タブ初期化
	if (!m_antStanbyOriented.Create(CAntStanbyOriented::IDD, this))
		return FALSE;
	m_antStanbyOriented.MoveWindow(&rect, FALSE);

	if (!m_downlinkCapture.Create(CDownlinkCapture::IDD, this))
		return FALSE;
	m_downlinkCapture.MoveWindow(&rect, FALSE);

	if (!m_uplinkCtrl.Create(CUplinkCtrl::IDD, this))
		return FALSE;
	m_uplinkCtrl.MoveWindow(&rect, FALSE);

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SATELLITECAPTURE_ANTSTANBYORIENTED, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	m_tabCtrl.InsertItem(0, szMsg);

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SATELLITECAPTURE_DOWNLINKCAPTURE, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	m_tabCtrl.InsertItem(1, szMsg);

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SATELLITECAPTURE_UPLINKCTRL, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	m_tabCtrl.InsertItem(2, szMsg);

	OnTcnSelchangeSatellitecaptureTab1(0, 0);

	UpdateData(FALSE);

	SetTimer(1, 1000, NULL);

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
// タブ選択変更時時に呼ばれる
/*============================================================================*/
void CSatelliteCapture54::OnTcnSelchangeSatellitecaptureTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int sel = m_tabCtrl.GetCurSel();

	switch (sel)
	{
	case 0:
		m_antStanbyOriented.ShowWindow(SW_SHOW);
		m_downlinkCapture.ShowWindow(SW_HIDE);
		m_uplinkCtrl.ShowWindow(SW_HIDE);
		break;

	case 1:
		m_antStanbyOriented.ShowWindow(SW_HIDE);
		m_downlinkCapture.ShowWindow(SW_SHOW);
		m_uplinkCtrl.ShowWindow(SW_HIDE);
		break;

	case 2:
	default:
		m_antStanbyOriented.ShowWindow(SW_HIDE);
		m_downlinkCapture.ShowWindow(SW_HIDE);
		m_uplinkCtrl.ShowWindow(SW_SHOW);
		break;
	}

	if (pResult)
		*pResult = 0;
}

/*============================================================================*/
// Windowプロシージャ呼び出し
/*============================================================================*/
LRESULT CSatelliteCapture54::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if( (message == eMessage_UpdateShareFile) || (message == eMessage_UpdateForecastFilePath) )
	{
		m_antStanbyOriented.PostMessageW(message, wParam, lParam);
	}

	return CDialogBase::WindowProc(message, wParam, lParam);
}

/*============================================================================*/
// タイマイベントで呼び出される
/*============================================================================*/
void CSatelliteCapture54::OnTimer(UINT_PTR nIDEvent)
{
	m_downlinkCapture.UpdateKanshiData();
	m_uplinkCtrl.UpdateKanshiData();
	UpdateKanshiData();

	CDialogBase::OnTimer(nIDEvent);
}

/*============================================================================*/
// ウィンドウ破棄時に呼び出される
/*============================================================================*/
void CSatelliteCapture54::OnDestroy()
{
	CDialogBase::OnDestroy();

	KillTimer(1);
}

