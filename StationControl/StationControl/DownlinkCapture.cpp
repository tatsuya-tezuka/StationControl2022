/*============================================================================*/
/*! DownlinkCapture.cpp

-# 衛星捕捉画面　ダウンリンク捕捉
*/
/*============================================================================*/
// DownlinkCapture.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "DownlinkCapture.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"
#include "SatelliteCapture.h"

// CDownlinkCapture ダイアログ

IMPLEMENT_DYNAMIC(CDownlinkCapture, CDialogBase)

/*============================================================================*/
/*! DownlinkCapture

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CDownlinkCapture::CDownlinkCapture(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDownlinkCapture::IDD, pParent)
	, m_strRcvFreqMode(_T(""))
	, m_strCtrlMode(_T(""))
	, m_rcvFreqMode(eRcvFreqModeType_Other)
	, m_ctrlMode(eCtrlModeType_Null)
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
CDownlinkCapture::CDownlinkCapture(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialogBase(IDD, pParent)
{

}

/*============================================================================*/
/*! CDownlinkCapture

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CDownlinkCapture::~CDownlinkCapture()
{
}

void CDownlinkCapture::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DOWNLINKCAPTURE_RCVFREQMODE_COMBO, m_CCBoxRcvFreqMode);
	DDX_Control(pDX, IDC_DOWNLINKCAPTURE_CTRLMODE_COMBO, m_CCBoxCtrlMode);
	DDX_Text(pDX, IDC_DOWNLINKCAPTURE_RCVFREQMODE_EDIT, m_strRcvFreqMode);
	DDX_Text(pDX, IDC_DOWNLINKCAPTURE_CTRLMODE_EDIT, m_strCtrlMode);
}


BEGIN_MESSAGE_MAP(CDownlinkCapture, CDialogBase)
	ON_BN_CLICKED(IDC_DOWNLINKCAPTURE_RCVFREQMODE_BUTTON, &CDownlinkCapture::OnBnClickedDownlinkcaptureRcvfreqmodeButton)
	ON_BN_CLICKED(IDC_DOWNLINKCAPTURE_CTRLMODE_BUTTON, &CDownlinkCapture::OnBnClickedDownlinkcaptureCtrlmodeButton)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_DOWNLINKCAPTURE_CTRLMODE_COMBO, &CDownlinkCapture::OnCbnSelchangeDownlinkcaptureCtrlmodeCombo)
END_MESSAGE_MAP()


// CDownlinkCapture メッセージ ハンドラー


/*============================================================================*/
/*! CDownlinkCapture

-# 受信周波数モードのボタンクリック時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CDownlinkCapture::OnBnClickedDownlinkcaptureRcvfreqmodeButton()
{
	// リモートシェルで局運用管制サーバに通知
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = SEND_NOTICE_SH;
	CString arg;
	unsigned char binary = GetRcvFreqModNotifyData();

	int nStation = theApp.GetSelectStation();
	arg.Format(_T("%s %d 0x%02x"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_ACQMOD, binary);

	stParam.strRemShellParam.Format(arg);

	int err = CSshShell::ExecShell(stParam);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnBnClickedDownlinkcaptureRcvfreqmodeButton"), _T("debug"), strDebug, nLogEx::debug);
	}
	if (err == 0)
	{
		// 何もしない
	}

#ifdef _DEBUG
	CString str1Way;
	CString str2Way;
	CString strDup;
	if (str1Way.LoadString(IDS_DOWNLINK_RCVFREQMODE_1WAY) != TRUE)
		return;
	if (str2Way.LoadString(IDS_DOWNLINK_RCVFREQMODE_2WAY) != TRUE)
		return;
	if (strDup.LoadString(IDS_DOWNLINK_RCVFREQMODE_DUP) != TRUE)
		return;
	// コンボボックスから受信周波数モード取得
	CString strCombo = GetRcvFreqModeComboStr();

	// WAY
	if (strCombo.CollateNoCase(str1Way) == 0)
	{
		m_rcvFreqMode = eRcvFreqModeType_1Way;
	}
	else if (strCombo.CollateNoCase(str2Way) == 0)
	{
		m_rcvFreqMode = eRcvFreqModeType_2Way;
	}
	else if (strCombo.CollateNoCase(strDup) == 0)
	{
		m_rcvFreqMode = eRcvFreqModeType_Dup;
	}
	else
	{
		m_rcvFreqMode = eRcvFreqModeType_Other;
	}
	CreateDlgRcvFreqMode();
#endif
}


/*============================================================================*/
/*! CDownlinkCapture

-# 制御モードのボタンクリック時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CDownlinkCapture::OnBnClickedDownlinkcaptureCtrlmodeButton()
{
	// リモートシェルで局運用管制サーバに通知
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = SEND_NOTICE_SH;
	CString arg;

	unsigned char binary = 0;
	CSatelliteCapture* pWnd = (CSatelliteCapture*)GetParent();
	if (pWnd == NULL)
		return;

	// アップリンク, CMD, RNG, WAY制御取得
	unsigned char cmd = pWnd->GetCmdBinary();			// ダウンリンクは監視からCMD取得
	unsigned char rng = pWnd->GetRngBinary();			// ダウンリンクは監視からRNG取得
	unsigned char downlink = GetCtrlModeFromCombo();
	unsigned char uplink = pWnd->GetUpLinkBinary();
	unsigned char way = GetWayBinary();

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
	if (downlink == 1)
	{
		// 自動
		m_ctrlMode = eCtrlModeType_Auto;
	}
	else
	{
		// 手動
		m_ctrlMode = eCtrlModeType_Manual;
	}
	CreateDlgCtrlMode();
#endif

}


/*============================================================================*/
/*! CDownlinkCapture

-# ダイアログ初期化処理

@param  なし
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
BOOL CDownlinkCapture::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_rcvFreqMode = eRcvFreqModeType_Other;
	m_ctrlMode = eCtrlModeType_Null;

	// ダイアログアイテムを生成する
	if (CreateDlgItems() == FALSE)
		return FALSE;

	// 監視データ取得
	(void)UpdateKanshiData();

	// 受信周波数モードのコントロールの表示、非表示
	ShowRcvFreqMode();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CDownlinkCapture

-# 描画時に呼ばれ、指定コントロールの背景を塗りつぶすブラシのハンドルを返す

@param	pDC：子ウィンドウのディスプレイコンテキストのポインタ
@param	pWnd：色を格納されたコントロールのポインタ
@param	nCtrlColor：コントロールの種類
@retval	コントロールの背景を塗りつぶすブラシのハンドル

*/
/*============================================================================*/
HBRUSH CDownlinkCapture::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
/*! CDownlinkCapture

-# ダイアログアイテムを生成する

@param	なし
@retval	TRUE: 生成成功 / FALSE: 生成失敗

*/
/*============================================================================*/
BOOL CDownlinkCapture::CreateDlgItems()
{
	if (CreateDlgRcvFreqMode() && CreateDlgCtrlMode())
	{
		return TRUE;
	}

	return FALSE;
}


