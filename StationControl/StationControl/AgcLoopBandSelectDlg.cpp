// AgcLoopBandSelectDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "AgcLoopBandSelectDlg.h"
#include "afxdialogex.h"


// AgcLoopBandSelectDlg ダイアログ

IMPLEMENT_DYNAMIC(AgcLoopBandSelectDlg, CDialogEx)

AgcLoopBandSelectDlg::AgcLoopBandSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(AgcLoopBandSelectDlg::IDD, pParent)
{

}

AgcLoopBandSelectDlg::~AgcLoopBandSelectDlg()
{
}

void AgcLoopBandSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOOPBANDCMB, m_LoopBandCmb);
}


BEGIN_MESSAGE_MAP(AgcLoopBandSelectDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &AgcLoopBandSelectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// AgcLoopBandSelectDlg メッセージ ハンドラー


BOOL AgcLoopBandSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (int i = 0; i < m_LBCmbList.size(); ++i)
	{
		m_LoopBandCmb.InsertString(-1, m_LBCmbList[i]);
	}
	m_LoopBandCmb.SetCurSel(0);

	// システムコマンドSC_CLOSEの使用禁止
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu)
		pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

	CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void AgcLoopBandSelectDlg::OnBnClickedOk()
{
	CString str;
	m_LoopBandCmb.GetLBText(m_LoopBandCmb.GetCurSel(), str);
	m_strLoopBand = str;

	CDialogEx::OnOK();
}
