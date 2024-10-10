/*============================================================================*/
/*! EngValCnv.cpp

-# 工学値変換
*/
/*============================================================================*/

#include "stdafx.h"
#include "EngValCnv.h"
#include <sys/stat.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <iomanip>

// timeGetTime使用
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

CEngValCnvDll::CEngValCnvDll(char* db_file_name)
{
	mCallBack = NULL;
	db_data = NULL;
	db_data2 = NULL;
	try{
		HANDLE hFile = CreateFileA(db_file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		mFileSize = GetFileSize(hFile, NULL) + 100;
		CloseHandle(hFile);
		db_data = new char[mFileSize];
		db_data2 = new char[mFileSize];
		//DB情報格納領域を初期化
		memset(db_data, 0, mFileSize);
		memset(db_data2, 0, mFileSize);

		//DBファイルを読んでくる
		if (DbRead(db_file_name, db_data) != 0){
			db_status = 1;
			return;
		}

		memcpy_s(db_data2, mFileSize, db_data, mFileSize);

		// DBテーブル作成
		CreateObsTbl();

		db_status = 0;
	}
	catch (...){
		//失敗
		db_status = 1;
	}

	m_lastCnvAfterType = 0;

	sprintf_s(mEqMonDBFileName, _MAX_PATH, "%s", db_file_name);
}

CEngValCnvDll::CEngValCnvDll()
{
	memset(mEqMonDBFileName, 0, _MAX_PATH);
	db_data = NULL;
	db_data2 = NULL;
}

CEngValCnvDll::~CEngValCnvDll()
{
	data_list.clear();

	if (db_data != NULL)
		delete[] db_data;
	if (db_data2 != NULL)
		delete[] db_data2;

	mMonTimeLDData.clear();
}

/*
	◇◇◇EngValConv.DLLの使用方法◇◇◇

	①CEngValCnvDllの定義
	  CEngValCnvDll engcnv;
	②監視DBファイル名の設定
	  CString val = _T("C:\\share\\udsc64\\ctrl\\db\ld\\eqmon.db.obj");
	  engcnv.SetEqMonDbFileName(val);
	③監視蓄積データパスの設定
	  val = _T("C:\\share\\udsc64\\log\\mon\\");
	  engcnv.SetMonDataPath(val);
	④監視データの取得
	  TL_TIME timeTarget={0,2017,5,1,0,0};
	  map< CString, vector<obs_tbl_res_t> > outDataList;
	  engcnv.GetMonitorData(timeTarget, outDataList);
*/

/*============================================================================*/
/*! 監視データアクセス

-# 監視DB「eqmon.db.obj」のファイルパス名（C:\share\udsc64\ctrl\db\ld\eqmon.db.obj）を指定する

@param	val		監視DBファイル名

@retval

*/
/*============================================================================*/
void CEngValCnvDll::SetEqMonDbFileName(TCHAR* val)
{
	mDbName = CString(val);
	sprintf_s(mEqMonDBFileName, _MAX_PATH, "%s", (LPCSTR)CStringA(val));

	if (db_data != NULL)
		delete[] db_data;
	if (db_data2 != NULL)
		delete[] db_data2;

	try{
		HANDLE hFile = CreateFileA(mEqMonDBFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		mFileSize = GetFileSize(hFile, NULL) + 100;
		CloseHandle(hFile);
		db_data = new char[mFileSize];
		db_data2 = new char[mFileSize];
		//DB情報格納領域を初期化
		memset(db_data, 0, mFileSize);
		memset(db_data2, 0, mFileSize);

		//DBファイルを読んでくる
		if (DbRead(mEqMonDBFileName, db_data) != 0){
			db_status = 1;
			return;
		}

		memcpy_s(db_data2, mFileSize, db_data, mFileSize);

		db_status = 0;
	}
	catch (...){
		//失敗
		db_status = 1;
	}

	m_lastCnvAfterType = 0;
}

/*
---------------------------------------------------------------------
$name = { 監視項目名テーブル作成 }
$outline = {
テーブル作成
}
$comment = {
}
---------------------------------------------------------------------
*/
/*============================================================================*/
/*! 監視データアクセス

-# 監視項目名テーブル作成

@param
@retval

*/
/*============================================================================*/
int CEngValCnvDll::CreateObsTbl()
{
	struct xobsname_t *obs_adrs;
	struct xext_tbl_t *ext_adr;
	char			 *mon_adr;
	long			l_rc;

	memcpy_s(db_data, mFileSize, db_data2, mFileSize);

	/*----- 各監視項目データ取得処理 -----*/
	mon_adr = db_data;
	mon_adr += (sizeof(long) * 2);

	xobs_tbl_t st;
	obs_map.clear();

	try{
		int l = 0;
		int t = 0;
		while (1){
			obs_adrs = (struct xobsname_t *)mon_adr;

			long ed_id = ds_htonl(obs_adrs->l_id);
			long ed_len = ds_htonl(obs_adrs->l_len);

			t++;

			if (ed_id == 0) {
				break;
			}
			if (ed_id == OBSNAME_ID)
			{
				st.obs_adr = obs_adrs;
				l_rc = ud_ext_adr(st.obs_adr->sz_obsname, obs_adrs, &ext_adr);
				if (l_rc == 0) {
					st.l_dataid = ds_htonl(ext_adr->s_pwb[0].l_pid);;

					long ed_max = ds_htonl(ext_adr->l_pw_cnt);
					for (int m = 1; m < ed_max; m++) {
						if (st.l_dataid != ds_htonl(ext_adr->s_pwb[m].l_pid)) {
							st.l_dataid = -1;
							break;
						}
					}
				}
				else {
					st.l_dataid = -2;
					st.obs_adr->l_no = -1;
				}

				st.l_offset = (long)(mon_adr - db_data);
				st.obs_adr->l_inflg = 0;
				l++;
				if (l >= OBS_MAX) {
					{
						CString strDebug;
						strDebug.Format(_T("%s(%d) :: Error over OBS_MAX(%d)"), __FILEW__, __LINE__, l);
						//						CLogTraceEx::Write(_T(""), _T("CEnvValCnv"), _T("CalcFromMsg"), _T("debug"), strDebug, nLogEx::debug);
					}
					return(-1);
				}

				obs_map[st.l_dataid].push_back(st);
			}
			mon_adr += ed_len;
		}
	}
	catch (...){
		TRACE("@@@ CalcFromFile Exception\n");
	}

	return 0;
}

/*============================================================================*/
/*! 監視データアクセス

-# 監視蓄積ファイルが格納されているパス名（C:\share\udsc64\log\mon\）を指定する

@param	val		監視蓄積ファイルパス名（終端\\）

@retval

*/
/*============================================================================*/
void CEngValCnvDll::SetMonDataPath(TCHAR* val)
{
	swprintf_s(mMonDataPathName, _MAX_PATH, _T("%s"), val);
}

/*============================================================================*/
/*! 監視データアクセス

-# 指定期間の監視データファイルを読み込み、指定された監視名に対応するデータを取得する

@param	startTime	開始日時
@param	endTime		終了日時
@param	outDataList	監視データリスト(入力)（ANT.AZ_DRIVE,ANT.REAL_AZ・・・）キー	(出力)監視データ

@retval	bool

  */
/*============================================================================*/
bool CEngValCnvDll::GetMonitorData(xTL_TIME& timeTarget, map< CString, vector<xobs_tbl_res_t> >& outDataList)
{
	// 最小タイマ分解能を設定(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();

	vector<CString> filelist;
	filelist.clear();

	xTL_TIME timeT = timeTarget;

	// 監視蓄積データの対象パスを取得する

	// 開始終了日時から対象日付フォルダを求める
	switch (timeTarget.bMode){
	case	eTimeDirect:
	{
		if (ConvertDirectRangeTime(timeTarget, timeT, true) == false)
			return false;
		GetTimeRangePath(timeT.sttime, timeT.sptime, filelist);
		break;
	}
	case	eTimeRange:
		GetTimeRangePath(timeT.sttime, timeT.sptime, filelist);
		break;
	default:
		return false;
	}

	if (filelist.size() == 0)
		return false;

	// 「監視蓄積ファイルパス名」＋「開始終了日時から求めたフォルダ」よりデータファイル名を取得する
	vector<CString>::iterator itr;
	CString fname;
	vector<CString> datafilelist;
	datafilelist.clear();
	for (itr = filelist.begin(); itr != filelist.end(); itr++){
		fname.Format(_T("%s%s\\"), mMonDataPathName, (LPCTSTR)(*itr));
		GetDataFileList(fname, datafilelist);
	}

	// 監視蓄積ファイルが決まったので工学値変換を行う
	char file[_MAX_PATH];
	for (itr = datafilelist.begin(); itr != datafilelist.end(); itr++){
		sprintf_s(file, _MAX_PATH, "%s", (LPCSTR)CStringA((*itr)));
		if (CalcFromFileEx(file, outDataList) < 0)
			continue;
		
		// 工学値変換した結果から、対象の監視名称を抜き出す
		vector<xobs_tbl_res_t>& data = GetAllVal();
		vector<xobs_tbl_res_t>::iterator itrd;
		for (itrd = data.begin(); itrd != data.end(); itrd++){

			// 対象の監視名が存在するかチェックする
			if (IsObsName((*itrd).obs_adr.sz_obsname, outDataList) == false){
				continue;
			}

			CStringA str((*itrd).obs_adr.sz_obsname);
			CString wstr = CString(str);
			map< CString, vector<xobs_tbl_res_t> >::iterator itrn;
			itrn = outDataList.find(wstr);

			// 時間範囲の確認
			if (timeTarget.bMode == eTimeRange){
				if (timeTarget.sttime > (*itrd).ld_time_val || timeTarget.sptime < (*itrd).ld_time_val){
					// 範囲外
					continue;
				}
			}

			(*itrn).second.push_back((*itrd));
			//CTime tm = CTime((*itrd).ld_time_val);
			//CString strtm = tm.Format("%Y-%m-%d  %H:%M:%S");
			//TRACE("==%s\n", CStringA(strtm));
		}
	}

	DWORD dwElapsed = (timeGetTime() - dwStart);
	TRACE("### GetMonitorData = %.3lf(sec)\n", (double)((double)dwElapsed / 1000.0));
	timeEndPeriod(1);

	return true;
}

bool CEngValCnvDll::GetMonitorDataEx(xTL_TIME* timeTarget)
{
	// 最小タイマ分解能を設定(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();

	xTL_TIME timeT = *timeTarget;

	// 監視蓄積データの対象ファイルパスを確認する
	if (mMonDataFileList.size() == 0)
		return false;

	DWORD dwElapsed = 0;

	// 監視蓄積ファイルが決まったので工学値変換を行う
	char file[_MAX_PATH];
	vector<CString>::iterator itr;
	for (itr = mMonDataFileList.begin(); itr != mMonDataFileList.end(); itr++){
		sprintf_s(file, _MAX_PATH, "%s", (LPCSTR)CStringA((*itr)));
		if (CalcFromFileEx(file, mOutDataList) < 0)
			continue;

		dwElapsed = (timeGetTime() - dwStart);
		TRACE("### CalcFromFileEx = %.3lf(sec)\n", (double)((double)dwElapsed / 1000.0));
		dwStart = timeGetTime();

		// 工学値変換した結果から、対象の監視名称を抜き出す
		vector<xobs_tbl_res_t>& data = GetAllVal();
		vector<xobs_tbl_res_t>::iterator itrd;
		for (itrd = data.begin(); itrd != data.end(); itrd++){

			// ■以下の処理はCalcFromFileEx関数内で行う
			// 対象の監視名が存在するかチェックする
			//if (IsObsName((*itrd).obs_adr.sz_obsname, mOutDataList) == false){
			//	continue;
			//}

			CStringA str((*itrd).obs_adr.sz_obsname);
			CString wstr = CString(str);
			map< CString, vector<xobs_tbl_res_t> >::iterator itrn;
			itrn = mOutDataList.find(wstr);

			// 時間範囲の確認
			if (timeTarget->bMode == eTimeRange){
				if (timeTarget->sttime > (*itrd).ld_time_val || timeTarget->sptime < (*itrd).ld_time_val){
					// 範囲外
					continue;
				}
			}

			(*itrn).second.push_back((*itrd));
			//CTime tm = CTime((*itrd).ld_time_val);
			//CString strtm = tm.Format("%Y-%m-%d  %H:%M:%S");
			//TRACE("==%s\n", CStringA(strtm));
		}

		dwElapsed = (timeGetTime() - dwStart);
		TRACE("### FileRead = %.3lf(sec)\n", (double)((double)dwElapsed / 1000.0));
		dwStart = timeGetTime();
	}

	timeEndPeriod(1);

	return true;
}

void CEngValCnvDll::ClearMonitorName()
{
	mOutDataList.clear();
}

void CEngValCnvDll::AddMonitorName(TCHAR* name)
{
	CString tag(name);
	vector<xobs_tbl_res_t> val;
	map< CString, vector<xobs_tbl_res_t> >::iterator itr = mOutDataList.find(tag);
	if (itr == mOutDataList.end())
		mOutDataList.insert(map< CString, vector<xobs_tbl_res_t> >::value_type(tag, val));
}

void CEngValCnvDll::ClearDataFileList()
{
	mMonDataFileList.clear();
}

void CEngValCnvDll::AddDataFileList(TCHAR* val)
{
	TCHAR buf[_MAX_PATH];
	swprintf_s(buf, _MAX_PATH, _T("%s"), val);
	mMonDataFileList.push_back(CString(buf));
}

/*============================================================================*/
/*! 監視データアクセス

-# 文字コード変換

@param	src		変換前文字列
@param	dest	返還後文字列

@retval	bool

*/
/*============================================================================*/
bool CEngValCnvDll::ConvertStatusChar(char* stsname, int len)
{
	char src[64];
	char dest[64];
	memcpy(src, stsname, 64);
	memset(dest, 0, 64);

#ifdef _ALLCODE
	int srcpos = 0, destpos = 0;
	bool error = false;
	CStringA str;
	while (srcpos < len){
		if (src[srcpos] == 0x00){
			dest[destpos++] = src[srcpos++];
			break;
		}
		str = UTF8_3CharsToSJis(&src[srcpos]);
		if (str == ""){
			error = true;
			break;
			//memcpy(&dest[destpos], (char*)(const char*)str, 1);
			//srcpos += 1;
			//destpos += 1;
		}
		else{
			memcpy(&dest[destpos], (char*)(const char*)str, 2);
			srcpos += 3;
			destpos += 2;
		}
	}
	if (error == true){
		srcpos = 0, destpos = 0;
		error = false;
		while (srcpos < len){
			if (src[srcpos] == 0x00){
				dest[destpos++] = src[srcpos++];
				break;
			}
			str = EUCToSJis(&src[srcpos]);
			if (str == ""){
				if (srcpos == 0){
					// 先頭文字がEUCではないのでエラーとする
					error = true;
					break;
				}
				memcpy(&dest[destpos], &src[srcpos], 1);
				srcpos += 1;
				destpos += 1;
			}
			else{
				memcpy(&dest[destpos], (char*)(const char*)str, 2);
				srcpos += 2;
				destpos += 2;
			}
		}
	}
	if (error == false){
		memcpy(stsname, dest, 64);
		CStringA str = CStringA(stsname);
		str.TrimLeft();
		str.TrimRight();
		memset(stsname, 0, 64);
		memcpy(stsname, (char*)(const char*)str, str.GetLength());
	}
#else

#ifdef _DEBUG
	static bool bTest = false;
	const char mon[32] = {
		(char)0x56, (char)0x4C, (char)0x42, (char)0x49, (char)0xB1, (char)0xBF, (char)0xCD, (char)0xD1,
		(char)0xBB, (char)0xFE, (char)0xB4, (char)0xD6, (char)0x20, (char)0x20, (char)0x20, (char)0x20,
		(char)0x20, (char)0x20, (char)0x20, (char)0x20, (char)0x20, (char)0x20, (char)0x20, (char)0x20,
		(char)0x20, (char)0x20, (char)0x20, (char)0x20, (char)0x20, (char)0x20, (char)0x20, (char)0x00,
	};

	if (bTest == true)
		memcpy(src, mon, 32);
#endif

	// 文字コードがEUCかをチェックする
	int srcpos = 0, destpos = 0;
	UINT nConvMode = 1; // EUCフラグを立てる
	while (1){
		if (src[srcpos] == 0x00){
			dest[destpos++] = src[srcpos++];
			break;
		}
		if (src[srcpos] >= 0x00 && src[srcpos] <= 0x7F){
			// ASCIIコード
			dest[destpos++] = src[srcpos++];
		}
		else{
			if ((UCHAR)src[srcpos] >= 0xA1 && (UCHAR)src[srcpos] <= 0xFE &&
				(UCHAR)src[srcpos + 1] >= 0xA1 && (UCHAR)src[srcpos + 1] <= 0xFE){
				// EUCの２バイト文字
				srcpos += 2;
				destpos += 2;
			}
			else{
				// EUC2バイト文字ではない
				nConvMode = 2;
				break;
			}
		}
	}

	memset(dest, 0, 64);

	srcpos = 0;
	destpos = 0;
	bool error = false;
	CStringA str;
	while (1){
		if (src[srcpos] == 0x00){
			dest[destpos++] = src[srcpos++];
			break;
		}
		if (src[srcpos] >= 0x00 && src[srcpos] <= 0x7F){
			// ASCIIコード
			dest[destpos++] = src[srcpos++];
		}
		else{
			if (nConvMode == 1){
				// EUCの２バイト文字
				str = EUCToSJis(&src[srcpos]);
				if (str == ""){
					error = true;
					break;
				}
				else{
					memcpy(&dest[destpos], (char*)(const char*)str, 2);
					srcpos += 2;
					destpos += 2;
				}
			}
			if (nConvMode == 2){
				// UTF-8
				str = UTF8_3CharsToSJis(&src[srcpos]);
				if (str == ""){
					error = true;
					break;
				}
				else{
					memcpy(&dest[destpos], (char*)(const char*)str, 2);
					srcpos += 3;
					destpos += 2;
				}
			}
		}
	}
	if (error == false){
		memcpy(stsname, dest, 64);
		str = CStringA(stsname);
		str.TrimLeft();
		str.TrimRight();
		memset(stsname, 0, 64);
		memcpy(stsname, (char*)(const char*)str, str.GetLength());
	}
#endif
	return !error;
}

/*============================================================================*/
/*! 監視データアクセス

-# 直接日付から範囲日付への変換

@param	dtime		直接日付
@param	rtime		範囲日付
@param	timezero	trueの場合変換後の時分秒ミリ秒をゼロにする

@retval	bool

*/
/*============================================================================*/
bool CEngValCnvDll::ConvertDirectRangeTime(xTL_TIME& dtime, xTL_TIME& rtime, bool timezero/*=true*/)
{
	CTimeSpan daySpan(1, 0, 0, 0);
	SYSTEMTIME sttime, sptime;
	memset(&sttime, 0, sizeof(SYSTEMTIME));
	memset(&sptime, 0, sizeof(SYSTEMTIME));
	if (dtime.wYear < 1970 || dtime.wYear > 3000)
		return false;
	sttime.wYear = dtime.wYear;
	sptime.wYear = dtime.wYear;
	if (dtime.wMonth < 0 || dtime.wMonth > 12)
		return false;
	sttime.wMonth = dtime.wMonth;
	sptime.wMonth = dtime.wMonth;
	if (dtime.wMonth == 0){
		sttime.wMonth = 1;
		sptime.wMonth = 12;
	}
	if (dtime.wDay < 0 || dtime.wDay > 31)
		return false;
	sttime.wDay = dtime.wDay;
	sptime.wDay = dtime.wDay;
	if (dtime.wDay == 0){
		sttime.wDay = 1;
		sptime.wDay = 1;
		if (sptime.wMonth == 12){
			sptime.wYear++;
			sptime.wMonth = 1;
		}
		else{
			sptime.wMonth++;
		}
		CTime tm(sptime);
		tm -= daySpan;
		if (tm.GetAsSystemTime(sptime) == false){
			memset(&sptime, 0, sizeof(SYSTEMTIME));
		}
	}

	if (timezero == false){
		sptime.wHour = 23;
		sptime.wMinute = 59;
		sptime.wSecond = 59;
		sptime.wMilliseconds = 999;
	}

	CTime st = CTime(sttime);
	CTime sp = CTime(sptime);

	rtime.sttime = st.GetTime();
	rtime.sptime = sp.GetTime();

	return true;
}

/*============================================================================*/
/*! 監視データアクセス

-# 指定期間の監視データファイルを読み込み、指定された監視名に対応するデータを取得する

@param	sttime		開始日時
@param	sptime		終了日時
@param	fileList	ファイルパス一覧（出力）

@retval	void

*/
/*============================================================================*/
void CEngValCnvDll::GetTimeRangePath(time_t sttime, time_t sptime, vector<CString>& fileList)
{
	fileList.clear();

	CTimeSpan daySpan(1, 0, 0, 0);
	CTime time;
	CString str;
	for (time_t tm = sttime; tm <= sptime;){
		// 日付から日付パスを求める
		time = CTime(tm);
		str = time.Format("%Y-%m-%d");

		fileList.push_back(str);

		time += daySpan;
		tm = time.GetTime();
	}
}

/*============================================================================*/
/*! 監視データアクセス

-# 指定フォルダのファイル一覧を取得する

@param	path	フォルダ名
@param	outFileList	監視ファイルリスト(出力)

@retval

*/
/*============================================================================*/
void CEngValCnvDll::GetDataFileList(CString& path, vector<CString>& outFileList)
{
	// 対象フォルダ内のファイル情報を取得する
	// 指定されたファイル名に一致するファイルを、ディレクトリ内で検索する
	WIN32_FIND_DATA ffdata;
	HANDLE hFindFile = FindFirstFile(path+_T("*"), &ffdata);
	if (hFindFile == INVALID_HANDLE_VALUE){
		return;
	}

	UINT item = 0;
	do{
		TRY
		{
			CString fname;
			if (ffdata.cFileName[0] == '\0')
			{
				AfxThrowUserException();
			}
			if (ffdata.cFileName[0] == '.')
			{
				AfxThrowUserException();
			}
			if (ffdata.cFileName[0] == '[')
			{
				AfxThrowUserException();
			}

			// ファイル名取得
			if (ffdata.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
			{
				CString name = CString(ffdata.cFileName);
				if (name.Find(_T(".")) >= 0){
					continue;
				}
				if (name.Left(3).CompareNoCase(_T("NON")) == 0){
					continue;
				}
				outFileList.push_back(path + name);
			}
		}
		CATCH(CUserException, e)
		{
		}
		AND_CATCH(CException, e)
		{
		}
		END_CATCH
	} while (FindNextFile(hFindFile, &ffdata));

	FindClose(hFindFile);
}
/*============================================================================*/
/*! 監視データアクセス

-# UTF8コードをSJISに変換

@param	pUtf8Str			UTF-8

@retval	CString
*/
/*============================================================================*/
CStringA CEngValCnvDll::UTF8_3CharsToSJis(char *pUtf8Str)
{
	wchar_t pUtfMsg[4];

	if (pUtf8Str[0] >= 0x00 && pUtf8Str[0] <= 0x7f){
		// 1バイト文字
		return "";
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
		return "";

	CStringA str(sjis);
	return str;
}
/*============================================================================*/
/*! 監視データアクセス

-# EUCコードをSJISに変換

@param	pEucStr			EUC

@retval	CString
*/
/*============================================================================*/
CStringA CEngValCnvDll::EUCToSJis(char *pEucStr)
{
	// EUCの２バイト文字かのチェック
	if ((UCHAR)pEucStr[0] >= 0xA1 && (UCHAR)pEucStr[0] <= 0xFE &&
		(UCHAR)pEucStr[1] >= 0xA1 && (UCHAR)pEucStr[1] <= 0xFE){
		// 文字コードは２バイト文字
	}
	else{
		return "";
	}

	char	sjis[10];

	// EUC-JIS
	sjis[0] = pEucStr[0] & ~0x80;
	sjis[1] = pEucStr[1] & ~0x80;
	sjis[2] = '\0';
	// JIS-SJIS
	if (sjis[0] & 0x01){
		sjis[0] >>= 1;
		if (sjis[0] < 0x2F)
			sjis[0] += 0x71;
		else
			sjis[0] -= 0x4F;

		if (sjis[1] > 0x5F)
			sjis[1] += 0x20;
		else
			sjis[1] += 0x1F;
	}
	else{
		sjis[0] >>= 1;
		if (sjis[0] < 0x2F)
			sjis[0] += 0x70;
		else
			sjis[0] -= 0x50;
		sjis[1] += 0x7E;
	}
	CStringA str(sjis);

	return str;
}

/*============================================================================*/
/*! 監視データアクセス

-# 監視項目名

@param	path	フォルダ名
@param	outFileList	監視ファイルリスト(出力)

@retval

*/
/*============================================================================*/
bool CEngValCnvDll::IsObsName(char* obsname, map< CString, vector<xobs_tbl_res_t> >& outDataList)
{
	CStringA str(obsname);
	CString wstr = CString(str);
	map< CString, vector<xobs_tbl_res_t> >::iterator itrn;
	itrn = outDataList.find(wstr);
	if (itrn == outDataList.end()){
		// 監視名が存在しないので何もしない
		return false;
	}
	return true;
}






int CEngValCnvDll::DbRead(char* db_file_nam, char *cp_loadadr_p)
{
	int errno;
	struct stat stbuf;
	int    i_readbyte;
	long   l_alldatasize = 0, l_datasize;
	char   sz_work[256] = { 0 };

	unsigned long ul_top_pos; /*ファイルの先頭からメモリロード内容の先頭までのバイト数*/
	long   l_idnum = 0;
	FILE *fp;
	char *fname = db_file_nam;

	/*ファイル名とみなす*/
	if (stat(fname, &stbuf) == -1){
		return(500);
	}

	l_datasize = stbuf.st_size;


	if (l_idnum == 0) {
		l_alldatasize += l_datasize;
	}
	else {
		l_alldatasize += (l_datasize * l_idnum);
	}

	errno_t ans;
	/*cp_loadadr_p = cp_loadadr;*/
	if (((ans = fopen_s(&fp, fname, "rb")) != 0) || (fp == NULL)){
		return(500);
	}
	if (fread(sz_work, sizeof(char), sizeof(sz_work), fp) == -1){/*#!HEAD:があるかどうか読んでみる*/
		fclose(fp); /*ファイルをcloseする*/
		return(500);
	}
	if (!strncmp(sz_work, "#!HEAD:", 7)){ /*#!HEAD:を読み飛ばす*/
		if ((ul_top_pos = (unsigned long)strchr(sz_work, '\n')) == NULL){
			fclose(fp); /*ファイルをcloseする*/
			return(500);
		}
		else{
			ul_top_pos = (ul_top_pos - (unsigned long)&sz_work[0]) + 1;
		}
	}
	else{ /*ファイルの先頭から全内容を読み込む*/
		ul_top_pos = 0; /*ファイルの先頭からメモリにロードする内容の先頭までのバイト数*/
	}
	if (fseek(fp, ul_top_pos, SEEK_SET) == -1){
		fclose(fp); /*ファイルをcloseする*/
		return(500);
	}
	for (;;){
		i_readbyte = (int)fread(cp_loadadr_p, sizeof(char), (l_datasize - ul_top_pos), fp);
		if (i_readbyte <= 0){ break; }
		if (i_readbyte == (l_datasize - ul_top_pos)){ break; }
		l_datasize -= i_readbyte;

		cp_loadadr_p = cp_loadadr_p + i_readbyte;
	}
	cp_loadadr_p = cp_loadadr_p + (l_datasize - ul_top_pos);
	l_alldatasize -= ul_top_pos; /*#!HEAD:の部分のバイト数を補正*/
	fclose(fp); /*ファイルをcloseする*/

	return 0;
}

/*============================================================================*/
/*! 監視データアクセス

-# 監視データファイルを読み込む
　　◆注意◆呼び出しもとで戻り値のTL_TLMDATA*アドレスを削除すること

  @param	fp	ファイルポインタ
  @retval	TL_TLMDATA* 監視データポインタ

  */
/*============================================================================*/
void* CEngValCnvDll::MonRead(FILE **fp)
{
	size_t i_readbyte2;

	xTL_INFO info;
	xTL_TLMDATA* pdata;

	// 最初に「情報フィールド」を読み込みBodyサイズを取得する
	i_readbyte2 = fread(&info, sizeof(xTL_INFO), 1, *fp);
	if (i_readbyte2 <= 0){
		return NULL;
	}

	//エンディアン変換
#if 1 //1=big-endianness 
	info.l_reclen = htonl(info.l_reclen);
#else
	info.l_reclen = info.l_reclen;
#endif

	// Bodyサイズがわかったので、サイズ分メモリを確保して「付加情報」＋「テレメトリ受信データ」を取得する
	pdata = (xTL_TLMDATA*)::malloc(sizeof(xTL_INFO) + info.l_reclen);
	if (pdata == NULL)
		return NULL;

	memcpy(&(pdata->info), &info, sizeof(xTL_INFO));
	i_readbyte2 = fread(&pdata->addinfo.uc_ver, info.l_reclen, 1, *fp);
	if (i_readbyte2 <= 0)
	{
		free(pdata);
		return NULL;
	}

	return (void*)pdata;
}

/*============================================================================*/
/*! 監視データアクセス

-# 監視蓄積ファイルから工学値変換を行う(トレンドグラフ用)

@param	nama_data_file	監視蓄積ファイル名
@param	outDataList		対象監視名一覧
@retval	int

*/
/*============================================================================*/
int CEngValCnvDll::CalcFromFileEx(char* nama_data_file, map< CString, vector<xobs_tbl_res_t> >& outDataList)
{
	struct xobsname_t *obs_adrs;
	struct xext_tbl_t *ext_adr;
	char      *mon_adr;
	long      l_rc;

	long      l_dataid;
	int       l_inflg;
	int       l_grmd;
	double    d_data;
	long double ld_data;/* 工学値データ64bit整数を格納できる精度 */
	long      l_bitinfo;   /* 整数ビット数:整数抽出データのビット長 */
	char      sz_work[STR_BUF_SIZE];
	std::vector<xobs_tbl_res_t>::iterator itr;


	if (db_status != 0){
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: Error db_status(%d)"), __FILEW__, __LINE__, db_status);
//			CLogTraceEx::Write(_T(""), _T("CEnvValCnv"), _T("CalcFromMsg"), _T("debug"), strDebug, nLogEx::debug);
		}
		return -1;
	}

	memcpy_s(db_data, mFileSize, db_data2, mFileSize);

	/*----- 各監視項目データ取得処理 -----*/
	mon_adr = db_data;
	mon_adr += (sizeof(long) * 2);

	data_list.clear();

	FILE *fp = NULL;
	bool bRead = true;
	int ret = 0;

	// 最小タイマ分解能を設定(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();
	const CStringA mStdMonTime = "MON_TIME";

	try{
		int l = 0;
		while (1){
			obs_adrs = (struct xobsname_t *)mon_adr;

			long ed_id = ds_htonl(obs_adrs->l_id);
			long ed_len = ds_htonl(obs_adrs->l_len);

			if (ed_id == 0) {
				break;
			}

			// 時間を取得するために「MON_TIME」は必要なので、
			// 設備毎の「MON_TIME」は登録する
			CStringA str = CStringA(obs_adrs->sz_obsname);
			if (str.Find(mStdMonTime) < 0){
				// 対象監視項目名が存在するかのチェック
				if (IsObsName(obs_adrs->sz_obsname, outDataList) == false){
					mon_adr += ed_len;
					continue;
				}
			}

			if (ed_id == OBSNAME_ID)
			{
				obs_tbl[l].obs_adr = obs_adrs;
				l_rc = ud_ext_adr(obs_tbl[l].obs_adr->sz_obsname, obs_adrs, &ext_adr);
				if (l_rc == 0) {
					obs_tbl[l].l_dataid = ds_htonl(ext_adr->s_pwb[0].l_pid);
					long ed_max = ds_htonl(ext_adr->l_pw_cnt);
					for (int m = 1; m < ed_max; m++) {
						if (obs_tbl[l].l_dataid != ds_htonl(ext_adr->s_pwb[m].l_pid)) {
							obs_tbl[l].l_dataid = -1;
							break;
						}
					}
				}
				else {
					obs_tbl[l].l_dataid = -2;
					obs_tbl[l].obs_adr->l_no = -1;
				}

				obs_tbl[l].l_offset = (long)(mon_adr - db_data);
				obs_tbl[l].obs_adr->l_inflg = 0;
				l++;
				if (l >= OBS_MAX) {
					{
						CString strDebug;
						strDebug.Format(_T("%s(%d) :: Error over OBS_MAX(%d)"), __FILEW__, __LINE__, l);
//						CLogTraceEx::Write(_T(""), _T("CEnvValCnv"), _T("CalcFromMsg"), _T("debug"), strDebug, nLogEx::debug);
					}
					return(-1);
				}
			}
			mon_adr += ed_len;
		}
		obs_tbl[l].obs_adr = NULL;
		obs_tbl[l].l_offset = NULL;


		/*--------------------------------------------------------*/
		/*    監視データ通知                                      */
		/*--------------------------------------------------------*/
		int offset = 0;
		unsigned char* buff;
		long cnt = 0;
		char* mon_time_name = "MON_TIME";
		char   sz_work2[256] = { 0 };
		unsigned long ul_top_pos;

		// 監視データのオープン
		if ((fopen_s(&fp, nama_data_file, "rb")) != 0){
			bRead = false;
		}
		// 「#!HEAD:」が存在する場合は読み捨てる
		if ((fp == NULL) || (fread(sz_work2, sizeof(char), sizeof(sz_work2), fp) == -1)){/*#!HEAD:があるかどうか読んでみる*/
			// 読み込みエラーなので何もしない
			if (fp != NULL)
			{
				fclose(fp); /*ファイルをcloseする*/
			}
			bRead = false;
		}
		else{
			// 「#!HEAD:」が存在したので読み捨てる
			if (!strncmp(sz_work2, "#!HEAD:", 7)){ /*#!HEAD:を読み飛ばす*/
				if ((ul_top_pos = (unsigned long)strchr(sz_work2, '\n')) == NULL){
					fclose(fp); /*ファイルをcloseする*/
					bRead = false;
				}
				else{
					// 監視データの先頭位置
					ul_top_pos = (ul_top_pos - (unsigned long)&sz_work2[0]) + 1;
				}
			}
			else{ /*ファイルの先頭から全内容を読み込む*/
				// ファイル先頭から監視データ
				ul_top_pos = 0; /*ファイルの先頭からメモリにロードする内容の先頭までのバイト数*/
			}
		}
		if (bRead == true){
			// 監視データ位置までシークする
			if (fseek(fp, ul_top_pos, SEEK_SET) == -1){
				fclose(fp); /*ファイルをcloseする*/
				bRead = false;
			}
		}

		bool bFirst = true;
		// ファイルENDまで以下を繰り返す
		while (bRead)
		{
			// 「TL_TLMDATA」単位でデータを読み込む
			xTL_TLMDATA* pdata;
			pdata = (xTL_TLMDATA*)MonRead(&fp);
			if (pdata == NULL)
			{
				// ファイルの終端あるいは読み込みエラーなので処理を終了
				fclose(fp); /*ファイルをcloseする*/
				bRead = false;
				break;
			}

			//======================================================================
			//	ここから読み込んだ監視データをデータリストに格納する
			//======================================================================
			l_dataid = pdata->addinfo.uca_op_data_id[0];
			buff = (unsigned char *)&pdata->addinfo.uca_timetag;

//			long double time_val = 0;
			bool leapSec = false; // うるう秒フラグ

			for (l = 0; obs_tbl[l].obs_adr != NULL; l++) {

				if (l_dataid == obs_tbl[l].l_dataid || obs_tbl[l].l_dataid == -1) {

					// 実際の工学値変換
					memset(sz_work, 0, STR_BUF_SIZE);
					l_rc = ud_montbl_data64(db_data, l_dataid, buff,
						obs_tbl[l].l_offset,
						&l_inflg,
						&l_grmd,
						&d_data,
						sz_work,
						&ld_data,
						&l_bitinfo);

					if (l_rc == 0 && l_inflg == 1) {
						obs_tbl[l].l_grp_cnt = cnt;
						obs_tbl[l].obs_adr->l_id = ds_htonl(obs_tbl[l].obs_adr->l_id);
						obs_tbl[l].obs_adr->l_len = ds_htonl(obs_tbl[l].obs_adr->l_len);
						obs_tbl[l].obs_adr->l_no = ds_htonl(obs_tbl[l].obs_adr->l_no);
						obs_tbl[l].obs_adr->l_inflg = l_inflg;
						obs_tbl[l].obs_adr->l_grmd = l_grmd;
						obs_tbl[l].obs_adr->d_data = d_data;
						obs_tbl[l].obs_adr->ld_data = ld_data;  /* 64bit精度拡張 */
						obs_tbl[l].obs_adr->l_bitinfo = l_bitinfo; /* 64bit精度拡張 */
						obs_tbl[l].obs_adr->l_cnvResType = GetCnvResType();
						strcpy_s(obs_tbl[l].obs_adr->sz_sts_name, sizeof(obs_tbl[l].obs_adr->sz_sts_name), sz_work);

						//結果を格納
						xobs_tbl_res_t res;
						res.l_dataid = obs_tbl[l].l_dataid;
						res.l_grp_cnt = obs_tbl[l].l_grp_cnt;
						res.l_offset = obs_tbl[l].l_offset;
						res.l_dataid = obs_tbl[l].l_dataid;
						memcpy_s(&res.obs_adr, sizeof(xobsname_t), obs_tbl[l].obs_adr, sizeof(xobsname_t));

						CString	tmp_obsname = CString(res.obs_adr.sz_obsname);
						CString	tmp_watch = tmp_obsname.Left(tmp_obsname.Find('.'));

						// 同一グループを同一時間に設定する
						char* ans = strstr(res.obs_adr.sz_obsname, mon_time_name);
						if (ans != NULL){
#if 0
							time_val = res.obs_adr.ld_data;
#else
							GetAndUpdateMonTime(tmp_watch, res.obs_adr.ld_data);
#endif
							// うるう秒の確認
							CString str = CString(res.obs_adr.sz_sts_name);
							if (str.Right(2) == _T("60")){
								leapSec = true;
							}
						}

#if 0
						res.ld_time_val = time_val;
#else
						res.ld_time_val = GetAndUpdateMonTime(tmp_watch);
#endif

						if (leapSec == true){
							res.obs_adr.l_cnvResType |= 1 << eCnvResType_TimeLeapSec;
						}

						if (bFirst == true){
#if 0
							// 初回だけ、すでにdata_listに登録されているデータを同一時間に設定する
							vector<xobs_tbl_res_t>::iterator itr;
							for (itr = data_list.begin(); itr != data_list.end(); itr++){
								(*itr).ld_time_val = time_val;
							}
#endif
							bFirst = false;
						}

						// 対象監視項目名が存在するかのチェック
						if (IsObsName(res.obs_adr.sz_obsname, outDataList) == true){
							// 変換結果ステータス文字列のコード変更（UTF-8->SJIS EUC-JI->SJIS）
							if (outDataList.size() == 0)	// setlocalで落ちるため
								ConvertStatusChar(res.obs_adr.sz_sts_name, 64);

							// データリストに格納
							if (mCallBack == NULL){
								data_list.push_back(res);
							}
							else{
								CString obsname = CString(res.obs_adr.sz_obsname);
								TCHAR buf[40];
								swprintf_s(buf, 40, _T("%s"), (LPCTSTR)obsname);
								mCallBack(buf, (time_t)res.ld_time_val, &res, leapSec);
							}
						}
					}
				}
			} //for (l = 0; obs_tbl[l].obs_adr != NULL; l++) {

			//グループカウントアップ
			cnt++;

			// メモリ削除
			delete pdata;

		} //while (bRead)

		ret = 0;
	}
	catch (...){
		TRACE("@@@ CalcFromFileEx Exception\n");
		if (bRead == true){
			fclose(fp);
		}
		ret = -1;
	}

	DWORD dwElapsed = (timeGetTime() - dwStart);
	TRACE("### CalcFromFileEx(%s) = %.3lf(sec)\n", nama_data_file, (double)((double)dwElapsed / 1000.0));

 	timeEndPeriod(1);

	return ret;
}

/*============================================================================*/
/*! 監視データアクセス

-# 監視蓄積ファイルから工学値変換を行う

@param	nama_data_file	監視蓄積ファイル名
@param	outDataList		対象監視名一覧
@retval	int

*/
/*============================================================================*/
int CEngValCnvDll::CalcFromFile(char* nama_data_file)
{
	struct xobsname_t *obs_adrs;
	struct xext_tbl_t *ext_adr;
	char      *mon_adr;
	long      l_rc;

	long      l_dataid;
	int       l_inflg;
	int       l_grmd;
	double    d_data;
	long double ld_data;/* 工学値データ64bit整数を格納できる精度 */
	long      l_bitinfo;   /* 整数ビット数:整数抽出データのビット長 */
	char      sz_work[STR_BUF_SIZE];
	std::vector<xobs_tbl_res_t>::iterator itr;


	if (db_status != 0){
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: Error db_status(%d)"), __FILEW__, __LINE__, db_status);
			//			CLogTraceEx::Write(_T(""), _T("CEnvValCnv"), _T("CalcFromMsg"), _T("debug"), strDebug, nLogEx::debug);
		}
		return -1;
	}

	memcpy_s(db_data, mFileSize, db_data2, mFileSize);

	/*----- 各監視項目データ取得処理 -----*/
	mon_adr = db_data;
	mon_adr += (sizeof(long) * 2);

	data_list.clear();

	FILE *fp = NULL;
	bool bRead = true;
	int ret = 0;

	// 最小タイマ分解能を設定(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();

	try{
		int l = 0;
		int t = 0;
		while (1){
			obs_adrs = (struct xobsname_t *)mon_adr;

			long ed_id = ds_htonl(obs_adrs->l_id);
			long ed_len = ds_htonl(obs_adrs->l_len);

			t++;

			if (ed_id == 0) {
				break;
			}
			if (ed_id == OBSNAME_ID)
			{
				obs_tbl[l].obs_adr = obs_adrs;
				l_rc = ud_ext_adr(obs_tbl[l].obs_adr->sz_obsname, obs_adrs, &ext_adr);
				if (l_rc == 0) {
					obs_tbl[l].l_dataid = ds_htonl(ext_adr->s_pwb[0].l_pid);
					long ed_max = ds_htonl(ext_adr->l_pw_cnt);
					for (int m = 1; m < ed_max; m++) {
						if (obs_tbl[l].l_dataid != ds_htonl(ext_adr->s_pwb[m].l_pid)) {
							obs_tbl[l].l_dataid = -1;
							break;
						}
					}
				}
				else {
					obs_tbl[l].l_dataid = -2;
					obs_tbl[l].obs_adr->l_no = -1;
				}

				obs_tbl[l].l_offset = (long)(mon_adr - db_data);
				obs_tbl[l].obs_adr->l_inflg = 0;
				l++;
				if (l >= OBS_MAX) {
					{
						CString strDebug;
						strDebug.Format(_T("%s(%d) :: Error over OBS_MAX(%d)"), __FILEW__, __LINE__, l);
						//						CLogTraceEx::Write(_T(""), _T("CEnvValCnv"), _T("CalcFromMsg"), _T("debug"), strDebug, nLogEx::debug);
					}
					return(-1);
				}
			}
			mon_adr += ed_len;
		}
		obs_tbl[l].obs_adr = NULL;
		obs_tbl[l].l_offset = NULL;


		/*--------------------------------------------------------*/
		/*    監視データ通知                                      */
		/*--------------------------------------------------------*/
		int offset = 0;
		unsigned char* buff;
		long cnt = 0;
		char* mon_time_name = "MON_TIME";
		char   sz_work2[256] = { 0 };
		unsigned long ul_top_pos;

		// 監視データのオープン
		if ((fopen_s(&fp, nama_data_file, "rb")) != 0){
			bRead = false;
		}
		// 「#!HEAD:」が存在する場合は読み捨てる
		if ((fp == NULL) || (fread(sz_work2, sizeof(char), sizeof(sz_work2), fp) == -1)){/*#!HEAD:があるかどうか読んでみる*/
			// 読み込みエラーなので何もしない
			if (fp != NULL)
			{
				fclose(fp); /*ファイルをcloseする*/
			}
			bRead = false;
		}
		else{
			// 「#!HEAD:」が存在したので読み捨てる
			if (!strncmp(sz_work2, "#!HEAD:", 7)){ /*#!HEAD:を読み飛ばす*/
				if ((ul_top_pos = (unsigned long)strchr(sz_work2, '\n')) == NULL){
					if (fp != NULL)
					{
						fclose(fp); /*ファイルをcloseする*/
					}
					bRead = false;
				}
				else{
					// 監視データの先頭位置
					ul_top_pos = (ul_top_pos - (unsigned long)&sz_work2[0]) + 1;
				}
			}
			else{ /*ファイルの先頭から全内容を読み込む*/
				// ファイル先頭から監視データ
				ul_top_pos = 0; /*ファイルの先頭からメモリにロードする内容の先頭までのバイト数*/
			}
		}
		if (bRead == true){
			// 監視データ位置までシークする
			if (fseek(fp, ul_top_pos, SEEK_SET) == -1){
				fclose(fp); /*ファイルをcloseする*/
				bRead = false;
			}
		}

		bool bFirst = true;
		// ファイルENDまで以下を繰り返す
		while (bRead)
		{
			// 「TL_TLMDATA」単位でデータを読み込む
			xTL_TLMDATA* pdata;
			pdata = (xTL_TLMDATA*)MonRead(&fp);
			if (pdata == NULL)
			{
				TRACE("### CalcFromFile(%s) Error\n", nama_data_file);
				// ファイルの終端あるいは読み込みエラーなので処理を終了
				fclose(fp); /*ファイルをcloseする*/
				bRead = false;
				break;
			}

			//======================================================================
			//	ここから読み込んだ監視データをデータリストに格納する
			//======================================================================
			l_dataid = pdata->addinfo.uca_op_data_id[0];
			buff = (unsigned char *)&pdata->addinfo.uca_timetag;

			long double time_val = 0;
			bool leapSec = false; // うるう秒フラグ

			for (l = 0; obs_tbl[l].obs_adr != NULL; l++) {
				if (l_dataid == obs_tbl[l].l_dataid || obs_tbl[l].l_dataid == -1) {
					memset(sz_work, 0, STR_BUF_SIZE);
					l_rc = ud_montbl_data64(db_data, l_dataid, buff,
						obs_tbl[l].l_offset,
						&l_inflg,
						&l_grmd,
						&d_data,
						sz_work,
						&ld_data,
						&l_bitinfo);

					if (l_rc == 0 && l_inflg == 1) {
						obs_tbl[l].l_grp_cnt = cnt;
						obs_tbl[l].obs_adr->l_id = ds_htonl(obs_tbl[l].obs_adr->l_id);
						obs_tbl[l].obs_adr->l_len = ds_htonl(obs_tbl[l].obs_adr->l_len);
						obs_tbl[l].obs_adr->l_no = ds_htonl(obs_tbl[l].obs_adr->l_no);
						obs_tbl[l].obs_adr->l_inflg = l_inflg;
						obs_tbl[l].obs_adr->l_grmd = l_grmd;
						obs_tbl[l].obs_adr->d_data = d_data;
						obs_tbl[l].obs_adr->ld_data = ld_data;  /* 64bit精度拡張 */
						obs_tbl[l].obs_adr->l_bitinfo = l_bitinfo; /* 64bit精度拡張 */
						obs_tbl[l].obs_adr->l_cnvResType = GetCnvResType();
						strcpy_s(obs_tbl[l].obs_adr->sz_sts_name, sizeof(obs_tbl[l].obs_adr->sz_sts_name), sz_work);

						//結果を格納
						xobs_tbl_res_t res;
						res.l_dataid = obs_tbl[l].l_dataid;
						res.l_grp_cnt = obs_tbl[l].l_grp_cnt;
						res.l_offset = obs_tbl[l].l_offset;
						res.l_dataid = obs_tbl[l].l_dataid;
						memcpy_s(&res.obs_adr, sizeof(xobsname_t), obs_tbl[l].obs_adr, sizeof(xobsname_t));

						// 同一グループを同一時間に設定する
						char* ans = strstr(res.obs_adr.sz_obsname, mon_time_name);
						if (ans != NULL){
							time_val = res.obs_adr.ld_data;
							// うるう秒の確認
							CString str = CString(res.obs_adr.sz_sts_name);
							if (str.Right(2) == _T("60")){
								leapSec = true;
							}
						}
						res.ld_time_val = time_val;
						if (leapSec == true){
							res.obs_adr.l_cnvResType |= 1 << eCnvResType_TimeLeapSec;
						}

						if (bFirst == true){
							// 初回だけ、すでにdata_listに登録されているデータを同一時間に設定する
							vector<xobs_tbl_res_t>::iterator itr;
							for (itr = data_list.begin(); itr != data_list.end(); itr++){
								(*itr).ld_time_val = time_val;
								if (leapSec == true){
									res.obs_adr.l_cnvResType |= 1 << eCnvResType_TimeLeapSec;
								}
							}
							bFirst = false;
						}

						// 変換結果ステータス文字列のコード変更（UTF-8->SJIS EUC-JI->SJIS）
						ConvertStatusChar(res.obs_adr.sz_sts_name, 64);

						// データリストに格納
						data_list.push_back(res);

					}
				}
			} //for (l = 0; obs_tbl[l].obs_adr != NULL; l++) {

			//グループカウントアップ
			cnt++;

			// メモリ削除
			delete pdata;

		} //while (bRead)

		ret = 0;
	}
	catch (...){
		TRACE("@@@ CalcFromFile Exception\n");
		if (bRead == true){
			fclose(fp);
		}
		ret = -1;
	}

	DWORD dwElapsed = (timeGetTime() - dwStart);
	TRACE("### CalcFromFile(%s) = %.3lf(sec)\n", nama_data_file, (double)((double)dwElapsed / 1000.0));

	timeEndPeriod(1);

	return ret;
}

int CEngValCnvDll::CalcFromMsg(xsdtp_msg_ex_t* msg)
{
	char      *mon_adr;
	long      l_dsu;

	int       l_inflg;
	int       l_grmd;
	double    d_data;
	long double ld_data;/* 工学値データ64bit整数を格納できる精度 */
	long      l_bitinfo;   /* 整数ビット数:整数抽出データのビット長 */
	char      sz_work[STR_BUF_SIZE];
	long      l_rc;
	std::vector<xobs_tbl_res_t>::iterator itr;


	if (db_status != 0){
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: Error db_status(%d)"), __FILEW__, __LINE__, db_status);
//			CLogTraceEx::Write(_T(""), _T("CEnvValCnv"), _T("CalcFromMsg"), _T("debug"), strDebug, nLogEx::debug);
		}
		return -1;
	}

	// ★多分、メモリDBに直接値を書き込んでいるらしいので、初期化しない。
	//memcpy_s(db_data, mFileSize, db_data2, mFileSize);

	/*----- 各監視項目データ取得処理 -----*/
	mon_adr = db_data;
	mon_adr += (sizeof(long) * 2);

	data_list.clear();

	try{
		int l = 0;
		int t = 0;
		bool bFirst = true;

		/*--------------------------------------------------------*/
		/*    監視データ通知                                      */
		/*--------------------------------------------------------*/
		//		if (msg->l_objtype == DATA_PDU)
		{
			l_dsu = msg->uca_SdtpRecNum[0] * 256 + msg->uca_SdtpRecNum[1];

			int j = 0;
			long k = 0;
			long cnt = 0;
			char* mon_time_name = "MON_TIME";

//			long double time_val = 0;
			for (int i = 0; i < l_dsu; i++) {
				long l_dataid = msg->uca_buff[j + 1];

				vector<xobs_tbl_t> &list = obs_map[l_dataid];
				for (int l = 0; l < (int)list.size(); ++l) {
					xobs_tbl_t st = list[l];
					if (l_dataid == st.l_dataid || st.l_dataid == -1) {

						memset(sz_work, 0, STR_BUF_SIZE);
						l_rc = ud_montbl_data64(db_data, l_dataid, &msg->uca_buff[j + 12],
							st.l_offset,
							&l_inflg,
							&l_grmd,
							&d_data,
							sz_work,
							&ld_data,
							&l_bitinfo);

						if (l_rc == 0 && l_inflg == 1) {
							st.l_grp_cnt = cnt;
							st.obs_adr->l_id = ds_htonl(st.obs_adr->l_id);
							st.obs_adr->l_len = ds_htonl(st.obs_adr->l_len);
							st.obs_adr->l_no = ds_htonl(st.obs_adr->l_no);
							st.obs_adr->l_inflg = l_inflg;
							st.obs_adr->l_grmd = l_grmd;
							st.obs_adr->d_data = d_data;
							st.obs_adr->ld_data = ld_data;  /* 64bit精度拡張 */
							st.obs_adr->l_bitinfo = l_bitinfo; /* 64bit精度拡張 */
							st.obs_adr->l_cnvResType = GetCnvResType();
							strcpy_s(st.obs_adr->sz_sts_name, sizeof(st.obs_adr->sz_sts_name), sz_work);


							//結果を格納
							xobs_tbl_res_t res;
							res.l_dataid = st.l_dataid;
							res.l_grp_cnt = st.l_grp_cnt;
							res.l_offset = st.l_offset;
//							res.l_cnvResType = GetCnvResType();
							memcpy_s(&res.obs_adr, sizeof(xobsname_t), st.obs_adr, sizeof(xobsname_t));

							CString	tmp_obsname = CString(res.obs_adr.sz_obsname);
							CString	tmp_watch = tmp_obsname.Left(tmp_obsname.Find('.'));

							// 同一グループを同一時間に設定する
							char* ans = strstr(res.obs_adr.sz_obsname, mon_time_name);
							if (ans != NULL){
#if 0
								time_val = res.obs_adr.ld_data;
#else
								GetAndUpdateMonTime(tmp_watch, res.obs_adr.ld_data);
#endif
							}

#if 0
							res.ld_time_val = time_val;
#else
							res.ld_time_val = GetAndUpdateMonTime(tmp_watch);
#endif

							if (bFirst == true){
#if 0
								// 初回だけ、すでにdata_listに登録されているデータを同一時間に設定する
								vector<xobs_tbl_res_t>::iterator itr;
								for (itr = data_list.begin(); itr != data_list.end(); itr++){
									(*itr).ld_time_val = time_val;
								}
#endif
								bFirst = false;
							}

							// 変換結果ステータス文字列のコード変更（UTF-8->SJIS EUC-JI->SJIS）
							ConvertStatusChar(res.obs_adr.sz_sts_name, 64);

							data_list.push_back(res);
						}
					}
				}

				////MONTIMEを見つけて同じグループのItemに格納する
				//long double time_val = 0;
				//for (itr = data_list.begin(); itr != data_list.end(); itr++){
				//	CEngValCnvDll::obs_tbl_res_t d = *itr;
				//	if (d.l_grp_cnt == cnt){
				//		char* ans = strstr(d.obs_adr.sz_obsname, mon_time_name);
				//		if (ans != NULL){
				//			time_val = d.obs_adr.ld_data;
				//			break;
				//		}
				//	}
				//}
				//for (itr = data_list.begin(); itr != data_list.end(); itr++){
				//	CEngValCnvDll::obs_tbl_res_t* d = &*itr;
				//	if (d->l_grp_cnt == cnt){
				//		d->ld_time_val = time_val;
				//	}
				//}

				memcpy(&k, &msg->uca_buff[j + 8], sizeof(long));
				k = ntohl(k);
				if (k > MAX_SDTP / 2)
				{
					CString strDebug;
					strDebug.Format(_T("%s(%d) :: break uca buff size k(%d)"), __FILEW__, __LINE__, k);
//					CLogTraceEx::Write(_T(""), _T("CEnvValCnv"), _T("CalcFromMsg"), _T("debug"), strDebug, nLogEx::debug);
					break;
				}
				j += (k + 12);
				cnt++;
			}

		}

		return 0;
	}
	catch (CMemoryException* e)
	{
		e->Delete();
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: CMemoryException!"), __FILEW__, __LINE__);
//		CLogTraceEx::Write(_T(""), _T("CEnvValCnv"), _T("CalcFromMsg"), _T("debug"), strDebug, nLogEx::debug);
		return -1;
	}
	catch (...){
		return -1;
	}
}



int CEngValCnvDll::ud_montbl_data64(char    *shmadr, int l_dataid, unsigned char ca_data[], int l_offset,
	int *l_inflg, int *l_grmd, double *d_data, char sz_sts_name[],
	long double *ldp_data, long *lp_bitinfo)
{
	char   *mon_adr;
	struct xobsname_t *obs_adr;     /* 監視項目定義情報 */
	struct xobsname_t *obs_ex_adr;  /* 監視項目定義情報 */
	struct xif_tbl_t  *if_adr;      /* 条件定義情報 */
	struct xcal_tbl_t *cal_adr;     /* 他項目計算情報 */
	struct xext_tbl_t *ext_adr;     /* 各抽出定義情報 */
	struct xpol_tbl_t *pol_adr;     /* 方程式定義定義テーブル */
	struct xcarv_tbl_t *carv_adr;   /* 校正点カーブ定義テーブル */
	struct xtimecvt_tbl_t *time_adr; /* 時刻変換定義テーブル */
	struct xobssts_tbl_t *obssts_adr;/* ステータス情報テーブル */

	int     l_if_inflg;    /* 監視項目データ有効フラグ 1:有効 [OUT] */
	int     l_if_grmd;     /* ステータスフラグ 1:ステータス項目 [OUT] */
	double  d_if_data;     /* 工学値データ [OUT] */
	char    sz_work[256];
	char    *ca_adr;
	long    l_flg;
	long    l_ifflg[20];

	long    l_rc;
	long    i;
	long    j;
	long    k;
	long    l;
	long    l_dataf[10];

	long double         ld_dataf;
	long long           ll_data = 0;
	unsigned long long  ull_data = 0;

	double  d_dataf[10];
	float   f_data;
	long    l_data;

	/*----- 共有メモリのアドレスＧＥＴ -----*/
	for (i = 0; i < 20; i++) {
		l_ifflg[i] = 0;
	}

	//obs_adr = (struct obsname_t *)(shmadr[EQMON_DB_SHM_ID] + l_offset);
	//設備監視名称情報 from db
	obs_adr = (struct xobsname_t *)(shmadr + l_offset);
	if (ds_htonl(obs_adr->l_id) != OBSNAME_ID) {
		return -1;
	}

	*l_inflg = 0;    /* 監視項目データ有効フラグを無効 */
	*l_grmd = 0;
	*d_data = 0.0;

	*ldp_data = 0.0;  /* 工学値データ64bit整数を格納できる精度 */
	*lp_bitinfo = 0;  /* 整数ビット数:整数抽出データのビット長 */

	mon_adr = (char *)obs_adr;
	mon_adr += ds_htonl(obs_adr->l_len);

	m_lastCnvAfterType = 0;

	while (1) {

		obssts_adr = (struct xobssts_tbl_t *)mon_adr;

		//
		long ed_id = ds_htonl(obssts_adr->l_id);
		long d_len = ds_htonl(obssts_adr->l_len);

		if (ed_id == ENDOFOBS_ID) {
			return 0;
		}

		if (l_ifflg[0] != 0 && l_ifflg[0] != 2) {
			if (ed_id != IF_ID &&
				ed_id != ELSE_ID &&
				ed_id != ELSEIF_ID &&
				ed_id != ENDIF_ID) {
				mon_adr += d_len;
				continue;
			}
		}

		switch (ed_id) {
			/*-----------------------------------------------------------*/
			/*              抽出データ定義条件文のデータ処理             */
			/*-----------------------------------------------------------*/
		case EXT_ID:
		case EXTS_ID:
			ext_adr = (struct xext_tbl_t *)mon_adr;

			/* d_dataf[0]  = 0.0 ; EXT/EXTSでは使わない */
			/* l_data = 0 ; EXT/EXTSでは使わない */

			ld_dataf = 0.0; /* long double型式のアキュムレータ */

			for (i = 0; i < ds_htonl(ext_adr->l_pw_cnt); i++) { /* 抽出定義個数 */

				if (l_dataid == ds_htonl(ext_adr->s_pwb[i].l_pid)) {

					*l_inflg = 1;    /* 監視項目データ有効フラグを有効 */
					j = ds_htonl(ext_adr->s_pwb[i].l_e_bit) - ds_htonl(ext_adr->s_pwb[i].l_s_bit);
					j++; /* j:ビット長 */

					(*lp_bitinfo) += j; /* 整数ビット数:整数抽出データのビット長 */

					if (ds_htonl(ext_adr->l_pw_cnt) == 1){ /* 抽出定義が１つ：通常 */

						if (ds_htonl(ext_adr->l_id) == EXTS_ID){ /* 符号付き */

							ua_ldbitfieldll((unsigned long long*)&ll_data, &ca_data[L_OFFSET],
								ds_htonl(ext_adr->s_pwb[i].l_word),
								ds_htonl(ext_adr->s_pwb[i].l_s_bit), j, 1);
							ld_dataf = (long double)ll_data;

						}
						else{ /* 符号なし */

							ua_ldbitfieldll(&ull_data, &ca_data[L_OFFSET],
								ds_htonl(ext_adr->s_pwb[i].l_word),
								ds_htonl(ext_adr->s_pwb[i].l_s_bit), j, 0);
							ld_dataf = (long double)ull_data;
						}

					}
					else{ /* ビットを複数個所から集めて１つにする */

						if (i == 0 && ds_htonl(ext_adr->l_id) == EXTS_ID) { /* 符号つきの先頭 */

							ua_ldbitfieldll((unsigned long long *)&ll_data, &ca_data[L_OFFSET],
								ds_htonl(ext_adr->s_pwb[i].l_word),
								ds_htonl(ext_adr->s_pwb[i].l_s_bit), j, 1);
							ld_dataf = (long double)ll_data;

						}
						else { /* 符号なし、または符号付きの２つめ */

							ua_ldbitfieldll(&ull_data, &ca_data[L_OFFSET],
								ds_htonl(ext_adr->s_pwb[i].l_word),
								ds_htonl(ext_adr->s_pwb[i].l_s_bit), j, 0);

							ld_dataf *= (long double)((((long long)1) << j)); /* 上位ビットへ */

							if (ld_dataf >= 0.0) {
								ld_dataf += ull_data;
							}
							else {
								ld_dataf -= ull_data;
							}

						}

					}/* end if ビットを複数個所から集める場合 */
				}/* end if l_dataid */
			}/* end for i */

			(*ldp_data) += ld_dataf;  /* 工学値データ64bit整数を格納できる精度 */

			(*d_data) = (double)(*ldp_data); /* 従来どおりのdouble値 */
			m_lastCnvAfterType |= 1 << eCnvResType_LongDouble;
			break;

			/*-----------------------------------------------------------*/
			/*              抽出データ定義条件文のデータ処理             */
			/*-----------------------------------------------------------*/
		case EXTAVG_ID:
		case EXTAVGS_ID:
			ext_adr = (struct xext_tbl_t *)mon_adr;
			l_data = 0;
			for (i = 0; i < ds_htonl(ext_adr->l_pw_cnt); i++) {
				if (l_dataid == ds_htonl(ext_adr->s_pwb[i].l_pid)) {
					*l_inflg = 1;    /* 監視項目データ有効フラグを有効 */
					j = ds_htonl(ext_adr->s_pwb[i].l_e_bit) - ds_htonl(ext_adr->s_pwb[i].l_s_bit);
					j++;
					ua_ldbitfield((unsigned long *)&l_dataf[i], &ca_data[L_OFFSET],
						ds_htonl(ext_adr->s_pwb[i].l_word),
						ds_htonl(ext_adr->s_pwb[i].l_s_bit), j,
						ds_htonl(ext_adr->l_id) - EXTAVG_ID);
					l_data += l_dataf[i];
				}
			}
			(*d_data) += ((double)l_data / (double)(ds_htonl(ext_adr->l_pw_cnt)));

			(*ldp_data) = (*d_data); /* 工学値データ64bit整数を格納できる精度 */
			m_lastCnvAfterType |= 1 << eCnvResType_Double;
			break;

			/*-----------------------------------------------------------*/
			/*              抽出データ（double)のデータ処理              */
			/*-----------------------------------------------------------*/
		case EXTDBL_ID:
			ext_adr = (struct xext_tbl_t *)mon_adr;
			l_data = 0;
			if (l_dataid == ds_htonl(ext_adr->s_pwb[0].l_pid)) {
				*l_inflg = 1;    /* 監視項目データ有効フラグを有効 */
				memcpy(&d_dataf[0], &ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[0].l_word)], sizeof(double));
#ifdef _CUT
				(*d_data) += d_dataf[0];
				(*ldp_data) = (*d_data); /* 工学値データ64bit整数を格納できる精度 */
#else
				(*d_data) += dswap2(d_dataf[0]);
				(*ldp_data) = (*d_data); /* 工学値データ64bit整数を格納できる精度 */
#endif
				m_lastCnvAfterType |= 1 << eCnvResType_Double;
			}
			break;

			/*-----------------------------------------------------------*/
			/*              抽出データ（float)のデータ処理               */
			/*-----------------------------------------------------------*/
		case EXTFLT_ID:
			ext_adr = (struct xext_tbl_t *)mon_adr;
			l_data = 0;
			if (l_dataid == ds_htonl(ext_adr->s_pwb[0].l_pid)) {
				*l_inflg = 1;    /* 監視項目データ有効フラグを有効 */
				memcpy(&f_data, &ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[0].l_word)], sizeof(float));
#ifdef _CUT
				(*d_data) += f_data;
				(*ldp_data) = (*d_data); /* 工学値データ64bit整数を格納できる精度 */
#else
				(*d_data) += fswap(f_data);
				(*ldp_data) = (*d_data); /* 工学値データ64bit整数を格納できる精度 */
#endif
				m_lastCnvAfterType |= 1 << eCnvResType_Double;
			}
			break;

			/*-----------------------------------------------------------*/
			/*              文字列抽出データのデータ処理                 */
			/*-----------------------------------------------------------*/
		case EXTCHAR_ID:
			ext_adr = (struct xext_tbl_t *)mon_adr;
			l_data = 0;
			if (l_dataid == ds_htonl(ext_adr->s_pwb[0].l_pid)) {
				j = ds_htonl(ext_adr->s_pwb[0].l_e_bit) - ds_htonl(ext_adr->s_pwb[0].l_s_bit);
				j++;
				j /= 8;
				if (j < sizeof(obs_adr->sz_sts_name)) {
					memcpy(sz_sts_name, &ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[0].l_word)], j);
					sz_sts_name[j] = '\0';
					*l_inflg = 1;    /* 監視項目データ有効フラグを有効 */
					*l_grmd = 2;
					m_lastCnvAfterType |= 1 << eCnvResType_TimeStr;
				}
				else if (strlen((char *)&ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[0].l_word)]) < sizeof(obs_adr->sz_sts_name)) {
					*l_inflg = 1;    /* 監視項目データ有効フラグを有効 */
					*l_grmd = 2;
					m_lastCnvAfterType |= 1 << eCnvResType_TimeStr;
					//strcpy(sz_sts_name, (char *)&ca_data[L_OFFSET + ext_adr->s_pwb[0].l_word]);
					strcpy_s(sz_sts_name, STR_BUF_SIZE, (char *)&ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[0].l_word)]);
				}
				else {
					ca_adr = strrchr((char *)&ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[0].l_word)], '/');
					if (ca_adr == NULL || strlen(ca_adr) > sizeof(obs_adr->sz_sts_name)) {
						memcpy(sz_sts_name, &ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[0].l_word)], sizeof(obs_adr->sz_sts_name));
						*l_inflg = 1;    /* 監視項目データ有効フラグを有効 */
						*l_grmd = 2;
						m_lastCnvAfterType |= 1 << eCnvResType_TimeStr;
						sz_sts_name[sizeof(obs_adr->sz_sts_name) - 1] = '\0';
					}
					else {
						ca_adr++;
						*l_inflg = 1;    /* 監視項目データ有効フラグを有効 */
						*l_grmd = 2;
						m_lastCnvAfterType |= 1 << eCnvResType_TimeStr;
						strcpy_s(sz_sts_name, STR_BUF_SIZE, ca_adr);
					}
				}
				/*
				printf ( "%s = %s \n" , obs_adr->sz_obsname , &ca_data[L_OFFSET+ext_adr->s_pwb[0].l_word] ) ;
				printf ( "%s = %s \n" , obs_adr->sz_obsname , sz_sts_name ) ;
				*/
			}
			break;

			/*-----------------------------------------------------------*/
			/*              文字列型数値抽出データのデータ処理           */
			/*-----------------------------------------------------------*/
		case EXTCHARDEC_ID:
			ext_adr = (struct xext_tbl_t *)mon_adr;
			l_data = 0;
			i = 0;
			if (l_dataid == ds_htonl(ext_adr->s_pwb[i].l_pid)) {
				j = ds_htonl(ext_adr->s_pwb[i].l_e_bit) - ds_htonl(ext_adr->s_pwb[i].l_s_bit);
				j++;
				j /= 8;
				if (j < sizeof(obs_adr->sz_sts_name)) {
					memcpy(sz_sts_name, &ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[i].l_word)], j);
					sz_sts_name[j] = '\0';
					*l_inflg = 1;    /* 監視項目データ有効フラグを有効 */
					*l_grmd = 1;
					m_lastCnvAfterType |= 1 << eCnvResType_StatusStr;
				}
				else if (strlen((char *)&ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[i].l_word)]) < sizeof(obs_adr->sz_sts_name)) {
					*l_inflg = 1;    /* 監視項目データ有効フラグを有効 */
					*l_grmd = 1;
					m_lastCnvAfterType |= 1 << eCnvResType_StatusStr;
					strcpy_s(sz_sts_name, STR_BUF_SIZE, (char *)&ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[i].l_word)]);
				}
				else {
					j = sizeof(obs_adr->sz_sts_name) - 1;
					memcpy(sz_sts_name, &ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[i].l_word)], j);
					sz_sts_name[j] = '\0';
					*l_inflg = 1;    /* 監視項目データ有効フラグを有効 */
					*l_grmd = 1;
					m_lastCnvAfterType |= 1 << eCnvResType_StatusStr;
				}
				l_rc = uc_valchk(sz_sts_name, (int *)&l_data, &d_dataf[0], NULL);
				if (l_rc == 1) {
					(*d_data) += d_dataf[0];
					(*ldp_data) = (*d_data); /* 工学値データ64bit整数を格納できる精度 */
					m_lastCnvAfterType |= 1 << eCnvResType_Double;
				}
			}
			/*
			printf ( "%s = %s \n" , obs_adr->sz_obsname , &ca_data[L_OFFSET+ext_adr->s_pwb[i].l_word] ) ;
			printf ( "%s = %s \n" , obs_adr->sz_obsname , sz_sts_name ) ;
			*/
			break;

			/*-----------------------------------------------------------*/
			/*              条件文のデータ処理                           */
			/*-----------------------------------------------------------*/
		case IF_ID:
			if (l_ifflg[0] != 0) {
				memmove(&l_ifflg[1], &l_ifflg[0], sizeof(int) * 19);
				if (l_ifflg[1] != 0 && l_ifflg[1] != 2) {
					l_ifflg[0] = 3;
					break;
				}
			}
			//break;//★★★問題★★★
		case ELSEIF_ID:
			if (ed_id == ELSEIF_ID) {
				if (l_ifflg[0] != 1) {
					l_ifflg[0] = 3;
					break;
				}
			}
			if_adr = (struct xif_tbl_t  *)mon_adr;

			if (if_adr->l_offset == NULL) {
				l_rc = ud_sts_flg(if_adr->sz_obsname, NULL, shmadr, 0,
					&l_if_grmd, &if_adr->l_offset, NULL);
				if (l_rc != 0) {
					if_adr->l_offset = -1;
					l_rc = 0;
				}
			}

			if (ds_htonl(if_adr->l_offset) != -1) {
				l_rc = ud_montbl_data(shmadr, l_dataid, ca_data, if_adr->l_offset, &l_if_inflg, &l_if_grmd, &d_if_data, sz_work);
				if (l_rc != 0 || l_if_inflg == 0) {
					obs_ex_adr = (struct xobsname_t *)(shmadr + if_adr->l_offset);
					l_if_inflg = ds_htonl(obs_ex_adr->l_inflg);
					l_if_grmd = ds_htonl(obs_ex_adr->l_grmd);
					d_if_data = dswap(obs_ex_adr->d_data);
					strcpy_s(sz_work, sizeof(sz_work), obs_ex_adr->sz_sts_name);
				}
			}
			else {
				l_if_inflg = 0;
			}
			l_ifflg[0] = 1;
			if (l_rc == 0 && l_if_inflg != 0) {

				switch (ds_htonl(if_adr->l_flag)) {
				case 0:
					if (d_if_data == dswap(if_adr->d_val)) {
						l_ifflg[0] = 2;
					}
					break;
				case 1:
					if (d_if_data != dswap(if_adr->d_val)) {
						l_ifflg[0] = 2;
					}
					break;
				case 2:
					if (d_if_data < dswap(if_adr->d_val)) {
						l_ifflg[0] = 2;
					}
					break;
				case 3:
					if (d_if_data > dswap(if_adr->d_val)) {
						l_ifflg[0] = 2;
					}
					break;
				}
			}
			break;
			/*-----------------------------------------------------------*/
			/*              ELSE文のデータ処理                           */
			/*-----------------------------------------------------------*/
		case ELSE_ID:
			if (l_ifflg[0] != 0) {
				l_ifflg[0] ++;
			}
			break;
			/*-----------------------------------------------------------*/
			/*              ENDIF文のデータ処理                           */
			/*-----------------------------------------------------------*/
		case ENDIF_ID:
			if (l_ifflg[1] != 0) {
				memmove(&l_ifflg[0], &l_ifflg[1], sizeof(int) * 19);
			}
			else {
				l_ifflg[0] = 0;
			}
			break;
			/*-----------------------------------------------------------*/
			/*              他項目計算情報処理                           */
			/*-----------------------------------------------------------*/
		case CAL_ID:
			if (*l_inflg == 1) {
				cal_adr = (struct xcal_tbl_t  *)mon_adr;

				if (cal_adr->l_offset == NULL) {
					l_rc = ud_sts_flg(cal_adr->sz_obsname, NULL, shmadr, 0,
						&l_if_grmd, &cal_adr->l_offset, NULL);
					if (l_rc != 0) {
						cal_adr->l_offset = -1;
					}
				}

				if (ds_htonl(cal_adr->l_offset) != -1) {
					l_rc = ud_montbl_data(shmadr, l_dataid, ca_data, cal_adr->l_offset,
						&l_if_inflg, &l_if_grmd, &d_if_data, sz_work);
					if (l_rc != 0 || l_if_inflg == 0) {
						obs_ex_adr = (struct xobsname_t *)(shmadr + cal_adr->l_offset);
						l_if_inflg = ds_htonl(obs_ex_adr->l_inflg);//★★変換いらないかも★★
						l_if_grmd = ds_htonl(obs_ex_adr->l_grmd);//★★変換いらないかも★★
						d_if_data = dswap(obs_ex_adr->d_data);
						strcpy_s(sz_work, sizeof(sz_work), obs_ex_adr->sz_sts_name);

					}
					/*
					if ( strcmp ( obs_adr->sz_obsname , "CTRL.LINE_11" ) == NULL ) {
					printf ( "l_rc = %d\n" , l_rc ) ;
					printf ( "l_if_inflg = %d\n" , l_if_inflg ) ;
					printf ( "d_if_data = %lg\n" , d_if_data ) ;
					printf ( "obs_ex_adr->sz_obsname = %s\n" , obs_ex_adr->sz_obsname ) ;
					}
					*/
				}
				else {
					l_if_inflg = 0;
				}
				/*
				if ( strcmp ( obs_adr->sz_obsname , "CTRL.LINE_11" ) == NULL ) {
				printf ( "cal_adr->l_flag = %d , d_data = %lg , d_if_data = %lg\n" , cal_adr->l_flag , *d_data ,  d_if_data ) ;
				}
				*/
				if (l_rc == 0 && l_if_inflg != 0) {
					switch (ds_htonl(cal_adr->l_flag)) {
					case 0:
						(*d_data) += d_if_data;
						break;
					case 1:
						(*d_data) -= d_if_data;
						break;
					case 2:
						(*d_data) *= d_if_data;
						break;
					case 3:
						(*d_data) /= d_if_data;
						break;
					}
					(*ldp_data) = (*d_data); /* 工学値データ64bit整数を格納できる精度 */
					m_lastCnvAfterType |= 1 << eCnvResType_Double;
				}
				else {
					*l_inflg = 0;
				}
			}
			break;
			/*-----------------------------------------------------------*/
			/*            BCD抽出データ定義条件文のデータ処理             */
			/*-----------------------------------------------------------*/
		case EXTBCD_ID:
			ext_adr = (struct xext_tbl_t *)mon_adr;
			d_dataf[1] = 0.0;

			for (i = 0; i < ds_htonl(ext_adr->l_pw_cnt); i++) {

				if (l_dataid == ds_htonl(ext_adr->s_pwb[i].l_pid)) {
					*l_inflg = 1;    /* 監視項目データ有効フラグを有効 */

					j = ds_htonl(ext_adr->s_pwb[i].l_e_bit) - ds_htonl(ext_adr->s_pwb[i].l_s_bit);
					j++;
					ub_bcdext(&d_dataf[i], &ca_data[L_OFFSET],
						ds_htonl(ext_adr->s_pwb[i].l_word),
						ds_htonl(ext_adr->s_pwb[i].l_s_bit), j);
				}
			}
			*d_data += (d_dataf[0] * pow(10.0, d_dataf[1]));
			(*ldp_data) = (*d_data); /* 工学値データ64bit整数を格納できる精度 */
			m_lastCnvAfterType |= 1 << eCnvResType_Double;
			break;
			/*-----------------------------------------------------------*/
			/*            BCD抽出データ定義条件文のデータ処理            */
			/*-----------------------------------------------------------*/
		case EXTBCDS_ID:
			ext_adr = (struct xext_tbl_t *)mon_adr;
			d_dataf[1] = 0.0;
			k = 0;
			l = 0;
			for (i = 0; i < ds_htonl(ext_adr->l_pw_cnt); i++) {
				if (l_dataid == ds_htonl(ext_adr->s_pwb[i].l_pid)) {
					*l_inflg = 1;    /* 監視項目データ有効フラグを有効 */
					j = ds_htonl(ext_adr->s_pwb[i].l_e_bit) - ds_htonl(ext_adr->s_pwb[i].l_s_bit);
					j++;
					if ((i == 0 && ds_htonl(ext_adr->l_pw_cnt) != 1) ||
						(i == 2 && ds_htonl(ext_adr->l_pw_cnt) == 4)) {
						ua_ldbitfield((unsigned long *)&l_dataf[k], &ca_data[L_OFFSET],
							ds_htonl(ext_adr->s_pwb[i].l_word),
							ds_htonl(ext_adr->s_pwb[i].l_s_bit),
							j, 0);
						k++;
					}
					else {
						ub_bcdext(&d_dataf[l], &ca_data[L_OFFSET],
							ds_htonl(ext_adr->s_pwb[i].l_word),
							ds_htonl(ext_adr->s_pwb[i].l_s_bit), j);
						l++;
					}
				}
			}
			for (i = 0; i < k; i++) {
				if (l_dataf[i] != 0) {
					d_dataf[i] = -d_dataf[i];
				}
			}
			*d_data += (d_dataf[0] * pow(10.0, d_dataf[1]));
			(*ldp_data) = (*d_data); /* 工学値データ64bit整数を格納できる精度 */
			m_lastCnvAfterType |= 1 << eCnvResType_Double;
			break;
			/*-----------------------------------------------------------*/
			/*            方程式データ変換処理                           */
			/*-----------------------------------------------------------*/
		case POL_ID:
			pol_adr = (struct xpol_tbl_t *)mon_adr;
			//d_dataf[i] = 0.0;★★★問題点：既存コードのままだとスタックが破壊される★★★
			d_dataf[0] = 0.0;
			for (i = 0; i < ds_htonl(pol_adr->l_val_cnt); i++) {
				d_dataf[i] = 1.0;
				for (j = i + 1; j < ds_htonl(pol_adr->l_val_cnt); j++) {
					d_dataf[i] *= (*d_data);
				}
				d_dataf[i] *= dswap(pol_adr->d_val[i]);
			}
			*d_data = 0.0;
			for (i = 0; i < ds_htonl(pol_adr->l_val_cnt); i++) {
				(*d_data) += d_dataf[i];
			}
			(*ldp_data) = (*d_data); /* 工学値データ64bit整数を格納できる精度 */
			m_lastCnvAfterType |= 1 << eCnvResType_Double;
			break;

			/*-----------------------------------------------------------*/
			/*            カーブ変換処理                                 */
			/*-----------------------------------------------------------*/
		case CARVTBL_ID:
			carv_adr = (struct xcarv_tbl_t *)mon_adr;
			for (i = 0; i < ds_htonl(carv_adr->l_carv_cnt) - 1; i++) {
				if (*d_data < dswap(carv_adr->d_carv[i][0])) {
					if (i == 0) {
						*d_data = dswap(carv_adr->d_carv[i][1]);
					}
					else {
						d_dataf[0] = (*d_data - dswap(carv_adr->d_carv[i][0])) /
							(dswap(carv_adr->d_carv[i + 1][0]) - dswap(carv_adr->d_carv[i][0]));
						d_dataf[1] = (dswap(carv_adr->d_carv[i + 1][1]) - dswap(carv_adr->d_carv[i][1]));
						*d_data = d_dataf[0] * d_dataf[1] + dswap(carv_adr->d_carv[i][1]);
					}
					break;
				}
			}
			if (i == ds_htonl(carv_adr->l_carv_cnt) - 1) {
				*d_data = dswap(carv_adr->d_carv[i - 1][1]);
			}
			(*ldp_data) = (*d_data); /* 工学値データ64bit整数を格納できる精度 */
			m_lastCnvAfterType |= 1 << eCnvResType_Double;
			break;
			/*-----------------------------------------------------------*/
			/*            時刻データ変換処理                             */
			/*-----------------------------------------------------------*/
		case TIMECVT_ID:
			time_adr = (struct xtimecvt_tbl_t *)mon_adr;
			*l_grmd = 2;
			m_lastCnvAfterType |= 1 << eCnvResType_TimeStr;
			if (ds_htonl(time_adr->l_cvtno) == 1) {
				ub_dtime_to_asc(*d_data, sz_work);
				sz_work[19] = '\0';
				//strcpy(sz_sts_name, sz_work);
				strcpy_s(sz_sts_name, STR_BUF_SIZE, sz_work);
			}
			else {
				if (d_dataf[0] < 0.0) {
					d_dataf[0] = -(*d_data);
					l_flg = 1;
				}
				else {
					d_dataf[0] = *d_data;
					l_flg = 0;
				}

				for (i = 0; i < 3; i++) {
					l_dataf[i] = (long)fmod(d_dataf[0], 100.0);
					d_dataf[0] = floor(d_dataf[0] / 100.0);
				}

				if (ds_htonl(time_adr->l_cvtno) == 2) {
					l_dataf[i] = (long)d_dataf[0];
					sprintf_s(sz_sts_name, STR_BUF_SIZE, "%03d %02d:%02d:%02d", l_dataf[3],
						l_dataf[2], l_dataf[1], l_dataf[0]);

					*d_data = ua_ddd_to_dtime(&sz_sts_name[0],
						&sz_sts_name[4],
						&sz_sts_name[7],
						&sz_sts_name[10]);
					(*ldp_data) = (*d_data); /* 工学値データ64bit整数を格納できる精度 */
					m_lastCnvAfterType |= 1 << eCnvResType_Double;
					break;
				}
				else if (ds_htonl(time_adr->l_cvtno) == 3) {
					sprintf_s(sz_sts_name, STR_BUF_SIZE, "%02d:%02d:%02d",
						l_dataf[2], l_dataf[1], l_dataf[0]);
				}
				else if (ds_htonl(time_adr->l_cvtno) == 4) {
					if (l_flg == 0) {
						sprintf_s(sz_sts_name, STR_BUF_SIZE, "+%02d:%02d",
							l_dataf[1], l_dataf[0]);
					}
					else {
						sprintf_s(sz_sts_name, STR_BUF_SIZE, "-%02d:%02d",
							l_dataf[1], l_dataf[0]);
					}
				}
				else if (ds_htonl(time_adr->l_cvtno) == 5) {
					(*d_data) = ub_asc_to_dtime(sz_sts_name);
				}
#ifdef _GREAT_ENGVAL
				/* 【GREAT】timecvt 6 対応 */
				else if (ds_htonl(time_adr->l_cvtno) == 6) {
					if (l_flg == 0) {
						sprintf_s(sz_sts_name, STR_BUF_SIZE, "+%01d:%02d",
							l_dataf[1], l_dataf[0]);
					}
					else {
						sprintf_s(sz_sts_name, STR_BUF_SIZE, "-%01d:%02d",
							l_dataf[1], l_dataf[0]);
					}
				}
#endif /* _GREAT_ENGVAL */
			}
			(*ldp_data) = (*d_data); /* 工学値データ64bit整数を格納できる精度 */
			m_lastCnvAfterType |= 1 << eCnvResType_Double;
			break;
			/*-----------------------------------------------------------*/
			/*            ステータス変換処理                             */
			/*-----------------------------------------------------------*/
		case OBSSTS_ID:
			obssts_adr = (struct xobssts_tbl_t *)mon_adr;
			l_data = (long)(*d_data);
			*l_grmd = 1;
			m_lastCnvAfterType |= 1 << eCnvResType_StatusStr;
			//strcpy(sz_sts_name, obssts_adr->sz_i_sts);
			strcpy_s(sz_sts_name, STR_BUF_SIZE, obssts_adr->sz_i_sts);
			for (i = 0; i < ds_htonl(obssts_adr->l_sts_cnt); i++) {
				if (l_data == ds_htonl(obssts_adr->chk_t[i].l_val)) {
					strcpy_s(sz_sts_name, STR_BUF_SIZE, obssts_adr->chk_t[i].sz_sts);
					break;
				}
			}
			break;

