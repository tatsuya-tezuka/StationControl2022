/*============================================================================*/
/*! TrsControlDetail.cpp

-# ���M����-�ݔ��ڍא�����
*/
/*============================================================================*/
// TrsControlDetail.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrsControlDetail.h"
#include "afxdialogex.h"


// CTrsControlDetail �_�C�A���O

IMPLEMENT_DYNAMIC(CTrsControlDetail, CDialogEx)

CTrsControlDetail::CTrsControlDetail(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTrsControlDetail::IDD, pParent)
	, m_strCtrlName(_T(""))
	, m_CtrlValue(0)
{

}

CTrsControlDetail::~CTrsControlDetail()
{
}

void CTrsControlDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TRSCTRL_D_CTRL_NAME_E_STATIC, m_strCtrlName);
	DDX_Text(pDX, IDC_EDIT1, m_CtrlValue);
}


BEGIN_MESSAGE_MAP(CTrsControlDetail, CDialogEx)
END_MESSAGE_MAP()


// CTrsControlDetail ���b�Z�[�W �n���h���[
