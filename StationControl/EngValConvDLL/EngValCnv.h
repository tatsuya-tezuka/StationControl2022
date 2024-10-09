
//---------------------------------------------
// 工学値変換用クラス
//---------------------------------------------
#pragma once
#include <tchar.h>
#include <vector>
#include <map>
#include <deque>
#include <WinSock2.h>

using namespace std;

/* ### ＰＤＵ種別 ### */
#define DATA_PDU         0x010030b0      /* データＰＤＵ */

#define VEQU_ID   0xf


#define TLMDATAINFSIZE (12)      /*「情報フィールド」のバイト数*/
#define TLMDATAHEDSIZE (12+64)   /*「情報フィールド」＋「付加情報」のバイト数*/

#define  SHMFLG          0666
#define  SIZEINF_SIZE    8
#define  EOFINF_SIZE     4

#define  LOG             0x0001
#define  START_LOG       2801
#define  END_LOG         2802
#define  FILESIZEERR_LOG 2803

#define  VMAX            1024


#define    U_LIB_PRIVATE  uc_valchk

/*
ローカル define宣言部
*/
#define   LX         0x5830                /* 0X */
#define   SX         0x7830                /* 0x */
#define   LB         0x4230                /* 0B */
#define   SB         0x6230                /* 0b */
#define   LT         0x5430                /* 0T */
#define   ST         0x7430                /* 0t */

/* DBがビックエンディアンか否か 1:ビック 0:リトル */
#define IS_BIG_ENDIA 0

#define EQCTL_DB_SHM_ID		1	/* 設備制御情報データベース */
#define EQMON_DB_SHM_ID		2	/* 設備制御情報データベース */
#define EQSAT_DB_SHM_ID		3	/* 衛星固有情報データベース */
#define EQDSP_DB_SHM_ID		4	/* 画面情報定義データベース */
#define EQINF_DB_SHM_ID		5	/* 設備情報定義データベース */
#define EQINI_DB_SHM_ID		6	/* 設備初期化情報データベース */

/*
設備制御情報ＤＢ
*/

#define	CTLNAME_LEN	28	/* 設備制御名称最大長+1 *//* [Ver1.02] 20->28byte */
#define	EQNAME_LEN	12	/* 設備名称最大長+1 */
#define	CTLID_LEN	( CTLNAME_LEN + EQNAME_LEN )	/* 制御項目ID最大長+1 */
#define	CHKVALDBL_NUM	2	/* 検証値：実数型の場合の配列数 */
#define	CHKVALSTR_LEN	( CHKVALDBL_NUM * ( sizeof(double) / sizeof(char)  ))
//#define CHKVALSTR_LEN   ( 6 * CHKVALDBL_NUM * ( sizeof(double) /sizeof(char)  ))

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

#define _GREAT_ENGVAL
#ifdef _GREAT_ENGVAL
/* GREAT 工学値変換の改修 */
#define LOG10_ID		0x0115		/* 常用対数(log10)変換定義ＩＤ */
#define EXP10_ID		0x0116		/* 常用対数(log10)逆変換定義ＩＤ */
#define ARCTAN_ID		0x0117		/* 逆正接(arctan)変換定義ＩＤ */
#endif /* _GREAT_ENGVAL */

#define	EQMON_INF_ID	0x0121		/* "inf":付加情報定義を意味するＩＤ */
#define ENDOFOBS_ID 	-1			/* 個々のコマンドの定義の終了ＩＤ */
#define L_OFFSET        6

#define CARV_MAX        32          /* カーブテーブルの最大数 */

#define ON         1     /*モード(有り)*/
#define OFF        0     /*モード(なし)*/
#define OK         0

#define iNULL 0  /* integerのNULL: NULLはポインタ型なので定義*/ 

#define OBS_MAX    8192
#define MAX_SDTP  (32*1024)
#define STR_BUF_SIZE 256

