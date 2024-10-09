#pragma once

#include "GraphWindow.h"

class CDBAccess
{
public:
	CDBAccess();
	virtual ~CDBAccess();

public:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	map<CString, struct obsname_t *>	m_adrtbl;	// DBアドレスリスト

public:
	map<DWORD, CString>		m_monnamelist;			// 監視名リスト

	map<CString, DWORD>		m_eqidlist;				// 設備名識別子リスト
	map<CString, DWORD>		m_dataidlist;			// データ名識別子リスト

	map<CString, map<UINT, CString>>	m_stslist;	// 監視名対ステータス文字列リスト

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	int all_eqmon_db_read();		// 設備制御DB取得

	int db_read(char* db_file_nam, char *cp_loadadr_p);

	int ud_adr_to_montbl(UINT station, char* eqmondb_data);
	int ud_adr_to_adrtbl(char* base_p, long l_id, long l_limitid, long l_endoftblid, deque<struct obsname_t *>& adr_tbl);

	int	create_stslist(vector<stSelectData>& sel_list);
	int	ud_obsname_adr(CString name, char *shmadr, struct obsname_t *obs_adr[]);
	int	get_sts_list(CString name, char *shmadr, map<UINT, CString>& sts_list);

protected:
	long ds_htonl(long val)
	{
#if IS_BIG_ENDIA
		return htonl(val);
#else
		return val;
#endif
	}
};

class CSatelliteData
{
public:
	CSatelliteData();
	~CSatelliteData();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	static const UINT mMaxDataCount = 1000;
	static const UINT mMaxDataTrace = 32;
protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	deque<stMMPair>	mData[mMaxDataTrace];
	CWinThread	*m_pDataThread;
	bool		m_bThreadLoop;
	int			m_angle[mMaxDataTrace];	// なんちゃってデータ用
	float		m_fLogData;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	void	GetDummyData(int pos, float& data);
	deque<stMMPair>&	GetData(int trace)	{ return mData[trace]; }
	void	ClearData()
	{
		for (int i = 0; i < mMaxDataTrace; i++){
			mData[i].clear();
		}
	}
	bool	GetThreadLoop()			{ return m_bThreadLoop; }
	void	SetThreadLoop(bool val)	{ m_bThreadLoop = val; }
	void	CollectionStart();
	void	CollectionStop();
	void	OpenFile(CString strFileName);
protected:
};

