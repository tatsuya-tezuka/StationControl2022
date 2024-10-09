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

// RemoteGraph.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "RemoteGraph.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "RemoteGraphDoc.h"
#include "RemoteGraphView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRemoteGraphApp

BEGIN_MESSAGE_MAP(CRemoteGraphApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CRemoteGraphApp::OnAppAbout)
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// CRemoteGraphApp コンストラクション

CRemoteGraphApp::CRemoteGraphApp()
{
	m_bHiColorIcons = TRUE;

	// 再起動マネージャーをサポートします
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// アプリケーションが共通言語ランタイム サポート (/clr) を使用して作成されている場合:
	//     1) この追加設定は、再起動マネージャー サポートが正常に機能するために必要です。
	//     2) 作成するには、プロジェクトに System.Windows.Forms への参照を追加する必要があります。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 下のアプリケーション ID 文字列を一意の ID 文字列で置換します。推奨される
	// 文字列の形式は CompanyName.ProductName.SubProduct.VersionInformation です
	SetAppID(_T("RemoteGraph.AppID.NoVersion"));

//	for(int i=0; i<MAX_VIEW; i++)
//		m_TrendScrollMode[i] = false;

	m_nSelectEquipment = 0;

	m_bAnalysisMigration = 0x0;

	m_bDiskFreeSpace = true;
	m_bCheckDiskFreeSpace = true;
}

// 唯一の CRemoteGraphApp オブジェクトです。

CRemoteGraphApp theApp;


// CRemoteGraphApp 初期化

BOOL CRemoteGraphApp::InitInstance()
{
	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// OLE ライブラリを初期化します。
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit コントロールを使用するには AfxInitRichEdit2() が必要です	
	// AfxInitRichEdit2();

	//============================================================================
	// コマンドラインの解析
	ParseCommandLine(m_cmdInfo);

	// コマンドライン解析の結果、「1局モード且つSELECT」の場合は局選択ダイアログを表示する
	m_nDefSelectMode = m_nSelectMode = m_cmdInfo.mStationMode;
	m_nDefSelectStation = m_nSelectStation = m_cmdInfo.mStation;
	m_strFileServerName = m_cmdInfo.mServerName;
	//============================================================================

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));
	LoadStdProfileSettings(4);  // 標準の INI ファイルのオプションをロードします (MRU を含む)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_RemoteGraphTYPE,
		RUNTIME_CLASS(CRemoteGraphDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CRemoteGraphView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// アプリケーションの初期化
	_InitializeAppl();

	// DATAフォルダ内のファイルを削除
	DeleteFileFromDataFolderAll();

	m_DataCollection.RequestDataList();

	// メイン MDI フレーム ウィンドウを作成します。
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// DDE、file open など標準のシェル コマンドのコマンド ラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

#if 1 // ↓ 起動時の新しい子ウィンドウの作成を行わない
	// Don't display a new MDI child window during startup
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
#endif

	// コマンド ラインで指定されたディスパッチ コマンドです。アプリケーションが
	// /RegServer、/Register、/Unregserver または /Unregister で起動された場合、False を返します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	// アプリケーションを初期化
	m_pMainWnd->PostMessage(eMessage_InitializeApplication, 0);

	{
		CString	sMsg;
		sMsg.Format(_T("MODE=%d, STATION=%d"), m_nSelectMode, m_nSelectStation);
		TRACE(_T("RemoteGraph %s"), sMsg);
		CLogTraceEx::Write(_T(""), _T("CRemoteGraphApp"), _T("InitInstance"), _T("RemoteGraph"), sMsg, nLogEx::info);
	}

	return TRUE;
}

