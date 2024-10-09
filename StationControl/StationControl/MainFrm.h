
// MainFrm.h : CMainFrame �N���X�̃C���^�[�t�F�C�X
//
#include "MFCMenuBarEx.h"
#include "MFCToolBarEx.h"
#include "const.h"
#include "MFCCaptionBarEx.h"

#include "RdnsChk64.h"
#include "RdnsChk54.h"
#include "RdnsChk34.h"
#include "RdnsChk20.h"
#include "SpectrumView.h"
#include "RangePropagationStatusMonitor.h"
#include "SessionStatus.h"
#include "ForecastList.h"
#include "OperationPlan.h"
#include "FrequencyCtrl.h"
#include "SatelliteCapture.h"
#include "InitSetting.h"
#include "TrsControlMng.h"
#include "SequentialCtrlRegister.h"
#include "OperationPlanTimeOffsetSet.h"
#include "OperationStartTimeChg.h"
#include "OperationEndTimeChg.h"
#include "BusinessDiary.h"
#include "EquipmentRecDumpOutput.h"
#include "EquipmentRecSelectFile.h"
#include "TblSatelliteSelection.h"
#include "TrendPropertiesDlg.h"

#ifdef MON_UT
#include "MonitorInfo.h"
#endif

#include "MessageThread.h"
#include "MessageStationThread.h"

#include <mmsystem.h>

// ���v���Z�X�ԒʐM
#include "../utility/StationPipe.h"

using namespace std;

#pragma once

typedef struct {
	CString		wavefile;
	CString		monname;
	UINT		code;
} stEventTone;
enum{
	eEvent_Init,
	eEvent_PassStart,
	eEvent_PassEnd,
	eEvent_Max,
};
const stEventTone mEventTone[] = {
	{ _T("initst.wav"), _T("CTRL.INIT_TIME"), 0x01 },	// �����ݒ�J�n��
	{ _T("passst.wav"), _T("CTRL.START_TIME"), 0x02 },	// �p�X�J�n��
	{ _T("passen.wav"), _T("CTRL.PASS_INFO"), 0x04 },	// �p�X�I����
};

class CMonitors
{
public:
	CMonitors();
	virtual ~CMonitors();

private:
	typedef struct{
		RECT			rect;
		HMONITOR		hMonitor;
		MONITORINFOEX	info;
	} mMonData;
	vector<mMonData> m_MonitorList;

	static BOOL CALLBACK MonitorNumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);

public:
	//CMonitor GetMonitor(const int index) const;
	int		GetCount() const { return (int)m_MonitorList.size(); }
	void	AddMonitor(HMONITOR hMonitor, CRect rect, MONITORINFOEX	*info)
	{
		mMonData data;
		data.rect.left = rect.left;
		data.rect.top = rect.top;
		data.rect.right = rect.right;
		data.rect.bottom = rect.bottom;
		data.hMonitor = hMonitor;
		memcpy(&data.info, info, sizeof(MONITORINFOEX));
		m_MonitorList.push_back(data);
	}
	bool	GetMonitor(const UINT nMonNo, RECT *prect, MONITORINFOEX *pinfo)
	{
		vector<mMonData>::iterator itr;
		for (itr = m_MonitorList.begin(); itr != m_MonitorList.end(); itr++){
			if (nMonNo == 0 && (*itr).info.dwFlags == MONITORINFOF_PRIMARY){
				memcpy(prect, &((*itr).rect), sizeof(RECT));
				memcpy(pinfo, &((*itr).info), sizeof(MONITORINFOEX));
				return true;
			}
			if (nMonNo == 1 && (*itr).info.dwFlags != MONITORINFOF_PRIMARY){
				memcpy(prect, &((*itr).rect), sizeof(RECT));
				memcpy(pinfo, &((*itr).info), sizeof(MONITORINFOEX));
				return true;
			}
		}
		return false;
	}
};

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

protected:
	static CWnd* m_pPipeMessageWnd;
	static CEngValCnv*	m_pEngval[eStation_MAX];
