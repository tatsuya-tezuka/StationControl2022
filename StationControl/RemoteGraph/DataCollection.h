#pragma once

#include "DataCollectionThread.h"
#include "MemMapList.h"
#include "TrendPipe.h"
#include "../EngValConv/EngValCnv.h"

#include "MessageThread.h"

using namespace std;

//#define _PIPE_COLLECTION

// CDataCollection コマンド ターゲット

class CDataCollection
{
public:
	CDataCollection();
	virtual ~CDataCollection();

protected:
	static CEngValCnv*	m_pEngval[eStation_MAX];
public:
	static CEngValCnv*	GetEngval(int pos);
	static void			SetEngval(int pos, CEngValCnv* eval);
	static void			DeleteEngval(int pos);

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	map<DWORD, CSpaceData>	mSpaceRecv;				// 監視データ(受信)
	map<CString, stQueData>	mSpaceRead;				// 監視データ(読み込み)

	// データの種類：データ登録されているデータを格納(取得非対象のデータは無効データとして保存)
	// データの順番：データ登録の順番に格納(取得されていないデータは詰められる)
	deque<CSpaceData>		mSpaceTemp;				// 監視データ(テンポラリ)

	// データの種類：データ登録されているデータを格納(取得非対象のデータは無効データとして保存)
	// データの順番：データの取得状況によってバラバラに格納
	deque<CSpaceData>		mSpaceData;				// 監視データ

	vector<stSelectData>	mSelectDataList;		// 選択データリスト
	map<CString, DWORD>		mSelName2Id;			// 名前⇒ID検出リスト
	map<DWORD, CString>		mSelId2Name;			// ID⇒名前検出リスト
	map<DWORD, int>			mSelId2Index;			// ID⇒インデックス検出リスト

	CDataCollectionThread*	mpCollectThread;		// データ収集スレッド
	CSaveRealTimeDataThread*	mpSaveRealTimeDataThread;		// リアルタイムデータ保存スレッド
	CSaveAnalysisDataThread*	mpSaveAnalysisDataThread[MAX_VIEW];		// ノンリアルタイムデータ保存スレッド

	CDBAccess	mDBAccess;	// DBアクセスクラス

	// データの種類：データ登録されているデータを格納
	// データの順番：データ登録の順番に格納
	deque<stSpaceData>	m_CompData[MAX_VIEW * MAX_TDATA][NUM_REAL_SCALE_INFO_TABLE];		// 圧縮データ
	vector<stSpaceData>	m_CompDataForCopy[MAX_VIEW * MAX_TDATA][NUM_REAL_SCALE_INFO_TABLE];	// コピー用圧縮データ

	stDrawInfo			m_TrendDraw[MAX_VIEW * MAX_TDATA][NUM_REAL_SCALE_INFO_TABLE];		// トレンド描画情報(AUTOスケール以外)

	bool	m_bAutoClear;

	// データの種類：データ登録されているデータを格納
	// データの順番：データ登録の順番に格納
	CMemMapList			m_SaveRealTimeData[MAX_VIEW * MAX_TDATA];

	// データの種類：データ登録されているデータを格納
	// データの順番：データ登録の順番に格納
	deque<vector<stSpaceData>>	m_SaveRealTimeDataQue;

	// データの種類：読み込まれているデータを格納
	// データの順番：読み込んだ順番に格納
	deque<stQueData>	m_SaveAnalysisDataQue[MAX_VIEW];

	// データの種類：ビュー毎にプロパティで選択されているデータを保存
	// データの順番：ビュー毎にプロパティで選択の順番に保存
	CMemMapList			m_SaveAnalysisData[MAX_VIEW][MAX_TDATA];

	deque<stLimitState>	m_LimitState[MAX_VIEW][MAX_TDATA];

	// ダミー用の参照
	deque<stSpaceData>	m_DummySpaceDataDeque;
	vector<stSpaceData>	m_DummySpaceDataVector;
	stDrawInfo	m_DummyDrawInfo;

	// ■プロセス間通信
	CTrendPipe		mPipeSend[eStation_MAX];
	CTrendPipe		mPipeRecv[eStation_MAX];
	bool			m_bCreatePipe[eStation_MAX];
	DWORD			m_ThreadNotify;
	// 監視データ（受信監視データ）
	map<UINT, map<CString, CEngValCnv::obsname_t>> m_mondatalist;

