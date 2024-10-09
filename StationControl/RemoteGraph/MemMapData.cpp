
#include "StdAfx.h"
#include "RemoteGraph.h"
#include "MemMapData.h"

CMemMapData::CMemMapData(void)
{
	ClearParameter();
}

CMemMapData::~CMemMapData(void)
{
}

//============================================================================
//	@brief	�������}�b�v�f�[�^�t�@�C�����}�b�s���O
//	@param	nMMDataMapView	�������}�b�v�f�[�^�t�@�C���}�b�v�r���[�̃T�C�Y
//	@return	bool	����
//============================================================================
bool CMemMapData::MappingUpdateMMDataFile(DWORD nMMDataMapView)
{
	LPSTR	pCur;
	bool	bRes = true;

//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("MappingUpdateMMDataFile"), _T("Start"), _T(""), nLogEx::debug);

	// �������}�b�v�f�[�^�t�@�C���}�b�v�r���[���m�F
	if( (pCur = (LPSTR)m_pMMDataMapView) != NULL )
	{
		while( nMMDataMapView )
		{
			// �o�C�g���i�[���̃T�C�Y���m�F
			if( nMMDataMapView < sizeof(unsigned long) )
			{
				bRes = false;
				break;
			}

			// �o�C�g�����擾
			unsigned long	nData = *((unsigned long *)pCur);

			// �o�C�g�����̃T�C�Y���m�F
			if( nMMDataMapView < nData )
			{
				bRes = false;
				break;
			}

			// �f�[�^�̐擪�|�C���^�����X�g�ɒǉ�
			m_listMMDataMap.push_back(pCur);

			// �|�C���^�ƃT�C�Y���X�V
			pCur += nData;
			nMMDataMapView -= nData;
		}
	}

//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("MappingUpdateMMDataFile"), _T("End"), _T(""), nLogEx::debug);

	return bRes;
}

//============================================================================
//	@brief	�������}�b�v�f�[�^�t�@�C���̃I�[�v��
//	@param	typeMMData	�������}�b�v�f�[�^�^�C�v
//	@param	flgRead		���[�h�I�[�v���t���O
//	@return	bool	����
//============================================================================
bool CMemMapData::OpenMMDataFile(eMMDataType typeMMData, bool flgRead/* = true*/)
{
//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("OpenMMDataFile"), _T("Start"), _T(""), nLogEx::debug);

	// �������}�b�v�f�[�^�t�@�C�������m�F
	if( m_strMMDataFile.IsEmpty() )
		return false;

	// �������}�b�v�f�[�^�t�@�C���n���h�����m�F
	if( m_hMMDataFile != INVALID_HANDLE_VALUE )
		return false;

	TRY
	{
		CStringA	strMMDataFile = (CT2A)m_strMMDataFile;

		// �������}�b�v�f�[�^�t�@�C���^�C�v��ݒ�
		m_eMMDataType = typeMMData;

		// �������}�b�v�f�[�^�t�@�C�����I�[�v��
		if( flgRead )
			m_hMMDataFile = ::CreateFileA(strMMDataFile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		else
			m_hMMDataFile = ::CreateFileA(strMMDataFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if( m_hMMDataFile == INVALID_HANDLE_VALUE )
		{
			CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("OpenMMDataFile"), _T("Error"), _T("CreateFileA error."), nLogEx::error);
			AfxThrowUserException();
		}

		if( flgRead )
		{
			DWORD	dwLow, dwHigh;

			// �t�@�C���T�C�Y���擾
			dwLow = ::GetFileSize(m_hMMDataFile, &dwHigh);
			if( (dwLow == -1) && (GetLastError() != NO_ERROR) )
			{
				CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("OpenMMDataFile"), _T("Error"), _T("GetFileSize error."), nLogEx::error);
				AfxThrowUserException();
			}

			// �t�@�C���}�b�s���O�I�u�W�F�N�g���쐬
			m_hMMDataObject = ::CreateFileMapping(m_hMMDataFile, NULL, PAGE_READONLY,  0, dwLow, NULL);
			if( m_hMMDataObject == NULL )
			{
				CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("OpenMMDataFile"), _T("Error"), _T("CreateFileMapping error."), nLogEx::error);
				AfxThrowUserException();
			}

			// �A�h���X��ԂɃt�@�C���̃r���[���}�b�v
			m_pMMDataMapView = ::MapViewOfFile(m_hMMDataObject, FILE_MAP_READ, 0, 0, dwLow);
			if( m_pMMDataMapView == NULL )
			{
				CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("OpenMMDataFile"), _T("Error"), _T("MapViewOfFile error."), nLogEx::error);
				AfxThrowUserException();
			}

			// �������}�b�v�f�[�^�t�@�C�����}�b�v
			if( !MappingUpdateMMDataFile(dwLow) )
			{
//				CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("OpenMMDataFile"), _T("Error"), _T("MappingUpdateMMDataFile error."), nLogEx::error);
//				AfxThrowUserException();
			}
		}
	}
	CATCH_ALL(e)
	{
		CString	sError;
		sError.Format(_T("GetLastError = %d"), ::GetLastError());
		CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("OpenMMDataFile"), _T("Exception"), sError, nLogEx::error);

		// �������}�b�v�f�[�^�}�b�v���X�g���폜
		if( m_listMMDataMap.size() > 0 )
		{
			vector<LPVOID>	tmp_listMMDataMap(0);

			m_listMMDataMap.swap(tmp_listMMDataMap);

			m_listMMDataMap.clear();
		}

		// �A�h���X��Ԃ̃t�@�C���r���[���A���}�b�v
		if( m_pMMDataMapView != NULL )
		{
			::UnmapViewOfFile(m_pMMDataMapView);

			m_pMMDataMapView = NULL;
		}

		// �t�@�C���}�b�s���O�I�u�W�F�N�g���N���[�Y
		if( m_hMMDataObject != NULL )
		{
			::CloseHandle(m_hMMDataObject);

			m_hMMDataObject = NULL;
		}

		// �������}�b�v�f�[�^�t�@�C�����N���[�Y
		if( m_hMMDataFile != INVALID_HANDLE_VALUE )
		{
			::CloseHandle(m_hMMDataFile);

			m_hMMDataFile = INVALID_HANDLE_VALUE;
		}

		// �������}�b�v�f�[�^�t�@�C���^�C�v���N���A
		m_eMMDataType = eMMDataType_None;

		return false;
	}
	END_CATCH_ALL

