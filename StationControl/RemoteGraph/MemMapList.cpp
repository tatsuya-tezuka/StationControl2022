
#include "StdAfx.h"
#include "RemoteGraph.h"
#include "MemMapList.h"

CMemMapList::CMemMapList(void)
{
	ClearParameter();
}

CMemMapList::~CMemMapList(void)
{
	DeleteMemMapList();
}

//============================================================================
//	@brief	�������}�b�v���X�g�̍폜
//	@param	
//	@return	bool	����
//============================================================================
bool CMemMapList::DeleteMemMapList()
{
//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("DeleteMemMapList"), _T("Start"), _T(""), nLogEx::debug);

	vector<CMemMapData>	tmp_listMMData(0);

	// ���X�g�����m�F
	if( m_listMMData.size() == 0 )
	{
//		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("DeleteMemMapList"), _T("End"), _T("No data."), nLogEx::debug);
		return true;
	}

	// �������}�b�v�f�[�^�t�@�C�����N���[�Y
	for( UINT i = 0; i < m_listMMData.size(); i++ )
		m_listMMData[i].CloseMMDataFile();

	// �������}�b�v�f�[�^���X�g�����S�ɍ폜���邽�߃��[�J�����X�g�Ɍ������č폜
	m_listMMData.swap(tmp_listMMData);

	// �p�����[�^���N���A
	ClearParameter();

//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("DeleteMemMapList"), _T("End"), _T(""), nLogEx::debug);

	return true;
}

//============================================================================
//	@brief	����f�[�^�ۑ��̊J�n����
//	@param	strMMInfoFile	�������}�b�v���t�@�C����
//	@param	nView		�r���[�ԍ�
//	@param	nData		�f�[�^�ԍ�
//	@param	nStartFile	�t�@�C���̊J�n�ԍ�
//	@return	bool	����
//============================================================================
bool CMemMapList::WriteInitialization(int nView, int nData, UINT nStartFile)
{
	CString	strBaseName;

//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteInitialization"), _T("Start"), _T(""), nLogEx::debug);

	// �������}�b�v���X�g���폜
	if( !DeleteMemMapList() )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteInitialization"), _T("Error"), _T("DeleteMemMapList failed."), nLogEx::error);
		return false;
	}

	// �p�����[�^��������
	m_nView = nView;
	m_nData = nData;
	m_startFile = nStartFile;
	m_posDataRWPos = 0;

	// �������}�b�v���t�@�C�������쐬
	CString	strMMInfoFile;
	if( nView == -1 )
		strMMInfoFile.Format(_T("%s\\%s.cfg"), (LPCTSTR)theApp.GetAppDataPath(), REALTIME_FNAME);
	else
		strMMInfoFile.Format(_T("%s\\%s%d.cfg"), (LPCTSTR)theApp.GetAppDataPath(), ANALYSIS_FNAME, nView + 1);

	// �x�[�X�t�@�C������ݒ�
	wchar_t	pFile[MAX_PATH];
	wcscpy_s(pFile, _countof(pFile), (LPCTSTR)strMMInfoFile);
	PathRemoveExtension((LPWSTR)pFile);
	PathStripPath((LPWSTR)pFile);
	m_strBaseName.Format(_T("%s_data%02d"), pFile, nData);

	// �������}�b�v���t�@�C������ݒ�
	m_strMMInfoFile = strMMInfoFile;

	// �X�e�[�^�X���X�V
	if( nView == -1 )
		m_eMMListStatus = eMMListStatus_WriteOpenRealTime;
	else
		m_eMMListStatus = eMMListStatus_WriteOpenAnalysis;

//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteInitialization"), _T("End"), _T(""), nLogEx::debug);

	return true;
}

//============================================================================
//	@brief	����f�[�^�ۑ��̏I������
//	@param	
//	@return	bool		����
//============================================================================
bool CMemMapList::WriteFinalization()
{
//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteFinalization"), _T("Start"), _T(""), nLogEx::debug);

	// �X�e�[�^�X���m�F
	if( (m_eMMListStatus != eMMListStatus_WriteOpenRealTime) && (m_eMMListStatus != eMMListStatus_WriteOpenAnalysis) )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteFinalization"), _T("Error"), _T("Status is no write open."), nLogEx::error);
		return false;
	}

	// �ۑ���������f�[�^����ݒ�
	m_countData = m_posDataRWPos;

#if 0
	// �������}�b�v���t�@�C���Ƀp�����[�^��ۑ�
	SaveMMInfoFile();
