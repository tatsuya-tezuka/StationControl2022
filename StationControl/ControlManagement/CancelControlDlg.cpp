/*============================================================================*/
/*! CancelControlDlg.cpp

-# �}�g�ǉ^�p�ǐ��[�����䌠������
*/
/*============================================================================*/
// CancelControlDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "ControlManagement.h"
#include "CancelControlDlg.h"
#include "afxdialogex.h"


// CCancelControlDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CCancelControlDlg, CDialogEx)

CCancelControlDlg::CCancelControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCancelControlDlg::IDD, pParent)
	, m_nType(0)
{

}

CCancelControlDlg::~CCancelControlDlg()
{
}

void CCancelControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
}


BEGIN_MESSAGE_MAP(CCancelControlDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCancelControlDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCancelControlDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CCancelControlDlg ���b�Z�[�W �n���h���[


BOOL CCancelControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! �}�g�ǉ^�p�ǐ��[�����䌠������

-# ���䌠���������s��

@param
@retval

*/
/*============================================================================*/
void CCancelControlDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	CFileAccess& ac = theApp.GetFileAccess();
	stLocalControl	tdata;
	CString strDefaultPass = mDefaultPassword;
	strDefaultPass = theApp.ManEncodeDecode(strDefaultPass);
	CStringA strPass = CStringA(strDefaultPass);

	if (ac.ReadLocalControl(tdata, eLOCALusc20 + m_nType) == true){
		tdata.flag = 0;
		ac.WriteLocalControl(tdata, eLOCALusc20 + m_nType);
	}
	else{
		// �f�t�H���g�ݒ�
		tdata.flag = 0;
		sprintf_s(tdata.lifetime, mMaxLifetime, "0");
		ac.WriteLocalControl(tdata, eLOCALusc20 + m_nType);
	}

	CDialogEx::OnOK();
}


void CCancelControlDlg::OnBnClickedCancel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDialogEx::OnCancel();
}
