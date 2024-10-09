// OptionAnt.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "OptionAnt.h"
#include "afxdialogex.h"


// COptionAnt ダイアログ

IMPLEMENT_DYNAMIC(COptionAnt, CDialogEx)

COptionAnt::COptionAnt(CWnd* pParent /*=NULL*/)
	: CDialogEx(COptionAnt::IDD, pParent)
	, m_nAntMode(0)
{

}

COptionAnt::~COptionAnt()
{
}

void COptionAnt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nAntMode);
}


BEGIN_MESSAGE_MAP(COptionAnt, CDialogEx)
END_MESSAGE_MAP()


// COptionAnt メッセージ ハンドラー


BOOL COptionAnt::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void COptionAnt::OnOK()
{
	UpdateData(TRUE);

	CDialogEx::OnOK();
}
