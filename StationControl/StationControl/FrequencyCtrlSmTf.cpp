/*============================================================================*/
/*! FrequencyCtrlSmTf.cpp

-# 周波数制御画面-シミュレーション-地上局送信周波数変更タブ
*/
/*============================================================================*/
// FrequencyCtrlSmTf.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlSmTf.h"
#include "afxdialogex.h"
#include "FrequencyCtrlSm.h"


// CFrequencyCtrlSmTf ダイアログ

IMPLEMENT_DYNAMIC(CFrequencyCtrlSmTf, CDialogBase)

/*============================================================================*/
/*! CFrequencyCtrlSmTf

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CFrequencyCtrlSmTf::CFrequencyCtrlSmTf(CWnd* pParent /*=NULL*/)
: CDialogBase(CFrequencyCtrlSmTf::IDD, pParent)
, m_strTFreq(_T(""))
, m_strCFreq(_T(""))
, m_SpecDay(0)
, m_SpecTime(0)
{
	for (int row = 0; row < mItemRows; row++){
		for (int col = 0; col < mItemCols; col++){
			m_strSimValue[row][col] = _T("");
		}
	}
}

/*============================================================================*/
/*! CFrequencyCtrlSmTf

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CFrequencyCtrlSmTf::~CFrequencyCtrlSmTf()
{
}

void CFrequencyCtrlSmTf::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_TF1_EDIT, m_strTFreq);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_CF_TF1_EDIT, m_strCFreq);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_TF_SPT_DAY_DP, m_SpecDay);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_TF_SPT_TIME_DP, m_SpecTime);
	DDX_Control(pDX, IDC_FREQ_CTRL_SML_TF_SPT_DAY_DP, m_SpecDayDTCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_SML_DP_TF_CMB, m_TDopplerCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_SML_DP_CF_CMB, m_CDopplerCtrl);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_TF2_PST_EDIT, m_strSimValue[0][0]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_TF2_PSET_EDIT, m_strSimValue[0][1]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_TF2_PEDT_EDIT, m_strSimValue[0][2]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_TF2_PEDT_SPT_EDIT, m_strSimValue[0][3]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_CCF_PST_EDIT, m_strSimValue[1][0]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_CCF_PSET_EDIT, m_strSimValue[1][1]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_CCF_PEDT_EDIT, m_strSimValue[1][2]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_CCF_SPT_EDIT, m_strSimValue[1][3]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_1W_PST_EDIT, m_strSimValue[2][0]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_1W_PSET_EDIT, m_strSimValue[2][1]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_1W_PEDT_EDIT, m_strSimValue[2][2]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_1W_SPT_EDIT, m_strSimValue[2][3]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_2W_PST_EDIT, m_strSimValue[3][0]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_2W_PSET_EDIT, m_strSimValue[3][1]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_2W_PEDT_EDIT, m_strSimValue[3][2]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_2W_SPT_EDIT, m_strSimValue[3][3]);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrlSmTf, CDialogBase)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_TF_CALC_BTN, &CFrequencyCtrlSmTf::OnBnClickedFreqCtrlSmlTfCalcBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_TF_NOMINAL_BTN, &CFrequencyCtrlSmTf::OnBnClickedFreqCtrlSmlTfNominalBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_CF_NOMINAL_BTN, &CFrequencyCtrlSmTf::OnBnClickedFreqCtrlSmlCfNominalBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_TFCF_CALC_BTN, &CFrequencyCtrlSmTf::OnBnClickedFreqCtrlSmlTfcfCalcBtn)
END_MESSAGE_MAP()


// CFrequencyCtrlSmTf メッセージ ハンドラー

/*============================================================================*/
/*! CFrequencyCtrlSmTf

-# 計算ボタン押下処理(パス開始時刻、終了時刻)

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlSmTf::OnBnClickedFreqCtrlSmlTfcfCalcBtn()
{
	UpdateData(TRUE);
	CFrequencyCtrlSm::stSimData data;
	if (((CFrequencyCtrlSm*)mCallWnd)->GetSimData(data) == false)
		return;

	double	d_aos;
	double	d_los;
	errno_t err = 0;
	bool bNewFormat = false;
	if (err != 0 || mTrendData.load_pred_data((char*)(const char*)CStringA(data.predfilepath), &d_aos, &d_los, &bNewFormat) != 0){
		CString msg;
		msg.Format(_T("予報値ファイル(%s)が存在しません"), data.predfilepath);
		MessageBox(msg, _T("周波数制御"), MB_OK | MB_ICONWARNING);
		// エラー
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		msg.Format(_T("ANP(%s) open error"), data.predfilepath);
		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlSmTf"), _T("OnBnClickedFreqCtrlSmlTfcfCalcBtn"), msg, _T(""), nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return;
	}

	double val;
	time_t center = data.passStart + ((data.passStop - data.passStart) / 2);

	GetEqstatDbValue(data.satname, 2);

	// パス開始時刻の周波数を求める
	val = CalculatioFreq(data.passStart, 0, 0);
	m_strSimValue[0][0].Format(_T("%0.1f"), val);
	val = CalculatioFreq(data.passStart, 1, 0);
	m_strSimValue[1][0].Format(_T("%0.1f"), val);
	val = CalculatioFreq(data.passStart, 2, 0);
	m_strSimValue[2][0].Format(_T("%0.1f"), val);
	val = CalculatioFreq(data.passStart, 3, 0);
	m_strSimValue[3][0].Format(_T("%0.1f"), val);
	// パスセンター時刻の周波数を求める
	val = CalculatioFreq(center, 0, 1);
	m_strSimValue[0][1].Format(_T("%0.1f"), val);
	val = CalculatioFreq(center, 1, 1);
	m_strSimValue[1][1].Format(_T("%0.1f"), val);
	val = CalculatioFreq(center, 2, 1);
	m_strSimValue[2][1].Format(_T("%0.1f"), val);
	val = CalculatioFreq(center, 3, 1);
	m_strSimValue[3][1].Format(_T("%0.1f"), val);
	// パス終了時刻の周波数を求める
	val = CalculatioFreq(data.passStop, 0, 2);
	m_strSimValue[0][2].Format(_T("%0.1f"), val);
	val = CalculatioFreq(data.passStop, 1, 2);
	m_strSimValue[1][2].Format(_T("%0.1f"), val);
	val = CalculatioFreq(data.passStop, 2, 2);
	m_strSimValue[2][2].Format(_T("%0.1f"), val);
	val = CalculatioFreq(data.passStop, 3, 2);
	m_strSimValue[3][2].Format(_T("%0.1f"), val);

	UpdateData(FALSE);
}

/*============================================================================*/
/*! CFrequencyCtrlSmTf

-# 指定時刻の周波数計算

@param  time		時刻
@param  type		0=地上局送信周波数　1=コマンドサブキャリ周波数　2=地上局受信周波数(1WAY)　3=地上局受信周波数(2WAY)
@param  timetype	0=計画開始時刻　1=計画センター時刻　2=計画終了時刻
@retval なし
*/
/*============================================================================*/
double CFrequencyCtrlSmTf::CalculatioFreq(time_t time, int type, int timetype, bool bPassUse/*=true*/)
{
	CFrequencyCtrlSm::stSimData data;
	if (((CFrequencyCtrlSm*)mCallWnd)->GetSimData(data) == false)
		return 0.0;

	// DBアクセスクラス
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();
	struct satinf_t *satinf_adr;
	satinf_adr = (struct satinf_t *)((__int64*)satAdrList[data.satname]);

	int posT = m_TDopplerCtrl.GetCurSel();
	int posC = m_CDopplerCtrl.GetCurSel();

	double txf = 0.0;
	double rrup;
	double rxfast;
	double dfdown;
#if 1
	double C = V_C;
#else
	double C = V_C / 1000;
#endif

	switch (type){
	case	0:
#if 1
		if (posT == eDoppOffNomi)
		{
			txf = _wtof(m_strTFreq);
			return txf;
		}
		else if (posT == eDoppOffCenter)
		{
			// 計画開始時刻の地上局送信周波数
			// 時刻tのアップリンク変化率の取得
			time = data.passStart;
			rrup = mTrendData.GetUpDownLinkRate(time, 0);
			// 衛星側ノミナル受信周波数の計算
			//		rxfast = _wtof(m_strTFreq) + (-(rrup / C) * _wtof(m_strTFreq));
			rxfast = _wtof(m_strTFreq);
			// 時刻tの送信周波数の計算
			double txf_s = C / (C - rrup) * rxfast;

			// 計画終了時刻の地上局送信周波数
			// 時刻tのアップリンク変化率の取得
			time = data.passStop;
			rrup = mTrendData.GetUpDownLinkRate(time, 0);
			// 衛星側ノミナル受信周波数の計算
			//		rxfast = _wtof(m_strTFreq) + (-(rrup / C) * _wtof(m_strTFreq));
			rxfast = _wtof(m_strTFreq);
			// 時刻tの送信周波数の計算
			double txf_e = C / (C - rrup) * rxfast;

			txf = (txf_s + txf_e) / 2.0;
			return txf;
		}
		else if (posT == eDoppOffAos)
		{
			time = data.passStart;
		}
		// 地上局送信周波数
		// 時刻tのアップリンク変化率の取得
		rrup = mTrendData.GetUpDownLinkRate(time, 0);
		// 衛星側ノミナル受信周波数の計算
		//		rxfast = _wtof(m_strTFreq) + (-(rrup / C) * _wtof(m_strTFreq));
		rxfast = _wtof(m_strTFreq);
		// 時刻tの送信周波数の計算
		txf = C / (C - rrup) * rxfast;
#else
		if (bPassUse == true){
			if (posT == eDoppOffNomi){
				txf = _wtof(m_strTFreq);
				return txf;
			}
			else if (posT == eDoppOffCenter){
				time_t center = data.passStart + ((data.passStop - data.passStart) / 2);
				time = center;
			}
			else if (posT == eDoppOffAos){
				time = data.passStart;
			}
		}
		// 地上局送信周波数
		// 時刻tのアップリンク変化率の取得
		rrup = mTrendData.GetUpDownLinkRate(time, 0);
		// 衛星側ノミナル受信周波数の計算
		//		rxfast = _wtof(m_strTFreq) + (-(rrup / C) * _wtof(m_strTFreq));
		rxfast = _wtof(m_strTFreq);
		// 時刻tの送信周波数の計算
		txf = C / (C - rrup) * rxfast;
#endif
		break;
	case	1:
#if 1
		if (posC == eDoppOffNomi){
			txf = _wtof(m_strCFreq);
			return txf;
		}
		else if (posC == eDoppOffCenter){
			// 計画開始時刻のコマンドサブキャリ周波数
			// 時刻tのアップリンク変化率の取得
			rrup = mTrendData.GetUpDownLinkRate(data.passStart, 0);
			// 衛星側ノミナル受信周波数の計算
			//		rxfast = _wtof(m_strCFreq) + (-(rrup / C) * _wtof(m_strCFreq));
			rxfast = _wtof(m_strCFreq);
			// 時刻tの送信周波数の計算
			double txf_s = C / (C - rrup) * rxfast;

			// 計画終了時刻のコマンドサブキャリ周波数
			// 時刻tのアップリンク変化率の取得
			rrup = mTrendData.GetUpDownLinkRate(data.passStop, 0);
			// 衛星側ノミナル受信周波数の計算
			//		rxfast = _wtof(m_strCFreq) + (-(rrup / C) * _wtof(m_strCFreq));
			rxfast = _wtof(m_strCFreq);
			// 時刻tの送信周波数の計算
			double txf_e = C / (C - rrup) * rxfast;

			txf = (txf_s + txf_e) / 2.0;
			return txf;
		}
		else if (posC == eDoppOffAos){
			time = data.passStart;
		}
#else
		if (bPassUse == true){
			if (posC == eDoppOffNomi){
				txf = _wtof(m_strCFreq);
				return txf;
			}
			else if (posC == eDoppOffCenter){
				time_t center = data.passStart + ((data.passStop - data.passStart) / 2);
				time = center;
			}
			else if (posC == eDoppOffAos){
				time = data.passStart;
			}
		}
#endif
		// コマンドサブキャリ周波数
		// 時刻tのアップリンク変化率の取得
		rrup = mTrendData.GetUpDownLinkRate(time, 0);
		// 衛星側ノミナル受信周波数の計算
//		rxfast = _wtof(m_strCFreq) + (-(rrup / C) * _wtof(m_strCFreq));
		rxfast = _wtof(m_strCFreq);
		// 時刻tの送信周波数の計算
		txf = C / (C - rrup) * rxfast;
		break;
	case	2:
		// 地上局受信周波数(1WAY)
		// 時刻tのダウンリンク変化率の取得
		rrup = mTrendData.GetUpDownLinkRate(time, 1);
		// 衛星側ノミナル送信周波数の計算
//		rxfast = _wtof(m_strTFreq) + (-(rrup / C) * _wtof(m_strTFreq));
		rxfast = _wtof(m_strRFreq);
		// 時刻tの1WAYドップラ周波数を求める
		dfdown = (-(rrup / C)) * rxfast;
		// 時刻tの受信周波数の計算
		txf = rxfast + dfdown;
		break;
	case	3:
		// 地上局受信周波数(2WAY)
		if (posT == eDoppOn)
		{
			// 動的補償の場合1WAY用の計算を行う。

			// 地上局受信周波数(1WAY)
			// 時刻tのダウンリンク変化率の取得
			rrup = mTrendData.GetUpDownLinkRate(time, 1);
			// 衛星側ノミナル送信周波数の計算
			//		rxfast = _wtof(m_strTFreq) + (-(rrup / C) * _wtof(m_strTFreq));
			rxfast = _wtof(m_strRFreq);
			// 時刻tの1WAYドップラ周波数を求める
			dfdown = (-(rrup / C)) * rxfast;
			// 時刻tの受信周波数の計算
			txf = rxfast + dfdown;
		}
		else 
		{
			// 時刻tの2WAY距離変化率の取得
			rrup = mTrendData.GetUpDownLinkRate(time, 2);
			// 時刻tの2WAYドップラ周波数を求める
			if (satinf_adr != NULL)
				dfdown = (-(rrup / C)) * _wtof(m_strSimValue[0][timetype]) * satinf_adr->d_xcoh;
			else
				dfdown = (-(rrup / C)) * _wtof(m_strSimValue[0][timetype]);
			// 時刻tの受信周波数の計算
			if (satinf_adr != NULL)
				txf = _wtof(m_strSimValue[0][timetype]) * satinf_adr->d_xcoh + dfdown;
			else
				txf = _wtof(m_strSimValue[0][timetype]) + dfdown;
		}
		break;
	}

	return txf;
}