//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("OpenMMDataFile"), _T("End"), _T(""), nLogEx::debug);

	return true;
}

//============================================================================
//	@brief	�������}�b�v�f�[�^�t�@�C���̃N���[�Y
//	@param	
//	@return	
//============================================================================
void CMemMapData::CloseMMDataFile()
{
//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("CloseMMDataFile"), _T("Start"), _T(""), nLogEx::debug);

	// �������}�b�v�f�[�^�}�b�v���X�g���폜
	if( m_listMMDataMap.size() > 0 )
	{
		vector<LPVOID>	tmp_listMMDataMap(0);

		m_listMMDataMap.swap(tmp_listMMDataMap);

		m_listMMDataMap.clear();
	}

	// �A�h���X��Ԃ̃t�@�C���r���[���A���}�b�v
	if( m_pMMDataMapView != NULL )
	{
		::UnmapViewOfFile(m_pMMDataMapView);

		m_pMMDataMapView = NULL;
	}

	// �t�@�C���}�b�s���O�I�u�W�F�N�g���N���[�Y
	if( m_hMMDataObject != NULL )
	{
		::CloseHandle(m_hMMDataObject);

		m_hMMDataObject = NULL;
	}

	// �������}�b�v�f�[�^�t�@�C�����N���[�Y
	if( m_hMMDataFile != INVALID_HANDLE_VALUE )
	{
		::CloseHandle(m_hMMDataFile);

		m_hMMDataFile = INVALID_HANDLE_VALUE;
	}

	// �������}�b�v�f�[�^�t�@�C���^�C�v���N���A
	m_eMMDataType = eMMDataType_None;

//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("CloseMMDataFile"), _T("End"), _T(""), nLogEx::debug);
}

//============================================================================
//	@brief	�f�[�^���������}�b�v�f�[�^�t�@�C���ɏ�������
//	@param	oSpaceData	�f�[�^
//	@return	bool		����
//============================================================================
bool CMemMapData::WriteMMDataFile(stSpaceData& oSpaceData)
{
//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("WriteMMDataFile"), _T("Start"), _T(""), nLogEx::debug);

	// �������}�b�v�f�[�^�t�@�C���n���h�����m�F
	if( m_hMMDataFile == INVALID_HANDLE_VALUE )
		return false;

	TRY
	{
		DWORD	nWritten;
		DWORD	nULong;

		// �o�C�g��(�S)
		nULong = sizeof(unsigned long) + sizeof(stSpaceData);
		if( ::WriteFile(m_hMMDataFile, &nULong, sizeof(unsigned long), &nWritten, NULL) == 0 )
			AfxThrowUserException();

		// ���莞��(�P�U)
		if( ::WriteFile(m_hMMDataFile, &oSpaceData.systime, sizeof(SYSTEMTIME), &nWritten, NULL) == 0 )
			AfxThrowUserException();

		// �X�e�[�^�X(�Q)
		if( ::WriteFile(m_hMMDataFile, &oSpaceData.status, sizeof(USHORT), &nWritten, NULL) == 0 )
			AfxThrowUserException();

		// ���k��(�Q)
		if( ::WriteFile(m_hMMDataFile, &oSpaceData.mmPair.cnt, sizeof(USHORT), &nWritten, NULL) == 0 )
			AfxThrowUserException();

		// �ő�l(�W)
		if( ::WriteFile(m_hMMDataFile, &oSpaceData.mmPair.max, sizeof(double), &nWritten, NULL) == 0 )
			AfxThrowUserException();

		// �ŏ��l(�W)
		if( ::WriteFile(m_hMMDataFile, &oSpaceData.mmPair.min, sizeof(double), &nWritten, NULL) == 0 )
			AfxThrowUserException();
	}
	CATCH_ALL(e)
	{
		CString	sError;
		sError.Format(_T("GetLastError = %d"), ::GetLastError());
		CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("WriteMMDataFile"), _T("Exception"), sError, nLogEx::error);

		// �������}�b�v�f�[�^�t�@�C�����N���[�Y
		if( m_hMMDataFile != INVALID_HANDLE_VALUE )
		{
			::CloseHandle(m_hMMDataFile);

			m_hMMDataFile = INVALID_HANDLE_VALUE;
		}

		return false;
	}
	END_CATCH_ALL

