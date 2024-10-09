/*============================================================================*/
/*! EquipmetCtrlNum.cpp

-# �ݔ�����-���l������
*/
/*============================================================================*/
// EquipmetCtrlNum.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "EquipmentCtrlNum.h"
#include "afxdialogex.h"


// CEquipmetCtrlNum �_�C�A���O

IMPLEMENT_DYNAMIC(CEquipmentCtrlNum, CDialogEx)

CEquipmentCtrlNum::CEquipmentCtrlNum(CWnd* pParent /*=NULL*/)
: CDialogEx(CEquipmentCtrlNum::IDD, pParent)
, m_strEquipment(_T(""))
, m_strVal(_T(""))
, m_strRange(_T(""))
, m_strWindowTitle(_T(""))
{

}

CEquipmentCtrlNum::CEquipmentCtrlNum(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD, pParent)
	, m_strEquipment(_T(""))
	, m_strVal(_T(""))
	, m_strRange(_T(""))
	, m_strWindowTitle(_T(""))
{

}

CEquipmentCtrlNum::~CEquipmentCtrlNum()
{
}

void CEquipmentCtrlNum::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EQUIPCTRL_NUM_EQIPT_STATIC, m_strEquipment);
	DDX_Text(pDX, IDC_EQUIPCTRL_NUM_EQIPVAL_EDIT, m_strVal);
	DDX_Text(pDX, IDC_EQUIPCTRL_NUM_RNG_STATIC, m_strRange);
	DDX_Control(pDX, IDC_EQUIPCTRL_NUM_EQIPVAL_EDIT, m_CtrlValueEdit);
}

BEGIN_MESSAGE_MAP(CEquipmentCtrlNum, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEquipmentCtrlNum::OnBnClickedOk)
END_MESSAGE_MAP()


// CEquipmetCtrlNum ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CEquipmentCtrlNum

-# �_�C�A���O����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CEquipmentCtrlNum::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (!m_strWindowTitle.IsEmpty())
		SetWindowText(m_strWindowTitle);

	m_strVal = m_RangeCur;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! CEquipmentCtrlNum

-# �o�^����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentCtrlNum::OnBnClickedOk()
{
	double val = 0;

	UpdateData();

	val = _wtof(m_strVal);
	if (val < m_RangeMin || val > m_RangeMax)
	{
		AfxMessageBox(_T("�ݒ肳�ꂽ�l���A���͔͈͊O�ł��B"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	m_RangeCur = m_strVal;

	CDialogEx::OnOK();
}


