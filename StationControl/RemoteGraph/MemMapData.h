#pragma once

using namespace std;

#include "const.h"

enum eMMDataType
{
	eMMDataType_None	= 0,
	eMMDataType_Data	= 1,
};

// �������}�b�v�f�[�^���Ǘ�����N���X
class CMemMapData
{
public:
	CMemMapData(void);
	~CMemMapData(void);

protected:
	eMMDataType	m_eMMDataType;			// �������}�b�v�f�[�^�^�C�v
	CString	m_strMMDataFile;			// �������}�b�v�f�[�^�t�@�C����
	HANDLE	m_hMMDataFile;				// �������}�b�v�f�[�^�t�@�C���n���h��
	HANDLE	m_hMMDataObject;			// �������}�b�v�f�[�^�t�@�C���}�b�s���O�I�u�W�F�N�g�n���h��
	LPVOID	m_pMMDataMapView;			// �������}�b�v�f�[�^�t�@�C���}�b�v�r���[
	vector<LPVOID>	m_listMMDataMap;	// �������}�b�v�f�[�^�}�b�v���X�g

protected:
	void	ClearParameter()
	{
		m_eMMDataType = eMMDataType_None;
		m_strMMDataFile.Empty();
		m_hMMDataFile = INVALID_HANDLE_VALUE;
		m_hMMDataObject = NULL;
		m_pMMDataMapView = NULL;
		m_listMMDataMap.clear();
	}

	bool	MappingUpdateMMDataFile(DWORD nMMDataMapView);

public:
	void	SetMMDataFileName(CString strMMDataFile)	{ m_strMMDataFile = strMMDataFile; }

	bool	IsOpenMMDataFile()	{ return (m_hMMDataFile != INVALID_HANDLE_VALUE) ? true : false; }

public:
	bool	OpenMMDataFile(eMMDataType typeMMData, bool flgRead = true);
	void	CloseMMDataFile();

	bool	WriteMMDataFile(stSpaceData& oSpaceData);
	bool	ReadMMDataFile(unsigned __int64 nStart, unsigned __int64 nEnd, vector<stSpaceData>& vSpaceData, UINT nOffset);
};
