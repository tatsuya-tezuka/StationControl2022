/*============================================================================*/
/*! FrequencyCtrlCtrl54.cpp

-# 周波数制御画面-個別設定タブ(54m)
*/
/*============================================================================*/
// FrequencyCtrlCtrl54.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlCtrl54.h"
#include "afxdialogex.h"


// CFrequencyCtrlCtrl54 ダイアログ

IMPLEMENT_DYNAMIC(CFrequencyCtrlCtrl54, CFrequencyCtrlCtrl)

CFrequencyCtrlCtrl54::CFrequencyCtrlCtrl54(CWnd* pParent /*=NULL*/)
: CFrequencyCtrlCtrl(CFrequencyCtrlCtrl54::IDD, pParent)
{

}

CFrequencyCtrlCtrl54::~CFrequencyCtrlCtrl54()
{

}

void CFrequencyCtrlCtrl54::DoDataExchange(CDataExchange* pDX)
{
	CFrequencyCtrlCtrl::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FREQ_CTRL_CTRL_SET_UC_EDIT, m_strUC);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_SET_DC_EDIT, m_strXDC);
	DDX_Text(pDX, IDC_FREQ_CTRL_CTRL_SET_CMD_EDIT, m_strCmdCareer);
	DDX_Text(pDX, IDC_FREQ_CTRL_CTRL_OFS_TF_EDIT, m_strTFreqOffset);
	DDX_Text(pDX, IDC_FREQ_CTRL_CTRL_OFS_TO_EDIT, m_strTimeOffset);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DP_TD_CMB, m_TDopplerCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DP_RD_CMB, m_RDopplerCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DP_CMD_CMB, m_DopplerCmdCareerCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_SCM_CMB, m_SpacecraftCOHModeCtrl);
	DDX_Text(pDX, IDC_FREQ_CTRL_CTRL_OFS_TF_EDIT2, m_strRFreqOffset);
	DDX_Text(pDX, IDC_STATIC_FREQ_CTRL_DELTAF_MON, m_DeltafCtrlFreqMoni);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DELTAF_CTRL_EDIT, m_strDeltafCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_DELTAF_CTRLFREQ_CHECK, m_DeltafCtrlFreqChkBtn);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DEFTAF_CTRLFREQ_EDIT, m_strDeltafFreq);
	DDX_Control(pDX, IDC_FREQ_CTRL_DELTAF_CTRLTIME_CHECK, m_DeltafCtrlTimeChkBtn);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DELTAF_CTRLTIME_EDIT, m_strDeltafTime);
	DDX_Text(pDX, IDC_STATIC_FREQ_CTRL_DELTAF_ENDTIME, m_DeltafCtrlFreqEndTime);
	DDX_Check(pDX, IDC_FREQ_CTRL_DELTAF_CTRL_CHECK, m_DeltafCtrlFreqCtrChkBtn);
	DDX_Control(pDX, IDC_FREQ_CTRL_DELTAF_CTRL_CHECK, m_DeltafCtrlChkBtnCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DELTAF_START_BTN, m_DeltafStartBtn);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DELTAF_STOP_BTN, m_DeltafStopBtn);
	DDX_Control(pDX, IDC_FREQ_CTRL_DC_COH_CMB, m_DcCOHModeCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_SET_KADC_EDIT, m_strKaDC);
}

BEGIN_MESSAGE_MAP(CFrequencyCtrlCtrl54, CFrequencyCtrlCtrl)
	ON_BN_CLICKED(IDC_FREQ_CTRL_DELTAF_CTRL_CHECK, &CFrequencyCtrlCtrl54::OnBnClickedDeltafCtrlFreqCtrChk)
	ON_BN_CLICKED(IDC_FREQ_CTRL_DELTAF_CTRLFREQ_CHECK, &CFrequencyCtrlCtrl54::OnBnClickedDeltafCtrlFreqChk)
	ON_BN_CLICKED(IDC_FREQ_CTRL_DELTAF_CTRLTIME_CHECK, &CFrequencyCtrlCtrl54::OnBnClickedDeltafCtrlTimeChk)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_DELTAF_START_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlStartBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_DELTAF_STOP_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlStopBtn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_SCM_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlScmBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_SET_CMD_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlSetCmdBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_DC_COH_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlDcCOHBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_SET_UC_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlSetUcBtn)
	ON_CBN_SELCHANGE(IDC_FREQ_CTRL_DC_COH_CMB, &CFrequencyCtrlCtrl54::OnCbnSelchangeFreqCtrlDcCohCmb)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_OFS_TF_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlOfsTfBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_OFS_TF_BTN2, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlOfsTfBtn2)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_OFS_TO_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlOfsToBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_DP_TD_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlDpTdBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_DP_RD_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlDpRdBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_DP_CMD_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlDpCmdBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_SET_CMD_TIME_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlSetCmdTimeBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlBtn)
	ON_EN_CHANGE(IDC_FREQ_CTRL_CTRL_DELTAF_CTRLTIME_EDIT, &CFrequencyCtrlCtrl54::OnEnChangeFreqCtrlCtrlDeltafCtrltimeEdit)
	ON_EN_CHANGE(IDC_FREQ_CTRL_CTRL_OFS_TO_EDIT, &CFrequencyCtrlCtrl54::OnEnChangeFreqCtrlCtrlOfsToEdit)
END_MESSAGE_MAP()