	CMessageThread*		m_pMessageThread;
	CMessageThread*		m_pStationMessageThread[eStation_MAX];

public:
	static void WINAPI receiveServerCallBack01(PIPEMONITORACK* pbuf);
	static void WINAPI receiveServerCallBack02(PIPEMONITORACK* pbuf);
	static void WINAPI receiveServerCallBack03(PIPEMONITORACK* pbuf);
	static void WINAPI receiveServerCallBack04(PIPEMONITORACK* pbuf);
	static void WINAPI receiveServerCallBack(PIPEMONITORACK* pbuf);
	void	createPipe();
	void	deletePipe();
	void	SendPipe(DWORD command, int nStation);
	void	SetEqMonData(UINT nStation, vector<CEngValCnv::obs_tbl_res_t>& data);
	char*	GetObsname_t(UINT nStation, CString strWatch);


	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	deque<CSpaceData>&		GetSpaceData()		{ return mSpaceData; }
	deque<CSpaceData>&		GetSpaceTemp()		{ return mSpaceTemp; }
	deque<stSpaceData>&		GetSameSpaceData(DWORD id);
	vector<stSpaceData>&	GetSameSpaceDataForCopy(DWORD id);
	deque<stSpaceData>&		GetCompData(DWORD id, int tbl);
	vector<stSpaceData>&	GetCompDataForCopy(DWORD id, int tbl);
	stDrawInfo&				GetDrawInfo(DWORD id, int tbl);

	void	SetAutoClear()	{ m_bAutoClear = true; }

	map<DWORD, CString>&	GetMonNameList()	{ return mDBAccess.m_monnamelist; }
	int	CreateStatusList()	{ return mDBAccess.create_stslist(mSelectDataList); }
	bool GetStatusList(CString name, map<UINT, CString>&list)
	{
		map<CString, map<UINT, CString>>::iterator	itr;
		if( (itr = mDBAccess.m_stslist.find(name)) != mDBAccess.m_stslist.end() )
		{
			 list = (*itr).second;
			 return true;
		}
		return false;
	}
	vector<stSelectData>&	GetSelectDataList()	{ return mSelectDataList; }
	map<CString, DWORD>&	GetSelName2Id()		{ return mSelName2Id; }
	map<DWORD, CString>&	GetSelId2Name()		{ return mSelId2Name; }
	map<DWORD, int>&		GetSelId2Index()	{ return mSelId2Index; }
	void	GetBaseStationList(vector<CString>& list);
	void	GetEquipmentList(CString baseName, vector<CString>& list);
	void	GetDataNameList(CString baseName, CString equipment, vector<CString>& list);

	DWORD	ConvNameToId(CString name);
	CString	ConvIdToName(DWORD id);

	DWORD	GetSelectDataId(CString name);
	CString	GetSelectDataName(DWORD id);
	int		GetSelectDataNo(DWORD id);

	void	RequestAddSignalList();
	void	RequestStartSignal();
	void	RequestStopSignal();
	void	RequestDataList();
	DWORD	GetData(LPBYTE lpData, DWORD dwSize);

	void	DeleteCollectionAll()
	{
		mSelectDataList.clear();
		mSelName2Id.clear();
		mSelId2Name.clear();
		mSelId2Index.clear();

		for( int i = 0; i < MAX_VIEW; i++ )
			DeleteAnalysisData(i);

		DeleteCollection();
	}
	void	DeleteCollection();
	void	StartCollection();
	void	StopCollection();
	void	StopAllAnalysisDataThread();
	bool	ConvertTrendData(UINT nStation, vector<CEngValCnv::obs_tbl_res_t>& convdata);
	bool	CreateSpaceData(LPBYTE lpData, DWORD dwSize);
	bool	IsSpaceTempEmpty();
	void	DeleteSpaceTemp();
	bool	AddCollection(UINT message, WPARAM wParam, LPARAM lParam);
	void	CompSpaceData(DWORD id, int pos, bool clr = false);
	void	ChangeDrawInfoEx(DWORD id, int tbl);
	void	UpdateDrawInfoEx(DWORD id, int tbl);

	void	DeleteSpaceRecv()
	{
		map<DWORD, CSpaceData>::iterator	itr;
		for( itr = mSpaceRecv.begin(); itr != mSpaceRecv.end(); itr++ )
		{
			(*itr).second.value.clear();
			(*itr).second.value_for_copy.clear();
		}
		mSpaceRecv.clear();
	}

