#pragma once


#include <vector>
using namespace std;

#include "TimeLine.h"
#include "afxwin.h"
#include "OperationProperty.h"
#include "..\Utility\ColorControl.h"

enum{
	ANCHORE_LEFTTOP = 0x0000,	// 左上は固定させる
	ANCHORE_RIGHT = 0x0001,	// 右側は固定させる
	ANCHORE_BOTTOM = 0x0002,	// 下側は固定させる
	RESIZE_HOR = 0x0004,	// 横のリサイズを可能にする
	RESIZE_VER = 0x0008,	// 縦のリサイズを可能にする
	RESIZE_BOTH = (RESIZE_HOR | RESIZE_VER),
};

static const stMonitorParam mOperationParam[] = {
	{ IDC_STATIC_SPACEVAL, _T("CTRL.SAT_NAME"), eControlStatus },						// 衛星名
	{ IDC_STATIC_PLANIDVAL, _T("CTRL.PASS_NO"), eControlStatus },						// 計画ID
	{ IDC_STATIC_FORECASTVAL, _T("CTRL.PRED_FILE"), eControlStatus },					// 予報値
	{ IDC_STATIC_OPESTATUSVAL, _T("CTRL.PASS_FLAG"), eControlDouble },					// 運用状態
	{ IDC_STATIC_OPESTARTVAL, _T("CTRL.START_TIME"), eControlTime },					// 運用開始日時
	{ IDC_STATIC_OPESSTOPVAL, _T("CTRL.END_TIME"), eControlTime },						// 運用終了日時
	{ IDC_STATIC_OPESTARTUNTILVAL, _T("CTRL.PASS_FLAG_STR"), eControlStatus },			// 運用開始まで
	{ IDC_STATIC_OPESTOPUNTILVAL, _T("CTRL.PASS_FLAG_STR"), eControlStatus },			// 運用終了まで
	//{ IDC_STATIC_UPLNKSTARTUNTILVAL, _T("X-CMDOUT2.HOLDTONE_STAT"), eControlStatus },	// アップリンク開始まで
	//{ IDC_STATIC_UPLNKSTOPUNTILVAL, _T("DSNTCR.RNG_CONT"), eControlStatus },			// アップリンク終了まで
	//{ IDC_STATIC_RNGSTARTUNTILVAL, _T("DSNTCR.CARRIER_LOCK_ANS"), eControlStatus },		// RNG運用開始まで
	//{ IDC_STATIC_RNGSTOPUNTILVAL, _T("DSNTCR.SUB_CARRIER_LOCK_STS"), eControlStatus },	// RNG運用終了まで
	//{ IDC_STATIC_CMDSTARTUNTILVAL, _T("DSNTCR.VITERBI_LOCK_STS"), eControlStatus },		// CMD運用開始まで
	//{ IDC_STATIC_CMDSTOPUNTILVAL, _T("DSNTCR.BIT_LOCK"), eControlStatus },				// CMD運用終了まで
	//{ IDC_STATIC_OPEOPSENDVAL, _T("X-RX2.PM_LOCK_ACH"), eControlStatus },				// 運用オプション-送信機ドップラ補償
	//{ IDC_STATIC_OPEOPRECVVAL, _T("X-RX2.DEM_LOCK_SUB_CARR_S"), eControlStatus },		// 運用オプション-受信機ドップラ補償
	//{ IDC_STATIC_OPEOPSENDOUTVAL, _T("X-RX2.DEM_LOCK_VITERBI_S"), eControlStatus },		// 運用オプション-送信出力
	{ IDC_STATIC_OPEOPLNAVAL, _T("X-RX2.X_LNA_SEL_ANS"), eControlStatus },				// 運用オプション-LNA
	//{ IDC_STATIC_OPEOPANTWAITVAL, _T("X-PKTDEM.FRM_LOCK_S"), eControlStatus },			// 運用オプション-ANT待ち受け指向通知
	//{ IDC_STATIC_OPEOPANTVAL, _T("X-PKTDEM.FRM_LOCK_S"), eControlStatus },				// 運用オプション-ANT天頂指向
	//{ IDC_STATIC_READAGCVAL, _T("X-PKTDEM.FRM_LOCK_S"), eControlStatus },				// レディネスチェック-AGC校正
	//{ IDC_STATIC_READRNGVAL, _T("X-PKTDEM.FRM_LOCK_S"), eControlStatus },				// レディネスチェック-RNG校正
	//{ IDC_STATIC_READTLMVAL, _T("X-PKTDEM.FRM_LOCK_S"), eControlStatus },				// レディネスチェック-TLM伝送
	//{ IDC_STATIC_READCMDVAL, _T("X-PKTDEM.FRM_LOCK_S"), eControlStatus },				// レディネスチェック-CMD伝送
	//{ IDC_STATIC_STLMRUNVAL, _T("X-PKTDEM.FRM_LOCK_S"), eControlStatus },				// S-TLM-実施有無
	//{ IDC_STATIC_STLMSTARTVAL, _T("X-PKTDEM.FRM_LOCK_S"), eControlStatus },				// S-TLM-開始日時
	//{ IDC_STATIC_STLMSTOPVAL, _T("X-PKTDEM.FRM_LOCK_S"), eControlStatus },				// S-TLM-終了日時
	//{ IDC_STATIC_XTLMRUNVAL, _T("X-PKTDEM.FRM_LOCK_S"), eControlStatus },				// X-TLM-実施有無
	//{ IDC_STATIC_XTLMSTARTVAL, _T("X-PKTDEM.FRM_LOCK_S"), eControlStatus },				// X-TLM-開始日時
	//{ IDC_STATIC_XTLMSTOPVAL, _T("X-PKTDEM.FRM_LOCK_S"), eControlStatus },				// X-TLM-終了日時
	//{ IDC_STATIC_KATLMRUNVAL, _T("X-PKTDEM.FRM_LOCK_S"), eControlStatus },				// Ka-TLM-実施有無
	//{ IDC_STATIC_KATLMSTARTVAL, _T("X-PKTDEM.FRM_LOCK_S"), eControlStatus },			// Ka-TLM-開始日時
	//{ IDC_STATIC_KATLMSTOPVAL, _T("X-PKTDEM.FRM_LOCK_S"), eControlStatus },				// Ka-TLM-終了日時
	//{ IDC_STATIC_MSVAL, _T("X-PKTDEM.FRM_LOCK_S"), eControlStatus },					// 主局/従局識別
	{ IDC_EDIT_PROPEVENTAUTOVAL, _T("CTRL.EVENT_CALC"), eControlStatus },				// 運用プロパティ-イベント-イベント日時の自動計算
	{ IDC_EDIT_PROPEVENTSUNSET, _T("CTRL.ZENITH_IN"), eControlNumeric },				// 運用プロパティ-イベント-天頂通過角度入り
	{ IDC_EDIT_PROPEVENTSUNRISE, _T("CTRL.ZENITH_OUT"), eControlNumeric },				// 運用プロパティ-イベント-天頂通過角度明け
	{ IDC_EDIT_PROPMARGINULON, _T("CTRL.MARGIN_UP_ON"), eControlNumeric },				// 運用プロパティ-マージン-角度マージン U/L ON
	{ IDC_EDIT_PROPMARGINULOFF, _T("CTRL.MARGIN_UP_OFF"), eControlNumeric },			// 運用プロパティ-マージン-角度マージン U/L OFF
	{ IDC_EDIT_PROPMARGINDL, _T("CTRL.MARGIN_DOWN"), eControlNumeric },					// 運用プロパティ-マージン-角度マージン ダウンリンク
	{ IDC_EDIT_PROPULCMD, _T("CTRL.OFS_CMD_END"), eControlNumeric },					// 運用プロパティ-U/L終了前の時刻オフセット-CMD運用終了
	{ IDC_EDIT_PROPULRNG, _T("CTRL.OFS_RNG_END"), eControlNumeric },					// 運用プロパティ-U/L終了前の時刻オフセット-RNG運用終了
	{ IDC_BUTTON_PROPCID, _T("CTRL.DEFAULT_CID"), eControlStatus },						// 運用プロパティ-デフォルトCID
	{ IDC_EDIT_PROPSPACEVAL, _T("CTRL.SAME_TIME_MOD"), eControlStatus },					// 運用プロパティ-CMD/RNG同時変調
};

