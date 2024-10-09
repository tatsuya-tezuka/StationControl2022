/*============================================================================*/
/*! SequentialCtrlRegister.cpp

-# 逐次制御登録
*/
/*============================================================================*/
// SequentialCtrlRegister.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "SequentialCtrlRegister.h"
#include "afxdialogex.h"
#include "SleepTimeInputDlg.h"
#include "PauseMsgInputDlg.h"
#include "CmdInputDlg.h"
#include "CommentInputDlg.h"
#include "HookMsgBox.h"
#include "SeqCtrlDefFileDlg.h"
#include "SeqCtrlDefCommentDlg.h"
#include "ControlRegistDlg.h"
#include "..\Utility\SshShell.h"

// CSequentialCtrlRegister ダイアログ

IMPLEMENT_DYNAMIC(CSequentialCtrlRegister, CDialogBase)

/*============================================================================*/
/*! CSequentialCtrlRegister

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CSequentialCtrlRegister::CSequentialCtrlRegister(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSequentialCtrlRegister::IDD, pParent)
	, m_nPosEditCtrl(-1)
	, m_bRunning(FALSE)
	, m_bPause(FALSE)
	, m_bModified(FALSE)
	, m_scrollPos(0)
	, m_hThreadExec(NULL)
	, m_hThreadSetLinePos(NULL)
	, m_hThreadPause(NULL)
	, m_hThreadEnd(NULL)
	, m_hThreadGetLinePos(NULL)
	, m_bThreadExecAlive(FALSE)
	, m_bThreadSetLinePosAlive(FALSE)
	, m_bThreadPauseAlive(FALSE)
	, m_bThreadEndAlive(FALSE)
	, m_bThreadGetLinePosAlive(FALSE)
	, m_newPos(-1)
	, m_bServerOk(FALSE)
{

}

/*============================================================================*/
/*! CSequentialCtrlRegister

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CSequentialCtrlRegister::~CSequentialCtrlRegister()
{
	ExitThread();
}

void CSequentialCtrlRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SEQCTRLREG_FILEID_EDIT, m_strFileName);
	DDX_Control(pDX, IDC_SEQCTRLREG_EQUIPMENTCTRL_LIST, m_listEqCtrl);
	DDX_Control(pDX, IDC_SEQCTRLREG_RUN_BUTTON, m_btnRun);
	DDX_Control(pDX, IDC_SEQCTRLREG_PAUSE_BUTTON, m_btnPause);
	DDX_Control(pDX, IDC_SEQCTRLREG_INTERRUPT_BUTTON, m_btnInt);
	DDX_Control(pDX, IDC_SEQCTRLREG_EXECSTEP_CHECK, m_chkStepExec);
	DDX_Control(pDX, IDC_SEQCTRLREG_WAITSEC_BUTTON, m_btnWaitSec);
	DDX_Control(pDX, IDC_SEQCTRLREG_PAUSE2_BUTTON, m_btnPause2);
	DDX_Control(pDX, IDC_SEQCTRLREG_COMMAND_BUTTON, m_btnComman);
	DDX_Control(pDX, IDC_SEQCTRLREG_COMMENT_BUTTON, m_btnComment);
	DDX_Control(pDX, IDC_SEQCTRLREG_UP_BUTTON, m_btn1UpLine);
	DDX_Control(pDX, IDC_SEQCTRLREG_DOWN_BUTTON, m_btn1DownLine);
	DDX_Control(pDX, IDC_SEQCTRLREG_DEL_BUTTON, m_btn1DelLine);
	DDX_Control(pDX, IDC_SEQCTRLREG_EDIT_BUTTON, m_btnEdit);
	DDX_Control(pDX, IDC_SEQCTRLREG_CTRLREGIST_BUTTON, m_btnCtrlRegist);
}


BEGIN_MESSAGE_MAP(CSequentialCtrlRegister, CDialogBase)
	ON_COMMAND(ID_SEQCTRLREG_FILE_CLOSE, &CSequentialCtrlRegister::OnMenuClose)
	ON_COMMAND(ID_SEQCTRLREG_FILE_OPEN, &CSequentialCtrlRegister::OnMenuOpen)
	ON_COMMAND(ID_SEQCTRLREG_FILE_NEW, &CSequentialCtrlRegister::OnMenuNew)
	ON_COMMAND(ID_SEQCTRLREG_FILE_SAVE, &CSequentialCtrlRegister::OnMenuSave)
	ON_BN_CLICKED(IDC_SEQCTRLREG_RUN_BUTTON, &CSequentialCtrlRegister::OnBnClickedSeqctrlregRunButton)
	ON_BN_CLICKED(IDC_SEQCTRLREG_PAUSE_BUTTON, &CSequentialCtrlRegister::OnBnClickedSeqctrlregPauseButton)
	ON_BN_CLICKED(IDC_SEQCTRLREG_INTERRUPT_BUTTON, &CSequentialCtrlRegister::OnBnClickedSeqctrlregInterruptButton)
	ON_BN_CLICKED(IDC_SEQCTRLREG_WAITSEC_BUTTON, &CSequentialCtrlRegister::OnBnClickedSeqctrlregWaitsecButton)
	ON_BN_CLICKED(IDC_SEQCTRLREG_PAUSE2_BUTTON, &CSequentialCtrlRegister::OnBnClickedSeqctrlregPause2Button)
	ON_BN_CLICKED(IDC_SEQCTRLREG_COMMAND_BUTTON, &CSequentialCtrlRegister::OnBnClickedSeqctrlregCommandButton)
	ON_BN_CLICKED(IDC_SEQCTRLREG_COMMENT_BUTTON, &CSequentialCtrlRegister::OnBnClickedSeqctrlregCommentButton)
	ON_BN_CLICKED(IDC_SEQCTRLREG_UP_BUTTON, &CSequentialCtrlRegister::OnBnClickedSeqctrlregUpButton)
	ON_BN_CLICKED(IDC_SEQCTRLREG_DOWN_BUTTON, &CSequentialCtrlRegister::OnBnClickedSeqctrlregDownButton)
	ON_BN_CLICKED(IDC_SEQCTRLREG_DEL_BUTTON, &CSequentialCtrlRegister::OnBnClickedSeqctrlregDelButton)
	ON_BN_CLICKED(IDC_SEQCTRLREG_EDIT_BUTTON, &CSequentialCtrlRegister::OnBnClickedSeqctrlregEditButton)
//	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_SEQCTRLREG_EQUIPMENTCTRL_LIST, &CSequentialCtrlRegister::OnLbnSelchangeSeqctrlregEquipmentctrlList)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SEQCTRLREG_CTRLREGIST_BUTTON, &CSequentialCtrlRegister::OnBnClickedSeqctrlregCtrlregistButton)
END_MESSAGE_MAP()


// CSequentialCtrlRegister メッセージ ハンドラー

/*============================================================================*/
/*! CSequentialCtrlRegister

-# ファイルメニューの閉じるから呼び出される

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnMenuClose()
{
	if (!CheckClose())
		return;

	//	OnOK();
	ShowWindow(SW_HIDE);
}

/*============================================================================*/
/*! CSequentialCtrlRegister

-# ファイルメニューの開くから呼び出される

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnMenuOpen()
{
	// エディットボックスキャンセル
//	CancelSeqCtrl();
	UpdateData(TRUE);

	// ファイルを開く
	CSeqCtrlDefFileDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_strFileName = dlg.GetFileName();
		m_strFilePath = dlg.GetFilePath();
		ReadComment(m_strFilePath + COMMENT_EXT, m_strComment);
		ReadFile(m_strFilePath);
	}

	UpdateData(FALSE);
	DrawMenuBar();
}

/*============================================================================*/
/*! CSequentialCtrlRegister

-# ファイルメニューの新規作成から呼び出される

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnMenuNew()
{
	m_strFileName = _T("");
	m_strFilePath = _T("");
	m_strComment = _T("");
//	// エディットボックスキャンセル
//	CancelSeqCtrl();
	// リストボックスクリア
	m_listEqCtrl.ResetContent();
	// 編集可能状態にする
	SetBtnEditable();
	UpdateData(FALSE);
}

/*============================================================================*/
/*! CSequentialCtrlRegister

-# ファイルメニューの上書き保存から呼び出される

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnMenuSave()
{
	(void)Save();
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 指定ファイルパスのファイルを読み込む

@param	filePath：ファイルパス
@retval	なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::ReadFile(CString& filePath)
{
	FILE *fp;

	TRY
	{

		if (_wfopen_s(&fp, filePath, _T("rb")) != 0 || fp == NULL)
		{
			TCHAR szTitle[256];
			TCHAR szMsg[256];
			memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
			memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
			if (!LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_READ, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
				return;
			if (!LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_DOSENOT_OPEN, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
				return;
			// ファイルオープン失敗
			MessageBox(CString(_T("逐次制御定義")) + CString(szMsg), szTitle, MB_OK | MB_ICONWARNING);
			return;
		}
	
		char szline[2048];
		CString strline;
		CString cbuf;
	
		// リストボックスクリア
		m_listEqCtrl.ResetContent();
		int nMaxLen = 0;
		CDC* memDC = m_listEqCtrl.GetDC();
		int nCount = 1;
		while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL)
		{
			// 読み込んだ文字列をリストに追加
			strline = CServerFile::ConvertUnicode(szline);
			strline.Replace(_T("\n"), _T(""));
			cbuf.Format(_T("%04d: %s"), nCount, (LPCTSTR)strline);
			m_listEqCtrl.InsertString(-1, cbuf);
			if (memDC != NULL)
			{
				CSize sz = memDC->GetTextExtent(cbuf);
				if (sz.cx > nMaxLen)
				{
					nMaxLen = sz.cx;
				}
			}
			nCount++;
		}

		m_listEqCtrl.SetHorizontalExtent(nMaxLen);
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL
	
	if (fp != NULL)
		fclose(fp);

	// リストボックスにフォーカスを当てる
	m_listEqCtrl.SetFocus();
	// リストボックスが空でない場合
	if (m_listEqCtrl.GetCount() > 0)
	{
		// 一番上の行にカーソルセット
		m_listEqCtrl.SetCurSel(0);
		// 実行編集可能状態にボタンをセット
		SetBtnRunEditable();
	}
	SetModified(FALSE);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 指定ファイルパスのファイルに保存する

@param	filePath：ファイルパス
@retval	なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::WriteFile(CString& filePath)
{
	FILE *fp;

	TRY
	{

		if (_wfopen_s(&fp, filePath, _T("wb")) != 0 || fp == NULL)
		{
			// ファイルオープン失敗
			TCHAR szTitle[256];
			TCHAR szMsg[256];
			memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
			memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
			if (!LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_SAVE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
				return;
			if (!LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_DOSENOT_SAVE, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
				return;
			MessageBox(CString(_T("逐次制御定義")) + CString(szMsg), szTitle, MB_OK | MB_ICONWARNING);
			return;
		}

		// リストボックスから文字列取得し、ファイルに書き込んでいく
		int nCount = m_listEqCtrl.GetCount();

		CString strLine;
		TCHAR szline[2048];
		CStringA cbuf;

		if (nCount == 0)
		{
			cbuf = CServerFile::ConvertEuc(_T(""));
			fputs(cbuf, fp);
		}
		else
		{
			for (int i = 0; i < nCount; i++)
			{
				m_listEqCtrl.GetText(i, strLine);
				strLine.Delete(0, 6);
				wsprintf(szline, _T("%s\n"), (LPCTSTR)strLine);
				cbuf = CServerFile::ConvertEuc(szline);
				fputs(cbuf, fp);
			}
		}

		m_strFilePath = filePath;
	}
	CATCH_ALL(e)
	{
		// ファイル書き込み失敗
		TCHAR szTitle[256];
		TCHAR szMsg[256];
		memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
		memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
		if (LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_SAVE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])) && 
			LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_DOSENOT_SAVE, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
			MessageBox(CString(_T("逐次制御定義")) + CString(szMsg), szTitle, MB_OK | MB_ICONWARNING);
	}
	END_CATCH_ALL

	if (fp != NULL)
		fclose(fp);

	SetModified(FALSE);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 指定ファイルパスのファイルからコメントを読み込む

@param	filePath：ファイルパス
@param	comment：コメント
@retval	なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::ReadComment(CString& filePath, CString& comment)
{
	FILE *fp;

	TRY
	{

		if (_wfopen_s(&fp, filePath, _T("rb")) != 0 || fp == NULL)
		{
			TCHAR szTitle[256];
			TCHAR szMsg[256];
			memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
			memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
			if (!LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_READ, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
				return;
			if (!LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_DOSENOT_OPEN, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
				return;
			// ファイルオープン失敗
			MessageBox(CString(_T("コメント")) + CString(szMsg), szTitle, MB_OK | MB_ICONWARNING);
			return;
		}

		char szline[2048];
		CString cbuf;

		if (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL)
		{
			// 読み込んだ文字列をリストに追加
			comment = CServerFile::ConvertUnicode(szline);
		}
	}
		CATCH_ALL(e)
	{
	}
	END_CATCH_ALL

	if (fp != NULL)
		fclose(fp);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 指定ファイルパスのファイルにコメントを保存する

@param	filePath：ファイルパス
@param	comment：コメント
@retval	なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::WriteComment(CString& filePath, CString& comment)
{
	FILE *fp;

	TRY
	{

		if (_wfopen_s(&fp, filePath, _T("wb")) != 0 || fp == NULL)
		{
			// ファイルオープン失敗
			TCHAR szTitle[256];
			TCHAR szMsg[256];
			memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
			memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
			if (!LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_SAVE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
				return;
			if (!LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_DOSENOT_SAVE, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
				return;
			MessageBox(CString(_T("コメント")) + CString(szMsg), szTitle, MB_OK | MB_ICONWARNING);
			return;
		}

		TCHAR szline[2048];
		CStringA cbuf;
		wsprintf(szline, _T("%s"), (LPCTSTR)comment);
		cbuf = CServerFile::ConvertEuc(szline);
		fputs(cbuf, fp);
	}
	CATCH_ALL(e)
	{
		// ファイル書き込み失敗
		TCHAR szTitle[256];
		TCHAR szMsg[256];
		memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
		memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
		if (LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_SAVE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])) &&
			LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_DOSENOT_SAVE, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
			MessageBox(CString(_T("コメント")) + CString(szMsg), szTitle, MB_OK | MB_ICONWARNING);
	}
	END_CATCH_ALL

	if (fp != NULL)
		fclose(fp);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 実行ボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregRunButton()
#if 0
{
	UpdateData(TRUE);

	// タイマークリア
	KillTimer(1);

	if (!m_bPause && !m_bRunning)
	{
		// 一時停止注または実行中でない場合は、一番最初の行にカーソルセット
		m_listEqCtrl.SetCurSel(0);
		UpdateData(FALSE);
	}

	// 実行中にセット
	m_bRunning = TRUE;
	m_bPause = FALSE;
	// コマンドを実行する
	ExecCommand();
	// ステップ実行の場合は、コマンドを実行
	if (m_chkStepExec.GetCheck())
	{
		GoNextListItem();
		// ボタンを実行可能にセット
		SetBtnRunable();
	}
	// ステップ実行でない場合は、タイマー
	else
	{
		// ボタンを実行中にセット
		SetBtnRunning();
		// タイマーセット
		SetTimer(1, 1000, NULL);
	}
}
#else
{
	UpdateData(TRUE);

	// 定義ファイルの更新チェック
	if (GetModified())
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(CString(_T("定義ファイルが更新されています。実行前にファイル保存してください。")), title, MB_OK | MB_ICONWARNING);
		return;
	}

	// 定義ファイル名があるかチェック
	if (m_strFileName.IsEmpty())
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(CString(_T("定義ファイルが指定されていません。")), title, MB_OK | MB_ICONWARNING);
		return;
	}

	// 定義ファイルが存在するかチェック
	{
		CString strFilePath;
		strFilePath.Format(_T("%s%s%s"), (LPCTSTR)theApp.GetShareFilePath(eFileType_SeqCtrlDef_Folder, theApp.GetSelectStation()), (LPCTSTR)m_strFileName, TXT_EXT);

		HANDLE hFile;
		hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE){
			CString title = _T("");
			GetWindowText(title);
			MessageBox(CString(_T("指定した定義ファイルが存在しません。")), title, MB_OK | MB_ICONWARNING);
			return;
		}
		CloseHandle(hFile);
	}

	m_newPos = m_listEqCtrl.GetCurSel();

	if (!m_bPause && !m_bRunning)
	{
		m_bFirstExec = TRUE;

		// スレッドを中断する
		m_bThreadExecAlive = FALSE;
		m_bThreadSetLinePosAlive = FALSE;
		m_bThreadPauseAlive = FALSE;
		m_bThreadEndAlive = FALSE;
		m_bThreadGetLinePosAlive = FALSE;

		// 行番号のファイルを消しておく
		CString strFolderPath = theApp.GetShareFilePath(eFileType_SeqCtrlTmp_Folder, theApp.GetSelectStation());
		CString strFilePath;
		strFilePath.Format(_T("%s\\%s"), (LPCTSTR)strFolderPath, SEQCTRL_LINEPOS_FILE);
		DeleteFile(strFilePath);
	}
	else
	{
		m_bFirstExec = FALSE;
	}

#if 0
	// 行位置読み込みスレッド
	if (m_bThreadGetLinePosAlive == FALSE)
	{
		UINT uiThreadId1;
		m_hThreadGetLinePos = (HANDLE)_beginthreadex(
			NULL,				// SECURITY_ATTRIBUTES 構造体へのポインタ
			0,					// 新しいスレッドのスタック サイズ
			SeqCtrlGetLinePosThread,	// 新しいスレッドの実行を開始するルーチンの開始アドレス
			this,				// スレッドパラメタ
			CREATE_SUSPENDED,	// 初期状態フラグ
			&uiThreadId1);		// スレッド識別子を受け取る 32 ビット変数へのポインタ

		if (m_hThreadGetLinePos != NULL)
		{
			m_bThreadGetLinePosAlive = TRUE;

			ResumeThread(m_hThreadGetLinePos);
		}
	}
#endif

	// 実行スレッド
	if (m_bThreadExecAlive == FALSE)
	{
		if (!m_bPause && !m_bRunning)
		{
			// 一時停止中および実行中でない場合は、一番最初の行にカーソルセット
			m_listEqCtrl.SetCurSel(0);
			UpdateData(FALSE);
		}

		if (m_hThreadExec != NULL)
		{
			// スレッドハンドラ解放
			WaitForSingleObject(m_hThreadExec, INFINITE);
			CloseHandle(m_hThreadExec);
			m_hThreadExec = NULL;
		}

		UINT uiThreadId2;
		m_hThreadExec = (HANDLE)_beginthreadex(
			NULL,				// SECURITY_ATTRIBUTES 構造体へのポインタ
			0,					// 新しいスレッドのスタック サイズ
			SeqCtrlExecThread,	// 新しいスレッドの実行を開始するルーチンの開始アドレス
			this,				// スレッドパラメタ
			CREATE_SUSPENDED,	// 初期状態フラグ
			&uiThreadId2);		// スレッド識別子を受け取る 32 ビット変数へのポインタ

		if (m_hThreadExec != NULL)
		{
			m_bThreadExecAlive = TRUE;
			// 実行中にセット
			m_bRunning = TRUE;
			m_bPause = FALSE;
			// ステップ実行の場合は、コマンドを実行
			if (m_chkStepExec.GetCheck())
			{
//				// ボタンを実行可能にセット
//				SetBtnRunable();
				// ボタンを処理待ちにセット
				SetBtnProcWaitting();
			}
			// ステップ実行でない場合
			else
			{
				// ボタンを実行中にセット
				SetBtnRunning();
			}

			ResumeThread(m_hThreadExec);
		}
	}
}
#endif

/*============================================================================*/
/*! CSequentialCtrlRegister

-# 一時停止ボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregPauseButton()
{
	UpdateData(TRUE);

	// ポーズスレッド
	if (m_bThreadPauseAlive == FALSE)
	{
		if (m_hThreadPause != NULL)
		{
			// スレッドハンドラ解放
			WaitForSingleObject(m_hThreadPause, INFINITE);
			CloseHandle(m_hThreadPause);
			m_hThreadPause = NULL;
		}

		UINT uiThreadId;
		m_hThreadPause = (HANDLE)_beginthreadex(
			NULL,				// SECURITY_ATTRIBUTES 構造体へのポインタ
			0,					// 新しいスレッドのスタック サイズ
			SeqCtrlPauseThread,	// 新しいスレッドの実行を開始するルーチンの開始アドレス
			this,				// スレッドパラメタ
			CREATE_SUSPENDED,	// 初期状態フラグ
			&uiThreadId);		// スレッド識別子を受け取る 32 ビット変数へのポインタ
	
		if (m_hThreadPause != NULL)
		{
			m_bThreadPauseAlive = TRUE;
//			// 停止
//			m_bRunning = FALSE;
//			m_bPause = TRUE;
//			// ボタンを実行可能にセット
//			SetBtnRunable();
			// ボタンを処理待ちにセット
			SetBtnProcWaitting();
	
			ResumeThread(m_hThreadPause);
		}
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 中断ボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregInterruptButton()
{
	UpdateData(TRUE);

	// 中断スレッド
	if (m_bThreadEndAlive == FALSE)
	{
		if (m_hThreadEnd != NULL)
		{
			// スレッドハンドラ解放
			WaitForSingleObject(m_hThreadEnd, INFINITE);
			CloseHandle(m_hThreadEnd);
			m_hThreadEnd = NULL;
		}

		UINT uiThreadId;
		m_hThreadEnd = (HANDLE)_beginthreadex(
			NULL,				// SECURITY_ATTRIBUTES 構造体へのポインタ
			0,					// 新しいスレッドのスタック サイズ
			SeqCtrlEndThread,	// 新しいスレッドの実行を開始するルーチンの開始アドレス
			this,				// スレッドパラメタ
			CREATE_SUSPENDED,	// 初期状態フラグ
			&uiThreadId);		// スレッド識別子を受け取る 32 ビット変数へのポインタ

		if (m_hThreadEnd != NULL)
		{
			m_bThreadEndAlive = TRUE;
//			// 中断
//			m_bRunning = FALSE;
//			m_bPause = FALSE;
//			m_listEqCtrl.SetCurSel(0);
//			// ボタンを編集可能にセット
//			SetBtnRunEditable();
			// ボタンを処理待ちにセット
			SetBtnProcWaitting();

			ResumeThread(m_hThreadEnd);
		}
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 指定秒待機挿入ボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregWaitsecButton()
{
//	// エディットボックスの内容登録
//	RegistSeqCtrl();
	UpdateData(TRUE);

	// 設定ダイアログ呼び出し
	CSleepTimeInputDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetString();
		// 指定秒待機挿入
		InsertSeqCtrl(str);
		// リストボックスが空でない場合
		if (m_listEqCtrl.GetCount() > 0)
		{
			// 実行編集可能状態にボタンをセット
			SetBtnRunEditable();
		}
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 一時停止挿入ボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregPause2Button()
{
//	// エディットボックスの内容登録
//	RegistSeqCtrl();
	UpdateData(TRUE);

	// 設定ダイアログ呼び出し
	CPauseMsgInputDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetString();
		// 一時停止挿入
		InsertSeqCtrl(str);
		// リストボックスが空でない場合
		if (m_listEqCtrl.GetCount() > 0)
		{
			// 実行編集可能状態にボタンをセット
			SetBtnRunEditable();
		}
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# コマンド挿入ボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregCommandButton()
{
//	// エディットボックスの内容登録
//	RegistSeqCtrl();
	UpdateData(TRUE);

	// 設定ダイアログ呼び出し
	CCmdInputDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetString();
		// コマンド挿入
		InsertSeqCtrl(str);
		// リストボックスが空でない場合
		if (m_listEqCtrl.GetCount() > 0)
		{
			// 実行編集可能状態にボタンをセット
			SetBtnRunEditable();
		}
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# コメント挿入ボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregCommentButton()
{
//	// エディットボックスの内容登録
//	RegistSeqCtrl();
	UpdateData(TRUE);

	// 設定ダイアログ呼び出し
	CCommentInputDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetString();
		// コメント挿入
		InsertSeqCtrl(str);
		// リストボックスが空でない場合
		if (m_listEqCtrl.GetCount() > 0)
		{
			// 実行編集可能状態にボタンをセット
			SetBtnRunEditable();
		}
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 一行上へボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregUpButton()
{
//	// エディットボックスの内容登録
//	RegistSeqCtrl();
	UpdateData(TRUE);

	// 現在行位置を取得
	int sel = m_listEqCtrl.GetCurSel();

	// 未選択もしくは一番上の行の場合は何もしない
	if (sel <= 0)
	{
		return;
	}

	// 上の行と入れ替える
	CString str;
	m_listEqCtrl.GetText(sel, str);
	m_listEqCtrl.DeleteString(sel);
	sel--;
	m_listEqCtrl.InsertString(sel, str);
	m_listEqCtrl.SetCurSel(sel);

	// 行番号振りなおし
	AddLineNumber();

	// リストボックスが空でない場合
	if (m_listEqCtrl.GetCount() > 0)
	{
		// カーソルセット
		m_listEqCtrl.SetCurSel(sel);
		// 実行編集可能状態にボタンをセット
		SetBtnRunEditable();
	}

	SetModified(TRUE);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 一行下へボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregDownButton()
{
//	// エディットボックスの内容登録
//	RegistSeqCtrl();
	UpdateData(TRUE);

	// 現在行位置を取得
	int sel = m_listEqCtrl.GetCurSel();

	// 未選択もしくは一番下の行の場合は何もしない
	if (sel < 0 || sel >= m_listEqCtrl.GetCount() - 1)
	{
		return;
	}

	// 下の行と入れ替える
	CString str;
	m_listEqCtrl.GetText(sel, str);
	m_listEqCtrl.DeleteString(sel);
	sel++;
	m_listEqCtrl.InsertString(sel, str);
	m_listEqCtrl.SetCurSel(sel);

	// 行番号振りなおし
	AddLineNumber();

	// リストボックスが空でない場合
	if (m_listEqCtrl.GetCount() > 0)
	{
		// カーソルセット
		m_listEqCtrl.SetCurSel(sel);
		// 実行編集可能状態にボタンをセット
		SetBtnRunEditable();
	}
	SetModified(TRUE);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 一削除ボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregDelButton()
{
//	// エディットボックスの内容登録
//	RegistSeqCtrl();
	UpdateData(TRUE);

	// 現在行位置を取得
	int sel = m_listEqCtrl.GetCurSel();

	// リストボックスが選択されている場合
	if (sel >= 0)
	{
		// 現在行を編集行にする
		m_nPosEditCtrl = sel;
	}
	// リストボックスの上のエディットボックスが選択されている場合
	else if (m_nPosEditCtrl >= 0)
	{
	}
	// 何も選択されていない場合
	else
	{
		// リストの最初の行を編集行にする
		m_nPosEditCtrl = 0;
	}
	// 削除する
	m_listEqCtrl.DeleteString(m_nPosEditCtrl);
	// 行番号振りなおし
	AddLineNumber();
	// リストの最後尾か調べる
	int nLast = m_listEqCtrl.GetCount() - 1;
	if (nLast >= 0)
	{
		if (m_nPosEditCtrl >= nLast)
		{
			// 選択位置を変更
			m_nPosEditCtrl = nLast;
		}
		// リストを選択しなおす
		m_listEqCtrl.SetCurSel(m_nPosEditCtrl);
	}
	SetModified(TRUE);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 編集ボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregEditButton()
{
//	// エディットボックスの内容登録
//	RegistSeqCtrl();
	UpdateData(TRUE);
	// 逐次制御を編集
	EditSeqCtrl();
}

/*============================================================================*/
/*! CSequentialCtrlRegister

-# 逐次制御を編集。
   編集文字列から、制御登録、指定秒待機、一時停止、コマンド、コメントに振り分ける

@param	なし
@retval TRUE: 編集に移行 / FALSE: 編集に移行しない

*/
/*============================================================================*/
BOOL CSequentialCtrlRegister::EditSeqCtrl()
#if 0
{
	// 現在行位置を取得
	int sel = m_listEqCtrl.GetCurSel();

	// リストボックスが選択されている場合
	if (sel >= 0)
	{
		// 現在行を編集行にする
		m_nPosEditCtrl = sel;
	}
	// リストボックスの上のエディットボックスが選択されている場合
	else if (m_nPosEditCtrl >= 0)
	{
	}
	// 何も選択されていない場合
	else
	{
		// リストの最初の行を編集行にする
		m_nPosEditCtrl = 0;
	}

	if (m_listEqCtrl.GetCount() <= 0)
	{
		// 文字が空の場合は、行1番号をリストに入れる
		m_listEqCtrl.InsertString(-1, _T("0001"));
	}

	CRect rect;
	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	m_listEqCtrl.GetItemRect(m_nPosEditCtrl, rect);

	// すでにエディットコントロールが有る場合は作り直す
	HWND hWnd = m_editCtrl.GetSafeHwnd();
	if (hWnd != NULL)
	{
		m_editCtrl.DestroyWindow();
	}

	// エディットコントロール生成
	m_editCtrl.Create(WS_VISIBLE | ES_AUTOHSCROLL | ES_LEFT, rect, &m_listEqCtrl, IDC_SEQCTRLREG_EDIT_ITEM);
	m_editCtrl.SetFont(pDefaultGUIFont, TRUE);
	CString str;
	m_listEqCtrl.GetText(m_nPosEditCtrl, str);
	str.TrimRight(_T("\n"));
	m_editCtrl.SetWindowTextW(str);
	m_editCtrl.ShowWindow(SW_SHOW);
	m_editCtrl.SetFocus();
	int nLen = str.GetLength();
	m_editCtrl.SetSel(nLen, nLen);
	SetModified(TRUE);

	return TRUE;
}
#else
{
	// 現在行位置を取得
	int sel = m_listEqCtrl.GetCurSel();

	// リストボックスが選択されている場合
	if (sel >= 0)
	{
		// 現在行を編集行にする
		m_nPosEditCtrl = sel;
	}
	// 何も選択されていない場合
	else
	{
		return FALSE;
	}

	// 選択行取得
	CString strOld, strNew;
	m_listEqCtrl.GetText(m_nPosEditCtrl, strOld);
	// 行番号を削除
	strOld.Delete(0, 6);
	strNew = _T("");

	// 文字列からダイアログ振り分け
	// sleep
	if (strOld.Find(_T("sleep"), 0) == 0)
	{
		strOld.Delete(0, 5 + 1);
		CSleepTimeInputDlg	dlg;
		dlg.SetString(strOld);
		if (dlg.DoModal() == IDOK)
		{
			strNew = dlg.GetString();
		}
	}
	// pause
	else if (strOld.Find(_T("pause"), 0) == 0)
	{
		strOld.Delete(0, 5 + 1);
		CPauseMsgInputDlg	dlg;
		dlg.SetString(strOld);
		if (dlg.DoModal() == IDOK)
		{
			strNew = dlg.GetString();
		}
	}
	// command
	else if (strOld.Find(_T("command"), 0) == 0)
	{
		strOld.Delete(0, 7 + 1);
		CCmdInputDlg	dlg;
		dlg.SetString(strOld);
		if (dlg.DoModal() == IDOK)
		{
			strNew = dlg.GetString();
		}
	}
	// #
	else if (strOld.Find(_T("#"), 0) == 0)
	{
		strOld.Delete(0, 1 + 1);
		CCommentInputDlg	dlg;
		dlg.SetString(strOld);
		if (dlg.DoModal() == IDOK)
		{
			strNew = dlg.GetString();
		}
	}
	// 制御
	else
	{
		CControlRegistDlg	dlg;
		CString strCrl, strVal;
		AfxExtractSubString(strCrl, strOld, 0, ',');
		AfxExtractSubString(strVal, strOld, 1, ',');
		dlg.SetCtrlString(strCrl);
		dlg.SetValString(strVal);
		if (dlg.DoModal() == IDOK)
		{
			strNew = dlg.GetString();
		}
	}

	if (strNew.IsEmpty())
	{
		return FALSE;
	}

	// 文字列削除
	m_listEqCtrl.DeleteString(m_nPosEditCtrl);
	// 文字列挿入
	CString cbuf;
	cbuf.Format(_T("%04d: %s"), m_nPosEditCtrl, (LPCTSTR)strNew);
	m_listEqCtrl.InsertString(m_nPosEditCtrl, cbuf);
	// 行番号振りなおし
	AddLineNumber();
	CDC* memDC = m_listEqCtrl.GetDC();
	if (memDC == NULL)
		return FALSE;
	CSize sz = memDC->GetTextExtent(cbuf);
	m_listEqCtrl.SetHorizontalExtent(sz.cx);
	m_listEqCtrl.SetCurSel(m_nPosEditCtrl);

	CRect rect;
	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	m_listEqCtrl.GetItemRect(m_nPosEditCtrl, rect);

	SetModified(TRUE);

	return TRUE;
}
#endif

