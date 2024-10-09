
// StationControl.h : StationControl �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��
#include "resource1.h"
#include "resource2.h"
#include "const.h"
#include "GraphException.h"
#include "DataCollection.h"
#include "LogTraceEx.h"
#include "GraphWindow.h"
#include "DialogBase.h"
#include "SatelliteData.h"
#include "CmdInf.h"

#include "ShareAlarmIf.h"
#include "AlarmListThread.h"

#include "ShareLbwIf.h"

#include "..\Utility\ColorControl.h"
#include "..\Utility\FileAccess.h"

// CStationControlApp:
// ���̃N���X�̎����ɂ��ẮAStationControl.cpp ���Q�Ƃ��Ă��������B
//

class CStationControlApp : public CWinAppEx
{
public:
	CStationControlApp();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CCommandLineInfoEx	m_cmdInfo;
	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	UINT		mIgnore;
	BYTE		mControlAccess;
	//!< �J�X�^�� MDI �q�t���[��
	CMultiDocTemplate* m_pDocTemplate[eViewType_Max];
	UINT		m_nWindowType;
	CString		m_strAppPath;						// �A�v���P�[�V�����p�X��
	CString		m_strAppInifileName;				// �A�v���P�[�V����INI�t�@�C����
	CString		m_strAppDataPath;					// �A�v���P�[�V����DATA�p�X��
	CString		m_strAppLogPath;					// �A�v���P�[�V����LOG�p�X��
	CString		m_strAppWorkPath;					// �A�v���P�[�V����WORK�p�X��
	CString		m_strAppLayoutPath;					// �A�v���P�[�V�������C�A�E�g�p�X��
	CString		m_strAppFaPanelPath;				// FAPANEL�p�X��
	CString		m_strManagementPath;				// �Ǘ��p�p�X��
	UINT		m_nLogLevel;
	UINT		m_nDrawUpdateInterval;

	CString		m_strFileServerName;				// �t�@�C���T�[�o�[����
	CString		m_strSshServerName;					// SSH�T�[�o�[����
	CString		m_strSshUserName;					// SSH���[�U����
	CString		m_strSshKeyPath;					// SSH�L�[�p�X
	CString		m_strBatchPath;						// SSH�o�b�`�t�@�C���p�X 
	CString		m_strEventTonePath;					// �C�x���g���t�@�C���i�[�p�X

	UINT		m_nExecuteMode;						// ���s���[�h
	UINT		m_nLimitedMode;						// ���������䃂�[�h
	UINT		m_nStationMax;						// �ő�ǐ�
	UINT		m_nSelectMode;						// �\���ǃ��[�h�i0:�P�� 1:5�� 2:10�ǁj
	UINT		m_nSelectStation;					// 1�ǃ��[�h���̋ǁi1:�P�c64m�� 2:�P�c54m�� 3:���V�Y34�� 4:���V�Y20�ǁj
	bool		m_bLaunchStation[eStation_MAX];		// �T�ǃ��[�h����P�ǃ��[�h���N�������Ƃ��̋ǋN���t���O
	UINT		m_nMessageThread;					// 0:�X���b�h�Ȃ��@1:�X���b�h����

	UINT		m_nPredDiffEnable;					// 1:PRED DIFF�L��, 0:PRED DIFF����
	double		m_dPredDiffMin;						// 54m�A���e�i�T�}����PRED DIFF�ŏ��L������(s)

	UINT		m_nOpenViewType;

	map<LPARAM, WPARAM>	mWindowList;

	CDataCollection	m_DataCollection;
	CSatelliteData	m_SatelliteData;

	bool		m_TrendScrollMode[MAX_VIEW];
	CString		m_strDefaultPrinterName;
	HWND		m_TimeLineWnd;
	bool		m_bTimeLineCall;

	CString		mTimeLineSelectSatellite, mTimeLineSelectPlanid;
	LPARAM		mTimeLineSelectGetForm;
	UINT		mTimeLineSelectType;
	CTime		mSartTime;

	CWnd*		m_pParentForecastListDlg;

	UINT		m_nHcopyType;

	CPoint		m_ptWndTmp;							// �E�B���h�E�e���|�����|�W�V����

	UINT		m_nOpenHistroySearch;

	int			mClientTerminal;					// �n��ǁA���͌��A�}�g�̋敪
	UINT		mServiceMode;						// �T�[�r�X���[�h

