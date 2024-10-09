/*============================================================================*/
/*! FrequencyCtrlPre.cpp

-# 周波数制御画面-一括設定タブ
*/
/*============================================================================*/
// FrequencyCtrlPre.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlPre.h"
#include "afxdialogex.h"
#include "FrequencyCtrl.h"


// CFrequencyCtrlPre ダイアログ

IMPLEMENT_DYNAMIC(CFrequencyCtrlPre, CDialogBase)

/*============================================================================*/
/*! CFrequencyCtrlPre

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CFrequencyCtrlPre::CFrequencyCtrlPre(CWnd* pParent /*=NULL*/)
: CDialogBase(CFrequencyCtrlPre::IDD, pParent)
, m_strSendOp(_T(""))
, m_nSendGet(0)
{

}

/*============================================================================*/
/*! CFrequencyCtrlPre

-# コンストラクタ

@param	IDD：IDD
@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CFrequencyCtrlPre::CFrequencyCtrlPre(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialogBase(IDD, pParent)
{

}

/*============================================================================*/
/*! CFrequencyCtrlPre

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CFrequencyCtrlPre::~CFrequencyCtrlPre()
{
}

void CFrequencyCtrlPre::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SENDOP, m_strSendOp);
	DDX_Control(pDX, IDC_COMBO_SENDGET, m_cbSendGet);
	DDX_CBIndex(pDX, IDC_COMBO_SENDGET, m_nSendGet);
	DDX_Control(pDX, IDC_EDIT_SENDOP, m_SendOpEdit);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrlPre, CDialogBase)
	ON_BN_CLICKED(IDC_BUTTON_PRE_1WAY, &CFrequencyCtrlPre::OnBnClickedButtonPre1way)
	ON_BN_CLICKED(IDC_BUTTON_PRE_2WAY1, &CFrequencyCtrlPre::OnBnClickedButtonPre2way1)
	ON_BN_CLICKED(IDC_BUTTON_PRE_2WAY2, &CFrequencyCtrlPre::OnBnClickedButtonPre2way2)
	ON_BN_CLICKED(IDC_BUTTON_PRE_2WAY3, &CFrequencyCtrlPre::OnBnClickedButtonPre2way3)
	ON_BN_CLICKED(IDC_BUTTON_PRE_2WAY4, &CFrequencyCtrlPre::OnBnClickedButtonPre2way4)
	ON_BN_CLICKED(IDC_BUTTON_PRE_3WAY, &CFrequencyCtrlPre::OnBnClickedButtonPre3way)
	ON_BN_CLICKED(IDC_BUTTON_GET, &CFrequencyCtrlPre::OnBnClickedButtonGet)
END_MESSAGE_MAP()


// CFrequencyCtrlPre メッセージ ハンドラー


/*============================================================================*/
/*! CFrequencyCtrlPre

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CFrequencyCtrlPre::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// 送信周波数取得先
	for (int i = 0; i < sizeof(mSenderFreq[theApp.GetSelectStation()]) / sizeof(mSenderFreq[theApp.GetSelectStation()][0]); i++){
		int pos = m_cbSendGet.AddString(mSenderFreq[theApp.GetSelectStation()][i].sendname);
		m_cbSendGet.SetItemData(pos, mSenderFreq[theApp.GetSelectStation()][i].sendid);
	}
	m_cbSendGet.SetCurSel(0);
	m_SendOpEdit.SetRoundPlaceValue(1);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! CFrequencyCtrlPre

-# ダイアログ更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CFrequencyCtrlPre::UpdateDlg()
{
	return TRUE;
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# 「1WAY運用（テレメ単独運用）」ボタン押下時処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlPre::OnBnClickedButtonPre1way()
{
	SetData(ePre_1WAY);
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# 「2WAY運用（COH,動的）」ボタン押下時処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlPre::OnBnClickedButtonPre2way1()
{
	SetData(ePre_2WAY1);
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# 「2WAY運用（COH,センター）」ボタン押下時処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlPre::OnBnClickedButtonPre2way2()
{
	SetData(ePre_2WAY2);
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# 「2WAY運用（INCOH,動的）」ボタン押下時処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlPre::OnBnClickedButtonPre2way3()
{
	SetData(ePre_2WAY3);
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# 「2WAY運用（INCOH,センター）」ボタン押下時処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlPre::OnBnClickedButtonPre2way4()
{
	SetData(ePre_2WAY4);
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# 「3WAY」ボタン押下時処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlPre::OnBnClickedButtonPre3way()
{
	SetData(ePre_3WAY);
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# 「取得」ボタン押下時処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlPre::OnBnClickedButtonGet()
{
	UpdateData(TRUE);
	// 相手局送信周波数を取得する
	int pos = m_cbSendGet.GetCurSel();
	UINT target = (UINT)m_cbSendGet.GetItemData(pos);
	CSatelliteData& sd = theApp.GetSatelliteData();
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(target, _T("X-UC.FREQUENCY"));
	m_strSendOp = _T("0.0");
	if (obs_adr != NULL){
		m_strSendOp.Format(_T("%0.1f"), obs_adr->d_data);
	}
	UpdateData(FALSE);
}

/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# 画面更新

@param  なし
@retval なし
*/
/*============================================================================*/
void CFrequencyCtrlPre::SetData(int type)
{
	CFrequencyCtrlPre::stSetCtlData mPreTable[] = {
		{ _T("CTRL.RX_OPE 1WAY") },
		{ _T("CTRL.RX_OPE 2WAY_COH_DYN") },
		{ _T("CTRL.RX_OPE 2WAY_COH_AVE") },
		{ _T("CTRL.RX_OPE 2WAY_INCOH_DYN") },
		{ _T("CTRL.RX_OPE 2WAY_INCOH_AVE") },
		{ _T("CTRL.RX_OPE 3WAY@%s") },
	};

	UpdateData(TRUE);

	/****************************************************************/
	/* 制御情報 */
	CString ctrl;
	ctrl = mPreTable[type].ctlname;
	if (type == ePre_3WAY)
	{
		if (m_strSendOp.IsEmpty() == true)
		{
			MessageBox(_T("相手局送信周波数が空です。"), 0, MB_OK | MB_ICONERROR);
			return;
		}
		if (m_strSendOp == _T("+") || m_strSendOp == _T("-"))
		{
			MessageBox(_T("数値を入力してください。"), 0, MB_OK | MB_ICONERROR);
			return;
		}

		double val = _wtof(m_strSendOp);
		if ((val < 1000000000.0) || (val >= 100000000000.0))
		{
			MessageBox(_T("相手局送信周波数は1000000000.0～99999999999.9[Hz]を入力してください。"), 0, MB_OK | MB_ICONERROR);
			return;
		}

		CString tmp = ctrl;
		ctrl.Format(tmp, m_strSendOp);
	}
	theApp.SendControl(ctrl, _T(""), _T(""), _T(""));
}