/*============================================================================*/
/*! CSequentialCtrlRegister

-# 逐次制御を挿入。

@param	挿入文字列
@retval TRUE: 編集に移行 / FALSE: 編集に移行しない

*/
/*============================================================================*/
BOOL CSequentialCtrlRegister::InsertSeqCtrl(CString insertStr /*= _T("")*/)
{
	// 現在行位置を取得
	int sel = m_listEqCtrl.GetCurSel();

	// リストボックスが選択されている場合
	if (sel >= 0)
	{
		// 現在行の次の行を編集行にする
		m_nPosEditCtrl = sel + 1;
	}
	// リストボックスの上のエディットボックスが選択されている場合
	else if (m_nPosEditCtrl >= 0 && m_listEqCtrl.GetCount() > 0)
	{
		// エディットボックスの次の行を編集行にする
		m_nPosEditCtrl++;
	}
	// 何も選択されていない場合
	else
	{
		// リストの最初の行を編集行にする
		m_nPosEditCtrl = 0;
	}

	// 文字列挿入
	CString cbuf;
	cbuf.Format(_T("%04d: %s"), m_nPosEditCtrl, (LPCTSTR)insertStr);
	m_listEqCtrl.InsertString(m_nPosEditCtrl, cbuf);
	// 行番号振りなおし
	AddLineNumber();
	CDC* memDC = m_listEqCtrl.GetDC();
	if (memDC == NULL)
		return FALSE;
	CSize sz = memDC->GetTextExtent(cbuf);
	m_listEqCtrl.SetHorizontalExtent(sz.cx);
	m_listEqCtrl.SetCurSel(m_nPosEditCtrl);

	CRect rect;
	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	m_listEqCtrl.GetItemRect(m_nPosEditCtrl, rect);

	SetModified(TRUE);

#if 0
	// すでにエディットコントロールが有る場合は作り直す
	HWND hWnd = m_editCtrl.GetSafeHwnd();
	if (hWnd != NULL)
	{
		m_editCtrl.DestroyWindow();
	}

	// エディットコントロール生成
	m_editCtrl.Create(WS_VISIBLE | ES_AUTOHSCROLL | ES_LEFT, rect, &m_listEqCtrl, IDC_SEQCTRLREG_EDIT_ITEM);
	m_editCtrl.SetFont(pDefaultGUIFont, TRUE);
	CString str;
	m_listEqCtrl.GetText(m_nPosEditCtrl, str);
	m_editCtrl.SetWindowTextW(str);
	m_editCtrl.ShowWindow(SW_SHOW);
	m_editCtrl.SetFocus();
	int nLen = str.GetLength();
	m_editCtrl.SetSel(nLen, nLen);
#endif

	return TRUE;
}