/*============================================================================*/
/*! CFrequencyCtrlSmTf

-# 地上局送信周波数「ノミナルに戻す」ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlSmTf::OnBnClickedFreqCtrlSmlTfNominalBtn()
{
	UpdateData(TRUE);
	CFrequencyCtrlSm::stSimData data;
	if (((CFrequencyCtrlSm*)mCallWnd)->GetSimData(data) == false)
		return;

	GetEqstatDbValue(data.satname, 0);
	GetEqstatDbValue(data.satname, 2);
	UpdateData(FALSE);
}

/*============================================================================*/
/*! CFrequencyCtrlSmTf

-# コマンドサブキャリア周波数「ノミナルに戻す」ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlSmTf::OnBnClickedFreqCtrlSmlCfNominalBtn()
{
	UpdateData(TRUE);
	CFrequencyCtrlSm::stSimData data;
	if (((CFrequencyCtrlSm*)mCallWnd)->GetSimData(data) == false)
		return;

	GetEqstatDbValue(data.satname, 1);
	UpdateData(FALSE);
}

/*============================================================================*/
/*! CFrequencyCtrlSmTf

-# 計算ボタン押下処理（任意時刻）

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlSmTf::OnBnClickedFreqCtrlSmlTfCalcBtn()
{
	UpdateData(TRUE);
	CTime t = CTime(m_SpecDay.GetYear(), m_SpecDay.GetMonth(), m_SpecDay.GetDay(), m_SpecTime.GetHour(), m_SpecTime.GetMinute(), m_SpecTime.GetSecond());
	time_t time = t.GetTime();
	CFrequencyCtrlSm::stSimData data;
	if (((CFrequencyCtrlSm*)mCallWnd)->GetSimData(data) == false)
		return;

	double	d_aos;
	double	d_los;
	errno_t err = 0;
	bool bNewFormat = false;
	if (err != 0 || mTrendData.load_pred_data((char*)(const char*)CStringA(data.predfilepath), &d_aos, &d_los, &bNewFormat) != 0){
		CString msg;
		msg.Format(_T("予報値ファイル(%s)が存在しません"), data.predfilepath);
		MessageBox(msg, _T("周波数制御"), MB_OK | MB_ICONWARNING);
		// エラー
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		msg.Format(_T("ANP(%s) open error"), data.predfilepath);
		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlSmTf"), _T("OnBnClickedFreqCtrlSmlTfcfCalcBtn"), msg, _T(""), nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return;
	}

	double val;

	// 任意時刻の周波数を求める
	val = CalculatioFreq(time, 0, 3, false);
	m_strSimValue[0][3].Format(_T("%0.1f"), val);
	val = CalculatioFreq(time, 1, 3, false);
	m_strSimValue[1][3].Format(_T("%0.1f"), val);
	val = CalculatioFreq(time, 2, 3, false);
	m_strSimValue[2][3].Format(_T("%0.1f"), val);
	val = CalculatioFreq(time, 3, 3, false);
	m_strSimValue[3][3].Format(_T("%0.1f"), val);
	UpdateData(FALSE);
}


/*============================================================================*/
/*! CFrequencyCtrlSmTf

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CFrequencyCtrlSmTf::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// 送信ドップラ
	for (int i = 0; i < sizeof(mSimTxDopp) / sizeof(mSimTxDopp[0]); i++){
		m_TDopplerCtrl.AddString(mSimTxDopp[i]);
	}
	m_TDopplerCtrl.SetCurSel(0);

	// コマンドサブキャリア
	for (int i = 0; i < sizeof(mSimCmdCarr) / sizeof(mSimCmdCarr[0]); i++){
		m_CDopplerCtrl.AddString(mSimCmdCarr[i]);
	}
	m_CDopplerCtrl.SetCurSel(0);

	// 日時フォーマット変更
	m_SpecDayDTCtrl.SetFormat(_T("yyyy-MM-dd"));

	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	CTime time = CTime(timeSys);

	// 指定時刻
	m_SpecDay = time;
	m_SpecTime = time;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! CFrequencyCtrlSmTf

-# ダイアログ更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CFrequencyCtrlSmTf::UpdateDlg()
{
	return TRUE;
}

/*============================================================================*/
/*! CFrequencyCtrlMon

-# 衛星固有DBから地上局送信周波数データを取得する

@param  pbuf	衛星固有情報
@retval なし
*/
/*============================================================================*/
bool CFrequencyCtrlSmTf::_getTxFqVal(char* pbuf)
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
	if (pdata->d_fq != 0.0)
	{
		m_strTFreq.Format(_T("%0.1f"), pdata->d_fq);
	}

	return true;
}

