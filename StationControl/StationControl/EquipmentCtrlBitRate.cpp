/*============================================================================*/
/*! EquipmentCtrlBitRate.cpp

-# �ݔ�����-�r�b�g���[�g�ύX���
*/
/*============================================================================*/
// EquipmentCtrlBitRate.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "EquipmentCtrlBitRate.h"
#include "afxdialogex.h"


#define MAC_MAX     256

// CEquipmentCtrlBitRate �_�C�A���O

IMPLEMENT_DYNAMIC(CEquipmentCtrlBitRate, CDialogEx)

CEquipmentCtrlBitRate::CEquipmentCtrlBitRate(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEquipmentCtrlBitRate::IDD, pParent)
	, m_DataTimeDay(0)
	, m_DataTimeTime(0)
	, m_strSatCmb(_T(""))
	, m_strWindowTitle(_T(""))
{

}

CEquipmentCtrlBitRate::~CEquipmentCtrlBitRate()
{
}

void CEquipmentCtrlBitRate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLG_EQUIPCTRL_BITRATE_SAT_CMB, m_SatCmb);
	DDX_Control(pDX, IDC_DLG_EQUIPCTRL_BITRATE_BR_LIST, m_BitRateListBox);
	DDX_Control(pDX, IDC_DLG_EQUIPCTRL_BITRATE_DP_DATA, m_DataTimeDayCtrl);
	DDX_DateTimeCtrl(pDX, IDC_DLG_EQUIPCTRL_BITRATE_DP_DATA, m_DataTimeDay);
	DDX_DateTimeCtrl(pDX, IDC_DLG_EQUIPCTRL_BITRATE_DP_TIME, m_DataTimeTime);
	DDX_Control(pDX, IDC_DLG_EQUIPCTRL_BITRATE_DP_TIME, m_DateTimeTimeCtrl);
	DDX_Control(pDX, IDC_CHECK1, m_SpecifyDateChkBtn);
	DDX_CBString(pDX, IDC_DLG_EQUIPCTRL_BITRATE_SAT_CMB, m_strSatCmb);
}


BEGIN_MESSAGE_MAP(CEquipmentCtrlBitRate, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK1, &CEquipmentCtrlBitRate::OnBnClickedCheck1)
	ON_CBN_SELCHANGE(IDC_DLG_EQUIPCTRL_BITRATE_SAT_CMB, &CEquipmentCtrlBitRate::OnCbnSelchangeDlgEquipctrlBitrateSatCmb)
	ON_BN_CLICKED(IDOK, &CEquipmentCtrlBitRate::OnBnClickedOk)
END_MESSAGE_MAP()


// CEquipmentCtrlBitRate ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CEquipmentCtrlBitRate

-# �_�C�A���O����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CEquipmentCtrlBitRate::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (!m_strWindowTitle.IsEmpty())
		SetWindowText(m_strWindowTitle);

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X

	// ���t�t�H�[�}�b�g�ύX
	m_DataTimeDayCtrl.SetFormat(_T("yyyy-MM-dd"));

	// ���ݎ����擾
	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	CTime time = CTime(timeSys);

	// �����w��R���g���[��������
	m_SpecifyDateChkBtn.SetCheck(0);
	m_DataTimeDayCtrl.EnableWindow(FALSE);
	m_DataTimeDay = time;
	m_DateTimeTimeCtrl.EnableWindow(FALSE);
	m_DataTimeTime = time;

	// �r�b�g���[�g���R�}���h�̏ꍇ�A�w�����w��x�͔񊈐�
	if (m_bCmdBitRate)
	{
		m_SpecifyDateChkBtn.EnableWindow(FALSE);
	}

#if 0
	GetBitRateListSat();
#else
	// �q�����R���{�ݒ�
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	vector<CString>	 &saNameList = dba.GetSatNameList();
	for (int i = 0; i < saNameList.size(); ++i)
	{
		if (saNameList[i] != _T("DEFAULT"))
		{
			m_SatCmb.InsertString(-1, saNameList[i]);
		}
	}
#endif

	// �Ď��f�[�^�x�[�X��ɉq����������΁A���̉q�������f�t�H�ݒ�B
	CString sat = _T("");
	UINT sta = theApp.GetSelectStation();
	obs_adr = (CEngValCnv::obsname_t *)theApp.GetSatelliteData().GetDBAccessCls().GetObsname_t(sta, CTRL_P_SAT);
	if (obs_adr != NULL)
	{
		sat = CString(obs_adr->sz_sts_name);

		int idx = m_SatCmb.FindStringExact(-1, sat);
		if (idx != CB_ERR)
		{
			m_SatCmb.SetCurSel(idx);
		}
		else
		{
			m_SatCmb.SetCurSel(0);
			m_SatCmb.GetLBText(0, sat);
		}
	}
	else
	{
		m_SatCmb.SetCurSel(0);
		m_SatCmb.GetLBText(0, sat);
	}

	// �r�b�g���[�g���X�g�ݒ�
	GetBitRateList(sat);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CEquipmentCtrlBitRate