#define YYYY_MAX 2069 /* 2038年が未来の最大値:時の終わり -> 2069年に変更 */

#define LOW3BIT  0x0007  /* 下位3ﾋﾞｯﾄ取りだし */

#define PCTIME_SPAN100		(60 * 60 * 24 * 100)	// PC時間の差異100日
#define PCTIME_SPAN365		(60 * 60 * 24 * 365)	// PC時間の差異365日

enum{
	eTimeDirect,
	eTimeRange,
};

#pragma pack(1)

#ifdef _ORG
struct TL_TLMDATA {  /*「情報フィールド＋付加情報＋テレメトリフレーム」 */
	/* 情報フィールド */
	unsigned char uc_recid;         /* レコードＩＤ %1 */
	unsigned char uca_dataid[7];    /* データＩＤ
									以下の２バイトはue_tlmmsgsnd
									->ue_msgrcvで設定している。
									dataid[0]:CPNサービス種:=0x08
									dataid[1]:VCID値
									*/
	long          l_reclen;         /* データフィールドのデータ長、つまり、
									「付加情報」のバイト数＋テレメトリ受信
									データのバイト数 %1 */
	/* 付加情報部 */
	unsigned char uc_ver;           /* 装置管理番号・バージョン番号 %3 */
	unsigned char uc_scno;          /* 衛星番号 %3 */

	unsigned char uca_bcdyyyy[2];   /*BCDフォーマットの年(4桁):1994..2001..%2*/
	unsigned char uca_bcddate[2];   /*BCDフォーマットの通算日:0001..0366 %2*/
	unsigned char uca_bcdtime[5];   /*BCDフォーマットの時刻  :HHMMSSmmmu %2*/
	unsigned char uc_bcdtime_uu;    /*BCDフォーマットの時刻  :uu %2*/

	unsigned char uc_bnd;           /* 受信バンド帯 %3 */
	unsigned char uc_fsts1;         /* 復調装置ステータス（その１）%1 */
	unsigned char uc_fsts2;         /* 復調装置ステータス（その２）%1 */
	unsigned char uc_locksts;       /* ロックステータス %1 */
	unsigned char uc_bitrate;       /* ビットレート %2 */
	unsigned char uca_psno[5];      /* パス番号 (BCDフォーマット) %3 */
	unsigned char uca_psno_yy;      /* パス番号 (BCDフォーマット):
									00:従来形式パス番号であることを示す
									19..20:年の上位２桁 %3 */
	unsigned char uc_antid;         /* 受信アンテナＩＤ %3 */
	unsigned char uca_space[40];    /* 予備 */

	/* テレメトリ受信データ（最大2048バイト）*/
	// 以下の配列と取り合えず100k確保する
	unsigned char uca_tlmfrm[1024 * 100];
	//unsigned char uca_tlmfrm[1024 * 2];
	/*衛星受信データ（テレメトリデータ）
	注意：この部分は衛星毎の可変長データです。Ｃコンパイラの都合で
	可変長データの定義が出来ないのでサイズ2048バイトになって
	いますが、実際は、128,512,2048バイト等になります。
	（l_reclenから「付加情報」のバイト数を引いた長さになります。）

	（したがって、tlm1rec_t/TL_TLMDATA形式のデータを配列構造に
	する場合にＣ言語のコーディング上は配列の参照方法でアクセス
	するのは間違いです（この間違いはコンパイル時にもエラーに
	ならない）ので注意して下さい）
	*/
	/*
	情報設定元：
	%1:テレメトリ入力ドライバ(/dev/tlmdem)+HWからの入力データそのもの
	%2:テレメトリ取り込みプロセス(tirtproc)
	%3:テレメトリ入力分配プロセス(tiinproc)
	%4:SDTPサーバプロセス(sdsvproc)
	*/
};
#else
typedef struct { // 情報フィールド
	unsigned char uc_recid;			// レコードＩＤ
	unsigned char uca_dataid[7];	// データＩＤ
	/* 以下の２バイトはue_tlmmsgsnd
	->ue_msgrcvで設定している。
	dataid[0]:CPNサービス種:=0x08
	dataid[1]:VCID値
	*/
	long          l_reclen;			// データフィールドのデータ長
	/*
	「付加情報」のバイト数＋テレメトリ受信データのバイト数
	*/
} xTL_INFO;

