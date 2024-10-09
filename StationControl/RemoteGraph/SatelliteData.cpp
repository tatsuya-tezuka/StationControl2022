
#include "stdafx.h"
#include "RemoteGraph.h"
#include "SatelliteData.h"
#include "sys/stat.h"


/* ------------------------------------------------------------------------------------ */
/* DBアクセスクラス
/* ------------------------------------------------------------------------------------ */
CDBAccess::CDBAccess()
{
	m_monnamelist.clear();
	m_eqidlist.clear();
	m_dataidlist.clear();
	m_adrtbl.clear();

	map<CString, map<UINT, CString>>::iterator	itr;
	for( itr = m_stslist.begin(); itr != m_stslist.end(); itr++ )
		(*itr).second.clear();
	m_stslist.clear();
}

CDBAccess::~CDBAccess()
{
	m_monnamelist.clear();
	m_eqidlist.clear();
	m_dataidlist.clear();
	m_adrtbl.clear();

	map<CString, map<UINT, CString>>::iterator	itr;
	for( itr = m_stslist.begin(); itr != m_stslist.end(); itr++ )
		(*itr).second.clear();
	m_stslist.clear();
}

/*============================================================================*/
/*! CDBAccess

-# 衛星固有情報DB読込処理

@param	list
@retval

*/
/*============================================================================*/
int CDBAccess::all_eqmon_db_read()
{
	try{
		m_monnamelist.clear();
		m_eqidlist.clear();
		m_dataidlist.clear();
		m_adrtbl.clear();

		for (int i = 0; i < eStation_MAX; i++)
		{
			CString filename = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, i);

			CLogTraceEx::Write(_T(""), _T("CDBAccess"), _T("all_eqmon_db_read"), _T("Share File Parh"), filename, nLogEx::debug);

			HANDLE hFile = CreateFileA(CStringA(filename), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if( hFile == NULL )
				continue;

			DWORD mFileSize = GetFileSize(hFile, NULL) + 100;
			CloseHandle(hFile);

			char	*eqmondb_data = new char[mFileSize];
			if( eqmondb_data == NULL )
				AfxThrowUserException();

			//DB情報格納領域を初期化
			memset(eqmondb_data, 0, mFileSize);

			//DBファイルを読んでくる
			if (db_read(CStringA(filename).GetBuffer(), eqmondb_data) == 0)
				ud_adr_to_montbl(i, eqmondb_data);

			delete[] eqmondb_data;
			eqmondb_data = NULL;
		}
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}

int CDBAccess::db_read(char* db_file_nam, char *cp_loadadr_p)
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
	if (stat(fname, &stbuf) == -1)
	{
		return(500);
	}

	l_datasize = stbuf.st_size;


	if (l_idnum == 0)
	{
		l_alldatasize += l_datasize;
	}
	else
	{
		l_alldatasize += (l_datasize * l_idnum);
	}

	errno_t ans;
	/*cp_loadadr_p = cp_loadadr;*/
	if ((ans = fopen_s(&fp, fname, "rb")) != 0)
	{
		return(500);
	}
	if( fp == NULL )
	{
		return(500);
	}

	if (fread(sz_work, sizeof(char), sizeof(sz_work), fp) == -1)
	{
		/*#!HEAD:があるかどうか読んでみる*/
		return(500);
	}
	if (!strncmp(sz_work, "#!HEAD:", 7))
	{
		/*#!HEAD:を読み飛ばす*/
		if ((ul_top_pos = (unsigned long)strchr(sz_work, '\n')) == NULL)
		{
			return(500);
		}
		else
		{
			ul_top_pos = (ul_top_pos - (unsigned long)&sz_work[0]) + 1;
		}
	}
	else
	{
		/*ファイルの先頭から全内容を読み込む*/
		ul_top_pos = 0; /*ファイルの先頭からメモリにロードする内容の先頭までのバイト数*/
	}
	if (fseek(fp, ul_top_pos, SEEK_SET) == -1)
	{
		return(500);
	}
	for (;;)
	{
		i_readbyte = (int)fread(cp_loadadr_p, sizeof(char), (l_datasize - ul_top_pos), fp);
		if (i_readbyte <= 0)
		{
			break;
		}
		if (i_readbyte == (l_datasize - ul_top_pos))
		{
			break;
		}
		l_datasize -= i_readbyte;

		cp_loadadr_p = cp_loadadr_p + i_readbyte;
	}
	cp_loadadr_p = cp_loadadr_p + (l_datasize - ul_top_pos);
	l_alldatasize -= ul_top_pos; /*#!HEAD:の部分のバイト数を補正*/
	fclose(fp); /*ファイルをcloseする*/

	return 0;
}

