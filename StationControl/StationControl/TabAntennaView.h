#pragma once
#include "afxwin.h"
#include "afxmaskededit.h"


static const stControlParam mAntennaParam[] = {
	{ IDC_BUTTON_AZ_DRIVE, 			_T("ANT.AZ_DRIVE"), 	_T("ANT.AZ_DRIVE"), 	eControlStatus },		// AZ DRIVE
	{ IDC_BUTTON_EL_DRIVE, 			_T("ANT.EL_DRIVE"), 	_T("ANT.EL_DRIVE"), 	eControlStatus },		// EL DRIVE
	{ IDC_BUTTON_AZ_OFFSET, 		_T("ANT.AZ_OFFSET"),	_T("ANT.AZ_OFFSET"),	eControlDouble },		// AZ OFFSET
	{ IDC_BUTTON_EL_OFFSET,			_T("ANT.EL_OFFSET"),	_T("ANT.EL_OFFSET"),	eControlDouble },		// EL OFFSET
	{ IDC_BUTTON_MODE, 				_T("ANT.DRIVE_MODE"),	_T("ANT.DRIVE_MODE"),	eControlStatus },		// MODE
	{ IDC_BUTTON_TIME,				_T("ANT.TIME_OFFSET"),	_T("ANT.TIME_OFFSET"),	eControlTime },			// TIME OFFSET
	{ IDC_MFCMASKEDEDIT_AZ_ANGLE, 	_T("ANT.REAL_AZ"),		_T(""),					eControlDouble },		// AZ ANGLE
	{ IDC_MFCMASKEDEDIT_EL_ANGLE, 	_T("ANT.REAL_EL"), 		_T(""), 				eControlDouble },		// EL ANGLE
};

// CTabAntennaView ダイアログ

class CTabAntennaView : public CDialogEx
{
	DECLARE_DYNCREATE(CTabAntennaView)

public:
	CTabAntennaView(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabAntennaView();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_TAB_ANTENNA };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

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
	CColorEdit m_edAzVal;
	CColorEdit m_edElVal;
	afx_msg void OnBnClickedButtonAzDrive();
	afx_msg void OnBnClickedButtonElDrive();
	afx_msg void OnBnClickedButtonAzOffset();
	afx_msg void OnBnClickedButtonElOffset();
	afx_msg void OnBnClickedButtonMode();
	afx_msg void OnBnClickedButtonTime();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
	CColorStatic mAzAngle;
	CColorStatic mElAngle;
};