#if 0
/*============================================================================*/
/*! CSequentialCtrlRegister

-# 編集した逐次制御行を登録。

@param	なし
@retval TRUE: 登録する / FALSE: 登録しない

*/
/*============================================================================*/
BOOL CSequentialCtrlRegister::RegistSeqCtrl()
{
	UpdateData(TRUE);

	BOOL ret = FALSE;
	HWND hWnd = m_editCtrl.GetSafeHwnd();
	if (hWnd == NULL)
		return ret;

	CString str;
	m_editCtrl.GetWindowText(str);
//	m_editCtrl.ShowWindow(SW_HIDE);
	m_editCtrl.DestroyWindow();
	if (str.CompareNoCase(_T("")) != 0 && m_nPosEditCtrl >= 0)
	{
		str = str + _T("\n");
		m_listEqCtrl.DeleteString(m_nPosEditCtrl);
		m_listEqCtrl.InsertString(m_nPosEditCtrl, str);
		// 行番号振りなおし
		AddLineNumber();
		CDC* memDC = m_listEqCtrl.GetDC();
		if (memDC == NULL)
			return FALSE;
		CSize sz = memDC->GetTextExtent(str);
		m_listEqCtrl.SetHorizontalExtent(sz.cx);
		SetModified(TRUE);
		m_listEqCtrl.SetSel(m_nPosEditCtrl, TRUE);
		ret = TRUE;
	}

	// リストボックスが空でない場合
	if (m_listEqCtrl.GetCount() > 0)
	{
		// 実行編集可能状態にボタンをセット
		SetBtnRunEditable();
	}

	return ret;
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 編集した逐次制御行をキャンセル。

@param	なし
@retval TRUE: 登録する / FALSE: 登録しない

*/
/*============================================================================*/
BOOL CSequentialCtrlRegister::CancelSeqCtrl()
//{
//	HWND hWnd = m_editCtrl.GetSafeHwnd();
//	if (hWnd == NULL)
//		return FALSE;
//
//	m_editCtrl.ShowWindow(SW_HIDE);
//
//	return TRUE;
//}
{
	HWND hWnd = m_editCtrl.GetSafeHwnd();
	if (hWnd != NULL)
	{
		m_editCtrl.DestroyWindow();
	}

	m_listEqCtrl.SetSel(m_nPosEditCtrl, TRUE);

	return TRUE;
}

BOOL CSequentialCtrlRegister::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg == NULL)
		return FALSE;

	if (pMsg->hwnd == m_editCtrl.GetSafeHwnd())
	{
		if (pMsg->wParam == VK_RETURN)
		{
			CWnd* pWnd = GetFocus();
			int nID = pWnd->GetDlgCtrlID();
			if (nID == IDC_SEQCTRLREG_EDIT_ITEM)
			{
				return RegistSeqCtrl();
			}
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			return CancelSeqCtrl();
		}
	}
	if (pMsg->hwnd == m_listEqCtrl.GetSafeHwnd())
	{
		CRect rect;
		m_listEqCtrl.GetItemRect(0, &rect);
		int index = m_listEqCtrl.GetTopIndex();
		int scrollPos = rect.Height() * index;
		if (scrollPos != m_scrollPos)
		{
			m_scrollPos = scrollPos;
			return RegistSeqCtrl();
		}
	}

	return CDialogBase::PreTranslateMessage(pMsg);
}
#endif

