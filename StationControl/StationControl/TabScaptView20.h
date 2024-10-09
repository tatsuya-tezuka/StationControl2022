#pragma once


static const stControlParam mScapt20Param[] = {
	{ IDC_BUTTON_REJECT, 		_T(""),						 		_T("S-ACQTCRDEM.CARR_REJECT"), 		eControlCommand },
	{ IDC_BUTTON_PLLBW,			_T("S-ACQTCRDEM.CARR_LOOP"),		_T("S-ACQTCRDEM.CARR_LOOP"),		eControlStatus },
	{ IDC_BUTTON_AGCBW,			_T("S-ACQTCRDEM.AGC_LOOP"),			_T("S-ACQTCRDEM.AGC_LOOP"),			eControlStatus },
	{ IDC_BUTTON_BITRATE,		_T("S-ACQDET.BIT_RATE"),			_T("S-ACQDET.BIT_RATE"),			eControlDouble },
	{ IDC_BUTTON_DCCTRL, 		_T("SS-DCSYN.FREQ_MON"), 			_T("SS-DCSYN.SET_FREQ"), 			eControlDouble },
	{ IDC_BUTTON_RX, 			_T("SS-DCSYN.FREQ_MON"), 			_T("SS-DCSYN.SET_FREQ"), 			eControlDouble },
	{ IDC_BUTTON_DC, 			_T("SS-DCSYN.FREQ_MON"), 			_T("SS-DCSYN.SET_FREQ"), 			eControlDouble },
	{ IDC_STATIC_CARRVAL,		_T("S-ACQTCRDEM.CARR_LOCK_ACH"),	_T("S-ACQTCRDEM.CARR_LOCK_ACH"),	eControlStatus },
	{ IDC_STATIC_CARRVAL2,		_T("S-ACQTCRDEM.CARR_LOCK_BCH"),	_T("S-ACQTCRDEM.CARR_LOCK_BCH"),	eControlStatus },
	{ IDC_MFCMASKEDEDIT_AGC, 	_T("S-ACQTCRAGC.AGC_A_AVG"), 		_T("S-ACQTCRAGC.AGC_A_AVG"), 		eControlDouble },
	{ IDC_MFCMASKEDEDIT_AGC2, 	_T("S-ACQTCRAGC.AGC_B_AVG"), 		_T("S-ACQTCRAGC.AGC_B_AVG"), 		eControlDouble },
};

// CTabScaptView20 ダイアログ

class CTabScaptView20 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabScaptView20)

public:
	CTabScaptView20(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabScaptView20();

	// ダイアログ データ
	enum { IDD = IDD_TAB_SCAPT20 };

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
	CColorStatic mCarrVal2;
	CColorStatic mAgcVal;
	CColorStatic mAgcVal2;
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
