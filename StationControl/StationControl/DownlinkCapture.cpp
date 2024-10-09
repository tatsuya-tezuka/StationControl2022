/*============================================================================*/
/*! DownlinkCapture.cpp

-# �q���ߑ���ʁ@�_�E�������N�ߑ�
*/
/*============================================================================*/
// DownlinkCapture.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "DownlinkCapture.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"
#include "SatelliteCapture.h"

// CDownlinkCapture �_�C�A���O

IMPLEMENT_DYNAMIC(CDownlinkCapture, CDialogBase)

/*============================================================================*/
/*! DownlinkCapture

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CDownlinkCapture::CDownlinkCapture(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDownlinkCapture::IDD, pParent)
	, m_strRcvFreqMode(_T(""))
	, m_strCtrlMode(_T(""))
	, m_rcvFreqMode(eRcvFreqModeType_Other)
	, m_ctrlMode(eCtrlModeType_Null)
{

}

/*============================================================================*/
/*! CSatelliteCapture

-# �R���X�g���N�^

@param	IDD�FIDD
@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CDownlinkCapture::CDownlinkCapture(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialogBase(IDD, pParent)
{

}

/*============================================================================*/
/*! CDownlinkCapture

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CDownlinkCapture::~CDownlinkCapture()
{
}

void CDownlinkCapture::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DOWNLINKCAPTURE_RCVFREQMODE_COMBO, m_CCBoxRcvFreqMode);
	DDX_Control(pDX, IDC_DOWNLINKCAPTURE_CTRLMODE_COMBO, m_CCBoxCtrlMode);
	DDX_Text(pDX, IDC_DOWNLINKCAPTURE_RCVFREQMODE_EDIT, m_strRcvFreqMode);
	DDX_Text(pDX, IDC_DOWNLINKCAPTURE_CTRLMODE_EDIT, m_strCtrlMode);
}


BEGIN_MESSAGE_MAP(CDownlinkCapture, CDialogBase)
	ON_BN_CLICKED(IDC_DOWNLINKCAPTURE_RCVFREQMODE_BUTTON, &CDownlinkCapture::OnBnClickedDownlinkcaptureRcvfreqmodeButton)
	ON_BN_CLICKED(IDC_DOWNLINKCAPTURE_CTRLMODE_BUTTON, &CDownlinkCapture::OnBnClickedDownlinkcaptureCtrlmodeButton)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_DOWNLINKCAPTURE_CTRLMODE_COMBO, &CDownlinkCapture::OnCbnSelchangeDownlinkcaptureCtrlmodeCombo)
END_MESSAGE_MAP()


// CDownlinkCapture ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CDownlinkCapture

-# ��M���g�����[�h�̃{�^���N���b�N���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CDownlinkCapture::OnBnClickedDownlinkcaptureRcvfreqmodeButton()
{
	// �����[�g�V�F���ŋǉ^�p�ǐ��T�[�o�ɒʒm
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = SEND_NOTICE_SH;
	CString arg;
	unsigned char binary = GetRcvFreqModNotifyData();

	int nStation = theApp.GetSelectStation();
	arg.Format(_T("%s %d 0x%02x"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_ACQMOD, binary);

	stParam.strRemShellParam.Format(arg);

	int err = CSshShell::ExecShell(stParam);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnBnClickedDownlinkcaptureRcvfreqmodeButton"), _T("debug"), strDebug, nLogEx::debug);
	}
	if (err == 0)
	{
		// �������Ȃ�
	}

#ifdef _DEBUG
	CString str1Way;
	CString str2Way;
	CString strDup;
	if (str1Way.LoadString(IDS_DOWNLINK_RCVFREQMODE_1WAY) != TRUE)
		return;
	if (str2Way.LoadString(IDS_DOWNLINK_RCVFREQMODE_2WAY) != TRUE)
		return;
	if (strDup.LoadString(IDS_DOWNLINK_RCVFREQMODE_DUP) != TRUE)
		return;
	// �R���{�{�b�N�X�����M���g�����[�h�擾
	CString strCombo = GetRcvFreqModeComboStr();

	// WAY
	if (strCombo.CollateNoCase(str1Way) == 0)
	{
		m_rcvFreqMode = eRcvFreqModeType_1Way;
	}
	else if (strCombo.CollateNoCase(str2Way) == 0)
	{
		m_rcvFreqMode = eRcvFreqModeType_2Way;
	}
	else if (strCombo.CollateNoCase(strDup) == 0)
	{
		m_rcvFreqMode = eRcvFreqModeType_Dup;
	}
	else
	{
		m_rcvFreqMode = eRcvFreqModeType_Other;
	}
	CreateDlgRcvFreqMode();
#endif
}


/*============================================================================*/
/*! CDownlinkCapture

-# ���䃂�[�h�̃{�^���N���b�N���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CDownlinkCapture::OnBnClickedDownlinkcaptureCtrlmodeButton()
{
	// �����[�g�V�F���ŋǉ^�p�ǐ��T�[�o�ɒʒm
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = SEND_NOTICE_SH;
	CString arg;

	unsigned char binary = 0;
	CSatelliteCapture* pWnd = (CSatelliteCapture*)GetParent();
	if (pWnd == NULL)
		return;

	// �A�b�v�����N, CMD, RNG, WAY����擾
	unsigned char cmd = pWnd->GetCmdBinary();			// �_�E�������N�͊Ď�����CMD�擾
	unsigned char rng = pWnd->GetRngBinary();			// �_�E�������N�͊Ď�����RNG�擾
	unsigned char downlink = GetCtrlModeFromCombo();
	unsigned char uplink = pWnd->GetUpLinkBinary();
	unsigned char way = GetWayBinary();

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
	if (downlink == 1)
	{
		// ����
		m_ctrlMode = eCtrlModeType_Auto;
	}
	else
	{
		// �蓮
		m_ctrlMode = eCtrlModeType_Manual;
	}
	CreateDlgCtrlMode();
#endif

}


/*============================================================================*/
/*! CDownlinkCapture

-# �_�C�A���O����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
BOOL CDownlinkCapture::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_rcvFreqMode = eRcvFreqModeType_Other;
	m_ctrlMode = eCtrlModeType_Null;

	// �_�C�A���O�A�C�e���𐶐�����
	if (CreateDlgItems() == FALSE)
		return FALSE;

	// �Ď��f�[�^�擾
	(void)UpdateKanshiData();

	// ��M���g�����[�h�̃R���g���[���̕\���A��\��
	ShowRcvFreqMode();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CDownlinkCapture

-# �`�掞�ɌĂ΂�A�w��R���g���[���̔w�i��h��Ԃ��u���V�̃n���h����Ԃ�

@param	pDC�F�q�E�B���h�E�̃f�B�X�v���C�R���e�L�X�g�̃|�C���^
@param	pWnd�F�F���i�[���ꂽ�R���g���[���̃|�C���^
@param	nCtrlColor�F�R���g���[���̎��
@retval	�R���g���[���̔w�i��h��Ԃ��u���V�̃n���h��

*/
/*============================================================================*/
HBRUSH CDownlinkCapture::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
/*! CDownlinkCapture

-# �_�C�A���O�A�C�e���𐶐�����

@param	�Ȃ�
@retval	TRUE: �������� / FALSE: �������s

*/
/*============================================================================*/
BOOL CDownlinkCapture::CreateDlgItems()
{
	if (CreateDlgRcvFreqMode() && CreateDlgCtrlMode())
	{
		return TRUE;
	}

	return FALSE;
}


