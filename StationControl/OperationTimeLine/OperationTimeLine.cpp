
// OperationTimeLine.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "OperationTimeLine.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "OperationTimeLineDoc.h"
#include "OperationTimeLineView.h"
#include "TimeLineView.h"
#include "EquipmentSystem.h"
#include "TimeEvenHistoryView.h"
#include "TimeLimitErrorHistoryView.h"
#include "EquipmentSystemForm.h"
#include <locale.h>
#include "..\Utility\SshShell.h"

#include "sys/stat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COperationTimeLineApp

BEGIN_MESSAGE_MAP(COperationTimeLineApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &COperationTimeLineApp::OnAppAbout)
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 標準の印刷セットアップ コマンド
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// COperationTimeLineApp コンストラクション

COperationTimeLineApp::COperationTimeLineApp()
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
	SetAppID(_T("OperationTimeLine.AppID.NoVersion"));

	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
	m_nHcopyType = ID_HCOPY_REVERSE;
}

// 唯一の COperationTimeLineApp オブジェクトです。

COperationTimeLineApp theApp;


// COperationTimeLineApp 初期化

BOOL COperationTimeLineApp::InitInstance()
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

	// コマンドラインの解析
	ParseCommandLine(m_cmdInfo);
	m_nSelectStation = m_cmdInfo.mSelectStation;
	m_nExecuteMode = m_cmdInfo.mExecuteMode;

	m_hCallWnd = m_cmdInfo.mCallWindow;
#ifndef _DEBUG
	if (m_hCallWnd == NULL){
		return FALSE;
	}
#endif

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

	// レジストリ情報を削除する
	Unregister();

	// 各種ファイル・フォルダの作成
	TCHAR	szFullPath[_MAX_PATH];
	TCHAR	szDir[_MAX_DIR];
	TCHAR	szDrive[_MAX_DRIVE];
	TCHAR	szFname[_MAX_FNAME];

	if (::GetModuleFileName(NULL, szFullPath, _MAX_PATH) == 0)
	{
		return FALSE;
	}

	_wsplitpath_s(szFullPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, NULL, 0);
	m_strAppPath.Format(_T("%s%s"), szDrive, szDir);
	//m_strAppInifileName.Format(_T("%s%s%s.ini"), szDrive, szDir, szFname);
	m_strAppInifileName.Format(_T("%s%sStationControl.ini"), szDrive, szDir);
	m_strAppDataPath.Format(_T("%s%sDATA"), szDrive, szDir);
	m_strAppLogPath.Format(_T("%s%sLOG\\"), szDrive, szDir);
	m_strAppFaPanelPath.Format(_T("C:\\FA-Panel\\PROJECT\\"));
	CreateDirectory(m_strAppDataPath, NULL);	//　フォルダ作成
	CreateDirectory(m_strAppLogPath, NULL);	//　フォルダ作成

	GetDrawUpdateInterval(true);

	TCHAR buf[MAX_PATH];
	GetPrivateProfileString(SECTION_SYSTEM, KEY_FILESERVER_NAME, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strServerName = buf;
	// ネットワークドライブの確立
	//CreateNetworkDrive();
	if (m_strServerName.IsEmpty() == false){
		m_strServerName += _T("\\");
	}

	// メイン MDI フレーム ウィンドウを作成します。
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// 親の「OperationControl」に自ハンドルを教える
	PostMessage(m_cmdInfo.mCallWindow, eMessageTime_TimeLineHandle, 1, (LPARAM)m_pMainWnd->m_hWnd);

	// DDE、file open など標準のシェル コマンドのコマンド ラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// コマンド ラインで指定されたディスパッチ コマンドです。アプリケーションが
	// /RegServer、/Register、/Unregserver または /Unregister で起動された場合、False を返します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_nLogLevel = GetPrivateProfileInt(SECTION_SYSTEM, KEY_LOGLEVEL, nLogEx::fatal, m_strAppInifileName);
	CLogTraceEx::Create(m_strAppLogPath, _T("OperationTimeLine"), m_nLogLevel, nLogEx::text);
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("COperationTimeLineApp"), _T("InitializeAppl"), _T("OperationTimeLine"), _T("Start"), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHSERVER_NAME, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strSshServerName = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHUSER_NAME, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strSshUserName = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHKEY_PATH, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strSshKeyPath = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHBATCH_PATH, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strBatchPath = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SERVICE_MODE, _T(""), buf, MAX_PATH, m_strAppInifileName);
	CString strServiceIni = buf;
	mServiceMode = eServiceMode_user;	// サービスのデフォルトはユーザモード
	for (int i = 0; i < eServiceMode_max; i++)
	{
		if (strServiceIni.CompareNoCase(mServiceModeString[i]) == 0)
		{
			mServiceMode = i;
			break;
		}
	}

	// 54m 履歴識別番号設定
	GetPrivateProfileString(SECTION_SYSTEM, KEY_HISTID54, DEFAULT_HISTID54, buf, MAX_PATH, m_strAppInifileName);
	m_nHistID54 = ::_tcstol(buf, NULL, 16);

	m_nMessageThread = GetPrivateProfileInt(SECTION_SYSTEM, KEY_MESSAGETHREAD, 1, m_strAppInifileName);

	SetLimitedMode();	//制限つき制御モードの設定

	if (CreateDocTemplate() == FALSE)
		return FALSE;

	for (int i = eTimeViewType_OperatioTimeline; i < eTimeViewType_Max; i++){
		OpenView(i);
	}
	((CMainFrame*)pMainFrame)->SetStationMenuBar();

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED/*m_nCmdShow*/);
	pMainFrame->UpdateWindow();

	((CMainFrame*)pMainFrame)->OnTileHorz();

	// 完全にメインフレームが作成された後でないとだめ
	::SendMessage(m_pMainWnd->m_hWnd, ID_PIPE_CREATE, 0, 0);

	return TRUE;
}
/*============================================================================*/
/*! アプリケーション

-# ドッキングウィンドウの作成

@param
@retval BOOL

*/
/*============================================================================*/
BOOL COperationTimeLineApp::CreateDocTemplate()
{
	for (int i = eTimeViewType_OperatioTimeline; i < eTimeViewType_Max; i++){
		CRuntimeClass* pViewClass;
		switch (i){
		case	eTimeViewType_OperatioTimeline:		pViewClass = RUNTIME_CLASS(CTimeLineView); break;
			//case	eTimeViewType_EquipmentSystem:		pViewClass = RUNTIME_CLASS(CEquipmentSystem); break;
		case	eTimeViewType_EquipmentSystem:		pViewClass = RUNTIME_CLASS(CEquipmentSystemForm); break;
		case	eTimeViewType_Event:				pViewClass = RUNTIME_CLASS(CTimeEvenHistoryView); break;
		case	eTimeViewType_LimitError:			pViewClass = RUNTIME_CLASS(CTimeLimitErrorHistoryView); break;
		default:	return TRUE;
		}
#ifdef MON_UT
		UINT nIDResource = IDR_PASSSTATUS_MON_UT;
#else
		UINT nIDResource = IDR_PASSSTATUS;
#endif /* MON_UT */
		switch (i){
		case	eTimeViewType_OperatioTimeline:	nIDResource = IDI_ICON_TIMELINE; break;
		case	eTimeViewType_EquipmentSystem:	nIDResource = IDI_ICON_STATUS; break;
		case	eTimeViewType_Event:			nIDResource = IDI_ICON_HISTORY; break;
		case	eTimeViewType_LimitError:		nIDResource = IDI_ICON_HISTORY; break;
		default:	return TRUE;
		}

		m_pDocTemplate[i] = new CMultiDocTemplate(nIDResource,
			RUNTIME_CLASS(COperationTimeLineDoc),
			RUNTIME_CLASS(CChildFrame), // custom MDI child frame
			pViewClass);
		if (!m_pDocTemplate[i])
			return FALSE;
		AddDocTemplate(m_pDocTemplate[i]);
	}

	return TRUE;
}

