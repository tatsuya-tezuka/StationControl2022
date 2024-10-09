// DataCollection.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "DataCollection.h"
#include <locale.h>
#include "EngValCnv.h"

// timeGetTime使用
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

// CDataCollection

CDataCollection::CDataCollection()
{
	mpCollectThread = NULL;

	for (int i = 0; i < sizeof(m_angle) / sizeof(m_angle[0]); ++i) {
		m_angle[i] = rand() * 360 / RAND_MAX;
	}
	for (int i = 0; i < eStation_MAX; i++)
	{
		mDataNameList[i].clear();
	}

	for (int i = 0; i < eStation_MAX; i++)
	{
		mGraphSigIndexNum[i] = 0;
	}

#ifdef _NOCOMM
	mOperationHistFile.m_hFile = INVALID_HANDLE_VALUE;
	mControlHistFile.m_hFile = INVALID_HANDLE_VALUE;
	mEquipmentHistFile.m_hFile = INVALID_HANDLE_VALUE;
	mEventHistFile.m_hFile = INVALID_HANDLE_VALUE;
#endif

	m_nHistID54 = 0;
//#ifdef _DEBUG
//	m_shortCycleTime = 0;
//	m_integtime = 0;
//#endif
}

CDataCollection::~CDataCollection()
{
}


