#pragma once
#include "afxwin.h"

static stControlParam mXmeasParam54[] = {
//	{ IDC_STATIC_RNGMESVAL, _T("CTRL.AMB_XX_DSN_DATA_KM"), _T(""), eControlDouble },						// RNG MEAS��
//	{ IDC_STATIC_OCVAL, _T("CTRL.AMB_DIFF_XX_DSN_DATA_KM"), _T(""), eControlDouble },						// O-C��
//	{ IDC_STATIC_RRMESVAL, _T("CTRL.DIST_XX_DSN_DATA"), _T(""), eControlDouble },							// RR MEAS��
//	{ IDC_STATIC_OC2VAL, _T("CTRL.DIST_DIFF_XX_DSN_DATA"), _T(""), eControlDouble },						// O-C��
	{ IDC_STATIC_SAMPLEVAL, _T("DSNTCR.RARR_SAMPLERATE"), _T(""), eControlStatus },							// SAMPLE��
	{ IDC_STATIC_INTGT1VAL, _T("DSNTCR.RNG_INTEG_TIME1"), _T(""), eControlDouble },							// INTG T1��
	{ IDC_STATIC_INTGT2VAL, _T("DSNTCR.RNG_INTEG_TIME2"), _T(""), eControlDouble },							// INTG T2��
//	{ IDC_STATIC_RNGNUMVAL, _T("DSNTCR_RARR.RNG_GET_DATA_NUM"), _T(""), eControlDouble },					// RNG NUM(���q)��
//	{ IDC_STATIC_RNGDENOVAL, _T("DSNTCR.RNG_NUM"), _T(""), eControlDouble },								// RNG NUM(����)��
	{ IDC_STATIC_RANGEVAL, _T("DSNTCR.RNG_CONT"), _T(""), eControlStatus },									// RANGE��
	{ IDC_STATIC_RNGMODEVAL, _T("DSNTCR.RNG_MODE"), _T(""), eControlStatus },								// RNG MODE��
	{ IDC_STATIC_COHVAL, _T("DSNTCR.TRP_RATIO"), _T(""), eControlStatus },									// COH/INCOH��
};


// RNG MEAS, O-C, RR MEAS, O-C
static stControlParam mXmeasParamRngRr54[eXmeasBandType_Num][4] = {
	// X�ю�M
	// CTRL.BAND = X
	{
		{ IDC_STATIC_RNGMESVAL, _T("CTRL.AMB_XX_DSN_DATA_KM"), _T(""), eControlDouble },						// RNG MEAS��
		{ IDC_STATIC_OCVAL, _T("CTRL.AMB_DIFF_XX_DSN_DATA_KM"), _T(""), eControlDouble },						// O-C��
		{ IDC_STATIC_RRMESVAL, _T("CTRL.DIST_XX_DSN_DATA"), _T(""), eControlDouble },							// RR MEAS��
		{ IDC_STATIC_OC2VAL, _T("CTRL.DIST_DIFF_XX_DSN_DATA"), _T(""), eControlDouble },						// O-C��
	},

	// Ka�ю�M
	// CTRL.BAND = Ka
	{
		{ IDC_STATIC_RNGMESVAL, _T("CTRL.AMB_XKa_DSN_DATA_KM"), _T(""), eControlDouble },						// RNG MEAS��
		{ IDC_STATIC_OCVAL, _T("CTRL.AMB_DIFF_XKa_DSN_DATA_KM"), _T(""), eControlDouble },						// O-C��
		{ IDC_STATIC_RRMESVAL, _T("CTRL.DIST_XKa_DSN_DATA"), _T(""), eControlDouble },							// RR MEAS��
		{ IDC_STATIC_OC2VAL, _T("CTRL.DIST_DIFF_XKa_DSN_DATA"), _T(""), eControlDouble },						// O-C��
	},
};

// RNG NUM
static stControlParam mXmeasParamRngNum54[eXmeasBandType_Num][eXmeasRngMode_Num][1] = {
	// X�ю�M
	{
		{
			// �]�����������W
			// CTRL.BAND = X
			// DSNTCR.RNG_MODE = TURNAAROUND
			{ IDC_STATIC_RNGNUMVAL, _T("CTRL.DX_RNG_GOT_CNT"), _T(""), eControlDouble },							// RNG NUM(���q)��
		},

		{
			// �Đ����������W
			// CTRL.BAND = X
			// DSNTCR.RNG_MODE = REGEN
			{ IDC_STATIC_RNGNUMVAL, _T("CTRL.RX_RNG_GOT_CNT"), _T(""), eControlDouble },							// RNG NUM(���q)��
		},
	},

	// Ka�ю�M
	{
		{
			// �]�����������W
			// CTRL.BAND = Ka
			// DSNTCR.RNG_MODE = TURNAAROUND
			{ IDC_STATIC_RNGNUMVAL, _T("CTRL.DKa_RNG_GOT_CNT"), _T(""), eControlDouble },					// RNG NUM(���q)��
		},

		{
			// �Đ����������W
			// CTRL.BAND = Ka
			// DSNTCR.RNG_MODE = REGEN
			{ IDC_STATIC_RNGNUMVAL, _T("CTRL.RKa_RNG_GOT_CNT"), _T(""), eControlDouble },					// RNG NUM(���q)��
		},
	},
};


// CTabXmeasView54 �_�C�A���O

class CTabXmeasView54 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabXmeasView54)

public:
	CTabXmeasView54(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CTabXmeasView54();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_TAB_XMEAS54 };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CColorStatic mRngMesVal;
	CColorStatic mOcVal;
	CColorStatic mRrMesVal;
	CColorStatic mRrOcVal;
	CColorStatic mSampleVal;
	CColorStatic mInteg1Val;
	CColorStatic mIteng2Val;
	CColorStatic mRngNumVal;
	CColorStatic mRangeVal;
	CColorStatic mRngModeVal;
	CColorStatic mCohVal;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

protected:
	void	UpdateStatus();
	void	InitStatus();

public:
	virtual BOOL OnInitDialog();

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};
