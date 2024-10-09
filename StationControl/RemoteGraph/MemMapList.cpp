
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
//	@brief	メモリマップリストの削除
//	@param	
//	@return	bool	結果
//============================================================================
bool CMemMapList::DeleteMemMapList()
{
//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("DeleteMemMapList"), _T("Start"), _T(""), nLogEx::debug);

	vector<CMemMapData>	tmp_listMMData(0);

	// リスト数を確認
	if( m_listMMData.size() == 0 )
	{
//		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("DeleteMemMapList"), _T("End"), _T("No data."), nLogEx::debug);
		return true;
	}

	// メモリマップデータファイルをクローズ
	for( UINT i = 0; i < m_listMMData.size(); i++ )
		m_listMMData[i].CloseMMDataFile();

	// メモリマップデータリストを完全に削除するためローカルリストに交換して削除
	m_listMMData.swap(tmp_listMMData);

	// パラメータをクリア
	ClearParameter();

//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("DeleteMemMapList"), _T("End"), _T(""), nLogEx::debug);

	return true;
}

//============================================================================
//	@brief	測定データ保存の開始処理
//	@param	strMMInfoFile	メモリマップ情報ファイル名
//	@param	nView		ビュー番号
//	@param	nData		データ番号
//	@param	nStartFile	ファイルの開始番号
//	@return	bool	結果
//============================================================================
bool CMemMapList::WriteInitialization(int nView, int nData, UINT nStartFile)
{
	CString	strBaseName;

//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteInitialization"), _T("Start"), _T(""), nLogEx::debug);

	// メモリマップリストを削除
	if( !DeleteMemMapList() )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteInitialization"), _T("Error"), _T("DeleteMemMapList failed."), nLogEx::error);
		return false;
	}

	// パラメータを初期化
	m_nView = nView;
	m_nData = nData;
	m_startFile = nStartFile;
	m_posDataRWPos = 0;

	// メモリマップ情報ファイル名を作成
	CString	strMMInfoFile;
	if( nView == -1 )
		strMMInfoFile.Format(_T("%s\\%s.cfg"), (LPCTSTR)theApp.GetAppDataPath(), REALTIME_FNAME);
	else
		strMMInfoFile.Format(_T("%s\\%s%d.cfg"), (LPCTSTR)theApp.GetAppDataPath(), ANALYSIS_FNAME, nView + 1);

	// ベースファイル名を設定
	wchar_t	pFile[MAX_PATH];
	wcscpy_s(pFile, _countof(pFile), (LPCTSTR)strMMInfoFile);
	PathRemoveExtension((LPWSTR)pFile);
	PathStripPath((LPWSTR)pFile);
	m_strBaseName.Format(_T("%s_data%02d"), pFile, nData);

	// メモリマップ情報ファイル名を設定
	m_strMMInfoFile = strMMInfoFile;

	// ステータスを更新
	if( nView == -1 )
		m_eMMListStatus = eMMListStatus_WriteOpenRealTime;
	else
		m_eMMListStatus = eMMListStatus_WriteOpenAnalysis;

//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteInitialization"), _T("End"), _T(""), nLogEx::debug);

	return true;
}

//============================================================================
//	@brief	測定データ保存の終了処理
//	@param	
//	@return	bool		結果
//============================================================================
bool CMemMapList::WriteFinalization()
{
//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteFinalization"), _T("Start"), _T(""), nLogEx::debug);

	// ステータスを確認
	if( (m_eMMListStatus != eMMListStatus_WriteOpenRealTime) && (m_eMMListStatus != eMMListStatus_WriteOpenAnalysis) )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteFinalization"), _T("Error"), _T("Status is no write open."), nLogEx::error);
		return false;
	}

	// 保存した測定データ数を設定
	m_countData = m_posDataRWPos;

#if 0
	// メモリマップ情報ファイルにパラメータを保存
	SaveMMInfoFile();
#endif

	// メモリマップデータファイルをクローズ
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

	// ステータスをクリア
	m_eMMListStatus = eMMListStatus_None;

//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteFinalization"), _T("End"), _T(""), nLogEx::debug);

	return true;
}

