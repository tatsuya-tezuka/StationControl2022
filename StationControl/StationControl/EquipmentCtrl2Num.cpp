/*============================================================================*/
/*! EquipmetCtrl2Num.cpp

-# �ݔ�����-���l������(�ݔ�����2�̏ꍇ�̓�������)
*/
/*============================================================================*/
// EquipmentCtrl2Num.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "EquipmentCtrl2Num.h"
#include "afxdialogex.h"


// CEquipmentCtrl2Num �_�C�A���O

IMPLEMENT_DYNAMIC(CEquipmentCtrl2Num, CDialogEx)

CEquipmentCtrl2Num::CEquipmentCtrl2Num(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEquipmentCtrl2Num::IDD, pParent)
	, m_strEquipment1(_T(""))
	, m_strVal1(_T(""))
	, m_strRange1(_T(""))
	, m_strEquipment2(_T(""))
	, m_strVal2(_T(""))
	, m_strRange2(_T(""))
	, m_strWindowTitle(_T(""))
{

}

CEquipmentCtrl2Num::~CEquipmentCtrl2Num()
{
}

void CEquipmentCtrl2Num::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EQUIPCTRL_NUM_EQIPT_STATIC1, m_strEquipment1);
	DDX_Text(pDX, IDC_EQUIPCTRL_NUM_EQIPVAL_EDIT1, m_strVal1);
	DDX_Text(pDX, IDC_EQUIPCTRL_NUM_RNG_STATIC1, m_strRange1);
	DDX_Control(pDX, IDC_EQUIPCTRL_NUM_EQIPVAL_EDIT1, m_CtrlValueEdit1);
	DDX_Text(pDX, IDC_EQUIPCTRL_NUM_EQIPT_STATIC2, m_strEquipment2);
	DDX_Text(pDX, IDC_EQUIPCTRL_NUM_EQIPVAL_EDIT2, m_strVal2);
	DDX_Text(pDX, IDC_EQUIPCTRL_NUM_RNG_STATIC2, m_strRange2);
	DDX_Control(pDX, IDC_EQUIPCTRL_NUM_EQIPVAL_EDIT2, m_CtrlValueEdit2);
}


BEGIN_MESSAGE_MAP(CEquipmentCtrl2Num, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEquipmentCtrl2Num::OnBnClickedOk)
END_MESSAGE_MAP()


// CEquipmentCtrl2Num ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! CEquipmentCtrl2Num

-# �_�C�A���O����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CEquipmentCtrl2Num::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (!m_strWindowTitle.IsEmpty())
		SetWindowText(m_strWindowTitle);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! CEquipmentCtrl2Num

-# �o�^����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentCtrl2Num::OnBnClickedOk()
{
	double val;

	UpdateData();

	CString str;
	// �p�����[�^�`�F�b�N
	for (UINT i = 0; i < m_nCtrlNumMax; i++)
	{
		if (i == 0)
		{
			str = m_strVal1;
		}
		else
		{
			str = m_strVal2;
		}
		val = _wtof(str);

		if (val < m_stEquipmentCtrl[i].dRangeMin || val > m_stEquipmentCtrl[i].dRangeMax)
		{
			CString strMsg;
			strMsg.Format(_T("%s�ɐݒ肳�ꂽ�l���A���͔͈͊O�ł��B"), (LPCTSTR)m_stEquipmentCtrl[i].strEquipment);
			AfxMessageBox((LPCTSTR)strMsg, MB_OK | MB_ICONEXCLAMATION);
			return;
		}
	}

	m_stEquipmentCtrl[0].dRangeCur = m_strVal1;
	m_stEquipmentCtrl[1].dRangeCur = m_strVal2;

	CDialogEx::OnOK();
}