/*============================================================================*/
// ダイアログ初期化処理
/*============================================================================*/
BOOL CFrequencyCtrlCtrl54::OnInitDialog()
{
	CFrequencyCtrlCtrl::OnInitDialog();

	// D/C COHモード
	for (int i = 0; i < sizeof(mDcCOHMode) / sizeof(mDcCOHMode[0]); i++){
		m_DcCOHModeCtrl.AddString(mDcCOHMode[i]);
	}
	m_DcCOHModeCtrl.SetCurSel(1);	//X-INCOH
	m_strXDC.EnableWindow(TRUE);
	m_strKaDC.EnableWindow(FALSE);

	// Δfリカバリ
	m_DeltafStopBtn.EnableWindow(FALSE);		//停止ボタン
	m_DeltafCtrlFreqCtrChkBtn = FALSE;			//Δf（制御）チェックボックス
	m_strDeltafCtrl.EnableWindow(FALSE);		//Δf（制御）入力非活性
	m_strDeltafFreq.SetWindowText(_T("0.1"));	//制御周波数
	m_strPreDeltafTime = _T("1");
	m_strDeltafTime.SetWindowText(m_strPreDeltafTime);		//制御時間
	m_strDeltafFreq.EnableWindow(FALSE);		//制御周波数 EDIT
	m_strDeltafTime.EnableWindow(FALSE);		//制御時間 EDIT
	m_RecoveryCont = FALSE;
	m_RngFlg = FALSE;
	m_strDeltafCtrlInit = FALSE;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
// ダイアログ更新
/*============================================================================*/
BOOL CFrequencyCtrlCtrl54::UpdateDlg()
{
	// 画面更新
	GetData();

	return TRUE;
}

/*============================================================================*/
// 画面更新
/*============================================================================*/
void CFrequencyCtrlCtrl54::GetData()
{
	CFrequencyCtrlCtrl54::stGetMonData mMonTable[] = {
		{ _T("CTRL.DELTA_FREQ"),       m_DeltafCtrlFreqMoni,    _T("%0.1f"), eDOUBLE },	// Δf（モニタ）
		{ _T("CTRL.RECOVER_END_TIME"), m_DeltafCtrlFreqEndTime, _T("%s"),    eSTRING },	// 終了予定時刻
		{ _T("CTRL.RECOVER_CONT"),     m_strRecoverCont,        _T("%s"),    eSTRING },	// RECOVERY OFF/ON
		{ _T("CTRL.RNG_FLG"),          m_strRngFlg,             _T("%s"),    eSTRING },	// Range計測中 終了/開始
	};

	// タイマ破棄
	KillTimer(TID_DELTAF_FREQ);

	UpdateData(TRUE);

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();
	CSatelliteData& sd = theApp.GetSatelliteData();

	/****************************************************************/
	/* 監視情報 */
	for (int index = 0; index < sizeof(mMonTable) / sizeof(mMonTable[0]); index++){
		// テーブルの監視名から監視データを取得して、コントロール値に設定する
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, mMonTable[index].monname);
		mMonTable[index].controlval.Empty();
		if (obs_adr != NULL){
			switch (mMonTable[index].type){
			case	eINT:
				mMonTable[index].controlval.Format(mMonTable[index].format, obs_adr->l_data);
				break;
			case	eDOUBLE:
				mMonTable[index].controlval.Format(mMonTable[index].format, obs_adr->d_data);
				break;
			case	eSTRING:
				mMonTable[index].controlval.Format(mMonTable[index].format, (LPCTSTR)CString(obs_adr->sz_sts_name));
				break;
			}
			if( (m_strDeltafCtrlInit == FALSE) && (index == 0) )
			{
				m_strDeltafCtrl.SetWindowText(m_DeltafCtrlFreqMoni);	//Δf(制御)の初期値は、監視データ「Δf」の値
				m_strDeltafCtrlInit = TRUE;								//初期値設定済
			}
		}
		else
		{
			//TRACE("GetObsname_t()エラー 監視名:%s\n", CStringA(mMonTable[index].monname));
		}
	}

//	//RECOVERY:ON or Range計測:開始 ならボタンや入力を非活性化
//	if( (m_strRecoverCont == _T("ON")) || (m_strRngFlg == _T("開始")) )
//	{
//		//=====================================================//
//		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
//		CString strDebug;
//		strDebug.Format(_T("Range Keisoku is on. station:%u, %s:%s, %s:%s, RecoveryContFlag:%d"),
//						   theApp.GetSelectStation(), (LPCTSTR)mMonTable[2].monname, (LPCTSTR)m_strRecoverCont,
//						   (LPCTSTR)mMonTable[3].monname, (LPCTSTR)m_strRngFlg, m_RecoveryCont);
//		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
//		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
//		//=====================================================//
//
//		if (m_RecoveryCont != TRUE)
//		{
//			//=====================================================//
//			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
//			CString strDebug;
//			strDebug.Format(_T("Range Keisoku is on. RecoveryCont is false. station:%u"), theApp.GetSelectStation());
//			CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
//			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
//			//=====================================================//
//
//			m_RecoveryCont = TRUE;
//			m_DeltafCtrlChkBtnCtrl.EnableWindow(FALSE);		//Δf (制御) CHECK
//			m_strDeltafCtrl.EnableWindow(FALSE);			//Δf (制御) EDIT
//			m_DeltafCtrlFreqChkBtn.EnableWindow(FALSE);		//制御周波数 CHECK
//			m_strDeltafFreq.EnableWindow(FALSE);			//制御周波数 EDIT
//			m_DeltafCtrlTimeChkBtn.EnableWindow(FALSE);		//制御時間 CHECK
//			m_strDeltafTime.EnableWindow(FALSE);			//制御時間 EDIT
//			m_DeltafStartBtn.EnableWindow(FALSE);			//開始
//			m_DeltafStopBtn.EnableWindow(TRUE);				//停止
//
//			// 再描画処理
//			InvalidateRect(NULL, TRUE);
//		}
//	}
//	//RECOVERY:OFF and Range計測:終了 ならボタンや入力を活性化
//	else
//	{
//		//=====================================================//
//		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
//		CString strDebug;
//		strDebug.Format(_T("Range Keisoku is off. station:%u, %s:%s, %s:%s, RecoveryContFlag:%d"),
//						theApp.GetSelectStation(), (LPCTSTR)mMonTable[2].monname, (LPCTSTR)m_strRecoverCont,
//						(LPCTSTR)mMonTable[3].monname, (LPCTSTR)m_strRngFlg, m_RecoveryCont);
//		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
//		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
//		//=====================================================//
//
//		if(m_RecoveryCont == TRUE)
//		{
//			//=====================================================//
//			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
//			CString strDebug;
//			strDebug.Format(_T("Range Keisoku is off. RecoveryCont is true. station:%u"), theApp.GetSelectStation());
//			CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
//			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
//			//=====================================================//
//
//			m_RecoveryCont = FALSE;
//			m_DeltafCtrlChkBtnCtrl.EnableWindow(TRUE);		//Δf (制御) CHECK
//			m_strDeltafCtrl.EnableWindow(TRUE);				//Δf (制御) EDIT
//			m_DeltafCtrlFreqChkBtn.EnableWindow(TRUE);		//制御周波数 CHECK
//			m_DeltafCtrlTimeChkBtn.EnableWindow(TRUE);		//制御時間 CHECK
//			if (m_DeltafCtrlTimeChkBtn.GetCheck() == 1)			//制御時間 にチェック
//			{
//				m_strDeltafTime.EnableWindow(TRUE);				//制御時間 EDIT
//				m_strDeltafFreq.EnableWindow(FALSE);			//制御周波数 EDIT
//			}
//			else if (m_DeltafCtrlFreqChkBtn.GetCheck() == 1)	//制御周波数 にチェック
//			{
//				m_strDeltafFreq.EnableWindow(TRUE);				//制御周波数 EDIT
//				m_strDeltafTime.EnableWindow(FALSE);			//制御時間 EDIT
//			}
//			m_DeltafStartBtn.EnableWindow(TRUE);			//開始
//			m_DeltafStopBtn.EnableWindow(FALSE);			//停止
//
//			// 再描画処理
//			InvalidateRect(NULL, TRUE);
//		}
//	}

	//Range計測:開始ならボタンや入力を全部非活性化
	if (m_strRngFlg == _T("開始"))
	{
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CString strDebug;
		strDebug.Format(_T("Range Keisoku is on. station:%u, %s:%s, %s:%s, RecoveryContFlag:%d"),
			theApp.GetSelectStation(), (LPCTSTR)mMonTable[2].monname, (LPCTSTR)m_strRecoverCont,
			(LPCTSTR)mMonTable[3].monname, (LPCTSTR)m_strRngFlg, m_RecoveryCont);
		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//

		// 直前がレンジ計測中でなかった場合、描画変更
		if (m_RngFlg != TRUE)
		{
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CString strDebug;
			strDebug.Format(_T("Range Keisoku is on. RecoveryCont is false. station:%u"), theApp.GetSelectStation());
			CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//

			// レンジ計測中フラグセット
			m_RngFlg = TRUE;
			m_DeltafCtrlChkBtnCtrl.EnableWindow(FALSE);		//Δf (制御) CHECK
			m_strDeltafCtrl.EnableWindow(FALSE);			//Δf (制御) EDIT
			m_DeltafCtrlFreqChkBtn.EnableWindow(FALSE);		//制御周波数 CHECK
			m_strDeltafFreq.EnableWindow(FALSE);			//制御周波数 EDIT
			m_DeltafCtrlTimeChkBtn.EnableWindow(FALSE);		//制御時間 CHECK
			m_strDeltafTime.EnableWindow(FALSE);			//制御時間 EDIT
			m_DeltafStartBtn.EnableWindow(FALSE);			//開始
			m_DeltafStopBtn.EnableWindow(FALSE);			//停止

			// 再描画処理
			InvalidateRect(NULL, TRUE);
		}
	}
	//Range計測:開始以外
	else
	{
		//RECOVERY:ONなら停止ボタンのみ活性化
		if (m_strRecoverCont == _T("ON"))
		{
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CString strDebug;
			strDebug.Format(_T("Range Keisoku is on. station:%u, %s:%s, %s:%s, RecoveryContFlag:%d"),
				theApp.GetSelectStation(), (LPCTSTR)mMonTable[2].monname, (LPCTSTR)m_strRecoverCont,
				(LPCTSTR)mMonTable[3].monname, (LPCTSTR)m_strRngFlg, m_RecoveryCont);
			CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//

			// 直前がレンジ計測中、またはリカバリ中でなかった場合、描画変更
			if (m_RngFlg == TRUE || m_RecoveryCont != TRUE)
			{
				//=====================================================//
				//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
				CString strDebug;
				strDebug.Format(_T("Range Keisoku is on. RecoveryCont is false. station:%u"), theApp.GetSelectStation());
				CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
				//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
				//=====================================================//

				// レンジ計測中フラグクリア
				m_RngFlg = FALSE;
				// リカバリーフラグセット
				m_RecoveryCont = TRUE;
				m_DeltafCtrlChkBtnCtrl.EnableWindow(FALSE);		//Δf (制御) CHECK
				m_strDeltafCtrl.EnableWindow(FALSE);			//Δf (制御) EDIT
				m_DeltafCtrlFreqChkBtn.EnableWindow(FALSE);		//制御周波数 CHECK
				m_strDeltafFreq.EnableWindow(FALSE);			//制御周波数 EDIT
				m_DeltafCtrlTimeChkBtn.EnableWindow(FALSE);		//制御時間 CHECK
				m_strDeltafTime.EnableWindow(FALSE);			//制御時間 EDIT
				m_DeltafStartBtn.EnableWindow(FALSE);			//開始
				m_DeltafStopBtn.EnableWindow(TRUE);				//停止

				// 再描画処理
				InvalidateRect(NULL, TRUE);
			}
		}
		//RECOVERY:OFFなら停止ボタン以外と入力を活性化。
		else
		{
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CString strDebug;
			strDebug.Format(_T("Range Keisoku is off. station:%u, %s:%s, %s:%s, RecoveryContFlag:%d"),
				theApp.GetSelectStation(), (LPCTSTR)mMonTable[2].monname, (LPCTSTR)m_strRecoverCont,
				(LPCTSTR)mMonTable[3].monname, (LPCTSTR)m_strRngFlg, m_RecoveryCont);
			CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//

			// 直前がレンジ計測中、またはリカバリー中だった場合、描画変更
			if (m_RngFlg == TRUE || m_RecoveryCont == TRUE)
			{
				//=====================================================//
				//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
				CString strDebug;
				strDebug.Format(_T("Range Keisoku is off. RecoveryCont is true. station:%u"), theApp.GetSelectStation());
				CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
				//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
				//=====================================================//

				// レンジ計測中フラグクリア
				m_RngFlg = FALSE;
				// リカバリ中フラグクリア
				m_RecoveryCont = FALSE;
				m_DeltafCtrlChkBtnCtrl.EnableWindow(TRUE);		//Δf (制御) CHECK
				m_strDeltafCtrl.EnableWindow(TRUE);				//Δf (制御) EDIT
				m_DeltafCtrlFreqChkBtn.EnableWindow(TRUE);		//制御周波数 CHECK
				m_DeltafCtrlTimeChkBtn.EnableWindow(TRUE);		//制御時間 CHECK
				if (m_DeltafCtrlTimeChkBtn.GetCheck() == 1)			//制御時間 にチェック
				{
					m_strDeltafTime.EnableWindow(TRUE);				//制御時間 EDIT
					m_strDeltafFreq.EnableWindow(FALSE);			//制御周波数 EDIT
				}
				else if (m_DeltafCtrlFreqChkBtn.GetCheck() == 1)	//制御周波数 にチェック
				{
					m_strDeltafFreq.EnableWindow(TRUE);				//制御周波数 EDIT
					m_strDeltafTime.EnableWindow(FALSE);			//制御時間 EDIT
				}
				m_DeltafStartBtn.EnableWindow(TRUE);			//開始
				m_DeltafStopBtn.EnableWindow(FALSE);			//停止

				// 再描画処理
				InvalidateRect(NULL, TRUE);
			}
		}
	}

	UpdateData(FALSE);

	// タイマ作成
	SetTimer(TID_DELTAF_FREQ, INTERVAL_DELTAF, 0);
}

