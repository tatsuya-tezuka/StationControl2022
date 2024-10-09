/*============================================================================*/
/*! FrequencyCtrlMon.cpp

-# 周波数制御画面-周波数モニタタブ
*/
/*============================================================================*/
// FrequencyCtrlMon.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlMon.h"
#include "afxdialogex.h"

#include "FrequencyCtrl.h"
#include "SatelliteData.h"


// CFrequencyCtrlMon ダイアログ

IMPLEMENT_DYNAMIC(CFrequencyCtrlMon, CDialogBase)

/*============================================================================*/
/*! CFrequencyCtrlMon

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CFrequencyCtrlMon::CFrequencyCtrlMon(CWnd* pParent /*=NULL*/)
: CDialogBase(CFrequencyCtrlMon::IDD, pParent)
	, m_strUC(_T(""))
	, m_strDC(_T(""))
	, m_strCmdCareer(_T(""))
	, m_strTFreqOffset(_T(""))
	, m_strTimeOffset(_T(""))
	, m_strForecastRTTUp(_T(""))
	, m_strForecastRTTDw(_T(""))
	, m_strTDoppler(_T(""))
	, m_strRDoppler(_T(""))
	, m_strDopplerCmdCareer(_T(""))
	, m_strSpacecraftCOHMode(_T(""))
	, m_tPredDay(0)
	, m_tPredTime(0)
	, m_strRFreqOffset(_T(""))
{
	mTargetSatellite.Empty();
	mTargetPredAnp.Empty();
}

