/*============================================================================*/
/*! OperationPlan.cpp

-# CID選択画面
-# 
-# 衛星名を指定した場合は、衛星選択コンボボックスは指定した衛星名でマスクされます。
-# また、衛星名とCIDを指定した場合は、そのCIDのパラメータが初期値になります。
*/
/*============================================================================*/
// CidSelect.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "CidSelect.h"
#include "afxdialogex.h"

#define CIDSEL_GREAT	1

namespace timeline{

/* ------------------------------------------------------------------------------------ */
/* CID選択画面データ管理クラス
/* ------------------------------------------------------------------------------------ */
CCIDSlctData::CCIDSlctData()
{
}


CCIDSlctData::~CCIDSlctData()
{
}

/*============================================================================*/
/*! CSatelliteData

-# 衛星毎のCIDリスト取得

@param  sat		衛星名
@retval CIDリスト（vector<CString>）
*/
/*============================================================================*/
vector<CString> CCIDSlctData::GetCidList(CString sat)
{
	vector<CString> cidlist;

	for (auto itr = m_CidParamList.begin(); itr != m_CidParamList.end(); ++itr)
	{
		if (sat == itr->first)
		{
			map<CString, stCIDParam> &list = itr->second;

			for (auto itr2 = list.begin(); itr2 != list.end(); ++itr2)
			{
				cidlist.push_back(itr2->first);
			}
		}
	}

	return cidlist;
}

/*============================================================================*/
/*! CCIDSlctData

-# CIDから無効文字列(CID_INVALID)除去

@param  stCid	入力CIDパラメータ
@retval st		出力CIDパラメータ
*/
/*============================================================================*/
stCIDParam CCIDSlctData::TrimInvalidString(stCIDParam stCid)
{
	stCIDParam st;
	(stCid.CID != CID_INVALID) ? st.CID = stCid.CID : st.CID = _T("");
	(stCid.TlmBitRateS != CID_INVALID) ? st.TlmBitRateS = stCid.TlmBitRateS : st.TlmBitRateS = _T("");
	(stCid.TlmBitRateX != CID_INVALID) ? st.TlmBitRateX = stCid.TlmBitRateX : st.TlmBitRateX = _T("");
	(stCid.TlmBitRateHX != CID_INVALID) ? st.TlmBitRateHX = stCid.TlmBitRateHX : st.TlmBitRateHX = _T("");
	(stCid.TlmBitRateKa != CID_INVALID) ? st.TlmBitRateKa = stCid.TlmBitRateKa : st.TlmBitRateKa = _T("");
	(stCid.CmdBitRateS != CID_INVALID) ? st.CmdBitRateS = stCid.CmdBitRateS : st.CmdBitRateS = _T("");
	(stCid.CmdBitRateX != CID_INVALID) ? st.CmdBitRateX = stCid.CmdBitRateX : st.CmdBitRateX = _T("");
	(stCid.TrsPower != CID_INVALID) ? st.TrsPower = stCid.TrsPower : st.TrsPower = _T("");
	(stCid.CarrLoopBandS != CID_INVALID) ? st.CarrLoopBandS = stCid.CarrLoopBandS : st.CarrLoopBandS = _T("");
	(stCid.CarrLoopBandX != CID_INVALID) ? st.CarrLoopBandX = stCid.CarrLoopBandX : st.CarrLoopBandX = _T("");
	(stCid.CarrLoopBandKa != CID_INVALID) ? st.CarrLoopBandKa = stCid.CarrLoopBandKa : st.CarrLoopBandKa = _T("");
	(stCid.UpLinkSweepShape != CID_INVALID) ? st.UpLinkSweepShape = stCid.UpLinkSweepShape : st.UpLinkSweepShape = _T("");
	(stCid.UpLinkSweepWidth != CID_INVALID) ? st.UpLinkSweepWidth = stCid.UpLinkSweepWidth : st.UpLinkSweepWidth = _T("");
	(stCid.UpLinkSweepTime != CID_INVALID) ? st.UpLinkSweepTime = stCid.UpLinkSweepTime : st.UpLinkSweepTime = _T("");
	(stCid.RngMesMode != CID_INVALID) ? st.RngMesMode = stCid.RngMesMode : st.RngMesMode = _T("");
	(stCid.RngMesSmplRate != CID_INVALID) ? st.RngMesSmplRate = stCid.RngMesSmplRate : st.RngMesSmplRate = _T("");
	(stCid.RngMesSmplItgTime != CID_INVALID) ? st.RngMesSmplItgTime = stCid.RngMesSmplItgTime : st.RngMesSmplItgTime = _T("");
	(stCid.RngMesSmplMod != CID_INVALID) ? st.RngMesSmplMod = stCid.RngMesSmplMod : st.RngMesSmplMod = _T("");
	(stCid.Modulation != CID_INVALID) ? st.Modulation = stCid.Modulation : st.Modulation = _T("");
	(stCid.ModulationS != CID_INVALID) ? st.ModulationS = stCid.ModulationS : st.ModulationS = _T("");
	(stCid.ModulationX != CID_INVALID) ? st.ModulationX = stCid.ModulationX : st.ModulationX = _T("");
	(stCid.CmdMod != CID_INVALID) ? st.CmdMod = stCid.CmdMod : st.CmdMod = _T("");
	(stCid.CmdSubCarrFreq != CID_INVALID) ? st.CmdSubCarrFreq = stCid.CmdSubCarrFreq : st.CmdSubCarrFreq = _T("");
	(stCid.TransDoppler != CID_INVALID) ? st.TransDoppler = stCid.TransDoppler : st.TransDoppler = _T("");
	(stCid.AntAutoTrack != CID_INVALID) ? st.AntAutoTrack = stCid.AntAutoTrack : st.AntAutoTrack = _T("");
	(stCid.AntAosPosn != CID_INVALID) ? st.AntAosPosn = stCid.AntAosPosn : st.AntAosPosn = _T("");
	(stCid.AntWaitPoint != CID_INVALID) ? st.AntWaitPoint = stCid.AntWaitPoint : st.AntWaitPoint = _T("");

	return st;
}


// CCidSelect ダイアログ

IMPLEMENT_DYNAMIC(CCidSelect, CDialogEx)

CCidSelect::CCidSelect(CWnd* pParent /*=NULL*/)
: CDialogEx(CCidSelect::IDD, pParent)
, m_strSatelliteCmb(_T(""))
, m_strCIDCmb(_T(""))
, m_strTlmBRSCmb(_T(""))
, m_strTlmBRXCmb(_T(""))
, m_strTlmBRHXCmb(_T(""))
, m_strTlmBRKaCmb(_T(""))
, m_strCmdBRSCmb(_T(""))
, m_strCmdBRXCmb(_T(""))
, m_strTPwrCmb(_T(""))
, m_strCarrTLbSCmb(_T(""))
, m_strCarrTLbXCmb(_T(""))
, m_strCarrTLbKaCmb(_T(""))
, m_strUpLinkShapeCmb(_T(""))
, m_strRngPrmModeCmb(_T(""))
, m_strCmdMdlCmb(_T(""))
, m_strCmdFreqCmb(_T(""))
, m_strMdlSCmb(_T(""))
, m_strMdlXCmb(_T(""))
, m_strTDoppCmb(_T(""))
, m_strAntAutoCmb(_T(""))
, m_strAntStandbyCmb(_T(""))
, m_strAntStandbyPosCmb(_T(""))
{
	m_xtx = XTX_N_ID;
}

CCidSelect::~CCidSelect()
{
}

void CCidSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CIDSEL_SATELLITE_CMB, m_SetalliteCmb);
	DDX_Control(pDX, IDC_CIDSEL_CID_CMB, m_CidCmb);
	DDX_Control(pDX, IDC_CIDSEL_TLMBR_S_CMB, m_TlmBrSCmb);
	DDX_Control(pDX, IDC_CIDSEL_TLMBR_X_CMB, m_TlmBrXCmb);
	DDX_Control(pDX, IDC_CIDSEL_TLMBR_HX_CMB, m_TlmBrHXCmb);
	DDX_Control(pDX, IDC_CIDSEL_TLMBR_KA_CMB, m_TlmBrKaCmb);
	DDX_Control(pDX, IDC_CIDSEL_CLB_S_CMB, m_CarrTLbSCmb);
	DDX_Control(pDX, IDC_CIDSEL_CLB_X_CMB, m_CarrTLbXCmb);
	DDX_Control(pDX, IDC_CIDSEL_CLB_KA_CMB, m_CarrTLbKaCmb);
	DDX_Control(pDX, IDC_CIDSEL_RPRM_SR_CMB, m_RngPrmModeCmb);
	DDX_Control(pDX, IDC_CIDSEL_MMTD_S_CMB, m_MdlSCmb);
	DDX_Control(pDX, IDC_CIDSEL_MMTD_X_CMB, m_MdlXCmb);
	DDX_Control(pDX, IDC_CIDSEL_ANTAT_MIG_CMB, m_AntAutoCmb);
	DDX_Control(pDX, IDC_CIDSEL_CMDBR_S_CMB, m_CmdBrSCmb);
	DDX_Control(pDX, IDC_CIDSEL_CMDBR_X_CMB, m_CmdBrXCmb);
	DDX_Control(pDX, IDC_CIDSEL_TPWR_PWR_CMB, m_TPwrCmb);
	DDX_Control(pDX, IDC_CIDSEL_UPLNK_SHP_CMB, m_UpLinkShapeCmb);
	DDX_Control(pDX, IDC_CIDSEL_CMDMDL_MDL_CMB, m_CmdMdlCmb);
	DDX_Control(pDX, IDC_CIDSEL_CMDSUBFREQ_FREQ_CMB, m_CmdFreqCmb);
	DDX_Control(pDX, IDC_CIDSEL_TDOP_CMP_CMB, m_TDoppCmb);
	DDX_CBString(pDX, IDC_CIDSEL_SATELLITE_CMB, m_strSatelliteCmb);
	DDX_CBString(pDX, IDC_CIDSEL_CID_CMB, m_strCIDCmb);
	DDX_CBString(pDX, IDC_CIDSEL_TLMBR_S_CMB, m_strTlmBRSCmb);
	DDX_CBString(pDX, IDC_CIDSEL_TLMBR_X_CMB, m_strTlmBRXCmb);
	DDX_CBString(pDX, IDC_CIDSEL_TLMBR_HX_CMB, m_strTlmBRHXCmb);
	DDX_CBString(pDX, IDC_CIDSEL_TLMBR_KA_CMB, m_strTlmBRKaCmb);
	DDX_CBString(pDX, IDC_CIDSEL_CMDBR_S_CMB, m_strCmdBRSCmb);
	DDX_CBString(pDX, IDC_CIDSEL_CMDBR_X_CMB, m_strCmdBRXCmb);
	DDX_CBString(pDX, IDC_CIDSEL_TPWR_PWR_CMB, m_strTPwrCmb);
	DDX_CBString(pDX, IDC_CIDSEL_CLB_S_CMB, m_strCarrTLbSCmb);
	DDX_CBString(pDX, IDC_CIDSEL_CLB_X_CMB, m_strCarrTLbXCmb);
	DDX_CBString(pDX, IDC_CIDSEL_CLB_KA_CMB, m_strCarrTLbKaCmb);
	DDX_CBString(pDX, IDC_CIDSEL_UPLNK_SHP_CMB, m_strUpLinkShapeCmb);
	DDX_CBString(pDX, IDC_CIDSEL_RPRM_SR_CMB, m_strRngPrmModeCmb);
	DDX_CBString(pDX, IDC_CIDSEL_CMDMDL_MDL_CMB, m_strCmdMdlCmb);
	DDX_CBString(pDX, IDC_CIDSEL_CMDSUBFREQ_FREQ_CMB, m_strCmdFreqCmb);
	DDX_CBString(pDX, IDC_CIDSEL_MMTD_S_CMB, m_strMdlSCmb);
	DDX_CBString(pDX, IDC_CIDSEL_MMTD_X_CMB, m_strMdlXCmb);
	DDX_CBString(pDX, IDC_CIDSEL_TDOP_CMP_CMB, m_strTDoppCmb);
	DDX_CBString(pDX, IDC_CIDSEL_ANTAT_MIG_CMB, m_strAntAutoCmb);
	DDX_Control(pDX, IDC_CIDSEL_ANTSTDORIENTED_CMB, m_AntStandbyCmb);
	DDX_CBString(pDX, IDC_CIDSEL_ANTSTDORIENTED_CMB, m_strAntStandbyCmb);
	DDX_Control(pDX, IDC_CIDSEL_ANTSTDPOSORIENTED_CMB, m_AntStandbyPosCmb);
	DDX_CBString(pDX, IDC_CIDSEL_ANTSTDPOSORIENTED_CMB, m_strAntStandbyPosCmb);
}


