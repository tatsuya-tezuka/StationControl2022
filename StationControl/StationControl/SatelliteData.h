#pragma once

#include "const.h"
#include "CommBase.h"
#include "EngValCnv.h"
#include "ComboBoxEx2.h"

using namespace std;


// PLL BW制御定義
#define SRCV_PLLBW				_T("SS-TCRDEM.CARR_LOOP")	/* SS-TCRDEM.CARR_LOOP */
#define XRCV_PLLBW				_T("DSNTCR.CARR_LOOP_BAND")	/* DSNTCR.CARR_LOOP_BAND */
#define XRCVOLD_PLLBW			_T("X-RX2.PLL_BW")			/* X-RX2.PLL_BW */

#define SRCV34_PLLBW			_T("SS-TCRDEM.CARR_LOOP")	/* SS-TCRDEM.CARR_LOOP */
#define XRCV34_PLLBW			_T("DSNTCR.CARR_LOOP_BAND")	/* DSNTCR.CARR_LOOP_BAND */
#define SCAP34_PLLBW			_T("S-ACQ_TRKRX.CARR_LOOP_BAND")	/* S-ACQ_TRKRX.CARR_LOOP_BAND */
#define XCAP34_PLLBW			_T("X-ACQ_TRKRX.CARR_LOOP_BAND")	/* X-ACQ_TRKRX.CARR_LOOP_BAND */
#define STRACK34_PLLBW			_T("S-TRKRX.CARR_LBW")		/* S-TRKRX.CARR_LBW */
#define XTRACK34_PLLBW			_T("X-TRKRX.CARR_LOOP_BAND")		/* X-TRKRX.CARR_LOOP_BAND */


#define RARRBAND_ID     0x03002e /* ＲＡＲＲ使用帯域情報であることの識別ＩＤ:2000-11-24*/
#define ENDOFSAT_ID     0x03ffff /* 個々の衛星情報の定義の終了を現すＩＤ */

enum{
	eShareFile_PredList=0x00000001,
	eShareFile_PlanList = 0x00000002,
};


#define	MAX_SIZE_PER_MMDATA		100		// メモリマップデータあたりの最大測定データ数
#define	NUM_AROUND_OPEN_MMDATA	1		// 前後のメモリマップデータファイルオープン数


// レンジ伝搬モニタ定義
enum eRngTransMode {
	eRngTransMode_SS = 0,
	eRngTransMode_XX_XKA_54m = 0,
	eRngTransMode_XXOldPast = 1,
	eRngTransMode_XX = 1,
	eRngTransMode_XXOldRegene = 2,
	eRngTransMode_XXNew,
	eRngTransMode_Num
};

enum eRngGeneration {
	eRngGeneration_Past = 0,
	eRngGeneration_Regene,
};

enum eRngStep {
	eRngStep_1 = 0,
	eRngStep_2,
	eRngStep_3,
	eRngStep_4,
	eRngStep_5,
	eRngStep_6,
	eRngStep_Num,
};



typedef enum{
	TYPE_AGC_S,
	TYPE_AGC_X,
	TYPE_RNG,
	TYPE_TLM_S,
	TYPE_TLM_X,
	TYPE_CMD,
	TYPE_AGC_KA,
	TYPE_TLM_KA
}eType;

typedef enum{
	SHELL_CTRL,
	SHELL_CALIB,
}eShell;

typedef enum{
	STATE_NONE,
	STATE_PRE,
	STATE_WAIT,
	STATE_RUN,
	STATE_STP,
}eState;

static const int mRdnsRunLogItem = 100;

typedef struct{
	int		item;
	eType	type;
	eShell	ShellType;
	CString	satellite;
	CString Msg1;
	CString Msg2;
	CString MsgCancel;
	CString MonString;
	eState	state;
	int		time;
	UINT	station;
	CString RdnsItem;
}stSendCtrlMsg;

typedef struct{
	CString ctrl;
	CString loopband;
	int		time;
}stCalibPlan;

typedef struct {
	CString		nameK;			// 監視名
	CString		value1;			// 監視値1
	CString		value2;			// 監視値2
} structRngStep;

static structRngStep mRngStep[eStation_MAX][eRngTransMode_Num][eRngStep_Num] = {
	// 臼田64m
	{
		// S/S
		{
			{ _T("CTRL.PASS_FLAG"), _T("パス中"), _T("") },		        // Step1
			{ _T("SS-TCRDEM.RNG_CONT"), _T("START"), _T("") },		// Step2
			{ _T(""), _T(""), _T("") },		        // Step3
			{ _T("SS-TCRDEM.RNG_CONT"), _T("START"), _T("STOP") },	// Step4
			{ _T("SS-TCRRARR.RNG_TX_STS"), _T("A_CODE"), _T("A_CODE") },	// Step5
			{ _T(""), _T(""), _T("") },		        // Step6
		},

		// X/X（旧系：従来）
		{
			{ _T("CTRL.PASS_FLAG"), _T("パス中"), _T("") },		        // Step1
			{ _T("XX-RARR.RNG_CONT"), _T("START"), _T("") },		// Step2
			{ _T("XX-RARR.RNG_CONT"), _T("START"), _T("") },		// Step3
			{ _T("XX-RARR.RNG_CONT"), _T("START"), _T("STOP") },	// Step4
			{ _T("XX-RARR.TX_STATUS"), _T("A_CODE"), _T("A_CODE") },	// Step5
			{ _T(""), _T(""), _T("") },		        // Step6
		},

		// X/X（旧系：再生）
		{
			{ _T("CTRL.PASS_FLAG"), _T("パス中"), _T("") },		        // Step1
			{ _T("XX-RRNG.RNG_CONT"), _T("START"), _T("") },		// Step2
			{ _T("XX-RRNG.RNG_CONT"), _T("START"), _T("") },		// Step3
			{ _T("XX-RRNG.RNG_CONT"), _T("START"), _T("STOP") },	// Step4
			{ _T("XX-RRNGDATA.RNG_TX_STS"), _T("AB_CODE"), _T("AB_CODE") },		// Step5
			{ _T(""), _T(""), _T("") },		        // Step6
		},

		// X/X（新系）
		{
			{ _T("CTRL.PASS_FLAG"), _T("パス中"), _T("") },		        // Step1
			{ _T("DSNTCR.RNG_CONT"), _T("START"), _T("") },		// Step2
			{ _T("DSNTCR.RNG_MODE"), _T("TURNAROUND"), _T("REGEN") },	// Step3
			{ _T("DSNTCR.RNG_CONT"), _T("START"), _T("STOP") },	// Step4
			{ _T("DSNTCR.RNG_TX_SEQUENCE"), _T("A_CODE"), _T("AB_CODE") },		// Step5
			{ _T(""), _T(""), _T("") },		        // Step6
		},
	},

	// 臼田54m
	{
		// X/X, X/Ka共通
		{
			{ _T("CTRL.PASS_FLAG"), _T("パス中"), _T("") },		        // Step1
			{ _T("DSNTCR.RNG_CONT"), _T("START"), _T("") },		// Step2
			{ _T("DSNTCR.RNG_MODE"), _T("TURNAROUND"), _T("REGEN") },	// Step3
			{ _T("DSNTCR.RNG_CONT"), _T("START"), _T("STOP") },	// Step4
			{ _T("DSNTCR.RNG_TX_SEQUENCE"), _T("A_CODE"), _T("AB_CODE") },	// Step5
			{ _T(""), _T(""), _T("") },		        // Step6
		},
		{
			{ _T(""), _T(""), _T("") },
		},
		{
			{ _T(""), _T(""), _T("") },
		},
		{
			{ _T(""), _T(""), _T("") },
		}

	},

	// 内之浦34m
	{
		// S/S
		{
			{ _T("CTRL.PASS_FLAG"), _T("パス中"), _T("") },		        // Step1
			{ _T("SS-TCRDEM.RNG_CONT"), _T("START"), _T("") },		// Step2
			{ _T(""), _T(""), _T("") },		        // Step3
			{ _T("SS-TCRDEM.RNG_CONT"), _T("START"), _T("STOP") },	// Step4
			{ _T("SS-TCRRARR.RNG_TX_STS"), _T("A_CODE"), _T("A_CODE") },	// Step5
			{ _T(""), _T(""), _T("") },		        // Step6
		},

		// X/X
		{
			{ _T("CTRL.PASS_FLAG"), _T("パス中"), _T("") },		        // Step1
			{ _T("DSNTCR.RNG_CONT"), _T("START"), _T("") },		// Step2
			{ _T("DSNTCR.RNG_MODE"), _T("TURNAROUND"), _T("REGEN") },	// Step3
			{ _T("DSNTCR.RNG_CONT"), _T("START"), _T("STOP") },		// Step4
			{ _T("DSNTCR.RNG_TX_SEQUENCE"), _T("A_CODE"), _T("AB_CODE") },	// Step5
			{ _T(""), _T(""), _T("") },		        // Step6
		},

		{
			{ _T(""), _T(""), _T("") },
		},
		{
			{ _T(""), _T(""), _T("") },
		}
	},

	// 内之浦20m
	{
		// S/S
		{
			{ _T("CTRL.PASS_FLAG"), _T("パス中"), _T("") },		        // Step1
			{ _T("SS-TCRDEM.RNG_CONT"), _T("START"), _T("") },		// Step2
			{ _T(""), _T(""), _T("") },		        // Step3
			{ _T("SS-TCRDEM.RNG_CONT"), _T("START"), _T("") },		// Step4
			{ _T("SS-TCRRARR.RNG_TX_STS"), _T("A_CODE"), _T("A_CODE") },	// Step5
			{ _T(""), _T(""), _T("") },		        // Step6
		},

		{
			{ _T(""), _T(""), _T("") },
		},
		{
			{ _T(""), _T(""), _T("") },
		},
		{
			{ _T(""), _T(""), _T("") },
		}
	},

};

#ifdef _DEBUG_RNGPROPDUMMY
//----------------------------------------------------------------------------------------
// ダミーデータ
typedef struct {
	CString		nameK;			// 監視名
	CString		status;			// ダミー文字列
	double		val;			// ダミー値
} structStepDummy;

static structStepDummy mRngStepDummy[5][eRngStep_Num] = {
	// S/S
	{
		{ _T("CTRL.PASS_FLAG"), _T("パス中"), 1 },		    // Step1
		{ _T("SS-TCRDEM.RNG_CONT"), _T("START"), 0 },		// Step2
		{ _T(""), _T(""), 0 },								// Step3
		{ _T("SS-TCRDEM.RNG_CONT"), _T("START"), 0 },		// Step4
		{ _T("SS-TCRRARR.RNG_TX_STS"), _T("OTHER"), 0 },	// Step5
		{ _T(""), _T(""), 0 },								// Step6
	},
	// X/X（旧系：従来）
	{
		{ _T("CTRL.PASS_FLAG"), _T("パス中"), 1 },		    // Step1
		{ _T("XX-RARR.RNG_CONT"), _T("START"), 0 },			// Step2
		{ _T("XX-RARR.RNG_CONT"), _T("START"), 0 },			// Step3
		{ _T("XX-RARR.RNG_CONT"), _T("START"), 0 },			// Step4
		{ _T("XX-RARR.TX_STATUS"), _T("OTHER"), 0 },		// Step5
		{ _T(""), _T(""), 0 },								// Step6
	},
	// X/X（旧系：再生）
	{
		{ _T("CTRL.PASS_FLAG"), _T("パス中"), 1 },		    // Step1
		{ _T("XX-RRNG.RNG_CONT"), _T("START"), 0 },			// Step2
		{ _T("XX-RRNG.RNG_CONT"), _T("START"), 0 },			// Step3
		{ _T("XX-RRNG.RNG_CONT"), _T("START"), 0 },			// Step4
		{ _T("XX-RRNGDATA.RNG_TX_STS"), _T("OTHER"), 0 },	// Step5
		{ _T(""), _T(""), 0 },								// Step6
	},
	// X/X（旧系：新系、従来型）
	{
		{ _T("CTRL.PASS_FLAG"), _T("パス中"), 1 },		    // Step1
		{ _T("DSNTCR.RNG_CONT"), _T("START"), 0 },			// Step2
		{ _T("DSNTCR.RNG_MODE"), _T("TURNAROUND"), 0 },		// Step3
		{ _T("DSNTCR.RNG_CONT"), _T("START"), 0 },			// Step4
		{ _T("DSNTCR.RNG_TX_SEQUENCE"), _T("OTHER"), 0 },	// Step5
		{ _T(""), _T(""), 0 },								// Step6
	},
	// X/X（旧系：新系、再生型）
	{
		{ _T("CTRL.PASS_FLAG"), _T("パス中"), 1 },		    // Step1
		{ _T("DSNTCR.RNG_CONT"), _T("START"), 0 },			// Step2
		{ _T("DSNTCR.RNG_MODE"), _T("REGEN"), 0 },			// Step3
		{ _T("DSNTCR.RNG_CONT"), _T("START"), 0 },			// Step4
		{ _T("DSNTCR.RNG_TX_SEQUENCE"), _T("OTHER"), 0 },	// Step5
		{ _T(""), _T(""), 0 },								// Step6
	},
};

#endif


// レンジ伝搬クラス
class CRangePropagation
{
public:
	CRangePropagation();
	virtual ~CRangePropagation();
	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum eRngStatus {
		eStop = 0,
		eStart
	};

	enum eRngFormat {
		eTone = 0
	};

	typedef struct {
		UINT	count;
		time_t	tStart;
		double	offset;
		double	pos;
		double  d2WayRange;
		double	rtt;
	} structPropData;

	static const UINT m_nMaxRngNum = 9999;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CTime m_dataTime;
	eRngStatus m_eRngStatus;
	UINT m_nRngNum;
	double m_2WayRange;
	double m_rtt;
	deque<structPropData>	m_queRngSnd;
	deque<structPropData>	m_queRngRcv;
	deque<structPropData>	m_queRngAll;

	UINT m_nSndCount;		// 送信回数

	UINT m_nStation;		// 局
	UINT m_nTransMode;		// 通信モード
	UINT m_nGeneMode;		// 生成モード
	UINT m_nStep;			// ステップ
	CString m_strLastRngToneStatus;	// 前回のレンジトーンステータス

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	CTime GetDataTime()						{ return m_dataTime; }
	eRngStatus GetRngStatus()				{ return m_eRngStatus; }
	double GetRange()						{ return m_2WayRange / 2; }
	double GetRtt()							{ return m_rtt; }
	deque<structPropData>&	GetRngSndData()	{ return m_queRngSnd; }
	deque<structPropData>&	GetRngRcvData()	{ return m_queRngRcv; }
	deque<structPropData>&	GetRngAllData()	{ return m_queRngAll; }
	UINT GetTransMode()						{ return m_nTransMode; }
	UINT GetGeneMode()						{ return m_nGeneMode; }
	UINT GetRngNum()						{ return m_nRngNum; }
	UINT GetSndCount()						{ return m_nSndCount; }
	UINT GetStep()							{ return m_nStep; }

	void InitStatus();
	void InitStatus54();
	void UpdateStep();
	BOOL UpdateStepProc();
	BOOL UpdateStepProc54();
	void DetectRngTone();
	void InitRngTone();
	double Get2WayRng();
	void MoveRngTone();

	double GetKanshiDouble();
	CString GetKanshiString(int step = -1);

	void CreateDummy();

	/* ------------------------------------------------------------------------------------ */
};


// サマリプロパティクラス
class CSummaryProperties
{
public:
	CSummaryProperties();
	CSummaryProperties(UINT nViewID, UINT nStationID, UINT nParentID, CString strWndName);
	virtual ~CSummaryProperties();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	UINT			m_nViewID;					// ビューID
	UINT			m_nStationID;				// 局ID
	UINT			m_nParentID;				// 親ウィンドウID
	CString			m_strWndName;				// ウィンドウ名

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	BOOL GetViewNameList(vector<CString>& listViewName);
	UINT GetViewID()		{ return m_nViewID; }
	UINT GetStationID()		{ return m_nStationID; }
	UINT GetParentID()		{ return m_nParentID; }
	CString GetWndName()	{ return m_strWndName; }
	/* ------------------------------------------------------------------------------------ */
};


// RFトリップ角定義ファイルクラス
class CRFTripAngleFile
{
public:
	CRFTripAngleFile();
	virtual ~CRFTripAngleFile();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	UINT m_nStation;								// 局番号
	CString m_strKanshiPower;						// 送信電力取得用監視名
	CString m_strKanshiMod;							// 変調形式取得用監視名 変調形式の判定が不要な場合、* とする
	CString m_strModSectionDef;						// 通常とは異なる変調形式のRFトリップを指定する場合の 監視ステータスを定義する
													// この定義は、変調形式セクション名 TRIP_XXXX の XXXX に対応する
	vector<CString> m_listPowerInfo;				// 送信電力(STSNAME_POWER)で取得したステータスを定義するリスト
													// この順番に、各RFトリップ角データが対応する。
#if 0
	map<CString, vector<Az_El>> m_mapRftripGene;	// RFトリップ汎用マップ
	map<CString, vector<Az_El>> m_mapRftripMod;		// RFトリップ指定マップ
#else
	map<CString, vector<double>> m_mapRftripGene;	// RFトリップ汎用マップ
	map<CString, vector<double>> m_mapRftripMod;	// RFトリップ指定マップ

#endif

	CString m_strSatellite;							// 衛星名

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
//	static CString GetRfTripFilePath(CString strPwr, UINT nStation);
//	static CString GetRfTripFilePath(UINT nStation);

	void SetStation(UINT nStation)		{ m_nStation = nStation; }
	BOOL ReadFile();						// RFトリップファイル読み込み
	BOOL ReadFile(CString& strSatellite);	// 指定衛星名のRFトリップファイル読み込み
	BOOL ReadFile(UINT nStation, CString& strSatellite, CString& strPowerValue, CString& strModValue, vector<double>& listAzEl);
			// 指定の局、指定衛星名のRFトリップファイル読み込んで、入力の送信電力値、変調形式値に応じたAZ, ELリストを返す

	CString& GetKanshiPower()	{ return m_strKanshiPower; }
	CString& GetKanshiMod()		{ return m_strKanshiMod; }
	CString& GetModSectionDef()	{ return m_strModSectionDef; }
	vector<CString>& GetListPowerInfo()	{ return m_listPowerInfo; }

	BOOL GetAzEl(vector<double>& listAzEl);													// 監視の送信電力、変調形式に応じたAZ, ELリストを返す
	BOOL GetAzEl(CString& strPowerValue, CString& strModValue, vector<double>& listAzEl);	// 入力の送信電力値、変調形式値に応じたAZ, ELリストを返す
};

// 印刷ユーティリティクラス
class CPrintUtility
{
public:
	CPrintUtility();
	virtual ~CPrintUtility();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	static BOOL	PrintSjisFile(CString& filepath, CString title = _T(""));
	static BOOL PrintSjisFileList(vector<CString>& fileList, CString title);
	static BOOL	PrintUtf8File(CString& filepath, CString title = _T(""));
	static BOOL	PrintEucFile(CString& filepath, CString title = _T(""));
	static BOOL PrintEucFile(vector<CString>& listFilepath, CString title = _T(""));
	/* ------------------------------------------------------------------------------------ */
};

class CDetailPrint
{
public:
	CDetailPrint(CDC* dc);
	virtual ~CDetailPrint();
	BOOL	StartPrinting(bool bDiary=false);
	void	EndPrinting();
	void	SetTitle(CString& val) { mTitle = val; }
	void	PrintLine(const CString& str, BOOL bHeader = TRUE);
	void	PrintHeader();
	void	SetPrinterFont(bool bDiary = false);
	CFont*	CreateFont(int nPoints, int lfWeight, CString strFaceName=_T(""));
	int		GetPageNumber() { return mPageNumber; }
protected:
	CString	mTitle;
	CDC*	mDc;
	int		mPageVMargin;
	int		mPageHMargin;
	int		mPageHeight;
	int		mPageWidth;
	int		mSaveVMargin;
	int		mLineHeight;
	BOOL	mPageStarted;
	BOOL	mDocStarted;
	int		mPageNumber;

	CFont*	mFontHeader;
	CFont*	mFontSubHeader;
	CFont*	mFontBody;
};


// ファイルユーティリティクラス
class CFileUtility
{
public:
	CFileUtility();
	virtual ~CFileUtility();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	static void DeleteTmpFlie(CString& filepath, int nDay);
	static BOOL	ExchangeFileUtf8toSjis(CString& srcFilePath, CString& dstFilePath);
	static BOOL	ExchangeFileEuctoSjis(CString& srcFilePath, CString& dstFilePath, BOOL bOverWrite = TRUE);
	static CString ConvertEuc2Sjis(char& szEuc);
	/* ------------------------------------------------------------------------------------ */
};


// DBアクセスクラス
/*
*******************************************************************************
* 設備制御情報  識別ＩＤ
*******************************************************************************
*/
/*
識別ＩＤ：ＤＢコンパイラは、個々の制御項目毎にこれらの情報をobjectと
して出力する場合に、この識別ＩＤの小さい順にならびなおしてください
（制御項目個々の定義の先頭は必ずCTLNAME_IDで始まり、最後は、必ず
ENDOFCTL_IDで終わるようにしてください。）
*/
#define CTLNAME_ID      0x010001 /* 設備制御項目名称であることの識別ＩＤ */
#define PARAMTBL_ID     0x010002 /* パラメータ変換の定義を意味する識別ＩＤ */
#define ENDOFPARAM_ID   0x010003 /* パラメータ変換の定義終了を意味する識別ＩＤ */
#define VAL_ID          0x010004 /* 数値型パラメータの定義を意味する識別ＩＤ */
#define ENDOFVAL_ID     0x010005 /* 数値型パラメータの定義終了を意味する識別ＩＤ */
#define STS_ID          0x010006 /* ステータス型変換の定義を意味する識別ＩＤ */
#define ENDOFSTS_ID     0x010007 /* ステータス型変換の定義終了を意味する識別ＩＤ */
#define FUNC_ID         0x010008 /* 関数型変換の定義を意味する識別ＩＤ */
#define ENDOFFUNC_ID    0x01000a /* 関数型変換の定義終了を意味する識別ＩＤ */
#define NOPARAM_ID      0x01000b /* 無変換型変換の定義を意味する識別ＩＤ */
#define ENDOFNOPARAM_ID 0x01000c /* 無変換型変換の定義終了を意味する識別ＩＤ */
#define SET_ID          0x01000d /* 設定値情報の定義を意味する識別ＩＤ */
#define ENDOFSET_ID     0x01000e /* 設定値情報の定義終了を意味する識別ＩＤ */
#define PRECHKTBL_ID    0x01000f /* 制御前検証の開始を意味する識別ＩＤ */
#define PRECHK_ID       0x010010 /* 制御前検証の個々の検証情報を意味する識別ＩＤ */
#define ENDOFPRECHK_ID  0x010011 /* 制御前検証の終了を意味する識別ＩＤ */
#define VERIFYTBL_ID    0x010012 /* 照合情報を意味する識別ＩＤ */
#define VERIFY_ID       0x010013 /* 個々の照合情報を意味する識別ＩＤ */
#define ENDOFVFY_ID     0x010014 /* 照合情報の終了を意味する識別ＩＤ */
#define AFTCTL_ID       0x010015 /* 制御後ステータス変更の定義を意味する識別ＩＤ */
#define ENDOFAFTCTL_ID  0x010016 /* 制御後ステータス変更の定義終了を意味する識別ＩＤ */
#define ENDOFCTL_ID     0x01ffff /* 個々のコマンドの定義情報の終了を現すＩＤ */

/*
*******************************************************************************
* 衛星固有情報  識別ＩＤ
*******************************************************************************
*/

