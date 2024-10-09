// TimeLineView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "TimeLineView.h"

#include "ChildFrm.h"
#include "MainFrm.h"

static bool mChildMinSize = false;

// CTimeLineView

IMPLEMENT_DYNCREATE(CTimeLineView, CFormView)

CTimeLineView::CTimeLineView()
: CFormView(CTimeLineView::IDD)
, m_nTimeScal(0)
, m_bAutoScrolOff(FALSE)
, m_strPrePass(_T("--- --:--:--"))
{
	mSaveCx = mSaveCy = 0;
	mNextPrePassTime = 0;
	mTimeRate = 1;
	mSelectBase = 0;
	mFirst = true;
	for (int i = 0; i < eChange_Max; i++){
		mCheckTime[i] = CTime(0);
	}
}

CTimeLineView::~CTimeLineView()
{
}

void CTimeLineView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_TIMESCALE, m_nTimeScal);
	DDX_Check(pDX, IDC_CHECK_AUTOSCROL, m_bAutoScrolOff);
	DDX_Control(pDX, IDC_BUTTON_FORWARD, m_btForward);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_btNext);
	DDX_Control(pDX, IDC_CHECK_LEGEND1, m_btCheck[0]);
	DDX_Control(pDX, IDC_CHECK_LEGEND2, m_btCheck[1]);
	DDX_Control(pDX, IDC_CHECK_LEGEND3, m_btCheck[2]);
	DDX_Control(pDX, IDC_CHECK_LEGEND4, m_btCheck[3]);
	DDX_Control(pDX, IDC_CHECK_LEGEND5, m_btCheck[4]);
	DDX_Control(pDX, IDC_CHECK_LEGEND6, m_btCheck[5]);
	DDX_Control(pDX, IDC_CHECK_LEGEND7, m_btCheck[6]);
	DDX_Control(pDX, IDC_CHECK_LEGEND8, m_btCheck[7]);
	DDX_Control(pDX, IDC_CHECK_LEGEND9, m_btCheck[8]);
	DDX_Control(pDX, IDC_CHECK_LEGEND10, m_btCheck[9]);
	DDX_Control(pDX, IDC_CHECK_LEGEND11, m_btCheck[10]);
	DDX_Control(pDX, IDC_CHECK_LEGEND12, m_btCheck[11]);
	DDX_Control(pDX, IDC_STATIC_NOTES, m_grpNotes);
	DDX_Text(pDX, IDC_STATIC_NEXTPREPASS, m_strPrePass);
	DDX_Control(pDX, IDC_STATIC_NEXTPREPASS, m_stNextPass);
	DDX_Control(pDX, IDC_MFCMENUBUTTON_BASE, mTimeLineBase);
}

BEGIN_MESSAGE_MAP(CTimeLineView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_FORWARD, &CTimeLineView::OnBnClickedButtonForward)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CTimeLineView::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_CHECK_AUTOSCROL, &CTimeLineView::OnBnClickedCheckAutoscrol)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_TIMESCALE, &CTimeLineView::OnCbnSelchangeComboTimescale)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_FORECAST, &CTimeLineView::OnBnClickedButtonForecast)
	ON_BN_CLICKED(IDC_BUTTON_PLANLIST, &CTimeLineView::OnBnClickedButtonPlanlist)
	ON_UPDATE_COMMAND_UI(ID_TIMELINEBASE_SPACE, OnUpdateBaseSpace)
	ON_UPDATE_COMMAND_UI(ID_TIMELINEBASE_STATION, OnUpdateBaseStation)
	ON_BN_CLICKED(IDC_MFCMENUBUTTON_BASE, &CTimeLineView::OnBnClickedMfcmenubuttonBase)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CTimeLineView 診断

#ifdef _DEBUG
void CTimeLineView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTimeLineView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
CStationControlDoc* CTimeLineView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


