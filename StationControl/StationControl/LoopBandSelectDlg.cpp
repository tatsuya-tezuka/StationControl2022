/*============================================================================*/
/*! LoopBandSelectDlg.cpp

-# ���[�v�o���h�I�����
*/
/*============================================================================*/
// LoopBandSelectDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "LoopBandSelectDlg.h"
#include "afxdialogex.h"


// CLoopBandSelectDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CLoopBandSelectDlg, CDialogEx)

CLoopBandSelectDlg::CLoopBandSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoopBandSelectDlg::IDD, pParent)
{

}

CLoopBandSelectDlg::~CLoopBandSelectDlg()
{
}

void CLoopBandSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOOPBANDCMB, m_LBCmbCtrl);
}


BEGIN_MESSAGE_MAP(CLoopBandSelectDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoopBandSelectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoopBandSelectDlg ���b�Z�[�W �n���h���[



BOOL CLoopBandSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (int i = 0; i < m_LBCmbList.size(); ++i)
	{
		m_LBCmbCtrl.InsertString(-1, m_LBCmbList[i]);
	}

	int idx = -1;
	if ((idx = m_LBCmbCtrl.FindStringExact(-1, m_strLoopBand)) != CB_ERR)
	{
		m_LBCmbCtrl.SetCurSel(idx);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void CLoopBandSelectDlg::OnBnClickedOk()
{
	CString str;
	m_LBCmbCtrl.GetLBText(m_LBCmbCtrl.GetCurSel(), str);
	m_strLoopBand = str;

	CDialogEx::OnOK();
}

