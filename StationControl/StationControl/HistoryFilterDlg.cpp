// HistoryFilterDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "HistoryFilterDlg.h"
#include "afxdialogex.h"


// CHistoryFilterDlg ダイアログ

IMPLEMENT_DYNAMIC(CHistoryFilterDlg, CBarChild)

CHistoryFilterDlg::CHistoryFilterDlg(CWnd* pParent /*=NULL*/)
: CBarChild(CHistoryFilterDlg::IDD, pParent)
{
	m_bOneStation = false;
	for (int i = 0; i < mStationMax+1; i++)
		m_bStation[i] = TRUE;
	for (int i = 0; i < mStatusFlagMax; i++)
		m_bStatus[i] = TRUE;
}

CHistoryFilterDlg::~CHistoryFilterDlg()
{
}

void CHistoryFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CBarChild::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SENDER, m_btSender[0]);
	DDX_Control(pDX, IDC_BUTTON_SENDER2, m_btSender[1]);
	DDX_Control(pDX, IDC_BUTTON_SENDER3, m_btSender[2]);
	DDX_Control(pDX, IDC_BUTTON_SENDER4, m_btSender[3]);
	DDX_Control(pDX, IDC_STATIC_STATION, m_grStation);
	DDX_Control(pDX, IDC_STATIC_SENDEQ, m_grSendEq);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_grStatus);
	DDX_Control(pDX, IDC_STATIC_MESSAGE, m_grMessage);
	DDX_Check(pDX, IDC_CHECK_STATION1, m_bStation[0]);
	DDX_Check(pDX, IDC_CHECK_STATION2, m_bStation[1]);
	DDX_Check(pDX, IDC_CHECK_STATION3, m_bStation[2]);
	DDX_Check(pDX, IDC_CHECK_STATION4, m_bStation[3]);
	DDX_Check(pDX, IDC_CHECK_STATION5, m_bStation[4]);
	DDX_Check(pDX, IDC_CHECK_STATUS1, m_bStatus[0]);
	DDX_Check(pDX, IDC_CHECK_STATUS2, m_bStatus[1]);
	DDX_Check(pDX, IDC_CHECK_STATUS3, m_bStatus[2]);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_edMessage);
	DDX_Control(pDX, IDC_CHECK_STATION1, m_btStation[0]);
	DDX_Control(pDX, IDC_CHECK_STATION2, m_btStation[1]);
	DDX_Control(pDX, IDC_CHECK_STATION3, m_btStation[2]);
	DDX_Control(pDX, IDC_CHECK_STATION4, m_btStation[3]);
	DDX_Control(pDX, IDC_CHECK_STATION5, m_btStation[4]);
}


BEGIN_MESSAGE_MAP(CHistoryFilterDlg, CBarChild)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL, &CHistoryFilterDlg::OnBnClickedButtonSelectall)
	ON_BN_CLICKED(IDC_BUTTON_EXEC, &CHistoryFilterDlg::OnBnClickedButtonExec)
	ON_BN_CLICKED(IDC_CHECK_STATION1, &CHistoryFilterDlg::OnBnClickedCheckStation1)
	ON_BN_CLICKED(IDC_CHECK_STATION2, &CHistoryFilterDlg::OnBnClickedCheckStation2)
	ON_BN_CLICKED(IDC_CHECK_STATION3, &CHistoryFilterDlg::OnBnClickedCheckStation3)
	ON_BN_CLICKED(IDC_CHECK_STATION4, &CHistoryFilterDlg::OnBnClickedCheckStation4)