/*============================================================================*/
/*! CDownlinkCapture

-# ��M���g�����[�h�_�C�A���O�A�C�e���𐶐�����

@param	�Ȃ�
@retval	TRUE: �������� / FALSE: �������s

*/
/*============================================================================*/
BOOL CDownlinkCapture::CreateDlgRcvFreqMode()
{
	TCHAR szMsg[256];

	// �R���{�{�b�N�X�̃N���A
	m_CCBoxRcvFreqMode.ResetContent();

	// �G�f�B�b�g�R���g���[��������
	// �R���{�{�b�N�X������
	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_DOWNLINK_RCVFREQMODE_1WAY, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	if (m_rcvFreqMode == eRcvFreqModeType_1Way)
	{
		m_strRcvFreqMode = CString(szMsg);
	}
	else
	{
		m_CCBoxRcvFreqMode.InsertString(-1, szMsg);
	}

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_DOWNLINK_RCVFREQMODE_2WAY, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	if (m_rcvFreqMode == eRcvFreqModeType_2Way)
	{
		m_strRcvFreqMode = CString(szMsg);
	}
	else
	{
		m_CCBoxRcvFreqMode.InsertString(-1, szMsg);
	}

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_DOWNLINK_RCVFREQMODE_DUP, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	if (m_rcvFreqMode == eRcvFreqModeType_Dup)
	{
		m_strRcvFreqMode = CString(szMsg);
	}
	else
	{
		m_CCBoxRcvFreqMode.InsertString(-1, szMsg);
	}

	if (m_rcvFreqMode == eRcvFreqModeType_Other)
	{
		m_strRcvFreqMode = CString(_T(""));
	}

	m_CCBoxRcvFreqMode.SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;
}