/*============================================================================*/
/*! アプリケーション

-# 指定されたビューを開く

@param	val			ウィンドウ種別
@retval

*/
/*============================================================================*/
void COperationTimeLineApp::OpenView(UINT val)
{
	// 指定のウィンドウを開く
	COperationTimeLineDoc* pdoc = (COperationTimeLineDoc*)m_pDocTemplate[val]->OpenDocumentFile(NULL);
}

/*============================================================================*/
/*! アプリケーション

-# 指定されたビューを閉じる

@param	val			ウィンドウ種別
@retval

*/
/*============================================================================*/
void COperationTimeLineApp::CloseView(UINT val)
{
	// ウィンドウ種別
	// 指定のウィンドウを閉じる
	if (m_pDocTemplate[val] != NULL){
		POSITION pt = m_pDocTemplate[val]->GetFirstDocPosition();
		while (pt){
			CDocument* pdoc = m_pDocTemplate[val]->GetNextDoc(pt);
			if (pdoc == NULL)
				break;
			POSITION pt2 = pdoc->GetFirstViewPosition();
			while (pt2){
				CView *pview = pdoc->GetNextView(pt2);
				CWnd *p;
				p = pview->GetParent();
				p->SendMessage(WM_CLOSE, 0, 0L);
				break;
			}
		}
	}
}
/*============================================================================*/
/*! アプリケーション

-# 指定されたビューを取得する

@param	val			ウィンドウ種別
@retval

*/
/*============================================================================*/
CView* COperationTimeLineApp::GetView(UINT val)
{
	if (m_pDocTemplate[val] != NULL){
		POSITION pt = m_pDocTemplate[val]->GetFirstDocPosition();
		while (pt){
			CDocument* pdoc = m_pDocTemplate[val]->GetNextDoc(pt);
			if (pdoc == NULL)
				break;
			POSITION pt2 = pdoc->GetFirstViewPosition();
			while (pt2){
				CView *pview = pdoc->GetNextView(pt2);
				return pview;
			}
		}
	}
	return NULL;
}

