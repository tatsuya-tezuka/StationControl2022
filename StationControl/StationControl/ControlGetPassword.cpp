// ControlGetPassword.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ControlGetPassword.h"
#include "afxdialogex.h"


// CControlGetPassword ダイアログ

IMPLEMENT_DYNAMIC(CControlGetPassword, CDialogEx)

CControlGetPassword::CControlGetPassword(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlGetPassword::IDD, pParent)
{

}

CControlGetPassword::~CControlGetPassword()
{
}

void CControlGetPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CControlGetPassword, CDialogEx)
END_MESSAGE_MAP()


// CControlGetPassword メッセージ ハンドラー
