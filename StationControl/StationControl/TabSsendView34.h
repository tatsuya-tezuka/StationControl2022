#pragma once


static const stControlParam mSSend34Param[] = {
	{ IDC_BUTTON_RF, 		_T("S-TX.RF_POWER_SEL"),			_T("S-TX.SS1K_POWER_SEL_CONT"), 	eControlStatus },	// 送信電力
	{ IDC_BUTTON_MOD, 		_T("SS-TCRMOD.MODULATION"), 		_T("SS-TCRMOD.MODULATION"), 		eControlStatus },	// MOD
	{ IDC_BUTTON_INDEXCMD, 	_T("SS-TCRMOD.CMD_MOD_INDEX"), 		_T("SS-TCRMOD.CMD_MOD_INDEX"), 		eControlDouble },	// INDEX CMD
	{ IDC_BUTTON_INDEXCMD2, _T("SS-TCRMOD.RNG_MOD_INDEX"), 		_T("SS-TCRMOD.RNG_MOD_INDEX"), 		eControlDouble },	// INDEX RNG
	{ IDC_BUTTON_MODMODE, 	_T("SS-TCRMOD.MOD_FORMAT_MODE"),	_T("SS-TCRMOD.MOD_FORMAT_MODE"),	eControlStatus },	// MOD MODE
	{ IDC_BUTTON_UCCTRL, 	_T("S-TXSYN.STT_FREQ"), 			_T("S-TXSYN.STT_FREQ"), 			eControlDouble },	// U/C CTRL
	{ IDC_BUTTON_TX,		_T("S-TXSYN.FREQ"), 				_T("S-TXSYN.STT_FREQ"), 			eControlDouble },	// TX
	{ IDC_BUTTON_UC,		_T("S-TXSYN.STT_FREQ"), 			_T("S-TXSYN.STT_FREQ"), 			eControlDouble },	// U/C
	{ IDC_BUTTON_SWEEP, 	_T("SS-TCRMOD.SWEEP_CMD_ANS"),	 	_T("SS-TCRMOD.SWEEP_CMD"),	 		eControlStatus },	// SWEEP
	{ IDC_BUTTON_HOLDTONE, 	_T("CMDOUT.HOLDTONE_STAT"),			_T("CMDOUT.HOLD_TONE"),				eControlStatus },	// HOLD TONE
	{ IDC_BUTTON_BITRATE,	_T("SS-TCRMOD.BIT_RATE"),			_T("CMD-BITRATE"),					eControlDouble },	// BIT RATE
	{ IDC_STATIC_RFVAL,		_T("S-TX.RF_ON_DET_ANS"),			_T(""), 							eControlStatus },	// RF ON
	{ IDC_STATIC_RFDUMMY,	_T("S-TX.ANT_DMY_COLIM"), 			_T(""), 							eControlStatus },	// ANT/DUMMY
	{ IDC_STATIC_RNGVAL,	_T("SS-TCRDEM.RNG_CONT"),			_T(""),								eControlStatus },	// RNG
};

// CTabSsendView34 ダイアログ

class CTabSsendView34 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabSsendView34)

public:
	CTabSsendView34(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabSsendView34();

// ダイアログ データ
	enum { IDD = IDD_TAB_SSEND34 };

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
	afx_msg void OnBnClickedButtonMod2();
};