/*============================================================================*/
/*! データ収集

-# データIDの取得

@param
@retval

*/
/*============================================================================*/
void CDataCollection::SetKanshiName()
{
	// 端末番号取得する
	CString strTermNo;
	UINT nTermNo = theApp.GetTermNo();
	if (nTermNo > 0)
		nTermNo = nTermNo - 1;
	strTermNo.Format(_T("%02u"), nTermNo);

	int i, j, k;
	int index[eStation_MAX];
	// 衛星捕捉用の局.設備.項目名をリストに追加
	for (i = 0; i < eStation_MAX; i++)
	{
		index[i] = 1;
		for (j = 0; j < eTrendWndType_Num; j++)
		{
			for (k = 0; k < MAX_TDATA; k++)
			{
				if (mListSatelliteCaptureSig[i][j][k].strSigName.IsEmpty())
					break;
				if (mListSatelliteCaptureSig[i][j][k].strSigName.Compare(UDSC64_AGC_XRX2_INTEG_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(UDSC64_AGC_XXTCR_INTEG_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(XDSC54_AGC_DSNMODEM_INTEG_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC34_AGC_INTEG_SIG) == 0)
				{
					// 局内の場所に応じて監視名の末尾を変える
					mListSatelliteCaptureSig[i][j][k].strSigName = ReplaceAreaKanshiName(mListSatelliteCaptureSig[i][j][k].strSigName) + strTermNo;
				}
				mDataNameList[i].insert(map<DWORD, CString>::value_type((j << 16) | k, mListSatelliteCaptureSig[i][j][k].strSigName));

				// グラフの監視マップインデックスに監視名が未セットならばインデックスセットする
				if (mGraphSigIndexMap[i][mListSatelliteCaptureSig[i][j][k].strSigName] == 0)
				{
					mGraphSigIndexMap[i][mListSatelliteCaptureSig[i][j][k].strSigName] = index[i];
					index[i]++;
				}
			}
		}
	}

	int pre_j = j;
	int pre_k = k;

	// サマリ用の局.設備.項目名をリストに追加
	for (i = 0; i < eStation_MAX; i++)
	{
		for (j = 0; j < eSummaryWndSetType_Num; j++)
		{
			for (k = 0; k < MAX_TDATA; k++)
			{
				if (mListSummarySig[i][j][k].strSigName.IsEmpty())
					break;
				mDataNameList[i].insert(map<DWORD, CString>::value_type(((j + pre_j) << 16) | (k + pre_k), mListSummarySig[i][j][k].strSigName));

				// グラフの監視マップインデックスに監視名が未セットならばインデックスセットする
				if (mGraphSigIndexMap[i][mListSummarySig[i][j][k].strSigName] == 0)
				{
					mGraphSigIndexMap[i][mListSummarySig[i][j][k].strSigName] = index[i];
					index[i]++;
				}
			}
		}
	}

	for (i = 0; i < eStation_MAX; i++)
	{
		mGraphSigIndexNum[i] = (int)mGraphSigIndexMap[i].size();
	}
}


/*============================================================================*/
/*! データ収集

-# データIDの取得

@param
@retval

*/
/*============================================================================*/
DWORD CDataCollection::ConvertNameToId(UINT nStation, CString name)
{
	map<DWORD, CString>::iterator itr;
	for (itr = mDataNameList[nStation].begin(); itr != mDataNameList[nStation].end(); itr++){
		if ((*itr).second == name){
			return (*itr).first;
		}
	}
	return 0xffffffff;
}

/*============================================================================*/
/*! データ収集

-# データ名称の取得

@param
@retval

*/
/*============================================================================*/
CString CDataCollection::ConvertIdToName(UINT nStation, DWORD id)
{
	map<DWORD, CString>::iterator itr = mDataNameList[nStation].find(id);
	if (itr == mDataNameList[nStation].end())
		return _T("");
	return (*itr).second;
}


/*============================================================================*/
/*! データ収集

-# サーバデータをアプリ内データに再作成

@param	lpData		サーバデータ
@param	dwSize		バッファサイズ
@param	nStation	局番号
@retval void
*/
/*============================================================================*/
void CDataCollection::CreateSpaceData(LPBYTE lpData, DWORD dwSize, UINT nStation)
{
	CGraphMutex::Lock(eData);

	try
	{
		stServerItemData *pdata = (stServerItemData*)(lpData + sizeof(stCommandHeader));
		dwSize -= sizeof(stCommandHeader);
		CSpaceData		data;
		stSpaceData		sData;
		int cnt = 0;
		for (DWORD i = 0; i + 1 < dwSize; i += sizeof(stServerItemData), pdata++)
		{
			cnt++;
			data.value.clear();
			memset(&sData, 0, sizeof(stSpaceData));
			CString strDataName = CString(pdata->name);
//			data.dataid = ConvertNameToId(nStation, strDataName);
			data.dataid = mGraphSigIndexMap[nStation][strDataName];
			if (data.dataid == 0)
				continue;
			data.dataname = strDataName;
			data.kind = pdata->kind;
			data.srcType = eSigSrcType_EngExchange;
			data.totalSize = 1;
			CCommBase::ConvertTime(&pdata->systime, &sData.systime);
			sData.idx = 0;
			sData.value = pdata->value;
			memcpy(sData.status, pdata->status, 8);
			sData.ppPair.max = sData.value;
			sData.ppPair.min = sData.value;
			sData.ppPair.count = 1;
			data.value.push_back(sData);
			// データを一時的に格納
			if (data.value.empty() == FALSE)
			{
				mSpaceTemp[nStation].push_back(data);
			}
		}
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);
}


/*============================================================================*/
/*! データ収集

-# サーバデータをアプリ内データに再作成

@param	data		サーバデータ
@param	nStation	局番号
@retval void
*/
/*============================================================================*/
void CDataCollection::CreateForecastData(stServerItemData* pdata, UINT nStation)
//#ifndef _ENGVALCNV_TREND
{
	CGraphMutex::Lock(eData);

	try
	{
		CSpaceData		data;
		stSpaceData		sData;
		data.value.clear();
		memset(&sData, 0, sizeof(stSpaceData));
		CString strDataName = CString(pdata->name);
//		data.dataid = ConvertNameToId(nStation, strDataName);
//		if (data.dataid != 0xffffffff)
		data.dataid = mGraphSigIndexMap[nStation][strDataName];
		if (data.dataid != 0)
		{
			data.dataname = strDataName;
			data.kind = pdata->kind;
			data.srcType = eSigSrcType_File;
			data.totalSize = 1;
			CCommBase::ConvertTime(&pdata->systime, &sData.systime);
			sData.idx = 0;
			sData.value = pdata->value;
			memcpy(sData.status, pdata->status, 8);
			sData.status[0] = 0;
			sData.ppPair.max = sData.value;
			sData.ppPair.min = sData.value;
			//			sData.ppPair.count = 0;
			sData.ppPair.count = 1;
			data.value.push_back(sData);
			// データを一時的に格納
			if (data.value.empty() == FALSE)
			{
				mSpaceTemp[nStation].push_back(data);
#ifdef _DEBUG
				{
					CTime ctime(sData.systime);
					CString msg;
					msg.Format(_T("add dataid : %s(%u) value(%f) at time(%I64d)"), (LPCTSTR)CString(pdata->name), data.dataid, sData.value, ctime.GetTime());
					CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateForecastData"), msg, _T("debug"), nLogEx::debug);
				}
#endif
			}
		}
#if _DEBUG
		else
		{
			CString msg;
			msg.Format(_T("dataid error : %s"), (LPCTSTR)CString(pdata->name));
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateForecastData"), msg, _T("debug"), nLogEx::debug);
		}
#endif
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);
}


/*============================================================================*/
/*! データ収集

-# 履歴情報の作成

@param	pUtf8Str			UTF-8

@retval	CString
*/
/*============================================================================*/
CString CDataCollection::UTF8_3CharsToSJis(char *pUtf8Str)
{
	wchar_t pUtfMsg[4];

	if (pUtf8Str[0] >= 0x00 && pUtf8Str[0] <= 0x7f){
		// 1バイト文字
		return _T("");
	}

	// UTF8 の３バイトをワイドキャラクタへ合成
	pUtfMsg[0] = ((pUtf8Str[0] & 0x0f) << 12)
		| ((pUtf8Str[1] & 0x3f) << 6)
		| (pUtf8Str[2] & 0x3f);
	pUtfMsg[1] = 0x0000;

	size_t wLen = 0;
	errno_t err = 0;

	//ロケール指定
	char* back = setlocale(LC_ALL, "japanese");
	//変換
	char	sjis[10];
	err = wcstombs_s(&wLen, sjis, 10, pUtfMsg, _TRUNCATE);
#ifndef _DEBUG
	setlocale(LC_ALL, back);
#endif
	if (err != 0)
	{
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CString msg;
		msg.Format(_T("Code Error : err(%d), 0x%02X 0x%02X 0x%02X"), err, pUtf8Str[0] & 0xFF, pUtf8Str[1] & 0xFF, pUtf8Str[2] & 0xFF);
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("UTF8_3CharsToSJis"), msg, _T("detail"), nLogEx::detail);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return _T("");
	}

	CString str(sjis);
	return str;
}
#ifdef _DEBUG
//#define _DEBUG_HISTORY 1
#endif
/*============================================================================*/
/*! データ収集

-# 履歴情報の作成

@param	pbuf		サーバー履歴データ

@retval
*/
/*============================================================================*/
void CDataCollection::CreateDataToHistory(PIPEMONITORACK* pbuf)
{
	DWORD	dwStart = timeGetTime();

	stHistoryData data;
	memset(&data, 0, sizeof(stHistoryData));

	// 該当局を調べる
	char* ptr = (char*)pbuf->buf;
	ptr += 0x0C;	// 送信元識別
	DWORD nStation = *((DWORD*)ptr);
	if (nStation == 0x010a0102) // UDSC64
		data.station = 0;
	else if (nStation == m_nHistID54) // XDSC54
		data.station = 1;
	else if (nStation == 0x010a0203) // USC34
		data.station = 2;
	else if (nStation == 0x010a0103) // USC20
		data.station = 3;
	else{
		if (nStation == 0) // 筑波局内
			data.station = (BYTE)theApp.GetSelectStation();
	}

	ptr = (char*)pbuf->buf;
	CEngValCnv::sdtp_msg_ex_t* pmsg = (CEngValCnv::sdtp_msg_ex_t*)pbuf->buf;

#ifdef _DEBUG_HISTORY
	const char sdtp[109] = {
		/*0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x89, 0x00, 0x00, 0x00, 0xB0, 0x30, 0x00, 0x01,
		0x01, 0x00, 0x00, 0x00, 0x09, 0x80, 0x05, 0x00,
		0x04, 0x00, 0x02, 0x00,*/ 0x41, 0x00, 0x00, 0x00,
		0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B,
		0x02, 0x01, 0x0A, 0x01, 0x00, 0x00, 0x02, 0x01,
		0x04, 0x01, 0x00, 0x00, 0x10, 0x03, 0x6D, 0x00,
		0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00,
		0x32, 0x30, 0x31, 0x37, 0x2D, 0x30, 0x33, 0x2D,
		0x31, 0x30, 0x20, 0x30, 0x35, 0x3A, 0x34, 0x31,
		0x3A, 0x33, 0x32, 0x20, 0x43, 0x30, 0x20, 0x28,
		0x30, 0x30, 0x30, 0x30, 0x39, 0x29, 0x20, 0x64,
		0x69, 0x20, 0xE3, 0x83, 0x86, 0xE3, 0x82, 0xB9,
		0xE3, 0x83, 0x88, 0xE3, 0x83, 0xA1, 0xE3, 0x83,
		0x83, 0xE3, 0x82, 0xBB, 0xE3, 0x83, 0xBC, 0xE3,
		0x82, 0xB8, 0xE3, 0x81, 0xA7, 0xE3, 0x81, 0x99,
		0x00,
	};
	memcpy(ptr, (char*)&sdtp[0], 109);
#endif

	CString strDate, strTime, strSend, strMsgid, strLevel, strMsg;

	ptr += 0x2C;

//	// 54m局以外の場合（リモート化）
//	if (data.station != eStation_Usuda642)
//	{
//		CString buf;
//		int item = 0;
//		CString temp;
//		buf.Empty();
//		while (1){
//			if (*ptr == 0x00){
//				strMsg = buf;
//				break;
//			}
//			if (*ptr == 0x20 && item < 5){
//				// 一つのアイテムが決定
//				switch (item){
//				case	0:
//					strDate = buf;
//					break;
//				case	1:
//					strTime = buf;
//					break;
//				case	2:
//					strSend = buf;
//					break;
//				case	3:
//					strMsgid = buf;
//					break;
//				case	4:
//					strLevel = buf;
//					break;
//				case	5:
//					strMsg = buf;
//					break;
//				}
//				ptr++;
//				item++;
//				buf.Empty();
//				continue;
//			}
//			if (*ptr >= 0x00 && *ptr <= 0x7F){
//				// ASCIIコード
//				buf += CString(char(*ptr));
//				ptr++;
//			}
//			else if ((*ptr & 0xE0) == 0xE0){
//				// 漢字
//				buf += UTF8_3CharsToSJis(ptr);
//				ptr += 3;
//			}
//			else{
//				// その他
//				//=====================================================//
//				//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
//				CString msg;
//				msg.Format(_T("Code Error : 0x%02X"), *ptr & 0xFF);
//				CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("detail"), nLogEx::detail);
//				//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
//				//=====================================================//
//				return;
//			}
//		}
//	}
//	// 54m局の場合（GREAT）
//	else
	{
		//電文よりメッセージ長を取得する(4ByteLSB) バッファが16384BYTEだったのでカット
		unsigned int msglen = ((BYTE)ptr[0x28] + ((BYTE)ptr[0x29] << 8) + ((BYTE)ptr[0x2A] << 16) + ((BYTE)ptr[0x2B] << 24)) % 0x4000;

		std::string stdbuf;
		int item = 0;
		CString temp;
		// メッセージ文字がメッセージ長になるまで繰り返す
		// メッセージ文字列例：2015-08-23 00:25:41 u6opr1 (5052) di CTRL.PRIV_STTN,ON,,ANS:OK
		// ①日付　②時間　③設備　④メッセージID　⑤状態　⑥メッセージ文字列
		while (1){
			if (msglen < 1 || *ptr == 0x00){
				// データフィールドが終わったので変換文字列をセットして終了
				// メッセージ文字列のコード変換
				strMsg = ConvUtf8Unicode(stdbuf);
				break;
			}
			// 区切り文字「0x20」見つかったら、変換した文字列をアイテム毎にセットする
			if (*ptr == 0x20 && item < 5){
				// 一つのアイテムが決定
				switch (item){
				case	0:	strDate = stdbuf.c_str();		break;
				case	1:	strTime = stdbuf.c_str();		break;
				case	2:	strSend = stdbuf.c_str();		break;
				case	3:	strMsgid = stdbuf.c_str();		break;
				case	4:	strLevel = stdbuf.c_str();		break;
				}
				ptr++;
				msglen--;
				item++;
				stdbuf = "";
				continue;
			}
			// 文字を連結する
			stdbuf += char(*ptr);
			ptr++;
			msglen--;
		}
	}

	CString str;
	int i = 0;
	// 日付
	i = 0;
	AfxExtractSubString(str, strDate, i++, '-');
	data.year = _wtoi(str);
	AfxExtractSubString(str, strDate, i++, '-');
	data.month = _wtoi(str);
	AfxExtractSubString(str, strDate, i++, '-');
	data.day = _wtoi(str);
	// 時間
	i = 0;
	AfxExtractSubString(str, strTime, i++, ':');
	data.hour = _wtoi(str);
	AfxExtractSubString(str, strTime, i++, ':');
	data.minute = _wtoi(str);
	AfxExtractSubString(str, strTime, i++, ':');
	data.second = _wtoi(str);

	// 送信元
	typedef struct {
		CString		name;
		UINT		code;
	} stSender;
	const stSender mSender[] = {
		{ _T("e1"), eSenderItem1 },	// 出先局側局運用管制装置
		{ _T("e2"), eSenderItem1 },	// 相模原側局運用管制装置
		{ _T("e3"), eSenderItemMax },	// 筑波局運用管制装置
		{ _T("c0"), eSenderItem2 },	// 設備制御装置
		{ _T("85"), eSenderItem3 },	// S帯TLM入力装置
		{ _T("86"), eSenderItem3 },	// X帯TLM入力装置
		{ _T("87"), eSenderItem4 },	// S帯コマンド出力装置
	};
#ifdef _DEBUG_HISTORY
	data.equipment = (BYTE)(rand() % (eSenderItemMax+1));
#else
	strSend.MakeLower();
	data.equipment = eSenderItemMax+1;
	for (int i = 0; i < sizeof(mSender) / sizeof(mSender[0]); i++){
		if (strSend == mSender[i].name){
			data.equipment = mSender[i].code;
			break;
		}
	}
	if (data.equipment == (eSenderItemMax+1)){
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CString msg;
		msg.Format(_T("%s %s %s %s %s %s"), (LPCTSTR)strDate, (LPCTSTR)strTime, (LPCTSTR)strSend, (LPCTSTR)strMsgid, (LPCTSTR)strLevel, (LPCTSTR)strMsg);
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return;
	}
#endif
	if (theApp.GetPipeIgnore() & 0x04){
		if (data.equipment == eSenderItemMax){
			return;
		}
	}

	// メッセージID
#ifdef _DEBUG_HISTORY
	data.message = (DWORD)((rand() % 7) * 10000 + (rand() % 10000));
//	data.message = (DWORD)40000;
#else
	data.message = _wtoi(strMsgid.Mid(1, strMsgid.GetLength() - 2));
#endif

	// メッセージ文字列
	wsprintf(data.msgstring, _T("%s"), (LPCTSTR)strMsg);
#ifdef _DEBUG_HISTORY
	if ((rand() % 5) == 0){
		wsprintf(data.msgstring, _T("[イベント実行]%s"), (LPCTSTR)strMsg);
	}

#endif

	bool bLimit = false;
	if (data.message >= 30000 && data.message <= 39999 ||
		data.message >= 40000 && data.message <= 49999){
		bLimit = IsMessageLimit(data.message);
	}

	// ステータス
	if (data.message >= 30000 && data.message <= 39999 ||
		data.message >= 40000 && data.message <= 49999)
	{
		bool bStatus = false;
		// 状態遷移履歴
		// リミットエラー履歴
		switch (data.message%10){
		case	1:/*POWER ON/OFF*/
			data.status = 4;
			break;
		case	2:/*REMOTE/LOCAL*/
			data.status = 3;
			break;
		case	3:/*MAINT/NORMAL*/		
			data.status = 3;		// REMOTE/LOCALに含める
			break;
		case	4:/*WARNING*/
			data.status = 1;
			break;
		case	5:/*FAULT*/
			data.status = 2;
			break;
		case	6:/*FAULT*/
			data.status = 2;
			break;
		case	7:/*STATUS*/
			data.status = 0;
			break;
		case	8:/*復旧*/
			data.status = 0;
			data.imstatus = 0;
			bStatus = true;
			break;
		default:
			if (bLimit == true){
				data.status = 0;
				data.imstatus = 0;
				bStatus = true;
				break;
			}
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CString msg;
			msg.Format(_T("%s %s %s %s %s %s"), (LPCTSTR)strDate, (LPCTSTR)strTime, (LPCTSTR)strSend, (LPCTSTR)strMsgid, (LPCTSTR)strLevel, (LPCTSTR)strMsg);
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::error);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
			return;
		}
		if (bStatus == false)
		{
#ifdef _DEBUG_HISTORY
			data.imstatus = (BYTE)(rand() % 3);
#else
			if (strLevel == _T("di"))
				data.imstatus = 0;
			else if (strLevel == _T("dw"))
				data.imstatus = 1;
			else if (strLevel == _T("ds"))
				data.imstatus = 2;
			else if (strLevel == _T("dg"))
				data.imstatus = 0;
			else{// if (strLevel == _T("lo")){
				//=====================================================//
				//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
				CString msg;
				msg.Format(_T("%s %s %s %s %s %s"), (LPCTSTR)strDate, (LPCTSTR)strTime, (LPCTSTR)strSend, (LPCTSTR)strMsgid, (LPCTSTR)strLevel, (LPCTSTR)strMsg);
				CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::error);
				//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
				//=====================================================//
				return;
			}
#endif
		}
	}
	else
	{
		// 上記以外
		data.imstatus = 0;
#ifdef _DEBUG_HISTORY
		data.status = (BYTE)(rand() % 3);
#else
		if (strLevel == _T("di"))
			data.status = 0;
		else if (strLevel == _T("dw"))
			data.status = 1;
		else if (strLevel == _T("ds"))
			data.status = 2;
		else if (strLevel == _T("dg"))
			data.status = 0;
		else{// if (strLevel == _T("lo")){
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CString msg;
			msg.Format(_T("%s %s %s %s %s %s"), (LPCTSTR)strDate, (LPCTSTR)strTime, (LPCTSTR)strSend, (LPCTSTR)strMsgid, (LPCTSTR)strLevel, (LPCTSTR)strMsg);
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::error);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
			return;
		}