public:
	static CWnd*		GetPipeMessageWnd();
	static void			SetPipeMessageWnd(CWnd* p);
	static CEngValCnv*	GetEngval(int pos);
	static void			SetEngval(int pos, CEngValCnv* eval);
	static void			DeleteEngval(int pos);

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	enum{
		eStatusSound_Play = 0x01,
		eStatusSound_Stop = 0x02,
		eStatusSound_Mute = 0x04,
	};

	
	const CString m_strAppName = _T("StationControl LayoutFile");		// �[���A�v�����̐ݒ�
	const double m_nVersion = 1.1;										// ���C�A�E�g�Ή��A�v���o�[�W�����̐ݒ�

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CMonitors	mMonitor;
	CEvent*		m_pTimeLineThreadsEvent;
	CFont		mCaptionFont;
	CEvent*		m_pStationThreadsEvent[eStation_MAX];
	UINT		m_nPlayStatusSound;
	MCIDEVICEID mStatusSoundId;
	MCIDEVICEID	m_EventSoundId;		// �T�E���hID
	UINT		m_dwEventStatus;	// �C�x���g���
	DWORD		m_dwEventStart;		// �T�E���h�Đ��J�n����

	// ���v���Z�X�ԒʐM
	CStationPipe		mPipeSend[ePIPETYPE_MAX];
	CStationPipe		mPipeRecv[ePIPETYPE_MAX];
	CShareMemory		mMemoryTimeLine;
	bool				m_bCreatePipe[ePIPETYPE_MAX];
	CStationPipe		mPipeServiceSend[eStation_MAX];
	CStationPipe		mPipeServiceRecv[eStation_MAX];
	bool				m_bCreatePipeService[eStation_MAX];
	CWinThread*			m_pTimelineThread;
//	CMessageThread*		m_pMessageThread;
	CMessageStationThread*		m_pMessageStationThread[eStation_MAX];

//	UINT		m_nLastSizeType;		// ���O�̃A�v���̃T�C�Y�^�C�v
	bool		m_bRdnsChkPlanForceStop;	// �Z���f�f�v�拭���I��
	time_t		m_tRdnsPlanStopTimeout;		// �Z���f�f�v�拭���I���̃^�C���A�E�g����

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	OnTileHorz();
	void	OnTileHorz_1();
	void	OnTileHorz_5();
	void	SetStationMenuBar(UINT eStation);
	void	SendServicePipe(DWORD command, int nStation);
	void	SendPipeHistory();
	void	OnFaBrowser(UINT station, CString fname);
	CMFCCaptionBarEx& GetWndCaption() { return m_wndCaption; }
	UINT	GetLoadLayoutIndex(UINT type, UINT subtype = 0);
	BOOL	IsLayoutExcludeDlg(UINT type);
	UINT	GetLoadLayoutOtherViewIndex(UINT type);
	void	EventToneProc();
	void	PlayEventSound(UINT type);
	void	StopEventSound();

	void	TrsCtrlX();				// ���C�A�E�g�������p�@���M����X��
	void	TrsCtrlXX();			// ���C�A�E�g�������p�@���M����X/X��
	void	TrsCtrlXX54();			// ���C�A�E�g�������p�@���M����X/X��(54m)
	void	TrsCtrlS64();			// ���C�A�E�g�������p�@���M����S��64m
	void	TrsCtrlS54();			// ���C�A�E�g�������p�@���M����S��54m
	void	TrsCtrlS34();			// ���C�A�E�g�������p�@���M����S��34m
	void	TrsCtrlS20();			// ���C�A�E�g�������p�@���M����S��20m

protected:
	void	SaveLayout(CString filename);
	void	LoadLayout(CString filename);
	bool	DirectPrint(CWnd* pwnd);
	bool	GeWindowCaptureMeasure(CWnd* pWnd, CBitmap& bitmap);
	void	paintCaption();
	void	PlayStatusSound(UINT status);
	void	StopStatusSound();
	void	MuteStatusSound();
	void	Emergency(UINT type);
	void	SetAGCGraphMinSize();
	void	SetRNGRRGraphMinSize();
	void	SetAGCRNGELGraphMinSize();

	void	CreateMessageThread();
	void	RemoveMessageThread();

	//==================================================================
	// �Z���f�f�v�拭���I��
	//==================================================================
public:
	void	ExecRdnsChkPlanForceStop();
//	bool	IsRdnsChkPlanForceStop()				{ return m_bRdnsChkPlanForceStop; }
//	void	SetRdnsChkPlanForceStop(bool val)		{ m_bRdnsChkPlanForceStop = val; }


	// ���v���Z�X�ԒʐM
	static void WINAPI receiveServerCallBack(LPPIPEBUFFER pbuf);
	static void WINAPI receiveClientCallBack(LPPIPEBUFFER pbuf);
	static void WINAPI receiveServerServiceCallBack01(PIPEMONITORACK* pbuf);
	static void WINAPI receiveServerServiceCallBack02(PIPEMONITORACK* pbuf);
	static void WINAPI receiveServerServiceCallBack03(PIPEMONITORACK* pbuf);
	static void WINAPI receiveServerServiceCallBack04(PIPEMONITORACK* pbuf);
	static void WINAPI receiveServerServiceCallBack(PIPEMONITORACK* pbuf);
	static void WINAPI receiveClientServiceCallBack(PIPEMONITORACK* pbuf);
