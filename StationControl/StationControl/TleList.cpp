/*============================================================================*/
/*! TleList.cpp

-# 予報値一覧画面　TLE一覧
*/
/*============================================================================*/
// TleList.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TleList.h"
#include "afxdialogex.h"

//----------------------------------------------------------------------
// ダミーデータ

static const CString strSampleSatelliteName[5] = {
	_T("ALL"),
	_T("GCOM-C"),
	_T("ALOS-2"),
	_T("HAYABUSA2"),
	_T("123456789012345")
};

static const CTleList::structListItem sampleListItem[13] =
{
	// 衛星名,               識別,           EPOCH,                        伝搬処理日時               
	{ _T("GCOM-C"),          CTleList::eNOM, CTime::GetCurrentTime() + 1,  CTime::GetCurrentTime() + 2  },
	{ _T("GCOM-C"),          CTleList::eNOM, CTime::GetCurrentTime() + 3,  CTime::GetCurrentTime() + 4  },
	{ _T("GCOM-C"),          CTleList::eOFF, CTime::GetCurrentTime() + 5,  CTime::GetCurrentTime() + 6  },
	{ _T("ALOS-2"),          CTleList::eNOM, CTime::GetCurrentTime() + 7,  CTime::GetCurrentTime() + 8  },
	{ _T("ALOS-2"),          CTleList::eNOM, CTime::GetCurrentTime() + 9,  CTime::GetCurrentTime() + 10 },
	{ _T("ALOS-2"),          CTleList::eNOM, CTime::GetCurrentTime() + 11, CTime::GetCurrentTime() + 12 },
	{ _T("ALOS-2"),          CTleList::eOFF, CTime::GetCurrentTime() + 13, CTime::GetCurrentTime() + 14 },
	{ _T("HAYABUSA2"),       CTleList::eOFF, CTime::GetCurrentTime() + 15, CTime::GetCurrentTime() + 16 },
	{ _T("HAYABUSA2"),       CTleList::eOFF, CTime::GetCurrentTime() + 17, CTime::GetCurrentTime() + 18 },
	{ _T("123456789012345"), CTleList::eNOM, CTime::GetCurrentTime() + 19, CTime::GetCurrentTime() + 20 },
	{ _T("123456789012345"), CTleList::eNOM, CTime::GetCurrentTime() + 21, CTime::GetCurrentTime() + 22 },
	{ _T("123456789012345"), CTleList::eNOM, CTime::GetCurrentTime() + 23, CTime::GetCurrentTime() + 24 },
	{ _T("123456789012345"), CTleList::eNOM, CTime::GetCurrentTime() + 25, CTime::GetCurrentTime() + 26 },
};

static const CString strShikibetsu[2] = {
	_T("NOM"),
	_T("OFF"),
};


// CTleList ダイアログ

IMPLEMENT_DYNAMIC(CTleList, CDialogBase)

/*============================================================================*/
/*! CTleList

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CTleList::CTleList(CWnd* pParent /*=NULL*/)
	: CDialogBase(CTleList::IDD, pParent)
	, m_bUpperSort(TRUE)
	, m_nSelectedColumn(0)
{

}

/*============================================================================*/
/*! CTleList

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CTleList::~CTleList()
{
}

void CTleList::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TLELIST_COMBO, m_CCBoxSatelliteName);
	DDX_Control(pDX, IDC_TLELIST_LIST1, m_listTle);
}


BEGIN_MESSAGE_MAP(CTleList, CDialogBase)
	ON_COMMAND(ID_MENU_CLOSE_RC2, &CTleList::OnMenuClose)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CTleList::OnHdnItemclickTlelistList1)
	ON_BN_CLICKED(IDC_TLELIST_BUTTON2, &CTleList::OnBnClickedTlelistButton2)
	ON_BN_CLICKED(IDC_TLELIST_BUTTON1, &CTleList::OnBnClickedTlelistButton1)
	ON_BN_CLICKED(IDC_TLELIST_BUTTON3, &CTleList::OnBnClickedTlelistButton3)
	ON_BN_CLICKED(IDC_TLELIST_BUTTON4, &CTleList::OnBnClickedTlelistButton4)
	ON_BN_CLICKED(IDC_TLELIST_BUTTON5, &CTleList::OnBnClickedTlelistButton5)
	ON_BN_CLICKED(IDC_TLELIST_BUTTON6, &CTleList::OnBnClickedTlelistButton6)
	ON_CBN_SELCHANGE(IDC_TLELIST_COMBO, &CTleList::OnCbnSelchangeTlelistCombo)
END_MESSAGE_MAP()


/*============================================================================*/
/*! CTleList

-# メニューからのファイルクローズ時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CTleList::OnMenuClose()
{
	OnOK();
}

// CTleList メッセージ ハンドラー

/*============================================================================*/
/*! CTleList

-# ダイアログ初期化処理

@param  なし
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
BOOL CTleList::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// サンプルデータ生成
	CreateSampleData();

	// コンボボックス初期化
	InitComboBox();

	// リスト初期化
	if (!InitList())
		return FALSE;

	// リストアイテム挿入
	if (!InsertListItem())
		return FALSE;

	//常にスクロールバーを表示させる
	m_listTle.EnableScrollBarCtrl(SB_VERT, TRUE);

	UpdateData(FALSE);

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! CTleList

-# サンプルデータ設定

@param	なし
@retval	なし

*/
/*============================================================================*/
void CTleList::CreateSampleData()
{
	const int itemNum = sizeof(sampleListItem) / sizeof(sampleListItem[0]);

	for (int i = 0; i < itemNum; i++)
	{
		m_dequeItem.push_back(sampleListItem[i]);
	}
}


