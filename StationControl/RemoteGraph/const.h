#pragma once
//#pragma pack(1)

using namespace std;

//=============================================================================
//	ビルドオプション
//=============================================================================

//#define	_TEST_MAXDATA			1				// 最終テーブルを3時間として試験
//#define	_TEST_LEAP				1				// うるう秒を強制に発生させて試験

#ifdef	_TEST_MAXDATA
#define	HighSpeedDemo		1					// 高速デモ
#endif	// 	_TEST_MAXDATA

#ifndef _PIPE_COLLECTION
//#define	HighSpeedDemo		1					// 高速デモ
#endif	// _PIPE_COLLECTION

#ifdef	_PIPE_COLLECTION
#ifdef	_DEBUG
//#define	DemoTimeStamp		1					// デモパイプ通信(受信データに現在時刻をスタンプ)
//#define	DemoStepData			1					// デモ階段波形データ通信
//#define	DemoSaigen			1					// 縦軸目盛り表示不正再現
//#define DemoSin			1						// デモSin波形
#endif	// _DEBUG
#endif	// _PIPE_COLLECTION

//=============================================================================
//	定数
//=============================================================================
#define MAX_TDATA	10
#define MAX_VIEW	 8
#define MAX_VSCALE	 2
#define MAX_HSCALE	 2

#define	DREAL_NAN_HIGH		0x7ff80000
#define	DREAL_NAN_LOW		0x00000000
#define	DREAL_ERROR_HIGH	0xffffffff
#define	DREAL_ERROR_LOW		0xfffffffe

#define MAX_OUTPUT_LIST		100

#define	MAX_USEDATA_DAYS	200					// アプリが扱う非圧縮データの最大日数(単位：日)
#define	DATA_BLOCK_UNIT		8					// アプリが扱う非圧縮データのブロック単位(単位：時間)
#define	DATA_BLOCK_RANGE	1					// アプリが扱う非圧縮データのブロック範囲(単位：個数)
#define	MAX_HOLDDATA_TIME	(DATA_BLOCK_UNIT + (DATA_BLOCK_UNIT * (DATA_BLOCK_RANGE * 2)))	// メモリ上に保持する非圧縮データの最大時間(単位：時間)
#define MAX_HOLDDATA_SIZE	(3600 * MAX_HOLDDATA_TIME)										// メモリ上に保持する非圧縮データの最大データ数(＝秒数)
#define MAX_ALLDATA_SIZE	(3600 * 24 * MAX_USEDATA_DAYS)									// アプリが扱う非圧縮データの最大データ数(＝秒数)
#define MAX_COMPDATA_SIZE	(3600 + 1)			// 圧縮データ保持サイズ（ウィンドウ右端位置のデータ表示のため、+1する）
#define MAX_DATASELECT		80
#define	MAX_LIMIT_HISTORY	10					// リミット履歴数

#ifdef	_TEST_MAXDATA
#define	MAX_SIZE_PER_MMDATA		3600	// メモリマップデータあたりの最大測定データ数(１時間分の秒数＝60×60)
										// １ファイルのサイズ：3600個×40バイト＝144000バイト(約140KB)
#else	// 	_TEST_MAXDATA
#define	MAX_SIZE_PER_MMDATA		86400	// メモリマップデータあたりの最大測定データ数(１日分の秒数＝60×60×24)
										// １ファイルのサイズ：86400個×40バイト＝3456000バイト(約3MB)
#endif	// 	_TEST_MAXDATA

#define VECMAX			256		/* ＤＢ検索最大個数（パラメータ数も同意。） */
#define	OBSNAME_ID		0x0101		/* 条件定義ＩＤ */

#define	CTLNAME_LEN	28	/* 設備制御名称最大長+1 *//* [Ver1.02] 20->28byte */
#define	EQNAME_LEN	12	/* 設備名称最大長+1 */
#define	CTLID_LEN	( CTLNAME_LEN + EQNAME_LEN )	/* 制御項目ID最大長+1 */

#define	FILE_VERSION		101				// INIファイルのバージョン

#define	DISK_FREE_SPACE		10				// ディスクの空き許容容量

#define	REALTIME_FNAME		_T("__tmp_RealTime")
#define	ANALYSIS_FNAME		_T("__tmp_Analysis")

