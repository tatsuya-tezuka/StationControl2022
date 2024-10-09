/*============================================================================*/
/*! RdnsChk20.cpp

-# �v�惌�f�B�l�X�`�F�b�N20m���
*/
/*============================================================================*/
// RdnsChk20.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "RdnsChk20.h"
#include "afxdialogex.h"
#include "HookMsgBox.h"

// timeGetTime�g�p
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )


#define CID_NA	_T("N/A")
#define MAC_MAX     256

// AGC�Z��
#define CTRL_AGC_S_ACQ_A			_T("CTRL.RDYCHK_AGC SX-ACQRX_S_A@%s@%s,%s;")	// S-ACQRX ACh
#define CTRL_AGC_S_ACQ_A_CANCEL		_T("CTRL.RDYCHK_AGC_CANCEL SX-ACQRX_S_A,%s;")

#define CTRL_AGC_S_ACQ_B			_T("CTRL.RDYCHK_AGC SX-ACQRX_S_B@%s@%s,%s;")	// S-ACQRX BCh
#define CTRL_AGC_S_ACQ_B_CANCEL		_T("CTRL.RDYCHK_AGC_CANCEL SX-ACQRX_S_B,%s;")

#define CTRL_AGC_S_ACQ_AB			_T("CTRL.RDYCHK_AGC SX-ACQRX_S_AB@%s@%s,%s;")	// S-ACQRX A+BCh
#define CTRL_AGC_S_ACQ_AB_CANCEL	_T("CTRL.RDYCHK_AGC_CANCEL SX-ACQRX_S_AB,%s;")

#define CTRL_AGC_SS_TCR_A			_T("CTRL.RDYCHK_AGC S-RX_A@%s@%s,%s;")			// SS-TCR ACh
#define CTRL_AGC_SS_TCR_A_CANCEL	_T("CTRL.RDYCHK_AGC_CANCEL S-RX_A,%s;")

#define CTRL_AGC_SS_TCR_B			_T("CTRL.RDYCHK_AGC S-RX_B@%s@%s,%s;")			// SS-TCR BCh
#define CTRL_AGC_SS_TCR_B_CANCEL	_T("CTRL.RDYCHK_AGC_CANCEL S-RX_B,%s;")

#define CTRL_AGC_SS_TCR_AB			_T("CTRL.RDYCHK_AGC S-RX_AB@%s@%s,%s;")			// SS-TCR A+BCh
#define CTRL_AGC_SS_TCR_AB_CANCEL	_T("CTRL.RDYCHK_AGC_CANCEL S-RX_AB,%s;")

#define CTRL_AGC_S_DET_A			_T("CTRL.RDYCHK_AGC SS-TCRDET_A@%s@%s,%s;")		// S-DET ACh
#define CTRL_AGC_S_DET_A_CANCEL		_T("CTRL.RDYCHK_AGC_CANCEL SS-TCRDET_A,%s;")

#define CTRL_AGC_S_DET_B			_T("CTRL.RDYCHK_AGC SS-TCRDET_B@%s@%s,%s;")		// S-DET ACh
#define CTRL_AGC_S_DET_B_CANCEL		_T("CTRL.RDYCHK_AGC_CANCEL SS-TCRDET_B,%s;")

#define CTRL_AGC_S_DET_AB			_T("CTRL.RDYCHK_AGC SS-TCRDET_AB@%s@%s,%s;")	// S-DET ACh
#define CTRL_AGC_S_DET_AB_CANCEL	_T("CTRL.RDYCHK_AGC_CANCEL SS-TCRDET_AB,%s;")

#define CTRL_AGC_SX_TCR_A			_T("CTRL.RDYCHK_AGC X-RX_A@%s@%s,%s;")			// SX-TCR ACh
#define CTRL_AGC_SX_TCR_A_CANCEL	_T("CTRL.RDYCHK_AGC_CANCEL X-RX_A,%s;")

#define CTRL_AGC_SX_TCR_B			_T("CTRL.RDYCHK_AGC X-RX_B@%s@%s,%s;")			// SX-TCR BCh
#define CTRL_AGC_SX_TCR_B_CANCEL	_T("CTRL.RDYCHK_AGC_CANCEL X-RX_B,%s;")

#define CTRL_AGC_SX_TCR_AB			_T("CTRL.RDYCHK_AGC X-RX_AB@%s@%s,%s;")			// SX-TCR A+BCh
#define CTRL_AGC_SX_TCR_AB_CANCEL	_T("CTRL.RDYCHK_AGC_CANCEL X-RX_AB,%s;")

#define CTRL_AGC_X_HRX_A			_T("CTRL.RDYCHK_AGC X-QPSKRX_A@%s@%s,%s;")		// X-HRX
#define CTRL_AGC_X_HRX_A_CANCEL		_T("CTRL.RDYCHK_AGC_CANCEL X-QPSKRX_A,%s;")

// �Ǔ��f�B���C�l�v��
#define CTRL_RNG_SS					_T("CTRL.RDYCHK_RNG SS@%s@%s,%s;")				// S/S
#define CTRL_RNG_SS_CANCEL			_T("CTRL.RDYCHK_RNG_CANCEL SS,%s;")

#define CTRL_RNG_SX					_T("CTRL.RDYCHK_RNG SX@%s@%s,%s;")				// S/X
#define CTRL_RNG_SX_CANCEL			_T("CTRL.RDYCHK_RNG_CANCEL SX,%s;")

// �e�����g���`���`�F�b�N
#define CTRL_TLM_S_TLMIN			_T("CTRL.RDYCHK_TLM S@%s@%s@%d@%s@%s,%s;")		// S-TLMIN
#define CTRL_TLM_S_TLMIN_CANCEL		_T("CTRL.RDYCHK_TLM_CANCEL S,%s;")

#define CTRL_TLM_X_TLMIN			_T("CTRL.RDYCHK_TLM X@%s@%s@%d@%s@%s,%s;")		// X-TLMIN
#define CTRL_TLM_X_TLMIN_CANCEL		_T("CTRL.RDYCHK_TLM_CANCEL X,%s;")

#define CTRL_TLM_X_HRXTLMIN			_T("CTRL.RDYCHK_TLM QPSK@%s@%s@%d@%s@%s,%s;")	// X-HRXTLMIN
#define CTRL_TLM_X_HRXTLMIN_CANCEL	_T("CTRL.RDYCHK_TLM_CANCEL QPSK,%s;")

// �R�}���h�`���`�F�b�N
#define CTRL_CMD_S_CMDOUT			_T("CTRL.RDYCHK_CMD 1@%s@%s@%d@%s@%s,%s;")		// S-CMD OUT
#define CTRL_CMD_S_CMDOUT_CANCEL	_T("CTRL.RDYCHK_CMD_CANCEL 1,%s;")

#define CTRL_CMD_X_CMDOUT			_T("CTRL.RDYCHK_CMD 2@%s@%s@%d@%s@%s,%s;")		// X-CMD OUT
#define CTRL_CMD_X_CMDOUT_CANCEL	_T("CTRL.RDYCHK_CMD_CANCEL 2,%s;")


#define MAC_INIT_SSTCRDEM		_T("INIT.SS-TCRDEM")						// TLM S��
#define MAC_SSTCRDEM_FRM		_T("SS-TCRDEM.FRAME_WORD_LEN")				// TLM S�с@�t���[����
#define MAC_SSTCRDEM_LCK		_T("SS-TCRDEM.SEACH_TO_LOCK")				// TLM S�с@���b�N�I���J�E���g

#define MAC_INIT_X_HRX_DEM		_T("INIT.X-HRX-DEM")						// TLM X�э���
#define MAC_X_HRX_DEM_FRM		_T("X-HRX.DEMO_FRAME_LENGTH")				// TLM X�э����@�t���[����
#define MAC_X_HRX_DEM_LCK		_T("X-HRX.FRAME_SEARCH_TO_LOCK")			// TLM X�э����@���b�N�I���J�E���g

#define MAC_INIT_SSTCRMOD		_T("INIT.SS-TCRMOD")						// CMD S��
#define MAC_SSTCRMOD_FRM		_T("SS-TCRMOD.FRM_WORD_LEN")				// CMD S�с@�t���[����

#define MON_AGC_S_COND			_T("CTRL.AGC_S_COND")
#define MON_AGC_X_COND			_T("CTRL.AGC_X_COND")
#define MON_RNG_COND			_T("CTRL.RNG_COND")
#define MON_TLM_S_COND			_T("CTRL.TLMCHK_S_COND")
#define MON_TLM_X_COND			_T("CTRL.TLMCHK_X_COND")
#define MON_CMD_COND			_T("CTRL.CMDCHK_COND")

#define RES_TITLE				_T("�Z���f�f����")
#define RES_FILE_AGCS			_T("AGC_S_RES.txt")
#define RES_FILE_AGCX			_T("AGC_X_RES.txt")
#define RES_FILE_RNG			_T("DELAY_RES.txt")
#define RES_FILE_TLM			_T("TLM_RES.txt")
#define RES_FILE_CMD			_T("CMD_RES.txt")

#define TID_RDNSSTATE		1
#define INTERVAL_RDNSSTATE	5000

#define TID_RES_AGCS		2
#define TID_RES_AGCX		3
#define TID_RES_RNG			4
#define TID_RES_TLM			5
#define TID_RES_CMD			6
#define INTERVAL_RES		1000

// �Z�����ʎ擾�p�Ď���
#define RESULT_AGCS				_T("CTRL.RDYCHK_SAGC_RES")
#define RESULT_AGCX				_T("CTRL.RDYCHK_XAGC_RES")
#define RESULT_RNG				_T("CTRL.RDYCHK_DELAY_RES")
#define RESULT_TLM				_T("CTRL.RDYCHK_TLM_RES")
#define RESULT_CMD				_T("CTRL.RDYCHK_CMD_RES")

// CRdnsChk20 �_�C�A���O

IMPLEMENT_DYNAMIC(CRdnsChk20, CDialogBase)

/*============================================================================*/
/*! CRdnsChk20

-# �R���X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CRdnsChk20::CRdnsChk20(CWnd* pParent /*=NULL*/)
: CDialogBase(CRdnsChk20::IDD, pParent)
	, m_AgcChkValue(FALSE)
	, m_AgcSACQChkValue(FALSE)
	, m_AgcSACQAChkValue(FALSE)
	, m_AgcSACQBChkValue(FALSE)
	, m_AgcSTCRChkValue(FALSE)
	, m_AgcSTCRAChkValue(FALSE)
	, m_AgcSTCRBChkValue(FALSE)
	, m_AgcSDETChkValue(FALSE)
	, m_AgcSDETAChkValue(FALSE)
	, m_AgcSDETBChkValue(FALSE)
	, m_AgcSXTCRChkValue(FALSE)
	, m_AgcSXTCRAChkValue(FALSE)
	, m_AgcSXTCRBChkValue(FALSE)
	, m_AgcXHRXChkValue(FALSE)
	, m_DlyChkValue(FALSE)
	, m_DlySSChkValue(FALSE)
	, m_DlySXChkValue(FALSE)
	, m_TLMChkValue(FALSE)
	, m_TLMSChkValue(FALSE)
	, m_TLMXChkValue(FALSE)
	, m_TLMXHChkValue(FALSE)
	, m_CmdOut1ChkValue(FALSE)
	, m_PeriodExecStartDay(0)
	, m_PeriodExecStartTime(0)
	, m_PeriodExecEndDay(0)
	, m_PeriodExecEndTime(0)
	, m_strTlmBRS(_T(""))
	, m_strTlmBRX(_T(""))
	, m_strTlmBRH(_T(""))
	, m_strCmdBRS(_T(""))
{
	m_LogDlg = NULL;
	m_strSatellite = _T("");
	m_hThread = NULL;
	m_TimeSpan = 0;
	m_ThreadAbort = false;
	m_ThreadRun = false;
	m_PassID = _T("");
	m_strTlmPassID = _T("");
	m_strCmdPassID = _T("");
	m_bNewPlan = TRUE;
	m_ResultDlg = NULL;
	m_RdnsChkAbort = false;
}

