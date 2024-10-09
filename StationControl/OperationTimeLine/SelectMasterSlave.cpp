// SelectMasterSlave.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "SelectMasterSlave.h"
#include "afxdialogex.h"


// CSelectMasterSlave ダイアログ

IMPLEMENT_DYNAMIC(CSelectMasterSlave, CDialogEx)

CSelectMasterSlave::CSelectMasterSlave(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectMasterSlave::IDD, pParent)
	, mSelectMode(0)
{

}

CSelectMasterSlave::~CSelectMasterSlave()
{
}

void CSelectMasterSlave::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, mSelectMode);
}


BEGIN_MESSAGE_MAP(CSelectMasterSlave, CDialogEx)
END_MESSAGE_MAP()


// CSelectMasterSlave メッセージ ハンドラー


BOOL CSelectMasterSlave::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CSelectMasterSlave::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	CDialogEx::OnOK();
}