/*============================================================================*/
/*! タイムライン

-# 運用計画データの作成

@param
@retval なし
*/
/*============================================================================*/
void CTimeLineView::CreateTimelineData()
{
	CGraphMutex::Lock(eTimeline);
	mGraphHeader.ClearTimeData();
	mGraphHeader.ClearForecastData();
	mGraph.ClearTimeData();
	mGraph.ClearForecastData();
//	CGraphMutex::Unlock(eTimeline);

	// 予報値からタイムラインデータを作成する
	CGraphMutex::Lock(eFile);
	stForecastData data;
	vector<stPredListData>& predlist = theApp.GetSatelliteData().GetPredList();
	vector<stPredListData>::iterator itr;
	BOOL bVLBIPred[eStation_MAX];
	for (int i = 0; i < eStation_MAX; i++)
	{
		// VLBI予報値フラグクリア
		bVLBIPred[i] = FALSE;
	}

	for (itr = predlist.begin(); itr != predlist.end(); itr++){
		wsprintf(data.satellite, _T("%s"), (LPCTSTR)(*itr).strSatellite);
		wsprintf(data.passId, _T("%s"), (LPCTSTR)(*itr).strPassId);
		data.aos = (*itr).tAOS;
		data.los = (*itr).tLOS;
		data.maxelTime = (*itr).tMaxEl;
		data.maxel = (*itr).dMaxEl;
		UINT id = theApp.GetSatelliteData().GetStationID((*itr).strStationName);
		if (id == 0xffffffff)
			continue;
		if (theApp.GetSelectStationMode() == eStation_1){
			if (id != theApp.GetSelectStation())
				continue;
		}
		data.station = id;
		data.flag = 0;
		mGraph.SetForecastData(data);
		mGraph.SetSatelliteData(data.satellite);

		if ((*itr).strSatellite.CompareNoCase(_T("vlbi")) == 0)
		{
			// 該当局にVLBI予報値有り
			bVLBIPred[id] = TRUE;
		}
	}

//	CGraphMutex::Unlock(eFile);
//	mGraph.SortForecastData();

//	CGraphMutex::Lock(eFile);
	vector<stTimeData>& tlist = mGraph.GetTimeDataList();
	// 運用計画データのみ作成
	tlist.clear();

	vector<stPlanListData>& planlist = theApp.GetSatelliteData().GetPlanList();
	vector<stPlanListData>::iterator itrt;

	for (itrt = planlist.begin(); itrt != planlist.end(); itrt++){
		// VLBI,MAINT,レディネスチェックの特別処理
		UINT flag = 0;
		CString strVlbi = _T("");
		CString strMaint = _T("");
		CString strRdns = _T("");
		CString strVlbiPassId = _T("");
		CString strMaintPassId = _T("");
		CString strRdnsPassId = _T("");
		__int64 aos = 0;
		__int64 los = 0;
		__int64 maxTime = 0;
		double maxel = 0.0;
		UINT vlbiStation = 0;
		UINT maintStation = 0;
		UINT rdnsStation = 0;
		// 局毎にVLBI,MAINT,レディネスチェックを特別処理する
		for (int nStationNo = 0; nStationNo < eStation_MAX; nStationNo++){
			if (theApp.GetSelectStationMode() == eStation_1){
				if (nStationNo != theApp.GetSelectStation())
					continue;
			}

			if ((*itrt).strSatelliteName.CompareNoCase(_T("vlbi")) == 0){
				// VLBI有り
				strVlbi = (*itrt).strSatelliteName;
				strVlbiPassId = (*itrt).strPassId;
				vlbiStation = theApp.GetSatelliteData().GetStationID((*itrt).strStationName);
				if (vlbiStation == 0xffffffff || vlbiStation != nStationNo)
					continue;
				flag |= 0x01;

				if (bVLBIPred[nStationNo] == FALSE)
				{
					// 局に予報値が見つからないVLBI計画があるので追加
					data.flag = flag;
					wsprintf(data.satellite, strVlbi);
					wsprintf(data.passId, strVlbiPassId);
					data.station = vlbiStation;
					mGraph.SetForecastData(data);
					mGraph.SetSatelliteData(data.satellite);
				}
			}
			if ((*itrt).strSatelliteName.CompareNoCase(_T("maint")) == 0){
				// MAINT有り
				strMaint = (*itrt).strSatelliteName;
				strMaintPassId = (*itrt).strPassId;
				maintStation = theApp.GetSatelliteData().GetStationID((*itrt).strStationName);
				if (maintStation == 0xffffffff || maintStation != nStationNo)
					continue;
				flag |= 0x02;

				// MAINTを追加
				data.flag = flag;
				wsprintf(data.satellite, strMaint);
				wsprintf(data.passId, strMaintPassId);
				data.station = maintStation;
				mGraph.SetForecastData(data);
				mGraph.SetSatelliteData(data.satellite);
			}

			if ((*itrt).strPlanType.CollateNoCase(_T("レディネスチェック")) == 0)
			{
				bool bFindSat = false;
				for (itr = predlist.begin(); itr != predlist.end(); itr++)
				{
					// 校正診断計画有り
					if ((*itr).strStationName != (*itrt).strStationName)
						continue;

					if ((*itr).strSatellite == (*itrt).strSatelliteName)
					{
						// 同じ局で同じ衛星有り
						bFindSat = true;
						break;
					}
				}

				if (bFindSat == false)
				{
					// 予報値に衛星が無い場合
					strRdns = (*itrt).strSatelliteName;
					strRdnsPassId = (*itrt).strPassId;
					rdnsStation = theApp.GetSatelliteData().GetStationID((*itrt).strStationName);
					if (rdnsStation == 0xffffffff || rdnsStation != nStationNo)
						continue;
					flag |= 0x04;

					// レディネスチェックを追加
					data.flag = flag;
					wsprintf(data.satellite, strRdns);
					wsprintf(data.passId, strRdnsPassId);
					data.station = rdnsStation;
					mGraph.SetForecastData(data);
					mGraph.SetSatelliteData(data.satellite);
				}
			}
		}
	}

	CGraphMutex::Unlock(eFile);
	mGraph.SortForecastData();

	CGraphMutex::Lock(eFile);

	// 局立案優先のため、局立案から検索
	for (itrt = planlist.begin(); itrt != planlist.end(); itrt++){
		UINT id = theApp.GetSatelliteData().GetStationID((*itrt).strStationName);
		if (theApp.GetSelectStationMode() == eStation_1){
			if (id != theApp.GetSelectStation())
				continue;
		}

		if ((*itrt).strGetFrom.CompareNoCase(_T("ENP")) != 0)
		{
			// ポストパス（ＡＮＴ天頂指向）
			GetPostPass((*itrt).strPlanName, (*itrt).strPostPass);

			// VLBI,MAINTの特別処理
			GetMaintComment((*itrt).strPlanName, (*itrt).strMaintComment);

			mGraph.SetTimeLineData((*itrt).strSatelliteName, (*itrt).strPassId, (*itrt), true);
		}
	}
	// 次にENP局立案を検索
	for (itrt = planlist.begin(); itrt != planlist.end(); itrt++){
		UINT id = theApp.GetSatelliteData().GetStationID((*itrt).strStationName);
		if (theApp.GetSelectStationMode() == eStation_1){
			if (id != theApp.GetSelectStation())
				continue;
		}

		if ((*itrt).strGetFrom.CompareNoCase(_T("ENP")) == 0)
		{
			// ポストパス（ＡＮＴ天頂指向）
			GetPostPass((*itrt).strPlanName, (*itrt).strPostPass);

			// VLBI,MAINTの特別処理
			GetMaintComment((*itrt).strPlanName, (*itrt).strMaintComment);

			mGraph.SetTimeLineData((*itrt).strSatelliteName, (*itrt).strPassId, (*itrt), true);
		}
	}

	mTimeRate = GetPrivateProfileInt(SECTION_DEBUG, KEY_TIMERATE, 1, theApp.GetAppInifileName());

	CGraphMutex::Unlock(eFile);
	CGraphMutex::Unlock(eTimeline);
}