/*============================================================================*/
/*! CRdnsChk20

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CRdnsChk20::~CRdnsChk20()
{
}

void CRdnsChk20::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RDNS20_SSEL_CMB, m_SatelliteSelCmb);
	DDX_Control(pDX, IDC_RDNS20_SDISTANCE_EDIT, m_SatelliteDistanceEdit);
	DDX_Check(pDX, IDC_RDNS20_AGC_CHKBTN, m_AgcChkValue);
	DDX_Check(pDX, IDC_RDNS20_SACQ_CHKBTN, m_AgcSACQChkValue);
	DDX_Check(pDX, IDC_RDNS20_SACQ_A_CHKBTN, m_AgcSACQAChkValue);
	DDX_Check(pDX, IDC_RDNS20_SACQ_B_CHKBTN, m_AgcSACQBChkValue);
	DDX_Check(pDX, IDC_RDNS20_STCR_CHKBTN, m_AgcSTCRChkValue);
	DDX_Check(pDX, IDC_RDNS20_STCR_A_CHKBTN, m_AgcSTCRAChkValue);
	DDX_Check(pDX, IDC_RDNS20_STCR_B_CHKBTN, m_AgcSTCRBChkValue);
	DDX_Check(pDX, IDC_RDNS20_SDET_CHKBTN, m_AgcSDETChkValue);
	DDX_Check(pDX, IDC_RDNS20_SDET_A_CHKBTN, m_AgcSDETAChkValue);
	DDX_Check(pDX, IDC_RDNS20_SDET_B_CHKBTN, m_AgcSDETBChkValue);
	DDX_Check(pDX, IDC_RDNS20_SXTCR_CHKBTN, m_AgcSXTCRChkValue);
	DDX_Check(pDX, IDC_RDNS20_SXTCR_A_CHKBTN, m_AgcSXTCRAChkValue);
	DDX_Check(pDX, IDC_RDNS20_SXTCR_B_CHKBTN, m_AgcSXTCRBChkValue);
	DDX_Check(pDX, IDC_RDNS20_XHRX_CHKBTN, m_AgcXHRXChkValue);
	DDX_Check(pDX, IDC_RDNS20_DELAY_CHKBTN, m_DlyChkValue);
	DDX_Check(pDX, IDC_RDNS20_D_SREQPM_SS_CHKBTN, m_DlySSChkValue);
	DDX_Check(pDX, IDC_RDNS20_D_SREQPM_SX_CHKBTN, m_DlySXChkValue);
	DDX_Check(pDX, IDC_RDNS20_TLM_CHKBTN, m_TLMChkValue);
	DDX_Check(pDX, IDC_RDNS20_TLM_R_STLMIN, m_TLMSChkValue);
	DDX_Check(pDX, IDC_RDNS20_TLM_R_XTLMIN, m_TLMXChkValue);
	DDX_Check(pDX, IDC_RDNS20_TLM_R_XHRTLMIN, m_TLMXHChkValue);
	DDX_Control(pDX, IDC_RDNS20_TLM_PATH_EDIT, m_TlmPathNoEdit);
	DDX_Check(pDX, IDC_RDNS20_CMD_CHKBTN, m_CmdChkValue);
	DDX_Check(pDX, IDC_RDNS20_CMD_CMD_1_CHKBTN, m_CmdOut1ChkValue);
	DDX_Control(pDX, IDC_RDNS20_CMD_PATH_EDIT, m_CmdPathNoEdit);
	DDX_DateTimeCtrl(pDX, IDC_RDNS20_PERIOD_STIME_DAY_DP, m_PeriodExecStartDay);
	DDX_DateTimeCtrl(pDX, IDC_RDNS20_PERIOD_STIME_TIME_DP, m_PeriodExecStartTime);
	DDX_DateTimeCtrl(pDX, IDC_RDNS20_PERIOD_ETIME_DAY_DP, m_PeriodExecEndDay);
	DDX_DateTimeCtrl(pDX, IDC_RDNS20_PERIOD_ETIME_TIME_DP, m_PeriodExecEndTime);
	DDX_Text(pDX, IDC_RDNS20_TLM_BR_S_EDIT, m_strTlmBRS);
	DDX_Text(pDX, IDC_RDNS20_TLM_BR_X_EDIT, m_strTlmBRX);
	DDX_Text(pDX, IDC_RDNS20_TLM_BR_H_EDIT, m_strTlmBRH);
	DDX_Text(pDX, IDC_RDNS20_CMD_BR_S_EDIT, m_strCmdBRS);
	DDX_Control(pDX, IDC_RDNS20_PERIOD_STIME_DAY_DP, m_RdnsChkStartDTCtrl);
	DDX_Control(pDX, IDC_RDNS20_PERIOD_ETIME_DAY_DP, m_RdnsChkEndDTCtrl);
	DDX_Control(pDX, IDC_RDNS20_CID_BTN, m_CidSelBtn);
	DDX_Control(pDX, IDC_RDNS20_SACQ_CHKBTN, m_AGC_S_ACQCtrl);
	DDX_Control(pDX, IDC_RDNS20_SACQ_A_CHKBTN, m_AGC_S_ACQ_ACtrl);
	DDX_Control(pDX, IDC_RDNS20_SACQ_B_CHKBTN, m_AGC_S_ACQ_BCtrl);
	DDX_Control(pDX, IDC_RDNS20_STCR_CHKBTN, m_AGC_S_TCRCtrl);
	DDX_Control(pDX, IDC_RDNS20_STCR_A_CHKBTN, m_AGC_S_TCR_ACtrl);
	DDX_Control(pDX, IDC_RDNS20_STCR_B_CHKBTN, m_AGC_S_TCR_BCtrl);
	DDX_Control(pDX, IDC_RDNS20_SDET_CHKBTN, m_AGC_S_DETCtrl);
	DDX_Control(pDX, IDC_RDNS20_SDET_A_CHKBTN, m_AGC_S_DET_ACtrl);
	DDX_Control(pDX, IDC_RDNS20_SDET_B_CHKBTN, m_AGC_S_DET_BCtrl);
	DDX_Control(pDX, IDC_RDNS20_SXTCR_CHKBTN, m_AGC_SX_TCRCtrl);
	DDX_Control(pDX, IDC_RDNS20_SXTCR_A_CHKBTN, m_AGC_SX_TCR_ACtrl);
	DDX_Control(pDX, IDC_RDNS20_SXTCR_B_CHKBTN, m_AGC_SX_TCR_BCtrl);
	DDX_Control(pDX, IDC_RDNS20_XHRX_CHKBTN, m_AGC_X_HRXCtrl);
	DDX_Control(pDX, IDC_RDNS20_D_SREQPM_SS_CHKBTN, m_Delay_SSCtrl);
	DDX_Control(pDX, IDC_RDNS20_D_SREQPM_SX_CHKBTN, m_Delay_SXCtrl);
	DDX_Control(pDX, IDC_RDNS20_TLM_R_STLMIN, m_TLM_SCtrl);
	DDX_Control(pDX, IDC_RDNS20_TLM_R_XTLMIN, m_TLM_XCtrl);
	DDX_Control(pDX, IDC_RDNS20_TLM_R_XHRTLMIN, m_TLM_X_HRXCtrl);
	DDX_Control(pDX, IDC_RDNS20_CMD_CMD_1_CHKBTN, m_CMD_OUT1Ctrl);
	DDX_Control(pDX, IDC_RDNS20_REALTIME_STOPBTN, m_AbortBtnCtr);
	DDX_Control(pDX, IDC_RDNS20_PERIOD_STIME_TIME_DP, m_RdnsChkStartTTCtrl);
	DDX_Control(pDX, IDC_RDNS20_PERIOD_ETIME_TIME_DP, m_RdnsChkEndTTCtrl);
}


BEGIN_MESSAGE_MAP(CRdnsChk20, CDialogBase)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CRdnsChk20::OnMenuClose)
	ON_BN_CLICKED(IDC_RDNS20_SACQ_CHKBTN, &CRdnsChk20::OnBnClickedRdns20SacqChkbtn)
	ON_BN_CLICKED(IDC_RDNS20_STCR_CHKBTN, &CRdnsChk20::OnBnClickedRdns20StcrChkbtn)
	ON_BN_CLICKED(IDC_RDNS20_SDET_CHKBTN, &CRdnsChk20::OnBnClickedRdns20SdetChkbtn)
	ON_BN_CLICKED(IDC_RDNS20_SXTCR_CHKBTN, &CRdnsChk20::OnBnClickedRdns20SxtcrChkbtn)
	ON_BN_CLICKED(IDC_RDNS20_REALTIME_RUNBTN, &CRdnsChk20::OnBnClickedRdns20RealtimeRunbtn)
	ON_BN_CLICKED(IDC_RDNS20_REALTIME_STOPBTN, &CRdnsChk20::OnBnClickedRdns20RealtimeStopbtn)
	ON_BN_CLICKED(IDC_RDNS20_PERIOD_ENTRYBTN, &CRdnsChk20::OnBnClickedRdns20PeriodEntrybtn)
	ON_BN_CLICKED(IDC_RDNS20_CID_BTN, &CRdnsChk20::OnBnClickedRdns20CidBtn)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_RDNS20_SSEL_CMB, &CRdnsChk20::OnCbnSelchangeRdns20SselCmb)
	ON_BN_CLICKED(IDC_RDNS20_SACQ_A_CHKBTN, &CRdnsChk20::OnBnClickedRdns20SacqAChkbtn)
	ON_BN_CLICKED(IDC_RDNS20_SACQ_B_CHKBTN, &CRdnsChk20::OnBnClickedRdns20SacqBChkbtn)
	ON_BN_CLICKED(IDC_RDNS20_STCR_A_CHKBTN, &CRdnsChk20::OnBnClickedRdns20StcrAChkbtn)
	ON_BN_CLICKED(IDC_RDNS20_STCR_B_CHKBTN, &CRdnsChk20::OnBnClickedRdns20StcrBChkbtn)
	ON_BN_CLICKED(IDC_RDNS20_SDET_A_CHKBTN, &CRdnsChk20::OnBnClickedRdns20SdetAChkbtn)
	ON_BN_CLICKED(IDC_RDNS20_SDET_B_CHKBTN, &CRdnsChk20::OnBnClickedRdns20SdetBChkbtn)
	ON_BN_CLICKED(IDC_RDNS20_SXTCR_A_CHKBTN, &CRdnsChk20::OnBnClickedRdns20SxtcrAChkbtn)
	ON_BN_CLICKED(IDC_RDNS20_SXTCR_B_CHKBTN, &CRdnsChk20::OnBnClickedRdns20SxtcrBChkbtn)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_RDNS20_PERIOD_STIME_DAY_DP, &CRdnsChk20::OnDtnDatetimechangeRdns20PeriodStimeDayDp)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_RDNS20_PERIOD_STIME_TIME_DP, &CRdnsChk20::OnDtnDatetimechangeRdns20PeriodStimeTimeDp)
	ON_BN_CLICKED(IDC_RDNS20_XHRX_CHKBTN, &CRdnsChk20::OnBnClickedRdns20XhrxChkbtn)
	ON_BN_CLICKED(IDC_RDNS20_D_SREQPM_SS_CHKBTN, &CRdnsChk20::OnBnClickedRdns20DSreqpmSsChkbtn)
	ON_BN_CLICKED(IDC_RDNS20_D_SREQPM_SX_CHKBTN, &CRdnsChk20::OnBnClickedRdns20DSreqpmSxChkbtn)
	ON_BN_CLICKED(IDC_RDNS20_TLM_R_STLMIN, &CRdnsChk20::OnBnClickedRdns20TlmRStlmin)
	ON_BN_CLICKED(IDC_RDNS20_TLM_R_XTLMIN, &CRdnsChk20::OnBnClickedRdns20TlmRXtlmin)
	ON_BN_CLICKED(IDC_RDNS20_TLM_R_XHRTLMIN, &CRdnsChk20::OnBnClickedRdns20TlmRXhrtlmin)
	ON_BN_CLICKED(IDC_RDNS20_CMD_CMD_1_CHKBTN, &CRdnsChk20::OnBnClickedRdns20CmdCmd1Chkbtn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RDNS20_AGC_CHKBTN, &CRdnsChk20::OnBnClickedRdns20AgcChkbtn)
	ON_BN_CLICKED(IDC_RDNS20_DELAY_CHKBTN, &CRdnsChk20::OnBnClickedRdns20DelayChkbtn)
	ON_BN_CLICKED(IDC_RDNS20_TLM_CHKBTN, &CRdnsChk20::OnBnClickedRdns20TlmChkbtn)
	ON_BN_CLICKED(IDC_RDNS20_CMD_CHKBTN, &CRdnsChk20::OnBnClickedRdns20CmdChkbtn)
END_MESSAGE_MAP()


// CRdnsChk20 ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CRdnsChk20

-# �t�@�C��-���郁�j���[����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnMenuClose()
{
	CString title;
	GetWindowText(title);

	if (m_ThreadRun == true)
	{
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("�Z���f�f���s���ł��B�I�����Ă�낵���ł����H\n���s���̍Z���f�f�͋����I�����܂��B"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			return;
		}
		m_ThreadAbort = true;
	}

	CloseLogDlg();
	InitTimer();
	//	OnOK();
	ShowWindow(SW_HIDE);
}

/*============================================================================*/
/*! CRdnsChk20

-# ���鏈��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnClose()
{
	CString title;
	GetWindowText(title);

	if (m_ThreadRun == true)
	{
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("�Z���f�f���s���ł��B�I�����Ă�낵���ł����H\n���s���̍Z���f�f�͋����I�����܂��B"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			return;
		}

		m_ThreadAbort = true;
	}
	else
	{
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("�I�����܂��B\n��낵���ł����H"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			return;
		}
	}

	CloseLogDlg();
	InitTimer();
	//	OnOK();
	ShowWindow(SW_HIDE);
}

/*============================================================================*/
/*! CRdnsChk20

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CRdnsChk20::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// �q���I��
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	vector<CString>			&saNameList = dba.GetSatNameList();
	int cnt = (int)saNameList.size();
	for (int i = 0; i < cnt; i++)
	{
		/*--------------------------*/
		/* �q���{�̏��A�h���X�擾 */
		CString satname = saNameList[i];

		if (satname == _T("DEFAULT") || satname == _T("�c�d�e�`�t�k�s"))
		{
			continue;
		}
		m_SatelliteSelCmb.InsertString(-1, satname);
	}

	// �q���܂ł̋���
	m_SatelliteDistanceEdit.SetRoundPlaceValue(3);	// �����_3��
	m_SatelliteDistanceEdit.SetWindowTextW(_T("10000.000"));

	// ���t�t�H�[�}�b�g
	m_RdnsChkStartDTCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_RdnsChkStartTTCtrl.SetFormat(_T("HH:mm:00"));

	m_RdnsChkEndDTCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_RdnsChkEndTTCtrl.SetFormat(_T("HH:mm:00"));

	UpdateData(FALSE);

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CRdnsChk20

-# ���O�_�C�A���O�\������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
//BOOL CRdnsChk20::UpdateDlg()
BOOL CRdnsChk20::UpdateRdnsChkDlg()
{
	if (m_ThreadRun == true)
	{
		return FALSE;
	}

	// ���ݎ����擾
	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	__int64 d_timeCurrent = CTime(timeSys).GetTime();
	CTime mintime = CTime((time_t)((double)d_timeCurrent + START_OFFSET));
	CTime time;
	if (m_bNewPlan == TRUE && m_StartTime == 0)
	{
		// �V�K�v��Ń^�C�����C���w�莞���łȂ��ꍇ
		time = mintime;
	}
	else
	{
		// �v��ύX, �������͐V�K�v��Ń^�C�����C���̊J�n�������g��
		time = m_StartTime;
	}

	// UTC���ݎ��Ԏ擾���A�J�n���ƏI�����𐧌�
	COleDateTime oletime1 = COleDateTime(time.GetTime());
	COleDateTime oletime2;
	oletime2.SetStatus(COleDateTime::null);
	m_RdnsChkStartDTCtrl.SetRange(&oletime1, &oletime2);
	m_RdnsChkEndDTCtrl.SetRange(&oletime1, &oletime2);

	m_strDay = time.Format(_T("%y%m%d"));
	CString passId = _T("");
	passId.Format(_T("%s-9999"), (LPCTSTR)m_strDay);

	// �Z���f�f�X�V���v��t�@�C���Ǎ�
	stPlandata stpd;
	vector<stCalibPlan> agclist;
	agclist.clear();
	vector<stCalibPlan> tlmlist;
	tlmlist.clear();
	vector<stCalibPlan> cmdlist;
	cmdlist.clear();

	CString cid = _T("");

	if (m_bNewPlan == FALSE)
	{
		// �v��ύX
		if (COprPlnData::ReadRdyChkPlanFile(m_strSatellite, m_PassID, stpd, agclist, tlmlist, cmdlist) != -1)
		{
			time = CSatelliteData::StrTimeToCTime(CString(stpd.sz_start));
			passId = m_PassID;
			cid = stpd.sz_cid;
		}
	}

	//---------------------------------------
	// �e�����g���`���`�F�b�N
	// �p�X�ԍ�
	CString strEntry;
	strEntry.Format(_T("______-____"));

	m_TlmPathNoEdit.EnableMask(_T("DDDDDD DDDD"),	// The mask string
		strEntry,								// Literal, "_" char = character entry
		_T(' '));								// Default char
	m_TlmPathNoEdit.SetValidChars(NULL); // Valid string characters
	m_TlmPathNoEdit.SetWindowTextW(passId);
	m_strTlmPassID = passId;

	//---------------------------------------
	// �R�}���h�`���`�F�b�N
	// �p�X�ԍ�
	m_CmdPathNoEdit.EnableMask(_T("DDDDDD DDDD"),	// The mask string
		strEntry,								// Literal, "_" char = character entry
		_T(' '));								// Default char
	m_CmdPathNoEdit.SetValidChars(NULL); // Valid string characters
	m_CmdPathNoEdit.SetWindowTextW(passId);
	m_strCmdPassID = passId;

	// �Z���f�f�J�n/�I������
	m_PeriodExecStartDay = time;
	m_PeriodExecStartTime = time;
	m_PeriodExecEndDay = time;
	m_PeriodExecEndTime = time;

	UpdateData(FALSE);

	// �q�����ACID�ݒ�
	vector<CString> &sList = theApp.GetSatelliteData().m_CIDSlctData.GetSatelliteNameList();
	map<CString, map<CString, stCIDParam>> &cidlist = theApp.GetSatelliteData().m_CIDSlctData.GetCidParamList();

//#if 0
//	if (sList.size() == 0)
//	{
//		m_SatelliteSelCmb.SetCurSel(0);
//		m_CidSelBtn.SetWindowTextW(CID_NA);
//		m_CidSelBtn.EnableWindow(FALSE);
//		return FALSE;
//	}
//#endif

	stCIDParam stCid;
	CString satname = _T("");
	BOOL bCid = TRUE;

	if ((m_strSatellite.CompareNoCase(_T("VLBI")) == 0)
		|| (m_strSatellite.CompareNoCase(_T("MAINT")) == 0))
	{
		m_strSatellite = _T("");
	}

	if (m_strSatellite == _T(""))
	{
		m_SatelliteSelCmb.SetCurSel(0);
		m_SatelliteSelCmb.EnableWindow(TRUE);
		m_SatelliteSelCmb.GetLBText(0, satname);

		if (sList.size() == 0)
		{
			m_CidSelBtn.SetWindowTextW(CID_NA);
			m_CidSelBtn.EnableWindow(FALSE);
			bCid = FALSE;
		}
		else
		{
			vector<CString> &cList = theApp.GetSatelliteData().m_CIDSlctData.GetCidList(satname);
			if (cList.size() == 0)
			{
				m_CidSelBtn.SetWindowTextW(CID_NA);
				m_CidSelBtn.EnableWindow(FALSE);
				bCid = FALSE;
			}
			else
			{
				stOpeProp stProp;
				theApp.GetSatelliteData().ReadPropertyFile(satname, stProp);
				CString strCid = CString(stProp.cid);
				auto itrl = std::find(cList.begin(), cList.end(), strCid);
				if (itrl == cList.end())
				{
					// �f�t�H���gCID��CID�t�@�C���ɖ����ꍇ�A��ԍŏ���CID���Z�b�g�B
					strCid = cList[0];
				}

				m_CidSelBtn.EnableWindow(TRUE);
				m_CidSelBtn.SetWindowTextW(strCid);
				stCid = CCIDSlctData::TrimInvalidString(cidlist[satname][strCid]);
			}
		}
	}
	else
	{
		m_SatelliteSelCmb.SelectStringEx(-1, m_strSatellite);
		m_SatelliteSelCmb.EnableWindow(FALSE);
		satname = m_strSatellite;

		vector<CString> &cList = theApp.GetSatelliteData().m_CIDSlctData.GetCidList(m_strSatellite);
		if (cList.size() == 0)
		{
			m_CidSelBtn.SetWindowTextW(CID_NA);
			m_CidSelBtn.EnableWindow(FALSE);
			bCid = FALSE;
		}
		else
		{
			stOpeProp stProp;
			theApp.GetSatelliteData().ReadPropertyFile(m_strSatellite, stProp);
			CString strCid = CString(stProp.cid);
			auto itrl = std::find(cList.begin(), cList.end(), strCid);
			if (itrl == cList.end())
			{
				// �f�t�H���gCID��CID�t�@�C���ɖ����ꍇ�A��ԍŏ���CID���Z�b�g�B
				strCid = cList[0];
			}

			if (cid.IsEmpty() == TRUE)
			{
				m_CidSelBtn.EnableWindow(TRUE);
				m_CidSelBtn.SetWindowTextW(strCid);
				stCid = CCIDSlctData::TrimInvalidString(cidlist[m_strSatellite][strCid]);
			}
			else
			{
				auto itrl = std::find(cList.begin(), cList.end(), cid);
				if (itrl != cList.end())
				{
					m_CidSelBtn.EnableWindow(TRUE);
					m_CidSelBtn.SetWindowTextW(cid);
					stCid = CCIDSlctData::TrimInvalidString(cidlist[m_strSatellite][cid]);
				}
				else
				{
					m_CidSelBtn.EnableWindow(TRUE);
					m_CidSelBtn.SetWindowTextW(strCid);
					stCid = CCIDSlctData::TrimInvalidString(cidlist[m_strSatellite][strCid]);
				}
			}
		}
	}

	// �q���ŗL���擾
	GetEqSatDBData(satname);

	// �Z���f�f�p�R���g���[���ݒ�
	SetWindowEnable(satname);

	// �v��t�@�C���X�V���̉�ʐݒ�
	SetSetting(agclist, tlmlist, cmdlist);

	if (bCid)
	{
		// �e�����g���r�b�g���[�g
		m_strTlmBRS = stCid.TlmBitRateS;
		m_strTlmBRX = stCid.TlmBitRateX;
		m_strTlmBRH = stCid.TlmBitRateHX;

		// �R�}���h�r�b�g���[�g
		m_strCmdBRS = stCid.CmdBitRateS;
	}
	else
	{
		// �e�����g���r�b�g���[�g
		m_strTlmBRS = _T("");
		m_strTlmBRX = _T("");
		m_strTlmBRH = _T("");

		// �R�}���h�r�b�g���[�g
		m_strCmdBRS = stCid.CmdBitRateS;
	}

	// �����I���{�^���񊈐�
	m_AbortBtnCtr.EnableWindow(FALSE);

	UpdateData(FALSE);

	m_ThreadRun = false;

	// �I���\�莞���ύX
	ChangeRdyChkEndTime();

	return TRUE;
}


