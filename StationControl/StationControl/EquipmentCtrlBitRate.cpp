/*============================================================================*/
/*! EquipmentCtrlBitRate.cpp

-# 設備制御-ビットレート変更画面
*/
/*============================================================================*/
// EquipmentCtrlBitRate.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "EquipmentCtrlBitRate.h"
#include "afxdialogex.h"


#define MAC_MAX     256

// CEquipmentCtrlBitRate ダイアログ

IMPLEMENT_DYNAMIC(CEquipmentCtrlBitRate, CDialogEx)

CEquipmentCtrlBitRate::CEquipmentCtrlBitRate(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEquipmentCtrlBitRate::IDD, pParent)
	, m_DataTimeDay(0)
	, m_DataTimeTime(0)
	, m_strSatCmb(_T(""))
	, m_strWindowTitle(_T(""))
{

}

CEquipmentCtrlBitRate::~CEquipmentCtrlBitRate()
{
}

void CEquipmentCtrlBitRate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLG_EQUIPCTRL_BITRATE_SAT_CMB, m_SatCmb);
	DDX_Control(pDX, IDC_DLG_EQUIPCTRL_BITRATE_BR_LIST, m_BitRateListBox);
	DDX_Control(pDX, IDC_DLG_EQUIPCTRL_BITRATE_DP_DATA, m_DataTimeDayCtrl);
	DDX_DateTimeCtrl(pDX, IDC_DLG_EQUIPCTRL_BITRATE_DP_DATA, m_DataTimeDay);
	DDX_DateTimeCtrl(pDX, IDC_DLG_EQUIPCTRL_BITRATE_DP_TIME, m_DataTimeTime);
	DDX_Control(pDX, IDC_DLG_EQUIPCTRL_BITRATE_DP_TIME, m_DateTimeTimeCtrl);
	DDX_Control(pDX, IDC_CHECK1, m_SpecifyDateChkBtn);
	DDX_CBString(pDX, IDC_DLG_EQUIPCTRL_BITRATE_SAT_CMB, m_strSatCmb);
}


BEGIN_MESSAGE_MAP(CEquipmentCtrlBitRate, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK1, &CEquipmentCtrlBitRate::OnBnClickedCheck1)
	ON_CBN_SELCHANGE(IDC_DLG_EQUIPCTRL_BITRATE_SAT_CMB, &CEquipmentCtrlBitRate::OnCbnSelchangeDlgEquipctrlBitrateSatCmb)
	ON_BN_CLICKED(IDOK, &CEquipmentCtrlBitRate::OnBnClickedOk)
END_MESSAGE_MAP()


// CEquipmentCtrlBitRate メッセージ ハンドラー


