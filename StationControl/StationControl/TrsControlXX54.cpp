/*============================================================================*/
/*! CTrsControlXX54.cpp

-# X帯送信制御画面 (54m局)
*/
/*============================================================================*/
// CTrsControlXX54.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrsControlXX54.h"
#include "afxdialogex.h"

// CTrsControlXX54 ダイアログ

IMPLEMENT_DYNAMIC(CTrsControlXX54, CTrsControlXX)

CTrsControlXX54::CTrsControlXX54(CWnd* pParent /*=NULL*/)
: CTrsControlXX(CTrsControlXX54::IDD, pParent)
{

}

CTrsControlXX54::~CTrsControlXX54()
{
}

void CTrsControlXX54::DoDataExchange(CDataExchange* pDX)
{
	CTrsControlXX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DELTAF_RECOVERY_GROUP, m_grDeltafRecovery);
	DDX_Control(pDX, IDC_STATIC_DELTAF_TIMERATE_GROUP, m_grDelfafTimeRate);
	DDX_Text(pDX, IDC_STATIC_DELTAF_MON, m_DeltafCtrlFreqMoni);
	DDX_Control(pDX, IDC_STATIC_DELTAF_RECOVERY_BOX, m_grRecovery);
	DDX_Control(pDX, IDC_TRXXCTRL_RECOVERY_BTNOFF, m_RecoveryOffBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_RECOVERY_BTNON, m_RecoveryOnBtn);
	DDX_Check(pDX, IDC_TRXXCTRL_DELTAF_CTRL_CHECK, m_DeltafCtrlFreqCtrChkBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_DELTAF_CTRL_CHECK, m_DeltafCtrlChkBtnCtrl);
	DDX_Control(pDX, IDC_TRXXCTRL_DELTAF_CTRL_EDIT, m_strDeltafCtrl);
	DDX_Control(pDX, IDC_TRXXCTRL_DELTAF_TIME_CHECK, m_DeltafCtrlTimeChkBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_DELTAF_TIME_EDIT, m_strDeltafTime);
	DDX_Control(pDX, IDC_TRXXCTRL_DELTAF_RATE_CHECK, m_DeltafCtrlFreqChkBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_DELTAF_RATE_EDIT, m_strDeltafFreq);
	DDX_Text(pDX, IDC_STATIC_DELTAF_ENDTIME, m_DeltafCtrlFreqEndTime);
}

BEGIN_MESSAGE_MAP(CTrsControlXX54, CTrsControlXX)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TRXXCTRL_RF_OFF_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlRfOffBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RF_ON_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlRfOnBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_NOUSE_OFF_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlNouseOffBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_NOUSE_ON_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlNouseOnBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_XXCOH_COH_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlXxcohCohBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_XXCOH_INCOH_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlXxcohIncohBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RNGING_STOP_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlRngingStopBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RNGING_START_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlRngingStartBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RGNTYPE_STOP_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlRgntypeStopBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RGNTYPE_START_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlRgntypeStartBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RECOVERY_BTNOFF, &CTrsControlXX54::OnBnClickedRecoveryOffBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RECOVERY_BTNON, &CTrsControlXX54::OnBnClickedRecoveryOnBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_DELTAF_CTRL_CHECK, &CTrsControlXX54::OnBnClickedDeltafCtrlFreqCtrChk)
	ON_BN_CLICKED(IDC_TRXXCTRL_DELTAF_TIME_CHECK, &CTrsControlXX54::OnBnClickedDeltafCtrlTimeChk)
	ON_BN_CLICKED(IDC_TRXXCTRL_DELTAF_RATE_CHECK, &CTrsControlXX54::OnBnClickedDeltafCtrlFreqChk)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_STOP_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetStopBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_INC_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetIncBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_INCDEC_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetIncdecBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_DEC_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetDecBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_DECINC_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetDecincBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_STOP_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetSubStopBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_INC_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetSubIncBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_INCDEC_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetSubIncdecBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_DEC_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetSubDecBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_DECINC_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetSubDecincBtn)
	ON_EN_CHANGE(IDC_TRXXCTRL_DELTAF_TIME_EDIT, &CTrsControlXX54::OnEnChangeTrxxctrlDeltafTimeEdit)
	ON_BN_CLICKED(IDC_TRXXCTRL_HOLD_OFF_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlHoldOffBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_HOLD_ON_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlHoldOnBtn)
END_MESSAGE_MAP()

