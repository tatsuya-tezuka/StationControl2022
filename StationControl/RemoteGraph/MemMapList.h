#pragma once

using namespace std;

#include "MemMapData.h"

#define	MEMMAP_VER	_T("1.00")

#define	NUM_AROUND_OPEN_MMDATA		1			// �O��̃������}�b�v�f�[�^�t�@�C���I�[�v����

enum eMMListStatus
{
	eMMListStatus_None				= 0,
	eMMListStatus_WriteOpenRealTime	= 1,
	eMMListStatus_WriteOpenAnalysis	= 2,
	eMMListStatus_ReadOpenRealTime	= 3,
	eMMListStatus_ReadOpenAnalysis	= 4
};

using namespace std;

class CMeasureData;
class CMaxMinData;

// �������}�b�v�f�[�^���X�g���Ǘ�����N���X
class CMemMapList
{
public:
	CMemMapList(void);
	~CMemMapList(void);

protected:
	eMMListStatus		m_eMMListStatus;		// �������}�b�v�f�[�^���X�g�X�e�[�^�X
	vector<CMemMapData>	m_listMMData;			// �f�[�^�p�̃������}�b�v�f�[�^���X�g

	UINT				m_startFile;			// �t�@�C���̊J�n�ԍ�
	unsigned __int64	m_countData;			// �ۑ���������f�[�^��(�f�[�^���F0�`18,446,744,073,709,551,615)
	CString				m_strMMInfoFile;		// �������}�b�v���t�@�C����

protected:
	int					m_nView;				// �r���[�ԍ�
	int					m_nData;				// �f�[�^�ԍ�
	CString				m_strBaseName;			// �x�[�X�t�@�C����
	unsigned __int64	m_posDataRWPos;			// ����f�[�^�̓ǂݏ����f�[�^�ʒu(0�`�f�[�^��-1�F0�`18,446,744,073,709,551,615)
	CString				m_strVersion;			// ����f�[�^�t�@�C���o�[�W����

protected:
	void	ClearParameter()
	{
		m_eMMListStatus = eMMListStatus_None;

		m_listMMData.clear();

		m_startFile = 0;
		m_countData = 0;
		m_strMMInfoFile.Empty();

		m_nView = 0;
		m_strBaseName.Empty();
		m_posDataRWPos = 0;
	}

#if 0
	void	SaveMMInfoFile()
	{
		if( !m_strMMInfoFile.IsEmpty() )
		{
			CString	str;

			// �o�[�W������ۑ�
			WritePrivateProfileString(_T("MEMMAP"), _T("Version"), MEMMAP_VER, m_strMMInfoFile);

			// �x�[�X�t�@�C������ۑ�
			WritePrivateProfileString(_T("MEMMAP"), _T("BaseName"), m_strBaseName, m_strMMInfoFile);

			// �f�[�^����ۑ�
			str.Format(_T("%llu"), m_countData);
			WritePrivateProfileString(_T("MEMMAP"), _T("CountData"), str, m_strMMInfoFile);
		}
	}
#endif

#if 0
	void	LoadMMInfoFile()
	{
		if( !m_strMMInfoFile.IsEmpty() )
		{
			TCHAR	wkbuf[256];

			// �o�[�W�������擾
			GetPrivateProfileString(_T("MEMMAP"), _T("Version"), _T("0.00"), wkbuf, _countof(wkbuf), m_strMMInfoFile);
			m_strVersion = wkbuf;

			// �x�[�X�t�@�C�����擾
			GetPrivateProfileString(_T("MEMMAP"), _T("BaseName"), _T(""), wkbuf, _countof(wkbuf), m_strMMInfoFile);
			m_strBaseName = wkbuf;

			// �f�[�^�����擾
			GetPrivateProfileString(_T("MEMMAP"), _T("CountData"), _T("0"), wkbuf, _countof(wkbuf), m_strMMInfoFile);
			m_countData = _tcstoui64(wkbuf, NULL, 10);
		}
	}
#endif

public:
	UINT	GetMaxSizePerMMData()	{ return MAX_SIZE_PER_MMDATA; }

	eMMListStatus	GetMemMapListStatus()	{ return m_eMMListStatus; }
	UINT	GetDataFileCount()	{ return (UINT)((m_countData + (GetMaxSizePerMMData() - 1)) / GetMaxSizePerMMData()); }
	UINT	GetStartFile()	{ return m_startFile; }
	unsigned __int64	GetDataCount()	{ return m_countData; }
	unsigned __int64	GetDataRWPos()	{ return m_posDataRWPos; }
	CString	GetMMInfoFileName()	{ return m_strMMInfoFile; }

public:
	bool	DeleteMemMapList();

	bool	WriteInitialization(int nView, int nData, UINT nStartFile);
	bool	WriteFinalization();
	bool	WriteData(stSpaceData& oSpaceData);

	bool	ReadInitialization(int nView, int nData, UINT nStartFile, unsigned __int64 nCountData);
	bool	ReadFinalization();
	bool	ReadData(unsigned __int64 nStart, unsigned __int64 nEnd, vector<stSpaceData>& vSpaceData);
};