//	ON_BN_CLICKED(IDC_CHECK_STATION5, &CHistoryFilterDlg::OnBnClickedCheckStation5)
//	ON_WM_LBUTTONDOWN()
//	ON_BN_CLICKED(IDC_CHECK_STATUS1, &CHistoryFilterDlg::OnBnClickedCheckStatus1)
//	ON_BN_CLICKED(IDC_CHECK_STATUS2, &CHistoryFilterDlg::OnBnClickedCheckStatus2)
//	ON_BN_CLICKED(IDC_CHECK_STATUS3, &CHistoryFilterDlg::OnBnClickedCheckStatus3)
//	ON_EN_CHANGE(IDC_EDIT_MESSAGE, &CHistoryFilterDlg::OnEnChangeEditMessage)
END_MESSAGE_MAP()


// CHistoryFilterDlg メッセージ ハンドラー


BOOL CHistoryFilterDlg::OnInitDialog()
{
	CBarChild::OnInitDialog();

	CRect rect;
	GetClientRect(rect);
	mMaxWidth = rect.Width();
	mMaxHeight = rect.Height();

	// １局モード時は局選択は必要ないので削除する
	CRect rect1, rect2;
	GetDlgItem(IDC_STATIC_STATION)->GetWindowRect(rect1);
	ScreenToClient(rect1);
	GetDlgItem(IDC_STATIC_SENDEQ)->GetWindowRect(rect2);
	ScreenToClient(rect2);
	int move = rect2.left - rect1.left;

	if (m_nHistoryType == eHistory_Event || m_nHistoryType == eHistory_SearchEvent){
		// イベント関連の場合は「送信元情報」は必要ない
		GetDlgItem(IDC_STATIC_STATUS)->GetWindowRect(rect2);
		ScreenToClient(rect2);
		move = rect2.left - rect1.left;

		// ※実際には仕様が決まってから行う

		const UINT mHideCtl[] = {
			IDC_STATIC_STATION,
			IDC_CHECK_STATION1,
			IDC_CHECK_STATION2,
			IDC_CHECK_STATION3,
			IDC_CHECK_STATION4,
			IDC_CHECK_STATION5,
			IDC_STATIC_SENDEQ,
			IDC_BUTTON_SENDER,
			IDC_BUTTON_SENDER2,
			IDC_BUTTON_SENDER3,
			IDC_BUTTON_SENDER4,
		};
		if (theApp.GetSelectStationMode() == eStation_1){
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
					// 移動
					CRect rc;
					pwnd->GetWindowRect(rc);
					rc.left -= move;
					rc.right -= move;
					ScreenToClient(rc);
					pwnd->MoveWindow(rc);
				}
				pwnd = pwnd->GetWindow(GW_HWNDNEXT);
			}
		}
	}
	else{

		// ※実際には仕様が決まってから行う

		const UINT mHideCtl[] = {
			IDC_STATIC_STATION,
			IDC_CHECK_STATION1,
			IDC_CHECK_STATION2,
			IDC_CHECK_STATION3,
			IDC_CHECK_STATION4,
			IDC_CHECK_STATION5,
			IDC_BUTTON_SENDER2,
			IDC_BUTTON_SENDER3,
			IDC_BUTTON_SENDER4,
		};
		if (theApp.GetSelectStationMode() == eStation_1){
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
					// 移動
					CRect rc;
					pwnd->GetWindowRect(rc);
					rc.left -= move;
					rc.right -= move;
					ScreenToClient(rc);
					pwnd->MoveWindow(rc);
				}
				pwnd = pwnd->GetWindow(GW_HWNDNEXT);
			}
		}
		else{
			if (m_nHistoryType != eHistory_Operation){
				CWnd *pwnd = GetWindow(GW_CHILD);
				while (pwnd != NULL){
					if (GetDlgItem(IDC_CHECK_STATION5) == pwnd){
						pwnd->ShowWindow(SW_HIDE);
					}
					pwnd = pwnd->GetWindow(GW_HWNDNEXT);
				}
			}
		}
	}

	// 送信元コントロールの作成
	for (int i = 0; i < mStationMax; i++)
		m_btSender[i].Create();

	m_btStation[0].SetStation(eStation_Usuda64);
	m_btStation[1].SetStation(eStation_Uchinoura34);
	m_btStation[2].SetStation(eStation_Uchinoura20);
	m_btStation[3].SetStation(eStation_Usuda642);
	m_btStation[4].SetStation(eStation_MAX);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CHistoryFilterDlg::OnBnClickedButtonSelectall()
{
//	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);

	UpdateData(TRUE);
	for (int i = 0; i < mStationMax; i++)
		m_nSender[i] = 0;
	for (int i = 0; i < mStationMax+1; i++)
		m_bStation[i] = TRUE;
	for (int i = 0; i < mStatusFlagMax; i++)
		m_bStatus[i] = TRUE;
	m_edMessage.SetSel(0, -1);
	m_edMessage.Clear();
	for (int i = 0; i < mStationMax; i++)
		m_btSender[i].SetStatus(0);

	for (int i = 0; i < mStationMax; i++)
		m_btSender[i].EnableWindow(m_bStation[i]);

	UpdateData(FALSE);
}


