//******************************************************************************
// @file	const.h
// @brief	定義ヘッダー
//******************************************************************************
#pragma once

using namespace std;

#ifdef _DEBUG
// 54mを64m環境でデバッグ確認
//#define _DEBUG_KANSHI64M 1			// 64mの捕捉状況ステータスの監視・制御を54mで使う
//#define _DEBUG_PIPEUSE64M 1			// 64mのパイプを54mで使う
//#define _DEBUG_PIPEUSE34M 1			// 34mのパイプを54mで使う
// 受信データに現在時刻をスタンプ
//#define	DemoTimeStamp		1
#endif

#define _GREAT_XSSPA_ISO	1			// 54m X-SSPA ISO対応


static const CString mFaBrowserApp = _T("FAPanelBrowser.exe");
static const CString mTrendGraphApp = _T("TrendGraph.exe");

#define DSN_DBSHM_IPC		0xa901	/* 共通シェアードメモリIPCキー値 */

// カラー定義

#define COLOR_WHITE			RGB(255, 255, 255)
#define COLOR_LIGHTGLEY		RGB(211, 211, 211)
#define COLOR_LIGHTBLUE		RGB(173, 216, 230)
#define COLOR_MAGENTA		RGB(255, 0, 255)
#define COLOR_ORANGE		RGB(255, 165, 0)
#define COLOR_GREEN			RGB(80, 159, 0)
#define COLOR_YELLOW		RGB(255, 255, 0)
#define COLOR_BLUE			RGB(0, 176, 240)

#define COLOR_OPE_OPERATION		COLOR_LIGHTGLEY
#define COLOR_OPE_UPLINK		COLOR_YELLOW
#define COLOR_OPE_RNG			COLOR_ORANGE
#define COLOR_OPE_CMD			COLOR_BLUE
#define COLOR_OPE_TLMS			COLOR_LIGHTGLEY
#define COLOR_OPE_TLMX			COLOR_LIGHTGLEY
#define COLOR_OPE_TLMKa			COLOR_LIGHTGLEY
#define COLOR_OPE_INIT			COLOR_GREEN
#define COLOR_OPE_RDCK			COLOR_LIGHTBLUE
#define COLOR_OPE_ANTZENITH		COLOR_ORANGE
#define COLOR_OPE_ANTSDBY		COLOR_ORANGE

#define COLOR_REMOTE		RGB(0, 0, 128)		// リモート非選択
#define COLOR_REMOTES		RGB(255, 165, 0)	// リモート選択
#define COLOR_D_REMOTE		RGB(192, 192, 255)	// リモート非選択(Disable)
#define COLOR_D_REMOTES		RGB(255, 237, 202)	// リモート選択(Disable)
#define COLOR_LOCAL			RGB(0, 0, 0)		// ローカル
#define COLOR_CTRLIN		RGB(128, 128, 128)	// 制御中
#define COLOR_ALARM			RGB(255, 0, 0)		// アラーム

enum eViewType1{
	eViewType1_Trend,
	eViewType1_EquipmentStatus,
	eViewType1_OperationHist,
	eViewType1_ControlHist,
	eViewType1_EquipmentHist,
	eViewType1_EventHist,
	eViewType1_AlarmList,
	eViewType1_OperationSearch,
	eViewType1_ControlSearch,
	eViewType1_EquipmentSearch,
	eViewType1_EventSearch,
	eViewType1_Max,
	eViewType5_EqOperation1,
	eViewType5_EqOperation2,
	eViewType5_EqOperation3,
	eViewType5_EqOperation4,
	eViewType5_EqOperation5,
	eViewType5_OperationHist,
	eViewType5_ControlHist,
	eViewType5_EquipmentHist,
	eViewType5_EventHist,
	eViewType5_AlarmList,
	eViewType5_OperationSearch,
	eViewType5_ControlSearch,
	eViewType5_EquipmentSearch,
	eViewType5_EventSearch,
	eViewType5_Max,

	eViewTypeTool_TimeLine,

	eViewType_Max,
};

static const TCHAR* mViewTitle[]{
	_T("捕捉状況グラフ"),
	_T("設備状態"),
	_T("運用履歴表示"),
	_T("制御履歴表示"),
	_T("設備状態履歴表示"),
	_T("イベント履歴表示"),
	_T("設備アラーム状況一覧"),
	_T("運用履歴表示"),
	_T("制御履歴表示"),
	_T("設備状態履歴表示"),
	_T("イベント履歴表示"),
	_T(""),
	_T("設備・運用状況"),
	_T("設備・運用状況"),
	_T("設備・運用状況"),
	_T("設備・運用状況"),
	_T("設備・運用状況"),
	_T("運用履歴表示"),
	_T("制御履歴表示"),
	_T("設備状態履歴表示"),
	_T("イベント履歴表示"),
	_T("設備アラーム状況一覧"),
	_T("運用履歴表示"),
	_T("制御履歴表示"),
	_T("設備状態履歴表示"),
	_T("イベント履歴表示"),
	_T(""),
	_T("運用計画タイムライン表示"),
	_T(""),
};

enum eExecuteMode{
	eExecuteMode_Control,
	eExecuteMode_Monitor,
};

enum eLimitedeMode{
	eLimitedMode_Disable,	//制御モードでかつ、制限つきでない
	eLimitedMode_Enable,	//制御モードでかつ、制限つき
};

enum eStationMode{
	eStation_1,
	eStation_5,
	eStation_10,
};
static const TCHAR* mStationModeString[]{
	_T("１局モード"),
	_T("４局モード"),
	_T("全局モード"),
};

enum eStation{
	eStation_Usuda64,
	eStation_Usuda642,
	eStation_Uchinoura34,
	eStation_Uchinoura20,
	eStation_MAX,
};
static const TCHAR* mStationString[]{
	_T("臼田64m局"),
	_T("臼田54m局"),
	_T("内之浦34m局"),
	_T("内之浦20m局"),
};
static const TCHAR* mStationStringEx = _T("筑波局運用管制");
static const TCHAR* mHistoryStationString[]{
	_T("臼田64m局"),
	_T("内之浦34m局"),
	_T("内之浦20m局"),
	_T("臼田54m局"),
};
static const TCHAR* mStationSimpleString[]{
	_T("UDSC64"),
	_T("UDSC54"),
	_T("USC34"),
	_T("USC20"),
};
static const TCHAR* mCtrlNameString[]{
	_T("tzopr1"),
	_T("tzopr1"),
	_T("tzopr1"),
	_T("tzopr1"),
};
static const TCHAR* mNetworkDrive[]{
	_T("U:\\"),	// 臼田64m
	_T("X:\\"),	// 臼田54m
	_T("V:\\"),	// 内之浦34m
	_T("W:\\"),	// 内之浦20m
};
static const TCHAR* mHostNameUsuda = _T("u5opr1W1");	//ホスト名 臼田宇宙空間観測所(臼田64m局舎内)
static const COLORREF mStationColor[]{
	RGB(195, 195, 195),
	RGB(255, 193, 164),
	RGB(255, 195, 255),
	RGB(153, 217, 255),
};
static const TCHAR* mStationCtrlDir[]{
	_T("/sos/opr/data/udsc64/log/dump/"),
	_T("/sos/opr/data/udsc54/log/dump/"),
	_T("/sos/opr/data/usc34/log/dump/"),
	_T("/sos/opr/data/usc20/log/dump/"),
};

// 局固定選択番号
enum eFixedSelectStation{
	eFixedSelectStation_Usuda64 = 10,
	eFixedSelectStation_Usuda642,
	eFixedSelectStation_Uchinoura34,
	eFixedSelectStation_Uchinoura20,
	eFixedSelectStation_MAX,
};

// 局の表示順
static const UINT mStationViewOrder[]{
	eStation_Uchinoura20,
	eStation_Uchinoura34,
	eStation_Usuda64,
	eStation_Usuda642,
	eStation_MAX,
};
enum eStationCnv{
	eStationCnv_Uchinoura20,
	eStationCnv_Uchinoura34,
	eStationCnv_Usuda64,
	eStationCnv_Usuda642,
	eStationCnv_MAX,
};
static const UINT mStationViewCnv[]{
	eStationCnv_Usuda64,
	eStationCnv_Usuda642,
	eStationCnv_Uchinoura34,
	eStationCnv_Uchinoura20,
	eStationCnv_MAX,
};

// 取得先
enum eGetFrom{
	eGetFrom_ENP,
	eGetFrom_STATION,
};
static const CString mGetFromString[]{
	_T("ENP"),
	_T("局管制"),
	_T("STTN"),
};

// 運用種別
enum eOperationKind{
	eOperationKind_StationOccupancy,
	eOperationKind_Plan,
	eOperationKind_Vlbi,
	eOperationKind_Rdychk,
};
static const CString mOperationKindString[]{
	_T("局占有"),
	_T("実運用"),
	_T("VLBI"),
	_T("レディネスチェック"),
};

//=============================================================================
//	設備運用状況
//=============================================================================
// 設備状況項目定義
// 64m
#define MONI64M_ANT		"CTRL.ANT_BLOCK"
#define MONI64M_CMDOUT		"CTRL.CMDOUT_BLOCK"
#define MONI64M_CMDOUT3	"CTRL.CMDOUT3_BLOCK"
#define MONI64M_S_CAL		"CTRL.S-CAL_BLOCK"
#define MONI64M_S_DC		"CTRL.SS_DCSYN2_BLOCK"
#define MONI64M_S_RXRF		"CTRL.S_RXRF_BLOCK"
#define MONI64M_S_TLMIN	"CTRL.S_TLMIN_BLOCK"
#define MONI64M_S_TXFCONV	"CTRL.S_TXFCONV_BLOCK"
#define MONI64M_S_TXRF		"CTRL.S_TXRF_BLOCK"
#define MONI64M_S_UC		"CTRL.S_TXFCONV_BLOCK"
#define MONI64M_SS_TCRDEM	"CTRL.SS_TCRDEM_BLOCK"
#define MONI64M_SS_TCRMOD	"CTRL.SS_TCRMOD_BLOCK"
#define MONI64M_WEATHER	"CTRL.WEATHER_BLOCK"
#define MONI64M_X_CAL		"CTRL.X-CALOUT_BLOCK"
#define MONI64M_X_CMDOUT2	"CTRL.X_CMDOUT2_BLOCK"
#define MONI64M_X_DC		"CTRL.DC_BLOCK"
#define MONI64M_X_DCSYN	"CTRL.X-RXCONV_BLOCK"
#define MONI64M_X_HPA		"CTRL.X-KLY-TX1_BLOCK"
#define MONI64M_X_LNA		"CTRL.LNA3_BLOCK"
#define MONI64M_X_RX2		"CTRL.X_RX2_BLOCK"
#define MONI64M_X_RXRF		"CTRL.X-RXCONV_BLOCK"
#define MONI64M_X_TCRMOD	"CTRL.XX_TCRMOD_BLOCK"
#define MONI64M_X_TLMIN	"CTRL.X_PKTDEM_BLOCK"
#define MONI64M_X_TLMIN2	"CTRL.X_TLMIN2_BLOCK"
#define MONI64M_X_TXRF		"CTRL.X-TXIF_BLOCK"
#define MONI64M_XR_RARR	"CTRL.XX_RRNG_BLOCK"
#define MONI64M_XS_DC		"CTRL.XS_DC_BLOCK"
#define MONI64M_XX_RARR	"CTRL.XX_RARR_BLOCK"
#define MONI64M_XX_TCRDEM	"CTRL.DSNTCR_BLOCK"
#define MONI64M_XX_TCRMOD	"CTRL.DSNTCR_BLOCK"
#define MONI64M_XX_TCRRARR	"CTRL.DSNTCR_BLOCK"
#define MONI64M_GREEN		"COLOR_GREEN"
//#define MONI64M_szopr1	固定(緑)
//#define MONI64M_tzopr1	固定(緑)
//#define MONI64M_tzopr2	固定(緑)
//#define MONI64M_u6ctrl	固定(緑)
//#define MONI64M_u6opr1	固定(緑)

// 54m
#define MONI54M_ANT		"CTRL.ANT_BLOCK"
#define MONI54M_CMDOUT		"CTRL.CMDOUT_BLOCK"
#define MONI54M_CMDOUT3	"CTRL.CMDOUT3_BLOCK"
#define MONI54M_S_CAL		"CTRL.S-CAL_BLOCK"
#define MONI54M_S_DC		"CTRL.SS_DCSYN2_BLOCK"
#define MONI54M_S_RXRF		"CTRL.S_RXRF_BLOCK"
#define MONI54M_S_TLMIN	"CTRL.S_TLMIN_BLOCK"
#define MONI54M_S_TXFCONV	"CTRL.S_TXFCONV_BLOCK"
#define MONI54M_S_TXRF		"CTRL.S_TXRF_BLOCK"
#define MONI54M_S_UC		"CTRL.S_TXFCONV_BLOCK"
#define MONI54M_SS_TCRDEM	"CTRL.SS_TCRDEM_BLOCK"
#define MONI54M_SS_TCRMOD	"CTRL.SS_TCRMOD_BLOCK"
#define MONI54M_WEATHER	"CTRL.WEATHER_BLOCK"
#define MONI54M_X_CAL		"CTRL.X-CAL_BLOCK"
#define MONI54M_X_CMDOUT2	"CTRL.X_CMDOUT_BLOCK"
#define MONI54M_X_DC		"CTRL.X-DC_BLOCK"
#define MONI54M_X_DCSYN	"CTRL.X-RXCONV_BLOCK"
#define MONI54M_X_HPA		"CTRL.X-KLY-TX1_BLOCK"		// SSPAの場合はCTRL.SSPA_BLOCK
#define MONI54M_SSPA		"CTRL.SSPA_BLOCK"
#define MONI54M_X_LNA		"CTRL.X-LNA_BLOCK"
#define MONI54M_X_RX2		"CTRL.X_RX2_BLOCK"
#define MONI54M_X_RXIF		"CTRL.X-RX_IF_BLOCK"
#define MONI54M_X_TCRMOD	"CTRL.XX_TCRMOD_BLOCK"
#define MONI54M_X_TLMIN	"CTRL.X_PKTDEM_BLOCK"
#define MONI54M_X_TLMIN2	"CTRL.XKa_TLMIN_BLOCK"
#define MONI54M_X_TXRF		"CTRL.X-TXIF_BLOCK"
#define MONI54M_XR_RARR	"CTRL.XX_RRNG_BLOCK"
#define MONI54M_XS_DC		"CTRL.XS_DC_BLOCK"
#define MONI54M_XX_RARR	"CTRL.XX_RARR_BLOCK"
#define MONI54M_XX_TCRDEM	"CTRL.DSNTCR_BLOCK"
#define MONI54M_XX_TCRMOD	"CTRL.DSNTCR_BLOCK"
#define MONI54M_XX_TCRRARR	"CTRL.DSNTCR_BLOCK"
#define MONI54M_GREEN		"COLOR_GREEN"

#define MONI54M_X_TXIF	"CTRL.X-TX_IF_BLOCK"		// X-TXIF(54m)
#define MONI54M_X_UC	"CTRL.X-UC_BLOCK"			// X-UC(54m)
#define MONI54M_DSN_MODEM	"CTRL.DSNTCR_BLOCK"		// DSN-MODEM(54m)
#define MONI54M_KA_LNA	"CTRL.Ka-LNA_BLOCK"			// Ka-LNA(54m)
#define MONI54M_KA_DC	"CTRL.Ka-DC_BLOCK"			// Ka-DC(54m)
#define MONI54M_REF_DIV	"CTRL.REF_DIV_BLOCK"		// REF-DIV(54m)	
#define MONI54M_X_TRANS	"CTRL.X-TRANS_BLOCK"		// X-TRANS(54m)	
#define MONI54M_TRANS_LO	"CTRL.TRANS_LO_BLOCK"	// TRANS-LO(54m)
#define MONI54M_KA_CAL	"CTRL.Ka-CAL_BLOCK"			// Ka-CAL(54m)
#define MONI54M_KA_TRANS	"CTRL.Ka-TRANS_BLOCK"	// Ka-TRANS(54m)
#define MONI54M_CVCF	"CTRL.CVCF_BLOCK"			// CVCF(54m)

