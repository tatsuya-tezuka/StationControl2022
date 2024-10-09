#pragma once


static const stControlParam mStrack34Param[] = {
	{ IDC_BUTTON_REJECT, 		_T(""),						 		_T("S-TRKRX.CARR_PLL_REJECT"), 		eControlCommand },
	{ IDC_BUTTON_PLLBW, 		_T("S-TRKRX.CARR_LBW"), 			_T("S-TRKRX.CARR_LBW"), 			eControlStatus },
	{ IDC_BUTTON_AGCBW, 		_T("S-TRKRX.AGC_LBW"), 				_T("S-TRKRX.AGC_LBW"), 				eControlStatus },
	{ IDC_BUTTON_BITRATE,		_T("S-TRKRX.BIT_RATE"),				_T("BITRATE-S_TRK"),				eControlDouble },
	{ IDC_BUTTON_DCCTRL, 		_T("S-DC.FREQUENCY"), 				_T("S-DC.FREQUENCY"), 				eControlDouble },
	{ IDC_BUTTON_RX, 			_T("S-DC.FREQUENCY"), 				_T("S-DC.FREQUENCY"), 				eControlDouble },
	{ IDC_BUTTON_DC, 			_T("S-DC.FREQUENCY"), 				_T("S-DC.FREQUENCY"), 				eControlDouble },
	{ IDC_STATIC_CARRVAL, 		_T("S-TRKRX.CARR_LOCK_ACH_STR"), 	_T("S-TRKRX.CARR_LOCK_ACH_STR"), 	eControlStatus },
	{ IDC_STATIC_CARRVAL2, 		_T("S-TRKRX.CARR_LOCK_BCH_STR"), 	_T("S-TRKRX.CARR_LOCK_BCH_STR"), 	eControlStatus },
	{ IDC_MFCMASKEDEDIT_AGC,	_T("S-RXAGC.AGC_A_AVG"),			_T("S-RXAGC.AGC_A_AVG"),			eControlDouble },
	{ IDC_MFCMASKEDEDIT_AGC2,	_T("S-RXAGC.AGC_B_AVG"),			_T("S-RXAGC.AGC_B_AVG"),			eControlDouble },
};

// CTabStrackView34 ダイアログ

class CTabStrackView34 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabStrackView34)

public:
	CTabStrackView34(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabStrackView34();

	// ダイアログ データ
	enum { IDD = IDD_TAB_STRACK34 };

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
