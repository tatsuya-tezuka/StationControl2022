/*============================================================================*/
/*! TblSatelliteSelection.cpp

-# 衛星選択画面共通

-#  AGC校正テーブル読込・衛星選択画面
-#  AGC校正テーブルファイルコピー・衛星選択画面
-#  AGC校正テーブルファイル復元・衛星選択画面
-#  局内ディレイ値テーブルファイルコピー・衛星選択画面
-#  局内ディレイ値テーブルファイル復元・衛星選択画面
*/
/*============================================================================*/
// TblSatelliteSelection.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TblSatelliteSelection.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"


// CTblSatelliteSelection ダイアログ

IMPLEMENT_DYNAMIC(CTblSatelliteSelection, CDialogEx)

CTblSatelliteSelection::CTblSatelliteSelection(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTblSatelliteSelection::IDD, pParent)
{
}

CTblSatelliteSelection::~CTblSatelliteSelection()
{
}

void CTblSatelliteSelection::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SATELLITE_CMB, m_SatCmb);
}


BEGIN_MESSAGE_MAP(CTblSatelliteSelection, CDialogEx)
	ON_BN_CLICKED(IDC_TBL_SATSEL_BTN, &CTblSatelliteSelection::OnBnClickedTblSatselBtn)
END_MESSAGE_MAP()


// CTblSatelliteSelection メッセージ ハンドラー


/*============================================================================*/
/*! CTblSatelliteSelection

-# ダイアログ初期化処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CTblSatelliteSelection::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 衛星選択
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	vector<CString>			&saNameList = dba.GetSatNameList();
	int cnt = (int)saNameList.size();
	for (int i = 0; i < cnt; i++)
	{
		/*--------------------------*/
		/* 衛星本体情報アドレス取得 */
		CString satname = saNameList[i];

		if (satname == _T("DEFAULT") || satname == _T("ＤＥＦＡＵＬＴ"))
		{
			continue;
		}
		m_SatCmb.InsertString(-1, satname);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! CTblSatelliteSelection

-# 『実行』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTblSatelliteSelection::OnBnClickedTblSatselBtn()
{
	CString sat;
	int idx = m_SatCmb.GetCurSel();

	if (idx == CB_ERR)
	{
		return;
	}

	m_SatCmb.GetLBText(idx, sat);

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = OPTION_AGCTBL_LOAD_SH;

	if (theApp.GetSelectStation() == eStation_Usuda64)
		stParam.strRemShellParam.Format(_T("udsc64 %s"), (LPCTSTR)sat);
	else if (theApp.GetSelectStation() == eStation_Usuda642)
		stParam.strRemShellParam.Format(_T("udsc54 %s"), (LPCTSTR)sat);
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
		stParam.strRemShellParam.Format(_T("usc34 %s"), (LPCTSTR)sat);
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
		stParam.strRemShellParam.Format(_T("usc20 %s"), (LPCTSTR)sat);

	CString msg;
	msg.Format(_T("GetCalib(%s)!!!"), (LPCTSTR)stParam.strRemShellName);
	CLogTraceEx::Write(_T(""), _T("CTblSatelliteSelection"), _T("GetCalib"), msg, _T(""), nLogEx::debug);

	int err = CSshShell::ExecShell(stParam);

	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CTblSatelliteSelection"), _T("GetCalib"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err == 0)
	{
		AfxMessageBox(_T("AGC校正テーブルファイルの読み込みに成功しました。"), MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		AfxMessageBox(_T("AGC校正テーブルファイルの読み込みに失敗しました。"), MB_OK | MB_ICONWARNING);
	}

	CDialogEx::OnOK();
}