enum{
	ePlanMasterSlave,
	ePlanPStart,
	ePlanPEnd,
};

namespace pass{
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

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	int eqsat_db_read();		// 衛星固有情報DB取得
	int eqctl_db_read();		// 設備制御DB取得
	int eqmon_db_read();		// 設備制御DB取得
	int rdychk_read();			// レディネスチェック時間取得

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

	map<CString, int>& GetRdyChkTimeList() { return m_rdychktimelist; }

	int GetRdyChkTimeFile(CString str);


protected:

};

}; // namespace pass


// CIniFileMap コマンド ターゲット

class CIniFileMap
{
public:
	CIniFileMap(CString name);
	virtual ~CIniFileMap();

	//-----------------------------------------------------------------------------
	//	定義
	//-----------------------------------------------------------------------------
public:
	static const UINT _OneMegaByte = (1024 * 1024);
	static const UINT _Page = (64 * 1024);
	static const UINT _FlushIncrement = _OneMegaByte;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	void	GetSectionList(vector<CString>& list);

protected:
	bool	Load(CStringA FileName);
	bool	Unload();
	void	CutZeroTail();
	void	UpdateMap();

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:

protected:
	CStringA		mFileName;
	HANDLE			m_hFile;
	HANDLE			m_hMMF;
	DWORD			m_dwSize;
	DWORD			m_dwUseSize;
	LPVOID			m_pData;
	LPVOID			m_pTrueData;
	bool			m_bDirty;

