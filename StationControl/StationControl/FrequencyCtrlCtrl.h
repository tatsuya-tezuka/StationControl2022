#pragma once
#include "afxwin.h"


// CFrequencyCtrlCtrl ダイアログ

// 探査機モード
static const CString mSatMode[] = {
	_T("INCOH"),
	_T("COH-880/749"),
	_T("COH-3344/749"),
	_T("COH-3328/749"),
	_T("COH-3360/749"),
};
// 送信ドップラ
static const CString mTxDopp[] = {
	_T("動的補償"),
	_T("静的補償(センター)"),
	_T("静的補償(ノミナル)"),
	_T("静的補償(AOSベストロック)"),
};
// 受信ドップラ
static const CString mRxDopp[] = {
	_T("1WAY"),
	_T("2WAY"),
};
// コマンドキャリア
static const CString mCmdCarr[] = {
	_T("動的補償"),
	_T("静的補償(センター)"),
	_T("静的補償(ノミナル)"),
	_T("静的補償(MOD ONベストロック)"),
};
static const CString mCmdCtrl[] = {
	_T("ON"),
	_T("AVE"),
	_T("NOMINAL"),
	_T("STT"),
};

class CFrequencyCtrlCtrl : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrlCtrl)

public:
	CFrequencyCtrlCtrl(CWnd* pParent = NULL);   // 標準コンストラクター
	CFrequencyCtrlCtrl(UINT IDD, CWnd* pParent = NULL);
	virtual ~CFrequencyCtrlCtrl();

// ダイアログ データ
	enum { IDD = IDD_DLG_FREQ_CTRL_CTRL };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum{
		eControl_COH,
		eControl_UC,
		eControl_DC,
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
	enum{
		eCOMBO,
		eEDIT,
		eNONE,
	};
	enum{
		ePhase_chk,
		ePhase_send,
		ePhase_end,
	};
	typedef struct{
		const CString		ctlname1;		// 制御名
		const CString		ctlname2;		// 制御名
		UINT				control;		// コントロールID
		UINT				type;			// 種別：0:COMBO 1:EDIT 2:NONE
	} stSetCtlData;

protected:

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
protected:
	CWnd*		mCallWnd;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	void	SetCallWnd(CWnd* p)	{ mCallWnd = p; }
	virtual BOOL UpdateDlg();

protected:
	void SetData(int type);
	bool CheckCtrlValRange(CString& strCtrl, CString& strVal, CString& strTitle);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	// 設定周波数-U/C
	CString m_strUC;
	// 設定周波数-D/C
	CString m_strDC;
	// 設定周波数-コマンドキャリア
	CString m_strCmdCareer;
	// オフセット-送信周波数オフセット
	CString m_strTFreqOffset;
	// オフセット-時刻オフセット
	CString m_strTimeOffset;
	CString m_strTimeOffsetPre;
	// ドップラ補償-送信ドップラ
	CComboBox m_TDopplerCtrl;
	// ドップラ補償-受信ドップラ
	CComboBox m_RDopplerCtrl;
	// ドップラ補償-コマンドキャリア
	CComboBox m_DopplerCmdCareerCtrl;
	// 探査機COHモード
	CComboBox m_SpacecraftCOHModeCtrl;
	afx_msg void OnBnClickedFreqCtrlCtrlSetUcBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlSetCmdBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlOfsTfBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlOfsToBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlDpTdBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlDpRdBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlDpCmdBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlScmBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedFreqCtrlCtrlSetCmdTimeBtn();
	CString m_strRFreqOffset;
	afx_msg void OnBnClickedFreqCtrlCtrlOfsTfBtn2();
	afx_msg void OnEnChangeFreqCtrlCtrlOfsToEdit();
};
