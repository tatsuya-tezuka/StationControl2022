#include "stdafx.h"
#include "ShareLbwIf.h"

static const CString mShareMemoryString = _T("LbwShareMemory");
static const CString mShareMemoryMutex = _T("LbwShareMutex");
static const CString mShareMemorySend = _T("LbwShareSend");
static const CString mShareMemoryRecv = _T("LbwShareRecv");


CShareLbwIf::CShareLbwIf()
{
	mServer = true;
	m_hMap = NULL;
	m_hMutex = NULL;
	m_hSend = NULL;
	m_hRecv = NULL;
}


CShareLbwIf::~CShareLbwIf()
{
	Delete();
}

/*============================================================================*/
/*! ���L�������iLoopBand�j

-# ���L�������̍쐬

@param	bServer		true�̏ꍇ�T�[�o�[�Afalse�̏ꍇ�N���C�A���g

@retval
*/
/*============================================================================*/
bool CShareLbwIf::Create(bool bServer)
{
	if (m_hMap != NULL && mServer == bServer){
		return true;
	}

	if (m_hMap != NULL){
		Delete();
	}

	mServer = bServer;

	// �v���Z�XID�̎擾
	m_dwProcessId = ::GetCurrentProcessId();

	// ���L�������p�t�@�C���}�b�s���O�I�u�W�F�N�g�̍쐬
	m_hMap = ::CreateFileMapping((HANDLE)INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(stItem)*mItemMax, mShareMemoryString);

	if (mServer == true){
		if (GetLastError() == ERROR_ALREADY_EXISTS){
			//AfxMessageBox(_T("���ɃA���[���T�[�o�[�Ƃ��ċN������Ă��܂��B"));
			return false;
		}
	}
	else{
		if (GetLastError() == ERROR_ALREADY_EXISTS){
			// ���ɋ��L�������I�u�W�F�N�g���쐬����Ă���̂ŁA�N���C�A���g�Ƃ��Ď��s
			m_hMap = ::OpenFileMapping(FILE_MAP_WRITE, FALSE, mShareMemoryString);
//			m_hMutex = ::CreateMutex(NULL, FALSE, mShareMemoryMutex);
		}
		else{
			//AfxMessageBox(_T("�A���[���T�[�o�[���N������Ă��܂���B\n�����𒆎~���܂�"));
			return false;
		}
	}

	// �����܂ł���΃T�[�o�[���邢�̓N���C�A���g���ċN������Ă���

	// 
	// �Ăяo�����v���Z�X�̃A�h���X��ԂɁA�t�@�C���̃r���[���}�b�v���܂�
	if (m_hMap != NULL)
		m_pBuf = (char*)::MapViewOfFile(m_hMap, FILE_MAP_WRITE, 0, 0, sizeof(stItem)*mItemMax);
	else
		m_pBuf = NULL;

	m_hSend = ::CreateEvent(NULL, FALSE, FALSE, mShareMemorySend);
	m_hRecv = ::CreateEvent(NULL, FALSE, FALSE, mShareMemoryRecv);

	return true;
}

/*============================================================================*/
/*! ���L�������iLoopBand�j

-# �A���[�����L�������̍폜

@param	bServer		true�̏ꍇ�T�[�o�[�Afalse�̏ꍇ�N���C�A���g

@retval
*/
/*============================================================================*/
void CShareLbwIf::Delete()
{
	::CloseHandle(m_hSend);
	::CloseHandle(m_hRecv);
	if (m_hMutex)
		::CloseHandle(m_hMutex);

	::UnmapViewOfFile(m_pBuf);
	::CloseHandle(m_hMap);

	m_hMap = NULL;
	m_hMutex = NULL;
	m_hSend = NULL;
	m_hRecv = NULL;
}

/*============================================================================*/
/*! ���L�������iLoopBand�j

-# �A���[�����L�������ւ̏�������

@param		stAlarmBuf		���L�������ɏ������ރf�[�^
@param		count			stItem�\���̂̃J�E���g

@retval
*/
/*============================================================================*/
void CShareLbwIf::Send(stItem* stBuf, UINT count)
{
	if (m_pBuf == NULL)
		return;

	// �T�[�o�[�Ƃ̔r������
//	if (::WaitForSingleObject(m_hMutex, 500) == WAIT_OBJECT_0)
	{
		// ���L�������o�b�t�@�ɏ�������
		memset(m_pBuf, 0, sizeof(stItem)*mItemMax);
		memcpy(m_pBuf, stBuf, sizeof(stItem)*count);

		// �������ݏI���C�x���g
		::SetEvent(m_hSend);
		TRACE("### Client Send Finish\n");

		// Wait for server to finish
		if (::WaitForSingleObject(m_hRecv, 500) == WAIT_OBJECT_0) {
			TRACE("### Server Receive Finish\n");
		}
		else {
			TRACE("### Server Receive Timeout\n");
		}
//		::ReleaseMutex(m_hMutex);
	}
}

/*============================================================================*/
/*! ���L�������iLoopBand�j

-# �A���[�����L����������̓ǂݍ��݁i��M�͑S�̈���擾����K�v������j

@param		stAlarmBuf		���L�������ɏ������ރf�[�^�i�ő�mItemMax�����j
@param		count			stItem�\���̂̃J�E���g

@retval
*/
/*============================================================================*/
bool CShareLbwIf::Recv(stItem* stBuf, UINT count)
{
	if (m_pBuf == NULL)
		return false;

	if (::WaitForSingleObject(m_hSend, 50) == WAIT_OBJECT_0) {
		// ���L�������o�b�t�@����ǂݍ���
		memset(stBuf, 0, sizeof(stItem)*count);
		memcpy(stBuf, &m_pBuf[0], sizeof(stItem)*count);
		::SetEvent(m_hRecv);
		TRACE("### Client Receive Finish\n");
		return true;
	}
	else{
		TRACE("### Client Send Timeout\n");
	}
	return false;
}