void CTimeLineView::GetMaintComment(CString strPlan, CString& comment)
{
	FILE *fp = NULL;
	if (_wfopen_s(&fp, strPlan, _T("r")) != 0 || fp == NULL)
	{
		return;
	}

	comment.Empty();
	TCHAR sz_buff[512];
	char sz_line[512];
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (wcsncmp(sz_buff, _T("# COMMENT"), wcslen(_T("# COMMENT"))) == 0)
		{
			CString temp;
			CString line(sz_buff);
			AfxExtractSubString(temp, line, 1, '=');
			temp = temp.Mid(0, temp.GetLength() - 1);	// 改行コード削除
			if (comment.IsEmpty() == false)
				comment += _T("\t");
			comment += temp;
		}
	}
	fclose(fp);
}

void CTimeLineView::GetPostPass(CString strPlan, CString& post)
{
	FILE *fp = NULL;
	if (_wfopen_s(&fp, strPlan, _T("r")) != 0 || fp == NULL)
	{
		return;
	}

	post.Empty();
	TCHAR sz_buff[512];
	char sz_line[512];
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		CString line(sz_buff);
		int pos = line.Find(ANT_STOW);
		if (pos >= 0){
			pos += (int)wcslen(ANT_STOW) + 1;
			post = line.Mid(pos, 2);
			break;
		}
	}
	fclose(fp);
}

