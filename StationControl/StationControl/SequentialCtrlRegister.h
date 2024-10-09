#pragma once

#include "ListBoxEx.h"

// CSequentialCtrlRegister �_�C�A���O

class CSequentialCtrlRegister : public CDialogBase
{
	DECLARE_DYNAMIC(CSequentialCtrlRegister)

public:
	CSequentialCtrlRegister(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSequentialCtrlRegister();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_SEQUENTIALCTRLREGISTER };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString			m_strFilePath;					// �t�@�C���p�X
	CString			m_strFileName;					// �t�@�C����
	CString			m_strComment;					// �R�����g
	CListBoxEx		m_listEqCtrl;					// �ݔ����䃊�X�g�{�b�N�X
	CButton			m_btnRun;						// ���s�{�^��
	CButton			m_btnPause;						// �ꎞ��~�{�^��
	CButton			m_btnInt;						// ���f�{�^��
	CButton			m_chkStepExec;					// �X�e�b�v���s�`�F�b�N�{�b�N�X
	CButton			m_btnWaitSec;					// �w��b�ҋ@�}���{�^��
	CButton			m_btnPause2;					// �ꎞ��~�}���{�^��
	CButton			m_btnComman;					// �R�}���h�}���{�^��
	CButton			m_btnComment;					// �R�����g�}���{�^��
	CButton			m_btn1UpLine;					// ��s��փ{�^��
	CButton			m_btn1DownLine;					// ��s���փ{�^��
	CButton			m_btn1DelLine;					// ��s�폜�{�^��
	CButton			m_btnEdit;						// �ҏW�{�^��
	CButton			m_btnCtrlRegist;				// ����o�^�{�^��
//	CEdit			m_editCtrl;						// �G�f�B�b�g�R���g���[��
	int				m_nPosEditCtrl;					// �G�f�B�b�g�R���g���[���ʒu
	BOOL			m_bRunning;						// ���s���t���O
	BOOL			m_bPause;						// �ꎞ��~�t���O
	BOOL			m_bModified;					// �X�V�t���O
	int				m_scrollPos;					// �X�N���[���ʒu 

	HANDLE			m_hThreadExec;					// ���s�X���b�h�n���h��
	HANDLE			m_hThreadSetLinePos;			// �s�Z�b�g�X���b�h�n���h��
	HANDLE			m_hThreadPause;					// �|�[�Y�X���b�h�n���h��
	HANDLE			m_hThreadEnd;					// ���f�X���b�h�n���h��
	HANDLE			m_hThreadGetLinePos;			// �s�ʒu�ǂݍ��݃X���b�h�n���h��
	BOOL			m_bThreadExecAlive;				// ���s�X���b�h����
	BOOL			m_bThreadSetLinePosAlive;		// ���s�X���b�h����
	BOOL			m_bThreadPauseAlive;			// �|�[�Y�X���b�h����
	BOOL			m_bThreadEndAlive;				// ���f�X���b�h����
	BOOL			m_bThreadGetLinePosAlive;		// �s�ʒu�ǂݍ��݃X���b�h����
	int				m_newPos;						// �V�����s�ʒu
	BOOL			m_bFirstExec;					// ������s
	BOOL			m_bServerOk;					// �T�[�oOK

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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
