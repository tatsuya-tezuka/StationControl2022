/*============================================================================*/
/*! CRangePropagationStatusMonitor.cpp

-# レンジ伝搬状況モニタ画面
*/
/*============================================================================*/
// CRangePropagationStatusMonitor.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "RangePropagationStatusMonitor.h"
#include "RangePropagationView.h"
#include "afxdialogex.h"

#ifdef _DEBUG_RNGPROPDUMMY
UINT mRngAreaDummySelect = 2;
#endif /* _DEBUG_RNGPROPDUMMY */

// CRangePropagationStatusMonitor ダイアログ

IMPLEMENT_DYNAMIC(CRangePropagationStatusMonitor, CDialogBase)

/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CRangePropagationStatusMonitor::CRangePropagationStatusMonitor(CWnd* pParent /*=NULL*/)
	: CDialogBase(CRangePropagationStatusMonitor::IDD, pParent)
	, m_pView(NULL)
{
}

/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CRangePropagationStatusMonitor::~CRangePropagationStatusMonitor()
{
}

void CRangePropagationStatusMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RANGEPROPAGATION_AREA, m_viewArea);
	DDX_Control(pDX, IDC_RANGEPROP_LIST, m_detailList);
}


BEGIN_MESSAGE_MAP(CRangePropagationStatusMonitor, CDialogBase)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CRangePropagationStatusMonitor::OnMenuClose)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CRangePropagationStatusMonitor メッセージ ハンドラー


/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# ダイアログ初期化処理

@param  なし
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
BOOL CRangePropagationStatusMonitor::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// ビューの作成
	CRect rect;
	m_viewArea.GetWindowRect(&rect);
	ScreenToClient(&rect);
	CRuntimeClass* pClass = RUNTIME_CLASS(CRangePropagationView);
	if (pClass == NULL)
	{
		return FALSE;
	}

	m_pView = (CRangePropagationView*)(pClass->CreateObject());
	if (m_pView != NULL)
	{
		m_pView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, 0, NULL);
		m_pView->SendMessage(WM_INITIALUPDATE);
	}

	InitStatus();

	// リスト初期化
	if (!InitList())
		return FALSE;

	// リストアイテム挿入
	if (!InsertListItem())
		return FALSE;

	//常にスクロールバーを表示させる
	m_detailList.EnableScrollBarCtrl(SB_VERT, TRUE);

	UpdateWindow();

	SetTimer(1, 1000, NULL);

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# リストコントロール初期化

@param	なし
@retval	TRUE:初期化成功 / FALSE:初期化失敗

*/
/*============================================================================*/
BOOL CRangePropagationStatusMonitor::InitList()
{
	// リスト項目初期化
	LVCOLUMN lvc;

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_FMT;

	lvc.iSubItem = 0;
	lvc.pszText = _T("No.");
	lvc.cx = m_nNoWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_detailList.InsertColumn(0, &lvc) < 0)
		return FALSE;

	lvc.iSubItem = 1;
	lvc.pszText = _T("RANGE NUMBER");
	lvc.cx = m_nRngNumWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_detailList.InsertColumn(1, &lvc) < 0)
		return FALSE;

	lvc.iSubItem = 2;
	lvc.pszText = _T("Tx TIME");
	lvc.cx = m_nTxTimeWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_detailList.InsertColumn(2, &lvc) < 0)
		return FALSE;

	lvc.iSubItem = 3;
	lvc.pszText = _T("SC TIME");
	lvc.cx = m_nScTimeWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_detailList.InsertColumn(3, &lvc) < 0)
		return FALSE;

	lvc.iSubItem = 4;
	lvc.pszText = _T("Rx TIME");
	lvc.cx = m_nRxTimeWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_detailList.InsertColumn(4, &lvc) < 0)
		return FALSE;

	m_detailList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	return TRUE;
}

/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# リストコントロールにアイテム挿入