/*============================================================================*/
/*! CDownlinkCapture

-# 受信周波数モードダイアログアイテムを生成する

@param	なし
@retval	TRUE: 生成成功 / FALSE: 生成失敗

*/
/*============================================================================*/
BOOL CDownlinkCapture::CreateDlgRcvFreqMode()
{
	TCHAR szMsg[256];

	// コンボボックスのクリア
	m_CCBoxRcvFreqMode.ResetContent();

	// エディットコントロール初期化
	// コンボボックス初期化
	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_DOWNLINK_RCVFREQMODE_1WAY, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	if (m_rcvFreqMode == eRcvFreqModeType_1Way)
	{
		m_strRcvFreqMode = CString(szMsg);
	}
	else
	{
		m_CCBoxRcvFreqMode.InsertString(-1, szMsg);
	}

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_DOWNLINK_RCVFREQMODE_2WAY, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	if (m_rcvFreqMode == eRcvFreqModeType_2Way)
	{
		m_strRcvFreqMode = CString(szMsg);
	}
	else
	{
		m_CCBoxRcvFreqMode.InsertString(-1, szMsg);
	}

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_DOWNLINK_RCVFREQMODE_DUP, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	if (m_rcvFreqMode == eRcvFreqModeType_Dup)
	{
		m_strRcvFreqMode = CString(szMsg);
	}
	else
	{
		m_CCBoxRcvFreqMode.InsertString(-1, szMsg);
	}

	if (m_rcvFreqMode == eRcvFreqModeType_Other)
	{
		m_strRcvFreqMode = CString(_T(""));
	}

	m_CCBoxRcvFreqMode.SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;
}


