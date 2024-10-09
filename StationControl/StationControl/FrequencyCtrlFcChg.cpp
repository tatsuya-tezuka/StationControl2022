/*============================================================================*/
/*! FrequencyCtrlFcChg.cpp

-# 周波数制御画面-予報値切替タブ
*/
/*============================================================================*/
// FrequencyCtrlFcChg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlFcChg.h"
#include "afxdialogex.h"
#include "FrequencyCtrl.h"

#include "WaitDialog.h"


// CFrequencyCtrlFcChg ダイアログ

IMPLEMENT_DYNAMIC(CFrequencyCtrlFcChg, CDialogBase)

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CFrequencyCtrlFcChg::CFrequencyCtrlFcChg(CWnd* pParent /*=NULL*/)
: CDialogBase(CFrequencyCtrlFcChg::IDD, pParent)
	, m_PassStartDay(0)
	, m_PassStartTime(0)
	, m_PassEndDay(0)
	, m_PassEndTime(0)
	, m_nTxDopp(0)
{
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CFrequencyCtrlFcChg::~CFrequencyCtrlFcChg()
{
}

void CFrequencyCtrlFcChg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FREQ_CTRL_FCCHG_SPCR_CMB, m_SpacecraftCmb);
	DDX_Control(pDX, IDC_FREQ_CTRL_FCCHG_TDP_CMB, m_TDoppCmb);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_FCCHG_PASST_DAY_DP, m_PassStartDay);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_FCCHG_PASST_TIME_DP, m_PassStartTime);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_FCCHG_PASET_DAY_DP, m_PassEndDay);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_FCCHG_PASET_TIME_DP, m_PassEndTime);
	DDX_Control(pDX, IDC_FREQ_CTRL_FCCHG_PASST_DAY_DP, m_PassStartDTCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_FCCHG_PASET_DAY_DP, m_PassEndDTCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_FCCHG_ANTFC_BTN, m_AntPredBtnCtrl);
	DDX_CBIndex(pDX, IDC_FREQ_CTRL_FCCHG_TDP_CMB, m_nTxDopp);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrlFcChg, CDialogBase)
	ON_BN_CLICKED(IDC_FREQ_CTRL_FCCHG_ANTFC_BTN, &CFrequencyCtrlFcChg::OnBnClickedFreqCtrlFcchgAntfcBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_FCCHG_BTN, &CFrequencyCtrlFcChg::OnBnClickedFreqCtrlFcchgBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_OPE_BTN, &CFrequencyCtrlFcChg::OnBnClickedFreqCtrlOpeBtn)
	ON_CBN_SELCHANGE(IDC_FREQ_CTRL_FCCHG_SPCR_CMB, &CFrequencyCtrlFcChg::OnCbnSelchangeFreqCtrlFcchgSpcrCmb)
	ON_CBN_SELCHANGE(IDC_FREQ_CTRL_FCCHG_TDP_CMB, &CFrequencyCtrlFcChg::OnCbnSelchangeFreqCtrlFcchgTdpCmb)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_FREQ_CTRL_FCCHG_PASST_DAY_DP, &CFrequencyCtrlFcChg::OnDtnDatetimechangeFreqCtrlFcchgPasstDayDp)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_FREQ_CTRL_FCCHG_PASST_TIME_DP, &CFrequencyCtrlFcChg::OnDtnDatetimechangeFreqCtrlFcchgPasstTimeDp)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_FREQ_CTRL_FCCHG_PASET_DAY_DP, &CFrequencyCtrlFcChg::OnDtnDatetimechangeFreqCtrlFcchgPasetDayDp)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_FREQ_CTRL_FCCHG_PASET_TIME_DP, &CFrequencyCtrlFcChg::OnDtnDatetimechangeFreqCtrlFcchgPasetTimeDp)
END_MESSAGE_MAP()


// CFrequencyCtrlFcChg メッセージ ハンドラー


