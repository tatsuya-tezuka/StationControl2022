
// SharedAlarm.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
//

#include "stdafx.h"
#include "SharedAlarm.h"
#include "SharedAlarmDlg.h"
#include "SelectServerClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSharedAlarmApp

BEGIN_MESSAGE_MAP(CSharedAlarmApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSharedAlarmApp �R���X�g���N�V����

CSharedAlarmApp::CSharedAlarmApp()
{
	// �ċN���}�l�[�W���[���T�|�[�g���܂�
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}


// �B��� CSharedAlarmApp �I�u�W�F�N�g�ł��B

CSharedAlarmApp theApp;


// CSharedAlarmApp ������

BOOL CSharedAlarmApp::InitInstance()
{
	// �A�v���P�[�V���� �}�j�t�F�X�g�� visual �X�^�C����L���ɂ��邽�߂ɁA
	// ComCtl32.dll Version 6 �ȍ~�̎g�p���w�肷��ꍇ�́A
	// Windows XP �� InitCommonControlsEx() ���K�v�ł��B�����Ȃ���΁A�E�B���h�E�쐬�͂��ׂĎ��s���܂��B
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// �A�v���P�[�V�����Ŏg�p���邷�ׂẴR���� �R���g���[�� �N���X���܂߂�ɂ́A
	// �����ݒ肵�܂��B
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	//============================================================================
	// �R�}���h���C���̉��
	ParseCommandLine(m_cmdInfo);
	mDebug = m_cmdInfo.mDebug;
	mAlarmFile = m_cmdInfo.mAlarmFile;

	// �_�C�A���O�ɃV�F�� �c���[ �r���[�܂��̓V�F�� ���X�g �r���[ �R���g���[����
	// �܂܂�Ă���ꍇ�ɃV�F�� �}�l�[�W���[���쐬���܂��B
	CShellManager *pShellManager = new CShellManager;

	// MFC �R���g���[���Ńe�[�}��L���ɂ��邽�߂ɁA"Windows �l�C�e�B�u" �̃r�W���A�� �}�l�[�W���[���A�N�e�B�u��
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// �W��������
	// �����̋@�\���g�킸�ɍŏI�I�Ȏ��s�\�t�@�C����
	// �T�C�Y���k���������ꍇ�́A�ȉ�����s�v�ȏ�����
	// ���[�`�����폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ��Ж��܂��͑g�D���Ȃǂ̓K�؂ȕ������
	// ���̕������ύX���Ă��������B
	SetRegistryKey(_T("�A�v���P�[�V���� �E�B�U�[�h�Ő������ꂽ���[�J�� �A�v���P�[�V����"));

	if (mDebug == 0){
		// �f�o�b�O���[�h�ł͂Ȃ��̂Ń_�C�A���O��\�����Ȃ��Ŏ��s
		WriteShareMemory();
		if (pShellManager != NULL)
		{
			delete pShellManager;
		}
		return FALSE;
	}

	CSelectServerClient dlgmode;
	if (dlgmode.DoModal() == IDCANCEL){
		if (pShellManager != NULL)
		{
			delete pShellManager;
		}
		return FALSE;
	}

	CSharedAlarmDlg dlg(dlgmode.m_nMode, m_cmdInfo.mAlarmFile);
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �_�C�A���O�� <OK> �ŏ����ꂽ���̃R�[�h��
		//  �L�q���Ă��������B
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �_�C�A���O�� <�L�����Z��> �ŏ����ꂽ���̃R�[�h��
		//  �L�q���Ă��������B
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "�x��: �_�C�A���O�̍쐬�Ɏ��s���܂����B�A�v���P�[�V�����͗\�������ɏI�����܂��B\n");
		TRACE(traceAppMsg, 0, "�x��: �_�C�A���O�� MFC �R���g���[�����g�p���Ă���ꍇ�A#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS ���w��ł��܂���B\n");
	}

	// ��ō쐬���ꂽ�V�F�� �}�l�[�W���[���폜���܂��B
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// �_�C�A���O�͕����܂����B�A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n���Ȃ���
	//  �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}

bool CSharedAlarmApp::WriteShareMemory()
{
	// �t�@�C���̓��e�����邩
	if (mAlarmFile.IsEmpty() == true){
		if (mDebug != 0){
			AfxMessageBox(_T("�A���[���t�@�C�����I������Ă��܂���"), MB_OK | MB_ICONERROR);
		}
		return false;
	}

	// �A���[���t�@�C���ǂݍ���
	vector<CShareAlarmIf::stAlarmItem> list;
	list.clear();
	int nStation = mShareAlarmIf.ConvertAlarmData(mAlarmFile, list);
	if (nStation < 0){
		if (mDebug != 0){
			AfxMessageBox(_T("�A���[���t�@�C���̓ǂݍ��݂Ɏ��s���܂���"), MB_OK | MB_ICONERROR);
		}
		return false;
	}

	// ���L����������
	bool ret = mShareAlarmIf.Create(false, nStation);
	if (ret == false){
		if (mDebug != 0){
			AfxMessageBox(_T("���L�������̍쐬�Ɏ��s���܂���"), MB_OK | MB_ICONERROR);
		}
		return false;
	}

	// �t�@�C���̓��e�����L�������ɏ�������
	mShareAlarmIf.Send((CShareAlarmIf::stAlarmItem*)&list.at(0), list.size());

	return true;
}
