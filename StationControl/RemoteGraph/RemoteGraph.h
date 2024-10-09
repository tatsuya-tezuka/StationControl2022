// この MFC サンプル ソース コードでは、MFC Microsoft Office Fluent ユーザー インターフェイス 
// ("Fluent UI") の使用方法を示します。このコードは、MFC C++ ライブラリ ソフトウェアに 
// 同梱されている Microsoft Foundation Class リファレンスおよび関連電子ドキュメントを
// 補完するための参考資料として提供されます。
// Fluent UI を複製、使用、または配布するためのライセンス条項は個別に用意されています。
// Fluent UI ライセンス プログラムの詳細については、Web サイト
// http://go.microsoft.com/fwlink/?LinkId=238214 を参照してください。
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// RemoteGraph.h : RemoteGraph アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル

#include "const.h"
#include "PropertiesWnd.h"
#include "SatelliteData.h"
#include "LogTraceEx.h"
#include "GraphException.h"
#include "DataCollection.h"
#include "CmdInf.h"

// CRemoteGraphApp:
// このクラスの実装については、RemoteGraph.cpp を参照してください。
//

class CRemoteGraphApp : public CWinAppEx
{
public:
	CRemoteGraphApp();

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	CString&		GetAppPath()			{ return m_strAppPath; }
	CString&		GetAppInifileName()		{ return m_strAppInifileName; }
	CString&		GetAppDataPath()		{ return m_strAppDataPath; }
	CString&		GetAppLogPath()			{ return m_strAppLogPath; }
//	CString&		GetServerAddress()		{ return m_strServerAddress; }
//	CString&		GetServerFilePath()		{ return m_strServerFilePath; }
	CString&		GetFileServerName()		{ return m_strFileServerName; }

	CPropertiesData&	GetPropertiesData(int pos)
	{
		if (pos < 0 || pos >= MAX_VIEW)
			return m_propData[0];
		return m_propData[pos];
	}
	void				SetPropertiesData(int pos, CPropertiesData val) { m_propData[pos] = val; }
	void	WriteAllPropertiesData(CString szFilePath)
	{
		for( int i = 0; i < MAX_VIEW; i++ )
			m_propData[i].WritePropertiesData(szFilePath, i);
	}
	void	ReadAllPropertiesData(CString szFilePath)
	{
		for( int i = 0; i < MAX_VIEW; i++ )
			m_propData[i].ReadPropertiesData(szFilePath, i);
	}

	CDataCollection&	GetDataCollection()		{ return m_DataCollection; }

	void			OpenNewView()				{ OnFileNew(); }

//	bool		GetTrendScrollMode(int pos) { return m_TrendScrollMode[pos]; }
//	void		SetTrendScrollMode(int pos, bool val) { m_TrendScrollMode[pos] = val; }

	UINT			GetDefSelectStationMode()			{ return m_nDefSelectMode; }
	void			SetDefSelectStationMode(UINT val)	{ m_nDefSelectMode = val; }
	UINT			GetDefSelectStation()				{ return m_nDefSelectStation; }
	void			SetDefSelectStation(UINT val)		{ m_nDefSelectStation = val; }
	UINT			GetSelectStationMode()				{ return m_nSelectMode; }
	void			SetSelectStationMode(UINT val)		{ m_nSelectMode = val; }
	UINT			GetSelectStation()					{ return m_nSelectStation; }
	void			SetSelectStation(UINT val)			{ m_nSelectStation = val; }
	UINT			GetSelectEquipment()					{ return m_nSelectEquipment; }
	void			SetSelectEquipment(UINT val)			{ m_nSelectEquipment = val; }

	// リアルタイムモードの測定の開始時刻と最新時刻(取得したデータの中で最も最新の時刻)
	CTime		GetRealTimeStart()	{ return m_RealTimeStart; }
	void		SetRealTimeStart(CTime& time)	{ m_RealTimeStart = time; }
	CTime		GetRealTimeLast()	{ return m_RealTimeLast; }
	void		SetRealTimeLast(CTime& time)	{ m_RealTimeLast = time; }
	CTimeSpan	GetRealTimeElapsed()	{ return m_RealTimeLast - m_RealTimeStart; }		// 開始時刻からの経過時間
	unsigned __int64	GetRealTimeCount()	{ return GetRealTimeElapsed().GetTotalSeconds() + 1; }	// 開始時刻からの取得点数