#define SATINF_ID       0x030001 /* 衛星本体情報であることの識別ＩＤ */
#define TXFQ_ID         0x030002 /* ノミナル送信周波数情報であることの識別ＩＤ */
#define TXFQVAL_ID      0x030003 /* ノミナル送信周波数設定値情報であることの識別ＩＤ */
#define ENDOFTXFQ_ID    0x030004 /* ノミナル送信周波数情報終了であることの識別ＩＤ */
#define RXFQ_ID         0x030005 /* ノミナル受信周波数情報であることの識別ＩＤ */
#define RXFQVAL_ID      0x030006 /* ノミナル受信周波数設定値情報であることの識別ＩＤ */
#define ENDOFRXFQ_ID    0x030007 /* ノミナル受信周波数情報終了であることの識別ＩＤ */
#define EIRP_ID         0x030008 /* 衛星ＥＩＲＰ情報であることの識別ＩＤ */
#define ENDOFEIRP_ID    0x030009 /* 衛星ＥＩＲＰ情報終了であることの識別ＩＤ */
#define SATTX_ID        0x03000a /* 送信機情報であることの識別ＩＤ */
#define ENDOFSATTX_ID   0x03000b /* 送信機情報終了であることの識別ＩＤ */
#define TXPWR_ID        0x03000c /* 送信機出力情報であることの識別ＩＤ */
#define ENDOFTXPWR_ID   0x03000d /* 送信機出力情報終了であることの識別ＩＤ */
#define SATANT_ID       0x03000e /* 衛星アンテナ情報であることの識別ＩＤ */
#define EIRPVAL_ID      0x03000f /* ＥＩＲＰ値情報であることの識別ＩＤ */
#define ENDOFSATANT_ID  0x030010 /* 衛星アンテナ情報終了であることの識別ＩＤ */
#define EQUSE_ID        0x030011 /* 設備使用指定情報であることの識別ＩＤ */
#define EQUSENAME_ID    0x030012 /* 使用設備名情報であることの識別ＩＤ */
#define ENDOFEQUSE_ID   0x030013 /* 設備使用指定情報終了であることの識別ＩＤ */
#define AGC_ID          0x030014 /* ＡＧＣ校正データファイル名称であることの識別ＩＤ */
#define RNG_ID          0x030015 /* ＲＮＧ局内ディレイ値ファイル名称であることの識別ＩＤ */
#define PRD_ID          0x030016 /* 予報値ファイル位置であることの識別ＩＤ */
#define MACDEF_ID       0x030017 /* 固有マクロ情報であることの識別ＩＤ */
#define ENDOFMACDEF_ID  0x030018 /* 固有マクロ情報終了であることの識別ＩＤ */
#define MAC_ID          0x030019 /* マクロ名情報であることの識別ＩＤ */
#define ENDOFMAC_ID     0x03001a /* マクロ名情報終了であることの識別ＩＤ */
#define CASE_ID         0x03001b /* 設備制御マクロのパラメータであることの識別ＩＤ */
#define BREAK_ID        0x03001c /* 設備制御マクロのパラメータ終了であることの識別ＩＤ */
#define EXEC_ID         0x03001d /* 設備制御実行文であることの識別ＩＤ */
#define ENDOFEXEC_ID    0x03001e /* 設備制御実行文であることの識別ＩＤ */
#define TLMBR_ID        0x03001f /* テレメトリビットレート情報であることの識別ＩＤ */
#define TLMBRVAL_ID     0x030020 /* テレメトリビットレート設定値情報であることの識別ＩＤ */
#define ENDOFTLMBR_ID   0x030021 /* テレメトリビットレート情報終了であることの識別ＩＤ */
#define CMDBR_ID        0x030022 /* コマンドビットレート情報であることの識別ＩＤ */
#define CMDBRVAL_ID     0x030023 /* コマンドビットレート設定値情報であることの識別ＩＤ */
#define ENDOFCMDBR_ID   0x030024 /* コマンドビットレート情報終了であることの識別ＩＤ */
#define PLAN_ID         0x030025 /* 計画ファイル位置であることの識別ＩＤ */
#define PASLOG_ID       0x030026 /* パス履歴ファイル位置であることの識別ＩＤ */
#define CATCH_ID        0x030027 /* 衛星補足情報であることの識別ＩＤ */
#define AGCRX_ID        0x030029 /* ＡＧＣ校正対象受信機情報であることの識別ＩＤ */
#define TOPANGL_ID      0x03002a /* 天頂通過監視角度情報であることの識別ＩＤ */
#define XTX_ID          0x03002b /* Ｘ帯送信設備使用有無情報であることの識別ＩＤ */
#define XRXSEL_ID       0x03002c /* Ｘ帯受信設備対象情報であることの識別ＩＤ */
#define RAMPCNT_ID      0x03002d /* 周波数ランプ制御許可情報であることの識別ＩＤ */
#define RARRBAND_ID     0x03002e /* ＲＡＲＲ使用帯域情報であることの識別ＩＤ */


#define ENDOFSAT_ID     0x03ffff /* 個々の衛星情報の定義の終了を現すＩＤ */

//----------------
#define VECMAX			256		/* ＤＢ検索最大個数（パラメータ数も同意。） */
#define	SATNAME_LEN		16		/* 衛星名称最大長+1 */
#define	SATCTLNAME_LEN	24		/* 接続衛星管制名称最大長+1 */
#define	TXPWRSTR_LEN	24		/* 衛星送信機出力識別最大長+1 */
#define	PRDPATH_LEN		128		/* 予報値ファイル格納ディレクトリ名称最大長+1 */
#define	PLANPATH_LEN	128		/* 計画ファイル格納ディレクトリ名称最大長+1 */
#define	MACNAME_LEN		24		/* 設備制御マクロ名最大長+1 */
#define	MACPARAM_LEN	24		/* 設備制御マクロパラメータ最大長+1 */
#define	EXECSTR_LEN		64		/* マクロ用設備制御実行文最大長+1 */

#define	CTLNAME_LEN		28								/* 設備制御名称最大長+1 */
#define	EQNAME_LEN		12								/* 設備名称最大長+1 */
#define	CTLID_LEN		( CTLNAME_LEN + EQNAME_LEN )	/* 制御項目ID最大長+1 */
#define	OPTSTR_LEN		24								/* 制御項目定義 付加情報最大長+1 */

//----------------
#define TXBAND_S_STR	"S"	/* 「Sﾊﾞﾝﾄﾞ」指定文字列 */
#define TXBAND_X_STR	"X"	/* 「Xﾊﾞﾝﾄﾞ」指定文字列 */

/* ------------------------------------------------------------------------------------ */
/* 定義                                                                                 */
/* ------------------------------------------------------------------------------------ */
struct res_t { /*共有メモリ上のテーブル構造*/
	long  l_id;
	long  l_len;
	char  buf[1];
};

struct mac_t {	/* マクロ名定義テーブル: ソース上"mac"に相当 */
	long l_id;	/* データｉｄ: 定数=MAC_ID値 */
	long l_len;	/* データ長  : sizeof( struct mac_t ) */
	char sz_macname[MACNAME_LEN];	/* 設備制御マクロ名 */
};

struct endofmac_t {	/* マクロ名定義テーブル終了: ソース上"endofmac"に相当 */
	long l_id;	/* データｉｄ: 定数=ENDOFMAC_ID値 */
	long l_len;	/* データ長  : sizeof( struct endofmac_t ) */
};
struct case_t {	/* マクロ定義テーブル: ソース上"case"に相当 */
	long l_id;	/* データｉｄ: 定数=CASE_ID値 */
	long l_len;	/* データ長  : sizeof( struct case_t ) */
	char sz_param[MACPARAM_LEN];	/* 設備制御マクロパラメータ */
};

struct exec_t {	/* 実行設備制御情報定義テーブル: ソース上"exec"に相当 */
	long l_id;	/* データｉｄ: 定数=EXEC_ID値 */
	long l_len;	/* データ長  : sizeof( struct exec_t ) */
	char sz_ctl[EXECSTR_LEN];	/* マクロ用設備制御実行文 */
	long l_exofs;	/* 実行時間オフセット値:省略時は０を設定する */
	long l_rfu;	/* パディング */
};

struct endofexec_t {	/* 実行設備制御情報定義テーブル終了: */
	long l_id;	/* データｉｄ: 定数=ENDOFEXEC_ID値 */
	long l_len;	/* データ長  : sizeof( struct endofexec_t ) */
};


/*
*******************************************************************************
* 設備制御名称情報
*******************************************************************************
*/
struct ctlname_t {				/* このテーブルは全制御項目について存在する */
	long l_id;					/* データｉｄ: 定数=CTLNAME_ID値 */
	long l_len;					/* データ長  : sizeof( struct ctlname_t ) */
	long l_no;					/* 制御項目番号（コンパイラがシーケンシャルに付加する。） */
	long l_adr;					/* 設置位置情報:ユニットアドレス(3bit) */
	char sz_ctlname[CTLID_LEN];	/* 制御項目ＩＤ: 設備名.制御項目名 */
	char sz_optstr[OPTSTR_LEN];	/* 付加情報 */

	long l_eqno;				/* 設備番号（＊１） *//* Ver1.05 */
	long l_rfu;					/* パディング *//* Ver1.05 */
};

/* 注記
＊１：コンパイル時に、制御項目ＩＤより設備名を取得し、設備情報ＤＢの
「設備情報定義」を参照し、「設備番号」を設定する。
*/

/*
*******************************************************************************
* 変換情報定義
*******************************************************************************
*/

struct paramtbl_t {		/* 変換情報テーブル: ソース上"param"に相当 */
	long l_id;			/* データｉｄ: 定数=PARAMTBL_ID値 */
	long l_len;			/* データ長  : sizeof( struct paramtbl_t ) */
	long l_paramno;		/* パラメ－タ番号（＊Ｂ１） */
	long l_intype;		/* 入力形式:（＊Ｂ２） */
	long l_incolint;	/* 入力桁数: 整数部（＊Ｂ４）*/
	long l_incolflt;	/* 入力桁数: 小数部（＊Ｂ４）（＊Ｂ５）*/
	long l_incolexp;	/* 入力桁数: 指数部（＊Ｂ４）（＊Ｂ５）*/
	long l_cvttype;		/* 変換形式:（＊Ｂ３）*/
	long l_cvtcolint;	/* 変換桁数: 整数部（＊Ｂ４） */
	long l_cvtcolflt;	/* 変換桁数: 小数部（＊Ｂ４）（＊Ｂ５）*/
	long l_cvtcolexp;	/* 変換桁数: 指数部（＊Ｂ４）（＊Ｂ５）*/
	long l_rfu;		/* パディング */
};
struct endofparamtbl_t {/* 変換情報テーブル終了: ソース上"endofparam"に相当 */
	long l_id;	/* データｉｄ: 定数=ENDOFPARAM_ID値 */
	long l_len;	/* データ長  : sizeof( struct endofparam_t ) */
};


/* 注記
＊Ｂ１：パラメータは複数存在可とするので、この番号を用いて「第何パラメータ」の
情報であるかを定義する。（１～）
＊Ｂ２：ソース上文字列で表されている形式をID値に変換して代入する。
＊Ｂ３：ソース上文字列で表されている形式をID値に変換して代入する。
＊Ｂ４："n.mEe"の形式で指定される(n)を整数部に、(m)を小数部に、(e)を指数部に
設定する。（"n"及び"n.m"の形式の場合もあり。）
＊Ｂ５：省略された場合、0を設定する。
*/

/*
入力形式文字列定義
*/
#define	IN_D_STR	"d"	/* アナログ値（符号付き10進数入力）*/
#define	IN_I_STR	"i"	/* アナログ値（整数値入力。先頭に"0x"もしくは"0X"がつけば16進数）*/
#define	IN_U_STR	"u"	/* アナログ値（符号無し10進数入力）*/
#define	IN_X_STR	"x"	/* アナログ値（16進数整数入力。先頭に"0x"あるいは"0X"が付いても付かなくても良い。）*/
#define	IN_F_STR	"f"	/* アナログ値（浮動小数点入力）*/
#define	IN_S_STR	"s"	/* 文字列 */
#define	IN_PRED_STR	"P"	/* 予報値ファイル名＋衛星名 h10.10.30 by yajima */
#define	IN_TIME_STR	"T"	/* 時刻形式文字列(HH:MM:SS) */
#define	IN_DATE_STR	"D"	/* 通算日形式文字列(DDD-HH:MM:SS) */
#define	IN_YEAR_STR	"Y"	/* 日付形式文字列(YYYY-MM-DD) */
#define	IN_NONE_STR	""	/* （省略）パラメータ無し。*/

/*
入力形式ＩＤ値定義
*/
#define	IN_D_ID		1	/* アナログ値（符号付き10進数入力）*/
#define	IN_I_ID		2	/* アナログ値（整数値入力。先頭に"0x"もしくは"0X"がつけば16進数）*/
#define	IN_U_ID		3	/* アナログ値（符号無し10進数入力）*/
#define	IN_X_ID		4	/* アナログ値（16進数整数入力。先頭に"0x"あるいは"0X"が付いても付かなくても良い。）*/
#define	IN_F_ID		5	/* アナログ値（浮動小数点入力）*/
#define	IN_S_ID		6	/* 文字列 */
#define	IN_PRED_ID 10	/* 予報値ファイル名＋衛星名 h10.10.30 by yajima */
#define	IN_TIME_ID	7	/* 時刻形式文字列(HH:MM:SS) */
#define	IN_DATE_ID	8	/* 通算日形式文字列(DDD-HH:MM:SS) */
#define	IN_YEAR_ID	9	/* 日付形式文字列(YYYY-MM-DD) */
#define	IN_NONE_ID	-1	/* （省略）パラメータ無し。*/

/*
変換形式文字列定義
*/
#define	CVT_B_STR	"B"	/* BCD変換 */
#define	CVT_A_STR	"A"	/* アナログ変換 */
#define	CVT_S_STR	"S"	/* ステータス変換 */
#define	CVT_F_STR	"F"	/* 関数変換 */
#define	CVT_NONE_STR	""	/* （省略）無変換 */
#define	CVT_STR_STR	"Z"	/* 文字列変換 *//* Ver1.07 */
#define CVT_L_STR   "L" /* 64bit整数変換 *//* 2013-TRUBO */
#define CVT_U_STR   "U" /* 64bit正整数変換 *//* 2013-TRUBO */

/*
変換形式ＩＤ値定義
*/
#define	CVT_B_ID	1	/* BCD変換 */
#define	CVT_A_ID	2	/* アナログ変換 */
#define	CVT_S_ID	3	/* ステータス変換 */
#define	CVT_F_ID	4	/* 関数変換 */
#define	CVT_NONE_ID	-1	/* （省略）無変換 */
#define	CVT_STR_ID	5	/* 文字列変換 *//* Ver1.07 */
#define CVT_L_ID    6   /* 64bit整数変換 *//* 2013-TRUBO */
#define CVT_U_ID    7   /* 64bit正整数変換 *//* 2013-TRUBO */

enum{
	eControlStatus,
	eControlNumeric,
	eControlDouble,
	eControlTime,
	eControlCommand,
	eControlNone,
};

typedef struct  {
	UINT		type;				// 0:文字列 1:数値 2:小数点 3:TIME
	char		buf[80];
	UINT		val;
	double		fval;
	__int64		tval;
	bool		ret;
} stControlValue;

typedef struct  {
	UINT		id;				// コントロールID
	CString		nameK;			// 監視名
	CString		nameS;			// 制御名
	UINT		type;			// 0:文字列 1:数値 2:小数点 3:TIME
} stControlParam;

typedef struct  {
	UINT		id;				// コントロールID
	CString		name;			// 監視名
	UINT		type;			// 0:文字列 1:数値 2:小数点 3:TIME
} stMonitorParam;

/*
*******************************************************************************
* 数値型変換定義
*******************************************************************************
*/

struct val_t {	/* 数値型変換テーブル: ソース上"val"に相当 */
	long l_id;	/* データｉｄ: 定数=VAL_ID値 */
	long l_len;	/* データ長  : sizeof( struct val_t ) */
	double d_min;	/* 下限値 */
	double d_max;	/* 上限値 */
	long double ld_min;	/* 下限値 *//* 2013-TURBO */
	long double ld_max;	/* 上限値 *//* 2013-TURBO */
};

struct endofval_t {	/* 数値型変換テーブル終了: ソース上"endofval"に相当 */
	long l_id;	/* データｉｄ: 定数=ENDOFVAL_ID値 */
	long l_len;	/* データ長  : sizeof( struct endofval_t ) */
};

/*
*******************************************************************************
* ステータス型変換定義
*******************************************************************************
*/

struct sts_t {	/* ステータス型変換テーブル: ソース上"sts"に相当 */
	long l_id;	/* データｉｄ: 定数=STS_ID値 */
	long l_len;	/* データ長  : sizeof( struct sts_t ) */
	char sz_sts[CHKVALSTR_LEN];	/* ステータス文字列 */
};

struct endofsts_t {	/* ステータス型変換テーブル終了: ソース上"endofsts"に相当 */
	long l_id;	/* データｉｄ: 定数=ENDOFSTS_ID値 */
	long l_len;	/* データ長  : sizeof( struct endofsts_t ) */
};


/*
*******************************************************************************
* 個々の制御項目に対する設備制御情報の終了を示す（ソース上の"end;"に対応する）
*******************************************************************************
*/
struct endofctl_t {	/* このテーブルは全制御項目について存在する */
	long l_id;	/* データｉｄ: 定数=ENDOFCTL_ID値 */
	long l_len;	/* データ長  : sizeof( struct endofctl_t ) */
};


/*
*******************************************************************************
* 衛星本体情報定義
*******************************************************************************
*/

struct satinf_t {	/* 定義テーブル: ソース上"satinf"に相当 */
	long	l_id;						/* データｉｄ: 定数=SATINF_ID値 */
	long	l_len;						/* データ長  : sizeof( struct satinf_t ) */
	char	sz_satname[SATNAME_LEN];	/* 衛星名 */
	long	l_satno;					/* 衛星番号 */
	long	l_useband;					/* 使用周波数帯（＊Ａ１） */
	long	l_level;					/* 使用帯域（＊Ａ２）  */
	long	l_rfu;						/* パディング */
	double	d_scoh;						/* Ｓ帯コーヒーレントレシオ:（＊Ａ３） */
	double	d_xcoh;						/* Ｘ帯コーヒーレントレシオ:（＊Ａ３） */
	long	l_brchg;					/* 自動ビットレート切り換えの有無（＊Ａ４） */
	long	l_cohmode;					/* コヒーレントモードの有無（＊Ａ５） */
	char	sz_satctl[SATCTLNAME_LEN];	/* 接続衛星管制名 */
};

struct endofsatinf_t {	/* 定義テーブル終了: ソース上"end"に相当 */
	long	l_id;	/* データｉｄ: 定数=ENDOFSAT_ID値 */
	long	l_len;	/* データ長  : sizeof( struct endofsatinf_t ) */
};

/*
＜使用周波数帯＞：：＝使用周波数帯を以下の文字列にて使用する。
"S":Sﾊﾞﾝﾄﾞのみ,"X":Xﾊﾞﾝﾄﾞのみ,
"S/X":SﾊﾞﾝﾄﾞとXﾊﾞﾝﾄﾞ両方
*/
#define USEBAND_S_STR	"S"	/* 「Sﾊﾞﾝﾄﾞのみ」指定文字列 */
#define USEBAND_X_STR	"X"	/* 「Xﾊﾞﾝﾄﾞのみ」指定文字列 */
#define USEBAND_SX_STR	"S/X"	/* 「SﾊﾞﾝﾄﾞとXﾊﾞﾝﾄﾞ両方」指定文字列 */

#define USEBAND_S_ID	1	/* 「Sﾊﾞﾝﾄﾞのみ」指定ＩＤ値 */
#define USEBAND_X_ID	2	/* 「Xﾊﾞﾝﾄﾞのみ」指定ＩＤ値 */
#define USEBAND_SX_ID	3	/* 「SﾊﾞﾝﾄﾞとXﾊﾞﾝﾄﾞ両方」指定ＩＤ値 */
#define USEBAND_XX_ID	4	/* 「X/Xﾊﾞﾝﾄﾞのみ」指定ＩＤ値 XUP 荒谷 00/12/05 */

/*
＜使用帯域＞：：＝使用帯域を､以下の文字列にて指定する。
"STDN"もしくは"DSN"
*/
#define USE_STDN_STR	"STDN"	/* 「近地球衛星」指定文字列 */
#define USE_DSN_STR		"DSN"	/* 「深宇宙衛星」指定文字列 */

#define USE_STDN_ID	1	/* "STDN"指定ＩＤ値 */
#define USE_DSN_ID	2	/* "DSN"指定ＩＤ値 */


/*
＜自動ビットレート切換えの有無＞
：：＝自動ビットレート切り換えを行う場合は"Y"を設定する。
自動ビットレート切換えを行わない場合は"N"を設定する。
*/
#define BRCHG_Y_STR	"Y"	/* "Y"指定文字列 */
#define BRCHG_N_STR	"N"	/* "N"指定文字列 */

#define BRCHG_Y_ID	1	/* "Y"指定ＩＤ値 */
#define BRCHG_N_ID	2	/* "N"指定ＩＤ値 */


/*
＜コヒーレントモードの有無＞
：：＝コヒーレントモードがある場合は、"Y"を設定する。
コヒーレントモードがない場合は、"N"を設定する。
*/
#define COH_Y_STR	"Y"	/* "Y"指定文字列 */
#define COH_N_STR	"N"	/* "N"指定文字列 */

#define COH_Y_ID	1	/* "Y"指定ＩＤ値 */
#define COH_N_ID	2	/* "N"指定ＩＤ値 */

/*
*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
* ［衛星ＥＩＲＰ情報］送信機出力情報定義
*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
*/

struct txpwr_t {	/* 送信機出力情報定義テーブル: ソース上"txpwr"に相当 */
	long l_id;	/* データｉｄ: 定数=TXPWR_ID値 */
	long l_len;	/* データ長  : sizeof( struct txpwr_t ) */
	char sz_txpwr[TXPWRSTR_LEN];	/* 送信機出力識別 */
};

struct endoftxpwr_t {	/* 送信機出力情報定義テーブル終了: ソース上"endoftxpwr"に相当 */
	long l_id;	/* データｉｄ: 定数=ENDOFTXPWR_ID値 */
	long l_len;	/* データ長  : sizeof( struct endoftxpwr_t ) */
};

/*
*******************************************************************************
* 予報値ファイル情報定義
*******************************************************************************
*/

struct prd_t {	/* 予報値ファイル情報定義テーブル: ソース上"prd"に相当 */
	long l_id;	/* データｉｄ: 定数=PRD_ID値 */
	long l_len;	/* データ長  : sizeof( struct prd_t ) */
	char sz_prdpath[PRDPATH_LEN];	/* 予報値ファイル格納ディレクトリ名 */
};


/*
*******************************************************************************
* 計画ファイル情報定義
*******************************************************************************
*/

struct plan_t {	/* 計画ファイル情報定義テーブル: ソース上"plan"に相当 */
	long l_id;	/* データｉｄ: 定数=PLAN_ID値 */
	long l_len;	/* データ長  : sizeof( struct plan_t ) */
	char sz_planpath[PLANPATH_LEN];	/* 計画ファイル格納ディレクトリ名 */
};

/*
*******************************************************************************
* Ｘ帯送信設備使用有無情報定義
*******************************************************************************
*/
struct xtx_t {	/* Ｘ帯送信設備使用有無情報定義テーブル: ソース上"xtx"に相当 */
	long l_id;	 /* データｉｄ: 定数=XTX_ID値 */
	long l_len;	 /* データ長  : sizeof( struct xtx_t ) */
	long l_x_tx; /* Ｘ帯送信設備使用有無:XTX_Y_ID .or. XTX_N_ID */
	long l_rfu;  /* パディング */
};

#define XTX_Y_STR	"Y"	/* "Y"指定文字列 */
#define XTX_N_STR	"N"	/* "N"指定文字列 */

#define XTX_Y_ID	1	/* "Y"指定ＩＤ値 */
#define XTX_N_ID	2	/* "N"指定ＩＤ値 */

/*
*******************************************************************************
* Ｘ帯受信設備対象情報定義
*******************************************************************************
*/
struct xrxsel_t {	/* Ｘ帯受信設備対象情報定義テーブル: ソース上"xrxsel"に相当 */
	long l_id;	     /* データｉｄ: 定数=XRXSEL_ID値 */
	long l_len;	     /* データ長  : sizeof( struct xrxsel_t ) */
	long l_x_rx_sel; /* 対象受信設備:XRXSEL_RX1_ID .or. XRXSEL_RX2_ID*/
	long l_rfu;	     /* パディング */
};

#define XRXSEL_RX1_STR	"RX1"	/* 「RX1」指定文字列 */
#define XRXSEL_RX2_STR	"RX2"	/* 「RX2」指定文字列 */

#define XRXSEL_RX1_ID	1	/* 「RX1」指定ＩＤ値 */
#define XRXSEL_RX2_ID	2	/* 「RX2」指定ＩＤ値 */

