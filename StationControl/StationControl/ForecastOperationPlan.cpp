/*============================================================================*/
/*! ForecastOperationPlan.cpp

-# 予報値一覧画面　運用計画
*/
/*============================================================================*/
// ForecastOperationPlan.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ForecastOperationPlan.h"
#include "ForecastList.h"
#include "afxdialogex.h"

//----------------------------------------------------------------------
// ダミーデータ

// 計画一覧更新確認周期
#define  INTERVAL_REAL	1000						// トレンド 表示開始時間（ミリ秒）

// CForecastOperationPlan ダイアログ

IMPLEMENT_DYNAMIC(CForecastOperationPlan, CDialogBase)

/*============================================================================*/
/*! CForecastOperationPlan

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CForecastOperationPlan::CForecastOperationPlan(CWnd* pParent /*=NULL*/)
	: CDialogBase(CForecastOperationPlan::IDD, pParent)
	, m_bUpperSort(TRUE)
	, m_nSelectedColumn(0)
	, m_nStationIndex(-1)
	, m_strSelectedStation(_T(""))
	, m_strSelectedSatellite(_T(""))
	, m_strSelectedPassId(_T(""))
	, m_strSelectedGetFrom(_T(""))
	, m_strSelectedFile(_T(""))
{

}

/*============================================================================*/
/*! CForecastOperationPlan

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CForecastOperationPlan::~CForecastOperationPlan()
{
}

void CForecastOperationPlan::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FORECASTOPERATIONPLAN_LIST1, m_listOperationPlan);
}


BEGIN_MESSAGE_MAP(CForecastOperationPlan, CDialogBase)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CForecastOperationPlan::OnHdnItemclickForecastoperationplanList1)
	ON_NOTIFY(NM_CLICK, IDC_FORECASTOPERATIONPLAN_LIST1, &CForecastOperationPlan::OnNMClickForecastoperationplanList1)
//	ON_WM_TIMER()
END_MESSAGE_MAP()


// CForecastOperationPlan メッセージ ハンドラー


/*============================================================================*/
/*! CForecastOperationPlan

-# ダイアログ初期化処理

@param  なし
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
BOOL CForecastOperationPlan::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_strSelectedStation = _T("");
	m_strSelectedSatellite = _T("");
	m_strSelectedPassId = _T("");
	m_strSelectedGetFrom = _T("");
	m_strSelectedFile = _T("");

	// リスト初期化
	if (!InitList())
		return FALSE;

	// リストアイテム挿入
	if (!InsertListItem())
		return FALSE;

	//常にスクロールバーを表示させる
	m_listOperationPlan.EnableScrollBarCtrl(SB_VERT, TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CForecastOperationPlan

-# リストコントロール初期化

@param	なし
@retval	TRUE:初期化成功 / FALSE:初期化失敗

*/
/*============================================================================*/
BOOL CForecastOperationPlan::InitList()
{
	LVCOLUMN lvc;
	TCHAR szMsg[256];

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTOPERATIONPLAN_STATIONNAME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 0;
	lvc.pszText = szMsg;
	lvc.cx = m_nStationNameWidth;
	if (m_listOperationPlan.InsertColumn(0, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTOPERATIONPLAN_SATELLITENAME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 1;
	lvc.pszText = szMsg;
	lvc.cx = m_nSatelliteNameWidth;
	if (m_listOperationPlan.InsertColumn(1, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_PASSID, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 2;
	lvc.pszText = szMsg;
	lvc.cx = m_nPassIdWidth;
	if (m_listOperationPlan.InsertColumn(2, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTOPERATIONPLAN_PREPASSSTARTTIME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 3;
	lvc.pszText = szMsg;
	lvc.cx = m_prePassStartTimeWidth;
	if (m_listOperationPlan.InsertColumn(3, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTOPERATIONPLAN_PASSSTARTTIME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 4;
	lvc.pszText = szMsg;
	lvc.cx = m_passStartTimeWidth;
	if (m_listOperationPlan.InsertColumn(4, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTOPERATIONPLAN_PASSENDTIME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 5;
	lvc.pszText = szMsg;
	lvc.cx = m_passEndTimeWidth;
	if (m_listOperationPlan.InsertColumn(5, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTOPARETIONPLAN_CREATE, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 6;
	lvc.pszText = szMsg;
	lvc.cx = m_createTypeWidth;
	if (m_listOperationPlan.InsertColumn(6, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTOPERATIONPLAN_OPERATIONTYPE, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 7;
	lvc.pszText = szMsg;
	lvc.cx = m_operationTypeWidth;
	if (m_listOperationPlan.InsertColumn(7, &lvc) < 0)
		return FALSE;

	m_listOperationPlan.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	return TRUE;
}

/*============================================================================*/
/*! CForecastOperationPlan

-# リストコントロールにアイテム挿入

@param	なし
@retval	TRUE:挿入成功 / FALSE:挿入失敗

*/
/*============================================================================*/
BOOL CForecastOperationPlan::InsertListItem()
{
	CForecastList* pWnd = (CForecastList*)GetParent();
	if (pWnd == NULL)
	{
		return FALSE;
	}

	CGraphMutex::Lock(eFile);
	vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();
//	// 計画一覧をコピー
//	m_listPlanList = listPlan;

	SetStationIndex();
	SetSatelliteName();

	m_listOperationPlan.DeleteAllItems();

	LVITEM lvi;
	CString str;
	int i, index;
//	TCHAR szMsg[256];
	CTime ctime;

	// 背景色をセット
	map<CString, COLORREF>	mapColor;
	int sz = sizeof(mStationSimpleString) / sizeof(mStationSimpleString[0]);
	for (int i = 0; i < sz; i++)
	{
//		mapColor.insert(map<CString, COLORREF>::value_type(mStationSimpleString[i], mStationColor[i]));
		mapColor.insert(map<CString, COLORREF>::value_type(mStationString[i], mStationColor[i]));
	}
	m_listOperationPlan.SetColorMap(mapColor);

	// リストコントロールにアイテム挿入
	for (i = 0, index = 0; i < (int)listPlan.size(); i++)
	{
		// 局名がALLまたは一致のときにリストに追加
//		str.Format(mStationSimpleString[m_nStationIndex]);
//		str = pWnd->GetSimpleStationName();
//		if (!(m_strStationName.CompareNoCase(_T("All")) == 0 || str.Compare(listPlan[i].strStationName) == 0))
		if (!(m_strStationName.CompareNoCase(_T("All")) == 0 || m_strStationName.Compare(listPlan[i].strStationName) == 0))
			continue;

		// 衛星名がALLまたは一致のときにリストに追加
		if (!(m_strSatelliteName.CompareNoCase(_T("All")) == 0 ||
			m_strSatelliteName.CompareNoCase(listPlan[i].strSatelliteName) == 0))
			continue;

		// リストコントロールに内容セット
		lvi.mask = LVIF_TEXT;

		lvi.iItem = i;
		lvi.iSubItem = 0;
		str.Format(listPlan[i].strStationName);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if ((index = m_listOperationPlan.InsertItem(&lvi)) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 1;
		str.Format(listPlan[i].strSatelliteName);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listOperationPlan.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 2;
//		str.Format(+_T("%07u"), listPlan[i].m_nPlanId);
		str.Format(listPlan[i].strPassId);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listOperationPlan.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 3;
//		ctime = listPlan[i].tPrePassStart;
		if (listPlan[i].lOfsFlag == 1)
		{
			ctime = listPlan[i].tPrePassStart + listPlan[i].tOffset;
		}
		else
		{
			ctime = listPlan[i].tPrePassStart;
		}
		str = ctime.Format("%Y-%m-%d %H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listOperationPlan.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 4;
//		ctime = listPlan[i].tPassStart;
		if (listPlan[i].lOfsFlag == 1)
		{
			ctime = listPlan[i].tPassStart + listPlan[i].tOffset;
		}
		else
		{
			ctime = listPlan[i].tPassStart;
		}
		str = ctime.Format("%Y-%m-%d %H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listOperationPlan.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 5;
//		ctime = listPlan[i].tPassEnd;
		if (listPlan[i].lOfsFlag == 1)
		{
			ctime = listPlan[i].tPassEnd + listPlan[i].tOffset;
		}
		else
		{
			ctime = listPlan[i].tPassEnd;
		}
		str = ctime.Format("%Y-%m-%d %H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listOperationPlan.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 6;
		str.Format(listPlan[i].strGetFrom);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listOperationPlan.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 7;
		str.Format(listPlan[i].strPlanType);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listOperationPlan.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}
	}
	CGraphMutex::Unlock(eFile);

	for (int i = 0; i < m_listOperationPlan.GetItemCount(); i++) {
		m_listOperationPlan.SetItemData(i, i);
	}

//	UpdateData(FALSE);
//	UpdateWindow();

	return TRUE;
}

/*============================================================================*/
/*! CForecastOperationPlan

-# リストコントロールのアイテムを更新する

@param	なし
@retval	TRUE:更新成功 / FALSE:更新失敗

*/
/*============================================================================*/
BOOL CForecastOperationPlan::UpdateListItem()
{
	int nItemCount = m_listOperationPlan.GetItemCount();
	CRect rect;
	m_listOperationPlan.GetItemRect(0, &rect, LVIR_BOUNDS);
	int index = m_listOperationPlan.GetTopIndex();
	int offset = rect.Height() * index;
	CSize cs;
	cs.cx = 0;
	cs.cy = offset;

	// アイテム挿入
	InsertListItem();

	// アイテム数が変わっていない場合は、記録したスクロール位置にセットする
	if (nItemCount == m_listOperationPlan.GetItemCount())
	{
		if (offset)
		{
			m_listOperationPlan.Scroll(cs);
		}
	}

	CString strStation, strSatellite, strPassId, strGetFrom, strPredPath;
	CGraphMutex::Lock(eFile);
	vector<stPlanListData>::iterator itr;
	vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();

	BOOL bFind = FALSE;

	nItemCount = m_listOperationPlan.GetItemCount();
	for (int i = 0; i < nItemCount && bFind == FALSE; i++)
	{
		strStation = m_listOperationPlan.GetItemText(i, 0);
		strSatellite = m_listOperationPlan.GetItemText(i, 1);
		strPassId = m_listOperationPlan.GetItemText(i, 2);
		strGetFrom = m_listOperationPlan.GetItemText(i, 6);

		if (strStation.CompareNoCase(m_strSelectedStation) == 0 &&
			strSatellite.CompareNoCase(m_strSelectedSatellite) == 0 &&
			strPassId.CompareNoCase(m_strSelectedPassId) == 0 &&
			strGetFrom.CompareNoCase(m_strSelectedGetFrom) == 0)
		{
			// リストコントロールと選択されていたものが一致
			for (itr = listPlan.begin(); itr != listPlan.end() && bFind == FALSE; itr++)
			{
				if ((*itr).strStationName.Compare(m_strSelectedStation) == 0 &&
					(*itr).strSatelliteName.Compare(m_strSelectedSatellite) == 0 &&
					(*itr).strPassId.Compare(m_strSelectedPassId) == 0 &&
					(*itr).strGetFrom.Compare(m_strSelectedGetFrom) == 0)
				{
					// 計画一覧ファイルの局、衛星、計画ID、作成元、予報値ファイルが一致
					if ((*itr).strStationName.Compare(m_strSelectedStation) == 0 &&
						(*itr).strSatelliteName.Compare(m_strSelectedSatellite) == 0 &&
						(*itr).strPassId.Compare(m_strSelectedPassId) == 0 &&
						(*itr).strGetFrom.Compare(m_strSelectedGetFrom) == 0 && 
						(*itr).strPredName.CompareNoCase(m_strSelectedFile) == 0)
					{
						// リストコントロール選択
						if (m_listOperationPlan.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED) == 0)
						{
							// リスト選択できなかった場合は、ファイルパスをクリア
							m_strSelectedFile = _T("");
						}

						bFind = TRUE;
					}
				}
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	InitRadarGraphView();

	return TRUE;
}


/*============================================================================*/
/*! CForecastOperationPlan

-# リストコントロールソート時のコールバック関数

@param	lParam1		比較元アイテム
@param	lParam2		比較先アイテム
@param	lParamSort	ウィンドウハンドル
@retval 0: 一致, -1:比較元の順番が低い, 1:比較元の順番が高い

*/
/*============================================================================*/
int CALLBACK CForecastOperationPlan::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CForecastOperationPlan* dlg = (CForecastOperationPlan*)lParamSort;
	return dlg->CompareFunc(lParam1, lParam2);
}

/*============================================================================*/
/*! CForecastOperationPlan

-# リストコントロールソート時の比較関数

@param	lParam1		比較元アイテム
@param	lParam2		比較先アイテム
@retval 0: 一致, -1:比較元の順番が低い, 1:比較元の順番が高い

*/
/*============================================================================*/
int CForecastOperationPlan::CompareFunc(LPARAM lParam1, LPARAM lParam2)
{
	CString str[2];
	LPARAM param[2] = { lParam1, lParam2 };

	// リストコントロールから比較するアイテム取得
	for (int i = 0; i < 2; ++i)
	{
		LVFINDINFO fi = { 0 };
		fi.flags = LVFI_PARAM;
		fi.lParam = param[i];
		str[i] = m_listOperationPlan.GetItemText(m_listOperationPlan.FindItem(&fi), m_nSelectedColumn);
	}

	// 比較処理
	int ret = 0;
	if (m_bUpperSort)
	{
		ret = str[0].CompareNoCase(str[1]);
	}
	else
	{
		ret = str[1].CompareNoCase(str[0]);
	}

	return ret;
}


/*============================================================================*/
/*! CForecastOperationPlan

-# 描画時に呼ばれ、指定コントロールの背景を塗りつぶすブラシのハンドルを返す

@param	pDC：子ウィンドウのディスプレイコンテキストのポインタ
@param	pWnd：色を格納されたコントロールのポインタ
@param	nCtrlColor：コントロールの種類
@retval	コントロールの背景を塗りつぶすブラシのハンドル

*/
/*============================================================================*/
HBRUSH CForecastOperationPlan::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogBase::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG && pWnd == this)
	{
		hbr = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
	}

	// TODO: 既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

/*============================================================================*/
/*! CForecastOperationPlan

-# 局名が変更されている場合に局名をセット

@param	なし
@retval TRUE: 変更されている。 FALSE: 変更されていない。

*/
/*============================================================================*/
BOOL CForecastOperationPlan::SetStationName()
{
	CForecastList* pWnd = (CForecastList*)GetParent();
	if (pWnd == NULL)
	{
		return FALSE;
	}

	CString str = pWnd->GetStationName();
	// 局名が変更された場合
	if (m_strStationName.CompareNoCase(str) != 0)
	{
		// 局名をセットする
		m_strStationName = str;
		return TRUE;
	}

	return FALSE;
}

/*============================================================================*/
/*! CForecastOperationPlan

-# 局インデックスをセット

@param	なし
@retval TRUE: 取得成功。 FALSE: 取得失敗。

*/
/*============================================================================*/
BOOL CForecastOperationPlan::SetStationIndex()
{
	SetStationName();

	CForecastList* pWnd = (CForecastList*)GetParent();
	if (pWnd == NULL)
	{
		return FALSE;
	}

	int nIndex = pWnd->GetStationIndex();
	if (pWnd->GetStationCount() > 1)
	{
		nIndex--;
	}
	m_nStationIndex = nIndex;

	return TRUE;
}


/*============================================================================*/
/*! CForecastOperationPlan

-# 衛星名が変更されている場合に衛星名をセット

@retval TRUE: 変更されている。 FALSE: 変更されていない。

*/
/*============================================================================*/
BOOL CForecastOperationPlan::SetSatelliteName()
{
	CForecastList* pWnd = (CForecastList*)GetParent();
	if (pWnd == NULL)
	{
		return FALSE;
	}

	CString str = pWnd->GetSatelliteName();
	// 衛星名が変更された場合
	if (m_strSatelliteName.CompareNoCase(str) != 0)
	{
		// 衛星名をセットする
		m_strSatelliteName = str;
		return TRUE;
	}

	return FALSE;
}

#if 0
/*============================================================================*/
/*! CForecastOperationPlan

-# 選択されている運用計画を削除する

@retval TRUE: 削除した。 FALSE: 削除しない。

*/
/*============================================================================*/
BOOL CForecastOperationPlan::DeleteListItem()
{
	POSITION pos;
	int nItem;
	vector<stPlanListData>::iterator itr;
	BOOL ret = FALSE;
	CString strSatelliteName, strPassId, strGetFrom;

	vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();

	pos = m_listOperationPlan.GetFirstSelectedItemPosition();
	if (!pos)
	{
		// 選択されていないメッセージ表示
		CForecastList* pWnd = (CForecastList*)GetParent();
		if (pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->ViewUnSelectedMessage();
			return FALSE;
		}
	}
	CGraphMutex::Lock(eFile);

	while (TRUE)
	{
		pos = m_listOperationPlan.GetFirstSelectedItemPosition();
		if (!pos)
			break;

		nItem = m_listOperationPlan.GetNextSelectedItem(pos);
		strSatelliteName = m_listOperationPlan.GetItemText(nItem, 1);
		strPassId = m_listOperationPlan.GetItemText(nItem, 2);
		strGetFrom = m_listOperationPlan.GetItemText(nItem, 6);
		for (itr = listPlan.begin(); itr != listPlan.end(); itr++)
		{
			// 画面のコントロールとリストで衛星名とパスIDの一致を調べる
			if ((*itr).strSatelliteName.Compare(strSatelliteName) == 0 &&
				(*itr).strPassId.Compare(strPassId) == 0 &&
				(*itr).strGetFrom.Compare(strGetFrom) == 0)
			{
				listPlan.erase(itr);
				m_listOperationPlan.DeleteItem(nItem);
				ret = TRUE;
				break;
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	// 最後に削除した位置を選択しておく
	int size = m_listOperationPlan.GetItemCount();
	if (size > 0)
	{
		if (nItem >= size)
		{
			nItem = size - 1;
		}
		if (nItem >= 0)
		{
			m_listOperationPlan.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
		}
	}

	UpdateData(FALSE);
	UpdateWindow();

	return ret;
}
#endif

/*============================================================================*/
/*! CForecastOperationPlan

-# リストコントロールの項目クリック時に呼ばれる

@retval TRUE: 削除した。 FALSE: 削除しない。

*/
/*============================================================================*/
void CForecastOperationPlan::OnHdnItemclickForecastoperationplanList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	if (phdr == NULL)
		return;

	m_nSelectedColumn = phdr->iItem;

	for (int i = 0; i < m_listOperationPlan.GetItemCount(); i++) {
		m_listOperationPlan.SetItemData(i, i);
	}
	m_listOperationPlan.SortItems(&CForecastOperationPlan::CompareFunc, (LPARAM)this);

	m_bUpperSort = !m_bUpperSort;

	*pResult = 0;
}


/*============================================================================*/
/*! CForecastValue

-# 選択されている予報値リストの一番最初のファイルパスを返す

@param	strFilePath：ファイルパス
@retval TRUE: 返す。 FALSE: 返さない。

*/
/*============================================================================*/
BOOL CForecastOperationPlan::GetSelectedPredFilePath(CString& strFilePath, BOOL bChk /*= FALSE*/)
{
	POSITION pos;
	int nItem;
	CString strStationName, strSatelliteName, strPassId, strGetFrom;

	pos = m_listOperationPlan.GetFirstSelectedItemPosition();
	if (bChk && !pos)
	{
		// 選択されていないメッセージ表示
		CForecastList* pWnd = (CForecastList*)GetParent();
		if (pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->ViewUnSelectedMessage();
			return FALSE;
		}
	}
	CGraphMutex::Lock(eFile);
	vector<stPlanListData>::iterator itr;
	vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();

	while (pos)
	{
		nItem = m_listOperationPlan.GetNextSelectedItem(pos);
		strStationName = m_listOperationPlan.GetItemText(nItem, 0);
		strSatelliteName = m_listOperationPlan.GetItemText(nItem, 1);
		strPassId = m_listOperationPlan.GetItemText(nItem, 2);
		strGetFrom = m_listOperationPlan.GetItemText(nItem, 6);
		for (itr = listPlan.begin(); itr != listPlan.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationName) == 0 &&
				(*itr).strSatelliteName.Compare(strSatelliteName) == 0 &&
				(*itr).strPassId.Compare(strPassId) == 0 &&
				(*itr).strGetFrom.Compare(strGetFrom) == 0)
			{
				strFilePath = ((*itr).strPredName);
				CGraphMutex::Unlock(eFile);
				return TRUE;
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	return FALSE;
}


/*============================================================================*/
/*! CForecastValue

-# 選択されている予報値リストの一番最初の計画ID、運用開始日時、運用終了日時を返す。

@param	strPlanID：計画ID
@param	tPassStart：運用開始日時
@param	tPassEnd：運用終了日時
@retval TRUE: 返す。 FALSE: 返さない。

*/
/*============================================================================*/
BOOL CForecastOperationPlan::GetSelectedPlanID(CString& strPlanID, __int64& tPassStart, __int64& tPassEnd, BOOL bChk /*= FALSE*/)
{
	POSITION pos, pos2;
	int nItem;
	CString strStationName, strSatelliteName, strPassIdTmp, strGetFrom;

	pos = m_listOperationPlan.GetFirstSelectedItemPosition();
	pos2 = pos;
	if (bChk && !pos)
	{
		// 選択されていないメッセージ表示
		CForecastList* pWnd = (CForecastList*)GetParent();
		if (pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->ViewUnSelectedMessage();
			return FALSE;
		}
	}
	CGraphMutex::Lock(eFile);
	vector<stPlanListData>::iterator itr;
	vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();

	// 先に局立案優先で局、衛星、計画IDが一致を調べる
	BOOL bDiffGetFrom = FALSE;
	while (pos)
	{
		nItem = m_listOperationPlan.GetNextSelectedItem(pos);
		strStationName = m_listOperationPlan.GetItemText(nItem, 0);
		strSatelliteName = m_listOperationPlan.GetItemText(nItem, 1);
		strPassIdTmp = m_listOperationPlan.GetItemText(nItem, 2);
		strGetFrom = m_listOperationPlan.GetItemText(nItem, 6);
		for (itr = listPlan.begin(); itr != listPlan.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationName) == 0 &&
				(*itr).strSatelliteName.Compare(strSatelliteName) == 0 &&
				(*itr).strPassId.Compare(strPassIdTmp) == 0)
			{
				if ((*itr).strGetFrom.Compare(strGetFrom) == 0)
				{
					// 局、衛星、計画ID、ENP/局管制の一致
					if ((*itr).strGetFrom.CompareNoCase(_T("ENP")) != 0)
					{
						// 局管制の場合
						strPlanID = ((*itr).strPassId);
						tPassStart = ((*itr).tPassStart);
						tPassEnd = ((*itr).tPassEnd);
						CGraphMutex::Unlock(eFile);
						return TRUE;
					}
				}
				else
				{
					// 局、衛星、計画IDが同じだが、ENP/局立案違いが見つかった
					bDiffGetFrom = TRUE;
				}
			}
		}
	}

	// 局立案検索後にENPで局、衛星、計画IDが一致を調べる
	while (pos2)
	{
		nItem = m_listOperationPlan.GetNextSelectedItem(pos2);
		strStationName = m_listOperationPlan.GetItemText(nItem, 0);
		strSatelliteName = m_listOperationPlan.GetItemText(nItem, 1);
		strPassIdTmp = m_listOperationPlan.GetItemText(nItem, 2);
		strGetFrom = m_listOperationPlan.GetItemText(nItem, 6);
		for (itr = listPlan.begin(); itr != listPlan.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationName) == 0 &&
				(*itr).strSatelliteName.Compare(strSatelliteName) == 0 &&
				(*itr).strPassId.Compare(strPassIdTmp) == 0 &&
				(*itr).strGetFrom.Compare(strGetFrom) == 0)
			{
				if ((*itr).strGetFrom.CompareNoCase(_T("ENP")) == 0 && bDiffGetFrom == FALSE)
				{
					// ENPでかつ局管制に同じ計画IDがなかった場合
					strPlanID = ((*itr).strPassId);
					tPassStart = ((*itr).tPassStart);
					tPassEnd = ((*itr).tPassEnd);
					CGraphMutex::Unlock(eFile);
					return TRUE;
				}
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	return FALSE;
}


/*============================================================================*/
/*! CForecastValue

-# 選択されている計画リストの一番最初の局名、衛星名、予報値ファイルパスを返す

@param	strStationName：局名
@param	strSatelliteName：衛星名
@param	strPassId：計画ID
@param	strGetFrom：作成元
@param	strFilePath：予報値ファイルパス
@retval TRUE: 返す。 FALSE: 返さない。

*/
/*============================================================================*/
BOOL CForecastOperationPlan::GetSelectedStationNameAndPredFilePath(CString& strStationName, CString& strSatelliteName, CString& strPassId, 
	CString& strGetFrom, CString& strFilePath, BOOL bChk /*= FALSE*/)
{
	POSITION pos;
	int nItem;
	CString strStationNameCmp, strSatelliteNameCmp, strPassIdCmp, strGetFromCmp;

	pos = m_listOperationPlan.GetFirstSelectedItemPosition();
	if (bChk && !pos)
	{
		// 選択されていないメッセージ表示
		CForecastList* pWnd = (CForecastList*)GetParent();
		if (pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->ViewUnSelectedMessage();
			return FALSE;
		}
	}
	CGraphMutex::Lock(eFile);
	vector<stPlanListData>::iterator itr;
	vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();

	while (pos)
	{
		nItem = m_listOperationPlan.GetNextSelectedItem(pos);
		strStationNameCmp = m_listOperationPlan.GetItemText(nItem, 0);
		strSatelliteNameCmp = m_listOperationPlan.GetItemText(nItem, 1);
		strPassIdCmp = m_listOperationPlan.GetItemText(nItem, 2);
		strGetFromCmp = m_listOperationPlan.GetItemText(nItem, 6);
		for (itr = listPlan.begin(); itr != listPlan.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationNameCmp) == 0 &&
				(*itr).strSatelliteName.Compare(strSatelliteNameCmp) == 0 &&
				(*itr).strPassId.Compare(strPassIdCmp) == 0 &&
				(*itr).strGetFrom.Compare(strGetFromCmp) == 0)
			{
				strStationName = ((*itr).strStationName);
				strSatelliteName = ((*itr).strSatelliteName);
				strPassId = ((*itr).strPassId);
				strGetFrom = ((*itr).strGetFrom);
				strFilePath = ((*itr).strPredName);
				CGraphMutex::Unlock(eFile);
				return TRUE;
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	return FALSE;
}


/*============================================================================*/
/*! CForecastValue

-# 選択されている一番最初の局名、衛星名、計画ファイル名、取得先を返す

@param	strStationName：局名
@param	strSatelliteName：衛星名
@param	strFilePath：パスID
@param	strGetFrom：取得先
@retval TRUE: 返す。 FALSE: 返さない。

*/
/*============================================================================*/
BOOL CForecastOperationPlan::GetSelectedStationNameAndPlanFilePath(CString& strStationName, CString& strSatelliteName, CString& strFilePath, CString& strGetFrom, BOOL bChk /*= FALSE*/)
{
	POSITION pos;
	int nItem;
	CString strPassId;
	CString strSatelliteNameCmp;
	CString strStationNameCmp;
	CString strGetFromTmp;

	pos = m_listOperationPlan.GetFirstSelectedItemPosition();
	if (bChk && !pos)
	{
		// 選択されていないメッセージ表示
		CForecastList* pWnd = (CForecastList*)GetParent();
		if (pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->ViewUnSelectedMessage();
			return FALSE;
		}
	}

	CGraphMutex::Lock(eFile);
	vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();
	vector<stPlanListData>::iterator itr;

	while (pos)
	{
		nItem = m_listOperationPlan.GetNextSelectedItem(pos);
		strStationNameCmp = m_listOperationPlan.GetItemText(nItem, 0);
		strSatelliteNameCmp = m_listOperationPlan.GetItemText(nItem, 1);
		strPassId = m_listOperationPlan.GetItemText(nItem, 2);
		strGetFromTmp = m_listOperationPlan.GetItemText(nItem, 6);
		for (itr = listPlan.begin(); itr != listPlan.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationNameCmp) == 0 &&
				(*itr).strSatelliteName.Compare(strSatelliteNameCmp) == 0 &&
				(*itr).strPassId.Compare(strPassId) == 0 &&
				(*itr).strGetFrom.Compare(strGetFromTmp) == 0)
			{
				strSatelliteName = ((*itr).strSatelliteName);
				strStationName = ((*itr).strStationName);
				// 計画ファイルパスを作成
				strFilePath.Format(_T("%s.%s"), (LPCTSTR)strPassId, PLAN_FILE_TYPE);
				strGetFrom = ((*itr).strGetFrom);
				CGraphMutex::Unlock(eFile);
				return TRUE;
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	return FALSE;
}


/*============================================================================*/
/*! CForecastValue

-# 選択されている一番最初の局名、衛星名、計画ファイル名、取得先を返す

@param	strStationName：局名
@param	strSatelliteName：衛星名
@param	strFilePath：パスID
@param	strGetFrom：取得先
@retval TRUE: 返す。 FALSE: 返さない。

*/
/*============================================================================*/
BOOL CForecastOperationPlan::GetSelectedStationNameAndPlanFilePath(vector<stPlanListData>& listPlan, BOOL bChk /*= FALSE*/)
{
	POSITION pos;
	int nItem;
	CString strPassId;
	CString strSatelliteNameCmp;
	CString strStationNameCmp;
	CString strGetFromTmp;

	BOOL bRet = FALSE;

	pos = m_listOperationPlan.GetFirstSelectedItemPosition();
	if (bChk && !pos)
	{
		// 選択されていないメッセージ表示
		CForecastList* pWnd = (CForecastList*)GetParent();
		if (pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->ViewUnSelectedMessage();
			return bRet;
		}
	}

	CGraphMutex::Lock(eFile);
	vector<stPlanListData>& srcListPlan = theApp.GetSatelliteData().GetPlanList();
	vector<stPlanListData>::iterator itr;

	while (pos)
	{
		nItem = m_listOperationPlan.GetNextSelectedItem(pos);
		strStationNameCmp = m_listOperationPlan.GetItemText(nItem, 0);
		strSatelliteNameCmp = m_listOperationPlan.GetItemText(nItem, 1);
		strPassId = m_listOperationPlan.GetItemText(nItem, 2);
		strGetFromTmp = m_listOperationPlan.GetItemText(nItem, 6);
		for (itr = srcListPlan.begin(); itr != srcListPlan.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationNameCmp) == 0 &&
				(*itr).strSatelliteName.Compare(strSatelliteNameCmp) == 0 &&
				(*itr).strPassId.Compare(strPassId) == 0 &&
				(*itr).strGetFrom.Compare(strGetFromTmp) == 0)
			{
				listPlan.push_back((*itr));
				// 計画ファイルパスを作成
				listPlan[listPlan.size() - 1].strPlanName.Format(_T("%s.%s"), (LPCTSTR)strPassId, PLAN_FILE_TYPE);
				bRet = TRUE;
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	return bRet;
}


/*============================================================================*/
/*! CForecastValue

-# 局名、衛星名、予報値パスIDが計画で使われているか調べる

@param	strStationName：局名
@param	strSatelliteName：衛星名
@param	strPredPassId：予報値パスID
@retval TRUE: 使われている。 FALSE: 使われていない。

*/
/*============================================================================*/
BOOL CForecastOperationPlan::IsUsedPlan(CString& strStationName, CString& strSatelliteName, CString& strPredPassId)
{
	CGraphMutex::Lock(eFile);
	vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();
	vector<stPlanListData>::iterator itr;

	CString strPredId;
	int idx = strPredPassId.Find(_T("-"));
	strPredId.Format(_T("%s%s"), (LPCTSTR)strPredPassId.Mid(0, idx), (LPCTSTR)strPredPassId.Mid(idx + 1, 4));

	for (itr = listPlan.begin(); itr != listPlan.end(); itr++)
	{
		if ((*itr).strStationName.Compare(strStationName) == 0 &&
			(*itr).strSatelliteName.Compare(strSatelliteName) == 0 &&
			(*itr).strPredName.IsEmpty() == false && 
			(*itr).strPredName.Find(strPredId) >= 0)
		{
			CGraphMutex::Unlock(eFile);
			return TRUE;
		}
	}
	CGraphMutex::Unlock(eFile);

	return FALSE;
}


/*============================================================================*/
/*! CForecastOperationPlan

-# 追尾グラフを初期化する。

@param  なし
@retval なし

*/
/*============================================================================*/
void CForecastOperationPlan::InitRadarGraphView()
{
	// 選択されている一番上の計画ファイルパスを取得
	CString strStationName, strSatelliteName, strPassId, strGetFrom, strFilePath;
	if (GetSelectedStationNameAndPredFilePath(strStationName, strSatelliteName, strPassId, strGetFrom, strFilePath))
	{
		// 選択されている場合、新しいファイルパス取得
		m_strSelectedStation = strStationName;
		m_strSelectedSatellite = strSatelliteName;
		m_strSelectedPassId = strPassId;
		m_strSelectedGetFrom = strGetFrom;
		m_strSelectedFile = strFilePath;
	}
	else
	{
		// 選択されていない場合
		m_strSelectedStation = _T("");
		m_strSelectedSatellite = _T("");
		m_strSelectedPassId = _T("");
		m_strSelectedGetFrom = _T("");
		m_strSelectedFile = _T("");
		strFilePath = _T("");
	}

	// 局名から局番号取得
	int nStation = CForecastList::GetStationIndex(strStationName);

	CForecastList* pWnd = (CForecastList*)GetParent();
	if (pWnd == NULL)
		return;

	CForecastRadarGraphView* pView = (CForecastRadarGraphView*)pWnd->GetView();
	if (pView == NULL || pView->GetSafeHwnd() == NULL)
		return;

	CString strPredId = strFilePath;
	CString strFullPath;
	if (strFilePath.IsEmpty() || strSatelliteName.IsEmpty() || nStation < 0)
	{
		// 未選択もしくはファイルが無い場合
		nStation = -1;
		strFullPath = _T("");
		strPredId = _T("");
	}
	else
	{
		// 予報値ファイルのフルパスを作る
		strFullPath = theApp.GetShareFilePath(eFileType_Pred_Folder, nStation) + strSatelliteName + _T("\\") + strFilePath;
		// 予報値ファイル名の拡張子を削除、'-'を挿入し、予報値IDを作る
		AfxExtractSubString(strPredId, strFilePath, 0, '.');
		strPredId.Insert(6, '-');
	}

	// 予報値の追尾グラフにする
	pView->SetViewtype(eRadarViewType_OperationPlan);
	pView->SetStationID(nStation);

	// 計画ID、運用開始時刻、運用終了時刻セット
	CString strPlanID;
	__int64 tPassStart, tPassEnd;
	if (GetSelectedPlanID(strPlanID, tPassStart, tPassEnd))
	{
		pView->SetPlanID(strPlanID, tPassStart, tPassEnd);	// 計画がみつかった
	}
	else
	{
		pView->SetPlanID(_T(""), 0, 0);		// 計画がみつからなかった
	}

	// 追尾グラフを初期化する
	pView->Init(strFullPath, strSatelliteName, strPredId);
}


/*============================================================================*/
/*! CForecastOperationPlan

-# 計画一覧から局名リストを取得する

@param	listName：局名リスト
@retval TRUE: 返す。 FALSE: 返さない。

*/
/*============================================================================*/
BOOL CForecastOperationPlan::GetStationNameList(vector<CString>& listName)
{
	CGraphMutex::Lock(eFile);
	vector<stPredListData>::iterator itr;
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();
	vector<CString>::iterator itrName;

	listName.clear();

	for (itr = listPred.begin(); itr != listPred.end(); itr++)
	{
		// 局名が既に局名リストにあるか調べる
		itrName = find(listName.begin(), listName.end(), (*itr).strStationName);
		if (itrName == listName.end())
		{
			// 無い場合は局名リストに追加
			listName.push_back((*itr).strStationName);
		}
	}
	CGraphMutex::Unlock(eFile);

	if (listName.empty() == TRUE)
		return FALSE;

	return TRUE;
}


/*============================================================================*/
/*! CForecastOperationPlan

-# 計画一覧から衛星名リストを取得する

@param	listName：衛星名リスト
@retval TRUE: 返す。 FALSE: 返さない。

*/
/*============================================================================*/
BOOL CForecastOperationPlan::GetSatelliteNameList(vector<CString>& listName)
{
	CGraphMutex::Lock(eFile);
	vector<stPlanListData>::iterator itr;
	vector<stPlanListData>& listPred = theApp.GetSatelliteData().GetPlanList();
	vector<CString>::iterator itrName;

	listName.clear();

	for (itr = listPred.begin(); itr != listPred.end(); itr++)
	{
		// 衛星名が既に衛星名リストにあるか調べる
		itrName = find(listName.begin(), listName.end(), (*itr).strSatelliteName);
		if (itrName == listName.end())
		{
			// 無い場合は局名リストに追加
			listName.push_back((*itr).strSatelliteName);
		}
	}
	CGraphMutex::Unlock(eFile);

	if (listName.empty() == TRUE)
		return FALSE;

	return TRUE;
}


/*============================================================================*/
/*! CForecastOperationPlan

-# リストコントロールをクリック時に呼ばれる。

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし

*/
/*============================================================================*/
void CForecastOperationPlan::OnNMClickForecastoperationplanList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	InitRadarGraphView();
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: ここにコントロール通知ハンドラー コードを追加します。
//	*pResult = 0;
}