BEGIN_MESSAGE_MAP(CCidSelect, CDialogEx)
	ON_CBN_SELCHANGE(IDC_CIDSEL_SATELLITE_CMB, &CCidSelect::OnCbnSelchangeCidselSatelliteCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_CID_CMB, &CCidSelect::OnCbnSelchangeCidselCidCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_TLMBR_S_CMB, &CCidSelect::OnCbnSelchangeCidselTlmbrSCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_TLMBR_X_CMB, &CCidSelect::OnCbnSelchangeCidselTlmbrXCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_TLMBR_HX_CMB, &CCidSelect::OnCbnSelchangeCidselTlmbrHxCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_TLMBR_KA_CMB, &CCidSelect::OnCbnSelchangeCidselTlmbrKaCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_CMDBR_S_CMB, &CCidSelect::OnCbnSelchangeCidselCmdbrSCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_CMDBR_X_CMB, &CCidSelect::OnCbnSelchangeCidselCmdbrXCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_TPWR_PWR_CMB, &CCidSelect::OnCbnSelchangeCidselTpwrPwrCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_CLB_S_CMB, &CCidSelect::OnCbnSelchangeCidselClbSCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_CLB_X_CMB, &CCidSelect::OnCbnSelchangeCidselClbXCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_CLB_KA_CMB, &CCidSelect::OnCbnSelchangeCidselClbKaCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_UPLNK_SHP_CMB, &CCidSelect::OnCbnSelchangeCidselUplnkShpCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_RPRM_SR_CMB, &CCidSelect::OnCbnSelchangeCidselRprmSrCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_CMDMDL_MDL_CMB, &CCidSelect::OnCbnSelchangeCidselCmdmdlMdlCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_CMDSUBFREQ_FREQ_CMB, &CCidSelect::OnCbnSelchangeCidselCmdsubfreqFreqCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_MMTD_S_CMB, &CCidSelect::OnCbnSelchangeCidselMmtdSCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_MMTD_X_CMB, &CCidSelect::OnCbnSelchangeCidselMmtdXCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_TDOP_CMP_CMB, &CCidSelect::OnCbnSelchangeCidselTdopCmpCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_ANTAT_MIG_CMB, &CCidSelect::OnCbnSelchangeCidselAntatMigCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_ANTSTDORIENTED_CMB, &CCidSelect::OnCbnSelchangeCidselAntstdorientedCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_ANTSTDPOSORIENTED_CMB, &CCidSelect::OnCbnSelchangeCidselAntstdposorientedCmb)
	ON_BN_CLICKED(IDOK, &CCidSelect::OnBnClickedOk)
END_MESSAGE_MAP()


// CCidSelect メッセージ ハンドラー




