
// SharedAlarm.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "SharedAlarm.h"
#include "SharedAlarmDlg.h"
#include "SelectServerClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSharedAlarmApp

BEGIN_MESSAGE_MAP(CSharedAlarmApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSharedAlarmApp コンストラクション

CSharedAlarmApp::CSharedAlarmApp()
{
	// 再起動マネージャーをサポートします
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の CSharedAlarmApp オブジェクトです。

CSharedAlarmApp theApp;


// CSharedAlarmApp 初期化

BOOL CSharedAlarmApp::InitInstance()
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

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	//============================================================================
	// コマンドラインの解析
	ParseCommandLine(m_cmdInfo);
	mDebug = m_cmdInfo.mDebug;
	mAlarmFile = m_cmdInfo.mAlarmFile;

	// ダイアログにシェル ツリー ビューまたはシェル リスト ビュー コントロールが
	// 含まれている場合にシェル マネージャーを作成します。
	CShellManager *pShellManager = new CShellManager;

	// MFC コントロールでテーマを有効にするために、"Windows ネイティブ" のビジュアル マネージャーをアクティブ化
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));

	if (mDebug == 0){
		// デバッグモードではないのでダイアログを表示しないで実行
		WriteShareMemory();
		if (pShellManager != NULL)
		{
			delete pShellManager;
		}
		return FALSE;
	}

	CSelectServerClient dlgmode;
	if (dlgmode.DoModal() == IDCANCEL){
		if (pShellManager != NULL)
		{
			delete pShellManager;
		}
		return FALSE;
	}

	CSharedAlarmDlg dlg(dlgmode.m_nMode, m_cmdInfo.mAlarmFile);
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//  記述してください。
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ダイアログが <キャンセル> で消された時のコードを
		//  記述してください。
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: ダイアログの作成に失敗しました。アプリケーションは予期せずに終了します。\n");
		TRACE(traceAppMsg, 0, "警告: ダイアログで MFC コントロールを使用している場合、#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS を指定できません。\n");
	}

	// 上で作成されたシェル マネージャーを削除します。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ダイアログは閉じられました。アプリケーションのメッセージ ポンプを開始しないで
	//  アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}

bool CSharedAlarmApp::WriteShareMemory()
{
	// ファイルの内容があるか
	if (mAlarmFile.IsEmpty() == true){
		if (mDebug != 0){
			AfxMessageBox(_T("アラームファイルが選択されていません"), MB_OK | MB_ICONERROR);
		}
		return false;
	}

	// アラームファイル読み込み
	vector<CShareAlarmIf::stAlarmItem> list;
	list.clear();
	int nStation = mShareAlarmIf.ConvertAlarmData(mAlarmFile, list);
	if (nStation < 0){
		if (mDebug != 0){
			AfxMessageBox(_T("アラームファイルの読み込みに失敗しました"), MB_OK | MB_ICONERROR);
		}
		return false;
	}

	// 共有メモリ生成
	bool ret = mShareAlarmIf.Create(false, nStation);
	if (ret == false){
		if (mDebug != 0){
			AfxMessageBox(_T("共有メモリの作成に失敗しました"), MB_OK | MB_ICONERROR);
		}
		return false;
	}

	// ファイルの内容を共有メモリに書き込み
	mShareAlarmIf.Send((CShareAlarmIf::stAlarmItem*)&list.at(0), list.size());

	return true;
}