void CHistoryFilterDlg::OnBnClickedButtonExec()
{
//	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);

	UpdateData(TRUE);
	stFilterData data;
	data.filter = true;
	data.station = 0;
	//for (int i = 0; i < mStationMax; i++){
	//	data.station |= (m_bStation[i]==TRUE)?(1<<i):(0<<i);
	//}
	data.station = 0;
	if (m_bStation[0] == TRUE)	data.station |= 0x01;
	if (m_bStation[1] == TRUE)	data.station |= 0x04;
	if (m_bStation[2] == TRUE)	data.station |= 0x08;
	if (m_bStation[3] == TRUE)	data.station |= 0x02;
	if (m_bStation[4] == TRUE)	data.station |= 0x10;
	data.status = 0;
	for (int i = 0; i < mStatusFlagMax; i++){
		data.status |= (m_bStatus[i] == TRUE) ? (1 << i) : (0 << i);
	}
	if (theApp.GetSelectStationMode() == eStation_1){
		data.sender[theApp.GetSelectStation()] = m_btSender[0].GetStatus();
	}
	else{
		data.sender[0] = m_btSender[0].GetStatus();
		data.sender[2] = m_btSender[1].GetStatus();
		data.sender[3] = m_btSender[2].GetStatus();
		data.sender[1] = m_btSender[3].GetStatus();
		//for (int i = 0; i < mStationMax; i++)
		//	data.sender[i] = m_btSender[i].GetStatus();
	}

	CString str;
	m_edMessage.GetWindowText(str);
	if (str.IsEmpty() == true)
		data.message = 0;
	else
		data.message = _wtoi(str);

	//ResetMenu()してもメニューは閉じない
	//for (int i = 0; i < mStationMax; i++)
	//	m_btSender[i].ResetMenu();
	//UpdateWindow();

	m_Callback(data, m_nHistoryType);
}

void CHistoryFilterDlg::OnBnClickedCheckStation1()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON_SENDER)->EnableWindow(m_bStation[0]);
//	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}
void CHistoryFilterDlg::OnBnClickedCheckStation2()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON_SENDER2)->EnableWindow(m_bStation[1]);
//	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}
void CHistoryFilterDlg::OnBnClickedCheckStation3()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON_SENDER3)->EnableWindow(m_bStation[2]);
//	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}
void CHistoryFilterDlg::OnBnClickedCheckStation4()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON_SENDER4)->EnableWindow(m_bStation[3]);
//	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}


void CHistoryFilterDlg::OnBnClickedCheckStation5()
{
	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}

void CHistoryFilterDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);

	CBarChild::OnLButtonDown(nFlags, point);
}

void CHistoryFilterDlg::OnBnClickedCheckStatus1()
{
	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}


void CHistoryFilterDlg::OnBnClickedCheckStatus2()
{
	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}


void CHistoryFilterDlg::OnBnClickedCheckStatus3()
{
	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}


void CHistoryFilterDlg::OnEnChangeEditMessage()
{
	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}