#endif
	}

	bool bOpeMsg = false;
	if (data.message >= 0 && data.message <= 29999){
		// 運用履歴
		bOpeMsg = true;
	}

#ifdef _DEBUG_HISTORY
	data.station = (BYTE)(rand() % eStation_MAX);
	if (data.equipment == eSenderItemMax){
		data.station = (BYTE)0;
	}
#else
	if (nStation == 0x010a0102) {// UDSC64
		// 関数冒頭で局番セット済み
	}
	else if (nStation == m_nHistID54) {// XDSC54
		// 関数冒頭で局番セット済み
	}
	else if (nStation == 0x010a0203) {// USC34
		// 関数冒頭で局番セット済み
	}
	else if (nStation == 0x010a0103) {// USC20
		// 関数冒頭で局番セット済み
	}
	else{
		if (bOpeMsg == true){
			// 運用履歴の場合は筑波局内のメッセージが存在し、局識別子が０
			// 関数冒頭で局番セット済み
		}
		else{
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CString msg;
			msg.Format(_T("%s %s %s %s %s 0x%08x %s"), (LPCTSTR)strDate, (LPCTSTR)strTime, (LPCTSTR)strSend, (LPCTSTR)strMsgid, (LPCTSTR)strLevel, nStation, (LPCTSTR)strMsg);
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::error);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
			return;
		}
	}
	if (data.equipment == eSenderItemMax){
//		data.station = (BYTE)0;
		data.station = (BYTE)theApp.GetSelectStation();
	}
#endif
	if (bOpeMsg == false){
		// 運用履歴以外は、送信元が筑波局運用管制装置はありえない
		if (data.equipment >= eSenderItemMax){
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CString msg;
			msg.Format(_T("%s %s %s %s %s %s"), (LPCTSTR)strDate, (LPCTSTR)strTime, (LPCTSTR)strSend, (LPCTSTR)strMsgid, (LPCTSTR)strLevel, (LPCTSTR)strMsg);
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::error);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
			return;
		}
	}

	if (theApp.GetSelectStationMode() == eStation_1 && theApp.GetSelectStation() != data.station){
		// 対象の局ではない
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CString msg;
		msg.Format(_T("Station(%d) is not support"), (int)data.station);
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return;
	}


	bool bEvent = IsMessageEvent(data.msgstring);

	CGraphMutex::Lock(eData);
	if (bEvent == true){
		// イベント履歴
		mEventHistoryTemp.push_back(data);
		mSubEventHistoryData.push_back(data);
	}
	else{
		if (data.message >= 0 && data.message <= 29999){
			// 運用履歴
			mOperationHistoryTemp.push_back(data);
		}
		if (data.message >= 30000 && data.message <= 39999){
			if (bLimit == true){
				// リミットエラー履歴
				//mEquipmentHistoryTemp.push_back(data);
				mSubEquipmentHistoryData.push_back(data);
			}
			else{
				// 状態遷移履歴
				mEquipmentHistoryTemp.push_back(data);
				//mSubEquipmentHistoryData.push_back(data);
			}
		}
		if (data.message >= 40000 && data.message <= 49999){
			// リミットエラー履歴
			//mEquipmentHistoryTemp.push_back(data);
			mSubEquipmentHistoryData.push_back(data);
		}
		if (data.message >= 50000 && data.message <= 59999){
			// 制御履歴
			mControlHistoryTemp.push_back(data);
		}
		if (data.message >= 60000 && data.message <= 69999){
			// イベント履歴
			mEventHistoryTemp.push_back(data);
			mSubEventHistoryData.push_back(data);
		}
	}
#ifdef _DEBUG
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CString msg;
	msg.Format(_T("%s %s %s %s %s %s"), (LPCTSTR)strDate, (LPCTSTR)strTime, (LPCTSTR)strSend, (LPCTSTR)strMsgid, (LPCTSTR)strLevel, (LPCTSTR)strMsg);
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
#endif

	DWORD dwElapsed = (timeGetTime() - dwStart);
