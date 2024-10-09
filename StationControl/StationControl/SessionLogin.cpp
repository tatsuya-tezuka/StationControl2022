/*============================================================================*/
/*! SessionLogin.cpp

-# セッション状態画面
*/
/*============================================================================*/
// SessionLogin.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "SessionLogin.h"
#include "afxdialogex.h"


// CSessionLogin ダイアログ

IMPLEMENT_DYNAMIC(CSessionLogin, CDialogBase)

/*============================================================================*/
/*! CSessionLogin

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CSessionLogin::CSessionLogin(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSessionLogin::IDD, pParent)
	, m_passwd(_T(""))
{

}

/*============================================================================*/
/*! CSessionLogin

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CSessionLogin::~CSessionLogin()
{
}

void CSessionLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SESSIONSTATUS_LOGIN_EDIT1, m_passwd);
}


BEGIN_MESSAGE_MAP(CSessionLogin, CDialogBase)
	ON_BN_CLICKED(IDOK, &CSessionLogin::OnBnClickedOk)
END_MESSAGE_MAP()


// CSessionLogin メッセージ ハンドラー


/*============================================================================*/
/*! CSessionLogin

-# OKボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void CSessionLogin::OnBnClickedOk()
{
	// セッション接続する

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! CSessionLogin

-# ダイアログ初期化処理

@param  なし
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
BOOL CSessionLogin::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	TCHAR szTitle[256];
	memset(szTitle, 0, sizeof(szTitle) / sizeof(szTitle[0]));

	switch (m_sessionType)
	{
	case eSessionType_Monitor:
		if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONLOGIN_MONITOR, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
			return FALSE;
		break;
	case eSessionType_Ctrl:
		if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONLOGIN_CTRL, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
			return FALSE;
		break;
	case eSessionType_Notify:
		if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONLOGIN_NOTIFY, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
			return FALSE;
		break;
	case eSessionType_History:
		if (!LoadString(GetModuleHandle(NULL), IDS_SESSIONLOGIN_HISTORY, szTitle, sizeof(szTitle) / sizeof(szTitle[0])))
			return FALSE;
		break;
	default:
		return FALSE;
		break;
	}

	SetWindowText(szTitle);
	m_passwd.Format(_T(""));

	UpdateData(FALSE);

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
