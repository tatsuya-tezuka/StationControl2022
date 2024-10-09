#pragma once
class CSshShell : public CWnd
{
	DECLARE_DYNAMIC(CSshShell)

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
//	deque<CWinThread*> threadList;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	static int ExecShell(structSshParam& sshParam, BOOL bWaitComplete = TRUE);
#if 0
	int ExecShell2(structSshParam& sshParam);

	void ExecShellThread(structSshParam& stParam);

	void SetParam(structSshParam param)		{ m_stSshParam = param; }

	static UINT ThreadProc(LPVOID pParam);
	void ThreadProc();
#endif

protected:
	DECLARE_MESSAGE_MAP()
//	afx_msg LRESULT OnCompleteSsh(WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};

