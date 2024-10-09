// ControlLogin.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ControlLogin.h"
#include "afxdialogex.h"


// CControlLogin ダイアログ

IMPLEMENT_DYNAMIC(CControlLogin, CDialogEx)

CControlLogin::CControlLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlLogin::IDD, pParent)
{

}

CControlLogin::~CControlLogin()
{
}

void CControlLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CControlLogin, CDialogEx)
END_MESSAGE_MAP()


// CControlLogin メッセージ ハンドラー