@param	なし
@retval	TRUE:挿入成功 / FALSE:挿入失敗

*/
/*============================================================================*/
BOOL CRangePropagationStatusMonitor::InsertListItem()
{
	CGraphMutex::Lock(eRngProp);

	// 伝搬データを取得
	CRangePropagation& rangePropagation = theApp.GetSatelliteData().GetRangePropagation();
	deque<CRangePropagation::structPropData>& dequeRngAll = rangePropagation.GetRngAllData();

	int dequeSize = (int)dequeRngAll.size();
	int oldListCtrlSize = m_detailList.GetItemCount();

	if (dequeSize == 0 && oldListCtrlSize == 0)
	{
		CGraphMutex::Unlock(eRngProp);
		return TRUE;
	}

	if (dequeSize == 0)
	{
		m_detailList.DeleteAllItems();
		CGraphMutex::Unlock(eRngProp);
		return TRUE;
	}

	if (dequeSize < oldListCtrlSize)
	{
		CGraphMutex::Unlock(eRngProp);
		return FALSE;
	}

	// レンジトーンリストの先頭インデックスで、リストコントロールの先頭から検索し、
	// レンジトーンリストから削除された分をリストコントロールから削除する
	CString strSearchCount;
	strSearchCount.Format(_T("%04u"), dequeRngAll.front().count);
	CString strCtrlCount;
	int nDelCount = 0;
	for (int i = 0; i < oldListCtrlSize; i++)
	{
		strCtrlCount = m_detailList.GetItemText(i, 1);
//		TRACE("###### Search delete string, dequeRngAll[0]:%04u\n", dequeRngAll.front().count);

		if (strCtrlCount.CompareNoCase(strSearchCount) == 0)
		{
			// 先頭から調べたカウント値が一致の場合
//			TRACE("###### Hit delete string, dequeRngAll[0]:%04u\n", dequeRngAll.front().count);
			nDelCount = i;
			break;
		}
	}

	// リストコントロールから不要分を削除
	for (int i = 0; i < nDelCount && m_detailList.GetItemCount() > 0; i++)
	{
		m_detailList.DeleteItem(0);
	}

	// リストコントロールの次の位置からが追加位置となる
	int newListCtrlSize = m_detailList.GetItemCount();
	if (dequeSize < newListCtrlSize)
	{
		CGraphMutex::Unlock(eRngProp);
		return FALSE;
	}

	if (dequeSize == newListCtrlSize)
	{
		CGraphMutex::Unlock(eRngProp);
		return TRUE;
	}

	LVITEM lvi;
	CString str;
	int index = 0;
	CTime ctime;

	TRACE("###### dequeSize:%04d, oldListCtrlSize:%04d, newListCtrlSize:%04d, nDelCount:%04d, count[%04d]:%04u\n", dequeSize, oldListCtrlSize, newListCtrlSize, nDelCount, newListCtrlSize, dequeRngAll[newListCtrlSize].count);

	// レンジトーン増加分をリストコントロールに追加
	for (int i = newListCtrlSize; i < dequeSize; i++)
	{
		// リストコントロールに内容セット
		lvi.mask = LVIF_TEXT;

		lvi.iItem = i;
		lvi.iSubItem = 0;
//		str.Format(_T("%04d"), i + 1);
		str.Format(_T("%04u"), dequeRngAll[i].count);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if ((index = m_detailList.InsertItem(&lvi)) < 0){
			CGraphMutex::Unlock(eRngProp);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 1;
		str.Format(_T("%04u"), dequeRngAll[i].count);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_detailList.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eRngProp);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 2;
		ctime = dequeRngAll[i].tStart;
		str = ctime.Format("%H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_detailList.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eRngProp);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 3;
		ctime = dequeRngAll[i].tStart + (time_t)(dequeRngAll[i].rtt / 2);
		str = ctime.Format("%H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_detailList.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eRngProp);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 4;
		ctime = dequeRngAll[i].tStart + (time_t)(dequeRngAll[i].rtt);
		str = ctime.Format("%H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_detailList.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eRngProp);
			return FALSE;
		}
	}
	m_detailList.EnsureVisible(index, FALSE);

	CGraphMutex::Unlock(eRngProp);

	return TRUE;
}


/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# ファイルメニューの閉じるから呼び出される

@param	なし
@retval	なし

*/
/*============================================================================*/
void CRangePropagationStatusMonitor::OnMenuClose()
{
	//	OnOK();
	ShowWindow(SW_HIDE);
}


/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# タイマイベントで呼び出される

@param	nIDEvent：イベントID
@retval	なし

*/
/*============================================================================*/
void CRangePropagationStatusMonitor::OnTimer(UINT_PTR nIDEvent)
{
#ifdef _DEBUG_RNGPROPDUMMY
	theApp.GetSatelliteData().GetRangePropagation().CreateDummy();
#endif /* _DEBUG_RNGPROPDUMMY */

	// ステータス更新
	if (theApp.GetSelectStation() == eStation_Usuda642)
	{
		// 54m局
		UpdateStatus54();
	}
	else
	{
		// それ以外の局
		UpdateStatus();
	}

	// 詳細一覧作成
	InsertListItem();

	// ビュー更新
	m_pView->Invalidate();

	CDialogBase::OnTimer(nIDEvent);
}


/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# ウィンドウ破棄時に呼び出される

@param	なし
@retval	なし

*/
/*============================================================================*/
void CRangePropagationStatusMonitor::OnDestroy()
{
	CDialogBase::OnDestroy();

	KillTimer(1);
}

/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# ステータスの更新

@param
@retval

*/
/*============================================================================*/
void CRangePropagationStatusMonitor::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	UINT nStation = theApp.GetSelectStation();
	CRangePropagation& rangePropagation = theApp.GetSatelliteData().GetRangePropagation();
	UINT nTransMode = rangePropagation.GetTransMode();
	UINT nGeneMode = rangePropagation.GetGeneMode();
	UINT nStep = rangePropagation.GetStep();
	CTime& dataTime = rangePropagation.GetDataTime();
	const CString mXX_RNG_CNT = _T("CTRL.XX_RNG_GOT_CNT");
	const CString mRX_RNG_CNT = _T("CTRL.RX_RNG_GOT_CNT");

	int szControl = sizeof(mRngParam[nStation][nTransMode]) / sizeof(mRngParam[nStation][nTransMode][0]);
	for (int i = 0; i < szControl && mRngParam[nStation][nTransMode][i].id != 0; i++)
	{
		CString monitorString = mRngParam[nStation][nTransMode][i].nameK;
		CString str;
		stControlValue val;
		val.type = mRngParam[nStation][nTransMode][i].type;

		if (!monitorString.IsEmpty())
		{
			if (szControl && mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT8 && nTransMode != eRngTransMode_SS)
			{
				// RNG RCV COUNTの値表示でかつS/Sでない場合
				if (nGeneMode == eRngGeneration_Past)
				{
					// 従来型
					obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, mXX_RNG_CNT);
				}
				else
				{
					// 再生型
					obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, mRX_RNG_CNT);
				}
			}
			else
			{
				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, monitorString);
#ifdef _DEBUG_RNGPROPDUMMY
				obs_adr = new CEngValCnv::obsname_t;
				sprintf_s(obs_adr->sz_sts_name, 64, ("%s"), (LPCSTR)mRngDummy[i].status);
				obs_adr->l_data = (long)mRngDummy[i].val;
				obs_adr->d_data = mRngDummy[i].val;
				obs_adr->l_inflg = INFLG_ON;
#endif
			}

			if (obs_adr == NULL)
			{
				continue;
			}
		}
		else
		{
			obs_adr = NULL;
		}

		switch (val.type){
		case	eControlStatus:
			if (obs_adr != NULL)
				str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT1)
			{
				// ANP FILE
//				str = str.Right(str.GetLength() - str.ReverseFind(_T('/')) - 1);
				str = theApp.GetSatelliteData().GetTrendData().GetAnpFileName(nStation);
			}
			else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT6 && nTransMode != eRngTransMode_SS)
			{
				// RNG FORMAT
				str = _T("PN_CODE");
			}
			break;
		case	eControlNumeric:
			if (obs_adr != NULL)
				str = CSatelliteData::AddCanma((double)obs_adr->l_data, 3);
			break;
		case	eControlDouble:
		{
			int keta;
			if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT10 ||
				mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT11)
				// RANGE, RANGE RATE
				keta = 3;
			else
				keta = 0;
			if (obs_adr != NULL)
				str = CSatelliteData::AddCanma(obs_adr->d_data, keta);
			break;
		}
		case	eControlTime:
			if (obs_adr != NULL)
				str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		default:
			break;
		}

		// DATE TIME
		if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT3)
		{
			theApp.GetCurrentUtcTime(dataTime);
			str = dataTime.Format(_T("%j %H:%M:%S"));
		}
		// RNG NUM
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT5)
		{
//			nRngNum = _wtol(str);
		}
		// RNG SEND COUNT
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT7)
		{
			// ステップが4以下の場合
			if (nStep <= eRngStep_4)
			{
				// 何も表示しない
				str.Format(_T(" "));
			}
			else
			{
				UINT nSndCount = rangePropagation.GetSndCount();
				// 従来型か再生型か
				if (nGeneMode == eRngGeneration_Regene)
				{
					// 再生型の場合は、何も表示しない
					str.Format(_T(" "));
				}
				else
				{
					// 検出したトーンの数を表示
					str.Format(_T("%u"), nSndCount);
				}
			}
		}
		// RNG RCV COUNT
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT8)
		{
			// ステップが4以下の場合
			if (nStep <= eRngStep_4)
			{
				// 何も表示しない
				str.Format(_T(" "));
			}
		}
		// NEXT RCV TIME
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT9)
		{
			// 従来型か再生型か
			if (nGeneMode == eRngGeneration_Regene)
			{
				// 再生型の場合は、何も表示しない
				str.Format(_T(" "));
			}
			else
			{
				// 画面上に複数のトーンが表示されている場合、最も地上局に近いトーンの受信時刻を予報値から計算し表示
				// 受信時刻 = アンテナからの距離 / 光速　+ 現在時刻
				deque<CRangePropagation::structPropData>& dequeRngRcv = rangePropagation.GetRngRcvData();

				if (!dequeRngRcv.empty())
				{
					CTime nextRcvTime = (time_t)(dequeRngRcv[0].pos / V_C + (double)dataTime.GetTime());
					str = nextRcvTime.Format(_T("%H:%M:%S"));
				}
				else
				{
					str.Format(_T("  :  :  "));
				}
			}
		}

		// RTT
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT12)
		{
			double rtt = rangePropagation.GetRtt();
			if (rtt >= 0.0)
			{
				str = CSatelliteData::AddCanma(rtt, 9);
				str.Format(_T("%s"), (LPCTSTR)str);
			}
			else
			{
				str.Format(_T(" "));
			}
		}

		// 表示
		GetDlgItem(mRngParam[nStation][nTransMode][i].id)->SetWindowText(str);

