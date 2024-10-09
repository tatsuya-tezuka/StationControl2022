#pragma once


static const stControlParam mXtrack34Param[] = {
	{ IDC_BUTTON_REJECT, 		_T(""),					 			_T("X-TRKRX.CARR_REJECT"), 			eControlCommand },
	{ IDC_BUTTON_PLLBW, 		_T("X-TRKRX.CARR_LOOP_BAND"), 		_T("X-TRKRX.CARR_LOOP_BAND"), 		eControlStatus },
	{ IDC_BUTTON_AGCBW, 		_T("X-TRKRX.AGC_LOOP_BAND_ANS"), 	_T("X-TRKRX.AGC_LOOP_BAND"), 		eControlStatus },
	{ IDC_BUTTON_BITRATE,		_T("X-TRKRX.BIT_RATE_ANS"),			_T("BITRATE-X"),					eControlDouble },
	{ IDC_BUTTON_DCCTRL,		_T("X-DC2.FREQUENCY"), 				_T("X-DC2.FREQUENCY"), 				eControlDouble },
	{ IDC_BUTTON_RX, 			_T("X-DC2.FREQUENCY"), 				_T("X-DC2.FREQUENCY"), 				eControlDouble },
	{ IDC_BUTTON_DC, 			_T("X-DC2.FREQUENCY"), 				_T("X-DC2.FREQUENCY"), 				eControlDouble },
	{ IDC_STATIC_CARRVAL, 		_T("X-TRKRX.CARR_LOCK_ACH_STR"),	_T("X-TRKRX.CARR_LOCK_ACH_STR"), 	eControlStatus },
	{ IDC_STATIC_CARRVAL2, 		_T("X-TRKRX.CARR_LOCK_BCH_STR"), 	_T("X-TRKRX.CARR_LOCK_BCH_STR"), 	eControlStatus },
	{ IDC_MFCMASKEDEDIT_AGC, 	_T("X-TRKRXAGC.AGC_A_AVG"), 		_T("X-TRKRXAGC.AGC_A_AVG"), 		eControlDouble },
	{ IDC_MFCMASKEDEDIT_AGC2,	_T("X-TRKRXAGC.AGC_B_AVG"), 		_T("X-TRKRXAGC.AGC_B_AVG"), 		eControlDouble },
};

// CTabXtrackView34 ダイアログ

class CTabXtrackView34 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabXtrackView34)

public:
	CTabXtrackView34(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabXtrackView34();

	// ダイアログ データ
	enum { IDD = IDD_TAB_XTRACK34 };

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
