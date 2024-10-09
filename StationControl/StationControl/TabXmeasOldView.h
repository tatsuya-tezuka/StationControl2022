#pragma once

static const stControlParam mXmeasOldParam[] = {
	{ IDC_STATIC_RNGMESVAL, 	_T("CTRL.AMB_XX_DATA_KM"), 		_T("CTRL.AMB_XX_DATA_KM"), 		eControlDouble },
	{ IDC_STATIC_OCVAL, 		_T("CTRL.AMB_DIFF_XX_DATA_KM"), _T("CTRL.AMB_DIFF_XX_DATA_KM"), eControlDouble },
	{ IDC_STATIC_RRMESVAL, 		_T("CTRL.DIST_XX_DATA"), 		_T("CTRL.DIST_XX_DATA"), 		eControlDouble },
	{ IDC_STATIC_OC2VAL, 		_T("CTRL.DIST_DIFF_XX_DATA"), 	_T("CTRL.DIST_DIFF_XX_DATA"), 	eControlDouble },
	{ IDC_STATIC_SAMPLEVAL, 	_T("XX-RARR.SAMPLING"), 		_T("XX-RARR.SAMPLING"), 		eControlStatus },
	{ IDC_STATIC_INTGT1VAL, 	_T("XX-RARR.INTEG_TIME1"), 		_T("XX-RARR.INTEG_TIME1"), 		eControlDouble },
	{ IDC_STATIC_INTGT2VAL, 	_T("XX-RARR.INTEG_TIME2"), 		_T("XX-RARR.INTEG_TIME2"), 		eControlDouble },
	{ IDC_STATIC_RNGNUMVAL,		_T("CTRL.XX_RNG_GOT_CNT"),		_T("CTRL.XX_RNG_GOT_CNT"),		eControlDouble },
	{ IDC_STATIC_RANGEVAL, 		_T("XX-RARR.RNG_CONT"), 		_T("XX-RARR.RNG_CONT"), 		eControlStatus },
	{ IDC_STATIC_COHVAL, 		_T("XX-RARR.COH_RATIO_S"), 		_T("XX-RARR.COH_RATIO_S"), 		eControlStatus },
};

// CTabXmeasOldView ダイアログ

class CTabXmeasOldView : public CDialogEx
{
	DECLARE_DYNCREATE(CTabXmeasOldView)

public:
	CTabXmeasOldView(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabXmeasOldView();

// ダイアログ データ
	enum { IDD = IDD_TAB_XMEASOLD };


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