	CMapStringToPtr	m_mapSection;
	//-----------------------------------------------------------------------------
};

// CTimeLineView フォーム ビュー

class CTimeLineView : public CFormView
{
	DECLARE_DYNCREATE(CTimeLineView)

protected:
	CTimeLineView();           // 動的生成で使用される protected コンストラクター
	virtual ~CTimeLineView();

public:
	enum { IDD = IDD_DIALOG_TIMELINE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	COperationTimeLineDoc* GetDocument() const;

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
	UINT		mTimeRate;
	CTime		mGetSharePredFileTime;
	CTime		mGetSharePlanFileTime;
	CTime		mGetSharePlanedFileTime;
//	CTime		mGetPropertyFileTime;

	CString		mStrPlanFilePath;

	SIZE				mSizeTotal, mSizePage, mSizeLine;
	int					mSaveCx, mSaveCy;
	vector<DWORD>		mControl;
	CTimeLine			mGraphHeader;
	CTimeLine			mGraph;
	CTime				mCurrentTime;

	CString				mSelectForecastName; // 選択予報値ファイル名
	CString				mSelectForecastId; // 選択予報値ID
	UINT				mSelectForecast;	// 選択予報値
	UINT				mMasterSlave;		// 選択主従局
	UINT				mSelectOpePlanData;	// 選択運用計画データ

	stOpeProperty		mOpeProp;			// 運用プロパティ
	CString				mOpeProp_Station;	// 対象衛星名
	CString				m_strTlmUser;
	CString				m_strCmdUser;

	bool				m_bModalRun;

	int					mXtx;
	int					mRxfxX;
	int					mRxfxKa;

	vector<CString>		m_RngEleList;

	bool				m_bReadCompletePlanFile;	// 計画ファイル読み込み完了

	pass::CDBAccess	mDBAccess;	// DBアクセスクラス

	// 監視データで取得した値
	CString			mPredFile;
	CString			mPredFileAnp;
	CString			mSataName;
	CString			mPlanId;
	CTime			mStartTime;
	CTime			mEndTime;

	// 初期設定プロパティの入力更新判別
	bool			m_bRecLevelSModified;
	bool			m_bRecLevelXModified;
	bool			m_bSubCarTlmModified;
	bool			m_bSubCarCmdModified;

//	BOOL			mbEndTimeChged;
//	BOOL			mbPlanIDChged;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetTimeLineData(COprPlnData data, UINT nForecastNo);
	void	ClearTimeLineData();
	UINT	GetSelectForecast()			{ return mSelectForecast; }
	void	SetSelectForecast(UINT val) { mSelectForecast = val; }
	BOOL	UpdateTimelineData(UINT selectForecast, bool bUntil=false);
//	void	UpdateInitializePropertyData();
//	void	UpdateOperationProperty();
	void	UpdateOperationOption();
	void	UpdateMonitorData();
	void	UpdatePlanData(int type, stPlandata data);
	BOOL	UpdatePlanDataAutoOff(CString& cbuf);
	CString	GetOpeMode(stPlandata data);
	int		GetSatelliteXTX();
	CString	ExchageRngTypePlanFile(CString str);
	CString	ExchagePlanFileRngType(CString str);
	bool	GetKanshiData();
	bool	GetPredMonitorFile();

	void	GetCurrentUtcTime(CTime& time);

	stOpeProperty&	GetOperationProperty() { return mOpeProp; }

protected:
	void	CreateTimelineData();
	void	initializeControl();
	void	setCurrentUtcTime();
	void	getAntForecastTime(CTime &start, CTime &end);
	void	setControlInfo(WORD id, WORD anchore);
	void	UpdateStatus();
	int		ub_asc_to_asctime(char sz_input_time[], char sz_output_time[]);

