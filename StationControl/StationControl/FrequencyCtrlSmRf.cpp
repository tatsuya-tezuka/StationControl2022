/*============================================================================*/
/*! FrequencyCtrlSmFf.cpp

-# ���g��������-�V�~�����[�V����-�n��ǎ�M���g���ύX�^�u�i���g�p�j
*/
/*============================================================================*/
// FrequencyCtrlSmFf.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlSmRf.h"
#include "afxdialogex.h"


// CFrequencyCtrlSmRf �_�C�A���O

IMPLEMENT_DYNAMIC(CFrequencyCtrlSmRf, CDialogBase)

/*============================================================================*/
/*! CFrequencyCtrlSmRf

-# �R���X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlSmRf::CFrequencyCtrlSmRf(CWnd* pParent /*=NULL*/)
: CDialogBase(CFrequencyCtrlSmRf::IDD, pParent)
	, m_strRFreq(_T(""))
	, m_SpecDay(0)
	, m_SpecTime(0)
{

}

/*============================================================================*/
/*! CFrequencyCtrlSmRf

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlSmRf::~CFrequencyCtrlSmRf()
{
}

void CFrequencyCtrlSmRf::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_RF_TF1_EDIT, m_strRFreq);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_RF_SPT_DAY_DP, m_SpecDay);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_RF_SPT_TIME_DP, m_SpecTime);
	DDX_Control(pDX, IDC_FREQ_CTRL_SML_RF_SPT_DAY_DP, m_SpecDayDTCtrl);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrlSmRf, CDialogBase)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_RF_DPCALC_BTN, &CFrequencyCtrlSmRf::OnBnClickedFreqCtrlSmlRfDpcalcBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_RF_DPVIEW_BTN, &CFrequencyCtrlSmRf::OnBnClickedFreqCtrlSmlRfDpviewBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_RF_CALC_BTN, &CFrequencyCtrlSmRf::OnBnClickedFreqCtrlSmlRfCalcBtn)
END_MESSAGE_MAP()


// CFrequencyCtrlSmRf ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! CFrequencyCtrlSmRf

-# �h�b�v���⏞�l�v�Z�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlSmRf::OnBnClickedFreqCtrlSmlRfDpcalcBtn()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}

/*============================================================================*/
/*! CFrequencyCtrlSmRf

-# �h�b�v���⏞�l�\���{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlSmRf::OnBnClickedFreqCtrlSmlRfDpviewBtn()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}

/*============================================================================*/
/*! CFrequencyCtrlSmRf

-# �v�Z�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlSmRf::OnBnClickedFreqCtrlSmlRfCalcBtn()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}


/*============================================================================*/
/*! CFrequencyCtrlSmRf

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CFrequencyCtrlSmRf::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// �����t�H�[�}�b�g�ύX
	m_SpecDayDTCtrl.SetFormat(_T("yyyy-MM-dd"));

	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	CTime time = CTime(timeSys);

	// �w�莞��
	m_SpecDay = time;
	m_SpecTime = time;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! CFrequencyCtrlSmRf

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CFrequencyCtrlSmRf::UpdateDlg()
{
	return TRUE;
}