#ifdef _GREAT_ENGVAL
			/* GREAT 工学値変換の改修 */
			/*-----------------------------------------------------------*/
			/*            常用対数(log10)変換処理                        */
			/*-----------------------------------------------------------*/
		case LOG10_ID:
			*d_data = log10(*d_data);
			(*ldp_data) = (*d_data); /* 工学値データ64bit整数を格納できる精度 */
			m_lastCnvAfterType |= 1 << eCnvResType_Double;
			break;

			/*-----------------------------------------------------------*/
			/*            常用対数(log10)逆変換処理                      */
			/*-----------------------------------------------------------*/
		case EXP10_ID:
			*d_data = pow(10.0, *d_data);
			(*ldp_data) = (*d_data); /* 工学値データ64bit整数を格納できる精度 */
			m_lastCnvAfterType |= 1 << eCnvResType_Double;
			break;

			/*-----------------------------------------------------------*/
			/*            逆正接(arctan)変換処理                         */
			/*-----------------------------------------------------------*/
		case ARCTAN_ID:
			*d_data = atan(*d_data);
			(*ldp_data) = (*d_data); /* 工学値データ64bit整数を格納できる精度 */
			m_lastCnvAfterType |= 1 << eCnvResType_Double;
			break;
#endif /* _GREAT_ENGVAL */

		}

		mon_adr += d_len;
	}
}


