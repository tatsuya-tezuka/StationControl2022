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

// CRdnsChk64 �_�C�A���O

// ���b�Z�[�W
enum{
	eRdnsChk64_ResultOpen = (WM_USER + 100),
};

class CRdnsChk64 : public CDialogBase
{
	DECLARE_DYNAMIC(CRdnsChk64)

public:
	CRdnsChk64(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CRdnsChk64();

	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_RDNSCHK64 };

	//-----------------------------------------------------------------------------
	//	��`
	//-----------------------------------------------------------------------------
public:
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
	CString m_xrx2_lbw;
	CString m_xx_tcr_lbw;

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
	int i_bitrate_x2;				// X2 BITRATE

	int i_tlm_s_frm;				// TLM S�� �t���[�����iINIT.SS-TCRDEM�ASS-TCRDEM.FRAME_WORD_LEN�j
	int i_tlm_s_lck;				// TLM S�� ���b�N�I���J�E���g�iINIT.SS-TCRDEM�ASS-TCRDEM.SEACH_TO_LOCK�j
	int i_tlm_x2_frm;				// TLM X�ѐV�n �t���[�����iINIT.DSNTCR-DEM�ADSNTCR.TLM_FRAME_LENGTH�j
	int i_tlm_x2_lck;				// TLM X�ѐV�n ���b�N�I���J�E���g�iINIT.DSNTCR-DEM�ADSNTCR.TO_LOCK�j

	int i_cmd_s_frm;				// CMD S�� �t���[�����iINIT.SS-TCRMOD�ASS-TCRMOD.FRAME_WORD_LEN�j
	int i_cmd_x2_frm;				// CMD X�ѐV�n �t���[�����iINIT.DSNTCR-MOD�ADSNTCR.SIM_FRAME_LENGTH�j


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

	vector<CString> m_X_RX2LBList;
	vector<CString> m_XX_TCRLBList;

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuClose();
	CComboBoxEx2 m_SatelliteSelCmb;
	CNumericEdit m_SatelliteDistanceEdit;
	afx_msg void OnBnClickedRdns64SstcrChkbtn();
	afx_msg void OnBnClickedRdns64Xrx2Btn();
	afx_msg void OnBnClickedRdns64XxtcrBtn();
	CMFCMaskedEdit m_TlmPathNoEdit;
	CMFCMaskedEdit m_CmdPathNoEdit;
	CString m_strTlmPassID;
	CString m_strCmdPassID;
	afx_msg void OnBnClickedRdns64RealtimeRunbtn();
	afx_msg void OnBnClickedRdns64RealtimeStopbtn();
	afx_msg void OnBnClickedRdns64PeriodEntrybtn();
	BOOL m_AgcChkValue;
	BOOL m_AgcSSTCRAChkValue;
	BOOL m_DlyChkValue;
	BOOL m_DlySSChkValue;
	BOOL m_DlyXXChkValue;
	BOOL m_DlySXChkValue;
	BOOL m_DlyXXRChkValue;
	BOOL m_DlyXXTCRChkValue;
	BOOL m_DlyXXTCRRChkValue;
	BOOL m_TLMChkValue;
	BOOL m_TLMSChkValue;
	BOOL m_TLMXChkValue;
	BOOL m_CmdChkValue;
	BOOL m_CmdSChkValue;
	BOOL m_CmdXChkValue;
	BOOL m_AgcSSTCRChkValue;
	virtual BOOL OnInitDialog();
	CTime m_PeriodExecStartDay;
	CTime m_PeriodExecStartTime;
	CTime m_PeriodExecEndDay;
	CTime m_PeriodExecEndTime;
	CString m_strXRX2LoopBand;
	CString m_strTlmBRS;
	CString m_strTlmBRX;
	CString m_strCmdBRS;
	CString m_strCmdBRX;
	CString m_strXXTCRLoopBand;
	afx_msg void OnBnClickedRdns64CidBtn();
	afx_msg void OnBnClickedRdns64Xrx2StepBtn();
	afx_msg void OnBnClickedRdns64XxtcrStepBtn();
	afx_msg void OnClose();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL m_AgcXRX2ChkValue;
	BOOL m_AgcXXTCRChkValue;
	// �Z���f�f�J�n����
	CDateTimeCtrlNoToday m_RdnsChkStartDTCtrl;
	// �Z���f�f�I������
	CDateTimeCtrlNoToday m_RdnsChkEndDTCtrl;
	CButton m_CidSelBtn;
	afx_msg void OnCbnSelchangeRdns64SselCmb();
	afx_msg void OnBnClickedRdns64SstcrAChkbtn();
	CButton m_AGC_SS_TCRCtrl;
	CButton m_AGC_SS_TCR_ACtrl;
	CButton m_AGC_X_RX2Ctrl;
	CEdit m_AGC_X_RX2_LBCtrl;
	CButton m_AGC_X_RX2_LBBCtrl;
	CButton m_AGC_X_RX2_SBCtrl;
	CButton m_AGC_XX_TCRCtrl;
	CEdit m_AGC_XX_TCR_LBCtrl;
	CButton m_AGC_XX_TCR_LBBCtrl;
	CButton m_AGC_XX_TCR_SBCtrl;
	CButton m_Delay_SSCtrl;
	CButton m_Delay_SXCtrl;
	CButton m_Delay_XXCtrl;
	CButton m_Delay_XXRCtrl;
	CButton m_Delay_XXTCRCtrl;
	CButton m_Delay_XXTCRRCtrl;
	CButton m_TLM_SCtrl;
	CButton m_TLM_XCtrl;
	CButton m_CMD_SCtrl;
	CButton m_CMD_XCtrl;
	CButton m_TLM_X2Ctrl;
	BOOL m_TLMX2ChkValue;
	CButton m_CMD_X2Ctrl;
	BOOL m_CmdX2ChkValue;
	CButton m_AbortBtnCtr;
	afx_msg void OnBnClickedRdns64Xrx2Chkbtn();
	afx_msg void OnBnClickedRdns64XxtcrChkbtn();
	afx_msg void OnBnClickedRdns64DelayChkbtn();
	afx_msg void OnBnClickedRdns64DSreqpmSsChkbtn();
	afx_msg void OnBnClickedRdns64DSreqpmXxChkbtn();
	afx_msg void OnBnClickedRdns64DSreqpmXxrChkbtn();
	afx_msg void OnBnClickedRdns64DSreqpmXxtChkbtn();
	afx_msg void OnBnClickedRdns64DSreqpmXxtrChkbtn();
	afx_msg void OnBnClickedRdns64TlmChkbtn();
	afx_msg void OnBnClickedRdns64TlmRStlmin();
	afx_msg void OnBnClickedRdns64TlmRXtlmin();
	afx_msg void OnBnClickedRdns64TlmRXtlmin2();
	afx_msg void OnBnClickedRdns64CmdChkbtn();
	afx_msg void OnBnClickedRdns64CmdCmdSChkbtn();
	afx_msg void OnBnClickedRdns64CmdCmdXChkbtn();
	afx_msg void OnBnClickedRdns64CmdCmdXChkbtn2();
	afx_msg void OnBnClickedRdns64AgcChkbtn();
	afx_msg void OnDtnDatetimechangeRdns64PeriodStimeDayDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeRdns64PeriodStimeTimeDp(NMHDR *pNMHDR, LRESULT *pResult);
	CDateTimeCtrlNoToday m_RdnsChkStartTTCtrl;
	CDateTimeCtrlNoToday m_RdnsChkEndTTCtrl;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
