/*============================================================================*/
/*! ForecastList.cpp

-# 予報値一覧画面
*/
/*============================================================================*/
// ForecastList.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ForecastList.h"
#include "afxdialogex.h"
#include "TleList.h"
#include "OperationPlanTimeOffsetSet.h"
#include "..\Utility\SshShell.h"
#include "HookMsgBox.h"

//----------------------------------------------------------------------
// ダミーデータ

static const CString strSampleSatelliteName[6] = {
	_T("ALL"),
	_T("GCOM-C"),
	_T("GCOM-W"),
	_T("ALOS-2"),
	_T("HAYABUSA2"),
	_T("123456789012345")
};

static const CString strSampleStationName[5] = {
	_T("ALL"),
	_T("臼田64m局"),
	_T("内之浦34m局"),
	_T("内之浦20m局"),
	_T("臼田54m局")
};

// 共有ファイルのメッセージ定義
#define eShareFile_PredList		0x00000001
#define eShareFile_PlanList		0x00000002

// CForecastList ダイアログ

IMPLEMENT_DYNAMIC(CForecastList, CDialogBase)

/*============================================================================*/
/*! CForecastList

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CForecastList::CForecastList(CWnd* pParent /*=NULL*/)
	: CDialogBase(CForecastList::IDD, pParent)
	, m_bUseSelectBtn(FALSE)
	, m_pCallWnd(NULL)
	, m_bCloseUnCheck(FALSE)
{

}

/*============================================================================*/
/*! CForecastList

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CForecastList::~CForecastList()
{
	if (mDetailDlg.GetSafeHwnd() != NULL){
		mDetailDlg.DestroyWindow();
	}
}

void CForecastList::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FORCASTLIST_TAB1, m_tabCtrl);
	DDX_Control(pDX, IDC_FORCASTLIST_COMBO, m_CCBoxSatelliteName);
	DDX_Control(pDX, IDC_FORCASTLIST_STATION_COMBO, m_CCBoxStationName);
	DDX_Control(pDX, IDC_STATIC_FORECAST_RADAR_AREA, m_viewArea);
	DDX_Control(pDX, IDC_FORECASTLIST_BUTTON1, m_CButonDelete);
	DDX_Control(pDX, IDC_FORECASTLIST_BUTTON6, m_CButonOffsetTime);
}

BEGIN_MESSAGE_MAP(CForecastList, CDialogBase)
	ON_COMMAND(ID_MENU_OPTION_CLOSE, &CForecastList::OnMenuClose)
	ON_COMMAND(ID_MENU_GET_AUTOMODE, &CForecastList::OnMenuSetAutoMode)
	ON_COMMAND(ID_MENU_GET_MANUALMODE, &CForecastList::OnMenuSetManualModel)
	ON_COMMAND(ID_MENU_MANUAL_GET_FROM_STOCKDATA, &CForecastList::OnMenuGetManual)
	ON_COMMAND(ID_MENU_RESEND_TO_EQUIPMENT_CTRL, &CForecastList::OnMenuResend)
	ON_NOTIFY(TCN_SELCHANGE, IDC_FORCASTLIST_TAB1, &CForecastList::OnTcnSelchangeForcastlistTab1)
	ON_CBN_SELCHANGE(IDC_FORCASTLIST_COMBO, &CForecastList::OnCbnSelchangeForcastlistCombo)
	ON_BN_CLICKED(IDC_FORECASTLIST_BUTTON1, &CForecastList::OnBnClickedForecastlistButton1)
	ON_BN_CLICKED(IDC_FORECASTLIST_BUTTON2, &CForecastList::OnBnClickedForecastlistButton2)
	ON_BN_CLICKED(IDC_FORECASTLIST_BUTTON3, &CForecastList::OnBnClickedForecastlistButton3)
	ON_BN_CLICKED(IDC_FORECASTLIST_BUTTON4, &CForecastList::OnBnClickedForecastlistButton4)
	ON_BN_CLICKED(IDC_FORECASTLIST_BUTTON5, &CForecastList::OnBnClickedForecastlistButton5)
	ON_BN_CLICKED(IDC_FORECASTLIST_BUTTON6, &CForecastList::OnBnClickedForecastlistButton6)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_FORCASTLIST_STATION_COMBO, &CForecastList::OnCbnSelchangeForcastlistStationCombo)
	ON_WM_INITMENUPOPUP()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CForecastList メッセージ ハンドラー



/*============================================================================*/
/*! CForecastList

-# メニューからのファイルクローズ時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastList::OnMenuClose()
{
//	OnOK();
	ShowWindow(SW_HIDE);
}

/*============================================================================*/
/*! CForecastList

-# メニューからの再送時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastList::OnMenuResend()
{
	CString strTitle;
	CString strMsg;
	if (!strTitle.LoadStringW(IDS_FORECASTLIST_RESEND_TITLE))
		return;
	if (!strMsg.LoadStringW(IDS_FORECASTLIST_RESEND_MSG))
		return;

	if (IDOK == MessageBoxHooked(this->m_hWnd, strMsg, strTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		// 予報値再送要求
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = SEND_NOTICE_SH;
		CString arg;

		int nStation = theApp.GetSelectStation();
		arg.Format(_T("%s %d ALL"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_GETPRE);

		stParam.strRemShellParam.Format(arg);

		int err = CSshShell::ExecShell(stParam, FALSE);		// シェル待ちしない
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnMenuResend"), _T("debug"), strDebug, nLogEx::debug);
		}
		if (err == 0)
		{
			// 何もしない
		}
	}
}

/*============================================================================*/
/*! CForecastList

-# ダイアログ初期化処理

@param  なし
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
BOOL CForecastList::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	CString str;
	CRect rect;

	m_bCloseUnCheck = FALSE;

	// タブ初期化
	if (!m_forcastValue.Create(CForecastValue::IDD, this))
		return FALSE;

	if (!m_operationPlan.Create(CForecastOperationPlan::IDD, this))
		return FALSE;

	m_tabCtrl.GetClientRect(&rect);
	rect.top += nTabTopOffset;
	rect.bottom += nTabBotOffset;
	rect.left += nTabLeftOffset;
	m_forcastValue.MoveWindow(&rect, FALSE);
	m_operationPlan.MoveWindow(&rect, FALSE);

	if (str.LoadStringW(IDS_FORECASTLIST_FORECASTVALUE) != TRUE)
		return FALSE;
	m_tabCtrl.InsertItem(0, str);

	if (str.LoadStringW(IDS_FORECASTLIST_OPERATIONPLAN) != TRUE)
		return FALSE;
	m_tabCtrl.InsertItem(1, str);

	// レーダー初期化
	CRect rectGraph;
	m_viewArea.GetWindowRect(&rectGraph);
	ScreenToClient(&rectGraph);
	CRuntimeClass* pClass = RUNTIME_CLASS(CForecastRadarGraphView);
	if (pClass == NULL)
	{
		return FALSE;
	}

	m_pView = (CForecastRadarGraphView*)(pClass->CreateObject());
	if (m_pView != NULL)
	{
		m_pView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rectGraph, this, 0, NULL);
		m_pView->SendMessage(WM_INITIALUPDATE);
	}

	OnTcnSelchangeForcastlistTab1(0, 0);

	InitCombobox();

	// 監視モード時はボタンを非活性にする
	if (theApp.GetExecuteMode() != eExecuteMode_Control)
	{
		GetDlgItem(IDC_FORECASTLIST_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_FORECASTLIST_BUTTON6)->EnableWindow(FALSE);
	}
	//制御モードでかつ、制限つきはボタンを非活性にする
	else if(theApp.GetLimitedMode() == eLimitedMode_Enable)
	{
		GetDlgItem(IDC_FORECASTLIST_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_FORECASTLIST_BUTTON6)->EnableWindow(FALSE);
	}

	m_bUseSelectBtn = FALSE;

	m_pCallWnd = NULL;

	UpdateData(FALSE);

	CenterWindowEx();

	// 制御モードで、かつ制限つきでない場合、タイマー処理する
	if (theApp.GetExecuteMode() == eExecuteMode_Control &&
		theApp.GetLimitedMode() == eLimitedMode_Disable)
	{
		SetTimer(1, 1000, NULL);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CForecastList

-# タブ選択変更時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし
*/
/*============================================================================*/
void CForecastList::OnTcnSelchangeForcastlistTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_bUseSelectBtn == TRUE)
	{
		m_tabCtrl.SetCurSel(eForecastListType_Forecast);
	}
	else
	{
		int sel = m_tabCtrl.GetCurSel();
		SelectForcastlistAndInitCombo(sel);
	}

	if (pResult)
		*pResult = 0;
}