/*

共通ルーチンua_ldbitfield()は、３２ビットデータ(long)を抽出する仕様なので
抽出結果が、64bit-integer(long long)になるように作られていません。

long long 値を抽出したいのであれば、以下のようにしてください。

unsigned long long ull; //これが、求めたいlong long値

unsigned long ula_work[2];

ua_ldbitfield( &ula_work[0], inputbuf
, l_wordno, ul_bitno, ul_bitlen, l_signflg );
#ifdef I8086
ull = (((unsigned long long)( (ula_work[1] ))<<32)
|  ((unsigned long long)( (ula_work[0] ));
#else
ull = (((unsigned long long)( (ula_work[0] ))<<32)
|  ((unsigned long long)( (ula_work[1] ));
#endif
ull = ull >> (64 - ul_bitlen);

または、以下の、ua_ldbitfieldll()を使用してください。
*/
/*<a name="ua_ldbitfieldll"></a>
@function : ua_ldbitfieldll()
@name     : ビットフィールド抽出の64bit(long long)バージョン
@改定履歴 : 2007-02-15 Ver0.0 1st coding
@処理概要 : ua_ldbitfield()の64bit(long long)バージョン
@
@  1. バイト位置、ビット位置、ビット長を指定してビットフィールドを抽出する
@
@    「ビット位置」は、MSB を0 ビットとするオクテット内ビット番号(0..7)を指定する。
@        （ビット位置が、8 ならば次のオクテット内の MSB を指定した意味になる）
@
@       0 1 2 3 4 5 6 7
@      |+-+-+-+-+-+-+-+|
@      |MSB         LSB|
@      |---------------|
@
@  2. 抽出時に符号あり/なしを指定可能（符号付きデータの場合は２の補数表現
@     とみなし最上位ビットを符号拡張（64ビットの符号付きデータに）する）
@
@     W0のビット4から8ビット符号なしで抽出する場合
@
@       抽出元
@           W0      W1      W2      W3
@           01234567012345670123456701234567
@               11111111
@               --------
@
@       抽出結果(64bit値)
@           b0..........................................................b63
@           000000000000000000000000000000000000000000000000000000011111111
@                                                                  --------
@補足     :
@         抽出ビット長に65ビット以上のビット数を指定しても
@         64ビットの抽出を指定したものと見なします。
*/
//unsigned long long *pull_outbuf;/*output: 抽出結果を格納する64ビットのバッファ */
//unsigned char inputbuf[]; /*input:  抽出元バッファの先頭（W0）アドレス*/
//long l_wordno;            /*input:  抽出するバイト番号    ：0..n      */
//unsigned long ul_bitno;   /*input:  抽出ビット番号        ：0..n      */
//unsigned long ul_bitlen;  /*input:  抽出ビット長          ：1..64     */
//long l_signflg;           /*input:  符号指定：（0:符号なし
//						  1:抽出後符号拡張する）*/