/*============================================================================*/
/*! CDownlinkCapture

-# 制御モードダイアログアイテムを生成する

@param	なし
@retval	TRUE: 生成成功 / FALSE: 生成失敗

*/
/*============================================================================*/
BOOL CDownlinkCapture::CreateDlgCtrlMode()
{
	TCHAR szMsg[256];

	// コンボボックスのクリア
	m_CCBoxCtrlMode.ResetContent();

	// エディットコントロール初期化
	// コンボボックス初期化
	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_DOWNLINK_CTRLMODE_AUTO, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	if (m_ctrlMode == eCtrlModeType_Auto)
	{
		m_strCtrlMode = CString(szMsg);
	}
	else
	{
		m_CCBoxCtrlMode.InsertString(-1, szMsg);
	}

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_DOWNLINK_CTRLMODE_MANUAL, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	if (m_ctrlMode == eCtrlModeType_Manual)
	{
		m_strCtrlMode = CString(szMsg);
	}
	else
	{
		m_CCBoxCtrlMode.InsertString(-1, szMsg);
	}

	// 監視を未受信時
	if (m_ctrlMode == eCtrlModeType_Null)
	{
		m_strCtrlMode = CString(_T(""));
	}

	m_CCBoxCtrlMode.SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;
}


/*============================================================================*/
/*! CDownlinkCapture

-# コンボボックスの受信周波数モード文字列を取得する

@param	なし
@retval	受信周波数モード文字列

*/
/*============================================================================*/
CString CDownlinkCapture::GetRcvFreqModeComboStr()
{
	CString str;
	UpdateData(TRUE);
	int nIndex = m_CCBoxRcvFreqMode.GetCurSel();
	if (nIndex < 0)
		return str;
	m_CCBoxRcvFreqMode.GetLBText(nIndex, str);

	return str;
}


/*============================================================================*/
/*! CDownlinkCapture

-# 受信周波数モードを設定する

@param	rcvFreqType：受信周波数モード
@retval	なし

*/
/*============================================================================*/
void CDownlinkCapture::SetRcvFreqMode(char szRcvFreqMode[])
{
	eRcvFreqModeType	rcvFreqMode;

	CString str(szRcvFreqMode);
	if (str.CompareNoCase(_T("1WAY")) == 0)
	{
		rcvFreqMode = eRcvFreqModeType_1Way;
	}
	else if (str.CompareNoCase(_T("2WAY")) == 0)
	{
		rcvFreqMode = eRcvFreqModeType_2Way;
	}
	else if (str.CompareNoCase(_T("DUP")) == 0)
	{
		rcvFreqMode = eRcvFreqModeType_Dup;
	}
	else
	{
		rcvFreqMode = eRcvFreqModeType_Other;
	}

	if (m_rcvFreqMode != rcvFreqMode)
	{
		m_rcvFreqMode = rcvFreqMode;
		CreateDlgRcvFreqMode();
	}
}


/*============================================================================*/
/*! CDownlinkCapture

-# コンボボックスの制御モード文字列を取得する

@param	なし
@retval	制御モード文字列

*/
/*============================================================================*/
CString CDownlinkCapture::GetCtrlModeComboStr()
{
	CString str;
	UpdateData(TRUE);
	int nIndex = m_CCBoxCtrlMode.GetCurSel();
	if (nIndex < 0)
		return str;
	m_CCBoxCtrlMode.GetLBText(nIndex, str);

	return str;
}