	// StationControl��OperationTimeline�Ƃ̊Ԃł̊Ď��f�[�^���Ƃ�̂��߂̃f�[�^���X�g
	deque<CMonitorDataTemp>			mMonitorDataList;

	//=============================================================================================
	// FA-PANEL����̐ݔ��A���[���󋵈ꗗ�擾�p�N���X
	CShareAlarmIf					mShareAlarmIf[CShareAlarmIf::mAlarmStationMax];
	// �ݔ��A���[���󋵈ꗗ�f�[�^�i�[�̈�
	vector<CShareAlarmIf::stAlarmItem> mAlarmList[CShareAlarmIf::mAlarmStationMax];
	// �ݔ��A���[���󋵈ꗗ�X���b�h
	CAlarmListThread*	mpAlarmThread;
	//=============================================================================================

	//=============================================================================================
	// CURRENT_LBW ���L�������N���X
	CShareLbwIf		mShareLbwIf;
	// CURRENT_LBW�f�[�^�i�[�̈�
	vector<CShareLbwIf::stItem> mLbwList;
	//=============================================================================================



	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	UINT			GetPipeIgnore()			{ return mIgnore; }
	BYTE			GetControlAccess()		{ return mControlAccess; }

	CString&		GetAppPath()			{ return m_strAppPath; }
	CString&		GetAppInifileName()		{ return m_strAppInifileName; }
	CString&		GetAppDataPath()		{ return m_strAppDataPath; }
	CString&		GetAppLogPath()			{ return m_strAppLogPath; }
	CString&		GetAppWorkPath()		{ return m_strAppWorkPath; }
	CString&		GetAppLayoutPath()		{ return m_strAppLayoutPath; }
	CString&		GetAppFaPanelPath()		{ return m_strAppFaPanelPath; }
	CString&		GetFileServerName()		{ return m_strFileServerName; }
	CString&		GetSshServerName()		{ return m_strSshServerName; }
	CString&		GetSshUserName()		{ return m_strSshUserName; }
	CString&		GetSshKeyPath()			{ return m_strSshKeyPath; }
	CString&		GetSshBatchPath()		{ return m_strBatchPath; }
	CString&		GetManagementPath()		{ return m_strManagementPath; }
	CString&		GetEventTonePath()		{ return m_strEventTonePath; }
	UINT&			IsPredDiffEnable()		{ return m_nPredDiffEnable; }
	double&			GetPredDiffMin()		{ return m_dPredDiffMin; }

	void			SetHcopyType(UINT val)	{ m_nHcopyType = val; }
	UINT			GetHcopyType()			{ return m_nHcopyType; }
	UINT			GetDrawUpdateInterval(bool bRead);
	UINT			GetMessageThread() { return m_nMessageThread; }

	deque<CMonitorDataTemp>&	GetMonitorDataList()	{ return mMonitorDataList; }
	void SetMonitorDataList(vector<CEngValCnv::obs_tbl_res_t>& data)
	{
		CGraphMutex::Lock(eTimeline);
		CMonitorDataTemp temp;
		temp.mData = data;
		mMonitorDataList.push_back(temp);
		CGraphMutex::Unlock(eTimeline);
	}
	void ClearMonitorDataList()
	{
		CGraphMutex::Lock(eTimeline);
		mMonitorDataList.clear();
		CGraphMutex::Unlock(eTimeline);
	}

	UINT			GetExecuteMode()					{ return m_nExecuteMode; }
	UINT			GetLimitedMode()					{ return m_nLimitedMode; }
	UINT			GetStationMax()						{ return m_nStationMax; }
	UINT			GetSelectStationMode()				{ return m_nSelectMode; }
	void			SetSelectStationMode(UINT val)		{ m_nSelectMode = val; }
	UINT			GetSelectStation()					{ return m_nSelectStation; }
	void			SetSelectStation(UINT val)			{ m_nSelectStation = val; }
	bool			GetLaunchStation(int pos)			{ return m_bLaunchStation[pos]; }
	void			SetLaunchStation(int pos, bool val)	{ m_bLaunchStation[pos] = val; }

	UINT			GetHistorySearchType()				{ return m_nOpenHistroySearch; }
	void			SetHistorySearchType(UINT val)		{ m_nOpenHistroySearch = val; }

	UINT			GetOpenViewType()		{ return m_nOpenViewType; }

	CString			GetDefaultPrintName()	{ return m_strDefaultPrinterName; }

