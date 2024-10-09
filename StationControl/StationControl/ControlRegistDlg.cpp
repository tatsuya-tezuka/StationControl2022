/*============================================================================*/
/*! CControlRegistDlg.cpp

-# 逐次制御登録・実行		制御登録
*/
/*============================================================================*/
// ControlRegistDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ControlRegistDlg.h"
#include "afxdialogex.h"
#include "CtrlSearchDlg.h"

// CControlRegistDlg ダイアログ

IMPLEMENT_DYNAMIC(CControlRegistDlg, CDialogBase)

/*============================================================================*/
/*! CControlRegistDlg

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CControlRegistDlg::CControlRegistDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CControlRegistDlg::IDD, pParent)
	, m_strCtrl(_T(""))
	, m_strVal(_T(""))
	, m_strValDisp(_T(""))
{

}

/*============================================================================*/
/*! CControlRegistDlg

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CControlRegistDlg::~CControlRegistDlg()
{
}

void CControlRegistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CTRLREGIST_CTRLNAME, m_strCtrl);
	DDX_Text(pDX, IDC_STATIC_CTRLREGIST_CTRLVAL, m_strValDisp);
	DDX_Control(pDX, IDC_EDIT_CTRLREGIST_CTRLNAME, m_cedit);
}


BEGIN_MESSAGE_MAP(CControlRegistDlg, CDialogBase)
	ON_BN_CLICKED(IDC_BUTTON_CTRLREGIST_SEARCH, &CControlRegistDlg::OnBnClickedButtonCtrlregistSearch)
	ON_BN_CLICKED(IDOK, &CControlRegistDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CControlRegistDlg メッセージ ハンドラー


/*============================================================================*/
/*! CControlRegistDlg

-# 制御名を検索する

@param  なし
@retval なし
*/
/*============================================================================*/
void CControlRegistDlg::OnBnClickedButtonCtrlregistSearch()
{
	UpdateData();
	CCtrlSearchDlg dlg;
	dlg.SetCtrlName(m_strCtrl);
	dlg.SetValName(m_strVal);
	if (dlg.DoModal() == IDOK)
	{
		m_strCtrl = dlg.GetCtrlNameSearch();
		m_strVal = dlg.GetCtrlValeSearch();
		if (m_strVal.IsEmpty())
		{
			m_strValDisp = _T("-");
		}
		else
		{
			m_strValDisp = m_strVal;
		}
		UpdateData(FALSE);
	}
}


/*============================================================================*/
/*! CControlRegistDlg

-# 制御登録を実行する

@param  なし
@retval なし
*/
/*============================================================================*/
void CControlRegistDlg::OnBnClickedOk()
{
	UpdateData();

	if (m_strCtrl.IsEmpty())
	{
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("制御名を入力してください"), title, MB_OK);
		return;
	}

//	if (m_strVal.IsEmpty())
//	{
//		CString title = _T("");
//		GetWindowText(title);
//		MessageBox(_T("ステータスを選択してください"), title, MB_OK);
//		return;
//	}

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! CControlRegistDlg

-# 設定文字列を取得する

@param  なし
@retval 設定文字列
*/
/*============================================================================*/
CString CControlRegistDlg::GetString()
{
	return m_strCtrl + _T(",") + m_strVal;
}


/*============================================================================*/
/*! CControlRegistDlg

-# 初期化時に呼ばれる

@param  なし
@retval TRUE:成功　/ FALSE:失敗
*/
/*============================================================================*/
BOOL CControlRegistDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	if (m_strVal.IsEmpty())
	{
		m_strValDisp = _T("-");
	}
	else
	{
		m_strValDisp = m_strVal;
	}

	UpdateData(FALSE);

	m_cedit.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
