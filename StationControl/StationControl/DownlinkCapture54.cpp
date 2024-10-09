/*============================================================================*/
/*! DownlinkCapture54.cpp

-# 衛星捕捉画面　ダウンリンク捕捉(54m)
*/
/*============================================================================*/
// DownlinkCapture54.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "DownlinkCapture54.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"
#include "SatelliteCapture54.h"

// CDownlinkCapture54 ダイアログ

IMPLEMENT_DYNAMIC(CDownlinkCapture54, CDownlinkCapture)

/*============================================================================*/
// コンストラクタ
/*============================================================================*/
CDownlinkCapture54::CDownlinkCapture54(CWnd* pParent /*=NULL*/)
	: CDownlinkCapture(CDownlinkCapture54::IDD, pParent)
	, m_strBandSelect(_T(""))
	, m_bandSelect(eBandSelectType_Num)
{

}

/*============================================================================*/
// デストラクタ
/*============================================================================*/
CDownlinkCapture54::~CDownlinkCapture54()
{

}

void CDownlinkCapture54::DoDataExchange(CDataExchange* pDX)
{
	CDownlinkCapture::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DOWNLINKCAPTURE_BANDSELECT_COMBO, m_CCBoxBandSelect);
	DDX_Text(pDX, IDC_DOWNLINKCAPTURE_BANDSELECT_EDIT, m_strBandSelect);
}

BEGIN_MESSAGE_MAP(CDownlinkCapture54, CDownlinkCapture)
	ON_BN_CLICKED(IDC_DOWNLINKCAPTURE_RCVFREQMODE_BUTTON, &CDownlinkCapture::OnBnClickedDownlinkcaptureRcvfreqmodeButton)
	ON_BN_CLICKED(IDC_DOWNLINKCAPTURE_CTRLMODE_BUTTON, &CDownlinkCapture::OnBnClickedDownlinkcaptureCtrlmodeButton)
	ON_BN_CLICKED(IDC_DOWNLINKCAPTURE_BANDSELECT_BUTTON, &CDownlinkCapture54::OnBnClickedDownlinkcaptureBandSelectButton)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CDownlinkCapture54 メッセージ ハンドラー

/*============================================================================*/
// ダイアログ初期化処理
/*============================================================================*/
BOOL CDownlinkCapture54::OnInitDialog()
{
	CDownlinkCapture::OnInitDialog();

	m_bandSelect = eBandSelectType_Null;

	// ダイアログアイテムを生成する
	if (CreateDlgItems() == FALSE)
		return FALSE;

	// 監視データ取得
	(void)UpdateKanshiData();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
// TODO: X帯/Ka帯選択のボタンクリック時に呼ばれる
/*============================================================================*/
void CDownlinkCapture54::OnBnClickedDownlinkcaptureBandSelectButton()
{
	// TODO: 設定処理未定

#ifdef _DEBUG
	int nIndex = m_CCBoxBandSelect.GetCurSel();
	CString str;
	m_CCBoxBandSelect.GetLBText(nIndex, str);
	if (str.CompareNoCase(_T("X帯")) == 0)
	{
		// X帯
		m_bandSelect = eBandSelectType_X;
	}
	else if (str.CompareNoCase(_T("Ka帯")) == 0)
	{
		// Ka帯
		m_bandSelect = eBandSelectType_Ka;
	}
	else
	{
		m_bandSelect = eBandSelectType_Null;
	}
	CreateDlgBandSelect();
#endif
}

/*============================================================================*/
// ダイアログアイテムを生成する
/*============================================================================*/
BOOL CDownlinkCapture54::CreateDlgItems()
{
	if(CreateDlgBandSelect())
	{
		return TRUE;
	}

	return FALSE;
}

/*============================================================================*/
// X帯/Ka帯選択ダイアログアイテムを生成する
/*============================================================================*/
BOOL CDownlinkCapture54::CreateDlgBandSelect()
{
	TCHAR szMsg[256];

	// コンボボックスのクリア
	m_CCBoxBandSelect.ResetContent();

	// エディットコントロール初期化
	// コンボボックス初期化
	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_DOWNLINK_BANDSELECT_X, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	if (m_bandSelect == eBandSelectType_X)
	{
		m_strBandSelect = CString(szMsg);
	}
	else
	{
		m_CCBoxBandSelect.InsertString(-1, szMsg);
	}

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_DOWNLINK_BANDSELECT_KA, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	if (m_bandSelect == eBandSelectType_Ka)
	{
		m_strBandSelect = CString(szMsg);
	}
	else
	{
		m_CCBoxBandSelect.InsertString(-1, szMsg);
	}

	// 監視を未受信時
	if (m_bandSelect == eBandSelectType_Null)
	{
		m_strBandSelect = CString(_T(""));
	}

	m_CCBoxBandSelect.SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;
}

/*============================================================================*/
// 監視から各項目の値を取得して更新する
/*============================================================================*/
BOOL CDownlinkCapture54::UpdateKanshiData()
{
	// 受信周波数モードと制御モードの更新
	CDownlinkCapture::UpdateKanshiData();

	// TODO: X帯<=>Ka帯切り替えの更新

	return TRUE;
}

/*============================================================================*/
// 受信周波数モードのコントロールの表示、非表示
/*============================================================================*/
void CDownlinkCapture54::ShowRcvFreqMode()
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