// CTimeLineView メッセージ ハンドラー
void CTimeLineView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	mChildMinSize = false;

	CRect rect;
	GetClientRect(rect);

	GetParentFrame()->RecalcLayout();

	GetDocument()->SetTitleEx(mViewTitle[eViewTypeTool_TimeLine]);

	int	nMapMode;
	GetDeviceScrollSizes(nMapMode, mSizeTotal, mSizePage, mSizeLine);
	//SetScaleToFitSize(mSizeTotal);
	ResizeParentToFit(FALSE);

	// システムコマンドSC_CLOSEのディセーブル
	CMenu* pSysMenu = GetParent()->GetSystemMenu(FALSE);
	//if (pSysMenu)	pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

	HICON icon = NULL;
	icon = theApp.LoadIcon(IDI_ICON_TIMELINE);
	GetParent()->SetIcon(icon, TRUE);
	GetParent()->SetIcon(icon, FALSE);
	if (icon != NULL)
		DestroyIcon(icon);


	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_FORWARD), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_btForward.SetIcon(hIcon);
	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_NEXT), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_btNext.SetIcon(hIcon);

	setControlInfo(IDC_STATIC_TIMESCALE, ANCHORE_LEFTTOP);
	setControlInfo(IDC_COMBO_TIMESCALE, ANCHORE_LEFTTOP);
	setControlInfo(IDC_BUTTON_FORWARD, ANCHORE_LEFTTOP);
	setControlInfo(IDC_BUTTON_NEXT, ANCHORE_LEFTTOP);
	setControlInfo(IDC_CHECK_AUTOSCROL, ANCHORE_LEFTTOP);
	setControlInfo(IDC_STATIC_NEXTPASS, ANCHORE_RIGHT);
	setControlInfo(IDC_STATIC_NEXTPREPASS, ANCHORE_RIGHT);
	setControlInfo(IDC_CHECK_LEGEND1, ANCHORE_BOTTOM);
	setControlInfo(IDC_CHECK_LEGEND2, ANCHORE_BOTTOM);
	setControlInfo(IDC_CHECK_LEGEND3, ANCHORE_BOTTOM);
	setControlInfo(IDC_CHECK_LEGEND4, ANCHORE_BOTTOM);
	setControlInfo(IDC_CHECK_LEGEND5, ANCHORE_BOTTOM);
	setControlInfo(IDC_CHECK_LEGEND6, ANCHORE_BOTTOM);
	setControlInfo(IDC_CHECK_LEGEND7, ANCHORE_BOTTOM);
	setControlInfo(IDC_CHECK_LEGEND8, ANCHORE_BOTTOM);
	setControlInfo(IDC_CHECK_LEGEND9, ANCHORE_BOTTOM);
	setControlInfo(IDC_CHECK_LEGEND10, ANCHORE_BOTTOM);
	setControlInfo(IDC_CHECK_LEGEND11, ANCHORE_BOTTOM);
	setControlInfo(IDC_CHECK_LEGEND12, ANCHORE_BOTTOM);
	setControlInfo(IDC_BUTTON_FORECAST, ANCHORE_RIGHT | ANCHORE_BOTTOM);
	setControlInfo(IDC_BUTTON_PLANLIST, ANCHORE_RIGHT | ANCHORE_BOTTOM);
	setControlInfo(IDC_STATIC_TIMELINEHEADER, ANCHORE_LEFTTOP | RESIZE_HOR);
	setControlInfo(IDC_STATIC_TIMELINE, ANCHORE_LEFTTOP | RESIZE_BOTH);
	setControlInfo(IDC_STATIC_NOTES, ANCHORE_BOTTOM);

	m_btCheck[0].SetNotesType(eColor_Forecast);
	m_btCheck[1].SetNotesType(eColor_PrePass);
	m_btCheck[2].SetNotesType(eColor_PostPass);
	m_btCheck[3].SetNotesType(eColor_ForecastUsed);
	m_btCheck[4].SetNotesType(eColor_OpePlanEnp);
	m_btCheck[5].SetNotesType(eColor_OpePlan);
	m_btCheck[6].SetNotesType(eColor_OpePlanCalib);
	m_btCheck[7].SetNotesType(eColor_OpePlanNormal);
	m_btCheck[8].SetNotesType(eColor_OpeStationOccupy);
	m_btCheck[10].SetNotesType(eColor_OpePlanAbnormal);
	m_btCheck[11].SetNotesType(eColor_OpeVlbi);

	// 現在時刻の取得
	GetCurrentUtcTime(mCurrentTime);
	mGraphHeader.SetCurrentTime(mCurrentTime);
	mGraph.SetCurrentTime(mCurrentTime);

	GetDlgItem(IDC_STATIC_TIMELINEHEADER)->GetWindowRect(rect);
	ScreenToClient(rect);
	mGraphHeader.SetHeader();
	mGraphHeader.Create(WS_VISIBLE, rect, this, 1000);

	GetDlgItem(IDC_STATIC_TIMELINE)->GetWindowRect(rect);
	ScreenToClient(rect);
	mGraph.Create(WS_VISIBLE, rect, this, 1001);

	mGraph.SetSyncTimeLine(&mGraphHeader);

	// ※CSatelliteData::GetShareFileで行う
	//// 最初の各種ファイルの更新日付を取得する
	//IsChangeShareFile(mAllFileMask);

	// タイムラインデータの作成
	CreateTimelineData();

	GetDlgItem(IDC_BUTTON_FORWARD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);

	m_stNextPass.SetFont(&afxGlobalData.fontDefaultGUIBold);

	if (theApp.GetSelectStationMode() == eStation_5){
		mBaseMenu.LoadMenu(IDR_MENU_TIMELINEBASE);
		mTimeLineBase.m_hMenu = mBaseMenu.GetSubMenu(0)->GetSafeHmenu();
		mTimeLineBase.SizeToContent();
		mTimeLineBase.m_bOSMenu = FALSE;
	}
	else{
		mTimeLineBase.ShowWindow(SW_HIDE);
	}

	SetTimer(1, 10, NULL);
}
/*============================================================================*/
/*! タイムライン

-# アンテナ予報値取得

@brief アンテナ予報値の開始終了日時を取得する

@param  start		開始日時
@param  end			終了日時
@retval なし
*/
/*============================================================================*/
void CTimeLineView::getAntForecastTime(CTime &start, CTime &end)
{
	// ダミー
	// アンテナ予報値AOSを現在時刻、LOSを現在時刻から12間後とする。
	CTime now, after;
	CTimeSpan span(0, OperationTime_Forecast, 0, 0);

	now = CTime::GetCurrentTime();
	after = now + span;

	start = now;
	end = after;
}