int COperationTimeLineApp::ExitInstance()
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	if (m_nLogLevel < nLogEx::none){
		CLogTraceEx::Write(_T(""), _T("COperationTimeLineApp"), _T("ExitInstance"), _T("OperationTimeLine"), _T("Stop"), nLogEx::info);
		CLogTraceEx::Close();
		CLogTraceEx::Delete();
	}
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	AfxOleTerm(FALSE);

	// 親の「OperationControl」に終了を教える
	PostMessage(m_cmdInfo.mCallWindow, eMessageTime_TimeLineFinish, 1, 0);

	return CWinAppEx::ExitInstance();
}

// COperationTimeLineApp メッセージ ハンドラー


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
void COperationTimeLineApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// COperationTimeLineApp のカスタマイズされた読み込みメソッドと保存メソッド

void COperationTimeLineApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void COperationTimeLineApp::LoadCustomState()
{
}

void COperationTimeLineApp::SaveCustomState()
{
}

// COperationTimeLineApp メッセージ ハンドラー





BOOL COperationTimeLineApp::PreTranslateMessage(MSG* pMsg)
{
	CView* p;
	if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_NCLBUTTONDOWN){
		for (int i = 0; i < eTimeViewType_Max; i++){
			if (i == eTimeViewType_Event || i == eTimeViewType_LimitError){
				p = GetView(i);
				if (p != NULL){
					if (i == eTimeViewType_Event)
						((CTimeEvenHistoryView*)p)->CloseChildWindow(pMsg);
					if (i == eTimeViewType_LimitError)
						((CTimeLimitErrorHistoryView*)p)->CloseChildWindow(pMsg);
				}
			}
		}
	}

	return CWinAppEx::PreTranslateMessage(pMsg);
}

/*============================================================================*/
/*! アプリケーション

-# ファイルパスの取得

@brief 共有フォルダのファイルパスまたはフォルダパスを取得する

@param	fileType	ファイルタイプ
@param	nStation	局番号（省略時は、１局画面で省略時は自局を、５局画面では臼田64m局を返す）
@retval ファイルパスまたはフォルダパス
*/
/*============================================================================*/
CString COperationTimeLineApp::GetShareFilePath(UINT fileType, UINT nStation/* = eStation_MAX*/)
{
	// 局番号の引数省略時
	if (nStation == eStation_MAX)
	{
		nStation = m_nSelectStation;
	}

	// 局番号とファイルタイプから局別ファイル・フォルダパスを取得
	CString strFilePath;
	strFilePath.Format(_T("\\\\%sshare\\%s\\%s"), (LPCTSTR)m_strServerName, mStationFolder[nStation], mSvFileFolder[fileType]);

	if (m_strServerName.IsEmpty() == true){
		strFilePath.Format(_T("C:\\share\\%s\\%s"), mStationFolder[nStation], mLocalFileFolder[fileType]);
	}

	return strFilePath;
}

/*============================================================================*/
/*! CDBAccess

-# 監視データ格納

@param	nStation	：局番号
@param	data		：工学値変換データ
@retval

*/
/*============================================================================*/
void COperationTimeLineApp::SetEqMonData(UINT nStation, vector<CEngValCnv::obs_tbl_res_t>& data)
{
	CGraphMutex::Lock(eData);

	int cnt = (int)data.size();
	for (int i = 0; i < cnt; ++i)
	{
		CEngValCnv::obs_tbl_res_t mon_data = data[i];
		CString obsname = CString(mon_data.obs_adr.sz_obsname);
		m_mondatalist[nStation][obsname] = mon_data.obs_adr;
	}

	CGraphMutex::Unlock(eData);
}
/*============================================================================*/
/*! CDBAccess

-# 監視データ取得

@param	nStation	：局番号
@param	strWatch	：監視名称
@retval strct obsname_t ポインタ

*/
/*============================================================================*/
char* COperationTimeLineApp::GetObsname_t(UINT nStation, CString strWatch)
{
	char *obs = NULL;

	CGraphMutex::Lock(eData);

	if (m_mondatalist.count(nStation) != 0)
	{
		map<CString, CEngValCnv::obsname_t> &mon = m_mondatalist[nStation];

		if (mon.count(strWatch) != 0)
		{
			obs = (char*)&mon[strWatch];
		}
	}

#ifdef MON_UT
	if ((obs == NULL) && (!mon_UT.empty()))
	{
		map<CString, CEngValCnv::obsname_t> &mon_ut = mon_UT;
		if (mon_ut.count(strWatch) != 0)
		{
			obs = (char*)&mon_ut[strWatch];
		}
	}
#endif

	CGraphMutex::Unlock(eData);

	return obs;
}

#ifdef MON_UT
/*============================================================================*/
// 監視情報テーブルの更新
/*============================================================================*/
void COperationTimeLineApp::SetMonUT(vector<CEngValCnv::obsname_t>& data)
{
	int cnt = (int)data.size();

	CGraphMutex::Lock(eData);

	mon_UT.clear();
	for (int i = 0; i<cnt; ++i)
	{
		CEngValCnv::obsname_t mon_data = data[i];
		CString obsname = CString(mon_data.sz_obsname);
		mon_UT[obsname] = mon_data;
	}

	CGraphMutex::Unlock(eData);
}
#endif

