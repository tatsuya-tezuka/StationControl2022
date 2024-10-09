// InputVlbi.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "InputVlbi.h"
#include "afxdialogex.h"


// CInputVlbi �_�C�A���O

IMPLEMENT_DYNAMIC(CInputVlbi, CDialogEx)

CInputVlbi::CInputVlbi(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputVlbi::IDD, pParent)
{
	SYSTEMTIME sys;
	GetSystemTime(&sys);
	mStartTime = CTime(sys);
	mEndTime = CTime(sys);
	mTimeLineTime = 0;
}

CInputVlbi::~CInputVlbi()
{
}

void CInputVlbi::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER_SDATE, m_StartDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_STIME, m_StartTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_EDATE, m_StopDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_ETIME, m_StopTime);
}


BEGIN_MESSAGE_MAP(CInputVlbi, CDialogEx)
END_MESSAGE_MAP()


// CInputVlbi ���b�Z�[�W �n���h���[


BOOL CInputVlbi::OnInitDialog()
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
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CInputVlbi::OnOK()
{
	CTime sd, st, start;
	CTime ed, et, stop;

	// �J�n�����擾
	m_StartDate.GetTime(sd);
	m_StartTime.GetTime(st);
	start = CTime(sd.GetYear(), sd.GetMonth(), sd.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());

	// �I�������擾
	m_StopDate.GetTime(ed);
	m_StopTime.GetTime(et);
	stop = CTime(ed.GetYear(), ed.GetMonth(), ed.GetDay(), et.GetHour(), et.GetMinute(), et.GetSecond());

	if ((start == stop) || (start > stop))
	{
		AfxMessageBox(_T("���͂��ꂽ�������s���ł��B"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// �v��t�@�C���쐬
	CString strTime = start.Format(_T("%y%m%d"));
	CString pid;
	GetPlanID(strTime, pid);
	CString satellite = _T("VLBI");

	stPlandata stpd;
	stpd.sz_start = start.Format(_T("%Y-%m-%d %H:%M:%S"));
	stpd.sz_end = stop.Format(_T("%Y-%m-%d %H:%M:%S"));

	// CID
	CString strCid = _T("");
	// �v���p�e�B�t�@�C���iProperty.dat�j���f�t�H���gCID���擾����B
	stOpeProp stProp;
	theApp.GetSatelliteData().ReadPropertyFile(satellite, stProp);

	stCIDParam stCid;
	vector<CString> list = theApp.GetSatelliteData().m_CIDSlctData.GetCidList(satellite);
	if (list.size() > 0)
	{
		strCid = CString(stProp.cid);
		auto itrl = std::find(list.begin(), list.end(), strCid);
		if (itrl == list.end())
		{
			// �f�t�H���gCID��CID�t�@�C���ɖ����ꍇ�A��ԍŏ���CID���Z�b�g�B
			strCid = list[0];
		}
	}

	int s = theApp.GetSelectStation();
	if (s == 0)	// �P�c64m
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
		stpd.sz_cid = strCid;
		stpd.l_antwait_flg = 0;
		stpd.l_ant_time = 2;
	}
	else if (s == 1)	// �P�c54m
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
		stpd.sz_cid = strCid;
		stpd.l_antwait_flg = 0;
		stpd.l_ant_time = 2;
	}
	else if (s == 2)	// ���V�Y34m
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
		stpd.sz_cid = strCid;
		stpd.l_antwait_flg = 0;
		stpd.l_ant_time = 2;
	}
	else if (s == 3)	// ���V�Y20m
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
		stpd.sz_cid = strCid;
		stpd.l_antwait_flg = 0;
		stpd.l_ant_time = 2;
	}

	if (COprPlnData::WritePlanFile(satellite, pid, stpd) != -1)
	{
		// �v��t�@�C���o�^
		COprPlnData::RegistPlanNew(satellite, pid);
	}

	CDialogEx::OnOK();
}

void CInputVlbi::GetPlanID(CString strDay, CString &id)
{
	CGraphMutex::Lock(eFile);

	stPlanListData pld;

	vector<stPlanListData>& planlist = theApp.GetSatelliteData().GetPlanList();
	vector<stPlanListData>::iterator itrt;

	int num = 0;
	for (itrt = planlist.begin(); itrt != planlist.end(); itrt++){
		if ((*itrt).strSatelliteName.CompareNoCase(_T("vlbi")) == 0){
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
