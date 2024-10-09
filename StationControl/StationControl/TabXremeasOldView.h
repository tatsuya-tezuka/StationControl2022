#pragma once


static const stControlParam mXremeasOldParam[] = {
	{ IDC_STATIC_RNGMESVAL, 	_T("CTRL.AMB_RX_DATA_KM"), 			_T("CTRL.AMB_RX_DATA_KM"), 			eControlDouble },
	{ IDC_STATIC_OCVAL, 		_T("CTRL.AMB_DIFF_RX_DATA_KM"), 	_T("CTRL.AMB_DIFF_RX_DATA_KM"), 	eControlDouble },
	{ IDC_STATIC_RRMESVAL, 		_T("CTRL.DIST_XX_DATA"), 			_T("CTRL.DIST_XX_DATA"), 			eControlDouble },
	{ IDC_STATIC_OC2VAL, 		_T("CTRL.DIST_DIFF_XX_DATA"), 		_T("CTRL.DIST_DIFF_XX_DATA"), 		eControlDouble },
	{ IDC_STATIC_SAMPLEVAL, 	_T("XX-RRNG.SAMPLE"), 				_T("XX-RRNG.SAMPLE"), 				eControlStatus },
	{ IDC_STATIC_INTGT1VAL, 	_T("XX-RRNG.INTEG_TIME_1"),	 		_T("XX-RRNG.INTEG_TIME_1"),	 		eControlDouble },
	{ IDC_STATIC_INTGT2VAL, 	_T("XX-RRNG.INTEG_TIME_2"), 		_T("XX-RRNG.INTEG_TIME_2"), 		eControlDouble },
	{ IDC_STATIC_RNGNUMVAL,		_T("CTRL.RX_RNG_GOT_CNT"),			_T("CTRL.RX_RNG_GOT_CNT"),			eControlDouble },
	{ IDC_STATIC_RANGEVAL, 		_T("XX-RRNG.RNG_CONT"), 			_T("XX-RRNG.RNG_CONT"), 			eControlStatus },
	{ IDC_STATIC_COHVAL, 		_T("XX-RARR.COH_RATIO_S"), 			_T("XX-RARR.COH_RATIO_S"), 			eControlStatus },
};

// CTabXremeasOldView ダイアログ

class CTabXremeasOldView : public CDialogEx
{
	DECLARE_DYNCREATE(CTabXremeasOldView)

public:
	CTabXremeasOldView(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabXremeasOldView();

// ダイアログ データ
	enum { IDD = IDD_TAB_XREMEASOLD };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	static const int mParamNum = 10;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	void	UpdateStatus();
	void	InitStatus();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CColorStatic mRngMesVal;
	CColorStatic mOcVal;
	CColorStatic mRrMesVal;
	CColorStatic mRrOcVal;
	CColorStatic mSampleVal;
	CColorStatic mInteg1Val;
	CColorStatic mIteng2Val;
	CColorStatic mRngNumVal;
	CColorStatic mRangeVal;
	CColorStatic mCohVal;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
