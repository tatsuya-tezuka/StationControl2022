/*============================================================================*/
/*! CSshShell.cpp

-# SSHシェル実行機能
*/
/*============================================================================*/
#include "stdafx.h"
#include "..\StationControl\StationControl.h"
#include "SshShell.h"

IMPLEMENT_DYNAMIC(CSshShell, CWnd)

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

BEGIN_MESSAGE_MAP(CSshShell, CWnd)
END_MESSAGE_MAP()

//#if 0
///*============================================================================*/
///*! CSshShell
//
//-# スレッド起動
//
//@param  pParam
//@retval 0：正常,
//*/
///*============================================================================*/
//UINT CSshShell::ThreadProc(LPVOID pParam)
//{
//	CSshShell* pDlg = dynamic_cast<CSshShell*>(reinterpret_cast<CWnd*>(pParam));
//	if (pDlg)
//	{
//		pDlg->ThreadProc();
//	}
//
//	return 0;
//}
//
//
///*============================================================================*/
///*! CSshShell
//
//-# スレッド起動
//
//@param  なし
//@retval なし
//*/
///*============================================================================*/
//void CSshShell::ThreadProc()
//{
//	int result = ExecShell2(m_stSshParam);
//	UINT nRemShellType = 0;
//	if (m_stSshParam.strRemShellName.Compare(DIS_SDTP_SH) == 0)
//	{
//		nRemShellType = eRemShellType_STAT_SDTP_SH;
//	}
//	GetParent()->PostMessage(eMessage_CompleteSsh, (WPARAM)result, (LPARAM)nRemShellType);
//	threadList.pop_front();
//}
//
//
///*============================================================================*/
///*! CSshShell
//
//-# スレッド起動
//
//@param  なし
//@retval なし
//*/
///*============================================================================*/
//void CSshShell::ExecShellThread(structSshParam& stParam)
//{
//	SetParam(stParam);
//
//	CWinThread* pThread = AfxBeginThread(ThreadProc, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
//	ASSERT(pThread);
//	if (pThread)
//	{
//		pThread->m_pMainWnd = this;
//		pThread->m_bAutoDelete = TRUE;
//		pThread->ResumeThread();
//		threadList.push_back(pThread);
//	}
//}
//#endif

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

	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: Batch param is (%s)"), __FILEW__, __LINE__, (LPCTSTR)strSshBatch);
		CLogTraceEx::Write(_T(""), _T("CSshShell"), _T("ExecShell"), _T("debug"), strDebug, nLogEx::debug);
	}

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
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: CreateProcess(ssh) failed, gle(%d)"), __FILEW__, __LINE__, GetLastError());
			CLogTraceEx::Write(_T(""), _T("CSshShell"), _T("ExecShell"), _T("error"), strDebug, nLogEx::error);
		}
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
			{
				CString strDebug;
				strDebug.Format(_T("%s(%d) :: CreateProcess(ssh), terminate(%lu)"), __FILEW__, __LINE__, rc);
				CLogTraceEx::Write(_T(""), _T("CSshShell"), _T("ExecShell"), _T("debug"), strDebug, nLogEx::debug);
			}

			ret = (int)rc;
		}
		else
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: CreateProcess(ssh), wait failed"), __FILEW__, __LINE__);
			CLogTraceEx::Write(_T(""), _T("CSshShell"), _T("ExecShell"), _T("debug"), strDebug, nLogEx::debug);
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

#if 0
/*============================================================================*/
/*! CSshShell

-# サーバにSSH接続し、シェルを実行する

@param  stParam：SSH構造体
@retval シェル実行結果(0：成功、その他：失敗）
*/
/*============================================================================*/
int CSshShell::ExecShell2(structSshParam& stParam)
{
	CWaitCursor wait;

	int ret = -1;

	CString strSshBatch;
	strSshBatch.Format(_T("%s %s@%s ./%s %s \"%s\""), stParam.strBatchPath, stParam.strUserName, stParam.strServerName, SVSTART_SH, stParam.strRemShellName, stParam.strRemShellParam);
	TRACE("%s", CStringA(strSshBatch));

	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: Batch param is (%s)"), __FILEW__, __LINE__, strSshBatch);
		CLogTraceEx::Write(_T(""), _T("CSshShell"), _T("ExecShell"), _T("debug"), strDebug, nLogEx::debug);
	}

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
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: CreateProcess(ssh) failed, gle(%d)"), __FILEW__, __LINE__, GetLastError());
			CLogTraceEx::Write(_T(""), _T("CSshShell"), _T("ExecShell"), _T("error"), strDebug, nLogEx::error);
		}
		strSshBatch.ReleaseBuffer();
		CloseHandle(stProcessInfo.hThread);
		CloseHandle(stProcessInfo.hProcess);
		return -1;
	}
	strSshBatch.ReleaseBuffer();

	// リモートシェル終了まで待機
	if (WaitForSingleObject(stProcessInfo.hProcess, INFINITE) == WAIT_OBJECT_0)
	{
		DWORD rc;
		GetExitCodeProcess(stProcessInfo.hProcess, &rc);
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: CreateProcess(ssh), terminate(%lu)"), __FILEW__, __LINE__, rc);
			CLogTraceEx::Write(_T(""), _T("CSshShell"), _T("ExecShell"), _T("debug"), strDebug, nLogEx::debug);
		}

		ret = (int)rc;
	}

	CloseHandle(stProcessInfo.hThread);
	CloseHandle(stProcessInfo.hProcess);

	return ret;
}
#endif

