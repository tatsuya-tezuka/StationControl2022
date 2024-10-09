/*============================================================================*/
/*! FrequencyCtrlSmCf.cpp

-# 周波数制御画面-シミュレーション-コマンドサブキャリア周波数変更タブ（未使用）
*/
/*============================================================================*/
// FrequencyCtrlSmCf.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlSmCf.h"
#include "afxdialogex.h"


// CFrequencyCtrlSmCf ダイアログ

IMPLEMENT_DYNAMIC(CFrequencyCtrlSmCf, CDialogBase)

/*============================================================================*/
/*! CFrequencyCtrlSmCf

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CFrequencyCtrlSmCf::CFrequencyCtrlSmCf(CWnd* pParent /*=NULL*/)
: CDialogBase(CFrequencyCtrlSmCf::IDD, pParent)
	, m_strCmdFreq(_T(""))
	, m_SpecDay(0)
	, m_SpecTime(0)
{

}

/*============================================================================*/
/*! CFrequencyCtrlSmCf

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CFrequencyCtrlSmCf::~CFrequencyCtrlSmCf()
{
}

void CFrequencyCtrlSmCf::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_CF_TF1_EDIT, m_strCmdFreq);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_CF_SPT_DAY_DP, m_SpecDay);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_CF_SPT_TIME_DP, m_SpecTime);
	DDX_Control(pDX, IDC_FREQ_CTRL_SML_CF_SPT_DAY_DP, m_SpecDayDTCtrl);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrlSmCf, CDialogBase)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_CF_DPCALC_BTN, &CFrequencyCtrlSmCf::OnBnClickedFreqCtrlSmlCfDpcalcBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_CF_DPVIEW_BTN, &CFrequencyCtrlSmCf::OnBnClickedFreqCtrlSmlCfDpviewBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_CF_CALC_BTN, &CFrequencyCtrlSmCf::OnBnClickedFreqCtrlSmlCfCalcBtn)
END_MESSAGE_MAP()


// CFrequencyCtrlSmCf メッセージ ハンドラー


/*============================================================================*/
/*! CFrequencyCtrlSmCf

-# ドップラ補償値計算ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlSmCf::OnBnClickedFreqCtrlSmlCfDpcalcBtn()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


/*============================================================================*/
/*! CFrequencyCtrlSmCf

-# ドップラ補償値表示ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlSmCf::OnBnClickedFreqCtrlSmlCfDpviewBtn()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


/*============================================================================*/
/*! CFrequencyCtrlSmCf

-# 計算ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlSmCf::OnBnClickedFreqCtrlSmlCfCalcBtn()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


/*============================================================================*/
/*! CFrequencyCtrlSmCf

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CFrequencyCtrlSmCf::OnInitDialog()
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
/*! CFrequencyCtrlSmCf

-# ダイアログ更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CFrequencyCtrlSmCf::UpdateDlg()
{
	return TRUE;
}