typedef struct { // 付加情報
	unsigned char uc_ver;			// 装置管理番号・バージョン番号 %3
	unsigned char uc_scno;			// 衛星番号 %3

	unsigned char uca_bcdyyyy[2];	// BCDフォーマットの年(4桁):1994..2001..%2
	unsigned char uca_bcddate[2];	// BCDフォーマットの通算日:0001..0366 %2
	unsigned char uca_bcdtime[5];	// BCDフォーマットの時刻  :HHMMSSmmmu %2
	unsigned char uc_bcdtime_uu;	// BCDフォーマットの時刻  :uu %2

	unsigned char uc_bnd;			// 受信バンド帯 %3
	unsigned char uc_fsts1;			// 復調装置ステータス（その１）%1
	unsigned char uc_fsts2;			// 復調装置ステータス（その２）%1
	unsigned char uc_locksts;		// ロックステータス %1
	unsigned char uc_bitrate;		// ビットレート %2
	unsigned char uca_psno[5];		// パス番号 (BCDフォーマット) %3
	unsigned char uca_psno_yy;		// パス番号 (BCDフォーマット)
	/*
	00:従来形式パス番号であることを示す
	19..20:年の上位２桁 %3 */
	unsigned char uc_antid;			// 受信アンテナＩＤ %3

	//unsigned char uca_space[40];	// 予備
	unsigned char uca_space[22];	// 予備
	unsigned char uc_op_rec_id;		// 情報フィールド・レコード識別
	unsigned char uca_op_data_id[7]; // 情報フィールド・データｉｄ
	unsigned char uca_op_rec_len[4]; // 局運用：情報フィールド・レコード長
	unsigned char uca_timetag[6];	// タイムタグ: 0D DD HH MM  SS ss
} xTL_ADDINFO;

typedef struct { // テレメトリ受信データ（可変）
	unsigned char uca_tlmfrm[1];
	/*衛星受信データ（テレメトリデータ）
	注意：この部分は衛星毎の可変長データです。Ｃコンパイラの都合で
	可変長データの定義が出来ないのでサイズ2048バイトになって
	いますが、実際は、128,512,2048バイト等になります。
	（l_reclenから「付加情報」のバイト数を引いた長さになります。）

	（したがって、tlm1rec_t/TL_TLMDATA形式のデータを配列構造に
	する場合にＣ言語のコーディング上は配列の参照方法でアクセス
	するのは間違いです（この間違いはコンパイル時にもエラーに
	ならない）ので注意して下さい）
	*/
	/*
	情報設定元：
	%1:テレメトリ入力ドライバ(/dev/tlmdem)+HWからの入力データそのもの
	%2:テレメトリ取り込みプロセス(tirtproc)
	%3:テレメトリ入力分配プロセス(tiinproc)
	%4:SDTPサーバプロセス(sdsvproc)
	*/
} xTL_RECDATA;

typedef struct {  /*「情報フィールド＋付加情報＋テレメトリフレーム」 */
	xTL_INFO		info;		// 情報フィールド
	xTL_ADDINFO	addinfo;	// 付加情報部
	xTL_RECDATA	recvdata;	// テレメトリ受信データ（可変）
} xTL_TLMDATA;

typedef struct {
	BYTE	bMode;		// eTimeDirect:日付の直接指定	eTimeRange：日時の範囲指定
	WORD	wYear;		// eTimeDirect時の年（1970-3000）
	WORD	wMonth;		// eTimeDirect時の月（0-12 0の場合wYearの1～12が対象）
	WORD	wDay;		// eTimeDirect時の日（0-31 0の場合wMonthの1～31が対象）
	time_t	sttime;		// 開始日時
	time_t	sptime;		// 終了日時
} xTL_TIME;
#endif

