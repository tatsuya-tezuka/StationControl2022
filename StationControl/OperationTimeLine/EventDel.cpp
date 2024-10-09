// EventDel.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "EventDel.h"
#include "afxdialogex.h"


// CEventDel ダイアログ

IMPLEMENT_DYNAMIC(CEventDel, CDialogEx)

CEventDel::CEventDel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEventDel::IDD, pParent)
	, m_strText(_T(""))
{

}

CEventDel::~CEventDel()
{
}

void CEventDel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TEXT, m_strText);
}


BEGIN_MESSAGE_MAP(CEventDel, CDialogEx)
END_MESSAGE_MAP()


// CEventDel メッセージ ハンドラー


BOOL CEventDel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_strText.Format(mEventDelText[mMode], mTargetNo + 1);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CEventDel::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	CDialogEx::OnOK();
}
