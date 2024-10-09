/*============================================================================*/
/*! TblFailSelection.cpp

-# ファイル選択画面共通

-#  AGC校正テーブルファイルコピー・ファイル選択画面
-#  AGC校正テーブルファイル復元・ファイル選択画面
-#  局内ディレイ値テーブルファイルコピー・ファイル選択画面
-#  局内ディレイ値テーブルファイル復元・ファイル選択画面
*/
/*============================================================================*/
// TblFailSelection.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TblFailSelection.h"
#include "afxdialogex.h"


// CTblFailSelection ダイアログ

IMPLEMENT_DYNAMIC(CTblFailSelection, CDialogEx)

CTblFailSelection::CTblFailSelection(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTblFailSelection::IDD, pParent)
	, m_strFile(_T(""))
{
}

CTblFailSelection::~CTblFailSelection()
{
}

void CTblFailSelection::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_FileList);
	DDX_Control(pDX, IDC_EDIT1, m_FileEdit);
	DDX_Text(pDX, IDC_EDIT1, m_strFile);
}


BEGIN_MESSAGE_MAP(CTblFailSelection, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTblFailSelection::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTblFailSelection メッセージ ハンドラー


/*============================================================================*/
/*! CTblFailSelection

-# ダイアログ初期化処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CTblFailSelection::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CTblFailSelection

-# 『実行』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTblFailSelection::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}