#ifdef _DEBUG_RNGPROPDUMMY
		if (obs_adr != NULL)
			delete obs_adr;
#endif
	}
	UpdateData(FALSE);
}

#define CTRL_RNG_AREA_XX_PAST		"X/X(従来)"		// CTRL.RNG_AREAのX/X帯 従来型文字列
#define CTRL_RNG_AREA_XX_REGENE		"X/X(再生)"		// CTRL.RNG_AREAのX/X帯 再生型文字列
#define CTRL_RNG_AREA_XKA_PAST		"X/Ka(従来)"	// CTRL.RNG_AREAのX/Ka帯従来型文字列
#define CTRL_RNG_AREA_XKA_REGENE	"X/Ka(再生)"	// CTRL.RNG_AREAのX/Ka帯再生型文字列

/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# ステータスの更新(54m)

@param
@retval

*/
/*============================================================================*/
void CRangePropagationStatusMonitor::UpdateStatus54()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	UINT nStation = theApp.GetSelectStation();
	CRangePropagation& rangePropagation = theApp.GetSatelliteData().GetRangePropagation();
	// XかKaか調べる
	UINT nTransMode = eXmeasBandType_X;
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, JUDGE_BANDTYPE_STR);
	if (obs_adr != NULL)
	{
		CString str = CString(obs_adr->sz_sts_name);
		for (UINT i = 0; i < eXmeasBandType_Num; i++)
		{
			if (str.CompareNoCase(mBandTypeString[i]) == 0)
			{
				nTransMode = i;
				break;
			}
		}
	}

	UINT nGeneMode = rangePropagation.GetGeneMode();
	UINT nStep = rangePropagation.GetStep();
	CTime& dataTime = rangePropagation.GetDataTime();
	const CString mDX_RNG_CNT = _T("CTRL.DX_RNG_GOT_CNT");
	const CString mRX_RNG_CNT = _T("CTRL.RX_RNG_GOT_CNT");
	const CString mDKA_RNG_CNT = _T("CTRL.DKa_RNG_GOT_CNT");
	const CString mRKA_RNG_CNT = _T("CTRL.RKa_RNG_GOT_CNT");