void CEngValCnvDll::ua_ldbitfieldll(unsigned long long *pull_outbuf, unsigned char inputbuf[]
	, long l_wordno, unsigned long ul_bitno, unsigned long ul_bitlen, long l_signflg)
{
	unsigned long long ull; /*これが、求めたいlong long値*/
	long long           ll; /*これが、求めたいlong long値*/

	unsigned long ula_work[2];

	ua_ldbitfield(&ula_work[0], inputbuf
		, l_wordno, ul_bitno, ul_bitlen, l_signflg);

	if (ul_bitlen > 32){

		//#ifdef I8086
		ull = (((unsigned long long)((ula_work[1])) << 32))
			| ((unsigned long long)((ula_work[0])));

		if (l_signflg == 0){
			ull = ull >> (64 - ul_bitlen);
			*pull_outbuf = ull;
		}
		else{
			ll = ull;
			ll = ll >> (64 - (long)ul_bitlen);
			*pull_outbuf = ll;
		}

	}
	else{
		if (l_signflg == 0){
			*pull_outbuf = ula_work[0];
		}
		else{
			*pull_outbuf = (long long)((long)ula_work[0]);
		}
	}
}


/*<a name="ua_ldbitfield"></a>
@function : ua_ldbitfield()
@name     : ビットフィールド抽出
@改定履歴 : 97-10-23 Ver0.0 1st coding
@処理概要 :
@  1. バイト位置、ビット位置、ビット長を指定してビットフィールドを抽出する
@
@    「ビット位置」は、MSB を0 ビットとするオクテット内ビット番号(0..7)を指定する。
@        （ビット位置が、8 ならば次のオクテット内の MSB を指定した意味になる）
@
@       0 1 2 3 4 5 6 7
@      |+-+-+-+-+-+-+-+|
@      |MSB         LSB|
@      |---------------|
@
@  2. 抽出時に符号あり/なしを指定可能（符号付きデータの場合は２の補数表現
@     とみなし最上位ビットを符号拡張（32ビットの符号付きデータに）する）
@
@     W0のビット4から8ビット符号なしで抽出する場合
@
@       抽出元
@           W0      W1      W2      W3
@           01234567012345670123456701234567
@               11111111
@               --------
@
@       抽出結果(32bit値)
@           b0...........................b31
@           00000000000000000000000011111111
@                                   --------
@
@戻り値   :  なし
@補足     :
@  1. 32ビットのデータを抽出指定された場合は、符号拡張指定は、無視
@     して、32ビットデータそのものを抽出結果とする。
@  2. 33ビット以上のデータが指定された場合も、符号拡張指定は、無視
@     される。
@  3. 33ビット以上のデータが指定された場合は、ビットアライメントが
@     とれている(ビット番号==0)場合と、とれていない(ビット番号>0)
@     場合で抽出結果を格納する方法が異なり、以下のようになる。
@
@     ビット番号==0の場合:
@         抽出結果を格納するバッファには、抽出元データから必ず64ビット
@         のデータが転送される。
@
@     ビット番号>0の場合:
@         抽出結果を格納するバッファには、抽出元データから抽出ビット長
@         のデータが転送される。このとき、抽出結果を格納するバッファの
@         他のビットは変更されない（抽出ビット長のみのビットが設定され
@         る）。
@         また、抽出ビット長に65ビット以上のビット数を指定した場合でも
@         64ビットまでの転送しか行わない。
@
@   4. 64bit整数値(long long)を抽出結果として得たい場合は、
@      ビットフィールド抽出の64bit(long long)バージョン( ua_ldbitfieldll() )
@      が用意されています
*/

