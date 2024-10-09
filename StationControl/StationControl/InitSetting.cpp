/*============================================================================*/
/*! InitSetting.cpp

-# 初期設定画面
*/
/*============================================================================*/
// InitSetting.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "InitSetting.h"
#include "afxdialogex.h"
#include "CidSelect.h"

#define CID_CTRL _T("INIT.CID")


// CInitSetting ダイアログ

IMPLEMENT_DYNAMIC(CInitSetting, CDialogBase)

/*============================================================================*/
/*! CInitSetting

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CInitSetting::CInitSetting(CWnd* pParent /*=NULL*/)
: CDialogBase(CInitSetting::IDD, pParent)
	, m_CidChkBox(FALSE)
{

}

/*============================================================================*/
/*! CInitSetting

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CInitSetting::~CInitSetting()
{
}

void CInitSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INITSETTING_COMBO1, m_CCBoxSatelliteName);
	DDX_Control(pDX, IDC_INITSETTING_CID_BUTTON, m_CButonSelCid);
	DDX_Control(pDX, IDC_INITSETTING_LIST1, m_listInitSetting);
	DDX_Control(pDX, IDC_INITSETING_CHECK, m_chkAllSelect);
	DDX_Check(pDX, IDC_CIDCHECK, m_CidChkBox);
}


BEGIN_MESSAGE_MAP(CInitSetting, CDialogBase)
	ON_BN_CLICKED(IDOK, &CInitSetting::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_INITSETTING_COMBO1, &CInitSetting::OnCbnSelchangeInitsettingCombo1)
	ON_BN_CLICKED(IDC_INITSETTING_CID_BUTTON, &CInitSetting::OnBnClickedInitsettingCidButton)
	ON_BN_CLICKED(IDC_INITSETING_CHECK, &CInitSetting::OnBnClickedInitsetingCheck)
	ON_BN_CLICKED(IDC_CIDCHECK, &CInitSetting::OnBnClickedCidcheck)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CInitSetting メッセージ ハンドラー


/*============================================================================*/
/*! CInitSetting

-# ダイアログ初期化処理

@param  なし
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
BOOL CInitSetting::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	struct satinf_t *satinf_adr;
	char			*sattbl_adr;
//	struct satinf_t *original_adr;
	struct mac_t	*mac_t_adr = NULL;

	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();

	vector<CString>			&saNameList = dba.GetSatNameList();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();

	m_listSatellite.clear();
	m_listInitMacr.clear();

	vector<CString> initList;
	int cnt = (int)saNameList.size();
	for (int i = 0; i < cnt; i++)
	{
		/*--------------------------*/
		/* 衛星本体情報アドレス取得 */
		CString satname = saNameList[i];

		if (satname == _T("DEFAULT") || satname == _T("ＤＥＦＡＵＬＴ"))
		{
			continue;
		}

		satinf_adr = (struct satinf_t *)((__int64*)satAdrList[satname]);
		sattbl_adr = (char *)satinf_adr;	/* char型に変換                   */

		m_listSatellite.push_back(satname);
		initList.clear();

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
		m_listInitMacr[satname] = initList;
	}

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CInitSetting

-# ダイアログ更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CInitSetting::UpdateDlg()
{
	InitDlg();
	return TRUE;
}


/*============================================================================*/
/*! CInitSetting

-# 初期設定マクロ定義リスト再設定

@param  なし
@retval なし
*/
/*============================================================================*/
void CInitSetting::RestListBox(CString sat)
{
	m_listInitSetting.SetRedraw(FALSE);
	m_listInitSetting.ResetContent();

	vector<CString> &list = m_listInitMacr[sat];
	for (int i = 0; i < (int)list.size(); ++i)
	{
		m_listInitSetting.InsertString(-1, list[i]);
	}
	m_listInitSetting.SetRedraw(TRUE);
}
	
/*============================================================================*/
/*! CInitSetting

-# 初期設定マクロ定義リスト再設定

@param  なし
@retval なし
*/
/*============================================================================*/
void CInitSetting::RestCIDBtn(CString sat)
{
	vector<CString> &list = theApp.GetSatelliteData().m_CIDSlctData.GetCidList(sat);

	if (list.size() > 0)
	{
#if 1
		stOpeProp stProp;
		theApp.GetSatelliteData().ReadPropertyFile(sat, stProp);
		CString strCid = CString(stProp.cid);
		auto itrl = std::find(list.begin(), list.end(), strCid);
		if (itrl == list.end())
		{
			// デフォルトCIDがCIDファイルに無い場合、一番最初のCIDをセット。
			strCid = list[0];
		}

		m_CButonSelCid.SetWindowTextW(strCid);
#else
		// 先頭のCIDをセット
		m_CButonSelCid.SetWindowTextW(list[0]);
#endif
		m_CButonSelCid.EnableWindow(TRUE);
	}
	else
	{
		// CIDファイルが無い
		m_CButonSelCid.SetWindowTextW(_T("N/A"));
		m_CButonSelCid.EnableWindow(FALSE);
	}
}

