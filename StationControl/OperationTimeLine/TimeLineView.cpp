// TimeLineView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "OperationTimeLineDoc.h"
#include "TimeLineView.h"
#include "SelectEventMode.h"
#include "SelectForecast.h"
#include "UpdateOpEnd.h"
#include "EventAdd.h"
#include "EventUpdate.h"
#include "EventDel.h"
#include "OptionAnt.h"
#include "SelectMasterSlave.h"

#include "ChildFrm.h"
#include "MainFrm.h"
#include "..\Utility\SshShell.h"

#include "sys/stat.h"

namespace pass{
	/* ------------------------------------------------------------------------------------ */
	/* DBアクセスクラス
	/* ------------------------------------------------------------------------------------ */
	CDBAccess::CDBAccess()
	{
		m_eqsatdb_data = NULL;
		m_eqctldb_data = NULL;
		m_rdychktimelist.clear();
	}

	CDBAccess::~CDBAccess()
	{
		if (m_eqsatdb_data != NULL)
		{
			delete m_eqsatdb_data;
		}

		if (m_eqctldb_data != NULL)
		{
			delete m_eqctldb_data;
		}

		if (m_eqmondb_data != NULL)
		{
			delete m_eqmondb_data;
		}

		//	deque<char*>::iterator itr;
		//	for (itr = mObList.begin(); itr != mObList.end(); itr++){
		//		delete (*itr);
		//	}
		//	mObList.clear();
	}

	/*============================================================================*/
	/*! CDBAccess

	-# マクロ定義のパラメータ情報の先頭アドレスから、展開する設備制御項目名を得る。

	@param	base_p;      input:検索を開始するメモリの先頭アドレス。
	（個々の定義情報の先頭アドレス）
	@param	tbl[];       output:制御名を格納するテーブルの先頭アドレス
	@param	vec[];       output:制御情報先頭アドレスを格納するテーブルの先頭アドレス
	@param	i_argc;      input: 呼び出し側で用意したtbl[]の個数(=vec[]の個数)

	@retval				-1:検索IDが発見できない。
	*/
	/*============================================================================*/
	int CDBAccess::ud_macstsadr_to_exectbl(char *base_p, char *tbl[], char *vec[], int i_argc)
	{
		char   *vec_tmp[VECMAX];
		int    i_ret;
		int    i;

		/*
		パラメータテーブル検索
		*/
		i_ret = ud_adr_to_resvec(base_p, EXEC_ID, BREAK_ID, ENDOFMAC_ID, vec_tmp, VECMAX);
		if (i_ret > i_argc){
			i_ret = i_argc;
		}

		/*
		パラメータ名称取得
		*/
		if (tbl != (char **)NULL)
		{
			for (i = 0; i < i_ret; i++)
			{
				tbl[i] = ((struct exec_t *)vec_tmp[i])->sz_ctl;
			}
		}

		/*
		パラメータ情報テーブルアドレスの格納
		*/
		if (vec != (char **)NULL){
			for (i = 0; i < i_ret; i++){
				vec[i] = vec_tmp[i];
			}
		}

		return (i_ret);
	}


	/*============================================================================*/
	/*! CDBAccess

	-# マクロ定義のパラメータ文字列から、パラメータ情報の先頭アドレスを検索
	-# し、展開する設備制御項目名を得る。


	@param	base_p;		input:検索を開始するメモリの先頭アドレス。
	（個々の定義情報の先頭アドレス）
	@param	psz_param;	input: パラメータ名
	@param	tbl[];		output:実行制御名を格納するテーブルの先頭アドレス
	@param	vec[];		output:実行制御情報先頭アドレスを格納するテーブルの先頭アドレス
	@param	i_argc;input: 呼び出し側で用意したtbl[]の個数(=vec[]の個数)

	@retval				-1:検索IDが発見できない。
	*/
	/*============================================================================*/
	int CDBAccess::ud_macsts_to_exectbl(char *base_p, char *psz_param, char *tbl[], char *vec[], int i_argc)
	{
		char   *vec_tmp[VECMAX];
		int    i_ret;
		int    i;

		/*
		マクロ名検索
		*/
		i_ret = ud_adr_to_resvec(base_p, CASE_ID, ENDOFMAC_ID, ENDOFMAC_ID, vec_tmp, VECMAX);
		if (i_ret == ERR_RETURN)
		{
			return (ERR_RETURN);
		}

		for (i = 0; i < i_ret; i++)
		{
			if (strcmp(((struct case_t *)vec_tmp[i])->sz_param, psz_param) == 0)
			{
				break;
			}
		}
		if (i >= i_ret)
		{
			return (ERR_RETURN);
		}

		/*
		パラメータテーブル検索
		*/
		i_ret = ud_macstsadr_to_exectbl(vec_tmp[i], tbl, vec, i_argc);

		return (i_ret);
	}

	/*============================================================================*/
	/*! CDBAccess

	-# メモリ上の衛星固有情報から、マクロ定義のパラメータのステータス文字列
	-# を検索し、ステータス文字列の先頭アドレス、及びパラメータ情報の先頭アド
	-# レスを配列に格納して返す。

	@param	base_p;		input:検索を開始するメモリの先頭アドレス。
	個々の定義情報の先頭アドレス）
	@param	tbl[];      output:パラメータ名を格納するテーブルの先頭アドレス
	@param	vec[];		output:パラメータ情報先頭アドレスを格納するテーブルの先頭アドレス
	@param	i_argc;		input: 呼び出し側で用意したtbl[]の個数(=vec[]の個数)

	@retval
	*/
	/*============================================================================*/
	int CDBAccess::ud_macadr_to_ststbl(char *base_p, char *tbl[], char *vec[], int i_argc)
	{
		char   *vec_tmp[VECMAX];
		int    i_ret;
		int    i;


		/*
		パラメータテーブル検索
		*/
		i_ret = ud_adr_to_resvec(base_p, CASE_ID, ENDOFMAC_ID, ENDOFMACDEF_ID, vec_tmp, VECMAX);
		if (i_ret > i_argc){
			i_ret = i_argc;
		}

		/*
		パラメータ名称取得
		*/
		if (tbl != (char **)NULL)
		{
			for (i = 0; i < i_ret; i++)
			{
				tbl[i] = ((struct case_t *)vec_tmp[i])->sz_param;
			}
		}

		/*
		パラメータ情報テーブルアドレスの格納
		*/
		if (vec != (char **)NULL)
		{
			for (i = 0; i < i_ret; i++)
			{
				vec[i] = vec_tmp[i];
			}
		}

		return (i_ret);
	}

	/*============================================================================*/
	/*! CDBAccess

	-# マクロ名称からパラメータのステータス文字列を検索し、ステータス文字
	-# 列の先頭アドレス、及びパラメータ情報の先頭アドレスを配列に格納して返す。
	-# １衛星の衛星固有情報を全て検索するので、低速である。
	-# （高速検索を行う場合は、ud_satname_to_mactbl()及びud_macadr_to_ststbl()
	-# を使用する事。）

	@param	char *base_p;	input:検索を開始するメモリの先頭アドレス。
	（個々の定義情報の先頭アドレス）
	@param	psz_macname;	input: マクロ名
	@param	tbl[];			output:ステータス名を格納するテーブルの先頭アドレス
	@param	vec[];			output:ステータス情報先頭アドレスを格納するテーブルの先頭アドレス
	@param	i_argc;			input: 呼び出し側で用意したtbl[]の個数(=vec[]の個数)

	@retval		-1:検索マクロ名が発見できない。
	*/
	/*============================================================================*/
	int CDBAccess::ud_macname_to_ststbl(char *base_p, char *psz_macname, char *tbl[], char *vec[], int i_argc)
	{
		char   *vec_tmp[VECMAX];
		//	struct mac_t  *pst_mac;
		int    i_ret;
		int    i;

		/*
		マクロ名検索
		*/
		i_ret = ud_adr_to_resvec(base_p, MAC_ID, ENDOFMACDEF_ID, ENDOFMACDEF_ID, vec_tmp, VECMAX);
		if (i_ret == ERR_RETURN)
		{
			return (ERR_RETURN);
		}

		for (i = 0; i < i_ret; i++)
		{
			if (strcmp(((struct mac_t *)vec_tmp[i])->sz_macname, psz_macname) == 0)
			{
				break;
			}
		}
		if (i >= i_ret){
			return (ERR_RETURN);
		}

		/*
		パラメータテーブル検索
		*/
		i_ret = ud_macadr_to_ststbl(vec_tmp[i], tbl, vec, i_argc);

		return (i_ret);
	}

	/*============================================================================*/
	/*! CDBAccess

	-# 個々の設備制御情報、衛星定義情報、または、設備情報定義の先頭アドレスから、
	-# データＩＤ値 の一致するデータ構造の先頭アドレスを求める。
	-# （設備制御情報，衛星固有情報，等と同等のデータ構造を持つテーブルの検索に
	-# 使える汎用のルーチン）
	-# ud_adr_to_res()との違いは、探しているデータＩＤに一致するテーブルが複数
	-# 存在している場合は、これらのテーブルの先頭アドレスを配列に格納 すること
	-# にある。

	@param	list
	@retval
	*/
	/*============================================================================*/
	int CDBAccess::ud_adr_to_resvec(char* base_p, long l_id, long l_limitid, long l_endoftblid, char* vec[], int i_argc)
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
				vec[n] = (char *)p;
				n++;
				if (n == i_argc)
				{
					break;
				}
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

	-# メモリ上の衛星固有情報ＤＢから、各衛星名文字列の先頭アドレスを
	-# 配列に格納して返す。

	@param	list
	@retval

	*/
	/*============================================================================*/
	int CDBAccess::ud_adr_to_sattbl(vector<CString>& list)
	{
		char   *vec[VECMAX];
		int i_ret;
		int i;

		i_ret = ud_adr_to_resvec((char *)(m_eqsatdb_data)+sizeof(long) + sizeof(long), SATINF_ID, NULL, NULL, vec, VECMAX);

		/*
		衛星名取得
		*/
		for (i = 0; i < i_ret; i++)
		{
			CString str = CString(((struct satinf_t *)vec[i])->sz_satname);
			list.push_back(str);
			m_satadrlist[str] = *(__int64*)&vec[i];
		}
		return (i_ret);
	}

	/*============================================================================*/
	/*! CDBAccess

	-# 衛星固有情報DB読込処理

	@param	list
	@retval

	*/
	/*============================================================================*/
	int CDBAccess::eqsat_db_read()
	{
		CString filename = theApp.GetShareFilePath(eFileType_DB_Eqsat_File, theApp.GetSelectStation());

		try{
			HANDLE hFile = CreateFileA(CStringA(filename), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			DWORD mFileSize = GetFileSize(hFile, NULL) + 100;

			CloseHandle(hFile);
			if (m_eqsatdb_data != NULL)
			{
				delete m_eqsatdb_data;
			}
			m_eqsatdb_data = new char[mFileSize];
			//DB情報格納領域を初期化
			memset(m_eqsatdb_data, 0, mFileSize);

			//DBファイルを読んでくる
			if (db_read(CStringA(filename).GetBuffer(), m_eqsatdb_data) != 0)
			{
				return -1;
			}

			m_satnamelist.clear();
			m_satadrlist.clear();
			ud_adr_to_sattbl(m_satnamelist);
		}
		catch (...)
		{
			return -1;
		}

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
	int CDBAccess::ud_adr_to_montbl(vector<CString>& list)
	{
		char   *vec[VECMAX * 100];
		int i_ret;
		int i;

		i_ret = ud_adr_to_resvec((char *)(m_eqmondb_data)+sizeof(long) + sizeof(long), OBSNAME_ID, NULL, NULL, vec, VECMAX * 100);

		/*
		衛星名取得
		*/
		for (i = 0; i < i_ret; i++)
		{
			CString str = CString(((struct CEngValCnv::obsname_t *)vec[i])->sz_obsname);
			list.push_back(str);
		}
		return (i_ret);
	}

	/*============================================================================*/
	/*! CDBAccess

	-# 衛星固有情報DB読込処理

	@param	list
	@retval

	*/
	/*============================================================================*/
	int CDBAccess::eqmon_db_read()
	{
		CString filename = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, theApp.GetSelectStation());

		try{
			HANDLE hFile = CreateFileA(CStringA(filename), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			DWORD mFileSize = GetFileSize(hFile, NULL) + 100;

			CloseHandle(hFile);
			if (m_eqmondb_data != NULL)
			{
				delete m_eqmondb_data;
			}
			m_eqmondb_data = new char[mFileSize];
			//DB情報格納領域を初期化
			memset(m_eqmondb_data, 0, mFileSize);

			//DBファイルを読んでくる
			if (db_read(CStringA(filename).GetBuffer(), m_eqmondb_data) != 0)
			{
				return -1;
			}

			m_monnamelist.clear();
			ud_adr_to_montbl(m_monnamelist);

			delete m_eqmondb_data;	// 取り合えず監視名リストが欲しいだけなので削除。（2017/04/14、石原）
			m_eqmondb_data = NULL;
		}
		catch (...)
		{
			return -1;
		}

		return 0;
	}

	/*============================================================================*/
	/*! CDBAccess

	-# メモリ上の設備制御DBから、各衛星名文字列の先頭アドレスを
	-# 配列に格納して返す。

	@param	list
	@retval

	*/
	/*============================================================================*/
	int CDBAccess::ud_adr_to_ctltbl(vector<CString>& list)
	{
		char   *vec[VECMAX * 10];
		int i_ret;
		int i;

		i_ret = ud_adr_to_resvec((char *)(m_eqctldb_data)+sizeof(long) + sizeof(long), CTLNAME_ID, NULL, NULL, vec, VECMAX * 10);

		/*
		衛星名取得
		*/
		for (i = 0; i < i_ret; i++)
		{
			CString str = CString(((struct ctlname_t *)vec[i])->sz_ctlname);
			list.push_back(str);
			m_ctladrlist[str] = *(__int64*)&vec[i];
		}

		return (i_ret);
	}

	/*============================================================================*/
	/*! CDBAccess

	-# 設備制御DB読込処理

	@param	list
	@retval

	*/
	/*============================================================================*/
	int CDBAccess::eqctl_db_read()
	{
		CString filename = theApp.GetShareFilePath(eFileType_DB_Eqctl_File, theApp.GetSelectStation());

		try{
			HANDLE hFile = CreateFileA(CStringA(filename), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			DWORD mFileSize = GetFileSize(hFile, NULL) + 100;

			CloseHandle(hFile);
			if (m_eqctldb_data != NULL)
			{
				delete m_eqctldb_data;
			}
			m_eqctldb_data = new char[mFileSize];
			//DB情報格納領域を初期化
			memset(m_eqctldb_data, 0, mFileSize);

			//DBファイルを読んでくる
			if (db_read(CStringA(filename).GetBuffer(), m_eqctldb_data) != 0)
			{
				return -1;
			}

			m_ctlnamelist.clear();
			m_ctladrlist.clear();
			ud_adr_to_ctltbl(m_ctlnamelist);
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
		FILE *fp = NULL;
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
		if ((ans = fopen_s(&fp, fname, "rb")) != 0 || fp == NULL)
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

	-# レディネスチェック時間ファイル読み込み

	@param
	@retval

	*/
	/*============================================================================*/
	int CDBAccess::rdychk_read()
	{
		FILE *fp;
		TCHAR sz_buff[64];
		TCHAR sz_ele[2][32];

		try{
			CString filename = theApp.GetShareFilePath(eFileType_Rdychk_File, theApp.GetSelectStation());
			CString strEle1, strEle2;
			int	val = 0;

			if ((_wfopen_s(&fp, filename, _T("r")) != 0) || (fp == NULL))
			{
				return -1;
			}

			char sz_line[512];
			while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp) != NULL)
			{
				wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
				swscanf_s(sz_buff, _T("%s%s;"),
					sz_ele[0], static_cast<unsigned>(_countof(sz_ele[0])),
					sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])));

				strEle1 = CString(sz_ele[0]);
				strEle2 = CString(sz_ele[1]);
				int idx = strEle2.Find(_T(";"));
				if (idx != -1)
				{
					strEle2 = strEle2.Mid(0, idx);
				}

				val = _wtoi(strEle2);

				m_rdychktimelist[strEle1] = val;
			}
			fclose(fp);
		}
		catch (...)
		{
			return -1;
		}

		return 0;
	}

	/*============================================================================*/
	/*! CRdnsChk64

	-# レディネスチェック定義ファイルより該当の時間を取得する

	@param	str：ターゲット
	@retval

	*/
	/*============================================================================*/
	int CDBAccess::GetRdyChkTimeFile(CString str)
	{
		int val = 0;

		if (m_rdychktimelist.find(str) != m_rdychktimelist.end())
		{
			val = m_rdychktimelist[str];
		}

		return val;
	}
}; // namespace pass

CIniFileMap::CIniFileMap(CString name)
{
	mFileName = _T("");
	m_hFile = INVALID_HANDLE_VALUE;
	Load(CStringA(name));
	UpdateMap();
}

CIniFileMap::~CIniFileMap()
{
	Unload();
}

// CIniFileMap メンバ関数

//============================================================================
/// @brief	アプリケーションＩＮＩファイルの読込
/// @param	FileName	ファイル名
/// @return
//============================================================================
bool CIniFileMap::Load(CStringA FileName)
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	//CLogTraceEx::Write(_T(""), _T("CIniFileMap"), _T("Load"), _T("Start"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	CStringA s;					// １行データ
	CStringA CurrentSection;	// セクション

	if (FileName.IsEmpty() == true)
		return false;

	// 既にロード済
	if (mFileName == FileName)
		return true;

	TRY
	{
		if (mFileName.IsEmpty() == false){
			Unload();
		}

		m_hFile = INVALID_HANDLE_VALUE;
		m_hMMF = NULL;
		m_pData = NULL;

		mFileName = FileName;
		m_bDirty = false;

		DWORD dwHigh, dwLow;

		DWORD dwTotal, dwError;
		m_hFile = ::CreateFileA(mFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (m_hFile != INVALID_HANDLE_VALUE){
			dwLow = GetFileSize(m_hFile, &dwHigh);
			dwTotal = 0;
			if (dwLow == -1 && (dwError = GetLastError()) != NO_ERROR){
				//=====================================================//
				//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
				CLogTraceEx::Write(_T(""), _T("CIniFileMap"), _T("Load"), _T("File Size Error"), _T(""), nLogEx::error);
				//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
				//=====================================================//
				AfxThrowUserException();
			}
			m_dwSize = dwLow;
		}
		else{
			// 新規作成
			m_hFile = ::CreateFileA(mFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (m_hFile == INVALID_HANDLE_VALUE){
				//=====================================================//
				//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
				CLogTraceEx::Write(_T(""), _T("CIniFileMap"), _T("Load"), _T("Create File Error"), _T(""), nLogEx::error);
				//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
				//=====================================================//
				AfxThrowUserException();
			}
			m_dwSize = 0;
		}

		// ここまでくればINIファイルとして扱える
		dwLow = m_dwSize + _FlushIncrement;
		m_hMMF = ::CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, dwLow, NULL);
		if (NULL == m_hMMF){
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CLogTraceEx::Write(_T(""), _T("CIniFileMap"), _T("Load"), _T("Create FileMap Error"), _T(""), nLogEx::error);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
			AfxThrowUserException();
		}

		// 呼び出し側プロセスのアドレス空間に、ファイルのビューをマップします
		m_pData = ::MapViewOfFile(m_hMMF, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, dwLow);
		if (m_pData == NULL){
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CLogTraceEx::Write(_T(""), _T("CIniFileMap"), _T("Load"), _T("MapViewOfFile Failure"), _T(""), nLogEx::error);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
			AfxThrowUserException();
		}
		m_dwUseSize = m_dwSize;

		LPSTR pHead = (LPSTR)m_pData;
		memset(&pHead[m_dwSize], 0, _FlushIncrement);

		CutZeroTail();

		m_pTrueData = m_pData;
	} CATCH_ALL(e){
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CIniFileMap"), _T("Load"), _T("Exception"), _T(""), nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		if (m_pData){
			::UnmapViewOfFile(m_pData);
			m_pData = NULL;
		}
		if (m_hMMF != NULL){
			::CloseHandle(m_hMMF);
			m_hMMF = NULL;
		}
		if (m_hFile != INVALID_HANDLE_VALUE){
			::CloseHandle(m_hFile);
			m_hFile = INVALID_HANDLE_VALUE;
		}
		m_dwSize = 0;
		//mFileName.Empty();
		return false;
	}
	END_CATCH_ALL


		return true;
}
//============================================================================
/// @brief	アプリケーションＩＮＩファイルのの書き込み
/// @param
/// @return
//============================================================================
bool CIniFileMap::Unload()
{
	DWORD dwFileSize;
	dwFileSize = m_dwUseSize;

	if (mFileName.IsEmpty() == true)
		return true;

	TRY
	{
		if (m_bDirty == false){
			// 更新フラグはOFF
			if (m_pData){
				::UnmapViewOfFile(m_pData);
				m_pData = NULL;
			}
			if (m_hMMF != NULL){
				::CloseHandle(m_hMMF);
				m_hMMF = NULL;
			}
			LONG highDist = 0;
			::SetFilePointer(m_hFile, dwFileSize, &highDist, FILE_BEGIN);
			::SetEndOfFile(m_hFile);
			if (m_hFile != INVALID_HANDLE_VALUE){
				::CloseHandle(m_hFile);
				m_hFile = INVALID_HANDLE_VALUE;
			}
			mFileName.Empty();

			m_dwSize = 0;

			m_mapSection.RemoveAll();
			return true;
		}

		// ファイルの更新を行うされているので更新する
		ASSERT(m_pData);
		if (!::FlushViewOfFile(m_pData, m_dwUseSize)){
			::MessageBox(NULL, _T("FlushViewMMF"), _T(""), MB_OK);
			::UnmapViewOfFile(m_pData);
			::CloseHandle(m_hMMF);
			LONG highDist = 0;
			::SetFilePointer(m_hFile, dwFileSize, &highDist, FILE_BEGIN);
			::SetEndOfFile(m_hFile);
			::CloseHandle(m_hFile);
			mFileName.Empty();
			m_pData = NULL;
			m_hMMF = NULL;
			m_hFile = INVALID_HANDLE_VALUE;
			m_dwSize = 0;

			m_mapSection.RemoveAll();

			return false;
		}

		::UnmapViewOfFile(m_pData);
		::CloseHandle(m_hMMF);
		LONG highDist = 0;
		::SetFilePointer(m_hFile, dwFileSize, &highDist, FILE_BEGIN);
		::SetEndOfFile(m_hFile);
		::CloseHandle(m_hFile);
		mFileName.Empty();
		m_pData = NULL;
		m_hMMF = NULL;
		m_hFile = INVALID_HANDLE_VALUE;
		m_dwSize = 0;

		m_mapSection.RemoveAll();
	} CATCH_ALL(e){
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CIniFileMap"), _T("Unload"), _T("Exception"), _T(""), nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		if (m_pData){
			::UnmapViewOfFile(m_pData);
			m_pData = NULL;
		}
		if (m_hMMF != NULL){
			::CloseHandle(m_hMMF);
			m_hMMF = NULL;
		}
		if (m_hFile != INVALID_HANDLE_VALUE){
			LONG highDist = 0;
			::SetFilePointer(m_hFile, dwFileSize, &highDist, FILE_BEGIN);
			::SetEndOfFile(m_hFile);
			::CloseHandle(m_hFile);
			m_hFile = INVALID_HANDLE_VALUE;
		}
		m_dwSize = 0;
		mFileName.Empty();
		m_mapSection.RemoveAll();
	}
	END_CATCH_ALL


		return true;
}

//============================================================================
/// @brief	終端NULLの削除
/// @param
/// @return
//============================================================================
void CIniFileMap::CutZeroTail()
{
	if (m_dwUseSize == 0){
		// データなし
		return;
	}

	LPBYTE pByte = (LPBYTE)m_pData;
	pByte += m_dwUseSize - sizeof(char);
	while (*pByte == 0x00 && pByte - (LPBYTE)m_pData > 0){
		pByte--;
		m_dwUseSize--;
	}
	return;
}

//============================================================================
/// @brief	INIファイルデータからセクションマップを作成する
/// @param
/// @return
//============================================================================
void CIniFileMap::UpdateMap()
{
	LPSTR pHead = (LPSTR)m_pTrueData;
	LPSTR p1 = pHead;
	LPSTR p2 = pHead;

	p1 = pHead;
	while (p1 - pHead < (int)(m_dwUseSize / sizeof(char)) - 1){
		while (p1 - pHead < (int)(m_dwUseSize / sizeof(char)) - 1 && *p1 != char('['))
			p1++;
		p2 = p1;
		while (p2 - pHead < (int)(m_dwUseSize / sizeof(char)) - 1 && *p2 != char(']'))
			p2++;
		if (p1 - pHead < (int)(m_dwUseSize / sizeof(char)) && p2 - pHead < (int)(m_dwUseSize / sizeof(char))){
			int lenSection = (int)(p2 - p1 - 1);
			if (lenSection >= 1){
				p1++;
				CStringA strSection;
				memcpy(strSection.GetBuffer(lenSection), p1, lenSection);
				strSection.ReleaseBuffer(lenSection);
				p1--;
				m_mapSection.SetAt(CString(strSection), (LPVOID)p1);
				//TRACE("%s\n", strSection);
			}
		}
		p1 = p2 + 1;
	}

}

void CIniFileMap::GetSectionList(vector<CString>& list)
{
	list.clear();
	POSITION pos = m_mapSection.GetStartPosition();
	while (pos){
		CString strSection;
		LPVOID lpPos;
		m_mapSection.GetNextAssoc(pos, strSection, lpPos);
		list.push_back(strSection);
	}
}


// CTimeLineView

IMPLEMENT_DYNCREATE(CTimeLineView, CFormView)

CTimeLineView::CTimeLineView()
: CFormView(CTimeLineView::IDD)
, m_strCurrentTime(_T(""))
, m_strSpaceName(_T(""))
, m_strPlanID(_T(""))
, m_strForecast(_T(""))
, m_strOpeStatus(_T(""))
, m_strOpeStart(_T(""))
, m_strOpeStop(_T(""))
, m_strOpeStartUntil(_T(""))
, m_strOpeStopUntil(_T(""))
, m_strUplnkStartUntil(_T(""))
, m_strUplnkStopUntil(_T(""))
, m_strRngStartUntil(_T(""))
, m_strRngStopUntil(_T(""))
, m_strCmdStartUntil(_T(""))
, m_strCmdStopUntil(_T(""))
, m_strUplnkRun(_T(""))
, m_strUplnkEvent(_T(""))
, m_nUplnkNo(0)
, m_strUplnkStart(_T(""))
, m_strUplnkStop(_T(""))
, m_strCmdRun(_T(""))
, m_strCmdEvent(_T(""))
, m_nCmdNo(0)
, m_strCmdStart(_T(""))
, m_strCmdStop(_T(""))
, m_strRngRun(_T(""))
, m_strRngEvent(_T(""))
, m_nRngNo(0)
, m_strRngStart(_T(""))
, m_strRngStop(_T(""))
, m_strOptInit(_T(""))
, m_strOptSend(_T(""))
, m_strOptRecv(_T(""))
, m_strOptSendout(_T(""))
, m_strOptLna(_T(""))
, m_strOptAntwait(_T(""))
, m_strOptAnt(_T(""))
, m_strReadAgc(_T(""))
, m_strReadRng(_T(""))
, m_strReadTlm(_T(""))
, m_strReadCmd(_T(""))
, m_strStlmRun(_T(""))
, m_strStlmStart(_T(""))
, m_strStlmStop(_T(""))
, m_strXtlmRun(_T(""))
, m_strXtlmStart(_T(""))
, m_strXtlmStop(_T(""))
, m_strKtlmRun(_T(""))
, m_strKtlmStart(_T(""))
, m_strKtlmStop(_T(""))
, m_strMasterSlave(_T(""))
//, m_nPropEventSunset(0)
//, m_nPropEventSunrise(0)
//, m_nPropMarginUlon(0)
//, m_nPropMarginUloff(0)
//, m_nPropMarginDl(0)
//, m_nPropUlCmd(0)
//, m_nPropUlRng(0)
//, m_strPropEventCalc(_T(""))
//, m_strSameTimeMod(_T(""))
, m_strRngType(_T(""))
, m_bRecLevelSModified(false)
, m_bRecLevelXModified(false)
, m_bSubCarTlmModified(false)
, m_bSubCarCmdModified(false)
, m_bReadCompletePlanFile(false)
, mStartTime(0)
, mEndTime(0)
//, mbEndTimeChged(FALSE)
//, mbPlanIDChged(FALSE)
{
	mSaveCx = mSaveCy = 0;
	m_nUplnkEvent = 0;
	m_nCmdEvent = 0;
	m_nRngEvent = 0;

	mSelectForecastName.Empty();
	mSelectForecast = -1;
	mMasterSlave = 0;
	mTimeRate = 1;
	mGetSharePredFileTime = CTime(0);
	mGetSharePlanFileTime = CTime(0);
	mGetSharePlanedFileTime = CTime(0);
//	mGetPropertyFileTime = CTime(0);

	m_strRecLevelS = _T("");
	m_strRecLevelX = _T("");
	m_bTlm = FALSE;
	m_strTlm = _T("");
	m_bCmd = FALSE;
	m_strCmd = _T("");
	m_bModalRun = false;

	// 衛星DBを読み込む
	mDBAccess.eqsat_db_read();
}

CTimeLineView::~CTimeLineView()
{
}

void CTimeLineView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_LEGEND1, m_btCheck[0]);
	DDX_Control(pDX, IDC_CHECK_LEGEND2, m_btCheck[1]);
	DDX_Control(pDX, IDC_CHECK_LEGEND3, m_btCheck[2]);
	DDX_Control(pDX, IDC_CHECK_LEGEND4, m_btCheck[3]);
	DDX_Control(pDX, IDC_CHECK_LEGEND5, m_btCheck[4]);
	DDX_Control(pDX, IDC_CHECK_LEGEND6, m_btCheck[5]);
	DDX_Control(pDX, IDC_CHECK_LEGEND7, m_btCheck[6]);
	DDX_Control(pDX, IDC_CHECK_LEGEND8, m_btCheck[7]);
	DDX_Control(pDX, IDC_CHECK_LEGEND9, m_btCheck[8]);
	//DDX_Control(pDX, IDC_STATIC_NOTES, m_grpNotes);
	//DDX_Control(pDX, IDC_STATIC_PLANINFO, m_grpOpeInfo);
	//DDX_Control(pDX, IDC_STATIC_UPLNKINFO, m_grpUpLink);
	//DDX_Control(pDX, IDC_STATIC_CMDINFO, m_grpCmd);
	//DDX_Control(pDX, IDC_STATIC_RNGINFO, m_grpRng);
	//DDX_Control(pDX, IDC_STATIC_OPEOPTION, m_grpOption);
	//DDX_Control(pDX, IDC_STATIC_READINESS, m_grpRead);
	//DDX_Control(pDX, IDC_STATIC_STLM, m_grpStlm);
	//DDX_Control(pDX, IDC_STATIC_XTLM, m_grpXtlm);
	//DDX_Control(pDX, IDC_STATIC_KATLM, m_grpKatlm);
	//DDX_Control(pDX, IDC_STATIC_MASTERSLAVE, m_grpMasterSlave);
	//DDX_Control(pDX, IDC_STATIC_PROPERTY, m_grpProperty);
	//DDX_Control(pDX, IDC_STATIC_PROPEVENT, m_grpEvent);
	//DDX_Control(pDX, IDC_STATIC_PROPMARGIN, m_grpMargin);
	//DDX_Control(pDX, IDC_STATIC_PROPUL, m_grpOffset);
	//DDX_Control(pDX, IDC_STATIC_DEFCID, m_grpDefaultCid);
	//DDX_Control(pDX, IDC_STATIC_PROPSPACE, m_grpSpace);
	DDX_Text(pDX, IDC_STATIC_CURRENTUTC, m_strCurrentTime);
	DDX_Text(pDX, IDC_STATIC_SPACEVAL, m_strSpaceName);
	DDX_Text(pDX, IDC_STATIC_PLANIDVAL, m_strPlanID);
	DDX_Text(pDX, IDC_STATIC_FORECASTVAL, m_strForecast);
	DDX_Text(pDX, IDC_STATIC_OPESTATUSVAL, m_strOpeStatus);
	DDX_Text(pDX, IDC_STATIC_OPESTARTVAL, m_strOpeStart);
	DDX_Text(pDX, IDC_STATIC_OPESSTOPVAL, m_strOpeStop);
	DDX_Text(pDX, IDC_STATIC_OPESTARTUNTILVAL, m_strOpeStartUntil);
	DDX_Text(pDX, IDC_STATIC_OPESTOPUNTILVAL, m_strOpeStopUntil);
	DDX_Text(pDX, IDC_STATIC_UPLNKSTARTUNTILVAL, m_strUplnkStartUntil);
	DDX_Text(pDX, IDC_STATIC_UPLNKSTOPUNTILVAL, m_strUplnkStopUntil);
	DDX_Text(pDX, IDC_STATIC_RNGSTARTUNTILVAL, m_strRngStartUntil);
	DDX_Text(pDX, IDC_STATIC_RNGSTOPUNTILVAL, m_strRngStopUntil);
	DDX_Text(pDX, IDC_STATIC_CMDSTARTUNTILVAL, m_strCmdStartUntil);
	DDX_Text(pDX, IDC_STATIC_CMDSTOPUNTILVAL, m_strCmdStopUntil);
	DDX_Text(pDX, IDC_STATIC_UPLNKRUNVAL, m_strUplnkRun);
	DDX_Text(pDX, IDC_STATIC_UPLNKEVENTVAL, m_strUplnkEvent);
//	DDX_CBIndex(pDX, IDC_COMBO_UPLNKNO, m_nUplnkNo);
	DDX_Control(pDX, IDC_COMBO_UPLNKNO, m_cbUplnkNo);
	DDX_Text(pDX, IDC_STATIC_UPLNKSTARTVAL, m_strUplnkStart);
	DDX_Text(pDX, IDC_STATIC_UPLNKSTOPVAL, m_strUplnkStop);
	DDX_Text(pDX, IDC_STATIC_CMDRUNVAL, m_strCmdRun);
	DDX_Text(pDX, IDC_STATIC_CMDEVENTVAL, m_strCmdEvent);
//	DDX_CBIndex(pDX, IDC_COMBO_CMDNO, m_nCmdNo);
	DDX_Control(pDX, IDC_COMBO_CMDNO, m_cbCmdNo);
	DDX_Text(pDX, IDC_STATIC_CMDSTARTVAL, m_strCmdStart);
	DDX_Text(pDX, IDC_STATIC_CMDSTOPVAL, m_strCmdStop);
	DDX_Text(pDX, IDC_STATIC_RNGRUNVAL, m_strRngRun);
	DDX_Text(pDX, IDC_STATIC_RNGEVENTVAL, m_strRngEvent);
//	DDX_CBIndex(pDX, IDC_COMBO_RNGNO, m_nRngNo);
	DDX_Control(pDX, IDC_COMBO_RNGNO, m_cbRngNo);
	DDX_Text(pDX, IDC_STATIC_RNGSTARTVAL, m_strRngStart);
	DDX_Text(pDX, IDC_STATIC_RNGSTOPVAL, m_strRngStop);
	DDX_Text(pDX, IDC_STATIC_OPEOPINITVAL, m_strOptInit);
	DDX_Text(pDX, IDC_STATIC_OPEOPSENDVAL, m_strOptSend);
	DDX_Text(pDX, IDC_STATIC_OPEOPRECVVAL, m_strOptRecv);
	DDX_Text(pDX, IDC_STATIC_OPEOPSENDOUTVAL, m_strOptSendout);
	DDX_Text(pDX, IDC_STATIC_OPEOPLNAVAL, m_strOptLna);
	DDX_Text(pDX, IDC_STATIC_OPEOPANTWAITVAL, m_strOptAntwait);
	DDX_Text(pDX, IDC_STATIC_OPEOPANTVAL, m_strOptAnt);
	DDX_Text(pDX, IDC_STATIC_READAGCVAL, m_strReadAgc);
	DDX_Text(pDX, IDC_STATIC_READRNGVAL, m_strReadRng);
	DDX_Text(pDX, IDC_STATIC_READTLMVAL, m_strReadTlm);
	DDX_Text(pDX, IDC_STATIC_READCMDVAL, m_strReadCmd);
	DDX_Text(pDX, IDC_STATIC_STLMRUNVAL, m_strStlmRun);
	DDX_Text(pDX, IDC_STATIC_STLMSTARTVAL, m_strStlmStart);
	DDX_Text(pDX, IDC_STATIC_STLMSTOPVAL, m_strStlmStop);
	DDX_Text(pDX, IDC_STATIC_XTLMRUNVAL, m_strXtlmRun);
	DDX_Text(pDX, IDC_STATIC_XTLMSTARTVAL, m_strXtlmStart);
	DDX_Text(pDX, IDC_STATIC_XTLMSTOPVAL, m_strXtlmStop);
	DDX_Text(pDX, IDC_STATIC_KATLMRUNVAL, m_strKtlmRun);
	DDX_Text(pDX, IDC_STATIC_KATLMSTARTVAL, m_strKtlmStart);
	DDX_Text(pDX, IDC_STATIC_KATLMSTOPVAL, m_strKtlmStop);
	DDX_Text(pDX, IDC_STATIC_MSVAL, m_strMasterSlave);
//	DDX_Text(pDX, IDC_EDIT_PROPEVENTSUNSET, m_nPropEventSunset);
//	DDV_MinMaxUInt(pDX, m_nPropEventSunset, 0, 90);
//	DDX_Text(pDX, IDC_EDIT_PROPEVENTSUNRISE, m_nPropEventSunrise);
//	DDV_MinMaxUInt(pDX, m_nPropEventSunrise, 0, 90);
//	DDX_Text(pDX, IDC_EDIT_PROPMARGINULON, m_nPropMarginUlon);
//	DDV_MinMaxUInt(pDX, m_nPropMarginUlon, 0, 90);
//	DDX_Text(pDX, IDC_EDIT_PROPMARGINULOFF, m_nPropMarginUloff);
//	DDV_MinMaxUInt(pDX, m_nPropMarginUloff, 0, 90);
//	DDX_Text(pDX, IDC_EDIT_PROPMARGINDL, m_nPropMarginDl);
//	DDV_MinMaxUInt(pDX, m_nPropMarginDl, 0, 90);
//	DDX_Text(pDX, IDC_EDIT_PROPULCMD, m_nPropUlCmd);
//	DDV_MinMaxUInt(pDX, m_nPropUlCmd, 0, 999);
//	DDX_Text(pDX, IDC_EDIT_PROPULRNG, m_nPropUlRng);
//	DDV_MinMaxUInt(pDX, m_nPropUlRng, 0, 999);
//	DDX_Control(pDX, IDC_EDIT_PROPEVENTSUNSET, m_edPropEventSunset);
//	DDX_Control(pDX, IDC_EDIT_PROPEVENTSUNRISE, m_edPropEventSunrise);
//	DDX_Control(pDX, IDC_EDIT_PROPMARGINULON, m_edPropMarginUlon);
//	DDX_Control(pDX, IDC_EDIT_PROPMARGINULOFF, m_edPropMarginUloff);
//	DDX_Control(pDX, IDC_EDIT_PROPMARGINDL, m_edPropMarginDl);
//	DDX_Control(pDX, IDC_EDIT_PROPULCMD, m_edPropUlCmd);
//	DDX_Control(pDX, IDC_EDIT_PROPULRNG, m_edPropUlRng);
	DDX_Control(pDX, IDC_LIST_WAY, m_cbWayList);
//	DDX_Text(pDX, IDC_EDIT_PROPEVENTAUTOVAL, m_strPropEventCalc);
//	DDX_Text(pDX, IDC_EDIT_PROPSPACEVAL, m_strSameTimeMod);
	DDX_Control(pDX, IDC_EDIT_RECLEVELVALS, m_edRecLevelS);	// 入力を小数に制限
	DDX_Control(pDX, IDC_EDIT_RECLEVELVALX, m_edRecLevelX);	// 入力を小数に制限
	//	DDX_Text(pDX, IDC_EDIT_RECLEVELVALS, m_strRecLevel);
	DDX_Check(pDX, IDC_CHECK_TLM, m_bTlm);
//	DDX_Text(pDX, IDC_EDIT_TLM, m_strTlm);
	DDX_Control(pDX, IDC_EDIT_TLM, m_edTlm);	// 入力を小数に制限
	DDX_Check(pDX, IDC_CHECK_CMD, m_bCmd);
//	DDX_Text(pDX, IDC_EDIT_CMD, m_strCmd);
	DDX_Control(pDX, IDC_EDIT_CMD, m_edCmd);	// 入力を小数に制限
	//DDX_Control(pDX, IDC_STATIC_INIT, m_grpInitProp);
	DDX_Text(pDX, IDC_RNG_TYPE, m_strRngType);
	DDX_Control(pDX, IDC_STATIC_UPLNKEVENTVAL, m_UplinkEventStatic);
	DDX_Control(pDX, IDC_STATIC_CMDEVENTVAL, m_CmdEventStatic);
	DDX_Control(pDX, IDC_STATIC_RNGEVENTVAL, m_RngEventStatic);
}

BEGIN_MESSAGE_MAP(CTimeLineView, CFormView)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_FORECAST, &CTimeLineView::OnBnClickedButtonForecast)
	ON_BN_CLICKED(IDC_BUTTON_OPESTOP, &CTimeLineView::OnBnClickedButtonOpestop)
	ON_BN_CLICKED(IDC_BUTTON_UPLNK, &CTimeLineView::OnBnClickedButtonUplnk)
	ON_BN_CLICKED(IDC_BUTTON_UPLNKADD, &CTimeLineView::OnBnClickedButtonUplnkadd)
	ON_BN_CLICKED(IDC_BUTTON_UPLNKUPDATE, &CTimeLineView::OnBnClickedButtonUplnkupdate)
	ON_BN_CLICKED(IDC_BUTTON_UPLNKDEL, &CTimeLineView::OnBnClickedButtonUplnkdel)
	ON_CBN_SELCHANGE(IDC_COMBO_UPLNKNO, &CTimeLineView::OnCbnSelchangeComboUplnkno)
	ON_BN_CLICKED(IDC_BUTTON_CMD, &CTimeLineView::OnBnClickedButtonCmd)
	ON_BN_CLICKED(IDC_BUTTON_CMDADD, &CTimeLineView::OnBnClickedButtonCmdadd)
	ON_BN_CLICKED(IDC_BUTTON_CMDUPDATE, &CTimeLineView::OnBnClickedButtonCmdupdate)
	ON_BN_CLICKED(IDC_BUTTON_CMDDEL, &CTimeLineView::OnBnClickedButtonCmddel)
	ON_CBN_SELCHANGE(IDC_COMBO_CMDNO, &CTimeLineView::OnCbnSelchangeComboCmdno)
	ON_BN_CLICKED(IDC_BUTTON_RNG, &CTimeLineView::OnBnClickedButtonRng)
	ON_BN_CLICKED(IDC_BUTTON_RNGADD, &CTimeLineView::OnBnClickedButtonRngadd)
	ON_BN_CLICKED(IDC_BUTTON_RNGUPDATE, &CTimeLineView::OnBnClickedButtonRngupdate)
	ON_BN_CLICKED(IDC_BUTTON_RNGDEL, &CTimeLineView::OnBnClickedButtonRngdel)
	ON_CBN_SELCHANGE(IDC_COMBO_RNGNO, &CTimeLineView::OnCbnSelchangeComboRngno)
	ON_BN_CLICKED(IDC_BUTTON_ANT, &CTimeLineView::OnBnClickedButtonAnt)
	ON_BN_CLICKED(IDC_BUTTON_MASTERSLAVE, &CTimeLineView::OnBnClickedButtonMasterslave)
	ON_BN_CLICKED(IDC_BUTTON_PROPSET, &CTimeLineView::OnBnClickedButtonPropset)
	ON_BN_CLICKED(IDC_BUTTON_PROPRESET, &CTimeLineView::OnBnClickedButtonPropreset)
	ON_BN_CLICKED(IDC_BUTTON_PROPCID, &CTimeLineView::OnBnClickedButtonPropcid)
	ON_BN_CLICKED(IDC_BUTTON_PROPINIT, &CTimeLineView::OnBnClickedButtonPropinit)
	ON_BN_CLICKED(IDC_CHECK_TLM, &CTimeLineView::OnBnClickedCheckTlm)
	ON_BN_CLICKED(IDC_CHECK_CMD, &CTimeLineView::OnBnClickedCheckCmd)
	ON_EN_CHANGE(IDC_EDIT_RECLEVELVALS, &CTimeLineView::OnEnChangeEditReclevelvals)
	ON_EN_CHANGE(IDC_EDIT_RECLEVELVALX, &CTimeLineView::OnEnChangeEditReclevelvalx)
	ON_EN_CHANGE(IDC_EDIT_TLM, &CTimeLineView::OnEnChangeEditTlm)
	ON_EN_CHANGE(IDC_EDIT_CMD, &CTimeLineView::OnEnChangeEditCmd)
END_MESSAGE_MAP()


// CTimeLineView 診断

#ifdef _DEBUG
void CTimeLineView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTimeLineView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
COperationTimeLineDoc* CTimeLineView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COperationTimeLineDoc)));
	return (COperationTimeLineDoc*)m_pDocument;
}
#endif //_DEBUG

void CTimeLineView::CreateTimelineData()
{
#ifdef _CUT
	SYSTEMTIME sys;
	GetSystemTime(&sys);
	//sys.wMinute = 0;
	sys.wSecond = 0;
	CTime local = CTime(sys);

	// 予報値データの作成
	mGraph.mListPredData.clear();
	stPredListData pdata;
	pdata.strSatellite = _T("HAYABUSAX");	// 衛星名
	pdata.strPassId = _T("161001-0100");	// パスID
	pdata.tAOS = (local + CTimeSpan(0, 0, 0, 0)).GetTime();
	pdata.tLOS = (local + CTimeSpan(0, 1, 0, 0)).GetTime();
	pdata.tMaxEl = 0;
	pdata.strPredPath = _T("161001-0000");	// パスID
	pdata.tOffset = 0;
	mGraph.mListPredData.push_back(pdata);
	pdata.strPassId = _T("161001-0200");	// パスID
	pdata.tAOS = (local + CTimeSpan(0, 2, 0, 0)).GetTime();
	pdata.tLOS = (local + CTimeSpan(0, 4, 0, 0)).GetTime();
	mGraph.mListPredData.push_back(pdata);
	pdata.strPassId = _T("161001-0300");	// パスID
	pdata.tAOS = (local + CTimeSpan(0, 3, 0, 0)).GetTime();
	pdata.tLOS = (local + CTimeSpan(0, 5, 0, 0)).GetTime();
	mGraph.mListPredData.push_back(pdata);
	pdata.strPassId = _T("161001-0400");	// パスID
	pdata.tAOS = (local + CTimeSpan(0, 1, 30, 0)).GetTime();
	pdata.tLOS = (local + CTimeSpan(0, 2, 30, 0)).GetTime();
	mGraph.mListPredData.push_back(pdata);
	pdata.strPassId = _T("161001-0500");	// パスID
	pdata.tAOS = (local + CTimeSpan(0, 3, 30, 0)).GetTime();
	pdata.tLOS = (local + CTimeSpan(0, 5, 30, 0)).GetTime();
	mGraph.mListPredData.push_back(pdata);

	// 運用計画データの作成
	mGraph.mListTimeData.clear();
	COprPlnData tdata;
	stTime time;

	CString str;

	//#マスター計画
	tdata.m_strPredID = _T("161001-0100");
	tdata.m_strPlanID = _T("161001-0101");
	tdata.m_OpetDay.start = local + CTimeSpan(0, 0, 10, 0);
	tdata.m_OpetDay.end = local + CTimeSpan(0, 0, 15, 0);
	str = (tdata.m_OpetDay.start).Format(_T("%Y-%m-%d %H:%M:%S"));
	TRACE("### %s\n", CStringA(str));

	tdata.m_UpLinkChk = true;
	tdata.m_UpLinkCnt = 1;
	time.start = local + CTimeSpan(0, 0, 10, 0);
	time.end = local + CTimeSpan(0, 0, 12, 0);
	tdata.m_ULDay.push_back(time);
	tdata.m_RngChk = true;
	tdata.m_RngCnt = 1;
	time.start = local + CTimeSpan(0, 0, 12, 0);
	time.end = local + CTimeSpan(0, 0, 13, 0);
	tdata.m_RngDayList.push_back(time);
	tdata.m_CmdChk = true;
	tdata.m_CmdCnt = 1;
	time.start = local + CTimeSpan(0, 0, 13, 0);
	time.end = local + CTimeSpan(0, 0, 15, 0);
	tdata.m_CmdDayList.push_back(time);
	tdata.m_TlmSChk = true;
	tdata.m_TlmSDay.start = CTime(0);
	tdata.m_TlmSDay.end = CTime(0);
	tdata.m_TlmXChk = true;
	tdata.m_TlmXDay.start = local + CTimeSpan(0, 0, 10, 0);
	tdata.m_TlmXDay.end = local + CTimeSpan(0, 0, 15, 0);
	tdata.m_TlmKaChk = false;
	tdata.m_TlmKaDay.start = local + CTimeSpan(0, 0, 10, 0);
	tdata.m_TlmKaDay.end = local + CTimeSpan(0, 0, 15, 0);
	tdata.m_AntStandby = 1;
	tdata.m_AntZenith = 1;

	tdata.m_OpInit = 1;
	tdata.m_OpTDP = 0;
	tdata.m_OpRDP = 0;
	tdata.m_OpTOutType = 0;
	tdata.m_OpTOut = 0;

	tdata.m_AgcChkValue = true;
	tdata.m_RndChkValue = true;
	tdata.m_TlmChkValue = true;
	tdata.m_CmdChkValue = true;

	tdata.m_strCID = _T("CID1234567");

	tdata.m_AntFcstValue.start = local + CTimeSpan(0, 0, 0, 0);
	tdata.m_AntFcstValue.end = local + CTimeSpan(0, 1, 0, 0);
	mGraph.mListTimeData.push_back(tdata);

	//#マスター計画
	tdata.m_strPredID = _T("161001-0200");
	tdata.m_strPlanID = _T("161001-0201");
	tdata.m_OpetDay.start = local + CTimeSpan(0, 0, 30, 0);
	tdata.m_OpetDay.end = local + CTimeSpan(0, 2, 30, 0);
	tdata.m_UpLinkChk = false;
	tdata.m_UpLinkCnt = 0;
	tdata.m_ULDay.clear();
	tdata.m_RngChk = false;
	tdata.m_RngCnt = 0;
	tdata.m_RngDayList.clear();
	tdata.m_CmdChk = false;
	tdata.m_CmdCnt = 0;
	tdata.m_CmdDayList.clear();
	tdata.m_TlmSChk = false;
	tdata.m_TlmSDay.start = CTime(0);
	tdata.m_TlmSDay.end = CTime(0);
	tdata.m_TlmXChk = false;
	tdata.m_TlmXDay.start = CTime(0);
	tdata.m_TlmXDay.end = CTime(0);
	tdata.m_TlmKaChk = false;
	tdata.m_TlmKaDay.start = CTime(0);
	tdata.m_TlmKaDay.end = CTime(0);
	tdata.m_AntStandby = 1;
	tdata.m_AntZenith = 1;

	tdata.m_OpInit = 1;
	tdata.m_OpTDP = 0;
	tdata.m_OpRDP = 0;
	tdata.m_OpTOutType = 0;
	tdata.m_OpTOut = 0;

	tdata.m_AgcChkValue = true;
	tdata.m_RndChkValue = true;
	tdata.m_TlmChkValue = true;
	tdata.m_CmdChkValue = true;

	tdata.m_strCID = _T("CID1234567");

	tdata.m_AntFcstValue.start = local + CTimeSpan(0, 1, 0, 0);
	tdata.m_AntFcstValue.end = local + CTimeSpan(0, 3, 0, 0);
	mGraph.mListTimeData.push_back(tdata);

	//#マスター計画+ダウンリンク
	tdata.m_strPredID = _T("161001-0300");
	tdata.m_strPlanID = _T("161001-0301");
	tdata.m_OpetDay.start = local + CTimeSpan(0, 1, 30, 0);
	tdata.m_OpetDay.end = local + CTimeSpan(0, 2, 30, 0);
	tdata.m_UpLinkChk = false;
	tdata.m_UpLinkCnt = 0;
	tdata.m_ULDay.clear();
	tdata.m_RngChk = false;
	tdata.m_RngCnt = 0;
	tdata.m_RngDayList.clear();
	tdata.m_CmdChk = false;
	tdata.m_CmdCnt = 0;
	tdata.m_CmdDayList.clear();
	tdata.m_TlmSChk = true;
	tdata.m_TlmSDay.start = CTime(0);
	tdata.m_TlmSDay.end = CTime(0);
	tdata.m_TlmXChk = true;
	tdata.m_TlmXDay.start = local + CTimeSpan(0, 1, 30, 0);
	tdata.m_TlmXDay.end = local + CTimeSpan(0, 1, 50, 0);
	tdata.m_TlmKaChk = false;
	tdata.m_TlmKaDay.start = local + CTimeSpan(0, 2, 0, 0);
	tdata.m_TlmKaDay.end = local + CTimeSpan(0, 2, 30, 0);
	tdata.m_AntStandby = 1;
	tdata.m_AntZenith = 1;

	tdata.m_OpInit = 1;
	tdata.m_OpTDP = 0;
	tdata.m_OpRDP = 0;
	tdata.m_OpTOutType = 0;
	tdata.m_OpTOut = 0;

	tdata.m_AgcChkValue = true;
	tdata.m_RndChkValue = true;
	tdata.m_TlmChkValue = true;
	tdata.m_CmdChkValue = true;

	tdata.m_strCID = _T("CID1234567");

	tdata.m_AntFcstValue.start = local + CTimeSpan(0, 1, 0, 0);
	tdata.m_AntFcstValue.end = local + CTimeSpan(0, 3, 0, 0);

	mGraph.mListTimeData.push_back(tdata);

	//#マスター計画+ダウンリンク+アップリンク
	tdata.m_strPredID = _T("161001-0400");
	tdata.m_strPlanID = _T("161001-0401");
	tdata.m_OpetDay.start = local + CTimeSpan(0, 1, 30, 0);
	tdata.m_OpetDay.end = local + CTimeSpan(0, 2, 30, 0);
	tdata.m_UpLinkChk = true;
	tdata.m_UpLinkCnt = 1;
	time.start = local + CTimeSpan(0, 1, 30, 0);
	time.end = local + CTimeSpan(0, 1, 45, 0);
	tdata.m_ULDay.push_back(time);
	tdata.m_RngChk = true;
	tdata.m_RngCnt = 1;
	time.start = local + CTimeSpan(0, 1, 45, 0);
	time.end = local + CTimeSpan(0, 2, 0, 0);
	tdata.m_RngDayList.push_back(time);
	tdata.m_CmdChk = true;
	tdata.m_CmdCnt = 1;
	time.start = local + CTimeSpan(0, 1, 30, 0);
	time.end = local + CTimeSpan(0, 2, 15, 0);
	tdata.m_CmdDayList.push_back(time);
	tdata.m_TlmSChk = true;
	tdata.m_TlmSDay.start = CTime(0);
	tdata.m_TlmSDay.end = CTime(0);
	tdata.m_TlmXChk = true;
	tdata.m_TlmXDay.start = local + CTimeSpan(0, 1, 30, 0);
	tdata.m_TlmXDay.end = local + CTimeSpan(0, 1, 45, 0);
	tdata.m_TlmKaChk = false;
	tdata.m_TlmKaDay.start = local + CTimeSpan(0, 2, 0, 0);
	tdata.m_TlmKaDay.end = local + CTimeSpan(0, 2, 30, 0);
	tdata.m_AntStandby = 1;
	tdata.m_AntZenith = 1;

	tdata.m_OpInit = 1;
	tdata.m_OpTDP = 0;
	tdata.m_OpRDP = 0;
	tdata.m_OpTOutType = 0;
	tdata.m_OpTOut = 0;

	tdata.m_AgcChkValue = true;
	tdata.m_RndChkValue = true;
	tdata.m_TlmChkValue = true;
	tdata.m_CmdChkValue = true;

	tdata.m_strCID = _T("CID1234567");

	tdata.m_AntFcstValue.start = local + CTimeSpan(0, 1, 0, 0);
	tdata.m_AntFcstValue.end = local + CTimeSpan(0, 3, 0, 0);

	mGraph.mListTimeData.push_back(tdata);
#else
	// 運用計画データの作成
	SetPredList_Euc();
	SetPlanList_Euc();
	CGraphMutex::Lock(ePassInfo);
	GetPlanDataFromFile();
	CGraphMutex::Unlock(ePassInfo);
#endif
	mTimeRate = GetPrivateProfileInt(SECTION_DEBUG, KEY_TIMERATE, 1, theApp.GetAppInifileName());
}

// CTimeLineView メッセージ ハンドラー
void CTimeLineView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CRect rect;
	GetClientRect(rect);

	GetParentFrame()->RecalcLayout();

	GetDocument()->SetTitleEx(mTimeViewTitle[eTimeViewType_OperatioTimeline]);

	int	nMapMode;
	GetDeviceScrollSizes(nMapMode, mSizeTotal, mSizePage, mSizeLine);
	//SetScaleToFitSize(mSizeTotal);
	ResizeParentToFit(FALSE);

#if 0
	// システムコマンドSC_CLOSEのディセーブル
	CMenu* pSysMenu = GetParent()->GetSystemMenu(FALSE);
	if (pSysMenu)	pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
#endif

	HICON icon = NULL;
	icon = theApp.LoadIcon(IDI_ICON_TIMELINE);
	GetParent()->SetIcon(icon, TRUE);
	GetParent()->SetIcon(icon, FALSE);
	if (icon != NULL)
		DestroyIcon(icon);

	//setControlInfo(IDC_STATIC_PLANINFO, ANCHORE_LEFTTOP);
	//setControlInfo(IDC_CHECK_LEGEND2, ANCHORE_BOTTOM);
	//setControlInfo(IDC_CHECK_LEGEND3, ANCHORE_BOTTOM);
	//setControlInfo(IDC_CHECK_LEGEND4, ANCHORE_BOTTOM);
	//setControlInfo(IDC_CHECK_LEGEND5, ANCHORE_BOTTOM);
	//setControlInfo(IDC_CHECK_LEGEND6, ANCHORE_BOTTOM);
	//setControlInfo(IDC_CHECK_LEGEND7, ANCHORE_BOTTOM);
	//setControlInfo(IDC_CHECK_LEGEND8, ANCHORE_BOTTOM);
	//setControlInfo(IDC_CHECK_LEGEND9, ANCHORE_BOTTOM);
	setControlInfo(IDC_STATIC_CURRENTTIME, ANCHORE_RIGHT);
	setControlInfo(IDC_STATIC_CURRENTUTC, ANCHORE_RIGHT);
	setControlInfo(IDC_STATIC_TIMELINEHEADER, ANCHORE_LEFTTOP | RESIZE_HOR);
	setControlInfo(IDC_STATIC_TIMELINE, ANCHORE_LEFTTOP | RESIZE_HOR);
	//setControlInfo(IDC_STATIC_NOTES, ANCHORE_BOTTOM);

	initializeControl();

	CreateTimelineData();

	GetDlgItem(IDC_EDIT_TLM)->EnableWindow(m_bTlm);
	m_edTlm.SetSel(0, -1);
	m_edTlm.SetFocus();
	GetDlgItem(IDC_EDIT_CMD)->EnableWindow(m_bCmd);
	m_edCmd.SetSel(0, -1);
	m_edCmd.SetFocus();

	GetCurrentUtcTime(mCurrentTime);
	setCurrentUtcTime();
	UpdateTimelineData(mSelectForecast);

	//	UpdateInitializePropertyData();
	//	UpdateOperationProperty();
	UpdateOperationOption();
	UpdateMonitorData();

	// アップリンク、CMD、RNG イベント
	m_UplinkEventStatic.SetGradient(false);
	m_UplinkEventStatic.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_UplinkEventStatic.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	m_UplinkEventStatic.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	m_CmdEventStatic.SetGradient(false);
	m_CmdEventStatic.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_CmdEventStatic.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	m_CmdEventStatic.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	m_RngEventStatic.SetGradient(false);
	m_RngEventStatic.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_RngEventStatic.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	m_RngEventStatic.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	m_edRecLevelS.SetDecimal(6);
	m_edRecLevelS.SetDelta(2);
	m_edRecLevelS.SetUsePlusMinusChar(FALSE, TRUE);

	m_edRecLevelX.SetDecimal(6);
	m_edRecLevelX.SetDelta(2);
	m_edRecLevelX.SetUsePlusMinusChar(FALSE, TRUE);

	m_edTlm.SetDecimal(12);
	m_edTlm.SetDelta(4);
	m_edTlm.SetUsePlusMinusChar(FALSE, FALSE);

	m_edCmd.SetDecimal(12);
	m_edCmd.SetDelta(4);
	m_edCmd.SetUsePlusMinusChar(FALSE, FALSE);

	m_bReadCompletePlanFile = false;

	mStartTime = 0;
	mEndTime = 0;

	SetTimer(1, 1000, NULL);
}

/*============================================================================*/
/*! タイムライン

-# 指定ウィンドウの子ウィンドウのボタンを非活性にする

@brief コントロールの初期化を行う

@param	pMain	親ウィンドウ
@retval なし
*/
/*============================================================================*/
void CTimeLineView::SetMonitorMode(CWnd* pMain)
{
	if (theApp.GetExecuteMode() == eExecuteMode_Control)		//制御モード
	{
		if(theApp.GetLimitedMode() == eLimitedMode_Disable)		//制御モードでかつ、制限つきでない
		{
			return;
		}
	}

	TCHAR name[100];
	CWnd *pwnd = pMain->GetWindow(GW_CHILD);
	while (pwnd != NULL){
		CWnd* p = GetDlgItem(IDC_BUTTON_FORECAST);
		if (GetDlgItem(IDC_BUTTON_FORECAST) == pwnd){
				int i = 0;
		}
		GetClassName(pwnd->m_hWnd, name, 100);
		CString str = name;
		if (str.CompareNoCase(_T("Button")) == 0){
			pwnd->EnableWindow(FALSE);
		}
		pwnd = pwnd->GetWindow(GW_HWNDNEXT);
	}

	GetDlgItem(IDC_EDIT_RECLEVELVALS)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_RECLEVELVALX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TLM)->EnableWindow(FALSE);
	m_edTlm.SetSel(0, -1);
	m_edTlm.SetFocus();
	GetDlgItem(IDC_EDIT_CMD)->EnableWindow(FALSE);
	m_edCmd.SetSel(0, -1);
	m_edCmd.SetFocus();
}

/*============================================================================*/
/*! タイムライン

-# コントロールの初期化

@brief コントロールの初期化を行う

@param
@retval なし
*/
/*============================================================================*/
void CTimeLineView::initializeControl()
{
	CRect rect;
	GetClientRect(rect);

//	// プロパティ変更コントロール
//	m_edPropEventSunset.EnableMask(_T("DD"), _T("__"), _T(' '));
//	m_edPropEventSunset.SetValidChars(NULL);
//	m_edPropEventSunset.SetWindowText(_T("0"));
//	m_edPropEventSunrise.EnableMask(_T("DD"), _T("__"), _T(' '));
//	m_edPropEventSunrise.SetValidChars(NULL);
//	m_edPropEventSunrise.SetWindowText(_T("0"));
//	m_edPropMarginUlon.EnableMask(_T("DD"), _T("__"), _T(' '));
//	m_edPropMarginUlon.SetValidChars(NULL);
//	m_edPropMarginUlon.SetWindowText(_T("0"));
//	m_edPropMarginUloff.EnableMask(_T("DD"), _T("__"), _T(' '));
//	m_edPropMarginUloff.SetValidChars(NULL);
//	m_edPropMarginUloff.SetWindowText(_T("0"));
//	m_edPropMarginDl.EnableMask(_T("DD"), _T("__"), _T(' '));
//	m_edPropMarginDl.SetValidChars(NULL);
//	m_edPropMarginDl.SetWindowText(_T("0"));
//	m_edPropUlCmd.EnableMask(_T("DDD"), _T("___"), _T(' '));
//	m_edPropUlCmd.SetValidChars(NULL);
//	m_edPropUlCmd.SetWindowText(_T("0"));
//	m_edPropUlRng.EnableMask(_T("DDD"), _T("___"), _T(' '));
//	m_edPropUlRng.SetValidChars(NULL);
//	m_edPropUlRng.SetWindowText(_T("0"));

	m_btCheck[0].SetNotesType(eColor_ReadinessCheck);
	m_btCheck[1].SetNotesType(eColor_EqInitialize);
	m_btCheck[2].SetNotesType(eColor_Antenna);
	m_btCheck[3].SetNotesType(eColor_DownlinkRecv);
	m_btCheck[4].SetNotesType(eColor_PostPass);
	m_btCheck[5].SetNotesType(eColor_UplinkkSend);
	m_btCheck[6].SetNotesType(eColor_Command);
	m_btCheck[7].SetNotesType(eColor_Range);
	m_btCheck[8].SetNotesType(eColor_UsedForecast);

	GetDlgItem(IDC_STATIC_TIMELINEHEADER)->GetWindowRect(rect);
	ScreenToClient(rect);
	mGraphHeader.SetHeader();
	mGraphHeader.Create(WS_VISIBLE, rect, this, 1000);

	GetDlgItem(IDC_STATIC_TIMELINE)->GetWindowRect(rect);
	ScreenToClient(rect);
	mGraph.Create(WS_VISIBLE, rect, this, 1001);

	m_grpNotes.SetGroupType(CTimeGroupBoxEx::eGroupType_BoxLine);

	m_bRecLevelSModified = false;
	m_bRecLevelXModified = false;
	m_bSubCarTlmModified = false;
	m_bSubCarCmdModified = false;

	m_cbWayList.ResetContent();

	// 54mの場合
	if (theApp.GetSelectStation() == eStation_Usuda642)
	{
		// 予想受信レベルS帯をKa帯に変える
		GetDlgItem(IDC_STATIC_RCVLEVEL_S)->SetWindowTextW(_T("Ka帯 予想受信レベル"));
		GetDlgItem(IDC_STATIC_RCVLEVEL_X)->SetWindowTextW(_T("X帯 予想受信レベル"));

		CRect rectX, rectKa;
		GetDlgItem(IDC_STATIC_RCVLEVEL_S)->GetWindowRect(rectX);
		GetDlgItem(IDC_STATIC_RCVLEVEL_X)->GetWindowRect(rectKa);
		ScreenToClient(rectX);
		ScreenToClient(rectKa);
		GetDlgItem(IDC_STATIC_RCVLEVEL_S)->MoveWindow(rectKa);
		GetDlgItem(IDC_STATIC_RCVLEVEL_X)->MoveWindow(rectX);

		GetDlgItem(IDC_EDIT_RECLEVELVALS)->GetWindowRect(rectX);
		GetDlgItem(IDC_EDIT_RECLEVELVALX)->GetWindowRect(rectKa);
		ScreenToClient(rectX);
		ScreenToClient(rectKa);
		GetDlgItem(IDC_EDIT_RECLEVELVALS)->MoveWindow(rectKa);
		GetDlgItem(IDC_EDIT_RECLEVELVALX)->MoveWindow(rectX);

		// S-TLMを非活性化
		GetDlgItem(IDC_STATIC_STLMRUN)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_STLMRUNVAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_STLMSTART)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_STLMSTARTVAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_STLMSTOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_STLMSTOPVAL)->EnableWindow(FALSE);

		// LNAを非表示
		GetDlgItem(IDC_STATIC_OPEOPLNA)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_OPEOPLNAVAL)->ShowWindow(FALSE);
	}
}

/*============================================================================*/
/*! タイムライン

-# 運用計画情報の更新

@brief 運用計画情報の更新

@param	selectForecast	予報値番号
@retval TRUE:有効/FALSE:無効
*/
/*============================================================================*/
BOOL CTimeLineView::UpdateTimelineData(UINT selectForecast, bool bUntil/*=false*/)
{
	if (mGraph.mListPredData.size() == 0)
	{
		selectForecast = -1;
	}

	// 監視に衛星があるならばプロパティ設定を有効にする
	if (m_strSpaceName.IsEmpty() || m_strSpaceName.Left(1) == _T("-") || m_bReadCompletePlanFile == false)
	{
		// 運用プロパティ
		GetDlgItem(IDC_BUTTON_PROPSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PROPRESET)->EnableWindow(FALSE);

		// 初期設定プロパティ
		GetDlgItem(IDC_BUTTON_PROPINIT)->EnableWindow(FALSE);

		GetDlgItem(IDC_CHECK_TLM)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CMD)->EnableWindow(FALSE);
	}
	else
	{
		// 運用プロパティ
		GetDlgItem(IDC_BUTTON_PROPSET)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PROPRESET)->EnableWindow(TRUE);

		// 初期設定プロパティ
		GetDlgItem(IDC_BUTTON_PROPINIT)->EnableWindow(TRUE);

		GetDlgItem(IDC_CHECK_TLM)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CMD)->EnableWindow(TRUE);
	}

	vector<COprPlnData>::iterator itr;
	BOOL bFindSateAndPlanId = FALSE;
	BOOL bFindPrd = FALSE;
	for (itr = mGraph.mListTimeData.begin(); itr != mGraph.mListTimeData.end(); itr++){
		if ((*itr).m_strSatellite == m_strSpaceName && (*itr).m_strPlanID == mPlanId){
			// 衛星と計画IDをチェックし、一致するならば予報値切替可能にする
			bFindSateAndPlanId = TRUE;
			if ((*itr).m_strPredID.CompareNoCase(m_strForecast) == 0)
			{
				bFindPrd = TRUE;
			}
			break;
		}
	}
	if (bFindSateAndPlanId)
	{
		if (m_bReadCompletePlanFile)
		{
			// 予報値切替可
			GetDlgItem(IDC_BUTTON_FORECAST)->EnableWindow(TRUE);
		}
		else
		{
			// 予報値切替不可
			GetDlgItem(IDC_BUTTON_FORECAST)->EnableWindow(FALSE);
		}
	}
	else
	{
		// 予報値切替不可
		GetDlgItem(IDC_BUTTON_FORECAST)->EnableWindow(FALSE);

		// 各項目をディセーブル
		m_strOpeStartUntil = _T("-------------------");
		m_strOpeStopUntil = _T("-------------------");
		m_strUplnkStartUntil = _T("-------------------");
		m_strUplnkStopUntil = _T("-------------------");
		m_strRngStartUntil = _T("-------------------");
		m_strRngStopUntil = _T("-------------------");
		m_strCmdStartUntil = _T("-------------------");
		m_strCmdStopUntil = _T("-------------------");
		GetDlgItem(IDC_BUTTON_OPESTOP)->EnableWindow(FALSE);
		m_strUplnkRun = _T("---");
		m_strUplnkEvent = _T("---");
		m_strUplnkStart = _T("-------------------");
		m_strUplnkStop = _T("-------------------");
		GetDlgItem(IDC_BUTTON_UPLNK)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_UPLNKNO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UPLNKADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UPLNKUPDATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UPLNKDEL)->EnableWindow(FALSE);
		m_strCmdRun = _T("---");
		m_strCmdEvent = _T("---");
		m_strCmdStart = _T("-------------------");
		m_strCmdStop = _T("-------------------");
		GetDlgItem(IDC_BUTTON_CMD)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CMDNO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CMDADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CMDUPDATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CMDDEL)->EnableWindow(FALSE);
		m_strRngRun = _T("---");
		m_strRngEvent = _T("---");
		m_strRngStart = _T("-------------------");
		m_strRngStop = _T("-------------------");
		m_strRngType = _T("---");
		GetDlgItem(IDC_BUTTON_RNG)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RNGNO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RNGADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RNGUPDATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RNGDEL)->EnableWindow(FALSE);
		m_strOptInit = _T("---");
		m_strOptSend = _T("---");
		m_strOptRecv = _T("---");
		m_strOptSendout = _T("---");
		m_strOptLna = _T("---");
		m_strOptAntwait = _T("---");
		m_strOptAnt = _T("---");
		m_strReadAgc = _T("---");
		m_strReadRng = _T("---");
		m_strReadTlm = _T("---");
		m_strReadCmd = _T("---");
		m_strStlmRun = _T("---");
		m_strStlmStart = _T("-------------------");
		m_strStlmStop = _T("-------------------");
		m_strXtlmRun = _T("---");
		m_strXtlmStart = _T("-------------------");
		m_strXtlmStop = _T("-------------------");
		m_strKtlmRun = _T("---");
		m_strKtlmStart = _T("-------------------");
		m_strKtlmStop = _T("-------------------");
		m_strMasterSlave = _T("---");
		GetDlgItem(IDC_BUTTON_ANT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MASTERSLAVE)->EnableWindow(FALSE);
//		GetDlgItem(IDC_BUTTON_PROPSET)->EnableWindow(FALSE);
//		GetDlgItem(IDC_BUTTON_PROPRESET)->EnableWindow(FALSE);
		m_cbUplnkNo.ResetContent();
		m_cbCmdNo.ResetContent();
		m_cbRngNo.ResetContent();
		UpdateData(FALSE);
		mGraph.SetSelectForecast(-1 /*selectForecast*/);
		mGraphHeader.SetSelectForecast(-1 /*selectForecast*/);
		ClearTimeLineData();
		SetMonitorMode(this);

		// 衛星と計画IDが見つからなかった場合は、何もしない
		return FALSE;
	}

	// 監視の計画IDからタイムラインのインデックスを調べる
	int nFindTimeline = -1;
	int pos;
	CString strPlanPath = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
	// sttn -> enpの順に計画ファイルフルパスを比較する
	CString strPlanFullPath;
	for (int nFrom = 0; nFrom < 2 && nFindTimeline < 0; nFrom++)
	{
		if (nFrom == 0)
		{
			strPlanFullPath.Format(_T("%ssttn\\%s\\%s.pln"), (LPCTSTR)strPlanPath, (LPCTSTR)m_strSpaceName, (LPCTSTR)m_strPlanID);
		}
		else
		{
			strPlanFullPath.Format(_T("%senp\\%s\\%s.pln"), (LPCTSTR)strPlanPath, (LPCTSTR)m_strSpaceName, (LPCTSTR)m_strPlanID);
		}

		for (itr = mGraph.mListTimeData.begin(), pos = 0; itr != mGraph.mListTimeData.end() && nFindTimeline < 0; itr++, pos++)
		{
			if ((*itr).m_strPlanFilePath == strPlanFullPath)
			{
				// タイムラインの計画ファイルフルパスと監視の計画ファイルフルパスが一致
				nFindTimeline = pos;
				break;
			}
		}
	}

	if (nFindTimeline < 0)
	{
		// 各項目をディセーブル
		m_strOpeStartUntil = _T("-------------------");
		m_strOpeStopUntil = _T("-------------------");
		m_strUplnkStartUntil = _T("-------------------");
		m_strUplnkStopUntil = _T("-------------------");
		m_strRngStartUntil = _T("-------------------");
		m_strRngStopUntil = _T("-------------------");
		m_strCmdStartUntil = _T("-------------------");
		m_strCmdStopUntil = _T("-------------------");
		GetDlgItem(IDC_BUTTON_OPESTOP)->EnableWindow(FALSE);
		m_strUplnkRun = _T("---");
		m_strUplnkEvent = _T("---");
		m_strUplnkStart = _T("-------------------");
		m_strUplnkStop = _T("-------------------");
		GetDlgItem(IDC_BUTTON_UPLNK)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_UPLNKNO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UPLNKADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UPLNKUPDATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UPLNKDEL)->EnableWindow(FALSE);
		m_strCmdRun = _T("---");
		m_strCmdEvent = _T("---");
		m_strCmdStart = _T("-------------------");
		m_strCmdStop = _T("-------------------");
		GetDlgItem(IDC_BUTTON_CMD)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CMDNO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CMDADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CMDUPDATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CMDDEL)->EnableWindow(FALSE);
		m_strRngRun = _T("---");
		m_strRngEvent = _T("---");
		m_strRngStart = _T("-------------------");
		m_strRngStop = _T("-------------------");
		m_strRngType = _T("---");
		GetDlgItem(IDC_BUTTON_RNG)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RNGNO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RNGADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RNGUPDATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RNGDEL)->EnableWindow(FALSE);
		m_strOptInit = _T("---");
		m_strOptSend = _T("---");
		m_strOptRecv = _T("---");
		m_strOptSendout = _T("---");
		m_strOptLna = _T("---");
		m_strOptAntwait = _T("---");
		m_strOptAnt = _T("---");
		m_strReadAgc = _T("---");
		m_strReadRng = _T("---");
		m_strReadTlm = _T("---");
		m_strReadCmd = _T("---");
		m_strStlmRun = _T("---");
		m_strStlmStart = _T("-------------------");
		m_strStlmStop = _T("-------------------");
		m_strXtlmRun = _T("---");
		m_strXtlmStart = _T("-------------------");
		m_strXtlmStop = _T("-------------------");
		m_strKtlmRun = _T("---");
		m_strKtlmStart = _T("-------------------");
		m_strKtlmStop = _T("-------------------");
		m_strMasterSlave = _T("---");
		GetDlgItem(IDC_BUTTON_ANT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MASTERSLAVE)->EnableWindow(FALSE);
//		GetDlgItem(IDC_BUTTON_PROPSET)->EnableWindow(FALSE);
//		GetDlgItem(IDC_BUTTON_PROPRESET)->EnableWindow(FALSE);
		m_cbUplnkNo.ResetContent();
		m_cbCmdNo.ResetContent();
		m_cbRngNo.ResetContent();
		UpdateData(FALSE);
		SetMonitorMode(this);

		// 計画ファイルフルパスが見つからなかった場合は、何もしない
		return FALSE;
	}

	COprPlnData tdata = mGraph.mListTimeData[nFindTimeline];
	mSelectOpePlanData = nFindTimeline;
	CString str;

	//============================================================================
	// 運用計画情報の更新
	// 現在時刻の取得
	CTimeSpan sp;
	CTime curtime = mCurrentTime;

	// 運用状態
	int kind = ePlanKind_None;
	if (tdata.m_OpetDay.start <= curtime && tdata.m_OpetDay.end >= curtime){
		kind = ePlanKind_Pass;
	}
	else{
		CTimeSpan sp = 0;
		if (tdata.m_OpInit || tdata.m_AntStandby){
			if (theApp.GetSelectStation() == eStation_Usuda64 || theApp.GetSelectStation() == eStation_Usuda642)
				sp += CTimeSpan(0, 0, 0, OperationTime_AntennaWait);
			else
				sp += CTimeSpan(0, 0, 0, OperationTime_AntennaWait2);
		}
		if (tdata.m_AgcChkValue){
			sp += CTimeSpan(0, 0, 0, OperationTime_Agc);
		}
		if (tdata.m_RndChkValue){
			sp += CTimeSpan(0, 0, 0, OperationTime_Rng);
		}
		if (tdata.m_TlmChkValue){
			sp += CTimeSpan(0, 0, 0, OperationTime_Tlm);
		}
		if (tdata.m_CmdChkValue){
			sp += CTimeSpan(0, 0, 0, OperationTime_Cmd);
		}
		if ((tdata.m_OpetDay.start - sp) <= curtime && tdata.m_OpetDay.start > curtime){
			kind = ePlanKind_PrePass;
		}
		if (tdata.m_AntZenith){
			if (theApp.GetSelectStation() == eStation_Usuda64 || theApp.GetSelectStation() == eStation_Usuda642)
				sp = CTimeSpan(0, 0, 0, OperationTime_AntennaZenith);
			else
				sp = CTimeSpan(0, 0, 0, OperationTime_AntennaZenith2);
			if ((tdata.m_OpetDay.end + sp) > curtime && tdata.m_OpetDay.end < curtime){
				kind = ePlanKind_PostPass;
			}
		}
	}

	// 運用開始まで
	if (mStartTime < curtime){
		m_strOpeStartUntil = _T("--- --:--:--");
	}
	else{
		sp = mStartTime - curtime;
		m_strOpeStartUntil = sp.Format(_T("%D %H:%M:%S"));
	}
	// 運用終了まで
	if (mEndTime < curtime){
		m_strOpeStopUntil = _T("--- --:--:--");
	}
	else{
		sp = mEndTime - curtime;
		m_strOpeStopUntil = sp.Format(_T("%D %H:%M:%S"));
	}

	// アップリンクイベント位置を調べる
	int nUplnkNo = (int)tdata.m_ULDay.size() - 1;
	if (nUplnkNo < 0)
		nUplnkNo = 0;
	if (m_nUplnkEvent == 0 && tdata.m_UpLinkChk == TRUE)
	{
		if (tdata.m_ULDay.size() > 0)
		{
			for (int i = 0; i < tdata.m_ULDay.size(); i++)
			{
				if (curtime >= tdata.m_ULDay[i].start && curtime <= tdata.m_ULDay[i].end)
				{
					nUplnkNo = i;
					break;
				}
				else if (curtime < tdata.m_ULDay[i].start)
				{
					nUplnkNo = i;
					break;
				}
			}
		}
	}

	if (m_nUplnkNo < 0)
		m_nUplnkNo = 0;
	if (tdata.m_ULDay.size() <= m_nUplnkNo)
		m_nUplnkNo = 0;


	// RNGイベント位置を調べる
	int nRngNo = (int)tdata.m_RngDayList.size() - 1;
	if (nRngNo < 0)
		nRngNo = 0;
	if (m_nRngEvent == 0 && tdata.m_RngChk == TRUE)
	{
		if (tdata.m_RngDayList.size() > 0)
		{
			for (int i = 0; i < tdata.m_RngDayList.size(); i++)
			{
				if (curtime >= tdata.m_RngDayList[i].start && curtime <= tdata.m_RngDayList[i].end)
				{
					nRngNo = i;
					break;
				}
				else if (curtime < tdata.m_RngDayList[i].start)
				{
					nRngNo = i;
					break;
				}
			}
		}
	}

	if (m_nRngNo < 0)
		m_nRngNo = 0;
	if (tdata.m_RngDayList.size() <= m_nRngNo)
		m_nRngNo = 0;


	// CMDイベント位置を調べる
	int nCmdNo = (int)tdata.m_CmdDayList.size() - 1;
	if (nCmdNo < 0)
		nCmdNo = 0;
	if (m_nCmdEvent == 0 && tdata.m_CmdChk == TRUE)
	{
		if (tdata.m_CmdDayList.size() > 0)
		{
			for (int i = 0; i < tdata.m_CmdDayList.size(); i++)
			{
				if (curtime >= tdata.m_CmdDayList[i].start && curtime <= tdata.m_CmdDayList[i].end)
				{
					nCmdNo = i;
					break;
				}
				else if (curtime < tdata.m_CmdDayList[i].start)
				{
					nCmdNo = i;
					break;
				}
			}
		}
	}

	if (m_nCmdNo < 0)
		m_nCmdNo = 0;
	if (tdata.m_CmdDayList.size() <= m_nCmdNo)
		m_nCmdNo = 0;


	//============================================================================
	// アップリンク
	m_strUplnkRun = (tdata.m_UpLinkChk == TRUE) ? _T("あり") : _T("なし");
	bool bUplnkEventChanged = false;
	// 自動/手動モード取得
	if (GetKanshiUpLinkMode() == 1){
		// 自動
		if (m_nUplnkEvent == 1)
		{
			// 手動だった場合は自動にする
			m_nUplnkEvent = 0;
			bUplnkEventChanged = true;
		}
	}
	else{
		// 手動
		if (m_nUplnkEvent == 0)
		{
			// 自動だった場合は手動にする
			m_nUplnkEvent = 1;
			bUplnkEventChanged = true;
		}
	}

	// 自動モードでかつイベントあり
	if (m_nUplnkEvent == 0 && tdata.m_UpLinkChk == TRUE){
		m_strUplnkStart = (tdata.m_ULDay[m_nUplnkNo].start).Format(_T("%Y-%m-%d %H:%M:%S"));
		m_strUplnkStop = (tdata.m_ULDay[m_nUplnkNo].end).Format(_T("%Y-%m-%d %H:%M:%S"));
		if (bUntil == false || bUplnkEventChanged == true)
		{
			m_cbUplnkNo.ResetContent();
			for (int pos = 0; pos < tdata.m_ULDay.size(); pos++){
				str.Format(_T("%d"), pos + 1);
				m_cbUplnkNo.AddString(str);
			}
			m_cbUplnkNo.SetCurSel(m_nUplnkNo);
		}

		// アップリンク開始までの時刻表示
		if (tdata.m_ULDay.size() == 0 || tdata.m_ULDay[nUplnkNo].start < curtime){
			m_strUplnkStartUntil = _T("--- --:--:--");
		}
		else{
			sp = tdata.m_ULDay[nUplnkNo].start - curtime;
			m_strUplnkStartUntil = sp.Format(_T("%D %H:%M:%S"));
		}
		// アップリンク終了までの時刻表示
		if (tdata.m_ULDay.size() == 0 || tdata.m_ULDay[nUplnkNo].end < curtime){
			m_strUplnkStopUntil = _T("--- --:--:--");
		}
		else{
			sp = tdata.m_ULDay[nUplnkNo].end - curtime;
			m_strUplnkStopUntil = sp.Format(_T("%D %H:%M:%S"));
		}
	}
	else{
		// その他
		m_strUplnkStart = _T("-------------------");
		m_strUplnkStop = _T("-------------------");
		m_strUplnkStartUntil = _T("-------------------");
		m_strUplnkStopUntil = _T("-------------------");
		m_cbUplnkNo.ResetContent();
	}

	if (m_nUplnkEvent == 0){
		// 自動
		m_UplinkEventStatic.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
		if (m_bReadCompletePlanFile)
		{
			GetDlgItem(IDC_COMBO_UPLNKNO)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_UPLNKADD)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_UPLNKUPDATE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_UPLNKDEL)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_COMBO_UPLNKNO)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_UPLNKADD)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_UPLNKUPDATE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_UPLNKDEL)->EnableWindow(FALSE);
		}
	}
	else{
		// 手動
		m_UplinkEventStatic.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
		GetDlgItem(IDC_COMBO_UPLNKNO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UPLNKADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UPLNKUPDATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UPLNKDEL)->EnableWindow(FALSE);
	}
	if (m_bReadCompletePlanFile)
	{
		GetDlgItem(IDC_BUTTON_UPLNK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_UPLNK)->EnableWindow(FALSE);
	}
	m_strUplnkEvent = mEventMode[m_nUplnkEvent];
	m_UplinkEventStatic.Invalidate();
	mGraph.m_nUplnkEvent = m_nUplnkEvent;
	mGraphHeader.m_nUplnkEvent = m_nUplnkEvent;

	//============================================================================
	// CMD
	m_strCmdRun = (tdata.m_CmdChk == TRUE) ? _T("あり") : _T("なし");
	bool bCmdEventChanged = false;
	// 自動/手動モード取得
	if (GetKanshiCmdMode() == 1){
		// 自動
		if (m_nCmdEvent == 1)
		{
			// 手動だった場合は自動にする
			m_nCmdEvent = 0;
			bCmdEventChanged = true;
		}
	}
	else{
		// 手動
		if (m_nCmdEvent == 0)
		{
			// 自動だった場合は手動にする
			m_nCmdEvent = 1;
			bCmdEventChanged = true;
		}
	}

	// 自動モードでかつイベントあり
	if (m_nCmdEvent == 0 && tdata.m_CmdChk == TRUE){
		m_strCmdStart = (tdata.m_CmdDayList[m_nCmdNo].start).Format(_T("%Y-%m-%d %H:%M:%S"));
		m_strCmdStop = (tdata.m_CmdDayList[m_nCmdNo].end).Format(_T("%Y-%m-%d %H:%M:%S"));
		if (bUntil == false || bCmdEventChanged == true)
		{
			m_cbCmdNo.ResetContent();
			for (int pos = 0; pos < tdata.m_CmdDayList.size(); pos++){
				str.Format(_T("%d"), pos + 1);
				m_cbCmdNo.AddString(str);
			}
			m_cbCmdNo.SetCurSel(m_nCmdNo);
		}

		// CMD開始までの時刻表示
		if (tdata.m_CmdDayList.size() == 0 || tdata.m_CmdDayList[nCmdNo].start < curtime){
			m_strCmdStartUntil = _T("--- --:--:--");
		}
		else{
			sp = tdata.m_CmdDayList[nCmdNo].start - curtime;
			m_strCmdStartUntil = sp.Format(_T("%D %H:%M:%S"));
		}
		// CMD終了までの時刻表示
		if (tdata.m_CmdDayList.size() == 0 || tdata.m_CmdDayList[nCmdNo].end < curtime){
			m_strCmdStopUntil = _T("--- --:--:--");
		}
		else{
			sp = tdata.m_CmdDayList[nCmdNo].end - curtime;
			m_strCmdStopUntil = sp.Format(_T("%D %H:%M:%S"));
		}
	}
	else{
		// その他
		m_strCmdStart = _T("-------------------");
		m_strCmdStop = _T("-------------------");
		m_strCmdStartUntil = _T("-------------------");
		m_strCmdStopUntil = _T("-------------------");
		m_cbCmdNo.ResetContent();
	}

	if (m_nCmdEvent == 0){
		// 自動
		m_CmdEventStatic.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
		if (m_bReadCompletePlanFile)
		{
			GetDlgItem(IDC_COMBO_CMDNO)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_CMDADD)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_CMDUPDATE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_CMDDEL)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_COMBO_CMDNO)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CMDADD)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CMDUPDATE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CMDDEL)->EnableWindow(FALSE);
		}
	}
	else{
		// 手動
		m_CmdEventStatic.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
		GetDlgItem(IDC_COMBO_CMDNO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CMDADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CMDUPDATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CMDDEL)->EnableWindow(FALSE);
	}
	if (m_bReadCompletePlanFile)
	{
		GetDlgItem(IDC_BUTTON_CMD)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_CMD)->EnableWindow(FALSE);
	}
	m_strCmdEvent = mEventMode[m_nCmdEvent];
	m_CmdEventStatic.Invalidate();
	mGraph.m_nCmdEvent = m_nCmdEvent;
	mGraphHeader.m_nCmdEvent = m_nCmdEvent;

	//============================================================================
	// RNG
	m_strRngRun = (tdata.m_RngChk == TRUE) ? _T("あり") : _T("なし");
	bool bRngEventChanged = false;
	// 自動/手動モード取得
	if (GetKanshiRngMode() == 1){
		// 自動
		if (m_nRngEvent == 1)
		{
			// 手動だった場合は自動にする
			m_nRngEvent = 0;
			bRngEventChanged = true;
		}
	}
	else{
		// 手動
		if (m_nRngEvent == 0)
		{
			// 自動だった場合は手動にする
			m_nRngEvent = 1;
			bRngEventChanged = true;
		}
	}

	// 自動モードでかつイベントあり
	if (m_nRngEvent == 0 && tdata.m_RngChk == TRUE){
		m_strRngStart = (tdata.m_RngDayList[m_nRngNo].start).Format(_T("%Y-%m-%d %H:%M:%S"));
		m_strRngStop = (tdata.m_RngDayList[m_nRngNo].end).Format(_T("%Y-%m-%d %H:%M:%S"));
		m_strRngType = tdata.m_RngTypeList[m_nRngNo];
		if (bUntil == false || bRngEventChanged == true)
		{
			m_cbRngNo.ResetContent();
			for (int pos = 0; pos < tdata.m_RngDayList.size(); pos++){
				str.Format(_T("%d"), pos + 1);
				m_cbRngNo.AddString(str);
			}
			m_cbRngNo.SetCurSel(m_nRngNo);
		}

		// RNG開始までの時刻表示
		if (tdata.m_RngDayList.size() == 0 || tdata.m_RngDayList[nRngNo].start < curtime){
			m_strRngStartUntil = _T("--- --:--:--");
		}
		else{
			sp = tdata.m_RngDayList[nRngNo].start - curtime;
			m_strRngStartUntil = sp.Format(_T("%D %H:%M:%S"));
		}
		// RNG終了までの時刻表示
		if (tdata.m_RngDayList.size() == 0 || tdata.m_RngDayList[nRngNo].end < curtime){
			m_strRngStopUntil = _T("--- --:--:--");
		}
		else{
			sp = tdata.m_RngDayList[nRngNo].end - curtime;
			m_strRngStopUntil = sp.Format(_T("%D %H:%M:%S"));
		}
	}
	else{
		m_strRngStart = _T("-------------------");
		m_strRngStop = _T("-------------------");
		m_strRngStartUntil = _T("-------------------");
		m_strRngStopUntil = _T("-------------------");
		m_strRngType = _T("---");
		m_cbRngNo.ResetContent();
	}

	if (m_nRngEvent == 0){
		// 自動
		m_RngEventStatic.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
		if (m_bReadCompletePlanFile)
		{
			GetDlgItem(IDC_COMBO_RNGNO)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_RNGADD)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_RNGUPDATE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_RNGDEL)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_COMBO_RNGNO)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RNGADD)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RNGUPDATE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RNGDEL)->EnableWindow(FALSE);
		}
	}
	else{
		// 手動
		m_RngEventStatic.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
		GetDlgItem(IDC_COMBO_RNGNO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RNGADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RNGUPDATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RNGDEL)->EnableWindow(FALSE);
	}
	if (m_bReadCompletePlanFile)
	{
		GetDlgItem(IDC_BUTTON_RNG)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_RNG)->EnableWindow(FALSE);
	}
	m_strRngEvent = mEventMode[m_nRngEvent];
	m_RngEventStatic.Invalidate();
	mGraph.m_nRngEvent = m_nRngEvent;
	mGraphHeader.m_nRngEvent = m_nRngEvent;

	//============================================================================
	if (bUntil == true){

		if (m_bReadCompletePlanFile)
		{
			if (bFindPrd)
			{
				GetDlgItem(IDC_BUTTON_OPESTOP)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(IDC_BUTTON_OPESTOP)->EnableWindow(FALSE);
			}
			GetDlgItem(IDC_BUTTON_ANT)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_MASTERSLAVE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_OPESTOP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ANT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MASTERSLAVE)->EnableWindow(FALSE);
		}

		UpdateData(FALSE);
		SetMonitorMode(this);
		return TRUE;
	}

	//============================================================================
//	m_nUplnkNo = 0;
//	m_nCmdNo = 0;
//	m_nRngNo = 0;

	//============================================================================
	// 運用計画情報
//	GetDlgItem(IDC_BUTTON_FORECAST)->EnableWindow(TRUE);
	if (bFindPrd)
	{
		GetDlgItem(IDC_BUTTON_OPESTOP)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_OPESTOP)->EnableWindow(FALSE);
	}
//	m_strSpaceName = pdata.strSatellite;
//	m_strSpaceName.MakeUpper();

	// 衛星固有情報取得
	GetSatelliteXTX();

	theApp.SetTargetSpaceName(m_strSpaceName);
//	m_strPlanID = tdata.m_strPlanID;
//	m_strForecast = tdata.m_strPredID;

//	m_strOpeStart = (mStartTime).Format(_T("%Y-%m-%d %H:%M:%S"));
//	m_strOpeStop = (mEndTime).Format(_T("%Y-%m-%d %H:%M:%S"));

	//============================================================================
	//// 運用オプション
#if 0
	UpdateOperationOption(tdata.m_strCID);
#else
	const CString mTxramp[] = {
		_T("あり"),
		_T("なし（センター）"),
		_T("なし（ベストロック）"),
		_T("なし（ノミナル）"),
	};
	static const CString mRxramp[] = {
		_T("SX"),
		_T("S"),
		_T("X"),
		_T("OFF"),
	};
	static const CString mRxramp54[] = {
		_T("あり"),
		_T("なし"),
		_T(""),
		_T(""),
	};
	GetDlgItem(IDC_BUTTON_ANT)->EnableWindow(TRUE);
	m_strOptInit = (tdata.m_OpInit == 0) ? _T("なし") : _T("あり");
	m_strOptSend = mTxramp[tdata.m_OpTDP];

	//受信ドップラ補償
	if(theApp.GetSelectStation() == eStation_Usuda642)
	{
		m_strOptRecv = mRxramp54[tdata.m_OpRDP];
	}
	else
	{
		m_strOptRecv = mRxramp[tdata.m_OpRDP];
	}

	//m_strOptSendout = mOptSendOutput[tdata.m_OpTOutType];
	////CString m_strOptLna;
	//m_strOptAntwait = (tdata.m_AntStandby == 0) ? _T("なし") : _T("あり");
	//m_strOptAnt = mOptANTZenith[tdata.m_AntZenith];
#endif
	//============================================================================
	// レディネスチェック
	m_strReadAgc = (tdata.m_AgcChkValue == FALSE) ? _T("なし") : _T("あり");
	m_strReadRng = (tdata.m_RndChkValue == FALSE) ? _T("なし") : _T("あり");
	m_strReadTlm = (tdata.m_TlmChkValue == FALSE) ? _T("なし") : _T("あり");
	m_strReadCmd = (tdata.m_CmdChkValue == FALSE) ? _T("なし") : _T("あり");
	//============================================================================
	// S-TLM
	m_strStlmRun = (tdata.m_TlmSChk == FALSE) ? _T("なし") : _T("あり");
	if (tdata.m_TlmSChk == TRUE){
		if (tdata.m_TlmSDayList.size() > 0)
		{
			int idx = (int)tdata.m_TlmSDayList.size() -  1;
			if (curtime < tdata.m_TlmSDayList[0].start)
			{
				idx = 0;
			}
			else
			{
				for (int i = 0; i < tdata.m_TlmSDayList.size(); i++)
				{
					if (curtime >= tdata.m_TlmSDayList[i].start && curtime <= tdata.m_TlmSDayList[i].end)
					{
						idx = i;
						break;
					}
				}
			}

			m_strStlmStart = (tdata.m_TlmSDayList[idx].start).Format(_T("%Y-%m-%d %H:%M:%S"));
			m_strStlmStop = (tdata.m_TlmSDayList[idx].end).Format(_T("%Y-%m-%d %H:%M:%S"));
		}
		else
		{
			m_strStlmStart = _T("-------------------");
			m_strStlmStop = _T("-------------------");
		}
	}
	else{
		m_strStlmStart = _T("-------------------");
		m_strStlmStop = _T("-------------------");
	}
	//============================================================================
	// X-TLM
	m_strXtlmRun = (tdata.m_TlmXChk == FALSE) ? _T("なし") : _T("あり");
	if(tdata.m_TlmXChk == TRUE){
		if (tdata.m_TlmXDayList.size() > 0)
		{
			int idx = (int)tdata.m_TlmXDayList.size() - 1;
			if (curtime < tdata.m_TlmXDayList[0].start)
			{
				idx = 0;
			}
			else
			{
				for (int i = 0; i < tdata.m_TlmXDayList.size(); i++)
				{
					if (curtime >= tdata.m_TlmXDayList[i].start && curtime <= tdata.m_TlmXDayList[i].end)
					{
						idx = i;
						break;
					}
				}
			}

			m_strXtlmStart = (tdata.m_TlmXDayList[idx].start).Format(_T("%Y-%m-%d %H:%M:%S"));
			m_strXtlmStop = (tdata.m_TlmXDayList[idx].end).Format(_T("%Y-%m-%d %H:%M:%S"));
		}
		else
		{
			m_strXtlmStart = _T("-------------------");
			m_strXtlmStop = _T("-------------------");
		}

	}
	else{
		m_strXtlmStart = _T("-------------------");
		m_strXtlmStop = _T("-------------------");
	}
	//============================================================================
	// Ka-TLM
	m_strKtlmRun = (tdata.m_TlmKaChk == FALSE) ? _T("なし") : _T("あり");
	if (tdata.m_TlmKaChk == TRUE){
		if (tdata.m_TlmKaDayList.size() > 0)
		{
			int idx = (int)tdata.m_TlmKaDayList.size() - 1;
			if (curtime < tdata.m_TlmKaDayList[0].start)
			{
				idx = 0;
			}
			else
			{
				for (int i = 0; i < tdata.m_TlmKaDayList.size(); i++)
				{
					if (curtime >= tdata.m_TlmKaDayList[i].start && curtime <= tdata.m_TlmKaDayList[i].end)
					{
						idx = i;
						break;
					}
				}
			}

			m_strKtlmStart = (tdata.m_TlmKaDayList[idx].start).Format(_T("%Y-%m-%d %H:%M:%S"));
			m_strKtlmStop = (tdata.m_TlmKaDayList[idx].end).Format(_T("%Y-%m-%d %H:%M:%S"));
		}
		else
		{
			m_strKtlmStart = _T("-------------------");
			m_strKtlmStop = _T("-------------------");
		}
	}
	else{
		m_strKtlmStart = _T("-------------------");
		m_strKtlmStop = _T("-------------------");
	}
	//============================================================================
	// 主局/従局識別
	GetDlgItem(IDC_BUTTON_MASTERSLAVE)->EnableWindow(TRUE);
	m_strMasterSlave = (tdata.m_OpPriority == _T("MASTER")) ? mMasterSlabe[0] : mMasterSlabe[1];
	mMasterSlave = (tdata.m_OpPriority == _T("MASTER")) ? 0 : 1;

	//============================================================================
	// 運用タイムラインデータの登録
	SetTimeLineData(mGraph.mListTimeData[mSelectOpePlanData], selectForecast);

	SetMonitorMode(this);

	UpdateData(FALSE);

	return TRUE;
}

///*============================================================================*/
///*! タイムライン
//
//-# 運用計画情報の更新
//
//@brief 初期設定情報の更新
//
//@param
//@retval なし
//*/
///*============================================================================*/
//void CTimeLineView::UpdateInitializePropertyData()
//{
//	UpdateData(TRUE);
//	_ReadOpeProp();
//
//	//=================================================================
//	// 初期設定プロパティ情報
//	// 予想受信レベルの設定
//	// 衛星固有情報取得
//	GetSatelliteXTX();
//	m_strRecLevel.Format(_T("%3.2lf"), (mXtx == XTX_Y_ID) ? mOpeProp.init.reclevelx : mOpeProp.init.reclevels);
//	GetDlgItem(IDC_EDIT_RECLEVELVALS)->EnableWindow((mOpeProp.init.autocalc==1)?TRUE:FALSE);		// 予想受信レベル固定時に活性化
//	// TLMサブキャリアの設定
//	m_strTlm.Format(_T("%7.4lf"), mOpeProp.init.tlm);
//	// CMDサブキャリアの設定
//	m_strCmd.Format(_T("%7.4lf"), mOpeProp.init.cmd);
//	m_strTlmUser = m_strTlm;
//	m_strCmdUser = m_strCmd;
//
//	// 画面に値を表示
//	GetDlgItem(IDC_EDIT_RECLEVELVALS)->SetWindowTextW(m_strRecLevel);
//	GetDlgItem(IDC_EDIT_TLM)->SetWindowTextW(m_strTlm);
//	GetDlgItem(IDC_EDIT_CMD)->SetWindowTextW(m_strCmd);
//
//	UpdateData(FALSE);
//}
//
//
///*============================================================================*/
///*! タイムライン
//
//-# 運用計画情報の更新
//
//@brief 運用プロパティ情報の更新
//
//@param
//@retval なし
//*/
///*============================================================================*/
//void CTimeLineView::UpdateOperationProperty()
//{
//	UpdateData(TRUE);
//
//	CString str;
//
//	//=================================================================
//	// 運用プロパティ情報
//	// 監視データで設定
//	m_strPropEventCalc = (mOpeProp.event_autocalc==0)?_T("なし"):_T("あり");
//	m_strSameTimeMod = (mOpeProp.opespace == 0) ? _T("なし") : _T("あり");
//	UpdateData(FALSE);
//
//	str.Format(_T("%d"), mOpeProp.even_sunset);
//	GetDlgItem(IDC_EDIT_PROPEVENTSUNSET)->SetWindowText(str);
//	str.Format(_T("%d"), mOpeProp.event_sunrise);
//	GetDlgItem(IDC_EDIT_PROPEVENTSUNRISE)->SetWindowText(str);
//	str.Format(_T("%d"), mOpeProp.mg_ULon);
//	GetDlgItem(IDC_EDIT_PROPMARGINULON)->SetWindowText(str);
//	str.Format(_T("%d"), mOpeProp.mg_ULoff);
//	GetDlgItem(IDC_EDIT_PROPMARGINULOFF)->SetWindowText(str);
//	str.Format(_T("%d"), mOpeProp.mg_DL);
//	GetDlgItem(IDC_EDIT_PROPMARGINDL)->SetWindowText(str);
//	str.Format(_T("%d"), mOpeProp.of_CMD);
//	GetDlgItem(IDC_EDIT_PROPULCMD)->SetWindowText(str);
//	str.Format(_T("%d"), mOpeProp.of_RNG);
//	GetDlgItem(IDC_EDIT_PROPULRNG)->SetWindowText(str);
//	str.Format(_T("%s"), CString(mOpeProp.cid));
//	GetDlgItem(IDC_BUTTON_PROPCID)->SetWindowText(str);
//}


/*============================================================================*/
/*! タイムライン

-# 運用計画情報の更新

@brief 運用オプション情報の更新

@param
@retval なし
*/
/*============================================================================*/
void CTimeLineView::UpdateOperationOption()
{
	UpdateData(TRUE);

	if (mGraph.mListTimeData.size() == 0)
		return;

	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];
	CString strCID = tdata.m_strCID;

	//============================================================================
	//// 運用オプション
	// CIDファイルから取得
//	CString strCID;
//	GetDlgItem(IDC_BUTTON_PROPCID)->GetWindowText(strCID);
	if (strCID.IsEmpty() == true || m_strSpaceName.Left(1) == _T("-") || m_strSpaceName.IsEmpty()){
//		m_strOptSend = _T("---");
//		m_strOptRecv = _T("---");
		m_strOptSendout = _T("---");
		m_strOptLna = _T("---");
		m_strOptAntwait = _T("---");
		m_strOptAnt = _T("---");
	}
	else{
		CCidSelect cid;
		cid.ReadCIDFile();
		map<CString, map<CString, timeline::stCIDParam>> &cidlist = cid.m_CIDSlctData.GetCidParamList();
		timeline::stCIDParam stCid = timeline::CCIDSlctData::TrimInvalidString(cidlist[m_strSpaceName][strCID]);
//		m_strOptSend = stCid.TransDoppler;
//		m_strOptRecv = _T("---");
		m_strOptSendout = stCid.TrsPower;
		m_strOptLna = _T("あり");
		m_strOptAntwait = stCid.AntAosPosn;
		// 吉角様の仕様ではCIDから取得することになっていたが、
		// 実際は計画ファイルから取得 or CIDから取得？
		// CIDから取得
		m_strOptAnt = mOptANTZenith[0];
		for (int index = 0; index < sizeof(mCIDOptANTZenith) / sizeof(mCIDOptANTZenith[0]); index++){
			if (mCIDOptANTZenith[index] == stCid.AntWaitPoint){
				int antZenithIdx = index;
				if (index == 3)
				{
					// 文字列が"天頂"だけの場合は"天頂(LOCK)"と同じ
					antZenithIdx = 1;
				}
				m_strOptAnt = mOptANTZenith[antZenithIdx];
			}
		}
		// 計画から取得
		//m_strOptAnt = mOptANTZenith[tdata.m_AntZenith];
	}

	UpdateData(FALSE);
}

/*============================================================================*/
/*! タイムライン

-# 運用計画情報の更新

@brief 監視データを取得して更新を行う

@param  なし
@retval なし
*/
/*============================================================================*/
void CTimeLineView::UpdateMonitorData()
{
	enum{
		eINT,
		eDWORD,
		eDOUBLE,
		eSTRING,
	};

	// コントロールがあるプロパティの監視取得
	typedef struct{
		const CString		monname;		// 監視名
		CString&			controlval;		// コントロール値
		UINT				controlid;		// コントロールID
		const CString		format;			// フォーマット
		UINT				type;			// 種別：0:INT 1:FLOAT 2:DOUBLE 3:STRING
		int&				nval;			// int値
		DWORD&				dwval;			// DOWRD値
		double&				dbval;			// double値
		CString&			strval;			// 文字列値
	} stSetCtlMonData;

	CString temp, curTemp;
	int nval;
	DWORD dwval;
	double dbval;
	CString strval;

	stSetCtlMonData mCtlMonTable[] = {
		//{ _T("CTRL.SAT_NAME"), m_strSpaceName, 0, _T("%s"), eSTRING },	// 衛星名
		//{ _T("CTRL.PASS_NO"), m_strPlanID, 0, _T("%s"), eSTRING },	// 計画ID
		//{ _T("CTRL.PASS_FLAG"), m_strOpeStatus, 0, _T("%s"), eSTRING },	// 運用状態

		{ _T("CTRL.PASS_INFO"), m_strOpeStatus, 0, _T("%s"), eSTRING, nval, dwval, dbval, strval },	// 運用状態

		//		{ _T("CTRL.START_TIME"), m_strOpeStart, 0, _T("%s"), eSTRING },	// 運用開始日時
		//		{ _T("CTRL.END_TIME"), m_strOpeStop, 0, _T("%s"), eSTRING },	// 運用終了日時
		//{ _T("CTRL.PASS_FLAG_STR"), m_strOpeStartUntil, 0, _T("%s"), eSTRING },	// 運用開始まで
		//{ _T("CTRL.PASS_FLAG_STR"), m_strOpeStopUntil, 0, _T("%s"), eSTRING },	// 運用終了まで

		{ _T("CTRL.EVENT_CALC"), temp, IDC_EDIT_PROPEVENTAUTOVAL, _T("%s"), eSTRING, mOpeProp.event_autocalc, dwval, dbval, strval },	// イベント日時の自動計算
		{ _T("CTRL.SAME_TIME_MOD"), temp, IDC_EDIT_PROPSPACEVAL, _T("%s"), eSTRING, mOpeProp.opespace, dwval, dbval, strval },	// CMD/RNG同時変調

		{ _T("CTRL.ZENITH_IN"), temp, IDC_EDIT_PROPEVENTSUNSET, _T("%2.2lf"), eDWORD, nval, dwval, mOpeProp.even_sunset, strval },	// 天頂通過角度入り
		{ _T("CTRL.ZENITH_OUT"), temp, IDC_EDIT_PROPEVENTSUNRISE, _T("%2.2lf"), eDWORD, nval, dwval, mOpeProp.event_sunrise, strval },	// 天頂通過角度明け
		{ _T("CTRL.MARGIN_UP_ON"), temp, IDC_EDIT_PROPMARGINULON, _T("%2.2lf"), eDWORD, nval, dwval, mOpeProp.mg_ULon, strval },	// 角度マージン U/L ON
		{ _T("CTRL.MARGIN_UP_OFF"), temp, IDC_EDIT_PROPMARGINULOFF, _T("%2.2lf"), eDWORD, nval, dwval, mOpeProp.mg_ULoff, strval },	// 角度マージン U/L OFF
		{ _T("CTRL.MARGIN_DOWN"), temp, IDC_EDIT_PROPMARGINDL, _T("%2.2lf"), eDWORD, nval, dwval, mOpeProp.mg_DL, strval },	// 角度マージン ダウンリンク
		{ _T("CTRL.OFS_CMD_END"), temp, IDC_EDIT_PROPULCMD, _T("%.0f"), eDWORD, nval, mOpeProp.of_CMD, dbval, strval },	// CMD運用終了
		{ _T("CTRL.OFS_RNG_END"), temp, IDC_EDIT_PROPULRNG, _T("%.0f"), eDWORD, nval, mOpeProp.of_RNG, dbval, strval },	// RNG運用終了

		{ _T("CTRL.DEFAULT_CID"), temp, IDC_BUTTON_PROPCID, _T("%s"), eSTRING, nval, dwval, dbval, strval },	// デフォルトCID

		{ _T("CTRL.LEVEL_FIXED_S"), temp, IDC_EDIT_RECLEVELVALS, _T("%3.2lf"), eDOUBLE, nval, dwval, mOpeProp.init.reclevels, strval },		// 予想受信レベル固定値(S)
		{ _T("CTRL.LEVEL_FIXED_X"), temp, IDC_EDIT_RECLEVELVALX, _T("%3.2lf"), eDOUBLE, nval, dwval, mOpeProp.init.reclevelx, strval },		// 予想受信レベル固定値(X)
		{ _T("CTRL.SC_TLM_FREQ"), temp, IDC_EDIT_TLM, _T("%7.4lf"), eDOUBLE, nval, dwval, mOpeProp.init.tlm, strval },		// TLMサブキャリア
		{ _T("CTRL.SC_CMD_FREQ"), temp, IDC_EDIT_CMD, _T("%7.4lf"), eDOUBLE, nval, dwval, mOpeProp.init.cmd, strval },		// CMDサブキャリア
	};

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT sta = theApp.GetSelectStation();
	TCHAR mUdsc54LevelFixedS[_MAX_PATH] = _T("CTRL.LEVEL_FIXED_Ka");

	/****************************************************************/
	/* 監視情報（文字列） */
	for (int index = 0; index < sizeof(mCtlMonTable) / sizeof(mCtlMonTable[0]); index++){
		// テーブルの監視名から監視データを取得して、コントロール値に設定する
		if (mCtlMonTable[index].type != eSTRING)
			continue;

		obs_adr = (CEngValCnv::obsname_t *)theApp.GetObsname_t(sta, mCtlMonTable[index].monname);
		mCtlMonTable[index].controlval.Empty();
		if (mCtlMonTable[index].controlid == IDC_BUTTON_PROPCID){
			// デフォルトCID
			if (obs_adr != NULL)
			{
				temp.Format(mCtlMonTable[index].format, (LPCTSTR)CString(obs_adr->sz_sts_name));
			}
			else
			{
				temp.Format(_T(""));
			}
			GetDlgItem(mCtlMonTable[index].controlid)->GetWindowText(curTemp);
			if (temp.Compare(curTemp) != 0)
				GetDlgItem(mCtlMonTable[index].controlid)->SetWindowText(temp);
			sprintf_s(mOpeProp.cid, 30, "%s", (LPCSTR)CStringA(temp));
		}
		else if (mCtlMonTable[index].controlid == IDC_EDIT_PROPEVENTAUTOVAL ||
				 mCtlMonTable[index].controlid == IDC_EDIT_PROPSPACEVAL){
			// イベント日時の自動計算、CMD/RNG同時変調
			if (obs_adr != NULL)
			{
				temp.Format(mCtlMonTable[index].format, (LPCTSTR)CString(obs_adr->sz_sts_name));
				if (temp.CompareNoCase(_T("ON")) == 0)
				{
					temp.Format(_T("あり"));
					mCtlMonTable[index].nval = 1;
				}
				else if (temp.CompareNoCase(_T("OFF")) == 0)
				{
					temp.Format(_T("なし"));
					mCtlMonTable[index].nval = 0;
				}
				else
				{
					temp.Format(_T(""));
					mCtlMonTable[index].nval = 0;
				}
			}
			else
			{
				temp.Format(_T(""));
				mCtlMonTable[index].nval = 0;
			}
			GetDlgItem(mCtlMonTable[index].controlid)->GetWindowText(curTemp);
			if (temp.Compare(curTemp) != 0)
				GetDlgItem(mCtlMonTable[index].controlid)->SetWindowText(temp);
		}
		else{
			// その他
			if (obs_adr != NULL)
			{
				mCtlMonTable[index].controlval.Format(mCtlMonTable[index].format, (LPCTSTR)CString(obs_adr->sz_sts_name));
			}
			else
			{
				mCtlMonTable[index].controlval.Format(mCtlMonTable[index].format, _T(""));
			}
		}
	}

	/****************************************************************/
	/* WAY情報 */
	CString str;
	CString way;
	m_cbWayList.ResetContent();
	{
		const CString strDateTimeNull = _T("-  -     :  :");
		const CString strDataNull = _T("      -  -  ");
		const CString strTimeNull = _T("    :  :  ");

		obs_adr = (CEngValCnv::obsname_t *)theApp.GetObsname_t(sta, _T("CTRL.1WAY_RECV_TIME"));
		if (obs_adr != NULL){
			m_cbWayList.AddString(_T("1WAY受信開始時刻"));
			way = CString(obs_adr->sz_sts_name);
			if (way.Find(strDateTimeNull) >= 0)
			{
				m_cbWayList.AddString(strDataNull);
				m_cbWayList.AddString(strTimeNull);
			}
			else
			{
				AfxExtractSubString(temp, way, 0, ' ');
				str.Format(_T("  %s"), (LPCTSTR)temp);
				m_cbWayList.AddString(str);
				AfxExtractSubString(temp, way, 1, ' ');
				str.Format(_T("  %s"), (LPCTSTR)temp);
				m_cbWayList.AddString(str);
			}
		}

		obs_adr = (CEngValCnv::obsname_t *)theApp.GetObsname_t(sta, _T("CTRL.2WAY_RECV_TIME"));
		if (obs_adr != NULL){
			m_cbWayList.AddString(_T("2WAY受信開始時刻"));
			way = CString(obs_adr->sz_sts_name);
			if (way.Find(strDateTimeNull) >= 0)
			{
				m_cbWayList.AddString(strDataNull);
				m_cbWayList.AddString(strTimeNull);
			}
			else
			{
				AfxExtractSubString(temp, way, 0, ' ');
				str.Format(_T("  %s"), (LPCTSTR)temp);
				m_cbWayList.AddString(str);
				AfxExtractSubString(temp, way, 1, ' ');
				str.Format(_T("  %s"), (LPCTSTR)temp);
				m_cbWayList.AddString(str);
			}
		}
	}

	GetSatelliteXTX();

//#ifdef _DEBUG
//	CEngValCnv::obsname_t obsAdr;
//#endif

	/****************************************************************/
	/* 監視情報（文字列以外） */
	for (int index = 0; index < sizeof(mCtlMonTable) / sizeof(mCtlMonTable[0]); index++){
		// テーブルの監視名から監視データを取得して、コントロール値に設定する
		if (mCtlMonTable[index].type == eSTRING)
			continue;

		if (sta == eStation_Usuda642 && mCtlMonTable[index].controlid == IDC_EDIT_RECLEVELVALS)
		{
			obs_adr = (CEngValCnv::obsname_t *)theApp.GetObsname_t(sta, mUdsc54LevelFixedS);
//#ifdef _DEBUG
//			obs_adr = &obsAdr;
//			obs_adr->d_data = -1.23;
//#endif
		}
		else
		{
			obs_adr = (CEngValCnv::obsname_t *)theApp.GetObsname_t(sta, mCtlMonTable[index].monname);
		}

		if (obs_adr != NULL)
		{
			temp.Format(mCtlMonTable[index].format, obs_adr->d_data);

			// S帯予想受信レベル, X帯予想受信レベル, TLMサブキャリア, CMDサブキャリアはキーボード入力更新されていた、または監視データが変ってないならば表示更新しない
			if (mCtlMonTable[index].controlid == IDC_EDIT_RECLEVELVALS)
			{
				if (m_bRecLevelSModified == true || temp.Compare(m_strRecLevelS) == 0)
					continue;
				m_strRecLevelS.Format(_T("%3.2lf"), obs_adr->d_data);
			}
			if (mCtlMonTable[index].controlid == IDC_EDIT_RECLEVELVALX)
			{
				if (m_bRecLevelXModified == true || temp.Compare(m_strRecLevelX) == 0)
					continue;
				m_strRecLevelX.Format(_T("%3.2lf"), obs_adr->d_data);
			}
			if (mCtlMonTable[index].controlid == IDC_EDIT_TLM)
			{
				if (m_bSubCarTlmModified == true || temp.Compare(m_strTlm) == 0)
					continue;
				m_strTlm.Format(_T("%7.4lf"), obs_adr->d_data);
			}
			if (mCtlMonTable[index].controlid == IDC_EDIT_CMD)
			{
				if (m_bSubCarCmdModified == true || temp.Compare(m_strCmd) == 0)
					continue;
				m_strCmd.Format(_T("%7.4lf"), obs_adr->d_data);
			}

			mCtlMonTable[index].dwval = (DWORD)obs_adr->d_data;
			mCtlMonTable[index].dbval = obs_adr->d_data;
		}
		else
		{
			if (mCtlMonTable[index].controlid == IDC_EDIT_RECLEVELVALS)
			{
				if (m_bRecLevelSModified == true)
					continue;
			}
			if (mCtlMonTable[index].controlid == IDC_EDIT_RECLEVELVALX)
			{
				if (m_bRecLevelXModified == true)
					continue;
			}
			if (mCtlMonTable[index].controlid == IDC_EDIT_TLM)
			{
				if (m_bSubCarTlmModified == true)
					continue;
			}
			if (mCtlMonTable[index].controlid == IDC_EDIT_CMD)
			{
				if (m_bSubCarCmdModified == true)
					continue;
			}

			temp.Format(_T(""));
			mCtlMonTable[index].dwval = 0;
			mCtlMonTable[index].dbval = 0.0;
		}

		GetDlgItem(mCtlMonTable[index].controlid)->GetWindowText(curTemp);
		if (temp.Compare(curTemp) != 0)
			// 表示更新
			GetDlgItem(mCtlMonTable[index].controlid)->SetWindowText(temp);
	}

	/********************************************************************************************************************************/
	// コントロールがない監視の取得

	typedef struct{
		const CString		monname;		// 監視名
		UINT				id;				// ID
		UINT				type;			// 種別：0:INT 1:FLOAT 2:DOUBLE 3:STRING
		int&				nval;			// int値
		double&				dbval;			// double値
	} stSetNonCtlMonData;

	stSetNonCtlMonData mNonCtlMonTable[] = {
		{ _T("CTRL.LEAP_YEAR"), eLEAP_YEAR, eSTRING, mOpeProp.init.leapyear, dbval },		// うるう年
		{ _T("CTRL.LEAP_SEC"), eLEAP_SEC, eSTRING, mOpeProp.init.leapsec, dbval },			// うるう秒
		{ _T("CTRL.EXPECT_LEVEL"), eEXPECT_LEVEL, eSTRING, mOpeProp.init.autocalc, dbval },	// 予想受信レベル自動計算選択
		{ _T("CTRL.EIRP"), eEIRP, eDOUBLE, nval, mOpeProp.init.eirp },						// 衛星送信EIRP
		{ _T("CTRL.GAIN"), eGAIN, eDOUBLE, nval, mOpeProp.init.gain },						// 地上局ANT利得
	};

	for (int index = 0; index < sizeof(mNonCtlMonTable) / sizeof(mNonCtlMonTable[0]); index++){
		// テーブルの監視名から監視データを取得して、プロパティメンバに設定する
		obs_adr = (CEngValCnv::obsname_t *)theApp.GetObsname_t(sta, mNonCtlMonTable[index].monname);

		if (mNonCtlMonTable[index].type == eSTRING)
		{
			// 文字列の場合
			if (mNonCtlMonTable[index].id == eLEAP_YEAR || mNonCtlMonTable[index].id == eEXPECT_LEVEL)
			{
				// うるう年、予想受信レベル自動計算選択はON/OFF判定
				if (obs_adr != NULL)
				{
					temp = CString(obs_adr->sz_sts_name);
					if (temp.CompareNoCase(_T("ON")) == 0)
					{
						mNonCtlMonTable[index].nval = 1;
					}
					else if (temp.CompareNoCase(_T("OFF")) == 0)
					{
						mNonCtlMonTable[index].nval = 0;
					}
					else
					{
						mNonCtlMonTable[index].nval = 0;
					}
				}
				else
				{
					mNonCtlMonTable[index].nval = 0;
				}
			}
			else if (mNonCtlMonTable[index].id == eLEAP_SEC)
			{
				// 1,4,7,10,OFF判定値取得
				if (obs_adr != NULL)
				{
					mNonCtlMonTable[index].nval = _GetLeapSecNo(CString(obs_adr->sz_sts_name));
				}
				else
				{
					mNonCtlMonTable[index].nval = 0;
				}
			}
		}
		else if (mNonCtlMonTable[index].type == eDOUBLE)
		{
			// doubleの場合
			if (obs_adr != NULL)
			{
				mNonCtlMonTable[index].dbval = obs_adr->d_data;
			}
			else
			{
				mNonCtlMonTable[index].dbval = 0.0;
			}
		}
	}

	// 予想受信レベルの活性化
	GetDlgItem(IDC_EDIT_RECLEVELVALS)->EnableWindow((mOpeProp.init.autocalc == 0) ? TRUE : FALSE);		// 予想受信レベル固定時に活性化
	GetDlgItem(IDC_EDIT_RECLEVELVALX)->EnableWindow((mOpeProp.init.autocalc == 0) ? TRUE : FALSE);		// 予想受信レベル固定時に活性化

	UpdateData(FALSE);
}

/*============================================================================*/
/*! 運用プロパティ

-# 運用プロパティの読み込み

@param
@retval

*/
/*============================================================================*/
bool CTimeLineView::_ReadOpeProp()
{
	if (mGraph.mListTimeData.size() == 0){
		// 運用計画データが存在しない
		return false;
	}
	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];
	mOpeProp_Station = m_strSpaceName;

	bool ret = true;
	CString forg = theApp.GetShareFilePath(eFileType_OperationProp_Folder, theApp.GetSelectStation());
	CString fname = theApp.GetShareFilePath(eFileType_OperationProp_Folder, theApp.GetSelectStation());
	forg += mOpePropFileName;
	fname += mOpePropFileTemp;
	CopyFile(forg, fname, FALSE);

	CIniFileMap cini(fname);

	UINT sta = theApp.GetSelectStation();

	// 初期化
	memset(&mOpeProp, 0, sizeof(stOpeProperty));
	// 新しいプロパティデータはINIファイル形式になっている
	TCHAR buf[MAX_PATH];
	//--------------------------
	// 共通部
	//--------------------------
	// 一度GetPrivateProfileStringを実行すると、セクションマップが作成されるので、
	// 作成されたセクションマップを使用してセクション文字列にする
	GetPrivateProfileString(_T("COMMON"), _T("LEAP_YEAR"), _T("OFF"), buf, MAX_PATH, fname);
	if (CString(buf) == _T("OFF"))
		mOpeProp.init.leapyear = 0;
	else
		mOpeProp.init.leapyear = 1;
	GetPrivateProfileString(_T("COMMON"), _T("LEAP_SEC"), _T("OFF"), buf, MAX_PATH, fname);
	mOpeProp.init.leapsec = _GetLeapSecNo(CString(buf));
	GetPrivateProfileString(_T("COMMON"), _T("GAIN"), _T("0"), buf, MAX_PATH, fname);
	mOpeProp.init.gain = _wtof(buf);
	//----------------------------
	vector<CString>	sectionList;
	vector<CString>::iterator itr;
	cini.GetSectionList(sectionList);
	//----------------------------
	// 衛星固有部
	//----------------------------
	for (itr = sectionList.begin(); itr != sectionList.end(); itr++){
		if (mOpeProp_Station.CompareNoCase((*itr)) == 0){
			// 同じ衛星名が見つかった
			GetPrivateProfileString((*itr), _T("EVENT_CALC"), _T("OFF"), buf, MAX_PATH, fname);
			if (CString(buf) == _T("OFF"))
				mOpeProp.event_autocalc = 0;
			else
				mOpeProp.event_autocalc = 1;
			GetPrivateProfileString((*itr), _T("ZENITH_IN"), _T("0"), buf, MAX_PATH, fname);
			mOpeProp.even_sunset = _wtof(buf);
			GetPrivateProfileString((*itr), _T("ZENITH_OUT"), _T("0"), buf, MAX_PATH, fname);
			mOpeProp.event_sunrise = _wtof(buf);
			GetPrivateProfileString((*itr), _T("MARGIN_UP_ON"), _T("0"), buf, MAX_PATH, fname);
			mOpeProp.mg_ULon = _wtof(buf);
			GetPrivateProfileString((*itr), _T("MARGIN_UP_OFF"), _T("0"), buf, MAX_PATH, fname);
			mOpeProp.mg_ULoff = _wtof(buf);
			GetPrivateProfileString((*itr), _T("MARGIN_DOWN"), _T("0"), buf, MAX_PATH, fname);
			mOpeProp.mg_DL = _wtof(buf);
			GetPrivateProfileString((*itr), _T("OFFSET_CMD_END"), _T("0"), buf, MAX_PATH, fname);
			mOpeProp.of_CMD = _wtoi(buf);
			GetPrivateProfileString((*itr), _T("OFFSET_RNG_END"), _T("0"), buf, MAX_PATH, fname);
			mOpeProp.of_RNG = _wtoi(buf);
			GetPrivateProfileString((*itr), _T("DEFAULT_CID"), _T("0"), buf, MAX_PATH, fname);
			sprintf_s(mOpeProp.cid, 30, "%s", (LPCSTR)CStringA(buf));
			GetPrivateProfileString((*itr), _T("SAME_TIME_MOD"), _T("OFF"), buf, MAX_PATH, fname);
			if (CString(buf) == _T("OFF"))
				mOpeProp.opespace = 0;
			else
				mOpeProp.opespace = 1;
			GetPrivateProfileString((*itr), _T("EXPECT_LEVEL"), _T("FIXED"), buf, MAX_PATH, fname);
			if (CString(buf) == _T("FIXED"))
				// 予想受信レベル固定時
				mOpeProp.init.autocalc = 0;
			else
				// 予想受信レベル自動時
				mOpeProp.init.autocalc = 1;
			if (sta == eStation_Usuda642)
			{
				GetPrivateProfileString((*itr), _T("EXPECT_LEVEL_FIXED_KA"), _T("0"), buf, MAX_PATH, fname);
			}
			else
			{
				GetPrivateProfileString((*itr), _T("EXPECT_LEVEL_FIXED_S"), _T("0"), buf, MAX_PATH, fname);
			}
			mOpeProp.init.reclevels = _wtof(buf);
			GetPrivateProfileString((*itr), _T("EXPECT_LEVEL_FIXED_X"), _T("0"), buf, MAX_PATH, fname);
			mOpeProp.init.reclevelx = _wtof(buf);
			GetPrivateProfileString((*itr), _T("EIRP"), _T("0"), buf, MAX_PATH, fname);
			mOpeProp.init.eirp = _wtof(buf);
			GetPrivateProfileString((*itr), _T("SUBSARR_TLM"), _T("0"), buf, MAX_PATH, fname);
			mOpeProp.init.tlmflag = true;
			mOpeProp.init.tlm = _wtof(buf);
			GetPrivateProfileString((*itr), _T("SUBSARR_CMD"), _T("0"), buf, MAX_PATH, fname);
			mOpeProp.init.cmdflag = true;
			mOpeProp.init.cmd = _wtof(buf);
			break;
		}
	}
	DeleteFile(fname);

	return ret;
}
/*============================================================================*/
/*! 運用プロパティ

-# 運用プロパティの書き込み

@param
@retval

*/
/*============================================================================*/
bool CTimeLineView::_WriteOpeProp(bool bInitialize/*=false*/)
{
//	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];
	mOpeProp_Station = m_strSpaceName;

	bool ret = true;
	CString str;
	CString backName;
	CString fname = theApp.GetShareFilePath(eFileType_OperationProp_Folder, theApp.GetSelectStation());
	backName = fname;
	fname += mOpePropFileName;
	backName += mOpePropFileNameBackup;

	SYSTEMTIME st_UTC;
	GetSystemTime(&st_UTC);
	str.Format(_T("%4d%02d%02d%02d%02d%02d"), st_UTC.wYear, st_UTC.wMonth, st_UTC.wDay, st_UTC.wHour, st_UTC.wMinute, st_UTC.wSecond);
	backName += str;
	CopyFile(fname, backName, FALSE);

	UINT sta = theApp.GetSelectStation();

	// 新しいプロパティデータはINIファイル形式になっている
	//--------------------------
	// 共通部
	//--------------------------
	if (bInitialize == false){
		// 一度GetPrivateProfileStringを実行すると、セクションマップが作成されるので、
		// 作成されたセクションマップを使用してセクション文字列にする
		str.Format(_T("%s"), (LPCTSTR)mStatusString[mOpeProp.init.leapyear]);
		WritePrivateProfileString(_T("COMMON"), _T("LEAP_YEAR"), str, fname);
		str.Format(_T("%s"), (LPCTSTR)_GetLeapSecString(mOpeProp.init.leapsec));
		WritePrivateProfileString(_T("COMMON"), _T("LEAP_SEC"), str, fname);
		str.Format(_T("%3.2lf"), mOpeProp.init.gain);
		WritePrivateProfileString(_T("COMMON"), _T("GAIN"), str, fname);
	}
	//----------------------------
	// 衛星固有部
	//----------------------------
	if (bInitialize == false){
		str.Format(_T("%s"), (LPCTSTR)mStatusString[mOpeProp.event_autocalc]);
		WritePrivateProfileString(mOpeProp_Station, _T("EVENT_CALC"), str, fname);
		str.Format(_T("%2.2lf"), mOpeProp.even_sunset);
		WritePrivateProfileString(mOpeProp_Station, _T("ZENITH_IN"), str, fname);
		str.Format(_T("%2.2lf"), mOpeProp.event_sunrise);
		WritePrivateProfileString(mOpeProp_Station, _T("ZENITH_OUT"), str, fname);
		str.Format(_T("%2.2lf"), mOpeProp.mg_ULon);
		WritePrivateProfileString(mOpeProp_Station, _T("MARGIN_UP_ON"), str, fname);
		str.Format(_T("%2.2lf"), mOpeProp.mg_ULoff);
		WritePrivateProfileString(mOpeProp_Station, _T("MARGIN_UP_OFF"), str, fname);
		str.Format(_T("%2.2lf"), mOpeProp.mg_DL);
		WritePrivateProfileString(mOpeProp_Station, _T("MARGIN_DOWN"), str, fname);
		str.Format(_T("%d"), mOpeProp.of_CMD);
		WritePrivateProfileString(mOpeProp_Station, _T("OFFSET_CMD_END"), str, fname);
		str.Format(_T("%d"), mOpeProp.of_RNG);
		WritePrivateProfileString(mOpeProp_Station, _T("OFFSET_RNG_END"), str, fname);
		str.Format(_T("%s"), (LPCTSTR)CString(mOpeProp.cid));
		WritePrivateProfileString(mOpeProp_Station, _T("DEFAULT_CID"), str, fname);
		str.Format(_T("%s"), (LPCTSTR)mStatusString[mOpeProp.opespace]);
		WritePrivateProfileString(mOpeProp_Station, _T("SAME_TIME_MOD"), str, fname);
		str.Format(_T("%s"), (LPCTSTR)mAutoCalString[mOpeProp.init.autocalc]);
		WritePrivateProfileString(mOpeProp_Station, _T("EXPECT_LEVEL"), str, fname);
		str.Format(_T("%3.2lf"), mOpeProp.init.eirp);
		WritePrivateProfileString(mOpeProp_Station, _T("EIRP"), str, fname);
	}
	if (bInitialize == true){
		if (mOpeProp.init.autocalc == 0)
		{
			// 予想受信レベル固定時
			str.Format(_T("%3.2lf"), mOpeProp.init.reclevels);
			if (sta == eStation_Usuda642)
			{
				WritePrivateProfileString(mOpeProp_Station, _T("EXPECT_LEVEL_FIXED_KA"), str, fname);
			}
			else
			{
				WritePrivateProfileString(mOpeProp_Station, _T("EXPECT_LEVEL_FIXED_S"), str, fname);
			}
			str.Format(_T("%3.2lf"), mOpeProp.init.reclevelx);
			WritePrivateProfileString(mOpeProp_Station, _T("EXPECT_LEVEL_FIXED_X"), str, fname);
		}
		if (m_bTlm)
		{
			// TLMサブキャリア入力有効時に保存
			str.Format(_T("%7.4lf"), mOpeProp.init.tlm);
			WritePrivateProfileString(mOpeProp_Station, _T("SUBSARR_TLM"), str, fname);
		}
		if (m_bCmd)
		{
			// CMDサブキャリア入力有効時に保存
			str.Format(_T("%7.4lf"), mOpeProp.init.cmd);
			WritePrivateProfileString(mOpeProp_Station, _T("SUBSARR_CMD"), str, fname);
		}
	}

	return ret;
}

/*============================================================================*/
/*! 運用プロパティ

-# 運用プロパティのうるう秒文字列に対する番号の取得

@param
@retval

*/
/*============================================================================*/
int CTimeLineView::_GetLeapSecNo(CString& buf)
{
	for (int i = 0; i < sizeof(mLeapSecString) / sizeof(mLeapSecString[0]); i++){
		if (buf.CompareNoCase(CString(mLeapSecString[i])) == 0){
			return i;
		}
	}
	return -1;
}

/*============================================================================*/
/*! 運用プロパティ

-# 運用プロパティのうるう秒番号に対する文字列の取得

@param
@retval

*/
/*============================================================================*/
CString CTimeLineView::_GetLeapSecString(int val)
{
	CString str = mLeapSecString[val];
	return str;
}

/*============================================================================*/
/*! タイムライン

-# 現在時刻の設定

@brief 現在時刻（UTC）を設定する

@param
@retval なし
*/
/*============================================================================*/
void CTimeLineView::setCurrentUtcTime()
{
	UpdateData(TRUE);

	// 現在時刻の描画
	CTime time = mCurrentTime;
	CString strDate, strTime;
	strDate = time.Format(_T("%Y-%m-%d(%j)"));
	strTime = time.Format(_T(" %H:%M:%S"));

	m_strCurrentTime = strDate + strTime;

	mGraph.SetCurrentTime(time);
	mGraphHeader.SetCurrentTime(time);

	UpdateData(FALSE);
}

/*============================================================================*/
/*! タイムライン

-# アンテナ予報値取得

@brief アンテナ予報値の開始終了日時を取得する

@param  start		開始日時
@param  end			終了日時
@retval なし
*/
/*============================================================================*/
void CTimeLineView::getAntForecastTime(CTime &start, CTime &end)
{
	// ダミー
	// アンテナ予報値AOSを現在時刻、LOSを現在時刻から12間後とする。
	CTime now, after;
	CTimeSpan span(0, OperationTime_Forecast, 0, 0);

	now = CTime::GetCurrentTime();
	after = now + span;

	start = now;
	end = after;
}

/*============================================================================*/
/*! タイムライン

-# リサイズ時のコントロール情報を設定する

@param	id		コントロールID
@param	anchore	リサイズ時の識別子

@retval void
*/
/*============================================================================*/
void CTimeLineView::setControlInfo(WORD id, WORD anchore)
{
	if (anchore == ANCHORE_LEFTTOP)
		return;

	// Add resizing behaviour for the control
	DWORD info = id | (anchore << 16);
	mControl.push_back(info);
}

void CTimeLineView::OnSize(UINT nType, int cx, int cy)
{
	static bool mChildMinSize = false;
	CFormView::OnSize(nType, cx, cy);

	if (nType == SIZE_MINIMIZED)
		return;

	if (GetSafeHwnd() == NULL)
		return;

	if (mControl.size() == 0)
		return;

	if (mSaveCx == 0){
		mSaveCx = cx;
		mSaveCy = cy;
		return;
	}

	int dx = cx - mSaveCx;
	int dy = cy - mSaveCy;

	CRect rect, rectClient;
	GetClientRect(rectClient);

	CWnd *pWnd;
	DWORD info;
	WORD anchore;

	vector<DWORD>::iterator itr;
	for (itr = mControl.begin(); itr != mControl.end(); itr++){
		info = (*itr);
		pWnd = GetDlgItem(LOWORD(info));
		if (!pWnd){
			continue;
		}

		if (!HIWORD(info))
			continue;

		anchore = HIWORD(info);
		pWnd->GetWindowRect(rect);
		ScreenToClient(rect);

		if (anchore & RESIZE_HOR)
			rect.right += dx;
		else if (anchore & ANCHORE_RIGHT)
			rect.OffsetRect(dx, 0);

		if (anchore & RESIZE_VER)
			rect.bottom += dy;
		else if (anchore & ANCHORE_BOTTOM)
			rect.OffsetRect(0, dy);

		if (LOWORD(info) == IDC_STATIC_TIMELINEHEADER){
			rect.left = 10;
			rect.right = rectClient.right - 10;
			int timeWidth = (rect.Width() - _FIXEDLABEL_WIDTH) / (MAX_TIMEDIV - 1);
			rect.right = rect.left + _FIXEDLABEL_WIDTH + (timeWidth*(MAX_TIMEDIV - 1));
			if (mGraphHeader.GetSafeHwnd() != NULL)
				mGraphHeader.MoveWindow(rect);
		}
		if (LOWORD(info) == IDC_STATIC_TIMELINE){
			rect.left = 10;
			rect.right = rectClient.right - 10;
			int timeWidth = (rect.Width() - _FIXEDLABEL_WIDTH) / (MAX_TIMEDIV - 1);
			rect.right = rect.left + _FIXEDLABEL_WIDTH + (timeWidth*(MAX_TIMEDIV - 1));
			if (mGraph.GetSafeHwnd() != NULL)
				mGraph.MoveWindow(rect);
		}

		pWnd->MoveWindow(rect);
	}

	if (mChildMinSize == false){
		mChildMinSize = true;
		CRect rect;
		((CChildFrame*)GetParent())->GetWindowRect(rect);
		((CChildFrame*)GetParent())->SetFrameMaxSize(rect.Width(), rect.Height());
	}

	mSaveCx = cx;
	mSaveCy = cy;
}

void CTimeLineView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1){
		if (m_bModalRun == true)
			return;

		KillTimer(1);

		// 描画更新間隔時間の取得
		theApp.GetDrawUpdateInterval(true);

		// 一覧ファイルの再読み込み
		CString strFilePath;
		HANDLE hFile;
		FILETIME cfTime, afTime, wfTime;

		bool bChangeFile = false;
//		bool bChangePropFile = false;

		bChangeFile = GetKanshiData();

		strFilePath = theApp.GetShareFilePath(eFileType_PredList_File, theApp.GetSelectStation());
		// 予報値一覧ファイル更新日時の取得
		hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile != INVALID_HANDLE_VALUE){
			GetFileTime(hFile, &cfTime, &afTime, &wfTime);
			CloseHandle(hFile);
			CTime time(wfTime);
			// 時間が更新されているか、監視が更新されているか調べる
			if (time != mGetSharePredFileTime || bChangeFile == true){
				// ファイルタイムに変化あり
				if (SetPredList_Euc() == TRUE)
				{
					mSelectForecast = -1;
					mGetSharePredFileTime = time;
					bChangeFile = true;
					mGraph.ResetScroll();
					mGraphHeader.ResetScroll();
					//=====================================================//
					//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
					CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("OnTimer"), _T("Change Pred List"), _T(""), nLogEx::info);
					//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
					//=====================================================//
				}
				else
				{
					mGetSharePredFileTime = 0;
					//=====================================================//
					//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
					CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("OnTimer"), _T("Pred List File open error"), _T(""), nLogEx::debug);
					//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
					//=====================================================//
				}
			}
		}
		else
		{
			mGetSharePredFileTime = 0;
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("OnTimer"), _T("Pred List File is invalidate handle"), _T(""), nLogEx::debug);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
		}

		strFilePath = theApp.GetShareFilePath(eFileType_PlanList_File, theApp.GetSelectStation());
		// 計画一覧ファイル更新日時の取得
		hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile != INVALID_HANDLE_VALUE){
			GetFileTime(hFile, &cfTime, &afTime, &wfTime);
			CloseHandle(hFile);
			CTime time(wfTime);
			// 時間が更新されているか、監視が更新されているか調べる
			if (time != mGetSharePlanFileTime || bChangeFile == true){
				// ファイルタイムに変化あり
				if (SetPlanList_Euc() == TRUE)
				{
					mGetSharePlanFileTime = time;
					bChangeFile = true;
					//=====================================================//
					//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
					CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("OnTimer"), _T("Change Plan List"), _T(""), nLogEx::info);
					//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
					//=====================================================//
				}
				else
				{
					mGetSharePlanFileTime = 0;
					//=====================================================//
					//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
					CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("OnTimer"), _T("Plan List File open error"), _T(""), nLogEx::debug);
					//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
					//=====================================================//
				}
			}
		}
		else
		{
			mGetSharePlanFileTime = 0;
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("OnTimer"), _T("Plan List File is invalidate handle"), _T(""), nLogEx::debug);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
		}

		// 計画一覧から計画IDに該当する計画ファイルパス取得
		CString strPlanFilePath = _T("");
		CString strPlanPath = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
		vector<stPlanListData>::iterator itr;
		// 先に局立案優先で局、衛星、計画IDが一致を調べる
		for (itr = mGraph.mListPlanData.begin(); itr != mGraph.mListPlanData.end(); itr++){
			// 局、衛星、計画ID、ENPではない、が一致
			if ((*itr).strStationName.CompareNoCase(mStationString[theApp.GetSelectStation()]) == 0 &&
				(*itr).strSatelliteName.CompareNoCase(mSataName) == 0 &&
				(*itr).strPassId.CompareNoCase(mPlanId) == 0 &&
				(*itr).strGetFrom.CompareNoCase(_T("ENP")) != 0){
				strPlanFilePath.Format(_T("%ssttn\\%s\\%s.pln"), (LPCTSTR)strPlanPath, (LPCTSTR)mSataName, (LPCTSTR)mPlanId);
				break;
			}
		}
		if (strPlanFilePath.IsEmpty()) {
			// 局立案検索後にENPで局、衛星、計画IDが一致を調べる
			for (itr = mGraph.mListPlanData.begin(); itr != mGraph.mListPlanData.end(); itr++){
				// 局、衛星、計画ID、ENPが一致
				if ((*itr).strStationName.CompareNoCase(mStationString[theApp.GetSelectStation()]) == 0 &&
					(*itr).strSatelliteName.CompareNoCase(mSataName) == 0 &&
					(*itr).strPassId.CompareNoCase(mPlanId) == 0 &&
					(*itr).strGetFrom.CompareNoCase(_T("ENP")) == 0){
					strPlanFilePath.Format(_T("%senp\\%s\\%s.pln"), (LPCTSTR)strPlanPath, (LPCTSTR)mSataName, (LPCTSTR)mPlanId);
					break;
				}
			}
		}

		if (mStrPlanFilePath.CompareNoCase(strPlanFilePath) != 0)
		{
			// 計画ファイルパスが一致しない場合
			mStrPlanFilePath = strPlanFilePath;
			bChangeFile = true;
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("OnTimer"), _T("Change Plan file path"), _T(""), nLogEx::info);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
		}
		hFile = CreateFile(mStrPlanFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile != INVALID_HANDLE_VALUE){
			GetFileTime(hFile, &cfTime, &afTime, &wfTime);
			CloseHandle(hFile);
			CTime time(wfTime);
			// 時間が更新されているか、監視が更新されているか調べる
			if (time != mGetSharePlanedFileTime || bChangeFile == true){
				// ファイルタイムに変化あり
				mGetSharePlanedFileTime = time;
				bChangeFile = true;
				//=====================================================//
				//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
				CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("OnTimer"), _T("Change Plan"), _T(""), nLogEx::info);
				//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
				//=====================================================//
			}
		}
		else
		{
			mGetSharePlanedFileTime = 0;
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("OnTimer"), _T("Planed File is invalidate handle"), _T(""), nLogEx::debug);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
		}

		CGraphMutex::Lock(ePassInfo);

		if (bChangeFile == true)
		{
			// 計画一覧に変化があるので、計画ファイルを取得する
			if (GetPlanDataFromFile() == FALSE)
			{
				mGetSharePlanedFileTime = 0;
				//=====================================================//
				//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
				CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("OnTimer"), _T("Planed File can not open"), _T(""), nLogEx::debug);
				//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
				//=====================================================//
			}
		}

		bool ret = GetPredMonitorFile();
		if (ret)
		{
			bChangeFile = true;
		}

		UpdateMonitorData();
		(void)UpdateTimelineData(mSelectForecast, !(bChangeFile));

//		if (bChangePropFile == true){
//			UpdateInitializePropertyData();
//			UpdateOperationProperty();
//		}
		if (bChangeFile == true)
		{
			UpdateOperationOption();
		}

		CGraphMutex::Unlock(ePassInfo);

		GetCurrentUtcTime(mCurrentTime);
		setCurrentUtcTime();
		mGraph.UpdateTime();
		mGraphHeader.UpdateTime();

		SetTimer(1, 1000, NULL);

		return;
	}

	CFormView::OnTimer(nIDEvent);
}

/*============================================================================*/
/*! タイムライン

-# 現在時刻の設定

@brief 現在時刻（UTC）を取得する

@param
@retval なし
*/
/*============================================================================*/
void CTimeLineView::GetCurrentUtcTime(CTime& time)
{
#ifdef _CUTNOCOMM
	static bool bFirst = true;
	static SYSTEMTIME tmpUTC;
	if (bFirst == false){
		time = CTime(tmpUTC);
		CTimeSpan sp = CTimeSpan(0, 0, 0, mTimeRate);
		time += sp;
		time.GetAsSystemTime(tmpUTC);
		time = CTime(tmpUTC);
		return;
	}
	GetSystemTime(&tmpUTC);
	bFirst = false;
#endif

//#ifdef _DEBUG
//	static bool bFirst = true;
//	static CTime timeFirst;
//	if (bFirst)
//	{
////		timeFirst = CTime(2017, 10, 25, 11, 30, 0);
////		timeFirst = CTime(2017, 11, 01, 5, 1, 0);
////		timeFirst = CTime(2017, 12, 5, 5, 0, 2);
//		timeFirst = CTime(2018, 12, 24, 0, 0, 0);
//		time = timeFirst;
//		bFirst = false;
//	}
//	else
//	{
//		if (time.GetTime() <= 0)
//		{
//			time = timeFirst;
//		}
//		CTimeSpan sp = CTimeSpan(0, 0, 0, 1);
//		time += sp;
//	}
//
//#else
	// 現在時刻の描画
	SYSTEMTIME st_UTC;
	GetSystemTime(&st_UTC);
	time = CTime(st_UTC);


//#endif
}

/*============================================================================*/
/*! 運用計画タイムライン

-# タイムラインデータの設定

@param		data		タイムラインデータ
@retval

*/
/*============================================================================*/
void CTimeLineView::SetTimeLineData(COprPlnData data, UINT nForecastNo)
{
	CTime fstart, fend;
	getAntForecastTime(fstart, fend);

	mGraph.SetForecastTime(fstart, fend, nForecastNo);
	mGraph.SetTimeLineData(data);

	mGraphHeader.SetForecastTime(fstart, fend, nForecastNo);
	mGraphHeader.SetTimeLineData(data);
}

/*============================================================================*/
/*! 運用計画タイムライン

-# タイムラインデータのクリア

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::ClearTimeLineData()
{
	//###
	mGraph.ClearTimeLineData();
	mGraphHeader.ClearTimeLineData();
}


BOOL CTimeLineView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CFormView::PreCreateWindow(cs);
}


BOOL CTimeLineView::OnEraseBkgnd(CDC* pDC)
{
	return CFormView::OnEraseBkgnd(pDC);
}

/*============================================================================*/
/*! 運用計画タイムライン

-# 運用計画情報（予報値切替）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonForecast()
{
	if (m_strSpaceName.Left(1) == _T("-") || m_strSpaceName.IsEmpty()){
		return;
	}

	if (theApp.GetCallWnd() != NULL){
		TCHAR *p = new TCHAR[80];
		swprintf_s(p, 80, _T("%s"), (LPCTSTR)m_strSpaceName);
		//swprintf_s(p, 80, _T("---"));
		mSelectForecastName.Empty();
		mGetSharePredFileTime = 0;
		(theApp.m_pMainWnd)->SendMessage(eTIMELINE_GET_PRED, 80, (LPARAM)p);
		delete[] p;
		return;
	}

	CSelectForecast dlg;
	theApp.SetDlgMap(_T("CSelectForecast"), (CWnd*)&dlg);
	dlg.mForecastList.clear();
	vector<stPredListData>::iterator itr;
	for (itr = mGraph.mListPredData.begin(); itr != mGraph.mListPredData.end(); itr++){
		dlg.mForecastList.push_back((*itr).strPassId);
	}
	dlg.mCurrentItem = mSelectForecast;
	if (dlg.DoModal() == IDCANCEL){
		theApp.DeleteDlgMap(_T("CSelectForecast"));
		return;
	}
	theApp.DeleteDlgMap(_T("CSelectForecast"));
	mSelectForecast = dlg.mCurrentItem;

	CString satellite;
	CString predfile;
	satellite = mGraph.mListPredData[mSelectForecast].strSatellite;
	predfile = ConvertPredfileToFname(mGraph.mListPredData[mSelectForecast].strPredPath);
	/****************************************************************/
	/* 制御情報 */
	CString ctrl = _T("");
	ctrl.Format(_T("CTRL.PRED_CHANGE %s@%s"), (LPCTSTR)satellite, (LPCTSTR)predfile);
	theApp.SendControl(ctrl, _T(""), _T(""), _T(""));

	mSelectForecastName.Empty();
	mGetSharePredFileTime = 0;

	UpdateTimelineData(mSelectForecast);
//	UpdateInitializePropertyData();
//	UpdateOperationProperty();
	UpdateOperationOption();
	UpdateMonitorData();
}
/*============================================================================*/
/*! 運用計画タイムライン

-# 運用計画情報（運用終了日時変更）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonOpestop()
{
	CUpdateOpEnd dlg;
	theApp.SetDlgMap(_T("CUpdateOpEnd"), (CWnd*)&dlg);
	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];
	dlg.mDefTime = tdata.m_OpetDay.end;
	dlg.mUpdateTime = tdata.m_OpetDay.end;
	CTime newEndTime = tdata.m_OpetDay.end;
	while (true)
	{
		if (dlg.DoModal() == IDCANCEL)
		{
			theApp.DeleteDlgMap(_T("CUpdateOpEnd"));
			return;
		}

		if (dlg.mUpdateTime.GetTime() < tdata.m_OpetDay.start.GetTime())
		{
			AfxMessageBox(_T("終了日時が開始日時より過去です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			// 終了時刻変更
			newEndTime = dlg.mUpdateTime;
			break;
		}
	}
	theApp.DeleteDlgMap(_T("CUpdateOpEnd"));
	CString strOpeStop = (newEndTime).Format(_T("%Y-%m-%d %H:%M:%S"));
//	UpdateData(FALSE);

	// 運用終了日時の変更通知を送る
	// 計画変更要求
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = SEND_NOTICE_SH;
	CString arg;

	int nStation = theApp.GetSelectStation();
	arg.Format(_T("%s %d %d,%s"),
		(LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(),	// 局
		INFO_ID_REQ_PLN_MO,												// 計画変更要求
		2,																// 終了時刻変更：2
		(LPCTSTR)strOpeStop);											// 時刻

	stParam.strRemShellParam.Format(arg);

	int err = CSshShell::ExecShell(stParam);							// シェル待ち
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("OnBnClickedButtonOpestop"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err != 0) // 異常終了
	{
		// 何もしない
	}
	else // 正常終了
	{
		// 計画更新通知を送る
		tdata.m_OpetDay.end = newEndTime;
		m_strOpeStop = strOpeStop;
		UpdateData(FALSE);
		stPlandata pdata;
		UpdatePlanData(ePlanPEnd, pdata);

		mGraph.SetTimeLineData(tdata);
		mGraphHeader.SetTimeLineData(tdata);
//#ifdef _DEBUG
//	mbEndTimeChged = TRUE;
//	mbPlanIDChged = TRUE;
//#endif
	}
}
/*============================================================================*/
/*! 運用計画タイムライン

-# アップリンク（イベント切替）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonUplnk()
{
	UpdateData(TRUE);
	CSelectEventMode dlg;
	theApp.SetDlgMap(_T("CSelectEventMode"), (CWnd*)&dlg);
	dlg.m_nEventMode = m_nUplnkEvent;
	if (dlg.DoModal() == IDCANCEL)
	{
		theApp.DeleteDlgMap(_T("CSelectEventMode"));
		return;
	}
	theApp.DeleteDlgMap(_T("CSelectEventMode"));

	int nUplnkEvent = dlg.m_nEventMode;

	if (nUplnkEvent != m_nUplnkEvent){
		unsigned char uplink = 0;

		if (nUplnkEvent == 1)	// 手動実行
		{
			// アップリンクが手動
			uplink = 0;
		}
		else
		{
			// アップリンクが自動
			uplink = 1;
		}

		// 衛星捕捉モード設定要求の通知を送る
		// ダウンリンク, CMD, RNG, WAY制御取得
		unsigned char cmd = GetKanshiCmdMode();
		unsigned char rng = GetKanshiRngMode();
		unsigned char downlink = GetKanshiDownLinkMode();
		unsigned char way = GetKanshiWayMode();
		unsigned char binary;
		binary = way << eSatellieMode_Way |						/* 1Way/2Way/Dup設定 */
			uplink << eSatellieMode_UpLink |					/* アップリンク制御設定 */
			downlink << eSatellieMode_DownLink |				/* ダウンリンク制御設定 */
			rng << eSatellieMode_RNG |							/* RNG制御設定 */
			cmd << eSatellieMode_CMD |							/* CMD制御設定 */
			1 << eSatellieMode_AntStandBy |						/* ANT待ち受け指向種別 */
			1 << eSatellieMode_AutoManual;						/* 自動捕捉/手動捕捉種別 */

		SendSatelliteCaptureModeNotify(binary);

//#ifdef _DEBUG
//		m_nUplnkEvent = nUplnkEvent;
//#endif

		m_UplinkEventStatic.Invalidate();
		UpdateData(FALSE);
	}
}
/*============================================================================*/
/*! 運用計画タイムライン

-# アップリンク（実施情報追加）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonUplnkadd()
{
	CEventAdd dlg;
	theApp.SetDlgMap(_T("CEventAdd"), (CWnd*)&dlg);
	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];
	dlg.mMode = eEventAddModeUplink;

#if 1
	dlg.mStartTime = tdata.m_OpetDay.start;
	dlg.mEndTime = tdata.m_OpetDay.end;

	int curpos = m_cbUplnkNo.GetCurSel();

	if(curpos < tdata.m_ULDay.size())
	{
		dlg.mStartTime = tdata.m_ULDay[curpos].end;
	}
	if(curpos + 1 < tdata.m_ULDay.size())
	{
		dlg.mEndTime = tdata.m_ULDay[curpos + 1].start;
	}

	CTime cSTime, cETime;

	while (1)
	{
		if (dlg.DoModal() == IDCANCEL)
		{
			theApp.DeleteDlgMap(_T("CEventAdd"));
			return;
		}
		theApp.DeleteDlgMap(_T("CEventAdd"));

		cSTime = dlg.mStartTime;
		cETime= dlg.mEndTime;

		if ((tdata.m_OpetDay.start <= cSTime && tdata.m_OpetDay.end >= cSTime)
			&& (tdata.m_OpetDay.start <= cETime && tdata.m_OpetDay.end >= cETime))
		{
			BOOL bConflict = FALSE;
			for (int i = 0; i < tdata.m_ULDay.size(); i++)
			{
				if (((tdata.m_ULDay[i].start <= cSTime) && (tdata.m_ULDay[i].end > cSTime))
					|| ((tdata.m_ULDay[i].start < cETime) && (tdata.m_ULDay[i].end >= cETime)))
				{
					bConflict = TRUE;
					break;
				}
				// 新イベント開始時刻と終了時刻の間に、既存イベント開始時刻と終了時刻が含まれるか確認
				if (cSTime <= tdata.m_ULDay[i].start && cETime >= tdata.m_ULDay[i].end)
				{
					bConflict = TRUE;
					break;
				}
			}
			if (bConflict)
			{
				AfxMessageBox(_T("アップリンク の開始/終了日時が重複しています。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				break;
			}
		}
		else
		{
			AfxMessageBox(_T("アップリンク の開始/終了日時が、運用開始/終了日時の範囲外です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
	}
#else
	if (tdata.m_ULDay.size() == 0){
		SYSTEMTIME time;
		GetSystemTime(&time);
		dlg.mStartTime = CTime(time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, 0);
		dlg.mEndTime = CTime(time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, 0);
	}
	else{
		dlg.mStartTime = tdata.m_ULDay[0].start;
		dlg.mEndTime = tdata.m_ULDay[0].end;
	}
#endif

	stTime data;
	data.start = dlg.mStartTime;
	data.end = dlg.mEndTime;

	if (curpos == -1 || tdata.m_ULDay.size() == 0)
	{
		curpos = -1;
		tdata.m_ULDay.insert(tdata.m_ULDay.begin(), data);
	}
	else
	{
		tdata.m_ULDay.insert(tdata.m_ULDay.begin() + curpos + 1, data);
	}
//	tdata.m_ULDay.push_back(data);

	tdata.m_UpLinkCnt = (int)tdata.m_ULDay.size();
	if (tdata.m_UpLinkCnt > 0)
		tdata.m_UpLinkChk = TRUE;
	else
		tdata.m_UpLinkChk = FALSE;

	int pos = curpos + 1;
//	int pos = tdata.m_UpLinkCnt - 1;
	m_strUplnkStart = (tdata.m_ULDay[pos].start).Format(_T("%Y-%m-%d %H:%M:%S"));
	m_strUplnkStop = (tdata.m_ULDay[pos].end).Format(_T("%Y-%m-%d %H:%M:%S"));
	m_cbUplnkNo.ResetContent();
	for (int i = 0; i < tdata.m_ULDay.size(); i++){
		CString str;
		str.Format(_T("%d"), i + 1);
		m_cbUplnkNo.AddString(str);
	}
	m_nUplnkNo = pos;
	m_cbUplnkNo.SetCurSel(m_nUplnkNo);
	UpdateData(FALSE);

	stPlandata pdata;
	UpdatePlanData(ePlanPStart, pdata);

	mGraph.SetSelectUplink(m_nUplnkNo);
	mGraphHeader.SetSelectUplink(m_nUplnkNo);
	mGraph.SetTimeLineData(tdata);
	mGraphHeader.SetTimeLineData(tdata);
}
/*============================================================================*/
/*! 運用計画タイムライン

-# アップリンク（実施情報変更）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonUplnkupdate()
{
	CEventUpdate dlg;
	theApp.SetDlgMap(_T("CEventUpdate"), (CWnd*)&dlg);
	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];
	dlg.mMode = eEventUpdateModeUplink;
	int curpos = m_cbUplnkNo.GetCurSel();
	if (tdata.m_ULDay.size() == 0){
		CString str;
		if (str.LoadString(IDS_ERROR_EVENT_NONE))
		{
			MessageBox(str);
		}
		return;
	}
	else{
		dlg.mDefStartTime = tdata.m_ULDay[curpos].start;
		dlg.mDefEndTime = tdata.m_ULDay[curpos].end;
		dlg.mStartTime = tdata.m_ULDay[curpos].start;
		dlg.mEndTime = tdata.m_ULDay[curpos].end;
	}
#if 1
	CTime cSTime, cETime;
	while (1)
	{
		if (dlg.DoModal() == IDCANCEL)
		{
			theApp.DeleteDlgMap(_T("CEventUpdate"));
			return;
		}
		theApp.DeleteDlgMap(_T("CEventUpdate"));

		cSTime = dlg.mStartTime;
		cETime= dlg.mEndTime;

		if ((tdata.m_OpetDay.start <= cSTime && tdata.m_OpetDay.end >= cSTime)
			&& (tdata.m_OpetDay.start <= cETime && tdata.m_OpetDay.end >= cETime))
		{
			BOOL bConflict = FALSE;
			for (int i = 0; i < tdata.m_ULDay.size(); i++)
			{
				if (i == curpos)
					continue;

				if (((tdata.m_ULDay[i].start <= cSTime) && (tdata.m_ULDay[i].end > cSTime))
					|| ((tdata.m_ULDay[i].start < cETime) && (tdata.m_ULDay[i].end >= cETime)))
				{
					bConflict = TRUE;
					break;
				}
				// 新イベント開始時刻と終了時刻の間に、既存イベント開始時刻と終了時刻が含まれるか確認
				if (cSTime <= tdata.m_ULDay[i].start && cETime >= tdata.m_ULDay[i].end)
				{
					bConflict = TRUE;
					break;
				}
			}
			if (bConflict)
			{
				AfxMessageBox(_T("アップリンク の開始/終了日時が重複しています。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				break;
			}
		}
		else
		{
			AfxMessageBox(_T("アップリンク の開始/終了日時が、運用開始/終了日時の範囲外です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
	}
#else
	if (dlg.DoModal() == IDCANCEL)
		return;
#endif

	tdata.m_UpLinkCnt = (int)tdata.m_ULDay.size();
	if (tdata.m_UpLinkCnt > 0)
		tdata.m_UpLinkChk = TRUE;
	else
		tdata.m_UpLinkChk = FALSE;

	tdata.m_ULDay[curpos].start = dlg.mStartTime;
	tdata.m_ULDay[curpos].end = dlg.mEndTime;
	m_strUplnkStart = (tdata.m_ULDay[curpos].start).Format(_T("%Y-%m-%d %H:%M:%S"));
	m_strUplnkStop = (tdata.m_ULDay[curpos].end).Format(_T("%Y-%m-%d %H:%M:%S"));
	m_cbUplnkNo.ResetContent();
	for (int i = 0; i < tdata.m_ULDay.size(); i++){
		CString str;
		str.Format(_T("%d"), i + 1);
		m_cbUplnkNo.AddString(str);
	}
	m_nUplnkNo = curpos;
	m_cbUplnkNo.SetCurSel(m_nUplnkNo);
	UpdateData(FALSE);

	stPlandata pdata;
	UpdatePlanData(ePlanPStart, pdata);

	mGraph.SetSelectUplink(m_nUplnkNo);
	mGraphHeader.SetSelectUplink(m_nUplnkNo);
	mGraph.SetTimeLineData(tdata);
	mGraphHeader.SetTimeLineData(tdata);
}
/*============================================================================*/
/*! 運用計画タイムライン

-# アップリンク（実施情報削除）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonUplnkdel()
{
	CEventDel dlg;
	theApp.SetDlgMap(_T("CEventDel"), (CWnd*)&dlg);
	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];
	dlg.mMode = eEventDelModeUplink;
	if (tdata.m_ULDay.size() <= 0){
		CString str;
		if (str.LoadString(IDS_ERROR_EVENT_DELETE))
		{
			MessageBox(str);
		}
		return;
	}
	dlg.mTargetNo = m_cbUplnkNo.GetCurSel();
	if (dlg.DoModal() == IDCANCEL)
	{
		theApp.DeleteDlgMap(_T("CEventDel"));
		return;
	}
	theApp.DeleteDlgMap(_T("CEventDel"));

	vector<stTime>::iterator itr;
	int pos = 0;
	for (itr = tdata.m_ULDay.begin(); itr != tdata.m_ULDay.end(); itr++){
		if (dlg.mTargetNo == pos){
			tdata.m_ULDay.erase(itr);
			break;
		}
		pos++;
	}

	tdata.m_UpLinkCnt = (int)tdata.m_ULDay.size();
	if (tdata.m_UpLinkCnt > 0)
		tdata.m_UpLinkChk = TRUE;
	else
		tdata.m_UpLinkChk = FALSE;

	m_cbUplnkNo.ResetContent();
	for (int i = 0; i < tdata.m_ULDay.size(); i++){
		CString str;
		str.Format(_T("%d"), i + 1);
		m_cbUplnkNo.AddString(str);
	}
	m_nUplnkNo = 0;
	m_cbUplnkNo.SetCurSel(m_nUplnkNo);
	if (tdata.m_ULDay.size() == 0){
		m_strUplnkStart = _T("-------------------");
		m_strUplnkStop = _T("-------------------");
	}
	else{
		m_strUplnkStart = (tdata.m_ULDay[m_nUplnkNo].start).Format(_T("%Y-%m-%d %H:%M:%S"));
		m_strUplnkStop = (tdata.m_ULDay[m_nUplnkNo].end).Format(_T("%Y-%m-%d %H:%M:%S"));
	}
	UpdateData(FALSE);

	stPlandata pdata;
	UpdatePlanData(ePlanPStart, pdata);

	mGraph.SetSelectUplink(m_nUplnkNo);
	mGraphHeader.SetSelectUplink(m_nUplnkNo);
	mGraph.SetTimeLineData(tdata);
	mGraphHeader.SetTimeLineData(tdata);
}
/*============================================================================*/
/*! 運用計画タイムライン

-# アップリンク（実施番号変更）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnCbnSelchangeComboUplnkno()
{
	UpdateData(TRUE);
	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];
	m_nUplnkNo = m_cbUplnkNo.GetCurSel();
	m_strUplnkStart = (tdata.m_ULDay[m_nUplnkNo].start).Format(_T("%Y-%m-%d %H:%M:%S"));
	m_strUplnkStop = (tdata.m_ULDay[m_nUplnkNo].end).Format(_T("%Y-%m-%d %H:%M:%S"));
	UpdateData(FALSE);

	mGraph.SetSelectUplink(m_nUplnkNo);
	mGraphHeader.SetSelectUplink(m_nUplnkNo);
	mGraph.SetTimeLineData(tdata);
	mGraphHeader.SetTimeLineData(tdata);
}

/*============================================================================*/
/*! 運用計画タイムライン

-# CMD（イベント切替）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonCmd()
{
	UpdateData(TRUE);
	CSelectEventMode dlg;
	theApp.SetDlgMap(_T("CSelectEventMode"), (CWnd*)&dlg);
	dlg.m_nEventMode = m_nCmdEvent;
	if (dlg.DoModal() == IDCANCEL)
	{
		theApp.DeleteDlgMap(_T("CSelectEventMode"));
		return;
	}
	theApp.DeleteDlgMap(_T("CSelectEventMode"));

	int nCmdEvent = dlg.m_nEventMode;

	if (nCmdEvent != m_nCmdEvent){
		unsigned char cmd = 0;

		if (nCmdEvent == 1)	// 手動実行
		{
			// CMDが手動
			cmd = 0;
		}
		else
		{
			// CMDが自動
			cmd = 1;
		}

		// 衛星捕捉モード設定要求の通知を送る
		// アップリンク, ダウンリンク, RNG, WAY制御取得
		unsigned char rng = GetKanshiRngMode();
		unsigned char downlink = GetKanshiDownLinkMode();
		unsigned char uplink = GetKanshiUpLinkMode();
		unsigned char way = GetKanshiWayMode();
		unsigned char binary;
		binary = way << eSatellieMode_Way |						/* 1Way/2Way/Dup設定 */
			uplink << eSatellieMode_UpLink |					/* アップリンク制御設定 */
			downlink << eSatellieMode_DownLink |				/* ダウンリンク制御設定 */
			rng << eSatellieMode_RNG |							/* RNG制御設定 */
			cmd << eSatellieMode_CMD |							/* CMD制御設定 */
			1 << eSatellieMode_AntStandBy |						/* ANT待ち受け指向種別 */
			1 << eSatellieMode_AutoManual;						/* 自動捕捉/手動捕捉種別 */

		SendSatelliteCaptureModeNotify(binary);

//#ifdef _DEBUG
//		m_nCmdEvent = nCmdEvent;
//#endif

		m_CmdEventStatic.Invalidate();
		UpdateData(FALSE);
	}
}
/*============================================================================*/
/*! 運用計画タイムライン

-# CMD（実施情報追加）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonCmdadd()
{
	CEventAdd dlg;
	theApp.SetDlgMap(_T("CEventAdd"), (CWnd*)&dlg);
	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];
	dlg.mMode = eEventAddModeCmd;
#if 1
	dlg.mStartTime = tdata.m_OpetDay.start;
	dlg.mEndTime = tdata.m_OpetDay.end;

	int curpos = m_cbCmdNo.GetCurSel();

	if (curpos < tdata.m_CmdDayList.size())
	{
		dlg.mStartTime = tdata.m_CmdDayList[curpos].end;
	}
	if (curpos + 1 < tdata.m_CmdDayList.size())
	{
		dlg.mEndTime = tdata.m_CmdDayList[curpos + 1].start;
	}

	CTime cSTime, cETime;

	while (1)
	{
		if (dlg.DoModal() == IDCANCEL)
		{
			theApp.DeleteDlgMap(_T("CEventAdd"));
			return;
		}
		theApp.DeleteDlgMap(_T("CEventAdd"));

		cSTime = dlg.mStartTime;
		cETime = dlg.mEndTime;

		if ((tdata.m_OpetDay.start <= cSTime && tdata.m_OpetDay.end >= cSTime)
			&& (tdata.m_OpetDay.start <= cETime && tdata.m_OpetDay.end >= cETime))
		{
			BOOL bConflict = FALSE;
			for (int i = 0; i < tdata.m_CmdDayList.size(); i++)
			{
				if (((tdata.m_CmdDayList[i].start <= cSTime) && (tdata.m_CmdDayList[i].end > cSTime))
					|| ((tdata.m_CmdDayList[i].start < cETime) && (tdata.m_CmdDayList[i].end >= cETime)))
				{
					bConflict = TRUE;
					break;
				}
				// 新イベント開始時刻と終了時刻の間に、既存イベント開始時刻と終了時刻が含まれるか確認
				if (cSTime <= tdata.m_CmdDayList[i].start && cETime >= tdata.m_CmdDayList[i].end)
				{
					bConflict = TRUE;
					break;
				}
			}

			if (bConflict)
			{
				AfxMessageBox(_T("CMD の開始/終了日時が重複しています。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				break;
			}
		}
		else
		{
			AfxMessageBox(_T("CMD の開始/終了日時が、運用開始/終了日時の範囲外です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
	}
#else
	dlg.mMode = eEventAddModeCmd;
	if (tdata.m_CmdDayList.size() == 0){
		SYSTEMTIME time;
		GetSystemTime(&time);
		dlg.mStartTime = CTime(time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, 0);
		dlg.mEndTime = CTime(time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, 0);
	}
	else{
		dlg.mStartTime = tdata.m_CmdDayList[0].start;
		dlg.mEndTime = tdata.m_CmdDayList[0].end;
	}
	if (dlg.DoModal() == IDCANCEL)
		return;
#endif

	stTime data;
	data.start = dlg.mStartTime;
	data.end = dlg.mEndTime;
//	tdata.m_CmdDayList.push_back(data);
	if (curpos == -1 || tdata.m_CmdDayList.size() == 0)
	{
		curpos = -1;
		tdata.m_CmdDayList.insert(tdata.m_CmdDayList.begin(), data);
	}
	else
	{
		tdata.m_CmdDayList.insert(tdata.m_CmdDayList.begin() + curpos + 1, data);
	}

	tdata.m_CmdCnt = (int)tdata.m_CmdDayList.size();
	if (tdata.m_CmdCnt > 0)
		tdata.m_CmdChk = TRUE;
	else
		tdata.m_CmdChk = FALSE;

//	int pos = tdata.m_CmdCnt - 1;
	int pos = curpos + 1;
	m_strCmdStart = (tdata.m_CmdDayList[pos].start).Format(_T("%Y-%m-%d %H:%M:%S"));
	m_strCmdStop = (tdata.m_CmdDayList[pos].end).Format(_T("%Y-%m-%d %H:%M:%S"));
	m_cbCmdNo.ResetContent();
	for (int i = 0; i < tdata.m_CmdDayList.size(); i++){
		CString str;
		str.Format(_T("%d"), i + 1);
		m_cbCmdNo.AddString(str);
	}
	m_nCmdNo = pos;
	m_cbCmdNo.SetCurSel(m_nCmdNo);
	UpdateData(FALSE);

	stPlandata pdata;
	UpdatePlanData(ePlanPStart, pdata);

	mGraph.SetSelectCmd(m_nCmdNo);
	mGraphHeader.SetSelectCmd(m_nCmdNo);
	mGraph.SetTimeLineData(tdata);
	mGraphHeader.SetTimeLineData(tdata);
}
/*============================================================================*/
/*! 運用計画タイムライン

-# CMD（実施情報変更）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonCmdupdate()
{
	CEventUpdate dlg;
	theApp.SetDlgMap(_T("CEventUpdate"), (CWnd*)&dlg);
	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];
	dlg.mMode = eEventUpdateModeCmd;
	int curpos = m_cbCmdNo.GetCurSel();
	if (tdata.m_CmdDayList.size() == 0){
		CString str;
		if (str.LoadString(IDS_ERROR_EVENT_NONE))
		{
			MessageBox(str);
		}
		return;
	}
	else{
		dlg.mDefStartTime = tdata.m_CmdDayList[curpos].start;
		dlg.mDefEndTime = tdata.m_CmdDayList[curpos].end;
		dlg.mStartTime = tdata.m_CmdDayList[curpos].start;
		dlg.mEndTime = tdata.m_CmdDayList[curpos].end;
	}
#if 1
	CTime cSTime, cETime;
	while (1)
	{
		if (dlg.DoModal() == IDCANCEL)
		{
			theApp.DeleteDlgMap(_T("CEventUpdate"));
			return;
		}
		theApp.DeleteDlgMap(_T("CEventUpdate"));

		cSTime = dlg.mStartTime;
		cETime = dlg.mEndTime;

		if ((tdata.m_OpetDay.start <= cSTime && tdata.m_OpetDay.end >= cSTime)
			&& (tdata.m_OpetDay.start <= cETime && tdata.m_OpetDay.end >= cETime))
		{
			BOOL bConflict = FALSE;
			for (int i = 0; i < tdata.m_CmdDayList.size(); i++)
			{
				if (i == curpos)
					continue;

				if (((tdata.m_CmdDayList[i].start <= cSTime) && (tdata.m_CmdDayList[i].end > cSTime))
					|| ((tdata.m_CmdDayList[i].start < cETime) && (tdata.m_CmdDayList[i].end >= cETime)))
				{
					bConflict = TRUE;
					break;
				}
				// 新イベント開始時刻と終了時刻の間に、既存イベント開始時刻と終了時刻が含まれるか確認
				if (cSTime <= tdata.m_CmdDayList[i].start && cETime >= tdata.m_CmdDayList[i].end)
				{
					bConflict = TRUE;
					break;
				}
			}
			if (bConflict)
			{
				AfxMessageBox(_T("CMD の開始/終了日時が重複しています。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				break;
			}
		}
		else
		{
			AfxMessageBox(_T("CMD の開始/終了日時が、運用開始/終了日時の範囲外です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
	}
#else
	if (dlg.DoModal() == IDCANCEL)
		return;
#endif

	tdata.m_CmdDayList[curpos].start = dlg.mStartTime;
	tdata.m_CmdDayList[curpos].end = dlg.mEndTime;

	tdata.m_CmdCnt = (int)tdata.m_CmdDayList.size();
	if (tdata.m_CmdCnt > 0)
		tdata.m_CmdChk = TRUE;
	else
		tdata.m_CmdChk = FALSE;

	m_strCmdStart = (tdata.m_CmdDayList[curpos].start).Format(_T("%Y-%m-%d %H:%M:%S"));
	m_strCmdStop = (tdata.m_CmdDayList[curpos].end).Format(_T("%Y-%m-%d %H:%M:%S"));
	m_cbCmdNo.ResetContent();
	for (int i = 0; i < tdata.m_CmdDayList.size(); i++){
		CString str;
		str.Format(_T("%d"), i + 1);
		m_cbCmdNo.AddString(str);
	}
	m_nCmdNo = curpos;
	m_cbCmdNo.SetCurSel(m_nCmdNo);
	UpdateData(FALSE);

	stPlandata pdata;
	UpdatePlanData(ePlanPStart, pdata);

	mGraph.SetSelectCmd(m_nCmdNo);
	mGraphHeader.SetSelectCmd(m_nCmdNo);
	mGraph.SetTimeLineData(tdata);
	mGraphHeader.SetTimeLineData(tdata);
}
/*============================================================================*/
/*! 運用計画タイムライン

-# CMD（実施情報削除）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonCmddel()
{
	CEventDel dlg;
	theApp.SetDlgMap(_T("CEventDel"), (CWnd*)&dlg);
	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];
	dlg.mMode = eEventDelModeCmd;
	if (tdata.m_CmdDayList.size() <= 0){
		CString str;
		if (str.LoadString(IDS_ERROR_EVENT_DELETE))
		{
			MessageBox(str);
		}
		return;
	}
	dlg.mTargetNo = m_cbCmdNo.GetCurSel();
	if (dlg.DoModal() == IDCANCEL)
	{
		theApp.DeleteDlgMap(_T("CEventDel"));
		return;
	}
	theApp.DeleteDlgMap(_T("CEventDel"));

	vector<stTime>::iterator itr;
	int pos = 0;
	for (itr = tdata.m_CmdDayList.begin(); itr != tdata.m_CmdDayList.end(); itr++){
		if (dlg.mTargetNo == pos){
			tdata.m_CmdDayList.erase(itr);
			break;
		}
		pos++;
	}

	tdata.m_CmdCnt = (int)tdata.m_CmdDayList.size();
	if (tdata.m_CmdCnt > 0)
		tdata.m_CmdChk = TRUE;
	else
		tdata.m_CmdChk = FALSE;

	m_cbCmdNo.ResetContent();
	for (int i = 0; i < tdata.m_CmdDayList.size(); i++){
		CString str;
		str.Format(_T("%d"), i + 1);
		m_cbCmdNo.AddString(str);
	}
	m_nCmdNo = 0;
	m_cbCmdNo.SetCurSel(m_nCmdNo);
	if (tdata.m_CmdDayList.size() == 0){
		m_strCmdStart = _T("-------------------");
		m_strCmdStop = _T("-------------------");
	}
	else{
		m_strCmdStart = (tdata.m_CmdDayList[m_nCmdNo].start).Format(_T("%Y-%m-%d %H:%M:%S"));
		m_strCmdStop = (tdata.m_CmdDayList[m_nCmdNo].end).Format(_T("%Y-%m-%d %H:%M:%S"));
	}
	UpdateData(FALSE);

	stPlandata pdata;
	UpdatePlanData(ePlanPStart, pdata);

	mGraph.SetSelectCmd(m_nCmdNo);
	mGraphHeader.SetSelectCmd(m_nCmdNo);
	mGraph.SetTimeLineData(tdata);
	mGraphHeader.SetTimeLineData(tdata);
}
/*============================================================================*/
/*! 運用計画タイムライン

-# CMD（実施番号変更）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnCbnSelchangeComboCmdno()
{
	UpdateData(TRUE);
	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];
	m_nCmdNo = m_cbCmdNo.GetCurSel();
	m_strCmdStart = (tdata.m_CmdDayList[m_nCmdNo].start).Format(_T("%Y-%m-%d %H:%M:%S"));
	m_strCmdStop = (tdata.m_CmdDayList[m_nCmdNo].end).Format(_T("%Y-%m-%d %H:%M:%S"));
	UpdateData(FALSE);

	mGraph.SetSelectCmd(m_nCmdNo);
	mGraphHeader.SetSelectCmd(m_nCmdNo);
	mGraph.SetTimeLineData(tdata);
	mGraphHeader.SetTimeLineData(tdata);
}

/*============================================================================*/
/*! 運用計画タイムライン

-# RNG（イベント切替）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonRng()
{
	UpdateData(TRUE);
	CSelectEventMode dlg;
	theApp.SetDlgMap(_T("CSelectEventMode"), (CWnd*)&dlg);
	dlg.m_nEventMode = m_nRngEvent;
	if (dlg.DoModal() == IDCANCEL)
	{
		theApp.DeleteDlgMap(_T("CSelectEventMode"));
		return;
	}
	theApp.DeleteDlgMap(_T("CSelectEventMode"));

	int nRngEvent = dlg.m_nEventMode;

	if (nRngEvent != m_nRngEvent){
		unsigned char rng = 0;

		if (nRngEvent == 1)	// 手動実行
		{
			// RNGが手動
			rng = 0;
		}
		else
		{
			// RNGが自動
			rng = 1;
		}

		// 衛星捕捉モード設定要求の通知を送る
		// アップリンク, ダウンリンク, CMD, WAY制御取得
		unsigned char cmd = GetKanshiCmdMode();
		unsigned char downlink = GetKanshiDownLinkMode();
		unsigned char uplink = GetKanshiUpLinkMode();
		unsigned char way = GetKanshiWayMode();
		unsigned char binary;
		binary = way << eSatellieMode_Way |						/* 1Way/2Way/Dup設定 */
			uplink << eSatellieMode_UpLink |					/* アップリンク制御設定 */
			downlink << eSatellieMode_DownLink |				/* ダウンリンク制御設定 */
			rng << eSatellieMode_RNG |							/* RNG制御設定 */
			cmd << eSatellieMode_CMD |							/* CMD制御設定 */
			1 << eSatellieMode_AntStandBy |						/* ANT待ち受け指向種別 */
			1 << eSatellieMode_AutoManual;						/* 自動捕捉/手動捕捉種別 */

		SendSatelliteCaptureModeNotify(binary);

//#ifdef _DEBUG
//		m_nRngEvent = nRngEvent;
//#endif

		m_RngEventStatic.Invalidate();
		UpdateData(FALSE);
	}
}
/*============================================================================*/
/*! 運用計画タイムライン

-# RNG（実施情報追加）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonRngadd()
{
	CEventAdd dlg;
	theApp.SetDlgMap(_T("CEventAdd"), (CWnd*)&dlg);
	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];
	dlg.mMode = eEventAddModeRng;
#if 1
	dlg.mStartTime = tdata.m_OpetDay.start;
	dlg.mEndTime = tdata.m_OpetDay.end;

	int curpos = m_cbRngNo.GetCurSel();

	if (curpos < tdata.m_RngDayList.size())
	{
		dlg.mStartTime = tdata.m_RngDayList[curpos].end;
	}
	if (curpos + 1 < tdata.m_RngDayList.size())
	{
		dlg.mEndTime = tdata.m_RngDayList[curpos + 1].start;
	}
	dlg.SetRngTypeList(m_RngEleList);

	CTime cSTime, cETime;

	while (1)
	{
		if (dlg.DoModal() == IDCANCEL)
		{
			theApp.DeleteDlgMap(_T("CEventAdd"));
			return;
		}
		theApp.DeleteDlgMap(_T("CEventAdd"));

		cSTime = dlg.mStartTime;
		cETime = dlg.mEndTime;

		if ((tdata.m_OpetDay.start <= cSTime && tdata.m_OpetDay.end >= cSTime)
			&& (tdata.m_OpetDay.start <= cETime && tdata.m_OpetDay.end >= cETime))
		{
			BOOL bConflict = FALSE;
			for (int i = 0; i < tdata.m_RngDayList.size(); i++)
			{
				if (((tdata.m_RngDayList[i].start <= cSTime) && (tdata.m_RngDayList[i].end > cSTime))
					|| ((tdata.m_RngDayList[i].start < cETime) && (tdata.m_RngDayList[i].end >= cETime)))
				{
					bConflict = TRUE;
					break;
				}
				// 新イベント開始時刻と終了時刻の間に、既存イベント開始時刻と終了時刻が含まれるか確認
				if (cSTime <= tdata.m_RngDayList[i].start && cETime >= tdata.m_RngDayList[i].end)
				{
					bConflict = TRUE;
					break;
				}
			}
			if (bConflict)
			{
				AfxMessageBox(_T("RNG の開始/終了日時が重複しています。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				break;
			}
		}
		else
		{
			AfxMessageBox(_T("RNG の開始/終了日時が、運用開始/終了日時の範囲外です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
	}
#else
	if (tdata.m_RngDayList.size() == 0){
		SYSTEMTIME time;
		GetSystemTime(&time);
		dlg.mStartTime = CTime(time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, 0);
		dlg.mEndTime = CTime(time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, 0);
	}
	else{
		dlg.mStartTime = tdata.m_RngDayList[0].start;
		dlg.mEndTime = tdata.m_RngDayList[0].end;
	}
	dlg.SetRngTypeList(m_RngEleList);
	if (dlg.DoModal() == IDCANCEL)
		return;
#endif

	stTime data;
	data.start = dlg.mStartTime;
	data.end = dlg.mEndTime;
//	tdata.m_RngDayList.push_back(data);
	if (curpos == -1 || tdata.m_RngDayList.size() == 0)
	{
		curpos = -1;
		tdata.m_RngDayList.insert(tdata.m_RngDayList.begin(), data);
		tdata.m_RngTypeList.insert(tdata.m_RngTypeList.begin(), dlg.mRngType);
	}
	else
	{
		tdata.m_RngDayList.insert(tdata.m_RngDayList.begin() + curpos + 1, data);
		tdata.m_RngTypeList.insert(tdata.m_RngTypeList.begin() + curpos + 1, dlg.mRngType);
	}

	tdata.m_RngCnt = (int)tdata.m_RngDayList.size();
	if (tdata.m_RngCnt > 0)
		tdata.m_RngChk = TRUE;
	else
		tdata.m_RngChk = FALSE;

//	int pos = tdata.m_RngCnt - 1;
	int pos = curpos + 1;
	m_strRngStart = (tdata.m_RngDayList[pos].start).Format(_T("%Y-%m-%d %H:%M:%S"));
	m_strRngStop = (tdata.m_RngDayList[pos].end).Format(_T("%Y-%m-%d %H:%M:%S"));
	m_strRngType = tdata.m_RngTypeList[pos];
	m_cbRngNo.ResetContent();
	for (int i = 0; i < tdata.m_RngDayList.size(); i++){
		CString str;
		str.Format(_T("%d"), i + 1);
		m_cbRngNo.AddString(str);
	}
	m_nRngNo = pos;
	m_cbRngNo.SetCurSel(m_nRngNo);
	UpdateData(FALSE);

	stPlandata pdata;
	UpdatePlanData(ePlanPStart, pdata);

	mGraph.SetSelectRng(m_nRngNo);
	mGraphHeader.SetSelectRng(m_nRngNo);
	mGraph.SetTimeLineData(tdata);
	mGraphHeader.SetTimeLineData(tdata);
}
/*============================================================================*/
/*! 運用計画タイムライン

-# RNG（実施情報変更）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonRngupdate()
{
	CEventUpdate dlg;
	theApp.SetDlgMap(_T("CEventUpdate"), (CWnd*)&dlg);
	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];
	dlg.mMode = eEventUpdateModeRng;
	int curpos = m_cbRngNo.GetCurSel();
	if (tdata.m_RngDayList.size() == 0){
		CString str;
		if (str.LoadString(IDS_ERROR_EVENT_NONE))
		{
			MessageBox(str);
		}
		return;
	}
	else{
		dlg.mDefStartTime = tdata.m_RngDayList[curpos].start;
		dlg.mDefEndTime = tdata.m_RngDayList[curpos].end;
		dlg.mStartTime = tdata.m_RngDayList[curpos].start;
		dlg.mEndTime = tdata.m_RngDayList[curpos].end;
	}
#if 1
	CTime cSTime, cETime;
	while (1)
	{
		if (dlg.DoModal() == IDCANCEL)
		{
			theApp.DeleteDlgMap(_T("CEventUpdate"));
			return;
		}
		theApp.DeleteDlgMap(_T("CEventUpdate"));

		cSTime = dlg.mStartTime;
		cETime = dlg.mEndTime;

		if ((tdata.m_OpetDay.start <= cSTime && tdata.m_OpetDay.end >= cSTime)
			&& (tdata.m_OpetDay.start <= cETime && tdata.m_OpetDay.end >= cETime))
		{
			BOOL bConflict = FALSE;
			for (int i = 0; i < tdata.m_RngDayList.size(); i++)
			{
				if (i == curpos)
					continue;

				if (((tdata.m_RngDayList[i].start <= cSTime) && (tdata.m_RngDayList[i].end > cSTime))
					|| ((tdata.m_RngDayList[i].start < cETime) && (tdata.m_RngDayList[i].end >= cETime)))
				{
					bConflict = TRUE;
					break;
				}
				// 新イベント開始時刻と終了時刻の間に、既存イベント開始時刻と終了時刻が含まれるか確認
				if (cSTime <= tdata.m_RngDayList[i].start && cETime >= tdata.m_RngDayList[i].end)
				{
					bConflict = TRUE;
					break;
				}
			}
			if (bConflict)
			{
				AfxMessageBox(_T("RNG の開始/終了日時が重複しています。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				break;
			}
		}
		else
		{
			AfxMessageBox(_T("RNG の開始/終了日時が、運用開始/終了日時の範囲外です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
	}
#else
	if (dlg.DoModal() == IDCANCEL)
		return;
#endif

	tdata.m_RngDayList[curpos].start = dlg.mStartTime;
	tdata.m_RngDayList[curpos].end = dlg.mEndTime;

	tdata.m_RngCnt = (int)tdata.m_RngDayList.size();
	if (tdata.m_RngCnt > 0)
		tdata.m_RngChk = TRUE;
	else
		tdata.m_RngChk = FALSE;

	m_strRngStart = (tdata.m_RngDayList[curpos].start).Format(_T("%Y-%m-%d %H:%M:%S"));
	m_strRngStop = (tdata.m_RngDayList[curpos].end).Format(_T("%Y-%m-%d %H:%M:%S"));
	m_strRngType = tdata.m_RngTypeList[curpos];
	m_cbRngNo.ResetContent();
	for (int i = 0; i < tdata.m_RngDayList.size(); i++){
		CString str;
		str.Format(_T("%d"), i + 1);
		m_cbRngNo.AddString(str);
	}
	m_nRngNo = curpos;
	m_cbRngNo.SetCurSel(m_nRngNo);
	UpdateData(FALSE);

	stPlandata pdata;
	UpdatePlanData(ePlanPStart, pdata);

	mGraph.SetSelectRng(m_nRngNo);
	mGraphHeader.SetSelectRng(m_nRngNo);
	mGraph.SetTimeLineData(tdata);
	mGraphHeader.SetTimeLineData(tdata);
}
/*============================================================================*/
/*! 運用計画タイムライン

-# RNG（実施情報削除）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonRngdel()
{
	CEventDel dlg;
	theApp.SetDlgMap(_T("CEventDel"), (CWnd*)&dlg);
	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];
	dlg.mMode = eEventDelModeRng;
	if (tdata.m_RngDayList.size() <= 0){
		CString str;
		if (str.LoadString(IDS_ERROR_EVENT_DELETE))
		{
			MessageBox(str);
		}
		return;
	}
	dlg.mTargetNo = m_cbRngNo.GetCurSel();
	if (dlg.DoModal() == IDCANCEL)
	{
		theApp.DeleteDlgMap(_T("CEventDel"));
		return;
	}
	theApp.DeleteDlgMap(_T("CEventDel"));

	vector<stTime>::iterator itr;
	int pos = 0;
	for (itr = tdata.m_RngDayList.begin(); itr != tdata.m_RngDayList.end(); itr++){
		if (dlg.mTargetNo == pos){
			tdata.m_RngDayList.erase(itr);
			break;
		}
		pos++;
	}

	vector<CString>::iterator itrType;
	pos = 0;
	for (itrType = tdata.m_RngTypeList.begin(); itrType != tdata.m_RngTypeList.end(); itrType++)
	{
		if (dlg.mTargetNo == pos)
		{
			tdata.m_RngTypeList.erase(itrType);
			break;
		}
		pos++;
	}

	tdata.m_RngCnt = (int)tdata.m_RngDayList.size();
	if (tdata.m_RngCnt > 0)
		tdata.m_RngChk = TRUE;
	else
		tdata.m_RngChk = FALSE;

	m_cbRngNo.ResetContent();
	for (int i = 0; i < tdata.m_RngDayList.size(); i++){
		CString str;
		str.Format(_T("%d"), i + 1);
		m_cbRngNo.AddString(str);
	}
	m_nRngNo = 0;
	m_cbRngNo.SetCurSel(m_nRngNo);
	if (tdata.m_RngDayList.size() == 0){
		m_strRngStart = _T("-------------------");
		m_strRngStop = _T("-------------------");
		m_strRngType = _T("---");
	}
	else{
		m_strRngStart = (tdata.m_RngDayList[m_nRngNo].start).Format(_T("%Y-%m-%d %H:%M:%S"));
		m_strRngStop = (tdata.m_RngDayList[m_nRngNo].end).Format(_T("%Y-%m-%d %H:%M:%S"));
		m_strRngType = tdata.m_RngTypeList[m_nRngNo];
	}
	UpdateData(FALSE);

	stPlandata pdata;
	UpdatePlanData(ePlanPStart, pdata);

	mGraph.SetSelectRng(m_nRngNo);
	mGraphHeader.SetSelectRng(m_nRngNo);
	mGraph.SetTimeLineData(tdata);
	mGraphHeader.SetTimeLineData(tdata);
}
/*============================================================================*/
/*! 運用計画タイムライン

-# RNG（実施番号変更）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnCbnSelchangeComboRngno()
{
	UpdateData(TRUE);
	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];
	m_nRngNo = m_cbRngNo.GetCurSel();
	m_strRngStart = (tdata.m_RngDayList[m_nRngNo].start).Format(_T("%Y-%m-%d %H:%M:%S"));
	m_strRngStop = (tdata.m_RngDayList[m_nRngNo].end).Format(_T("%Y-%m-%d %H:%M:%S"));
	m_strRngType = tdata.m_RngTypeList[m_nRngNo];
	UpdateData(FALSE);

	mGraph.SetSelectRng(m_nRngNo);
	mGraphHeader.SetSelectRng(m_nRngNo);
	mGraph.SetTimeLineData(tdata);
	mGraphHeader.SetTimeLineData(tdata);
}
/*============================================================================*/
/*! 運用計画タイムライン

-# 運用オプション（ANT天頂指向切替）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonAnt()
{
	UpdateData(TRUE);
	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];
	COptionAnt dlg;
	theApp.SetDlgMap(_T("COptionAnt"), (CWnd*)&dlg);
	dlg.m_nAntMode = tdata.m_AntZenith;
	if (dlg.DoModal() == IDCANCEL)
	{
		theApp.DeleteDlgMap(_T("COptionAnt"));
		return;
	}
	theApp.DeleteDlgMap(_T("COptionAnt"));

	tdata.m_AntZenith = dlg.m_nAntMode;
	m_strOptAnt = mOptANTZenith[tdata.m_AntZenith];
	UpdateData(FALSE);
	/****************************************************************/
	/* 制御情報 */
	CString ctrl = _T("");
	ctrl.Format(_T("CTRL.ANT_WAITPOINT %s"), (LPCTSTR)mControlOptANTZenith[tdata.m_AntZenith]);
	theApp.SendControl(ctrl, _T(""), _T(""), _T(""));

	mGraph.SetTimeLineData(tdata);
	mGraphHeader.SetTimeLineData(tdata);
}
/*============================================================================*/
/*! 運用計画タイムライン

-# 主局従局識別（主従切替）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonMasterslave()
{
	UpdateData(TRUE);
	CSelectMasterSlave dlg;
	theApp.SetDlgMap(_T("CSelectMasterSlave"), (CWnd*)&dlg);
	dlg.mSelectMode = mMasterSlave;
	if (dlg.DoModal() == IDCANCEL)
	{
		theApp.DeleteDlgMap(_T("CSelectMasterSlave"));
		return;
	}
	theApp.DeleteDlgMap(_T("CSelectMasterSlave"));

	UINT masterSlave = dlg.mSelectMode;

	// 主局従局の内容を計画ファイルに書き出す
	stPlandata data;
	data.sz_priority = mMasterSlabePlan[masterSlave];

	UpdatePlanData(ePlanMasterSlave, data);

	CGraphMutex::Lock(ePassInfo);
	GetPlanDataFromFile();
	UpdateTimelineData(mSelectForecast);
//	UpdateInitializePropertyData();
//	UpdateOperationProperty();
	UpdateOperationOption();
	UpdateMonitorData();
	CGraphMutex::Unlock(ePassInfo);

	UpdateData(FALSE);
}
/*============================================================================*/
/*! 運用計画タイムライン

-# プロパティ変更（設定）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonPropset()
{
//	UpdateInitializePropertyData();
//	UpdateOperationProperty();

	GetSatelliteXTX();
	stOpeProperty back = mOpeProp;
	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];
	COperationProperty dlg(this);
	theApp.SetDlgMap(_T("COperationProperty"), (CWnd*)&dlg);
	dlg.SetOpeProp_Station(m_strSpaceName);
	dlg.SetOpeProp_Equipment(tdata.mGetForm==true?_T("enp"):_T("sttn"));
	dlg.SetOpeProp_TxtMode(mXtx);
	if (dlg.DoModal() == IDCANCEL)
	{
		theApp.DeleteDlgMap(_T("COperationProperty"));
		return;
	}
	theApp.DeleteDlgMap(_T("COperationProperty"));

	UpdateData(TRUE);

	// 運用プロパティウィンドウで上書きされるかもしれないので元に戻す
	mOpeProp.init.reclevels = back.init.reclevels;
	mOpeProp.init.reclevelx = back.init.reclevelx;
	mOpeProp.init.tlm = back.init.tlm;
	mOpeProp.init.cmd = back.init.cmd;

	// プロパティデータの書き込み
	_WriteOpeProp();

	// シェルコマンドを実行
	const CString mRegistProp = _T("REGIST_OPEPROPERTY.sh"); // 運用プロパティ登録要求

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = mRegistProp;
	CString arg;

	int nStation = theApp.GetSelectStation();
	arg.Format(_T("%s"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower());
	stParam.strRemShellParam.Format(arg);

	// リモシェ実行
	int err = CSshShell::ExecShell(stParam, TRUE);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell"), __FILEW__, __LINE__);
		CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("OnBnClickedButtonPropset"), _T("debug"), strDebug, nLogEx::debug);
	}

//	UpdateOperationProperty();
}
/*============================================================================*/
/*! 運用計画タイムライン

-# プロパティ変更（リセット）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonPropreset()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}
/*============================================================================*/
/*! 運用計画タイムライン

-# プロパティ変更（CID切替）

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonPropcid()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}




/*============================================================================*/
/*! CSatelliteData

-# 予報値一覧ファイルを読み込みデータ取得

@param	なし
@retval	1：成功 / -1：失敗

*/
/*============================================================================*/
BOOL CTimeLineView::SetPredList_Euc()
{
	BOOL ret = TRUE;

	mGraph.mListPredData.clear();

	// 1局モード
	ret = SetPredList_Euc(theApp.GetSelectStation());

	return ret;
}


/*============================================================================*/
/*! CSatelliteData

-# 指定局の予報値一覧ファイルを読み込みデータ取得(EUC対応)

@param	nStation：局番号
@retval	1：成功 / -1：失敗

*/
/*============================================================================*/
BOOL CTimeLineView::SetPredList_Euc(UINT nStation)
{
	FILE *fp;

	//	CString strFilePath;
	//	strFilePath.Format(_T("%s%s"), theApp.GetAppPath(), PRED_LIST_FILE);
	CString strFilePath = theApp.GetShareFilePath(eFileType_PredList_File, nStation);

	TCHAR sz_aos[50];
	TCHAR sz_aos_date[50];
	TCHAR sz_los[50];
	TCHAR sz_los_date[50];
	TCHAR sz_max[50];
	TCHAR sz_max_date[50];

	TCHAR sz_stationname[50] = { 0 };
	TCHAR sz_satname[50] = { 0 };
	TCHAR sz_t_flag[50];
	TCHAR sz_p_flag[50];
	TCHAR sz_pass[50] = { 0 };
	TCHAR sz_a_date[50];
	TCHAR sz_a_time[50];
	TCHAR sz_a_az[50] = { 0 };
	TCHAR sz_a_el[50] = { 0 };
	TCHAR sz_a_rng[50];
	TCHAR sz_l_date[50];
	TCHAR sz_l_time[50];
	TCHAR sz_l_az[50] = { 0 };
	TCHAR sz_l_el[50] = { 0 };
	TCHAR sz_l_rng[50];
	TCHAR sz_m_date[50];
	TCHAR sz_m_time[50];
	TCHAR sz_m_az[50] = { 0 };
	TCHAR sz_m_el[50] = { 0 };
	TCHAR sz_m_rng[50];
	TCHAR sz_date[50];
	TCHAR sz_time[50];
	TCHAR sz_pred_path[50] = { 0 };
	TCHAR sz_offset[50] = { 0 };

	TCHAR sz_format[50] = { 0 };
	TCHAR sz_get_from[50] = { 0 };

	int nCol;
	CTime	cTime;

	if ((_wfopen_s(&fp, strFilePath, _T("r")) != 0) || (fp == NULL))
	{
		//		fprintf(stderr, "%s\n", "予報値一覧ファイルのオープンに失敗しました。");
		//		uc_opelog(NULL, NULL, BASE_OPE_NO + 14, NULL, NULL);
		return FALSE;
	}

#if 1
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	//CTime now(CTime::GetCurrentTime());
	CTime now(systime);
	//CTime now(CTime::GetCurrentTime());
	CTimeSpan tspan, tspan2;
	CString sat;
	CTime bs, be;

#endif

	char szline[2048];
	CString str, cbuf;
	while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL)
	{
		cbuf = CServerFile::ConvertUnicode(szline);
		stPredListData pld;

		// 新フォーマット読み込み
		nCol = swscanf_s(cbuf, L"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
			sz_stationname, static_cast<unsigned>(_countof(sz_stationname)),
			sz_satname, static_cast<unsigned>(_countof(sz_satname)),
			sz_t_flag, static_cast<unsigned>(_countof(sz_t_flag)),
			sz_p_flag, static_cast<unsigned>(_countof(sz_p_flag)),
			sz_pass, static_cast<unsigned>(_countof(sz_pass)),
			sz_a_date, static_cast<unsigned>(_countof(sz_a_date)),
			sz_a_time, static_cast<unsigned>(_countof(sz_a_time)),
			sz_a_az, static_cast<unsigned>(_countof(sz_a_az)),
			sz_a_el, static_cast<unsigned>(_countof(sz_a_el)),
			sz_a_rng, static_cast<unsigned>(_countof(sz_a_rng)),
			sz_l_date, static_cast<unsigned>(_countof(sz_l_date)),
			sz_l_time, static_cast<unsigned>(_countof(sz_l_time)),
			sz_l_az, static_cast<unsigned>(_countof(sz_l_az)),
			sz_l_el, static_cast<unsigned>(_countof(sz_l_el)),
			sz_l_rng, static_cast<unsigned>(_countof(sz_l_rng)),
			sz_m_date, static_cast<unsigned>(_countof(sz_m_date)),
			sz_m_time, static_cast<unsigned>(_countof(sz_m_time)),
			sz_m_az, static_cast<unsigned>(_countof(sz_m_az)),
			sz_m_el, static_cast<unsigned>(_countof(sz_m_el)),
			sz_m_rng, static_cast<unsigned>(_countof(sz_m_rng)),
			sz_date, static_cast<unsigned>(_countof(sz_date)),
			sz_time, static_cast<unsigned>(_countof(sz_time)),
			sz_pred_path, static_cast<unsigned>(_countof(sz_pred_path)),
			sz_offset, static_cast<unsigned>(_countof(sz_offset)),
			sz_format, static_cast<unsigned>(_countof(sz_format)),
			sz_get_from, static_cast<unsigned>(_countof(sz_get_from)));

		if (nCol < 0)
			continue;

		if (nCol != 26)
		{
			// 新フォーマット読み込み（タイプなし）
			nCol = swscanf_s(cbuf, L"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
				sz_stationname, static_cast<unsigned>(_countof(sz_stationname)),
				sz_satname, static_cast<unsigned>(_countof(sz_satname)),
				sz_t_flag, static_cast<unsigned>(_countof(sz_t_flag)),
				sz_p_flag, static_cast<unsigned>(_countof(sz_p_flag)),
				sz_pass, static_cast<unsigned>(_countof(sz_pass)),
				sz_a_date, static_cast<unsigned>(_countof(sz_a_date)),
				sz_a_time, static_cast<unsigned>(_countof(sz_a_time)),
				sz_a_az, static_cast<unsigned>(_countof(sz_a_az)),
				sz_a_el, static_cast<unsigned>(_countof(sz_a_el)),
				sz_a_rng, static_cast<unsigned>(_countof(sz_a_rng)),
				sz_l_date, static_cast<unsigned>(_countof(sz_l_date)),
				sz_l_time, static_cast<unsigned>(_countof(sz_l_time)),
				sz_l_az, static_cast<unsigned>(_countof(sz_l_az)),
				sz_l_el, static_cast<unsigned>(_countof(sz_l_el)),
				sz_l_rng, static_cast<unsigned>(_countof(sz_l_rng)),
				sz_m_date, static_cast<unsigned>(_countof(sz_m_date)),
				sz_m_time, static_cast<unsigned>(_countof(sz_m_time)),
				sz_m_az, static_cast<unsigned>(_countof(sz_m_az)),
				sz_m_el, static_cast<unsigned>(_countof(sz_m_el)),
				sz_m_rng, static_cast<unsigned>(_countof(sz_m_rng)),
				sz_date, static_cast<unsigned>(_countof(sz_date)),
				sz_time, static_cast<unsigned>(_countof(sz_time)),
				sz_pred_path, static_cast<unsigned>(_countof(sz_pred_path)),
				sz_offset, static_cast<unsigned>(_countof(sz_offset)),
				sz_get_from, static_cast<unsigned>(_countof(sz_get_from)));
		}

		if (nCol < 25)
		{
			// 旧フォーマット読み込み
			swscanf_s(cbuf, L"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
				sz_satname, static_cast<unsigned>(_countof(sz_satname)),
				sz_t_flag, static_cast<unsigned>(_countof(sz_t_flag)),
				sz_p_flag, static_cast<unsigned>(_countof(sz_p_flag)),
				sz_pass, static_cast<unsigned>(_countof(sz_pass)),
				sz_a_date, static_cast<unsigned>(_countof(sz_a_date)),
				sz_a_time, static_cast<unsigned>(_countof(sz_a_time)),
				sz_a_az, static_cast<unsigned>(_countof(sz_a_az)),
				sz_a_el, static_cast<unsigned>(_countof(sz_a_el)),
				sz_a_rng, static_cast<unsigned>(_countof(sz_a_rng)),
				sz_l_date, static_cast<unsigned>(_countof(sz_l_date)),
				sz_l_time, static_cast<unsigned>(_countof(sz_l_time)),
				sz_l_az, static_cast<unsigned>(_countof(sz_l_az)),
				sz_l_el, static_cast<unsigned>(_countof(sz_l_el)),
				sz_l_rng, static_cast<unsigned>(_countof(sz_l_rng)),
				sz_m_date, static_cast<unsigned>(_countof(sz_m_date)),
				sz_m_time, static_cast<unsigned>(_countof(sz_m_time)),
				sz_m_az, static_cast<unsigned>(_countof(sz_m_az)),
				sz_m_el, static_cast<unsigned>(_countof(sz_m_el)),
				sz_m_rng, static_cast<unsigned>(_countof(sz_m_rng)),
				sz_date, static_cast<unsigned>(_countof(sz_date)),
				sz_time, static_cast<unsigned>(_countof(sz_time)),
				sz_pred_path, static_cast<unsigned>(_countof(sz_pred_path)),
				sz_offset, static_cast<unsigned>(_countof(sz_offset)));
		}

		if (nCol < 16)
			continue;

		{
			if (nCol >= 25)
			{
				// 局名
				pld.strStationName = CString(sz_stationname);
			}

			// 衛星名
			pld.strSatellite = CString(sz_satname);

			// 衛星がカレントの衛星でない場合はリストに追加しない
			if (pld.strSatellite.CollateNoCase(mSataName) != 0)
				continue;

			// パスID
			pld.strPassId = CString(sz_pass);

			// AOS時刻の変換
			swprintf_s(sz_aos, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_a_date, sz_a_time, L".0000");
			if (ub_asc_to_asctime(sz_aos, sz_aos_date) < 0) {
				fclose(fp);
				return FALSE;
			}
			cTime = StrTimeToCTime(CString(sz_aos_date));
#if 0	// ダミー
			CTime ts = cTime;
			tspan = now - cTime;
			cTime += tspan;

			if (sat == CString(sz_satname))
			{
				tspan = ts - bs;
				cTime += tspan;
			}
			bs = cTime;
#endif

			pld.tAOS = cTime.GetTime();

			// AOS AZ
			str = CString(sz_a_az);
			pld.dAosAz = _ttof(str.GetBuffer());
			str.ReleaseBuffer();

			// AOS EL
			str = CString(sz_a_el);
			pld.dAosEl = _ttof(str.GetBuffer());
			str.ReleaseBuffer();

			// LOS時刻の変換
			swprintf_s(sz_los, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_l_date, sz_l_time, L".0000");
			if (ub_asc_to_asctime(sz_los, sz_los_date) < 0) {
				fclose(fp);
				return FALSE;
			}
			cTime = StrTimeToCTime(CString(sz_los_date));

#if 0	// ダミー
			CTime te = cTime;
			tspan = now - cTime;
			tspan2 = te - ts;
			cTime += (tspan + tspan2);

			if (sat == CString(sz_satname))
			{
				tspan = te - be;
				cTime += tspan;
			}
			be = cTime;
#endif

			pld.tLOS = cTime.GetTime();

			// LOS AZ
			str = CString(sz_l_az);
			pld.dLosAz = _ttof(str.GetBuffer());
			str.ReleaseBuffer();

			// LOS EL
			str = CString(sz_l_el);
			pld.dLosEl = _ttof(str.GetBuffer());
			str.ReleaseBuffer();

			// MAX EL時刻の変換
			swprintf_s(sz_max, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_m_date, sz_m_time, L".0000");
			if (ub_asc_to_asctime(sz_max, sz_max_date) < 0) {
				fclose(fp);
				return FALSE;
			}
			cTime = StrTimeToCTime(CString(sz_max_date));

			pld.tMaxEl = cTime.GetTime();

			// MAX EL AZ
			str = CString(sz_m_az);
			pld.dMaxAz = _ttof(str.GetBuffer());
			str.ReleaseBuffer();

			// MAX EL EL
			str = CString(sz_m_el);
			pld.dMaxEl = _ttof(str.GetBuffer());
			str.ReleaseBuffer();

			// 予報値ファイル名
			pld.strPredPath = CString(sz_pred_path);

			// オフセット
			str = CString(sz_offset);
			// 正負を抽出
			long nSign = 1;
			if (str.Left(1) == _T('-'))
			{
				nSign = -1;
				str.Remove(_T('-'));
			}
			// 右から２つが秒
			CString strSS = str.Right(2);
			// 残りが分と":"なので秒と":"の文字数を除外し、分を出す
			long nLength = str.GetLength();
			CString strMM = str.Left(nLength - (2 + 1));
			long nMM = _ttoi(strMM);
			long nSS = _ttoi(strSS);
			pld.tOffset = nSign * (nMM * 60 + nSS);

			if (nCol >= 25)
			{
				if (nCol == 25)
				{
					// タイプ
					pld.strFormat = _T("");
				}
				else
				{
					// タイプ
					pld.strFormat = CString(sz_format);
				}

				// 取得先
				pld.strGetFrom = CString(sz_get_from).MakeUpper();
			}

			mGraph.mListPredData.push_back(pld);

#if 1
			sat = CString(sz_satname);
#endif
		}
	}

	fclose(fp);

#ifdef _CUTNOCOMM

	SYSTEMTIME sys;
	GetSystemTime(&sys);
	sys.wSecond = 0;
	CTime local = CTime(sys);
	int pos = 0;
	//	for (int i = 0; i < sizeof(mStationString) / sizeof(mStationString[0]); i++)
	{
		if (m_listPredList.size() == 0){
			return FALSE;
		}
		int i = nStation;
		stPredListData pld = m_listPredList[0];
		const TCHAR* mSpaceString[]{
			_T("ASTRO-E2"),
				_T("HAYABUSA2"),
				_T("IKAROS"),
				_T("ASTRO-H"),
				_T("GEOTAIL"),
		};
		//for (int j = 0; j < sizeof(mSpaceString) / sizeof(mSpaceString[0]); j++)
		{
			int j = 0;
			// 局名
			pld.strStationName = mStationString[i];
			// 衛星名
			pld.strSatellite = mSpaceString[j];
			// パスID
			pld.strPassId.Format(_T("170200-0%d%d0"), i, j);
			// AOS
			pld.tAOS = (local + CTimeSpan(0, i, 10 * (i + 1), 0)).GetTime();
			// LOS
			pld.tLOS = (local + CTimeSpan(0, i + 1, 20 * (i + 1), 0)).GetTime();
			// 予報値ファイル名
			pld.strPredPath = _T("/CONFIG/prd/planet-c/1601100100
				
				");

			m_listPredList.push_back(pld);

			// パスID
			pld.strPassId.Format(_T("170200-9%d%d0"), i, j);
			// AOS
			pld.tAOS = (local + CTimeSpan(0, i + 1, 40 * (i + 1), 0)).GetTime();
			// LOS
			pld.tLOS = (local + CTimeSpan(0, i + 2, 50 * (i + 1), 0)).GetTime();

			// 予報値ファイル名(ishi)
			pld.strPredPath.Format(_T("/CONFIG/prd/%s/1702009%d%d0.prd"), mSpaceString[j], i, j);

			m_listPredList.push_back(pld);


			pos++;
		}
	}
#endif

	return TRUE;
}


/*============================================================================*/
/*! CSatelliteData

-# 予報値一覧ファイルを読み込みデータ取得

@param	なし
@retval	1：成功 / -1：失敗

*/
/*============================================================================*/
BOOL CTimeLineView::SetPredList_Utf8()
{
	BOOL ret = TRUE;

	mGraph.mListPredData.clear();

	// 1局モード
	ret = SetPredList_Utf8(theApp.GetSelectStation());

	return ret;
}


/*============================================================================*/
/*! CSatelliteData

-# 指定局の予報値一覧ファイルを読み込みデータ取得(UTF8対応)

@param	nStation：局番号
@retval	1：成功 / -1：失敗

*/
/*============================================================================*/
BOOL CTimeLineView::SetPredList_Utf8(UINT nStation)
{
	FILE *fp;

	//	CString strFilePath;
	//	strFilePath.Format(_T("%s%s"), theApp.GetAppPath(), PRED_LIST_FILE);
	CString strFilePath = theApp.GetShareFilePath(eFileType_PredList_File, nStation);

	TCHAR sz_pred_line[300];
	TCHAR sz_aos[50];
	TCHAR sz_aos_date[50];
	TCHAR sz_los[50];
	TCHAR sz_los_date[50];
	TCHAR sz_max[50];
	TCHAR sz_max_date[50];

	TCHAR sz_stationname[50] = { 0 };
	TCHAR sz_satname[50] = { 0 };
	TCHAR sz_t_flag[50];
	TCHAR sz_p_flag[50];
	TCHAR sz_pass[50] = { 0 };
	TCHAR sz_a_date[50];
	TCHAR sz_a_time[50];
	TCHAR sz_a_az[50] = { 0 };
	TCHAR sz_a_el[50] = { 0 };
	TCHAR sz_a_rng[50];
	TCHAR sz_l_date[50];
	TCHAR sz_l_time[50];
	TCHAR sz_l_az[50] = { 0 };
	TCHAR sz_l_el[50] = { 0 };
	TCHAR sz_l_rng[50];
	TCHAR sz_m_date[50];
	TCHAR sz_m_time[50];
	TCHAR sz_m_az[50] = { 0 };
	TCHAR sz_m_el[50] = { 0 };
	TCHAR sz_m_rng[50];
	TCHAR sz_date[50];
	TCHAR sz_time[50];
	TCHAR sz_pred_path[50] = { 0 };
	TCHAR sz_offset[50] = { 0 };

	TCHAR sz_format[50] = { 0 };
	TCHAR sz_get_from[50] = { 0 };

	int nCol;

	CTime	cTime;


	//if (_wfopen_s(&fp, strFilePath, _T("r, ccs=UTF-8")) != 0)
	if ((_wfopen_s(&fp, strFilePath, _T("r")) != 0) || (fp == NULL))
	{
		//		fprintf(stderr, "%s\n", "予報値一覧ファイルのオープンに失敗しました。");
		//		uc_opelog(NULL, NULL, BASE_OPE_NO + 14, NULL, NULL);
		return FALSE;
	}

#if 1
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	//CTime now(CTime::GetCurrentTime());
	CTime now(systime);
	CTimeSpan tspan, tspan2;
	CString sat;
	CTime bs, be;

#endif
	CString str;

	char sz_line[300];
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp) != NULL)
	{
		wsprintf(sz_pred_line, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		stPredListData pld;

		// 新フォーマット読み込み
		nCol = swscanf_s(sz_pred_line, L"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
			sz_stationname, static_cast<unsigned>(_countof(sz_stationname)),
			sz_satname, static_cast<unsigned>(_countof(sz_satname)),
			sz_t_flag, static_cast<unsigned>(_countof(sz_t_flag)),
			sz_p_flag, static_cast<unsigned>(_countof(sz_p_flag)),
			sz_pass, static_cast<unsigned>(_countof(sz_pass)),
			sz_a_date, static_cast<unsigned>(_countof(sz_a_date)),
			sz_a_time, static_cast<unsigned>(_countof(sz_a_time)),
			sz_a_az, static_cast<unsigned>(_countof(sz_a_az)),
			sz_a_el, static_cast<unsigned>(_countof(sz_a_el)),
			sz_a_rng, static_cast<unsigned>(_countof(sz_a_rng)),
			sz_l_date, static_cast<unsigned>(_countof(sz_l_date)),
			sz_l_time, static_cast<unsigned>(_countof(sz_l_time)),
			sz_l_az, static_cast<unsigned>(_countof(sz_l_az)),
			sz_l_el, static_cast<unsigned>(_countof(sz_l_el)),
			sz_l_rng, static_cast<unsigned>(_countof(sz_l_rng)),
			sz_m_date, static_cast<unsigned>(_countof(sz_m_date)),
			sz_m_time, static_cast<unsigned>(_countof(sz_m_time)),
			sz_m_az, static_cast<unsigned>(_countof(sz_m_az)),
			sz_m_el, static_cast<unsigned>(_countof(sz_m_el)),
			sz_m_rng, static_cast<unsigned>(_countof(sz_m_rng)),
			sz_date, static_cast<unsigned>(_countof(sz_date)),
			sz_time, static_cast<unsigned>(_countof(sz_time)),
			sz_pred_path, static_cast<unsigned>(_countof(sz_pred_path)),
			sz_offset, static_cast<unsigned>(_countof(sz_offset)),
			sz_format, static_cast<unsigned>(_countof(sz_format)),
			sz_get_from, static_cast<unsigned>(_countof(sz_get_from)));

		if (nCol != 26)
		{
			// 新フォーマット読み込み（タイプなし）
			nCol = swscanf_s(sz_pred_line, L"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
				sz_stationname, static_cast<unsigned>(_countof(sz_stationname)),
				sz_satname, static_cast<unsigned>(_countof(sz_satname)),
				sz_t_flag, static_cast<unsigned>(_countof(sz_t_flag)),
				sz_p_flag, static_cast<unsigned>(_countof(sz_p_flag)),
				sz_pass, static_cast<unsigned>(_countof(sz_pass)),
				sz_a_date, static_cast<unsigned>(_countof(sz_a_date)),
				sz_a_time, static_cast<unsigned>(_countof(sz_a_time)),
				sz_a_az, static_cast<unsigned>(_countof(sz_a_az)),
				sz_a_el, static_cast<unsigned>(_countof(sz_a_el)),
				sz_a_rng, static_cast<unsigned>(_countof(sz_a_rng)),
				sz_l_date, static_cast<unsigned>(_countof(sz_l_date)),
				sz_l_time, static_cast<unsigned>(_countof(sz_l_time)),
				sz_l_az, static_cast<unsigned>(_countof(sz_l_az)),
				sz_l_el, static_cast<unsigned>(_countof(sz_l_el)),
				sz_l_rng, static_cast<unsigned>(_countof(sz_l_rng)),
				sz_m_date, static_cast<unsigned>(_countof(sz_m_date)),
				sz_m_time, static_cast<unsigned>(_countof(sz_m_time)),
				sz_m_az, static_cast<unsigned>(_countof(sz_m_az)),
				sz_m_el, static_cast<unsigned>(_countof(sz_m_el)),
				sz_m_rng, static_cast<unsigned>(_countof(sz_m_rng)),
				sz_date, static_cast<unsigned>(_countof(sz_date)),
				sz_time, static_cast<unsigned>(_countof(sz_time)),
				sz_pred_path, static_cast<unsigned>(_countof(sz_pred_path)),
				sz_offset, static_cast<unsigned>(_countof(sz_offset)),
				sz_get_from, static_cast<unsigned>(_countof(sz_get_from)));
		}

		if (nCol < 25)
		{
			// 旧フォーマット読み込み
			swscanf_s(sz_pred_line, L"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
				sz_satname, static_cast<unsigned>(_countof(sz_satname)),
				sz_t_flag, static_cast<unsigned>(_countof(sz_t_flag)),
				sz_p_flag, static_cast<unsigned>(_countof(sz_p_flag)),
				sz_pass, static_cast<unsigned>(_countof(sz_pass)),
				sz_a_date, static_cast<unsigned>(_countof(sz_a_date)),
				sz_a_time, static_cast<unsigned>(_countof(sz_a_time)),
				sz_a_az, static_cast<unsigned>(_countof(sz_a_az)),
				sz_a_el, static_cast<unsigned>(_countof(sz_a_el)),
				sz_a_rng, static_cast<unsigned>(_countof(sz_a_rng)),
				sz_l_date, static_cast<unsigned>(_countof(sz_l_date)),
				sz_l_time, static_cast<unsigned>(_countof(sz_l_time)),
				sz_l_az, static_cast<unsigned>(_countof(sz_l_az)),
				sz_l_el, static_cast<unsigned>(_countof(sz_l_el)),
				sz_l_rng, static_cast<unsigned>(_countof(sz_l_rng)),
				sz_m_date, static_cast<unsigned>(_countof(sz_m_date)),
				sz_m_time, static_cast<unsigned>(_countof(sz_m_time)),
				sz_m_az, static_cast<unsigned>(_countof(sz_m_az)),
				sz_m_el, static_cast<unsigned>(_countof(sz_m_el)),
				sz_m_rng, static_cast<unsigned>(_countof(sz_m_rng)),
				sz_date, static_cast<unsigned>(_countof(sz_date)),
				sz_time, static_cast<unsigned>(_countof(sz_time)),
				sz_pred_path, static_cast<unsigned>(_countof(sz_pred_path)),
				sz_offset, static_cast<unsigned>(_countof(sz_offset)));
		}

		{
			if (nCol >= 25)
			{
				// 局名
				pld.strStationName = CString(sz_stationname);
			}

			// 衛星名
			pld.strSatellite = CString(sz_satname);

			// パスID
			pld.strPassId = CString(sz_pass);

			// AOS時刻の変換
			swprintf_s(sz_aos, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_a_date, sz_a_time, L".0000");
			if (ub_asc_to_asctime(sz_aos, sz_aos_date) < 0) {
				fclose(fp);
				return FALSE;
			}
			cTime = StrTimeToCTime(CString(sz_aos_date));
#if 0	// ダミー
			CTime ts = cTime;
			tspan = now - cTime;
			cTime += tspan;

			if (sat == CString(sz_satname))
			{
				tspan = ts - bs;
				cTime += tspan;
			}
			bs = cTime;
#endif

			pld.tAOS = cTime.GetTime();

			// AOS AZ
			str = CString(sz_a_az);
			pld.dAosAz = _ttof(str.GetBuffer());
			str.ReleaseBuffer();

			// AOS EL
			str = CString(sz_a_el);
			pld.dAosEl = _ttof(str.GetBuffer());
			str.ReleaseBuffer();

			// LOS時刻の変換
			swprintf_s(sz_los, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_l_date, sz_l_time, L".0000");
			if (ub_asc_to_asctime(sz_los, sz_los_date) < 0) {
				fclose(fp);
				return FALSE;
			}
			cTime = StrTimeToCTime(CString(sz_los_date));

#if 0	// ダミー
			CTime te = cTime;
			tspan = now - cTime;
			tspan2 = te - ts;
			cTime += (tspan + tspan2);

			if (sat == CString(sz_satname))
			{
				tspan = te - be;
				cTime += tspan;
			}
			be = cTime;
#endif

			pld.tLOS = cTime.GetTime();

			// LOS AZ
			str = CString(sz_l_az);
			pld.dLosAz = _ttof(str.GetBuffer());
			str.ReleaseBuffer();

			// LOS EL
			str = CString(sz_l_el);
			pld.dLosEl = _ttof(str.GetBuffer());
			str.ReleaseBuffer();

			// MAX EL時刻の変換
			swprintf_s(sz_max, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_m_date, sz_m_time, L".0000");
			if (ub_asc_to_asctime(sz_max, sz_max_date) < 0) {
				fclose(fp);
				return FALSE;
			}
			cTime = StrTimeToCTime(CString(sz_max_date));

			pld.tMaxEl = cTime.GetTime();

			// MAX EL AZ
			str = CString(sz_m_az);
			pld.dMaxAz = _ttof(str.GetBuffer());
			str.ReleaseBuffer();

			// MAX EL EL
			str = CString(sz_m_el);
			pld.dMaxEl = _ttof(str.GetBuffer());
			str.ReleaseBuffer();

			// 予報値ファイル名
			pld.strPredPath = CString(sz_pred_path);

			// オフセット
			str = CString(sz_offset);
			// 正負を抽出
			long nSign = 1;
			if (str.Left(1) == _T('-'))
			{
				nSign = -1;
				str.Remove(_T('-'));
			}
			// 右から２つが秒
			CString strSS = str.Right(2);
			// 残りが分と":"なので秒と":"の文字数を除外し、分を出す
			long nLength = str.GetLength();
			CString strMM = str.Left(nLength - (2 + 1));
			long nMM = _ttoi(strMM);
			long nSS = _ttoi(strSS);
			pld.tOffset = nSign * (nMM * 60 + nSS);

			if (nCol >= 25)
			{
				if (nCol == 25)
				{
					// タイプ
					pld.strFormat = _T("");
				}
				else
				{
					// タイプ
					pld.strFormat = CString(sz_format);
				}

				// 取得先
				pld.strGetFrom = CString(sz_get_from).MakeUpper();
			}

			mGraph.mListPredData.push_back(pld);

#if 1
			sat = CString(sz_satname);
#endif
		}
	}

	fclose(fp);


#ifdef _CUTNOCOMM

	SYSTEMTIME sys;
	GetSystemTime(&sys);
	sys.wSecond = 0;
	CTime local = CTime(sys);
	int pos = 0;
	//	for (int i = 0; i < sizeof(mStationString) / sizeof(mStationString[0]); i++)
	{
		if (mGraph.mListPredData.size() == 0){
			return FALSE;
		}
		int i = nStation;
		stPredListData pld = mGraph.mListPredData[0];
		const TCHAR* mSpaceString[]{
			_T("ASTRO-E2"),
				_T("HAYABUSA2"),
				_T("IKAROS"),
				_T("ASTRO-H"),
				_T("GEOTAIL"),
		};
		//for (int j = 0; j < sizeof(mSpaceString) / sizeof(mSpaceString[0]); j++)
		{
			int j = 0;
			// 局名
			pld.strStationName = mStationString[i];
			// 衛星名
			pld.strSatellite = mSpaceString[j];
			// パスID
			pld.strPassId.Format(_T("170200-0%d%d0"), i, j);
			// AOS
			pld.tAOS = (local + CTimeSpan(0, i, 10 * (i + 1), 0)).GetTime();
			// LOS
			pld.tLOS = (local + CTimeSpan(0, i + 1, 20 * (i + 1), 0)).GetTime();
			// 予報値ファイル名
			pld.strPredPath = _T("/CONFIG/prd/planet-c/1601100100.prd");

			mGraph.mListPredData.push_back(pld);

			// パスID
			pld.strPassId.Format(_T("170200-9%d%d0"), i, j);
			// AOS
			pld.tAOS = (local + CTimeSpan(0, i + 1, 40 * (i + 1), 0)).GetTime();
			// LOS
			pld.tLOS = (local + CTimeSpan(0, i + 2, 50 * (i + 1), 0)).GetTime();

			// 予報値ファイル名(ishi)
			pld.strPredPath.Format(_T("/CONFIG/prd/%s/1702009%d%d0.prd"), mSpaceString[j], i, j);

			mGraph.mListPredData.push_back(pld);

			pos++;
		}
	}
#endif

	return TRUE;
}

/*============================================================================*/
/*! CSatelliteData

-# 計画一覧を取得する

@param	なし
@retval	1：成功 / -1：失敗

*/
/*============================================================================*/
BOOL CTimeLineView::SetPlanList_Euc()
{
	BOOL ret = TRUE;

	mGraph.mListPlanData.clear();

	// 1局モード
	ret = SetPlanList_Euc(theApp.GetSelectStation());

	return ret;
}


/*============================================================================*/
/*! CSatelliteData

-# 指定局の計画一覧を取得する(EUC対応）

@param  nStation：局番号
@retval	1：成功 / -1：失敗
*/
/*============================================================================*/
BOOL CTimeLineView::SetPlanList_Euc(UINT nStation)
{
	FILE *fp;
	//	TCHAR sz_buff[500];
	TCHAR sz_stationname[50] = { 0 };
	TCHAR sz_satname[50] = { 0 };
	TCHAR sz_planned_pass[50] = { 0 };
	TCHAR sz_planned_stime1[50];
	TCHAR sz_planned_stime2[50];
	TCHAR sz_planned_etime1[50];
	TCHAR sz_planned_etime2[50];
	TCHAR sz_planned_itime1[50];
	TCHAR sz_planned_itime2[50];
	TCHAR sz_pred_name[50] = { 0 };

	TCHAR sz_pre[50];
	TCHAR sz_pre_date[50];
	TCHAR sz_aos[50];
	TCHAR sz_aos_date[50];
	TCHAR sz_los[50];
	TCHAR sz_los_date[50];

	TCHAR sz_get_from[50] = { 0 };
	TCHAR sz_plan_type[50] = { 0 };

	int	i_valid_flag;
	int	i_ofs_flag;
	int	i_offset;

	int nCol;

	//	CString strFilePath;
	//	strFilePath.Format(_T("%s%s"), theApp.GetAppPath(), PLAN_LIST_FILE);
	CString strFilePath = theApp.GetShareFilePath(eFileType_PlanList_File, nStation);


	CTime	cTime;

	if ((_wfopen_s(&fp, strFilePath, _T("r")) != 0) || (fp == NULL))
	{
		//		fprintf(stderr, "%s\n", "計画一覧ファイルのオープンに失敗しました。");
		//		uc_opelog(NULL, NULL, BASE_OPE_NO + 15, NULL, NULL);
		return FALSE;
	}

	char szline[2048];
	CString cbuf;
	fgets(szline, static_cast<unsigned>(_countof(szline)), fp);                /*計画一覧ファイルのヘッダー部の読み飛ばし*/
	while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != 0){    /*計画一覧ファイルの読み込み*/
		cbuf = CServerFile::ConvertUnicode(szline);
		stPlanListData pld;

		// 新フォーマット読み込み
		nCol = swscanf_s(cbuf, L"%s%s%s%s%s%s%s%s%s%s%d%d%d%s%s",
			sz_stationname, static_cast<unsigned>(_countof(sz_stationname)),
			sz_satname, static_cast<unsigned>(_countof(sz_satname)),
			sz_planned_pass, static_cast<unsigned>(_countof(sz_planned_pass)),
			sz_planned_stime1, static_cast<unsigned>(_countof(sz_planned_stime1)),
			sz_planned_stime2, static_cast<unsigned>(_countof(sz_planned_stime2)),
			sz_planned_etime1, static_cast<unsigned>(_countof(sz_planned_etime1)),
			sz_planned_etime2, static_cast<unsigned>(_countof(sz_planned_etime2)),
			sz_planned_itime1, static_cast<unsigned>(_countof(sz_planned_itime1)),
			sz_planned_itime2, static_cast<unsigned>(_countof(sz_planned_itime2)),
			sz_pred_name, static_cast<unsigned>(_countof(sz_pred_name)),
			&i_valid_flag, &i_ofs_flag, &i_offset,
			sz_get_from, static_cast<unsigned>(_countof(sz_get_from)),
			sz_plan_type, static_cast<unsigned>(_countof(sz_plan_type)));

		if (nCol < 9)
			continue;

		if (nCol < 15)
		{
			// レディネスチェック計画での予報値が無い場合を想定
			swscanf_s(cbuf, L"%s%s%s%s%s%s%s%s%s%d%d%d%s%s",
				sz_stationname, static_cast<unsigned>(_countof(sz_stationname)),
				sz_satname, static_cast<unsigned>(_countof(sz_satname)),
				sz_planned_pass, static_cast<unsigned>(_countof(sz_planned_pass)),
				sz_planned_stime1, static_cast<unsigned>(_countof(sz_planned_stime1)),
				sz_planned_stime2, static_cast<unsigned>(_countof(sz_planned_stime2)),
				sz_planned_etime1, static_cast<unsigned>(_countof(sz_planned_etime1)),
				sz_planned_etime2, static_cast<unsigned>(_countof(sz_planned_etime2)),
				sz_planned_itime1, static_cast<unsigned>(_countof(sz_planned_itime1)),
				sz_planned_itime2, static_cast<unsigned>(_countof(sz_planned_itime2)),
				&i_valid_flag, &i_ofs_flag, &i_offset,
				sz_get_from, static_cast<unsigned>(_countof(sz_get_from)),
				sz_plan_type, static_cast<unsigned>(_countof(sz_plan_type)));
		}

//		if (nCol == 15)
		{
			// 局名
			pld.strStationName = CString(sz_stationname);
		}

		// 衛星名
		pld.strSatelliteName = CString(sz_satname);

		// パスID
		pld.strPassId = CString(sz_planned_pass);

		// プリパス開始日時
		swprintf_s(sz_pre, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_planned_itime1, sz_planned_itime2, L".0000");
		if (ub_asc_to_asctime(sz_pre, sz_pre_date) < 0) {
			fclose(fp);
			return FALSE;
		}
		cTime = StrTimeToCTime(CString(sz_pre_date));
		pld.tPrePassStart = cTime.GetTime() + i_offset;

		// パス開始日時
		swprintf_s(sz_aos, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_planned_stime1, sz_planned_stime2, L".0000");
		if (ub_asc_to_asctime(sz_aos, sz_aos_date) < 0) {
			fclose(fp);
			return FALSE;
		}
		cTime = StrTimeToCTime(CString(sz_aos_date));
		pld.tPassStart = cTime.GetTime() + i_offset;

		// パス終了日時
		swprintf_s(sz_los, static_cast<unsigned>(_countof(sz_los)), L"%s %s%s", sz_planned_etime1, sz_planned_etime2, L".0000");
		if (ub_asc_to_asctime(sz_los, sz_los_date) < 0) {
			fclose(fp);
			return FALSE;
		}
		cTime = StrTimeToCTime(CString(sz_los_date));
		pld.tPassEnd = cTime.GetTime() + i_offset;

		// 予報値ファイル名
		if (nCol == 15)
		{
			pld.strPredName = CString(sz_pred_name);
		}
		else
		{
			pld.strPredName = _T("");
		}

		// チェック結果フラグ
		pld.lValidFlag = i_valid_flag;

		// 時刻オフセット追従条件フラグ
		pld.lOfsFlag = i_ofs_flag;

		// 時刻オフセット
		pld.tOffset = i_offset;

//		if (nCol == 15)
		{
			pld.strGetFrom = CString(sz_get_from).MakeUpper();
			pld.strPlanType = CString(sz_plan_type).MakeUpper();
		}

		// 計画ファイル名
		CString strPlanPath = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
		if (pld.strGetFrom.CompareNoCase(_T("ENP")) == 0)
		{
			pld.strPlanName = strPlanPath + _T("enp\\") + CString(sz_satname) + _T("\\") + CString(sz_planned_pass) + _T(".pln");
		}
		else
		{
			pld.strPlanName = strPlanPath + _T("sttn\\") + CString(sz_satname) + _T("\\") + CString(sz_planned_pass) + _T(".pln");
		}

		// リストに追加
		mGraph.mListPlanData.push_back(pld);
	}

	fclose(fp);

	return TRUE;
}

/*============================================================================*/
/*! CSatelliteData

-# 計画一覧を取得する

@param	なし
@retval	1：成功 / -1：失敗

*/
/*============================================================================*/
BOOL CTimeLineView::SetPlanList_Utf8()
{
	BOOL ret = TRUE;

	mGraph.mListPlanData.clear();

	// 1局モード
	ret = SetPlanList_Utf8(theApp.GetSelectStation());

	return ret;
}

/*============================================================================*/
/*! CSatelliteData

-# 指定局の計画一覧を取得する(UTF8対応）

@param  nStation：局番号
@retval	1：成功 / -1：失敗
*/
/*============================================================================*/
BOOL CTimeLineView::SetPlanList_Utf8(UINT nStation)
{
	FILE *fp;
	TCHAR sz_buff[500];
	TCHAR sz_stationname[50] = { 0 };
	TCHAR sz_satname[50] = { 0 };
	TCHAR sz_planned_pass[50] = { 0 };
	TCHAR sz_planned_stime1[50];
	TCHAR sz_planned_stime2[50];
	TCHAR sz_planned_etime1[50];
	TCHAR sz_planned_etime2[50];
	TCHAR sz_planned_itime1[50];
	TCHAR sz_planned_itime2[50];
	TCHAR sz_pred_name[50] = { 0 };

	TCHAR sz_pre[50];
	TCHAR sz_pre_date[50];
	TCHAR sz_aos[50];
	TCHAR sz_aos_date[50];
	TCHAR sz_los[50];
	TCHAR sz_los_date[50];

	TCHAR sz_get_from[50] = { 0 };
	TCHAR sz_plan_type[50] = { 0 };

	int	i_valid_flag;
	int	i_ofs_flag;
	int	i_offset;

	int nCol;

	CTime	cTime;

	//	CString strFilePath;
	//	strFilePath.Format(_T("%s%s"), theApp.GetAppPath(), PLAN_LIST_FILE);
	CString strFilePath = theApp.GetShareFilePath(eFileType_PlanList_File, nStation);

	//if (_wfopen_s(&fp, strFilePath, _T("r, ccs=UTF-8")) != 0)
	if ((_wfopen_s(&fp, strFilePath, _T("r")) != 0) || (fp == NULL))
	{
		//		fprintf(stderr, "%s\n", "計画一覧ファイルのオープンに失敗しました。");
		//		uc_opelog(NULL, NULL, BASE_OPE_NO + 15, NULL, NULL);
		return FALSE;
	}

	char sz_line[500];

	fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp);                /*計画一覧ファイルのヘッダー部の読み飛ばし*/
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp) != 0){    /*計画一覧ファイルの読み込み*/
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		stPlanListData pld;

		// 新フォーマット読み込み
		nCol = swscanf_s(sz_buff, L"%s%s%s%s%s%s%s%s%s%s%d%d%d%s%s",
			sz_stationname, static_cast<unsigned>(_countof(sz_stationname)),
			sz_satname, static_cast<unsigned>(_countof(sz_satname)),
			sz_planned_pass, static_cast<unsigned>(_countof(sz_planned_pass)),
			sz_planned_stime1, static_cast<unsigned>(_countof(sz_planned_stime1)),
			sz_planned_stime2, static_cast<unsigned>(_countof(sz_planned_stime2)),
			sz_planned_etime1, static_cast<unsigned>(_countof(sz_planned_etime1)),
			sz_planned_etime2, static_cast<unsigned>(_countof(sz_planned_etime2)),
			sz_planned_itime1, static_cast<unsigned>(_countof(sz_planned_itime1)),
			sz_planned_itime2, static_cast<unsigned>(_countof(sz_planned_itime2)),
			sz_pred_name, static_cast<unsigned>(_countof(sz_pred_name)),
			&i_valid_flag, &i_ofs_flag, &i_offset,
			sz_get_from, static_cast<unsigned>(_countof(sz_get_from)),
			sz_plan_type, static_cast<unsigned>(_countof(sz_plan_type)));

		if (nCol < 15)
		{
			// 旧フォーマット読み込み
			swscanf_s(sz_buff, L"%s%s%s%s%s%s%s%s%s%d%d%d",
				sz_satname, static_cast<unsigned>(_countof(sz_satname)),
				sz_planned_pass, static_cast<unsigned>(_countof(sz_planned_pass)),
				sz_planned_stime1, static_cast<unsigned>(_countof(sz_planned_stime1)),
				sz_planned_stime2, static_cast<unsigned>(_countof(sz_planned_stime2)),
				sz_planned_etime1, static_cast<unsigned>(_countof(sz_planned_etime1)),
				sz_planned_etime2, static_cast<unsigned>(_countof(sz_planned_etime2)),
				sz_planned_itime1, static_cast<unsigned>(_countof(sz_planned_itime1)),
				sz_planned_itime2, static_cast<unsigned>(_countof(sz_planned_itime2)),
				sz_pred_name, static_cast<unsigned>(_countof(sz_pred_name)),
				&i_valid_flag, &i_ofs_flag, &i_offset);
		}

		if (nCol == 15)
		{
			// 局名
			pld.strStationName = CString(sz_stationname);
		}

		// 衛星名
		pld.strSatelliteName = CString(sz_satname);

		// パスID
		pld.strPassId = CString(sz_planned_pass);

		// プリパス開始日時
		swprintf_s(sz_pre, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_planned_itime1, sz_planned_itime2, L".0000");
		if (ub_asc_to_asctime(sz_pre, sz_pre_date) < 0) {
			fclose(fp);
			return FALSE;
		}
		cTime = StrTimeToCTime(CString(sz_pre_date));
		pld.tPrePassStart = cTime.GetTime() + i_offset;

		// パス開始日時
		swprintf_s(sz_aos, static_cast<unsigned>(_countof(sz_aos)), L"%s %s%s", sz_planned_stime1, sz_planned_stime2, L".0000");
		if (ub_asc_to_asctime(sz_aos, sz_aos_date) < 0) {
			fclose(fp);
			return FALSE;
		}
		cTime = StrTimeToCTime(CString(sz_aos_date));
		pld.tPassStart = cTime.GetTime() + i_offset;

		// パス終了日時
		swprintf_s(sz_los, static_cast<unsigned>(_countof(sz_los)), L"%s %s%s", sz_planned_etime1, sz_planned_etime2, L".0000");
		if (ub_asc_to_asctime(sz_los, sz_los_date) < 0) {
			fclose(fp);
			return FALSE;
		}
		cTime = StrTimeToCTime(CString(sz_los_date));
		pld.tPassEnd = cTime.GetTime() + i_offset;

		// 予報値ファイル名
		pld.strPredName = CString(sz_pred_name);

		// 計画ファイル名
		CString strStPath = mStationSimpleString[0];
		for (int s = 0; s < sizeof(mStationString) / sizeof(mStationString[0]); s++){
			if (CString(sz_stationname) == mStationString[s]){
				strStPath = mStationSimpleString[s];
				break;
			}
		}
		pld.strPlanName = strStPath + _T("\\data\\plan\\exec\\") + CString(sz_satname) + _T("\\") + CString(sz_planned_pass) + _T(".pln");

		// チェック結果フラグ
		pld.lValidFlag = i_valid_flag;

		// 時刻オフセット追従条件フラグ
		pld.lOfsFlag = i_ofs_flag;

		// 時刻オフセット
		pld.tOffset = i_offset;

		if (nCol == 15)
		{
			pld.strGetFrom = CString(sz_get_from).MakeUpper();
			pld.strPlanType = CString(sz_plan_type).MakeUpper();
		}

		// リストに追加
		mGraph.mListPlanData.push_back(pld);
	}

	fclose(fp);

	return TRUE;
}

/*============================================================================*/
/*! CSatelliteData

-# テキスト型のTimeをCTime型に変換する


@param  なし
@retval 時刻(CTime)
*/
/*============================================================================*/
CTime CTimeLineView::StrTimeToCTime(CString strTime)
{
	int dot = strTime.Find(_T("."));
	if (dot != -1)
	{
		strTime = strTime.Mid(0, dot);
	}

	COleDateTime oleTime;
	if (oleTime.ParseDateTime(strTime) == false){
		return CTime(0);
	}
	int yy = oleTime.GetYear();
	if (yy <= 1970){
		return CTime(0);
	}

	CTime cTime(oleTime.GetYear(), oleTime.GetMonth(), oleTime.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());

	return cTime;
}

/*============================================================================*/
/*! CSatelliteData

-# 省略フォーマットの時刻文字列をfull-formatの時刻文字列に変換

@param  sz_time：入力時刻文字列
@            1999-07-01 11:22:33 の省略形としては以下の形式を許す
@            1999-07-01 11:22
@            1999-07-01 112233
@            1999-07-01 1122
@            1999-07-01 11
@                      .11
@                      .1122
@                      .112233
@                    01.112233
@                    01.1122
@                    01.11
@                  0701.112233
@                990701.112233
@              19990701.112233
@              19990701112233
@param	output_time:フルフォーマット出力時刻文字列
@retval	5 : yy-mm-dd hh:mm
@       6 : yy-mm-dd hh:mm:ss.s
@       7 :
@       1 : 異常終了

*/
/*============================================================================*/
int CTimeLineView::ub_asc_to_asctime(TCHAR sz_input_time[], TCHAR sz_output_time[])
{
	int i, j;
	int year, mon, mday, hour, min, sec, i_100usec;
	int i_hhmmss, i_yymmdd;
	TCHAR sz_time[40], sz_work[40], sz_dmy[40];
	TCHAR sz_yymmdd[40], sz_hhmmss[40], sz_0sec[40];
	double d_sec, d_hhmmss, d_yymmdd;
	struct tm stT;
	time_t l_time;
	int sz = 40;

	for (i = 0, sz_time[0] = NULL; sz_input_time[i] != NULL; i++){
		if (sz_input_time[i] == ' '){ continue; } /*先頭スペースサプレス*/
		wcscpy_s(sz_time, sz, &sz_input_time[i]);
		break;
	};
	if (sz_time[0] == NULL){
		return(-1);
	};

	for (i = (int)wcslen(sz_time) - 1, j = 0; i > 0; i--){
		if ((j == 0) && (sz_time[i] == ' ')){
			sz_time[i] = NULL; j = i; /*後ろのスペースサプレス*/
		}
		else if (sz_time[i] != ' '){
			j = i;
		}
	}

	if ((i = swscanf_s(sz_time, L"%d-%d-%d %d:%d:%lf%s"
		, &year, &mon, &mday, &hour, &min, &d_sec, sz_dmy, sz)) == 6){
		sec = (int)floor(d_sec);
		i_100usec = (int)((d_sec - (double)sec)*10000.0);
		if (year < 70){
			year += 2000;
		}
		else if (year < 1900){
			year += 1900;
		}
		swprintf_s(sz_output_time, sz, L"%04d-%02d-%02d %02d:%02d:%02d.%04d"
			, year, mon, mday, hour, min, sec, i_100usec);
		return(6); /* yy-mm-dd hh:mm:ss.s */
	}
	else if ((i = swscanf_s(sz_time, L"%d-%d-%d %d:%d%s"
		, &year, &mon, &mday, &hour, &min, sz_dmy, sz)) == 5){
		if (year < 70){
			year += 2000;
		}
		else if (year < 1900){
			year += 1900;
		}
		swprintf_s(sz_output_time, sz, L"%04d-%02d-%02d %02d:%02d:%02d.0000"
			, year, mon, mday, hour, min, 0);
		return(5); /* yy-mm-dd hh:mm */
	}
	else if ((i = swscanf_s(sz_time, L"%d-%d-%d %lf%s"
		, &year, &mon, &mday, &d_hhmmss, sz_dmy, sz)) == 4){
		swscanf_s(sz_time, L"%d-%d-%d %s", &year, &mon, &mday, sz_hhmmss, sz);
		if (sz_hhmmss[0] == '.'){
			swprintf_s(sz_yymmdd, sz, L"%02d%02d%02d%s", year, mon, mday, sz_hhmmss);
		}
		else{
			swprintf_s(sz_yymmdd, sz, L"%02d%02d%02d.%s", year, mon, mday, sz_hhmmss);
		}
		goto yymmdd_hhmmss_0sec; /* yy-mm-dd hhmmss */
	}
	else if (((i = swscanf_s(sz_time, L"%lf%s", &d_yymmdd, sz_dmy, sz)) == 1)
		&& (d_yymmdd >= 0101000000.0)){ /* 000101=2000-01-01 */
		i_yymmdd = (int)floor(d_yymmdd / 1000000.0);
		i_hhmmss = (int)fmod(d_yymmdd, 1000000.0);
		if (swscanf_s(sz_time, L"%[^.].%s", sz_yymmdd, sz, sz_0sec, sz) == 2){
			swprintf_s(sz_yymmdd, sz, L"%06d.%06d.%s", i_yymmdd, i_hhmmss, sz_0sec);
		}
		else{
			swprintf_s(sz_yymmdd, sz, L"%06d.%06d.0000", i_yymmdd, i_hhmmss);
		}
		goto yymmdd_hhmmss_0sec; /* yymmddhhmmss */
	}
	else if ((sz_time[0] == '.') && ((i = swscanf_s(sz_time + 1, L"%lf%s", &d_hhmmss, sz_dmy, sz)) == 1)){
		swprintf_s(sz_yymmdd, sz, L"00%s", sz_time);
		goto yymmdd_hhmmss_0sec; /* .hhmmss */
	}
	else if ((i = swscanf_s(sz_time, L"%[^.].%lf%s", sz_work, sz, &d_hhmmss, sz_dmy, sz)) == 2){
		wcscpy_s(sz_yymmdd, sz, sz_time);
		goto yymmdd_hhmmss_0sec; /* yymmdd.hhmmss */
	}
	else{

		wcscpy_s(sz_output_time, sz, sz_time);
		return(-1);
	};

yymmdd_hhmmss_0sec:

	if (swscanf_s(sz_yymmdd, L"%[^.].%[^.].%s", sz_dmy, sz, sz_hhmmss, sz, sz_0sec, sz) == 3){

		if (wcslen(sz_hhmmss) != 6){
			wcscpy_s(sz_output_time, sz, sz_time);
			return(-1);
		};
		wcscat_s(sz_0sec, sz, L"000000");
		sz_0sec[4] = NULL;
	}
	else{
		wcscpy_s(sz_0sec, sz, L"0000");

		switch (wcslen(sz_hhmmss)){
		case 2: case 4: case 6:
			wcscat_s(sz_hhmmss, sz, L"000000");
			sz_hhmmss[6] = NULL;
			break;
		default: wcscpy_s(sz_output_time, sz, sz_time);
			return(-1);
		};
	};

	l_time = time(NULL);
	if (localtime_s(&stT, &l_time) != 0)
	{
		return(1);
	}
	swscanf_s(sz_dmy, L"%d", &i_yymmdd);
	if ((mday = i_yymmdd % 100) == 0){
		mday = stT.tm_mday; /* 1..31*/
	}
	if ((mon = (i_yymmdd / 100) % 100) == 0){
		mon = stT.tm_mon + 1; /* 0..11*/
	}
	if ((year = i_yymmdd / 10000) == 0){
		year = stT.tm_year; /* n -1900*/
	}

	switch (wcslen(sz_dmy)){
	case 6: case 8:/* yymmdd */
		year = i_yymmdd / 10000;
		break;
	case 2: case 4:/* ????dd ??mmdd */
		break;
	default: wcscpy_s(sz_output_time, sz, sz_time);
		return(-1);
	};
	swscanf_s(sz_hhmmss, L"%d", &i_hhmmss);
	sec = i_hhmmss % 100;
	min = (i_hhmmss / 100) % 100;
	hour = i_hhmmss / 10000;

	if (year < 70){
		year += 2000;
	}
	else if (year < 1900){
		year += 1900;
	}

	swprintf_s(sz_output_time, sz, L"%02d-%02d-%02d %02d:%02d:%02d.%s"
		, year, mon, mday, hour, min, sec, sz_0sec);

	return(7);
}

/*============================================================================*/
/*! CSatelliteData

-# 運用計画ファイルの取得


@param  なし
@retval
*/
/*============================================================================*/
BOOL CTimeLineView::GetPlanDataFromFile()
{
	mGraph.mListTimeData.clear();
	mSelectOpePlanData = 0;

	int nSelectStation = theApp.GetSelectStation();
	int ret = 0;
	vector<stPlanListData>::iterator itr;

	for (itr = mGraph.mListPlanData.begin(); itr != mGraph.mListPlanData.end(); itr++){
		stPlandata	data;
		DWORD dw = 0;
		if ((*itr).strGetFrom.CompareNoCase(_T("ENP")) == 0)
			dw = 1;

//#if 0
//		// 運用種別が、実運用のみリストする。
//		if ((*itr).strPlanType.CompareNoCase(_T("実運用")) != 0)
//		{
//			continue;
//		}
//#else
//		if ((*itr).strPlanType.CompareNoCase(_T("実運用")) != 0 &&
//			(*itr).strPlanType.CompareNoCase(_T("VLBI")) != 0)
//		{
//			continue;
//		}
//#endif

		if (GetPlanFile((*itr).strSatelliteName, (*itr).strPassId, dw)){
			// ファイルが存在したのでデータを取得する
			switch(nSelectStation){
			case	0:			// 臼田64m
				ret = ReadPlanFile64((*itr).strSatelliteName, (*itr).strPassId, data, dw);
				break;
			case	1:			// 臼田54m
				ret = ReadPlanFile54((*itr).strSatelliteName, (*itr).strPassId, data, dw);
				break;
			case	2:			// 内之浦34m
				ret = ReadPlanFile34((*itr).strSatelliteName, (*itr).strPassId, data, dw);
				break;
			case	3:			// 内之浦20m
				ret = ReadPlanFile20((*itr).strSatelliteName, (*itr).strPassId, data, dw);
				break;
			}
			CTime m_tAOS;
			CTime m_tLOS;
			COprPlnData tdata;
			stPlandata stpd;
			tdata.m_strPlanID = (*itr).strPassId;
			GetPredTime((*itr).strSatelliteName, (*itr).strPassId, m_tAOS, m_tLOS, stpd);
			tdata.m_AntFcstValue.start = m_tAOS;
			tdata.m_AntFcstValue.end = m_tLOS;
			ConvertData(tdata, data);
			tdata.mGetForm = (dw==1)?true:false;
			tdata.m_strSatellite = (*itr).strSatelliteName;
			// プリパス（運用計画前の処理）時刻の設定
			tdata.m_PrePassTime.start = CTime((*itr).tPrePassStart);
			tdata.m_PrePassTime.end = CTime((*itr).tPassEnd);
			// ANT待ち受け指向時刻の設定
			if (data.l_antwait_time > 0 && data.l_antinit_time > 0)
			{
				if (data.l_antwait_time <= data.l_antinit_time)
				{
					tdata.m_AntWaitTime.start = CTime(data.l_antwait_time);
				}
				else
				{
					tdata.m_AntWaitTime.start = CTime(data.l_antinit_time);
				}
			}
			else if (data.l_antwait_time <= 0 && data.l_antinit_time > 0)
			{
				tdata.m_AntWaitTime.start = CTime(data.l_antinit_time);
			}
			else if (data.l_antwait_time > 0 && data.l_antinit_time <= 0)
			{
				tdata.m_AntWaitTime.start = CTime(data.l_antwait_time);
			}
			else
			{
				tdata.m_AntWaitTime.start = CTime(0);
			}
			// 設備初期設定時刻の設定
			tdata.m_InitPassTime.start = CTime(data.l_initmin_time);
			// オペレーションモードの設定
			tdata.m_strOpeMode = data.sz_ope_mode;
			// 自動生成の設定
			tdata.m_strAuto = data.sz_auto;

			mGraph.mListTimeData.push_back(tdata);
		}
		else{
			//###mGetSharePlanFileTime = 0;
		}
	}

	if (ret == 0)
	{
		if (mGraph.mListTimeData.size() > 0)
			m_bReadCompletePlanFile = true;
		else
			m_bReadCompletePlanFile = false;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


/*============================================================================*/
/*! CTimeLineView

-# 予報値一覧ファイルより、予報値時刻を取得する

@param  pc_satname	：衛星名
@param  pc_pass		：パス
@param  pc_aos_time	：AOS時刻
@param  pc_los_time	：LOS時刻
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
BOOL CTimeLineView::GetPredTime(CString pc_satname, CString pc_pass, CTime& pc_aos_time, CTime& pc_los_time, stPlandata& stpd)
{
	BOOL ret = FALSE;
	CString passid = pc_pass;
	// 計画一覧より予報値情報取得
	vector<stPlanListData>::iterator itr;

	// 先に局立案優先で局、衛星、計画IDが一致を調べる
	BOOL bFindPassId = FALSE;
	for (itr = mGraph.mListPlanData.begin(); itr != mGraph.mListPlanData.end(); itr++)
	{
		stPlanListData st = (stPlanListData)(*itr);

		if (st.strStationName == mStationString[theApp.GetSelectStation()])
		{
			if (st.strSatelliteName == pc_satname && st.strPassId == pc_pass && st.strGetFrom != _T("ENP"))
			{
				int idx = st.strPredName.Find(_T("."));
				CString strid = st.strPredName.Mid(0, idx);
				CString temp = strid;
				strid.Format(_T("%s-%s"), (LPCTSTR)temp.Mid(0, 6), (LPCTSTR)temp.Mid(6, 4));
				passid = strid;
				bFindPassId = TRUE;
				break;
			}
		}
	}

	// 局立案検索後にENPで局、衛星、計画IDが一致を調べる
	if (!bFindPassId)
	{
		for (itr = mGraph.mListPlanData.begin(); itr != mGraph.mListPlanData.end(); itr++)
		{
			stPlanListData st = (stPlanListData)(*itr);

			if (st.strStationName == mStationString[theApp.GetSelectStation()])
			{
				if (st.strSatelliteName == pc_satname && st.strPassId == pc_pass && st.strGetFrom == _T("ENP"))
				{
					int idx = st.strPredName.Find(_T("."));
					CString strid = st.strPredName.Mid(0, idx);
					CString temp = strid;
					strid.Format(_T("%s-%s"), (LPCTSTR)temp.Mid(0, 6), (LPCTSTR)temp.Mid(6, 4));
					passid = strid;
					break;
				}
			}
		}
	}

	// 予報値一覧からAos/Los取得
	vector<stPredListData>::iterator itrp;
	for (itrp = mGraph.mListPredData.begin(); itrp != mGraph.mListPredData.end(); itrp++)
	{
		stPredListData st = (stPredListData)(*itrp);
		if ((st.strSatellite == pc_satname) && (st.strPassId == passid))
		{
			pc_aos_time = CTime(st.tAOS);
			pc_los_time = CTime(st.tLOS);
			//stpd.l_offset_val = st.tOffset;
			//stpd.sz_pred_path = st.strPredPath.Right(st.strPredPath.GetLength() - st.strPredPath.ReverseFind('/') - 1);
			ret = TRUE;
			break;
		}
	}
	return ret;
}

/*============================================================================*/
/*! CTimeLineView

-# イベントを時刻順にリストに追加していく

@param	time	追加する時刻データ(I)
@param  list	追加先イベントのリスト(O)
@retval	なし
*/
/*============================================================================*/
void CTimeLineView::AddEventList(stTime time, vector<stTime>& list)
{
	size_t size = list.size();

	for (size_t i = 0; i < size; i++)
	{
		if (time.start < list[i].start)
		{
			// リストの現在位置に追加
			list.insert(list.begin() + i, time);
			return;
		}
	}

	// リストに挿入されなかったら、リスト最後尾に追加
	list.push_back(time);
}

/*============================================================================*/
/*! CTimeLineView

-# RNGイベントを時刻順にリストに追加していく

@param	time	追加する時刻データ(I)
@param	type	追加するタイプデータ(I)
@param  list	追加先イベントのリスト(O)
@param  listType	追加先イベントのタイプリスト(O)
@retval	なし
*/
/*============================================================================*/
void CTimeLineView::AddRngEventList(stTime time, CString type, vector<stTime>& list, vector<CString>& listType)
{
	size_t size = list.size();

	for (size_t i = 0; i < size; i++)
	{
		if (time.start < list[i].start)
		{
			// リストの現在位置に追加
			list.insert(list.begin() + i, time);
			listType.insert(listType.begin() + i, type);
			return;
		}
	}

	// リストに挿入されなかったら、リスト最後尾に追加
	list.push_back(time);
	listType.push_back(type);
}

/*============================================================================*/
/*! CTimeLineView

-# 運用計画データの変換

@param  tdata	運用計画データクラス
@param  data	運用計画データ構造体
@retval
*/
/*============================================================================*/
void CTimeLineView::ConvertData(COprPlnData& tdata, stPlandata data)
{
	int i;
	stTime time;
	CString str;

	tdata.m_strPlanFilePath = data.sz_filepath;
	tdata.m_strFtpPlanFilePath = data.sz_ftpfilepath;

	//#マスター計画
	str = data.sz_pred_path;
	tdata.m_strPredID = str.Left(6) + _T("-") + str.Mid(6, 4);
	tdata.m_OpetDay.start = StrTimeToCTime(data.sz_start);
	tdata.m_OpetDay.end = StrTimeToCTime(data.sz_end);

	//---------------- アップリンク ---------------------//
	tdata.m_UpLinkCnt = (int)data.sz_uplink_s_timelst.size();
	tdata.m_UpLinkChk = (tdata.m_UpLinkCnt != 0) ? true : false;
	// アップリンクを時刻順に並び替えてリストに追加していく
	for (i = 0; i < data.sz_uplink_s_timelst.size(); i++){
		time.start = StrTimeToCTime(data.sz_uplink_s_timelst[i]);
		time.end = StrTimeToCTime(data.sz_uplink_e_timelst[i]);
		AddEventList(time, tdata.m_ULDay);
	}

	//---------------- RNG ---------------------//
	tdata.m_RngCnt = (int)data.sz_rng_s_timelst.size();
	tdata.m_RngChk = (tdata.m_RngCnt != 0) ? true : false;
	// RNGを時刻順に並び替えてリストに追加していく
	for (i = 0; i < data.sz_rng_s_timelst.size(); i++){
		time.start = StrTimeToCTime(data.sz_rng_s_timelst[i]);
		time.end = StrTimeToCTime(data.sz_rng_e_timelst[i]);
		AddRngEventList(time, data.sz_rng_kindlst[i], tdata.m_RngDayList, tdata.m_RngTypeList);
	}

	//---------------- CMD ---------------------//
	tdata.m_CmdCnt = (int)data.sz_cmd_s_timelst.size();
	tdata.m_CmdChk = (tdata.m_CmdCnt != 0) ? true : false;
	// CMDを時刻順に並び替えてリストに追加していく
	for (i = 0; i < data.sz_cmd_s_timelst.size(); i++){
		time.start = StrTimeToCTime(data.sz_cmd_s_timelst[i]);
		time.end = StrTimeToCTime(data.sz_cmd_e_timelst[i]);
		AddEventList(time, tdata.m_CmdDayList);
	}

	//---------------- S-TLM ---------------------//
	str = data.sz_ope_mode;
	tdata.m_TlmSChk = (str.Find(_T("S-TLM")) >= 0 && data.sz_tlms_s_timelst.size() > 0) ? true : false;
	// S-TLMを時刻順に並び替えてリストに追加していく
	for (i = 0; i < data.sz_tlms_s_timelst.size(); i++)
	{
		time.start = StrTimeToCTime(data.sz_tlms_s_timelst[i]);
		time.end = StrTimeToCTime(data.sz_tlms_e_timelst[i]);
		AddEventList(time, tdata.m_TlmSDayList);
	}

	//---------------- X-TLM ---------------------//
	str = data.sz_ope_mode;
	tdata.m_TlmXChk = (str.Find(_T("X-TLM")) >= 0 && data.sz_tlmx_s_timelst.size() > 0) ? true : false;
	// X-TLMを時刻順に並び替えてリストに追加していく
	for (i = 0; i < data.sz_tlmx_s_timelst.size(); i++)
	{
		time.start = StrTimeToCTime(data.sz_tlmx_s_timelst[i]);
		time.end = StrTimeToCTime(data.sz_tlmx_e_timelst[i]);
		AddEventList(time, tdata.m_TlmXDayList);
	}

	//---------------- Ka-TLM ---------------------//
	str = data.sz_ope_mode;
	tdata.m_TlmKaChk = (str.Find(_T("Ka-TLM")) >= 0 && data.sz_tlmka_s_timelst.size() > 0) ? true : false;
	// Ka-TLMを時刻順に並び替えてリストに追加していく
	for (i = 0; i < data.sz_tlmka_s_timelst.size(); i++)
	{
		time.start = StrTimeToCTime(data.sz_tlmka_s_timelst[i]);
		time.end = StrTimeToCTime(data.sz_tlmka_e_timelst[i]);
		AddEventList(time, tdata.m_TlmKaDayList);
	}

	tdata.m_AntStandby = data.l_antwait_flg;
	if (data.l_ant_stow == 0)	tdata.m_AntZenith = 1;
	if (data.l_ant_stow == 1)	tdata.m_AntZenith = 2;
	if (data.l_ant_stow == 2)	tdata.m_AntZenith = 0;

	tdata.m_OpInit = data.l_init;
	tdata.m_OpTDP = data.l_tx_ramp;
	tdata.m_OpRDP = data.l_rx_ramp;
	tdata.m_OpTOutType = data.l_tx_pwr_sel;
	tdata.m_OpTOut = 0;	//#

	tdata.m_AgcChkValue = (data.l_agc_index == 1) ? true : false;
	tdata.m_AgcTime     = (data.l_agc_index == 1) ? CTime(data.l_agc_time) : CTime(0);
	tdata.m_RndChkValue = (data.l_rng_index == 1) ? true : false;
	tdata.m_RngTime     = (data.l_rng_index == 1) ? CTime(data.l_rng_time) : CTime(0);
	tdata.m_TlmChkValue = (data.l_tlm_index == 1) ? true : false;
	tdata.m_TlmTime     = (data.l_tlm_index == 1) ? CTime(data.l_tlm_time) : CTime(0);
	tdata.m_CmdChkValue = (data.l_cmd_index == 1) ? true : false;
	tdata.m_CmdTime     = (data.l_cmd_index == 1) ? CTime(data.l_cmd_time) : CTime(0);

	tdata.m_strCID = data.sz_cid;

	tdata.m_OpPriority = data.sz_priority;

	tdata.m_strPlanType = data.sz_plan_type;
}

/*============================================================================*/
/*! CTimeLineView

-# 監視データを取得し、変更を調べる

@param
@param
@retval true：変化あり/false変化なし、あるいは取得できない
*/
/*============================================================================*/
bool CTimeLineView::GetKanshiData()
{
	const CString mPLAN_NO = _T("CTRL.PASS_NO");// 計画番号
	const CString mEISEI_NAME = _T("CTRL.SAT_NAME"); //衛星名
	const CString mPREDFILE_NAME = _T("CTRL.PRED_FILE"); //予報値ファイルパス名

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	UINT nStation = theApp.GetSelectStation();
	bool ret = false;

	// 監視データから予報値ファイル名を取得する
	obs_adr = (CEngValCnv::obsname_t *)theApp.GetObsname_t(nStation, mPREDFILE_NAME);
	//	mPredFile = _T("");
	mPredFileAnp = _T("");
	if (obs_adr == NULL){
		// 予報値ファイル名が取得できない
		mPredFile = _T("");
//#ifdef _DEBUG
////		mPredFile = _T("C:/share/udsc64/ctrl/prd/geotail/1710020100.prd");//###
////		mPredFile = _T("C:/share/udsc64/ctrl/prd/vlbi/1710259990.prd");//###
////		mPredFile = _T("C:/share/udsc64/ctrl/prd/hayabusa2/1805240001.prd");//###
////		mPredFile = _T("C:/share/udsc64/ctrl/prd/geotail/1712159999.prd");//###
////		mPredFile = _T("C:/share/udsc34/ctrl/prd/planet-c/1712050001.prd");//###
//#else
		mSelectForecast = -1;
//#endif
	}
	else{
		CString strPredFile;
		strPredFile.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
		if (mPredFile.CompareNoCase(strPredFile) != 0)
		{
			mPredFile = strPredFile;
			ret = true;
		}
	}

	if (mPredFile.Left(1) == _T(" ") || mPredFile.IsEmpty())
	{
		m_strForecast = _T("----------");
	}
	else
	{
		// ファイル名切り出し
		CString strFileName = mPredFile.Right(mPredFile.GetLength() - mPredFile.ReverseFind(_T('/')) - 1);
		// 拡張子削除
		CString strPredId = strFileName.Left(strFileName.ReverseFind(_T('.')));
		m_strForecast = strPredId.Left(6) + _T("-") + strPredId.Mid(6, 4);
	}

	// 監視データから衛星名を取得する
	obs_adr = (CEngValCnv::obsname_t *)theApp.GetObsname_t(nStation, mEISEI_NAME);
	//	mSataName = _T("");
	if (obs_adr == NULL){
		// 衛星名が取得できない
		mSataName = _T("");
//#ifdef _DEBUG
////		mSataName = _T("VLBI");//###
////		mSataName = _T("MAINT");//###
////		mSataName = _T("GEOTAIL");//###
//		mSataName = _T("HAYABUSA2");//###
//#endif
	}
	else{
		CString strSataName;
		strSataName.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
#if 0
		// 監視の衛星名がVLBIまたはMAINTの場合は何もしないため、衛星名を空にする
		if (strSataName.CompareNoCase(_T("VLBI")) == 0 ||
			strSataName.CompareNoCase(_T("MAINT")) == 0)
		{
			strSataName = _T("");
		}
#endif
		if (mSataName.CompareNoCase(strSataName) != 0)
		{
			mSataName = strSataName;
			ret = true;

			// 衛星が変ったため、初期設定プロパティの入力欄を更新無しにする
			m_bRecLevelSModified = false;
			m_bRecLevelXModified = false;
			m_bSubCarTlmModified = false;
			m_bSubCarCmdModified = false;
		}
	}

	if (mSataName.Left(1) == _T(" ") || mSataName.IsEmpty())
	{
		m_strSpaceName = _T("----------");
	}
	else
	{
		m_strSpaceName = mSataName;
	}

	// 監視データから計画IDを取得する
	obs_adr = (CEngValCnv::obsname_t *)theApp.GetObsname_t(nStation, mPLAN_NO);
	//	mPlanId = _T("");
	if (obs_adr == NULL){
		// 計画IDが取得できない
		mPlanId = _T("");
//#ifdef _DEBUG
//		mPlanId = _T("180524-0001");//###
////		mPlanId = _T("171215-9999");//###
//#endif
	}
	else{
		CString strPlanId;
		strPlanId.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));

		//#ifdef _DEBUG
		//		if (mbPlanIDChged)
		//			strPlanId = _T("170828-0006");
		//#endif

		if (mPlanId.CompareNoCase(strPlanId) != 0)
		{
			mPlanId = strPlanId;
			ret = true;
		}
	}

	if (mPlanId.Left(1) == _T(" ") || mPlanId.IsEmpty())
	{
		m_strPlanID = _T("----------");
	}
	else
	{
		m_strPlanID = mPlanId;
	}

	const CString mSTART_TIME = _T("CTRL.START_TIME");	// 運用開始日時
	const CString mEND_TIME = _T("CTRL.END_TIME");		// 運用終了日時

	// 監視データから運用開始日時を取得する
	obs_adr = (CEngValCnv::obsname_t *)theApp.GetObsname_t(nStation, mSTART_TIME);
	if (obs_adr == NULL){
		// 開始日時が取得できない
		mStartTime = 0;
//#ifdef _DEBUG
//		//			mStartTime = StrTimeToCTime(_T("2017-10-03 05:00:00"));
//		mStartTime = StrTimeToCTime(_T("2018-12-24 00:34:00"));
//		//			mStartTime = StrTimeToCTime(_T("2017-12-05 06:00:00"));
//#endif
	}
	else{
		char	  ca_stime[40];				/* パス運用開始時刻（フル）   */
		ub_asc_to_asctime(obs_adr->sz_sts_name, ca_stime);

		CString strTime = _T("");
		CString tmp = CString(ca_stime);
		int idx = 0;
		if ((idx = tmp.Find(_T("."))) != -1)
		{
			strTime = tmp.Mid(0, idx);
		}
		else
		{
			strTime = tmp;
		}
		CTime tmpTime = StrTimeToCTime(strTime);
		if (mStartTime.GetTime() != tmpTime.GetTime())
		{
			// 開始時刻が変更の場合、ファイルを読み直す
			ret = true;
			mStartTime = tmpTime;
		}
	}

	if (mStartTime > 0)
	{
		m_strOpeStart = (mStartTime).Format(_T("%Y-%m-%d %H:%M:%S"));
	}
	else
	{
		m_strOpeStart = _T("-------------------");
	}

	// 監視データから運用終了日時を取得する
	obs_adr = (CEngValCnv::obsname_t *)theApp.GetObsname_t(nStation, mEND_TIME);
	if (obs_adr == NULL){
		// 終了日時が取得できない
		mEndTime = 0;
//#ifdef _DEBUG
//		//			mEndTime = StrTimeToCTime(_T("2017-10-03 10:00:00"));
//		mEndTime = StrTimeToCTime(_T("2018-12-24 10:55:00"));
//		//			mEndTime = StrTimeToCTime(_T("2017-12-05 06:00:00"));
//#endif
	}
	else{
		char	  ca_stime[40];				/* パス運用開始時刻（フル）   */
		ub_asc_to_asctime(obs_adr->sz_sts_name, ca_stime);

		CString strTime = _T("");
		CString tmp = CString(ca_stime);
		int idx = 0;
		if ((idx = tmp.Find(_T("."))) != -1)
		{
			strTime = tmp.Mid(0, idx);
		}
		else
		{
			strTime = tmp;
		}
		CTime tmpTime = StrTimeToCTime(strTime);
		if (mEndTime.GetTime() != tmpTime.GetTime())
		{
			// 終了時刻が変更の場合、ファイルを読み直す
			ret = true;
			mEndTime = tmpTime;
		}
		//#ifdef _DEBUG
		//			if (mbEndTimeChged)
		//			{
		//				mEndTime += 3600;
		//			}
		//#endif
	}

	if (mEndTime > 0)
	{
		m_strOpeStop = (mEndTime).Format(_T("%Y-%m-%d %H:%M:%S"));
	}
	else
	{
		m_strOpeStop = _T("-------------------");
	}

	return ret;
}


/*============================================================================*/
/*! CTimeLineView

-# 監視データの予報値ファイルを取得して、現在の予報値ファイルから変化があった場合はtrueを返す

@param
@param
@retval true：変化あり/false変化なし、あるいは取得できない
*/
/*============================================================================*/
bool CTimeLineView::GetPredMonitorFile()
{
	bool ret = false;

	// 予報値ファイルフルパス名（予報値ファイル名）
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("GetPredMonitorFile"), _T("Pred file name"), mPredFile, nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	CString satName;
	CString anpName;
	CString anpFile;

	CString strFilePath = mPredFile;
	strFilePath.Trim(_T(" "));
	if (strFilePath.IsEmpty()){
		// ファイル名が存在しない
		return false;
	}

	// 拡張子がprdの場合、anpに置き換える
	if (strFilePath.Right(3).MakeLower() == PRED_FILE_TYPE){
		strFilePath = strFilePath.Left(strFilePath.GetLength() - 3);
		strFilePath += ANP_FILE_TYPE;
	}

	// ファイル名と衛星名を切り出す
	int i = 0;
	CString str;
	anpName.Empty();
	satName.Empty();
	while (AfxExtractSubString(str, strFilePath, i++, '/')){
		satName = anpName;
		anpName = str;
	}
	mPredFileAnp = anpName;

	// 予報値ファイル名の作成
	strFilePath = theApp.GetShareFilePath(eFileType_Pred_Folder, theApp.GetSelectStation());
	anpFile.Format(_T("%s%s\\%s"), (LPCTSTR)strFilePath, (LPCTSTR)satName, (LPCTSTR)anpName);
	if (PathFileExists(anpFile) == FALSE)
	{
		// ファイルが存在しない
		return false;
	}

	CString predid = ConvertPredfileToId(anpFile);
	vector<stPredListData>::iterator itr;
	i = 0;
	mSelectForecast = -1;
	mSelectForecastId.Empty();
	//###
	for (itr = mGraph.mListPredData.begin(); itr != mGraph.mListPredData.end(); itr++, i++){
		if ((*itr).strPassId.CompareNoCase(predid) == 0 &&
			(*itr).strSatellite.CompareNoCase(satName) == 0){
			mSelectForecast = i;
			mSelectForecastId = predid;
			break;
		}
	}

	if (mSelectForecastName.CompareNoCase(anpFile) == 0){
		// 予報値ファイル名は変化なし
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("GetPredMonitorFile"), _T("No Change Pred file name"), mPredFile, nLogEx::debug);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
//		return false;
		return ret;
	}
	else{
		ret = true;
	}


	mSelectForecastName = anpFile;

	return ret;
}

/*============================================================================*/
/*! CTimeLineView

-# 計画ファイルのGET

@param  path:ファイルパス（衛星名）
@param  file:ファイル名（*.pln）
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
BOOL CTimeLineView::GetPlanFile(CString path, CString file, LPARAM lParam)
{
	// ftp? 計画ファイルを取得
	CString strPFile;
#if 1
	CString filename = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
	if (lParam == 1)
		strPFile.Format(_T("%senp\\%s\\%s.pln"), (LPCTSTR)filename, (LPCTSTR)path, (LPCTSTR)file);
	else
		strPFile.Format(_T("%ssttn\\%s\\%s.pln"), (LPCTSTR)filename, (LPCTSTR)path, (LPCTSTR)file);
#else
	strPFile.Format(_T("%s%s%s\\%s.pln"), theApp.GetAppPath(), PLAN_FILE_DIR, path, file);
#endif

	if (PathFileExists(strPFile) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

/*============================================================================*/
/*! CTimeLineView

-# 計画ファイルを読み込む

@param  なし
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
int CTimeLineView::ReadPlanFile64(CString sz_satname, CString sz_passno, stPlandata& stpd, LPARAM lParam)
{
	FILE	*fp_plan;
	CString sz_p_file;
	CString sz_p_ftpfile;

	TCHAR sz_s_start[50];
	TCHAR sz_buff[512];
	TCHAR sz_work[256];
	TCHAR sz_date[50] = { 0 };
	TCHAR sz_time[50] = { 0 };
//	TCHAR sz_txrfpwr[3][20];

	TCHAR sz_work2[256];
	TCHAR sz_work3[256];
	TCHAR sz_work4[256];
	TCHAR sz_work5[256];
	TCHAR sz_work6[256];
	TCHAR sz_work7[256];
	TCHAR sz_work8[256];

	long i;
	long j;
	long l;
	long k;
	long l_rc;

	__int64 d_s_start;
	__int64 d_time;

	CString str;
	CString sz_txcomp;
	CString strTime;

	TCHAR sz_ele[10][100];
	memset(&sz_ele[0], 0, static_cast<unsigned>(_countof(sz_ele)));

#if 1
	CString strPlanPath = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
	CString strFtpPlanPath = theApp.GetShareFilePath(eFileType_PlanOut_Folder, theApp.GetSelectStation());
	if (lParam == 1){
		sz_p_file.Format(_T("%senp\\%s\\%s.pln"), (LPCTSTR)strPlanPath, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
		sz_p_ftpfile.Format(_T("%s%s\\%s.pln"), (LPCTSTR)strFtpPlanPath, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	}
	else{
		sz_p_file.Format(_T("%ssttn\\%s\\%s.pln"), (LPCTSTR)strPlanPath, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
		sz_p_ftpfile.Format(_T("%s%s\\%s.pln"), (LPCTSTR)strFtpPlanPath, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	}
#else
	CString ca_cthome;
	ca_cthome.Format(_T("%s%s"), theApp.GetAppPath(), DATA_DIR);

	j = (long)strlen(sd.satinfo.sz_plan_dir);
	if (sd.satinfo.sz_plan_dir[j - 1] == '/')
	{
		sz_p_file.Format(_T("%s%s.pln"), CString(sd.satinfo.sz_plan_dir), sz_passno);
	}
	else
	{
		sz_p_file.Format(_T("%s\\%s.pln"), CString(sd.satinfo.sz_plan_dir), sz_passno);
	}
#endif
	sz_s_start[0] = '\0';


	/*----- ftp file get  "sz_p_file" -----*/
	CString path(strPlanPath);
	CString file(sz_p_file);

	//	if (GetPlanFile(path, file) == FALSE)
	//	{
	//		return -1;
	//	}

	//if (_wfopen_s(&fp_plan, sz_p_file, _T("r, ccs=UTF-8")) != 0)
	if (_wfopen_s(&fp_plan, sz_p_file, _T("r")) != 0)
	{
		return -1;
	}

	stpd.sz_pred_path = _T("");
	stpd.sz_filepath = _T("");
	stpd.sz_ftpfilepath = _T("");
	stpd.sz_start = _T("");
	stpd.sz_end = _T("");
	stpd.sz_s_bitrate = _T("");
	stpd.sz_x_bitrate = _T("");
	stpd.sz_x_qpsk = _T("");
	stpd.sz_cmd_bit_rate = _T("");
	stpd.sz_x_cmd_bit_rate = _T("");
	stpd.l_cmd_mod = 0;
	stpd.l_rng_mod = 0;
	stpd.sz_rng_band = _T("");
	stpd.sz_rng_start = _T("");
	stpd.sz_rng_end = _T("");
	stpd.l_rdychk = 0;
	stpd.l_init = 0;
	stpd.l_auto_acq = 0;
	stpd.l_offset = 0;
	stpd.l_a_flag = 0;
	stpd.l_l_flag = 0;
	stpd.l_term_ant = 1;
	stpd.l_tx_ramp = 0;
	stpd.l_rx_ramp = 0;
	stpd.l_acq_route = 0;
	stpd.l_pl_sel = 0;
	stpd.l_top_mon = 0;
	stpd.l_ent_angle = 0;
	stpd.l_ext_angle = 0;
	stpd.sz_level_s = _T("");
	stpd.sz_level_x = _T("");
	stpd.sz_ant_s = _T("");
	stpd.sz_ant_x = _T("");
	stpd.l_delay_sel = 0;
	stpd.l_tx_pwr_sel = 0;
	stpd.l_mod_sel = 0;
	stpd.l_tx_uc_sel = -1;
	stpd.l_antwait_flg = 0;
	stpd.l_cmd_index = 0;
	stpd.l_tlm_index = 0;
	stpd.l_agc_index = 0;
	stpd.l_rng_index = 0;
	stpd.l_range_band = 0;
	stpd.l_ant_stow = 2;
	stpd.sz_cid = _T("");
	stpd.sz_station = _T("");
	stpd.sz_priority = _T("M");
	stpd.sz_ope_mode = _T("");
	stpd.l_uplink = 0;
	stpd.sz_uplink_s_timelst.clear();
	stpd.sz_uplink_e_timelst.clear();
	stpd.l_rng = 0;
	stpd.sz_rng_s_timelst.clear();
	stpd.sz_rng_e_timelst.clear();
	stpd.sz_rng_kindlst.clear();
	stpd.l_cmd = 0;
	stpd.sz_cmd_s_timelst.clear();
	stpd.sz_cmd_e_timelst.clear();
	stpd.l_tlms = 0;
	stpd.sz_tlms_s_timelst.clear();
	stpd.sz_tlms_e_timelst.clear();
	stpd.l_tlmx = 0;
	stpd.sz_tlmx_s_timelst.clear();
	stpd.sz_tlmx_e_timelst.clear();
	stpd.l_tlmka = 0;
	stpd.sz_tlmka_s_timelst.clear();
	stpd.sz_tlmka_e_timelst.clear();

	// 「ANT待ち受け指向」「設備初期設定」内の最小時刻を調べる
	stpd.l_antwait_time = 0;
	stpd.l_antinit_time = 0;
	stpd.l_initmin_time = 0;

	__int64 d_tm;

	d_s_start = 0;
	d_time = 0;

	stpd.sz_filepath = sz_p_file;
	stpd.sz_ftpfilepath = sz_p_ftpfile;

	// 計画ファイルタイプ
	stpd.sz_plan_type = _T("");

	// レディネスチェック時間
	stpd.l_agc_time = LONG64_MAX;
	stpd.l_rng_time = LONG64_MAX;
	stpd.l_tlm_time = LONG64_MAX;
	stpd.l_cmd_time = LONG64_MAX;

	char sz_line[512];
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_plan) != 0)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			if (wcsncmp(sz_buff, SOS_HEADER_ID, wcslen(SOS_HEADER_ID)) == 0)
			{
				swscanf_s(sz_buff, _T("%s%s%s%s%s%s%s%s"),
					sz_work, static_cast<unsigned>(_countof(sz_work)),
					sz_work2, static_cast<unsigned>(_countof(sz_work2)),
					sz_work3, static_cast<unsigned>(_countof(sz_work3)),
					sz_work4, static_cast<unsigned>(_countof(sz_work4)),
					sz_work5, static_cast<unsigned>(_countof(sz_work5)),
					sz_work6, static_cast<unsigned>(_countof(sz_work6)),
					sz_work7, static_cast<unsigned>(_countof(sz_work7)),
					sz_work8, static_cast<unsigned>(_countof(sz_work8)));

				// 計画タイプ
				if (stpd.sz_plan_type.IsEmpty() == true)
					stpd.sz_plan_type = CString(sz_work2);

				// 予報値ファイル名
				if (stpd.sz_pred_path.IsEmpty() == true)
					stpd.sz_pred_path = CString(sz_work8);

			}

			l_rc = swscanf_s(&sz_buff[1], _T("%s%s%s%s%s%s%s%s%s%s"),
				sz_ele[0], static_cast<unsigned>(_countof(sz_ele[0])),
				sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])),
				sz_ele[2], static_cast<unsigned>(_countof(sz_ele[2])),
				sz_ele[3], static_cast<unsigned>(_countof(sz_ele[3])),
				sz_ele[4], static_cast<unsigned>(_countof(sz_ele[4])),
				sz_ele[5], static_cast<unsigned>(_countof(sz_ele[5])),
				sz_ele[6], static_cast<unsigned>(_countof(sz_ele[6])),
				sz_ele[7], static_cast<unsigned>(_countof(sz_ele[7])),
				sz_ele[8], static_cast<unsigned>(_countof(sz_ele[8])),
				sz_ele[9], static_cast<unsigned>(_countof(sz_ele[9])));

			for (i = 0; i < l_rc; ++i)
			{
//				str = CString(sz_ele[i]);
				AfxExtractSubString(str, CString(sz_ele[i]), 0, '=');

				/*---------- 初期値 チェック ----------*/
				if (str.Find(sz_c_ele64[2]) >= 0)
				{
					j = sz_c_ele64[2].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_init = k;
							break;
						}
					}
				}

				/*---------- CMD MOD の AUTO/MANチェック ----------*/
				if (str.Find(sz_c_ele64[6]) >= 0)
				{
					j = sz_c_ele64[6].GetLength() + 1;
					for (k = 0; sz_auto[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_auto[k])
						{
							stpd.l_cmd_mod = k;
							break;
						}
					}
				}
				/*---------- RNG MOD の AUTO/MANチェック ----------*/
				if (str.Find(sz_c_ele64[7]) >= 0)
				{
					j = sz_c_ele64[7].GetLength() + 1;
					for (k = 0; sz_auto[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_auto[k])
						{
							stpd.l_rng_mod = k;
							break;
						}
					}
				}
				/*---------- RNG BAND の 取り込み ----------*/
				if (str.Find(sz_c_ele64[8]) >= 0)
				{
					j = sz_c_ele64[8].GetLength() + 1;
					stpd.sz_rng_band = CString(&sz_ele[i][j]);
				}
				/*---------- RNG START の 取り込み ----------*/
				if (str.Find(sz_c_ele64[9]) >= 0)
				{
					j = sz_c_ele64[9].GetLength() + 1;
					stpd.sz_rng_start = CString(&sz_ele[i][j]);
				}
				/*---------- RNG END の 取り込み ----------*/
				if (str.Find(sz_c_ele64[10]) >= 0)
				{
					j = sz_c_ele64[10].GetLength() + 1;
					stpd.sz_rng_end = CString(&sz_ele[i][j]);
				}
				/*---------- 時刻オフセットへの追従 ----------*/
				if (str.Find(sz_c_ele64[11]) >= 0)
				{
					j = sz_c_ele64[11].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_offset = k;
							break;
						}
					}
				}
				/*---------- AOS FLAG ----------*/
				if (str.Find(sz_c_ele64[14]) >= 0)
				{
					j = sz_c_ele64[14].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_a_flag = k;
							break;
						}
					}
				}
				/*---------- LOS FLAG ----------*/
				if (str.Find(sz_c_ele64[15]) >= 0)
				{
					j = sz_c_ele64[15].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_l_flag = k;
							break;
						}
					}
				}
				/*---------- 自動捕捉の有無チェック ----------*/
				if (str.Find(sz_c_ele64[16]) >= 0)
				{
					j = sz_c_ele64[16].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_auto_acq = k;
							break;
						}
					}
				}
				/*---------- パス終了時のＡＮＴモード ----------*/
				if (str.Find(sz_c_ele64[18]) >= 0)
				{
					j = sz_c_ele64[18].GetLength() + 1;
					for (k = 0; sz_term_ant[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_term_ant[k])
						{
							stpd.l_term_ant = k;
							break;
						}
					}
				}
				/*---------- 送信設備ランプ制御有無 ----------*/
				if (str.Find(sz_c_ele64[19]) >= 0)
				{
					j = sz_c_ele64[19].GetLength() + 1;
					for (k = 0; sz_tx_ramp[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_tx_ramp[k])
						{
							stpd.l_tx_ramp = k;
							break;
						}
					}
				}
				/*---------- 受信設備ランプ制御有無 ----------*/
				if (str.Find(sz_c_ele64[20]) >= 0)
				{
					j = sz_c_ele64[20].GetLength() + 1;
					for (k = 0; sz_rx_ramp[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_rx_ramp[k])
						{
							stpd.l_rx_ramp = k;
							break;
						}
					}
				}
				/*---------- 補足ルート ----------*/
				if (str.Find(sz_c_ele64[21]) >= 0)
				{
					j = sz_c_ele64[21].GetLength() + 1;
					stpd.l_acq_route = _wtoi(&sz_ele[i][j]);
					stpd.l_acq_route -= 1;	/* 配列番号に変換 */
				}
				/*---------- ACQ/MAIN判定有無 ----------*/
				if (str.Find(sz_c_ele64[22]) >= 0)
				{
					j = sz_c_ele64[22].GetLength() + 1;
					for (k = 0; sz_on_off[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on_off[k])
						{
							stpd.l_pl_sel = k;
							break;
						}
					}
				}
				/*---------- 天頂通過監視有無 ----------*/
				if (str.Find(sz_c_ele64[23]) >= 0)
				{
					j = sz_c_ele64[23].GetLength() + 1;
					for (k = 0; sz_on_off[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on_off[k])
						{
							stpd.l_top_mon = k;
							break;
						}
					}
				}
				/*---------- 天頂通過監視入角 ----------*/
				if (str.Find(sz_c_ele64[24]) >= 0)
				{
					j = sz_c_ele64[24].GetLength() + 1;
					stpd.l_ent_angle = _wtoi(&sz_ele[i][j]);
				}
				/*---------- 天頂通過監視出角 ----------*/
				if (str.Find(sz_c_ele64[25]) >= 0)
				{
					j = sz_c_ele64[25].GetLength() + 1;
					stpd.l_ext_angle = _wtoi(&sz_ele[i][j]);
				}
				/*---------- 再生測距対応　局内ディレイ値選択 ----------*/
				if (str.Find(sz_c_ele64[33]) >= 0)
				{
					j = sz_c_ele64[33].GetLength() + 1;
					for (k = 0; sz_delay_sel[k] != _T(""); ++k) {
						if (CString(&sz_ele[i][j]) == sz_delay_sel[k])
						{
							stpd.l_delay_sel = k;
							break;
						}
					}
				}

				/*---------- ANT天頂指向有無 ----------*/
				if (str.Find(sz_c_ele64[46]) >= 0)
				{
					j = sz_c_ele64[46].GetLength() + 1;
					for (k = 0; sz_ant_stow[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_ant_stow[k])
						{
							stpd.l_ant_stow = k;
							break;
						}
					}
				}

				/*---------- CID ----------*/
				if (str.Find(sz_c_ele64[47]) >= 0)
				{
					j = sz_c_ele64[47].GetLength() + 1;
					stpd.sz_cid = CString(&sz_ele[i][j]);
				}

				/*---------- 局名 ----------*/
				if (str.Find(sz_c_ele64[48]) >= 0)
				{
					j = sz_c_ele64[48].GetLength() + 1;
					stpd.sz_station = CString(&sz_ele[i][j]);
				}

				/*---------- 主従局 ----------*/
				if (str.Find(sz_c_ele64[49]) >= 0)
				{
					j = sz_c_ele64[49].GetLength() + 1;
					stpd.sz_priority = CString(&sz_ele[i][j]);
				}

				/*---------- オペレーションモード ----------*/
				if (str.Find(sz_c_ele64[50]) >= 0)
				{
					j = sz_c_ele64[50].GetLength() + 1;
					stpd.sz_ope_mode = CString(&sz_ele[i][j]);
				}

				/*---------- 自動生成 ----------*/
				if (str.CompareNoCase(sz_c_ele64[61]) == 0)
				{
					j = sz_c_ele64[61].GetLength() + 1;
					stpd.sz_auto = CString(&sz_ele[i][j]);
				}

			}
			continue;
		}

		swscanf_s(sz_buff, _T("%s%s%s"),
			sz_date, static_cast<unsigned>(_countof(sz_date)),
			sz_time, static_cast<unsigned>(_countof(sz_time)),
			sz_work, static_cast<unsigned>(_countof(sz_work)));

		// 「ANT待ち受け指向」「設備初期設定」内の最小時刻を調べる
		CString temp = CString(sz_buff);
		if (temp.Find(_T("INIT.")) >= 0){
			// 「設備初期設定」が見つかった
			CString work;
			work.Format(_T("%s %s"), sz_date, sz_time);
			CTime ct = StrTimeToCTime(work);
			if (ct != CTime(0) && stpd.l_initmin_time != 0 && ct.GetTime() < stpd.l_initmin_time){
				stpd.l_initmin_time = ct.GetTime();
			} else if (ct != CTime(0) && stpd.l_initmin_time == 0){
				stpd.l_initmin_time = ct.GetTime();
			}
		}


		j = 0;
		l = 0;
		for (k = 0; sz_work[k] != '\0'; ++k)
		{
			if (sz_work[k] == ',')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_work[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
				break;
			}
			else if (sz_work[k] != ' ')
			{
				sz_ele[j][l] = sz_work[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			continue;
		}

		/* X帯送受信ルート判定文字列を探す */
		str = CString(sz_ele[0]);
		if (str.Find(X_ROUTE_DEF_TXT) >= 0)
		{
			stpd.l_route_mode = 1;
		}

		for (k = 0; sz_c_ele64[k] != _T(""); ++k)
		{
			if ((k == 2) || (k == 4) || (k == 35))
			{	/* 判定ケースを追加 */
				if (sz_c_ele64[k].Find(str) >= 0)
				{
					break;
				}
			}
			else
			{
				if (str == sz_c_ele64[k])
				{
					break;
				}
			}
		}

		if (sz_c_ele64[k] == _T(""))
		{
			continue;
		}

		swprintf_s(sz_work, static_cast<unsigned>(_countof(sz_work)), _T("%s %s"), sz_date, sz_time);
		if (d_s_start == 0.0 || d_s_start > d_time)
		{
			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), _T(" "));
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_time);
			d_s_start = d_time;
		}

		switch (k)
		{
			/*----- レディネスチェック処理 -----*/
		case 0:
			(stpd.l_rdychk) |= 0x01;
			break;

		case 1:
			(stpd.l_rdychk) |= 0x02;
			break;

			/*----- 初期設定処理 -----*/
		case 2:
			stpd.l_init = 1;
			break;

			/*----- ビットレートＳ -----*/
		case 3:
			stpd.sz_s_bitrate = CString(sz_ele[1]);
			break;

			/*----- ビットレートＸ -----*/
		case 4:
			stpd.sz_x_bitrate = CString(sz_ele[1]);
			break;

			/*----- ビットレートＸＱＰＳＫ -----*/
		case 17:
			stpd.sz_x_qpsk = CString(sz_ele[1]);
			break;

			/*----- S/X コマンドビットレート -----*/
		case 5:
			stpd.sz_cmd_bit_rate = CString(sz_ele[1]);
			break;

			/*----- X コマンドビットレート -----*/
		case 36:
		case 43:
			stpd.sz_x_cmd_bit_rate = CString(sz_ele[1]);
			break;


			/*----- 開始時刻処理 -----*/
		case 12:
			stpd.sz_start.Format(_T("%s %s"), sz_date, sz_time);

			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), _T(" "));
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_time);

			TCHAR sz_lwork[50];
			__int64 d_work;
			__int64 l_tm;
			swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s.0000"), sz_s_start);

			d_work = StrTimeToCTime(CString(sz_lwork)).GetTime();
			l_tm = d_work - d_tm;
			stpd.l_ant_time = (long)(l_tm / 60);
			break;

			/*----- 終了時刻処理 -----*/
		case 13:
			stpd.sz_end.Format(_T("%s %s"), sz_date, sz_time);
			break;

			/*----- Ｓ帯衛星送信情報処理 -----*/
		case 28:
		{
			/* 送信レベルとアンテナ選択情報に分ける */
			CString str = CString(sz_ele[1]);
			int idx = str.Find(_T("-"));
			stpd.sz_level_s = str.Mid(0, idx);
			stpd.sz_ant_s = str.Mid(idx + 1);
		}
		break;

		/*----- Ｘ帯衛星送信情報処理 -----*/
		case 29:
		{
			/* 送信レベルとアンテナ選択情報に分ける */
			CString str = CString(sz_ele[1]);
			int idx = str.Find(_T("-"));
			stpd.sz_level_x = str.Mid(0, idx);
			stpd.sz_ant_x = str.Mid(idx + 1);
		}
		break;

		/*----- ４ＷＡＹ運用有無情報処理 -----*/
		case 30:
			if (wcsncmp(sz_ele[1], CTRL_4WAY, wcslen(CTRL_4WAY)) == 0)
			{
				stpd.l_selene_4way = 0;
			}
			else if (wcsncmp(sz_ele[1], _T("ANT"), wcslen(_T("ANT"))) == 0)
			{
				TCHAR sz_lwork[50];
				stpd.l_antwait_flg = 1;
				swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s %s.0000"), sz_date, sz_time);
				d_tm = StrTimeToCTime(CString(sz_lwork)).GetTime();
				stpd.l_antinit_time = d_tm;
			}
			break;

			/*----- S-TX 送信出力選択処理 -----*//* 2006-11-08 追加 */
		case 31:
			for (i = 0; sz_tx_pwr_sel64[i] != (char*)NULL; i++)
			{
				if (wcscmp(sz_tx_pwr_sel64[i], sz_ele[1]) == 0)
				{
					stpd.l_tx_pwr_sel = i;
					break;
				}
			}
			break;

			/*----- X-TX 送信出力選択処理 -----*//* 2006-11-15 追加 */
		case 32:
			for (i = 0; sz_tx_pwr_sel64[i] != (char*)NULL; i++)
			{
				if (wcscmp(sz_tx_pwr_sel64[i], sz_ele[1]) == 0)
				{
					stpd.l_tx_pwr_sel = i;
					break;
				}
			}
			break;

			/*----- Ｘ帯変調装置選択処理 -----*//* 2009-02-06 追加 */
		case 35:
			for (i = 0; sz_mod_sel[i] != (char*)NULL; i++)
			{
				if (wcscmp(sz_mod_sel[i], sz_ele[1]) == 0)
				{
					stpd.l_mod_sel = i;
					break;
				}
			}
			break;

		case 37: //レディネスチェック（AGC)
//			if ((CString(sz_ele[1]) == STR_S_RX_A) ||
//				(CString(sz_ele[1]) == STR_X_RX2_A) ||
//				(CString(sz_ele[1]) == STR_DSN_MODEM_A))
			{
				stpd.l_agc_index = 1;
				// もっとも古い時刻を調べる
				CTime ct = StrTimeToCTime(CString(sz_work));
				time_t rdy_t = ct.GetTime();
				if (rdy_t < stpd.l_agc_time)
				{
					stpd.l_agc_time = rdy_t;
				}
			}
			break;

		case 38: //レディネスチェック（RNG)
//			if ((CString(sz_ele[1]) == STR_SS) ||
//				(CString(sz_ele[1]) == STR_SX) ||
//				(CString(sz_ele[1]) == STR_XX) ||
//				(CString(sz_ele[1]) == STR_RX) ||
//				(CString(sz_ele[1]) == STR_DXN) ||
//				(CString(sz_ele[1]) == STR_DXR))
			{
				stpd.l_rng_index = 1;
				// もっとも古い時刻を調べる
				CTime ct = StrTimeToCTime(CString(sz_work));
				time_t rdy_t = ct.GetTime();
				if (rdy_t < stpd.l_rng_time)
				{
					stpd.l_rng_time = rdy_t;
				}
			}
			break;

		case 39: //レディネスチェック（テレメトリ伝送）
//			if ((CString(sz_ele[1]) == STR_S) ||
//				(CString(sz_ele[1]) == STR_X) ||
//				(CString(sz_ele[1]) == STR_X2))
			{
				stpd.l_tlm_index = 1;
				// もっとも古い時刻を調べる
				CTime ct = StrTimeToCTime(CString(sz_work));
				time_t rdy_t = ct.GetTime();
				if (rdy_t < stpd.l_tlm_time)
				{
					stpd.l_tlm_time = rdy_t;
				}
			}
			break;

		case 40: //レディネスチェック（コマンド伝送）
//			if ((CString(sz_ele[1]) == STR_2) ||
//				(CString(sz_ele[1]) == STR_3) ||
//				(CString(sz_ele[1]) == STR_4))
			{
				stpd.l_cmd_index = 1;
				// もっとも古い時刻を調べる
				CTime ct = StrTimeToCTime(CString(sz_work));
				time_t rdy_t = ct.GetTime();
				if (rdy_t < stpd.l_cmd_time)
				{
					stpd.l_cmd_time = rdy_t;
				}
			}
			break;


		case 41: //アンテナ待ち受け指向
			stpd.l_antwait_flg = 1;
			{
				// 「ANT待ち受け指向」「設備初期設定」内の最小時刻を調べる
				// 「ANT待ち受け指向」が見つかった
				CString work;
				work.Format(_T("%s %s"), sz_date, sz_time);
				CTime ct = StrTimeToCTime(work);
				if (ct != CTime(0)){
					stpd.l_antwait_time = ct.GetTime();
				}
			}
			break;


		case 53: // UP START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_uplink_s_timelst.push_back(CString(strTime));
			break;

		case 54: // UP END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_uplink_e_timelst.push_back(CString(strTime));
			break;

		case 55: // RNG_START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_rng_s_timelst.push_back(CString(strTime));

			// 種別
			stpd.sz_rng_kindlst.push_back(ExchagePlanFileRngType(CString(sz_ele[1])));
			break;

		case 56: // RNG_END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_rng_e_timelst.push_back(CString(strTime));
			break;

		case 57: // CMD_START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_cmd_s_timelst.push_back(CString(strTime));
			break;

		case 58: // CMD_END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_cmd_e_timelst.push_back(CString(strTime));
			break;

		case 59: // TLMS_START
			if (CString(sz_ele[1]) == TLM_S)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				if (StrTimeToCTime(strTime) != CTime(0))
					stpd.sz_tlms_s_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_X)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				if (StrTimeToCTime(strTime) != CTime(0))
					stpd.sz_tlmx_s_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_KA)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				if (StrTimeToCTime(strTime) != CTime(0))
					stpd.sz_tlmka_s_timelst.push_back(CString(strTime));
			}
			break;

		case 60: // TLMS_END
			if (CString(sz_ele[1]) == TLM_S)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				if (StrTimeToCTime(strTime) != CTime(0))
					stpd.sz_tlms_e_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_X)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				if (StrTimeToCTime(strTime) != CTime(0))
					stpd.sz_tlmx_e_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_KA)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				if (StrTimeToCTime(strTime) != CTime(0))
					stpd.sz_tlmka_e_timelst.push_back(CString(strTime));
			}
			break;

		}
	}

	fclose(fp_plan);
	return 0;
}


/*============================================================================*/
/*! CTimeLineView

-# 計画ファイルを読み込む(54m)

@param  なし
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
int CTimeLineView::ReadPlanFile54(CString sz_satname, CString sz_passno, stPlandata& stpd, LPARAM lParam)
{
	FILE	*fp_plan;
	CString sz_p_file;
	CString sz_p_ftpfile;

	TCHAR sz_s_start[50];
	TCHAR sz_buff[512];
	TCHAR sz_work[256];
	TCHAR sz_date[50] = { 0 };
	TCHAR sz_time[50] = { 0 };
	//	TCHAR sz_txrfpwr[3][20];

	TCHAR sz_work2[256];
	TCHAR sz_work3[256];
	TCHAR sz_work4[256];
	TCHAR sz_work5[256];
	TCHAR sz_work6[256];
	TCHAR sz_work7[256];
	TCHAR sz_work8[256];

	long i;
	long j;
	long l;
	long k;
	long l_rc;

	__int64 d_s_start;
	__int64 d_time;

	CString str;
	CString sz_txcomp;
	CString strTime;

	TCHAR sz_ele[10][100];
	memset(&sz_ele[0], 0, static_cast<unsigned>(_countof(sz_ele)));

	CString strPlanPath = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
	CString strFtpPlanPath = theApp.GetShareFilePath(eFileType_PlanOut_Folder, theApp.GetSelectStation());
	if (lParam == 1){
		sz_p_file.Format(_T("%senp\\%s\\%s.pln"), (LPCTSTR)strPlanPath, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
		sz_p_ftpfile.Format(_T("%s%s\\%s.pln"), (LPCTSTR)strFtpPlanPath, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	}
	else{
		sz_p_file.Format(_T("%ssttn\\%s\\%s.pln"), (LPCTSTR)strPlanPath, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
		sz_p_ftpfile.Format(_T("%s%s\\%s.pln"), (LPCTSTR)strFtpPlanPath, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	}

	sz_s_start[0] = '\0';


	/*----- ftp file get  "sz_p_file" -----*/
	CString path(strPlanPath);
	CString file(sz_p_file);

	if (_wfopen_s(&fp_plan, sz_p_file, _T("r")) != 0)
	{
		return -1;
	}

	stpd.sz_pred_path = _T("");
	stpd.sz_filepath = _T("");
	stpd.sz_ftpfilepath = _T("");
	stpd.sz_start = _T("");
	stpd.sz_end = _T("");
	stpd.sz_s_bitrate = _T("");
	stpd.sz_x_bitrate = _T("");
	stpd.sz_ka_bitrate = _T("");
	stpd.sz_x_qpsk = _T("");
	stpd.sz_cmd_bit_rate = _T("");
	stpd.sz_x_cmd_bit_rate = _T("");
	stpd.l_cmd_mod = 0;
	stpd.l_rng_mod = 0;
	stpd.sz_rng_band = _T("");
	stpd.sz_rng_start = _T("");
	stpd.sz_rng_end = _T("");
	stpd.l_rdychk = 0;
	stpd.l_init = 0;
	stpd.l_auto_acq = 0;
	stpd.l_offset = 0;
	stpd.l_a_flag = 0;
	stpd.l_l_flag = 0;
	stpd.l_term_ant = 1;
	stpd.l_tx_ramp = 0;
	stpd.l_rx_ramp = 0;
	stpd.l_acq_route = 0;
	stpd.l_pl_sel = 0;
	stpd.l_top_mon = 0;
	stpd.l_ent_angle = 0;
	stpd.l_ext_angle = 0;
	stpd.sz_level_s = _T("");
	stpd.sz_level_x = _T("");
	stpd.sz_ant_s = _T("");
	stpd.sz_ant_x = _T("");
	stpd.l_delay_sel = 0;
	stpd.l_tx_pwr_sel = 0;
	stpd.l_mod_sel = 0;
	stpd.l_tx_uc_sel = -1;
	stpd.l_antwait_flg = 0;
	stpd.l_cmd_index = 0;
	stpd.l_tlm_index = 0;
	stpd.l_agc_index = 0;
	stpd.l_rng_index = 0;
	stpd.l_range_band = 0;
	stpd.l_ant_stow = 2;
	stpd.sz_cid = _T("");
	stpd.sz_station = _T("");
	stpd.sz_priority = _T("M");
	stpd.sz_ope_mode = _T("");
	stpd.l_uplink = 0;
	stpd.sz_uplink_s_timelst.clear();
	stpd.sz_uplink_e_timelst.clear();
	stpd.l_rng = 0;
	stpd.sz_rng_s_timelst.clear();
	stpd.sz_rng_e_timelst.clear();
	stpd.sz_rng_kindlst.clear();
	stpd.l_cmd = 0;
	stpd.sz_cmd_s_timelst.clear();
	stpd.sz_cmd_e_timelst.clear();
	stpd.l_tlms = 0;
	stpd.sz_tlms_s_timelst.clear();
	stpd.sz_tlms_e_timelst.clear();
	stpd.l_tlmx = 0;
	stpd.sz_tlmx_s_timelst.clear();
	stpd.sz_tlmx_e_timelst.clear();
	stpd.l_tlmka = 0;
	stpd.sz_tlmka_s_timelst.clear();
	stpd.sz_tlmka_e_timelst.clear();

	// 「ANT待ち受け指向」「設備初期設定」内の最小時刻を調べる
	stpd.l_antwait_time = 0;
	stpd.l_antinit_time = 0;
	stpd.l_initmin_time = 0;

	__int64 d_tm;

	d_s_start = 0;
	d_time = 0;

	stpd.sz_filepath = sz_p_file;
	stpd.sz_ftpfilepath = sz_p_ftpfile;

	// 計画ファイルタイプ
	stpd.sz_plan_type = _T("");

	// レディネスチェック時間
	stpd.l_agc_time = LONG64_MAX;
	stpd.l_rng_time = LONG64_MAX;
	stpd.l_tlm_time = LONG64_MAX;
	stpd.l_cmd_time = LONG64_MAX;

	char sz_line[512];
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_plan) != 0)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			if (wcsncmp(sz_buff, SOS_HEADER_ID, wcslen(SOS_HEADER_ID)) == 0)
			{
				swscanf_s(sz_buff, _T("%s%s%s%s%s%s%s%s"),
					sz_work, static_cast<unsigned>(_countof(sz_work)),
					sz_work2, static_cast<unsigned>(_countof(sz_work2)),
					sz_work3, static_cast<unsigned>(_countof(sz_work3)),
					sz_work4, static_cast<unsigned>(_countof(sz_work4)),
					sz_work5, static_cast<unsigned>(_countof(sz_work5)),
					sz_work6, static_cast<unsigned>(_countof(sz_work6)),
					sz_work7, static_cast<unsigned>(_countof(sz_work7)),
					sz_work8, static_cast<unsigned>(_countof(sz_work8)));

				// 計画タイプ
				if (stpd.sz_plan_type.IsEmpty() == true)
					stpd.sz_plan_type = CString(sz_work2);

				// 予報値ファイル名
				if (stpd.sz_pred_path.IsEmpty() == true)
					stpd.sz_pred_path = CString(sz_work8);

			}

			l_rc = swscanf_s(&sz_buff[1], _T("%s%s%s%s%s%s%s%s%s%s"),
				sz_ele[0], static_cast<unsigned>(_countof(sz_ele[0])),
				sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])),
				sz_ele[2], static_cast<unsigned>(_countof(sz_ele[2])),
				sz_ele[3], static_cast<unsigned>(_countof(sz_ele[3])),
				sz_ele[4], static_cast<unsigned>(_countof(sz_ele[4])),
				sz_ele[5], static_cast<unsigned>(_countof(sz_ele[5])),
				sz_ele[6], static_cast<unsigned>(_countof(sz_ele[6])),
				sz_ele[7], static_cast<unsigned>(_countof(sz_ele[7])),
				sz_ele[8], static_cast<unsigned>(_countof(sz_ele[8])),
				sz_ele[9], static_cast<unsigned>(_countof(sz_ele[9])));

			for (i = 0; i < l_rc; ++i)
			{
				//				str = CString(sz_ele[i]);
				AfxExtractSubString(str, CString(sz_ele[i]), 0, '=');

				/*---------- 初期値 チェック ----------*/
				if (str.Find(sz_c_ele54[2]) >= 0)
				{
					j = sz_c_ele54[2].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_init = k;
							break;
						}
					}
				}

				/*---------- CMD MOD の AUTO/MANチェック ----------*/
				if (str.Find(sz_c_ele54[6]) >= 0)
				{
					j = sz_c_ele54[6].GetLength() + 1;
					for (k = 0; sz_auto[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_auto[k])
						{
							stpd.l_cmd_mod = k;
							break;
						}
					}
				}
				/*---------- RNG MOD の AUTO/MANチェック ----------*/
				if (str.Find(sz_c_ele54[7]) >= 0)
				{
					j = sz_c_ele54[7].GetLength() + 1;
					for (k = 0; sz_auto[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_auto[k])
						{
							stpd.l_rng_mod = k;
							break;
						}
					}
				}
				/*---------- RNG BAND の 取り込み ----------*/
				if (str.Find(sz_c_ele54[8]) >= 0)
				{
					j = sz_c_ele54[8].GetLength() + 1;
					stpd.sz_rng_band = CString(&sz_ele[i][j]);
				}
				/*---------- RNG START の 取り込み ----------*/
				if (str.Find(sz_c_ele54[9]) >= 0)
				{
					j = sz_c_ele54[9].GetLength() + 1;
					stpd.sz_rng_start = CString(&sz_ele[i][j]);
				}
				/*---------- RNG END の 取り込み ----------*/
				if (str.Find(sz_c_ele54[10]) >= 0)
				{
					j = sz_c_ele54[10].GetLength() + 1;
					stpd.sz_rng_end = CString(&sz_ele[i][j]);
				}
				/*---------- 時刻オフセットへの追従 ----------*/
				if (str.Find(sz_c_ele54[11]) >= 0)
				{
					j = sz_c_ele54[11].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_offset = k;
							break;
						}
					}
				}
				/*---------- AOS FLAG ----------*/
				if (str.Find(sz_c_ele54[14]) >= 0)
				{
					j = sz_c_ele54[14].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_a_flag = k;
							break;
						}
					}
				}
				/*---------- LOS FLAG ----------*/
				if (str.Find(sz_c_ele54[15]) >= 0)
				{
					j = sz_c_ele54[15].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_l_flag = k;
							break;
						}
					}
				}
				/*---------- 自動捕捉の有無チェック ----------*/
				if (str.Find(sz_c_ele54[16]) >= 0)
				{
					j = sz_c_ele54[16].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_auto_acq = k;
							break;
						}
					}
				}
				/*---------- パス終了時のＡＮＴモード ----------*/
				if (str.Find(sz_c_ele54[18]) >= 0)
				{
					j = sz_c_ele54[18].GetLength() + 1;
					for (k = 0; sz_term_ant[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_term_ant[k])
						{
							stpd.l_term_ant = k;
							break;
						}
					}
				}
				/*---------- 送信設備ランプ制御有無 ----------*/
				if (str.Find(sz_c_ele54[19]) >= 0)
				{
					j = sz_c_ele54[19].GetLength() + 1;
					for (k = 0; sz_tx_ramp[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_tx_ramp[k])
						{
							stpd.l_tx_ramp = k;
							break;
						}
					}
				}
				/*---------- 受信設備ランプ制御有無 ----------*/
				if (str.Find(sz_c_ele54[20]) >= 0)
				{
					j = sz_c_ele54[20].GetLength() + 1;
					for (k = 0; sz_rx_ramp54[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_rx_ramp54[k])
						{
							stpd.l_rx_ramp = k;
							break;
						}
					}
				}
				/*---------- 補足ルート ----------*/
				if (str.Find(sz_c_ele54[21]) >= 0)
				{
					j = sz_c_ele54[21].GetLength() + 1;
					stpd.l_acq_route = _wtoi(&sz_ele[i][j]);
					stpd.l_acq_route -= 1;	/* 配列番号に変換 */
				}
				/*---------- ACQ/MAIN判定有無 ----------*/
				if (str.Find(sz_c_ele54[22]) >= 0)
				{
					j = sz_c_ele54[22].GetLength() + 1;
					for (k = 0; sz_on_off[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on_off[k])
						{
							stpd.l_pl_sel = k;
							break;
						}
					}
				}
				/*---------- 天頂通過監視有無 ----------*/
				if (str.Find(sz_c_ele54[23]) >= 0)
				{
					j = sz_c_ele54[23].GetLength() + 1;
					for (k = 0; sz_on_off[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on_off[k])
						{
							stpd.l_top_mon = k;
							break;
						}
					}
				}
				/*---------- 天頂通過監視入角 ----------*/
				if (str.Find(sz_c_ele54[24]) >= 0)
				{
					j = sz_c_ele54[24].GetLength() + 1;
					stpd.l_ent_angle = _wtoi(&sz_ele[i][j]);
				}
				/*---------- 天頂通過監視出角 ----------*/
				if (str.Find(sz_c_ele54[25]) >= 0)
				{
					j = sz_c_ele54[25].GetLength() + 1;
					stpd.l_ext_angle = _wtoi(&sz_ele[i][j]);
				}
				/*---------- 再生測距対応　局内ディレイ値選択 ----------*/
				if (str.Find(sz_c_ele54[33]) >= 0)
				{
					j = sz_c_ele54[33].GetLength() + 1;
					for (k = 0; sz_delay_sel[k] != _T(""); ++k) {
						if (CString(&sz_ele[i][j]) == sz_delay_sel[k])
						{
							stpd.l_delay_sel = k;
							break;
						}
					}
				}

				/*---------- ANT天頂指向有無 ----------*/
				if (str.Find(sz_c_ele54[46]) >= 0)
				{
					j = sz_c_ele54[46].GetLength() + 1;
					for (k = 0; sz_ant_stow[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_ant_stow[k])
						{
							stpd.l_ant_stow = k;
							break;
						}
					}
				}

				/*---------- CID ----------*/
				if (str.Find(sz_c_ele54[47]) >= 0)
				{
					j = sz_c_ele54[47].GetLength() + 1;
					stpd.sz_cid = CString(&sz_ele[i][j]);
				}

				/*---------- 局名 ----------*/
				if (str.Find(sz_c_ele54[48]) >= 0)
				{
					j = sz_c_ele54[48].GetLength() + 1;
					stpd.sz_station = CString(&sz_ele[i][j]);
				}

				/*---------- 主従局 ----------*/
				if (str.Find(sz_c_ele54[49]) >= 0)
				{
					j = sz_c_ele54[49].GetLength() + 1;
					stpd.sz_priority = CString(&sz_ele[i][j]);
				}

				/*---------- オペレーションモード ----------*/
				if (str.Find(sz_c_ele54[50]) >= 0)
				{
					j = sz_c_ele54[50].GetLength() + 1;
					stpd.sz_ope_mode = CString(&sz_ele[i][j]);
				}

				/*---------- 自動生成 ----------*/
				if (str.CompareNoCase(sz_c_ele54[61]) == 0)
				{
					j = sz_c_ele54[61].GetLength() + 1;
					stpd.sz_auto = CString(&sz_ele[i][j]);
				}

			}
			continue;
		}

		swscanf_s(sz_buff, _T("%s%s%s"),
			sz_date, static_cast<unsigned>(_countof(sz_date)),
			sz_time, static_cast<unsigned>(_countof(sz_time)),
			sz_work, static_cast<unsigned>(_countof(sz_work)));

		// 「ANT待ち受け指向」「設備初期設定」内の最小時刻を調べる
		CString temp = CString(sz_buff);
		if (temp.Find(_T("INIT.")) >= 0){
			// 「設備初期設定」が見つかった
			CString work;
			work.Format(_T("%s %s"), sz_date, sz_time);
			CTime ct = StrTimeToCTime(work);
			if (ct != CTime(0) && stpd.l_initmin_time != 0 && ct.GetTime() < stpd.l_initmin_time){
				stpd.l_initmin_time = ct.GetTime();
			}
			else if (ct != CTime(0) && stpd.l_initmin_time == 0){
				stpd.l_initmin_time = ct.GetTime();
			}
		}


		j = 0;
		l = 0;
		for (k = 0; sz_work[k] != '\0'; ++k)
		{
			if (sz_work[k] == ',')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_work[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
				break;
			}
			else if (sz_work[k] != ' ')
			{
				sz_ele[j][l] = sz_work[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			continue;
		}

		/* X帯送受信ルート判定文字列を探す */
		str = CString(sz_ele[0]);
		if (str.Find(X_ROUTE_DEF_TXT) >= 0)
		{
			stpd.l_route_mode = 1;
		}

		for (k = 0; sz_c_ele54[k] != _T(""); ++k)
		{
			if ((k == 2) || (k == 4) || (k == 35))
			{	/* 判定ケースを追加 */
				if (sz_c_ele54[k].Find(str) >= 0)
				{
					break;
				}
			}
			else
			{
				if (str == sz_c_ele54[k])
				{
					break;
				}
			}
		}

		if (sz_c_ele54[k] == _T(""))
		{
			continue;
		}

		swprintf_s(sz_work, static_cast<unsigned>(_countof(sz_work)), _T("%s %s"), sz_date, sz_time);
		if (d_s_start == 0.0 || d_s_start > d_time)
		{
			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), _T(" "));
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_time);
			d_s_start = d_time;
		}

		switch (k)
		{
			/*----- レディネスチェック処理 -----*/
		case 0:
			(stpd.l_rdychk) |= 0x01;
			break;

		case 1:
			(stpd.l_rdychk) |= 0x02;
			break;

			/*----- 初期設定処理 -----*/
		case 2:
			stpd.l_init = 1;
			break;

			/*----- ビットレートKa -----*/
		case 3:
			stpd.sz_ka_bitrate = CString(sz_ele[1]);
			break;

			/*----- ビットレートＸ -----*/
		case 4:
			stpd.sz_x_bitrate = CString(sz_ele[1]);
			break;

			/*----- ビットレートＸＱＰＳＫ -----*/
		case 17:
			stpd.sz_x_qpsk = CString(sz_ele[1]);
			break;

			/*----- S/X コマンドビットレート -----*/
		case 5:
			stpd.sz_cmd_bit_rate = CString(sz_ele[1]);
			break;

			/*----- X コマンドビットレート -----*/
		case 36:
		case 43:
			stpd.sz_x_cmd_bit_rate = CString(sz_ele[1]);
			break;


			/*----- 開始時刻処理 -----*/
		case 12:
			stpd.sz_start.Format(_T("%s %s"), sz_date, sz_time);

			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), _T(" "));
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_time);

			TCHAR sz_lwork[50];
			__int64 d_work;
			__int64 l_tm;
			swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s.0000"), sz_s_start);

			d_work = StrTimeToCTime(CString(sz_lwork)).GetTime();
			l_tm = d_work - d_tm;
			stpd.l_ant_time = (long)(l_tm / 60);
			break;

			/*----- 終了時刻処理 -----*/
		case 13:
			stpd.sz_end.Format(_T("%s %s"), sz_date, sz_time);
			break;

			/*----- Ｓ帯衛星送信情報処理 -----*/
		case 28:
		{
			/* 送信レベルとアンテナ選択情報に分ける */
			CString str = CString(sz_ele[1]);
			int idx = str.Find(_T("-"));
			stpd.sz_level_s = str.Mid(0, idx);
			stpd.sz_ant_s = str.Mid(idx + 1);
		}
		break;

		/*----- Ｘ帯衛星送信情報処理 -----*/
		case 29:
		{
			/* 送信レベルとアンテナ選択情報に分ける */
			CString str = CString(sz_ele[1]);
			int idx = str.Find(_T("-"));
			stpd.sz_level_x = str.Mid(0, idx);
			stpd.sz_ant_x = str.Mid(idx + 1);
		}
		break;

		/*----- ４ＷＡＹ運用有無情報処理 -----*/
		case 30:
			if (wcsncmp(sz_ele[1], CTRL_4WAY, wcslen(CTRL_4WAY)) == 0)
			{
				stpd.l_selene_4way = 0;
			}
			else if (wcsncmp(sz_ele[1], _T("ANT"), wcslen(_T("ANT"))) == 0)
			{
				TCHAR sz_lwork[50];
				stpd.l_antwait_flg = 1;
				swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s %s.0000"), sz_date, sz_time);
				d_tm = StrTimeToCTime(CString(sz_lwork)).GetTime();
				stpd.l_antinit_time = d_tm;
			}
			break;

			/*----- X-TX 送信出力選択処理 -----*//* 2006-11-15 追加 */
		case 32:
			for (i = 0; sz_tx_pwr_sel54[i] != (char*)NULL; i++)
			{
				if (wcscmp(sz_tx_pwr_sel54[i], sz_ele[1]) == 0)
				{
					stpd.l_tx_pwr_sel = i;
					break;
				}
			}
			break;

			/*----- Ｘ帯変調装置選択処理 -----*//* 2009-02-06 追加 */
		case 35:
			for (i = 0; sz_mod_sel[i] != (char*)NULL; i++)
			{
				if (wcscmp(sz_mod_sel[i], sz_ele[1]) == 0)
				{
					stpd.l_mod_sel = i;
					break;
				}
			}
			break;

		case 37: //レディネスチェック（AGC)
			//			if ((CString(sz_ele[1]) == STR_S_RX_A) ||
			//				(CString(sz_ele[1]) == STR_X_RX2_A) ||
			//				(CString(sz_ele[1]) == STR_DSN_MODEM_A))
		{
			stpd.l_agc_index = 1;
			// もっとも古い時刻を調べる
			CTime ct = StrTimeToCTime(CString(sz_work));
			time_t rdy_t = ct.GetTime();
			if (rdy_t < stpd.l_agc_time)
			{
				stpd.l_agc_time = rdy_t;
			}
		}
		break;

		case 38: //レディネスチェック（RNG)
			//			if ((CString(sz_ele[1]) == STR_SS) ||
			//				(CString(sz_ele[1]) == STR_SX) ||
			//				(CString(sz_ele[1]) == STR_XX) ||
			//				(CString(sz_ele[1]) == STR_RX) ||
			//				(CString(sz_ele[1]) == STR_DXN) ||
			//				(CString(sz_ele[1]) == STR_DXR))
		{
			stpd.l_rng_index = 1;
			// もっとも古い時刻を調べる
			CTime ct = StrTimeToCTime(CString(sz_work));
			time_t rdy_t = ct.GetTime();
			if (rdy_t < stpd.l_rng_time)
			{
				stpd.l_rng_time = rdy_t;
			}
		}
		break;

		case 39: //レディネスチェック（テレメトリ伝送）
			//			if ((CString(sz_ele[1]) == STR_S) ||
			//				(CString(sz_ele[1]) == STR_X) ||
			//				(CString(sz_ele[1]) == STR_X2))
		{
			stpd.l_tlm_index = 1;
			// もっとも古い時刻を調べる
			CTime ct = StrTimeToCTime(CString(sz_work));
			time_t rdy_t = ct.GetTime();
			if (rdy_t < stpd.l_tlm_time)
			{
				stpd.l_tlm_time = rdy_t;
			}
		}
		break;

		case 40: //レディネスチェック（コマンド伝送）
			//			if ((CString(sz_ele[1]) == STR_2) ||
			//				(CString(sz_ele[1]) == STR_3) ||
			//				(CString(sz_ele[1]) == STR_4))
		{
			stpd.l_cmd_index = 1;
			// もっとも古い時刻を調べる
			CTime ct = StrTimeToCTime(CString(sz_work));
			time_t rdy_t = ct.GetTime();
			if (rdy_t < stpd.l_cmd_time)
			{
				stpd.l_cmd_time = rdy_t;
			}
		}
		break;


		case 41: //アンテナ待ち受け指向
			stpd.l_antwait_flg = 1;
			{
				// 「ANT待ち受け指向」「設備初期設定」内の最小時刻を調べる
				// 「ANT待ち受け指向」が見つかった
				CString work;
				work.Format(_T("%s %s"), sz_date, sz_time);
				CTime ct = StrTimeToCTime(work);
				if (ct != CTime(0)){
					stpd.l_antwait_time = ct.GetTime();
				}
			}
			break;


		case 53: // UP START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_uplink_s_timelst.push_back(CString(strTime));
			break;

		case 54: // UP END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_uplink_e_timelst.push_back(CString(strTime));
			break;

		case 55: // RNG_START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_rng_s_timelst.push_back(CString(strTime));

			// 種別
			stpd.sz_rng_kindlst.push_back(ExchagePlanFileRngType(CString(sz_ele[1])));
			break;

		case 56: // RNG_END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_rng_e_timelst.push_back(CString(strTime));
			break;

		case 57: // CMD_START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_cmd_s_timelst.push_back(CString(strTime));
			break;

		case 58: // CMD_END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_cmd_e_timelst.push_back(CString(strTime));
			break;

		case 59: // TLMS_START
			if (CString(sz_ele[1]) == TLM_S)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				if (StrTimeToCTime(strTime) != CTime(0))
					stpd.sz_tlms_s_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_X)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				if (StrTimeToCTime(strTime) != CTime(0))
					stpd.sz_tlmx_s_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_KA)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				if (StrTimeToCTime(strTime) != CTime(0))
					stpd.sz_tlmka_s_timelst.push_back(CString(strTime));
			}
			break;

		case 60: // TLMS_END
			if (CString(sz_ele[1]) == TLM_S)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				if (StrTimeToCTime(strTime) != CTime(0))
					stpd.sz_tlms_e_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_X)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				if (StrTimeToCTime(strTime) != CTime(0))
					stpd.sz_tlmx_e_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_KA)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				if (StrTimeToCTime(strTime) != CTime(0))
					stpd.sz_tlmka_e_timelst.push_back(CString(strTime));
			}
			break;

		}
	}

	fclose(fp_plan);
	return 0;
}


int CTimeLineView::ReadPlanFile34(CString sz_satname, CString sz_passno, stPlandata& stpd, LPARAM lParam)
{
	FILE	*fp_plan;
	CString sz_p_file;
	CString sz_p_ftpfile;

	TCHAR sz_s_start[50];
	TCHAR sz_buff[512];
	TCHAR sz_work[256];
	TCHAR sz_date[50] = { 0 };
	TCHAR sz_time[50] = { 0 };
	TCHAR sz_txrfpwr[3][20];

	TCHAR sz_work2[256];
	TCHAR sz_work3[256];
	TCHAR sz_work4[256];
	TCHAR sz_work5[256];
	TCHAR sz_work6[256];
	TCHAR sz_work7[256];
	TCHAR sz_work8[256];

	long i;
	long j;
	long l;
	long k;
	long l_rc;

	__int64 d_s_start;
	__int64 d_time;

	CString str;
	CString sz_txcomp;
	CString strTime;

	TCHAR sz_ele[10][100];
	memset(&sz_ele[0], 0, static_cast<unsigned>(_countof(sz_ele)));


#if 1
	CString strPlanPath = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
	CString strFtpPlanPath = theApp.GetShareFilePath(eFileType_PlanOut_Folder, theApp.GetSelectStation());
	if (lParam == 1){
		sz_p_file.Format(_T("%senp\\%s\\%s.pln"), (LPCTSTR)strPlanPath, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
		sz_p_ftpfile.Format(_T("%s%s\\%s.pln"), (LPCTSTR)strFtpPlanPath, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	}
	else{
		sz_p_file.Format(_T("%ssttn\\%s\\%s.pln"), (LPCTSTR)strPlanPath, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
		sz_p_ftpfile.Format(_T("%s%s\\%s.pln"), (LPCTSTR)strFtpPlanPath, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	}
#else
	CString ca_cthome;
	ca_cthome.Format(_T("%s%s"), theApp.GetAppPath(), DATA_DIR);

	j = (long)strlen(sd.satinfo.sz_plan_dir);
	if (sd.satinfo.sz_plan_dir[j - 1] == '/')
	{
		sz_p_file.Format(_T("%s%s.pln"), CString(sd.satinfo.sz_plan_dir), sz_passno);
	}
	else
	{
		sz_p_file.Format(_T("%s\\%s.pln"), CString(sd.satinfo.sz_plan_dir), sz_passno);
	}
#endif
	sz_s_start[0] = '\0';


	/*----- ftp file get  "sz_p_file" -----*/
	CString path(strPlanPath);
	CString file(sz_p_file);

	//	if (GetPlanFile(path, file) == FALSE)
	//	{
	//		return -1;
	//	}

	//if (_wfopen_s(&fp_plan, sz_p_file, _T("r, ccs=UTF-8")) != 0)
	if (_wfopen_s(&fp_plan, sz_p_file, _T("r")) != 0)
	{
		return -1;
	}

	stpd.sz_pred_path = _T("");
	stpd.sz_filepath = _T("");
	stpd.sz_ftpfilepath = _T("");
	stpd.sz_start = _T("");
	stpd.sz_end = _T("");
	stpd.sz_s_bitrate = _T("");
	stpd.sz_x_bitrate = _T("");
	stpd.sz_x_qpsk = _T("");
	stpd.sz_cmd_bit_rate = _T("");
	stpd.sz_x_cmd_bit_rate = _T("");
	stpd.l_cmd_mod = 0;
	stpd.l_rng_mod = 0;
	stpd.sz_rng_band = _T("");
	stpd.sz_rng_start = _T("");
	stpd.sz_rng_end = _T("");
	stpd.l_rdychk = 0;
	stpd.l_init = 0;
	stpd.l_auto_acq = 0;
	stpd.l_offset = 0;
	stpd.l_a_flag = 0;
	stpd.l_l_flag = 0;
	stpd.l_term_ant = 1;
	stpd.l_tx_ramp = 0;
	stpd.l_rx_ramp = 0;
	stpd.l_acq_route = 0;
	stpd.l_pl_sel = 0;
	stpd.l_top_mon = 0;
	stpd.l_ent_angle = 0;
	stpd.l_ext_angle = 0;
	stpd.sz_level_s = _T("");
	stpd.sz_level_x = _T("");
	stpd.sz_ant_s = _T("");
	stpd.sz_ant_x = _T("");
	stpd.l_delay_sel = 0;
	stpd.l_tx_pwr_sel = 0;
	stpd.l_mod_sel = 0;
	stpd.l_tx_uc_sel = -1;
	stpd.l_antwait_flg = 0;
	stpd.l_cmd_index = 0;
	stpd.l_tlm_index = 0;
	stpd.l_agc_index = 0;
	stpd.l_rng_index = 0;
	stpd.l_range_band = 0;
	stpd.l_ant_stow = 2;
	stpd.sz_cid = _T("");
	stpd.sz_station = _T("");
	stpd.sz_priority = _T("");
	stpd.sz_ope_mode = _T("");
	stpd.sz_uplink_s_timelst.clear();
	stpd.sz_uplink_e_timelst.clear();
	stpd.l_rng = 0;
	stpd.sz_rng_s_timelst.clear();
	stpd.sz_rng_e_timelst.clear();
	stpd.sz_rng_kindlst.clear();
	stpd.l_cmd = 0;
	stpd.sz_cmd_s_timelst.clear();
	stpd.sz_cmd_e_timelst.clear();
	stpd.l_tlms = 0;
	stpd.sz_tlms_s_timelst.clear();
	stpd.sz_tlms_e_timelst.clear();
	stpd.l_tlmx = 0;
	stpd.sz_tlmx_s_timelst.clear();
	stpd.sz_tlmx_e_timelst.clear();
	stpd.l_tlmka = 0;
	stpd.sz_tlmka_s_timelst.clear();
	stpd.sz_tlmka_e_timelst.clear();

	// 「ANT待ち受け指向」「設備初期設定」内の最小時刻を調べる
	stpd.l_antwait_time = 0;
	stpd.l_antinit_time = 0;
	stpd.l_initmin_time = 0;

	__int64 d_tm;

	d_s_start = 0;
	d_time = 0;

	stpd.sz_filepath = sz_p_file;
	stpd.sz_ftpfilepath = sz_p_ftpfile;

	// 計画ファイルタイプ
	stpd.sz_plan_type = _T("");

	// レディネスチェック時間
	stpd.l_agc_time = LONG64_MAX;
	stpd.l_rng_time = LONG64_MAX;
	stpd.l_tlm_time = LONG64_MAX;
	stpd.l_cmd_time = LONG64_MAX;

	char sz_line[512];
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_plan) != 0)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			if (wcsncmp(sz_buff, SOS_HEADER_ID, wcslen(SOS_HEADER_ID)) == 0)
			{
				swscanf_s(sz_buff, _T("%s%s%s%s%s%s%s%s"),
					sz_work, static_cast<unsigned>(_countof(sz_work)),
					sz_work2, static_cast<unsigned>(_countof(sz_work2)),
					sz_work3, static_cast<unsigned>(_countof(sz_work3)),
					sz_work4, static_cast<unsigned>(_countof(sz_work4)),
					sz_work5, static_cast<unsigned>(_countof(sz_work5)),
					sz_work6, static_cast<unsigned>(_countof(sz_work6)),
					sz_work7, static_cast<unsigned>(_countof(sz_work7)),
					sz_work8, static_cast<unsigned>(_countof(sz_work8)));

				// 計画タイプ
				if (stpd.sz_plan_type.IsEmpty() == true)
					stpd.sz_plan_type = CString(sz_work2);

				// 予報値ファイル名
				if (stpd.sz_pred_path.IsEmpty() == true)
					stpd.sz_pred_path = CString(sz_work8);
			}

			l_rc = swscanf_s(&sz_buff[1], _T("%s%s%s%s%s%s%s%s%s%s"),
				sz_ele[0], static_cast<unsigned>(_countof(sz_ele[0])),
				sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])),
				sz_ele[2], static_cast<unsigned>(_countof(sz_ele[2])),
				sz_ele[3], static_cast<unsigned>(_countof(sz_ele[3])),
				sz_ele[4], static_cast<unsigned>(_countof(sz_ele[4])),
				sz_ele[5], static_cast<unsigned>(_countof(sz_ele[5])),
				sz_ele[6], static_cast<unsigned>(_countof(sz_ele[6])),
				sz_ele[7], static_cast<unsigned>(_countof(sz_ele[7])),
				sz_ele[8], static_cast<unsigned>(_countof(sz_ele[8])),
				sz_ele[9], static_cast<unsigned>(_countof(sz_ele[9])));

			for (i = 0; i < l_rc; ++i)
			{
//				str = CString(sz_ele[i]);
				AfxExtractSubString(str, CString(sz_ele[i]), 0, '=');

				/*---------- 初期値 チェック ----------*/
				if (str.Find(sz_c_ele34[2]) >= 0)
				{
					j = sz_c_ele34[2].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_init = k;
							break;
						}
					}
				}

				/*---------- CMD MOD の AUTO/MANチェック ----------*/
				if (str.Find(sz_c_ele34[6]) >= 0)
				{
					j = sz_c_ele34[6].GetLength() + 1;
					for (k = 0; sz_auto[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_auto[k])
						{
							stpd.l_cmd_mod = k;
							break;
						}
					}
				}
				/*---------- RNG MOD の AUTO/MANチェック ----------*/
				if (str.Find(sz_c_ele34[7]) >= 0)
				{
					j = sz_c_ele34[7].GetLength() + 1;
					for (k = 0; sz_auto[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_auto[k])
						{
							stpd.l_rng_mod = k;
							break;
						}
					}
				}
				/*---------- RNG BAND の 取り込み ----------*/
				if (str.Find(sz_c_ele34[8]) >= 0)
				{
					j = sz_c_ele34[8].GetLength() + 1;
					stpd.sz_rng_band = CString(&sz_ele[i][j]);
				}
				/*---------- RNG START の 取り込み ----------*/
				if (str.Find(sz_c_ele34[9]) >= 0)
				{
					j = sz_c_ele34[9].GetLength() + 1;
					stpd.sz_rng_start = CString(&sz_ele[i][j]);
				}
				/*---------- RNG END の 取り込み ----------*/
				if (str.Find(sz_c_ele34[10]) >= 0)
				{
					j = sz_c_ele34[10].GetLength() + 1;
					stpd.sz_rng_end = CString(&sz_ele[i][j]);
				}
				/*---------- 時刻オフセットへの追従 ----------*/
				if (str.Find(sz_c_ele34[11]) >= 0)
				{
					j = sz_c_ele34[11].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_offset = k;
							break;
						}
					}
				}
				/*---------- AOS FLAG ----------*/
				if (str.Find(sz_c_ele34[14]) >= 0)
				{
					j = sz_c_ele34[14].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_a_flag = k;
							break;
						}
					}
				}
				/*---------- LOS FLAG ----------*/
				if (str.Find(sz_c_ele34[15]) >= 0)
				{
					j = sz_c_ele34[15].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_l_flag = k;
							break;
						}
					}
				}
				/*---------- 自動捕捉の有無チェック ----------*/
				if (str.Find(sz_c_ele34[16]) >= 0)
				{
					j = sz_c_ele34[16].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_auto_acq = k;
							break;
						}
					}
				}
				/*---------- パス終了時のＡＮＴモード ----------*/
				if (str.Find(sz_c_ele34[18]) >= 0)
				{
					j = sz_c_ele34[18].GetLength() + 1;
					for (k = 0; sz_term_ant[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_term_ant[k])
						{
							stpd.l_term_ant = k;
							break;
						}
					}
				}
				/*---------- 送信設備ランプ制御有無 ----------*/
				if (str.Find(sz_c_ele34[19]) >= 0)
				{
					j = sz_c_ele34[19].GetLength() + 1;
					for (k = 0; sz_tx_ramp[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_tx_ramp[k])
						{
							stpd.l_tx_ramp = k;
							break;
						}
					}
				}
				/*---------- 受信設備ランプ制御有無 ----------*/
				if (str.Find(sz_c_ele34[20]) >= 0)
				{
					j = sz_c_ele34[20].GetLength() + 1;
					for (k = 0; sz_rx_ramp[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_rx_ramp[k])
						{
							stpd.l_rx_ramp = k;
							break;
						}
					}
				}
				/*---------- 補足ルート ----------*/
				if (str.Find(sz_c_ele34[21]) >= 0)
				{
					j = sz_c_ele34[21].GetLength() + 1;
					stpd.l_acq_route = _wtoi(&sz_ele[i][j]);
					stpd.l_acq_route -= 1;	/* 配列番号に変換 */
				}
				/*---------- ACQ/MAIN判定有無 ----------*/
				if (str.Find(sz_c_ele34[22]) >= 0)
				{
					j = sz_c_ele34[22].GetLength() + 1;
					for (k = 0; sz_on_off[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on_off[k])
						{
							stpd.l_pl_sel = k;
							break;
						}
					}
				}
				/*---------- 天頂通過監視有無 ----------*/
				if (str.Find(sz_c_ele34[23]) >= 0)
				{
					j = sz_c_ele34[23].GetLength() + 1;
					for (k = 0; sz_on_off[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on_off[k])
						{
							stpd.l_top_mon = k;
							break;
						}
					}
				}
				/*---------- 天頂通過監視入角 ----------*/
				if (str.Find(sz_c_ele34[24]) >= 0)
				{
					j = sz_c_ele34[24].GetLength() + 1;
					stpd.l_ent_angle = _wtoi(&sz_ele[i][j]);
				}
				/*---------- 天頂通過監視出角 ----------*/
				if (str.Find(sz_c_ele34[25]) >= 0)
				{
					j = sz_c_ele34[25].GetLength() + 1;
					stpd.l_ext_angle = _wtoi(&sz_ele[i][j]);
				}
				/*---------- 再生測距対応　局内ディレイ値選択 ----------*/
				if (str.Find(sz_c_ele34[30]) >= 0)
				{
					j = sz_c_ele34[30].GetLength() + 1;
					for (k = 0; sz_delay_sel[k] != _T(""); ++k) {
						if (CString(&sz_ele[i][j]) == sz_delay_sel[k])
						{
							stpd.l_delay_sel = k;
							break;
						}
					}
				}
				/*---------- S帯RNG計測帯域 ----------*/
				if (str.Find(sz_c_ele34[42]) >= 0)
				{
					j = sz_c_ele34[42].GetLength() + 1;
					for (k = 0; sz_range_band[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_range_band[k])
						{
							stpd.l_range_band = k;
							break;
						}
					}
				}
				/*---------- ANT天頂指向有無 ----------*/
				if (str.Find(sz_c_ele34[43]) >= 0)
				{
					j = sz_c_ele34[43].GetLength() + 1;
					for (k = 0; sz_ant_stow[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_ant_stow[k])
						{
							stpd.l_ant_stow = k;
							break;
						}
					}
				}

				/*---------- CID ----------*/
				if (str.Find(sz_c_ele34[51]) >= 0)
				{
					j = sz_c_ele34[51].GetLength() + 1;
					stpd.sz_cid = CString(&sz_ele[i][j]);
				}

				/*---------- 局名 ----------*/
				if (str.Find(sz_c_ele34[52]) >= 0)
				{
					j = sz_c_ele34[52].GetLength() + 1;
					stpd.sz_station = CString(&sz_ele[i][j]);
				}

				/*---------- 主従局 ----------*/
				if (str.Find(sz_c_ele34[53]) >= 0)
				{
					j = sz_c_ele34[53].GetLength() + 1;
					stpd.sz_priority = CString(&sz_ele[i][j]);
				}

				/*---------- オペレーションモード ----------*/
				if (str.Find(sz_c_ele34[54]) >= 0)
				{
					j = sz_c_ele34[54].GetLength() + 1;
					stpd.sz_ope_mode = CString(&sz_ele[i][j]);
				}

				/*---------- 自動生成 ----------*/
				if (str.CompareNoCase(sz_c_ele34[65]) == 0)
				{
					j = sz_c_ele34[65].GetLength() + 1;
					stpd.sz_auto = CString(&sz_ele[i][j]);
				}
			}
			continue;
		}

		swscanf_s(sz_buff, _T("%s%s%s"),
			sz_date, static_cast<unsigned>(_countof(sz_date)),
			sz_time, static_cast<unsigned>(_countof(sz_time)),
			sz_work, static_cast<unsigned>(_countof(sz_work)));

		// 「ANT待ち受け指向」「設備初期設定」内の最小時刻を調べる
		CString temp = CString(sz_buff);
		if (temp.Find(_T("INIT.")) >= 0){
			// 「設備初期設定」が見つかった
			CString work;
			work.Format(_T("%s %s"), sz_date, sz_time);
			CTime ct = StrTimeToCTime(work);
			if (ct != CTime(0) && stpd.l_initmin_time != 0 && ct.GetTime() < stpd.l_initmin_time){
				stpd.l_initmin_time = ct.GetTime();
			}
			else if (ct != CTime(0) && stpd.l_initmin_time == 0){
				stpd.l_initmin_time = ct.GetTime();
			}
		}

		j = 0;
		l = 0;
		for (k = 0; sz_work[k] != '\0'; ++k)
		{
			if (sz_work[k] == ',')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_work[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
				break;
			}
			else if (sz_work[k] != ' ')
			{
				sz_ele[j][l] = sz_work[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			continue;
		}

		/* X帯送受信ルート判定文字列を探す */
		str = CString(sz_ele[0]);
		if (str.Find(X_ROUTE_DEF_TXT) >= 0)
		{
			stpd.l_route_mode = 1;
		}

		for (k = 0; sz_c_ele34[k] != _T(""); ++k)
		{
			if ((k == 2) || (k == 4) || (k == 35))
			{	/* 判定ケースを追加 */
				if (sz_c_ele34[k].Find(str) >= 0)
				{
					break;
				}
			}
			else
			{
				if (str == sz_c_ele34[k])
				{
					break;
				}
			}
		}

		if (sz_c_ele34[k] == _T(""))
		{
			continue;
		}

		swprintf_s(sz_work, static_cast<unsigned>(_countof(sz_work)), _T("%s %s"), sz_date, sz_time);
		if (d_s_start == 0.0 || d_s_start > d_time)
		{
			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), _T(" "));
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_time);
			d_s_start = d_time;
		}

		switch (k)
		{
			/*----- レディネスチェック処理 -----*/
		case 0:
			(stpd.l_rdychk) |= 0x01;
			break;

		case 1:
			(stpd.l_rdychk) |= 0x02;
			break;

			/*----- 初期設定処理 -----*/
		case 2:
			stpd.l_init = 1;
			break;

			/*----- ビットレートＳ -----*/
		case 3:
			stpd.sz_s_bitrate = CString(sz_ele[1]);
			break;

			/*----- ビットレートＸ -----*/
		case 4:
			stpd.sz_x_bitrate = CString(sz_ele[1]);
			break;

			/*----- ビットレートＸＱＰＳＫ -----*/
		case 17:
			stpd.sz_x_qpsk = CString(sz_ele[1]);
			break;

			/*----- S/X コマンドビットレート -----*/
		case 5:
			stpd.sz_cmd_bit_rate = CString(sz_ele[1]);
			break;

			/*----- X コマンドビットレート -----*/
		case 35:
			stpd.sz_x_cmd_bit_rate = CString(sz_ele[1]);
			break;

			/*----- 開始時刻処理 -----*/
		case 12:
			stpd.sz_start.Format(_T("%s %s"), sz_date, sz_time);

			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), _T(" "));
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_time);

			TCHAR sz_lwork[50];
			__int64 d_work;
			__int64 l_tm;
			swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s.0000"), sz_s_start);

			d_work = StrTimeToCTime(CString(sz_lwork)).GetTime();
			l_tm = d_work - d_tm;
			stpd.l_ant_time = (long)(l_tm / 60);
			break;

			/*----- 終了時刻処理 -----*/
		case 13:
			stpd.sz_end.Format(_T("%s %s"), sz_date, sz_time);
			break;

			/*----- S-TX 送信出力選択処理 -----*/
		case 32:
			/* 送信機-送信出力-S帯送信周波数変換盤を解析 */
			memset(sz_txrfpwr, 0, sizeof(sz_txrfpwr));

			swscanf_s(sz_ele[1], _T("%[^_]_%[^_]_%[^_]"),
				sz_txrfpwr[0], static_cast<unsigned>(_countof(sz_txrfpwr[0])),
				sz_txrfpwr[1], static_cast<unsigned>(_countof(sz_txrfpwr[1])),
				sz_txrfpwr[2], static_cast<unsigned>(_countof(sz_txrfpwr[2])));

			sz_txcomp.Format(_T("%s_%s"), sz_txrfpwr[0], sz_txrfpwr[1]);
			for (i = 0; sz_s_tx_pwr_sel[i] != _T(""); ++i)
			{
				if (sz_s_tx_pwr_sel[i] == sz_txcomp)
				{
					stpd.l_tx_pwr_sel = i;
					break;
				}
			}
			/* S帯送信周波数変換盤 追加 */
			stpd.l_tx_uc_sel = i;
			break;

			/*----- X-TX 送信出力選択処理 -----*/
		case 33:
			for (i = 0; sz_x_tx_pwr_sel[i] != _T(""); ++i)
			{
				if (sz_x_tx_pwr_sel[i] == CString(sz_ele[1]))
				{
					stpd.l_tx_pwr_sel = i;
					break;
				}
			}
			break;

			/*----- Ｘ帯変調装置選択処理 -----*/
		case 34:
			stpd.l_mod_sel = i;
			break;

		case 36: //レディネスチェック（AGC)
//			if ((CString(sz_ele[1]).Find(STR_SX_ACQRX_S) != -1) ||
//				(CString(sz_ele[1]).Find(STR_SX_ACQRX_X) != -1) ||
//				(CString(sz_ele[1]).Find(STR_SS_TCR) != -1) ||
//				(CString(sz_ele[1]).Find(STR_S_RX) != -1) ||
//				(CString(sz_ele[1]).Find(STR_X_RX) != -1) ||
//				(CString(sz_ele[1]) == STR_X_QPSKRX_A) ||
//				(CString(sz_ele[1]) == STR_DSN_MODEM_A))
			{
				stpd.l_agc_index = 1;
				// もっとも古い時刻を調べる
				CTime ct = StrTimeToCTime(CString(sz_work));
				time_t rdy_t = ct.GetTime();
				if (rdy_t < stpd.l_agc_time)
				{
					stpd.l_agc_time = rdy_t;
				}
		}
			break;

		case 37: //レディネスチェック（RNG)
//			if ((CString(sz_ele[1]) == STR_SS) ||
//				(CString(sz_ele[1]) == STR_SX) ||
//				(CString(sz_ele[1]) == STR_DXN) ||
//				(CString(sz_ele[1]) == STR_DXR))
			{
				stpd.l_rng_index = 1;
				// もっとも古い時刻を調べる
				CTime ct = StrTimeToCTime(CString(sz_work));
				time_t rdy_t = ct.GetTime();
				if (rdy_t < stpd.l_rng_time)
				{
					stpd.l_rng_time = rdy_t;
				}
			}
			break;

		case 38: //レディネスチェック（テレメトリ伝送）
//			if ((CString(sz_ele[1]) == STR_S) ||
//				(CString(sz_ele[1]) == STR_X2) ||
//				(CString(sz_ele[1]) == STR_QPSK))
			{
				stpd.l_tlm_index = 1;
				// もっとも古い時刻を調べる
				CTime ct = StrTimeToCTime(CString(sz_work));
				time_t rdy_t = ct.GetTime();
				if (rdy_t < stpd.l_tlm_time)
				{
					stpd.l_tlm_time = rdy_t;
				}
			}
			break;

		case 39: //レディネスチェック（コマンド伝送）
//			if ((CString(sz_ele[1]) == STR_1) ||
//				(CString(sz_ele[1]) == STR_3))
			{
				stpd.l_cmd_index = 1;
				// もっとも古い時刻を調べる
				CTime ct = StrTimeToCTime(CString(sz_work));
				time_t rdy_t = ct.GetTime();
				if (rdy_t < stpd.l_cmd_time)
				{
					stpd.l_cmd_time = rdy_t;
				}
			}
			break;

		case 40: //アンテナ待ち受け指向
			stpd.l_antwait_flg = 1;
			{
				// 「ANT待ち受け指向」「設備初期設定」内の最小時刻を調べる
				// 「ANT待ち受け指向」が見つかった
				CString work;
				work.Format(_T("%s %s"), sz_date, sz_time);
				CTime ct = StrTimeToCTime(work);
				if (ct != CTime(0)){
					stpd.l_antwait_time = ct.GetTime();
				}
			}
			break;

		case 41:
			if (CString(sz_ele[1]).Find(STR_ANT) >= 0)
			{
				TCHAR sz_lwork[50];
				stpd.l_antwait_flg = 1;

				swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s %s.0000"), sz_date, sz_time);
				d_tm = StrTimeToCTime(CString(sz_lwork)).GetTime();
				stpd.l_antinit_time = d_tm;
			}
			break;

		case 57: // UP START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_uplink_s_timelst.push_back(CString(strTime));
			break;

		case 58: // UP END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_uplink_e_timelst.push_back(CString(strTime));
			break;

		case 59: // RNG_START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_rng_s_timelst.push_back(CString(strTime));

			// 種別
			stpd.sz_rng_kindlst.push_back(ExchagePlanFileRngType(CString(sz_ele[1])));
			break;

		case 60: // RNG_END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_rng_e_timelst.push_back(CString(strTime));
			break;

		case 61: // CMD_START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_cmd_s_timelst.push_back(CString(strTime));
			break;

		case 62: // CMD_END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_cmd_e_timelst.push_back(CString(strTime));
			break;

		case 63: // TLM_START
			if (CString(sz_ele[1]) == TLM_S)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlms_s_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_X)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmx_s_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_KA)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmka_s_timelst.push_back(CString(strTime));
			}
			break;

		case 64: // TLM_END
			if (CString(sz_ele[1]) == TLM_S)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlms_e_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_X)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmx_e_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_KA)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmka_e_timelst.push_back(CString(strTime));
			}
			break;

		}
	}

	fclose(fp_plan);
	return 0;
}

int CTimeLineView::ReadPlanFile20(CString sz_satname, CString sz_passno, stPlandata& stpd, LPARAM lParam)
{
	FILE	*fp_plan;
	CString sz_p_file;
	CString sz_p_ftpfile;

	TCHAR sz_s_start[50];
	TCHAR sz_buff[512];
	TCHAR sz_work[256];
	TCHAR sz_date[50] = { 0 };
	TCHAR sz_time[50] = { 0 };
	TCHAR sz_txrfpwr[3][20];

	TCHAR sz_work2[256];
	TCHAR sz_work3[256];
	TCHAR sz_work4[256];
	TCHAR sz_work5[256];
	TCHAR sz_work6[256];
	TCHAR sz_work7[256];
	TCHAR sz_work8[256];

	long i;
	long j;
	long l;
	long k;
	long l_rc;

	__int64 d_s_start;
	__int64 d_time;

	CString str;
	CString sz_txcomp;
	CString strTime;

	TCHAR sz_ele[10][100];
	memset(&sz_ele[0], 0, static_cast<unsigned>(_countof(sz_ele)));

#if 1
	CString strPlanPath = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
	CString strFtpPlanPath = theApp.GetShareFilePath(eFileType_PlanOut_Folder, theApp.GetSelectStation());
	if (lParam == 1){
		sz_p_file.Format(_T("%senp\\%s\\%s.pln"), (LPCTSTR)strPlanPath, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
		sz_p_ftpfile.Format(_T("%s%s\\%s.pln"), (LPCTSTR)strFtpPlanPath, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	}
	else{
		sz_p_file.Format(_T("%ssttn\\%s\\%s.pln"), (LPCTSTR)strPlanPath, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
		sz_p_ftpfile.Format(_T("%s%s\\%s.pln"), (LPCTSTR)strFtpPlanPath, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	}
#else
	CString ca_cthome;
	ca_cthome.Format(_T("%s%s"), theApp.GetAppPath(), DATA_DIR);

	j = (long)strlen(sd.satinfo.sz_plan_dir);
	if (sd.satinfo.sz_plan_dir[j - 1] == '/')
	{
		sz_p_file.Format(_T("%s%s.pln"), CString(sd.satinfo.sz_plan_dir), sz_passno);
	}
	else
	{
		sz_p_file.Format(_T("%s\\%s.pln"), CString(sd.satinfo.sz_plan_dir), sz_passno);
	}
#endif
	sz_s_start[0] = '\0';


	/*----- ftp file get  "sz_p_file" -----*/
	CString path(strPlanPath);
	CString file(sz_p_file);

	//	if (GetPlanFile(path, file) == FALSE)
	//	{
	//		return -1;
	//	}

	//if (_wfopen_s(&fp_plan, sz_p_file, _T("r, ccs=UTF-8")) != 0)
	if (_wfopen_s(&fp_plan, sz_p_file, _T("r")) != 0)
	{
		return -1;
	}

	stpd.sz_pred_path = _T("");
	stpd.sz_filepath = _T("");
	stpd.sz_ftpfilepath = _T("");
	stpd.sz_start = _T("");
	stpd.sz_end = _T("");
	stpd.sz_s_bitrate = _T("");
	stpd.sz_x_bitrate = _T("");
	stpd.sz_x_qpsk = _T("");
	stpd.sz_cmd_bit_rate = _T("");
	stpd.sz_x_cmd_bit_rate = _T("");
	stpd.l_cmd_mod = 0;
	stpd.l_rng_mod = 0;
	stpd.sz_rng_band = _T("");
	stpd.sz_rng_start = _T("");
	stpd.sz_rng_end = _T("");
	stpd.l_rdychk = 0;
	stpd.l_init = 0;
	stpd.l_auto_acq = 0;
	stpd.l_offset = 0;
	stpd.l_a_flag = 0;
	stpd.l_l_flag = 0;
	stpd.l_term_ant = 1;
	stpd.l_tx_ramp = 0;
	stpd.l_rx_ramp = 0;
	stpd.l_acq_route = 0;
	stpd.l_pl_sel = 0;
	stpd.l_top_mon = 0;
	stpd.l_ent_angle = 0;
	stpd.l_ext_angle = 0;
	stpd.sz_level_s = _T("");
	stpd.sz_level_x = _T("");
	stpd.sz_ant_s = _T("");
	stpd.sz_ant_x = _T("");
	stpd.l_delay_sel = 0;
	stpd.l_tx_pwr_sel = 0;
	stpd.l_mod_sel = 0;
	stpd.l_tx_uc_sel = -1;
	stpd.l_antwait_flg = 0;
	stpd.l_cmd_index = 0;
	stpd.l_tlm_index = 0;
	stpd.l_agc_index = 0;
	stpd.l_rng_index = 0;
	stpd.l_range_band = 0;
	stpd.l_ant_stow = 2;
	stpd.sz_cid = _T("");
	stpd.sz_station = _T("");
	stpd.sz_priority = _T("");
	stpd.sz_ope_mode = _T("");
	stpd.sz_uplink_s_timelst.clear();
	stpd.sz_uplink_e_timelst.clear();
	stpd.l_rng = 0;
	stpd.sz_rng_s_timelst.clear();
	stpd.sz_rng_e_timelst.clear();
	stpd.sz_rng_kindlst.clear();
	stpd.l_cmd = 0;
	stpd.sz_cmd_s_timelst.clear();
	stpd.sz_cmd_e_timelst.clear();
	stpd.l_tlms = 0;
	stpd.sz_tlms_s_timelst.clear();
	stpd.sz_tlms_e_timelst.clear();
	stpd.l_tlmx = 0;
	stpd.sz_tlmx_s_timelst.clear();
	stpd.sz_tlmx_e_timelst.clear();
	stpd.l_tlmka = 0;
	stpd.sz_tlmka_s_timelst.clear();
	stpd.sz_tlmka_e_timelst.clear();

	// 「ANT待ち受け指向」「設備初期設定」内の最小時刻を調べる
	stpd.l_antwait_time = 0;
	stpd.l_antinit_time = 0;
	stpd.l_initmin_time = 0;

	__int64 d_tm;

	d_s_start = 0;
	d_time = 0;

	stpd.sz_filepath = sz_p_file;
	stpd.sz_ftpfilepath = sz_p_ftpfile;

	// 計画ファイルタイプ
	stpd.sz_plan_type = _T("");

	// レディネスチェック時間
	stpd.l_agc_time = LONG64_MAX;
	stpd.l_rng_time = LONG64_MAX;
	stpd.l_tlm_time = LONG64_MAX;
	stpd.l_cmd_time = LONG64_MAX;

	char sz_line[512];
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_plan) != 0)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			if (wcsncmp(sz_buff, SOS_HEADER_ID, wcslen(SOS_HEADER_ID)) == 0)
			{
				swscanf_s(sz_buff, _T("%s%s%s%s%s%s%s%s"),
					sz_work, static_cast<unsigned>(_countof(sz_work)),
					sz_work2, static_cast<unsigned>(_countof(sz_work2)),
					sz_work3, static_cast<unsigned>(_countof(sz_work3)),
					sz_work4, static_cast<unsigned>(_countof(sz_work4)),
					sz_work5, static_cast<unsigned>(_countof(sz_work5)),
					sz_work6, static_cast<unsigned>(_countof(sz_work6)),
					sz_work7, static_cast<unsigned>(_countof(sz_work7)),
					sz_work8, static_cast<unsigned>(_countof(sz_work8)));

				// 計画タイプ
				if (stpd.sz_plan_type.IsEmpty() == true)
					stpd.sz_plan_type = CString(sz_work2);

				// 予報値ファイル名
				if (stpd.sz_pred_path.IsEmpty() == true)
					stpd.sz_pred_path = CString(sz_work8);
			}

			l_rc = swscanf_s(&sz_buff[1], _T("%s%s%s%s%s%s%s%s%s%s"),
				sz_ele[0], static_cast<unsigned>(_countof(sz_ele[0])),
				sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])),
				sz_ele[2], static_cast<unsigned>(_countof(sz_ele[2])),
				sz_ele[3], static_cast<unsigned>(_countof(sz_ele[3])),
				sz_ele[4], static_cast<unsigned>(_countof(sz_ele[4])),
				sz_ele[5], static_cast<unsigned>(_countof(sz_ele[5])),
				sz_ele[6], static_cast<unsigned>(_countof(sz_ele[6])),
				sz_ele[7], static_cast<unsigned>(_countof(sz_ele[7])),
				sz_ele[8], static_cast<unsigned>(_countof(sz_ele[8])),
				sz_ele[9], static_cast<unsigned>(_countof(sz_ele[9])));

			for (i = 0; i < l_rc; ++i)
			{
//				str = CString(sz_ele[i]);
				AfxExtractSubString(str, CString(sz_ele[i]), 0, '=');

				/*---------- 初期値 チェック ----------*/
				if (str.Find(sz_c_ele20[2]) >= 0)
				{
					j = sz_c_ele20[2].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_init = k;
							break;
						}
					}
				}

				/*---------- CMD MOD の AUTO/MANチェック ----------*/
				if (str.Find(sz_c_ele20[6]) >= 0)
				{
					j = sz_c_ele20[6].GetLength() + 1;
					for (k = 0; sz_auto[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_auto[k])
						{
							stpd.l_cmd_mod = k;
							break;
						}
					}
				}
				/*---------- RNG MOD の AUTO/MANチェック ----------*/
				if (str.Find(sz_c_ele20[7]) >= 0)
				{
					j = sz_c_ele20[7].GetLength() + 1;
					for (k = 0; sz_auto[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_auto[k])
						{
							stpd.l_rng_mod = k;
							break;
						}
					}
				}
				/*---------- RNG BAND の 取り込み ----------*/
				if (str.Find(sz_c_ele20[8]) >= 0)
				{
					j = sz_c_ele20[8].GetLength() + 1;
					stpd.sz_rng_band = CString(&sz_ele[i][j]);
				}
				/*---------- RNG START の 取り込み ----------*/
				if (str.Find(sz_c_ele20[9]) >= 0)
				{
					j = sz_c_ele20[9].GetLength() + 1;
					stpd.sz_rng_start = CString(&sz_ele[i][j]);
				}
				/*---------- RNG END の 取り込み ----------*/
				if (str.Find(sz_c_ele20[10]) >= 0)
				{
					j = sz_c_ele20[10].GetLength() + 1;
					stpd.sz_rng_end = CString(&sz_ele[i][j]);
				}
				/*---------- 時刻オフセットへの追従 ----------*/
				if (str.Find(sz_c_ele20[11]) >= 0)
				{
					j = sz_c_ele20[11].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_offset = k;
							break;
						}
					}
				}
				/*---------- AOS FLAG ----------*/
				if (str.Find(sz_c_ele20[14]) >= 0)
				{
					j = sz_c_ele20[14].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_a_flag = k;
							break;
						}
					}
				}
				/*---------- LOS FLAG ----------*/
				if (str.Find(sz_c_ele20[15]) >= 0)
				{
					j = sz_c_ele20[15].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_l_flag = k;
							break;
						}
					}
				}
				/*---------- 自動捕捉の有無チェック ----------*/
				if (str.Find(sz_c_ele20[16]) >= 0)
				{
					j = sz_c_ele20[16].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_auto_acq = k;
							break;
						}
					}
				}
				/*---------- パス終了時のＡＮＴモード ----------*/
				if (str.Find(sz_c_ele20[18]) >= 0)
				{
					j = sz_c_ele20[18].GetLength() + 1;
					for (k = 0; sz_term_ant[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_term_ant[k])
						{
							stpd.l_term_ant = k;
							break;
						}
					}
				}
				/*---------- 送信設備ランプ制御有無 ----------*/
				if (str.Find(sz_c_ele20[19]) >= 0)
				{
					j = sz_c_ele20[19].GetLength() + 1;
					for (k = 0; sz_tx_ramp[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_tx_ramp[k])
						{
							stpd.l_tx_ramp = k;
							break;
						}
					}
				}
				/*---------- 受信設備ランプ制御有無 ----------*/
				if (str.Find(sz_c_ele20[20]) >= 0)
				{
					j = sz_c_ele20[20].GetLength() + 1;
					for (k = 0; sz_rx_ramp[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_rx_ramp[k])
						{
							stpd.l_rx_ramp = k;
							break;
						}
					}
				}
				/*---------- 補足ルート ----------*/
				if (str.Find(sz_c_ele20[21]) >= 0)
				{
					j = sz_c_ele20[21].GetLength() + 1;
					stpd.l_acq_route = _wtoi(&sz_ele[i][j]);
					stpd.l_acq_route -= 1;	/* 配列番号に変換 */
				}
				/*---------- ACQ/MAIN判定有無 ----------*/
				if (str.Find(sz_c_ele20[22]) >= 0)
				{
					j = sz_c_ele20[22].GetLength() + 1;
					for (k = 0; sz_on_off[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on_off[k])
						{
							stpd.l_pl_sel = k;
							break;
						}
					}
				}
				/*---------- 天頂通過監視有無 ----------*/
				if (str.Find(sz_c_ele20[23]) >= 0)
				{
					j = sz_c_ele20[23].GetLength() + 1;
					for (k = 0; sz_on_off[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on_off[k])
						{
							stpd.l_top_mon = k;
							break;
						}
					}
				}
				/*---------- 天頂通過監視入角 ----------*/
				if (str.Find(sz_c_ele20[24]) >= 0)
				{
					j = sz_c_ele20[24].GetLength() + 1;
					stpd.l_ent_angle = _wtoi(&sz_ele[i][j]);
				}
				/*---------- 天頂通過監視出角 ----------*/
				if (str.Find(sz_c_ele20[25]) >= 0)
				{
					j = sz_c_ele20[25].GetLength() + 1;
					stpd.l_ext_angle = _wtoi(&sz_ele[i][j]);
				}
				/*---------- 再生測距対応　局内ディレイ値選択 ----------*/
				if (str.Find(sz_c_ele20[30]) >= 0)
				{
					j = sz_c_ele20[30].GetLength() + 1;
					for (k = 0; sz_delay_sel[k] != _T(""); ++k) {
						if (CString(&sz_ele[i][j]) == sz_delay_sel[k])
						{
							stpd.l_delay_sel = k;
							break;
						}
					}
				}
				/*---------- S帯RNG計測帯域 ----------*/
				if (str.Find(sz_c_ele20[42]) >= 0)
				{
					j = sz_c_ele20[42].GetLength() + 1;
					for (k = 0; sz_range_band[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_range_band[k])
						{
							stpd.l_range_band = k;
							break;
						}
					}
				}
				/*---------- ANT天頂指向有無 ----------*/
				if (str.Find(sz_c_ele20[43]) >= 0)
				{
					j = sz_c_ele20[43].GetLength() + 1;
					for (k = 0; sz_ant_stow[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_ant_stow[k])
						{
							stpd.l_ant_stow = k;
							break;
						}
					}
				}

				/*---------- CID ----------*/
				if (str.Find(sz_c_ele20[49]) >= 0)
				{
					j = sz_c_ele20[49].GetLength() + 1;
					stpd.sz_cid = CString(&sz_ele[i][j]);
				}

				/*---------- 局名 ----------*/
				if (str.Find(sz_c_ele20[50]) >= 0)
				{
					j = sz_c_ele20[50].GetLength() + 1;
					stpd.sz_station = CString(&sz_ele[i][j]);
				}

				/*---------- 主従局 ----------*/
				if (str.Find(sz_c_ele20[51]) >= 0)
				{
					j = sz_c_ele20[51].GetLength() + 1;
					stpd.sz_priority = CString(&sz_ele[i][j]);
				}

				/*---------- オペレーションモード ----------*/
				if (str.Find(sz_c_ele20[52]) >= 0)
				{
					j = sz_c_ele20[52].GetLength() + 1;
					stpd.sz_ope_mode = CString(&sz_ele[i][j]);
				}

				/*---------- 自動生成 ----------*/
				if (str.CompareNoCase(sz_c_ele20[63]) == 0)
				{
					j = sz_c_ele20[63].GetLength() + 1;
					stpd.sz_auto = CString(&sz_ele[i][j]);
				}
			}
			continue;
		}

		swscanf_s(sz_buff, _T("%s%s%s"),
			sz_date, static_cast<unsigned>(_countof(sz_date)),
			sz_time, static_cast<unsigned>(_countof(sz_time)),
			sz_work, static_cast<unsigned>(_countof(sz_work)));

		// 「ANT待ち受け指向」「設備初期設定」内の最小時刻を調べる
		CString temp = CString(sz_buff);
		if (temp.Find(_T("INIT.")) >= 0){
			// 「設備初期設定」が見つかった
			CString work;
			work.Format(_T("%s %s"), sz_date, sz_time);
			CTime ct = StrTimeToCTime(work);
			if (ct != CTime(0) && stpd.l_initmin_time != 0 && ct.GetTime() < stpd.l_initmin_time){
				stpd.l_initmin_time = ct.GetTime();
			}
			else if (ct != CTime(0) && stpd.l_initmin_time == 0){
				stpd.l_initmin_time = ct.GetTime();
			}
		}

		j = 0;
		l = 0;
		for (k = 0; sz_work[k] != '\0'; ++k)
		{
			if (sz_work[k] == ',')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_work[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
				break;
			}
			else if (sz_work[k] != ' ')
			{
				sz_ele[j][l] = sz_work[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			continue;
		}

		/* X帯送受信ルート判定文字列を探す */
		str = CString(sz_ele[0]);
		if (str.Find(X_ROUTE_DEF_TXT) >= 0)
		{
			stpd.l_route_mode = 1;
		}

		for (k = 0; sz_c_ele20[k] != _T(""); ++k)
		{
			if ((k == 2) || (k == 4) || (k == 35))
			{	/* 判定ケースを追加 */
				if (sz_c_ele20[k].Find(str) >= 0)
				{
					break;
				}
			}
			else
			{
				if (str == sz_c_ele20[k])
				{
					break;
				}
			}
		}

		if (sz_c_ele20[k] == _T(""))
		{
			continue;
		}

		swprintf_s(sz_work, static_cast<unsigned>(_countof(sz_work)), _T("%s %s"), sz_date, sz_time);
		if (d_s_start == 0.0 || d_s_start > d_time)
		{
			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), _T(" "));
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_time);
			d_s_start = d_time;
		}

		switch (k)
		{
			/*----- レディネスチェック処理 -----*/
		case 0:
			(stpd.l_rdychk) |= 0x01;
			break;

		case 1:
			(stpd.l_rdychk) |= 0x02;
			break;

			/*----- 初期設定処理 -----*/
		case 2:
			stpd.l_init = 1;
			break;

			/*----- ビットレートＳ -----*/
		case 3:
			stpd.sz_s_bitrate = CString(sz_ele[1]);
			break;

			/*----- ビットレートＸ -----*/
		case 4:
			stpd.sz_x_bitrate = CString(sz_ele[1]);
			break;

			/*----- ビットレートＸＱＰＳＫ -----*/
		case 17:
			if (CString(sz_ele[0]) == BIT_XQPSK)
			{
				stpd.sz_x_qpsk = CString(sz_ele[1]);
				break;
			}
			else
			{
				stpd.sz_x_bitrate = CString(sz_ele[1]);
				break;
			}
			break;

			/*----- S/X コマンドビットレート -----*/
		case 5:
			stpd.sz_cmd_bit_rate = CString(sz_ele[1]);
			break;

			/*----- X コマンドビットレート -----*/
		case 35:
			stpd.sz_x_cmd_bit_rate = CString(sz_ele[1]);
			break;

			/*----- 開始時刻処理 -----*/
		case 12:
			stpd.sz_start.Format(_T("%s %s"), sz_date, sz_time);

			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), _T(" "));
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_time);

			TCHAR sz_lwork[50];
			__int64 d_work;
			__int64 l_tm;
			swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s.0000"), sz_s_start);

			d_work = StrTimeToCTime(CString(sz_lwork)).GetTime();
			l_tm = d_work - d_tm;
			stpd.l_ant_time = (long)(l_tm / 60);
			break;

			/*----- 終了時刻処理 -----*/
		case 13:
			stpd.sz_end.Format(_T("%s %s"), sz_date, sz_time);
			break;

			/*----- S-TX 送信出力選択処理 -----*/
		case 32:
			/* 送信機-送信出力-S帯送信周波数変換盤を解析 */
			memset(sz_txrfpwr, 0, sizeof(sz_txrfpwr));

			swscanf_s(sz_ele[1], _T("%[^_]_%[^_]_%[^_]"),
				sz_txrfpwr[0], static_cast<unsigned>(_countof(sz_txrfpwr[0])),
				sz_txrfpwr[1], static_cast<unsigned>(_countof(sz_txrfpwr[1])),
				sz_txrfpwr[2], static_cast<unsigned>(_countof(sz_txrfpwr[2])));

			sz_txcomp.Format(_T("%s_%s"), sz_txrfpwr[0], sz_txrfpwr[1]);
			for (i = 0; sz_tx_pwr_sel20[i] != _T(""); ++i)
			{
				if (sz_tx_pwr_sel20[i] == sz_txcomp)
				{
					stpd.l_tx_pwr_sel = i;
					break;
				}
			}
			/* S帯送信周波数変換盤 追加 */
			stpd.l_tx_uc_sel = i;
			break;

		case 36: //レディネスチェック（AGC)
//			if ((CString(sz_ele[1]).Find(STR_SX_ACQRX_S) >= 0) ||
//				(CString(sz_ele[1]).Find(STR_S_RX) >= 0) ||
//				(CString(sz_ele[1]).Find(STR_SS_TCRDET) >= 0) ||
//				(CString(sz_ele[1]).Find(STR_X_RX) >= 0) ||
//				(CString(sz_ele[1]) == STR_X_QPSKRX_A))
			{
				stpd.l_agc_index = 1;
				// もっとも古い時刻を調べる
				CTime ct = StrTimeToCTime(CString(sz_work));
				time_t rdy_t = ct.GetTime();
				if (rdy_t < stpd.l_agc_time)
				{
					stpd.l_agc_time = rdy_t;
				}
			}
			break;

		case 37: //レディネスチェック（RNG)
//			if ((CString(sz_ele[1]) == STR_SS) ||
//				(CString(sz_ele[1]) == STR_SX))
			{
				stpd.l_rng_index = 1;
				// もっとも古い時刻を調べる
				CTime ct = StrTimeToCTime(CString(sz_work));
				time_t rdy_t = ct.GetTime();
				if (rdy_t < stpd.l_rng_time)
				{
					stpd.l_rng_time = rdy_t;
				}
			}
			break;

		case 38: //レディネスチェック（テレメトリ伝送）
//			if ((CString(sz_ele[1]) == STR_S) ||
//				(CString(sz_ele[1]) == STR_X) ||
//				(CString(sz_ele[1]) == STR_QPSK))
			{
				stpd.l_tlm_index = 1;
				// もっとも古い時刻を調べる
				CTime ct = StrTimeToCTime(CString(sz_work));
				time_t rdy_t = ct.GetTime();
				if (rdy_t < stpd.l_tlm_time)
				{
					stpd.l_tlm_time = rdy_t;
				}
			}
			break;

		case 39: //レディネスチェック（コマンド伝送）
//			if (CString(sz_ele[1]) == STR_1)
			{
				stpd.l_cmd_index = 1;
				// もっとも古い時刻を調べる
				CTime ct = StrTimeToCTime(CString(sz_work));
				time_t rdy_t = ct.GetTime();
				if (rdy_t < stpd.l_cmd_time)
				{
					stpd.l_cmd_time = rdy_t;
				}
			}
			break;

		case 40: //アンテナ待ち受け指向
			stpd.l_antwait_flg = 1;
			{
				// 「ANT待ち受け指向」「設備初期設定」内の最小時刻を調べる
				// 「ANT待ち受け指向」が見つかった
				CString work;
				work.Format(_T("%s %s"), sz_date, sz_time);
				CTime ct = StrTimeToCTime(work);
				if (ct != CTime(0)){
					stpd.l_antwait_time = ct.GetTime();
				}
			}
			break;

		case 41:
			if (CString(sz_ele[1]).Find(STR_ANT) >= 0)
			{
				TCHAR sz_lwork[50];
				stpd.l_antwait_flg = 1;

				swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s %s.0000"), sz_date, sz_time);
				d_tm = StrTimeToCTime(CString(sz_lwork)).GetTime();
				stpd.l_antinit_time = d_tm;
			}
			break;

		case 55: // UP START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_uplink_s_timelst.push_back(CString(strTime));
			break;

		case 56: // UP END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_uplink_e_timelst.push_back(CString(strTime));
			break;

		case 57: // RNG_START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_rng_s_timelst.push_back(CString(strTime));

			// 種別
			stpd.sz_rng_kindlst.push_back(ExchagePlanFileRngType(CString(sz_ele[1])));
			break;

		case 58: // RNG_END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_rng_e_timelst.push_back(CString(strTime));
			break;

		case 59: // CMD_START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_cmd_s_timelst.push_back(CString(strTime));
			break;

		case 60: // CMD_END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_cmd_e_timelst.push_back(CString(strTime));
			break;

		case 61: // TLM_START
			if (CString(sz_ele[1]) == TLM_S)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlms_s_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_X)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmx_s_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_KA)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmka_s_timelst.push_back(CString(strTime));
			}
			break;

		case 62: // TLM_END
			if (CString(sz_ele[1]) == TLM_S)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlms_e_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_X)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmx_e_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_KA)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmka_e_timelst.push_back(CString(strTime));
			}

			break;
		}
	}

	fclose(fp_plan);
	return 0;
}

///*============================================================================*/
///*! S帯受信
//
//-# パス状況運用プロパティステータスの更新
//
//@param
//@retval
//
//*/
///*============================================================================*/
//void CTimeLineView::UpdateStatus()
//{
//	UpdateData(TRUE);
//
//	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
//
//	int szControl = sizeof(mOperationParam) / sizeof(mOperationParam[0]);
//
//	UINT sta = theApp.GetSelectStation();
//
//	for (int i = 0; i < szControl; i++)
//	{
//		CString mControlString = mOperationParam[i].name;
//		CString str;
//		stControlValue val;
//		val.ret = 0;
//		val.type = mOperationParam[i].type;
//
//		if (mControlString.IsEmpty())
//		{
//			continue;
//		}
//
//		// 監視データから取得
//		obs_adr = (CEngValCnv::obsname_t *)theApp.GetObsname_t(sta, mControlString);
//
//		if (obs_adr == NULL)
//		{
//			continue;
//		}
//
//		switch (val.type){
//		case	eControlStatus:
//			str.Format(_T("%s"), CString(obs_adr->sz_sts_name));
//			break;
//		case	eControlNumeric:
//			str.Format(_T("%d"), obs_adr->l_data);
//			break;
//		case	eControlDouble:
//			str.Format(_T("%0.3f"), obs_adr->d_data);
//			break;
//		case	eControlTime:
//			str.Format(_T("%s"), CString(obs_adr->sz_sts_name));
//			break;
//		}
//
//		// 文字・表示色セット
//		switch (mOperationParam[i].id)
//		{
//			// 衛星名
//			// 計画ID
//		case IDC_STATIC_SPACEVAL:
//		case IDC_STATIC_PLANIDVAL:
//			GetDlgItem(mOperationParam[i].id)->SetWindowText(str);
//			break;
//
//			// 予報値
//		case IDC_STATIC_FORECASTVAL:
//			str.Trim(_T(" "));
//			if (!str.IsEmpty())
//			{
//				CString strYohoID = ConvertPredfileToId(str);
//				GetDlgItem(mOperationParam[i].id)->SetWindowText(strYohoID);
//			}
//			break;
//
//			// 運用状態
//		case IDC_STATIC_OPESTATUSVAL:
//			if (obs_adr->d_data == 1.0)
//			{
//				str.Format(_T("パス運用中"));
//			}
//			else
//			{
//				str.Format(_T("非運用中"));
//			}
//			GetDlgItem(mOperationParam[i].id)->SetWindowText(str);
//			break;
//
//			// 運用開始日時
//		case IDC_STATIC_OPESTARTVAL:
//			// 運用終了日時
//		case IDC_STATIC_OPESSTOPVAL:
//			str.Trim(_T(" "));
//			if (!str.IsEmpty())
//			{
//				char	  ca_stime[40];				/* パス運用開始時刻（フル）   */
//				ub_asc_to_asctime(obs_adr->sz_sts_name, ca_stime);
//				GetDlgItem(mOperationParam[i].id)->SetWindowText(CString(ca_stime));
//			}
//			break;
//
//		case IDC_STATIC_OPESTARTUNTILVAL:
//			break;
//		case IDC_STATIC_OPESTOPUNTILVAL:
//			break;
//		case IDC_STATIC_UPLNKSTARTUNTILVAL:
//			break;
//		case IDC_STATIC_UPLNKSTOPUNTILVAL:
//			break;
//		case IDC_STATIC_RNGSTARTUNTILVAL:
//			break;
//		case IDC_STATIC_RNGSTOPUNTILVAL:
//			break;
//		case IDC_STATIC_CMDSTARTUNTILVAL:
//			break;
//		case IDC_STATIC_CMDSTOPUNTILVAL:
//			break;
//		case IDC_STATIC_OPEOPSENDVAL:
//			break;
//		case IDC_STATIC_OPEOPRECVVAL:
//			break;
//		case IDC_STATIC_OPEOPSENDOUTVAL:
//			break;
//		case IDC_STATIC_OPEOPLNAVAL:
//			GetDlgItem(mOperationParam[i].id)->SetWindowText(str);
//			break;
//		case IDC_STATIC_OPEOPANTWAITVAL:
//			break;
//		case IDC_STATIC_OPEOPANTVAL:
//			break;
//		case IDC_STATIC_READAGCVAL:
//			break;
//		case IDC_STATIC_READRNGVAL:
//			break;
//		case IDC_STATIC_READTLMVAL:
//			break;
//		case IDC_STATIC_READCMDVAL:
//			break;
//		case IDC_STATIC_STLMRUNVAL:
//			break;
//		case IDC_STATIC_STLMSTARTVAL:
//			break;
//		case IDC_STATIC_STLMSTOPVAL:
//			break;
//		case IDC_STATIC_XTLMRUNVAL:
//			break;
//		case IDC_STATIC_XTLMSTARTVAL:
//			break;
//		case IDC_STATIC_XTLMSTOPVAL:
//			break;
//		case IDC_STATIC_KATLMRUNVAL:
//			break;
//		case IDC_STATIC_KATLMSTARTVAL:
//			break;
//		case IDC_STATIC_KATLMSTOPVAL:
//			break;
//		case IDC_STATIC_MSVAL:
//			break;
//		case IDC_EDIT_PROPEVENTAUTOVAL:
//			GetDlgItem(mOperationParam[i].id)->SetWindowText(str);
//			break;
//		case IDC_EDIT_PROPEVENTSUNSET:
//			GetDlgItem(mOperationParam[i].id)->SetWindowText(str);
//			break;
//		case IDC_EDIT_PROPEVENTSUNRISE:
//			GetDlgItem(mOperationParam[i].id)->SetWindowText(str);
//			break;
//		case IDC_EDIT_PROPMARGINULON:
//			GetDlgItem(mOperationParam[i].id)->SetWindowText(str);
//			break;
//		case IDC_EDIT_PROPMARGINULOFF:
//			GetDlgItem(mOperationParam[i].id)->SetWindowText(str);
//			break;
//		case IDC_EDIT_PROPMARGINDL:
//			GetDlgItem(mOperationParam[i].id)->SetWindowText(str);
//			break;
//		case IDC_EDIT_PROPULCMD:
//			GetDlgItem(mOperationParam[i].id)->SetWindowText(str);
//			break;
//		case IDC_EDIT_PROPULRNG:
//			GetDlgItem(mOperationParam[i].id)->SetWindowText(str);
//			break;
//		case IDC_BUTTON_PROPCID:
//			GetDlgItem(mOperationParam[i].id)->SetWindowText(str);
//			break;
//		case IDC_EDIT_PROPSPACEVAL:
//			GetDlgItem(mOperationParam[i].id)->SetWindowText(str);
//			break;
//
//		default:
//			break;
//		}
//	}
//
//	UpdateData(FALSE);
//}

/*============================================================================*/
/*! 運用状況画面

-# 予報値ファイル名から予報値IDへの変換

@param		name	予報値ファイル名
@retval 予報値ID文字列
*/
/*============================================================================*/
CString CTimeLineView::ConvertPredfileToId(CString name)
{
	TCHAR	szDir[_MAX_DIR];
	TCHAR	szDrive[_MAX_DRIVE];
	TCHAR	szFname[_MAX_FNAME];
	_wsplitpath_s(name, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, NULL, 0);
	CString str, temp;
	temp = szFname;
	str.Format(_T("%s-%s"), (LPCTSTR)(temp.Left(6)), (LPCTSTR)(temp.Mid(6)));
	return str;
}
CString CTimeLineView::ConvertPredfileToFname(CString name)
{
	TCHAR	szDir[_MAX_DIR];
	TCHAR	szDrive[_MAX_DRIVE];
	TCHAR	szFname[_MAX_FNAME];
	_wsplitpath_s(name, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, NULL, 0);
	CString str, temp;
	temp = szFname;
	str.Format(_T("%s.anp"), (LPCTSTR)temp);
	return str;
}
/*============================================================================*/
/*! CSatelliteData

-# 省略フォーマットの時刻文字列をfull-formatの時刻文字列に変換

@param  sz_time：入力時刻文字列
@            1999-07-01 11:22:33 の省略形としては以下の形式を許す
@            1999-07-01 11:22
@            1999-07-01 112233
@            1999-07-01 1122
@            1999-07-01 11
@                      .11
@                      .1122
@                      .112233
@                    01.112233
@                    01.1122
@                    01.11
@                  0701.112233
@                990701.112233
@              19990701.112233
@              19990701112233
@param	output_time:フルフォーマット出力時刻文字列
@retval	5 : yy-mm-dd hh:mm
@       6 : yy-mm-dd hh:mm:ss.s
@       7 :
@       1 : 異常終了

*/
/*============================================================================*/
int CTimeLineView::ub_asc_to_asctime(char sz_input_time[], char sz_output_time[])
{
	int i, j;
	int year, mon, mday, hour, min, sec, i_100usec;
	int i_hhmmss, i_yymmdd;
	char sz_time[40], sz_work[40], sz_dmy[40];
	char sz_yymmdd[40], sz_hhmmss[40], sz_0sec[40];
	double d_sec, d_hhmmss, d_yymmdd;
	struct tm stT;
	time_t l_time;
	int sz = 40;

	for (i = 0, sz_time[0] = NULL; sz_input_time[i] != NULL; i++){
		if (sz_input_time[i] == ' '){ continue; } /*先頭スペースサプレス*/
		strcpy_s(sz_time, sz, &sz_input_time[i]);
		break;
	};
	if (sz_time[0] == NULL){
		return(-1);
	};

	for (i = (int)strlen(sz_time) - 1, j = 0; i > 0; i--){
		if ((j == 0) && (sz_time[i] == ' ')){
			sz_time[i] = NULL; j = i; /*後ろのスペースサプレス*/
		}
		else if (sz_time[i] != ' '){
			j = i;
		}
	}

	if ((i = sscanf_s(sz_time, "%d-%d-%d %d:%d:%lf%s"
		, &year, &mon, &mday, &hour, &min, &d_sec, sz_dmy, sz)) == 6){
		sec = (int)floor(d_sec);
		i_100usec = (int)((d_sec - (double)sec)*10000.0);
		if (year < 70){
			year += 2000;
		}
		else if (year < 1900){
			year += 1900;
		}
		sprintf_s(sz_output_time, sz, "%04d-%02d-%02d %02d:%02d:%02d.%04d"
			, year, mon, mday, hour, min, sec, i_100usec);
		return(6); /* yy-mm-dd hh:mm:ss.s */
	}
	else if ((i = sscanf_s(sz_time, "%d-%d-%d %d:%d%s"
		, &year, &mon, &mday, &hour, &min, sz_dmy, sz)) == 5){
		if (year < 70){
			year += 2000;
		}
		else if (year < 1900){
			year += 1900;
		}
		sprintf_s(sz_output_time, sz, "%04d-%02d-%02d %02d:%02d:%02d.0000"
			, year, mon, mday, hour, min, 0);
		return(5); /* yy-mm-dd hh:mm */
	}
	else if ((i = sscanf_s(sz_time, "%d-%d-%d %lf%s"
		, &year, &mon, &mday, &d_hhmmss, sz_dmy, sz)) == 4){
		sscanf_s(sz_time, "%d-%d-%d %s", &year, &mon, &mday, sz_hhmmss, sz);
		if (sz_hhmmss[0] == '.'){
			sprintf_s(sz_yymmdd, sz, "%02d%02d%02d%s", year, mon, mday, sz_hhmmss);
		}
		else{
			sprintf_s(sz_yymmdd, sz, "%02d%02d%02d.%s", year, mon, mday, sz_hhmmss);
		}
		goto yymmdd_hhmmss_0sec; /* yy-mm-dd hhmmss */
	}
	else if (((i = sscanf_s(sz_time, "%lf%s", &d_yymmdd, sz_dmy, sz)) == 1)
		&& (d_yymmdd >= 0101000000.0)){ /* 000101=2000-01-01 */
		i_yymmdd = (int)floor(d_yymmdd / 1000000.0);
		i_hhmmss = (int)fmod(d_yymmdd, 1000000.0);
		if (sscanf_s(sz_time, "%[^.].%s", sz_yymmdd, sz, sz_0sec, sz) == 2){
			sprintf_s(sz_yymmdd, sz, "%06d.%06d.%s", i_yymmdd, i_hhmmss, sz_0sec);
		}
		else{
			sprintf_s(sz_yymmdd, sz, "%06d.%06d.0000", i_yymmdd, i_hhmmss);
		}
		goto yymmdd_hhmmss_0sec; /* yymmddhhmmss */
	}
	else if ((sz_time[0] == '.') && ((i = sscanf_s(sz_time + 1, "%lf%s", &d_hhmmss, sz_dmy, sz)) == 1)){
		sprintf_s(sz_yymmdd, sz, "00%s", sz_time);
		goto yymmdd_hhmmss_0sec; /* .hhmmss */
	}
	else if ((i = sscanf_s(sz_time, "%[^.].%lf%s", sz_work, sz, &d_hhmmss, sz_dmy, sz)) == 2){
		strcpy_s(sz_yymmdd, sz, sz_time);
		goto yymmdd_hhmmss_0sec; /* yymmdd.hhmmss */
	}
	else{

		strcpy_s(sz_output_time, sz, sz_time);
		return(-1);
	};

yymmdd_hhmmss_0sec:

	if (sscanf_s(sz_yymmdd, "%[^.].%[^.].%s", sz_dmy, sz, sz_hhmmss, sz, sz_0sec, sz) == 3){

		if (strlen(sz_hhmmss) != 6){
			strcpy_s(sz_output_time, sz, sz_time);
			return(-1);
		};
		strcat_s(sz_0sec, sz, "000000");
		sz_0sec[4] = NULL;
	}
	else{
		strcpy_s(sz_0sec, sz, "0000");

		switch (strlen(sz_hhmmss)){
		case 2: case 4: case 6:
			strcat_s(sz_hhmmss, sz, "000000");
			sz_hhmmss[6] = NULL;
			break;
		default: strcpy_s(sz_output_time, sz, sz_time);
			return(-1);
		};
	};

	l_time = time(NULL);
	if (localtime_s(&stT, &l_time) != 0)
	{
		return(1);
	}
	sscanf_s(sz_dmy, "%d", &i_yymmdd);
	if ((mday = i_yymmdd % 100) == 0){
		mday = stT.tm_mday; /* 1..31*/
	}
	if ((mon = (i_yymmdd / 100) % 100) == 0){
		mon = stT.tm_mon + 1; /* 0..11*/
	}
	if ((year = i_yymmdd / 10000) == 0){
		year = stT.tm_year; /* n -1900*/
	}

	switch (strlen(sz_dmy)){
	case 6: case 8:/* yymmdd */
		year = i_yymmdd / 10000;
		break;
	case 2: case 4:/* ????dd ??mmdd */
		break;
	default: strcpy_s(sz_output_time, sz, sz_time);
		return(-1);
	};
	sscanf_s(sz_hhmmss, "%d", &i_hhmmss);
	sec = i_hhmmss % 100;
	min = (i_hhmmss / 100) % 100;
	hour = i_hhmmss / 10000;

	if (year < 70){
		year += 2000;
	}
	else if (year < 1900){
		year += 1900;
	}

	sprintf_s(sz_output_time, sz, "%02d-%02d-%02d %02d:%02d:%02d.%s"
		, year, mon, mday, hour, min, sec, sz_0sec);

	return(7);
}

/*============================================================================*/
/*! 運用計画タイムライン

-# 初期設定プロパティ登録

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::OnBnClickedButtonPropinit()
{
//	UpdateData(TRUE);

	// コントロールから値を取得する
	GetDlgItem(IDC_EDIT_RECLEVELVALS)->GetWindowTextW(m_strRecLevelS);
	GetDlgItem(IDC_EDIT_RECLEVELVALX)->GetWindowTextW(m_strRecLevelX);
	GetDlgItem(IDC_EDIT_TLM)->GetWindowTextW(m_strTlm);
	GetDlgItem(IDC_EDIT_CMD)->GetWindowTextW(m_strCmd);

	double reclevels, reclevelx, tlm, cmd;
	int nStation = theApp.GetSelectStation();

	GetSatelliteXTX();

	if (mOpeProp.init.autocalc == 0)	// 予想受信レベル固定時
	{
		reclevels = _wtof(m_strRecLevelS);
		if (reclevels > -80.0 || reclevels < -179.99)
		{
			m_edRecLevelS.SetSel(0, -1);
			m_edRecLevelS.SetFocus();
			if (nStation == eStation_Usuda642)
			{
				MessageBox(_T("Ka帯 予想受信レベルは-179.99から-80.00を入力してください。"), 0, MB_OK | MB_ICONERROR);
			}
			else
			{
				MessageBox(_T("S帯 予想受信レベルは-179.99から-80.00を入力してください。"), 0, MB_OK | MB_ICONERROR);
			}
			return;
		}
		reclevelx = _wtof(m_strRecLevelX);
		if (reclevelx > -80.0 || reclevelx < -179.99)
		{
			m_edRecLevelX.SetSel(0, -1);
			m_edRecLevelX.SetFocus();
			MessageBox(_T("X帯 予想受信レベルは-179.99から-80.00を入力してください。"), 0, MB_OK | MB_ICONERROR);
			return;
		}
	}
	if (m_bTlm)
	{
		tlm = _wtof(m_strTlm);
		if (tlm < 0.0001 || tlm > 1200000.0)
		{
			m_edTlm.SetSel(0, -1);
			m_edTlm.SetFocus();
			MessageBox(_T("TLMサブキャリアは0.0001から1200000.0000を入力してください。"), 0, MB_OK | MB_ICONERROR);
			return;
		}
	}
	if (m_bCmd)
	{
		cmd = _wtof(m_strCmd);
		if (cmd < 100.0 || cmd > 1200000.0)
		{
			m_edCmd.SetSel(0, -1);
			m_edCmd.SetFocus();
			MessageBox(_T("CMDサブキャリアは100.0000から1200000.0000を入力してください。"), 0, MB_OK | MB_ICONERROR);
			return;
		}
	}

	// 入力値OK
	if (mOpeProp.init.autocalc == 0)	// 予想受信レベル固定時
	{
		mOpeProp.init.reclevels = reclevels;
		mOpeProp.init.reclevelx = reclevelx;
	}
	if (m_bTlm)
	{
		// TLMサブキャリアの設定
		mOpeProp.init.tlm = tlm;
	}
	if (m_bCmd)
	{
		// CMDサブキャリアの設定
		mOpeProp.init.cmd = cmd;
	}
	
	// プロパティデータの書き込み
	_WriteOpeProp(true);
	// シェルコマンドを実行

	// シェルコマンドを実行
	const CString mRegistProp = _T("REGIST_OPEPROPERTY.sh"); // 運用プロパティ登録要求

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = mRegistProp;
	CString arg;

	arg.Format(_T("%s %s"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), _T(""));
	stParam.strRemShellParam.Format(arg);

	// リモシェ実行
	int err = CSshShell::ExecShell(stParam, TRUE);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell"), __FILEW__, __LINE__);
		CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("OnBnClickedButtonPropset"), _T("debug"), strDebug, nLogEx::debug);
	}
}


void CTimeLineView::OnBnClickedCheckTlm()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_TLM)->EnableWindow(m_bTlm);
	m_edTlm.SetSel(0, -1);
	m_edTlm.SetFocus();
	m_bSubCarTlmModified = false;

//	if (m_bTlm == FALSE){
//		GetDlgItem(IDC_EDIT_TLM)->GetWindowTextW(m_strTlmUser);
//		// TLMサブキャリアの設定
//		m_strTlm.Format(_T("%7.4lf"), mOpeProp.init.tlm);
//	}
//	else{
//		if (m_strTlmUser.IsEmpty() == false)
//			m_strTlm = m_strTlmUser;
//	}
//	GetDlgItem(IDC_EDIT_TLM)->SetWindowTextW(m_strTlm);
//	UpdateData(FALSE);
}


void CTimeLineView::OnBnClickedCheckCmd()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_CMD)->EnableWindow(m_bCmd);
	m_edCmd.SetSel(0, -1);
	m_edCmd.SetFocus();
	m_bSubCarCmdModified = false;

//	if (m_bCmd == FALSE){
//		GetDlgItem(IDC_EDIT_CMD)->GetWindowTextW(m_strCmdUser);
//		// CMDサブキャリアの設定
//		m_strCmd.Format(_T("%7.4lf"), mOpeProp.init.cmd);
//	}
//	else{
//		if (m_strCmdUser.IsEmpty() == false)
//			m_strCmd = m_strCmdUser;
//	}
//	GetDlgItem(IDC_EDIT_CMD)->SetWindowTextW(m_strCmd);
//	UpdateData(FALSE);
}

/*============================================================================*/
/*! タイムライン

-# 計画ファイルのAUTO OFF処理

@param	cbuf	計画ファイルの文字列
@retval TRUE:AUTOをOFFに置き換えた / FALSE:AUTOを置き換えてない
*/
/*============================================================================*/
BOOL CTimeLineView::UpdatePlanDataAutoOff(CString& cbuf)
{
	// 「AUTO=」を探す
	const CString strSearchString = _T("AUTO=");
	if (cbuf.Find(strSearchString) >= 0){
		// 「AUTO=」がみつかったのでOFFにするため、「AUTO=」より右側を消す
		CString str[20];
		int count = 0;
		int pos = 0;
		int i = 0;
		while (AfxExtractSubString(str[i], cbuf, i, ' ')){
			str[i].TrimLeft();
			str[i].TrimRight();
			if (str[i].Find(strSearchString) >= 0){
				pos = i;
			}
			i++;
		}
		count = i;
		for (i = 0; i < count; i++){
			if (i == 0){
				cbuf = str[i];
			}
			else{
				cbuf += _T(" ");
				if (i == pos){
					cbuf += strSearchString;
				}
				else{
					cbuf += str[i];
				}
			}
		}
		cbuf += _T("\n");
		return TRUE;
	}

	return FALSE;
}

/*============================================================================*/
/*! タイムライン

-# 計画ファイルの更新

@brief 現在対象の計画ファイルの更新を行う

@param	type	更新タイプ（ePlanMasterSlave|ePlanPStart）
@param	data	計画データ（必要のものしか更新しない）
				ePlanMasterSlaveの場合、ヘッダーのみ
				ePlanPStartの場合、運用開始時刻のみ
@retval なし
*/
/*============================================================================*/
void CTimeLineView::UpdatePlanData(int type, stPlandata data)
{
	CGraphMutex::Lock(ePassInfo);
	UpdateData(TRUE);

	// 現在選択されている計画データを取得する
	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];

	// 現在選択されている計画ファイルを一気読み
	FILE *file;
	if (_wfopen_s(&file, tdata.m_strPlanFilePath, _T("r")) != 0){
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("UpdatePlanData"), _T("Plan File Open(R) Error"), tdata.m_strPlanFilePath, nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		mGetSharePlanedFileTime = 0;
		CGraphMutex::Unlock(ePassInfo);
		return;
	}

	vector<CString>	plandata;
	vector<CString>::iterator itr;
	CString cbuf;
	CStringA abuf;

	TRY
	{
		char sz_line[512];
		while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), file) != NULL){
			cbuf = CServerFile::ConvertUnicode(sz_line);
			plandata.push_back(cbuf);
		}
	}CATCH_ALL(e){
	}
	END_CATCH_ALL

	fclose(file);

	CString strFtpFolder = tdata.m_strFtpPlanFilePath.Left(tdata.m_strFtpPlanFilePath.ReverseFind(_T('\\'))).MakeLower();
	if (PathFileExists(strFtpFolder) == FALSE)
	{
		CreateDirectory(strFtpFolder, NULL);
	}

	FILE *fileeuc;
	if (_wfopen_s(&fileeuc, tdata.m_strFtpPlanFilePath, _T("wb")) != 0){
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("UpdatePlanData"), _T("Plan File Open(W) Error"), tdata.m_strPlanFilePath, nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		CGraphMutex::Unlock(ePassInfo);
		return;
	}

	// 更新種別によって処理を分ける
	if (type == ePlanMasterSlave){
		// 主局従局の切り替え
		for (itr = plandata.begin(); itr != plandata.end(); itr++){
			cbuf = (*itr);

			// AUTO OFF
			if (UpdatePlanDataAutoOff(cbuf))
			{
			}
			else
			{
				const CString strSearchString = _T("PRI=");
				// 「PRI=」を探す
				if (cbuf.Find(strSearchString) >= 0){
					// 「PRI=」がみつかったので主局従局を設定する
					CString str[20];
					int count = 0;
					int pos = 0;
					int i = 0;
					while (AfxExtractSubString(str[i], cbuf, i, ' ')){
						str[i].TrimLeft();
						str[i].TrimRight();
						if (str[i].Find(strSearchString) >= 0){
							pos = i;
						}
						i++;
					}
					count = i;
					for (i = 0; i < count; i++){
						if (i == 0){
							cbuf = str[i];
						}
						else{
							cbuf += _T(" ");
							if (i == pos){
								cbuf += strSearchString;
								cbuf += data.sz_priority;
							}
							else{
								cbuf += str[i];
							}
						}
					}
					cbuf += _T("\n");
				}
			}
			abuf = CServerFile::ConvertEuc((LPTSTR)(LPCTSTR)cbuf);
			fputs(abuf, fileeuc);
		}
	}
	else if (type == ePlanPStart){
		// アップリンク、CMD、RNG情報
		GetSatelliteXTX();
		CString strXtx = (mXtx == XTX_Y_ID) ? _T("X") : _T("S");
		bool bFindPStart = false;
		bool bWriteUplink = false;
		bool bWriteCmd = false;
		bool bWriteRng = false;
		for (itr = plandata.begin(); itr != plandata.end(); itr++){
			cbuf = (*itr);

			// AUTO OFF
			if (UpdatePlanDataAutoOff(cbuf))
			{
			}
			else
			{
				// 「OPE_MODE」を探す
				if (cbuf.Find(_T("OPE_MODE")) >= 0 && cbuf.Left(1) == _T("#")){
					// 「OPE_MODE」がみつかったのでここでアップリンク、CMD、RNG情報を設定する
					// 先ずはスペース区切りで各情報を取得する
					CString buf[10];
					int count = 0;
					while (AfxExtractSubString(buf[count], cbuf, count, ' ')){
						count++;
					}
					cbuf.Empty();
					int oppos = -1;
					for (int i = 0; i < count; i++){
						CString str;
						if (buf[i].Find(_T("OPE_MODE")) >= 0){
							oppos = i;
							str = _T("OPE_MODE=");
							// TLM設定
							bool comma = false;
							if (buf[i].Find(_T("S-TLM")) >= 0){
								str += _T("S-TLM");
								comma = true;
							}
							if (buf[i].Find(_T("X-TLM")) >= 0){
								if (comma == true)
									str += _T(",");
								str += _T("X-TLM");
								comma = true;
							}
							if (buf[i].Find(_T("Ka-TLM")) >= 0){
								if (comma == true)
									str += _T(",");
								str += _T("Ka-TLM");
								comma = true;
							}
							// CMD設定
							if (tdata.m_CmdChk == TRUE){
								if (comma == true)
									str += _T(",");
								if (mXtx == XTX_Y_ID){
									str += _T("X-CMD");
								}
								else{
									str += _T("S-CMD");
								}
								comma = true;
							}
							// RNG設定
							if (tdata.m_RngChk == TRUE){
								if (tdata.m_RngDayList.size() > 0){
									if (comma == true)
										str += _T(",");
									CString temp;
									temp.Format(_T("%s"), (LPCTSTR)ExchageRngTypePlanFile(tdata.m_RngTypeList[0]));
									str += temp;
								}
								comma = true;
							}
							if (oppos == (count - 1))
								str += _T("\n");
						}
						else{
							str = buf[i];
						}
						if (cbuf.IsEmpty() == false)
							cbuf += _T(" ");
						cbuf += str;
					}
				}
				// 「P-START」を探す
				if (bFindPStart == false){
					if (cbuf.Find(_T("P-START")) >= 0 && cbuf.Left(1) != _T("#")){
						// 「P-START」がみつかったのでここからアップリンク、CMD、RNG情報を設定する
						bFindPStart = true;
					}
				}
				else{
					if (cbuf == _T("\n") || cbuf.Find(_T("SYSTEM.RmtmixSeqStart")) >= 0){
						// 「運用開始時刻」グループの最後？まできたので、情報が書かれていない場合はここで一気に書き込む
						CString cbufback = cbuf;
						if (tdata.m_UpLinkChk == TRUE && bWriteUplink == false){
							for (int i = 0; i < tdata.m_ULDay.size(); i++){
								cbuf.Format(_T("%s CTRL.UP_START,,%s;\n"), (LPCTSTR)(tdata.m_ULDay[i].start).Format(_T("%Y-%m-%d %H:%M:%S")), (LPCTSTR)m_strSpaceName);
								abuf = CServerFile::ConvertEuc((LPTSTR)(LPCTSTR)cbuf);
								fputs(abuf, fileeuc);
								cbuf.Format(_T("%s CTRL.UP_END,,%s;\n"), (LPCTSTR)(tdata.m_ULDay[i].end).Format(_T("%Y-%m-%d %H:%M:%S")), (LPCTSTR)m_strSpaceName);
								abuf = CServerFile::ConvertEuc((LPTSTR)(LPCTSTR)cbuf);
								fputs(abuf, fileeuc);
							}
							bWriteUplink = true;
						}
						if (tdata.m_CmdChk == TRUE && bWriteCmd == false){
							for (int i = 0; i < tdata.m_CmdDayList.size(); i++){
								cbuf.Format(_T("%s CTRL.CMD_START,%s,%s;\n"), (LPCTSTR)(tdata.m_CmdDayList[i].start).Format(_T("%Y-%m-%d %H:%M:%S")), (LPCTSTR)strXtx, (LPCTSTR)m_strSpaceName);
								abuf = CServerFile::ConvertEuc((LPTSTR)(LPCTSTR)cbuf);
								fputs(abuf, fileeuc);
								cbuf.Format(_T("%s CTRL.CMD_END,%s,%s;\n"), (LPCTSTR)(tdata.m_CmdDayList[i].end).Format(_T("%Y-%m-%d %H:%M:%S")), (LPCTSTR)strXtx, (LPCTSTR)m_strSpaceName);
								abuf = CServerFile::ConvertEuc((LPTSTR)(LPCTSTR)cbuf);
								fputs(abuf, fileeuc);
							}
							bWriteCmd = true;
						}
						if (tdata.m_RngChk == TRUE && bWriteRng == false){
							for (int i = 0; i < tdata.m_RngDayList.size(); i++){
								cbuf.Format(_T("%s CTRL.RNG_START,%s,%s;\n"), (LPCTSTR)(tdata.m_RngDayList[i].start).Format(_T("%Y-%m-%d %H:%M:%S")), (LPCTSTR)ExchageRngTypePlanFile(tdata.m_RngTypeList[i]), (LPCTSTR)m_strSpaceName);
								abuf = CServerFile::ConvertEuc((LPTSTR)(LPCTSTR)cbuf);
								fputs(abuf, fileeuc);
								cbuf.Format(_T("%s CTRL.RNG_END,%s,%s;\n"), (LPCTSTR)(tdata.m_RngDayList[i].end).Format(_T("%Y-%m-%d %H:%M:%S")), (LPCTSTR)ExchageRngTypePlanFile(tdata.m_RngTypeList[i]), (LPCTSTR)m_strSpaceName);
								abuf = CServerFile::ConvertEuc((LPTSTR)(LPCTSTR)cbuf);
								fputs(abuf, fileeuc);
							}
							bWriteRng = true;
						}
						cbuf = cbufback;
					}

					// ここでアップリンク、CMD、RNG情報を書き込む
					if (cbuf.Find(_T("CTRL.DOWN_")) >= 0 || cbuf.Find(_T("CTRL.TLM_")) >= 0){
						// 何もしないで、そのまま書く
					}
					else{
						if (cbuf.Find(_T("CTRL.UP_")) >= 0){
							if (tdata.m_UpLinkChk == FALSE || bWriteUplink == true)
								continue;
							for (int i = 0; i < tdata.m_ULDay.size(); i++){
								cbuf.Format(_T("%s CTRL.UP_START,,%s;\n"), (LPCTSTR)(tdata.m_ULDay[i].start).Format(_T("%Y-%m-%d %H:%M:%S")), (LPCTSTR)m_strSpaceName);
								abuf = CServerFile::ConvertEuc((LPTSTR)(LPCTSTR)cbuf);
								fputs(abuf, fileeuc);
								cbuf.Format(_T("%s CTRL.UP_END,,%s;\n"), (LPCTSTR)(tdata.m_ULDay[i].end).Format(_T("%Y-%m-%d %H:%M:%S")), (LPCTSTR)m_strSpaceName);
								abuf = CServerFile::ConvertEuc((LPTSTR)(LPCTSTR)cbuf);
								fputs(abuf, fileeuc);
							}
							bWriteUplink = true;
							continue;
						}
						if (cbuf.Find(_T("CTRL.CMD_")) >= 0){
							if (tdata.m_CmdChk == FALSE || bWriteCmd == true)
								continue;
							for (int i = 0; i < tdata.m_CmdDayList.size(); i++){
								cbuf.Format(_T("%s CTRL.CMD_START,%s,%s;\n"), (LPCTSTR)(tdata.m_CmdDayList[i].start).Format(_T("%Y-%m-%d %H:%M:%S")), (LPCTSTR)strXtx, (LPCTSTR)m_strSpaceName);
								abuf = CServerFile::ConvertEuc((LPTSTR)(LPCTSTR)cbuf);
								fputs(abuf, fileeuc);
								cbuf.Format(_T("%s CTRL.CMD_END,%s,%s;\n"), (LPCTSTR)(tdata.m_CmdDayList[i].end).Format(_T("%Y-%m-%d %H:%M:%S")), (LPCTSTR)strXtx, (LPCTSTR)m_strSpaceName);
								abuf = CServerFile::ConvertEuc((LPTSTR)(LPCTSTR)cbuf);
								fputs(abuf, fileeuc);
							}
							bWriteCmd = true;
							continue;
						}
						if (cbuf.Find(_T("CTRL.RNG_")) >= 0){
							if (tdata.m_RngChk == FALSE || bWriteRng == true)
								continue;
							for (int i = 0; i < tdata.m_RngDayList.size(); i++){
								cbuf.Format(_T("%s CTRL.RNG_START,%s,%s;\n"), (LPCTSTR)(tdata.m_RngDayList[i].start).Format(_T("%Y-%m-%d %H:%M:%S")), (LPCTSTR)ExchageRngTypePlanFile(tdata.m_RngTypeList[i]), (LPCTSTR)m_strSpaceName);
								abuf = CServerFile::ConvertEuc((LPTSTR)(LPCTSTR)cbuf);
								fputs(abuf, fileeuc);
								cbuf.Format(_T("%s CTRL.RNG_END,%s,%s;\n"), (LPCTSTR)(tdata.m_RngDayList[i].end).Format(_T("%Y-%m-%d %H:%M:%S")), (LPCTSTR)ExchageRngTypePlanFile(tdata.m_RngTypeList[i]), (LPCTSTR)m_strSpaceName);
								abuf = CServerFile::ConvertEuc((LPTSTR)(LPCTSTR)cbuf);
								fputs(abuf, fileeuc);
							}
							bWriteRng = true;
							continue;
						}
					}
				}
			}
			abuf = CServerFile::ConvertEuc((LPTSTR)(LPCTSTR)cbuf);
			fputs(abuf, fileeuc);
		}
	}
	else if (type == ePlanPEnd){
		// P-ENDの書き換え
		for (itr = plandata.begin(); itr != plandata.end(); itr++){
			cbuf = (*itr);

			// AUTO OFF
			if (UpdatePlanDataAutoOff(cbuf))
			{
			}
			else
			{
				// ヘッダー行の『P-END』を探す
				if ((cbuf.Find(_T("# ")) >= 0) && (cbuf.Find(_T("P-END=")) >= 0)){
					CString header = _T("");
					CString item = _T("");
					CString str;
					int i = 1;
					while (AfxExtractSubString(str, cbuf, i++, ' ')){
						str.TrimLeft();
						str.TrimRight();
						if (str.Find(_T("P-START")) >= 0)
						{
							header += _T(" ") + str;
							AfxExtractSubString(str, cbuf, i++, ' ');	// HH:MM::SS
							header += _T(" ") + str;
						}
						else if (str.Find(_T("P-END")) >= 0)
						{
							item.Format(_T("P-END=%s"), (LPCTSTR)m_strOpeStop);
							header += _T(" ") + item;
							i++;	// HH:MM::SS
						}
						else
						{
							header += _T(" ") + str;
						}
					}
					cbuf.Format(_T("#%s\n"), (LPCTSTR)header);
				}
				else if (cbuf.Find(_T("P-END")) >= 0){
					// 本文の『P-END』を探す
					cbuf.Format(_T("%s P-END,,%s;\n"), (LPCTSTR)m_strOpeStop, (LPCTSTR)m_strSpaceName);
				}
			}
			abuf = CServerFile::ConvertEuc((LPTSTR)(LPCTSTR)cbuf);
			fputs(abuf, fileeuc);
		}
	}
	else{
		fclose(fileeuc);
		CGraphMutex::Unlock(ePassInfo);
		return;
	}
	fclose(fileeuc);

	// 新しい計画ファイル読み込みが必要なため、クリアしておく
	bool bTmpReadCompletePlanFile = m_bReadCompletePlanFile;
	m_bReadCompletePlanFile = false;

	// 計画登録要求
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = REGIST_PLAN_SH;
	CString arg;
	/* 更新 */
	// 計画更新要求の送信
	//if (tdata.mGetForm == true)
	//{
	//	arg.Format(_T("%s renew %s enp %s.pln"), (LPCTSTR)CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower(), (LPCTSTR)m_strSpaceName.MakeLower(), (LPCTSTR)tdata.m_strPlanID);
	//}
	//else
	{
		CString temp = m_strSpaceName;
		arg.Format(_T("%s renew %s sttn %s.pln"), (LPCTSTR)CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower(), (LPCTSTR)temp.MakeLower(), (LPCTSTR)tdata.m_strPlanID);
	}

	stParam.strRemShellParam.Format(arg);

	int err = CSshShell::ExecShell(stParam);
	if (err != 0)
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("UpdatePlanData"), _T(""), strDebug, nLogEx::error);

		m_bReadCompletePlanFile = bTmpReadCompletePlanFile;	// シェル失敗時に読み込みフラグを元に戻す
	}


	CGraphMutex::Unlock(ePassInfo);
}
/*============================================================================*/
/*! タイムライン

-# 計画ファイルの更新

@brief 運用モードの取得

@param	data	計画データ
@retval 運用モード
*/
/*============================================================================*/
CString CTimeLineView::GetOpeMode(stPlandata data)
{
	CString mode = _T("");

	// 現在選択されている計画データを取得する
	COprPlnData& tdata = mGraph.mListTimeData[mSelectOpePlanData];

	// TLM S
	if (tdata.m_TlmSChk){
		if (tdata.m_TlmSDayList.size() > 0){
			if (mode.IsEmpty() == false)
				mode += _T(",");
			mode += _T("S-TLM");
		}
	}

	// TLM X
	if (tdata.m_TlmXChk){
		if (tdata.m_TlmXDayList.size() > 0){
			if (mode.IsEmpty() == false)
				mode += _T(",");
			mode += _T("X-TLM");
		}
	}

	// TLM Ka
	if (tdata.m_TlmKaChk){
		if (tdata.m_TlmKaDayList.size() > 0){
			if (mode.IsEmpty() == false)
				mode += _T(",");
			mode += _T("Ka-TLM");
		}
	}

//	int xtx = GetSatelliteXTX();

	// CMD
	if (tdata.m_CmdChk){
		if (tdata.m_CmdDayList.size() > 0){
			if (mode.IsEmpty() == false)
				mode += _T(",");
			if (mXtx == XTX_Y_ID){
				mode += _T("X-CMD");
			} else{
				mode += _T("S-CMD");
			}
		}
	}

	// RNG
	if (tdata.m_RngChk){
		int count = (int)tdata.m_RngDayList.size();
		for (int i = 0; i < count; ++i){
			//CString strRngType = m_RngList.GetItemText(i, 2);
			//GetOpeModeAddString(mode, ExchageRngTypePlanFile(strRngType));
		}
	}

	return mode;
}
/*============================================================================*/
/*! CTimeLineView

-# 衛星固有DBより取得したデータより該当する衛星のインデックスを取得する

@brief このインデックスはリスト上のもの。

@param	str :衛星名
@retval	インデックス
*/
/*============================================================================*/
int CTimeLineView::GetSatelliteXTX()
{
	if (m_strSpaceName.Left(1) == _T("-") || m_strSpaceName.IsEmpty()){
		return XTX_N_ID;
	}
	map<CString, __int64> &satAdrList = mDBAccess.GetSatAdrList();
	struct satinf_t *satinf_adr;
	struct satinf_t *original_adr;
	struct xtx_t	*xtx_t_adr;		/* Ｘ帯送信設備使用有無情報テーブルアドレス */
	struct rxfqval_t	*rxfqval_adr;
	char *sattbl_adr;
	satinf_adr = (struct satinf_t *)((__int64*)satAdrList[m_strSpaceName]);
	if (satinf_adr == NULL)
		return XTX_N_ID;
	original_adr = satinf_adr;
	sattbl_adr = (char *)satinf_adr;

	/*--------------------------------------*/
	/* Ｘ帯送信設備使用有無情報取得 */
	/*--------------------------------------*/
	mXtx = XTX_N_ID;
	mRxfxX = 0;
	mRxfxKa = 0;

	while (1){
		satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
		if (satinf_adr->l_id == ENDOFSAT_ID){
			/*識別ＩＤが終了の場合*/
			break;
		}
		if (satinf_adr->l_id == XTX_ID)	{
			/*識別ＩＤが「Ｘ帯送信設備使用有無情報」の場合*/
			xtx_t_adr = (struct xtx_t *)satinf_adr;
			mXtx = xtx_t_adr->l_x_tx;
//			break;
		}
		if (satinf_adr->l_id == RXFQVAL_ID)
		{
			/*識別ＩＤが『ノミナル受信周波数情報』の場合*/
			rxfqval_adr = (struct rxfqval_t *)satinf_adr;
			if (rxfqval_adr->l_band == RXBAND_X_ID)
			{
				if (rxfqval_adr->d_fq != 0)
				{
					// X 帯有り
					mRxfxX = 1;
				}
			}
			if (rxfqval_adr->l_band == RXBAND_KA_ID)
			{
				if (rxfqval_adr->d_fq != 0)
				{
					// Ka 帯有り
					mRxfxKa = 1;
				}
			}
		}
		sattbl_adr += satinf_adr->l_len;
	}

	// RNG 種別取得
	m_RngEleList.clear();
	int station = theApp.GetSelectStation();
	if (station == 0)	// 臼田64m
	{
		if (mXtx == XTX_Y_ID)
		{
			m_RngEleList.push_back(_T("XX新"));
			m_RngEleList.push_back(_T("XX新再"));
		}
		else
		{
			m_RngEleList.push_back(_T("SS"));
			if (mRxfxX == 1)
			{
				m_RngEleList.push_back(_T("SX"));
			}
		}
	}
	else if (station == 1)	// 臼田54m
	{
		if (mXtx == XTX_Y_ID)
		{
			m_RngEleList.push_back(_T("XX"));
			m_RngEleList.push_back(_T("XX再"));
			m_RngEleList.push_back(_T("XKa"));
			m_RngEleList.push_back(_T("XKa再"));
		}
		else
		{
			m_RngEleList.push_back(_T("SS"));
			if (mRxfxX == 1)
			{
				m_RngEleList.push_back(_T("SX"));
			}
		}
	}
	else if (station == 2)	// 内之浦34m
	{
		if (mXtx == XTX_Y_ID)
		{
			m_RngEleList.push_back(_T("XX新"));
			m_RngEleList.push_back(_T("XX新再"));
		}
		else
		{
			m_RngEleList.push_back(_T("SS"));
			if (mRxfxX == 1)
			{
				m_RngEleList.push_back(_T("SX"));
			}
		}
	}
	else if (station == 3)	// 内之浦20m
	{
		m_RngEleList.push_back(_T("SS"));
		if (mRxfxX == 1)
		{
			m_RngEleList.push_back(_T("SX"));
		}
	}

	return 1;
}
/*============================================================================*/
/*! CTimeLineView

-# RNG種別取得

@param  なし
@retval 計画ファイル出力用のRNG種別文字列
*/
/*============================================================================*/
CString	CTimeLineView::ExchageRngTypePlanFile(CString str)
{
	CString type = _T("");

	if (theApp.GetSelectStation() == eStation_Usuda642)
	{
		// 54m局
		if (str == _T("XX"))
		{
			type = _T("XX-RNG_NOR");
		}
		else if (str == _T("XX再"))
		{
			type = _T("XX-RNG_REG");
		}
		else if (str == _T("XKa"))
		{
			type = _T("XKa-RNG_NOR");
		}
		else if (str == _T("XKa再"))
		{
			type = _T("XKa-RNG_REG");
		}
	}
	else
	{
		// それ以外の局
		if (str == _T("SS"))
		{
			type = _T("SS-RNG");
		}
		else if (str == _T("SX"))
		{
			type = _T("SX-RNG");
		}
		else if (str == _T("XX新"))
		{
			type = _T("XX-RNG_NOR");
		}
		else if (str == _T("XX新再"))
		{
			type = _T("XX-RNG_REG");
		}
		else if (str == _T("XKa新"))
		{
			type = _T("XKa-RNG_NOR");
		}
		else if (str == _T("XKa新再"))
		{
			type = _T("XKa-RNG_REG");
		}
	}

	return type;
}

CString	CTimeLineView::ExchagePlanFileRngType(CString str)
{
	CString type = _T("");
	if (theApp.GetSelectStation() == eStation_Usuda642)
	{
		// 54m局
		if (str == _T("XX-RNG_NOR"))
		{
			type = _T("XX");
		}
		else if (str == _T("XX-RNG_REG"))
		{
			type = _T("XX再");
		}
		else if (str == _T("XKa-RNG_NOR"))
		{
			type = _T("XKa");
		}
		else if (str == _T("XKa-RNG_REG"))
		{
			type = _T("XKa再");
		}
	}
	else
	{
		// それ以外の局
		if (str == _T("SS-RNG"))
		{
			type = _T("SS");
		}
		else if (str == _T("SX-RNG"))
		{
			type = _T("SX");
		}
		else if (str == _T("XX-RNG_NOR"))
		{
			type = _T("XX新");
		}
		else if (str == _T("XX-RNG_REG"))
		{
			type = _T("XX新再");
		}
		else if (str == _T("XKa-RNG_NOR"))
		{
			type = _T("XKa新");
		}
		else if (str == _T("XKa-RNG_REG"))
		{
			type = _T("XKa新再");
		}
	}

	return type;
}



LRESULT CTimeLineView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == eMessage_UpdateForecastFilePath){
		CString predfile = CString((TCHAR*)lParam);
		predfile = predfile.Right(predfile.GetLength() - predfile.ReverseFind(_T('/')) - 1);
		int idx = predfile.Find(_T("."));
		predfile = predfile.Mid(0, idx);
		CString strPassId;
		strPassId.Format(_T("%s-%s"), (LPCTSTR)(predfile.Left(6)), (LPCTSTR)(predfile.Right(4)));

		CGraphMutex::Lock(ePassInfo);

		vector<stPredListData>::iterator itr;
		int pos = -1;
		int i = 0;
		for (itr = mGraph.mListPredData.begin(); itr != mGraph.mListPredData.end(); itr++, i++){
			if ((*itr).strPassId.CompareNoCase(strPassId) == 0){
				pos = i;
				break;
			}
		}
		if (pos < 0){
			CGraphMutex::Unlock(ePassInfo);
			return TRUE;
		}

		mSelectForecast = pos;

		CString satellite;
		satellite = mGraph.mListPredData[mSelectForecast].strSatellite;
		predfile = ConvertPredfileToFname(mGraph.mListPredData[mSelectForecast].strPredPath);
		/****************************************************************/
		/* 制御情報 */

		// 計画ID
		UpdateData();
		CString strKeikakuID = m_strPlanID;

		CString ctrl = _T("");
		ctrl.Format(_T("CTRL.PRED_CHANGE_ANT %s@%s@%s"), (LPCTSTR)satellite, (LPCTSTR)predfile, (LPCTSTR)strKeikakuID);
		theApp.SendControl(ctrl, _T(""), _T(""), _T(""));

		mSelectForecastName.Empty();
		mGetSharePredFileTime = 0;

		UpdateTimelineData(mSelectForecast);
//		UpdateInitializePropertyData();
//		UpdateOperationProperty();
		UpdateOperationOption();
		UpdateMonitorData();
		CGraphMutex::Unlock(ePassInfo);
		return TRUE;
	}
	return CFormView::WindowProc(message, wParam, lParam);
}


/*============================================================================*/
/*! CTimeLineView

-# 監視からCMDモード取得

@param  なし
@retval 1:自動 / 0:手動
*/
/*============================================================================*/
unsigned char CTimeLineView::GetKanshiCmdMode()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	char		ca_mode[30];		/* CMDモード */

	/* CMDモード取得 */
	obs_adr = (CEngValCnv::obsname_t *)theApp.GetObsname_t(theApp.GetSelectStation(), CString(CTRL_P_CAP_CMD));
	if (obs_adr == NULL)
	{
//#ifdef _DEBUG
//		if (m_nCmdEvent == 0)
//			return 1;
//		else
//			return 0;
//#endif
		return 1;
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		strcpy_s(ca_mode, 30, obs_adr->sz_sts_name);
		CString str(ca_mode);
		if (str.CompareNoCase(_T("自動")) == 0)
		{
			return 1;
		}
		else if (str.CompareNoCase(_T("手動")) == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 1;
	}
}


/*============================================================================*/
/*! CTimeLineView

-# 監視からRNGモード取得

@param  なし
@retval 1:自動 / 0:手動
*/
/*============================================================================*/
unsigned char CTimeLineView::GetKanshiRngMode()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	char		ca_mode[30];		/* CMDモード */

	/* RNGモード取得 */
	obs_adr = (CEngValCnv::obsname_t *)theApp.GetObsname_t(theApp.GetSelectStation(), CString(CTRL_P_CAP_RNG));
	if (obs_adr == NULL)
	{
//#ifdef _DEBUG
//		if (m_nRngEvent == 0)
//			return 1;
//		else
//			return 0;
//#endif
		return 1;
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		strcpy_s(ca_mode, 30, obs_adr->sz_sts_name);
		CString str(ca_mode);
		if (str.CompareNoCase(_T("自動")) == 0)
		{
			return 1;
		}
		else if (str.CompareNoCase(_T("手動")) == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 1;
	}
}


/*============================================================================*/
/*! CTimeLineView

-# 監視からダウンリンクモード取得

@param  なし
@retval 1:自動 / 0:手動
*/
/*============================================================================*/
unsigned char CTimeLineView::GetKanshiDownLinkMode()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	char		ca_mode[30];		/* CMDモード */

	/* ダウンリンクモード取得 */
	obs_adr = (CEngValCnv::obsname_t *)theApp.GetObsname_t(theApp.GetSelectStation(), CString(CTRL_P_CAP_RX));
	if (obs_adr == NULL)
	{
		return 1;
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		strcpy_s(ca_mode, 30, obs_adr->sz_sts_name);
		CString str(ca_mode);
		if (str.CompareNoCase(_T("自動")) == 0)
		{
			return 1;
		}
		else if (str.CompareNoCase(_T("手動")) == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 1;
	}
}


/*============================================================================*/
/*! CTimeLineView

-# 監視からアップリンクモード取得

@param  なし
@retval 1:自動 / 0:手動
*/
/*============================================================================*/
unsigned char CTimeLineView::GetKanshiUpLinkMode()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	char		ca_mode[30];		/* CMDモード */

	/* アップリンクモード取得 */
	obs_adr = (CEngValCnv::obsname_t *)theApp.GetObsname_t(theApp.GetSelectStation(), CString(CTRL_P_CAP_UPLINK));
	if (obs_adr == NULL)
	{
//#ifdef _DEBUG
//		if (m_nUplnkEvent == 0)
//			return 1;
//		else
//			return 0;
//#endif
		return 1;
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		strcpy_s(ca_mode, 30, obs_adr->sz_sts_name);
		CString str(ca_mode);
		if (str.CompareNoCase(_T("自動")) == 0)
		{
			return 1;
		}
		else if (str.CompareNoCase(_T("手動")) == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 1;
	}
}


/*============================================================================*/
/*! CTimeLineView

-# 監視から1WAY/2WAY/Dup取得

@param  なし
@retval 0:1WAY / 1:WAY / 2:Dup
*/
/*============================================================================*/
unsigned char CTimeLineView::GetKanshiWayMode()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	char		ca_mode[30];		/* CMDモード */

	/* WAYモード取得 */
	obs_adr = (CEngValCnv::obsname_t *)theApp.GetObsname_t(theApp.GetSelectStation(), CString(CTRL_P_CAP_WAY));
	if (obs_adr == NULL)
	{
		return 0;
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		strcpy_s(ca_mode, 30, obs_adr->sz_sts_name);
		CString str(ca_mode);
		if (str.CompareNoCase(_T("1WAY")) == 0)
		{
			return 0;
		}
		else if (str.CompareNoCase(_T("2WAY")) == 0)
		{
			return 1;
		}
		else if (str.CompareNoCase(_T("DUP")) == 0)
		{
			return 2;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}


/*============================================================================*/
/*! CTimeLineView

-# 衛星捕捉モードを通知する

@param  通知バイナリ
@retval なし
*/
/*============================================================================*/
void CTimeLineView::SendSatelliteCaptureModeNotify(unsigned char binary)
{
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = SEND_NOTICE_SH;
	CString arg;

	int nStation = theApp.GetSelectStation();
	arg.Format(_T("%s %d 0x%02x"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_ACQMOD, binary);

	stParam.strRemShellParam.Format(arg);

	int err = CSshShell::ExecShell(stParam);							// シェル待ち
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("SendSatelliteCaptureModeNotify"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err != 0) // 異常終了
	{
		// 何もしない
	}
}

/*============================================================================*/
/*! CTimeLineView

-# S帯予想受信レベル入力変更時に呼び出される

@param  なし
@retval なし
*/
/*============================================================================*/
void CTimeLineView::OnEnChangeEditReclevelvals()
{
	CString str;
	GetDlgItem(IDC_EDIT_RECLEVELVALS)->GetWindowTextW(str);
	if (str.Compare(m_strRecLevelS) != 0)
	{
		m_strRecLevelS = str;
		m_bRecLevelSModified = true;
	}
	else
	{
		m_bRecLevelSModified = false;
	}
}


/*============================================================================*/
/*! CTimeLineView

-# X帯予想受信レベル入力変更時に呼び出される

@param  なし
@retval なし
*/
/*============================================================================*/
void CTimeLineView::OnEnChangeEditReclevelvalx()
{
	CString str;
	GetDlgItem(IDC_EDIT_RECLEVELVALX)->GetWindowTextW(str);
	if (str.Compare(m_strRecLevelX) != 0)
	{
		m_strRecLevelX = str;
		m_bRecLevelXModified = true;
	}
	else
	{
		m_bRecLevelXModified = false;
	}
}


/*============================================================================*/
/*! CTimeLineView

-# サブキャリア周波数(TLM)入力変更時に呼び出される

@param  なし
@retval なし
*/
/*============================================================================*/
void CTimeLineView::OnEnChangeEditTlm()
{
	CString str;
	GetDlgItem(IDC_EDIT_TLM)->GetWindowTextW(str);
	if (str.Compare(m_strTlm) != 0)
	{
		m_strTlm = str;
//		m_strTlmUser = m_strTlm;
		m_bSubCarTlmModified = true;
	}
	else
	{
		m_bSubCarTlmModified = false;
	}
}


/*============================================================================*/
/*! CTimeLineView

-# サブキャリア周波数(CMD)入力変更時に呼び出される

@param  なし
@retval なし
*/
/*============================================================================*/
void CTimeLineView::OnEnChangeEditCmd()
{
	CString str;
	GetDlgItem(IDC_EDIT_CMD)->GetWindowTextW(str);
	if (str.Compare(m_strCmd) != 0)
	{
		m_strCmd = str;
//		m_strCmdUser = m_strCmd;
		m_bSubCarCmdModified = true;
	}
	else
	{
		m_bSubCarCmdModified = false;
	}
}

