/*============================================================================*/
/*! CmdInputDlg.cpp

-# 逐次制御登録・実行		コマンド設定
*/
/*============================================================================*/
// CmdInputDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "CmdInputDlg.h"
#include "afxdialogex.h"


// CCmdInputDlg ダイアログ

IMPLEMENT_DYNAMIC(CCmdInputDlg, CDialogBase)

/*============================================================================*/
/*! CmdInputDlg

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CCmdInputDlg::CCmdInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CCmdInputDlg::IDD, pParent)
{

}

/*============================================================================*/
/*! CCmdInputDlg

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CCmdInputDlg::~CCmdInputDlg()
{
}

void CCmdInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEQCTRL_CMD, m_str);
	DDX_Control(pDX, IDC_EDIT_SEQCTRL_CMD, m_cedit);
}


BEGIN_MESSAGE_MAP(CCmdInputDlg, CDialogBase)
	ON_BN_CLICKED(IDOK, &CCmdInputDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCmdInputDlg メッセージ ハンドラー


/*============================================================================*/
/*! CCmdInputDlg

-# 実行ボタン押下時に呼び出される

@param  なし
@retval なし
*/
/*============================================================================*/
void CCmdInputDlg::OnBnClickedOk()
{
	UpdateData();

	if (m_str.IsEmpty())
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("コマンドを入力してください"), title, MB_OK);
		return;
	}

	CDialogBase::OnOK();
}

/*============================================================================*/
/*! CCmdInputDlg

-# 設定文字列を取得する

@param  なし
@retval 設定文字列
*/
/*============================================================================*/
CString CCmdInputDlg::GetString()
{
	return _T("command,") + m_str;
}


/*============================================================================*/
/*! CCmdInputDlg

-# 初期化時に呼ばれる

@param  なし
@retval TRUE:成功　/ FALSE:失敗
*/
/*============================================================================*/
BOOL CCmdInputDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_cedit.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
