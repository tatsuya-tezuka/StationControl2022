#pragma once
#include "afxwin.h"
#include "RdnsAgcDetail.h"
#include "RdnsRunLog.h"
#include "atltime.h"
#include "RdnsAgcDetailStep.h"
#include "CidSelect.h"
#include "afxdtctl.h"
#include "NumericEdit.h"
#include "DetailViewDlg.h"
#include "RdnsChkResult.h"
#include "DateTimeCtrlNoToday.h"


// CRdnsChk34 ダイアログ

// メッセージ
enum{
	eRdnsChk34_ResultOpen = (WM_USER + 100),
};

class CRdnsChk34 : public CDialogBase
{
	DECLARE_DYNAMIC(CRdnsChk34)

public:
	CRdnsChk34(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CRdnsChk34();

// ダイアログ データ
	enum { IDD = IDD_DLG_RDNSCHK34 };

	//-----------------------------------------------------------------------------
	//	定義
	//-----------------------------------------------------------------------------
protected:
	typedef struct {
		int count;
		BOOL bResult;
	}stResult;

	typedef struct {
		stResult agc_s;
		stResult agc_x;
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
	CString m_xx_tcr_lbw;

	long l_satno;
	long l_useband;                 // 使用周波数帯（S,X,S/X) 
	long l_rarr_band;               // 使用周波数帯（S,X,S/X) 
	long l_level;                   // 近地球／深宇宙 (STDN,DSN)

	long l_x_tx;					// Ｘ帯送信設備使用有無

	long l_s_rx;                    // S-RX 使用有無
	long l_sx_acqrx_s;              // S-ACQ 使用有無
	long l_sx_acqrx_x;              // X-ACQ 使用有無
	long l_x_rx;                    // X-RX 使用有無
	long l_x_qpskrx;                // X-HRX 使用有無
	long l_x_rx1;					// 臼田Ｘ帯受信機１
	long l_x_rx2;					// 臼田Ｘ帯受信機２

	int i_bitrate_s;				// S BITRATE
	int i_bitrate_x2;				// X BITRATE
	int i_bitrate_xqpsk;			// XQPSK BITRATE

	int i_tlm_s_frm;				// TLM S帯 フレーム長
	int i_tlm_s_lck;				// TLM S帯 ロックオンカウント
	int i_tlm_x_frm;				// TLM X帯新系 フレーム長
	int i_tlm_x_lck;				// TLM X帯新系 ロックオンカウント
	int i_tlm_xh_frm;				// TLM X帯高速 フレーム長
	int i_tlm_xh_lck;				// TLM X帯高速 ロックオンカウント

	int i_cmd_s_frm;				// CMD S帯 フレーム長
	int i_cmd_x2_frm;				// CMD X帯新系 フレーム長

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

	vector<CString> m_XX_TCRLBList;

	CTime	m_AgcSResFileTime;
	CTime	m_AgcXResFileTime;
	CTime	m_DelayResFileTime;
	CTime	m_TlmResFileTime;
	CTime	m_CmdResFileTime;

	CRdnsChkResult* m_ResultDlg;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
//	virtual BOOL UpdateDlg();
	BOOL UpdateRdnsChkDlg();
	void CloseLogDlg(bool bDestroy = TRUE);
	void SetSatelliteName(CString name) { m_strSatellite = name; }
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
	void SetShareMemCurrentLBW();
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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CComboBoxEx2 m_SatelliteSelCmb;
	CNumericEdit m_SatelliteDistanceEdit;
	BOOL m_AgcChkValue;
	BOOL m_AgcSSTCRChkValue;
	BOOL m_AgcSSTCRAChkValue;
	BOOL m_AgcSSTCRBChkValue;
	BOOL m_AgcSTRKRXChkValue;
	BOOL m_AgcSTRKRXAChkValue;
	BOOL m_AgcSTRKRXBChkValue;
	BOOL m_AgcSACQRXChkValue;
	BOOL m_AgcSACQRXAChkValue;
	BOOL m_AgcSACQRXBChkValue;
	BOOL m_AgcXTRKRXChkValue;
	BOOL m_AgcXTRKRXAChkValue;
	BOOL m_AgcXTRKRXBChkValue;
	BOOL m_AgcXACQRXChkValue;
	BOOL m_AgcXACQRXAChkValue;
	BOOL m_AgcXACQRXBChkValue;
	BOOL m_AgcXHRXChkValue;
	BOOL m_AgcXXTCRChkValue;
	BOOL m_DlyChkValue;
	BOOL m_DlySSChkValue;
	BOOL m_DlySXChkValue;
	BOOL m_DlyXXChkValue;
	BOOL m_DlyXXRChkValue;
	BOOL m_TLMChkValue;
	BOOL m_TLMSChkValue;
	BOOL m_TLMXChkValue;
	BOOL m_TLMXHChkValue;
	CMFCMaskedEdit m_TlmPathNoEdit;
	BOOL m_CmdChkValue;
	BOOL m_CmdSChkValue;
	BOOL m_CmdXChkValue;
	CMFCMaskedEdit m_CmdPathNoEdit;
	CString m_strTlmPassID;
	CString m_strCmdPassID;
	afx_msg void OnBnClickedRdns34SstcrChkbtn();
	afx_msg void OnBnClickedRdns34StrkrxChkbtn();
	afx_msg void OnBnClickedRdns34SaqcrxChkbtn();
	afx_msg void OnBnClickedRdns34XtrkrxChkbtn();
	afx_msg void OnBnClickedRdns34XaqcrxChkbtn();
	afx_msg void OnBnClickedRdns34XxtcrBtn();
	afx_msg void OnBnClickedRdns34RealtimeRunbtn();
	afx_msg void OnBnClickedRdns34RealtimeStopbtn();
	afx_msg void OnBnClickedRdns34PeriodEntrybtn();
	afx_msg void OnMenuClose();
	virtual BOOL OnInitDialog();
	CTime m_PeriodExecStartDay;
	CTime m_PeriodExecStartTime;
	CTime m_PeriodExecEndDay;
	CTime m_PeriodExecEndTime;
	CString m_strTlmBRS;
	CString m_strTlmBRX;
	CString m_strTlmBRH;
	CString m_strCmdBRS;
	CString m_strCmdBRX;
	CString m_strXXTCRLoopBand;
	afx_msg void OnBnClickedRdns34CidBtn();
	afx_msg void OnBnClickedRdns34XxtcrStepBtn();
	afx_msg void OnClose();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	// 校正診断開始日時
	CDateTimeCtrlNoToday m_RdnsChkStartDTCtrl;
	// 校正診断終了日時
	CDateTimeCtrlNoToday m_RdnsChkEndDTCtrl;
	CButton m_CidSelBtn;
	afx_msg void OnCbnSelchangeRdns34SselCmb();
	afx_msg void OnBnClickedRdns34SstcrAChkbtn();
	afx_msg void OnBnClickedRdns34SstcrBChkbtn();
	afx_msg void OnBnClickedRdns34StrkrxAChkbtn();
	afx_msg void OnBnClickedRdns34StrkrxBChkbtn();
	afx_msg void OnBnClickedRdns34SaqcrxAChkbtn();
	afx_msg void OnBnClickedRdns34SaqcrxBChkbtn();
	afx_msg void OnBnClickedRdns34XtrkrxAChkbtn();
	afx_msg void OnBnClickedRdns34XtrkrxBChkbtn();
	afx_msg void OnBnClickedRdns34XaqcrxAChkbtn();
	afx_msg void OnBnClickedRdns34XaqcrxBChkbtn();
	CButton m_AGC_SS_TCRCtrl;
	CButton m_AGC_SS_TCR_ACtrl;
	CButton m_AGC_SS_TCR_BCtrl;
	CButton m_AGC_S_TRKRXCtrl;
	CButton m_AGC_S_TRKRX_ACtrl;
	CButton m_AGC_S_TRKRX_BCtrl;
	CButton m_AGC_S_ACQRXCtrl;
	CButton m_AGC_S_ACQRX_ACtrl;
	CButton m_AGC_S_ACQRX_BCtrl;
	CButton m_AGC_X_TRKRXCtrl;
	CButton m_AGC_X_TRKRX_ACtrl;
	CButton m_AGC_X_TRKRX_BCtrl;
	CButton m_AGC_X_ACQRXCtrl;
	CButton m_AGC_X_ACQRX_ACtrl;
	CButton m_AGC_X_ACQRX_BCtrl;
	CButton m_AGC_X_HRXCtrl;
	CButton m_AGC_XX_TCRCtrl;
	CEdit m_AGC_XX_TCR_LBCtrl;
	CButton m_AGC_XX_TCR_LBBCtrl;
	CButton m_AGC_XX_TCR_SBCtrl;
	CButton m_Delay_SSCtrl;
	CButton m_Delay_SXCtrl;
	CButton m_Delay_XXCtrl;
	CButton m_Delay_XXRCtrl;
	CButton m_TLM_SCtrl;
	CButton m_TLM_XCtrl;
	CButton m_TLM_X_HRXCtrl;
	CButton m_CMD_SCtrl;
	CButton m_CMD_XCtrl;
	CButton m_AbortBtnCtr;
	afx_msg void OnDtnDatetimechangeRdns34PeriodStimeDayDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeRdns34PeriodStimeTimeDp(NMHDR *pNMHDR, LRESULT *pResult);
	CDateTimeCtrlNoToday m_RdnsChkStartTTCtrl;
	CDateTimeCtrlNoToday m_RdnsChkEndTTCtrl;
	afx_msg void OnBnClickedRdns34XhrxChkbtn();
	afx_msg void OnBnClickedRdns34XxtcrChkbtn();
	afx_msg void OnBnClickedRdns34DSreqpmSsChkbtn();
	afx_msg void OnBnClickedRdns34DSreqpmSxChkbtn();
	afx_msg void OnBnClickedRdns34DSreqpmXxChkbtn();
	afx_msg void OnBnClickedRdns34DSreqpmXxrChkbtn();
	afx_msg void OnBnClickedRdns34TlmRStlmin();
	afx_msg void OnBnClickedRdns34TlmRXtlmin();
	afx_msg void OnBnClickedRdns34TlmRXhrtlmin();
	afx_msg void OnBnClickedRdns34CmdCmdSChkbtn();
	afx_msg void OnBnClickedRdns34CmdCmdXChkbtn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRdns34AgcChkbtn();
	afx_msg void OnBnClickedRdns34DelayChkbtn();
	afx_msg void OnBnClickedRdns34TlmChkbtn();
	afx_msg void OnBnClickedRdns34CmdChkbtn();
};