/*============================================================================*/
/*! CForecastList

-# 衛星名コンボボックス選択変更時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastList::OnCbnSelchangeForcastlistCombo()
{
	// 再描画
	m_forcastValue.UpdateListItem();
	m_operationPlan.UpdateListItem();
}

/*============================================================================*/
/*! CForecastList

-# 局名取得

@param	なし
@retval	衛星名

*/
/*============================================================================*/
CString CForecastList::GetStationName()
{
	CString str;
	UpdateData(TRUE);
	int nIndex = m_CCBoxStationName.GetCurSel();
	if (nIndex < 0)
		return str;
	m_CCBoxStationName.GetLBText(nIndex, str);

	return str;
}

/*============================================================================*/
/*! CForecastList

-# 簡易局名取得

@param	なし
@retval	衛星名

*/
/*============================================================================*/
CString CForecastList::GetSimpleStationName()
{
	CString str = GetStationName();

	for (int i = 0; i < eStation_MAX; i++)
	{
		if (str.Compare(mStationString[i]) == 0)
			return mStationSimpleString[i];
	}

	return NULL;
}

/*============================================================================*/
/*! CForecastList

-# 局インデックス取得

@param	なし
@retval	局インデックス

*/
/*============================================================================*/
int CForecastList::GetStationIndex()
{
	UpdateData(TRUE);
	return m_CCBoxStationName.GetCurSel();
}


/*============================================================================*/
/*! CForecastList

-# 局名に応じた局インデックス取得

@param	局名
@retval	局インデックス

*/
/*============================================================================*/
int CForecastList::GetStationIndex(CString& strStationName)
{
	for (int i = 0; i < eStation_MAX; i++)
	{
		if (strStationName.Compare(mStationString[i]) == 0)
			return i;
	}

	return -1;
}


/*============================================================================*/
/*! CForecastList

-# 局数を取得

@param	なし
@retval	局数

*/
/*============================================================================*/
int CForecastList::GetStationCount()
{
	return m_CCBoxStationName.GetCount();
}

/*============================================================================*/
/*! CForecastList

-# 衛星名取得

@param	なし
@retval	衛星名

*/
/*============================================================================*/
CString CForecastList::GetSatelliteName()
{
	CString str;
	UpdateData(TRUE);
	int nIndex = m_CCBoxSatelliteName.GetCurSel();
	if (nIndex < 0)
		return str;
	m_CCBoxSatelliteName.GetLBText(nIndex, str);

	return str;
}

/*============================================================================*/
/*! CForecastList

-# 時刻オフセットボタンにフォーカスをあてる

@param	なし
@retval	衛星名

*/
/*============================================================================*/
void CForecastList::SetFocusToOffsetTimeBtn()
{
	GotoDlgCtrl(GetDlgItem(IDC_FORECASTLIST_BUTTON6));
}


/*============================================================================*/
/*! CForecastList

-# タブを選択し、衛星名をセットする

@param	sel:タブ番号
@param	strSatelliteName:衛星名
@retval	なし

*/
/*============================================================================*/
void CForecastList::SelectTabAndSatellite(int sel, CString& strSatelliteName)
{
	m_tabCtrl.SetCurSel(sel);
	SelectForcastlist(sel, strSatelliteName);
}