//=============================================================================
//	プロパティ
//=============================================================================
//#define	AXIS_MINMAX_RANGE		1000000000			// 縦軸の最大幅
//#define	AXIS_MINMAX_VALUE		999999999.999999	// 縦軸の最大値
#define	AXIS_MINMAX_RANGE		10000000000000			// 縦軸の最大幅
#define	AXIS_MINMAX_VALUE		9999999999999.0			// 縦軸の最大値
#define	AXIS_SCALE_MIN			2					// 目盛りの分割数の最小値
#define	AXIS_SCALE_MAX			10					// 目盛りの分割数の最大値
//#define	LIMIT_MINMAX_RANGE		1000000000			// リミットの最大幅
//#define	LIMIT_MINMAX_VALUE		999999999.999999	// リミットの最大値
#define	LIMIT_MINMAX_RANGE		10000000000000			// リミットの最大幅
#define	LIMIT_MINMAX_VALUE		9999999999999.0			// リミットの最大値

//=============================================================================
//	INIファイル
//=============================================================================
const CString SECTION_SYSTEM	= _T("SYSTEM");
const CString KEY_LOGLEVEL		= _T("LogLevel");		// ログレベル
const CString KEY_SERVER		= _T("Server");			// サーバーアドレス
const CString KEY_SERVERPATH	= _T("ServerPath");		// サーバーデータ格納パス
const CString KEY_FILEVERSION	= _T("FileVersion");	// ファイルバージョン
const CString KEY_DATANAMEOUT	= _T("DataNameOut");	// データ名を出力
const CString KEY_CHECKDISKFREESPACE = _T("CheckDiskFreeSpace");	// ディスク空き容量チェック

const CString SECTION_DATALIST	= _T("DATALIST");
const CString KEY_DATANAMENUM	= _T("DataNameNumber");	// 監視名の登録数
const CString KEY_DATANAME		= _T("DataName%d");		// 監視名

const CString SECTION_PROPDATA	= _T("PROPDATA");
const CString KEY_REALTIME		= _T("Realtime");
const CString KEY_VSCL_AUTO		= _T("VScaleAuto");
const CString KEY_VSCL_LMAX		= _T("VScaleLMax");
const CString KEY_VSCL_LMIN		= _T("VScaleLMin");
const CString KEY_VSCL_RMAX		= _T("VScaleRMax");
const CString KEY_VSCL_RMIN		= _T("VScaleRMin");
const CString KEY_VSCL_ITVL		= _T("VScaleItvl");
const CString KEY_HSCL_NUM		= _T("HScaleNumber%d");
const CString KEY_DATA_NAME		= _T("DataName%d");
const CString KEY_DATA_GNAME	= _T("DataGraphName%d");
const CString KEY_DATA_UNIT		= _T("DataUnit%d");
const CString KEY_GRAPH_VMODE	= _T("GraphViewMode%d");
const CString KEY_GRAPH_DMODE	= _T("GraphDrawMode%d");
const CString KEY_GRAPH_COL		= _T("GraphColor%d");
const CString KEY_GRAPH_DOTSZ	= _T("GraphDotSize%d");
const CString KEY_GRAPH_VSKIND	= _T("GraphVScaleKind%d");
const CString KEY_COM_STR		= _T("CommentString%d");
const CString KEY_COM_DISP		= _T("CommentDisplay%d");
const CString KEY_LMT_WAR_U_V	= _T("LimitWarningUpperValid%d");
const CString KEY_LMT_WAR_U		= _T("LimitWarningUpper%d");
const CString KEY_LMT_WAR_L_V	= _T("LimitWarningLowerValid%d");
const CString KEY_LMT_WAR_L		= _T("LimitWarningLower%d");
const CString KEY_LMT_ALM_U_V	= _T("LimitAlarmUpperValid%d");
const CString KEY_LMT_ALM_U		= _T("LimitAlarmUpper%d");
const CString KEY_LMT_ALM_L_V	= _T("LimitAlarmLowerValid%d");
const CString KEY_LMT_ALM_L		= _T("LimitAlarmLower%d");

enum eStationMode{
	eStation_1,
	eStation_5,
	eStation_10,
};

enum eStation{
	eStation_Usuda64,
	eStation_Usuda642,
	eStation_Uchinoura34,
	eStation_Uchinoura20,
	eStation_MAX,
};

static const TCHAR* mStationFolder[]{
	_T("udsc64"),
	_T("udsc54"),
	_T("usc34"),
	_T("usc20"),
};

static const TCHAR* mStationSimpleString[]{
	_T("UDSC64"),
	_T("UDSC54"),
	_T("USC34"),
	_T("USC20"),
};

// ファイルタイプ
enum eFileType{
	// データベース
	eFileType_DB_Eqctl_File,
	eFileType_DB_Eqdsp_File,
	eFileType_DB_Eqinf_File,
	eFileType_DB_Eqmon_File,
	eFileType_DB_Eqsat_File,

