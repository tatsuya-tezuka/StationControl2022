/*============================================================================*/
/*! SleepTimeInputDlg.cpp

-# 逐次制御登録・実行		指定秒待機設定
*/
/*============================================================================*/
// SleepTimeInputDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "SleepTimeInputDlg.h"
#include "afxdialogex.h"


// CSleepTimeInputDlg ダイアログ

IMPLEMENT_DYNAMIC(CSleepTimeInputDlg, CDialogBase)

/*============================================================================*/
/*! SleepTimeInputDlg

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CSleepTimeInputDlg::CSleepTimeInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSleepTimeInputDlg::IDD, pParent)
{

}

/*============================================================================*/
/*! SleepTimeInputDlg

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CSleepTimeInputDlg::~CSleepTimeInputDlg()
{
}

void CSleepTimeInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEQCTRL_SLEEPTIME, m_str);
	DDX_Control(pDX, IDC_EDIT_SEQCTRL_SLEEPTIME, m_cedit);
}


BEGIN_MESSAGE_MAP(CSleepTimeInputDlg, CDialogBase)
	ON_BN_CLICKED(IDOK, &CSleepTimeInputDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSleepTimeInputDlg メッセージ ハンドラー


/*============================================================================*/
/*! SleepTimeInputDlg

-# 実行ボタン押下時に呼び出される

@param  なし
@retval なし
*/
/*============================================================================*/
void CSleepTimeInputDlg::OnBnClickedOk()
{
	UpdateData();

	if (m_str.IsEmpty())
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("Sleep時間を入力してください"), title, MB_OK);
		return;
	}

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! SleepTimeInputDlg

-# 設定文字列を取得する

@param  なし
@retval 設定文字列
*/
/*============================================================================*/
CString CSleepTimeInputDlg::GetString()
{
	return _T("sleep,") + m_str;
}


/*============================================================================*/
/*! SleepTimeInputDlg

-# 初期化時に呼ばれる

@param  なし
@retval TRUE:成功　/ FALSE:失敗
*/
/*============================================================================*/
BOOL CSleepTimeInputDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_cedit.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
