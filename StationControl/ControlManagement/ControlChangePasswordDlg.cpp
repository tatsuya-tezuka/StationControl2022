/*============================================================================*/
/*! ControlChangePasswordDlg.cpp

-# ���䌠���擾�p�p�X���[�h�ύX
*/
/*============================================================================*/
// ControlChangePasswordDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "ControlManagement.h"
#include "ControlChangePasswordDlg.h"
#include "afxdialogex.h"


// CControlChangePasswordDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CControlChangePasswordDlg, CDialogEx)

CControlChangePasswordDlg::CControlChangePasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlChangePasswordDlg::IDD, pParent)
	, m_nType(0)
	, m_strOldPass(_T(""))
	, m_strNewPass(_T(""))
	, m_strTarget(_T(""))
{

}

CControlChangePasswordDlg::~CControlChangePasswordDlg()
{
}

void CControlChangePasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
	DDX_Text(pDX, IDC_EDIT_OLDPASSWORD, m_strOldPass);
	DDX_Text(pDX, IDC_EDIT_NEWPASSWORD, m_strNewPass);
	DDX_Text(pDX, IDC_STATIC_TARGET, m_strTarget);
}


BEGIN_MESSAGE_MAP(CControlChangePasswordDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CControlChangePasswordDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CControlChangePasswordDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO1, &CControlChangePasswordDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CControlChangePasswordDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CControlChangePasswordDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CControlChangePasswordDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CControlChangePasswordDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &CControlChangePasswordDlg::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, &CControlChangePasswordDlg::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &CControlChangePasswordDlg::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO9, &CControlChangePasswordDlg::OnBnClickedRadio9)
	ON_BN_CLICKED(IDC_RADIO10, &CControlChangePasswordDlg::OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_RADIO11, &CControlChangePasswordDlg::OnBnClickedRadio11)
	ON_BN_CLICKED(IDC_RADIO12, &CControlChangePasswordDlg::OnBnClickedRadio12)
END_MESSAGE_MAP()


// CControlChangePasswordDlg ���b�Z�[�W �n���h���[


BOOL CControlChangePasswordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	OnBnClickedRadio1();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CControlChangePasswordDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	bool ret = true;
	switch (m_nType){
	case	0:
		ret = SetPassword(eSOSusc20, m_strNewPass);
		break;
	case	1:
		ret = SetPassword(eSOSusc34, m_strNewPass);
		break;
	case	2:
		ret = SetPassword(eSOSudsc64, m_strNewPass);
		break;
	case	3:
		ret = SetPassword(eSOSxdsc54, m_strNewPass);
		break;
	case	4:
		ret = SetPassword(eSSOCusc20, m_strNewPass);
		break;
	case	5:
		ret = SetPassword(eSSOCusc34, m_strNewPass);
		break;
	case	6:
		ret = SetPassword(eSSOCudsc64, m_strNewPass);
		break;
	case	7:
		ret = SetPassword(eSSOCxdsc54, m_strNewPass);
		break;
	case	8:
		ret = SetPassword(eTKSCusc20, m_strNewPass);
		break;
	case	9:
		ret = SetPassword(eTKSCusc34, m_strNewPass);
		break;
	case	10:
		ret = SetPassword(eTKSCudsc64, m_strNewPass);
		break;
	case	11:
		ret = SetPassword(eTKSCxdsc54, m_strNewPass);
		break;
	}
	if (ret == false)
		return;

	CDialogEx::OnOK();
}

void CControlChangePasswordDlg::OnBnClickedCancel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDialogEx::OnCancel();
}

