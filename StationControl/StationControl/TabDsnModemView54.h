#pragma once
#include "afxwin.h"
#include "afxmaskededit.h"

#ifdef _DEBUG_KANSHI64M
// 64m監視
static const stControlParam mDsnModemParam54[] = {
	{ IDC_BUTTON_REJECT, _T(""), _T("DSNTCR.CARRIER_REJECT"), eControlCommand },		// REJECT■
	{ IDC_BUTTON_PLLBW, _T("DSNTCR.CARR_LOOP_BAND"), _T("DSNTCR.CARR_LOOP_BAND"), eControlStatus },		// PLL  BW■
	{ IDC_BUTTON_AGCBW, _T("DSNTCR.AGC_LOOP_BAND"), _T("DSNTCR.AGC_LOOP_BAND"), eControlStatus },		// AGC BW■
	{ IDC_BUTTON_BITRATE, _T("DSNTCR.RX_BIT_RATE"), _T("DSNTCR.RX_BIT_RATE"), eControlDouble },		// BIT RATE■
	{ IDC_BUTTON_RXROUTE, _T("DSNTCR.TX_SC_DOPP_PRE"), _T("DSNTCR.TX_SC_DOPP_PRE"), eControlStatus },		// RX ROUTE■
	{ IDC_BUTTON_CMDSC, _T("DSNTCR.TX_SC_DOPP_PRE"), _T("DSNTCR.TX_SC_DOPP_PRE"), eControlStatus },		// CMD SC■
	{ IDC_BUTTON_TLMSC, _T("DSNTCR.RX_CARR_DOPP_PRE"), _T("DSNTCR.RX_CARR_DOPP_PRE_STS"), eControlStatus },		// TLM SC■
	{ IDC_BUTTON_XCMDOUT, _T("X-CMDOUT2.LOCATION"), _T("X-CMDOUT2.DSNMODEM_SWITCH"), eControlStatus },		// X-CMDOUT2■
	{ IDC_BUTTON_XTLMIN, _T("X-TLMIN2.LOCATION"), _T("X-TLMIN2.DSNMODEM_SWITCH"), eControlStatus },		// X-TLMIN2■
	{ IDC_STATIC_EB, _T("DSNTCR.Eb/N0"), _T(""), eControlDouble },		// Eb/N0■
	{ IDC_STATIC_CARRVAL, _T("DSNTCR.CARRIER_LOCK"), _T(""), eControlStatus },		// CARR■
	{ IDC_STATIC_SUBCARRVAL, _T("DSNTCR.SUB_CARRIER_LOCK"), _T(""), eControlStatus },		// SUB CARR■
	{ IDC_STATIC_VITERBIVAL, _T("DSNTCR.VITERBI_LOCK"), _T(""), eControlStatus },		// VITERBI■
	{ IDC_STATIC_BITVAL, _T("DSNTCR.BIT_LOCK"), _T(""), eControlStatus },		// BIT■
	{ IDC_STATIC_FRAMEVAL, _T("DSNTCR.FRAME_LOCK"), _T(""), eControlStatus },		// FRAME■
	{ IDC_STATIC_DOPPLER, _T("CTRL.PRED_TIMETAG_REQ"), _T(""), eControlTime },			// DOPPLER□																		// DOPPLER
	{ IDC_STATIC_AGC, _T("DSNTCR.INPUT_LEVEL"), _T(""), eControlDouble },		// AGC■
	{ IDC_STATIC_RTT, _T("DSNTCR.RX_BIT_RATE"), _T(""), eControlDouble },		// TIME□
//	{ IDC_STATIC_TXFREQ, _T("DSNTCR.RX_BIT_RATE"), _T(""), eControlDouble },		// TX FREQ□
//	{ IDC_STATIC_RXFREQ, _T("DSNTCR.RX_BIT_RATE"), _T(""), eControlDouble },		// RX FREQ□
	{ IDC_STATIC_TXFREQ, _T("CTRL.DOPP_UL0"), _T(""), eControlDouble },		// TX FREQ□
	{ IDC_STATIC_RXFREQ, _T("CTRL.DOPP_DL0"), _T(""), eControlDouble },		// RX FREQ□
};
#else
// 54m監視
static const stControlParam mDsnModemParam54[] = {
	{ IDC_BUTTON_REJECT, _T(""), _T("DSNTCR.CARRIER_REJECT"), eControlCommand },		// REJECT■
	{ IDC_BUTTON_PLLBW, _T("DSNTCR.CARR_LOOP_BAND"), _T("DSNTCR.CARR_LOOP_BAND"), eControlStatus },		// PLL  BW■
	{ IDC_BUTTON_AGCBW, _T("DSNTCR.AGC_LOOP_BAND"), _T("DSNTCR.AGC_LOOP_BAND"), eControlStatus },		// AGC BW■
	{ IDC_BUTTON_BITRATE, _T("DSNTCR.RX_BIT_RATE"), _T("BITRATE-X2"), eControlDouble },		// BIT RATE X■
	{ IDC_BUTTON_RXROUTE, _T("X-DC.ROUTE_SELECT"), _T("X-DC.ROUTE_SELECT"), eControlStatus },		// RX ROUTE■
	{ IDC_BUTTON_CMDSC, _T("DSNTCR.TX_SC_DOPP_PRE"), _T("DSNTCR.TX_SC_DOPP_PRE"), eControlStatus },		// CMD SC■
	{ IDC_BUTTON_TLMSC, _T("DSNTCR.TLM_AID"), _T("DSNTCR.TLM_AID"), eControlStatus },		// TLM SC■
	{ IDC_BUTTON_XCMDOUT, _T("X-CMDOUT2.LOCATION"), _T("X-CMDOUT2.DSNMODEM_SWITCH"), eControlStatus },		// X-CMDOUT2■
	{ IDC_BUTTON_XTLMIN, _T("X-TLMIN2.LOCATION"), _T("X-TLMIN2.DSNMODEM_SWITCH"), eControlStatus },		// X-TLMIN2■
	{ IDC_BUTTON_BITRATE_KA, _T("DSNTCR.RX_BIT_RATE"), _T("BITRATE-Ka"), eControlDouble },		// BIT RATE Ka■
	{ IDC_STATIC_EB, _T("DSNTCR.Eb/N0"), _T(""), eControlDouble },		// Eb/N0■
	{ IDC_STATIC_CARRVAL, _T("DSNTCR.CARRIER_LOCK"), _T(""), eControlStatus },		// CARR■
	{ IDC_STATIC_SUBCARRVAL, _T("DSNTCR.SUB_CARRIER_LOCK"), _T(""), eControlStatus },		// SUB CARR■
	{ IDC_STATIC_VITERBIVAL, _T("DSNTCR.VITERBI_LOCK"), _T(""), eControlStatus },		// VITERBI■
	{ IDC_STATIC_BITVAL, _T("DSNTCR.BIT_LOCK"), _T(""), eControlStatus },		// BIT■
	{ IDC_STATIC_FRAMEVAL, _T("DSNTCR.FRAME_LOCK"), _T(""), eControlStatus },		// FRAME■
	{ IDC_STATIC_DOPPLER, _T("CTRL.PRED_TIMETAG_REQ"), _T(""), eControlTime },			// DOPPLER■																		// DOPPLER
	{ IDC_STATIC_AGC, _T("DSNTCR_AGSH.AGC_A_AVG"), _T(""), eControlDouble },		// AGC■
	{ IDC_STATIC_RTT, _T("CTRL.RNG_RTT"), _T(""), eControlDouble },		// RTT■
	{ IDC_STATIC_TXFREQ, _T("CTRL.DOPP_UL0"), _T(""), eControlDouble },		// TX FREQ■
	{ IDC_STATIC_RXFREQ, _T("CTRL.DOPP_DL0"), _T(""), eControlDouble },		// RX FREQ■
};
#endif

// CTabDsnModemView54 ダイアログ

class CTabDsnModemView54 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabDsnModemView54)

public:
	CTabDsnModemView54(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabDsnModemView54();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_TAB_DSNMODEM54 };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CColorStatic mCarrVal;
	CColorStatic mSubCarrVal;
	CColorStatic mViterbiVal;
	CColorStatic mBitVal;
	CColorStatic mFrameVal;
	CColorStatic mAgcVal;
	CColorStatic mEbVal;
	CColorStatic mTimeVal;
	CColorStatic mTxFreqVal;
	CColorStatic mRxFreqVal;

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

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonPllbw();
	afx_msg void OnBnClickedButtonAgcbw();
	afx_msg void OnBnClickedButtonBitrate();
	afx_msg void OnBnClickedButtonRxroute();
	afx_msg void OnBnClickedButtonCmdsc();
	afx_msg void OnBnClickedButtonTlmsc();
	afx_msg void OnBnClickedButtonReject();
	afx_msg void OnBnClickedButtonXcmdout();
	afx_msg void OnBnClickedButtonXtlmin();
	/* ------------------------------------------------------------------------------------ */
	afx_msg void OnBnClickedButtonBitrateKa();
};
