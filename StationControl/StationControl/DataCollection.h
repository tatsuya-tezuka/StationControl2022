#pragma once

#include "DataCollectionThread.h"
#include "EngValCnv.h"
#include "..\utility\StationPipe.h"

using namespace std;


//=============================================================================
//	構造体
//=============================================================================
//=============================================================================
//	サマリデータ
//=============================================================================
#define MAX_SUMMARY_DATA	50
typedef struct {
	double	value;
} stSummaryData;
//=============================================================================
//	履歴データ
//=============================================================================
#define MAX_HISTORY_DATA 1000
#define MAX_HISTORYSEARCH_DATA 50000

//=============================================================================
//	定義
//=============================================================================

// 送信元
static const CString mHistEquipmentString[] = {
	_T("局運用管制装置"), _T("設備制御装置"), _T("X帯TLM入力装置(2)"), _T("X帯CMD出力装置(2)"),
};
static const CString mHistEquipmentStringSimple[] = {
	_T("局"), _T("設"), _T("XTLM"), _T("XCMD"),
};
enum{
	eSenderItem1,
	eSenderItem2,
	eSenderItem3,
	eSenderItem4,

	eSenderItemMax,
};

// ステータス
static const CString mHistStatusString[] = {
	_T("情報"), _T("警告"), _T("異常"), _T("STATUS"), _T("WARNING"), _T("FAULT"), _T("REMOTE/LOCAL"), _T("POWER ON/OFF"),
};
static const CString mHistImStatusString[] = {
	_T("復旧"), _T("警告"), _T("異常"),
};
static const CString mEqHistStatusString[] = {
	_T("STATUS"), _T("WARNING"), _T("FAULT"), _T("REMOTE/LOCAL"), _T("POWER ON/OFF"),
};

enum{
	eHistStatus_Normal,
	eHistStatus_Warning,
	eHistStatus_Fault,
};

enum{
	eEqHistStatus_Status,
	eEqHistStatus_Warning,
	eEqHistStatus_Fault,
	eEqHistStatus_RemoteLocal,
	eEqHistStatus_PowerOnOff,
	eEqHistStatus_Max,
};

//=============================================================================
//	設備運用状況
//=============================================================================
#define mTitleEquipment	_T("設備状態")
#define mTitleOperation	_T("運用状態")
#ifdef _DEBUG
static const CString mEqString[4][5] = {
	{ _T("DSN\nMODEM"), _T(""), _T(""), _T(""), _T("") },
	{ _T("TRK-RX\n"), _T(""), _T(""), _T(""), _T("") },
	{ _T("ACU\n"), _T(""), _T(""), _T(""), _T("") },
	{ _T("SSPA\n"), _T(""), _T(""), _T(""), _T("") },
};
static const CString mOpeString[4][5] = {
	{ _T("AGC\nCAL"), _T("STLM\nSET"), _T("STLM\nON"), _T("STLM\nPASS"), _T("STLM\nOFF") },
	{ _T("TLM\nCHK"), _T("XTLM\nSET"), _T("XTLM\nON"), _T("XTLM\nPASS"), _T("XTLM\nOFF") },
	{ _T("CMD\nCHK"), _T("U/L\nSET"), _T("U/L\nON"), _T("U/L\nPASS"), _T("U/L\nOFF") },
	{ _T("RNG\nCAL"), _T("RNG\nSET"), _T("RNG\nON"), _T("RNG\nPASS"), _T("RNG\nOFF") },
};
#endif

// グラフ監視数
#define GRAPH_KANSHI_NUM	50


// CDataCollection コマンド ターゲット

