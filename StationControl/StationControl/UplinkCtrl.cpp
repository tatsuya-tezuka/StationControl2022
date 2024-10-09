/*============================================================================*/
/*! UplinkCtrl.cpp

-# 衛星捕捉画面　アップリンク制御
*/
/*============================================================================*/
// UplinkCtrl.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "UplinkCtrl.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"
#include "SatelliteCapture.h"

// CUplinkCtrl ダイアログ

IMPLEMENT_DYNAMIC(CUplinkCtrl, CDialogBase)

/*============================================================================*/
/*! CUplinkCtrl

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CUplinkCtrl::CUplinkCtrl(CWnd* pParent /*=NULL*/)
	: CDialogBase(CUplinkCtrl::IDD, pParent)
	, m_strRfTripMonitor(_T(""))
	, m_strRealAz(_T(""))
	, m_strAzSkyLIne(_T(""))
	, m_rfTripMonitor(eRfTripMonitorType_Null)
{

}

/*============================================================================*/
/*! CUplinkCtrl

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CUplinkCtrl::~CUplinkCtrl()
{
}

void CUplinkCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UPLINKCTRL_RFTRIPMONITOR_COMBO, m_CCBoxRfTripMonitor);
	DDX_Text(pDX, IDC_UPLINKCTRL_RFTRIPNONITOR_EDIT, m_strRfTripMonitor);
	DDX_Text(pDX, IDC_UPLINKCTRL_REALAZ_EDIT, m_strRealAz);
	DDX_Text(pDX, IDC_UPLINKCTRL_AZSKYLINE_EDIT, m_strAzSkyLIne);
}


BEGIN_MESSAGE_MAP(CUplinkCtrl, CDialogBase)
	ON_BN_CLICKED(IDC_UPLINKCTRL_RFTRIPMONITOR_BUTTON, &CUplinkCtrl::OnBnClickedUplinkctrlRftripmonitorButton)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CUplinkCtrl メッセージ ハンドラー


/*============================================================================*/
/*! CUplinkCtrl

-# 設定ボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CUplinkCtrl::OnBnClickedUplinkctrlRftripmonitorButton()
{
	CSatelliteCapture* pWnd = (CSatelliteCapture*)GetParent();
	if (pWnd == NULL)
		return;

	// リモートシェルで局運用管制サーバに通知
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = SEND_NOTICE_SH;
	CString arg;
	unsigned char binary = 0;
	CString strAuto;
	CString strManual;

	if (strAuto.LoadString(IDS_DOWNLINK_CTRLMODE_AUTO) != TRUE)
		return;
	if (strManual.LoadString(IDS_DOWNLINK_CTRLMODE_MANUAL) != TRUE)
		return;
	CString strCombo = GetRfTripMonitorStr();
	UINT nCtrlMod = pWnd->GetCtrlMode();
	unsigned char cmd;
	unsigned char rng;
	unsigned char downlink = pWnd->GetDownLinkBinary();
	unsigned char uplink = 0;
	unsigned char way = pWnd->GetWayBinary();

	if (strCombo.CollateNoCase(strAuto) == 0)
	{
		// 自動
		uplink = 1;
	}
	else if (strCombo.CollateNoCase(strManual) == 0)
	{
		// 手動
		uplink = 0;
	}

	// CMDとRNGはアップリンクに合わせる
	cmd = uplink;
	rng = uplink;

	binary = way << eSatellieMode_Way |						/* 1Way/2Way/Dup設定 */
		uplink << eSatellieMode_UpLink |					/* アップリンク制御設定 */
		downlink << eSatellieMode_DownLink |				/* ダウンリンク制御設定 */
		rng << eSatellieMode_RNG |							/* RNG制御設定 */
		cmd << eSatellieMode_CMD |							/* CMD制御設定 */
		1 << eSatellieMode_AntStandBy |						/* ANT待ち受け指向種別 */
		1 << eSatellieMode_AutoManual;						/* 自動捕捉/手動捕捉種別 */

	int nStation = theApp.GetSelectStation();
	arg.Format(_T("%s %d 0x%02x"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_ACQMOD, binary);

	stParam.strRemShellParam.Format(arg);

	int err = CSshShell::ExecShell(stParam);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnBnClickedDownlinkcaptureCtrlmodeButton"), _T("debug"), strDebug, nLogEx::debug);
	}
	if (err == 0)
	{
		// 何もしない
	}

#ifdef _DEBUG
	if (uplink == 1)
	{
		// 自動
		m_rfTripMonitor = eRfTripMonitorType_Auto;
	}
	else
	{
		// 手動
		m_rfTripMonitor = eRfTripMonitorType_Manual;
	}
	CreateDlgItems();
#endif
}


