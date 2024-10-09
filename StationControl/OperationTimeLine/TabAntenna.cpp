// TabAntenna.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "TabAntenna.h"
#include "afxdialogex.h"


// CTabAntenna ダイアログ

IMPLEMENT_DYNCREATE(CTabAntenna, CDialogEx)

CTabAntenna::CTabAntenna(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabAntenna::IDD, pParent)
{

}

CTabAntenna::~CTabAntenna()
{
}

void CTabAntenna::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabAntenna, CDialogEx)
END_MESSAGE_MAP()


// CTabAntenna メッセージ ハンドラー
