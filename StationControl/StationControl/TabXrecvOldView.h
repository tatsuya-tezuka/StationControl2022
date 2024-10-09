#pragma once

static const stControlParam mXrecvOldParam[] = {
	{ IDC_BUTTON_REJECT, 		_T(""), 							_T("X-RX2.LOCK_ON_REJECT"), 		eControlCommand },		// REJECT
	{ IDC_BUTTON_PLLBW, 		_T("X-RX2.PLL_BW"), 				_T("X-RX2.PLL_BW"), 				eControlStatus },		// PLL  BW
	{ IDC_BUTTON_AGCBW, 		_T("X-RX2.AGC_BW"), 				_T("X-RX2.AGC_BW"), 				eControlStatus },		// AGC BW
	{ IDC_BUTTON_BITRATE,		_T("X-PKTDEM.BIT_RATE"),			_T("BITRATE-X"),					eControlDouble },		// BIT RATE
	{ IDC_BUTTON_DCCTRL, 		_T("X-RX2SYN.STT_FREQ"), 			_T("X-RX2SYN.STT_FREQ"), 			eControlDouble },		// D/C CTRL
	{ IDC_BUTTON_RX, 			_T("X-RX2SYN.FREQ"), 				_T("X-RX2SYN.STT_FREQ"), 			eControlDouble },		// RX
	{ IDC_BUTTON_DC, 			_T("X-RX2SYN.STT_FREQ"), 			_T("X-RX2SYN.STT_FREQ"), 			eControlDouble },		// D/C
	{ IDC_STATIC_CARRVAL, 		_T("X-RX2.PM_LOCK_ACH"), 			_T(""), 							eControlStatus },		// CARR
	{ IDC_STATIC_SUBCARRVAL, 	_T("X-RX2.DEM_LOCK_SUB_CARR_S"), 	_T(""),								eControlStatus },		// SUB CARR
	{ IDC_STATIC_VITERBIVAL, 	_T("X-RX2.DEM_LOCK_VITERBI_S"),	 	_T(""),								eControlStatus },		// VITERBI
	{ IDC_STATIC_BITVAL, 		_T("X-RX2.DEM_LOCK_SYMBOL_S"), 		_T(""), 							eControlStatus },		// BIT
	{ IDC_STATIC_FRAMEVAL, 		_T("X-PKTDEM.FRM_LOCK_S"), 			_T(""), 							eControlStatus },		// FRAME
	{ IDC_MFCMASKEDEDIT_AGC, 	_T("X-RX2AGC.AGC_A_AVG"), 			_T(""), 							eControlDouble },		// AGC
};																																
																																
// CTabXrecvOldView ダイアログ

class CTabXrecvOldView : public CDialogEx
{
	DECLARE_DYNCREATE(CTabXrecvOldView)

public:
	CTabXrecvOldView(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabXrecvOldView();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_TAB_XRECVOLD };

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