	// 設備監視データ（生データ）フォルダ
	eFileType_Equipment_Folder,

	// 計画雛形フォルダ
	eFileType_Form_Folder,

	// 計画フォルダ
	eFileType_Plan_Folder,

	// 計画出力フォルダ
	eFileType_PlanOut_Folder,

	// 計画一覧ファイル
	eFileType_PlanList_File,

	// 予報値フォルダ
	eFileType_Pred_Folder,

	// 予報値一覧ファイル
	eFileType_PredList_File,

	// CIDフォルダ
	eFileType_CID_Folder,

	// 業務日誌/パス履歴フォルダ
	eFileType_Diary_Folder,

	// 業務日誌ファイル
	eFileType_Diary_File,

	// 業務日誌送信情報ファイル
	eFileType_Diary_File_Info,

	// RF情報ファイル
	eFileType_RfInfo_File,

	// 運用履歴フォルダ
	eFileType_OpHistory_Folder,

	// 制御履歴フォルダ
	eFileType_CtrlHistory_Folder,

	// 設備履歴フォルダ
	eFileType_EquipHistory_Folder,

	// リミットエラーフォルダ
	eFileType_Limit_Folder,

	// イベント履歴フォルダ
	eFileType_EventHistory_Folder,

	// アンテナ設備駆動履歴フォルダ
	eFileType_AntEquipDriveHistory_Folder,

	// アンテナ設備運用履歴フォルダ
	eFileType_AntEquipOpHistory_Folder,

	// RFトリップフォルダ
	eFileType_RFTrip_Folder,

	// RFトリップファイル
	eFileType_RFTrip_25w_File,
	eFileType_RFTrip_100w_File,
	eFileType_RFTrip_200w_File,
	eFileType_RFTrip_1kw_File,
	eFileType_RFTrip_2kw_File,
	eFileType_RFTrip_20kw_File,

	// スカイラインフォルダ
	eFileType_Skyline_Folder,

	// スカイラインファイル
	eFileType_Skyline_File,

	// 追尾予報値フォルダ
	eFileType_Yoho_Folder,

	// 追尾予報値ファイル
	eFileType_Yoho_File,

	// 逐次制御定義フォルダ
	eFileType_SeqCtrl_Folder,

	// レディネスチェック時間ファイル
	eFileType_Rdychk_File,

	// リモートシェルフォルダ
	eFileType_Remsh,

	// プロパティファイル
	eFileType_Property,

	// 局フォルダ
	eFileType_Station_Folder,

	// 監視データダンプ・定義フォルダ
	eFileType_MonDump_Def,

	// 監視データダンプ・出力フォルダ
	eFileType_MonDump_Ftp,

	// AGC校正テーブルフォルダ
	eFileType_AGCCalib_Folder,

	// ファイル・フォルダ定義数
	eFileType_Max
};