class CDataCollection
{
public:
	CDataCollection();
	virtual ~CDataCollection();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	static const time_t		mKanshiDisposeTimeFromUTC = 60 * 60 * 24 * 360;		// 監視のUTC時刻からの破棄判定時間(s)

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	int				m_angle[80];
	CSpaceData				mSpaceData[eStation_MAX][GRAPH_KANSHI_NUM];	// 監視データ
	deque<CSpaceData>		mSpaceTemp[eStation_MAX];		// サーバ受信用のワーク
	deque<CSpaceData>		mSpaceEngValTemp[eStation_MAX];	// サーバ受信用のワーク（工学値変換後データ）
	deque<CSpaceData>		mForecastData[eStation_MAX];	// 予報値監視データ
	deque<CSpaceData>		mForecastTemp[eStation_MAX];	// 予報値のワーク

	map<DWORD, CString>		mDataNameList[eStation_MAX];			// データ名称リスト
	map<CString, UINT>		mGraphSigIndexMap[eStation_MAX];		// 衛星捕捉グラフ用監視のマップインデックス
	int						mGraphSigIndexNum[eStation_MAX];		// 衛星捕捉グラフ用監視のインデックス個数
	CDataCollectionThread*	mpCollectThread;		// データ収集スレッド
	deque<stHistoryData>	mOperationHistoryData;	// 運用履歴データ
	deque<stHistoryData>	mOperationHistoryTemp;	// 運用履歴データサーバ受信用のワーク
	deque<stHistoryData>	mControlHistoryData;	// 制御履歴データ
	deque<stHistoryData>	mControlHistoryTemp;	// 制御履歴データサーバ受信用のワーク
	deque<stHistoryData>	mEquipmentHistoryData;	// 設備状況履歴データ
	deque<stHistoryData>	mEquipmentHistoryTemp;	// 設備状況履歴データサーバ受信用のワーク
	deque<stHistoryData>	mEventHistoryData;		// イベント履歴データ
	deque<stHistoryData>	mEventHistoryTemp;		// イベント履歴データサーバ受信用のワーク
	deque<stHistoryData>	mSubEquipmentHistoryData;	// 設備状況履歴データ（サブ画面）
	deque<stHistoryData>	mSubEventHistoryData;		// イベント履歴データ（サブ画面）

	deque<PIPEMONITORACK>	mMessageTemp[eStation_MAX];

//	map<time_t, double>		mELPredInterpolateMap[eStation_MAX];	// EL予報値補間データマップ
	vector<stPredData>		mELPredInterpolateList[eStation_MAX];	// EL予報値補間データリスト

	DWORD					m_nHistID54;			// 54m 履歴識別番号

#ifdef _NOCOMM
	CStdioFile	mOperationHistFile;
	CStdioFile	mControlHistFile;
	CStdioFile	mEquipmentHistFile;
	CStdioFile	mEventHistFile;
#endif

//#ifdef _DEBUG
//	time_t	m_shortCycleTime;
//	time_t	m_integtime;
//#endif

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

	CSpaceData*				GetSpaceData(int nStation = eStation_MAX);
	deque<CSpaceData>&		GetSpaceTemp(int nStation = eStation_MAX);
	deque<CSpaceData>&		GetSpaceEngValTemp(int nStation = eStation_MAX);
//	map<time_t, double>&	GetELPredInterpolateMap(UINT nStation)	{ return mELPredInterpolateMap[nStation]; }
	vector<stPredData>&		GetELPredInterpolateList(UINT nStation)	{ return mELPredInterpolateList[nStation]; }

	int					GetGraphSigIndex(int nStation, CString& strSigName) {
		if (nStation < 0 || nStation >= eStation_MAX) {
			if (mGraphSigIndexMap[0][strSigName] == 0)
				return -1;		// グラフのインデックスマップに監視が存在しない場合
			return (int)mGraphSigIndexMap[0][strSigName] - 1;
		}

		if (mGraphSigIndexMap[nStation][strSigName] == 0)
			return -1;		// グラフのインデックスマップに監視が存在しない場合

		return (int)mGraphSigIndexMap[nStation][strSigName] - 1;
	}
	