/*============================================================================*/
// 探査機COHモード 設定ボタン処理
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlScmBtn()
{
	SetData(eControl_COH);
}

/*============================================================================*/
// D/C-COHモード 設定ボタン処理
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlDcCOHBtn()
{
	SetData(eControl_DC_COH);
}

/*============================================================================*/
// 設定周波数-U/C、X-D/C 、Ka-D/C 設定ボタン押下処理
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlSetUcBtn()
{
	bool bUC = SetData(eControl_UC, ePhase_chk);
	bool bX_UC = SetData(eControl_X_DC, ePhase_chk);
	bool bKa_DC = SetData(eControl_Ka_DC, ePhase_chk);

	if (bUC && bX_UC && bKa_DC)
	{
		// 入力チェックがOKならば送信
		SetData(eControl_UC, ePhase_send);
		SetData(eControl_X_DC, ePhase_send);
		SetData(eControl_Ka_DC, ePhase_send);
	}
}

/*============================================================================*/
// 設定周波数 - コマンドキャリア 設定ボタン押下処理
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlSetCmdBtn()
{
	SetData(eControl_CS);
}

/*============================================================================*/
// オフセット-送信周波数オフセット 設定ボタン押下処理
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlOfsTfBtn()
{
	SetData(eControl_TOFFSET);
}