/*
*******************************************************************************
* 周波数ランプ制御許可情報定義
*******************************************************************************
*/
struct rampcnt_t {    /* 周波数ランプ制御許可情報定義テーブル: ソース上"rampcnt"に相当 */
	long l_id;       /* データｉｄ: 定数=RAMPCNT_ID値 */
	long l_len;       /* データ長  : sizeof( struct rampcnt_t ) */
	long l_s_tx;      /* Ｓ帯送信機周波数ランプ制御  有無（＊Ａ１） */
	long l_x_tx;      /* Ｘ帯送信機周波数ランプ制御 有無（＊Ａ１） */
	long l_s_rx;      /* Ｓ帯受信機周波数ランプ制御  有無（＊Ａ１） */
	long l_x_rx;      /* 鹿児島Ｘ帯受信機周波数ランプ制御  有無（＊Ａ１） */
	long l_x_rx1;     /* 臼田Ｘ帯受信機１周波数ランプ制御  有無（＊Ａ１） */
	long l_x_rx2;     /* 臼田Ｘ帯受信機２周波数ランプ制御  有無（＊Ａ１） */
	long l_sx_acqrx;  /* 鹿児島ＳＸ帯捕捉受信機周波数ランプ制御　有無（＊Ａ１） */
	long l_x_qpskrx;  /* Ｘ帯高速受信機周波数ランプ制御  有無（＊Ａ１） */
};

/* 注記
＊Ａ１：ソース上文字列で表されている形式をID値に変換して代入する。
*/

#define RAMPCNT_Y_STR    "Y"    /* "Y"指定文字列 */
#define RAMPCNT_N_STR    "N"    /* "N"指定文字列 */

#define RAMPCNT_Y_ID    1    /* "Y"指定ＩＤ値 */
#define RAMPCNT_N_ID    2    /* "N"指定ＩＤ値 */

/*
*******************************************************************************
* 天頂通過監視角度情報定義
*******************************************************************************
*/
struct topangl_t {	/* 天頂通過監視角度情報定義テーブル: ソース上"topangl"に相当 */
	long l_id;	/* データｉｄ: 定数=TOPANGL_ID値 */
	long l_len;	/* データ長  : sizeof( struct topangl_t ) */
	long l_in;  /* 天頂通過入角(deg)値 */
	long l_out; /* 天頂通過出角(deg)値 */
};

/*
*******************************************************************************
* 衛星捕捉情報定義
*******************************************************************************
*/

struct catch_t {	/* 衛星補足情報定義テーブル: ソース上"catch;...;.."に相当 */
	long l_id;	/* データｉｄ: 定数=CATCH_ID値 */
	long l_len;	/* データ長  : sizeof( struct catch_t ) */

	long   l_route;					/* 捕捉ルート番号               （Ａ１） */
	long   l_s3way;					/* Ｓ帯３ＷＡＹの有無           （Ａ１） */
	long   l_x3way;					/* Ｘ帯３ＷＡＹの有無           （Ａ１） */
	long   l_s_txsweep;				/* Ｓ帯送信機ＳＷＥＥＰ有無     （Ａ１） */
	long   l_x_txsweep;				/* Ｘ帯送信機ＳＷＥＥＰ有無     （Ａ１） */
	long   l_cmdrng;				/* ＣＭＤ、ＲＮＧ同時変調の有無 （Ａ１） */
	double d_s_rx_az_errv;			/* S-RX AZ      許容誤差電圧値           */
	double d_s_rx_el_errv;			/* S-RX EL      許容誤差電圧値           */
	long   l_s_rx_a_lockon;			/* S-RX Ach     LOCK-ON 判定回数         */
	long   l_s_rx_a_lockoff;		/* S-RX Ach     LOCK-OFF判定回数         */
	long   l_s_rx_b_lockon;			/* S-RX Bch     LOCK-ON 判定回数         */
	long   l_s_rx_b_lockoff;		/* S-RX Bch     LOCK-OFF判定回数         */
	double d_x_rx_az_errv;			/* X-RX AZ      許容誤差電圧値           */
	double d_x_rx_el_errv;			/* X-RX EL      許容誤差電圧値           */
	long   l_x_rx_a_lockon;			/* X-RX Ach     LOCK-ON 判定回数         */
	long   l_x_rx_a_lockoff;		/* X-RX Ach     LOCK-OFF判定回数         */
	long   l_x_rx_b_lockon;			/* X-RX Bch     LOCK-ON 判定回数         */
	long   l_x_rx_b_lockoff;		/* X-RX Bch     LOCK-OFF判定回数         */
	double d_sx_acqrx_az_errv;		/* SX-ACQRX AZ  許容誤差電圧値           */
	double d_sx_acqrx_el_errv;		/* SX-ACQRX EL  許容誤差電圧値           */
	long   l_sx_acqrx_a_lockon;		/* SX-ACQRX Ach LOCK-ON 判定回数         */
	long   l_sx_acqrx_a_lockoff;	/* SX-ACQRX Ach LOCK-OFF判定回数         */
	long   l_sx_acqrx_b_lockon;		/* SX-ACQRX Bch LOCK-ON 判定回数         */
	long   l_sx_acqrx_b_lockoff;	/* SX-ACQRX Bch LOCK-OFF判定回数         */
	double d_x_qpskrx_az_errv;		/* X-QPSKRX AZ  許容誤差電圧値           */
	double d_x_qpskrx_el_errv;		/* X-QPSKRX EL  許容誤差電圧値           */
	long   l_x_qpskrx_a_lockon;		/* X-QPSKRX Ach LOCK-ON 判定回数         */
	long   l_x_qpskrx_a_lockoff;	/* X-QPSKRX Ach LOCK-OFF判定回数         */
	long   l_x_qpskrx_b_lockon;		/* X-QPSKRX Bch LOCK-ON 判定回数         */
	long   l_x_qpskrx_b_lockoff;	/* X-QPSKRX Bch LOCK-OFF判定回数         */

	/*2000-11-22拡張部:上位コンパチになるように以下に追加*/
	double d_x_rx1_az_errv;			/* X-RX1 AZ      許容誤差電圧値          */
	double d_x_rx1_el_errv;			/* X-RX1 EL      許容誤差電圧値          */
	long   l_x_rx1_a_lockon;		/* X-RX1 Ach     LOCK-ON 判定回数        */
	long   l_x_rx1_a_lockoff;		/* X-RX1 Ach     LOCK-OFF判定回数        */
	long   l_x_rx1_b_lockon;		/* X-RX1 Bch     LOCK-ON 判定回数        */
	long   l_x_rx1_b_lockoff;		/* X-RX1 Bch     LOCK-OFF判定回数        */
	double d_x_rx2_az_errv;			/* X-RX2 AZ      許容誤差電圧値          */
	double d_x_rx2_el_errv;			/* X-RX2 EL      許容誤差電圧値          */
	long   l_x_rx2_a_lockon;		/* X-RX2 Ach     LOCK-ON 判定回数        */
	long   l_x_rx2_a_lockoff;		/* X-RX2 Ach     LOCK-OFF判定回数        */
	long   l_x_rx2_b_lockon;		/* X-RX2 Bch     LOCK-ON 判定回数        */
	long   l_x_rx2_b_lockoff;		/* X-RX2 Bch     LOCK-OFF判定回数        */
};
/* 注記
＊Ａ１：ソース上文字列で表されている形式をID値に変換して代入する。
*/

/*
＜Ｓ帯３ＷＡＹの有無          ＞
＜Ｘ帯３ＷＡＹの有無          ＞
＜Ｓ帯送信機ＳＷＥＥＰ有無    ＞
＜Ｘ帯送信機ＳＷＥＥＰ有無    ＞
＜ＣＭＤ、ＲＮＧ同時変調の有無＞
：：＝有りの場合は、"Y"を設定する。
無しの場合は、"N"を設定する。
*/
#define CATCH_Y_STR	"Y"	/* "Y"指定文字列 */
#define CATCH_N_STR	"N"	/* "N"指定文字列 */

#define CATCH_Y_ID	1	/* "Y"指定ＩＤ値 */
#define CATCH_N_ID	2	/* "N"指定ＩＤ値 */


/*
*******************************************************************************
* ＡＧＣ校正対象受信機情報定義
*******************************************************************************
*/

struct agcrx_t {	/* ＡＧＣ校正対象受信機情報定義テーブル: ソース上"agcrx"に相当 */
	long l_id;			/* データｉｄ: 定数=AGCRX_ID値 */
	long l_len;			/* データ長  : sizeof( struct agcrx_t ) */
	long l_s_rx;		/* Ｓ帯主受信設備  有無（＊Ａ１） */
	long l_x_rx;		/* Ｘ帯主受信設備  有無（＊Ａ１） */
	long l_sx_acqrx_s;	/* ＳＸ帯補足用受信設備　Ｓ帯　有無（＊Ａ１） */
	long l_sx_acqrx_x;	/* ＳＸ帯補足用受信設備　Ｘ帯　有無（＊Ａ１） */
	long l_x_qpskrx;	/* Ｘ帯高速データ用受信設備  有無（＊Ａ１） */

	/*2000-11-22拡張部:テーブル構造が上位コンパチになるように以下に追加*/
	long l_x_rx1;		/* 臼田Ｘ帯受信機１  有無（＊Ａ１） */
	long l_x_rx2;		/* 臼田Ｘ帯受信機２  有無（＊Ａ１） */

	long l_rfu;			/* パディング */
};

struct agcrx54_t {	/* ＡＧＣ校正対象受信機情報定義テーブル: ソース上"agcrx"に相当 */
	long l_id;			/* データｉｄ: 定数=AGCRX_ID値 */
	long l_len;			/* データ長  : sizeof( struct agcrx54_t ) */
	long l_s_rx;		/* Ｓ帯主受信設備  有無（＊Ａ１） */
	long l_x_rx;		/* Ｘ帯主受信設備  有無（＊Ａ１） */
	long l_sx_acqrx_s;	/* ＳＸ帯補足用受信設備　Ｓ帯　有無（＊Ａ１） */
	long l_sx_acqrx_x;	/* ＳＸ帯補足用受信設備　Ｘ帯　有無（＊Ａ１） */
	long l_x_qpskrx;	/* Ｘ帯高速データ用受信設備  有無（＊Ａ１） */
	long l_x_rx1;		/* 臼田Ｘ帯受信機１  有無（＊Ａ１） */
	long l_x_rx2;		/* 臼田Ｘ帯受信機２  有無（＊Ａ１） */
	long l_x_dsntcr1;	/* X帯DSN-MODEM1  有無（＊Ａ１） */
	long l_ka_dsntcr1;	/* Ka帯DSN-MODEM1  有無（＊Ａ１） */
	long l_rfu;			/* パディング */
};

/* 注記
＊Ａ１：ソース上文字列で表されている形式をID値に変換して代入する。
*/

/*
＜コヒーレントモードの有無＞
＜Ｓ帯主受信設備  有無＞
　＜Ｘ帯主受信設備  有無＞
 　＜ＳＸ帯補足用受信設備　Ｓ帯　有無＞
  　＜ＳＸ帯補足用受信設備　Ｘ帯　有無＞
   　＜Ｘ帯高速データ用受信設備  有無＞
	：：＝ＡＧＣ校正を行う場合は、"Y"を設定する。
	ＡＧＣ校正を行わない場合は、"N"を設定する。
	*/
#define AGCRX_Y_STR	"Y"	/* "Y"指定文字列 */
#define AGCRX_N_STR	"N"	/* "N"指定文字列 */

#define AGCRX_Y_ID	1	/* "Y"指定ＩＤ値 */
#define AGCRX_N_ID	2	/* "N"指定ＩＤ値 */


/*
*******************************************************************************
* ノミナル受信周波数情報定義
*******************************************************************************
*/

struct rxfq_t {	/* ノミナル受信周波数情報定義テーブル: ソース上"rxfq"に相当 */
	long l_id;	/* データｉｄ: 定数=RXFQ_ID値 */
	long l_len;	/* データ長  : sizeof( struct rxfq_t ) */
	long l_valnum;	/* 設定値数（＊Ｃ１） */
	long l_rfu;	/* パディング */
};

struct rxfqval_t {	/* ノミナル受信周波数設定値定義テーブル:（＊Ｃ２）  */
	long l_id;	/* データｉｄ: 定数=RXFQVAL_ID値 */
	long l_len;	/* データ長  : sizeof( struct rxfqval_t ) */
	long l_no;	/* 設定値番号（＊Ｃ３） */
	long l_band;	/* バンド指定（＊Ｃ４） */
	double d_fq;	/* 受信周波数値[Hz] */
};

struct endofrxfq_t {	/* ノミナル受信周波数情報定義テーブル終了: ソース上rxfq行の";"に相当 */
	long l_id;	/* データｉｄ: 定数=ENDOFRXFQ_ID値 */
	long l_len;	/* データ長  : sizeof( struct endofrxfq_t ) */
};

/* 注記
＊Ｃ１：設定された受信周波数の数(次にくるrxfqval_tの数)
->ソース上"rxfq"を識別としてub_getln()により得られる。
＊Ｃ２：本項目は、ソース上"rxfq arg1,arg2...,argN;"と記述される
"arg1,...,argN"に相当する。（アーギュメント数分確保する。）
＊Ｃ３：本項目は、ソース上"rxfq arg1,arg2...,argN;"と記述される
アーギュメントに対し、本設定値が第何argに相当するかを示す。（１～）
＊Ｃ４：ソース上文字列で表されている形式をID値に変換して代入する。
*/

/*
＜受信周波数＞：：＝衛星に対する、対応バンドのノミナル受信周波数[Hz]を
「ﾊﾞﾝﾄﾞ指定=受信周波数」の形式で指定する。
バンド指定方法:'S'-Sバンド,'X'-Xバンド
*/

#define RXBAND_S_ID		1		/* 「Sﾊﾞﾝﾄﾞ」指定ＩＤ値 */
#define RXBAND_X_ID		2		/* 「Xﾊﾞﾝﾄﾞ」指定ＩＤ値 */
#define RXBAND_KA_ID	3		/* 「Kaﾊﾞﾝﾄﾞ」指定ＩＤ値 */


/*
*******************************************************************************
* ＲＡＲＲ使用帯域情報
*******************************************************************************
*/
struct rarrband_t {	/* ＲＡＲＲ使用帯域情報定義テーブル: ソース上"rarrband"に相当 */
	long l_id;	     /* データｉｄ: 定数=RARRBAND_ID値 */
	long l_len;	     /* データ長  : sizeof( struct rarrband_t ) */
	long l_rarr_band; /* 使用帯域:*/
	long l_rfu;	      /* パディング */
};

#define RARRBAND_S_STR		"S"    /* 「S」指定文字列 */
#define RARRBAND_X_STR		"X"	   /* 「X」指定文字列 */
#define RARRBAND_SX_STR		"S/X"  /* 「S/X」指定文字列 */
#define RARRBAND_RX_STR		"RX"   /* 「RX」指定文字列 *//* 2008-01-31 再生型測距対応　追加 */
#define RARRBAND_XKA_STR	"XKa"  /* 「XKa」指定文字列 */
#define RARRBAND_RXKA_STR	"RXKa" /* 「RXKa」指定文字列 */

#define RARRBAND_S_ID		1	/* 「S」指定ＩＤ値    */
#define RARRBAND_X_ID		2	/* 「X」指定ＩＤ値    */
#define RARRBAND_SX_ID		3	/* 「S/X」指定ＩＤ値  */
#define RARRBAND_RX_ID		4	/* 「RX」指定ＩＤ値   *//* 2008-01-31 再生型測距対応　追加 */
#define RARRBAND_XKA_ID		5	/* 「X/X」「X/Ka」 指定ＩＤ値 */
#define RARRBAND_RXKA_ID	6	/* 「X/X」「X/X再生」「X/Ka」「X/Ka再生」指定ＩＤ値 */


#define ERR_RETURN -1


/*
*******************************************************************************
* 設備監視情報 識別文字列マクロ定義
*******************************************************************************
*/
#define OBSNAME_STR       "obs"		/* 設備監視名称であることの識別文字列 */
#define IF_STR            "if"		/* 条件定義を意味する識別文字列 */
#define ELSE_STR          "else"	/* 条件定義を意味する識別文字列 */
#define ELSEIF_STR        "elseif"	/* 条件定義を意味する識別文字列 */
#define ENDIF_STR         "endif"	/* 条件定義を意味する識別文字列 */
#define EXT_STR           "ext"		/* バイナリ抽出定義（符号無）を意味する識別文字列 */
#define EXTS_STR          "exts"	/* バイナリ抽出定義（符号有）を意味する識別文字列 */
#define EXTBCD_STR        "extbcd"	/* ＢＣＤ抽出定義（符号無）を意味する識別文字列 */
#define EXTBCDS_STR       "extbcds"	/* ＢＣＤ抽出定義（符号有）を意味する識別文字列 */
#define EXTAVG_STR        "extavg"	/* 抽出データの平均（符号無）を意味する識別文字列 */
#define EXTAVGS_STR       "extavgs"	/* 抽出データの平均（符号有）を意味する識別文字列 */
#define EXTCHAR_STR       "extchar"	/* 文字列抽出データを意味する識別文字列 */
#define EXTCHARDEC_STR    "extchardec"/* 文字列型数値抽出データを意味する識別文字列 */
#define EXTDBL_STR        "extdbl"	/* double抽出データを意味する識別文字列 */
#define EXTFLT_STR        "extflt"	/* float抽出データを意味する識別文字列 */
#define CATDATA_STR       "catdata"	/* 他の監視データを連結する識別文字列 */
#define POL_STR           "pol"		/* 方程式変換定義を意味する識別文字列 */
#define CARVTBL_STR       "carvtbl"	/* カーブ変換定義を意味する識別文字列 */
#define CAL_STR           "cal"	        /* 他項目計算定義を意味する識別文字列 */
#define TIMECVT_STR       "timecvt"	/* 時刻変換定義を意味する識別文字列 */
#define STS_STR           "sts"		/* ステータス定義を意味する識別文字列 */

#define EQMON_INF_STR     "inf"		/* 付加情報定義を意味する識別文字列:2006-08-26 */

#define ENDOFCTL_STR      "end"		/* 個々のコマンドの定義情報の終了を現す文字列 */


#define	OBSNAME_ID		0x0101		/* 条件定義ＩＤ */
#define	IF_ID			0x0102		/* 条件定義ＩＤ */
#define	ELSE_ID			0x010d		/* 条件定義ＩＤ */
#define	ELSEIF_ID		0x010e		/* 条件定義ＩＤ */
#define	ENDIF_ID		0x010f		/* 条件定義ＩＤ */
#define	EXT_ID			0x0103		/* バイナリ抽出（符号無し）ＩＤ */
#define	EXTS_ID			0x0104		/* バイナリ抽出（符号有り）ＩＤ */
#define	EXTBCD_ID		0x0105		/* ＢＣＤ抽出（符号無し）ＩＤ */
#define	EXTBCDS_ID		0x0106		/* ＢＣＤ抽出（符号有り）ＩＤ */
#define	EXTAVG_ID		0x0107		/* 抽出データ平均（符号無し）ＩＤ */
#define	EXTAVGS_ID		0x0108		/* 抽出データ平均（符号有り）ＩＤ */
#define	EXTCHAR_ID		0x0110		/* 文字列抽出ＩＤ */
#define	EXTCHARDEC_ID	0x0113		/* 文字列型数値抽出ＩＤ */
#define	EXTDBL_ID		0x0111		/* double抽出ＩＤ */
#define	EXTFLT_ID		0x0114		/* float抽出ＩＤ */
#define POL_ID			0x0109		/* 方程式変換定義ＩＤ */
/*#define CATDATA_ID		0x0112		*//* 他の監視データを連結するＩＤ */
#define CARVTBL_ID		0x010a		/* カーブ変換定義ＩＤ */
#define TIMECVT_ID		0x010b		/* 時刻変換定義ＩＤ */
#define OBSSTS_ID		0x010c		/* ステータス定義ＩＤ */
#define CAL_ID			0x0112		/* 他項目計算定義ＩＤ */

#define	EQMON_INF_ID	0x0121		/* "inf":付加情報定義を意味するＩＤ */

#define ENDOFOBS_ID 	-1			/* 個々のコマンドの定義の終了ＩＤ */
#define L_OFFSET        6
#define CARV_MAX        32          /* カーブテーブルの最大数 */


class CDBAccess
{
public:
	CDBAccess();
	virtual ~CDBAccess();

public:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	// 衛星固有情報DB
	char* m_eqsatdb_data;
	vector<CString> m_satnamelist;
	map<CString, __int64> m_satadrlist;

	// 設備制御DB
	char* m_eqctldb_data;
	vector<CString> m_ctlnamelist;
	map<CString, __int64> m_ctladrlist;

	// 監視データ項目リスト
	char* m_eqmondb_data;
	vector<CString> m_monnamelist;

	// 監視データ（受信監視データ）
	map<UINT, map<CString, CEngValCnv::obsname_t>> m_mondatalist;
//	vector<CEngValCnv::obsname_t> m_mondatalist[4];
//	CEngValCnv::obsname_t	mObsName[4][sizeof(CEngValCnv::obsname_t)];
//	deque<char*> mObList;

	// レディネスチェック時間
	map<CString, int> m_rdychktimelist;
	// 計画時間
	map<CString, int> m_plntimelist;

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	int eqsat_db_read();		// 衛星固有情報DB取得
	int eqctl_db_read();		// 設備制御DB取得
	int eqmon_db_read();		// 設備制御DB取得
	int rdychk_read();			// レディネスチェック時間取得
	int plntime_read();			// 計画時間取得

	int db_read(char* db_file_nam, char *cp_loadadr_p);

	int ud_adr_to_sattbl(vector<CString>& snamelist);
	int ud_adr_to_ctltbl(vector<CString>& list);
	int ud_adr_to_montbl(vector<CString>& list);

	int ud_adr_to_resvec(char* base_p, long l_id, long l_limitid, long l_endoftblid, char* vec[], int i_argc);
	int ud_macname_to_ststbl(char *base_p, char *psz_macname, char *tbl[], char *vec[], int i_argc);
	int ud_macadr_to_ststbl(char *base_p, char *tbl[], char *vec[], int i_argc);
	int ud_macstsadr_to_exectbl(char *base_p, char *tbl[], char *vec[], int i_argc);
	int ud_macsts_to_exectbl(char *base_p, char *psz_param, char *tbl[], char *vec[], int i_argc);

	vector<CString>&		GetSatNameList() { return m_satnamelist; }
	map<CString, __int64>&	GetSatAdrList() { return m_satadrlist; }

	vector<CString>&		GetCtlNameList() { return m_ctlnamelist; }
	map<CString, __int64>&	GetCtlAdrList() { return m_ctladrlist; }

	vector<CString>&		GetMonNameList() { return m_monnamelist; }

	void SetEqMonData(UINT nStation, vector<CEngValCnv::obs_tbl_res_t>& data);
	char* GetObsname_t(UINT nStation, CString strWatch);

	int GetIdxSatelliteEqSatDB(CString str);

	map<CString, int>& GetRdyChkTimeList() { return m_rdychktimelist; }
	map<CString, int>& GetPlnTimeList() { return m_plntimelist; }

	int GetRdyChkTimeFile(CString str);
	int GetPlnTimeFile(CString str);

#ifdef MON_UT
	map<CString, CEngValCnv::obsname_t> mon_UT;
	void SetMonUT(vector<CEngValCnv::obsname_t>& data);
	map<CString, CEngValCnv::obsname_t> GetMonUT()	{ return mon_UT; }
	CEngValCnv::obsname_t mon_UT_time;
#endif

protected:

};



// セッション情報クラス
class CSessionInfo
{
public:
	CSessionInfo();
	virtual ~CSessionInfo();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum eStatus {
		eUnknown = 0,
		eGettingNow,
		eError,
		eDisconnect,
		eConnect
	};

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

	UINT m_sessionStatus[eSessionType_Num];	// セッション状態

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	UINT GetSessionStatus(UINT pos)					{ return m_sessionStatus[pos]; }
	void SetSessionStatus(UINT pos, UINT eStatus)	
	{ 
		if (pos < eSessionType_Num)
		{
			m_sessionStatus[pos] = eStatus;
		}
		else
		{
			for (int i = 0; i < eSessionType_Num; i++)
			{
				m_sessionStatus[i] = eStatus;
			}
		}
	}
//	void UpdateSessionStatus();
	void ReadStatusFile();
	int GetSessionIdx(CString& str);
	void SetSessionStatus(CString& strSession, CString& strStatus);
};

// トレンドデータクラス
class CTrendData
{
public:
	CTrendData();
	virtual ~CTrendData();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	/*  予報値テーブル構造体 */

	typedef struct {
		CString strSigName;			// 信号名
		time_t	t;					// 時刻
		double	value;				// 値
	} structForecastValue;