/*============================================================================*/
/*! CDBAccess

-# メモリ上の衛星固有情報ＤＢから、各衛星名文字列の先頭アドレスを
-# 配列に格納して返す。

@param	list
@retval

*/
/*============================================================================*/
int CDBAccess::ud_adr_to_montbl(UINT station, char* eqmondb_data)
{
//	char   *vec[VECMAX * 100];
//	char	**vec;
	int i_ret;
	int i;

//	if( (vec = new char *[VECMAX * 100]) == NULL )
//		return 0;

	deque<struct obsname_t *>	adr_tbl;
	i_ret = ud_adr_to_adrtbl((char *)(eqmondb_data)+sizeof(long) + sizeof(long), OBSNAME_ID, NULL, NULL, adr_tbl);

	int size = sizeof(struct obsname_t);

	/*
	衛星名取得
	*/
	for (i = 0; i < i_ret; i++)
	{
		CString str = CString(adr_tbl[i]->sz_obsname);
		CString	eq_name = _T(""), data_name = _T("");

		int	f = str.Find('.');
		if( f == -1 )
			continue;

		eq_name = str.Left(f);
		data_name = str.Mid(f);

		UINT	eq_id = 0, data_id = 0;

		if (eq_name.IsEmpty() || data_name.IsEmpty())
			continue;

		// 設備名から識別子を取得
		map<CString, DWORD>::iterator itr_eq = m_eqidlist.find(eq_name);
		if (itr_eq == m_eqidlist.end())
		{
			eq_id = (UINT)m_eqidlist.size();
			m_eqidlist.insert(map<CString, DWORD>::value_type(eq_name, eq_id));
		}
		else
			eq_id = (*itr_eq).second;

		// データ名から識別子を取得
		map<CString, DWORD>::iterator itr_data = m_dataidlist.find(data_name);
		if (itr_data == m_dataidlist.end())
		{
			data_id = (UINT)m_dataidlist.size();
			m_dataidlist.insert(map<CString, DWORD>::value_type(data_name, data_id));
		}
		else
			data_id = (*itr_data).second;

		str = mStationSimpleString[station] + CString(_T(".")) + str;

		m_monnamelist.insert(map<DWORD, CString>::value_type(((station & 0xF) << 28) | ((eq_id & 0xFFF) << 16) | (data_id & 0xFFFF), str));

		m_adrtbl.insert(map<CString, struct obsname_t *>::value_type(str, adr_tbl[i]));
	}

//	delete[] vec;

	adr_tbl.clear();

	return (i_ret);
}

/*============================================================================*/
/*! CDBAccess

@param	list
@retval
*/
/*============================================================================*/
int CDBAccess::ud_adr_to_adrtbl(char* base_p, long l_id, long l_limitid, long l_endoftblid, deque<struct obsname_t *>& adr_tbl)
{
	struct res_t *p;
	int    n = 0;

	for (p = (struct res_t *)base_p; p->l_id != NULL; p = (struct res_t *)((char*)p + p->l_len))
	{
		if (p->l_len == 0)
		{
			break;
		}
		if (p->l_id == l_id)
		{
			adr_tbl.push_back((struct obsname_t *)p);
			n++;
		}
		else if ((p->l_id == l_endoftblid) || (p->l_id == l_limitid))
		{
			break;
		}
	}

	return n;
}