/*============================================================================*/
/*! CTleList

-# コンボボックス初期化

@param	なし
@retval	なし

*/
/*============================================================================*/
void CTleList::InitComboBox()
{
	int sz = sizeof(strSampleSatelliteName) / sizeof(strSampleSatelliteName[0]);
	for (int i = 0; i < sz; ++i)
	{
		m_CCBoxSatelliteName.InsertString(-1, strSampleSatelliteName[i]);
	}
	m_CCBoxSatelliteName.SetCurSel(0);
}


/*============================================================================*/
/*! CTleList

-# リストコントロール初期化

@param	なし
@retval	TRUE:初期化成功 / FALSE:初期化失敗

*/
/*============================================================================*/
BOOL CTleList::InitList()
{
	LVCOLUMN lvc;
	TCHAR szMsg[256];

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_TLELIST_SATELLITENAME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 0;
	lvc.pszText = szMsg;
	lvc.cx = m_nSatelliteNameWidth;
	if (m_listTle.InsertColumn(0, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_TLELIST_SHIKIBETSU, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 1;
	lvc.pszText = szMsg;
	lvc.cx = m_nSikibetuWidth;
	if (m_listTle.InsertColumn(1, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_TLELIST_EPOCH, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 2;
	lvc.pszText = szMsg;
	lvc.cx = m_epochTimeWidth;
	if (m_listTle.InsertColumn(2, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_TLELIST_PROPPROCTIME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 3;
	lvc.pszText = szMsg;
	lvc.cx = m_propProcTimeWidth;
	if (m_listTle.InsertColumn(3, &lvc) < 0)
		return FALSE;

	m_listTle.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	return TRUE;
}

/*============================================================================*/
/*! CTleList

-# リストコントロールにアイテム挿入

@param	なし
@retval	TRUE:挿入成功 / FALSE:挿入失敗

*/
/*============================================================================*/
BOOL CTleList::InsertListItem()
{
	SetSatelliteName();

	m_listTle.DeleteAllItems();

	LVITEM lvi;
	CString str;
	int i, index;

	for (i = 0, index = 0; i < (int)m_dequeItem.size(); i++)
	{
		// 衛星名がALLまたは一致のときにリストに追加
		if (!(m_strSatelliteName.CompareNoCase(_T("All")) == 0 ||
			m_strSatelliteName.CompareNoCase(m_dequeItem[i].m_satelliteName) == 0))
			continue;

		lvi.mask = LVIF_TEXT;

		lvi.iItem = i;
		lvi.iSubItem = 0;
		str.Format(m_dequeItem[i].m_satelliteName);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if ((index = m_listTle.InsertItem(&lvi)) < 0)
			return FALSE;

		lvi.iItem = index;
		lvi.iSubItem = 1;
		str.Format(strShikibetsu[m_dequeItem[i].m_shikibetsu]);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listTle.SetItem(&lvi) < 0)
			return FALSE;

		lvi.iItem = index;
		lvi.iSubItem = 2;
		str = m_dequeItem[i].m_epochTime.Format("%Y-%m-%d %H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listTle.SetItem(&lvi) < 0)
			return FALSE;

		lvi.iItem = index;
		lvi.iSubItem = 3;
		str = m_dequeItem[i].m_propProcTime.Format("%Y-%m-%d %H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listTle.SetItem(&lvi) < 0)
			return FALSE;
	}

	for (int i = 0; i < m_listTle.GetItemCount(); i++) {
		m_listTle.SetItemData(i, i);
	}

	return TRUE;
}

/*============================================================================*/
/*! CTleList

-# リストコントロールのアイテムを更新する

@param	なし
@retval	TRUE:更新成功 / FALSE:更新失敗

*/
/*============================================================================*/
BOOL CTleList::UpdateListItem()
{
	int nItemCount = m_listTle.GetItemCount();
	CRect rect;
	m_listTle.GetItemRect(0, &rect, LVIR_BOUNDS);
	int index = m_listTle.GetTopIndex();
	int offset = rect.Height() * index;
	CSize cs;
	cs.cx = 0;
	cs.cy = offset;

	// アイテム挿入
	InsertListItem();

	// アイテム数が変わっていない場合は、記録したスクロール位置にセットする
	if (nItemCount == m_listTle.GetItemCount())
	{
		if (offset)
		{
			m_listTle.Scroll(cs);
		}
	}

	return TRUE;
}


/*============================================================================*/
/*! CTleList

-# リストコントロールソート時のコールバック関数

@param	lParam1：		比較元アイテム
@param	lParam2：		比較先アイテム
@param	lParamSort：	ウィンドウハンドル
@retval 0: 一致, -1:比較元の順番が低い, 1:比較元の順番が高い

*/
/*============================================================================*/
int CALLBACK CTleList::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CTleList* dlg = (CTleList*)lParamSort;
	return dlg->CompareFunc(lParam1, lParam2);

}

/*============================================================================*/
/*! CTleList

-# リストコントロールソート時の比較関数

@param	lParam1：		比較元アイテム
@param	lParam2：		比較先アイテム
@retval 0: 一致, -1:比較元の順番が低い, 1:比較元の順番が高い

*/
/*============================================================================*/
int CTleList::CompareFunc(LPARAM lParam1, LPARAM lParam2)
{
	CString str[2];
	LPARAM param[2] = { lParam1, lParam2 };

	// リストコントロールから比較するアイテム取得
	for (int i = 0; i < 2; ++i)
	{
		LVFINDINFO fi = { 0 };
		fi.flags = LVFI_PARAM;
		fi.lParam = param[i];
		str[i] = m_listTle.GetItemText(m_listTle.FindItem(&fi), m_nSelectedColumn);
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
/*! CTleList

-# 衛星名取得

@param	なし
@retval	衛星名

*/
/*============================================================================*/
CString CTleList::GetSatelliteName()
{
	CString str;
	UpdateData(TRUE);
	int nIndex = m_CCBoxSatelliteName.GetCurSel();
	if (nIndex < 0)
		return str;
	m_CCBoxSatelliteName.GetLBText(nIndex, str);

	return str;
}

/*============================================================================*/
/*! CTleList

-# 衛星名が変更されている場合に衛星名をセット

@param	なし
@retval TRUE: 変更されている。 FALSE: 変更されていない。

*/
/*============================================================================*/
BOOL CTleList::SetSatelliteName()
{
	CString str = GetSatelliteName();
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
/*! CTleList

-# 選択されているTLE一覧を削除する

@param	なし
@retval TRUE: 削除した。 FALSE: 削除しない。

*/
/*============================================================================*/
BOOL CTleList::DeleteListItem()
{
	POSITION pos;
	int nItem;
	int number;
	deque<structListItem>::iterator itr;
	BOOL ret = FALSE;

	while (TRUE)
	{
		pos = m_listTle.GetFirstSelectedItemPosition();
		if (!pos)
			break;

		nItem = m_listTle.GetNextSelectedItem(pos);
		m_listTle.DeleteItem(nItem);
		for (itr = m_dequeItem.begin(), number = 0; itr != m_dequeItem.end(); itr++, number++)
		{
			if (number == nItem){
				m_dequeItem.erase(itr);
				ret = TRUE;
				break;
			}
		}
	}

	UpdateData(FALSE);
	UpdateWindow();

	return ret;
}


/*============================================================================*/
/*! CTleList

-# リストコントロールの項目クリック時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし
*/
/*============================================================================*/
void CTleList::OnHdnItemclickTlelistList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	if (phdr == NULL)
		return;

	m_nSelectedColumn = phdr->iItem;

	for (int i = 0; i < m_listTle.GetItemCount(); i++) {
		m_listTle.SetItemData(i, i);
	}
	m_listTle.SortItems(&CTleList::CompareFunc, (LPARAM)this);

	m_bUpperSort = !m_bUpperSort;

	*pResult = 0;
}


/*============================================================================*/
/*! CTleList

-# 伝播実行ボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CTleList::OnBnClickedTlelistButton1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


/*============================================================================*/
/*! CTleList

-# 削除ボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CTleList::OnBnClickedTlelistButton2()
{
	// TLE一覧からアイテムを削除する
	DeleteListItem();
}


/*============================================================================*/
/*! CTleList

-# 取り込みボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CTleList::OnBnClickedTlelistButton3()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


/*============================================================================*/
/*! CTleList

-# 詳細表示ボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CTleList::OnBnClickedTlelistButton4()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


/*============================================================================*/
/*! CTleList

-# 詳細印刷ボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CTleList::OnBnClickedTlelistButton5()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


/*============================================================================*/
/*! CTleList

-# 一覧印刷ボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CTleList::OnBnClickedTlelistButton6()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


/*============================================================================*/
/*! CTleList

-# コンボボックス選択変更時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CTleList::OnCbnSelchangeTlelistCombo()
{
	// 再描画
	UpdateListItem();
}

