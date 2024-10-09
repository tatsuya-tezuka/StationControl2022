/*============================================================================*/
/*! RdnsAgcDetail.cpp

-# 計画レディネスチェック-AGC校正詳細設定画面
*/
/*============================================================================*/
// RdnsAgcDetail.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "RdnsAgcDetail.h"
#include "afxdialogex.h"


// CRdnsAgcDetail ダイアログ

IMPLEMENT_DYNAMIC(CRdnsAgcDetail, CDialogBase)

/*============================================================================*/
/*! CRdnsAgcDetail

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CRdnsAgcDetail::CRdnsAgcDetail(CWnd* pParent /*=NULL*/)
: CDialogBase(CRdnsAgcDetail::IDD, pParent)
, m_nLoopBand(0)
{

}

/*============================================================================*/
/*! CRdnsAgcDetail

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CRdnsAgcDetail::~CRdnsAgcDetail()
{
}

void CRdnsAgcDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RDNS_AGC_1KHZ, m_nLoopBand);
}


BEGIN_MESSAGE_MAP(CRdnsAgcDetail, CDialogBase)
	ON_BN_CLICKED(IDOK, &CRdnsAgcDetail::OnBnClickedOk)
END_MESSAGE_MAP()


// CRdnsAgcDetail メッセージ ハンドラー


/*============================================================================*/
/*! CRdnsAgcDetail

-# ループバンド設定

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsAgcDetail::SetLoopBand(CString strLB)
{
	for (int i = 0; strAGCLoopBand[i] != _T(""); ++i)
	{
		if (strLB == strAGCLoopBand[i])
		{
			m_nLoopBand = i;
			return;
		}
	}
}


/*============================================================================*/
/*! CRdnsAgcDetail

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CRdnsAgcDetail::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CRdnsAgcDetail

-# 設定ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsAgcDetail::OnBnClickedOk()
{
	OnOK();
}