/*============================================================================*/
/*! CForecastList

-# タブを選択し、コンボ初期化する

@param	タブ番号
@retval	なし

*/
/*============================================================================*/
void CForecastList::SelectTabAndInitCombo(int sel)
{
	m_tabCtrl.SetCurSel(sel);
	SelectForcastlistAndInitCombo(sel);
}


/*============================================================================*/
/*! CForecastList

-# 予報値一覧／計画一覧タブを選択する

@param	なし
@retval	衛星名

*/
/*============================================================================*/
void CForecastList::SelectForcastlist(int sel, CString& strSatelliteName)
{
	CString str;
	CMenu* pMenu = GetMenu();

	// タブ内の表示切替、削除/選択ボタン切替
	switch (sel)
	{
	case 0:
		// オプションメニューを非活性化
		if (pMenu != NULL)
		{
			if (theApp.GetExecuteMode() == eExecuteMode_Control)
			{
				//制御モードでかつ、制限つきはボタンを非活性にする
				if(theApp.GetLimitedMode() == eLimitedMode_Enable)
				{
					pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
				}
				else
				{
					pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
				}
			}
			else
			{
				pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
			}
			DrawMenuBar();
		}

		// 局名コンボ一覧、衛星名コンボ一覧を予報値一覧から取得する
		m_forcastValue.GetStationNameList(m_listStationName);
		m_forcastValue.GetSatelliteNameList(m_listSatelliteName);

		SetSatelliteName(strSatelliteName);

		/// 予報値一覧を表示
		m_forcastValue.ShowWindow(SW_SHOW);
		m_operationPlan.ShowWindow(SW_HIDE);

		// 削除ボタン名セット
		if (m_bUseSelectBtn == FALSE)
		{
			if (str.LoadStringW(IDS_FORECASTLIST_DELETE) != TRUE)
				return;
		}
		else
		// 選択ボタン名セット
		{
			if (str.LoadStringW(IDS_FORECASTLIST_SELECT) != TRUE)
				return;
		}

		// 予報値一覧の追尾グラフを表示
		m_forcastValue.InitRadarGraphView();
		break;

	case 1:
	default:
		// オプションメニューを非活性化
		if (pMenu != NULL)
		{
			pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
			DrawMenuBar();
		}

		// 局名コンボ一覧、衛星名コンボ一覧を計画一覧から取得する
		m_operationPlan.GetStationNameList(m_listStationName);
		m_operationPlan.GetSatelliteNameList(m_listSatelliteName);

		// 計画一覧を表示
		m_forcastValue.ShowWindow(SW_HIDE);
		m_operationPlan.ShowWindow(SW_SHOW);

		// 削除ボタン名セット
		if (str.LoadStringW(IDS_FORECASTLIST_DELETE) != TRUE)
			return;

		// 計画一覧の追尾グラフを表示
		m_operationPlan.InitRadarGraphView();
		break;
	}

	// 削除/選択ボタン表示切替
	m_CButonDelete.SetWindowTextW(str);
}


/*============================================================================*/
/*! CForecastList

-# 予報値一覧／計画一覧タブを選択する

@param	なし
@retval	衛星名

*/
/*============================================================================*/
void CForecastList::SelectForcastlistAndInitCombo(int sel)
{
	CString str;
	CMenu* pMenu = GetMenu();

	// タブ内の表示切替、削除/選択ボタン切替
	switch (sel)
	{
	case 0:
		// オプションメニューを非活性化
		if (pMenu != NULL)
		{
			if (theApp.GetExecuteMode() == eExecuteMode_Control)
			{
				//制御モードでかつ、制限つきはボタンを非活性にする
				if(theApp.GetLimitedMode() == eLimitedMode_Enable)
				{
					pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
				}
				else
				{
					pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
				}
			}
			else
			{
				pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
			}
			DrawMenuBar();
		}

		// 局名コンボ一覧、衛星名コンボ一覧を予報値一覧から取得する
		m_forcastValue.GetStationNameList(m_listStationName);
		m_forcastValue.GetSatelliteNameList(m_listSatelliteName);

		InitCombobox();

		/// 予報値一覧を表示
		m_forcastValue.ShowWindow(SW_SHOW);
		m_operationPlan.ShowWindow(SW_HIDE);

		// 削除ボタン名セット
		if (m_bUseSelectBtn == FALSE)
		{
			if (str.LoadStringW(IDS_FORECASTLIST_DELETE) != TRUE)
				return;
		}
		else
			// 選択ボタン名セット
		{
			if (str.LoadStringW(IDS_FORECASTLIST_SELECT) != TRUE)
				return;
		}

		// 予報値一覧の追尾グラフを表示
		m_forcastValue.InitRadarGraphView();

		// 予報値一覧であることを示す
		SetWindowSubCode(0);
		break;

	case 1:
	default:
		// オプションメニューを非活性化
		if (pMenu != NULL)
		{
			pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
			DrawMenuBar();
		}

		// 局名コンボ一覧、衛星名コンボ一覧を計画一覧から取得する
		m_operationPlan.GetStationNameList(m_listStationName);
		m_operationPlan.GetSatelliteNameList(m_listSatelliteName);

		InitCombobox();

		// 計画一覧を表示
		m_forcastValue.ShowWindow(SW_HIDE);
		m_operationPlan.ShowWindow(SW_SHOW);

		// 削除ボタン名セット
		if (str.LoadStringW(IDS_FORECASTLIST_DELETE) != TRUE)
			return;

		// 計画一覧の追尾グラフを表示
		m_operationPlan.InitRadarGraphView();

		// 計画一覧であることを示す
		SetWindowSubCode(1);
		break;
	}

	// 削除/選択ボタン表示切替
	m_CButonDelete.SetWindowTextW(str);
}


