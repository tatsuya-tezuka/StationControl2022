
// SharedAlarm.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��
#include "CmdInf.h"
#include "ShareAlarmIf.h"


// CSharedAlarmApp:
// ���̃N���X�̎����ɂ��ẮASharedAlarm.cpp ���Q�Ƃ��Ă��������B
//

class CSharedAlarmApp : public CWinApp
{
public:
	CSharedAlarmApp();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CCommandLineInfoEx	m_cmdInfo;
	bool				mDebug;
	CString				mAlarmFile;

	CShareAlarmIf		mShareAlarmIf;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	bool	GetDebugMode() { return mDebug; }
	CString	GetAlarmFile() { return mAlarmFile; }

	bool	WriteShareMemory();

protected:

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CSharedAlarmApp theApp;