/*============================================================================*/
/*! タイムライン

-# リサイズ時のコントロール情報を設定する

@param	id		コントロールID
@param	anchore	リサイズ時の識別子

@retval void
*/
/*============================================================================*/
void CTimeLineView::setControlInfo(WORD id, WORD anchore)
{
	if (anchore == ANCHORE_LEFTTOP)
		return;

	DWORD info = id | (anchore << 16);
	mControl.push_back(info);
}


void CTimeLineView::OnBnClickedButtonForward()
{
	mGraphHeader.Forward();
	mGraph.Forward();
}


void CTimeLineView::OnBnClickedButtonNext()
{
	mGraphHeader.Next();
	mGraph.Next();
}


void CTimeLineView::OnBnClickedCheckAutoscrol()
{
	UpdateData(TRUE);
	mGraphHeader.AutoScrol(m_bAutoScrolOff == FALSE ? true : false);
	mGraph.AutoScrol(m_bAutoScrolOff == FALSE ? true : false);
	GetDlgItem(IDC_BUTTON_FORWARD)->EnableWindow(m_bAutoScrolOff);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(m_bAutoScrolOff);
}


void CTimeLineView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if (nType == SIZE_MINIMIZED)
		return;

	if (GetSafeHwnd() == NULL)
		return;

	if (mControl.size() == 0)
		return;

	if (mSaveCx == 0){
		mSaveCx = cx;
		mSaveCy = cy;
		return;
	}

	int dx = cx - mSaveCx;
	int dy = cy - mSaveCy;

	CRect rect, rectClient;
	GetClientRect(rectClient);

	CWnd *pWnd;
	DWORD info;
	WORD anchore;

	vector<DWORD>::iterator itr;
	for (itr = mControl.begin(); itr != mControl.end(); itr++){
		info = (*itr);
		pWnd = GetDlgItem(LOWORD(info));
		if (!pWnd){
			continue;
		}

		if (!HIWORD(info))
			continue;

		anchore = HIWORD(info);
		pWnd->GetWindowRect(rect);
		ScreenToClient(rect);

		if (anchore & RESIZE_HOR)
			rect.right += dx;
		else if (anchore & ANCHORE_RIGHT)
			rect.OffsetRect(dx, 0);

		if (anchore & RESIZE_VER)
			rect.bottom += dy;
		else if (anchore & ANCHORE_BOTTOM)
			rect.OffsetRect(0, dy);

		if (LOWORD(info) == IDC_STATIC_TIMELINEHEADER){
			rect.left = 10;
			rect.right = rectClient.right - 10;
			int timeWidth = (rect.Width() - _FIXEDLABEL_WIDTH) / (MAX_TIMEDIV - 1);
			rect.right = rect.left + _FIXEDLABEL_WIDTH + (timeWidth*(MAX_TIMEDIV - 1));
			mGraphHeader.MoveWindow(rect);
		}
		if (LOWORD(info) == IDC_STATIC_TIMELINE){
			rect.left = 10;
			rect.right = rectClient.right - 10;
			int timeWidth = (rect.Width() - _FIXEDLABEL_WIDTH) / (MAX_TIMEDIV - 1);
			rect.right = rect.left + _FIXEDLABEL_WIDTH + (timeWidth*(MAX_TIMEDIV - 1));
			mGraph.MoveWindow(rect);
		}

		pWnd->MoveWindow(rect);
	}

	if (mChildMinSize == false){
		mChildMinSize = true;
		CRect rect;
		((CChildFrame*)GetParent())->GetWindowRect(rect);
		((CChildFrame*)GetParent())->SetFrameMaxSize(rect.Width(), rect.Height());
	}

	mSaveCx = cx;
	mSaveCy = cy;
}