/*============================================================================*/
/*! CRdnsChk20

-# �ǂݍ��񂾌v��t�@�C���ɏ]����ʂ�ݒ肷��B

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::SetSetting(vector<stCalibPlan>& agclist, vector<stCalibPlan>& tlmlist, vector<stCalibPlan>& cmdlist)
{
	// AGC,RNG �ݒ�
	BOOL bAGC = FALSE;
	BOOL bRNG = FALSE;
	for (int i = 0; i < agclist.size(); ++i)
	{
		CString item = agclist[i].ctrl;
		if (item.CompareNoCase(_T("SX-ACQRX_S_AB")) == 0)
		{
			m_AgcSACQAChkValue = TRUE;
			m_AgcSACQBChkValue = TRUE;
			m_AgcSACQChkValue = TRUE;
			bAGC = TRUE;
		}
		else if (item.CompareNoCase(_T("SX-ACQRX_S_A")) == 0)
		{
			m_AgcSACQAChkValue = TRUE;
			m_AgcSACQChkValue = TRUE;
			bAGC = TRUE;
		}
		else if (item.CompareNoCase(_T("SX-ACQRX_S_B")) == 0)
		{
			m_AgcSACQBChkValue = TRUE;
			m_AgcSACQChkValue = TRUE;
			bAGC = TRUE;
		}
		else if (item.CompareNoCase(_T("S-RX_AB")) == 0)
		{
			m_AgcSTCRAChkValue = TRUE;
			m_AgcSTCRBChkValue = TRUE;
			m_AgcSTCRChkValue = TRUE;
			bAGC = TRUE;
		}
		else if (item.CompareNoCase(_T("S-RX_A")) == 0)
		{
			m_AgcSTCRAChkValue = TRUE;
			m_AgcSTCRChkValue = TRUE;
			bAGC = TRUE;
		}
		else if (item.CompareNoCase(_T("S-RX_B")) == 0)
		{
			m_AgcSTCRBChkValue = TRUE;
			m_AgcSTCRChkValue = TRUE;
			bAGC = TRUE;
		}
		else if (item.CompareNoCase(_T("SS-TCRDET_AB")) == 0)
		{
			m_AgcSDETAChkValue = TRUE;
			m_AgcSDETBChkValue = TRUE;
			m_AgcSDETChkValue = TRUE;
			bAGC = TRUE;
		}
		else if (item.CompareNoCase(_T("SS-TCRDET_A")) == 0)
		{
			m_AgcSDETAChkValue = TRUE;
			m_AgcSDETChkValue = TRUE;
			bAGC = TRUE;
		}
		else if (item.CompareNoCase(_T("SS-TCRDET_B")) == 0)
		{
			m_AgcSDETBChkValue = TRUE;
			m_AgcSDETChkValue = TRUE;
			bAGC = TRUE;
		}
		else if (item.CompareNoCase(_T("X-RX_AB")) == 0)
		{
			m_AgcSXTCRAChkValue = TRUE;
			m_AgcSXTCRBChkValue = TRUE;
			m_AgcSXTCRChkValue = TRUE;
			bAGC = TRUE;
		}
		else if (item.CompareNoCase(_T("X-RX_A")) == 0)
		{
			m_AgcSXTCRAChkValue = TRUE;
			m_AgcSXTCRChkValue = TRUE;
			bAGC = TRUE;
		}
		else if (item.CompareNoCase(_T("X-RX_B")) == 0)
		{
			m_AgcSXTCRBChkValue = TRUE;
			m_AgcSXTCRChkValue = TRUE;
			bAGC = TRUE;
		}
		else if (item.CompareNoCase(_T("X-QPSKRX_A")) == 0)
		{
			m_AgcXHRXChkValue = TRUE;
			bAGC = TRUE;
		}
		else if (item.CompareNoCase(_T("SS")) == 0)
		{
			m_DlySSChkValue = TRUE;
			bRNG = TRUE;
		}
		else if (item.CompareNoCase(_T("SX")) == 0)
		{
			m_DlySXChkValue = TRUE;
			bRNG = TRUE;
		}
	}

	if (bAGC == TRUE)
	{
		m_AgcChkValue = TRUE;
	}

	if (bRNG == TRUE)
	{
		m_DlyChkValue = TRUE;
	}


	// TLM �ݒ�
	BOOL bTLM = FALSE;
	for (int i = 0; i < tlmlist.size(); ++i)
	{
		CString item = tlmlist[i].ctrl;
		if (item.CompareNoCase(_T("S")) == 0)
		{
			m_TLMSChkValue = TRUE;
			bTLM = TRUE;
		}
		else if (item.CompareNoCase(_T("X")) == 0)
		{
			m_TLMXChkValue = TRUE;
			bTLM = TRUE;
		}
		else if (item.CompareNoCase(_T("QPSK")) == 0)
		{
			m_TLMXHChkValue = TRUE;
			bTLM = TRUE;
		}
	}
	if (bTLM == TRUE)
	{
		m_TLMChkValue = TRUE;
	}

	// CMD �ݒ�
	BOOL bCMD = FALSE;
	for (int i = 0; i < cmdlist.size(); ++i)
	{
		CString item = cmdlist[i].ctrl;
		if (item.CompareNoCase(_T("1")) == 0)
		{
			m_CmdOut1ChkValue = TRUE;
			bCMD = TRUE;
		}
	}

	if (bCMD == TRUE)
	{
		m_CmdChkValue = TRUE;
	}

	UpdateData(FALSE);
}


/*============================================================================*/
/*! CRdnsChk20

-# AGC�Z�� �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20AgcChkbtn()
{
	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# S-ACQ �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20SacqChkbtn()
{
	UpdateData();

	// Ach/Bch ����
	m_AgcSACQAChkValue = m_AgcSACQChkValue;
	m_AgcSACQBChkValue = m_AgcSACQChkValue;

	UpdateData(FALSE);

	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# S-ACQ Ach �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20SacqAChkbtn()
{
	UpdateData();

	if (m_AgcSACQAChkValue == FALSE && m_AgcSACQBChkValue == FALSE)
	{
		m_AgcSACQChkValue = FALSE;
	}
	else
	{
		m_AgcSACQChkValue = TRUE;
	}

	UpdateData(FALSE);

	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# S-ACQ Bch �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20SacqBChkbtn()
{
	UpdateData();

	if (m_AgcSACQAChkValue == FALSE && m_AgcSACQBChkValue == FALSE)
	{
		m_AgcSACQChkValue = FALSE;
	}
	else
	{
		m_AgcSACQChkValue = TRUE;
	}

	UpdateData(FALSE);

	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# S-TCR �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20StcrChkbtn()
{
	UpdateData();

	// Ach/Bch ����
	m_AgcSTCRAChkValue = m_AgcSTCRChkValue;
	m_AgcSTCRBChkValue = m_AgcSTCRChkValue;

	UpdateData(FALSE);

	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# S-TCR Ach �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20StcrAChkbtn()
{
	UpdateData();

	if (m_AgcSTCRAChkValue == FALSE && m_AgcSTCRBChkValue == FALSE)
	{
		m_AgcSTCRChkValue = FALSE;
	}
	else
	{
		m_AgcSTCRChkValue = TRUE;
	}

	UpdateData(FALSE);

	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# S-TCR Bch �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20StcrBChkbtn()
{
	UpdateData();

	if (m_AgcSTCRAChkValue == FALSE && m_AgcSTCRBChkValue == FALSE)
	{
		m_AgcSTCRChkValue = FALSE;
	}
	else
	{
		m_AgcSTCRChkValue = TRUE;
	}

	UpdateData(FALSE);

	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# S-DET �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20SdetChkbtn()
{
	UpdateData();

	// Ach/Bch ����
	m_AgcSDETAChkValue = m_AgcSDETChkValue;
	m_AgcSDETBChkValue = m_AgcSDETChkValue;

	UpdateData(FALSE);

	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# S-DET Ach �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20SdetAChkbtn()
{
	UpdateData();

	if (m_AgcSDETAChkValue == FALSE && m_AgcSDETBChkValue == FALSE)
	{
		m_AgcSDETChkValue = FALSE;
	}
	else
	{
		m_AgcSDETChkValue = TRUE;
	}

	UpdateData(FALSE);

	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# S-DET Bch �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20SdetBChkbtn()
{
	UpdateData();

	if (m_AgcSDETAChkValue == FALSE && m_AgcSDETBChkValue == FALSE)
	{
		m_AgcSDETChkValue = FALSE;
	}
	else
	{
		m_AgcSDETChkValue = TRUE;
	}

	UpdateData(FALSE);

	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# SX-TCR �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20SxtcrChkbtn()
{
	UpdateData();

	// Ach/Bch ����
	m_AgcSXTCRAChkValue = m_AgcSXTCRChkValue;
	m_AgcSXTCRBChkValue = m_AgcSXTCRChkValue;

	UpdateData(FALSE);

	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# SX-TCR Ach �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20SxtcrAChkbtn()
{
	UpdateData();

	if (m_AgcSXTCRAChkValue == FALSE && m_AgcSXTCRBChkValue == FALSE)
	{
		m_AgcSXTCRChkValue = FALSE;
	}
	else
	{
		m_AgcSXTCRChkValue = TRUE;
	}

	UpdateData(FALSE);

	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# SX-TCR Bch �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20SxtcrBChkbtn()
{
	UpdateData();

	if (m_AgcSXTCRAChkValue == FALSE && m_AgcSXTCRBChkValue == FALSE)
	{
		m_AgcSXTCRChkValue = FALSE;
	}
	else
	{
		m_AgcSXTCRChkValue = TRUE;
	}

	UpdateData(FALSE);

	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# X-HRX �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20XhrxChkbtn()
{
	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# �Ǔ��f�B���C�l �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20DelayChkbtn()
{
	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# S/S �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20DSreqpmSsChkbtn()
{
	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# S/X �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20DSreqpmSxChkbtn()
{
	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# �e�����g���`�� �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20TlmChkbtn()
{
	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# S-TLMIN �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20TlmRStlmin()
{
	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# X-TLMIN �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20TlmRXtlmin()
{
	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# X-HRXTLMIN �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20TlmRXhrtlmin()
{
	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# �R�}���h�`�� �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20CmdChkbtn()
{
	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}

/*============================================================================*/
/*! CRdnsChk20

-# CMD OUT �`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20CmdCmd1Chkbtn()
{
	// �I���\�莞���ύX
	ChangeRdyChkEndTime();
}


/*============================================================================*/
/*! CRdnsChk20

-# ���A���^�C�����s-���s�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20RealtimeRunbtn()
{
	if (m_ThreadRun == true)
	{
		return;
	}

	CString title;
	GetWindowText(title);

	m_CtrlList.clear();
	GetSetting();

	if (m_CtrlList.size() > 0)
	{
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("�Z���f�f�����s���܂��B\n��낵���ł����H"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return;

//#if 1
		HANDLE hFile;
		FILETIME cfTime, afTime, wfTime;

		int flg = 0;
		m_AgcSResFileTime = 0;
		m_AgcXResFileTime = 0;
		m_DelayResFileTime = 0;
		m_TlmResFileTime = 0;
		m_CmdResFileTime = 0;

		CString dir = theApp.GetShareFilePath(eFileType_Calib_Result_Folder, theApp.GetSelectStation());
		CString strFilePath = _T("");

//#ifdef _DEBUG
//		CString strTime;
//#endif
		for (int i = 0; i < m_CtrlList.size(); ++i)
		{
			stSendCtrlMsg st = m_CtrlList[i];

			if (((flg & 0x01) == 0) && st.type == TYPE_AGC_S)
			{
				strFilePath = dir + RES_FILE_AGCS;
				hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
				if (hFile != INVALID_HANDLE_VALUE)
				{
					GetFileTime(hFile, &cfTime, &afTime, &wfTime);
					CloseHandle(hFile);
					m_AgcSResFileTime = CTime(wfTime);
//#ifdef _DEBUG
//					strTime = m_AgcSResFileTime.Format("%Y-%m-%d %H:%M:%S");
//#endif
				}
				flg |= 0x01;
			}
			else if (((flg & 0x02) == 0) && st.type == TYPE_AGC_X)
			{
				strFilePath = dir + RES_FILE_AGCX;
				hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
				if (hFile != INVALID_HANDLE_VALUE)
				{
					GetFileTime(hFile, &cfTime, &afTime, &wfTime);
					CloseHandle(hFile);
					m_AgcXResFileTime = CTime(wfTime);
//#ifdef _DEBUG
//					strTime = m_AgcXResFileTime.Format("%Y-%m-%d %H:%M:%S");
//#endif
				}
				flg |= 0x02;
			}
			else if (((flg & 0x04) == 0) && st.type == TYPE_RNG)
			{
				strFilePath = dir + RES_FILE_RNG;
				hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
				if (hFile != INVALID_HANDLE_VALUE)
				{
					GetFileTime(hFile, &cfTime, &afTime, &wfTime);
					CloseHandle(hFile);
					m_DelayResFileTime = CTime(wfTime);
//#ifdef _DEBUG
//					strTime = m_DelayResFileTime.Format("%Y-%m-%d %H:%M:%S");
//#endif
				}
				flg |= 0x04;
			}
			else if (((flg & 0x08) == 0) && (st.type == TYPE_TLM_S || st.type == TYPE_TLM_X))
			{
				strFilePath = dir + RES_FILE_TLM;
				hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
				if (hFile != INVALID_HANDLE_VALUE)
				{
					GetFileTime(hFile, &cfTime, &afTime, &wfTime);
					CloseHandle(hFile);
					m_TlmResFileTime = CTime(wfTime);
//#ifdef _DEBUG
//					strTime = m_TlmResFileTime.Format("%Y-%m-%d %H:%M:%S");
//#endif
				}
				flg |= 0x08;
			}
			else if (((flg & 0x10) == 0) && st.type == TYPE_CMD)
			{
				strFilePath = dir + RES_FILE_CMD;
				hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
				if (hFile != INVALID_HANDLE_VALUE)
				{
					GetFileTime(hFile, &cfTime, &afTime, &wfTime);
					CloseHandle(hFile);
					m_CmdResFileTime = CTime(wfTime);
//#ifdef _DEBUG
//					strTime = m_CmdResFileTime.Format("%Y-%m-%d %H:%M:%S");
//#endif
				}
				flg |= 0x10;
			}
		}

		InitTimer();
//#endif
		m_ThreadAbort = false;
		m_ThreadRun = false;
		m_RdnsChkAbort = false;

//#if 1
		// ���ʕ\���p�_�C�A���O�\��
		if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
		{
			m_ResultDlg->DestroyWindow();
		}

		m_ResultDlg = new CRdnsChkResult();
		if (m_ResultDlg != NULL)
		{
			m_ResultDlg->SetRdnsCheckList(m_CtrlList);
			if (m_ResultDlg->Create(IDD_DLG_RDNSCHK_RESULT, this))
			{
				m_ResultDlg->ShowWindow(SW_SHOW);
			}
		}
//#endif

		if (m_hThread != NULL)
		{
			WaitForSingleObject(m_hThread, INFINITE);
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}

		// ���䑗�M�p�X���b�h�̍쐬
		UINT uiThreadId;
		m_hThread = (HANDLE)_beginthreadex(NULL,				// SECURITY_ATTRIBUTES �\���̂ւ̃|�C���^
			0,					// �V�����X���b�h�̃X�^�b�N �T�C�Y
			SendCtrlThread,		// �V�����X���b�h�̎��s���J�n���郋�[�`���̊J�n�A�h���X
			this,				// �X���b�h�p�����^
			CREATE_SUSPENDED,	// ������ԃt���O
			&uiThreadId);		// �X���b�h���ʎq���󂯎�� 32 �r�b�g�ϐ��ւ̃|�C���^

		if (m_hThread != NULL)
		{
			m_SendFlg = TRUE;
			ResumeThread(m_hThread);
		}

		// ���s�{�^���񊈐���
		GetDlgItem(IDC_RDNS20_REALTIME_RUNBTN)->EnableWindow(FALSE);

		// �o�^�{�^���񊈐���
		GetDlgItem(IDC_RDNS20_PERIOD_ENTRYBTN)->EnableWindow(FALSE);

		// �����I���{�^��������
		m_AbortBtnCtr.EnableWindow(TRUE);

		// �q���I���R���{�{�b�N�X�񊈐���
		m_SatelliteSelCmb.EnableWindow(FALSE);

		// CID�I���{�^���񊈐���
		m_CidSelBtn.EnableWindow(FALSE);

//#ifdef _DEBUG
//		SetTimer(TID_RDNSSTATE, INTERVAL_RDNSSTATE, 0);
//#endif	// <<< Debug
	}
	else
	{
		MessageBox(_T("�Z���f�f�̑Ώۂ��I������Ă��܂���B"), title, MB_OK | MB_ICONWARNING);
	}
}

/*============================================================================*/
/*! CRdnsChk20

-# ���A���^�C�����s-�����I���{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20RealtimeStopbtn()
{
	CString title;
	GetWindowText(title);

	if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("���s���̍Z���f�f�𒆎~���܂��B\n��낵���ł����H"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		return;
	}

	if (m_hThread == NULL)
	{
		return;
	}

//#if 1

//#if 0
//	m_RdnsChkAbort = true;
//#else
	m_ThreadAbort = true;
//	InitTimer();
//#endif

//#else
//	m_SendFlg = FALSE;
//	if (WaitForSingleObject(m_hThread, 1000L) == WAIT_TIMEOUT)
//	{
//		TerminateThread(m_hThread, 1L);
//	}
//	CloseHandle(m_hThread);
//	m_hThread = NULL;
//
//	BOOL bAGC = FALSE;
//	BOOL bRNG = FALSE;
//	BOOL bTLM = FALSE;
//	BOOL bCMD = FALSE;
//	CString strCtrl = _T("");
//
//	int cnt = (int)m_CtrlList.size();
//	for (int i = 0; i < cnt; ++i)
//	{
//		stSendCtrlMsg &st = m_CtrlList[i];
//		if (st.bRun)
//		{
//			if (st.type == TYPE_AGC && bAGC == FALSE)
//			{
//				strCtrl.Format(CTRL_AGC_CANCEL, (LPCTSTR)st.satellite);
//				theApp.SendControl(strCtrl, _T(""), _T(""), _T(""));
//				bAGC = TRUE;
//			}
//
//			if (st.type == TYPE_RNG && bRNG == FALSE)
//			{
//				strCtrl.Format(CTRL_RNG_CANCEL, (LPCTSTR)st.satellite);
//				theApp.SendControl(strCtrl, _T(""), _T(""), _T(""));
//				bRNG = TRUE;
//			}
//
//			if (st.type == TYPE_TLM && bTLM == FALSE)
//			{
//				strCtrl.Format(CTRL_TLM_CANCEL, (LPCTSTR)st.satellite);
//				theApp.SendControl(strCtrl, _T(""), _T(""), _T(""));
//				bTLM = TRUE;
//			}
//
//			if (st.type == TYPE_CMD && bCMD == FALSE)
//			{
//				strCtrl.Format(CTRL_CMD_CANCEL, (LPCTSTR)st.satellite);
//				theApp.SendControl(strCtrl, _T(""), _T(""), _T(""));
//				bCMD = TRUE;
//			}
//		}
//	}
//
//#if 0
//	// �����I���{�^���񊈐���
//	m_AbortBtnCtr.EnableWindow(FALSE);
//#endif
//#endif
}


/*============================================================================*/
/*! CRdnsChk20

-# ��ʐݒ�擾

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::GetSetting()
{
	UpdateData();

	CString strCtrl = _T("");
	CString strCancelCtrl = _T("");

	m_CtrlList.clear();
	stSendCtrlMsg st;

	// �z�X�g���̎擾
	CString strHost = CString(mCtrlNameString[theApp.GetSelectStation()]);

	// �ǖ��擾
	CString strStation = CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower();

	// �q�����擾
	CString strSatellite = _T("");
	m_SatelliteSelCmb.GetLBText(m_SatelliteSelCmb.GetCurSel(), strSatellite);

	// �q���ԍ��擾
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	int nSatellite = dba.GetIdxSatelliteEqSatDB(strSatellite);

	// CID�ԍ��擾
	CString strCID;
	m_CidSelBtn.GetWindowTextW(strCID);

	memset(&m_VResult, 0, sizeof(m_VResult));

	// AGC�Z�� �`�F�b�N��Ԏ擾
	if (m_AgcChkValue)
	{
		// S-ACQ
		if (m_AgcSACQAChkValue && m_AgcSACQBChkValue)
		{
			// Ach + Bch
			strCtrl.Format(CTRL_AGC_S_ACQ_AB, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_AGC_S_ACQ_AB_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_AGC_S;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_AGC_S_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("AGC_S")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("S-ACQ AB");
			m_CtrlList.push_back(st);

			m_VResult.agc_s.count++;
		}
		else if (m_AgcSACQAChkValue)
		{
			// Ach
			strCtrl.Format(CTRL_AGC_S_ACQ_A, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_AGC_S_ACQ_A_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_AGC_S;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_AGC_S_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("AGC_S")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("S-ACQ A");
			m_CtrlList.push_back(st);

			m_VResult.agc_s.count++;
		}
		else if (m_AgcSACQBChkValue)
		{
			// Bch
			strCtrl.Format(CTRL_AGC_S_ACQ_B, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_AGC_S_ACQ_B_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_AGC_S;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_AGC_S_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("AGC_S")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("S-ACQ B");
			m_CtrlList.push_back(st);

			m_VResult.agc_s.count++;
		}

		// S-TCR
		if (m_AgcSTCRAChkValue && m_AgcSTCRBChkValue)
		{
			// Ach + Bch
			strCtrl.Format(CTRL_AGC_SS_TCR_AB, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_AGC_SS_TCR_AB_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_AGC_S;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_AGC_S_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("AGC_S")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("S-TCR AB");
			m_CtrlList.push_back(st);

			m_VResult.agc_s.count++;
		}
		else if (m_AgcSTCRAChkValue)
		{
			// Ach
			strCtrl.Format(CTRL_AGC_SS_TCR_A, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_AGC_SS_TCR_A_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_AGC_S;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_AGC_S_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("AGC_S")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("S-TCR A");
			m_CtrlList.push_back(st);

			m_VResult.agc_s.count++;
		}
		else if (m_AgcSTCRBChkValue)
		{
			// Bch
			strCtrl.Format(CTRL_AGC_SS_TCR_B, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_AGC_SS_TCR_B_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_AGC_S;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_AGC_S_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("AGC_S")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("S-TCR B");
			m_CtrlList.push_back(st);

			m_VResult.agc_s.count++;
		}

		// S-DET
		if (m_AgcSDETAChkValue && m_AgcSDETBChkValue)
		{
			// Ach + Bch
			strCtrl.Format(CTRL_AGC_S_DET_AB, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_AGC_S_DET_AB_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_AGC_S;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_AGC_S_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("AGC_S")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("S-DET AB");
			m_CtrlList.push_back(st);

			m_VResult.agc_s.count++;
		}
		else if (m_AgcSDETAChkValue)
		{
			// Ach
			strCtrl.Format(CTRL_AGC_S_DET_A, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_AGC_S_DET_A_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_AGC_S;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_AGC_S_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("AGC_S")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("S-DET A");
			m_CtrlList.push_back(st);

			m_VResult.agc_s.count++;
		}
		else if (m_AgcSDETBChkValue)
		{
			// Bch
			strCtrl.Format(CTRL_AGC_S_DET_B, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_AGC_S_DET_B_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_AGC_S;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_AGC_S_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("AGC_S")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("S-DET B");
			m_CtrlList.push_back(st);

			m_VResult.agc_s.count++;
		}

		if (m_VResult.agc_s.count > 0)
		{
			m_VResult.agc_s.bResult = TRUE;
		}

		// SX-TCR
		if (m_AgcSXTCRAChkValue && m_AgcSXTCRBChkValue)
		{
			// Ach + Bch
			strCtrl.Format(CTRL_AGC_SX_TCR_AB, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_AGC_SX_TCR_AB_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_AGC_X;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_AGC_X_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("AGC_X")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("SX-TCR AB");
			m_CtrlList.push_back(st);

			m_VResult.agc_x.count++;
		}
		else if (m_AgcSXTCRAChkValue)
		{
			// Ach
			strCtrl.Format(CTRL_AGC_SX_TCR_A, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_AGC_SX_TCR_A_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_AGC_X;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_AGC_X_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("AGC_X")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("SX-TCR A");
			m_CtrlList.push_back(st);

			m_VResult.agc_x.count++;
		}
		else if (m_AgcSXTCRBChkValue)
		{
			// Bch
			strCtrl.Format(CTRL_AGC_SX_TCR_B, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_AGC_SX_TCR_B_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_AGC_X;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_AGC_X_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("AGC_X")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("SX-TCR B");
			m_CtrlList.push_back(st);

			m_VResult.agc_x.count++;
		}

		// X-HRX
		if (m_AgcXHRXChkValue)
		{
			strCtrl.Format(CTRL_AGC_X_HRX_A, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_AGC_X_HRX_A_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_AGC_X;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_AGC_X_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("AGC_XQ")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("X-HRX");
			m_CtrlList.push_back(st);

			m_VResult.agc_x.count++;
		}

		if (m_VResult.agc_x.count > 0)
		{
			m_VResult.agc_x.bResult = TRUE;
		}
	}

	// �Ǔ��f�B���C�l�v��
	if (m_DlyChkValue)
	{
		// S/S
		if (m_DlySSChkValue)
		{
			strCtrl.Format(CTRL_RNG_SS, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_RNG_SS_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_RNG;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_RNG_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("RNG_SS")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("S/S");
			m_CtrlList.push_back(st);

			m_VResult.delay.count++;
		}

		// S/X
		if (m_DlySXChkValue)
		{
			strCtrl.Format(CTRL_RNG_SX, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
			strCancelCtrl.Format(CTRL_RNG_SX_CANCEL, (LPCTSTR)strSatellite);
			st.type = TYPE_RNG;
			st.satellite = strSatellite;
			st.Msg1 = strCtrl;
			st.state = STATE_PRE;
			st.MonString = MON_RNG_COND;
			st.ShellType = SHELL_CTRL;
			st.time = dba.GetRdyChkTimeFile(_T("RNG_SX")) * 60;
			st.MsgCancel = strCancelCtrl;
			st.RdnsItem = _T("S/X");
			m_CtrlList.push_back(st);

			m_VResult.delay.count++;
		}

		if (m_VResult.delay.count > 0)
		{
			m_VResult.delay.bResult = TRUE;
		}
	}

	// �e�����g���`���`�F�b�N
	if (m_TLMChkValue)
	{
		// �p�X�ԍ��擾
		CString strTlmPass = _T("");
		m_TlmPathNoEdit.GetWindowTextW(strTlmPass);
		strTlmPass = strTlmPass.Mid(0, 6) + _T("-") + strTlmPass.Mid(6);	// XXXXXXXXXX ��XXXXXX-XXXX �`���ϊ�

		// S-TLMIN
		if (m_TLMSChkValue)
		{
			if (m_strTlmBRS != _T(""))
			{
				strCtrl.Format(CTRL_TLM_S_TLMIN, (LPCTSTR)strTlmPass, (LPCTSTR)m_strTlmBRS, nSatellite, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
				strCancelCtrl.Format(CTRL_TLM_S_TLMIN_CANCEL, (LPCTSTR)strSatellite);
				st.type = TYPE_TLM_S;
				st.satellite = strSatellite;
				st.Msg1 = strCtrl;
				st.state = STATE_PRE;
				st.MonString = MON_TLM_S_COND;
				st.ShellType = SHELL_CTRL;
				st.time = dba.GetRdyChkTimeFile(_T("TLM_S")) * 60;
				st.MsgCancel = strCancelCtrl;
				st.RdnsItem = _T("S-TLMIN");
				m_CtrlList.push_back(st);

				m_VResult.tlm.count++;
			}
		}

		// X-TLMIN
		if (m_TLMXChkValue)
		{
			if (m_strTlmBRX != _T(""))
			{
				strCtrl.Format(CTRL_TLM_X_TLMIN, (LPCTSTR)strTlmPass, (LPCTSTR)m_strTlmBRX, nSatellite, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
				strCancelCtrl.Format(CTRL_TLM_X_TLMIN_CANCEL, (LPCTSTR)strSatellite);
				st.type = TYPE_TLM_X;
				st.satellite = strSatellite;
				st.Msg1 = strCtrl;
				st.state = STATE_PRE;
				st.MonString = MON_TLM_X_COND;
				st.ShellType = SHELL_CTRL;
				st.time = dba.GetRdyChkTimeFile(_T("TLM_X")) * 60;
				st.MsgCancel = strCancelCtrl;
				st.RdnsItem = _T("X-TLMIN");
				m_CtrlList.push_back(st);

				m_VResult.tlm.count++;
			}
		}

		// X-HRXTLMIN
		if (m_TLMXHChkValue)
		{
			if (m_strTlmBRH != _T(""))
			{
				strCtrl.Format(CTRL_TLM_X_HRXTLMIN, (LPCTSTR)strTlmPass, (LPCTSTR)m_strTlmBRH, nSatellite, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
				strCancelCtrl.Format(CTRL_TLM_X_HRXTLMIN_CANCEL, (LPCTSTR)strSatellite);
				st.type = TYPE_TLM_X;
				st.satellite = strSatellite;
				st.Msg1 = strCtrl;
				st.state = STATE_PRE;
				st.MonString = MON_TLM_X_COND;
				st.ShellType = SHELL_CTRL;
				st.time = dba.GetRdyChkTimeFile(_T("TLM_XQ")) * 60;
				st.MsgCancel = strCancelCtrl;
				st.RdnsItem = _T("X-HRXTLMIN");
				m_CtrlList.push_back(st);

				m_VResult.tlm.count++;
			}
		}

		if (m_VResult.tlm.count > 0)
		{
			m_VResult.tlm.bResult = TRUE;
		}
	}

	// �R�}���h�`���`�F�b�N
	if (m_CmdChkValue)
	{
		// �p�X�ԍ�
		CString strCmdPass;
		m_CmdPathNoEdit.GetWindowTextW(strCmdPass);
		strCmdPass = strCmdPass.Mid(0, 6) + _T("-") + strCmdPass.Mid(6);	// XXXXXXXXXX ��XXXXXX-XXXX �`���ϊ�

		// CMD OUT1
		if (m_CmdOut1ChkValue)
		{
			if (m_strCmdBRS != _T(""))
			{
//				strCtrl.Format(CTRL_CMD_S_CMDOUT, (LPCTSTR)strCmdPass, (LPCTSTR)m_strCmdBRS, nSatellite, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
				CString strBitrate = _T("");
				strBitrate.Format(_T("CMD1_%s"), (LPCTSTR)m_strCmdBRS);
				strCtrl.Format(CTRL_CMD_S_CMDOUT, (LPCTSTR)strCmdPass, (LPCTSTR)strBitrate, nSatellite, (LPCTSTR)strHost, (LPCTSTR)strCID, (LPCTSTR)strSatellite);
				strCancelCtrl.Format(CTRL_CMD_S_CMDOUT_CANCEL, (LPCTSTR)strSatellite);
				st.type = TYPE_CMD;
				st.satellite = strSatellite;
				st.Msg1 = strCtrl;
				st.state = STATE_PRE;
				st.MonString = MON_CMD_COND;
				st.ShellType = SHELL_CTRL;
				st.time = dba.GetRdyChkTimeFile(_T("CMD_S")) * 60;
				st.MsgCancel = strCancelCtrl;
				st.RdnsItem = _T("CMDOUT");
				m_CtrlList.push_back(st);

				m_VResult.cmd.count++;
			}
		}

		if (m_VResult.cmd.count > 0)
		{
			m_VResult.cmd.bResult = TRUE;
		}
	}
}

/*============================================================================*/
/*! CRdnsChk20

-# ������s-�o�^�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20PeriodEntrybtn()
{
	// TLM�p�XID, CMD�p�XID�̏ƍ��L��ōZ���f�f�v��o�^���s
	if (RegistPlan(true) == false)
	{
		// ��L�֐��������r���ŁATLM�p�XID, CMD�p�XID�����킹��ꍇ�́A�p�X�����킹�����ID�ƍ������œo�^���s
		RegistPlan(false);
	}
}

/*============================================================================*/
/*! CRdnsChk20

-# �Z���f�f�v��o�^

@param  bChkPassId�F true�FTLM�p�XID�ACMD�p�XID��AGC�Z���E�Ǔ��f�B���C�l�v��ID���ƍ�����
                     false�F�v��ID���ƍ����Ȃ�
@retval true�F�I�� / false�F�r��
*/
/*============================================================================*/
bool CRdnsChk20::RegistPlan(bool bChkPassId)
{
	// �ݒ�l�擾
	m_CtrlList.clear();
	GetSetting();

	if (m_CtrlList.size() == 0)
	{
		CString title;
		GetWindowText(title);
		MessageBox(_T("�Z���f�f�̑Ώۂ��I������Ă��܂���B"), title, MB_OK | MB_ICONWARNING);
		return true;
	}

	// AGC �� DELAY�͓��t�@�C��
	//	CString passIdAGC = m_strDay + _T("-9999");
	CString passIdAGC;
	if (m_bNewPlan)
	{
		// �V�K�v�掞�́A�J�n��������AGC�Z���E�Ǔ��f�B���C�l�v��ID�𐶐�
		passIdAGC = m_PeriodExecStartDay.Format(_T("%y%m%d")) + _T("-9999");
	}
	else
	{
		// �v��ύX���́A���̌v��ID��AGC�Z���E�Ǔ��f�B���C�l�v��ID�Ɏg��
		passIdAGC = m_PassID;
	}

	// �q�����擾
	CString strSatellite = _T("");
	m_SatelliteSelCmb.GetLBText(m_SatelliteSelCmb.GetCurSel(), strSatellite);

	// TLM�̃p�XID�擾
	CString passIdTLM;
	m_TlmPathNoEdit.GetWindowTextW(passIdTLM);
	passIdTLM = passIdTLM.Mid(0, 6) + _T("-") + passIdTLM.Mid(6, 4);

	// CMD�̃p�XID�擾
	CString passIdCMD;
	m_CmdPathNoEdit.GetWindowTextW(passIdCMD);
	passIdCMD = passIdCMD.Mid(0, 6) + _T("-") + passIdCMD.Mid(6, 4);

	// CID�ԍ��擾
	CString strCID;
	m_CidSelBtn.GetWindowTextW(strCID);

	//	l_satno	�q���ԍ�

	// �e���v�����擾
	// AGC�{DELAY
	m_AgcDelayPlanList.clear();
	int time_agc = GetClibTime(0);

	// TLM
	bool bChgPassId = false;
	m_TlmPlanList.clear();
	int time_tlm = GetClibTime(1);
	if (bChkPassId == true && m_TlmPlanList.size() > 0)
	{
		// �V�K�v��ŁATLM ID��AGC�Z���E�Ǔ��f�B���C�l�v��ID���قȂ�ꍇ
		if (m_bNewPlan == TRUE && passIdTLM != passIdAGC)
		{
			// TLM ID��AGC�Z���E�Ǔ��f�B���C�l�v��ID�ɍ��킹�邩�m�F���b�Z�[�W
			CString title;
			GetWindowText(title);
			CString strMsg;
			strMsg.Format(_T("�ȉ��̃p�X�ԍ��ݒ�ɍ��ق�����܂��B\n\n �v��J�n�����p�X�ԍ� : %s\n �e�����g���`���`�F�b�N�p�X�ԍ� : %s\n\n�e�����g���`���`�F�b�N�̃p�X�ԍ����v��J�n�����p�X�ԍ��ɍ��킹�܂����H"), passIdAGC, passIdTLM);
			if (IDYES == MessageBoxHooked(this->m_hWnd, strMsg, title, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2))
			{
				// �v��ID�����킹��
				passIdTLM = passIdAGC;
				m_TlmPathNoEdit.SetWindowTextW(passIdTLM);
				bChgPassId = true;
			}
		}
	}

	// CMD
	m_CmdPlanList.clear();
	int time_cmd = GetClibTime(2);
	if (bChkPassId == true && m_CmdPlanList.size() > 0)
	{
		// �V�K�v��ŁACMD ID��AGC�Z���E�Ǔ��f�B���C�l�v��ID���قȂ�ꍇ
		if (m_bNewPlan == TRUE && passIdCMD != passIdAGC)
		{
			// CMD ID��AGC�Z���E�Ǔ��f�B���C�l�v��ID�ɍ��킹�邩�m�F���b�Z�[�W
			CString title;
			GetWindowText(title);
			CString strMsg;
			strMsg.Format(_T("�ȉ��̃p�X�ԍ��ݒ�ɍ��ق�����܂��B\n\n �v��J�n�����p�X�ԍ� : %s\n �R�}���h�`���`�F�b�N�p�X�ԍ� : %s\n\n�R�}���h�`���`�F�b�N�̃p�X�ԍ����v��J�n�����p�X�ԍ��ɍ��킹�܂����H"), passIdAGC, passIdCMD);
			if (IDYES == MessageBoxHooked(this->m_hWnd, strMsg, title, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2))
			{
				// �v��ID�����킹��
				passIdCMD = passIdAGC;
				m_CmdPathNoEdit.SetWindowTextW(passIdCMD);
				bChgPassId = true;
			}
		}
	}

	if (bChgPassId)
	{
		// TLM��CMD�̌v��ID��ύX����ꍇ�͊֐���r���I��
		return false;
	}

	// Dummy
	vector<stCalibPlan> dmylist;
	dmylist.clear();

	stPlandata stpd;
	CTime t_start;
	CTime t_end;
	CTimeSpan span;

	stpd.sz_cid = strCID;
	stpd.l_rdychk = 0;
	stpd.l_init = 0;
	stpd.l_mod_sel = 0;
	stpd.l_auto_acq = 0;
	stpd.l_offset = 0;
	stpd.l_a_flag = 0;
	stpd.l_l_flag = 0;
	stpd.l_term_ant = 1;
	stpd.l_cmd_mod = 0;
	stpd.l_rng_mod = 0;
	stpd.l_tx_ramp = 4;
	stpd.l_rx_ramp = 4;
	stpd.l_acq_route = 7;
	stpd.l_pl_sel = 1;
	stpd.l_top_mon = 1;
	stpd.l_ent_angle = 0;
	stpd.l_ext_angle = 0;
	stpd.l_delay_sel = 2;
	stpd.l_offset_val = 0;

	UpdateData();

	t_start = CTime(m_PeriodExecStartDay.GetYear(), m_PeriodExecStartDay.GetMonth(), m_PeriodExecStartDay.GetDay(), m_PeriodExecStartTime.GetHour(), m_PeriodExecStartTime.GetMinute(), 0);

	BOOL bNew = TRUE;

	if ((passIdAGC == passIdTLM) && (passIdAGC == passIdCMD))
	{
		/* AGC(DELAY) �� TLM�ACMD�����p�XID�̏ꍇ */
		// �v��t�@�C����1�B

		// AGC(Delay) + TLM + CMD
		if (m_AgcDelayPlanList.size() > 0 || m_TlmPlanList.size() > 0 || m_CmdPlanList.size() > 0)
		{
			span = CTimeSpan(0, 0, 0, time_agc + time_tlm + time_cmd);
			t_end = t_start + span;

			stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
			stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

			if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdAGC, stpd, m_AgcDelayPlanList, m_TlmPlanList, m_CmdPlanList) != -1)
			{
				// �v��t�@�C���o�^
				COprPlnData::RegistPlanNew(strSatellite, passIdAGC, (m_PassID == passIdAGC) ? FALSE : TRUE);
			}
		}
	}
	else if ((passIdAGC == passIdTLM) && (passIdAGC != passIdCMD))
	{
		/* AGC(DELAY) �� TLM �������p�XID�ACMD���قȂ�p�XID�̏ꍇ */
		// �v��t�@�C����2�B

		// AGC(Delay) + TLM
		if (m_AgcDelayPlanList.size() > 0 || m_TlmPlanList.size() > 0)
		{
			span = CTimeSpan(0, 0, 0, time_agc + time_tlm);
			t_end = t_start + span;
			stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
			stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

			if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdAGC, stpd, m_AgcDelayPlanList, m_TlmPlanList, dmylist) != -1)
			{
				// �v��t�@�C���o�^
				COprPlnData::RegistPlanNew(strSatellite, passIdAGC, (m_PassID == passIdAGC) ? FALSE : TRUE);
			}
			t_start = t_end;
		}

		// CMD
		if (m_CmdPlanList.size() > 0)
		{
			span = CTimeSpan(0, 0, 0, time_cmd);
			t_end = t_start + span;
			stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
			stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

			if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdCMD, stpd, dmylist, dmylist, m_CmdPlanList) != -1)
			{
				// �v��t�@�C���o�^
				COprPlnData::RegistPlanNew(strSatellite, passIdCMD, (m_PassID == passIdCMD) ? FALSE : TRUE);
			}
		}
	}
	else if ((passIdAGC == passIdCMD) && (passIdAGC != passIdTLM))
	{
		/* AGC(DELAY) �� CMD �������p�XID�ATLM���قȂ�p�XID�̏ꍇ */
		// �v��t�@�C����2�B

		// AGC(Delay) + CMD
		if (m_AgcDelayPlanList.size() > 0 || m_CmdPlanList.size() > 0)
		{
			span = CTimeSpan(0, 0, 0, time_agc + time_cmd);
			t_end = t_start + span;
			stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
			stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

			if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdAGC, stpd, m_AgcDelayPlanList, dmylist, m_CmdPlanList) != -1)
			{
				// �v��t�@�C���o�^
				COprPlnData::RegistPlanNew(strSatellite, passIdAGC, (m_PassID == passIdAGC) ? FALSE : TRUE);
			}
			t_start = t_end;
		}

		// TLM
		if (m_TlmPlanList.size() > 0)
		{
			span = CTimeSpan(0, 0, 0, time_tlm);
			t_end = t_start + span;
			stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
			stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

			if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdTLM, stpd, dmylist, m_TlmPlanList, dmylist) != -1)
			{
				// �v��t�@�C���o�^
				COprPlnData::RegistPlanNew(strSatellite, passIdTLM, (m_PassID == passIdTLM) ? FALSE : TRUE);
			}
		}
	}
	else if ((passIdAGC != passIdTLM) && (passIdAGC != passIdCMD))
	{
		if (passIdTLM == passIdCMD)
		{
			/* AGC(DELAY) �� TLM �ACMD�̃p�XID���قȂ�ATLM��CMD�������p�XID�̏ꍇ */
			// �v��t�@�C����2�B

			// AGC(Delay)
			if (m_AgcDelayPlanList.size() > 0)
			{
				span = CTimeSpan(0, 0, 0, time_agc);
				t_end = t_start + span;
				stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
				stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

				if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdAGC, stpd, m_AgcDelayPlanList, dmylist, dmylist) != -1)
				{
					// �v��t�@�C���o�^
					COprPlnData::RegistPlanNew(strSatellite, passIdAGC, (m_PassID == passIdAGC) ? FALSE : TRUE);
				}
				t_start = t_end;
			}

			// TLM + CMD
			if (m_TlmPlanList.size() > 0 || m_CmdPlanList.size() > 0)
			{
				span = CTimeSpan(0, 0, 0, time_tlm + time_cmd);
				t_end = t_start + span;
				stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
				stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

				if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdTLM, stpd, dmylist, m_TlmPlanList, m_CmdPlanList) != -1)
				{
					// �v��t�@�C���o�^
					COprPlnData::RegistPlanNew(strSatellite, passIdTLM, (m_PassID == passIdTLM) ? FALSE : TRUE);
				}
			}
		}
		else
		{
			/* AGC(DELAY) �� TLM ��CMD���قȂ�p�XID�̏ꍇ */
			// �v��t�@�C����3�B

			// AGC(Delay)
			if (m_AgcDelayPlanList.size() > 0)
			{
				span = CTimeSpan(0, 0, 0, time_agc);
				t_end = t_start + span;
				stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
				stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

				if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdAGC, stpd, m_AgcDelayPlanList, dmylist, dmylist) != -1)
				{
					// �v��t�@�C���o�^
					COprPlnData::RegistPlanNew(strSatellite, passIdAGC, (m_PassID == passIdAGC) ? FALSE : TRUE);
				}
				t_start = t_end;
			}

			// TLM
			if (m_TlmPlanList.size() > 0)
			{
				span = CTimeSpan(0, 0, 0, time_tlm);
				t_end = t_start + span;
				stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
				stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

				if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdTLM, stpd, dmylist, m_TlmPlanList, dmylist) != -1)
				{
					// �v��t�@�C���o�^
					COprPlnData::RegistPlanNew(strSatellite, passIdTLM, (m_PassID == passIdTLM) ? FALSE : TRUE);
				}
				t_start = t_end;
			}

			// CMD
			if (m_CmdPlanList.size() > 0)
			{
				span = CTimeSpan(0, 0, 0, time_cmd);
				t_end = t_start + span;
				stpd.sz_start = t_start.Format(_T("%Y-%m-%d %H:%M:%S"));
				stpd.sz_end = t_end.Format(_T("%Y-%m-%d %H:%M:%S"));

				if (COprPlnData::WriteRdyChkPlanFile(strSatellite, l_satno, passIdCMD, stpd, dmylist, dmylist, m_CmdPlanList) != -1)
				{
					// �v��t�@�C���o�^
					COprPlnData::RegistPlanNew(strSatellite, passIdCMD, (m_PassID == passIdCMD) ? FALSE : TRUE);
				}
			}
		}
	}

	return true;
}