// ファイル・フォルダパス
static const TCHAR* mSvFileFolder[] =
{
	// データベースファイル
	_T("ctrl\\db\\ld\\eqctl.db.obj"),
	_T("ctrl\\db\\ld\\eqdsp.db.obj"),
	_T("ctrl\\db\\ld\\eqinf.db.obj"),
	_T("ctrl\\db\\ld\\eqmon.db.obj"),
	_T("ctrl\\db\\ld\\eqsat.db.obj"),

	// 設備監視データ（生データ）フォルダ
	_T("log\\mon\\"),

	// 計画雛形フォルダ
	_T("ctrl\\plan\\form\\"),

	// 計画フォルダ
	_T("ctrl\\plan\\exec\\"),

	// 計画出力フォルダ
	_T("ctrl\\plan\\ftp\\"),

	// 計画一覧ファイル
	_T("ctrl\\plan\\exec\\plan_list.txt"),

	// 予報値フォルダ
	_T("ctrl\\prd\\"),

	// 予報値一覧ファイル
	_T("ctrl\\prd\\pred_list.txt"),

	// CIDフォルダ
	_T("ctrl\\db\\cid\\"),

	// 業務日誌/パス履歴フォルダ
	_T("ctrl\\log\\pass\\"),

	// 業務日誌ファイル
	_T("ctrl\\log\\pass\\radio_report.txt"),

	// 業務日誌送信情報ファイル
	_T("ctrl\\log\\pass\\rf_info_report.txt"),

	// RF情報ファイル
	_T("ctrl\\log\\pass\\rarf_report.txt"),

	// 運用履歴フォルダ(ファイル名：Operation_YYYYMMDD.txt)
	_T("log\\opr\\"),

	// 制御履歴フォルダ(ファイル名：Contrl_YYYYMMDD.txt)
	_T("log\\ctrl\\"),

	// 設備履歴フォルダ(ファイル名：Equipment_YYYYMMDD.txt)
	_T("log\\moni\\"),

	// リミットエラーフォルダ
	_T("log\\limit\\"),

	// イベント履歴フォルダ(ファイル名：Event_YYYYMMDD.txt)
	_T("log\\event\\"),

	// アンテナ設備駆動履歴フォルダ
	_T("ctrl\\log\\ant\\"),

	// アンテナ設備運用履歴フォルダ
	_T("ctrl\\log\\ant\\"),

	// RFトリップフォルダ
	_T("cfg\\etc\\"),

	// RFトリップファイル
	_T("cfg\\etc\\ct_rftrip_25w.dat"),
	_T("cfg\\etc\\ct_rftrip_100w.dat"),
	_T("cfg\\etc\\ct_rftrip_200w.dat"),
	_T("cfg\\etc\\ct_rftrip_1kw.dat"),
	_T("cfg\\etc\\ct_rftrip_2kw.dat"),
	_T("cfg\\etc\\ct_rftrip_20kw.dat"),

	// スカイラインフォルダ
	_T("cfg\\etc\\"),

	// スカイラインファイル
	_T("cfg\\etc\\ct_skyline.dat"),

	// 追尾予報値フォルダ
	_T("cfg\\etc\\grph_yofile\\"),

	// 追尾予報値ファイル
	_T("cfg\\etc\\grph_yofile\\opgt_yofile"),

	// 逐次制御定義フォルダ
	_T("cfg\\etc\\cont\\def\\"),

	// レディネスチェック時間ファイル
	_T("cfg\\etc\\RDY_TIME.txt"),

	// リモートシェルフォルダ
	_T(""),

	// プロパティファイル
	_T(""),

	// 局フォルダ
	_T(""),

	// 監視データダンプ・定義フォルダ
	_T("log\\dump\\cfg\\"),

	// 監視データダンプ・出力フォルダ
	_T("log\\dump\\"),

	// AGC校正テーブルフォルダ
	_T("ctrl\\agc\\"),

	// ファイル・フォルダ定義数
	_T(""),
};

// デバッグ用ローカルファイル・フォルダパス
static const TCHAR* mLocalFileFolder[] =
{
	// データベースファイル
	_T("ctrl\\db\\ld\\eqctl.db.obj"),
	_T("ctrl\\db\\ld\\eqdsp.db.obj"),
	_T("ctrl\\db\\ld\\eqinf.db.obj"),
	_T("ctrl\\db\\ld\\eqmon.db.obj"),
	_T("ctrl\\db\\ld\\eqsat.db.obj"),

	// 設備監視データ（生データ）フォルダ
	_T("log\\mon\\"),

	// 計画雛形フォルダ
	_T("ctrl\\plan\\form\\"),

	// 計画フォルダ
	_T("ctrl\\plan\\exec\\"),

	// 計画出力フォルダ
	_T("ctrl\\plan\\ftp\\"),

	// 計画一覧ファイル
	_T("ctrl\\plan\\exec\\plan_list.txt"),

	// 予報値フォルダ
	_T("ctrl\\prd\\"),

	// 予報値一覧ファイル
	_T("ctrl\\prd\\pred_list.txt"),

	// CIDフォルダ
	_T("ctrl\\db\\cid\\"),

	// 業務日誌/パス履歴フォルダ
	_T("ctrl\\log\\pass\\"),

	// 業務日誌ファイル
	_T("ctrl\\log\\pass\\radio_report.txt"),

	// 業務日誌送信情報ファイル
	_T("ctrl\\log\\pass\\rf_info_report.txt"),

	// RF情報ファイル
	_T("ctrl\\log\\pass\\rarf_report.txt"),

	// 運用履歴フォルダ(ファイル名：Operation_YYYYMMDD.txt)
	_T("log\\opr\\"),

	// 制御履歴フォルダ(ファイル名：Contrl_YYYYMMDD.txt)
	_T("log\\ctrl\\"),

	// 設備履歴フォルダ(ファイル名：Equipment_YYYYMMDD.txt)
	_T("log\\moni\\"),

	// リミットエラーフォルダ
	_T("log\\limit\\"),

	// イベント履歴フォルダ(ファイル名：Event_YYYYMMDD.txt)
	_T("log\\event\\"),

	// アンテナ設備駆動履歴フォルダ
	_T("ctrl\\log\\ant\\"),

	// アンテナ設備運用履歴フォルダ
	_T("ctrl\\log\\ant\\"),

	// RFトリップフォルダ
	_T("cfg\\etc\\"),

	// RFトリップファイル
	_T("cfg\\etc\\ct_rftrip_25w.dat"),
	_T("cfg\\etc\\ct_rftrip_100w.dat"),
	_T("cfg\\etc\\ct_rftrip_200w.dat"),
	_T("cfg\\etc\\ct_rftrip_1kw.dat"),
	_T("cfg\\etc\\ct_rftrip_2kw.dat"),
	_T("cfg\\etc\\ct_rftrip_20kw.dat"),

	// スカイラインフォルダ
	_T("cfg\\etc\\"),

	// スカイラインファイル
	_T("cfg\\etc\\ct_skyline.dat"),

	// 追尾予報値フォルダ
	_T("cfg\\etc\\grph_yofile\\"),

	// 追尾予報値ファイル
	_T("cfg\\etc\\grph_yofile\\opgt_yofile"),

	// 逐次制御定義フォルダ
	_T("cfg\\etc\\cont\\def\\"),

	// レディネスチェック時間ファイル
	_T("cfg\\etc\\RDY_TIME.txt"),

	// リモートシェルフォルダ
	_T(""),

	// プロパティファイル
	_T(""),

	// 局フォルダ
	_T(""),

	// 監視データダンプ・定義フォルダ
	_T("log\\dump\\cfg\\"),

	// 監視データダンプ・出力フォルダ
	_T("log\\dump\\"),

	// AGC校正テーブルフォルダ
	_T("ctrl\\agc\\"),

	// ファイル・フォルダ定義数
	_T(""),
};

