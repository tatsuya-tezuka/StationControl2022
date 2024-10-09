
// ControlManagement.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "ControlManagement.h"
#include "ControlManagementDlg.h"
#include "ControlManagementInputPassword.h"

#include <lm.h>
#pragma comment( lib, "Netapi32.lib" )

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CControlManagementApp

BEGIN_MESSAGE_MAP(CControlManagementApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CControlManagementApp コンストラクション

CControlManagementApp::CControlManagementApp()
{
	// 再起動マネージャーをサポートします
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の CControlManagementApp オブジェクトです。

CControlManagementApp theApp;


// CControlManagementApp 初期化

BOOL CControlManagementApp::InitInstance()
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

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	InitializeApp();

	CControlManagementInputPassword man;
	if (man.DoModal() == IDCANCEL){
		return FALSE;
	}

	AfxEnableControlContainer();

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

	CControlManagementDlg dlg;
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

/*============================================================================*/
/*! 制御権利管理

-# アプリケーションの初期化

@param
@retval

*/
/*============================================================================*/
void CControlManagementApp::InitializeApp()
{
	// 各種ファイル・フォルダの作成
	TCHAR	szFullPath[_MAX_PATH];
	TCHAR	szDir[_MAX_DIR];
	TCHAR	szDrive[_MAX_DRIVE];
	TCHAR	szFname[_MAX_FNAME];

	if (::GetModuleFileName(NULL, szFullPath, _MAX_PATH) == 0)
	{
		return;
	}

	_wsplitpath_s(szFullPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, NULL, 0);
	m_strAppPath.Format(_T("%s%s"), szDrive, szDir);
	m_strAppInifileName.Format(_T("%s%sStationControl.ini"), szDrive, szDir);

	TCHAR buf[MAX_PATH];
	GetPrivateProfileString(SECTION_SYSTEM, KEY_FILESERVER_NAME, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strFileServerName = buf;
	CString strssoc, strsttn, strtksc;
	if (m_strFileServerName.IsEmpty() == false){
		m_strFileServerName += _T("\\");
	}
	if (m_strFileServerName.IsEmpty() == true){
		m_strManagementPath.Format(_T("C:\\share\\%s\\"), (LPCTSTR)mManagementPath);
		strssoc.Format(_T("C:\\share\\%s\\%s\\"), (LPCTSTR)mManagementPath, (LPCTSTR)mManagementSsoc);
		strsttn.Format(_T("C:\\share\\%s\\%s\\"), (LPCTSTR)mManagementPath, (LPCTSTR)mManagementSttn);
		strtksc.Format(_T("C:\\share\\%s\\%s\\"), (LPCTSTR)mManagementPath, (LPCTSTR)mManagementTksc);
	}
	else{
		m_strManagementPath.Format(_T("\\\\%sshare\\%s\\"), (LPCTSTR)m_strFileServerName, (LPCTSTR)mManagementPath);
		strssoc.Format(_T("\\\\%sshare\\%s\\%s\\"), (LPCTSTR)m_strFileServerName, (LPCTSTR)mManagementPath, (LPCTSTR)mManagementSsoc);
		strsttn.Format(_T("\\\\%sshare\\%s\\%s\\"), (LPCTSTR)m_strFileServerName, (LPCTSTR)mManagementPath, (LPCTSTR)mManagementSttn);
		strtksc.Format(_T("\\\\%sshare\\%s\\%s\\"), (LPCTSTR)m_strFileServerName, (LPCTSTR)mManagementPath, (LPCTSTR)mManagementTksc);
	}

	// フォルダがないかもしれないので作成する
	CreateDirectory(m_strManagementPath, NULL);	//　フォルダ作成
	//CreateDirectory(strssoc, NULL);	//　フォルダ作成
	//CreateDirectory(strsttn, NULL);	//　フォルダ作成
	//CreateDirectory(strtksc, NULL);	//　フォルダ作成


	// 制御権利関連のファイルが存在しない場合はデフォルト設定で作成する
	int i;
	CFileAccess& ac = theApp.GetFileAccess();
	CString strDefaultPass = mDefaultPassword;
	strDefaultPass = theApp.ManEncodeDecode(strDefaultPass);
	CStringA strPass = CStringA(strDefaultPass);

	//=========================================================================
	//	運用アカウント設定ファイルの作成
	//=========================================================================
	vector<stAccount> mAcList;
	vector<stAccount> mAccountList;
	vector<stUserInfo> mUserList;
	theApp.GetLocalUserList(mUserList);
	if (ac.ReadOperationAccount(mAcList) == false){
		// 運用アカウントファイルが存在しないので、ローカルユーザで初期化する
		mAccountList.clear();
		vector<stUserInfo>::iterator itr;
		for (itr = mUserList.begin(); itr != mUserList.end(); itr++){
			stAccount data;
			data.user = (*itr).user;
			data.usersid = (*itr).usersid;
			data.usc20 = false;
			data.usc34 = false;
			data.udsc64 = false;
			data.xdsc54 = false;
			data.enable = true;
			data.mycomputer = true;
			data.dummy = 0;
			mAccountList.push_back(data);
		}
		ac.WriteOperationAccount(mAccountList);
	}
	//=========================================================================
	//	制御権利管理用パスワードファイルの作成
	//=========================================================================
	CString mManPassword;
	if (ac.ReadManagementPassword(mManPassword) == false){
		// パスワードが設定されていないので、デフォルトパスワードを設定する
		mManPassword = mDefaultPassword;
		mManPassword = theApp.ManEncodeDecode(mManPassword);
		ac.WriteManagementPassword(mManPassword);
	}
	//=========================================================================
	//	パスワード・制御権利管理ファイルの作成
	//=========================================================================
	stControl	mTControl[mMaxType];
	stControl	mKControl[mMaxType];
	stControl	mSControl[mMaxType];
	// TKSC
	for (i = 0; i < mMaxType; i++){
		if (ac.ReadSOSControl(mTControl[i], eTKSCusc20 + i) == false){
			// ファイルが存在しないのでデフォルトを設定する
			// TKSCは常時１
			memset(&mTControl[i], 0, sizeof(stControl));
			mTControl[i].flag = 1;
			memcpy((char*)mTControl[i].password, (char*)(const char*)strPass, mMaxPassword);
			sprintf_s(mTControl[i].lifetime, mMaxLifetime, "%I64d", INT64_MAX);
			ac.WriteSOSControl(mTControl[i], eTKSCusc20 + i, true);
		}
	}
	// 地上局
	for (i = 0; i < mMaxType; i++){
		if (ac.ReadSOSControl(mKControl[i], eSOSusc20 + i) == false){
			// ファイルが存在しないのでデフォルトを設定する
			memset(&mKControl[i], 0, sizeof(stControl));
			mKControl[i].flag = 0;
			memcpy((char*)mKControl[i].password, (char*)(const char*)strPass, mMaxPassword);
			sprintf_s(mKControl[i].lifetime, mMaxLifetime, "0");
			ac.WriteSOSControl(mKControl[i], eSOSusc20 + i);
		}
	}
	// 相模原局管制
	for (i = 0; i < mMaxType; i++){
		if (ac.ReadSSOCControl(mSControl[i], eSSOCusc20 + i) == false){
			// ファイルが存在しないのでデフォルトを設定する
			memset(&mSControl[i], 0, sizeof(stControl));
			mSControl[i].flag = 0;
			memcpy((char*)mSControl[i].password, (char*)(const char*)strPass, mMaxPassword);
			sprintf_s(mSControl[i].lifetime, mMaxLifetime, "0");
			ac.WriteSSOCControl(mSControl[i], eSSOCusc20 + i);
		}
	}
	//=========================================================================
	//	制御権利取得状況ファイル
	//=========================================================================
	for (i = 0; i < mMaxType; i++){
		stLocalControl	tdata;
		if (ac.ReadLocalControl(tdata, eLOCALusc20 + i) == false){
			tdata.flag = 0;
			sprintf_s(tdata.lifetime, mMaxLifetime, "0");
			ac.WriteLocalControl(tdata, eLOCALusc20 + i);
		}
	}

}

/*============================================================================*/
/*! 制御権利管理

-# パスワードのエンコード,デコード

@param
@retval

*/
/*============================================================================*/
CString CControlManagementApp::ManEncodeDecode(CString str)
{
	// 8文字固定
	char tobuf[mMaxPassword + 1];
	char frombuf[mMaxPassword + 1];
	sprintf_s((char*)tobuf, mMaxPassword + 1, "%s", (LPCSTR)CStringA(str));
	memset(frombuf, 0, mMaxPassword + 1);

	short* toptr = (short*)tobuf;
	short* fromptr = (short*)frombuf;

	for (int i = 0; i < mMaxPassword / 2; i++){
		*fromptr = htons(*toptr);
		toptr++;
		fromptr++;
	}
	CString ret = CString(frombuf);
	return ret;
}

#include <sddl.h>

/*============================================================================*/
/*! 制御権利管理

-# ローカルユーザ一覧の取得

@param
@retval

*/
/*============================================================================*/
void CControlManagementApp::GetLocalUserList(vector<stUserInfo>& userList)
{
	LPUSER_INFO_1 pBuf = NULL;
	LPUSER_INFO_1 pTmpBuf;
	DWORD dwLevel = 1;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD i;
	DWORD dwTotalCount = 0;
	NET_API_STATUS nStatus;
	LPTSTR pszServerName = NULL;

	userList.clear();

	do
	{
		nStatus = NetUserEnum((LPCWSTR)pszServerName,
			dwLevel,
			FILTER_NORMAL_ACCOUNT, // global users
			(LPBYTE*)&pBuf,
			dwPrefMaxLen,
			&dwEntriesRead,
			&dwTotalEntries,
			&dwResumeHandle);
		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
		{
			if ((pTmpBuf = pBuf) != NULL)
			{
				// エントリー数分繰り返す
				for (i = 0; (i < dwEntriesRead); i++)
				{
					if (pTmpBuf == NULL)
					{
						break;
					}
					TRACE("\t-- %s\n", CStringA(pTmpBuf->usri1_name));
					if (!(pTmpBuf->usri1_flags & UF_ACCOUNTDISABLE) && pTmpBuf->usri1_priv != USER_PRIV_GUEST){
						CString str = CString(pTmpBuf->usri1_name);
						stUserInfo info;
						info.user = str;
						getUserSid(str, info.usersid);
						userList.push_back(info);
					}

					pTmpBuf++;
					dwTotalCount++;
				}
			}
		}
		if (pBuf != NULL)
		{
			NetApiBufferFree(pBuf);
			pBuf = NULL;
		}
	}
	while (nStatus == ERROR_MORE_DATA); // end do
	if (pBuf != NULL)
		NetApiBufferFree(pBuf);
}

bool CControlManagementApp::getUserSid(CString& strUserName, CString& strSid)
{
	// ローカルアカウントのSIDを取得する
	DWORD			cbDomain = 0;
	DWORD			cbUserSID = 0;
	SID_NAME_USE	snuType;
	TCHAR			*pszDomain = NULL;
	LPVOID			pUserSID = NULL;
	BOOL ret;

	strSid.Empty();

	// 最初の実行で各種サイズを取得する
	ret = ::LookupAccountName(NULL, (TCHAR*)(const TCHAR*)strUserName, pUserSID, &cbUserSID, pszDomain, &cbDomain, &snuType);
	if (ret == FALSE){
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER){
			return false;
		}
	}
	pUserSID = (PSID)malloc(cbUserSID);
	if (pUserSID == NULL){
		// メモリ確保失敗
		return false;
	}

	pszDomain = (TCHAR*)::malloc(cbDomain*sizeof(TCHAR));
	if (NULL == pszDomain){
		// メモリ確保失敗
		free(pUserSID);
		return false;
	}
	memset(pszDomain, 0, cbDomain * sizeof(TCHAR));

	// SID取得
	ret = ::LookupAccountName(NULL, (TCHAR*)(const TCHAR*)strUserName, pUserSID, &cbUserSID, pszDomain, &cbDomain, &snuType);
	if (ret == FALSE){
		free(pUserSID);
		free(pszDomain);
		return false;
	}

	// ここまでくればSIDは正常に取得できた
	PSID_IDENTIFIER_AUTHORITY psia;
	DWORD dwSidSize, dwSubAuthorities;

	if (!IsValidSid(pUserSID)){
		free(pUserSID);
		free(pszDomain);
		return false;
	}

	// コンピュータ名領域のクリア
	free(pszDomain);

	psia = GetSidIdentifierAuthority(pUserSID);
	dwSubAuthorities = *GetSidSubAuthorityCount(pUserSID);
	// S-SID_REVISION- + IdentifierAuthority- + subauthorities- + NULL
	dwSidSize = (15 + 12 + (12 * dwSubAuthorities) + 1) * sizeof(TCHAR);

	//LPTSTR pSID = NULL;
	//DWORD size = dwSidSize;
	//pSID = (LPTSTR)malloc(size);
	//if (pSID){
	//	memset(pSID, 0, size);
	//}
	//else{
	//	free(pUserSID);
	//	free(pszDomain);
	//	return false;
	//}

	CString temp;
	strSid.Empty();
	temp.Format(_T("S-%lu-"), SID_REVISION);
	strSid += temp;
	//dwSidSize = swprintf_s(pSID, dwSidSize * 2, _T("S-%lu-"), SID_REVISION);


	if ((psia->Value[0] != 0) || (psia->Value[1] != 0))
	{
		temp.Format(_T("0x%02hx%02hx%02hx%02hx%02hx%02hx"),
			(USHORT)psia->Value[0],
			(USHORT)psia->Value[1],
			(USHORT)psia->Value[2],
			(USHORT)psia->Value[3],
			(USHORT)psia->Value[4],
			(USHORT)psia->Value[5]);
		strSid += temp;
		//dwSidSize += swprintf_s(pSID + lstrlen(pSID),
		//	size,
		//	_T("0x%02hx%02hx%02hx%02hx%02hx%02hx"),
		//	(USHORT)psia->Value[0],
		//	(USHORT)psia->Value[1],
		//	(USHORT)psia->Value[2],
		//	(USHORT)psia->Value[3],
		//	(USHORT)psia->Value[4],
		//	(USHORT)psia->Value[5]);
	}
	else
	{
		temp.Format(_T("%lu"),
			(ULONG)(psia->Value[5]) +
			(ULONG)(psia->Value[4] << 8) +
			(ULONG)(psia->Value[3] << 16) +
			(ULONG)(psia->Value[2] << 24));
		strSid += temp;
		//dwSidSize += swprintf_s(pSID + lstrlen(pSID),
		//	size,
		//	_T("%lu"),
		//	(ULONG)(psia->Value[5]) +
		//	(ULONG)(psia->Value[4] << 8) +
		//	(ULONG)(psia->Value[3] << 16) +
		//	(ULONG)(psia->Value[2] << 24));
	}

	// Add SID subauthorities to the string
	for (DWORD dwCounter = 0; dwCounter < dwSubAuthorities; dwCounter++)
	{
		temp.Format(_T("-%lu"), *GetSidSubAuthority(pUserSID, dwCounter));
		strSid += temp;
		//dwSidSize += swprintf_s(pSID + dwSidSize,
		//	size,
		//	_T("-%lu"),
		//	*GetSidSubAuthority(pUserSID, dwCounter));
	}

	free(pUserSID);

	return true;
}

void CControlManagementApp::getLastError()
{
	LPVOID lpMsgBuf;
	if (FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 既定の言語
		(LPTSTR)&lpMsgBuf,
		0,
		NULL) == 0)
	{
		return;
	}

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, _T("Error"), MB_OK | MB_ICONINFORMATION);
	// バッファを解放する。
	LocalFree(lpMsgBuf);
}