CFrequencyCtrlMon::CFrequencyCtrlMon(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialogBase(IDD, pParent)
{

}

/*============================================================================*/
/*! CFrequencyCtrlMon

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CFrequencyCtrlMon::~CFrequencyCtrlMon()
{
}

void CFrequencyCtrlMon::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_SET_UC_EDIT, m_strUC);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_SET_DC_EDIT, m_strDC);
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


BEGIN_MESSAGE_MAP(CFrequencyCtrlMon, CDialogBase)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFrequencyCtrlMon メッセージ ハンドラー


/*============================================================================*/
/*! CFrequencyCtrlMon

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CFrequencyCtrlMon::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_PredTimeDayCtrl.SetFormat(_T("yyyy-MM-dd"));

	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	CTime time = CTime(timeSys);

	m_tPredDay = time;
	m_tPredTime = time;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! CFrequencyCtrlMon

-# ダイアログ更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CFrequencyCtrlMon::UpdateDlg()
{
	// 画面更新
	SetData();

	return TRUE;
}

/*============================================================================*/
/*! CFrequencyCtrlMon

-# 画面更新

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlMon::SetData()
{
	CString mPreFreqTx;	// 予測送信周波数(TX)の計算用
	CString mPreFreqRx;	// 予測送信周波数(RX)の計算用
	CString mPreFreqSc;	// 予測送信周波数(SC)の計算用
	CString mPredFile;	// 予報値ファイル名

	CFrequencyCtrlMon::stSetMonData mMonTable[] = {
		{ _T("X-UC.FREQUENCY"), m_strUC, _T("%0.1f"), eDOUBLE },					// 設定周波数（U/C）
		{ _T("S70-DC.MON_FREQ"), m_strDC, _T("%0.1f"), eDOUBLE },					// 設定周波数（D/C）
		{ _T("DSNTCR.TX_SC_FREQ"), m_strCmdCareer, _T("%0.1f"), eDOUBLE },			// 設定周波数（コマンドサブキャリア）
		{ _T("CTRL.UP_FREQ_MODE"), m_strTDoppler, _T("%s"), eSTRING },				// ドップラ補償（送信ドップラ）
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
	mPredFile = _T("C:/share/udsc64/ctrl/prd/geotail/1708270400.prd");
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
		msg.Format(_T("ANP(%s) open error"), anpfile);
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
/*! CFrequencyCtrlMon

-# タイマ処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlMon::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_FREQMON)
	{
		SetData();
	}
	CDialogBase::OnTimer(nIDEvent);
}

/*============================================================================*/
/*! CFrequencyCtrlMon

-# 衛星固有DBから周波数モニタで使用するデータを取得する

@param  pbuf	衛星固有情報
@retval なし
*/
/*============================================================================*/
bool CFrequencyCtrlMon::_getTxFqVal(char* pbuf)
{
	// ノミナル送信周波数設定値定義テーブル
	struct txfqval_t {
		long l_id;		// データｉｄ: 定数=TXFQVAL_ID値
		long l_len;		// データ長
		long l_no;		// 設定値番号
		long l_band;	// バンド指定
		double d_fq;	// 送信周波数値[Hz]
	};
	struct txfqval_t *pdata = (struct txfqval_t *)pbuf;
	mTxFreqVal = pdata->d_fq;

	return true;
}
bool CFrequencyCtrlMon::_getRxFqVal(char* pbuf)
{
	// ノミナル受信周波数設定値定義テーブル
	struct rxfqval_t {
		long l_id;		// データｉｄ: 定数=TXFQVAL_ID値
		long l_len;		// データ長
		long l_no;		// 設定値番号
		long l_band;	// バンド指定
		double d_fq;	// 受信周波数値[Hz]
	};
	struct rxfqval_t *pdata = (struct rxfqval_t *)pbuf;
	if (pdata->l_no == 1)	mRxFreqVal[0] = pdata->d_fq;
	if (pdata->l_no == 2)	mRxFreqVal[1] = pdata->d_fq;
	// それ以外はなにもしない

	return true;
}
bool CFrequencyCtrlMon::_getXTXTypeVal(char* pbuf)
{
	// ノミナル受信周波数設定値定義テーブル
	struct xtx_t {	/* Ｘ帯送信設備使用有無情報定義テーブル: ソース上"xtx"に相当 */
		long l_id;	 /* データｉｄ: 定数=XTX_ID値 */
		long l_len;	 /* データ長  : sizeof( struct xtx_t ) */
		long l_x_tx; /* Ｘ帯送信設備使用有無:XTX_Y_ID .or. XTX_N_ID */
		long l_rfu;  /* パディング */
	};
	struct xtx_t *pdata = (struct xtx_t *)pbuf;
	mTXTType = (pdata->l_x_tx == XTX_N_ID)?0:1;

	return true;
}
/*============================================================================*/
/*! CFrequencyCtrlMon

-# 衛星固有DBから周波数モニタで使用するデータを取得する

@param  strst	衛星名
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlMon::GetEqstatDbValue(CString& strst)
{
	typedef struct{
		UINT	dbid;
		bool(CFrequencyCtrlMon::*func)(char*);
	} stFreqFuncTable;
	const stFreqFuncTable _FreqCtrlMon[] = {
		{ TXFQVAL_ID, &CFrequencyCtrlMon::_getTxFqVal },
		{ RXFQVAL_ID, &CFrequencyCtrlMon::_getRxFqVal },
		{ XTX_ID, &CFrequencyCtrlMon::_getXTXTypeVal },
	};

	// 初期化
	mTxFreqVal = 0;
	mRxFreqVal[0] = 0;
	mRxFreqVal[1] = 0;
	mTXTType = 0;

	// DBアクセスクラス
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();
	struct satinf_t *satinf_adr;
	satinf_adr = (struct satinf_t *)((__int64*)satAdrList[strst]);
	if (satinf_adr == NULL){
		return;
	}
	char* sattbl_adr = (char *)satinf_adr;
	while (1){
		satinf_adr = (struct satinf_t *)sattbl_adr;
		if (satinf_adr->l_id == ENDOFSAT_ID){
			// 個々の衛星情報の定義の終了を現すＩＤ
			break;
		}

		// データを取得するため、設定テーブルを全件検索する
		for (int index = 0; index < sizeof(_FreqCtrlMon) / sizeof(_FreqCtrlMon[0]); index++){
			if (satinf_adr->l_id == _FreqCtrlMon[index].dbid){
				(this->*_FreqCtrlMon[index].func)(sattbl_adr);
			}
		}
		sattbl_adr += satinf_adr->l_len;
	}
}

/*============================================================================*/
/*! CFrequencyCtrlMon

-# 予報値ファイル名の変換

@param  strst	監視予報値名
@retval なし
*/
/*============================================================================*/
bool CFrequencyCtrlMon::_convertPredFileName(CString& monPredFileName, CString& satName, CString& anpName)
{
	// 予報値ファイルフルパス名（予報値ファイル名）
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlMon"), _T("_convertPredFileName"), _T("Pred file name"), monPredFileName, nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	CString strFilePath = monPredFileName;
	strFilePath.Trim(_T(" "));
	if (strFilePath.IsEmpty()){
		// ファイル名が存在しない
		return false;
	} else{
		// 拡張子がprdの場合、anpに置き換える
		CString fname = strFilePath;
		if (fname.Right(3).MakeLower() == PRED_FILE_TYPE){
			strFilePath = fname.Left(fname.GetLength() - 3);
			strFilePath += ANP_FILE_TYPE;
		}

		// ファイル名と衛星名を切り出す
		int i = 0;
		CString str;
		anpName.Empty();
		satName.Empty();
		while (AfxExtractSubString(str, strFilePath, i++, '/')){
			satName = anpName;
			anpName = str;
		}

		// 局フォルダをつける
		strFilePath = theApp.GetShareFilePath(eFileType_Pred_Folder, theApp.GetSelectStation());
		//monPredFileName.Format(_T("%s%s\\%s"), (LPCTSTR)strFilePath, (LPCTSTR)satName, (LPCTSTR)anpName);
	}
	return true;
}