/*============================================================================*/
// オフセット-受信周波数オフセット 設定ボタン押下処理
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlOfsTfBtn2()
{
	SetData(eControl_ROFFSET);
}

/*============================================================================*/
// オフセット-時刻オフセット 設定ボタン押下処理
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlOfsToBtn()
{
	SetData(eControl_TIMEOFFSET);
}

/*============================================================================*/
// ドップラ補償-送信ドップラ 設定ボタン処理
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlDpTdBtn()
{
	SetData(eControl_TDP);
}

/*============================================================================*/
// ドップラ補償-受信ドップラ 設定ボタン処理
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlDpRdBtn()
{
	SetData(eControl_RDP);
}

/*============================================================================*/
// ドップラ補償-コマンドキャリア 設定ボタン処理
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlDpCmdBtn()
{
	SetData(eControl_CDP);
}

/*============================================================================*/
// コマンドサブキャリア-現在時刻ボタン押下処理
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlSetCmdTimeBtn()
{
	SetData(eControl_TIMELOCK);
}

/*============================================================================*/
// 一括設定ボタン押下処理
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlBtn()
{
	CFrequencyCtrlCtrl54::SetData(eControl_ALLSET);
}

/*============================================================================*/
// 設定ボタン処理
/*============================================================================*/
void CFrequencyCtrlCtrl54::SetData(int type)
{
	if (SetData(type, ePhase_chk))
	{
		// 入力チェックがOKならば送信
		SetData(type, ePhase_send);
	}
}