//	TRACE("### CreateDataToHistory Elapsed(%d)\n", dwElapsed);
	CGraphMutex::Unlock(eData);
}

/*============================================================================*/
/*! データ収集

-# 文字コード変換を行う

@param	src		　　UTF-8テキスト

@retval	Cstring		ShiftJIS(CP932)テキスト
*/
/*============================================================================*/
CString CDataCollection::ConvUtf8SjisEx(std::string const& src)
{
	//UTF-16変換後のサイズを取得
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0U, src.data(), int(src.size()) + 1, nullptr, 0U);
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];
	//UTF-8からUTF-16へ一旦変換
	MultiByteToWideChar(CP_UTF8, 0, src.data(), int(src.size()) + 1, bufUnicode, lenghtUnicode);
	int lengthSJis = WideCharToMultiByte(932, 0U, bufUnicode, -1, nullptr, 0U, nullptr, nullptr);
	char* bufShiftJis = new char[lengthSJis];
	//UTF-16からShift-JIS(CP932)へ変換
	WideCharToMultiByte(932, 0, bufUnicode, lenghtUnicode, bufShiftJis, lengthSJis, nullptr, nullptr);
	std::string strSJis(bufShiftJis);

	delete[] bufUnicode;
	delete[] bufShiftJis;

	return CString(&strSJis[0]);
}

/*============================================================================*/
/*! データ収集

-# 文字コード変換を行う

@param	src		　　UTF-8テキスト

@retval	Cstring		UNICODEテキスト
*/
/*============================================================================*/
CString CDataCollection::ConvUtf8Unicode(std::string const& src)
{
	//UTF-16変換後のサイズを取得
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0U, src.data(), int(src.size()) + 1, nullptr, 0U);
	if (lenghtUnicode <= 0)
	{
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CString msg;
		msg.Format(_T("Code Error : MultiByteToWideChar Length(%d)"), lenghtUnicode);
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvUtf8Unicode"), msg, _T("detail"), nLogEx::detail);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return _T("");
	}

	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];
	//UTF-8からUTF-16へ変換
	MultiByteToWideChar(CP_UTF8, 0, src.data(), int(src.size()) + 1, bufUnicode, lenghtUnicode);

	CString strUnicode(bufUnicode);
	delete[] bufUnicode;

	return strUnicode;
}

/*============================================================================*/
/*! データ収集

-# メッセージ内容からイベント履歴かのチェックを行う

@param	pmsg	メッセージ文字列

@retval bool
*/
/*============================================================================*/
bool CDataCollection::IsMessageEvent(TCHAR* pmsg)
{
	const CString mMessageEvent[] = {
		{ _T("[イベント実行]") },
		{ _T("[シーケンス管理]") },
		{ _T("[TLM運用]") },
		{ _T("[CMD運用]") },
		{ _T("[RNG運用]") },
		{ _T("[変調制御管理]") },
		{ _T("[UPLINK運用]") },
		{ _T("[制御出力サブ]") },
		{ _T("[S-DL捕捉]") },
		{ _T("[X-DL捕捉]") },
		{ _T("[Ka-DL捕捉]") },
	};
	CString msg = CString(pmsg);
	for (int i = 0; i < sizeof(mMessageEvent) / sizeof(mMessageEvent[0]); i++){
		if (msg.Find(mMessageEvent[i]) >= 0){
			// イベント履歴の文字列が見つかった
			return true;
		}
	}
	return false;
}

/*============================================================================*/
/*! データ収集

-# メッセージ内容からイベント履歴かのチェックを行う

@param	msgno	メッセージ番号

@retval	bool
*/
/*============================================================================*/
bool CDataCollection::IsMessageLimit(DWORD msgno)
{
	CString msg;
	msg.Format(_T("%d"), msgno);
	if (msg.Right(1) == _T("9")){
		return true;
	}
	return false;
}