	typedef struct	{
		unsigned char  uca_time[PRED_TIME_LEN];             /* 時刻(BCD:YYYYMMDDHHMMSS)     */
		double d_predict_az;                                /* 予測方位角                   */
		double d_predict_el;                                /* 予測仰角                     */
		double d_predict_uplink_range;                      /* 予測アップリンクレンジ       */
		double d_predict_downlink_range;                    /* 予測ダウンリンクレンジ       */
		double d_predict_uplink_range_rate;                 /* 予測アップリンクレンジレート */
		double d_predict_downlink_range_rate;               /* 予測ダウンリンクレンジレート */
		double d_predict_2way_rng;                          /* 予測２ＷＡＹレンジ           *//* <<2013-TURBO>> 追加 */
		double d_predict_2way_range_rate;                   /* 予測２ＷＡＹレンジレート     *//* <<2013-TURBO>> 追加 */
		double d_predict_3way_range_rate;                   /* 予測３ＷＡＹレンジレート     *//* 2017/12/16 追加 */
		char   ca_predict_s_tx_freq[PRED_FREQ_LEN];         /* 予測送信周波数               */
		char   ca_predict_s_rx_1way_freq[PRED_FREQ_LEN];    /* Ｓ帯１ＷＡＹ予測受信周波数   */
		char   ca_predict_x_rx_1way_freq[PRED_FREQ_LEN];    /* Ｘ帯１ＷＡＹ予測受信周波数   */
		char   ca_predict_s_rx_2way_freq[PRED_FREQ_LEN];    /* Ｓ帯２ＷＡＹ予測受信周波数   */
		char   ca_predict_x_rx_2way_freq[PRED_FREQ_LEN];    /* Ｘ帯２ＷＡＹ予測受信周波数   */
		char   ca_predict_s_rx_1way_ramprate[PRED_RAMP_LEN];/* Ｓ帯１ＷＡＹランプレート     */
		char   ca_predict_x_rx_1way_ramprate[PRED_RAMP_LEN];/* Ｘ帯１ＷＡＹランプレート     */
		char   ca_predict_s_rx_2way_ramprate[PRED_RAMP_LEN];/* Ｓ帯２ＷＡＹランプレート     */
		char   ca_predict_x_rx_2way_ramprate[PRED_RAMP_LEN];/* Ｘ帯２ＷＡＹランプレート     */
		char   ca_dummy[PRED_DUMY_LEN];              /* ８バイト境界合わせ用ダミー          */
	} one_predata_t;

	typedef struct	{
		one_predata_t	predata[MAX_PRED_SIZE];
	}predata_SHM_t;                                 /* 予報値テーブル構造体                 */

	typedef struct	{
		long	l_id;
		long	l_size;
		one_predata_t	predata[MAX_PRED_SIZE];
	}predata_t;                                     /* 予報値テーブル構造体(参照用)         */

	/*** 装置毎表示データ情報テーブル ***/
	struct   uni_inf_t {
		int      uni_on;                      /* 表示対象有無(1:ON/0:OFF)   */
//		double   d_end_time;                  /* 最終格納データ発生時刻     */
		long     l_end;                       /* 最終格納データ位置［］     */
		double   d_last_time;                 /* 最終描画データ発生時刻     */
		struct   shmdata_t*  	shmdata_adr;    /* 装置毎蓄積データアドレス   */
		unsigned long  		uni_color;      /* グラフ表示色               */
		char     name[50];					/* 装置名（監視項目名）       */
	};

	/*** グラフ表示用設定値テーブル ***/
	struct   grdsp_inf_t {
		int      i_dsp_type;                  /* 初期（再）／トレンド表示   */
		int      i_dsp_mode;                  /* リアル／保存データ表示起動 */
		int      i_dsp_mode2;                 /* 保存データ初期表示／予報値選択表示 */
		int      i_dsp_flg;					/* 表示切り替え（要／不要）   */
		int      i_unyou_mode;				/* 運用状況モード（今回）     */
		int      i_unyou_mode_s;				/* 運用状況モード（前回）     */
		int      i_band;						/* 使用する周波数帯           */
		int	   i_yohouti_umu;				/* 予報値ファイル有無(V2-1)   */
		int      i_yohouti_get;               /* 予報値ファイル取得状況(V2-5) */
		int      i_mondat_NGcnt;              /* 制御用監視データ無効カウンタ(98.08.25) */

		/**** 今回の状況 (V2-2) *******/
		char     ca_eisei[100];               /* 衛星名					  */
		char     ca_fname[1024];	         /* 予報値ファイルパス名       */
		char     ca_ftime[30];                /* 予報値ファイル更新日時     */
		char     ca_stime[30];                /* パス開始時刻               */
		/**** 前回の状況 (V2-2) *******/
		char     ca_eisei_s[100];             /* 衛星名					  */
		char     ca_fname_s[1024];       /* 予報値ファイルパス名       */
		char     ca_ftime_s[30];              /* 予報値ファイル更新日時     */
		char     ca_stime_s[30];              /* パス開始時刻               */

		/** 保存データ表示起動時の対象 **/
		char     ca_eisei_def[100];           /* 衛星名		              */
		char     ca_fname_def[1024];     /* 予報値ファイルパス名       */

		/**** 運用中パスの表示対象 ****/
		double   d_AOS_time;                  /* 予報値（ＡＯＳ時刻）       */
		double   d_LOS_time;                  /* 予報値（ＬＯＳ時刻）       */
		int      i_OFF_timeSig;               /* 予報値（時刻オフセット符号）*/
		double   d_OFF_time;                  /* 予報値（時刻オフセット値） */
		double   d_OFF_freq;                  /* 予報値（周波数オフセット値）*/
		double   d_Xstattime;                 /* 横軸（開始時刻）           */
		double   d_Xstoptime;                 /* 横軸（終了時刻）           */
		double   d_Xtime;                     /* 横軸（表示幅：時間）       */
		double   d_RNG_Ymin;                  /* ＲＮＧ縦軸（最小値）       */
		double   d_RNG_Ymax;                  /* ＲＮＧ縦軸（最大値）       */
		double   d_RNG_Yval;                  /* ＲＮＧ縦軸（表示幅：値）   */
		double   d_RR_Ymin;                   /* ＲＲ縦軸（最小値）         */
		double   d_RR_Ymax;                   /* ＲＲ縦軸（最大値）         */
		double   d_RR_Yval;                   /* ＲＲ縦軸（表示幅：値）     */
		int      i_YRNG;						/* ＲＮＧグラフ（Ｙ座標）下限 */
		int      i_YRR;						/* ＲＲグラフ  （Ｙ座標）下限 */

		//	unsigned long  pixmap_color;			/* ピックスマップ背景色       */
		//	unsigned long  grph_color;			/* グラフ領域背景色           */
		//	unsigned long  frem_color;            /* グラフ枠表示色             */
		//	unsigned long  grid_color;            /* グラフ罫線表示色           */
		//	unsigned long  strtit_color;          /* タイトル文字表示色         */
		//	unsigned long  strcon_color;          /* 固定項目文字表示色         */
		//	unsigned long  strvar_color;          /* 可変項目文字表示色         */
		//	unsigned long  over_color;			/* グラフ描画色(範囲オーバー) */
		//	unsigned long  yohouti_color;			/* グラフ描画色(予報値) 	  */

		//	int	   i_non_pid_num;				/* 保存データ起動中プロセス数 */
		//	int	   i_pid[NON_PID_NUM];			/* 保存データ表示プロセスPID  */

		int      end_id;                      /* 最新データ装置[位置]       */
		struct   uni_inf_t  uni_inf[GRNUM_RNG_MAX]; /* 装置毎表示データ情報 */
	};

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	// 予報値テーブル構造体ポインタ
	predata_t	m_gst_predata;
	struct grdsp_inf_t   m_gst_dspinf;    /* グラフ表示制御情報テーブル     */
	long	m_gla_prd_cont[2];				/* 予報値格納件数             */
	time_t	m_glla_time[2][86400];			/* 予報値時刻                 */
	double	m_gda_dwRNG[2][86400];			/* ダウンリンクレンジ         */
	double	m_gda_dwRR[2][86400];			/* ダウンリンクレンジレート   */

	// 予報値
	time_t	m_lla_time[MAX_PRED_SIZE];						/* 時刻(UNIX時刻：通算秒)		*/
	double	m_da_predict_az[MAX_PRED_SIZE];					/* 予測方位角					*/
	double	m_da_predict_el[MAX_PRED_SIZE];					/* 予測仰角						*/
	double	m_da_predict_uplink_range[MAX_PRED_SIZE];		/* 予測アップリンクレンジ		*/
	double	m_da_predict_downlink_range[MAX_PRED_SIZE];		/* 予測ダウンリンクレンジ		*/
	double	m_da_predict_uplink_range_rate[MAX_PRED_SIZE];	/* 予測アップリンクレンジレート	*/
	double	m_da_predict_downlink_range_rate[MAX_PRED_SIZE];	/* 予測ダウンリンクレンジレート	*/
	double	m_da_predict_2way_rng[MAX_PRED_SIZE];	        /* 予測２ＷＡＹダウンリンクレンジ		*//* <<2013-TURBO>> */
	double	m_da_predict_2way_range_rate[MAX_PRED_SIZE];	/* 予測２ＷＡＹダウンリンクレンジレート	*//* <<2013-TURBO>> */
	double	m_da_predict_s_tx_freq[MAX_PRED_SIZE];			/* 予測送信周波数				*/
	double	m_da_predict_s_rx_1way_freq[MAX_PRED_SIZE];		/* Ｓ帯１ＷＡＹ予測受信周波数	*/
	double	m_da_predict_x_rx_1way_freq[MAX_PRED_SIZE];		/* Ｘ帯１ＷＡＹ予測受信周波数	*/
	double	m_da_predict_s_rx_2way_freq[MAX_PRED_SIZE];		/* Ｓ帯２ＷＡＹ予測受信周波数	*/
	double	m_da_predict_x_rx_2way_freq[MAX_PRED_SIZE];		/* Ｘ帯２ＷＡＹ予測受信周波数	*/
	double	m_da_predict_s_rx_1way_ramprate[MAX_PRED_SIZE];	/* Ｓ帯１ＷＡＹランプレート		*/
	double	m_da_predict_x_rx_1way_ramprate[MAX_PRED_SIZE];	/* Ｘ帯１ＷＡＹランプレート		*/
	double	m_da_predict_s_rx_2way_ramprate[MAX_PRED_SIZE];	/* Ｓ帯２ＷＡＹランプレート		*/
	double	m_da_predict_x_rx_2way_ramprate[MAX_PRED_SIZE];	/* Ｘ帯２ＷＡＹランプレート		*/

	// 予報値ファイル
	CString		m_yohoFilePath[eStation_MAX];				/* 予報値ファイルパス */
	FILETIME	m_yohoFileTime[eStation_MAX];				/* 予報値ファイル時間 */

	// 予報値、往復距離
	map<time_t, double>		m_2WayRngMap;					

	map<time_t, double>		m_UpDownLinkRangeMap;
	map<time_t, double>		m_UpDownLinkRateMap;
	long m_PredItemCount;

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	// 予報値ファイル情報取得ルーチン
	int gryohouti(int i_get_type, UINT nStation = eStation_MAX);
	int gryohouti2(int i_prot_mode, UINT nStation = eStation_MAX);
	int load_pred_data(char *szp_file_name, double *d_aos, double *d_los, bool* pNewFormat=NULL);
	void asc19_to_bcd7_time(unsigned char* psz_asc19_time, unsigned char* psz_bcd7_time);
	void bcd7_to_asc19_time(unsigned char* psz_bcd7_time, unsigned char* psz_asc19_time);

	double	GetUpDownLinkRange(time_t t, int type);
	map<time_t, double>&	GetUpDownLinkRangeMap() { return m_UpDownLinkRangeMap; }
	double GetLinearInterpolationRange(time_t t1, double dRng1, time_t t2, double dRng2, time_t t);
	double	GetUpDownLinkRate(time_t t, int type);
	map<time_t, double>&	GetUpDownLinkRateMap() { return m_UpDownLinkRateMap; }
	double GetLinearInterpolationRate(time_t t1, double dRng1, time_t t2, double dRng2, time_t t);

//	double lb_rttcal(predata_t* stp_predict, double d_pred_t, int i_cal_flag, double* dp_range, char* psz_offset_time, double d_offset_freq);
//	double lb_rttcal_rr(predata_t* stp_predict, double d_pred_t, int i_cal_flag, double* dp_range_rate, char* psz_offset_time, double d_offset_freq);
//	int lb_prdcal(one_predata_t* st_predict, predata_t* gpst_predict, double d_pred_t, int i_sx, char* szp_time_offset, double d_freq_offset);
//	void lb_ramp0(one_predata_t* st_predict, int i_sx);
//	int lb_prdcal_hit(int i, one_predata_t* st_predict, predata_t* gpst_predict, double d_pred_t, int i_sx, double d_freq_offset);
//	double lb_lagrange(int i_dynamic_counter, int n, double t, time_t i_in_time[], double d_input_parameter[]);
	double InterpolatePrd(double t, __int64* idx, CSpaceData* data);
	double CalcLagrange(int i_dynamic_counter, int n, double t, time_t i_in_time[], double d_input_parameter[]);
	
	void AddLinearInterpolation(time_t t1, double dRng1, time_t t2, double dRng2);

	map<time_t, double>& Get2WayRngMap()	{ return m_2WayRngMap; }
	double GetLinearInterpolation(time_t t1, double dRng1, time_t t2, double dRng2, time_t t);
	double Get2WayRng(time_t t);

	CString GetAnpFileName(UINT nStation)	{ 
		if (nStation < 0 || nStation >= eStation_MAX)
			return _T("");

		CString str = m_yohoFilePath[nStation].Right(m_yohoFilePath[nStation].GetLength() - m_yohoFilePath[nStation].ReverseFind(_T('/')) - 1);
		str = str.Right(str.GetLength() - str.ReverseFind(_T('\\')) - 1);
		return str;
	}
};


// サマリ観測データ名セット
typedef struct {
	CString dataName;	// データ名称
	CString dataGName;	// データ名称(グラフ用名)
} stSummaryDataName;

typedef struct {
	int			wndIdx;						// ウィンドー番号
	TCHAR		wndName[256];				// ウィンドー名称
	LPARAM		wnd;						// CView
	int			nGraphIdx;					// グラフ番号

	bool		isRealTime;					// リアルタイム（true）/ ノンリアルタイム（false）
	CTime		startTime;					// 開始日時
	stVScale	trandVScale;				// 縦軸スケール[0:左、1:右]
	HScale		trandHScale[MAX_HSCALE];	// 横軸スケール[0:トレンドグラフビュー1、1:トレンドグラフビュー2]

	TCHAR		dataName[MAX_TDATA][256];	// データ名称
	TCHAR		dataGName[MAX_TDATA][256];	// データ名称(グラフ用名)
	TCHAR		dataUnit[MAX_TDATA][256];	// 単位
	int			nDecDidits[MAX_TDATA];		// 少数点以下の桁数
	bool		isViewMode[MAX_TDATA];		// 表示On/Off
	DrawMode	drawMode[MAX_TDATA];		// 描画モード（Line、Dots、L&D）
	COLORREF	color[MAX_TDATA];			// 表示色
	int			lineWidth[MAX_TDATA];		// 線幅
	DotSize		dotSize[MAX_TDATA];			// ドットサイズ
	VScaleKind	vscaleKind[MAX_TDATA];		// 縦軸スケール選択（右/左）
	TCHAR		comment[MAX_TDATA][256];	// コメント
	bool		isComment[MAX_TDATA];		// コメント表示
	bool		isAlarm[MAX_TDATA];			// アラーム表示
	double		alarm1Major[MAX_TDATA];		// アラーム1 上限
	double		alarm1Minor[MAX_TDATA];		// アラーム1 下限 
	double		alarm2Major[MAX_TDATA];		// アラーム2 上限
	double		alarm2Minor[MAX_TDATA];		// アラーム2 下限
} stTrendPropiesMemData;


// トレンドプロパティデータクラス
class CPropertiesData
{
public:
	CPropertiesData()	{ m_bSetted = FALSE; };
	virtual ~CPropertiesData(){};

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	int			wndIdx;						// ウィンドー番号
	CString		wndName;					// ウィンドー名称
	LPARAM		wnd;						// CView
	int			nGraphIdx;					// グラフ番号

	bool		isRealTime;					// リアルタイム（true）/ ノンリアルタイム（false）
	CTime		startTime;					// 開始日時
	stVScale	trandVScale;				// 縦軸スケール[0:左、1:右]
	HScale		trandHScale[MAX_HSCALE];	// 横軸スケール[0:トレンドグラフビュー1、1:トレンドグラフビュー2]

	CString		dataName[MAX_TDATA];		// データ名称
	CString		dataGName[MAX_TDATA];		// データ名称(グラフ用名)
	CString		dataUnit[MAX_TDATA];		// 単位
	int			nDecDidits[MAX_TDATA];		// 少数点以下の桁数
	bool		isViewMode[MAX_TDATA];		// 表示On/Off
	DrawMode	drawMode[MAX_TDATA];		// 描画モード（Line、Dots、L&D）
	COLORREF	color[MAX_TDATA];			// 表示色
	int			lineWidth[MAX_TDATA];		// 線幅
	DotSize		dotSize[MAX_TDATA];			// ドットサイズ
	VScaleKind	vscaleKind[MAX_TDATA];		// 縦軸スケール選択（右/左）
	CString		comment[MAX_TDATA];			// コメント
	bool		isComment[MAX_TDATA];		// コメント表示
	bool		isAlarm[MAX_TDATA];			// アラーム表示
	double		alarm1Major[MAX_TDATA];		// アラーム1 上限
	double		alarm1Minor[MAX_TDATA];		// アラーム1 下限 
	double		alarm2Major[MAX_TDATA];		// アラーム2 上限
	double		alarm2Minor[MAX_TDATA];		// アラーム2 下限

	BOOL		m_bSetted;					// 設定済み

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	int getHScal(CString str) {
		for (int i = 0; i < sizeof(strHScale) / sizeof(strHScale[0]); ++i)
		{
			if (str == strHScale[i])
			{
				return i;
			}
		};
		return -1;
	};
};


// スペクトラムグラフ表示データクラス
class CSpectrumViewData
{
public:
	CSpectrumViewData();
	~CSpectrumViewData();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	typedef struct {
		CString    sz_sts;
		CString    sz_low;
		CString    sz_upp;
		double  d_low;
		double  d_upp;
	} BandWidth_t;

	typedef struct {
		CString		strHeader;					/* スペクトラムファイル識別ヘッダ */
		BOOL		bAutoScale;					/* オートスケール */
		long        gl_y_grid_max;              /* Y軸グリッド最大値 */
		long        gl_y_grid_min;              /* Y軸グリッド最小値 */
		long        gl_y_grid_val;              /* Y軸グリッド間隔値 */
		double      gd_y_offset_val;            /* Y軸オフセット値 */
	} struct_VScale;

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	double		m_gda_spectrum_data[SPECTRUM_NUM];		// スペクトラムデータ
	double		m_gd_spectrum_peak;						// スペクトラムピーク値
	CString		m_gsz_time_tag;							// データ時刻
	CString		m_gsz_bandw_sts;						// バンド幅
	CString		m_gsz_moni_point;						// 監視ポイント
	double		m_gd_freq_ofs;							// 周波数オフセット			
	double		m_gd_center_freq;						// 中心周波数

	long        m_gl_moni_point_sel;			/* モニターポイント選択 */
	long        m_gl_bw_sel = 0;                /* 帯域幅設定選択 */
	double      m_gd_freq_ofs_in;               /* 中心周波数オフセット入力 */
	double		m_gd_peak_level;				/* ピークレベル */
	double      m_gd_peak_freq;                 /* ピーク周波数 */

//	long        m_gl_y_grid_min;                /* Y軸グリッド最小値 */
//	long        m_gl_y_grid_max;                /* Y軸グリッド最大値 */
//	long        m_gl_y_grid_val;                /* Y軸グリッド間隔値 */

//	double      m_gd_y_offset_val;              /* Y軸オフセット値 */
	double      m_gd_y_offset_att;              /* Y軸ATT値 */

	int			m_nDummyOffset;					/* ダミーオフセット */
	double		m_dDummyVOffset;				/* ダミー縦軸オフセット */

//	BOOL		m_bAutoScale;					/* オートスケール */
	struct_VScale	m_stVScale;					/* 縦軸設定 */

//	UINT		m_dummy_moni_point;
//	UINT		m_dummy_bandw;
//	double		m_dummy_freq_ofs;
//	double		m_dummy_center_freq;

	UINT		m_nStation;						/* 局番号 */
	int			m_nRetSpectrumData;				/* スペクトラムデータ取得結果 */

protected:

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	long GetVerticalMax()	{ return m_stVScale.gl_y_grid_max; }
	void SetVerticalMax(long max)	{ m_stVScale.gl_y_grid_max = max; }
	long GetVerticalMin()	{ return m_stVScale.gl_y_grid_min; }
	void SetVerticalMin(long min)	{ m_stVScale.gl_y_grid_min = min; }
	long GetVerticalGrid()	{ return m_stVScale.gl_y_grid_val; }
	void SetVerticalGrid(long grid)	{ m_stVScale.gl_y_grid_val = grid; }
	long GetAutoScale()		{ return m_stVScale.bAutoScale; }
	void SetAutoScale(BOOL autoscale)	{ m_stVScale.bAutoScale = autoscale; }
	struct_VScale& GetVScale()	{ return m_stVScale; }

	void SetStation(UINT nStation)	{ m_nStation = nStation; }

	void InitializeData(UINT nGraphType);
	int FileRead(FILE* fp, UINT nStationID, UINT nViewID, UINT nGraphType);
	int FileWrite(FILE* fp, UINT nStationID, UINT nViewID, UINT nGraphType);
	int MonOffsetGet();
	int GetSpectrumData();
	void CreateDummyData();
	void CreateDummyOffset();

	int GetRetSpectrumData()	{ return m_nRetSpectrumData; }

protected:
};


// 追尾レーダー制御監視データクラス
class CTrackingRadarCtrlMonData
{
public:
	CTrackingRadarCtrlMonData();
	~CTrackingRadarCtrlMonData();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */

public:
	// ON/OFF
	enum {
		eOFF = 0,
		eON,
		eCOMP
	} eONOFF;

	// 運用状況
	enum {
		eUNYOU_OUT = 0,						// 運用状況（非運用状態
		eUNYOU_START,						// 運用状況（運用開始30分以内前）
		eUNYOU_IN,							// 運用状況（運用中）   
		eUNYOU_TIME = 604800				// 運用切り替え開始時間（７日）
	} eUNYOU;

	enum {
		eDSP_OFF = 0,						// 表示切り替え不要  
		eDSP_ON								// 表示切り替え要
	} eDSP;

	enum {
		eYOHOUTI_NASI = 0,					// 予報値ファイルなし 
		eYOHOUTI_ARI						// 予報値ファイル有り
	} eYOHOUTI;

	enum {
		eYOHOGET_MATI = 1,					// 予報値ファイル取得待ち
		eYOHOGET_SUMI,						// 予報値ファイル取得済み
		eYOHOGET_OK							// 予報値ファイル取得可能
	} eYOHOGET;

	// グラフ表示用設定値テーブル
	struct gtdsp_inf_t {
		int		i_dsp_type;					// 表示状態（初期／トレンド）
		int		i_dsp_flg;   				// 表示切り替え（要／不要）  
		int		i_unyou_mode;				// 運用状況モード（今回）    
		int		i_unyou_mode_s;				// 運用状況モード（前回）     
		int		i_yohouti_umu;				// 予報値ファイル有無   
		int		i_yohouti_get;              // 予報値ファイル取得状況
		int		i_mondat_NGcnt;             // 制御用監視データ無効カウンタ

		// 今回の状況
		char	ca_stime[30];	            // パス開始時刻          
		char	ca_etime[30];	            // パス終了時刻         
		TCHAR	ca_fpath[1024];             // 予報値ファイルパス名
		char	ca_ftime[30];				// 予報値ファイル更新日時
		char	ca_plnno[30];	            // 計画番号      
		char	ca_eisei[100];              // 衛星名
		char	ca_AOS_time[30];            // ＡＯＳ時刻
		char	ca_AOS_AZ[30];              // ＡＯＳ ＡＺ値
		char	ca_AOS_EL[30];              // ＡＯＳ ＥＬ値 
		char	ca_LOS_time[30];            // ＬＯＳ時刻
		char	ca_LOS_AZ[30];              // ＬＯＳ ＡＺ値
		char	ca_LOS_EL[30];              // ＬＯＳ ＥＬ値
		char	ca_MEL_time[30];            // ＭＡＸ ＥＬ時刻
		char	ca_MEL_AZ[30];              // ＭＡＸ ＥＬ ＡＺ値
		char	ca_MEL_EL[30];              // ＭＡＸ ＥＬ ＥＬ値
		char	ca_visible[10];	            // ＶＩＳＩＢＬＥ

