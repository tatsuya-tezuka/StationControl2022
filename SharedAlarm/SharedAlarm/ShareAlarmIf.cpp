#include "stdafx.h"
#include "SharedAlarm.h"


CShareAlarmIf::CShareAlarmIf()
{
	mServer = true;
	m_hMap = NULL;
	m_hMutex = NULL;
	m_hSend = NULL;
	m_hRecv = NULL;
}

CShareAlarmIf::~CShareAlarmIf()
{
	Delete();
}

/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

-# �A���[�����L�������̍쐬

@param	bServer		true�̏ꍇ�T�[�o�[�Afalse�̏ꍇ�N���C�A���g
@param	nStation	�ǔԍ�

@retval
*/
/*============================================================================*/
bool CShareAlarmIf::Create(bool bServer, int nStation)
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

	// ���L��������
	CString strShareMemory, strShareMemoryMutex, strShareMemorySend, strShareMemoryRecv;
	strShareMemory.Format(_T("%s%d"), (LPCTSTR)mShareMemoryString, nStation);
	strShareMemoryMutex.Format(_T("%s%d"), (LPCTSTR)mShareMemoryMutex, nStation);
	strShareMemorySend.Format(_T("%s%d"), (LPCTSTR)mShareMemorySend, nStation);
	strShareMemoryRecv.Format(_T("%s%d"), (LPCTSTR)mShareMemoryRecv, nStation);

	if (mServer == true)
	{
		// �T�[�o�̏ꍇ
		// ���L�������p�t�@�C���}�b�s���O�I�u�W�F�N�g�̍쐬
		m_hMap = ::CreateFileMapping((HANDLE)INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(stAlarmItem)*mAlarmItemMax, strShareMemory);
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			if (theApp.GetDebugMode())
			{
				AfxMessageBox(_T("���ɃA���[���T�[�o�[�Ƃ��ċN������Ă��܂��B"));
			}
			if (m_hMap != NULL)
			{
				::CloseHandle(m_hMap);
				m_hMap = NULL;
			}
			return false;
		}
	}
	else{
		// �N���C�A���g�̏ꍇ
		// �T�[�o���쐬�ς݂̋��L���������J��
		m_hMap = ::OpenFileMapping(FILE_MAP_WRITE, FALSE, strShareMemory);
		if (m_hMap == NULL || GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			// �[���A�v���������Ă��鋤�L���������J���Ȃ�����
			if (theApp.GetDebugMode())
			{
				AfxMessageBox(_T("�A���[���T�[�o�[���N������Ă��܂���B\n���L���������J�������𒆎~���܂��B"));
			}
			if (m_hMap != NULL)
			{
				::CloseHandle(m_hMap);
				m_hMap = NULL;
			}
			return false;
		}
		m_hMutex = ::CreateMutex(NULL, FALSE, strShareMemoryMutex);
	}

	// �����܂ł���΃T�[�o�[���邢�̓N���C�A���g���ċN������Ă���

	// 
	// �Ăяo�����v���Z�X�̃A�h���X��ԂɁA�t�@�C���̃r���[���}�b�v���܂�
	if (m_hMap != NULL)
		m_pAlarmBuf = (char*)::MapViewOfFile(m_hMap, FILE_MAP_WRITE, 0, 0, sizeof(stAlarmItem)*mAlarmItemMax);
	else
		m_pAlarmBuf = NULL;

	m_hSend = ::CreateEvent(NULL, FALSE, FALSE, strShareMemorySend);
	m_hRecv = ::CreateEvent(NULL, FALSE, FALSE, strShareMemoryRecv);
	
	return true;
}

/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

-# �A���[�����L�������̍쐬