//unsigned long *pul_outbuf;/*output: 抽出結果を格納する32ビットまたは
//							  64ビットのバッファ                */
//unsigned char inputbuf[]; /*input:  抽出元バッファの先頭（W0）アドレス*/
//long l_wordno;            /*input:  抽出するバイト番号    ：0..n      */
//unsigned long ul_bitno;   /*input:  抽出ビット番号        ：0..n      */
//unsigned long ul_bitlen;  /*input:  抽出ビット長          ：1..64     */
//long l_signflg;           /*input:  符号指定：（0:符号なし
//						  1:抽出後符号拡張する）*/

void CEngValCnvDll::ua_ldbitfield(unsigned long *pul_outbuf, unsigned char inputbuf[]
	, long l_wordno, unsigned long ul_bitno, unsigned long ul_bitlen, long l_signflg)
{
	unsigned long ul_w;
	//unsigned char *p;
	//int i; 
	long l_w;
	register  unsigned long ul_w_r;
	register           long  l_w_r;

	l_wordno = l_wordno + (ul_bitno >> 3); /* バイト位置を求める */
	ul_bitno = ul_bitno & LOW3BIT;           /* バイト内のビット番号を求める */

	if (ul_bitlen <= 24){       /*24ビットまでのデータ*/
		ul_w_r = ((unsigned long)inputbuf[l_wordno] << 24)
			| ((unsigned long)inputbuf[l_wordno + 1] << 16)
			| ((unsigned long)inputbuf[l_wordno + 2] << 8)
			| (unsigned long)inputbuf[l_wordno + 3];
		ul_w_r = ul_w_r << (ul_bitno); /*先頭ビットをワードの先頭にsift:(93-11-05)*/


		if (l_signflg == 0){/*0:then abs data*/
			*pul_outbuf = ul_w_r >> (32 - ul_bitlen);/*最下位ビットをワードのLSBにsift*/
		}
		else{                                 /*最下位ビットをワードのLSBに算術sift*/
			l_w_r = (long)ul_w_r >> (32 - (long)ul_bitlen);
			*pul_outbuf = l_w_r;
		}

	}
	else if ((ul_bitlen == 32) && (ul_bitno == 0)){

		/*32ビットデータでビットアライメントが取れている場合*/

		ul_w_r = ((unsigned long)inputbuf[l_wordno] << 24)
			| ((unsigned long)inputbuf[l_wordno + 1] << 16)
			| ((unsigned long)inputbuf[l_wordno + 2] << 8)
			| (unsigned long)inputbuf[l_wordno + 3];
		*pul_outbuf = ul_w_r;

		/*
		#### 8086用抽出手順 ###############################################
		*/
	}
	else if (ul_bitlen >= 32){

		/*8086:32ビット以上データでビットアライメントが取れていない場合*/

		if (ul_bitlen >64){ ul_bitlen = 64; }
		ua_insbitfield((unsigned char *)pul_outbuf, 0L, ul_bitlen, (char *)&inputbuf[l_wordno], ul_bitno);

		if (ul_bitlen == 32){
			(*pul_outbuf) = ntohl(*pul_outbuf); /*long値でストア*/
		}
		else{
			ul_w_r = ntohl(*pul_outbuf);        /*double値とみてワード逆順でストア*/
			(*pul_outbuf) = ntohl(*(pul_outbuf + 1));
			(*(pul_outbuf + 1)) = ul_w_r;
		}


	}
	else{    /*25ビットから31ビットまでのデータ*/

		/*本当は、25ビットから64ビットまでをここで抽出して良いのであるが
		32bit/64bitデータがビット境界から始まることは無いので最適化
		のため上のような処理とした。
		*/

		ul_w = 0;
		ua_insbitfield((unsigned char *)&ul_w, 0L, ul_bitlen, (char *)&inputbuf[l_wordno], ul_bitno); /*(93-11-05)*/
		ul_w = ntohl(ul_w); /* mem1234 -> reg4321 -> reg1234 */

		if (l_signflg == 0){/*0:then abs data*/
			*pul_outbuf = (unsigned long)ul_w >> (32 - ul_bitlen);/*最下位ビットをワードのLSBにsift*/
		}
		else{                                 /*最下位ビットをワードのLSBに算術sift*/
			l_w = (long)ul_w >> (32 - (long)ul_bitlen);
			*pul_outbuf = l_w;
		}
	}
}


