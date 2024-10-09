#pragma once
#include "afxwin.h"

static const stControlParam mSSendParam[] = {
	{ IDC_BUTTON_RF, 		_T("S-TXRF.IF_RF_PWR"),			_T("S-TXRF.IF_RF_PWR"), eControlStatus },				// 送信電力
	{ IDC_BUTTON_CMDSC, 	_T("SS-TCRMOD.SUB_CARR"),		_T("SS-TCRMOD.SUB_CARR"), eControlDouble },				// CMD SC
	{ IDC_BUTTON_MOD, 		_T("SS-TCRMOD.MODULATION"),		_T("SS-TCRMOD.MODULATION"), eControlStatus },			// MOD
	{ IDC_BUTTON_INDEXCMD, 	_T("SS-TCRMOD.CMD_MOD_INDEX"),	_T("SS-TCRMOD.CMD_MOD_INDEX"), eControlDouble },		// INDEX CMD
	{ IDC_BUTTON_INDEXCMD2,	_T("SS-TCRMOD.RNG_MOD_INDEX"),	_T("SS-TCRMOD.RNG_MOD_INDEX"), eControlDouble },		// INDEX RNG
	{ IDC_BUTTON_UCCTRL, 	_T("S-UCSYN.SET_FREQ"),			_T("S-UCSYN.SET_FREQ"), eControlDouble },				// U/C CTRL
	{ IDC_BUTTON_TX, 		_T("S-UCSYN.SET_FREQ"),			_T("SS-TCRMOD.STT_FREQ_UP"), eControlDouble },			// TX
	{ IDC_BUTTON_UC, 		_T("S-UCSYN.SET_FREQ"),			_T("S-UCSYN.SET_FREQ"), eControlDouble },				// U/C
	{ IDC_BUTTON_SWEEP,		_T("S-UCSYN.SWEEP_CONT"), 		_T("S-UCSYN.SWEEP_CONT"), eControlStatus },				// SWEEP
	{ IDC_BUTTON_HOLDTONE,	_T("CMDOUT.HOLDTONE_STAT"),		_T("CMDOUT.HOLD_TONE"), eControlStatus },				// HOLD TONE
	{ IDC_BUTTON_BITRATE,	_T("SS-TCRMOD.BIT_RATE"),		_T("CMD-BITRATE"), eControlDouble },					// BIT RATE
	{ IDC_STATIC_RFVAL,		_T("S-TXRF.IF_RF_OUTPUT_CONT"),	_T(""), eControlStatus },								// RF ON
	{ IDC_STATIC_RFDUMMY,	_T("S-TXRF.IF_LOAD_SEL"), 		_T(""), eControlStatus },								// ANT/DUMMY
	{ IDC_STATIC_RNGVAL,	_T("SS-TCRDEM.RNG_CONT"), 		_T(""), eControlStatus },								// RNG
};


// CTabSsendView ダイアログ

class CTabSsendView : public CDialogEx
{
	DECLARE_DYNCREATE(CTabSsendView)

public:
	CTabSsendView(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabSsendView();


	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_TAB_SSEND };

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
	afx_msg void OnBnClickedButtonSweep();
	afx_msg void OnBnClickedButtonHoldtone();
	afx_msg void OnBnClickedButtonBitrate();
	CColorStatic mRngVal;
	CColorStatic mRfVal;
	CColorStatic mDummyVal;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};
