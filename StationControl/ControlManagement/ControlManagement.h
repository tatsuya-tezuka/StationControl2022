
// ControlManagement.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル
#include "..\Utility\FileAccess.h"

// TKSC,SSOCホスト名
enum eHostName{
	eHost_sttn,		// 地上局
	eHost_ssoc,		// 相模原局
	eHost_tksc,		// 筑波局
};
// ※ホスト名は実際のホスト名の先頭５文字
static const TCHAR* mHostNameString[]{
//	_T("uzopr"),//仮
	_T("u6opr"),//臼田64局内
	_T("szopr"),
	_T("tzopr"),
};

using namespace std;

//=============================================================================
//	INIファイル
//=============================================================================
const CString SECTION_SYSTEM = _T("SYSTEM");
const CString KEY_FILESERVER_NAME = _T("FileServerName");	// ファイルサーバー名称
const CString KEY_SSHUSER_NAME = _T("SshUserName");		// SSHユーザ名称
const CString KEY_SSHSERVER_NAME = _T("SshServerName");	// SSHサーバ名称
const CString KEY_SSHKEY_PATH = _T("SshKeyFilePath");	// SSHキーファイルパス名称
const CString KEY_SSHBATCH_PATH = _T("SshBatchPath");	// SSHバッチファイルパス名称
#define SVSTART_SH				_T("start.sh")					// サーバ環境シェル
#define SEND_CRM_SH				_T("SEND_CRM.sh")				// 制御権利管理ファイル転送


#define SSH_WAITTIME			10000							// SSH待ち時間(ms)

class CSshShell
{
public:
	CSshShell();
	~CSshShell();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// SSH構造体
	typedef struct
	{
		CString strBatchPath;			// バッチファイルパス
		CString strUserName;			// SSHログインユーザ名
		CString strServerName;			// SSH接続先サーバ名またはIPアドレス
		CString strKeyFilePath;			// 秘密鍵ファイルパス
		CString strRemShellName;		// 実行するリモートシェル名
		CString strRemShellParam;		// 実行するリモートシェルコマンドへの引数。引数を複数記述する場合、"でくくる
	} structSshParam;

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	structSshParam	m_stSshParam;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	static int ExecShell(structSshParam& sshParam, BOOL bWaitComplete = TRUE);
};



// CControlManagementApp:
// このクラスの実装については、ControlManagement.cpp を参照してください。
//

class CControlManagementApp : public CWinApp
{
public:
	CControlManagementApp();

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
protected:
	CString		m_strAppPath;						// アプリケーションパス名
	CString		m_strAppInifileName;				// アプリケーションINIファイル名
	CString		m_strFileServerName;
	CString		m_strManagementPath;				// 管理用パス名
	CFileAccess	mFileAcs;
public:

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
protected:
	void	InitializeApp();
	bool	getUserSid(CString& strUserName, CString& strSid);
	void	getLastError();

public:
	CString		GetAppPath()		{ return m_strAppPath; }
	CString		GetAppIniFileName()	{ return m_strAppInifileName; }
	CString		GetServerName()		{ return m_strFileServerName; }
	CString		GetManagementPath()	{ return m_strManagementPath; }

	CFileAccess&	GetFileAccess()		{ return mFileAcs; }
	CString			ManEncodeDecode(CString str);
	void			GetLocalUserList(vector<stUserInfo>& userList);

//-----------------------------------------------------------------------------

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CControlManagementApp theApp;