/*============================================================================*/
/*! CRdnsChk20

-# �Z���f�f��ʏ��v�����擾

@param  type = 0:AGC+Delay�A1:TLM�A2�FCMD
@retval �Ȃ�
*/
/*============================================================================*/
int CRdnsChk20::GetClibTime(int type)
{
	int time = 0;

	stCalibPlan st;
	CString ctrl, prm;

	for (int i = 0; i < (int)m_CtrlList.size(); ++i)
	{
		if (type == 0)
		{
			if ((m_CtrlList[i].type == TYPE_AGC_S)
				|| (m_CtrlList[i].type == TYPE_AGC_X)
				|| (m_CtrlList[i].type == TYPE_RNG))
			{
				time += m_CtrlList[i].time;

				st.time = m_CtrlList[i].time;
//#if 1
				AfxExtractSubString(ctrl, m_CtrlList[i].Msg1, 0, ' ');
				AfxExtractSubString(prm, m_CtrlList[i].Msg1, 1, ' ');
				st.ctrl = ctrl + _T(",") + prm;
//#else
//				st.ctrl = m_CtrlList[i].Msg1;
//#endif
				m_AgcDelayPlanList.push_back(st);
			}
		}
		else if (type == 1)
		{
			if ((m_CtrlList[i].type == TYPE_TLM_S)
				|| (m_CtrlList[i].type == TYPE_TLM_X))
			{
				time += m_CtrlList[i].time;

				st.time = m_CtrlList[i].time;
//#if 1
				AfxExtractSubString(ctrl, m_CtrlList[i].Msg1, 0, ' ');
				AfxExtractSubString(prm, m_CtrlList[i].Msg1, 1, ' ');
				st.ctrl = ctrl + _T(",") + prm;
//#else
//				st.ctrl = m_CtrlList[i].Msg1;
//#endif
				m_TlmPlanList.push_back(st);
			}
		}
		else if (type == 2)
		{
			if (m_CtrlList[i].type == TYPE_CMD)
			{
				time += m_CtrlList[i].time;

				st.time = m_CtrlList[i].time;
//#if 1
				AfxExtractSubString(ctrl, m_CtrlList[i].Msg1, 0, ' ');
				AfxExtractSubString(prm, m_CtrlList[i].Msg1, 1, ' ');
				st.ctrl = ctrl + _T(",") + prm;
//#else
//				st.ctrl = m_CtrlList[i].Msg1;
//#endif
				m_CmdPlanList.push_back(st);
			}
		}
	}

	return time;
}


