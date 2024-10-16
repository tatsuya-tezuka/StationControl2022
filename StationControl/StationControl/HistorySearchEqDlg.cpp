// HistorySearchEqDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "HistorySearchEqDlg.h"
#include "afxdialogex.h"


// CHistorySearchEqDlg ダイアログ

IMPLEMENT_DYNAMIC(CHistorySearchEqDlg, CBarChild)

CHistorySearchEqDlg::CHistorySearchEqDlg(CWnd* pParent /*=NULL*/)
: CBarChild(CHistorySearchEqDlg::IDD, pParent)
{
	m_bOneStation = false;
	for (int i = 0; i < mStationMax; i++)
		m_bStation[i] = TRUE;
	for (int i = 0; i < mImStatusKindMax; i++)
		m_bImStatus[i] = TRUE;
	for (int i = 0; i < mStatusKindMax; i++)
		m_bStatus[i] = TRUE;
}

CHistorySearchEqDlg::~CHistorySearchEqDlg()
{
}

void CHistorySearchEqDlg::DoDataExchange(CDataExchange* pDX)
{
	CBarChild::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TIME, m_grTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_SDATE, m_StartDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_STIME, m_StartTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_PDATE, m_StopDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_PTIME, m_StopTime);
	DDX_Control(pDX, IDC_BUTTON_SENDER, m_btSender[0]);
	DDX_Control(pDX, IDC_BUTTON_SENDER2, m_btSender[1]);
	DDX_Control(pDX, IDC_BUTTON_SENDER3, m_btSender[2]);
	DDX_Control(pDX, IDC_BUTTON_SENDER4, m_btSender[3]);
	DDX_Control(pDX, IDC_STATIC_STATION, m_grStation);
	DDX_Control(pDX, IDC_STATIC_SENDEQ, m_grSendEq);
	DDX_Control(pDX, IDC_STATIC_IMPORT, m_grImStatus);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_grStatus);
	DDX_Control(pDX, IDC_STATIC_MESSAGE, m_grMessage);
	DDX_Check(pDX, IDC_CHECK_STATION1, m_bStation[0]);
	DDX_Check(pDX, IDC_CHECK_STATION2, m_bStation[1]);
	DDX_Check(pDX, IDC_CHECK_STATION3, m_bStation[2]);
	DDX_Check(pDX, IDC_CHECK_STATION4, m_bStation[3]);
	DDX_Check(pDX, IDC_CHECK_IMSTATUS1, m_bImStatus[0]);
	DDX_Check(pDX, IDC_CHECK_IMSTATUS2, m_bImStatus[1]);
	DDX_Check(pDX, IDC_CHECK_IMSTATUS3, m_bImStatus[2]);
	DDX_Check(pDX, IDC_CHECK_STATUS1, m_bStatus[0]);
	DDX_Check(pDX, IDC_CHECK_STATUS2, m_bStatus[1]);
	DDX_Check(pDX, IDC_CHECK_STATUS3, m_bStatus[2]);
	DDX_Check(pDX, IDC_CHECK_STATUS4, m_bStatus[3]);
	DDX_Check(pDX, IDC_CHECK_STATUS5, m_bStatus[4]);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_edMessage);
	DDX_Control(pDX, IDC_CHECK_STATION1, m_btStation[0]);
	DDX_Control(pDX, IDC_CHECK_STATION2, m_btStation[1]);
	DDX_Control(pDX, IDC_CHECK_STATION3, m_btStation[2]);
	DDX_Control(pDX, IDC_CHECK_STATION4, m_btStation[3]);
}


BEGIN_MESSAGE_MAP(CHistorySearchEqDlg, CBarChild)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL, &CHistorySearchEqDlg::OnBnClickedButtonSelectall)
	ON_BN_CLICKED(IDC_BUTTON_EXEC, &CHistorySearchEqDlg::OnBnClickedButtonExec)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CHistorySearchEqDlg::OnBnClickedButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CHistorySearchEqDlg::OnBnClickedButtonPrint)
END_MESSAGE_MAP()


// CHistorySearchEqDlg メッセージ ハンドラー