//============================================================================
//	@brief	データの書き込み
//	@param	oSpaceData	データ
//	@return	bool		結果
//============================================================================
bool CMemMapList::WriteData(stSpaceData& oSpaceData)
{
//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteData"), _T("Start"), _T(""), nLogEx::debug);

	// ステータスを確認
	if( (m_eMMListStatus != eMMListStatus_WriteOpenRealTime) && (m_eMMListStatus != eMMListStatus_WriteOpenAnalysis) )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteData"), _T("Error"), _T("Status is no write open."), nLogEx::error);
		return false;
	}

	// 書き込みデータ位置を確認
	if( (m_posDataRWPos % GetMaxSizePerMMData()) == 0 )
	{
		CString	strMMDataFile;
		CMemMapData	newMMData;

		// メモリマップデータファイルをクローズ
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

		// メモリマップデータファイル名を作成
		strMMDataFile.Format(_T("%s\\%s_%d.spd"), (LPCTSTR)theApp.GetAppDataPath(), (LPCTSTR)m_strBaseName, (UINT)(m_posDataRWPos / GetMaxSizePerMMData()) + m_startFile + 1);
		newMMData.SetMMDataFileName(strMMDataFile);

		// メモリマップデータファイルをオープン
		if( !newMMData.OpenMMDataFile(eMMDataType_Data, false) )
		{
			CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteData"), _T("Error"), _T("Open error."), nLogEx::error);
			return false;
		}

		// 新しいメモリマップデータをリストに追加
		m_listMMData.push_back(newMMData);
	}

	// 書き込み中のメモリマップデータを取得
	unsigned int	posTmp = (unsigned int)(m_posDataRWPos / GetMaxSizePerMMData());
	if( posTmp >= m_listMMData.size() )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteData"), _T("Error"), _T("Access position over."), nLogEx::error);
		return false;
	}
	CMemMapData&	curMMData = m_listMMData[posTmp];

	// 書き込みデータ位置を更新
	m_posDataRWPos++;

	// 有効データを書き込む
	if( !curMMData.WriteMMDataFile(oSpaceData) )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteData"), _T("Error"), _T("Write error."), nLogEx::error);
		return false;
	}

//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("WriteData"), _T("End"), _T(""), nLogEx::debug);

	return true;
}

//============================================================================
//	@brief	測定データ展開の開始処理
//	@param	nView			ビュー番号
//	@param	nData			データ番号
//	@param	nStartFile		ファイルの開始番号
//	@param	nCountData		データ数
//	@return	bool			結果
//============================================================================
bool CMemMapList::ReadInitialization(int nView, int nData, UINT nStartFile, unsigned __int64 nCountData)
{
//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadInitialization"), _T("Start"), _T(""), nLogEx::debug);

	// メモリマップリストを削除
	if( !DeleteMemMapList() )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadInitialization"), _T("Error"), _T("DeleteMemMapList failed."), nLogEx::error);
		return false;
	}

	// パラメータを初期化
	m_nView = nView;
	m_nData = nData;
	m_startFile = nStartFile;
	m_posDataRWPos = 0;

	// メモリマップ情報ファイル名を作成
	CString	strMMInfoFile;
	if( nView == -1 )
		strMMInfoFile.Format(_T("%s\\%s.cfg"), (LPCTSTR)theApp.GetAppDataPath(), REALTIME_FNAME);
	else
		strMMInfoFile.Format(_T("%s\\%s%d.cfg"), (LPCTSTR)theApp.GetAppDataPath(), ANALYSIS_FNAME, nView + 1);

	// パスからファイル名を取り除く
	wchar_t	pPath[MAX_PATH];
	wcscpy_s(pPath, _countof(pPath), (LPCTSTR)strMMInfoFile);
	PathRemoveFileSpec((LPWSTR)pPath);

#if 0
	// メモリマップ情報ファイルからパラメータを設定
	LoadMMInfoFile();
#else
	// ベースファイル名を設定
	wchar_t	pFile[MAX_PATH];
	wcscpy_s(pFile, _countof(pFile), (LPCTSTR)strMMInfoFile);
	PathRemoveExtension((LPWSTR)pFile);
	PathStripPath((LPWSTR)pFile);
	m_strBaseName.Format(_T("%s_data%02d"), pFile, nData);

	// メモリマップ情報ファイル名を設定
	m_strMMInfoFile = strMMInfoFile;

	// データ数を取得
	m_countData = nCountData;
#endif

	// メモリマップデータを作成
	for( unsigned __int64 i = 0; i < m_countData; i += GetMaxSizePerMMData() )
	{
		CString	strMMDataFile;

		// メモリマップデータファイル名を作成
		strMMDataFile.Format(_T("%s\\%s_%d.spd"), pPath, (LPCTSTR)m_strBaseName, (UINT)(i / GetMaxSizePerMMData()) + m_startFile + 1);

		{
			CMemMapData	newMMData;

			// メモリマップデータファイル名を設定
			newMMData.SetMMDataFileName(strMMDataFile);

			// 新しいメモリマップデータをリストに追加
			m_listMMData.push_back(newMMData);
		}
	}

	// ステータスを更新
	if( nView == -1 )
		m_eMMListStatus = eMMListStatus_ReadOpenRealTime;
	else
		m_eMMListStatus = eMMListStatus_ReadOpenAnalysis;