/*============================================================================*/
/*! CRdnsChk20

-# CID�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnBnClickedRdns20CidBtn()
{
	CString satelliete;
	CString cid;
	m_SatelliteSelCmb.GetWindowTextW(satelliete);
	m_CidSelBtn.GetWindowTextW(cid);

	// CID�I����ʕ\��
	CCidSelect dlg;
	dlg.SetSatelliteName(satelliete);
	dlg.SetCID(cid);
	if (dlg.DoModal() == IDOK)
	{
		stCIDParam st = CCIDSlctData::TrimInvalidString(dlg.GetParameter());
		m_CidSelBtn.SetWindowTextW(st.CID);

		// �e�����g���r�b�g���[�g
		m_strTlmBRS = st.TlmBitRateS;
		m_strTlmBRX = st.TlmBitRateX;
		m_strTlmBRH = st.TlmBitRateHX;

		// �R�}���h�r�b�g���[�g
		m_strCmdBRS = st.CmdBitRateS;

		UpdateData(FALSE);

		// �I�������X�V
		ChangeRdyChkEndTime();
	}
}

/*============================================================================*/
/*! CRdnsChk20

-# ���b�Z�[�W����

@param  message	�F���b�Z�[�W�̎��ʎq
@param  wParam	�F���b�Z�[�W�̍ŏ��̃p�����[�^
@param  lParam	�F���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
@retval ��������
*/
/*============================================================================*/
LRESULT CRdnsChk20::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case eMessage_ChildDestroy:
		m_LogDlg = NULL;
		break;
	case eRdnsChk20_ResultOpen:
	{
		ResultFileOpen((int)lParam);
		break;
	}
	default:
		return CDialogBase::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}

