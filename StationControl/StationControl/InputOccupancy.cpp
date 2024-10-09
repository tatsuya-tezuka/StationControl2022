// InputOccupancy.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "InputOccupancy.h"
#include "afxdialogex.h"


// CInputOccupancy ダイアログ

IMPLEMENT_DYNAMIC(CInputOccupancy, CDialogEx)

CInputOccupancy::CInputOccupancy(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputOccupancy::IDD, pParent)
	, m_strComment(_T(""))
{
	SYSTEMTIME sys;
	GetSystemTime(&sys);
	mStartTime = CTime(sys);
	mEndTime = CTime(sys);
}

CInputOccupancy::~CInputOccupancy()
{
}

void CInputOccupancy::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER_SDATE, m_StartDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_STIME, m_StartTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_EDATE, m_StopDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_ETIME, m_StopTime);
	DDX_Text(pDX, IDC_EDIT1, m_strComment);
}


BEGIN_MESSAGE_MAP(CInputOccupancy, CDialogEx)
END_MESSAGE_MAP()


// CInputOccupancy メッセージ ハンドラー


BOOL CInputOccupancy::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_StartDate.SetFormat(_T("yyyy-MM-dd"));
	m_StartTime.SetFormat(_T("HH:mm:ss"));
	m_StopDate.SetFormat(_T("yyyy-MM-dd"));
	m_StopTime.SetFormat(_T("HH:mm:ss"));

	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	CTime time;
	if (mTimeLineTime == 0)
	{
		time = CTime(timeSys);
	}
	else
	{
		time = mTimeLineTime;
	}

	m_StartDate.SetTime(&time);
	m_StartTime.SetTime(&time);
	m_StopDate.SetTime(&time);
	m_StopTime.SetTime(&time);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CInputOccupancy::OnOK()
{
	CTime sd, st, start;
	CTime ed, et, stop;

	UpdateData();

	// 開始時刻取得
	m_StartDate.GetTime(sd);
	m_StartTime.GetTime(st);
	start = CTime(sd.GetYear(), sd.GetMonth(), sd.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());

	// 終了時刻取得
	m_StopDate.GetTime(ed);
	m_StopTime.GetTime(et);
	stop = CTime(ed.GetYear(), ed.GetMonth(), ed.GetDay(), et.GetHour(), et.GetMinute(), et.GetSecond());

	if ((start == stop) || (start > stop))
	{
		AfxMessageBox(_T("入力された時刻が不正です。"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// 計画ファイル作成
	CString strTime = start.Format(_T("%y%m%d"));
	CString pid;
	GetPlanID(strTime, pid);
	CString satellite = _T("MAINT");

	stPlandata stpd;
	stpd.sz_start = start.Format(_T("%Y-%m-%d %H:%M:%S"));
	stpd.sz_end = stop.Format(_T("%Y-%m-%d %H:%M:%S"));

	int s = theApp.GetSelectStation();
	if (s == 0)	// 臼田64m
	{
		stpd.l_rdychk = 0;
		stpd.l_init = 0;
		stpd.l_mod_sel = 0;
		stpd.l_auto_acq = 0;
		stpd.l_offset = 0;
		stpd.l_a_flag = 0;
		stpd.l_l_flag = 0;
		stpd.l_term_ant = 1;
		stpd.l_cmd_mod = 0;
		stpd.l_rng_mod = 0;
		stpd.l_tx_ramp = 4;
		stpd.l_rx_ramp = 4;
		stpd.l_acq_route = 6;
		stpd.l_pl_sel = 1;
		stpd.l_top_mon = 1;
		stpd.l_ent_angle = 0;
		stpd.l_ext_angle = 0;
		stpd.l_delay_sel = 2;
		stpd.l_offset_val = 0;
		stpd.l_antwait_flg = 0;
		stpd.l_ant_time = 2;
	}
	else if (s == 1)	// 臼田54m
	{
		stpd.l_rdychk = 0;
		stpd.l_init = 0;
		stpd.l_mod_sel = 0;
		stpd.l_auto_acq = 0;
		stpd.l_offset = 0;
		stpd.l_a_flag = 0;
		stpd.l_l_flag = 0;
		stpd.l_term_ant = 1;
		stpd.l_cmd_mod = 0;
		stpd.l_rng_mod = 0;
		stpd.l_tx_ramp = 4;
		stpd.l_rx_ramp = 4;
		stpd.l_acq_route = 6;
		stpd.l_pl_sel = 1;
		stpd.l_top_mon = 1;
		stpd.l_ent_angle = 0;
		stpd.l_ext_angle = 0;
		stpd.l_delay_sel = 2;
		stpd.l_offset_val = 0;
		stpd.l_antwait_flg = 0;
		stpd.l_ant_time = 2;
	}
	else if (s == 2)	// 内之浦34m
	{
		stpd.l_rdychk = 0;
		stpd.l_init = 0;
		stpd.l_mod_sel = 0;
		stpd.l_auto_acq = 0;
		stpd.l_offset = 0;
		stpd.l_a_flag = 0;
		stpd.l_l_flag = 0;
		stpd.l_term_ant = 1;
		stpd.l_cmd_mod = 0;
		stpd.l_rng_mod = 0;
		stpd.l_tx_ramp = 4;
		stpd.l_rx_ramp = 4;
		stpd.l_acq_route = 6;
		stpd.l_pl_sel = 1;
		stpd.l_top_mon = 1;
		stpd.l_ent_angle = 0;
		stpd.l_ext_angle = 0;
		stpd.l_delay_sel = 2;
		stpd.l_offset_val = 0;
		stpd.l_antwait_flg = 0;
		stpd.l_ant_time = 2;
	}
	else if (s == 3)	// 内之浦20m
	{
		stpd.l_rdychk = 0;
		stpd.l_init = 0;
		stpd.l_mod_sel = 0;
		stpd.l_auto_acq = 0;
		stpd.l_offset = 0;
		stpd.l_a_flag = 0;
		stpd.l_l_flag = 0;
		stpd.l_term_ant = 1;
		stpd.l_cmd_mod = 0;
		stpd.l_rng_mod = 0;
		stpd.l_tx_ramp = 4;
		stpd.l_rx_ramp = 4;
		stpd.l_acq_route = 6;
		stpd.l_pl_sel = 1;
		stpd.l_top_mon = 1;
		stpd.l_ent_angle = 0;
		stpd.l_ext_angle = 0;
		stpd.l_delay_sel = 2;
		stpd.l_offset_val = 0;
		stpd.l_antwait_flg = 0;
		stpd.l_ant_time = 2;
	}

	if (COprPlnData::WritePlanFile(satellite, pid, stpd, m_strComment) != -1)
	{
		// 計画ファイル登録
		COprPlnData::RegistPlanNew(satellite, pid);
	}

	CDialogEx::OnOK();
}


void CInputOccupancy::GetPlanID(CString strDay, CString &id)
{
	CGraphMutex::Lock(eFile);

	stPlanListData pld;

	vector<stPlanListData>& planlist = theApp.GetSatelliteData().GetPlanList();
	vector<stPlanListData>::iterator itrt;

	int num = 0;
	for (itrt = planlist.begin(); itrt != planlist.end(); itrt++){
		if ((*itrt).strSatelliteName.CompareNoCase(_T("maint")) == 0){
			CString passid = (*itrt).strPassId;
			CString right = passid.Mid(0, passid.Find(_T("-")));
			if (right == strDay)
			{
				int tmp = _wtoi(passid.Mid(passid.Find(_T("-")) + 1));
				if (num < tmp)
				{
					num = tmp;
				}
			}
		}
	}

	id.Format(_T("%s-%04d"), (LPCTSTR)strDay, num + 1);

	CGraphMutex::Unlock(eFile);
}