/*<a name="ua_stbitfield"></a>
@function : ua_stbitfield()
@name     : ビットフィールドへの値のストア(ua_ldbitfieldの逆)
@改定履歴 : 2011-05-15 Ver0.0 1st coding
@処理概要 :
@
@  1. 数値を、ビットフィールドへストアする。ビットフィールド内の
@    バイト位置、ビット位置、ビット長は引数で指定する。
@
@    「ビット位置」は、MSB を0 ビットとするオクテット内ビット番号(0..7)を指定する。
@        （ビット位置が、8 ならば次のオクテット内の MSB を指定した意味になる）
@
@       0 1 2 3 4 5 6 7
@      |+-+-+-+-+-+-+-+|
@      |MSB         LSB|
@      |---------------|
@
@
@     ストアしたい数値（ストアするビット長はLSB側から必要ビット取られる)
@
@           b0...........................b31(引数で指定した32bit値)
@           00000000000000000000000011111111
@                                   --------
@                                   <-8bit->
@
@     ストア先ビットフィールド（W0のビット4から8ビットストア）
@
@           W0      W1      W2      W3
@           01234567012345670123456701234567
@               11111111
@               --------
@               <-8bit->
@                 この8ビットのみ置き換えられる（他のビットは変化しない）
@
@補足     :
*/

//unsigned long ul_input;   /*input: ストアしたい数値
//							  */
//unsigned char outputbuf[];/*output: ストア先ビットフィールドの先頭（W0）アドレス*/
//long l_wordno;            /*input:  ストア先バイト番号    ：0..n      */
//unsigned long ul_bitno;   /*input:  ストア先ビット番号    ：0..n      */
//unsigned long ul_bitlen;  /*input:  ストア先ビット長      ：1..32     */

void CEngValCnvDll::ua_stbitfield(unsigned long ul_input, unsigned char outputbuf[],
	long l_wordno, unsigned long ul_bitno, unsigned long ul_bitlen)
{
	unsigned long ul_work;

	ul_work = htonl(ul_input << (32 - ul_bitlen));                       /*左に寄せる*/
	ua_insbitfield(&outputbuf[l_wordno], ul_bitno, ul_bitlen, (char *)&ul_work, 0); /*先頭ビット=b0からins*/
}


/*<a name="ua_insbitfield"></a>
@function : ua_insbitfield()
@name     : ビットフィールドインサート処理
@改定履歴 : 97-10-23 Ver0.0 1st coding
@処理概要 :
@  1. 指定された先の任意のビットフィールドを任意のビットフィールドで
@     置き換える（ビット単位のストア）
@
@     元データのビット 4 から 4 ビットをインサート先のビット 6 から挿入
@
@       インサートする元データ
@           b0....b7
@           xxxx1111
@               ----
@
@       インサート先
@           W0      W1      W2      W3
@           01234567012345670123456701234567
@           yyyyyy1111yyyyyyyyyyyyyyyyyyyyyy
@                 ----
@                 この４ビットのみ置き換えられる（他のビットは変化しない）
@
@戻り値   :  なし
@補足     :
*/

//char  *tgtadr;   /*input,output:インサート先先頭バイトアドレス*/
//long  l_inspos;  /*input:インサート先ビット位置:0..n          */
//long  l_inslen;  /*input:インサートビット長    :1..n          */
//char  *suadr;    /*input:インサートする元データの先頭バイトの
//				 アドレス                             */
//long  l_extpos;  /*input:インサートする元データのビット位置
//				 :0..7 MSBをビット0,LSBをビット7とする*/
void CEngValCnvDll::ua_insbitfield(unsigned char *tgtadr, long l_inspos, long l_inslen, char  *suadr, long l_extpos)
{
	register unsigned long ul_tgt, ul_su, ul_hi, ul_mid, ul_low;
	register long l_bytepos;

	while (l_inslen > 24){
		ua_insbitfield(tgtadr, l_inspos, 24L, suadr, l_extpos);
		tgtadr += 3; suadr += 3; l_inslen -= 24;
	}
	l_bytepos = l_inspos >> 3;       /*下位３ﾋﾞｯﾄmask(div 8)*/
	l_inspos = l_inspos & LOW3BIT;  /*下位３ﾋﾞｯﾄのみ取り出す(mod 8)*/

	/*ディストネーション側３２ビット値をregisterにloadする*/

	ul_tgt = ((unsigned long)*((unsigned char *)(tgtadr + l_bytepos)) << 24)
		| ((unsigned long)*((unsigned char *)(tgtadr + l_bytepos + 1)) << 16)
		| ((unsigned long)*((unsigned char *)(tgtadr + l_bytepos + 2)) << 8)
		| (unsigned long)*((unsigned char *)(tgtadr + l_bytepos + 3));

	/*ソース側３２ビットをregisterにloadする*/

	ul_su = ((unsigned long)*((unsigned char *)(suadr)) << 24)
		| ((unsigned long)*((unsigned char *)(suadr + 1)) << 16)
		| ((unsigned long)*((unsigned char *)(suadr + 2)) << 8)
		| (unsigned long)*((unsigned char *)(suadr + 3));


	if (l_inspos == 0){
		ul_mid = ul_su << l_extpos;
		ul_mid = ul_mid >> (32 - l_inslen);            /*LSBに寄せる:    0000mm */
		ul_mid = ul_mid << (32 - l_inslen);                           /* 00mm00 */
		ul_low = ul_tgt << l_inslen;
		ul_low = ul_low >> l_inslen;               /*上位bitfieldをｸﾘｱ:  0000LL */
		ul_tgt = ul_mid | ul_low;                   /*ﾋﾞｯﾄのﾏｰｼﾞ:         hhmmLL */
	}
	else{
		ul_hi = ul_tgt >> (32 - l_inspos);      /*insposより上位のﾋﾞｯﾄのみ残す*/
		ul_hi = ul_hi << (32 - l_inspos);                          /* hh0000 */
		ul_mid = ul_su << l_extpos;
		ul_mid = ul_mid >> (32 - l_inslen);            /*LSBに寄せる:    0000mm */
		ul_mid = ul_mid << (32 - (l_inspos + l_inslen));                /* 00mm00 */
		ul_low = ul_tgt << (l_inspos + l_inslen);
		ul_low = ul_low >> (l_inspos + l_inslen); /*上位bitfieldをｸﾘｱ:  0000LL */
		ul_tgt = ul_hi | ul_mid | ul_low;           /*ﾋﾞｯﾄのﾏｰｼﾞ:         hhmmLL */
	}


	/*ビットマージ結果（３２ビット）をディストネーション側にストア*/

	*((unsigned char *)(tgtadr + l_bytepos)) = ul_tgt >> 24;
	*((unsigned char *)(tgtadr + l_bytepos + 1)) = (unsigned char)(ul_tgt >> 16);
	*((unsigned char *)(tgtadr + l_bytepos + 2)) = (unsigned char)(ul_tgt >> 8);
	*((unsigned char *)(tgtadr + l_bytepos + 3)) = (unsigned char)ul_tgt;
}



/*
---------------------------------------------------------------------
$name = { 監視テーブルデータ抽出関数 }
$outline = {
指定の監視データバッファより抽出したい項目テーブルのデータを
抽出する。-->抽出するだけではなく工学値変換を行っている。
}
$comment = {
}
---------------------------------------------------------------------
*/
//char    *shmadr[];  /* DBテーブルアドレス uc_get_shm_adr ()で選られたアドレスの配列[IN] */
//int     l_dataid;   /* 監視項目データの装置番号 [IN] */
//unsigned char ca_data[];  /* 監視項目データのデータフィールド [IN] */
//int     l_offset;   /* 監視項目データのオフセット位置 [IN] */
//int     *l_inflg;    /* 監視項目データ有効フラグ 1:有効 [OUT] */
//int     *l_grmd;     /* ステータスフラグ 1:ステータス項目
//					 　　　　　　　　　　　　　　　　　　　　　　 2:時刻定義情報
//											0:工学値 [OUT] */
//double  *d_data;     /* 工学値データ [OUT] */
//char    sz_sts_name[];/* 文字列データ
//					  (MAX 64:sizeof(obsname_t.sz_sts_name)文字) [OUT] */

int CEngValCnvDll::ud_montbl_data(char *shmadr, int l_dataid, unsigned char ca_data[], int l_offset, int *l_inflg, int *l_grmd, double *d_data, char sz_sts_name[])
{
	long double ld_data;
	long        l_bitinfo;

	return  ud_montbl_data64(shmadr, l_dataid, ca_data, l_offset
		, l_inflg, l_grmd, d_data, sz_sts_name
		, &ld_data, &l_bitinfo);
}




/*
---------------------------------------------------------------------
$name = { BCDデータ抽出変換処理 }
$outline = {
指定のデータをＢＣＤ変換してデータを抜き出す。
}
$comment = {
}
---------------------------------------------------------------------
*/
//double *d_data;             /* 変換結果 */
//char   ca_data[];           /* 監視データ */
//long   l_word;              /* ワード位置 */
//long   l_bit;               /* ビット位置 */
//long   l_len;               /* サイズ */

void CEngValCnvDll::ub_bcdext(double *d_data, unsigned char ca_data[], long l_word, long l_bit, long l_len)
{
	long   i;
	long   l;
	long   m;
	//	long   n;
	long   l_data;

	l = l_len / 4;
	m = l_len % 4;
	if (m == 0) {
		m = 4;
	}
	else {
		l++;
	}

	*d_data = 0.0;
	for (i = 0; i < l; i++) {
		ua_ldbitfield((unsigned long *)&l_data, ca_data, l_word, l_bit, m, 0);
		l_bit += m;
		if (l_bit >= 8) {
			l_bit -= 8;
			l_word += 1;
		}
		m = 4;
		(*d_data) *= 10.0;
		(*d_data) += l_data;
	}
	return;
}

/*<a name="ub_dtime_to_asc"></a>
@function : ub_dtime_to_asc()
@name     : unix ut time code to ascii strings
@改定履歴 : 93-11-05 Ver1.0 1st coding
@改定履歴 : 2001-05-08 Ver1.2 0.99995秒が1.00000秒にくりあがらないBUG-FIX
@処理概要 : ut 時刻情報をYYYY-MM-DD HH:MM:SS.ssss文字列に変換する。
@戻り値   : なし
@補足     :
*/
//	double           d_uttime;     /* input : 時刻情報 */
//char            *sz_time;      /* output: YYYYを格納する領域 */
void CEngValCnvDll::ub_dtime_to_asc(double d_uttime, char *sz_time)
{
	long l_100usec;
	__time64_t long_time;
	struct  tm t;
	long    l_yyyy, l_mm;
	if (d_uttime <= -2147483648.0){ /* 0x80000000..0x7fffffff の範囲チェック */
		d_uttime = -2147483648.0;    /* これはinteger変換overflow-FPEが発生する*/
	}
	else if (d_uttime >= 2147483646.0){ /*ことを避けるためである。*/
		d_uttime = 2147483646.0;          /* 1998-03-18 */
	}
	long_time = (__time64_t)d_uttime;
	l_100usec = (long)((d_uttime - ((double)long_time) + 0.00005) * 10000); /* 100usec */
	if (l_100usec >= 10000){ /* 0.99995 は 1.00000 になる */
		l_100usec -= 10000;
		long_time++;           /* 秒の桁を補正 */
	}
	//t = localtime(&l_uttime); /*gmtime->localtime:1997-10-23*/
	_localtime64_s(&t, &long_time); /*gmtime->localtime:1997-10-23*/
	l_yyyy = t.tm_year + 1900;  /* 70 --> 1970 */
	l_mm = t.tm_mon + 1;      /* 0..11 -> 1..12 */

	//sprintf(sz_time, "%04d-%02d-%02d %02d:%02d:%02d.%04d", l_yyyy
	sprintf_s(sz_time, sizeof(sz_time), "%04d-%02d-%02d %02d:%02d:%02d.%04d", l_yyyy
		, l_mm, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, l_100usec);
	return;
}