//#define MONI54M_szopr1	固定(緑)
//#define MONI54M_tzopr1	固定(緑)
//#define MONI54M_tzopr2	固定(緑)
//#define MONI54M_u6ctrl	固定(緑)
//#define MONI54M_u6opr1	固定(緑)

// 34m
#define MONI34M_ANT		"CTRL.ANT_BLOCK"
//#define MONI34M_CTRL_1	固定(緑)
//#define MONI34M_CTRL_2	固定(緑)
//#define MONI34M_CTRL_3	固定(緑)
//#define MONI34M_CTRL_4	固定(緑)
#define MONI34M_DSNTCR_DEM	"CTRL.DSNTCR_BLOCK"
#define MONI34M_DSNTCR_MOD	"CTRL.DSNTCR_BLOCK"
#define MONI34M_DSNTCR_RARR	"CTRL.DSNTCR_BLOCK"
//#define MONI34M_QD	固定(緑)
#define MONI34M_S_CAL		"CTRL.S_CAL_BLOCK"
#define MONI34M_S_CMDOUT	"CTRL.CMDOUT_BLOCK"
#define MONI34M_S_RXIFSW	"CTRL.S_RXIFSW_BLOCK"
#define MONI34M_S_RXSYN	"CTRL.S_RXSYN_BLOCK"
#define MONI34M_S_TLMIN	"CTRL.S_TLMIN_BLOCK"
#define MONI34M_S_TRKRX	"CTRL.S_RX_BLOCK"
#define MONI34M_S_TX		"CTRL.S_TX_BLOCK"
#define MONI34M_S_TXIFSW	"CTRL.S_TXIFSW_BLOCK"
#define MONI34M_S_TXSYN	"CTRL.S_TXSYN_BLOCK"
//#define MONI34M_S/SX追尾状況1	固定(緑)
//#define MONI34M_S/SX追尾状況2	固定(緑)
#define MONI34M_SS_TCRDEM	"CTRL.SS_TCRDEM_BLOCK"
#define MONI34M_SS_TCRMOD	"CTRL.SS_TCRMOD_BLOCK"
#define MONI34M_SS_TCRRARR	"CTRL.SS_TCRDEM_BLOCK"
//#define MONI34M_STS_1	固定(緑)
#define MONI34M_SX_ACQRX	"CTRL.S-ACQ_TRKRX_BLOCK"
#define MONI34M_SX_ACQRXSYN	"CTRL.SX_ACQRXSYN_BLOCK"
#define MONI34M_TIME		"CTRL.TIME_BLOCK"
#define MONI34M_TX_POL		"CTRL.SX_TXPW_BLOCK"
#define MONI34M_WEATHER	"CTRL.WEATHER_BLOCK"
#define MONI34M_X_CAL		"CTRL.X-CALOUT_BLOCK"
#define MONI34M_X_CMDOUT2	"CTRL.X_CMDOUT2_BLOCK"
#define MONI34M_X_HRX_DEM	"CTRL.X_QPSKRX_BLOCK"
#define MONI34M_X_HRX_MOD	"CTRL.X_QPSKRX_BLOCK"
#define MONI34M_X_HRXTLMIN	"CTRL.X_QPSKTLMIN_BLOCK"
#define MONI34M_X_IFSW		"CTRL.X-RF_IF_BLOCK"
#define MONI34M_X_RX		"CTRL.X_RX_BLOCK"
#define MONI34M_X_RXSYN	"CTRL.X-ACQ_TRKRX_BLOCK"
#define MONI34M_X_TRKRX		"CTRL.X-TRKRX_BLOCK"
#define MONI34M_X_TLMIN2	"CTRL.X_TLMIN2_BLOCK"
#define MONI34M_X_TX		"CTRL.X-UL_CNT_BLOCK"
#define MONI34M_X_TXSYN	"CTRL.X-UC_BLOCK"
//#define MONI34M_X/X追尾状況	固定(緑)
//#define MONI34M_ブロック図	固定(緑)
#define MONI34M_GREEN		"COLOR_GREEN"

// 20m
#define MONI20M_ANT		"CTRL.ANT_BLOCK"
#define MONI20M_CMDOUT1	"CTRL.CMDOUT1_BLOCK"
#define MONI20M_CMDOUT2	"CTRL.CMDOUT2_BLOCK"
//#define MONI20M_CTRL_1	固定(緑)
//#define MONI20M_CTRL_2	固定(緑)
//#define MONI20M_CTRL_3	固定(緑)
//#define MONI20M_QD	固定(緑)
//#define MONI20M_QD2	固定(緑)
#define MONI20M_S_ACQDET	"CTRL.S_ACQDET_BLOCK"
#define MONI20M_S_ACQTCRDEM	"CTRL.S_ACQTCRDEM_BLOCK"
#define MONI20M_S_CALRF		"CTRL.S_CALRF_BLOCK"
#define MONI20M_S_DCSYN		"CTRL.SS_DCSYN_BLOCK"
#define MONI20M_S_RXRF		"CTRL.S-RXRF_SUM_BLOCK"
#define MONI20M_S_TCRDEM	"CTRL.SS_TCRDEM_BLOCK"
#define MONI20M_S_TCRDET	"CTRL.SS_TCRDET_BLOCK"
#define MONI20M_S_TCRRARR	"CTRL.SS_TCRDEM_BLOCK"
#define MONI20M_S_TLMIN		"CTRL.S_TLMIN_BLOCK"
#define MONI20M_S_TXFCONV	"CTRL.S-RFOUT_BLOCK"
#define MONI20M_S_TXRF		"CTRL.S_TXRF_BLOCK"
#define MONI20M_S_UCSYN		"CTRL.S_UCSYN_BLOCK"
#define MONI20M_SS_TCRMOD	"CTRL.SS_TCRMOD_BLOCK"
#define MONI20M_SS_TRANSSYN	"CTRL.SS_TRANSSYN_BLOCK"
//#define MONI20M_STS_1	固定(緑)
#define MONI20M_SX_ACQDCSYN	"CTRL.S_ACQDCSYN_BLOCK"
#define MONI20M_SX_TCRMOD	"CTRL.SX_TCRMOD_BLOCK"
#define MONI20M_SX_TRANSSYN	"CTRL.SX_TRANSSYN_BLOCK"
#define MONI20M_WEATHER		"CTRL.WEATHER_BLOCK"
#define MONI20M_X_CALRF_UC	"CTRL.X-CALRFOUT_BLOCK"
#define MONI20M_X_DCSYN		"CTRL.XS_DCSYN_BLOCK"
#define MONI20M_X_HRX_DEM	"CTRL.X_QPSKRXDEM_BLOCK"
#define MONI20M_X_HRX_MOD	"CTRL.X_QPSKRXDEM_BLOCK"
#define MONI20M_X_HRXTLMIN	"CTRL.X_QPSKTLMIN_BLOCK"
#define MONI20M_X_TCRDEM	"CTRL.SX_TCRDEM_BLOCK"
#define MONI20M_X_TCRDET	"CTRL.SX_TCRDET_BLOCK"
#define MONI20M_X_TCRRARR	"CTRL.SX_TCRRARR_BLOCK"
#define MONI20M_X_TLMDEC	"CTRL.X_TLMDEC_BLOCK"
#define MONI20M_X_TLMIN		"CTRL.X_TLMIN_BLOCK"
#define MONI20M_X_TLMSIM	"CTRL.X_TLMSIM_BLOCK"
//#define MONI20M_ブロック図	固定(緑)
//#define MONI20M_追尾状況	固定(緑)
//#define MONI20M_運用状況	固定(緑)
#define MONI20M_GREEN		"COLOR_GREEN"

#define MONIEQUIPMENT_GREEN		"COLOR_GREEN"

static const CString mEquipmentStatusString[4][9][6] = {
	{
		// 64m
		{ _T("ANT"), _T("S-TXRF"), _T("S-TXFCONV"), _T("S-UC"), _T("SS-TCRMOD"), _T("CMDOUT") },
		{ _T(""), _T("S-RXRF"), _T(""), _T("S-DC"), _T("SS-TCRDEM"), _T("S-TLMIN") },
		{ _T("X-HPA"), _T("X-TXRF"), _T(""), _T(""), _T("X-TCRMOD"), _T("CMDOUT3") },
		{ _T("X-LNA"), _T("X-RXRF"), _T("X-DC"), _T("X-RX2"), _T("X-DCSYN"), _T("X-TLMIN") },
		{ _T(""), _T(""), _T(""), _T(""), _T("XX-RARR"), _T("XR-RARR") },
		{ _T(""), _T(""), _T("XS-DC"), _T("XX-TCRMOD"), _T("XX-TCRDEM"), _T("X-CMDOUT2") },
		{ _T(""), _T(""), _T(""), _T(""), _T("XX-TCRRARR"), _T("X-TLMIN2") },
		{ _T("S-CAL"), _T("X-CAL"), _T("WEATHER"), _T(""), _T(""), _T("") },
		{ _T("u6ctrl"), _T("u6opr1"), _T("szopr1"), _T("tzopr1"), _T("tzopr2"), _T("") },
	},
	{
		// 54m
		{ _T("ANT"), _T("X-SSPA"), _T("X-TXIF"), _T("X-UC"), _T(""), _T("X-CMDOUT") },
		{ _T(""), _T(""), _T(""), _T(""), _T(""), _T("") },
		{ _T("X-LNA"), _T("X-RXIF"), _T("X-DC"), _T("DSN-MODEM"), _T(""), _T("XKa-TLMIN") },
		{ _T("Ka-LNA"), _T("Ka-DC"), _T(""), _T(""), _T(""), _T("") },
		{ _T(""), _T(""), _T(""), _T(""), _T("REF-DIV"), _T("") },
		{ _T("X-CAL"), _T("X-TRANS"), _T("TRANS-LO"), _T(""), _T(""), _T("") },
		{ _T("Ka-CAL"), _T("Ka-TRANS"), _T(""), _T(""), _T(""), _T("") },
		{ _T(""), _T(""), _T(""), _T(""), _T("WEATHER"), _T("CVCF") },
		{ _T("u5ctrl"), _T("u5opr1"), _T("szopr4"), _T("tzopr1"), _T(""), _T("") },
	},
	{
		// 34
		{ _T("ANT"), _T("S-TX"), _T("S-TXSYN"), _T("S-TXIFSW"), _T("SS-TCRMOD"), _T("S-CMDOUT") },
		{ _T("TX-POL"), _T("S-TRKRX"), _T(""), _T("S-RXIFSW"), _T("SS-TCRDEM"), _T("S-TLMIN") },
		{ _T(""), _T("S-ACQRX"), _T(""), _T(""), _T("SS-TCRRARR"), _T("") },
		{ _T("X-TX"), _T("X-RF_IF"), _T("X-UC"), _T(""), _T("DSNTCR-MOD"), _T("X-CMDOUT2") },
		{ _T("X-RX"), _T("X-ACQRX"), _T("X-TRXRX"), _T(""), _T("DSNTCR-DEM"), _T("X-TLMIN2") },
		{ _T(""), _T(""), _T("X-HRXDEM"), _T("X-HRXMOD"), _T("X-HRXTLMIN"), _T("") },
		{ _T("S-CAL"), _T("X-CAL"), _T(""), _T(""), _T("DSNTOR-RARR"), _T("") },
		{ _T("S/SX追尾状況1"), _T("S/SX追尾状況2"), _T("X/X追尾状況"), _T("STS-1"), _T("TIME"), _T("WEATHER") },
		{ _T("QD"), _T("CTRL-1"), _T("CTRL-2"), _T("CTRL-3"), _T("CTRL-4"), _T("ブロック図") },
	},
	{
		// 20m
		{ _T("ANT"), _T("S-TXRF"), _T("S-TXFCONV"), _T("S-USCYN"), _T("SS-TCRMOD"), _T("CMDOUT1") },
		{ _T(""), _T(""), _T(""), _T(""), _T("SX-TCRMOD"), _T("CMDOUT2") },
		{ _T("S-RXRF"), _T("S-DCSYN"), _T("S-TCRDET"), _T(""), _T("S-TCRDEM"), _T("S-TLMIN") },
		{ _T(""), _T("SX-ACQDCSYN"), _T("S-ACQDET"), _T("S-ACQTCRDEM"), _T("S-TCRRARR"), _T("") },
		{ _T(""), _T("X-DCSYN"), _T("X-TCRDET"), _T("X-TCRRARR"), _T("X-TCRDEM"), _T("X-TLMIN") },
		{ _T(""), _T(""), _T(""), _T("X-HRXDEM"), _T("X-HRXMOD"), _T("X-HRTLMIN") },
		{ _T("S-CALRF"), _T("X-CALRF U/C"), _T("SS-TRANSSYN"), _T("SX-TRANSSYN"), _T("X-TLMDEC"), _T("X-TLMSIM") },
		{ _T("CTRL-1"), _T("CTRL-2"), _T("CTRL-3"), _T(""), _T(""), _T("WEATHER") },
		{ _T("追尾状況"), _T("運用状況"), _T("QD"), _T("QD2"), _T("STS-1"), _T("ブロック図") },
	},
};