BOOL CHistorySearchEqDlg::OnInitDialog()
{
	CBarChild::OnInitDialog();

	CRect rect;
	GetClientRect(rect);
	mMaxWidth = rect.Width();
	mMaxHeight = rect.Height();

	if (theApp.GetSelectStationMode() == eStation_1){
		// １局モード時は局選択は必要ないので削除する
		CRect rect1, rect2;
		GetDlgItem(IDC_STATIC_STATION)->GetWindowRect(rect1);
		ScreenToClient(rect1);
		GetDlgItem(IDC_STATIC_SENDEQ)->GetWindowRect(rect2);
		ScreenToClient(rect2);
		int move = rect2.left - rect1.left;

		// ※実際には仕様が決まってから行う
		const UINT mHideCtl[] = {
			IDC_STATIC_STATION,
			IDC_CHECK_STATION1,
			IDC_CHECK_STATION2,
			IDC_CHECK_STATION3,
			IDC_CHECK_STATION4,
			IDC_BUTTON_SENDER2,
			IDC_BUTTON_SENDER3,
			IDC_BUTTON_SENDER4,
		};
		const UINT mNomoveCtl[] = {
			IDC_STATIC_TIME,
			IDC_STATIC_START2,
			IDC_STATIC_STOP2,
			IDC_DATETIMEPICKER_SDATE,
			IDC_DATETIMEPICKER_STIME,
			IDC_DATETIMEPICKER_PDATE,
			IDC_DATETIMEPICKER_PTIME,
		};
		CWnd *pwnd = GetWindow(GW_CHILD);
		while (pwnd != NULL){
			bool ret = false;
			for (int i = 0; i < sizeof(mHideCtl) / sizeof(mHideCtl[0]); ++i) {
				if (GetDlgItem(mHideCtl[i]) == pwnd){
					ret = true;
					pwnd->ShowWindow(SW_HIDE);
					break;
				}
			}
			if (ret == false){
				ret = false;
				for (int i = 0; i < sizeof(mNomoveCtl) / sizeof(mNomoveCtl[0]); ++i) {
					if (GetDlgItem(mNomoveCtl[i]) == pwnd){
						ret = true;
						break;
					}
				}
				if (ret == false){
					// 移動
					CRect rc;
					pwnd->GetWindowRect(rc);
					rc.left -= move;
					rc.right -= move;
					ScreenToClient(rc);
					pwnd->MoveWindow(rc);
				}
			}
			pwnd = pwnd->GetWindow(GW_HWNDNEXT);
		}
	}

	// 送信元コントロールの作成
	for (int i = 0; i < mStationMax; i++)
		m_btSender[i].Create();

	m_btStation[0].SetStation(eStation_Usuda64);
	m_btStation[1].SetStation(eStation_Uchinoura34);
	m_btStation[2].SetStation(eStation_Uchinoura20);
	m_btStation[3].SetStation(eStation_Usuda642);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CHistorySearchEqDlg::OnBnClickedButtonSelectall()
{
	UpdateData(TRUE);
	for (int i = 0; i < mStationMax; i++)
		m_nSender[i] = 0;
	for (int i = 0; i < mStationMax; i++)
		m_bStation[i] = TRUE;
	for (int i = 0; i < mImStatusKindMax; i++)
		m_bImStatus[i] = TRUE;
	for (int i = 0; i < mStatusKindMax; i++)
		m_bStatus[i] = TRUE;
	m_edMessage.SetSel(0, -1);
	m_edMessage.Clear();
	for (int i = 0; i < mStationMax; i++)
		m_btSender[i].SetStatus(0);
	UpdateData(FALSE);
}


void CHistorySearchEqDlg::OnBnClickedButtonExec()
{
	UpdateData(TRUE);
	stFilterData data;
	data.filter = false;
	data.station = 0;
	for (int i = 0; i < mStationMax; i++){
		data.station |= (m_bStation[i] == TRUE) ? (1 << i) : (0 << i);
	}
	data.imstatus = 0;
	for (int i = 0; i < mImStatusKindMax; i++){
		data.imstatus |= (m_bImStatus[i] == TRUE) ? (1 << i) : (0 << i);
	}
	data.status = 0;
	for (int i = 0; i < mStatusKindMax; i++){
		data.status |= (m_bStatus[i] == TRUE) ? (1 << i) : (0 << i);
	}
	if (theApp.GetSelectStationMode() == eStation_1){
		data.sender[theApp.GetSelectStation()] = m_btSender[0].GetStatus();
	}
	else{
		for (int i = 0; i < mStationMax; i++)
			data.sender[i] = m_btSender[i].GetStatus();
	}
	CString str;
	m_edMessage.GetWindowText(str);
	if (str.IsEmpty() == true)
		data.message = 0;
	else
		data.message = _wtoi(str);

	SYSTEMTIME date, time;
	m_StartDate.GetTime(&date);
	m_StartTime.GetTime(&time);
	data.stime.wYear = date.wYear;
	data.stime.wMonth = date.wMonth;
	data.stime.wDay = date.wDay;
	data.stime.wHour = time.wHour;
	data.stime.wMinute = time.wMinute;
	data.stime.wSecond = time.wSecond;
	m_StopDate.GetTime(&date);
	m_StopTime.GetTime(&time);
	data.ptime.wYear = date.wYear;
	data.ptime.wMonth = date.wMonth;
	data.ptime.wDay = date.wDay;
	data.ptime.wHour = time.wHour;
	data.ptime.wMinute = time.wMinute;
	data.ptime.wSecond = time.wSecond;

	m_Callback(data, m_nHistoryType);
}

void CHistorySearchEqDlg::OnBnClickedButtonFile()
{
}

void CHistorySearchEqDlg::OnBnClickedButtonPrint()
{
}