/*============================================================================*/
// ダイアログ初期化処理
/*============================================================================*/
BOOL CTrsControlXX54::OnInitDialog()
{
	CTrsControlXX::OnInitDialog();

	m_DeltafCtrlFreqCtrChkBtn = FALSE;			//Δf（CTRL）チェックボックス
	m_strDeltafCtrl.EnableWindow(FALSE);		//Δf（CTRL）入力非活性
	m_strDeltafFreq.SetWindowText(_T("0.1"));	//RATE
	m_strPreDeltafTime = _T("1");
	m_strDeltafTime.SetWindowText(m_strPreDeltafTime);		//TIME
	m_strDeltafFreq.EnableWindow(FALSE);		//RATE EDIT
	m_strDeltafTime.EnableWindow(FALSE);		//TIME EDIT

	// RECOVERY
	m_RecoveryOffBtn.EnableWindow(FALSE);		//RECOVERY OFF
	m_RecoveryOffBtn.SetColor(COLOR_REMOTES);	//選択   オレンジ
	m_RecoveryOffBtn.SetCtrlButton(TRUE);		//Ctrlキー +
	m_RecoveryOnBtn.SetColor(COLOR_REMOTE);		//非選択 青
	m_RecoveryOnBtn.SetCtrlButton(TRUE);		//Ctrlキー +
	m_RecoveryCont = FALSE;
	m_RngFlg = FALSE;
	m_strDeltafCtrlInit = FALSE;

	// グループボックス
	m_grDeltafRecovery.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grDeltafRecovery.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grDelfafTimeRate.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grDelfafTimeRate.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRecovery.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRecovery.SetGroupType(CGroupBoxEx::eGroupType_Round);

    m_grXXRng.SetWindowText(_T("RANGING NUM"));
    m_grXXCoh.SetWindowText(_T("COH"));

	m_grNoUse.ShowWindow(SW_HIDE);
	m_NoUseOffBtn.ShowWindow(SW_HIDE);
	m_NoUseOnBtn.ShowWindow(SW_HIDE);

	CenterWindowEx();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}

/*============================================================================*/
// ダイアログ更新
/*============================================================================*/
BOOL CTrsControlXX54::UpdateDlg()
{
	// 画面更新
	SetData();

	CTrsControlXX::UpdateDlg();

	return TRUE;
}

/*============================================================================*/
// 画面更新
/*============================================================================*/
void CTrsControlXX54::SetData()
{
	CTrsControlXX54::stSetMonData mMonTable[] = {
		{ _T("CTRL.DELTA_FREQ"),       m_DeltafCtrlFreqMoni,    _T("%0.1f"), eDOUBLE },	// Δf（MON）
		{ _T("CTRL.RECOVER_END_TIME"), m_DeltafCtrlFreqEndTime, _T("%s"),    eSTRING },	// 終了予定日時
		{ _T("CTRL.RECOVER_CONT"),     m_RecoveryOffOn,         _T("%s"),    eSTRING },	// RECOVERY OFF/ON
		{ _T("CTRL.RNG_FLG"),          m_strRngFlg,             _T("%s"),    eSTRING },	// Range計測中 終了/開始
	};

	// タイマ破棄
	KillTimer(TID_DELTAF_TRS);

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
				m_strDeltafCtrl.SetWindowText(m_DeltafCtrlFreqMoni);	//Δf(CTRL)の初期値は、監視データ「Δf」の値
				m_strDeltafCtrlInit = TRUE;								//初期値設定済
			}
		}
		else
		{
			//TRACE("GetObsname_t()エラー 監視名:%s\n", CStringA(mMonTable[index].monname));
		}
	}