/*
*******************************************************************************
* 設備監視名称情報
*******************************************************************************
*/
struct xobsname_t {	/* このテーブルは全監視項目について存在する */
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
* ステータス照合定義
*******************************************************************************
*/
typedef struct xchksts_tbl_t {	/* ステータス照合定義テーブル */
	long l_val;	/* 項目値 */
	long l_rsv;	/* サイズ調整 */
	char sz_sts[CHKVALSTR_LEN];/* ステータス文字列 */
} xC_STS;

/*
*******************************************************************************
* ステータス情報
*******************************************************************************
*/
struct xobssts_tbl_t {	/* ステータス情報テーブル */
	long l_id;	/* データｉｄ: 定数=OBSSTS_ID値 */
	long l_len;	/* データ長  : sizeof( struct obssts_tbl_t ) */
	long l_sts_cnt; /* ステータス項目数 */
	long l_rsv;	/* サイズ調整 */
	char sz_i_sts[CHKVALSTR_LEN];/* 初期値ステータス文字列 */
	xchksts_tbl_t chk_t[32]; /* ステータス変換項目 */
};

/*
*******************************************************************************
* 抽出定義情報
*******************************************************************************
*/
typedef struct xpwb_tbl_t {	/* 抽出定義テーブル上に存在する。 */
	long l_pid;	/* 装置番号 */
	long l_word;	/* ワード位置 */
	long l_s_bit;  /* 開始ビット位置 */
	long l_e_bit;  /* 終了ビット位置 */
} xPWB_T;

/*
*******************************************************************************
* 各抽出定義情報
*******************************************************************************
*/
struct xext_tbl_t{	/* 各抽出定義情報 */
	long l_id;	/* データｉｄ:
				EXT_ID or EXTS_ID or
				EXTBCD_ID or EXTBCDS_ID or
				EXTAVG_ID or EXTAVGS_ID or
				EXTCHAR_ID or EXTCHARDEC or
				EXTDBL or EXTFLT */
	long l_len;	/* データ長  : sizeof( struct ext_tbl_t ) */
	long l_pw_cnt;	/* 抽出定義個数 */
	long l_rsv;	/* サイズ調整 */
	xpwb_tbl_t s_pwb[10];/* 抽出定義情報 */
};

/*
*******************************************************************************
* 条件定義情報(if)
*******************************************************************************
*/
struct xif_tbl_t {	/* 条件定義テーブル */
	long l_id;	/* データｉｄ: 定数=IF_ID , CATDATA_ID値 */
	long l_len;	/* データ長  : sizeof( struct if_tbl_t ) */
	char sz_obsname[CTLID_LEN]; 	/* 監視項目ＩＤ: 設備名.監視項目名 */
	long l_flag;   /* 条件式 0:'=' 1:'!' 2:'<' 3:'>' CATDATA_IDの時は無効 */
	long l_offset;	/* 監視項目オフセット */
	double d_val;  /* 値 */
};

/*
*******************************************************************************
* 他項目計算情報
*******************************************************************************
*/
struct xcal_tbl_t {	/* 他項目計算定義テーブル */
	long l_id;	/* データｉｄ: 定数=CAL_ID値 */
	long l_len;	/* データ長  : sizeof( struct cal_tbl_t ) */
	long l_flag;	/* 演算式 0:'+' 1:'-' 2:'*' 3:'/' */
	long l_offset;	/* 監視項目オフセット */
	char sz_obsname[CTLID_LEN];/* 監視項目ＩＤ: 設備名.監視項目名 */
};

/*
*******************************************************************************
* 方程式定義情報
*******************************************************************************
*/
struct xpol_tbl_t {	/* 方程式定義定義テーブル */
	long l_id;	/* データｉｄ: 定数=pol_ID値 */
	long l_len;	/* データ長  : sizeof( struct pol_tbl_t ) */
	long l_val_cnt; /* d_valの個数 */
	long l_rsv;	/* サイズ調整 */
	double d_val[5];/* 値 */
};

/*
*******************************************************************************
* カーブ定義情報
*******************************************************************************
*/
struct xcarv_tbl_t {	/* カーブ定義テーブル */
	long l_id;	/* データｉｄ: 定数=CARV_ID値 */
	long l_len;	/* データ長  : sizeof( struct carv_tbl_t ) */
	long l_carv_cnt; /* d_carvの個数 */
	long l_rsv;	/* サイズ調整 */
	double d_carv[CARV_MAX][2]; /* AGC校正点テーブル[][1] */
};

/*
*******************************************************************************
* 時刻変換情報
*******************************************************************************
*/
struct xtimecvt_tbl_t {	/* 時刻変換情報定義テーブル */
	long l_id;	/* データｉｄ: 定数=TIMECVT_ID値 */
	long l_len;	/* データ長  : sizeof( struct timecvt_tbl_t ) */
	long l_cvtno;  /* 変換番号 */
	long l_rsv;	/* サイズ調整 */
};

struct  xsdtp_msg_t {               /* sdtp メッセージテーブル*/
	long long   l_unix_msgtype;    /* メッセージタイプ(64) (8 byte -> 16 byte)*/
	long        l_len;             /* 送信バイト数 */
	long        l_objtype;         /* 要求ＩＤ */
	long        l_blkcnt;          /* ブロックカウント */
	long        l_send_ipc;        /* 送信側メッセージキューＩＤ */
	long        l_recv_ipc;        /* 受信側メッセージキューＩＤ */
	unsigned char uca_SdtpDummy[26];/* 受信側メッセージキューＩＤ */
	unsigned char uca_SdtpPDULen[4];/* PDU長 */
	unsigned char uca_SdtpRecNum[2];/* レコード数 */
	unsigned char uca_buff[MAX_SDTP];/* データバッファ */
};

struct  xsdtp_msg_ex_t {               /* sdtp メッセージテーブル 2017年版 */
	unsigned char uca_SdtpDummy[26];/* 受信側メッセージキューＩＤ */
	unsigned char uca_SdtpPDULen[4];/* PDU長 */
	unsigned char uca_SdtpRecNum[2];/* レコード数 */
	unsigned char uca_buff[MAX_SDTP];/* データバッファ */
};

union   xumoji {                         /* 文字列先頭2文字のバッファ */
	char   c_moji[2];
	short  s_moji;
};


struct xobs_tbl_t {
	struct xobsname_t *obs_adr;     /* 監視項目定義情報 */
	long             l_dataid;     /* 抽出データＩＤ */
	long             l_offset;     /* 監視項目のオフセット */
	long             l_grp_cnt;    /* 拡張：グループカウント */
};


/*
*******************************************************************************
* 工学値変換結果
*******************************************************************************
*/
struct xobs_tbl_res_t {
	struct xobsname_t obs_adr;      /* 監視項目定義情報 */
	long             l_dataid;     /* 抽出データＩＤ */
	long             l_offset;     /* 監視項目のオフセット */
	long double      ld_time_val;  /* 拡張：時間値 */
	long             l_grp_cnt;    /* 拡張：グループカウント */
};

/*
*******************************************************************************
* 工学値変換結果の格納タイプ
*******************************************************************************
*/
enum eCnvResType
{
	eCnvResType_Long = 0,				/* ビット0: long */
	eCnvResType_Double,					/* ビット1: double */
	eCnvResType_LongDouble,				/* ビット2: long double */
	eCnvResType_StatusStr,				/* ビット3: ステータス文字列 */
	eCnvResType_TimeStr,				/* ビット4: 時刻文字列 */
	eCnvResType_TimeLeapSec,			/* ビット5: うるう秒 */

