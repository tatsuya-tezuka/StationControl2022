#pragma once
#include "afxwin.h"
#include "atltime.h"
#include "afxdtctl.h"


// CFrequencyCtrlFcChg ダイアログ

static const CString mCtrlFcChgTxDopp[] = {
	_T("動的補償"),
	_T("静的補償(センター)"),
	_T("静的補償(ノミナル)"),
	_T("静的補償(AOSベストロック)"),
	_T("")
};

class CFrequencyCtrlFcChg : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrlFcChg)

public:
	CFrequencyCtrlFcChg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CFrequencyCtrlFcChg();

// ダイアログ データ
	enum { IDD = IDD_DLG_FREQ_CTRL_FCCHG };

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
	CWnd*		mCallWnd;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	void	Initialize();
	void	SetCallWnd(CWnd* p)	{ mCallWnd = p; }
	virtual BOOL UpdateDlg();

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	// 探査機
	CComboBox m_SpacecraftCmb;
	// 送信ドップラ補償
	CComboBox m_TDoppCmb;
	// パス開始日時（Day）
	CTime m_PassStartDay;
	// パス開始日時（Time）
	CTime m_PassStartTime;
	// パス終了日時（Day）
	CTime m_PassEndDay;
	// パス終了日時（Time）
	CTime m_PassEndTime;
	CDateTimeCtrl m_PassStartDTCtrl;
	CDateTimeCtrl m_PassEndDTCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedFreqCtrlFcchgAntfcBtn();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CButton m_AntPredBtnCtrl;
	afx_msg void OnBnClickedFreqCtrlFcchgBtn();
	afx_msg void OnBnClickedFreqCtrlOpeBtn();
	afx_msg void OnCbnSelchangeFreqCtrlFcchgSpcrCmb();
	afx_msg void OnCbnSelchangeFreqCtrlFcchgTdpCmb();
	int m_nTxDopp;
	afx_msg void OnDtnDatetimechangeFreqCtrlFcchgPasstDayDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeFreqCtrlFcchgPasstTimeDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeFreqCtrlFcchgPasetDayDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeFreqCtrlFcchgPasetTimeDp(NMHDR *pNMHDR, LRESULT *pResult);
};
