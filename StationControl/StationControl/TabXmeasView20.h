#pragma once
#include "afxwin.h"


static stControlParam mXmeas20Param[] = {
	{ IDC_STATIC_RNGMESVAL, 	_T("CTRL.AMB_SX_DATA_KM"), 		_T("CTRL.AMB_SX_DATA_KM"), 		eControlDouble },
	{ IDC_STATIC_OCVAL,			_T("CTRL.AMB_DIFF_SX_DATA_KM"), _T("CTRL.AMB_DIFF_SX_DATA_KM"), eControlDouble },
	{ IDC_STATIC_RRMESVAL,		_T("CTRL.DIST_SX_DATA"), 		_T("CTRL.DIST_SX_DATA"), 		eControlDouble },
	{ IDC_STATIC_OC2VAL, 		_T("CTRL.DIST_DIFF_SX_DATA"), 	_T("CTRL.DIST_DIFF_SX_DATA"), 	eControlDouble },
	{ IDC_STATIC_SAMPLEVAL, 	_T("SX-TCRDEM.SAMPLE"), 		_T("SX-TCRDEM.SAMPLE"), 		eControlStatus },
	{ IDC_STATIC_RANGEVAL, 		_T("SX-TCRDEM.RNG_CONT"), 		_T("SX-TCRDEM.RNG_CONT"), 		eControlStatus },
	{ IDC_STATIC_COHVAL, 		_T("SX-TCRDEM.TRAPON_RATIO"), 	_T("SX-TCRDEM.TRAPON_RATIO"), 	eControlStatus },
};

//static connst CColorStatic[] = {}

// CTabXmeasView20 ダイアログ

class CTabXmeasView20 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabXmeasView20)

public:
	CTabXmeasView20(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabXmeasView20();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_TAB_SMEAS20 };

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
