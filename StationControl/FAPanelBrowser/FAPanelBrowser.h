
// FAPanelBrowser.h : FAPanelBrowser �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��
#include "CmdInf.h"


// CFAPanelBrowserApp:
// ���̃N���X�̎����ɂ��ẮAFAPanelBrowser.cpp ���Q�Ƃ��Ă��������B
//

class CFAPanelBrowserApp : public CWinApp
{
public:
	CFAPanelBrowserApp();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CCommandLineInfoEx	m_cmdInfo;
	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString				mTitle;
	CString				mXmlFile;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString		GetTitle()	{ return mTitle; }
	CString		GetXmFile()	{ return mXmlFile; }
protected:
	/* ------------------------------------------------------------------------------------ */

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFAPanelBrowserApp theApp;