		// 前回の状況
		TCHAR	ca_fpath_s[1024];           // 予報値ファイルパス名
		char	ca_stime_s[30];	            // パス開始時刻
		char	ca_ftime_s[30];				// 予報値ファイル更新日時

		COLORREF	pixmap_color;			// ピックスマップ背景色
		COLORREF	grph_color;				// グラフ領域背景色
		COLORREF	frem_color;				// グラフ枠表示色
		COLORREF	frem_color2;			// グラフ枠表示色2
		COLORREF	e80_frem_color;			// EL80グラフ枠表示色
		COLORREF	grid_color;				// グラフ罫線表示色
		COLORREF	grid_color2;			// グラフ罫線表示色2
		COLORREF	strtit_color;			// タイトル文字表示色
		COLORREF	strcon_color;			// 固定項目文字表示色
		COLORREF	strvar_color;			// 可変項目文字表示色
		COLORREF	aos_yoho_color;			// グラフ描画色(予報値aos)
		COLORREF	los_yoho_color;			// グラフ描画色(予報値los)
		COLORREF	yoho_color;				// グラフ描画色(予報値通常)
		COLORREF	trnd_color;	  			// グラフ描画色(実測値)
		COLORREF	skyl_color;    			// グラフ描画色(スカイライン)
		COLORREF	rftr_color;	   			// グラフ描画色(ＲＦトリップ)
		COLORREF	over_color;				// グラフ描画色(範囲オーバー)

//		double		d_end_time;				// 最終格納データ発生時刻
	};

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

	long		m_l_inflg;				/* 監視データ有効状態             */

	gtdsp_inf_t	m_gst_dspinf;		// グラフ表示設定テーブル
	long		m_gpla_shmadr[GRNUM_PRS_MAX + 1]; // 共有メモリ格納アドレス格納領域
	int			m_i_into_flag;		// パス開始３０分以上前から３０分以内に入ったかどうかのフラグ
	long long	m_l_latest_time;	// 前回のカウントダウン値
	int			m_gi_cmp1;
	int			m_gi_cmp2;

	deque<Az_El>	m_queAzElSkyline;			// スカイラインデータキュー
	deque<Forecast_Az_El>	m_queAzElForecast;	// 予報値データキュー
	deque<Az_El> m_queAzElActual;				// 実測値データキュー

	int			m_i_unyou;				/* パス運用状況 				  */
//	char        m_ca_wtime[30];			/* パス運用開始までの時間 		  */
//	long long	m_d_time;				/* パス運用開始までの時間 		  */
//	char		m_ca_stime[30];			/* パス開始時刻                   */
//	char		m_ca_etime[30];			/* パス終了時刻                   */
//	TCHAR		m_ca_fpath[1024];		/* 予報値ファイル名（フルパス）   */
//	char		m_ca_ftime[30];			/* 予報値ファイル更新日時		  */
//	char		m_ca_plnno[30];			/* 計画番号                       */
//	char		m_ca_eisei[100];		/* 衛星名       			      */
//	char		m_ca_AOS_time[20];		/* ＡＯＳ時刻                     */
//	char		m_ca_AOS_AZ[9];			/* ＡＯＳ時 ＡＺ値                */
//	char		m_ca_AOS_EL[8];			/* ＡＯＳ時 ＥＬ値                */
//	char		m_ca_LOS_time[20];		/* ＬＯＳ時刻                     */
//	char		m_ca_LOS_AZ[9];			/* ＬＯＳ時 ＡＺ値                */
//	char		m_ca_LOS_EL[8];			/* ＬＯＳ時 ＥＬ値                */
//	char		m_ca_MEL_time[20];		/* ＭＡＸ－ＥＬ時刻               */
//	char		m_ca_MEL_AZ[9];			/* ＭＡＸ－ＥＬ時 ＡＺ値          */
//	char		m_ca_MEL_EL[8];			/* ＭＡＸ－ＥＬ時 ＥＬ値          */
//	char		m_ca_VISIBLE[9];		/* ＶＩＳＩＢＬＥ時刻             */
	char        m_ca_yohouget[30];		/* 予報値ファイル取得状況         */

	int			m_nViewID;				/* ビューID */
	int			m_nStationID;			/* 局ID */

	CString		m_yohoFilePath;			/* 予報値ファイルパス */
	FILETIME	m_yohoFileTime;			/* 予報値ファイル時刻 */

	bool		m_bPassEnd;				/* パス終了 */

protected:

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
//	void SetPredFilePath(TCHAR* fpath)  { wcscpy_s(m_ca_fpath, 1024, fpath); }
	void SetViewID(int nID)		{ m_nViewID = nID; }
	void SetStationID(int nID)	{ m_nStationID = nID; }
	void UpdateData();
//	int Gtunyou();
	int GetKanshiData();
//	int GetFileData();
	void Gtinit();
	void Gtgetskyl();
	void Gtgetrftr();
	int Gtgetyoho();
	void ClearQeue();

	void GetAzEl(char* pc_rec, Az_El* az_el);
	void GetForecastAzEl(int i_type, char* pc_rec, Forecast_Az_El* az_el, int i_roop_cnt);

protected:

};


// 校正診断画面データ管理クラス

#define SS_TCR_LBW	_T("SS-TCRDEM.CARR_LOOP")
#define XX_TCR_LBW	_T("DSNTCR.CARR_LOOP_BAND")
#define X_RX2_LBW	_T("X-RX2.PLL_BW")
#define S_TRKRX_LBW	_T("S-TRKRX.CARR_LBW")
#define S_ACQRX_LBW	_T("S-ACQ_TRKRX.CARR_LOOP_BAND")
#define X_TRKRX_LBW	_T("X-TRKRX.CARR_LOOP_BAND")
#define X_ACQRX		_T("X-ACQ_TRKRX.CARR_LOOP_BAND")

class CRdnsChkData
{
public:
	CRdnsChkData();
	~CRdnsChkData();

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

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	double BitrateStrToDouble(CString str);

protected:

};


// 計画立案画面データ管理クラス

typedef struct  {
	CTime		start;		// 開始日時
	CTime		end;		// 終了日時
} stTime;	// 開始終了日時構造体

typedef struct  {
	CString		start;		// 開始日時
	CString		end;		// 終了日時
} stTimeStr;	// 開始終了日時構造体

enum {
	STRON_OFF,
	STRON_ON,
};
static const CString strON[] = {
	_T("無し"),
	_T("有り"),
	_T(""),
};

enum {
	ARINASI_ARI,
	ARINASI_NASI,
};
static const CString strAriNasi[] = {
	_T("あり"),
	_T("なし"),
	_T(""),
};

// ANT天頂指向有無
enum {
	ANTZENITH_ARI_SWTOW,
	ANTZENITH_ARI,
	ANTZENITH_NASI,
};
static const CString strANTZenith[] = {
	_T("あり(STOWあり)"),
	_T("あり(STOWなし)"),
	_T("なし"),
	_T(""),
};

// 送信機ドップラ補償
enum {
	TXDOPPLER_ARI,
	TXDOPPLER_NASI_AVE,
	TXDOPPLER_NASI_STT,
	TXDOPPLER_NASI_NOM,
};
static const CString strTxDoppler[] = {
	_T("有り"),
	_T("無し（センター）"),
	_T("無し（ベストロック）"),
	_T("無し（ノミナル）"),
	_T(""),
};

static const CString strTxDoppler2[] = {
	_T("動的補償"),
	_T("パスセンター"),
	_T("AOSベストロック"),
	_T("ノミナル"),
	_T(""),
};

// 受信信機ドップラ補償
enum {
	RXDOPPLER_SX,
	RXDOPPLER_S,
	RXDOPPLER_X,
	RXDOPPLER_NASI,
};
static const CString strRxDoppler[] = {
	_T("S/X"),
	_T("S"),
	_T("X"),
	_T("無し"),
	_T(""),
};

static const CString strRxDoppler54[] = {
	_T("有り"),
	_T("無し"),
	_T(""),
	_T(""),
	_T(""),
};


// 送信出力
enum {
	TRSPWR_HPA_10KW,
	TRSPWR_SSPA_1KW,
	TRSPWR_SSPA_100W,
	TRSPWR_SSPA_25W
};
static const CString strTrsPwr20[] = {
	_T("HPA_10KW"),
	_T("SSPA_1KW"),
	_T("SSPA_100W"),
	_T("SSPA_25W"),
	_T(""),
};

enum {
	TRSPWRX_X,
	TRSPWRX_S,
};

enum {
	TRSPWRX_HPA20kw,
	TRSPWRX_HPA2kw,
	TRSPWRX_HPA200w,
	TRSPWRX_SSPA100w,
	TRSPWRX_SSPA10w,
};
static const CString strTrsPwrX34[] = {
	_T("HPA-20kw"),
	_T("HPA-2kw"),
	_T("HPA-200w"),
	_T("SSPA-100w"),
	_T("SSPA-10w"),
	_T(""),
};

enum {
	TRSPWRS_HPA10kw,
	TRSPWRS_SSPA1kw,
	TRSPWRS_SSPA100w,
	TRSPWRS_SSPA25w,
	TRSPWRS_SSPA75w,
};
static const CString strTrsPwrS34[] = {
	_T("HPA-10kw"),
	_T("SSPA-1kw"),
	_T("SSPA-100w"),
	_T("SSPA-25w"),
	_T("SSPA-75w"),
	_T(""),
};

enum {
	TRSPWR_20kw,
	TRSPWR_2kw,
	TRSPWR_200w
};
static const CString strTrsPwr64[] = {
	_T("20kw"),
	_T("2kw"),
	_T("200w"),
	_T(""),
};

// strTrsPwr54[]は、関数(COprPlnData::GetOpTOutIdx)が未使用なので追加不要

// eqdb_lib.h >>>
#define	SATNAME_LEN		16	/* 衛星名称最大長+1 */
#define	PRDPATH_LEN		128	/* 予報値ファイル格納ディレクトリ名称最大長+1 */
#define	PLANPATH_LEN	128	/* 計画ファイル格納ディレクトリ名称最大長+1 */
#define	SATTXSTR_LEN	24	/* 衛星送信機識別最大長+1 */
#define	SATANTSTR_LEN	24	/* 衛星アンテナ識別名称最大長+1 */
// <<< eqdb_lib.h

// opr_def.h >>>
#define TBLMAX			40	/*一時的にテーブル最大数を定義しておく。後で削除すること*/
// <<< opr_def.h

// opr_plan.h >>>
struct txpwr_inf_t{
	char *sz_txpwr_val;								/* 送信パワー */
	char *satant_sval[20];							/* アンテナ */
	int i_s_snd_level;								/* S-BAND送信レベル項目数 */
	int i_x_snd_level;								/* X-BAND送信レベル項目数 */
};

struct sat_inf_t{									/* 衛星本体情報控え */
	char sz_satnames[SATNAME_LEN];					/* 衛星名 */
	long l_sat_no;									/* 衛星番号 */
	long l_use_band;								/* 使用周波数帯 */
	long l_level;									/* 使用帯域 */
	double d_s_coh;									/* sーbandコヒーレントレシオ */
	double d_x_coh;									/* xーbandコヒーレントレシオ */
	double nominal_tx[256];							/* ノミナル送信周波数 */
	double nominal_rx[256];							/* ノミナル受信周波数 */
	char *cmd_tbl[TBLMAX];							/* s/xーbandコマンドビットレートテーブル */
	char *x_cmd_tbl[TBLMAX];						/* xーbandコマンドビットレートテーブル */
	int rxbands[TBLMAX];							/* ノミナル受信周波数バンド帯 */
	/*	char *lev_s_tbl[TBLMAX];       */			/* sーband衛星通信レベル */
	/*	char *lev_x_tbl[TBLMAX];       */			/* xーband衛星通信レベル */
	char *br_s_tbl[TBLMAX];							/* sーbandテレメビットレート */
	char *br_x_tbl[TBLMAX];							/* xーbandテレメビットレート */
	char *br_qpsk_tbl[TBLMAX];						/* xーbandテレメビットレート */
	char sz_pred_dir[PRDPATH_LEN];					/* 予報値ファイルディレクトリ */
	char sz_plan_dir[PLANPATH_LEN];					/* 計画ファイルディレクトリ */
	char sz_sattx_val[SATTXSTR_LEN];				/* */
	struct txpwr_inf_t s_txpwr_sval[20];			/* */
	struct txpwr_inf_t x_txpwr_sval[20];			/* */
	int i_cmd_bitrate;								/* S/X-BANDコマンドビットレート項目数 */
	int i_x_cmd_bitrate;							/* X-BANDコマンドビットレート項目数 */
	int i_bitrate_s;								/* S-BANDテレメビットレート項目数 */
	int i_bitrate_x;								/* X-BANDテレメビットレート項目数 */
	int i_bitrate_x2;								/* X-BANDテレメビットレート項目数 */
	int i_bitrate_ka;								/* Ka-BANDテレメビットレート項目数 */
	int i_bitrate_qpsk;								/* X-BANDテレメビットレート項目数 */
	int i_s_power;									/* S-BAND送信パワー項目数 */
	int i_x_power;									/* X-BAND送信パワー項目数 */
	int	i_x_tx_sel;									/* Ｘ帯送信設備有無 */
	int	i_x_rx_sel;									/* Ｘ帯受信設備識別 */
	int	i_s_tx_ramp;								/* Ｓ帯送信設備ランプ制御有無 */
	int	i_x_tx_ramp;								/* Ｘ帯送信設備ランプ制御有無 */
	int	i_s_rx_ramp;								/* Ｓ帯受信設備ランプ制御有無 */
	int	i_x_rx_ramp;								/* Ｘ帯受信設備ランプ制御有無（鹿児島専用） */
	int	i_x_rx1_ramp;								/* Ｘ帯受信設備１ランプ制御有無（臼田専用） */
	int	i_x_rx2_ramp;								/* Ｘ帯受信設備２ランプ制御有無（臼田専用） */
	int	i_sx_acqrx_ramp;							/* ＳＸ帯捕捉受信設備ランプ制御有無（鹿児島専用） */
	int	i_x_qpskrx_ramp;							/* Ｘ帯高速受信設備ランプ制御有無 */
	int	i_acq_route;								/* 補足ルート */
	int	i_pl_sel;									/* ACQ/MAIN判定有無 */
	int	i_top_mon;									/* 天頂通過監視有無 */
	int	i_ent_angle;								/* 天頂通過監視入角 */
	int	i_ext_angle;								/* 天頂通過監視出角 */
	int	i_s_rx;										/* ＡＧＣ校正対象受信機情報（Ｓ帯主受信設備） */
	int	i_x_rx;										/* ＡＧＣ校正対象受信機情報（Ｘ帯主受信設備）鹿児島専用 */
	int	i_x_rx1;									/* ＡＧＣ校正対象受信機情報（Ｘ帯主受信設備１）臼田専用 */
	int	i_x_rx2;									/* ＡＧＣ校正対象受信機情報（Ｘ帯主受信設備２）臼田専用 */
	int	i_x_dsntcr1;								/* ＡＧＣ校正対象受信機情報（X帯DSN-MODEM1）GREAT専用 */
	int	i_ka_dsntcr1;								/* ＡＧＣ校正対象受信機情報（Ka帯DSN-MODEM1）GREAT専用 */
	int	i_sx_acqrx_s;								/* ＡＧＣ校正対象受信機情報（ＳＸ帯補足用受信設備　Ｓ帯）鹿児島専用 */
	int	i_sx_acqrx_x;								/* ＡＧＣ校正対象受信機情報（ＳＸ帯補足用受信設備　Ｘ帯）鹿児島専用 */
	int	i_x_qpskrx;									/* ＡＧＣ校正対象受信機情報（Ｘ帯高速データ用受信設備） */
	int i_rarr_band;								/* ＲＡＲＲ使用帯域情報 */
	int	i_ss_tcr;									/* ＡＧＣ校正対象受信機情報（ＳＳ－ＴＣＲＤＥＭ） */
	int i_rxfq_ka;									/* ノミナル受信周波数情報 Ka 有無 */
	int i_rxfq_x;									/* ノミナル受信周波数情報 X 有無 */
	int i_rxfq_s;									/* ノミナル受信周波数情報 S 有無 */
};
// <<< opr_plan.h

// eqsat_db.h >>>
struct sattx_t {									/* 衛星送信機情報定義テーブル: ソース上"sattx"に相当 */
	long l_id;										/* データｉｄ: 定数=SATTX_ID値 */
	long l_len;										/* データ長  : sizeof( struct sattx_t ) */
	char sz_sattx[SATTXSTR_LEN];					/* 送信機識別 */
};
struct satant_t {									/* 衛星アンテナ情報定義テーブル: ソース上"satant"に相当 */
	long l_id;										/* データｉｄ: 定数=SATANT_ID値 */
	long l_len;										/* データ長  : sizeof( struct satant_t ) */
	char sz_satant[SATANTSTR_LEN];					/* 衛星アンテナ識別 */
	long l_valnum;									/* 設定値数（＊Ｆ１） */
};
// <<< eqsat_db.h


#define RDY_AGC_OLD				_T("CTRL.RDYCHK_AGC_OLD")		/* RDYCHK AGC */
#define RDY_RNG_OLD				_T("CTRL.RDYCHK_RNG_OLD")		/* RDYCHK RNG */
#define INIT					_T("INIT")						/* INIT */
#define BIT_S					_T("BITRATE-S")					/* TLM(S) */
#define BIT_X					_T("BITRATE-X")					/* TLM(X) */
#define BIT_X2					_T("BITRATE-X2")				/* TLM(X)2 */
#define BIT_KA					_T("BITRATE-KA")				/* TLM(Ka) */
#define C_BIT					_T("CMD-BITRATE")				/* COMMAND BITRATE(S) */
#define C_BIT2					_T("CMD2-BITRATE")				/* COMMAND BITRATE */
#define C_BIT3					_T("CMD3-BITRATE")
#define C_BIT4					_T("CMD4-BITRATE")				/* COMMAND BITRATE */
#define X_CMD2_BITRATE_TXT		_T("CMD4-BITRATE")

#define X_CMD2_BITRATE			_T("XCMD-BITRATE2")
#define S_ACQ_BITRATE			_T("BITRATE-S_ACQ")
#define X_ACQ_BITRATE			_T("BITRATE-X_ACQ")
#define S_TRKRX_BITRATE			_T("BITRATE-S_TRK")
#define S_ACQT_BITRATE			_T("S-ACQTCRDEM.BIT_RATE")
#define X_HRX_DATARATE			_T("X-HRX.DEMO_DATA_RATE_CONT")
#define SS_BITRATE				_T("SS-TCRDET.BIT_RATE")


#define C_MOD					_T("CMD_MOD_ON")				/* CMD_MOD */
#define R_MOD					_T("RNG_MOD_ON")				/* RNG_MOD */
#define R_BAND					_T("RNG_BAND")					/* RNG_BAND */
#define R_START					_T("R-START")					/* RNG_START */
#define R_END					_T("R-END")						/* RNG_END */
#define T_OFF					_T("OFFSET")					/* OFFSET */
#define P_START					_T("P-START")					/* P-START */
#define P_END					_T("P-END")						/* P-END */
#define A_FLAG					_T("AOS_FLAG")					/* AOS_FLAG */
#define L_FLAG					_T("LOS_FLAG")					/* LOS_FLAG */
#define AUTO_ACQ				_T("AUTO_ACQ")					/* AUTO_ACQ */
#define BIT_XQPSK				_T("BITRATE-XQPSK")				/* XQPSK(X)*/
#define BIT_XQPSK20				_T("BITRATE-XHRX")				/* XQPSK(X)*/
#define BIT_XHRX				_T("BITRATE-XHRX")				/* XHRX */

#define BIT_TRKRX				_T("BITRATE-X")

#define T_ANT					_T("TERM_ANT_MODE")				/* T_ANT_MODE */
#define X_TX_RAMP				_T("TX_RAMP")					/* X_TX_RAMP_MODE */
#define RX_RAMP					_T("RX_RAMP")					/* RX_RAMP_MODE */
#define ACQ_ROUTE				_T("ACQ_ROUTE")					/* ACQ_ROUTE */
#define FALSE_LOCK				_T("FALSE_LOCK")				/* FALSE_LOCK_MODE */
#define POL_CHK					_T("POL_CHK")					/* POL_CHK_MODE */
#define POL_ENT					_T("POL_ENT")					/* POL_ENT */
#define POL_EXIT				_T("POL_EXIT")					/* POL_EXIT */
#define	MODECHK					_T("CTRL.MODECHK")				/* MODECHK */
#define	PREDGET					_T("PREDGET")					/* PREDGET */
#define LEV_S					_T("LEVEL-S")					/* COMINICATION LEVEL(S)*/
#define LEV_X					_T("LEVEL-X")					/* COMINICATION LEVEL(X)*/
#define DELAY_SEL_TXT			_T("RNG_TYPE")					/* 再生測距対応 */
#define DELAY_SEL_CONT_TXT		_T("XX-RRNG.DIST_RNG_VIDEO")	/* 再生測距対応 */
#define S_TX_PWR_SET_TXT		_T("CTRL.S_TX_RF_PWR")			/* Ｘ帯ＨＰＡ増設対応 */
#define X_TX_PWR_SET_TXT		_T("CTRL.X_TX_RF_PWR")			/* Ｘ帯ＨＰＡ増設対応 */
#if 1
#define	X_MOD_SEL_TXT			_T("CTRL.DSN_TCRMOD_SEL")		/* Ｘ帯ＨＰＡ増設対応 */
#else
#define	X_MOD_SEL_TXT			_T("CTRL.X_MOD_SEL")			/* Ｘ帯ＨＰＡ増設対応 */
#endif
#define C_BIT_X					_T("XCMD-BITRATE")				/* COMMAND BITRATE(X) */
#define C_BIT_X2				_T("XCMD-BITRATE2")				/* COMMAND BITRATE(X) */
#define RDY_AGC					_T("CTRL.RDYCHK_AGC")			/* RDYCHK AGC */
#define RDY_RNG					_T("CTRL.RDYCHK_RNG")			/* RDYCHK RNG */
#define RDY_TLM					_T("CTRL.RDYCHK_TLM")			/* RDYCHK TLM */
#define RDY_CMD					_T("CTRL.RDYCHK_CMD")			/* RDYCHK CMD */
#define SYS_ANT					_T("SYSTEM.ant_waitpoint&")		/* REM ANT */
#define	CTRL_IINFO				_T("CTRL.INITINFO")				/* CTRL.INITINFO */
#define	RANGE_BAND				_T("RANGE_BAND")				/* S帯RNG計測帯域 */
#define	ANT_STOW				_T("ANT_STOW")					/* ANT天頂指向有無 */
#define	S_TLMIN_CTRL_TXT		_T("STLMIN.TLM_START")			/* S帯BBE換装対応 */
#define	CTRL_S_RNG_ROUTE		_T("CTRL.S_RNG_ROUTE")			/* S帯BBE換装対応 */
#define	DELAY_SEL_CONT_DSN_TXT	_T("DSNTCR.RNG_MODE")			
#define DSN_TX_AID_ON_TXT		_T("DSNTCR.TX_CARR_DOPP_PRE")	
#define DSN_RX_AID_ON_TXT		_T("DSNTCR.RX_CARR_DOPP_PRE")	
#define S_TX_INIT_TXT			_T("INIT.S-TX")
#define X_TX_INIT_TXT			_T("INIT.X-TX")
#define INIT_CID				_T("INIT.CID")
#define X_ROUTE_DEF_TXT			_T("INIT.DSNTCR-DEM")
#define	X_TXSYN_TXT				_T("X-TXSYN.")
#define DSN_PREDGET_TXT			_T("XX-DSNTCRFC.PREDGET")			
#define	SS_TCRMOD_TXT			_T("SS-TCRMOD.")
#define	S_DCSYN_TXT				_T("SS-DCSYN1.")
#define	S_RXSYN_TXT				_T("SS-TCRDEM.")
#define	X_RXSYN_TXT				_T("X-RXSYN.")
#define	X_RX1SYN_TXT			_T("SX-DPL.")
#define	X_RX2SYN_TXT			_T("X-RX2SYN.")
#define	X_QPSKSYN_TXT			_T("X-QPSKRXSYN.")
#define S_TX_PWR_SET_64_TXT		_T("S-TXRF.IF_RF_PWR")
//#define X_TX_PWR_SET_64_TXT		_T("X-TX.HPA_RF_PWR")
#define X_TX_PWR_SET_64_TXT		_T("X-UL_CNT.RF_PWR_200/2k/20kW")
#define X_TX_PWR_SET_54_TXT		_T("X-SSPA.TX_POWER_MODE")
#define CID_TXT					_T("CID")
#define ST_NAME					_T("ST_NAME")
#define PRI						_T("PRI")
#define OPE_MODE				_T("OPE_MODE")

