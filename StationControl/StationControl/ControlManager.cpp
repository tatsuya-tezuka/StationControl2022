// ControlManager.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ControlManager.h"
#include "afxdialogex.h"


// CControlManager ダイアログ

IMPLEMENT_DYNAMIC(CControlManager, CDialogEx)

CControlManager::CControlManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlManager::IDD, pParent)
{

}

CControlManager::~CControlManager()
{
}

void CControlManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CControlManager, CDialogEx)
END_MESSAGE_MAP()


// CControlManager メッセージ ハンドラー