void CTimeLineView::OnCbnSelchangeComboTimescale()
{
	UpdateData(TRUE);
	mGraphHeader.SetTimeScaleIndex(m_nTimeScal);
	mGraph.SetTimeScaleIndex(m_nTimeScal);
	mGraphHeader.RedrawGraph();
	mGraph.RedrawGraph();
}

void CTimeLineView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1){
		if (mFirst == true){
			KillTimer(1);
		}

		// ※CSatelliteData::GetShareFileで行う
		//// ファイルの変更をチェック
		//UINT ret = IsChangeShareFile(mAllFileMask);
		//if (ret != 0){
		//	CreateTimelineData();
		//}

		UpdateData(TRUE);
		// 現在時刻の取得
		GetCurrentUtcTime(mCurrentTime);
		// 直近のプリパス時間の取得
		GetNearPrePassTime();

		mGraphHeader.SetCurrentTime(mCurrentTime);
		mGraph.SetCurrentTime(mCurrentTime);

		if (mNextPrePassTime == 0){
			m_strPrePass = _T("--- --:--:--");
			UpdateData(FALSE);
		}
		else{
			CTime temptime(mNextPrePassTime);
			CTimeSpan sp = temptime - mCurrentTime;
			m_strPrePass = sp.Format(_T("%D %H:%M:%S"));
			UpdateData(FALSE);
		}
		if (m_bAutoScrolOff == FALSE){
			mGraphHeader.RedrawGraph(mFirst);
			mGraph.RedrawGraph(mFirst);
		}
		if (mFirst == true){
			SetTimer(1, 1000, NULL);
		}
		mFirst = false;
		return;
	}

	CFormView::OnTimer(nIDEvent);
}

