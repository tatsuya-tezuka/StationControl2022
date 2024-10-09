/*============================================================================*/
/*! OperationPlan.cpp

-# 計画立案画面
*/
/*============================================================================*/
// OperationPlan.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "OperationPlan.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "HookMsgBox.h"
#include "..\Utility\SshShell.h"
#include <locale.h>

#if 1
#define PASS_BASE_TIME20	50	// パス開始からの基本時刻（分）
#else
#define PASS_BASE_TIME20	33	// パス開始からの基本時刻（分）
#endif
#define ANT_TIME20			7

#if 1
#define PASS_BASE_TIME34	50	// パス開始からの基本時刻（分）
#else
#define PASS_BASE_TIME34	30	// パス開始からの基本時刻（分）
#define BASE_TIME_PLUS34	5	// S/X帯衛星の場合の増分（分）
#endif
#define ANT_TIME34			7

#if 1
#define PASS_BASE_TIME64	45	// パス開始からの基本時刻（分）
#else
#define PASS_BASE_TIME64	41	// パス開始からの基本時刻（分）
#endif
#define ANT_TIME64			21
#define ANT_TIME			2

// COperationPlan ダイアログ

IMPLEMENT_DYNAMIC(COperationPlan, CDialogBase)

/*============================================================================*/
/*! COperationPlan

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
COperationPlan::COperationPlan(CWnd* pParent /*=NULL*/)
: CDialogBase(COperationPlan::IDD, pParent)
	, m_OpeStartDay(0)
	, m_OpeStartTime(0)
	, m_OpeEndDay(0)
	, m_OpeEndTime(0)
	, m_AGCChkValue(FALSE)
	, m_RNGChkValue(FALSE)
	, m_TLMChkValue(FALSE)
	, m_CMDChkValue(FALSE)
	, m_strPlanID(_T(""))
	, m_UpLinkChkBox(FALSE)
	, m_RngChkBox(FALSE)
	, m_CmdChkBox(FALSE)
	, m_TLMSChkBox(FALSE)
	, m_TLMXChkBox(FALSE)
	, m_TLMKaChkBox(FALSE)
	, m_strBRTLMS(_T(""))
	, m_strBRTLMX(_T(""))
	, m_strBRTLMKa(_T(""))
	, m_strBRHTLM(_T(""))
	, m_strBRCMDS(_T(""))
	, m_strBRCMDX(_T(""))
	, m_strSatellite(_T(""))
	, m_PredAosDay(0)
	, m_PredAosTime(0)
	, m_PredLosDay(0)
	, m_PredLosTime(0)
	, m_PredFileName(_T(""))
	, m_TrsPowerStr(_T(""))
	, m_OpTDPStr(_T(""))
	, m_AntStandbyStr(_T(""))
	, m_AntZenithStr(_T(""))
{
	m_bAntStandby = FALSE;
	m_bAntZenith = FALSE;
	m_bOpInit = FALSE;
	m_SatelliteName = _T("");
	m_PassID = _T("");
	m_bXTX = FALSE;
	m_bNewPlan = TRUE;
	m_RngEleList.clear();
	m_IdxEqsat = -1;
	m_GetForm = 0;
	m_bForceClosed = FALSE;
}

/*============================================================================*/
/*! COperationPlan

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
COperationPlan::~COperationPlan()
{
	if (m_DetailDlg.GetSafeHwnd() != NULL){
		m_DetailDlg.DestroyWindow();
	}
}

void COperationPlan::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPEPLN_STLT_LIST, m_SatelliteList);
	DDX_DateTimeCtrl(pDX, IDC_OPEPLN_BASICPRM_OPE_SDAY_DP, m_OpeStartDay);
	DDX_DateTimeCtrl(pDX, IDC_OPEPLN_BASICPRM_OPE_STIME_DP, m_OpeStartTime);
	DDX_DateTimeCtrl(pDX, IDC_OPEPLN_BASICPRM_OPE_EDAY_DP, m_OpeEndDay);
	DDX_DateTimeCtrl(pDX, IDC_OPEPLN_BASICPRM_OPE_ETIME_DP, m_OpeEndTime);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_UL_LIST, m_UpLinkList);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_RNG_LIST, m_RngList);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_CMD_LIST, m_CmdList);
	DDX_Control(pDX, IDC_OPEPLN_OPEOP_INITSET_CMB, m_OpInitCmb);
	DDX_Control(pDX, IDC_OPEPLN_OPEOP_RDOP_CMB, m_OpRDPCmb);
	DDX_Check(pDX, IDC_OPEPLN_RDNS_AGC_CB, m_AGCChkValue);
	DDX_Check(pDX, IDC_OPEPLN_RDNS_RNG_CB, m_RNGChkValue);
	DDX_Check(pDX, IDC_OPEPLN_RDNS_TLM_CB, m_TLMChkValue);
	DDX_Check(pDX, IDC_OPEPLN_RDNS_CMD_CB, m_CMDChkValue);
	DDX_Control(pDX, IDC_OPEPLN_PICCTRL, m_OperationGraph);
	DDX_Text(pDX, IDC_OPEPLN_BASICPRM_OPEID_EDIT2, m_strPlanID);
	DDX_Check(pDX, IDC_OPEPLN_BASICPRM_UL_CHECK, m_UpLinkChkBox);
	DDX_Check(pDX, IDC_OPEPLN_BASICPRM_RNG_CHECK, m_RngChkBox);
	DDX_Check(pDX, IDC_OPEPLN_BASICPRM_CMD_CHECK, m_CmdChkBox);
	DDX_Check(pDX, IDC_OPEPLN_BASICPRM_TLMS_CHECK, m_TLMSChkBox);
	DDX_Check(pDX, IDC_OPEPLN_BASICPRM_TLMX_CHECK, m_TLMXChkBox);
	DDX_Check(pDX, IDC_OPEPLN_BASICPRM_TLMKA_CHECK, m_TLMKaChkBox);
	DDX_Text(pDX, IDC_OPEPLN_BR_CID_TLMS_EDIT, m_strBRTLMS);
	DDX_Text(pDX, IDC_OPEPLN_BR_CID_TLMX_EDIT, m_strBRTLMX);
	DDX_Text(pDX, IDC_OPEPLN_BR_CID_TLMKA_EDIT, m_strBRTLMKa);
	DDX_Text(pDX, IDC_OPEPLN_BR_CID_HSTLM_EDIT, m_strBRHTLM);
	DDX_Text(pDX, IDC_OPEPLN_BR_CID_CMDS_EDIT, m_strBRCMDS);
	DDX_Text(pDX, IDC_OPEPLN_BR_CID_CMDX_EDIT, m_strBRCMDX);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_OPEID_EDIT2, m_PlanIDEdit);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_OPE_SDAY_DP, m_OpeStartDayCtrl);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_OPE_EDAY_DP, m_OpeEndDayCtrl);
	DDX_Text(pDX, IDC_OPEPLN_BASICPRM_SAT_EDIT, m_strSatellite);
	DDX_DateTimeCtrl(pDX, IDC_OPEPLN_BASICPRM_PRED_SDAY_DP, m_PredAosDay);
	DDX_DateTimeCtrl(pDX, IDC_OPEPLN_BASICPRM_PRED_STIME_DP, m_PredAosTime);
	DDX_DateTimeCtrl(pDX, IDC_OPEPLN_BASICPRM_PRED_EDAY_DP, m_PredLosDay);
	DDX_DateTimeCtrl(pDX, IDC_OPEPLN_BASICPRM_PRED_ETIME_DP, m_PredLosTime);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_PRED_SDAY_DP, m_PredAosDayCtrl);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_PRED_EDAY_DP, m_PredLosDayCtrl);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_TLMS_CHECK, m_TLMSCtrl);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_TLMKA_CHECK, m_TLMKaCtrl);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_OPE_STIME_DP, m_OpeStartTimeCtrl);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_OPE_ETIME_DP, m_OpeEndTimeCtrl);
	DDX_Text(pDX, IDC_OPEPLN_BASICPRM_PREDFILE_EDIT, m_PredFileName);
	DDX_Control(pDX, IDC_OPEPLN_BR_CID_BTN, m_CidBtnCtrl);
	DDX_Control(pDX, IDC_OPEPLN_BR_CID_CMDX_EDIT, m_CmdXEditCtrl);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_TLMS_LIST, m_TlmSList);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_TLMX_LIST, m_TlmXList);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_TLMKA_LIST, m_TlmKaList);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_TLMS_ADD_BTN, m_TlmSAddBtn);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_TLMS_EDIT_BTN, m_TlmSEditBtn);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_TLMS_DEL_BTN, m_TlmSDelBtn);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_TLMKA_ADD_BTN, m_TlmKaAddBtn);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_TLMKA_EDIT_BTN, m_TlmKaEditBtn);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_TLMKA_DEL_BTN, m_TlmKaDelBtn);
	DDX_Control(pDX, IDC_OPEPLN_BR_CID_HSTLM_EDIT, m_TlmXHEditCtrl);
	DDX_Control(pDX, IDC_OPEPLN_BR_CID_CMDS_EDIT, m_CmdSEditCtrl);
	DDX_Text(pDX, IDC_OPEPLN_OPEOP_TOUT__EDIT, m_TrsPowerStr);
	DDX_Text(pDX, IDC_OPEPLN_OPEOP_TDOP_EDIT, m_OpTDPStr);
	DDX_Text(pDX, IDC_OPEPLN_BASICPRM_ANTSTDBY_EDIT, m_AntStandbyStr);
	DDX_Text(pDX, IDC_OPEPLN_BASICPRM_ANTZNTH_EDIT, m_AntZenithStr);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_PRED_STIME_DP, m_PredAosTimeCtrl);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_PRED_ETIME_DP, m_PredLosTimeCtrl);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_TLMX_CHECK, m_TLMXCtrl);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_TLMX_ADD_BTN, m_TlmXAddBtn);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_TLMX_EDIT_BTN, m_TlmXEditBtn);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_TLMX_DEL_BTN, m_TlmXDelBtn);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_UL_CHECK, m_UplinkChkBtnCtrl);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_RNG_CHECK, m_RngChkBtnCtrl);
	DDX_Control(pDX, IDC_OPEPLN_BASICPRM_CMD_CHECK, m_CmdChkBtnCtrl);
}


BEGIN_MESSAGE_MAP(COperationPlan, CDialogBase)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &COperationPlan::OnMenuClose)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_UL_ADD_BTN, &COperationPlan::OnBnClickedOpeplnBasicprmUlAddBtn)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_UL_EDIT_BTN, &COperationPlan::OnBnClickedOpeplnBasicprmUlEditBtn)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_UL_DEL_BTN, &COperationPlan::OnBnClickedOpeplnBasicprmUlDelBtn)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_RNG_ADD_BTN, &COperationPlan::OnBnClickedOpeplnBasicprmRngAddBtn)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_RNG_EDIT_BTN, &COperationPlan::OnBnClickedOpeplnBasicprmRngEditBtn)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_RNG_DEL_BTN, &COperationPlan::OnBnClickedOpeplnBasicprmRngDelBtn)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_CMD_ADD_BTN, &COperationPlan::OnBnClickedOpeplnBasicprmCmdAddBtn)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_CMD_EDIT_BTN, &COperationPlan::OnBnClickedOpeplnBasicprmCmdEditBtn)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_CMD_DEL_BTN, &COperationPlan::OnBnClickedOpeplnBasicprmCmdDelBtn)
	ON_BN_CLICKED(IDC_OPEPLN_DETAIL_BTN, &COperationPlan::OnBnClickedOpeplnDetailBtn)
	ON_BN_CLICKED(IDC_OPEPLN_PRINT_BTN, &COperationPlan::OnBnClickedOpeplnPrintBtn)
	ON_BN_CLICKED(IDC_OPEPLN_ENTRY_BTN, &COperationPlan::OnBnClickedOpeplnEntryBtn)
	ON_BN_CLICKED(IDC_OPEPLN_RDNS_AGC_CB, &COperationPlan::OnBnClickedOpeplnRdnsAgcCb)
	ON_BN_CLICKED(IDC_OPEPLN_RDNS_RNG_CB, &COperationPlan::OnBnClickedOpeplnRdnsRngCb)
	ON_BN_CLICKED(IDC_OPEPLN_RDNS_TLM_CB, &COperationPlan::OnBnClickedOpeplnRdnsTlmCb)
	ON_BN_CLICKED(IDC_OPEPLN_RDNS_CMD_CB, &COperationPlan::OnBnClickedOpeplnRdnsCmdCb)
	ON_CBN_SELCHANGE(IDC_OPEPLN_OPEOP_INITSET_CMB, &COperationPlan::OnCbnSelchangeOpeplnOpeopInitsetCmb)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_OPEPLN_BR_CID_BTN, &COperationPlan::OnBnClickedOpeplnBrCidBtn)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_UL_CHECK, &COperationPlan::OnBnClickedOpeplnBasicprmUlCheck)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_RNG_CHECK, &COperationPlan::OnBnClickedOpeplnBasicprmRngCheck)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_CMD_CHECK, &COperationPlan::OnBnClickedOpeplnBasicprmCmdCheck)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_TLMS_CHECK, &COperationPlan::OnBnClickedOpeplnBasicprmTlmsCheck)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_TLMX_CHECK, &COperationPlan::OnBnClickedOpeplnBasicprmTlmxCheck)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_TLMKA_CHECK, &COperationPlan::OnBnClickedOpeplnBasicprmTlmkaCheck)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_OPEPLN_STLT_LIST, &COperationPlan::OnLvnItemchangedOpeplnStltList)
	ON_NOTIFY(NM_DBLCLK, IDC_OPEPLN_STLT_LIST, &COperationPlan::OnNMDblclkOpeplnStltList)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_TLMS_ADD_BTN, &COperationPlan::OnBnClickedOpeplnBasicprmTlmsAddBtn)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_TLMS_EDIT_BTN, &COperationPlan::OnBnClickedOpeplnBasicprmTlmsEditBtn)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_TLMS_DEL_BTN, &COperationPlan::OnBnClickedOpeplnBasicprmTlmsDelBtn)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_TLMX_ADD_BTN, &COperationPlan::OnBnClickedOpeplnBasicprmTlmxAddBtn)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_TLMX_EDIT_BTN, &COperationPlan::OnBnClickedOpeplnBasicprmTlmxEditBtn)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_TLMX_DEL_BTN, &COperationPlan::OnBnClickedOpeplnBasicprmTlmxDelBtn)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_TLMKA_ADD_BTN, &COperationPlan::OnBnClickedOpeplnBasicprmTlmkaAddBtn)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_TLMKA_EDIT_BTN, &COperationPlan::OnBnClickedOpeplnBasicprmTlmkaEditBtn)
	ON_BN_CLICKED(IDC_OPEPLN_BASICPRM_TLMKA_DEL_BTN, &COperationPlan::OnBnClickedOpeplnBasicprmTlmkaDelBtn)
	ON_WM_CLOSE()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_OPEPLN_BASICPRM_OPE_SDAY_DP, &COperationPlan::OnDtnDatetimechangeOpeplnBasicprmOpeSdayDp)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_OPEPLN_BASICPRM_OPE_STIME_DP, &COperationPlan::OnDtnDatetimechangeOpeplnBasicprmOpeStimeDp)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_OPEPLN_BASICPRM_OPE_EDAY_DP, &COperationPlan::OnDtnDatetimechangeOpeplnBasicprmOpeEdayDp)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_OPEPLN_BASICPRM_OPE_ETIME_DP, &COperationPlan::OnDtnDatetimechangeOpeplnBasicprmOpeEtimeDp)
END_MESSAGE_MAP()


// COperationPlan メッセージ ハンドラー


/*============================================================================*/
/*! COperationPlan

-# ファイル-閉じるメニュー処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnMenuClose()
{
	if (m_bNewPlan == FALSE)
	{
		// 編集時変更チェック
		if (CheckPlanDataDlg())
		{
			if (IDCANCEL == AfxMessageBoxHooked(_T("設定が変更されています。\n設定を破棄しますか？"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			{
				return;
			}
		}
	}

	//	OnOK();
	ShowWindow(SW_HIDE);
}


/*============================================================================*/
/*! COperationPlan

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL COperationPlan::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	CString str;

	// 日時形式フォーマット変更
	m_OpeStartDayCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_OpeStartTimeCtrl.SetFormat(_T("HH:mm:ss"));
	m_OpeEndDayCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_OpeEndTimeCtrl.SetFormat(_T("HH:mm:ss"));
	m_PredAosDayCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_PredAosTimeCtrl.SetFormat(_T("HH:mm:ss"));
	m_PredLosDayCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_PredLosTimeCtrl.SetFormat(_T("HH:mm:ss"));

	DWORD		dwStyle;
	LVCOLUMN	lvc;
	int         i;

	dwStyle = m_SatelliteList.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_SatelliteList.SetExtendedStyle(dwStyle);

	TCHAR       caption[][32] = { _T("衛星名"), _T("計画ID") };
	int			csize[] = { 100, 85 };
	const int   clmNum = sizeof caption / sizeof caption[0];

	// ヘッダー
	for (int i = 0; i < clmNum; i++)
	{
		lvc.iSubItem = i;
		lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.pszText = caption[i];
		lvc.cchTextMax = (int)wcslen(caption[i]);
		lvc.cx = csize[i];
		m_SatelliteList.InsertColumn(i, &lvc);
	}

	// アップリンク開始終了日時リスト
	dwStyle = m_UpLinkList.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_UpLinkList.SetExtendedStyle(dwStyle);

	TCHAR       caption2[][32] = { _T("開始日時"), _T("終了日時"), _T("") };
	TCHAR       caption3[][32] = { _T("開始日時"), _T("終了日時"), _T("種別") };
	int			csize2[] = { 120, 120, 50 };
	const int   clmNum2 = sizeof caption2 / sizeof caption2[0];

	for (i = 0; i < clmNum2; i++)
	{
		lvc.iSubItem = i;
		lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.pszText = caption2[i];
		lvc.cchTextMax = (int)wcslen(caption2[i]);
		lvc.cx = csize2[i];
		m_UpLinkList.InsertColumn(i, &lvc);
	}

	// RNG 開始終了日時リスト
	dwStyle = m_RngList.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_RngList.SetExtendedStyle(dwStyle);

	for (i = 0; i < clmNum2; i++)
	{
		lvc.iSubItem = i;
		lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.pszText = caption3[i];
		lvc.cchTextMax = (int)wcslen(caption2[i]);
		lvc.cx = csize2[i];
		m_RngList.InsertColumn(i, &lvc);
	}

	// CMD 開始終了日時リスト
	dwStyle = m_CmdList.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_CmdList.SetExtendedStyle(dwStyle);

	for (i = 0; i < clmNum2; i++)
	{
		lvc.iSubItem = i;
		lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.pszText = caption2[i];
		lvc.cchTextMax = (int)wcslen(caption2[i]);
		lvc.cx = csize2[i];
		m_CmdList.InsertColumn(i, &lvc);
	}

	// TLM S 開始終了日時リスト
	dwStyle = m_TlmSList.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_TlmSList.SetExtendedStyle(dwStyle);

	for (i = 0; i < clmNum2; i++)
	{
		lvc.iSubItem = i;
		lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.pszText = caption2[i];
		lvc.cchTextMax = (int)wcslen(caption2[i]);
		lvc.cx = csize2[i];
		m_TlmSList.InsertColumn(i, &lvc);
	}

	// TLM X 開始終了日時リスト
	dwStyle = m_TlmXList.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_TlmXList.SetExtendedStyle(dwStyle);

	for (i = 0; i < clmNum2; i++)
	{
		lvc.iSubItem = i;
		lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.pszText = caption2[i];
		lvc.cchTextMax = (int)wcslen(caption2[i]);
		lvc.cx = csize2[i];
		m_TlmXList.InsertColumn(i, &lvc);
	}

	// TLM Ka 開始終了日時リスト
	dwStyle = m_TlmKaList.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_TlmKaList.SetExtendedStyle(dwStyle);

	for (i = 0; i < clmNum2; i++)
	{
		lvc.iSubItem = i;
		lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.pszText = caption2[i];
		lvc.cchTextMax = (int)wcslen(caption2[i]);
		lvc.cx = csize2[i];
		m_TlmKaList.InsertColumn(i, &lvc);
	}

	//-----------------------------------
	// 計画ID
#if 0
	m_PlanIDEdit.EnableMask(_T("       dddd"), // The mask string
		_T("000000-____"), // Literal, "_" char = character entry
		_T(' ')); // Default char
	m_PlanIDEdit.SetValidChars(NULL); // Valid string characters
	m_PlanIDEdit.SetWindowText(_T("000000-1234"));
#endif

	int s = theApp.GetSelectStation();	// 局

	// 衛星固有情報取得
	if (s == eStation_Usuda64)	// 臼田64m
	{
		data_get_from_db64();
	}
	else if (s == eStation_Usuda642)	// 臼田54m
	{
		data_get_from_db54();
	}
	else if (s == eStation_Uchinoura34)	// 内之浦34m
	{
		data_get_from_db34();
	}
	else if (s == eStation_Uchinoura20)	// 内之浦20m
	{
		data_get_from_db20();
	}

	//-----------------------------------
	// 描画領域
	m_OperationGraph.SetParent(this);

	UpdateData(FALSE);
	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL COperationPlan::InitDlg()
{
	int i = 0;

	CSatelliteData& sd = theApp.GetSatelliteData();
	int s = theApp.GetSelectStation();	// 局

	m_IdxEqsat = GetIndexEqSatDBData(m_SatelliteName);

#if 0
	// 衛星・計画IDリスト
	m_SatelliteList.DeleteAllItems();
#endif

	//-----------------------------------
	// 予報値
#if 1
	//	m_PredFileName = m_PlanData.sz_pred_path.Right(m_PlanData.sz_pred_path.GetLength() - m_PlanData.sz_pred_path.ReverseFind('/') - 1);	// フルパスからファイル名を取得
	m_PredFileName = m_PlanData.sz_pred_path.Mid(0, m_PlanData.sz_pred_path.Find('.')) + _T(".anp");	// 拡張子を"anp"に変更
	m_PredAosDay = m_tAOS;
	m_PredAosTime = m_tAOS;
	m_PredLosDay = m_tLOS;
	m_PredLosTime = m_tLOS;

#else // UTC
//	tm t1;
//	time_t tm;
//	m_tAOS.GetGmtTm(&t1);
//	tm = mktime(&t1);
//	m_PredAosDay = CTime(tm);
//	m_PredAosTime = CTime(tm);
//	m_tLOS.GetGmtTm(&t1);
//	tm = mktime(&t1);
//	m_PredLosDay = CTime(tm);
//	m_PredLosTime = CTime(tm);
#endif

	//-----------------------------------
	// アンテナ予報値データ取得
	GetAnpData();

	//-----------------------------------
	// 運用日時

	// 新規の場合、アンテナ予報値で初期化
	m_OpeStartDay = m_tAOS;
	m_OpeStartTime = m_tAOS;

	m_OpeEndDay = m_tLOS;
	m_OpeEndTime = m_tLOS;

	m_OprPlnDataOrg.m_OpetDay.start = m_OpeStartDay;
	m_OprPlnDataOrg.m_OpetDay.end = m_OpeEndDay;

	// 範囲
	m_OpeStartDayCtrl.SetRange(&m_tAOS, &m_tLOS);
	//	m_OpeStartTimeCtrl.SetRange(&m_tAOS, &m_tLOS);
	m_OpeEndDayCtrl.SetRange(&m_tAOS, &m_tLOS);
	//	m_OpeEndTimeCtrl.SetRange(&m_tAOS, &m_tLOS);

	// 描画用
	m_tOpeS = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	m_tOpeE = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	// Up Link
	m_UpLinkList.DeleteAllItems();
	m_UpLinkChkBox = FALSE;
	m_OprPlnDataOrg.m_UpLinkChk = FALSE;

	// RNG
	m_RngList.DeleteAllItems();
	m_RngChkBox = FALSE;
	m_OprPlnDataOrg.m_RngChk = FALSE;

	// RNG 種別取得
	m_RngEleList.clear();
	if (s == eStation_Usuda64)	// 臼田64m
	{
		if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_Y_ID)
		{
			m_RngEleList.push_back(_T("XX新"));
			m_RngEleList.push_back(_T("XX新再"));
		}
		else
		{
			m_RngEleList.push_back(_T("SS"));
			if(sd.m_satinfo[m_IdxEqsat].i_rxfq_x == 1)
			{
				m_RngEleList.push_back(_T("SX"));
			}
		}
	}
	else if (s == eStation_Usuda642)	// 臼田54m
	{
		if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_Y_ID)
		{
			m_RngEleList.push_back(_T("XX"));
			m_RngEleList.push_back(_T("XX再"));
			m_RngEleList.push_back(_T("XKa"));
			m_RngEleList.push_back(_T("XKa再"));
		}
	}
	else if (s == eStation_Uchinoura34)	// 内之浦34m
	{
		if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_Y_ID)
		{
			m_RngEleList.push_back(_T("XX新"));
			m_RngEleList.push_back(_T("XX新再"));
		}
		else
		{
			m_RngEleList.push_back(_T("SS"));
			if(sd.m_satinfo[m_IdxEqsat].i_rxfq_x == 1)
			{
				m_RngEleList.push_back(_T("SX"));
			}
		}
	}
	else if (s == eStation_Uchinoura20)	// 内之浦20m
	{
		m_RngEleList.push_back(_T("SS"));
		if (sd.m_satinfo[m_IdxEqsat].i_rxfq_x == 1)
		{
			m_RngEleList.push_back(_T("SX"));
		}
	}

	// CMD
	m_CmdList.DeleteAllItems();
	m_CmdChkBox = FALSE;
	m_OprPlnDataOrg.m_CmdChk = FALSE;


	// VLBIの場合画面をマスクする
	if (m_SatelliteName.CompareNoCase(_T("VLBI")) == 0)
	{
		CWnd *pCtrl = GetTopWindow();
		do{
			UINT id = pCtrl->GetDlgCtrlID();
			if ((id == IDC_OPEPLN_BASICPRM_SAT_STATIC) ||
				(id == IDC_OPEPLN_BASICPRM_SAT_EDIT) ||
				(id == IDC_OPEPLN_BASICPRM_OPEID_STATIC2) ||
				(id == IDC_OPEPLN_BASICPRM_OPEID_EDIT2) ||
				(id == IDC_OPEPLN_BASICPRM_OPESTART_STATIC) ||
				(id == IDC_OPEPLN_BASICPRM_OPE_SDAY_DP) ||
				(id == IDC_OPEPLN_BASICPRM_OPE_STIME_DP) ||
				(id == IDC_OPEPLN_BASICPRM_OPEEND_STATIC) ||
				(id == IDC_OPEPLN_BASICPRM_OPE_EDAY_DP) ||
				(id == IDC_OPEPLN_BASICPRM_OPE_ETIME_DP) ||
				(id == IDC_OPEPLN_BASICPRM_PREDFILE_STATIC) ||
				(id == IDC_OPEPLN_BASICPRM_PREDFILE_EDIT) ||
				(id == IDC_OPEPLN_BASICPRM_PREDSTART_STATIC) ||
				(id == IDC_OPEPLN_BASICPRM_PREDEND_STATIC) ||
				(id == IDC_OPEPLN_BR_CID2_STATIC) ||
				(id == IDC_OPEPLN_BR_CID_BTN) ||
				(id == IDC_OPEPLN_OPEOP_INITSET_STATIC) ||
				(id == IDC_OPEPLN_OPEOP_TDOP_STATIC) ||
				(id == IDC_OPEPLN_OPEOP_TDOP_EDIT) ||
				(id == IDC_OPEPLN_OPEOP_RDOP_STATIC) ||
				(id == IDC_OPEPLN_OPEOP_TOUT_STATIC) ||
				(id == IDC_OPEPLN_OPEOP_TOUT__EDIT) ||
				(id == IDC_OPEPLN_BR_CID_TLMS_STATIC) ||
				(id == IDC_OPEPLN_BR_CID_TLMS_EDIT) ||
				(id == IDC_OPEPLN_BR_CID_TLMX_STATIC) ||
				(id == IDC_OPEPLN_BR_CID_TLMX_EDIT) ||
				(id == IDC_OPEPLN_BR_CID_HSTLM_STATIC) ||
				(id == IDC_OPEPLN_BR_CID_HSTLM_EDIT) ||
				(id == IDC_OPEPLN_BR_CID_CMDS_STATIC) ||
				(id == IDC_OPEPLN_BR_CID_CMDS_EDIT) ||
				(id == IDC_OPEPLN_BR_CID_CMDX_STATIC) ||
				(id == IDC_OPEPLN_BR_CID_CMDX_EDIT) ||
				(id == IDC_OPEPLN_BASICPRM_ANTSTDBY_STATIC) ||
				(id == IDC_OPEPLN_BASICPRM_ANTSTDBY_EDIT) ||
				(id == IDC_OPEPLN_BASICPRM_ANTZNTH_STATIC) ||
				(id == IDC_OPEPLN_BASICPRM_ANTZNTH_EDIT) ||
				(id == IDC_OPEPLN_STLT_LIST) ||
				(id == IDC_OPEPLN_DETAIL_BTN) ||
				(id == IDC_OPEPLN_PRINT_BTN) ||
				(id == IDC_OPEPLN_ENTRY_BTN))
			{
				//			pCtrl->EnableWindow(TRUE);
			}
			else
			{
				pCtrl->EnableWindow(FALSE);
			}
		} while (pCtrl = pCtrl->GetNextWindow());
	}
	else
	{
		CWnd *pCtrl = GetTopWindow();
		do{
			UINT id = pCtrl->GetDlgCtrlID();
			if ( (id != IDC_OPEPLN_BASICPRM_PRED_SDAY_DP) &&
				 (id != IDC_OPEPLN_BASICPRM_PRED_STIME_DP) &&
				 (id != IDC_OPEPLN_BASICPRM_PRED_EDAY_DP) &&
				 (id != IDC_OPEPLN_BASICPRM_PRED_ETIME_DP) )
			{
				pCtrl->EnableWindow(TRUE);
			}

		} while (pCtrl = pCtrl->GetNextWindow());
	}

	//-----------------------------------
	// 基本パラメータ-TLM

	// TLM S 活性/非活性
	if (sd.m_satinfo[m_IdxEqsat].i_rxfq_s == 1)
	{
		// TLM S は活性
		m_TLMSCtrl.SetCheck(0);
		m_TLMSCtrl.EnableWindow(TRUE);
		m_TlmSAddBtn.EnableWindow(TRUE);
		m_TlmSEditBtn.EnableWindow(TRUE);
		m_TlmSDelBtn.EnableWindow(TRUE);
		m_TlmSList.EnableWindow(TRUE);
	}
	else
	{
		// TLM S は非活性
		m_TLMSCtrl.SetCheck(0);
		m_TLMSCtrl.EnableWindow(FALSE);
		m_TlmSAddBtn.EnableWindow(FALSE);
		m_TlmSEditBtn.EnableWindow(FALSE);
		m_TlmSDelBtn.EnableWindow(FALSE);
		m_TlmSList.EnableWindow(FALSE);
	}

	// TLM X 活性/非活性
	if (sd.m_satinfo[m_IdxEqsat].i_rxfq_x == 1)
	{
		// TLM X は活性
		m_TLMXCtrl.SetCheck(0);
		m_TLMXCtrl.EnableWindow(TRUE);
		m_TlmXAddBtn.EnableWindow(TRUE);
		m_TlmXEditBtn.EnableWindow(TRUE);
		m_TlmXDelBtn.EnableWindow(TRUE);
		m_TlmXList.EnableWindow(TRUE);
	}
	else
	{
		// TLM X は非活性
		m_TLMXCtrl.SetCheck(0);
		m_TLMXCtrl.EnableWindow(FALSE);
		m_TlmXAddBtn.EnableWindow(FALSE);
		m_TlmXEditBtn.EnableWindow(FALSE);
		m_TlmXDelBtn.EnableWindow(FALSE);
		m_TlmXList.EnableWindow(FALSE);
	}

	// TLM Ka 活性/非活性
	if (sd.m_satinfo[m_IdxEqsat].i_rxfq_ka == 1)
	{
		// TLM Ka は活性
		m_TLMKaCtrl.SetCheck(0);
		m_TLMKaCtrl.EnableWindow(TRUE);
		m_TlmKaAddBtn.EnableWindow(TRUE);
		m_TlmKaEditBtn.EnableWindow(TRUE);
		m_TlmKaDelBtn.EnableWindow(TRUE);
		m_TlmKaList.EnableWindow(TRUE);
	}
	else
	{
		// TLM Ka は非活性
		m_TLMKaCtrl.SetCheck(0);
		m_TLMKaCtrl.EnableWindow(FALSE);
		m_TlmKaAddBtn.EnableWindow(FALSE);
		m_TlmKaEditBtn.EnableWindow(FALSE);
		m_TlmKaDelBtn.EnableWindow(FALSE);
		m_TlmKaList.EnableWindow(FALSE);
	}


	//-----------------------------------
	// 基本パラメータ-TLM S
	m_TlmSList.DeleteAllItems();
	m_TLMSChkBox = FALSE;
	m_OprPlnDataOrg.m_TlmSChk = FALSE;

	//-----------------------------------
	// 基本パラメータ-TLM X
	m_TlmXList.DeleteAllItems();
	m_TLMXChkBox = FALSE;
	m_OprPlnDataOrg.m_TlmXChk = FALSE;

	//-----------------------------------
	// 基本パラメータ-TLM Ka
	m_TlmKaList.DeleteAllItems();
	m_TLMKaChkBox = FALSE;
	m_OprPlnDataOrg.m_TlmKaChk = FALSE;

#if 0
	//-----------------------------------
	// 基本パラメータ-ANT待ち受け指向通知
	m_AntStandbyCmb.ResetContent();
	for (i = 0; strAriNasi[i] != _T(""); ++i)
	{
		m_AntStandbyCmb.InsertString(-1, strAriNasi[i]);
	}
	m_AntStandbyCmb.SetCurSel(1);
	m_bAntStandby = FALSE;
	m_OprPlnDataOrg.m_AntStandby = 1;

	//-----------------------------------
	// 基本パラメータ-ANT天頂指向有無
	m_AntZenithCmb.ResetContent();
	for (i = 0; strANTZenith[i] != _T(""); ++i)
	{
		m_AntZenithCmb.InsertString(-1, strANTZenith[i]);
	}
	m_AntZenithCmb.SetCurSel(2);
	m_bAntZenith = FALSE;
	m_OprPlnDataOrg.m_AntZenith = 2;

	if (s == 0 || s == 1)	// 臼田64m or 臼田54m
	{
		m_AntZenithCmb.EnableWindow(FALSE);
	}
	else
	{
		m_AntZenithCmb.EnableWindow(TRUE);
	}
#endif

	//-----------------------------------
	// 運用オプション-初期設定
	m_OpInitCmb.ResetContent();
	for (i = 0; strAriNasi[i] != _T(""); ++i)
	{
		m_OpInitCmb.InsertString(-1, strAriNasi[i]);
	}
	if (m_SatelliteName.CompareNoCase(_T("VLBI")) == 0)
	{
		m_OpInitCmb.SetCurSel(1);
		m_bOpInit = FALSE;
		m_OprPlnDataOrg.m_OpInit = 0;
	}
	else
	{
		m_OpInitCmb.SetCurSel(0);
		m_bOpInit = TRUE;
		m_OprPlnDataOrg.m_OpInit = 1;
	}

#if 0
	//-----------------------------------
	// 運用オプション-送信機ドップラ補償
	m_OpTDPCmb.ResetContent();
	for (i = 0; strTxDoppler[i] != _T(""); ++i)
	{
		m_OpTDPCmb.InsertString(-1, strTxDoppler[i]);
	}
	m_OpTDPCmb.SetCurSel(0);
	m_OprPlnDataOrg.m_OpTDP = 0;
#endif

	//-----------------------------------
	// 運用オプション-受信機ドップラ補償
	m_OpRDPCmb.ResetContent();
	if(s == eStation_Usuda642)	// 臼田54m
	{
		for(i=0; strRxDoppler54[i] != _T(""); ++i)
		{
			m_OpRDPCmb.InsertString(-1, strRxDoppler54[i]);
		}
	}
	else
	{
		for (i = 0; strRxDoppler[i] != _T(""); ++i)
		{
			m_OpRDPCmb.InsertString(-1, strRxDoppler[i]);
		}
	}
	m_OpRDPCmb.SetCurSel(0);
	m_OprPlnDataOrg.m_OpRDP = 0;

#if 0
	//-----------------------------------
	// 運用オプション-送信出力
	m_OpTOutCmb.ResetContent();
	if (s == 0)	// 臼田64m
	{
		for (i = 0; strTrsPwr64[i] != _T(""); ++i)
		{
			m_OpTOutCmb.InsertString(-1, strTrsPwr64[i]);
		}
	}
	else if (s == 1)	// 臼田54m
	{
		for (i = 0; strTrsPwr64[i] != _T(""); ++i)
		{
			m_OpTOutCmb.InsertString(-1, strTrsPwr64[i]);
		}
	}
	else if (s == 2)	// 内之浦34m
	{
		m_OpTOutType = TRSPWRX_X;
		if (m_OpTOutType == TRSPWRX_X)
		{
			for (i = 0; strTrsPwrX34[i] != _T(""); ++i)
			{
				m_OpTOutCmb.InsertString(-1, strTrsPwrX34[i]);
			}
		}
		else if (m_OpTOutType == TRSPWRX_S)
		{
			for (i = 0; strTrsPwrS34[i] != _T(""); ++i)
			{
				m_OpTOutCmb.InsertString(-1, strTrsPwrS34[i]);
			}
		}
	}
	else if (s == 3)	// 内之浦20m
	{
		for (i = 0; strTrsPwr20[i] != _T(""); ++i)
		{
			m_OpTOutCmb.InsertString(-1, strTrsPwr20[i]);
		}
	}
	m_OpTOutCmb.SetCurSel(0);
	m_OprPlnDataOrg.m_OpTOut = 0;
#endif


	/* レディネスチェック */

	// AGC
	m_AGCChkValue = FALSE;
	m_OprPlnDataOrg.m_AgcChkValue = FALSE;

	// RNG
	m_RNGChkValue = FALSE;
	m_OprPlnDataOrg.m_RndChkValue = FALSE;

	// TLM
	m_TLMChkValue = FALSE;
	m_OprPlnDataOrg.m_TlmChkValue = FALSE;

	// CMD
	m_CMDChkValue = FALSE;
	m_OprPlnDataOrg.m_CmdChkValue = FALSE;


	// CID
	// プロパティファイル（Property.dat）よりデフォルトCIDを取得する。
	stOpeProp stProp;
	theApp.GetSatelliteData().ReadPropertyFile(m_SatelliteName, stProp);

	stCIDParam stCid;
	vector<CString> list = theApp.GetSatelliteData().m_CIDSlctData.GetCidList(m_SatelliteName);
	if (list.size() > 0)
	{
		CString strCid = CString(stProp.cid);
		auto itrl = std::find(list.begin(), list.end(), strCid);
		if (itrl == list.end())
		{
			// デフォルトCIDがCIDファイルに無い場合、一番最初のCIDをセット。
			strCid = list[0];
		}

		map<CString, map<CString, stCIDParam>> &cidlist = theApp.GetSatelliteData().m_CIDSlctData.GetCidParamList();

		if (cidlist.size() > 0)
		{
			stCid = CCIDSlctData::TrimInvalidString(cidlist[m_SatelliteName][strCid]);
			m_CidBtnCtrl.SetWindowTextW(strCid);
			m_OprPlnDataOrg.m_strCID = strCid;
		}
	}

	// ビットレート-TLM S
	m_strBRTLMS = stCid.TlmBitRateS;

	// ビットレート-TLM X
	m_strBRTLMX = stCid.TlmBitRateX;

	// ビットレート-TLM Ka
	m_strBRTLMKa = stCid.TlmBitRateKa;

	// ビットレート-高速TLM
	m_strBRHTLM = stCid.TlmBitRateHX;

	// ビットレート-CMD S
	m_strBRCMDS = stCid.CmdBitRateS;

	// ビットレート-CMD X
	m_strBRCMDX = stCid.CmdBitRateX;

	// レンジ計測パラメータ
	m_strRngMesMode = stCid.RngMesMode;

	//-----------------------------------
	// 運用オプション-送信機ドップラ補償
	m_OpTDPStr = stCid.TransDoppler;

	//-----------------------------------
	// 運用オプション-送信出力
	m_TrsPowerStr = stCid.TrsPower;

	//-----------------------------------
	// 基本パラメータ-ANT待ち受け指向通知
	m_AntStandbyStr = stCid.AntAosPosn;
	if (m_AntStandbyStr == strAriNasi[ARINASI_ARI])
	{
		m_bAntStandby = TRUE;
	}
	else
	{
		m_bAntStandby = FALSE;
	}

	//-----------------------------------
	// 基本パラメータ-ANT天頂指向有無
#if 1
	if ((stCid.AntWaitPoint.CompareNoCase(_T("天頂")) == 0) || (stCid.AntWaitPoint.CompareNoCase(_T("天頂(LOCK)")) == 0))
	{
		m_AntZenithStr = strANTZenith[ANTZENITH_ARI_SWTOW];
		m_bAntZenith = TRUE;
	}
	else if (stCid.AntWaitPoint.CompareNoCase(_T("天頂(UNLOCK)")) == 0)
	{
		m_AntZenithStr = strANTZenith[ANTZENITH_ARI];
		m_bAntZenith = TRUE;
	}
	else
	{
		m_AntZenithStr = strANTZenith[ANTZENITH_NASI];
		m_bAntZenith = FALSE;
	}
#else
//	m_AntZenithStr = stCid.AntWaitPoint;
#endif

	UpdateData(FALSE);

	// アンテナ予報値に基づいたAOS側 / LOS側のRF-Trip交差日時を取得する。
	CTime sTime, eTime;
	GetIntersectionDateRFTrip(sTime, eTime);
	m_tAOSRf = sTime;
	m_tLOSRf = eTime;

	// アンテナ予報値に基づいたAOS側 / LOS側のスカイライン交差日時を取得する。
	GetIntersectionDateSkyline(sTime, eTime);
	m_tAOSTLM = sTime;
	m_tLOSTLM = eTime;
	
	m_OperationGraph.Invalidate();

#ifdef _DEBUG
	TRACE("AOS 開始日時            :[%s]\n", CStringA(m_tAOS.Format("%Y-%m-%d %H:%M:%S")));
	TRACE("TOS 終了日時            :[%s]\n", CStringA(m_tLOS.Format("%Y-%m-%d %H:%M:%S")));
	TRACE("AOS RF-Trip交差日時     :[%s]\n", CStringA(m_tAOSRf.Format("%Y-%m-%d %H:%M:%S")));
	TRACE("LOS RF-Trip交差日時     :[%s]\n", CStringA(m_tLOSRf.Format("%Y-%m-%d %H:%M:%S")));
	TRACE("AOS スカイライン交差日時:[%s]\n", CStringA(m_tAOSTLM.Format("%Y-%m-%d %H:%M:%S")));
	TRACE("LOS スカイライン交差日時:[%s]\n", CStringA(m_tLOSTLM.Format("%Y-%m-%d %H:%M:%S")));
#endif

	return TRUE;
}

/*============================================================================*/
/*! COperationPlan

-# ダイアログ更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL COperationPlan::UpdateOperationPlanDlg()
{
	// 計画データ初期化
	InitPlanData();

	// 衛星名
	m_strSatellite = m_SatelliteName;

	// 計画ID
	CString strLeft = _T("");
	CString strEntry = _T("");
	strLeft = m_PassID.Mid(0, 6);	// 左側取得
	m_PassIDMask = strLeft;
	strEntry.Format(_T("%s-____"), (LPCTSTR)m_PassIDMask);

	m_PlanIDEdit.EnableMask(_T("       DDDD"),	// The mask string
		strEntry,								// Literal, "_" char = character entry
		_T(' '));								// Default char
	m_PlanIDEdit.SetValidChars(NULL); // Valid string characters
	m_PlanIDEdit.SetWindowText(m_PassID);
	m_strPlanID = m_PassID;		// これでセットしないと計画IDの右側に値が入らない。

	//----------------------------------------------------

	// 受信環境の設定
#if 0
	if (rev_env_set() < 0) 
	{
		CString strNo;
		strNo.Format(_T("%d"), BASE_OPE_NO);
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T(""), strNo, _T(""), nLogEx::info);
		return FALSE;
	}
#endif

	// 計画立案制御権要求の取得（T.B.D）


	// 計画一覧ファイルからデータ取得
	GetPlanList();

	// 衛星固有情報取得
	GetSatelliteInfoData(m_SatelliteName);

	CSatelliteData& sd = theApp.GetSatelliteData();
//	int x_tx = sd.satinfo.i_x_tx_sel;

	// 予報値時刻の取得(予報値一覧)
	if (GetPredTime(m_SatelliteName, m_PassID, m_tAOS, m_tLOS, m_PlanData) == FALSE)
	{
		CString strMsg;
		strMsg.Format(_T("予報値一覧から衛星名%sの予報値が取得できなかったため、計画立案画面を閉じます。\n"), m_SatelliteName);
		AfxMessageBox(strMsg, MB_OK | MB_ICONEXCLAMATION);
		m_bForceClosed = TRUE;
		PostMessage(WM_CLOSE);
		return FALSE;
	}

	// 画面初期化
	InitDlg();

	// 計画ファイルの取得
	int s = theApp.GetSelectStation();
	int ret = 0;
	DWORD dw = m_GetForm;
#if 0
	if (GetPlanFile(m_SatelliteName, m_PassID, dw))
#else
	if (m_bNewPlan == FALSE)
#endif
	{
		/* 編集	*/

		if (s == eStation_Usuda64)	// 臼田64m
		{
			ret = ReadPlanFile64(m_SatelliteName, m_PassID, m_PlanData, dw);
		}
		else if (s == eStation_Usuda642)	// 臼田54m
		{
			ret = ReadPlanFile54(m_SatelliteName, m_PassID, m_PlanData, dw);
		}
		else if (s == eStation_Uchinoura34)	// 内之浦34m
		{
			ret = ReadPlanFile34(m_SatelliteName, m_PassID, m_PlanData, dw);
		}
		else if (s == eStation_Uchinoura20)	// 内之浦20m
		{
			ret = ReadPlanFile20(m_SatelliteName, m_PassID, m_PlanData, dw);
		}

		// 画面更新
		if (ret == 0)
		{
			SetPlanDataDlg(s, m_PassID, m_PlanData);
		}
	}
	else
	{
		/* 新規	*/

		if (s == eStation_Usuda64)	// 臼田64m
		{
			ReadPlanFormFile64(m_SatelliteName, m_PassID, m_PlanData);
		}
		else if (s == eStation_Usuda642)	// 臼田54m
		{
			ReadPlanFormFile54(m_SatelliteName, m_PassID, m_PlanData);
		}
		else if (s == eStation_Uchinoura34)	// 内之浦34m
		{
			ReadPlanFormFile34(m_SatelliteName, m_PassID, m_PlanData);
		}
		else if (s == eStation_Uchinoura20)	// 内之浦20m
		{
			ReadPlanFormFile20(m_SatelliteName, m_PassID, m_PlanData);
		}
	}

	SetPlanData();
	return TRUE;
}

/*============================================================================*/
/*! COperationPlan

-# アップリンク開始終了日時 追加ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmUlAddBtn()
{
	COperationPlanTimeSet dlg;
	CTime      cTime = 0;
	CTime      cSTime = 0;
	CTime      cETime = 0;
	CString    strStartDT, strEndDT;

	int nSelected = m_UpLinkList.GetNextItem(-1, LVNI_SELECTED);

	// リストされている場合、次の開始/終了日時を取得する。
	GetTimeInList(m_UpLinkList, nSelected, cSTime, cETime);

	CTime opeSTime = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	CTime opeETime = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	if (cSTime == 0)
	{
		if (opeSTime > m_tAOSRf)
		{
			cSTime = opeSTime;		// 運用開始日時
		}
		else
		{
			cSTime = m_tAOSRf;		// アンテナ予報値に基づいたAOS側のRF-Trip交差日時
		}
	}
	if (cETime == 0)
	{
		if (opeETime < m_tLOSRf)
		{
			cETime = opeETime;		// 運用終了日時
		}
		else
		{
			cETime = m_tLOSRf;		// アンテナ予報値に基づいたLOS側のRF-Trip交差日時
		}
	}

	while (1)
	{
		dlg.SetDefTime(cSTime, cETime);
		dlg.SetRangeTime(cSTime, cETime);
		if (dlg.DoModal() == IDCANCEL)
		{
			return;
		}

		CTime sd = dlg.m_StartDay;
		CTime st = dlg.m_StartTime;
		CTime ed = dlg.m_EndDay;
		CTime et = dlg.m_EndTime;

		cTime = CTime(sd.GetYear(), sd.GetMonth(), sd.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
		cSTime = cTime;
		strStartDT = cTime.Format("%Y-%m-%d %H:%M:%S");

		cTime = CTime(ed.GetYear(), ed.GetMonth(), ed.GetDay(), et.GetHour(), et.GetMinute(), et.GetSecond());
		cETime = cTime;
		strEndDT = cTime.Format("%Y-%m-%d %H:%M:%S");

		// 開始/終了日時チェック
		if (IsInPlanDayTime(cSTime, cETime) == FALSE)
		{
			AfxMessageBox(_T("アップリンク の開始/終了日時が、運用開始/終了日時の範囲外です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			// 重複チェック
			if (IsConflictTimeInList(m_UpLinkList, cSTime, cETime) == TRUE)
			{
				AfxMessageBox(_T("アップリンクの開始/終了日時が重複しています。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				break;
			}
		}
	}

	// 近接チェック
	IsProximityTime(cSTime, cETime, 0, -1);

	LVITEM       lvi;
	CString      str;

	int count = m_UpLinkList.GetItemCount();

	if (nSelected != -1 && nSelected < count - 1)
	{
		count = nSelected + 1;
	}

	// 開始日時
	lvi.lParam = (LPARAM)count;
	lvi.iItem = count;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = strStartDT.GetBuffer();
	m_UpLinkList.InsertItem(&lvi);
	
	// 終了日時
	lvi.lParam = (LPARAM)count;
	lvi.iItem = count;
	lvi.iSubItem = 1;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = strEndDT.GetBuffer();
	m_UpLinkList.SetItem(&lvi);

	m_UpLinkChkBox = TRUE;
	UpdateData(FALSE);

	m_tOpeS = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	m_tOpeE = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	m_OperationGraph.Invalidate();
}

/*============================================================================*/
/*! COperationPlan

-# アップリンク開始終了日時 編集ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmUlEditBtn()
{
	int nSelected = m_UpLinkList.GetNextItem(-1, LVNI_SELECTED);
	if (nSelected < 0)
	{
		return;
	}

	COleDateTime oleSTime, oleETime;
	CTime cSTime, cETime;
	CString strStartDT, strEndDT;
	COperationPlanTimeSet dlg;

	CTime opeSTime = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	CTime opeETime = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	CTime rangStime, rangEtime;
	if (opeSTime > m_tAOSRf)
	{
		rangStime = opeSTime;		// 運用開始日時
	}
	else
	{
		rangStime = m_tAOSRf;		// アンテナ予報値に基づいたAOS側のRF-Trip交差日時
	}

	if (opeETime < m_tLOSRf)
	{
		rangEtime = opeETime;		// 運用終了日時
	}
	else
	{
		rangEtime = m_tLOSRf;		// アンテナ予報値に基づいたLOS側のRF-Trip交差日時
	}

	while (1)
	{
		strStartDT = m_UpLinkList.GetItemText(nSelected, 0);
		strEndDT = m_UpLinkList.GetItemText(nSelected, 1);

		oleSTime.ParseDateTime(strStartDT);
		oleETime.ParseDateTime(strEndDT);

		cSTime = CTime(oleSTime.GetYear(), oleSTime.GetMonth(), oleSTime.GetDay(), oleSTime.GetHour(), oleSTime.GetMinute(), oleSTime.GetSecond());
		cETime = CTime(oleETime.GetYear(), oleETime.GetMonth(), oleETime.GetDay(), oleETime.GetHour(), oleETime.GetMinute(), oleETime.GetSecond());

		dlg.SetDefTime(cSTime, cETime);
		dlg.SetRangeTime(rangStime, rangEtime);

		if (dlg.DoModal() == IDCANCEL)
		{
			return;
		}

		CTime sd = dlg.m_StartDay;
		CTime st = dlg.m_StartTime;
		CTime ed = dlg.m_EndDay;
		CTime et = dlg.m_EndTime;

		cSTime = CTime(sd.GetYear(), sd.GetMonth(), sd.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
		strStartDT = cSTime.Format("%Y-%m-%d %H:%M:%S");

		cETime = CTime(ed.GetYear(), ed.GetMonth(), ed.GetDay(), et.GetHour(), et.GetMinute(), et.GetSecond());
		strEndDT = cETime.Format("%Y-%m-%d %H:%M:%S");

		// 開始/終了日時チェック
		if (IsInPlanDayTime(cSTime, cETime) == FALSE)
		{
			AfxMessageBox(_T("アップリンク の開始/終了日時が、運用開始/終了日時の範囲外です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			// 重複チェック
			if (IsConflictTimeInList(m_UpLinkList, cSTime, cETime, nSelected) == TRUE)
			{
				AfxMessageBox(_T("アップリンクの開始/終了日時が重複しています。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				break;
			}
		}
	}

	// 近接チェック
	IsProximityTime(cSTime, cETime, 0, nSelected);

	m_UpLinkList.SetItemText(nSelected, 0, strStartDT);
	m_UpLinkList.SetItemText(nSelected, 1, strEndDT);

	m_OperationGraph.Invalidate();
}

/*============================================================================*/
/*! COperationPlan

-# アップリンク開始終了日時 削除ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmUlDelBtn()
{
	int nSelected = m_UpLinkList.GetNextItem(-1, LVNI_SELECTED);
	if (nSelected < 0)
	{
		return;
	}

	// CMD/RNGはアップリンクが必要
	int u_cnt = m_UpLinkList.GetItemCount();
	int r_cnt = m_RngList.GetItemCount();
	int c_cnt = m_CmdList.GetItemCount();

	// 最後のアイテムを削除する場合
	if ((u_cnt == 1) && ((r_cnt > 0) || (c_cnt > 0)))
	{
		if (IDCANCEL == AfxMessageBoxHooked(_T("登録済みのRNG、CMDの実施項目も全て削除します。\nよろしいですか？"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			return;
		}

		// RNGリスト削除、チェックOFF
		m_RngList.DeleteAllItems();
		m_RngChkBox = FALSE;
		// CMDリスト削除、チェックOFF
		m_CmdList.DeleteAllItems();
		m_CmdChkBox = FALSE;
	}

	m_UpLinkList.DeleteItem(nSelected);

	if (m_UpLinkList.GetItemCount() == 0)
	{
		m_UpLinkChkBox = FALSE;
		UpdateData(FALSE);
	}

	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# RNG開始終了日時 追加ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmRngAddBtn()
{
	COperationPlanTimeSet dlg;
	CTime		cTime = 0;
	CTime		cSTime = 0;
	CTime		cETime = 0;
	CString		strStartDT, strEndDT;
	CString		strKind;

	// アップリンクが設定されていない場合、設定不可
	if (m_UpLinkList.GetItemCount() <= 0)
	{
		AfxMessageBox(_T("アップリンクが未設定の為、RNGの設定はできません。"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	int nSelected = m_RngList.GetNextItem(-1, LVNI_SELECTED);

	// リストされている場合、次の開始/終了日時を取得する。
	GetTimeInList(m_RngList, nSelected, cSTime, cETime);

	CTime opeSTime = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	CTime opeETime = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	if (cSTime == 0)
	{
		if (opeSTime > m_tAOSRf)
		{
			cSTime = opeSTime;		// 運用開始日時
		}
		else
		{
			cSTime = m_tAOSRf;		// アンテナ予報値に基づいたAOS側のRF-Trip交差日時
		}
	}
	if (cETime == 0)
	{
		if (opeETime < m_tLOSRf)
		{
			cETime = opeETime;		// 運用終了日時
		}
		else
		{
			cETime = m_tLOSRf;		// アンテナ予報値に基づいたLOS側のRF-Trip交差日時
		}
	}

	while (1)
	{
		dlg.SetDefTime(cSTime, cETime);
		dlg.SetRangeTime(cSTime, cETime);
		dlg.SetParam(COperationPlanTimeSet::TYPE_RNG, m_RngEleList);
		if (dlg.DoModal() == IDCANCEL)
		{
			return;
		}

		CTime sd = dlg.m_StartDay;
		CTime st = dlg.m_StartTime;
		CTime ed = dlg.m_EndDay;
		CTime et = dlg.m_EndTime;
		strKind = dlg.m_OpeTypeKind;

		cTime = CTime(sd.GetYear(), sd.GetMonth(), sd.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
		cSTime = cTime;
		strStartDT = cTime.Format("%Y-%m-%d %H:%M:%S");

		cTime = CTime(ed.GetYear(), ed.GetMonth(), ed.GetDay(), et.GetHour(), et.GetMinute(), et.GetSecond());
		cETime = cTime;
		strEndDT = cTime.Format("%Y-%m-%d %H:%M:%S");


		// 開始/終了日時チェック
		if (IsInPlanDayTime(cSTime, cETime) == FALSE)
		{
			AfxMessageBox(_T("RNG の開始/終了日時が、運用開始/終了日時の範囲外です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			// 重複チェック
			if (IsConflictTimeInList(m_RngList, cSTime, cETime) == TRUE)
			{
				AfxMessageBox(_T("RNG の開始/終了日時が重複しています。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				break;
			}
		}
	}

	// 近接チェック
	IsProximityTime(cSTime, cETime, 1, -1);

	LVITEM       lvi;
	CString      str;

	int count = m_RngList.GetItemCount();

	if (nSelected != -1 && nSelected < count - 1)
	{
		count = nSelected + 1;
	}

	// 開始日時
	lvi.lParam = (LPARAM)count;
	lvi.iItem = count;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = strStartDT.GetBuffer();
	m_RngList.InsertItem(&lvi);

	// 終了日時
	lvi.lParam = (LPARAM)count;
	lvi.iItem = count;
	lvi.iSubItem = 1;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = strEndDT.GetBuffer();
	m_RngList.SetItem(&lvi);

	// 種別
	lvi.lParam = (LPARAM)count;
	lvi.iItem = count;
	lvi.iSubItem = 2;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = strKind.GetBuffer();
	m_RngList.SetItem(&lvi);

	m_RngChkBox = TRUE;
	UpdateData(FALSE);

	m_tOpeS = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	m_tOpeE = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# RND開始終了日時 編集ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmRngEditBtn()
{
	int nSelected = m_RngList.GetNextItem(-1, LVNI_SELECTED);
	if (nSelected < 0)
	{
		return;
	}

	COleDateTime oleSTime, oleETime;
	CString strStartDT, strEndDT;
	CTime cSTime, cETime;
	COperationPlanTimeSet dlg;
	CString strKind;

	CTime opeSTime = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	CTime opeETime = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	CTime rangStime, rangEtime;
	if (opeSTime > m_tAOSRf)
	{
		rangStime = opeSTime;		// 運用開始日時
	}
	else
	{
		rangStime = m_tAOSRf;		// アンテナ予報値に基づいたAOS側のRF-Trip交差日時
	}

	if (opeETime < m_tLOSRf)
	{
		rangEtime = opeETime;		// 運用終了日時
	}
	else
	{
		rangEtime = m_tLOSRf;		// アンテナ予報値に基づいたLOS側のRF-Trip交差日時
	}

	while (1)
	{
		strStartDT = m_RngList.GetItemText(nSelected, 0);
		strEndDT = m_RngList.GetItemText(nSelected, 1);
		strKind = m_RngList.GetItemText(nSelected, 2);

		oleSTime.ParseDateTime(strStartDT);
		oleETime.ParseDateTime(strEndDT);

		cSTime = CTime(oleSTime.GetYear(), oleSTime.GetMonth(), oleSTime.GetDay(), oleSTime.GetHour(), oleSTime.GetMinute(), oleSTime.GetSecond());
		cETime = CTime(oleETime.GetYear(), oleETime.GetMonth(), oleETime.GetDay(), oleETime.GetHour(), oleETime.GetMinute(), oleETime.GetSecond());

		dlg.SetDefTime(cSTime, cETime);
		dlg.SetRangeTime(rangStime, rangEtime);
		dlg.SetParam(COperationPlanTimeSet::TYPE_RNG, m_RngEleList, strKind);

		if (dlg.DoModal() == IDCANCEL)
		{
			return;
		}

		CTime sd = dlg.m_StartDay;
		CTime st = dlg.m_StartTime;
		CTime ed = dlg.m_EndDay;
		CTime et = dlg.m_EndTime;

		cSTime = CTime(sd.GetYear(), sd.GetMonth(), sd.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
		strStartDT = cSTime.Format("%Y-%m-%d %H:%M:%S");

		cETime = CTime(ed.GetYear(), ed.GetMonth(), ed.GetDay(), et.GetHour(), et.GetMinute(), et.GetSecond());
		strEndDT = cETime.Format("%Y-%m-%d %H:%M:%S");

		strKind = dlg.m_OpeTypeKind;

		// 開始/終了日時チェック
		if (IsInPlanDayTime(cSTime, cETime) == FALSE)
		{
			AfxMessageBox(_T("RNG の開始/終了日時が、運用開始/終了日時の範囲外です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			// 重複チェック
			if (IsConflictTimeInList(m_RngList, cSTime, cETime, nSelected) == TRUE)
			{
				AfxMessageBox(_T("RNG の開始/終了日時が重複しています。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				break;
			}
		}
	}

	// 近接チェック
	IsProximityTime(cSTime, cETime, 1, nSelected);

	m_RngList.SetItemText(nSelected, 0, strStartDT);
	m_RngList.SetItemText(nSelected, 1, strEndDT);
	m_RngList.SetItemText(nSelected, 2, strKind);

	m_OperationGraph.Invalidate();
}

/*============================================================================*/
/*! COperationPlan

-# RND開始終了日時 削除ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmRngDelBtn()
{
	int nSelected = m_RngList.GetNextItem(-1, LVNI_SELECTED);
	if (nSelected < 0)
	{
		return;
	}

	m_RngList.DeleteItem(nSelected);

	if (m_RngList.GetItemCount() == 0)
	{
		m_RngChkBox = FALSE;
		UpdateData(FALSE);
	}

	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# CMD開始終了日時 追加ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmCmdAddBtn()
{
	COperationPlanTimeSet dlg;
	CTime      cTime = 0;
	CTime      cSTime = 0;
	CTime      cETime = 0;
	CString    strStartDT, strEndDT;

	// アップリンクが設定されていない場合、設定不可
	if (m_UpLinkList.GetItemCount() <= 0)
	{
		AfxMessageBox(_T("アップリンクが未設定の為、CMDの設定はできません。"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	int nSelected = m_CmdList.GetNextItem(-1, LVNI_SELECTED);

	// リストされている場合、次の開始/終了日時を取得する。
	GetTimeInList(m_CmdList, nSelected, cSTime, cETime);

	CTime opeSTime = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	CTime opeETime = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	if (cSTime == 0)
	{
		if (opeSTime > m_tAOSRf)
		{
			cSTime = opeSTime;		// 運用開始日時
		}
		else
		{
			cSTime = m_tAOSRf;		// アンテナ予報値に基づいたAOS側のRF-Trip交差日時
		}
	}
	if (cETime == 0)
	{
		if (opeETime < m_tLOSRf)
		{
			cETime = opeETime;		// 運用終了日時
		}
		else
		{
			cETime = m_tLOSRf;		// アンテナ予報値に基づいたLOS側のRF-Trip交差日時
		}
	}

	while (1)
	{
		dlg.SetDefTime(cSTime, cETime);
		dlg.SetRangeTime(cSTime, cETime);
		if (dlg.DoModal() == IDCANCEL)
		{
			return;
		}

		CTime sd = dlg.m_StartDay;
		CTime st = dlg.m_StartTime;
		CTime ed = dlg.m_EndDay;
		CTime et = dlg.m_EndTime;

		cTime = CTime(sd.GetYear(), sd.GetMonth(), sd.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
		strStartDT = cTime.Format("%Y-%m-%d %H:%M:%S");
		cSTime = cTime;

		cTime = CTime(ed.GetYear(), ed.GetMonth(), ed.GetDay(), et.GetHour(), et.GetMinute(), et.GetSecond());
		strEndDT = cTime.Format("%Y-%m-%d %H:%M:%S");
		cETime = cTime;

		// 開始/終了日時チェック
		if (IsInPlanDayTime(cSTime, cETime) == FALSE)
		{
			AfxMessageBox(_T("CMD の開始/終了日時が、運用開始/終了日時の範囲外です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			// 重複チェック
			if (IsConflictTimeInList(m_CmdList, cSTime, cETime) == TRUE)
			{
				AfxMessageBox(_T("CMD の開始/終了日時が重複しています。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				break;
			}
		}
	}

	// 近接チェック
	IsProximityTime(cSTime, cETime, 2, -1);

	LVITEM       lvi;
	CString      str;

	int count = m_CmdList.GetItemCount();

	if (nSelected != -1 && nSelected < count - 1)
	{
		count = nSelected + 1;
	}

	// 開始日時
	lvi.lParam = (LPARAM)count;
	lvi.iItem = count;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = strStartDT.GetBuffer();
	m_CmdList.InsertItem(&lvi);

	// 終了日時
	lvi.lParam = (LPARAM)count;
	lvi.iItem = count;
	lvi.iSubItem = 1;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = strEndDT.GetBuffer();
	m_CmdList.SetItem(&lvi);

	m_CmdChkBox = TRUE;
	UpdateData(FALSE);

	m_tOpeS = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	m_tOpeE = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# CMD開始終了日時 編集ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmCmdEditBtn()
{
	int nSelected = m_CmdList.GetNextItem(-1, LVNI_SELECTED);
	if (nSelected < 0)
	{
		return;
	}

	COleDateTime oleSTime, oleETime;
	CTime cSTime, cETime;
	CString strStartDT, strEndDT;
	COperationPlanTimeSet dlg;

	CTime opeSTime = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	CTime opeETime = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	CTime rangStime, rangEtime;
	if (opeSTime > m_tAOSRf)
	{
		rangStime = opeSTime;		// 運用開始日時
	}
	else
	{
		rangStime = m_tAOSRf;		// アンテナ予報値に基づいたAOS側のRF-Trip交差日時
	}

	if (opeETime < m_tLOSRf)
	{
		rangEtime = opeETime;		// 運用終了日時
	}
	else
	{
		rangEtime = m_tLOSRf;		// アンテナ予報値に基づいたLOS側のRF-Trip交差日時
	}

	while (1)
	{
		strStartDT = m_CmdList.GetItemText(nSelected, 0);
		strEndDT = m_CmdList.GetItemText(nSelected, 1);

		oleSTime.ParseDateTime(strStartDT);
		oleETime.ParseDateTime(strEndDT);

		cSTime = CTime(oleSTime.GetYear(), oleSTime.GetMonth(), oleSTime.GetDay(), oleSTime.GetHour(), oleSTime.GetMinute(), oleSTime.GetSecond());
		cETime = CTime(oleETime.GetYear(), oleETime.GetMonth(), oleETime.GetDay(), oleETime.GetHour(), oleETime.GetMinute(), oleETime.GetSecond());

		dlg.SetDefTime(cSTime, cETime);
		dlg.SetRangeTime(rangStime, rangEtime);

		if (dlg.DoModal() == IDCANCEL)
		{
			return;
		}

		CTime sd = dlg.m_StartDay;
		CTime st = dlg.m_StartTime;
		CTime ed = dlg.m_EndDay;
		CTime et = dlg.m_EndTime;

		cSTime = CTime(sd.GetYear(), sd.GetMonth(), sd.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
		strStartDT = cSTime.Format("%Y-%m-%d %H:%M:%S");

		cETime = CTime(ed.GetYear(), ed.GetMonth(), ed.GetDay(), et.GetHour(), et.GetMinute(), et.GetSecond());
		strEndDT = cETime.Format("%Y-%m-%d %H:%M:%S");

		// 開始/終了日時チェック
		if (IsInPlanDayTime(cSTime, cETime) == FALSE)
		{
			AfxMessageBox(_T("CMD の開始/終了日時が、運用開始/終了日時の範囲外です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			// 重複チェック
			if (IsConflictTimeInList(m_CmdList, cSTime, cETime, nSelected) == TRUE)
			{
				AfxMessageBox(_T("CMD の開始/終了日時が重複しています。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				break;
			}
		}
	}

	// 近接チェック
	IsProximityTime(cSTime, cETime, 2, nSelected);

	m_CmdList.SetItemText(nSelected, 0, strStartDT);
	m_CmdList.SetItemText(nSelected, 1, strEndDT);

	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# CMD開始終了日時 削除ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmCmdDelBtn()
{
	int nSelected = m_CmdList.GetNextItem(-1, LVNI_SELECTED);
	if (nSelected < 0)
	{
		return;
	}

	m_CmdList.DeleteItem(nSelected);

	if (m_CmdList.GetItemCount() == 0)
	{
		m_CmdChkBox = FALSE;
		UpdateData(FALSE);
	}

	m_OperationGraph.Invalidate();
}

/*============================================================================*/
/*! COperationPlan

-# TLM S 開始終了日時 追加ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmTlmsAddBtn()
{
	COperationPlanTimeSet dlg;
	CTime      cTime = 0;
	CTime      cSTime = 0;
	CTime      cETime = 0;
	CString    strStartDT, strEndDT;

	int nSelected = m_TlmSList.GetNextItem(-1, LVNI_SELECTED);

	// リストされている場合、次の開始/終了日時を取得する。
	GetTimeInList(m_TlmSList, nSelected, cSTime, cETime);

	CTime opeSTime = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	CTime opeETime = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	if (cSTime == 0)
	{
		if (opeSTime > m_tAOSTLM)
		{
			cSTime = opeSTime;		// 運用開始日時
		}
		else
		{
			cSTime = m_tAOSTLM;		// アンテナ予報値に基づいたAOSスカイライン交差日時
		}
	}
	if (cETime == 0)
	{
		if (opeETime < m_tLOSTLM)
		{
			cETime = opeETime;		// 運用終了日時
		}
		else
		{
			cETime = m_tLOSTLM;		// アンテナ予報値に基づいたLOSスカイライン交差日時
		}
	}

	while (1)
	{
		dlg.SetDefTime(cSTime, cETime);
		dlg.SetRangeTime(cSTime, cETime);
		if (dlg.DoModal() == IDCANCEL)
		{
			return;
		}

		CTime sd = dlg.m_StartDay;
		CTime st = dlg.m_StartTime;
		CTime ed = dlg.m_EndDay;
		CTime et = dlg.m_EndTime;

		cTime = CTime(sd.GetYear(), sd.GetMonth(), sd.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
		strStartDT = cTime.Format("%Y-%m-%d %H:%M:%S");
		cSTime = cTime;

		cTime = CTime(ed.GetYear(), ed.GetMonth(), ed.GetDay(), et.GetHour(), et.GetMinute(), et.GetSecond());
		strEndDT = cTime.Format("%Y-%m-%d %H:%M:%S");
		cETime = cTime;

		// 開始/終了日時チェック
		if (IsInPlanDayTime(cSTime, cETime) == FALSE)
		{
			AfxMessageBox(_T("TLM S の開始/終了日時が、運用開始/終了日時の範囲外です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			// 重複チェック
			if (IsConflictTimeInList(m_TlmSList, cSTime, cETime) == TRUE)
			{
				AfxMessageBox(_T("TLM S の開始/終了日時が重複しています。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				break;
			}
		}
	}

	LVITEM       lvi;
	CString      str;

	int count = m_TlmSList.GetItemCount();

	if (nSelected != -1 && nSelected < count - 1)
	{
		count = nSelected + 1;
	}

	// 開始日時
	lvi.lParam = (LPARAM)count;
	lvi.iItem = count;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = strStartDT.GetBuffer();
	m_TlmSList.InsertItem(&lvi);

	// 終了日時
	lvi.lParam = (LPARAM)count;
	lvi.iItem = count;
	lvi.iSubItem = 1;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = strEndDT.GetBuffer();
	m_TlmSList.SetItem(&lvi);

	m_TLMSChkBox = TRUE;
	UpdateData(FALSE);

	m_tOpeS = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	m_tOpeE = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# TLM S 開始終了日時 編集ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmTlmsEditBtn()
{
	int nSelected = m_TlmSList.GetNextItem(-1, LVNI_SELECTED);
	if (nSelected < 0)
	{
		return;
	}

	COleDateTime oleSTime, oleETime;
	CTime cSTime, cETime;
	CString strStartDT, strEndDT;
	COperationPlanTimeSet dlg;

	CTime opeSTime = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	CTime opeETime = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	CTime rangStime, rangEtime;
	if (opeSTime > m_tAOSTLM)
	{
		rangStime = opeSTime;		// 運用開始日時
	}
	else
	{
		rangStime = m_tAOSTLM;		// アンテナ予報値に基づいたAOSスカイライン交差日時
	}

	if (opeETime < m_tLOSTLM)
	{
		rangEtime = opeETime;		// 運用終了日時
	}
	else
	{
		rangEtime = m_tLOSTLM;		// アンテナ予報値に基づいたLOSスカイライン交差日時
	}

	while (1)
	{
		strStartDT = m_TlmSList.GetItemText(nSelected, 0);
		strEndDT = m_TlmSList.GetItemText(nSelected, 1);

		oleSTime.ParseDateTime(strStartDT);
		oleETime.ParseDateTime(strEndDT);

		cSTime = CTime(oleSTime.GetYear(), oleSTime.GetMonth(), oleSTime.GetDay(), oleSTime.GetHour(), oleSTime.GetMinute(), oleSTime.GetSecond());
		cETime = CTime(oleETime.GetYear(), oleETime.GetMonth(), oleETime.GetDay(), oleETime.GetHour(), oleETime.GetMinute(), oleETime.GetSecond());

		dlg.SetDefTime(cSTime, cETime);
		dlg.SetRangeTime(rangStime, rangEtime);

		if (dlg.DoModal() == IDCANCEL)
		{
			return;
		}

		CTime sd = dlg.m_StartDay;
		CTime st = dlg.m_StartTime;
		CTime ed = dlg.m_EndDay;
		CTime et = dlg.m_EndTime;

		cSTime = CTime(sd.GetYear(), sd.GetMonth(), sd.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
		strStartDT = cSTime.Format("%Y-%m-%d %H:%M:%S");

		cETime = CTime(ed.GetYear(), ed.GetMonth(), ed.GetDay(), et.GetHour(), et.GetMinute(), et.GetSecond());
		strEndDT = cETime.Format("%Y-%m-%d %H:%M:%S");

		// 開始/終了日時チェック
		if (IsInPlanDayTime(cSTime, cETime) == FALSE)
		{
			AfxMessageBox(_T("TLM S の開始/終了日時が、運用開始/終了日時の範囲外です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			// 重複チェック
			if (IsConflictTimeInList(m_TlmSList, cSTime, cETime, nSelected) == TRUE)
			{
				AfxMessageBox(_T("TLM S の開始/終了日時が重複しています。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				break;
			}
		}
	}

	m_TlmSList.SetItemText(nSelected, 0, strStartDT);
	m_TlmSList.SetItemText(nSelected, 1, strEndDT);

	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# TLM S 開始終了日時 削除ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmTlmsDelBtn()
{
	int nSelected = m_TlmSList.GetNextItem(-1, LVNI_SELECTED);
	if (nSelected < 0)
	{
		return;
	}

	m_TlmSList.DeleteItem(nSelected);

	if (m_TlmSList.GetItemCount() == 0)
	{
		m_TLMSChkBox = FALSE;
		UpdateData(FALSE);
	}

	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# TLM X 開始終了日時 追加ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmTlmxAddBtn()
{
	COperationPlanTimeSet dlg;
	CTime      cTime = 0;
	CTime      cSTime = 0;
	CTime      cETime = 0;
	CString    strStartDT, strEndDT;

	int nSelected = m_TlmXList.GetNextItem(-1, LVNI_SELECTED);

	// リストされている場合、次の開始/終了日時を取得する。
	GetTimeInList(m_TlmXList, nSelected, cSTime, cETime);

	CTime opeSTime = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	CTime opeETime = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	if (cSTime == 0)
	{
		if (opeSTime > m_tAOSTLM)
		{
			cSTime = opeSTime;		// 運用開始日時
		}
		else
		{
			cSTime = m_tAOSTLM;		// アンテナ予報値に基づいたAOSスカイライン交差日時
		}
	}
	if (cETime == 0)
	{
		if (opeETime < m_tLOSTLM)
		{
			cETime = opeETime;		// 運用終了日時
		}
		else
		{
			cETime = m_tLOSTLM;		// アンテナ予報値に基づいたLOSスカイライン交差日時
		}
	}

	while (1)
	{
		dlg.SetDefTime(cSTime, cETime);
		dlg.SetRangeTime(cSTime, cETime);
		if (dlg.DoModal() == IDCANCEL)
		{
			return;
		}

		CTime sd = dlg.m_StartDay;
		CTime st = dlg.m_StartTime;
		CTime ed = dlg.m_EndDay;
		CTime et = dlg.m_EndTime;

		cTime = CTime(sd.GetYear(), sd.GetMonth(), sd.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
		strStartDT = cTime.Format("%Y-%m-%d %H:%M:%S");
		cSTime = cTime;

		cTime = CTime(ed.GetYear(), ed.GetMonth(), ed.GetDay(), et.GetHour(), et.GetMinute(), et.GetSecond());
		strEndDT = cTime.Format("%Y-%m-%d %H:%M:%S");
		cETime = cTime;

		// 開始/終了日時チェック
		if (IsInPlanDayTime(cSTime, cETime) == FALSE)
		{
			AfxMessageBox(_T("TLM X の開始/終了日時が、運用開始/終了日時の範囲外です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			// 重複チェック
			if (IsConflictTimeInList(m_TlmXList, cSTime, cETime) == TRUE)
			{
				AfxMessageBox(_T("TLM X の開始/終了日時が重複しています。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				break;
			}
		}
	}

	LVITEM       lvi;
	CString      str;

	int count = m_TlmXList.GetItemCount();

	if (nSelected != -1 && nSelected < count - 1)
	{
		count = nSelected + 1;
	}

	// 開始日時
	lvi.lParam = (LPARAM)count;
	lvi.iItem = count;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = strStartDT.GetBuffer();
	m_TlmXList.InsertItem(&lvi);

	// 終了日時
	lvi.lParam = (LPARAM)count;
	lvi.iItem = count;
	lvi.iSubItem = 1;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = strEndDT.GetBuffer();
	m_TlmXList.SetItem(&lvi);

	m_TLMXChkBox = TRUE;
	UpdateData(FALSE);

	m_tOpeS = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	m_tOpeE = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# TLM X 開始終了日時 編集ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmTlmxEditBtn()
{
	int nSelected = m_TlmXList.GetNextItem(-1, LVNI_SELECTED);
	if (nSelected < 0)
	{
		return;
	}

	COleDateTime oleSTime, oleETime;
	CTime cSTime, cETime;
	CString strStartDT, strEndDT;
	COperationPlanTimeSet dlg;

	CTime opeSTime = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	CTime opeETime = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	CTime rangStime, rangEtime;
	if (opeSTime > m_tAOSTLM)
	{
		rangStime = opeSTime;		// 運用開始日時
	}
	else
	{
		rangStime = m_tAOSTLM;		// アンテナ予報値に基づいたAOSスカイライン交差日時
	}

	if (opeETime < m_tLOSTLM)
	{
		rangEtime = opeETime;		// 運用終了日時
	}
	else
	{
		rangEtime = m_tLOSTLM;		// アンテナ予報値に基づいたLOSスカイライン交差日時
	}

	while (1)
	{
		strStartDT = m_TlmXList.GetItemText(nSelected, 0);
		strEndDT = m_TlmXList.GetItemText(nSelected, 1);

		oleSTime.ParseDateTime(strStartDT);
		oleETime.ParseDateTime(strEndDT);

		cSTime = CTime(oleSTime.GetYear(), oleSTime.GetMonth(), oleSTime.GetDay(), oleSTime.GetHour(), oleSTime.GetMinute(), oleSTime.GetSecond());
		cETime = CTime(oleETime.GetYear(), oleETime.GetMonth(), oleETime.GetDay(), oleETime.GetHour(), oleETime.GetMinute(), oleETime.GetSecond());

		dlg.SetDefTime(cSTime, cETime);
		dlg.SetRangeTime(rangStime, rangEtime);

		if (dlg.DoModal() == IDCANCEL)
		{
			return;
		}

		CTime sd = dlg.m_StartDay;
		CTime st = dlg.m_StartTime;
		CTime ed = dlg.m_EndDay;
		CTime et = dlg.m_EndTime;

		cSTime = CTime(sd.GetYear(), sd.GetMonth(), sd.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
		strStartDT = cSTime.Format("%Y-%m-%d %H:%M:%S");

		cETime = CTime(ed.GetYear(), ed.GetMonth(), ed.GetDay(), et.GetHour(), et.GetMinute(), et.GetSecond());
		strEndDT = cETime.Format("%Y-%m-%d %H:%M:%S");

		// 開始/終了日時チェック
		if (IsInPlanDayTime(cSTime, cETime) == FALSE)
		{
			AfxMessageBox(_T("TLM X の開始/終了日時が、運用開始/終了日時の範囲外です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			// 重複チェック
			if (IsConflictTimeInList(m_TlmXList, cSTime, cETime, nSelected) == TRUE)
			{
				AfxMessageBox(_T("TLM X の開始/終了日時が重複しています。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				break;
			}
		}
	}

	m_TlmXList.SetItemText(nSelected, 0, strStartDT);
	m_TlmXList.SetItemText(nSelected, 1, strEndDT);

	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# TLM X 開始終了日時 削除ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmTlmxDelBtn()
{
	int nSelected = m_TlmXList.GetNextItem(-1, LVNI_SELECTED);
	if (nSelected < 0)
	{
		return;
	}

	m_TlmXList.DeleteItem(nSelected);

	if (m_TlmXList.GetItemCount() == 0)
	{
		m_TLMXChkBox = FALSE;
		UpdateData(FALSE);
	}

	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# TLM Ka 開始終了日時 追加ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmTlmkaAddBtn()
{
	COperationPlanTimeSet dlg;
	CTime      cTime = 0;
	CTime      cSTime = 0;
	CTime      cETime = 0;
	CString    strStartDT, strEndDT;

	int nSelected = m_TlmKaList.GetNextItem(-1, LVNI_SELECTED);

	// リストされている場合、次の開始/終了日時を取得する。
	GetTimeInList(m_TlmKaList, nSelected, cSTime, cETime);

	CTime opeSTime = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	CTime opeETime = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	if (cSTime == 0)
	{
		if (opeSTime > m_tAOSTLM)
		{
			cSTime = opeSTime;		// 運用開始日時
		}
		else
		{
			cSTime = m_tAOSTLM;		// アンテナ予報値に基づいたAOSスカイライン交差日時
		}
	}
	if (cETime == 0)
	{
		if (opeETime < m_tLOSTLM)
		{
			cETime = opeETime;		// 運用終了日時
		}
		else
		{
			cETime = m_tLOSTLM;		// アンテナ予報値に基づいたLOSスカイライン交差日時
		}
	}

	while (1)
	{
		dlg.SetDefTime(cSTime, cETime);
		dlg.SetRangeTime(cSTime, cETime);
		if (dlg.DoModal() == IDCANCEL)
		{
			return;
		}

		CTime sd = dlg.m_StartDay;
		CTime st = dlg.m_StartTime;
		CTime ed = dlg.m_EndDay;
		CTime et = dlg.m_EndTime;

		cTime = CTime(sd.GetYear(), sd.GetMonth(), sd.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
		strStartDT = cTime.Format("%Y-%m-%d %H:%M:%S");
		cSTime = cTime;

		cTime = CTime(ed.GetYear(), ed.GetMonth(), ed.GetDay(), et.GetHour(), et.GetMinute(), et.GetSecond());
		strEndDT = cTime.Format("%Y-%m-%d %H:%M:%S");
		cETime = cTime;

		// 開始/終了日時チェック
		if (IsInPlanDayTime(cSTime, cETime) == FALSE)
		{
			AfxMessageBox(_T("TLM Ka の開始/終了日時が、運用開始/終了日時の範囲外です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			// 重複チェック
			if (IsConflictTimeInList(m_TlmKaList, cSTime, cETime) == TRUE)
			{
				AfxMessageBox(_T("TLM Ka の開始/終了日時が重複しています。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				break;
			}
		}
	}

	LVITEM       lvi;
	CString      str;

	int count = m_TlmKaList.GetItemCount();

	if (nSelected != -1 && nSelected < count - 1)
	{
		count = nSelected + 1;
	}

	// 開始日時
	lvi.lParam = (LPARAM)count;
	lvi.iItem = count;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = strStartDT.GetBuffer();
	m_TlmKaList.InsertItem(&lvi);

	// 終了日時
	lvi.lParam = (LPARAM)count;
	lvi.iItem = count;
	lvi.iSubItem = 1;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = strEndDT.GetBuffer();
	m_TlmKaList.SetItem(&lvi);

	m_TLMKaChkBox = TRUE;
	UpdateData(FALSE);

	m_tOpeS = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	m_tOpeE = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# TLM Ka 開始終了日時 編集ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmTlmkaEditBtn()
{
	int nSelected = m_TlmKaList.GetNextItem(-1, LVNI_SELECTED);
	if (nSelected < 0)
	{
		return;
	}

	COleDateTime oleSTime, oleETime;
	CTime cSTime, cETime;
	CString strStartDT, strEndDT;
	COperationPlanTimeSet dlg;

	CTime opeSTime = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	CTime opeETime = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;

	CTime rangStime, rangEtime;
	if (opeSTime > m_tAOSTLM)
	{
		rangStime = opeSTime;		// 運用開始日時
	}
	else
	{
		rangStime = m_tAOSTLM;		// アンテナ予報値に基づいたAOSスカイライン交差日時
	}

	if (opeETime < m_tLOSTLM)
	{
		rangEtime = opeETime;		// 運用終了日時
	}
	else
	{
		rangEtime = m_tLOSTLM;		// アンテナ予報値に基づいたLOSスカイライン交差日時
	}

	while (1)
	{
		strStartDT = m_TlmKaList.GetItemText(nSelected, 0);
		strEndDT = m_TlmKaList.GetItemText(nSelected, 1);

		oleSTime.ParseDateTime(strStartDT);
		oleETime.ParseDateTime(strEndDT);

		cSTime = CTime(oleSTime.GetYear(), oleSTime.GetMonth(), oleSTime.GetDay(), oleSTime.GetHour(), oleSTime.GetMinute(), oleSTime.GetSecond());
		cETime = CTime(oleETime.GetYear(), oleETime.GetMonth(), oleETime.GetDay(), oleETime.GetHour(), oleETime.GetMinute(), oleETime.GetSecond());

		dlg.SetDefTime(cSTime, cETime);
		dlg.SetRangeTime(rangStime, rangEtime);

		if (dlg.DoModal() == IDCANCEL)
		{
			return;
		}

		CTime sd = dlg.m_StartDay;
		CTime st = dlg.m_StartTime;
		CTime ed = dlg.m_EndDay;
		CTime et = dlg.m_EndTime;

		cSTime = CTime(sd.GetYear(), sd.GetMonth(), sd.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
		strStartDT = cSTime.Format("%Y-%m-%d %H:%M:%S");

		cETime = CTime(ed.GetYear(), ed.GetMonth(), ed.GetDay(), et.GetHour(), et.GetMinute(), et.GetSecond());
		strEndDT = cETime.Format("%Y-%m-%d %H:%M:%S");

		// 開始/終了日時チェック
		if (IsInPlanDayTime(cSTime, cETime) == FALSE)
		{
			AfxMessageBox(_T("TLM Ka の開始/終了日時が、運用開始/終了日時の範囲外です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			// 重複チェック
			if (IsConflictTimeInList(m_TlmKaList, cSTime, cETime, nSelected) == TRUE)
			{
				AfxMessageBox(_T("TLM Ka の開始/終了日時が重複しています。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				break;
			}
		}
	}

	m_TlmKaList.SetItemText(nSelected, 0, strStartDT);
	m_TlmKaList.SetItemText(nSelected, 1, strEndDT);

	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# TLM Ka 開始終了日時 削除ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmTlmkaDelBtn()
{
	int nSelected = m_TlmKaList.GetNextItem(-1, LVNI_SELECTED);
	if (nSelected < 0)
	{
		return;
	}

	m_TlmKaList.DeleteItem(nSelected);

	if (m_TlmKaList.GetItemCount() == 0)
	{
		m_TLMKaChkBox = FALSE;
		UpdateData(FALSE);
	}

	m_OperationGraph.Invalidate();
}




/*============================================================================*/
/*! COperationPlan

-# 詳細表示ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnDetailBtn()
{
	int ret = -1;

	// 計画ファイル作成
	if ((ret = CreatePlanFile(FALSE)) == -1)
	{
		AfxMessageBox(_T("計画ファイル作成に失敗しました。"), MB_OK | MB_ICONWARNING);
		return;
	}

// ADD 2017.09.16 >>>
	if (ret == 1)
	{
		// ガードタイム検出
		return;
	}
// ADD 2017.09.16 <<<

	// 計画ファイルパスの取得
	CString strFilePath = GetFilePath();

	// 計画詳細表示 
	m_DetailDlg.SetTitle(_T("計画詳細表示"));

	if (!strFilePath.IsEmpty())
	{
		// 詳細ビュー表示
		m_DetailDlg.SetSelectFileName(strFilePath);
		if (m_DetailDlg.GetSafeHwnd() != NULL)
		{
			m_DetailDlg.DestroyWindow();
		}
		m_DetailDlg.Create(IDD_DIALOG_DETAILVIEW, this);
		m_DetailDlg.ShowWindow(SW_SHOW);

		// ファイル削除
		DeleteFile(strFilePath);
	}
}


/*============================================================================*/
/*! COperationPlan

-# 詳細印刷ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnPrintBtn()
{
	int ret = -1;

	// 計画ファイル作成
	if ((ret = CreatePlanFile(FALSE)) == -1)
	{
		AfxMessageBox(_T("計画ファイル作成に失敗しました。"), MB_OK | MB_ICONWARNING);
		return;
	}

// ADD 2017.09.16 >>>
	if (ret == 1)
	{
		// ガードタイム検出
		return;
	}
// ADD 2017.09.16 <<<

	// 計画ファイルパスの取得
	CString strFilePath = GetFilePath();

	// 帳票用出力
	CSatelliteData &sd = theApp.GetSatelliteData();
	CString strTmpFile = sd.CreatePlnPrintFile(strFilePath);

#if 1
	// 帳票
	CSatelliteData::ExcelOutPut(m_hWnd, strTmpFile);

#else
//	// 印刷
//	if (!strFilePath.IsEmpty())
//	{
//		if (CPrintUtility::PrintEucFile(strFilePath))
//		{
//			// 印刷したメッセージ表示
//			CString strTitle;
//			CString strMsg, strMsgTmp;
//			if (strTitle.LoadString(IDS_PRINT_TITLE) == FALSE)
//				return;
//			if (strMsgTmp.LoadString(IDS_PRINT_OK_MSG) == FALSE)
//				return;
//			strMsg.Format(strMsgTmp, strFilePath);
//			MessageBox(strMsg, strTitle, MB_OK | MB_ICONINFORMATION);
//		}
//		else
//		{
//			// 印刷できなかったメッセージ表示
//			CString strTitle;
//			CString strMsg, strMsgTmp;
//			if (strTitle.LoadString(IDS_PRINT_TITLE) == FALSE)
//				return;
//			if (strMsgTmp.LoadString(IDS_PRINT_NG_MSG) == FALSE)
//				return;
//			strMsg.Format(strMsgTmp, strFilePath);
//			MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
//		}
//
//		// ファイル削除
//		DeleteFile(strFilePath);
//	}
#endif
}


/*============================================================================*/
/*! COperationPlan

-# レディネス時刻算出

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::CalcRdyTime20()
{
	if (m_bAntStandby)
	{
		// ANT待ち受け指向あり
		m_PlanData.l_ant_time = ANT_TIME20;

		// レディネスチェック開始時刻
		m_PlanData.l_rdy_time = PASS_BASE_TIME20;
	}
	else
	{
		// ANT待ち受け指向なし
		m_PlanData.l_ant_time = ANT_TIME;

		// レディネスチェック開始時刻
		m_PlanData.l_rdy_time = PASS_BASE_TIME20 - (ANT_TIME20 - ANT_TIME);
	}
}

void COperationPlan::CalcRdyTime34()
{
	CSatelliteData &sd = theApp.GetSatelliteData();

	// ANT待ち受け指向
	if (m_bAntStandby)
	{
		m_PlanData.l_ant_time = ANT_TIME34;
	}
	else
	{
		m_PlanData.l_ant_time = ANT_TIME;
	}

	// レディネスチェック開始時刻
#if 1
	// ANT待ち受け指向
	if (m_bAntStandby)
	{
		m_PlanData.l_rdy_time = PASS_BASE_TIME34;
	}
	else
	{
		m_PlanData.l_rdy_time = PASS_BASE_TIME34 - (ANT_TIME34 - ANT_TIME);
	}
#else
//	if (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_S_ID)
//	{
//		if (sd.m_satinfo[m_IdxEqsat].i_bitrate_qpsk > 0)
//		{
//			// ANT待ち受け指向
//			if (m_bAntStandby)
//			{
//				/* SS帯RNG S帯CMD S帯TLM X帯高速TLM */
//				m_PlanData.l_rdy_time = PASS_BASE_TIME34 + BASE_TIME_PLUS34;
//			}
//			else
//			{
//				/* SS帯RNG S帯CMD S帯TLM X帯高速TLM */
//				m_PlanData.l_rdy_time = (PASS_BASE_TIME34 + BASE_TIME_PLUS34) - (ANT_TIME34 - ANT_TIME);
//			}
//		}
//		else
//		{
//			// ANT待ち受け指向
//			if (m_bAntStandby)
//			{
//				/* SS帯RNG S帯CMD S帯TLM */
//				m_PlanData.l_rdy_time = PASS_BASE_TIME34;
//			}
//			else
//			{
//				/* SS帯RNG S帯CMD S帯TLM */
//				m_PlanData.l_rdy_time = PASS_BASE_TIME34 - (ANT_TIME34 - ANT_TIME);
//			}
//		}
//	}
//	else if (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_SX_ID)
//	{
//		// ANT待ち受け指向
//		if (m_bAntStandby)
//		{
//			/* SX帯RNG S帯CMD S帯TLM */
//			m_PlanData.l_rdy_time = PASS_BASE_TIME34 + BASE_TIME_PLUS34;
//		}
//		else
//		{
//			/* SX帯RNG S帯CMD S帯TLM */
//			m_PlanData.l_rdy_time = (PASS_BASE_TIME34 + BASE_TIME_PLUS34) - (ANT_TIME34 - ANT_TIME);
//		}
//	}
//	else if (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_RX_ID)
//	{
//		// ANT待ち受け指向
//		if (m_bAntStandby)
//		{
//			/* XX帯RNG X帯CMD X帯新系TLM */
//			m_PlanData.l_rdy_time = PASS_BASE_TIME34;
//		}
//		else
//		{
//			/* XX帯RNG X帯CMD X帯新系TLM */
//			m_PlanData.l_rdy_time = PASS_BASE_TIME34 - (ANT_TIME34 - ANT_TIME);
//		}
//	}
#endif
}

void COperationPlan::CalcRdyTime64()
{
	int basetime = 0;

	CSatelliteData& sd = theApp.GetSatelliteData();
	m_IdxEqsat = GetIndexEqSatDBData(m_SatelliteName);

	if (m_bAntStandby)
	{
		// ANT待ち受け指向あり
		m_PlanData.l_ant_time = ANT_TIME64;

		// レディネスチェック開始時刻
		m_PlanData.l_rdy_time = PASS_BASE_TIME64;
	}
	else
	{
		// ANT待ち受け指向なし
		m_PlanData.l_ant_time = ANT_TIME;

		// レディネスチェック開始時刻
		m_PlanData.l_rdy_time = PASS_BASE_TIME64 - (ANT_TIME64 - ANT_TIME);
	}
}

/*============================================================================*/
/*! COperationPlan

-# 計画ファイル作成

@param  なし
@retval なし
*/
/*============================================================================*/
int COperationPlan::CreatePlanFile(BOOL bRegist)
{
	int ret = -1;
	UpdateData();

	// 設定データを格納
	if (SetPlanData() == FALSE)
	{
		return -1;
	}

	CString str = _T("");
	CString pid = _T("");

	// 基本パラメータ-計画ID
	m_PlanIDEdit.GetWindowText(str);
	pid = m_PassIDMask + _T("-") + str;

	// 計画ファイル書き込み
	int s = theApp.GetSelectStation();
	if (s == eStation_Usuda64)	// 臼田64m
	{
		// レディネスチェック時刻取得
		CalcRdyTime64();
		ret = WritePlanFile64(m_SatelliteName, pid, m_PlanData, bRegist);
	}
	else if (s == eStation_Usuda642)	// 臼田54m
	{
		// レディネスチェック時刻取得
		CalcRdyTime54();
		ret = WritePlanFile54(m_SatelliteName, pid, m_PlanData, bRegist);
	}
	else if (s == eStation_Uchinoura34)	// 内之浦34m
	{
		// レディネスチェック時刻取得
		CalcRdyTime34();
		ret = WritePlanFile34(m_SatelliteName, pid, m_PlanData, bRegist);
	}
	else if (s == eStation_Uchinoura20)	// 内之浦20m
	{
		// レディネスチェック時刻取得
		CalcRdyTime20();
		ret = WritePlanFile20(m_SatelliteName, pid, m_PlanData, bRegist);
	}

	return ret;
}

/*============================================================================*/
/*! COperationPlan

-# 登録ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnEntryBtn()
{
	int ret = -1;

	UpdateData();

	// 計画ファイル作成
	if ((ret = CreatePlanFile()) == -1)
	{
		AfxMessageBox(_T("計画ファイル作成に失敗しました。"), MB_OK | MB_ICONWARNING);
		return ;
	}

	if (ret == 1)
	{
		// ガードタイム検出
		return;
	}

	// 衛星名
	CString satellite = m_strSatellite;

	// 計画登録要求
	CString str = _T("");
	CString pid = _T("");
	m_PlanIDEdit.GetWindowText(str);
	pid = m_PassIDMask + _T("-") + str;

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = REGIST_PLAN_SH;
	CString arg;

	//	arg.Format(_T("%s %s %s"), mStationSimpleString[theApp.GetSelectStation()], m_strSatellite, pid);
	if (m_bNewPlan == TRUE)
	{
		/* 新規追加 */
		// 計画新規作成要求の送信
		arg.Format(_T("%s new %s sttn %s.pln"), (LPCTSTR)CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower(), (LPCTSTR)satellite.MakeLower(), (LPCTSTR)pid);
	}
	else
	{
		/* 更新 */
		// 計画更新要求の送信
		arg.Format(_T("%s renew %s sttn %s.pln"), (LPCTSTR)CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower(), (LPCTSTR)satellite.MakeLower(), (LPCTSTR)pid);
	}

	stParam.strRemShellParam.Format(arg);

	int err = CSshShell::ExecShell(stParam);

	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("OnBnClickedOpeplnEntryBtn"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err == 0)
	{
		AfxMessageBox(_T("計画登録に成功しました。"), MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		AfxMessageBox(_T("計画登録に失敗しました。"), MB_OK | MB_ICONWARNING);
	}
}



void COperationPlan::OnSize(UINT nType, int cx, int cy)
{
	CDialogBase::OnSize(nType, cx, cy);
}

void COperationPlan::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// 描画メッセージで CDialogEx::OnPaint() を呼び出さないでください。
}


/*============================================================================*/
/*! COperationPlan

-# レディネスチェック-AGC校正ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnRdnsAgcCb()
{
	UpdateData();
	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# レディネスチェック-RNG校正ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnRdnsRngCb()
{
	UpdateData();
	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# レディネスチェック-TLM伝送ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnRdnsTlmCb()
{
	UpdateData();
	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# レディネスチェック-CMD伝送ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnRdnsCmdCb()
{
	UpdateData();
	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# 運用オプション-初期設定コンボ変更処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnCbnSelchangeOpeplnOpeopInitsetCmb()
{
	CString str;

	int idx = m_OpInitCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}

	m_OpInitCmb.GetLBText(idx, str);

	m_bOpInit = FALSE;
	if (str == strAriNasi[ARINASI_ARI])	// 有り
	{
		m_bOpInit = TRUE;
	}

	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# CIDボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBrCidBtn()
{
	BOOL isFindSat = FALSE;

	// 該当衛星のCID有無確認
	map<CString, stCIDPrmEle> &cidEleList = theApp.GetSatelliteData().m_CIDSlctData.GetCidElementList();
	for (auto itrs = cidEleList.begin(); itrs != cidEleList.end(); ++itrs)
	{
		CString sat = itrs->first;
		if (m_SatelliteName.CompareNoCase(sat) == 0)
		{
			isFindSat = TRUE;
			break;
		}
	}

	if (isFindSat == FALSE)
	{
		AfxMessageBox(_T("該当する衛星のCIDファイルがありません。"), MB_OK | MB_ICONEXCLAMATION);
		return ;
	}

	// CID選択画面表示
	CString cid;
	m_CidBtnCtrl.GetWindowTextW(cid);

	CCidSelect dlg;
	dlg.SetSatelliteName(m_SatelliteName);
	dlg.SetCID(cid);
	if (dlg.DoModal() == IDOK)
	{
		stCIDParam st = CCIDSlctData::TrimInvalidString(dlg.GetParameter());

		m_CidBtnCtrl.SetWindowTextW(st.CID);

		// ビットレート-TLM S
		m_strBRTLMS = st.TlmBitRateS;

		// ビットレート-TLM X
		m_strBRTLMX = st.TlmBitRateX;

		// ビットレート-TLM Ka
		m_strBRTLMKa = st.TlmBitRateKa;

		// ビットレート-高速TLM
		m_strBRHTLM = st.TlmBitRateHX;

		// ビットレート-CMD S
		m_strBRCMDS = st.CmdBitRateS;

		// ビットレート-CMD X
		m_strBRCMDX = st.CmdBitRateX;

		// 運用オプション-送信機ドップラ補償
		m_OpTDPStr = st.TransDoppler;

		// 運用オプション-送信出力
		m_TrsPowerStr = st.TrsPower;

		// レンジ計測パラメータ
		m_strRngMesMode = st.RngMesMode;

		// 基本パラメータ-ANT待ち受け指向通知
		m_AntStandbyStr = st.AntAosPosn;
		if (m_AntStandbyStr == strAriNasi[ARINASI_ARI])
		{
			m_bAntStandby = TRUE;
		}
		else
		{
			m_bAntStandby = FALSE;
		}

		// 基本パラメータ-ANT天頂指向有無
#if 1
		if ((st.AntWaitPoint.CompareNoCase(_T("天頂")) == 0) || (st.AntWaitPoint.CompareNoCase(_T("天頂(LOCK)")) == 0))
		{
			m_AntZenithStr = strANTZenith[ANTZENITH_ARI_SWTOW];
			m_bAntZenith = TRUE;
		}
		else if (st.AntWaitPoint.CompareNoCase(_T("天頂(UNLOCK)")) == 0)
		{
			m_AntZenithStr = strANTZenith[ANTZENITH_ARI];
			m_bAntZenith = TRUE;
		}
		else
		{
			m_AntZenithStr = strANTZenith[ANTZENITH_NASI];
			m_bAntZenith = FALSE;
		}
#else
//		m_AntZenithStr = st.AntWaitPoint;
#endif

		UpdateData(FALSE);

		// アンテナ予報値に基づいたAOS側 / LOS側のRF-Trip交差日時を取得する。
		CTime sTime, eTime;
		GetIntersectionDateRFTrip(sTime, eTime);
		m_tAOSRf = sTime;
		m_tLOSRf = eTime;

		m_OperationGraph.Invalidate();
	}
}


/*============================================================================*/
/*! COperationPlan

-# アップリンクチェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmUlCheck()
{
	UpdateData();

	if (m_UpLinkList.GetItemCount() > 0 && m_UpLinkChkBox == FALSE)
	{
		if (IDOK == AfxMessageBoxHooked(_T("登録済みのアップリンク、RNG、CMDの実施項目を全て削除します。\nよろしいですか？"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			// アップリンクリスト削除
			m_UpLinkList.DeleteAllItems();
			// RNGリスト削除、チェックOFF
			m_RngList.DeleteAllItems();
			m_RngChkBox = FALSE;
			// CMDリスト削除、チェックOFF
			m_CmdList.DeleteAllItems();
			m_CmdChkBox = FALSE;
		}
		else
		{
			m_UpLinkChkBox = TRUE;
		}
		UpdateData(FALSE);
	}

	if (m_OpeStartDay != 0)
	{
		m_tOpeS = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	}
	if (m_OpeEndDay != 0)
	{
		m_tOpeE = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;
	}
	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# RNGチェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmRngCheck()
{
	UpdateData();

	if (m_RngList.GetItemCount() > 0 && m_RngChkBox == FALSE)
	{
		if (IDOK == AfxMessageBoxHooked(_T("登録済みのRNG実施項目を全て削除します。\nよろしいですか？"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			m_RngList.DeleteAllItems();
		}
		else
		{
			m_RngChkBox = TRUE;
			UpdateData(FALSE);
		}
	}

	if (m_OpeStartDay != 0)
	{
		m_tOpeS = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	}
	if (m_OpeEndDay != 0)
	{
		m_tOpeE = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;
	}
	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# CMDチェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmCmdCheck()
{
	UpdateData();

	if (m_CmdList.GetItemCount() > 0 && m_CmdChkBox == FALSE)
	{
		if (IDOK == AfxMessageBoxHooked(_T("登録済みのCMD実施項目を全て削除します。\nよろしいですか？"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			m_CmdList.DeleteAllItems();
		}
		else
		{
			m_CmdChkBox = TRUE;
			UpdateData(FALSE);
		}
	}

	if (m_OpeStartDay != 0)
	{
		m_tOpeS = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	}
	if (m_OpeEndDay != 0)
	{
		m_tOpeE = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;
	}
	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# TLM Sチェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmTlmsCheck()
{
	UpdateData();

	if (m_TlmSList.GetItemCount() > 0 && m_TLMSChkBox == FALSE)
	{
		if (IDOK == AfxMessageBoxHooked(_T("登録済みのTLM S実施項目を全て削除します。\nよろしいですか？"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			m_TlmSList.DeleteAllItems();
		}
		else
		{
			m_TLMSChkBox = TRUE;
			UpdateData(FALSE);
		}
	}

	if (m_OpeStartDay != 0)
	{
		m_tOpeS = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	}
	if (m_OpeEndDay != 0)
	{
		m_tOpeE = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;
	}
	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# TLM Xチェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmTlmxCheck()
{
	UpdateData();

	if (m_TlmXList.GetItemCount() > 0 && m_TLMXChkBox == FALSE)
	{
		if (IDOK == AfxMessageBoxHooked(_T("登録済みのTLM X実施項目を全て削除します。\nよろしいですか？"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			m_TlmXList.DeleteAllItems();
		}
		else
		{
			m_TLMXChkBox = TRUE;
			UpdateData(FALSE);
		}
	}

	if (m_OpeStartDay != 0)
	{
		m_tOpeS = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	}
	if (m_OpeEndDay != 0)
	{
		m_tOpeE = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;
	}

	m_OperationGraph.Invalidate();
}

/*============================================================================*/
/*! COperationPlan

-# TLM Kaチェックボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnBnClickedOpeplnBasicprmTlmkaCheck()
{
	UpdateData();

	if (m_TlmKaList.GetItemCount() > 0 && m_TLMKaChkBox == FALSE)
	{
		if (IDOK == AfxMessageBoxHooked(_T("登録済みのTLM Ka実施項目を全て削除します。\nよろしいですか？"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			m_TlmKaList.DeleteAllItems();
		}
		else
		{
			m_TLMKaChkBox = TRUE;
			UpdateData(FALSE);
		}
	}

	if (m_OpeStartDay != 0)
	{
		m_tOpeS = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	}
	if (m_OpeEndDay != 0)
	{
		m_tOpeE = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;
	}

	m_OperationGraph.Invalidate();
}


/*============================================================================*/
/*! COperationPlan

-# 衛星ー計画リスト選択

@brief リストで選択されている計画IDを、基本設定の計画IDへ設定する。

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnLvnItemchangedOpeplnStltList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

#if 0
	int col = pNMLV->iItem;
	m_strPlanID = m_SatelliteList.GetItemText(col, 1);
	UpdateData(FALSE);
#endif

	*pResult = 0;
}


/*============================================================================*/
/*! COperationPlan

-# 衛星-計画リスト上でのダブルクリック処理

@brief 計画ファイルを読み込み表示する。

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::OnNMDblclkOpeplnStltList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	COprPlnData opd;
	BOOL bData = FALSE;
	CString pid;

	// 変更チェック
	if (CheckPlanDataDlg())
	{
		if (IDCANCEL == AfxMessageBoxHooked(_T("設定が変更されています。\n設定を破棄しますか？"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			return;
		}
	}

	int col = pNMItemActivate->iItem;
	CString strSat = m_SatelliteList.GetItemText(col, 0);
	CString strCid = m_SatelliteList.GetItemText(col, 1);

	m_GetForm = (DWORD)m_SatelliteList.GetItemData(col);

	// 計画ファイル確認
	if (GetPlanFile(strSat, strCid, m_GetForm))
	{
		m_bNewPlan = FALSE;
		m_SatelliteName = strSat;
		m_PassID = strCid;

		if (UpdateOperationPlanDlg() == FALSE)
		{
			return;
		}
	}
	else
	{
		AfxMessageBox(_T("計画ファイルが存在しません。"), MB_OK | MB_ICONINFORMATION);
	}

	UpdateData(FALSE);

	m_OperationGraph.Invalidate();

	*pResult = 0;
}

/*============================================================================*/
/*! COperationPlan

-# アンテナ予報値取得

@brief アンテナ予報値のAOS、LOSを取得する

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlan::GetAntFrcTime(CTime &cAos, CTime &cLos)
{
	// ダミー
	// アンテナ予報値AOSを現在時刻、LOSを現在時刻から12間後とする。
	CTime now, after;
	CTimeSpan span(0, 12, 0, 0);	

	now = CTime::GetCurrentTime();
	after = now + span;

	cAos = now;
	cLos = after;

}

/*============================================================================*/
/*! COperationPlan

-# 設定時刻が、運用開始/終了日時の範囲内かを確認する。


@param  なし
@retval FALSE：範囲外/ TRUE：範囲内
*/
/*============================================================================*/
BOOL COperationPlan::IsInPlanDayTime(CTime cSTime, CTime cETime)
{
	UpdateData();

	// 運用開始日時
	CTime OpeStart = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());
	// 運用終了日時
	CTime OpeEnd = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());

	if (((OpeStart <= cSTime) && (OpeEnd >= cSTime))
		&& ((OpeStart <= cETime) && (OpeEnd >= cETime)))
	{
		return TRUE;
	}

	return FALSE;
}

/*============================================================================*/
/*! COperationPlan

-# リストに登録されている日時（開始/終了）が重複していないかをチェックする。


@param  なし
@retval FALSE：重複なし/ TRUE：重複あり
*/
/*============================================================================*/
BOOL COperationPlan::IsConflictTimeInList(CListCtrl& list, CTime cSTime, CTime cETime, int idx/*=-1*/)
{
	int count = list.GetItemCount();
	BOOL bConflict = FALSE;
	for (int i = 0; i < count; ++i)
	{
		if (i == idx)
		{
			continue;
		}

		CString strSTime = list.GetItemText(i, 0);
		CTime cListSTime = CSatelliteData::StrTimeToCTime(strSTime);

		CString strETime = list.GetItemText(i, 1);
		CTime cListETime = CSatelliteData::StrTimeToCTime(strETime);

		if (((cListSTime <= cSTime) && (cListETime > cSTime))
			|| ((cListSTime < cETime) && (cListETime >= cETime)))
		{
			bConflict = TRUE;
			break;
		}
	}
	return bConflict;
}

/*============================================================================*/
/*! COperationPlan

-# リストに登録されている日時（開始/終了）の近接チェック


@param  cStime ：開始時刻
@param  cEtime ：終了時刻
@param  type 　：種別（0:アップリンク、1:RNG、2:CMD）
@param  idx　　：編集時のリストインデックス
@retval FALSE：重複なし/ TRUE：重複あり
*/
/*============================================================================*/
BOOL COperationPlan::IsProximityTime(CTime cSTime, CTime cETime, int type, int idx)
{
	int count = 0;

	// アップリンクリスト
	CString msg = _T("");
	CString msg_uplink = _T("");
	BOOL bHeader = FALSE;
	count = m_UpLinkList.GetItemCount();
	for (int i = 0; i < count; ++i)
	{
		if ( type == 0 && i == idx)
		{
			continue;
		}

		CString strSTime = m_UpLinkList.GetItemText(i, 0);
		CTime cListSTime = CSatelliteData::StrTimeToCTime(strSTime);

		CString strETime = m_UpLinkList.GetItemText(i, 1);
		CTime cListETime = CSatelliteData::StrTimeToCTime(strETime);

		__time64_t lst = cListSTime.GetTime();
		__time64_t let = cListETime.GetTime();
		__time64_t st = cSTime.GetTime();
		__time64_t et = cETime.GetTime();

		// 近接チェック
		__time64_t val1 = lst - et;
		__time64_t val2 = st - let;
		if (((cListSTime > cSTime) && (val1 >= 0 && val1 <= 60)) || ((cListETime < cETime) && (val2 >= 0 && val2 <= 60)))
		{
			if (bHeader == FALSE)
			{
				msg_uplink = _T("□アップリンク\n");
				bHeader = TRUE;
			}
			msg.Format(_T(" 開始日時:%s 終了日時:%s\n"), strSTime, strETime);
			msg_uplink += msg;
		}
	}

	// RNG
	CString msg_rng = _T("");
	bHeader = FALSE;
	count = m_RngList.GetItemCount();
	for (int i = 0; i < count; ++i)
	{
		if (type == 1 && i == idx)
		{
			continue;
		}

		CString strSTime = m_RngList.GetItemText(i, 0);
		CTime cListSTime = CSatelliteData::StrTimeToCTime(strSTime);

		CString strETime = m_RngList.GetItemText(i, 1);
		CTime cListETime = CSatelliteData::StrTimeToCTime(strETime);

		__time64_t lst = cListSTime.GetTime();
		__time64_t let = cListETime.GetTime();
		__time64_t st = cSTime.GetTime();
		__time64_t et = cETime.GetTime();

		// 近接チェック
		__time64_t val1 = lst - et;
		__time64_t val2 = st - let;
		if (((cListSTime > cSTime) && (val1 >= 0 && val1 <= 60)) || ((cListETime < cETime) && (val2 >= 0 && val2 <= 60)))
		{
			if (bHeader == FALSE)
			{
				msg_rng = _T("□RNG\n");
				bHeader = TRUE;
			}
			msg.Format(_T(" 開始日時:%s 終了日時:%s\n"), strSTime, strETime);
			msg_rng += msg;
		}
	}

	// CMD
	CString msg_cmd = _T("");
	bHeader = FALSE;
	count = m_CmdList.GetItemCount();
	for (int i = 0; i < count; ++i)
	{
		if (type == 2 && i == idx)
		{
			continue;
		}

		CString strSTime = m_CmdList.GetItemText(i, 0);
		CTime cListSTime = CSatelliteData::StrTimeToCTime(strSTime);

		CString strETime = m_CmdList.GetItemText(i, 1);
		CTime cListETime = CSatelliteData::StrTimeToCTime(strETime);

		__time64_t lst = cListSTime.GetTime();
		__time64_t let = cListETime.GetTime();
		__time64_t st = cSTime.GetTime();
		__time64_t et = cETime.GetTime();

		// 近接チェック
		__time64_t val1 = lst - et;
		__time64_t val2 = st - let;
		if (((cListSTime > cSTime) && (val1 >= 0 && val1 <= 60)) || ((cListETime < cETime) && (val2 >= 0 && val2 <= 60)))
		{
			if (bHeader == FALSE)
			{
				msg_cmd = _T("□CMD\n");
				bHeader = TRUE;
			}
			msg.Format(_T(" 開始日時:%s 終了日時:%s\n"), strSTime, strETime);
			msg_cmd += msg;
		}
	}
	
	msg = _T("");
	if (msg_uplink.IsEmpty() == FALSE)
	{
		msg += msg_uplink;
	}
	if (msg_rng.IsEmpty() == FALSE)
	{
		msg += msg_rng;
	}
	if (msg_cmd.IsEmpty() == FALSE)
	{
		msg += msg_cmd;
	}

	if (msg.IsEmpty() == FALSE)
	{
		CString title;
		CString info  = _T("");
		GetWindowText(title);

		info = _T("追加、編集しようとした時間帯に、近接する計画が登録されています。\n");
		info += msg;
		MessageBox(info, title, MB_OK | MB_ICONWARNING);
	}

	return TRUE;
}

/*============================================================================*/
/*! COperationPlan

-# リストに登録されている日時より開始・終了日時を求める。


@param  なし
@retval FALSE：異常終了/ TRUE：正常終了
*/
/*============================================================================*/
BOOL COperationPlan::GetTimeInList(CListCtrl& list, int idx, CTime& cSTime, CTime& cETime)
{
	CString strSTime, strETime;
	CTime cListSTime, cListETime;

	int count = list.GetItemCount();

	if (idx == -1)
	{
		if (count >= 1)
		{
			// 開始時刻取得
			strETime = list.GetItemText(count - 1, 1);
			cListETime = CSatelliteData::StrTimeToCTime(strETime);
			cSTime = cListETime;
		}
	}
	else
	{
		// 開始時刻取得
		strETime = list.GetItemText(idx, 1);
		cListETime = CSatelliteData::StrTimeToCTime(strETime);
		cSTime = cListETime;

		if (idx + 1 < count)
		{
			// 終了時刻取得
			strSTime = list.GetItemText(idx + 1, 0);
			cListSTime = CSatelliteData::StrTimeToCTime(strSTime);
			cETime = cListSTime;
		}
	}

	return TRUE;
}

/*============================================================================*/
/*! COperationPlan

-# 計画ID整形

@param  XXXXXXXXXX
@retval XXXXXX-XXXX
*/
/*============================================================================*/
CString COperationPlan::ChgPlanIDFormat(CString str)
{
	CString retStr = _T("");
	retStr.Format(_T("%s-%s"), (LPCTSTR)str.Mid(0, 6), (LPCTSTR)str.Mid(6, 4));
	return retStr;
}

/*============================================================================*/
/*! COperationPlan

-# 設定データを格納クラスに保存する。

@param  なし
@retval TRUE：登録済み/FALSE：新規
*/
/*============================================================================*/
BOOL COperationPlan::SetPlanData()
{
	BOOL ret = TRUE;

	CString str = _T("");
	CString pid = _T("");
	stTime	timese;
	stTimeStr timestr;
	CTime ts, te;
	int count = 0;

	UpdateData();

	CSatelliteData& sd = theApp.GetSatelliteData();
	int s = theApp.GetSelectStation();

	// 基本パラメータ-計画ID
	m_PlanIDEdit.GetWindowTextW(str);
	pid = ChgPlanIDFormat(str);
	m_OprPlnDataOrg.m_strPlanID = pid;

	// 基本パラメータ-運用開始終了日時
	m_tOpeS = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());
	m_tOpeE = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());

	m_OprPlnDataOrg.m_OpetDay.start = m_tOpeS;
	m_OprPlnDataOrg.m_OpetDay.end = m_tOpeE;

	m_PlanData.sz_start = m_tOpeS.Format(_T("%Y-%m-%d %H:%M:%S"));
	m_PlanData.sz_end = m_tOpeE.Format(_T("%Y-%m-%d %H:%M:%S"));


	// 基本パラメータ-アップリンク 有効/無効フラグ
	m_OprPlnDataOrg.m_UpLinkChk = m_UpLinkChkBox;
	if (m_UpLinkChkBox)
	{
		m_PlanData.l_uplink = 1;
	}
	else
	{
		m_PlanData.l_uplink = 0;
	}

	// 基本パラメータ-RNG 設定数
	count = m_UpLinkList.GetItemCount();
	m_OprPlnDataOrg.m_UpLinkCnt = count;
	m_OprPlnDataOrg.m_ULDay.clear();

	// 基本パラメータ-アップリンク 開始終了日時リスト
	for (int i = 0; i < count; ++i)
	{
		CString strSTime = m_UpLinkList.GetItemText(i, 0);
		ts = CSatelliteData::StrTimeToCTime(strSTime);

		CString strETime = m_UpLinkList.GetItemText(i, 1);
		te = CSatelliteData::StrTimeToCTime(strETime);

		timese.start = ts;
		timese.end = te;
		m_OprPlnDataOrg.m_ULDay.push_back(timese);
	}

	// 基本パラメータ-RNG 有効/無効フラグ
	m_OprPlnDataOrg.m_RngChk = m_RngChkBox;
	if (m_RngChkBox)
	{
		m_PlanData.l_rng = 1;
	}
	else
	{
		m_PlanData.l_rng = 0;
	}

	// 基本パラメータ-RNG 設定数
	count = m_RngList.GetItemCount();
	m_OprPlnDataOrg.m_RngCnt = count;
	m_OprPlnDataOrg.m_RngDayList.clear();

	// 基本パラメータ-RNG 開始終了日時リスト
	for (int i = 0; i < count; ++i)
	{
		CString strSTime = m_RngList.GetItemText(i, 0);
		ts = CSatelliteData::StrTimeToCTime(strSTime);

		CString strETime = m_RngList.GetItemText(i, 1);
		te = CSatelliteData::StrTimeToCTime(strETime);

		timese.start = ts;
		timese.end = te;
		m_OprPlnDataOrg.m_RngDayList.push_back(timese);
	}

	// 基本パラメータ-CMD 有効/無効フラグ
	m_OprPlnDataOrg.m_CmdChk = m_CmdChkBox;
	if (m_CmdChkBox)
	{
		m_PlanData.l_cmd = 1;
	}
	else
	{
		m_PlanData.l_cmd = 0;
	}

	// 基本パラメータ-CMD 設定数
	count = m_CmdList.GetItemCount();
	m_OprPlnDataOrg.m_CmdCnt = count;
	m_OprPlnDataOrg.m_CmdDayList.clear();

	// 基本パラメータ-CMD 開始終了日時リスト
	for (int i = 0; i < count; ++i)
	{
		CString strSTime = m_CmdList.GetItemText(i, 0);
		ts = CSatelliteData::StrTimeToCTime(strSTime);

		CString strETime = m_CmdList.GetItemText(i, 1);
		te = CSatelliteData::StrTimeToCTime(strETime);

		timese.start = ts;
		timese.end = te;
		m_OprPlnDataOrg.m_CmdDayList.push_back(timese);
	}

	// 基本パラメータ-TLM S 有効/無効フラグ
	m_OprPlnDataOrg.m_TlmSChk = m_TLMSChkBox;
	if (m_TLMSChkBox)
	{
		m_PlanData.l_tlms = 1;
	}
	else
	{
		m_PlanData.l_tlms = 0;
	}

	// 基本パラメータ-TLM S 開始終了日時
	count = m_TlmSList.GetItemCount();
	m_OprPlnDataOrg.m_TlmSDayList.clear();

	for (int i = 0; i < count; ++i)
	{
		CString strSTime = m_TlmSList.GetItemText(i, 0);
		ts = CSatelliteData::StrTimeToCTime(strSTime);

		CString strETime = m_TlmSList.GetItemText(i, 1);
		te = CSatelliteData::StrTimeToCTime(strETime);

		timese.start = ts;
		timese.end = te;
		m_OprPlnDataOrg.m_TlmSDayList.push_back(timese);
	}

	// 基本パラメータ-TLM X 有効/無効フラグ
	m_OprPlnDataOrg.m_TlmXChk = m_TLMXChkBox;
	if (m_TLMXChkBox)
	{
		m_PlanData.l_tlmx = 1;
	}
	else
	{
		m_PlanData.l_tlmx = 0;
	}

	// 基本パラメータ-TLM X  開始終了日時
	count = m_TlmXList.GetItemCount();
	m_OprPlnDataOrg.m_TlmXDayList.clear();

	for (int i = 0; i < count; ++i)
	{
		CString strSTime = m_TlmXList.GetItemText(i, 0);
		ts = CSatelliteData::StrTimeToCTime(strSTime);

		CString strETime = m_TlmXList.GetItemText(i, 1);
		te = CSatelliteData::StrTimeToCTime(strETime);

		timese.start = ts;
		timese.end = te;
		m_OprPlnDataOrg.m_TlmXDayList.push_back(timese);
	}

	// 基本パラメータ-TLM Ka 有効/無効フラグ
	m_OprPlnDataOrg.m_TlmKaChk = m_TLMKaChkBox;
	if (m_TLMKaChkBox)
	{
		m_PlanData.l_tlmka = 1;
	}
	else
	{
		m_PlanData.l_tlmka = 0;
	}

	// 基本パラメータ-TLM Ka 開始終了日時
	count = m_TlmKaList.GetItemCount();
	m_OprPlnDataOrg.m_TlmKaDayList.clear();

	for (int i = 0; i < count; ++i)
	{
		CString strSTime = m_TlmKaList.GetItemText(i, 0);
		ts = CSatelliteData::StrTimeToCTime(strSTime);

		CString strETime = m_TlmKaList.GetItemText(i, 1);
		te = CSatelliteData::StrTimeToCTime(strETime);

		timese.start = ts;
		timese.end = te;
		m_OprPlnDataOrg.m_TlmKaDayList.push_back(timese);
	}

	// ANT待ちうけ指向通知
#if 1
	//	m_OprPlnDataOrg.m_strAntStandbyTime = m_AntStandbyStr;
	m_OprPlnDataOrg.m_strAntStandby = m_AntStandbyStr;
	m_PlanData.sz_antwait_flg = m_AntStandbyStr;
//	m_PlanData.l_antwait_flg = COprPlnData::GetAntStandbyIdx(m_AntStandbyStr);
	m_PlanData.l_ant_time = 0;

	if (m_AntStandbyStr == strAriNasi[ARINASI_ARI])
	{
		m_PlanData.l_antwait_flg = 1;
	}
	else
	{
		m_PlanData.l_antwait_flg = 0;
	}

#else
//	m_AntStandbyCmb.GetWindowTextW(str);
//	m_OprPlnDataOrg.m_AntStandby = COprPlnData::GetAntStandbyIdx(str);
//	m_PlanData.l_antwait_flg = COprPlnData::GetAntStandbyIdx(str);
//	m_PlanData.l_ant_time = 0;
#endif

	// ANT天頂志向有無
#if 1
	m_OprPlnDataOrg.m_strAntZenith = m_AntZenithStr;
	m_PlanData.sz_ant_stow = m_AntZenithStr;

	if (m_AntZenithStr == strANTZenith[ANTZENITH_ARI_SWTOW])
	{
		m_PlanData.l_ant_stow = 0;
	}
	else if (m_AntZenithStr == strANTZenith[ANTZENITH_ARI])
	{
		m_PlanData.l_ant_stow = 1;
	}
	else
	{
		m_PlanData.l_ant_stow = 2;
	}

#else
//	m_AntZenithCmb.GetWindowTextW(str);
//	m_OprPlnDataOrg.m_AntZenith = COprPlnData::GetAntZenithIdx(str);
//	m_PlanData.l_ant_stow = COprPlnData::GetAntZenithIdx(str);
#endif

	// 運用オプション-初期設定
	m_OpInitCmb.GetWindowTextW(str);
	int init = COprPlnData::GetOpInitIdx(str);
	if (init == 0)
	{
		m_OprPlnDataOrg.m_OpInit = 1;
		m_PlanData.l_init = 1;
	}
	else
	{
		m_OprPlnDataOrg.m_OpInit = 0;
		m_PlanData.l_init = 0;
	}

	// 運用オプション-送信機ドップラ補償
#if 1
	m_OprPlnDataOrg.m_OpTDPStr = m_OpTDPStr;
	m_PlanData.sz_tx_ramp = m_OpTDPStr;
	m_PlanData.l_tx_ramp = COprPlnData::GetOpTDPIdx2(m_OpTDPStr);
#else
//	m_OpTDPCmb.GetWindowTextW(str);
//	m_OprPlnDataOrg.m_OpTDP = COprPlnData::GetOpTDPIdx(str);
//	m_PlanData.l_tx_ramp = COprPlnData::GetOpTDPIdx(str);
#endif

	// 運用オプション-受信機ドップラ補償
	m_OpRDPCmb.GetWindowTextW(str);
	m_OprPlnDataOrg.m_OpRDP = COprPlnData::GetOpRDPIdx(str);
	m_PlanData.l_rx_ramp = COprPlnData::GetOpRDPIdx(str);

	// 運用オプション-送信出力
#if 1
	m_OprPlnDataOrg.m_OpTOutstr = m_TrsPowerStr;
	m_PlanData.sz_tx_pwr_sel = m_TrsPowerStr;

	long txPwrSel = -1;
	if (s == eStation_Usuda64)			// 臼田64m
	{
		if (m_TrsPowerStr.IsEmpty() == FALSE)
		{
			if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_Y_ID)
			{
				for (int i = 0; sz_x_tx_pwr_sel_cid64[i] != _T(""); ++i)
				{
					if (sz_x_tx_pwr_sel_cid64[i].CompareNoCase(m_TrsPowerStr) == 0)
					{
						//					m_PlanData.l_tx_pwr_sel = i / 2;
						txPwrSel = i / 2;
						break;
					}
				}
			}
			else
			{
				for (int i = 0; sz_s_tx_pwr_sel_cid64[i] != _T(""); ++i)
				{
					if (sz_s_tx_pwr_sel_cid64[i].CompareNoCase(m_TrsPowerStr) == 0)
					{
						//					m_PlanData.l_tx_pwr_sel = i;
						txPwrSel = i;
						break;
					}
				}
			}
		}
		else
		{
			txPwrSel = 2;
		}
	}
	else if (s == eStation_Usuda642)	// 臼田54m
	{
		if (m_TrsPowerStr.IsEmpty() == FALSE)
		{
			for (int i = 0; sz_x_tx_pwr_sel_cid54[i] != _T(""); ++i)
			{
				if (sz_x_tx_pwr_sel_cid54[i].CompareNoCase(m_TrsPowerStr) == 0)
				{
					//					m_PlanData.l_tx_pwr_sel = i;
					txPwrSel = i;
					break;
				}
			}
		}
		else
		{
			txPwrSel = 3;	//200w
		}
	}
	else if (s == eStation_Uchinoura34)	// 内之浦34m
	{
		if (m_TrsPowerStr.IsEmpty() == FALSE)
		{
			if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_Y_ID)
			{
				for (int i = 0; sz_x_tx_pwr_sel_cid34[i] != _T(""); ++i)
				{
					if (sz_x_tx_pwr_sel_cid34[i].CompareNoCase(m_TrsPowerStr) == 0)
					{
						//					m_PlanData.l_tx_pwr_sel = i;
						txPwrSel = i;
						break;
					}
				}
			}
			else
			{
				for (int i = 0; sz_s_tx_pwr_sel_cid34[i] != _T(""); ++i)
				{
					if (sz_s_tx_pwr_sel_cid34[i].CompareNoCase(m_TrsPowerStr) == 0)
					{
						//					m_PlanData.l_tx_pwr_sel = i;
						txPwrSel = i;
						break;
					}
				}
			}
		}
		else
		{
			txPwrSel = 0;
		}
	}
	else if (s == eStation_Uchinoura20)	// 内之浦20m
	{
		if (m_TrsPowerStr.IsEmpty() == FALSE)
		{
			for (int i = 0; sz_tx_pwr_sel_cid20[i] != _T(""); ++i)
			{
				if (sz_tx_pwr_sel_cid20[i].CompareNoCase(m_TrsPowerStr) == 0)
				{
					//				m_PlanData.l_tx_pwr_sel = i;
					txPwrSel = i;
					break;
				}
			}
		}
		else
		{
			txPwrSel = 0;
		}
	}

	if (txPwrSel == -1)
	{
		AfxMessageBox(_T("運用オプションの送信出力値が不正です。"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	else
	{
		m_PlanData.l_tx_pwr_sel = txPwrSel;
	}


#else
//	m_OprPlnDataOrg.m_OpTOutType = m_OpTOutType;
//	m_OpTOutCmb.GetWindowTextW(str);
//	m_OprPlnDataOrg.m_OpTOut = COprPlnData::GetOpTOutIdx(str, m_OpTOutType);
//	m_PlanData.l_tx_pwr_sel = COprPlnData::GetOpTOutIdx(str, m_OpTOutType);
#endif

	// レディネスチェック-AGC校正
	m_OprPlnDataOrg.m_AgcChkValue = m_AGCChkValue;
	if (m_AGCChkValue)
	{
		m_PlanData.l_agc_index = 1;
	}
	else
	{
		m_PlanData.l_agc_index = 0;
	}
	m_PlanData.l_agc_time = 0;
	m_PlanData.l_agc_s_time = 0;
	m_PlanData.l_agc_x_time = 0;


	// レディネスチェック-RNG校正
	m_OprPlnDataOrg.m_RndChkValue = m_RNGChkValue;
	if (m_RNGChkValue)
	{
		m_PlanData.l_rng_index = 1;
	}
	else
	{
		m_PlanData.l_rng_index = 0;
	}
	m_PlanData.l_rng_time = 0;
	m_PlanData.l_rng_xx_time = 0;
	m_PlanData.l_rng_rx_time = 0;


	// レディネスチェック-TLM転送
	m_OprPlnDataOrg.m_TlmChkValue = m_TLMChkValue;
	if (m_TLMChkValue)
	{
		m_PlanData.l_tlm_index = 1;
	}
	else
	{
		m_PlanData.l_tlm_index = 0;
	}
	m_PlanData.l_tlm_time = 0;
	m_PlanData.l_tlm_s_time = 0;
	m_PlanData.l_tlm_x_time = 0;


	// レディネスチェック-CMD伝送
	m_OprPlnDataOrg.m_CmdChkValue = m_CMDChkValue;
	if (m_CMDChkValue)
	{
		m_PlanData.l_cmd_index = 1;
	}
	else
	{
		m_PlanData.l_cmd_index = 0;
	}
	m_PlanData.l_cmd_time = 0;

	// CID
	m_CidBtnCtrl.GetWindowTextW(str);
	m_OprPlnDataOrg.m_strCID = str;
	m_PlanData.sz_cid = str;

	// ビットレート-TLM S
	m_OprPlnDataOrg.m_strBRTLMS = m_strBRTLMS;
	m_PlanData.sz_s_bitrate = m_strBRTLMS;

	// ビットレート-TLM X
	m_OprPlnDataOrg.m_strBRTLMX = m_strBRTLMX;
	m_PlanData.sz_x_bitrate = m_strBRTLMX;

	// ビットレート-TLM Ka
	m_OprPlnDataOrg.m_strBRTLMKa = m_strBRTLMKa;
	m_PlanData.sz_ka_bitrate = m_strBRTLMKa;

	// ビットレート-高速TLM
	m_OprPlnDataOrg.m_strBRHTLM = m_strBRHTLM;
	m_PlanData.sz_x_qpsk = m_strBRHTLM;

	// ビットレート-CMD S
	m_OprPlnDataOrg.m_strBRCMDS = m_strBRCMDS;
	m_PlanData.sz_cmd_bit_rate = m_strBRCMDS;

	// ビットレート-CMD X
	m_OprPlnDataOrg.m_strBRCMDX = m_strBRCMDX;
	m_PlanData.sz_x_cmd_bit_rate = m_strBRCMDX;

	// アンテナ予報値（AOS、LOS）
	m_OprPlnDataOrg.m_AntFcstValue.start = m_tAOS;
	m_OprPlnDataOrg.m_AntFcstValue.end = m_tLOS;

	if (m_tOpeS == m_tAOS)
	{
		m_PlanData.l_a_flag = 1;
	}
	else
	{
		m_PlanData.l_a_flag = 0;
	}

	if (m_tOpeE == m_tLOS)
	{
		m_PlanData.l_l_flag = 1;
	}
	else
	{
		m_PlanData.l_l_flag = 0;
	}

	m_PlanData.l_offset_val = 0;

	m_PlanData.l_rdychk = 0;

	// AUTO_ACQ ON固定
	m_PlanData.l_auto_acq = 1;

	return ret;
}

/*============================================================================*/
/*! COperationPlan

-# データを格納クラスに保存する

@param  なし
@retval TRUE：登録済み/FALSE：新規
*/
/*============================================================================*/
BOOL COperationPlan::SetPlanDataMap(CString satellite, CString planid, COprPlnData pdnew)
{
	BOOL bData = FALSE;
	CSatelliteData& sd = theApp.GetSatelliteData();
	for (auto itr_s = sd.m_listOprPlnData.begin(); itr_s != sd.m_listOprPlnData.end(); ++itr_s)
	{
		CString strStl = (CString)(itr_s->first);
		if (strStl == satellite)
		{
			vector<COprPlnData> *vpd = (vector<COprPlnData>*)(&itr_s->second);
			for (auto itr_o = vpd->begin(); itr_o != vpd->end(); ++itr_o)
			{
				COprPlnData pd = (COprPlnData)*itr_o;
				if (pd.m_strPlanID == planid)
				{
					// 更新
					(*itr_o) = pdnew;
					theApp.GetSatelliteData().m_listOprPlnData[satellite] = *vpd;
					bData = TRUE;
					break;
				}
			}
		}
	}

	// 新規追加
	if (bData == FALSE)
	{
		theApp.GetSatelliteData().m_listOprPlnData[satellite].push_back(pdnew);
	}

	return bData;
}

/*============================================================================*/
/*! COperationPlan

-# データを格納クラスから取得する

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL COperationPlan::GetPlanData(CString satellite, CString planid, COprPlnData& data)
{
	BOOL bData = FALSE;
	CSatelliteData& sd = theApp.GetSatelliteData();
	for (auto itr_s = sd.m_listOprPlnData.begin(); itr_s != sd.m_listOprPlnData.end(); ++itr_s)
	{
		CString strStl = (CString)(itr_s->first);
		if (strStl == satellite)
		{
			vector<COprPlnData> *vpd = (vector<COprPlnData>*)(&itr_s->second);
			for (auto itr_o = vpd->begin(); itr_o != vpd->end(); ++itr_o)
			{
				COprPlnData pd = (COprPlnData)*itr_o;
				if (pd.m_strPlanID == planid)
				{
					data = pd;
					bData = TRUE;
					break;
				}
			}
		}
	}

	if (bData == FALSE)
	{
		;
	}
	return bData;
}

/*============================================================================*/
/*! COperationPlan

-# データを格納クラスから取得する

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL COperationPlan::CheckPlanDataDlg()
{
	int i = 0;
	int idx = 0;
	CTime cSTime, cETime;
	CString strSTimeD, strETimeD;
	CString strSTimeL, strETimeL;
	CString str;

	UpdateData();

	// 基本パラメータ-運用開始終了日時
	CTime tStart = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());
	CTime tEnd = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());

	if ((tStart != m_OprPlnDataOrg.m_OpetDay.start) || (tEnd != m_OprPlnDataOrg.m_OpetDay.end))
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("OpeTime"), nLogEx::debug);
		return TRUE;
	}

	// 基本パラメータ-アップリンク 有効/無効フラグ
	if (m_UpLinkChkBox != m_OprPlnDataOrg.m_UpLinkChk)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("UpLink Flg"), nLogEx::debug);
		return TRUE;
	}

	// 基本パラメータ-アップリンク 開始終了日時リスト
	int lcount = m_UpLinkList.GetItemCount();
	int vcount = (int)m_OprPlnDataOrg.m_ULDay.size();
	if (lcount != vcount)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("UpLink Time Size"), nLogEx::debug);
		return TRUE;
	}

	i = 0;
	for (auto itr = m_OprPlnDataOrg.m_ULDay.begin(); itr != m_OprPlnDataOrg.m_ULDay.end(); ++itr)
	{
		stTime time = (stTime)*itr;

		cSTime = time.start;
		strSTimeD = cSTime.Format("%Y-%m-%d %H:%M:%S");
		cETime = time.end;
		strETimeD = cETime.Format("%Y-%m-%d %H:%M:%S");

		strSTimeL = m_UpLinkList.GetItemText(i, 0);
		strETimeL = m_UpLinkList.GetItemText(i, 1);

		if ((strSTimeD != strSTimeL) || (strETimeD != strETimeL))
		{
			CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("UpLink Time"), nLogEx::debug);
			return TRUE;
		}
		++i;
	}

	// 基本パラメータ-RNG 有効/無効フラグ
	if (m_RngChkBox != m_OprPlnDataOrg.m_RngChk)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("RNG Flg"), nLogEx::debug);
		return TRUE;
	}

	// 基本パラメータ-RNG 開始終了日時リスト
	lcount = m_RngList.GetItemCount();
	vcount = (int)m_OprPlnDataOrg.m_RngDayList.size();
	if (lcount != vcount)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("RNG Time Size"), nLogEx::debug);
		return TRUE;
	}

	i = 0;
	for (auto itr = m_OprPlnDataOrg.m_RngDayList.begin(); itr != m_OprPlnDataOrg.m_RngDayList.end(); ++itr)
	{
		stTime time = (stTime)*itr;

		cSTime = time.start;
		strSTimeD = cSTime.Format("%Y-%m-%d %H:%M:%S");
		cETime = time.end;
		strETimeD = cETime.Format("%Y-%m-%d %H:%M:%S");

		strSTimeL = m_RngList.GetItemText(i, 0);
		strETimeL = m_RngList.GetItemText(i, 1);

		if ((strSTimeD != strSTimeL) || (strETimeD != strETimeL))
		{
			CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("RNG Time"), nLogEx::debug);
			return TRUE;
		}
		++i;
	}

	// 基本パラメータ-CMD 有効/無効フラグ
	if (m_CmdChkBox != m_OprPlnDataOrg.m_CmdChk)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("CMD Flg"), nLogEx::debug);
		return TRUE;
	}

	// 基本パラメータ-CMD 開始終了日時
	lcount = m_CmdList.GetItemCount();
	vcount = (int)m_OprPlnDataOrg.m_CmdDayList.size();
	if (lcount != vcount)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("CMD Time Size"), nLogEx::debug);
		return TRUE;
	}

	i = 0;
	for (auto itr = m_OprPlnDataOrg.m_CmdDayList.begin(); itr != m_OprPlnDataOrg.m_CmdDayList.end(); ++itr)
	{
		stTime time = (stTime)*itr;

		cSTime = time.start;
		strSTimeD = cSTime.Format("%Y-%m-%d %H:%M:%S");
		cETime = time.end;
		strETimeD = cETime.Format("%Y-%m-%d %H:%M:%S");

		strSTimeL = m_CmdList.GetItemText(i, 0);
		strETimeL = m_CmdList.GetItemText(i, 1);

		if ((strSTimeD != strSTimeL) || (strETimeD != strETimeL))
		{
			CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("CMD Time"), nLogEx::debug);
			return TRUE;
		}
		++i;
	}

	// 基本パラメータ-TLM S 有効/無効フラグ
	if (m_TLMSChkBox != m_OprPlnDataOrg.m_TlmSChk)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("TLM-S Flg"), nLogEx::debug);
		return TRUE;
	}

	// 基本パラメータ-TLM S 開始終了日時
	i = 0;
	for (auto itr = m_OprPlnDataOrg.m_TlmSDayList.begin(); itr != m_OprPlnDataOrg.m_TlmSDayList.end(); ++itr)
	{
		stTime time = (stTime)*itr;

		cSTime = time.start;
		strSTimeD = cSTime.Format("%Y-%m-%d %H:%M:%S");
		cETime = time.end;
		strETimeD = cETime.Format("%Y-%m-%d %H:%M:%S");

		strSTimeL = m_TlmSList.GetItemText(i, 0);
		strETimeL = m_TlmSList.GetItemText(i, 1);

		if ((strSTimeD != strSTimeL) || (strETimeD != strETimeL))
		{
			CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("TLM-S Time"), nLogEx::debug);
			return TRUE;
		}
		++i;
	}


	// 基本パラメータ-TLM X 有効/無効フラグ
	if (m_TLMXChkBox != m_OprPlnDataOrg.m_TlmXChk)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("TLM-X Flg"), nLogEx::debug);
		return TRUE;
	}

	// 基本パラメータ-TLM X  開始終了日時
	i = 0;
	for (auto itr = m_OprPlnDataOrg.m_TlmXDayList.begin(); itr != m_OprPlnDataOrg.m_TlmXDayList.end(); ++itr)
	{
		stTime time = (stTime)*itr;

		cSTime = time.start;
		strSTimeD = cSTime.Format("%Y-%m-%d %H:%M:%S");
		cETime = time.end;
		strETimeD = cETime.Format("%Y-%m-%d %H:%M:%S");

		strSTimeL = m_TlmXList.GetItemText(i, 0);
		strETimeL = m_TlmXList.GetItemText(i, 1);

		if ((strSTimeD != strSTimeL) || (strETimeD != strETimeL))
		{
			CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("TLM-X Time"), nLogEx::debug);
			return TRUE;
		}
		++i;
	}

	// 基本パラメータ-TLM Ka 有効/無効フラグ
	if (m_TLMKaChkBox != m_OprPlnDataOrg.m_TlmKaChk)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("TLM-Ka Flg"), nLogEx::debug);
		return TRUE;
	}

	// 基本パラメータ-TLM Ka 開始終了日時
	i = 0;
	for (auto itr = m_OprPlnDataOrg.m_TlmKaDayList.begin(); itr != m_OprPlnDataOrg.m_TlmKaDayList.end(); ++itr)
	{
		stTime time = (stTime)*itr;

		cSTime = time.start;
		strSTimeD = cSTime.Format("%Y-%m-%d %H:%M:%S");
		cETime = time.end;
		strETimeD = cETime.Format("%Y-%m-%d %H:%M:%S");

		strSTimeL = m_TlmKaList.GetItemText(i, 0);
		strETimeL = m_TlmKaList.GetItemText(i, 1);

		if ((strSTimeD != strSTimeL) || (strETimeD != strETimeL))
		{
			CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("TLM-Ka Time"), nLogEx::debug);
			return TRUE;
		}
		++i;
	}

	// ANT待ちうけ指向通知
#if 1
	if (m_AntStandbyStr != m_OprPlnDataOrg.m_strAntStandby)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("AntStandby"), nLogEx::debug);
		return TRUE;
	}
#else
//	m_AntStandbyCmb.GetWindowTextW(str);
//	idx = COprPlnData::GetAntStandbyIdx(str);
//	if (idx != m_OprPlnDataOrg.m_AntStandby)
//	{
//		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("AntStandby"), nLogEx::debug);
//		return TRUE;
//	}
#endif

	// ANT天頂志向有無
#if 1
	if (m_AntZenithStr != m_OprPlnDataOrg.m_strAntZenith)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("AntZenith"), nLogEx::debug);
		return TRUE;
	}
#else
//	m_AntZenithCmb.GetWindowTextW(str);
//	idx = COprPlnData::GetAntZenithIdx(str);
//	if (idx != m_OprPlnDataOrg.m_AntZenith)
//	{
//		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("AntZenith"), nLogEx::debug);
//		return TRUE;
//	}
#endif

	// 運用オプション-初期設定
	m_OpInitCmb.GetWindowTextW(str);
	idx = COprPlnData::GetOpInitIdx(str);
	idx ^= 1;
	if (idx != m_OprPlnDataOrg.m_OpInit)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("OpInit"), nLogEx::debug);
		return TRUE;
	}

	// 運用オプション-送信機ドップラ補償
#if 1
	if (m_OpTDPStr != m_OprPlnDataOrg.m_OpTDPStr)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("OpTDp"), nLogEx::debug);
		return TRUE;
	}
#else
//	m_OpTDPCmb.GetWindowTextW(str);
//	idx = COprPlnData::GetOpTDPIdx(str);
//	if (idx != m_OprPlnDataOrg.m_OpTDP)
//	{
//		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("OpTDp"), nLogEx::debug);
//		return TRUE;
//	}
#endif

	// 運用オプション-受信機ドップラ補償
	m_OpRDPCmb.GetWindowTextW(str);
	idx = COprPlnData::GetOpRDPIdx(str);
	if (idx != m_OprPlnDataOrg.m_OpRDP)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("OpRDp"), nLogEx::debug);
		return TRUE;
	}

#if 0	// 画面に無いので削除
	// 運用オプション-送信出力タイプ
	if (m_OpTOutType != m_OprPlnDataOrg.m_OpTOutType)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("OpTOutType"), nLogEx::debug);
		return TRUE;
	}
#endif

	// 運用オプション-送信出力
#if 1
#if 0	// CID のチェックでOK
	if (m_TrsPowerStr != m_OprPlnDataOrg.m_OpTOutstr)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("OpTOut"), nLogEx::debug);
		return TRUE;
	}
#endif
#else
//	m_OpTOutCmb.GetWindowTextW(str);
//	idx = COprPlnData::GetOpTOutIdx(str, m_OpTOutType);
//	if (idx != m_OprPlnDataOrg.m_OpTOut)
//	{
//		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("OpTOut"), nLogEx::debug);
//		return TRUE;
//	}
#endif

	// レディネスチェック-AGC校正
	if (m_AGCChkValue != m_OprPlnDataOrg.m_AgcChkValue)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("RDNS AGC"), nLogEx::debug);
		return TRUE;
	}

	// レディネスチェック-RNG校正
	if (m_RNGChkValue != m_OprPlnDataOrg.m_RndChkValue)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("RDNS RNG"), nLogEx::debug);
		return TRUE;
	}

	// レディネスチェック-TLM転送
	if (m_TLMChkValue != m_OprPlnDataOrg.m_TlmChkValue)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("RDNS TLM"), nLogEx::debug);
		return TRUE;
	}

	// レディネスチェック-CMD伝送
	if (m_CMDChkValue != m_OprPlnDataOrg.m_CmdChkValue)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("RDNS CMD"), nLogEx::debug);
		return TRUE;
	}

	// CID
	m_CidBtnCtrl.GetWindowTextW(str);
	if (str != m_OprPlnDataOrg.m_strCID)
	{
		CLogTraceEx::Write(_T(""), _T("COperationPlan"), _T("CheckPlanDataDlg"), _T("debug"), _T("CID"), nLogEx::debug);
		return TRUE;
	}

#if 0
//	// ビットレート-TLM S
//	if (m_strBRTLMS != m_OprPlnDataOrg.m_strBRTLMS)
//	{
//		return TRUE;
//	}
//
//	// ビットレート-TLM X
//	if (m_strBRTLMX != m_OprPlnDataOrg.m_strBRTLMX)
//	{
//		return TRUE;
//	}
//
//	// ビットレート-TLM Ka
//	if (m_strBRTLMKa != m_OprPlnDataOrg.m_strBRTLMKa)
//	{
//		return TRUE;
//	}
//
//	// ビットレート-高速TLM
//	if (m_strBRHTLM != m_OprPlnDataOrg.m_strBRHTLM)
//	{
//		return TRUE;
//	}
//
//	// ビットレート-CMD S
//	if (m_strBRCMDS != m_OprPlnDataOrg.m_strBRCMDS)
//	{
//		return TRUE;
//	}
//
//	// ビットレート-CMD X
//	if (m_strBRCMDX != m_OprPlnDataOrg.m_strBRCMDX)
//	{
//		return TRUE;
//	}
#endif
	return FALSE;
}


/*============================================================================*/
/*! COperationPlan

-# 予報値一覧ファイルより、予報値時刻を取得する

@param  pc_satname	：衛星名
@param  pc_pass		：パス
@param  pc_aos_time	：AOS時刻
@param  pc_los_time	：LOS時刻
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
BOOL COperationPlan::GetPredTime(CString pc_satname, CString pc_pass, CTime& pc_aos_time, CTime& pc_los_time, stPlandata& stpd)
{
	BOOL ret = FALSE;
	CString passid = pc_pass;

#if 1

	// 計画一覧より予報値情報取得
	CGraphMutex::Lock(eFile);
	vector<stPlanListData>& planlist = theApp.GetSatelliteData().GetPlanList();
	for (auto itr = planlist.begin(); itr != planlist.end(); itr++)
	{
		stPlanListData st = (stPlanListData)(*itr);

		if (st.strStationName == mStationString[theApp.GetSelectStation()])
		{
			if (st.strSatelliteName == pc_satname && st.strPassId == pc_pass)
			{
				int idx = st.strPredName.Find(_T("."));
				CString strid = st.strPredName.Mid(0, idx);
				if (strid.IsEmpty() == false)
				{
					// 予報値IDが取得できた場合
					strid = ChgPlanIDFormat(strid);
					passid = strid;
					break;
				}
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	// 予報値一覧からAos/Los取得
	CGraphMutex::Lock(eFile);
	vector<stPredListData>& predlist = theApp.GetSatelliteData().GetPredList();
	for (auto itr = predlist.begin(); itr != predlist.end(); itr++)
	{
		stPredListData st = (stPredListData)(*itr);
		if ((st.strSatellite == pc_satname) && (st.strPassId == passid))
		{
			pc_aos_time = CTime(st.tAOS);
			pc_los_time = CTime(st.tLOS);
			stpd.l_offset_val = st.tOffset;
			stpd.sz_pred_path = st.strPredPath.Right(st.strPredPath.GetLength() - st.strPredPath.ReverseFind('/') - 1);
			ret = TRUE;
			break;
		}
	}
	CGraphMutex::Unlock(eFile);

#else
//	FILE *fp;
//
//	CString strFilePath;
//	strFilePath.Format(_T("%s%s"), theApp.GetAppPath(), PRED_LIST_FILE);
//
//	CString satname = _T("");
//	CString pass = _T("");
//
//	char sz_pred_line[300];
//	char sz_aos[50];
//	char sz_aos_date[50];
//	char sz_los[50];
//	char sz_los_date[50];
//
//	char sz_stationname[50];
//	char sz_satname[50];
//	char sz_t_flag[50];
//	char sz_p_flag[50];
//	char sz_pass[50];
//	char sz_a_date[50];
//	char sz_a_time[50];
//	char sz_a_az[50];
//	char sz_a_el[50];
//	char sz_a_rng[50];
//	char sz_l_date[50];
//	char sz_l_time[50];
//	char sz_l_az[50];
//	char sz_l_el[50];
//	char sz_l_rng[50];
//	char sz_m_date[50];
//	char sz_m_time[50];
//	char sz_m_az[50];
//	char sz_m_el[50];
//	char sz_m_rng[50];
//	char sz_date[50];
//	char sz_time[50];
//	char sz_pred_path[50];
//	char sz_offset[50];
//
//	int i;
//	long l_offset;
//
//	if (_wfopen_s(&fp, strFilePath, _T("r")) != 0)
//	{
//		return FALSE;
//	}
//
//	while ( fgets ( sz_pred_line, sizeof ( sz_pred_line ), fp ) != NULL ) 
//	{
//		sscanf_s(sz_pred_line, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
//			sz_stationname, sizeof(sz_stationname),
//			sz_satname, sizeof(sz_satname), sz_t_flag, sizeof(sz_t_flag), sz_p_flag, sizeof(sz_p_flag), sz_pass, sizeof(sz_pass),
//			sz_a_date, sizeof(sz_a_date), sz_a_time, sizeof(sz_a_time), sz_a_az, sizeof(sz_a_az), sz_a_el, sizeof(sz_a_el), sz_a_rng, sizeof(sz_a_rng),
//			sz_l_date, sizeof(sz_l_date), sz_l_time, sizeof(sz_l_time), sz_l_az, sizeof(sz_l_az), sz_l_el, sizeof(sz_l_el), sz_l_rng, sizeof(sz_l_rng),
//			sz_m_date, sizeof(sz_m_date), sz_m_time, sizeof(sz_m_time), sz_m_az, sizeof(sz_m_az), sz_m_el, sizeof(sz_m_el), sz_m_rng, sizeof(sz_m_rng),
//			sz_date, sizeof(sz_date), sz_time, sizeof(sz_time), sz_pred_path, sizeof(sz_pred_path), sz_offset, sizeof(sz_offset));
//
//		satname = CString(sz_satname);
//		pass = CString(sz_pass);
//
//		if (satname == pc_satname && pass == pc_pass)
//		{
//			// オフセット(秒)
//			for( i=0; i<strlen( sz_offset ); i++ )
//			{
//				if( sz_offset[i] == ':' )
//				{
//					break;
//				}
//			}
//			l_offset = labs( atoi( sz_offset ) ) * 60;
//			l_offset += atoi( &sz_offset[i+1] );
//			stpd.l_offset_val = l_offset;
//
//
//			// AOS時刻
//			sprintf_s(sz_aos, sizeof(sz_aos), "%s %s%s", sz_a_date, sz_a_time, ".0000");
//			if (CSatelliteData::ub_asc_to_asctime(sz_aos, sz_aos_date) < 0) {
//				//				fprintf(stderr, "%s\n", "運用開始時刻を変換できませんでした。");
//				fclose(fp);
//				return -1;
//			}
//			pc_aos_time = CSatelliteData::StrTimeToCTime(CString(sz_aos_date));
//
//			// LOS時刻
//			sprintf_s(sz_los, sizeof(sz_aos), "%s %s%s", sz_l_date, sz_l_time, ".0000");
//			if (CSatelliteData::ub_asc_to_asctime(sz_los, sz_los_date) < 0) {
//				//				fprintf(stderr, "%s\n", "運用終了時刻を変換できませんでした。");
//				fclose(fp);
//				return -1;
//			}
//			pc_los_time = CSatelliteData::StrTimeToCTime(CString(sz_los_date));
//
//
//			// 予報値ファイル
//			CStringA str(sz_pred_path);
//			str = str.Right(str.GetLength() - str.ReverseFind('/')-1);
//			stpd.sz_pred_path = str;
//
//			fclose(fp);
//			ret = TRUE;
//			break;
//		}
//	}
//
//	fclose(fp);
#endif

	return ret;
}

/*============================================================================*/
/*! COperationPlan

-# 計画一覧ファイルより、衛星名、計画IDを取得する

@param  なし
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
CString COperationPlan::GetFullPath2FileName(CString str)
{
	return _T("");
}


/*============================================================================*/
/*! COperationPlan

-# 計画一覧ファイルより、衛星名、計画IDを取得する

@param  なし
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
#define PLAN_TIME_LEN 20
BOOL COperationPlan::GetPlanList()
{
#if 1
	m_SatelliteList.DeleteAllItems();

	LVITEM  lvi;
	int		i = 0;

	int idxSel = -1;

	CGraphMutex::Lock(eFile);
	vector<stPlanListData>& planlist = theApp.GetSatelliteData().GetPlanList();
	for (auto itr = planlist.begin(); itr != planlist.end(); ++itr)
	{
		stPlanListData &st = (stPlanListData)(*itr);

		// 運用種別が、実運用のみリストする。
		if (st.strPlanType.CompareNoCase(_T("実運用")) != 0)
		{
			continue;
		}

		if (st.strStationName == mStationString[theApp.GetSelectStation()])
		{

			// 取得先
			DWORD dw = 0;
			if (st.strGetFrom.CompareNoCase(_T("ENP")) == 0)
			{
				dw = 1;
			}

			// 衛星名
			lvi.lParam = (LPARAM)dw;
			lvi.iItem = i;
			lvi.iSubItem = 0;
			lvi.mask = LVIF_TEXT;
			lvi.pszText = st.strSatelliteName.GetBuffer();
			m_SatelliteList.InsertItem(&lvi);


			// 計画ID
			lvi.lParam = (LPARAM)dw;
			lvi.iItem = i;
			lvi.iSubItem = 1;
			lvi.mask = LVIF_TEXT;
			lvi.pszText = st.strPassId.GetBuffer();
			m_SatelliteList.SetItem(&lvi);


			if (st.strSatelliteName == m_SatelliteName && st.strPassId == m_PassID)
			{
				idxSel = i;
			}

			m_SatelliteList.SetItemData(i, dw);

			++i;
		}

	}
	CGraphMutex::Unlock(eFile);

	if (idxSel != -1)
	{
		m_SatelliteList.SetItemState(idxSel, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}


#else
//
//	FILE *p_op;
//	char sz_buff[500];
//	char sz_planned_name[50];
//	char sz_planned_path[50];
//	char sz_planned_stime1[50];
//	char sz_planned_stime2[50];
//	char sz_planned_etime1[50];
//	char sz_planned_etime2[50];
//	char sz_planned_itime1[50];
//	char sz_planned_itime2[50];
//	char sz_pred_name[50];
//	char sz_stationname[50];
//
//	int	i_valid_flag;
//	int	i_ofs_flag;
//	int	i_offset;
//	int i;
//
//	CString strSatellite, strPass;
//	CString strFilePath;
//	LVITEM       lvi;
//
//
//	// 衛星リスト
//	m_SatelliteList.DeleteAllItems();
//
//	strFilePath.Format(_T("%s%s"), theApp.GetAppPath(), PLAN_LIST_FILE);
//	if (_wfopen_s(&p_op, strFilePath, _T("r")) != 0)
//	{
//		return FALSE;
//	}
//
//	i = 0;
//	fgets(sz_buff, sizeof(sz_buff), p_op);					/*計画一覧ファイルのヘッダー部の読み飛ばし*/
//	while (fgets(sz_buff, sizeof(sz_buff), p_op) != 0){		/*計画一覧ファイルの読み込み*/
//
//		sz_buff[strlen(sz_buff) - 1] = '\0';				/*最後の'\n'を'\0'に上書きする*/
//		sscanf_s(sz_buff, "%s%s%s%s%s%s%s%s%s%d%d%d",
//			sz_stationname, sizeof(sz_stationname),
//			sz_planned_name, sizeof(sz_planned_name), sz_planned_path, sizeof(sz_planned_path), sz_planned_stime1, sizeof(sz_planned_stime1),
//			sz_planned_stime2, sizeof(sz_planned_stime2), sz_planned_etime1, sizeof(sz_planned_etime1), sz_planned_etime2, sizeof(sz_planned_etime2),
//			sz_planned_itime1, sizeof(sz_planned_itime1), sz_planned_itime2, sizeof(sz_planned_itime2), sz_pred_name, sizeof(sz_pred_name),
//			&i_valid_flag, &i_ofs_flag, &i_offset);
//
//
//		strSatellite = CString(sz_planned_name);
//		strPass = CString(sz_planned_path);
//
//		// 衛星名
//		lvi.lParam = (LPARAM)i;
//		lvi.iItem = i;
//		lvi.iSubItem = 0;
//		lvi.mask = LVIF_TEXT;
//		lvi.pszText = strSatellite.GetBuffer();
//		m_SatelliteList.InsertItem(&lvi);
//
//		// 計画ID
//		lvi.lParam = (LPARAM)i;
//		lvi.iItem = i;
//		lvi.iSubItem = 1;
//		lvi.mask = LVIF_TEXT;
//		lvi.pszText = strPass.GetBuffer();
//		m_SatelliteList.SetItem(&lvi);
//
//		++i;
//	}
//	fclose(p_op);
#endif
	return TRUE;
}

/*============================================================================*/
/*! COperationPlan

-# 衛星固有情報取得

@param  なし
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
BOOL COperationPlan::GetSatelliteInfoData(CString satname)
{
#if 0
	CSatelliteData& sd = theApp.GetSatelliteData();

	CString filename = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
	CString ca_cthome;
	ca_cthome.Format(_T("%s%s"), filename, satname);
	strcpy_s(&sd.m_satinfo[m_IdxEqsat].sz_plan_dir[0], 128, CStringA(ca_cthome));
#endif
	return TRUE;
}

/*============================================================================*/
/*! COperationPlan

-# 計画ファイルのGET

@param  path:ファイルパス（衛星名）
@param  file:ファイル名（*.pln）
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
BOOL COperationPlan::GetPlanFile(CString path, CString file, DWORD lParam)
{
	// ftp? 計画ファイルを取得
	CString strPFile;
#if 1
	CString filename = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
	if (lParam == 1)
	{
		// ENP
		strPFile.Format(_T("%senp\\%s\\%s.pln"), (LPCTSTR)filename, (LPCTSTR)path, (LPCTSTR)file);
	}
	else
	{
		// STTP
		strPFile.Format(_T("%ssttn\\%s\\%s.pln"), (LPCTSTR)filename, (LPCTSTR)path, (LPCTSTR)file);
	}

#else
//	strPFile.Format(_T("%s%s%s\\%s.pln"), theApp.GetAppPath(), PLAN_FILE_DIR, path, file);
#endif

	if (PathFileExists(strPFile) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

/*============================================================================*/
/*! COperationPlan

-# 計画ファイルのPUT

@param  path:ファイルパス（衛星名）
@param  file:ファイル名（*.pln）
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
BOOL COperationPlan::PutPlanFile(CString path, CString file)
{
	// ftp? 計画ファイルを格納
	return TRUE;
}

/*============================================================================*/
/*! COperationPlan

-# 計画ファイルをアクセスする。

@param  なし
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
BOOL COperationPlan::AccessPlanData()
{
	return TRUE;
}


/*============================================================================*/
/*! COperationPlan

-# 計画雛形ファイルを読み込む

@param  なし
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
int COperationPlan::ReadPlanFormFile20(CString sz_satname, CString sz_passno, stPlandata& stpd)
{
	FILE *fp_smpl;
	CString sz_s_file;

	TCHAR sz_buff[512];
	TCHAR sz_time[50];
	TCHAR sz_txrfpwr[3][20];

	long l_ncount;

	long i;
	long j;
	long k;
	long l;

	TCHAR sz_ele[10][100];
	memset(&sz_ele[0], 0, sizeof(sz_ele));

	CString sz_txcomp;

	CSatelliteData& sd = theApp.GetSatelliteData();

#if 1
	CString filename = theApp.GetShareFilePath(eFileType_Form_Folder, theApp.GetSelectStation());
	sz_s_file.Format(_T("%s%s.pln"), (LPCTSTR)filename, (LPCTSTR)sz_satname);
#else
//	CString ca_cthome;
//	ca_cthome.Format(_T("%s%s"), theApp.GetAppPath(), DATA_DIR);
//
//	j = (long)strlen(sd.m_satinfo[m_IdxEqsat].sz_plan_dir);
//	if (sd.m_satinfo[m_IdxEqsat].sz_plan_dir[j - 1] == '/')
//	{
//		sz_s_file.Format(_T("%s\\plan\\form\\%s.pln"), ca_cthome, sz_satname);
//	}
//	else
//	{
//		sz_s_file.Format(_T("%s\\plan\\form\\%s.pln"), ca_cthome, sz_satname);
//	}
#endif

	stpd.l_mod_sel = 0;
	stpd.l_tx_pwr_sel = 0;
	stpd.l_tx_mode = 0;
	stpd.l_route_mode = 0;
	stpd.l_term_ant = 0;
	stpd.l_offset = 0;
	stpd.l_auto_acq = 0;
	stpd.l_cmd_mod = 0;
	stpd.l_rng_mod = 0;
	stpd.l_delay_sel = 0;
	stpd.l_pl_sel = 0;
	stpd.l_top_mon = 0;
	stpd.l_ent_angle = 0;
	stpd.l_ext_angle = 0;
	stpd.l_pl_sel = 0;
	stpd.l_ant_stow = 2;
	stpd.l_range_band = 0;
	stpd.l_acq_route = 0;
	stpd.sz_priority = _T("");


	/*-------------------------------------------------------------------------*/
	/*-----   雛形ファイル読込処理                                        -----*/
	/*-------------------------------------------------------------------------*/
	if (_wfopen_s(&fp_smpl, sz_s_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("計画雛形ファイルを開くことができません。\n%s"), (LPCTSTR)sz_s_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}
	
	char sz_line[512];

	/*---------- コメント用計画内容ファイル出力処理 ----------*/
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_smpl) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			continue;
		}

		l = 0;
		if (sz_buff[0] == 'N')
		{
			if (sz_buff[1] >= 0x30 && sz_buff[1] <= 0x39)
			{
				l_ncount = sz_buff[1] - 0x30;
				k = 2;
			}
			else
			{
				l_ncount = 0;
				k = 1;
			}
		}
		else
		{
			l_ncount = -1;
			k = 0;
		}
		for (; sz_buff[k] != '\0'; k++)
		{
			if (sz_buff[k] != ' ')
			{
				sz_time[l] = sz_buff[k];
				l++;
			}
			else
			{
				if (l > 0)
				{
					sz_time[l] = '\0';
					break;
				}
			}
		}

		j = 0;
		l = 0;
		sz_ele[2][0] = '\0';
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] == ',')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_buff[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				sz_ele[j][l] = '\0';
				l = 0;
				break;
			}
			else if (sz_buff[k] != ' ')
			{
				sz_ele[j][l] = sz_buff[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			continue;
		}

		/*----- S-TX 送信出力選択処理 -----*/
		if (sz_c_ele20[32] == CString(sz_ele[0]))
		{
			/* 送信機-送信出力-S帯送信周波数変換盤を解析 */
			memset(sz_txrfpwr, 0, static_cast<unsigned>(_countof(sz_txrfpwr)));
			swscanf_s(sz_ele[1], _T("%[^_]_%[^_]_%[^_]"),
				sz_txrfpwr[0], static_cast<unsigned>(_countof(sz_txrfpwr[0])),
				sz_txrfpwr[1], static_cast<unsigned>(_countof(sz_txrfpwr[1])),
				sz_txrfpwr[2], static_cast<unsigned>(_countof(sz_txrfpwr[2])));

			/* 送信機+パワー */
			sz_txcomp.Format(_T("%s_%s"), sz_txrfpwr[0], sz_txrfpwr[1]);

			for (i = 0; sz_tx_pwr_sel20[i] != _T(""); ++i)
			{
				if (sz_tx_pwr_sel20[i] == sz_txcomp)
				{
					stpd.l_tx_pwr_sel = i;
					break;
				}
			}
		}
		stpd.l_tx_mode = 0;
	}

	fclose(fp_smpl);
	return 0;
}

int COperationPlan::ReadPlanFormFile34(CString sz_satname, CString sz_passno, stPlandata& stpd)
{
	FILE *fp_smpl;
	CString sz_s_file;

	TCHAR sz_buff[512];
	TCHAR sz_time[50];
	TCHAR sz_txrfpwr[3][20];

	long l_ncount;

	long i;
	long j;
	long k;
	long l;

	TCHAR sz_ele[10][100];
	memset(&sz_ele[0], 0, sizeof(sz_ele));

	CString sz_txcomp;

	CSatelliteData& sd = theApp.GetSatelliteData();

#if 1
	CString filename = theApp.GetShareFilePath(eFileType_Form_Folder, theApp.GetSelectStation());
	sz_s_file.Format(_T("%s%s.pln"), (LPCTSTR)filename, (LPCTSTR)sz_satname);
#else
//	CString ca_cthome;
//	ca_cthome.Format(_T("%s%s"), theApp.GetAppPath(), DATA_DIR);
//
//	j = (long)strlen(sd.m_satinfo[m_IdxEqsat].sz_plan_dir);
//	if (sd.m_satinfo[m_IdxEqsat].sz_plan_dir[j - 1] == '/')
//	{
//		sz_s_file.Format(_T("%s\\plan\\form\\%s.pln"), ca_cthome, sz_satname);
//	}
//	else
//	{
//		sz_s_file.Format(_T("%s\\plan\\form\\%s.pln"), ca_cthome, sz_satname);
//	}
#endif

	stpd.l_mod_sel = 0;
	stpd.l_tx_pwr_sel = 0;
	stpd.l_tx_mode = 0;
	stpd.l_route_mode = 0;
	stpd.l_term_ant = 0;
	stpd.l_offset = 0;
	stpd.l_auto_acq = 0;
	stpd.l_cmd_mod = 0;
	stpd.l_rng_mod = 0;
	stpd.l_delay_sel = 0;
	stpd.l_pl_sel = 0;
	stpd.l_top_mon = 0;
	stpd.l_ent_angle = 0;
	stpd.l_ext_angle = 0;
	stpd.l_pl_sel = 0;
	stpd.l_ant_stow = 2;
	stpd.l_range_band = 0;
	stpd.l_acq_route = 0;
	stpd.sz_priority = _T("");

	/*-------------------------------------------------------------------------*/
	/*-----   雛形ファイル読込処理                                        -----*/
	/*-------------------------------------------------------------------------*/
	if (_wfopen_s(&fp_smpl, sz_s_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("計画雛形ファイルを開くことができません。\n%s"), (LPCTSTR)sz_s_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	char sz_line[512];

	/*---------- コメント用計画内容ファイル出力処理 ----------*/
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_smpl) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));

		if (sz_buff[0] == '#')
		{
			continue;
		}

		l = 0;
		if (sz_buff[0] == 'N')
		{
			if (sz_buff[1] >= 0x30 && sz_buff[1] <= 0x39)
			{
				l_ncount = sz_buff[1] - 0x30;
				k = 2;
			}
			else
			{
				l_ncount = 0;
				k = 1;
			}
		}
		else
		{
			l_ncount = -1;
			k = 0;
		}
		for (; sz_buff[k] != '\0'; k++)
		{
			if (sz_buff[k] != ' ')
			{
				sz_time[l] = sz_buff[k];
				l++;
			}
			else
			{
				if (l > 0)
				{
					sz_time[l] = '\0';
					break;
				}
			}
		}

		j = 0;
		l = 0;
		sz_ele[2][0] = '\0';
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] == ',')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_buff[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				sz_ele[j][l] = '\0';
				l = 0;
				break;
			}
			else if (sz_buff[k] != ' ')
			{
				sz_ele[j][l] = sz_buff[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			continue;
		}

		/*----- S-TX 送信出力選択処理 -----*/
		if (sz_c_ele34[32] == CString(sz_ele[0]))
		{
			/* 送信機-送信出力-S帯送信周波数変換盤を解析 */
			memset(sz_txrfpwr, 0, static_cast<unsigned>(_countof(sz_txrfpwr)));
			swscanf_s(sz_ele[1], _T("%[^_]_%[^_]_%[^_]"),
				sz_txrfpwr[0], static_cast<unsigned>(_countof(sz_txrfpwr[0])),
				sz_txrfpwr[1], static_cast<unsigned>(_countof(sz_txrfpwr[1])),
				sz_txrfpwr[2], static_cast<unsigned>(_countof(sz_txrfpwr[2])));

			/* 送信機+パワー */
			sz_txcomp.Format(_T("%s_%s"), sz_txrfpwr[0], sz_txrfpwr[1]);

			for (i = 0; sz_s_tx_pwr_sel[i] != _T(""); ++i)
			{
				if (sz_s_tx_pwr_sel[i] == sz_txcomp)
				{
					stpd.l_tx_pwr_sel = i;
					break;
				}
			}

			/* Ｓ帯送信周波数変換盤 */
			for (i = 0; sz_s_tx_uc_sel[i] != _T(""); ++i) {
				if (sz_s_tx_uc_sel[i] == CString(sz_txrfpwr[2])) {
					stpd.l_tx_uc_sel = i;
					break;
				}
			}
		}

		/*----- X-TX 送信出力選択処理 -----*/
		if (sz_c_ele34[33] == CString(sz_ele[0]))
		{
			for (i = 0; sz_x_tx_pwr_sel[i] != _T(""); ++i)
			{
				if (sz_x_tx_pwr_sel[i] == CString(sz_ele[1]))
				{
					stpd.l_tx_pwr_sel = i;
					break;
				}
			}
		}

		/* 送信出力がX-TX */
		if (sz_c_ele34[50] == CString(sz_ele[0]))
		{
			stpd.l_tx_mode = 1;
		}

		/* X帯送受信ルート判定文字列を探す */
		if (CString(sz_ele[0]).Find(X_ROUTE_DEF_TXT) >= 0 )
		{
			stpd.l_route_mode = 1;
		}

	}

	fclose(fp_smpl);
	return 0;
}

int COperationPlan::ReadPlanFormFile64(CString sz_satname, CString sz_passno, stPlandata& stpd)
{
	FILE *fp_smpl;
	CString sz_s_file;

	TCHAR sz_buff[512];
	TCHAR sz_time[50];
//	TCHAR sz_txrfpwr[3][20];

	long l_ncount;

	long i;
	long j;
	long k;
	long l;

	TCHAR sz_ele[10][100];
	memset(&sz_ele[0], 0, sizeof(sz_ele));

	CString sz_txcomp;

	CSatelliteData& sd = theApp.GetSatelliteData();

#if 1
	CString filename = theApp.GetShareFilePath(eFileType_Form_Folder, theApp.GetSelectStation());
	sz_s_file.Format(_T("%s%s.pln"), (LPCTSTR)filename, (LPCTSTR)sz_satname);

#else
//	CString ca_cthome;
//	ca_cthome.Format(_T("%s%s"), theApp.GetAppPath(), DATA_DIR);
//
//	j = (long)strlen(sd.m_satinfo[m_IdxEqsat].sz_plan_dir);
//	if (sd.m_satinfo[m_IdxEqsat].sz_plan_dir[j - 1] == '/')
//	{
//		sz_s_file.Format(_T("%s\\plan\\form\\%s.pln"), ca_cthome, sz_satname);
//	}
//	else
//	{
//		sz_s_file.Format(_T("%s\\plan\\form\\%s.pln"), ca_cthome, sz_satname);
//	}
#endif

	stpd.l_mod_sel = 0;
	stpd.l_tx_pwr_sel = 0;
	stpd.l_tx_mode = 0;
	stpd.l_route_mode = 0;
	stpd.l_term_ant = 0;
	stpd.l_offset = 0;
	stpd.l_auto_acq = 0;
	stpd.l_cmd_mod = 0;
	stpd.l_rng_mod = 0;
	stpd.l_delay_sel = 0;
	stpd.l_pl_sel = 0;
	stpd.l_top_mon = 0;
	stpd.l_ent_angle = 0;
	stpd.l_ext_angle = 0;
	stpd.l_pl_sel = 0;
	stpd.l_range_band = 0;
	stpd.l_acq_route = 0;
	stpd.sz_priority = _T("");

	/*-------------------------------------------------------------------------*/
	/*-----   雛形ファイル読込処理                                        -----*/
	/*-------------------------------------------------------------------------*/
	if (_wfopen_s(&fp_smpl, sz_s_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("計画雛形ファイルを開くことができません。\n%s"), (LPCTSTR)sz_s_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	char sz_line[512];

	/*---------- コメント用計画内容ファイル出力処理 ----------*/
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_smpl) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));

		if (sz_buff[0] == '#')
		{
			continue;
		}

		l = 0;
		if (sz_buff[0] == 'N')
		{
			if (sz_buff[1] >= 0x30 && sz_buff[1] <= 0x39)
			{
				l_ncount = sz_buff[1] - 0x30;
				k = 2;
			}
			else
			{
				l_ncount = 0;
				k = 1;
			}
		}
		else
		{
			l_ncount = -1;
			k = 0;
		}
		for (; sz_buff[k] != '\0'; k++)
		{
			if (sz_buff[k] != ' ')
			{
				sz_time[l] = sz_buff[k];
				l++;
			}
			else
			{
				if (l > 0)
				{
					sz_time[l] = '\0';
					break;
				}
			}
		}

		j = 0;
		l = 0;
		sz_ele[2][0] = '\0';
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] == ',')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_buff[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				sz_ele[j][l] = '\0';
				l = 0;
				break;
			}
			else if (sz_buff[k] != ' ')
			{
				sz_ele[j][l] = sz_buff[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			continue;
		}

		/*----- S-TX 送信出力選択処理 -----*/
		if (sz_c_ele64[31] == CString(sz_ele[0]))
		{
			for (i = 0; sz_tx_pwr_sel64[i] != _T(""); ++i)
			{
				if (sz_tx_pwr_sel64[i] == CString(sz_ele[1]))
				{
					stpd.l_tx_pwr_sel = i;
					break;
				}
			}
		}

		/*----- X-TX 送信出力選択処理 -----*/
		if (sz_c_ele64[32] == CString(sz_ele[0]))
		{
			for (i = 0; sz_tx_pwr_sel64[i] != _T(""); ++i)
			{
				if (sz_tx_pwr_sel64[i] == CString(sz_ele[1]))
				{
					stpd.l_tx_pwr_sel = i;
					break;
				}
			}
		}

		/*----- Ｘ帯変調装置選択処理 -----*/
		if (sz_c_ele64[35] == CString(sz_ele[0]))
		{
			for (i = 0; sz_mod_sel[i] != _T(""); i++) 
			{
				if (sz_mod_sel[i] == CString(sz_ele[1]))
				{
					stpd.l_mod_sel = i;
					break;
				}
			}
		}

		/* 送信出力がX-TX */
		if (sz_c_ele64[50] == CString(sz_ele[0]))
		{
			stpd.l_tx_mode = 1;
		}

		/* X帯送受信ルート判定文字列を探す */
		if (CString(sz_ele[0]).Find(X_ROUTE_DEF_TXT) >= 0)
		{
			stpd.l_route_mode = 1;
		}


	}

	fclose(fp_smpl);
	return 0;
}

/*============================================================================*/
/*! COperationPlan

-# 計画ファイルを読み込む

@param  なし
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
int COperationPlan::ReadPlanFile20(CString sz_satname, CString sz_passno, stPlandata& stpd, DWORD dw)
{
	FILE	*fp_plan;
	CString sz_p_file;

	TCHAR sz_s_start[50];
	TCHAR sz_buff[512];
	TCHAR sz_work[256];
	TCHAR sz_date[50] = { 0 };
	TCHAR sz_time[50] = { 0 };
	TCHAR sz_txrfpwr[3][20];

	TCHAR sz_work2[256];
	TCHAR sz_work3[256];
	TCHAR sz_work4[256];
	TCHAR sz_work5[256];
	TCHAR sz_work6[256];
	TCHAR sz_work7[256];
	TCHAR sz_work8[256];

	long i;
	long j;
	long l;
	long k;
	long l_rc;

	__int64 d_s_start;
	__int64 d_time;

	CString str;
	CString sz_txcomp;
	CString strTime;

	TCHAR sz_ele[10][100];
	memset(&sz_ele[0], 0, static_cast<unsigned>(_countof(sz_ele)));


	CSatelliteData& sd = theApp.GetSatelliteData();

#if 1
	CString filename = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
	if (dw == 1)
		sz_p_file.Format(_T("%senp\\%s\\%s.pln"), (LPCTSTR)filename, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	else
		sz_p_file.Format(_T("%ssttn\\%s\\%s.pln"), (LPCTSTR)filename, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
#else
//	j = (long)strlen(sd.m_satinfo[m_IdxEqsat].sz_plan_dir);
//	if (sd.m_satinfo[m_IdxEqsat].sz_plan_dir[j - 1] == '/')
//	{
//		sz_p_file.Format(_T("%s%s.pln"), CString(sd.m_satinfo[m_IdxEqsat].sz_plan_dir), sz_passno);
//	}
//	else
//	{
//		sz_p_file.Format(_T("%s\\%s.pln"), CString(sd.m_satinfo[m_IdxEqsat].sz_plan_dir), sz_passno);
//	}
#endif
	sz_s_start[0] = '\0';


	/*----- ftp file get  "sz_p_file" -----*/
	CString path(sd.m_satinfo[m_IdxEqsat].sz_plan_dir);
	CString file(sz_p_file);

	//	if (GetPlanFile(path, file) == FALSE)
	//	{
	//		return -1;
	//	}

	if (_wfopen_s(&fp_plan, sz_p_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("計画ファイルを開くことができません。\n%s"), (LPCTSTR)sz_p_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	stpd.sz_start = _T("");
	stpd.sz_end = _T("");
	stpd.sz_s_bitrate = _T("");
	stpd.sz_x_bitrate = _T("");
	stpd.sz_x_qpsk = _T("");
	stpd.sz_cmd_bit_rate = _T("");
	stpd.sz_x_cmd_bit_rate = _T("");
	stpd.l_cmd_mod = 0;
	stpd.l_rng_mod = 0;
	stpd.sz_rng_band = _T("");
	stpd.sz_rng_start = _T("");
	stpd.sz_rng_end = _T("");
	stpd.l_rdychk = 0;
	stpd.l_init = 0;
	stpd.l_auto_acq = 0;
	stpd.l_offset = 0;
	stpd.l_a_flag = 0;
	stpd.l_l_flag = 0;
	stpd.l_term_ant = 1;
	stpd.l_tx_ramp = 0;
	stpd.l_rx_ramp = 0;
	stpd.l_acq_route = 0;
	stpd.l_pl_sel = 0;
	stpd.l_top_mon = 0;
	stpd.l_ent_angle = 0;
	stpd.l_ext_angle = 0;
	stpd.sz_level_s = _T("");
	stpd.sz_level_x = _T("");
	stpd.sz_ant_s = _T("");
	stpd.sz_ant_x = _T("");
	stpd.l_delay_sel = 0;
	stpd.l_tx_pwr_sel = 0;
	stpd.l_mod_sel = 0;
	stpd.l_tx_uc_sel = -1;
	stpd.l_antwait_flg = 0;
	stpd.l_cmd_index = 0;
	stpd.l_tlm_index = 0;
	stpd.l_agc_index = 0;
	stpd.l_rng_index = 0;
	stpd.l_range_band = 0;
	stpd.l_ant_stow = 2;
	stpd.sz_cid = _T("");
	stpd.sz_station = _T("");
	stpd.sz_priority = _T("");
	stpd.sz_ope_mode = _T("");
	stpd.l_uplink = 0;
	stpd.sz_uplink_s_timelst.clear();
	stpd.sz_uplink_e_timelst.clear();
	stpd.l_rng = 0;
	stpd.sz_rng_s_timelst.clear();
	stpd.sz_rng_e_timelst.clear();
	stpd.l_cmd = 0;
	stpd.sz_cmd_s_timelst.clear();
	stpd.sz_cmd_e_timelst.clear();
	stpd.l_tlms = 0;
	stpd.sz_tlms_s_timelst.clear();
	stpd.sz_tlms_e_timelst.clear();
	stpd.l_tlmx = 0;
	stpd.sz_tlmx_s_timelst.clear();
	stpd.sz_tlmx_e_timelst.clear();
	stpd.l_tlmka = 0;
	stpd.sz_tlmka_s_timelst.clear();
	stpd.sz_tlmka_e_timelst.clear();

	__int64 d_tm;

	d_s_start = 0;
	d_time = 0;

	char sz_line[512];
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_plan) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			if (wcsncmp(sz_buff, SOS_HEADER_ID, wcslen(SOS_HEADER_ID)) == 0)
			{
				swscanf_s(sz_buff, _T("%s%s%s%s%s%s%s%s"),
					sz_work, static_cast<unsigned>(_countof(sz_work)),
					sz_work2, static_cast<unsigned>(_countof(sz_work2)),
					sz_work3, static_cast<unsigned>(_countof(sz_work3)),
					sz_work4, static_cast<unsigned>(_countof(sz_work4)),
					sz_work5, static_cast<unsigned>(_countof(sz_work5)),
					sz_work6, static_cast<unsigned>(_countof(sz_work6)),
					sz_work7, static_cast<unsigned>(_countof(sz_work7)),
					sz_work8, static_cast<unsigned>(_countof(sz_work8)));
			}

			l_rc = swscanf_s(&sz_buff[1], _T("%s%s%s%s%s%s%s%s%s%s"),
				sz_ele[0], static_cast<unsigned>(_countof(sz_ele[0])),
				sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])),
				sz_ele[2], static_cast<unsigned>(_countof(sz_ele[2])),
				sz_ele[3], static_cast<unsigned>(_countof(sz_ele[3])),
				sz_ele[4], static_cast<unsigned>(_countof(sz_ele[4])),
				sz_ele[5], static_cast<unsigned>(_countof(sz_ele[5])),
				sz_ele[6], static_cast<unsigned>(_countof(sz_ele[6])),
				sz_ele[7], static_cast<unsigned>(_countof(sz_ele[7])),
				sz_ele[8], static_cast<unsigned>(_countof(sz_ele[8])),
				sz_ele[9], static_cast<unsigned>(_countof(sz_ele[9])));

			for (i = 0; i < l_rc; ++i)
			{
				str = CString(sz_ele[i]);
				/*---------- 初期値 チェック ----------*/
				if (str.Find(sz_c_ele20[2]) >= 0)
				{
					j = sz_c_ele20[2].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_init = k;
							break;
						}
					}
				}

				/*---------- CMD MOD の AUTO/MANチェック ----------*/
				if (str.Find(sz_c_ele20[6]) >= 0)
				{
					j = sz_c_ele20[6].GetLength() + 1;
					for (k = 0; sz_auto[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_auto[k])
						{
							stpd.l_cmd_mod = k;
							break;
						}
					}
				}
				/*---------- RNG MOD の AUTO/MANチェック ----------*/
				if (str.Find(sz_c_ele20[7]) >= 0)
				{
					j = sz_c_ele20[7].GetLength() + 1;
					for (k = 0; sz_auto[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_auto[k])
						{
							stpd.l_rng_mod = k;
							break;
						}
					}
				}
				/*---------- RNG BAND の 取り込み ----------*/
				if (str.Find(sz_c_ele20[8]) >= 0)
				{
					j = sz_c_ele20[8].GetLength() + 1;
					stpd.sz_rng_band = CString(&sz_ele[i][j]);
				}
				/*---------- RNG START の 取り込み ----------*/
				if (str.Find(sz_c_ele20[9]) >= 0)
				{
					j = sz_c_ele20[9].GetLength() + 1;
					stpd.sz_rng_start = CString(&sz_ele[i][j]);
				}
				/*---------- RNG END の 取り込み ----------*/
				if (str.Find(sz_c_ele20[10]) >= 0)
				{
					j = sz_c_ele20[10].GetLength() + 1;
					stpd.sz_rng_end = CString(&sz_ele[i][j]);
				}
				/*---------- 時刻オフセットへの追従 ----------*/
				if (str.Find(sz_c_ele20[11]) >= 0)
				{
					j = sz_c_ele20[11].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_offset = k;
							break;
						}
					}
				}
				/*---------- AOS FLAG ----------*/
				if (str.Find(sz_c_ele20[14]) >= 0)
				{
					j = sz_c_ele20[14].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_a_flag = k;
							break;
						}
					}
				}
				/*---------- LOS FLAG ----------*/
				if (str.Find(sz_c_ele20[15]) >= 0)
				{
					j = sz_c_ele20[15].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_l_flag = k;
							break;
						}
					}
				}
				/*---------- 自動捕捉の有無チェック ----------*/
				if (str.Find(sz_c_ele20[16]) >= 0)
				{
					j = sz_c_ele20[16].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_auto_acq = k;
							break;
						}
					}
				}
				/*---------- パス終了時のＡＮＴモード ----------*/
				if (str.Find(sz_c_ele20[18]) >= 0)
				{
					j = sz_c_ele20[18].GetLength() + 1;
					for (k = 0; sz_term_ant[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_term_ant[k])
						{
							stpd.l_term_ant = k;
							break;
						}
					}
				}
				/*---------- 送信設備ランプ制御有無 ----------*/
				if (str.Find(sz_c_ele20[19]) >= 0)
				{
					j = sz_c_ele20[19].GetLength() + 1;
					for (k = 0; sz_tx_ramp[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_tx_ramp[k])
						{
							stpd.l_tx_ramp = k;
							break;
						}
					}
				}
				/*---------- 受信設備ランプ制御有無 ----------*/
				if (str.Find(sz_c_ele20[20]) >= 0)
				{
					j = sz_c_ele20[20].GetLength() + 1;
					for (k = 0; sz_rx_ramp[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_rx_ramp[k])
						{
							stpd.l_rx_ramp = k;
							break;
						}
					}
				}
				/*---------- 補足ルート ----------*/
				if (str.Find(sz_c_ele20[21]) >= 0)
				{
					j = sz_c_ele20[21].GetLength() + 1;
					stpd.l_acq_route = _wtoi(&sz_ele[i][j]);
					stpd.l_acq_route -= 1;	/* 配列番号に変換 */
				}
				/*---------- ACQ/MAIN判定有無 ----------*/
				if (str.Find(sz_c_ele20[22]) >= 0)
				{
					j = sz_c_ele20[22].GetLength() + 1;
					for (k = 0; sz_on_off[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on_off[k])
						{
							stpd.l_pl_sel = k;
							break;
						}
					}
				}
				/*---------- 天頂通過監視有無 ----------*/
				if (str.Find(sz_c_ele20[23]) >= 0)
				{
					j = sz_c_ele20[23].GetLength() + 1;
					for (k = 0; sz_on_off[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on_off[k])
						{
							stpd.l_top_mon = k;
							break;
						}
					}
				}
				/*---------- 天頂通過監視入角 ----------*/
				if (str.Find(sz_c_ele20[24]) >= 0)
				{
					j = sz_c_ele20[24].GetLength() + 1;
					stpd.l_ent_angle = _wtoi(&sz_ele[i][j]);
				}
				/*---------- 天頂通過監視出角 ----------*/
				if (str.Find(sz_c_ele20[25]) >= 0)
				{
					j = sz_c_ele20[25].GetLength() + 1;
					stpd.l_ext_angle = _wtoi(&sz_ele[i][j]);
				}
				/*---------- 再生測距対応　局内ディレイ値選択 ----------*/
				if (str.Find(sz_c_ele20[30]) >= 0)
				{
					j = sz_c_ele20[30].GetLength() + 1;
					for (k = 0; sz_delay_sel[k] != _T(""); ++k) {
						if (CString(&sz_ele[i][j]) == sz_delay_sel[k])
						{
							stpd.l_delay_sel = k;
							break;
						}
					}
				}
				/*---------- S帯RNG計測帯域 ----------*/
				if (str.Find(sz_c_ele20[42]) >= 0)
				{
					j = sz_c_ele20[42].GetLength() + 1;
					for (k = 0; sz_range_band[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_range_band[k])
						{
							stpd.l_range_band = k;
							break;
						}
					}
				}
				/*---------- ANT天頂指向有無 ----------*/
				if (str.Find(sz_c_ele20[43]) >= 0)
				{
					j = sz_c_ele20[43].GetLength() + 1;
					for (k = 0; sz_ant_stow[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_ant_stow[k])
						{
							stpd.l_ant_stow = k;
							break;
						}
					}
				}

				/*---------- CID ----------*/
				if (str.Find(sz_c_ele20[49]) >= 0)
				{
					j = sz_c_ele20[49].GetLength() + 1;
					stpd.sz_cid = CString(&sz_ele[i][j]);
				}

				/*---------- 局名 ----------*/
				if (str.Find(sz_c_ele20[50]) >= 0)
				{
					j = sz_c_ele20[50].GetLength() + 1;
					stpd.sz_station = CString(&sz_ele[i][j]);
				}

				/*---------- 主従局 ----------*/
				if (str.Find(sz_c_ele20[51]) >= 0)
				{
					j = sz_c_ele20[51].GetLength() + 1;
					stpd.sz_priority = CString(&sz_ele[i][j]);
				}

				/*---------- オペレーションモード ----------*/
				if (str.Find(sz_c_ele20[52]) >= 0)
				{
					j = sz_c_ele20[52].GetLength() + 1;
					stpd.sz_ope_mode = CString(&sz_ele[i][j]);
				}

			}
			continue;
		}

		swscanf_s(sz_buff, _T("%s%s%s"),
			sz_date, static_cast<unsigned>(_countof(sz_date)),
			sz_time, static_cast<unsigned>(_countof(sz_time)),
			sz_work, static_cast<unsigned>(_countof(sz_work)));

		j = 0;
		l = 0;
		for (k = 0; sz_work[k] != '\0'; ++k)
		{
			if (sz_work[k] == ',')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_work[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
				break;
			}
			else if (sz_work[k] != ' ')
			{
				sz_ele[j][l] = sz_work[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			continue;
		}

		/* X帯送受信ルート判定文字列を探す */
		str = CString(sz_ele[0]);
		if (str.Find(X_ROUTE_DEF_TXT) >= 0)
		{
			stpd.l_route_mode = 1;
		}

		for (k = 0; sz_c_ele20[k] != _T(""); ++k)
		{
			if ((k == 2) || (k == 4) || (k == 35))
			{	/* 判定ケースを追加 */
				if (sz_c_ele20[k].Find(str) >= 0)
				{
					break;
				}
			}
			else
			{
				if (str == sz_c_ele20[k])
				{
					break;
				}
			}
		}

		if (sz_c_ele20[k] == _T(""))
		{
			continue;
		}

		swprintf_s(sz_work, static_cast<unsigned>(_countof(sz_work)), _T("%s %s"), sz_date, sz_time);
		if (d_s_start == 0.0 || d_s_start > d_time)
		{
			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), _T(" "));
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_time);
			d_s_start = d_time;
		}

		switch (k)
		{
			/*----- レディネスチェック処理 -----*/
		case 0:
			(stpd.l_rdychk) |= 0x01;
			break;

		case 1:
			(stpd.l_rdychk) |= 0x02;
			break;

			/*----- 初期設定処理 -----*/
		case 2:
			stpd.l_init = 1;
			break;

			/*----- ビットレートＳ -----*/
		case 3:
			stpd.sz_s_bitrate = CString(sz_ele[1]);
			break;

			/*----- ビットレートＸ -----*/
		case 4:
			stpd.sz_x_bitrate = CString(sz_ele[1]);
			break;

			/*----- ビットレートＸＱＰＳＫ -----*/
		case 17:
			if (CString(sz_ele[0]) == BIT_XQPSK20)
			{
				stpd.sz_x_qpsk = CString(sz_ele[1]);
				break;
			}
			else 
			{
				stpd.sz_x_bitrate = CString(sz_ele[1]);
				break;
			}
			break;

			/*----- S/X コマンドビットレート -----*/
		case 5:
			stpd.sz_cmd_bit_rate = CString(sz_ele[1]);
			break;

			/*----- X コマンドビットレート -----*/
		case 35:
			stpd.sz_x_cmd_bit_rate = CString(sz_ele[1]);
			break;

			/*----- 開始時刻処理 -----*/
		case 12:
			stpd.sz_start.Format(_T("%s %s"), sz_date, sz_time);

			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), _T(" "));
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_time);

			TCHAR sz_lwork[50];
			__int64 d_work;
			__int64 l_tm;
			swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s.0000"), sz_s_start);

			d_work = CSatelliteData::StrTimeToCTime(CString(sz_lwork)).GetTime();
			l_tm = d_work - d_tm;
			stpd.l_ant_time = (long)(l_tm / 60);
			break;

			/*----- 終了時刻処理 -----*/
		case 13:
			stpd.sz_end.Format(_T("%s %s"), sz_date, sz_time);
			break;

			/*----- S-TX 送信出力選択処理 -----*/
		case 32:
			/* 送信機-送信出力-S帯送信周波数変換盤を解析 */
			memset(sz_txrfpwr, 0, sizeof(sz_txrfpwr));

			swscanf_s(sz_ele[1], _T("%[^_]_%[^_]_%[^_]"),
				sz_txrfpwr[0], static_cast<unsigned>(_countof(sz_txrfpwr[0])),
				sz_txrfpwr[1], static_cast<unsigned>(_countof(sz_txrfpwr[1])),
				sz_txrfpwr[2], static_cast<unsigned>(_countof(sz_txrfpwr[2])));

			sz_txcomp.Format(_T("%s_%s"), sz_txrfpwr[0], sz_txrfpwr[1]);
			for (i = 0; sz_tx_pwr_sel20[i] != _T(""); ++i)
			{
				if (sz_tx_pwr_sel20[i] == sz_txcomp)
				{
					stpd.l_tx_pwr_sel = i;
					break;
				}
			}
			/* S帯送信周波数変換盤 追加 */
			stpd.l_tx_uc_sel = i;
			break;

		case 36: //レディネスチェック（AGC)
			if ((CString(sz_ele[1]).Find(STR_SX_ACQRX_S) >= 0) ||
				(CString(sz_ele[1]).Find(STR_S_RX) >= 0) ||
				(CString(sz_ele[1]).Find(STR_SS_TCRDET) >= 0) ||
				(CString(sz_ele[1]).Find(STR_X_RX) >= 0) ||
				(CString(sz_ele[1]) == STR_X_QPSKRX_A))
			{
				stpd.l_agc_index = 1;
			}
			break;

		case 37: //レディネスチェック（RNG)
			if ((CString(sz_ele[1]) == STR_SS) ||
				(CString(sz_ele[1]) == STR_SX))
			{
				stpd.l_rng_index = 1;
			}
			break;

		case 38: //レディネスチェック（テレメトリ伝送）
			if ((CString(sz_ele[1]) == STR_S) ||
				(CString(sz_ele[1]) == STR_X) ||
				(CString(sz_ele[1]) == STR_QPSK))
			{
				stpd.l_tlm_index = 1;
			}
			break;

		case 39: //レディネスチェック（コマンド伝送）
			if (CString(sz_ele[1]) == STR_1)
			{
				stpd.l_cmd_index = 1;
			}
			break;

		case 40: //アンテナ待ち受け指向
			stpd.l_antwait_flg = 1;
			break;

		case 41:
			if (CString(sz_ele[1]).Find(STR_ANT) >= 0)
			{
				TCHAR sz_lwork[50];

				swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s %s.0000"), sz_date, sz_time);
				d_tm = CSatelliteData::StrTimeToCTime(CString(sz_lwork)).GetTime();
			}
			break;

		case 55: // UP START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_uplink_s_timelst.push_back(CString(strTime));
			break;

		case 56: // UP END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_uplink_e_timelst.push_back(CString(strTime));
			break;

		case 57: // RNG_START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_rng_s_timelst.push_back(CString(strTime));

			// 種別
			stpd.sz_rng_kindlst.push_back(CString(sz_ele[1]));
			break;

		case 58: // RNG_END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_rng_e_timelst.push_back(CString(strTime));
			break;

		case 59: // CMD_START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_cmd_s_timelst.push_back(CString(strTime));
			break;

		case 60: // CMD_END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_cmd_e_timelst.push_back(CString(strTime));
			break;

		case 61: // TLM_START
			if (CString(sz_ele[1]) == TLM_S)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlms_s_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_X)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmx_s_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_KA)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmka_s_timelst.push_back(CString(strTime));
			}
			break;

		case 62: // TLM_END
			if (CString(sz_ele[1]) == TLM_S)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlms_e_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_X)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmx_e_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_KA)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmka_e_timelst.push_back(CString(strTime));
			}

			break;

		}
	}

	fclose(fp_plan);

	return 0;
}

int COperationPlan::ReadPlanFile34(CString sz_satname, CString sz_passno, stPlandata& stpd, DWORD dw)
{
	FILE	*fp_plan;
	CString sz_p_file;

	TCHAR sz_s_start[50];
	TCHAR sz_buff[512];
	TCHAR sz_work[256];
	TCHAR sz_date[50] = { 0 };
	TCHAR sz_time[50] = { 0 };
	TCHAR sz_txrfpwr[3][20];

	TCHAR sz_work2[256];
	TCHAR sz_work3[256];
	TCHAR sz_work4[256];
	TCHAR sz_work5[256];
	TCHAR sz_work6[256];
	TCHAR sz_work7[256];
	TCHAR sz_work8[256];

	long i;
	long j;
	long l;
	long k;
	long l_rc;

	__int64 d_s_start;
	__int64 d_time;

	CString str;
	CString sz_txcomp;
	CString strTime;

	TCHAR sz_ele[10][100];
	memset(&sz_ele[0], 0, static_cast<unsigned>(_countof(sz_ele)));


	CSatelliteData& sd = theApp.GetSatelliteData();

#if 1
	CString filename = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
	if (dw == 1)
		sz_p_file.Format(_T("%senp\\%s\\%s.pln"), (LPCTSTR)filename, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	else
		sz_p_file.Format(_T("%ssttn\\%s\\%s.pln"), (LPCTSTR)filename, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
#else
//	j = (long)strlen(sd.m_satinfo[m_IdxEqsat].sz_plan_dir);
//	if (sd.m_satinfo[m_IdxEqsat].sz_plan_dir[j - 1] == '/')
//	{
//		sz_p_file.Format(_T("%s%s.pln"), CString(sd.m_satinfo[m_IdxEqsat].sz_plan_dir), sz_passno);
//	}
//	else
//	{
//		sz_p_file.Format(_T("%s\\%s.pln"), CString(sd.m_satinfo[m_IdxEqsat].sz_plan_dir), sz_passno);
//	}
#endif
	sz_s_start[0] = '\0';


	/*----- ftp file get  "sz_p_file" -----*/
	CString path(sd.m_satinfo[m_IdxEqsat].sz_plan_dir);
	CString file(sz_p_file);

//	if (GetPlanFile(path, file) == FALSE)
//	{
//		return -1;
//	}

	if (_wfopen_s(&fp_plan, sz_p_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("計画ファイルを開くことができません。\n%s"), (LPCTSTR)sz_p_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	stpd.sz_start = _T("");
	stpd.sz_end = _T("");
	stpd.sz_s_bitrate = _T("");
	stpd.sz_x_bitrate = _T("");
	stpd.sz_x_qpsk = _T("");
	stpd.sz_cmd_bit_rate = _T("");
	stpd.sz_x_cmd_bit_rate = _T("");
	stpd.l_cmd_mod = 0;
	stpd.l_rng_mod = 0;
	stpd.sz_rng_band = _T("");
	stpd.sz_rng_start = _T("");
	stpd.sz_rng_end = _T("");
	stpd.l_rdychk = 0;
	stpd.l_init = 0;
	stpd.l_auto_acq = 0;
	stpd.l_offset = 0;
	stpd.l_a_flag = 0;
	stpd.l_l_flag = 0;
	stpd.l_term_ant = 1;
	stpd.l_tx_ramp = 0;
	stpd.l_rx_ramp = 0;
	stpd.l_acq_route = 0;
	stpd.l_pl_sel = 0;
	stpd.l_top_mon = 0;
	stpd.l_ent_angle = 0;
	stpd.l_ext_angle = 0;
	stpd.sz_level_s = _T("");
	stpd.sz_level_x = _T("");
	stpd.sz_ant_s = _T("");
	stpd.sz_ant_x = _T("");
	stpd.l_delay_sel = 0;
	stpd.l_tx_pwr_sel = 0;
	stpd.l_mod_sel = 0;
	stpd.l_tx_uc_sel = -1;
	stpd.l_antwait_flg = 0;
	stpd.l_cmd_index = 0;
	stpd.l_tlm_index = 0;
	stpd.l_agc_index = 0;
	stpd.l_rng_index = 0;
	stpd.l_range_band = 0;
	stpd.l_ant_stow = 2;
	stpd.sz_cid = _T("");
	stpd.sz_station = _T("");
	stpd.sz_priority = _T("");
	stpd.sz_ope_mode = _T("");
	stpd.l_uplink = 0;
	stpd.sz_uplink_s_timelst.clear();
	stpd.sz_uplink_e_timelst.clear();
	stpd.l_rng = 0;
	stpd.sz_rng_s_timelst.clear();
	stpd.sz_rng_e_timelst.clear();
	stpd.l_cmd = 0;
	stpd.sz_cmd_s_timelst.clear();
	stpd.sz_cmd_e_timelst.clear();
	stpd.l_tlms = 0;
	stpd.sz_tlms_s_timelst.clear();
	stpd.sz_tlms_e_timelst.clear();
	stpd.l_tlmx = 0;
	stpd.sz_tlmx_s_timelst.clear();
	stpd.sz_tlmx_e_timelst.clear();
	stpd.l_tlmka = 0;
	stpd.sz_tlmka_s_timelst.clear();
	stpd.sz_tlmka_e_timelst.clear();

	__int64 d_tm;

	d_s_start = 0;
	d_time = 0;

	char sz_line[512];
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_plan) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			if (wcsncmp(sz_buff, SOS_HEADER_ID, wcslen(SOS_HEADER_ID)) == 0)
			{
				swscanf_s(sz_buff, _T("%s%s%s%s%s%s%s%s"),
					sz_work, static_cast<unsigned>(_countof(sz_work)),
					sz_work2, static_cast<unsigned>(_countof(sz_work2)),
					sz_work3, static_cast<unsigned>(_countof(sz_work3)),
					sz_work4, static_cast<unsigned>(_countof(sz_work4)),
					sz_work5, static_cast<unsigned>(_countof(sz_work5)),
					sz_work6, static_cast<unsigned>(_countof(sz_work6)),
					sz_work7, static_cast<unsigned>(_countof(sz_work7)),
					sz_work8, static_cast<unsigned>(_countof(sz_work8)));
			}

			l_rc = swscanf_s(&sz_buff[1], _T("%s%s%s%s%s%s%s%s%s%s"),
				sz_ele[0], static_cast<unsigned>(_countof(sz_ele[0])),
				sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])),
				sz_ele[2], static_cast<unsigned>(_countof(sz_ele[2])),
				sz_ele[3], static_cast<unsigned>(_countof(sz_ele[3])),
				sz_ele[4], static_cast<unsigned>(_countof(sz_ele[4])),
				sz_ele[5], static_cast<unsigned>(_countof(sz_ele[5])),
				sz_ele[6], static_cast<unsigned>(_countof(sz_ele[6])),
				sz_ele[7], static_cast<unsigned>(_countof(sz_ele[7])),
				sz_ele[8], static_cast<unsigned>(_countof(sz_ele[8])),
				sz_ele[9], static_cast<unsigned>(_countof(sz_ele[9])));

			for (i = 0; i < l_rc; ++i)
			{
				str = CString(sz_ele[i]);
				/*---------- 初期値 チェック ----------*/
				if (str.Find(sz_c_ele34[2]) >= 0)
				{
					j = sz_c_ele34[2].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_init = k;
							break;
						}
					}
				}

				/*---------- CMD MOD の AUTO/MANチェック ----------*/
				if (str.Find(sz_c_ele34[6]) >= 0)
				{
					j = sz_c_ele34[6].GetLength() + 1;
					for (k = 0; sz_auto[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_auto[k])
						{
							stpd.l_cmd_mod = k;
							break;
						}
					}
				}
				/*---------- RNG MOD の AUTO/MANチェック ----------*/
				if (str.Find(sz_c_ele34[7]) >= 0)
				{
					j = sz_c_ele34[7].GetLength() + 1;
					for (k = 0; sz_auto[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_auto[k])
						{
							stpd.l_rng_mod = k;
							break;
						}
					}
				}
				/*---------- RNG BAND の 取り込み ----------*/
				if (str.Find(sz_c_ele34[8]) >= 0)
				{
					j = sz_c_ele34[8].GetLength() + 1;
					stpd.sz_rng_band = CString(&sz_ele[i][j]);
				}
				/*---------- RNG START の 取り込み ----------*/
				if (str.Find(sz_c_ele34[9]) >= 0)
				{
					j = sz_c_ele34[9].GetLength() + 1;
					stpd.sz_rng_start = CString(&sz_ele[i][j]);
				}
				/*---------- RNG END の 取り込み ----------*/
				if (str.Find(sz_c_ele34[10]) >= 0)
				{
					j = sz_c_ele34[10].GetLength() + 1;
					stpd.sz_rng_end = CString(&sz_ele[i][j]);
				}
				/*---------- 時刻オフセットへの追従 ----------*/
				if (str.Find(sz_c_ele34[11]) >= 0)
				{
					j = sz_c_ele34[11].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_offset = k;
							break;
						}
					}
				}
				/*---------- AOS FLAG ----------*/
				if (str.Find(sz_c_ele34[14]) >= 0)
				{
					j = sz_c_ele34[14].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_a_flag = k;
							break;
						}
					}
				}
				/*---------- LOS FLAG ----------*/
				if (str.Find(sz_c_ele34[15]) >= 0)
				{
					j = sz_c_ele34[15].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_l_flag = k;
							break;
						}
					}
				}
				/*---------- 自動捕捉の有無チェック ----------*/
				if (str.Find(sz_c_ele34[16]) >= 0)
				{
					j = sz_c_ele34[16].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_auto_acq = k;
							break;
						}
					}
				}
				/*---------- パス終了時のＡＮＴモード ----------*/
				if (str.Find(sz_c_ele34[18]) >= 0)
				{
					j = sz_c_ele34[18].GetLength() + 1;
					for (k = 0; sz_term_ant[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_term_ant[k])
						{
							stpd.l_term_ant = k;
							break;
						}
					}
				}
				/*---------- 送信設備ランプ制御有無 ----------*/
				if (str.Find(sz_c_ele34[19]) >= 0)
				{
					j = sz_c_ele34[19].GetLength() + 1;
					for (k = 0; sz_tx_ramp[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_tx_ramp[k])
						{
							stpd.l_tx_ramp = k;
							break;
						}
					}
				}
				/*---------- 受信設備ランプ制御有無 ----------*/
				if (str.Find(sz_c_ele34[20]) >= 0)
				{
					j = sz_c_ele34[20].GetLength() + 1;
					for (k = 0; sz_rx_ramp[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_rx_ramp[k])
						{
							stpd.l_rx_ramp = k;
							break;
						}
					}
				}
				/*---------- 補足ルート ----------*/
				if (str.Find(sz_c_ele34[21]) >= 0)
				{
					j = sz_c_ele34[21].GetLength() + 1;
					stpd.l_acq_route = _wtoi(&sz_ele[i][j]);
					stpd.l_acq_route -= 1;	/* 配列番号に変換 */
				}
				/*---------- ACQ/MAIN判定有無 ----------*/
				if (str.Find(sz_c_ele34[22]) >= 0)
				{
					j = sz_c_ele34[22].GetLength() + 1;
					for (k = 0; sz_on_off[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on_off[k])
						{
							stpd.l_pl_sel = k;
							break;
						}
					}
				}
				/*---------- 天頂通過監視有無 ----------*/
				if (str.Find(sz_c_ele34[23]) >= 0)
				{
					j = sz_c_ele34[23].GetLength() + 1;
					for (k = 0; sz_on_off[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on_off[k])
						{
							stpd.l_top_mon = k;
							break;
						}
					}
				}
				/*---------- 天頂通過監視入角 ----------*/
				if (str.Find(sz_c_ele34[24]) >= 0)
				{
					j = sz_c_ele34[24].GetLength() + 1;
					stpd.l_ent_angle = _wtoi(&sz_ele[i][j]);
				}
				/*---------- 天頂通過監視出角 ----------*/
				if (str.Find(sz_c_ele34[25]) >= 0)
				{
					j = sz_c_ele34[25].GetLength() + 1;
					stpd.l_ext_angle = _wtoi(&sz_ele[i][j]);
				}
				/*---------- 再生測距対応　局内ディレイ値選択 ----------*/
				if (str.Find(sz_c_ele34[30]) >= 0)
				{
					j = sz_c_ele34[30].GetLength() + 1;
					for (k = 0; sz_delay_sel[k] != _T(""); ++k) {
						if (CString(&sz_ele[i][j]) == sz_delay_sel[k])
						{
							stpd.l_delay_sel = k;
							break;
						}
					}
				}
				/*---------- S帯RNG計測帯域 ----------*/
				if (str.Find(sz_c_ele34[42]) >= 0)
				{
					j = sz_c_ele34[42].GetLength() + 1;
					for (k = 0; sz_range_band[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_range_band[k])
						{
							stpd.l_range_band = k;
							break;
						}
					}
				}
				/*---------- ANT天頂指向有無 ----------*/
				if (str.Find(sz_c_ele34[43]) >= 0)
				{
					j = sz_c_ele34[43].GetLength() + 1;
					for (k = 0; sz_ant_stow[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_ant_stow[k])
						{
							stpd.l_ant_stow = k;
							break;
						}
					}
				}

				/*---------- CID ----------*/
				if (str.Find(sz_c_ele34[51]) >= 0)
				{
					j = sz_c_ele34[51].GetLength() + 1;
					stpd.sz_cid = CString(&sz_ele[i][j]);
				}

				/*---------- 局名 ----------*/
				if (str.Find(sz_c_ele34[52]) >= 0)
				{
					j = sz_c_ele34[52].GetLength() + 1;
					stpd.sz_station = CString(&sz_ele[i][j]);
				}

				/*---------- 主従局 ----------*/
				if (str.Find(sz_c_ele34[53]) >= 0)
				{
					j = sz_c_ele34[53].GetLength() + 1;
					stpd.sz_priority = CString(&sz_ele[i][j]);
				}

				/*---------- オペレーションモード ----------*/
				if (str.Find(sz_c_ele34[54]) >= 0)
				{
					j = sz_c_ele34[54].GetLength() + 1;
					stpd.sz_ope_mode = CString(&sz_ele[i][j]);
				}

			}
			continue;
		}

		swscanf_s(sz_buff, _T("%s%s%s"),
			sz_date, static_cast<unsigned>(_countof(sz_date)),
			sz_time, static_cast<unsigned>(_countof(sz_time)),
			sz_work, static_cast<unsigned>(_countof(sz_work)));

		j = 0;
		l = 0;
		for (k = 0; sz_work[k] != '\0'; ++k)
		{
			if (sz_work[k] == ',')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_work[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
				break;
			}
			else if (sz_work[k] != ' ')
			{
				sz_ele[j][l] = sz_work[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			continue;
		}

		/* X帯送受信ルート判定文字列を探す */
		str = CString(sz_ele[0]);
		if (str.Find(X_ROUTE_DEF_TXT) >= 0)
		{
			stpd.l_route_mode = 1;
		}

		for (k = 0; sz_c_ele34[k] != _T(""); ++k)
		{
			if ((k == 2) || (k == 4) || (k == 35))
			{	/* 判定ケースを追加 */
				if (sz_c_ele34[k].Find(str) >= 0)
				{
					break;
				}
			}
			else
			{
				if (str == sz_c_ele34[k])
				{
					break;
				}
			}
		}

		if (sz_c_ele34[k] == _T(""))
		{
			continue;
		}

		swprintf_s(sz_work, static_cast<unsigned>(_countof(sz_work)), _T("%s %s"), sz_date, sz_time);
		if (d_s_start == 0.0 || d_s_start > d_time)
		{
			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), _T(" "));
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_time);
			d_s_start = d_time;
		}

		switch (k) 
		{
			/*----- レディネスチェック処理 -----*/
		case 0:
			(stpd.l_rdychk) |= 0x01;
			break;

		case 1:
			(stpd.l_rdychk) |= 0x02;
			break;

			/*----- 初期設定処理 -----*/
		case 2:
			stpd.l_init = 1;
			break;

			/*----- ビットレートＳ -----*/
		case 3:
			stpd.sz_s_bitrate = CString(sz_ele[1]);
			break;

			/*----- ビットレートＸ -----*/
		case 4:
			stpd.sz_x_bitrate = CString(sz_ele[1]);
			break;

			/*----- ビットレートＸＱＰＳＫ -----*/
		case 17:
			stpd.sz_x_qpsk = CString(sz_ele[1]);
			break;

			/*----- S/X コマンドビットレート -----*/
		case 5:
			stpd.sz_cmd_bit_rate = CString(sz_ele[1]);
			break;

			/*----- X コマンドビットレート -----*/
		case 35:
			stpd.sz_x_cmd_bit_rate = CString(sz_ele[1]);
			break;

			/*----- 開始時刻処理 -----*/
		case 12:
			stpd.sz_start.Format(_T("%s %s"), sz_date, sz_time);

			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), _T(" "));
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_time);

			TCHAR sz_lwork[50];
			__int64 d_work;
			__int64 l_tm;
			swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s.0000"), sz_s_start);

			d_work = CSatelliteData::StrTimeToCTime(CString(sz_lwork)).GetTime();
			l_tm = d_work - d_tm;
			stpd.l_ant_time = (long)(l_tm / 60);
			break;

			/*----- 終了時刻処理 -----*/
		case 13:
			stpd.sz_end.Format(_T("%s %s"), sz_date, sz_time);
			break;

			/*----- S-TX 送信出力選択処理 -----*/
		case 32:
			/* 送信機-送信出力-S帯送信周波数変換盤を解析 */
			memset(sz_txrfpwr, 0, sizeof(sz_txrfpwr));

			swscanf_s(sz_ele[1], _T("%[^_]_%[^_]_%[^_]"),
				sz_txrfpwr[0], static_cast<unsigned>(_countof(sz_txrfpwr[0])),
				sz_txrfpwr[1], static_cast<unsigned>(_countof(sz_txrfpwr[1])),
				sz_txrfpwr[2], static_cast<unsigned>(_countof(sz_txrfpwr[2])));

			sz_txcomp.Format(_T("%s_%s"), sz_txrfpwr[0], sz_txrfpwr[1]);
			for (i = 0; sz_s_tx_pwr_sel[i] != _T(""); ++i)
			{
				if (sz_s_tx_pwr_sel[i] == sz_txcomp)
				{
					stpd.l_tx_pwr_sel = i;
					break;
				}
			}
			/* S帯送信周波数変換盤 追加 */
			for (i = 0; sz_s_tx_uc_sel[i] != _T(""); ++i) {
				if (sz_s_tx_uc_sel[i] == CString(sz_txrfpwr[2])) {
					stpd.l_tx_uc_sel = i;
					break;
				}
			}
			break;

			/*----- X-TX 送信出力選択処理 -----*/
		case 33:
			for (i = 0; sz_x_tx_pwr_sel[i] != _T(""); ++i)
			{
				if (sz_x_tx_pwr_sel[i] == CString(sz_ele[1]))
				{
					stpd.l_tx_pwr_sel = i;
					break;
				}
			}
			break;

			/*----- Ｘ帯変調装置選択処理 -----*/
		case 34:
			stpd.l_mod_sel = i;
			break;

		case 36: //レディネスチェック（AGC)
			if ((CString(sz_ele[1]).Find(STR_SX_ACQRX_S) != -1) ||
				(CString(sz_ele[1]).Find(STR_SX_ACQRX_X) != -1) ||
				(CString(sz_ele[1]).Find(STR_SS_TCR) != -1) ||
				(CString(sz_ele[1]).Find(STR_S_RX) != -1) ||
				(CString(sz_ele[1]).Find(STR_X_RX) != -1) ||
				(CString(sz_ele[1]) == STR_X_QPSKRX_A) ||
				(CString(sz_ele[1]) == STR_DSN_MODEM_A))
			{
				stpd.l_agc_index = 1;
			}
			break;

		case 37: //レディネスチェック（RNG)
			if ((CString(sz_ele[1]) == STR_SS) ||
				(CString(sz_ele[1]) == STR_SX) ||
				(CString(sz_ele[1]) == STR_DXN) ||
				(CString(sz_ele[1]) == STR_DXR))
			{
				stpd.l_rng_index = 1;
			}

			break;

		case 38: //レディネスチェック（テレメトリ伝送）
			if ((CString(sz_ele[1]) == STR_S) ||
				(CString(sz_ele[1]) == STR_X2) ||
				(CString(sz_ele[1]) == STR_QPSK))
			{
				stpd.l_tlm_index = 1;
			}
			break;

		case 39: //レディネスチェック（コマンド伝送）
			if ((CString(sz_ele[1]) == STR_1) ||
				(CString(sz_ele[1]) == STR_3))
			{
				stpd.l_cmd_index = 1;
			}
			break;

		case 40: //アンテナ待ち受け指向
			stpd.l_antwait_flg = 1;
			break;

		case 41:
			if (CString(sz_ele[1]).Find(STR_ANT) >= 0)
			{
				TCHAR sz_lwork[50];

				swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s %s.0000"), sz_date, sz_time);
				d_tm = CSatelliteData::StrTimeToCTime(CString(sz_lwork)).GetTime();
			}
			break;

		case 57: // UP START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_uplink_s_timelst.push_back(CString(strTime));
			break;

		case 58: // UP END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_uplink_e_timelst.push_back(CString(strTime));
			break;

		case 59: // RNG_START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_rng_s_timelst.push_back(CString(strTime));

			// 種別
			stpd.sz_rng_kindlst.push_back(CString(sz_ele[1]));
			break;

		case 60: // RNG_END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_rng_e_timelst.push_back(CString(strTime));
			break;

		case 61: // CMD_START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_cmd_s_timelst.push_back(CString(strTime));
			break;
			
		case 62: // CMD_END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_cmd_e_timelst.push_back(CString(strTime));
			break;

		case 63: // TLM_START
			if (CString(sz_ele[1]) == TLM_S)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlms_s_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_X)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmx_s_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_KA)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmka_s_timelst.push_back(CString(strTime));
			}
			break;
			
		case 64: // TLM_END
			if (CString(sz_ele[1]) == TLM_S)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlms_e_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_X)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmx_e_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_KA)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmka_e_timelst.push_back(CString(strTime));
			}
			break;
		}
	}

	fclose(fp_plan);

	return 0;
}

int COperationPlan::ReadPlanFile64(CString sz_satname, CString sz_passno, stPlandata& stpd, DWORD dw)
{
	FILE	*fp_plan;
	CString sz_p_file;

	TCHAR sz_s_start[50];
	TCHAR sz_buff[512];
	TCHAR sz_work[256];
	TCHAR sz_date[50] = { 0 };
	TCHAR sz_time[50] = { 0 };
//	TCHAR sz_txrfpwr[3][20];

	TCHAR sz_work2[256];
	TCHAR sz_work3[256];
	TCHAR sz_work4[256];
	TCHAR sz_work5[256];
	TCHAR sz_work6[256];
	TCHAR sz_work7[256];
	TCHAR sz_work8[256];

	long i;
	long j;
	long l;
	long k;
	long l_rc;

	__int64 d_s_start;
	__int64 d_time;

	CString str;
	CString sz_txcomp;
	CString strTime;

	TCHAR sz_ele[10][100];
	memset(&sz_ele[0], 0, static_cast<unsigned>(_countof(sz_ele)));


	CSatelliteData& sd = theApp.GetSatelliteData();

#if 1
	CString filename = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
	if (dw == 1)
		sz_p_file.Format(_T("%senp\\%s\\%s.pln"), (LPCTSTR)filename, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	else
		sz_p_file.Format(_T("%ssttn\\%s\\%s.pln"), (LPCTSTR)filename, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
#else
//	j = (long)strlen(sd.m_satinfo[m_IdxEqsat].sz_plan_dir);
//	if (sd.m_satinfo[m_IdxEqsat].sz_plan_dir[j - 1] == '/')
//	{
//		sz_p_file.Format(_T("%s%s.pln"), CString(sd.m_satinfo[m_IdxEqsat].sz_plan_dir), sz_passno);
//	}
//	else
//	{
//		sz_p_file.Format(_T("%s\\%s.pln"), CString(sd.m_satinfo[m_IdxEqsat].sz_plan_dir), sz_passno);
//	}
#endif
	sz_s_start[0] = '\0';


	/*----- ftp file get  "sz_p_file" -----*/
	CString path(sd.m_satinfo[m_IdxEqsat].sz_plan_dir);
	CString file(sz_p_file);

	//	if (GetPlanFile(path, file) == FALSE)
	//	{
	//		return -1;
	//	}

	if (_wfopen_s(&fp_plan, sz_p_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("計画ファイルを開くことができません。\n%s"), (LPCTSTR)sz_p_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	stpd.sz_start = _T("");
	stpd.sz_end = _T("");
	stpd.sz_s_bitrate = _T("");
	stpd.sz_x_bitrate = _T("");
	stpd.sz_x_qpsk = _T("");
	stpd.sz_cmd_bit_rate = _T("");
	stpd.sz_x_cmd_bit_rate = _T("");
	stpd.l_cmd_mod = 0;
	stpd.l_rng_mod = 0;
	stpd.sz_rng_band = _T("");
	stpd.sz_rng_start = _T("");
	stpd.sz_rng_end = _T("");
	stpd.l_rdychk = 0;
	stpd.l_init = 0;
	stpd.l_auto_acq = 0;
	stpd.l_offset = 0;
	stpd.l_a_flag = 0;
	stpd.l_l_flag = 0;
	stpd.l_term_ant = 1;
	stpd.l_tx_ramp = 0;
	stpd.l_rx_ramp = 0;
	stpd.l_acq_route = 0;
	stpd.l_pl_sel = 0;
	stpd.l_top_mon = 0;
	stpd.l_ent_angle = 0;
	stpd.l_ext_angle = 0;
	stpd.sz_level_s = _T("");
	stpd.sz_level_x = _T("");
	stpd.sz_ant_s = _T("");
	stpd.sz_ant_x = _T("");
	stpd.l_delay_sel = 0;
	stpd.l_tx_pwr_sel = 0;
	stpd.l_mod_sel = 0;
	stpd.l_tx_uc_sel = -1;
	stpd.l_antwait_flg = 0;
	stpd.l_cmd_index = 0;
	stpd.l_tlm_index = 0;
	stpd.l_agc_index = 0;
	stpd.l_rng_index = 0;
	stpd.l_range_band = 0;
	stpd.l_ant_stow = 2;
	stpd.sz_cid = _T("");
	stpd.sz_station = _T("");
	stpd.sz_priority = _T("");
	stpd.sz_ope_mode = _T("");
	stpd.l_uplink = 0;
	stpd.sz_uplink_s_timelst.clear();
	stpd.sz_uplink_e_timelst.clear();
	stpd.l_rng = 0;
	stpd.sz_rng_s_timelst.clear();
	stpd.sz_rng_e_timelst.clear();
	stpd.l_cmd = 0;
	stpd.sz_cmd_s_timelst.clear();
	stpd.sz_cmd_e_timelst.clear();
	stpd.l_tlms = 0;
	stpd.sz_tlms_s_timelst.clear();
	stpd.sz_tlms_e_timelst.clear();
	stpd.l_tlmx = 0;
	stpd.sz_tlmx_s_timelst.clear();
	stpd.sz_tlmx_e_timelst.clear();
	stpd.l_tlmka = 0;
	stpd.sz_tlmka_s_timelst.clear();
	stpd.sz_tlmka_e_timelst.clear();

	__int64 d_tm;

	d_s_start = 0;
	d_time = 0;

	char sz_line[512];
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_plan) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			if (wcsncmp(sz_buff, SOS_HEADER_ID, wcslen(SOS_HEADER_ID)) == 0)
			{
				swscanf_s(sz_buff, _T("%s%s%s%s%s%s%s%s"),
					sz_work, static_cast<unsigned>(_countof(sz_work)),
					sz_work2, static_cast<unsigned>(_countof(sz_work2)),
					sz_work3, static_cast<unsigned>(_countof(sz_work3)),
					sz_work4, static_cast<unsigned>(_countof(sz_work4)),
					sz_work5, static_cast<unsigned>(_countof(sz_work5)),
					sz_work6, static_cast<unsigned>(_countof(sz_work6)),
					sz_work7, static_cast<unsigned>(_countof(sz_work7)),
					sz_work8, static_cast<unsigned>(_countof(sz_work8)));
			}

			l_rc = swscanf_s(&sz_buff[1], _T("%s%s%s%s%s%s%s%s%s%s"),
				sz_ele[0], static_cast<unsigned>(_countof(sz_ele[0])),
				sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])),
				sz_ele[2], static_cast<unsigned>(_countof(sz_ele[2])),
				sz_ele[3], static_cast<unsigned>(_countof(sz_ele[3])),
				sz_ele[4], static_cast<unsigned>(_countof(sz_ele[4])),
				sz_ele[5], static_cast<unsigned>(_countof(sz_ele[5])),
				sz_ele[6], static_cast<unsigned>(_countof(sz_ele[6])),
				sz_ele[7], static_cast<unsigned>(_countof(sz_ele[7])),
				sz_ele[8], static_cast<unsigned>(_countof(sz_ele[8])),
				sz_ele[9], static_cast<unsigned>(_countof(sz_ele[9])));

			for (i = 0; i < l_rc; ++i)
			{
				str = CString(sz_ele[i]);
				/*---------- 初期値 チェック ----------*/
				if (str.Find(sz_c_ele64[2]) >= 0)
				{
					j = sz_c_ele64[2].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_init = k;
							break;
						}
					}
				}

				/*---------- CMD MOD の AUTO/MANチェック ----------*/
				if (str.Find(sz_c_ele64[6]) >= 0)
				{
					j = sz_c_ele64[6].GetLength() + 1;
					for (k = 0; sz_auto[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_auto[k])
						{
							stpd.l_cmd_mod = k;
							break;
						}
					}
				}
				/*---------- RNG MOD の AUTO/MANチェック ----------*/
				if (str.Find(sz_c_ele64[7]) >= 0)
				{
					j = sz_c_ele64[7].GetLength() + 1;
					for (k = 0; sz_auto[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_auto[k])
						{
							stpd.l_rng_mod = k;
							break;
						}
					}
				}
				/*---------- RNG BAND の 取り込み ----------*/
				if (str.Find(sz_c_ele64[8]) >= 0)
				{
					j = sz_c_ele64[8].GetLength() + 1;
					stpd.sz_rng_band = CString(&sz_ele[i][j]);
				}
				/*---------- RNG START の 取り込み ----------*/
				if (str.Find(sz_c_ele64[9]) >= 0)
				{
					j = sz_c_ele64[9].GetLength() + 1;
					stpd.sz_rng_start = CString(&sz_ele[i][j]);
				}
				/*---------- RNG END の 取り込み ----------*/
				if (str.Find(sz_c_ele64[10]) >= 0)
				{
					j = sz_c_ele64[10].GetLength() + 1;
					stpd.sz_rng_end = CString(&sz_ele[i][j]);
				}
				/*---------- 時刻オフセットへの追従 ----------*/
				if (str.Find(sz_c_ele64[11]) >= 0)
				{
					j = sz_c_ele64[11].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_offset = k;
							break;
						}
					}
				}
				/*---------- AOS FLAG ----------*/
				if (str.Find(sz_c_ele64[14]) >= 0)
				{
					j = sz_c_ele64[14].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_a_flag = k;
							break;
						}
					}
				}
				/*---------- LOS FLAG ----------*/
				if (str.Find(sz_c_ele64[15]) >= 0)
				{
					j = sz_c_ele64[15].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_l_flag = k;
							break;
						}
					}
				}
				/*---------- 自動捕捉の有無チェック ----------*/
				if (str.Find(sz_c_ele64[16]) >= 0)
				{
					j = sz_c_ele64[16].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_auto_acq = k;
							break;
						}
					}
				}
				/*---------- パス終了時のＡＮＴモード ----------*/
				if (str.Find(sz_c_ele64[18]) >= 0)
				{
					j = sz_c_ele64[18].GetLength() + 1;
					for (k = 0; sz_term_ant[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_term_ant[k])
						{
							stpd.l_term_ant = k;
							break;
						}
					}
				}
				/*---------- 送信設備ランプ制御有無 ----------*/
				if (str.Find(sz_c_ele64[19]) >= 0)
				{
					j = sz_c_ele64[19].GetLength() + 1;
					for (k = 0; sz_tx_ramp[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_tx_ramp[k])
						{
							stpd.l_tx_ramp = k;
							break;
						}
					}
				}
				/*---------- 受信設備ランプ制御有無 ----------*/
				if (str.Find(sz_c_ele64[20]) >= 0)
				{
					j = sz_c_ele64[20].GetLength() + 1;
					for (k = 0; sz_rx_ramp[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_rx_ramp[k])
						{
							stpd.l_rx_ramp = k;
							break;
						}
					}
				}
				/*---------- 補足ルート ----------*/
				if (str.Find(sz_c_ele64[21]) >= 0)
				{
					j = sz_c_ele64[21].GetLength() + 1;
					stpd.l_acq_route = _wtoi(&sz_ele[i][j]);
					stpd.l_acq_route -= 1;	/* 配列番号に変換 */
				}
				/*---------- ACQ/MAIN判定有無 ----------*/
				if (str.Find(sz_c_ele64[22]) >= 0)
				{
					j = sz_c_ele64[22].GetLength() + 1;
					for (k = 0; sz_on_off[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on_off[k])
						{
							stpd.l_pl_sel = k;
							break;
						}
					}
				}
				/*---------- 天頂通過監視有無 ----------*/
				if (str.Find(sz_c_ele64[23]) >= 0)
				{
					j = sz_c_ele64[23].GetLength() + 1;
					for (k = 0; sz_on_off[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on_off[k])
						{
							stpd.l_top_mon = k;
							break;
						}
					}
				}
				/*---------- 天頂通過監視入角 ----------*/
				if (str.Find(sz_c_ele64[24]) >= 0)
				{
					j = sz_c_ele64[24].GetLength() + 1;
					stpd.l_ent_angle = _wtoi(&sz_ele[i][j]);
				}
				/*---------- 天頂通過監視出角 ----------*/
				if (str.Find(sz_c_ele64[25]) >= 0)
				{
					j = sz_c_ele64[25].GetLength() + 1;
					stpd.l_ext_angle = _wtoi(&sz_ele[i][j]);
				}
				/*---------- 再生測距対応　局内ディレイ値選択 ----------*/
				if (str.Find(sz_c_ele64[33]) >= 0)
				{
					j = sz_c_ele64[33].GetLength() + 1;
					for (k = 0; sz_delay_sel[k] != _T(""); ++k) {
						if (CString(&sz_ele[i][j]) == sz_delay_sel[k])
						{
							stpd.l_delay_sel = k;
							break;
						}
					}
				}

				/*---------- ANT天頂指向有無 ----------*/
				if (str.Find(sz_c_ele64[46]) >= 0)
				{
					j = sz_c_ele64[46].GetLength() + 1;
					for (k = 0; sz_ant_stow[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_ant_stow[k])
						{
							stpd.l_ant_stow = k;
							break;
						}
					}
				}

				/*---------- CID ----------*/
				if (str.Find(sz_c_ele64[47]) >= 0)
				{
					j = sz_c_ele64[47].GetLength() + 1;
					stpd.sz_cid = CString(&sz_ele[i][j]);
				}

				/*---------- 局名 ----------*/
				if (str.Find(sz_c_ele64[48]) >= 0)
				{
					j = sz_c_ele64[48].GetLength() + 1;
					stpd.sz_station = CString(&sz_ele[i][j]);
				}

				/*---------- 主従局 ----------*/
				if (str.Find(sz_c_ele64[49]) >= 0)
				{
					j = sz_c_ele64[49].GetLength() + 1;
					stpd.sz_priority = CString(&sz_ele[i][j]);
				}

				/*---------- オペレーションモード ----------*/
				if (str.Find(sz_c_ele64[50]) >= 0)
				{
					j = sz_c_ele64[50].GetLength() + 1;
					stpd.sz_ope_mode = CString(&sz_ele[i][j]);
				}

			}
			continue;
		}

		swscanf_s(sz_buff, _T("%s%s%s"),
			sz_date, static_cast<unsigned>(_countof(sz_date)),
			sz_time, static_cast<unsigned>(_countof(sz_time)),
			sz_work, static_cast<unsigned>(_countof(sz_work)));

		j = 0;
		l = 0;
		for (k = 0; sz_work[k] != '\0'; ++k)
		{
			if (sz_work[k] == ',')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_work[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
				break;
			}
			else if (sz_work[k] != ' ')
			{
				sz_ele[j][l] = sz_work[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			continue;
		}

		/* X帯送受信ルート判定文字列を探す */
		str = CString(sz_ele[0]);
		if (str.Find(X_ROUTE_DEF_TXT) >= 0)
		{
			stpd.l_route_mode = 1;
		}

		for (k = 0; sz_c_ele64[k] != _T(""); ++k)
		{
			if ((k == 2) || (k == 4) || (k == 35))
			{	/* 判定ケースを追加 */
				if (sz_c_ele64[k].Find(str) >= 0)
				{
					break;
				}
			}
			else
			{
				if (str == sz_c_ele64[k])
				{
					break;
				}
			}
		}

		if (sz_c_ele64[k] == _T(""))
		{
			continue;
		}

		swprintf_s(sz_work, static_cast<unsigned>(_countof(sz_work)), _T("%s %s"), sz_date, sz_time);
		if (d_s_start == 0.0 || d_s_start > d_time)
		{
			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), _T(" "));
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_time);
			d_s_start = d_time;
		}

		switch (k)
		{
			/*----- レディネスチェック処理 -----*/
		case 0:
			(stpd.l_rdychk) |= 0x01;
			break;

		case 1:
			(stpd.l_rdychk) |= 0x02;
			break;

			/*----- 初期設定処理 -----*/
		case 2:
			stpd.l_init = 1;
			break;

			/*----- ビットレートＳ -----*/
		case 3:
			stpd.sz_s_bitrate = CString(sz_ele[1]);
			break;

			/*----- ビットレートＸ -----*/
		case 4:
			stpd.sz_x_bitrate = CString(sz_ele[1]);
			break;

			/*----- ビットレートＸＱＰＳＫ -----*/
		case 17:
			stpd.sz_x_qpsk = CString(sz_ele[1]);
			break;

			/*----- S/X コマンドビットレート -----*/
		case 5:
			stpd.sz_cmd_bit_rate = CString(sz_ele[1]);
			break;

			/*----- X コマンドビットレート -----*/
		case 36:
		case 43:
			stpd.sz_x_cmd_bit_rate = CString(sz_ele[1]);
			break;


			/*----- 開始時刻処理 -----*/
		case 12:
			stpd.sz_start.Format(_T("%s %s"), sz_date, sz_time);

			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), _T(" "));
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_time);

			TCHAR sz_lwork[50];
			__int64 d_work;
			__int64 l_tm;
			swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s.0000"), sz_s_start);

			d_work = CSatelliteData::StrTimeToCTime(CString(sz_lwork)).GetTime();
			l_tm = d_work - d_tm;
			stpd.l_ant_time = (long)(l_tm / 60);
			break;

			/*----- 終了時刻処理 -----*/
		case 13:
			stpd.sz_end.Format(_T("%s %s"), sz_date, sz_time);
			break;

			/*----- Ｓ帯衛星送信情報処理 -----*/
		case 28:
			{
				/* 送信レベルとアンテナ選択情報に分ける */
				CString str = CString(sz_ele[1]);
				int idx = str.Find(_T("-"));
				stpd.sz_level_s = str.Mid(0, idx);
				stpd.sz_ant_s = str.Mid(idx+1);
			}
			break;

			/*----- Ｘ帯衛星送信情報処理 -----*/
		case 29:
			{
				/* 送信レベルとアンテナ選択情報に分ける */
				CString str = CString(sz_ele[1]);
				int idx = str.Find(_T("-"));
				stpd.sz_level_x = str.Mid(0, idx);
				stpd.sz_ant_x = str.Mid(idx+1);
			}
			break;

			/*----- ４ＷＡＹ運用有無情報処理 -----*/
		case 30:
			if (wcsncmp(sz_ele[1], CTRL_4WAY, wcslen(CTRL_4WAY)) == 0) 
			{
				stpd.l_selene_4way = 0;
			}
			else if (wcsncmp(sz_ele[1], _T("ANT"), wcslen(_T("ANT"))) == 0) 
			{
				TCHAR sz_lwork[50];
				swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s %s.0000"), sz_date, sz_time);
				d_tm = CSatelliteData::StrTimeToCTime(CString(sz_lwork)).GetTime();
			}
			break;

			/*----- S-TX 送信出力選択処理 -----*//* 2006-11-08 追加 */
		case 31:
			for (i = 0; sz_tx_pwr_sel64[i] != (char*)NULL; i++) 
			{
				if (wcscmp(sz_tx_pwr_sel64[i], sz_ele[1]) == 0) 
				{
					stpd.l_tx_pwr_sel = i;
					break;
				}
			}
			break;

			/*----- X-TX 送信出力選択処理 -----*//* 2006-11-15 追加 */
		case 32:
			for (i = 0; sz_tx_pwr_sel64[i] != (char*)NULL; i++) 
			{
				if (wcscmp(sz_tx_pwr_sel64[i], sz_ele[1]) == 0) 
				{
					stpd.l_tx_pwr_sel = i;
					break;
				}
			}
			break;

			/*----- Ｘ帯変調装置選択処理 -----*//* 2009-02-06 追加 */
		case 35:
			for (i = 0; sz_mod_sel[i] != (char*)NULL; i++) 
			{
				if (wcscmp(sz_mod_sel[i], sz_ele[1]) == 0) 
				{
					stpd.l_mod_sel = i;
					break;
				}
			}
			break;

		case 37: //レディネスチェック（AGC)
			if ((CString(sz_ele[1]) == STR_S_RX_A) ||
				(CString(sz_ele[1]) == STR_X_RX2_A) ||
				(CString(sz_ele[1]) == STR_DSN_MODEM_A))
			{
				stpd.l_agc_index = 1;
			}

			break;

		case 38: //レディネスチェック（RNG)
			if ((CString(sz_ele[1]) == STR_SS) ||
				(CString(sz_ele[1]) == STR_SX) ||
				(CString(sz_ele[1]) == STR_XX) ||
				(CString(sz_ele[1]) == STR_RX) ||
				(CString(sz_ele[1]) == STR_DXN) ||
				(CString(sz_ele[1]) == STR_DXR))
			{
				stpd.l_rng_index = 1;
			}

			break;

		case 39: //レディネスチェック（テレメトリ伝送）
			if ((CString(sz_ele[1]) == STR_S) ||
				(CString(sz_ele[1]) == STR_X) ||
				(CString(sz_ele[1]) == STR_X2))
			{
				stpd.l_tlm_index = 1;
			}
			break;

		case 40: //レディネスチェック（コマンド伝送）
			if ((CString(sz_ele[1]) == STR_2) ||
				(CString(sz_ele[1]) == STR_3) ||
				(CString(sz_ele[1]) == STR_4))
			{
				stpd.l_cmd_index = 1;
			}
			break;


		case 41: //アンテナ待ち受け指向
			stpd.l_antwait_flg = 1;
			break;


		case 53: // UP START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_uplink_s_timelst.push_back(CString(strTime));
			break;

		case 54: // UP END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_uplink_e_timelst.push_back(CString(strTime));
			break;

		case 55: // RNG_START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_rng_s_timelst.push_back(CString(strTime));

			// 種別
			stpd.sz_rng_kindlst.push_back(CString(sz_ele[1]));
			break;

		case 56: // RNG_END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_rng_e_timelst.push_back(CString(strTime));
			break;

		case 57: // CMD_START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_cmd_s_timelst.push_back(CString(strTime));
			break;
			
		case 58: // CMD_END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_cmd_e_timelst.push_back(CString(strTime));
			break;

		case 59: // TLM_START
			if (CString(sz_ele[1]) == TLM_S)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlms_s_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_X)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmx_s_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_KA)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmka_s_timelst.push_back(CString(strTime));
			}
			break;

		case 60: // TLM_END
			if (CString(sz_ele[1]) == TLM_S)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlms_e_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_X)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmx_e_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_KA)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmka_e_timelst.push_back(CString(strTime));
			}
			break;


		}
	}

	fclose(fp_plan);

	return 0;
}

/*============================================================================*/
/*! COperationPlan

-# RNG種別取得

@param  なし
@retval 計画ファイル出力用のRNG種別文字列
*/
/*============================================================================*/
CString	COperationPlan::ExchageRngTypePlanFile(CString str)
{
	CString type = _T("");
	if (str == _T("SS"))
	{
		type = _T("SS-RNG");
	}
	else if (str == _T("SX"))
	{
		type = _T("SX-RNG");
	}
	else if (str == _T("XX新"))
	{
		type = _T("XX-RNG_NOR");
	}
	else if (str == _T("XX新再"))
	{
		type = _T("XX-RNG_REG");
	}
	else if (str == _T("XKa新"))
	{
		type = _T("XKa-RNG_NOR");
	}
	else if (str == _T("XKa新再"))
	{
		type = _T("XKa-RNG_REG");
	}

	return type;
}

/*============================================================================*/
/*! COperationPlan

-# 運用モード取得

@param  なし
@retval 計画ファイル出力用の運用モード文字列
*/
/*============================================================================*/
void COperationPlan::GetOpeModeAddString(CString &str, CString mode)
{
	if (str == _T(""))
	{
		str = mode;
	}
	else
	{
		if (str.Find(mode) == -1)
		{
			str += _T(",") + mode;
		}
	}
}

CString COperationPlan::GetOpeMode()
{
	CSatelliteData& sd = theApp.GetSatelliteData();
	m_IdxEqsat = GetIndexEqSatDBData(m_SatelliteName);

	CString mode = _T("");

	// TLM S
	if (m_TLMSChkBox)
	{
		if (m_TlmSList.GetItemCount() > 0)
		{
			GetOpeModeAddString(mode, _T("S-TLM"));
		}
	}

	// TLM X
	if (m_TLMXChkBox)
	{
		if (m_TlmXList.GetItemCount() > 0)
		{
			GetOpeModeAddString(mode, _T("X-TLM"));
		}
	}

	// TLM Ka
	if (m_TLMKaChkBox)
	{
		if (m_TlmKaList.GetItemCount() > 0)
		{
			GetOpeModeAddString(mode, _T("Ka-TLM"));
		}
	}

	// CMD
	if (m_CmdChkBox)
	{
		if (m_CmdList.GetItemCount() > 0)
		{
			if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_Y_ID)
			{
				GetOpeModeAddString(mode, _T("X-CMD"));
			}
			else
			{
				GetOpeModeAddString(mode, _T("S-CMD"));
			}
		}
	}

	// RNG
	if (m_RngChkBox)
	{
		int count = m_RngList.GetItemCount();
		for (int i = 0; i < count; ++i)
		{
			CString strRngType = m_RngList.GetItemText(i, 2);
			GetOpeModeAddString(mode, ExchageRngTypePlanFile(strRngType));
		}
	}

	return mode;
}


/*============================================================================*/
/*! COperationPlan

-# 計画ファイルを書き込む

@param  なし
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
int	COperationPlan::WritePlanFile20(CString sz_satname, CString sz_passno, stPlandata& stpd, BOOL bRegist)
{
	FILE *fp_smpl;
	FILE *fp_plan;
	CString sz_s_file;
	CString sz_p_file;
	CString sz_p_folder;

	TCHAR sz_s_start[50];
	TCHAR sz_buff[512];
	TCHAR sz_time[50];
	TCHAR sz_ele[10][100];
	TCHAR sz_work[256];
	TCHAR sz_date[50];
	TCHAR *chp_pos;

	long j;
	long k;
	long l;
	long m;
	long l_ncount;

	long l_hour;
	long l_min;
	long l_sec;

	__int64 d_s_start;
	__int64 d_ntime[10];
	__int64 d_time;
	__int64 d_start;

	int count = 0;
	int t_rdy = 0;

	memset(&sz_ele[0], 0, sizeof(sz_ele));


	/*----- 雛形＆計画ファイルのパス名作成 -----*/
	CSatelliteData& sd = theApp.GetSatelliteData();

#if 1
	CString filenamep = theApp.GetShareFilePath(eFileType_PlanOut_Folder, theApp.GetSelectStation());
	sz_p_file.Format(_T("%s%s\\%s.pln"), (LPCTSTR)filenamep, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno.MakeLower());
	sz_p_folder.Format(_T("%s%s"), (LPCTSTR)filenamep, (LPCTSTR)sz_satname);
	if (PathFileExists(sz_p_folder) == FALSE)
	{
		CreateDirectory(sz_p_folder, NULL);
	}

	CString filenames = theApp.GetShareFilePath(eFileType_Form_Folder, theApp.GetSelectStation());
	sz_s_file.Format(_T("%s%s.pln"), (LPCTSTR)filenames, (LPCTSTR)sz_satname);
#else
//	CString filename = theApp.GetShareFilePath(eFileType_Form_Folder, theApp.GetSelectStation());
//
//	j = (long)strlen(sd.satinfo.sz_plan_dir);
//	if (sd.satinfo.sz_plan_dir[j - 1] == '/')
//	{
//		sz_p_file.Format(_T("%s%s.pln"), CString(sd.satinfo.sz_plan_dir), sz_passno);
//	}
//	else
//	{
//		sz_p_file.Format(_T("%s\\%s.pln"), CString(sd.satinfo.sz_plan_dir), sz_passno);
//	}
//	sz_s_file.Format(_T("%s%s.pln"), filename, sz_satname);
#endif
	sz_s_start[0] = '\0';


	if (_wfopen_s(&fp_smpl, sz_s_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("計画雛形ファイル開くことができません。\n%s"), (LPCTSTR)sz_s_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	/*---------- 開始時刻コメント用計画内容ファイル出力処理 ----------*/
	d_s_start = 0;
	for (k = 0; k < 10; k++)
	{
		d_ntime[k] = 0;
	}

	/*---------- コメント用計画内容ファイル出力処理 ----------*/

	char sz_line[512];
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_smpl) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			continue;
		}

		/*---------- 計画内容作成処理 ----------*/

		l = 0;
		if (sz_buff[0] == 'N')
		{
			if (sz_buff[1] >= 0x30 && sz_buff[1] <= 0x39)
			{
				l_ncount = sz_buff[1] - 0x30;
				k = 2;
			}
			else
			{
				l_ncount = 0;
				k = 1;
			}
		}
		else
		{
			l_ncount = -1;
			k = 0;
		}
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] != ' ')
			{
				sz_time[l] = sz_buff[k];
				l++;
			}
			else
			{
				if (l > 0)
				{
					sz_time[l] = '\0';
					break;
				}
			}
		}

		j = 0;
		l = 0;
		sz_ele[2][0] = '\0';
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] == ',') {
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_buff[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				sz_ele[j][l] = '\0';
				l = 0;
				break;
			}
			else if (sz_buff[k] != ' ')
			{
				sz_ele[j][l] = sz_buff[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			continue;
		}

		for (k = 0; sz_c_ele20[k] != _T(""); ++k)
		{
			if (sz_c_ele20[k].Find(CString(sz_ele[0])) >= 0)
			{
				break;
			}
		}

		m = swscanf_s(sz_time, _T("%d:%d:%d%s"),
			&l_hour, &l_min, &l_sec,
			sz_work, static_cast<unsigned>(_countof(sz_work)));

		if (m != 3)
		{
			if (sz_c_ele20[k] == _T(""))
			{
				continue;
			}
			fclose(fp_smpl);
			return -1;
		}
		d_time = (l_hour * 3600) + (l_min * 60) + l_sec;
		if (sz_time[0] == '-')
		{
			d_time = -d_time;
		}
		if (k < 2)
		{
			/*----- レディネスチェック処理 -----*/
			l = (stpd.l_rdychk >> k) % 2;
			if (l == 0)
			{
				continue;
			}
		}
		/*----- 初期設定処理 -----*/
		if (k == 2)
		{
			if (stpd.l_init == 0)
			{
				continue;
			}
		}
		if (l_ncount >= 0)
		{
			if (d_ntime[l_ncount] > d_time)
			{
				d_ntime[l_ncount] = d_time;
			}
		}
	}

	TCHAR sz_lwork[50];
	swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s"), (LPCTSTR)stpd.sz_start);
	d_start = CSatelliteData::StrTimeToCTime(CString(sz_lwork)).GetTime();
	for (k = 0; k < 9; ++k)
	{
		d_ntime[k + 1] += d_ntime[k];
	}

	fclose(fp_smpl);


	if (_wfopen_s(&fp_smpl, sz_s_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("計画雛形ファイル開くことができません。\n%s"), (LPCTSTR)sz_s_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	if (_wfopen_s(&fp_plan, sz_p_file, _T("wb")) != 0)
	{
		CString mes;
		mes.Format(_T("計画ファイルを開くことができません。\n%s"), (LPCTSTR)sz_p_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		fclose(fp_smpl);
		return -1;
	}

	/*---------- コメント用計画内容ファイル出力処理 ----------*/

	TCHAR szwline[2048];
	CStringA cbuf;
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_smpl) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));

		if (sz_buff[0] == '#')
		{
			if (wcsncmp(sz_buff, SOS_HEADER_ID, wcslen(SOS_HEADER_ID)) == 0)
			{
#if 1	// UTC
				SYSTEMTIME timeSys;
				GetSystemTime(&timeSys);
				CTime time = CTime(timeSys);
#else
//				CTime time(CTime::GetCurrentTime());
#endif
				CString str_date = time.Format(_T("%Y-%m-%d %H:%M:%S"));

				sz_date[19] = '\0';
				
				wsprintf(szwline, _T("%s PLAN %s     1 %2d %s %s\n"),
					SOS_HEADER_ID,
					(LPCTSTR)CString(str_date),
					sd.m_satinfo[m_IdxEqsat].l_sat_no,
					(LPCTSTR)CString(sz_satname).MakeUpper(),
					(LPCTSTR)stpd.sz_pred_path);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

#if 0
				wsprintf(szwline, _T("%s PLAN %s     1 %2d %s %s\n"),
					SOS_HEADER_ID, 
					CString(str_date), 
					sd.m_satinfo[m_IdxEqsat].l_sat_no, 
					CString(sz_satname), 
					stpd.sz_pred_path);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
#endif

				wsprintf(szwline, _T("# 衛星名=%s パス番号=%s RDYCHK=%s\n"),
					(LPCTSTR)CString(sz_satname).MakeUpper(),
					(LPCTSTR)CString(sz_passno),
					(LPCTSTR)sz_rdychk[stpd.l_rdychk]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele20[2], (LPCTSTR)sz_on[stpd.l_init],
					(LPCTSTR)sz_c_ele20[3], (LPCTSTR)stpd.sz_s_bitrate,
					(LPCTSTR)sz_c_ele20[4], (LPCTSTR)stpd.sz_x_bitrate,
					(LPCTSTR)sz_c_ele20[17], (LPCTSTR)stpd.sz_x_qpsk);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				if (stpd.sz_cmd_bit_rate.GetLength() > 0)
				{
					wsprintf(szwline, _T("# %s=%s %s=%s %s=%s\n"),
						(LPCTSTR)sz_c_ele20[5], (LPCTSTR)stpd.sz_cmd_bit_rate,
						(LPCTSTR)sz_c_ele20[12], (LPCTSTR)stpd.sz_start,
						(LPCTSTR)sz_c_ele20[13], (LPCTSTR)stpd.sz_end);
					cbuf = CServerFile::ConvertEuc(szwline);
					fputs(cbuf, fp_plan);
				}
				else if (stpd.sz_x_cmd_bit_rate.GetLength() > 0)
				{
					wsprintf(szwline, _T("# %s=%s %s=%s %s=%s\n"),
						(LPCTSTR)sz_c_ele20[35], (LPCTSTR)stpd.sz_x_cmd_bit_rate,
						(LPCTSTR)sz_c_ele20[12], (LPCTSTR)stpd.sz_start,
						(LPCTSTR)sz_c_ele20[13], (LPCTSTR)stpd.sz_end);
					cbuf = CServerFile::ConvertEuc(szwline);
					fputs(cbuf, fp_plan);
				}
#if 1
				else
				{
					wsprintf(szwline, _T("# %s=%s %s=%s %s=%s\n"),
						(LPCTSTR)sz_c_ele20[35], (LPCTSTR)stpd.sz_x_cmd_bit_rate,
						(LPCTSTR)sz_c_ele20[12], (LPCTSTR)stpd.sz_start,
						(LPCTSTR)sz_c_ele20[13], (LPCTSTR)stpd.sz_end);
					cbuf = CServerFile::ConvertEuc(szwline);
					fputs(cbuf, fp_plan);
				}
#endif

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele20[16], (LPCTSTR)sz_on[stpd.l_auto_acq],
					(LPCTSTR)sz_c_ele20[11], (LPCTSTR)sz_on[stpd.l_offset],
					(LPCTSTR)sz_c_ele20[14], (LPCTSTR)sz_on[stpd.l_a_flag],
					(LPCTSTR)sz_c_ele20[15], (LPCTSTR)sz_on[stpd.l_l_flag],
					(LPCTSTR)sz_c_ele20[18], (LPCTSTR)sz_term_ant[stpd.l_term_ant]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele20[6], (LPCTSTR)sz_auto[stpd.l_cmd_mod],
					(LPCTSTR)sz_c_ele20[7], (LPCTSTR)sz_auto[stpd.l_rng_mod],
					(LPCTSTR)sz_c_ele20[8], (LPCTSTR)stpd.sz_rng_band,
					(LPCTSTR)sz_c_ele20[9], (LPCTSTR)stpd.sz_rng_start,
					(LPCTSTR)sz_c_ele20[10], (LPCTSTR)stpd.sz_rng_end);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%d %s=%s\n"),
					(LPCTSTR)sz_c_ele20[19], (LPCTSTR)sz_tx_ramp[stpd.l_tx_ramp],
					(LPCTSTR)sz_c_ele20[20], (LPCTSTR)sz_rx_ramp[stpd.l_rx_ramp],
					(LPCTSTR)sz_c_ele20[21], stpd.l_acq_route + 1,
					(LPCTSTR)sz_c_ele20[43], (LPCTSTR)sz_ant_stow[stpd.l_ant_stow]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%d %s=%d %s=%s\n"),
					(LPCTSTR)sz_c_ele20[22], (LPCTSTR)sz_on_off[stpd.l_pl_sel],
					(LPCTSTR)sz_c_ele20[23], (LPCTSTR)sz_on_off[stpd.l_top_mon],
					(LPCTSTR)sz_c_ele20[24], stpd.l_ent_angle,
					(LPCTSTR)sz_c_ele20[25], stpd.l_ext_angle,
					OPE_TYPE, (LPCTSTR)sz_ope_type[0]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele20[49], (LPCTSTR)stpd.sz_cid,									// CID(CID)
					(LPCTSTR)sz_c_ele20[50], mStationSimpleString[theApp.GetSelectStation()],		// 局名(ST_NAME)
					(LPCTSTR)sz_c_ele20[51], _T("MASTER"),											// 主従局(PRI):MASTER固定
					(LPCTSTR)sz_c_ele20[52], (LPCTSTR)GetOpeMode());								// 運用モード(OPE_MODE)
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s\n"),
					(LPCTSTR)sz_c_ele20[63], _T(""));											// Auto
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				continue;
			}

			cbuf = CServerFile::ConvertEuc(sz_buff);
			fputs(cbuf, fp_plan);
			continue;
		}

		/*---------- 計画内容作成処理 ----------*/

		l = 0;
		if (sz_buff[0] == 'N')
		{
			if (sz_buff[1] >= 0x30 && sz_buff[1] <= 0x39)
			{
				l_ncount = sz_buff[1] - 0x30;
				k = 2;
			}
			else
			{
				l_ncount = 0;
				k = 1;
			}
		}
		else
		{
			l_ncount = -1;
			k = 0;
		}
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] != ' ') {
				sz_time[l] = sz_buff[k];
				l++;
			}
			else
			{
				if (l > 0)
				{
					sz_time[l] = '\0';
					break;
				}
			}
		}

		j = 0;
		l = 0;
		sz_ele[2][0] = '\0';
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] == ',')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_buff[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				sz_ele[j][l] = '\0';
				l = 0;
				break;
			}
			else if (sz_buff[k] != ' ')
			{
				sz_ele[j][l] = sz_buff[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			cbuf = CServerFile::ConvertEuc(sz_buff);
			fputs(cbuf, fp_plan);
			continue;
		}

		for (k = 0; sz_c_ele20[k] != _T(""); ++k)
		{
			if ((k == 2) || (k == 4) || (k == 35))
			{ 
				if (sz_c_ele20[k].Find(CString(sz_ele[0])) >= 0)
				{
					if ((k == 4) && (sz_c_ele20[17] == CString(sz_ele[0])))
					{
						continue;
					}
					break;
				}
			}
			else if (k == 27) {
				/* PREDGETを確認する */
				chp_pos = wcschr(sz_ele[0], '.');
				if (chp_pos != (TCHAR *)NULL)
				{
					chp_pos++;
					if (sz_c_ele20[k].Find(CString(chp_pos)) >= 0)
					{
						break;
					}
				}
			}
			else
			{
				if (sz_c_ele20[k] == CString(sz_ele[0]))
				{
					break;
				}
			}
		}

		if (k == 12)
		{
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)stpd.sz_start);
		}
		else if (k == 13)
		{
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)stpd.sz_end);
		}
		else
		{
			m = swscanf_s(sz_time, _T("%d:%d:%d%s"), &l_hour, &l_min, &l_sec, sz_work, static_cast<unsigned>(_countof(sz_work)));
			if (m != 3)
			{
				if (sz_c_ele20[k] == _T(""))
				{
					cbuf = CServerFile::ConvertEuc(sz_buff);
					fputs(cbuf, fp_plan);
					continue;
				}
				fclose(fp_smpl);
				fclose(fp_plan);
				return -1;
			}
			d_time = (l_hour * 3600) + (l_min * 60) + l_sec;
			if (sz_time[0] == '-')
			{
				d_time = -d_time;
			}
			if (l_ncount >= 0)
			{
				d_time = d_ntime[l_ncount];
			}

			// ANT待ち受け指向なし時、2分前以前のイベントは2分とする。
			if (stpd.l_antwait_flg == 0 && d_time < -120)
			{
				d_time = -120;
			}

			d_time += d_start;
			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';
		}
		if (k < 2)
		{
			/*----- レディネスチェック処理 -----*/
			l = (stpd.l_rdychk >> k) % 2;
			if (l == 0)
			{
				continue;
			}
		}

		switch (k) {
			/*----- 初期設定処理 -----*/
		case 2:
		case 26:	/* <<XUP>> モードチェックを追加 */
			if (stpd.l_init == 0)
			{
				continue;
			}
			break;

			/*----- BITRATE-S処理 -----*/
		case 3:
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_s_bitrate);
			break;

			/*----- BITRATE-X処理 -----*/
		case 4:
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_x_bitrate);
			break;

			/*----- BITRATE-XQPSK処理 -----*/
		case 17:
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_x_qpsk);
			break;

			/*----- S/X CMD_BITRATE処理 -----*/
		case 5:
			if (stpd.sz_cmd_bit_rate.GetLength() > 0)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_cmd_bit_rate);
			}
			break;

			/*----- 送信出力選択処理 -----*/
		case 32:
#if 0
			if (stpd.sz_tx_pwr_sel != _T(""))
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_tx_pwr_sel);
			}
#else
			if (stpd.l_tx_pwr_sel != -1)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)sz_tx_pwr_sel20[stpd.l_tx_pwr_sel]);
			}
#endif
			break;

			/*----- X CMD_BITRATE処理 -----*/
		case 35:
			if (stpd.sz_x_cmd_bit_rate.GetLength() > 0)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_x_cmd_bit_rate);
			}
			break;

			/*----- PREDGET処理 -----*/
		case 27:
			/* Ｘ帯送信機ランプ制御の場合 ランプなし時の処理追加 */
			if (wcsncmp(sz_ele[0], X_TXSYN_TXT, wcslen(X_TXSYN_TXT)) == 0)
			{
				if ((stpd.l_tx_ramp == 1) || (stpd.l_tx_ramp == 2) || (stpd.l_tx_ramp == 3))
				{
					/* 送信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			/* DSN-MODEM(MOD) ランプ制御の場合 */
			if (wcsncmp(sz_ele[0], DSN_PREDGET_TXT, wcslen(DSN_PREDGET_TXT)) == 0)
			{
				if ((stpd.l_tx_ramp != 0) && (stpd.l_rx_ramp != 0) && (stpd.l_rx_ramp != 2))
				{
					/* 送/受信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			/* S帯送信機ランプ制御の場合 ランプなし時の処理追加 */
			if (wcsncmp(sz_ele[0], SS_TCRMOD_TXT, wcslen(SS_TCRMOD_TXT)) == 0)
			{
				if ((stpd.l_tx_ramp == 1) || (stpd.l_tx_ramp == 2) || (stpd.l_tx_ramp == 3))
				{	/* 送信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}
			/* Ｓ帯受信機ランプ制御の場合 */
			if (wcsncmp(sz_ele[0], S_RXSYN_TXT, wcslen(S_RXSYN_TXT)) == 0)
			{
				if ((stpd.l_rx_ramp != 0) && (stpd.l_rx_ramp != 1))
				{
					/* 受信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			/* Ｘ帯受信機ランプ制御の場合 */
			if (wcsncmp(sz_ele[0], X_RXSYN_TXT, wcslen(X_RXSYN_TXT)) == 0)
			{
				if ((stpd.l_rx_ramp != 0) && (stpd.l_rx_ramp != 2))
				{
					/* 受信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			break;

		case 36: //レディネスチェック（AGC)
		{
			if (stpd.l_agc_index == 0)	// OFF
			{
				continue;
			}

			if (wcsncmp(sz_ele[1], _T("S-RX"), wcslen(_T("S-RX"))) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_s_rx == AGCRX_Y_ID)
				{
					t_rdy = GetRdyTime34(_T("AGC_S"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcsncmp(sz_ele[1], _T("X-RX"), wcslen(_T("X-RX"))) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_x_rx == AGCRX_Y_ID)
				{
					t_rdy = GetRdyTime34(_T("AGC_X"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcsncmp(sz_ele[1], _T("X-QPSKRX"), wcslen(_T("X-QPSKRX"))) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_x_qpskrx == AGCRX_Y_ID)
				{
					t_rdy = GetRdyTime34(_T("AGC_XQ"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}

			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';

		}
		break;
		
		case 37: //レディネスチェック（RNG)
		{
			if (stpd.l_rng_index == 0)	// OFF
			{
				continue;
			}

			if (wcscmp(sz_ele[1], _T("SS")) == 0)
			{
				if ((sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_S_ID) ||
					(sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_SX_ID))
				{
					t_rdy = GetRdyTime34(_T("RNG_SS"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcscmp(sz_ele[1], _T("SX")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_SX_ID)
				{
					t_rdy = GetRdyTime34(_T("RNG_SX"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}

			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';
		}
		break;

		case 38: //レディネスチェック（テレメトリ伝送）
		{
			if (stpd.l_tlm_index == 0)	// OFF
			{
				continue;
			}

			if (wcscmp(sz_ele[1], _T("S")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_bitrate_s > 0)
				{
					t_rdy = GetRdyTime34(_T("TLM_S"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcscmp(sz_ele[1], _T("X")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_bitrate_x > 0)
				{
					t_rdy = GetRdyTime34(_T("TLM_X"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcscmp(sz_ele[1], _T("QPSK")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_bitrate_qpsk > 0)
				{
					t_rdy = GetRdyTime34(_T("TLM_XQ"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}

			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';
		}
		break;

		case 39: //レディネスチェック（コマンド伝送）
		{
			if (stpd.l_cmd_index == 0)	// OFF
			{
				continue;
			}

			if (wcscmp(sz_ele[1], _T("1")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_N_ID)
				{
					t_rdy = GetRdyTime34(_T("CMD_S"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcscmp(sz_ele[1], _T("2")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_Y_ID)
				{
					t_rdy = GetRdyTime34(_T("CMD_X"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}

			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';

		}
		break;

		case 40: //アンテナ待ち受け指向　
		{
			if (stpd.l_antwait_flg == 1)
			{
				d_time = d_start - (stpd.l_ant_time * 60);
				d_time = d_time + (5 * 60); //アンテナ待ち受け通知の５分後
				CTime time(d_time);
				swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
				sz_date[19] = '\0';
			}
			else
			{
				continue;
			}

		}
		break;

		case 41:
		{
			if (wcscmp(sz_ele[1], _T("ANT")) == 0)
			{
				d_time = d_start - (stpd.l_ant_time * 60);
				if (stpd.l_antwait_flg == 0)
				{
					d_time -= 2;
				}
				CTime time(d_time);
				swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
				sz_date[19] = '\0';
			}
			else if (wcsncmp(sz_ele[1], _T("S-TLMIN"), wcslen(_T("S-TLMIN"))) == 0)
			{
				if (stpd.l_range_band != 0)
				{
					continue;
				}
			}
		}
		break;

		/*----- S帯RNG計測帯域選択処理 -----*/
		case 45:
			if (stpd.l_range_band != -1)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)sz_range_band_ctrl[stpd.l_range_band]);
			}
			break;

			
		case 47: //送信予報値制御開始　
			/* DSN-MODEM 送信ランプ制御なしの場合 */
			if (CString(sz_ele[0]) == DSN_TX_AID_ON_TXT)
			{
				if (stpd.l_tx_ramp != 0)
				{
					/* 送信機ランプ制御なしの場合は送信予報値制御開始を外す */
					continue;
				}
			}
			break;

		case 48: //受信予報値制御開始　
			/* DSN-MODEM 受信ランプ制御なしの場合 */
			if (CString(sz_ele[0]) == DSN_RX_AID_ON_TXT)
			{
				if ((stpd.l_rx_ramp != 0) && (stpd.l_rx_ramp != 2))
				{
					/* 受信機ランプ制御なしの場合は受信予報値制御開始を外す */
					continue;
				}
			}
			break;

		case 65: // CTRL.PROP_READ
			// アンテナ待ち受け指向なしの場合
			if (stpd.l_antwait_flg == 0)
			{
				d_time = d_start - (stpd.l_ant_time * 60);
				CTime time(d_time);
				swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
				sz_date[19] = '\0';
			}
			break;
		}

		if (j == 4)
		{
			wsprintf(szwline, _T("%s %s,%s,%s,%s;\n"), sz_date, sz_ele[0], sz_ele[1], sz_ele[2], sz_ele[3]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);
		}
		else if ((k == 5) || (k == 35))
		{
			if (wcslen(sz_ele[1]) > 0)
			{
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, sz_ele[0], sz_ele[1], sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}
		}
		else if (k == 12) // P-START
		{
			/* P-START*/
			wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, sz_ele[0], sz_ele[1], sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);


			/* DOWN */
			wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, DOWN_START, sz_ele[1], sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);


			wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)stpd.sz_end, DOWN_END, sz_ele[1], sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);

			/* Up Link */
			count = m_UpLinkList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_UpLinkList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, UP_START, sz_ele[1], sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_UpLinkList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, UP_END, sz_ele[1], sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* CMD */
			CString strCmdType = _T("");
			if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_Y_ID)
			{
				strCmdType = _T("X");
			}
			else if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_N_ID)
			{
				strCmdType = _T("S");
			}

			count = m_CmdList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_CmdList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, CMD_START, (LPCTSTR)strCmdType, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_CmdList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, CMD_END, (LPCTSTR)strCmdType, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* RNG */
			count = m_RngList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strRngType = m_RngList.GetItemText(i, 2);
				CString strSTime = m_RngList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, RNG_START, (LPCTSTR)ExchageRngTypePlanFile(strRngType), sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_RngList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, RNG_END, (LPCTSTR)ExchageRngTypePlanFile(strRngType), sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* TLM S */
			count = m_TlmSList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_TlmSList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, TLM_START, TLM_S, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_TlmSList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, TLM_END, TLM_S, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* TLM X */
			count = m_TlmXList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_TlmXList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, TLM_START, TLM_X, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_TlmXList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, TLM_END, TLM_X, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* TLM Ka */
			count = m_TlmKaList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_TlmKaList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, TLM_START, TLM_KA, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_TlmKaList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, TLM_END, TLM_KA, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}
		}
		else if (k == 64)	// INIT.CID
		{
			wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, sz_ele[0], (LPCTSTR)stpd.sz_cid, sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);
		}
		else
		{
			wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, sz_ele[0], sz_ele[1], sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);
		}

		/* 最初の一件は無条件でコピー */
		if (sz_s_start[0] == '\0')
		{
			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
		}
		else
		{
			if (wcscmp(sz_s_start, sz_date) > 0)
			{
				wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			}
		}
	}
	fclose(fp_plan);
	fclose(fp_smpl);


	/*---------- 計画一覧リスト作成処理 ----------*/
	TCHAR sz_plan_lst[256];

	// 衛星名16文字幅
	wcscpy_s(sz_work, static_cast<unsigned>(_countof(sz_work)), sz_satname);
#if 0
	for (l = (long)wcslen(sz_work); l < 16; ++l)
	{
		wcscat_s(sz_work, static_cast<unsigned>(_countof(sz_work)), _T(" "));
	}
#endif

	sz_work[16] = '\0';
#if 1
	swprintf_s(sz_plan_lst, static_cast<unsigned>(_countof(sz_plan_lst)), _T("%-8s %-16s %s %s %s %s %s 1 %1d %5d %-6s %-15s\n"),
		mStationString[theApp.GetSelectStation()],
		sz_work,
		(LPCTSTR)CString(sz_passno),
		(LPCTSTR)stpd.sz_start,
		(LPCTSTR)stpd.sz_end,
		sz_s_start,
		(LPCTSTR)stpd.sz_pred_path,
		stpd.l_offset,
		stpd.l_offset_val,
		PLAN_GETFORM,
		PLAN_TYPE
		);

	stPlanListData new_listplan;
	CTime cTime;
	
	new_listplan.strStationName = mStationString[theApp.GetSelectStation()];	// 局名
	new_listplan.strSatelliteName = sz_satname;									// 衛星名
	new_listplan.strPassId = sz_passno;											// パスID
	cTime = CSatelliteData::StrTimeToCTime(stpd.sz_start);
	new_listplan.tPassStart = cTime.GetTime();									// パス開始日時
	cTime = CSatelliteData::StrTimeToCTime(stpd.sz_end);
	new_listplan.tPassEnd = cTime.GetTime();									// パス終了日時
	cTime = CSatelliteData::StrTimeToCTime(CString(sz_s_start));
	new_listplan.tPrePassStart = cTime.GetTime();								// プリパス開始日時
	new_listplan.strPredName = stpd.sz_pred_path;								// 予報値ファイル名
	new_listplan.lValidFlag = 1;												// チェック結果フラグ
	new_listplan.lOfsFlag = stpd.l_offset;										// 時刻オフセット追従条件フラグ
	new_listplan.tOffset = stpd.l_offset_val;									// 時刻オフセット
	new_listplan.strGetFrom = PLAN_GETFORM;										// 取得先
	new_listplan.strPlanType = PLAN_TYPE;										// 計画種別
	new_listplan.strResult = _T("");											// 実行結果
#else
//	swprintf_s(sz_plan_lst, static_cast<unsigned>(_countof(sz_plan_lst)), _T("%s %s %s %s %s %s 1 %1d %5d\n"),
//		sz_work,
//		CString(sz_passno),
//		stpd.sz_start,
//		stpd.sz_end,
//		sz_s_start,
//		stpd.sz_pred_path,
//		stpd.l_offset,
//		stpd.l_offset_val
//		);
#endif

	// 詳細表示及び詳細印刷時は以降処理はしない
	if (bRegist == FALSE)
	{
		return 0;
	}

	/*---------- 計画一覧ファイル更新処理 ----------*/
	// (実際に更新していない。)

	// 計画時刻　＞　現在時刻 
	__int64 d_time2 = CSatelliteData::StrTimeToCTime(CString(sz_s_start)).GetTime() + stpd.l_offset_val;
#if 1	// UTC
	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	__int64 d_time3 = CTime(timeSys).GetTime();
#else
//	__int64 d_time3 = CTime::GetCurrentTime().GetTime();
#endif

	CTime time2(d_time2);
	CTime time3(d_time3);
	CString strTime2 = time2.Format(_T("%Y-%m-%d %H:%M:%S"));
	CString strTime3 = time3.Format(_T("%Y-%m-%d %H:%M:%S"));

	if (d_time2 < d_time3 + START_OFFSET) 
	{
		CString mes = _T("");
		mes.Format(_T("制御開始時刻までに時間の余裕がないので計画を登録できません。\n\n 制御開始時刻 :%s\n パス開始時刻 :%s\n パス終了時刻 :%s\n\n 現在時刻       :%s\n ガードタイム :%d秒")
			, (LPCTSTR)strTime2, (LPCTSTR)stpd.sz_start, (LPCTSTR)stpd.sz_end, (LPCTSTR)strTime3, (int)START_OFFSET);

// CHG 2017.09.16 >>>
//		AfxMessageBox(mes, MB_OK | MB_ICONQUESTION);
		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
// CHG 2017.09.16 <<<
		DeleteFile(sz_p_file);
		return 1;
	}

	// 開始時刻　＜　終了時刻　
	if (new_listplan.tPassStart > new_listplan.tPassEnd)
	{
		CString mes = _T("");
		mes.Format(_T("パス終了時刻がパス開始時刻よりも過去のため計画を登録できません。\n\n パス開始時刻 :%s\n パス終了時刻 :%s")
			, (LPCTSTR)stpd.sz_start, (LPCTSTR)stpd.sz_end);

		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
		DeleteFile(sz_p_file);
		return 1;
	}

	// AOS < 開始時刻 , LOS > 終了時刻
	if (m_tAOS.GetTime() > new_listplan.tPassStart || m_tLOS.GetTime() < new_listplan.tPassEnd)
	{
		CString strTAos, strTLos;
		strTAos = m_tAOS.Format(_T("%Y-%m-%d %H:%M:%S"));
		strTLos = m_tLOS.Format(_T("%Y-%m-%d %H:%M:%S"));
		CString mes = _T("");
		mes.Format(_T("計画の時刻が予報値の範囲外のため計画を登録できません。\n\n AOS時刻 :%s\n LOS時刻 :%s\n\n パス開始時刻 :%s\n パス終了時刻 :%s")
			, (LPCTSTR)strTAos, (LPCTSTR)strTLos, (LPCTSTR)stpd.sz_start, (LPCTSTR)stpd.sz_end);

		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
		DeleteFile(sz_p_file);
		return 1;
	}

	vector<stPlanListData> v_plan_list2;
	stPlanListData st;

	vector<stPlanListData> &plist = theApp.GetSatelliteData().GetPlanList();
	if ((int)plist.size() > 0)
	{
		long i;
		l = (long)plist.size();
		for (i = 0; i < l; ++i)
		{
			st = plist[i];
			if ((st.strSatelliteName == sz_satname) && (st.strPassId == sz_passno))
			{
				break;
			}
		}
		if (i >= l)
		{
			l++;
		}
	}
	else
	{
		l = 1;
	}

	v_plan_list2 = plist;
	v_plan_list2.push_back(new_listplan);

	// 全時刻をオフセットを考慮したものに変更
	long n = (long)v_plan_list2.size();
	for (long i = 0; i < n; ++i)
	{
		stPlanListData &st = v_plan_list2[i];
		if(st.tOffset != 0)
		{
			// パス開始日時変更
			st.tPassStart += st.tOffset;

			// パス終了日時変更
			st.tPassEnd += st.tOffset;

			// 制御開始日時変更
			st.tPrePassStart += st.tOffset;
		}
	}

	// 計画リストソーティング＆重複チェック処理

	for(long i = (l-1); i > 0; --i)
	{
		for(long j = 0; j < i; ++j)
		{
			stPlanListData st1 = v_plan_list2[j];
			stPlanListData st2 = v_plan_list2[j+1];

			if(st1.tPassStart > st2.tPassEnd)
			{
				// データ入れ替え
				if (st1.tPassEnd > st2.tPassStart + START_OFFSET)
				{
					v_plan_list2[j]		= st2;
					v_plan_list2[j+1]	= st1;
				}
				else
				{
					// 重複チェック異常
					CString mes1 = _T("");
					CString mes2 = _T("");

					mes1.Format(_T("%sの%sと"), (LPCTSTR)st1.strSatelliteName, (LPCTSTR)st1.strPassId);
					mes2.Format(_T("%sの%sの運用時間が重複しています。"), (LPCTSTR)st2.strSatelliteName, (LPCTSTR)st2.strPassId);
					mes1 += mes2;
					AfxMessageBox(mes1, MB_OK | MB_ICONQUESTION);
					DeleteFile(sz_p_file);
				}
			}
		}
	}

	/*----- ftp file put "sz_p_file"  -----*/
	if (PutPlanFile(sz_satname, sz_passno) == FALSE)
	{
		return -1;
	}

	return 0;
}

int	COperationPlan::WritePlanFile34(CString sz_satname, CString sz_passno, stPlandata& stpd, BOOL bRegist)
{
	FILE *fp_smpl;
	FILE *fp_plan;
	CString sz_s_file;
	CString sz_p_file;
	CString sz_p_folder;

	TCHAR sz_s_start[50];
	TCHAR sz_buff[512];
	TCHAR sz_time[50];
	TCHAR sz_ele[10][100];
	TCHAR sz_work[256];
	TCHAR sz_date[50];
	TCHAR *chp_pos;

	long j;
	long k;
	long l;
	long m;
	long l_ncount;

	long l_hour;
	long l_min;
	long l_sec;

	__int64 d_s_start;
	__int64 d_ntime[10];
	__int64 d_time;
	__int64 d_start;

	int count = 0;
	int t_rdy = 0;
	int sameflg = 0;

	BOOL bAGCFirst = TRUE;
	long t_agc_s = 0;
	long t_agc_x = 0;

	memset(&sz_ele[0], 0, sizeof(sz_ele));


	/*----- 雛形＆計画ファイルのパス名作成 -----*/
	CSatelliteData& sd = theApp.GetSatelliteData();

#if 1
	CString filenamep = theApp.GetShareFilePath(eFileType_PlanOut_Folder, theApp.GetSelectStation());
	sz_p_file.Format(_T("%s%s\\%s.pln"), (LPCTSTR)filenamep, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	sz_p_folder.Format(_T("%s%s"), (LPCTSTR)filenamep, (LPCTSTR)sz_satname.MakeLower());
	if (PathFileExists(sz_p_folder) == FALSE)
	{
		CreateDirectory(sz_p_folder, NULL);
	}

	CString filenames = theApp.GetShareFilePath(eFileType_Form_Folder, theApp.GetSelectStation());
	sz_s_file.Format(_T("%s%s.pln"), (LPCTSTR)filenames, (LPCTSTR)sz_satname);
#else
//	CString ca_cthome;
//	ca_cthome.Format(_T("%s%s"), theApp.GetAppPath(), DATA_DIR);
//
//	j = (long)strlen(sd.m_satinfo[m_IdxEqsat].sz_plan_dir);
//	if (sd.m_satinfo[m_IdxEqsat].sz_plan_dir[j - 1] == '/')
//	{
//		sz_s_file.Format(_T("%s\\plan\\form\\%s.pln"), ca_cthome, sz_satname);
//		sz_p_file.Format(_T("%s%s.pln"), CString(sd.m_satinfo[m_IdxEqsat].sz_plan_dir), sz_passno);
//	}
//	else
//	{
//		sz_s_file.Format(_T("%s\\plan\\form\\%s.pln"), ca_cthome, sz_satname);
//		sz_p_file.Format(_T("%s\\%s.pln"), CString(sd.m_satinfo[m_IdxEqsat].sz_plan_dir), sz_passno);
//	}
#endif
	sz_s_start[0] = '\0';


	if (_wfopen_s(&fp_smpl, sz_s_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("計画雛形ファイル開くことができません。\n%s"), (LPCTSTR)sz_s_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	/*---------- 開始時刻コメント用計画内容ファイル出力処理 ----------*/
	d_s_start = 0;
	for (k = 0; k < 10; k++)
	{
		d_ntime[k] = 0;
	}

	/*---------- コメント用計画内容ファイル出力処理 ----------*/

	char sz_line[512];
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_smpl) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			continue;
		}

		/*---------- 計画内容作成処理 ----------*/

		l = 0;
		if (sz_buff[0] == 'N')
		{
			if (sz_buff[1] >= 0x30 && sz_buff[1] <= 0x39)
			{
				l_ncount = sz_buff[1] - 0x30;
				k = 2;
			}
			else
			{
				l_ncount = 0;
				k = 1;
			}
		}
		else
		{
			l_ncount = -1;
			k = 0;
		}
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] != ' ')
			{
				sz_time[l] = sz_buff[k];
				l++;
			}
			else
			{
				if (l > 0)
				{
					sz_time[l] = '\0';
					break;
				}
			}
		}

		j = 0;
		l = 0;
		sz_ele[2][0] = '\0';
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] == ',') {
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_buff[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				sz_ele[j][l] = '\0';
				l = 0;
				break;
			}
			else if (sz_buff[k] != ' ')
			{
				sz_ele[j][l] = sz_buff[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			continue;
		}

		for (k = 0; sz_c_ele34[k] != _T(""); ++k)
		{
			if (sz_c_ele34[k].Find(CString(sz_ele[0])) >= 0)
			{
				break;
			}
		}

		m = swscanf_s(sz_time, _T("%d:%d:%d%s"),
			&l_hour, &l_min, &l_sec,
			sz_work, static_cast<unsigned>(_countof(sz_work)));

		if (m != 3)
		{
			if (sz_c_ele34[k] == _T(""))
			{
				continue;
			}
			fclose(fp_smpl);
			return -1;
		}
		d_time = (l_hour * 3600) + (l_min * 60) + l_sec;
		if (sz_time[0] == '-')
		{
			d_time = -d_time;
		}
		if (k < 2)
		{
			/*----- レディネスチェック処理 -----*/
			l = (stpd.l_rdychk >> k) % 2;
			if (l == 0)
			{
				continue;
			}
		}
		/*----- 初期設定処理 -----*/
		if (k == 2)
		{
			if (stpd.l_init == 0)
			{
				continue;
			}
		}
		if (l_ncount >= 0)
		{
			if (d_ntime[l_ncount] > d_time)
			{
				d_ntime[l_ncount] = d_time;
			}
		}
	}

	TCHAR sz_lwork[50];
	swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s"), (LPCTSTR)stpd.sz_start);
	d_start = CSatelliteData::StrTimeToCTime(CString(sz_lwork)).GetTime();
	for (k = 0; k < 9; ++k)
	{
		d_ntime[k + 1] += d_ntime[k];
	}

	fclose(fp_smpl);


	if (_wfopen_s(&fp_smpl, sz_s_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("計画雛形ファイル開くことができません。\n%s"), (LPCTSTR)sz_s_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	if (_wfopen_s(&fp_plan, sz_p_file, _T("wb")) != 0)
	{
		CString mes;
		mes.Format(_T("計画ファイルを開くことができません。\n%s"), (LPCTSTR)sz_p_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		fclose(fp_smpl);
		return -1;
	}

	/*---------- コメント用計画内容ファイル出力処理 ----------*/

	TCHAR szwline[2048];
	CStringA cbuf;
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_smpl) != 0)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			if (wcsncmp(sz_buff, SOS_HEADER_ID, wcslen(SOS_HEADER_ID)) == 0)
			{
#if 1	// UTC
				SYSTEMTIME timeSys;
				GetSystemTime(&timeSys);
				CTime time = CTime(timeSys);
#else
				CTime time(CTime::GetCurrentTime());
#endif

				CString str_date = time.Format(_T("%Y-%m-%d %H:%M:%S"));

				sz_date[19] = '\0';
				wsprintf(szwline, _T("%s PLAN %s     1 %2d %s %s\n"),
					SOS_HEADER_ID,
					(LPCTSTR)CString(str_date),
					sd.m_satinfo[m_IdxEqsat].l_sat_no,
					(LPCTSTR)CString(sz_satname).MakeUpper(),
					(LPCTSTR)stpd.sz_pred_path);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# 衛星名=%s パス番号=%s RDYCHK=%s\n"),
					(LPCTSTR)CString(sz_satname).MakeUpper(),
					(LPCTSTR)CString(sz_passno),
					(LPCTSTR)sz_rdychk[stpd.l_rdychk]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele34[2], (LPCTSTR)sz_on[stpd.l_init],
					(LPCTSTR)sz_c_ele34[3], (LPCTSTR)stpd.sz_s_bitrate,
					(LPCTSTR)sz_c_ele34[4], (LPCTSTR)stpd.sz_x_bitrate,
					(LPCTSTR)sz_c_ele34[17], (LPCTSTR)stpd.sz_x_qpsk);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				if (stpd.sz_cmd_bit_rate.GetLength() > 0)
				{
					wsprintf(szwline, _T("# %s=%s %s=%s %s=%s\n"),
						(LPCTSTR)sz_c_ele34[5], (LPCTSTR)stpd.sz_cmd_bit_rate,
						(LPCTSTR)sz_c_ele34[12], (LPCTSTR)stpd.sz_start,
						(LPCTSTR)sz_c_ele34[13], (LPCTSTR)stpd.sz_end);
					cbuf = CServerFile::ConvertEuc(szwline);
					fputs(cbuf, fp_plan);
				}
				else if (stpd.sz_x_cmd_bit_rate.GetLength() > 0)
				{
					wsprintf(szwline, _T("# %s=%s %s=%s %s=%s\n"),
						(LPCTSTR)sz_c_ele34[35], (LPCTSTR)stpd.sz_x_cmd_bit_rate,
						(LPCTSTR)sz_c_ele34[12], (LPCTSTR)stpd.sz_start,
						(LPCTSTR)sz_c_ele34[13], (LPCTSTR)stpd.sz_end);
					cbuf = CServerFile::ConvertEuc(szwline);
					fputs(cbuf, fp_plan);
				}
#if 1
				else
				{
					wsprintf(szwline, _T("# %s=%s %s=%s %s=%s\n"),
						(LPCTSTR)sz_c_ele34[35], (LPCTSTR)stpd.sz_x_cmd_bit_rate,
						(LPCTSTR)sz_c_ele34[12], (LPCTSTR)stpd.sz_start,
						(LPCTSTR)sz_c_ele34[13], (LPCTSTR)stpd.sz_end);
					cbuf = CServerFile::ConvertEuc(szwline);
					fputs(cbuf, fp_plan);
				}
#endif

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele34[16], (LPCTSTR)sz_on[stpd.l_auto_acq],
					(LPCTSTR)sz_c_ele34[11], (LPCTSTR)sz_on[stpd.l_offset],
					(LPCTSTR)sz_c_ele34[14], (LPCTSTR)sz_on[stpd.l_a_flag],
					(LPCTSTR)sz_c_ele34[15], (LPCTSTR)sz_on[stpd.l_l_flag],
					(LPCTSTR)sz_c_ele34[18], (LPCTSTR)sz_term_ant[stpd.l_term_ant]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele34[6], (LPCTSTR)sz_auto[stpd.l_cmd_mod],
					(LPCTSTR)sz_c_ele34[7], (LPCTSTR)sz_auto[stpd.l_rng_mod],
					(LPCTSTR)sz_c_ele34[8], (LPCTSTR)stpd.sz_rng_band,
					(LPCTSTR)sz_c_ele34[9], (LPCTSTR)stpd.sz_rng_start,
					(LPCTSTR)sz_c_ele34[10], (LPCTSTR)stpd.sz_rng_end);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%d %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele34[19], (LPCTSTR)sz_tx_ramp[stpd.l_tx_ramp],
					(LPCTSTR)sz_c_ele34[20], (LPCTSTR)sz_rx_ramp[stpd.l_rx_ramp],
					(LPCTSTR)sz_c_ele34[21], stpd.l_acq_route + 1,
					(LPCTSTR)sz_c_ele34[42], (LPCTSTR)sz_range_band[stpd.l_range_band],
					(LPCTSTR)sz_c_ele34[43], (LPCTSTR)sz_ant_stow[stpd.l_ant_stow]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%d %s=%d %s=%s\n"),
					(LPCTSTR)sz_c_ele34[22], (LPCTSTR)sz_on_off[stpd.l_pl_sel],
					(LPCTSTR)sz_c_ele34[23], (LPCTSTR)sz_on_off[stpd.l_top_mon],
					(LPCTSTR)sz_c_ele34[24], stpd.l_ent_angle,
					(LPCTSTR)sz_c_ele34[25], stpd.l_ext_angle,
					(LPCTSTR)sz_c_ele34[30], (LPCTSTR)sz_delay_sel[stpd.l_delay_sel]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele34[51], (LPCTSTR)stpd.sz_cid,								// CID(CID)
					(LPCTSTR)sz_c_ele34[52], mStationSimpleString[theApp.GetSelectStation()],	// 局名(ST_NAME)
					(LPCTSTR)sz_c_ele34[53], _T("MASTER"),										// 主従局(PRI):MASTER固定
					(LPCTSTR)sz_c_ele34[54], (LPCTSTR)GetOpeMode());							// 運用モード(OPE_MODE)
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s\n"),
					(LPCTSTR)sz_c_ele34[65], _T(""));											// Auto
				cbuf = CServerFile::ConvertEuc(szwline);

				continue;
			}

			cbuf = CServerFile::ConvertEuc(sz_buff);
			fputs(cbuf, fp_plan);
			continue;
		}

		/*---------- 計画内容作成処理 ----------*/

		l = 0;
		if (sz_buff[0] == 'N')
		{
			if (sz_buff[1] >= 0x30 && sz_buff[1] <= 0x39)
			{
				l_ncount = sz_buff[1] - 0x30;
				k = 2;
			}
			else
			{
				l_ncount = 0;
				k = 1;
			}
		}
		else
		{
			l_ncount = -1;
			k = 0;
		}
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] != ' ') {
				sz_time[l] = sz_buff[k];
				l++;
			}
			else
			{
				if (l > 0)
				{
					sz_time[l] = '\0';
					break;
				}
			}
		}

		j = 0;
		l = 0;
		sz_ele[2][0] = '\0';
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] == ',')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_buff[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				sz_ele[j][l] = '\0';
				l = 0;
				break;
			}
			else if (sz_buff[k] != ' ')
			{
				sz_ele[j][l] = sz_buff[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			cbuf = CServerFile::ConvertEuc(sz_buff);
			fputs(cbuf, fp_plan);
			continue;
		}

		for (k = 0; sz_c_ele34[k] != _T(""); ++k)
		{
			if ((k == 2) || (k == 4) || (k == 35))
			{
				if (sz_c_ele34[k].Find(CString(sz_ele[0])) >= 0)
				{
					if ((k == 4) && (sz_c_ele34[17] == CString(sz_ele[0])))
					{
						continue;
					}
					break;
				}
			}
			else if (k == 27) {
				/* PREDGETを確認する */
				chp_pos = wcschr(sz_ele[0], '.');
				if (chp_pos != (TCHAR *)NULL)
				{
					chp_pos++;
					if (sz_c_ele34[k].Find(CString(chp_pos)) >= 0)
					{
						break;
					}
				}
			}
			else
			{
				if (sz_c_ele34[k] == CString(sz_ele[0]))
				{
					break;
				}
			}
		}

		if (k == 12)
		{
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)stpd.sz_start);
		}
		else if (k == 13)
		{
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)stpd.sz_end);
		}
		else
		{
			m = swscanf_s(sz_time, _T("%d:%d:%d%s"), &l_hour, &l_min, &l_sec, sz_work, static_cast<unsigned>(_countof(sz_work)));
			if (m != 3)
			{
				if (sz_c_ele34[k] == _T(""))
				{
					cbuf = CServerFile::ConvertEuc(sz_buff);
					fputs(cbuf, fp_plan);
					continue;
				}
				fclose(fp_smpl);
				fclose(fp_plan);
				return -1;
			}
			d_time = (l_hour * 3600) + (l_min * 60) + l_sec;
			if (sz_time[0] == '-')
			{
				d_time = -d_time;
			}
			if (l_ncount >= 0)
			{
				d_time = d_ntime[l_ncount];
			}

			// ANT待ち受け指向なし時、2分前以前のイベントは2分とする。
			if (stpd.l_antwait_flg == 0 && d_time < -120)
			{
				d_time = -120;
			}

			d_time += d_start;
			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';
		}
		if (k < 2)
		{
			/*----- レディネスチェック処理 -----*/
			l = (stpd.l_rdychk >> k) % 2;
			if (l == 0)
			{
				continue;
			}
		}

		switch (k) {
			/*----- 初期設定処理 -----*/
		case 2:
		case 26:	/* <<XUP>> モードチェックを追加 */
			if (stpd.l_init == 0)
			{
				continue;
			}
			break;

			/*----- BITRATE-S処理 -----*/
		case 3:
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_s_bitrate);
			break;

			/*----- BITRATE-S_TRK処理 -----*/
		case 67:
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_s_bitrate);
			break;

			/*----- BITRATE-S_ACQ処理 -----*/
		case 68:
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_s_bitrate);
			break;

			/*----- BITRATE-X処理 -----*/
		case 4:
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_x_bitrate);
			break;

			/*----- BITRATE-XQPSK処理 -----*/
		case 17:
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_x_qpsk);
			break;

			/*----- BITRATE-X2処理 -----*/
		case 69:
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_x_bitrate);
			break;

			/*----- BITRATE-X_ACQ処理 -----*/
		case 70:
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_x_bitrate);
			break;

			/*----- S/X CMD_BITRATE処理 -----*/
		case 5:
			if (stpd.sz_cmd_bit_rate.GetLength() > 0)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_cmd_bit_rate);
			}
			break;

			/*----- X CMD_BITRATE処理 -----*/
		case 35:
			if (stpd.sz_x_cmd_bit_rate.GetLength() > 0)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_x_cmd_bit_rate);
			}
			break;

			/*----- PREDGET処理 -----*/
		case 27:
			/* Ｘ帯送信機ランプ制御の場合 ランプなし時の処理追加 */
			if (wcsncmp(sz_ele[0], X_TXSYN_TXT, wcslen(X_TXSYN_TXT)) == 0)
			{
				if ((stpd.l_tx_ramp == 1) || (stpd.l_tx_ramp == 2) || (stpd.l_tx_ramp == 3))
				{
					/* 送信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			/* DSN-MODEM(MOD) ランプ制御の場合 */
			if (wcsncmp(sz_ele[0], DSN_PREDGET_TXT, wcslen(DSN_PREDGET_TXT)) == 0)
			{
				if ((stpd.l_tx_ramp != 0) && (stpd.l_rx_ramp != 0) && (stpd.l_rx_ramp != 2))
				{
					/* 送/受信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			/* S帯送信機ランプ制御の場合 ランプなし時の処理追加 */
			if (wcsncmp(sz_ele[0], SS_TCRMOD_TXT, wcslen(SS_TCRMOD_TXT)) == 0)
			{
				if ((stpd.l_tx_ramp == 1) || (stpd.l_tx_ramp == 2) || (stpd.l_tx_ramp == 3))
				{	/* 送信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}
			/* Ｓ帯受信機ランプ制御の場合 */
			if (wcsncmp(sz_ele[0], S_RXSYN_TXT, wcslen(S_RXSYN_TXT)) == 0)
			{
				if ((stpd.l_rx_ramp != 0) && (stpd.l_rx_ramp != 1))
				{
					/* 受信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			/* Ｘ帯受信機ランプ制御の場合 */
			if (wcsncmp(sz_ele[0], X_RXSYN_TXT, wcslen(X_RXSYN_TXT)) == 0)
			{
				if ((stpd.l_rx_ramp != 0) && (stpd.l_rx_ramp != 2))
				{
					/* 受信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			break;

			/*----- 再生型測距使用選択処理 -----*/
		case 31:
			if (stpd.l_delay_sel != -1)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)sz_delay_sel_prm[stpd.l_delay_sel]);
			}
			break;

			/*----- 再生型測距使用選択処理(DSN-MODEM) -----*/
		case 46:
			if (stpd.l_delay_sel != -1)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)sz_delay_sel_dsn_prm[stpd.l_delay_sel]);
			}
			break;

			/*----- S-TX 送信出力選択処理 -----*/
		case 32:
#if 0
			if (stpd.sz_tx_pwr_sel != _T(""))
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_tx_pwr_sel);
			}
#else
			if (stpd.l_tx_pwr_sel != -1)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s_%s"), (LPCTSTR)sz_s_tx_pwr_sel[stpd.l_tx_pwr_sel], (LPCTSTR)sz_s_tx_uc_sel[stpd.l_tx_uc_sel]);
			}
#endif
			break;

			/*----- X-TX 送信出力選択処理 -----*/
		case 33:
#if 0
			if (stpd.sz_tx_pwr_sel != _T(""))
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_tx_pwr_sel);
			}
#else
			if (stpd.l_tx_pwr_sel != -1)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)sz_x_tx_pwr_sel[stpd.l_tx_pwr_sel]);
			}
#endif
			break;

			/*----- Ｘ帯変調装置選択処理 -----*/
		case 34:
			if (stpd.l_mod_sel != -1)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)sz_mod_sel[stpd.l_mod_sel]);
			}
			break;

		case 36: //レディネスチェック（AGC)
		{
			if (stpd.l_agc_index == 0)	// OFF
			{
				continue;
			}

			if (bAGCFirst == TRUE)
			{
				t_agc_s = stpd.l_rdy_time;
				t_agc_x = stpd.l_rdy_time;
				bAGCFirst = FALSE;
			}

			if (wcsncmp(sz_ele[1], _T("SS-TCR"), wcslen(_T("SS-TCR"))) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_s_rx == AGCRX_Y_ID)
				{
					t_rdy = GetRdyTime34(_T("AGC_S"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((t_agc_s)* 60);
					t_agc_s -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcsncmp(sz_ele[1], _T("X-RX"), wcslen(_T("X-RX"))) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_x_rx == AGCRX_Y_ID)
				{
					t_rdy = GetRdyTime34(_T("AGC_X"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((t_agc_x)* 60);
					t_agc_x -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcsncmp(sz_ele[1], _T("X-TRKRX"), wcslen(_T("X-TRKRX"))) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_x_rx == AGCRX_Y_ID)
				{
					t_rdy = GetRdyTime34(_T("AGC_X"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((t_agc_x)* 60);
					t_agc_x -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcsncmp(sz_ele[1], _T("X-QPSKRX"), wcslen(_T("X-QPSKRX"))) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_x_qpskrx == AGCRX_Y_ID)
				{
					t_rdy = GetRdyTime34(_T("AGC_XQ"));
					if (t_rdy == -1)
					{
						continue;
					}

					d_time = d_start - ((t_agc_x)* 60);
					t_agc_x -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcsncmp(sz_ele[1], _T("DSN-MODEM"), wcslen(_T("DSN-MODEM"))) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_x_rx == AGCRX_Y_ID)
				{
					t_rdy = GetRdyTime34(_T("AGC_DX"));
					if (t_rdy == -1)
					{
						continue;
					}

					d_time = d_start - ((t_agc_x)* 60);
					t_agc_x -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}

			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';

		}
		break;

		case 37: //レディネスチェック（RNG)
		{
			if (stpd.l_rng_index == 0)
			{
				continue;
			}

			if (bAGCFirst == FALSE)
			{
				long val = (t_agc_s < t_agc_x) ? t_agc_s : t_agc_x;
				stpd.l_rdy_time = val;
				bAGCFirst = TRUE;
			}

			if (wcscmp(sz_ele[1], _T("SS")) == 0)
			{
				if ((sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_S_ID) ||
					(sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_SX_ID))
				{
					t_rdy = GetRdyTime34(_T("RNG_SS"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcscmp(sz_ele[1], _T("SX")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_SX_ID)
				{
					t_rdy = GetRdyTime34(_T("RNG_SX"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcscmp(sz_ele[1], _T("DXN")) == 0)
			{
				if ((sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_X_ID) ||
					((sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_RX_ID) && (m_strRngMesMode.CompareNoCase(_T("通常")) == 0)))
				{
					t_rdy = GetRdyTime34(_T("RNG_DX"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcscmp(sz_ele[1], _T("DXR")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_RX_ID && (m_strRngMesMode.CompareNoCase(_T("再生")) == 0))
				{
					t_rdy = GetRdyTime34(_T("RNG_DR"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}

			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';
		}
		break;

		case 38: //レディネスチェック（テレメトリ伝送）
		{
			if (stpd.l_tlm_index == 0)	// OFF
			{
				continue;
			}

			if (bAGCFirst == FALSE)
			{
				long val = (t_agc_s < t_agc_x) ? t_agc_s : t_agc_x;
				stpd.l_rdy_time = val;
				bAGCFirst = TRUE;
			}

			if (wcscmp(sz_ele[1], _T("S")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_bitrate_s > 0)
				{
					t_rdy = GetRdyTime34(_T("TLM_S"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcscmp(sz_ele[1], _T("QPSK")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_bitrate_qpsk > 0)
				{
					t_rdy = GetRdyTime34(_T("TLM_XQ"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcscmp(sz_ele[1], _T("X2")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_bitrate_x2 > 0)
				{
					t_rdy = GetRdyTime34(_T("TLM_DX"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}

			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';
		}
		break;

		case 39: //レディネスチェック（コマンド伝送）
		{
			if (stpd.l_cmd_index == 0)	// OFF
			{
				continue;
			}

			if (bAGCFirst == FALSE)
			{
				long val = (t_agc_s < t_agc_x) ? t_agc_s : t_agc_x;
				stpd.l_rdy_time = val;
				bAGCFirst = TRUE;
			}

			if (wcscmp(sz_ele[1], _T("1")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_N_ID)
				{
					t_rdy = GetRdyTime34(_T("CMD_S"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcscmp(sz_ele[1], _T("3")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_Y_ID)
				{
					t_rdy = GetRdyTime34(_T("CMD_DX"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}

			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';

		}
		break;
		case 40: //アンテナ待ち受け指向　
		{
			if (stpd.l_antwait_flg == 1)
			{
				d_time = d_start - (stpd.l_ant_time * 60);
				d_time = d_time + (5 * 60); //アンテナ待ち受け通知の５分後
				CTime time(d_time);
				swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
				sz_date[19] = '\0';
			}
			else
			{
				continue;
			}

		}
		break;
		case 41:
		{
			if (wcscmp(sz_ele[1], _T("ANT")) == 0)
			{
				d_time = d_start - (stpd.l_ant_time * 60);
				if (stpd.l_antwait_flg == 0)
				{
					d_time -= 2;
				}
				CTime time(d_time);
				swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
				sz_date[19] = '\0';
			}
			else if (wcsncmp(sz_ele[1], _T("S-TLMIN"), wcslen(_T("S-TLMIN"))) == 0)
			{
				if (stpd.l_range_band != 0)
				{
					continue;
				}
			}
		}
		break;
		case 44:
			if (stpd.l_range_band != 0)
			{
				continue;
			}
			break;

			/*----- S帯RNG計測帯域選択処理 -----*/
		case 45:
			if (stpd.l_range_band != -1)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)sz_range_band_ctrl[stpd.l_range_band]);
			}
			break;

		case 47: //送信予報値制御開始　
			/* DSN-MODEM 送信ランプ制御なしの場合 */
			if (CString(sz_ele[0]) == DSN_TX_AID_ON_TXT)
			{
				if (stpd.l_tx_ramp != 0)
				{
					/* 送信機ランプ制御なしの場合は送信予報値制御開始を外す */
					continue;
				}
			}
			break;

		case 48: //受信予報値制御開始　
			/* DSN-MODEM 受信ランプ制御なしの場合 */
			if (CString(sz_ele[0]) == DSN_RX_AID_ON_TXT)
			{
				if ((stpd.l_rx_ramp != 0) && (stpd.l_rx_ramp != 2))
				{
					/* 受信機ランプ制御なしの場合は受信予報値制御開始を外す */
					continue;
				}
			}
			break;

		case 71: // CTRL.PROP_READ
			// アンテナ待ち受け指向なしの場合
			if (stpd.l_antwait_flg == 0)
			{
				d_time = d_start - (stpd.l_ant_time * 60);
				CTime time(d_time);
				swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
				sz_date[19] = '\0';
			}
			break;
		}

		if (j == 4)
		{
			wsprintf(szwline, _T("%s %s,%s,%s,%s;\n"), sz_date, sz_ele[0], sz_ele[1], sz_ele[2], sz_ele[3]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);
		}
		else if ((k == 5) || (k == 35))
		{
			if (wcslen(sz_ele[1]) > 0)
			{
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, sz_ele[0], sz_ele[1], sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}
		}
		else if (k == 12) // P-START
		{
			/* P-START*/
			wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, sz_ele[0], sz_ele[1], sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);


			/* DOWN */
			wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, DOWN_START, sz_ele[1], sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);

			wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)stpd.sz_end, DOWN_END, sz_ele[1], sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);

			/* Up Link */
			count = m_UpLinkList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_UpLinkList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, UP_START, sz_ele[1], sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_UpLinkList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, UP_END, sz_ele[1], sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* CMD */
			CString strCmdType;
			if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_Y_ID)
			{
				strCmdType = _T("X");
			}
			else if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_N_ID)
			{
				strCmdType = _T("S");
			}

			count = m_CmdList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_CmdList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, CMD_START, (LPCTSTR)strCmdType, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_CmdList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, CMD_END, (LPCTSTR)strCmdType, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* RNG */
			count = m_RngList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strRngType = m_RngList.GetItemText(i, 2);
				CString strSTime = m_RngList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, RNG_START, (LPCTSTR)ExchageRngTypePlanFile(strRngType), sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_RngList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, RNG_END, (LPCTSTR)ExchageRngTypePlanFile(strRngType), sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* TLM S */
			count = m_TlmSList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_TlmSList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, TLM_START, TLM_S, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_TlmSList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, TLM_END, TLM_S, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* TLM X */
			count = m_TlmXList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_TlmXList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, TLM_START, TLM_X, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_TlmXList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, TLM_END, TLM_X, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* TLM Ka */
			count = m_TlmKaList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_TlmKaList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, TLM_START, TLM_KA, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_TlmKaList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, TLM_END, TLM_KA, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}
		}
		else if (k == 66)	// INIT.CID
		{
			wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, sz_ele[0], (LPCTSTR)stpd.sz_cid, sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);
		}
		else
		{
			wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, sz_ele[0], sz_ele[1], sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);
		}

		/* 最初の一件は無条件でコピー */
		if (sz_s_start[0] == '\0')
		{
			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
		}
		else
		{
			if (wcscmp(sz_s_start, sz_date) > 0)
			{
				wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			}
		}
	}
	fclose(fp_plan);
	fclose(fp_smpl);


	/*---------- 計画一覧リスト作成処理 ----------*/
	TCHAR sz_plan_lst[256];

	// 衛星名16文字幅
	wcscpy_s(sz_work, static_cast<unsigned>(_countof(sz_work)), sz_satname);
#if 0
	for (l = (long)wcslen(sz_work); l < 16; ++l)
	{
		wcscat_s(sz_work, static_cast<unsigned>(_countof(sz_work)), _T(" "));
	}
#endif

	sz_work[16] = '\0';
#if 1
	swprintf_s(sz_plan_lst, static_cast<unsigned>(_countof(sz_plan_lst)), _T("%-8s %-16s %s %s %s %s %s 1 %1d %5d %-6s %-15s\n"),
		mStationString[theApp.GetSelectStation()],
		sz_work,
		(LPCTSTR)CString(sz_passno),
		(LPCTSTR)stpd.sz_start,
		(LPCTSTR)stpd.sz_end,
		sz_s_start,
		(LPCTSTR)stpd.sz_pred_path,
		stpd.l_offset,
		stpd.l_offset_val,
		PLAN_GETFORM,
		PLAN_TYPE
		);

	stPlanListData new_listplan;
	CTime cTime;

	new_listplan.strStationName = mStationString[theApp.GetSelectStation()];	// 局名
	new_listplan.strSatelliteName = sz_satname;									// 衛星名
	new_listplan.strPassId = sz_passno;											// パスID
	cTime = CSatelliteData::StrTimeToCTime(stpd.sz_start);
	new_listplan.tPassStart = cTime.GetTime();									// パス開始日時
	cTime = CSatelliteData::StrTimeToCTime(stpd.sz_end);
	new_listplan.tPassEnd = cTime.GetTime();									// パス終了日時
	cTime = CSatelliteData::StrTimeToCTime(CString(sz_s_start));
	new_listplan.tPrePassStart = cTime.GetTime();								// プリパス開始日時
	new_listplan.strPredName = stpd.sz_pred_path;								// 予報値ファイル名
	new_listplan.lValidFlag = 1;												// チェック結果フラグ
	new_listplan.lOfsFlag = stpd.l_offset;										// 時刻オフセット追従条件フラグ
	new_listplan.tOffset = stpd.l_offset_val;									// 時刻オフセット
	new_listplan.strGetFrom = PLAN_GETFORM;										// 取得先
	new_listplan.strPlanType = PLAN_TYPE;										// 計画種別
	new_listplan.strResult = _T("");											// 実行結果
#else
//	swprintf_s(sz_plan_lst, static_cast<unsigned>(_countof(sz_plan_lst)), _T("%s %s %s %s %s %s 1 %1d %5d\n"),
//		sz_work,
//		CString(sz_passno),
//		stpd.sz_start,
//		stpd.sz_end,
//		sz_s_start,
//		stpd.sz_pred_path,
//		stpd.l_offset,
//		stpd.l_offset_val
//		);
#endif

	// 詳細表示及び詳細印刷時は以降処理はしない
	if (bRegist == FALSE)
	{
		return 0;
	}

	/*---------- 計画一覧ファイル更新処理 ----------*/
	// (実際に更新していない。)

	// 計画時刻　＞　現在時刻 
	__int64 d_time2 = CSatelliteData::StrTimeToCTime(CString(sz_s_start)).GetTime() + stpd.l_offset_val;
#if 1	// UTC
	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	__int64 d_time3 = CTime(timeSys).GetTime();
#else
//	__int64 d_time3 = CTime::GetCurrentTime().GetTime();
#endif


	CTime time2(d_time2);
	CTime time3(d_time3);
	CString strTime2 = time2.Format(_T("%Y-%m-%d %H:%M:%S"));
	CString strTime3 = time3.Format(_T("%Y-%m-%d %H:%M:%S"));

	if (d_time2 < d_time3 + START_OFFSET)
	{
		CString mes = _T("");
		mes.Format(_T("制御開始時刻までに時間の余裕がないので計画を登録できません。\n\n 制御開始時刻 :%s\n パス開始時刻 :%s\n パス終了時刻 :%s\n\n 現在時刻       :%s\n ガードタイム :%d秒")
			, (LPCTSTR)strTime2, (LPCTSTR)stpd.sz_start, (LPCTSTR)stpd.sz_end, (LPCTSTR)strTime3, (int)START_OFFSET);

// CHG 2017.09.16 >>>
//		AfxMessageBox(mes, MB_OK | MB_ICONQUESTION);
		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
// CHG 2017.09.16 <<<
		DeleteFile(sz_p_file);
		return 1;
	}

	// 開始時刻　＜　終了時刻　
	if (new_listplan.tPassStart > new_listplan.tPassEnd)
	{
		CString mes = _T("");
		mes.Format(_T("パス終了時刻がパス開始時刻よりも過去のため計画を登録できません。\n\n パス開始時刻 :%s\n パス終了時刻 :%s")
			, (LPCTSTR)stpd.sz_start, (LPCTSTR)stpd.sz_end);

		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
		DeleteFile(sz_p_file);
		return 1;
	}

	// AOS < 開始時刻 , LOS > 終了時刻
	if (m_tAOS.GetTime() > new_listplan.tPassStart || m_tLOS.GetTime() < new_listplan.tPassEnd)
	{
		CString strTAos, strTLos;
		strTAos = m_tAOS.Format(_T("%Y-%m-%d %H:%M:%S"));
		strTLos = m_tLOS.Format(_T("%Y-%m-%d %H:%M:%S"));
		CString mes = _T("");
		mes.Format(_T("計画の時刻が予報値の範囲外のため計画を登録できません。\n\n AOS時刻 :%s\n LOS時刻 :%s\n\n パス開始時刻 :%s\n パス終了時刻 :%s")
			, (LPCTSTR)strTAos, (LPCTSTR)strTLos, (LPCTSTR)stpd.sz_start, (LPCTSTR)stpd.sz_end);

		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
		DeleteFile(sz_p_file);
		return 1;
	}

	vector<stPlanListData> v_plan_list2;
	stPlanListData st;

	vector<stPlanListData> &plist = theApp.GetSatelliteData().GetPlanList();
	if ((int)plist.size() > 0)
	{
		long i;
		l = (long)plist.size();
		for (i = 0; i < l; ++i)
		{
			st = plist[i];
			if ((st.strSatelliteName == sz_satname) && (st.strPassId == sz_passno))
			{
				break;
			}
		}
		if (i >= l)
		{
			l++;
		}
	}
	else
	{
		l = 1;
	}

	v_plan_list2 = plist;
	v_plan_list2.push_back(new_listplan);

	// 全時刻をオフセットを考慮したものに変更
	long n = (long)v_plan_list2.size();
	for (long i = 0; i < n; ++i)
	{
		stPlanListData &st = v_plan_list2[i];
		if (st.tOffset != 0)
		{
			// パス開始日時変更
			st.tPassStart += st.tOffset;

			// パス終了日時変更
			st.tPassEnd += st.tOffset;

			// 制御開始日時変更
			st.tPrePassStart += st.tOffset;
		}
	}

	// 計画リストソーティング＆重複チェック処理

	for (long i = (l - 1); i > 0; --i)
	{
		for (long j = 0; j < i; ++j)
		{
			stPlanListData st1 = v_plan_list2[j];
			stPlanListData st2 = v_plan_list2[j + 1];

			if (st1.tPassStart > st2.tPassEnd)
			{
				// データ入れ替え
				if (st1.tPassEnd > st2.tPassStart + START_OFFSET)
				{
					v_plan_list2[j] = st2;
					v_plan_list2[j + 1] = st1;
				}
				else
				{
					// 重複チェック異常
					CString mes1 = _T("");
					CString mes2 = _T("");

					mes1.Format(_T("%sの%sと"), (LPCTSTR)st1.strSatelliteName, (LPCTSTR)st1.strPassId);
					mes2.Format(_T("%sの%sの運用時間が重複しています。"), (LPCTSTR)st2.strSatelliteName, (LPCTSTR)st2.strPassId);
					mes1 += mes2;
					AfxMessageBox(mes1, MB_OK | MB_ICONQUESTION);
					DeleteFile(sz_p_file);
				}
			}
		}
	}

	/*----- ftp file put "sz_p_file"  -----*/
	if (PutPlanFile(sz_satname, sz_passno) == FALSE)
	{
		return -1;
	}

	return 0;
}

int	COperationPlan::WritePlanFile64(CString sz_satname, CString sz_passno, stPlandata& stpd, BOOL bRegist)
{
	FILE *fp_smpl;
	FILE *fp_plan;
	CString sz_s_file;
	CString sz_p_folder;
	CString sz_p_file;

	TCHAR sz_s_start[50];
	TCHAR sz_buff[512];
	TCHAR sz_time[50];
	TCHAR sz_ele[10][100];
	TCHAR sz_work[256];
	TCHAR sz_date[50];
	TCHAR *chp_pos;

	long j;
	long k;
	long l;
	long m;
	long l_ncount;

	long l_hour;
	long l_min;
	long l_sec;

	__int64 d_s_start;
	__int64 d_ntime[10];
	__int64 d_time;
	__int64 d_start;

	int count = 0;
	int t_rdy = 0;

	memset(&sz_ele[0], 0, sizeof(sz_ele));

	/*----- 雛形＆計画ファイルのパス名作成 -----*/
	CSatelliteData& sd = theApp.GetSatelliteData();

#if 1
	CString filenamep = theApp.GetShareFilePath(eFileType_PlanOut_Folder, theApp.GetSelectStation());
	sz_p_file.Format(_T("%s%s\\%s.pln"), (LPCTSTR)filenamep, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	sz_p_folder.Format(_T("%s%s"), (LPCTSTR)filenamep, (LPCTSTR)sz_satname.MakeLower());
	if (PathFileExists(sz_p_folder) == FALSE)
	{
		CreateDirectory(sz_p_folder, NULL);
	}

	CString filenames = theApp.GetShareFilePath(eFileType_Form_Folder, theApp.GetSelectStation());
	sz_s_file.Format(_T("%s%s.pln"), (LPCTSTR)filenames, (LPCTSTR)sz_satname);
#else
//	CString ca_cthome;
//	ca_cthome.Format(_T("%s%s"), theApp.GetAppPath(), DATA_DIR);
//
//	j = (long)strlen(sd.m_satinfo[m_IdxEqsat].sz_plan_dir);
//	if (sd.m_satinfo[m_IdxEqsat].sz_plan_dir[j - 1] == '/')
//	{
//		sz_s_file.Format(_T("%s\\plan\\form\\%s.pln"), ca_cthome, sz_satname);
//		sz_p_file.Format(_T("%s%s.pln"), CString(sd.m_satinfo[m_IdxEqsat].sz_plan_dir), sz_passno);
//	}
//	else
//	{
//		sz_s_file.Format(_T("%s\\plan\\form\\%s.pln"), ca_cthome, sz_satname);
//		sz_p_file.Format(_T("%s\\%s.pln"), CString(sd.m_satinfo[m_IdxEqsat].sz_plan_dir), sz_passno);
//	}
#endif
	sz_s_start[0] = '\0';


	if (_wfopen_s(&fp_smpl, sz_s_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("計画雛形ファイル開くことができません。\n%s"), (LPCTSTR)sz_s_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	/*---------- 開始時刻コメント用計画内容ファイル出力処理 ----------*/
	d_s_start = 0;
	for (k = 0; k < 10; k++)
	{
		d_ntime[k] = 0;
	}

	/*---------- コメント用計画内容ファイル出力処理 ----------*/

	char sz_line[512];
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_smpl) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			continue;
		}

		/*---------- 計画内容作成処理 ----------*/

		l = 0;
		if (sz_buff[0] == 'N')
		{
			if (sz_buff[1] >= 0x30 && sz_buff[1] <= 0x39)
			{
				l_ncount = sz_buff[1] - 0x30;
				k = 2;
			}
			else
			{
				l_ncount = 0;
				k = 1;
			}
		}
		else
		{
			l_ncount = -1;
			k = 0;
		}
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] != ' ')
			{
				sz_time[l] = sz_buff[k];
				l++;
			}
			else
			{
				if (l > 0)
				{
					sz_time[l] = '\0';
					break;
				}
			}
		}

		j = 0;
		l = 0;
		sz_ele[2][0] = '\0';
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] == ',') {
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_buff[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				sz_ele[j][l] = '\0';
				l = 0;
				break;
			}
			else if (sz_buff[k] != ' ')
			{
				sz_ele[j][l] = sz_buff[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			continue;
		}

		for (k = 0; sz_c_ele64[k] != _T(""); ++k)
		{
			if (sz_c_ele64[k].Find(CString(sz_ele[0])) >= 0)
			{
				break;
			}
		}

		m = swscanf_s(sz_time, _T("%d:%d:%d%s"),
			&l_hour, &l_min, &l_sec,
			sz_work, static_cast<unsigned>(_countof(sz_work)));

		if (m != 3)
		{
			if (sz_c_ele64[k] == _T(""))
			{
				continue;
			}
			fclose(fp_smpl);
			return -1;
		}
		d_time = (l_hour * 3600) + (l_min * 60) + l_sec;
		if (sz_time[0] == '-')
		{
			d_time = -d_time;
		}
		if (k < 2)
		{
			/*----- レディネスチェック処理 -----*/
			l = (stpd.l_rdychk >> k) % 2;
			if (l == 0)
			{
				continue;
			}
		}
		/*----- 初期設定処理 -----*/
		if (k == 2)
		{
			if (stpd.l_init == 0)
			{
				continue;
			}
		}
		if (l_ncount >= 0)
		{
			if (d_ntime[l_ncount] > d_time)
			{
				d_ntime[l_ncount] = d_time;
			}
		}
	}	//1回目の計画雛形ファイル読込み

	TCHAR sz_lwork[50];
	swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s"), (LPCTSTR)stpd.sz_start);
	d_start = CSatelliteData::StrTimeToCTime(CString(sz_lwork)).GetTime();
	for (k = 0; k < 9; ++k)
	{
		d_ntime[k + 1] += d_ntime[k];
	}

	fclose(fp_smpl);


	if (_wfopen_s(&fp_smpl, sz_s_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("計画雛形ファイル開くことができません。\n%s"), (LPCTSTR)sz_s_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	if (_wfopen_s(&fp_plan, sz_p_file, _T("wb")) != 0)
	{
		CString mes;
		mes.Format(_T("計画ファイル開くことができません。\n%s"), (LPCTSTR)sz_p_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		fclose(fp_smpl);
		return -1;
	}

	/*---------- コメント用計画内容ファイル出力処理 ----------*/

	TCHAR szwline[2048];
	CStringA cbuf;
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_smpl) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			if (wcsncmp(sz_buff, SOS_HEADER_ID, wcslen(SOS_HEADER_ID)) == 0)
			{
#if 1	// UTC
				SYSTEMTIME timeSys;
				GetSystemTime(&timeSys);
				CTime time = CTime(timeSys);

				CTime timegm(CTime::GetCurrentTime());
				CString str_gm = timegm.Format(_T("%Y-%m-%d %H:%M:%S"));
#else
//				CTime time(CTime::GetCurrentTime());
#endif
				CString str_date = time.Format(_T("%Y-%m-%d %H:%M:%S"));

				sz_date[19] = '\0';
				wsprintf(szwline, _T("%s PLAN %s     1 %2d %s %s\n"),
					SOS_HEADER_ID,
					(LPCTSTR)CString(str_date),
					sd.m_satinfo[m_IdxEqsat].l_sat_no,
					(LPCTSTR)CString(sz_satname).MakeUpper(),
					(LPCTSTR)stpd.sz_pred_path);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# 衛星名=%s パス番号=%s RDYCHK=%s\n"),
					(LPCTSTR)CString(sz_satname).MakeUpper(),
					(LPCTSTR)CString(sz_passno),
					(LPCTSTR)sz_rdychk[stpd.l_rdychk]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele64[2], (LPCTSTR)sz_on[stpd.l_init],
					(LPCTSTR)sz_c_ele64[3], (LPCTSTR)stpd.sz_s_bitrate,
					(LPCTSTR)sz_c_ele64[4], (LPCTSTR)stpd.sz_x_bitrate,
					(LPCTSTR)sz_c_ele64[17], (LPCTSTR)stpd.sz_x_qpsk);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

//				if (stpd.l_mod_sel == 0)
				if (stpd.sz_cmd_bit_rate.GetLength() > 0)
				{
					wsprintf(szwline, _T("# %s=%s %s=%s %s=%s\n"),
						(LPCTSTR)sz_c_ele64[5], (LPCTSTR)stpd.sz_cmd_bit_rate,
						(LPCTSTR)sz_c_ele64[12], (LPCTSTR)stpd.sz_start,
						(LPCTSTR)sz_c_ele64[13], (LPCTSTR)stpd.sz_end);
					cbuf = CServerFile::ConvertEuc(szwline);
					fputs(cbuf, fp_plan);
				}
//				else if (stpd.l_mod_sel == 1)
				else if (stpd.sz_x_cmd_bit_rate.GetLength() > 0)
				{
					wsprintf(szwline, _T("# %s=%s %s=%s %s=%s\n"),
						(LPCTSTR)sz_c_ele64[36], (LPCTSTR)stpd.sz_x_cmd_bit_rate,
						(LPCTSTR)sz_c_ele64[12], (LPCTSTR)stpd.sz_start,
						(LPCTSTR)sz_c_ele64[13], (LPCTSTR)stpd.sz_end);
					cbuf = CServerFile::ConvertEuc(szwline);
					fputs(cbuf, fp_plan);
				}
#if 1
				else
				{
					wsprintf(szwline, _T("# %s=%s %s=%s %s=%s\n"),
						(LPCTSTR)sz_c_ele64[36], (LPCTSTR)stpd.sz_x_cmd_bit_rate,
						(LPCTSTR)sz_c_ele64[12], (LPCTSTR)stpd.sz_start,
						(LPCTSTR)sz_c_ele64[13], (LPCTSTR)stpd.sz_end);
					cbuf = CServerFile::ConvertEuc(szwline);
					fputs(cbuf, fp_plan);
				}
#endif

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele64[16], (LPCTSTR)sz_on[stpd.l_auto_acq],
					(LPCTSTR)sz_c_ele64[11], (LPCTSTR)sz_on[stpd.l_offset],
					(LPCTSTR)sz_c_ele64[14], (LPCTSTR)sz_on[stpd.l_a_flag],
					(LPCTSTR)sz_c_ele64[15], (LPCTSTR)sz_on[stpd.l_l_flag],
					(LPCTSTR)sz_c_ele64[18], (LPCTSTR)sz_term_ant[stpd.l_term_ant]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele64[6], (LPCTSTR)sz_auto[stpd.l_cmd_mod],
					(LPCTSTR)sz_c_ele64[7], (LPCTSTR)sz_auto[stpd.l_rng_mod],
					(LPCTSTR)sz_c_ele64[8], (LPCTSTR)stpd.sz_rng_band,
					(LPCTSTR)sz_c_ele64[9], (LPCTSTR)stpd.sz_rng_start,
					(LPCTSTR)sz_c_ele64[10], (LPCTSTR)stpd.sz_rng_end);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%d\n"),
					(LPCTSTR)sz_c_ele64[19], (LPCTSTR)sz_tx_ramp[stpd.l_tx_ramp],
					(LPCTSTR)sz_c_ele64[20], (LPCTSTR)sz_rx_ramp[stpd.l_rx_ramp],
					(LPCTSTR)sz_c_ele64[21], stpd.l_acq_route + 1);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%d %s=%d %s=%s\n"),
					(LPCTSTR)sz_c_ele64[22], (LPCTSTR)sz_on_off[stpd.l_pl_sel],
					(LPCTSTR)sz_c_ele64[23], (LPCTSTR)sz_on_off[stpd.l_top_mon],
					(LPCTSTR)sz_c_ele64[24], stpd.l_ent_angle,
					(LPCTSTR)sz_c_ele64[25], stpd.l_ext_angle,
					(LPCTSTR)sz_c_ele64[33], (LPCTSTR)sz_delay_sel[stpd.l_delay_sel]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele64[47], (LPCTSTR)stpd.sz_cid,								// CID(CID)
					(LPCTSTR)sz_c_ele64[48], mStationSimpleString[theApp.GetSelectStation()],	// 局名(ST_NAME)
					(LPCTSTR)sz_c_ele64[49], _T("MASTER"),										// 主従局(PRI):MASTER固定
					(LPCTSTR)sz_c_ele64[50], (LPCTSTR)GetOpeMode());							// 運用モード(OPE_MODE)
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s\n"),
					(LPCTSTR)sz_c_ele64[61], _T(""));											// Auto
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				continue;
			}
			cbuf = CServerFile::ConvertEuc(sz_buff);
			fputs(cbuf, fp_plan);
			continue;
		}

		/*---------- 計画内容作成処理 ----------*/

		l = 0;
		if (sz_buff[0] == 'N')
		{
			if (sz_buff[1] >= 0x30 && sz_buff[1] <= 0x39)
			{
				l_ncount = sz_buff[1] - 0x30;
				k = 2;
			}
			else
			{
				l_ncount = 0;
				k = 1;
			}
		}
		else
		{
			l_ncount = -1;
			k = 0;
		}
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] != ' ') {
				sz_time[l] = sz_buff[k];
				l++;
			}
			else
			{
				if (l > 0)
				{
					sz_time[l] = '\0';
					break;
				}
			}
		}

		j = 0;
		l = 0;
		sz_ele[2][0] = '\0';
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] == ',')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_buff[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				sz_ele[j][l] = '\0';
				l = 0;
				break;
			}
			else if (sz_buff[k] != ' ')
			{
				sz_ele[j][l] = sz_buff[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			cbuf = CServerFile::ConvertEuc(sz_buff);
			fputs(cbuf, fp_plan);
			continue;
		}

		for (k = 0; sz_c_ele64[k] != _T(""); ++k)
		{
			if ((k == 2) || (k == 4) || (k == 35))
			{
				if (sz_c_ele64[k].Find(CString(sz_ele[0])) >= 0)
				{
					if ((k == 4) && (sz_c_ele64[17] == CString(sz_ele[0])))
					{
						continue;
					}
					break;
				}
			}
			else if (k == 27) {
				/* PREDGETを確認する */
				chp_pos = wcschr(sz_ele[0], '.');
				if (chp_pos != (TCHAR *)NULL)
				{
					chp_pos++;
					if (sz_c_ele64[k].Find(CString(chp_pos)) >= 0)
					{
						break;
					}
				}
			}
			else
			{
				if (sz_c_ele64[k] == CString(sz_ele[0]))
				{
					break;
				}
			}
		}

		if (k == 12)
		{
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)stpd.sz_start);
		}
		else if (k == 13)
		{
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)stpd.sz_end);
		}
		else
		{
			m = swscanf_s(sz_time, _T("%d:%d:%d%s"), &l_hour, &l_min, &l_sec, sz_work, static_cast<unsigned>(_countof(sz_work)));
			if (m != 3)
			{
				if (sz_c_ele64[k] == _T(""))
				{
					cbuf = CServerFile::ConvertEuc(sz_buff);
					fputs(cbuf, fp_plan);
					continue;
				}
				fclose(fp_smpl);
				fclose(fp_plan);
				return -1;
			}
			d_time = (l_hour * 3600) + (l_min * 60) + l_sec;
			if (sz_time[0] == '-')
			{
				d_time = -d_time;
			}
			if (l_ncount >= 0)
			{
				d_time = d_ntime[l_ncount];
			}

			// ANT待ち受け指向なし時、2分前以前のイベントは2分とする。
			if (stpd.l_antwait_flg == 0 && d_time < -120)
			{
				d_time = -120;
			}

			d_time += d_start;
			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';
		}
		if (k < 2)
		{
			/*----- レディネスチェック処理 -----*/
			l = (stpd.l_rdychk >> k) % 2;
			if (l == 0)
			{
				continue;
			}
		}

		switch (k) {
			/*----- 初期設定処理 -----*/
		case 2:
		case 26:	/* <<XUP>> モードチェックを追加 */
			if (stpd.l_init == 0)
			{
				continue;
			}
			break;

			/*----- BITRATE-S処理 -----*/
		case 3:
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_s_bitrate);
			break;

			/*----- BITRATE-X処理 -----*/
		case 4:
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_x_bitrate);
			break;

			/*----- BITRATE-XQPSK処理 -----*/
		case 17:
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_x_qpsk);
			break;

			/*----- S/X CMD_BITRATE処理 -----*/
		case 5:
			if (stpd.sz_cmd_bit_rate.GetLength() > 0)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_cmd_bit_rate);
			}
			break;

			/*----- X CMD_BITRATE処理 -----*/
		case 36:
		case 43:
			if (stpd.sz_x_cmd_bit_rate.GetLength() > 0)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_x_cmd_bit_rate);
			}
			break;

			/*----- PREDGET処理 -----*/
		case 27:
			/* Ｘ帯送信機ランプ制御の場合 ランプなし時の処理追加 */
			if (wcsncmp(sz_ele[0], X_TXSYN_TXT, wcslen(X_TXSYN_TXT)) == 0)
			{
				if ((stpd.l_tx_ramp == 1) || (stpd.l_tx_ramp == 2) || (stpd.l_tx_ramp == 3))
				{
					/* 送信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			/* DSN-MODEM(MOD) ランプ制御の場合 */
			if (wcsncmp(sz_ele[0], DSN_PREDGET_TXT, wcslen(DSN_PREDGET_TXT)) == 0)
			{
				if ((stpd.l_tx_ramp != 0) && (stpd.l_rx_ramp != 0) && (stpd.l_rx_ramp != 2))
				{
					/* 送/受信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			/* S帯送信機ランプ制御の場合 ランプなし時の処理追加 */
			if (wcsncmp(sz_ele[0], SS_TCRMOD_TXT, wcslen(SS_TCRMOD_TXT)) == 0)
			{
				if ((stpd.l_tx_ramp == 1) || (stpd.l_tx_ramp == 2) || (stpd.l_tx_ramp == 3))
				{
					/* 送信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}
			/* Ｓ帯Ｄ／Ｃ　ＳＹＮ１ランプ制御の場合 */
			if (wcsncmp(sz_ele[0], S_DCSYN_TXT, wcslen(S_DCSYN_TXT)) == 0) 
			{
				if ((stpd.l_rx_ramp != 0) && (stpd.l_rx_ramp != 1))
				{
					/* 受信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			/* Ｘ帯受信機１ランプ制御の場合 */
			if (wcsncmp(sz_ele[0], X_RX1SYN_TXT, wcslen(X_RX1SYN_TXT)) == 0) 
			{
				if ((stpd.l_rx_ramp != 0) && (stpd.l_rx_ramp != 2))
				{
					/* 受信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			/* Ｘ帯受信機２ランプ制御の場合 */
			if (wcsncmp(sz_ele[0], X_RX2SYN_TXT, wcslen(X_RX2SYN_TXT)) == 0)
			{
				if ((stpd.l_rx_ramp != 0) && (stpd.l_rx_ramp != 2))
				{
					/* 受信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			/* Ｘ帯高速受信機ランプ制御の場合 */
			if (wcsncmp(sz_ele[0], X_QPSKSYN_TXT, wcslen(X_QPSKSYN_TXT)) == 0)
			{
				if ((stpd.l_rx_ramp != 0) && (stpd.l_rx_ramp != 2))
				{
					/* 受信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			break;

			/*----- Ｓ帯衛星送信情報処理 -----*/
		case 28:
#if 1
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s-%s"), _T("H"), _T("A"));
#else
//			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s-%s"), (LPCTSTR)stpd.sz_level_s, (LPCTSTR)stpd.sz_ant_s);
#endif
			break;

			/*----- Ｘ帯衛星送信情報処理 -----*/
		case 29:
#if 1
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s-%s"), _T("H"), _T("A"));
#else
//			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s-%s"), (LPCTSTR)stpd.sz_level_x, (LPCTSTR)stpd.sz_ant_x);
#endif
			break;

			/*----- ４ＷＡＹ運用有無 -----*/
		case 30:
			if (wcscmp(sz_ele[1], _T("CTRL")) == 0) 
			{
				if (stpd.l_selene_4way == 0) 
				{
					wcscpy_s(sz_ele[1], CTRL_4WAY);
				}
			}
			else if (wcscmp(sz_ele[1], _T("CTRL_OLD")) == 0) 
			{
				if (stpd.l_selene_4way == 0) 
				{
					wcscpy_s(sz_ele[1], static_cast<unsigned>(_countof(sz_s_start)), CTRL_4WAY);
					wcscat_s(sz_ele[1], static_cast<unsigned>(_countof(sz_s_start)), CTRL_OLD);
				}
			}
			else if (wcscmp(sz_ele[1], _T("ANT")) == 0) 
			{
				d_time = d_start - (stpd.l_ant_time * 60);
				if (stpd.l_antwait_flg == 0)
				{
					d_time -= 2;
				}
				CTime time(d_time);
				swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
				sz_date[19] = '\0';
			}
			break;

			/*----- S-TX 送信出力選択処理 -----*/
		case 31:
#if 0
			if (stpd.sz_tx_pwr_sel != _T(""))
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_tx_pwr_sel);
			}
#else
			if (stpd.l_tx_pwr_sel != -1)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)sz_tx_pwr_sel64[stpd.l_tx_pwr_sel]);
			}
#endif
			break;

			/*----- X-TX 送信出力選択処理 -----*/
		case 32:
#if 0
			if (stpd.sz_tx_pwr_sel != _T(""))
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_tx_pwr_sel);
			}
#else
			if (stpd.l_tx_pwr_sel != -1)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)sz_tx_pwr_sel64[stpd.l_tx_pwr_sel]);
			}
#endif
			break;

			/*----- 再生型測距使用選択処理 -----*/
		case 34:
			if (stpd.l_delay_sel != -1)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)sz_delay_sel_prm[stpd.l_delay_sel]);
			}
			break;

			/*----- 再生型測距使用選択処理(DSN-MODEM) -----*/
		case 42:
			if (stpd.l_delay_sel != -1)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)sz_delay_sel_dsn_prm[stpd.l_delay_sel]);
			}
			break;

			/*----- Ｘ帯変調装置選択処理 -----*/
		case 35:
			if (stpd.l_mod_sel != -1)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)sz_mod_sel[stpd.l_mod_sel]);
			}
			break;

		case 37: //レディネスチェック（AGC)
		{
			if (stpd.l_agc_index == 0)	// OFF
			{
				continue;
			}

			if (wcscmp(sz_ele[1], _T("S-RX_A")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_s_rx == AGCRX_Y_ID)
				{
					t_rdy = GetRdyTime64(_T("AGC_S"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
#if 0	// 新系のみ実施の為削除
			else if (wcscmp(sz_ele[1], _T("X-RX2_A")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_x_rx2 == AGCRX_Y_ID)
				{
					t_rdy = GetRdyTime64(_T("AGC_X"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
#endif
			else if(wcscmp(sz_ele[1], _T("DSN-MODEM_A")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_x_rx2 == AGCRX_Y_ID)
				{
					t_rdy = GetRdyTime64(_T("AGC_DX"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}

			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';
		}
		break;

		case 38: //レディネスチェック（RNG)
		{
			if (stpd.l_rng_index == 0)	// OFF
			{
				continue;
			}

			if (wcscmp(sz_ele[1], _T("SS")) == 0)
			{
				if ( (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_S_ID) ||
					(sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_SX_ID))
				{
					t_rdy = GetRdyTime64(_T("RNG_SS"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcscmp(sz_ele[1], _T("SX")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_SX_ID)
				{
					t_rdy = GetRdyTime64(_T("RNG_SS"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
#if 0	// 新系のみ実施の為削除
			else if (wcscmp(sz_ele[1], _T("XX")) == 0)
			{
				if ((sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_X_ID) ||
					((sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_RX_ID) && (m_strRngMesMode.CompareNoCase(_T("通常")) == 0)))
				{
					t_rdy = GetRdyTime64(_T("RNG_XX"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcscmp(sz_ele[1], _T("RX")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_RX_ID && (m_strRngMesMode.CompareNoCase(_T("再生")) == 0))
				{
					t_rdy = GetRdyTime64(_T("RNG_RX"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
#endif
			else if (wcscmp(sz_ele[1], _T("DXN")) == 0)
			{
				if ((sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_X_ID) ||
					((sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_RX_ID) && (m_strRngMesMode.CompareNoCase(_T("通常")) == 0)))
				{
					t_rdy = GetRdyTime64(_T("RNG_DX"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcscmp(sz_ele[1], _T("DXR")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_RX_ID && (m_strRngMesMode.CompareNoCase(_T("再生")) == 0))
				{
					t_rdy = GetRdyTime64(_T("RNG_DR"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}

			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';
		}
		break;

		case 39: //レディネスチェック（テレメトリ伝送）
		{
			if (stpd.l_tlm_index == 0)	// OFF
			{
				continue;
			}

			if (wcscmp(sz_ele[1], _T("S")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_bitrate_s > 0)
				{
					t_rdy = GetRdyTime64(_T("TLM_S"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
#if 0	// 新系のみ実施の為削除
			else if (wcscmp(sz_ele[1], _T("X")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_bitrate_x > 0)
				{
					t_rdy = GetRdyTime64(_T("TLM_X"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
#endif
			else if (wcscmp(sz_ele[1], _T("X2")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_bitrate_x2 > 0)
				{
					t_rdy = GetRdyTime64(_T("TLM_DX"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}

			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';
		}
		break;

		case 40: //レディネスチェック（コマンド伝送）
		{
			if (stpd.l_cmd_index == 0)	// OFF
			{
				continue;
			}

			if (wcscmp(sz_ele[1], _T("2")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_N_ID)
				{
					t_rdy = GetRdyTime64(_T("CMD_S"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
#if 0	// 新系のみ実施の為削除
			else if (wcscmp(sz_ele[1], _T("3")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_Y_ID)
				{
					t_rdy = GetRdyTime64(_T("CMD_X"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
#endif
			else if(wcscmp(sz_ele[1], _T("4")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_Y_ID)
				{
					t_rdy = GetRdyTime64(_T("CMD_DX"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}

			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';
		}
		break;

		case 41: //アンテナ待ち受け指向　
		{
			if (stpd.l_antwait_flg == 1)
			{
				d_time = d_start - (stpd.l_ant_time * 60);
				d_time = d_time + (5 * 60); //アンテナ待ち受け通知の５分後
				CTime time(d_time);
				swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
				sz_date[19] = '\0';
			}
			else
			{
				continue;
			}
		}
		break;

		case 44: //送信予報値制御開始　
			/* DSN-MODEM 送信ランプ制御なしの場合 */
			if (CString(sz_ele[0]) == DSN_TX_AID_ON_TXT)
			{
				if (stpd.l_tx_ramp != 0)
				{
					/* 送信機ランプ制御なしの場合は送信予報値制御開始を外す */
					continue;
				}
			}
			break;

		case 45: //受信予報値制御開始　
			/* DSN-MODEM 受信ランプ制御なしの場合 */
			if (CString(sz_ele[0]) == DSN_RX_AID_ON_TXT)
			{
				if ((stpd.l_rx_ramp != 0) && (stpd.l_rx_ramp != 2))
				{
					/* 受信機ランプ制御なしの場合は受信予報値制御開始を外す */
					continue;
				}
			}
			break;

		case 63: // CTRL.PROP_READ
			// アンテナ待ち受け指向なしの場合
			if (stpd.l_antwait_flg == 0)
			{
				d_time = d_start - (stpd.l_ant_time * 60);
				CTime time(d_time);
				swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
				sz_date[19] = '\0';
			}
			break;
		}

		if (j == 4)
		{
			wsprintf(szwline, _T("%s %s,%s,%s,%s;\n"), sz_date, sz_ele[0], sz_ele[1], sz_ele[2], sz_ele[3]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);
		}
		else if ((k == 5) || (k == 35))
		{
			if (wcslen(sz_ele[1]) > 0)
			{
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, sz_ele[0], sz_ele[1], sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}
		}
		else if (k == 12) // P-START
		{
			/* P-START*/
			wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, sz_ele[0], sz_ele[1], sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);


			/* DOWN */
			wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, DOWN_START, sz_ele[1], sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);

			wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)stpd.sz_end, DOWN_END, sz_ele[1], sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);

			/* Up Link */
			count = m_UpLinkList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_UpLinkList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, UP_START, sz_ele[1], sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_UpLinkList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, UP_END, sz_ele[1], sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* CMD */
			CString strCmdType = _T("");
			if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_Y_ID)
			{
				strCmdType = _T("X");
			}
			else if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_N_ID)
			{
				strCmdType = _T("S");
			}

			count = m_CmdList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_CmdList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, CMD_START, (LPCTSTR)strCmdType, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_CmdList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, CMD_END, (LPCTSTR)strCmdType, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* RNG */
			count = m_RngList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strRngType = m_RngList.GetItemText(i, 2);
				CString strSTime = m_RngList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, RNG_START, (LPCTSTR)ExchageRngTypePlanFile(strRngType), sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_RngList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, RNG_END, (LPCTSTR)ExchageRngTypePlanFile(strRngType), sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* TLM S */
			count = m_TlmSList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_TlmSList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, TLM_START, TLM_S, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_TlmSList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, TLM_END, TLM_S, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* TLM X */
			count = m_TlmXList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_TlmXList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, TLM_START, TLM_X, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_TlmXList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, TLM_END, TLM_X, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* TLM Ka */
			count = m_TlmKaList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_TlmKaList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, TLM_START, TLM_KA, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_TlmKaList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, TLM_END, TLM_KA, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}
		}
		else if (k == 62)	// INIT.CID
		{
			wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, sz_ele[0], (LPCTSTR)stpd.sz_cid, sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);
		}
		else
		{
			wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, sz_ele[0], sz_ele[1], sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);
		}

		/* 最初の一件は無条件でコピー */
		if (sz_s_start[0] == '\0')
		{
			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
		}
		else
		{
			if (wcscmp(sz_s_start, sz_date) > 0)
			{
				wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			}
		}
	}	//2回目の計画雛形ファイル読込み
	fclose(fp_plan);
	fclose(fp_smpl);

	/*---------- 計画一覧リスト作成処理 ----------*/
	TCHAR sz_plan_lst[256];

	// 衛星名16文字幅
	wcscpy_s(sz_work, static_cast<unsigned>(_countof(sz_work)), sz_satname);
#if 0
	for (l = (long)wcslen(sz_work); l < 16; ++l)
	{
		wcscat_s(sz_work, static_cast<unsigned>(_countof(sz_work)), _T(" "));
	}
#endif

	sz_work[16] = '\0';
#if 1
	swprintf_s(sz_plan_lst, static_cast<unsigned>(_countof(sz_plan_lst)), _T("%-8s %-16s %s %s %s %s %s 1 %1d %5d %-6s %-15s"),
		mStationString[theApp.GetSelectStation()],
		sz_work,
		(LPCTSTR)CString(sz_passno),
		(LPCTSTR)stpd.sz_start,
		(LPCTSTR)stpd.sz_end,
		sz_s_start,
		(LPCTSTR)stpd.sz_pred_path,
		stpd.l_offset,
		stpd.l_offset_val,
		PLAN_GETFORM,
		PLAN_TYPE
		);

	stPlanListData new_listplan;
	CTime cTime;

	new_listplan.strStationName = mStationString[theApp.GetSelectStation()];	// 局名
	new_listplan.strSatelliteName = sz_satname;									// 衛星名
	new_listplan.strPassId = sz_passno;											// パスID
	cTime = CSatelliteData::StrTimeToCTime(stpd.sz_start);
	new_listplan.tPassStart = cTime.GetTime();									// パス開始日時
	cTime = CSatelliteData::StrTimeToCTime(stpd.sz_end);
	new_listplan.tPassEnd = cTime.GetTime();									// パス終了日時
	cTime = CSatelliteData::StrTimeToCTime(CString(sz_s_start));
	new_listplan.tPrePassStart = cTime.GetTime();								// プリパス開始日時
	new_listplan.strPredName = stpd.sz_pred_path;								// 予報値ファイル名
	new_listplan.lValidFlag = 1;												// チェック結果フラグ
	new_listplan.lOfsFlag = stpd.l_offset;										// 時刻オフセット追従条件フラグ
	new_listplan.tOffset = stpd.l_offset_val;									// 時刻オフセット
	new_listplan.strGetFrom = PLAN_GETFORM;										// 取得先
	new_listplan.strPlanType = PLAN_TYPE;										// 計画種別
	new_listplan.strResult = _T("");											// 実行結果
#else
//	swprintf_s(sz_plan_lst, static_cast<unsigned>(_countof(sz_plan_lst)), _T("%s %s %s %s %s %s 1 %1d %5d\n"),
//		sz_work,
//		CString(sz_passno),
//		stpd.sz_start,
//		stpd.sz_end,
//		sz_s_start,
//		stpd.sz_pred_path,
//		stpd.l_offset,
//		stpd.l_offset_val
//		);
#endif

	// 詳細表示及び詳細印刷時は以降処理はしない
	if (bRegist == FALSE)
	{
		return 0;
	}

	/*---------- 計画一覧ファイル更新処理 ----------*/
	// (実際に更新していない。)

	// 計画時刻　＞　現在時刻 
	__int64 d_time2 = CSatelliteData::StrTimeToCTime(CString(sz_s_start)).GetTime() + stpd.l_offset_val;
#if 1	// UTC
	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	__int64 d_time3 = CTime(timeSys).GetTime();
#else
//	__int64 d_time3 = CTime::GetCurrentTime().GetTime();
#endif

	CTime time2(d_time2);
	CTime time3(d_time3);
	CString strTime2 = time2.Format(_T("%Y-%m-%d %H:%M:%S"));
	CString strTime3 = time3.Format(_T("%Y-%m-%d %H:%M:%S"));

	if (d_time2 < d_time3 + START_OFFSET)
	{
		CString mes = _T("");
		mes.Format(_T("制御開始時刻までに時間の余裕がないので計画を登録できません。\n\n 制御開始時刻 :%s\n パス開始時刻 :%s\n パス終了時刻 :%s\n\n 現在時刻       :%s\n ガードタイム :%d秒")
			, (LPCTSTR)strTime2, (LPCTSTR)stpd.sz_start, (LPCTSTR)stpd.sz_end, (LPCTSTR)strTime3, (int)START_OFFSET);

// CHG 2017.09.16 >>>
//		AfxMessageBox(mes, MB_OK | MB_ICONQUESTION);
		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
// CHG 2017.09.16 <<<
		DeleteFile(sz_p_file);
		return 1;
	}

	// 開始時刻　＜　終了時刻　
	if (new_listplan.tPassStart > new_listplan.tPassEnd)
	{
		CString mes = _T("");
		mes.Format(_T("パス終了時刻がパス開始時刻よりも過去のため計画を登録できません。\n\n パス開始時刻 :%s\n パス終了時刻 :%s")
			, (LPCTSTR)stpd.sz_start, (LPCTSTR)stpd.sz_end);

		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
		DeleteFile(sz_p_file);
		return 1;
	}

	// AOS < 開始時刻 , LOS > 終了時刻
	if (m_tAOS.GetTime() > new_listplan.tPassStart || m_tLOS.GetTime() < new_listplan.tPassEnd)
	{
		CString strTAos, strTLos;
		strTAos = m_tAOS.Format(_T("%Y-%m-%d %H:%M:%S"));
		strTLos = m_tLOS.Format(_T("%Y-%m-%d %H:%M:%S"));
		CString mes = _T("");
		mes.Format(_T("計画の時刻が予報値の範囲外のため計画を登録できません。\n\n AOS時刻 :%s\n LOS時刻 :%s\n\n パス開始時刻 :%s\n パス終了時刻 :%s")
			, (LPCTSTR)strTAos, (LPCTSTR)strTLos, (LPCTSTR)stpd.sz_start, (LPCTSTR)stpd.sz_end);

		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
		DeleteFile(sz_p_file);
		return 1;
	}

	vector<stPlanListData> v_plan_list2;
	stPlanListData st;

	vector<stPlanListData> &plist = theApp.GetSatelliteData().GetPlanList();
	if ((int)plist.size() > 0)
	{
		long i;
		l = (long)plist.size();
		for (i = 0; i < l; ++i)
		{
			st = plist[i];
			if ((st.strSatelliteName == sz_satname) && (st.strPassId == sz_passno))
			{
				break;
			}
		}
		if (i >= l)
		{
			l++;
		}
	}
	else
	{
		l = 1;
	}

	v_plan_list2 = plist;
	v_plan_list2.push_back(new_listplan);

	// 全時刻をオフセットを考慮したものに変更
	long n = (long)v_plan_list2.size();
	for (long i = 0; i < n; ++i)
	{
		stPlanListData &st = v_plan_list2[i];
		if (st.tOffset != 0)
		{
			// パス開始日時変更
			st.tPassStart += st.tOffset;

			// パス終了日時変更
			st.tPassEnd += st.tOffset;

			// 制御開始日時変更
			st.tPrePassStart += st.tOffset;
		}
	}

	// 計画リストソーティング＆重複チェック処理

	for (long i = (l - 1); i > 0; --i)
	{
		for (long j = 0; j < i; ++j)
		{
			stPlanListData st1 = v_plan_list2[j];
			stPlanListData st2 = v_plan_list2[j + 1];

			if (st1.tPassStart > st2.tPassEnd)
			{
				// データ入れ替え
				if (st1.tPassEnd > st2.tPassStart + START_OFFSET)
				{
					v_plan_list2[j] = st2;
					v_plan_list2[j + 1] = st1;
				}
				else
				{
					// 重複チェック異常
					CString mes1 = _T("");
					CString mes2 = _T("");

					mes1.Format(_T("%sの%sと"), (LPCTSTR)st1.strSatelliteName, (LPCTSTR)st1.strPassId);
					mes2.Format(_T("%sの%sの運用時間が重複しています。"), (LPCTSTR)st2.strSatelliteName, (LPCTSTR)st2.strPassId);
					mes1 += mes2;
					AfxMessageBox(mes1, MB_OK | MB_ICONQUESTION);
					DeleteFile(sz_p_file);
				}
			}
		}
	}

#if 0
#ifdef _NOCOMM
	FILE *fp_plist;

	if (m_bNewPlan == TRUE)
	{
		CString fPlist = theApp.GetShareFilePath(eFileType_PlanList_File, theApp.GetSelectStation());

		if (_wfopen_s(&fp_plist, fPlist, _T("r+")) == 0)
		{
			fseek(fp_plist, 0, SEEK_END);

			wsprintf(szwline, _T("%s\n"), sz_plan_lst);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plist);

			fclose(fp_plist);
		}
	}

#endif
#endif

	/*----- ftp file put "sz_p_file"  -----*/
	if (PutPlanFile(sz_satname, sz_passno) == FALSE)
	{
		return -1;
	}

	return 0;
}

/*============================================================================*/
/*! COperationPlan

-# 受信環境設定ルーチン

@param  なし
@retval 0：正常終了、0以外：エラー
*/
/*============================================================================*/
int	COperationPlan::rev_env_set()
{
	return 0;
}


/*============================================================================*/
/*! COperationPlan

-# 計画IDより、計画ファイルパスを取得する

@param  なし
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
CString COperationPlan::GetFilePath()
{
	CString file, str, pid;

	// 基本パラメータ-計画ID
	m_PlanIDEdit.GetWindowText(str);
	pid = m_PassIDMask + _T("-") + str;

	CString dir = theApp.GetShareFilePath(eFileType_PlanOut_Folder, theApp.GetSelectStation());
	file.Format(_T("%s%s\\%s.pln"), (LPCTSTR)dir, (LPCTSTR)m_strSatellite, (LPCTSTR)pid);

	return file;
}


/*============================================================================*/
/*! COperationPlan

-# レディネスチェック時刻取得

@param  なし
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
int COperationPlan::GetRdyTime20(CString str)
{
	int time = -1;

	CSatelliteData &sd = theApp.GetSatelliteData();
	CDBAccess &dba = theApp.GetSatelliteData().GetDBAccessCls();

#if 1
	time = dba.GetRdyChkTimeFile(str);
#else
//	if (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_S_ID)
//	{
//		if (sd.m_satinfo[m_IdxEqsat].i_bitrate_qpsk > 0)
//		{
//			/* SS帯RNG S帯CMD S帯TLM X帯高速TLM */
//			if ((str.CompareNoCase(_T("AGC_S")) == 0)
//				|| (str.CompareNoCase(_T("AGC_XQ")) == 0)
//				|| (str.CompareNoCase(_T("RNG_SS")) == 0)
//				|| (str.CompareNoCase(_T("TLM_S")) == 0)
//				|| (str.CompareNoCase(_T("CMD_S")) == 0))
//			{
//				time = dba.GetRdyChkTimeFile(str);
//			}
//		}
//		else
//		{
//			/* SS帯RNG S帯CMD S帯TLM X帯中低速TLM */
//			if ((str.CompareNoCase(_T("AGC_S")) == 0)
//				|| (str.CompareNoCase(_T("AGC_X")) == 0)
//				|| (str.CompareNoCase(_T("RNG_SS")) == 0)
//				|| (str.CompareNoCase(_T("TLM_S")) == 0)
//				|| (str.CompareNoCase(_T("TLM_X")) == 0)
//				|| (str.CompareNoCase(_T("CMD_S")) == 0))
//			{
//				time = dba.GetRdyChkTimeFile(str);
//			}
//		}
//	}
//	else if (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_SX_ID)
//	{
//		/* SX帯RNG S帯CMD X帯中低速TLM */
//		if ((str.CompareNoCase(_T("AGC_X")) == 0)
//			|| (str.CompareNoCase(_T("RNG_SX")) == 0)
//			|| (str.CompareNoCase(_T("TLM_S")) == 0)
//			|| (str.CompareNoCase(_T("TLM_X")) == 0)
//			|| (str.CompareNoCase(_T("CMD_S")) == 0))
//		{
//			time = dba.GetRdyChkTimeFile(str);
//		}
//	}
#endif

	return time;
}

int COperationPlan::GetRdyTime34(CString str)
{
	int time = -1;

	CSatelliteData &sd = theApp.GetSatelliteData();
	CDBAccess &dba = theApp.GetSatelliteData().GetDBAccessCls();

#if 1
	time = dba.GetRdyChkTimeFile(str);
#else
//	if (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_S_ID)
//	{
//		if (sd.m_satinfo[m_IdxEqsat].i_bitrate_qpsk > 0)
//		{
//			/* SS帯RNG S帯CMD S帯TLM X帯高速TLM */
//			if ((str.CompareNoCase(_T("AGC_S")) == 0)
//				|| (str.CompareNoCase(_T("RNG_SS")) == 0)
//				|| (str.CompareNoCase(_T("TLM_S")) == 0)
//				|| (str.CompareNoCase(_T("CMD_S")) == 0))
//			{
//				time = dba.GetRdyChkTimeFile(str);
//			}
//		}
//		else
//		{
//			/* SS帯RNG S帯CMD S帯TLM */
//			if ((str.CompareNoCase(_T("AGC_S")) == 0)
//				|| (str.CompareNoCase(_T("AGC_XQ")) == 0)
//				|| (str.CompareNoCase(_T("RNG_SS")) == 0)
//				|| (str.CompareNoCase(_T("TLM_S")) == 0)
//				|| (str.CompareNoCase(_T("TLM_XQ")) == 0)
//				|| (str.CompareNoCase(_T("TLM_DX")) == 0)
//				|| (str.CompareNoCase(_T("CMD_S")) == 0))
//			{
//				time = dba.GetRdyChkTimeFile(str);
//			}
//		}
//	}
//	else if (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_SX_ID)
//	{
//		/* SX帯RNG S帯CMD S帯TLM */
//		if ((str.CompareNoCase(_T("AGC_DX")) == 0)
//			|| (str.CompareNoCase(_T("RNG_SX")) == 0)
//			|| (str.CompareNoCase(_T("TLM_S")) == 0)
//			|| (str.CompareNoCase(_T("CMD_S")) == 0))
//		{
//			time = dba.GetRdyChkTimeFile(str);
//		}
//	}
//	else if (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_RX_ID)
//	{
//		/* XX帯RNG X帯CMD X帯新系TLM */
//		if ((str.CompareNoCase(_T("AGC_DX")) == 0)
//			|| (str.CompareNoCase(_T("RNG_DX")) == 0)
//			|| (str.CompareNoCase(_T("RNG_DR")) == 0)
//			|| (str.CompareNoCase(_T("TLM_DX")) == 0)
//			|| (str.CompareNoCase(_T("CMD_DX")) == 0))
//		{
//			time = dba.GetRdyChkTimeFile(str);
//		}
//	}
#endif

	return time;
}

int COperationPlan::GetRdyTime64(CString str)
{
	int time = -1;
	CSatelliteData &sd = theApp.GetSatelliteData();
	CDBAccess &dba = theApp.GetSatelliteData().GetDBAccessCls();

#if 1
	time = dba.GetRdyChkTimeFile(str);
#else
//	if (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_S_ID)
//	{
//		/* SS帯 */
//		if ((str.CompareNoCase(_T("AGC_S")) == 0)
//			|| (str.CompareNoCase(_T("RNG_SS")) == 0)
//			|| (str.CompareNoCase(_T("TLM_S")) == 0)
//			|| (str.CompareNoCase(_T("CMD_S")) == 0))
//		{
//			time = dba.GetRdyChkTimeFile(str);
//		}
//	}
//	else if (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_SX_ID)
//	{
//		/* SX帯 */
//		if ((str.CompareNoCase(_T("AGC_S")) == 0)
//			|| (str.CompareNoCase(_T("AGC_X")) == 0)
//			|| (str.CompareNoCase(_T("RNG_SS")) == 0)
//			|| (str.CompareNoCase(_T("TLM_S")) == 0)
//			|| (str.CompareNoCase(_T("TLM_X")) == 0)
//			|| (str.CompareNoCase(_T("CMD_S")) == 0))
//		{
//			time = dba.GetRdyChkTimeFile(str);
//		}
//	}
//	else if (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_X_ID)
//	{
//		/* XX帯新系 */
//		if ((str.CompareNoCase(_T("AGC_DX")) == 0)
//			|| (str.CompareNoCase(_T("RNG_DX")) == 0)
//			|| (str.CompareNoCase(_T("TLM_DX")) == 0)
//			|| (str.CompareNoCase(_T("CMD_DX")) == 0))
//		{
//			time = dba.GetRdyChkTimeFile(str);
//		}
//	}
//	else if (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_RX_ID)
//	{
//		/* XX帯新系再生 */
//		if ((str.CompareNoCase(_T("AGC_DX")) == 0)
//			|| (str.CompareNoCase(_T("RNG_DX")) == 0)
//			|| (str.CompareNoCase(_T("RNG_DR")) == 0)
//			|| (str.CompareNoCase(_T("TLM_DX")) == 0)
//			|| (str.CompareNoCase(_T("CMD_DX")) == 0))
//		{
//			time = dba.GetRdyChkTimeFile(str);
//		}
//	}
#endif

	return time;
}

/*============================================================================*/
/*! COperationPlan

-# 計画雛形ファイル、または計画ファイルより画面を設定する。

@param  なし
@retval TRUE：正常終了/FALSE：異常終了
*/
/*============================================================================*/
BOOL COperationPlan::SetPlanDataDlg(int nStation, CString sz_passno, stPlandata& stpd)
{
	LVITEM lvi;
	stTime sttime;

	/*------------------------------------------------------------*/
	/* 運用オプション */

	// 初期設定
	if (strON[stpd.l_init] == strON[STRON_ON])
	{
		m_OpInitCmb.SelectStringEx(-1, strAriNasi[ARINASI_ARI]);
		m_bOpInit = TRUE;
		m_OprPlnDataOrg.m_OpInit = TRUE;
	}
	else
	{
		m_OpInitCmb.SelectStringEx(-1, strAriNasi[ARINASI_NASI]);
		m_bOpInit = FALSE;
		m_OprPlnDataOrg.m_OpInit = FALSE;
	}

	// 受信機ドップラ補償
	if(nStation == eStation_Usuda642)	// 臼田54m
	{
		m_OpRDPCmb.SelectStringEx(-1, strRxDoppler54[stpd.l_rx_ramp]);
	}
	else
	{
		m_OpRDPCmb.SelectStringEx(-1, strRxDoppler[stpd.l_rx_ramp]);
	}
	m_OprPlnDataOrg.m_OpRDP = stpd.l_rx_ramp;

#if 0
	int s = theApp.GetSelectStation();

	// 送信機ドップラ補償
	m_OpTDPCmb.SelectStringEx(-1, strTxDoppler[stpd.l_tx_ramp]);
	m_OprPlnDataOrg.m_OpTDP = stpd.l_tx_ramp;


	// 送信出力
	if (s == 0)	// 臼田64m
	{
		m_OpTOutCmb.SelectStringEx(-1, strTrsPwr64[stpd.l_tx_pwr_sel]);
	}
	else if (s == 1)	// 臼田54m
	{
		m_OpTOutCmb.SelectStringEx(-1, strTrsPwr64[stpd.l_tx_pwr_sel]);
	}
	else if (s == 2)	// 内之浦34m
	{
		if (m_OpTOutType == TRSPWRX_X)
		{
			m_OpTOutCmb.SelectStringEx(-1, strTrsPwrX34[stpd.l_tx_pwr_sel]);
		}
		else if (m_OpTOutType == TRSPWRX_S)
		{
			m_OpTOutCmb.SelectStringEx(-1, strTrsPwrS34[stpd.l_tx_pwr_sel]);
		}
	}
	else if (s == 3)	// 内之浦20m
	{
		m_OpTOutCmb.SelectStringEx(-1, strTrsPwr20[stpd.l_tx_pwr_sel]);
	}
	m_OprPlnDataOrg.m_OpTOut = stpd.l_tx_pwr_sel;
#endif

	/*------------------------------------------------------------*/
	/* レディネスチェック */

	// AGC
	if (stpd.l_agc_index == 1)
	{
		m_AGCChkValue = TRUE;
		m_OprPlnDataOrg.m_AgcChkValue = TRUE;
	}
	// RNG
	if (stpd.l_rng_index == 1)
	{
		m_RNGChkValue = TRUE;
		m_OprPlnDataOrg.m_RndChkValue = TRUE;
	}
	// TLM
	if (stpd.l_tlm_index == 1)
	{
		m_TLMChkValue = TRUE;
		m_OprPlnDataOrg.m_TlmChkValue = TRUE;
	}
	// CMD
	if (stpd.l_cmd_index == 1)
	{
		m_CMDChkValue = TRUE;
		m_OprPlnDataOrg.m_CmdChkValue = TRUE;
	}

	/*------------------------------------------------------------*/
	/* ビットレート */
	// CID
	m_CidBtnCtrl.SetWindowTextW(stpd.sz_cid);
	m_OprPlnDataOrg.m_strCID = stpd.sz_cid;
#if 1
	// CID よりビットレートを設定する
	map<CString, map<CString, stCIDParam>> &cidlist = theApp.GetSatelliteData().m_CIDSlctData.GetCidParamList();
	if (cidlist.size() > 0)
	{
		stCIDParam stCid = CCIDSlctData::TrimInvalidString(cidlist[m_SatelliteName][stpd.sz_cid]);

		// TLM S
		m_strBRTLMS = stCid.TlmBitRateS;

		// TLM X
		m_strBRTLMX = stCid.TlmBitRateX;

		// TLM Ka
		m_strBRTLMKa = stCid.TlmBitRateKa;

		// 高速TLM
		m_strBRHTLM = stCid.TlmBitRateHX;

		// CMD S
		m_strBRCMDS = stCid.CmdBitRateS;

		// CMD X
		m_strBRCMDX = stCid.CmdBitRateX;

		// 送信出力
		m_TrsPowerStr = stCid.TrsPower;

		// 送信ドップラ
		m_OpTDPStr = stCid.TransDoppler;

		// レンジ計測パラメータ
		m_strRngMesMode = stCid.RngMesMode;

		// ANT待ち受け指向
		m_AntStandbyStr = stCid.AntAosPosn;
		if (m_AntStandbyStr == strAriNasi[ARINASI_ARI])
		{
			m_bAntStandby = TRUE;
		}
		else
		{
			m_bAntStandby = FALSE;
		}

		// ANT天頂指向
#if 1
		if ((stCid.AntWaitPoint.CompareNoCase(_T("天頂")) == 0) || (stCid.AntWaitPoint.CompareNoCase(_T("天頂(LOCK)")) == 0))
		{
			m_AntZenithStr = strANTZenith[ANTZENITH_ARI_SWTOW];
			m_bAntZenith = TRUE;
		}
		else if (stCid.AntWaitPoint.CompareNoCase(_T("天頂(UNLOCK)")) == 0)
		{
			m_AntZenithStr = strANTZenith[ANTZENITH_ARI];
			m_bAntZenith = TRUE;
		}
		else
		{
			m_AntZenithStr = strANTZenith[ANTZENITH_NASI];
			m_bAntZenith = FALSE;
		}
#else
		m_AntZenithStr = stCid.AntWaitPoint;
#endif
	}
#else
//	// TLM S
//	m_strBRTLMS = stpd.sz_s_bitrate;
//
//	// TLM X
//	m_strBRTLMX = stpd.sz_x_bitrate;
//
//	// TLM Ka
//	m_strBRTLMKa = stpd.sz_ka_bitrate;
//
//	// 高速TLM
//	m_strBRHTLM = stpd.sz_x_qpsk;
//
//	// CMD S
//	m_strBRCMDS = stpd.sz_cmd_bit_rate;
//
//	// CMD X
//	m_strBRCMDX = stpd.sz_x_cmd_bit_rate;
#endif

#if 0
	/*------------------------------------------------------------*/
	/* ANT 設定 */

	// ANT待ち受け指向通知
	if (stpd.l_antwait_flg)
	{
		m_AntStandbyCmb.SelectStringEx(-1, strAriNasi[ARINASI_ARI]);
		m_bAntStandby = TRUE;
		m_OprPlnDataOrg.m_AntStandby = ARINASI_ARI;
	}
	else
	{
		m_AntStandbyCmb.SelectStringEx(-1, strAriNasi[ARINASI_NASI]);
		m_bAntStandby = FALSE;
		m_OprPlnDataOrg.m_AntStandby = ARINASI_NASI;
	}

	// ANT天頂指向有無
	if (s != 0 && s != 1)	// 臼田64m,臼田54m以外
	{
		m_AntZenithCmb.SelectStringEx(-1, strANTZenith[stpd.l_ant_stow]);
		if ((strANTZenith[stpd.l_ant_stow] == strANTZenith[ANTZENITH_ARI_SWTOW])
			|| (strANTZenith[stpd.l_ant_stow] == strANTZenith[ANTZENITH_ARI]))
		{
			m_bAntZenith = TRUE;
		}
		else
		{
			m_bAntZenith = FALSE;
		}
		m_OprPlnDataOrg.m_AntZenith = stpd.l_ant_stow;
	}
#endif

	/*------------------------------------------------------------*/
	/* 基本パラメータ */

	// 運用開始/終了日時
	if (stpd.sz_start != _T("") && stpd.sz_end != _T(""))
	{
		CTime sTime = CSatelliteData::StrTimeToCTime(stpd.sz_start);
		CTime eTime = CSatelliteData::StrTimeToCTime(stpd.sz_end);

		// 範囲解除
		m_OpeStartDayCtrl.SetRange(&CTime(0), &CTime(0));
		m_OpeEndDayCtrl.SetRange(&CTime(0), &CTime(0));

		m_OpeStartDay = sTime;
		m_OpeStartTime = sTime;
		m_OpeEndDay = eTime;
		m_OpeEndTime = eTime;

		m_tOpeS = sTime;
		m_tOpeE = eTime;

		m_OprPlnDataOrg.m_OpetDay.start = sTime;
		m_OprPlnDataOrg.m_OpetDay.end = eTime;

		// 範囲指定
		m_OpeStartDayCtrl.SetRange(&sTime, &eTime);
		m_OpeEndDayCtrl.SetRange(&sTime, &eTime);
	}

	int s_idx, e_idx;

	// アップリンク
	m_UpLinkList.DeleteAllItems();
	m_OprPlnDataOrg.m_ULDay.clear();
	s_idx = (int)stpd.sz_uplink_s_timelst.size();
	e_idx = (int)stpd.sz_uplink_e_timelst.size();
	if (s_idx != 0 && (s_idx == e_idx))
	{
		for (int i = 0; i < s_idx; ++i)
		{
			CString szSTime = stpd.sz_uplink_s_timelst[i];
			CString szETime = stpd.sz_uplink_e_timelst[i];

			// 開始日時
			lvi.lParam = (LPARAM)i;
			lvi.iItem = i;
			lvi.iSubItem = 0;
			lvi.mask = LVIF_TEXT;
			lvi.pszText = szSTime.GetBuffer();
			m_UpLinkList.InsertItem(&lvi);

			// 終了日時
			lvi.lParam = (LPARAM)i;
			lvi.iItem = i;
			lvi.iSubItem = 1;
			lvi.mask = LVIF_TEXT;
			lvi.pszText = szETime.GetBuffer();
			m_UpLinkList.SetItem(&lvi);

			sttime.start = CSatelliteData::StrTimeToCTime(szSTime);
			sttime.end = CSatelliteData::StrTimeToCTime(szETime);
			m_OprPlnDataOrg.m_ULDay.push_back(sttime);
		}
		m_UpLinkChkBox = TRUE;
		m_OprPlnDataOrg.m_UpLinkChk = TRUE;
	}
	else
	{
		m_UpLinkChkBox = FALSE;
	}

	// RNG
	m_RngList.DeleteAllItems();
	m_OprPlnDataOrg.m_RngDayList.clear();
	s_idx = (int)stpd.sz_rng_s_timelst.size();
	e_idx = (int)stpd.sz_rng_e_timelst.size();
	if (s_idx != 0 && (s_idx == e_idx))
	{
		for (int i = 0; i < s_idx; ++i)
		{
			CString szSTime = stpd.sz_rng_s_timelst[i];
			CString szETime = stpd.sz_rng_e_timelst[i];
			CString szKind = stpd.sz_rng_kindlst[i];

			// 開始日時
			lvi.lParam = (LPARAM)i;
			lvi.iItem = i;
			lvi.iSubItem = 0;
			lvi.mask = LVIF_TEXT;
			lvi.pszText = szSTime.GetBuffer();
			m_RngList.InsertItem(&lvi);

			// 終了日時
			lvi.lParam = (LPARAM)i;
			lvi.iItem = i;
			lvi.iSubItem = 1;
			lvi.mask = LVIF_TEXT;
			lvi.pszText = szETime.GetBuffer();
			m_RngList.SetItem(&lvi);

			// 種別
			if (szKind == _T("SS-RNG"))
			{
				szKind = _T("SS");
			}
			else if (szKind == _T("SX-RNG"))
			{
				szKind = _T("SX");
			}
			else if (szKind == _T("XX-RNG_NOR"))
			{
				szKind = _T("XX新");
				if(nStation == eStation_Usuda642)	// 臼田54m
				{
					szKind = _T("XX");
				}
			}
			else if (szKind == _T("XX-RNG_REG"))
			{
				szKind = _T("XX新再");
				if(nStation == eStation_Usuda642)	// 臼田54m
				{
					szKind = _T("XX再");
				}
			}
			else if (szKind == _T("XKa-RNG_NOR"))
			{
				szKind = _T("XKa新");
				if(nStation == eStation_Usuda642)	// 臼田54m
				{
					szKind = _T("XKa");
				}
			}
			else if (szKind == _T("XKa-RNG_REG"))
			{
				szKind = _T("XKa新再");
				if(nStation == eStation_Usuda642)	// 臼田54m
				{
					szKind = _T("XKa再");
				}
			}

			lvi.lParam = (LPARAM)i;
			lvi.iItem = i;
			lvi.iSubItem = 2;
			lvi.mask = LVIF_TEXT;
			lvi.pszText = szKind.GetBuffer();
			m_RngList.SetItem(&lvi);

			sttime.start = CSatelliteData::StrTimeToCTime(szSTime);
			sttime.end = CSatelliteData::StrTimeToCTime(szETime);
			m_OprPlnDataOrg.m_RngDayList.push_back(sttime);
		}
		m_RngChkBox = TRUE;
		m_OprPlnDataOrg.m_RngChk = TRUE;
	}
	else
	{
		m_RngChkBox = FALSE;
	}

	// CMD
	m_CmdList.DeleteAllItems();
	m_OprPlnDataOrg.m_CmdDayList.clear();
	s_idx = (int)stpd.sz_cmd_s_timelst.size();
	e_idx = (int)stpd.sz_cmd_e_timelst.size();
	if (s_idx != 0 && (s_idx == e_idx))
	{
		for (int i = 0; i < s_idx; ++i)
		{
			CString szSTime = stpd.sz_cmd_s_timelst[i];
			CString szETime = stpd.sz_cmd_e_timelst[i];

			// 開始日時
			lvi.lParam = (LPARAM)i;
			lvi.iItem = i;
			lvi.iSubItem = 0;
			lvi.mask = LVIF_TEXT;
			lvi.pszText = szSTime.GetBuffer();
			m_CmdList.InsertItem(&lvi);

			// 終了日時
			lvi.lParam = (LPARAM)i;
			lvi.iItem = i;
			lvi.iSubItem = 1;
			lvi.mask = LVIF_TEXT;
			lvi.pszText = szETime.GetBuffer();
			m_CmdList.SetItem(&lvi);

			sttime.start = CSatelliteData::StrTimeToCTime(szSTime);
			sttime.end = CSatelliteData::StrTimeToCTime(szETime);
			m_OprPlnDataOrg.m_CmdDayList.push_back(sttime);
		}
		m_CmdChkBox = TRUE;
		m_OprPlnDataOrg.m_CmdChk = TRUE;
	}
	else
	{
		m_CmdChkBox = FALSE;
	}

	// TLM S
	m_TlmSList.DeleteAllItems();
	m_OprPlnDataOrg.m_TlmSDayList.clear();
	s_idx = (int)stpd.sz_tlms_s_timelst.size();
	e_idx = (int)stpd.sz_tlms_e_timelst.size();
	if (s_idx != 0 && (s_idx == e_idx))
	{
		for (int i = 0; i < s_idx; ++i)
		{
			CString szSTime = stpd.sz_tlms_s_timelst[i];
			CString szETime = stpd.sz_tlms_e_timelst[i];

			// 開始日時
			lvi.lParam = (LPARAM)i;
			lvi.iItem = i;
			lvi.iSubItem = 0;
			lvi.mask = LVIF_TEXT;
			lvi.pszText = szSTime.GetBuffer();
			m_TlmSList.InsertItem(&lvi);

			// 終了日時
			lvi.lParam = (LPARAM)i;
			lvi.iItem = i;
			lvi.iSubItem = 1;
			lvi.mask = LVIF_TEXT;
			lvi.pszText = szETime.GetBuffer();
			m_TlmSList.SetItem(&lvi);

			sttime.start = CSatelliteData::StrTimeToCTime(szSTime);
			sttime.end = CSatelliteData::StrTimeToCTime(szETime);
			m_OprPlnDataOrg.m_TlmSDayList.push_back(sttime);
		}
		m_TLMSChkBox = TRUE;
		m_OprPlnDataOrg.m_TlmSChk = TRUE;
	}
	else
	{
		m_TLMSChkBox = FALSE;
	}

	// TLM X
	m_TlmXList.DeleteAllItems();
	m_OprPlnDataOrg.m_TlmXDayList.clear();
	s_idx = (int)stpd.sz_tlmx_s_timelst.size();
	e_idx = (int)stpd.sz_tlmx_e_timelst.size();
	if (s_idx != 0 && (s_idx == e_idx))
	{
		for (int i = 0; i < s_idx; ++i)
		{
			CString szSTime = stpd.sz_tlmx_s_timelst[i];
			CString szETime = stpd.sz_tlmx_e_timelst[i];

			// 開始日時
			lvi.lParam = (LPARAM)i;
			lvi.iItem = i;
			lvi.iSubItem = 0;
			lvi.mask = LVIF_TEXT;
			lvi.pszText = szSTime.GetBuffer();
			m_TlmXList.InsertItem(&lvi);

			// 終了日時
			lvi.lParam = (LPARAM)i;
			lvi.iItem = i;
			lvi.iSubItem = 1;
			lvi.mask = LVIF_TEXT;
			lvi.pszText = szETime.GetBuffer();
			m_TlmXList.SetItem(&lvi);

			sttime.start = CSatelliteData::StrTimeToCTime(szSTime);
			sttime.end = CSatelliteData::StrTimeToCTime(szETime);
			m_OprPlnDataOrg.m_TlmXDayList.push_back(sttime);
		}
		m_TLMXChkBox = TRUE;
		m_OprPlnDataOrg.m_TlmXChk = TRUE;
	}
	else
	{
		m_TLMXChkBox = FALSE;
	}

	// TLM Ka
	m_TlmKaList.DeleteAllItems();
	m_OprPlnDataOrg.m_TlmKaDayList.clear();
	s_idx = (int)stpd.sz_tlmka_s_timelst.size();
	e_idx = (int)stpd.sz_tlmka_e_timelst.size();
	if (s_idx != 0 && (s_idx == e_idx))
	{
		for (int i = 0; i < s_idx; ++i)
		{
			CString szSTime = stpd.sz_tlmka_s_timelst[i];
			CString szETime = stpd.sz_tlmka_e_timelst[i];

			// 開始日時
			lvi.lParam = (LPARAM)i;
			lvi.iItem = i;
			lvi.iSubItem = 0;
			lvi.mask = LVIF_TEXT;
			lvi.pszText = szSTime.GetBuffer();
			m_TlmKaList.InsertItem(&lvi);

			// 終了日時
			lvi.lParam = (LPARAM)i;
			lvi.iItem = i;
			lvi.iSubItem = 1;
			lvi.mask = LVIF_TEXT;
			lvi.pszText = szETime.GetBuffer();
			m_TlmKaList.SetItem(&lvi);

			sttime.start = CSatelliteData::StrTimeToCTime(szSTime);
			sttime.end = CSatelliteData::StrTimeToCTime(szETime);
			m_OprPlnDataOrg.m_TlmKaDayList.push_back(sttime);
		}
		m_TLMKaChkBox = TRUE;
		m_OprPlnDataOrg.m_TlmKaChk = TRUE;
	}
	else
	{
		m_TLMKaChkBox = FALSE;
	}

	UpdateData(FALSE);
	return TRUE;
}

/*============================================================================*/
/*! COperationPlan

-# データベースから情報を取得する

@param  なし
@retval 
*/
/*============================================================================*/
int COperationPlan::data_get_from_db20()
{

	struct satinf_t *satinf_adr;
	CSatelliteData& sd = theApp.GetSatelliteData();

	CDBAccess& dba = sd.GetDBAccessCls();

	vector<CString>			&saNameList = dba.GetSatNameList();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();

	int i_j;
	int i_m;

	struct satinf_t *original_adr;
	char *sattbl_adr;

	struct txpwr_inf_t *txpwr_a = NULL;
	struct sattx_t *v_sx_adr[5];
	int l_sx_cnt = 0;
	int l_sx_no = 0;
	struct txpwr_t *v_adr[20];
	long l_pwr_cnt;
	long l_pwr_no;
	struct satant_t *v_adr_sp[20];
	long l_sp_cnt;
	long l_sp_no;

	struct prd_t		*prd_t_adr;
	struct plan_t		*plan_t_adr;
	struct xtx_t		*xtx_t_adr;		/* Ｘ帯送信設備使用有無情報テーブルアドレス */
	struct xrxsel_t		*xrxsel_t_adr;	/* Ｘ帯受信設備対象情報テーブルアドレス */
	struct rampcnt_t	*rampcnt_t_adr;	/* 周波数ランプ制御許可情報テーブルアドレス */
	struct topangl_t	*topangl_t_adr;	/* 天頂通過監視角度情報テーブルアドレス */
	struct catch_t		*catch_t_adr;	/* 衛星捕捉情報テーブルアドレス */
	struct agcrx_t		*agcrx_t_adr;	/* ＡＧＣ校正対象受信機情報テーブルアドレス */
	struct rxfqval_t	*rxfqval_adr;
	struct rarrband_t	*rarr_t_adr;	/* ＲＡＲＲ使用帯域情報テーブルアドレス */

	char *cmd_br_vec[TBLMAX];
	char *br_s_vec[TBLMAX];
	char *br_x_vec[TBLMAX];
	char *br_qpsk_vec[TBLMAX];

	int cnt = (int)saNameList.size();
	for (i_j = 0; i_j < cnt; i_j++){
		/*--------------------------*/
		/* 衛星本体情報アドレス取得 */
		CString satname = saNameList[i_j];
		satinf_adr = (struct satinf_t *)((__int64*)satAdrList[satname]);

		original_adr = satinf_adr;			/* アドレスの退避                 */
		sattbl_adr = (char *)satinf_adr;	/* char型に変換                   */
		satinf_adr = original_adr;			/* 衛星本体情報先頭アドレスセット */

		/*--------------------*/
		/* 衛星本体情報の取得 */
		/*--------------------*/
		strcpy_s(sd.m_satinfo[i_j].sz_satnames, sizeof(sd.m_satinfo[i_j].sz_satnames), satinf_adr->sz_satname);
		sd.m_satinfo[i_j].l_sat_no = satinf_adr->l_satno;                 /* 衛星番号     */
		sd.m_satinfo[i_j].l_use_band = satinf_adr->l_useband;             /* 使用バンド帯 */
		sd.m_satinfo[i_j].l_level = satinf_adr->l_level;                  /* 使用帯域     */

		/*----------------------------------*/
		/* 使用バンド帯関連情報項目数の取得 */
		/*----------------------------------*/
		l_sx_cnt = dba.ud_adr_to_resvec((char*)satinf_adr, SATTX_ID, ENDOFSAT_ID, NULL, (char**)v_sx_adr, 5);
		for (l_sx_no = 0; l_sx_no < l_sx_cnt; l_sx_no++)
		{
			if (strcmp(v_sx_adr[l_sx_no]->sz_sattx, TXBAND_S_STR) == 0)
			{
				txpwr_a = sd.m_satinfo[i_j].s_txpwr_sval;
			}
			if (strcmp(v_sx_adr[l_sx_no]->sz_sattx, TXBAND_X_STR) == 0)
			{
				txpwr_a = sd.m_satinfo[i_j].x_txpwr_sval;
			}
			l_pwr_cnt = dba.ud_adr_to_resvec((char*)v_sx_adr[l_sx_no], TXPWR_ID, ENDOFSAT_ID, ENDOFSATTX_ID, (char**)v_adr, 20);
			for (l_pwr_no = 0; l_pwr_no < l_pwr_cnt; l_pwr_no++)
			{
				if (strcmp(v_sx_adr[l_sx_no]->sz_sattx, TXBAND_S_STR) == 0)
				{
					sd.m_satinfo[i_j].i_s_power++;      /* S-BAND送信パワー項目数 */
				}
				else
				{
					sd.m_satinfo[i_j].i_x_power++;      /* X-BAND送信パワー項目数 */
				}

				txpwr_a->sz_txpwr_val = &v_adr[l_pwr_no]->sz_txpwr[0];
				l_sp_cnt = dba.ud_adr_to_resvec((char*)v_adr[l_pwr_no], SATANT_ID, ENDOFSAT_ID, ENDOFTXPWR_ID, (char**)v_adr_sp, 20);
				for (l_sp_no = 0; l_sp_no < l_sp_cnt; l_sp_no++)
				{
					if (strcmp(v_sx_adr[l_sx_no]->sz_sattx, TXBAND_S_STR) == 0)
					{
						sd.m_satinfo[i_j].s_txpwr_sval[l_pwr_no].i_s_snd_level++;       /* S-BAND送信レベル項目数 */
					}
					else
					{
						sd.m_satinfo[i_j].x_txpwr_sval[l_pwr_no].i_x_snd_level++;       /* X-BAND送信レベル項目数 */
					}
					txpwr_a->satant_sval[l_sp_no] = &v_adr_sp[l_sp_no]->sz_satant[0];
				}
				txpwr_a->satant_sval[l_sp_no] = NULL;
				txpwr_a++;
			}
			txpwr_a->sz_txpwr_val = NULL;
		}

		i_m = 0;
		satinf_adr = original_adr;				/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;		/*char型に変換*/

		/*----------------------------------*/
		/* 予報値ファイルディレクトリの取得 */
		/*----------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == PRD_ID)
			{
				/*識別ＩＤがの場合*/
				prd_t_adr = (struct prd_t *)satinf_adr;
				strcpy_s(sd.m_satinfo[i_j].sz_pred_dir, sizeof(sd.m_satinfo[i_j].sz_pred_dir), prd_t_adr->sz_prdpath);
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}

		i_m = 0;
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*------------------------------*/
		/* 計画ファイルディレクトリ取得 */
		/*------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == PLAN_ID)
			{				/*識別ＩＤがの場合*/
				plan_t_adr = (struct plan_t *)satinf_adr;
				strcpy_s(sd.m_satinfo[i_j].sz_plan_dir, sizeof(sd.m_satinfo[i_j].sz_plan_dir), plan_t_adr->sz_planpath);
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*--------------------------------------*/
		/* Ｘ帯送信設備使用有無情報取得 */
		/*--------------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == XTX_ID)
			{
				/*識別ＩＤが「Ｘ帯送信設備使用有無情報」の場合*/
				xtx_t_adr = (struct xtx_t *)satinf_adr;
				sd.m_satinfo[i_j].i_x_tx_sel = xtx_t_adr->l_x_tx;
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*----------------------------------*/
		/* Ｘ帯受信設備対象情報取得 */
		/*----------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{ 
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == XRXSEL_ID)
			{
				/*識別ＩＤが「Ｘ帯受信設備対象情報」の場合*/
				xrxsel_t_adr = (struct xrxsel_t *)satinf_adr;
				sd.m_satinfo[i_j].i_x_rx_sel = xrxsel_t_adr->l_x_rx_sel;
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*--------------------------------------*/
		/* 周波数ランプ制御許可情報取得 */
		/*--------------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == RAMPCNT_ID)
			{
				/*識別ＩＤが「周波数ランプ制御許可情報」の場合*/
				rampcnt_t_adr = (struct rampcnt_t *)satinf_adr;
				sd.m_satinfo[i_j].i_s_tx_ramp = rampcnt_t_adr->l_s_tx;			/* Ｓ帯送信機ランプ制御有無 */
				sd.m_satinfo[i_j].i_x_tx_ramp = rampcnt_t_adr->l_x_tx;			/* Ｘ帯送信機ランプ制御有無 */
				sd.m_satinfo[i_j].i_s_rx_ramp = rampcnt_t_adr->l_s_rx;			/* Ｓ帯受信設備ランプ制御有無 */
				sd.m_satinfo[i_j].i_x_rx_ramp = rampcnt_t_adr->l_x_rx;			/* Ｘ帯受信設備ランプ制御有無（鹿児島専用） */
				sd.m_satinfo[i_j].i_x_rx1_ramp = rampcnt_t_adr->l_x_rx1;		/* Ｘ帯受信設備１ランプ制御有無（臼田専用） */
				sd.m_satinfo[i_j].i_x_rx2_ramp = rampcnt_t_adr->l_x_rx2;		/* Ｘ帯受信設備２ランプ制御有無（臼田専用） */
				sd.m_satinfo[i_j].i_sx_acqrx_ramp = rampcnt_t_adr->l_sx_acqrx;	/* ＳＸ帯捕捉受信設備ランプ制御有無（鹿児島専用） */
				sd.m_satinfo[i_j].i_x_qpskrx_ramp = rampcnt_t_adr->l_x_qpskrx;	/* Ｘ帯高速受信設備ランプ制御有無 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*----------------------------------*/
		/* 天頂通過監視角度情報取得 */
		/*----------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == TOPANGL_ID)
			{
				/*識別ＩＤが「天頂通過監視角度情報」の場合*/
				topangl_t_adr = (struct topangl_t *)satinf_adr;
				sd.m_satinfo[i_j].i_ent_angle = topangl_t_adr->l_in;		/* 天頂通過監視入角 */
				sd.m_satinfo[i_j].i_ext_angle = topangl_t_adr->l_out;		/* 天頂通過監視出角 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*--------------------------*/
		/* <<XUP>> 衛星捕捉情報取得 */
		/*--------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == CATCH_ID)
			{
				/*識別ＩＤが「衛星捕捉情報」の場合*/
				catch_t_adr = (struct catch_t *)satinf_adr;
				sd.m_satinfo[i_j].i_acq_route = catch_t_adr->l_route;	/* 捕捉ルート番号 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;            /*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;    /*char型に変換*/

		/*--------------------------------------*/
		/* ＡＧＣ校正対象受信機情報取得 */
		/*--------------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{          /*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == AGCRX_ID)
			{
				/*識別ＩＤが「ＡＧＣ校正対象受信機情報」の場合*/
				agcrx_t_adr = (struct agcrx_t *)satinf_adr;
				sd.m_satinfo[i_j].i_s_rx = agcrx_t_adr->l_s_rx;				/* Ｓ帯主受信設備 */
				sd.m_satinfo[i_j].i_x_rx = agcrx_t_adr->l_x_rx;				/* Ｘ帯主受信設備 */
				sd.m_satinfo[i_j].i_x_rx1 = agcrx_t_adr->l_x_rx1;			/* Ｘ帯主受信設備１ */
				sd.m_satinfo[i_j].i_x_rx2 = agcrx_t_adr->l_x_rx2;			/* Ｘ帯主受信設備２ */
				sd.m_satinfo[i_j].i_sx_acqrx_s = agcrx_t_adr->l_sx_acqrx_s;	/* ＳＸ帯補足用受信設備　Ｓ帯 */
				sd.m_satinfo[i_j].i_sx_acqrx_x = agcrx_t_adr->l_sx_acqrx_x;	/* ＳＸ帯補足用受信設備　Ｘ帯 */
				sd.m_satinfo[i_j].i_x_qpskrx = agcrx_t_adr->l_x_qpskrx;		/* Ｘ帯高速データ用受信設備 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;            /*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;    /*char型に変換*/


		/*----------------------------------*/
		/* ノミナル受信周波数情報取得 */
		/*----------------------------------*/
		sd.m_satinfo[i_j].i_rxfq_ka = 0;
		sd.m_satinfo[i_j].i_rxfq_x = 0;
		sd.m_satinfo[i_j].i_rxfq_s = 0;
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFRXFQ_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == RXFQVAL_ID)
			{
				/*識別ＩＤがの場合*/
				rxfqval_adr = (struct rxfqval_t *)satinf_adr;
				if (rxfqval_adr->l_band == RXBAND_S_ID)
				{
					if (rxfqval_adr->d_fq != 0)
					{
						// S 帯有り
						sd.m_satinfo[i_j].i_rxfq_s = 1;
					}
				}
				if (rxfqval_adr->l_band == RXBAND_X_ID)
				{
					if (rxfqval_adr->d_fq != 0)
					{
						// X 帯有り
						sd.m_satinfo[i_j].i_rxfq_x = 1;
					}
				}
				if (rxfqval_adr->l_band == RXBAND_KA_ID)
				{
					if (rxfqval_adr->d_fq != 0)
					{
						// Ka 帯有り
						sd.m_satinfo[i_j].i_rxfq_ka = 1;
					}
				}
			}

			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;				/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;		/*char型に変換*/

		/*----------------------------------*/


		/*-----------------------*/
		/* S/X-CMD BITRATEの取得 */
		/*-----------------------*/
		if ((sd.m_satinfo[i_j].i_cmd_bitrate = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(C_BIT).GetBuffer(), sd.m_satinfo[i_j].cmd_tbl, cmd_br_vec, TBLMAX)) == -1)
		{
			sd.m_satinfo[i_j].i_cmd_bitrate = 0;
		}

		/*-----------------*/
		/* S BITRATEの取得 */
		/*-----------------*/
		if ((sd.m_satinfo[i_j].i_bitrate_s = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(BIT_S).GetBuffer(), sd.m_satinfo[i_j].br_s_tbl, br_s_vec, TBLMAX)) == -1)
		{
			sd.m_satinfo[i_j].i_bitrate_s = 0;
		}

		/*-----------------*/
		/* X BITRATEの取得 */
		/*-----------------*/
		if ((sd.m_satinfo[i_j].i_bitrate_x = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(BIT_X).GetBuffer(), sd.m_satinfo[i_j].br_x_tbl, br_x_vec, TBLMAX)) == -1)
		{
			sd.m_satinfo[i_j].i_bitrate_x = 0;
		}

		/*----------------------*/
		/* QPSK X BITRATEの取得 */
		/*----------------------*/
		if ((sd.m_satinfo[i_j].i_bitrate_qpsk = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(BIT_XQPSK20).GetBuffer(), sd.m_satinfo[i_j].br_qpsk_tbl, br_qpsk_vec, TBLMAX)) == -1)
		{
			sd.m_satinfo[i_j].i_bitrate_qpsk = 0;
		}

		/*--------------------------*/
		/* ＲＡＲＲ使用帯域情報取得 */
		/*--------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == RARRBAND_ID)
			{
				/*識別ＩＤが「ＲＡＲＲ使用帯域情報」の場合*/
				rarr_t_adr = (struct rarrband_t *)satinf_adr;
				sd.m_satinfo[i_j].i_rarr_band = rarr_t_adr->l_rarr_band; /* ＲＡＲＲ使用帯域番号 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/
	}

	return 0;
}

int COperationPlan::data_get_from_db34()
{

	struct satinf_t *satinf_adr;
	CSatelliteData& sd = theApp.GetSatelliteData();

	CDBAccess& dba = sd.GetDBAccessCls();

	vector<CString>			&saNameList = dba.GetSatNameList();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();

	int i_j;
	int i_m;

	struct satinf_t *original_adr;
	char *sattbl_adr;

	struct txpwr_inf_t *txpwr_a = NULL;
	struct sattx_t *v_sx_adr[5];
	int l_sx_cnt = 0;
	int l_sx_no = 0;
	struct txpwr_t *v_adr[20];
	long l_pwr_cnt;
	long l_pwr_no;
	struct satant_t *v_adr_sp[20];
	long l_sp_cnt;
	long l_sp_no;

	struct prd_t		*prd_t_adr;
	struct plan_t		*plan_t_adr;
	struct xtx_t		*xtx_t_adr;		/* Ｘ帯送信設備使用有無情報テーブルアドレス */
	struct xrxsel_t		*xrxsel_t_adr;	/* Ｘ帯受信設備対象情報テーブルアドレス */
	struct rampcnt_t	*rampcnt_t_adr;	/* 周波数ランプ制御許可情報テーブルアドレス */
	struct topangl_t	*topangl_t_adr;	/* 天頂通過監視角度情報テーブルアドレス */
	struct catch_t		*catch_t_adr;	/* 衛星捕捉情報テーブルアドレス */
	struct agcrx_t		*agcrx_t_adr;	/* ＡＧＣ校正対象受信機情報テーブルアドレス */
	struct rarrband_t	*rarr_t_adr;	/* ＲＡＲＲ使用帯域情報テーブルアドレス */
	struct rxfqval_t	*rxfqval_adr;

	char *cmd_br_vec[TBLMAX];
	char *br_s_vec[TBLMAX];
	char *br_x_vec[TBLMAX];
	char *br_qpsk_vec[TBLMAX];

	int cnt = (int)saNameList.size();
	for (i_j = 0; i_j < cnt; i_j++){
		/*--------------------------*/
		/* 衛星本体情報アドレス取得 */
		CString satname = saNameList[i_j];
		satinf_adr = (struct satinf_t *)((__int64*)satAdrList[satname]);

		original_adr = satinf_adr;			/* アドレスの退避                 */
		sattbl_adr = (char *)satinf_adr;	/* char型に変換                   */
		satinf_adr = original_adr;			/* 衛星本体情報先頭アドレスセット */

		/*--------------------*/
		/* 衛星本体情報の取得 */
		/*--------------------*/
		strcpy_s(sd.m_satinfo[i_j].sz_satnames, sizeof(sd.m_satinfo[i_j].sz_satnames), satinf_adr->sz_satname);
		sd.m_satinfo[i_j].l_sat_no = satinf_adr->l_satno;                 /* 衛星番号     */
		sd.m_satinfo[i_j].l_use_band = satinf_adr->l_useband;             /* 使用バンド帯 */
		sd.m_satinfo[i_j].l_level = satinf_adr->l_level;                  /* 使用帯域     */

		/*----------------------------------*/
		/* 使用バンド帯関連情報項目数の取得 */
		/*----------------------------------*/
		l_sx_cnt = dba.ud_adr_to_resvec((char*)satinf_adr, SATTX_ID, ENDOFSAT_ID, NULL, (char**)v_sx_adr, 5);
		for (l_sx_no = 0; l_sx_no < l_sx_cnt; l_sx_no++)
		{
			if (strcmp(v_sx_adr[l_sx_no]->sz_sattx, TXBAND_S_STR) == 0)
			{
				txpwr_a = sd.m_satinfo[i_j].s_txpwr_sval;
			}
			if (strcmp(v_sx_adr[l_sx_no]->sz_sattx, TXBAND_X_STR) == 0)
			{
				txpwr_a = sd.m_satinfo[i_j].x_txpwr_sval;
			}
			l_pwr_cnt = dba.ud_adr_to_resvec((char*)v_sx_adr[l_sx_no], TXPWR_ID, ENDOFSAT_ID, ENDOFSATTX_ID, (char**)v_adr, 20);
			for (l_pwr_no = 0; l_pwr_no < l_pwr_cnt; l_pwr_no++)
			{
				if (strcmp(v_sx_adr[l_sx_no]->sz_sattx, TXBAND_S_STR) == 0)
				{
					sd.m_satinfo[i_j].i_s_power++;      /* S-BAND送信パワー項目数 */
				}
				else
				{
					sd.m_satinfo[i_j].i_x_power++;      /* X-BAND送信パワー項目数 */
				}

				txpwr_a->sz_txpwr_val = &v_adr[l_pwr_no]->sz_txpwr[0];
				l_sp_cnt = dba.ud_adr_to_resvec((char*)v_adr[l_pwr_no], SATANT_ID, ENDOFSAT_ID, ENDOFTXPWR_ID, (char**)v_adr_sp, 20);
				for (l_sp_no = 0; l_sp_no < l_sp_cnt; l_sp_no++)
				{
					if (strcmp(v_sx_adr[l_sx_no]->sz_sattx, TXBAND_S_STR) == 0)
					{
						sd.m_satinfo[i_j].s_txpwr_sval[l_pwr_no].i_s_snd_level++;       /* S-BAND送信レベル項目数 */
					}
					else
					{
						sd.m_satinfo[i_j].x_txpwr_sval[l_pwr_no].i_x_snd_level++;       /* X-BAND送信レベル項目数 */
					}
					txpwr_a->satant_sval[l_sp_no] = &v_adr_sp[l_sp_no]->sz_satant[0];
				}
				txpwr_a->satant_sval[l_sp_no] = NULL;
				txpwr_a++;
			}
			txpwr_a->sz_txpwr_val = NULL;
		}

		i_m = 0;
		satinf_adr = original_adr;				/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;		/*char型に変換*/

		/*----------------------------------*/
		/* 予報値ファイルディレクトリの取得 */
		/*----------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == PRD_ID)
			{
				/*識別ＩＤがの場合*/
				prd_t_adr = (struct prd_t *)satinf_adr;
				strcpy_s(sd.m_satinfo[i_j].sz_pred_dir, sizeof(sd.m_satinfo[i_j].sz_pred_dir), prd_t_adr->sz_prdpath);
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}

		i_m = 0;
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*------------------------------*/
		/* 計画ファイルディレクトリ取得 */
		/*------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == PLAN_ID)
			{				/*識別ＩＤがの場合*/
				plan_t_adr = (struct plan_t *)satinf_adr;
				strcpy_s(sd.m_satinfo[i_j].sz_plan_dir, sizeof(sd.m_satinfo[i_j].sz_plan_dir), plan_t_adr->sz_planpath);
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*--------------------------------------*/
		/* Ｘ帯送信設備使用有無情報取得 */
		/*--------------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == XTX_ID)
			{
				/*識別ＩＤが「Ｘ帯送信設備使用有無情報」の場合*/
				xtx_t_adr = (struct xtx_t *)satinf_adr;
				sd.m_satinfo[i_j].i_x_tx_sel = xtx_t_adr->l_x_tx;
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*----------------------------------*/
		/* Ｘ帯受信設備対象情報取得 */
		/*----------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == XRXSEL_ID)
			{
				/*識別ＩＤが「Ｘ帯受信設備対象情報」の場合*/
				xrxsel_t_adr = (struct xrxsel_t *)satinf_adr;
				sd.m_satinfo[i_j].i_x_rx_sel = xrxsel_t_adr->l_x_rx_sel;
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*--------------------------------------*/
		/* 周波数ランプ制御許可情報取得 */
		/*--------------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == RAMPCNT_ID)
			{
				/*識別ＩＤが「周波数ランプ制御許可情報」の場合*/
				rampcnt_t_adr = (struct rampcnt_t *)satinf_adr;
				sd.m_satinfo[i_j].i_s_tx_ramp = rampcnt_t_adr->l_s_tx;			/* Ｓ帯送信機ランプ制御有無 */
				sd.m_satinfo[i_j].i_x_tx_ramp = rampcnt_t_adr->l_x_tx;			/* Ｘ帯送信機ランプ制御有無 */
				sd.m_satinfo[i_j].i_s_rx_ramp = rampcnt_t_adr->l_s_rx;			/* Ｓ帯受信設備ランプ制御有無 */
				sd.m_satinfo[i_j].i_x_rx_ramp = rampcnt_t_adr->l_x_rx;			/* Ｘ帯受信設備ランプ制御有無（鹿児島専用） */
				sd.m_satinfo[i_j].i_x_rx1_ramp = rampcnt_t_adr->l_x_rx1;		/* Ｘ帯受信設備１ランプ制御有無（臼田専用） */
				sd.m_satinfo[i_j].i_x_rx2_ramp = rampcnt_t_adr->l_x_rx2;		/* Ｘ帯受信設備２ランプ制御有無（臼田専用） */
				sd.m_satinfo[i_j].i_sx_acqrx_ramp = rampcnt_t_adr->l_sx_acqrx;	/* ＳＸ帯捕捉受信設備ランプ制御有無（鹿児島専用） */
				sd.m_satinfo[i_j].i_x_qpskrx_ramp = rampcnt_t_adr->l_x_qpskrx;	/* Ｘ帯高速受信設備ランプ制御有無 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*----------------------------------*/
		/* 天頂通過監視角度情報取得 */
		/*----------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == TOPANGL_ID)
			{
				/*識別ＩＤが「天頂通過監視角度情報」の場合*/
				topangl_t_adr = (struct topangl_t *)satinf_adr;
				sd.m_satinfo[i_j].i_ent_angle = topangl_t_adr->l_in;		/* 天頂通過監視入角 */
				sd.m_satinfo[i_j].i_ext_angle = topangl_t_adr->l_out;		/* 天頂通過監視出角 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*--------------------------*/
		/* <<XUP>> 衛星捕捉情報取得 */
		/*--------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == CATCH_ID)
			{
				/*識別ＩＤが「衛星捕捉情報」の場合*/
				catch_t_adr = (struct catch_t *)satinf_adr;
				sd.m_satinfo[i_j].i_acq_route = catch_t_adr->l_route;	/* 捕捉ルート番号 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;            /*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;    /*char型に変換*/

		/*--------------------------------------*/
		/* ＡＧＣ校正対象受信機情報取得 */
		/*--------------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{          /*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == AGCRX_ID)
			{
				/*識別ＩＤが「ＡＧＣ校正対象受信機情報」の場合*/
				agcrx_t_adr = (struct agcrx_t *)satinf_adr;
				sd.m_satinfo[i_j].i_s_rx = agcrx_t_adr->l_s_rx;				/* Ｓ帯主受信設備 */
				sd.m_satinfo[i_j].i_x_rx = agcrx_t_adr->l_x_rx;				/* Ｘ帯主受信設備 */
				sd.m_satinfo[i_j].i_x_rx1 = agcrx_t_adr->l_x_rx1;			/* Ｘ帯主受信設備１ */
				sd.m_satinfo[i_j].i_x_rx2 = agcrx_t_adr->l_x_rx2;			/* Ｘ帯主受信設備２ */
				sd.m_satinfo[i_j].i_sx_acqrx_s = agcrx_t_adr->l_sx_acqrx_s;	/* ＳＸ帯補足用受信設備　Ｓ帯 */
				sd.m_satinfo[i_j].i_sx_acqrx_x = agcrx_t_adr->l_sx_acqrx_x;	/* ＳＸ帯補足用受信設備　Ｘ帯 */
				sd.m_satinfo[i_j].i_x_qpskrx = agcrx_t_adr->l_x_qpskrx;		/* Ｘ帯高速データ用受信設備 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;            /*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;    /*char型に変換*/

		/*----------------------------------*/
		/* ノミナル受信周波数情報取得 */
		/*----------------------------------*/
		sd.m_satinfo[i_j].i_rxfq_ka = 0;
		sd.m_satinfo[i_j].i_rxfq_x = 0;
		sd.m_satinfo[i_j].i_rxfq_s = 0;
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFRXFQ_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == RXFQVAL_ID)
			{
				/*識別ＩＤがの場合*/
				rxfqval_adr = (struct rxfqval_t *)satinf_adr;
				if (rxfqval_adr->l_band == RXBAND_S_ID)
				{
					if (rxfqval_adr->d_fq != 0)
					{
						// S 帯有り
						sd.m_satinfo[i_j].i_rxfq_s = 1;
					}
				}
				if (rxfqval_adr->l_band == RXBAND_X_ID)
				{
					if (rxfqval_adr->d_fq != 0)
					{
						// X 帯有り
						sd.m_satinfo[i_j].i_rxfq_x = 1;
					}
				}
				if (rxfqval_adr->l_band == RXBAND_KA_ID)
				{
					if (rxfqval_adr->d_fq != 0)
					{
						// Ka 帯有り
						sd.m_satinfo[i_j].i_rxfq_ka = 1;
					}
				}
			}

			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;				/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;		/*char型に変換*/

		/*-----------------------*/
		/* S/X-CMD BITRATEの取得 */
		/*-----------------------*/
		if ((sd.m_satinfo[i_j].i_cmd_bitrate = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(C_BIT).GetBuffer(), sd.m_satinfo[i_j].cmd_tbl, cmd_br_vec, TBLMAX)) == -1)
		{
			sd.m_satinfo[i_j].i_cmd_bitrate = 0;
		}

		/*---------------------*/
		/* X-CMD BITRATEの取得 */
		/*---------------------*/
		if ((sd.m_satinfo[i_j].i_x_cmd_bitrate = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(C_BIT_X).GetBuffer(), sd.m_satinfo[i_j].x_cmd_tbl, cmd_br_vec, TBLMAX)) == -1)
		{
			sd.m_satinfo[i_j].i_x_cmd_bitrate = 0;
		}

		/*-----------------*/
		/* S BITRATEの取得 */
		/*-----------------*/
		if ((sd.m_satinfo[i_j].i_bitrate_s = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(BIT_S).GetBuffer(), sd.m_satinfo[i_j].br_s_tbl, br_s_vec, TBLMAX)) == -1)
		{
			sd.m_satinfo[i_j].i_bitrate_s = 0;
		}

		/*-----------------*/
		/* X BITRATEの取得 */
		/*-----------------*/
		if ((sd.m_satinfo[i_j].i_bitrate_x2 = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(BIT_X2).GetBuffer(), sd.m_satinfo[i_j].br_x_tbl, br_x_vec, TBLMAX)) == -1)
		{
			sd.m_satinfo[i_j].i_bitrate_x2 = 0;
		}

		/*----------------------*/
		/* QPSK X BITRATEの取得 */
		/*----------------------*/
		if ((sd.m_satinfo[i_j].i_bitrate_qpsk = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(BIT_XHRX).GetBuffer(), sd.m_satinfo[i_j].br_qpsk_tbl, br_qpsk_vec, TBLMAX)) == -1)
		{
			sd.m_satinfo[i_j].i_bitrate_qpsk = 0;
		}

		/*--------------------------*/
		/* ＲＡＲＲ使用帯域情報取得 */
		/*--------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == RARRBAND_ID)
			{
				/*識別ＩＤが「ＲＡＲＲ使用帯域情報」の場合*/
				rarr_t_adr = (struct rarrband_t *)satinf_adr;
				sd.m_satinfo[i_j].i_rarr_band = rarr_t_adr->l_rarr_band; /* ＲＡＲＲ使用帯域番号 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/
	}

	return 0;
}

int COperationPlan::data_get_from_db64()
{

	struct satinf_t *satinf_adr;
	CSatelliteData& sd = theApp.GetSatelliteData();

	CDBAccess& dba = sd.GetDBAccessCls();

	vector<CString>			&saNameList = dba.GetSatNameList();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();

	int	i_i;
	int i_j;
	int i_m;

	struct satinf_t *original_adr;
	char *sattbl_adr;

	struct txpwr_inf_t *txpwr_a = NULL;
	struct sattx_t *v_sx_adr[5];
	int l_sx_cnt = 0;
	int l_sx_no = 0;
	struct txpwr_t *v_adr[20];
	long l_pwr_cnt;
	long l_pwr_no;
	struct satant_t *v_adr_sp[20];
	long l_sp_cnt;
	long l_sp_no;

	struct prd_t		*prd_t_adr;
	struct plan_t		*plan_t_adr = NULL;
	struct xtx_t		*xtx_t_adr;		/* Ｘ帯送信設備使用有無情報テーブルアドレス */
	struct xrxsel_t		*xrxsel_t_adr;	/* Ｘ帯受信設備対象情報テーブルアドレス */
	struct rampcnt_t	*rampcnt_t_adr;	/* 周波数ランプ制御許可情報テーブルアドレス */
	struct topangl_t	*topangl_t_adr;	/* 天頂通過監視角度情報テーブルアドレス */
	struct catch_t		*catch_t_adr;	/* 衛星捕捉情報テーブルアドレス */
	struct agcrx_t		*agcrx_t_adr;	/* ＡＧＣ校正対象受信機情報テーブルアドレス */
	struct rarrband_t	*rarr_t_adr;	/* ＲＡＲＲ使用帯域情報テーブルアドレス */
	struct rxfqval_t	*rxfqval_adr;	

	char *cmd_br_vec[TBLMAX];
	char *br_s_vec[TBLMAX];
	char *br_x_vec[TBLMAX * 2];
//	char *br_qpsk_vec[TBLMAX];

	int cnt = (int)saNameList.size();
	for (i_j = 0, i_i = 0; i_j < cnt; i_j++)
	{
		/*--------------------------*/
		/* 衛星本体情報アドレス取得 */
		CString satname = saNameList[i_j];
		satinf_adr = (struct satinf_t *)((__int64*)satAdrList[satname]);

		original_adr = satinf_adr;			/* アドレスの退避                 */
		sattbl_adr = (char *)satinf_adr;	/* char型に変換                   */
		satinf_adr = original_adr;			/* 衛星本体情報先頭アドレスセット */

		/*--------------------*/
		/* 衛星本体情報の取得 */
		/*--------------------*/
		strcpy_s(sd.m_satinfo[i_i].sz_satnames, sizeof(sd.m_satinfo[i_i].sz_satnames), satinf_adr->sz_satname);
		sd.m_satinfo[i_i].l_sat_no = satinf_adr->l_satno;                 /* 衛星番号     */
		sd.m_satinfo[i_i].l_use_band = satinf_adr->l_useband;             /* 使用バンド帯 */
		sd.m_satinfo[i_i].l_level = satinf_adr->l_level;                  /* 使用帯域     */

		/*----------------------------------*/
		/* 使用バンド帯関連情報項目数の取得 */
		/*----------------------------------*/
		l_sx_cnt = dba.ud_adr_to_resvec((char*)satinf_adr, SATTX_ID, ENDOFSAT_ID, NULL, (char**)v_sx_adr, 5);
		for (l_sx_no = 0; l_sx_no < l_sx_cnt; l_sx_no++)
		{
			if (strcmp(v_sx_adr[l_sx_no]->sz_sattx, TXBAND_S_STR) == 0)
			{
				txpwr_a = sd.m_satinfo[i_i].s_txpwr_sval;
			}
			if (strcmp(v_sx_adr[l_sx_no]->sz_sattx, TXBAND_X_STR) == 0)
			{
				txpwr_a = sd.m_satinfo[i_i].x_txpwr_sval;
			}
			l_pwr_cnt = dba.ud_adr_to_resvec((char*)v_sx_adr[l_sx_no], TXPWR_ID, ENDOFSAT_ID, ENDOFSATTX_ID, (char**)v_adr, 20);
			for (l_pwr_no = 0; l_pwr_no < l_pwr_cnt; l_pwr_no++)
			{
				if (strcmp(v_sx_adr[l_sx_no]->sz_sattx, TXBAND_S_STR) == 0)
				{
					sd.m_satinfo[i_i].i_s_power++;      /* S-BAND送信パワー項目数 */
				}
				else
				{
					sd.m_satinfo[i_i].i_x_power++;      /* X-BAND送信パワー項目数 */
				}

				txpwr_a->sz_txpwr_val = &v_adr[l_pwr_no]->sz_txpwr[0];
				l_sp_cnt = dba.ud_adr_to_resvec((char*)v_adr[l_pwr_no], SATANT_ID, ENDOFSAT_ID, ENDOFTXPWR_ID, (char**)v_adr_sp, 20);
				for (l_sp_no = 0; l_sp_no < l_sp_cnt; l_sp_no++)
				{
					if (strcmp(v_sx_adr[l_sx_no]->sz_sattx, TXBAND_S_STR) == 0)
					{
						sd.m_satinfo[i_i].s_txpwr_sval[l_pwr_no].i_s_snd_level++;       /* S-BAND送信レベル項目数 */
					}
					else
					{
						sd.m_satinfo[i_i].x_txpwr_sval[l_pwr_no].i_x_snd_level++;       /* X-BAND送信レベル項目数 */
					}
					txpwr_a->satant_sval[l_sp_no] = &v_adr_sp[l_sp_no]->sz_satant[0];
				}
				txpwr_a->satant_sval[l_sp_no] = NULL;
				txpwr_a++;
			}
			txpwr_a->sz_txpwr_val = NULL;
		}

#if 0
		struct mac_t *mac_t_adr = NULL;
		i_m = 0;
		satinf_adr = original_adr;				/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;		/*char型に変換*/

		vector<CString> initList;
		/*----------------------------------*/
		/* INIT. */
		/*----------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == MAC_ID)
			{
				/*識別ＩＤがの場合*/
				mac_t_adr = (struct mac_t *)satinf_adr;

				CString mac = CString(mac_t_adr->sz_macname);
				if (mac.Find(_T("INIT.")) == 0)
				{
					initList.push_back(mac);
				}
			}
			sattbl_adr += satinf_adr->l_len;
		}
#endif
		i_m = 0;
		satinf_adr = original_adr;				/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;		/*char型に変換*/

		/*----------------------------------*/
		/* 予報値ファイルディレクトリの取得 */
		/*----------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == PRD_ID)
			{
				/*識別ＩＤがの場合*/
				prd_t_adr = (struct prd_t *)satinf_adr;
				strcpy_s(sd.m_satinfo[i_i].sz_pred_dir, sizeof(sd.m_satinfo[i_i].sz_pred_dir), prd_t_adr->sz_prdpath);
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}

		i_m = 0;
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*------------------------------*/
		/* 計画ファイルディレクトリ取得 */
		/*------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == PLAN_ID)
			{				/*識別ＩＤがの場合*/
				plan_t_adr = (struct plan_t *)satinf_adr;
				strcpy_s(sd.m_satinfo[i_i].sz_plan_dir, sizeof(sd.m_satinfo[i_i].sz_plan_dir), plan_t_adr->sz_planpath);
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*--------------------------------------*/
		/* Ｘ帯送信設備使用有無情報取得 */
		/*--------------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == XTX_ID)
			{
				/*識別ＩＤが「Ｘ帯送信設備使用有無情報」の場合*/
				xtx_t_adr = (struct xtx_t *)satinf_adr;
				sd.m_satinfo[i_i].i_x_tx_sel = xtx_t_adr->l_x_tx;
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*----------------------------------*/
		/* Ｘ帯受信設備対象情報取得 */
		/*----------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == XRXSEL_ID)
			{
				/*識別ＩＤが「Ｘ帯受信設備対象情報」の場合*/
				xrxsel_t_adr = (struct xrxsel_t *)satinf_adr;
				sd.m_satinfo[i_i].i_x_rx_sel = xrxsel_t_adr->l_x_rx_sel;
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*--------------------------------------*/
		/* 周波数ランプ制御許可情報取得 */
		/*--------------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == RAMPCNT_ID)
			{
				/*識別ＩＤが「周波数ランプ制御許可情報」の場合*/
				rampcnt_t_adr = (struct rampcnt_t *)satinf_adr;
				sd.m_satinfo[i_i].i_s_tx_ramp = rampcnt_t_adr->l_s_tx;			/* Ｓ帯送信機ランプ制御有無 */
				sd.m_satinfo[i_i].i_x_tx_ramp = rampcnt_t_adr->l_x_tx;			/* Ｘ帯送信機ランプ制御有無 */
				sd.m_satinfo[i_i].i_s_rx_ramp = rampcnt_t_adr->l_s_rx;			/* Ｓ帯受信設備ランプ制御有無 */
				sd.m_satinfo[i_i].i_x_rx_ramp = rampcnt_t_adr->l_x_rx;			/* Ｘ帯受信設備ランプ制御有無（鹿児島専用） */
				sd.m_satinfo[i_i].i_x_rx1_ramp = rampcnt_t_adr->l_x_rx1;		/* Ｘ帯受信設備１ランプ制御有無（臼田専用） */
				sd.m_satinfo[i_i].i_x_rx2_ramp = rampcnt_t_adr->l_x_rx2;		/* Ｘ帯受信設備２ランプ制御有無（臼田専用） */
				sd.m_satinfo[i_i].i_sx_acqrx_ramp = rampcnt_t_adr->l_sx_acqrx;	/* ＳＸ帯捕捉受信設備ランプ制御有無（鹿児島専用） */
				sd.m_satinfo[i_i].i_x_qpskrx_ramp = rampcnt_t_adr->l_x_qpskrx;	/* Ｘ帯高速受信設備ランプ制御有無 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*----------------------------------*/
		/* 天頂通過監視角度情報取得 */
		/*----------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == TOPANGL_ID)
			{
				/*識別ＩＤが「天頂通過監視角度情報」の場合*/
				topangl_t_adr = (struct topangl_t *)satinf_adr;
				sd.m_satinfo[i_i].i_ent_angle = topangl_t_adr->l_in;		/* 天頂通過監視入角 */
				sd.m_satinfo[i_i].i_ext_angle = topangl_t_adr->l_out;		/* 天頂通過監視出角 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*--------------------------*/
		/* <<XUP>> 衛星捕捉情報取得 */
		/*--------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == CATCH_ID)
			{
				/*識別ＩＤが「衛星捕捉情報」の場合*/
				catch_t_adr = (struct catch_t *)satinf_adr;
				sd.m_satinfo[i_i].i_acq_route = catch_t_adr->l_route;	/* 捕捉ルート番号 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;            /*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;    /*char型に変換*/

		/*--------------------------------------*/
		/* ＡＧＣ校正対象受信機情報取得 */
		/*--------------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{          /*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == AGCRX_ID)
			{
				/*識別ＩＤが「ＡＧＣ校正対象受信機情報」の場合*/
				agcrx_t_adr = (struct agcrx_t *)satinf_adr;
				sd.m_satinfo[i_i].i_s_rx = agcrx_t_adr->l_s_rx;				/* Ｓ帯主受信設備 */
				sd.m_satinfo[i_i].i_x_rx = agcrx_t_adr->l_x_rx;				/* Ｘ帯主受信設備 */
				sd.m_satinfo[i_i].i_x_rx1 = agcrx_t_adr->l_x_rx1;			/* Ｘ帯主受信設備１ */
				sd.m_satinfo[i_i].i_x_rx2 = agcrx_t_adr->l_x_rx2;			/* Ｘ帯主受信設備２ */
				sd.m_satinfo[i_i].i_sx_acqrx_s = agcrx_t_adr->l_sx_acqrx_s;	/* ＳＸ帯補足用受信設備　Ｓ帯 */
				sd.m_satinfo[i_i].i_sx_acqrx_x = agcrx_t_adr->l_sx_acqrx_x;	/* ＳＸ帯補足用受信設備　Ｘ帯 */
				sd.m_satinfo[i_i].i_x_qpskrx = agcrx_t_adr->l_x_qpskrx;		/* Ｘ帯高速データ用受信設備 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;            /*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;    /*char型に変換*/

		/*----------------------------------*/
		/* ノミナル受信周波数情報取得 */
		/*----------------------------------*/
		sd.m_satinfo[i_i].i_rxfq_ka = 0;
		sd.m_satinfo[i_i].i_rxfq_x = 0;
		sd.m_satinfo[i_j].i_rxfq_s = 0;
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFRXFQ_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == RXFQVAL_ID)
			{
				/*識別ＩＤがの場合*/
				rxfqval_adr = (struct rxfqval_t *)satinf_adr;
				if (rxfqval_adr->l_band == RXBAND_S_ID)
				{
					if (rxfqval_adr->d_fq != 0)
					{
						// S 帯有り
						sd.m_satinfo[i_i].i_rxfq_s = 1;
					}
				}
				if (rxfqval_adr->l_band == RXBAND_X_ID)
				{
					if (rxfqval_adr->d_fq != 0)
					{
						// X 帯有り
						sd.m_satinfo[i_i].i_rxfq_x = 1;
					}
				}
				if (rxfqval_adr->l_band == RXBAND_KA_ID)
				{
					if (rxfqval_adr->d_fq != 0)
					{
						// Ka 帯有り
						sd.m_satinfo[i_i].i_rxfq_ka = 1;
					}
				}
			}

			sattbl_adr += satinf_adr->l_len;
		}

		satinf_adr = original_adr;				/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;		/*char型に変換*/

		/*-----------------------*/
		/* S/X-CMD BITRATEの取得 */
		/*-----------------------*/
		if ((sd.m_satinfo[i_i].i_cmd_bitrate = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(C_BIT2).GetBuffer(), sd.m_satinfo[i_i].cmd_tbl, cmd_br_vec, TBLMAX)) == -1)
		{
			if ((sd.m_satinfo[i_i].i_cmd_bitrate = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(C_BIT).GetBuffer(), sd.m_satinfo[i_i].cmd_tbl, cmd_br_vec, TBLMAX)) == -1)
			{
				sd.m_satinfo[i_i].i_cmd_bitrate = 0;
			}
		}

		/*---------------------*/
		/* X-CMD BITRATEの取得 */
		/*---------------------*/
		if ((sd.m_satinfo[i_j].i_x_cmd_bitrate = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(C_BIT3).GetBuffer(), sd.m_satinfo[i_j].x_cmd_tbl, cmd_br_vec, TBLMAX)) == -1)
		{
			if ((sd.m_satinfo[i_j].i_x_cmd_bitrate = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(C_BIT4).GetBuffer(), sd.m_satinfo[i_j].x_cmd_tbl, cmd_br_vec, TBLMAX)) == -1)
			{
				sd.m_satinfo[i_j].i_x_cmd_bitrate = 0;
			}
		}

		/*-----------------*/
		/* S BITRATEの取得 */
		/*-----------------*/
		if ((sd.m_satinfo[i_i].i_bitrate_s = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(BIT_S).GetBuffer(), sd.m_satinfo[i_i].br_s_tbl, br_s_vec, TBLMAX)) == -1)
		{
			sd.m_satinfo[i_i].i_bitrate_s = 0;
		}

		/*-----------------*/
		/* X BITRATEの取得 */
		/*-----------------*/
		if ((sd.m_satinfo[i_i].i_bitrate_x = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(BIT_X).GetBuffer(), sd.m_satinfo[i_i].br_x_tbl, br_x_vec, TBLMAX * 2)) == -1)
		{
			sd.m_satinfo[i_i].i_bitrate_x = 0;
		}

		/*-----------------*/
		/* X2 BITRATEの取得 */
		/*-----------------*/
		if ((sd.m_satinfo[i_i].i_bitrate_x2 = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(BIT_X2).GetBuffer(), sd.m_satinfo[i_i].br_x_tbl, br_x_vec, TBLMAX * 2)) == -1)
		{
			sd.m_satinfo[i_i].i_bitrate_x2 = 0;
		}

		/*--------------------------*//* 2008-02-07 再生測距対応　追加 */
		/* ＲＡＲＲ使用帯域情報取得 */
		/*--------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == RARRBAND_ID)
			{
				/*識別ＩＤが「ＲＡＲＲ使用帯域情報」の場合*/
				rarr_t_adr = (struct rarrband_t *)satinf_adr;
				sd.m_satinfo[i_i].i_rarr_band = rarr_t_adr->l_rarr_band;	/* ＲＡＲＲ使用帯域番号 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;            /*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;    /*char型に変換*/

		i_i++;
	}

	if ((cnt != 0) && (plan_t_adr != NULL))
	{
		char *cp_ptr;
		strcpy_s(sd.m_satinfo[i_i].sz_satnames, sizeof(sd.m_satinfo[i_i].sz_satnames), "MAINT");
		strcpy_s(sd.m_satinfo[i_i].sz_plan_dir, sizeof(sd.m_satinfo[i_i].sz_plan_dir), plan_t_adr->sz_planpath);
		cp_ptr = strrchr(sd.m_satinfo[i_i].sz_plan_dir, '/');
		*cp_ptr = '\0';
		strcpy_s(strrchr(sd.m_satinfo[i_i].sz_plan_dir, '/') + 1, sizeof(sd.m_satinfo[i_i].sz_plan_dir), "maint/");
		i_i++;
		strcpy_s(sd.m_satinfo[i_i].sz_satnames, sizeof(sd.m_satinfo[i_i].sz_satnames), "VLBI");
		strcpy_s(sd.m_satinfo[i_i].sz_plan_dir, sizeof(sd.m_satinfo[i_i].sz_plan_dir), plan_t_adr->sz_planpath);
		cp_ptr = strrchr(sd.m_satinfo[i_i].sz_plan_dir, '/');
		*cp_ptr = '\0';
		strcpy_s(strrchr(sd.m_satinfo[i_i].sz_plan_dir, '/') + 1, sizeof(sd.m_satinfo[i_i].sz_plan_dir), "vlbi/");
		i_i++;
		//	i_sat_num = i_i;
	}


	return 0;
}

/*============================================================================*/
/*! COperationPlan

-# 衛星固有DBより取得したデータより該当する衛星のインデックスを取得する

@brief このインデックスはリスト上のもの。

@param	str :衛星名
@retval	インデックス
*/
/*============================================================================*/
int COperationPlan::GetIndexEqSatDBData(CString str)
{
	int idx = -1;

	CSatelliteData& sd = theApp.GetSatelliteData();
	CDBAccess& dba = sd.GetDBAccessCls();
	vector<CString> &saNameList = dba.GetSatNameList();

	int cnt = (int)saNameList.size();

	if (cnt == 0)
	{
		// サイズ0はデータベースオープンに失敗している。
		return -1;
	}

	for (int i = 0; i < cnt; ++i)
	{
		CString satname = saNameList[i];
		if (satname == str)
		{
			idx = i;
			break;
		}
	}

	if (idx == -1)
	{
		CString mes;
		mes.Format(_T("衛星固有情報データベースに%sの情報が有りません。"), (LPCTSTR)str);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
	}

	return idx;
}

/*============================================================================*/
/*! COperationPlan

-# 予報値ファイル(anp)からデータ取得

@param	なし
@retval	なし
*/
/*============================================================================*/
void COperationPlan::GetAnpData()
{
	CString anpPath;
	FILE *fp;

	char	szLine[512];
	TCHAR	szBuff[512];

	TCHAR	szTemp1[8];
	TCHAR	szTemp2[11];
	TCHAR	szHdrTime[15];
	TCHAR	szHdrAz[9];
	TCHAR	szHdrEl[9];
	TCHAR	szTemp4[15];

	TCHAR	szTime[15] = { 0 };	// 時刻
	TCHAR	szAZ[9] = { 0 };	// 方位角
	TCHAR	szEL[8] = { 0 };	// 仰角
	TCHAR	szUpRNG[18];		// ＲＮＧ（アップリンク）
	TCHAR	szUpRR[12];			// ＲＲ  （アップリンク）
	TCHAR	szDwRNG[18];		// ＲＮＧ（ダウンリンク）
	TCHAR	szDwRR[12];			// ＲＲ  （ダウンリンク）

	COleDateTime oleTime;
	CTime tmpTime;

	stAnpData st;

	if (m_PredFileName == _T("") || m_strSatellite == _T(""))
	{
		return;
	}

	// 予報値フォルダ取得
	CString dir = theApp.GetShareFilePath(eFileType_Pred_Folder, theApp.GetSelectStation());

	// 予報値ファイルフルパス取得
	anpPath.Format(_T("%s%s\\%s"), (LPCTSTR)dir, (LPCTSTR)m_strSatellite, (LPCTSTR)m_PredFileName);

	// 予報値ファイルより、日時、AZ、EL取得
	if ((_wfopen_s(&fp, anpPath, _T("r")) != 0) || (fp == NULL))
	{
		return;
	}

	m_AnpDataList.clear();

	// ヘッダー部読み飛ばし
	for (int i = 0; i < 5; ++i)
	{
		fgets(szLine, static_cast<unsigned>(_countof(szLine)), fp);

		if (i < 2)
		{
			continue;
		}

		wsprintf(szBuff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(szLine));
		swscanf_s(szBuff, _T("%s %s %s %s %s %s"),
			szTemp1, static_cast<unsigned>(_countof(szTemp1)),
			szTemp2, static_cast<unsigned>(_countof(szTemp2)),
			szHdrTime, static_cast<unsigned>(_countof(szHdrTime)),
			szHdrAz, static_cast<unsigned>(_countof(szHdrAz)),
			szHdrEl, static_cast<unsigned>(_countof(szHdrEl)),
			szTemp4, static_cast<unsigned>(_countof(szTemp4)));

		// 日時取得
		CString str = CString(szHdrTime);
		str.Format(_T("%s-%s-%s %s:%s:%s"), (LPCTSTR)str.Mid(0, 4), (LPCTSTR)str.Mid(4, 2), (LPCTSTR)str.Mid(6, 2), (LPCTSTR)str.Mid(8, 2), (LPCTSTR)str.Mid(10, 2), (LPCTSTR)str.Mid(12, 2));
		oleTime.ParseDateTime(str);
		CTime hTime = CTime(oleTime.GetYear(), oleTime.GetMonth(), oleTime.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());

		if (i == 2)
		{
			// AOS
			m_AnpDataAos.time = hTime;
			m_AnpDataAos.AZ = _wtof(szHdrAz);
			m_AnpDataAos.EL = _wtof(szHdrEl);
		}
		else if (i == 3)
		{
			// LOS
			m_AnpDataLos.time = hTime;
			m_AnpDataLos.AZ = _wtof(szHdrAz);
			m_AnpDataLos.EL = _wtof(szHdrEl);
		}
		else if (i == 4)
		{
			// MAX
			m_AnpDataMax.time = hTime;
			m_AnpDataMax.AZ = _wtof(szHdrAz);
			m_AnpDataMax.EL = _wtof(szHdrEl);
		}
	}

	while (fgets(szLine, static_cast<unsigned>(_countof(szLine)), fp) != NULL)
	{
		wsprintf(szBuff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(szLine));
		swscanf_s(szBuff, _T("%s %s %s %s %s %s %s"),
			szTime, static_cast<unsigned>(_countof(szTime)),
			szAZ, static_cast<unsigned>(_countof(szAZ)),
			szEL, static_cast<unsigned>(_countof(szEL)),
			szUpRNG, static_cast<unsigned>(_countof(szUpRNG)),
			szUpRR, static_cast<unsigned>(_countof(szUpRR)),
			szDwRNG, static_cast<unsigned>(_countof(szDwRNG)),
			szDwRR, static_cast<unsigned>(_countof(szDwRR)));

		// 日時取得
		CString str = CString(szTime);
		str.Format(_T("%s-%s-%s %s:%s:%s"), (LPCTSTR)str.Mid(0, 4), (LPCTSTR)str.Mid(4, 2), (LPCTSTR)str.Mid(6, 2), (LPCTSTR)str.Mid(8, 2), (LPCTSTR)str.Mid(10, 2), (LPCTSTR)str.Mid(12, 2));
		oleTime.ParseDateTime(str);
		st.time = CTime(oleTime.GetYear(), oleTime.GetMonth(), oleTime.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());
//		str = tmpTime.Format(_T("%Y-%m-%d"));

		// AZ EL 取得
		st.AZ = _wtof(szAZ);
		st.EL = _wtof(szEL);

		m_AnpDataList.push_back(st);
	}
	fclose(fp);
}

/*============================================================================*/
/*! COperationPlan

-# スカイラインファイルより、AZとELを取得する

@param	なし
@retval	なし
*/
/*============================================================================*/
BOOL COperationPlan::GetSkylineData()
{
	FILE *fp;
	char	szLine[512];

	// スカイラインファイル取得
	CString file = theApp.GetShareFilePath(eFileType_Skyline_File, theApp.GetSelectStation());

	// スカイラインファイルより、AZ、EL取得
	if ((_wfopen_s(&fp, file, _T("r")) != 0) || (fp == NULL))
	{
		return FALSE;
	}

	int cnt = 0;
	stAzEl st;

	m_SkyLineDataList.clear();

	CString cbuf;
	while (fgets(szLine, static_cast<unsigned>(_countof(szLine)), fp) != NULL)
	{
		cnt++;
		if (cnt < 5)
		{
			// ヘッダー部読み飛ばし（4行）
			continue;
		}

		cbuf = CServerFile::ConvertUnicode(szLine);
		cbuf = cbuf.Mid(0, cbuf.GetLength() - 1);	// 改行コード削除

		int idx = cbuf.Find(_T(":"));
		CString strAz = cbuf.Mid(0, idx);
		CString strEl = cbuf.Mid(idx + 1);

		st.AZ = _wtof(strAz);
		st.EL = _wtof(strEl);

		m_SkyLineDataList.push_back(st);
	}

	fclose(fp);

	return TRUE;
}

/*============================================================================*/
/*! COperationPlan

-# スカイラインファイルより、該当AZ時の対応するELを取得する

@param	type(in)	取得条件　0：AOS側、1：LOS側
@param	el(out)		EL
@retval	なし
*/
/*============================================================================*/
BOOL COperationPlan::GetELforSkylineData(int type, double &el)
{
	BOOL ret = FALSE;
	el = 0.0;

	if (type > 1)
	{
		return FALSE;
	}

	int cnt = (int)m_SkyLineDataList.size();
	stAzEl st;

	for (int i = 0; i < cnt; ++i)
	{
		st = m_SkyLineDataList[i];
		if (type == 0)
		{
			if (m_AnpDataAos.AZ < st.AZ)
			{
				el = st.EL;
				ret = TRUE;
				break;
			}
		}
		else if (type == 1)
		{
			if (m_AnpDataLos.AZ < st.AZ)
			{
				el = st.EL;
				ret = TRUE;
				break;
			}
		}
	}

	return ret;
}

/*============================================================================*/
/*! COperationPlan

-# アンテナ予報値に基づいたAOS側/LOS側のスカイライン交差日時を取得する。

@param	なし
@retval	なし
*/
/*============================================================================*/
void COperationPlan::GetIntersectionDateSkyline(CTime &sTime, CTime &eTime)
{
	double el = 0;
	stAnpData st;

	sTime = m_tAOS;
	eTime = m_tLOS;

	// スカイラインファイルよりAZ/ELリスト作成
	if (GetSkylineData() == FALSE)
	{
		return;
	}

	int cnt = (int)m_AnpDataList.size();
	if (cnt <= 0)
	{
		return;
	}

	//-----------------------------------------------------
	// 開始日時

	// スカイラインファイルより、アンテナ予報値AOS側AZが越える時のELを取得
	if (GetELforSkylineData(0, el) == TRUE)
	{
		// アンテナ予報値ファイルより、ELが超える日時を取得する。
		for (int i = 0; i < cnt; ++i)
		{
			st = m_AnpDataList[i];

			if (el < st.EL)
			{
				sTime = st.time;
				break;
			}
		}
	}

	//-----------------------------------------------------
	// 終了日時

	// スカイラインファイルより、アンテナ予報値LOS側AZが越える時のELを取得
	if (GetELforSkylineData(1, el) == TRUE)
	{
		// アンテナ予報値ファイルより、ELが超える日時を取得する。
		for (int i = (cnt - 1); i >= 0; --i)
		{
			st = m_AnpDataList[i];

			if (el < st.EL)
			{
				eTime = st.time;
				break;
			}
		}
	}



#if 0	// debug
	CString str;
	str = sTime.Format(_T("%Y-%m-%d"));
	str = eTime.Format(_T("%Y-%m-%d"));
#endif
}

/*============================================================================*/
/*! COperationPlan

-# RF-Tripファイルより、AZとELを取得する

@param	なし
@retval	なし
*/
/*============================================================================*/
BOOL COperationPlan::GetRFTripData()
{
	UpdateData();

	// 設定CID取得
	CString strCid;
	m_CidBtnCtrl.GetWindowTextW(strCid);

	stCIDParam stCid;
	map<CString, map<CString, stCIDParam>> &cidlist = theApp.GetSatelliteData().m_CIDSlctData.GetCidParamList();
	if (cidlist.size() > 0)
	{
		stCid = CCIDSlctData::TrimInvalidString(cidlist[m_SatelliteName][strCid]);
	}

	// 変調方式確認
	CString strBPSK = _T("");
	if (stCid.Modulation.CompareNoCase(_T("BPSK")) == 0)
	{
		strBPSK = _T("BPSK");
	}

	CRFTripAngleFile rftrip;
	m_RFTripDataList.clear();
	CString satname = m_SatelliteName;

	if (rftrip.ReadFile(theApp.GetSelectStation(), satname, stCid.TrsPower, strBPSK, m_RFTripDataList) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

/*============================================================================*/
/*! COperationPlan

-# スカイラインファイルより、該当AZ時の対応するELを取得する

@param	type(in)	取得条件　0：AOS側、1：LOS側
@param	el(out)		EL
@retval	なし
*/
/*============================================================================*/
BOOL COperationPlan::GetELforRFTripData(int type, double &el)
{
	el = 0.0;

	if (type > 1)
	{
		return FALSE;
	}

	int cnt = (int)m_RFTripDataList.size();

	if(cnt == 0)
	{
		return FALSE;
	}

	int iAZ = -1;
	if(type == 0)
	{
		iAZ = (int)ceil(m_AnpDataAos.AZ);
		el = m_RFTripDataList[iAZ];
	}
	else if(type == 1)
	{
		iAZ = (int)ceil(m_AnpDataLos.AZ);
		el = m_RFTripDataList[iAZ];
	}

	return TRUE;
}

/*============================================================================*/
/*! COperationPlan

-# アンテナ予報値に基づいたAOS側/LOS側のRF-Trip交差日時を取得する。

@param	なし
@retval	なし
*/
/*============================================================================*/
void COperationPlan::GetIntersectionDateRFTrip(CTime &sTime, CTime &eTime)
{
	double el = 0;
	stAnpData st;

	sTime = m_tAOS;
	eTime = m_tLOS;

	// RF-Trip データ取得
	if (GetRFTripData() == FALSE)
	{
		return;
	}

	int cnt = (int)m_AnpDataList.size();
	if (cnt <= 0)
	{
		return;
	}

	//-----------------------------------------------------
	// 開始日時

	// スカイラインファイルより、アンテナ予報値AOS側AZが越える時のELを取得
	if (GetELforRFTripData(0, el) == TRUE)
	{
		// アンテナ予報値ファイルより、ELが超える日時を取得する。
		for (int i = 0; i < cnt; ++i)
		{
			st = m_AnpDataList[i];

			if (el < st.EL)
			{
				sTime = st.time;
				break;
			}
		}
	}


	//-----------------------------------------------------
	// 終了日時

	// スカイラインファイルより、アンテナ予報値LOS側AZが越える時のELを取得
	if (GetELforRFTripData(1, el) == TRUE)
	{
		// アンテナ予報値ファイルより、ELが超える日時を取得する。
		for (int i = (cnt - 1); i >= 0; --i)
		{
			st = m_AnpDataList[i];

			if (el < st.EL)
			{
				eTime = st.time;
				break;
			}
		}
	}
}


/*============================================================================*/
/*! COperationPlan

-# 閉じるボタン処理

@param	なし
@retval	なし
*/
/*============================================================================*/
void COperationPlan::OnClose()
{
	if (m_bForceClosed)
	{
		// 画面を強制クローズ時
		CDialogEx::OnClose();
		return;
	}

	if (m_bNewPlan == FALSE)
	{
		// 編集時変更チェック
		if (CheckPlanDataDlg())
		{
			if (IDCANCEL == AfxMessageBoxHooked(_T("設定が変更されています。\n設定を破棄しますか？"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			{
				return;
			}
		}
	}

	CDialogBase::OnClose();
}


/*============================================================================*/
/*! COperationPlan

-# 運用開始日時 日付変更処理

@param	なし
@retval	なし
*/
/*============================================================================*/
void COperationPlan::OnDtnDatetimechangeOpeplnBasicprmOpeSdayDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	InvalidOerationGraph();

	*pResult = 0;
}


/*============================================================================*/
/*! COperationPlan

-# 運用開始日時 時刻変更処理

@param	なし
@retval	なし
*/
/*============================================================================*/
void COperationPlan::OnDtnDatetimechangeOpeplnBasicprmOpeStimeDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	InvalidOerationGraph();

	*pResult = 0;
}


/*============================================================================*/
/*! COperationPlan

-# 運用終了日時 日付変更処理

@param	なし
@retval	なし
*/
/*============================================================================*/
void COperationPlan::OnDtnDatetimechangeOpeplnBasicprmOpeEdayDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	InvalidOerationGraph();

	*pResult = 0;
}


/*============================================================================*/
/*! COperationPlan

-# 運用終了日時 時刻変更処理

@param	なし
@retval	なし
*/
/*============================================================================*/
void COperationPlan::OnDtnDatetimechangeOpeplnBasicprmOpeEtimeDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	InvalidOerationGraph();

	*pResult = 0;
}

/*============================================================================*/
/*! COperationPlan

-# 運用開始/終了日時変更時グラフ描画処理

@param	なし
@retval	なし
*/
/*============================================================================*/
void COperationPlan::InvalidOerationGraph()
{
	UpdateData();

	m_tOpeS = CTime(m_OpeStartDay.GetYear(), m_OpeStartDay.GetMonth(), m_OpeStartDay.GetDay(), m_OpeStartTime.GetHour(), m_OpeStartTime.GetMinute(), m_OpeStartTime.GetSecond());;
	m_tOpeE = CTime(m_OpeEndDay.GetYear(), m_OpeEndDay.GetMonth(), m_OpeEndDay.GetDay(), m_OpeEndTime.GetHour(), m_OpeEndTime.GetMinute(), m_OpeEndTime.GetSecond());;
	m_OperationGraph.Invalidate();
}

/*============================================================================*/
/*! COperationPlan

-# 計画データを初期化する

@param	なし
@retval	なし
*/
/*============================================================================*/
void COperationPlan::InitPlanData()
{
	m_PlanData.sz_filepath = _T("");
	m_PlanData.sz_ftpfilepath = _T("");
	m_PlanData.sz_start = _T("");
	m_PlanData.sz_end = _T("");
	m_PlanData.sz_s_bitrate = _T("");
	m_PlanData.sz_x_bitrate = _T("");
	m_PlanData.sz_ka_bitrate = _T("");
	m_PlanData.sz_x_qpsk = _T("");
	m_PlanData.sz_cmd_bit_rate = _T("");
	m_PlanData.sz_x_cmd_bit_rate = _T("");
	m_PlanData.l_cmd_mod = 0;
	m_PlanData.l_rng_mod = 0;
	m_PlanData.sz_rng_band = _T("");
	m_PlanData.sz_rng_start = _T("");
	m_PlanData.sz_rng_end = _T("");
	m_PlanData.l_rdychk = 0;
	m_PlanData.l_init = 0;
	m_PlanData.l_auto_acq = 0;
	m_PlanData.l_offset = 0;
	m_PlanData.l_a_flag = 0;
	m_PlanData.l_l_flag = 0;
	m_PlanData.l_term_ant = 1;
	m_PlanData.l_tx_ramp = 0;
	m_PlanData.sz_tx_ramp = _T("");
	m_PlanData.l_rx_ramp = 0;
	m_PlanData.l_acq_route = 0;
	m_PlanData.l_pl_sel = 0;
	m_PlanData.l_top_mon = 0;
	m_PlanData.l_ent_angle = 0;
	m_PlanData.l_ext_angle = 0;
	m_PlanData.sz_level_s = _T("");
	m_PlanData.sz_level_x = _T("");
	m_PlanData.sz_ant_s = _T("");
	m_PlanData.sz_ant_x = _T("");
	m_PlanData.sz_pred_path = _T("");
	m_PlanData.l_offset_val = 0;
	m_PlanData.l_delay_sel = 0;
	m_PlanData.l_tx_pwr_sel = 0;
	m_PlanData.sz_tx_pwr_sel = _T("");
	m_PlanData.l_mod_sel = 0;
	m_PlanData.l_tx_uc_sel = -1;
	m_PlanData.l_antwait_flg = 0;
	m_PlanData.sz_antwait_flg = _T("");
	m_PlanData.l_ant_time = 0;
	m_PlanData.l_rdy_time = 0;
	m_PlanData.l_agc_index = 0;
	m_PlanData.l_agc_time = 0;
	m_PlanData.l_agc_s_time = 0;
	m_PlanData.l_agc_x_time = 0;
	m_PlanData.l_rng_index = 0;
	m_PlanData.l_rng_time = 0;
	m_PlanData.l_rng_xx_time = 0;
	m_PlanData.l_rng_rx_time = 0;
	m_PlanData.l_tlm_index = 0;
	m_PlanData.l_tlm_time = 0;
	m_PlanData.l_tlm_s_time = 0;
	m_PlanData.l_tlm_x_time = 0;
	m_PlanData.l_cmd_index = 0;
	m_PlanData.l_cmd_time = 0;
	m_PlanData.l_range_band = 0;
	m_PlanData.l_ant_stow = 2;
	m_PlanData.sz_ant_stow = _T("");
	m_PlanData.l_route_mode = 0;
	m_PlanData.l_tx_mode = 0;

	m_PlanData.l_uplink = 0;
	m_PlanData.sz_uplink_s_timelst.clear();
	m_PlanData.sz_uplink_e_timelst.clear();
	m_PlanData.sz_rng_kindlst.clear();
	m_PlanData.l_rng = 0;
	m_PlanData.sz_rng_s_timelst.clear();
	m_PlanData.sz_rng_e_timelst.clear();
	m_PlanData.l_cmd = 0;
	m_PlanData.sz_cmd_s_timelst.clear();
	m_PlanData.sz_cmd_e_timelst.clear();
	m_PlanData.sz_cmd_type = _T("");
	m_PlanData.l_tlms = 0;
	m_PlanData.sz_tlms_s_timelst.clear();
	m_PlanData.sz_tlms_e_timelst.clear();
	m_PlanData.l_tlmx = 0;
	m_PlanData.sz_tlmx_s_timelst.clear();
	m_PlanData.sz_tlmx_e_timelst.clear();
	m_PlanData.l_tlmka = 0;
	m_PlanData.sz_tlmka_s_timelst.clear();
	m_PlanData.sz_tlmka_e_timelst.clear();
	m_PlanData.sz_cid = _T("");
	m_PlanData.sz_station = _T("");
	m_PlanData.l_selene_4way = 0;
	m_PlanData.sz_priority = _T("");
	m_PlanData.sz_ope_mode = _T("");
	m_PlanData.sz_auto = _T("");

	m_PlanData.l_antwait_time = 0;
	m_PlanData.l_antinit_time = 0;
	m_PlanData.l_initmin_time = 0;

	m_PlanData.sz_plan_type = _T("");
}