/*============================================================================*/
/*! CRdnsChk20

-# ���O�_�C�A���O�I������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::CloseLogDlg(bool bDestroy/*=TRUE*/)
{
	if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
	{
		m_ResultDlg->DestroyWindow();
		m_ResultDlg = NULL;
	}
}


/*============================================================================*/
/*! CRdnsChk20

-# �q���I���R���{�{�b�N�X�ύX����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::OnCbnSelchangeRdns20SselCmb()
{
	CString satellite;
	int idx = m_SatelliteSelCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}

	m_SatelliteSelCmb.GetLBText(idx, satellite);

	BOOL bCid = TRUE;
	CString strCid;
	if (satellite != _T(""))
	{
		map<CString, map<CString, stCIDParam>> &cidparam = theApp.GetSatelliteData().m_CIDSlctData.GetCidParamList();
		vector<CString> &cList = theApp.GetSatelliteData().m_CIDSlctData.GetCidList(satellite);
		if (cList.size() == 0)
		{
			m_CidSelBtn.SetWindowTextW(CID_NA);
			m_CidSelBtn.EnableWindow(FALSE);
			bCid = FALSE;
		}
		else
		{
			stOpeProp stProp;
			theApp.GetSatelliteData().ReadPropertyFile(satellite, stProp);
			strCid = CString(stProp.cid);
			auto itrl = std::find(cList.begin(), cList.end(), strCid);
			if (itrl == cList.end())
			{
				// �f�t�H���gCID��CID�t�@�C���ɖ����ꍇ�A��ԍŏ���CID���Z�b�g�B
				strCid = cList[0];
			}

			m_CidSelBtn.EnableWindow(TRUE);
			m_CidSelBtn.SetWindowTextW(strCid);
		}

		// �q���ŗL���擾
		GetEqSatDBData(satellite);

		// �Z���f�f�p�R���g���[���ݒ�
		SetWindowEnable(satellite);

		if (bCid)
		{
			stCIDParam stCid = CCIDSlctData::TrimInvalidString(cidparam[satellite][strCid]);

			// �e�����g���r�b�g���[�g
			m_strTlmBRS = stCid.TlmBitRateS;
			m_strTlmBRX = stCid.TlmBitRateX;
			m_strTlmBRH = stCid.TlmBitRateHX;

			// �R�}���h�r�b�g���[�g
			m_strCmdBRS = stCid.CmdBitRateS;
		}
		else
		{
			// �e�����g���r�b�g���[�g
			m_strTlmBRS = _T("");
			m_strTlmBRX = _T("");
			m_strTlmBRH = _T("");

			// �R�}���h�r�b�g���[�g
			m_strCmdBRS = _T("");
		}

		UpdateData(FALSE);

		// �I�������X�V
		ChangeRdyChkEndTime();
	}
}

/*============================================================================*/
/*! CRdnsChk20

-# �q���ŗL���DB���擾

@param  satname�F�q����
@retval 0�F����I���A0�ȊO�F�ُ�I��
*/
/*============================================================================*/
int CRdnsChk20::GetEqSatDBData(CString satname)
{
	struct satinf_t *satinf_adr;
	struct satinf_t *original_adr;
	char *sattbl_adr;

	struct xtx_t *xtx_t_adr;			// �w�ё��M�ݔ��g�p�L�����e�[�u���A�h���X
	struct agcrx_t *agcrx_t_adr;		// �`�f�b�Z���Ώێ�M�@���e�[�u���A�h���X
	struct rarrband_t *rarrband_t_adr;	// �q�`�q�q�g�p�ш���e�[�u���A�h���X
	struct mac_t	*mac_t_adr = NULL;

	char *psz_tbl[MAC_MAX];				// �}�N�������i�[����e�[�u���̐擪�A�h���X
	char *psz_vec[MAC_MAX];				// �}�N�����A�h���X���i�[����e�[�u����

	if (satname == _T(""))
	{
		return -1;
	}

	// DB�A�N�Z�X�N���X
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();

	vector<CString>			&saNameList = dba.GetSatNameList();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();

	if (saNameList.size() == 0 || satAdrList.size() == 0)
	{
		return -1;
	}

	satinf_adr = (struct satinf_t *)((__int64*)satAdrList[satname]);
	original_adr = satinf_adr;
	sattbl_adr = (char *)satinf_adr;

	// �g�p���g����
	l_useband = satinf_adr->l_useband;

	// �ߒn���^�[�F�� (STDN,DSN)
	l_level = satinf_adr->l_level;

	// �q���ԍ�
	l_satno = satinf_adr->l_satno;

	while (1)
	{
		satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_t�ɕϊ�*/
		if (satinf_adr->l_id == ENDOFSAT_ID)
		{
			/*���ʂh�c���I���̏ꍇ*/
			break;
		}

		// �w�ё��M�ݔ��g�p�L�����
		if (satinf_adr->l_id == XTX_ID)
		{
			xtx_t_adr = (struct xtx_t *)satinf_adr;
			l_x_tx = xtx_t_adr->l_x_tx;
		}

		// �`�f�b�Z���Ώێ�M�@���
		if (satinf_adr->l_id == AGCRX_ID)
		{
			agcrx_t_adr = (struct agcrx_t *)satinf_adr;
			l_s_rx = agcrx_t_adr->l_s_rx;				// S-RX �g�p�L��
			l_sx_acqrx_s = agcrx_t_adr->l_sx_acqrx_s;	// S-ACQ �g�p�L��
			l_sx_acqrx_x = agcrx_t_adr->l_sx_acqrx_x;	// X-ACQ�g�p�L��
			l_x_rx = agcrx_t_adr->l_x_rx;				// X-RX �g�p�L��
			l_x_qpskrx = agcrx_t_adr->l_x_qpskrx;		// X-HRX �g�p�L��
			l_x_rx1 = agcrx_t_adr->l_x_rx1;				// �P�c�w�ю�M�@�P  �L���i���`�P�j
			l_x_rx2 = agcrx_t_adr->l_x_rx2;				// �P�c�w�ю�M�@�Q  �L���i���`�P�j
		}

		// �w�q�`�q�q�g�p�ш���
		if (satinf_adr->l_id == RARRBAND_ID)
		{
			rarrband_t_adr = (struct rarrband_t *)satinf_adr;
			l_rarr_band = rarrband_t_adr->l_rarr_band;
		}

		// MAC
		if (satinf_adr->l_id == MAC_ID)
		{
			/*���ʂh�c���̏ꍇ*/
			mac_t_adr = (struct mac_t *)satinf_adr;

			CString mac = CString(mac_t_adr->sz_macname);
			if (mac.Find(MAC_INIT_SSTCRDEM) == 0)	// TLM S��
			{
				i_tlm_s_frm = 0;
				i_tlm_s_lck = 2;
				int i_ret = dba.ud_macstsadr_to_exectbl(sattbl_adr, psz_tbl, psz_vec, MAC_MAX);
				for (int i = 0; i < i_ret; ++i)
				{
					CString tmp = CString(psz_tbl[i]);
					CString tag, val;
					AfxExtractSubString(tag, tmp, 0, ',');
					tag.TrimRight();
					AfxExtractSubString(val, tmp, 1, ',');
					val.TrimRight();

					if (tag == MAC_SSTCRDEM_FRM)
					{
						i_tlm_s_frm = _wtoi(val);
					}
					else if (tag == MAC_SSTCRDEM_LCK)
					{
						i_tlm_s_lck = _wtoi(val);
					}
				}
			}
			else if (mac.Find(MAC_INIT_X_HRX_DEM) == 0)	// TLM X�э���
			{
				i_tlm_xh_frm = 0;
				i_tlm_xh_lck = 2;
				int i_ret = dba.ud_macstsadr_to_exectbl(sattbl_adr, psz_tbl, psz_vec, MAC_MAX);
				for (int i = 0; i < i_ret; ++i)
				{
					CString tmp = CString(psz_tbl[i]);
					CString tag, val;
					AfxExtractSubString(tag, tmp, 0, ',');
					tag.TrimRight();
					AfxExtractSubString(val, tmp, 1, ',');
					val.TrimRight();

					if (tag == MAC_X_HRX_DEM_FRM)
					{
						i_tlm_xh_frm = _wtoi(val);
					}
					else if (tag == MAC_X_HRX_DEM_LCK)
					{
						i_tlm_xh_lck = _wtoi(val);
					}
				}
			}
			else if (mac.Find(MAC_INIT_SSTCRMOD) == 0)	// CMD S��
			{
				i_cmd_s_frm = 0;
				int i_ret = dba.ud_macstsadr_to_exectbl(sattbl_adr, psz_tbl, psz_vec, MAC_MAX);
				for (int i = 0; i < i_ret; ++i)
				{
					CString tmp = CString(psz_tbl[i]);
					CString tag, val;
					AfxExtractSubString(tag, tmp, 0, ',');
					tag.TrimRight();
					AfxExtractSubString(val, tmp, 1, ',');
					val.TrimRight();

					if (tag == MAC_SSTCRMOD_FRM)
					{
						i_cmd_s_frm = _wtoi(val);
					}
				}
			}
		}
		sattbl_adr += satinf_adr->l_len;
	}

	satinf_adr = original_adr;				/*�q���{�̏��擪�A�h���X�Z�b�g*/

	/*-----------------*/
	/* S BITRATE�̎擾 */
	/*-----------------*/
	i_bitrate_s = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(BIT_S).GetBuffer(), psz_tbl, psz_vec, MAC_MAX);

	/*-----------------*/
	/* X BITRATE�̎擾 */
	/*-----------------*/
	i_bitrate_x = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(BIT_X).GetBuffer(), psz_tbl, psz_vec, MAC_MAX);

	/*-----------------*/
	/* XQPSK BITRATE�̎擾 */
	/*-----------------*/
	i_bitrate_xqpsk = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(BIT_XQPSK20).GetBuffer(), psz_tbl, psz_vec, MAC_MAX);

	return 0;
}

/*============================================================================*/
/*! CRdnsChk20

-# �Z���f�f�p�R���g���[������/�񊈐��ݒ�

@param  satname �F �q����
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsChk20::SetWindowEnable(CString satname)
{
	/********************************************/
	/* AGC �Z�� */
	m_AgcChkValue = FALSE;

	// S-ACQ
	m_AgcSACQChkValue = FALSE;
	m_AgcSACQAChkValue = FALSE;
	m_AgcSACQBChkValue = FALSE;
	if (l_sx_acqrx_s == AGCRX_Y_ID)
	{
		m_AGC_S_ACQCtrl.EnableWindow(TRUE);
		m_AGC_S_ACQ_ACtrl.EnableWindow(TRUE);
		m_AGC_S_ACQ_BCtrl.EnableWindow(TRUE);
	}
	else
	{
		m_AGC_S_ACQCtrl.EnableWindow(FALSE);
		m_AGC_S_ACQ_ACtrl.EnableWindow(FALSE);
		m_AGC_S_ACQ_BCtrl.EnableWindow(FALSE);
	}

	// SS-TCR
	m_AgcSTCRChkValue = FALSE;
	m_AgcSTCRAChkValue = FALSE;
	m_AgcSTCRBChkValue = FALSE;
	if (l_s_rx == AGCRX_Y_ID)
	{
		m_AGC_S_TCRCtrl.EnableWindow(TRUE);
		m_AGC_S_TCR_ACtrl.EnableWindow(TRUE);
		m_AGC_S_TCR_BCtrl.EnableWindow(TRUE);
	}
	else
	{
		m_AGC_S_TCRCtrl.EnableWindow(FALSE);
		m_AGC_S_TCR_ACtrl.EnableWindow(FALSE);
		m_AGC_S_TCR_BCtrl.EnableWindow(FALSE);
	}

	// S-DET
	m_AgcSDETChkValue = FALSE;
	m_AgcSDETAChkValue = FALSE;
	m_AgcSDETBChkValue = FALSE;
	if (l_s_rx == AGCRX_Y_ID)
	{
		m_AGC_S_DETCtrl.EnableWindow(TRUE);
		m_AGC_S_DET_ACtrl.EnableWindow(TRUE);
		m_AGC_S_DET_BCtrl.EnableWindow(TRUE);
	}
	else
	{
		m_AGC_S_DETCtrl.EnableWindow(FALSE);
		m_AGC_S_DET_ACtrl.EnableWindow(FALSE);
		m_AGC_S_DET_BCtrl.EnableWindow(FALSE);
	}

	// SX-TCR
	m_AgcSXTCRChkValue = FALSE;
	m_AgcSXTCRAChkValue = FALSE;
	m_AgcSXTCRBChkValue = FALSE;
	if (l_x_rx == AGCRX_Y_ID)
	{
		m_AGC_SX_TCRCtrl.EnableWindow(TRUE);
		m_AGC_SX_TCR_ACtrl.EnableWindow(TRUE);
		m_AGC_SX_TCR_BCtrl.EnableWindow(TRUE);
	}
	else
	{
		m_AGC_SX_TCRCtrl.EnableWindow(FALSE);
		m_AGC_SX_TCR_ACtrl.EnableWindow(FALSE);
		m_AGC_SX_TCR_BCtrl.EnableWindow(FALSE);
	}

	// X-HRX
	m_AgcXHRXChkValue = FALSE;
	if (l_x_qpskrx == AGCRX_Y_ID)
	{
		m_AGC_X_HRXCtrl.EnableWindow(TRUE);
	}
	else
	{
		m_AGC_X_HRXCtrl.EnableWindow(FALSE);
	}


	/********************************************/
	/* �Ǔ��f�B���C�l�v�� */
	m_DlyChkValue = FALSE;
	m_DlySSChkValue = FALSE;
	m_DlySXChkValue = FALSE;
	if (l_rarr_band == RARRBAND_S_ID)
	{
		m_Delay_SSCtrl.EnableWindow(TRUE);
		m_Delay_SXCtrl.EnableWindow(FALSE);
	}
	else if (l_rarr_band == RARRBAND_X_ID)
	{
		m_Delay_SSCtrl.EnableWindow(FALSE);
		m_Delay_SXCtrl.EnableWindow(FALSE);
	}
	else if (l_rarr_band == RARRBAND_SX_ID)
	{
		m_Delay_SSCtrl.EnableWindow(TRUE);
		m_Delay_SXCtrl.EnableWindow(TRUE);
	}
	else if (l_rarr_band == RARRBAND_RX_ID)
	{
		m_Delay_SSCtrl.EnableWindow(FALSE);
		m_Delay_SXCtrl.EnableWindow(FALSE);
	}


	/********************************************/
	/* �e�����g���`���`�F�b�N */
	m_TLMChkValue = FALSE;

	// S BITRATE
	m_TLMSChkValue = FALSE;
	if (i_bitrate_s < 0)
	{
		m_TLM_SCtrl.EnableWindow(FALSE);
	}
	else
	{
		m_TLM_SCtrl.EnableWindow(TRUE);
	}

	// X BITRATE
	m_TLMXChkValue = FALSE;
	if (i_bitrate_x < 0)
	{
		m_TLM_XCtrl.EnableWindow(FALSE);
	}
	else
	{
		m_TLM_XCtrl.EnableWindow(TRUE);
	}

	// X-HRX BITRATE
	m_TLMXHChkValue = FALSE;
	if (i_bitrate_xqpsk < 0)
	{
		m_TLM_X_HRXCtrl.EnableWindow(FALSE);
	}
	else
	{
		m_TLM_X_HRXCtrl.EnableWindow(TRUE);
	}


	/********************************************/
	/* �R�}���h�`���`�F�b�N */
	m_CmdChkValue = FALSE;

	m_CmdOut1ChkValue = FALSE;
	if (l_x_tx == XTX_Y_ID)
	{
		m_CMD_OUT1Ctrl.EnableWindow(FALSE);
	}
	else
	{
		m_CMD_OUT1Ctrl.EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}

