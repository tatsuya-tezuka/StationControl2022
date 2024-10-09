/*============================================================================*/
/*! FrequencyCtrlPre54.cpp

-# 周波数制御画面-一括設定タブ(54m)
*/
/*============================================================================*/
// FrequencyCtrlPre54.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlPre54.h"
#include "afxdialogex.h"


// CFrequencyCtrlPre54 ダイアログ

IMPLEMENT_DYNAMIC(CFrequencyCtrlPre54, CFrequencyCtrlPre)

CFrequencyCtrlPre54::CFrequencyCtrlPre54(CWnd* pParent /*=NULL*/)
: CFrequencyCtrlPre(CFrequencyCtrlPre54::IDD, pParent)
{

}

CFrequencyCtrlPre54::~CFrequencyCtrlPre54()
{
}

void CFrequencyCtrlPre54::DoDataExchange(CDataExchange* pDX)
{
	CFrequencyCtrlPre::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SENDOP, m_strSendOp);
	DDX_Control(pDX, IDC_COMBO_SENDGET, m_cbSendGet);
	DDX_CBIndex(pDX, IDC_COMBO_SENDGET, m_nSendGet);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrlPre54, CFrequencyCtrlPre)
END_MESSAGE_MAP()


// CFrequencyCtrlPre54 メッセージ ハンドラー
