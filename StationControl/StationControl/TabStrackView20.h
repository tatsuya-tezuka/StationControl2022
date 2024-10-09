#pragma once


static const stControlParam mStrack20Param[] = {
	{ IDC_BUTTON_REJECT, 		_T(""), 							_T("SS-TCRDET.CARR_PLL_REJECT"), 	eControlCommand },
	{ IDC_BUTTON_PLLBW, 		_T("SS-TCRDET.CARR_LBW"),	 		_T("SS-TCRDET.CARR_LBW"),	 		eControlStatus },
	{ IDC_BUTTON_AGCBW, 		_T("SS-TCRDET.AGC_LBW"), 			_T("SS-TCRDET.AGC_LBW"), 			eControlStatus },
	{ IDC_BUTTON_BITRATE,		_T("SS-TCRDET.BIT_RATE"),			_T("SS-TCRDET.BIT_RATE"),			eControlDouble },
	{ IDC_BUTTON_DCCTRL, 		_T("SS-DCSYN.FREQ_MON"), 			_T("SS-DCSYN.SET_FREQ"), 			eControlDouble },
	{ IDC_BUTTON_RX, 			_T("SS-DCSYN.FREQ_MON"), 			_T("SS-DCSYN.SET_FREQ"), 			eControlDouble },
	{ IDC_BUTTON_DC, 			_T("SS-DCSYN.FREQ_MON"),	 		_T("SS-DCSYN.SET_FREQ"),	 		eControlDouble },
	{ IDC_STATIC_CARRVAL, 		_T("SS-TCRDET.CARR_LOCK_ACH_STR"), 	_T("SS-TCRDET.CARR_LOCK_ACH_STR"), 	eControlStatus },
	{ IDC_STATIC_CARRVAL2, 		_T("SS-TCRDET.CARR_LOCK_BCH_STR"),	_T("SS-TCRDET.CARR_LOCK_BCH_STR"),	eControlStatus },
	{ IDC_MFCMASKEDEDIT_AGC, 	_T("SS-TCRDTAGC.AGC_A_AVG"), 		_T("SS-TCRDTAGC.AGC_A_AVG"), 		eControlDouble },
	{ IDC_MFCMASKEDEDIT_AGC2, 	_T("SS-TCRDTAGC.AGC_B_AVG"), 		_T("SS-TCRDTAGC.AGC_B_AVG"), 		eControlDouble },
};

// CTabStrackView20 ダイアログ

class CTabStrackView20 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabStrackView20)

public:
	CTabStrackView20(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabStrackView20();

	// ダイアログ データ
	enum { IDD = IDD_TAB_STRACK20 };

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
	CColorStatic mSubCarrVal;
	CColorStatic mViterbiVal;
	CColorStatic mBitVal;
	CColorStatic mFrameVal;
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
