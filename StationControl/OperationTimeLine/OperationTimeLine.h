
// OperationTimeLine.h : OperationTimeLine アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル

#include "CmdInf.h"

#include "GraphException.h"

#include "../StationControl/SatelliteData.h"
#include "../StationControl/DataCollection.h"
#include "../utility/StationPipe.h"
#include "LogTraceEx.h"


enum eTimeViewType{
	eTimeViewType_OperatioTimeline,
	eTimeViewType_EquipmentSystem,
	eTimeViewType_Event,
	eTimeViewType_LimitError,
	eTimeViewType_Max,
};

static const TCHAR* mTimeViewTitle[]{
	_T("パス状況"),
	_T("設備系統"),
	_T("イベント履歴表示"),
	_T("リミットエラー履歴表示"),
	_T(""),
};

enum eTimeLineStation{
	eTimeLineStation_Usuda64,
	eTimeLineStation_Usuda642,
	eTimeLineStation_Uchinoura34,
	eTimeLineStation_Uchinoura20,
	TimeLineeStation_MAX,
};
static const COLORREF mTimeLineStationColor[]{
	RGB(195, 195, 195),
	RGB(255, 193, 164),
	RGB(255, 195, 255),
	RGB(153, 217, 255),
};

// COperationTimeLineApp:
// このクラスの実装については、OperationTimeLine.cpp を参照してください。
//


class COperationTimeLineApp : public CWinAppEx
{
public:
	COperationTimeLineApp();

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CMultiDocTemplate* m_pDocTemplate[eTimeViewType_Max];
	CString		m_strAppPath;						// アプリケーションパス名
	CString		m_strAppInifileName;				// アプリケーションINIファイル名
	CString		m_strAppDataPath;					// アプリケーションDATAパス名
	CString		m_strAppLogPath;					// アプリケーションLOGパス名
	CString		m_strAppFaPanelPath;				// FAPANELパス名
	CString		m_strServerName;					// サーバー名称
	CString		m_strSpaceName;						// 対象衛星名
	UINT		m_nLogLevel;
	CString		m_strSshServerName;					// SSHサーバー名称
	CString		m_strSshUserName;					// SSHユーザ名称
	CString		m_strSshKeyPath;					// SSHキーパス
	CString		m_strBatchPath;						// SSHバッチファイルパス 
	UINT		m_nMessageThread;					// 0:スレッドなし　1:スレッドあり

	UINT		m_nDrawUpdateInterval;

	UINT		m_nSelectStation;					// 1局モード時の局（1:臼田64m局 2:臼田54m局 3:内之浦34局 4:内之浦20局）
	UINT		m_nExecuteMode;						// 0:制御モード 1:監視モード
	UINT		m_nLimitedMode;						// 制限つき制御モード
	HWND		m_hCallWnd;							// 呼び出しウィンドウハンドル

	UINT		mServiceMode;						// サービスモード
	DWORD		m_nHistID54;						// 54m 履歴識別番号

	// 履歴データ
	deque<stHistoryData>	mEquipmentHistoryTemp;		// 設備状況履歴データサーバ受信用のワーク
	deque<stHistoryData>	mEquipmentHistoryData;	// 設備状況履歴データ（サブ画面）
	deque<stHistoryData>	mEventHistoryTemp;			// イベント履歴データサーバ受信用のワーク
	deque<stHistoryData>	mEventHistoryData;		// イベント履歴データ（サブ画面）

	UINT		m_nHcopyType;

	deque<PIPEMONITORACK>	mMessageTemp[eStation_MAX];

	map<CString, CWnd*>	m_pDlgMap;					// ダイアログマップ

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString&		GetAppPath()			{ return m_strAppPath; }
	CString&		GetAppInifileName()		{ return m_strAppInifileName; }
	CString&		GetAppDataPath()		{ return m_strAppDataPath; }
	CString&		GetAppLogPath()			{ return m_strAppLogPath; }
	CString&		GetAppFaPanelPath()		{ return m_strAppFaPanelPath; }
	CString&		GetServerName()			{ return m_strServerName; }
	CString&		GetSshServerName()		{ return m_strSshServerName; }
	CString&		GetSshUserName()		{ return m_strSshUserName; }
	CString&		GetSshKeyPath()			{ return m_strSshKeyPath; }
	CString&		GetSshBatchPath()		{ return m_strBatchPath; }

	CString&		GetTargetSpaceName()				{ return m_strSpaceName; }
	void			SetTargetSpaceName(CString& val)	{ m_strSpaceName = val; }

	UINT			GetDrawUpdateInterval(bool bRead);
	UINT			GetMessageThread() { return m_nMessageThread; }

	void			SetHcopyType(UINT val)	{ m_nHcopyType = val; }
	UINT			GetHcopyType()			{ return m_nHcopyType; }

	UINT			GetSelectStation()					{ return m_nSelectStation; }
	void			SetSelectStation(UINT val)			{ m_nSelectStation = val; }

	UINT			GetExecuteMode()					{ return m_nExecuteMode; }
	UINT			GetLimitedMode()					{ return m_nLimitedMode; }

	HWND			GetCallWnd()						{ return m_hCallWnd; }

	CString	GetShareFilePath(UINT fileType, UINT nStation = eStation_MAX);

	void	OpenView(UINT val);
	void	CloseView(UINT val);
	CView*	GetView(UINT val);

	deque<stHistoryData>&	GetEquipmentHistoryData()		{ return mEquipmentHistoryData; }
	deque<stHistoryData>&	GetEventHistoryData()			{ return mEventHistoryData; }

	void	SendControl(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete = TRUE);
	void	SendControlNoUI(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete = TRUE);

	int		GetPipeStationIndex()
	{
		if (m_nSelectStation == 0) return 0;
		if (m_nSelectStation == 1) return 3;
		if (m_nSelectStation == 2) return 1;
		if (m_nSelectStation == 3) return 2;
		return 0;
	};

	map<CString, CWnd*>	GetDlgMap()	{ return m_pDlgMap; }
	CWnd*	GetDlgMap(CString strClass)	{ return m_pDlgMap[strClass]; }
	void	SetDlgMap(CString strClass, CWnd* pWnd)
	{
		m_pDlgMap[strClass] = pWnd;
	}
	void	DeleteDlgMap(CString strClass)
	{
		m_pDlgMap[strClass] = NULL;
	}

protected:
	BOOL	CreateDocTemplate();


	// 監視データ＆履歴データ
public:
	void	SetEqMonData(UINT nStation, vector<CEngValCnv::obs_tbl_res_t>& data);
	char*	GetObsname_t(UINT nStation, CString strWatch);
	DWORD	GetHostIpInfo();

	deque<PIPEMONITORACK>&	GetMessageTemp(int nDispStation)		{ return mMessageTemp[nDispStation]; }

	CString	UTF8_3CharsToSJis(char *pUtf8Str);
	CString ConvUtf8Unicode(std::string& src);
	void	CreateDataToHistory(PIPEMONITORACK* pbuf);
	UINT	AddHistory(UINT message, WPARAM wParam, LPARAM lParam);

	bool	IsMessageEvent(TCHAR* pmsg);
	bool	IsMessageLimit(DWORD msgno);
	void	SetLimitedMode();

#ifdef MON_UT
	map<CString, CEngValCnv::obsname_t> mon_UT;
	void SetMonUT(vector<CEngValCnv::obsname_t>& data);
#endif


protected:
	// 監視データ（受信監視データ）
	map<UINT, map<CString, CEngValCnv::obsname_t>> m_mondatalist;




// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	CCommandLineInfoEx	m_cmdInfo;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern COperationTimeLineApp theApp;
