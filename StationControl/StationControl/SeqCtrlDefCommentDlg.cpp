/*============================================================================*/
/*! SeqCtrlDefCommentDlg.cpp

-# 逐次制御定義ファイル保存コメント入力画面
*/
/*============================================================================*/
// SeqCtrlDefCommentDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "SeqCtrlDefCommentDlg.h"
#include "afxdialogex.h"


// CSeqCtrlDefCommentDlg ダイアログ

IMPLEMENT_DYNAMIC(CSeqCtrlDefCommentDlg, CDialogBase)

/*============================================================================*/
/*! CSeqCtrlDefCommentDlg

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CSeqCtrlDefCommentDlg::CSeqCtrlDefCommentDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSeqCtrlDefCommentDlg::IDD, pParent)
{

}

/*============================================================================*/
/*! CSeqCtrlDefCommentDlg

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CSeqCtrlDefCommentDlg::~CSeqCtrlDefCommentDlg()
{
}

void CSeqCtrlDefCommentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEQCTRLDEFCOMMENT, m_strComment);
}


BEGIN_MESSAGE_MAP(CSeqCtrlDefCommentDlg, CDialogBase)
	ON_BN_CLICKED(IDOK, &CSeqCtrlDefCommentDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSeqCtrlDefCommentDlg メッセージ ハンドラー


/*============================================================================*/
/*! CSeqCtrlDefCommentDlg

-# 実行時に呼び出される

@param  なし
@retval なし
*/
/*============================================================================*/
void CSeqCtrlDefCommentDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	CDialogBase::OnOK();
}