	//BOOL	ReadCIDFile();
	//void	AddDataVector(vector<CString> &v, CString str);

	/* ------------------------------------------------------------------------------------ */
	BOOL	SetPredList_Utf8();
	BOOL	SetPredList_Utf8(UINT nStation);
	BOOL	SetPredList_Euc();
	BOOL	SetPredList_Euc(UINT nStation);
	BOOL	SetPlanList_Utf8();
	BOOL	SetPlanList_Utf8(UINT nStation);
	BOOL	SetPlanList_Euc();
	BOOL	SetPlanList_Euc(UINT nStation);
	int		ub_asc_to_asctime(TCHAR sz_input_time[], TCHAR sz_output_time[]);
	CTime	StrTimeToCTime(CString strTime);
	BOOL	GetPlanDataFromFile();
	BOOL	GetPlanFile(CString path, CString file, LPARAM lParam);
	int		ReadPlanFile64(CString sz_satname, CString sz_passno, stPlandata& stpd, LPARAM lParam);
	int		ReadPlanFile54(CString sz_satname, CString sz_passno, stPlandata& stpd, LPARAM lParam);
	int		ReadPlanFile34(CString sz_satname, CString sz_passno, stPlandata& stpd, LPARAM lParam);
	int		ReadPlanFile20(CString sz_satname, CString sz_passno, stPlandata& stpd, LPARAM lParam);
	BOOL	GetPredTime(CString pc_satname, CString pc_pass, CTime& pc_aos_time, CTime& pc_los_time, stPlandata& stpd);
	void	ConvertData(COprPlnData& tdata, stPlandata data);
	void	AddEventList(stTime time, vector<stTime>& list);
	void	AddRngEventList(stTime time, CString type, vector<stTime>& list, vector<CString>& listType);

	void	SetMonitorMode(CWnd* pMain);
	CString	ConvertPredfileToId(CString name);
	CString ConvertPredfileToFname(CString name);

	bool	_ReadOpeProp();
	bool	_WriteOpeProp(bool bInitialize=false);
	int		_GetLeapSecNo(CString& buf);
	CString	_GetLeapSecString(int val);

	/* ------------------------------------------------------------------------------------ */