/*============================================================================*/
/*! CSshShell

-# コンストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CSshShell::CSshShell()
{
}


/*============================================================================*/
/*! CSshShell

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CSshShell::~CSshShell()
{
}
/*============================================================================*/
/*! CSshShell

-# サーバにSSH接続し、シェルを実行する

@param  stParam：SSH構造体
@param	bWaitComplete：TRUE:シェル終了を待つ（デフォルト） / FALSE:シェル終了を待たない
@retval シェル実行結果(0：成功、その他：失敗）
*/
/*============================================================================*/
int CSshShell::ExecShell(structSshParam& stParam, BOOL bWaitComplete /*= TRUE*/)
{
	CWaitCursor wait;

	int ret = -1;

	CString strSshBatch;
	strSshBatch.Format(_T("%s %s@%s ./%s %s \"%s\""), (LPCTSTR)stParam.strBatchPath, (LPCTSTR)stParam.strUserName, (LPCTSTR)stParam.strServerName, SVSTART_SH, (LPCTSTR)stParam.strRemShellName, (LPCTSTR)stParam.strRemShellParam);
	TRACE("%s\n", CStringA(strSshBatch));

	if (stParam.strBatchPath.IsEmpty() ||
		stParam.strUserName.IsEmpty() ||
		stParam.strServerName.IsEmpty() ||
		stParam.strRemShellName.IsEmpty() ||
		stParam.strRemShellParam.IsEmpty())
		return ret;

	// 新しいプロセス用の構造体を起動する
	STARTUPINFO    stStartUpInfo;
	// メンバーの多くは予約されているか無視されるので、すべてのメンバーを
	// 0 に設定してから、必要なメンバーだけをリセットする
	memset(&stStartUpInfo, 0, sizeof(STARTUPINFO));
	stStartUpInfo.cb = sizeof(STARTUPINFO);			// 構造体のサイズls
	stStartUpInfo.dwFlags = STARTF_USESHOWWINDOW;	// wShowWindow メンバーを使う
	//	stStartUpInfo.wShowWindow = SW_SHOWDEFAULT;		// デフォルトの値
	stStartUpInfo.wShowWindow = SW_HIDE;			// 画面は非表示

	// 新しいプロセス上で情報を受け取るプロセス構造体
	PROCESS_INFORMATION stProcessInfo;

	// 新しいプロセスを作ると、プロセスはプライマリスレッドを作り、
	// アプリケーションを起動する
	if (CreateProcess(NULL, (LPTSTR)strSshBatch.GetBuffer(), NULL, NULL, FALSE,
		/*CREATE_SUSPENDED*/NORMAL_PRIORITY_CLASS, NULL,
		NULL, &stStartUpInfo, &stProcessInfo) == 0)
	{
		strSshBatch.ReleaseBuffer();
		return -1;
	}
	strSshBatch.ReleaseBuffer();

	// シェル終了を待つ場合
	if (bWaitComplete)
	{
		// リモートシェル終了まで待機
		if (WaitForSingleObject(stProcessInfo.hProcess, SSH_WAITTIME) == WAIT_OBJECT_0)
		{
			DWORD rc;
			GetExitCodeProcess(stProcessInfo.hProcess, &rc);
			ret = (int)rc;
		}
		else
		{
		}
	}
	// シェル終了を待たない場合
	else
	{
		ret = 0;
	}

	CloseHandle(stProcessInfo.hThread);
	CloseHandle(stProcessInfo.hProcess);

	return ret;
}
