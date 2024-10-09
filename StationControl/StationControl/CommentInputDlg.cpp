/*============================================================================*/
/*! CommentInputDlg.cpp

-# 逐次制御登録・実行		指定秒待機設定
*/
/*============================================================================*/
// CommentInputDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "CommentInputDlg.h"
#include "afxdialogex.h"


// CCommentInputDlg ダイアログ

IMPLEMENT_DYNAMIC(CCommentInputDlg, CDialogBase)

/*============================================================================*/
/*! CCommentInputDlg

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CCommentInputDlg::CCommentInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CCommentInputDlg::IDD, pParent)
{

}

/*============================================================================*/
/*! CCommentInputDlg

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CCommentInputDlg::~CCommentInputDlg()
{
}

void CCommentInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEQCTRL_COMMENT, m_str);
	DDX_Control(pDX, IDC_EDIT_SEQCTRL_COMMENT, m_cedit);
}


BEGIN_MESSAGE_MAP(CCommentInputDlg, CDialogBase)
	ON_BN_CLICKED(IDOK, &CCommentInputDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCommentInputDlg メッセージ ハンドラー


/*============================================================================*/
/*! CCommentInputDlg

-# 実行ボタン押下時に呼び出される

@param  なし
@retval なし
*/
/*============================================================================*/
void CCommentInputDlg::OnBnClickedOk()
{
	UpdateData();

	if (m_str.IsEmpty())
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("コメントを入力してください"), title, MB_OK);
		return;
	}

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! CCommentInputDlg

-# 設定文字列を取得する

@param  なし
@retval 設定文字列
*/
/*============================================================================*/
CString CCommentInputDlg::GetString()
{
	return _T("#,") + m_str;
}


/*============================================================================*/
/*! CCommentInputDlg

-# 初期化時に呼ばれる

@param  なし
@retval TRUE:成功　/ FALSE:失敗
*/
/*============================================================================*/
BOOL CCommentInputDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_cedit.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
