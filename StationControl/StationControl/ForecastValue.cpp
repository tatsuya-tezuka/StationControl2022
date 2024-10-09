/*============================================================================*/
/*! ForecastValue.cpp

-# 予報値一覧画面　予報値
*/
/*============================================================================*/
// ForecastValue.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ForecastValue.h"
#include "ForecastList.h"
#include "afxdialogex.h"


// CForecastValue ダイアログ

IMPLEMENT_DYNAMIC(CForecastValue, CDialogBase)

/*============================================================================*/
/*! CForecastValue

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CForecastValue::CForecastValue(CWnd* pParent /*=NULL*/)
	: CDialogBase(CForecastValue::IDD, pParent)
	, m_bUpateView(FALSE)
	, m_bUpperSort(TRUE)
	, m_nSelectedColumn(0)
	, m_nPosEditCtrl(-1)
	, m_nStationIndex(-1)
	, m_strSelectedFilePath(_T(""))
{

}

/*============================================================================*/
/*! CForecastValue

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CForecastValue::~CForecastValue()
{
}

void CForecastValue::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FORECASTVALUE_LIST1, m_listForecastValue);
	DDX_Control(pDX, IDC_STATIC_FORECASTVALUE_GROUP_AREA, m_viewArea);
}


BEGIN_MESSAGE_MAP(CForecastValue, CDialogBase)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FORECASTVALUE_LIST1, &CForecastValue::OnLvnItemchangedForecastvalueList1)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CForecastValue::OnHdnItemclickForecastvalueList1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_FORECASTVALUE_LIST1, &CForecastValue::OnNMCustomdrawForecastvalueList1)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_FORECASTVALUE_LIST1, &CForecastValue::OnLvnEndlabeleditForecastvalueList1)
	ON_NOTIFY(NM_DBLCLK, IDC_FORECASTVALUE_LIST1, &CForecastValue::OnNMDblclkForecastvalueList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_FORECASTVALUE_LIST1, &CForecastValue::OnLvnColumnclickForecastvalueList1)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_FORECASTVALUE_LIST1, &CForecastValue::OnNMClickForecastvalueList1)
	ON_WM_VSCROLL()
	ON_NOTIFY(LVN_BEGINSCROLL, IDC_FORECASTVALUE_LIST1, &CForecastValue::OnLvnBeginScrollForecastvalueList1)
	ON_NOTIFY(HDN_ENDTRACK, 0, &CForecastValue::OnHdnEndtrackForecastvalueList1)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CForecastValue メッセージ ハンドラー


/*============================================================================*/
/*! CForecastValue

-# リストコントロールアイテム変更時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし
*/
/*============================================================================*/
void CForecastValue::OnLvnItemchangedForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	*pResult = 0;
}