/*============================================================================*/
/*! CForecastList

-# 削除/選択ボタンクリック時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastList::OnBnClickedForecastlistButton1()
{
	switch (m_tabCtrl.GetCurSel())
	{
	case 0:
		// 予報値削除
		if (m_bUseSelectBtn == FALSE)
		{
			// 確認ダイアログ表示
			CString strTitle;
			CString strMsg;
			if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
				return;
			if (strMsg.LoadString(IDS_DELETE_SELECTED_PRED_MSG) == FALSE)
				return;
			if (IDCANCEL == MessageBoxHooked(this->m_hWnd, strMsg, strTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
				return;

			vector<stPredListData> listPred;
			BOOL bGetFilePath = m_forcastValue.GetSelectedForecastStationNameAndFullFilePath(listPred);
			int nSize = (int)listPred.size();
			if (bGetFilePath == FALSE || nSize <= 0)
			{
				return;
			}

			for (int i = 0; i < nSize; i++)
			{
				CString strStationName = listPred[i].strStationName;
				CString strSatelliteName = listPred[i].strSatellite;
				CString strPassId = listPred[i].strPassId;
				CString strFilePath = listPred[i].strPredPath;
				// 拡張子がprdの場合、anpに置き換える
				strFilePath.TrimRight(PRED_FILE_TYPE);
				strFilePath += ANP_FILE_TYPE;

				// 局名かつ衛星名かつ予報値パスIDが計画で使われているか調べる
				if (m_operationPlan.IsUsedPlan(strStationName, strSatelliteName, strPassId))
				{
					// 計画で使われている旨を表示
					CString strTitle;
					GetWindowText(strTitle);
					MessageBox(_T("計画で使われている予報値のため削除できません。"), strTitle, MB_OK | MB_ICONWARNING);
					continue;
				}

				// 予報値削除要求
				CSshShell::structSshParam stParam;
				stParam.strBatchPath = theApp.GetSshBatchPath();
				stParam.strUserName = theApp.GetSshUserName();
				stParam.strServerName = theApp.GetSshServerName();
				stParam.strKeyFilePath = theApp.GetSshKeyPath();
				stParam.strRemShellName = SEND_NOTICE_SH;
				CString arg;

				// 削除
				{
					// 局名から局番号取得
					int nStation = CForecastList::GetStationIndex(strStationName);
					arg.Format(_T("%s %d %s"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_DELPRE, (LPCTSTR)strFilePath);

					stParam.strRemShellParam.Format(arg);

					int err = CSshShell::ExecShell(stParam, TRUE);
					{
						CString strDebug;
						strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
						CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnBnClickedForecastlistButton1"), _T("debug"), strDebug, nLogEx::debug);
					}
					if (err == 0)
					{
						// 何もしない
					}
				}
			}
		}
		// 予報値選択
		else
		{
			CString str;
			if (m_forcastValue.GetSelectedForecastFilePath(str, TRUE))
			{
				m_bCloseUnCheck = TRUE;
				m_strAntFilePath = str;
				theApp.m_pMainWnd->SendMessage(eMessage_UpdateForecastFilePath, (WPARAM)this, (LPARAM)&m_strAntFilePath);
				PostMessage(WM_CLOSE);
			}
		}
		break;

	case 1:
	default:
	{
		// 確認ダイアログ表示
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return;
		if (strMsg.LoadString(IDS_DELETE_SELECTED_PLAN_MSG) == FALSE)
			return;
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, strMsg, strTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return;

		vector<stPlanListData> listPlan;
		BOOL bGetFilePath = m_operationPlan.GetSelectedStationNameAndPlanFilePath(listPlan, TRUE);
		int nSize = (int)listPlan.size();
		if (bGetFilePath == FALSE || nSize <= 0)
		{
			return;
		}
		for (int i = 0; i < nSize; i++)
		{
			// 計画削除要求
			CSshShell::structSshParam stParam;
			stParam.strBatchPath = theApp.GetSshBatchPath();
			stParam.strUserName = theApp.GetSshUserName();
			stParam.strServerName = theApp.GetSshServerName();
			stParam.strKeyFilePath = theApp.GetSshKeyPath();
			stParam.strRemShellName = REGIST_PLAN_SH;
			CString arg;
			CString strStationName = listPlan[i].strStationName;
			CString strSatelliteName = listPlan[i].strSatelliteName;
			CString strFilePath = listPlan[i].strPlanName;
			CString strGetFrom = listPlan[i].strGetFrom;

			// 局名から局番号取得
			int nStation = CForecastList::GetStationIndex(strStationName);
			// 取得名が局管制の場合、アルファベットに変更
			if (strGetFrom.Compare(PLAN_GETFORM) == 0)
				strGetFrom.Format(_T("sttn"));
			else
				strGetFrom.Format(_T("enp"));

			arg.Format(_T("%s del %s %s %s"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), (LPCTSTR)strSatelliteName.MakeLower(), (LPCTSTR)strGetFrom.MakeLower(), (LPCTSTR)strFilePath.MakeLower());

			stParam.strRemShellParam.Format(arg);

			int err = CSshShell::ExecShell(stParam, TRUE);
			{
				CString strDebug;
				strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
				CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnBnClickedForecastlistButton1"), _T("debug"), strDebug, nLogEx::debug);
			}
			if (err == 0)
			{
				// 何もしない
			}
		}
	}
		break;
	}
}


/*============================================================================*/
/*! CForecastList

-# 未選択エラー表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastList::ViewUnSelectedMessage()
{
	CString strTitle;
	CString strMsg;
	if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
		return;
	if (strMsg.LoadString(IDS_UNSELECTED_ERR) == FALSE)
		return;
	MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
}


/*============================================================================*/
/*! CForecastList

-# TLE一覧ボタンクリック時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastList::OnBnClickedForecastlistButton2()
{
	// TLE一覧画面を開く
	CTleList dlg;
	if (dlg.DoModal() == IDCLOSE)
	{
		return;
	}
}


/*============================================================================*/
/*! CForecastList

-# 詳細表示ボタンクリック時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastList::OnBnClickedForecastlistButton3()
{
	CString strStationName, strSatelliteName, strFilePath, fpath, strGetFrom;
	int nFormat = 0;

	switch (m_tabCtrl.GetCurSel())
	{
	// 予報値詳細表示
	case 0:
		if (m_forcastValue.GetSelectedForecastStationNameAndFilePath(strStationName, strFilePath, nFormat, TRUE))
		{
			// 局名から局番号取得
			int nStation = CForecastList::GetStationIndex(strStationName);

			// 拡張子がprdの場合、anpに置き換える
			strFilePath.TrimRight(PRED_FILE_TYPE);
			strFilePath += ANP_FILE_TYPE;

			fpath.Format(_T("%s%s"), (LPCTSTR)theApp.GetShareFilePath(eFileType_Pred_Folder, nStation), (LPCTSTR)strFilePath);
			mDetailDlg.SetTitle(_T("予報値詳細表示"));

			if (!fpath.IsEmpty())
			{
				// 詳細ビュー表示
				fpath.Replace(_T("/"), _T("\\"));
				mDetailDlg.SetSelectFileName(fpath);
				if (mDetailDlg.GetSafeHwnd() != NULL){
					mDetailDlg.DestroyWindow();
				}
				mDetailDlg.Create(IDD_DIALOG_DETAILVIEW_PRD, this);
				mDetailDlg.ShowWindow(SW_SHOW);
			}
		}
		break;

	// 計画詳細表示
	case 1:
		if (m_operationPlan.GetSelectedStationNameAndPlanFilePath(strStationName, strSatelliteName, strFilePath, strGetFrom, TRUE))
		{
			// 局名から局番号取得
			int nStation = CForecastList::GetStationIndex(strStationName);
			// 取得名が局管制の場合、アルファベットに変更
			if (strGetFrom.Compare(PLAN_GETFORM) == 0)
				strGetFrom.Format(_T("sttn"));
			else
				strGetFrom.Format(_T("enp"));

			fpath.Format(_T("%s%s\\%s\\%s"), (LPCTSTR)theApp.GetShareFilePath(eFileType_Plan_Folder, nStation), (LPCTSTR)strGetFrom, (LPCTSTR)strSatelliteName, (LPCTSTR)strFilePath);
			mDetailDlg.SetTitle(_T("計画詳細表示"));

			if (!fpath.IsEmpty())
			{
				// 詳細ビュー表示
				fpath.Replace(_T("/"), _T("\\"));
				mDetailDlg.SetSelectFileName(fpath);
				if (mDetailDlg.GetSafeHwnd() != NULL){
					mDetailDlg.DestroyWindow();
				}
				mDetailDlg.Create(IDD_DIALOG_DETAILVIEW, this);
				mDetailDlg.ShowWindow(SW_SHOW);
			}
		}
		break;

	default:
		break;
	}
}


/*============================================================================*/
/*! CForecastList

-# 詳細印刷ボタンクリック時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastList::OnBnClickedForecastlistButton4()
{
	CString strStationName, strSatelliteName, strFilePath, fpath, strGetFrom;
	int nFormat = 0;

	CString excelPrm;
	CSatelliteData &sd = theApp.GetSatelliteData();

	switch (m_tabCtrl.GetCurSel())
	{
	// 予報値ファイルパス取得
	case 0:
		if (m_forcastValue.GetSelectedForecastStationNameAndFilePath(strStationName, strFilePath, nFormat, TRUE))
		{
			// 局名から局番号取得
			int nStation = CForecastList::GetStationIndex(strStationName);

			// 拡張子がprdの場合、anpに置き換える
			strFilePath.TrimRight(PRED_FILE_TYPE);
			strFilePath += ANP_FILE_TYPE;

			fpath.Format(_T("%s%s"), (LPCTSTR)theApp.GetShareFilePath(eFileType_Pred_Folder, nStation), (LPCTSTR)strFilePath);
			excelPrm.Format(_T("%s /P%d"), (LPCTSTR)fpath, nFormat);
		}
		break;

	// 計画ファイルパス取得
	case 1:
		if (m_operationPlan.GetSelectedStationNameAndPlanFilePath(strStationName, strSatelliteName, strFilePath, strGetFrom, TRUE))
		{
			// 局名から局番号取得
			int nStation = CForecastList::GetStationIndex(strStationName);
			// 取得名が局管制の場合、アルファベットに変更
			if (strGetFrom.Compare(PLAN_GETFORM) == 0)
				strGetFrom.Format(_T("sttn"));
			else
				strGetFrom.Format(_T("enp"));

			fpath.Format(_T("%s%s\\%s\\%s"), (LPCTSTR)theApp.GetShareFilePath(eFileType_Plan_Folder, nStation), (LPCTSTR)strGetFrom, (LPCTSTR)strSatelliteName, (LPCTSTR)strFilePath);
			excelPrm = sd.CreatePlnPrintFile(fpath);
		}
		break;

	default:
		break;
	}

	if (!fpath.IsEmpty())
	{
#if 1
		// 帳票
		CSatelliteData::ExcelOutPut(m_hWnd, excelPrm);

#else
		// ファイル印刷
		fpath.Replace(_T("/"), _T("\\"));
		CString strWorkPath = fpath;
		if (CPrintUtility::PrintEucFile(strWorkPath))
		{
			// 印刷したメッセージ表示
			CString strTitle;
			CString strMsg, strMsgTmp;
			if (strTitle.LoadString(IDS_PRINT_TITLE) == FALSE)
				return;
			if (strMsgTmp.LoadString(IDS_PRINT_OK_MSG) == FALSE)
				return;
			strMsg.Format(strMsgTmp, fpath);
			MessageBox(strMsg, strTitle, MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			// 印刷できなかったメッセージ表示
			CString strTitle;
			CString strMsg, strMsgTmp;
			if (strTitle.LoadString(IDS_PRINT_TITLE) == FALSE)
				return;
			if (strMsgTmp.LoadString(IDS_PRINT_NG_MSG) == FALSE)
				return;
			strMsg.Format(strMsgTmp, fpath);
			MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);

		}
#endif
	}
}


/*============================================================================*/
/*! CForecastList

-# 一覧印刷する

@param  nStation：局番号
@retval なし
*/
/*============================================================================*/
BOOL CForecastList::PrintList(int nStation, CString& fname)
{
	CString fpath;
	switch (m_tabCtrl.GetCurSel())
	{
		// 予報値一覧ファイルパス取得
	case 0:
		fpath = theApp.GetShareFilePath(eFileType_PredList_File, nStation);
		break;

		// 計画一覧ファイルパス取得
	case 1:
		fpath = theApp.GetShareFilePath(eFileType_PlanList_File, nStation);
//		fname = fpath.Right(fpath.GetLength() - fpath.ReverseFind(_T('\\')) - 1);
		break;

	default:
		break;
	}

	if (!fpath.IsEmpty())
	{
		// ファイル印刷
		fpath.Replace(_T("/"), _T("\\"));
		CString strWorkPath = fpath;
		//		fname = strWorkPath.Right(strWorkPath.GetLength() - strWorkPath.ReverseFind(_T('\\')) - 1);
		fname = strWorkPath;
		CString str;
//		str.Format(_T("%s(%d) :: 一覧印刷"), __FILEW__, __LINE__);
//		str.Format(_T("%s(%d) :: (%s, %d)"), __FILEW__, __LINE__, strWorkPath, nStation);
		str.Format(_T("%s(%d) :: %s, %d"), __FILEW__, __LINE__, (LPCTSTR)strWorkPath, nStation);
		CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("PrintList"), _T("Info"), str, nLogEx::info);
		if (CPrintUtility::PrintEucFile(strWorkPath))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	return FALSE;
}


