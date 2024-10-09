#pragma once
#include "afxwin.h"


static stControlParam mSmeas20Param[] = {
	{ IDC_STATIC_RNGMESVAL, 	_T("CTRL.AMB_SS_DATA_KM"), 			_T(""), 			eControlDouble },
	{ IDC_STATIC_OCVAL, 		_T("CTRL.AMB_DIFF_SS_DATA_KM"), 	_T(""), 			eControlDouble },
	{ IDC_STATIC_RRMESVAL, 		_T("CTRL.DIST_SS_DATA"), 			_T(""), 			eControlDouble },
	{ IDC_STATIC_OC2VAL, 		_T("CTRL.DIST_DIFF_SS_DATA"), 		_T(""), 			eControlDouble },
	{ IDC_STATIC_SAMPLEVAL, 	_T("SS-TCRDEM.SAMPLE"), 			_T(""), 			eControlStatus },
	{ IDC_STATIC_RANGEVAL, 		_T("SS-TCRDEM.RNG_CONT"), 			_T(""), 			eControlStatus },
	{ IDC_STATIC_COHVAL, 		_T("SS-TCRDEM.TRAPON_RATIO"), 		_T(""), 			eControlStatus },
};

//static connst CColorStatic[] = {}

// CTabSmeasView20 �_�C�A���O

class CTabSmeasView20 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabSmeasView20)

public:
	CTabSmeasView20(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CTabSmeasView20();

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