#ifdef _DEBUG_RNGPROPDUMMY
	CEngValCnv::obsname_t obs_adr_debug;
#endif /* _DEBUG_RNGPROPDUMMY */

	int szControl = sizeof(mRngParam[nStation][nTransMode]) / sizeof(mRngParam[nStation][nTransMode][0]);
	for (int i = 0; i < szControl && mRngParam[nStation][nTransMode][i].id != 0; i++)
	{
		CString monitorString = mRngParam[nStation][nTransMode][i].nameK;
		CString str;
		stControlValue val;
		val.type = mRngParam[nStation][nTransMode][i].type;

		if (!monitorString.IsEmpty())
		{
			if (szControl && mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT8)
			{
				// RNG RCV COUNTの値表示の場合、XかKaか調べる
				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, monitorString);
#ifdef _DEBUG_RNGPROPDUMMY
//				obs_adr = &obs_adr_debug;
//				sprintf_s(obs_adr->sz_sts_name, 64, ("%s"), mRngAreaDummy[mRngAreaDummySelect]);
//				obs_adr->l_data = 0;
//				obs_adr->d_data = 0;
//				obs_adr->l_inflg = INFLG_ON;
#endif /* _DEBUG_RNGPROPDUMMY */
				if (obs_adr == NULL)
				{
					continue;
				}
				size_t len = strlen(obs_adr->sz_sts_name);
				if (len == 0)
				{
					continue;
				}
				if (len == strlen(CTRL_RNG_AREA_XX_PAST) && strncmp(obs_adr->sz_sts_name, CTRL_RNG_AREA_XX_PAST, len) == 0)
				{
					// X/X(従来)の場合
					obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, mDX_RNG_CNT);
#ifdef _DEBUG_RNGPROPDUMMY
//					obs_adr = &obs_adr_debug;
//					sprintf_s(obs_adr->sz_sts_name, 64, (""));
//					obs_adr->l_data = 0;
//					obs_adr->d_data = 12;
//					obs_adr->l_inflg = INFLG_ON;
#endif /* _DEBUG_RNGPROPDUMMY */
				}
				else if (len == strlen(CTRL_RNG_AREA_XX_REGENE) && strncmp(obs_adr->sz_sts_name, CTRL_RNG_AREA_XX_REGENE, len) == 0)
				{
					// X/X(再生)の場合
					obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, mRX_RNG_CNT);
#ifdef _DEBUG_RNGPROPDUMMY
//					obs_adr = &obs_adr_debug;
//					sprintf_s(obs_adr->sz_sts_name, 64, (""));
//					obs_adr->l_data = 0;
//					obs_adr->d_data = 34;
//					obs_adr->l_inflg = INFLG_ON;
#endif /* _DEBUG_RNGPROPDUMMY */
				}
				else if (len == strlen(CTRL_RNG_AREA_XKA_PAST) && strncmp(obs_adr->sz_sts_name, CTRL_RNG_AREA_XKA_PAST, len) == 0)
				{
					// X/Ka(従来)の場合
					obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, mDKA_RNG_CNT);
