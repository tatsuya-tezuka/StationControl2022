#pragma once

static const stControlParam mXsendOldParam[] = {
	{ IDC_BUTTON_RF, 			_T("X-UL_CNT.RF_POWER_SEL"), 		_T("X-UL_CNT.RF_PWR_200/2k/20kW"), 	eControlStatus },		// 送信電力
	{ IDC_BUTTON_CMDSC, 		_T("XX-TCRMOD.SUB_CARR"), 			_T("XX-TCRMOD.SUB_CARR"), 			eControlDouble },		// CMD SC
	{ IDC_BUTTON_MOD, 			_T("XX-TCRMOD.MODULATION"), 		_T("XX-TCRMOD.MODULATION"), 		eControlStatus },		// MOD
	{ IDC_BUTTON_INDEXCMD, 		_T("XX-TCRMOD.CMD_MOD_INDEX"), 		_T("XX-TCRMOD.CMD_MOD_INDEX"), 		eControlDouble },		// INDEX CMD
	{ IDC_BUTTON_INDEXCMD2, 	_T("XX-TCRMOD.RNG_MOD_INDEX"), 		_T("XX-TCRMOD.RNG_MOD_INDEX"), 		eControlDouble },		// INDEX RNG
	{ IDC_BUTTON_UCCTRL, 		_T("X-TXSYN.STT_FREQ"), 			_T("X-TXSYN.STT_FREQ"), 			eControlDouble },		// U/C CTRL
	{ IDC_BUTTON_TX, 			_T("X-TXSYN.FREQ"), 				_T("X-TXSYN.STT_FREQ"), 			eControlDouble },		// TX
	{ IDC_BUTTON_UC, 			_T("X-TXSYN.STT_FREQ"), 			_T("X-TXSYN.STT_FREQ"), 			eControlDouble },		// U/C
	{ IDC_BUTTON_SWEEP, 		_T("X-TXSYN.SWEEP_CONT"), 			_T("X-TXSYN.SWEEP_CONT"), 			eControlStatus },		// SWEEP
	{ IDC_BUTTON_HOLDTONE, 		_T("CMDOUT3.HOLD_TONE"), 			_T("CMDOUT3.HOLD_TONE"), 			eControlStatus },		// HOLD TONE
	{ IDC_BUTTON_BITRATE,		_T("XX-TCRMOD.BIT_RATE"),			_T("CMD3-BITRATE"),					eControlDouble },		// BIT RATE
	{ IDC_STATIC_RFVAL, 		_T("X-UL_CNT.RF_ON_CONT"), 			_T("X-UL_CNT.RF_ON_CONT"), 			eControlStatus },		// RF ON
	{ IDC_STATIC_RFDUMMY, 		_T("X-UL_CNT.ANT/DUMMY_SELECT"), 	_T("X-UL_CNT.ANT/DUMMY_SELECT"), 	eControlStatus },		// ANT/DUMMY
	{ IDC_STATIC_RNGVAL, 		_T("XX-RARR.RNG_CONT"), 			_T("XX-RARR.RNG_CONT"), 			eControlStatus },		// RNG
};																																

// CTabXsendOldView ダイアログ

class CTabXsendOldView : public CDialogEx
{
	DECLARE_DYNCREATE(CTabXsendOldView)

public:
	CTabXsendOldView(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabXsendOldView();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_TAB_XSENDOLD };

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
	CColorStatic mRngVal;
	CColorStatic mRfVal;
	CColorStatic mDummyVal;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};