static const CString mEquipmentStatusStatusTag[4][9][6] = {
	{
		// 64m
		{ CString(MONI64M_ANT), CString(MONI64M_S_TXRF), CString(MONI64M_S_TXFCONV), CString(MONI64M_S_UC), CString(MONI64M_SS_TCRMOD), CString(MONI64M_CMDOUT) },
		{ CString(""), CString(MONI64M_S_RXRF), CString(""), CString(MONI64M_S_DC), CString(MONI64M_SS_TCRDEM), CString(MONI64M_S_TLMIN) },
		{ CString(MONI64M_X_HPA), CString(MONI64M_X_TXRF), CString(""), CString(""), CString(MONI64M_X_TCRMOD), CString(MONI64M_CMDOUT3) },
		{ CString(MONI64M_X_LNA), CString(MONI64M_X_RXRF), CString(MONI64M_X_DC), CString(MONI64M_X_RX2), CString(MONI64M_X_DCSYN), CString(MONI64M_X_TLMIN) },
		{ CString(""), CString(""), CString(""), CString(""), CString(MONI64M_XX_RARR), CString(MONI64M_XR_RARR) },
		{ CString(""), CString(""), CString(MONI64M_XS_DC), CString(MONI64M_XX_TCRMOD), CString(MONI64M_XX_TCRDEM), CString(MONI64M_X_CMDOUT2) },
		{ CString(""), CString(""), CString(""), CString(""), CString(MONI64M_XX_TCRRARR), CString(MONI64M_X_TLMIN2) },
		{ CString(MONI64M_S_CAL), CString(MONI64M_X_CAL), CString(MONI64M_WEATHER), CString(""), CString(""), CString("") },
		{ CString(MONI64M_GREEN), CString(MONI64M_GREEN), CString(MONI64M_GREEN), CString(MONI64M_GREEN), CString(MONI64M_GREEN), CString("") },
	},
	{
		// 54m
		{ CString(MONI54M_ANT), CString(MONI54M_SSPA), CString(MONI54M_X_TXIF), CString(MONI54M_X_UC), CString(""), CString(MONI54M_X_CMDOUT2) },
		{ CString(""), CString(""), CString(""), CString(""), CString(""), CString("") },
		{ CString(MONI54M_X_LNA), CString(MONI54M_X_RXIF), CString(MONI54M_X_DC), CString(MONI54M_DSN_MODEM), CString(""), CString(MONI54M_X_TLMIN2) },
		{ CString(MONI54M_KA_LNA), CString(MONI54M_KA_DC), CString(""), CString(""), CString(""), CString("") },
		{ CString(""), CString(""), CString(""), CString(""), CString(MONI54M_REF_DIV), CString("") },
		{ CString(MONI54M_X_CAL), CString(MONI54M_X_TRANS), CString(MONI54M_TRANS_LO), CString(""), CString(""), CString("") },
		{ CString(MONI54M_KA_CAL), CString(MONI54M_KA_TRANS), CString(""), CString(""), CString(""), CString("") },
		{ CString(""), CString(""), CString(""), CString(""), CString(MONI54M_WEATHER), CString(MONI54M_CVCF) },
		{ CString(MONI54M_GREEN), CString(MONI54M_GREEN), CString(MONI54M_GREEN), CString(MONI54M_GREEN), CString(""), CString("") },
	},
	{
		// 34m
		{ CString(MONI34M_ANT), CString(MONI34M_S_TX), CString(MONI34M_S_TXSYN), CString(MONI34M_S_TXIFSW), CString(MONI34M_SS_TCRMOD), CString(MONI34M_S_CMDOUT) },
		{ CString(MONI34M_TX_POL), CString(MONI34M_S_TRKRX), CString(MONI34M_S_RXSYN), CString(MONI34M_S_RXIFSW), CString(MONI34M_SS_TCRDEM), CString(MONI34M_S_TLMIN) },
		{ CString(""), CString(MONI34M_SX_ACQRX), CString(""), CString(""), CString(MONI34M_SS_TCRRARR), CString("") },
		{ CString(MONI34M_X_TX), CString(MONI34M_X_IFSW), CString(MONI34M_X_TXSYN), CString(""), CString(MONI34M_DSNTCR_MOD), CString(MONI34M_X_CMDOUT2) },
		{ CString(MONI34M_X_RX), CString(MONI34M_X_RXSYN), CString(MONI34M_X_TRKRX), CString(""), CString(MONI34M_DSNTCR_DEM), CString(MONI34M_X_TLMIN2) },
		{ CString(""), CString(""), CString(MONI34M_X_HRX_DEM), CString(MONI34M_X_HRX_MOD), CString(MONI34M_X_HRXTLMIN), CString("") },
		{ CString(MONI34M_S_CAL), CString(MONI34M_X_CAL), CString(""), CString(""), CString(MONI34M_DSNTCR_RARR), CString("") },
		{ CString(MONI34M_GREEN), CString(MONI34M_GREEN), CString(MONI34M_GREEN), CString(MONI34M_GREEN), CString(MONI34M_TIME), CString(MONI34M_WEATHER) },
		{ CString(MONI34M_GREEN), CString(MONI34M_GREEN), CString(MONI34M_GREEN), CString(MONI34M_GREEN), CString(MONI34M_GREEN), CString(MONI34M_GREEN) },
	},
	{
		// 20m
		{ CString(MONI20M_ANT), CString(MONI20M_S_TXRF), CString(MONI20M_S_TXFCONV), CString(MONI20M_S_UCSYN), CString(MONI20M_SS_TCRMOD), CString(MONI20M_CMDOUT1) },
		{ CString(""), CString(""), CString(""), CString(""), CString(MONI20M_SX_TCRMOD), CString(MONI20M_CMDOUT2) },
		{ CString(MONI20M_S_RXRF), CString(MONI20M_S_DCSYN), CString(MONI20M_S_TCRDET), CString(""), CString(MONI20M_S_TCRDEM), CString(MONI20M_S_TLMIN) },
		{ CString(""), CString(MONI20M_SX_ACQDCSYN), CString(MONI20M_S_ACQDET), CString(MONI20M_S_ACQTCRDEM), CString(MONI20M_S_TCRRARR), CString("") },
		{ CString(""), CString(MONI20M_X_DCSYN), CString(MONI20M_X_TCRDET), CString(MONI20M_X_TCRRARR), CString(MONI20M_X_TCRDEM), CString(MONI20M_X_TLMIN) },
		{ CString(""), CString(""), CString(""), CString(MONI20M_X_HRX_DEM), CString(MONI20M_X_HRX_MOD), CString(MONI20M_X_HRXTLMIN) },
		{ CString(MONI20M_S_CALRF), CString(MONI20M_X_CALRF_UC), CString(MONI20M_SS_TRANSSYN), CString(MONI20M_SX_TRANSSYN), CString(MONI20M_X_TLMDEC), CString(MONI20M_X_TLMSIM) },
		{ CString(MONI20M_GREEN), CString(MONI20M_GREEN), CString(MONI20M_GREEN), CString(""), CString(""), CString(MONI20M_WEATHER) },
		{ CString(MONI20M_GREEN), CString(MONI20M_GREEN), CString(MONI20M_GREEN), CString(MONI20M_GREEN), CString(MONI20M_GREEN), CString(MONI20M_GREEN) },
	},
};


enum eHistory{
	eHistory_Operation,
	eHistory_Control,
	eHistory_Equipment,
	eHistory_Event,
	eHistory_Alarm,
	eHistory_SearchOperation,
	eHistory_SearchControl,
	eHistory_SearchEquipment,
	eHistory_SearchEvent,
	eHistory_SearchLimit,
	eHistory_SearchAlarm,
	eHistory_Max,
};

// 履歴情報
#define	MINCOLWIDTH		80
template <class T> inline const T& MAX(const T& t1, const T& t2) {
	return t1 < t2 ? t2 : t1;
}
#pragma pack(1)
typedef struct {
	WORD	year;			// 年
	BYTE	month;			// 月
	BYTE	day;			// 日
	BYTE	hour;			// 時
	BYTE	minute;			// 分
	BYTE	second;			// 秒
	BYTE	station;		// 局
	BYTE	equipment;		// 送信元
	BYTE	imstatus;		// 設備重要度
	BYTE	status;			// ステータス
	DWORD	message;		// メッセージID
	TCHAR	msgstring[1024];
	UINT	searchtype;
} stHistoryData;

typedef struct {
	bool	filter;
	WORD	station;
	BYTE	sender[eStation_MAX];
	BYTE	imstatus;
	BYTE	status;
	DWORD	message;
	DWORD	message2;
	SYSTEMTIME stime;
	SYSTEMTIME ptime;
	UINT	searchtype;
} stFilterData;
#pragma pack()

enum eFilterItem{
	eFilterItem_Station,
	eFilterItem_Time,
	eFilterItem_Equipment,
	eFilterItem_Status,
	eFilterItem_MessageId,
	eFilterItem_Message,
};

enum eFilterItemEq{
	eFilterItemEq_Station,
	eFilterItemEq_Time,
	eFilterItemEq_Equipment,
	eFilterItemEq_ImStatus,
	eFilterItemEq_Status,
	eFilterItemEq_MessageId,
	eFilterItemEq_Message,
};

enum eSearchtem{
	eSearchitem_Station,
	eSearchitem_Time,
	eSearchitem_Equipment,
	eSearchitem_ImStatus,
	eSearchitem_Status,
	eSearchitem_MessageId,
	eSearchitem_Message,
	eSearchitem_Max,
};

enum eFilterItemEvent{
	eFilterItemEvent_Station,
	eFilterItemEvent_Time,
	eFilterItemEvent_Status,
	eFilterItemEvent_MessageId,
	eFilterItemEvent_Message,
};

enum eDirectWindowID{
	eCtrl_AntEmg = (WM_USER+1000),
	eCtrl_SndStop,
	eCtrl_SnapShot,
	eCtrl_Sound,
	eCtrl_Mute,
	eCtrl_Help,
	eGridCtrl_Equipment = 510001,
	eGridCtrl_Operation,
	eTrackingCtrl,
	eSummaryCtrl,
	eSummaryCtrl2,
	eSummaryCtrl3,
	eSummaryCtrl4,
	eSummaryGraph,
	eSummaryGraph2,
	eSummaryGraph3,
	eSummaryGraph4,
	eTrendCtrl,
	eOperationHistory,
	eOperationSearch,
	eControlHistory,
	eControlSearch,
	eEquipmentHistory,
	eEquipmentSearch,
	eEventHistory,
	eEventSearch,
	eAlarmList,
};

// AGC校正 ループバンド
enum {
	AGCLB_1kHz,
	AGCLB_300Hz,
	AGCLB_100Hz,
	AGCLB_30Hz,
	AGCLB_10Hz,
	AGCLB_5p6Hz,
	AGCLB_3Hz,
	AGCLB_1p8Hz,
	AGCLB_1Hz,
	AGCLB_0p56Hz,
	AGCLB_0p3Hz,
	AGCLB_0p18Hz,
	AGCLB_01Hz,
	AGCLB_CURRENT_LBW,
};

static const CString strAGCLoopBand[] = {
	_T("1kHz"),
	_T("300Hz"),
	_T("100Hz"),
	_T("30Hz"),
	_T("10Hz"),
	_T("5.6Hz"),
	_T("3Hz"),
	_T("1.8Hz"),
	_T("1Hz"),
	_T("0.56Hz"),
	_T("0.3Hz"),
	_T("0.18Hz"),
	_T("0.1Hz"),
	_T("CURRENT_LBW"),
	_T(""),
};

#define CURRENT_LBW _T("CURRENT_LBW")

//=============================================================================
//	INIファイル
//=============================================================================
const CString SECTION_SYSTEM = _T("SYSTEM");
const CString KEY_STATIONMAX = _T("StationMax");	// 最大局数
const CString KEY_FILESERVER_NAME = _T("FileServerName");	// ファイルサーバー名称
const CString KEY_LOGLEVEL = _T("LogLevel");		// ログレベル
const CString SECTION_DEBUG = _T("DEBUG");
const CString KEY_TIMERATE = _T("TimeRate");

const CString KEY_SSHUSER_NAME = _T("SshUserName");		// SSHユーザ名称
const CString KEY_SSHSERVER_NAME = _T("SshServerName");	// SSHサーバ名称
const CString KEY_SSHKEY_PATH = _T("SshKeyFilePath");	// SSHキーファイルパス名称
const CString KEY_SSHBATCH_PATH = _T("SshBatchPath");	// SSHバッチファイルパス名称
const CString KEY_EVENTTONE_PATH = _T("EventTonePath");	// イベント音ファイル格納パス

const CString KEY_DRAWUPDATEINTERVAL = _T("DrawUpdateInterval");	// 描画更新間隔
const CString KEY_MESSAGETHREAD = _T("MessageThread");	// メッセージスレッド

const CString KEY_PREDDIFF_ENABLE = _T("PredDiffEnable");	// PRED DIFF計算有効無効(1:有効/0:無効)
const CString KEY_PREDDIFF_MIN = _T("PredDiffMin");		// 最小PRED DIFF間隔(54m)

const CString KEY_HISTID54 = _T("HistoryID54");			// 54mの履歴識別番号

const CString KEY_SERVICE_MODE = _T("ServiceMode");		// サービスモード（admin, sttn, ssoc, user)

static const TCHAR* mServiceModeString[]{
	_T("user"),		// ユーザモード・パスワード要
	_T("admin"),	// 管理者モード・パスワード不要
	_T("sttn"),		// 地上局モード・パスワード不要
	_T("ssoc"),		// 相模原モード・パスワード不要
	_T("limited"),	// 制限モード・パスワード要
	_T("unlimited"),	// 制限モード無し・パスワード要
	_T("oldremote"),	// 旧リモート化のモード
};
enum eServiceMode{
	eServiceMode_user,
	eServiceMode_admin,
	eServiceMode_sttn,
	eServiceMode_ssoc,
	eServiceMode_limited,
	eServiceMode_unlimited,
	eServiceMode_oldremote,
	eServiceMode_max,
};

const CString APP_TIMELINE = _T("OperationTimeLine.exe");	// 運用タイムラインアプリ名称
//=============================================================================
//	フレーム
//=============================================================================
#define OFFSET_EMGBUTTON_RIGHT	50
#define MAX_EMGBUTTON_WIDTH		120

//=============================================================================
//	トレンドグラフ
//=============================================================================
#define MAX_TDATA	13
#define MAX_SUMMARYTDATA	10
#define MAX_VIEW	 8
#define MAX_VSCALE	 2
#define MAX_HSCALE	 2

#define	DREAL_NAN_HIGH		0x7ff80000
#define	DREAL_NAN_LOW		0x00000000
#define	DREAL_ERROR_HIGH	0xffffffff
#define	DREAL_ERROR_LOW		0xfffffffe

#define MAX_OUTPUT_LIST		100

#define MAX_SPACEDATA_SIZE	(3600 * 24)	// 衛星生データ保持サイズ（1日）
//#define MAX_SPACEDATA_SIZE	(300)	// 衛星生データ保持サイズ動作確認テスト

//#define MAX_PPDATA_SIZE	(12 + 1)		// 動作確認テスト
//#define MAX_PPDATA_SIZE	(120 + 1)		// 動作確認テスト
#define MAX_PPDATA_SIZE		(3600 + 1)		// PP圧縮データ保持サイズ（ウィンドウ右端位置のデータ表示のため、+1する）
#define MAX_DATASELECT		80

#define MAX_SUMMARY_VIEW	4

enum eTrendWndType{
	eTrendWndType_AGC = 0,
	eTrendWndType_RNG,
	eTrendWndType_RR,
	eTrendWndType_EL,
	eTrendWndType_Num
};

enum eTrendWndSetType{
	eTrendWndSetType_AGC = 0,
	eTrendWndSetType_RNG_RR,
	eTrendWndSetType_AGC_RNG_EL,
	eTrendWndSetType_Num
};

enum eSummaryWndSetType{
	eSummaryWndSetType_RNG = 0,
	eSummaryWndSetType_RR,
	eSummaryWndSetType_XSP,
	eSummaryWndSetType_KASP,
	eSummaryWndSetType_SAGC,
	eSummaryWndSetType_XAGC,
	eSummaryWndSetType_Num
};

enum eMessagex{
	// ウィンドウ系
	eMessage_ChildActivate = (WM_USER + 100),
	eMessage_ChildDestroy,
	eMessage_DirectPrint,
	eMessage_WindowClose,

	// スレッド系
	eMessage_ThreadNormal,
	eMessage_ThreadError,
	eMessage_ThreadFail,
	eMessage_ThreadException,
	eMessage_ThreadEvent,

	// プロパティ系
	eMessage_PropetyChange,
	eMessage_SummaryPropetyChange,
	eMessage_PropetyCmbChange,
	eMessage_AutoClear,

	// 運用タイムライン
	eMessage_TimeLineHandle = (WM_USER + 6000),	// ウィンドウハンドル
	eMessage_TimeLineFinish,					// アプリ終了
	eMessage_TimeLinePlan,						// 計画立案
	eMessage_TimeLineForcast,					// 予報値一覧
	eMessage_TimeLineEmg,						// アンテナ強制停止
	eMessage_TimeLineStop,						// 送信機停止

	// 設備状態
	eMessage_EqStatusControl = (WM_USER + 7000),	// コントロールハンドル

	// トレンドグラフ
	eMessage_InitTrendWnd = (WM_USER + 8000),		// トレンドグラフウィンドウ初期化
	eMessage_OpenTrendSetDlg,						// トレンドグラフ設定ウィンドウ呼び出し
	eMessage_OpenSummaryTrendSetDlg,				// サマリトレンドグラフ設定ウィンドウ呼び出し
	eMessage_OpenSummaryTrendSetDlgChg,				// サマリトレンドグラフ設定ウィンドウ呼び出し（ダイアログをスペクトラム設定からトレンド設定に変更）
	eMessage_UpdateSummaryTabCombo,					// サマリグラフタブコンボ更新通知
	eMessage_UpdateForecastData,					// 予報値データ更新通知
	eMessage_UpdateEngValData,						// 工学値変換データ更新通知
	eMessage_NewEngValData,							// 新しく工学値変換データを受けた通知

	// スペクトラムグラフ
	eMessage_OpenSpectrumSetDlg = (WM_USER + 8100),		// スペクトラム設定ウィンドウ呼び出し
	eMessage_OpenSummarySpectrumSetDlg,					// サマリスペクトラムグラフ設定ウィンドウ呼び出し
	eMessage_OpenSummarySpectrumSetDlgChg,				// サマリスペクトラムグラフ設定ウィンドウ呼び出し（ダイアログをトレンド設定からスペクトラム設定に変更）

	// 予報値一覧
	eMessage_OpenForecastListDlg = (WM_USER + 8200),	// 予報値一覧呼び出し
	eMessage_UpdateForecastFilePath,					// 予報値ファイルパス更新

	// サマリグラフ
	eMessage_InitSummaryWnd = (WM_USER + 8300),				// サマリグラフウィンドウ初期化

	// 共有ファイル
	eMessage_UpdateShareFile = (WM_USER + 8400),			// 共有ファイルの更新