#if 0
/*============================================================================*/
/*! CSequentialCtrlRegister

-# タイマイベントで呼び出される

@param	nIDEvent：イベントID
@retval	なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnTimer(UINT_PTR nIDEvent)
{
	// コマンドを実行する
	ExecCommand();
	// 次のリストアイテムに進む
	GoNextListItem();

	CDialogBase::OnTimer(nIDEvent);
}
#endif

/*============================================================================*/
/*! CSequentialCtrlRegister

-# ダイアログ初期化処理

@param  なし
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
BOOL CSequentialCtrlRegister::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_nPosEditCtrl = -1;
	m_bRunning = FALSE;
	m_bPause = FALSE;
	m_bModified = FALSE;
	m_scrollPos = 0;

	m_hThreadExec = NULL;
	m_hThreadSetLinePos = NULL;
	m_hThreadPause = NULL;
	m_hThreadEnd = NULL;
	m_hThreadGetLinePos = NULL;
	m_bThreadExecAlive = FALSE;
	m_bThreadSetLinePosAlive = FALSE;
	m_bThreadPauseAlive = FALSE;
	m_bThreadEndAlive = FALSE;
	m_bThreadGetLinePosAlive = FALSE;

	m_newPos = -1;
	m_bServerOk = FALSE;

	SetBtnEditable();

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# ウィンドウ破棄時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnDestroy()
{
	CDialogBase::OnDestroy();

#if 0
	HWND hWnd = m_editCtrl.GetSafeHwnd();
	if (hWnd != NULL)
	{
		m_editCtrl.DestroyWindow();
	}
#endif

	KillTimer(1);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# ボタンを編集可能状態にする。

@param	なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::SetBtnEditable()
{
	m_btnRun.EnableWindow(FALSE);
	m_btnPause.EnableWindow(FALSE);
	m_btnInt.EnableWindow(FALSE);
	m_chkStepExec.EnableWindow(TRUE);
	m_btnWaitSec.EnableWindow(TRUE);
	m_btnPause2.EnableWindow(TRUE);
	m_btnComman.EnableWindow(TRUE);
	m_btnComment.EnableWindow(TRUE);
	m_btn1UpLine.EnableWindow(TRUE);
	m_btn1DownLine.EnableWindow(TRUE);
	m_btn1DelLine.EnableWindow(TRUE);
	m_btnEdit.EnableWindow(TRUE);
	m_btnCtrlRegist.EnableWindow(TRUE);

	// ファイルメニューを活性化
	CMenu* pMenu = GetMenu();
	if (pMenu != NULL)
	{
		pMenu->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
		DrawMenuBar();
	}

	// 閉じるボタン活性化
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(SC_CLOSE, MF_ENABLED);

	UpdateData(FALSE);
}

/*============================================================================*/
/*! CSequentialCtrlRegister

-# ボタンを実行編集可能状態にする。

@param	なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::SetBtnRunEditable()
{
	m_btnRun.EnableWindow(TRUE);
	m_btnPause.EnableWindow(FALSE);
	m_btnInt.EnableWindow(FALSE);
	m_chkStepExec.EnableWindow(TRUE);
	m_btnWaitSec.EnableWindow(TRUE);
	m_btnPause2.EnableWindow(TRUE);
	m_btnComman.EnableWindow(TRUE);
	m_btnComment.EnableWindow(TRUE);
	m_btn1UpLine.EnableWindow(TRUE);
	m_btn1DownLine.EnableWindow(TRUE);
	m_btn1DelLine.EnableWindow(TRUE);
	m_btnEdit.EnableWindow(TRUE);
	m_btnCtrlRegist.EnableWindow(TRUE);

	// ファイルメニューを活性化
	CMenu* pMenu = GetMenu();
	if (pMenu != NULL)
	{
		pMenu->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
		DrawMenuBar();
	}

	// 閉じるボタン活性化
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(SC_CLOSE, MF_ENABLED);

	UpdateData(FALSE);
}

/*============================================================================*/
/*! CSequentialCtrlRegister

-# ボタンを実行中状態にする。

@param	なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::SetBtnRunning()
{
	m_btnRun.EnableWindow(FALSE);
	m_btnPause.EnableWindow(TRUE);
	m_btnInt.EnableWindow(TRUE);
	m_chkStepExec.EnableWindow(FALSE);
	m_btnWaitSec.EnableWindow(FALSE);
	m_btnPause2.EnableWindow(FALSE);
	m_btnComman.EnableWindow(FALSE);
	m_btnComment.EnableWindow(FALSE);
	m_btn1UpLine.EnableWindow(FALSE);
	m_btn1DownLine.EnableWindow(FALSE);
	m_btn1DelLine.EnableWindow(FALSE);
	m_btnEdit.EnableWindow(FALSE);
	m_btnCtrlRegist.EnableWindow(FALSE);

	// ファイルメニューを非活性化
	CMenu* pMenu = GetMenu();
	if (pMenu != NULL)
	{
		pMenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED);
		DrawMenuBar();
	}

	// 閉じるボタン非活性化
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED);

	UpdateData(FALSE);
}

/*============================================================================*/
/*! CSequentialCtrlRegister

-# ボタンを実行可能状態にする。

@param	なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::SetBtnRunable()
{
	m_btnRun.EnableWindow(TRUE);
	m_btnPause.EnableWindow(FALSE);
	m_btnInt.EnableWindow(TRUE);
	m_chkStepExec.EnableWindow(TRUE);
	m_btnWaitSec.EnableWindow(FALSE);
	m_btnPause2.EnableWindow(FALSE);
	m_btnComman.EnableWindow(FALSE);
	m_btnComment.EnableWindow(FALSE);
	m_btn1UpLine.EnableWindow(FALSE);
	m_btn1DownLine.EnableWindow(FALSE);
	m_btn1DelLine.EnableWindow(FALSE);
	m_btnEdit.EnableWindow(FALSE);
	m_btnCtrlRegist.EnableWindow(FALSE);

	// ファイルメニューを非活性化
	CMenu* pMenu = GetMenu();
	if (pMenu != NULL)
	{
		pMenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED);
		DrawMenuBar();
	}

	// 閉じるボタン非活性化
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED);

	UpdateData(FALSE);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# ボタンを処理待ち状態にする。

@param	なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::SetBtnProcWaitting()
{
	m_btnRun.EnableWindow(FALSE);
	m_btnPause.EnableWindow(FALSE);
	m_btnInt.EnableWindow(FALSE);
	m_chkStepExec.EnableWindow(FALSE);
	m_btnWaitSec.EnableWindow(FALSE);
	m_btnPause2.EnableWindow(FALSE);
	m_btnComman.EnableWindow(FALSE);
	m_btnComment.EnableWindow(FALSE);
	m_btn1UpLine.EnableWindow(FALSE);
	m_btn1DownLine.EnableWindow(FALSE);
	m_btn1DelLine.EnableWindow(FALSE);
	m_btnEdit.EnableWindow(FALSE);
	m_btnCtrlRegist.EnableWindow(FALSE);

	// ファイルメニューを非活性化
	CMenu* pMenu = GetMenu();
	if (pMenu != NULL)
	{
		pMenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED);
		DrawMenuBar();
	}

	// 閉じるボタン非活性化
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED);

	UpdateData(FALSE);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 次のリストアイテムに進む。

@param	なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::GoNextListItem()
{
	int sel = m_listEqCtrl.GetCurSel();

	// 一番下に到達の場合、タイマーを止める
	if (sel >= m_listEqCtrl.GetCount() - 1)
	{
		KillTimer(1);
	}

//	// ステップ実行の場合は、一時停止
//	if (m_chkStepExec.GetCheck() == 1)
//	{
//		OnBnClickedSeqctrlregPauseButton();
//	}

	// リストの最後尾か調べる
	int nLast = m_listEqCtrl.GetCount() - 1;
	if (nLast >= 0)
	{
		if (sel >= nLast)
		{
			// 選択位置を変更
//			sel = nLast;
			sel = 0;
			// 終了
			m_bRunning = FALSE;
			m_bPause = FALSE;
			// ボタンを編集可能にセット
			SetBtnRunEditable();
		}
		else
		{
			// リストを選択しなおす
			sel++;
		}
		m_listEqCtrl.SetCurSel(sel);
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# コマンドを実行する。 リモートシェルを呼び出す。 ★★★

@param	strParam:コマンド引数文字列
@retval TRUE:成功 / FALSE:失敗

*/
/*============================================================================*/
BOOL CSequentialCtrlRegister::ExecCommand(CString strParam)
{
	// コマンドをリモートシェルで実行する
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = SEQ_CTRL_SH;
	CString arg;

	int nStation = theApp.GetSelectStation();
	arg.Format(_T("%s %s"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), (LPCTSTR)strParam);
	stParam.strRemShellParam.Format(arg);

	// リモシェ実行
	int err = CSshShell::ExecShell(stParam, TRUE);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShellThread"), __FILEW__, __LINE__);
		CLogTraceEx::Write(_T(""), _T("CSequentialCtrlRegister"), _T("ExecCommand"), _T("debug"), strDebug, nLogEx::debug);
	}
	if (err)
	{
		return FALSE;
	}

	return TRUE;
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# リストボックス選択変更時に呼ばれる。

