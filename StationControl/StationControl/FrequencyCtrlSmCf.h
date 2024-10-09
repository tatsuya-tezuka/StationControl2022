#pragma once
#include "atltime.h"
#include "afxdtctl.h"


// CFrequencyCtrlSmCf ダイアログ

class CFrequencyCtrlSmCf : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrlSmCf)

public:
	CFrequencyCtrlSmCf(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CFrequencyCtrlSmCf();

// ダイアログ データ
	enum { IDD = IDD_DLG_FREQ_CTRL_SML_CF };

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
	// コマンドサブキャリア周波数
	CString m_strCmdFreq;
	afx_msg void OnBnClickedFreqCtrlSmlCfDpcalcBtn();
	afx_msg void OnBnClickedFreqCtrlSmlCfDpviewBtn();
	afx_msg void OnBnClickedFreqCtrlSmlCfCalcBtn();
	// 指定時刻（Day）
	CTime m_SpecDay;
	// 指定時刻（Time）
	CTime m_SpecTime;
	CDateTimeCtrl m_SpecDayDTCtrl;
	virtual BOOL OnInitDialog();
};
