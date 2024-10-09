#pragma once
#include "afxcmn.h"
#include "atltime.h"
#include "afxwin.h"
#include "OperationPlanTimeSet.h"
#include "OperationPlanGraph.h"
#include "CidSelect.h"
#include "SatelliteData.h"
#include "afxdtctl.h"
#include "DetailViewDlg.h"
#include "DateTimeCtrlNoToday.h"


#define BASE_OPE_NO		5201
#define MSG_CREATE		2

// COperationPlan ダイアログ

class COperationPlan : public CDialogBase
{
	DECLARE_DYNAMIC(COperationPlan)

public:
	COperationPlan(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~COperationPlan();

// ダイアログ データ
	enum { IDD = IDD_DLG_OPERATIONPLAN };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	typedef struct {
		double AZ;
		double EL;
	} stAzEl;

	typedef struct {
		CTime time;
		double AZ;
		double EL;
	} stAnpData;

protected:


	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
	// ANT待ちうけ指向通知
	BOOL m_bAntStandby;
	// ANT天頂志向有無
	BOOL m_bAntZenith;
	// 運用オプション-初期設定
	BOOL m_bOpInit;

	// アンテナ予報値 AOS
	CTime m_tAOS;
	CTime m_tAOSRf;
	CTime m_tAOSTLM;
	// アンテナ予報値 LOS
	CTime m_tLOS;
	CTime m_tLOSRf;
	CTime m_tLOSTLM;

	// 運用開始日時
	CTime m_tOpeS;
	// 運用終了日時
	CTime m_tOpeE;

	// TLM S 開始日時
	CTime m_tTLMSS;
	// TLM S 終了日時
	CTime m_tTLMSE;

	// TLM X 開始日時
	CTime m_tTLMXS;
	// TLM X 終了日時
	CTime m_tTLMXE;

	// TLM Ka 開始日時
	CTime m_tTLMKaS;
	// TLM Ka 終了日時
	CTime m_tTLMKaE;

	// 運用オプション-送信出力タイプ
	int m_OpTOutType;

	// x_tx (Kaフラグ)
	BOOL m_bXTX;

protected:
	long		m_l_my_ipc;			// IPCキー
	long		m_l_my_mid;			// 受信メッセージID
	CString		m_SatelliteName;
	CString		m_PassID;
	CString		m_PassIDMask;
	stPlandata	m_PlanData;
	map<CString, int> m_RdyTimeAGCList;
	map<CString, int> m_RdyTimeRNGList;
	map<CString, int> m_RdyTimeTLMList;
	map<CString, int> m_RdyTimeCMDList;
	COprPlnData m_OprPlnDataOrg;
	BOOL		m_bNewPlan;
	vector<CString> m_RngEleList;

	int			m_IdxEqsat;
	DWORD		m_GetForm;

	vector<stAzEl>		m_SkyLineDataList;
	vector<double>		m_RFTripDataList;
	vector<stAnpData>	m_AnpDataList;
	stAnpData			m_AnpDataAos;
	stAnpData			m_AnpDataLos;
	stAnpData			m_AnpDataMax;

	CString				m_strRngMesMode;
	BOOL		m_bForceClosed;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	BOOL	IsConflictTimeInList(CListCtrl& list, CTime cSTime, CTime cETime, int idx = -1);
	BOOL	IsProximityTime(CTime cSTime, CTime cETime, int type, int idx);
	void	SetSatelliteName(CString str) { m_SatelliteName = str; }
	void	SetPassId(CString str)	{ m_PassID = str; }
	void	SetGetForm(DWORD form)	{ m_GetForm = form; }
	void	SetType(UINT type)		{
		if (type == 1)
		{
			m_bNewPlan = TRUE;
		}
		else
		{
			m_bNewPlan = FALSE;
		}
	}
	//virtual BOOL UpdateDlg();
	BOOL	UpdateOperationPlanDlg();
	void	ClearForceClosed()		{ m_bForceClosed = FALSE; }

protected:
	int		rev_env_set();
	void	GetAntFrcTime(CTime &cAos, CTime &cLos);		// アンテナ予報値取得
	CString ChgPlanIDFormat(CString str);
	BOOL	SetPlanData();
	BOOL	GetPlanData(CString satellite, CString planid, COprPlnData& pd);
	BOOL	SetPlanDataMap(CString satellite, CString planid, COprPlnData data);
	BOOL	CheckPlanDataDlg();
	BOOL	GetPredTime(CString pc_satname, CString pc_pass, CTime& pc_aos_time, CTime& pc_los_time, stPlandata& stpd);
	BOOL	GetPlanList();
	BOOL	GetSatelliteInfoData(CString satname);
	BOOL	GetPlanFile(CString satname, CString pass, DWORD lParam);
	BOOL	PutPlanFile(CString path, CString file);
	BOOL	AccessPlanData();
//	int		ud_plan_rw(long l_mode, CString sz_satname, CString sz_passno, stPlandata& stpd);
	int		ReadPlanFormFile20(CString sz_satname, CString sz_passno, stPlandata& stpd);
	int		ReadPlanFormFile34(CString sz_satname, CString sz_passno, stPlandata& stpd);
	int		ReadPlanFormFile64(CString sz_satname, CString sz_passno, stPlandata& stpd);
	int		ReadPlanFormFile54(CString sz_satname, CString sz_passno, stPlandata& stpd);
	int		ReadPlanFile20(CString sz_satname, CString sz_passno, stPlandata& stpd, DWORD dw);
	int		ReadPlanFile34(CString sz_satname, CString sz_passno, stPlandata& stpd, DWORD dw);
	int		ReadPlanFile64(CString sz_satname, CString sz_passno, stPlandata& stpd, DWORD dw);
	int		ReadPlanFile54(CString sz_satname, CString sz_passno, stPlandata& stpd, DWORD dw);
	int 	CreatePlanFile(BOOL bRegist = TRUE);
	int		WritePlanFile20(CString sz_satname, CString sz_passno, stPlandata& stpd, BOOL bRegist = TRUE);
	int		WritePlanFile34(CString sz_satname, CString sz_passno, stPlandata& stpd, BOOL bRegist = TRUE);
	int		WritePlanFile64(CString sz_satname, CString sz_passno, stPlandata& stpd, BOOL bRegist = TRUE);
	int		WritePlanFile54(CString sz_satname, CString sz_passno, stPlandata& stpd, BOOL bRegist = TRUE);
	CString	ExchageRngTypePlanFile(CString str);
	CString	ExchageRngTypePlanFile54(CString str);
	CString	GetOpeMode();
	CString	GetOpeMode54();
	void	GetOpeModeAddString(CString &str, CString mode);
	void	CalcRdyTime20();
	void	CalcRdyTime34();
	void	CalcRdyTime64();
	void	CalcRdyTime54();
	BOOL	InitDlg();
	CString GetFullPath2FileName(CString str);
	CString GetFilePath();
	BOOL	GetTimeInList(CListCtrl& list, int idx, CTime& cSTime, CTime& cETime);
	int		GetRdyTime20(CString str);
	int		GetRdyTime34(CString str);
	int		GetRdyTime64(CString str);
	int		GetRdyTime54(CString str);
	int		GetPlnTime54(CString str);
	BOOL	SetPlanDataDlg(int nStation, CString sz_passno, stPlandata& stpd);

	int		data_get_from_db20();
	int		data_get_from_db34();
	int		data_get_from_db64();
	int		data_get_from_db54();

	int		GetIndexEqSatDBData(CString str);

	BOOL	IsInPlanDayTime(CTime cSTime, CTime cETime);
	void	GetAnpData();

	BOOL	GetSkylineData();
	BOOL	GetELforSkylineData(int type, double &el);
	void	GetIntersectionDateSkyline(CTime &sTime, CTime &eTime);

	BOOL	GetRFTripData();
	BOOL	GetELforRFTripData(int type, double &el);
	void	GetIntersectionDateRFTrip(CTime &sTime, CTime &eTime);

	void	InvalidOerationGraph();

	void	InitPlanData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuClose();
	afx_msg void OnBnClickedOpeplnBasicprmUlAddBtn();
	afx_msg void OnBnClickedOpeplnBasicprmUlDelBtn();
	afx_msg void OnBnClickedOpeplnBasicprmRngAddBtn();
	afx_msg void OnBnClickedOpeplnBasicprmRngDelBtn();
	afx_msg void OnBnClickedOpeplnBasicprmCmdAddBtn();
	afx_msg void OnBnClickedOpeplnBasicprmCmdDelBtn();
	afx_msg void OnBnClickedOpeplnDetailBtn();
	afx_msg void OnBnClickedOpeplnPrintBtn();
	afx_msg void OnBnClickedOpeplnEntryBtn();
	afx_msg void OnBnClickedOpeplnBasicprmUlEditBtn();
	afx_msg void OnBnClickedOpeplnBasicprmRngEditBtn();
	afx_msg void OnBnClickedOpeplnBasicprmCmdAddBtn2();
	afx_msg void OnBnClickedOpeplnBasicprmCmdEditBtn();
	afx_msg void OnBnClickedOpeplnRdnsAgcCb();
	afx_msg void OnBnClickedOpeplnRdnsRngCb();
	afx_msg void OnBnClickedOpeplnRdnsTlmCb();
	afx_msg void OnBnClickedOpeplnRdnsCmdCb();
	afx_msg void OnCbnSelchangeOpeplnOpeopInitsetCmb();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOpeplnBrCidBtn();
	afx_msg void OnBnClickedOpeplnBasicprmUlCheck();
	afx_msg void OnBnClickedOpeplnBasicprmRngCheck();
	afx_msg void OnBnClickedOpeplnBasicprmCmdCheck();
	afx_msg void OnBnClickedOpeplnBasicprmTlmsCheck();
	afx_msg void OnBnClickedOpeplnBasicprmTlmxCheck();
	afx_msg void OnBnClickedOpeplnBasicprmTlmkaCheck();
	afx_msg void OnLvnItemchangedOpeplnStltList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkOpeplnStltList(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_SatelliteList;
	virtual BOOL OnInitDialog();
	// 運用開始日時（Day）
	CTime m_OpeStartDay;
	// 運用開始日時（Time）
	CTime m_OpeStartTime;
	// 運用終了日時（Day）
	CTime m_OpeEndDay;
	// 運用終了日時（Time）
	CTime m_OpeEndTime;
	// アップリンク開始終了日時リスト
	CListCtrl m_UpLinkList;
	// RNG開始終了日時リスト
	CListCtrl m_RngList;
	// CMD開始終了日時
	CListCtrl m_CmdList;
	// 運用オプション-初期設定
	CComboBoxEx2 m_OpInitCmb;
	// 運用オプション-受信機ドップラ補償
	CComboBoxEx2 m_OpRDPCmb;
	// レディネスチェック-AGC校正
	BOOL m_AGCChkValue;
	// レディネスチェック-RNG校正
	BOOL m_RNGChkValue;
	// レディネスチェック-TLM転送
	BOOL m_TLMChkValue;
	// レディネスチェック-CMD伝送
	BOOL m_CMDChkValue;
	COperationPlanGraph m_OperationGraph;
	// 基本パラメータ-計画ID
	CString m_strPlanID;
	CMFCMaskedEdit m_PlanIDEdit;
	// 基本パラメータ-アップリンク
	BOOL m_UpLinkChkBox;
	// 基本パラメータ-RNG
	BOOL m_RngChkBox;
	// 基本パラメータ-CMD
	BOOL m_CmdChkBox;
	// 基本パラメータ-TLMS
	BOOL m_TLMSChkBox;
	// 基本パラメータ TLMXチェックボックス
	CButton m_TLMSCtrl;
	// 基本パラメータ-TLMX
	BOOL m_TLMXChkBox;
	// 基本パラメータ-TLMKa
	BOOL m_TLMKaChkBox;
	// 基本パラメータ-TLMKaチェックボックス
	CButton m_TLMKaCtrl;
	// ビットレート-TLM S
	CString m_strBRTLMS;
	// ビットレート-TLM X
	CString m_strBRTLMX;
	// ビットレート-TLM Ka
	CString m_strBRTLMKa;	//画面入出力用
	// ビットレート-高速TLM
	CString m_strBRHTLM;
	// ビットレート-CMD S
	CString m_strBRCMDS;
	// ビットレート-CMD X
	CString m_strBRCMDX;
	// 運用開始日時（Day）コントロール
	CDateTimeCtrlNoToday m_OpeStartDayCtrl;
	// 運用開始日時（Time）コントロール
	CDateTimeCtrl m_OpeStartTimeCtrl;
	// 運用終了日時（Day）コントロール
	CDateTimeCtrlNoToday m_OpeEndDayCtrl;
	// 運用終了日時（Time）コントロール
	CDateTimeCtrl m_OpeEndTimeCtrl;
	// 衛星名
	CString m_strSatellite;
	// 予報値AOS（DAY）
	CTime m_PredAosDay;
	// 予報値AOS（Time）
	CTime m_PredAosTime;
	// 予報値LOS（DAY）
	CTime m_PredLosDay;
	// 予報値LOS（Time）
	CTime m_PredLosTime;
	// 予報値AOSコントロール
	CDateTimeCtrlNoToday m_PredAosDayCtrl;
	// 予報値LOSコントロール
	CDateTimeCtrlNoToday m_PredLosDayCtrl;

	// 詳細表示画面
	CDetailViewDlg m_DetailDlg;			
	// 予報値ファイル名
	CString m_PredFileName;
	// CID選択ボタンコントロール
	CButton m_CidBtnCtrl;
	// ビットレート-CMD X
	CEdit m_CmdXEditCtrl;
	// TLM S 日時リスト
	CListCtrl m_TlmSList;
	// TLM X 日時リスト
	CListCtrl m_TlmXList;
	// TLM Ka 日時リスト
	CListCtrl m_TlmKaList;
	// TLM S 追加ボタン
	CButton m_TlmSAddBtn;
	// TLM S 編集ボタン
	CButton m_TlmSEditBtn;
	// TLM S 削除ボタン
	CButton m_TlmSDelBtn;
	afx_msg void OnBnClickedOpeplnBasicprmTlmsAddBtn();
	afx_msg void OnBnClickedOpeplnBasicprmTlmsEditBtn();
	afx_msg void OnBnClickedOpeplnBasicprmTlmsDelBtn();
	afx_msg void OnBnClickedOpeplnBasicprmTlmxAddBtn();
	afx_msg void OnBnClickedOpeplnBasicprmTlmxEditBtn();
	afx_msg void OnBnClickedOpeplnBasicprmTlmxDelBtn();
	afx_msg void OnBnClickedOpeplnBasicprmTlmkaAddBtn();
	afx_msg void OnBnClickedOpeplnBasicprmTlmkaEditBtn();
	afx_msg void OnBnClickedOpeplnBasicprmTlmkaDelBtn();
	// TLM Ka 追加ボタン
	CButton m_TlmKaAddBtn;
	// TLM Ka 編集ボタン
	CButton m_TlmKaEditBtn;
	// TLM Ka 削除ボタン
	CButton m_TlmKaDelBtn;
	// TLM 高速
	CEdit m_TlmXHEditCtrl;
	// ビットレート-CMD S
	CEdit m_CmdSEditCtrl;
	// 運用オプション-送信出力
	CString m_TrsPowerStr;
	// 運用オプション-送信機ドップラ補償
	CString m_OpTDPStr;
	// ANT待ちうけ指向通知
	CString m_AntStandbyStr;
	// ANT天頂志向有無
	CString m_AntZenithStr;
	CDateTimeCtrl m_PredAosTimeCtrl;
	CDateTimeCtrl m_PredLosTimeCtrl;
	afx_msg void OnClose();
	// TLM X
	CButton m_TLMXCtrl;
	CButton m_TlmXAddBtn;
	CButton m_TlmXEditBtn;
	CButton m_TlmXDelBtn;
	afx_msg void OnDtnDatetimechangeOpeplnBasicprmOpeSdayDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeOpeplnBasicprmOpeStimeDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeOpeplnBasicprmOpeEdayDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeOpeplnBasicprmOpeEtimeDp(NMHDR *pNMHDR, LRESULT *pResult);
	// 基本パラメータ-アップリンクチェックボックス
	CButton m_UplinkChkBtnCtrl;
	// 基本パラメータ-RNGチェックボックス
	CButton m_RngChkBtnCtrl;
	// 基本パラメータ-CMDチェックボックス
	CButton m_CmdChkBtnCtrl;
};
