#pragma once

#include "ListBoxEx.h"

// CSequentialCtrlRegister ダイアログ

class CSequentialCtrlRegister : public CDialogBase
{
	DECLARE_DYNAMIC(CSequentialCtrlRegister)

public:
	CSequentialCtrlRegister(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSequentialCtrlRegister();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_SEQUENTIALCTRLREGISTER };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString			m_strFilePath;					// ファイルパス
	CString			m_strFileName;					// ファイル名
	CString			m_strComment;					// コメント
	CListBoxEx		m_listEqCtrl;					// 設備制御リストボックス
	CButton			m_btnRun;						// 実行ボタン
	CButton			m_btnPause;						// 一時停止ボタン
	CButton			m_btnInt;						// 中断ボタン
	CButton			m_chkStepExec;					// ステップ実行チェックボックス
	CButton			m_btnWaitSec;					// 指定秒待機挿入ボタン
	CButton			m_btnPause2;					// 一時停止挿入ボタン
	CButton			m_btnComman;					// コマンド挿入ボタン
	CButton			m_btnComment;					// コメント挿入ボタン
	CButton			m_btn1UpLine;					// 一行上へボタン
	CButton			m_btn1DownLine;					// 一行下へボタン
	CButton			m_btn1DelLine;					// 一行削除ボタン
	CButton			m_btnEdit;						// 編集ボタン
	CButton			m_btnCtrlRegist;				// 制御登録ボタン
//	CEdit			m_editCtrl;						// エディットコントロール
	int				m_nPosEditCtrl;					// エディットコントロール位置
	BOOL			m_bRunning;						// 実行中フラグ
	BOOL			m_bPause;						// 一時停止フラグ
	BOOL			m_bModified;					// 更新フラグ
	int				m_scrollPos;					// スクロール位置 

	HANDLE			m_hThreadExec;					// 実行スレッドハンドラ
	HANDLE			m_hThreadSetLinePos;			// 行セットスレッドハンドラ
	HANDLE			m_hThreadPause;					// ポーズスレッドハンドラ
	HANDLE			m_hThreadEnd;					// 中断スレッドハンドラ
	HANDLE			m_hThreadGetLinePos;			// 行位置読み込みスレッドハンドラ
	BOOL			m_bThreadExecAlive;				// 実行スレッド生存
	BOOL			m_bThreadSetLinePosAlive;		// 実行スレッド生存
	BOOL			m_bThreadPauseAlive;			// ポーズスレッド生存
	BOOL			m_bThreadEndAlive;				// 中断スレッド生存
	BOOL			m_bThreadGetLinePosAlive;		// 行位置読み込みスレッド生存
	int				m_newPos;						// 新しい行位置
	BOOL			m_bFirstExec;					// 初回実行
	BOOL			m_bServerOk;					// サーバOK

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

	void ReadFile(CString& filePath);
	void WriteFile(CString& filePath);
	void ReadComment(CString& filePath, CString& comment);
	void WriteComment(CString& filePath, CString& comment);
	BOOL EditSeqCtrl();
	BOOL InsertSeqCtrl(CString insertStr = _T(""));
//	BOOL RegistSeqCtrl();
//	BOOL CancelSeqCtrl();
	void SetModified(BOOL modified)			{ m_bModified = modified; }
	BOOL GetModified()						{ return m_bModified; }

	void SetBtnEditable();
	void SetBtnRunEditable();
	void SetBtnRunning();
	void SetBtnRunable();
	void SetBtnProcWaitting();
	void GoNextListItem();
	BOOL ExecCommand(CString strParam);
	BOOL CheckClose();
	BOOL Save();
	void AddLineNumber();
	void UpdateDispSeqCtrlEnd();

	void BeginSeqCtrlSetLinePosThread();

public:
	static UINT WINAPI SeqCtrlExecThread(LPVOID pParam);
	static UINT WINAPI SeqCtrlSetLinePosThread(LPVOID pParam);
	static UINT WINAPI SeqCtrlGetLinePosThread(LPVOID pParam);
	static UINT WINAPI SeqCtrlPauseThread(LPVOID pParam);
	static UINT WINAPI SeqCtrlEndThread(LPVOID pParam);
	static int ReadLinePosFile(LPVOID pParam);
	static char ReadPauseFile(LPVOID pParam);

	void	ExitThread();

public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	afx_msg void OnDestroy();
	afx_msg void OnMenuClose();
	afx_msg void OnMenuOpen();
	afx_msg void OnMenuNew();
	afx_msg void OnMenuSave();
	afx_msg void OnMenuSaveAs();
	afx_msg void OnBnClickedSeqctrlregRunButton();
	afx_msg void OnBnClickedSeqctrlregPauseButton();
	afx_msg void OnBnClickedSeqctrlregInterruptButton();
	afx_msg void OnBnClickedSeqctrlregWaitsecButton();
	afx_msg void OnBnClickedSeqctrlregPause2Button();
	afx_msg void OnBnClickedSeqctrlregCommandButton();
	afx_msg void OnBnClickedSeqctrlregCommentButton();
	afx_msg void OnBnClickedSeqctrlregUpButton();
	afx_msg void OnBnClickedSeqctrlregDownButton();
	afx_msg void OnBnClickedSeqctrlregDelButton();
	afx_msg void OnBnClickedSeqctrlregEditButton();
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLbnSelchangeSeqctrlregEquipmentctrlList();
	afx_msg void OnClose();
	afx_msg void OnBnClickedSeqctrlregCtrlregistButton();
	/* ------------------------------------------------------------------------------------ */
};