//	//RECOVERY:ON or Range計測:開始 ならボタンや入力を非活性化
//	if( (m_RecoveryOffOn == _T("ON")) || (m_strRngFlg == _T("開始")) )
//	{
//		if(m_RecoveryCont != TRUE)
//		{
//			m_RecoveryCont = TRUE;
//			m_DeltafCtrlChkBtnCtrl.EnableWindow(FALSE);		//Δf (CTRL) CHECK
//			m_strDeltafCtrl.EnableWindow(FALSE);			//Δf (CTRL) EDIT
//			m_DeltafCtrlTimeChkBtn.EnableWindow(FALSE);		//TIME CHECK
//			m_strDeltafTime.EnableWindow(FALSE);			//TIME EDIT
//			m_DeltafCtrlFreqChkBtn.EnableWindow(FALSE);		//RATE CHECK
//			m_strDeltafFreq.EnableWindow(FALSE);			//RATE EDIT
//			m_RecoveryOffBtn.SetColor(COLOR_REMOTE);		//非選択 青
//			m_RecoveryOnBtn.SetColor(COLOR_REMOTES);		//選択   オレンジ
//			// 更新
//			// m_RecoveryOffBtn.Invalidate();
//			// m_RecoveryOnBtn.Invalidate();
//			//RNG STARTボタン(従来型及び再生型)は、監視データ「CTRL.RECOVER_CONT」が「ON」の場合はクリックできないようにする。
//			m_RngingStartBtn.EnableWindow(FALSE);			//RANGING STARTボタン
//			m_RRngStartBtn.EnableWindow(FALSE);				//再生型 STARTボタン
//			// 活性・非活性
//			m_RecoveryOffBtn.EnableWindow(TRUE);			//RECOVERY OFF
//			m_RecoveryOnBtn.EnableWindow(FALSE);			//RECOVERY ON
//
//			// 再描画処理
//			InvalidateRect(NULL, TRUE);
//		}
//	}
//	//RECOVERY:OFF and Range計測:終了 ならボタンや入力を活性化
//	else
//	{
//		if(m_RecoveryCont == TRUE)
//		{
//			m_RecoveryCont = FALSE;
//			m_DeltafCtrlChkBtnCtrl.EnableWindow(TRUE);		//Δf (CTRL) CHECK
//			m_strDeltafCtrl.EnableWindow(TRUE);				//Δf (CTRL) EDIT
//			m_DeltafCtrlTimeChkBtn.EnableWindow(TRUE);		//TIME CHECK
//			m_DeltafCtrlFreqChkBtn.EnableWindow(TRUE);		//RATE CHECK
//			if (m_DeltafCtrlTimeChkBtn.GetCheck() == 1)			// TIME にチェック
//			{
//				m_strDeltafTime.EnableWindow(TRUE);				//TIME EDIT
//				m_strDeltafFreq.EnableWindow(FALSE);			//RATE EDIT
//			}
//			else if (m_DeltafCtrlFreqChkBtn.GetCheck() == 1)	//RATE にチェック
//			{
//				m_strDeltafFreq.EnableWindow(TRUE);				//RATE EDIT
//				m_strDeltafTime.EnableWindow(FALSE);			//TIME EDIT
//			}
//			m_RecoveryOffBtn.SetColor(COLOR_REMOTES);		//選択   オレンジ
//			m_RecoveryOnBtn.SetColor(COLOR_REMOTE);			//非選択 青
//			// 更新
//			// m_RecoveryOffBtn.Invalidate();
//			// m_RecoveryOnBtn.Invalidate();
//			// 活性
//			m_RngingStartBtn.EnableWindow(TRUE);			//RANGING STARTボタン
//			m_RRngStartBtn.EnableWindow(TRUE);				//再生型 STARTボタン
//			// 活性・非活性
//			m_RecoveryOffBtn.EnableWindow(FALSE);			//RECOVERY OFF
//			m_RecoveryOnBtn.EnableWindow(TRUE);				//RECOVERY ON
//
//			// 再描画処理
//			InvalidateRect(NULL, TRUE);
//		}
//	}

	//Range計測:開始ならボタンや入力を全部非活性化
	if (m_strRngFlg == _T("開始"))
	{
		// 直前がレンジ計測中でなかった場合、描画変更
		if (m_RngFlg != TRUE)
		{
			// レンジ計測中フラグセット
			m_RngFlg = TRUE;
			m_DeltafCtrlChkBtnCtrl.EnableWindow(FALSE);		//Δf (CTRL) CHECK
			m_strDeltafCtrl.EnableWindow(FALSE);			//Δf (CTRL) EDIT
			m_DeltafCtrlTimeChkBtn.EnableWindow(FALSE);		//TIME CHECK
			m_strDeltafTime.EnableWindow(FALSE);			//TIME EDIT
			m_DeltafCtrlFreqChkBtn.EnableWindow(FALSE);		//RATE CHECK
			m_strDeltafFreq.EnableWindow(FALSE);			//RATE EDIT
			m_RecoveryOffBtn.SetColor(COLOR_REMOTES);		//選択   オレンジ
			m_RecoveryOnBtn.SetColor(COLOR_LOCAL);			//ローカル　黒
			// 更新
			// m_RecoveryOffBtn.Invalidate();
			// m_RecoveryOnBtn.Invalidate();
			//RNG STARTボタン(従来型及び再生型)は、監視データ「CTRL.RECOVER_CONT」が「ON」の場合はクリックできないようにする。
			m_RngingStartBtn.EnableWindow(FALSE);			//RANGING STARTボタン
			m_RRngStartBtn.EnableWindow(FALSE);				//再生型 STARTボタン
			// 活性・非活性
			m_RecoveryOffBtn.EnableWindow(FALSE);			//RECOVERY OFF
			m_RecoveryOnBtn.EnableWindow(FALSE);			//RECOVERY ON

			// 再描画処理
			InvalidateRect(NULL, TRUE);
		}
	}
	//Range計測:開始以外
	else
	{
		//RECOVERY:ONならOFFボタンのみ活性化
		if (m_RecoveryOffOn == _T("ON"))
		{
			// 直前がレンジ計測中、またはリカバリ中でなかった場合、描画変更
			if (m_RngFlg == TRUE || m_RecoveryCont != TRUE)
			{
				// レンジ計測中フラグクリア
				m_RngFlg = FALSE;
				// リカバリーフラグセット
				m_RecoveryCont = TRUE;
				m_DeltafCtrlChkBtnCtrl.EnableWindow(FALSE);		//Δf (CTRL) CHECK
				m_strDeltafCtrl.EnableWindow(FALSE);			//Δf (CTRL) EDIT
				m_DeltafCtrlTimeChkBtn.EnableWindow(FALSE);		//TIME CHECK
				m_strDeltafTime.EnableWindow(FALSE);			//TIME EDIT
				m_DeltafCtrlFreqChkBtn.EnableWindow(FALSE);		//RATE CHECK
				m_strDeltafFreq.EnableWindow(FALSE);			//RATE EDIT
				m_RecoveryOffBtn.SetColor(COLOR_REMOTE);		//非選択 青
				m_RecoveryOnBtn.SetColor(COLOR_REMOTES);		//選択   オレンジ
				// 更新
				// m_RecoveryOffBtn.Invalidate();
				// m_RecoveryOnBtn.Invalidate();
				//RNG STARTボタン(従来型及び再生型)は、監視データ「CTRL.RECOVER_CONT」が「ON」の場合はクリックできないようにする。
				m_RngingStartBtn.EnableWindow(FALSE);			//RANGING STARTボタン
				m_RRngStartBtn.EnableWindow(FALSE);				//再生型 STARTボタン
				// 活性・非活性
				m_RecoveryOffBtn.EnableWindow(TRUE);			//RECOVERY OFF
				m_RecoveryOnBtn.EnableWindow(FALSE);			//RECOVERY ON

				// 再描画処理
				InvalidateRect(NULL, TRUE);
			}
		}
		//RECOVERY:OFFなら停止ボタン以外と入力を活性化。
		else
		{
			// 直前がレンジ計測中、またはリカバリー中だった場合、描画変更
			if (m_RngFlg == TRUE || m_RecoveryCont == TRUE)
			{
				// レンジ計測中フラグクリア
				m_RngFlg = FALSE;
				// リカバリ中フラグクリア
				m_RecoveryCont = FALSE;
				m_DeltafCtrlChkBtnCtrl.EnableWindow(TRUE);		//Δf (CTRL) CHECK
				m_strDeltafCtrl.EnableWindow(TRUE);				//Δf (CTRL) EDIT
				m_DeltafCtrlTimeChkBtn.EnableWindow(TRUE);		//TIME CHECK
				m_DeltafCtrlFreqChkBtn.EnableWindow(TRUE);		//RATE CHECK
				if (m_DeltafCtrlTimeChkBtn.GetCheck() == 1)			// TIME にチェック
				{
					m_strDeltafTime.EnableWindow(TRUE);				//TIME EDIT
					m_strDeltafFreq.EnableWindow(FALSE);			//RATE EDIT
				}
				else if (m_DeltafCtrlFreqChkBtn.GetCheck() == 1)	//RATE にチェック
				{
					m_strDeltafFreq.EnableWindow(TRUE);				//RATE EDIT
					m_strDeltafTime.EnableWindow(FALSE);			//TIME EDIT
				}
				m_RecoveryOffBtn.SetColor(COLOR_REMOTES);		//選択   オレンジ
				m_RecoveryOnBtn.SetColor(COLOR_REMOTE);			//非選択 青
				// 更新
				// m_RecoveryOffBtn.Invalidate();
				// m_RecoveryOnBtn.Invalidate();
				// 活性
				m_RngingStartBtn.EnableWindow(TRUE);			//RANGING STARTボタン
				m_RRngStartBtn.EnableWindow(TRUE);				//再生型 STARTボタン
				// 活性・非活性
				m_RecoveryOffBtn.EnableWindow(FALSE);			//RECOVERY OFF
				m_RecoveryOnBtn.EnableWindow(TRUE);				//RECOVERY ON

				// 再描画処理
				InvalidateRect(NULL, TRUE);
			}
		}
	}

	UpdateData(FALSE);

	// タイマ作成
	SetTimer(TID_DELTAF_TRS, INTERVAL_DELTAF, 0);
}

