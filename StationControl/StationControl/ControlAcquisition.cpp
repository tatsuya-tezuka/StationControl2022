// ControlAcquisition.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "ControlAcquisition.h"
#include "afxdialogex.h"


// CControlAcquisition �_�C�A���O

IMPLEMENT_DYNAMIC(CControlAcquisition, CDialogEx)

CControlAcquisition::CControlAcquisition(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlAcquisition::IDD, pParent)
	, m_strText(_T(""))
	, m_strPassword(_T(""))
{
	mGetTimer = 0;
}

CControlAcquisition::~CControlAcquisition()
{
}

void CControlAcquisition::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TEXT, m_strText);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
}


BEGIN_MESSAGE_MAP(CControlAcquisition, CDialogEx)
	ON_BN_CLICKED(IDOK, &CControlAcquisition::OnBnClickedOk)
	ON_BN_CLICKED(IDMODE, &CControlAcquisition::OnBnClickedMode)
	ON_BN_CLICKED(IDCANCEL, &CControlAcquisition::OnBnClickedCancel)
END_MESSAGE_MAP()


// CControlAcquisition ���b�Z�[�W �n���h���[


BOOL CControlAcquisition::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CFileAccess ac;

	stLocalControl local;
	stControl data;
	char buf[mMaxPassword + 1] = { 0 };
	CString strDefaultPass;
	int type = -1;
	bool bControl = true;

	// �A�v���N���[���̑Ώۋǂ̎擾
	// ������SSOC�ASTTN�ATKSC�����ʂł��Ȃ��̂ŁATKSC�ōs��
	int target = ac.GetClientTerminal();
	mClientTerminal = target;

	/*
		�ȉ��̏������S�Đ��������ꍇ�ɂ̂ݐ��䌠���̎擾���\�ƂȂ�B
		�@�p�X���[�h�E���䌠���Ǘ��t�@�C���́u���䌠���t�^�L���t���O�v�� �u1�i�L�j�v
		�A���ݓ������A�p�X���[�h�E���䌠���Ǘ��t�@�C���́u���䌠���t�^�J�n�����v����u���䌠���t�^���ԁv�ȓ�
		�B�p�X���[�h�E���䌠���Ǘ��t�@�C���́u���䌠���t�^�J�n�����v���A���䌠���擾�󋵃t�@�C���́u���䌠���擾�J�n�����v��������
		�C���䌠���擾�󋵃t�@�C���́u���䌠���擾�L���t���O�v���A�u0�i���j�v
	*/

	// ���䌠���擾�󋵃t�@�C���擾
	switch (theApp.GetSelectStation()){
	case	eStation_Usuda64:
		type = eLOCALudsc64;
		break;
	case	eStation_Usuda642:
		type = eLOCALxdsc54;
		break;
	case	eStation_Uchinoura34:
		type = eLOCALusc34;
		break;
	case	eStation_Uchinoura20:
		type = eLOCALusc20;
		break;
	default:
		EndDialog(IDABORT);
	}

	memset(&local, 0, sizeof(stLocalControl));
	ac.ReadLocalControl(local, type);

	// �p�X���[�h�E���䌠���Ǘ��t�@�C���̎擾
	type = GetControlType(target);
	if (type >= 0){
		bool ret = false;
		switch (target){
		case	eClientSttn:
			ret = ac.ReadSOSControl(data, type);
			break;
		case	eClientSsoc:
			ret = ac.ReadSSOCControl(data, type);
			break;
		case	eClientTksc:
			ret = ac.ReadTKSCControl(data, type);
			break;
		}
		if (ret == true){
			// �p�X���[�h�擾
			memset(buf, 0, mMaxPassword + 1);
			memcpy(buf, data.password, mMaxPassword);
			strDefaultPass = CString(buf);
			strDefaultPass = theApp.ManEncodeDecode(strDefaultPass);
			CString str = CString(data.lifetime);
			mGetTimer = _wtoi64(str);
			bControl = CheckControl(target, data, local);
		}
		else{
			strDefaultPass = mDefaultPassword;
			bControl = false;
		}
		mGetPassword = strDefaultPass;
	}
	else{
		strDefaultPass = mDefaultPassword;
		bControl = false;
	}

	CString strStation = mStationString[theApp.GetSelectStation()];
	m_strText.Format(_T("%s�̐��䌠�����擾����ꍇ�A\n�p�X���[�h����͂��Ă��������B"), (LPCTSTR)strStation);

	if (bControl == true){
		// ���䌠���擾�[�������݂��Ă��Ȃ�
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDMODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(TRUE);
	}
	else{
		// ���䌠���擾�[�������݂��Ă���
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDMODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(FALSE);
		m_strText.Format(_T("%s�̐��䌠���͑��[���Ŏ擾���ł��B"), (LPCTSTR)strStation);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! ���䌠���Ǘ�

-# �p�X���[�h�E���䌠���Ǘ��Ώۂ̎擾

@param
@retval

*/
/*============================================================================*/
int CControlAcquisition::GetControlType(int target)
{
	int type;
	switch (theApp.GetSelectStation()){
	case	eStation_Usuda64:
		if (target == eClientSttn)
			type = eSOSudsc64;
		else if (target == eClientSsoc)
			type = eSSOCudsc64;
		else if (target == eClientTksc)
			type = eTKSCudsc64;
		else
			return -1;
		break;
	case	eStation_Usuda642:
		if (target == eClientSttn)
			type = eSOSxdsc54;
		else if (target == eClientSsoc)
			type = eSSOCxdsc54;
		else if (target == eClientTksc)
			type = eTKSCxdsc54;
		else
			return -1;
		break;
	case	eStation_Uchinoura34:
		if (target == eClientSttn)
			type = eSOSusc34;
		else if (target == eClientSsoc)
			type = eSSOCusc34;
		else if (target == eClientTksc)
			type = eTKSCusc34;
		else
			return -1;
		break;
	case	eStation_Uchinoura20:
		if (target == eClientSttn)
			type = eSOSusc20;
		else if (target == eClientSsoc)
			type = eSSOCusc20;
		else if (target == eClientTksc)
			type = eTKSCusc20;
		else
			return -1;
		break;
	default:
		return -1;
	}
	return type;
}

/*============================================================================*/
/*! ���䌠���Ǘ�

-# ���䌠���擾�\���̃`�F�b�N

@param
@retval

*/
/*============================================================================*/
bool CControlAcquisition::CheckControl(int target, stControl& data, stLocalControl& local)
{
	/*
	�ȉ��̏������S�Đ��������ꍇ�ɂ̂ݐ��䌠���̎擾���\�ƂȂ�B
	�@�p�X���[�h�E���䌠���Ǘ��t�@�C���́u���䌠���t�^�L���t���O�v�� �u1�i�L�j�v
	�A���ݓ������A�p�X���[�h�E���䌠���Ǘ��t�@�C���́u���䌠���t�^�J�n�����v����u���䌠���t�^���ԁv�ȓ�
	�B�p�X���[�h�E���䌠���Ǘ��t�@�C���́u���䌠���t�^�J�n�����v���A���䌠���擾�󋵃t�@�C���́u���䌠���擾�J�n�����v��������
	�C���䌠���擾�󋵃t�@�C���́u���䌠���擾�L���t���O�v���A�u0�i���j�v
	*/

	// �@�p�X���[�h�E���䌠���Ǘ��t�@�C���́u���䌠���t�^�L���t���O�v�� �u1�i�L�j�v
	if (data.flag == 0)
		return false;

	if (target != eClientTksc){
		// �}�g�Ǌǐ��ȊO
		// �A���ݓ������A�p�X���[�h�E���䌠���Ǘ��t�@�C���́u���䌠���t�^�J�n�����v����u���䌠���t�^���ԁv�ȓ�
		// ���ݓ����̎擾
		CTime ct;
		theApp.GetCurrentUtcTime(ct);
		// ���䌠���t�^�J�n����
		CString cbuf, strDate, strTime;
		cbuf = CString(data.latestdate);
		AfxExtractSubString(strDate, cbuf, 0, ' ');
		AfxExtractSubString(strTime, cbuf, 1, ' ');
		SYSTEMTIME st;
		memset(&st, 0, sizeof(SYSTEMTIME));
		cbuf = strDate;
		AfxExtractSubString(strDate, cbuf, 0, '-');
		st.wYear = _wtoi(strDate);
		AfxExtractSubString(strDate, cbuf, 1, '-');
		st.wMonth = _wtoi(strDate);
		AfxExtractSubString(strDate, cbuf, 2, '-');
		st.wDay = _wtoi(strDate);
		cbuf = strTime;
		AfxExtractSubString(strTime, cbuf, 0, ':');
		st.wHour = _wtoi(strTime);
		AfxExtractSubString(strTime, cbuf, 1, ':');
		st.wMinute = _wtoi(strTime);
		AfxExtractSubString(strTime, cbuf, 2, ':');
		st.wSecond = _wtoi(strTime);
		CTime tm(st);
		if (ct.GetTime() < tm.GetTime() || ct.GetTime() > (tm.GetTime() + mGetTimer))
			return false;

		// �B�p�X���[�h�E���䌠���Ǘ��t�@�C���́u���䌠���t�^�J�n�����v���A���䌠���擾�󋵃t�@�C���́u���䌠���擾�J�n�����v��������
		cbuf = CString(local.latestdate);
		AfxExtractSubString(strDate, cbuf, 0, ' ');
		AfxExtractSubString(strTime, cbuf, 1, ' ');
		memset(&st, 0, sizeof(SYSTEMTIME));
		cbuf = strDate;
		AfxExtractSubString(strDate, cbuf, 0, '-');
		st.wYear = _wtoi(strDate);
		AfxExtractSubString(strDate, cbuf, 1, '-');
		st.wMonth = _wtoi(strDate);
		AfxExtractSubString(strDate, cbuf, 2, '-');
		st.wDay = _wtoi(strDate);
		cbuf = strTime;
		AfxExtractSubString(strTime, cbuf, 0, ':');
		st.wHour = _wtoi(strTime);
		AfxExtractSubString(strTime, cbuf, 1, ':');
		st.wMinute = _wtoi(strTime);
		AfxExtractSubString(strTime, cbuf, 2, ':');
		st.wSecond = _wtoi(strTime);
		CTime ltm(st);
		if (ltm.GetTime() >= tm.GetTime())
			return false;
	}

	// �C���䌠���擾�󋵃t�@�C���́u���䌠���擾�L���t���O�v���A�u0�i���j�v
	if (local.flag == 1)
		return false;

	return true;
}

/*============================================================================*/
/*! ���䌠���Ǘ�

-# ���䌠���擾�󋵃t�@�C���̃t���O���N���A����

@param
@retval

*/
/*============================================================================*/
void CControlAcquisition::ResetControlFlag()
{
	CFileAccess ac;
	stLocalControl local;
	int type;

	// ���䌠���擾�󋵃t�@�C���擾
	switch (theApp.GetSelectStation()){
	case	eStation_Usuda64:
		type = eLOCALudsc64;
		break;
	case	eStation_Usuda642:
		type = eLOCALxdsc54;
		break;
	case	eStation_Uchinoura34:
		type = eLOCALusc34;
		break;
	case	eStation_Uchinoura20:
		type = eLOCALusc20;
		break;
	default:
		return;
	}

	memset(&local, 0, sizeof(stLocalControl));
	ac.ReadLocalControl(local, type);

	local.flag = 0;
	sprintf_s(local.lifetime, mMaxLifetime, "0");
	ac.WriteLocalControl(local, type);
}

void CControlAcquisition::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_strPassword.GetLength() != mMaxPassword){
		MessageBox(_T("���͂��ꂽ�p�X���[�h��8�����ł͂���܂���B\n�Ď����[�h�ŋN�����܂��B"), MB_OK);
		EndDialog(IDABORT);
		return;
	}
	if (m_strPassword != mGetPassword){
		MessageBox(_T("���͂��ꂽ�p�X���[�h�Ɍ�肪����܂��B\n�Ď����[�h�ŋN�����܂��B"), MB_OK);
		EndDialog(IDABORT);
		return;
	}

	// ���䌠�����擾
	CFileAccess ac;
	stLocalControl local;
	int type = -1;
	switch (theApp.GetSelectStation()){
	case	eStation_Usuda64:
		type = eLOCALudsc64;
		break;
	case	eStation_Usuda642:
		type = eLOCALxdsc54;
		break;
	case	eStation_Uchinoura34:
		type = eLOCALusc34;
		break;
	case	eStation_Uchinoura20:
		type = eLOCALusc20;
		break;
	default:
		EndDialog(IDABORT);
	}

	ac.ReadLocalControl(local, type);
	local.flag = 1;
	sprintf_s(local.lifetime, mMaxLifetime, "%I64d", mGetTimer);
	ac.WriteLocalControl(local, type);

	CDialogEx::OnOK();
}

void CControlAcquisition::OnBnClickedMode()
{
	EndDialog(IDABORT);
}


void CControlAcquisition::OnBnClickedCancel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDialogEx::OnCancel();
}