	eCnvResType_Max,
};
#pragma pack()


#pragma warning(disable:4251)

typedef void (CALLBACK *NOTIFYDATACALLBACK)(TCHAR* monName, time_t monTime, xobs_tbl_res_t* monData, bool bLeapSec);

class CEngValCnvDll
{
public:
	CEngValCnvDll(char* db_file_name);
	CEngValCnvDll();
	~CEngValCnvDll();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	char		mEqMonDBFileName[_MAX_PATH];
	TCHAR		mMonDataPathName[_MAX_PATH];
	CString		mDbName;
	map< CString, vector<xobs_tbl_res_t> >	mOutDataList;			// 工学値変換用監視名マップ
	vector<CString>							mMonDataFileList;		// 工学値変換対象のファイルリスト
	std::vector<xobs_tbl_res_t> data_list;

protected:

	long g_l_ub_dtime_timezoneflg = 0; /*1:timezoneは有効*/
	int db_status; /* read状態 */

	int mFileSize;
	char *db_data;// [BUFF_SIZE];
	char *db_data2;// [BUFF_SIZE];
	struct xobs_tbl_t obs_tbl[OBS_MAX];

	long m_lastCnvAfterType;

	map<int, vector<xobs_tbl_t>> obs_map;

	map<CString, long double>	mMonTimeLDData;			// 設備ごとのMON_TIMEの時刻

