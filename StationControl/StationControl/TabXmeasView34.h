#pragma once
#include "afxwin.h"

static stControlParam mXmeas34Param[] = {
	{ IDC_STATIC_RNGMESVAL, 	_T("CTRL.AMB_XX_DSN_DATA_KM"), 		_T("CTRL.AMB_XX_DSN_DATA_KM"), 		eControlDouble },
	{ IDC_STATIC_OCVAL, 		_T("CTRL.AMB_DIFF_XX_DSN_DATA_KM"), _T("CTRL.AMB_DIFF_XX_DSN_DATA_KM"), eControlDouble },
	{ IDC_STATIC_RRMESVAL, 		_T("CTRL.DIST_XX_DSN_DATA"), 		_T("CTRL.DIST_XX_DSN_DATA"), 		eControlDouble },
	{ IDC_STATIC_OC2VAL, 		_T("CTRL.DIST_DIFF_XX_DSN_DATA"), 	_T("CTRL.DIST_DIFF_XX_DSN_DATA"), 	eControlDouble },
	{ IDC_STATIC_SAMPLEVAL, 	_T("DSNTCR.RARR_SAMPLERATE"), 		_T("DSNTCR.RARR_SAMPLERATE"), 		eControlStatus },
	{ IDC_STATIC_INTGT1VAL, 	_T("DSNTCR.RNG_INTEG_TIME1"), 		_T("DSNTCR.RNG_INTEG_TIME1"), 		eControlDouble },
	{ IDC_STATIC_INTGT2VAL, 	_T("DSNTCR.RNG_INTEG_TIME2"), 		_T("DSNTCR.RNG_INTEG_TIME2"), 		eControlDouble },
	{ IDC_STATIC_RNGNUMVAL,		_T("DSNTCR_RARR.RNG_GET_DATA_NUM"), _T("DSNTCR_RARR.RNG_GET_DATA_NUM"), eControlDouble },
	{ IDC_STATIC_RANGEVAL, 		_T("DSNTCR.RNG_CONT"), 				_T("DSNTCR.RNG_CONT"), 				eControlStatus },
	{ IDC_STATIC_COHVAL, 		_T("DSNTCR.TRP_RATIO"), 			_T("DSNTCR.TRP_RATIO"), 			eControlStatus },
	{ IDC_STATIC_RNGMODEVAL, 	_T("DSNTCR.RNG_MODE"), 				_T("DSNTCR.RNG_MODE"), 				eControlStatus },
};

// CTabXmeasView34 ダイアログ

class CTabXmeasView34 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabXmeasView34)

public:
	CTabXmeasView34(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabXmeasView34();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_TAB_XMEAS34 };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	static const int mParamNum = 7;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

protected:
	void	UpdateStatus();
	void	InitStatus();

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
	CColorStatic mRngModeVal;
	CColorStatic mEbVal;
	/* ------------------------------------------------------------------------------------ */
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