/*============================================================================*/
/*! データ収集

-# 運用履歴データに作成

メッセージIDの区分は以下のようになります。
0 ～ 29999 運用履歴
30000 ～ 39999 状態遷移履歴
40000 ～ 49999 リミットエラー履歴
50000 ～ 59999 制御履歴
60000 ～ 69999 イベント履歴

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::CreateOperationHistory()
{
	CGraphMutex::Lock(eData);

	try
	{
		stHistoryData data;
		SYSTEMTIME time;
		GetLocalTime(&time);
		data.year = time.wYear;
		data.month = (BYTE)time.wMonth;
		data.day = (BYTE)time.wDay;
		data.hour = (BYTE)time.wHour;
		data.minute = (BYTE)time.wMinute;
		data.second = (BYTE)time.wSecond;
		UINT val;
		val = sizeof(mStationString) / sizeof(mStationString[0]);
		data.station = (BYTE)(rand() % val);
		val = sizeof(mHistEquipmentString) / sizeof(mHistEquipmentString[0]);
		data.equipment = (BYTE)(rand() % val);
		BYTE sb = (BYTE)(rand() % 23);
		if (sb == 13)
			data.status = 1;
		else if (sb == 17)
			data.status = 2;
		else
			data.status = 0;
		data.message = (BYTE)(rand() % 30000);
		mOperationHistoryTemp.push_back(data);
#ifdef _NOCOMM
		CString str, cbuf, temp;
		cbuf.Empty();
		str.Format(_T("%04d-%02d-%02d"), data.year, data.month, data.day);
		cbuf += str;
		str.Format(_T(",%02d:%02d:%02d"), data.hour, data.minute, data.second);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mStationString[data.station]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistEquipmentString[data.equipment]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistStatusString[data.status]);
		cbuf += str;
		str.Format(_T(",%d"), data.message);
		cbuf += str;
		str.Format(_T(",[### TESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTEST ###"));
		cbuf += str;
		cbuf += _T("\n");
		if (mOperationHistFile.m_hFile != INVALID_HANDLE_VALUE){
			TCHAR* p = _wsetlocale(LC_ALL, _T(""));
			mOperationHistFile.WriteString(cbuf);
			_wsetlocale(LC_ALL, p);
			mOperationHistFile.Flush();
		}
#endif
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);
}

/*============================================================================*/
/*! データ収集

-# 制御履歴データに作成

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::CreateControlHistory()
{
	CGraphMutex::Lock(eData);

	try
	{
		stHistoryData data;
		SYSTEMTIME time;
		GetLocalTime(&time);
		data.year = time.wYear;
		data.month = (BYTE)time.wMonth;
		data.day = (BYTE)time.wDay;
		data.hour = (BYTE)time.wHour;
		data.minute = (BYTE)time.wMinute;
		data.second = (BYTE)time.wSecond;
		UINT val;
		val = sizeof(mStationString) / sizeof(mStationString[0]);
		data.station = (BYTE)(rand() % val);
		val = sizeof(mHistEquipmentString) / sizeof(mHistEquipmentString[0]);
		data.equipment = (BYTE)(rand() % val);
		BYTE sb = (BYTE)(rand() % 23);
		if (sb == 13)
			data.status = 1;
		else if (sb == 17)
			data.status = 2;
		else
			data.status = 0;
		data.message = (BYTE)(rand() % 10000)+50000;
		mControlHistoryTemp.push_back(data);
#ifdef _NOCOMM
		CString str, cbuf, temp;
		cbuf.Empty();
		str.Format(_T("%04d-%02d-%02d"), data.year, data.month, data.day);
		cbuf += str;
		str.Format(_T(",%02d:%02d:%02d"), data.hour, data.minute, data.second);
		cbuf += str;
		str.Format(_T(",%s"), mStationString[data.station]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistEquipmentString[data.equipment]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistStatusString[data.status]);
		cbuf += str;
		str.Format(_T(",%d"), data.message);
		cbuf += str;
		str.Format(_T(",[### TESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTEST ###"));
		cbuf += str;
		cbuf += _T("\n");
		if (mControlHistFile.m_hFile != INVALID_HANDLE_VALUE){
			TCHAR* p = _wsetlocale(LC_ALL, _T(""));
			mControlHistFile.WriteString(cbuf);
			_wsetlocale(LC_ALL, p);
			mControlHistFile.Flush();
		}
#endif
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);
}

/*============================================================================*/
/*! データ収集

-# 設備状況履歴データに作成

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::CreateEquipmentHistory()
{
	CGraphMutex::Lock(eData);

	try
	{
		stHistoryData data;
		SYSTEMTIME time;
		GetLocalTime(&time);
		data.year = time.wYear;
		data.month = (BYTE)time.wMonth;
		data.day = (BYTE)time.wDay;
		data.hour = (BYTE)time.wHour;
		data.minute = (BYTE)time.wMinute;
		data.second = (BYTE)time.wSecond;
		UINT val;
		val = sizeof(mStationString) / sizeof(mStationString[0]);
		data.station = (BYTE)(rand() % val);
		val = sizeof(mHistEquipmentString) / sizeof(mHistEquipmentString[0]);
		data.equipment = (BYTE)(rand() % val);
		BYTE sb = (BYTE)(rand() % 93);
		if (sb == 13)
			data.imstatus = 1;
		else if (sb == 17)
			data.imstatus = 2;
		else
			data.imstatus = 0;
		data.status = (BYTE)(rand() % 5);
		data.message = (BYTE)(rand() % 10000) + 40000;
		mEquipmentHistoryTemp.push_back(data);
#ifdef _NOCOMM
		CString str, cbuf, temp;
		cbuf.Empty();
		str.Format(_T("%04d-%02d-%02d"), data.year, data.month, data.day);
		cbuf += str;
		str.Format(_T(",%02d:%02d:%02d"), data.hour, data.minute, data.second);
		cbuf += str;
		str.Format(_T(",%s"), mStationString[data.station]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistEquipmentString[data.equipment]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistImStatusString[data.imstatus]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistStatusString[data.status]);
		cbuf += str;
		str.Format(_T(",%d"), data.message);
		cbuf += str;
		str.Format(_T(",[### TESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTEST ###"));
		cbuf += str;
		cbuf += _T("\n");
		if (mEquipmentHistFile.m_hFile != INVALID_HANDLE_VALUE){
			TCHAR* p = _wsetlocale(LC_ALL, _T(""));
			mEquipmentHistFile.WriteString(cbuf);
			_wsetlocale(LC_ALL, p);
			mEquipmentHistFile.Flush();
		}
#endif
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);
}

/*============================================================================*/
/*! データ収集

-# イベント履歴データに作成

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::CreateEventHistory()
{
	CGraphMutex::Lock(eData);

	try
	{
		stHistoryData data;
		SYSTEMTIME time;
		GetLocalTime(&time);
		data.year = time.wYear;
		data.month = (BYTE)time.wMonth;
		data.day = (BYTE)time.wDay;
		data.hour = (BYTE)time.wHour;
		data.minute = (BYTE)time.wMinute;
		data.second = (BYTE)time.wSecond;
		UINT val;
		val = sizeof(mStationString) / sizeof(mStationString[0]);
		data.station = (BYTE)(rand() % val);
		val = sizeof(mHistEquipmentString) / sizeof(mHistEquipmentString[0]);
		data.equipment = (BYTE)(rand() % val);
		BYTE sb = (BYTE)(rand() % 93);
		if (sb == 13)
			data.imstatus = 1;
		else if (sb == 17)
			data.imstatus = 2;
		else
			data.imstatus = 0;
		data.status = (BYTE)(rand() % 3);
		data.message = (BYTE)(rand() % 10000) + 60000;
		mEventHistoryTemp.push_back(data);
#ifdef _NOCOMM
		CString str, cbuf, temp;
		cbuf.Empty();
		str.Format(_T("%04d-%02d-%02d"), data.year, data.month, data.day);
		cbuf += str;
		str.Format(_T(",%02d:%02d:%02d"), data.hour, data.minute, data.second);
		cbuf += str;
		str.Format(_T(",%s"), mStationString[data.station]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistEquipmentString[data.equipment]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistImStatusString[data.imstatus]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistStatusString[data.status]);
		cbuf += str;
		str.Format(_T(",%d"), data.message);
		cbuf += str;
		str.Format(_T(",[### TESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTEST ###"));
		cbuf += str;
		cbuf += _T("\n");
		if (mEventHistFile.m_hFile != INVALID_HANDLE_VALUE){
			TCHAR* p = _wsetlocale(LC_ALL, _T(""));
			mEventHistFile.WriteString(cbuf);
			_wsetlocale(LC_ALL, p);
			mEventHistFile.Flush();
		}
#endif
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);
}

/*============================================================================*/
/*! データ収集

-# データ収集の開始

@param
@retval

*/
/*============================================================================*/
void CDataCollection::StartCollection()
{
#ifdef _CUTNOCOMM
	SYSTEMTIME time;
	GetLocalTime(&time);
	if (mOperationHistFile.m_hFile != INVALID_HANDLE_VALUE){
		mOperationHistFile.Close();
	}
	if (mControlHistFile.m_hFile != INVALID_HANDLE_VALUE){
		mControlHistFile.Close();
	}
	if (mEquipmentHistFile.m_hFile != INVALID_HANDLE_VALUE){
		mEquipmentHistFile.Close();
	}
	if (mEventHistFile.m_hFile != INVALID_HANDLE_VALUE){
		mEventHistFile.Close();
	}

	bool ret;
	CString str, fname;
	str.Format(_T("%4d%02d%02d"), time.wYear, time.wMonth, time.wDay);
	fname = theApp.GetAppDataPath();
	fname += _T("\\");
	fname += _T("Operation_");
	fname += str;
	fname += _T(".txt");
	ret = false;
	mOperationHistFile.Open(fname, CFile::modeRead);
	if (mOperationHistFile.m_hFile != INVALID_HANDLE_VALUE){
		// 既にある
		ret = true;
		mOperationHistFile.Close();
	}
	mOperationHistFile.Open(fname, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyWrite);
	if (ret == true)
		mOperationHistFile.SeekToEnd();

	fname = theApp.GetAppDataPath();
	fname += _T("\\");
	fname += _T("Control_");
	fname += str;
	fname += _T(".txt");
	ret = false;
	mControlHistFile.Open(fname, CFile::modeRead);
	if (mControlHistFile.m_hFile != INVALID_HANDLE_VALUE){
		// 既にある
		ret = true;
		mControlHistFile.Close();
	}
	mControlHistFile.Open(fname, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyWrite);
	if (ret == true)
		mControlHistFile.SeekToEnd();

	fname = theApp.GetAppDataPath();
	fname += _T("\\");
	fname += _T("Equipment_");
	fname += str;
	fname += _T(".txt");
	ret = false;
	mEquipmentHistFile.Open(fname, CFile::modeRead);
	if (mEquipmentHistFile.m_hFile != INVALID_HANDLE_VALUE){
		// 既にある
		ret = true;
		mEquipmentHistFile.Close();
	}
	mEquipmentHistFile.Open(fname, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyWrite);
	if (ret == true)
		mEquipmentHistFile.SeekToEnd();

	fname = theApp.GetAppDataPath();
	fname += _T("\\");
	fname += _T("Event_");
	fname += str;
	fname += _T(".txt");
	ret = false;
	mEventHistFile.Open(fname, CFile::modeRead);
	if (mEventHistFile.m_hFile != INVALID_HANDLE_VALUE){
		// 既にある
		ret = true;
		mEventHistFile.Close();
	}
	mEventHistFile.Open(fname, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyWrite);
	if (ret == true)
		mEventHistFile.SeekToEnd();
#endif
	createCollectionThread();
}

/*============================================================================*/
/*! データ収集

-# データ収集スレッドの作成

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::createCollectionThread()
{
	stThreadParam param;

	if (mpCollectThread == NULL){
		mpCollectThread = (CDataCollectionThread*)AfxBeginThread(RUNTIME_CLASS(CDataCollectionThread), THREAD_PRIORITY_HIGHEST/*THREAD_PRIORITY_NORMAL*/, 0, CREATE_SUSPENDED);
		param.parent = this;
		param.callback = messageProc;
		mpCollectThread->SetThreadParam(param);
		mpCollectThread->ResumeThread();
	}
}
/*============================================================================*/
/*! データ収集

-# データ収集スレッドの作成

@param		message	メッセージ
@param		wParam	メッセージの付加情報
@param		lParam	メッセージの付加情報

@retval void
*/
/*============================================================================*/
void CALLBACK CDataCollection::messageProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_ThreadNormal:
		break;
	case	eMessage_ThreadError:
		break;
	case	eMessage_ThreadFail:
		break;
	case	eMessage_ThreadException:
		break;
	case	eMessage_UpdateForecastData:
		break;
	default:
		return;
		break;
	}
	if ((theApp.m_pMainWnd)->GetSafeHwnd())
		(theApp.m_pMainWnd)->PostMessage(message, wParam, lParam);
}