/*============================================================================*/
// 設定ボタン処理
/*============================================================================*/
bool CFrequencyCtrlCtrl54::SetData(int type, int phase)
{
	CFrequencyCtrlCtrl54::stSetCtlData54 mCtlTable[] = {
		{ _T("DSNTCR.TRP_RATIO"), _T(""), _T(""), _T(""), _T(""), IDC_FREQ_CTRL_CTRL_SCM_CMB, eCOMBO },								// 探査機COHモード
		{ _T("X-DC.COH_INCOH"), _T(""), _T(""), _T(""), _T(""), IDC_FREQ_CTRL_DC_COH_CMB, eCOMBO },									// D/C COHモード
		{ _T("X-UC.FREQUENCY"), _T("X-SSPA.TX_FREQUENCY"), _T("X-DC.X-TX_FREQUENCY"), _T("CTRL.TX_DOPP_ZERO"), _T("U/C,SSPA,X-D/C_TX"), IDC_FREQ_CTRL_CTRL_SET_UC_EDIT, eEDIT },			// U/C, SSPA, X-D/C_TX
		{ _T("X-DC.X-RX_FREQUENCY"), _T("CTRL.RX_DOPP_ZERO"), _T(""), _T(""), _T("X-D/C_RX"), IDC_FREQ_CTRL_CTRL_SET_DC_EDIT, eEDIT },		// X-D/C_RX
		{ _T("X-DC.Ka-RX_FREQUENCY"), _T("CTRL.RX_DOPP_ZERO"), _T(""), _T(""), _T("Ka-D/C"), IDC_FREQ_CTRL_CTRL_SET_KADC_EDIT, eEDIT },	// Ka-D/C
		{ _T("DSNTCR.TX_SC_FREQ"), _T("DSNTCR.TX_SC_DOPP_PRE"), _T(""), _T(""), _T("コマンドサブキャリア"), IDC_FREQ_CTRL_CTRL_SET_CMD_EDIT, eEDIT },	// コマンドサブキャリア
		{ _T("CTRL.CMD_SC_NOW"), _T(""), _T(""), _T(""), _T(""), IDC_FREQ_CTRL_CTRL_SET_CMD_TIME_BTN, eNONE },						// 現在時刻ベストロック
		{ _T("CTRL.DOPP_MODE"), _T(""), _T(""), _T(""), _T(""), IDC_FREQ_CTRL_CTRL_DP_TD_CMB, eCOMBO },								// 送信ドップラ
		{ _T("XX-DSNTCRFC.1WAY/2WAY"), _T(""), _T(""), _T(""), _T(""), IDC_FREQ_CTRL_CTRL_DP_RD_CMB, eCOMBO },						// 受信ドップラ
		{ _T("CTRL.SC_DOPP_MODE"), _T(""), _T(""), _T(""), _T(""), IDC_FREQ_CTRL_CTRL_DP_CMD_CMB, eCOMBO },							// コマンドサブキャリア
		{ _T("XX-DSNTCRFC.FREQ_OFFSET_U"), _T(""), _T(""), _T(""), _T("送信周波数オフセット"), IDC_FREQ_CTRL_CTRL_OFS_TF_EDIT, eEDIT },					// 送信周波数オフセット
		{ _T("XX-DSNTCRFC.FREQ_OFFSET"), _T(""), _T(""), _T(""), _T("受信周波数オフセット"), IDC_FREQ_CTRL_CTRL_OFS_TF_EDIT2, eEDIT },					// 受信周波数オフセット
		{ _T("XX-DSNTCRFC.TIME_OFFSET"), _T(""), _T(""), _T(""), _T(""), IDC_FREQ_CTRL_CTRL_OFS_TO_EDIT, eEDIT },					// 時刻オフセット
	};

	const CString strTimeOffsetErrMsg = _T("時刻オフセットの分秒指定(±M:SS)が間違っています。");

	UpdateData(TRUE);

	/****************************************************************/
	/* 制御情報 */
	CString ctrl, val;
	bool bRet = true;
	bool bAll = false;
	if (type == eControl_ALLSET)
		bAll = true;

	for (int item = 0; item < eControl_ALLSET; item++){
		if (bAll == true){
			type = item;
		}
		for (int i = 0; i < 4; i++){
			bool bRangeChk = false;
			switch (type){
			case	eControl_COH:
				ctrl = mCtlTable[type].ctlname1;
				val = mSatMode[m_SpacecraftCOHModeCtrl.GetCurSel()];
				i = 4;
				break;
			case	eControl_DC_COH:
				ctrl = mCtlTable[type].ctlname1;
				val = mDcCOHModeCtl[m_DcCOHModeCtrl.GetCurSel()];	//表示名と制御名は違う
				i = 4;
				break;
			case	eControl_UC:
				switch (i)
				{
				case 0:
					ctrl = mCtlTable[type].ctlname1;
					val = m_strUC;
					bRangeChk = true;								//範囲値チェック
					break;
				case 1:
					ctrl = mCtlTable[type].ctlname2;
					val = m_strUC;
					bRangeChk = true;								//範囲値チェック
					break;
				case 2:
					ctrl = mCtlTable[type].ctlname3;
					val = m_strUC;
					bRangeChk = true;								//範囲値チェック
					break;
				case 3:
					ctrl = mCtlTable[type].ctlname4;
					val = _T(",");
					break;
				default:
					break;
				}
				break;
			case	eControl_X_DC:
				val = mDcCOHMode[m_DcCOHModeCtrl.GetCurSel()];
				if (val == _T("X-INCOH"))
				{
					if (i == 0){
						ctrl = mCtlTable[type].ctlname1;
						m_strXDC.GetWindowText(val);
						bRangeChk = true;								//範囲値チェック
					}
					else{
						ctrl = mCtlTable[type].ctlname2;
						val = _T(",");
						i = 4;
					}
				}
				else
				{
					continue;	//X-INCOH以外は要求を送信しない
				}
				break;
			case	eControl_Ka_DC:
				val = mDcCOHMode[m_DcCOHModeCtrl.GetCurSel()];
				if (val == _T("Ka-INCOH"))
				{
					if (i == 0){
						ctrl = mCtlTable[type].ctlname1;
						m_strKaDC.GetWindowText(val);
						bRangeChk = true;								//範囲値チェック
					}
					else{
						ctrl = mCtlTable[type].ctlname2;
						val = _T(",");
						i = 4;
					}
				}
				else if (val != _T("X-INCOH"))	//X-INCOHでもKa-INCOHでもない場合は、"CTRL.RX_DOPP_ZERO"のみ送信
				{
					ctrl = mCtlTable[type].ctlname2;
					val = _T(",");
					i = 4;
				}
				else
				{
					continue;	//Ka-INCOH以外は要求を送信しない
				}
				break;
			case	eControl_CS:
				if (i == 0){
					ctrl = mCtlTable[type].ctlname1;
					val = m_strCmdCareer;
					bRangeChk = true;								//範囲値チェック
				}
				else{
					ctrl = mCtlTable[type].ctlname2;
					val = _T("OFF");
					i = 4;
				}
				break;
			case	eControl_TIMELOCK:
				ctrl = mCtlTable[type].ctlname1;
				val = _T(",");
				i = 4;
				break;
			case	eControl_TDP:
				ctrl = mCtlTable[type].ctlname1;
				val = mCmdCtrl[m_TDopplerCtrl.GetCurSel()];
				i = 4;
				break;
			case	eControl_RDP:
				ctrl = mCtlTable[type].ctlname1;
				val = mRxDopp[m_RDopplerCtrl.GetCurSel()];
				i = 4;
				break;
			case	eControl_CDP:
				ctrl = mCtlTable[type].ctlname1;
				val = mCmdCtrl[m_DopplerCmdCareerCtrl.GetCurSel()];
				i = 4;
				break;
			case	eControl_TOFFSET:
				ctrl = mCtlTable[type].ctlname1;
				val = m_strTFreqOffset;
				bRangeChk = true;								//範囲値チェック
				i = 4;
				break;
			case	eControl_ROFFSET:
				ctrl = mCtlTable[type].ctlname1;
				val = m_strRFreqOffset;
				bRangeChk = true;								//範囲値チェック
				i = 4;
				break;
			case	eControl_TIMEOFFSET:
				ctrl = mCtlTable[type].ctlname1;
				{
					// ★M:SS時刻フォーマットチェック
					int min, sec;
					TCHAR sz_time_in[6], sz_time_chk[6];
					int sz = 6;
					CString strTimeOffset, strSign, strChk;

					bool bFormatErr = false;
					if (m_strTimeOffset.IsEmpty() == true || m_strTimeOffset.GetLength() >= sz)		// サイズ（終端文字分'\0'を考慮）チェック
					{
						// 入力フォーマットが異なる
						bFormatErr = true;
					}
					else
					{
						strChk = m_strTimeOffset;
						if (strChk.Remove(_T(':')) > 1)
						{
							// 入力フォーマットが異なる
							bFormatErr = true;
						}
						else
						{
							strTimeOffset = m_strTimeOffset;

							// 符号チェック
							if (strTimeOffset.Left(1) == _T("+"))
							{
								strSign = _T("+");
								strTimeOffset.TrimLeft(_T('+'));
							}
							else if (strTimeOffset.Left(1) == _T("-"))
							{
								strSign = _T("-");
								strTimeOffset.TrimLeft(_T('-'));
							}
							else if (strTimeOffset.Left(1) == _T(' '))
							{
								strSign = _T("+");
								strTimeOffset.TrimLeft(_T(' '));
							}
							else
							{
								strSign = _T("+");
							}

							if (wcscpy_s(sz_time_in, sz, (LPCTSTR)strTimeOffset) != 0)
							{
								// 入力フォーマットが異なる
								bFormatErr = true;
							}
							else
							{
								// m:ss形式
								if (swscanf_s(sz_time_in, L"%01d:%d", &min, &sec) == 2)
								{
									if ((min < 0) || (min  > 9) ||
										(sec < 0) || (sec  > 59))
									{
										// 入力フォーマットが異なる
										bFormatErr = true;
									}
								}
								else
								{
									// 入力フォーマットが異なる
									bFormatErr = true;
								}
							}
						}
					}

					if (phase == ePhase_chk)
					{
						// 制御送信前チェックフェーズ
						if (bFormatErr)
						{
							// フォーマットエラーならば制御送信しない
							MessageBox(strTimeOffsetErrMsg, 0, MB_OK | MB_ICONERROR);
							i = 4;
							bRet = false;
							continue;
						}
					}

					swprintf_s(sz_time_chk, sz, L"%s%01d:%02d", (LPCTSTR)strSign, min, sec);
					m_strTimeOffset = sz_time_chk;
					//TRACE("日時指定:%s\n", CStringA(m_strTimeOffset));
				}
				val = m_strTimeOffset;
				i = 4;
				break;
			case	eControl_ALLSET:
				break;
			}
			
			if (phase == ePhase_chk)
			{
				// 制御送信前チェックフェーズ
				CString title = mCtlTable[type].title;
				if (bRangeChk == true && CheckCtrlValRange(ctrl, val, title) == false)
				{
					// 制御送信しない
					i = 4;
					bRet = false;
					continue;
				}
			}
			else
			{
				// 制御送信フェーズ
				TRACE("[%s] [%s]\n", CStringA(ctrl), CStringA(val));
				theApp.SendControl(ctrl, val, _T(""), _T(""));
			}
		}
		if (bAll == false)
			break;
	}

	UpdateData(FALSE);

	return bRet;
}