/*============================================================================*/
// タイマ処理
/*============================================================================*/
void CTrsControlXX54::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_DELTAF_TRS)
	{
		SetData();
	}

	CTrsControlXX::OnTimer(nIDEvent);
}

/*============================================================================*/
// 『TX CONTROL』-『RF-POWER』-『OFF』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlRfOffBtn()
{
	// ボタンを制御中にする
	m_RfPwrOffBtn.SetColor(COLOR_CTRLIN);
	m_RfPwrOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("X-SSPA.TX_ON/OFF"), _T("OFF"));
	theApp.GetSatelliteData().opctproc(_T("X-TX_IF.TX1_OUTPUT"), _T("OFF"));

	// カウントダウン開始
	m_l_rf_power_off_cntdwn = RF_POWER_TIME_XX_LIMIT;

	// 更新
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}

/*============================================================================*/
// 『TX CONTROL』-『RF-POWER』-『ON』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlRfOnBtn()
{
	// ボタンを制御中にする
	m_RfPwrOffBtn.SetColor(COLOR_CTRLIN);
	m_RfPwrOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("X-TX_IF.TX1_OUTPUT"), _T("ON"));
	theApp.GetSatelliteData().opctproc(_T("X-SSPA.TX_ON/OFF"), _T("ON"));

	// カウントダウン開始
	m_l_rf_power_on_cntdwn = RF_POWER_TIME_XX_LIMIT;

	// 更新
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}

