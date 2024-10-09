/*============================================================================*/
/*! SequentialCtrlRegister.cpp

-# ��������o�^
*/
/*============================================================================*/
// SequentialCtrlRegister.cpp : �����t�@�C��
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

// CSequentialCtrlRegister �_�C�A���O

IMPLEMENT_DYNAMIC(CSequentialCtrlRegister, CDialogBase)

/*============================================================================*/
/*! CSequentialCtrlRegister

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
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

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
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


// CSequentialCtrlRegister ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! CSequentialCtrlRegister

-# �t�@�C�����j���[�̕��邩��Ăяo�����

@param	�Ȃ�
@retval	�Ȃ�

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

-# �t�@�C�����j���[�̊J������Ăяo�����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnMenuOpen()
{
	// �G�f�B�b�g�{�b�N�X�L�����Z��
//	CancelSeqCtrl();
	UpdateData(TRUE);

	// �t�@�C�����J��
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

-# �t�@�C�����j���[�̐V�K�쐬����Ăяo�����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnMenuNew()
{
	m_strFileName = _T("");
	m_strFilePath = _T("");
	m_strComment = _T("");
//	// �G�f�B�b�g�{�b�N�X�L�����Z��
//	CancelSeqCtrl();
	// ���X�g�{�b�N�X�N���A
	m_listEqCtrl.ResetContent();
	// �ҏW�\��Ԃɂ���
	SetBtnEditable();
	UpdateData(FALSE);
}

/*============================================================================*/
/*! CSequentialCtrlRegister

-# �t�@�C�����j���[�̏㏑���ۑ�����Ăяo�����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnMenuSave()
{
	(void)Save();
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# �w��t�@�C���p�X�̃t�@�C����ǂݍ���

@param	filePath�F�t�@�C���p�X
@retval	�Ȃ�

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
			// �t�@�C���I�[�v�����s
			MessageBox(CString(_T("���������`")) + CString(szMsg), szTitle, MB_OK | MB_ICONWARNING);
			return;
		}
	
		char szline[2048];
		CString strline;
		CString cbuf;
	
		// ���X�g�{�b�N�X�N���A
		m_listEqCtrl.ResetContent();
		int nMaxLen = 0;
		CDC* memDC = m_listEqCtrl.GetDC();
		int nCount = 1;
		while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL)
		{
			// �ǂݍ��񂾕���������X�g�ɒǉ�
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

	// ���X�g�{�b�N�X�Ƀt�H�[�J�X�𓖂Ă�
	m_listEqCtrl.SetFocus();
	// ���X�g�{�b�N�X����łȂ��ꍇ
	if (m_listEqCtrl.GetCount() > 0)
	{
		// ��ԏ�̍s�ɃJ�[�\���Z�b�g
		m_listEqCtrl.SetCurSel(0);
		// ���s�ҏW�\��ԂɃ{�^�����Z�b�g
		SetBtnRunEditable();
	}
	SetModified(FALSE);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# �w��t�@�C���p�X�̃t�@�C���ɕۑ�����

@param	filePath�F�t�@�C���p�X
@retval	�Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::WriteFile(CString& filePath)
{
	FILE *fp;

	TRY
	{

		if (_wfopen_s(&fp, filePath, _T("wb")) != 0 || fp == NULL)
		{
			// �t�@�C���I�[�v�����s
			TCHAR szTitle[256];
			TCHAR szMsg[256];
			memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
			memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
			if (!LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_SAVE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
				return;
			if (!LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_DOSENOT_SAVE, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
				return;
			MessageBox(CString(_T("���������`")) + CString(szMsg), szTitle, MB_OK | MB_ICONWARNING);
			return;
		}

		// ���X�g�{�b�N�X���當����擾���A�t�@�C���ɏ�������ł���
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
		// �t�@�C���������ݎ��s
		TCHAR szTitle[256];
		TCHAR szMsg[256];
		memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
		memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
		if (LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_SAVE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])) && 
			LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_DOSENOT_SAVE, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
			MessageBox(CString(_T("���������`")) + CString(szMsg), szTitle, MB_OK | MB_ICONWARNING);
	}
	END_CATCH_ALL

	if (fp != NULL)
		fclose(fp);

	SetModified(FALSE);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# �w��t�@�C���p�X�̃t�@�C������R�����g��ǂݍ���

@param	filePath�F�t�@�C���p�X
@param	comment�F�R�����g
@retval	�Ȃ�

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
			// �t�@�C���I�[�v�����s
			MessageBox(CString(_T("�R�����g")) + CString(szMsg), szTitle, MB_OK | MB_ICONWARNING);
			return;
		}

		char szline[2048];
		CString cbuf;

		if (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL)
		{
			// �ǂݍ��񂾕���������X�g�ɒǉ�
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

-# �w��t�@�C���p�X�̃t�@�C���ɃR�����g��ۑ�����

@param	filePath�F�t�@�C���p�X
@param	comment�F�R�����g
@retval	�Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::WriteComment(CString& filePath, CString& comment)
{
	FILE *fp;

	TRY
	{

		if (_wfopen_s(&fp, filePath, _T("wb")) != 0 || fp == NULL)
		{
			// �t�@�C���I�[�v�����s
			TCHAR szTitle[256];
			TCHAR szMsg[256];
			memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
			memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
			if (!LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_SAVE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
				return;
			if (!LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_DOSENOT_SAVE, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
				return;
			MessageBox(CString(_T("�R�����g")) + CString(szMsg), szTitle, MB_OK | MB_ICONWARNING);
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
		// �t�@�C���������ݎ��s
		TCHAR szTitle[256];
		TCHAR szMsg[256];
		memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));
		memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
		if (LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_SAVE, szTitle, sizeof(szTitle) / sizeof(szTitle[0])) &&
			LoadString(GetModuleHandle(NULL), IDS_SEQCTRLREG_FILE_DOSENOT_SAVE, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
			MessageBox(CString(_T("�R�����g")) + CString(szMsg), szTitle, MB_OK | MB_ICONWARNING);
	}
	END_CATCH_ALL

	if (fp != NULL)
		fclose(fp);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# ���s�{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregRunButton()
#if 0
{
	UpdateData(TRUE);

	// �^�C�}�[�N���A
	KillTimer(1);

	if (!m_bPause && !m_bRunning)
	{
		// �ꎞ��~���܂��͎��s���łȂ��ꍇ�́A��ԍŏ��̍s�ɃJ�[�\���Z�b�g
		m_listEqCtrl.SetCurSel(0);
		UpdateData(FALSE);
	}

	// ���s���ɃZ�b�g
	m_bRunning = TRUE;
	m_bPause = FALSE;
	// �R�}���h�����s����
	ExecCommand();
	// �X�e�b�v���s�̏ꍇ�́A�R�}���h�����s
	if (m_chkStepExec.GetCheck())
	{
		GoNextListItem();
		// �{�^�������s�\�ɃZ�b�g
		SetBtnRunable();
	}
	// �X�e�b�v���s�łȂ��ꍇ�́A�^�C�}�[
	else
	{
		// �{�^�������s���ɃZ�b�g
		SetBtnRunning();
		// �^�C�}�[�Z�b�g
		SetTimer(1, 1000, NULL);
	}
}
#else
{
	UpdateData(TRUE);

	// ��`�t�@�C���̍X�V�`�F�b�N
	if (GetModified())
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(CString(_T("��`�t�@�C�����X�V����Ă��܂��B���s�O�Ƀt�@�C���ۑ����Ă��������B")), title, MB_OK | MB_ICONWARNING);
		return;
	}

	// ��`�t�@�C���������邩�`�F�b�N
	if (m_strFileName.IsEmpty())
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(CString(_T("��`�t�@�C�����w�肳��Ă��܂���B")), title, MB_OK | MB_ICONWARNING);
		return;
	}

	// ��`�t�@�C�������݂��邩�`�F�b�N
	{
		CString strFilePath;
		strFilePath.Format(_T("%s%s%s"), (LPCTSTR)theApp.GetShareFilePath(eFileType_SeqCtrlDef_Folder, theApp.GetSelectStation()), (LPCTSTR)m_strFileName, TXT_EXT);

		HANDLE hFile;
		hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE){
			CString title = _T("");
			GetWindowText(title);
			MessageBox(CString(_T("�w�肵����`�t�@�C�������݂��܂���B")), title, MB_OK | MB_ICONWARNING);
			return;
		}
		CloseHandle(hFile);
	}

	m_newPos = m_listEqCtrl.GetCurSel();

	if (!m_bPause && !m_bRunning)
	{
		m_bFirstExec = TRUE;

		// �X���b�h�𒆒f����
		m_bThreadExecAlive = FALSE;
		m_bThreadSetLinePosAlive = FALSE;
		m_bThreadPauseAlive = FALSE;
		m_bThreadEndAlive = FALSE;
		m_bThreadGetLinePosAlive = FALSE;

		// �s�ԍ��̃t�@�C���������Ă���
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
	// �s�ʒu�ǂݍ��݃X���b�h
	if (m_bThreadGetLinePosAlive == FALSE)
	{
		UINT uiThreadId1;
		m_hThreadGetLinePos = (HANDLE)_beginthreadex(
			NULL,				// SECURITY_ATTRIBUTES �\���̂ւ̃|�C���^
			0,					// �V�����X���b�h�̃X�^�b�N �T�C�Y
			SeqCtrlGetLinePosThread,	// �V�����X���b�h�̎��s���J�n���郋�[�`���̊J�n�A�h���X
			this,				// �X���b�h�p�����^
			CREATE_SUSPENDED,	// ������ԃt���O
			&uiThreadId1);		// �X���b�h���ʎq���󂯎�� 32 �r�b�g�ϐ��ւ̃|�C���^

		if (m_hThreadGetLinePos != NULL)
		{
			m_bThreadGetLinePosAlive = TRUE;

			ResumeThread(m_hThreadGetLinePos);
		}
	}
#endif

	// ���s�X���b�h
	if (m_bThreadExecAlive == FALSE)
	{
		if (!m_bPause && !m_bRunning)
		{
			// �ꎞ��~������ю��s���łȂ��ꍇ�́A��ԍŏ��̍s�ɃJ�[�\���Z�b�g
			m_listEqCtrl.SetCurSel(0);
			UpdateData(FALSE);
		}

		if (m_hThreadExec != NULL)
		{
			// �X���b�h�n���h�����
			WaitForSingleObject(m_hThreadExec, INFINITE);
			CloseHandle(m_hThreadExec);
			m_hThreadExec = NULL;
		}

		UINT uiThreadId2;
		m_hThreadExec = (HANDLE)_beginthreadex(
			NULL,				// SECURITY_ATTRIBUTES �\���̂ւ̃|�C���^
			0,					// �V�����X���b�h�̃X�^�b�N �T�C�Y
			SeqCtrlExecThread,	// �V�����X���b�h�̎��s���J�n���郋�[�`���̊J�n�A�h���X
			this,				// �X���b�h�p�����^
			CREATE_SUSPENDED,	// ������ԃt���O
			&uiThreadId2);		// �X���b�h���ʎq���󂯎�� 32 �r�b�g�ϐ��ւ̃|�C���^

		if (m_hThreadExec != NULL)
		{
			m_bThreadExecAlive = TRUE;
			// ���s���ɃZ�b�g
			m_bRunning = TRUE;
			m_bPause = FALSE;
			// �X�e�b�v���s�̏ꍇ�́A�R�}���h�����s
			if (m_chkStepExec.GetCheck())
			{
//				// �{�^�������s�\�ɃZ�b�g
//				SetBtnRunable();
				// �{�^���������҂��ɃZ�b�g
				SetBtnProcWaitting();
			}
			// �X�e�b�v���s�łȂ��ꍇ
			else
			{
				// �{�^�������s���ɃZ�b�g
				SetBtnRunning();
			}

			ResumeThread(m_hThreadExec);
		}
	}
}
#endif

/*============================================================================*/
/*! CSequentialCtrlRegister

-# �ꎞ��~�{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregPauseButton()
{
	UpdateData(TRUE);

	// �|�[�Y�X���b�h
	if (m_bThreadPauseAlive == FALSE)
	{
		if (m_hThreadPause != NULL)
		{
			// �X���b�h�n���h�����
			WaitForSingleObject(m_hThreadPause, INFINITE);
			CloseHandle(m_hThreadPause);
			m_hThreadPause = NULL;
		}

		UINT uiThreadId;
		m_hThreadPause = (HANDLE)_beginthreadex(
			NULL,				// SECURITY_ATTRIBUTES �\���̂ւ̃|�C���^
			0,					// �V�����X���b�h�̃X�^�b�N �T�C�Y
			SeqCtrlPauseThread,	// �V�����X���b�h�̎��s���J�n���郋�[�`���̊J�n�A�h���X
			this,				// �X���b�h�p�����^
			CREATE_SUSPENDED,	// ������ԃt���O
			&uiThreadId);		// �X���b�h���ʎq���󂯎�� 32 �r�b�g�ϐ��ւ̃|�C���^
	
		if (m_hThreadPause != NULL)
		{
			m_bThreadPauseAlive = TRUE;
//			// ��~
//			m_bRunning = FALSE;
//			m_bPause = TRUE;
//			// �{�^�������s�\�ɃZ�b�g
//			SetBtnRunable();
			// �{�^���������҂��ɃZ�b�g
			SetBtnProcWaitting();
	
			ResumeThread(m_hThreadPause);
		}
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# ���f�{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregInterruptButton()
{
	UpdateData(TRUE);

	// ���f�X���b�h
	if (m_bThreadEndAlive == FALSE)
	{
		if (m_hThreadEnd != NULL)
		{
			// �X���b�h�n���h�����
			WaitForSingleObject(m_hThreadEnd, INFINITE);
			CloseHandle(m_hThreadEnd);
			m_hThreadEnd = NULL;
		}

		UINT uiThreadId;
		m_hThreadEnd = (HANDLE)_beginthreadex(
			NULL,				// SECURITY_ATTRIBUTES �\���̂ւ̃|�C���^
			0,					// �V�����X���b�h�̃X�^�b�N �T�C�Y
			SeqCtrlEndThread,	// �V�����X���b�h�̎��s���J�n���郋�[�`���̊J�n�A�h���X
			this,				// �X���b�h�p�����^
			CREATE_SUSPENDED,	// ������ԃt���O
			&uiThreadId);		// �X���b�h���ʎq���󂯎�� 32 �r�b�g�ϐ��ւ̃|�C���^

		if (m_hThreadEnd != NULL)
		{
			m_bThreadEndAlive = TRUE;
//			// ���f
//			m_bRunning = FALSE;
//			m_bPause = FALSE;
//			m_listEqCtrl.SetCurSel(0);
//			// �{�^����ҏW�\�ɃZ�b�g
//			SetBtnRunEditable();
			// �{�^���������҂��ɃZ�b�g
			SetBtnProcWaitting();

			ResumeThread(m_hThreadEnd);
		}
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# �w��b�ҋ@�}���{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregWaitsecButton()
{
//	// �G�f�B�b�g�{�b�N�X�̓��e�o�^
//	RegistSeqCtrl();
	UpdateData(TRUE);

	// �ݒ�_�C�A���O�Ăяo��
	CSleepTimeInputDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetString();
		// �w��b�ҋ@�}��
		InsertSeqCtrl(str);
		// ���X�g�{�b�N�X����łȂ��ꍇ
		if (m_listEqCtrl.GetCount() > 0)
		{
			// ���s�ҏW�\��ԂɃ{�^�����Z�b�g
			SetBtnRunEditable();
		}
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# �ꎞ��~�}���{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregPause2Button()
{
//	// �G�f�B�b�g�{�b�N�X�̓��e�o�^
//	RegistSeqCtrl();
	UpdateData(TRUE);

	// �ݒ�_�C�A���O�Ăяo��
	CPauseMsgInputDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetString();
		// �ꎞ��~�}��
		InsertSeqCtrl(str);
		// ���X�g�{�b�N�X����łȂ��ꍇ
		if (m_listEqCtrl.GetCount() > 0)
		{
			// ���s�ҏW�\��ԂɃ{�^�����Z�b�g
			SetBtnRunEditable();
		}
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# �R�}���h�}���{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregCommandButton()
{
//	// �G�f�B�b�g�{�b�N�X�̓��e�o�^
//	RegistSeqCtrl();
	UpdateData(TRUE);

	// �ݒ�_�C�A���O�Ăяo��
	CCmdInputDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetString();
		// �R�}���h�}��
		InsertSeqCtrl(str);
		// ���X�g�{�b�N�X����łȂ��ꍇ
		if (m_listEqCtrl.GetCount() > 0)
		{
			// ���s�ҏW�\��ԂɃ{�^�����Z�b�g
			SetBtnRunEditable();
		}
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# �R�����g�}���{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregCommentButton()
{
//	// �G�f�B�b�g�{�b�N�X�̓��e�o�^
//	RegistSeqCtrl();
	UpdateData(TRUE);

	// �ݒ�_�C�A���O�Ăяo��
	CCommentInputDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetString();
		// �R�����g�}��
		InsertSeqCtrl(str);
		// ���X�g�{�b�N�X����łȂ��ꍇ
		if (m_listEqCtrl.GetCount() > 0)
		{
			// ���s�ҏW�\��ԂɃ{�^�����Z�b�g
			SetBtnRunEditable();
		}
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# ��s��փ{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregUpButton()
{
//	// �G�f�B�b�g�{�b�N�X�̓��e�o�^
//	RegistSeqCtrl();
	UpdateData(TRUE);

	// ���ݍs�ʒu���擾
	int sel = m_listEqCtrl.GetCurSel();

	// ���I���������͈�ԏ�̍s�̏ꍇ�͉������Ȃ�
	if (sel <= 0)
	{
		return;
	}

	// ��̍s�Ɠ���ւ���
	CString str;
	m_listEqCtrl.GetText(sel, str);
	m_listEqCtrl.DeleteString(sel);
	sel--;
	m_listEqCtrl.InsertString(sel, str);
	m_listEqCtrl.SetCurSel(sel);

	// �s�ԍ��U��Ȃ���
	AddLineNumber();

	// ���X�g�{�b�N�X����łȂ��ꍇ
	if (m_listEqCtrl.GetCount() > 0)
	{
		// �J�[�\���Z�b�g
		m_listEqCtrl.SetCurSel(sel);
		// ���s�ҏW�\��ԂɃ{�^�����Z�b�g
		SetBtnRunEditable();
	}

	SetModified(TRUE);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# ��s���փ{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregDownButton()
{
//	// �G�f�B�b�g�{�b�N�X�̓��e�o�^
//	RegistSeqCtrl();
	UpdateData(TRUE);

	// ���ݍs�ʒu���擾
	int sel = m_listEqCtrl.GetCurSel();

	// ���I���������͈�ԉ��̍s�̏ꍇ�͉������Ȃ�
	if (sel < 0 || sel >= m_listEqCtrl.GetCount() - 1)
	{
		return;
	}

	// ���̍s�Ɠ���ւ���
	CString str;
	m_listEqCtrl.GetText(sel, str);
	m_listEqCtrl.DeleteString(sel);
	sel++;
	m_listEqCtrl.InsertString(sel, str);
	m_listEqCtrl.SetCurSel(sel);

	// �s�ԍ��U��Ȃ���
	AddLineNumber();

	// ���X�g�{�b�N�X����łȂ��ꍇ
	if (m_listEqCtrl.GetCount() > 0)
	{
		// �J�[�\���Z�b�g
		m_listEqCtrl.SetCurSel(sel);
		// ���s�ҏW�\��ԂɃ{�^�����Z�b�g
		SetBtnRunEditable();
	}
	SetModified(TRUE);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# ��폜�{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregDelButton()
{
//	// �G�f�B�b�g�{�b�N�X�̓��e�o�^
//	RegistSeqCtrl();
	UpdateData(TRUE);

	// ���ݍs�ʒu���擾
	int sel = m_listEqCtrl.GetCurSel();

	// ���X�g�{�b�N�X���I������Ă���ꍇ
	if (sel >= 0)
	{
		// ���ݍs��ҏW�s�ɂ���
		m_nPosEditCtrl = sel;
	}
	// ���X�g�{�b�N�X�̏�̃G�f�B�b�g�{�b�N�X���I������Ă���ꍇ
	else if (m_nPosEditCtrl >= 0)
	{
	}
	// �����I������Ă��Ȃ��ꍇ
	else
	{
		// ���X�g�̍ŏ��̍s��ҏW�s�ɂ���
		m_nPosEditCtrl = 0;
	}
	// �폜����
	m_listEqCtrl.DeleteString(m_nPosEditCtrl);
	// �s�ԍ��U��Ȃ���
	AddLineNumber();
	// ���X�g�̍Ō�������ׂ�
	int nLast = m_listEqCtrl.GetCount() - 1;
	if (nLast >= 0)
	{
		if (m_nPosEditCtrl >= nLast)
		{
			// �I���ʒu��ύX
			m_nPosEditCtrl = nLast;
		}
		// ���X�g��I�����Ȃ���
		m_listEqCtrl.SetCurSel(m_nPosEditCtrl);
	}
	SetModified(TRUE);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# �ҏW�{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregEditButton()
{
//	// �G�f�B�b�g�{�b�N�X�̓��e�o�^
//	RegistSeqCtrl();
	UpdateData(TRUE);
	// ���������ҏW
	EditSeqCtrl();
}

/*============================================================================*/
/*! CSequentialCtrlRegister

-# ���������ҏW�B
   �ҏW�����񂩂�A����o�^�A�w��b�ҋ@�A�ꎞ��~�A�R�}���h�A�R�����g�ɐU�蕪����

@param	�Ȃ�
@retval TRUE: �ҏW�Ɉڍs / FALSE: �ҏW�Ɉڍs���Ȃ�

*/
/*============================================================================*/
BOOL CSequentialCtrlRegister::EditSeqCtrl()
#if 0
{
	// ���ݍs�ʒu���擾
	int sel = m_listEqCtrl.GetCurSel();

	// ���X�g�{�b�N�X���I������Ă���ꍇ
	if (sel >= 0)
	{
		// ���ݍs��ҏW�s�ɂ���
		m_nPosEditCtrl = sel;
	}
	// ���X�g�{�b�N�X�̏�̃G�f�B�b�g�{�b�N�X���I������Ă���ꍇ
	else if (m_nPosEditCtrl >= 0)
	{
	}
	// �����I������Ă��Ȃ��ꍇ
	else
	{
		// ���X�g�̍ŏ��̍s��ҏW�s�ɂ���
		m_nPosEditCtrl = 0;
	}

	if (m_listEqCtrl.GetCount() <= 0)
	{
		// ��������̏ꍇ�́A�s1�ԍ������X�g�ɓ����
		m_listEqCtrl.InsertString(-1, _T("0001"));
	}

	CRect rect;
	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	m_listEqCtrl.GetItemRect(m_nPosEditCtrl, rect);

	// ���łɃG�f�B�b�g�R���g���[�����L��ꍇ�͍�蒼��
	HWND hWnd = m_editCtrl.GetSafeHwnd();
	if (hWnd != NULL)
	{
		m_editCtrl.DestroyWindow();
	}

	// �G�f�B�b�g�R���g���[������
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
	// ���ݍs�ʒu���擾
	int sel = m_listEqCtrl.GetCurSel();

	// ���X�g�{�b�N�X���I������Ă���ꍇ
	if (sel >= 0)
	{
		// ���ݍs��ҏW�s�ɂ���
		m_nPosEditCtrl = sel;
	}
	// �����I������Ă��Ȃ��ꍇ
	else
	{
		return FALSE;
	}

	// �I���s�擾
	CString strOld, strNew;
	m_listEqCtrl.GetText(m_nPosEditCtrl, strOld);
	// �s�ԍ����폜
	strOld.Delete(0, 6);
	strNew = _T("");

	// �����񂩂�_�C�A���O�U�蕪��
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
	// ����
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

	// ������폜
	m_listEqCtrl.DeleteString(m_nPosEditCtrl);
	// ������}��
	CString cbuf;
	cbuf.Format(_T("%04d: %s"), m_nPosEditCtrl, (LPCTSTR)strNew);
	m_listEqCtrl.InsertString(m_nPosEditCtrl, cbuf);
	// �s�ԍ��U��Ȃ���
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

-# ���������}���B

@param	�}��������
@retval TRUE: �ҏW�Ɉڍs / FALSE: �ҏW�Ɉڍs���Ȃ�

*/
/*============================================================================*/
BOOL CSequentialCtrlRegister::InsertSeqCtrl(CString insertStr /*= _T("")*/)
{
	// ���ݍs�ʒu���擾
	int sel = m_listEqCtrl.GetCurSel();

	// ���X�g�{�b�N�X���I������Ă���ꍇ
	if (sel >= 0)
	{
		// ���ݍs�̎��̍s��ҏW�s�ɂ���
		m_nPosEditCtrl = sel + 1;
	}
	// ���X�g�{�b�N�X�̏�̃G�f�B�b�g�{�b�N�X���I������Ă���ꍇ
	else if (m_nPosEditCtrl >= 0 && m_listEqCtrl.GetCount() > 0)
	{
		// �G�f�B�b�g�{�b�N�X�̎��̍s��ҏW�s�ɂ���
		m_nPosEditCtrl++;
	}
	// �����I������Ă��Ȃ��ꍇ
	else
	{
		// ���X�g�̍ŏ��̍s��ҏW�s�ɂ���
		m_nPosEditCtrl = 0;
	}

	// ������}��
	CString cbuf;
	cbuf.Format(_T("%04d: %s"), m_nPosEditCtrl, (LPCTSTR)insertStr);
	m_listEqCtrl.InsertString(m_nPosEditCtrl, cbuf);
	// �s�ԍ��U��Ȃ���
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
	// ���łɃG�f�B�b�g�R���g���[�����L��ꍇ�͍�蒼��
	HWND hWnd = m_editCtrl.GetSafeHwnd();
	if (hWnd != NULL)
	{
		m_editCtrl.DestroyWindow();
	}

	// �G�f�B�b�g�R���g���[������
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

-# �ҏW������������s��o�^�B

@param	�Ȃ�
@retval TRUE: �o�^���� / FALSE: �o�^���Ȃ�

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
		// �s�ԍ��U��Ȃ���
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

	// ���X�g�{�b�N�X����łȂ��ꍇ
	if (m_listEqCtrl.GetCount() > 0)
	{
		// ���s�ҏW�\��ԂɃ{�^�����Z�b�g
		SetBtnRunEditable();
	}

	return ret;
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# �ҏW������������s���L�����Z���B

@param	�Ȃ�
@retval TRUE: �o�^���� / FALSE: �o�^���Ȃ�

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

-# �^�C�}�C�x���g�ŌĂяo�����

@param	nIDEvent�F�C�x���gID
@retval	�Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnTimer(UINT_PTR nIDEvent)
{
	// �R�}���h�����s����
	ExecCommand();
	// ���̃��X�g�A�C�e���ɐi��
	GoNextListItem();

	CDialogBase::OnTimer(nIDEvent);
}
#endif

/*============================================================================*/
/*! CSequentialCtrlRegister

-# �_�C�A���O����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
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
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# �E�B���h�E�j�����ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

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

-# �{�^����ҏW�\��Ԃɂ���B

@param	�Ȃ�
@retval �Ȃ�

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

	// �t�@�C�����j���[��������
	CMenu* pMenu = GetMenu();
	if (pMenu != NULL)
	{
		pMenu->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
		DrawMenuBar();
	}

	// ����{�^��������
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(SC_CLOSE, MF_ENABLED);

	UpdateData(FALSE);
}

/*============================================================================*/
/*! CSequentialCtrlRegister

-# �{�^�������s�ҏW�\��Ԃɂ���B

@param	�Ȃ�
@retval �Ȃ�

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

	// �t�@�C�����j���[��������
	CMenu* pMenu = GetMenu();
	if (pMenu != NULL)
	{
		pMenu->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
		DrawMenuBar();
	}

	// ����{�^��������
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(SC_CLOSE, MF_ENABLED);

	UpdateData(FALSE);
}

/*============================================================================*/
/*! CSequentialCtrlRegister

-# �{�^�������s����Ԃɂ���B

@param	�Ȃ�
@retval �Ȃ�

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

	// �t�@�C�����j���[��񊈐���
	CMenu* pMenu = GetMenu();
	if (pMenu != NULL)
	{
		pMenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED);
		DrawMenuBar();
	}

	// ����{�^���񊈐���
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED);

	UpdateData(FALSE);
}

/*============================================================================*/
/*! CSequentialCtrlRegister

-# �{�^�������s�\��Ԃɂ���B

@param	�Ȃ�
@retval �Ȃ�

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

	// �t�@�C�����j���[��񊈐���
	CMenu* pMenu = GetMenu();
	if (pMenu != NULL)
	{
		pMenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED);
		DrawMenuBar();
	}

	// ����{�^���񊈐���
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED);

	UpdateData(FALSE);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# �{�^���������҂���Ԃɂ���B

@param	�Ȃ�
@retval �Ȃ�

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

	// �t�@�C�����j���[��񊈐���
	CMenu* pMenu = GetMenu();
	if (pMenu != NULL)
	{
		pMenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED);
		DrawMenuBar();
	}

	// ����{�^���񊈐���
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED);

	UpdateData(FALSE);
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# ���̃��X�g�A�C�e���ɐi�ށB

@param	�Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::GoNextListItem()
{
	int sel = m_listEqCtrl.GetCurSel();

	// ��ԉ��ɓ��B�̏ꍇ�A�^�C�}�[���~�߂�
	if (sel >= m_listEqCtrl.GetCount() - 1)
	{
		KillTimer(1);
	}

//	// �X�e�b�v���s�̏ꍇ�́A�ꎞ��~
//	if (m_chkStepExec.GetCheck() == 1)
//	{
//		OnBnClickedSeqctrlregPauseButton();
//	}

	// ���X�g�̍Ō�������ׂ�
	int nLast = m_listEqCtrl.GetCount() - 1;
	if (nLast >= 0)
	{
		if (sel >= nLast)
		{
			// �I���ʒu��ύX
//			sel = nLast;
			sel = 0;
			// �I��
			m_bRunning = FALSE;
			m_bPause = FALSE;
			// �{�^����ҏW�\�ɃZ�b�g
			SetBtnRunEditable();
		}
		else
		{
			// ���X�g��I�����Ȃ���
			sel++;
		}
		m_listEqCtrl.SetCurSel(sel);
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# �R�}���h�����s����B �����[�g�V�F�����Ăяo���B ������

@param	strParam:�R�}���h����������
@retval TRUE:���� / FALSE:���s

*/
/*============================================================================*/
BOOL CSequentialCtrlRegister::ExecCommand(CString strParam)
{
	// �R�}���h�������[�g�V�F���Ŏ��s����
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

	// �����V�F���s
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

-# ���X�g�{�b�N�X�I��ύX���ɌĂ΂��B

@param	�Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnLbnSelchangeSeqctrlregEquipmentctrlList()
{
	UpdateData(TRUE);

	int sel = m_listEqCtrl.GetCurSel();
	// ���X�g�̑I��ύX��
	if (sel >= 0 && sel != m_nPosEditCtrl)
	{
		m_nPosEditCtrl = sel;

		// �s�ύX�X���b�h
		if (m_bThreadSetLinePosAlive == FALSE)
		{
			if (m_hThreadSetLinePos != NULL)
			{
				// �X���b�h�n���h�����
				WaitForSingleObject(m_hThreadSetLinePos, INFINITE);
				CloseHandle(m_hThreadSetLinePos);
				m_hThreadSetLinePos = NULL;
			}

			UINT uiThreadId;
			m_hThreadSetLinePos = (HANDLE)_beginthreadex(
				NULL,				// SECURITY_ATTRIBUTES �\���̂ւ̃|�C���^
				0,					// �V�����X���b�h�̃X�^�b�N �T�C�Y
				SeqCtrlSetLinePosThread,	// �V�����X���b�h�̎��s���J�n���郋�[�`���̊J�n�A�h���X
				this,				// �X���b�h�p�����^
				CREATE_SUSPENDED,	// ������ԃt���O
				&uiThreadId);		// �X���b�h���ʎq���󂯎�� 32 �r�b�g�ϐ��ւ̃|�C���^

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

-# �E�B���h�E�v���V�[�W���B

@param	message�F���b�Z�[�W
@param	wParam�F�ǉ����
@param	lParam�F�ǉ����
@retval �Ȃ�

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
			// ��ʕ\���𒀎����䒆�f��Ԃɂ���
			UpdateDispSeqCtrlEnd();
		}
		else if (pos >= 0)
		{
			// �V�����s�ʒu���Z�b�g���čĕ`�悷��
			m_newPos = pos;
			m_listEqCtrl.SetCurSel(m_newPos);

			// �X�e�b�v���s�̏ꍇ�́A�R�}���h�����s
			if (m_chkStepExec.GetCheck())
			{
				// �X�e�b�v���s�̏ꍇ�́A���s�\��ԂɈڂ�
				m_bRunning = FALSE;
				m_bPause = TRUE;
				SetBtnRunable();
			}
			UpdateData(FALSE);
		}
	}
		break;

	case eMessage_PauseCompltete:
		// ��ʕ\���𒀎�����|�[�Y��Ԃɂ���
		m_bRunning = FALSE;
		m_bPause = TRUE;
		// �{�^�������s�\�ɃZ�b�g
		SetBtnRunable();
		break;

	case eMessage_WarnSeqCtrlSetDefFile:
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("���������`�t�@�C�����w��ł��܂���ł����B"), title, MB_OK | MB_ICONWARNING);
		// ��ʕ\���𒀎����䒆�f��Ԃɂ���
		UpdateDispSeqCtrlEnd();
	}
		break;

	case eMessage_WarnSeqCtrlExec:
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("������������s�ł��܂���ł����B"), title, MB_OK | MB_ICONWARNING);
		// ��ʕ\���𒀎����䒆�f��Ԃɂ���
		UpdateDispSeqCtrlEnd();
	}
		break;

	case eMessage_WarnSeqCtrlPause:
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("����������|�[�Y�ł��܂���ł����B"), title, MB_OK | MB_ICONWARNING);
		// ��ʕ\���𒀎����䒆�f��Ԃɂ���
		UpdateDispSeqCtrlEnd();
	}
		break;

	case eMessage_WarnSeqCtrlSelLinePos:
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("��������̍s�ʒu���w��ł��܂���ł����B"), title, MB_OK | MB_ICONWARNING);
		// ��ʕ\���𒀎����䒆�f��Ԃɂ���
		UpdateDispSeqCtrlEnd();
	}
		break;

	case eMessage_WarnSeqCtrlEnd:
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("����������I���ł��܂���ł����B"), title, MB_OK | MB_ICONWARNING);
		// ��ʕ\���𒀎����䒆�f��Ԃɂ���
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