@param	なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnLbnSelchangeSeqctrlregEquipmentctrlList()
{
	UpdateData(TRUE);

	int sel = m_listEqCtrl.GetCurSel();
	// リストの選択変更時
	if (sel >= 0 && sel != m_nPosEditCtrl)
	{
		m_nPosEditCtrl = sel;

		// 行変更スレッド
		if (m_bThreadSetLinePosAlive == FALSE)
		{
			if (m_hThreadSetLinePos != NULL)
			{
				// スレッドハンドラ解放
				WaitForSingleObject(m_hThreadSetLinePos, INFINITE);
				CloseHandle(m_hThreadSetLinePos);
				m_hThreadSetLinePos = NULL;
			}

			UINT uiThreadId;
			m_hThreadSetLinePos = (HANDLE)_beginthreadex(
				NULL,				// SECURITY_ATTRIBUTES 構造体へのポインタ
				0,					// 新しいスレッドのスタック サイズ
				SeqCtrlSetLinePosThread,	// 新しいスレッドの実行を開始するルーチンの開始アドレス
				this,				// スレッドパラメタ
				CREATE_SUSPENDED,	// 初期状態フラグ
				&uiThreadId);		// スレッド識別子を受け取る 32 ビット変数へのポインタ

			if (m_hThreadSetLinePos != NULL)
			{
				m_bThreadSetLinePosAlive = TRUE;
				ResumeThread(m_hThreadSetLinePos);
			}
		}
	}
}

