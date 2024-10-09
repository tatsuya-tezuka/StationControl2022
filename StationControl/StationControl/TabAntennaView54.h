#pragma once
#include "afxwin.h"
#include "afxmaskededit.h"


#ifdef _DEBUG_KANSHI64M
// 64m監視
static const stControlParam mAntennaParam54[] = {
	{ IDC_BUTTON_AZ_DRIVE, _T("ANT.AZ_DRIVE"), _T("ANT.AZ_DRIVE"), eControlStatus },		// AZ DRIVE■
	{ IDC_BUTTON_EL_DRIVE, _T("ANT.EL_DRIVE"), _T("ANT.EL_DRIVE"), eControlStatus },		// EL DRIVE■
	{ IDC_BUTTON_AZ_OFFSET, _T("ANT.AZ_OFFSET"), _T("ANT.AZ_OFFSET"), eControlDouble },		// AZ OFFSET■
	{ IDC_BUTTON_EL_OFFSET, _T("ANT.EL_OFFSET"), _T("ANT.EL_OFFSET"), eControlDouble },		// EL OFFSET■
	{ IDC_BUTTON_MODE, _T("ANT.DRIVE_MODE"), _T("ANT.DRIVE_MODE"), eControlStatus },		// MODE□
	{ IDC_BUTTON_TIME, _T("ANT.TIME_OFFSET"), _T("ANT.TIME_OFFSET"), eControlTime },		// TIME OFFSET■
	{ IDC_BUTTON_RADEC_OFFSET, _T(""), _T(""), eControlCommand },							// RA/DEC OFFSET□
	{ IDC_BUTTON_KAAUTOOFFSET, _T("ANT.AZ_DRIVE"), _T("ANT.AZ_DRIVE"), eControlStatus },	// Ka AUTO OFFSET■
	{ IDC_MFCMASKEDEDIT_AZ_ANGLE, _T("ANT.REAL_AZ"), _T(""), eControlDouble },		// AZ ANGLE■
	{ IDC_MFCMASKEDEDIT_EL_ANGLE, _T("ANT.REAL_EL"), _T(""), eControlDouble },		// EL ANGLE■
	{ IDC_MFCMASKEDEDIT_AZ_ERROR, _T("ANT.ERR_AZ"), _T(""), eControlDouble },		// AZ ERROR■
	{ IDC_MFCMASKEDEDIT_EL_ERROR, _T("ANT.ERR_EL"), _T(""), eControlDouble },		// EL ERROR■
	{ IDC_STATIC_PREDDIFF, _T("ANT.TIME_OFFSET"), _T(""), eControlTime },		// PRED DIFF□
	{ IDC_STATIC_RA, _T("ANT.REAL_AZ"), _T(""), eControlNone },		// RA□
	{ IDC_STATIC_DEC, _T("ANT.REAL_EL"), _T(""), eControlNone },		// DEC□
	{ IDC_STATIC_CONTDATE, _T(""), _T(""), eControlNone },		// 制御時刻■
};

#else
// 54m監視
static const stControlParam mAntennaParam54[] = {
	{ IDC_BUTTON_AZ_DRIVE, _T("ANT.AZ_DRIVE_ON"), _T("ANT.AZ_DRIVE"), eControlStatus },		// AZ DRIVE■
	{ IDC_BUTTON_EL_DRIVE, _T("ANT.EL_DRIVE_ON"), _T("ANT.EL_DRIVE"), eControlStatus },		// EL DRIVE■
	{ IDC_BUTTON_AZ_OFFSET, _T("ANT.OFFSET_AZ"), _T("ANT.AZ_OFFSET"), eControlDouble },		// AZ OFFSET■
	{ IDC_BUTTON_EL_OFFSET, _T("ANT.OFFSET_EL"), _T("ANT.EL_OFFSET"), eControlDouble },		// EL OFFSET■
	{ IDC_BUTTON_MODE, _T("ANT.DRIVE_MODE"), _T("ANT.DRIVE_MODE"), eControlStatus },		// MODE■
	{ IDC_BUTTON_TIME, _T("ANT.TIME_OFFSET"), _T("ANT.TIME_OFFSET"), eControlTime },		// TIME OFFSET■
	{ IDC_BUTTON_RADEC_OFFSET, _T(""), _T("CTRL.ANT_RADEC"), eControlCommand },							// RA/DEC OFFSET■
	{ IDC_BUTTON_KAAUTOOFFSET, _T("ANT.Ka-AUTO_OFS_ON"), _T("ANT.KA_AUTO_OFFSET"), eControlStatus },	// Ka AUTO OFFSET■
	{ IDC_MFCMASKEDEDIT_AZ_ANGLE, _T("ANT.REAL_AZ"), _T(""), eControlDouble },		// AZ ANGLE■
	{ IDC_MFCMASKEDEDIT_EL_ANGLE, _T("ANT.REAL_EL"), _T(""), eControlDouble },		// EL ANGLE■
	{ IDC_MFCMASKEDEDIT_AZ_ERROR, _T("ANT.ERR_AZ"), _T(""), eControlDouble },		// AZ ERROR■
	{ IDC_MFCMASKEDEDIT_EL_ERROR, _T("ANT.ERR_EL"), _T(""), eControlDouble },		// EL ERROR■
	{ IDC_STATIC_PREDDIFF, _T("ANT.MON_TIME"), _T(""), eControlDouble },		// PRED DIFF■
	{ IDC_STATIC_RA, _T("CTRL.ANT_RA_OFFSET"), _T(""), eControlDouble },		// RA■
	{ IDC_STATIC_DEC, _T("CTRL.ANT_DEC_OFFSET"), _T(""), eControlDouble },		// DEC■
	{ IDC_STATIC_CONTDATE, _T("CTRL.ANT_RA_DEC_OFFSET_TIME"), _T(""), eControlTime },		// 制御時刻■
};

#endif


// CTabAntennaView54 ダイアログ

class CTabAntennaView54 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabAntennaView54)

public:
	CTabAntennaView54(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabAntennaView54();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_TAB_ANTENNA54 };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CColorStatic mAzAngle;
	CColorStatic mElAngle;
	CColorStatic mAzError;
	CColorStatic mElError;
	CColorStatic mPredDiff;
	CColorStatic mRa;
	CColorStatic mDec;
	CColorStatic mContDate;
	CString mStrRaVal;
	CString mStrDecVal;

	double mCurRealElAngle;
	double mPreRealElAngle;

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
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedButtonAzDrive();
	afx_msg void OnBnClickedButtonElDrive();
	afx_msg void OnBnClickedButtonAzOffset();
	afx_msg void OnBnClickedButtonElOffset();
	afx_msg void OnBnClickedButtonMode();
	afx_msg void OnBnClickedButtonTime();
	afx_msg void OnBnClickedButtonRadecOffset();
	afx_msg void OnBnClickedButtonKaautooffset();
	/* ------------------------------------------------------------------------------------ */
};
