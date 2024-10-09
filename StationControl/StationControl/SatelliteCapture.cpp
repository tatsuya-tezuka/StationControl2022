/*============================================================================*/
/*! SatelliteCapture.cpp

-# 衛星捕捉画面
*/
/*============================================================================*/
// SatelliteCapture.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "SatelliteCapture.h"
#include "afxdialogex.h"


// CSatelliteCapture ダイアログ

IMPLEMENT_DYNAMIC(CSatelliteCapture, CDialogBase)

/*============================================================================*/
/*! CSatelliteCapture

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CSatelliteCapture::CSatelliteCapture(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSatelliteCapture::IDD, pParent)
{

}

/*============================================================================*/
/*! CSatelliteCapture

-# コンストラクタ

@param	IDD：IDD
@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CSatelliteCapture::CSatelliteCapture(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialogBase(IDD, pParent)
{

}

/*============================================================================*/
/*! CSatelliteCapture

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CSatelliteCapture::~CSatelliteCapture()
{
}

void CSatelliteCapture::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SATELLITECAPTURE_TAB1, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(CSatelliteCapture, CDialogBase)
	ON_COMMAND(ID_MENU_CLOSE_RC2, &CSatelliteCapture::OnMenuClose)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SATELLITECAPTURE_TAB1, &CSatelliteCapture::OnTcnSelchangeSatellitecaptureTab1)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSatelliteCapture メッセージ ハンドラー


/*============================================================================*/
/*! CSatelliteCapture

-# ファイルメニューの閉じるから呼び出される

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSatelliteCapture::OnMenuClose()
{
	//	OnOK();
	ShowWindow(SW_HIDE);
}

/*============================================================================*/
/*! CSatelliteCapture

-# ダイアログ初期化処理

@param  なし
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
BOOL CSatelliteCapture::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	TCHAR szMsg[256];
	CRect rect;
	m_tabCtrl.GetClientRect(&rect);
	rect.top += nTabTopOffset;
	rect.left += nTabLeftOffset;

	// タブ初期化
//	if (!m_antStanbyOriented.Create(CAntStanbyOriented::IDD, this))
//		return FALSE;
//	m_antStanbyOriented.MoveWindow(&rect, FALSE);

	if (!m_downlinkCapture.Create(CDownlinkCapture::IDD, this))
		return FALSE;
	m_downlinkCapture.MoveWindow(&rect, FALSE);

	if (!m_uplinkCtrl.Create(CUplinkCtrl::IDD, this))
		return FALSE;
	m_uplinkCtrl.MoveWindow(&rect, FALSE);

//	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
//	if (!LoadString(GetModuleHandle(NULL), IDS_SATELLITECAPTURE_ANTSTANBYORIENTED, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
//		return FALSE;
//	m_tabCtrl.InsertItem(0, szMsg);

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SATELLITECAPTURE_DOWNLINKCAPTURE, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	m_tabCtrl.InsertItem(0, szMsg);

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SATELLITECAPTURE_UPLINKCTRL, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	m_tabCtrl.InsertItem(1, szMsg);

	OnTcnSelchangeSatellitecaptureTab1(0, 0);

	UpdateData(FALSE);

	SetTimer(1, 1000, NULL);

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CSatelliteCapture

-# タブ選択変更時時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし
*/
/*============================================================================*/
void CSatelliteCapture::OnTcnSelchangeSatellitecaptureTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int sel = m_tabCtrl.GetCurSel();

	switch (sel)
	{
//	case 0:
//		m_antStanbyOriented.ShowWindow(SW_SHOW);
//		m_downlinkCapture.ShowWindow(SW_HIDE);
//		m_uplinkCtrl.ShowWindow(SW_HIDE);
//		break;

	case 0:
//		m_antStanbyOriented.ShowWindow(SW_HIDE);
		m_downlinkCapture.ShowWindow(SW_SHOW);
		m_uplinkCtrl.ShowWindow(SW_HIDE);
		break;

	case 1:
	default:
//		m_antStanbyOriented.ShowWindow(SW_HIDE);
		m_downlinkCapture.ShowWindow(SW_HIDE);
		m_uplinkCtrl.ShowWindow(SW_SHOW);
		break;
	}

	if (pResult)
		*pResult = 0;
}


/*============================================================================*/
/*! CSatelliteCapture

-# Windowプロシージャ呼び出し

@param  message	：メッセージの識別子
@param  wParam	：メッセージの最初のパラメータ
@param  lParam	：メッセージの 2 番目のパラメータ
@retval 処理結果
*/
/*============================================================================*/
LRESULT CSatelliteCapture::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
//	if (message == eMessage_UpdateForecastFilePath)
//	{
//		m_antStanbyOriented.PostMessageW(eMessage_UpdateForecastFilePath, wParam, lParam);
//	}

	return CDialogBase::WindowProc(message, wParam, lParam);
}