/*============================================================================*/
/*! アプリケーション

-#  自IPアドレスの取得

@param

@retval

*/
/*============================================================================*/
DWORD COperationTimeLineApp::GetHostIpInfo()
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

/*============================================================================*/
/*! データ収集

-# 履歴情報の作成

@param	pUtf8Str			UTF-8

@retval	CString
*/
/*============================================================================*/
CString COperationTimeLineApp::UTF8_3CharsToSJis(char *pUtf8Str)
{
	wchar_t pUtfMsg[4];

	if (pUtf8Str[0] >= 0x00 && pUtf8Str[0] <= 0x7f){
		// 1バイト文字
		return _T("");
	}

	// UTF8 の３バイトをワイドキャラクタへ合成
	pUtfMsg[0] = ((pUtf8Str[0] & 0x0f) << 12)
		| ((pUtf8Str[1] & 0x3f) << 6)
		| (pUtf8Str[2] & 0x3f);
	pUtfMsg[1] = 0x0000;

	size_t wLen = 0;
	errno_t err = 0;

	//ロケール指定
	char* back = setlocale(LC_ALL, "japanese");
	//変換
	char	sjis[10];
	err = wcstombs_s(&wLen, sjis, 10, pUtfMsg, _TRUNCATE);
#ifndef _DEBUG
	setlocale(LC_ALL, back);
#endif
	if (err != 0)
	{
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CString msg;
		msg.Format(_T("Code Error : err(%d), 0x%02X 0x%02X 0x%02X"), err, pUtf8Str[0] & 0xFF, pUtf8Str[1] & 0xFF, pUtf8Str[2] & 0xFF);
		CLogTraceEx::Write(_T(""), _T("COperationTimeLineApp"), _T("UTF8_3CharsToSJis"), msg, _T("detail"), nLogEx::detail);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return _T("");
	}

	CString str(sjis);
	return str;
}
#ifdef _DEBUG
//#define _DEBUG_HISTORY 1		// 履歴デバッグデータ表示
#endif
/*============================================================================*/
/*! データ収集

-# 履歴情報の作成

@param	pbuf		サーバー履歴データ

@retval
*/
/*============================================================================*/
void COperationTimeLineApp::CreateDataToHistory(PIPEMONITORACK* pbuf)
{
	DWORD	dwStart = timeGetTime();

	stHistoryData data;
	memset(&data, 0, sizeof(stHistoryData));

	char* ptr = (char*)pbuf->buf;

	// 該当局を調べる
	ptr += 0x0C;	// 送信元識別
	DWORD nStation = *((DWORD*)ptr);
	if (nStation == 0x010a0102) // UDSC64
		data.station = 0;
	else if (nStation == m_nHistID54) // XDSC54
		data.station = 1;
	else if (nStation == 0x010a0203) // USC34
		data.station = 2;
	else if (nStation == 0x010a0103) // USC20
		data.station = 3;
	else{
		if (nStation == 0) // 筑波局内
			data.station = (BYTE)theApp.GetSelectStation();
	}

	ptr = (char*)pbuf->buf;
	CEngValCnv::sdtp_msg_ex_t* pmsg = (CEngValCnv::sdtp_msg_ex_t*)pbuf->buf;

#ifdef _DEBUG_HISTORY
	const char sdtp[109] = {
		/*0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x89, 0x00, 0x00, 0x00, 0xB0, 0x30, 0x00, 0x01,
		0x01, 0x00, 0x00, 0x00, 0x09, 0x80, 0x05, 0x00,
		0x04, 0x00, 0x02, 0x00,*/ 0x41, 0x00, 0x00, 0x00,
		0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B,
		0x02, 0x01, 0x0A, 0x01, 0x00, 0x00, 0x02, 0x01,
		0x04, 0x01, 0x00, 0x00, 0x10, 0x03, 0x6D, 0x00,
		0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00,
		0x32, 0x30, 0x31, 0x37, 0x2D, 0x30, 0x33, 0x2D,
		0x31, 0x30, 0x20, 0x30, 0x35, 0x3A, 0x34, 0x31,
		0x3A, 0x33, 0x32, 0x20, 0x43, 0x30, 0x20, 0x28,
		0x30, 0x30, 0x30, 0x30, 0x39, 0x29, 0x20, 0x64,
		0x69, 0x20, 0xE3, 0x83, 0x86, 0xE3, 0x82, 0xB9,
		0xE3, 0x83, 0x88, 0xE3, 0x83, 0xA1, 0xE3, 0x83,
		0x83, 0xE3, 0x82, 0xBB, 0xE3, 0x83, 0xBC, 0xE3,
		0x82, 0xB8, 0xE3, 0x81, 0xA7, 0xE3, 0x81, 0x99,
		0x00,
	};
	memcpy(ptr, (char*)&sdtp[0], 109);
#endif

	CString strDate, strTime, strSend, strMsgid, strLevel, strMsg;

	ptr += 0x2C;

//	// 54m局以外の場合（リモート化）
//	if (data.station != eStation_Usuda642)
//	{
//		CString buf;
//		int item = 0;
//		CString temp;
//		buf.Empty();
//		while (1){
//			if (*ptr == 0x00){
//				strMsg = buf;
//				break;
//			}
//			if (*ptr == 0x20 && item < 5){
//				// 一つのアイテムが決定
//				switch (item){
//				case	0:
//					strDate = buf;
//					break;
//				case	1:
//					strTime = buf;
//					break;
//				case	2:
//					strSend = buf;
//					break;
//				case	3:
//					strMsgid = buf;
//					break;
//				case	4:
//					strLevel = buf;
//					break;
//				case	5:
//					strMsg = buf;
//					break;
//				}
//				ptr++;
//				item++;
//				buf.Empty();
//				continue;
//			}
//			if (*ptr >= 0x00 && *ptr <= 0x7F){
//				// ASCIIコード
//				buf += CString(char(*ptr));
//				ptr++;
//			}
//			else if ((*ptr & 0xE0) == 0xE0){
//				// 漢字
//				buf += UTF8_3CharsToSJis(ptr);
//				ptr += 3;
//			}
//			else{
//				// その他
//				//=====================================================//
//				//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
//				CString msg;
//				msg.Format(_T("Code Error : 0x%02X"), *ptr & 0xFF);
//				CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("detail"), nLogEx::detail);
//				//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
//				//=====================================================//
//				return;
//			}
//		}
//	}
//	else
	{
		//電文よりメッセージ長を取得する(4ByteLSB) バッファが16384BYTEだったのでカット
		unsigned int msglen = ((BYTE)ptr[0x28] + ((BYTE)ptr[0x29] << 8) + ((BYTE)ptr[0x2A] << 16) + ((BYTE)ptr[0x2B] << 24)) % 0x4000;

		std::string stdbuf;
		int item = 0;
		CString temp;
		// メッセージ文字がメッセージ長になるまで繰り返す
		// メッセージ文字列例：2015-08-23 00:25:41 u6opr1 (5052) di CTRL.PRIV_STTN,ON,,ANS:OK
		// ①日付　②時間　③設備　④メッセージID　⑤状態　⑥メッセージ文字列
		while (1){
			if (msglen < 1 || *ptr == 0x00){
				// データフィールドが終わったので変換文字列をセットして終了
				// メッセージ文字列のコード変換
				strMsg = ConvUtf8Unicode(stdbuf);
				break;
			}
			// 区切り文字「0x20」見つかったら、変換した文字列をアイテム毎にセットする
			if (*ptr == 0x20 && item < 5){
				// 一つのアイテムが決定
				switch (item){
				case	0:	strDate = stdbuf.c_str();		break;
				case	1:	strTime = stdbuf.c_str();		break;
				case	2:	strSend = stdbuf.c_str();		break;
				case	3:	strMsgid = stdbuf.c_str();		break;
				case	4:	strLevel = stdbuf.c_str();		break;
				}
				ptr++;
				msglen--;
				item++;
				stdbuf = "";
				continue;
			}
			// 文字を連結する
			stdbuf += char(*ptr);
			ptr++;
			msglen--;
		}
	}

	CString str;
	int i = 0;
	// 日付
	i = 0;
	AfxExtractSubString(str, strDate, i++, '-');
	data.year = _wtoi(str);
	AfxExtractSubString(str, strDate, i++, '-');
	data.month = _wtoi(str);
	AfxExtractSubString(str, strDate, i++, '-');
	data.day = _wtoi(str);
	// 時間
	i = 0;
	AfxExtractSubString(str, strTime, i++, ':');
	data.hour = _wtoi(str);
	AfxExtractSubString(str, strTime, i++, ':');
	data.minute = _wtoi(str);
	AfxExtractSubString(str, strTime, i++, ':');
	data.second = _wtoi(str);

	// 送信元
	typedef struct {
		CString		name;
		UINT		code;
	} stSender;
	const stSender mSender[] = {
		{ _T("e1"), eSenderItem1 },	// 出先局側局運用管制装置
		{ _T("e2"), eSenderItem1 },	// 相模原側局運用管制装置
		{ _T("e3"), eSenderItemMax },	// 筑波局運用管制装置
		{ _T("c0"), eSenderItem2 },	// 設備制御装置
		{ _T("85"), eSenderItem3 },	// S帯TLM入力装置
		{ _T("86"), eSenderItem3 },	// X帯TLM入力装置
		{ _T("87"), eSenderItem4 },	// S帯コマンド出力装置
	};
#ifdef _DEBUG_HISTORY
	data.equipment = (BYTE)(rand() % (eSenderItemMax + 1));
#else
	strSend.MakeLower();
	data.equipment = eSenderItemMax + 1;
	for (int i = 0; i < sizeof(mSender) / sizeof(mSender[0]); i++){
		if (strSend == mSender[i].name){
			data.equipment = mSender[i].code;
			break;
		}
	}
	if (data.equipment == (eSenderItemMax + 1)){
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CString msg;
		msg.Format(_T("%s %s %s %s %s %s"), strDate, strTime, strSend, strMsgid, strLevel, strMsg);
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::debug);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return;
	}
