/*============================================================================*/
/*! FrequencyCtrlPre.cpp

-# ���g��������-�ꊇ�ݒ�^�u
*/
/*============================================================================*/
// FrequencyCtrlPre.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlPre.h"
#include "afxdialogex.h"
#include "FrequencyCtrl.h"


// CFrequencyCtrlPre �_�C�A���O

IMPLEMENT_DYNAMIC(CFrequencyCtrlPre, CDialogBase)

/*============================================================================*/
/*! CFrequencyCtrlPre

-# �R���X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlPre::CFrequencyCtrlPre(CWnd* pParent /*=NULL*/)
: CDialogBase(CFrequencyCtrlPre::IDD, pParent)
, m_strSendOp(_T(""))
, m_nSendGet(0)
{

}

/*============================================================================*/
/*! CFrequencyCtrlPre

-# �R���X�g���N�^

@param	IDD�FIDD
@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlPre::CFrequencyCtrlPre(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialogBase(IDD, pParent)
{

}

/*============================================================================*/
/*! CFrequencyCtrlPre

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlPre::~CFrequencyCtrlPre()
{
}

void CFrequencyCtrlPre::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SENDOP, m_strSendOp);
	DDX_Control(pDX, IDC_COMBO_SENDGET, m_cbSendGet);
	DDX_CBIndex(pDX, IDC_COMBO_SENDGET, m_nSendGet);
	DDX_Control(pDX, IDC_EDIT_SENDOP, m_SendOpEdit);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrlPre, CDialogBase)
	ON_BN_CLICKED(IDC_BUTTON_PRE_1WAY, &CFrequencyCtrlPre::OnBnClickedButtonPre1way)
	ON_BN_CLICKED(IDC_BUTTON_PRE_2WAY1, &CFrequencyCtrlPre::OnBnClickedButtonPre2way1)
	ON_BN_CLICKED(IDC_BUTTON_PRE_2WAY2, &CFrequencyCtrlPre::OnBnClickedButtonPre2way2)
	ON_BN_CLICKED(IDC_BUTTON_PRE_2WAY3, &CFrequencyCtrlPre::OnBnClickedButtonPre2way3)
	ON_BN_CLICKED(IDC_BUTTON_PRE_2WAY4, &CFrequencyCtrlPre::OnBnClickedButtonPre2way4)
	ON_BN_CLICKED(IDC_BUTTON_PRE_3WAY, &CFrequencyCtrlPre::OnBnClickedButtonPre3way)
	ON_BN_CLICKED(IDC_BUTTON_GET, &CFrequencyCtrlPre::OnBnClickedButtonGet)
END_MESSAGE_MAP()


// CFrequencyCtrlPre ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CFrequencyCtrlPre

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CFrequencyCtrlPre::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// ���M���g���擾��
	for (int i = 0; i < sizeof(mSenderFreq[theApp.GetSelectStation()]) / sizeof(mSenderFreq[theApp.GetSelectStation()][0]); i++){
		int pos = m_cbSendGet.AddString(mSenderFreq[theApp.GetSelectStation()][i].sendname);
		m_cbSendGet.SetItemData(pos, mSenderFreq[theApp.GetSelectStation()][i].sendid);
	}
	m_cbSendGet.SetCurSel(0);
	m_SendOpEdit.SetRoundPlaceValue(1);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! CFrequencyCtrlPre

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CFrequencyCtrlPre::UpdateDlg()
{
	return TRUE;
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# �u1WAY�^�p�i�e�����P�Ɖ^�p�j�v�{�^������������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlPre::OnBnClickedButtonPre1way()
{
	SetData(ePre_1WAY);
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# �u2WAY�^�p�iCOH,���I�j�v�{�^������������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlPre::OnBnClickedButtonPre2way1()
{
	SetData(ePre_2WAY1);
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# �u2WAY�^�p�iCOH,�Z���^�[�j�v�{�^������������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlPre::OnBnClickedButtonPre2way2()
{
	SetData(ePre_2WAY2);
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# �u2WAY�^�p�iINCOH,���I�j�v�{�^������������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlPre::OnBnClickedButtonPre2way3()
{
	SetData(ePre_2WAY3);
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# �u2WAY�^�p�iINCOH,�Z���^�[�j�v�{�^������������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlPre::OnBnClickedButtonPre2way4()
{
	SetData(ePre_2WAY4);
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# �u3WAY�v�{�^������������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlPre::OnBnClickedButtonPre3way()
{
	SetData(ePre_3WAY);
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# �u�擾�v�{�^������������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlPre::OnBnClickedButtonGet()
{
	UpdateData(TRUE);
	// ����Ǒ��M���g�����擾����
	int pos = m_cbSendGet.GetCurSel();
	UINT target = (UINT)m_cbSendGet.GetItemData(pos);
	CSatelliteData& sd = theApp.GetSatelliteData();
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(target, _T("X-UC.FREQUENCY"));
	m_strSendOp = _T("0.0");
	if (obs_adr != NULL){
		m_strSendOp.Format(_T("%0.1f"), obs_adr->d_data);
	}
	UpdateData(FALSE);
}

/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# ��ʍX�V

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlPre::SetData(int type)
{
	CFrequencyCtrlPre::stSetCtlData mPreTable[] = {
		{ _T("CTRL.RX_OPE 1WAY") },
		{ _T("CTRL.RX_OPE 2WAY_COH_DYN") },
		{ _T("CTRL.RX_OPE 2WAY_COH_AVE") },
		{ _T("CTRL.RX_OPE 2WAY_INCOH_DYN") },
		{ _T("CTRL.RX_OPE 2WAY_INCOH_AVE") },
		{ _T("CTRL.RX_OPE 3WAY@%s") },
	};

	UpdateData(TRUE);

	/****************************************************************/
	/* ������ */
	CString ctrl;
	ctrl = mPreTable[type].ctlname;
	if (type == ePre_3WAY)
	{
		if (m_strSendOp.IsEmpty() == true)
		{
			MessageBox(_T("����Ǒ��M���g������ł��B"), 0, MB_OK | MB_ICONERROR);
			return;
		}
		if (m_strSendOp == _T("+") || m_strSendOp == _T("-"))
		{
			MessageBox(_T("���l����͂��Ă��������B"), 0, MB_OK | MB_ICONERROR);
			return;
		}

		double val = _wtof(m_strSendOp);
		if ((val < 1000000000.0) || (val >= 100000000000.0))
		{
			MessageBox(_T("����Ǒ��M���g����1000000000.0�`99999999999.9[Hz]����͂��Ă��������B"), 0, MB_OK | MB_ICONERROR);
			return;
		}

		CString tmp = ctrl;
		ctrl.Format(tmp, m_strSendOp);
	}
	theApp.SendControl(ctrl, _T(""), _T(""), _T(""));
}