	bool	IsLeapUpdateOK(stSpaceData &s1, stSpaceData s2)
	{
		// うるう秒(６０秒)⇒０秒になった時
		if( ((s1.status & SDS_LEAP) != 0) && ((s2.status & SDS_LEAP) == 0) )
		{
			// 更新元のMin/Max値に差がありかつSDS_LEAPMIN／SDS_LEAPMAXが無いならうるう秒の値はMin/Max値に埋もれているので、それ以外なら更新OK
			if( !((s1.mmPair.min != s1.mmPair.max) && ((s1.status & (SDS_LEAPMIN | SDS_LEAPMAX)) == 0)) )
				return true;
		}

		return false;
	}

	bool	SetSpaceTemp(CSpaceData& data)
	{
		bool	res = false;
		map<DWORD, CSpaceData>::iterator	itr;
		if( (itr = mSpaceRecv.find(data.dataid)) == mSpaceRecv.end() )
			mSpaceRecv.insert(map<DWORD, CSpaceData>::value_type(data.dataid, data));
		else
		{
			CTime	t_src = (*itr).second.value.front().systime;
			CTime	t_dst = data.value.front().systime;
			if( t_src.GetTime() < t_dst.GetTime() )
			{
				// トータル秒が進んだならデータを次の工程に流す
				mSpaceTemp.push_back((*itr).second);
				(*itr).second.value.clear();
				mSpaceRecv.erase(itr);
				mSpaceRecv.insert(map<DWORD, CSpaceData>::value_type(data.dataid, data));
				res = true;
			}
			else if( t_src.GetTime() == t_dst.GetTime() )
			{
				if( ((*itr).second.value.front().status == SDS_INVALID) && (data.value.front().status & SDS_VALID) )	// 無効⇔有効
				{
					// トータル秒が同じならデータを置き換え
					(*itr).second.value.clear();
					mSpaceRecv.erase(itr);
					mSpaceRecv.insert(map<DWORD, CSpaceData>::value_type(data.dataid, data));
				}
				else if( ((*itr).second.value.front().status & SDS_VALID) && (data.value.front().status & SDS_VALID) )	// 有効⇔有効
				{
					// トータル秒が同じならMM圧縮(必然的にうるう秒もMM圧縮される)
					if( (*itr).second.value.front().mmPair.max < data.value.front().mmPair.max )
					{
						// ０秒がMAX値側なら、うるう秒はMIN値側
						if( IsLeapUpdateOK((*itr).second.value.front(), data.value.front()) )
						{
							if( (*itr).second.value.front().status & SDS_LEAPMAX )
								(*itr).second.value.front().status &= ~SDS_LEAPMAX;	// うるう秒がMAX値側なら埋もれるのでフラグをクリア
							else
								(*itr).second.value.front().status |= SDS_LEAPMIN;
						}

						(*itr).second.value.front().mmPair.max = data.value.front().mmPair.max;
					}
					else if( (*itr).second.value.front().mmPair.min > data.value.front().mmPair.min )
					{
						// ０秒がMIN値側なら、うるう秒はMAX値側
						if( IsLeapUpdateOK((*itr).second.value.front(), data.value.front()) )
						{
							if( (*itr).second.value.front().status & SDS_LEAPMIN )
								(*itr).second.value.front().status &= ~SDS_LEAPMIN;	// うるう秒がMIN値側なら埋もれるのでフラグをクリア
							else
								(*itr).second.value.front().status |= SDS_LEAPMAX;
						}

						(*itr).second.value.front().mmPair.min = data.value.front().mmPair.min;
					}
					else
					{
						// ０秒がMin/Max値以内なら、うるう秒はMIN値側(固定)
						if( IsLeapUpdateOK((*itr).second.value.front(), data.value.front()) )
						{
							if( ((*itr).second.value.front().status & (SDS_LEAPMIN | SDS_LEAPMAX)) == 0 )
								(*itr).second.value.front().status |= SDS_LEAPMIN;
						}
					}
				}
			}
		}
		return res;
	}

	void	DeleteSpaceRead()
	{
		map<CString, stQueData>::iterator	itr;
		mSpaceRead.clear();
	}

