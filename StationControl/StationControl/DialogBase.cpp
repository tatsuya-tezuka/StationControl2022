// DialogBase.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "DialogBase.h"
#include "afxdialogex.h"
#include "HookMsgBox.h"


// CDialogBase �_�C�A���O

IMPLEMENT_DYNAMIC(CDialogBase, CDialogEx)

CDialogBase::CDialogBase(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD, pParent)
	, m_nSubCode(0)
{
	m_IDD = IDD;
	m_modal = false;
}

CDialogBase::~CDialogBase()
{
}

void CDialogBase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogBase, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDialogBase ���b�Z�[�W �n���h���[


BOOL CDialogBase::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


BOOL CDialogBase::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
	return CDialogEx::Create(nIDTemplate, pParentWnd);
}

void CDialogBase::CenterWindowEx(bool bCloseEnable/*=false*/)
{
#if 0
	if (bCloseEnable == false){
		// �V�X�e���R�}���hSC_CLOSE�̎g�p�֎~
		CMenu* pSysMenu = GetSystemMenu(FALSE);
		if (pSysMenu)
			pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
	}
#endif
	if (theApp.GetTimeLineCall() == true){
		theApp.SetTimeLineCall(false);
		CWnd* pWnd = CWnd::FromHandle(theApp.GetTimeLineHandle());
		//CenterWindow(pWnd);
		CenterWindow();
	}
	else{
		CenterWindow();
	}
}

BOOL CDialogBase::UpdateDlg()
{
	return TRUE;
}


/*============================================================================*/
/*! CDialogBase

-# ���b�Z�[�W����

@breif	Enter�L�[�iEsc�L�[�j�������Ă����Ȃ��悤�ɂ���B
@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CDialogBase::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			return FALSE;
		case VK_ESCAPE:
			return FALSE;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogBase::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (nStatus == SW_PARENTCLOSING)
	{
		m_bClosedByEndDialog = FALSE;
		return;
	}

//	if (bShow == SW_SHOW)
//	if ((bShow == SW_SHOW) || (bShow == SW_SHOWNORMAL))
	if (bShow)
	{
		UpdateDlg();
	}
	m_bClosedByEndDialog = !bShow;
}

// �I���m�F
void CDialogBase::OnClose()
{
	CString title;
	GetWindowText(title);

	if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("�I�����܂��B\n��낵���ł����H"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return;

	if (m_modal)
	{
		CDialogEx::OnClose();
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}

INT_PTR CDialogBase::DoModal()
{
	m_modal = true;
	return CDialogEx::DoModal();
}