	// SSH
	eMessage_CompleteSsh = (WM_USER + 8500),				// SSH完了

	// セッション状態
	eMessage_UpdateSessionStatus = (WM_USER + 8600),		// セッション状態更新

	// 逐次制御
	eMessage_UpdateSeqCtrlPos = (WM_USER + 8700),			// 逐次制御の行位置更新
	eMessage_PauseCompltete,								// ポーズ完了
	eMessage_WarnSeqCtrlSetDefFile,							// 逐次制御の定義ファイル指定失敗ワーニング
	eMessage_WarnSeqCtrlExec,								// 逐次制御実行失敗ワーニング
	eMessage_WarnSeqCtrlPause,								// 逐次制御のポーズ失敗ワーニング
	eMessage_WarnSeqCtrlSelLinePos,							// 逐次制御の行指定失敗ワーニング
	eMessage_WarnSeqCtrlEnd,								// 逐次制御の中断失敗ワーニング

	// 履歴
	eMessage_History,

	// 校正診断
	eMessage_RdnsChkTimeout = (WM_USER + 8900),				// 校正診断タイムアウト
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
}VScaleKind;

typedef struct {
	bool	Auto;
	double	VScalLMax;
	double	VScalLMin;
	double	VScalRMax;
	double	VScalRMin;
	long	VSCalItvl;
} stVScale;

typedef enum {
	m1,
	m5,
	m10,
	m20,
	m30,
	m60,
	h2,
	h4,
	h8,
	h12,
	h16,
	h20,
	h24,
//	autoscale,
	yohochiscale,
}HScale;

static const CString strHScale[] = {
	_T("1分"),
	_T("5分"),
	_T("10分"),
	_T("20分"),
	_T("30分"),
	_T("60分"),
	_T("2時間"),
	_T("4時間"),
	_T("8時間"),
	_T("12時間"),
	_T("16時間"),
	_T("20時間"),
	_T("24時間"),
//	_T("オート"),
	_T("予報値に合わせる"),
	_T(""),
};
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
	RGB(128, 128, 128),	// gray
	RGB(0, 0, 128),		// navy
	RGB(128, 0, 0),		// maroon
	RGB(128, 0, 128),	// purple
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
	long		lineWidth;
	CString		DotSize;
	CString		SelVScale;
	CString		dataUnit;
	double		Alarm1Major;
	double		Alarm1Minor;
	double		Alarm2Major;
	double		Alarm2Minor;
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
	_T("60分"),
	_T("リアルモード"),
	_T("On"),
	_T("Line"),
	1,
	_T("Small"),
	_T("左"),
	_T(""),
	0,
	0,
	0,
	0,
};

static const stPropData initSummaryPropData = {
	_T("On"),
	_T(""),
	100,
	-100,
	50,
	-50,
	4,
	_T("24時間"),
	_T("60分"),
	_T("リアルモード"),
	_T("On"),
	_T("Line"),
	1,
	_T("Small"),
	_T("左"),
	_T(""),
	0,
	0,
	0,
	0,
};

typedef struct
{
	CEvent*		pTermThreadEvent;	// スレッドイベントを終了
	CWnd*		pWnd;				// ウィンドウハンドル
	bool		bPassStatus;		// パス状況画面フラグ
	UINT		nStation;			// 選択局番号
	int			nExecuteMode;		// 制御モード、監視モード
} stThreadInfo;


//=============================================================================
//	監視データ
//=============================================================================

typedef struct {
	double	max;
	double	min;
	__int64	count;
} stPPPair;

typedef struct  {
	DWORD		idx;		// インデックス
	SYSTEMTIME	systime;	// 時刻
	//DWORD		dataid;		// データID
	//TCHAR		name[80];	// データ名称
	//DWORD		kind;		// データ種別
	double		value;		// データ
	BYTE		status[8];	// ステータス
	stPPPair	ppPair;		// PP圧縮
	TCHAR		res[16];
} stSpaceData;

typedef struct {
	DWORD	id;			// データID
	CString	name;		// データ名称
} stSelectData;

enum eStatus{
	eStatus_None,		// データ無し
	eStatus_Enable,		// データ有効
	eStatus_Disable,	// データ無効（例えばデータ抜け等）
};


class CSpaceData
{
public:
	CSpaceData(){};
	virtual ~CSpaceData(){};

public:
	DWORD			dataid;		// データID
	CString			dataname;	// データ名
	DWORD			kind;		// データ種別
	int				srcType;	// 0:オンラインデータ / 1:ファイルデータ
	__int64			totalSize;	// キュー削除前のトータルサイズ
	deque<stSpaceData>	value;	// トレンドデータキュー
};

typedef struct {
	DWORD		dataid;		// データID
	SYSTEMTIME	systime;	// 時刻
} stDataIdTime;


//=============================================================================
//	セッション
//=============================================================================
enum eSessionType
{
	eSessionType_Monitor = 0,	// 監視セッション
	eSessionType_Ctrl,			// 制御セッション
	eSessionType_Notify,		// 通知セッション
	eSessionType_History,		// 履歴セッション
	eSessionType_Num			// セッション数
};

//=============================================================================
//	受信周波数モード
//=============================================================================
enum eRcvFreqModeType
{
	eRcvFreqModeType_1Way = 0,	// 1WAY
	eRcvFreqModeType_2Way,		// 2WAY
//	eRcvFreqModeType_3Way,		// 3WAY
	eRcvFreqModeType_Dup,		// DUP
	eRcvFreqModeType_Other,		// Other
	eRcvFreqModeType_Num,		// モード数
};

//=============================================================================
//	制御モード
//=============================================================================
enum eCtrlModeType
{
	eCtrlModeType_Manual = 0,	// 手動
	eCtrlModeType_Auto,			// 自動
	eCtrlModeType_Null,			// NULL
	eCtrlModeType_Num,			// モード数
};

//=============================================================================
//	X帯<=>Ka帯切り替え
//=============================================================================
enum eBandSelectType
{
	eBandSelectType_X = 0,		// X帯
	eBandSelectType_Ka,			// Ka帯
	eBandSelectType_Null,		// NULL
	eBandSelectType_Num,		// モード数
};

//=============================================================================
//	RF TRIP監視
//=============================================================================
enum eRfTripMonitorType
{
	eRfTripMonitorType_Manual = 0,	// 手動
	eRfTripMonitorType_Auto,		// 自動
	eRfTripMonitorType_Null,		// NULL
	eRfTripMonitorType_Num,			// モード数
};

//=============================================================================
//	追尾状況グラフ
//=============================================================================
/*--------------*/
/* 運用状況関係 */
/*--------------*/
#define  UNYOU_OUT        0         /* 運用状況（非運用状態）         */
#define  UNYOU_START      1         /* 運用状況（運用開始30分以内前） */
#define  UNYOU_IN         2         /* 運用状況（運用中）             */
#define  UNYOU_TIME  604800         /* 運用切り替え開始時間（７日）   */
#define  DSP_ON           1			/* 表示切り替え要                 */
#define  DSP_OFF          0			/* 表示切り替え不要               */
#define  YOHOUTI_ARI      1			/* 予報値ファイル有り             */
#define  YOHOUTI_NASI     0			/* 予報値ファイルなし             */
#define  YOHOGET_MATI     1         /* 予報値ファイル取得待ち(V2-5)   */
#define  YOHOGET_SUMI     2         /* 予報値ファイル取得済み(V2-5)   */
#define  YOHOGET_OK       3         /* 予報値ファイル取得可能(V2-5)   */

#define  INFLG_OFF	    0							// 監視データ無効
#define  INFLG_ON	    1				            // 監視データ有効

#define  UNYOU_FLAG		"CTRL.PASS_FLAG"			/* 運用パス状態フラグ		  */
#define  TILL_TIME 		"CTRL.PASS_CNTDOWN(DEC)"	/* 運用パス開始までの時間	  */
#define  START_TIME		"CTRL.START_TIME"    		/* パス開始時刻               */
#define  STOP_TIME		"CTRL.END_TIME"     		/* パス終了時刻               */
#define  PREDFILE_NAME	"CTRL.PRED_FILE"			/* 予報値ファイルパス名       */
#define  PREDFILE_TIME	"CTRL.MAKE_PRED_TIME" 		/* 予報値ファイル作成時刻     */
#define  PLAN_NO		"CTRL.PASS_NO"				/* 計画番号                   */
#define  EISEI_NAME		"CTRL.SAT_NAME"				/* 衛星名                     */
#define  AOS_TIME		"CTRL.PRED_AOS_TIME"		/* ＡＯＳ時刻                 */
#define  AOS_AZ			"CTRL.AZ_AT_AOS"			/* ＡＯＳ ＡＺ値              */
#define  AOS_EL			"CTRL.EL_AT_AOS"			/* ＡＯＳ ＥＬ値              */
#define  LOS_TIME		"CTRL.PRED_LOS_TIME"		/* ＬＯＳ時刻                 */
#define  LOS_AZ			"CTRL.AZ_AT_LOS"			/* ＬＯＳ ＡＺ値              */
#define  LOS_EL			"CTRL.EL_AT_LOS"			/* ＬＯＳ ＥＬ値              */
#define  MAXEL_TIME		"CTRL.PRED_MAX_EL_TIME"		/* ＭＡＸ ＥＬ時刻            */
#define  MAXEL_AZ		"CTRL.AZ_AT_MAX_EL"			/* ＭＡＸ ＥＬ ＡＺ値         */
#define  MAXEL_EL		"CTRL.EL_AT_MAX_EL"			/* ＭＡＸ ＥＬ ＥＬ値         */
#define  VISIBLE		"CTRL.VISIBLE"				/* ＶＩＳＩＢＬＥ(HH:MM)      */
#define  YOHOU_GET      "CTRL.PRED_COND"            /* 予報値取得状態         	  */
#define  REAL_AZ		"ANT.REAL_AZ"				/* AZ実測値 */
#define  REAL_EL		"ANT.REAL_EL"				/* EL実測値 */
#define  CTRL_MON_TIME	"CTRL.MON_TIME"				/* 追尾の現在の監視時刻 */

//=============================================================================
//	グラフ用データ数
//=============================================================================
#define	 GRDATA_ID_MAX   10		/* グラフ監視項目最大数               */
#define	 GRNUM_AGC_MAX   10		/* ＡＧＣグラフ監視項目数             */
#define	 GRNUM_RNG_MAX   8		/* ＲＮＧ，ＲＲグラフ監視項目数		　*/
#define	 GRNUM_PRS_MAX   2		/* 追尾状況グラフ監視項目数           */

#define  GRDATA_NUM_MAX  86400	/* グラフデータＭＡＸ件数             */
#define  GRDATA_NUM_AGC  86400	/* ＡＧＣグラフデータ件数             */
#define  GRDATA_NUM_RNG  86400	/* ＲＮＧ，ＲＲグラフデータ件数       */
#define  GRDATA_NUM_PRS  86400	/* 追尾状況グラフデータ件数           */
#define  GRDATA_NUM_AGC_SH  864000	/* ＡＧＣグラフデータ件数(短周期) */

// 追尾グラフタイプ
enum eRadarViewType
{
	eRadarViewType_Station = 0,
	eRadarViewType_ForecastList,
	eRadarViewType_OperationPlan,
};

struct Az_El {
	double d_AZ;						// AZデータ
	double d_EL;						// ELデータ
	double d_time;						// 時刻
};

struct Forecast_Az_El {
	double	d_AZ;
	double	d_EL;
	double  d_aos;
	double	d_time;
	int		i_roop_cnt;
};

//=============================================================================
//	グラフ表示保存データ格納構造
//=============================================================================
struct   shmdata_t {            /* グラフ表示保存データ格納構造       */
	long   l_id;				/* データＩＤ(領域分割使用）          */
	long   l_size;				/* データＩＤ毎領域サイズ             */
	long   l_end;				/* 前回最終格納データ位置[]           */
	double d_endtime;			/* 前回最終格納データ時刻             */
	double d_data[GRDATA_NUM_MAX];/* トレンドデータ格納エリア(毎秒）    */
};

/* 短周期用データ格納エリア */
struct   shmdata_sh_t {         /* グラフ表示保存データ格納構造       */
	long   l_id;				/* データＩＤ(領域分割使用）          */
	long   l_size;				/* データＩＤ毎領域サイズ             */
	long   l_end;				/* 前回最終格納データ位置[]           */
	double d_endtime;			/* 前回最終格納データ時刻             */
	double d_data[GRDATA_NUM_AGC_SH];/* トレンドデータ格納エリア(毎秒） */
};

//=============================================================================
//	スペクトラムグラフ表示定義
//=============================================================================
#define SPECTRUM_NUM        ( 1024 )
#define SPECTRUM_NUM_MIN    ( 0 )
#define SPECTRUM_NUM_MAX    ( 1023 )
#define SPECTRUM_INTERVAL        ( 300 ) /* タイムアウトコールバック呼び出し間隔[msec] */
#define SPECTRUM_MONIP_70MIF_SEL ( 0 )
#define SPECTRUM_MONIP_CL_SEL    ( 1 )
#define SPECTRUM_MONIP_CLA_SEL   ( 2 )
#define SPECTRUM_EXEC_CMD        _T("opctproc")	/* 制御実行コマンド名 */
/* スペクトラムファイル設定 */
#define SPECTRUM_FILE_AUTO_SCALE	_T("AutoScale")     /* オートスケール識別 */	
#define SPECTRUM_FILE_GRID_MIN   _T("GridMin")		/* グリッド最小値識別 */
#define SPECTRUM_FILE_GRID_MAX   _T("GridMax")       /* グリッド最大値識別 */
#define SPECTRUM_FILE_GRID_VAL   _T("GridVal")       /* グリッド値識別 */
#define SPECTRUM_FILE_GRID_OFS   _T("Offset")        /* グリッドオフセット値識別 */
#define SPECTRUM_PEAK_DEF   ( -160.0 )
// ラジアン
#define	SPECTRUM_RADCV	(3.1415926538/180.0)
// 桁定義
#define SPECTRUM_MAXDIGIT	(1 + 9 + 2 + 1)		// マイナス +  整数部 + カンマ + 文字列終わり 

#define SPECTRUM_FREQ_OFS_MIN  ( double )( -4000000.000 )
#define SPECTRUM_FREQ_OFS_MAX  ( double )(  4000000.000 )

#define SPECTRUM_INPUTMAX 100000000
#define SPECTRUM_INPUTMIN -100000000
#define SPECTRUM_AUTOSCALE_ON	0
#define SPECTRUM_AUTOSCALE_OFF	1

// ファイルヘッダ
#define SPECTRUM_HEADER		("!#HEAD")

enum eSpectrumGraphType{
	eSpectrumGraphType_Normal = 0,
	eSpectrumGraphType_Sammary
};

// 監視データ関連定義
#define MON_TIME_TAG_NAME		"DSNTCR_SPE.TIME_TAG"
#define MON_MONI_POINT_NAME     "DSNTCR_SPE.SPEC_MONI_POINT"
#define MON_MEASURE_BW_NAME		"DSNTCR_SPE.SPEC_MEASURE_BW"
#define MON_SPECTRUM_NAME		"DSNTCR_SPE.SPECTRUM_%d"
#define MON_FREQ_OFFSET_NAME    "DSNTCR_SPE.SPEC_FRE_OFFSET"
#define CTL_MONI_POINT_NAME     "DSNTCR.SPEC_MONI_POINT"
#define CTL_MEASURE_BW_NAME     "DSNTCR.SPEC_MEASURE_BW"
#define CTL_FREQ_OFFSET_NAME    "DSNTCR.SPEC_FRE_OFFSET"
#define MON_CENTER_FREQ_NAME    "DSNTCR_SPE.SPEC_CENTER_FREQ"

//=============================================================================
//	衛星捕捉グラフ定義
//=============================================================================
// 衛星捕捉グラフ用の局.設備.項目名と簡易表示名のセット
typedef struct {
	int		nSigType;				// 信号タイプ
	CString strSigName;				// 局.設備.項目名
	CString strSimpleSigName;		// 簡易表示名
	CString strUnit;				// 単位表示名
	int		nIntDidits;				// 整数部桁数
	int		nDecDidits;				// 少数点以下桁数
} structSatelliteCaptureSig;