/*============================================================================*/
/*! CSequentialCtrlRegister

-# ウィンドウプロシージャ。

@param	message：メッセージ
@param	wParam：追加情報
@param	lParam：追加情報
@retval なし

*/
/*============================================================================*/
LRESULT CSequentialCtrlRegister::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case eMessage_UpdateSeqCtrlPos:
	{
		int pos = (int)wParam;
		if (pos == SEQCTRL_END_LINE)
		{
			// 画面表示を逐次制御中断状態にする
			UpdateDispSeqCtrlEnd();
		}
		else if (pos >= 0)
		{
			// 新しい行位置をセットして再描画する
			m_newPos = pos;
			m_listEqCtrl.SetCurSel(m_newPos);

			// ステップ実行の場合は、コマンドを実行
			if (m_chkStepExec.GetCheck())
			{
				// ステップ実行の場合は、実行可能状態に移る
				m_bRunning = FALSE;
				m_bPause = TRUE;
				SetBtnRunable();
			}
			UpdateData(FALSE);
		}
	}
		break;

	case eMessage_PauseCompltete:
		// 画面表示を逐次制御ポーズ状態にする
		m_bRunning = FALSE;
		m_bPause = TRUE;
		// ボタンを実行可能にセット
		SetBtnRunable();
		break;

	case eMessage_WarnSeqCtrlSetDefFile:
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("逐次制御定義ファイルを指定できませんでした。"), title, MB_OK | MB_ICONWARNING);
		// 画面表示を逐次制御中断状態にする
		UpdateDispSeqCtrlEnd();
	}
		break;

	case eMessage_WarnSeqCtrlExec:
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("逐次制御を実行できませんでした。"), title, MB_OK | MB_ICONWARNING);
		// 画面表示を逐次制御中断状態にする
		UpdateDispSeqCtrlEnd();
	}
		break;

	case eMessage_WarnSeqCtrlPause:
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("逐次制御をポーズできませんでした。"), title, MB_OK | MB_ICONWARNING);
		// 画面表示を逐次制御中断状態にする
		UpdateDispSeqCtrlEnd();
	}
		break;

	case eMessage_WarnSeqCtrlSelLinePos:
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("逐次制御の行位置を指定できませんでした。"), title, MB_OK | MB_ICONWARNING);
		// 画面表示を逐次制御中断状態にする
		UpdateDispSeqCtrlEnd();
	}
		break;

	case eMessage_WarnSeqCtrlEnd:
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("逐次制御を終了できませんでした。"), title, MB_OK | MB_ICONWARNING);
		// 画面表示を逐次制御中断状態にする
		UpdateDispSeqCtrlEnd();
	}
		break;

	default:
		return CDialogBase::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 画面表示を逐次制御中断状態にする

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::UpdateDispSeqCtrlEnd()
{
	// スレッドを中断する
	m_bThreadExecAlive = FALSE;
	m_bThreadSetLinePosAlive = FALSE;
	m_bThreadPauseAlive = FALSE;
	m_bThreadEndAlive = FALSE;
	m_bThreadGetLinePosAlive = FALSE;

	// 画面表示を逐次制御中断状態にする
	UpdateData(TRUE);
	// 中断
	m_bRunning = FALSE;
	m_bPause = FALSE;
	m_listEqCtrl.SetCurSel(0);
	// ボタンを編集可能にセット
	SetBtnRunEditable();
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 閉じるボタン押下時に呼び出される

@param	なし
@retval	なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnClose()
{
	if (GetModified())
	{
		if (!CheckClose())
			return;

//		CDialogEx::OnOK();
		ShowWindow(SW_HIDE);
	}
	else
	{
		CDialogBase::OnClose();
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 閉じる確認時に呼び出される

@param	なし
@retval	TRUE:閉じる　/ FALSE:閉じない

*/
/*============================================================================*/
BOOL CSequentialCtrlRegister::CheckClose()
{
	if (GetModified())
	{
		TCHAR szTitle[256];
		TCHAR szMsg[256];
		memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
		memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
		if (!LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_TITLE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
			return FALSE;
		if (!LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_MODIFIED, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
			return FALSE;
		int nID = MessageBoxHooked(this->m_hWnd, szMsg, szTitle, MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON3);
		switch (nID)
		{
		case IDYES:
			return Save();
			break;

		case IDCANCEL:
			return FALSE;

		case IDNO:
		default:
			break;
		}
	}

	return TRUE;
}



/*============================================================================*/
/*! CSequentialCtrlRegister

-# 保存時に呼び出される

@param	なし
@retval	TRUE:保存した　/ FALSE:保存してない

*/
/*============================================================================*/
BOOL CSequentialCtrlRegister::Save()
{
	// エディットボックスの内容登録
//	RegistSeqCtrl();
	UpdateData(TRUE);

	if (m_strFileName.CompareNoCase(_T("")) == 0)
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("逐次制御定義ファイル名を入力してください。"), title, MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	else
	{
		CString strFilePath;
		strFilePath.Format(_T("%s%s%s"), (LPCTSTR)theApp.GetShareFilePath(eFileType_SeqCtrlDef_Folder, theApp.GetSelectStation()), (LPCTSTR)m_strFileName, TXT_EXT);
		// コメント保存
		CSeqCtrlDefCommentDlg dlg;
		dlg.SetComment(m_strComment);
		if (dlg.DoModal() == IDOK)
		{
			// コメント保存
			m_strComment = dlg.GetComment();
			WriteComment(strFilePath + COMMENT_EXT, m_strComment);
		}
		// ファイルを上書き保存
		WriteFile(strFilePath);
	}
	DrawMenuBar();

	return TRUE;
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# リストボックスの行番号を振りなおす。

@param	なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::AddLineNumber()
{
	int nCount = m_listEqCtrl.GetCount();
	CString strLine;
	CString cbuf;
	vector<CString> listNew;
	for (int i = 0; i < nCount; i++)
	{
		m_listEqCtrl.GetText(i, strLine);
		// 行番号を削除
		strLine.Delete(0, 6);
		// 行番号を振りなおしてリストに追加しておく
		cbuf.Format(_T("%04d: %s"), i + 1, (LPCTSTR)strLine);
		listNew.push_back(cbuf);
	}

	// リストボックスクリア
	m_listEqCtrl.ResetContent();

	// リストボックスに追加しなおす
	int nMaxLen = 0;
	CDC* memDC = m_listEqCtrl.GetDC();
	nCount = (int)listNew.size();
	for (int i = 0; i < nCount; i++)
	{
		m_listEqCtrl.InsertString(-1, listNew[i]);
		if (memDC != NULL)
		{
			CSize sz = memDC->GetTextExtent(listNew[i]);
			if (sz.cx > nMaxLen)
			{
				nMaxLen = sz.cx;
			}
		}
	}

	m_listEqCtrl.SetHorizontalExtent(nMaxLen);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 制御登録時に呼び出される。

@param	なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregCtrlregistButton()
{
	UpdateData(TRUE);

	CControlRegistDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetString();
		// コメント挿入
		InsertSeqCtrl(str);
		// リストボックスが空でない場合
		if (m_listEqCtrl.GetCount() > 0)
		{
			// 実行編集可能状態にボタンをセット
			SetBtnRunEditable();
		}
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 逐次制御実行スレッド ★★★

@param	なし
@retval なし

*/
/*============================================================================*/
UINT WINAPI CSequentialCtrlRegister::SeqCtrlExecThread(LPVOID pParam)
{
	CSequentialCtrlRegister* pThis = reinterpret_cast<CSequentialCtrlRegister*>(pParam);
	_ASSERTE(pThis != NULL);

	CString strCommand;

	// 初回の場合は定義ファイルを指定
	if (pThis->m_bFirstExec)
	{
		// 行番号を１オリジンにする
		strCommand.Format(_T("begin %s%s %d"), (LPCTSTR)pThis->m_strFileName, TXT_EXT, pThis->m_newPos + 1);
		if (pThis->ExecCommand(strCommand) == FALSE)
		{
			if (pThis->GetSafeHwnd() == NULL)
			{
				CString strDebug;
				strDebug.Format(_T("%s(%d) :: SeqCtrlExecThread (begin, NULL GetSafeHwnd)"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CSequentialCtrlRegister"), _T("SeqCtrlExecThread"), _T("debug"), strDebug, nLogEx::debug);

				return 0;
			}

			{
				CString strDebug;
				strDebug.Format(_T("%s(%d) :: SeqCtrlExecThread (eMessage_WarnSeqCtrlSetDefFile)"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CSequentialCtrlRegister"), _T("SeqCtrlExecThread"), _T("debug"), strDebug, nLogEx::debug);
			}

			pThis->m_bServerOk = FALSE;
			pThis->PostMessageW(eMessage_WarnSeqCtrlSetDefFile, 0, 0);

			pThis->m_bThreadExecAlive = FALSE;
		}
		else
		{
			pThis->m_bServerOk = TRUE;
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: SeqCtrlExecThread (begin m_bServerOk set TRUE)"), __FILEW__, __LINE__);
			CLogTraceEx::Write(_T(""), _T("CSequentialCtrlRegister"), _T("SeqCtrlExecThread"), _T("debug"), strDebug, nLogEx::debug);
		}
	}

	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: SeqCtrlExecThread (after begin)"), __FILEW__, __LINE__);
		CLogTraceEx::Write(_T(""), _T("CSequentialCtrlRegister"), _T("SeqCtrlExecThread"), _T("debug"), strDebug, nLogEx::debug);
	}

	CString strCommandStep;

	// ステップ実行
	if (pThis->m_chkStepExec.GetCheck())
	{
		char szPause, szPause2;
		do
		{
			// ポーズファイルを見て引数を決める
			if (pThis->GetSafeHwnd() == NULL)
				return 0;
			szPause = ReadPauseFile(pParam);
			Sleep(50);
			if (pThis->GetSafeHwnd() == NULL)
				return 0;
			szPause2 = ReadPauseFile(pParam);
			Sleep(50);

			{
				CString strDebug;
				strDebug.Format(_T("%s(%d) :: SeqCtrlExecThread (%c, %c)"), __FILEW__, __LINE__, szPause, szPause2);
				CLogTraceEx::Write(_T(""), _T("CSequentialCtrlRegister"), _T("SeqCtrlExecThread"), _T("debug"), strDebug, nLogEx::debug);
			}


		} while (szPause != szPause2);

		if (pThis->m_bFirstExec == TRUE || szPause == '0' || szPause == 0)
		{
			strCommandStep.Format(_T("pause S"));
		}
		else
		{
			int nPause;
			if (szPause == 'S')
			{
				nPause = 1;
			}
			else
			{
				nPause = atoi(&szPause);
				nPause++;
				if (nPause > 9)
					nPause = 1;
			}
			strCommandStep.Format(_T("pause %d"), nPause);
		}
	}
	// 再開実行
	else if (!pThis->m_bFirstExec)
	{
		strCommandStep.Format(_T("pause 0"));
	}

	// ステップ実行または再開実行
	if (!strCommandStep.IsEmpty())
	{
		if (pThis->ExecCommand(strCommandStep) == FALSE)
		{
			if (pThis->GetSafeHwnd() == NULL)
				return 0;

			{
				CString strDebug;
				strDebug.Format(_T("%s(%d) :: SeqCtrlExecThread (eMessage_WarnSeqCtrlExec)"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CSequentialCtrlRegister"), _T("SeqCtrlExecThread"), _T("debug"), strDebug, nLogEx::debug);
			}

			pThis->m_bServerOk = FALSE;
			pThis->PostMessageW(eMessage_WarnSeqCtrlExec, 0, 0);
		}
		else
		{
			{
				CString strDebug;
				strDebug.Format(_T("%s(%d) :: SeqCtrlExecThread (pause m_bServerOk set TRUE)"), __FILEW__, __LINE__);
				CLogTraceEx::Write(_T(""), _T("CSequentialCtrlRegister"), _T("SeqCtrlExecThread"), _T("debug"), strDebug, nLogEx::debug);
			}
			pThis->m_bServerOk = TRUE;
		}
	}

	pThis->m_bThreadExecAlive = FALSE;

#if 1
	// 行位置読み込みスレッド開始
	pThis->BeginSeqCtrlSetLinePosThread();
#endif

//	_endthreadex(0);

	return 0;
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 行位置読み込みスレッド開始

@param	なし
@retval なし

*/
/*============================================================================*/
void CSequentialCtrlRegister::BeginSeqCtrlSetLinePosThread()
{
	// 行位置読み込みスレッド
	if (m_bThreadGetLinePosAlive == FALSE)
	{
		if (m_hThreadGetLinePos != NULL)
		{
			// スレッドハンドラ解放
			WaitForSingleObject(m_hThreadGetLinePos, INFINITE);
			CloseHandle(m_hThreadGetLinePos);
			m_hThreadGetLinePos = NULL;
		}

		UINT uiThreadId1;
		m_hThreadGetLinePos = (HANDLE)_beginthreadex(
			NULL,				// SECURITY_ATTRIBUTES 構造体へのポインタ
			0,					// 新しいスレッドのスタック サイズ
			SeqCtrlGetLinePosThread,	// 新しいスレッドの実行を開始するルーチンの開始アドレス
			this,				// スレッドパラメタ
			CREATE_SUSPENDED,	// 初期状態フラグ
			&uiThreadId1);		// スレッド識別子を受け取る 32 ビット変数へのポインタ

		if (m_hThreadGetLinePos != NULL)
		{
			m_bThreadGetLinePosAlive = TRUE;

			ResumeThread(m_hThreadGetLinePos);
		}
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 逐次制御行番号セットスレッド ★★★

@param	なし
@retval なし

*/
/*============================================================================*/
UINT WINAPI CSequentialCtrlRegister::SeqCtrlSetLinePosThread(LPVOID pParam)
{
	CSequentialCtrlRegister* pThis = reinterpret_cast<CSequentialCtrlRegister*>(pParam);
	_ASSERTE(pThis != NULL);

	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: SeqCtrlSetLinePosThread (%d)"), __FILEW__, __LINE__, pThis->m_nPosEditCtrl + 1);
		CLogTraceEx::Write(_T(""), _T("CSequentialCtrlRegister"), _T("SeqCtrlSetLinePosThread"), _T("debug"), strDebug, nLogEx::debug);
	}

	CString strCommand;
	strCommand.Format(_T("goto %d"), pThis->m_nPosEditCtrl + 1);
	if (pThis->ExecCommand(strCommand) == FALSE)
	{
		if (pThis->GetSafeHwnd() == NULL)
			return 0;
		pThis->PostMessageW(eMessage_WarnSeqCtrlSelLinePos, 0, 0);
	}

	pThis->m_bThreadSetLinePosAlive = FALSE;
//	_endthreadex(0);

	return 0;
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 逐次制御ポーズスレッド ★★★

@param	なし
@retval なし

*/
/*============================================================================*/
UINT WINAPI CSequentialCtrlRegister::SeqCtrlPauseThread(LPVOID pParam)
{
	CSequentialCtrlRegister* pThis = reinterpret_cast<CSequentialCtrlRegister*>(pParam);
	_ASSERTE(pThis != NULL);

	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: SeqCtrlPauseThread (pause P)"), __FILEW__, __LINE__);
		CLogTraceEx::Write(_T(""), _T("CSequentialCtrlRegister"), _T("SeqCtrlPauseThread"), _T("debug"), strDebug, nLogEx::debug);
	}

	CString strCommand;
	strCommand.Format(_T("pause P"));
	if (pThis->ExecCommand(strCommand) == FALSE)
	{
		if (pThis->GetSafeHwnd() == NULL)
			return 0;
		pThis->PostMessageW(eMessage_WarnSeqCtrlPause, 0, 0);
		pThis->m_bThreadPauseAlive = FALSE;
		return 0;
	}

#if 1

	char szPause, szPause2;
	do
	{
		// ポーズファイルを見て引数を決める
		if (pThis->GetSafeHwnd() == NULL)
			return 0;
		szPause = ReadPauseFile(pParam);
		Sleep(50);
		if (pThis->GetSafeHwnd() == NULL)
			return 0;
		szPause2 = ReadPauseFile(pParam);
		Sleep(50);

		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: SeqCtrlExecThread (%c, %c)"), __FILEW__, __LINE__, szPause, szPause2);
			CLogTraceEx::Write(_T(""), _T("CSequentialCtrlRegister"), _T("SeqCtrlExecThread"), _T("debug"), strDebug, nLogEx::debug);
		}


	} while (szPause != 'P' || szPause2 != 'P');

#endif
	
	pThis->PostMessageW(eMessage_PauseCompltete, 0, 0);
	pThis->m_bThreadPauseAlive = FALSE;
//	_endthreadex(0);

	return 0;
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 逐次制御中断スレッド ★★★

@param	なし
@retval なし

*/
/*============================================================================*/
UINT WINAPI CSequentialCtrlRegister::SeqCtrlEndThread(LPVOID pParam)
{
	CSequentialCtrlRegister* pThis = reinterpret_cast<CSequentialCtrlRegister*>(pParam);
	_ASSERTE(pThis != NULL);

	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: SeqCtrlEndThread (end)"), __FILEW__, __LINE__);
		CLogTraceEx::Write(_T(""), _T("CSequentialCtrlRegister"), _T("SeqCtrlEndThread"), _T("debug"), strDebug, nLogEx::debug);
	}

	CString strCommand;
	strCommand.Format(_T("end"));
	if (pThis->ExecCommand(strCommand) == FALSE)
	{
		if (pThis->GetSafeHwnd() == NULL)
			return 0;
		pThis->PostMessageW(eMessage_WarnSeqCtrlEnd, 0, 0);
	}

	if (pThis->GetSafeHwnd() == NULL)
		return 0;

	pThis->m_bThreadEndAlive = FALSE;

//	_endthreadex(0);

	return 0;
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 逐次制御行位置読み込みスレッド ★★★

@param	なし
@retval なし

*/
/*============================================================================*/
UINT WINAPI CSequentialCtrlRegister::SeqCtrlGetLinePosThread(LPVOID pParam)
{
	CSequentialCtrlRegister* pThis = reinterpret_cast<CSequentialCtrlRegister*>(pParam);
	_ASSERTE(pThis != NULL);

	if (pThis->GetSafeHwnd() == NULL)
		return 0;

	int newPos1, newPos2, pos;

	while (pThis->m_bThreadGetLinePosAlive)
	{
		if (pThis->GetSafeHwnd() == NULL)
			return 0;

		// serialcont.EXECファイルの定期読み込み
		pos = ReadLinePosFile(pParam);
		if (pos > 0)
			newPos1 = pos - 1;
		else
			newPos1 = pos;

		Sleep(50);		// サーバ周期より短い50msで更新

		if (pThis->GetSafeHwnd() == NULL)
			return 0;

		pos = ReadLinePosFile(pParam);
		if (pos > 0)
			newPos2 = pos - 1;
		else
			newPos2 = pos;

//		{
//			CString strDebug;
//			strDebug.Format(_T("%s(%d) :: SeqCtrlGetLinePosThread (polling pos1 %d, pos2 %d)"), __FILEW__, __LINE__, newPos1, newPos2);
//			CLogTraceEx::Write(_T(""), _T("CSequentialCtrlRegister"), _T("SeqCtrlGetLinePosThread"), _T("debug"), strDebug, nLogEx::debug);
//		}

		// 2回読んで同じだったらファイル書き込み済みと判断し、行位置を更新
//		if (pThis->m_bThreadExecAlive == FALSE &&
#if 0
		if (newPos1 != -1 && newPos2 != -1 &&
			newPos1 == newPos2 &&
			newPos1 != pThis->m_newPos)
#else
		if (newPos1 != -1 && newPos2 != -1 && pos != 0 && 
			newPos1 == newPos2 &&
			(newPos1 != pThis->m_newPos || (pos == 1 && pThis->m_bRunning == TRUE)))
#endif
		{
			{
				CString strDebug;
				strDebug.Format(_T("%s(%d) :: SeqCtrlGetLinePosThread (new pos %d)"), __FILEW__, __LINE__, newPos1);
				CLogTraceEx::Write(_T(""), _T("CSequentialCtrlRegister"), _T("SeqCtrlGetLinePosThread"), _T("debug"), strDebug, nLogEx::debug);
			}

			pThis->PostMessageW(eMessage_UpdateSeqCtrlPos, newPos1, 0);
		}

		Sleep(50);		// サーバ周期より短い50msで更新
	}

//	_endthreadex(0);

	return 0;
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# 行番号ファイル読み込み

@param	なし
@retval なし

*/
/*============================================================================*/
int CSequentialCtrlRegister::ReadLinePosFile(LPVOID pParam)
{
	CSequentialCtrlRegister* pThis = reinterpret_cast<CSequentialCtrlRegister*>(pParam);
	_ASSERTE(pThis != NULL);

	if (!pThis->m_bServerOk)
		return -1;

	FILE* fp;
	CString strFolderPath = theApp.GetShareFilePath(eFileType_SeqCtrlTmp_Folder, theApp.GetSelectStation());
	CString strFilePath;
	strFilePath.Format(_T("%s\\%s"), (LPCTSTR)strFolderPath, SEQCTRL_LINEPOS_FILE);

	TRY
	{
		if (_wfopen_s(&fp, strFilePath, _T("r")) != 0 || fp == NULL)
		{
			return -1;
		}

		char szline[2048];
		CString cbuf;
		while (!feof(fp))
		{
			/* １行読み込み */
			if (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) == NULL)
			{
				break;
			}
			else
			{
				fclose(fp);

				int nline = atoi(szline);

				{
					if (nline == SEQCTRL_END_LINE)
					{
						CString strInfo;
						strInfo.Format(_T("%s(%d) :: ReadLinePosFile (nline %d)"), __FILEW__, __LINE__, nline);
						CLogTraceEx::Write(_T(""), _T("CSequentialCtrlRegister"), _T("ReadLinePosFile"), _T("info"), strInfo, nLogEx::info);
					}
				}

//			if (nline == 0)
//			{
//				nline = 1;
//			}
				return nline;
			}
		}
	} CATCH_ALL(e){
	}
	END_CATCH_ALL
		if (fp != NULL)
			fclose(fp);

	return -1;
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# ポーズファイル読み込み

@param	なし
@retval なし

*/
/*============================================================================*/
char CSequentialCtrlRegister::ReadPauseFile(LPVOID pParam)
{
	CSequentialCtrlRegister* pThis = reinterpret_cast<CSequentialCtrlRegister*>(pParam);
	_ASSERTE(pThis != NULL);

	char szline[2048];
	memset(szline, 0, 2048);

	if (!pThis->m_bServerOk)
		return szline[0];

	FILE* fp;
	CString strFolderPath = theApp.GetShareFilePath(eFileType_SeqCtrlTmp_Folder, theApp.GetSelectStation());
	CString strFilePath;
	strFilePath.Format(_T("%s\\%s"), (LPCTSTR)strFolderPath, SEQCTRL_PAUSE_FILE);

	TRY
	{
		if (_wfopen_s(&fp, strFilePath, _T("r")) != 0 || fp == NULL)
		{
			return szline[0];
		}

		CString cbuf;
		while (!feof(fp))
		{
			/* １行読み込み */
			if (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) == NULL)
			{
				break;
			}
			else
			{
				fclose(fp);
				return szline[0];
			}
		}
	} CATCH_ALL(e){
	}
	END_CATCH_ALL
		if (fp != NULL)
			fclose(fp);

	return szline[0];
}

/*============================================================================*/
/*! CSequentialCtrlRegister

-# スレッドを抜ける

@param
@retval

*/
/*============================================================================*/
void CSequentialCtrlRegister::ExitThread()
{
	// スレッド停止
	if (m_hThreadExec != NULL)
	{
		m_bThreadExecAlive = FALSE;
		WaitForSingleObject(m_hThreadExec, INFINITE);
		CloseHandle(m_hThreadExec);
		m_hThreadExec = NULL;
	}
	if (m_hThreadSetLinePos != NULL)
	{
		m_bThreadSetLinePosAlive = FALSE;
		WaitForSingleObject(m_hThreadSetLinePos, INFINITE);
		CloseHandle(m_hThreadSetLinePos);
		m_hThreadSetLinePos = NULL;
	}
	if (m_hThreadPause != NULL)
	{
		m_bThreadPauseAlive = FALSE;
		WaitForSingleObject(m_hThreadPause, INFINITE);
		CloseHandle(m_hThreadPause);
		m_hThreadPause = NULL;
	}
	if (m_hThreadEnd != NULL)
	{
		m_bThreadEndAlive = FALSE;
		WaitForSingleObject(m_hThreadEnd, INFINITE);
		CloseHandle(m_hThreadEnd);
		m_hThreadEnd = NULL;
	}
	if (m_hThreadGetLinePos != NULL)
	{
		m_bThreadGetLinePosAlive = FALSE;
		WaitForSingleObject(m_hThreadGetLinePos, INFINITE);
		CloseHandle(m_hThreadGetLinePos);
		m_hThreadGetLinePos = NULL;
	}
}