/*============================================================================*/
// タイマ処理
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_DELTAF_FREQ)
	{
		GetData();
	}
	CDialogBase::OnTimer(nIDEvent);
}

/*============================================================================*/
// CFrequencyCtrlCtrl54 メッセージ ハンドラー
/*============================================================================*/

/*============================================================================*/
// Δf（制御）チェックボックス
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedDeltafCtrlFreqCtrChk()
{
	UpdateData();

	if(m_DeltafCtrlFreqCtrChkBtn)
	{
		// Δf（制御）入力活性
		m_strDeltafCtrl.EnableWindow(TRUE);
	}
	else
	{
		// Δf（制御）入力非活性
		m_strDeltafCtrl.EnableWindow(FALSE);
	}
}

/*============================================================================*/
//Δfリカバリ 制御周波数チェックボックス
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedDeltafCtrlFreqChk()
{
	m_DeltafCtrlFreqChkBtn.SetCheck(1);
	m_DeltafCtrlTimeChkBtn.SetCheck(0);
	m_strDeltafFreq.EnableWindow(TRUE);				//制御周波数 EDIT
	m_strDeltafTime.EnableWindow(FALSE);			//制御時間 EDIT
}

/*============================================================================*/
//Δfリカバリ 制御時間チェックボックス
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedDeltafCtrlTimeChk()
{
	m_DeltafCtrlFreqChkBtn.SetCheck(0);
	m_DeltafCtrlTimeChkBtn.SetCheck(1);
	m_strDeltafTime.EnableWindow(TRUE);				//制御時間 EDIT
	m_strDeltafFreq.EnableWindow(FALSE);			//制御周波数 EDIT
}

