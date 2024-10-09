/*============================================================================*/
/*! FrequencyCtrlMon54.cpp

-# 周波数制御画面-周波数モニタタブ(54m)
*/
/*============================================================================*/
#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlMon54.h"
#include "afxdialogex.h"

#include "FrequencyCtrl.h"
#include "SatelliteData.h"


// CFrequencyCtrlMon54 ダイアログ

IMPLEMENT_DYNAMIC(CFrequencyCtrlMon54, CFrequencyCtrlMon)

/*============================================================================*/
// コンストラクタ
/*============================================================================*/
CFrequencyCtrlMon54::CFrequencyCtrlMon54(CWnd* pParent /*=NULL*/)
: CFrequencyCtrlMon(CFrequencyCtrlMon54::IDD, pParent)
{

}

/*============================================================================*/
// デストラクタ
/*============================================================================*/
CFrequencyCtrlMon54::~CFrequencyCtrlMon54()
{
}

void CFrequencyCtrlMon54::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_SET_UC_EDIT, m_strUC);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_SET_DC_EDIT, m_strDC);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_SET_KADC_EDIT, m_strKaDC);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_DC_COH_EDIT, m_strDCCOHMode);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_SET_CMD_EDIT, m_strCmdCareer);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_OFS_TF_EDIT, m_strTFreqOffset);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_OFS_TO_EDIT, m_strTimeOffset);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_FCV_RU_EDIT, m_strForecastRTTUp);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_FCV_RD_EDIT, m_strForecastRTTDw);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_DP_TD_EDIT, m_strTDoppler);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_DP_RD_EDIT, m_strRDoppler);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_DP_CMD_EDIT, m_strDopplerCmdCareer);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_SCM_EDIT, m_strSpacecraftCOHMode);
	DDX_Control(pDX, IDC_FREQ_CTRL_MONITOR_PRED_DAY_DP, m_PredTimeDayCtrl);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_MONITOR_PRED_DAY_DP, m_tPredDay);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_MONITOR_PRED_TIME_DP, m_tPredTime);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_PRE_TX_EDIT, m_strPreFreqTx);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_PRE_RX_EDIT, m_strPreFreqRx);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_PRE_SC_EDIT, m_strPreFreqSc);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_OFS_TF2_EDIT, m_strRFreqOffset);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrlMon54, CFrequencyCtrlMon)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFrequencyCtrlMon54 メッセージ ハンドラー

/*============================================================================*/
// ダイアログ初期化処理
/*============================================================================*/
BOOL CFrequencyCtrlMon54::OnInitDialog()
{
	CFrequencyCtrlMon::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
// ダイアログ更新処理
/*============================================================================*/
BOOL CFrequencyCtrlMon54::UpdateDlg()
{
	// 画面更新
	SetData();

	return TRUE;
}

/*============================================================================*/
// 画面更新
/*============================================================================*/
void CFrequencyCtrlMon54::SetData()
{
	CString mPreFreqTx;	// 予測送信周波数(TX)の計算用
	CString mPreFreqRx;	// 予測送信周波数(RX)の計算用
	CString mPreFreqSc;	// 予測送信周波数(SC)の計算用
	CString mPredFile;	// 予報値ファイル名

	CFrequencyCtrlMon54::stSetMonData mMonTable[] = {
		{ _T("X-UC.FREQUENCY"), m_strUC, _T("%0.1f"), eDOUBLE },					// 設定周波数（U/C）
		{ _T("X-DC.X-RX_FREQUENCY"), m_strDC, _T("%0.1f"), eDOUBLE },				// 設定周波数（X-D/C）
		{ _T("X-DC.Ka-RX_FREQUENCY"), m_strKaDC, _T("%0.1f"), eDOUBLE },			// 設定周波数（Ka-D/C）
		{ _T("X-DC.COH_INCOH"), m_strDCCOHMode, _T("%s"), eSTRING },				// D/C COHモード
		{ _T("DSNTCR.TX_SC_FREQ"), m_strCmdCareer, _T("%0.1f"), eDOUBLE },			// 設定周波数（コマンドサブキャリア）
		{ _T("CTRL.TX_DOPP_MODE"), m_strTDoppler, _T("%s"), eSTRING },				// ドップラ補償（送信ドップラ）
		{ _T("CTRL.MODEM_12WAY"), m_strRDoppler, _T("%s"), eSTRING },				// ドップラ補償（受信ドップラ）
		{ _T("CTRL.CMD_SC_DOPP_MODE"), m_strDopplerCmdCareer, _T("%s"), eSTRING },	// ドップラ補償（コマンドサブキャリア）
		{ _T("CTRL.FREQ_OFFSET_UL"), m_strTFreqOffset, _T("%0.1f"), eDOUBLE },		// オフセット（送信周波数オフセット）
		{ _T("CTRL.FREQ_OFFSET_DL"), m_strRFreqOffset, _T("%0.1f"), eDOUBLE },		// オフセット（受信周波数オフセット）
		{ _T("CTRL.PRED_TIME_OFFSET"), m_strTimeOffset, _T("%s"), eSTRING },		// オフセット（時刻オフセット）
		{ _T("DSNTCR.TRP_RATIO"), m_strSpacecraftCOHMode, _T("%s"), eSTRING },		// 探査機COHモード

		{ _T("DSNTCR.TX_CARR_FREQ_DEV_MON"), mPreFreqTx, _T("%0.1f"), eDOUBLE },	// 予測送信周波数の計算用
		{ _T("DSNTCR.RX_CARR_FREQ_DEV_MON"), mPreFreqRx, _T("%0.1f"), eDOUBLE },	// 予測送信周波数の計算用
		{ _T("DSNTCR.TX_SC_DEV_MON"), mPreFreqSc, _T("%0.1f"), eDOUBLE },			// 予測送信周波数の計算用

		{ _T("CTRL.PRED_FILE"), mPredFile, _T("%s"), eSTRING },						// 予報値ファイル名
	};

	// タイマ破棄
	KillTimer(TID_FREQMON);

	UpdateData(TRUE);

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();
	CSatelliteData& sd = theApp.GetSatelliteData();

	CString str;

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
		}
		else
		{
			//TRACE("GetObsname_t()エラー 監視名:%s\n", CStringA(mMonTable[index].monname));
		}
	}

	/****************************************************************/
	/* 予測周波数 */
	// TX
	double fval = _wtoi(m_strUC) + _wtoi(mPreFreqTx);
	m_strPreFreqTx.Format(_T("%0.1f"), fval);

	// RX
	fval = _wtoi(m_strDC) + _wtoi(mPreFreqRx);
	m_strPreFreqRx.Format(_T("%0.1f"), fval);

	// SC
	fval = _wtoi(m_strCmdCareer) + _wtoi(mPreFreqSc);
	m_strPreFreqSc.Format(_T("%0.1f"), fval);

	/****************************************************************/
	/* 予報値情報 */
	// 日時
	CTime time;
	theApp.GetCurrentUtcTime(time);
	m_tPredDay = time;
	m_tPredTime = time;

