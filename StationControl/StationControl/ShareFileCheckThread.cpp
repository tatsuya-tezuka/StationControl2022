// ShareFileCheckThread.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "ShareFileCheckThread.h"

// timeGetTime�g�p
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

// CShareFileCheckThread

IMPLEMENT_DYNCREATE(CShareFileCheckThread, CWinThread)

CShareFileCheckThread::CShareFileCheckThread()
: m_bStop(false)
{
}

CShareFileCheckThread::~CShareFileCheckThread()
{
}

BOOL CShareFileCheckThread::InitInstance()
{
	m_bAutoDelete = FALSE;

	return TRUE;
}

int CShareFileCheckThread::ExitInstance()
{
	// TODO:    �X���b�h���Ƃ̌㏈���������Ŏ��s���܂��B
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CShareFileCheckThread, CWinThread)
END_MESSAGE_MAP()


// CShareFileCheckThread ���b�Z�[�W �n���h���[


int CShareFileCheckThread::Run()
{
	// �ŏ��^�C�}����\��ݒ�(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();

	while (m_bStop == false){
		// ����J�n����̌o�ߎ��ԁi�~���b�j�̌v�Z
		DWORD dwElapsed = (timeGetTime() - dwStart);
		if (dwElapsed >= 1000){
			dwStart = timeGetTime();
			// �T�[�o�[�̋��L�t�H���_���f�[�^���擾����
			theApp.GetSatelliteData().GetShareFile();
		}
		else{
			Sleep(1);
			continue;
		}
	}

	return 0;
}
