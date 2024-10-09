/*============================================================================*/
/*! SelectLNADlg.cpp

-# AGC校正テーブルLNA切替:LNA選択
*/
/*============================================================================*/
// SelectLNADlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "SelectLNADlg.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"
#include "HookMsgBox.h"


// CSelectLNADlg ダイアログ

IMPLEMENT_DYNAMIC(CSelectLNADlg, CDialogBase)

/*============================================================================*/
/*! CSelectLNADlg

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CSelectLNADlg::CSelectLNADlg(CWnd* pParent /*=NULL*/)
: CDialogBase(CSelectLNADlg::IDD, pParent)
, m_nSelLNA(eLNA1)
{

}

/*============================================================================*/
/*! CSelectLNADlg

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CSelectLNADlg::~CSelectLNADlg()
{
}

void CSelectLNADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSelectLNADlg, CDialogBase)
	ON_BN_CLICKED(IDC_SELECT_LNA1, &CSelectLNADlg::OnBnClickedSelectLna1)
	ON_BN_CLICKED(IDC_SELECT_LNA2, &CSelectLNADlg::OnBnClickedSelectLna2)
END_MESSAGE_MAP()


// CSelectLNADlg メッセージ ハンドラー


/*============================================================================*/
/*! CSelectLNADlg

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CSelectLNADlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// 選択されているLNAにチェックを入れる
	if (m_nSelLNA == eLNA1)
	{
		((CButton*)GetDlgItem(IDC_SELECT_LNA1))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_SELECT_LNA2))->SetCheck(BST_UNCHECKED);
	}
	else if (m_nSelLNA == eLNA2)
	{
		((CButton*)GetDlgItem(IDC_SELECT_LNA1))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_SELECT_LNA2))->SetCheck(BST_CHECKED);
	}

	// 実行ボタンを非活性にしておく
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	// 監視モード時は選択ボタンを非活性にする
	if (theApp.GetExecuteMode() != eExecuteMode_Control)
	{
		((CButton*)GetDlgItem(IDC_SELECT_LNA1))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_SELECT_LNA2))->EnableWindow(FALSE);
	}

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CSelectLNADlg

-# 実行ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CSelectLNADlg::OnOK()
{
	UpdateData(TRUE);

	CString str = _T("");
	if (((CButton*)GetDlgItem(IDC_SELECT_LNA1))->GetCheck() == BST_CHECKED)
	{
		str = _T("LNA1");
	}
	else if (((CButton*)GetDlgItem(IDC_SELECT_LNA2))->GetCheck() == BST_CHECKED)
	{
		str = _T("LNA2");
	}

	if (str.IsEmpty())
	{
		return;
	}

	CString strMsg;
//	strMsg.Format(_T("%sに切り替え実行しますか？"), str);
//	if (IDCANCEL == AfxMessageBoxHooked(strMsg, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
//	{
//		return;
//	}

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = AGCTBL_LNASEL_SH;

	if (theApp.GetSelectStation() == eStation_Usuda64)
		stParam.strRemShellParam.Format(_T("udsc64 %s"), (LPCTSTR)str);
	else if (theApp.GetSelectStation() == eStation_Usuda642)
		stParam.strRemShellParam.Format(_T("udsc54 %s"), (LPCTSTR)str);
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
		stParam.strRemShellParam.Format(_T("usc34 %s"), (LPCTSTR)str);
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
		stParam.strRemShellParam.Format(_T("usc20 %s"), (LPCTSTR)str);

	int err = CSshShell::ExecShell(stParam);

	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CSelectLNADlg"), _T("OnOK"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err == 1)
	{
		// 切替成功
		strMsg.Format(_T("%sに切り替えました。"), str);
		AfxMessageBox(strMsg, MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CSelectLNADlg"), _T("OnOK"), _T("Error"), strDebug, nLogEx::error);

		AfxMessageBox(_T("LNA切替に失敗しました。"), MB_OK | MB_ICONWARNING);
	}

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! CSelectLNADlg

-# LNA1ラジオボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CSelectLNADlg::OnBnClickedSelectLna1()
{
	UpdateData(TRUE);
	EnableExecButton();
}


/*============================================================================*/
/*! CSelectLNADlg

-# LNA2ラジオボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CSelectLNADlg::OnBnClickedSelectLna2()
{
	UpdateData(TRUE);
	EnableExecButton();
}


/*============================================================================*/
/*! CSelectLNADlg

-# 実行ボタン活性・非活性処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CSelectLNADlg::EnableExecButton()
{
	// すでに選択されているLNAの場合は、実行ボタンは非活性にする
	if (m_nSelLNA == eLNA1)
	{
		if (((CButton*)GetDlgItem(IDC_SELECT_LNA1))->GetCheck() == BST_CHECKED)
		{
			GetDlgItem(IDOK)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDOK)->EnableWindow(TRUE);
		}
	}
	else if (m_nSelLNA == eLNA2)
	{
		if (((CButton*)GetDlgItem(IDC_SELECT_LNA2))->GetCheck() == BST_CHECKED)
		{
			GetDlgItem(IDOK)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDOK)->EnableWindow(TRUE);
		}
	}
}
