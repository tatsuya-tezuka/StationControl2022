// ControlGrant.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ControlGrant.h"
#include "afxdialogex.h"


// CControlGrant ダイアログ

IMPLEMENT_DYNAMIC(CControlGrant, CDialogEx)

CControlGrant::CControlGrant(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlGrant::IDD, pParent)
{

}

CControlGrant::~CControlGrant()
{
}

void CControlGrant::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CControlGrant, CDialogEx)
END_MESSAGE_MAP()


// CControlGrant メッセージ ハンドラー
