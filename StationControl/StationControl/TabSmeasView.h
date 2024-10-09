#pragma once
#include "afxwin.h"


static stControlParam mSmeasParam[] = {
	{ IDC_STATIC_RNGMESVAL, _T("CTRL.AMB_SS_DATA_KM"), 		_T(""), 						eControlDouble },		// RNG MEAS
	{ IDC_STATIC_OCVAL,		_T("CTRL.AMB_DIFF_SS_DATA_KM"), _T(""), 						eControlDouble },		// O-C
	{ IDC_STATIC_RRMESVAL,	_T("CTRL.DIST_SS_DATA"), 		_T(""), 						eControlDouble },		// RR MEAS
	{ IDC_STATIC_OC2VAL, 	_T("CTRL.DIST_DIFF_SS_DATA"), 	_T(""), 						eControlDouble },		// O-C
	{ IDC_STATIC_SAMPLEVAL, _T("SS-TCRDEM.SAMPLE"), 		_T(""), 						eControlStatus },		// SAMPLE
	{ IDC_STATIC_RANGEVAL, 	_T("SS-TCRDEM.RNG_CONT"), 		_T(""), 						eControlStatus },		// RANGE
	{ IDC_STATIC_COHVAL, 	_T("SS-TCRDEM.TRAPON_RATIO"), 	_T(""), 						eControlStatus },		// COH/INCOH
};

//static connst CColorStatic[] = {}

// CTabSmeasView ダイアログ

class CTabSmeasView : public CDialogEx
{
	DECLARE_DYNCREATE(CTabSmeasView)

public:
	CTabSmeasView(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabSmeasView();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_TAB_SMEAS };

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
	CColorStatic mRangeVal;
	CColorStatic mCohVal;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};
