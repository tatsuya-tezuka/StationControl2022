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
/*! プロセス間通信：パイプクラス

-# アラーム共有メモリの作成

@param	bServer		trueの場合サーバー、falseの場合クライアント
@param	nStation	局番号

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

	// プロセスIDの取得
	m_dwProcessId = ::GetCurrentProcessId();

	// 共有メモリ名
	CString strShareMemory, strShareMemoryMutex, strShareMemorySend, strShareMemoryRecv;
	strShareMemory.Format(_T("%s%d"), (LPCTSTR)mShareMemoryString, nStation);
	strShareMemoryMutex.Format(_T("%s%d"), (LPCTSTR)mShareMemoryMutex, nStation);
	strShareMemorySend.Format(_T("%s%d"), (LPCTSTR)mShareMemorySend, nStation);
	strShareMemoryRecv.Format(_T("%s%d"), (LPCTSTR)mShareMemoryRecv, nStation);

	if (mServer == true)
	{
		// サーバの場合
		// 共有メモリ用ファイルマッピングオブジェクトの作成
		m_hMap = ::CreateFileMapping((HANDLE)INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(stAlarmItem)*mAlarmItemMax, strShareMemory);
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			if (theApp.GetDebugMode())
			{
				AfxMessageBox(_T("既にアラームサーバーとして起動されています。"));
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
		// クライアントの場合
		// サーバが作成済みの共有メモリを開く
		m_hMap = ::OpenFileMapping(FILE_MAP_WRITE, FALSE, strShareMemory);
		if (m_hMap == NULL || GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			// 端末アプリが持っている共有メモリを開けなかった
			if (theApp.GetDebugMode())
			{
				AfxMessageBox(_T("アラームサーバーが起動されていません。\n共有メモリを開く処理を中止します。"));
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

	// ここまでくればサーバーあるいはクライアントして起動されている

	// 
	// 呼び出し側プロセスのアドレス空間に、ファイルのビューをマップします
	if (m_hMap != NULL)
		m_pAlarmBuf = (char*)::MapViewOfFile(m_hMap, FILE_MAP_WRITE, 0, 0, sizeof(stAlarmItem)*mAlarmItemMax);
	else
		m_pAlarmBuf = NULL;

	m_hSend = ::CreateEvent(NULL, FALSE, FALSE, strShareMemorySend);
	m_hRecv = ::CreateEvent(NULL, FALSE, FALSE, strShareMemoryRecv);
	
	return true;
}

/*============================================================================*/
/*! プロセス間通信：パイプクラス

-# アラーム共有メモリの作成

@param	bServer		trueの場合サーバー、falseの場合クライアント

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
/*! プロセス間通信：パイプクラス

-# アラーム共有メモリへの書き込み

@param		stAlarmBuf		共有メモリに書き込むデータ
@param		count			stAlarmItem構造体のカウント

@retval
*/
/*============================================================================*/
void CShareAlarmIf::Send(stAlarmItem* stAlarmBuf, UINT count)
{
	// サーバーとの排他制御
	if (::WaitForSingleObject(m_hMutex, 500) == WAIT_OBJECT_0){
		// 共有メモリバッファに書き込む
		memset(&m_pAlarmBuf[0], 0, sizeof(stAlarmItem)*mAlarmItemMax);
		memcpy(&m_pAlarmBuf[0], stAlarmBuf, sizeof(stAlarmItem)*count);

		// 書き込み終了イベント
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
/*! プロセス間通信：パイプクラス

-# アラーム共有メモリからの読み込み（受信は全領域を取得する必要がある）

@param		stAlarmBuf		共有メモリに書き込むデータ（最大mAlarmItemMax数分）
@param		count			stAlarmItem構造体のカウント

@retval
*/
/*============================================================================*/
bool CShareAlarmIf::Recv(stAlarmItem* stAlarmBuf, UINT count)
{
	if (::WaitForSingleObject(m_hSend, 50) == WAIT_OBJECT_0) {
		// 共有メモリバッファから読み込む
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
/*! プロセス間通信：パイプクラス

-# アラームファイルを構造体に変換する

@param		strFileName		アラームファイル
@param		list			アラーム構造体

@retval		int		局番号
*/
/*============================================================================*/
int CShareAlarmIf::ConvertAlarmData(CString strFileName, vector<stAlarmItem>& list)
{
	int ret = -1;

	// ファイル名から局判定
	CString fname = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind(_T('\\')) - 1);
	for (int sta = 0; sta < sizeof(mStationFileString) / sizeof(mStationFileString[0]); sta++)
	{
		if (fname.Find(mStationFileString[sta]) == 0)
		{
			ret = sta;
			break;
		}
	}

	// ファイル読み込み
	FILE* fp;
	if (_wfopen_s(&fp, strFileName, _T("r")) != 0 || fp == NULL){
		return -1;
	}

	TRY
	{
		char    szline[2048];	    /* レコード読み込みバッファ       */
		CString cbuf, str;
		stAlarmItem data;
		memset(&data, 0, sizeof(stAlarmItem));
		// 先頭の更新日時を取得
		if (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) == NULL) {
			fclose(fp);
			return -1;
		}
		// 更新日時
		sprintf_s(data.updatetime, mAlarmItemTimeMax, "%s", (LPCSTR)CStringA(szline));

		while (!feof(fp) && list.size() < mAlarmItemMax){
			/* １行読み込み */
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
					// 局名
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
					// 送信元
					data.line.sender = 0xffffffff;
					for (pos = 0; pos < sizeof(mAlarmEquipmentString) / sizeof(mAlarmEquipmentString[0]); pos++){
						if (str.CompareNoCase(mAlarmEquipmentString[pos]) == 0){
							data.line.sender = pos;
							break;
						}
					}
					break;
				case	2:
					// ステータス
					data.line.status = 0xffffffff;
					for (pos = 0; pos < sizeof(mAlarmStatusString) / sizeof(mAlarmStatusString[0]); pos++){
						if (str.CompareNoCase(mAlarmStatusString[pos]) == 0){
							data.line.status = pos;
							break;
						}
					}
					break;
				case	3:
					// 監視項目名
					sprintf_s(data.line.monname, mAlarmItemNameMax, "%s", (LPCSTR)CStringA(str));
					break;
				case	4:
					// 監視項目
					sprintf_s(data.line.monval, mAlarmItemValMax, "%s", (LPCSTR)CStringA(str));
					break;
				default:
					continue;
				}
			}

			// データチェック
			if (data.line.station == 0xffffffff || data.line.sender == 0xffffffff || data.line.status == 0xffffffff){
				// エラー
				continue;
			}

			list.push_back(data);
		}

		// リストが空の場合、時刻情報のみリスト追加する。
		// 行番号は-1を指定する。
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
