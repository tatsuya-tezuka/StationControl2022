// HistoryFilterEqDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "HistoryFilterEqDlg.h"
#include "afxdialogex.h"


// CHistoryFilterEqDlg ダイアログ

IMPLEMENT_DYNAMIC(CHistoryFilterEqDlg, CBarChild)

CHistoryFilterEqDlg::CHistoryFilterEqDlg(CWnd* pParent /*=NULL*/)
: CBarChild(CHistoryFilterEqDlg::IDD, pParent)
{
	m_bOneStation = false;
	for (int i = 0; i < mStationMax; i++)
		m_bStation[i] = TRUE;
	for (int i = 0; i < mImStatusKindMax; i++)
		m_bImStatus[i] = TRUE;
	for (int i = 0; i < mStatusKindMax; i++)
		m_bStatus[i] = TRUE;
}

CHistoryFilterEqDlg::~CHistoryFilterEqDlg()
{
}

void CHistoryFilterEqDlg::DoDataExchange(CDataExchange* pDX)
{
	CBarChild::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SENDER, m_btSender);
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
	DDX_Check(pDX, IDC_CHECK_STATUS5, m_bStatus[3]);
	DDX_Check(pDX, IDC_CHECK_STATUS6, m_bStatus[4]);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_edMessage);
	DDX_Control(pDX, IDC_CHECK_STATION1, m_btStation[0]);
	DDX_Control(pDX, IDC_CHECK_STATION2, m_btStation[1]);
	DDX_Control(pDX, IDC_CHECK_STATION3, m_btStation[2]);
	DDX_Control(pDX, IDC_CHECK_STATION4, m_btStation[3]);
}


BEGIN_MESSAGE_MAP(CHistoryFilterEqDlg, CBarChild)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL, &CHistoryFilterEqDlg::OnBnClickedButtonSelectall)
	ON_BN_CLICKED(IDC_BUTTON_EXEC, &CHistoryFilterEqDlg::OnBnClickedButtonExec)
//	ON_BN_CLICKED(IDC_CHECK_IMSTATUS1, &CHistoryFilterEqDlg::OnBnClickedCheckImstatus1)
//	ON_BN_CLICKED(IDC_CHECK_IMSTATUS2, &CHistoryFilterEqDlg::OnBnClickedCheckImstatus2)
//	ON_BN_CLICKED(IDC_CHECK_IMSTATUS3, &CHistoryFilterEqDlg::OnBnClickedCheckImstatus3)
//	ON_BN_CLICKED(IDC_CHECK_STATUS1, &CHistoryFilterEqDlg::OnBnClickedCheckStatus1)
//	ON_BN_CLICKED(IDC_CHECK_STATUS2, &CHistoryFilterEqDlg::OnBnClickedCheckStatus2)
//	ON_BN_CLICKED(IDC_CHECK_STATUS3, &CHistoryFilterEqDlg::OnBnClickedCheckStatus3)
//	ON_BN_CLICKED(IDC_CHECK_STATUS4, &CHistoryFilterEqDlg::OnBnClickedCheckStatus4)
//	ON_BN_CLICKED(IDC_CHECK_STATUS5, &CHistoryFilterEqDlg::OnBnClickedCheckStatus5)
//	ON_BN_CLICKED(IDC_CHECK_STATUS6, &CHistoryFilterEqDlg::OnBnClickedCheckStatus6)
//	ON_EN_CHANGE(IDC_EDIT_MESSAGE, &CHistoryFilterEqDlg::OnEnChangeEditMessage)
//	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CHistoryFilterEqDlg メッセージ ハンドラー

BOOL CHistoryFilterEqDlg::OnInitDialog()
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

	// ※実際には仕様が決まってから行う
	const UINT mHideCtl[] = {
		IDC_STATIC_STATION,
		IDC_CHECK_STATION1,
		IDC_CHECK_STATION2,
		IDC_CHECK_STATION3,
		IDC_CHECK_STATION4,
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

	// 送信元コントロールの作成
	m_btSender.Create();

	m_btStation[0].SetStation(eStation_Usuda64);
	m_btStation[1].SetStation(eStation_Uchinoura34);
	m_btStation[2].SetStation(eStation_Uchinoura20);
	m_btStation[3].SetStation(eStation_Usuda642);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CHistoryFilterEqDlg::OnBnClickedButtonSelectall()
{
//	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);

	UpdateData(TRUE);
	m_nSender = 0;
	for (int i = 0; i < mStationMax; i++)
		m_bStation[i] = TRUE;
	for (int i = 0; i < mImStatusKindMax; i++)
		m_bImStatus[i] = TRUE;
	for (int i = 0; i < mStatusKindMax; i++)
		m_bStatus[i] = TRUE;
	m_edMessage.SetSel(0, -1);
	m_edMessage.Clear();

	m_btSender.SetStatus(0);

	UpdateData(FALSE);
}


void CHistoryFilterEqDlg::OnBnClickedButtonExec()
{
//	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);

	UpdateData(TRUE);
	stFilterData data;
	data.filter = true;
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
	data.sender[theApp.GetSelectStation()] = m_btSender.GetStatus();
	CString str;
	m_edMessage.GetWindowText(str);
	if (str.IsEmpty() == true)
		data.message = 0;
	else
		data.message = _wtoi(str);

	m_Callback(data, m_nHistoryType);
}


void CHistoryFilterEqDlg::OnBnClickedCheckImstatus1()
{
	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}


void CHistoryFilterEqDlg::OnBnClickedCheckImstatus2()
{
	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}


void CHistoryFilterEqDlg::OnBnClickedCheckImstatus3()
{
	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}


void CHistoryFilterEqDlg::OnBnClickedCheckStatus1()
{
	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}


void CHistoryFilterEqDlg::OnBnClickedCheckStatus2()
{
	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}


void CHistoryFilterEqDlg::OnBnClickedCheckStatus3()
{
	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}


void CHistoryFilterEqDlg::OnBnClickedCheckStatus4()
{
	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}


void CHistoryFilterEqDlg::OnBnClickedCheckStatus5()
{
	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}


void CHistoryFilterEqDlg::OnBnClickedCheckStatus6()
{
	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}


void CHistoryFilterEqDlg::OnEnChangeEditMessage()
{
	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);
}


void CHistoryFilterEqDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	theApp.m_pMainWnd->PostMessageW(WM_LBUTTONDOWN);

	CBarChild::OnLButtonDown(nFlags, point);
}