/*============================================================================*/
/*! CDownlinkCapture

-# ���䃂�[�h�_�C�A���O�A�C�e���𐶐�����

@param	�Ȃ�
@retval	TRUE: �������� / FALSE: �������s

*/
/*============================================================================*/
BOOL CDownlinkCapture::CreateDlgCtrlMode()
{
	TCHAR szMsg[256];

	// �R���{�{�b�N�X�̃N���A
	m_CCBoxCtrlMode.ResetContent();

	// �G�f�B�b�g�R���g���[��������
	// �R���{�{�b�N�X������
	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_DOWNLINK_CTRLMODE_AUTO, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	if (m_ctrlMode == eCtrlModeType_Auto)
	{
		m_strCtrlMode = CString(szMsg);
	}
	else
	{
		m_CCBoxCtrlMode.InsertString(-1, szMsg);
	}

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_DOWNLINK_CTRLMODE_MANUAL, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	if (m_ctrlMode == eCtrlModeType_Manual)
	{
		m_strCtrlMode = CString(szMsg);
	}
	else
	{
		m_CCBoxCtrlMode.InsertString(-1, szMsg);
	}

	// �Ď��𖢎�M��
	if (m_ctrlMode == eCtrlModeType_Null)
	{
		m_strCtrlMode = CString(_T(""));
	}

	m_CCBoxCtrlMode.SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;
}


/*============================================================================*/
/*! CDownlinkCapture

-# �R���{�{�b�N�X�̎�M���g�����[�h��������擾����

@param	�Ȃ�
@retval	��M���g�����[�h������

*/
/*============================================================================*/
CString CDownlinkCapture::GetRcvFreqModeComboStr()
{
	CString str;
	UpdateData(TRUE);
	int nIndex = m_CCBoxRcvFreqMode.GetCurSel();
	if (nIndex < 0)
		return str;
	m_CCBoxRcvFreqMode.GetLBText(nIndex, str);

	return str;
}


/*============================================================================*/
/*! CDownlinkCapture

-# ��M���g�����[�h��ݒ肷��

@param	rcvFreqType�F��M���g�����[�h
@retval	�Ȃ�

*/
/*============================================================================*/
void CDownlinkCapture::SetRcvFreqMode(char szRcvFreqMode[])
{
	eRcvFreqModeType	rcvFreqMode;

	CString str(szRcvFreqMode);
	if (str.CompareNoCase(_T("1WAY")) == 0)
	{
		rcvFreqMode = eRcvFreqModeType_1Way;
	}
	else if (str.CompareNoCase(_T("2WAY")) == 0)
	{
		rcvFreqMode = eRcvFreqModeType_2Way;
	}
	else if (str.CompareNoCase(_T("DUP")) == 0)
	{
		rcvFreqMode = eRcvFreqModeType_Dup;
	}
	else
	{
		rcvFreqMode = eRcvFreqModeType_Other;
	}

	if (m_rcvFreqMode != rcvFreqMode)
	{
		m_rcvFreqMode = rcvFreqMode;
		CreateDlgRcvFreqMode();
	}
}


/*============================================================================*/
/*! CDownlinkCapture

-# �R���{�{�b�N�X�̐��䃂�[�h��������擾����

@param	�Ȃ�
@retval	���䃂�[�h������

*/
/*============================================================================*/
CString CDownlinkCapture::GetCtrlModeComboStr()
{
	CString str;
	UpdateData(TRUE);
	int nIndex = m_CCBoxCtrlMode.GetCurSel();
	if (nIndex < 0)
		return str;
	m_CCBoxCtrlMode.GetLBText(nIndex, str);

	return str;
}