	bool	PushAnalysisData(int nView, stQueData& que)
	{
		bool	res = false;
		map<CString, stQueData>::iterator	itr;
		if( (itr = mSpaceRead.find(que.name)) == mSpaceRead.end() )
			mSpaceRead.insert(map<CString, stQueData>::value_type(que.name, que));
		else
		{
			CTime	t_src = (*itr).second.data.systime;
			CTime	t_dst = que.data.systime;
			if( t_src.GetTime() < t_dst.GetTime() )
			{
				// トータル秒が進んだならデータを次の工程に流す
				PushSaveAnalysisDataQue(nView, (*itr).second);
				mSpaceRead.erase(itr);
				mSpaceRead.insert(map<CString, stQueData>::value_type(que.name, que));
				res = true;
			}
			else if( t_src.GetTime() == t_dst.GetTime() )
			{
				if( ((*itr).second.data.status == SDS_INVALID) && (que.data.status & SDS_VALID) )	// 無効⇔有効
				{
					// トータル秒が同じならデータを置き換え
					mSpaceRead.erase(itr);
					mSpaceRead.insert(map<CString, stQueData>::value_type(que.name, que));
				}
				else if( ((*itr).second.data.status & SDS_VALID) && (que.data.status & SDS_VALID) )	// 有効⇔有効
				{
					// トータル秒が同じならMM圧縮(必然的にうるう秒もMM圧縮される)
					if( (*itr).second.data.mmPair.max < que.data.mmPair.max )
					{
						// ０秒がMAX値側なら、うるう秒はMIN値側
						if( IsLeapUpdateOK((*itr).second.data, que.data) )
						{
							if( (*itr).second.data.status & SDS_LEAPMAX )
								(*itr).second.data.status &= ~SDS_LEAPMAX;	// うるう秒がMAX値側なら埋もれるのでフラグをクリア
							else
								(*itr).second.data.status |= SDS_LEAPMIN;
						}

						(*itr).second.data.mmPair.max = que.data.mmPair.max;
					}
					else if( (*itr).second.data.mmPair.min > que.data.mmPair.min )
					{
						// ０秒がMIN値側なら、うるう秒はMAX値側
						if( IsLeapUpdateOK((*itr).second.data, que.data) )
						{
							if( (*itr).second.data.status & SDS_LEAPMIN )
								(*itr).second.data.status &= ~SDS_LEAPMIN;	// うるう秒がMIN値側なら埋もれるのでフラグをクリア
							else
								(*itr).second.data.status |= SDS_LEAPMAX;
						}

						(*itr).second.data.mmPair.min = que.data.mmPair.min;
					}
					else
					{
						// ０秒がMin/Max値以内なら、うるう秒はMIN値側(固定)
						if( IsLeapUpdateOK((*itr).second.data, que.data) )
						{
							if( ((*itr).second.data.status & (SDS_LEAPMIN | SDS_LEAPMAX)) == 0 )
								(*itr).second.data.status |= SDS_LEAPMIN;
						}
					}
				}
			}
		}
		return res;
	}

	bool	PushAnalysisDataFinish(int nView)
	{
		bool	res = false;
		map<CString, stQueData>::iterator	itr;
		for( itr = mSpaceRead.begin(); itr != mSpaceRead.end(); itr++ )
		{
			PushSaveAnalysisDataQue(nView, (*itr).second);
			res = true;
		}
		mSpaceRead.clear();
		return res;
	}

	deque<stLimitState>&	GetLimitState(int view, int data)	{ return m_LimitState[view][data]; }
	void	ClearLimitState(int view, int data)
	{
		CGraphMutex::Lock(eLimitState);
		GetLimitState(view, data).clear();
		CGraphMutex::Unlock(eLimitState);
	}

	void	WriteRealTimeData();
	void	CheckLimitState(DWORD dataid, stSpaceData& data1, stSpaceData& data2);

	void	CreateAnalysisDataFromRealTime(int nView);
	void	CreateAnalysisDataFromFile(int nView, CTime nStart, CTime nEnd);
	void	FinishAnalysisDataFromFile(int nView) { removeSaveAnalysisDataThread(nView); }
	void	DeleteAnalysisData(int nView);
	void	UpdateTrendZoomData(int nView, bool bLock = true);
	void	UpdateTrendCursorData(int nView, bool bLock = true);

	void	WriteDataNameList(CString szFilePath);
	void	ReadDataNameList(CString szFilePath);

	void	SetSelectDataValid();

	void	ConvertTime(const stServerTime* unixTime, SYSTEMTIME* winTime);
	void	ConvertTime(const SYSTEMTIME* winTime, stServerTime* unixTime);
	void	ConvertTime(const long double montime, SYSTEMTIME* winTime);