/*============================================================================*/
/*! CCidSelect

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CCidSelect::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitDlg();
	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! CCidSelect

-# ダイアログ更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CCidSelect::UpdateDlg()
{
	InitDlg();
	return TRUE;
}

/*============================================================================*/
/*! CCidSelect

-# ダイアログ初期化処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CCidSelect::InitDlg()
{
	ReadCIDFile();

	// コンボボックス 設定
	SetComboboxSatCIDParam(m_strSatelliteName);
	return TRUE;
}

/*============================================================================*/
/*! CCidSelect

-# 衛星選択変更処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CCidSelect::SetComboboxSatCIDParam(CString sat)
{
	CString strSatName = sat;
	BOOL isFindSat = FALSE;

	m_SetalliteCmb.ResetContent();
	map<CString, stCIDPrmEle> &cidEleList = m_CIDSlctData.GetCidElementList();
	for (auto itrs = cidEleList.begin(); itrs != cidEleList.end(); ++itrs)
	{
		CString sat = itrs->first;

		m_SetalliteCmb.InsertString(-1, sat);

		if (strSatName.CompareNoCase(sat) == 0)
		{
			isFindSat = TRUE;
		}
	}

	if (strSatName != _T("") && isFindSat == FALSE)
	{
		AfxMessageBox(_T("該当する衛星のCIDファイルがありません。"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	// 衛星名が設定されている場合は衛星選択コンボボックスを非活性にする
	if (m_strSatelliteName != _T("") && isFindSat == TRUE)
	{
		m_SetalliteCmb.SelectStringEx(-1, strSatName);
		m_SetalliteCmb.EnableWindow(FALSE);
	}
	else
	{
		if (strSatName != _T(""))
		{
			m_SetalliteCmb.SelectStringEx(-1, strSatName);
		}
		else
		{
			m_SetalliteCmb.SetCurSel(0);
			m_SetalliteCmb.GetLBText(m_SetalliteCmb.GetCurSel(), strSatName);
		}
	}

	//// 衛星固有情報取得
	////GetEqSatDB(m_strSatelliteName);
	//TCHAR *p = new TCHAR[80];
	//swprintf_s(p, 80, _T("%s"), (LPCTSTR)m_strSatelliteName);
	//(theApp.m_pMainWnd)->SendMessage(eTIMELINE_GET_XS, 80, (LPARAM)p);
	//m_xtx = _wtoi(CString(p));
	//delete[] p;

	// 各コンボ初期化
	m_CidCmb.ResetContent();
	m_TlmBrSCmb.ResetContent();
	m_TlmBrXCmb.ResetContent();
	m_TlmBrHXCmb.ResetContent();
	m_TlmBrKaCmb.ResetContent();
	m_CmdBrSCmb.ResetContent();
	m_CmdBrXCmb.ResetContent();
	m_TPwrCmb.ResetContent();
	m_CarrTLbSCmb.ResetContent();
	m_CarrTLbXCmb.ResetContent();
	m_CarrTLbKaCmb.ResetContent();
	m_UpLinkShapeCmb.ResetContent();
	m_RngPrmModeCmb.ResetContent();
	m_MdlSCmb.ResetContent();
	m_MdlXCmb.ResetContent();
	m_CmdMdlCmb.ResetContent();
	m_CmdFreqCmb.ResetContent();
	m_TDoppCmb.ResetContent();
	m_AntAutoCmb.ResetContent();
	m_AntStandbyCmb.ResetContent();
	m_AntStandbyPosCmb.ResetContent();

	map<CString, stCIDPrmFlg> &cidPrmFlgList = m_CIDSlctData.GetCidPrmFlgList();
	stCIDPrmFlg stPrmflg = cidPrmFlgList[strSatName];


	for (auto itrs = cidEleList.begin(); itrs != cidEleList.end(); ++itrs)
	{
		CString sat = itrs->first;
		stCIDPrmEle &st = itrs->second;

		if (sat == strSatName)
		{
			// CID
			int selectCID = -1;
			int idx = 0;
			for (auto itri = st.CID.begin(); itri != st.CID.end(); ++itri, ++idx)
			{
				CString item = (*itri);
				m_CidCmb.InsertString(-1, item);

				if (m_strCID == item)
				{
					selectCID = idx;
				}

			}
			if (selectCID != -1)
			{
				m_CidCmb.SetCurSel(selectCID);
			}
			else
			{
				m_CidCmb.SetCurSel(0);
			}

			// テレメトリビットレート S帯
			if (stPrmflg.TlmBitRateS)
			{
				for (auto itri = st.TlmBitRateS.begin(); itri != st.TlmBitRateS.end(); ++itri)
				{
					CString item = (*itri);
					m_TlmBrSCmb.InsertString(-1, item);
				}
				m_TlmBrSCmb.SetCurSel(0);
				m_TlmBrSCmb.EnableWindow(TRUE);
			}
			else
			{
				m_TlmBrSCmb.EnableWindow(FALSE);
			}

			// テレメトリビットレート X帯
			if (stPrmflg.TlmBitRateX)
			{
				for (auto itri = st.TlmBitRateX.begin(); itri != st.TlmBitRateX.end(); ++itri)
				{
					CString item = (*itri);
					m_TlmBrXCmb.InsertString(-1, item);
				}
				m_TlmBrXCmb.SetCurSel(0);
				m_TlmBrXCmb.EnableWindow(TRUE);
			}
			else
			{
				m_TlmBrXCmb.EnableWindow(FALSE);
			}

			// テレメトリビットレート HX帯
			if (stPrmflg.TlmBitRateHX)
			{
				for (auto itri = st.TlmBitRateHX.begin(); itri != st.TlmBitRateHX.end(); ++itri)
				{
					CString item = (*itri);
					m_TlmBrHXCmb.InsertString(-1, item);
				}
				m_TlmBrHXCmb.SetCurSel(0);
				m_TlmBrHXCmb.EnableWindow(TRUE);
			}
			else
			{
				m_TlmBrHXCmb.EnableWindow(FALSE);
			}

			// テレメトリビットレート Ka帯
			if (stPrmflg.TlmBitRateKa)
			{
				for (auto itri = st.TlmBitRateKa.begin(); itri != st.TlmBitRateKa.end(); ++itri)
				{
					CString item = (*itri);
					m_TlmBrKaCmb.InsertString(-1, item);
				}
				m_TlmBrKaCmb.SetCurSel(0);
				m_TlmBrKaCmb.EnableWindow(TRUE);
			}
			else
			{
				m_TlmBrKaCmb.EnableWindow(FALSE);
			}

			// コマンドビットレート S帯
			if (stPrmflg.CmdBitRateS)
			{
				for (auto itri = st.CmdBitRateS.begin(); itri != st.CmdBitRateS.end(); ++itri)
				{
					CString item = (*itri);
					m_CmdBrSCmb.InsertString(-1, item);
				}
				m_CmdBrSCmb.SetCurSel(0);
				m_CmdBrSCmb.EnableWindow(TRUE);
			}
			else
			{
				m_CmdBrSCmb.EnableWindow(FALSE);
			}

			// コマンドビットレート X帯
			if (stPrmflg.CmdBitRateX)
			{
				for (auto itri = st.CmdBitRateX.begin(); itri != st.CmdBitRateX.end(); ++itri)
				{
					CString item = (*itri);
					m_CmdBrXCmb.InsertString(-1, item);
				}
				m_CmdBrXCmb.SetCurSel(0);
				m_CmdBrXCmb.EnableWindow(TRUE);
			}
			else
			{
				m_CmdBrXCmb.EnableWindow(FALSE);
			}

			// 送信電力
			if (stPrmflg.TrsPower)
			{
				for (auto itri = st.TrsPower.begin(); itri != st.TrsPower.end(); ++itri)
				{
					CString item = (*itri);
					m_TPwrCmb.InsertString(-1, item);
				}
				m_TPwrCmb.SetCurSel(0);
				m_TPwrCmb.EnableWindow(TRUE);
			}
			else
			{
				m_TPwrCmb.EnableWindow(FALSE);
			}

			// キャリア受信ループバンド S帯
			if (stPrmflg.CarrLoopBandS)
			{
				for (auto itri = st.CarrLoopBandS.begin(); itri != st.CarrLoopBandS.end(); ++itri)
				{
					CString item = (*itri);
					m_CarrTLbSCmb.InsertString(-1, item);
				}
				m_CarrTLbSCmb.SetCurSel(0);
				m_CarrTLbSCmb.EnableWindow(TRUE);
			}
			else
			{
				m_CarrTLbSCmb.EnableWindow(FALSE);
			}

			// キャリア受信ループバンド X帯
			if (stPrmflg.CarrLoopBandX)
			{
				for (auto itri = st.CarrLoopBandX.begin(); itri != st.CarrLoopBandX.end(); ++itri)
				{
					CString item = (*itri);
					m_CarrTLbXCmb.InsertString(-1, item);
				}
				m_CarrTLbXCmb.SetCurSel(0);
				m_CarrTLbXCmb.EnableWindow(TRUE);
			}
			else
			{
				m_CarrTLbXCmb.EnableWindow(FALSE);
			}

			// キャリア受信ループバンド Ka帯
			if (stPrmflg.CarrLoopBandKa)
			{
				for (auto itri = st.CarrLoopBandKa.begin(); itri != st.CarrLoopBandKa.end(); ++itri)
				{
					CString item = (*itri);
					m_CarrTLbKaCmb.InsertString(-1, item);
				}
				m_CarrTLbKaCmb.SetCurSel(0);
				m_CarrTLbKaCmb.EnableWindow(TRUE);
			}
			else
			{
				m_CarrTLbKaCmb.EnableWindow(FALSE);
			}

			// アップリンク掃引パラメータ 形状
			if (stPrmflg.UpLinkSweepShape)
			{
				for (auto itri = st.UpLinkSweepShape.begin(); itri != st.UpLinkSweepShape.end(); ++itri)
				{
					CString item = (*itri);
					m_UpLinkShapeCmb.InsertString(-1, item);
				}
				m_UpLinkShapeCmb.SetCurSel(0);
				m_UpLinkShapeCmb.EnableWindow(TRUE);
			}
			else
			{
				m_UpLinkShapeCmb.EnableWindow(FALSE);
			}

			// レンジ計測パラメータ モード
			if (stPrmflg.RngMesMode)
			{
				for (auto itri = st.RngMesMode.begin(); itri != st.RngMesMode.end(); ++itri)
				{
					CString item = (*itri);
					m_RngPrmModeCmb.InsertString(-1, item);
				}
				m_RngPrmModeCmb.SetCurSel(0);
				m_RngPrmModeCmb.EnableWindow(TRUE);
			}
			else
			{
				m_RngPrmModeCmb.EnableWindow(FALSE);
			}

			// 変調方式 S帯/X帯
			if (stPrmflg.Modulation)
			{
				if (m_xtx == XTX_Y_ID)	// X帯
				{
					for (auto itri = st.Modulation.begin(); itri != st.Modulation.end(); ++itri)
					{
						CString item = (*itri);
						m_MdlXCmb.InsertString(-1, item);
					}
					m_MdlXCmb.SetCurSel(0);
					m_MdlXCmb.EnableWindow(TRUE);
					m_MdlSCmb.EnableWindow(FALSE);
				}
				else	// S帯
				{
					for (auto itri = st.Modulation.begin(); itri != st.Modulation.end(); ++itri)
					{
						CString item = (*itri);
						m_MdlSCmb.InsertString(-1, item);
					}
					m_MdlSCmb.SetCurSel(0);
					m_MdlSCmb.EnableWindow(TRUE);
					m_MdlXCmb.EnableWindow(FALSE);
				}
			}
			else
			{
				m_MdlSCmb.EnableWindow(FALSE);
				m_MdlXCmb.EnableWindow(FALSE);
			}

			// コマンド変調度
			if (stPrmflg.CmdMod)
			{
				for (auto itri = st.CmdMod.begin(); itri != st.CmdMod.end(); ++itri)
				{
					CString item = (*itri);
					m_CmdMdlCmb.InsertString(-1, item);
				}
				m_CmdMdlCmb.SetCurSel(0);
				m_CmdMdlCmb.EnableWindow(TRUE);
			}
			else
			{
				m_CmdMdlCmb.EnableWindow(FALSE);
			}

			// コマンドサブキャリア周波数
			if (stPrmflg.CmdSubCarrFreq)
			{
				for (auto itri = st.CmdSubCarrFreq.begin(); itri != st.CmdSubCarrFreq.end(); ++itri)
				{
					CString item = (*itri);
					m_CmdFreqCmb.InsertString(-1, item);
				}
				m_CmdFreqCmb.SetCurSel(0);
				m_CmdFreqCmb.EnableWindow(TRUE);
			}
			else
			{
				m_CmdFreqCmb.EnableWindow(FALSE);
			}

			// 送信ドップラ
			if (stPrmflg.TransDoppler)
			{
				for (auto itri = st.TransDoppler.begin(); itri != st.TransDoppler.end(); ++itri)
				{
					CString item = (*itri);
					m_TDoppCmb.InsertString(-1, item);
				}
				m_TDoppCmb.SetCurSel(0);
				m_TDoppCmb.EnableWindow(TRUE);
			}
			else
			{
				m_TDoppCmb.EnableWindow(FALSE);
			}

			// アンテナ自動追尾
			if (stPrmflg.AntAutoTrack)
			{
				for (auto itri = st.AntAutoTrack.begin(); itri != st.AntAutoTrack.end(); ++itri)
				{
					CString item = (*itri);
					m_AntAutoCmb.InsertString(-1, item);
				}
				m_AntAutoCmb.SetCurSel(0);
				m_AntAutoCmb.EnableWindow(TRUE);
			}
			else
			{
				m_AntAutoCmb.EnableWindow(FALSE);
			}

			// アンテナ待ち受け指向
			if (stPrmflg.AntAosPosn)
			{
				for (auto itri = st.AntAosPosn.begin(); itri != st.AntAosPosn.end(); ++itri)
				{
					CString item = (*itri);
					m_AntStandbyCmb.InsertString(-1, item);
				}
				m_AntStandbyCmb.SetCurSel(0);
				m_AntStandbyCmb.EnableWindow(TRUE);
			}
			else
			{
				m_AntStandbyCmb.EnableWindow(FALSE);
			}

			// アンテナ待機位置指向
			if (stPrmflg.AntWaitPoint)
			{
				for (auto itri = st.AntWaitPoint.begin(); itri != st.AntWaitPoint.end(); ++itri)
				{
					CString item = (*itri);
					m_AntStandbyPosCmb.InsertString(-1, item);
				}
				m_AntStandbyPosCmb.SetCurSel(0);
				m_AntStandbyPosCmb.EnableWindow(TRUE);
			}
			else
			{
				m_AntStandbyPosCmb.EnableWindow(FALSE);
			}
		}
	}

	CString strCid;
	m_CidCmb.GetWindowTextW(strCid);

	// 
	SetComboboxCIDParam(strSatName, strCid);
	return TRUE;
}

/*============================================================================*/
/*! CCidSelect

-# CID毎のパラメータを設定

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CCidSelect::SetComboboxCIDParam(CString sat, CString cid)
{
	map<CString, map<CString, stCIDParam>> &cidlist = m_CIDSlctData.GetCidParamList();
	stCIDParam stCid = cidlist[sat][cid];

	m_TlmBrSCmb.SelectStringEx(-1, stCid.TlmBitRateS);
	m_TlmBrXCmb.SelectStringEx(-1, stCid.TlmBitRateX);
	m_TlmBrHXCmb.SelectStringEx(-1, stCid.TlmBitRateHX);
	m_TlmBrKaCmb.SelectStringEx(-1, stCid.TlmBitRateKa);
	m_CmdBrSCmb.SelectStringEx(-1, stCid.CmdBitRateS);
	m_CmdBrXCmb.SelectStringEx(-1, stCid.CmdBitRateX);
	m_TPwrCmb.SelectStringEx(-1, stCid.TrsPower);
	m_CarrTLbSCmb.SelectStringEx(-1, stCid.CarrLoopBandS);
	m_CarrTLbXCmb.SelectStringEx(-1, stCid.CarrLoopBandX);
	m_CarrTLbKaCmb.SelectStringEx(-1, stCid.CarrLoopBandKa);
	m_UpLinkShapeCmb.SelectStringEx(-1, stCid.UpLinkSweepShape);
	m_RngPrmModeCmb.SelectStringEx(-1, stCid.RngMesMode);
	if (m_xtx == XTX_Y_ID)
	{
		m_MdlXCmb.SelectStringEx(-1, stCid.Modulation);
	}
	else
	{
		m_MdlSCmb.SelectStringEx(-1, stCid.Modulation);
	}
	m_CmdMdlCmb.SelectStringEx(-1, stCid.CmdMod);
	m_CmdFreqCmb.SelectStringEx(-1, stCid.CmdSubCarrFreq);
	m_TDoppCmb.SelectStringEx(-1, stCid.TransDoppler);
	m_AntAutoCmb.SelectStringEx(-1, stCid.AntAutoTrack);
	m_AntStandbyCmb.SelectStringEx(-1, stCid.AntAosPosn);
	m_AntStandbyPosCmb.SelectStringEx(-1, stCid.AntWaitPoint);

	return TRUE;
}


/*============================================================================*/
/*! CCidSelect

-# 『衛星選択』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselSatelliteCmb()
{
	CString str;

	int idx = m_SetalliteCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_SetalliteCmb.GetLBText(idx, str);

	SetComboboxSatCIDParam(str);
}

/*============================================================================*/
/*! CCidSelect

-# 『CID』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselCidCmb()
{
	CString cid;

	int idx = m_CidCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_CidCmb.GetLBText(idx, cid);

	if (cid == CID_INVALID)
	{
		return;
	}
	else
	{
		int invalid = m_CidCmb.SelectStringEx(-1, CID_INVALID);
		m_CidCmb.DeleteString(invalid);
		m_CidCmb.SetCurSel(idx);
	}

	CString sat;
	m_SetalliteCmb.GetWindowTextW(sat);

	SetComboboxCIDParam(sat, cid);
}


/*============================================================================*/
/*! CCidSelect

-# 変更されたパラメータがCID登録されているか確認

@param  なし
@retval 該当CID番号
*/
/*============================================================================*/
CString CCidSelect::CheckCid()
{
	UpdateData();

	// CID取得
	map<CString, map<CString, stCIDParam>> &cidAllList = m_CIDSlctData.GetCidParamList();
	map<CString, stCIDParam> &cidList = cidAllList[m_strSatelliteCmb];

	// CIDパラメータ有効フラグ取得
	map<CString, stCIDPrmFlg> &cidPrmFlg = m_CIDSlctData.GetCidPrmFlgList();
	stCIDPrmFlg stPrmFlg = cidPrmFlg[m_strSatelliteCmb];

	CString cid = _T("");
	for (auto itr = cidList.begin(); itr != cidList.end(); ++itr)
	{
		cid = itr->first;
		stCIDParam &st = itr->second;

		if (m_xtx == XTX_Y_ID)
		{
			if ((m_strTlmBRSCmb.IsEmpty() == true || st.TlmBitRateS == m_strTlmBRSCmb)					// テレメトリビットレート S帯
				&& (m_strTlmBRXCmb.IsEmpty() == true || st.TlmBitRateX == m_strTlmBRXCmb)				// テレメトリビットレート X帯
				&& (m_strTlmBRHXCmb.IsEmpty() == true || st.TlmBitRateHX == m_strTlmBRHXCmb)			// テレメトリビットレート X高速
				&& (m_strTlmBRKaCmb.IsEmpty() == true || st.TlmBitRateKa == m_strTlmBRKaCmb)			// テレメトリビットレート Ka高速
				&& (m_strCmdBRSCmb.IsEmpty() == true || st.CmdBitRateS == m_strCmdBRSCmb)				// コマンドビットレート S帯
				&& (m_strCmdBRXCmb.IsEmpty() == true || st.CmdBitRateX == m_strCmdBRXCmb)				// コマンドビットレート X帯
				&& (m_strTPwrCmb.IsEmpty() == true || st.TrsPower == m_strTPwrCmb)						// 送信電力
				&& (m_strCarrTLbSCmb.IsEmpty() == true || st.CarrLoopBandS == m_strCarrTLbSCmb)			// キャリア受信ループバンド S帯
				&& (m_strCarrTLbXCmb.IsEmpty() == true || st.CarrLoopBandX == m_strCarrTLbXCmb)			// キャリア受信ループバンド X帯
				&& (m_strCarrTLbKaCmb.IsEmpty() == true || st.CarrLoopBandKa == m_strCarrTLbKaCmb)		// キャリア受信ループバンド Ka帯
				&& (m_strUpLinkShapeCmb.IsEmpty() == true || st.UpLinkSweepShape == m_strUpLinkShapeCmb)	// アップリンク掃引パラメータ 形状
				&& (m_strRngPrmModeCmb.IsEmpty() == true || st.RngMesMode == m_strRngPrmModeCmb)		// レンジ計測パラメータ モード
				&& (m_strMdlXCmb.IsEmpty() == true || st.Modulation == m_strMdlXCmb)					// 変調方式 X帯
				&& (m_strCmdMdlCmb.IsEmpty() == true || st.CmdMod == m_strCmdMdlCmb)					// コマンド変調度
				&& (m_strCmdFreqCmb.IsEmpty() == true || st.CmdSubCarrFreq == m_strCmdFreqCmb)			// コマンドサブキャリア周波数
				&& (m_strTDoppCmb.IsEmpty() == true || st.TransDoppler == m_strTDoppCmb)				// 送信ドップラ
				&& (m_strAntAutoCmb.IsEmpty() == true || st.AntAutoTrack == m_strAntAutoCmb)			// アンテナ自動追尾
				&& (m_strAntStandbyCmb.IsEmpty() == true || st.AntAosPosn == m_strAntStandbyCmb)		// アンテナ待ち受け指向
				&& (m_strAntStandbyPosCmb.IsEmpty() == true || st.AntWaitPoint == m_strAntStandbyPosCmb)	// アンテナ待機位置指向
				)
			{
				return cid;
			}
		}
		else
		{
			if ((m_strTlmBRSCmb.IsEmpty() == true || st.TlmBitRateS == m_strTlmBRSCmb)					// テレメトリビットレート S帯
				&& (m_strTlmBRXCmb.IsEmpty() == true || st.TlmBitRateX == m_strTlmBRXCmb)				// テレメトリビットレート X帯
				&& (m_strTlmBRHXCmb.IsEmpty() == true || st.TlmBitRateHX == m_strTlmBRHXCmb)			// テレメトリビットレート X高速
				&& (m_strTlmBRKaCmb.IsEmpty() == true || st.TlmBitRateKa == m_strTlmBRKaCmb)			// テレメトリビットレート Ka高速
				&& (m_strCmdBRSCmb.IsEmpty() == true || st.CmdBitRateS == m_strCmdBRSCmb)				// コマンドビットレート S帯
				&& (m_strCmdBRXCmb.IsEmpty() == true || st.CmdBitRateX == m_strCmdBRXCmb)				// コマンドビットレート X帯
				&& (m_strTPwrCmb.IsEmpty() == true || st.TrsPower == m_strTPwrCmb)						// 送信電力
				&& (m_strCarrTLbSCmb.IsEmpty() == true || st.CarrLoopBandS == m_strCarrTLbSCmb)			// キャリア受信ループバンド S帯
				&& (m_strCarrTLbXCmb.IsEmpty() == true || st.CarrLoopBandX == m_strCarrTLbXCmb)			// キャリア受信ループバンド X帯
				&& (m_strCarrTLbKaCmb.IsEmpty() == true || st.CarrLoopBandKa == m_strCarrTLbKaCmb)		// キャリア受信ループバンド Ka帯
				&& (m_strUpLinkShapeCmb.IsEmpty() == true || st.UpLinkSweepShape == m_strUpLinkShapeCmb)	// アップリンク掃引パラメータ 形状
				&& (m_strRngPrmModeCmb.IsEmpty() == true || st.RngMesMode == m_strRngPrmModeCmb)		// レンジ計測パラメータ モード
				&& (m_strMdlSCmb.IsEmpty() == true || st.Modulation == m_strMdlSCmb)					// 変調方式 S帯
				&& (m_strCmdMdlCmb.IsEmpty() == true || st.CmdMod == m_strCmdMdlCmb)					// コマンド変調度
				&& (m_strCmdFreqCmb.IsEmpty() == true || st.CmdSubCarrFreq == m_strCmdFreqCmb)			// コマンドサブキャリア周波数
				&& (m_strTDoppCmb.IsEmpty() == true || st.TransDoppler == m_strTDoppCmb)				// 送信ドップラ
				&& (m_strAntAutoCmb.IsEmpty() == true || st.AntAutoTrack == m_strAntAutoCmb)			// アンテナ自動追尾
				&& (m_strAntStandbyCmb.IsEmpty() == true || st.AntAosPosn == m_strAntStandbyCmb)		// アンテナ待ち受け指向
				&& (m_strAntStandbyPosCmb.IsEmpty() == true || st.AntWaitPoint == m_strAntStandbyPosCmb)	// アンテナ待機位置指向
				)
			{
				return cid;
			}
		}
	}

	return CID_INVALID;
}

