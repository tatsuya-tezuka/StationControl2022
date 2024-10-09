#pragma once
#include "afxwin.h"
#include "RdnsAgcDetail.h"
#include "RdnsRunLog.h"
#include "atltime.h"
#include "CidSelect.h"
#include "afxdtctl.h"
#include "NumericEdit.h"
#include "DetailViewDlg.h"
#include "RdnsChkResult.h"
#include "DateTimeCtrlNoToday.h"


// CRdnsChk20 �_�C�A���O

// ���b�Z�[�W
enum{
	eRdnsChk20_ResultOpen = (WM_USER + 100),
};

class CRdnsChk20 : public CDialogBase
{
	DECLARE_DYNAMIC(CRdnsChk20)

public:
	CRdnsChk20(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CRdnsChk20();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_RDNSCHK20 };

	//-----------------------------------------------------------------------------
	//	��`
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
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
protected:
	CRdnsRunLog* m_LogDlg;
	CString m_strSatellite;

	long l_satno;
	long l_useband;                 // �g�p���g���сiS,X,S/X) 
	long l_rarr_band;               // �g�p���g���сiS,X,S/X) 
	long l_level;                   // �ߒn���^�[�F�� (STDN,DSN)

	long l_x_tx;					// �w�ё��M�ݔ��g�p�L��

	long l_s_rx;                    // S-RX �g�p�L��
	long l_sx_acqrx_s;              // S-ACQ �g�p�L��
	long l_sx_acqrx_x;              // X-ACQ�g�p�L��
	long l_x_rx;                    // X-RX �g�p�L��
	long l_x_qpskrx;                // X-HRX �g�p�L��
	long l_x_rx1;					// �P�c�w�ю�M�@�P
	long l_x_rx2;					// �P�c�w�ю�M�@�Q

	int i_bitrate_s;				// S BITRATE
	int i_bitrate_x;				// X BITRATE
	int i_bitrate_xqpsk;			// XQPSK BITRATE

	int i_tlm_s_frm;				// TLM S�� �t���[����
	int i_tlm_s_lck;				// TLM S�� ���b�N�I���J�E���g
	int i_tlm_xh_frm;				// TLM X�э��� �t���[����
	int i_tlm_xh_lck;				// TLM X�э��� ���b�N�I���J�E���g

	int i_cmd_s_frm;				// CMD S�� �t���[����

	HANDLE	m_hThread;
	BOOL	m_SendFlg;
	vector<stSendCtrlMsg>	m_CtrlList;

	int		m_TimeSpan;

	bool	m_ThreadAbort;
	bool	m_ThreadRun;
	bool	m_RdnsChkAbort;

	// �ڍו\�����
	stVResult m_VResult;

	CString		m_strDay;

	vector<stCalibPlan>	m_AgcDelayPlanList;
	vector<stCalibPlan>	m_TlmPlanList;
	vector<stCalibPlan>	m_CmdPlanList;

	BOOL		m_bNewPlan;
	CString		m_PassID;
	DWORD		m_GetForm;
	CTime		m_StartTime;

	CTime	m_AgcSResFileTime;
	CTime	m_AgcXResFileTime;
	CTime	m_DelayResFileTime;
	CTime	m_TlmResFileTime;
	CTime	m_CmdResFileTime;

	CRdnsChkResult* m_ResultDlg;