public:
	void	createPipe(UINT type);
	void	deletePipe(UINT type);
	void	messagePipe(UINT message, WPARAM wParam, LPARAM lParam);
	void	sendPipe(UINT type, DWORD command, char* pbuf=NULL, UINT size=0);

	void	CreateTimelineThread();
	void	DeleteTimelineThread();
	void	SendTimelineThread(CMonitorDataTemp& data);

	void	ExitThread();

	/* ------------------------------------------------------------------------------------ */

// ����
public:

// ����
public:

// �I�[�o�[���C�h
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// ����
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �R���g���[�� �o�[�p�����o�[
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CMFCCaptionBarEx m_wndCaption;

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnWindowTileHorz();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMsPlanning();
	afx_msg void OnMsOrbitdata();
	afx_msg void OnMsSessionstate4();
	afx_msg void OnMsTimeOffsetSetting();
	afx_msg void OnMsStartTimeChange();
	afx_msg void OnMsEndTimeChange();
	afx_msg void OnMsForcedOperationStop();
	afx_msg void OnMsCalibStpDel();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCommandStatusTime();
	afx_msg void OnUpdateStatusTime(CCmdUI *pCmdUI);
	afx_msg void OnViewTrend();
	afx_msg void OnViewOpeDetail();
	afx_msg void OnViewStationSit();
	afx_msg void OnViewEqOperation();
	afx_msg void OnViewEqOperation2();
	afx_msg void OnViewEqOperation3();
	afx_msg void OnViewEqOperation4();
	afx_msg void OnViewEqOperation5();
	afx_msg void OnUpdateViewTrend(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewOpeDetail(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewStationSit(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewEqOperation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewEqOperation2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewEqOperation3(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewEqOperation4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewEqOperation5(CCmdUI *pCmdUI);
	afx_msg void OnDirectPrint();
	afx_msg void OnViewOpeTimeLine();
	afx_msg void OnUpdateViewOpeTimeLine(CCmdUI *pCmdUI);
//	afx_msg void OnMgTrendProparties();
	afx_msg void OnMgAgcGraph();
	afx_msg void OnMgRngRRGraph();
	afx_msg void OnMgAGCRNGELGraph();
	afx_msg void OnMgTrendGraph();
	afx_msg void OnMgSpectrumGraph();
	afx_msg void OnMgRngMonitor();
	afx_msg void OnMoManualinitialSetting();
	afx_msg void OnMoTransmissionCtrl();
	afx_msg void OnMoSequentialCtrl();
	afx_msg void OnMpPlanningList();
	afx_msg void OnMpForecastList();
	afx_msg void OnMpForecastListSelBtn();
	afx_msg void OnMdDiary();
	afx_msg void OnMcCalibDiagns();
	afx_msg void OnMeMntrDataDmpOutput();
	afx_msg void OnMeDumpFileSel();
	afx_msg void OnMeAntEqipOpeHis();
	afx_msg void OnMeAntEqipDrvHis();
	afx_msg void OnMeOpeHisRealtime();
	afx_msg void OnMeOpeHisSearch();
	afx_msg void OnMeCtrlHisRealtime();
	afx_msg void OnMeCtrlHisSearch();
	afx_msg void OnMeEqipHisRealtime();
	afx_msg void OnMeEqipHisSearch();
	afx_msg void OnMeEventHisRealtime();
	afx_msg void OnMeEventHisSearch();
	afx_msg void OnMiSessionSet();
	afx_msg void OnMrFreqCtrl();
	afx_msg void OnMrSatlCap();
	afx_msg void OnMtAgcTblRead();
	afx_msg void OnMtAgcTblSelectLNA();
	afx_msg void OnMtAgcTblFileCpy();
	afx_msg void OnMtAgcTblFileRst();
	afx_msg void OnMtStaTblFileCpy();
	afx_msg void OnMtStaTblFileRst();
	afx_msg void OnMtLayoutSave();
	afx_msg void OnMtLayoutRestor();
	afx_msg void OnDestroy();
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcPaint();
//	afx_msg void OnUpdateMgTrendProparties(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMgAgcGraph(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMgRngRRGraph(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMgAGCRNGELGraph(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMgTrendGraph(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMgSpectrumGraph(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMgRngMonitor(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMoManualinitialSetting(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMoTransmissionCtrl(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMoSequentialCtrl(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMpPlanningList(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMpForecastList(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMdDiary(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMcCalibDiagns(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeMntrDataDmpOutput(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeDumpFileSel(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeAntEqipOpeHis(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeAntEqipDrvHis(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeOpeHisRealtime(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeOpeHisSearch(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeCtrlHisRealtime(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeCtrlHisSearch(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeEqipHisRealtime(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeEqipHisSearch(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeEventHisRealtime(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMeEventHisSearch(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMiSessionSet(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMiSessionSet4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMrFreqCtrl(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMrSatlCap(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMtAgcTblRead(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMtAgcTblSelectLNA(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMtAgcTblFileCpy(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMtAgcTblFileRst(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMtStaTblFileCpy(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMtStaTblFileRst(CCmdUI *pCmdUI);
	afx_msg void OnMeEqipStatusOut();
	afx_msg void OnUpdateMeEqipStatusOut(CCmdUI *pCmdUI);
	afx_msg void OnStation10();
	afx_msg void OnUpdateStation10(CCmdUI *pCmdUI);
	afx_msg void OnStationUsuda64();
	afx_msg void OnUpdateStationUsuda64(CCmdUI *pCmdUI);
	afx_msg void OnStationUsuda2();
	afx_msg void OnUpdateStationUsuda2(CCmdUI *pCmdUI);
	afx_msg void OnStationUchinoura34();
	afx_msg void OnUpdateStationUchinoura34(CCmdUI *pCmdUI);
	afx_msg void OnStationUchinoura20();
	afx_msg void OnUpdateStationUchinoura20(CCmdUI *pCmdUI);
	afx_msg void OnMpTimeline();
	afx_msg void OnUpdateMpTimeline(CCmdUI *pCmdUI);
	afx_msg void OnHcopyNormal();
	afx_msg void OnUpdateHcopyNormal(CCmdUI *pCmdUI);
	afx_msg void OnHcopyReverse();
	afx_msg void OnUpdateHcopyReverse(CCmdUI *pCmdUI);
	afx_msg void OnMpForecastDetail();
	afx_msg void OnMoEquipstatus();
	afx_msg void OnUpdateMoEquipstatus(CCmdUI *pCmdUI);
	afx_msg void OnMhManual();
	afx_msg void OnMeEqipAlarm();
	afx_msg void OnUpdateMeEqipAlarm(CCmdUI *pCmdUI);
#ifdef MON_UT
	afx_msg void OnMonUt();
	afx_msg void OnUpdateMonUt(CCmdUI *pCmdUI);
#endif
};

typedef void(CMainFrame::*mFuncTable)();
typedef struct {
	mFuncTable func;
	UINT		code;
	UINT		subcode;
} stLayoutDlg;

// ���C�A�E�g�ۑ��_�C�A���O
const stLayoutDlg mLayoutCreateDlg[] = {
	{ &CMainFrame::OnMsSessionstate4, IDD_DLG_SESSIONSTATUS, 0 },				// �Z�b�V������ԂS�ǃ��[�h
	{ &CMainFrame::OnMgSpectrumGraph, IDD_DLG_SPECTRUMVIEW, 0 },				// �X�y�N�g����
	{ &CMainFrame::OnMgRngMonitor, IDD_DLG_RANGEPROPAGATIONSTATUSMONITOR, 0 },	// �����W�`��
	{ &CMainFrame::OnMoSequentialCtrl, IDD_DLG_SEQUENTIALCTRLREGISTER, 0 },		// ��������		
	{ &CMainFrame::OnMpPlanningList, IDD_DLG_FORECASTLIST, 1 },					// �v��ꗗ
	{ &CMainFrame::OnMpForecastList, IDD_DLG_FORECASTLIST, 0 },					// �\��l�ꗗ
	{ &CMainFrame::OnMcCalibDiagns, IDD_DLG_RDNSCHK64, 0},						// �Z���f�f64m
	{ &CMainFrame::OnMcCalibDiagns, IDD_DLG_RDNSCHK54, 0 },						// �Z���f�f54m
	{ &CMainFrame::OnMcCalibDiagns, IDD_DLG_RDNSCHK34, 0 },						// �Z���f�f34m
	{ &CMainFrame::OnMcCalibDiagns, IDD_DLG_RDNSCHK20, 0 },						// �Z���f�f20m
	{ &CMainFrame::OnMiSessionSet, IDD_DLG_SESSIONSTATUS, 1 },					// �Z�b�V������ԂP�ǃ��[�h
	{ &CMainFrame::OnMrFreqCtrl, IDD_DLG_FREQ_CTRL, 0 },						// ���g������
	{ &CMainFrame::OnMrFreqCtrl, IDD_DLG_FREQ_CTRL54, 0 },						// ���g������(54m)
	{ &CMainFrame::OnMrSatlCap, IDD_DLG_SATELLITECAPTURE, 0 },					// �q���ߑ�����
	{ &CMainFrame::OnMrSatlCap, IDD_DLG_SATELLITECAPTURE54, 0 },				// �q���ߑ�����(54m)
	{ &CMainFrame::OnMeAntEqipOpeHis, IDD_DIALOG_ANTHISTORY, 0 },				// �A���e�i�ݔ��^�p����
	{ &CMainFrame::OnMeAntEqipDrvHis, IDD_DIALOG_ANTHISTORY+200, 0 },			// �A���e�i�ݔ��쓮����
};

// ���C�A�E�g���O�_�C�A���O
const stLayoutDlg mLayoutExcludeDlg[] = {
	{ &CMainFrame::OnMsPlanning, IDD_DLG_OPERATIONPLAN, 0 },					// �v�旧��
	{ &CMainFrame::OnMeMntrDataDmpOutput, IDD_DLG_EQIPREC_DUMPOUTPUT, 0 },		// �Ď��f�[�^�_���v�o��
	{ &CMainFrame::OnMeDumpFileSel, IDD_DLG_EQIPREC_SELFILE, 0 },				// �_���v��`�t�@�C���I��
	{ &CMainFrame::TrsCtrlX, IDD_DLG_TRSCTRL_X, 0 },							// ���M����X��
	{ &CMainFrame::TrsCtrlXX, IDD_DLG_TRSCTRL_XX, 0 },							// ���M����X/X��
	{ &CMainFrame::TrsCtrlXX54, IDD_DLG_TRSCTRL_XX54, 0 },						// ���M����X/X��54m
	{ &CMainFrame::TrsCtrlS64, IDD_DLG_TRSCTRL_S64, 0 },						// ���M����S��64m
	{ &CMainFrame::TrsCtrlS54, IDD_DLG_TRSCTRL_S64, 0 },						// ���M����S��54m
	{ &CMainFrame::TrsCtrlS34, IDD_DLG_TRSCTRL_S34, 0 },						// ���M����S��34m
	{ &CMainFrame::TrsCtrlS20, IDD_DLG_TRSCTRL_S20, 0 },						// ���M����S��20m
	{ &CMainFrame::OnMoTransmissionCtrl, IDD_DLG_TRSCONTROL, 0 },				// ���M����I���_�C�A���O
	{ &CMainFrame::OnMdDiary, IDD_DLG_DIARY, 0 },								// �����Ɩ�����
	{ NULL,                   IDD_DIALOG_DETAILVIEW, 0 },						// �ڍו\�����
};

// ���C�A�E�g�ۑ� ���̑��r���[�E�B���h�E
const stLayoutDlg mLayoutCreateOtherView[] = {
	{ &CMainFrame::OnMeOpeHisSearch, eViewType1_OperationSearch, 0 },			// �^�p���������i1�ǁj
	{ &CMainFrame::OnMeCtrlHisSearch, eViewType1_ControlSearch, 0 },			// ���䗚�������i1�ǁj
	{ &CMainFrame::OnMeEqipHisSearch, eViewType1_EquipmentSearch, 0 },			// �ݔ���ԗ��������i1�ǁj
	{ &CMainFrame::OnMeEventHisSearch, eViewType1_EventSearch, 0 },				// �C�x���g���������i1�ǁj
	{ &CMainFrame::OnMeOpeHisSearch, eViewType5_OperationSearch, 0 },			// �^�p���������i5�ǁj
	{ &CMainFrame::OnMeCtrlHisSearch, eViewType5_ControlSearch, 0 },			// ���䗚�������i5�ǁj
	{ &CMainFrame::OnMeEqipHisSearch, eViewType5_EquipmentSearch, 0 },			// �ݔ���ԗ��������i5�ǁj
	{ &CMainFrame::OnMeEventHisSearch, eViewType5_EventSearch, 0 },				// �C�x���g���������i5�ǁj
};