enum eMessagex{
	// ウィンドウ系
	eMessage_ChildActivate = (WM_USER + 100),
	eMessage_ChildDestroy,
	eMessage_ChangeActiveTab,
	eMessage_InitializeApplication,
	eMessage_UpdateGraph,
	eMessage_AnalysisKeyEvent,
	eMessage_UpdateZoomRectangle,
	eMessage_UpdateCursorLine,
	eMessage_MoveZoomRectangle,
	eMessage_CreateZoomRectangle,
	eMessage_MoveCursorLine,
	eMessage_CreateCursorLine,
	eMessage_CommentChange,
	eMessage_DifferChange,
//	eMessage_CloseWaitDialog,

	// スレッド系
	eMessage_ThreadNormal,
	eMessage_ThreadError,
	eMessage_ThreadFail,
	eMessage_ThreadException,

	// プロパティ系
	eMessage_PropetyChange,
	eMessage_PropetyCmbChange,
	eMessage_AutoClear,
};

enum {
	grapf1,
	grapf2,
	grapf3,
	grapf4,
	grapf5,
	grapf6,
	grapf7,
	grapf8,
	grapf9,

	grapfmax,
};

typedef enum {
	Line,
	Dots,
	LineAndDots,
}DrawMode;

typedef enum {
	Small,
	Medium,
	Large,
}DotSize;

typedef enum {
	second,
	minute,
	hour,
}AxisUnit;

typedef enum {
	LScale,
	RScale,
	VScaleKind_Max
}VScaleKind;

typedef enum {
	LimitWarning,	// ワーニング
	LimitAlarm,		// アラーム
	LimitKind_Max
}LimitKind;

typedef enum {
	LimitUpper,		// 上限
	LimitLower,		// 下限
}LimitRange;

typedef enum {
	LimitAbove,		// ～より上回る
	LimitBelow,		// ～より下回る
}LimitStatus;

typedef struct {
	CTime	time;
	ULONG	state;
} stLimitState;

#define	MAKE_LIMIT_STATE(k, r, s)	(((s) << 16) | ((r) << 8) | ((k) << 0))
#define	GET_LIMIT_KIND(state)		(((state) >>  0) & 0xff)
#define	GET_LIMIT_RANGE(state)		(((state) >>  8) & 0xff)
#define	GET_LIMIT_STATUS(state)		(((state) >> 16) & 0xff)

typedef enum {
	DifferNone,		// なし
	DifferSameData,	// 同一データ
	DifferSameTime	// 同一時刻
}DifferMode;

#define	DIFFER_ADD	10
#define	DIFFER_TEXT	50

typedef struct {
	bool	Auto;
	double	VScalLMax;
	double	VScalLMin;
	double	VScalRMax;
	double	VScalRMin;
	long	VSCalItvl;
} stVScale;

