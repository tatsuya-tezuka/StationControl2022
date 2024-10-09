// ControlManagementInputPassword.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ControlManagement.h"
#include "ControlManagementInputPassword.h"
#include "afxdialogex.h"


// CControlManagementInputPassword ダイアログ

IMPLEMENT_DYNAMIC(CControlManagementInputPassword, CDialogEx)

CControlManagementInputPassword::CControlManagementInputPassword(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlManagementInputPassword::IDD, pParent)
	, mInputPassword(_T(""))
{

}

CControlManagementInputPassword::~CControlManagementInputPassword()
{
}

void CControlManagementInputPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, mInputPassword);
}


BEGIN_MESSAGE_MAP(CControlManagementInputPassword, CDialogEx)
	ON_BN_CLICKED(IDOK, &CControlManagementInputPassword::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CControlManagementInputPassword::OnBnClickedCancel)
END_MESSAGE_MAP()


// CControlManagementInputPassword メッセージ ハンドラー


BOOL CControlManagementInputPassword::OnInitDialog()
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


void CControlManagementInputPassword::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (mInputPassword.GetLength() != mMaxPassword){
		MessageBox(_T("入力されたパスワードが8文字ではありません。\n再度入力してください。"), MB_OK);
		return;
	}
	if (mInputPassword != mManPassword){
		MessageBox(_T("入力されたパスワードに誤りがあります。\n再度入力してください。"), MB_OK);
		return;
	}

	CDialogEx::OnOK();
}


void CControlManagementInputPassword::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}
