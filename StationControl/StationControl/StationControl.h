
// StationControl.h : StationControl アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル
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
// このクラスの実装については、StationControl.cpp を参照してください。
//

class CStationControlApp : public CWinAppEx
{
public:
	CStationControlApp();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CCommandLineInfoEx	m_cmdInfo;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	UINT		mIgnore;
	BYTE		mControlAccess;
	//!< カスタム MDI 子フレーム
	CMultiDocTemplate* m_pDocTemplate[eViewType_Max];
	UINT		m_nWindowType;
	CString		m_strAppPath;						// アプリケーションパス名
	CString		m_strAppInifileName;				// アプリケーションINIファイル名
	CString		m_strAppDataPath;					// アプリケーションDATAパス名
	CString		m_strAppLogPath;					// アプリケーションLOGパス名
	CString		m_strAppWorkPath;					// アプリケーションWORKパス名
	CString		m_strAppLayoutPath;					// アプリケーションレイアウトパス名
	CString		m_strAppFaPanelPath;				// FAPANELパス名
	CString		m_strManagementPath;				// 管理用パス名
	UINT		m_nLogLevel;
	UINT		m_nDrawUpdateInterval;

	CString		m_strFileServerName;				// ファイルサーバー名称
	CString		m_strSshServerName;					// SSHサーバー名称
	CString		m_strSshUserName;					// SSHユーザ名称
	CString		m_strSshKeyPath;					// SSHキーパス
	CString		m_strBatchPath;						// SSHバッチファイルパス 
	CString		m_strEventTonePath;					// イベント音ファイル格納パス

	UINT		m_nExecuteMode;						// 実行モード
	UINT		m_nLimitedMode;						// 制限つき制御モード
	UINT		m_nStationMax;						// 最大局数
	UINT		m_nSelectMode;						// 表示局モード（0:１局 1:5局 2:10局）
	UINT		m_nSelectStation;					// 1局モード時の局（1:臼田64m局 2:臼田54m局 3:内之浦34局 4:内之浦20局）
	bool		m_bLaunchStation[eStation_MAX];		// ５局モードから１局モードを起動したときの局起動フラグ
	UINT		m_nMessageThread;					// 0:スレッドなし　1:スレッドあり

	UINT		m_nPredDiffEnable;					// 1:PRED DIFF有効, 0:PRED DIFF無効
	double		m_dPredDiffMin;						// 54mアンテナサマリのPRED DIFF最小有効時間(s)

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

	CPoint		m_ptWndTmp;							// ウィンドウテンポラリポジション

	UINT		m_nOpenHistroySearch;

	int			mClientTerminal;					// 地上局、相模原、筑波の区分
	UINT		mServiceMode;						// サービスモード

	// StationControlとOperationTimelineとの間での監視データやりとりのためのデータリスト
	deque<CMonitorDataTemp>			mMonitorDataList;

	//=============================================================================================
	// FA-PANELからの設備アラーム状況一覧取得用クラス
	CShareAlarmIf					mShareAlarmIf[CShareAlarmIf::mAlarmStationMax];
	// 設備アラーム状況一覧データ格納領域
	vector<CShareAlarmIf::stAlarmItem> mAlarmList[CShareAlarmIf::mAlarmStationMax];
	// 設備アラーム状況一覧スレッド
	CAlarmListThread*	mpAlarmThread;
	//=============================================================================================

	//=============================================================================================
	// CURRENT_LBW 共有メモリクラス
	CShareLbwIf		mShareLbwIf;
	// CURRENT_LBWデータ格納領域
	vector<CShareLbwIf::stItem> mLbwList;
	//=============================================================================================



	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
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

	// 各種ダイアログ、ウィンドウ関連
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

	// タイムラインと計画立案のためのデータ設定・取得
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
	// 制御コマンド関連
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
				// 同じ監視名が存在した
				if ((*itr).state == STATE_WAIT){
					if (result.CompareNoCase(_T("STOP")) != 0){
						// 実行中に遷移した
						(*itr).state = STATE_RUN;
						TRACE(_T("RUN \n"));
					}
				} else if ((*itr).state == STATE_RUN){
					if (result.CompareNoCase(_T("STOP")) == 0){
						// 終了に遷移した
						(*itr).state = STATE_STP;
						TRACE(_T("STOP \n"));
					}
				}
				break;
			}
		}
	}

	//=============================================================================================
	// FA-PANELからの設備アラーム状況一覧取得用クラス
	CShareAlarmIf& GetAlarmIf(int nStation) {
		if (nStation < 0 || nStation >= CShareAlarmIf::mAlarmStationMax)
			return mShareAlarmIf[0];

		return mShareAlarmIf[nStation];
	}
	// 設備アラーム状況一覧データ格納領域
	vector<CShareAlarmIf::stAlarmItem>& GetAlarmList(int nStation) { 
		if (nStation < 0 || nStation >= CShareAlarmIf::mAlarmStationMax)
			return mAlarmList[0];

		return mAlarmList[nStation];
	}
	// 設備アラーム状況一覧スレッドの作成
	void	CreateAlarmThread();
	// 設備アラーム状況一覧スレッドの削除
	void	DeleteAlarmThread();
	//=============================================================================================

	//=============================================================================================
	// CURRENT_LBW 共有メモリクラス
	CShareLbwIf& GetShareLbwIf() { return mShareLbwIf; }
	// CURRENT_LBWデータ格納領域
	vector<CShareLbwIf::stItem>& GetShareLbwList() { return mLbwList; }
	// CURRENT_LBW サーバー用スレッドの作成
	void CreateShareLbwReadIF();
	// CURRENT_LBW クライアント用スレッドの作成
	void CreateShareLbwWriteIF();
	//=============================================================================================

protected:
	BOOL	CreateDocTemplate(UINT nStation);
	bool	InitializeAppl(bool bPathOnly=false);
	void	OnFilePrintSetup();
	void	ExecLoopBandSetDlg(CString ctrl);


	//==================================================================
	// 制御権利用
	//==================================================================
	BYTE	GetOperationAccess();
	bool	getUserSid(CString& strUserName, CString& strSid);
public:
	CString	ManEncodeDecode(CString str);

	//==================================================================
	// FAへの制御モード通知用
	//==================================================================
protected:
	void WriteExecuteMode2FAFile(UINT mode);

	//==================================================================
	// 地上局、相模原、筑波の区分取得
	//==================================================================
public:
	int		GetClientTerminal()	{ return mClientTerminal; }
	UINT	GetServiceMode()	{ return mServiceMode; }

// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
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
