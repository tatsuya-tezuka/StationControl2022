/*============================================================================*/
/*! FrequencyCtrl.cpp

-# 周波数制御画面
*/
/*============================================================================*/
// FrequencyCtrl.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrl.h"
#include "afxdialogex.h"


// CFrequencyCtrl ダイアログ

IMPLEMENT_DYNAMIC(CFrequencyCtrl, CDialogBase)

/*============================================================================*/
/*! CFrequencyCtrl

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CFrequencyCtrl::CFrequencyCtrl(CWnd* pParent /*=NULL*/)
: CDialogBase(CFrequencyCtrl::IDD, pParent)
{
	mTargetData.satellite.Empty();
	mTargetData.predAnp.Empty();
	mTargetData.txDopp = 0;
	mTargetData.timeStart = 0;
	mTargetData.timeStop = 0;

	Initialize(false);
}

/*============================================================================*/
/*! CFrequencyCtrl

-# コンストラクタ

@param	IDD：IDD
@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CFrequencyCtrl::CFrequencyCtrl(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialogBase(IDD, pParent)
{

}

/*============================================================================*/
/*! CFrequencyCtrl

-# デストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CFrequencyCtrl::~CFrequencyCtrl()
{
}

/*============================================================================*/
/*! CForecastOperationPlan

-# 計画一覧から衛星名リストを取得する

@param	listName：衛星名リスト
@retval TRUE: 返す。 FALSE: 返さない。

*/
/*============================================================================*/
BOOL CFrequencyCtrl::GetSatelliteNameList(vector<CString>& listName)
{
	CGraphMutex::Lock(eFile);
	vector<stPredListData>::iterator itr;
	vector<CString>::iterator itrName;

	listName.clear();

	for (itr = mAllPredList.begin(); itr != mAllPredList.end(); itr++)
	{
		// 衛星名が既に衛星名リストにあるか調べる
		itrName = find(listName.begin(), listName.end(), (*itr).strSatellite);
		if (itrName == listName.end())
		{
			// 無い場合は局名リストに追加
			listName.push_back((*itr).strSatellite);
		}
	}
	CGraphMutex::Unlock(eFile);

	if (listName.empty() == TRUE)
		return FALSE;

	return TRUE;
}

void CFrequencyCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FREQ_CTRL_TAB, m_FreqTabCtrl);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrl, CDialogBase)
	ON_NOTIFY(TCN_SELCHANGE, IDC_FREQ_CTRL_TAB, &CFrequencyCtrl::OnTcnSelchangeFreqCtrlTab)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CFrequencyCtrl::OnMenuClose)
END_MESSAGE_MAP()


// CFrequencyCtrl メッセージ ハンドラー

/*============================================================================*/
/*! CFrequencyCtrl

-# ファイル-閉じるメニュー処理

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrl::OnMenuClose()
{
	//	OnOK();
	ShowWindow(SW_HIDE);
}

/*============================================================================*/
/*! CFrequencyCtrl

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CFrequencyCtrl::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// 子ダイアログ作成
	m_TabMon.SetCallWnd(this);
	m_TabCtrl.SetCallWnd(this);
	m_TabPre.SetCallWnd(this);
	m_TabSm.SetCallWnd(this);
	m_TabFcChg.SetCallWnd(this);
	m_TabMon.Create(CFrequencyCtrlMon::IDD, &m_FreqTabCtrl);
	m_TabCtrl.Create(CFrequencyCtrlCtrl::IDD, &m_FreqTabCtrl);
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

-# ダイアログ更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CFrequencyCtrl::UpdateDlg()
{
	return TRUE;
}


/*============================================================================*/
/*! CFrequencyCtrl

-# コントロールの初期化

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrl::Initialize(bool bResetControl/* = true*/)
{
	mAllPredList = theApp.GetSatelliteData().GetPredList();
	GetSatelliteNameList(mSatelliteNameList);
	if (bResetControl == true){
		m_TabSm.Initialize();
		m_TabFcChg.Initialize();
	}
}

/*============================================================================*/
/*! CFrequencyCtrl

-# タブ選択変更通知

@param  pNMHDR	：NMHDR構造体ポインター。
@param  pResult	：メッセージが処理されたとき結果を格納するコードする LRESULT の変数へのポインター。
@retval 入力フォーカス設定
*/
/*============================================================================*/
void CFrequencyCtrl::OnTcnSelchangeFreqCtrlTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int        sel;
	sel = m_FreqTabCtrl.GetCurSel();

	// 監視モード時は先頭の「周波数モニタ」しか選択できない
	if (theApp.GetExecuteMode() != eExecuteMode_Control){
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


/*============================================================================*/
/*! CFrequencyCtrl

-# 予報値ファイル(anp)からデータ取得

@param	satName		衛星名
@param	anpName		予報値ANPファイル名
@retval	なし
*/
/*============================================================================*/
void CFrequencyCtrl::GetAnpData(CString satName, CString anpName)
{
	CString anpPath;
	FILE *fp;

	char	szLine[512];
	TCHAR	szBuff[512];

	COleDateTime oleTime;
	CTime tmpTime;

	m_AnpDataList.clear();

	if (anpName == _T("") || satName == _T("")){
		return;
	}

	// 予報値フォルダ取得
	CString dir = theApp.GetShareFilePath(eFileType_Pred_Folder, theApp.GetSelectStation());

	// 予報値ファイルフルパス取得
	anpPath.Format(_T("%s%s\\%s"), (LPCTSTR)dir, (LPCTSTR)satName, (LPCTSTR)anpName);

	// 予報値ファイルより、日時、AZ、EL取得
	if ((_wfopen_s(&fp, anpPath, _T("r")) != 0) || (fp == NULL)){
		return;
	}


	// ヘッダー部読み飛ばし
	for (int i = 0; i < 5; ++i){
		fgets(szLine, static_cast<unsigned>(_countof(szLine)), fp);
	}

	while (fgets(szLine, static_cast<unsigned>(_countof(szLine)), fp) != NULL){
		CFrequencyCtrl::stAnpData data = { 0 };
		wsprintf(szBuff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(szLine));
		swscanf_s(szBuff, _T("%s %s %s %s %s %s %s"),
			data.szTime, static_cast<TCHAR>(_countof(data.szTime)),
			data.szAZ, static_cast<TCHAR>(_countof(data.szAZ)),
			data.szEL, static_cast<TCHAR>(_countof(data.szEL)),
			data.szUpRNG, static_cast<TCHAR>(_countof(data.szUpRNG)),
			data.szDwRNG, static_cast<TCHAR>(_countof(data.szDwRNG)),
			data.szUpRR, static_cast<TCHAR>(_countof(data.szUpRR)),
			data.szDwRR, static_cast<TCHAR>(_countof(data.szDwRR)),
			data.sz2wayRNG, static_cast<TCHAR>(_countof(data.sz2wayRNG)),
			data.sz2wayRR, static_cast<TCHAR>(_countof(data.sz2wayRR)));

		m_AnpDataList.push_back(data);
	}
	fclose(fp);
}


LRESULT CFrequencyCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