#ifdef	_TEST_MAXDATA
typedef enum {
	m1,
	m10,
	m30,
	h1,
	h3,
	autoscale
}HScale;

static const CString strHScale[] = {
	_T("1分"),
	_T("10分"),
	_T("30分"),
	_T("1時間"),
	_T("3時間"),
	_T("オート"),
	_T(""),
};
#else	// 	_TEST_MAXDATA
typedef enum {
	m1,
	m10,
	m30,
	h1,
	h3,
	h6,
	h12,
	h24,
	h48,
	h96,
	h120,
	h240,
	h480,
	h960,
	h1200,
	h2400,
	h4800,
	autoscale
}HScale;

static const CString strHScale[] = {
	_T("1分"),
	_T("10分"),
	_T("30分"),
	_T("1時間"),
	_T("3時間"),
	_T("6時間"),
	_T("12時間"),
	_T("24時間"),
	_T("48時間"),
	_T("96時間"),
	_T("120時間"),
	_T("240時間"),
	_T("480時間"),
	_T("960時間"),
	_T("1200時間"),
	_T("2400時間"),
	_T("4800時間"),
	_T("オート"),
	_T(""),
};
#endif	// 	_TEST_MAXDATA

static const CString strDrawMode[] = {
	_T("Line"),
	_T("Dots"),
	_T("L&D"),
	_T(""),
};

static COLORREF colordef[] =
{
	RGB(0, 0, 255),		// blue
	RGB(255, 0, 0),		// red
	RGB(255, 0, 255),	// magenta
	RGB(0, 255, 0),		// green
	RGB(0, 255, 255),	// cyan
	RGB(255, 255, 0),	// yellow
	RGB(255, 255, 255),	// white
	RGB(255, 165, 0),	// orange
	RGB(165, 42, 42),	// brown
	RGB(50, 205, 50),	// lime
	NULL
};

typedef struct {
	CString		Auto;
	CString		startDate;
	double		VScalLMax;
	double		VScalLMin;
	double		VScalRMax;
	double		VScalRMin;
	long		VSCalItvl;
	CString		HScaleU;
	CString		HScaleL;
	CString		ViewMode;
	CString		Display;
	CString		DrawKind;
	CString		DotSize;
	CString		SelVScale;
	CString		dataUnit;
	double		LimitWarningUpper;
	double		LimitWarningLower;
	double		LimitAlarmUpper;
	double		LimitAlarmLower;
} stPropData;

static const stPropData initPropData = {
	_T("On"),
	_T(""),
	100,
	-100,
	50,
	-50,
	5,
	_T("24時間"),
	_T("1時間"),
	_T("リアルモード"),
	_T("On"),
	_T("Line"),
	_T("Small"),
	_T("左"),
	_T(""),
	0,
	0,
	0,
	0,
};

//=============================================================================
//	監視データ
//=============================================================================

enum eSpaceDataStatus
{
	SDS_INVALID	= 0x0000,

	SDS_VALID	= 0x0001,

	SDS_LEAP	= 0x0002,
	SDS_LEAPMIN	= 0x0004,
	SDS_LEAPMAX	= 0x0008
};

#pragma pack(1)
typedef struct {
	USHORT	cnt;
	double	max;
	double	min;
} stMMPair;

typedef struct  {
	SYSTEMTIME	systime;	// 時刻
	USHORT		status;		// ステータス
	stMMPair	mmPair;		// ピーク圧縮
} stSpaceData;
#pragma pack()

typedef struct {
	unsigned __int64	start_pos;	// 描画位置
	unsigned __int64	data_num;	// 描画点数
	unsigned __int64	update_pos;	// 更新開始位置
	bool				change_flg;	// 100%⇒90%への変更フラグ
} stDrawInfo;

typedef struct {
	DWORD	id;			// データID
	CString	name;		// データ名称
	bool	valid;		// データ取得対象フラグ
} stSelectData;

typedef struct {
	CString	name;		// データ名称
	stSpaceData	data;	// データ
} stQueData;

enum eStatus{
	eStatus_None,		// データ無し
	eStatus_Enable,		// データ有効
	eStatus_Disable,	// データ無効（例えばデータ抜け等）
};