@param	bServer		true�̏ꍇ�T�[�o�[�Afalse�̏ꍇ�N���C�A���g

@retval
*/
/*============================================================================*/
void CShareAlarmIf::Delete()
{
	::CloseHandle(m_hSend);
	::CloseHandle(m_hRecv);
	if (m_hMutex)
		::CloseHandle(m_hMutex);

	::UnmapViewOfFile(m_pAlarmBuf);
	::CloseHandle(m_hMap);

	m_hMap = NULL;
	m_hMutex = NULL;
	m_hSend = NULL;
	m_hRecv = NULL;
}

/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

-# �A���[�����L�������ւ̏�������

@param		stAlarmBuf		���L�������ɏ������ރf�[�^
@param		count			stAlarmItem�\���̂̃J�E���g

@retval
*/
/*============================================================================*/
void CShareAlarmIf::Send(stAlarmItem* stAlarmBuf, UINT count)
{
	// �T�[�o�[�Ƃ̔r������
	if (::WaitForSingleObject(m_hMutex, 500) == WAIT_OBJECT_0){
		// ���L�������o�b�t�@�ɏ�������
		memset(&m_pAlarmBuf[0], 0, sizeof(stAlarmItem)*mAlarmItemMax);
		memcpy(&m_pAlarmBuf[0], stAlarmBuf, sizeof(stAlarmItem)*count);

		// �������ݏI���C�x���g
		::SetEvent(m_hSend);
		//TRACE("### Client Send Finish\n");

		// Wait for server to finish
		if (::WaitForSingleObject(m_hRecv, 500) == WAIT_OBJECT_0) {
			//TRACE("### Server Receive Finish\n");
		}
		else {
			//TRACE("### Server Receive Timeout\n");
		}
		::ReleaseMutex(m_hMutex);
	}
}

/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

-# �A���[�����L����������̓ǂݍ��݁i��M�͑S�̈���擾����K�v������j

@param		stAlarmBuf		���L�������ɏ������ރf�[�^�i�ő�mAlarmItemMax�����j
@param		count			stAlarmItem�\���̂̃J�E���g

@retval
*/
/*============================================================================*/
bool CShareAlarmIf::Recv(stAlarmItem* stAlarmBuf, UINT count)
{
	if (::WaitForSingleObject(m_hSend, 50) == WAIT_OBJECT_0) {
		// ���L�������o�b�t�@����ǂݍ���
		memset(stAlarmBuf, 0, sizeof(stAlarmItem)*count);
		memcpy(stAlarmBuf, &m_pAlarmBuf[0], sizeof(stAlarmItem)*count);
		::SetEvent(m_hRecv);
		//TRACE("### Client Receive Finish\n");
		return true;
	}
	else{
		//TRACE("### Client Send Timeout\n");
	}
	return false;
}

/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

-# �A���[���t�@�C�����\���̂ɕϊ�����

@param		strFileName		�A���[���t�@�C��
@param		list			�A���[���\����

@retval		int		�ǔԍ�
*/
/*============================================================================*/
int CShareAlarmIf::ConvertAlarmData(CString strFileName, vector<stAlarmItem>& list)
{
	int ret = -1;

	// �t�@�C��������ǔ���
	CString fname = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind(_T('\\')) - 1);
	for (int sta = 0; sta < sizeof(mStationFileString) / sizeof(mStationFileString[0]); sta++)
	{
		if (fname.Find(mStationFileString[sta]) == 0)
		{
			ret = sta;
			break;
		}
	}

	// �t�@�C���ǂݍ���
	FILE* fp;
	if (_wfopen_s(&fp, strFileName, _T("r")) != 0 || fp == NULL){
		return -1;
	}

	TRY
	{
		char    szline[2048];	    /* ���R�[�h�ǂݍ��݃o�b�t�@       */
		CString cbuf, str;
		stAlarmItem data;
		memset(&data, 0, sizeof(stAlarmItem));
		// �擪�̍X�V�������擾
		if (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) == NULL) {
			fclose(fp);
			return -1;
		}
		// �X�V����
		sprintf_s(data.updatetime, mAlarmItemTimeMax, "%s", (LPCSTR)CStringA(szline));

		while (!feof(fp) && list.size() < mAlarmItemMax){
			/* �P�s�ǂݍ��� */
			if (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) == NULL) {
				break;
			}
			int pos, i = 0;
			cbuf = CString(szline);
			while (AfxExtractSubString(str, cbuf, i++, ',')){
				str.TrimLeft();
				str.TrimRight();
				switch (i - 1){
				case	0:
					// �ǖ�
					data.line.no = list.size() + 1;
					data.line.station = 0xffffffff;
					for (pos = 0; pos < sizeof(mAlarmStationString) / sizeof(mAlarmStationString[0]); pos++){
						if (str.CompareNoCase(mAlarmStationString[pos]) == 0){
							data.line.station = pos;
							break;
						}
					}
					break;
				case	1:
					// ���M��
					data.line.sender = 0xffffffff;
					for (pos = 0; pos < sizeof(mAlarmEquipmentString) / sizeof(mAlarmEquipmentString[0]); pos++){
						if (str.CompareNoCase(mAlarmEquipmentString[pos]) == 0){
							data.line.sender = pos;
							break;
						}
					}
					break;
				case	2:
					// �X�e�[�^�X
					data.line.status = 0xffffffff;
					for (pos = 0; pos < sizeof(mAlarmStatusString) / sizeof(mAlarmStatusString[0]); pos++){
						if (str.CompareNoCase(mAlarmStatusString[pos]) == 0){
							data.line.status = pos;
							break;
						}
					}
					break;
				case	3:
					// �Ď����ږ�
					sprintf_s(data.line.monname, mAlarmItemNameMax, "%s", (LPCSTR)CStringA(str));
					break;
				case	4:
					// �Ď�����
					sprintf_s(data.line.monval, mAlarmItemValMax, "%s", (LPCSTR)CStringA(str));
					break;
				default:
					continue;
				}
			}

			// �f�[�^�`�F�b�N
			if (data.line.station == 0xffffffff || data.line.sender == 0xffffffff || data.line.status == 0xffffffff){
				// �G���[
				continue;
			}

			list.push_back(data);
		}

		// ���X�g����̏ꍇ�A�������̂݃��X�g�ǉ�����B
		// �s�ԍ���-1���w�肷��B
		if (list.size() == 0)
		{
			data.line.no = (DWORD)-1;
			list.push_back(data);
		}
	}CATCH_ALL(e){
		ret = -1;
	}
	END_CATCH_ALL

	fclose(fp);
	return ret;
}
