/*============================================================================*/
/*! EquipmentCtrlSameNum.cpp

-# �ݔ�����-���l������(�Q�̐���ɓ��l��ݒ�j
*/
/*============================================================================*/
// EquipmentCtrlSameNum.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "EquipmentCtrlSameNum.h"


// CEquipmentCtrlSameNum �_�C�A���O

IMPLEMENT_DYNAMIC(CEquipmentCtrlSameNum, CEquipmentCtrlNum)

CEquipmentCtrlSameNum::CEquipmentCtrlSameNum(CWnd* pParent /*=NULL*/)
: CEquipmentCtrlNum(CEquipmentCtrlSameNum::IDD, pParent)
, m_strEquipment2(_T(""))
{

}

CEquipmentCtrlSameNum::~CEquipmentCtrlSameNum()
{
}

void CEquipmentCtrlSameNum::DoDataExchange(CDataExchange* pDX)
{
	CEquipmentCtrlNum::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EQUIPCTRL_NUM_EQIPT_STATICCTRL2, m_strEquipment2);
}


BEGIN_MESSAGE_MAP(CEquipmentCtrlSameNum, CEquipmentCtrlNum)
	ON_BN_CLICKED(IDOK, &CEquipmentCtrlNum::OnBnClickedOk)
END_MESSAGE_MAP()


// CEquipmentCtrlSameNum ���b�Z�[�W �n���h���[


BOOL CEquipmentCtrlSameNum::OnInitDialog()
{
	CEquipmentCtrlNum::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
