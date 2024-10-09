/*============================================================================*/
/*! DownlinkCapture54.cpp

-# �q���ߑ���ʁ@�_�E�������N�ߑ�(54m)
*/
/*============================================================================*/
// DownlinkCapture54.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "DownlinkCapture54.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"
#include "SatelliteCapture54.h"

// CDownlinkCapture54 �_�C�A���O

IMPLEMENT_DYNAMIC(CDownlinkCapture54, CDownlinkCapture)

/*============================================================================*/
// �R���X�g���N�^
/*============================================================================*/
CDownlinkCapture54::CDownlinkCapture54(CWnd* pParent /*=NULL*/)
	: CDownlinkCapture(CDownlinkCapture54::IDD, pParent)
	, m_strBandSelect(_T(""))
	, m_bandSelect(eBandSelectType_Num)
{

}

/*============================================================================*/
// �f�X�g���N�^
/*============================================================================*/
CDownlinkCapture54::~CDownlinkCapture54()
{

}

void CDownlinkCapture54::DoDataExchange(CDataExchange* pDX)
{
	CDownlinkCapture::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DOWNLINKCAPTURE_BANDSELECT_COMBO, m_CCBoxBandSelect);
	DDX_Text(pDX, IDC_DOWNLINKCAPTURE_BANDSELECT_EDIT, m_strBandSelect);
}

BEGIN_MESSAGE_MAP(CDownlinkCapture54, CDownlinkCapture)
	ON_BN_CLICKED(IDC_DOWNLINKCAPTURE_RCVFREQMODE_BUTTON, &CDownlinkCapture::OnBnClickedDownlinkcaptureRcvfreqmodeButton)
	ON_BN_CLICKED(IDC_DOWNLINKCAPTURE_CTRLMODE_BUTTON, &CDownlinkCapture::OnBnClickedDownlinkcaptureCtrlmodeButton)
	ON_BN_CLICKED(IDC_DOWNLINKCAPTURE_BANDSELECT_BUTTON, &CDownlinkCapture54::OnBnClickedDownlinkcaptureBandSelectButton)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CDownlinkCapture54 ���b�Z�[�W �n���h���[

/*============================================================================*/
// �_�C�A���O����������
/*============================================================================*/
BOOL CDownlinkCapture54::OnInitDialog()
{
	CDownlinkCapture::OnInitDialog();

	m_bandSelect = eBandSelectType_Null;

	// �_�C�A���O�A�C�e���𐶐�����
	if (CreateDlgItems() == FALSE)
		return FALSE;

	// �Ď��f�[�^�擾
	(void)UpdateKanshiData();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
// TODO: X��/Ka�ёI���̃{�^���N���b�N���ɌĂ΂��
/*============================================================================*/
void CDownlinkCapture54::OnBnClickedDownlinkcaptureBandSelectButton()
{
	// TODO: �ݒ菈������

#ifdef _DEBUG
	int nIndex = m_CCBoxBandSelect.GetCurSel();
	CString str;
	m_CCBoxBandSelect.GetLBText(nIndex, str);
	if (str.CompareNoCase(_T("X��")) == 0)
	{
		// X��
		m_bandSelect = eBandSelectType_X;
	}
	else if (str.CompareNoCase(_T("Ka��")) == 0)
	{
		// Ka��
		m_bandSelect = eBandSelectType_Ka;
	}
	else
	{
		m_bandSelect = eBandSelectType_Null;
	}
	CreateDlgBandSelect();
#endif
}

/*============================================================================*/
// �_�C�A���O�A�C�e���𐶐�����
/*============================================================================*/
BOOL CDownlinkCapture54::CreateDlgItems()
{
	if(CreateDlgBandSelect())
	{
		return TRUE;
	}

	return FALSE;
}

/*============================================================================*/
// X��/Ka�ёI���_�C�A���O�A�C�e���𐶐�����
/*============================================================================*/
BOOL CDownlinkCapture54::CreateDlgBandSelect()
{
	TCHAR szMsg[256];

	// �R���{�{�b�N�X�̃N���A
	m_CCBoxBandSelect.ResetContent();

	// �G�f�B�b�g�R���g���[��������
	// �R���{�{�b�N�X������
	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_DOWNLINK_BANDSELECT_X, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	if (m_bandSelect == eBandSelectType_X)
	{
		m_strBandSelect = CString(szMsg);
	}
	else
	{
		m_CCBoxBandSelect.InsertString(-1, szMsg);
	}

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_DOWNLINK_BANDSELECT_KA, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	if (m_bandSelect == eBandSelectType_Ka)
	{
		m_strBandSelect = CString(szMsg);
	}
	else
	{
		m_CCBoxBandSelect.InsertString(-1, szMsg);
	}

	// �Ď��𖢎�M��
	if (m_bandSelect == eBandSelectType_Null)
	{
		m_strBandSelect = CString(_T(""));
	}

	m_CCBoxBandSelect.SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;
}

/*============================================================================*/
// �Ď�����e���ڂ̒l���擾���čX�V����
/*============================================================================*/
BOOL CDownlinkCapture54::UpdateKanshiData()
{
	// ��M���g�����[�h�Ɛ��䃂�[�h�̍X�V
	CDownlinkCapture::UpdateKanshiData();

	// TODO: X��<=>Ka�ѐ؂�ւ��̍X�V

	return TRUE;
}

/*============================================================================*/
// ��M���g�����[�h�̃R���g���[���̕\���A��\��
/*============================================================================*/
void CDownlinkCapture54::ShowRcvFreqMode()
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