/*============================================================================*/
// 『TX CONTROL』-『NO-USE』-『OFF』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlNouseOffBtn()
{
	COLORREF col = m_NoUseOffBtn.GetColor();

	if (col == COLOR_REMOTE)
	{
		// リモート（非選択）

		// ボタンを制御中にする
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// 制御通知送信
		theApp.GetSatelliteData().opctproc(_T("SSPA.RF_CUT_NO_USE"), _T("OFF"));

		// カウントダウン開始
		m_l_no_use_off_cntdwn = NO_USE_TIME_XX_LIMIT;
	}
	else if (col == COLOR_CTRLIN)
	{
		// 制御中

		// ボタンを制御中にする
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// 制御通知送信
		theApp.GetSatelliteData().opctproc(_T("SSPA.RF_CUT_NO_USE"), _T("OFF"));

		// カウントダウン開始
		m_l_no_use_off_cntdwn = NO_USE_TIME_XX_LIMIT;

		// 他方のカウントダウン終了
		m_l_no_use_on_cntdwn = -1;
	}

	// 更新
	m_NoUseOffBtn.Invalidate();
}

/*============================================================================*/
// 『TX CONTROL』-『NO-USE』-『ON』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlNouseOnBtn()
{
	COLORREF col = m_NoUseOnBtn.GetColor();

	if (col == COLOR_REMOTE)
	{
		// リモート（非選択）

		// ボタンを制御中にする
		m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

		// 制御通知送信
		theApp.GetSatelliteData().opctproc(_T("SSPA.RF_CUT_NO_USE"), _T("ON"));

		// カウントダウン開始
		m_l_no_use_on_cntdwn = NO_USE_TIME_XX_LIMIT;
	}
	else if (col == COLOR_CTRLIN)
	{
		// 制御中

		// ボタンを制御中にする
		m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

		// 制御通知送信
		theApp.GetSatelliteData().opctproc(_T("SSPA.RF_CUT_NO_USE"), _T("ON"));

		// カウントダウン開始
		m_l_no_use_on_cntdwn = NO_USE_TIME_XX_LIMIT;

		// 他方のカウントダウン終了
		m_l_no_use_off_cntdwn = -1;
	}

	// 更新
	m_NoUseOnBtn.Invalidate();
}

/*============================================================================*/
// 『TX CONTROL』-『SWEEP-CONT』-『STOP』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetStopBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("SWEEP_OFF"));
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("STOP"));

	// カウントダウン開始
	m_l_main_sweep_cont_stop_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountMStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecIncBtn.SetColor(COLOR_CTRLIN);
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}

/*============================================================================*/
// 『TX CONTROL』-『SWEEP-CONT』-『INC』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetIncBtn()
{
	if (m_SweepContNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("SWEEP_ON"));

	// カウントダウン開始
	m_l_main_sweep_cont_inc_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountMStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecIncBtn.SetColor(COLOR_CTRLIN);
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}

/*============================================================================*/
// 『TX CONTROL』-『SWEEP-CONT』-『INCDEC』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetIncdecBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("SWEEP_ON"));

	// カウントダウン開始
	m_l_main_sweep_cont_incdec_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountMStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecIncBtn.SetColor(COLOR_CTRLIN);
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}

/*============================================================================*/
// 『TX CONTROL』-『SWEEP-CONT』-『DEC』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetDecBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("SWEEP_ON"));

	// カウントダウン開始
	m_l_main_sweep_cont_dec_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountMStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecIncBtn.SetColor(COLOR_CTRLIN);
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}

