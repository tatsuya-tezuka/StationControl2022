#pragma once

#include "TimeLine.h"

#if 0
static const stMonitorParam mOperationParam[] = {
	{ IDC_STATIC_SATELLITEVAL, _T("CTRL.SAT_NAME"), eControlStatus },
	{ IDC_STATIC_PLANVAL, _T("CTRL.PASS_NO"), eControlStatus },
	{ IDC_STATIC_FORECASTVAL, _T("CTRL.PRED_FILE"), eControlStatus },
	{ IDC_STATIC_OPESTATUSVAL, _T("CTRL.PASS_FLAG"), eControlDouble },
	{ IDC_STATIC_OPESTARTVAL, _T("CTRL.START_TIME"), eControlTime },
	{ IDC_STATIC_OPESTOPVAL, _T("CTRL.END_TIME"), eControlTime },
	{ IDC_STATIC_SENDSTATUSVAL, _T("X-UL_CNT.RF_ON_CONT"), eControlStatus },
	{ IDC_STATIC_SENDSTATUS_RF, _T("X-UL_CNT.RF_ON_CONT"), eControlStatus },
	{ IDC_STATIC_SENDSTATUS_HOLDTONE, _T("X-CMDOUT2.HOLDTONE_STAT"), eControlStatus },
	{ IDC_STATIC_SENDSTATUS_RNGCONT, _T("DSNTCR.RNG_CONT"), eControlStatus },
	{ IDC_STATIC_RECVNEW_CARR, _T("DSNTCR.CARRIER_LOCK_ANS"), eControlStatus },
	{ IDC_STATIC_RECVNEW_SUBCARR, _T("DSNTCR.SUB_CARRIER_LOCK_STS"), eControlStatus },
	{ IDC_STATIC_RECVNEW_VITERBI, _T("DSNTCR.VITERBI_LOCK_STS"), eControlStatus },
	{ IDC_STATIC_RECVNEW_BIT, _T("DSNTCR.BIT_LOCK"), eControlStatus },
	{ IDC_STATIC_RECVNEW_FRAME, _T("DSNTCR.FRAME_LOCK_STS"), eControlStatus },
	{ IDC_STATIC_RECVOLD_CARR, _T("X-RX2.PM_LOCK_ACH"), eControlStatus },
	{ IDC_STATIC_RECVOLD_SUBCARR, _T("X-RX2.DEM_LOCK_SUB_CARR_S"), eControlStatus },
	{ IDC_STATIC_RECVOLD_VITERBI, _T("X-RX2.DEM_LOCK_VITERBI_S"), eControlStatus },
	{ IDC_STATIC_RECVOLD_BIT, _T("X-RX2.DEM_LOCK_SYMBOL_S"), eControlStatus },
	{ IDC_STATIC_RECVOLD_FRAME, _T("X-PKTDEM.FRM_LOCK_S"), eControlStatus },
};
#else
static const stMonitorParam mOperationParam[eStation_MAX][20] = {
	// 臼田64m
	{
		{ IDC_STATIC_SATELLITEVAL,        _T("CTRL.SAT_NAME"),               eControlStatus },
		{ IDC_STATIC_PLANVAL,             _T("CTRL.PASS_NO"),                eControlStatus },
		{ IDC_STATIC_FORECASTVAL,         _T("CTRL.PRED_FILE"),              eControlStatus },
		{ IDC_STATIC_OPESTATUSVAL,        _T("CTRL.PASS_FLAG"),              eControlStatus },
		{ IDC_STATIC_OPESTARTVAL,         _T("CTRL.START_TIME"),             eControlTime   },
		{ IDC_STATIC_OPESTOPVAL,          _T("CTRL.END_TIME"),               eControlTime   },
		{ IDC_STATIC_SENDSTATUSVAL,       _T("X-UL_CNT.RF_ON_CONT"),         eControlStatus },
		{ IDC_STATIC_SENDSTATUS_RF,       _T("X-UL_CNT.RF_ON_CONT"),         eControlStatus },
		{ IDC_STATIC_SENDSTATUS_HOLDTONE, _T("X-CMDOUT2.HOLDTONE_STAT"),     eControlStatus },
		{ IDC_STATIC_SENDSTATUS_RNGCONT,  _T("DSNTCR.RNG_CONT"),             eControlStatus },
		{ IDC_STATIC_RECVNEW_CARR,        _T("DSNTCR.CARRIER_LOCK_ANS"),     eControlStatus },
		{ IDC_STATIC_RECVNEW_SUBCARR,     _T("DSNTCR.SUB_CARRIER_LOCK_STS"), eControlStatus },
		{ IDC_STATIC_RECVNEW_VITERBI,     _T("DSNTCR.VITERBI_LOCK_STS"),     eControlStatus },
		{ IDC_STATIC_RECVNEW_BIT,         _T("DSNTCR.BIT_LOCK"),             eControlStatus },
		{ IDC_STATIC_RECVNEW_FRAME,       _T("DSNTCR.FRAME_LOCK_STS"),       eControlStatus },
		{ IDC_STATIC_RECVOLD_CARR,        _T("X-RX2.PM_LOCK_ACH"),           eControlStatus },
		{ IDC_STATIC_RECVOLD_SUBCARR,     _T("X-RX2.DEM_LOCK_SUB_CARR_S"),   eControlStatus },
		{ IDC_STATIC_RECVOLD_VITERBI,     _T("X-RX2.DEM_LOCK_VITERBI_S"),    eControlStatus },
		{ IDC_STATIC_RECVOLD_BIT,         _T("X-RX2.DEM_LOCK_SYMBOL_S"),     eControlStatus },
		{ IDC_STATIC_RECVOLD_FRAME,       _T("X-PKTDEM.FRM_LOCK_S"),         eControlStatus },
	},
	// 臼田54m
	{
		{ IDC_STATIC_SATELLITEVAL,        _T("CTRL.SAT_NAME"),               eControlStatus },
		{ IDC_STATIC_PLANVAL,             _T("CTRL.PASS_NO"),                eControlStatus },
		{ IDC_STATIC_FORECASTVAL,         _T("CTRL.PRED_FILE"),              eControlStatus },
		{ IDC_STATIC_OPESTATUSVAL,        _T("CTRL.PASS_FLAG"),              eControlStatus },
		{ IDC_STATIC_OPESTARTVAL,         _T("CTRL.START_TIME"),             eControlTime   },
		{ IDC_STATIC_OPESTOPVAL,          _T("CTRL.END_TIME"),               eControlTime   },
		{ IDC_STATIC_SENDSTATUSVAL,       _T("X-SSPA.TX_ON/OFF"),            eControlStatus },
		{ IDC_STATIC_SENDSTATUS_RF,       _T("X-SSPA.TX_ON/OFF"),            eControlStatus },
		{ IDC_STATIC_SENDSTATUS_HOLDTONE, _T("X-CMDOUT.HOLDTONE_STAT"),     eControlStatus },
		{ IDC_STATIC_SENDSTATUS_RNGCONT,  _T("DSNTCR.RNG_CONT"),             eControlStatus },
		{ IDC_STATIC_RECVNEW_CARR,        _T("DSNTCR.CARRIER_LOCK"),         eControlStatus },
		{ IDC_STATIC_RECVNEW_SUBCARR,     _T("DSNTCR.SUB_CARRIER_LOCK"),     eControlStatus },
		{ IDC_STATIC_RECVNEW_VITERBI,     _T("DSNTCR.VITERBI_LOCK"),         eControlStatus },
		{ IDC_STATIC_RECVNEW_BIT,         _T("DSNTCR.BIT_LOCK"),             eControlStatus },
		{ IDC_STATIC_RECVNEW_FRAME,       _T("DSNTCR.FRAME_LOCK"),           eControlStatus },
		{ IDC_STATIC_RECVOLD_CARR,        _T(""),                            eControlStatus },
		{ IDC_STATIC_RECVOLD_SUBCARR,     _T(""),                            eControlStatus },
		{ IDC_STATIC_RECVOLD_VITERBI,     _T(""),                            eControlStatus },
		{ IDC_STATIC_RECVOLD_BIT,         _T(""),                            eControlStatus },
		{ IDC_STATIC_RECVOLD_FRAME,       _T(""),                            eControlStatus },
	},
	// 内之浦34m
	{
		{ IDC_STATIC_SATELLITEVAL, _T("CTRL.SAT_NAME"), eControlStatus },
		{ IDC_STATIC_PLANVAL, _T("CTRL.PASS_NO"), eControlStatus },
		{ IDC_STATIC_FORECASTVAL, _T("CTRL.PRED_FILE"), eControlStatus },
		{ IDC_STATIC_OPESTATUSVAL, _T("CTRL.PASS_FLAG"), eControlStatus },
		{ IDC_STATIC_OPESTARTVAL, _T("CTRL.START_TIME"), eControlTime },
		{ IDC_STATIC_OPESTOPVAL, _T("CTRL.END_TIME"), eControlTime },
		{ IDC_STATIC_SENDSTATUSVAL, _T("X-UL_CNT.RF_ON_CONT"), eControlStatus },
		{ IDC_STATIC_SENDSTATUS_RF, _T("X-UL_CNT.RF_ON_CONT"), eControlStatus },
		{ IDC_STATIC_SENDSTATUS_HOLDTONE, _T("X-CMDOUT2.HOLDTONE_STAT"), eControlStatus },
		{ IDC_STATIC_SENDSTATUS_RNGCONT, _T("DSNTCR.RNG_CONT"), eControlStatus },
		{ IDC_STATIC_RECVNEW_CARR, _T("SS-TCRDEM.CARR_LOCK_COMB"), eControlStatus },
		{ IDC_STATIC_RECVNEW_SUBCARR, _T("SS-TCRDEM.TLM_LOCK_SUB_CARR"), eControlStatus },
		{ IDC_STATIC_RECVNEW_VITERBI, _T("SS-TCRDEM.TLM_LOCK_VITERBI"), eControlStatus },
		{ IDC_STATIC_RECVNEW_BIT, _T("SS-TCRDEM.TLM_LOCK_BIT"), eControlStatus },
		{ IDC_STATIC_RECVNEW_FRAME, _T("SS-TCRDEM.FRAME_LOCK_DSP"), eControlStatus },
		{ IDC_STATIC_RECVOLD_CARR, _T("DSNTCR.CARRIER_LOCK_ANS"), eControlStatus },
		{ IDC_STATIC_RECVOLD_SUBCARR, _T("DSNTCR.SUB_CARRIER_LOCK_STS"), eControlStatus },
		{ IDC_STATIC_RECVOLD_VITERBI, _T("DSNTCR.VITERBI_LOCK_STS"), eControlStatus },
		{ IDC_STATIC_RECVOLD_BIT, _T("DSNTCR.BIT_LOCK_STS"), eControlStatus },
		{ IDC_STATIC_RECVOLD_FRAME, _T("DSNTCR.FRAME_LOCK_STS"), eControlStatus },
	},
	// 内之浦20m
	{
		{ IDC_STATIC_SATELLITEVAL, _T("CTRL.SAT_NAME"), eControlStatus },
		{ IDC_STATIC_PLANVAL, _T("CTRL.PASS_NO"), eControlStatus },
		{ IDC_STATIC_FORECASTVAL, _T("CTRL.PRED_FILE"), eControlStatus },
		{ IDC_STATIC_OPESTATUSVAL, _T("CTRL.PASS_FLAG"), eControlStatus },
		{ IDC_STATIC_OPESTARTVAL, _T("CTRL.START_TIME"), eControlTime },
		{ IDC_STATIC_OPESTOPVAL, _T("CTRL.END_TIME"), eControlTime },
		{ IDC_STATIC_SENDSTATUSVAL, _T("S-TXRF.IF_RF_OUTPUT_CONT"), eControlStatus },
		{ IDC_STATIC_SENDSTATUS_RF, _T("S-TXRF.IF_RF_OUTPUT_CONT"), eControlStatus },
		{ IDC_STATIC_SENDSTATUS_HOLDTONE, _T("CMDOUT1.HOLDTONE_STAT"), eControlStatus },
		{ IDC_STATIC_SENDSTATUS_RNGCONT, _T("SS-TCRDEM.RNG_CONT"), eControlStatus },
		{ IDC_STATIC_RECVNEW_CARR, _T("SS-TCRDEM.CARR_LOCK_COMB"), eControlStatus },
		{ IDC_STATIC_RECVNEW_SUBCARR, _T("SS-TCRDEM.TLM_LOCK_SUB_CARR"), eControlStatus },
		{ IDC_STATIC_RECVNEW_VITERBI, _T("SS-TCRDEM.TLM_LOCK_VITERBI"), eControlStatus },
		{ IDC_STATIC_RECVNEW_BIT, _T("SS-TCRDEM.TLM_LOCK_BIT"), eControlStatus },
		{ IDC_STATIC_RECVNEW_FRAME, _T("SS-TCRDEM.FRAME_LOCK_DSP"), eControlStatus },
		{ IDC_STATIC_RECVOLD_CARR, _T("SX-TCRDEM.CARR_LOCK_COMB"), eControlStatus },
		{ IDC_STATIC_RECVOLD_SUBCARR, _T("SX-TCRDEM.TLM_LOCK_SUB_CARR"), eControlStatus },
		{ IDC_STATIC_RECVOLD_VITERBI, _T("SX-TCRDEM.TLM_LOCK_VITERBI"), eControlStatus },
		{ IDC_STATIC_RECVOLD_BIT, _T("SX-TCRDEM.TLM_LOCK_BIT"), eControlStatus },
		{ IDC_STATIC_RECVOLD_FRAME, _T("X-TLMDEC.FRM_LOCK_S"), eControlStatus },
	}
};
#endif

