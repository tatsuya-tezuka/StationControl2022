/*============================================================================*/
/*! UplinkCtrl.cpp

-# �q���ߑ���ʁ@�A�b�v�����N����
*/
/*============================================================================*/
// UplinkCtrl.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "UplinkCtrl.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"
#include "SatelliteCapture.h"

// CUplinkCtrl �_�C�A���O

IMPLEMENT_DYNAMIC(CUplinkCtrl, CDialogBase)

/*============================================================================*/
/*! CUplinkCtrl

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CUplinkCtrl::CUplinkCtrl(CWnd* pParent /*=NULL*/)
	: CDialogBase(CUplinkCtrl::IDD, pParent)
	, m_strRfTripMonitor(_T(""))
	, m_strRealAz(_T(""))
	, m_strAzSkyLIne(_T(""))
	, m_rfTripMonitor(eRfTripMonitorType_Null)
{

}

/*============================================================================*/
/*! CUplinkCtrl

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CUplinkCtrl::~CUplinkCtrl()
{
}

void CUplinkCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UPLINKCTRL_RFTRIPMONITOR_COMBO, m_CCBoxRfTripMonitor);
	DDX_Text(pDX, IDC_UPLINKCTRL_RFTRIPNONITOR_EDIT, m_strRfTripMonitor);
	DDX_Text(pDX, IDC_UPLINKCTRL_REALAZ_EDIT, m_strRealAz);
	DDX_Text(pDX, IDC_UPLINKCTRL_AZSKYLINE_EDIT, m_strAzSkyLIne);
}


BEGIN_MESSAGE_MAP(CUplinkCtrl, CDialogBase)
	ON_BN_CLICKED(IDC_UPLINKCTRL_RFTRIPMONITOR_BUTTON, &CUplinkCtrl::OnBnClickedUplinkctrlRftripmonitorButton)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CUplinkCtrl ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CUplinkCtrl

-# �ݒ�{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CUplinkCtrl::OnBnClickedUplinkctrlRftripmonitorButton()
{
	CSatelliteCapture* pWnd = (CSatelliteCapture*)GetParent();
	if (pWnd == NULL)
		return;

	// �����[�g�V�F���ŋǉ^�p�ǐ��T�[�o�ɒʒm
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = SEND_NOTICE_SH;
	CString arg;
	unsigned char binary = 0;
	CString strAuto;
	CString strManual;

	if (strAuto.LoadString(IDS_DOWNLINK_CTRLMODE_AUTO) != TRUE)
		return;
	if (strManual.LoadString(IDS_DOWNLINK_CTRLMODE_MANUAL) != TRUE)
		return;
	CString strCombo = GetRfTripMonitorStr();
	UINT nCtrlMod = pWnd->GetCtrlMode();
	unsigned char cmd;
	unsigned char rng;
	unsigned char downlink = pWnd->GetDownLinkBinary();
	unsigned char uplink = 0;
	unsigned char way = pWnd->GetWayBinary();

	if (strCombo.CollateNoCase(strAuto) == 0)
	{
		// ����
		uplink = 1;
	}
	else if (strCombo.CollateNoCase(strManual) == 0)
	{
		// �蓮
		uplink = 0;
	}

	// CMD��RNG�̓A�b�v�����N�ɍ��킹��
	cmd = uplink;
	rng = uplink;

	binary = way << eSatellieMode_Way |						/* 1Way/2Way/Dup�ݒ� */
		uplink << eSatellieMode_UpLink |					/* �A�b�v�����N����ݒ� */
		downlink << eSatellieMode_DownLink |				/* �_�E�������N����ݒ� */
		rng << eSatellieMode_RNG |							/* RNG����ݒ� */
		cmd << eSatellieMode_CMD |							/* CMD����ݒ� */
		1 << eSatellieMode_AntStandBy |						/* ANT�҂��󂯎w����� */
		1 << eSatellieMode_AutoManual;						/* �����ߑ�/�蓮�ߑ���� */

	int nStation = theApp.GetSelectStation();
	arg.Format(_T("%s %d 0x%02x"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_ACQMOD, binary);

	stParam.strRemShellParam.Format(arg);

	int err = CSshShell::ExecShell(stParam);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnBnClickedDownlinkcaptureCtrlmodeButton"), _T("debug"), strDebug, nLogEx::debug);
	}
	if (err == 0)
	{
		// �������Ȃ�
	}