	int					GetGraphSigSize(int nStation = eStation_MAX) {
		if (nStation < 0 || nStation >= eStation_MAX) {
//			return (int)mGraphSigIndexMap[0].size();
			return mGraphSigIndexNum[0];
		}

//		return (int)mGraphSigIndexMap[nStation].size();
		return mGraphSigIndexNum[nStation];
	}


	BOOL DbRead();

	deque<stHistoryData>&	GetOperationHistoryData()	{ return mOperationHistoryData; }
	deque<stHistoryData>&	GetOperationHistoryTemp()	{ return mOperationHistoryTemp; }
	deque<stHistoryData>&	GetControlHistoryData()		{ return mControlHistoryData; }
	deque<stHistoryData>&	GetControlHistoryTemp()		{ return mControlHistoryTemp; }
	deque<stHistoryData>&	GetEquipmentHistoryData()		{ return mEquipmentHistoryData; }
	deque<stHistoryData>&	GetEquipmentHistoryTemp()		{ return mEquipmentHistoryTemp; }
	deque<stHistoryData>&	GetEventHistoryData()		{ return mEventHistoryData; }
	deque<stHistoryData>&	GetEventHistoryTemp()		{ return mEventHistoryTemp; }
	deque<stHistoryData>&	GetSubEquipmentHistoryData()		{ return mSubEquipmentHistoryData; }
	deque<stHistoryData>&	GetSubEventHistoryData()		{ return mSubEventHistoryData; }

	deque<PIPEMONITORACK>&	GetMessageTemp(int nDispStation)		{ return mMessageTemp[nDispStation]; }

	vector<stSelectData>	mSelectDataList;		// 選択データリスト

	map<DWORD, CString>&	GetDataNameList(UINT nStation)	{ return mDataNameList[nStation]; }
	vector<stSelectData>&	GetSelectDataList()	{ return mSelectDataList; }
	void	GetBaseStationList(UINT nStation, vector<CString>& list);
	void	GetEquipmentList(UINT nStation, CString baseName, vector<CString>& list);
	void	GetDataNameList(UINT nStation, CString baseName, CString equipment, vector<CString>& list);

	DWORD	ConvertNameToId(UINT nStation, CString name);
	CString	ConvertIdToName(UINT nStation, DWORD id);
	void	CreateSpaceData(LPBYTE lpData, DWORD dwSize, UINT nStation);
	void	CreateForecastData(stServerItemData* pdata, UINT nStation);

	CString ConvUtf8SjisEx(std::string const& src);
	CString ConvUtf8Unicode(std::string const& src);

	bool	IsMessageEvent(TCHAR* pmsg);
	bool	IsMessageLimit(DWORD msgno);

	CString	UTF8_3CharsToSJis(char *pUtf8Str);
	void	CreateDataToHistory(PIPEMONITORACK* pbuf);
	void	CreateOperationHistory();
	void	CreateControlHistory();
	void	CreateEquipmentHistory();
	void	CreateEventHistory();

	void	StartCollection();
	void	StopCollection();
	bool	AddCollection(UINT message, WPARAM wParam, LPARAM lParam);				// ダミー用データを衛星リストに追加
	bool	AddCollection(UINT nStation, vector<CEngValCnv::obs_tbl_res_t>& data);		// 監視生データを衛星リストに追加
	void	ClearCollection(UINT nStation, CString strGName);							// グラフ信号名のデータをキューから削除する
	void	ClearCollection(UINT nStation);												// グラフ信号名のデータをキューから削除する
	UINT	AddHistory(UINT message, WPARAM wParam, LPARAM lParam);

	void	SetKanshiName();

	CString ReplaceAreaKanshiName(CString& str);

	void	SetHistID54(DWORD val)			{ m_nHistID54 = val; }

protected:
	void	createCollectionThread();
	void	removeMeasureThread();
	static void CALLBACK messageProc(UINT message, WPARAM wParam, LPARAM lParam);

	void	LimitQueueSize(deque<stSpaceData>& value);

	/* ------------------------------------------------------------------------------------ */
};