/*============================================================================*/
/*! CForecastList

-# 一覧印刷ボタンクリック時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastList::OnBnClickedForecastlistButton5()
{
	BOOL ret = TRUE;
	CString fname;
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		ret = PrintList(theApp.GetSelectStation(), fname);
	}
	else if (theApp.GetSelectStationMode() == eStation_5)
	{
		for (int i = 0; i < eStation_MAX; i++)
		{
			if (PrintList(i, fname) == FALSE)
			{
				ret = FALSE;
			}
		}
	}

	if (ret)
	{
		// 印刷したメッセージ表示
		CString strTitle;
		CString strMsg, strMsgTmp;
		if (strTitle.LoadString(IDS_PRINT_TITLE) == FALSE)
			return;
		if (strMsgTmp.LoadString(IDS_PRINT_OK_MSG) == FALSE)
			return;
		strMsg.Format(strMsgTmp, fname);
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		// 印刷できなかったメッセージ表示
		CString strTitle;
		CString strMsg, strMsgTmp;
		if (strTitle.LoadString(IDS_PRINT_TITLE) == FALSE)
			return;
		if (strMsgTmp.LoadString(IDS_PRINT_NG_MSG) == FALSE)
			return;
		strMsg.Format(strMsgTmp, fname);
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
	}
}

/*============================================================================*/
/*! CForecastList

-# 時刻オフセットボタンクリック時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastList::OnBnClickedForecastlistButton6()
{
	// 一覧から時刻オフセット取得
	time_t offsetTime;
	if (!m_forcastValue.GetSelectedForecastOffsetTime(offsetTime))
	{
		return;
	}

	// 選択されている予報値の一番上の局名とファイルパスを取得
	CString strStationName, strSatellite, strPassId, strFilePath;
	if (!m_forcastValue.GetSelectedForecastStationNameAndFullFilePath(strStationName, strSatellite, strPassId, strFilePath))
	{
		return;
	}
	// 局名から局番号取得
	int nStation = CForecastList::GetStationIndex(strStationName);

	// 時刻オフセット設定ダイアログを呼ぶ
	COperationPlanTimeOffsetSet dlg;
	dlg.SetTime(offsetTime);
	dlg.SetStationName(CString(mStationSimpleString[nStation]).MakeLower());
	dlg.SetFullFilePath(strFilePath);
	if (dlg.DoModal() == IDCLOSE)
	{
		return;
	}
}


/*============================================================================*/
/*! CForecastList

-# 描画時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastList::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// 運用計画タブの場合は、時刻オフセットボタンをディセーブルにする
	if (m_tabCtrl.GetCurSel() == 1)
	{
		m_CButonOffsetTime.ShowWindow(SW_HIDE);
	}
	// 予報値タブの場合は、時刻オフセットボタンをイネーブルにする
	else
	{
		m_CButonOffsetTime.ShowWindow(SW_SHOW);
	}
}


/*============================================================================*/
/*! CForecastList

-# 局名コンボボックス選択変更時時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastList::OnCbnSelchangeForcastlistStationCombo()
{
	// 再描画
	m_forcastValue.UpdateListItem();
	m_operationPlan.UpdateListItem();
}


/*============================================================================*/
/*! CForecastList

-# 局名を局インデックスに変換する

@param  なし
@retval -1：局名がみつからなかった / それ以外：局名に対応するインデックス
*/
/*============================================================================*/
int CForecastList::ChangeSimpleStationNameToIndex(CString stationName)
{
	int sz = sizeof(mStationSimpleString) / sizeof(mStationSimpleString[0]);

	for (int i = 0; i < sz; i++)
	{
		CString str(mStationSimpleString[i]);

		if (str.Compare(stationName) == 0)
		{
			return i;
		}
	}

	return -1;
}


