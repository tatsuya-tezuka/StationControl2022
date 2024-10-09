#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "FrequencyCtrlSmTf.h"
#include "FrequencyCtrlSmRf.h"
#include "FrequencyCtrlSm3W.h"
#include "FrequencyCtrlSmcf.h"
#include "atltime.h"
#include "afxdtctl.h"


// CFrequencyCtrlSm ダイアログ

class CFrequencyCtrlSm : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrlSm)

public:
	CFrequencyCtrlSm(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CFrequencyCtrlSm();

// ダイアログ データ
	enum { IDD = IDD_DLG_FREQ_CTRL_SML };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	typedef struct{
		CString		satname;		// 衛星名
		CString		predname;		// 予報値名
		CString		predfilepath;	// 予報値ファイルパス名
		time_t		passStart;		// パス開始時刻
		time_t		passStop;		// パス終了時刻
	} stSimData;
protected:

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
protected:
	CWnd*		mCallWnd;
	CString		mTargetPredFilePath;
private:
	CFrequencyCtrlSmTf	m_TabTf;
	CFrequencyCtrlSmRf	m_TabFf;	// (未使用）
	CFrequencyCtrlSm3W	m_Tab3W;
	CFrequencyCtrlSmCf	m_TabCf;	// (未使用）


	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	void	Initialize();
	void	SetCallWnd(CWnd* p)	{ mCallWnd = p; }
	bool	GetSimData(stSimData& data);
	virtual BOOL UpdateDlg();

protected:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	// 探査機
	CComboBox m_SpacecraftCtrl;
	afx_msg void OnBnClickedFreqCtrlSmlAntfcBtn();
	CTabCtrl m_SmlTabCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeDlgFreqCtrlSmlTab(NMHDR *pNMHDR, LRESULT *pResult);
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
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CButton m_AntPredBtnCtrl;
	afx_msg void OnCbnSelchangeFreqCtrlSmlSpcrCmb();
};