class CSpaceData
{
public:
	CSpaceData()
	{
		value.clear();
		value_for_copy.clear();
		data_max = -DBL_MAX;
		data_min = DBL_MAX;
	};
	virtual ~CSpaceData(){};

public:
	DWORD			dataid;		// データID
	DWORD			kind;		// データ種別
	double			data_max;	// 最大値
	double			data_min;	// 最小値
	__int64			totalSize;	// キュー削除前のトータルサイズ
	deque<stSpaceData>	value;	// 生トレンドデータキュー
	vector<stSpaceData>	value_for_copy;	// コピー用トレンドデータ
};

typedef struct {
	DWORD		dataid;		// データID
	SYSTEMTIME	systime;	// 時刻
} stDataIdTime;

//=============================================================================
//	スケール情報
//=============================================================================

typedef struct {
	unsigned short		cmp_rate;		// 圧縮数
	unsigned __int64	all_data;		// 全点数
	unsigned __int64	hld_data;		// 保持点数
	unsigned long		dsp_data;		// 表示点数
	bool				flg_auto;		// AUTO用スケール
} stScaleInfo;

#ifdef	_TEST_MAXDATA
const stScaleInfo	tblRealScaleInfo[]={
	{	   1,	MAX_ALLDATA_SIZE		/*17280000*/,	3601,	  61,	false	},	// １分
	{	   1,	MAX_ALLDATA_SIZE		/*17280000*/,	3601,	 601,	false	},	// １０分
	{	   1,	MAX_ALLDATA_SIZE		/*17280000*/,	3601,	1801,	false	},	// ３０分
	{	   1,	MAX_ALLDATA_SIZE		/*17280000*/,	3601,	3601,	false	},	// １時間
	{	   3,	MAX_ALLDATA_SIZE /    3	/* 5760000*/,	3600,	3601,	false	},	// ３時間
};
#else	// _TEST_MAXDATA
const stScaleInfo	tblRealScaleInfo[]={
	{	   1,	MAX_ALLDATA_SIZE		/*17280000*/,	3601,	  61,	false	},	// １分
	{	   1,	MAX_ALLDATA_SIZE		/*17280000*/,	3601,	 601,	false	},	// １０分
	{	   1,	MAX_ALLDATA_SIZE		/*17280000*/,	3601,	1801,	false	},	// ３０分
	{	   1,	MAX_ALLDATA_SIZE		/*17280000*/,	3601,	3601,	false	},	// １時間
	{	   3,	MAX_ALLDATA_SIZE /    3	/* 5760000*/,	3601,	3601,	false	},	// ３時間
	{	   6,	MAX_ALLDATA_SIZE /    6	/* 2880000*/,	3601,	3601,	false	},	// ６時間
	{	  12,	MAX_ALLDATA_SIZE /   12	/* 1440000*/,	3601,	3601,	false	},	// １２時間
	{	  24,	MAX_ALLDATA_SIZE /   24	/*  720000*/,	3601,	3601,	false	},	// ２４時間
	{	  48,	MAX_ALLDATA_SIZE /   48	/*  360000*/,	3601,	3601,	true	},	// ４８時間
	{	  96,	MAX_ALLDATA_SIZE /   96	/*  180000*/,	3601,	3601,	true	},	// ９６時間
	{	 120,	MAX_ALLDATA_SIZE /  120	/*  144000*/,	3601,	3601,	true	},	// １２０時間
	{	 240,	MAX_ALLDATA_SIZE /  240	/*   72000*/,	3601,	3601,	true	},	// ２４０時間
	{	 480,	MAX_ALLDATA_SIZE /  480	/*   36000*/,	3601,	3601,	true	},	// ４８０時間
	{	 960,	MAX_ALLDATA_SIZE /  960	/*   18000*/,	3601,	3601,	true	},	// ９６０時間
	{	1200,	MAX_ALLDATA_SIZE / 1200	/*   14400*/,	3601,	3601,	true	},	// １２００時間
	{	2400,	MAX_ALLDATA_SIZE / 2400	/*    7200*/,	3601,	3601,	true	},	// ２４００時間
	{	4800,	MAX_ALLDATA_SIZE / 4800	/*    3600*/,	3600,	3601,	true	}	// ４８００時間
};
#endif	// _TEST_MAXDATA

#define	NUM_REAL_SCALE_INFO_TABLE		(sizeof(tblRealScaleInfo) / sizeof(tblRealScaleInfo[0]))

/*
設備制御情報ＤＢ
*/

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
#define CARVTBL_ID		0x010a		/* カーブ変換定義ＩＤ */
#define TIMECVT_ID		0x010b		/* 時刻変換定義ＩＤ */
#define OBSSTS_ID		0x010c		/* ステータス定義ＩＤ */
#define CAL_ID			0x0112		/* 他項目計算定義ＩＤ */
#define	EQMON_INF_ID	0x0121		/* "inf":付加情報定義を意味するＩＤ */
#define ENDOFOBS_ID 	-1			/* 個々のコマンドの定義の終了ＩＤ */
#define L_OFFSET        6

