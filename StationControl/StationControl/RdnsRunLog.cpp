/*============================================================================*/
/*! RdnsRunLog.cpp

-# �v�惌�f�B�l�X�`�F�b�N20m���
*/
/*============================================================================*/
// RdnsRunLog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "RdnsRunLog.h"
#include "afxdialogex.h"
#include "RdnsChk64.h"


// CRdnsRunLog �_�C�A���O

IMPLEMENT_DYNAMIC(CRdnsRunLog, CDialogBase)

/*============================================================================*/
/*! CRdnsRunLog

-# �R���X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CRdnsRunLog::CRdnsRunLog(CWnd* pParent /*=NULL*/)
: CDialogBase(CRdnsRunLog::IDD, pParent)
{

}

/*============================================================================*/
/*! CRdnsRunLog

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CRdnsRunLog::~CRdnsRunLog()
{
}

void CRdnsRunLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RDNS_RUNLOGLIST, m_RunLogList);
}


BEGIN_MESSAGE_MAP(CRdnsRunLog, CDialogBase)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CRdnsRunLog::OnMenuClose)
END_MESSAGE_MAP()


// CRdnsRunLog ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CRdnsRunLog

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CRdnsRunLog::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	SetWindowPos(NULL, m_left, m_top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	// �V�X�e���R�}���hSC_CLOSE�̎g�p�֎~
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu)
		pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CRdnsChk20

-# �t�@�C��-���郁�j���[����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsRunLog::OnMenuClose()
{
	this->GetParent()->PostMessageW(eMessage_ChildDestroy, 0, 0);

	DestroyWindow();
//	OnOK();
}


void CRdnsRunLog::OnCancel()
{
	this->GetParent()->PostMessageW(eMessage_ChildDestroy, 0, 0);
	DestroyWindow();

//	CDialogEx::OnCancel();
}


void CRdnsRunLog::PostNcDestroy()
{
	delete this;

//	CDialogEx::PostNcDestroy();
}
