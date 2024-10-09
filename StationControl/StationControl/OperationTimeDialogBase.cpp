// OperationTimeDialogBase.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "OperationTimeDialogBase.h"
#include "afxdialogex.h"

#include "..\Utility\SshShell.h"

// COperationTimeDialogBase ダイアログ

IMPLEMENT_DYNAMIC(COperationTimeDialogBase, CDialogBase)

COperationTimeDialogBase::COperationTimeDialogBase(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialogBase(IDD, pParent)
	, m_strTimeBefore(_T(""))
{
	GetSystemTime(&m_timeBefore);
}

COperationTimeDialogBase::~COperationTimeDialogBase()
{
}

void COperationTimeDialogBase::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COperationTimeDialogBase, CDialogBase)
END_MESSAGE_MAP()


// COperationTimeDialogBase メッセージ ハンドラー


/*============================================================================*/
/*! COperationTimeDialogBase

-# ダイアログ初期化処理

@param  なし
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
BOOL COperationTimeDialogBase::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	CTime ctime = m_timeBefore;
	m_strDateBefore = ctime.Format("%Y-%m-%d");
	m_strTimeBefore = ctime.Format("%H:%M:%S");
	m_dateAfterCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_timeAfterCtrl.SetFormat(_T("HH:mm:ss"));

	m_dateAfterCtrl.SetTime(&m_timeBefore);
	m_timeAfterCtrl.SetTime(&m_timeBefore);

	UpdateData(FALSE);

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! COperationTimeDialogBase

-# OKボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void COperationTimeDialogBase::OnBnClickedOk()
{
	SYSTEMTIME date, time;
	// 年月日を変更する
	if (m_dateAfterCtrl.GetTime(&date) == GDT_VALID && m_timeAfterCtrl.GetTime(&time) == GDT_VALID)
	{
		m_timeAfter.wYear = date.wYear;
		m_timeAfter.wMonth = date.wMonth;
		m_timeAfter.wDayOfWeek = date.wDayOfWeek;
		m_timeAfter.wDay = date.wDay;
		m_timeAfter.wHour = time.wHour;
		m_timeAfter.wMinute = time.wMinute;
		m_timeAfter.wSecond = time.wSecond;
		m_timeAfter.wMilliseconds = time.wMilliseconds;

		CString strTime;
		strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), m_timeAfter.wYear, m_timeAfter.wMonth, m_timeAfter.wDay, m_timeAfter.wHour, m_timeAfter.wMinute, m_timeAfter.wSecond);

		char sz_time[40];
		char sz_asctime[40];
		sprintf_s(sz_time, 40, "%s", (LPCSTR)CStringA(strTime));
		CSatelliteData::ub_asc_to_asctime(sz_time, sz_asctime);
		double tNew = CSatelliteData::ub_asc_to_dtime(sz_asctime);

		if (m_Type == TYPE_START)	// 開始時刻変更の場合
		{
			if (tNew < (double)m_tMin)
			{
				CString strTitle;
				GetWindowText(strTitle);
				MessageBox(_T("開始日時を早めることは出来ません。"), strTitle, MB_OK | MB_ICONWARNING);
				return;
			}
			if (tNew > (double)m_tMax)
			{
				CString strTitle;
				GetWindowText(strTitle);
				MessageBox(_T("開始日時を終了日時より後には出来ません。"), strTitle, MB_OK | MB_ICONWARNING);
				return;
			}
		}
		else if (m_Type == TYPE_END)	// 終了時刻変更の場合
		{
			if (tNew < (double)m_tMin)
			{
				CString strTitle;
				GetWindowText(strTitle);
				MessageBox(_T("終了日時を開始日時より早めることは出来ません。"), strTitle, MB_OK | MB_ICONWARNING);
				return;
			}
		}

		// 計画変更要求
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = SEND_NOTICE_SH;
		CString arg;

		int nStation = theApp.GetSelectStation();
		arg.Format(_T("%s %d %d,%s"),
			(LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(),	// 局
			INFO_ID_REQ_PLN_MO,												// 計画変更要求
			m_Type,															// 開始時刻変更：1 or 終了時刻変更：2
			(LPCTSTR)strTime);												// 時刻

		stParam.strRemShellParam.Format(arg);

		int err = CSshShell::ExecShell(stParam);		// シェル待ちする
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnMenuResend"), _T("debug"), strDebug, nLogEx::debug);
		}

		if (err != 0) // 異常終了
		{
		}
		else // 正常終了
		{
			// 計画更新通知を送る
			UpdatePlanData(nStation, strTime);
		}
	}

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! COperationTimeDialogBase

-# 計画ファイルのAUTO OFF処理

@param	cbuf	計画ファイルの文字列
@retval TRUE:AUTOをOFFに置き換えた / FALSE:AUTOを置き換えてない
*/
/*============================================================================*/
BOOL COperationTimeDialogBase::UpdatePlanDataAutoOff(CString& cbuf)
{
	// 「AUTO=」を探す
	const CString strSearchString = _T("AUTO=");
	if (cbuf.Find(strSearchString) >= 0){
		// 「AUTO=」がみつかったのでOFFにするため、「AUTO=」より右側を消す
		CString str[20];
		int count = 0;
		int pos = 0;
		int i = 0;
		while (AfxExtractSubString(str[i], cbuf, i, ' ')){
			str[i].TrimLeft();
			str[i].TrimRight();
			if (str[i].Find(strSearchString) >= 0){
				pos = i;
			}
			i++;
		}
		count = i;
		for (i = 0; i < count; i++){
			if (i == 0){
				cbuf = str[i];
			}
			else{
				cbuf += _T(" ");
				if (i == pos){
					cbuf += strSearchString;
				}
				else{
					cbuf += str[i];
				}
			}
		}
		cbuf += _T("\n");
		return TRUE;
	}

	return FALSE;
}