/*============================================================================*/
/*! CForecastList

-# 衛星名をセット

@param  strStationName：衛星名
@retval なし
*/
/*============================================================================*/
void CForecastList::SetSatelliteName(CString& strStationName)
{
	m_CCBoxSatelliteName.ResetContent();
	if (strStationName.IsEmpty() == true){
		for (int i = 0; i < m_listSatelliteName.size(); i++)
		{
			m_CCBoxSatelliteName.InsertString(-1, m_listSatelliteName[i]);
		}
		m_CCBoxSatelliteName.SetCurSel(0);
	}
	else{
		m_CCBoxSatelliteName.InsertString(-1, strStationName);
	}
	m_CCBoxSatelliteName.SetCurSel(0);

	// 再描画
	m_forcastValue.UpdateListItem();
	m_operationPlan.UpdateListItem();

	UpdateData(FALSE);
}


/*============================================================================*/
/*! CForecastList

-# コンボボックスを初期化

@param  strStationName：衛星名
@retval なし
*/
/*============================================================================*/
void CForecastList::InitCombobox()
#if 0
{
	InitComboAndList();
	UpdateData(FALSE);
	m_forcastValue.UpdateData(FALSE);
	m_operationPlan.UpdateData(FALSE);
}
#else
{
	CString str;
	int sz;

	// コンボの中身を削除
	m_CCBoxStationName.ResetContent();
	m_CCBoxSatelliteName.ResetContent();

	// コンボボックス初期化
	// 局名
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		str = mStationString[theApp.GetSelectStation()];
		m_CCBoxStationName.InsertString(-1, str);
	}
	else if (theApp.GetSelectStationMode() == eStation_5)
	{
		m_CCBoxStationName.InsertString(-1, _T("ALL"));
		sz = sizeof(mStationString) / sizeof(mStationString[0]);
		for (int i = 0; i < sz; ++i)
		{
			m_CCBoxStationName.InsertString(-1, mStationString[i]);
		}
	}
	else
	{
	}
	m_CCBoxStationName.SetCurSel(0);

	// 衛星名
	// 予報値一覧 or 計画一覧から衛星名リストをつくりコンボにセット
	m_CCBoxSatelliteName.InsertString(-1, _T("ALL"));
	for (int i = 0; i < m_listSatelliteName.size(); i++)
	{
		m_CCBoxSatelliteName.InsertString(-1, m_listSatelliteName[i]);
	}
	m_CCBoxSatelliteName.SetCurSel(0);

	m_forcastValue.UpdateListItem();
	m_operationPlan.UpdateListItem();
}
#endif