// 衛星捕捉グラフの信号元タイプ
enum eSigSrcType
{
	eSigSrcType_EngExchange = 0,	// 工学値変換データ
	eSigSrcType_File,				// ファイルデータ
	eSigSrcType_Num
};

// 衛星捕捉グラフ監視項目名
// 臼田64m局
// AGC
#define UDSC64_AGC_SS_TCR_SIG		_T("SS-TCRAGC.AGC_A_AVG")
#define UDSC64_AGC_X_RX2_SIG		_T("X-RX2AGC.AGC_A_AVG")
#define UDSC64_AGC_XX_TCR_SIG		_T("DSNTCR_AGAV.AGC_A_AVG")
#define UDSC64_AGC_XRX2_INTEG_SIG	_T("X-RX2AGC.AGC_A_TKSC")
#define UDSC64_AGC_XXTCR_INTEG_SIG	_T("DSNTCR_AGAV.AGC_A_TKSC")

// RNG
#define UDSC64_RNG_SS_SIG			_T("CTRL.AMB_SS_DATA_KM")
#define UDSC64_RNG_XX_SIG			_T("CTRL.AMB_XX_DATA_KM")
#define UDSC64_RNG_XX_REPLAY_SIG	_T("CTRL.AMB_RX_DATA_KM")
#define UDSC64_RNG_XX_TCR_SIG		_T("CTRL.AMB_XX_DSN_DATA_KM")
#define UDSC64_RNG_PRED_SIG			_T("RNG(PRED)")

// RR
#define UDSC64_RR_SS_SIG			_T("CTRL.DIST_SS_DATA")
#define UDSC64_RR_XX_SIG			_T("CTRL.DIST_XX_DATA")
#define UDSC64_RR_XX_TCR_SIG		_T("CTRL.DIST_XX_DSN_DATA")
#define UDSC64_RR_PRED_SIG			_T("RR(PRED)")

// EL
#define UDSC64_EL_EL_SIG			_T("ANT.REAL_EL")
#define UDSC64_EL_EL_PRED_SIG		_T("EL(PRED)")
#define UDSC64_EL_SKYLINE_SIG		_T("SKYLINE")
#define UDSC64_EL_RF_TRIP_SIG		_T("RF-TRIP")


// 臼田54m局
// AGC
#define XDSC54_AGC_SS_TCR_SIG		_T("SS-TCRAGC.AGC_A_AVG")
#define XDSC54_AGC_X_RX2_SIG		_T("X-RX2AGC.AGC_A_AVG")
#define XDSC54_AGC_XX_TCR_SIG		_T("DSNTCR_AGAV.AGC_A_AVG")
#define XDSC54_AGC_XRX2_INTEG_SIG	_T("X-RX2AGC.AGC_A_TKSC")
#define XDSC54_AGC_XXTCR_INTEG_SIG	_T("DSNTCR_AGAV.AGC_A_TKSC")
//#define XDSC54_AGC_DSNMODEM_SIG		_T("DSNTCR_AGAV.AGC_A_AVG")	
#define XDSC54_AGC_DSNMODEM_SIG		_T("DSNTCR_AGSH.AGC_A_AVG")
#define XDSC54_AGC_DSNMODEM_INTEG_SIG	_T("DSNTCR_AGAV.AGC_A_TKSC")		// DataCollectionで局エリアで置き換え

// RNG
#define XDSC54_RNG_SS_SIG			_T("CTRL.AMB_SS_DATA_KM")
#define XDSC54_RNG_XX_SIG			_T("CTRL.AMB_XX_DATA_KM")
#define XDSC54_RNG_XX_REPLAY_SIG	_T("CTRL.AMB_RX_DATA_KM")
#define XDSC54_RNG_XX_TCR_SIG		_T("CTRL.AMB_XX_DSN_DATA_KM")
#define XDSC54_RNG_DSNMODEM_X_SIG	_T("CTRL.AMB_XX_DSN_DATA_KM")
#define XDSC54_RNG_DSNMODEM_KA_SIG	_T("CTRL.AMB_XKa_DSN_DATA_KM")
#define XDSC54_RNG_PRED_SIG			_T("RNG(PRED)")

// RR
#define XDSC54_RR_SS_SIG			_T("CTRL.DIST_SS_DATA")
#define XDSC54_RR_XX_SIG			_T("CTRL.DIST_XX_DATA")
#define XDSC54_RR_XX_TCR_SIG		_T("CTRL.DIST_XX_DSN_DATA")
#define XDSC54_RR_DSNMODEM_X_SIG	_T("CTRL.DIST_XX_DSN_DATA")
#define XDSC54_RR_DSNMODEM_KA_SIG	_T("CTRL.DIST_XKa_DSN_DATA")
#define XDSC54_RR_PRED_SIG			_T("RR(PRED)")

// EL
#define XDSC54_EL_EL_SIG			_T("ANT.REAL_EL")
#define XDSC54_EL_EL_PRED_SIG		_T("EL(PRED)")
#define XDSC54_EL_SKYLINE_SIG		_T("SKYLINE")
#define XDSC54_EL_RF_TRIP_SIG		_T("RF-TRIP")


// 内之浦34m局
// AGC
#define USC34_AGC_SS_TCR_A_SIG		_T("SS-TCRAGC.AGC_A_AVG")
#define USC34_AGC_SS_TCR_B_SIG		_T("SS-TCRAGC.AGC_B_AVG")
#define USC34_AGC_S_TRKRX_A_SIG		_T("S-RXAGC.AGC_A_AVG")
#define USC34_AGC_S_TRKRX_B_SIG		_T("S-RXAGC.AGC_B_AVG")
#define USC34_AGC_X_RX_A_SIG		_T("X-TRKRXAGC.AGC_A_AVG")
#define USC34_AGC_X_RX_B_SIG		_T("X-TRKRXAGC.AGC_B_AVG")
#define USC34_AGC_S_ACQRX_A_SIG		_T("S-ACQRXAGC.AGC_A_AVG")
#define USC34_AGC_S_ACQRX_B_SIG		_T("S-ACQRXAGC.AGC_B_AVG")
#define USC34_AGC_X_ACQRX_A_SIG		_T("X-ACQRXAGC.AGC_A_AVG")
#define USC34_AGC_X_ACQRX_B_SIG		_T("X-ACQRXAGC.AGC_B_AVG")
#define USC34_AGC_X_HRX_SIG			_T("X-HRXAGC.AGC_A_AVG")
#define USC34_AGC_SHXX_TCR_SIG		_T("DSNTCR_AGSH.AGC_A_AVG")
#define USC34_AGC_INTEG_SIG			_T("DSNTCR_AGAV.AGC_A_TKSC")

// RNG
#define USC34_RNG_SS_SIG			_T("CTRL.AMB_SS_DATA_KM")
#define USC34_RNG_XX_SIG			_T("CTRL.AMB_XX_DSN_DATA_KM")
#define USC34_RNG_PRED_SIG			_T("RNG(PRED)")

// RR
#define USC34_RR_SS_SIG				_T("CTRL.DIST_SS_DATA")
#define USC34_RR_XX_SIG				_T("CTRL.DIST_XX_DSN_DATA")
#define USC34_RR_PRED_SIG			_T("RR(PRED)")

// EL
#define USC34_EL_EL_SIG				_T("ANT.REAL_EL")
#define USC34_EL_EL_PRED_SIG		_T("EL(PRED)")
#define USC34_EL_SKYLINE_SIG		_T("SKYLINE")
#define USC34_EL_RF_TRIP_SIG		_T("RF-TRIP")


// 内之浦20m局
// AGC
#define USC20_AGC_SS_TCR_A_SIG		_T("SS-TCRAGC.AGC_A_AVG")
#define USC20_AGC_SS_TCR_B_SIG		_T("SS-TCRAGC.AGC_B_AVG")
#define USC20_AGC_S_DET_A_SIG		_T("SS-TCRDTAGC.AGC_A_AVG")
#define USC20_AGC_S_DET_B_SIG		_T("SS-TCRDTAGC.AGC_B_AVG")
#define USC20_AGC_SX_TCR_A_SIG		_T("SX-TCRAGC.AGC_A_AVG")
#define USC20_AGC_SX_TCR_B_SIG		_T("SX-TCRAGC.AGC_B_AVG")
#define USC20_AGC_S_ACQTCR_A_SIG	_T("S-ACQTCRAGC.AGC_A_AVG")
#define USC20_AGC_S_ACQTCR_B_SIG	_T("S-ACQTCRAGC.AGC_B_AVG")
#define USC20_AGC_X_HRX_SIG			_T("X-HRXAGC.AGC_A_AVG")

// RNG
#define USC20_RNG_SS_SIG			_T("CTRL.AMB_SS_DATA_KM")
#define USC20_RNG_PRED_SIG			_T("RNG(PRED)")

// RR
#define USC20_RR_SS_SIG				_T("CTRL.DIST_SS_DATA")
#define USC20_RR_PRED_SIG			_T("RR(PRED)")

// EL
#define USC20_EL_EL_SIG				_T("ANT.REAL_EL")
#define USC20_EL_EL_PRED_SIG		_T("EL(PRED)")
#define USC20_EL_SKYLINE_SIG		_T("SKYLINE")
#define USC20_EL_RF_TRIP_SIG		_T("RF-TRIP")


// 衛星捕捉グラフ監視表示名
// 臼田64m局
// AGC
#define UDSC64_AGC_SS_TCR_NAME		_T("SS-TCR")
#define UDSC64_AGC_X_RX2_NAME		_T("X-RX2")
#define UDSC64_AGC_XX_TCR_NAME		_T("XX-TCR")
#define UDSC64_AGC_XRX2_INTEG_NAME	_T("INTEG")
//#define UDSC64_AGC_XXTCR_INTEG_NAME	_T("XX-TCR(INTEG)")
#define UDSC64_AGC_XXTCR_INTEG_NAME	_T("INTEG")
//#define UDSC64_AGC_INTEG_NAME		_T("INTEG")

// RNG
#define UDSC64_RNG_SS_NAME			_T("S/S")
#define UDSC64_RNG_XX_NAME			_T("X/X")
#define UDSC64_RNG_XX_REPLAY_NAME	_T("X/X再生型")
#define UDSC64_RNG_XX_TCR_NAME		_T("XX-TCR")
#define UDSC64_RNG_PRED_NAME		_T("PRED")

// RR
#define UDSC64_RR_SS_NAME			_T("S/S")
#define UDSC64_RR_XX_NAME			_T("X/X")
#define UDSC64_RR_XX_TCR_NAME		_T("XX-TCR")
#define UDSC64_RR_PRED_NAME			_T("PRED")

// EL
#define UDSC64_EL_EL_NAME			_T("EL")
#define UDSC64_EL_EL_PRED_NAME		_T("EL(PRED)")
#define UDSC64_EL_SKYLINE_NAME		_T("SKYLINE")
#define UDSC64_EL_RF_TRIP_NAME		_T("RF-TRIP")


// 臼田54m局
// AGC
#define XDSC54_AGC_SS_TCR_NAME		_T("SS-TCR")
#define XDSC54_AGC_X_RX2_NAME		_T("X-RX2")
#define XDSC54_AGC_XX_TCR_NAME		_T("XX-TCR")
#define XDSC54_AGC_DSNMODEM_NAME		_T("DSN-MODEM")
//#define XDSC54_AGC_XRX2_INTEG_NAME	_T("X-RX2(INTEG)")
//#define XDSC54_AGC_XXTCR_INTEG_NAME	_T("XX-TCR(INTEG)")
#define XDSC54_AGC_XXTCR_INTEG_NAME	_T("INTEG")
#define XDSC54_AGC_DSNMODEM_INTEG_NAME	_T("INTEG")
//#define XDSC54_AGC_INTEG_NAME		_T("INTEG")

// RNG
#define XDSC54_RNG_SS_NAME			_T("S/S")
#define XDSC54_RNG_XX_NAME			_T("X/X")
#define XDSC54_RNG_XX_REPLAY_NAME	_T("X/X再生型")
#define XDSC54_RNG_XX_TCR_NAME		_T("XX-TCR")
#define XDSC54_RNG_DSNMODEM_X_NAME	_T("DSN-MODEM(X)")
#define XDSC54_RNG_DSNMODEM_KA_NAME	_T("DSN-MODEM(Ka)")
#define XDSC54_RNG_PRED_NAME		_T("PRED")

// RR
#define XDSC54_RR_SS_NAME			_T("S/S")
#define XDSC54_RR_XX_NAME			_T("X/X")
#define XDSC54_RR_XX_TCR_NAME		_T("XX-TCR")
#define XDSC54_RR_DSNMODEM_X_NAME	_T("DSN-MODEM(X)")
#define XDSC54_RR_DSNMODEM_KA_NAME	_T("DSN-MODEM(Ka)")
#define XDSC54_RR_PRED_NAME			_T("PRED")

// EL
#define XDSC54_EL_EL_NAME			_T("EL")
#define XDSC54_EL_EL_PRED_NAME		_T("EL(PRED)")
#define XDSC54_EL_SKYLINE_NAME		_T("SKYLINE")
#define XDSC54_EL_RF_TRIP_NAME		_T("RF-TRIP")


// 内之浦34m局
// AGC
#define USC34_AGC_SS_TCR_A_NAME		_T("SS-TCR(A)")
#define USC34_AGC_SS_TCR_B_NAME		_T("SS-TCR(B)")
#define USC34_AGC_S_TRKRX_A_NAME	_T("S-TRKRX(A)")
#define USC34_AGC_S_TRKRX_B_NAME	_T("S-TRKRX(B)")
#define USC34_AGC_X_RX_A_NAME		_T("X-TRKRX(A)")
#define USC34_AGC_X_RX_B_NAME		_T("X-TRKRX(B)")
#define USC34_AGC_S_ACQRX_A_NAME	_T("S-ACQRX(A)")
#define USC34_AGC_S_ACQRX_B_NAME	_T("S-ACQRX(B)")
#define USC34_AGC_X_ACQRX_A_NAME	_T("X-ACQRX(A)")
#define USC34_AGC_X_ACQRX_B_NAME	_T("X-ACQRX(B)")
#define USC34_AGC_X_HRX_NAME		_T("X-HRX")
#define USC34_AGC_SHXX_TCR_NAME		_T("XX-TCR")
#define USC34_AGC_INTEG_NAME		_T("INTEG")

// RNG
#define USC34_RNG_SS_NAME			_T("S/S")
#define USC34_RNG_XX_NAME			_T("X/X")
#define USC34_RNG_PRED_NAME			_T("PRED")

// RR
#define USC34_RR_SS_NAME			_T("S/S")
#define USC34_RR_XX_NAME			_T("X/X")
#define USC34_RR_PRED_NAME			_T("PRED")

// EL
#define USC34_EL_EL_NAME			_T("EL")
#define USC34_EL_EL_PRED_NAME		_T("EL(PRED)")
#define USC34_EL_SKYLINE_NAME		_T("SKYLINE")
#define USC34_EL_RF_TRIP_NAME		_T("RF-TRIP")


// 内之浦20m局
// AGC
#define USC20_AGC_SS_TCR_A_NAME		_T("SS-TCR(A)")
#define USC20_AGC_SS_TCR_B_NAME		_T("SS-TCR(B)")
#define USC20_AGC_S_DET_A_NAME		_T("S-DET(A)")
#define USC20_AGC_S_DET_B_NAME		_T("S-DET(B)")
#define USC20_AGC_SX_TCR_A_NAME		_T("SX-TCR(A)")
#define USC20_AGC_SX_TCR_B_NAME		_T("SX-TCR(B)")
#define USC20_AGC_S_ACQTCR_A_NAME	_T("S-ACQTCR(A)")
#define USC20_AGC_S_ACQTCR_B_NAME	_T("S-ACQTCR(B)")
#define USC20_AGC_X_HRX_NAME		_T("X-HRX")

// RNG
#define USC20_RNG_SS_NAME			_T("S/S")
#define USC20_RNG_PRED_NAME			_T("PRED")

// RR
#define USC20_RR_SS_NAME			_T("S/S")
#define USC20_RR_PRED_NAME			_T("PRED")