/*============================================================================*/
/*! CSatelliteCapture

-# タイマイベントで呼び出される

@param	nIDEvent：イベントID
@retval	なし

*/
/*============================================================================*/
void CSatelliteCapture::OnTimer(UINT_PTR nIDEvent)
{
	m_downlinkCapture.UpdateKanshiData();
	m_uplinkCtrl.UpdateKanshiData();
	UpdateKanshiData();

	CDialogBase::OnTimer(nIDEvent);
}


/*============================================================================*/
/*! CSatelliteCapture

-# ウィンドウ破棄時に呼び出される

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSatelliteCapture::OnDestroy()
{
	CDialogBase::OnDestroy();

	KillTimer(1);
}

/*============================================================================*/
/*! CSatelliteCapture

-# 監視からCMDモードとRNGモードを取得して更新する

@param	なし
@retval	TRUE:成功 / FALSE:失敗

*/
/*============================================================================*/
BOOL CSatelliteCapture::UpdateKanshiData()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT sta = theApp.GetSelectStation();

	char		ca_CmdMode[30];		/* CMDモード */
	char		ca_RngMode[30];		/* RNGモード */

	/* CMDモード取得 */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(CTRL_P_CAP_CMD));
	if (obs_adr == NULL)
	{
		SetCmdMode("");
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		strcpy_s(ca_CmdMode, 30, obs_adr->sz_sts_name);
		SetCmdMode(ca_CmdMode);
	}
	else
	{
		SetCmdMode("");
	}

	/* RNGモード取得 */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(CTRL_P_CAP_RNG));
	if (obs_adr == NULL)
	{
		SetRngMode("");
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		strcpy_s(ca_RngMode, 30, obs_adr->sz_sts_name);
		SetRngMode(ca_RngMode);
	}
	else
	{
		SetRngMode("");
	}

	return TRUE;
}


/*============================================================================*/
/*! CSatelliteCapture

-# CMD制御モードを設定する

@param	制御モード
@retval	なし

*/
/*============================================================================*/
void CSatelliteCapture::SetCmdMode(char szCtrlMode[])
{
	eCtrlModeType ctrlMode;

	CString str(szCtrlMode);
	if (str.CompareNoCase(_T("自動")) == 0)
	{
		ctrlMode = eCtrlModeType_Auto;
	}
	else if (str.CompareNoCase(_T("手動")) == 0)
	{
		ctrlMode = eCtrlModeType_Manual;
	}
	else
	{
		ctrlMode = eCtrlModeType_Null;
	}

	if (m_CmdMode != ctrlMode)
	{
		m_CmdMode = ctrlMode;
	}
}


/*============================================================================*/
/*! CSatelliteCapture

-# RNG制御モードを設定する

@param	制御モード
@retval	なし

*/
/*============================================================================*/
void CSatelliteCapture::SetRngMode(char szCtrlMode[])
{
	eCtrlModeType ctrlMode;

	CString str(szCtrlMode);
	if (str.CompareNoCase(_T("自動")) == 0)
	{
		ctrlMode = eCtrlModeType_Auto;
	}
	else if (str.CompareNoCase(_T("手動")) == 0)
	{
		ctrlMode = eCtrlModeType_Manual;
	}
	else
	{
		ctrlMode = eCtrlModeType_Null;
	}

	if (m_RngMode != ctrlMode)
	{
		m_RngMode = ctrlMode;
	}
}


/*============================================================================*/
/*! CSatelliteCapture

-# CMD制御バイナリを取得する

@param	なし
@retval	制御モード

*/
/*============================================================================*/
unsigned char CSatelliteCapture::GetCmdBinary()
{
	unsigned char binary = 1;

	switch (m_CmdMode)
	{
	case eCtrlModeType_Auto:
		binary = 1;
		break;
	case eCtrlModeType_Manual:
		binary = 0;
		break;
	default:
		break;
	}

	return binary;
}


/*============================================================================*/
/*! CSatelliteCapture

-# RNG制御バイナリを取得する

@param	なし
@retval	制御モード

*/
/*============================================================================*/
unsigned char CSatelliteCapture::GetRngBinary()
{
	unsigned char binary = 1;

	switch (m_RngMode)
	{
	case eCtrlModeType_Auto:
		binary = 1;
		break;
	case eCtrlModeType_Manual:
		binary = 0;
		break;
	default:
		break;
	}

	return binary;
}

