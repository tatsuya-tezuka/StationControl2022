/*============================================================================*/
/*! EquipmentRecSelectFile.cpp

-# 設備記録 ダンプ定義ファイル選択画面
*/
/*============================================================================*/
// EquipmentRecSelectFile.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "EquipmentRecSelectFile.h"
#include "afxdialogex.h"


// CEquipmentRecSelectFile ダイアログ

IMPLEMENT_DYNAMIC(CEquipmentRecSelectFile, CDialogBase)

CEquipmentRecSelectFile::CEquipmentRecSelectFile(CWnd* pParent /*=NULL*/)
: CDialogBase(CEquipmentRecSelectFile::IDD, pParent)
{

}

CEquipmentRecSelectFile::~CEquipmentRecSelectFile()
{
}

void CEquipmentRecSelectFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EQIPREC_SELFILE_LIST, m_FileList);
}


BEGIN_MESSAGE_MAP(CEquipmentRecSelectFile, CDialogBase)
	ON_BN_CLICKED(IDC_EQIPREC_SELFILE_READ_BTN, &CEquipmentRecSelectFile::OnBnClickedEqiprecSelfileReadBtn)
	ON_BN_CLICKED(IDC_EQIPREC_SELFILE_DEL_BTN, &CEquipmentRecSelectFile::OnBnClickedEqiprecSelfileDelBtn)
END_MESSAGE_MAP()


// CEquipmentRecSelectFile メッセージ ハンドラー


/*============================================================================*/
/*! CEquipmentRecSelectFile

-# ダイアログ初期化処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CEquipmentRecSelectFile::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CEquipmentRecSelectFile

-# 『読み込み』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentRecSelectFile::OnBnClickedEqiprecSelfileReadBtn()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


/*============================================================================*/
/*! CEquipmentRecSelectFile

-# 『削除』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentRecSelectFile::OnBnClickedEqiprecSelfileDelBtn()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}
