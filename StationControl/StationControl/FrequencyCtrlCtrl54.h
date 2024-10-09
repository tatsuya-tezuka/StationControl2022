#pragma once

#include "FrequencyCtrlCtrl.h"
#include "afxwin.h"
#include "NumericEdit.h"

// タイマ定義
#define TID_DELTAF_FREQ		8
#define INTERVAL_DELTAF 	800 	//800ms周期

// CFrequencyCtrlCtrl54 ダイアログ

// D/C COHモード 表示用
static const CString mDcCOHMode[] = {
	_T("X-COH(880/749)"),
	_T("X-INCOH"),
	_T("Ka-COH(3328/749)"),
	_T("Ka-COH(3344/749)"),
	_T("Ka-COH(3360/749)"),
	_T("Ka-INCOH"),
};
// D/C COHモード 制御名
static const CString mDcCOHModeCtl[] = {
	_T("X-COH_880/749"),
	_T("X-INCOH"),
	_T("Ka-COH_3328/749"),
	_T("Ka-COH_3344/749"),
	_T("Ka-COH_3360/749"),
	_T("Ka-INCOH"),
};

class CFrequencyCtrlCtrl54 : public CFrequencyCtrlCtrl
{
	DECLARE_DYNAMIC(CFrequencyCtrlCtrl54)

public:
	CFrequencyCtrlCtrl54(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CFrequencyCtrlCtrl54();

// ダイアログ データ
	enum { IDD = IDD_DLG_FREQUENCYCTRLCTRL54 };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
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
	} stGetMonData;

	typedef struct{
		const CString		ctlname1;		// 制御名
		const CString		ctlname2;		// 制御名
		const CString		ctlname3;		// 制御名
		const CString		ctlname4;		// 制御名
		const CString		title;			// タイトル
		UINT				control;		// コントロールID
		UINT				type;			// 種別：0:COMBO 1:EDIT 2:NONE
	} stSetCtlData54;

	enum{
		eControl_COH,
		eControl_DC_COH,
		eControl_UC,
		eControl_X_DC,
		eControl_Ka_DC,
		eControl_CS,
		eControl_TIMELOCK,
		eControl_TDP,
		eControl_RDP,
		eControl_CDP,
		eControl_TOFFSET,
		eControl_ROFFSET,
		eControl_TIMEOFFSET,
		eControl_ALLSET,
	};

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString m_DeltafCtrlFreqMoni;       //Δf（モニタ）
	BOOL m_DeltafCtrlFreqCtrChkBtn;     //Δf（制御）チェックボックス
	CButton m_DeltafCtrlChkBtnCtrl;
	CNumericEdit m_strDeltafCtrl;       //Δf（制御）
	CButton m_DeltafCtrlFreqChkBtn;     //制御周波数チェックボックス
	CNumericEdit m_strDeltafFreq;       //制御周波数
	CButton m_DeltafCtrlTimeChkBtn;     //制御時間チェックボックス
	CEdit m_strDeltafTime;              //制御時間
	CString m_strPreDeltafTime;			//前回の制御時間
	CButton m_DeltafStartBtn;           //開始
	CButton m_DeltafStopBtn;            //停止
	CString m_DeltafCtrlFreqEndTime;    //終了予定時刻
	BOOL m_RecoveryCont;				//リカバリー中フラグ
	BOOL m_RngFlg;						//レンジ計測中フラグ
	CEdit m_strXDC;
	CEdit m_strKaDC;
	CString m_strRecoverCont;
	CString m_strRngFlg;
	CComboBox m_DcCOHModeCtrl;
	BOOL m_strDeltafCtrlInit;			//Δf（制御）初期値設定フラグ(TRUE:初期値設定済)

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	virtual BOOL UpdateDlg();

protected:
	void GetData();
	void SetData(int type);
	bool SetData(int type, int phase);
	BOOL CheckTimeSec(CString second);
	BOOL CheckFrequencyRate(CString freq);
	BOOL CheckFrequencyInput(CString freq);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedFreqCtrlStartBtn();
	afx_msg void OnBnClickedFreqCtrlStopBtn();
	afx_msg void OnBnClickedDeltafCtrlFreqCtrChk();
	afx_msg void OnBnClickedDeltafCtrlFreqChk();
	afx_msg void OnBnClickedDeltafCtrlTimeChk();
	BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedFreqCtrlCtrlSetUcBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlSetCmdBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlOfsTfBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlOfsToBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlDpTdBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlDpRdBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlDpCmdBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlScmBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlDcCOHBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlSetCmdTimeBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlOfsTfBtn2();
	afx_msg void OnCbnSelchangeFreqCtrlDcCohCmb();
	afx_msg void OnEnChangeFreqCtrlCtrlDeltafCtrltimeEdit();
	afx_msg void OnEnChangeFreqCtrlCtrlOfsToEdit();
};

