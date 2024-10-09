/*============================================================================*/
/*! IntegTimeDlg.cpp

-# 積分時間設定画面
*/
/*============================================================================*/
// IntegTimeDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "IntgTimeDlg.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"


// CIntgTimeDlg ダイアログ

IMPLEMENT_DYNAMIC(CIntgTimeDlg, CDialogBase)

/*============================================================================*/
/*! CIntgTimeDlg

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CIntgTimeDlg::CIntgTimeDlg(CWnd* pParent /*=NULL*/)
: CDialogBase(CIntgTimeDlg::IDD, pParent)
	, m_nIntgTime(1)
{

}

/*============================================================================*/
/*! CIntgTimeDlg

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CIntgTimeDlg::~CIntgTimeDlg()
{
}

void CIntgTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INTGTIME_EDIT, m_nIntgTime);
	DDV_MinMaxInt(pDX, m_nIntgTime, 1, 300);
	DDX_Control(pDX, IDC_INTGTIME_EDIT, m_cedit);
}


BEGIN_MESSAGE_MAP(CIntgTimeDlg, CDialogBase)
	ON_BN_CLICKED(IDOK, &CIntgTimeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CIntgTimeDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CIntgTimeDlg メッセージ ハンドラー


/*============================================================================*/
/*! CIntgTimeDlg

-# 実行時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CIntgTimeDlg::OnBnClickedOk()
{
	// 積分時間設定をメンバ変数に反映する
	if (UpdateData(TRUE))
	{
		// リモートシェルで局運用管制サーバに通知
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = SEND_NOTICE_SH;
		CString arg;
		int nStation = theApp.GetSelectStation();
		UINT nTermNo = theApp.GetTermNo();
		if (nTermNo > 0)
			nTermNo = nTermNo - 1;

//		arg.Format(_T("%s %d %s%02u,%d,;"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_INTEG, NOTIFY_INTEG_MSG, nTermNo, m_nIntgTime);
		int nAreaNo = theApp.GetClientTerminal();
		arg.Format(_T("%s %d %s%s%02u,%d,;"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_INTEG, NOTIFY_INTEG_MSG, mAreaNameString[nAreaNo], nTermNo, m_nIntgTime);

		stParam.strRemShellParam.Format(arg);

		int err = CSshShell::ExecShell(stParam, FALSE);		// シェル待ちしない
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CIntgTimeDlg"), _T("OnBnClickedOk"), _T("debug"), strDebug, nLogEx::debug);
		}
		if (err == 0)
		{
			// 何もしない
		}

		CDialogBase::OnOK();
	}
}


/*============================================================================*/
/*! CIntgTimeDlg

-# キャンセル時に呼ばれる

@param  なし
@retval なし
*/
/*============================================================================*/
void CIntgTimeDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogBase::OnCancel();
}


/*============================================================================*/
/*! CIntgTimeDlg

-# 初期化時に呼ばれる

@param  なし
@retval TRUE:成功　/ FALSE:失敗
*/
/*============================================================================*/
BOOL CIntgTimeDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_cedit.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