#endif
	// メッセージID
#ifdef _DEBUG_HISTORY
	switch (rand() % 3){
	case	0:
		data.message = (DWORD)((rand() % 10000) + 30000);
		break;
	case	1:
		data.message = (DWORD)((rand() % 10000) + 10000);
		break;
	case	2:
		data.message = (DWORD)(30000);
		break;
	}
#else
	data.message = _wtoi(strMsgid.Mid(1, strMsgid.GetLength() - 2));
#endif

	// メッセージ文字列
	wsprintf(data.msgstring, _T("%s"), (LPCTSTR)strMsg);
#ifdef _DEBUG_HISTORY
	if ((rand() % 5) == 0){
		wsprintf(data.msgstring, _T("[イベント実行]%s"), (LPCTSTR)strMsg);
	}

#endif

	bool bLimit = false;
	if (data.message >= 30000 && data.message <= 39999 ||
		data.message >= 40000 && data.message <= 49999){
		bLimit = IsMessageLimit(data.message);
	}

	// ステータス
	if (data.message >= 30000 && data.message <= 39999 ||
		data.message >= 40000 && data.message <= 49999)
	{
		bool bStatus = false;
		// 状態遷移履歴
		// リミットエラー履歴
		switch (data.message % 10){
		case	1:/*POWER ON/OFF*/
			data.status = 4;
			break;
		case	2:/*REMOTE/LOCAL*/
			data.status = 3;
			break;
		case	3:/*MAINT/NORMAL*/
			data.status = 3;		// REMOTE/LOCALに含める
			break;
		case	4:/*WARNING*/
			data.status = 1;
			break;
		case	5:/*FAULT*/
			data.status = 2;
			break;
		case	6:/*FAULT*/
			data.status = 2;
			break;
		case	7:/*STATUS*/
			data.status = 0;
			break;
		case	8:/*復旧*/
			data.status = 0;
			data.imstatus = 0;
			bStatus = true;
			break;
		default:
			if (bLimit == true){
				if ((data.message % 10) == 0)
				{
					data.status = 0;	// STATUS
					data.imstatus = 0;	// 復旧
				}
				else if ((data.message % 10) == 9)
				{
					data.status = 2;	// FAULT
					data.imstatus = 2;	// 異常
				}
				bStatus = true;
				break;
			}
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CString msg;
			msg.Format(_T("%s %s %s %s %s %s"), (LPCTSTR)strDate, (LPCTSTR)strTime, (LPCTSTR)strSend, (LPCTSTR)strMsgid, (LPCTSTR)strLevel, (LPCTSTR)strMsg);
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::debug);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
			return;
		}
		if (bStatus == false)
		{
#ifdef _DEBUG_HISTORY
			data.imstatus = (BYTE)(rand() % 3);
#else
			if (strLevel == _T("di"))
				data.imstatus = 0;
			else if (strLevel == _T("dw"))
				data.imstatus = 1;
			else if (strLevel == _T("ds"))
				data.imstatus = 2;
			else if (strLevel == _T("dg"))
				data.imstatus = 0;
			else{// if (strLevel == _T("lo")){
				//=====================================================//
				//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
				CString msg;
				msg.Format(_T("%s %s %s %s %s %s"), strDate, strTime, strSend, strMsgid, strLevel, strMsg);
				CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::debug);
				//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
				//=====================================================//
				return;
			}