#define	CTLNAME_LEN	28	/* 設備制御名称最大長+1 *//* [Ver1.02] 20->28byte */
#define	EQNAME_LEN	12	/* 設備名称最大長+1 */
#define	CTLID_LEN	( CTLNAME_LEN + EQNAME_LEN )	/* 制御項目ID最大長+1 */
#define	CHKVALDBL_NUM	2	/* 検証値：実数型の場合の配列数 */
#define	CHKVALSTR_LEN	( CHKVALDBL_NUM * ( sizeof(double) / sizeof(char)  ))
//#define CHKVALSTR_LEN   ( 6 * CHKVALDBL_NUM * ( sizeof(double) /sizeof(char)  ))

/*
*******************************************************************************
* 設備監視名称情報
*******************************************************************************
*/
struct obsname_t {	/* このテーブルは全監視項目について存在する */
	long   l_id;	/* データｉｄ: 定数=OBSNAME_ID値 */
	long   l_len;	/* データ長  : sizeof( struct obsname_t ) */
	long   l_no;	/* 監視項目番号（コンパイラがシーケンシャルに付加する。） */
	char   sz_obsname[CTLID_LEN]; 	/* 監視項目ＩＤ: 設備名.監視項目名 */
	long   l_inflg; /* データ有効フラグ 1=有効  ＊１ */
	long   l_grmd;  /* ステータスフラグ 1=有効,2=時刻文字列 ＊１ */
	long   l_data;  /* 変換結果（整数）         ＊１ */
	double d_data;  /* 変換結果（実数）         ＊１ */
	char   sz_sts_name[64]; /* 変換結果ステータス文字列 */

	/* 以下は64bit整数精度対応拡張領域 */

	long double ld_data;/* 工学値データ64bit整数を格納できる精度 ＊１ */

	long   l_bitinfo;   /* 整数ビット数:整数抽出データのビット長:1..64
						==0:整数抽出ではない   ＊１ */
	long   l__rfu__;    /* 未使用：paddingバイト */

	long   l_cnvResType;	/* 変換結果タイプ（ビット並び） */
};

/*
*******************************************************************************
* 抽出定義情報
*******************************************************************************
*/
typedef struct pwb_tbl_t {	/* 抽出定義テーブル上に存在する。 */
	long l_pid;	/* 装置番号 */
	long l_word;	/* ワード位置 */
	long l_s_bit;  /* 開始ビット位置 */
	long l_e_bit;  /* 終了ビット位置 */
} PWB_T;

/*
*******************************************************************************
* 各抽出定義情報
*******************************************************************************
*/
struct ext_tbl_t{	/* 各抽出定義情報 */
	long l_id;	/* データｉｄ:
				EXT_ID or EXTS_ID or
				EXTBCD_ID or EXTBCDS_ID or
				EXTAVG_ID or EXTAVGS_ID or
				EXTCHAR_ID or EXTCHARDEC or
				EXTDBL or EXTFLT */
	long l_len;	/* データ長  : sizeof( struct ext_tbl_t ) */
	long l_pw_cnt;	/* 抽出定義個数 */
	long l_rsv;	/* サイズ調整 */
	pwb_tbl_t s_pwb[10];/* 抽出定義情報 */
};

/*
*******************************************************************************
* ステータス照合定義
*******************************************************************************
*/
typedef struct chksts_tbl_t {	/* ステータス照合定義テーブル */
	long l_val;	/* 項目値 */
	long l_rsv;	/* サイズ調整 */
	char sz_sts[CHKVALSTR_LEN];/* ステータス文字列 */
} C_STS;

/*
*******************************************************************************
* ステータス情報
*******************************************************************************
*/
struct obssts_tbl_t {	/* ステータス情報テーブル */
	long l_id;	/* データｉｄ: 定数=OBSSTS_ID値 */
	long l_len;	/* データ長  : sizeof( struct obssts_tbl_t ) */
	long l_sts_cnt; /* ステータス項目数 */
	long l_rsv;	/* サイズ調整 */
	char sz_i_sts[CHKVALSTR_LEN];/* 初期値ステータス文字列 */
	chksts_tbl_t chk_t[32]; /* ステータス変換項目 */
};

struct res_t { /*共有メモリ上のテーブル構造*/
	long  l_id;
	long  l_len;
	char  buf[1];
};