/*<a name="ua_ddd_to_dtime"></a>
@function : ua_ddd_to_dtime()
@name     : ddd to dtime時刻変換処理
@改定履歴 : 95-12-27 Ver1.1 uk_ddd_to_dtime.c -> ua_ddd_to_dtime.c
@処理概要 : 各アーギュメントは、最後がNULLの文字列である必要はない。
@戻り値   : double time
@補足     :
時刻 char ddd : 通算日
char hh  : 時
char mm  : 分     ---->    double dtime : 1970年からの通算秒
char ss  : 秒
[外部参照関数]
void      ub_bcd6_to_bcd();
double    ub_bcd_to_dtime();
*/
//char    *cap_ddd;      /* i: 時刻(ddd) */
//char    *cap_hh;       /* i: 時刻(hh)  */
//char    *cap_mm;       /* i: 時刻(mm)  */
//char    *cap_ss;       /* i: 時刻(ss)  */
//
double CEngValCnvDll::ua_ddd_to_dtime(char *cap_ddd, char *cap_hh, char *cap_mm, char *cap_ss)
{
	unsigned char   uca_bcd[7];         /* 時刻(BCD)   */
	unsigned char   uca_year[2];        /* yyy(BCD)    */
	unsigned char   uca_time[7];        /* hhmmss(BCD) */

	/****  6byte BCD 変換 ****/
	uca_bcd[0] = cap_ddd[0] & 0x0f;
	uca_bcd[1] = ((cap_ddd[1] & 0x0f) << 4) + (cap_ddd[2] & 0x0f);
	uca_bcd[2] = ((cap_hh[0] & 0x0f) << 4) + (cap_hh[1] & 0x0f);
	uca_bcd[3] = ((cap_mm[0] & 0x0f) << 4) + (cap_mm[1] & 0x0f);
	uca_bcd[4] = ((cap_ss[0] & 0x0f) << 4) + (cap_ss[1] & 0x0f);
	uca_bcd[5] = 0x00;

#ifdef true
	fprintf(stderr, "in(%c%c%c,%c%c:%c%c:%c%c) -> BCD(%02x%02x,%02x:%02x:%02x)\n",
		cap_ddd[0], cap_ddd[1], cap_ddd[2], cap_hh[0], cap_hh[1],
		cap_mm[0], cap_mm[1], cap_ss[0], cap_ss[1],
		uca_bcd[0], uca_bcd[1], uca_bcd[2], uca_bcd[3],
		uca_bcd[4], uca_bcd[5]);
	fflush(stderr);
#endif

	/****  6byte BCD to 9byte BCD ****/
	ub_bcd6_to_bcd(uca_bcd, uca_year, uca_time);
#ifdef true
	fprintf(stderr, "year(%02x%02x),time(%02x%02x,%02x:%02x:%02x.%02x%02x)\n",
		uca_year[0], uca_year[1], uca_time[0], uca_time[1], uca_time[2],
		uca_time[3], uca_time[4], uca_time[5], uca_time[6]);
	fflush(stderr);
#endif

	/****  9byte BCD to dtime ****/
	return(ub_bcd_to_dtime(uca_year, uca_time));

}

/*<a name="ub_bcd6_to_bcd"></a>
@function : ub_bcd6_to_bcd()
@name     : ６ﾊﾞｲﾄBCD形式の時刻の９ﾊﾞｲﾄ形式時刻への変換処理
@改定履歴 : 95-02-07 Ver1.0 1st coding
@処理概要 : 0dddHHMMSStt --> yyyy, 0dddHHMMSSttt0
@戻り値   : なし
@補足     :
@    unsigned char achBCD6[6]    時刻情報(1970-01-01からの秒数)
@    unsigned char achYYYY[]     YYYYを格納する領域 2バイト
@    unsigned char achTime[]     時刻を格納する領域 ７バイト
*/
//unsigned char    achBCD6[6];   /* input : 時刻情報 */
//unsigned char    achYYYY[];    /* output: YYYYを格納する領域 */
//unsigned char    achTime[];    /* output: 時刻を格納する領域 */

void CEngValCnvDll::ub_bcd6_to_bcd(unsigned char achBCD6[], unsigned char achYYYY[], unsigned char achTime[])
{
	//long l_time;
	long long l_time;
	struct tm p_tm;
	static unsigned char uca_yyyy[2] = { 0, 0 };
	static unsigned char uc_old_dddhi = 0x7f;

	memcpy(achTime, achBCD6, 6);
	achTime[6] = 0;
	if (achTime[0] < uc_old_dddhi){
		l_time = time(NULL) + 1800; /* 1800sec = 30m : set date error limit */
		//p_tm = localtime(&l_time);          /*BCDﾌｫｰﾏｯﾄの年(4桁):1994..2001....*/
		_localtime64_s(&p_tm, &l_time);
		uca_yyyy[0] = (((p_tm.tm_year + 1900) / 1000) << 4) /* 年 1000の位 */
			+ ((p_tm.tm_year + 1900) % 1000 / 100);    /* 年  100の位 */
		uca_yyyy[1] = (((p_tm.tm_year + 1900) % 100 / 10) << 4) /* 年   10の位 */
			+ ((p_tm.tm_year + 1900) % 10);    /* 年    1の位 */
	};
	uc_old_dddhi = (achTime[0] & 0x03);
	achYYYY[0] = uca_yyyy[0];
	achYYYY[1] = uca_yyyy[1];
}

/*<a name="ub_bcd_to_dtime"></a>
@function : ub_bcd_to_dtime()
@name     : bcd time/ ascii time to unix ut time code
@改定履歴 : 93-11-05 Ver0.0 1st coding
@処理概要 : 1970-01-01 00:00:00 からの秒数求め処理モジュール
@戻り値   : n:通算秒
@補足     :
*/
//unsigned char	uca_yyyy[];	/* input: YYYY */
///*         0 1 2 3 4 5 6 */
//unsigned char	uca_time[];	/* input: 0DDDHHMMSSmmm0 */


double CEngValCnvDll::ub_bcd_to_dtime(unsigned char uca_yyyy[], unsigned char uca_time[])  /*  0 1 */
{
	long  l_year, l_day, l_hour, l_min, l_sec, l_100usec, l_uru;
	long  l_byou;
	int   i_cnt;
	long  l_timez;
	double d_sec;
	static long la_yyyysec[100] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };/*1970..2069*/
	/*
	年月日をセット
	*/
	l_year = (uca_yyyy[0] >> 4) * 1000 + (uca_yyyy[0] & 0x0f) * 100
		+ (uca_yyyy[1] >> 4) * 10 + (uca_yyyy[1] & 0x0f);
	l_day = (uca_time[0] & 0x0f) * 100
		+ (uca_time[1] >> 4) * 10 + (uca_time[1] & 0x0f);
	l_hour = (uca_time[2] >> 4) * 10 + (uca_time[2] & 0x0f);
	l_min = (uca_time[3] >> 4) * 10 + (uca_time[3] & 0x0f);
	l_sec = (uca_time[4] >> 4) * 10 + (uca_time[4] & 0x0f);
	l_100usec = (uca_time[5] >> 4) * 1000 + (uca_time[5] & 0x0f) * 100
		+ (uca_time[6] >> 4) * 10 + (uca_time[6] & 0x0f);
	/*
	求める秒数の計算
	*/
	l_uru = 0;
	if (la_yyyysec[1] == 0){ /*1971年==0ならばテーブル未作成*/
		for (l_byou = 0, i_cnt = 1970; i_cnt < YYYY_MAX; i_cnt++){
			if ((i_cnt % 4) != 0){ /*閏年では無い*/
				l_uru = 0;
			}
			else if ((i_cnt % 400) == 0){ /*400年に１回閏年である*/
				l_uru = 1;
			}
			else if ((i_cnt % 100) == 0){ /*100年に１回閏年で無い*/
				l_uru = 0;
			}
			else{
				l_uru = 1;                /*閏年*/
			}
			l_byou += (365L + l_uru) * 60L * 60L * 24L;
			la_yyyysec[i_cnt - 1970 + 1] = l_byou;
		}
	}
	if ((l_year < 1970) || (l_year > YYYY_MAX)){
		l_byou = 0;
	}
	else{
		l_byou = la_yyyysec[l_year - 1970];
	}

	if (g_l_ub_dtime_timezoneflg == 0){ /*1:timezoneは有効*/
		_tzset();
		g_l_ub_dtime_timezoneflg = 1; /*1:timezoneは有効*/
	}
	_get_timezone(&l_timez);
	l_byou += l_timez; /*これでUTの通算秒:1997-10-23*/
	l_byou += l_sec;

	d_sec = (double)l_byou
		+ (((double)l_100usec) / 10000.0)
		+ ((double)l_min      * 60.0)
		+ ((double)l_hour     * 3600.0)
		+ ((double)(l_day - 1)  * 86400.0);

	// PC時刻取得
	SYSTEMTIME st_UTC;
	GetSystemTime(&st_UTC);
	CTime ctimePC = CTime(st_UTC);
	double d_pc_time = (double)ctimePC.GetTime();

	long year_incdec = 0;
	// PCの年ずれ対策。新しい監視時刻が直近の監視から100日以上の場合は、1年引いて計算しなおす
	if (d_sec - d_pc_time >= (double)PCTIME_SPAN100)
	{
		double d_span = d_sec - d_pc_time;
		while (d_span >= (double)PCTIME_SPAN100)
		{
			d_span = d_span - (double)PCTIME_SPAN365;
			year_incdec--;
		}
	}
	// PCの年ずれ対策。新しい監視時刻が直近の監視時刻よりも100日以下の場合は、1年足して計算しなおす
	else if (d_pc_time - d_sec >= (double)PCTIME_SPAN100)
	{
		double d_span = d_pc_time - d_sec;
		while (d_span >= (double)PCTIME_SPAN100)
		{
			d_span = d_span - (double)PCTIME_SPAN365;
			year_incdec++;
		}
	}

	if (year_incdec != 0)
	{
		// PC時刻が大きくずれている場合、年を修正して時刻計算しなおす
		l_year = l_year + year_incdec;

		if ((l_year < 1970) || (l_year > YYYY_MAX)){
			l_byou = 0;
		}
		else{
			l_byou = la_yyyysec[l_year - 1970];
		}

		if (g_l_ub_dtime_timezoneflg == 0){ /*1:timezoneは有効*/
			_tzset();
			g_l_ub_dtime_timezoneflg = 1; /*1:timezoneは有効*/
		}
		_get_timezone(&l_timez);
		l_byou += l_timez; /*これでUTの通算秒:1997-10-23*/
		l_byou += l_sec;

		d_sec = (double)l_byou
			+ (((double)l_100usec) / 10000.0)
			+ ((double)l_min      * 60.0)
			+ ((double)l_hour     * 3600.0)
			+ ((double)(l_day - 1)  * 86400.0);
	}

	//	fprintf(stderr, "yyyy=%x:%x:date=%x:%x:", uca_yyyy[0], uca_yyyy[1], uca_time[0], uca_time[1]);
	//	fprintf(stderr, "year=%d,day=%d,hour=%d,min=%d,sec=%d,100usec=%d,uru=%d,byou=%d\n", l_year, l_day, l_hour, l_min, l_sec, l_100usec, l_uru, l_byou);

	return(d_sec);

}


/*<a name="ub_asc_to_dtime"></a>
@function : ub_asc_to_dtime()
@name     : 時刻文字列を通算秒にする。
@改定履歴 : 93-11-05 Ver1.0 1st coding
@処理概要 :
@戻り値   :  n :通算秒
@補足     :
*/
//char   *sz_time;   /* input: YYYY */
double CEngValCnvDll::ub_asc_to_dtime(char *sz_time)
{
	int i;
	struct tm t;
	double d_sec, d_0sec;
	long l_uttime;

	/*
	年月日をセット YYYY-MM-DD HH:MM:SS.mmmm
	*/
	if ((i = sscanf_s(sz_time, "%d-%d-%d %d:%d:%lf", &t.tm_year, &t.tm_mon
		, &t.tm_mday, &t.tm_hour, &t.tm_min, &d_sec)) < 6)
	{
		return (-1.0);
	};
	if ((d_sec < 0.0) || (d_sec >= 60.0)){ return (-1.0); }
	t.tm_sec = (int)floor(d_sec);
	d_0sec = d_sec - t.tm_sec;
	if ((t.tm_mon  < 1) || (t.tm_mon  > 12)){ return (-1.0); }
	if ((t.tm_mday < 1) || (t.tm_mday > 31)){ return (-1.0); }
	if ((t.tm_hour < 0) || (t.tm_hour > 23)){ return (-1.0); }
	if ((t.tm_min  < 0) || (t.tm_min  > 59)){ return (-1.0); }
	if (t.tm_year < 70){ t.tm_year += 2000; }; /* yyyy == 00 --> 2000 */
	if (t.tm_year > 1900){ t.tm_year -= 1900; }; /* yyyy == 1970 --> 70 */
	t.tm_mon -= 1;  /* mon == 1..12 --> 0..11 */
	t.tm_isdst = 0; /* daylight flg off */
	if ((l_uttime = (long)mktime(&t)) == -1){
		return (-1.0);
	};
	/* l_uttime -= timezone; /@ local-time to UTC:削除:1997-10-23 */

#ifdef DEBUGTIMEZONE
	fprintf(stderr, "(uc_asc_to_dtime=%s:%lf)", asctime(&t), d_0sec);
#endif

	return((double)l_uttime + d_0sec);
}


/*
---------------------------------------------------------------------
$name = { 監視項目ワード位置テーブルアドレス検索 }
$outline = {
指定の監視項目ワード位置テーブルのアドレスを検索する
}
$comment = {
}
---------------------------------------------------------------------
*/
//char             sz_watch[]; /* 監視項目名称 [IN] */
//char             *shmadr[];  /* DBテーブルアドレス
//							 uc_get_shm_adr ()で選られたアドレスの配列[IN] */
//struct ext_tbl_t *ext_adr[]; /* 各抽出定義情報[OUT] */

int CEngValCnvDll::ud_ext_adr(char sz_watch[], xobsname_t *obs_adr, xext_tbl_t *ext_adr[])
{
	char *mon_adr;
	/*----- 共有メモリのアドレスＧＥＴ -----*/

	mon_adr = (char *)obs_adr;
	while (1) {
		ext_adr[0] = (struct xext_tbl_t *)mon_adr;

		long ed_id = ds_htonl(ext_adr[0]->l_id);
		if (ed_id == EXT_ID ||
			ed_id == EXTS_ID ||
			ed_id == EXTAVG_ID ||
			ed_id == EXTAVGS_ID ||
			ed_id == EXTCHAR_ID ||
			ed_id == EXTCHARDEC_ID ||
			ed_id == EXTDBL_ID ||
			ed_id == EXTFLT_ID ||
			ed_id == EXTBCD_ID ||
			ed_id == EXTBCDS_ID) {
			return 0;
		}
		if (ed_id == ENDOFOBS_ID) {
			return -1;
		}

		long ed_len = ds_htonl(ext_adr[0]->l_len);
		mon_adr += ed_len;
	}
}


double CEngValCnvDll::dswap(double d)
{
#if IS_BIG_ENDIA
	double s = d;
	char *ps((char*)&s), *pd((char*)&d);

	ps[0] = pd[7];
	ps[1] = pd[6];
	ps[2] = pd[5];
	ps[3] = pd[4];
	ps[4] = pd[3];
	ps[5] = pd[2];
	ps[6] = pd[1];
	ps[7] = pd[0];

	return s;
#else
	return d;
#endif
}

double CEngValCnvDll::dswap2(double d)
{
	double s = d;
	char *ps((char*)&s), *pd((char*)&d);

	ps[0] = pd[7];
	ps[1] = pd[6];
	ps[2] = pd[5];
	ps[3] = pd[4];
	ps[4] = pd[3];
	ps[5] = pd[2];
	ps[6] = pd[1];
	ps[7] = pd[0];

	return s;
}

float CEngValCnvDll::fswap(float d)
{
	float s = d;
	char *ps((char*)&s), *pd((char*)&d);

	ps[0] = pd[3];
	ps[1] = pd[2];
	ps[2] = pd[1];
	ps[3] = pd[0];

	return s;
}


long CEngValCnvDll::ds_htonl(long val)
{
#if IS_BIG_ENDIA
	return htonl(val);
#else
	return val;
#endif


}

