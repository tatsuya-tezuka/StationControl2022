/*============================================================================*/
/*! ControlManagementChangePasswordDlg

-# 制御権利管理パスワード変更
*/
/*============================================================================*/
// ControlManagementChangePasswordDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ControlManagement.h"
#include "ControlManagementChangePasswordDlg.h"
#include "afxdialogex.h"


// CControlManagementChangePasswordDlg ダイアログ

IMPLEMENT_DYNAMIC(CControlManagementChangePasswordDlg, CDialogEx)

CControlManagementChangePasswordDlg::CControlManagementChangePasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlManagementChangePasswordDlg::IDD, pParent)
	, mCurrentPass(_T(""))
	, mNewPass(_T(""))
	, mRenewPass(_T(""))
{

}

CControlManagementChangePasswordDlg::~CControlManagementChangePasswordDlg()
{
}

void CControlManagementChangePasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CURPASSWORD, mCurrentPass);
	DDX_Text(pDX, IDC_EDIT_NEWPASSWORD, mNewPass);
	DDX_Text(pDX, IDC_EDIT_RENEWPASSWORD, mRenewPass);
	DDX_Control(pDX, IDC_EDIT_CURPASSWORD, mEdCurrentPass);
	DDX_Control(pDX, IDC_EDIT_NEWPASSWORD, mEdNewPass);
	DDX_Control(pDX, IDC_EDIT_RENEWPASSWORD, mEdRenewPass);
}


BEGIN_MESSAGE_MAP(CControlManagementChangePasswordDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CControlManagementChangePasswordDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CControlManagementChangePasswordDlg メッセージ ハンドラー


BOOL CControlManagementChangePasswordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CFileAccess& ac = theApp.GetFileAccess();
	if (ac.ReadManagementPassword(mManPassword) == false){
		// パスワードが設定されていないので、デフォルトパスワードを設定する
		mManPassword = mDefaultPassword;
		mManPassword = theApp.ManEncodeDecode(mManPassword);
		ac.WriteManagementPassword(mManPassword);
	}
	mManPassword = theApp.ManEncodeDecode(mManPassword);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CControlManagementChangePasswordDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (mCurrentPass.GetLength() != mMaxPassword){
		MessageBox(_T("入力されたパスワードが8文字ではありません。\n再度入力してください。"), MB_OK);
		mEdCurrentPass.SetSel(0, -1);
		mEdCurrentPass.SetFocus();
		return;
	}
	if (mNewPass.GetLength() != mMaxPassword){
		MessageBox(_T("入力されたパスワードが8文字ではありません。\n再度入力してください。"), MB_OK);
		mEdNewPass.SetSel(0, -1);
		mEdNewPass.SetFocus();
		return;
	}
	if (mRenewPass.GetLength() != mMaxPassword){
		MessageBox(_T("入力されたパスワードが8文字ではありません。\n再度入力してください。"), MB_OK);
		mEdRenewPass.SetSel(0, -1);
		mEdRenewPass.SetFocus();
		return;
	}
	if (mCurrentPass != mManPassword){
		MessageBox(_T("入力されたパスワードに誤りがあります。\n再度入力してください。"), MB_OK);
		mEdCurrentPass.SetSel(0, -1);
		mEdCurrentPass.SetFocus();
		return;
	}
	if (mNewPass != mRenewPass){
		MessageBox(_T("入力されたパスワードに誤りがあります。\n再度入力してください。"), MB_OK);
		mEdRenewPass.SetSel(0, -1);
		mEdRenewPass.SetFocus();
		return;
	}

	CFileAccess& ac = theApp.GetFileAccess();
	CString str = theApp.ManEncodeDecode(mNewPass);
	ac.WriteManagementPassword(str);

	CDialogEx::OnOK();
}