/*============================================================================*/
/*! CCidSelect

-# 『テレメトリビットレート S帯』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselTlmbrSCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_TlmBrSCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_TlmBrSCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strTlmBRSCmb)
	{
//		return;
	}

	UpdateData();

#ifdef CIDSEL_GREAT
	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
#else
	CString newCid = _T("");
	BOOL bFind = FALSE;
	map<CString, map<CString, stCIDParam>> &cidlist = m_CIDSlctData.GetCidParamList();
	for (auto itr = cidlist.begin(); itr != cidlist.end(); ++itr)
	{
		CString sat = itr->first;

		if (sat == m_strSatelliteCmb)
		{
			CString cid = _T("");
			map<CString, stCIDParam> &cidplist = itr->second;
			for (auto itrp = cidplist.begin(); itrp != cidplist.end(); ++itrp)
			{
				cid = itrp->first;
				stCIDParam &st = itrp->second;

				if (st.TlmBitRateS == str)
				{
					bFind = TRUE;
					break;
				}
			}

			if (bFind)
			{
				newCid = cid;
				break;
			}
		}
	}

	if (bFind)
	{
		m_CidCmb.SelectStringEx(-1, newCid);
		SetComboboxCIDParam(m_strSatelliteCmb, newCid);
	}
#endif /* CIDSEL_GREAT */
}