/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CFrequencyCtrlFcChg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	Initialize();

	// 日時フォーマット変更
	m_PassStartDTCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_PassEndDTCtrl.SetFormat(_T("yyyy-MM-dd"));

	// 送信ドップラ補償
	for (int i = 0; mCtrlFcChgTxDopp[i] != _T(""); ++i)
	{
		m_TDoppCmb.InsertString(-1, mCtrlFcChgTxDopp[i]);
	}
	m_TDoppCmb.SetCurSel(0);
	m_nTxDopp = 0;

	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	CTime time = CTime(timeSys);

	// パス開始時刻
	m_PassStartDay = time;
	m_PassStartTime = time;

	// パス終了時刻
	m_PassEndDay = time;
	m_PassEndTime = time;

	UpdateData(FALSE);

	((CFrequencyCtrl*)mCallWnd)->GetTargetData().timeStart = time.GetTime();
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().timeStop = time.GetTime();
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().txDopp = m_nTxDopp;

	// カレントの予報値を設定する
	OnCbnSelchangeFreqCtrlFcchgSpcrCmb();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# ダイアログ更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CFrequencyCtrlFcChg::UpdateDlg()
{
	return TRUE;
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# コントロールの初期化

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlFcChg::Initialize()
{
	// 衛星名リストの作成
	vector<CString>::iterator itr;
	m_SpacecraftCmb.ResetContent();
	vector<CString>& stNameList = ((CFrequencyCtrl*)mCallWnd)->GetSatelliteNameList();
	for (itr = stNameList.begin(); itr != stNameList.end(); itr++){
		m_SpacecraftCmb.AddString((*itr));
	}
	// 取り合えず先頭の衛星アクティブにする
	m_SpacecraftCmb.SetCurSel(0);

	// カレントの予報値を設定する
	OnCbnSelchangeFreqCtrlFcchgSpcrCmb();
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# 衛星選択処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlFcChg::OnCbnSelchangeFreqCtrlFcchgSpcrCmb()
{
	UpdateData(TRUE);
	CString satellite;
	vector<CString>& stNameList = ((CFrequencyCtrl*)mCallWnd)->GetSatelliteNameList();
	vector<stPredListData>& allPred = ((CFrequencyCtrl*)mCallWnd)->GetAllPredList();
	vector<stPredListData>& partPred = ((CFrequencyCtrl*)mCallWnd)->GetPartPredList();
	if (stNameList.size() == 0)
		return;
	int pos = m_SpacecraftCmb.GetCurSel();
	satellite = stNameList[pos];
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().satellite = satellite;
	vector<stPredListData>::iterator itr;
	partPred.clear();
	for (itr = allPred.begin(); itr != allPred.end(); itr++){
		if (satellite.CompareNoCase((*itr).strSatellite) == 0){
			partPred.push_back((*itr));
		}
	}
	CString predfile = partPred[0].strPredPath;
	int idx = predfile.ReverseFind(_T('/'));
	predfile = predfile.Mid(idx + 1);
	idx = predfile.Find(_T("."));
	predfile = predfile.Mid(0, idx) + _T(".anp");
	m_AntPredBtnCtrl.SetWindowTextW(predfile);
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().predAnp = predfile;
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# アンテナ予報値ボタン押下時処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlFcChg::OnBnClickedFreqCtrlFcchgAntfcBtn()
{
	CString satellite;
	vector<CString>& stNameList = ((CFrequencyCtrl*)mCallWnd)->GetSatelliteNameList();
	if (stNameList.size() == 0)
		return;
	int pos = m_SpacecraftCmb.GetCurSel();
	satellite = stNameList[pos];
	theApp.m_pMainWnd->SendMessage(eMessage_OpenForecastListDlg, (WPARAM)this, (LPARAM)&satellite);
}

/*============================================================================*/
/*! CFrequencyCtrlSm

-# 予報値ファイルパスをボタンに表示処理

@param  message	：メッセージの識別子
@param  wParam	：メッセージの最初のパラメータ
@param  lParam	：メッセージの 2 番目のパラメータ
@retval 処理結果
*/
/*============================================================================*/
LRESULT CFrequencyCtrlFcChg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == eMessage_UpdateForecastFilePath)
	{
		//return TRUE;
		CString predfile = (*(CString*)lParam);
		predfile = predfile.Right(predfile.GetLength() - predfile.ReverseFind(_T('/')) - 1);

		int idx = predfile.Find(_T("."));
		predfile = predfile.Mid(0, idx) + _T(".anp");

		m_AntPredBtnCtrl.SetWindowTextW(predfile);
		((CFrequencyCtrl*)mCallWnd)->GetTargetData().predAnp = predfile;

		UpdateData(FALSE);
		return TRUE;
	}

	return CDialogBase::WindowProc(message, wParam, lParam);
}


/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# 予報値設定ボタンボタン押下時処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlFcChg::OnBnClickedFreqCtrlFcchgBtn()
{
	//CDlgWaitCmd wait;
	//wait.Open(_T("テスト"), _T("各種DBを読み込んでいます。\n\nしばらくおままちください"), false);
	//Sleep(10000);
	//wait.Close();

	UpdateData(TRUE);

	CString satellite;
	vector<CString>& stNameList = ((CFrequencyCtrl*)mCallWnd)->GetSatelliteNameList();
	vector<stPredListData>& allPred = ((CFrequencyCtrl*)mCallWnd)->GetAllPredList();
	vector<stPredListData>& partPred = ((CFrequencyCtrl*)mCallWnd)->GetPartPredList();
	if (stNameList.size() == 0)
		return;
	int pos = m_SpacecraftCmb.GetCurSel();
	satellite = stNameList[pos];
	CString predfile;
	m_AntPredBtnCtrl.GetWindowText(predfile);
	/****************************************************************/
	/* 制御情報 */
	CString ctrl = _T("");
	ctrl.Format(_T("CTRL.PRED_CHANGE_FREQ %s@%s"), (LPCTSTR)satellite, (LPCTSTR)predfile);
	theApp.SendControl(ctrl, _T(""), _T(""), _T(""));
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# 運用制御情報計算ボタンボタン押下時処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlFcChg::OnBnClickedFreqCtrlOpeBtn()
{
	const CString mCmdCtrl[] = {
		_T("ON"),
		_T("AVE"),
		_T("NOMINAL"),
		_T("STT"),
	};

	UpdateData(TRUE);
	CString satellite;
	vector<CString>& stNameList = ((CFrequencyCtrl*)mCallWnd)->GetSatelliteNameList();
	vector<stPredListData>& allPred = ((CFrequencyCtrl*)mCallWnd)->GetAllPredList();
	vector<stPredListData>& partPred = ((CFrequencyCtrl*)mCallWnd)->GetPartPredList();
	if (stNameList.size() == 0)
		return;
	int pos = m_SpacecraftCmb.GetCurSel();
	satellite = stNameList[pos];
	CString predfile;
	m_AntPredBtnCtrl.GetWindowText(predfile);
	/****************************************************************/
	/* 制御情報 */
	CString ctrl = _T("");
	ctrl.Format(_T("CTRL.PRED_GEN %s@%s@%s"), (LPCTSTR)satellite, (LPCTSTR)predfile, (LPCTSTR)mCmdCtrl[m_nTxDopp]);
	theApp.SendControl(ctrl, _T(""), _T(""), _T(""));
}


void CFrequencyCtrlFcChg::OnCbnSelchangeFreqCtrlFcchgTdpCmb()
{
	UpdateData(TRUE);
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().txDopp = m_nTxDopp;
}


void CFrequencyCtrlFcChg::OnDtnDatetimechangeFreqCtrlFcchgPasstDayDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	CTime time = CTime(m_PassStartDay.GetYear(), m_PassStartDay.GetMonth(), m_PassStartDay.GetDay(), m_PassStartTime.GetHour(), m_PassStartTime.GetMinute(), m_PassStartTime.GetSecond());
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().timeStart = time.GetTime();
	*pResult = 0;
}


void CFrequencyCtrlFcChg::OnDtnDatetimechangeFreqCtrlFcchgPasstTimeDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	CTime time = CTime(m_PassStartDay.GetYear(), m_PassStartDay.GetMonth(), m_PassStartDay.GetDay(), m_PassStartTime.GetHour(), m_PassStartTime.GetMinute(), m_PassStartTime.GetSecond());
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().timeStart = time.GetTime();
	*pResult = 0;
}


void CFrequencyCtrlFcChg::OnDtnDatetimechangeFreqCtrlFcchgPasetDayDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	CTime time = CTime(m_PassEndDay.GetYear(), m_PassEndDay.GetMonth(), m_PassEndDay.GetDay(), m_PassEndTime.GetHour(), m_PassEndTime.GetMinute(), m_PassEndTime.GetSecond());
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().timeStop = time.GetTime();
	*pResult = 0;
}


void CFrequencyCtrlFcChg::OnDtnDatetimechangeFreqCtrlFcchgPasetTimeDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	CTime time = CTime(m_PassEndDay.GetYear(), m_PassEndDay.GetMonth(), m_PassEndDay.GetDay(), m_PassEndTime.GetHour(), m_PassEndTime.GetMinute(), m_PassEndTime.GetSecond());
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().timeStop = time.GetTime();
	*pResult = 0;
}
