/*============================================================================*/
/*! SessionStatus.cpp

-# セッション状態画面
*/
/*============================================================================*/
// SessionStatus.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "SessionStatus.h"
#include "afxdialogex.h"


// CSessionStatus ダイアログ

IMPLEMENT_DYNAMIC(CSessionStatus, CDialogBase)

/*============================================================================*/
/*! CSessionStatus

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CSessionStatus::CSessionStatus(CWnd* pParent /*=NULL*/)
: CDialogBase(CSessionStatus::IDD, pParent)
{
}

/*============================================================================*/
/*! CSessionStatus

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CSessionStatus::~CSessionStatus()
{
}

void CSessionStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SESSIONSTATUS_TAB1, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(CSessionStatus, CDialogBase)
	ON_COMMAND(ID_MENU_CLOSE_RC2, &CSessionStatus::OnMenuClose)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SESSIONSTATUS_TAB1, &CSessionStatus::OnTcnSelchangeSessionstatusTab1)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


/*============================================================================*/
/*! CSessionStatus

-# 局名追加

@param	nIndex：インデックス
@param	stationName：局名
@retval	なし

*/
/*============================================================================*/
void CSessionStatus::AddStation(UINT nIndex, CString stationName)
{
	m_station[nIndex].SetStationName(stationName);
	m_station[nIndex].SetStationID(nIndex);
}


// CSessionStatus メッセージ ハンドラー


/*============================================================================*/
/*! CSessionStatus

-# メニューからのファイルクローズ時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CSessionStatus::OnMenuClose()
{
	//	OnOK();
	ShowWindow(SW_HIDE);
}

/*============================================================================*/
/*! CSessionStatus

-# ダイアログ初期化処理

@param  なし
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
BOOL CSessionStatus::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	CRect rect;
	m_tabCtrl.GetClientRect(&rect);
	rect.top += m_nTabTopOffset;
	rect.left += m_nTabLeftOffset;
	// リストに登録された局のタブ初期化
	for (int i = 0, nTabIndex = 0; i < eSessionType_Num; i++)
	{
#if 0
		CString stationName = m_station[i].GetStationName();
		if (stationName.CompareNoCase(_T("")) == 0)
			continue;
		if (!m_station[i].Create(CSessionStatusTab::IDD, this))
			return FALSE;
		m_station[i].MoveWindow(&rect, FALSE);
		m_tabCtrl.InsertItem(nTabIndex, m_station[i].GetStationName());
		nTabIndex++;
#else
		// ★表示インデックスを内之浦20m, 内之浦34m, 臼田64m, 臼田54mの順にする
		UINT nViewIdx = mStationViewOrder[i];
		CString stationName = m_station[nViewIdx].GetStationName();
		if (stationName.CompareNoCase(_T("")) == 0)
			continue;
		if (!m_station[nViewIdx].Create(CSessionStatusTab::IDD, this))
			return FALSE;
		m_station[nViewIdx].MoveWindow(&rect, FALSE);
		m_tabCtrl.InsertItem(nTabIndex, m_station[nViewIdx].GetStationName());
		nTabIndex++;
#endif
	}

//	m_tabCtrl.SetCurSel(m_nStation);
	m_tabCtrl.SetCurSel(mStationViewCnv[m_nStation]);

	if (theApp.GetSelectStationMode() == eStation_1)
	{
		SetWindowSubCode(1);
	}
	else
	{
		SetWindowSubCode(0);
	}

	OnTcnSelchangeSessionstatusTab1(0, 0);

#if 1
	// 監視タイマセット
	SetTimer(IDD_DLG_SESSIONSTATUS, m_nIntervalTime, NULL);
#endif
	UpdateData(FALSE);

	CenterWindowEx();

	// セッション状態を取得する
	GetSessionStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CSessionStatus

-# タブ選択変更時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし
*/
/*============================================================================*/
void CSessionStatus::OnTcnSelchangeSessionstatusTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int sel = m_tabCtrl.GetCurSel();
	int nStation = theApp.GetSelectStation();

	// 1局モード以外および、1局モードでかつ局タブ一致時のみ、選択したタブを表示する
	// ★表示インデックスを内之浦20m, 内之浦34m, 臼田64m, 臼田54mの順にする
	if (theApp.GetSelectStationMode() != eStation_1 ||
		theApp.GetSelectStationMode() == eStation_1 && mStationViewOrder[sel] == nStation)
	{
		// 選択されているタブを表示する
		for (int i = 0, nTabIndex = 0; i < eSessionType_Num; i++)
		{
			UINT nViewIdx = mStationViewOrder[i];
			CString stationName = m_station[nViewIdx].GetStationName();
			if (stationName.CompareNoCase(_T("")) == 0)
				continue;

			if (sel == nTabIndex)
			{
				m_station[nViewIdx].ShowWindow(SW_SHOW);
				if (theApp.GetSelectStationMode() != eStation_1)
				{
					m_nStation = nViewIdx;
				}
			}
			else
			{
				m_station[nViewIdx].ShowWindow(SW_HIDE);
			}
			nTabIndex++;
		}
	}
	else
	{
//		m_tabCtrl.SetCurSel(m_nStation);
		m_tabCtrl.SetCurSel(mStationViewCnv[m_nStation]);
	}

	if (pResult)
		*pResult = 0;
}


/*============================================================================*/
/*! CSessionStatus

-# タイマイベントで呼び出される

@param	nIDEvent：イベントID
@retval	なし

*/
/*============================================================================*/
void CSessionStatus::OnTimer(UINT_PTR nIDEvent)
{
	// 画面表示中のみ、セッション状態を取得する
	GetSessionStatus(FALSE, FALSE);

	CDialogBase::OnTimer(nIDEvent);
}


/*============================================================================*/
/*! CSessionStatus

-# 画面表示中のみ、セッション状態を取得する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSessionStatus::GetSessionStatus(BOOL bWaitComplete /*= TRUE*/, BOOL bForce /*= TRUE*/)
{
	if (this->IsWindowVisible() || bForce)
	{
		int sel = m_tabCtrl.GetCurSel();
		// ★表示インデックスを内之浦20m, 内之浦34m, 臼田64m, 臼田54mの順にする
		if (mStationViewOrder[sel] == m_nStation)
		{
			m_station[m_nStation].GetSessionStatus(bWaitComplete);
		}
	}
}


/*============================================================================*/
/*! CSessionStatus

-# ウィンドウ破棄時に呼び出される

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSessionStatus::OnDestroy()
{
	CDialogBase::OnDestroy();

	KillTimer(1);
}


/*============================================================================*/
/*! CSessionStatus

-# セッション状態表示更新時に処理する

@param	なし
@retval	なし

*/
/*============================================================================*/
LRESULT CSessionStatus::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_UpdateSessionStatus:
	{
		// セッション状態表示更新
		// 1局モード
		int nStation = theApp.GetSelectStation();

		// 1局モード時
		if (theApp.GetSelectStationMode() == eStation_1)
		{
			m_station[nStation].UpdateSessionDisplay();
		}
		// 1局モード以外
		else
		{
			for (int i = 0, nTabIndex = 0; i < eSessionType_Num; i++)
			{
				CString stationName = m_station[i].GetStationName();
				if (stationName.CompareNoCase(_T("")) == 0)
					continue;

				m_station[i].UpdateSessionDisplay();
				nTabIndex++;
			}
		}
	}
		break;

	default:
		return CDialogBase::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}