/*============================================================================*/
/*! CRdnsChk20

-# ����R�}���h���M�X���b�h

@param
@retval

*/
/*============================================================================*/
UINT WINAPI CRdnsChk20::SendCtrlThread(LPVOID pParam)
{
	CRdnsChk20* pThis = reinterpret_cast<CRdnsChk20*>(pParam);
	_ASSERTE(pThis != NULL);

	vector<stSendCtrlMsg>::iterator itr, itr2, itr_tmp;

	UINT nStation = theApp.GetSelectStation();

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();
	CString res;

	// �Z���f�f���s���t���OON
	pThis->m_ThreadRun = true;

	// �ŏ��^�C�}����\��ݒ�(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = 0;
	bool bTimeout = false;

	while (1/*pThis->ThreadEnd() == false*/)
	{
		bool bRunWait = false;
		// ���X�g�ɓo�^����Ă���R�}���h�����s���̍��ڂ����݂���Αҋ@����
		for (itr = pThis->m_CtrlList.begin(); itr != pThis->m_CtrlList.end(); ++itr)
		{
			if (pThis->ThreadAbort() == true)
			{
				break;
			}

			if ((*itr).state == STATE_WAIT || (*itr).state == STATE_RUN)
			{
				// �R�}���h�I���ҋ@
				bRunWait = true;
				break;
			}
		}

		if (pThis->ThreadAbort() == true)
		{
			break;
		}

		if (itr != pThis->m_CtrlList.end())
		{
			// ���s���̃R�}���h�����݂����̂őҋ@
			while (pThis->ThreadAbort() == false)
			{
				// �I���ҋ@
				// ���s���̃R�}���h���I�����������`�F�b�N����
				// theApp����R�}���h���I�����������m�F����
				if (theApp.GetControlCommand(nStation, mRdnsRunLogItem, (*itr).MonString) == STATE_STP)
				{
					// �^�C�}�[�N���A
					dwStart = 0;

					// �R�}���h���I�����Ă�̂ŁuSTATE_STP�v��ݒ肷��
					(*itr).state = STATE_STP;

					// ���ʃt�@�C���\��
					if ((*itr).type == TYPE_AGC_S)
					{
						// ���ʎ擾
						obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), RESULT_AGCS);
						if (obs_adr != NULL)
						{
							res = CString(obs_adr->sz_sts_name);

							// ���ʃ_�C�A���O�փ��b�Z�[�W���M
							if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
							{
								pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
							}
						}
//#ifdef _DEBUG
//						res = _T("OK");
//
//						// ���ʃ_�C�A���O�փ��b�Z�[�W���M
//						if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
//						{
//							pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
//						}
//#endif

						if (pThis->m_VResult.agc_s.bResult == TRUE)
						{
							pThis->m_VResult.agc_s.count--;
							if (pThis->m_VResult.agc_s.count <= 0)
							{
								// AGC S ���ʃt�@�C���\��
								pThis->SendMessage(eRdnsChk20_ResultOpen, 0, (LPARAM)TYPE_AGC_S);
								pThis->m_VResult.agc_s.bResult = FALSE;
							}
						}
					}
					else if ((*itr).type == TYPE_AGC_X)
					{
						// ���ʎ擾
						obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), RESULT_AGCX);
						if (obs_adr != NULL)
						{
							res = CString(obs_adr->sz_sts_name);

							// ���ʃ_�C�A���O�փ��b�Z�[�W���M
							if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
							{
								pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
							}
						}
//#ifdef _DEBUG
//						res = _T("OK");
//
//						// ���ʃ_�C�A���O�փ��b�Z�[�W���M
//						if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
//						{
//							pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
//						}
//#endif

						if (pThis->m_VResult.agc_x.bResult == TRUE)
						{
							pThis->m_VResult.agc_x.count--;
							if (pThis->m_VResult.agc_x.count <= 0)
							{
								// AGC X ���ʃt�@�C���\��
								pThis->SendMessage(eRdnsChk20_ResultOpen, 0, (LPARAM)TYPE_AGC_X);
								pThis->m_VResult.agc_x.bResult = FALSE;
							}
						}
					}
					else if ((*itr).type == TYPE_RNG)
					{
						// ���ʎ擾
						obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), RESULT_RNG);
						if (obs_adr != NULL)
						{
							res = CString(obs_adr->sz_sts_name);

							// ���ʃ_�C�A���O�փ��b�Z�[�W���M
							if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
							{
								pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
							}
						}
//#ifdef _DEBUG
//						res = _T("OK");
//
//						// ���ʃ_�C�A���O�փ��b�Z�[�W���M
//						if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
//						{
//							pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
//						}
//#endif

						if (pThis->m_VResult.delay.bResult == TRUE)
						{
							pThis->m_VResult.delay.count--;
							if (pThis->m_VResult.delay.count <= 0)
							{
								// Delay ���ʃt�@�C���\��
								pThis->SendMessage(eRdnsChk20_ResultOpen, 0, (LPARAM)TYPE_RNG);
								pThis->m_VResult.delay.bResult = FALSE;
							}
						}
					}
					else if (((*itr).type == TYPE_TLM_S) || ((*itr).type == TYPE_TLM_X))
					{
						// ���ʎ擾
						obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), RESULT_TLM);
						if (obs_adr != NULL)
						{
							res = CString(obs_adr->sz_sts_name);

							// ���ʃ_�C�A���O�փ��b�Z�[�W���M
							if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
							{
								pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
							}
						}
//#ifdef _DEBUG
//						res = _T("OK");
//
//						// ���ʃ_�C�A���O�փ��b�Z�[�W���M
//						if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
//						{
//							pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
//						}
//#endif

						if (pThis->m_VResult.tlm.bResult == TRUE)
						{
							pThis->m_VResult.tlm.count--;
							if (pThis->m_VResult.tlm.count <= 0)
							{
								// Tlm ���ʃt�@�C���\��
								pThis->SendMessage(eRdnsChk20_ResultOpen, 0, (LPARAM)TYPE_TLM_S);
								pThis->m_VResult.tlm.bResult = FALSE;
							}
						}
					}
					else if ((*itr).type == TYPE_CMD)
					{
						// ���ʎ擾
						obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), RESULT_CMD);
						if (obs_adr != NULL)
						{
							res = CString(obs_adr->sz_sts_name);

							// ���ʃ_�C�A���O�փ��b�Z�[�W���M
							if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
							{
								pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
							}
						}
//#ifdef _DEBUG
//						res = _T("OK");
//
//						// ���ʃ_�C�A���O�փ��b�Z�[�W���M
//						if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
//						{
//							pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
//						}
//#endif

						if (pThis->m_VResult.cmd.bResult == TRUE)
						{
							pThis->m_VResult.cmd.count--;
							if (pThis->m_VResult.cmd.count <= 0)
							{
								// Cmd ���ʃt�@�C���\��
								pThis->SendMessage(eRdnsChk20_ResultOpen, 0, (LPARAM)TYPE_CMD);
								pThis->m_VResult.cmd.bResult = FALSE;
							}
						}
					}

					break;
				}

				// ����J�n����̌o�ߎ��ԁi�~���b�j�̌v�Z
				if (dwStart != 0 &&
					timeGetTime() - dwStart >= 30000 &&
					theApp.GetControlCommand(nStation, mRdnsRunLogItem, (*itr).MonString) < STATE_RUN)
				{
					// 30�b�𒴂��Ă��Ă��J�n�ɂȂ��Ă��Ȃ��ꍇ�́A�^�C���A�E�g����
					// ���C���t���[���փ��b�Z�[�W���M
					theApp.m_pMainWnd->PostMessageW(eMessage_RdnsChkTimeout, 0, 0);

					// �X���b�h�I��
					pThis->m_ThreadAbort = true;
					bTimeout = true;
					break;
				}

//#if 0
//				if (pThis->RdnsChkAbort() == true)
//				{
//					theApp.SendControlNoUI((*itr).MsgCancel, _T(""), _T(""), _T(""));
//					pThis->RdnsChkAbortClear();
//				}
//#endif

				Sleep(10);
			}

			if (pThis->ThreadAbort() == true)
			{
				break;
			}
		}
		else
		{
			// ���s���̃R�}���h�����݂��Ȃ��̂Ő擪����uSTATE_PRE�v�̃R�}���h�����s����
			for (itr = pThis->m_CtrlList.begin(); itr != pThis->m_CtrlList.end(); ++itr)
			{
				if (pThis->ThreadAbort() == true)
					break;

				if ((*itr).state == STATE_PRE)
				{
//#if 0	// AGC S�т�X�т��������s�ł���̂�34m����
//					// AGC S��
//					if ((*itr).type == TYPE_AGC_S)
//					{
//						bool bNext = false;
//						bool bRun = false;
//
//						// �R�}���h���s
//						theApp.SendControlNoUI((*itr).Msg1, _T(""), _T(""), _T(""));
//
//						// theApp�ɃR�}���h���s�������Ƃ�ʒm
//						(*itr).item = mRdnsRunLogItem;
//						(*itr).state = STATE_WAIT;
//						(*itr).station = nStation;
//						theApp.SetControlCommand((*itr));
//
//						// AGC X�т����s���Ŗ�����Γ������s����B
//						for (itr2 = itr; itr2 != pThis->m_CtrlList.end(); ++itr2)
//						{
//							if ((*itr2).type == TYPE_AGC_X)
//							{
//								if (((*itr2).state == STATE_WAIT) || ((*itr2).state == STATE_RUN))
//								{
//									bRun = true;
//									break;
//								}
//								if ((*itr2).state == STATE_PRE)
//								{
//									if (bNext == false)
//									{
//										itr_tmp = itr2;
//										bNext = true;
//									}
//								}
//							}
//						}
//						if (bRun == false && bNext == true)
//						{
//							// �R�}���h���s(AGC S��X�͓������s�\)
//							theApp.SendControlNoUI((*itr_tmp).Msg1, _T(""), _T(""), _T(""));
//
//							// theApp�ɃR�}���h���s�������Ƃ�ʒm
//							(*itr_tmp).item = mRdnsRunLogItem;
//							(*itr_tmp).state = STATE_WAIT;
//							(*itr_tmp).station = nStation;
//							theApp.SetControlCommand((*itr_tmp));
//						}
//						break;
//					}
//					// AGC X��
//					else if ((*itr).type == TYPE_AGC_X)
//					{
//						bool bNext = false;
//						bool bRun = false;
//
//						// �R�}���h���s
//						theApp.SendControlNoUI((*itr).Msg1, _T(""), _T(""), _T(""));
//
//						// theApp�ɃR�}���h���s�������Ƃ�ʒm
//						(*itr).item = mRdnsRunLogItem;
//						(*itr).state = STATE_WAIT;
//						(*itr).station = nStation;
//						theApp.SetControlCommand((*itr));
//
//						// AGC S�т����s���Ŗ�����Γ������s����B
//						for (itr2 = itr; itr2 != pThis->m_CtrlList.end(); ++itr2)
//						{
//							if ((*itr2).type == TYPE_AGC_S)
//							{
//								if (((*itr2).state == STATE_WAIT) || ((*itr2).state == STATE_RUN))
//								{
//									bRun = true;
//									break;
//								}
//								if ((*itr2).state == STATE_PRE)
//								{
//									if (bNext == false)
//									{
//										itr_tmp = itr2;
//										bNext = true;
//									}
//								}
//							}
//						}
//						if (bRun == false && bNext == true)
//						{
//							// �R�}���h���s(AGC S��X�͓������s�\)
//							theApp.SendControlNoUI((*itr_tmp).itr_tmp, _T(""), _T(""), _T(""));
//
//							// theApp�ɃR�}���h���s�������Ƃ�ʒm
//							(*itr_tmp).item = mRdnsRunLogItem;
//							(*itr_tmp).state = STATE_WAIT;
//							(*itr_tmp).station = nStation;
//							theApp.SetControlCommand((*itr_tmp));
//						}
//						break;
//					}
//					else
//#endif
					{
						// �R�}���h���s
						theApp.SendControlNoUI((*itr).Msg1, _T(""), _T(""), _T(""));

						// theApp�ɃR�}���h���s�������Ƃ�ʒm
						(*itr).item = mRdnsRunLogItem;
						(*itr).state = STATE_WAIT;
						(*itr).station = nStation;
						theApp.SetControlCommand((*itr));

						// �^�C�}�[�Z�b�g
						dwStart = timeGetTime();

						break;
					}
				}
			}

			if (itr == pThis->m_CtrlList.end())
			{
				// �S�ẴR�}���h�����s���ꂽ�B
				break;
			}

			if (pThis->ThreadAbort() == true)
			{
				break;
			}
		}
	}

	if (pThis->ThreadAbort() == true)
	{
		CString strCtrl;
		CString res = _T("NG");
		int flg = 0;
		for (itr = pThis->m_CtrlList.begin(); itr != pThis->m_CtrlList.end(); ++itr)
		{
			if ((*itr).state == STATE_WAIT || (*itr).state == STATE_RUN)
			{
				// �u�����I���v�����s���ꂽ�̂Ŏ��s���̃R�}���h�������I��������
				// �����I���V�F�������s

//#if 1
				if (pThis->RdnsChkAbort() == false)
				{
					if (bTimeout == false)
					{
						// �^�C���A�E�g�łȂ��ꍇ�́A�Z���f�f�L�����Z������𑗐M
						theApp.SendControlNoUI((*itr).MsgCancel, _T(""), _T(""), _T(""));
					}

					if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
					{
						pThis->m_ResultDlg->SendMessage(eRdnsChk_Result, (WPARAM)&(*itr).RdnsItem, (LPARAM)&res);
					}

					// ���ʃt�@�C���\��
					if ((*itr).type == TYPE_AGC_S)
					{
						if (pThis->m_VResult.agc_s.bResult == TRUE)
						{
							// AGC S ���ʃt�@�C���\��
							pThis->SendMessage(eRdnsChk20_ResultOpen, 0, (LPARAM)TYPE_AGC_S);
							pThis->m_VResult.agc_s.bResult = FALSE;
						}
					}
					else if ((*itr).type == TYPE_AGC_X)
					{
						if (pThis->m_VResult.agc_x.bResult == TRUE)
						{
							// AGC X ���ʃt�@�C���\��
							pThis->SendMessage(eRdnsChk20_ResultOpen, 0, (LPARAM)TYPE_AGC_X);
							pThis->m_VResult.agc_x.bResult = FALSE;
						}
					}
					else if ((*itr).type == TYPE_RNG)
					{
						if (pThis->m_VResult.delay.bResult == TRUE)
						{
							// Delay ���ʃt�@�C���\��
							pThis->SendMessage(eRdnsChk20_ResultOpen, 0, (LPARAM)TYPE_RNG);
							pThis->m_VResult.delay.bResult = FALSE;
						}
					}
					else if (((*itr).type == TYPE_TLM_S) || ((*itr).type == TYPE_TLM_X))
					{
						if (pThis->m_VResult.tlm.bResult == TRUE)
						{
							// Tlm ���ʃt�@�C���\��
							pThis->SendMessage(eRdnsChk20_ResultOpen, 0, (LPARAM)TYPE_TLM_S);
							pThis->m_VResult.tlm.bResult = FALSE;
						}
					}
					else if ((*itr).type == TYPE_CMD)
					{
						if (pThis->m_VResult.cmd.bResult == TRUE)
						{
							// Cmd ���ʃt�@�C���\��
							pThis->SendMessage(eRdnsChk20_ResultOpen, 0, (LPARAM)TYPE_CMD);
							pThis->m_VResult.cmd.bResult = FALSE;
						}
					}
				}
//#else
//				if (((flg & 0x01) == 0) && ((*itr).type == TYPE_AGC_S || (*itr).type == TYPE_AGC_X))
//				{
//					// AGC
//					strCtrl.Format(CTRL_AGC_CANCEL, (LPCTSTR)(*itr).satellite);
//					theApp.SendControlNoUI(strCtrl, _T(""), _T(""), _T(""));
//					flg |= 0x01;
//				}
//				else if (((flg & 0x02) == 0) && ((*itr).type == TYPE_RNG))
//				{
//					// RNG
//					strCtrl.Format(CTRL_RNG_CANCEL, (LPCTSTR)(*itr).satellite);
//					theApp.SendControlNoUI(strCtrl, _T(""), _T(""), _T(""));
//					flg |= 0x02;
//				}
//				else if (((flg & 0x04) == 0) && ((*itr).type == TYPE_TLM_S || (*itr).type == TYPE_TLM_X))
//				{
//					// TLM
//					strCtrl.Format(CTRL_TLM_CANCEL, (LPCTSTR)(*itr).satellite);
//					theApp.SendControlNoUI(strCtrl, _T(""), _T(""), _T(""));
//					flg |= 0x04;
//				}
//				else if (((flg & 0x08) == 0) && ((*itr).type == TYPE_CMD))
//				{
//					// CMD
//					strCtrl.Format(CTRL_CMD_CANCEL, (LPCTSTR)(*itr).satellite);
//					theApp.SendControlNoUI(strCtrl, _T(""), _T(""), _T(""));
//					flg |= 0x08;
//				}
//#endif
				// theApp�ɑ΂��ďI���������s��
				theApp.ClearControlCommand(nStation, mRdnsRunLogItem, (*itr).MonString);
			}
		}
		pThis->m_CtrlList.clear();
	}

	if (pThis->RdnsChkAbort() == false)
	{
		// ���s�{�^��������
		pThis->GetDlgItem(IDC_RDNS20_REALTIME_RUNBTN)->EnableWindow(TRUE);

		// �o�^�{�^��������
		pThis->GetDlgItem(IDC_RDNS20_PERIOD_ENTRYBTN)->EnableWindow(TRUE);

		// �����I���{�^���񊈐���
		pThis->m_AbortBtnCtr.EnableWindow(FALSE);

		// �q���I���R���{�{�b�N�X������
		pThis->m_SatelliteSelCmb.EnableWindow(TRUE);

		// CID�I���{�^��������
		pThis->m_CidSelBtn.EnableWindow(TRUE);

		if (pThis->m_ResultDlg != NULL && pThis->m_ResultDlg->GetSafeHwnd() != NULL)
		{
			pThis->m_ResultDlg->SendMessage(eRdnsChk_End, 0, 0);
		}
	}

	// �Z���f�f���s���t���OOFF
	pThis->m_ThreadRun = false;

	return 0;
}