	void	CreateMessageThread();
	void	RemoveMessageThread();

public:
	CMemMapList&	GetSaveRealTime(int index) { return m_SaveRealTimeData[index]; }
	CMemMapList&	GetSaveAnalysis(int view, int data) { return m_SaveAnalysisData[view][data]; }

	void ClearSaveRealTimeDataQue()
	{
		CGraphMutex::Lock(eRealTimeSave);
		for( int i = 0; i < m_SaveRealTimeDataQue.size(); i++ )
			m_SaveRealTimeDataQue[i].clear();
		m_SaveRealTimeDataQue.clear();
		CGraphMutex::Unlock(eRealTimeSave);
	}
	bool IsExistSaveRealTimeDataQue()
	{
		bool	res;
		CGraphMutex::Lock(eRealTimeSave);
		res = (m_SaveRealTimeDataQue.size() == 0) ? false : true;
		CGraphMutex::Unlock(eRealTimeSave);
		return res;
	}
	void PushSaveRealTimeDataQue(vector<stSpaceData>& val)
	{
		CGraphMutex::Lock(eRealTimeSave);
		m_SaveRealTimeDataQue.push_back(val);
		CGraphMutex::Unlock(eRealTimeSave);
	}
	bool GetSaveRealTimeDataQue(vector<stSpaceData>& val)
	{
		bool	res = false;
		CGraphMutex::Lock(eRealTimeSave);
		if( m_SaveRealTimeDataQue.size() != 0 )
		{
			val = m_SaveRealTimeDataQue.front();
//			m_SaveRealTimeDataQue.pop_front();
			res = true;
		}
		CGraphMutex::Unlock(eRealTimeSave);
		return res;
	}
	void PopSaveRealTimeDataQue()
	{
		CGraphMutex::Lock(eRealTimeSave);
		if( m_SaveRealTimeDataQue.size() != 0 )
			m_SaveRealTimeDataQue.pop_front();
		CGraphMutex::Unlock(eRealTimeSave);
	}
	void DeleteSaveRealTimeDataQue()
	{
		CGraphMutex::Lock(eRealTimeSave);
		while( !m_SaveRealTimeDataQue.empty() )
		{
			m_SaveRealTimeDataQue.front().clear();
			m_SaveRealTimeDataQue.pop_front();
		}
		CGraphMutex::Unlock(eRealTimeSave);
	}

	void ClearSaveAnalysisDataQue(int nView)
	{
		CGraphMutex::Lock(eAnalysisSave);
		m_SaveAnalysisDataQue[nView].clear();
		CGraphMutex::Unlock(eAnalysisSave);
	}
	bool IsExistSaveAnalysisDataQue(int nView)
	{
		bool	res;
		CGraphMutex::Lock(eAnalysisSave);
		res = (m_SaveAnalysisDataQue[nView].size() == 0) ? false : true;
		CGraphMutex::Unlock(eAnalysisSave);
		return res;
	}
	void PushSaveAnalysisDataQue(int nView, stQueData& val)
	{
		CGraphMutex::Lock(eAnalysisSave);
		m_SaveAnalysisDataQue[nView].push_back(val);
		CGraphMutex::Unlock(eAnalysisSave);
	}
	bool GetSaveAnalysisDataQue(int nView, stQueData& val)
	{
		bool	res = false;
		CGraphMutex::Lock(eAnalysisSave);
		if( m_SaveAnalysisDataQue[nView].size() != 0 )
		{
			val = m_SaveAnalysisDataQue[nView].front();
//			m_SaveAnalysisDataQue[nView].pop_front();
			res = true;
		}
		CGraphMutex::Unlock(eAnalysisSave);
		return res;
	}
	void PopSaveAnalysisDataQue(int nView)
	{
		CGraphMutex::Lock(eAnalysisSave);
		if( m_SaveAnalysisDataQue[nView].size() != 0 )
			m_SaveAnalysisDataQue[nView].pop_front();
		CGraphMutex::Unlock(eAnalysisSave);
	}
	void DeleteSaveAnalysisDataQue(int nView)
	{
		CGraphMutex::Lock(eAnalysisSave);
		while( !m_SaveAnalysisDataQue[nView].empty() )
			m_SaveAnalysisDataQue[nView].pop_front();
		CGraphMutex::Unlock(eAnalysisSave);
	}

protected:
	CTime	mAnalysisStart[MAX_VIEW];
	CTime	mAnalysisEnd[MAX_VIEW];