-# ��ʕ\���𒀎����䒆�f��Ԃɂ���

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::UpdateDispSeqCtrlEnd()
{
	// �X���b�h�𒆒f����
	m_bThreadExecAlive = FALSE;
	m_bThreadSetLinePosAlive = FALSE;
	m_bThreadPauseAlive = FALSE;
	m_bThreadEndAlive = FALSE;
	m_bThreadGetLinePosAlive = FALSE;

	// ��ʕ\���𒀎����䒆�f��Ԃɂ���
	UpdateData(TRUE);
	// ���f
	m_bRunning = FALSE;
	m_bPause = FALSE;
	m_listEqCtrl.SetCurSel(0);
	// �{�^����ҏW�\�ɃZ�b�g
	SetBtnRunEditable();
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# ����{�^���������ɌĂяo�����

@param	�Ȃ�
@retval	�Ȃ�

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

-# ����m�F���ɌĂяo�����

@param	�Ȃ�
@retval	TRUE:����@/ FALSE:���Ȃ�

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

-# �ۑ����ɌĂяo�����

@param	�Ȃ�
@retval	TRUE:�ۑ������@/ FALSE:�ۑ����ĂȂ�

*/
/*============================================================================*/
BOOL CSequentialCtrlRegister::Save()
{
	// �G�f�B�b�g�{�b�N�X�̓��e�o�^
//	RegistSeqCtrl();
	UpdateData(TRUE);

	if (m_strFileName.CompareNoCase(_T("")) == 0)
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("���������`�t�@�C��������͂��Ă��������B"), title, MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	else
	{
		CString strFilePath;
		strFilePath.Format(_T("%s%s%s"), (LPCTSTR)theApp.GetShareFilePath(eFileType_SeqCtrlDef_Folder, theApp.GetSelectStation()), (LPCTSTR)m_strFileName, TXT_EXT);
		// �R�����g�ۑ�
		CSeqCtrlDefCommentDlg dlg;
		dlg.SetComment(m_strComment);
		if (dlg.DoModal() == IDOK)
		{
			// �R�����g�ۑ�
			m_strComment = dlg.GetComment();
			WriteComment(strFilePath + COMMENT_EXT, m_strComment);
		}
		// �t�@�C�����㏑���ۑ�
		WriteFile(strFilePath);
	}
	DrawMenuBar();

	return TRUE;
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# ���X�g�{�b�N�X�̍s�ԍ���U��Ȃ����B

@param	�Ȃ�
@retval �Ȃ�

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
		// �s�ԍ����폜
		strLine.Delete(0, 6);
		// �s�ԍ���U��Ȃ����ă��X�g�ɒǉ����Ă���
		cbuf.Format(_T("%04d: %s"), i + 1, (LPCTSTR)strLine);
		listNew.push_back(cbuf);
	}

	// ���X�g�{�b�N�X�N���A
	m_listEqCtrl.ResetContent();

	// ���X�g�{�b�N�X�ɒǉ����Ȃ���
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

-# ����o�^���ɌĂяo�����B

@param	�Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::OnBnClickedSeqctrlregCtrlregistButton()
{
	UpdateData(TRUE);

	CControlRegistDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetString();
		// �R�����g�}��
		InsertSeqCtrl(str);
		// ���X�g�{�b�N�X����łȂ��ꍇ
		if (m_listEqCtrl.GetCount() > 0)
		{
			// ���s�ҏW�\��ԂɃ{�^�����Z�b�g
			SetBtnRunEditable();
		}
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# ����������s�X���b�h ������

@param	�Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
UINT WINAPI CSequentialCtrlRegister::SeqCtrlExecThread(LPVOID pParam)
{
	CSequentialCtrlRegister* pThis = reinterpret_cast<CSequentialCtrlRegister*>(pParam);
	_ASSERTE(pThis != NULL);

	CString strCommand;

	// ����̏ꍇ�͒�`�t�@�C�����w��
	if (pThis->m_bFirstExec)
	{
		// �s�ԍ����P�I���W���ɂ���
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

	// �X�e�b�v���s
	if (pThis->m_chkStepExec.GetCheck())
	{
		char szPause, szPause2;
		do
		{
			// �|�[�Y�t�@�C�������Ĉ��������߂�
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
	// �ĊJ���s
	else if (!pThis->m_bFirstExec)
	{
		strCommandStep.Format(_T("pause 0"));
	}

	// �X�e�b�v���s�܂��͍ĊJ���s
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
	// �s�ʒu�ǂݍ��݃X���b�h�J�n
	pThis->BeginSeqCtrlSetLinePosThread();
#endif

//	_endthreadex(0);

	return 0;
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# �s�ʒu�ǂݍ��݃X���b�h�J�n

@param	�Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSequentialCtrlRegister::BeginSeqCtrlSetLinePosThread()
{
	// �s�ʒu�ǂݍ��݃X���b�h
	if (m_bThreadGetLinePosAlive == FALSE)
	{
		if (m_hThreadGetLinePos != NULL)
		{
			// �X���b�h�n���h�����
			WaitForSingleObject(m_hThreadGetLinePos, INFINITE);
			CloseHandle(m_hThreadGetLinePos);
			m_hThreadGetLinePos = NULL;
		}

		UINT uiThreadId1;
		m_hThreadGetLinePos = (HANDLE)_beginthreadex(
			NULL,				// SECURITY_ATTRIBUTES �\���̂ւ̃|�C���^
			0,					// �V�����X���b�h�̃X�^�b�N �T�C�Y
			SeqCtrlGetLinePosThread,	// �V�����X���b�h�̎��s���J�n���郋�[�`���̊J�n�A�h���X
			this,				// �X���b�h�p�����^
			CREATE_SUSPENDED,	// ������ԃt���O
			&uiThreadId1);		// �X���b�h���ʎq���󂯎�� 32 �r�b�g�ϐ��ւ̃|�C���^

		if (m_hThreadGetLinePos != NULL)
		{
			m_bThreadGetLinePosAlive = TRUE;

			ResumeThread(m_hThreadGetLinePos);
		}
	}
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# ��������s�ԍ��Z�b�g�X���b�h ������

@param	�Ȃ�
@retval �Ȃ�

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

-# ��������|�[�Y�X���b�h ������

@param	�Ȃ�
@retval �Ȃ�

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
		// �|�[�Y�t�@�C�������Ĉ��������߂�
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

-# �������䒆�f�X���b�h ������

@param	�Ȃ�
@retval �Ȃ�

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

-# ��������s�ʒu�ǂݍ��݃X���b�h ������

@param	�Ȃ�
@retval �Ȃ�

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

		// serialcont.EXEC�t�@�C���̒���ǂݍ���
		pos = ReadLinePosFile(pParam);
		if (pos > 0)
			newPos1 = pos - 1;
		else
			newPos1 = pos;

		Sleep(50);		// �T�[�o�������Z��50ms�ōX�V

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

		// 2��ǂ�œ�����������t�@�C���������ݍς݂Ɣ��f���A�s�ʒu���X�V
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

		Sleep(50);		// �T�[�o�������Z��50ms�ōX�V
	}

//	_endthreadex(0);

	return 0;
}


/*============================================================================*/
/*! CSequentialCtrlRegister

-# �s�ԍ��t�@�C���ǂݍ���

@param	�Ȃ�
@retval �Ȃ�

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
			/* �P�s�ǂݍ��� */
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

-# �|�[�Y�t�@�C���ǂݍ���

@param	�Ȃ�
@retval �Ȃ�

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
			/* �P�s�ǂݍ��� */
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

-# �X���b�h�𔲂���

@param
@retval

*/
/*============================================================================*/
void CSequentialCtrlRegister::ExitThread()
{
	// �X���b�h��~
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