// EL
#define USC20_EL_EL_NAME			_T("EL")
#define USC20_EL_EL_PRED_NAME		_T("EL(PRED)")
#define USC20_EL_SKYLINE_NAME		_T("SKYLINE")
#define USC20_EL_RF_TRIP_NAME		_T("RF-TRIP")


// サマリグラフ監視項目名
// 臼田64m局
// 衛星捕捉に含まれている

// 臼田54m局
// 衛星捕捉に含まれている

// 内之浦34m局
// S-AGC
#define USC34_SAGC_TRKRXAGC_A_SIG	_T("S-ACQ_TRKRXAGC.AGC_A_AVG")
#define USC34_SAGC_TRKRXAGC_B_SIG	_T("S-ACQ_TRKRXAGC.AGC_B_AVG")

// X-AGC
#define USC34_XAGC_ACQ_TRKRXAGC_A_SIG	_T("X-ACQ_TRKRXAGC.AGC_A_AVG")
#define USC34_XAGC_ACQ_TRKRXAGC_B_SIG	_T("X-ACQ_TRKRXAGC.AGC_B_AVG")
#define USC34_XAGC_TRKRXAGC_A_SIG	_T("X-TRKRXAGC.AGC_A_AVG")
#define USC34_XAGC_TRKRXAGC_B_SIG	_T("X-TRKRXAGC.AGC_B_AVG")
#define USC34_AGC_XX_TCR_SIG		_T("DSNTCR_AGAV.AGC_A_AVG")

// 内之浦20m局
// 衛星捕捉に含まれている


// 積分通知メッセージ
//#define NOTIFY_INTEG_MSG			_T("V-EQU.AGC_INTEG_TKSC")
#define NOTIFY_INTEG_MSG			_T("V-EQU.AGC_INTEG_")

#define LGR_SMP_NUM       4         /* 予報値補間処理サンプリング数             */

//=============================================================================
//	予報値一覧
//=============================================================================
// 予報値一覧タイプ
enum eForecastListType {
	eForecastListType_Forecast = 0,
	eForecastListType_Operation
};

//=============================================================================
//	予報値, 衛星捕捉グラフ
//=============================================================================
#define     V_C       (double)(299792458)       /* 光速度 (m/sec)                       */
#define     BAND_SS             (int)1          /* Ｓ／Ｓ　        を対象とする         */
#define     BAND_SX             (int)2          /* Ｓ／Ｘ　        を対象とする         */
#define     BAND_SSSX           (int)3          /* Ｓ／Ｓ＋Ｓ／Ｘ　を対象とする         */
#define     BAND_XX             (int)4          /* Ｘ／Ｘ          を対象とする         */
#define     BAND_RX             (int)5          /* Ｘ／Ｘ(再生型)  を対象とする         *//* 2008-02-27 再生測距対応　追加 */
#define     BAND_DX_T           (int)6          /* Ｘ／Ｘ          を対象とする         *//* <<2013-TURBO>> DSN-MODEM対応 */
#define     BAND_DX_R           (int)7          /* Ｘ／Ｘ(再生型)  を対象とする         *//* <<2013-TURBO>> DSN-MODEM対応 */

#define     RTT_BASE_UP         (int)1          /* アップリンクレンジをベースにRTT計算  */
#define     RTT_BASE_DOWN       (int)2          /* ダウンリンクレンジをベースにRTT計算  */

#define     INTERVAL_NEXT_PASS  (int)10         /* ANT天頂指向実施を判断する時間（分）  */

#define	SAMPLING_NUM		4     /* 補間する基のデータ点数（必ず偶数を設定すること） */
#define     OPT_DMY             (long)(0x0001)  /* 履歴出力形態:使用せず                */
#define     FNC_DMY             ""              /* 機能識別    :使用せず                */
#define     TYP_DMY             ""              /* 形式識別    :使用せず                */

#define	PRED_TIME_LEN               8           /* 予報値時刻タグバイト長               */
#define	PRED_FREQ_LEN               18          /* 予測周波数バイト長                   */
#define	PRED_RAMP_LEN               14          /* ランプレートバイト長                 */
#define	PRED_DUMY_LEN               6           /* ダミーデータバイト長                 */
#define	MAX_PRED_SIZE             60*60*24          /* 予報値テーブルサイズ（秒）           */
#define	END_OF_TABLE        "***** END ****"    /*                                      */

// 表示状態
#define  TYPE_NEW		0							// プロセス起動時の初期表示
#define  TYPE_TRND		1							// 初期表示後のトレンド表示

#define YOHOUTI_ARI		 1			/* 予報値ファイル有り			  */
#define YOHOUTI_NASI	 0			/* 予報値ファイルなし			  */
#define YOHOGET_MATI     1         /* 予報値ファイル取得待ち(V2-5)   */
#define YOHOGET_SUMI     2         /* 予報値ファイル取得済み(V2-5)   */
#define YOHOGET_OK       3         /* 予報値ファイル取得可能(V2-5)   */
#define GET_REAL         1         /* リアル表示中での            予報値取得要求(V2-5) */
#define GET_SET          2         /* 予報値選択「設定」時の      予報値取得要求(V2-5) */
#define GET_DEF          3         /* 予報値選択「デフォルト」時の予報値取得要求(V2-5) */
#define COPY_ALL         0         /* 描画種別(全体)          (V2-3) */
#define COPY_RIGHT       1         /* 描画種別(最右グリッド)  (V2-3) */
#define YOHOU_RNG        0         /* ＲＮＧグラフの予報値描画(V2-3) */
#define YOHOU_RR         1         /* ＲＲ  グラフの予報値描画(V2-3) */

//=============================================================================
//	衛星捕捉制御
//=============================================================================
#define CTRL_P_CAP_ANT   "CTRL.CAP_ANT"     /* ANT待受指向制御                            */
#define CTRL_P_CAP_INIT  "CTRL.CAP_INIT"    /* 初期捕捉制御                               */
#define CTRL_P_CAP_NORMAL "CTRL.CAP_NORMAL" /* 定常捕捉制御                               */
#define CTRL_P_CAP_RX    "CTRL.CAP_RX_STR"      /* RXダウンリンク捕捉制御                     */
#define CTRL_P_CAP_UPLINK "CTRL.CAP_UPLINK_STR" /* アップリンク制御                           */
#define CTRL_P_CAP_WAY   "CTRL.CAP_1WAY/2WAY_SET"	/* 1Way/2Way設定値                            */
													/* "2WAY","1WAY"                              */
#define CTRL_P_PLAN_FILE "CTRL.PLAN_FILE"   /* 展開済み計画ファイル名                     */

#define CTRL_P_CAP_CMD	"CTRL.CAP_CMD_STR"	/* CMD制御 */
#define CTRL_P_CAP_RNG	"CTRL.CAP_RNG_STR"	/* RNG制御 */

enum eSatellieMode {
	eSatellieMode_AutoManual = 0,	/* 自動捕捉/手動捕捉種別 */
	eSatellieMode_AntStandBy,		/* ANT待ち受け指向種別 */
	eSatellieMode_CMD,				/* CMD制御設定 */
	eSatellieMode_RNG,				/* RNG制御設定 */
	eSatellieMode_DownLink,			/* ダウンリンク制御設定 */
	eSatellieMode_UpLink,			/* アップリンク制御設定 */
	eSatellieMode_Way,				/* 1Way/2Way/Dup設定 */
};


//=============================================================================
//	フォルダ定義
//=============================================================================
// ファイルパス
//#define  YOLIST_FILE	_T("CONFIG\\opr\\etc\\pred_list.txt")		// 予報値一覧ファイルデフォルトパス
//#define  YOHOUTI_PATH	_T("CONFIG\\opr\\etc\\grph_yofile\\")		// 予報値ファイル格納パス
#define  YOHOUTI_NAME	_T("opgt_yofile")				// 予報値ファイル名
//#define  SKYLINE_FILE	_T("CONFIG\\opr\\etc\\ct_skyline.dat")	// スカイラインＦＩＬＥ名
//#define  RFTRIP_FILE	_T("CONFIG\\opr\\etc\\ct_rftrip.dat")		// ＲＦトリップＦＩＬＥ名

//#define SPECTRUM_FILE	_T("DATA\\opr\\etc\\opgsproc.dat")	// スペクトラムＦＩＬＥ名
#define SPECTRUM_FILE	_T("opgsproc")	// スペクトラムＦＩＬＥ名
#define SPECTRUM_FILE_TYPE	_T("dat")	// スペクトラムＦＩＬＥ拡張子

#define TRENDPROP_FILE	_T("trendprop")	// トレンド設定ＦＩＬＥ名
#define TRENDPROP_FILE_TYPE	_T("dat")	// トレンド設定ＦＩＬＥ拡張子

#define DATA_DIR			_T("CONFIG")							// データパス
#define PRED_LIST_FILE		_T("CONFIG\\prd\\pred_list.txt")		// 予報値一覧ファイルパス
#define PRED_FILE_DIR		_T("CONFIG\\prd\\")						// 予報値ファイルディレクトリ
#define PLAN_LIST_FILE		_T("CONFIG\\plan\\exec\\plan_list.txt")	// 計画値一覧ファイルパス
#define PLAN_FORM_FILE_DIR	_T("CONFIG\\plan\\form\\")				// 計画値雛形ファイルディレクトリ
#define PLAN_FILE_DIR		_T("CONFIG\\plan\\exec\\")				// 計画値ファイルディレクトリ
#define CID_FILE_DIR		_T("CONFIG\\cid\\")						// CIDファイルディレクトリ
#if 1
#define CID_FILE			_T("cid.csv")							// CIDファイル名
#define CID_EXT				_T(".cid")								// CIDファイル名
#else
#define CID_FILE			_T("cid.txt")							// CIDファイル名
#endif

#define PRED_FILE_TYPE		_T("prd")								// 予報値ファイル拡張子
#define ANP_FILE_TYPE		_T("anp")								// アンテナ予報値ファイル拡張子
#define PLAN_FILE_TYPE		_T("pln")								// 計画ファイル拡張子

#define TILL_TIME			"CTRL.PASS_CNTDOWN(DEC)"    /* 運用パス開始までの時間 			*/
#define PRED_FILE_NAME		"CTRL.PRED_FILE"            /* 予報値ファイルパス名   			*/
#define PRED_FILE_TIME		"CTRL.MAKE_PRED_TIME"       /* 予報値ファイル作成時刻 			*/
#define EISEI_NAME			"CTRL.SAT_NAME"             /* 衛星名                 			*/
#define VEQU_EISEI_NAME		"V-EQU.BBE_SATNAME"         /* VEQU 衛星名             			*/
#define BAND_TYPE			"CTRL.BAND"                 /* 使用周波数帯フラグ     			*/
#define PASS_START_TIME		"CTRL.START_TIME"           /* パス開始時刻           			*/
#define YOHOU_GET			"CTRL.PRED_COND"            /* 予報値取得状態          			*/
#define PRED_VISIBLE		"CTRL.VISIBLE"              /* 予報値可視時間          			*/

#define PRED_GET_MODE		"CTRL.PRED_MODE"			/* 予報値取得モード */
#define AUTO_STR			"AUTO"						/* 自動モード文字列 */
#define MANUAL_STR			"MANUAL"					/* 手動モード文字列 */
#define STOP_STR			"STOP"						/* 停止モード文字列 */

// パス状況
#define CTRL_PASS_INFO_NO			"CTRL.PASS_INFO"			// パス進行状態
#define	CT_PASS_INFO_INIT			"初期設定"					// 初期設定
#define	CT_PASS_INFO_1WAY			"衛星捕捉(1WAY)"			// １ＷＡＹ捕捉中
#define	CT_PASS_INFO_2WAY			"衛星捕捉(2WAY)"			// ２ＷＡＹ捕捉中
#define	CT_PASS_INFO_PASS_START1	"運用中(1WAY)"				// パス開始(1WAY)
#define	CT_PASS_INFO_PASS_START2	"運用中(2WAY)"				// パス開始(2WAY)
#define	CT_PASS_INFO_POL			"天頂通過監視領域内"		// 天頂通過監視領域内
#define	CT_PASS_INFO_POSN			"ANT POSN MODE 移行角検出"	// ANT POSN MODE 移行角検出
#define	CT_PASS_INFO_EXTEND			"運用中(自動延長)"			// パス延長


//=============================================================================
//	通知データ識別値定義
//=============================================================================
#define		INFO_ID_DAT_INSEQU	(unsigned char)(0x01)	/* 設備監視データ						*/
#define		INFO_ID_DAT_CTLEQU	(unsigned char)(0x02)	/* 設備制御データ						*/
#define		INFO_ID_ANS_CTLEQU	(unsigned char)(0x03)	/* 設備制御アンサデータ 各設備とのIF用	*/
#define		INFO_ID_DAT_HISOPE	(unsigned char)(0x04)	/* 運用履歴データ						*/
#define		INFO_ID_REQ_CHKREV	(unsigned char)(0x06)	/* リビジョンチェック要求				*/
#define		INFO_ID_REQ_CHNGDB	(unsigned char)(0x07)	/* データベース更新要求					*/
#define		INFO_ID_CMD_SNDPRE	(unsigned char)(0x08)	/* 予報値送出指示						*/
#define		INFO_ID_ANS_SNDPRE	(unsigned char)(0x09)	/* 予報値送出アンサ						*/
#define		INFO_ID_NOT_CPLPRE	(unsigned char)(0x0a)	/* 予報値送出完了通知					*/
#define		INFO_ID_NOT_CHGPST	(unsigned char)(0x0b)	/* 計画開始時刻変更通知					*/
#define		INFO_ID_NOT_CHGPED	(unsigned char)(0x0c)	/* 計画終了時刻変更通知					*/
#define		INFO_ID_ANS_CHGPST	(unsigned char)(0x0d)	/* 計画開始時刻変更アンサ				*/
#define		INFO_ID_ANS_CHGPED	(unsigned char)(0x0e)	/* 計画終了時刻変更アンサ				*/
#define		INFO_ID_NOT_PLNINF	(unsigned char)(0x0f)	/* 計画運用情報通知						*/

#define		INFO_ID_REQ_REGSDB	(unsigned char)(0x10)	/* データベース登録要求					*/
#define		INFO_ID_NOT_CPLRDB	(unsigned char)(0x11)	/* データベース登録完了通知				*/
#define		INFO_ID_NOT_INIEQU	(unsigned char)(0x13)	/* 設備初期設定通知						*/
#define		INFO_ID_NOT_CHGPAS	(unsigned char)(0x14)	/* パス変更通知							*/
#define		INFO_ID_NOT_STPPAS	(unsigned char)(0x15)	/* パス強制終了通知						*/
#define		INFO_ID_NOT_CHGBIT	(unsigned char)(0x16)	/* ビットレート変更通知					*/
/* 98.05.18 削除
#define		INFO_ID_ANS_CTLEQ2	(unsigned char)(0x17) */ /* 設備制御アンサデータ（ＲＴで生成）	*/
#define		INFO_ID_DAT_RNGDAT	(unsigned char)(0x31)	/* ＲＮＧデータ							*/
#define		INFO_ID_DAT_RRDATA	(unsigned char)(0x32)	/* Ｒ  Ｒデータ							*/

#define		INFO_ID_INF_INIEQU	(unsigned char)(0x40)	/* 設備初期設定通知データ（新）			*/
#define		INFO_ID_RTN_INIEQU	(unsigned char)(0x41)	/* 設備初期設定応答データ（新）			*/
#define		INFO_ID_INF_CHGPAS	(unsigned char)(0x42)	/* パス時刻変更通知データ（新）			*/
#define		INFO_ID_RTN_CHGPAS	(unsigned char)(0x43)	/* パス時刻変更応答データ（新）			*/
#define		INFO_ID_INF_STPPAS	(unsigned char)(0x44)	/* パス強制終了通知データ（新）			*/
#define		INFO_ID_RTN_STPPAS	(unsigned char)(0x45)	/* パス強制終了応答データ（新）			*/
#define		INFO_ID_INF_STTCAL	(unsigned char)(0x46)	/* 校正診断開始通知データ				*/
#define		INFO_ID_RTN_STTCAL	(unsigned char)(0x47)	/* 校正診断開始応答データ				*/
#define		INFO_ID_INF_STPCAL	(unsigned char)(0x48)	/* 校正診断終了通知データ				*/
#define		INFO_ID_RTN_STPCAL	(unsigned char)(0x49)	/* 校正診断終了応答データ				*/
#define		INFO_ID_INF_LOCPTN	(unsigned char)(0x4a)	/* コリメーションLOCK ONパターン測定結果通知データ*/

