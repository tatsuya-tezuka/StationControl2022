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

// CTabXmeasView20 �_�C�A���O

class CTabXmeasView20 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabXmeasView20)

public:
	CTabXmeasView20(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CTabXmeasView20();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_TAB_SMEAS20 };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	static const int mParamNum = 7;

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
