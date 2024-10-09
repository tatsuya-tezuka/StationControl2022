/*============================================================================*/
/*! FrequencyCtrlSm3W.cpp

-# 周波数制御画面-シミュレーション-3WAYタブ
*/
/*============================================================================*/
// FrequencyCtrlSm3W.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlSm3W.h"
#include "afxdialogex.h"
#include "FrequencyCtrlSm.h"


// CFrequencyCtrlSm3W ダイアログ

IMPLEMENT_DYNAMIC(CFrequencyCtrlSm3W, CDialogBase)

/*============================================================================*/
/*! CFrequencyCtrlSm3W

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CFrequencyCtrlSm3W::CFrequencyCtrlSm3W(CWnd* pParent /*=NULL*/)
: CDialogBase(CFrequencyCtrlSm3W::IDD, pParent)
	, m_str3WAYFreq(_T(""))
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
/*! CFrequencyCtrlSm3W

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CFrequencyCtrlSm3W::~CFrequencyCtrlSm3W()
{
}

void CFrequencyCtrlSm3W::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_TF1_EDIT, m_str3WAYFreq);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_3W_SPT_DAY_DP, m_SpecDay);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_3W_SPT_TIME_DP, m_SpecTime);
	DDX_Control(pDX, IDC_FREQ_CTRL_SML_3W_SPT_DAY_DP, m_SpecDayDTCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_SML_3W_GETFREQ_CMB, m_cbSimSendGet);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_TF2_PST_EDIT, m_strSimValue[0][0]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_TF2_PSET_EDIT, m_strSimValue[0][1]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_TF2_PEDT_EDIT, m_strSimValue[0][2]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_TF2_PEDT_SPT_EDIT, m_strSimValue[0][3]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_1W_PST_EDIT, m_strSimValue[1][0]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_1W_PSET_EDIT, m_strSimValue[1][1]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_1W_PEDT_EDIT, m_strSimValue[1][2]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_1W_SPT_EDIT, m_strSimValue[1][3]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_2W_PST_EDIT, m_strSimValue[2][0]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_2W_PSET_EDIT, m_strSimValue[2][1]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_2W_PEDT_EDIT, m_strSimValue[2][2]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_2W_SPT_EDIT, m_strSimValue[2][3]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_3W_PST_EDIT, m_strSimValue[3][0]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_3W_PSET_EDIT, m_strSimValue[3][1]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_3W_PEDT_EDIT, m_strSimValue[3][2]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_3W_SPT_EDIT, m_strSimValue[3][3]);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrlSm3W, CDialogBase)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_3W_CALC_BTN, &CFrequencyCtrlSm3W::OnBnClickedFreqCtrlSml3wCalcBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_3W_CALC_BTN2, &CFrequencyCtrlSm3W::OnBnClickedFreqCtrlSml3wCalcBtn2)
	ON_BN_CLICKED(IDC_BUTTON_GET, &CFrequencyCtrlSm3W::OnBnClickedButtonGet)
END_MESSAGE_MAP()


// CFrequencyCtrlSm3W メッセージ ハンドラー


/*============================================================================*/
/*! CFrequencyCtrlSm3W

-# 計算ボタン押下処理(取得先等)

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlSm3W::OnBnClickedFreqCtrlSml3wCalcBtn2()
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
		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlSm3W"), _T("OnBnClickedFreqCtrlSml3wCalcBtn2"), msg, _T(""), nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return;
	}

	double val;
	time_t center = data.passStart + ((data.passStop - data.passStart) / 2);

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

@param  time	時刻
@param  type	0=相手局送信周波数　1=地上局送信周波数(1WAY)　2=地上局受信周波数(2WAY)　3=地上局受信周波数(3WAY)
@param  timetype	0=計画開始時刻　1=計画センター時刻　2=計画終了時刻
@retval なし
*/
/*============================================================================*/
double CFrequencyCtrlSm3W::CalculatioFreq(time_t time, int type, int timetype, bool bPassUse/*=true*/)
{
	CFrequencyCtrlSm::stSimData data;
	if (((CFrequencyCtrlSm*)mCallWnd)->GetSimData(data) == false)
		return 0.0;

	// DBアクセスクラス
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();
	struct satinf_t *satinf_adr;
	satinf_adr = (struct satinf_t *)((__int64*)satAdrList[data.satname]);

	double txf = 0.0;
	double rrup;
	double rxfast;
	double dfdown;
#if 1
	double C = V_C;
#else
	double C = V_C/1000;
#endif

	switch (type){
	case	0:
		// 相手局送信周波数
		// 時刻tのアップリンク変化率の取得
		rrup = mTrendData.GetUpDownLinkRate(time, 0);
		// 衛星側ノミナル受信周波数の計算
//		rxfast = _wtof(m_str3WAYFreq) + (-(rrup / C) * _wtof(m_str3WAYFreq));
		rxfast = _wtof(m_str3WAYFreq);
		// 時刻tの送信周波数の計算
		txf = C / (C - rrup) * rxfast;
		break;
	case	1:
		// 地上局受信周波数(1WAY)
		// 時刻tのダウンリンク変化率の取得
		rrup = mTrendData.GetUpDownLinkRate(time, 1);
		// 衛星側ノミナル受信周波数の計算
//		rxfast = _wtof(m_str3WAYFreq) + (-(rrup / C) * _wtof(m_str3WAYFreq));
		rxfast = _wtof(m_str3WAYFreq);
		// 時刻tの1WAYドップラ周波数を求める
		dfdown = (-(rrup / C)) * rxfast;
		// 時刻tの受信周波数の計算
		txf = rxfast + dfdown;
		break;
	case	2:
		// 地上局受信周波数(2WAY)
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
		break;
	case	3:
		// 地上局受信周波数(3WAY)
		// 時刻tの2WAY距離変化率の取得
		rrup = mTrendData.GetUpDownLinkRate(time, 3);
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
		break;
	}

	return txf;
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# 「取得」ボタン押下時処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlSm3W::OnBnClickedButtonGet()
{
	UpdateData(TRUE);
	// 相手局送信周波数を取得する
	int pos = m_cbSimSendGet.GetCurSel();
	UINT target = (UINT)m_cbSimSendGet.GetItemData(pos);
	CSatelliteData& sd = theApp.GetSatelliteData();
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(target, _T("X-UC.FREQUENCY"));
	m_str3WAYFreq = _T("0.0");
	if (obs_adr != NULL){
		m_str3WAYFreq.Format(_T("%0.1f"), obs_adr->d_data);
//#ifdef _DEBUG
//		m_str3WAYFreq.Format(_T("100.0"));
//#endif
	}
	UpdateData(FALSE);
}

/*============================================================================*/
/*! CFrequencyCtrlSm3W

-# 計算ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlSm3W::OnBnClickedFreqCtrlSml3wCalcBtn()
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
		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlSm3W"), _T("OnBnClickedFreqCtrlSml3wCalcBtn"), msg, _T(""), nLogEx::error);
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
/*! CFrequencyCtrlSm3W

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CFrequencyCtrlSm3W::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// 送信周波数取得先
	for (int i = 0; i < sizeof(mSimSenderFreq[theApp.GetSelectStation()]) / sizeof(mSimSenderFreq[theApp.GetSelectStation()][0]); i++){
		int pos = m_cbSimSendGet.AddString(mSimSenderFreq[theApp.GetSelectStation()][i].sendname);
		m_cbSimSendGet.SetItemData(pos, mSimSenderFreq[theApp.GetSelectStation()][i].sendid);
	}
	m_cbSimSendGet.SetCurSel(0);

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
/*! CFrequencyCtrlSm3W

-# ダイアログ更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CFrequencyCtrlSm3W::UpdateDlg()
{
	return TRUE;
}