#define DOWN_START				_T("CTRL.DOWN_START")
#define DOWN_END				_T("CTRL.DOWN_END")
#define UP_START				_T("CTRL.UP_START")
#define UP_END					_T("CTRL.UP_END")
#define RNG_START				_T("CTRL.RNG_START")
#define RNG_END					_T("CTRL.RNG_END")
#define RNG_SS					_T("SS")
#define RNG_SX					_T("SX")
#define RNG_XX					_T("XX")
#define RNG_XKa					_T("XKa")
#define CMD_START				_T("CTRL.CMD_START")
#define CMD_END					_T("CTRL.CMD_END")
#define CMD_S					_T("S")
#define CMD_X					_T("X")
#define TLM_START				_T("CTRL.TLM_START")
#define TLM_END					_T("CTRL.TLM_END")
#define TLM_S					_T("S")
#define TLM_X					_T("X")
#define TLM_KA					_T("Ka")
#define AutoGenerated			_T("AUTO")

#define BIT_S_TRK				_T("BITRATE-S_TRK")
#define BIT_S_ACQ				_T("BITRATE-S_ACQ")
#define BIT_X_ACQ				_T("BITRATE-X_ACQ")
#define BIT_Ka					_T("BITRATE-Ka")	/* eqsat.db(マクロ)、CID */

#define PROP_READ				_T("CTRL.PROP_READ")


//char *sz_delay_sel_prm[] = { _T("REPLAY"), _T("NORMAL"), NULL };
//char *sz_delay_sel_dsn_prm[] = { _T("REGEN"), _T("TURNAROUND"), NULL };

#define SAT_DATA     15
#define START_OFFSET 120.0

static const CString sz_c_ele20[] = {
	RDY_AGC_OLD,				// 0
	RDY_RNG_OLD,				// 1
	INIT,						// 2
	BIT_S,						// 3
	BIT_X,						// 4
	C_BIT,						// 5
	C_MOD,						// 6
	R_MOD,						// 7
	R_BAND,						// 8
	R_START,					// 9
	R_END,						// 10
	T_OFF,						// 11
	P_START,					// 12
	P_END,						// 13
	A_FLAG,						// 14
	L_FLAG,						// 15
	AUTO_ACQ,					// 16
	BIT_XQPSK,					// 17
	T_ANT,						// 18
	X_TX_RAMP,					// 19
	RX_RAMP,					// 20
	ACQ_ROUTE,					// 21
	FALSE_LOCK,					// 22
	POL_CHK,					// 23
	POL_ENT,					// 24
	POL_EXIT,					// 25
	MODECHK,					// 26
	PREDGET,					// 27
	LEV_S,						// 28
	LEV_X,						// 29
	DELAY_SEL_TXT,				// 30
	DELAY_SEL_CONT_TXT,			// 31
	S_TX_PWR_SET_TXT,			// 32
	X_TX_PWR_SET_TXT,			// 33
	X_MOD_SEL_TXT,				// 34
	_T("*"),					// 35
	RDY_AGC,					// 36
	RDY_RNG,					// 37
	RDY_TLM,					// 38
	RDY_CMD,					// 39
	SYS_ANT,					// 40
	CTRL_IINFO,					// 41
	_T("*"),					// 42
	ANT_STOW,					// 43
	S_TLMIN_CTRL_TXT,			// 44
	CTRL_S_RNG_ROUTE,			// 45
	DELAY_SEL_CONT_DSN_TXT,		// 46
	DSN_TX_AID_ON_TXT,			// 47
	DSN_RX_AID_ON_TXT,			// 48
	CID_TXT,					// 49
	ST_NAME,					// 50
	PRI,						// 51
	OPE_MODE,					// 52
	DOWN_START,					// 53
	DOWN_END,					// 54
	UP_START,					// 55
	UP_END,						// 56
	RNG_START,					// 57
	RNG_END,					// 58
	CMD_START,					// 59
	CMD_END,					// 60
	TLM_START,					// 61
	TLM_END,					// 62
	AutoGenerated,				// 63
	INIT_CID,					// 64
	PROP_READ,					// 65
	_T(""),
};

static const CString sz_c_ele34[] = {
	RDY_AGC_OLD,				// 0
	RDY_RNG_OLD,				// 1
	INIT,						// 2
	BIT_S,						// 3
	BIT_X,						// 4
	C_BIT,						// 5
	C_MOD,						// 6
	R_MOD,						// 7
	R_BAND,						// 8
	R_START,					// 9
	R_END,						// 10
	T_OFF,						// 11
	P_START,					// 12
	P_END,						// 13
	A_FLAG,						// 14
	L_FLAG,						// 15
	AUTO_ACQ,					// 16
	BIT_XHRX,					// 17
	T_ANT,						// 18
	X_TX_RAMP,					// 19
	RX_RAMP,					// 20
	ACQ_ROUTE,					// 21
	FALSE_LOCK,					// 22
	POL_CHK,					// 23
	POL_ENT,					// 24
	POL_EXIT,					// 25
	MODECHK,					// 26
	PREDGET,					// 27
	LEV_S,						// 28
	LEV_X,						// 29
	DELAY_SEL_TXT,				// 30
	DELAY_SEL_CONT_TXT,			// 31
	S_TX_PWR_SET_TXT,			// 32
	X_TX_PWR_SET_TXT,			// 33
	X_MOD_SEL_TXT,				// 34
	C_BIT_X2,					// 35
	RDY_AGC,					// 36
	RDY_RNG,					// 37
	RDY_TLM,					// 38
	RDY_CMD,					// 39
	SYS_ANT,					// 40
	CTRL_IINFO,					// 41
	RANGE_BAND,					// 42
	ANT_STOW,					// 43
	S_TLMIN_CTRL_TXT,			// 44
	CTRL_S_RNG_ROUTE,			// 45
	DELAY_SEL_CONT_DSN_TXT,		// 46
	DSN_TX_AID_ON_TXT,			// 47
	DSN_RX_AID_ON_TXT,			// 48
	S_TX_INIT_TXT,				// 49
	X_TX_INIT_TXT,				// 50
	CID_TXT,					// 51
	ST_NAME,					// 52
	PRI,						// 53
	OPE_MODE,					// 54
	DOWN_START,					// 55
	DOWN_END,					// 56
	UP_START,					// 57
	UP_END,						// 58
	RNG_START,					// 59
	RNG_END,					// 60
	CMD_START,					// 61
	CMD_END,					// 62
	TLM_START,					// 63
	TLM_END,					// 64
	AutoGenerated,				// 65
	INIT_CID,					// 66
	BIT_S_TRK,					// 67
	BIT_S_ACQ,					// 68
	BIT_X2,						// 69
	BIT_X_ACQ,					// 70
	PROP_READ,					// 71
	_T(""),
};

static const CString sz_c_ele64[] = {
	RDY_AGC_OLD,				// 0
	RDY_RNG_OLD,				// 1
	INIT,						// 2
	BIT_S,						// 3
	BIT_X,						// 4
	C_BIT,						// 5
	C_MOD,						// 6
	R_MOD,						// 7
	R_BAND,						// 8
	R_START,					// 9
	R_END,						// 10
	T_OFF,						// 11
	P_START,					// 12
	P_END,						// 13
	A_FLAG,						// 14
	L_FLAG,						// 15
	AUTO_ACQ,					// 16
	BIT_XQPSK,					// 17
	T_ANT,						// 18
	X_TX_RAMP,					// 19
	RX_RAMP,					// 20
	ACQ_ROUTE,					// 21
	FALSE_LOCK,					// 22
	POL_CHK,					// 23
	POL_ENT,					// 24
	POL_EXIT,					// 25
	MODECHK,					// 26
	PREDGET,					// 27
	LEV_S,						// 28
	LEV_X,						// 29
	CTRL_IINFO,					// 30
	S_TX_PWR_SET_64_TXT,		// 31
	X_TX_PWR_SET_64_TXT,		// 32
	DELAY_SEL_TXT,				// 33
	DELAY_SEL_CONT_TXT,			// 34
	X_MOD_SEL_TXT,				// 35
	C_BIT3,						// 36
	RDY_AGC,					// 37
	RDY_RNG,					// 38
	RDY_TLM,					// 39
	RDY_CMD,					// 40
	SYS_ANT,					// 41
	DELAY_SEL_CONT_DSN_TXT,		// 42
	X_CMD2_BITRATE_TXT,			// 43
	DSN_TX_AID_ON_TXT,			// 44
	DSN_RX_AID_ON_TXT,			// 45
	ANT_STOW,					// 46
	CID_TXT,					// 47
	ST_NAME,					// 48
	PRI,						// 49
	OPE_MODE,					// 50
	DOWN_START,					// 51
	DOWN_END,					// 52
	UP_START,					// 53
	UP_END,						// 54
	RNG_START,					// 55
	RNG_END,					// 56
	CMD_START,					// 57
	CMD_END,					// 58
	TLM_START,					// 59
	TLM_END,					// 60
	AutoGenerated,				// 61
	INIT_CID,					// 62
	PROP_READ,					// 63
	_T(""),
};

static const CString sz_c_ele54[] = {
	RDY_AGC_OLD,				// 0
	RDY_RNG_OLD,				// 1
	INIT,						// 2
	BIT_KA,						// 3	BITRATE-KA
	BIT_X,						// 4
	C_BIT,						// 5
	C_MOD,						// 6
	R_MOD,						// 7
	R_BAND,						// 8
	R_START,					// 9
	R_END,						// 10
	T_OFF,						// 11
	P_START,					// 12
	P_END,						// 13
	A_FLAG,						// 14
	L_FLAG,						// 15
	AUTO_ACQ,					// 16
	BIT_XQPSK,					// 17
	T_ANT,						// 18
	X_TX_RAMP,					// 19
	RX_RAMP,					// 20
	ACQ_ROUTE,					// 21
	FALSE_LOCK,					// 22
	POL_CHK,					// 23
	POL_ENT,					// 24
	POL_EXIT,					// 25
	MODECHK,					// 26
	PREDGET,					// 27
	LEV_S,						// 28
	LEV_X,						// 29
	CTRL_IINFO,					// 30
	S_TX_PWR_SET_64_TXT,		// 31
	X_TX_PWR_SET_54_TXT,		// 32	SSPA
	DELAY_SEL_TXT,				// 33
	DELAY_SEL_CONT_TXT,			// 34
	X_MOD_SEL_TXT,				// 35
	C_BIT3,						// 36
	RDY_AGC,					// 37
	RDY_RNG,					// 38
	RDY_TLM,					// 39
	RDY_CMD,					// 40
	SYS_ANT,					// 41
	DELAY_SEL_CONT_DSN_TXT,		// 42
	X_CMD2_BITRATE_TXT,			// 43
	DSN_TX_AID_ON_TXT,			// 44
	DSN_RX_AID_ON_TXT,			// 45
	ANT_STOW,					// 46
	CID_TXT,					// 47
	ST_NAME,					// 48
	PRI,						// 49
	OPE_MODE,					// 50
	DOWN_START,					// 51
	DOWN_END,					// 52
	UP_START,					// 53
	UP_END,						// 54
	RNG_START,					// 55
	RNG_END,					// 56
	CMD_START,					// 57
	CMD_END,					// 58
	TLM_START,					// 59
	TLM_END,					// 60
	AutoGenerated,				// 61
	INIT_CID,					// 62
	PROP_READ,					// 63
	_T(""),
};

#define STR_SX_ACQRX_S			_T("SX-ACQRX_S")
#define STR_SX_ACQRX_X			_T("SX-ACQRX_X")
#define STR_SS_TCR				_T("SS-TCR")
#define STR_S_RX				_T("S-RX")
#define STR_S_RX_A				_T("S-RX_A")
#define STR_X_RX2_A				_T("X-RX2_A")
#define STR_SS_TCRDET			_T("SS-TCRDET")
#define STR_1					_T("1")
#define STR_2					_T("2")
#define STR_3					_T("3")
#define STR_4					_T("4")

#define STR_SS_TCR_AB			_T("SS-TCR_AB")
#define STR_X_RX				_T("X-RX")
#define STR_X_QPSKRX_A			_T("X-QPSKRX_A")
#define STR_DSN_MODEM_A			_T("DSN-MODEM_A")
#define STR_DSN_MODEM_X			_T("DSN-MODEM_X_A")
#define STR_DSN_MODEM_KA		_T("DSN-MODEM_KA_A")
#define STR_SS					_T("SS")
#define STR_SX					_T("SX")
#define STR_XX					_T("XX")
#define STR_RX					_T("RX")
#define STR_DXN					_T("DXN")
#define STR_DXR					_T("DXR")
#define STR_DKAN				_T("DKAN")
#define STR_DKAR				_T("DKAR")
#define STR_S					_T("S")
#define STR_X					_T("X")
#define STR_X2					_T("X2")
#define STR_KA					_T("KA")
#define STR_QPSK				_T("QPSK")
#define STR_ANT					_T("ANT")
#define STR_S_RX_AB				_T("S-RX_AB")
#define CTRL_4WAY				_T("CTRL_4WAY")	
#define CTRL_OLD				_T("_OLD")
#define OPE_TYPE				_T("OPE_TYPE")


static const CString sz_sx[] = {
	_T("S"),
	_T("X"),
	_T("S/X"),
	_T(""),
};

static const CString sz_on[] = { 
	_T("OFF"),
	_T("ON"),
	_T(""),
};
static const CString sz_auto[] = {
	_T("MAN"),
	_T("AUTO"),
	_T("")
};
static const CString sz_term_ant[] = {
	_T("POSN"),
	_T("NA"),
	_T("")
};
static const CString sz_term_ant64[] = {
	_T("PROG"),
	_T("MAN"),
	_T("STOW"),
	_T("NA"),
	_T("POS-1"),
	_T("POS-2"),
	_T("POS-3"),
	_T("POS-4"),
	_T("")
};
static const CString sz_rdychk[] = {
	_T("OFF"),
	_T("AGC"),
	_T("RNG"),
	_T("AGC+RNG"),
	_T("")
};
static const CString sz_on_off[] = { 
	_T("ON"),
	_T("OFF"),
	_T("")
};
static const CString sz_tx_ramp[] = {
	_T("ON"),
	_T("AVE"),
	_T("STT"),
	_T("NOMINAL"),
	_T("")
};
static const CString sz_rx_ramp[] = {
	_T("SX"),
	_T("S"),
	_T("X"),
	_T("OFF"),
	_T("")
};
static const CString sz_rx_ramp54[] = {
	_T("X"),	//受信ドップラ補償 有り
	_T("OFF"),	//受信ドップラ補償 無し
	_T("")
	_T("")
	_T("")
};
static const CString sz_delay_sel[] = {
	_T("REP"),
	_T("NOR"),
	_T("")
};
static const CString sz_delay_sel_btn[] = {
	_T("再生型"),
	_T("従来型"),
	_T("")
};
static const CString sz_delay_sel_prm[] = {
	_T("REPLAY"),
	_T("NORMAL"),
	_T("")
};
static const CString sz_delay_sel_dsn_prm[] = {
	_T("REGEN"),
	_T("TURNAROUND"),
	_T("")
};

// CID >>>
static const CString sz_tx_pwr_sel_cid20[] = {
	_T("HPA-10kw"),
	_T("SSPA-1kw"),
	_T("SSPA-100w"),
	_T("SSPA-25w"),
	_T("HPA-1kw"),
	_T("HPA-100w"),
	_T("")
};

static const CString sz_s_tx_pwr_sel_cid34[] = {
	_T("HPA-10kw"),
	_T("SSPA-1kw"),
	_T("SSPA-100w"),
	_T("SSPA-25w"),
	_T("SSPA-75w"),
	_T("HPA-1kw"),
	_T("HPA-100w"),
	_T("")
};

static const CString sz_x_tx_pwr_sel_cid34[] = {
	_T("20kw"),
	_T("2kw"),
	_T("200w"),
	_T("100w"),
	_T("10w"),
	_T("")
};

static const CString sz_s_tx_pwr_sel_cid64[] = {
	_T("20kw"),
	_T("2kw"),
	_T("200w"),
	_T("")
};

// sz_s_tx_pwr_sel_cid54[]は、未使用なので追加不要

static const CString sz_x_tx_pwr_sel_cid64[] = {
	_T("HPA1-20kw"),
	_T("HPA2-20kw"),
	_T("HPA1-2kw"),
	_T("HPA2-2kw"),
	_T("HPA1-200w"),
	_T("HPA2-200w"),
	_T("")
};

static const CString sz_x_tx_pwr_sel_cid54[] = {
	_T("40kW"),
#ifdef _GREAT_XSSPA_ISO
	_T("19.5kW"),
#else
	_T("20kW"),
#endif // _GREAT_XSSPA_ISO
	_T("2kW"),
	_T("200W"),
	_T("")
};
// <<< CID

static const CString sz_tx_pwr_sel20[] = {
	_T("HPA_10KW"),
	_T("SSPA_1KW"),
	_T("SSPA_100W"),
	_T("SSPA_25W"),
	_T("HPA-1KW"),
	_T("HPA-100W"),
	_T("")
};

static const CString sz_s_tx_pwr_sel[] = {
	_T("HPA_10kw"),
	_T("SSPA_1kw"),
	_T("SSPA_100w"),
	_T("SSPA_25w"),
	_T("SSPA_75w"),
	_T("HPA-1kw"),
	_T("HPA-100w"),
	_T("")
};
static const CString sz_x_tx_pwr_sel[] = {
	_T("HPA_20kw"),
	_T("HPA_2kw"),
	_T("HPA_200w"),
	_T("SSPA_100w"),
	_T("SSPA_10w"),
	_T("")
};
static CString sz_tx_pwr_sel64[] = {
	_T("20kw"),
	_T("2kw"),
	_T("200w"),
	_T("")
};
static CString sz_tx_pwr_sel54[] = {
	_T("40kW"),
#ifdef _GREAT_XSSPA_ISO
	_T("19.5kW"),
#else
	_T("20kW"),
#endif // _GREAT_XSSPA_ISO
	_T("2kW"),
	_T("200W"),
	_T("")
};
static const CString sz_s_tx_uc_sel[] = {
	_T("U/C1"),
	_T("U/C2"),
	_T("")
};
static const CString sz_mod_sel[] = {
	_T("DSN-MODEM"),
	_T("TCRMOD"),
	_T("")
};
static const CString sz_range_band[] = {
	_T("SS"),
	_T("SX"),
	_T("")
};
static const CString sz_range_band_ctrl[] = {
	_T("S/S"),
	_T("S/X"),
	_T("")
};
static const CString sz_ant_stow[] = {
	_T("ON"),
	_T("OF"),
	_T("NA"),
	_T("")
};

static const CString sz_ope_type[] = {
	_T("NORM"),
	_T("RECV"),
	_T("")
};

#define PLAN_GETFORM _T("局管制")
#define PLAN_TYPE	 _T("実運用")


struct stPlandata {
	CString		sz_filepath;		// 計画ファイルフルパス
	CString		sz_ftpfilepath;		// 計画ファイル出力フルパス
	CString		sz_start;			// パス開始時刻 "YYYY-MM-DD HH:MM:SS"[IN/OUT]
	CString		sz_end;				// パス終了時刻 "YYYY-MM-DD HH:MM:SS"[IN/OUT]
	CString		sz_mes;				// 重複チェックメッセージ[OUT]
	CString		sz_s_bitrate;		// Ｓ帯 送信レベル High/Mid/Low/OFF  [IN/OUT]
	CString		sz_x_bitrate;		// Ｘ帯 送信レベル High/Mid/Low/OFF  [IN/OUT]
	CString		sz_ka_bitrate;		// Ka帯 ビットレート
	CString		sz_x_qpsk;			// Ｘ帯QPSK 送信レベル High/Mid/Low/OFF  [IN/OUT]
	CString		sz_cmd_bit_rate;	// CMD S BIT RATE High/Mid/Low/OFF  [IN/OUT]
	CString		sz_x_cmd_bit_rate;	// CMD X BIT RATE High/Mid/Low/OFF  [IN/OUT]
	long		l_cmd_mod;			// CMD MOD 0:手動,1:自動 [IN/OUT]
	long		l_rng_mod;			// RNG MOD 0:手動,1:自動 [IN/OUT]
	CString		sz_rng_band;		// RNG使用バンド S/X/SX  [IN/OUT]
	CString		sz_rng_start;		// RNG開始時刻   MM:SS   [IN/OUT]
	CString		sz_rng_end;			// RNG終了時刻   HH:MM:SS[IN/OUT]
	long		l_rdychk;			// レディネスチェック 0:無し,1:AGC,2:RNG,3:AGC+RNG[IN/OUT]
	long		l_init;				// 初期設定 0:無し,1:有り [IN/OUT]
	long		l_auto_acq;			// 自動捕捉 0:無し,1:有り [IN/OUT]
	long		l_offset;			// 時刻オフセットへの追従 0:無し,1:有り [IN/OUT]
	long		l_a_flag;			// AOS FLAG 0:OFF,1:ON   [IN/OUT]
	long		l_l_flag;			// LOS FLAG 0:OFF,1:ON   [IN/OUT]
	long		l_term_ant;			// パス終了時ＡＮＴモード 0:POSN, 1:N/A       [IN/OUT]
	long		l_tx_ramp;			// 送信設備ランプ制御有無（0:ON, 1:センター, 2:ベストロック, 3:ノミナル）
	CString		sz_tx_ramp;			// 送信設備ランプ制御有無（0:ON, 1:センター, 2:ベストロック, 3:ノミナル）文字列
	long		l_rx_ramp;			// 受信設備ランプ制御有無（0:S/X, 1:S, 2:X, 3:なし） 54m(0:有り 1:無し)
	long		l_acq_route;		// 補足ルート（０～７）
	long		l_pl_sel;			// ACQ/MAIN判定有無（0:ON, 1:OFF）
	long		l_top_mon;			// 天頂通過監視有無（0:ON, 1:OFF）
	long		l_ent_angle;		// 天頂通過入角
	long		l_ext_angle;		// 天頂通過出角
	CString		sz_level_s;			// Ｓ帯衛星送信レベル    [IN/OUT]
	CString		sz_level_x;			// Ｘ帯衛星送信レベル    [IN/OUT]
	CString		sz_ant_s;			// Ｓ帯アンテナ選択情報  [IN/OUT]
	CString		sz_ant_x;			// Ｘ帯アンテナ選択情報  [IN/OUT]
	CString		sz_pred_path;		// 予報値ファイルパス名
	long		l_offset_val;		// 時刻オフセット値 [IN]
	long		l_delay_sel;		// 局内ディレイ値選択 0:再生型,1:従来型 [IN/OUT]
	long		l_tx_pwr_sel;		// 送信出力選択
	CString 	sz_tx_pwr_sel;		// 送信出力選択(文字列)
	long		l_mod_sel;			// Ｘ帯変調装置選択 0:MOD,1:TCRMOD [IN/OUT]
	long		l_tx_uc_sel;		// Ｓ帯送信周波数変換盤選択 0:U/C1,1:U/C2 [IN/OUT]
	long		l_antwait_flg;		// アンテナ待ち受け指向通知
	CString		sz_antwait_flg;		// アンテナ待ち受け指向通知 文字列
	long		l_ant_time;			// アンテナ待ち受け指向通知時刻
	long		l_rdy_time;			// レディちぇくチェック開始時刻
	long		l_agc_index;		// レディネスチェック AGC
	time_t		l_agc_time;
	long		l_agc_s_time;
	long		l_agc_x_time;
	long		l_rng_index;		// レディネスチェック RNG
	time_t		l_rng_time;
	long		l_rng_xx_time;
	long		l_rng_rx_time;
	long		l_tlm_index;		// レディネスチェック TLM
	time_t		l_tlm_time;
	long		l_tlm_s_time;
	long		l_tlm_x_time;
	long		l_cmd_index;		// レディネスチェック CMD
	time_t		l_cmd_time;
	long		l_range_band;
	long		l_ant_stow;
	CString		sz_ant_stow;
	long		l_route_mode;		// X帯送受信ルート判定
	long		l_tx_mode;			// 送信出力　0:S-TX、1：X-TX