/*============================================================================*/
//Δfリカバリ 開始ボタン
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlStartBtn()
{
	// ①ラジオボタンで「制御時間」が選択された場合
	//   CTRL.DF_START,DFxxxxxxxxx@Tyyyyyyyyy,;
	// ②ラジオボタンで「制御周波数」が選択された場合
	//   CTRL.DF_START,DFxxxxxxxxx@Rzzzzzzzzz,;
	// xxxxxxxxx:Δf(制御)チェックボックスが
	//   チェックされている場合は、その表示値を設定[Hz]
	//   チェックされていない場合は、「-」を設定
	// yyyyyyyyy:制御時間欄の表示値を設定[sまたは、YYYY-MM-DD_hh:mm:ss]
	// zzzzzzzzz:制御周波数欄の表示値を設定[Hz/s]
	CString deltaFCtrl;	//xxxxxxxxx
	CString ctrlTime;	//yyyyyyyyy
	CString ctrlFreq;	//zzzzzzzzz
	CString strCtrl = _T("");
	BOOL check;

	if (m_DeltafCtrlFreqCtrChkBtn)
	{
		m_strDeltafCtrl.GetWindowText(deltaFCtrl);
		check = CheckFrequencyInput(deltaFCtrl);
		if(check != TRUE)
		{
			CString str;
			str.Format(_T("Δf（制御）は0.0を除く±4000000.0以内、かつ絶対値にした場合にモニタ値%sの絶対値以内を入力してください。"), m_DeltafCtrlFreqMoni);
			MessageBox(str, 0, MB_OK | MB_ICONERROR);
			return;
		}
	}
	else
	{
		deltaFCtrl = _T("-");
	}
	strCtrl = _T("CTRL.DF_START,DF") + deltaFCtrl;	//"CTRL.DF_START,DFxxxxxxxxx"

	if (m_DeltafCtrlTimeChkBtn.GetCheck() == 1)	//制御時間 にチェック
	{
		int year, mon, day, hour, min, sec;
		TCHAR sz_time_in[40], sz_time_chk[40];
		int sz = 40;

		m_strDeltafTime.GetWindowText(ctrlTime);
		if (ctrlTime.GetLength() >= sz || wcscpy_s(sz_time_in, sz, (LPCTSTR)ctrlTime) != 0)		// サイズ（終端文字分'\0'を考慮）チェック後に、文字列コピー
		{
			MessageBox(_T("制御時間の日時指定が間違っています。"), 0, MB_OK | MB_ICONERROR);
			return;
		}

		//YYYY-MM-DD hh:mm:ss形式
		if(swscanf_s(sz_time_in, L"%04d-%02d-%02d %02d:%02d:%02d", &year, &mon, &day, &hour, &min, &sec) == 6)
		{
			if( (year < 2019) || (year > 2099) ||
				(mon  < 1)    || (mon  > 12)   ||
				(day  < 1)    || (day  > 31)   ||
				(hour < 0)    || (hour > 23)   ||
				(min  < 0)    || (min  > 59)   ||
				(sec  < 0)    || (sec  > 59)   )
			{
				MessageBox(_T("制御時間の日時指定が間違っています。"), 0, MB_OK | MB_ICONERROR);
				return;
			}
			//YYYY-MM-DD hh:mm:ss を YYYY-MM-DD_hh:mm:ss に変換 (スペースをアンダーバーに変更)
			swprintf_s(sz_time_chk, sz, L"%04d-%02d-%02d_%02d:%02d:%02d", year, mon, day, hour, min, sec);
			ctrlTime = sz_time_chk;
			//TRACE("日時指定:%s\n", CStringA(ctrlTime));
		}
		//秒形式
		else if(swscanf_s(sz_time_in, L"%d", &sec) == 1)
		{
			swprintf_s(sz_time_chk, sz, L"%d", sec);
			check = FALSE;
			if(_tcscmp(sz_time_in, sz_time_chk) == 0)
			{
				check = CheckTimeSec(ctrlTime);
			}
			if(check != TRUE)
			{
				MessageBox(_T("制御時間は1～86400[秒]を入力してください。"), 0, MB_OK | MB_ICONERROR);
				return;
			}
			//TRACE("秒指定:%d\n", CStringA(ctrlTime));
		}
		else
		{
			MessageBox(_T("制御時間は日時指定か秒を入力してください。"), 0, MB_OK | MB_ICONERROR);
			return;
		}
		strCtrl += _T("@T") + ctrlTime + _T(",;");	//"CTRL.DF_START,DFxxxxxxxxx@Tyyyyyyyyy,;"
	}
	else if (m_DeltafCtrlFreqChkBtn.GetCheck() == 1)	//制御周波数 にチェック
	{
		m_strDeltafFreq.GetWindowText(ctrlFreq);
		check = CheckFrequencyRate(ctrlFreq);
		if(check != TRUE)
		{
			MessageBox(_T("制御周波数は0.1～4000000.0を入力してください。"), 0, MB_OK | MB_ICONERROR);
			return;
		}
		strCtrl += _T("@R") + ctrlFreq + _T(",;");	//"CTRL.DF_START,DFxxxxxxxxx@Rzzzzzzzzz,;"
	}
	else
	{
		MessageBox(_T("制御周波数か制御時間を指定してください。"), 0, MB_OK | MB_ICONERROR);
		return;
	}

	// 開始」ボタンがクリックされた場合、以下の制御文字列を生成し送信する。
	//TRACE("%s\n", CStringA(strCtrl));
	theApp.SendControlNoUI(strCtrl, _T(""), _T(""), _T(""));
}

