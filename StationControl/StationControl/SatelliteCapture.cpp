/*============================================================================*/
/*! SatelliteCapture.cpp

-# �q���ߑ����
*/
/*============================================================================*/
// SatelliteCapture.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "SatelliteCapture.h"
#include "afxdialogex.h"


// CSatelliteCapture �_�C�A���O

IMPLEMENT_DYNAMIC(CSatelliteCapture, CDialogBase)

/*============================================================================*/
/*! CSatelliteCapture

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CSatelliteCapture::CSatelliteCapture(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSatelliteCapture::IDD, pParent)
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
CSatelliteCapture::CSatelliteCapture(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialogBase(IDD, pParent)
{

}

/*============================================================================*/
/*! CSatelliteCapture

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSatelliteCapture::~CSatelliteCapture()
{
}

void CSatelliteCapture::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SATELLITECAPTURE_TAB1, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(CSatelliteCapture, CDialogBase)
	ON_COMMAND(ID_MENU_CLOSE_RC2, &CSatelliteCapture::OnMenuClose)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SATELLITECAPTURE_TAB1, &CSatelliteCapture::OnTcnSelchangeSatellitecaptureTab1)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSatelliteCapture ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CSatelliteCapture

-# �t�@�C�����j���[�̕��邩��Ăяo�����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSatelliteCapture::OnMenuClose()
{
	//	OnOK();
	ShowWindow(SW_HIDE);
}

/*============================================================================*/
/*! CSatelliteCapture

-# �_�C�A���O����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
BOOL CSatelliteCapture::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	TCHAR szMsg[256];
	CRect rect;
	m_tabCtrl.GetClientRect(&rect);
	rect.top += nTabTopOffset;
	rect.left += nTabLeftOffset;

	// �^�u������
//	if (!m_antStanbyOriented.Create(CAntStanbyOriented::IDD, this))
//		return FALSE;
//	m_antStanbyOriented.MoveWindow(&rect, FALSE);

	if (!m_downlinkCapture.Create(CDownlinkCapture::IDD, this))
		return FALSE;
	m_downlinkCapture.MoveWindow(&rect, FALSE);

	if (!m_uplinkCtrl.Create(CUplinkCtrl::IDD, this))
		return FALSE;
	m_uplinkCtrl.MoveWindow(&rect, FALSE);

//	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
//	if (!LoadString(GetModuleHandle(NULL), IDS_SATELLITECAPTURE_ANTSTANBYORIENTED, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
//		return FALSE;
//	m_tabCtrl.InsertItem(0, szMsg);

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SATELLITECAPTURE_DOWNLINKCAPTURE, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	m_tabCtrl.InsertItem(0, szMsg);

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_SATELLITECAPTURE_UPLINKCTRL, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	m_tabCtrl.InsertItem(1, szMsg);

	OnTcnSelchangeSatellitecaptureTab1(0, 0);

	UpdateData(FALSE);

	SetTimer(1, 1000, NULL);

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CSatelliteCapture

-# �^�u�I��ύX�����ɌĂ΂��

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�
*/
/*============================================================================*/
void CSatelliteCapture::OnTcnSelchangeSatellitecaptureTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int sel = m_tabCtrl.GetCurSel();

	switch (sel)
	{
//	case 0:
//		m_antStanbyOriented.ShowWindow(SW_SHOW);
//		m_downlinkCapture.ShowWindow(SW_HIDE);
//		m_uplinkCtrl.ShowWindow(SW_HIDE);
//		break;

	case 0:
//		m_antStanbyOriented.ShowWindow(SW_HIDE);
		m_downlinkCapture.ShowWindow(SW_SHOW);
		m_uplinkCtrl.ShowWindow(SW_HIDE);
		break;

	case 1:
	default:
//		m_antStanbyOriented.ShowWindow(SW_HIDE);
		m_downlinkCapture.ShowWindow(SW_HIDE);
		m_uplinkCtrl.ShowWindow(SW_SHOW);
		break;
	}

	if (pResult)
		*pResult = 0;
}


/*============================================================================*/
/*! CSatelliteCapture

-# Window�v���V�[�W���Ăяo��

@param  message	�F���b�Z�[�W�̎��ʎq
@param  wParam	�F���b�Z�[�W�̍ŏ��̃p�����[�^
@param  lParam	�F���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
@retval ��������
*/
/*============================================================================*/
LRESULT CSatelliteCapture::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
//	if (message == eMessage_UpdateForecastFilePath)
//	{
//		m_antStanbyOriented.PostMessageW(eMessage_UpdateForecastFilePath, wParam, lParam);
//	}

	return CDialogBase::WindowProc(message, wParam, lParam);
}


