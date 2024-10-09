// SelectEventMode.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "SelectEventMode.h"
#include "afxdialogex.h"
#include "../StationControl/HookMsgBox.h"

// CSelectEventMode ダイアログ

IMPLEMENT_DYNAMIC(CSelectEventMode, CDialogEx)

CSelectEventMode::CSelectEventMode(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectEventMode::IDD, pParent)
	, m_nEventMode(0)
{

}

CSelectEventMode::~CSelectEventMode()
{
}

void CSelectEventMode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nEventMode);
}


BEGIN_MESSAGE_MAP(CSelectEventMode, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelectEventMode::OnBnClickedOk)
END_MESSAGE_MAP()


// CSelectEventMode メッセージ ハンドラー


BOOL CSelectEventMode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CSelectEventMode::OnBnClickedOk()
{
	UpdateData();

//	if (m_nEventMode == 1)
//	{
//		// 手動モード
//		if (IDCANCEL == AfxMessageBoxHooked(_T("登録済みのイベントが削除されますが、よろしいでしょうか？"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
//		{
//			return;
//		}
//	}

	CDialogEx::OnOK();
}