/*============================================================================*/
/*! CInitSetting

-# ダイアログ初期化処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CInitSetting::InitDlg()
{
	// 衛星コンボボックス設定
#if 1
	for (int i = 0; i < (int)m_listSatellite.size(); ++i)
	{
		m_CCBoxSatelliteName.InsertString(-1, m_listSatellite[i]);
	}
#else
	for (auto itr = m_listInitMacr.begin(); itr != m_listInitMacr.end(); ++itr)
	{
		CString sat = itr->first;
		m_CCBoxSatelliteName.InsertString(-1, sat);
	}
#endif
	m_CCBoxSatelliteName.SetCurSel(0);

	// 初期設定リストボックス設定
	CString str;
	m_CCBoxSatelliteName.GetLBText(0, str);
	RestListBox(str);

	// CIDチェックデフォルトOFF
	m_CidChkBox = FALSE;

	// CID更新
	RestCIDBtn(str);
	return TRUE;
}


/*============================================================================*/
/*! CInitSetting

-# 実行ボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CInitSetting::OnBnClickedOk()
{
	// 衛星名取得
	m_CCBoxSatelliteName.GetLBText(m_CCBoxSatelliteName.GetCurSel(), m_satSel);

	UpdateData();

	if (m_CidChkBox == TRUE)
	{
		/* INIT.CID制御 */

		// CID取得
		CString cid;
		m_CButonSelCid.GetWindowTextW(cid);
		CString ctrl = _T("");
		ctrl.Format(_T("%s %s,%s;"), CID_CTRL, (LPCTSTR)cid, (LPCTSTR)m_satSel);

		// 制御シェル呼出
		CWaitCursor wait;
		theApp.SendControlNoUI(ctrl, _T(""), _T(""), _T(""));
		wait.Restore();

		CDialogBase::OnOK();
	}
	else
	{
		/* INIT制御 */

		// 設備取得
		int nCount = m_listInitSetting.GetSelCount();
		if (nCount <= 0)
		{
			CString title;
			GetWindowText(title);
			MessageBox(_T("初期設定を行う設備が選択されていません。"), title, MB_OK | MB_ICONEXCLAMATION);
			return;
		}

		m_aryListBoxSel.RemoveAll();
		m_aryListBoxSel.SetSize(nCount);
		m_listInitSetting.GetSelItems(nCount, m_aryListBoxSel.GetData());

		m_aryListBoxPos = 0;

		// 実行、キャンセルを非活性化
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

		// タイマ作成
		SetTimer(1, 5, 0);
	}
}


/*============================================================================*/
/*! CInitSetting

-# コンボボックス選択変更時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CInitSetting::OnCbnSelchangeInitsettingCombo1()
{
	CString satname;
	int idx = m_CCBoxSatelliteName.GetCurSel();
	if (idx == CB_ERR)
	{
		return;
	}
	m_CCBoxSatelliteName.GetLBText(idx, satname);

	// リスト更新
	RestListBox(satname);

	// CID更新
	RestCIDBtn(satname);
}


/*============================================================================*/
/*! CInitSetting

-# CID選択ボタンクリック時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CInitSetting::OnBnClickedInitsettingCidButton()
{
	UpdateData();

	// 衛星名取得
	CString sat;
	m_CCBoxSatelliteName.GetLBText(m_CCBoxSatelliteName.GetCurSel(), sat);

	// CID取得
	CString cid;
	m_CButonSelCid.GetWindowTextW(cid);

	// CID選択画面表示
	CCidSelect dlg;
	dlg.SetSatelliteName(sat);
	dlg.SetCID(cid);
	if (dlg.DoModal() == IDOK)
	{
		stCIDParam st = dlg.GetParameter();
		m_CButonSelCid.SetWindowTextW(st.CID);
		UpdateData(FALSE);
	}
}


/*============================================================================*/
/*! CInitSetting

-# 設備全選択ボタンクリック時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CInitSetting::OnBnClickedInitsetingCheck()
{
	UpdateData(TRUE);

	if (m_chkAllSelect.GetCheck() == BST_CHECKED)
	{
		CString str;
		if (str.LoadStringW(IDS_INITSETTING_UNSELECTALL) == FALSE)
			return;
		m_chkAllSelect.SetWindowTextW(str);
		m_listInitSetting.SetSel(-1, TRUE);
	}
	else
	{
		CString str;
		if (str.LoadStringW(IDS_INITSETTING_SELECTALL) == FALSE)
			return;
		m_chkAllSelect.SetWindowTextW(str);
		m_listInitSetting.SetSel(-1, FALSE);
	}
}


/*============================================================================*/
/*! CInitSetting

-# CIDチェックボタン押下時処理

@param  なし
@retval なし

*/
/*============================================================================*/
void CInitSetting::OnBnClickedCidcheck()
{
	UpdateData();

	if (m_CidChkBox == TRUE)
	{
		// 初期設定リスト、選択ボタン非活性
		m_listInitSetting.EnableWindow(FALSE);
		m_chkAllSelect.EnableWindow(FALSE);
	}
	else
	{
		// 初期設定リスト、選択ボタン活性
		m_listInitSetting.EnableWindow(TRUE);
		m_chkAllSelect.EnableWindow(TRUE);
	}
}


/*============================================================================*/
/*! CInitSetting

-# 初期設定実行中タイマー処理

@param  nIDEvent
@retval なし

*/
/*============================================================================*/
void CInitSetting::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);

	CWaitCursor wait;
	CString strEquip;
	int pos;
	// 3つまで連続送信
	for (pos = m_aryListBoxPos; pos < m_aryListBoxSel.GetCount() && pos < m_aryListBoxPos + 3; ++pos)
	{
		int idx = m_aryListBoxSel.GetAt(pos);
		m_listInitSetting.GetText(idx, strEquip);

		CString ctrl = _T("");
		ctrl.Format(_T("%s ,%s;"), (LPCTSTR)strEquip, (LPCTSTR)m_satSel);

		// 制御シェル呼出
		theApp.SendControlNoUI(ctrl, _T(""), _T(""), _T(""));
	}

//	wait.Restore();

	// リストの終わりをチェック
	if (pos == m_aryListBoxSel.GetCount())
	{
		// 初期設定画面を閉じる
		EndDialog(IDOK);
	}
	// 未送信有り
	else
	{
		m_aryListBoxPos = pos;
		// タイマ作成
		SetTimer(1, 3000, 0);
	}

	CDialogBase::OnTimer(nIDEvent);
}