#define		INFO_ID_ANS_CTRLEQ	(unsigned char)(0x50)	/* 制御アンサデータ設備制御装置内部識別 */

#define		INFO_ID_REQ_MNUGET	(unsigned char)(0x60)	/* 手動取得モード要求					*/
#define		INFO_ID_ANS_MNUGET	(unsigned char)(0x61)	/* 手動取得モード応答					*/
#define		INFO_ID_REQ_AUTGET	(unsigned char)(0x62)	/* 自動取得モード要求					*/
#define		INFO_ID_ANS_AUTGET	(unsigned char)(0x63)	/* 自動取得モード応答					*/
#define		INFO_ID_REQ_STPPRC	(unsigned char)(0x64)	/* 処理中断要求							*/
#define		INFO_ID_ANS_STPPRC	(unsigned char)(0x65)	/* 処理中断応答							*/
#define		INFO_ID_REQ_RSMPRC	(unsigned char)(0x66)	/* 処理再開要求							*/
#define		INFO_ID_ANS_RSMPRC	(unsigned char)(0x67)	/* 処理再開応答							*/
#define		INFO_ID_REQ_GETPRE	(unsigned char)(0x68)	/* 予報値取得要求						*/
#define		INFO_ID_NOT_GETPRE	(unsigned char)(0x69)	/* 予報値取得結果通知					*/
#define		INFO_ID_REQ_DELPRE	(unsigned char)(0x6a)	/* 予報値削除要求						*/
#define		INFO_ID_NOT_DELPRE	(unsigned char)(0x6b)	/* 予報値削除結果通知					*/
#define		INFO_ID_REQ_CHGHST	(unsigned char)(0x6c)	/* 予報値取得対象装置指定要求			*/
#define		INFO_ID_ANS_CHGHST	(unsigned char)(0x6d)	/* 予報値取得対象装置指定応答			*/
#define		INFO_ID_REQ_TIMOFS	(unsigned char)(0x6e)	/* 時刻オフセット設定要求				*/
#define		INFO_ID_NOT_TIMOFS	(unsigned char)(0x6f)	/* 時刻オフセット設定結果通知			*/

#define		INFO_ID_REQ_PRIVIL	(unsigned char)(0x80)	/* 計画立案制御権要求					*/
#define		INFO_ID_ANS_PRIVIL	(unsigned char)(0x81)	/* 計画立案制御権応答					*/
#define		INFO_ID_REQ_PRI_RE	(unsigned char)(0x82)	/* 計画立案制御権開放要求				*/
#define		INFO_ID_ANS_PRI_RE	(unsigned char)(0x83)	/* 計画立案制御権開放応答				*/
#define		INFO_ID_REQ_PLN_UD	(unsigned char)(0x84)	/* 計画更新要求							*/
#define		INFO_ID_ANS_PLN_NE	(unsigned char)(0x85)	/* 計画更新結果応答（新規登録）			*/
#define		INFO_ID_ANS_PLN_DE	(unsigned char)(0x86)	/* 計画更新結果応答（削除）				*/
#define		INFO_ID_ANS_PLN_UD	(unsigned char)(0x87)	/* 計画更新結果応答（更新）				*/
#define		INFO_ID_REQ_CHGOFS	(unsigned char)(0x88)	/* 時刻オフセット変更要求				*/
#define		INFO_ID_ANS_CHGOFS	(unsigned char)(0x89)	/* 時刻オフセット変更完了通知			*/
#define		INFO_ID_REQ_PLN_MO	(unsigned char)(0x8a)	/* 計画変更要求							*/
#define		INFO_ID_ANS_CHGSTM	(unsigned char)(0x8b)	/* 計画変更結果応答	（計画開始時刻変更）*/
#define		INFO_ID_ANS_CHGETM	(unsigned char)(0x8c)	/* 計画変更結果応答	（計画終了時刻変更）*/
#define		INFO_ID_ANS_TERMIN	(unsigned char)(0x8d)	/* 計画変更結果応答	（強制終了）		*/
#define		INFO_ID_NOT_PREDUD	(unsigned char)(0x8e)	/* 予報値更新通知						*/

#define		INFO_ID_DAT_SSRARR	(unsigned char)(0x90)	/* Ｓ／Ｓ帯  追跡データ					*/
#define		INFO_ID_DAT_SXRARR	(unsigned char)(0x91)	/* Ｓ／Ｘ帯  追跡データ					*/
#define		INFO_ID_DAT_XXRARR	(unsigned char)(0x92)	/* Ｘ／Ｘ帯  追跡データ					*/
#define		INFO_ID_DAT_FREQI 	(unsigned char)(0x93)	/* 送信周波数情報 (仮設定　荒谷) XUP	*/

#define		INFO_ID_REQ_ACQCAN	(unsigned char)(0xa0)	/* 自動補足中止要求						*/
#define		INFO_ID_ANS_ACQCAN	(unsigned char)(0xa1)	/* 自動補足中止結果応答					*/
#define		INFO_ID_REQ_ACQMOD	(unsigned char)(0xa2)	/* 衛星補足モード設定要求				*/
#define		INFO_ID_ANS_ACQMOD	(unsigned char)(0xa3)	/* 衛星補足モード設定結果応答			*/
#define		INFO_ID_NOT_RNGMON	(unsigned char)(0xb0)	/* レンジ伝搬モニタ通知データ			*/
#define		INFO_ID_REQ_STPAUS	(unsigned char)(0xb8)	/* S帯追跡データ送信停止要求			*/
#define		INFO_ID_REQ_STRSND	(unsigned char)(0xb9)	/* S帯追跡データ送信再開要求			*/

#define		INFO_ID_REQ_INTEG	(unsigned char)(0xc1)	/* 積分時間指定要求						*/


//=============================================================================
//	共有フォルダ
//=============================================================================
static const TCHAR* mStationFolder[]{
	_T("udsc64"),
	_T("udsc54"),
	_T("usc34"),
	_T("usc20"),
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

	// 送信情報ファイル
	eFileType_Diary_RFInfo_File,

	// 送信ファイル
	eFileType_Diary_Rf_File,

	// 業務日誌フォーマットフォルダ
	eFileType_Diary_Form_Folder,

	// 業務日誌CSV出力フォルダ
	eFileType_Diary_CSVOUT_Folder,

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
	eFileType_SeqCtrlDef_Folder,

	// 逐次制御テンポラリフォルダ
	eFileType_SeqCtrlTmp_Folder,

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

	// 局内ディレイ値テーブルフォルダ
	eFileType_Delay_Folder,

	// 校正診断結果フォルダ
	eFileType_Calib_Result_Folder,

	// 運用プロパティフォルダ
	eFileType_OperationProp_Folder,

	// 計画時間ファイル
	eFileType_PlnTime_File,

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

	// 送信情報ファイル
	_T("ctrl\\log\\pass\\rf_info_report.txt"),

	// 送信ファイル
	_T("ctrl\\log\\pass\\rf_report.txt"),

	// 業務日誌フォーマットフォルダ
	_T("ctrl\\log\\pass\\form\\"),

	// 業務日誌CSV出力フォルダ
	_T("ctrl\\log\\pass\\csv\\"),

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

	// 逐次制御テンポラリフォルダ
	_T("cfg\\etc\\cont\\tmp\\"),

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

	// 局内ディレイ値テーブルフォルダ
	_T("ctrl\\delay\\"),

	// 校正診断結果フォルダ
	_T("ctrl\\log\\calib\\"),

	// 運用プロパティフォルダ
	_T("ctrl\\cfg\\etc\\"),

	// 計画時間ファイル
	_T("cfg\\etc\\PLN_TIME.txt"),

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
	_T("ctrl\\log\\mon\\"),

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

	// 送信情報ファイル
	_T("ctrl\\log\\pass\\rf_info_report.txt"),

	// 送信ファイル
	_T("ctrl\\log\\pass\\rf_report.txt"),

	// 業務日誌フォーマットフォルダ
	_T("ctrl\\log\\pass\\form\\"),

	// 業務日誌CSV出力フォルダ
	_T("ctrl\\log\\pass\\csv"),

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

	// 逐次制御テンポラリフォルダ
	_T("cfg\\etc\\cont\\tmp\\"),

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

	// 局内ディレイ値テーブルフォルダ
	_T("ctrl\\delay\\"),

	// 校正診断結果フォルダ
	_T("ctrl\\log\\calib\\"),

	// 運用プロパティフォルダ
	_T("ctrl\\cfg\\etc\\"),

	// 計画時間ファイル
	_T("cfg\\etc\\PLN_TIME.txt"),

	// ファイル・フォルダ定義数
	_T(""),
};

// サーバ共有フォルダ
static const TCHAR* mSvShareFolder = _T("CONFIG\\share\\");


//=============================================================================
//	RFトリップ角定義
//=============================================================================
// 送信電力識別項目
#define XTX_HPA_SEL_SIG	_T("X-TX.HPA_SEL")
#define HPA1_RES		_T("HPA-1")
#define HPA2_RES		_T("HPA-2")

#define	XTX_HPA_RF_PWR_SIG	_T("X-TX.HPA_RF_PWR")
#define	XTX_HPA2_RF_PWR_SIG	_T("X-TX.HPA2_RF_PWR")

#define STX_RF_POWER_SEL	_T("S-TX.RF_POWER_SEL")
#define XTX_HPA_RF_PWR_CONT	_T("X-TX.HPA_RF_PWR_CONT")

#define STXRF_RF_POWER_SEL	_T("S-TXRF.RF_POWER_SEL")

#define PWR_25W			_T("25w")
#define PWR_100W		_T("100w")
#define PWR_200W		_T("200w")
#define PWR_1KW			_T("1kw")
#define PWR_2KW			_T("2kw")
#define PWR_20KW		_T("20kw")

//=============================================================================
//	RFトリップ角定義ファイル
//=============================================================================
#define RFTRIP_FILENAME					_T("ct_rftrip_%s.dat")		// RFトリップ角定義ファイル名
#define RFTRIP_SECTION_INFO				_T("INFO")					// INFOセクション
#define RFTRIP_KEY_KANSHI_POWER			_T("STSNAME_POWER")			// 送信電力取得用監視名
#define RFTRIP_KEY_KANSHI_MOD			_T("STSNAME_MOD")			// 変調形式取得用監視名
#define RFTRIP_KEY_MOD_SECTION_DEF		_T("STSDATA_MOD")			// 通常とは異なる変調形式のRFトリップを指定する場合の 監視ステータス定義名
#define RFTRIP_KEY_POWERINFO			_T("POWERINFO")				// 送信電力(STSNAME_POWER)で取得したステータス
#define RFTRIP_SECTION_RFTRIP_GENE		_T("TRIP_*")				// 変調形式 「通常」セクション
#define RFTRIP_SECTION_RFTRIP_MOD		_T("TRIP_")					// 変調形式 「指定」セクション

//=============================================================================
//	FA-Panel
//=============================================================================
static const TCHAR* mFapanelBlockFile[] =
{
	_T("RemoteCtrlTrm_UDSC64m.xml"),	// 臼田64
	_T("RemoteCtrlTrm_UDSC54m.xml"),	// 臼田54
	_T("RemoteCtrlTrm_USC34m.xml"),		// 内之浦34
	_T("RemoteCtrlTrm_USC20m.xml"),		// 内之浦20
};

#define FA_AUTH_FILE			_T("auth.txt")					// FA用制御権利ファイル

//=============================================================================
//	リモートシェル定義
//=============================================================================
//#define SERVER_LOGINNAME		_T("sos")						// サーバーログイン名（暫定）
//#define SSHBATCHFILENAME		_T("remsh.bat")					// SSH起動バッチファイル
#define SVSTART_SH				_T("start.sh")					// サーバ環境シェル
#define GET_RADIOLOG_SH			_T("GET_RADIOLOG.sh")			// 無線業務日誌取得
#define REGIST_RADIOLOG_SH		_T("REGIST_RADIOLOG.sh")		// 無線業務日誌登録要求
#define REGIST_OPEPROPERTY_SH	_T("REGIST_OPEPROPERTY.sh")		// 運用プロパティ登録要求
#define REGIST_PLAN_SH			_T("REGIST_PLAN.ksh")			// 計画登録要求(2017.03.24 sh->ksh)
#define SEND_NOTICE_SH			_T("SEND_NOTICE.sh")			// 通知
#define SEND_CONTROL_SH			_T("SEND_CONTROL_UI.sh")		// 設備制御(UI)
#define SEND_CONTROL_NOUI_SH	_T("SEND_CONTROL.sh")			// 設備制御
#define AGCTBL_LOAD_SH			_T("agctbl_load.ksh")			// AGCテーブルロード
#define RSYNC_SH				_T("RSYNC.sh")					// ファイル転送
#define EMERGENCY_ANT_SH		_T("EMERGENCY_STOP_ANT.sh")		// 緊急停止（アンテナ）
#define EMERGENCY_TX_SH			_T("EMERGENCY_STOP_TX.sh")		// 緊急停止（送信機）
#define MONDUMP_SH				_T("MON_DUMP_PROC.sh")			// 監視データダンプ
#define AGCCALIB_GET_SH			_T("GET_CALIB.sh")				// 校正診断用AGC校正ファイル取得
#define AGCCALIB_REQ_SH			_T("REQUEST_CALIB.sh")			// 校正診断実行要求
#define CON_SDTP_SH				_T("CON_SDTP.sh")				// SDTPセッション接続
#define DIS_SDTP_SH				_T("DIS_SDTP.sh")				// SDTPセッション切断
#define STAT_SDTP_SH			_T("STAT_SDTP.sh")				// SDTPセッション接続状況確認
#define SEND_CRM_SH				_T("SEND_CRM.sh")				// 制御権利管理ファイル転送
#define FTP_GET_SH				_T("FTPGET.sh")					// FTP GET
#define FTP_PUT_SH				_T("FTPPUT.sh")					// FTP PUT
#define SEQ_CTRL_SH				_T("opccCtrl.sh")				// 逐次制御
#define OPTION_AGCTBL_LOAD_SH	_T("agctbl_load_all.ksh")		// AGCテーブルロード(オプションメニュー)
#define AGCTBL_LNASEL_SH		_T("AGCTBL_LNASEL")				// LNA1, LNA2切替

enum eRemShellType {
	eRemShellType_GET_RADIOLOG_SH = 0,
	eRemShellType_REGIST_RADIOLOG_SH,
	eRemShellType_REGIST_OPEPROPERTY_SH,
	eRemShellType_REGIST_PLAN_SH,
	eRemShellType_SEND_NOTICE_SH,
	eRemShellType_SEND_CONTROL_SH,
	eRemShellType_RSYNC_SH,
	eRemShellType_EMERGENCY_ANT_SH,
	eRemShellType_EMERGENCY_TX_SH,
	eRemShellType_MONDUMP_SH,
	eRemShellType_AGCCALIB_GET_SH,
	eRemShellType_AGCCALIB_REQ_SH,
	eRemShellType_CON_SDTP_SH,
	eRemShellType_DIS_SDTP_SH,
	eRemShellType_STAT_SDTP_SH,
	eRemShellType_SEND_CRM_SH,
	eRemShellType_FTP_GET_SH,
	eRemShellType_FTP_PUT_SH,
};

#define SSH_WAITTIME			45000							// SSH待ち時間(ms)
#define CYOHO_WAITTIME			20000							// 帳票待ち時間(ms)

#define STAT_SDTP_FILENAME		_T("remsh_res.txt")				// セッション状態の結果格納ファイル
#define STAT_SDTP_LOCKFILENAME	_T("remsh_res.loc")				// セッション状態の結果ロックファイル