//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadInitialization"), _T("End"), _T(""), nLogEx::debug);

	return true;
}

//============================================================================
//	@brief	測定データ展開の終了処理
//	@param	
//	@return	bool		結果
//============================================================================
bool CMemMapList::ReadFinalization()
{
//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadFinalization"), _T("Start"), _T(""), nLogEx::debug);

	// ステータスを確認
	if( (m_eMMListStatus != eMMListStatus_ReadOpenRealTime) && (m_eMMListStatus != eMMListStatus_ReadOpenAnalysis) )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadFinalization"), _T("Error"), _T("Status is no read open."), nLogEx::error);
		return false;
	}

	// メモリマップデータファイルをクローズ
	for( UINT i = 0; i < m_listMMData.size(); i++ )
		m_listMMData[i].CloseMMDataFile();

	// パラメータをクリア
	ClearParameter();

	// ステータスをクリア
	m_eMMListStatus = eMMListStatus_None;

//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadFinalization"), _T("End"), _T(""), nLogEx::debug);

	return true;
}

//============================================================================
//	@brief	データの読み込み
//	@param	nStart			取得するデータの開始番号
//	@param	nEnd			取得するデータの終了番号
//	@param	vCompData		データの格納アドレスへの参照
//	@return	bool			結果
//============================================================================
bool CMemMapList::ReadData(unsigned __int64 nStart, unsigned __int64 nEnd, vector<stSpaceData>& vSpaceData)
{
	unsigned int	nCurIndex, nCurFIndex, nCurBIndex, nPreIndex, nPreFIndex, nPreBIndex, nMinIndex, nMaxIndex;
	UINT	nOffset = 0;

//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadData"), _T("Start"), _T(""), nLogEx::debug);

	// ステータスを確認
	if( (m_eMMListStatus != eMMListStatus_ReadOpenRealTime) && (m_eMMListStatus != eMMListStatus_ReadOpenAnalysis) )
	{
		CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadData"), _T("Error"), _T("Status is no read open."), nLogEx::error);
		return false;
	}

	// 指定された通し番号を確認
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
		// 指定された通し番号からメモリマップデータのインデックスを取得
		nCurIndex = (unsigned int)(nCur / GetMaxSizePerMMData());
		if( nCurIndex >= m_listMMData.size() )
		{
			CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadData"), _T("Error"), _T("Index is invalid."), nLogEx::error);
			return false;
		}

		// オープンした前後のメモリマップデータファイルのインデックスを取得
		nCurFIndex = ((nCurIndex - NUM_AROUND_OPEN_MMDATA) > nCurIndex) ? 0 : (nCurIndex - NUM_AROUND_OPEN_MMDATA);
		nCurBIndex = (((nCurIndex + NUM_AROUND_OPEN_MMDATA) < nCurIndex) || ((nCurIndex + NUM_AROUND_OPEN_MMDATA) > ((unsigned int)m_listMMData.size() - 1))) ? ((unsigned int)m_listMMData.size() - 1) : (nCurIndex + NUM_AROUND_OPEN_MMDATA);

		// 元々オープン中のメモリマップデータのインデックスを取得
		nPreIndex = (unsigned int)(m_posDataRWPos / GetMaxSizePerMMData());

		// 元々オープン中の前後のメモリマップデータファイルのインデックスを取得
		nPreFIndex = ((nPreIndex - NUM_AROUND_OPEN_MMDATA) > nPreIndex) ? 0 : (nPreIndex - NUM_AROUND_OPEN_MMDATA);
		nPreBIndex = (((nPreIndex + NUM_AROUND_OPEN_MMDATA) < nPreIndex) || ((nPreIndex + NUM_AROUND_OPEN_MMDATA) > ((unsigned int)m_listMMData.size() - 1))) ? ((unsigned int)m_listMMData.size() - 1) : (nPreIndex + NUM_AROUND_OPEN_MMDATA);

		// 最小／最大インデックスを取得
		nMinIndex = min(nCurFIndex, nPreFIndex);
		nMaxIndex = max(nCurBIndex, nPreBIndex);

		// メモリマップデータファイルをオープン／クローズ
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

		// データを読み込む
		if( !m_listMMData[nCurIndex].ReadMMDataFile(nCur, nTmpEnd, vSpaceData, nOffset) )
		{
			CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadData"), _T("Error"), _T("Read error."), nLogEx::error);
			return false;
		}

		// 読み込みデータ位置を更新
		m_posDataRWPos = nTmpEnd;

		nCur = nTmpEnd + 1;
		nOffset = (UINT)(nCur - nStart);
	}

//	CLogTraceEx::Write(_T(""), _T("CMemMapList"), _T("ReadData"), _T("End"), _T(""), nLogEx::debug);

	return true;
}
