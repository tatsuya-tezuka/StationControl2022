/*============================================================================*/
/*! FrequencyCtrlSmCf.cpp

-# ���g��������-�V�~�����[�V����-�R�}���h�T�u�L�����A���g���ύX�^�u�i���g�p�j
*/
/*============================================================================*/
// FrequencyCtrlSmCf.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlSmCf.h"
#include "afxdialogex.h"


// CFrequencyCtrlSmCf �_�C�A���O

IMPLEMENT_DYNAMIC(CFrequencyCtrlSmCf, CDialogBase)

/*============================================================================*/
/*! CFrequencyCtrlSmCf

-# �R���X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlSmCf::CFrequencyCtrlSmCf(CWnd* pParent /*=NULL*/)
: CDialogBase(CFrequencyCtrlSmCf::IDD, pParent)
	, m_strCmdFreq(_T(""))
	, m_SpecDay(0)
	, m_SpecTime(0)
{

}

/*============================================================================*/
/*! CFrequencyCtrlSmCf

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlSmCf::~CFrequencyCtrlSmCf()
{
}

void CFrequencyCtrlSmCf::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_CF_TF1_EDIT, m_strCmdFreq);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_CF_SPT_DAY_DP, m_SpecDay);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_CF_SPT_TIME_DP, m_SpecTime);
	DDX_Control(pDX, IDC_FREQ_CTRL_SML_CF_SPT_DAY_DP, m_SpecDayDTCtrl);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrlSmCf, CDialogBase)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_CF_DPCALC_BTN, &CFrequencyCtrlSmCf::OnBnClickedFreqCtrlSmlCfDpcalcBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_CF_DPVIEW_BTN, &CFrequencyCtrlSmCf::OnBnClickedFreqCtrlSmlCfDpviewBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_CF_CALC_BTN, &CFrequencyCtrlSmCf::OnBnClickedFreqCtrlSmlCfCalcBtn)
END_MESSAGE_MAP()


// CFrequencyCtrlSmCf ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CFrequencyCtrlSmCf

-# �h�b�v���⏞�l�v�Z�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlSmCf::OnBnClickedFreqCtrlSmlCfDpcalcBtn()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}


/*============================================================================*/
/*! CFrequencyCtrlSmCf

-# �h�b�v���⏞�l�\���{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlSmCf::OnBnClickedFreqCtrlSmlCfDpviewBtn()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}


/*============================================================================*/
/*! CFrequencyCtrlSmCf

-# �v�Z�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlSmCf::OnBnClickedFreqCtrlSmlCfCalcBtn()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}


/*============================================================================*/
/*! CFrequencyCtrlSmCf

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CFrequencyCtrlSmCf::OnInitDialog()
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
/*! CFrequencyCtrlSmCf

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CFrequencyCtrlSmCf::UpdateDlg()
{
	return TRUE;
}

