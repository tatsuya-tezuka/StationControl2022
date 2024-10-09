#pragma once


static const stControlParam mSSend20Param[] = {
	{ IDC_BUTTON_RF, 		_T("S-TXRF.RF_POWER_SEL"), 			_T("CTRL.S_TX_RF_PWR"), 			eControlStatus },	// 送信電力
	{ IDC_BUTTON_MOD, 		_T("SS-TCRMOD.MODULATION"), 		_T("SS-TCRMOD.MODULATION"), 		eControlStatus },	// MOD
	{ IDC_BUTTON_INDEXCMD, 	_T("SS-TCRMOD.CMD_MOD_INDEX"), 		_T("SS-TCRMOD.CMD_MOD_INDEX"), 		eControlDouble },
	{ IDC_BUTTON_INDEXCMD2, _T("SS-TCRMOD.RNG_MOD_INDEX"), 		_T("SS-TCRMOD.RNG_MOD_INDEX"), 		eControlDouble },
	{ IDC_BUTTON_MODMODE, 	_T("SS-TCRMOD.MOD_FORMAT_MODE"),	_T("SS-TCRMOD.MOD_FORMAT_MODE"),	eControlStatus },
	{ IDC_BUTTON_UCCTRL, 	_T("S-UCSYN.FREQ_MON"), 			_T("S-UCSYN.SET_FREQ"), 			eControlDouble },	// U/C CTRL
	{ IDC_BUTTON_TX, 		_T("S-UCSYN.FREQ_MON"), 			_T("S-UCSYN.SET_FREQ"), 			eControlDouble },	// TX
	{ IDC_BUTTON_UC, 		_T("S-UCSYN.FREQ_MON"), 			_T("S-UCSYN.SET_FREQ"), 			eControlDouble },	// U/C
	{ IDC_BUTTON_SWEEP, 	_T("SS-TCRMOD.SWEEP_CMD_ANS"),		_T("SS-TCRMOD.SWEEP_CMD"),			eControlStatus },	// SWEEP
	{ IDC_BUTTON_HOLDTONE, 	_T("CMDOUT1.HOLDTONE_STAT"), 		_T("CMDOUT1.HOLD_TONE"), 			eControlStatus },	// HOLD TONE
	{ IDC_BUTTON_BITRATE,	_T("SS-TCRMOD.BIT_RATE"),			_T("CMD-BITRATE"),					eControlDouble },	// BIT RATE
	{ IDC_STATIC_RFVAL,		_T("S-TXRF.IF_RF_OUTPUT_CONT"),		_T(""), 							eControlStatus },	// RF ON
	{ IDC_STATIC_RFDUMMY,	_T("S-TXRF.IF_DUMMY_ANT"), 			_T(""), 							eControlStatus },	// ANT/DUMMY
	{ IDC_STATIC_RNGVAL,	_T("SS-TCRDEM.RNG_CONT"),			_T(""),								eControlStatus },	// RNG
};

// CTabSsendView20 ダイアログ

class CTabSsendView20 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabSsendView20)

public:
	CTabSsendView20(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabSsendView20();

	// ダイアログ データ
	enum { IDD = IDD_TAB_SSEND20 };

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
	afx_msg void OnBnClickedButtonMod();
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