/*============================================================================*/
/*! CDownlinkCapture

-# 制御モードを設定する

@param	制御モード
@retval	なし

*/
/*============================================================================*/
void CDownlinkCapture::SetCtrlMode(char szCtrlMode[])
{
	eCtrlModeType ctrlMode;

	CString str(szCtrlMode);
	if (str.CompareNoCase(_T("自動")) == 0)
	{
		ctrlMode = eCtrlModeType_Auto;
	}
	else
	{
		ctrlMode = eCtrlModeType_Manual;
	}

	if (m_ctrlMode != ctrlMode)
	{ 
		m_ctrlMode = ctrlMode;
		CreateDlgCtrlMode();
	}
}


/*============================================================================*/
/*! CDownlinkCapture

-# 制御モードをNull設定する

@param	制御モード
@retval	なし

*/
/*============================================================================*/
void CDownlinkCapture::SetCtrlModeNull()
{
	eCtrlModeType ctrlMode = eCtrlModeType_Null;
	if (m_ctrlMode != ctrlMode)
	{
		m_ctrlMode = ctrlMode;
		CreateDlgCtrlMode();
	}
}


/*============================================================================*/
/*! CDownlinkCapture

-# 監視から受信周波数モードと制御モードを取得して更新する

@param	なし
@retval	TRUE:成功 / FALSE:失敗

*/
/*============================================================================*/
BOOL CDownlinkCapture::UpdateKanshiData()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT sta = theApp.GetSelectStation();

	char		ca_rcvFreqMode[30];		/* 受信周波数モード   */
	char		ca_ctrlMode[30];		/* 制御モード         */

	/* 受信周波数モード取得 */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(CTRL_P_CAP_WAY));
	if (obs_adr == NULL)
	{
#ifdef _DEBUG
#else
	SetRcvFreqMode("");
#endif
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		strcpy_s(ca_rcvFreqMode, 30, obs_adr->sz_sts_name);
		SetRcvFreqMode(ca_rcvFreqMode);
	}
	else
	{
		SetRcvFreqMode("");
	}

	/* 制御モード取得 */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(CTRL_P_CAP_RX));
	if (obs_adr == NULL)
	{
#ifdef _DEBUG
#else
		SetCtrlModeNull();
#endif
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		strcpy_s(ca_ctrlMode, 30, obs_adr->sz_sts_name);
		SetCtrlMode(ca_ctrlMode);
	}
	else
	{
		SetCtrlModeNull();
	}

	// 受信周波数モードのコントロールの表示、非表示
	ShowRcvFreqMode();

	return TRUE;
}


/*============================================================================*/
/*! CDownlinkCapture

-# 受信周波数モード通知データを取得する

@param	なし
@retval	通知データ

*/
/*============================================================================*/
unsigned char CDownlinkCapture::GetRcvFreqModNotifyData()
{
	unsigned char binary = 0;

	CSatelliteCapture* pWnd = (CSatelliteCapture*)GetParent();
	if (pWnd == NULL)
		return binary;

	// ダウンリンク、アップリンク, CMD, RNG, WAY制御取得
	unsigned char uplink = pWnd->GetUpLinkBinary();
	unsigned char cmd = pWnd->GetCmdBinary();
	unsigned char rng = pWnd->GetRngBinary();

	CString str1Way;
	CString str2Way;
	CString strDup;
	if (str1Way.LoadString(IDS_DOWNLINK_RCVFREQMODE_1WAY) != TRUE)
		return binary;
	if (str2Way.LoadString(IDS_DOWNLINK_RCVFREQMODE_2WAY) != TRUE)
		return binary;
	if (strDup.LoadString(IDS_DOWNLINK_RCVFREQMODE_DUP) != TRUE)
		return binary;
	// コンボボックスから受信周波数モード取得
	CString strCombo = GetRcvFreqModeComboStr();

	// WAY
	int way = 0;
	if (strCombo.CollateNoCase(str1Way) == 0)
	{
		way = 0;
	}
	else if (strCombo.CollateNoCase(str2Way) == 0)
	{
		way = 1;
	}
	else if (strCombo.CollateNoCase(strDup) == 0)
	{
		way = 2;
	}

	binary = way << eSatellieMode_Way |						/* 1Way/2Way/Dup設定 */
		uplink << eSatellieMode_UpLink |					/* アップリンク制御設定 */
		0 << eSatellieMode_DownLink |						/* ダウンリンク制御設定（手動固定） */
		rng << eSatellieMode_RNG |							/* RNG制御設定 */
		cmd << eSatellieMode_CMD |							/* CMD制御設定 */
		1 << eSatellieMode_AntStandBy |						/* ANT待ち受け指向種別 */
		1 << eSatellieMode_AutoManual;						/* 自動捕捉/手動捕捉種別 */

	return binary;
}


