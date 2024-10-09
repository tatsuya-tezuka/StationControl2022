#pragma once

using namespace std;

#include "const.h"

enum eMMDataType
{
	eMMDataType_None	= 0,
	eMMDataType_Data	= 1,
};

// メモリマップデータを管理するクラス
class CMemMapData
{
public:
	CMemMapData(void);
	~CMemMapData(void);

protected:
	eMMDataType	m_eMMDataType;			// メモリマップデータタイプ
	CString	m_strMMDataFile;			// メモリマップデータファイル名
	HANDLE	m_hMMDataFile;				// メモリマップデータファイルハンドル
	HANDLE	m_hMMDataObject;			// メモリマップデータファイルマッピングオブジェクトハンドル
	LPVOID	m_pMMDataMapView;			// メモリマップデータファイルマップビュー
	vector<LPVOID>	m_listMMDataMap;	// メモリマップデータマップリスト

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