#if 0
/*============================================================================*/
/*! CForecastList

-# コンボボックスとリストを初期化

@param  strStationName：衛星名
@retval なし
*/
/*============================================================================*/
void CForecastList::InitComboAndList()
{
	CString str;
	int sz;

	// コンボの中身を削除
	m_CCBoxStationName.ResetContent();
	m_CCBoxSatelliteName.ResetContent();

	// コンボボックス初期化
	// 局名
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		str = mStationString[theApp.GetSelectStation()];
		m_CCBoxStationName.InsertString(-1, str);
	}
	else if (theApp.GetSelectStationMode() == eStation_5)
	{
		m_CCBoxStationName.InsertString(-1, _T("ALL"));
		sz = sizeof(mStationString) / sizeof(mStationString[0]);
		for (int i = 0; i < sz; ++i)
		{
			m_CCBoxStationName.InsertString(-1, mStationString[i]);
		}
	}
	else
	{
	}
	m_CCBoxStationName.SetCurSel(0);

	// 衛星名
	// 予報値一覧 or 計画一覧から衛星名リストをつくりコンボにセット
	m_CCBoxSatelliteName.InsertString(-1, _T("ALL"));
	for (int i = 0; i < m_listSatelliteName.size(); i++)
	{
		m_CCBoxSatelliteName.InsertString(-1, m_listSatelliteName[i]);
	}
	m_CCBoxSatelliteName.SetCurSel(0);

	m_forcastValue.UpdateListItem();
	m_operationPlan.UpdateListItem();
}
#endif

/*============================================================================*/
/*! CForecastList

-# 一覧更新時に呼ばれる

@param  strStationName：衛星名
@retval なし
*/
/*============================================================================*/
LRESULT CForecastList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
//#ifdef _NOCOMM
//	return CDialogBase::WindowProc(message, wParam, lParam);
//#endif

	switch (message){

	case	eMessage_UpdateShareFile:
		// 予報値一覧ファイル更新
		if ((int)lParam & eShareFile_PredList)
		{
			m_forcastValue.UpdateListItem();
		}

		if ((int)lParam & eShareFile_PlanList)
		{
			// 計画一覧ファイル更新
			m_operationPlan.UpdateListItem();
		}

		return TRUE;

	default:
		break;
	}

	return CDialogBase::WindowProc(message, wParam, lParam);
}