/*============================================================================*/
//Δfリカバリ 停止ボタン
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlStopBtn()
{
	// 停止」ボタンがクリックされた場合、以下の制御文字列を生成し送信する。
	//TRACE("CTRL.DF_STOP,,;\n");
	theApp.SendControlNoUI(_T("CTRL.DF_STOP,,;"), _T(""), _T(""), _T(""));
}

/*============================================================================*/
//D/C COHモード コンボボックス
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnCbnSelchangeFreqCtrlDcCohCmb()
{
	CString val;

	val = mDcCOHMode[m_DcCOHModeCtrl.GetCurSel()];
	if(val == _T("X-INCOH"))
	{
		m_strXDC.EnableWindow(TRUE);
	}
	else
	{
		m_strXDC.EnableWindow(FALSE);
	}
	if(val == _T("Ka-INCOH"))
	{
		m_strKaDC.EnableWindow(TRUE);
	}
	else
	{
		m_strKaDC.EnableWindow(FALSE);
	}
}

/*============================================================================*/
//制御時間(秒) 範囲チェック
/*============================================================================*/
BOOL CFrequencyCtrlCtrl54::CheckTimeSec(CString second)
{
	int val;

	val = _wtoi(second);
	if( (val < 1) || (val > 86400) )
	{
		return FALSE;
	}

	return TRUE;
}

/*============================================================================*/
//制御周波数 範囲チェック
/*============================================================================*/
BOOL CFrequencyCtrlCtrl54::CheckFrequencyRate(CString freq)
{
	double val;

	val = _wtof(freq);
	if( (val < 0.1) || (val > 4000000.0) )
	{
		return FALSE;
	}

	return TRUE;
}

/*============================================================================*/
//Δf（制御） 範囲チェック
/*============================================================================*/
BOOL CFrequencyCtrlCtrl54::CheckFrequencyInput(CString freq)
{
	double val, monVal;

	val = _wtof(freq);
	monVal = _wtof(m_DeltafCtrlFreqMoni);
	if ((val < -4000000.0) || (val > 4000000.0) || (val == 0.0) || fabs(val) > fabs(monVal))
	{
		return FALSE;
	}

	return TRUE;
}



void CFrequencyCtrlCtrl54::OnEnChangeFreqCtrlCtrlDeltafCtrltimeEdit()
{
	UpdateData(TRUE);

	CString strDeltafTime, strDeltafTimeChk;
	m_strDeltafTime.GetWindowText(strDeltafTime);
	strDeltafTimeChk = strDeltafTime;

	// 文字列から数字0～9, '-', ':', 半角空白があれば削除し、残った文字があれば禁止文字有り
	WCHAR szPermitMoji[] = {
		_T('0'), _T('1'), _T('2'), _T('3'), _T('4'), _T('5'), _T('6'), _T('7'), _T('8'), _T('9'), _T('-'), _T(':'), _T(' ')
	};

	for (int i = 0; i < sizeof(szPermitMoji) / sizeof(szPermitMoji[0]); i++)
	{
		strDeltafTimeChk.Remove(szPermitMoji[i]);
	}

	if (strDeltafTimeChk.IsEmpty() == false)
	{
		// 禁止文字が残っているため、ワーニングを表示し、入力前の値に戻す
		CString strMsg;
		strMsg.Format(_T("文字[%s]は入力できません。\nTIMEは日時指定か、整数値の秒を入力してください。"), strDeltafTimeChk);
		MessageBox(strMsg, 0, MB_OK | MB_ICONERROR);
		m_strDeltafTime.SetWindowTextW(m_strPreDeltafTime);
		UpdateData(FALSE);
	}
	else
	{
		// 禁止文字が無い場合
		m_strPreDeltafTime = strDeltafTime;
	}
}


void CFrequencyCtrlCtrl54::OnEnChangeFreqCtrlCtrlOfsToEdit()
{
	UpdateData(TRUE);

	CString strTimeChk = m_strTimeOffset;

	// 文字列から数字0～9, '-', '+', ':'があれば削除し、残った文字があれば禁止文字有り
	WCHAR szPermitMoji[] = {
		_T('0'), _T('1'), _T('2'), _T('3'), _T('4'), _T('5'), _T('6'), _T('7'), _T('8'), _T('9'), _T('-'), _T('+'), _T(':')
	};

	for (int i = 0; i < sizeof(szPermitMoji) / sizeof(szPermitMoji[0]); i++)
	{
		strTimeChk.Remove(szPermitMoji[i]);
	}

	if (strTimeChk.IsEmpty() == false)
	{
		// 禁止文字が残っているため、ワーニングを表示し、入力前の値に戻す
//		CString strMsg;
//		strMsg.Format(_T("文字[%s]は入力できません。\n時刻オフセットは分秒指定(±M:SS)を入力してください。"), strTimeChk);
//		MessageBox(strMsg, 0, MB_OK | MB_ICONERROR);
		m_strTimeOffset = m_strTimeOffsetPre;
		UpdateData(FALSE);
	}
	else
	{
		// 禁止文字が無い場合
		m_strTimeOffsetPre = m_strTimeOffset;
	}
}