#endif

	// �������}�b�v�f�[�^�t�@�C�����N���[�Y
	if( m_countData != 0 )
	{
		unsigned int posDataRWPos = (unsigned int)(m_posDataRWPos / GetMaxSizePerMMData());
		if( (m_posDataRWPos % GetMaxSizePerMMData()) == 0 )
			posDataRWPos -= 1;
		if( posDataRWPos >= m_listMMData.size() )
		{
			CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteFinalization"), _T("Error"), _T("Close position over."), nLogEx::error);
			return false;
		}
		m_listMMData[posDataRWPos].CloseMMDataFile();
	}

	// �X�e�[�^�X���N���A
	m_eMMListStatus = eMMListStatus_None;

//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteFinalization"), _T("End"), _T(""), nLogEx::debug);

	return true;
}

//============================================================================
//	@brief	�f�[�^�̏�������
//	@param	oSpaceData	�f�[�^
//	@return	bool		����
//============================================================================
bool CMemMapList::WriteData(stSpaceData& oSpaceData)
{
//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteData"), _T("Start"), _T(""), nLogEx::debug);

	// �X�e�[�^�X���m�F
	if( (m_eMMListStatus != eMMListStatus_WriteOpenRealTime) && (m_eMMListStatus != eMMListStatus_WriteOpenAnalysis) )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteData"), _T("Error"), _T("Status is no write open."), nLogEx::error);
		return false;
	}

	// �������݃f�[�^�ʒu���m�F
	if( (m_posDataRWPos % GetMaxSizePerMMData()) == 0 )
	{
		CString	strMMDataFile;
		CMemMapData	newMMData;

		// �������}�b�v�f�[�^�t�@�C�����N���[�Y
		if( m_posDataRWPos != 0 )
		{
			unsigned int	posTmp = (unsigned int)(m_posDataRWPos / GetMaxSizePerMMData()) - 1;
			if( posTmp >= m_listMMData.size() )
			{
				CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteData"), _T("Error"), _T("Close position over."), nLogEx::error);
				return false;
			}
			m_listMMData[posTmp].CloseMMDataFile();
		}

		// �������}�b�v�f�[�^�t�@�C�������쐬
		strMMDataFile.Format(_T("%s\\%s_%d.spd"), (LPCTSTR)theApp.GetAppDataPath(), (LPCTSTR)m_strBaseName, (UINT)(m_posDataRWPos / GetMaxSizePerMMData()) + m_startFile + 1);
		newMMData.SetMMDataFileName(strMMDataFile);

		// �������}�b�v�f�[�^�t�@�C�����I�[�v��
		if( !newMMData.OpenMMDataFile(eMMDataType_Data, false) )
		{
			CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteData"), _T("Error"), _T("Open error."), nLogEx::error);
			return false;
		}

		// �V�����������}�b�v�f�[�^�����X�g�ɒǉ�
		m_listMMData.push_back(newMMData);
	}

	// �������ݒ��̃������}�b�v�f�[�^���擾
	unsigned int	posTmp = (unsigned int)(m_posDataRWPos / GetMaxSizePerMMData());
	if( posTmp >= m_listMMData.size() )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteData"), _T("Error"), _T("Access position over."), nLogEx::error);
		return false;
	}
	CMemMapData&	curMMData = m_listMMData[posTmp];

	// �������݃f�[�^�ʒu���X�V
	m_posDataRWPos++;

	// �L���f�[�^����������
	if( !curMMData.WriteMMDataFile(oSpaceData) )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteData"), _T("Error"), _T("Write error."), nLogEx::error);
		return false;
	}

//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteData"), _T("End"), _T(""), nLogEx::debug);

	return true;
}

//============================================================================
//	@brief	����f�[�^�W�J�̊J�n����
//	@param	nView			�r���[�ԍ�
//	@param	nData			�f�[�^�ԍ�
//	@param	nStartFile		�t�@�C���̊J�n�ԍ�
//	@param	nCountData		�f�[�^��
//	@return	bool			����
//============================================================================
bool CMemMapList::ReadInitialization(int nView, int nData, UINT nStartFile, unsigned __int64 nCountData)
{
//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadInitialization"), _T("Start"), _T(""), nLogEx::debug);

	// �������}�b�v���X�g���폜
	if( !DeleteMemMapList() )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadInitialization"), _T("Error"), _T("DeleteMemMapList failed."), nLogEx::error);
		return false;
	}

	// �p�����[�^��������
	m_nView = nView;
	m_nData = nData;
	m_startFile = nStartFile;
	m_posDataRWPos = 0;

	// �������}�b�v���t�@�C�������쐬
	CString	strMMInfoFile;
	if( nView == -1 )
		strMMInfoFile.Format(_T("%s\\%s.cfg"), (LPCTSTR)theApp.GetAppDataPath(), REALTIME_FNAME);
	else
		strMMInfoFile.Format(_T("%s\\%s%d.cfg"), (LPCTSTR)theApp.GetAppDataPath(), ANALYSIS_FNAME, nView + 1);

	// �p�X����t�@�C��������菜��
	wchar_t	pPath[MAX_PATH];
	wcscpy_s(pPath, _countof(pPath), (LPCTSTR)strMMInfoFile);
	PathRemoveFileSpec((LPWSTR)pPath);