/*============================================================================*/
/*! CForecastList

-# メニューで、取得モードを自動選択時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastList::OnMenuSetAutoMode()
{
	CString strTitle;
	CString strMsg;
	if (!strTitle.LoadStringW(IDS_FORECASTLIST_SETMODE_TITLE))
		return;
	if (!strMsg.LoadStringW(IDS_FORECASTLIST_SETMODE_AUTO_MSG))
		return;

	if (IDOK == MessageBoxHooked(this->m_hWnd, strMsg, strTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		// 予報値自動取得モード要求
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = SEND_NOTICE_SH;
		CString arg;

		int nStation = theApp.GetSelectStation();
		arg.Format(_T("%s %d"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_AUTGET);

		stParam.strRemShellParam.Format(arg);

		int err = CSshShell::ExecShell(stParam, FALSE);		// シェル待ちしない
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnMenuSetAutoMode"), _T("debug"), strDebug, nLogEx::debug);
		}
		if (err == 0)
		{
			// 何もしない
		}
	}
}


/*============================================================================*/
/*! CForecastList

-# メニューで、取得モードを手動選択時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastList::OnMenuSetManualModel()
{
	CString strTitle;
	CString strMsg;
	if (!strTitle.LoadStringW(IDS_FORECASTLIST_SETMODE_TITLE))
		return;
	if (!strMsg.LoadStringW(IDS_FORECASTLIST_SETMODE_MANUAL_MSG))
		return;

	if (IDOK == MessageBoxHooked(this->m_hWnd, strMsg, strTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		// 予報値手動取得モード要求
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = SEND_NOTICE_SH;
		CString arg;

		int nStation = theApp.GetSelectStation();
		arg.Format(_T("%s %d"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_MNUGET);

		stParam.strRemShellParam.Format(arg);

		int err = CSshShell::ExecShell(stParam, FALSE);		// シェル待ちしない
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnMenuSetManualModel"), _T("debug"), strDebug, nLogEx::debug);
		}
		if (err == 0)
		{
			// 何もしない
		}
	}
}


/*============================================================================*/
/*! CForecastList

-# 予報値を手動取得するメニュー選択時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastList::OnMenuGetManual()
{
	CString strTitle;
	CString strMsg;
	if (!strTitle.LoadStringW(IDS_FORECASTLIST_MANUALGET_TITLE))
		return;
	if (!strMsg.LoadStringW(IDS_FORECASTLIST_MANUALGET_MSG))
		return;

	if (IDOK == MessageBoxHooked(this->m_hWnd, strMsg, strTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		// 予報値取得要求
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = SEND_NOTICE_SH;
		CString arg;
		CString strSatelliteName = GetSatelliteName();

		int nStation = theApp.GetSelectStation();
		arg.Format(_T("%s %d %s"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_GETPRE, (LPCTSTR)strSatelliteName);

		stParam.strRemShellParam.Format(arg);

		int err = CSshShell::ExecShell(stParam, FALSE);		// シェル待ちしない
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnMenuGetManual"), _T("debug"), strDebug, nLogEx::debug);
		}
		if (err == 0)
		{
			// 予報値取得中の場合非活性化
			GetDlgItem(IDC_FORECASTLIST_BUTTON1)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON6)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON3)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON4)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON5)->EnableWindow(FALSE);
		}
	}
}


/*============================================================================*/
/*! CForecastList

-# 予報値取得モードを監視から取得する

@param  なし
@retval 取得モード：　ePredGetType_Auto:自動 / ePredGetType_Manual:手動
*/
/*============================================================================*/
UINT CForecastList::GetPredGetMode()
{
	UINT nCheck = ePredGetType_Other;

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT sta = theApp.GetSelectStation();
	CString str;

	/* データ蓄積装置からの取得モード */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(PRED_GET_MODE));
	if (obs_adr == NULL)
	{
		str.Format(_T("%s(%d) :: Can not get db access."), __FILEW__, __LINE__);
		CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnUpdateManualMode"), _T("Error"), str, nLogEx::error);
	}
	else if (obs_adr->l_inflg != INFLG_ON)
	{
		str.Format(_T("%s(%d) :: %s is invalidate."), __FILEW__, __LINE__, (LPCTSTR)CString(obs_adr->sz_obsname));
		CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnUpdateManualMode"), _T("Error"), str, nLogEx::error);
	}
	else
	{
		CString str(obs_adr->sz_sts_name);
		if (str.Compare(_T(AUTO_STR)) == 0)
		{
			nCheck = ePredGetType_Auto;
		}
		else if (str.Compare(_T(MANUAL_STR)) == 0)
		{
			nCheck = ePredGetType_Manual;
		}
		else if (str.Compare(_T(STOP_STR)) == 0)
		{
			nCheck = ePredGetType_Stop;
		}
		else
		{
			nCheck = ePredGetType_Other;
		}
	}

	return nCheck;
}


/*============================================================================*/
/*! CForecastList

-# メニュー呼び出し時に呼ばれる

@param  pPopupMenu
@param  nIndex		
@param  bSysMenu
@retval なし
*/
/*============================================================================*/
void CForecastList::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialogBase::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	if (pPopupMenu->GetMenuItemID(0) == ID_MENU_GET_AUTOMODE)
	{
		UINT nKanshiMode = GetPredGetMode();
		if (nKanshiMode == ePredGetType_Auto)
		{
			pPopupMenu->CheckMenuItem(ID_MENU_GET_AUTOMODE, MF_BYCOMMAND | MFS_CHECKED);
		}
		else
		{
			pPopupMenu->CheckMenuItem(ID_MENU_GET_AUTOMODE, MF_BYCOMMAND | MFS_UNCHECKED);
		}
	}

	if (pPopupMenu->GetMenuItemID(1) == ID_MENU_GET_MANUALMODE)
	{
		UINT nKanshiMode = GetPredGetMode();
		if (nKanshiMode == ePredGetType_Manual)
		{
			pPopupMenu->CheckMenuItem(ID_MENU_GET_MANUALMODE, MF_BYCOMMAND | MFS_CHECKED);
		}
		else
		{
			pPopupMenu->CheckMenuItem(ID_MENU_GET_MANUALMODE, MF_BYCOMMAND | MFS_UNCHECKED);
		}
	}
}


/*============================================================================*/
/*! CForecastList

-# 閉じる時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastList::OnClose()
{
	if (m_bCloseUnCheck == FALSE)
	{
		CDialogBase::OnClose();
	}
	else
	{
		m_bCloseUnCheck = FALSE;
		CDialogEx::OnClose();
	}
}

/*============================================================================*/
/*! CForecastList

-# タイマ毎に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastList::OnTimer(UINT_PTR nIDEvent)
{
	const CString mPRED_COND = _T("CTRL.PRED_COND");
	const CString mTAKING = _T("TAKING");

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT nStation = theApp.GetSelectStation();

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, mPRED_COND);
	if (obs_adr != NULL)
	{
		CString strPredStatus = CString(obs_adr->sz_sts_name);
		if (strPredStatus.CompareNoCase(mTAKING) == 0)
		{
			// 予報値取得中の場合非活性化
			GetDlgItem(IDC_FORECASTLIST_BUTTON1)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON6)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON3)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON4)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON5)->EnableWindow(FALSE);
		}
		else
		{
			// それ以外は活性化
			GetDlgItem(IDC_FORECASTLIST_BUTTON1)->EnableWindow(TRUE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON6)->EnableWindow(TRUE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON3)->EnableWindow(TRUE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON4)->EnableWindow(TRUE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON5)->EnableWindow(TRUE);
		}
	}

	CDialogBase::OnTimer(nIDEvent);
}
