/*============================================================================*/
/*! TrsControlDetail.cpp

-# 送信制御-設備詳細制御画面
*/
/*============================================================================*/
// TrsControlDetail.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrsControlDetail.h"
#include "afxdialogex.h"


// CTrsControlDetail ダイアログ

IMPLEMENT_DYNAMIC(CTrsControlDetail, CDialogEx)

CTrsControlDetail::CTrsControlDetail(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTrsControlDetail::IDD, pParent)
	, m_strCtrlName(_T(""))
	, m_CtrlValue(0)
{

}

CTrsControlDetail::~CTrsControlDetail()
{
}

void CTrsControlDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TRSCTRL_D_CTRL_NAME_E_STATIC, m_strCtrlName);
	DDX_Text(pDX, IDC_EDIT1, m_CtrlValue);
}


BEGIN_MESSAGE_MAP(CTrsControlDetail, CDialogEx)
END_MESSAGE_MAP()


// CTrsControlDetail メッセージ ハンドラー