// セッションタイプ文字列
static const TCHAR* msStrSessionType[] =
{
	_T("mon"),	// 監視
	_T("ctl"),	// 制御
	_T("nti"),	// 通知
	_T("his"),	// 履歴
};

//=============================================================================
//	逐次制御定義
//=============================================================================
#define SEQCTRL_LINEPOS_FILE	_T("serialcont.EXEC")
#define SEQCTRL_PAUSE_FILE		_T("serialcont.PAUSE")
#define SEQCTRL_END_LINE		-9999

//=============================================================================
// TKSC,SSOCホスト名
enum eHostName{
	eHost_sttn = 3,	// 地上局(64m, 54m, 34m, 20m)
	eHost_ssoc,		// 相模原局
	eHost_tksc,		// 筑波局
};
// ※ホスト名は実際のホスト名の先頭５文字
static const TCHAR* mHostNameString[]{
	_T("u6opr"),	//臼田64m局内
	_T("u5opr"),	//臼田54m局内
	_T("u3opr"),	//内之浦34m局内
	_T("u2opr"),	//内之浦20m局内
	_T("szopr"),
	_T("tzopr"),
};

static const TCHAR* mAreaNameString[]{
	_T("STTN"),		// 地上局
	_T("SSOC"),		// 相模原局
	_T("TKSC"),		// 筑波局
};

//=============================================================================
//	監視名定義
//=============================================================================
#define CTRL_P_SAT	_T("V-EQU.BBE_SATNAME")

#define RNG_DENOMINATOR_NUM	_T("V-EQU.RNG_NUM")		// RNGの分母数

//=============================================================================
//	校正診断 強制終了テーブル
//=============================================================================
typedef struct
{
	CString param;
	CString ctrl;
}stRdnsCancelst;

//----------------
// 64m
// AGC S帯
static const stRdnsCancelst m_RdnsCancel64AgcSTbl[] =
{
	{ _T("S-RX"),				_T("CTRL.RDYCHK_AGC_CANCEL S-RX_A,%s;"), },
	{ _T(""),					_T(""), },
};
// AGC X帯
static const stRdnsCancelst m_RdnsCancel64AgcXTbl[] =
{
	{ _T("X-RX2"),				_T("CTRL.RDYCHK_AGC_CANCEL X-RX2_A,%s;"), },
	{ _T("XX-TCR"),				_T("CTRL.RDYCHK_AGC_CANCEL X2-TCR_A,%s;"), },
	{ _T(""), _T(""), },
};
// RNG
static const stRdnsCancelst m_RdnsCancel64RngTbl[] =
{
	{ _T("S/S"),				_T("CTRL.RDYCHK_RNG_CANCEL SS,%s;"), },
	{ _T("S/X"),				_T("CTRL.RDYCHK_RNG_CANCEL SX,%s;"), },
	{ _T("X/X"),				_T("CTRL.RDYCHK_RNG_CANCEL XX,%s;"), },
	{ _T("X/X(再生型)"),		_T("CTRL.RDYCHK_RNG_CANCEL RX,%s;"), },
	{ _T("X/X_DSN"),			_T("CTRL.RDYCHK_RNG_CANCEL DX,%s;"), },
	{ _T("X/X_DSN(再生型)"),	_T("CTRL.RDYCHK_RNG_CANCEL DR,%s;"), },
	{ _T(""), _T(""), },
};
// TLM S帯
static const stRdnsCancelst m_RdnsCancel64TlmSTbl[] =
{
	{ _T("SS-TCR"),				_T("CTRL.RDYCHK_TLM_CANCEL S,%s;"), },
	{ _T(""), _T(""), },
};
// TLM X帯
static const stRdnsCancelst m_RdnsCancel64TlmXTbl[] =
{
	{ _T("SX-TCR"),				_T("CTRL.RDYCHK_TLM_CANCEL X,%s;"), },
	{ _T("XX-TCR"),				_T("CTRL.RDYCHK_TLM_CANCEL X2,%s;"), },
	{ _T(""), _T(""), },
};
// CMD
static const stRdnsCancelst m_RdnsCancel64CmdTbl[] =
{
	{ _T("CMDOUT2"),			_T("CTRL.RDYCHK_CMD_CANCEL 2,%s;"), },
	{ _T("CMDOUT3"),			_T("CTRL.RDYCHK_CMD_CANCEL 3,%s;"), },
	{ _T("X-CMDOUT2"),			_T("CTRL.RDYCHK_CMD_CANCEL 4,%s;"), },
	{ _T(""), _T(""), },
};

//----------------
// 54m
// AGC X帯
static const stRdnsCancelst m_RdnsCancel54AgcXTbl[] =
{
	{ _T("DSN_TCR"), _T("CTRL.RDYCHK_AGC_CANCEL DSN-MODEM_X_A,%s;"), },
	{ _T(""), _T(""), },
};
// AGC Ka帯
static const stRdnsCancelst m_RdnsCancel54AgcKaTbl[] =
{
	{ _T("DSN_TCR"), _T("CTRL.RDYCHK_AGC_CANCEL DSN-MODEM_KA_A,%s;"), },
	{ _T(""), _T(""), },
};
// RNG
static const stRdnsCancelst m_RdnsCancel54RngTbl[] =
{
	{ _T("X/X(従来)"), _T("CTRL.RDYCHK_RNG_CANCEL DXN,%s;"), },
	{ _T("X/X(再生)"), _T("CTRL.RDYCHK_RNG_CANCEL DXR,%s;"), },
	{ _T("X/Ka(従来)"), _T("CTRL.RDYCHK_RNG_CANCEL DKAN,%s;"), },
	{ _T("X/Ka(再生)"), _T("CTRL.RDYCHK_RNG_CANCEL DKAR,%s;"), },
	{ _T(""), _T(""), },
};
// TLM X帯
static const stRdnsCancelst m_RdnsCancel54TlmXTbl[] =
{
	{ _T("DSN_TCR"), _T("CTRL.RDYCHK_TLM_CANCEL X,%s;"), },
	{ _T(""), _T(""), },
};
// TLM Ka帯
static const stRdnsCancelst m_RdnsCancel54TlmKaTbl[] =
{
	{ _T("DSN_TCR"), _T("CTRL.RDYCHK_TLM_CANCEL KA,%s;"), },
	{ _T(""), _T(""), },
};
// CMD
static const stRdnsCancelst m_RdnsCancel54CmdTbl[] =
{
	{ _T("X-CMDOUT2"), _T("CTRL.RDYCHK_CMD_CANCEL 1,%s;"), },
	{ _T(""), _T(""), },
};

//----------------
// 34m
// AGC S帯
static const stRdnsCancelst m_RdnsCancel34AgcSTbl[] =
{
	{ _T("SS-TCR"),				_T("CTRL.RDYCHK_AGC_CANCEL SS-TCR_AB,%s;"), },
	{ _T("S-RX"),				_T("CTRL.RDYCHK_AGC_CANCEL S-TRKRX_AB,%s;"), },
	{ _T("SX-ACQRX_S"),			_T("CTRL.RDYCHK_AGC_CANCEL S-ACQRX_AB,%s;"), },
	{ _T("SX-ACQRX_X"),			_T("CTRL.RDYCHK_AGC_CANCEL X-ACQRX_AB,%s;"), },
	{ _T(""), _T(""), },
};
// AGC X帯
static const stRdnsCancelst m_RdnsCancel34AgcXTbl[] =
{
	{ _T("X-RX"),				_T("CTRL.RDYCHK_AGC_CANCEL X-TRKRX_AB,%s;"), },
	{ _T("X-ACQRX"),			_T("CTRL.RDYCHK_AGC_CANCEL X-ACQRX_AB,%s;"), },
	{ _T("X-QPSKRX"),			_T("CTRL.RDYCHK_AGC_CANCEL X-QPSKRX_A,%s;"), },
	{ _T("XX-TCR"),				_T("CTRL.RDYCHK_AGC_CANCEL X2-TCR_A,%s;"), },
	{ _T(""), _T(""), },
};
// RNG
static const stRdnsCancelst m_RdnsCancel34RngTbl[] =
{
	{ _T("S/S"),				_T("CTRL.RDYCHK_RNG_CANCEL SS,%s;"), },
	{ _T("S/X"),				_T("CTRL.RDYCHK_RNG_CANCEL SX,%s;"), },
	{ _T("X/X_DSN"),			_T("CTRL.RDYCHK_RNG_CANCEL DX,%s;"), },
	{ _T("X/X_DSN(再生型)"),	_T("CTRL.RDYCHK_RNG_CANCEL DR,%s;"), },
	{ _T(""), _T(""), },
};
// TLM S帯
static const stRdnsCancelst m_RdnsCancel34TlmSTbl[] =
{
	{ _T("SS-TCR"),				_T("CTRL.RDYCHK_TLM_CANCEL S,%s;"), },
	{ _T(""), _T(""), },
};
// TLM X帯
static const stRdnsCancelst m_RdnsCancel34TlmXTbl[] =
{
	{ _T("XX-TCR"),				_T("CTRL.RDYCHK_TLM_CANCEL X2,%s;"), },
	{ _T("X-QPSKRX"),			_T("CTRL.RDYCHK_TLM_CANCEL QPSK,%s;"), },
	{ _T(""), _T(""), },
};
// CMD
static const stRdnsCancelst m_RdnsCancel34CmdTbl[] =
{
	{ _T("S-CMDOUT"),			_T("CTRL.RDYCHK_CMD_CANCEL 1,%s;"), },
	{ _T("X-CMDOUT2"),			_T("CTRL.RDYCHK_CMD_CANCEL 3,%s;"), },
	{ _T(""), _T(""), },
};

//----------------
// 20m
// AGC S帯
static const stRdnsCancelst m_RdnsCancel20AgcSTbl[] =
{
	{ _T("SX-ACQRX_S"),			_T("CTRL.RDYCHK_AGC_CANCEL SX-ACQRX_S_A,%s;"), },
	{ _T("S-RX"),				_T("CTRL.RDYCHK_AGC_CANCEL S-RX_AB,%s;"), },
	{ _T("SS-TCRDET"),			_T("CTRL.RDYCHK_AGC_CANCEL SS-TCRDET_AB,%s;"), },
	{ _T(""), _T(""), },
};
// AGC X帯
static const stRdnsCancelst m_RdnsCancel20AgcXTbl[] =
{
	{ _T("X-RX"),				_T("CTRL.RDYCHK_AGC_CANCEL X-RX_AB,%s;"), },
	{ _T("X-QPSKRX"),			_T("CTRL.RDYCHK_AGC_CANCEL X-QPSKRX_A,%s;"), },
	{ _T(""), _T(""), },
};
// RNG
static const stRdnsCancelst m_RdnsCancel20RngTbl[] =
{
	{ _T("S/S"),				_T("CTRL.RDYCHK_RNG_CANCEL SS,%s;"), },
	{ _T("S/X"),				_T("CTRL.RDYCHK_RNG_CANCEL SX,%s;"), },
	{ _T(""), _T(""), },
};
// TLM S帯
static const stRdnsCancelst m_RdnsCancel20TlmSTbl[] =
{
	{ _T("SS-TCR"),				_T("CTRL.RDYCHK_TLM_CANCEL S,%s;"), },
	{ _T(""), _T(""), },
};
// TLM X帯
static const stRdnsCancelst m_RdnsCancel20TlmXTbl[] =
{
	{ _T("SX-TCR"),				_T("CTRL.RDYCHK_TLM_CANCEL X,%s;"), },
	{ _T("X-QPSKRX"),			_T("CTRL.RDYCHK_TLM_CANCEL QPSK,%s;"), },
	{ _T(""), _T(""), },
};
// CMD
static const stRdnsCancelst m_RdnsCancel20CmdTbl[] =
{
	{ _T("CMDOUT1"),			_T("CTRL.RDYCHK_CMD_CANCEL 1,%s;"), },
	{ _T(""), _T(""), },
};

//=============================================================================
//	取扱説明書名
//=============================================================================
#define DOCUMENT_FILENAME		_T("StationControl.pdf")

//=============================================================================
//	帳票アプリ名
//=============================================================================
#define CYOHYO_APPNAME			_T("Excel帳票作成.exe")

//=============================================================================
//	筑波局運用管制装置ログ
//=============================================================================
enum eTkscLog {
	eTkscLog_Ope = 100,
	eTkscLog_Ctl = 101,
};

//=============================================================================
//	プロパティ
//=============================================================================
#define	AXIS_SCALE_MIN			2					// 目盛りの分割数の最小値
#define	AXIS_SCALE_MAX			10					// 目盛りの分割数の最大値

//=============================================================================
//	衛星選択テーブルタイプ
//=============================================================================
enum eSatSelTblType {
	eSatSelTblType_AgcTblRead,
	eSatSelTblType_AgcTblCopy,
	eSatSelTbltype_AgcTblRecovery,
	eSatSelTbltype_SatDlyTblcopy,
	eSatSelTbltype_SatDlyTblRecovery,
};

static const TCHAR* mSatSelTblTitleString[]{
	_T("AGC校正テーブル読込・衛星選択"),
	_T("AGC校正テーブルファイルコピー・衛星選択"),
	_T("AGC校正テーブルファイル復元・衛星選択"),
	_T("局内ディレイ値テーブルファイルコピー・衛星選択"),
	_T("局内ディレイ値テーブル復元・衛星選択"),
};

static const TCHAR* mSatSelTblMsgString[]{
	_T("AGC校正テーブルファイル読み込み対象の衛星を選択してください。"),
	_T("AGC校正テーブルファイルコピー対象の衛星を選択してください。"),
	_T("AGC校正テーブルファイル復元対象の衛星を選択してください。"),
	_T("局内ディレイ値テーブルファイルコピー対象の衛星を選択してください。"),
	_T("局内ディレイ値テーブル復元対象の衛星を選択してください。"),
};


//=============================================================================
//	設備詳細制御
//=============================================================================
typedef struct
{
	CString strEquipment;
	long	nType;
	CString strRang;
	CString	dRangeCur;
	double	dRangeMin;
	double	dRangeMax;
	long	l_incolflt;	/* 入力桁数: 小数部（＊Ｂ４）（＊Ｂ５）*/
} stEquipment2Ctrl;		// 設備詳細制御２

#ifdef _DEBUG_KANSHI64M
// 64m監視	仮の制御名
#define RA_OFFSET	_T("ANT.AZ_OFFSET")
#define DEC_OFFSET	_T("ANT.EL_OFFSET")
#else
// 54m監視
#define RA_OFFSET	_T("ANT.RA_OFFSET")
#define DEC_OFFSET	_T("ANT.DEC_OFFSET")
#endif

//=============================================================================
//	PRED DIFF
//=============================================================================
typedef struct
{
	double	t;			// 時刻
	double	data;		// データ
} stPredData;

#define DEFAULT_PREDDIFFMIN	_T("10")					// デフォルトの最小PRED DIFF間隔(54m)

//=============================================================================
//	履歴識別番号(54m)
//=============================================================================
//#define DEFAULT_HISTID54	_T("0x010a010a")			// 54m 履歴識別番号デフォルト値
#define DEFAULT_HISTID54	_T("0x010a0110")			// 54m 履歴識別番号デフォルト値


//=============================================================================
//	X帯従来、X帯再生、Ka帯従来、Ka帯再生切替
//=============================================================================
enum eXmeasBandType {
	eXmeasBandType_X,
	eXmeasBandType_Ka,
	eXmeasBandType_Num,
};

enum eXmeasRngMode {
	eXmeasRngMode_Turnaround,
	eXmeasRngMode_Regen,
	eXmeasRngMode_Num,
};

static const TCHAR* mBandTypeString[eXmeasBandType_Num] = {
	_T("X"),
	_T("Ka"),
};

static const TCHAR* mRngModeString[eXmeasRngMode_Num] = {
	_T("TURNAROUND"),
	_T("REGEN"),
};

#define JUDGE_BANDTYPE_STR		_T("CTRL.BAND")
#define JUDGE_RNGMODE_STR		_T("DSNTCR.RNG_MODE")

//=============================================================================
//	校正診断計画　タイムアウト時間(秒)
//=============================================================================
#define RDNSCHKPLAN_FORCESTOP_TIMEOUT	30