	CTime		GetRealTimeWriteData(int index)	{ return m_RealTimeWriteData[index]; }
	void		SetRealTimeWriteData(int index, CTime& time)	{ m_RealTimeWriteData[index] = time; }
	CTime		GetRealTimeWriteMax()	{ return m_RealTimeWriteMax; }
	void		SetRealTimeWriteMax(CTime& time)	{ m_RealTimeWriteMax = time; }

	UINT		GetStartFile()	{ return m_StartFile; }
	void		SetStartFile(UINT value)	{ m_StartFile = value; }

	CString		GetShareFilePath(UINT fileType, UINT nStation = eStation_MAX);

	void	DeleteFileFromDataFolder(int nView);
	void	DeleteFileFromDataFolderAll();

	void		SetAnalysisMigration(int view)	{ m_bAnalysisMigration |= (1 << view); }
	void		ClrAnalysisMigration(int view)	{ m_bAnalysisMigration &= ~(1 << view); }
	bool		IsAnalysisMigration(int view)	{ return (m_bAnalysisMigration & (1 << view)) ? true : false; }
	bool		IsAnalysisMigrationAll()		{ return (m_bAnalysisMigration != 0x0) ? true : false; }

	bool		IsDiskFreeSpace()				{ return m_bDiskFreeSpace; }
	void		SetDiskFreeSpace(bool flg)		{ m_bDiskFreeSpace = flg; }
	bool		IsCheckDiskFreeSpace()			{ return m_bCheckDiskFreeSpace; }

	DWORD		GetHostIpInfo();

	UINT		GetLogLevel()	{ return m_nLogLevel; }

	CString		AddFileName(CString sSrcPath, CString sAddFileName)
	{
		TCHAR	szDir[_MAX_DIR];
		TCHAR	szDrive[_MAX_DRIVE];
		TCHAR	szFname[_MAX_FNAME];
		TCHAR	szExt[_MAX_EXT];

		_wsplitpath_s(sSrcPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, szExt, _MAX_EXT);

		CString	sResult;
		sResult.Format(_T("%s%s%s%s%s"), szDrive, szDir, szFname, (LPCTSTR)sAddFileName, szExt);

		return sResult;
	}

	deque<PIPEMONITORACK>&	GetMessageTemp(int nDispStation)		{ return mMessageTemp[nDispStation]; }

protected:
	void	_InitializeAppl();

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
	deque<PIPEMONITORACK>	mMessageTemp[eStation_MAX];

protected:
	CString		m_strAppPath;						// アプリケーションパス名
	CString		m_strAppInifileName;				// アプリケーションINIファイル名
	CString		m_strAppDataPath;					// アプリケーションDATAパス名
	CString		m_strAppLogPath;					// アプリケーションLOGパス名
	UINT		m_nLogLevel;
//	CString		m_strServerAddress;					// サーバーアドレス文字列（192.168.1.1）
//	CString		m_strServerFilePath;				// サーバーファイルパス文字列（\\server\\data）
	CString		m_strFileServerName;				// ファイルサーバー名称

	CPropertiesData	m_propData[MAX_VIEW];
	CDataCollection	m_DataCollection;

	CTime		m_RealTimeStart;
	CTime		m_RealTimeLast;

	// データの種類：データ登録されているデータを格納
	// データの順番：データ登録の順番に格納
	CTime		m_RealTimeWriteData[MAX_VIEW * MAX_TDATA];
	CTime		m_RealTimeWriteMax;

	UINT		m_StartFile;

//	bool		m_TrendScrollMode[MAX_VIEW];

	CCommandLineInfoEx	m_cmdInfo;
	
	UINT		m_nDefSelectMode;					// デフォルト表示局モード（0:１局 1:5局 2:10局）
	UINT		m_nDefSelectStation;				// デフォルト1局モード時の局（1:臼田64m局 2:臼田54m局 3:内之浦34局 4:内之浦20局）
	UINT		m_nSelectMode;						// 表示局モード（0:１局 1:5局 2:10局）
	UINT		m_nSelectStation;					// 1局モード時の局（1:臼田64m 2:臼田54m局 3:内之浦34局 4:内之浦20局）
	UINT		m_nSelectEquipment;					// データ登録の選択設備

	DWORD		m_bAnalysisMigration;				// 解析移行フラグ

	bool	m_bDiskFreeSpace;						// ディスク空き容量フラグ
	bool	m_bCheckDiskFreeSpace;					// ディスク空き容量チェック

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
};

extern CRemoteGraphApp theApp;
