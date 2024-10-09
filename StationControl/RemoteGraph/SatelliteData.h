#pragma once

#include "GraphWindow.h"

class CDBAccess
{
public:
	CDBAccess();
	virtual ~CDBAccess();

public:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	map<CString, struct obsname_t *>	m_adrtbl;	// DB�A�h���X���X�g

public:
	map<DWORD, CString>		m_monnamelist;			// �Ď������X�g

	map<CString, DWORD>		m_eqidlist;				// �ݔ������ʎq���X�g
	map<CString, DWORD>		m_dataidlist;			// �f�[�^�����ʎq���X�g

	map<CString, map<UINT, CString>>	m_stslist;	// �Ď����΃X�e�[�^�X�����񃊃X�g

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	int all_eqmon_db_read();		// �ݔ�����DB�擾

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
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	static const UINT mMaxDataCount = 1000;
	static const UINT mMaxDataTrace = 32;
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	deque<stMMPair>	mData[mMaxDataTrace];
	CWinThread	*m_pDataThread;
	bool		m_bThreadLoop;
	int			m_angle[mMaxDataTrace];	// �Ȃ񂿂���ăf�[�^�p
	float		m_fLogData;

	//-----------------------------------------------------------------------------
	//	�����o�֐�
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