//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("WriteMMDataFile"), _T("End"), _T(""), nLogEx::debug);

	return true;
}

//============================================================================
//	@brief	�f�[�^���������}�b�v�f�[�^�t�@�C������ǂݍ���
//	@param	nStart			�擾����f�[�^�̊J�n�ԍ�
//	@param	nEndl			�擾����f�[�^�̏I���ԍ�
//	@param	vCompData		�f�[�^�̊i�[�A�h���X�ւ̎Q��
//	@param	nOffset			�f�[�^�̊i�[�ʒu
//	@return	bool			����
//============================================================================
bool CMemMapData::ReadMMDataFile(unsigned __int64 nStart, unsigned __int64 nEnd, vector<stSpaceData>& vSpaceData, UINT nOffset)
{
	LPSTR	pCur;

	//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("ReadMMDataFile"), _T("Start"), _T(""), nLogEx::debug);

	// �������}�b�v�f�[�^�t�@�C���n���h�����m�F
	if( m_hMMDataFile == INVALID_HANDLE_VALUE )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("ReadMMDataFile"), _T("Error"), _T("File handle is none."), nLogEx::error);
		return false;
	}

	// �������}�b�v�f�[�^�}�b�v���X�g���m�F
	if( m_listMMDataMap.size() <= 0 )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("ReadMMDataFile"), _T("Error"), _T("Map list is none."), nLogEx::error);
		return false;
	}

	unsigned long	nBytes;

	for( unsigned __int64 nCur = nStart; nCur <= nEnd; nCur++ )
	{
		if( (UINT)(nCur % MAX_SIZE_PER_MMDATA) >= m_listMMDataMap.size() )
			continue;

		// �w�肳�ꂽ�ʂ��ԍ��̃f�[�^�ւ̃|�C���^���擾
		if( (pCur = (LPSTR)m_listMMDataMap[(UINT)(nCur % MAX_SIZE_PER_MMDATA)]) == NULL )
		{
			CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("ReadMMDataFile"), _T("Error"), _T("Pointer is null."), nLogEx::error);
			return false;
		}

		// �o�C�g��(�S)
		nBytes = *((unsigned long *)pCur);
		pCur += sizeof(unsigned long);	nBytes -= sizeof(unsigned long);

		// ���莞��(�P�U)
		memcpy(&(vSpaceData[UINT(nCur - nStart) + nOffset].systime), ((SYSTEMTIME *)pCur), sizeof(SYSTEMTIME));
		pCur += sizeof(SYSTEMTIME);	nBytes -= sizeof(SYSTEMTIME);

		// �X�e�[�^�X(�Q)
		memcpy(&(vSpaceData[UINT(nCur - nStart) + nOffset].status), ((USHORT *)pCur), sizeof(USHORT));
		pCur += sizeof(USHORT);	nBytes -= sizeof(USHORT);

		// ���k��(�Q)
		memcpy(&(vSpaceData[UINT(nCur - nStart) + nOffset].mmPair.cnt), ((USHORT *)pCur), sizeof(USHORT));
		pCur += sizeof(USHORT);	nBytes -= sizeof(USHORT);

		// �ő�l(�W)
		memcpy(&(vSpaceData[UINT(nCur - nStart) + nOffset].mmPair.max), ((double *)pCur), sizeof(double));
		pCur += sizeof(double);	nBytes -= sizeof(double);

		// �ŏ��l(�W)
		memcpy(&(vSpaceData[UINT(nCur - nStart) + nOffset].mmPair.min), ((double *)pCur), sizeof(double));
		pCur += sizeof(double);	nBytes -= sizeof(double);

		// �c��̃o�C�g�����m�F
		if( nBytes != 0 )
		{
			CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("ReadMMDataFile"), _T("Error"), _T("Bytes is left over."), nLogEx::error);
			return false;
		}
	}

//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("ReadMMDataFile"), _T("End"), _T(""), nLogEx::debug);

	return true;
}