#if 0
	// �������}�b�v���t�@�C������p�����[�^��ݒ�
	LoadMMInfoFile();
#else
	// �x�[�X�t�@�C������ݒ�
	wchar_t	pFile[MAX_PATH];
	wcscpy_s(pFile, _countof(pFile), (LPCTSTR)strMMInfoFile);
	PathRemoveExtension((LPWSTR)pFile);
	PathStripPath((LPWSTR)pFile);
	m_strBaseName.Format(_T("%s_data%02d"), pFile, nData);

	// �������}�b�v���t�@�C������ݒ�
	m_strMMInfoFile = strMMInfoFile;

	// �f�[�^�����擾
	m_countData = nCountData;
#endif

	// �������}�b�v�f�[�^���쐬
	for( unsigned __int64 i = 0; i < m_countData; i += GetMaxSizePerMMData() )
	{
		CString	strMMDataFile;

		// �������}�b�v�f�[�^�t�@�C�������쐬
		strMMDataFile.Format(_T("%s\\%s_%d.spd"), pPath, (LPCTSTR)m_strBaseName, (UINT)(i / GetMaxSizePerMMData()) + m_startFile + 1);

		{
			CMemMapData	newMMData;

			// �������}�b�v�f�[�^�t�@�C������ݒ�
			newMMData.SetMMDataFileName(strMMDataFile);

			// �V�����������}�b�v�f�[�^�����X�g�ɒǉ�
			m_listMMData.push_back(newMMData);
		}
	}

	// �X�e�[�^�X���X�V
	if( nView == -1 )
		m_eMMListStatus = eMMListStatus_ReadOpenRealTime;
	else
		m_eMMListStatus = eMMListStatus_ReadOpenAnalysis;

//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadInitialization"), _T("End"), _T(""), nLogEx::debug);

	return true;
}

//============================================================================
//	@brief	����f�[�^�W�J�̏I������
//	@param	
//	@return	bool		����
//============================================================================
bool CMemMapList::ReadFinalization()
{
//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadFinalization"), _T("Start"), _T(""), nLogEx::debug);

	// �X�e�[�^�X���m�F
	if( (m_eMMListStatus != eMMListStatus_ReadOpenRealTime) && (m_eMMListStatus != eMMListStatus_ReadOpenAnalysis) )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadFinalization"), _T("Error"), _T("Status is no read open."), nLogEx::error);
		return false;
	}

	// �������}�b�v�f�[�^�t�@�C�����N���[�Y
	for( UINT i = 0; i < m_listMMData.size(); i++ )
		m_listMMData[i].CloseMMDataFile();

	// �p�����[�^���N���A
	ClearParameter();

	// �X�e�[�^�X���N���A
	m_eMMListStatus = eMMListStatus_None;

//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadFinalization"), _T("End"), _T(""), nLogEx::debug);

	return true;
}

