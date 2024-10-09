// SelectServerClient.cpp : 実装ファイル
//

#include "stdafx.h"
#include "SharedAlarm.h"
#include "SelectServerClient.h"
#include "afxdialogex.h"


// CSelectServerClient ダイアログ

IMPLEMENT_DYNAMIC(CSelectServerClient, CDialogEx)

CSelectServerClient::CSelectServerClient(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectServerClient::IDD, pParent)
	, m_nMode(0)
{

}

CSelectServerClient::~CSelectServerClient()
{
}

void CSelectServerClient::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nMode);
}


BEGIN_MESSAGE_MAP(CSelectServerClient, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelectServerClient::OnBnClickedOk)
END_MESSAGE_MAP()


// CSelectServerClient メッセージ ハンドラー


BOOL CSelectServerClient::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CSelectServerClient::OnBnClickedOk()
{
	UpdateData(TRUE);

	CDialogEx::OnOK();
}