	CDataCollection&	GetDataCollection()		{ return m_DataCollection; }
	CSatelliteData&		GetSatelliteData()		{ return m_SatelliteData; }
	bool		GetTrendScrollMode(int pos) { return m_TrendScrollMode[pos]; }
	void		SetTrendScrollMode(int pos, bool val) { m_TrendScrollMode[pos] = val; }

	CString		GetShareFilePath(UINT fileType, UINT nStation = eStation_MAX);
	CString		GetCheckDialogPath(UINT nStation);
	void		ExecCheckDialog();
	void		StopCheckDialog();

	// �e��_�C�A���O�A�E�B���h�E�֘A
	void	AddWindowHandle(LPARAM lParam, WPARAM wParam = 0);
	CWnd*	GetWindowHandle(WPARAM wParam = 0);
	void	RemoveWindowHandle(LPARAM lParam);

	void	OpenView(UINT val);
	void	CloseView(UINT val);
	void	SetWindowType(UINT val, bool bOpen)
	{
		if (bOpen)
			m_nWindowType |= (1 << val);
		else
			m_nWindowType &= ~(1 << val);
	}
	UINT	GetWindowType()	{ return m_nWindowType; }
	CView*	GetView(UINT val);
	UINT	GetStationWindowType(CWnd* pWnd);
	void	SetTimeLineHandle(HWND val)	{ m_TimeLineWnd = val; }
	HWND	GetTimeLineHandle()			{ return m_TimeLineWnd; }
	void	SetTimeLineCall(bool val)	{ m_bTimeLineCall = val; }
	bool	GetTimeLineCall()			{ return m_bTimeLineCall; }

	// �^�C�����C���ƌv�旧�Ă̂��߂̃f�[�^�ݒ�E�擾
	void	SetTimeLinePlan(CString val1, CString val2, LPARAM val3, UINT val4, CTime val5) { mTimeLineSelectSatellite = val1; mTimeLineSelectPlanid = val2; mTimeLineSelectGetForm = val3; mTimeLineSelectType = val4; mSartTime = val5; }
	void	GetTimeLinePlan(CString& val1, CString& val2, LPARAM& val3, UINT& val4, CTime& val5) { val1 = mTimeLineSelectSatellite; val2 = mTimeLineSelectPlanid; val3 = mTimeLineSelectGetForm; val4 = mTimeLineSelectType; val5 = mSartTime; }

	void	GetCurrentUtcTime(CTime& time);

	DWORD	GetHostIpInfo();
	UINT	GetTermNo();

	void	SetForecastListDlgParent(CWnd* parent) { m_pParentForecastListDlg = parent; }
	CWnd*	GetForecastListDlgParent() { return m_pParentForecastListDlg; }

	void	GetCalib(CString station, CString satellite, CString cust, CString dflt, BOOL bWaitComplete = TRUE);
	void	RequestCalib(CString station, CString satellite, CString file, CString ctrl, BOOL bWaitComplete = TRUE);

	void	SendControl(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete = TRUE);
	void	SendControl2(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete = TRUE);
	void	SendControlNoUI(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete = TRUE);
	void	SendControlAgcTblLoad(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete = TRUE);
	BOOL	SendControlChkDlg(CString val1, CString val2, CString val3, CString val4);
	BOOL	SendControlSameNumChkDlg(CString val1, CString val2, CString val3, CString val4, CString val5);
	BOOL	SendControlChkDlg2(CString val1, CString val2, CString val3, CString val4);
	BOOL	SendControlNoUIChkDlg(CString val1, CString val2, CString val3, CString val4);

	BOOL	SendControlDlgCURRENT_LBW(CString val1, CString val2, CString val3, CString val4);
	BOOL	SendControlDlgPLL_BW(CString val1, CString val2, CString val3, CString val4);

	void	SetWindowTmpPoint(int x, int y)		{ m_ptWndTmp.x = x; m_ptWndTmp.y = y; };
	CPoint	GetWindowTmpPoint()					{ return m_ptWndTmp; }

	void	SetMonitorMode(CWnd* pMain);
	void	SetLimitedMode();

	//==================================================================
	// ����R�}���h�֘A
	//==================================================================
	deque<stSendCtrlMsg> mConntrolList;
	void SetControlCommand(stSendCtrlMsg cmd)
	{
		deque<stSendCtrlMsg>::iterator itr;
		for (itr = mConntrolList.begin(); itr != mConntrolList.end(); itr++){
			if ((*itr).station == cmd.station && (*itr).item == cmd.item && (*itr).MonString.CollateNoCase(cmd.MonString) == 0){
				(*itr) = cmd;
				return;
			}
		}
		mConntrolList.push_back(cmd);
	}

