/*============================================================================*/
/*! FrequencyCtrl54.cpp

-# 周波数制御画面(54m)
*/
/*============================================================================*/
// FrequencyCtrl54.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrl54.h"
#include "afxdialogex.h"


// CFrequencyCtrl54 ダイアログ

IMPLEMENT_DYNAMIC(CFrequencyCtrl54, CFrequencyCtrl)

CFrequencyCtrl54::CFrequencyCtrl54(CWnd* pParent /*=NULL*/)
: CFrequencyCtrl(CFrequencyCtrl54::IDD, pParent)
{

}

CFrequencyCtrl54::~CFrequencyCtrl54()
{
}

void CFrequencyCtrl54::DoDataExchange(CDataExchange* pDX)
{
	CFrequencyCtrl::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FREQ_CTRL_TAB, m_FreqTabCtrl);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrl54, CFrequencyCtrl)
	ON_NOTIFY(TCN_SELCHANGE, IDC_FREQ_CTRL_TAB, &CFrequencyCtrl54::OnTcnSelchangeFreqCtrlTab)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CFrequencyCtrl::OnMenuClose)
END_MESSAGE_MAP()


// CFrequencyCtrl54 メッセージ ハンドラー

/*============================================================================*/
/*! CFrequencyCtrl54

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CFrequencyCtrl54::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// 子ダイアログ作成
	m_TabMon.SetCallWnd(this);
	m_TabCtrl.SetCallWnd(this);
	m_TabPre.SetCallWnd(this);
	m_TabSm.SetCallWnd(this);
	m_TabFcChg.SetCallWnd(this);
	m_TabMon.Create(CFrequencyCtrlMon54::IDD, &m_FreqTabCtrl);
	m_TabCtrl.Create(CFrequencyCtrlCtrl54::IDD, &m_FreqTabCtrl);
	m_TabPre.Create(CFrequencyCtrlPre::IDD, &m_FreqTabCtrl);
	m_TabSm.Create(CFrequencyCtrlSm::IDD, &m_FreqTabCtrl);
	m_TabFcChg.Create(CFrequencyCtrlFcChg::IDD, &m_FreqTabCtrl);

	// 子ダイアログ移動
	CRect	rect;
	m_FreqTabCtrl.GetWindowRect(rect);
	m_FreqTabCtrl.AdjustRect(FALSE, rect);
	m_FreqTabCtrl.ScreenToClient(rect);
	rect.OffsetRect(0, 18);
	rect.bottom = rect.bottom - 18;


	m_TabMon.MoveWindow(&rect, FALSE);
	m_TabCtrl.MoveWindow(&rect, FALSE);
	m_TabPre.MoveWindow(&rect, FALSE);
	m_TabSm.MoveWindow(&rect, FALSE);
	m_TabFcChg.MoveWindow(&rect, FALSE);

	// タブ作成
	m_FreqTabCtrl.InsertItem(0, _T("周波数モニタ"));
	m_FreqTabCtrl.InsertItem(1, _T("個別設定"));
	m_FreqTabCtrl.InsertItem(2, _T("一括設定"));
	m_FreqTabCtrl.InsertItem(3, _T("シミュレーション"));
	m_FreqTabCtrl.InsertItem(4, _T("予報値切り替え"));

	// 初期表示
	OnTcnSelchangeFreqCtrlTab(0, 0);

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! CFrequencyCtrl

-# タブ選択変更通知

@param  pNMHDR	：NMHDR構造体ポインター。
@param  pResult	：メッセージが処理されたとき結果を格納するコードする LRESULT の変数へのポインター。
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CFrequencyCtrl54::OnTcnSelchangeFreqCtrlTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int        sel;
	sel = m_FreqTabCtrl.GetCurSel();

	// 監視モード時は先頭の「周波数モニタ」しか選択できない
	// 制御モードでかつ、制限つきも同様
	if( (theApp.GetExecuteMode() == eExecuteMode_Monitor) ||
		(theApp.GetLimitedMode() == eLimitedMode_Enable) ){
		m_TabMon.ShowWindow(SW_SHOW);
		m_TabCtrl.ShowWindow(SW_HIDE);
		m_TabPre.ShowWindow(SW_HIDE);
		m_TabSm.ShowWindow(SW_HIDE);
		m_TabFcChg.ShowWindow(SW_HIDE);
		m_FreqTabCtrl.SetCurSel(0);
	}
	else{
		m_TabMon.ShowWindow(sel == 0 ? SW_SHOW : SW_HIDE);
		m_TabCtrl.ShowWindow(sel == 1 ? SW_SHOW : SW_HIDE);
		m_TabPre.ShowWindow(sel == 2 ? SW_SHOW : SW_HIDE);
		m_TabSm.ShowWindow(sel == 3 ? SW_SHOW : SW_HIDE);
		m_TabFcChg.ShowWindow(sel == 4 ? SW_SHOW : SW_HIDE);
	}


	//	*pResult = 0;
	if (pResult)
		*pResult = 0;
}

LRESULT CFrequencyCtrl54::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_UpdateShareFile:
		Initialize();
		break;

	default:
		return CDialogBase::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}
