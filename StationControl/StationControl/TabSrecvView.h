#pragma once
#include "afxwin.h"
#include "afxmaskededit.h"


static const stControlParam mSrecvParam[] = {
	{ IDC_BUTTON_REJECT,		_T(""),						 		_T("SS-TCRDEM.CARR_REJECT"), 		eControlCommand },		// REJECT
	{ IDC_BUTTON_PLLBW, 		_T("SS-TCRDEM.CARR_LOOP"), 			_T("SS-TCRDEM.CARR_LOOP"), 			eControlStatus },		// PLL BW
	{ IDC_BUTTON_AGCBW, 		_T("SS-TCRDEM.AGC_LOOP"),			_T("SS-TCRDEM.AGC_LOOP"),			eControlStatus },		// AGC BW
	{ IDC_BUTTON_BITRATE,		_T("SS-TCRDEM.BIT_RATE"),			_T("BITRATE-S"),					eControlDouble },		// BIT RATE
	{ IDC_BUTTON_DCCTRL, 		_T("SS-DCSYN1.SET_FREQ"),			_T("SS-DCSYN1.SET_FREQ"),			eControlDouble },		// D/C CTRL
	{ IDC_BUTTON_RX, 			_T("SS-DCSYN1.FREQ_MON"), 			_T("SS-DCSYN1.SET_FREQ"), 			eControlDouble },		// RX
	{ IDC_BUTTON_DC, 			_T("SS-DCSYN1.FREQ_MON"), 			_T("SS-DCSYN1.SET_FREQ"), 			eControlDouble },		// D/C
	{ IDC_STATIC_CARRVAL, 		_T("SS-TCRDEM.CARR_LOCK_ACH"), 		_T(""), 							eControlStatus },		// CARR
	{ IDC_STATIC_SUBCARRVAL, 	_T("SS-TCRDEM.TLM_LOCK_SUB_CARR"),	_T(""),								eControlStatus },		// SUB CARR
	{ IDC_STATIC_VITERBIVAL,	_T("SS-TCRDEM.TLM_LOCK_VITERBI"), 	_T(""), 							eControlStatus },		// VITERBI
	{ IDC_STATIC_BITVAL, 		_T("SS-TCRDEM.TLM_LOCK_BIT"), 		_T(""), 							eControlStatus },		// BIT
	{ IDC_STATIC_FRAMEVAL, 		_T("SS-TCRDEM.FRAME_LOCK_DSP"), 	_T(""), 							eControlStatus },		// FRAME
	{ IDC_MFCMASKEDEDIT_AGC, 	_T("SS-TCRAGC.AGC_A_AVG"), 			_T(""), 							eControlDouble },		// AGC
};


// CTabSrecvView ダイアログ

class CTabSrecvView : public CDialogEx
{
	DECLARE_DYNCREATE(CTabSrecvView)

public:
	CTabSrecvView(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabSrecvView();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_TAB_SRECV };

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
	afx_msg void OnBnClickedButtonReject();
	afx_msg void OnBnClickedButtonPllbw();
	afx_msg void OnBnClickedButtonAgcbw();
	afx_msg void OnBnClickedButtonBitrate();
	afx_msg void OnBnClickedButtonDcctrl();
	afx_msg void OnBnClickedButtonRx();
	afx_msg void OnBnClickedButtonDc();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
	CColorStatic mAgcVal;
};