/*============================================================================*/
/*! CForecastValue

-# ダイアログ初期化処理

@param  なし
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
BOOL CForecastValue::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_strSelectedFilePath = _T("");

	// リスト初期化
	if (!InitList())
		return FALSE;

	// リストアイテム挿入
	if (!InsertListItem())
		return FALSE;

	// グループビュー生成
	if (!CreateGroupView())
		return FALSE;

	//常にスクロールバーを表示させる
	m_listForecastValue.EnableScrollBarCtrl(SB_VERT, TRUE);

	UpdateData(FALSE);
	UpdateWindow();

	m_listForecastValue.RedrawWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CForecastValue

-# リストコントロール初期化

@param	なし
@retval	TRUE:初期化成功 / FALSE:初期化失敗

*/
/*============================================================================*/
BOOL CForecastValue::InitList()
{
	// リスト項目初期化
	LVCOLUMN lvc;
	TCHAR szMsg[256];

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_FMT;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_STATIONNAME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 0;
	lvc.pszText = szMsg;
	lvc.cx = m_nStationNameWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_listForecastValue.InsertColumn(0, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_SATELLITENAME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 1;
	lvc.pszText = szMsg;
	lvc.cx = m_nSatelliteNameWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_listForecastValue.InsertColumn(1, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_PASSID, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 2;
	lvc.pszText = szMsg;
	lvc.cx = m_nPassIDWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_listForecastValue.InsertColumn(2, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_DATETIME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 3;
	lvc.pszText = szMsg;
	lvc.cx = m_nDateWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_listForecastValue.InsertColumn(3, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_AZ, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 4;
	lvc.pszText = szMsg;
	lvc.cx = m_nAZWidth;
	lvc.fmt = LVCFMT_RIGHT;
	if (m_listForecastValue.InsertColumn(4, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_EL, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 5;
	lvc.pszText = szMsg;
	lvc.cx = m_nAZWidth;
	lvc.fmt = LVCFMT_RIGHT;
	if (m_listForecastValue.InsertColumn(5, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_DATETIME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 6;
	lvc.pszText = szMsg;
	lvc.cx = m_nDateWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_listForecastValue.InsertColumn(6, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_AZ, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 7;
	lvc.pszText = szMsg;
	lvc.cx = m_nAZWidth;
	lvc.fmt = LVCFMT_RIGHT;
	if (m_listForecastValue.InsertColumn(7, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_EL, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 8;
	lvc.pszText = szMsg;
	lvc.cx = m_nAZWidth;
	lvc.fmt = LVCFMT_RIGHT;
	if (m_listForecastValue.InsertColumn(8, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_DATETIME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 9;
	lvc.pszText = szMsg;
	lvc.cx = m_nDateWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_listForecastValue.InsertColumn(9, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_AZ, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 10;
	lvc.pszText = szMsg;
	lvc.cx = m_nAZWidth;
	lvc.fmt = LVCFMT_RIGHT;
	if (m_listForecastValue.InsertColumn(10, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_EL, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 11;
	lvc.pszText = szMsg;
	lvc.cx = m_nAZWidth;
	lvc.fmt = LVCFMT_RIGHT;
	if (m_listForecastValue.InsertColumn(11, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_OFFSET, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 12;
	lvc.pszText = szMsg;
	lvc.cx = m_nOffsetWidth;
	lvc.fmt = LVCFMT_RIGHT;
	if (m_listForecastValue.InsertColumn(12, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_FORMAT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 13;
	lvc.pszText = szMsg;
	lvc.cx = m_nFormatWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_listForecastValue.InsertColumn(13, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_GETFROM, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 14;
	lvc.pszText = szMsg;
	lvc.cx = m_nGetStateWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_listForecastValue.InsertColumn(14, &lvc) < 0)
		return FALSE;

	m_listForecastValue.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	return TRUE;
}

/*============================================================================*/
/*! CForecastValue

-# リストコントロールにアイテム挿入

@param	なし
@retval	TRUE:挿入成功 / FALSE:挿入失敗

*/
/*============================================================================*/
BOOL CForecastValue::InsertListItem()
{
	CForecastList* pWnd = (CForecastList*)GetParent();
	if (pWnd == NULL)
		return FALSE;

	CGraphMutex::Lock(eFile);
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();

	SetStationIndex();
	SetSatelliteName();

	m_listForecastValue.DeleteAllItems();

	// 背景色をセット
	map<CString, COLORREF>	mapColor;
	int sz = sizeof(mStationSimpleString) / sizeof(mStationSimpleString[0]);
	for (int i = 0; i < sz; i++)
	{
//		mapColor.insert(map<CString, COLORREF>::value_type(mStationSimpleString[i], mStationColor[i]));
		mapColor.insert(map<CString, COLORREF>::value_type(mStationString[i], mStationColor[i]));
	}
	m_listForecastValue.SetColorMap(mapColor);

	// リストコントロールにアイテム挿入
	LVITEM lvi;
	CString str;
	int i, index;
	CTime ctime;

	for (i = 0, index = 0; i < (int)listPred.size(); i++)
	{
		// 局名がALLまたは一致のときにリストに追加
//		str.Format(mStationSimpleString[m_nStationIndex]);
//		str = pWnd->GetSimpleStationName();
//		if (!(m_strStationName.CompareNoCase(_T("All")) == 0 || str.Compare(listPred[i].strStationName) == 0))
		if (!(m_strStationName.CompareNoCase(_T("All")) == 0 || m_strStationName.Compare(listPred[i].strStationName) == 0))
			continue;

		// 衛星名がALLまたは一致のときにリストに追加
		if (!(m_strSatelliteName.CompareNoCase(_T("All")) == 0 ||
			m_strSatelliteName.CompareNoCase(listPred[i].strSatellite) == 0))
			continue;

		// リストコントロールに内容セット
		lvi.mask = LVIF_TEXT;

		lvi.iItem = i;
		lvi.iSubItem = 0;
		str.Format(listPred[i].strStationName);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if ((index = m_listForecastValue.InsertItem(&lvi)) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 1;
		str.Format(listPred[i].strSatellite);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 2;
		str.Format(listPred[i].strPassId);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 3;
		ctime = listPred[i].tAOS;
		str = ctime.Format("%Y-%m-%d %H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 4;
		str.Format(_T("%03.2f"), listPred[i].dAosAz);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 5;
		str.Format(_T("%03.2f"), listPred[i].dAosEl);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 6;
		ctime = listPred[i].tLOS;
		str = ctime.Format("%Y-%m-%d %H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 7;
		str.Format(_T("%03.2f"), listPred[i].dLosAz);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 8;
		str.Format(_T("%03.2f"), listPred[i].dLosEl);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 9;
		ctime = listPred[i].tMaxEl;
		str = ctime.Format("%Y-%m-%d %H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 10;
		str.Format(_T("%03.2f"), listPred[i].dMaxAz);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 11;
		str.Format(_T("%03.2f"), listPred[i].dMaxEl);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 12;
		long nOffset = labs(listPred[i].tOffset);
		if (listPred[i].tOffset >= 0)
		{
			str.Format(_T("%02d:%02d"), nOffset / 60, nOffset % 60);
		}
		else
		{
			str.Format(_T("-%02d:%02d"), nOffset / 60, nOffset % 60);
		}

		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 13;
		str.Format(listPred[i].strFormat);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 14;
		str.Format(listPred[i].strGetFrom);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}
	}
	CGraphMutex::Unlock(eFile);

	for (int i = 0; i < m_listForecastValue.GetItemCount(); i++) {
		m_listForecastValue.SetItemData(i, i);
	}

	return TRUE;
}

/*============================================================================*/
/*! CForecastValue

-# リストコントロールのアイテムを更新する

@param	なし
@retval	TRUE:更新成功 / FALSE:更新失敗

*/
/*============================================================================*/
BOOL CForecastValue::UpdateListItem()
{
	int nItemCount = m_listForecastValue.GetItemCount();
	CRect rect;
	m_listForecastValue.GetItemRect(0, &rect, LVIR_BOUNDS);
	int index = m_listForecastValue.GetTopIndex();
	int offset = rect.Height() * index;
	CSize cs;
	cs.cx = 0;
	cs.cy = offset;

	// アイテム挿入
	InsertListItem();

	// アイテム数が変わっていない場合は、記録したスクロール位置にセットする
	if (nItemCount == m_listForecastValue.GetItemCount())
	{
		if (offset)
		{
			m_listForecastValue.Scroll(cs);
		}
	}

	CString strStation, strSatellite, strPassId, strPredPath;
	CGraphMutex::Lock(eFile);
	vector<stPredListData>::iterator itr;
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();

	BOOL bFind = FALSE;

	nItemCount = m_listForecastValue.GetItemCount();
	for (int i = 0; i < nItemCount && bFind == FALSE; i++)
	{
		strStation = m_listForecastValue.GetItemText(i, 0);
		strSatellite = m_listForecastValue.GetItemText(i, 1);
		strPassId = m_listForecastValue.GetItemText(i, 2);
		for (itr = listPred.begin(); itr != listPred.end() && bFind == FALSE; itr++)
		{
			if ((*itr).strStationName.Compare(strStation) == 0 &&
				(*itr).strSatellite.Compare(strSatellite) == 0 &&
				(*itr).strPassId.Compare(strPassId) == 0)
			{
				// 局、衛星、予報値IDが一致
				if (((*itr).strPredPath).CompareNoCase(m_strSelectedFilePath) == 0)
				{
					// ファイルパスが一致したら、リストコントロール選択
					if (m_listForecastValue.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED) == 0)
					{
						// リスト選択できなかった場合は、ファイルパスをクリア
						m_strSelectedFilePath = _T("");
					}

					bFind = TRUE;
				}
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	InitRadarGraphView();

	return TRUE;
}


/*============================================================================*/
/*! CForecastValue

-# グループのビュー作成

@param	なし
@retval	TRUE:挿入成功 / FALSE:挿入失敗

*/
/*============================================================================*/
BOOL CForecastValue::CreateGroupView()
{
	// ビューの作成
	CRect rect;
	m_viewArea.GetWindowRect(&rect);
	ScreenToClient(&rect);
	CRuntimeClass* pClass = RUNTIME_CLASS(CForecastValueGroupView);
	if (pClass == NULL)
	{
		return FALSE;
	}

	m_pView = (CForecastValueGroupView*)(pClass->CreateObject());
	if (m_pView != NULL)
	{
		m_pView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, 0, NULL);
		m_pView->SendMessage(WM_INITIALUPDATE);
	}

	return TRUE;
}


/*============================================================================*/
/*! CForecastValue

-# AOSグループのX位置取得

@param	stXPos：グループの開始位置、終了位置
@retval	なし

*/
/*============================================================================*/
void CForecastValue::GetAosXPos(structGroupXPos& stXPos)
{
	int xStart = 0;
	int xStop = 0;

	// AOSより前
	for (int i = 0; i < 3; ++i)
	{
		xStart += m_listForecastValue.GetColumnWidth(i);
	}
	
	// AOSの幅
	for (int i = 3; i <6; ++i)
	{
		xStop += m_listForecastValue.GetColumnWidth(i);
	}

	stXPos.x1 = xStart;
	stXPos.x2 = xStart + xStop;
}

/*============================================================================*/
/*! CForecastValue

-# LOSグループのX位置取得

@param	stXPos：グループの開始位置、終了位置
@retval	なし

*/
/*============================================================================*/
void CForecastValue::GetLosXPos(structGroupXPos& stXPos)
{
	structGroupXPos stStartXPos;
	int xStop = 0;

	// LOSより前
	GetAosXPos(stStartXPos);

	// LOSの幅
	for (int i = 6; i < 9; ++i)
	{
		xStop += m_listForecastValue.GetColumnWidth(i);
	}

	stXPos.x1 = stStartXPos.x2;
	stXPos.x2 = stStartXPos.x2 + xStop;
}

/*============================================================================*/
/*! CForecastValue

-# MAX ELグループのX位置取得

@param	stXPos：グループの開始位置、終了位置
@retval	なし

*/
/*============================================================================*/
void CForecastValue::GetMaxElXPos(structGroupXPos& stXPos)
{
	structGroupXPos stStartXPos;
	int xStop = 0;

	// MAX ELより前
	GetLosXPos(stStartXPos);

	// LOSの幅
	for (int i = 9; i < 12; ++i)
	{
		xStop += m_listForecastValue.GetColumnWidth(i);
	}

	stXPos.x1 = stStartXPos.x2;
	stXPos.x2 = stStartXPos.x2 + xStop;
}


/*============================================================================*/
/*! CForecastValue

-# リストコントロールソート時のコールバック関数

@param	lParam1		比較元アイテム
@param	lParam2		比較先アイテム
@param	lParamSort	ウィンドウハンドル
@retval 0: 一致, -1:比較元の順番が低い, 1:比較元の順番が高い

*/
/*============================================================================*/
int CALLBACK CForecastValue::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CForecastValue* dlg = (CForecastValue*)lParamSort;
	return dlg->CompareFunc(lParam1, lParam2);

}

/*============================================================================*/
/*! CForecastValue

-# リストコントロールソート時の比較関数

@param	lParam1		比較元アイテム
@param	lParam2		比較先アイテム
@retval 0: 一致, -1:比較元の順番が低い, 1:比較元の順番が高い

*/
/*============================================================================*/
int CForecastValue::CompareFunc(LPARAM lParam1, LPARAM lParam2)
{
	CString str[2];
	LPARAM param[2] = { lParam1, lParam2 };

	// リストコントロールから比較するアイテム取得
	for (int i = 0; i < 2; ++i)
	{
		LVFINDINFO fi = { 0 };
		fi.flags = LVFI_PARAM;
		fi.lParam = param[i];
		str[i] = m_listForecastValue.GetItemText(m_listForecastValue.FindItem(&fi), m_nSelectedColumn);
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
/*! CForecastValue

-# 描画時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CForecastValue::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	// 描画メッセージで CDialogBase::OnPaint() を呼び出さないでください。
}


/*============================================================================*/
/*! CForecastValue

-# 描画時に呼ばれ、指定コントロールの背景を塗りつぶすブラシのハンドルを返す

@param	pDC：子ウィンドウのディスプレイコンテキストのポインタ
@param	pWnd：色を格納されたコントロールのポインタ
@param	nCtrlColor：コントロールの種類
@retval	コントロールの背景を塗りつぶすブラシのハンドル

*/
/*============================================================================*/
HBRUSH CForecastValue::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
/*! CForecastValue

-# リストコントロールの項目クリック時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし
*/
/*============================================================================*/
void CForecastValue::OnHdnItemclickForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	if (phdr == NULL)
		return;

	m_nSelectedColumn = phdr->iItem;

//	CancelOffsetTime();

	for (int i = 0; i < m_listForecastValue.GetItemCount(); i++) {
		m_listForecastValue.SetItemData(i, i);
	}
	m_listForecastValue.SortItems(&CForecastValue::CompareFunc, (LPARAM)this);

	m_bUpperSort = !m_bUpperSort;

	*pResult = 0;
}


/*============================================================================*/
/*! CForecastValue

-# リストコントロールのカスタム描画時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし
*/
/*============================================================================*/
void CForecastValue::OnNMCustomdrawForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	if (m_pView == NULL)
		return;

	m_pView->Invalidate();

	*pResult = 0;
}

/*============================================================================*/
/*! CForecastValue

-# 局名が変更されている場合に局名をセット

@param	なし
@retval TRUE: 変更されている。 FALSE: 変更されていない。

*/
/*============================================================================*/
BOOL CForecastValue::SetStationName()
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
/*! CForecastValue

-# 局インデックスをセット

@param	なし
@retval TRUE: 取得成功。 FALSE: 取得失敗。

*/
/*============================================================================*/
BOOL CForecastValue::SetStationIndex()
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
/*! CForecastValue

-# 衛星名が変更されている場合に衛星名をセット

@param	なし
@retval TRUE: 変更されている。 FALSE: 変更されていない。

*/
/*============================================================================*/
BOOL CForecastValue::SetSatelliteName()
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


/*============================================================================*/
/*! CForecastValue

-# 選択されている予報値リストの一番最初のファイルパスを返す

@param	strFilePath：ファイルパス
@param	bChk：TRUE:選択されているかチェックする / FALSE:チェックしない
@retval TRUE: 返す。 FALSE: 返さない。

*/
/*============================================================================*/
BOOL CForecastValue::GetSelectedForecastFilePath(CString& strFilePath, BOOL bChk /*= FALSE*/)
{
	POSITION pos;
	int nItem;
	CString strStationName, strSatellite, strPassId;

	pos = m_listForecastValue.GetFirstSelectedItemPosition();
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
	vector<stPredListData>::iterator itr;
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();

	while (pos)
	{
		nItem = m_listForecastValue.GetNextSelectedItem(pos);
		strStationName = m_listForecastValue.GetItemText(nItem, 0);
		strSatellite = m_listForecastValue.GetItemText(nItem, 1);
		strPassId = m_listForecastValue.GetItemText(nItem, 2);
		for (itr = listPred.begin(); itr != listPred.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationName) == 0 &&
				(*itr).strSatellite.Compare(strSatellite) == 0 &&
				(*itr).strPassId.Compare(strPassId) == 0)
			{
				strFilePath = ((*itr).strPredPath);
				// 拡張子がprdの場合、anpに置き換える
				strFilePath.TrimRight(PRED_FILE_TYPE);
				strFilePath += ANP_FILE_TYPE;

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

-# 選択されている予報値リストの一番最初の局名とファイルパスを返す

@param	strStationName：局名
@param	strFilePath：ファイルパス
@param	nFormat：0 近宇宙/ 1 深宇宙
@param	bChk：TRUE:選択されているかチェックする / FALSE:チェックしない
@retval TRUE: 返す。 FALSE: 返さない。

*/
/*============================================================================*/
BOOL CForecastValue::GetSelectedForecastStationNameAndFilePath(CString& strStationName, CString& strFilePath, int& nFormat, BOOL bChk /*= FALSE*/)
{
	POSITION pos;
	int nItem;
	CString strStationNameTmp, strSatellite, strPassId;

	pos = m_listForecastValue.GetFirstSelectedItemPosition();
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
	vector<stPredListData>::iterator itr;
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();

	CString strFormat = _T("");
	while (pos)
	{
		nItem = m_listForecastValue.GetNextSelectedItem(pos);
		strStationNameTmp = m_listForecastValue.GetItemText(nItem, 0);
		strSatellite = m_listForecastValue.GetItemText(nItem, 1);
		strPassId = m_listForecastValue.GetItemText(nItem, 2);
		strFormat = m_listForecastValue.GetItemText(nItem, 13);
		for (itr = listPred.begin(); itr != listPred.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationNameTmp) == 0 &&
				(*itr).strSatellite.Compare(strSatellite) == 0 &&
				(*itr).strPassId.Compare(strPassId) == 0)
			{
				strStationName = ((*itr).strStationName);
				CString strFileName = (*itr).strPassId;
				strFileName.Remove('-');
				strFilePath.Format(_T("%s\\%s.%s"), (LPCTSTR)(*itr).strSatellite, (LPCTSTR)strFileName, PRED_FILE_TYPE);

				if (strFormat.CompareNoCase(_T("深宇宙")) == 0)
				{
					nFormat = 1;
				}
				else
				{
					nFormat = 0;
				}

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

-# 選択されている予報値リストの一番最初の局名、衛星名、パスID、フルファイルパスを返す

@param	strStationName：局名
@param	strSatelliteName：衛星名
@param	strPassId：パスID
@param	strFilePath：フルファイルパス
@param	bChk：TRUE:選択されているかチェックする / FALSE:チェックしない
@retval TRUE: 返す。 FALSE: 返さない。

*/
/*============================================================================*/
BOOL CForecastValue::GetSelectedForecastStationNameAndFullFilePath(CString& strStationName, CString& strSatelliteName, CString& strPassId, CString& strFilePath, BOOL bChk /*= FALSE*/)
{
	POSITION pos;
	int nItem;
	CString strStationNameTmp, strSatelliteTmp, strPassIdTmp;

	pos = m_listForecastValue.GetFirstSelectedItemPosition();
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
	vector<stPredListData>::iterator itr;
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();

	while (pos)
	{
		nItem = m_listForecastValue.GetNextSelectedItem(pos);
		strStationNameTmp = m_listForecastValue.GetItemText(nItem, 0);
		strSatelliteTmp = m_listForecastValue.GetItemText(nItem, 1);
		strPassIdTmp = m_listForecastValue.GetItemText(nItem, 2);
		for (itr = listPred.begin(); itr != listPred.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationNameTmp) == 0 &&
				(*itr).strSatellite.Compare(strSatelliteTmp) == 0 &&
				(*itr).strPassId.Compare(strPassIdTmp) == 0)
			{
				strStationName = ((*itr).strStationName);
				strSatelliteName = ((*itr).strSatellite);
				strPassId = ((*itr).strPassId);
				strFilePath = ((*itr).strPredPath);
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

-# 選択されている予報値リストの一番最初の局名、衛星名、パスID、フルファイルパスを返す

@param	strStationName：局名
@param	strSatelliteName：衛星名
@param	strPassId：パスID
@param	strFilePath：フルファイルパス
@param	bChk：TRUE:選択されているかチェックする / FALSE:チェックしない
@retval TRUE: 返す。 FALSE: 返さない。

*/
/*============================================================================*/
BOOL CForecastValue::GetSelectedForecastStationNameAndFullFilePath(vector<stPredListData>& listPred, BOOL bChk /*= FALSE*/)
{
	POSITION pos;
	int nItem;
	CString strStationNameTmp, strSatelliteTmp, strPassIdTmp;

	BOOL bRet = FALSE;

	pos = m_listForecastValue.GetFirstSelectedItemPosition();
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
	vector<stPredListData>::iterator itr;
	vector<stPredListData>& srcListPred = theApp.GetSatelliteData().GetPredList();

	while (pos)
	{
		nItem = m_listForecastValue.GetNextSelectedItem(pos);
		strStationNameTmp = m_listForecastValue.GetItemText(nItem, 0);
		strSatelliteTmp = m_listForecastValue.GetItemText(nItem, 1);
		strPassIdTmp = m_listForecastValue.GetItemText(nItem, 2);
		for (itr = srcListPred.begin(); itr != srcListPred.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationNameTmp) == 0 &&
				(*itr).strSatellite.Compare(strSatelliteTmp) == 0 &&
				(*itr).strPassId.Compare(strPassIdTmp) == 0)
			{
				listPred.push_back((*itr));
				bRet = TRUE;
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	return bRet;
}


/*============================================================================*/
/*! CForecastValue

-# 選択されている予報値リストの一番最初のオフセット時刻を返す

@param	offsetTime：オフセット時刻
@retval TRUE: 返す。 FALSE: 返さない。

*/
/*============================================================================*/
BOOL CForecastValue::GetSelectedForecastOffsetTime(time_t& offsetTime)
{
	POSITION pos;
	int nItem;
	CString strStationNameTmp, strSatellite, strPassId;

	pos = m_listForecastValue.GetFirstSelectedItemPosition();
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
	vector<stPredListData>::iterator itr;
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();

	while (pos)
	{
		nItem = m_listForecastValue.GetNextSelectedItem(pos);
		strStationNameTmp = m_listForecastValue.GetItemText(nItem, 0);
		strSatellite = m_listForecastValue.GetItemText(nItem, 1);
		strPassId = m_listForecastValue.GetItemText(nItem, 2);
		for (itr = listPred.begin(); itr != listPred.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationNameTmp) == 0 &&
				(*itr).strSatellite.Compare(strSatellite) == 0 &&
				(*itr).strPassId.Compare(strPassId) == 0)
			{
				offsetTime = ((*itr).tOffset);
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

-# 予報値一覧から局名リストを取得する

@param	listName：局名リスト
@retval TRUE: 返す。 FALSE: 返さない。

*/
/*============================================================================*/
BOOL CForecastValue::GetStationNameList(vector<CString>& listName)
{
	CGraphMutex::Lock(eFile);
	vector<stPredListData>::iterator itr;
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();
	vector<CString>::iterator itrName;

	listName.clear();

	for (itr = listPred.begin(); itr != listPred.end(); itr++)
	{
		// 局名が既に局名リストにあるか調べる
		if (listName.empty())
		{
			listName.push_back((*itr).strStationName);
		}
		else
		{
			itrName = find(listName.begin(), listName.end(), (*itr).strStationName);
			if (itrName == listName.end())
			{
				// 無い場合は局名リストに追加
				listName.push_back((*itr).strStationName);
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	if (listName.empty() == TRUE)
		return FALSE;

	return TRUE;
}


/*============================================================================*/
/*! CForecastValue

-# 予報値一覧から衛星名リストを取得する

@param	listName：衛星名リスト
@retval TRUE: 返す。 FALSE: 返さない。

*/
/*============================================================================*/
BOOL CForecastValue::GetSatelliteNameList(vector<CString>& listName)
{
	CGraphMutex::Lock(eFile);
	vector<stPredListData>::iterator itr;
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();
	vector<CString>::iterator itrName;

	listName.clear();

	for (itr = listPred.begin(); itr != listPred.end(); itr++)
	{
		// 衛星名が既に衛星名リストにあるか調べる
		if (listName.empty())
		{
			listName.push_back((*itr).strSatellite);
		}
		else
		{
			itrName = find(listName.begin(), listName.end(), (*itr).strSatellite);
			if (itrName == listName.end())
			{
				// 無い場合は局名リストに追加
				listName.push_back((*itr).strSatellite);
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	if (listName.empty() == TRUE)
		return FALSE;

	return TRUE;
}


/*============================================================================*/
/*! CForecastValue

-# 追尾グラフを初期化する。

@param  なし
@retval なし

*/
/*============================================================================*/
void CForecastValue::InitRadarGraphView()
{
	// 選択されている予報値の一番上の局名とファイルパスを取得
	CString strStationName, strSatellite, strPassId, strFilePath;
	
	if (GetSelectedForecastStationNameAndFullFilePath(strStationName, strSatellite, strPassId, strFilePath))
	{
		// 選択されている場合、新しいファイルパス取得
		m_strSelectedFilePath = strFilePath;
		CString strPredFileName = strPassId;
		strPredFileName.Remove('-');
		strFilePath.Format(_T("%s\\%s.%s"), (LPCTSTR)strSatellite, (LPCTSTR)strPredFileName, PRED_FILE_TYPE);		// 予報値IDからWindows局管制サーバのファイルパスを作る
	}
	else
	{
		// 選択されていない場合
		m_strSelectedFilePath = _T("");
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

	CString strFullPath;
	if (strFilePath.IsEmpty() || nStation < 0)
	{
		// 未選択もしくはファイルが無い場合
		nStation = -1;
		strFullPath = _T("");
	}
	else
	{
		strFullPath = theApp.GetShareFilePath(eFileType_Pred_Folder, nStation) + strFilePath;
	}
	// 予報値の追尾グラフにする
	pView->SetViewtype(eRadarViewType_ForecastList);
	pView->SetStationID(nStation);
	pView->SetPlanID(_T(""), 0, 0);		// 予報値一覧なので計画の情報なし
	// 追尾グラフを初期化する
	pView->Init(strFullPath, strSatellite, strPassId);
}


/*============================================================================*/
/*! CForecastValue

-# リストコントロールの項目編集終了に呼ばれる。

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし

*/
/*============================================================================*/
void CForecastValue::OnLvnEndlabeleditForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	*pResult = 0;
}


/*============================================================================*/
/*! CForecastValue

-# リストコントロールをクリック時に呼ばれる。

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし

*/
/*============================================================================*/
void CForecastValue::OnNMClickForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	InitRadarGraphView();
}


/*============================================================================*/
/*! CForecastValue

-# リストコントロールをダブルクリック時に呼ばれる。

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし

*/
/*============================================================================*/
void CForecastValue::OnNMDblclkForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult)
{

}


/*============================================================================*/
/*! CForecastValue

-# リストコントロールの列をクリック時に呼ばれる。

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし

*/
/*============================================================================*/
void CForecastValue::OnLvnColumnclickForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;
}


/*============================================================================*/
/*! CForecastValue

-# メッセージがディスパッチされる前に呼ばれる。

@param  pMsg：通知メッセージポインタ
@retval なし

*/
/*============================================================================*/
BOOL CForecastValue::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if (pMsg == NULL)
		return FALSE;

	return CDialogBase::PreTranslateMessage(pMsg);
}


/*============================================================================*/
/*! CForecastValue

-# ウィンドウ破棄時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CForecastValue::OnDestroy()
{
	CDialogBase::OnDestroy();
}


/*============================================================================*/
/*! CForecastValue

-# スクロール開始時に呼ばれる。

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし

*/
/*============================================================================*/
void CForecastValue::OnLvnBeginScrollForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// この機能は Internet Explorer 5.5 かそれ以降のバージョンを必要とします。
	// シンボル _WIN32_IE は >= 0x0560 にならなければなりません。
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);
	// 時刻オフセット入力をクリア
//	CancelOffsetTime();

	*pResult = 0;
}


/*============================================================================*/
/*! CForecastValue

-# 時刻オフセットコントロールがフォーカスを失った時の処理

@param  message	：メッセージの識別子
@param  wParam	：メッセージの最初のパラメータ
@param  lParam	：メッセージの 2 番目のパラメータ
@retval 処理結果
*/
/*============================================================================*/
LRESULT CForecastValue::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CDialogBase::WindowProc(message, wParam, lParam);
}


/*============================================================================*/
/*! CForecastValue

-# 区分線トラック終了時に呼ばれる。

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし

*/
/*============================================================================*/
void CForecastValue::OnHdnEndtrackForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	if (m_pView == NULL)
		return;

	m_pView->Invalidate();

	*pResult = 0;
}