/*============================================================================*/
/*! CDownlinkCapture

-# ���䃂�[�h��ݒ肷��

@param	���䃂�[�h
@retval	�Ȃ�

*/
/*============================================================================*/
void CDownlinkCapture::SetCtrlMode(char szCtrlMode[])
{
	eCtrlModeType ctrlMode;

	CString str(szCtrlMode);
	if (str.CompareNoCase(_T("����")) == 0)
	{
		ctrlMode = eCtrlModeType_Auto;
	}
	else
	{
		ctrlMode = eCtrlModeType_Manual;
	}

	if (m_ctrlMode != ctrlMode)
	{ 
		m_ctrlMode = ctrlMode;
		CreateDlgCtrlMode();
	}
}


/*============================================================================*/
/*! CDownlinkCapture

-# ���䃂�[�h��Null�ݒ肷��

@param	���䃂�[�h
@retval	�Ȃ�

*/
/*============================================================================*/
void CDownlinkCapture::SetCtrlModeNull()
{
	eCtrlModeType ctrlMode = eCtrlModeType_Null;
	if (m_ctrlMode != ctrlMode)
	{
		m_ctrlMode = ctrlMode;
		CreateDlgCtrlMode();
	}
}


/*============================================================================*/
/*! CDownlinkCapture

-# �Ď������M���g�����[�h�Ɛ��䃂�[�h���擾���čX�V����

@param	�Ȃ�
@retval	TRUE:���� / FALSE:���s

*/
/*============================================================================*/
BOOL CDownlinkCapture::UpdateKanshiData()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT sta = theApp.GetSelectStation();

	char		ca_rcvFreqMode[30];		/* ��M���g�����[�h   */
	char		ca_ctrlMode[30];		/* ���䃂�[�h         */

	/* ��M���g�����[�h�擾 */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(CTRL_P_CAP_WAY));
	if (obs_adr == NULL)
	{
#ifdef _DEBUG
#else
	SetRcvFreqMode("");
#endif
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		strcpy_s(ca_rcvFreqMode, 30, obs_adr->sz_sts_name);
		SetRcvFreqMode(ca_rcvFreqMode);
	}
	else
	{
		SetRcvFreqMode("");
	}

	/* ���䃂�[�h�擾 */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(CTRL_P_CAP_RX));
	if (obs_adr == NULL)
	{
#ifdef _DEBUG
#else
		SetCtrlModeNull();
#endif
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		strcpy_s(ca_ctrlMode, 30, obs_adr->sz_sts_name);
		SetCtrlMode(ca_ctrlMode);
	}
	else
	{
		SetCtrlModeNull();
	}

	// ��M���g�����[�h�̃R���g���[���̕\���A��\��
	ShowRcvFreqMode();

	return TRUE;
}


/*============================================================================*/
/*! CDownlinkCapture

-# ��M���g�����[�h�ʒm�f�[�^���擾����

@param	�Ȃ�
@retval	�ʒm�f�[�^

*/
/*============================================================================*/
unsigned char CDownlinkCapture::GetRcvFreqModNotifyData()
{
	unsigned char binary = 0;

	CSatelliteCapture* pWnd = (CSatelliteCapture*)GetParent();
	if (pWnd == NULL)
		return binary;

	// �_�E�������N�A�A�b�v�����N, CMD, RNG, WAY����擾
	unsigned char uplink = pWnd->GetUpLinkBinary();
	unsigned char cmd = pWnd->GetCmdBinary();
	unsigned char rng = pWnd->GetRngBinary();

	CString str1Way;
	CString str2Way;
	CString strDup;
	if (str1Way.LoadString(IDS_DOWNLINK_RCVFREQMODE_1WAY) != TRUE)
		return binary;
	if (str2Way.LoadString(IDS_DOWNLINK_RCVFREQMODE_2WAY) != TRUE)
		return binary;
	if (strDup.LoadString(IDS_DOWNLINK_RCVFREQMODE_DUP) != TRUE)
		return binary;
	// �R���{�{�b�N�X�����M���g�����[�h�擾
	CString strCombo = GetRcvFreqModeComboStr();

	// WAY
	int way = 0;
	if (strCombo.CollateNoCase(str1Way) == 0)
	{
		way = 0;
	}
	else if (strCombo.CollateNoCase(str2Way) == 0)
	{
		way = 1;
	}
	else if (strCombo.CollateNoCase(strDup) == 0)
	{
		way = 2;
	}

	binary = way << eSatellieMode_Way |						/* 1Way/2Way/Dup�ݒ� */
		uplink << eSatellieMode_UpLink |					/* �A�b�v�����N����ݒ� */
		0 << eSatellieMode_DownLink |						/* �_�E�������N����ݒ�i�蓮�Œ�j */
		rng << eSatellieMode_RNG |							/* RNG����ݒ� */
		cmd << eSatellieMode_CMD |							/* CMD����ݒ� */
		1 << eSatellieMode_AntStandBy |						/* ANT�҂��󂯎w����� */
		1 << eSatellieMode_AutoManual;						/* �����ߑ�/�蓮�ߑ���� */

	return binary;
}


