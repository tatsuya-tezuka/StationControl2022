#pragma once
#include "atltime.h"
#include "afxdtctl.h"


// CFrequencyCtrlSmRf ダイアログ

class CFrequencyCtrlSmRf : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrlSmRf)

public:
	CFrequencyCtrlSmRf(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CFrequencyCtrlSmRf();

// ダイアログ データ
	enum { IDD = IDD_DLG_FREQ_CTRL_SML_RF };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
protected:

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	virtual BOOL UpdateDlg();

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	// 地上局受信周波数
	CString m_strRFreq;
	afx_msg void OnBnClickedFreqCtrlSmlRfDpcalcBtn();
	afx_msg void OnBnClickedFreqCtrlSmlRfDpviewBtn();
	afx_msg void OnBnClickedFreqCtrlSmlRfCalcBtn();
	// 指定時刻（Day）
	CTime m_SpecDay;
	// 指定時刻（Time）
	CTime m_SpecTime;
	CDateTimeCtrl m_SpecDayDTCtrl;
	virtual BOOL OnInitDialog();
};