/*============================================================================*/
/*! CDownlinkCapture

-# 1WAY/2WAY/Dupデータを取得する

@param	なし
@retval	通知データ

*/
/*============================================================================*/
unsigned char CDownlinkCapture::GetWayBinary()
{
	unsigned char binary = 0;

	switch (m_rcvFreqMode)
	{
	case eRcvFreqModeType_1Way:
		binary = 0;
		break;
	case eRcvFreqModeType_2Way:
		binary = 1;
		break;
	case eRcvFreqModeType_Dup:
		binary = 2;
		break;
	default:
		break;
	}

	return binary;
}


/*============================================================================*/
/*! CDownlinkCapture

-# ダウンリンク制御バイナリを取得する

@param	なし
@retval	制御モード

*/
/*============================================================================*/
unsigned char CDownlinkCapture::GetDownLinkBinary()
{
	unsigned char binary = 1;

	switch (m_ctrlMode)
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
/*! CDownlinkCapture

-# 制御モードが選択変更された

@param	なし
@retval なし

*/
/*============================================================================*/
void CDownlinkCapture::OnCbnSelchangeDownlinkcaptureCtrlmodeCombo()
{
}


/*============================================================================*/
/*! CDownlinkCapture

-# 制御モードをコンボから取得

@param	なし
@retval 1:自動 / 0:手動

*/
/*============================================================================*/
unsigned char CDownlinkCapture::GetCtrlModeFromCombo()
{
	unsigned char downlink = 0;

	CString strAuto;
	CString strManual;

	if (strAuto.LoadString(IDS_DOWNLINK_CTRLMODE_AUTO) != TRUE)
		return downlink;
	if (strManual.LoadString(IDS_DOWNLINK_CTRLMODE_MANUAL) != TRUE)
		return downlink;

	CString strCombo = GetCtrlModeComboStr();
	if (strCombo.CollateNoCase(strAuto) == 0)
	{
		// 自動
		downlink = 1;
	}
	else if (strCombo.CollateNoCase(strManual) == 0)
	{
		// 手動
		downlink = 0;
	}

	return downlink;
}


/*============================================================================*/
/*! CDownlinkCapture

-# 受信周波数モードのコントロールの表示、非表示

@param	なし
@retval なし

*/
/*============================================================================*/
void CDownlinkCapture::ShowRcvFreqMode()
{
	CWnd* pButtonWnd = GetDlgItem(IDC_DOWNLINKCAPTURE_RCVFREQMODE_BUTTON);
	if (pButtonWnd == NULL)
		return;

//	if (GetCtrlModeFromCombo() == 1)
	if (m_ctrlMode == eCtrlModeType_Auto)
	{
		// 自動の場合、受信周波数モードのコンボと設定ボタンをディセーブル
		m_CCBoxRcvFreqMode.EnableWindow(FALSE);
		pButtonWnd->EnableWindow(FALSE);
	}
	else
	{
		// 手動の場合、受信周波数モードのコンボと設定ボタンをイネーブル
		m_CCBoxRcvFreqMode.EnableWindow(TRUE);
		pButtonWnd->EnableWindow(TRUE);
	}
}