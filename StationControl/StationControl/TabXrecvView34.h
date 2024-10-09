#pragma once


static const stControlParam mXrecv34Param[] = {
	{ IDC_BUTTON_REJECT,		_T(""), 							_T("DSNTCR.CARRIER_REJECT"), 		eControlCommand },	// REJECT
	{ IDC_BUTTON_PLLBW, 		_T("DSNTCR.CARR_LOOP_BAND"), 		_T("DSNTCR.CARR_LOOP_BAND"), 		eControlStatus },	// PLL BW
	{ IDC_BUTTON_AGCBW, 		_T("DSNTCR.AGC_LOOP_BAND"), 		_T("DSNTCR.AGC_LOOP_BAND"), 		eControlStatus },	// AGC BW
	{ IDC_BUTTON_BITRATE,		_T("DSNTCR.BIT_RATE_MON"),			_T("BITRATE-X2"),					eControlDouble },	// BITRATE
	{ IDC_BUTTON_DCCTRL, 		_T("S70-DC.SET_FREQ"), 				_T("S70-DC.SET_FREQ"), 				eControlDouble },	// D/C CTRL
	{ IDC_BUTTON_RX, 			_T("S70-DC.SET_FREQ"), 				_T("S70-DC.SET_FREQ"), 				eControlDouble },	// RX
	{ IDC_BUTTON_DC, 			_T("S70-DC.SET_FREQ"), 				_T("S70-DC.SET_FREQ"), 				eControlDouble },	// D/C
	{ IDC_STATIC_EB, 			_T("DSNTCR.Eb/N0"), 				_T(""), 							eControlDouble },	// Eb/N0
	{ IDC_STATIC_CARRVAL, 		_T("DSNTCR.CARRIER_LOCK_ANS"), 		_T(""), 							eControlStatus },	// CARR
	{ IDC_STATIC_SUBCARRVAL, 	_T("DSNTCR.SUB_CARRIER_LOCK_STS"), 	_T(""), 							eControlStatus },	// SUB CARR
	{ IDC_STATIC_VITERBIVAL, 	_T("DSNTCR.VITERBI_LOCK_STS"), 		_T(""), 							eControlStatus },	// VITERBI
	{ IDC_STATIC_BITVAL, 		_T("DSNTCR.BIT_LOCK_STS"),			_T(""),								eControlStatus },	// BIT
	{ IDC_STATIC_FRAMEVAL, 		_T("DSNTCR.FRAME_LOCK_STS"), 		_T(""), 							eControlStatus },	// FRAME
	{ IDC_MFCMASKEDEDIT_AGC, 	_T("DSNTCR_AGAV.AGC_A_AVG"), 		_T(""), 							eControlDouble },	// AGC
};

// CTabXrecvView34 ダイアログ

class CTabXrecvView34 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabXrecvView34)

public:
	CTabXrecvView34(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabXrecvView34();

	// ダイアログ データ
	enum { IDD = IDD_TAB_XRECV34 };

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
	CColorStatic mSubCarrVal;
	CColorStatic mViterbiVal;
	CColorStatic mBitVal;
	CColorStatic mFrameVal;
	CColorStatic mEbVal;
	CColorStatic mAgcVal;
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