/*============================================================================*/
/*! CSatelliteCapture

-# �^�C�}�C�x���g�ŌĂяo�����

@param	nIDEvent�F�C�x���gID
@retval	�Ȃ�

*/
/*============================================================================*/
void CSatelliteCapture::OnTimer(UINT_PTR nIDEvent)
{
	m_downlinkCapture.UpdateKanshiData();
	m_uplinkCtrl.UpdateKanshiData();
	UpdateKanshiData();

	CDialogBase::OnTimer(nIDEvent);
}


/*============================================================================*/
/*! CSatelliteCapture

-# �E�B���h�E�j�����ɌĂяo�����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSatelliteCapture::OnDestroy()
{
	CDialogBase::OnDestroy();

	KillTimer(1);
}

/*============================================================================*/
/*! CSatelliteCapture

-# �Ď�����CMD���[�h��RNG���[�h���擾���čX�V����

@param	�Ȃ�
@retval	TRUE:���� / FALSE:���s

*/
/*============================================================================*/
BOOL CSatelliteCapture::UpdateKanshiData()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT sta = theApp.GetSelectStation();

	char		ca_CmdMode[30];		/* CMD���[�h */
	char		ca_RngMode[30];		/* RNG���[�h */

	/* CMD���[�h�擾 */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(CTRL_P_CAP_CMD));
	if (obs_adr == NULL)
	{
		SetCmdMode("");
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		strcpy_s(ca_CmdMode, 30, obs_adr->sz_sts_name);
		SetCmdMode(ca_CmdMode);
	}
	else
	{
		SetCmdMode("");
	}

	/* RNG���[�h�擾 */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(CTRL_P_CAP_RNG));
	if (obs_adr == NULL)
	{
		SetRngMode("");
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		strcpy_s(ca_RngMode, 30, obs_adr->sz_sts_name);
		SetRngMode(ca_RngMode);
	}
	else
	{
		SetRngMode("");
	}

	return TRUE;
}


/*============================================================================*/
/*! CSatelliteCapture

-# CMD���䃂�[�h��ݒ肷��

@param	���䃂�[�h
@retval	�Ȃ�

*/
/*============================================================================*/
void CSatelliteCapture::SetCmdMode(char szCtrlMode[])
{
	eCtrlModeType ctrlMode;

	CString str(szCtrlMode);
	if (str.CompareNoCase(_T("����")) == 0)
	{
		ctrlMode = eCtrlModeType_Auto;
	}
	else if (str.CompareNoCase(_T("�蓮")) == 0)
	{
		ctrlMode = eCtrlModeType_Manual;
	}
	else
	{
		ctrlMode = eCtrlModeType_Null;
	}

	if (m_CmdMode != ctrlMode)
	{
		m_CmdMode = ctrlMode;
	}
}


/*============================================================================*/
/*! CSatelliteCapture

-# RNG���䃂�[�h��ݒ肷��

@param	���䃂�[�h
@retval	�Ȃ�

*/
/*============================================================================*/
void CSatelliteCapture::SetRngMode(char szCtrlMode[])
{
	eCtrlModeType ctrlMode;

	CString str(szCtrlMode);
	if (str.CompareNoCase(_T("����")) == 0)
	{
		ctrlMode = eCtrlModeType_Auto;
	}
	else if (str.CompareNoCase(_T("�蓮")) == 0)
	{
		ctrlMode = eCtrlModeType_Manual;
	}
	else
	{
		ctrlMode = eCtrlModeType_Null;
	}

	if (m_RngMode != ctrlMode)
	{
		m_RngMode = ctrlMode;
	}
}


/*============================================================================*/
/*! CSatelliteCapture

-# CMD����o�C�i�����擾����

@param	�Ȃ�
@retval	���䃂�[�h

*/
/*============================================================================*/
unsigned char CSatelliteCapture::GetCmdBinary()
{
	unsigned char binary = 1;

	switch (m_CmdMode)
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
/*! CSatelliteCapture

-# RNG����o�C�i�����擾����

@param	�Ȃ�
@retval	���䃂�[�h

*/
/*============================================================================*/
unsigned char CSatelliteCapture::GetRngBinary()
{
	unsigned char binary = 1;

	switch (m_RngMode)
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