/*============================================================================*/
// 『TX CONTROL』-『SWEEP-CONT』-『DECINC』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetDecincBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("SWEEP_ON"));

	// カウントダウン開始
	m_l_main_sweep_cont_decinc_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountMStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecIncBtn.SetColor(COLOR_CTRLIN);
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// 更新
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}

/*============================================================================*/
// 『TX CONTROL』-『SWEEP-CONT(SubCarr)』-『STOP』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetSubStopBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContSubNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("SWEEP_OFF"));
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("STOP"));

	// カウントダウン開始
	m_l_sub_sweep_cont_stop_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountSStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecIncBtn.SetColor(COLOR_CTRLIN);

	m_SweepContSubChkBtn.EnableWindow(FALSE);
	m_SweepContSubNorChkBtn.EnableWindow(FALSE);
	m_SweepContSubTriChkBtn.EnableWindow(FALSE);

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}

/*============================================================================*/
// 『TX CONTROL』-『SWEEP-CONT(SubCarr)』-『INC』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetSubIncBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContSubNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("SWEEP_ON"));

	// カウントダウン開始
	m_l_sub_sweep_cont_inc_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountSStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecIncBtn.SetColor(COLOR_CTRLIN);

	m_SweepContSubChkBtn.EnableWindow(FALSE);
	m_SweepContSubNorChkBtn.EnableWindow(FALSE);
	m_SweepContSubTriChkBtn.EnableWindow(FALSE);

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}

/*============================================================================*/
// 『TX CONTROL』-『SWEEP-CONT(SubCarr)』-『INCDEC』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetSubIncdecBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContSubNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("SWEEP_ON"));

	// カウントダウン開始
	m_l_sub_sweep_cont_incdec_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountSStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecIncBtn.SetColor(COLOR_CTRLIN);

	m_SweepContSubChkBtn.EnableWindow(FALSE);
	m_SweepContSubNorChkBtn.EnableWindow(FALSE);
	m_SweepContSubTriChkBtn.EnableWindow(FALSE);

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}

/*============================================================================*/
// 『TX CONTROL』-『SWEEP-CONT(SubCarr)』-『DEC』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetSubDecBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContSubNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("SWEEP_ON"));

	// カウントダウン開始
	m_l_sub_sweep_cont_dec_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountSStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecIncBtn.SetColor(COLOR_CTRLIN);

	m_SweepContSubChkBtn.EnableWindow(FALSE);
	m_SweepContSubNorChkBtn.EnableWindow(FALSE);
	m_SweepContSubTriChkBtn.EnableWindow(FALSE);

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}

/*============================================================================*/
// 『TX CONTROL』-『SWEEP-CONT(SubCarr)』-『DECINC』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetSubDecincBtn()
{
	//  SAW or TRIANGLE スイープの取得
	if (m_SweepContSubNorChkBtn.GetCheck() == 1)	// NOR にチェック
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("SWEEP_ON"));

	// カウントダウン開始
	m_l_sub_sweep_cont_decinc_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// ボタンを制御中にする
	m_SweepCountSStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecIncBtn.SetColor(COLOR_CTRLIN);

	m_SweepContSubChkBtn.EnableWindow(FALSE);
	m_SweepContSubNorChkBtn.EnableWindow(FALSE);
	m_SweepContSubTriChkBtn.EnableWindow(FALSE);

	// 更新
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}

/*============================================================================*/
// 『RX CONTROL』-『X/X-COH』-『COH』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlXxcohCohBtn()
{
	// ボタンを制御中にする
	m_XXCohCohBtn.SetColor(COLOR_CTRLIN);
	m_XXCohInCohBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("COH"));

	// カウントダウン開始
	m_l_coh_incoh_coh_cntdwn = COH_INCOH_TIME_XX_LIMIT;

	// 更新
	m_XXCohCohBtn.Invalidate();
	m_XXCohInCohBtn.Invalidate();
}

/*============================================================================*/
// 『RX CONTROL』-『X/X-COH』-『INCOH』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlXxcohIncohBtn()
{
	// ボタンを制御中にする
	m_XXCohCohBtn.SetColor(COLOR_CTRLIN);
	m_XXCohInCohBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("INCOH"));

	// カウントダウン開始
	m_l_coh_incoh_incoh_cntdwn = COH_INCOH_TIME_XX_LIMIT;

	// 更新
	m_XXCohCohBtn.Invalidate();
	m_XXCohInCohBtn.Invalidate();
}

/*============================================================================*/
// 『RX CONTROL』-『RNGING』-『STOP』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlRngingStopBtn()
{
	// ボタンを制御中にする
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING_CANCEL"), _T(","));

	// カウントダウン開始
	m_l_rnging_stop_cntdwn = RNGING_TIME_XX64_LIMIT;

	// RNG種別フラグをクリアする
	m_l_rng_type = RNG_TYPE_NON;

	// 更新
	m_RngingStopBtn.Invalidate();
}

