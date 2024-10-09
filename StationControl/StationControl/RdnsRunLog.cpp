/*============================================================================*/
/*! RdnsRunLog.cpp

-# 計画レディネスチェック20m画面
*/
/*============================================================================*/
// RdnsRunLog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "RdnsRunLog.h"
#include "afxdialogex.h"
#include "RdnsChk64.h"


// CRdnsRunLog ダイアログ

IMPLEMENT_DYNAMIC(CRdnsRunLog, CDialogBase)

/*============================================================================*/
/*! CRdnsRunLog

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CRdnsRunLog::CRdnsRunLog(CWnd* pParent /*=NULL*/)
: CDialogBase(CRdnsRunLog::IDD, pParent)
{

}

/*============================================================================*/
/*! CRdnsRunLog

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CRdnsRunLog::~CRdnsRunLog()
{
}

void CRdnsRunLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RDNS_RUNLOGLIST, m_RunLogList);
}


BEGIN_MESSAGE_MAP(CRdnsRunLog, CDialogBase)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CRdnsRunLog::OnMenuClose)
END_MESSAGE_MAP()


// CRdnsRunLog メッセージ ハンドラー


/*============================================================================*/
/*! CRdnsRunLog

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CRdnsRunLog::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	SetWindowPos(NULL, m_left, m_top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	// システムコマンドSC_CLOSEの使用禁止
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu)
		pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CRdnsChk20

-# ファイル-閉じるメニュー処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsRunLog::OnMenuClose()
{
	this->GetParent()->PostMessageW(eMessage_ChildDestroy, 0, 0);

	DestroyWindow();
//	OnOK();
}


void CRdnsRunLog::OnCancel()
{
	this->GetParent()->PostMessageW(eMessage_ChildDestroy, 0, 0);
	DestroyWindow();

//	CDialogEx::OnCancel();
}


void CRdnsRunLog::PostNcDestroy()
{
	delete this;

//	CDialogEx::PostNcDestroy();
}
