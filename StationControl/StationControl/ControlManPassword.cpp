// ControlManPassword.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ControlManPassword.h"
#include "afxdialogex.h"


// CControlManPassword ダイアログ

IMPLEMENT_DYNAMIC(CControlManPassword, CDialogEx)

CControlManPassword::CControlManPassword(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlManPassword::IDD, pParent)
{

}

CControlManPassword::~CControlManPassword()
{
}

void CControlManPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CControlManPassword, CDialogEx)
END_MESSAGE_MAP()


// CControlManPassword メッセージ ハンドラー