#endif
		}
	}
	else
	{
		// 上記以外
		data.imstatus = 0;
#ifdef _DEBUG_HISTORY
		data.status = (BYTE)(rand() % 3);
#else
		if (strLevel == _T("di"))
			data.status = 0;
		else if (strLevel == _T("dw"))
			data.status = 1;
		else if (strLevel == _T("ds"))
			data.status = 2;
		else if (strLevel == _T("dg"))
			data.status = 0;
		else{// if (strLevel == _T("lo")){
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CString msg;
			msg.Format(_T("%s %s %s %s %s %s"), strDate, strTime, strSend, strMsgid, strLevel, strMsg);
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::debug);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
			return;
		}
#endif
	}

	bool bOpeMsg = false;
	if (data.message >= 0 && data.message <= 29999){
		// 運用履歴
		bOpeMsg = true;
	}

#ifdef _DEBUG_HISTORY
	data.station = (BYTE)(rand() % eStation_MAX);
	if (data.equipment == eSenderItemMax){
		data.station = (BYTE)0;
	}
#else
	if (nStation == 0x010a0102) {// UDSC64
		// 関数冒頭で局番セット済み
	}
	else if (nStation == m_nHistID54) {// XDSC54
		// 関数冒頭で局番セット済み
	}
	else if (nStation == 0x010a0203) {// USC34
		// 関数冒頭で局番セット済み
	}
	else if (nStation == 0x010a0103) {// USC20
		// 関数冒頭で局番セット済み
	}
	else{
		if (bOpeMsg == true){
			// 運用履歴の場合は筑波局内のメッセージが存在し、局識別子が０
			// 関数冒頭で局番セット済み
		}
		else{
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CString msg;
			msg.Format(_T("%s %s %s %s %s 0x%08x %s"), strDate, strTime, strSend, strMsgid, strLevel, nStation, strMsg);
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::error);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
			return;
		}
	}
	if (data.equipment == eSenderItemMax){
		data.station = (BYTE)0;
	}
