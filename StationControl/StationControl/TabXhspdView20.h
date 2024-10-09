#pragma once


static const stControlParam mXhspd20Param[] = {
	{ IDC_BUTTON_REJECT, 		_T(""),								_T("X-HRX.DEMO_LOCK_REJECT"), 		eControlCommand },
	{ IDC_BUTTON_PLLBW, 		_T(""), 							_T(""), 							eControlNone },
	{ IDC_BUTTON_AGCBW, 		_T(""), 							_T(""), 							eControlNone },
	{ IDC_BUTTON_BITRATE,		_T("X-HRX.DEMO_DATA_RATE_S"),		_T("BITRATE-XHRX"),					eControlDouble },
	{ IDC_BUTTON_DCCTRL, 		_T("X-HRX.DEMO_RECV_FREQ"), 		_T("X-HRX.DEMO_CENTER_FREQUENCY"), 	eControlDouble },
	{ IDC_BUTTON_RX, 			_T("X-HRX.DEMO_RECV_FREQ"), 		_T("X-HRX.DEMO_CENTER_FREQUENCY"), 	eControlDouble },
	{ IDC_BUTTON_DC, 			_T("X-HRX.DEMO_RECV_FREQ"), 		_T("X-HRX.DEMO_CENTER_FREQUENCY"), 	eControlDouble },
	{ IDC_STATIC_CARRVAL, 		_T("X-HRX.LOCK_STATUS_CARRIER_S"), 	_T("X-HRX.LOCK_STATUS_CARRIER_S"), 	eControlStatus },
	{ IDC_STATIC_VITERBIVAL, 	_T("X-HRX.LOCK_STATUS_VITERBI_S"), 	_T("X-HRX.LOCK_STATUS_VITERBI_S"), 	eControlStatus },
	{ IDC_STATIC_FRAMEVAL, 		_T("X-HRX.LOCK_STATUS_FRAME_S"), 	_T("X-HRX.LOCK_STATUS_FRAME_S"), 	eControlStatus },
	{ IDC_MFCMASKEDEDIT_AGC, 	_T("X-HRXAGC.AGC_A_AVG"), 			_T("X-HRXAGC.AGC_A_AVG"), 			eControlDouble },
	{ IDC_STATIC_EB,			_T("X-HRX.EB_N0_ICH"),				_T("X-HRX.EB_N0_ICH"),				eControlDouble },
	{ IDC_STATIC_EB2,			_T("X-HRX.EB_N0_QCH"),				_T("X-HRX.EB_N0_QCH"),				eControlDouble },
};

// CTabXhspdView20 ダイアログ

class CTabXhspdView20 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabXhspdView20)

public:
	CTabXhspdView20(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabXhspdView20();

	// ダイアログ データ
	enum { IDD = IDD_TAB_XHSPD20 };

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
	CColorStatic mViterbiVal;
	CColorStatic mFrameVal;
	CColorStatic mEbVal1;
	CColorStatic mEbVal2;
	CColorStatic mAgcVal;
	CColorStatic mAgcVal2;
	afx_msg void OnBnClickedButtonReject();
	afx_msg void OnBnClickedButtonBitrate();
	afx_msg void OnBnClickedButtonDcctrl();
	afx_msg void OnBnClickedButtonRx();
	afx_msg void OnBnClickedButtonDc();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};