/*============================================================================*/
// 『RX CONTROL』-『RNGING』-『START』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlRngingStartBtn()
{
	// ボタンを制御中にする
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);
	m_RngingStartBtn.SetColor(COLOR_CTRLIN);

	// 再生型ボタンをインセンシティブにする
	if (m_RngingStartBtn.IsWindowEnabled() == TRUE)
	{
		m_RRngStopBtn.EnableWindow(FALSE);
		m_RRngStartBtn.EnableWindow(FALSE);
	}

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CTRL.SEL_DSN_RNG_MODE"), _T("TURNAROUND"));
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.RNG_MODE"), _T("TURNAROUND"));

	// 従来型切り替え制御実行中を設定
	m_l_dsnrng_turn_start_flag = 1;

	// 更新
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}

/*============================================================================*/
// 『RX CONTROL』-『再生型』-『STOP』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlRgntypeStopBtn()
{
	// 再生型ボタンをインセンシティブにする
	m_RRngStopBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING_CANCEL"), _T(","));

	// カウントダウン開始
	m_l_rep_rnging_stop_cntdwn = REP_RNGING_TIME_XX64_LIMIT;

	// RNG種別フラグをクリアする
	m_l_rng_type = RNG_TYPE_NON;

	// 更新
	m_RRngStopBtn.Invalidate();
}

/*============================================================================*/
// 『RX CONTROL』-『再生型』-『START』ボタン押下処理
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlRgntypeStartBtn()
{
	// ボタンを制御中にする
	m_RRngStopBtn.SetColor(COLOR_CTRLIN);
	m_RRngStartBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("CTRL.SEL_DSN_RNG_MODE"), _T("REGEN"));
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.RNG_MODE"), _T("REGEN"));

	// 従来型ボタンをインセンシティブにする
	if (m_RngingStopBtn.IsWindowEnabled() == TRUE)
	{
		m_RngingStopBtn.EnableWindow(FALSE);
		m_RngingStartBtn.EnableWindow(FALSE);
	}

	// 再生型切り替え制御実行中を設定
	m_l_dsnrng_regen_start_flag = 1;

	// 更新
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}

/*============================================================================*/
// Δf（CTRL）チェックボックス
/*============================================================================*/
void CTrsControlXX54::OnBnClickedDeltafCtrlFreqCtrChk()
{
	UpdateData();

	if (m_DeltafCtrlFreqCtrChkBtn)
	{
		// Δf（CTRL）入力活性
		m_strDeltafCtrl.EnableWindow(TRUE);
	}
	else
	{
		// Δf（CTRL）入力非活性
		m_strDeltafCtrl.EnableWindow(FALSE);
	}
}

/*============================================================================*/
//Δfリカバリ TIMEチェックボックス
/*============================================================================*/
void CTrsControlXX54::OnBnClickedDeltafCtrlTimeChk()
{
	m_DeltafCtrlFreqChkBtn.SetCheck(0);
	m_DeltafCtrlTimeChkBtn.SetCheck(1);
	m_strDeltafTime.EnableWindow(TRUE);				//TIME EDIT
	m_strDeltafFreq.EnableWindow(FALSE);			//RATE EDIT
}

/*============================================================================*/
//Δfリカバリ RATEチェックボックス
/*============================================================================*/
void CTrsControlXX54::OnBnClickedDeltafCtrlFreqChk()
{
	m_DeltafCtrlFreqChkBtn.SetCheck(1);
	m_DeltafCtrlTimeChkBtn.SetCheck(0);
	m_strDeltafFreq.EnableWindow(TRUE);				//RATE EDIT
	m_strDeltafTime.EnableWindow(FALSE);			//TIME EDIT
}