/*============================================================================*/
/*! CRdnsChk20

-# ���s�\��-�Z���f�f�I���\������ύX

@param
@retval

*/
/*============================================================================*/
void CRdnsChk20::ChangeRdyChkEndTime()
{
	// �`�F�b�N�L���m�F
	UpdateData();

	CRdnsChkData &rdy = theApp.GetSatelliteData().m_CRdnsChkData;
	CDBAccess &dba = theApp.GetSatelliteData().GetDBAccessCls();

	m_TimeSpan = 0;

	// AGC�Z��
	if (m_AgcChkValue)
	{
		// S-ACQ
		if (m_AgcSACQChkValue)
		{
			if (m_AgcSACQAChkValue || m_AgcSACQBChkValue)
			{
				m_TimeSpan += dba.GetRdyChkTimeFile(_T("AGC_S")) * 60;
			}
		}

		// SS-TCR
		if (m_AgcSTCRChkValue)
		{
			if (m_AgcSTCRAChkValue || m_AgcSTCRBChkValue)
			{
				m_TimeSpan += dba.GetRdyChkTimeFile(_T("AGC_S")) * 60;
			}
		}

		// S-DET
		if (m_AgcSDETChkValue)
		{
			if (m_AgcSDETAChkValue || m_AgcSDETBChkValue)
			{
				m_TimeSpan += dba.GetRdyChkTimeFile(_T("AGC_S")) * 60;
			}
		}

		// SX-TCR
		if (m_AgcSXTCRChkValue)
		{
			if (m_AgcSXTCRAChkValue || m_AgcSXTCRBChkValue)
			{
				m_TimeSpan += dba.GetRdyChkTimeFile(_T("AGC_X")) * 60;
			}
		}

		// X-HRX
		if (m_AgcXHRXChkValue)
		{
			m_TimeSpan += dba.GetRdyChkTimeFile(_T("AGC_XQ")) * 60;
		}
	}

	// �Ǔ��f�B���C�l�v��
	if (m_DlyChkValue)
	{
		// S/S
		if (m_DlySSChkValue)
		{
			m_TimeSpan += dba.GetRdyChkTimeFile(_T("RNG_SS")) * 60;
		}

		// S/X
		if (m_DlySXChkValue)
		{
			m_TimeSpan += dba.GetRdyChkTimeFile(_T("RNG_SX")) * 60;
		}
	}

	// �e�����g���`���`�F�b�N
	if (m_TLMChkValue)
	{
		// S-TLMIN
		if (m_TLMSChkValue)
		{
//#if 1
			m_TimeSpan += dba.GetRdyChkTimeFile(_T("TLM_S")) * 60;
//#else
//			double time = 0.0;
//			if (m_strTlmBRS.IsEmpty() == FALSE)
//			{
//				time = i_tlm_s_frm * 8 / rdy.BitrateStrToDouble(m_strTlmBRS) * i_tlm_s_lck + 30;
//			}
//			m_TimeSpan += int(ceil(time));
//#endif
		}

		// X-TLMIN
		if (m_TLMXChkValue)
		{
			m_TimeSpan += dba.GetRdyChkTimeFile(_T("TLM_X")) * 60;
		}

		// X-HRXTLMIN
		if (m_TLMXHChkValue)
		{
//#if 1
			m_TimeSpan += dba.GetRdyChkTimeFile(_T("TLM_XQ")) * 60;
//#else
//			double time = 0.0;
//			if (m_strTlmBRH.IsEmpty() == FALSE)
//			{
//				time = i_tlm_xh_frm * 8 / rdy.BitrateStrToDouble(m_strTlmBRH) * i_tlm_xh_lck + 30;
//			}
//			m_TimeSpan += int(ceil(time));
//#endif
		}
	}

	// �R�}���h�`���`�F�b�N
	if (m_CmdChkValue)
	{
		// CMD OUT
		if (m_CmdOut1ChkValue)
		{
//#if 1
			m_TimeSpan += dba.GetRdyChkTimeFile(_T("CMD_S")) * 60;
//#else
//			double time = 0.0;
//			if (m_strCmdBRS.IsEmpty() == FALSE)
//			{
//				time = i_cmd_s_frm * 8 / rdy.BitrateStrToDouble(m_strCmdBRS) + 30;
//			}
//			m_TimeSpan += int(ceil(time));
//#endif
		}
	}

	SetRdyChkEndTime();

	return;
}


/*============================================================================*/
/*! CRdnsChk20

-# �Z���f�f�I���\������ݒ�

@param
@retval

*/
/*============================================================================*/
void CRdnsChk20::SetRdyChkEndTime()
{
	UpdateData();

	CTime sTime = CTime(m_PeriodExecStartDay.GetYear(), m_PeriodExecStartDay.GetMonth(), m_PeriodExecStartDay.GetDay(), m_PeriodExecStartTime.GetHour(), m_PeriodExecStartTime.GetMinute(), 0);
	CTimeSpan span(0, 0, 0, m_TimeSpan);
	CTime eTime = sTime + span;

	m_PeriodExecEndDay = eTime;
	m_PeriodExecEndTime = eTime;

	UpdateData(FALSE);
}


/*============================================================================*/
/*! CRdnsChk20

-# �Z���f�f�J�n���� �N���� �ύX�ʒm

@param
@retval

*/
/*============================================================================*/
void CRdnsChk20::OnDtnDatetimechangeRdns20PeriodStimeDayDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	SetRdyChkEndTime();

	if (m_bNewPlan)
	{
		// �V�K�v��̏ꍇ�A�Z���f�f�v��p�X�ԍ������킹��
		CString strCurPassID = m_PeriodExecStartDay.Format(_T("%y%m%d-9999"));
		CString strTlmPass = _T("");

		m_TlmPathNoEdit.GetWindowTextW(strTlmPass);
		strTlmPass = strTlmPass.Mid(0, 6) + _T("-") + strTlmPass.Mid(6);	// XXXXXXXXXX ��XXXXXX-XXXX �`���ϊ�
		if (strTlmPass == m_strTlmPassID)
		{
			// �e�����g���E�p�X�ԍ���ύX���Ă��Ȃ��ꍇ�A�Z���f�f�v��p�X�ԍ��ɍ��킹��
			m_strTlmPassID = strCurPassID;
			m_TlmPathNoEdit.SetWindowTextW(m_strTlmPassID);
			m_TlmPathNoEdit.UpdateData(FALSE);
		}
		CString strCmdPass = _T("");
		m_CmdPathNoEdit.GetWindowTextW(strCmdPass);
		strCmdPass = strCmdPass.Mid(0, 6) + _T("-") + strCmdPass.Mid(6);	// XXXXXXXXXX ��XXXXXX-XXXX �`���ϊ�
		if (strCmdPass == m_strCmdPassID)
		{
			// �R�}���h�E�p�X�ԍ���ύX���Ă��Ȃ��ꍇ�A�Z���f�f�v��p�X�ԍ��ɍ��킹��
			m_strCmdPassID = strCurPassID;
			m_CmdPathNoEdit.SetWindowTextW(m_strCmdPassID);
			m_CmdPathNoEdit.UpdateData(FALSE);
		}
	}

	*pResult = 0;
}


/*============================================================================*/
/*! CRdnsChk20

-# �Z���f�f�J�n���� ���� �ύX�ʒm

@param
@retval

*/
/*============================================================================*/
void CRdnsChk20::OnDtnDatetimechangeRdns20PeriodStimeTimeDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	SetRdyChkEndTime();

	*pResult = 0;
}


/*============================================================================*/
/*! CRdnsChk20

-# �Z���f�f���ʃt�@�C���\��

@param	type :�Z���f�f���
@retval

*/
/*============================================================================*/
void CRdnsChk20::ResultFileOpen(int type)
{
	if (type == TYPE_AGC_S)
	{
		SetTimer(TID_RES_AGCS, INTERVAL_RES, 0);
	}
	else if (type == TYPE_AGC_X)
	{
		SetTimer(TID_RES_AGCX, INTERVAL_RES, 0);
	}
	else if (type == TYPE_RNG)
	{
		SetTimer(TID_RES_RNG, INTERVAL_RES, 0);
	}
	else if (type == TYPE_TLM_S)
	{
		SetTimer(TID_RES_TLM, INTERVAL_RES, 0);
	}
	else if (type == TYPE_CMD)
	{
		SetTimer(TID_RES_CMD, INTERVAL_RES, 0);
	}
	else
	{
		return;
	}
}


/*============================================================================*/
/*! CRdnsChk20

-# �f�o�b�O�p

@param
@retval

*/
/*============================================================================*/
void CRdnsChk20::MonitoringRdnsRunState()
{
	KillTimer(TID_RDNSSTATE);

	UINT nStation = theApp.GetSelectStation();

	for (auto itr = m_CtrlList.begin(); itr != m_CtrlList.end(); ++itr)
	{
		if (theApp.GetControlCommand(nStation, mRdnsRunLogItem, (*itr).MonString) == STATE_WAIT)
		{
			theApp.UpdateControlCommand(nStation, (*itr).MonString, _T("RUN"));
			break;
		}
		if (theApp.GetControlCommand(nStation, mRdnsRunLogItem, (*itr).MonString) == STATE_RUN)
		{
			theApp.UpdateControlCommand(nStation, (*itr).MonString, _T("STOP"));
			break;
		}
	}

	SetTimer(TID_RDNSSTATE, INTERVAL_RDNSSTATE, 0);
}

void CRdnsChk20::OnTimer(UINT_PTR nIDEvent)
{
	HANDLE hFile;
	FILETIME cfTime, afTime, wfTime;

	CString dir = theApp.GetShareFilePath(eFileType_Calib_Result_Folder, theApp.GetSelectStation());
	CString strFilePath = _T("");

	CTime time = 0;

	if (nIDEvent == TID_RDNSSTATE)
	{
		MonitoringRdnsRunState();
	}
	else if (nIDEvent == TID_RES_AGCS)
	{
		strFilePath = dir + RES_FILE_AGCS;
		hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			GetFileTime(hFile, &cfTime, &afTime, &wfTime);
			CloseHandle(hFile);
			time = CTime(wfTime);

			if (time != m_AgcSResFileTime)
			{
				// �ڍ׃r���[�\��
				if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
				{
					m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_AGC_S);
				}
				KillTimer(TID_RES_AGCS);
			}
		}
//#ifdef _DEBUG
//		// �ڍ׃r���[�\��
//		if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
//		{
//			m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_AGC_S);
//		}
//		KillTimer(TID_RES_AGCS);
//#endif
	}
	else if (nIDEvent == TID_RES_AGCX)
	{
		strFilePath = dir + RES_FILE_AGCX;
		hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			GetFileTime(hFile, &cfTime, &afTime, &wfTime);
			CloseHandle(hFile);
			time = CTime(wfTime);

			if (time != m_AgcXResFileTime)
			{
				// �ڍ׃r���[�\��
				if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
				{
					m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_AGC_X);
				}
				KillTimer(TID_RES_AGCX);
			}
		}
//#ifdef _DEBUG
//		// �ڍ׃r���[�\��
//		if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
//		{
//			m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_AGC_X);
//		}
//		KillTimer(TID_RES_AGCX);
//#endif
	}
	else if (nIDEvent == TID_RES_RNG)
	{
		strFilePath = dir + RES_FILE_RNG;
		hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			GetFileTime(hFile, &cfTime, &afTime, &wfTime);
			CloseHandle(hFile);
			time = CTime(wfTime);

			if (time != m_DelayResFileTime)
			{
				// �ڍ׃r���[�\��
				if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
				{
					m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_RNG);
				}
				KillTimer(TID_RES_RNG);
			}
		}
//#ifdef _DEBUG
//		// �ڍ׃r���[�\��
//		if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
//		{
//			m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_RNG);
//		}
//		KillTimer(TID_RES_RNG);
//#endif
	}
	else if (nIDEvent == TID_RES_TLM)
	{
		strFilePath = dir + RES_FILE_TLM;
		hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			GetFileTime(hFile, &cfTime, &afTime, &wfTime);
			CloseHandle(hFile);
			time = CTime(wfTime);

			if (time != m_TlmResFileTime)
			{
				// �ڍ׃r���[�\��
				if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
				{
					m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_TLM_S);
				}
				KillTimer(TID_RES_TLM);
			}
		}
//#ifdef _DEBUG
//		// �ڍ׃r���[�\��
//		if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
//		{
//			m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_TLM_S);
//		}
//		KillTimer(TID_RES_TLM);
//#endif
	}
	else if (nIDEvent == TID_RES_CMD)
	{
		strFilePath = dir + RES_FILE_CMD;
		hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			GetFileTime(hFile, &cfTime, &afTime, &wfTime);
			CloseHandle(hFile);
			time = CTime(wfTime);

			if (time != m_CmdResFileTime)
			{
				// �ڍ׃r���[�\��
				if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
				{
					m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_CMD);
				}
				KillTimer(TID_RES_CMD);
			}
		}
//#ifdef _DEBUG
//		// �ڍ׃r���[�\��
//		if (m_ResultDlg != NULL && m_ResultDlg->GetSafeHwnd() != NULL)
//		{
//			m_ResultDlg->PostMessage(eRdnsChk_LogOpen, 0, (LPARAM)TYPE_CMD);
//		}
//		KillTimer(TID_RES_CMD);
//#endif
	}

	CDialogBase::OnTimer(nIDEvent);
}

/*============================================================================*/
/*! CRdnsChk20

-# KillTimer ALL

@param
@retval

*/
/*============================================================================*/
void CRdnsChk20::InitTimer()
{
	KillTimer(TID_RES_AGCS);
	KillTimer(TID_RES_AGCX);
	KillTimer(TID_RES_RNG);
	KillTimer(TID_RES_TLM);
	KillTimer(TID_RES_CMD);
}

/*============================================================================*/
/*! CRdnsChk64

-# �X���b�h�𔲂���

@param
@retval

*/
/*============================================================================*/
void CRdnsChk20::ExitThread()
{
	m_RdnsChkAbort = true;

	if (m_hThread != NULL)
	{
		if (m_ThreadRun == true)
		{
			m_ThreadAbort = true;
		}

		CloseLogDlg();
		InitTimer();

		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}