#endif

	if (theApp.GetSelectStation() != data.station){
		// 対象の局ではない
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CString msg;
		msg.Format(_T("Station(%d) is not support"), (int)data.station);
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("debug"), nLogEx::debug);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return;
	}


	bool bEvent = IsMessageEvent(data.msgstring);

	CGraphMutex::Lock(eData);
	if (bEvent == true){
		// イベント履歴
		mEventHistoryTemp.push_back(data);
	}
	else{
		if (data.message >= 30000 && data.message <= 39999){
			if (bLimit == true){
				// リミットエラー履歴
				mEquipmentHistoryTemp.push_back(data);
			}
		}
		if (data.message >= 40000 && data.message <= 49999){
			// リミットエラー履歴
			mEquipmentHistoryTemp.push_back(data);
		}
		if (data.message >= 60000 && data.message <= 69999){
			// イベント履歴
			mEventHistoryTemp.push_back(data);
		}
	}
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CString msg;
	msg.Format(_T("%s %s %s %s %s %s"), (LPCTSTR)strDate, (LPCTSTR)strTime, (LPCTSTR)strSend, (LPCTSTR)strMsgid, (LPCTSTR)strLevel, (LPCTSTR)strMsg);
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	DWORD dwElapsed = (timeGetTime() - dwStart);
	TRACE("### CreateDataToHistory Elapsed(%d)\n", dwElapsed);
	CGraphMutex::Unlock(eData);
}

/*============================================================================*/
/*! データ収集

-# 文字コード変換を行う

@param	src		　　UTF-8テキスト

@retval	Cstring		UNICODEテキスト
*/
/*============================================================================*/
CString COperationTimeLineApp::ConvUtf8Unicode(std::string& src)
{
	//UTF-16変換後のサイズを取得
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0U, src.data(), int(src.size()) + 1, nullptr, 0U);
	if (lenghtUnicode <= 0)
	{
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CString msg;
		msg.Format(_T("Code Error : MultiByteToWideChar Length(%d)"), lenghtUnicode);
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvUtf8Unicode"), msg, _T("detail"), nLogEx::detail);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return _T("");
	}

	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];
	//UTF-8からUTF-16へ変換
	MultiByteToWideChar(CP_UTF8, 0, src.data(), int(src.size()) + 1, bufUnicode, lenghtUnicode);

	CString strUnicode(bufUnicode);
	delete[] bufUnicode;

	return strUnicode;
}

/*============================================================================*/
/*! データ収集

-# メッセージ内容からイベント履歴かのチェックを行う

@param	pmsg	メッセージ文字列

@retval bool
*/
/*============================================================================*/
bool COperationTimeLineApp::IsMessageEvent(TCHAR* pmsg)
{
	const CString mMessageEvent[] = {
		{ _T("[イベント実行]") },
		{ _T("[シーケンス管理]") },
		{ _T("[TLM運用]") },
		{ _T("[CMD運用]") },
		{ _T("[RNG運用]") },
		{ _T("[変調制御管理]") },
		{ _T("[UPLINK運用]") },
		{ _T("[制御出力サブ]") },
		{ _T("[S-DL捕捉]") },
		{ _T("[X-DL捕捉]") },
		{ _T("[Ka-DL捕捉]") },
	};
	CString msg = CString(pmsg);
	for (int i = 0; i < sizeof(mMessageEvent) / sizeof(mMessageEvent[0]); i++){
		if (msg.Find(mMessageEvent[i]) >= 0){
			// イベント履歴の文字列が見つかった
			return true;
		}
	}
	return false;
}

/*============================================================================*/
/*! データ収集

-# メッセージ内容からイベント履歴かのチェックを行う処理

@param	msgno	メッセージ番号

@retval	bool
*/
/*============================================================================*/
bool COperationTimeLineApp::IsMessageLimit(DWORD msgno)
{
	CString msg;
	msg.Format(_T("%d"), msgno);
	if (msg.Right(1) == _T("9") || msg.Right(1) == _T("0")){
		return true;
	}
	return false;
}

/*============================================================================*/
// 制限つき制御モードの設定
/*============================================================================*/
void COperationTimeLineApp::SetLimitedMode()
{
	char host_name[256]; //ホスト名

	m_nLimitedMode = eLimitedMode_Disable;

	if(theApp.GetSelectStation() != eStation_Usuda642)
	{
		return;		//アプリ54m用以外
	}

	if(theApp.GetExecuteMode() != eExecuteMode_Control)
	{
		return;		//制御モード以外
	}

	if (mServiceMode == eServiceMode_limited)
	{
		// INIファイル設定によるデバッグ用の制限モード設定
		m_nLimitedMode = eLimitedMode_Enable;
		return;
	}
	else if (mServiceMode == eServiceMode_unlimited)
	{
		// INIファイル設定による制限モード無し設定のため、何もしない
		return;
	}

	if(gethostname(host_name, sizeof(host_name)) != 0)
	{
		return;
	}

	CString strHost = CString(host_name);
	if(strHost.Left(8).CompareNoCase(mHostNameUsuda) == 0)	//ホスト名 臼田宇宙空間観測所(臼田64m局舎内)
	{
		m_nLimitedMode = eLimitedMode_Enable;
	}

	return;
}