/*============================================================================*/
/*! CDBAccess

@param
@retval
*/
/*============================================================================*/
int CDBAccess::create_stslist(vector<stSelectData>& sel_list)
{
	try{
		map<CString, map<UINT, CString>>::iterator	itr;
		for( itr = m_stslist.begin(); itr != m_stslist.end(); itr++ )
			(*itr).second.clear();
		m_stslist.clear();

		for (int i = 0; i < eStation_MAX; i++)
		{
			CString filename = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, i);

			HANDLE hFile = CreateFileA(CStringA(filename), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if( hFile == NULL )
				continue;

			DWORD mFileSize = GetFileSize(hFile, NULL) + 100;
			CloseHandle(hFile);

			char	*eqmondb_data = new char[mFileSize];
			if( eqmondb_data == NULL )
				AfxThrowUserException();

			//DB情報格納領域を初期化
			memset(eqmondb_data, 0, mFileSize);

			//DBファイルを読んでくる
			if (db_read(CStringA(filename).GetBuffer(), eqmondb_data) == 0)
			{
				for( int j = 0; j < sel_list.size(); j++ )
				{
					map<UINT, CString>	sts_list;

					sts_list.clear();

					get_sts_list(sel_list[j].name.Mid(sel_list[j].name.Find('.') + 1), eqmondb_data, sts_list);

					m_stslist.insert(map<CString, map<UINT, CString>>::value_type(sel_list[j].name, sts_list));
				}
			}

			delete[] eqmondb_data;
			eqmondb_data = NULL;
		}
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}

/*============================================================================*/
/*! CDBAccess

@param
@retval
*/
/*============================================================================*/
int CDBAccess::ud_obsname_adr(CString name, char *shmadr, struct obsname_t *obs_adr[])
{
	char *mon_adr;

	/*----- 共有メモリのアドレスＧＥＴ -----*/

	mon_adr = shmadr;
	mon_adr += (sizeof(long) * 2);

	int a = 0;

	while (1) {
		obs_adr[0] = (struct obsname_t *)mon_adr;
		long id = ds_htonl(obs_adr[0]->l_id);

		a++;

		if (id == 0) {
			return -1;
		}
		if (id == OBSNAME_ID) {
			if (strcmp(CStringA(name), obs_adr[0]->sz_obsname) == 0) {
				return 0;
			}
		}
		mon_adr += ds_htonl(obs_adr[0]->l_len);
	}
}

/*============================================================================*/
/*! CDBAccess

@param
@retval
*/
/*============================================================================*/
int CDBAccess::get_sts_list(CString name, char *shmadr, map<UINT, CString>& sts_list)
{
	struct obssts_tbl_t *obssts_adr;/* ステータス情報テーブル */
	struct obsname_t *obs_adr; /* 監視項目定義情報 */
	struct ext_tbl_t *ext_adr; /* 各抽出定義情報 */
	char             *shmadrs;
	char             *m_adr;
	char *mon_adr;
	long i;
	long j;


	/*----- 共有メモリのアドレスＧＥＴ -----*/

	m_adr = NULL;
	shmadrs = shmadr;

	if (ud_obsname_adr(name, shmadrs, &obs_adr) != 0)
		return -1;

	mon_adr = (char *)obs_adr;

	while( 1 )
	{
		ext_adr = (struct ext_tbl_t *)mon_adr;

//		if ((sz_sts_tbl != (char **)NULL) && (ds_htonl(ext_adr->l_id) == OBSSTS_ID)) {
		if (ds_htonl(ext_adr->l_id) == OBSSTS_ID) {
//			*l_grmd = 1;
			if (m_adr != NULL) {
				free(m_adr);
			}
//			if (l_n < 1) {
//				return 0;
//			}
			obssts_adr = (struct obssts_tbl_t *)mon_adr;
			if (obssts_adr->sz_i_sts[0] == '\0') {
				sts_list.insert(map<UINT, CString>::value_type((UINT)-1, _T("")));
//				sz_sts_tbl[0] = NULL;
				i = 0;
			}
			else {
				sts_list.insert(map<UINT, CString>::value_type((UINT)-1, CString(obssts_adr->sz_i_sts)));
//				sz_sts_tbl[0] = obssts_adr->sz_i_sts;
				i = 1;
			}
			for (j = 0; j < obssts_adr->l_sts_cnt; j++, i++) {
//				if (i >= l_n) {
//					return 0;
//				}
				sts_list.insert(map<UINT, CString>::value_type((UINT)obssts_adr->chk_t[j].l_val, CString(obssts_adr->chk_t[j].sz_sts)));
//				sz_sts_tbl[i] = obssts_adr->chk_t[j].sz_sts;
			}
//			if (i >= l_n) {
//				return 0;
//			}
//			sz_sts_tbl[i] = NULL;
			return 0;
		}
		if (ds_htonl(ext_adr->l_id) == TIMECVT_ID) {
//			*l_grmd = 2;
			if (m_adr != NULL) {
				free(m_adr);
			}
			return 0;
		}
		if (ds_htonl(ext_adr->l_id) == ENDOFOBS_ID) {
//			*l_grmd = 0;
			if (m_adr != NULL) {
				free(m_adr);
			}
			return 0;
		}
		mon_adr += ds_htonl(ext_adr->l_len);
	}

	return 0;
}

/* ------------------------------------------------------------------------------------ */
/* CSatelliteDataクラス
/* ------------------------------------------------------------------------------------ */
CSatelliteData::CSatelliteData()
{
	m_pDataThread = NULL;
	m_bThreadLoop = true;
	for (int i = 0; i < sizeof(m_angle) / sizeof(m_angle[0]); ++i) {
		m_angle[i] = rand() * 360 / RAND_MAX;
	}
	m_fLogData = 0.0f;
}


CSatelliteData::~CSatelliteData()
{
	if (m_pDataThread != NULL){
		WaitForSingleObject(m_pDataThread->m_hThread, 1000);
		m_pDataThread = NULL;
	}
}


UINT CollectionDataThread(LPVOID pParam)
{
	CSatelliteData	*pData = (CSatelliteData *)pParam;
	stMMPair mmPair;
	float val;

	while (pData->GetThreadLoop()){

		CGraphMutex::Lock(eRealTimeData);
		for (int i = 0; i < CSatelliteData::mMaxDataTrace; i++){
			pData->GetDummyData(i, val);
			deque<stMMPair>& item = pData->GetData(i);
			int size = (int)item.size();
			if ((double)size >= CSatelliteData::mMaxDataCount){
				item.pop_front();
			}
			mmPair.max = val;
			mmPair.min = val;
			item.push_back(mmPair);
		}
		CGraphMutex::Unlock(eRealTimeData);

		Sleep(1000);
	}

	return 0;
}

void CSatelliteData::CollectionStart()
{
	ClearData();
	SetThreadLoop(true);
	m_pDataThread = AfxBeginThread(CollectionDataThread, this);
}

void CSatelliteData::CollectionStop()
{
	SetThreadLoop(false);
	WaitForSingleObject(m_pDataThread->m_hThread, 1000);
	m_pDataThread = NULL;
}

void CSatelliteData::OpenFile(CString strFileName)
{
	stMMPair mmPair;
	float val;
	ClearData();
	CGraphMutex::Lock(eRealTimeData);
	for (int i = 0; i < CSatelliteData::mMaxDataTrace; i++){
		for (int pos = 0; pos < CSatelliteData::mMaxDataCount; pos++){
			GetDummyData(i, val);
			deque<stMMPair>& item = GetData(i);
			mmPair.max = val;
			mmPair.min = val;
			item.push_back(mmPair);
		}
	}
	CGraphMutex::Unlock(eRealTimeData);
}

void CSatelliteData::GetDummyData(int pos, float& data)
{
	for (int i = 0; i < sizeof(m_angle) / sizeof(m_angle[0]); ++i) {
		m_angle[i] = (m_angle[i] + 2 * (i + 1)) % 360;
	}
	int r = rand() % 997;
	data = (float)((sin(m_angle[pos] * 3.1415926538 / 180.0) + 1) / 2 * (100 - 0) + 0);
	if (pos == 3){
		data = m_fLogData;
		if (m_fLogData == 0.0f)
			m_fLogData = 10.0f;
		else
			m_fLogData = 0.0f;
	}
}