/*============================================================================*/
/*! CCidSelect

-# 『テレメトリビットレート X帯』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselTlmbrXCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_TlmBrXCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_TlmBrXCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strTlmBRXCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# 『テレメトリビットレート X高速』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselTlmbrHxCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_TlmBrHXCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_TlmBrHXCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strTlmBRHXCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# 『テレメトリビットレート Ka帯』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselTlmbrKaCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_TlmBrKaCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_TlmBrKaCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strTlmBRKaCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# 『コマンドビットレート S帯』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselCmdbrSCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_CmdBrSCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_CmdBrSCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strCmdBRSCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# 『コマンドビットレート X帯』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselCmdbrXCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_CmdBrXCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_CmdBrXCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strCmdBRXCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# 『送信電力』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselTpwrPwrCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_TPwrCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_TPwrCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strTPwrCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# 『キャリア受信ループバンド S帯』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselClbSCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_CarrTLbSCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_CarrTLbSCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strCarrTLbSCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# 『キャリア受信ループバンド X帯』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselClbXCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_CarrTLbXCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_CarrTLbXCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strCarrTLbXCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# 『キャリア受信ループバンド Ka帯』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselClbKaCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_CarrTLbKaCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_CarrTLbKaCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strCarrTLbKaCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# 『アップリンク掃引パラメータ 形状』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselUplnkShpCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_UpLinkShapeCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_UpLinkShapeCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strUpLinkShapeCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# 『レンジ計測パラメータ サンプリングレート』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselRprmSrCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_RngPrmModeCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_RngPrmModeCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strRngPrmModeCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# 『コマンド変調度』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselCmdmdlMdlCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_CmdMdlCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_CmdMdlCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strCmdMdlCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# 『コマンドサブキャリア周波数』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselCmdsubfreqFreqCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_CmdFreqCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_CmdFreqCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strCmdFreqCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# 『変調方式 S帯』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselMmtdSCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_MdlSCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_MdlSCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strMdlSCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# 『変調方式 X帯』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselMmtdXCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_MdlXCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_MdlXCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strMdlXCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# 『送信ドップラ』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselTdopCmpCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_TDoppCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_TDoppCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strTDoppCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# 『アンテナ自動追尾』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselAntatMigCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_AntAutoCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_AntAutoCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strAntAutoCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# 『アンテナ待ち受け指向』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselAntstdorientedCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_AntStandbyCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_AntStandbyCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strAntAutoCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# 『アンテナ待機位置指向』コンボ 変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselAntstdposorientedCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_AntStandbyPosCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_AntStandbyPosCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strAntStandbyPosCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# CID パラメータ構造体を返す

@param  なし
@retval CID パラメータ構造体
*/
/*============================================================================*/
stCIDParam CCidSelect::GetParameter()
{
	stCIDParam st;
	st.CID = m_strCIDCmb;						// CID
	st.TlmBitRateS = m_strTlmBRSCmb;			// テレメトリビットレート S帯
	st.TlmBitRateX = m_strTlmBRXCmb;			// テレメトリビットレート X帯
	st.TlmBitRateHX = m_strTlmBRHXCmb;			// テレメトリビットレート X高速
	st.TlmBitRateKa = m_strTlmBRKaCmb;			// テレメトリビットレート Ka帯
	st.CmdBitRateS = m_strCmdBRSCmb;			// コマンドビットレート S帯
	st.CmdBitRateX = m_strCmdBRXCmb;			// コマンドビットレート X帯
	st.TrsPower = m_strTPwrCmb;					// 送信電力
	st.CarrLoopBandS = m_strCarrTLbSCmb;		// キャリア受信ループバンド S帯
	st.CarrLoopBandX = m_strCarrTLbXCmb;		// キャリア受信ループバンド X帯
	st.CarrLoopBandKa = m_strCarrTLbKaCmb;		// キャリア受信ループバンド Ka帯
	st.UpLinkSweepShape = m_strUpLinkShapeCmb;	// アップリンク掃引パラメータ 形状
	st.RngMesMode = m_strRngPrmModeCmb;			// レンジ計測パラメータ モード
	st.ModulationS = m_strMdlSCmb;				// 変調方式 S帯
	st.ModulationX = m_strMdlXCmb;				// 変調方式 X帯
	st.CmdMod = m_strCmdMdlCmb;					// コマンド変調度
	st.CmdSubCarrFreq = m_strCmdFreqCmb;		// コマンドサブキャリア周波数
	st.TransDoppler = m_strTDoppCmb;			// 送信ドップラ
	st.AntAutoTrack = m_strAntAutoCmb;			// アンテナ自動追尾
	st.AntAosPosn = m_strAntStandbyCmb;			// アンテナ待ち受け指向
	st.AntWaitPoint = m_strAntStandbyPosCmb;	// アンテナ待機位置指向

	return st;
}


