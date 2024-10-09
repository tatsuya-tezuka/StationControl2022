
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
//	@brief	メモリマップデータファイルをマッピング
//	@param	nMMDataMapView	メモリマップデータファイルマップビューのサイズ
//	@return	bool	結果
//============================================================================
bool CMemMapData::MappingUpdateMMDataFile(DWORD nMMDataMapView)
{
	LPSTR	pCur;
	bool	bRes = true;

//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("MappingUpdateMMDataFile"), _T("Start"), _T(""), nLogEx::debug);

	// メモリマップデータファイルマップビューを確認
	if( (pCur = (LPSTR)m_pMMDataMapView) != NULL )
	{
		while( nMMDataMapView )
		{
			// バイト数格納分のサイズを確認
			if( nMMDataMapView < sizeof(unsigned long) )
			{
				bRes = false;
				break;
			}

			// バイト数を取得
			unsigned long	nData = *((unsigned long *)pCur);

			// バイト数分のサイズを確認
			if( nMMDataMapView < nData )
			{
				bRes = false;
				break;
			}

			// データの先頭ポインタをリストに追加
			m_listMMDataMap.push_back(pCur);

			// ポインタとサイズを更新
			pCur += nData;
			nMMDataMapView -= nData;
		}
	}

//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("MappingUpdateMMDataFile"), _T("End"), _T(""), nLogEx::debug);

	return bRes;
}

//============================================================================
//	@brief	メモリマップデータファイルのオープン
//	@param	typeMMData	メモリマップデータタイプ
//	@param	flgRead		リードオープンフラグ
//	@return	bool	結果
//============================================================================
bool CMemMapData::OpenMMDataFile(eMMDataType typeMMData, bool flgRead/* = true*/)
{
//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("OpenMMDataFile"), _T("Start"), _T(""), nLogEx::debug);

	// メモリマップデータファイル名を確認
	if( m_strMMDataFile.IsEmpty() )
		return false;

	// メモリマップデータファイルハンドルを確認
	if( m_hMMDataFile != INVALID_HANDLE_VALUE )
		return false;

	TRY
	{
		CStringA	strMMDataFile = (CT2A)m_strMMDataFile;

		// メモリマップデータファイルタイプを設定
		m_eMMDataType = typeMMData;

		// メモリマップデータファイルをオープン
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

			// ファイルサイズを取得
			dwLow = ::GetFileSize(m_hMMDataFile, &dwHigh);
			if( (dwLow == -1) && (GetLastError() != NO_ERROR) )
			{
				CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("OpenMMDataFile"), _T("Error"), _T("GetFileSize error."), nLogEx::error);
				AfxThrowUserException();
			}

			// ファイルマッピングオブジェクトを作成
			m_hMMDataObject = ::CreateFileMapping(m_hMMDataFile, NULL, PAGE_READONLY,  0, dwLow, NULL);
			if( m_hMMDataObject == NULL )
			{
				CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("OpenMMDataFile"), _T("Error"), _T("CreateFileMapping error."), nLogEx::error);
				AfxThrowUserException();
			}

			// アドレス空間にファイルのビューをマップ
			m_pMMDataMapView = ::MapViewOfFile(m_hMMDataObject, FILE_MAP_READ, 0, 0, dwLow);
			if( m_pMMDataMapView == NULL )
			{
				CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("OpenMMDataFile"), _T("Error"), _T("MapViewOfFile error."), nLogEx::error);
				AfxThrowUserException();
			}

			// メモリマップデータファイルをマップ
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

		// メモリマップデータマップリストを削除
		if( m_listMMDataMap.size() > 0 )
		{
			vector<LPVOID>	tmp_listMMDataMap(0);

			m_listMMDataMap.swap(tmp_listMMDataMap);

			m_listMMDataMap.clear();
		}

		// アドレス空間のファイルビューをアンマップ
		if( m_pMMDataMapView != NULL )
		{
			::UnmapViewOfFile(m_pMMDataMapView);

			m_pMMDataMapView = NULL;
		}

		// ファイルマッピングオブジェクトをクローズ
		if( m_hMMDataObject != NULL )
		{
			::CloseHandle(m_hMMDataObject);

			m_hMMDataObject = NULL;
		}

		// メモリマップデータファイルをクローズ
		if( m_hMMDataFile != INVALID_HANDLE_VALUE )
		{
			::CloseHandle(m_hMMDataFile);

			m_hMMDataFile = INVALID_HANDLE_VALUE;
		}

		// メモリマップデータファイルタイプをクリア
		m_eMMDataType = eMMDataType_None;

		return false;
	}
	END_CATCH_ALL

//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("OpenMMDataFile"), _T("End"), _T(""), nLogEx::debug);

	return true;
}

//============================================================================
//	@brief	メモリマップデータファイルのクローズ
//	@param	
//	@return	
//============================================================================
void CMemMapData::CloseMMDataFile()
{
//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("CloseMMDataFile"), _T("Start"), _T(""), nLogEx::debug);

	// メモリマップデータマップリストを削除
	if( m_listMMDataMap.size() > 0 )
	{
		vector<LPVOID>	tmp_listMMDataMap(0);

		m_listMMDataMap.swap(tmp_listMMDataMap);

		m_listMMDataMap.clear();
	}

	// アドレス空間のファイルビューをアンマップ
	if( m_pMMDataMapView != NULL )
	{
		::UnmapViewOfFile(m_pMMDataMapView);

		m_pMMDataMapView = NULL;
	}

	// ファイルマッピングオブジェクトをクローズ
	if( m_hMMDataObject != NULL )
	{
		::CloseHandle(m_hMMDataObject);

		m_hMMDataObject = NULL;
	}

	// メモリマップデータファイルをクローズ
	if( m_hMMDataFile != INVALID_HANDLE_VALUE )
	{
		::CloseHandle(m_hMMDataFile);

		m_hMMDataFile = INVALID_HANDLE_VALUE;
	}

	// メモリマップデータファイルタイプをクリア
	m_eMMDataType = eMMDataType_None;

//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("CloseMMDataFile"), _T("End"), _T(""), nLogEx::debug);
}