//#define _DEBUGTIME 1
/*============================================================================*/
/*! タイムライン

-# 現在時刻の設定

@brief 現在時刻（UTC）を取得する

@param
@retval なし
*/
/*============================================================================*/
void CTimeLineView::GetCurrentUtcTime(CTime& time)
{
#ifdef _DEBUGTIME
	static bool bFirst = true;
	static SYSTEMTIME tmpUTC;
	if (bFirst == false){
		time = CTime(tmpUTC);
		CTimeSpan sp = CTimeSpan(0, 0, 0, mTimeRate);
		time += sp;
		time.GetAsSystemTime(tmpUTC);
		time = CTime(tmpUTC);
		return;
	}
	GetSystemTime(&tmpUTC);
	bFirst = false;
#endif

//#ifdef _DEBUG
//	static bool bFirst = true;
//	static CTime timeFirst;
//	if (bFirst)
//	{
//		//		timeFirst = CTime(2017, 10, 25, 11, 30, 0);
//		//		timeFirst = CTime(2017, 11, 01, 4, 0, 0);
//		//		timeFirst = CTime(2017, 11, 01, 4, 30, 0);
//		timeFirst = CTime(2018, 12, 15, 03, 0, 0);
//		time = timeFirst;
//		bFirst = false;
//	}
//	else
//	{
//		if (time.GetTime() <= 0)
//		{
//			time = timeFirst;
//		}
//		CTimeSpan sp = CTimeSpan(0, 0, 0, 1);
//		time += sp;
//	}
//
//#else
	// 現在時刻の描画
	SYSTEMTIME st_UTC;
	GetSystemTime(&st_UTC);
	time = CTime(st_UTC);

//#endif
}

/*============================================================================*/
/*! 運用計画タイムライン

-# 直近のプリパス時間の更新

@param
@retval

*/
/*============================================================================*/
void CTimeLineView::GetNearPrePassTime()
{
	CTime curtime = mCurrentTime;

	vector<stTimeData>& data = mGraph.GetTimeDataList();

	mNextPrePassTime = 0;
	vector<stTimeData>::iterator itr;
	for (itr = data.begin(); itr != data.end(); itr++){
		map< __int64, vector<stTimeLine> >::iterator itrm;
		for (itrm = (*itr).item.begin(); itrm != (*itr).item.end(); itrm++){
			vector<stTimeLine>::iterator itrp;
			for (itrp = (*itrm).second.begin(); itrp != (*itrm).second.end(); itrp++){
				if ((*itrp).start >= curtime.GetTime()){
					if ((*itrp).kind == ePassKind_PrePass){
						if (mNextPrePassTime == 0){
							mNextPrePassTime = (*itrp).start;
						}
						else{
							mNextPrePassTime = __min(mNextPrePassTime, (*itrp).start);
						}
					}
				}
			}
		}
	}
}