-# �w���t�w��x�`�F�b�N�{�b�N�X��������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentCtrlBitRate::OnBnClickedCheck1()
{
	if (m_SpecifyDateChkBtn.GetCheck() == 0)
	{
		m_DataTimeDayCtrl.EnableWindow(FALSE);
		m_DateTimeTimeCtrl.EnableWindow(FALSE);
	}
	else
	{
		m_DataTimeDayCtrl.EnableWindow(TRUE);
		m_DateTimeTimeCtrl.EnableWindow(TRUE);
	}
}


/*============================================================================*/
/*! CEquipmentCtrlBitRate

-# �w�q�����x�R���{�{�b�N�X�ύX����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentCtrlBitRate::OnCbnSelchangeDlgEquipctrlBitrateSatCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_SatCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_SatCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strSatCmb) 
	{
		return;
	}

	GetBitRateList(str);
}


/*============================================================================*/
/*! CEquipmentCtrlBitRate

-# �w�q�����x���̃r�b�g���[�g���擾

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentCtrlBitRate::GetBitRateList(CString satname)
{
#if 0
	// �r�b�g���[�g�擾
	m_BitRateListBox.ResetContent();
	vector<CString> &brList = m_SatBitRateMap[satname];
	for(int i=0; i<brList.size(); ++i)
	{
		m_BitRateListBox.InsertString(-1, brList[i]);
	}

#else
	struct satinf_t *satinf_adr;
	int i_init_ret;
	char    *psz_tbl[MAC_MAX];                 /* �}�N�������i�[����e�[�u���̐擪�A�h���X*/
	char    *psz_vec[MAC_MAX];                 /* �}�N�����A�h���X���i�[����e�[�u����  */

	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();

	vector<CString>			&saNameList = dba.GetSatNameList();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();

	// �q���ŗL���̃A�h���X�擾
	satinf_adr = (struct satinf_t *)((__int64*)satAdrList[satname]);
	if (satinf_adr == NULL)
	{
		return;
	}

	// �r�b�g���[�g�擾
	m_BitRateListBox.ResetContent();
	if ((i_init_ret = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(m_strBitRateCtrl).GetBuffer(), psz_tbl, psz_vec, MAC_MAX)) > -1)
	{
		m_BitRateListBox.ResetContent();
		for (int i = 0; i < i_init_ret; ++i) 
		{
			CString strBitRate =  CString(psz_tbl[i]);
			m_BitRateListBox.InsertString(-1, strBitRate);
		}
	}
#endif
}

/*============================================================================*/
/*! CEquipmentCtrlBitRate

-# �w�q�����x���̃r�b�g���[�g���擾

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentCtrlBitRate::GetBitRateListSat()
{
	struct satinf_t *satinf_adr;
	int i_init_ret;
	char    *psz_tbl[MAC_MAX];                 /* �}�N�������i�[����e�[�u���̐擪�A�h���X*/
	char    *psz_vec[MAC_MAX];                 /* �}�N�����A�h���X���i�[����e�[�u����  */

	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	vector<CString>	 &saNameList = dba.GetSatNameList();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();

	m_SatBitRateMap.clear();
	vector<CString> brList;

	for (int i = 0; i < saNameList.size(); ++i)
	{
		if (saNameList[i] != _T("DEFAULT"))
		{
			// �q���ŗL���̃A�h���X�擾
			satinf_adr = (struct satinf_t *)((__int64*)satAdrList[saNameList[i]]);
			if (satinf_adr != NULL)
			{
				// �r�b�g���[�g�擾
				brList.clear();
				if ((i_init_ret = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(m_strBitRateCtrl).GetBuffer(), psz_tbl, psz_vec, MAC_MAX)) > -1)
				{
					for (int b = 0; b < i_init_ret; ++b)
					{
						CString strBitRate = CString(psz_tbl[b]);
						brList.push_back(strBitRate);
					}

					if (brList.size() > 0)
					{
						// �q���I���R���{�{�b�N�X�ݒ�
						m_SatCmb.InsertString(-1, saNameList[i]);

						// ���X�g�ɒǉ�
						m_SatBitRateMap[saNameList[i]] = brList;
					}
				}
			}
		}
	}
}


/*============================================================================*/
/*! CEquipmentCtrlBitRate

-# �w���s�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentCtrlBitRate::OnBnClickedOk()
{
	UpdateData();

	// �I������Ă���r�b�g���[�g�擾
	int idx = m_BitRateListBox.GetCurSel();
	if (idx == LB_ERR)
	{
		CString title;
		GetWindowText(title);
		MessageBox(_T("�r�b�g���[�g��I�����Ă��������B"), title, MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	m_BitRateListBox.GetText(idx, m_stBitRate.bitrate);

	// �I������Ă���q�����擾
	m_stBitRate.satellite = m_strSatCmb;

	// �����w��擾
	m_stBitRate.bTime = m_SpecifyDateChkBtn.GetCheck();
	m_stBitRate.time = CTime(m_DataTimeDay.GetYear(), m_DataTimeDay.GetMonth(), m_DataTimeDay.GetDay(), m_DataTimeTime.GetHour(), m_DataTimeTime.GetMinute(), m_DataTimeTime.GetSecond());

	CDialogEx::OnOK();
}
