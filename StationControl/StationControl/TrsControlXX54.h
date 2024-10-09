#pragma once

#include "TrsControlXX.h"
#include "afxwin.h"
#include "NumericEdit.h"

// タイマ定義
#define TID_DELTAF_TRS		7
#define INTERVAL_DELTAF 	800 	//800ms周期

// CTrsControlXX54 ダイアログ

class CTrsControlXX54 : public CTrsControlXX
{
	DECLARE_DYNAMIC(CTrsControlXX54)

public:
	CTrsControlXX54(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTrsControlXX54();

// ダイアログ データ
	enum { IDD = IDD_DLG_TRSCTRL_XX54 };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
public:
	enum{
		eINT,
		eDOUBLE,
		eSTRING,
	};
	typedef struct{
		const CString		monname;		// 監視名
		CString&			controlval;		// コントロール値
		const CString		format;			// フォーマット
		UINT				type;			// 種別：0:INT 1:FLOAT 2:DOUBLE 3:STRING
	} stSetMonData;

protected:

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
	CGroupBoxEx m_grDeltafRecovery;
	CGroupBoxEx m_grDelfafTimeRate;

protected:
	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	virtual BOOL UpdateDlg();

protected:
	void SetData();
	BOOL CheckTimeSec(CString second);
	BOOL CheckFrequencyRate(CString freq);
	BOOL CheckFrequencyInput(CString freq);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

public:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	/* ------------------------------------------------------------------------------------ */
	CString m_DeltafCtrlFreqMoni;       //Δf（MON）
	BOOL m_DeltafCtrlFreqCtrChkBtn;     //Δf（CTRL）チェックボックス
	CButton m_DeltafCtrlChkBtnCtrl;
	CNumericEdit m_strDeltafCtrl;       //Δf（CTRL）
	CButton m_DeltafCtrlTimeChkBtn;     //制御時間チェックボックス
	CEdit m_strDeltafTime;              //制御時間
	CString m_strPreDeltafTime;			//前回の制御時間
	CButton m_DeltafCtrlFreqChkBtn;     //制御周波数チェックボックス
	CNumericEdit m_strDeltafFreq;       //制御周波数
	CGroupBoxEx m_grRecovery;
	CColorButtonEx m_RecoveryOffBtn;    //OFF
	CColorButtonEx m_RecoveryOnBtn;     //ON
	CString m_RecoveryOffOn;
	CString m_strRngFlg;
	CString m_DeltafCtrlFreqEndTime;    //終了予定時刻
	BOOL m_RecoveryCont;				//リカバリー中フラグ
	BOOL m_RngFlg;						//レンジ計測中フラグ
	BOOL m_strDeltafCtrlInit;			//Δf（CTRL）初期値設定フラグ(TRUE:初期値設定済)

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedTrxxctrlRfOffBtn();
	afx_msg void OnBnClickedTrxxctrlRfOnBtn();
	afx_msg void OnBnClickedTrxxctrlNouseOffBtn();
	afx_msg void OnBnClickedTrxxctrlNouseOnBtn();
	afx_msg void OnBnClickedTrxxctrlSweetStopBtn();
	afx_msg void OnBnClickedTrxxctrlSweetIncBtn();
	afx_msg void OnBnClickedTrxxctrlSweetIncdecBtn();
	afx_msg void OnBnClickedTrxxctrlSweetDecBtn();
	afx_msg void OnBnClickedTrxxctrlSweetDecincBtn();
	afx_msg void OnBnClickedTrxxctrlSweetSubStopBtn();
	afx_msg void OnBnClickedTrxxctrlSweetSubIncBtn();
	afx_msg void OnBnClickedTrxxctrlSweetSubIncdecBtn();
	afx_msg void OnBnClickedTrxxctrlSweetSubDecBtn();
	afx_msg void OnBnClickedTrxxctrlSweetSubDecincBtn();
	afx_msg void OnBnClickedTrxxctrlXxcohCohBtn();
	afx_msg void OnBnClickedTrxxctrlXxcohIncohBtn();
	afx_msg void OnBnClickedTrxxctrlRngingStopBtn();
	afx_msg void OnBnClickedTrxxctrlRngingStartBtn();
	afx_msg void OnBnClickedTrxxctrlRgntypeStopBtn();
	afx_msg void OnBnClickedTrxxctrlRgntypeStartBtn();
	afx_msg void OnBnClickedStaticDeltafRecoveryBox();
	afx_msg void OnBnClickedRecoveryOffBtn();
	afx_msg void OnBnClickedRecoveryOnBtn();
	afx_msg void OnBnClickedDeltafCtrlFreqCtrChk();
	afx_msg void OnBnClickedDeltafCtrlTimeChk();
	afx_msg void OnBnClickedDeltafCtrlFreqChk();
	afx_msg void OnEnChangeTrxxctrlDeltafTimeEdit();
	afx_msg void OnBnClickedTrxxctrlHoldOffBtn();
	afx_msg void OnBnClickedTrxxctrlHoldOnBtn();
};
