#pragma once
#include "afxwin.h"

static const stControlParam mXsendParam54[] = {
	{ IDC_BUTTON_RF, _T("X-SSPA.TX_POWER_MODE"), _T("X-SSPA.TX_POWER_MODE"), eControlStatus },		// 送信電力■
	{ IDC_BUTTON_CMDSC, _T("DSNTCR.TX_SC_FREQ"), _T("DSNTCR.TX_SC_FREQ"), eControlDouble },		// CMD SC■
	{ IDC_BUTTON_RNGMOD, _T("DSNTCR.TX_IF_MODULATION"), _T("DSNTCR.TX_IF_MODULATION"), eControlStatus },		// RNG MOD■
	{ IDC_BUTTON_CMDMOD, _T("DSNTCR.TX_IF_MODULATION"), _T("DSNTCR.TX_IF_MODULATION"), eControlStatus },		// CMD MOD■
//	{ IDC_BUTTON_RNGCTRL, _T("DSNTCR.RNG_CONT"), _T("DSNTCR.RNG_CONT"), eControlStatus },		// RNG CTRL■
	{ IDC_STATIC_RNGVAL, _T("DSNTCR.RNG_CONT"), _T(""), eControlStatus },		// RNG CTRL■
	{ IDC_BUTTON_UCCTRL, _T("X-UC.FREQUENCY"), _T("X-UC.FREQUENCY"), eControlDouble },		// U/C CTRL■
	{ IDC_BUTTON_TX, _T("DSNTCR.UP_REF_FREQ"), _T("DSNTCR.UP_REF_FREQ"), eControlDouble },		// TX■
	{ IDC_BUTTON_SWEEP, _T("DSNTCR.TX_CARR_SWP_MODE"), _T("DSNTCR.TX_CARR_SWP_MODE"), eControlStatus },		// SWEEP■
	{ IDC_BUTTON_HOLDTONE, _T("X-CMDOUT.HOLDTONE_STAT"), _T("X-CMDOUT.HOLD_TONE"), eControlStatus },		// HOLD TONE■
	{ IDC_BUTTON_BITRATE, _T("DSNTCR.TX_BIT_RATE"), _T("CMD4-BITRATE"), eControlDouble },		// BIT RATE■
	{ IDC_STATIC_RFVAL, _T("X-SSPA.TX_ON/OFF"), _T(""), eControlStatus },		// RF ON■
	{ IDC_STATIC_RFDUMMY, _T("X-SSPA.ANT/DUMMY_POL"), _T(""), eControlStatus },		// ANT/DUMMY■
	{ IDC_BUTTON_SSPACTRL, _T("X-SSPA.TX_FREQUENCY"), _T("X-SSPA.TX_FREQUENCY"), eControlDouble }, // SSPA CTRL■
};

// CTabXsendView54 ダイアログ

class CTabXsendView54 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabXsendView54)

public:
	CTabXsendView54(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabXsendView54();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
// ダイアログ データ
	enum { IDD = IDD_TAB_XSEND54 };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CColorStatic mRngVal;
	CColorStatic mRfVal;
	CColorStatic mDummyVal;

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
	afx_msg void OnBnClickedButtonRf();
	afx_msg void OnBnClickedButtonCmdsc();
	afx_msg void OnBnClickedButtonRngmod();
	afx_msg void OnBnClickedButtonCmdmod();
	afx_msg void OnBnClickedButtonRngctrl();
	afx_msg void OnBnClickedButtonUcctrl();
	afx_msg void OnBnClickedButtonTx();
	afx_msg void OnBnClickedButtonSweep();
	afx_msg void OnBnClickedButtonHoldtone();
	afx_msg void OnBnClickedButtonBitrate();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
	afx_msg void OnBnClickedButtonSspactrl();
};