/*============================================================================*/
/*! CDownlinkCapture

-# 1WAY/2WAY/Dup�f�[�^���擾����

@param	�Ȃ�
@retval	�ʒm�f�[�^

*/
/*============================================================================*/
unsigned char CDownlinkCapture::GetWayBinary()
{
	unsigned char binary = 0;

	switch (m_rcvFreqMode)
	{
	case eRcvFreqModeType_1Way:
		binary = 0;
		break;
	case eRcvFreqModeType_2Way:
		binary = 1;
		break;
	case eRcvFreqModeType_Dup:
		binary = 2;
		break;
	default:
		break;
	}

	return binary;
}


/*============================================================================*/
/*! CDownlinkCapture

-# �_�E�������N����o�C�i�����擾����

@param	�Ȃ�
@retval	���䃂�[�h

*/
/*============================================================================*/
unsigned char CDownlinkCapture::GetDownLinkBinary()
{
	unsigned char binary = 1;

	switch (m_ctrlMode)
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


/*============================================================================*/
/*! CDownlinkCapture

-# ���䃂�[�h���I��ύX���ꂽ

@param	�Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CDownlinkCapture::OnCbnSelchangeDownlinkcaptureCtrlmodeCombo()
{
}


/*============================================================================*/
/*! CDownlinkCapture

-# ���䃂�[�h���R���{����擾

@param	�Ȃ�
@retval 1:���� / 0:�蓮

*/
/*============================================================================*/
unsigned char CDownlinkCapture::GetCtrlModeFromCombo()
{
	unsigned char downlink = 0;

	CString strAuto;
	CString strManual;

	if (strAuto.LoadString(IDS_DOWNLINK_CTRLMODE_AUTO) != TRUE)
		return downlink;
	if (strManual.LoadString(IDS_DOWNLINK_CTRLMODE_MANUAL) != TRUE)
		return downlink;

	CString strCombo = GetCtrlModeComboStr();
	if (strCombo.CollateNoCase(strAuto) == 0)
	{
		// ����
		downlink = 1;
	}
	else if (strCombo.CollateNoCase(strManual) == 0)
	{
		// �蓮
		downlink = 0;
	}

	return downlink;
}


/*============================================================================*/
/*! CDownlinkCapture

-# ��M���g�����[�h�̃R���g���[���̕\���A��\��

@param	�Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CDownlinkCapture::ShowRcvFreqMode()
{
	CWnd* pButtonWnd = GetDlgItem(IDC_DOWNLINKCAPTURE_RCVFREQMODE_BUTTON);
	if (pButtonWnd == NULL)
		return;

//	if (GetCtrlModeFromCombo() == 1)
	if (m_ctrlMode == eCtrlModeType_Auto)
	{
		// �����̏ꍇ�A��M���g�����[�h�̃R���{�Ɛݒ�{�^�����f�B�Z�[�u��
		m_CCBoxRcvFreqMode.EnableWindow(FALSE);
		pButtonWnd->EnableWindow(FALSE);
	}
	else
	{
		// �蓮�̏ꍇ�A��M���g�����[�h�̃R���{�Ɛݒ�{�^�����C�l�[�u��
		m_CCBoxRcvFreqMode.EnableWindow(TRUE);
		pButtonWnd->EnableWindow(TRUE);
	}
}