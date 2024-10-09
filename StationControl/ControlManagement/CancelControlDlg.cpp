/*============================================================================*/
/*! CancelControlDlg.cpp

-# 筑波局運用管制端末制御権利解除
*/
/*============================================================================*/
// CancelControlDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ControlManagement.h"
#include "CancelControlDlg.h"
#include "afxdialogex.h"


// CCancelControlDlg ダイアログ

IMPLEMENT_DYNAMIC(CCancelControlDlg, CDialogEx)

CCancelControlDlg::CCancelControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCancelControlDlg::IDD, pParent)
	, m_nType(0)
{

}

CCancelControlDlg::~CCancelControlDlg()
{
}

void CCancelControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
}


BEGIN_MESSAGE_MAP(CCancelControlDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCancelControlDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCancelControlDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CCancelControlDlg メッセージ ハンドラー


BOOL CCancelControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! 筑波局運用管制端末制御権利解除

-# 制御権利解除を行う

@param
@retval

*/
/*============================================================================*/
void CCancelControlDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	CFileAccess& ac = theApp.GetFileAccess();
	stLocalControl	tdata;
	CString strDefaultPass = mDefaultPassword;
	strDefaultPass = theApp.ManEncodeDecode(strDefaultPass);
	CStringA strPass = CStringA(strDefaultPass);

	if (ac.ReadLocalControl(tdata, eLOCALusc20 + m_nType) == true){
		tdata.flag = 0;
		ac.WriteLocalControl(tdata, eLOCALusc20 + m_nType);
	}
	else{
		// デフォルト設定
		tdata.flag = 0;
		sprintf_s(tdata.lifetime, mMaxLifetime, "0");
		ac.WriteLocalControl(tdata, eLOCALusc20 + m_nType);
	}

	CDialogEx::OnOK();
}


void CCancelControlDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}
