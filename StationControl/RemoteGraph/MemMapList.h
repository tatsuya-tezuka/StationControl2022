#pragma once

using namespace std;

#include "MemMapData.h"

#define	MEMMAP_VER	_T("1.00")

#define	NUM_AROUND_OPEN_MMDATA		1			// 前後のメモリマップデータファイルオープン数

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

// メモリマップデータリストを管理するクラス
class CMemMapList
{
public:
	CMemMapList(void);
	~CMemMapList(void);

protected:
	eMMListStatus		m_eMMListStatus;		// メモリマップデータリストステータス
	vector<CMemMapData>	m_listMMData;			// データ用のメモリマップデータリスト

	UINT				m_startFile;			// ファイルの開始番号
	unsigned __int64	m_countData;			// 保存した測定データ数(データ数：0～18,446,744,073,709,551,615)
	CString				m_strMMInfoFile;		// メモリマップ情報ファイル名

protected:
	int					m_nView;				// ビュー番号
	int					m_nData;				// データ番号
	CString				m_strBaseName;			// ベースファイル名
	unsigned __int64	m_posDataRWPos;			// 測定データの読み書きデータ位置(0～データ数-1：0～18,446,744,073,709,551,615)
	CString				m_strVersion;			// 測定データファイルバージョン

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

			// バージョンを保存
			WritePrivateProfileString(_T("MEMMAP"), _T("Version"), MEMMAP_VER, m_strMMInfoFile);

			// ベースファイル名を保存
			WritePrivateProfileString(_T("MEMMAP"), _T("BaseName"), m_strBaseName, m_strMMInfoFile);

			// データ数を保存
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

			// バージョンを取得
			GetPrivateProfileString(_T("MEMMAP"), _T("Version"), _T("0.00"), wkbuf, _countof(wkbuf), m_strMMInfoFile);
			m_strVersion = wkbuf;

			// ベースファイルを取得
			GetPrivateProfileString(_T("MEMMAP"), _T("BaseName"), _T(""), wkbuf, _countof(wkbuf), m_strMMInfoFile);
			m_strBaseName = wkbuf;

			// データ数を取得
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