//============================================================================
//	@brief	データをメモリマップデータファイルに書き込む
//	@param	oSpaceData	データ
//	@return	bool		結果
//============================================================================
bool CMemMapData::WriteMMDataFile(stSpaceData& oSpaceData)
{
//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("WriteMMDataFile"), _T("Start"), _T(""), nLogEx::debug);

	// メモリマップデータファイルハンドルを確認
	if( m_hMMDataFile == INVALID_HANDLE_VALUE )
		return false;

	TRY
	{
		DWORD	nWritten;
		DWORD	nULong;

		// バイト数(４)
		nULong = sizeof(unsigned long) + sizeof(stSpaceData);
		if( ::WriteFile(m_hMMDataFile, &nULong, sizeof(unsigned long), &nWritten, NULL) == 0 )
			AfxThrowUserException();

		// 測定時刻(１６)
		if( ::WriteFile(m_hMMDataFile, &oSpaceData.systime, sizeof(SYSTEMTIME), &nWritten, NULL) == 0 )
			AfxThrowUserException();

		// ステータス(２)
		if( ::WriteFile(m_hMMDataFile, &oSpaceData.status, sizeof(USHORT), &nWritten, NULL) == 0 )
			AfxThrowUserException();

		// 圧縮数(２)
		if( ::WriteFile(m_hMMDataFile, &oSpaceData.mmPair.cnt, sizeof(USHORT), &nWritten, NULL) == 0 )
			AfxThrowUserException();

		// 最大値(８)
		if( ::WriteFile(m_hMMDataFile, &oSpaceData.mmPair.max, sizeof(double), &nWritten, NULL) == 0 )
			AfxThrowUserException();

		// 最小値(８)
		if( ::WriteFile(m_hMMDataFile, &oSpaceData.mmPair.min, sizeof(double), &nWritten, NULL) == 0 )
			AfxThrowUserException();
	}
	CATCH_ALL(e)
	{
		CString	sError;
		sError.Format(_T("GetLastError = %d"), ::GetLastError());
		CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("WriteMMDataFile"), _T("Exception"), sError, nLogEx::error);

		// メモリマップデータファイルをクローズ
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
//	@brief	データをメモリマップデータファイルから読み込む
//	@param	nStart			取得するデータの開始番号
//	@param	nEndl			取得するデータの終了番号
//	@param	vCompData		データの格納アドレスへの参照
//	@param	nOffset			データの格納位置
//	@return	bool			結果
//============================================================================
bool CMemMapData::ReadMMDataFile(unsigned __int64 nStart, unsigned __int64 nEnd, vector<stSpaceData>& vSpaceData, UINT nOffset)
{
	LPSTR	pCur;

	//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("ReadMMDataFile"), _T("Start"), _T(""), nLogEx::debug);

	// メモリマップデータファイルハンドルを確認
	if( m_hMMDataFile == INVALID_HANDLE_VALUE )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("ReadMMDataFile"), _T("Error"), _T("File handle is none."), nLogEx::error);
		return false;
	}

	// メモリマップデータマップリストを確認
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

		// 指定された通し番号のデータへのポインタを取得
		if( (pCur = (LPSTR)m_listMMDataMap[(UINT)(nCur % MAX_SIZE_PER_MMDATA)]) == NULL )
		{
			CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("ReadMMDataFile"), _T("Error"), _T("Pointer is null."), nLogEx::error);
			return false;
		}

		// バイト数(４)
		nBytes = *((unsigned long *)pCur);
		pCur += sizeof(unsigned long);	nBytes -= sizeof(unsigned long);

		// 測定時刻(１６)
		memcpy(&(vSpaceData[UINT(nCur - nStart) + nOffset].systime), ((SYSTEMTIME *)pCur), sizeof(SYSTEMTIME));
		pCur += sizeof(SYSTEMTIME);	nBytes -= sizeof(SYSTEMTIME);

		// ステータス(２)
		memcpy(&(vSpaceData[UINT(nCur - nStart) + nOffset].status), ((USHORT *)pCur), sizeof(USHORT));
		pCur += sizeof(USHORT);	nBytes -= sizeof(USHORT);

		// 圧縮数(２)
		memcpy(&(vSpaceData[UINT(nCur - nStart) + nOffset].mmPair.cnt), ((USHORT *)pCur), sizeof(USHORT));
		pCur += sizeof(USHORT);	nBytes -= sizeof(USHORT);

		// 最大値(８)
		memcpy(&(vSpaceData[UINT(nCur - nStart) + nOffset].mmPair.max), ((double *)pCur), sizeof(double));
		pCur += sizeof(double);	nBytes -= sizeof(double);

		// 最小値(８)
		memcpy(&(vSpaceData[UINT(nCur - nStart) + nOffset].mmPair.min), ((double *)pCur), sizeof(double));
		pCur += sizeof(double);	nBytes -= sizeof(double);

		// 残りのバイト数を確認
		if( nBytes != 0 )
		{
			CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("ReadMMDataFile"), _T("Error"), _T("Bytes is left over."), nLogEx::error);
			return false;
		}
	}

//	CLogTraceEx::Write(_T(""), _T("CMemMapData"), _T("ReadMMDataFile"), _T("End"), _T(""), nLogEx::debug);

	return true;
}