	NOTIFYDATACALLBACK	mCallBack;

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	void SetEqMonDbFileName(TCHAR* val);
	void SetMonDataPath(TCHAR* val);
	bool GetMonitorData(xTL_TIME& timeTarget, map< CString, vector<xobs_tbl_res_t> >& outDataList);
	bool GetMonitorDataEx(xTL_TIME* timeTarget);
	void ClearMonitorName();
	void AddMonitorName(TCHAR* name);
	void SetCallBack(NOTIFYDATACALLBACK fCallBack) { mCallBack = fCallBack; }

	map< CString, vector<xobs_tbl_res_t> >&	GetOutDataList() { return mOutDataList; }
	vector<CString>&						GetMonDataFileList() { return mMonDataFileList; }
	void GetDataFileList(CString& path, vector<CString>& outFileList);
	void GetTimeRangePath(time_t sttime, time_t sptime, vector<CString>& fileList);
	void ClearDataFileList();
	void AddDataFileList(TCHAR* val);
protected:
	bool ConvertDirectRangeTime(xTL_TIME& dtime, xTL_TIME& rtime, bool timezero=true);
	bool ConvertStatusChar(char* stsname, int len);
	CStringA UTF8_3CharsToSJis(char *pUtf8Str);
	CStringA EUCToSJis(char *pEucStr);
	bool IsObsName(char* obsname, map< CString, vector<xobs_tbl_res_t> >& outDataList);

	long double GetAndUpdateMonTime(CString strWatch, long double fTimeVal = 0.0)
	{
		long double	res_time = 0.0;

		if( mMonTimeLDData.count(strWatch) != 0 )
		{
			if( fTimeVal != 0.0 )
				mMonTimeLDData[strWatch] = fTimeVal;
			res_time = mMonTimeLDData[strWatch];
		}
		else
			mMonTimeLDData.insert(pair<CString, long double>(strWatch, fTimeVal));

		return res_time;
	}

public:
	int  CalcFromFileEx(char *cp_loadadr_p, map< CString, vector<xobs_tbl_res_t> >& outDataList);
	int  CalcFromFile(char *cp_loadadr_p);
	int  CalcFromMsg(xsdtp_msg_ex_t* msg);
	//vector<obs_tbl_res_t>& GetAllVal() { return data_list; }
	vector<xobs_tbl_res_t>& GetAllVal() { return data_list; }