///*============================================================================*/
///*! CCidSelect
//
//-# 衛星固有DBよりデータ取得
//
//@param  衛星名
//@retval なし
//*/
///*============================================================================*/
//void CCidSelect::GetEqSatDB(CString satname)
//{
//	struct satinf_t *satinf_adr;
//	struct satinf_t *original_adr;
//	char *sattbl_adr;
//
//	struct xtx_t		*xtx_t_adr;		/* Ｘ帯送信設備使用有無情報テーブルアドレス */
//
//	if (satname == _T(""))
//	{
//		return;
//	}
//
//	// DBアクセスクラス
//	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
//
//	vector<CString>			&saNameList = dba.GetSatNameList();
//	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();
//
//	satinf_adr = (struct satinf_t *)((__int64*)satAdrList[satname]);
//	original_adr = satinf_adr;
//	sattbl_adr = (char *)satinf_adr;
//
//	/*--------------------------------------*/
//	/* Ｘ帯送信設備使用有無情報取得 */
//	/*--------------------------------------*/
//	while (1)
//	{
//		satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
//		if (satinf_adr->l_id == ENDOFSAT_ID)
//		{
//			/*識別ＩＤが終了の場合*/
//			break;
//		}
//		if (satinf_adr->l_id == XTX_ID)
//		{
//			/*識別ＩＤが「Ｘ帯送信設備使用有無情報」の場合*/
//			xtx_t_adr = (struct xtx_t *)satinf_adr;
//			m_xtx = xtx_t_adr->l_x_tx;
//			break;
//		}
//		sattbl_adr += satinf_adr->l_len;
//	}
//
//}

