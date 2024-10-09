#pragma once
#include "afxwin.h"
#include "RdnsAgcDetail.h"
#include "RdnsRunLog.h"
#include "atltime.h"
#include "RdnsAgcDetailStep.h"
#include "CidSelect.h"
#include "const.h"
#include "afxdtctl.h"
#include "NumericEdit.h"
#include "DetailViewDlg.h"
#include "RdnsChkResult.h"
#include "DateTimeCtrlNoToday.h"

// CRdnsChk54 ダイアログ

// メッセージ
enum{
	eRdnsChk54_ResultOpen = (WM_USER + 100),
};

class CRdnsChk54 : public CDialogBase
{
	DECLARE_DYNAMIC(CRdnsChk54)

public:
	CRdnsChk54(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CRdnsChk54();

	// ダイアログ データ
	enum { IDD = IDD_DLG_RDNSCHK54 };

	//-----------------------------------------------------------------------------
	//	定義
	//-----------------------------------------------------------------------------
public:
	typedef struct {
		int count;
		BOOL bResult;
	}stResult;

	typedef struct {
		stResult agc_x;
		stResult agc_ka;
		stResult delay;
		stResult tlm;
		stResult cmd;
	}stVResult;


	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
protected:
	CRdnsRunLog* m_LogDlg;
	CString m_strSatellite;
	CString m_xrx2_lbw;
	CString m_strKa_lbw;

	long l_satno;
	long l_useband;                 // 使用周波数帯（S,X,S/X) 
	long l_rarr_band;               // 使用周波数帯（S,X,S/X) 
	long l_level;                   // 近地球／深宇宙 (STDN,DSN)

	long l_x_tx;					// X帯送信設備使用有無
	long l_x_dsntcr1;				// X帯受信機 DSN-MODEM1
	long l_ka_dsntcr1;				// Ka帯受信機 DSN-MODEM1

	int i_bitrate_x2;				// X BITRATE
	int i_bitrate_ka;				// Ka BITRATE

	HANDLE	m_hThread;
	BOOL	m_SendFlg;
	vector<stSendCtrlMsg>	m_CtrlList;

	int		m_TimeSpan;

	bool	m_ThreadAbort;
	bool	m_ThreadRun;
	bool	m_RdnsChkAbort;

	// 詳細表示画面
	stVResult m_VResult;

	CString		m_strDay;

	vector<stCalibPlan>	m_AgcDelayPlanList;
	vector<stCalibPlan>	m_TlmPlanList;
	vector<stCalibPlan>	m_CmdPlanList;

	BOOL		m_bNewPlan;
	CString		m_PassID;
	DWORD		m_GetForm;
	CTime		m_StartTime;

	vector<CString> m_X_RX2LBList;
	vector<CString> m_Ka_LBList;

	CTime	m_AgcXResFileTime;
	CTime	m_AgcKaResFileTime;
	CTime	m_DelayResFileTime;
	CTime	m_TlmResFileTime;
	CTime	m_CmdResFileTime;

	CRdnsChkResult* m_ResultDlg;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	BOOL	UpdateRdnsChkDlg();
	void	CloseLogDlg(bool bDestroy = TRUE);
	void	SetSatelliteName(CString name) { m_strSatellite = name; }
	void	SetPassId(CString str)	{ m_PassID = str; }
	void	SetGetForm(DWORD form)	{ m_GetForm = form; }
	void	SetType(UINT type)		{
		if (type == 1)
		{
			m_bNewPlan = TRUE;
		}
		else
		{
			m_bNewPlan = FALSE;
		}
	}
	void	SetStartTime(CTime time) { m_StartTime = time; }

	void	ExitThread();

protected:
	int GetEqSatDBData(CString satname);
	void GetCurrentLWB();
	void SetWindowEnable(CString satname);

	static UINT WINAPI SendCtrlThread(LPVOID pParam);

	void ChangeRdyChkEndTime();
	void SetRdyChkEndTime();

	bool ThreadAbort() { return m_ThreadAbort; }
	bool RdnsChkAbort() { return m_RdnsChkAbort; }
	void RdnsChkAbortClear() { m_RdnsChkAbort = false; }

	void MonitoringRdnsRunState();
	void ResultFileOpen(int type);

	void	GetSetting();
	int		GetClibTime(int type);
	void	SetSetting(vector<stCalibPlan>& agclist, vector<stCalibPlan>& tlmlist, vector<stCalibPlan>& cmdlist);
	void	InitTimer();
	bool RegistPlan(bool bChkPassId);

	void CreateExctrRouteCombo();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CComboBoxEx2 m_SatelliteSelCmb;
	CNumericEdit m_SatelliteDistanceEdit;
	CMFCMaskedEdit m_TlmPathNoEdit;
	CMFCMaskedEdit m_CmdPathNoEdit;
	CString m_strTlmPassID;
	CString m_strCmdPassID;
	BOOL m_AgcChkValue;
	BOOL m_DlyChkValue;
	BOOL m_DlyXXChkValue;
	BOOL m_DlyXXRChkValue;
	BOOL m_DlyXKaTCRChkValue;
	BOOL m_DlyXKaTCRRChkValue;
	BOOL m_TLMChkValue;
	BOOL m_TLMXChkValue;
	BOOL m_CmdChkValue;
	BOOL m_CmdXChkValue;
	CTime m_PeriodExecStartDay;
	CTime m_PeriodExecStartTime;
	CTime m_PeriodExecEndDay;
	CTime m_PeriodExecEndTime;
	CString m_strXRX2LoopBand;
	CString m_strTlmBRX;
	CString m_strTlmBRKa;
	CString m_strCmdBRX;
	CString m_strKaTCRLoopBand;
	BOOL m_AgcXRX2ChkValue;
	BOOL m_AgcKaTCRChkValue;
	CButton m_CidSelBtn;
	CButton m_AGC_X_RX2Ctrl;
	CEdit m_AGC_X_RX2_LBCtrl;
	CButton m_AGC_X_RX2_LBBCtrl;
	CButton m_AGC_X_RX2_SBCtrl;
	CButton m_AGC_Ka_TCRCtrl;
	CEdit m_AGC_Ka_TCR_LBCtrl;
	CButton m_AGC_Ka_TCR_LBBCtrl;
	CButton m_AGC_Ka_TCR_SBCtrl;
	CButton m_Delay_XXCtrl;
	CButton m_Delay_XXRCtrl;
	CButton m_Delay_XKaTCRCtrl;
	CButton m_Delay_XKaTCRRCtrl;
	CButton m_TLM_XCtrl;
	CButton m_CMD_XCtrl;
	CButton m_TLM_KaCtrl;
	BOOL m_TLMKaChkValue;
	CButton m_AbortBtnCtr;
	// 校正診断開始日時
	CDateTimeCtrlNoToday m_RdnsChkStartDTCtrl;
	// 校正診断終了日時
	CDateTimeCtrlNoToday m_RdnsChkEndDTCtrl;
	CDateTimeCtrlNoToday m_RdnsChkStartTTCtrl;
	CDateTimeCtrlNoToday m_RdnsChkEndTTCtrl;
	CComboBox m_ExctrRouteCombo;

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuClose();
	afx_msg void OnBnClickedRdns54Xrx2Btn();
	afx_msg void OnBnClickedRdns54KaLoopBtn();
	afx_msg void OnBnClickedRdns54RealtimeRunbtn();
	afx_msg void OnBnClickedRdns54RealtimeStopbtn();
	afx_msg void OnBnClickedRdns54PeriodEntrybtn();
	afx_msg void OnBnClickedRdns54CidBtn();
	afx_msg void OnBnClickedRdns54Xrx2StepBtn();
	afx_msg void OnBnClickedRdns54KaStepBtn();
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeRdns54SselCmb();
	afx_msg void OnBnClickedRdns54Xrx2Chkbtn();
	afx_msg void OnBnClickedRdns54KaChkbtn();
	afx_msg void OnBnClickedRdns54DelayChkbtn();
	afx_msg void OnBnClickedRdns54DSreqpmXxChkbtn();
	afx_msg void OnBnClickedRdns54DSreqpmXxrChkbtn();
	afx_msg void OnBnClickedRdns54DSreqpmXKaChkbtn();
	afx_msg void OnBnClickedRdns54DSreqpmXKarChkbtn();
	afx_msg void OnBnClickedRdns54TlmChkbtn();
	afx_msg void OnBnClickedRdns54TlmRXtlmin();
	afx_msg void OnBnClickedRdns54TlmRKatlmin();
	afx_msg void OnBnClickedRdns54CmdChkbtn();
	afx_msg void OnBnClickedRdns54CmdCmdXChkbtn();
	afx_msg void OnBnClickedRdns54AgcChkbtn();
	afx_msg void OnDtnDatetimechangeRdns54PeriodStimeDayDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeRdns54PeriodStimeTimeDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