	//-----------------------------------------------------------------------------
	//	�����o�֐�
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuClose();
	CComboBoxEx2 m_SatelliteSelCmb;
	CNumericEdit m_SatelliteDistanceEdit;
	BOOL m_AgcChkValue;
	BOOL m_AgcSACQChkValue;
	BOOL m_AgcSACQAChkValue;
	BOOL m_AgcSACQBChkValue;
	BOOL m_AgcSTCRChkValue;
	BOOL m_AgcSTCRAChkValue;
	BOOL m_AgcSTCRBChkValue;
	BOOL m_AgcSDETChkValue;
	BOOL m_AgcSDETAChkValue;
	BOOL m_AgcSDETBChkValue;
	BOOL m_AgcSXTCRChkValue;
	BOOL m_AgcSXTCRAChkValue;
	BOOL m_AgcSXTCRBChkValue;
	BOOL m_AgcXHRXChkValue;
	BOOL m_DlyChkValue;
	BOOL m_DlySSChkValue;
	BOOL m_DlySXChkValue;
	BOOL m_TLMChkValue;
	BOOL m_TLMSChkValue;
	BOOL m_TLMXChkValue;
	BOOL m_TLMXHChkValue;
	CMFCMaskedEdit m_TlmPathNoEdit;
	BOOL m_CmdChkValue;
	BOOL m_CmdOut1ChkValue;
	CMFCMaskedEdit m_CmdPathNoEdit;
	CString m_strTlmPassID;
	CString m_strCmdPassID;
	afx_msg void OnBnClickedRdns20SacqChkbtn();
	afx_msg void OnBnClickedRdns20StcrChkbtn();
	afx_msg void OnBnClickedRdns20SdetChkbtn();
	afx_msg void OnBnClickedRdns20SxtcrChkbtn();
	afx_msg void OnBnClickedRdns20RealtimeRunbtn();
	afx_msg void OnBnClickedRdns20RealtimeStopbtn();
	afx_msg void OnBnClickedRdns20PeriodEntrybtn();
	virtual BOOL OnInitDialog();
	CTime m_PeriodExecStartDay;
	CTime m_PeriodExecStartTime;
	CTime m_PeriodExecEndDay;
	CTime m_PeriodExecEndTime;
	CString m_strTlmBRS;
	CString m_strTlmBRX;
	CString m_strTlmBRH;
	CString m_strCmdBRS;
	afx_msg void OnBnClickedRdns20CidBtn();
	afx_msg void OnClose();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	// �Z���f�f�J�n����
	CDateTimeCtrlNoToday m_RdnsChkStartDTCtrl;
	// �Z���f�f�I������
	CDateTimeCtrlNoToday m_RdnsChkEndDTCtrl;
	CButton m_CidSelBtn;
	afx_msg void OnCbnSelchangeRdns20SselCmb();
	afx_msg void OnBnClickedRdns20SacqAChkbtn();
	afx_msg void OnBnClickedRdns20SacqBChkbtn();
	afx_msg void OnBnClickedRdns20StcrAChkbtn();
	afx_msg void OnBnClickedRdns20StcrBChkbtn();
	afx_msg void OnBnClickedRdns20SdetAChkbtn();
	afx_msg void OnBnClickedRdns20SdetBChkbtn();
	afx_msg void OnBnClickedRdns20SxtcrAChkbtn();
	afx_msg void OnBnClickedRdns20SxtcrBChkbtn();
	CButton m_AGC_S_ACQCtrl;
	CButton m_AGC_S_ACQ_ACtrl;
	CButton m_AGC_S_ACQ_BCtrl;
	CButton m_AGC_S_TCRCtrl;
	CButton m_AGC_S_TCR_ACtrl;
	CButton m_AGC_S_TCR_BCtrl;
	CButton m_AGC_S_DETCtrl;
	CButton m_AGC_S_DET_ACtrl;
	CButton m_AGC_S_DET_BCtrl;
	CButton m_AGC_SX_TCRCtrl;
	CButton m_AGC_SX_TCR_ACtrl;
	CButton m_AGC_SX_TCR_BCtrl;
	CButton m_AGC_X_HRXCtrl;
	CButton m_Delay_SSCtrl;
	CButton m_Delay_SXCtrl;
	CButton m_TLM_SCtrl;
	CButton m_TLM_XCtrl;
	CButton m_TLM_X_HRXCtrl;
	CButton m_CMD_OUT1Ctrl;
	CButton m_AbortBtnCtr;
	afx_msg void OnDtnDatetimechangeRdns20PeriodStimeDayDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeRdns20PeriodStimeTimeDp(NMHDR *pNMHDR, LRESULT *pResult);
	CDateTimeCtrlNoToday m_RdnsChkStartTTCtrl;
	CDateTimeCtrlNoToday m_RdnsChkEndTTCtrl;
	afx_msg void OnBnClickedRdns20XhrxChkbtn();
	afx_msg void OnBnClickedRdns20DSreqpmSsChkbtn();
	afx_msg void OnBnClickedRdns20DSreqpmSxChkbtn();
	afx_msg void OnBnClickedRdns20TlmRStlmin();
	afx_msg void OnBnClickedRdns20TlmRXtlmin();
	afx_msg void OnBnClickedRdns20TlmRXhrtlmin();
	afx_msg void OnBnClickedRdns20CmdCmd1Chkbtn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRdns20AgcChkbtn();
	afx_msg void OnBnClickedRdns20DelayChkbtn();
	afx_msg void OnBnClickedRdns20TlmChkbtn();
	afx_msg void OnBnClickedRdns20CmdChkbtn();
};