/*============================================================================*/
/*! CCidSelect

-# 『決定』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CCidSelect::OnBnClickedOk()
{
	UpdateData();

	if (m_strCIDCmb == CID_INVALID)
	{
		CString title;
		GetWindowText(title);
		MessageBox(_T("選択されているCIDが不正です。"), title, MB_OK | MB_ICONWARNING);
		return;
	}

	OnOK();
}

/*============================================================================*/
/*! CSatelliteData

-# CIDファイル読込

@param  sz_watch[]		監視項目名称 [IN]
BOOL ReadCIDFile();
@retval -1：異常終了/ 0：正常終了
*/
/*============================================================================*/
BOOL CCidSelect::ReadCIDFile()
{
	const CString mEmptyString = CID_INVALID;

	FILE *fp;
	TCHAR sz_buff[500];

	CString strFilePath = theApp.GetShareFilePath(eFileType_CID_Folder, theApp.GetSelectStation());
	CString tempPath = strFilePath + _T("*");

	// 対象フォルダ内のファイル情報を取得する
	// 指定されたファイル名に一致するファイルを、ディレクトリ内で検索する
	WIN32_FIND_DATA ffdata;
	HANDLE hFindFile = FindFirstFile(tempPath, &ffdata);
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	UINT item = 0;
	do
	{
		TRY
		{
			CString fname;
			if (ffdata.cFileName[0] == '\0')
			{
				AfxThrowUserException();
			}
			if (ffdata.cFileName[0] == '.')
			{
				AfxThrowUserException();
			}
			if (ffdata.cFileName[0] == '[')
			{
				AfxThrowUserException();
			}

			// ディレクトリ名取得
			if (ffdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				CString file = strFilePath + _T("\\") + CString(ffdata.cFileName);
				m_CIDSlctData.GetSatelliteNameList().push_back(ffdata.cFileName);
			}
		}
		CATCH(CUserException, e)
		{
		}
		AND_CATCH(CException, e)
		{
		}
		END_CATCH
	} while (FindNextFile(hFindFile, &ffdata)); // ファイルがなくなるまで続ける


	map<CString, stCIDPrmEle> &cidEleList = m_CIDSlctData.GetCidElementList();
	cidEleList.clear();

	map<CString, map<CString, stCIDParam>> &cidlist = m_CIDSlctData.GetCidParamList();
	cidlist.clear();

	map<CString, stCIDPrmFlg> &cidPrmFlglist = m_CIDSlctData.GetCidPrmFlgList();
	cidPrmFlglist.clear();


	bool bRead = false;

	// 衛星毎のCIDファイル読込
	vector<CString>& slist = m_CIDSlctData.GetSatelliteNameList();
	for (auto itr = slist.begin(); itr != slist.end(); ++itr)
	{
		CString sat = (CString)(*itr);	// 衛星名
		//CString file = strFilePath + sat + _T("\\") + CID_FILE;
		CString file = strFilePath + sat + _T("\\") + sat + CID_EXT;

		if ((_wfopen_s(&fp, file, _T("r")) != 0) || (fp == NULL))
		{
			// 次のフォルダを調べる
			continue;
		}
//#if 0	// ヘッダー行が有る場合は、有効にすべし
//		// ヘッダー部読み飛ばし
//		fgetws(sz_buff, static_cast<unsigned>(_countof(sz_buff)), fp);
//#endif
		stCIDPrmEle	stele;
		stCIDParam	stcid;
		stCIDPrmFlg	stpflg;
		stCIDPrmFlg	stpflgSat;
		memset(&stpflg, 0, sizeof(stpflg));
		memset(&stpflgSat, 0, sizeof(stpflgSat));

		char sz_line[512];

		while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp) != NULL)
		{
			bRead = true;
//#if 0
//			CString line(sz_line);
//#else
			wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
			CString line(sz_buff);
//#endif
			BOOL isLineRead = TRUE;
			int	 iStart = 0;
			int  iFind = 0;

			int iCount = 0;
			int prmIdx = 0;

			while (isLineRead)
			{
				iFind = line.Find(_T(","), iStart);
				CString item = _T("");

				if (iFind == -1)
				{
					iFind = line.Find(_T("\n"), iStart);
					isLineRead = FALSE;
				}

				item = line.Mid(iStart, iFind - iStart);

				switch (iCount)
				{
				case 0:		// CID番号
					stcid.CID = item;
					AddDataVector(stele.CID, item);
					break;

				case 1:		// パラメータ番号
					prmIdx = _wtoi(item);
					break;

				case 2:		// パラメータ名
					break;

				case 3:		// パラメータ有効フラグ
				{
					int nflg = _wtoi(item);
					switch (prmIdx)
					{
					case 1:	// S帯テレメトリビットレート
						stpflg.TlmBitRateS = nflg;
						break;
					case 2:	// X帯テレメトリビットレート
						stpflg.TlmBitRateX = nflg;
						break;
					case 3:	// X高速テレメトリビットレート
						stpflg.TlmBitRateHX = nflg;
						break;
					case 4:	// Ka帯テレメトリビットレート 
						stpflg.TlmBitRateKa = nflg;
						break;
					case 5:	// S帯コマンドビットレート
						stpflg.CmdBitRateS = nflg;
						break;
					case 6:	// X帯コマンドビットレート
						stpflg.CmdBitRateX = nflg;
						break;
					case 7:	// 送信電力
						stpflg.TrsPower = nflg;
						break;
					case 8:	// アップリンク掃引パラメータ
						stpflg.UpLinkSweepShape = nflg;
						break;
					case 9:	// レンジ計測パラメータ モード
						stpflg.RngMesMode = nflg;
						break;
					case 10:// S帯キャリア受信ループバンド
						stpflg.CarrLoopBandS = nflg;
						break;
					case 11:// X帯キャリア受信ループバンド
						stpflg.CarrLoopBandX = nflg;
						break;
					case 12:// Ka帯キャリア受信ループバンド
						stpflg.CarrLoopBandKa = nflg;
						break;
					case 13:// コマンド変調度
						stpflg.CmdMod = nflg;
						break;
					case 14:// コマンドサブキャリア周波数
						stpflg.CmdSubCarrFreq = nflg;
						break;
					case 15:// 送信ドップラ補償の有無
						stpflg.TransDoppler = nflg;
						break;
					case 16:// アンテナ自動追尾
						stpflg.AntAutoTrack = nflg;
						break;
					case 17:// 変調方式
						stpflg.Modulation = nflg;
						break;
					case 18:// アンテナ待ち受け指向の有無
						stpflg.AntAosPosn = nflg;
						break;
					case 19:// アンテナ待機位置指向の有無
						stpflg.AntWaitPoint = nflg;

//						cidPrmFlglist[sat.MakeUpper()] = stpflg;
						// 衛星別のパラメータ活性非活性情報を付加していく
						stpflgSat.TlmBitRateS |= stpflg.TlmBitRateS;				// テレメトリビットレート S帯
						stpflgSat.TlmBitRateX |= stpflg.TlmBitRateX;				// テレメトリビットレート X帯
						stpflgSat.TlmBitRateHX |= stpflg.TlmBitRateHX;				// テレメトリビットレート X高速
						stpflgSat.TlmBitRateKa |= stpflg.TlmBitRateKa;				// テレメトリビットレート Ka帯
						stpflgSat.CmdBitRateS |= stpflg.CmdBitRateS;				// コマンドビットレート S帯
						stpflgSat.CmdBitRateX |= stpflg.CmdBitRateX;				// コマンドビットレート X帯
						stpflgSat.TrsPower |= stpflg.TrsPower;						// 送信電力
						stpflgSat.CarrLoopBandS |= stpflg.CarrLoopBandS;			// キャリア受信ループバンド S帯
						stpflgSat.CarrLoopBandX |= stpflg.CarrLoopBandX;			// キャリア受信ループバンド X帯
						stpflgSat.CarrLoopBandKa |= stpflg.CarrLoopBandKa;			// キャリア受信ループバンド Ka帯
						stpflgSat.UpLinkSweepShape |= stpflg.UpLinkSweepShape;		// アップリンク掃引パラメータ 形状
						stpflgSat.UpLinkSweepWidth |= stpflg.UpLinkSweepWidth;		// アップリンク掃引パラメータ 幅
						stpflgSat.UpLinkSweepTime |= stpflg.UpLinkSweepTime;		// アップリンク掃引パラメータ 時間
						stpflgSat.RngMesMode |= stpflg.RngMesMode;					// レンジ計測パラメータ モード
						stpflgSat.Modulation |= stpflg.Modulation;					// 変調方式 S帯
						stpflgSat.ModulationS |= stpflg.ModulationS;				// 変調方式 S帯
						stpflgSat.ModulationX |= stpflg.ModulationX;				// 変調方式 X帯
						stpflgSat.CmdMod |= stpflg.CmdMod;							// コマンド変調度
						stpflgSat.CmdSubCarrFreq |= stpflg.CmdSubCarrFreq;			// コマンドサブキャリア周波数
						stpflgSat.TransDoppler |= stpflg.TransDoppler;				// 送信ドップラ
						stpflgSat.AntAutoTrack |= stpflg.AntAutoTrack;				// アンテナ自動追尾
						stpflgSat.AntAosPosn |= stpflg.AntAosPosn;					// アンテナ待ち受け指向
						stpflgSat.AntWaitPoint |= stpflg.AntWaitPoint;				// アンテナ待機位置指向
						break;
					}
				}
				break;

				case 4:		// パラメータ値
				{
					switch (prmIdx)
					{
					case 1:	// S帯テレメトリビットレート
						if (stpflg.TlmBitRateS == 0)
						{
							item = mEmptyString;
						}
						stcid.TlmBitRateS = item;
						AddDataVector(stele.TlmBitRateS, item);
						break;
					case 2:	// X帯テレメトリビットレート
						if (stpflg.TlmBitRateX == 0)
						{
							item = mEmptyString;
						}
						stcid.TlmBitRateX = item;
						AddDataVector(stele.TlmBitRateX, item);
						break;
					case 3:	// X高速テレメトリビットレート
						if (stpflg.TlmBitRateHX == 0)
						{
							item = mEmptyString;
						}
						stcid.TlmBitRateHX = item;
						AddDataVector(stele.TlmBitRateHX, item);
						break;
					case 4:	// Ka帯テレメトリビットレート 
						if (stpflg.TlmBitRateKa == 0)
						{
							item = mEmptyString;
						}
						stcid.TlmBitRateKa = item;
						AddDataVector(stele.TlmBitRateKa, item);
						break;
					case 5:	// S帯コマンドビットレート
						if (stpflg.CmdBitRateS == 0)
						{
							item = mEmptyString;
						}
						stcid.CmdBitRateS = item;
						AddDataVector(stele.CmdBitRateS, item);
						break;
					case 6:	// X帯コマンドビットレート
						if (stpflg.CmdBitRateX == 0)
						{
							item = mEmptyString;
						}
						stcid.CmdBitRateX = item;
						AddDataVector(stele.CmdBitRateX, item);
						break;
					case 7:	// 送信電力
						if (stpflg.TrsPower == 0)
						{
							item = mEmptyString;
						}
						stcid.TrsPower = item;
						AddDataVector(stele.TrsPower, item);
						break;
					case 8:	// アップリンク掃引パラメータ
						if (stpflg.UpLinkSweepShape == 0)
						{
							item = mEmptyString;
						}
						stcid.UpLinkSweepShape = item;
						AddDataVector(stele.UpLinkSweepShape, item);
						break;
					case 9:	// レンジ計測パラメータ モード
						if (stpflg.RngMesMode == 0)
						{
							item = mEmptyString;
						}
						stcid.RngMesMode = item;
						AddDataVector(stele.RngMesMode, item);
						break;
					case 10:// S帯キャリア受信ループバンド
						if (stpflg.CarrLoopBandS == 0)
						{
							item = mEmptyString;
						}
						stcid.CarrLoopBandS = item;
						AddDataVector(stele.CarrLoopBandS, item);
						break;
					case 11:// X帯キャリア受信ループバンド
						if (stpflg.CarrLoopBandX == 0)
						{
							item = mEmptyString;
						}
						stcid.CarrLoopBandX = item;
						AddDataVector(stele.CarrLoopBandX, item);
						break;
					case 12:// Ka帯キャリア受信ループバンド
						if (stpflg.CarrLoopBandKa == 0)
						{
							item = mEmptyString;
						}
						stcid.CarrLoopBandKa = item;
						AddDataVector(stele.CarrLoopBandKa, item);
						break;
					case 13:// コマンド変調度
						if (stpflg.CmdMod == 0)
						{
							item = mEmptyString;
						}
						stcid.CmdMod = item;
						AddDataVector(stele.CmdMod, item);
						break;
					case 14:// コマンドサブキャリア周波数
						if (stpflg.CmdSubCarrFreq == 0)
						{
							item = mEmptyString;
						}
						stcid.CmdSubCarrFreq = item;
						AddDataVector(stele.CmdSubCarrFreq, item);
						break;
					case 15:// 送信ドップラ補償の有無
						if (stpflg.TransDoppler == 0)
						{
							item = mEmptyString;
						}
						stcid.TransDoppler = item;
						AddDataVector(stele.TransDoppler, item);
						break;
					case 16:// アンテナ自動追尾
						if (stpflg.AntAutoTrack == 0)
						{
							item = mEmptyString;
						}
						stcid.AntAutoTrack = item;
						AddDataVector(stele.AntAutoTrack, item);
						break;
					case 17:// 変調方式
						if (stpflg.Modulation == 0)
						{
							item = mEmptyString;
						}
						stcid.Modulation = item;
						AddDataVector(stele.Modulation, item);
						break;
					case 18:// アンテナ待ち受け指向の有無
						if (stpflg.AntAosPosn == 0)
						{
							item = mEmptyString;
						}
						stcid.AntAosPosn = item;
						AddDataVector(stele.AntAosPosn, item);
						break;
					case 19:// アンテナ待機位置指向の有無
						if (stpflg.AntWaitPoint == 0)
						{
							item = mEmptyString;
						}
						stcid.AntWaitPoint = item;
						AddDataVector(stele.AntWaitPoint, item);

						cidlist[sat.MakeUpper()][stcid.CID] = stcid;

						memset(&stpflg, 0, sizeof(stpflg));		// CIDが変わるたびにクリア
						break;
					}
				}
				break;
				}

				iStart = iFind + 1;
				iCount++;

			}

		}

		// 衛星別のパラメータ活性非活性登録
		cidPrmFlglist[sat.MakeUpper()] = stpflgSat;
		cidEleList[sat.MakeUpper()] = stele;
		fclose(fp);

