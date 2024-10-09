/*============================================================================*/
/*! FrequencyCtrlSmFf.cpp

-# 周波数制御画面-シミュレーション-地上局受信周波数変更タブ（未使用）
*/
/*============================================================================*/
// FrequencyCtrlSmFf.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlSmRf.h"
#include "afxdialogex.h"


// CFrequencyCtrlSmRf ダイアログ

IMPLEMENT_DYNAMIC(CFrequencyCtrlSmRf, CDialogBase)

/*============================================================================*/
/*! CFrequencyCtrlSmRf

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CFrequencyCtrlSmRf::CFrequencyCtrlSmRf(CWnd* pParent /*=NULL*/)
: CDialogBase(CFrequencyCtrlSmRf::IDD, pParent)
	, m_strRFreq(_T(""))
	, m_SpecDay(0)
	, m_SpecTime(0)
{

}

/*============================================================================*/
/*! CFrequencyCtrlSmRf

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CFrequencyCtrlSmRf::~CFrequencyCtrlSmRf()
{
}

void CFrequencyCtrlSmRf::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_RF_TF1_EDIT, m_strRFreq);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_RF_SPT_DAY_DP, m_SpecDay);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_RF_SPT_TIME_DP, m_SpecTime);
	DDX_Control(pDX, IDC_FREQ_CTRL_SML_RF_SPT_DAY_DP, m_SpecDayDTCtrl);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrlSmRf, CDialogBase)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_RF_DPCALC_BTN, &CFrequencyCtrlSmRf::OnBnClickedFreqCtrlSmlRfDpcalcBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_RF_DPVIEW_BTN, &CFrequencyCtrlSmRf::OnBnClickedFreqCtrlSmlRfDpviewBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_RF_CALC_BTN, &CFrequencyCtrlSmRf::OnBnClickedFreqCtrlSmlRfCalcBtn)
END_MESSAGE_MAP()


// CFrequencyCtrlSmRf メッセージ ハンドラー

/*============================================================================*/
/*! CFrequencyCtrlSmRf

-# ドップラ補償値計算ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlSmRf::OnBnClickedFreqCtrlSmlRfDpcalcBtn()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}

/*============================================================================*/
/*! CFrequencyCtrlSmRf

-# ドップラ補償値表示ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlSmRf::OnBnClickedFreqCtrlSmlRfDpviewBtn()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}

/*============================================================================*/
/*! CFrequencyCtrlSmRf

-# 計算ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlSmRf::OnBnClickedFreqCtrlSmlRfCalcBtn()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


/*============================================================================*/
/*! CFrequencyCtrlSmRf

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CFrequencyCtrlSmRf::OnInitDialog()
{
	CDialogBase::OnInitDialog();

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
/*! CFrequencyCtrlSmRf

-# ダイアログ更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CFrequencyCtrlSmRf::UpdateDlg()
{
	return TRUE;
}