// CTabOperationView ダイアログ

class CTabOperationView : public CDialogEx
{
	DECLARE_DYNCREATE(CTabOperationView)

public:
	CTabOperationView(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabOperationView();

// ダイアログ データ
	enum { IDD = IDD_TAB_OPERATION };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	UINT					mStation;
//	CTime					mCurrentTime;
//	vector<stTimeData>		mTimeDataList;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
//	void	UpdateOperationPlanInfo();
	CString	ConvertPredfileToId(CString name);
	void	UpdateStatus(UINT nStationID);
	void	UpdateStatus34m(UINT nStationID);
	void	UpdateStatus54m(UINT nStationID);
	int		GetEqSatDB(CString satname);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CColorStatic m_stSatelliteVal;
	CColorStatic m_stPlanVal;
	CColorStatic m_stForecastVal;
	CColorStatic m_stOpeStatusVal;
	CColorStatic m_stOpeStartVal;
	CColorStatic m_stOpeStopVal;
	CColorStatic m_stSendStatusVal;
	CColorStatic m_stSendStatuRf;
	CColorStatic m_stSendStatusHoldTone;
	CColorStatic m_stSendStatusRngCont;
	CColorStatic m_stRecvNewCarr;
	CColorStatic m_stRecvNewSubCarr;
	CColorStatic m_stRecvNewViteRbi;
	CColorStatic m_stRecvNewBit;
	CColorStatic m_stRecvNewFrame;
	CColorStatic m_stRecvOldCarr;
	CColorStatic m_stRecvOldSubCarr;
	CColorStatic m_stRecvOldViteRbi;
	CColorStatic m_stRecvOldBit;
	CColorStatic m_stRecvOldFrame;
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString mSatelliteVal;
	CString mPlanVal;
	CString mForecastVal;
	CString mOpeStatusVal;
	CString mOpeStartVal;
	CString mOpeStopVal;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};