/*============================================================================*/
/*! CFrequencyCtrlMon

-# 衛星固有DBから地上局送信周波数データを取得する

@param  pbuf	衛星固有情報
@retval なし
*/
/*============================================================================*/
bool CFrequencyCtrlSmTf::_getRxFqVal(char* pbuf)
{
	// ノミナル送信周波数設定値定義テーブル
	struct rxfqval_t {
		long l_id;		// データｉｄ: 定数=TXFQVAL_ID値
		long l_len;		// データ長
		long l_no;		// 設定値番号
		long l_band;	// バンド指定
		double d_fq;	// 送信周波数値[Hz]
	};
	struct rxfqval_t *pdata = (struct rxfqval_t *)pbuf;
	if (pdata->d_fq != 0.0)
	{
		m_strRFreq.Format(_T("%0.1f"), pdata->d_fq);
	}

	return true;
}

/*============================================================================*/
/*! CFrequencyCtrlMon

-# 衛星固有DBからコマンドサブキャリア周波数データを取得する

@param  pbuf	衛星固有情報
@retval なし
*/
/*============================================================================*/
bool CFrequencyCtrlSmTf::_getCmdFqVal(char* pbuf)
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
	m_strCFreq.Format(_T("%0.1f"), pdata->d_fq);

	return true;
}
/*============================================================================*/
/*! CFrequencyCtrlMon

-# 衛星固有DBからコマンドサブキャリア周波数データを取得する

@param  pbuf	衛星固有情報
@retval なし
*/
/*============================================================================*/
bool CFrequencyCtrlSmTf::_getCmdFqValEx(CString pbuf)
{
	double val = _wtof(pbuf);
	m_strCFreq.Format(_T("%0.1f"), val);
//	m_strCFreq = pbuf;

	return true;
}
/*============================================================================*/
/*! CFrequencyCtrlMon

-# 衛星固有DBからSIMで使用するデータを取得する

@param  strst	衛星名
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlSmTf::GetEqstatDbValue(CString& strst, int type)
{
	if (strst.IsEmpty() == TRUE)
	{
		MessageBox(_T("衛星が選択されていません。"), _T("周波数制御"), MB_OK | MB_ICONWARNING);
	}

	typedef struct{
		UINT	dbid;
		bool(CFrequencyCtrlSmTf::*func)(char*);
	} stFreqFuncTable;
	const stFreqFuncTable _FreqCtrlMon[] = {
#if 1
		{ TXFQVAL_ID, &CFrequencyCtrlSmTf::_getTxFqVal },
		{ RXFQVAL_ID, &CFrequencyCtrlSmTf::_getCmdFqVal },
		{ RXFQVAL_ID, &CFrequencyCtrlSmTf::_getRxFqVal },
#else
		{ TXFQVAL_ID, &CFrequencyCtrlSmTf::_getTxFqVal },
		{ RXFQVAL_ID, &CFrequencyCtrlSmTf::_getCmdFqVal },
#endif
	};

	// DBアクセスクラス
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();
	struct satinf_t *satinf_adr;
	struct satinf_t *original_adr;
	satinf_adr = (struct satinf_t *)((__int64*)satAdrList[strst]);
	original_adr = satinf_adr;

	char* sattbl_adr = (char *)satinf_adr;
	while (1){
		if (type == 1){
			// コマンドサブキャリアは「DSN-MODEM MODULATION初期設定」より取得する
			break;
		}
		satinf_adr = (struct satinf_t *)sattbl_adr;
		if (satinf_adr->l_id == ENDOFSAT_ID){
			// 個々の衛星情報の定義の終了を現すＩＤ
			break;
		}

		// データを取得するため、設定テーブルを全件検索する
		if (satinf_adr->l_id == _FreqCtrlMon[type].dbid){
			(this->*_FreqCtrlMon[type].func)(sattbl_adr);
//			break;
		}
		sattbl_adr += satinf_adr->l_len;
	}

	if (type != 1)
		return;

	//=========================================================================
	// コマンドサブキャリア周波数「ノミナルに戻す」処理を行う
	// 衛星DBの「INIT.DSNTCR-MOD」-「DSNTCR.TX_SC_FREQ」の設定値を使用する。
	// IDが存在しない場合は何もしない
	const CString mMacName = _T("INIT.DSNTCR-MOD");
	const CString mExecName = _T("DSNTCR.TX_SC_FREQ");
	const UINT MAC_MAX = 256;
	char *psz_tbl[MAC_MAX];				// マクロ名を格納するテーブルの先頭アドレス
	char *psz_vec[MAC_MAX];				// マクロ情報アドレスを格納するテーブルの
	int caseNum = 0;
	int execNum = 0;

	satinf_adr = original_adr;				/*衛星本体情報先頭アドレスセット*/
	// MAC名称「INIT.DSNTCR-MOD」を探し、CASEパラメータ群を取得する
	caseNum = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(mMacName).GetBuffer(), psz_tbl, psz_vec, MAC_MAX);
	execNum = 0;
	bool bFind = false;
	if (caseNum > 0){
		// 取得されたCASEパラメタ群よりEXECパラメタ群を取得する
		for (int i = 0; i < caseNum; i++){
			char *psz_exec[MAC_MAX];				// マクロ情報アドレスを格納するテーブルの
			execNum = dba.ud_macstsadr_to_exectbl((char*)psz_vec[i], psz_tbl, psz_exec, MAC_MAX);
		}
		for(int i = 0; i < execNum; i++){
			// 指定の文字列「DSNTCR.TX_SC_FREQ」を探す
			CString strExec, str, strName, strValue;
			strExec = CString(psz_tbl[i]);
			int pos = 0;
			while (1){
				if (AfxExtractSubString(str, strExec, pos, ',') == FALSE)
					break;
				str.TrimLeft();
				str.TrimRight();
				switch (pos){
				case	0:	// EXEC名称
					strName = str;
					break;
				case	1:	// 値
					strValue = str;
					break;
				}
				pos++;
			}
			if (strName.CompareNoCase(mExecName) == 0){
				// 対象のEXEC名称が見つかった
				_getCmdFqValEx(strValue);
				bFind = true;
				break;
			}
		}
	}
	if (bFind == false){
		MessageBox(_T("DBデータが存在しません"), _T("周波数制御"), MB_OK | MB_ICONWARNING);
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CString msg;
		msg.Format(_T("DSNTCR.TX_SC_FREQ not found"));
		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlSmTf"), _T("GetEqstatDbValue"), msg, _T(""), nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
	}
}