#ifdef _DEBUG
	if (uplink == 1)
	{
		// ����
		m_rfTripMonitor = eRfTripMonitorType_Auto;
	}
	else
	{
		// �蓮
		m_rfTripMonitor = eRfTripMonitorType_Manual;
	}
	CreateDlgItems();
#endif
}


/*============================================================================*/
/*! CUplinkCtrl

-# �_�C�A���O����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
BOOL CUplinkCtrl::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_rfTripMonitor = eRfTripMonitorType_Null;

	// �_�C�A���O�A�C�e���𐶐�����
	if (CreateDlgItems() == FALSE)
		return FALSE;

	// �Ď��f�[�^�擾
	if (UpdateKanshiData() == FALSE)
		return FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CUplinkCtrl

-# �`�掞�ɌĂ΂�A�w��R���g���[���̔w�i��h��Ԃ��u���V�̃n���h����Ԃ�

@param	pDC�F�q�E�B���h�E�̃f�B�X�v���C�R���e�L�X�g�̃|�C���^
@param	pWnd�F�F���i�[���ꂽ�R���g���[���̃|�C���^
@param	nCtrlColor�F�R���g���[���̎��
@retval	�R���g���[���̔w�i��h��Ԃ��u���V�̃n���h��

*/
/*============================================================================*/
HBRUSH CUplinkCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogBase::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		// �ÓI�e�L�X�g�w�i�F
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(DC_BRUSH);
		hbr = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
		break;

	case CTLCOLOR_DLG:
		// �_�C�A���O�̔w�i�F
		if (pWnd == this)
		{
			hbr = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
		}
		break;

	default:
		break;
	}

	// TODO: ����l���g�p�������Ȃ��ꍇ�͕ʂ̃u���V��Ԃ��܂��B
	return hbr;
}


/*============================================================================*/
/*! CUplinkCtrl

-# �_�C�A���O�A�C�e���𐶐�����

@param	�Ȃ�
@retval	TRUE: �������� / FALSE: �������s

*/
/*============================================================================*/
BOOL CUplinkCtrl::CreateDlgItems()
{
	TCHAR szMsg[256];

	// �R���{�{�b�N�X�̃N���A
	m_CCBoxRfTripMonitor.ResetContent();

	// �G�f�B�b�g�R���g���[��������
	// �R���{�{�b�N�X������
	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_DOWNLINK_CTRLMODE_AUTO, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	if (m_rfTripMonitor == eRfTripMonitorType_Auto)
	{
		m_strRfTripMonitor = CString(szMsg);
	}
	else
	{
		m_CCBoxRfTripMonitor.InsertString(-1, szMsg);
	}

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_DOWNLINK_CTRLMODE_MANUAL, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	if (m_rfTripMonitor == eRfTripMonitorType_Manual)
	{
		m_strRfTripMonitor = CString(szMsg);
	}
	else
	{
		m_CCBoxRfTripMonitor.InsertString(-1, szMsg);
	}

	m_CCBoxRfTripMonitor.SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;
}


/*============================================================================*/
/*! CUplinkCtrl

-# �R���{�{�b�N�X��RF TRIP�Ď���������擾����

@param	�Ȃ�
@retval	���䃂�[�h������

*/
/*============================================================================*/
CString CUplinkCtrl::GetRfTripMonitorStr()
{
	CString str;
	UpdateData(TRUE);
	int nIndex = m_CCBoxRfTripMonitor.GetCurSel();
	if (nIndex < 0)
		return str;
	m_CCBoxRfTripMonitor.GetLBText(nIndex, str);

	return str;
}


