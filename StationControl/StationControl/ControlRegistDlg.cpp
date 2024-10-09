/*============================================================================*/
/*! CControlRegistDlg.cpp

-# ��������o�^�E���s		����o�^
*/
/*============================================================================*/
// ControlRegistDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "ControlRegistDlg.h"
#include "afxdialogex.h"
#include "CtrlSearchDlg.h"

// CControlRegistDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CControlRegistDlg, CDialogBase)

/*============================================================================*/
/*! CControlRegistDlg

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CControlRegistDlg::CControlRegistDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CControlRegistDlg::IDD, pParent)
	, m_strCtrl(_T(""))
	, m_strVal(_T(""))
	, m_strValDisp(_T(""))
{

}

/*============================================================================*/
/*! CControlRegistDlg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CControlRegistDlg::~CControlRegistDlg()
{
}

void CControlRegistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CTRLREGIST_CTRLNAME, m_strCtrl);
	DDX_Text(pDX, IDC_STATIC_CTRLREGIST_CTRLVAL, m_strValDisp);
	DDX_Control(pDX, IDC_EDIT_CTRLREGIST_CTRLNAME, m_cedit);
}


BEGIN_MESSAGE_MAP(CControlRegistDlg, CDialogBase)
	ON_BN_CLICKED(IDC_BUTTON_CTRLREGIST_SEARCH, &CControlRegistDlg::OnBnClickedButtonCtrlregistSearch)
	ON_BN_CLICKED(IDOK, &CControlRegistDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CControlRegistDlg ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CControlRegistDlg

-# ���䖼����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CControlRegistDlg::OnBnClickedButtonCtrlregistSearch()
{
	UpdateData();
	CCtrlSearchDlg dlg;
	dlg.SetCtrlName(m_strCtrl);
	dlg.SetValName(m_strVal);
	if (dlg.DoModal() == IDOK)
	{
		m_strCtrl = dlg.GetCtrlNameSearch();
		m_strVal = dlg.GetCtrlValeSearch();
		if (m_strVal.IsEmpty())
		{
			m_strValDisp = _T("-");
		}
		else
		{
			m_strValDisp = m_strVal;
		}
		UpdateData(FALSE);
	}
}


/*============================================================================*/
/*! CControlRegistDlg

-# ����o�^�����s����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CControlRegistDlg::OnBnClickedOk()
{
	UpdateData();

	if (m_strCtrl.IsEmpty())
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("���䖼����͂��Ă�������"), title, MB_OK);
		return;
	}

//	if (m_strVal.IsEmpty())
//	{
//		CString title = _T("");
//		GetWindowText(title);
//		MessageBox(_T("�X�e�[�^�X��I�����Ă�������"), title, MB_OK);
//		return;
//	}

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! CControlRegistDlg

-# �ݒ蕶������擾����

@param  �Ȃ�
@retval �ݒ蕶����
*/
/*============================================================================*/
CString CControlRegistDlg::GetString()
{
	return m_strCtrl + _T(",") + m_strVal;
}


/*============================================================================*/
/*! CControlRegistDlg

-# ���������ɌĂ΂��

@param  �Ȃ�
@retval TRUE:�����@/ FALSE:���s
*/
/*============================================================================*/
BOOL CControlRegistDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	if (m_strVal.IsEmpty())
	{
		m_strValDisp = _T("-");
	}
	else
	{
		m_strValDisp = m_strVal;
	}

	UpdateData(FALSE);

	m_cedit.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