	long GetCnvResType()	{ return m_lastCnvAfterType; }
	static BOOL IsCnvResLong(struct xobsname_t& obs_adr)
	{
		if ((obs_adr.l_cnvResType & (1 << eCnvResType_Long)) != 0)
			return TRUE;
		else
			return FALSE;
	}
	static BOOL IsCnvResDouble(struct xobsname_t& obs_adr)
	{
		if ((obs_adr.l_cnvResType & (1 << eCnvResType_Double)) != 0)
			return TRUE;
		else
			return FALSE;
	}
	static BOOL IsCnvResLongDouble(struct xobsname_t& obs_adr)
	{
		if ((obs_adr.l_cnvResType & (1 << eCnvResType_LongDouble)) != 0)
			return TRUE;
		else
			return FALSE;
	}
	static BOOL IsCnvResStatusStr(struct xobsname_t& obs_adr)
	{
		if ((obs_adr.l_cnvResType & (1 << eCnvResType_StatusStr)) != 0)
			return TRUE;
		else
			return FALSE;
	}
	static BOOL IsCnvResTimeStr(struct xobsname_t& obs_adr)
	{
		if ((obs_adr.l_cnvResType & (1 << eCnvResType_TimeStr)) != 0)
			return TRUE;
		else
			return FALSE;
	}

protected:
	int DbRead(char* db_file_nam, char *cp_loadadr_p);
	void* MonRead(FILE **fp);

	int ud_montbl_data64(char *shmadr, int l_dataid, unsigned char ca_data[], int l_offset, int *l_inflg, int *l_grmd, double *d_data, char sz_sts_name[], long double *ldp_data, long *lp_bitinfo);
	void ua_ldbitfieldll(unsigned long long *pull_outbuf, unsigned char inputbuf[], long l_wordno, unsigned long ul_bitno, unsigned long ul_bitlen, long l_signflg);
	void ua_ldbitfield(unsigned long *pul_outbuf, unsigned char inputbuf[], long l_wordno, unsigned long ul_bitno, unsigned long ul_bitlen, long l_signflg);
	void ua_insbitfield(unsigned char  *tgtadr, long l_inspos, long l_inslen, char  *suadr, long l_extpos);
	void ub_bcdext(double *d_data, unsigned char ca_data[], long l_word, long l_bit, long l_len);
	int ud_montbl_data(char *shmadr, int l_dataid, unsigned char ca_data[], int l_offset, int *l_inflg, int *l_grmd, double *d_data, char sz_sts_name[]);
	void ub_dtime_to_asc(double d_uttime, char *sz_time);
	void ub_bcd6_to_bcd(unsigned char achBCD6[], unsigned char achYYYY[], unsigned char achTime[]);
	double ub_bcd_to_dtime(unsigned char uca_yyyy[], unsigned char uca_time[]);
	double ua_ddd_to_dtime(char *cap_ddd, char *cap_hh, char *cap_mm, char *cap_ss);
	double ub_asc_to_dtime(char *sz_time);
	void ua_stbitfield(unsigned long ul_input, unsigned char outputbuf[], long l_wordno, unsigned long ul_bitno, unsigned long ul_bitlen);
	int ud_ext_adr(char sz_watch[], xobsname_t *obs_adr, xext_tbl_t *ext_adr[]);
	double dswap(double d);
	double dswap2(double d);
	float fswap(float d);
	long ds_htonl(long val);
	int uc_valchk(char *s, int *i_p, double *d_p, int l_opt);
	int uc_valchkll(char *s, long *i_p, long double *d_p, long l_opt);
	int ud_sts_flg(char sz_watch[], char *sz_path, char *shmadr, long l_n, int *l_grmd, long *l_offset, char *sz_sts_tbl[]);
	int ud_obsname_adr(char sz_watch[], char *shmadr, struct xobsname_t *obs_adr[]);
	int CreateObsTbl();

};