/*============================================================================*/
//Δfリカバリ ONボタン
/*============================================================================*/
void CTrsControlXX54::OnBnClickedRecoveryOnBtn()
{
	// ①ラジオボタンで「TIME」が選択された場合
	//   CTRL.DF_START,DFxxxxxxxxx@Tyyyyyyyyy,;
	// ②ラジオボタンで「RATE」が選択された場合
	//   CTRL.DF_START,DFxxxxxxxxx@Rzzzzzzzzz,;
	// xxxxxxxxx:Δf(制御)チェックボックスが
	//   チェックされている場合は、その表示値を設定[Hz]
	//   チェックされていない場合は、「-」を設定
	// yyyyyyyyy:TIME欄の表示値を設定[sまたは、YYYY-MM-DD_hh:mm:ss]
	// zzzzzzzzz:RATE欄の表示値を設定[Hz/s]
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
			str.Format(_T("Δf（CTRL）は0.0を除く±4000000.0以内、かつ絶対値にした場合にモニタ値%sの絶対値以内を入力してください。"), m_DeltafCtrlFreqMoni);
			MessageBox(str, 0, MB_OK | MB_ICONERROR);
			return;
		}
	}
	else
	{
		deltaFCtrl = _T("-");
	}
	strCtrl = _T("CTRL.DF_START,DF") + deltaFCtrl;	//"CTRL.DF_START,DFxxxxxxxxx"

	if (m_DeltafCtrlTimeChkBtn.GetCheck() == 1)	//TIME にチェック
	{
		int year, mon, day, hour, min, sec;
		TCHAR sz_time_in[40], sz_time_chk[40];
		int sz = 40;

		m_strDeltafTime.GetWindowText(ctrlTime);
		if (ctrlTime.GetLength() >= sz || wcscpy_s(sz_time_in, sz, (LPCTSTR)ctrlTime) != 0)		// サイズ（終端文字分'\0'を考慮）チェック後に、文字列コピー
		{
			MessageBox(_T("TIMEの日時指定が間違っています。"), 0, MB_OK | MB_ICONERROR);
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
				MessageBox(_T("TIMEの日時指定が間違っています。"), 0, MB_OK | MB_ICONERROR);
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
				MessageBox(_T("TIMEは1～86400[秒]を入力してください。"), 0, MB_OK | MB_ICONERROR);
				return;
			}
			//TRACE("秒指定:%d\n", CStringA(ctrlTime));
		}
		else
		{
			MessageBox(_T("TIMEは日時指定か秒を入力してください。"), 0, MB_OK | MB_ICONERROR);
			return;
		}
		strCtrl += _T("@T") + ctrlTime + _T(",;");	//"CTRL.DF_START,DFxxxxxxxxx@Tyyyyyyyyy,;"
	}
	else if (m_DeltafCtrlFreqChkBtn.GetCheck() == 1)	//RATE にチェック
	{
		m_strDeltafFreq.GetWindowText(ctrlFreq);
		check = CheckFrequencyRate(ctrlFreq);
		if(check != TRUE)
		{
			MessageBox(_T("RATEは0.1～4000000.0を入力してください。"), 0, MB_OK | MB_ICONERROR);
			return;
		}
		strCtrl += _T("@R") + ctrlFreq + _T(",;");	//"CTRL.DF_START,DFxxxxxxxxx@Rzzzzzzzzz,;"
	}
	else
	{
		MessageBox(_T("RATEかTIMEを指定してください。"), 0, MB_OK | MB_ICONERROR);
		return;
	}

	// ON」ボタンがクリックされた場合、以下の制御文字列を生成し送信する。
	//TRACE("%s\n", CStringA(strCtrl));
	theApp.SendControlNoUI(strCtrl, _T(""), _T(""), _T(""));
}

/*============================================================================*/
//Δfリカバリ OFFボタン
/*============================================================================*/
void CTrsControlXX54::OnBnClickedRecoveryOffBtn()
{
	// OFF」ボタンがクリックされた場合、以下の制御文字列を生成し送信する。
	//TRACE("CTRL.DF_STOP,,;\n");
	theApp.SendControlNoUI(_T("CTRL.DF_STOP,,;"), _T(""), _T(""), _T(""));
}

/*============================================================================*/
//TIME(秒) 範囲チェック
/*============================================================================*/
BOOL CTrsControlXX54::CheckTimeSec(CString second)
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
//RATE 範囲チェック
/*============================================================================*/
BOOL CTrsControlXX54::CheckFrequencyRate(CString freq)
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
//Δf（CTRL） 範囲チェック
/*============================================================================*/
BOOL CTrsControlXX54::CheckFrequencyInput(CString freq)
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


/*============================================================================*/
//Δf TIME 値チェック
/*============================================================================*/
void CTrsControlXX54::OnEnChangeTrxxctrlDeltafTimeEdit()
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

/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『HOLD-TONE』-『OFF』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlHoldOffBtn()
{
	// ボタンを制御中にする
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("X-CMDOUT.HOLD_TONE"), _T("OFF"));

	// カウントダウン開始
	m_l_hold_tone_off_cntdwn = HOLD_TONE_TIME_XX_LIMIT;

	// 更新
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# 『TX CONTROL』-『HOLD-TONE』-『ON』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlHoldOnBtn()
{
	// ボタンを制御中にする
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// 制御通知送信
	theApp.GetSatelliteData().opctproc(_T("X-CMDOUT.HOLD_TONE"), _T("ON"));

	// カウントダウン開始
	m_l_hold_tone_on_cntdwn = HOLD_TONE_TIME_XX_LIMIT;

	// 更新
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}