#ifdef _DEBUG
		{
			CString strLog;

			strLog.Format(_T("%s, %s, CID(%s), 1:TlmBitRateS(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.TlmBitRateS);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 2:TlmBitRateX(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.TlmBitRateX);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 3:TlmBitRateHX(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.TlmBitRateHX);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 4:TlmBitRateKa(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.TlmBitRateKa);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 5:CmdBitRateS(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.CmdBitRateS);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 6:CmdBitRateX(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.CmdBitRateX);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 7:TrsPower(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.TrsPower);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 10:CarrLoopBandS(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.CarrLoopBandS);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 11:CarrLoopBandX(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.CarrLoopBandX);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 12:CarrLoopBandKa(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.CarrLoopBandKa);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 8:UpLinkSweepShape(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.UpLinkSweepShape);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), -:UpLinkSweepWidth(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.UpLinkSweepWidth);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), -:UpLinkSweepTime(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.UpLinkSweepTime);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 9:RngMesMode(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.RngMesMode);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 17:Modulation(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.Modulation);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), -:ModulationS(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.ModulationS);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), -:ModulationX(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.ModulationX);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 13:CmdMod(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.CmdMod);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 14:CmdSubCarrFreq(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.CmdSubCarrFreq);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 15:TransDoppler(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.TransDoppler);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 16:AntAutoTrack(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.AntAutoTrack);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 18:AntAosPosn(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.AntAosPosn);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);

			strLog.Format(_T("%s, %s, CID(%s), 19:AntWaitPoint(%d)"), mStationSimpleString[theApp.GetSelectStation()], (LPCTSTR)sat, (LPCTSTR)stcid.CID, stpflgSat.AntWaitPoint);
			CLogTraceEx::Write(_T(""), _T("CSatelliteData"), _T("ReadCIDFile"), _T("debug"), strLog, nLogEx::debug);
		}
#endif /* _DEBUG */
	}

	if (bRead == true)
		return TRUE;
	else
		return FALSE;
}

void CCidSelect::AddDataVector(vector<CString> &v, CString str)
{
	for (auto itr = v.begin(); itr != v.end(); ++itr)
	{
		if (str == *itr)
		{
			return;
		}
	}

	v.push_back(str);
}


};