#ifdef _DEBUG_RNGPROPDUMMY
//					obs_adr = &obs_adr_debug;
//					sprintf_s(obs_adr->sz_sts_name, 64, (""));
//					obs_adr->l_data = 0;
//					obs_adr->d_data = 56;
//					obs_adr->l_inflg = INFLG_ON;
#endif /* _DEBUG_RNGPROPDUMMY */
				}
				else if (len == strlen(CTRL_RNG_AREA_XKA_REGENE) && strncmp(obs_adr->sz_sts_name, CTRL_RNG_AREA_XKA_REGENE, len) == 0)
				{
					// X/Ka(再生)の場合
					obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, mRKA_RNG_CNT);
#ifdef _DEBUG_RNGPROPDUMMY
//					obs_adr = &obs_adr_debug;
//					sprintf_s(obs_adr->sz_sts_name, 64, (""));
//					obs_adr->l_data = 0;
//					obs_adr->d_data = 78;
//					obs_adr->l_inflg = INFLG_ON;
#endif /* _DEBUG_RNGPROPDUMMY */
				}
				else
				{
					CString strDebug;
					strDebug.Format(_T("%s(%d) :: CTRL_RNG_AREA(%s) miss match"), __FILEW__, __LINE__, (LPCTSTR)monitorString);
					CLogTraceEx::Write(_T(""), _T("CRangePropagationStatusMonitor"), _T("UpdateStatus54"), _T("debug"), strDebug, nLogEx::debug);
					continue;
				}
			}
			else
			{
				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, monitorString);