	int	mTrendAllTblNo[MAX_VIEW];
	int	mTrendZoomTblNo[MAX_VIEW];

	unsigned __int64	mTrendZoomStart[MAX_VIEW];
	unsigned __int64	mTrendZoomEnd[MAX_VIEW];

	unsigned __int64	mTrendCursor[MAX_VIEW];

	// データの種類：ビュー毎にプロパティで選択されているデータを保存
	// データの順番：ビュー毎にプロパティで選択の順番に保存
	vector<stSpaceData>	mTrendAllData[MAX_VIEW][MAX_TDATA];

	// データの種類：ビュー毎にプロパティで選択されているデータを保存
	// データの順番：ビュー毎にプロパティで選択の順番に保存
	CMemMapList			mTrendZoomMemMap[MAX_VIEW][MAX_TDATA];
	vector<stSpaceData>	mTrendZoomData[MAX_VIEW][MAX_TDATA];

	stMMPair	mTrendZoomMMPair[MAX_VIEW][MAX_TDATA];

	vector<stSpaceData>	mTrendCursorData[MAX_VIEW][MAX_TDATA];

public:
	CTime		GetAnalysisStart(int nView)		{ return mAnalysisStart[nView]; }
	CTime		GetAnalysisEnd(int nView)		{ return mAnalysisEnd[nView]; }
	CTimeSpan	GetAnalysisElapsed(int nView)		// 開始時刻からの終了時刻までの時間
	{
		return mAnalysisEnd[nView] - mAnalysisStart[nView];
	}
	unsigned __int64	GetAnalysisCount(int nView)	// 開始時刻からの終了時刻までの点数
	{
		if( (GetAnalysisStart(nView) == CTime(0)) && (GetAnalysisEnd(nView) == CTime(0)) )
			return 0;

		return GetAnalysisElapsed(nView).GetTotalSeconds() + 1;
	}

	int			GetTrendAllTblNo(int nView)	{ return mTrendAllTblNo[nView]; }
	int			GetTrendZoomTblNo(int nView)	{ return mTrendZoomTblNo[nView]; }
	void		UpdateTrendZoomTblNo(int nView, int nTbl, bool bLock = true);

	unsigned __int64	GetTrendZoomStart(int nView)	{ return mTrendZoomStart[nView]; }
	unsigned __int64	GetTrendZoomEnd(int nView)		{ return mTrendZoomEnd[nView]; }
	void				UpdateTrendZoomPos(int nView, unsigned __int64 nStart, bool bLock = true);

	unsigned __int64	GetTrendAllCursor(int nView)	{ return mTrendCursor[nView]; }
	unsigned __int64	GetTrendZoomCursor(int nView)
	{
		if( (mTrendZoomStart[nView] <= mTrendCursor[nView]) && (mTrendCursor[nView] <= mTrendZoomEnd[nView]) )
			return mTrendCursor[nView] - mTrendZoomStart[nView];
		else
			return -1;
	}
	void				UpdateTrendCursorPos(int nView, unsigned __int64 nPos, bool bLock = true);

	vector<stSpaceData>& GetTrendAllData(int nView, int nData) { return mTrendAllData[nView][nData]; }
	CMemMapList&	GetTrendZoomMemMap(int nView, int nData) { return mTrendZoomMemMap[nView][nData]; }
	vector<stSpaceData>& GetTrendZoomData(int nView, int nData) { return mTrendZoomData[nView][nData]; }

	stMMPair& GetTrendZoomMMPair(int nView, int nData) { return mTrendZoomMMPair[nView][nData]; }

	vector<stSpaceData>& GetTrendCursorData(int nView, int nData)	{ return mTrendCursorData[nView][nData]; }
	vector<stSpaceData>& GetTrendCursorDataMaster(int nView)
	{
		for( int i = 0; i < MAX_TDATA; i++ )
		{
			if( mTrendCursorData[nView][i].size() != 0 )
				return mTrendCursorData[nView][i];
		}

		return mTrendCursorData[nView][0];
	}

public:
	void	OutputCsvFile(int nView, CString sFilePath);

protected:
	void	createCollectionThread();
	void	removeCollectionThread();
	void	createSaveRealTimeDataThread();
	void	removeSaveRealTimeDataThread();
	void	createSaveAnalysisDataThread(int nView);
	void	removeSaveAnalysisDataThread(int nView);
	static void CALLBACK messageProc(UINT message, WPARAM wParam, LPARAM lParam);

};


