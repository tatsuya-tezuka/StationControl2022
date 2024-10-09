// DialogBase.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "DialogBase.h"
#include "afxdialogex.h"
#include "HookMsgBox.h"


// CDialogBase ダイアログ

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


// CDialogBase メッセージ ハンドラー


BOOL CDialogBase::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


BOOL CDialogBase::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
	return CDialogEx::Create(nIDTemplate, pParentWnd);
}

void CDialogBase::CenterWindowEx(bool bCloseEnable/*=false*/)
{
#if 0
	if (bCloseEnable == false){
		// システムコマンドSC_CLOSEの使用禁止
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

-# メッセージ処理

@breif	Enterキー（Escキー）を押しても閉じないようにする。
@param  なし
@retval なし
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

// 終了確認
void CDialogBase::OnClose()
{
	CString title;
	GetWindowText(title);

	if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("終了します。\nよろしいですか？"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
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
