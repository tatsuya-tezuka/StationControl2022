// AlarmFilterDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "AlarmFilterDlg.h"
#include "afxdialogex.h"


// CAlarmFilterDlg ダイアログ

IMPLEMENT_DYNAMIC(CAlarmFilterDlg, CBarChild)

CAlarmFilterDlg::CAlarmFilterDlg(CWnd* pParent /*=NULL*/)
: CBarChild(CAlarmFilterDlg::IDD, pParent)
{
	m_bOneStation = false;
	for (int i = 0; i < mStationMax; i++)
		m_bStation[i] = TRUE;
	for (int i = 0; i < mImStatusKindMax; i++)
		m_bImStatus[i] = TRUE;
	for (int i = 0; i < mStatusKindMax; i++)
		m_bStatus[i] = TRUE;
}

CAlarmFilterDlg::~CAlarmFilterDlg()
{
}

void CAlarmFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CBarChild::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_STATION, m_grStation);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_grImStatus);
	DDX_Control(pDX, IDC_STATIC_STATUS2, m_grStatus);
	DDX_Check(pDX, IDC_CHECK_STATION1, m_bStation[0]);
	DDX_Check(pDX, IDC_CHECK_STATION2, m_bStation[1]);
	DDX_Check(pDX, IDC_CHECK_STATION3, m_bStation[2]);
	DDX_Check(pDX, IDC_CHECK_STATION4, m_bStation[3]);
	DDX_Check(pDX, IDC_CHECK_STATUS1, m_bImStatus[0]);
	DDX_Check(pDX, IDC_CHECK_STATUS2, m_bImStatus[1]);
	DDX_Check(pDX, IDC_CHECK_STATUS3, m_bStatus[0]);
	DDX_Check(pDX, IDC_CHECK_STATUS4, m_bStatus[1]);
	DDX_Check(pDX, IDC_CHECK_STATUS5, m_bStatus[2]);
	DDX_Check(pDX, IDC_CHECK_STATUS6, m_bStatus[3]);
	DDX_Control(pDX, IDC_CHECK_STATION1, m_btStation[0]);
	DDX_Control(pDX, IDC_CHECK_STATION2, m_btStation[1]);
	DDX_Control(pDX, IDC_CHECK_STATION3, m_btStation[2]);
	DDX_Control(pDX, IDC_CHECK_STATION4, m_btStation[3]);
}


BEGIN_MESSAGE_MAP(CAlarmFilterDlg, CBarChild)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL, &CAlarmFilterDlg::OnBnClickedButtonSelectall)
	ON_BN_CLICKED(IDC_BUTTON_EXEC, &CAlarmFilterDlg::OnBnClickedButtonExec)
END_MESSAGE_MAP()


// CAlarmFilterDlg メッセージ ハンドラー


BOOL CAlarmFilterDlg::OnInitDialog()
{
	CBarChild::OnInitDialog();

	CRect rect;
	GetClientRect(rect);
	mMaxWidth = rect.Width();
	mMaxHeight = rect.Height();

	m_btStation[0].SetStation(eStation_Usuda64);
	m_btStation[1].SetStation(eStation_Uchinoura34);
	m_btStation[2].SetStation(eStation_Uchinoura20);
	m_btStation[3].SetStation(eStation_Usuda642);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CAlarmFilterDlg::OnBnClickedButtonSelectall()
{
	UpdateData(TRUE);
	for (int i = 0; i < mStationMax; i++)
		m_bStation[i] = TRUE;
	for (int i = 0; i < mImStatusKindMax; i++)
		m_bImStatus[i] = TRUE;
	for (int i = 0; i < mStatusKindMax; i++)
		m_bStatus[i] = TRUE;
	UpdateData(FALSE);
}


void CAlarmFilterDlg::OnBnClickedButtonExec()
{
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

	m_Callback(data, m_nHistoryType);
}