void CControlChangePasswordDlg::GetPassword(int type, CString& oldPass)
{
	UpdateData(TRUE);

	CFileAccess& ac = theApp.GetFileAccess();
	CString strDefaultPass = mDefaultPassword;
	//strDefaultPass = theApp.ManEncodeDecode(strDefaultPass);
	stControl	data;
	memset(&data, 0, sizeof(stControl));
	if (type >= eSOSusc20 && type <= eSOSxdsc54){
		if (ac.ReadSOSControl(data, type) == true){
			char buf[mMaxPassword + 1];
			memset(buf, 0, mMaxPassword + 1);
			memcpy(buf, data.password, mMaxPassword);
			strDefaultPass = CString(buf);
			strDefaultPass = theApp.ManEncodeDecode(strDefaultPass);
		}
	}
	if (type >= eSSOCusc20 && type <= eSSOCxdsc54){
		if (ac.ReadSSOCControl(data, type) == true){
			char buf[mMaxPassword + 1];
			memset(buf, 0, mMaxPassword + 1);
			memcpy(buf, data.password, mMaxPassword);
			strDefaultPass = CString(buf);
			strDefaultPass = theApp.ManEncodeDecode(strDefaultPass);
		}
	}
	if (type >= eTKSCusc20 && type <= eTKSCxdsc54){
		if (ac.ReadTKSCControl(data, type) == true){
			char buf[mMaxPassword + 1];
			memset(buf, 0, mMaxPassword + 1);
			memcpy(buf, data.password, mMaxPassword);
			strDefaultPass = CString(buf);
			strDefaultPass = theApp.ManEncodeDecode(strDefaultPass);
		}
	}
	oldPass = strDefaultPass;
}

bool CControlChangePasswordDlg::SetPassword(int type, CString newPass)
{
	if (newPass.GetLength() != mMaxPassword){
		MessageBox(_T("���͂��ꂽ�p�X���[�h��8�����ł͂���܂���B\n�ēx���͂��Ă��������B"), MB_OK);
		return false;
	}
	if (newPass.IsEmpty() == true){
		MessageBox(_T("���͂��ꂽ�p�X���[�h�Ɍ�肪����܂��B\n�ēx���͂��Ă��������B"), MB_OK);
		return false;
	}

	CFileAccess& ac = theApp.GetFileAccess();
	CString strDefaultPass = newPass;
	strDefaultPass = theApp.ManEncodeDecode(strDefaultPass);
	CStringA strPass = CStringA(strDefaultPass);
	stControl	data;
	memset(&data, 0, sizeof(stControl));
	if (type >= eSOSusc20 && type <= eSOSxdsc54){
		if (ac.ReadSOSControl(data, type) == true){
			memcpy(data.password, (char*)(const char*)CStringA(strPass), mMaxPassword);
			ac.WriteSOSControl(data, type);
			return true;
		}
		else{
			data.flag = 0;
			memcpy(data.password, (char*)(const char*)CStringA(strPass), mMaxPassword);
			sprintf_s(data.lifetime, mMaxLifetime, "0");
			ac.WriteSOSControl(data, type);
			return true;
		}
	}
	if (type >= eSSOCusc20 && type <= eSSOCxdsc54){
		if (ac.ReadSSOCControl(data, type) == true){
			memcpy(data.password, (char*)(const char*)CStringA(strPass), mMaxPassword);
			ac.WriteSSOCControl(data, type);
			return true;
		}
		else{
			data.flag = 0;
			memcpy(data.password, (char*)(const char*)CStringA(strPass), mMaxPassword);
			sprintf_s(data.lifetime, mMaxLifetime, "0");
			ac.WriteSSOCControl(data, type);
			return true;
		}
	}
	if (type >= eTKSCusc20 && type <= eTKSCxdsc54){
		if (ac.ReadTKSCControl(data, type) == true){
			memcpy(data.password, (char*)(const char*)CStringA(strPass), mMaxPassword);
			ac.WriteTKSCControl(data, type);
			return true;
		}
		else{
			data.flag = 1;
			memcpy(data.password, (char*)(const char*)CStringA(strPass), mMaxPassword);
			sprintf_s(data.lifetime, mMaxLifetime, "%I64d", INT64_MAX);
			ac.WriteTKSCControl(data, type);
			return true;
		}
	}
	MessageBox(_T("�t�@�C�������݂��Ȃ���������܂���B\n�m�F���Ă��������B"), MB_OK);
	return false;
}