/*============================================================================*/
/*! CUplinkCtrl

-# RF TRIP�Ď���ݒ肷��

@param	rfTripMonitorType�F���䃂�[�h
@retval	�Ȃ�

*/
/*============================================================================*/
void CUplinkCtrl::SetRfTripMonitor(char szRfTripMode[])
{
	eRfTripMonitorType rfTripMonitor;

	CString str(szRfTripMode);
	if (str.CompareNoCase(_T("����")) == 0)
	{
		rfTripMonitor = eRfTripMonitorType_Auto;
	}
	else
	{
		rfTripMonitor = eRfTripMonitorType_Manual;
	}

	if (m_rfTripMonitor != rfTripMonitor)
	{
		m_rfTripMonitor = rfTripMonitor;
		CreateDlgItems();
	}
}


/*============================================================================*/
/*! CUplinkCtrl

-# RF TRIP�Ď���NULL�ݒ肷��

@param	rfTripMonitorType�F���䃂�[�h
@retval	�Ȃ�

*/
/*============================================================================*/
void CUplinkCtrl::SetRfTripMonitorNull()
{
	eRfTripMonitorType rfTripMonitor = eRfTripMonitorType_Null;
	if (m_rfTripMonitor != rfTripMonitor)
	{
		m_rfTripMonitor = rfTripMonitor;
		CreateDlgItems();
	}
}


/*============================================================================*/
/*! CUplinkCtrl

-# �Ď�����RF-TRIP���[�h��REAL AZ, AZ������SKYLINE���擾���čX�V����

@param	�Ȃ�
@retval	TRUE:���� / FALSE:���s

*/
/*============================================================================*/
BOOL CUplinkCtrl::UpdateKanshiData()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT sta = theApp.GetSelectStation();

	char		ca_rfTripMode[30];		/* RF-TRIP���[�h   */
	double		db_realAZ;				/* REAL AZ */

	/* RF-TRIP���[�h�擾 */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(CTRL_P_CAP_UPLINK));
	if (obs_adr == NULL)
	{
#ifdef _DEBUG
#else
		SetRfTripMonitorNull();
#endif
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		strcpy_s(ca_rfTripMode, 30, obs_adr->sz_sts_name);
		SetRfTripMonitor(ca_rfTripMode);
	}
	else
	{
		SetRfTripMonitorNull();
	}

	/* REAL AZ�擾 */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(REAL_AZ));
	if (obs_adr == NULL)
	{
		m_strRealAz.Format(_T(""));
		return FALSE;
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		db_realAZ = obs_adr->d_data;
		m_strRealAz.Format(_T("%0.1f��"), db_realAZ);
	}
	else
	{
		m_strRealAz.Format(_T(""));
		return FALSE;
	}

	CGraphMutex::Lock(eRadar);

	/* AZ������SKYLINE */
	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(theApp.GetSelectStation());
	if (pCtrlMonData == NULL)
	{
		m_strAzSkyLIne.Format(_T(""));
	}
	else if (db_realAZ >= 0 && db_realAZ < 360)
	{
		if (pCtrlMonData->m_queAzElSkyline.size() > db_realAZ)
		{
			double db_skylineEL = pCtrlMonData->m_queAzElSkyline[(UINT)db_realAZ].d_EL;
			m_strAzSkyLIne.Format(_T("%0.1f��"), db_skylineEL);
		}
	}
	else
	{
		m_strAzSkyLIne.Format(_T(""));
	}

	CGraphMutex::Unlock(eRadar);

	UpdateData(FALSE);

	return TRUE;
}


/*============================================================================*/
/*! CSatelliteCapture

-# �A�b�v�����N����o�C�i�����擾����

@param	�Ȃ�
@retval	���䃂�[�h

*/
/*============================================================================*/
unsigned char CUplinkCtrl::GetUpLinkBinary()
{
	unsigned char binary = 1;

	switch (m_rfTripMonitor)
	{
	case eCtrlModeType_Auto:
		binary = 1;
		break;
	case eCtrlModeType_Manual:
		binary = 0;
		break;
	default:
		break;
	}

	return binary;
}