	long				l_uplink;				// アップリンク 有無
	vector<CString>		sz_uplink_s_timelst;	// アップリンク 時刻リスト
	vector<CString>		sz_uplink_e_timelst;	// アップリンク 時刻リスト
	long				l_rng;					// RNG 有無
	vector<CString>		sz_rng_s_timelst;		// RNG 時刻リスト
	vector<CString>		sz_rng_e_timelst;		// RNG 時刻リスト
	vector<CString>		sz_rng_kindlst;			// RNG 種別リスト
	long				l_cmd;					// CMD 有無
	vector<CString>		sz_cmd_s_timelst;		// CMD 時刻リスト
	vector<CString>		sz_cmd_e_timelst;		// CMD 時刻リスト
	CString				sz_cmd_type;
	long				l_tlms;					// TLM S 有無
	vector<CString>		sz_tlms_s_timelst;		// TLM S 時刻リスト
	vector<CString>		sz_tlms_e_timelst;		// TLM S 時刻リスト
	long				l_tlmx;					// TLM X 有無
	vector<CString>		sz_tlmx_s_timelst;		// TLM X 時刻リスト
	vector<CString>		sz_tlmx_e_timelst;		// TLM X 時刻リスト
	long				l_tlmka;				// TLM Ka 有無
	vector<CString>		sz_tlmka_s_timelst;		// TLM Ka 時刻リスト
	vector<CString>		sz_tlmka_e_timelst;		// TLM Ka 時刻リスト
	CString		sz_cid;							// CID
	CString		sz_station;						// 局名
	long		l_selene_4way;					// ４ＷＡＹ運用有無
	CString		sz_priority;					// 主従局
	CString		sz_ope_mode;					// 運用モード
	CString		sz_auto;						// 自動モード

	__int64		l_antwait_time;					// アンテナ待ち受け指向時刻
	__int64		l_antinit_time;					// アンテナ初期化時刻
	__int64		l_initmin_time;					// 設備初期設定最小時刻

	CString		sz_plan_type;					// 計画タイプ
};

#define SOS_HEADER_ID	_T("#!HEAD:")	/* ファイル管理識別 */

// ud_plan_rw モード
enum {
	READ_MODE,
	WRITE_MODE,
	PRINT_MODE,
	DEL_MODE,
	FORM_MODE,
};

#define XTX_Y_ID 1	/* "Y"指定ＩＤ値 */
#define XTX_N_ID 2  /* "N"指定ＩＤ値 */

class COprPlnData
{
public:
	COprPlnData(){};
	~COprPlnData(){};

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
#if 0	// コピーコンストラクタ
	COprPlnData &operator=(const COprPlnData &other)
	{
		// 自身の代入チェック
		if (this != &other) {
			m_strPlanID = other.m_strPlanID;
		}
		return *this;
	}
#endif
	void saveTime(CArchive& ar, stTime& val)
	{
		ar << val.start.GetTime();
		ar << val.end.GetTime();
	}
	void Save(CArchive& ar)
	{
		UINT i, size;
		ar << m_strPredID;
		ar << m_strPlanID;
		saveTime(ar, m_OpetDay);
		ar << m_UpLinkChk;
		ar << m_UpLinkCnt;
		size = (UINT)m_ULDay.size();
		ar << size;
		for (i = 0; i<size; i++){
			saveTime(ar, m_ULDay[i]);
		}
		ar << m_RngChk;
		ar << m_RngCnt;
		size = (UINT)m_RngDayList.size();
		ar << size;
		for (i = 0; i<size; i++){
			saveTime(ar, m_RngDayList[i]);
		}
		ar << m_CmdChk;
		ar << m_CmdCnt;
		size = (UINT)m_CmdDayList.size();
		ar << size;
		for (i = 0; i<size; i++){
			saveTime(ar, m_CmdDayList[i]);
		}
		ar << m_TlmSChk;
		saveTime(ar, m_TlmSDay);
		ar << m_TlmXChk;
		saveTime(ar, m_TlmXDay);
		ar << m_TlmKaChk;
		saveTime(ar, m_TlmKaDay);
		ar << m_AntStandby;
		ar << m_strAntStandbyTime;
		ar << m_AntZenith;
		ar << m_OpInit;
		ar << m_OpTDP;
		ar << m_OpRDP;
		ar << m_OpTOutType;
		ar << m_OpTOut;
		ar << m_AgcChkValue;
		ar << m_RndChkValue;
		ar << m_TlmChkValue;
		ar << m_CmdChkValue;
		ar << m_strCID;
		ar << m_strBRTLMS;
		ar << m_strBRTLMX;
		ar << m_strBRTLMKa;
		ar << m_strBRHTLM;
		ar << m_strBRCMDS;
		ar << m_strBRCMDX;
		saveTime(ar, m_AntFcstValue);
	}
	void loadTime(CArchive& ar, stTime& val)
	{
		__time64_t time;
		ar >> time;
		val.start = CTime(time);
		ar >> time;
		val.end = CTime(time);
	}
	void Load(CArchive& ar)
	{
		UINT i, size;
		ar >> m_strPredID;
		ar >> m_strPlanID;
		loadTime(ar, m_OpetDay);
		ar >> m_UpLinkChk;
		ar >> m_UpLinkCnt;
		ar >> size;
		m_ULDay.clear();
		for (i = 0; i<size; i++){
			stTime val;
			loadTime(ar, val);
			m_ULDay.push_back(val);
		}
		ar >> m_RngChk;
		ar >> m_RngCnt;
		ar >> size;
		m_RngDayList.clear();
		for (i = 0; i<size; i++){
			stTime val;
			loadTime(ar, val);
			m_RngDayList.push_back(val);
		}
		ar >> m_CmdChk;
		ar >> m_CmdCnt;
		ar >> size;
		m_CmdDayList.clear();
		for (i = 0; i<size; i++){
			stTime val;
			loadTime(ar, val);
			m_CmdDayList.push_back(val);
		}
		ar >> m_TlmSChk;
		loadTime(ar, m_TlmSDay);
		ar >> m_TlmXChk;
		loadTime(ar, m_TlmXDay);
		ar >> m_TlmKaChk;
		loadTime(ar, m_TlmKaDay);
		ar >> m_AntStandby;
		ar >> m_strAntStandbyTime;
		ar >> m_AntZenith;
		ar >> m_OpInit;
		ar >> m_OpTDP;
		ar >> m_OpRDP;
		ar >> m_OpTOutType;
		ar >> m_OpTOut;
		ar >> m_AgcChkValue;
		ar >> m_RndChkValue;
		ar >> m_TlmChkValue;
		ar >> m_CmdChkValue;
		ar >> m_strCID;
		ar >> m_strBRTLMS;
		ar >> m_strBRTLMX;
		ar >> m_strBRTLMKa;
		ar >> m_strBRHTLM;
		ar >> m_strBRCMDS;
		ar >> m_strBRCMDX;
		loadTime(ar, m_AntFcstValue);
	}
protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString			m_strPlanFilePath;	// 計画ファイルフルパス
	CString			m_strFtpPlanFilePath;	// FTP計画ファイルフルパス
	CString			m_strSatellite;		// 衛星名
	CString			m_strPredID;		// 予報値ID
	CString			m_strPlanID;		// 基本パラメータ-計画ID
	stTime			m_OpetDay;			// 基本パラメータ-運用開始終了日時
	BOOL			m_UpLinkChk;		// 基本パラメータ-アップリンク 有効/無効フラグ
	int				m_UpLinkCnt;		// 基本パラメータ-アップリンク 設定数
	vector<stTime>	m_ULDay;			// 基本パラメータ-アップリンク 開始終了日時リスト
	BOOL			m_RngChk;			// 基本パラメータ-RNG 有効/無効フラグ
	int				m_RngCnt;			// 基本パラメータ-RNG 設定数
	vector<stTime>	m_RngDayList;		// 基本パラメータ-RNG 開始終了日時リスト
	vector<CString>	m_RngTypeList;		// 基本パラメータ-RNG 種別リスト
	BOOL			m_CmdChk;			// 基本パラメータ-CMD 有効/無効フラグ
	int				m_CmdCnt;			// 基本パラメータ-CMD 設定数
	vector<stTime>	m_CmdDayList;		// 基本パラメータ-CMD 開始終了日時
	BOOL			m_TlmSChk;			// 基本パラメータ-TLM S 有効/無効フラグ
	stTime			m_TlmSDay;			// 基本パラメータ-TLM S 開始終了日時
	vector<stTime>	m_TlmSDayList;		// 基本パラメータ-TLM S 開始終了日時
	BOOL			m_TlmXChk;			// 基本パラメータ-TLM X 有効/無効フラグ
	stTime			m_TlmXDay;			// 基本パラメータ-TLM X  開始終了日時
	vector<stTime>	m_TlmXDayList;		// 基本パラメータ-TLM X  開始終了日時
	BOOL			m_TlmKaChk;			// 基本パラメータ-TLM Ka 有効/無効フラグ
	stTime			m_TlmKaDay;			// 基本パラメータ-TLM Ka 開始終了日時
	vector<stTime>	m_TlmKaDayList;		// 基本パラメータ-TLM Ka 開始終了日時

	int				m_AntStandby;		// ANT待ちうけ指向通知
	CString			m_strAntStandby;	// ANT待ちうけ指向通知 文字列
	CString			m_strAntStandbyTime;// ANT待ち受け指向通知 時間	
	int				m_AntZenith;		// ANT天頂志向有無
	CString			m_strAntZenith;		// ANT天頂志向有無 文字列

	int				m_OpInit;			// 運用オプション-初期設定
	int				m_OpTDP;			// 運用オプション-送信機ドップラ補償
	CString			m_OpTDPStr;			// 運用オプション-送信機ドップラ補償 文字列
	int				m_OpRDP;			// 運用オプション-受信機ドップラ補償
	int				m_OpTOutType;		// 運用オプション-送信出力タイプ
	int				m_OpTOut;			// 運用オプション-送信電力
	CString			m_OpTOutstr;		// 運用オプション-送信電力文字列
	CString			m_OpPriority;		// 主従局

	BOOL			m_AgcChkValue;		// レディネスチェック-AGC校正
	BOOL			m_RndChkValue;		// レディネスチェック-RNG校正
	BOOL			m_TlmChkValue;		// レディネスチェック-TLM転送
	BOOL			m_CmdChkValue;		// レディネスチェック-CMD伝送

	CString			m_strCID;			// CID（"CIDXXXXXXX"）
	CString			m_strBRTLMS;		// ビットレート-TLM S
	CString			m_strBRTLMX;		// ビットレート-TLM X
	CString			m_strBRTLMKa;		// ビットレート-TLM Ka	(計画ファイル入出力用)
	CString			m_strBRHTLM;		// ビットレート-高速TLM
	CString			m_strBRCMDS;		// ビットレート-CMD S
	CString			m_strBRCMDX;		// ビットレート-CMD X

	stTime			m_AntFcstValue;		// アンテナ予報値 (AOS,LOS)
	stTime			m_InitPassTime;		// 初期化時刻
	stTime			m_AntWaitTime;		// ANT待ち受け指向
	stTime			m_PrePassTime;		// プリパス（運用計画前の処理）時刻

	bool			mGetForm;			// true:enp false:sttn

	CString			m_strOpeMode;		// 運用モード
	CString			m_strAuto;			// 自動生成

	CString			m_strPlanType;		// 計画タイプ

	CTime			m_AgcTime;			// レディネスチェックAGC時刻
	CTime			m_RngTime;			// レディネスチェックRNG時刻
	CTime			m_TlmTime;			// レディネスチェックTLM時刻
	CTime			m_CmdTime;			// レディネスチェックCMD時刻

protected:

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */

public:
	static int WritePlanFile(CString sz_satname, CString sz_passno, stPlandata stpd, CString comment=_T(""));
	static int RegistPlanNew(CString sz_satname, CString sz_passno, BOOL bNew = TRUE);
	static int WriteRdyChkPlanFile(CString sz_satname, long satno, CString sz_passno, stPlandata stpd, vector<stCalibPlan>& agclist, vector<stCalibPlan>& tlmlist, vector<stCalibPlan>& cmdlist);
	static int ReadRdyChkPlanFile(CString sz_satname, CString sz_passno, stPlandata& stpd, vector<stCalibPlan>& agclist, vector<stCalibPlan>& tlmlist, vector<stCalibPlan>& cmdlist);

public:
	static int GetAntStandbyIdx(CString str);
	static int GetAntZenithIdx(CString str);
	static int GetOpInitIdx(CString str);
	static int GetOpTDPIdx(CString str);
	static int GetOpTDPIdx2(CString str);
	static int GetOpRDPIdx(CString str);
	static int GetOpTOutIdx(CString str, int type=0);

protected:

};


// 周波数制御画面データ管理クラス

// タイマ定義
#define TID_FREQMON			1
#define INTERVAL_FREQMON	800

// 設定周波数-U/C
static const CString FreqCtrlFreqUC = _T("X-UC.FREQUENCY");
static const CString FreqCtrlFreqUCCtrl = _T("X-UC.FREQUENCY");

// 設定周波数-D/C
static const CString FreqCtrlFreqDC = _T("S70-DC.MON_FREQ");
static const CString FreqCtrlFreqDCCtrl = _T("S70-DC.MON_FREQ");

// 設定周波数-コマンドサブキャリア
static const CString FreqCtrlFreqSC = _T("DSNTCR.TX_SC_FREQ");
static const CString FreqCtrlFreqSCCtrl = _T("DSNTCR.TX_SC_FREQ");

// オフセット-送信周波数オフセット
static const CString FreqCtrlOffSetTx = _T("XX-DSNTCRFC.FREQ_OFFSET");
static const CString FreqCtrlOffSetTxCtrl = _T("XX-DSNTCRFC.FREQ_OFFSET");

// オフセット-時刻オフセット
static const CString FreqCtrlOffSetTime = _T("XX-DSNTCRFC.TIME_OFFSET");
static const CString FreqCtrlOffSetTimeCtrl = _T("XX-DSNTCRFC.TIME_OFFSET");

// ドップラ補償-送信ドップラ
static const CString FreqCtrlDopplerTx = _T("CTRL.UP_FREQ_MODE");
static const CString FreqCtrlDopplerTxCtrl = _T("CTRL.UP_FREQ_MODE");

// ドップラ補償-受信ドップラ
static const CString FreqCtrlDopplerRx = _T("XX-DSNTCRFC.DOPP_MODE");
static const CString FreqCtrlDopplerRxCtrl = _T("XX-DSNTCRFC.DOPP_MODE");

// ドップラ補償-コマンドサブキャリア
static const CString FreqCtrlDopplerSC[] = {
	_T("CTRL.SC_AID_MODE"),	// 臼田64m局
	_T(""),					// 臼田54m局
	_T("CTRL.SC_AID_MODE"),	// 内之浦34m局
	_T("CTRL.SC_AID_MODE")	// 内之浦20m局
};
static const CString FreqCtrlDopplerSCCtrl[] = {
	_T("CTRL.SC_AID_MODE"),	// 臼田64m局
	_T(""),					// 臼田54m局
	_T("CTRL.SC_AID_MODE"),	// 内之浦34m局
	_T("CTRL.SC_AID_MODE")	// 内之浦20m局
};

// 探査機COHモード
static const CString FreqCtrlCohMode = _T("DSNTCR.TRP_RATIO");


// 送信ドップラ
enum{
	TXDOPP_A,
	TXDOPP_N_C,
	TXDOPP_N_T,
};
static const CString szTxDopp[] = {
	_T("あり"),
	_T("なし(センター)"),
	_T("なし(時刻指定)"),
	_T("")
};
// 受信ドップラ
enum{
	RXDOPP_1WAY,
	RXDOPP_2WAY,
	RXDOPP_3WAY,
};
static const CString szRxDopp[] = {
	_T("動的補償(1WAY)"),
	_T("動的補償(2WAY)"),
	_T("動的補償(3WAY)"),
	_T("")
};
// コマンドキャリア
static const CString szCmdCarr[] = {
	_T("動的補償"),
	_T("静的補償(センター)"),
	_T("静的補償(ノミナル)"),
	_T("静的補償(AOSベストロック)"),
	_T("")
};
// 探査機モード
static const CString szSatMode[] = {
	_T("INCOH"),
	_T("COH-880/749"), 
	_T("COH-3344/749"),
	_T("COH-3328/749"),
	_T("COH-3360/749"),
	_T("")
};
// 3WAY用他局送信周波数
enum {
	OTHER_SAT_34,
	OTHER_SAT_64,
	OTHER_SAT_54,
};
static const CString sz3WayOtherSta[] = {
	_T("34m"),
	_T("64m"),
	_T("54m"),
	_T("")
};



class CFreqCtrlData
{
public:
	CFreqCtrlData();
	~CFreqCtrlData();

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

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
protected:

};


// 送信制御画面データ管理クラス
#define	CTLNAME_LEN	28								/* 設備制御名称最大長+1 *//* [Ver1.02] 20->28byte */
#define	EQNAME_LEN	12								/* 設備名称最大長+1 */
#define	CTLID_LEN	( CTLNAME_LEN + EQNAME_LEN )	/* 制御項目ID最大長+1 */
//struct obsname_t
//{
//	long	l_id;					/* データｉｄ: 定数=OBSNAME_ID値 */
//	long	l_len;					/* データ長  : sizeof( struct obsname_t ) */
//	long	l_no;					/* 監視項目番号（コンパイラがシーケンシャルに付加する。） */
//	char	sz_obsname[CTLID_LEN]; 	/* 監視項目ＩＤ: 設備名.監視項目名 */
//	long	l_inflg;				/* データ有効フラグ 1=有効  ＊１ */
//	long	l_grmd;					/* ステータスフラグ 1=有効,2=時刻文字列 ＊１ */
//	long	l_data;					/* 変換結果（整数）         ＊１ */
//	double	d_data;					/* 変換結果（実数）         ＊１ */
//	char	sz_sts_name[64];		/* 変換結果ステータス文字列 */
//
//	__int64	i64_data;				/* 工学値データ64bit整数を格納できる精度 ＊１ */
//	long	l_bitinfo;				/* 整数ビット数:整数抽出データのビット長:1..64、==0:整数抽出ではない   ＊１ */
//	long	l__rfu__;				/* 未使用：paddingバイト */
//};

#define CTRL_SWEEP_NA        (long)0      /* 1shot-sweep */
#define CTRL_SWEEP_BEFORE    (long)1      /* 1shot-sweep */
#define CTRL_SWEEP_AFTER     (long)2      /* 1shot-sweep */

#define SWEEP_MODE_NON	0
#define SWEEP_MODE_SAW	1
#define SWEEP_MODE_TRI	2

#define TID_CONDITION	1
#define TID_COUNTDOWN	2

#define INTERVAL_CONDITION		800
#define INTERVAL_COUNTDOWN		1000

#define RNG_TYPE_NOR	1
#define RNG_TYPE_REP	2
#define RNG_TYPE_NON	-1

#define MON_STR_PASS     "パス中"			// パス運用中フラグ 監視パラメータ
#define MON_STR_NOPASS   "パス外"			// パス運用中フラグ 監視パラメータ

class CTrnCtrlData
{
public:
	CTrnCtrlData();
	~CTrnCtrlData();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CEngValCnv::obsname_t	m_obs[100];	// ダミー

protected:

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:

protected:

};


// 無線業務日誌画面データ管理クラス
class CBsnsDiaryData
{
public:
	CBsnsDiaryData();
	~CBsnsDiaryData();

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

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
protected:

};


// CID選択画面データ管理クラス

typedef struct {	// 衛星毎、CID選択項目
	vector<CString>	CID;				// CID
	vector<CString>	TlmBitRateS;		// テレメトリビットレート S帯
	vector<CString>	TlmBitRateX;		// テレメトリビットレート X帯
	vector<CString>	TlmBitRateHX;		// テレメトリビットレート X高速
	vector<CString>	TlmBitRateKa;		// テレメトリビットレート Ka帯
	vector<CString>	CmdBitRateS;		// コマンドビットレート S帯
	vector<CString>	CmdBitRateX;		// コマンドビットレート X帯
	vector<CString>	TrsPower;			// 送信電力
	vector<CString>	UpLinkSweepShape;	// アップリンク掃引パラメータ 形状
	vector<CString>	RngMesMode;			// レンジ計測パラメータ モード
	vector<CString>	CarrLoopBandS;		// キャリア受信ループバンド S帯
	vector<CString>	CarrLoopBandX;		// キャリア受信ループバンド X帯
	vector<CString>	CarrLoopBandKa;		// キャリア受信ループバンド Ka帯
	vector<CString>	CmdMod;				// コマンド変調度
	vector<CString>	CmdSubCarrFreq;		// コマンドサブキャリア周波数
	vector<CString>	TransDoppler;		// 送信ドップラ
	vector<CString>	AntAutoTrack;		// アンテナ自動追尾移行の有無
	vector<CString>	Modulation;			// 変調方式
	vector<CString>	ModulationS;		// 変調方式 S帯
	vector<CString>	ModulationX;		// 変調方式 X帯
	vector<CString>	AntAosPosn;			// アンテナ待ち受け指向の有無
	vector<CString>	AntWaitPoint;		// アンテナ待機位置の有無
}stCIDPrmEle;

typedef struct {	// CID毎パラメータ
	CString	CID;				// CID
	CString	TlmBitRateS;		// テレメトリビットレート S帯
	CString	TlmBitRateX;		// テレメトリビットレート X帯
	CString	TlmBitRateHX;		// テレメトリビットレート X高速
	CString	TlmBitRateKa;		// テレメトリビットレート Ka帯
	CString	CmdBitRateS;		// コマンドビットレート S帯
	CString	CmdBitRateX;		// コマンドビットレート X帯
	CString	TrsPower;			// 送信電力
	CString	CarrLoopBandS;		// キャリア受信ループバンド S帯
	CString	CarrLoopBandX;		// キャリア受信ループバンド X帯
	CString	CarrLoopBandKa;		// キャリア受信ループバンド Ka帯
	CString	UpLinkSweepShape;	// アップリンク掃引パラメータ 形状
	CString	UpLinkSweepWidth;	// アップリンク掃引パラメータ 幅
	CString	UpLinkSweepTime;	// アップリンク掃引パラメータ 時間
	CString	RngMesMode;			// レンジ計測パラメータ モード
	CString	RngMesSmplRate;		// レンジ計測パラメータ サンプリングレート
	CString	RngMesSmplItgTime;	// レンジ計測パラメータ 積分時間
	CString	RngMesSmplMod;		// レンジ計測パラメータ 変調度
	CString	Modulation;			// 変調方式 S帯
	CString	ModulationS;		// 変調方式 S帯
	CString	ModulationX;		// 変調方式 X帯
	CString	CmdMod;				// コマンド変調度
	CString	CmdSubCarrFreq;		// コマンドサブキャリア周波数
	CString	TransDoppler;		// 送信ドップラ
	CString	AntAutoTrack;		// アンテナ自動追尾
	CString	AntAosPosn;			// アンテナ待ち受け指向
	CString	AntWaitPoint;		// アンテナ待機位置指向
}stCIDParam;

typedef struct {	// CID毎パラメータ
	BOOL	CID;				// CID
	BOOL	TlmBitRateS;		// テレメトリビットレート S帯
	BOOL	TlmBitRateX;		// テレメトリビットレート X帯
	BOOL	TlmBitRateHX;		// テレメトリビットレート X高速
	BOOL	TlmBitRateKa;		// テレメトリビットレート Ka帯
	BOOL	CmdBitRateS;		// コマンドビットレート S帯
	BOOL	CmdBitRateX;		// コマンドビットレート X帯
	BOOL	TrsPower;			// 送信電力
	BOOL	CarrLoopBandS;		// キャリア受信ループバンド S帯
	BOOL	CarrLoopBandX;		// キャリア受信ループバンド X帯
	BOOL	CarrLoopBandKa;		// キャリア受信ループバンド Ka帯
	BOOL	UpLinkSweepShape;	// アップリンク掃引パラメータ 形状
	BOOL	UpLinkSweepWidth;	// アップリンク掃引パラメータ 幅
	BOOL	UpLinkSweepTime;	// アップリンク掃引パラメータ 時間
	BOOL	RngMesMode;			// レンジ計測パラメータ モード
	BOOL	Modulation;			// 変調方式 S帯
	BOOL	ModulationS;		// 変調方式 S帯
	BOOL	ModulationX;		// 変調方式 X帯
	BOOL	CmdMod;				// コマンド変調度
	BOOL	CmdSubCarrFreq;		// コマンドサブキャリア周波数
	BOOL	TransDoppler;		// 送信ドップラ
	BOOL	AntAutoTrack;		// アンテナ自動追尾
	BOOL	AntAosPosn;			// アンテナ待ち受け指向
	BOOL	AntWaitPoint;		// アンテナ待機位置指向
}stCIDPrmFlg;

#define CID_INVALID		_T("---------")

