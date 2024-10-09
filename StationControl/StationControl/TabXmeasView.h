#pragma once
#include "afxwin.h"

static stControlParam mXmeasParam[] = {
	{ IDC_STATIC_RNGMESVAL, 	_T("CTRL.AMB_XX_DSN_DATA_KM"), 		_T(""), 		eControlDouble },
	{ IDC_STATIC_OCVAL, 		_T("CTRL.AMB_DIFF_XX_DSN_DATA_KM"), _T(""),			eControlDouble },
	{ IDC_STATIC_RRMESVAL, 		_T("CTRL.DIST_XX_DSN_DATA"), 		_T(""), 		eControlDouble },
	{ IDC_STATIC_OC2VAL, 		_T("CTRL.DIST_DIFF_XX_DSN_DATA"), 	_T(""), 		eControlDouble },
	{ IDC_STATIC_SAMPLEVAL, 	_T("DSNTCR.RARR_SAMPLERATE"), 		_T(""), 		eControlStatus },
	{ IDC_STATIC_INTGT1VAL, 	_T("DSNTCR.RNG_INTEG_TIME1"), 		_T(""), 		eControlDouble },
	{ IDC_STATIC_INTGT2VAL, 	_T("DSNTCR.RNG_INTEG_TIME2"), 		_T(""), 		eControlDouble },
	{ IDC_STATIC_RNGNUMVAL,		_T("DSNTCR_RARR.RNG_GET_DATA_NUM"), _T(""),			eControlDouble },
	{ IDC_STATIC_RANGEVAL, 		_T("DSNTCR.RNG_CONT"), 				_T(""), 		eControlStatus },
	{ IDC_STATIC_COHVAL, 		_T("DSNTCR.TRP_RATIO"), 			_T(""),		 	eControlStatus },
};


// CTabXmeasView ダイアログ

class CTabXmeasView : public CDialogEx
{
	DECLARE_DYNCREATE(CTabXmeasView)

public:
	CTabXmeasView(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabXmeasView();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_TAB_XMEAS };

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
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};