#ifdef _DEBUG
	mPredFile = _T("C:/share/udsc54/ctrl/prd/hayabusa2/1708270400.prd");
	mPredFile.MakeUpper();
#endif

	CString satName, anpName;
	bool bPredOk = _convertPredFileName(mPredFile, satName, anpName);

	if (satName.IsEmpty() || anpName.IsEmpty()){
		// 対象ファイルを以前のファイルで表示する
		satName = mTargetSatellite;
		anpName = mTargetPredAnp;
	}

	if (mTargetSatellite != satName){
		GetEqstatDbValue(satName);
		mTargetSatellite = satName;
	}
	if (mTargetPredAnp != anpName){
		mTargetPredAnp = anpName;
	}

	((CFrequencyCtrl*)mCallWnd)->GetAnpData(satName, anpName);
	vector<CFrequencyCtrl::stAnpData>& anp = ((CFrequencyCtrl*)mCallWnd)->GetAnpDataList();
	int anpsize = (int)anp.size();
	if (anpsize == 0){
		UpdateData(FALSE);
		SetTimer(TID_FREQMON, INTERVAL_FREQMON, 0);
		return;
	}

	// 予報値ファイルを読み込む
	CString anppath, anpfile;
	anppath = theApp.GetShareFilePath(eFileType_Pred_Folder, theApp.GetSelectStation());
	anpfile.Format(_T("%s%s\\%s"), (LPCTSTR)anppath, (LPCTSTR)satName, (LPCTSTR)anpName);
	double	d_aos;
	double	d_los;
	errno_t err = 0;
	bool bNewFormat = false;
	if (err != 0 || mTrendData.load_pred_data((char*)(const char*)CStringA(anpfile), &d_aos, &d_los, &bNewFormat) != 0){
		// エラー
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CString msg;
		msg.Format(_T("ANP(%s) open error"), (LPCTSTR)anpfile);
		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlMon"), _T("SetData"), msg, _T(""), nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
	}
	else{
		if (bNewFormat == true){
			// RTT(Up Link)
			// 2WAY距離 / 光速
			double db2Way = mTrendData.GetUpDownLinkRange(time.GetTime(), 2);
			m_strForecastRTTUp.Format(_T("%0.3f"), db2Way / V_C);

			// RTT(Down Link)
			// アップリンク距離 x 2 / 光速
			double dbUplinkRange = mTrendData.GetUpDownLinkRange(time.GetTime(), 0);
			m_strForecastRTTDw.Format(_T("%0.3f"), (dbUplinkRange * 2) / V_C);
		}
		else{
			// RTT(Up Link)
			// アップリンク距離 x 2 / 光速
			double dbUplinkRange = mTrendData.GetUpDownLinkRange(time.GetTime(), 0);
			m_strForecastRTTUp.Format(_T("%0.3f"), (dbUplinkRange * 2) / V_C);

			// RTT(Down Link)
			// ダウンリンク距離 x 2 / 光速
			double dbDownlinkRange = mTrendData.GetUpDownLinkRange(time.GetTime(), 1);
			m_strForecastRTTDw.Format(_T("%0.3f"), (dbDownlinkRange * 2) / V_C);
		}
	}

	UpdateData(FALSE);

	// タイマ作成
	SetTimer(TID_FREQMON, INTERVAL_FREQMON, 0);
}

/*============================================================================*/
// タイマ処理
/*============================================================================*/
void CFrequencyCtrlMon54::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_FREQMON)
	{
		SetData();
	}
	CDialogBase::OnTimer(nIDEvent);
}