	int GetControlCommand(UINT station, int item, CString monstring)
	{
		deque<stSendCtrlMsg>::iterator itr;
		for (itr = mConntrolList.begin(); itr != mConntrolList.end(); itr++){
			stSendCtrlMsg &st = (*itr);
			if ((*itr).station == station && (*itr).item == item && (*itr).MonString.CollateNoCase(monstring) == 0){
				return (*itr).state;
			}
		}
		return STATE_NONE;
	}

	void ClearControlCommand(UINT station, int item, CString monstring)
	{
		deque<stSendCtrlMsg>::iterator itr;
		for (itr = mConntrolList.begin(); itr != mConntrolList.end(); itr++){
			if ((*itr).station == station && (*itr).item == item && (*itr).MonString.CollateNoCase(monstring) == 0){
				mConntrolList.erase(itr);
				break;
			}
		}
	}

	void UpdateControlCommand(UINT station, CString monstring, CString result)
	{
		deque<stSendCtrlMsg>::iterator itr;
		for (itr = mConntrolList.begin(); itr != mConntrolList.end(); itr++){
			if ( (*itr).station == station && (*itr).MonString.CollateNoCase(monstring) == 0){
				// �����Ď��������݂���
				if ((*itr).state == STATE_WAIT){
					if (result.CompareNoCase(_T("STOP")) != 0){
						// ���s���ɑJ�ڂ���
						(*itr).state = STATE_RUN;
						TRACE(_T("RUN \n"));
					}
				} else if ((*itr).state == STATE_RUN){
					if (result.CompareNoCase(_T("STOP")) == 0){
						// �I���ɑJ�ڂ���
						(*itr).state = STATE_STP;
						TRACE(_T("STOP \n"));
					}
				}
				break;
			}
		}
	}

	//=============================================================================================
	// FA-PANEL����̐ݔ��A���[���󋵈ꗗ�擾�p�N���X
	CShareAlarmIf& GetAlarmIf(int nStation) {
		if (nStation < 0 || nStation >= CShareAlarmIf::mAlarmStationMax)
			return mShareAlarmIf[0];

		return mShareAlarmIf[nStation];
	}
	// �ݔ��A���[���󋵈ꗗ�f�[�^�i�[�̈�
	vector<CShareAlarmIf::stAlarmItem>& GetAlarmList(int nStation) { 
		if (nStation < 0 || nStation >= CShareAlarmIf::mAlarmStationMax)
			return mAlarmList[0];

		return mAlarmList[nStation];
	}
	// �ݔ��A���[���󋵈ꗗ�X���b�h�̍쐬
	void	CreateAlarmThread();
	// �ݔ��A���[���󋵈ꗗ�X���b�h�̍폜
	void	DeleteAlarmThread();
	//=============================================================================================

	//=============================================================================================
	// CURRENT_LBW ���L�������N���X
	CShareLbwIf& GetShareLbwIf() { return mShareLbwIf; }
	// CURRENT_LBW�f�[�^�i�[�̈�
	vector<CShareLbwIf::stItem>& GetShareLbwList() { return mLbwList; }
	// CURRENT_LBW �T�[�o�[�p�X���b�h�̍쐬
	void CreateShareLbwReadIF();
	// CURRENT_LBW �N���C�A���g�p�X���b�h�̍쐬
	void CreateShareLbwWriteIF();
	//=============================================================================================

protected:
	BOOL	CreateDocTemplate(UINT nStation);
	bool	InitializeAppl(bool bPathOnly=false);
	void	OnFilePrintSetup();
	void	ExecLoopBandSetDlg(CString ctrl);


	//==================================================================
	// ���䌠���p
	//==================================================================
	BYTE	GetOperationAccess();
	bool	getUserSid(CString& strUserName, CString& strSid);
public:
	CString	ManEncodeDecode(CString str);

	//==================================================================
	// FA�ւ̐��䃂�[�h�ʒm�p
	//==================================================================
protected:
	void WriteExecuteMode2FAFile(UINT mode);

	//==================================================================
	// �n��ǁA���͌��A�}�g�̋敪�擾
	//==================================================================
public:
	int		GetClientTerminal()	{ return mClientTerminal; }
	UINT	GetServiceMode()	{ return mServiceMode; }

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	/* ------------------------------------------------------------------------------------ */
};

extern CStationControlApp theApp;
