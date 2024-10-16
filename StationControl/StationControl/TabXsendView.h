#pragma once
#include "afxwin.h"

static const stControlParam mXsendParam[] = {
	{ IDC_BUTTON_RF,			_T("X-UL_CNT.RF_POWER_SEL"), 		_T("X-UL_CNT.RF_PWR_200/2k/20kW"), 	eControlStatus },		// 送信電力
	{ IDC_BUTTON_CMDSC, 		_T("DSNTCR.TX_SC_FREQ"), 			_T("DSNTCR.TX_SC_FREQ"), 			eControlDouble },		// CMD SC
	{ IDC_BUTTON_MOD, 			_T("DSNTCR.TX_IF_MODULATION"), 		_T("DSNTCR.TX_IF_MODULATION"), 		eControlStatus },		// MOD
	{ IDC_BUTTON_INDEXCMD, 		_T("DSNTCR.TX_MOD_INDEX"), 			_T("DSNTCR.TX_MOD_INDEX"), 			eControlDouble },		// INDEX CMD
	{ IDC_BUTTON_INDEXCMD2,	 	_T("DSNTCR.RNG_MOD_INDEX"), 		_T("DSNTCR.RNG_MOD_INDEX"), 		eControlDouble },		// INDEX RNG
	{ IDC_BUTTON_UCCTRL, 		_T("X-UC.FREQUENCY"), 				_T("X-UC.FREQUENCY"), 				eControlDouble },		// U/C CTRL
	{ IDC_BUTTON_TX, 			_T("X-UC.FREQUENCY"), 				_T("X-UC.FREQUENCY"), 				eControlDouble },		// TX
	{ IDC_BUTTON_UC, 			_T("X-UC.FREQUENCY"), 				_T("X-UC.FREQUENCY"), 				eControlDouble },		// U/C
	{ IDC_BUTTON_SWEEP, 		_T("DSNTCR.TX_SC_SWP_MODE"), 		_T("DSNTCR.TX_SC_SWP_MODE"), 		eControlStatus },		// SWEEP
	{ IDC_BUTTON_HOLDTONE, 		_T("X-CMDOUT2.HOLDTONE_STAT"), 		_T("X-CMDOUT2.HOLD_TONE"), 			eControlStatus },		// HOLD TONE
	{ IDC_BUTTON_BITRATE,		_T("DSNTCR.TX_BIT_RATE"),			_T("CMD4-BITRATE"),					eControlDouble },		// BIT RATE
	{ IDC_BUTTON_XCMDOUT, 		_T("X-CMDOUT2.LOCATION"), 			_T("X-CMDOUT2.DSNMODEM_SWITCH"), 	eControlStatus },		// X-CMDOUT2
	{ IDC_STATIC_RFVAL, 		_T("X-UL_CNT.RF_ON_CONT"), 			_T(""),				 				eControlStatus },		// RF ON
	{ IDC_STATIC_RFDUMMY, 		_T("X-UL_CNT.ANT/DUMMY_SELECT"),	_T(""),				 				eControlStatus },		// ANT/DUMMY
	{ IDC_STATIC_RNGVAL, 		_T("DSNTCR.RNG_CONT"), 				_T(""), 							eControlStatus },		// RNG
};

// CTabXsendView フォーム ビュー

class CTabXsendView : public CDialogEx
{
	DECLARE_DYNCREATE(CTabXsendView)

public:
	CTabXsendView(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabXsendView();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum { IDD = IDD_TAB_XSEND };

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
	afx_msg void OnBnClickedButtonRf();
	afx_msg void OnBnClickedButtonCmdsc();
	afx_msg void OnBnClickedButtonMod();
	afx_msg void OnBnClickedButtonIndexcmd();
	afx_msg void OnBnClickedButtonIndexcmd2();
	afx_msg void OnBnClickedButtonUcctrl();
	afx_msg void OnBnClickedButtonTx();
	afx_msg void OnBnClickedButtonUc();
	afx_msg void OnBnClickedButtonScoffset();
	afx_msg void OnBnClickedButtonSweep();
	afx_msg void OnBnClickedButtonHoldtone();
	afx_msg void OnBnClickedButtonBitrate();
	afx_msg void OnBnClickedButtonXcmdout();
	CColorStatic mRngVal;
	CColorStatic mRfVal;
	CColorStatic mDummyVal;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};