int CRemoteGraphApp::ExitInstance()
{
	// DATAフォルダ内のファイルを削除
	DeleteFileFromDataFolderAll();

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	if (m_nLogLevel < nLogEx::none){
		CLogTraceEx::Write(_T(""), _T("CRemoteGraphApp"), _T("ExitInstance"), _T("RemoteGraph"), _T("Stop"), nLogEx::info);
		CLogTraceEx::Close();
		CLogTraceEx::Delete();
	}
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

/*============================================================================*/
/*! アプリケーション

	-# WORKフォルダをクリア

	@param	
	@retval 
*/
/*============================================================================*/
void CRemoteGraphApp::DeleteFileFromDataFolder(int nView)
{
	static const CString search_file[] =
	{
		_T("%s\\%s%s*.spd"),
	};

	HANDLE	hFind;
	WIN32_FIND_DATA	wData;

	for( int i = 0; i < sizeof(search_file) / sizeof(search_file[0]); i++ )
	{
		CString	szSearch;

		if( nView == -1 )
			szSearch.Format(search_file[i], theApp.GetAppDataPath(), REALTIME_FNAME, _T(""));
		else
		{
			CString	str;
			str.Format(_T("%d"), nView + 1);
			szSearch.Format(search_file[i], theApp.GetAppDataPath(), ANALYSIS_FNAME, str);
		}

		if( (hFind = FindFirstFile(szSearch, &wData)) == INVALID_HANDLE_VALUE )
			continue;

		do
		{
			if( !(wData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
			{
				CString	str = m_strAppDataPath + _T("\\") + wData.cFileName;
				DeleteFile(str);
			}
		}
		while( FindNextFile(hFind, &wData) );

		FindClose(hFind);
	}
}
void CRemoteGraphApp::DeleteFileFromDataFolderAll()
{
	static const CString search_file[] =
	{
		_T("\\*.spd"),
	};

	HANDLE	hFind;
	WIN32_FIND_DATA	wData;

	for( int i = 0; i < sizeof(search_file) / sizeof(search_file[0]); i++ )
	{
		CString	szSearch = theApp.GetAppDataPath() + search_file[i];

		if( (hFind = FindFirstFile(szSearch, &wData)) == INVALID_HANDLE_VALUE )
			continue;

		do
		{
			if( !(wData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
			{
				CString	str = m_strAppDataPath + _T("\\") + wData.cFileName;
				DeleteFile(str);
			}
		}
		while( FindNextFile(hFind, &wData) );

		FindClose(hFind);
	}
}

/*============================================================================*/
/*! アプリケーション

-# アプリケーションの初期化を行う

@param
@retval

*/
/*============================================================================*/
void CRemoteGraphApp::_InitializeAppl()
{
	// 各種ファイル・フォルダの作成
	TCHAR	szFullPath[_MAX_PATH];
	TCHAR	szDir[_MAX_DIR];
	TCHAR	szDrive[_MAX_DRIVE];
	TCHAR	szFname[_MAX_FNAME];

	if( ::GetModuleFileName(NULL, szFullPath, _MAX_PATH) == 0 )
		return;

	_wsplitpath_s(szFullPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, NULL, 0);
	m_strAppPath.Format(_T("%s%s"), szDrive, szDir);
	m_strAppInifileName.Format(_T("%s%s%s.ini"), szDrive, szDir, szFname);

	// Dドライブの存在を確認
	if( PathIsDirectory(_T("D:\\")) )
	{
		// DATAフォルダを作成
		m_strAppDataPath.Format(_T("D:\\%s"), theApp.m_pszAppName);
		CreateDirectory(m_strAppDataPath, NULL);
		m_strAppDataPath.Format(_T("D:\\%s\\DATA"), theApp.m_pszAppName);
		CreateDirectory(m_strAppDataPath, NULL);

		// LOGフォルダを作成
		m_strAppLogPath.Format(_T("D:\\%s\\LOG\\"), theApp.m_pszAppName);
		CreateDirectory(m_strAppLogPath, NULL);
	}
	else
	{
		// DATAフォルダを作成
		m_strAppDataPath.Format(_T("%s%sDATA"), szDrive, szDir);
		CreateDirectory(m_strAppDataPath, NULL);

		// LOGフォルダを作成
		m_strAppLogPath.Format(_T("%s%sLOG\\"), szDrive, szDir);
		CreateDirectory(m_strAppLogPath, NULL);
	}

	TCHAR buf[MAX_PATH];
	// ディスク空き容量チェック有効
	GetPrivateProfileString(SECTION_SYSTEM, KEY_CHECKDISKFREESPACE, _T("ON"), buf, MAX_PATH, m_strAppInifileName);
	CString strChkDiskSpace = buf;
	if (strChkDiskSpace == _T("OFF"))
	{
		m_bCheckDiskFreeSpace = false;
	}
	else
	{
		m_bCheckDiskFreeSpace = true;
	}

//	char szLocal[MAX_PATH];
//	CCommBase::GetLocalAddress(szLocal, MAX_PATH);
//	CString strLocal(szLocal);

//	TCHAR buf[MAX_PATH];
//	GetPrivateProfileString(SECTION_SYSTEM, KEY_SERVER, strLocal, buf, _countof(buf), m_strAppInifileName);
//	m_strServerAddress = buf;

//	GetPrivateProfileString(SECTION_SYSTEM, KEY_SERVERPATH, m_strAppDataPath, buf, _countof(buf), m_strAppInifileName);
//	m_strServerFilePath = buf;

	m_nLogLevel = GetPrivateProfileInt(SECTION_SYSTEM, KEY_LOGLEVEL, nLogEx::fatal, m_strAppInifileName);
	CLogTraceEx::Create(m_strAppLogPath, _T("RemoteGraph"), m_nLogLevel, nLogEx::text);

	// プロパティデータをINIファイルから復元
	theApp.ReadAllPropertiesData(theApp.GetAppInifileName());

	// データ取得対象フラグを設定
	theApp.GetDataCollection().SetSelectDataValid();

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CString	file_name, path_name = m_pszExeName;
	file_name = PathFindFileName(path_name);
	CLogTraceEx::Write(_T(""), _T("CRemoteGraphApp"), _T("_InitializeAppl"), file_name, _T("Start"), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
}



// CRemoteGraphApp メッセージ ハンドラー


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CRemoteGraphApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CRemoteGraphApp のカスタマイズされた読み込みメソッドと保存メソッド

void CRemoteGraphApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
//	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CRemoteGraphApp::LoadCustomState()
{
}

void CRemoteGraphApp::SaveCustomState()
{
}

// CRemoteGraphApp メッセージ ハンドラー

#if 0

void CRemoteGraphApp::OpenFile(CString strFileName)
{
	((CMainFrame*)m_pMainWnd)->OpenFile(strFileName);
}

#endif // 0

/*============================================================================*/
/*! アプリケーション

-# ファイルパスの取得

@brief 共有フォルダのファイルパスまたはフォルダパスを取得する

@param	fileType	ファイルタイプ
@param	nStation	局番号（省略時は、１局画面で省略時は自局を、５局画面では臼田64m局を返す）
@retval ファイルパスまたはフォルダパス
*/
/*============================================================================*/
CString CRemoteGraphApp::GetShareFilePath(UINT fileType, UINT nStation/* = eStation_MAX*/)
{
	// 局番号の引数省略時
	if (nStation == eStation_MAX)
	{
		// 局モードチェック
		switch (m_nSelectMode)
		{
		case eStation_1:
			// 自局番号をセット
			nStation = m_nSelectStation;
			break;

		case eStation_5:
			// 臼田64mをセット
			nStation = eStation_Usuda64;
			break;

		case eStation_10:
			// 10局モード
			nStation = eStation_Usuda64;
			break;

		default:
			nStation = eStation_Usuda64;
			break;
		}
	}

	// 局番号とファイルタイプから局別ファイル・フォルダパスを取得
	CString strFilePath;
	//	strFilePath.Format(_T("%s%s"), mNetworkDrive[nStation], mSvFileFolder[fileType]);
	strFilePath.Format(_T("\\\\%sshare\\%s\\%s"), (LPCTSTR)theApp.GetFileServerName(), mStationFolder[nStation], mSvFileFolder[fileType]);

	if( m_strFileServerName.IsEmpty() == true )
	{
		strFilePath.Format(_T("C:\\share\\%s\\%s"), mStationFolder[nStation], mLocalFileFolder[fileType]);
	}
#if 0 // ファイルの存在をチェックするなら下記コードを有効化
	else
	{
		CFile		f;
		CFileStatus	s;
		if( f.GetStatus(strFilePath, s) == 0 )
			strFilePath.Format(_T("C:\\share\\%s\\%s"), mStationFolder[nStation], mLocalFileFolder[fileType]);
	}
#endif

	return strFilePath;
}

/*============================================================================*/
/*! アプリケーション

-#  自IPアドレスの取得

@param

@retval

*/
/*============================================================================*/
DWORD CRemoteGraphApp::GetHostIpInfo()
{
	char 		host_name[256];				// ホストの名前
	memset(host_name, 0, sizeof(host_name));
	DWORD	ipAddress = 0;

	// ホスト名の取得
	if (gethostname(host_name, 256) == 0){
		struct addrinfo hints, *res;
		struct in_addr addr;
		int err;

		memset(&hints, 0, sizeof(hints));
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_family = AF_INET;

		if ((err = getaddrinfo(host_name, NULL, &hints, &res)) != 0) {
			printf("error %d\n", err);
			return 1;
		}

		addr.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;
		ipAddress = addr.S_un.S_addr;
	}
	return ipAddress;
}