	unsigned char GetKanshiCmdMode();
	unsigned char GetKanshiRngMode();
	unsigned char GetKanshiDownLinkMode();
	unsigned char GetKanshiUpLinkMode();
	unsigned char GetKanshiWayMode();
	void SendSatelliteCaptureModeNotify(unsigned char binary);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CNotesButton m_btCheck[eColor_MAX];
	CTimeGroupBoxEx m_grpNotes;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CTimeGroupBoxEx m_grpOpeInfo;
	CTimeGroupBoxEx m_grpUpLink;
	CTimeGroupBoxEx m_grpCmd;
	CTimeGroupBoxEx m_grpRng;
	CTimeGroupBoxEx m_grpOption;
	CTimeGroupBoxEx m_grpRead;
	CTimeGroupBoxEx m_grpStlm;
	CTimeGroupBoxEx m_grpXtlm;
	CTimeGroupBoxEx m_grpKatlm;
	CTimeGroupBoxEx m_grpInitProp;
	CTimeGroupBoxEx m_grpMasterSlave;
	CTimeGroupBoxEx m_grpProperty;
	CTimeGroupBoxEx m_grpEvent;
	CTimeGroupBoxEx m_grpMargin;
	CTimeGroupBoxEx m_grpOffset;
	CTimeGroupBoxEx m_grpDefaultCid;
	CTimeGroupBoxEx m_grpSpace;
	CString m_strCurrentTime;
	// 運用計画情報
	CString m_strSpaceName;
	CString m_strPlanID;
	CString m_strForecast;
	CString m_strOpeStatus;
	CString m_strOpeStart;
	CString m_strOpeStop;
	CString m_strOpeStartUntil;
	CString m_strOpeStopUntil;
	CString m_strUplnkStartUntil;
	CString m_strUplnkStopUntil;
	CString m_strRngStartUntil;
	CString m_strRngStopUntil;
	CString m_strCmdStartUntil;
	CString m_strCmdStopUntil;
	afx_msg void OnBnClickedButtonForecast();
	afx_msg void OnBnClickedButtonOpestop();
	// アップリンク
	CString m_strUplnkRun;
	CString m_strUplnkEvent;
	int m_nUplnkEvent;
	int m_nUplnkNo;
	CComboBox m_cbUplnkNo;
	CString m_strUplnkStart;
	CString m_strUplnkStop;
	afx_msg void OnBnClickedButtonUplnk();
	afx_msg void OnBnClickedButtonUplnkadd();
	afx_msg void OnBnClickedButtonUplnkupdate();
	afx_msg void OnBnClickedButtonUplnkdel();
	afx_msg void OnCbnSelchangeComboUplnkno();
	// CMD
	CString m_strCmdRun;
	CString m_strCmdEvent;
	int m_nCmdEvent;
	int m_nCmdNo;
	CComboBox m_cbCmdNo;
	CString m_strCmdStart;
	CString m_strCmdStop;
	afx_msg void OnBnClickedButtonCmd();
	afx_msg void OnBnClickedButtonCmdadd();
	afx_msg void OnBnClickedButtonCmdupdate();
	afx_msg void OnBnClickedButtonCmddel();
	afx_msg void OnCbnSelchangeComboCmdno();
	// RNG
	CString m_strRngRun;
	CString m_strRngEvent;
	int m_nRngEvent;
	int m_nRngNo;
	CComboBox m_cbRngNo;
	CString m_strRngStart;
	CString m_strRngStop;
	afx_msg void OnBnClickedButtonRng();
	afx_msg void OnBnClickedButtonRngadd();
	afx_msg void OnBnClickedButtonRngupdate();
	afx_msg void OnBnClickedButtonRngdel();
	afx_msg void OnCbnSelchangeComboRngno();
	// 運用オプション
	CString m_strOptInit;
	CString m_strOptSend;
	CString m_strOptRecv;
	CString m_strOptSendout;
	CString m_strOptLna;
	CString m_strOptAntwait;
	CString m_strOptAnt;
	afx_msg void OnBnClickedButtonAnt();
	// レディネスチェック
	CString m_strReadAgc;
	CString m_strReadRng;
	CString m_strReadTlm;
	CString m_strReadCmd;
	// S-TLM
	CString m_strStlmRun;
	CString m_strStlmStart;
	CString m_strStlmStop;
	// X-TLM
	CString m_strXtlmRun;
	CString m_strXtlmStart;
	CString m_strXtlmStop;
	// Ka-TLM
	CString m_strKtlmRun;
	CString m_strKtlmStart;
	CString m_strKtlmStop;
	CString m_strMasterSlave;
	afx_msg void OnBnClickedButtonMasterslave();
	afx_msg void OnBnClickedButtonPropset();
	afx_msg void OnBnClickedButtonPropreset();
//	DWORD m_nPropEventSunset;
//	DWORD m_nPropEventSunrise;
//	DWORD m_nPropMarginUlon;
//	DWORD m_nPropMarginUloff;
//	DWORD m_nPropMarginDl;
//	DWORD m_nPropUlCmd;
//	DWORD m_nPropUlRng;
	afx_msg void OnBnClickedButtonPropcid();
//	CMFCMaskedEdit m_edPropEventSunset;
//	CMFCMaskedEdit m_edPropEventSunrise;
//	CMFCMaskedEdit m_edPropMarginUlon;
//	CMFCMaskedEdit m_edPropMarginUloff;
//	CMFCMaskedEdit m_edPropMarginDl;
//	CMFCMaskedEdit m_edPropUlCmd;
//	CMFCMaskedEdit m_edPropUlRng;
	CListBox m_cbWayList;
//	CString m_strPropEventCalc;
//	CString m_strSameTimeMod;
	CFloatEdit m_edRecLevelS;
	CFloatEdit m_edRecLevelX;
	CFloatEdit m_edTlm;
	CFloatEdit m_edCmd;
	BOOL m_bTlm;
	BOOL m_bCmd;
	CString m_strRecLevelS;
	CString m_strRecLevelX;
	CString m_strTlm;
	CString m_strCmd;
	afx_msg void OnBnClickedButtonPropinit();
	afx_msg void OnBnClickedCheckTlm();
	afx_msg void OnBnClickedCheckCmd();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CString m_strRngType;
	CColorStatic m_UplinkEventStatic;
	CColorStatic m_CmdEventStatic;
	CColorStatic m_RngEventStatic;
	afx_msg void OnEnChangeEditReclevelvals();
	afx_msg void OnEnChangeEditReclevelvalx();
	afx_msg void OnEnChangeEditTlm();
	afx_msg void OnEnChangeEditCmd();
};

#ifndef _DEBUG  // OperationTimeLineView.cpp のデバッグ バージョン
inline COperationTimeLineDoc* CTimeLineView::GetDocument() const
{
	return reinterpret_cast<COperationTimeLineDoc*>(m_pDocument);
}
#endif

