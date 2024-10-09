/*============================================================================*/
/*! EquipmentCtrlSameNum.cpp

-# 設備制御-数値制御画面(２つの制御に同値を設定）
*/
/*============================================================================*/
// EquipmentCtrlSameNum.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "EquipmentCtrlSameNum.h"


// CEquipmentCtrlSameNum ダイアログ

IMPLEMENT_DYNAMIC(CEquipmentCtrlSameNum, CEquipmentCtrlNum)

CEquipmentCtrlSameNum::CEquipmentCtrlSameNum(CWnd* pParent /*=NULL*/)
: CEquipmentCtrlNum(CEquipmentCtrlSameNum::IDD, pParent)
, m_strEquipment2(_T(""))
{

}

CEquipmentCtrlSameNum::~CEquipmentCtrlSameNum()
{
}

void CEquipmentCtrlSameNum::DoDataExchange(CDataExchange* pDX)
{
	CEquipmentCtrlNum::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EQUIPCTRL_NUM_EQIPT_STATICCTRL2, m_strEquipment2);
}


BEGIN_MESSAGE_MAP(CEquipmentCtrlSameNum, CEquipmentCtrlNum)
	ON_BN_CLICKED(IDOK, &CEquipmentCtrlNum::OnBnClickedOk)
END_MESSAGE_MAP()


// CEquipmentCtrlSameNum メッセージ ハンドラー


BOOL CEquipmentCtrlSameNum::OnInitDialog()
{
	CEquipmentCtrlNum::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
