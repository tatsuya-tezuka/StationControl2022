/*============================================================================*/
/*! PauseMsgInputDlg.cpp

-# 逐次制御登録・実行		一時停止設定
*/
/*============================================================================*/
// PauseMsgInputDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "PauseMsgInputDlg.h"
#include "afxdialogex.h"


// CPauseMsgInputDlg ダイアログ

IMPLEMENT_DYNAMIC(CPauseMsgInputDlg, CDialogBase)

/*============================================================================*/
/*! CPauseMsgInputDlg

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CPauseMsgInputDlg::CPauseMsgInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CPauseMsgInputDlg::IDD, pParent)
{

}

/*============================================================================*/
/*! CPauseMsgInputDlg

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CPauseMsgInputDlg::~CPauseMsgInputDlg()
{
}

void CPauseMsgInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEQCTRL_PAUSEMSG, m_str);
	DDX_Control(pDX, IDC_EDIT_SEQCTRL_PAUSEMSG, m_cedit);
}


BEGIN_MESSAGE_MAP(CPauseMsgInputDlg, CDialogBase)
	ON_BN_CLICKED(IDOK, &CPauseMsgInputDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPauseMsgInputDlg メッセージ ハンドラー


/*============================================================================*/
/*! CPauseMsgInputDlg

-# 実行ボタン押下時に呼び出される

@param  なし
@retval なし
*/
/*============================================================================*/
void CPauseMsgInputDlg::OnBnClickedOk()
{
	UpdateData();

	if (m_str.IsEmpty())
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("示唆メッセージを入力してください"), title, MB_OK);
		return;
	}

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! CPauseMsgInputDlg

-# 設定文字列を取得する

@param  なし
@retval 設定文字列
*/
/*============================================================================*/
CString CPauseMsgInputDlg::GetString()
{
	return _T("pause,") + m_str;
}


/*============================================================================*/
/*! CPauseMsgInputDlg

-# 初期化時に呼ばれる

@param  なし
@retval TRUE:成功　/ FALSE:失敗
*/
/*============================================================================*/
BOOL CPauseMsgInputDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_cedit.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
