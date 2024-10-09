/*============================================================================*/
/*! SessionLogin.cpp

-# �Z�b�V������ԉ��
*/
/*============================================================================*/
// SessionLogin.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "SessionLogin.h"
#include "afxdialogex.h"


// CSessionLogin �_�C�A���O

IMPLEMENT_DYNAMIC(CSessionLogin, CDialogBase)

/*============================================================================*/
/*! CSessionLogin

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CSessionLogin::CSessionLogin(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSessionLogin::IDD, pParent)
	, m_passwd(_T(""))
{

}

/*============================================================================*/
/*! CSessionLogin

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSessionLogin::~CSessionLogin()
{
}

void CSessionLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SESSIONSTATUS_LOGIN_EDIT1, m_passwd);
}


BEGIN_MESSAGE_MAP(CSessionLogin, CDialogBase)
	ON_BN_CLICKED(IDOK, &CSessionLogin::OnBnClickedOk)
END_MESSAGE_MAP()


// CSessionLogin ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CSessionLogin

-# OK�{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSessionLogin::OnBnClickedOk()
{
	// �Z�b�V�����ڑ�����

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! CSessionLogin

-# �_�C�A���O����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
BOOL CSessionLogin::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	TCHAR szTitle[256];
	memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));

	switch (m_sessionType)
	{
	case eSessionType_Monitor:
		if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONLOGIN_MONITOR, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
			return FALSE;
		break;
	case eSessionType_Ctrl:
		if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONLOGIN_CTRL, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
			return FALSE;
		break;
	case eSessionType_Notify:
		if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONLOGIN_NOTIFY, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
			return FALSE;
		break;
	case eSessionType_History:
		if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONLOGIN_HISTORY, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
			return FALSE;
		break;
	default:
		return FALSE;
		break;
	}

	SetWindowText(szTitle);
	m_passwd.Format(_T(""));

	UpdateData(FALSE);

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