//============================================================================
//	@brief	�f�[�^�̓ǂݍ���
//	@param	nStart			�擾����f�[�^�̊J�n�ԍ�
//	@param	nEnd			�擾����f�[�^�̏I���ԍ�
//	@param	vCompData		�f�[�^�̊i�[�A�h���X�ւ̎Q��
//	@return	bool			����
//============================================================================
bool CMemMapList::ReadData(unsigned __int64 nStart, unsigned __int64 nEnd, vector<stSpaceData>& vSpaceData)
{
	unsigned int	nCurIndex, nCurFIndex, nCurBIndex, nPreIndex, nPreFIndex, nPreBIndex, nMinIndex, nMaxIndex;
	UINT	nOffset = 0;

//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadData"), _T("Start"), _T(""), nLogEx::debug);

	// �X�e�[�^�X���m�F
	if( (m_eMMListStatus != eMMListStatus_ReadOpenRealTime) && (m_eMMListStatus != eMMListStatus_ReadOpenAnalysis) )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadData"), _T("Error"), _T("Status is no read open."), nLogEx::error);
		return false;
	}

	// �w�肳�ꂽ�ʂ��ԍ����m�F
	if( nStart >= m_countData )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadData"), _T("Error"), _T("Serial start number is invalid."), nLogEx::error);
		return false;
	}
	if( nEnd >= m_countData )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadData"), _T("Error"), _T("Serial end number is invalid."), nLogEx::error);
		return false;
	}

	for( unsigned __int64 nCur = nStart; nCur <= nEnd; )
	{
		// �w�肳�ꂽ�ʂ��ԍ����烁�����}�b�v�f�[�^�̃C���f�b�N�X���擾
		nCurIndex = (unsigned int)(nCur / GetMaxSizePerMMData());
		if( nCurIndex >= m_listMMData.size() )
		{
			CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadData"), _T("Error"), _T("Index is invalid."), nLogEx::error);
			return false;
		}

		// �I�[�v�������O��̃������}�b�v�f�[�^�t�@�C���̃C���f�b�N�X���擾
		nCurFIndex = ((nCurIndex - NUM_AROUND_OPEN_MMDATA) > nCurIndex) ? 0 : (nCurIndex - NUM_AROUND_OPEN_MMDATA);
		nCurBIndex = (((nCurIndex + NUM_AROUND_OPEN_MMDATA) < nCurIndex) || ((nCurIndex + NUM_AROUND_OPEN_MMDATA) > ((unsigned int)m_listMMData.size() - 1))) ? ((unsigned int)m_listMMData.size() - 1) : (nCurIndex + NUM_AROUND_OPEN_MMDATA);

		// ���X�I�[�v�����̃������}�b�v�f�[�^�̃C���f�b�N�X���擾
		nPreIndex = (unsigned int)(m_posDataRWPos / GetMaxSizePerMMData());

		// ���X�I�[�v�����̑O��̃������}�b�v�f�[�^�t�@�C���̃C���f�b�N�X���擾
		nPreFIndex = ((nPreIndex - NUM_AROUND_OPEN_MMDATA) > nPreIndex) ? 0 : (nPreIndex - NUM_AROUND_OPEN_MMDATA);
		nPreBIndex = (((nPreIndex + NUM_AROUND_OPEN_MMDATA) < nPreIndex) || ((nPreIndex + NUM_AROUND_OPEN_MMDATA) > ((unsigned int)m_listMMData.size() - 1))) ? ((unsigned int)m_listMMData.size() - 1) : (nPreIndex + NUM_AROUND_OPEN_MMDATA);

		// �ŏ��^�ő�C���f�b�N�X���擾
		nMinIndex = min(nCurFIndex, nPreFIndex);
		nMaxIndex = max(nCurBIndex, nPreBIndex);

		// �������}�b�v�f�[�^�t�@�C�����I�[�v���^�N���[�Y
		for( unsigned int nTmpIndex = nMinIndex; nTmpIndex <= nMaxIndex; nTmpIndex++ )
		{
			if( nTmpIndex >= m_listMMData.size() )
			{
				CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadData"), _T("Error"), _T("Access position over."), nLogEx::error);
				return false;
			}

			if( (nCurFIndex <= nTmpIndex) && (nTmpIndex <= nCurBIndex) )
			{
				CMemMapData&	tmpMMData = m_listMMData[nTmpIndex];

				if( !tmpMMData.IsOpenMMDataFile() )
				{
					if( !tmpMMData.OpenMMDataFile(eMMDataType_Data, true) )
					{
						CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadData"), _T("Error"), _T("Open error."), nLogEx::error);
						return false;
					}
				}

				continue;
			}

			if( (nPreFIndex <= nTmpIndex) && (nTmpIndex <= nPreBIndex) )
			{
				CMemMapData&	tmpMMData = m_listMMData[nTmpIndex];

				if( tmpMMData.IsOpenMMDataFile() )
					tmpMMData.CloseMMDataFile();

				continue;
			}
		}

		unsigned __int64	nTmpEnd = nEnd;

		if( nCurIndex != (unsigned int)(nEnd / GetMaxSizePerMMData()) )
			nTmpEnd = nCurIndex * GetMaxSizePerMMData() + (GetMaxSizePerMMData() - 1);

		// �f�[�^��ǂݍ���
		if( !m_listMMData[nCurIndex].ReadMMDataFile(nCur, nTmpEnd, vSpaceData, nOffset) )
		{
			CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadData"), _T("Error"), _T("Read error."), nLogEx::error);
			return false;
		}

		// �ǂݍ��݃f�[�^�ʒu���X�V
		m_posDataRWPos = nTmpEnd;

		nCur = nTmpEnd + 1;
		nOffset = (UINT)(nCur - nStart);
	}

//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadData"), _T("End"), _T(""), nLogEx::debug);

	return true;
}