/*============================================================================*/
/*! データ収集

-# データ収集の終了

@param
@retval

*/
/*============================================================================*/
void CDataCollection::StopCollection()
{
	removeMeasureThread();
#ifdef _NOCOMM
	if (mOperationHistFile.m_hFile != INVALID_HANDLE_VALUE){
		mOperationHistFile.Close();
	}
	if (mControlHistFile.m_hFile != INVALID_HANDLE_VALUE){
		mControlHistFile.Close();
	}
	if (mEquipmentHistFile.m_hFile != INVALID_HANDLE_VALUE){
		mEquipmentHistFile.Close();
	}
	if (mEventHistFile.m_hFile != INVALID_HANDLE_VALUE){
		mEventHistFile.Close();
	}
	mOperationHistFile.m_hFile = INVALID_HANDLE_VALUE;
	mControlHistFile.m_hFile = INVALID_HANDLE_VALUE;
	mEquipmentHistFile.m_hFile = INVALID_HANDLE_VALUE;
	mEventHistFile.m_hFile = INVALID_HANDLE_VALUE;
#endif
}
/*============================================================================*/
/*! データ収集

-# データ収集スレッドの削除

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::removeMeasureThread()
{
	if (mpCollectThread != NULL){
		mpCollectThread->StopCollection();
		WaitForSingleObject(mpCollectThread->m_hThread, 10000);
		delete mpCollectThread;
		mpCollectThread = NULL;
	}
}

/*============================================================================*/
/*! データ収集

-# サから受信したデータを登録する

@param

@retval void
*/
/*============================================================================*/
bool CDataCollection::AddCollection(UINT message, WPARAM wParam, LPARAM lParam)
{
	CGraphMutex::Lock(eData);

	try
	{
		for (int nStation = 0; nStation < eStation_MAX; nStation++)
		{
			// 一時的に格納されたデータを恒久領域に格納する
			while (!mSpaceTemp[nStation].empty())
			{
				CSpaceData newData = mSpaceTemp[nStation].front();
				mSpaceTemp[nStation].erase(mSpaceTemp[nStation].begin());

				// インデックスマップからインデックス取得
				UINT nIndex = mGraphSigIndexMap[nStation][newData.dataname];
				if (nIndex == 0)
				{
					// マップにないため、グラフ対象外
					continue;
				}
				
				nIndex--;

				if (mSpaceData[nStation][nIndex].dataname.IsEmpty())
				{
					// 初回は監視情報を一通りコピー
					mSpaceData[nStation][nIndex].dataid = newData.dataid;
					mSpaceData[nStation][nIndex].dataname = newData.dataname;
					mSpaceData[nStation][nIndex].kind = newData.kind;
					mSpaceData[nStation][nIndex].srcType = newData.srcType;
					mSpaceData[nStation][nIndex].totalSize = newData.totalSize;
					mSpaceData[nStation][nIndex].value = newData.value;
				}
				else
				{
					// 同じ監視データがキューあったら、値を追加
					while (!newData.value.empty())
					{
						stSpaceData spaceData = newData.value.front();
						newData.value.pop_front();
#ifdef _DEBUG
						{
							CString strDebug;
							strDebug.Format(_T("%s(%d) :: Add Data to mSpaceData: Time(%I64d), SigId(%u), Val(%f)"), __FILEW__, __LINE__, CTime(spaceData.systime).GetTime(), newData.dataid, spaceData.value);
							CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection_Dummy"), _T("debug"), strDebug, nLogEx::debug);
						}
#endif
						// データ歯抜け対応
						int size = (int)mSpaceData[nStation][nIndex].value.size();
						if (size > 0)
						{
							deque<stSpaceData>::iterator itr = mSpaceData[nStation][nIndex].value.end();
							itr--;
							CTime lastTime = (*itr).systime;
							CTime curTime = spaceData.systime;

							// 新しいデータの時刻が、リスト格納済みデータよりも古い場合は新しいデータを破棄する
							if (curTime < lastTime)
							{
								break;
							}

							while (curTime > lastTime + 1)
							{
								stSpaceData		sEmptyData;
								memset(&sEmptyData, 0, sizeof(stSpaceData));
								lastTime += 1;
								lastTime.GetAsSystemTime(sEmptyData.systime);
								sEmptyData.status[0] = -1;
								sEmptyData.ppPair.max = 0.0;
								sEmptyData.ppPair.min = 0.0;
								sEmptyData.ppPair.count = 1;
								// 監視データのキューサイズ制限
//								while (mSpaceData[nStation][nIndex].value.size() >= MAX_SPACEDATA_SIZE)
//								{
//									mSpaceData[nStation][nIndex].value.pop_front();
//								}
								LimitQueueSize(mSpaceData[nStation][nIndex].value);

								// データトータルサイズをインクリメント
								mSpaceData[nStation][nIndex].totalSize++;
								// 監視データのキューに追加
								mSpaceData[nStation][nIndex].value.push_back(sEmptyData);
							}
						}

						// 監視データのキューサイズ制限
//						while (mSpaceData[nStation][nIndex].value.size() >= MAX_SPACEDATA_SIZE)
//						{
//							mSpaceData[nStation][nIndex].value.pop_front();
//						}
						LimitQueueSize(mSpaceData[nStation][nIndex].value);

						// データトータルサイズをインクリメント
						mSpaceData[nStation][nIndex].totalSize++;
						// 監視データのキューに追加
						mSpaceData[nStation][nIndex].value.push_back(spaceData);

						break;
					}
				}
			}
		}

	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);

	// ここで１秒周期で取得できなかったデータの対策を行う。
	// CSpaceData内の「value」にはstSpaceData構造体が複数格納されている
	/*
	①[stSpaceData(A)][stSpaceData(B)][stSpaceData(C)][stSpaceData(D)]・・・
	②[stSpaceData(B)][stSpaceData(C)][stSpaceData(D)]・・・
	１秒周期毎に取得したデータ数はサーバーコントローラの性能により異なる
	例えば、①で取得したデータに「(A)」データが存在しない場合は
	「(A)」の「status」を「None」にして登録する必要がある
	②[stSpaceData(B)][stSpaceData(C)][stSpaceData(D)]・・・[stSpaceData(A)]
	グラフ描画する際に「status」が「None」のデータは描画しない
	*/

	return true;
}


/*============================================================================*/
/*! データ収集

-# 工学値変換から受信したデータを登録する

@param

@retval void
*/
/*============================================================================*/
bool CDataCollection::AddCollection(UINT nStation, vector<CEngValCnv::obs_tbl_res_t>& data)
{
	CGraphMutex::Lock(eData);

	try
	{

		for (vector<CEngValCnv::obs_tbl_res_t>::iterator itrEng = data.begin(); itrEng != data.end(); itrEng++)
		{
			CSpaceData		newData;
			stSpaceData		sData;
			newData.value.clear();
			memset(&sData, 0, sizeof(stSpaceData));
			CString strDataName = CString((*itrEng).obs_adr.sz_obsname);
			newData.dataid = mGraphSigIndexMap[nStation][strDataName];
			if (newData.dataid == 0)
				continue;

			newData.dataname = strDataName;
			newData.kind = 0;		// 表示対象データ種別(0:データ値 / 1:ステート値)
			newData.srcType = eSigSrcType_EngExchange;
			newData.totalSize = 1;

			CTime	cTime((time_t)(*itrEng).ld_time_val);
			cTime.GetAsSystemTime(sData.systime);

#ifdef	DemoTimeStamp
			// デモパイプ通信用に現在時刻をスタンプ
			GetSystemTime(&sData.systime);
#endif	// DemoTimeStamp

			time_t	data_t = cTime.GetTime();
			CTime	curTime;
			theApp.GetCurrentUtcTime(curTime);
			time_t  utc_t = curTime.GetTime();

			// 監視データ時刻が現在のUTCシステム時刻より規定時間以上古い場合は、グラフ衛星監視データキューに格納せずに破棄する
			if (data_t + mKanshiDisposeTimeFromUTC < utc_t)
			{
#ifdef _DEBUG
				{
					TCHAR szSigName[_MAX_PATH];
					MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, (*itrEng).obs_adr.sz_obsname, -1, szSigName, _MAX_PATH);
					CString strDebug;
					strDebug.Format(_T("%s(%d) :: Recieved kanshi data time(%I64d) + Kanshi dispase time(%I64d) is older System UTC Time(%I64d), SigName(%s), SigId(%u), Val(%f)"), __FILEW__, __LINE__, data_t, mKanshiDisposeTimeFromUTC, utc_t, szSigName, newData.dataid, (*itrEng).obs_adr.d_data);
					CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("debug"), strDebug, nLogEx::debug);
				}
#endif
				continue;
			}

			sData.idx = 0;
			sData.value = (*itrEng).obs_adr.d_data;
			memcpy(sData.status, (*itrEng).obs_adr.sz_sts_name, 8);

			sData.status[0] = 0;

			sData.ppPair.max = sData.value;
			sData.ppPair.min = sData.value;
			//			sData.ppPair.count = 0;
			sData.ppPair.count = 1;
			newData.value.push_back(sData);

#ifdef _DEBUG
			{
				TCHAR szSigName[_MAX_PATH];
				//								MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, (*itrEng).obs_adr.sz_obsname, (int)strlen((*itrEng).obs_adr.sz_obsname), szSigName, (int)sizeof(szSigName) / 2);
				MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, (*itrEng).obs_adr.sz_obsname, -1, szSigName, _MAX_PATH);
				CString strDebug;
				strDebug.Format(_T("%s(%d) :: Recv Eng Data: Time(%lf), SigName(%s), SigId(%u), Val(%f)"), __FILEW__, __LINE__, (*itrEng).ld_time_val, szSigName, newData.dataid, (*itrEng).obs_adr.d_data);
				CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("debug"), strDebug, nLogEx::debug);
			}
