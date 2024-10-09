#pragma once


static const stControlParam mScapt34Param[] = {
	{ IDC_BUTTON_REJECT, 		_T(""), 							_T("S-ACQ_TRKRX.CARR_REJECT"),		eControlCommand },
	{ IDC_BUTTON_PLLBW, 		_T("S-ACQ_TRKRX.CARR_LOOP_BAND"), 	_T("S-ACQ_TRKRX.CARR_LOOP_BAND"),	eControlStatus },
	{ IDC_BUTTON_AGCBW, 		_T("S-ACQ_TRKRX.AGC_LOOP_BAND_ANS"),_T("S-ACQ_TRKRX.AGC_LOOP_BAND"), 	eControlStatus },
	{ IDC_BUTTON_BITRATE,		_T("S-ACQ_TRKRX.BIT_RATE_ANS"),		_T("BITRATE-S_ACQ"),				eControlDouble },
	{ IDC_BUTTON_DCCTRL, 		_T("S-DC.FREQUENCY"), 				_T("S-DC.FREQUENCY"), 				eControlDouble },
	{ IDC_BUTTON_RX, 			_T("S-DC.FREQUENCY"), 				_T("S-DC.FREQUENCY"), 				eControlDouble },
	{ IDC_BUTTON_DC, 			_T("S-DC.FREQUENCY"), 				_T("S-DC.FREQUENCY"), 				eControlDouble },
	{ IDC_STATIC_CARRVAL,		_T("S-ACQ_TRKRX.CARR_LOCK_ACH"),	_T("S-ACQ_TRKRX.CARR_LOCK_ACH"),	eControlStatus },
	{ IDC_STATIC_CARRVAL2,		_T("S-ACQ_TRKRX.CARR_LOCK_BCH"),	_T("S-ACQ_TRKRX.CARR_LOCK_BCH"),	eControlStatus },
	{ IDC_MFCMASKEDEDIT_AGC, 	_T("S-ACQRXAGC.AGC_A_AVG"), 		_T("S-ACQRXAGC.AGC_A_AVG"), 		eControlDouble },
	{ IDC_MFCMASKEDEDIT_AGC2, 	_T("S-ACQRXAGC.AGC_B_AVG"), 		_T("S-ACQRXAGC.AGC_B_AVG"), 		eControlDouble },
};

// CTabScaptView34 ダイアログ

class CTabScaptView34 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabScaptView34)

public:
	CTabScaptView34(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabScaptView34();

	// ダイアログ データ
	enum { IDD = IDD_TAB_SCAPT34 };

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