/*<a name="uc_valchk"></a>
@function : uc_valchk()
@name     : 数値文字列チェック＆バイナリ値変換処理
@改定履歴 : 92-05-29 Ver1.0 1st coding  mika
@改定履歴 : 92-07-03 Ver1.1   mika
@改定履歴 : 92-08-17 Ver1.2   mika
@改定履歴 : 97-01-16 Ver2.0   T.S.
@改定履歴 : 97-12-17 Ver2.1   T.S. (K/M対応)
@改定履歴 : 98-04-09 Ver2.2   T.S. (8086対応)
@改定履歴 : 2000-08-23 Ver2.4 ８進数(0t777,0T777)対応
@処理概要 : 変換する文字列を与え数値文字列チェックとバイナリ値変換を行う
@            1) s        ---<  input : sz_ :  n-byte : >---
@                             変換する文字列
@            2)i_p       ---<  output : long  :  1  :  >---
@                             変換された4バイトのバイナリデータ
@                             (i_p=NULLの場合は結果は戻さない)
@            3)d_p       ---<  output  : double : 1  : >---
@                             変換された8バイトのdoubleデータ
@                             (d_p=NULLの場合は結果は戻さない)

@                             ('0x'+16進数, '0b'+2進数, '0t'+8進数の
@                              場合は符号なしデータとみなすので注意)

@                             このポインタ値が -1 の場合は第４パラ
@                             メータ(l_opt)が有効になる

@            4)l_opt     ---<  input  :  long  :  1  : >---
@                             1000 または、1024 を指定すると、数値
@                             の後ろに K M が付加されていた場合に
@                             変換値として、K:1000倍または1024倍
@                             M:1000*1000倍または1024*1024倍の値
@                             を返す
@
@戻り値   :    (チェック/変換されたデータの型を示す)
@              -1 : 数値ではない
@                          (変換結果はもどさなかった)
@               0 : sがNULLポインタであった
@                          (変換結果はもどさなかった)
@               1 : long型の数値として変換した
@               (10進数, '0x'+16進数, '0b'+2進数の場合)
@               (8進数, '0t'+8進数の場合)
@
@               2 : double型の数値として変換した
@                   (999.99, 99.9e99形式の場合)
@                   (longではオーバーフローする場合)
@
@補足     :
*/
//char    *s;                             /* 変換する文字列 */
//INT32bit *i_p;                           /* 変換された4バイトのバイナリデータ */
//double  *d_p;                           /* 変換された8バイトのdoubleデータ   */
//INT32bit l_opt;                          /* d_p==-1の時のみ有効*/

int CEngValCnvDll::uc_valchk(char *s, int *i_p, double *d_p, int l_opt)
{
	int     i = 2;                            /* 文字列カウンタ */
	int     j = 0;                            /* カウンタ */
	char    sz_dmy[512];                    /* sscanfダミーバッファ */

	long    l_bitbuf = 0;                     /* ビットシフトバッファ */
	double  d_bitbuf = 0.0;                   /* 32bit以上の場合 */
	long    l_data;                         /* 計算結果データ  */
	double  d_data;

	union   xumoji  mojibuf;                 /* 文字列先頭2文字のバッファ */



	/*********************************************************************/
	/*                             PROCEDURE                             */
	/*********************************************************************/

	/* sがNULLか？ */
	if (s == NULL)  return(0);

	/* 文字列先頭2文字の取得 */
	//strncpy(mojibuf.c_moji, 2, s);
	//strncpy_s(mojibuf.c_moji, 2, s, 2);
	memcpy(mojibuf.c_moji, s, 2);

	/* 文字列先頭2文字により処理を選択 */
	switch (mojibuf.s_moji) {

	case LX: /* 0X */
	case SX: /* 0x */
		while (s[i] != iNULL) {  /* 2005-1-7 */
			l_bitbuf = l_bitbuf << 4;    /* 左へ4ビットシフト */
			d_bitbuf = d_bitbuf * 16;
			if (0x30 <= s[i] && 0x39 >= s[i]) {
				l_data = (long)(s[i] - 0x30);
				l_bitbuf = l_bitbuf | l_data; /* OR */
				d_bitbuf = d_bitbuf + l_data;
			}
			else  if (0x61 <= (s[i] = tolower(s[i])) && 0x66 >= (s[i] = tolower(s[i]))) {
				l_data = (long)(s[i] - 0x57);
				l_bitbuf = l_bitbuf | l_data;  /* OR */
				d_bitbuf = d_bitbuf + l_data;
			}
			else {
				return(-1);
			}
			i++;
		}
		if (i_p != NULL){ *i_p = l_bitbuf; }
		if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_bitbuf; }
		return(1);
		break;

	case LB: /* 0B */
	case SB: /* 0b */
		while (s[i] != iNULL) { /* 2005-1-7 */
			l_bitbuf = l_bitbuf << 1;    /* 左へ1ビットシフト */
			d_bitbuf = d_bitbuf * 2;
			switch (s[i]) {
			case '0': break;
			case '1': l_bitbuf = l_bitbuf + 1;
				d_bitbuf = d_bitbuf + 1;
				break;
			default:  return(-1);
			}
			i++;
		}
		if (i_p != NULL){ *i_p = l_bitbuf; }
		if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_bitbuf; }
		return(1);
		break;

	case LT: /* 0T */
	case ST: /* 0t */
		while (s[i] != iNULL) {  /* 2005-1-7 */
			l_bitbuf = l_bitbuf << 3;    /* 左へ3ビットシフト */
			d_bitbuf = d_bitbuf * 8;
			if ((s[i] >= '0') && (s[i] <= '7')){
				l_bitbuf = l_bitbuf + (s[i] - '0'); /* 0..7 */
				d_bitbuf = d_bitbuf + (s[i] - '0'); /* 0..7 */
			}
			else{
				return(-1);
			}
			i++;
		}
		if (i_p != NULL){ *i_p = l_bitbuf; }
		if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_bitbuf; }
		return(1);
		break;

	default:
		if ((int)d_p == -1){
			//j = sscanf(s, "%lf%s", &d_data, sz_dmy);
			//j = sscanf_s(s, "%lf%s", &d_data, sz_dmy, sizeof(sz_dmy));
			j = sscanf_s(s, "%lf%s", &d_data, sz_dmy, (unsigned)_countof(sz_dmy));
			if (j == 1){
				; /*d_dataをそのまま返す*/
			}
			else if (j == 2){
				if (!strcmp(sz_dmy, "K")){
					d_data = d_data * l_opt;
				}
				else if (!strcmp(sz_dmy, "M")){
					d_data = d_data * l_opt * l_opt;
				}
				else{
					return(-1); /*ERROR*/
				}
			}
			else{
				return(-1); /*ERROR*/
			}
			if (i_p != NULL){ *i_p = (int)d_data; }
			return(1); /*整数値*/
		}
		//if (sscanf(s, "%lf%s", &d_data, sz_dmy) != 1){ return(-1); }
		//if (sscanf_s(s, "%lf%s", &d_data, sz_dmy, sizeof(sz_dmy)) != 1){ return(-1); }
		if (sscanf_s(s, "%lf%s", &d_data, sz_dmy, (unsigned)_countof(sz_dmy)) != 1){ return(-1); }
		if ((d_data >  2147483647.0) || (d_data < -2147483648.0)){
			if (i_p != NULL){ *i_p = 0x80000000; }
			if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_data; }
			return(2); /*integerがオーバーフローするので実数値*/
		}
		else{
			/* 整数値? */
			if ((strchr(s, '.') == NULL) && (strchr(s, 'e') == NULL) && (strchr(s, 'E') == NULL)){
				if (i_p != NULL){ *i_p = (int)d_data; }
				if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_data; }
				return(1); /*整数値*/
			}
			else{
				if (i_p != NULL){ *i_p = (int)d_data; }
				if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_data; }
				return(2);
			}
		}
	}/*end switch*/
}

/***********************************************************************************
* uc_valchk()の64ビット(long long)バージョン
************************************************************************************/

/*<a name="uc_valchkll"></a>
@function : uc_valchkll()
@name     : 数値文字列チェック＆バイナリ値変換処理の64bit(long long)バージョン
@改定履歴 : 2010-09-19 Ver0.0 1st coding
@処理概要 : uc_valchk()の64bit(long long)バージョン
@補足     :
*/
//char        *s;     /*input:  変換する文字列 */
//INT64bit    *i_p;   /*output: 変換された8バイトのバイナリデータ */
//long double *d_p;   /*output: 変換された10.or.16バイトのlong doubleデータ   */
//long         l_opt; /*input:  通常は、iNULLを指定すること;d_p==-1の時のみ有効 */


int CEngValCnvDll::uc_valchkll(char *s, long *i_p, long double *d_p, long l_opt)
{
	int     i = 2;                            /* 文字列カウンタ */
	int     j = 0;                            /* カウンタ */
	char    sz_dmy[512];                    /* sscanfダミーバッファ */

	unsigned long long l_bitbuf = 0;          /* ビットシフトバッファ */
	long double        d_bitbuf = 0.0;        /* 32bit以上の場合 */
	long long          l_data;              /* 計算結果データ  */
	long double        d_data;

	union   xumoji  mojibuf;                 /* 文字列先頭2文字のバッファ */



	/*********************************************************************/
	/*                             PROCEDURE                             */
	/*********************************************************************/

	/* sがNULLか？ */
	if (s == NULL)  return(0);

	/* 文字列先頭2文字の取得 */
	//strncpy(mojibuf.c_moji, s, 2);
	//	strcpy_s(mojibuf.c_moji, 2, s);
	memcpy(mojibuf.c_moji, s, 2);

	/* 文字列先頭2文字により処理を選択 */
	switch (mojibuf.s_moji) {

	case LX: /* 0X */
	case SX: /* 0x */
		while (s[i] != iNULL) {  /* 2005-1-7 */
			l_bitbuf = l_bitbuf << 4;    /* 左へ4ビットシフト */
			d_bitbuf = d_bitbuf * 16;
			if (0x30 <= s[i] && 0x39 >= s[i]) {
				l_data = (long long)(s[i] - 0x30);
				l_bitbuf = l_bitbuf | l_data; /* OR */
				d_bitbuf = d_bitbuf + l_data;
			}
			else  if (0x61 <= (s[i] = tolower(s[i])) && 0x66 >= (s[i] = tolower(s[i]))) {
				l_data = (long long)(s[i] - 0x57);
				l_bitbuf = l_bitbuf | l_data;  /* OR */
				d_bitbuf = d_bitbuf + l_data;
			}
			else {
				return(-1);
			}
			i++;
		}
		if (i_p != NULL){ *i_p = (long)l_bitbuf; }
		if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_bitbuf; }
		return(1);
		break;

	case LB: /* 0B */
	case SB: /* 0b */
		while (s[i] != iNULL) { /* 2005-1-7 */
			l_bitbuf = l_bitbuf << 1;    /* 左へ1ビットシフト */
			d_bitbuf = d_bitbuf * 2;
			switch (s[i]) {
			case '0': break;
			case '1': l_bitbuf = l_bitbuf + 1;
				d_bitbuf = d_bitbuf + 1;
				break;
			default:  return(-1);
			}
			i++;
		}
		if (i_p != NULL){ *i_p = (long)l_bitbuf; }
		if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_bitbuf; }
		return(1);
		break;

	case LT: /* 0T */
	case ST: /* 0t */
		while (s[i] != iNULL) {  /* 2005-1-7 */
			l_bitbuf = l_bitbuf << 3;    /* 左へ3ビットシフト */
			d_bitbuf = d_bitbuf * 8;
			if ((s[i] >= '0') && (s[i] <= '7')){
				l_bitbuf = l_bitbuf + (s[i] - '0'); /* 0..7 */
				d_bitbuf = d_bitbuf + (s[i] - '0'); /* 0..7 */
			}
			else{
				return(-1);
			}
			i++;
		}
		if (i_p != NULL){ *i_p = (long)l_bitbuf; }
		if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_bitbuf; }
		return(1);
		break;

	default:
		if ((int)d_p == -1){
			//j = sscanf(s, "%Lf%s", &d_data, sz_dmy); /* %Lf: long double */
			//j = sscanf_s(s, "%Lf%s", &d_data, sz_dmy, sizeof(sz_dmy)); /* %Lf: long double */
			j = sscanf_s(s, "%Lf%s", &d_data, sz_dmy, (unsigned)_countof(sz_dmy)); /* %Lf: long double */
			if (j == 1){
				; /*d_dataをそのまま返す*/
			}
			else if (j == 2){
				if (!strcmp(sz_dmy, "K")){
					d_data = d_data * l_opt;
				}
				else if (!strcmp(sz_dmy, "M")){
					d_data = d_data * l_opt * l_opt;
				}
				else{
					return(-1); /*ERROR*/
				}
			}
			else{
				return(-1); /*ERROR*/
			}
			if (i_p != NULL){ *i_p = (long)d_data; }
			return(1); /*整数値*/
		}
		//if (sscanf(s, "%Lf%s", &d_data, sz_dmy) != 1){ return(-1); } /* %Lf: long double */
		//if (sscanf_s(s, "%Lf%s", &d_data, sz_dmy, sizeof(sz_dmy)) != 1){ return(-1); } /* %Lf: long double */
		if (sscanf_s(s, "%Lf%s", &d_data, sz_dmy, (unsigned)_countof(sz_dmy)) != 1){ return(-1); } /* %Lf: long double */
		if ((d_data > 9223372036854775807.0L) || (d_data < -9223372036854775808.0L)){
			if (i_p != NULL){ *((unsigned long long *)i_p) = 0x8000000000000000LL; }
			if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_data; }
			return(2); /*integerがオーバーフローするので実数値*/
		}
		else{
			/* 整数値? */
			//if (sscanf(s, "%lld%s", &l_data, sz_dmy) == 1){ /* 整数 */
			//if (sscanf_s(s, "%lld%s", &l_data, sz_dmy, sizeof(sz_dmy)) == 1){ /* 整数 */
			if (sscanf_s(s, "%lld%s", &l_data, sz_dmy, (unsigned)_countof(sz_dmy)) == 1){ /* 整数 */

				if (i_p != NULL){ *i_p = (long)l_data; }
				if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_data; }
				return(1); /*整数値*/

			}
			else{ /* 整数でないなら実数 */

				if (i_p != NULL){ *i_p = (long)d_data; }
				if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_data; }
				return(2);

			}
		}
	}/*end switch*/
}

/*
---------------------------------------------------------------------
$name = { 設備監視ＤＢファイル監視項目チェック }
$outline = {
設備監視ＤＢファイルより監視項目名称が存在するかチェックし、
そのデータがステータス項目か調べる。
}
$comment = {
}
---------------------------------------------------------------------
*/
//char             sz_watch[]; /* 監視項目名称 [IN] */
//char             *sz_path;   /* DBテーブルファイルパス名[IN] */
//char             *shmadr[];  /* DBテーブルアドレス[IN] */
//INT32bit         l_n;        /* ステータテス文字列のテーブル個数 */
//INT32bit         *l_grmd;    /* ステータスフラグ[OUT] */
//INT32bit         *l_offset;  /* 監視データのオフセット位置[OUT]*/
//char             *sz_sts_tbl[]; /* ステータス文字列テーブル[OUT]
//								sz_sts_tbl[0] = otherの文字列アドレス（無い場合はNULL)
//								sz_sts_tbl[1] = ステータス文字列
//								sz_sts_tbl[2] = ステータス文字列
//								.
//								.
//								.
//								sz_sts_tbl[n] = NULL 最後 */
int CEngValCnvDll::ud_sts_flg(char sz_watch[], char *sz_path, char *shmadr, long l_n, int *l_grmd, long *l_offset, char *sz_sts_tbl[])
{
	struct xobssts_tbl_t *obssts_adr;/* ステータス情報テーブル */
	struct xobsname_t *obs_adr; /* 監視項目定義情報 */
	struct xext_tbl_t *ext_adr; /* 各抽出定義情報 */
	char             *shmadrs;
	char             *m_adr;
	char *mon_adr;
	long i;
	long j;


	/*----- 共有メモリのアドレスＧＥＴ -----*/

	m_adr = NULL;
	if (sz_path == NULL) {
		shmadrs = shmadr;
	}
	else {
		return -1;
	}

	if (ud_obsname_adr(sz_watch, shmadrs, &obs_adr) != 0) {
		return -1;
	}

	mon_adr = (char *)obs_adr;
	*l_offset = (long)(mon_adr - shmadrs);

	while (1) {
		ext_adr = (struct xext_tbl_t *)mon_adr;

		if ((sz_sts_tbl != (char **)NULL) && (ds_htonl(ext_adr->l_id) == OBSSTS_ID)) {
			*l_grmd = 1;
			if (m_adr != NULL) {
				free(m_adr);
			}
			if (l_n < 1) {
				return 0;
			}
			obssts_adr = (struct xobssts_tbl_t *)mon_adr;
			if (obssts_adr->sz_i_sts[0] == '\0') {
				sz_sts_tbl[0] = NULL;
				i = 0;
			}
			else {
				sz_sts_tbl[0] = obssts_adr->sz_i_sts;
				i = 1;
			}
			for (j = 0; j < obssts_adr->l_sts_cnt; j++, i++) {
				if (i >= l_n) {
					return 0;
				}
				sz_sts_tbl[i] = obssts_adr->chk_t[j].sz_sts;
			}
			if (i >= l_n) {
				return 0;
			}
			sz_sts_tbl[i] = NULL;
			return 0;
		}
		if (ds_htonl(ext_adr->l_id) == TIMECVT_ID) {
			*l_grmd = 2;
			if (m_adr != NULL) {
				free(m_adr);
			}
			return 0;
		}
		if (ds_htonl(ext_adr->l_id) == ENDOFOBS_ID) {
			*l_grmd = 0;
			if (m_adr != NULL) {
				free(m_adr);
			}
			return 0;
		}
		mon_adr += ds_htonl(ext_adr->l_len);
	}
}

/*
---------------------------------------------------------------------
$name = { 監視項目名テーブルアドレス検索 }
$outline = {
指定の監視項目名のテーブルのアドレスを検索する
}
$comment = {
}
---------------------------------------------------------------------
*/
//char             sz_watch[]; /* 監視項目名称 [IN] */
//char             *shmadr[];  /* DBテーブルアドレス
//							 uc_get_shm_adr ()で選られたアドレスの配列[IN] */
//struct obsname_t *obs_adr[]; /* 監視項目定義情報[OUT] */
int CEngValCnvDll::ud_obsname_adr(char sz_watch[], char *shmadr, struct xobsname_t *obs_adr[])
{
	char *mon_adr;

	/*----- 共有メモリのアドレスＧＥＴ -----*/

	// ★多分、メモリDBに直接値を書き込んでいるらしいので、初期化しない。
	//mon_adr = db_data2;
	mon_adr = shmadr;

	mon_adr += (sizeof(long) * 2);

	int a = 0;

	while (1) {
		obs_adr[0] = (struct xobsname_t *)mon_adr;
		long id = ds_htonl(obs_adr[0]->l_id);

		a++;

		if (id == 0) {
			return -1;
		}
		if (id == OBSNAME_ID) {
			if (strcmp(sz_watch, obs_adr[0]->sz_obsname) == 0) {
				return 0;
			}
		}
		mon_adr += ds_htonl(obs_adr[0]->l_len);
	}
}