#ifdef _DEBUG_RNGPROPDUMMY
				obs_adr = &obs_adr_debug;
				sprintf_s(obs_adr->sz_sts_name, 64, ("%s"), (LPCSTR)mRngDummy[i].status);
				obs_adr->l_data = (long)mRngDummy[i].val;
				obs_adr->d_data = mRngDummy[i].val;
				obs_adr->l_inflg = INFLG_ON;
#endif /* _DEBUG_RNGPROPDUMMY */
			}

			if (obs_adr == NULL)
			{
				continue;
			}
		}
		else
		{
			obs_adr = NULL;
		}

		switch (val.type){
		case	eControlStatus:
			if (obs_adr != NULL)
				str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT1)
			{
				// ANP FILE
				//				str = str.Right(str.GetLength() - str.ReverseFind(_T('/')) - 1);
				str = theApp.GetSatelliteData().GetTrendData().GetAnpFileName(nStation);
			}
			else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT6)
			{
				// RNG FORMAT
				str = _T("PN_CODE");
			}
			break;
		case	eControlNumeric:
			if (obs_adr != NULL)
				str = CSatelliteData::AddCanma((double)obs_adr->l_data, 3);
			break;
		case	eControlDouble:
		{
			int keta;
			if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT10 ||
				mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT11)
				// RANGE, RANGE RATE
				keta = 3;
			else
				keta = 0;
			if (obs_adr != NULL)
				str = CSatelliteData::AddCanma(obs_adr->d_data, keta);
			break;
		}
		case	eControlTime:
			if (obs_adr != NULL)
				str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		default:
			break;
		}

		// DATE TIME
		if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT3)
		{
			theApp.GetCurrentUtcTime(dataTime);
			str = dataTime.Format(_T("%j %H:%M:%S"));
		}
		// RNG NUM
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT5)
		{
			//			nRngNum = _wtol(str);
		}
		// RNG SEND COUNT
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT7)
		{
			// ステップが4以下の場合
			if (nStep <= eRngStep_4)
			{
				// 何も表示しない
				str.Format(_T(" "));
			}
			else
			{
				UINT nSndCount = rangePropagation.GetSndCount();
				// 従来型か再生型か
				if (nGeneMode == eRngGeneration_Regene)
				{
					// 再生型の場合は、何も表示しない
					str.Format(_T(" "));
				}
				else
				{
					// 検出したトーンの数を表示
					str.Format(_T("%u"), nSndCount);
				}
			}
		}
		// RNG RCV COUNT
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT8)
		{
			// ステップが4以下の場合
			if (nStep <= eRngStep_4)
			{
				// 何も表示しない
				str.Format(_T(" "));
			}
		}
		// NEXT RCV TIME
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT9)
		{
			// 従来型か再生型か
			if (nGeneMode == eRngGeneration_Regene)
			{
				// 再生型の場合は、何も表示しない
				str.Format(_T(" "));
			}
			else
			{
				// 画面上に複数のトーンが表示されている場合、最も地上局に近いトーンの受信時刻を予報値から計算し表示
				// 受信時刻 = アンテナからの距離 / 光速　+ 現在時刻
				deque<CRangePropagation::structPropData>& dequeRngRcv = rangePropagation.GetRngRcvData();

				if (!dequeRngRcv.empty())
				{
					CTime nextRcvTime = (time_t)(dequeRngRcv[0].pos / V_C + (double)dataTime.GetTime());
					str = nextRcvTime.Format(_T("%H:%M:%S"));
				}
				else
				{
					str.Format(_T("  :  :  "));
				}
			}
		}

		// RTT
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT12)
		{
			double rtt = rangePropagation.GetRtt();
			if (rtt >= 0.0)
			{
				str = CSatelliteData::AddCanma(rtt, 9);
				str.Format(_T("%s"), (LPCTSTR)str);
			}
			else
			{
				str.Format(_T(" "));
			}
		}

		// 表示
		GetDlgItem(mRngParam[nStation][nTransMode][i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# ステータス初期化

@param
@retval

*/
/*============================================================================*/
void CRangePropagationStatusMonitor::InitStatus()
{
	UINT nStation = theApp.GetSelectStation();
	UINT nTransMode = eRngTransMode_SS;

	int szControl = sizeof(mRngParam[nStation][nTransMode]) / sizeof(mRngParam[nStation][nTransMode][0]);
	for (int i = 0; i < szControl && mRngParam[nStation][nTransMode][i].id != 0; i++)
	{
		CString str;
		stControlValue val;
		val.type = mRngParam[nStation][nTransMode][i].type;

		switch (val.type){
		case	eControlStatus:
			str.Format(_T(""));
			break;
		case	eControlNumeric:
			str.Format(_T(""));
			break;
		case	eControlDouble:
			str.Format(_T(""));
			break;
		case	eControlTime:
			str.Format(_T(""));
			break;
		default:
			break;
		}

		if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT3)
			str.Format(_T("      :  :  "));
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT9)
			str.Format(_T("  :  :  "));

		// RNG SEND COUNT
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT7 ||
			mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT8)
		{
			// 何も表示しない
			str.Format(_T(" "));
		}

		if (str.IsEmpty())
			str.Format(_T(" "));
		GetDlgItem(mRngParam[nStation][nTransMode][i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}

