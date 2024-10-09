#pragma once


static const stControlParam mXrecv20Param[] = {
	{ IDC_BUTTON_REJECT, 		_T(""), 							_T("SX-TCRDEM.CARR_REJECT"), 		eControlCommand },	// REJECT
	{ IDC_BUTTON_PLLBW, 		_T("SX-TCRDEM.CARR_LOOP"), 			_T("SX-TCRDEM.CARR_LOOP"), 			eControlStatus },	// PLL BW
	{ IDC_BUTTON_AGCBW, 		_T("SX-TCRDEM.AGC_LOOP"), 			_T("SX-TCRDEM.AGC_LOOP"), 			eControlStatus },	// AGC BW
	{ IDC_BUTTON_BITRATE,		_T("SX-TCRDEM.BIT_RATE"),			_T("BITRATE-X"),					eControlDouble },	// BITRATE
	{ IDC_BUTTON_DCCTRL,		_T("XS-DCSYN.X_FREQ_MON"), 			_T("XS-DCSYN.SET_FREQ"), 			eControlDouble },	// D/C CTRL
	{ IDC_BUTTON_RX, 			_T("XS-DCSYN.X_FREQ_MON"), 			_T("XS-DCSYN.SET_FREQ"), 			eControlDouble },	// RX
	{ IDC_BUTTON_DC, 			_T("XS-DCSYN.X_FREQ_MON"), 			_T("XS-DCSYN.SET_FREQ"), 			eControlDouble },	// D/C
	{ IDC_STATIC_CARRVAL, 		_T("SX-TCRDEM.CARR_LOCK_ACH"), 		_T(""),						 		eControlStatus },	// CARR-A
	{ IDC_STATIC_CARRVAL2, 		_T("SX-TCRDEM.CARR_LOCK_BCH"), 		_T(""), 							eControlStatus },	// CARR-B
	{ IDC_STATIC_SUBCARRVAL, 	_T("SX-TCRDEM.TLM_LOCK_SUB_CARR"), 	_T(""),							 	eControlStatus },	// SUB CARR
	{ IDC_STATIC_VITERBIVAL, 	_T("SX-TCRDEM.TLM_LOCK_VITERBI"), 	_T(""), 							eControlStatus },	// VITERBI
	{ IDC_STATIC_BITVAL, 		_T("SX-TCRDEM.TLM_LOCK_BIT"), 		_T(""), 							eControlStatus },	// BIT
	{ IDC_STATIC_FRAMEVAL, 		_T("X-TLMDEC.FRM_LOCK_S"), 			_T(""), 							eControlStatus },	// FRAME
	{ IDC_MFCMASKEDEDIT_AGC, 	_T("SX-TCRAGC.AGC_A_AVG"), 			_T(""), 							eControlDouble },	// AGC-A
	{ IDC_MFCMASKEDEDIT_AGC2, 	_T("SX-TCRAGC.AGC_B_AVG"), 			_T(""), 							eControlDouble },	// AGC-B
};

// CTabXrecvView20 ダイアログ

class CTabXrecvView20 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabXrecvView20)

public:
	CTabXrecvView20(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabXrecvView20();

	// ダイアログ データ
	enum { IDD = IDD_TAB_XRECV20 };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

	void	ControlParamExecute(UINT pos);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

protected:
	void	UpdateStatus();
	void	InitStatus();

public:
	virtual BOOL OnInitDialog();
	CColorStatic mCarrVal;
	CColorStatic mCarrVal2;
	CColorStatic mSubCarrVal;
	CColorStatic mViterbiVal;
	CColorStatic mBitVal;
	CColorStatic mFrameVal;
	CColorStatic mAgcVal;
	CColorStatic mAgcVal2;
	afx_msg void OnBnClickedButtonReject();
	afx_msg void OnBnClickedButtonPllbw();
	afx_msg void OnBnClickedButtonAgcbw();
	afx_msg void OnBnClickedButtonBitrate();
	afx_msg void OnBnClickedButtonDcctrl();
	afx_msg void OnBnClickedButtonRx();
	afx_msg void OnBnClickedButtonDc();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};