#endif

			// インデックスマップからインデックス取得
			UINT nIndex = mGraphSigIndexMap[nStation][newData.dataname];
			if (nIndex == 0)
			{
				// マップにないため、グラフ対象外
				continue;
			}

			nIndex--;

			if (mSpaceData[nStation][nIndex].dataname.IsEmpty())
			{
				BOOL bNewEngValData = TRUE;
				int size = GetGraphSigSize(nStation);
				for (int i = 0; i < size; i++)
				{
					if (mSpaceData[nStation][i].value.size() > 0)
					{
						bNewEngValData = FALSE;
						break;
					}
				}

				// 初回は監視情報を一通りコピー
				mSpaceData[nStation][nIndex].dataid = newData.dataid;
				mSpaceData[nStation][nIndex].dataname = newData.dataname;
				mSpaceData[nStation][nIndex].kind = newData.kind;
				mSpaceData[nStation][nIndex].srcType = newData.srcType;
				mSpaceData[nStation][nIndex].totalSize = newData.totalSize;
				mSpaceData[nStation][nIndex].value = newData.value;

#ifdef _DEBUG
				{
					TCHAR szSigName[_MAX_PATH];
					MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, (*itrEng).obs_adr.sz_obsname, -1, szSigName, _MAX_PATH);
					CString strDebug;
					strDebug.Format(_T("%s(%d) :: Add Data to mSpaceData(New ID): Time(%lf), SigName(%s), SigId(%u), Val(%f)"), __FILEW__, __LINE__, (*itrEng).ld_time_val, szSigName, newData.dataid, (*itrEng).obs_adr.d_data);
					CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("debug"), strDebug, nLogEx::debug);
				}
#endif
				// 空の場合は、新しく工学値変換を受けたことを通知
				if (bNewEngValData)
					theApp.m_pMainWnd->PostMessage(eMessage_NewEngValData, 0, 0);
			}
			else
			{

#ifdef _DEBUG
				{
					TCHAR szSigName[_MAX_PATH];
					MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, (*itrEng).obs_adr.sz_obsname, -1, szSigName, _MAX_PATH);
					CString strDebug;
					strDebug.Format(_T("%s(%d) :: Detect Same Data in mSpaceData: Time(%lf), SigName(%s), SigId(%u), Val(%f)"), __FILEW__, __LINE__, (*itrEng).ld_time_val, szSigName, newData.dataid, (*itrEng).obs_adr.d_data);
					CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("debug"), strDebug, nLogEx::debug);
				}
#endif

				// 同じ監視データがキューあったら、値を追加
				while (!newData.value.empty())
				{
					stSpaceData spaceData = newData.value.front();
					newData.value.pop_front();

					// データ歯抜け対応
					int size = (int)mSpaceData[nStation][nIndex].value.size();
					if (size > 0)
					{
						deque<stSpaceData>::iterator itrSpace = mSpaceData[nStation][nIndex].value.end();
						itrSpace--;
						CTime lastTime = (*itrSpace).systime;
						CTime curTime = spaceData.systime;

						// 新しいデータの時刻が、リスト格納済みデータよりも古い場合は新しいデータを破棄する
						if (curTime < lastTime)
						{
//#ifdef _DEBUG
//							//---------------------- 少ない監視データで長時間動作確認したいときのみ使用 ------------------------------//
//							time_t inc_t = 1;
//							if (newData.dataname.Compare(USC34_AGC_SHXX_TCR_SIG) == 0)
//							{
//								time_t cur_t = curTime.GetTime();
//								// 新しいデータ時刻と直前の短周期時刻を比較し、一致しているならば時刻インクリメントは無し
//								if (cur_t == m_shortCycleTime)
//								{
//									inc_t = 0;
//								}
//								// 一致していない場合は、新しいデータ時刻を入れる
//								else
//								{
//									m_shortCycleTime = cur_t;
//								}
//
//								// 積分のデータ時刻と1秒より離れている時は時刻インクリメントは無し
//								if (lastTime.GetTime() > 0 && m_integtime > 0 && lastTime.GetTime() > m_integtime + 1)
//								{
//									inc_t = 0;
//								}
//							}
//
//							if (newData.dataname.Compare(USC34_EL_EL_SIG) == 0)
//							{
//								// 積分のデータ時刻と1秒より離れている時は時刻インクリメントは無し
//								if (lastTime.GetTime() > 0 && m_integtime > 0 && lastTime.GetTime() > m_integtime + 1)
//								{
//									inc_t = 0;
//								}
//							}
//
//							curTime = lastTime + inc_t;
//							curTime.GetAsSystemTime(spaceData.systime);
//
//							if (newData.dataname.Find(USC34_AGC_INTEG_SIG) >= 0)
//							{
//								// 積分のデータ時刻を控えておく
//								m_integtime = curTime.GetTime();
//							}
//
//							//--------------------------------------------------------------------------------------------------------//
//#else
							break;
//#endif
						}

						while (curTime > lastTime + 1)
						{
							stSpaceData		sEmptyData;
							memset(&sEmptyData, 0, sizeof(stSpaceData));
							lastTime += 1;
							lastTime.GetAsSystemTime(sEmptyData.systime);
							sEmptyData.status[0] = -1;
							sEmptyData.ppPair.max = 0.0;
							sEmptyData.ppPair.min = 0.0;
							sEmptyData.ppPair.count = 1;
							// 監視データのキューサイズ制限
//							while (mSpaceData[nStation][nIndex].value.size() >= MAX_SPACEDATA_SIZE)
//							{
//								mSpaceData[nStation][nIndex].value.pop_front();
//							}
							LimitQueueSize(mSpaceData[nStation][nIndex].value);

							// データトータルサイズをインクリメント
							mSpaceData[nStation][nIndex].totalSize++;
							// 監視データのキューに追加
							mSpaceData[nStation][nIndex].value.push_back(sEmptyData);
						}
					}

					// 監視データのキューサイズ制限
//					while (mSpaceData[nStation][nIndex].value.size() >= MAX_SPACEDATA_SIZE)
//					{
//						mSpaceData[nStation][nIndex].value.pop_front();
//					}
					LimitQueueSize(mSpaceData[nStation][nIndex].value);

#ifdef _DEBUG
						{
							TCHAR szSigName[_MAX_PATH];
							MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, (*itrEng).obs_adr.sz_obsname, -1, szSigName, _MAX_PATH);
							CString strDebug;
							strDebug.Format(_T("%s(%d) :: Add Data to mSpaceData(Same ID): Time(%lf), SigName(%s), SigId(%u), Val(%f)"), __FILEW__, __LINE__, (*itrEng).ld_time_val, szSigName, newData.dataid, (*itrEng).obs_adr.d_data);
							CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("debug"), strDebug, nLogEx::debug);
						}
#endif

					// データトータルサイズをインクリメント
					mSpaceData[nStation][nIndex].totalSize++;
					// 監視データのキューに追加
					mSpaceData[nStation][nIndex].value.push_back(spaceData);
					break;
				}
			}
		}
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);

	// ここで１秒周期で取得できなかったデータの対策を行う。
	// CSpaceData内の「value」にはstSpaceData構造体が複数格納されている
	/*
	①[stSpaceData(A)][stSpaceData(B)][stSpaceData(C)][stSpaceData(D)]・・・
	②[stSpaceData(B)][stSpaceData(C)][stSpaceData(D)]・・・
	１秒周期毎に取得したデータ数はサーバーコントローラの性能により異なる
	例えば、①で取得したデータに「(A)」データが存在しない場合は
	「(A)」の「status」を「None」にして登録する必要がある
	②[stSpaceData(B)][stSpaceData(C)][stSpaceData(D)]・・・[stSpaceData(A)]
	グラフ描画する際に「status」が「None」のデータは描画しない
	*/

	return true;
}