class CCIDSlctData
{
public:
	CCIDSlctData();
	~CCIDSlctData();

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
	vector<CString>							m_SatelliteNameList;	// 衛星名リスト
	map<CString, stCIDPrmEle>				m_CidElementList;		// 衛星毎CIDパラメータ要素
	map<CString, stCIDPrmFlg>				m_CidPrmFlgList;		// 衛星毎CIDパラメータ有効/無効フラグ
	map<CString, map<CString, stCIDParam>>	m_CidParamList;			// 衛星毎CID毎パラメータ設定値

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	vector<CString>&						GetSatelliteNameList() { return m_SatelliteNameList; }
	vector<CString>							GetCidList(CString sat);
	map<CString, stCIDPrmEle>&				GetCidElementList() { return m_CidElementList; }
	map<CString, map<CString, stCIDParam>>&	GetCidParamList() { return m_CidParamList; }
	map<CString, stCIDPrmFlg>&				GetCidPrmFlgList()	{ return m_CidPrmFlgList; }
	static stCIDParam						TrimInvalidString(stCIDParam stCid);

protected:

};



// トレンドプロパティメモリマップデータを管理するクラス
class CTrendPropertiesMemMapData
{
public:
	CTrendPropertiesMemMapData(void);
	~CTrendPropertiesMemMapData(void);

	DWORD GetPropertiesDataCnt() { return m_nPropertiesDataCnt; }


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
	CString	m_strMMDataFile;			// メモリマップデータファイル名
	HANDLE	m_hMMDataFile;				// メモリマップデータファイルハンドル
	HANDLE	m_hMMDataObject;			// メモリマップデータファイルマッピングオブジェクトハンドル
	LPVOID	m_pMMDataMapView;			// メモリマップデータファイルマップビュー
	vector<LPVOID>	m_listMMDataMap;	// メモリマップデータマップリスト
	DWORD	m_nPropertiesDataCnt;		// プロパティデータ数

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
protected:
	void	ClearParameter()
	{
		m_strMMDataFile.Empty();
		m_hMMDataFile = INVALID_HANDLE_VALUE;
		m_hMMDataObject = NULL;
		m_pMMDataMapView = NULL;
		m_listMMDataMap.clear();
	}

	bool	MappingUpdateMMDataFile(DWORD nMMDataMapView);

public:
	void	SetMMDataFileName(CString strMMDataFile)	{ m_strMMDataFile = strMMDataFile; }

	bool	IsOpenMMDataFile()	{ return (m_hMMDataFile != INVALID_HANDLE_VALUE) ? true : false; }

public:
	bool	OpenMMDataFile(bool flgRead = true);
	void	CloseMMDataFile();

	bool	WriteMMDataFile(CPropertiesData& propertiesData);
	bool	ReadMMDataFile(DWORD idx, CPropertiesData& propertiesData);
	/* ------------------------------------------------------------------------------------ */
};

#if 0
// トレンドプロパティメモリマップデータリストを管理するクラス
class CTrendPropertiesMemMapList
{
public:
	CTrendPropertiesMemMapList(void);
	~CTrendPropertiesMemMapList(void);


	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	enum eMMListStatus
	{
		eMMListStatus_None = 0,
		eMMListStatus_WriteOpen = 1,
		eMMListStatus_ReadOpen = 2
	};

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	eMMListStatus		m_eMMListStatus;		// メモリマップデータリストステータス
	vector<CTrendPropertiesMemMapData>	m_listMMData;			// データ用のメモリマップデータリスト
	map<CString, CTrendPropertiesMemMapData>	m_MapMMData;	// データ用のメモリマップデータリスト

	unsigned __int64	m_countData;			// 保存したデータ数
	CString				m_strMMInfoFile;		// メモリマップ情報ファイル名

protected:
	CString				m_strBaseName;			// ベースファイル名
	unsigned __int64	m_posRWPos;				// データの読み書きデータ位置(0～Primeデータ数-1：0～18,446,744,073,709,551,615)

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
protected:
	void	ClearParameter()
	{
		m_eMMListStatus = eMMListStatus_None;

		m_listMMData.clear();

		m_countData = 0;
		m_strMMInfoFile.Empty();

		m_strBaseName.Empty();
		m_posRWPos = 0;
	}

	void	SaveMMInfoFile()
	{
		int a = 0;
	}

	void	LoadMMInfoFile()
	{
		int a = 0;
	}

public:
	UINT	GetMaxSizePerMMData()	{ return MAX_SIZE_PER_MMDATA; }

	eMMListStatus	GetMemMapListStatus()	{ return m_eMMListStatus; }
	UINT	GetFileCount()	{ return (UINT)((m_countData + (GetMaxSizePerMMData() - 1)) / GetMaxSizePerMMData()); }
	unsigned __int64	GetDataCount()	{ return m_countData; }
	CString	GetMMInfoFileName()	{ return m_strMMInfoFile; }

public:
	bool	DeleteMemMapList();

	bool	WriteInitialization(CString strFileName);
	bool	WriteFinalization();
	bool	WriteData(CString strFileName, vector<CPropertiesData>& propertiesdataList);

	bool	ReadInitialization(CString strFileName);
	bool	ReadFinalization();
	bool	ReadData(CString strFileName, vector<CPropertiesData>& propertiesdataList);
	/* ------------------------------------------------------------------------------------ */
};
#endif

// 予報値リスト管理クラス
typedef struct {
	CString	strStationName;						// 局名
	CString strSatellite;						// 衛星名
	CString strPassId;							// パスID
	__int64 tAOS;								// AOS 日時
	double  dAosAz;								// AOS AZ
	double  dAosEl;								// AOS EL
	__int64 tLOS;								// LOS 日時
	double  dLosAz;								// LOS AZ
	double  dLosEl;								// LOS EL
	__int64 tMaxEl;								// Max EL 日時
	double  dMaxAz;								// Max AZ
	double  dMaxEl;								// Max EL
	CString strPredPath;						// 予報値格納パス
	long	tOffset;							// オフセット時刻
	CString strFormat;							// タイプ
	CString strGetFrom;							// 取得先
} stPredListData;


// 計画リスト管理クラス
typedef struct {
	CString	strStationName;						// 局名
	CString	strSatelliteName;					// 衛星名
	CString	strPassId;							// パスID
	__int64	tPrePassStart;						// プリパス開始日時
	__int64	tPassStart;							// パス開始日時
	__int64	tPassEnd;							// パス終了日時
	CString	strPredName;						// 予報値ファイル名
	CString strPlanName;						// 計画ファイル
	long	lValidFlag;							// チェック結果フラグ
	long	lOfsFlag;							// 時刻オフセット追従条件フラグ
	long	tOffset;							// 時刻オフセット
	CString	strGetFrom;							// 取得先
	CString	strPlanType;						// 計画種別
	CString strResult;							// 実行結果
	CString strMaintComment;					// MAINT用コメント
	CString strPostPass;						// ポストパス（ＡＮＴ天頂指向）
} stPlanListData;


// ホスト装置名定義
#define STTN_OPR_HOST  "k3opr1\n"		// USC 局運用管制装置ホスト名
#define SSOC_OPR_HOST  "szopr2\n"		// SSOC局運用管制装置ホスト名

#define	DEV_NO_STTN_OPR		0xe1		// 出先局側局運用管制装置
#define	DEV_NO_SSOC_OPR		0xe2		// 相模原側局運用管制装置

#define DATA_ID_POS 6                   // データ種別位置
#define SEND_ID_POS 4                   // 局運用管制装置用要求元機能識別位置
#define REAL_DATA_POS 12                // 送信実データ開始位置
#define DATA_NO_USE_CHAR NULL           // 送信データ未使用部分設定値
#define LF_CHAR 0x0a                    // LF文字コード

// op_ipc.h >>>
#define MSG_ALLOC       1
#define MAX_SDTP		(32*1024)
#define SDTP_INFO		24
#define SDTP_HED		32
#define SDTP_INFO_FIELD 12
#define DATA_PDU_K		0x010020b0		// データＰＤＵ
// <<< op_ipc.h

// dns_def.h >>>
#define	DEV_NO_EQU_CTRL		0xc0					// 設備制御装置
#define	INFO_ID_DAT_CTLEQU	(unsigned char)(0x02)	// 設備制御データ
#define	INFO_SENDID_STTN    0x10					// 出先側局運用管制装置内機能
#define	INFO_SENDID_SSOC    0x11					// 相模原局運用管制装置内機能
#define	INFO_NO_SEND		0x00					// 送信元なし
// <<< dns_def.h

// sdtp.h >>>
#define SD_OPR_CONTROL		0x0007		// UDSC64 & KSC34 設備制御
// <<< sdtp.h

// optcmesg.h >>>
#define DEFAULT_SEND_IPC   0x8403		// 送信用
// <<< optcmesg.h

struct  sdtp_msg_t {					// sdtp メッセージテーブル
//	long        l_unix_msgtype;			// メッセージタイプ(64)
	long long   l_unix_msgtype;			// メッセージタイプ(64)
	long        l_len;					// 送信バイト数
	long        l_objtype;				// 要求ＩＤ
	long        l_blkcnt;				// ブロックカウント
	long        l_send_ipc;				// 送信側メッセージキューＩＤ
	long        l_recv_ipc;				// 受信側メッセージキューＩＤ
	unsigned char uca_SdtpDummy[26];	// 受信側メッセージキューＩＤ
	unsigned char uca_SdtpPDULen[4];	// PDU長
	unsigned char uca_SdtpRecNum[2];	// レコード数
	unsigned char uca_buff[MAX_SDTP];	// データバッファ
};

/*                                   1   2   3   4   5   6   7   8   9  10  11  12 */
static const long l_chkday[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

#pragma pack(1)
// 初期プロパティ
typedef struct
{
	int		leapyear;	// うるう年（★衛星に関係ない）
	int		leapsec;	// うるう秒（★衛星に関係ない）
	double	gain;		// 地上局ANT利得（★衛星に関係ない）
	int		autocalc;	// 予想受信レベル自動計算選択
	double	reclevels;	// 予想受信レベル（固定値）
	double	reclevelx;	// 予想受信レベル（固定値）
	double	eirp;		// 衛星送信EIRP
	bool	tlmflag;	// TLMサブキャリア
	double	tlm;		// TLMサブキャリア
	bool	cmdflag;	// CMDサブキャリア
	double	cmd;		// CMDサブキャリア
} stInitProp;

// 運用プロパティ
typedef struct
{
	int		event_autocalc;	// イベント日時の自動計算
	DWORD	even_sunset;	// 天頂通過角度入り
	DWORD	event_sunrise;	// 天頂通過角度明け
	DWORD	mg_ULon;		// マージン角度U/L ON
	DWORD	mg_ULoff;		// マージン角度U/L OFF
	DWORD	mg_DL;			// マージン角度D/L
	DWORD	of_CMD;			// U/L終了前の時刻オフセット CMD運用終了
	DWORD	of_RNG;			// U/L終了前の時刻オフセット RNG運用終了
	char	cid[30];		// デフォルトCID
	int		opespace;		// 衛星運用CMD/RNG同時変調
	stInitProp	init;	// 初期プロパティ
} stOpeProp;
#pragma pack()

/* ------------------------------------------------------------------------------------ */
/* 管制データ管理クラス
/* ------------------------------------------------------------------------------------ */
class CSatelliteData
{
public:
	CSatelliteData();
	~CSatelliteData();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	union   umoji {                         /* 文字列先頭2文字のバッファ */
		char   c_moji[2];
		short  s_moji;
	};


	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	//	vector<CRdnsChkData>		m_listRdnsChkData;													// 校正診断画面データ
	CRdnsChkData				m_CRdnsChkData;														// 校正診断画面データ
	map<CString, vector<COprPlnData>> m_listOprPlnData;													// 計画立案画面データ
	CFreqCtrlData				m_listFreqCtrlData;														// 周波数制御画面データ
	CTrnCtrlData				m_TrnCtrlData;															// 送信制御画面データ
	CBsnsDiaryData				m_listBsnsDiaryData;													// 無線業務日誌画面データ
	CCIDSlctData				m_CIDSlctData;															// CID選択画面データ
	CTrackingRadarCtrlMonData	m_arrayTrackingRadarCtrlMonData[eStation_MAX + 2];						// 追尾レーダー制御監視データ配列（サマリx4局, 予報値一覧、計画一覧 ）
//	CTrackingRadarCtrlMonData	m_dummyTrackingRadarCtrlMonData[eStation_MAX + 2];						// 追尾レーダー制御監視ダミーデータ（サマリx4局, 予報値一覧、計画一覧 ）
	CSpectrumViewData			m_spectrumData;															// スペクトラムデータ
	CSpectrumViewData			m_arraySummarySpectrumData[eStation_MAX][MAX_SUMMARY_VIEW];				// スペクトラムデータ（サマリx4局 x ビュー数用意）
	CPropertiesData				m_arrayTrendPropertiesData[eTrendWndType_Num];							// トレンドプロパティデータ配列（グラフ種類数用意）
	CPropertiesData				m_arraySummaryPropertiesData[eStation_MAX][MAX_SUMMARY_VIEW];			// サマリプロパティデータ配列（サマリx4局 x ビュー数用意）
	vector<stPredListData>		m_listPredList;															// 予報値一覧データ
	vector<stPlanListData>	m_listPlanList;																// 計画一覧データ
	CString						m_strSelectedForecastFilePath;											// 選択した予報値ファイル名
	CTrendData					m_trendData;															// 衛星捕捉データ
	CSessionInfo				m_sessionInfo[eStation_MAX];											// セッション情報
	CRangePropagation			m_rngPropagation;														// RNG伝搬

	struct sat_inf_t satinfo;																			// 衛星本体情報テーブル
	struct sat_inf_t m_satinfo[TBLMAX];																	// 衛星本体情報テーブル

	CTime						mGetSharePredFileTime[eStation_MAX];
	CTime						mGetSharePlanFileTime[eStation_MAX];

	FILETIME					m_SessionFileTime;														// セッションファイル時刻

	CRFTripAngleFile			m_arrayRFTripData[eStation_MAX];										// RFトリップデータ

//	CTrendPropertiesMemMapList	m_trendPropMemMapList;													// トレンドプロパティメモリマップデータリスト

protected:
	sdtp_msg_t	m_st_sdtp_msg;			// SDTP通信用構造体

	CDBAccess	m_dbaccess;				// DB アクセスクラス

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	static double ub_asc_to_dtime(char* sz_time);
	static int ub_asc_to_asctime(char sz_input_time[], char sz_output_time[]);
	static int ub_asc_to_asctime(TCHAR sz_input_time[], TCHAR sz_output_time[]);
	static void ub_dtime_to_asc(double d_uttime, char* sz_time);
//	static int ua_strmatch(char* string, char* pattern);
//	static long ub_getln(FILE* fp, long* lp_line, char* argv[], long argc);
//	static int ub_fgetconf(TCHAR* sz_filename, char* keyword, char* formatstring, 
//							char* arg1 = NULL, char* arg2 = NULL, char* arg3 = NULL, char* arg4 = NULL, char* arg5 = NULL, char* arg6 = NULL, char* arg7 = NULL, char* arg8 = NULL);

	static int MovetHeaderInFile(FILE* fp, TCHAR* strId);
	static int GetConfFromFile(FILE* fp, TCHAR* strId, long* val);
	static int GetConfFromFile(FILE* fp, TCHAR* strId, double* val);

	CPropertiesData&	GetTrendPropertiesData(int pos)
	{
		if (pos < 0 || pos >= eTrendWndType_Num)
			return m_arrayTrendPropertiesData[0];
		return m_arrayTrendPropertiesData[pos];
	}

	void				SetTrendPropertiesData(int pos, CPropertiesData val)	{ m_arrayTrendPropertiesData[pos] = val; }

	CPropertiesData&	GetSummaryPropertiesData(int nStationIndex, int pos)
	{
		if (nStationIndex < 0 || nStationIndex >= eStation_MAX || pos < 0 || pos >= MAX_SUMMARY_VIEW)
			return m_arraySummaryPropertiesData[0][0];
		return m_arraySummaryPropertiesData[nStationIndex][pos];
	}

	void				SetSummaryPropertiesData(int nStationIndex, int pos, CPropertiesData val) { m_arraySummaryPropertiesData[nStationIndex][pos] = val; }

	void				CreateSummaryPropertiesData();

	CSpectrumViewData& GetSpectrumViewData()	{ return m_spectrumData; }

	CSpectrumViewData& GetSummarySpectrumViewData(int nStationIndex, int pos)
	{
		if (pos < 0 || pos >= eStation_MAX)
			return m_arraySummarySpectrumData[0][0];
		return m_arraySummarySpectrumData[nStationIndex][pos];
	}

	void SetSummarySpectrumStation()
	{
		for (int i = 0; i < eStation_MAX; i++)
		{
			for (int j = 0; j < MAX_SUMMARY_VIEW; j++)
			{
				CSpectrumViewData& spectrum = GetSummarySpectrumViewData(i, j);
				spectrum.SetStation(i);
			}
		}
	}

//	static CString AddCanma(double pos, int pointKeta = 0);
	static CString AddCanma(double pos, int pointKeta, int singFlag = 0);
	void SpectrumInitialize();
	void SpectrumInitialize(UINT nStationID);
	int SpectrumFileRead(UINT nStationID);
	int SpectrumFileWrite(UINT nStationID);

//	CTrendPropertiesMemMapList& GetTrendPropertiesMemMapList()	{ return m_trendPropMemMapList; }
	void TrendPropInitialize();

	bool	TrendPropWriteData(UINT nStationID);

	bool	TrendPropReadData(UINT nStationID);

	CTrendData& GetTrendData()		{ return m_trendData; }

//	BOOL	SetPredListDataList();
//	BOOL	SetPredList_Utf8();
//	BOOL	SetPredList_Utf8(UINT nStation);
	BOOL	SetPredList_Euc();
	BOOL	SetPredList_Euc(UINT nStation);
	BOOL	IsPredListFileUpdated(UINT nStation);
	static CTime	StrTimeToCTime(CString strTime);

	vector<stPredListData>&	GetPredList() { return m_listPredList; }				// 予報値一覧データ

//	BOOL SetPlanList();
//	BOOL SetPlanList_Utf8();
//	BOOL SetPlanList_Utf8(UINT nStation);
	BOOL SetPlanList_Euc();
	BOOL SetPlanList_Euc(UINT nStation);
	BOOL IsPlanListFileUpdated(UINT nStation);

	vector<stPlanListData>&	GetPlanList() { return m_listPlanList; }				// 計画一覧データ取得
	BOOL IsUsePredFile(CString& strFilePath);

	CString opctproc(CString strCtrl, CString strParam, CString val = _T(""));
	void opctproc2(CString strCtrl, CString strParam);

	void SendControlSelect(CString strCtrl, stControlValue* pval, CString strCtrl2 = _T(""));
	int RegistControlSelect(CString strCtrl, stControlValue* pval);
	int param_chk(CString strCtrl, stControlValue* pval = NULL, CString strAddStatus = _T(""), BOOL bSeqCtrl = FALSE, CString strCtrl2 = _T(""));
	int param_chk2(CString strCtrl, stControlValue* pval, CString strCtrl2, stControlValue* pval2);
	bool GetEquipmentParam(CString &strCtrl, struct paramtbl_t *param_adr[], struct val_t *val_adr[], stEquipment2Ctrl *stEquip, double fval);
	int get_paramlist(CString strCtrl, vector<CString> &list);

	BOOL IsPllBwCtrl(CString strCtrl);

	BOOL IsBitRateCtrl(CString strCtrl);
	BOOL IsCmdBitRateCtrl(CString strCtrl);
	CString CtrlBitRate(CString strCtrl, stControlValue* pval = NULL, BOOL bSeqCtrl = FALSE);

	CTrackingRadarCtrlMonData& GetTrackingRadarCtrlMonData(int pos)
	{ 
		if (pos < 0 || pos >= eStation_MAX + 2)
			return m_arrayTrackingRadarCtrlMonData[0];
		return m_arrayTrackingRadarCtrlMonData[pos];
	}

	CRFTripAngleFile& GetRFTripAngleData(int pos)
	{
		if (pos < 0 || pos >= eStation_MAX)
			return m_arrayRFTripData[0];
		return m_arrayRFTripData[pos];
	}
	void ReadRFTripFile();

	UINT	GetSimpleStationID(CString name);
	UINT	GetStationID(CString name);

	BOOL ReadCIDFile();

	void	GetShareFile(UINT val=0xffffffff);

	inline const bool CompareData(stPredListData& val1, stPredListData& val2)
	{
		if (val1.strStationName != val2.strStationName)	return true;
		if (val1.strSatellite != val2.strSatellite)	return true;
		if (val1.strPassId != val2.strPassId)	return true;
		if (val1.tAOS != val2.tAOS)	return true;
		if ((__int64)(val1.dAosAz*100.0) != (__int64)(val2.dAosAz*100.0))	return true;
		if ((__int64)(val1.dAosEl*100.0) != (__int64)(val2.dAosEl*100.0))	return true;
		if (val1.tLOS != val2.tLOS)	return true;
		if ((__int64)(val1.dLosAz*100.0) != (__int64)(val2.dLosAz*100.0))	return true;
		if ((__int64)(val1.dLosEl*100.0) != (__int64)(val2.dLosEl*100.0))	return true;
		if (val1.dLosEl != val2.dLosEl)	return true;
		if (val1.tMaxEl != val2.tMaxEl)	return true;
		if ((__int64)(val1.dMaxAz*100.0) != (__int64)(val2.dMaxAz*100.0))	return true;
		if ((__int64)(val1.dMaxEl*100.0) != (__int64)(val2.dMaxEl*100.0))	return true;
		if (val1.strPredPath != val2.strPredPath)	return true;
		if (val1.tOffset != val2.tOffset)	return true;
		if (val1.strFormat != val2.strFormat)	return true;
		if (val1.strGetFrom != val2.strGetFrom)	return true;
		return false;
	}
	inline const bool CompareData(stPlanListData& val1, stPlanListData& val2)
	{
		if (val1.strStationName != val2.strStationName)	return true;
		if (val1.strSatelliteName != val2.strSatelliteName)	return true;
		if (val1.strPassId != val2.strPassId)	return true;
		if (val1.tPrePassStart != val2.tPrePassStart)	return true;
		if (val1.tPassStart != val2.tPassStart)	return true;
		if (val1.tPassEnd != val2.tPassEnd)	return true;
		if (val1.strPredName != val2.strPredName)	return true;
		if (val1.lValidFlag != val2.lValidFlag)	return true;
		if (val1.lOfsFlag != val2.lOfsFlag)	return true;
		if (val1.tOffset != val2.tOffset)	return true;
		if (val1.strGetFrom != val2.strGetFrom)	return true;
		if (val1.strPlanType != val2.strPlanType)	return true;
		if (val1.strResult != val2.strResult)	return true;
		return false;
	}

	CSessionInfo& GetSessionInfo(int pos)
	{
		if (pos < 0 || pos >= eStation_MAX)
			return m_sessionInfo[0];
		return m_sessionInfo[pos];
	}
	BOOL ReadSessionStatusFile();
	void DeleteSessionStatusFile();

	CRangePropagation& GetRangePropagation()	{ return m_rngPropagation; }

	int stn_tbl_access();
	CDBAccess& GetDBAccessCls() { return m_dbaccess; }
	int opctcheck(struct paramtbl_t *param_adr, struct val_t *val_adr, char sz_indata[], int i_signflag);
	int uc_valchk(char *s, int *i_p, double *d_p, int l_opt);

	// レイアウト読み込み＆保存（設定情報）
	bool	Load(CArchive& ar);
	bool	Save(CArchive& ar);

	// プロパティファイル読込
	int ReadPropertyFile(CString strSat, stOpeProp &stProp);

	// 計画ファイル帳票出力用テンポラリファイル作成
	typedef struct {
		CString strDate;
		CTime	tDate;
		CString strEventName;
		CString strCtrl;
		CString strPrm;
	}stPlnoutDetail;
	CString	CreatePlnPrintFile(CString fname);

	static void ExcelOutPut(HWND hwnd, CString param);
	static int ExcelOutPutProc(HWND hwnd, CString param, BOOL bWaitComplete = TRUE);

protected:
	void AddDataVector(vector<CString> &v, CString str);
};


/* ------------------------------------------------------------------------------------ */
/* StationControlとOperationTimelineとの監視データ
/* ------------------------------------------------------------------------------------ */
class CMonitorDataTemp
{
public:
	CMonitorDataTemp(){};
	~CMonitorDataTemp(){};

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:


	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	vector<CEngValCnv::obs_tbl_res_t>	mData;

protected:

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:

protected:

};