/*============================================================================*/
/*! CEquipmentCtrlBitRate

-# ダイアログ初期化処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CEquipmentCtrlBitRate::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (!m_strWindowTitle.IsEmpty())
		SetWindowText(m_strWindowTitle);

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス

	// 日付フォーマット変更
	m_DataTimeDayCtrl.SetFormat(_T("yyyy-MM-dd"));

	// 現在時刻取得
	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	CTime time = CTime(timeSys);

	// 日時指定コントロール初期化
	m_SpecifyDateChkBtn.SetCheck(0);
	m_DataTimeDayCtrl.EnableWindow(FALSE);
	m_DataTimeDay = time;
	m_DateTimeTimeCtrl.EnableWindow(FALSE);
	m_DataTimeTime = time;

	// ビットレートがコマンドの場合、『日時指定』は非活性
	if (m_bCmdBitRate)
	{
		m_SpecifyDateChkBtn.EnableWindow(FALSE);
	}

#if 0
	GetBitRateListSat();
#else
	// 衛星名コンボ設定
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	vector<CString>	 &saNameList = dba.GetSatNameList();
	for (int i = 0; i < saNameList.size(); ++i)
	{
		if (saNameList[i] != _T("DEFAULT"))
		{
			m_SatCmb.InsertString(-1, saNameList[i]);
		}
	}
#endif

	// 監視データベース上に衛星名があれば、その衛星名がデフォ設定。
	CString sat = _T("");
	UINT sta = theApp.GetSelectStation();
	obs_adr = (CEngValCnv::obsname_t *)theApp.GetSatelliteData().GetDBAccessCls().GetObsname_t(sta, CTRL_P_SAT);
	if (obs_adr != NULL)
	{
		sat = CString(obs_adr->sz_sts_name);

		int idx = m_SatCmb.FindStringExact(-1, sat);
		if (idx != CB_ERR)
		{
			m_SatCmb.SetCurSel(idx);
		}
		else
		{
			m_SatCmb.SetCurSel(0);
			m_SatCmb.GetLBText(0, sat);
		}
	}
	else
	{
		m_SatCmb.SetCurSel(0);
		m_SatCmb.GetLBText(0, sat);
	}

	// ビットレートリスト設定
	GetBitRateList(sat);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CEquipmentCtrlBitRate

-# 『日付指定』チェックボックス押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentCtrlBitRate::OnBnClickedCheck1()
{
	if (m_SpecifyDateChkBtn.GetCheck() == 0)
	{
		m_DataTimeDayCtrl.EnableWindow(FALSE);
		m_DateTimeTimeCtrl.EnableWindow(FALSE);
	}
	else
	{
		m_DataTimeDayCtrl.EnableWindow(TRUE);
		m_DateTimeTimeCtrl.EnableWindow(TRUE);
	}
}


/*============================================================================*/
/*! CEquipmentCtrlBitRate

-# 『衛星名』コンボボックス変更処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentCtrlBitRate::OnCbnSelchangeDlgEquipctrlBitrateSatCmb()
{
	CString str;

	// 変更後の文字列取得
	int idx = m_SatCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_SatCmb.GetLBText(idx, str);

	// 前回と同じなら何もしない
	if (str == m_strSatCmb) 
	{
		return;
	}

	GetBitRateList(str);
}


/*============================================================================*/
/*! CEquipmentCtrlBitRate

-# 『衛星名』毎のビットレートを取得

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentCtrlBitRate::GetBitRateList(CString satname)
{
#if 0
	// ビットレート取得
	m_BitRateListBox.ResetContent();
	vector<CString> &brList = m_SatBitRateMap[satname];
	for(int i=0; i<brList.size(); ++i)
	{
		m_BitRateListBox.InsertString(-1, brList[i]);
	}

#else
	struct satinf_t *satinf_adr;
	int i_init_ret;
	char    *psz_tbl[MAC_MAX];                 /* マクロ名を格納するテーブルの先頭アドレス*/
	char    *psz_vec[MAC_MAX];                 /* マクロ情報アドレスを格納するテーブルの  */

	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();

	vector<CString>			&saNameList = dba.GetSatNameList();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();

	// 衛星固有情報のアドレス取得
	satinf_adr = (struct satinf_t *)((__int64*)satAdrList[satname]);
	if (satinf_adr == NULL)
	{
		return;
	}

	// ビットレート取得
	m_BitRateListBox.ResetContent();
	if ((i_init_ret = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(m_strBitRateCtrl).GetBuffer(), psz_tbl, psz_vec, MAC_MAX)) > -1)
	{
		m_BitRateListBox.ResetContent();
		for (int i = 0; i < i_init_ret; ++i) 
		{
			CString strBitRate =  CString(psz_tbl[i]);
			m_BitRateListBox.InsertString(-1, strBitRate);
		}
	}
#endif
}

/*============================================================================*/
/*! CEquipmentCtrlBitRate

-# 『衛星名』毎のビットレートを取得

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentCtrlBitRate::GetBitRateListSat()
{
	struct satinf_t *satinf_adr;
	int i_init_ret;
	char    *psz_tbl[MAC_MAX];                 /* マクロ名を格納するテーブルの先頭アドレス*/
	char    *psz_vec[MAC_MAX];                 /* マクロ情報アドレスを格納するテーブルの  */

	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	vector<CString>	 &saNameList = dba.GetSatNameList();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();

	m_SatBitRateMap.clear();
	vector<CString> brList;

	for (int i = 0; i < saNameList.size(); ++i)
	{
		if (saNameList[i] != _T("DEFAULT"))
		{
			// 衛星固有情報のアドレス取得
			satinf_adr = (struct satinf_t *)((__int64*)satAdrList[saNameList[i]]);
			if (satinf_adr != NULL)
			{
				// ビットレート取得
				brList.clear();
				if ((i_init_ret = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(m_strBitRateCtrl).GetBuffer(), psz_tbl, psz_vec, MAC_MAX)) > -1)
				{
					for (int b = 0; b < i_init_ret; ++b)
					{
						CString strBitRate = CString(psz_tbl[b]);
						brList.push_back(strBitRate);
					}

					if (brList.size() > 0)
					{
						// 衛星選択コンボボックス設定
						m_SatCmb.InsertString(-1, saNameList[i]);

						// リストに追加
						m_SatBitRateMap[saNameList[i]] = brList;
					}
				}
			}
		}
	}
}


/*============================================================================*/
/*! CEquipmentCtrlBitRate

-# 『実行』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentCtrlBitRate::OnBnClickedOk()
{
	UpdateData();

	// 選択されているビットレート取得
	int idx = m_BitRateListBox.GetCurSel();
	if (idx == LB_ERR)
	{
		CString title;
		GetWindowText(title);
		MessageBox(_T("ビットレートを選択してください。"), title, MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	m_BitRateListBox.GetText(idx, m_stBitRate.bitrate);

	// 選択されている衛星名取得
	m_stBitRate.satellite = m_strSatCmb;

	// 日時指定取得
	m_stBitRate.bTime = m_SpecifyDateChkBtn.GetCheck();
	m_stBitRate.time = CTime(m_DataTimeDay.GetYear(), m_DataTimeDay.GetMonth(), m_DataTimeDay.GetDay(), m_DataTimeTime.GetHour(), m_DataTimeTime.GetMinute(), m_DataTimeTime.GetSecond());

	CDialogEx::OnOK();
}