void CTimeLineView::OnBnClickedButtonForecast()
{
	// 予報値呼び出し
	theApp.SetTimeLineCall(true);
	(theApp.m_pMainWnd)->PostMessage(WM_COMMAND, ID_MP_FORECASTLIST, 0);
}


void CTimeLineView::OnBnClickedButtonPlanlist()
{
	// 運用計画呼び出し
	theApp.SetTimeLineCall(true);
	(theApp.m_pMainWnd)->PostMessage(WM_COMMAND, ID_MP_PLANLIST, 0);
}

void CTimeLineView::OnUpdateBaseSpace(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck((mSelectBase == 0));
}

void CTimeLineView::OnUpdateBaseStation(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck((mSelectBase == 1));
}

void CTimeLineView::OnBnClickedMfcmenubuttonBase()
{
	CString strItem;

	switch (mTimeLineBase.m_nMenuResult)
	{
	case ID_TIMELINEBASE_SPACE:
		mSelectBase = 0;
		break;
	case ID_TIMELINEBASE_STATION:
		mSelectBase = 1;
		break;

	default:
		return;
	}

	mTimeLineBase.SetWindowText(mButtonString[mSelectBase]);
	mGraphHeader.SetSelectBase(mSelectBase);
	mGraph.SetSelectBase(mSelectBase);
	mGraphHeader.RedrawGraph();
	mGraph.RedrawGraph();
}


LRESULT CTimeLineView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_UpdateShareFile:
		if (lParam == 0)
			break;
		CreateTimelineData();
		break;
	default:
		return CFormView::WindowProc(message, wParam, lParam);
	}
	return TRUE;
}


void CTimeLineView::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 100;
	lpMMI->ptMinTrackSize.y = 100;

	CFormView::OnGetMinMaxInfo(lpMMI);
}

///*============================================================================*/
///*! 運用計画タイムライン
//
//-# 共有ファイルの変更チェック
//
//@param	nMask		更新日付の変更チェックを行う共有ファイルマスク
//@retval		更新日付が変更されたビット値
//
//*/
///*============================================================================*/
//UINT CTimeLineView::IsChangeShareFile(UINT nMask)
//{
//	typedef struct {
//		UINT		bit;
//		UINT		type;
//	} stCheckTable;
//	const stCheckTable mCheckTable[] = {
//		{ eChange_PredListFile, eFileType_PredList_File },
//		{ eChange_PLanListFile, eFileType_PlanList_File },
//	};
//
//	UINT retBit = 0;
//	CString strFilePath;
//	HANDLE hFile;
//	FILETIME cfTime, afTime, wfTime;
//
//	for (int i = 0; i < sizeof(mCheckTable) / sizeof(mCheckTable[0]); i++){
//		if (nMask & (1 << mCheckTable[i].bit)){
//			// 一旦ファイルをオープンして指定ファイルの更新日付をチェックする
//			strFilePath = theApp.GetShareFilePath(mCheckTable[i].type, theApp.GetSelectStation());
//			hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
//			if (hFile != INVALID_HANDLE_VALUE){
//				GetFileTime(hFile, &cfTime, &afTime, &wfTime);
//				CloseHandle(hFile);
//				CTime time(wfTime);
//				if (time != mCheckTime[mCheckTable[i].bit]){
//					// ファイルの更新日付が更新されている
//					mCheckTime[mCheckTable[i].bit] = time;
//					retBit |= (1 << mCheckTable[i].bit);
//				}
//			}
//		}
//	}
//	return retBit;
//}