/*============================================================================*/
/*! データ収集

-# キューのグラフデータを削除する

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::ClearCollection(UINT nStation, CString strGName)
{
	CGraphMutex::Lock(eData);

	try
	{
		// インデックスマップからインデックス取得
		UINT nIndex = mGraphSigIndexMap[nStation][strGName];
		if (nIndex == 0)
		{
			// マップにないため、グラフ対象外
			CGraphMutex::Unlock(eData);
			return;
		}

		nIndex--;

		mSpaceData[nStation][nIndex].value.clear();
		mSpaceData[nStation][nIndex].totalSize = 0;
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);
}


/*============================================================================*/
/*! データ収集

-# キューのグラフデータを削除する

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::ClearCollection(UINT nStation)
{
	for (int i = 0; i < GRAPH_YOHOSIGNUM; i++)
	{
		ClearCollection(nStation, mListYohoSig[nStation][i]);
	}
}


/*============================================================================*/
/*! データ収集

-# サから受信したデータを登録する

@param

@retval bool
*/
/*============================================================================*/
UINT CDataCollection::AddHistory(UINT message, WPARAM wParam, LPARAM lParam)
{
	CGraphMutex::Lock(eData);
	UINT	status = eHistStatus_Normal;

	try
	{
		// 一時的に格納されたデータを恒久領域に格納する
		while (!mOperationHistoryTemp.empty())
		{
			stHistoryData newData = mOperationHistoryTemp.front();
			mOperationHistoryTemp.pop_front();

			if (mOperationHistoryData.size() == MAX_HISTORY_DATA){
				mOperationHistoryData.pop_front();
			}
			mOperationHistoryData.push_back(newData);
			status = __max((BYTE)status, newData.status);
		}

		while (!mControlHistoryTemp.empty())
		{
			stHistoryData newData = mControlHistoryTemp.front();
			mControlHistoryTemp.pop_front();

			if (mControlHistoryData.size() == MAX_HISTORY_DATA){
				mControlHistoryData.pop_front();
			}
			mControlHistoryData.push_back(newData);
			status = __max((BYTE)status, newData.status);
		}

		while (!mEquipmentHistoryTemp.empty())
		{
			stHistoryData newData = mEquipmentHistoryTemp.front();
			mEquipmentHistoryTemp.pop_front();

			if (mEquipmentHistoryData.size() == MAX_HISTORY_DATA){
				mEquipmentHistoryData.pop_front();
			}
			mEquipmentHistoryData.push_back(newData);
			status = __max((BYTE)status, newData.imstatus & 0x7fffffff);
		}

		if (mSubEquipmentHistoryData.size() > MAX_HISTORY_DATA){
			mSubEquipmentHistoryData.pop_front();
		}

		while (!mEventHistoryTemp.empty())
		{
			stHistoryData newData = mEventHistoryTemp.front();
			mEventHistoryTemp.pop_front();

			if (mEventHistoryData.size() == MAX_HISTORY_DATA){
				mEventHistoryData.pop_front();
			}
			if (mSubEventHistoryData.size() == MAX_HISTORY_DATA){
				mSubEventHistoryData.pop_front();
			}
			mEventHistoryData.push_back(newData);
			mSubEventHistoryData.push_back(newData);
			status = __max((BYTE)status, newData.imstatus & 0x7fffffff);
		}

	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);

	return status;
}


/*============================================================================*/
/*! データ収集

-# 基地局リストの取得

@param		list	基地局名称格納バッファ
@retval

*/
/*============================================================================*/
void CDataCollection::GetBaseStationList(UINT nStation, vector<CString>& list)
{
	list.clear();
	vector<CString>::iterator itrl;

	map<DWORD, CString>::iterator itr;
	CString strs, stre;
	for (itr = mDataNameList[nStation].begin(); itr != mDataNameList[nStation].end(); itr++){
		AfxExtractSubString(strs, (*itr).second, 0, '.');
		AfxExtractSubString(stre, (*itr).second, 1, '.');
		bool set = true;
		for (itrl = list.begin(); itrl != list.end(); itrl++){
			if (strs == (*itrl)){
				set = false;
				break;
			}
		}
		if (set == true)
			list.push_back(strs);
	}
}
/*============================================================================*/
/*! データ収集

-# 設備（機器）リストの取得

@param		baseName	基地局名称
@param		list		基地局内の設備（機器）名称格納バッファ
@retval

*/
/*============================================================================*/
void CDataCollection::GetEquipmentList(UINT nStation, CString baseName, vector<CString>& list)
{
	list.clear();
	vector<CString>::iterator itrl;

	map<DWORD, CString>::iterator itr;
	CString strs, stre;
	for (itr = mDataNameList[nStation].begin(); itr != mDataNameList[nStation].end(); itr++){
		AfxExtractSubString(strs, (*itr).second, 0, '.');
		AfxExtractSubString(stre, (*itr).second, 1, '.');
		if (baseName.IsEmpty() == false && baseName != strs)
			continue;
		bool set = true;
		for (itrl = list.begin(); itrl != list.end(); itrl++){
			if (stre == (*itrl)){
				set = false;
				break;
			}
		}
		if (set == true)
			list.push_back(stre);
	}
}
/*============================================================================*/
/*! データ収集

-# データ名称リストの取得

@param		baseName	基地局名称
@param		equipment	基地局内の設備（機器）名称
@param		list		データ名称格納バッファ
@retval

*/
/*============================================================================*/
void CDataCollection::GetDataNameList(UINT nStation, CString baseName, CString equipment, vector<CString>& list)
{
	list.clear();

	map<DWORD, CString>::iterator itr;
	CString strs, stre;
	for (itr = mDataNameList[nStation].begin(); itr != mDataNameList[nStation].end(); itr++){
		AfxExtractSubString(strs, (*itr).second, 0, '.');
		AfxExtractSubString(stre, (*itr).second, 1, '.');
		if (baseName.IsEmpty() == false && baseName != strs)
			continue;
		if (equipment.IsEmpty() == false && equipment != stre)
			continue;
		list.push_back((*itr).second);
	}
}

/*============================================================================*/
/*! データ収集

-# 衛星捕捉データ取得

@param		nStation	局番号
@retval		衛星捕捉データキュー

*/
/*============================================================================*/
CSpaceData* CDataCollection::GetSpaceData(int nStation /*= eStation_MAX*/)
{
	if (nStation < 0 || nStation > eStation_MAX)
		return &mSpaceData[0][0];
	if (nStation == eStation_MAX)
		return &mSpaceData[theApp.GetSelectStation()][0];
	return &mSpaceData[nStation][0];
}

/*============================================================================*/
/*! データ収集

-# 衛星捕捉テンポラリデータ取得（ダミーデータ）

@param		nStation	局番号
@retval		衛星捕捉テンポラリデータキュー

*/
/*============================================================================*/
deque<CSpaceData>& CDataCollection::GetSpaceTemp(int nStation /*= eStation_MAX*/)
{
	if (nStation < 0 || nStation > eStation_MAX)
		return mSpaceTemp[0];
	if (nStation == eStation_MAX)
		return mSpaceTemp[theApp.GetSelectStation()];
	return mSpaceTemp[nStation];
}

/*============================================================================*/
/*! データ収集

-# 衛星捕捉テンポラリデータ取得（工学値変換後データ）

@param		nStation	局番号
@retval		衛星捕捉テンポラリデータキュー

*/
/*============================================================================*/
deque<CSpaceData>& CDataCollection::GetSpaceEngValTemp(int nStation /*= eStation_MAX*/)
{
	if (nStation < 0 || nStation > eStation_MAX)
		return mSpaceTemp[0];
	if (nStation == eStation_MAX)
		return mSpaceTemp[theApp.GetSelectStation()];
	return mSpaceTemp[nStation];
}


/*============================================================================*/
/*! データ収集

-# 衛星生データのサイズを最大値までに制限する

@param		value	衛星データキュー
@retval		なし

*/
/*============================================================================*/
void CDataCollection::LimitQueueSize(deque<stSpaceData>& value)
{
	// 削除する場合
	if (value.size() > MAX_SPACEDATA_SIZE)
	{
		// キューの先頭と終わりの時刻を調べる
		deque<stSpaceData>::iterator itrSpace;
		itrSpace = value.begin();
		CTime firstTime = (*itrSpace).systime;

		itrSpace = value.end();
		itrSpace--;
		CTime lastTime = (*itrSpace).systime;

		time_t first_t = firstTime.GetTime();
		time_t last_t = lastTime.GetTime();

		// キューサイズ制限から新しい先頭時刻を出す
		time_t new_first_t = last_t - ((time_t)MAX_SPACEDATA_SIZE - 1);
		if (new_first_t < 0)
		{
			new_first_t = 0;
		}

		while (first_t < new_first_t)
		{
			// キューから新しい先頭より前の時刻のデータを削除する
			value.pop_front();
			itrSpace = value.begin();
			firstTime = (*itrSpace).systime;
			first_t = firstTime.GetTime();
		}
	}
}

/*============================================================================*/
/*! アプリケーション

-# 局内別の監視名置き換え

@param str:監視名

@retval void
*/
/*============================================================================*/
CString CDataCollection::ReplaceAreaKanshiName(CString& str)
{
	CString strKanshi = str;
	int nAreaNo = theApp.GetClientTerminal();
	switch (nAreaNo)
	{
	case eClientSttn:
		strKanshi.Replace(mAreaNameString[eClientTksc], mAreaNameString[eClientSttn]);
		break;
	
	case eClientSsoc:
		strKanshi.Replace(mAreaNameString[eClientTksc], mAreaNameString[eClientSsoc]);
		break;
	
	case eClientTksc:
	default:
		// 何もしない
		break;
	}

	return strKanshi;
}