/*============================================================================*/
/*! CUplinkCtrl

-# ダイアログ初期化処理

@param  なし
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
BOOL CUplinkCtrl::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_rfTripMonitor = eRfTripMonitorType_Null;

	// ダイアログアイテムを生成する
	if (CreateDlgItems() == FALSE)
		return FALSE;

	// 監視データ取得
	if (UpdateKanshiData() == FALSE)
		return FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CUplinkCtrl

-# 描画時に呼ばれ、指定コントロールの背景を塗りつぶすブラシのハンドルを返す

@param	pDC：子ウィンドウのディスプレイコンテキストのポインタ
@param	pWnd：色を格納されたコントロールのポインタ
@param	nCtrlColor：コントロールの種類
@retval	コントロールの背景を塗りつぶすブラシのハンドル

*/
/*============================================================================*/
HBRUSH CUplinkCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogBase::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		// 静的テキスト背景色
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(DC_BRUSH);
		hbr = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
		break;

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
/*! CUplinkCtrl

-# ダイアログアイテムを生成する

@param	なし
@retval	TRUE: 生成成功 / FALSE: 生成失敗

*/
/*============================================================================*/
BOOL CUplinkCtrl::CreateDlgItems()
{
	TCHAR szMsg[256];

	// コンボボックスのクリア
	m_CCBoxRfTripMonitor.ResetContent();

	// エディットコントロール初期化
	// コンボボックス初期化
	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_DOWNLINK_CTRLMODE_AUTO, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	if (m_rfTripMonitor == eRfTripMonitorType_Auto)
	{
		m_strRfTripMonitor = CString(szMsg);
	}
	else
	{
		m_CCBoxRfTripMonitor.InsertString(-1, szMsg);
	}

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_DOWNLINK_CTRLMODE_MANUAL, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	if (m_rfTripMonitor == eRfTripMonitorType_Manual)
	{
		m_strRfTripMonitor = CString(szMsg);
	}
	else
	{
		m_CCBoxRfTripMonitor.InsertString(-1, szMsg);
	}

	m_CCBoxRfTripMonitor.SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;
}


/*============================================================================*/
/*! CUplinkCtrl

-# コンボボックスのRF TRIP監視文字列を取得する

@param	なし
@retval	制御モード文字列

*/
/*============================================================================*/
CString CUplinkCtrl::GetRfTripMonitorStr()
{
	CString str;
	UpdateData(TRUE);
	int nIndex = m_CCBoxRfTripMonitor.GetCurSel();
	if (nIndex < 0)
		return str;
	m_CCBoxRfTripMonitor.GetLBText(nIndex, str);

	return str;
}


/*============================================================================*/
/*! CUplinkCtrl

-# RF TRIP監視を設定する

@param	rfTripMonitorType：制御モード
@retval	なし

*/
/*============================================================================*/
void CUplinkCtrl::SetRfTripMonitor(char szRfTripMode[])
{
	eRfTripMonitorType rfTripMonitor;

	CString str(szRfTripMode);
	if (str.CompareNoCase(_T("自動")) == 0)
	{
		rfTripMonitor = eRfTripMonitorType_Auto;
	}
	else
	{
		rfTripMonitor = eRfTripMonitorType_Manual;
	}

	if (m_rfTripMonitor != rfTripMonitor)
	{
		m_rfTripMonitor = rfTripMonitor;
		CreateDlgItems();
	}
}


/*============================================================================*/
/*! CUplinkCtrl

-# RF TRIP監視をNULL設定する

@param	rfTripMonitorType：制御モード
@retval	なし

*/
/*============================================================================*/
void CUplinkCtrl::SetRfTripMonitorNull()
{
	eRfTripMonitorType rfTripMonitor = eRfTripMonitorType_Null;
	if (m_rfTripMonitor != rfTripMonitor)
	{
		m_rfTripMonitor = rfTripMonitor;
		CreateDlgItems();
	}
}


/*============================================================================*/
/*! CUplinkCtrl

-# 監視からRF-TRIPモードとREAL AZ, AZ方向のSKYLINEを取得して更新する

@param	なし
@retval	TRUE:成功 / FALSE:失敗

*/
/*============================================================================*/
BOOL CUplinkCtrl::UpdateKanshiData()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT sta = theApp.GetSelectStation();

	char		ca_rfTripMode[30];		/* RF-TRIPモード   */
	double		db_realAZ;				/* REAL AZ */

	/* RF-TRIPモード取得 */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(CTRL_P_CAP_UPLINK));
	if (obs_adr == NULL)
	{
#ifdef _DEBUG
#else
		SetRfTripMonitorNull();
#endif
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		strcpy_s(ca_rfTripMode, 30, obs_adr->sz_sts_name);
		SetRfTripMonitor(ca_rfTripMode);
	}
	else
	{
		SetRfTripMonitorNull();
	}

	/* REAL AZ取得 */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(REAL_AZ));
	if (obs_adr == NULL)
	{
		m_strRealAz.Format(_T(""));
		return FALSE;
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		db_realAZ = obs_adr->d_data;
		m_strRealAz.Format(_T("%0.1f°"), db_realAZ);
	}
	else
	{
		m_strRealAz.Format(_T(""));
		return FALSE;
	}

	CGraphMutex::Lock(eRadar);

	/* AZ方向のSKYLINE */
	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(theApp.GetSelectStation());
	if (pCtrlMonData == NULL)
	{
		m_strAzSkyLIne.Format(_T(""));
	}
	else if (db_realAZ >= 0 && db_realAZ < 360)
	{
		if (pCtrlMonData->m_queAzElSkyline.size() > db_realAZ)
		{
			double db_skylineEL = pCtrlMonData->m_queAzElSkyline[(UINT)db_realAZ].d_EL;
			m_strAzSkyLIne.Format(_T("%0.1f°"), db_skylineEL);
		}
	}
	else
	{
		m_strAzSkyLIne.Format(_T(""));
	}

	CGraphMutex::Unlock(eRadar);

	UpdateData(FALSE);

	return TRUE;
}


/*============================================================================*/
/*! CSatelliteCapture

-# アップリンク制御バイナリを取得する

@param	なし
@retval	制御モード

*/
/*============================================================================*/
unsigned char CUplinkCtrl::GetUpLinkBinary()
{
	unsigned char binary = 1;

	switch (m_rfTripMonitor)
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