void CControlChangePasswordDlg::OnBnClickedRadio1()
{
	GetPassword(eSOSusc20, m_strOldPass);
	m_strNewPass.Empty();
	m_strTarget = _T("�n��ǁF���V�Y20m��");
	UpdateData(FALSE);
}
void CControlChangePasswordDlg::OnBnClickedRadio2()
{
	GetPassword(eSOSusc34, m_strOldPass);
	m_strNewPass.Empty();
	m_strTarget = _T("�n��ǁF���V�Y34m��");
	UpdateData(FALSE);
}
void CControlChangePasswordDlg::OnBnClickedRadio3()
{
	GetPassword(eSOSudsc64, m_strOldPass);
	m_strNewPass.Empty();
	m_strTarget = _T("�n��ǁF�P�c64m��");
	UpdateData(FALSE);
}
void CControlChangePasswordDlg::OnBnClickedRadio4()
{
	GetPassword(eSOSxdsc54, m_strOldPass);
	m_strNewPass.Empty();
	m_strTarget = _T("�n��ǁF�P�c54m��");
	UpdateData(FALSE);
}
void CControlChangePasswordDlg::OnBnClickedRadio5()
{
	GetPassword(eSSOCusc20, m_strOldPass);
	m_strNewPass.Empty();
	m_strTarget = _T("���͌��Ǌǐ��F���V�Y20m�ǉ^�p");
	UpdateData(FALSE);
}
void CControlChangePasswordDlg::OnBnClickedRadio6()
{
	GetPassword(eSSOCusc34, m_strOldPass);
	m_strNewPass.Empty();
	m_strTarget = _T("���͌��Ǌǐ��F���V�Y34m�ǉ^�p");
	UpdateData(FALSE);
}
void CControlChangePasswordDlg::OnBnClickedRadio7()
{
	GetPassword(eSSOCudsc64, m_strOldPass);
	m_strNewPass.Empty();
	m_strTarget = _T("���͌��Ǌǐ��F�P�c64m�ǉ^�p");
	UpdateData(FALSE);
}
void CControlChangePasswordDlg::OnBnClickedRadio8()
{
	GetPassword(eSSOCxdsc54, m_strOldPass);
	m_strNewPass.Empty();
	m_strTarget = _T("���͌��Ǌǐ��F�P�c54m�ǉ^�p");
	UpdateData(FALSE);
}
void CControlChangePasswordDlg::OnBnClickedRadio9()
{
	GetPassword(eTKSCusc20, m_strOldPass);
	m_strNewPass.Empty();
	m_strTarget = _T("�}�g�Ǌǐ��F���V�Y20m�ǉ^�p");
	UpdateData(FALSE);
}
void CControlChangePasswordDlg::OnBnClickedRadio10()
{
	GetPassword(eTKSCusc34, m_strOldPass);
	m_strNewPass.Empty();
	m_strTarget = _T("�}�g�Ǌǐ��F���V�Y34m�ǉ^�p");
	UpdateData(FALSE);
}
void CControlChangePasswordDlg::OnBnClickedRadio11()
{
	GetPassword(eTKSCudsc64, m_strOldPass);
	m_strNewPass.Empty();
	m_strTarget = _T("�}�g�Ǌǐ��F�P�c64m�ǉ^�p");
	UpdateData(FALSE);
}
void CControlChangePasswordDlg::OnBnClickedRadio12()
{
	GetPassword(eTKSCxdsc54, m_strOldPass);
	m_strNewPass.Empty();
	m_strTarget = _T("�}�g�Ǌǐ��F�P�c54m�ǉ^�p");
	UpdateData(FALSE);
}
