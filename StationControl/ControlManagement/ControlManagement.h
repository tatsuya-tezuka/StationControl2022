
// ControlManagement.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��
#include "..\Utility\FileAccess.h"

// TKSC,SSOC�z�X�g��
enum eHostName{
	eHost_sttn,		// �n���
	eHost_ssoc,		// ���͌���
	eHost_tksc,		// �}�g��
};
// ���z�X�g���͎��ۂ̃z�X�g���̐擪�T����
static const TCHAR* mHostNameString[]{
//	_T("uzopr"),//��
	_T("u6opr"),//�P�c64�Ǔ�
	_T("szopr"),
	_T("tzopr"),
};

using namespace std;

//=============================================================================
//	INI�t�@�C��
//=============================================================================
const CString SECTION_SYSTEM = _T("SYSTEM");
const CString KEY_FILESERVER_NAME = _T("FileServerName");	// �t�@�C���T�[�o�[����
const CString KEY_SSHUSER_NAME = _T("SshUserName");		// SSH���[�U����
const CString KEY_SSHSERVER_NAME = _T("SshServerName");	// SSH�T�[�o����
const CString KEY_SSHKEY_PATH = _T("SshKeyFilePath");	// SSH�L�[�t�@�C���p�X����
const CString KEY_SSHBATCH_PATH = _T("SshBatchPath");	// SSH�o�b�`�t�@�C���p�X����
#define SVSTART_SH				_T("start.sh")					// �T�[�o���V�F��
#define SEND_CRM_SH				_T("SEND_CRM.sh")				// ���䌠���Ǘ��t�@�C���]��


#define SSH_WAITTIME			10000							// SSH�҂�����(ms)

class CSshShell
{
public:
	CSshShell();
	~CSshShell();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// SSH�\����
	typedef struct
	{
		CString strBatchPath;			// �o�b�`�t�@�C���p�X
		CString strUserName;			// SSH���O�C�����[�U��
		CString strServerName;			// SSH�ڑ���T�[�o���܂���IP�A�h���X
		CString strKeyFilePath;			// �閧���t�@�C���p�X
		CString strRemShellName;		// ���s���郊���[�g�V�F����
		CString strRemShellParam;		// ���s���郊���[�g�V�F���R�}���h�ւ̈����B�����𕡐��L�q����ꍇ�A"�ł�����
	} structSshParam;

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	structSshParam	m_stSshParam;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	static int ExecShell(structSshParam& sshParam, BOOL bWaitComplete = TRUE);
};



// CControlManagementApp:
// ���̃N���X�̎����ɂ��ẮAControlManagement.cpp ���Q�Ƃ��Ă��������B
//

class CControlManagementApp : public CWinApp
{
public:
	CControlManagementApp();

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
protected:
	CString		m_strAppPath;						// �A�v���P�[�V�����p�X��
	CString		m_strAppInifileName;				// �A�v���P�[�V����INI�t�@�C����
	CString		m_strFileServerName;
	CString		m_strManagementPath;				// �Ǘ��p�p�X��
	CFileAccess	mFileAcs;
public:

	//-----------------------------------------------------------------------------
	//	�����o�֐�
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

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CControlManagementApp theApp;