/*============================================================================*/
/*! データ収集

-# サから受信したデータを登録する

@param

@retval bool
*/
/*============================================================================*/
UINT COperationTimeLineApp::AddHistory(UINT message, WPARAM wParam, LPARAM lParam)
{
	CGraphMutex::Lock(eData);
	UINT	status = eHistStatus_Normal;

	try
	{
		// 一時的に格納されたデータを恒久領域に格納する
		while (!mEquipmentHistoryTemp.empty())
		{
			stHistoryData newData = mEquipmentHistoryTemp.front();
			mEquipmentHistoryTemp.pop_front();
			if (mEquipmentHistoryData.size() == MAX_HISTORY_DATA){
				mEquipmentHistoryData.pop_front();
			}
			mEquipmentHistoryData.push_back(newData);
			status = __max((BYTE)status, newData.status);
		}

		while (!mEventHistoryTemp.empty())
		{
			stHistoryData newData = mEventHistoryTemp.front();
			mEventHistoryTemp.pop_front();
			if (mEventHistoryData.size() == MAX_HISTORY_DATA){
				mEventHistoryData.pop_front();
			}
			mEventHistoryData.push_back(newData);
			status = __max((BYTE)status, newData.status);
		}
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);

	return status;
}
/*============================================================================*/
/*! アプリケーション

-# 制御送信

@brief	制御送信

@param	val1: 制御対象
@param	val2~4: パラメータ
@param	bWaitComplete：TRUE:シェル終了を待つ（デフォルト） / FALSE:シェル終了を待たない
@retval	なし

*/
/*============================================================================*/
void COperationTimeLineApp::SendControl(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete /*= TRUE*/)
{
	CString ctrl = val1;
	if (val2 != _T(""))
	{
		ctrl += _T(" ") + val2;
		if (val3 != _T(""))
		{
			ctrl += _T(" ") + val3;
			if (val4 != _T(""))
			{
				ctrl += _T(" ") + val4;
			}
		}
	}

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = SEND_CONTROL_SH;

	if (theApp.GetSelectStation() == eStation_Usuda64)
		stParam.strRemShellParam.Format(_T("udsc64 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Usuda642)
		stParam.strRemShellParam.Format(_T("udsc54 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
		stParam.strRemShellParam.Format(_T("usc34 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
		stParam.strRemShellParam.Format(_T("usc20 %s"), (LPCTSTR)ctrl);

	CString msg;
	msg.Format(_T("SendControl(%s)!!!"), (LPCTSTR)stParam.strRemShellName);
	CLogTraceEx::Write(_T(""), _T("COperationTimeLineApp"), _T("SendControl"), msg, _T(""), nLogEx::info);

	int err = CSshShell::ExecShell(stParam, bWaitComplete);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("COperationTimeLineApp"), _T("SendControl"), _T("debug"), strDebug, nLogEx::debug);
	}
}

void COperationTimeLineApp::SendControlNoUI(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete /*= TRUE*/)
{
	CString ctrl = val1;
	if (val2 != _T(""))
	{
		ctrl += _T(" ") + val2;
		if (val3 != _T(""))
		{
			ctrl += _T(" ") + val3;
			if (val4 != _T(""))
			{
				ctrl += _T(" ") + val4;
			}
		}
	}

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = SEND_CONTROL_NOUI_SH;

	if (theApp.GetSelectStation() == eStation_Usuda64)
		stParam.strRemShellParam.Format(_T("udsc64 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Usuda642)
		stParam.strRemShellParam.Format(_T("udsc54 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
		stParam.strRemShellParam.Format(_T("usc34 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
		stParam.strRemShellParam.Format(_T("usc20 %s"), (LPCTSTR)ctrl);

	CString msg;
	msg.Format(_T("SendControl(%s)!!!"), (LPCTSTR)stParam.strRemShellName);
	CLogTraceEx::Write(_T(""), _T("COperationTimeLineApp"), _T("SendControlNoUI"), msg, _T(""), nLogEx::info);

	int err = CSshShell::ExecShell(stParam, bWaitComplete);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("COperationTimeLineApp"), _T("SendControlNoUI"), _T("debug"), strDebug, nLogEx::debug);
	}
}


/*============================================================================*/
/*! アプリケーション

-# 設備アラーム状況一覧の削除

@param

@retval void
*/
/*============================================================================*/
UINT COperationTimeLineApp::GetDrawUpdateInterval(bool bRead)
{
	if (bRead == true){
		m_nDrawUpdateInterval = GetPrivateProfileInt(SECTION_SYSTEM, KEY_DRAWUPDATEINTERVAL, 500, m_strAppInifileName);
	}
	return m_nDrawUpdateInterval;
}
