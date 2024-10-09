/*============================================================================*/
/*! CSshShell.cpp

-# SSH�V�F�����s�@�\
*/
/*============================================================================*/
#include "stdafx.h"
#include "..\StationControl\StationControl.h"
#include "SshShell.h"

IMPLEMENT_DYNAMIC(CSshShell, CWnd)

/*============================================================================*/
/*! CSshShell

-# �R���X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSshShell::CSshShell()
{
}


/*============================================================================*/
/*! CSshShell

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
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
//-# �X���b�h�N��
//
//@param  pParam
//@retval 0�F����,
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
//-# �X���b�h�N��
//
//@param  �Ȃ�
//@retval �Ȃ�
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
//-# �X���b�h�N��
//
//@param  �Ȃ�
//@retval �Ȃ�
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

-# �T�[�o��SSH�ڑ����A�V�F�������s����

@param  stParam�FSSH�\����
@param	bWaitComplete�FTRUE:�V�F���I����҂i�f�t�H���g�j / FALSE:�V�F���I����҂��Ȃ�
@retval �V�F�����s����(0�F�����A���̑��F���s�j
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

	// �V�����v���Z�X�p�̍\���̂��N������
	STARTUPINFO    stStartUpInfo;
	// �����o�[�̑����͗\�񂳂�Ă��邩���������̂ŁA���ׂẴ����o�[��
	// 0 �ɐݒ肵�Ă���A�K�v�ȃ����o�[���������Z�b�g����
	memset(&stStartUpInfo, 0, sizeof(STARTUPINFO));
	stStartUpInfo.cb = sizeof(STARTUPINFO);			// �\���̂̃T�C�Yls
	stStartUpInfo.dwFlags = STARTF_USESHOWWINDOW;	// wShowWindow �����o�[���g��
	//	stStartUpInfo.wShowWindow = SW_SHOWDEFAULT;		// �f�t�H���g�̒l
	stStartUpInfo.wShowWindow = SW_HIDE;			// ��ʂ͔�\��

	// �V�����v���Z�X��ŏ����󂯎��v���Z�X�\����
	PROCESS_INFORMATION stProcessInfo;

	// �V�����v���Z�X�����ƁA�v���Z�X�̓v���C�}���X���b�h�����A
	// �A�v���P�[�V�������N������
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

	// �V�F���I����҂ꍇ
	if (bWaitComplete)
	{
		// �����[�g�V�F���I���܂őҋ@
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
	// �V�F���I����҂��Ȃ��ꍇ
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

-# �T�[�o��SSH�ڑ����A�V�F�������s����

@param  stParam�FSSH�\����
@retval �V�F�����s����(0�F�����A���̑��F���s�j
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

	// �V�����v���Z�X�p�̍\���̂��N������
	STARTUPINFO    stStartUpInfo;
	// �����o�[�̑����͗\�񂳂�Ă��邩���������̂ŁA���ׂẴ����o�[��
	// 0 �ɐݒ肵�Ă���A�K�v�ȃ����o�[���������Z�b�g����
	memset(&stStartUpInfo, 0, sizeof(STARTUPINFO));
	stStartUpInfo.cb = sizeof(STARTUPINFO);			// �\���̂̃T�C�Yls
	stStartUpInfo.dwFlags = STARTF_USESHOWWINDOW;	// wShowWindow �����o�[���g��
//	stStartUpInfo.wShowWindow = SW_SHOWDEFAULT;		// �f�t�H���g�̒l
	stStartUpInfo.wShowWindow = SW_HIDE;			// ��ʂ͔�\��

	// �V�����v���Z�X��ŏ����󂯎��v���Z�X�\����
	PROCESS_INFORMATION stProcessInfo;

	// �V�����v���Z�X�����ƁA�v���Z�X�̓v���C�}���X���b�h�����A
	// �A�v���P�[�V�������N������
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

	// �����[�g�V�F���I���܂őҋ@
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

