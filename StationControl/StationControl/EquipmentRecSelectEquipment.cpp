/*============================================================================*/
/*! EquipmentRecSelectEquipment.cpp

-# 設備記録 ダンプ出力対象設備選択画面
*/
/*============================================================================*/
// EquipmentRecSelectEquipment.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "EquipmentRecSelectEquipment.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"


// CEquipmentRecSelectEquipment ダイアログ

IMPLEMENT_DYNAMIC(CEquipmentRecSelectEquipment, CDialogBase)

CEquipmentRecSelectEquipment::CEquipmentRecSelectEquipment(CWnd* pParent /*=NULL*/)
: CDialogBase(CEquipmentRecSelectEquipment::IDD, pParent)
{
	m_pParent = pParent;
}

CEquipmentRecSelectEquipment::~CEquipmentRecSelectEquipment()
{
}

void CEquipmentRecSelectEquipment::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EQIPREC_SELEQIP_LIST, m_EquipList);
}


BEGIN_MESSAGE_MAP(CEquipmentRecSelectEquipment, CDialogBase)
	ON_BN_CLICKED(IDC_EQIPREC_SELEQIP_OUTPUT_BTN, &CEquipmentRecSelectEquipment::OnBnClickedEqiprecSeleqipOutputBtn)
END_MESSAGE_MAP()


// CEquipmentRecSelectEquipment メッセージ ハンドラー


/*============================================================================*/
/*! CEquipmentRecSelectEquipment

-# ダイアログ初期化処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CEquipmentRecSelectEquipment::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	for (int i = 0; i < m_EquipNameList.size(); ++i)
	{
		CString eqip = m_EquipNameList[i];
		m_EquipList.InsertString(-1, eqip);
	}

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

#include "CommBase.h"

/*============================================================================*/
/*! CEquipmentRecSelectEquipment

-# 通算日の取得

@param  なし
@retval なし
*/
/*============================================================================*/
CString CEquipmentRecSelectEquipment::GetTotalDay(CTime time)
{
	CString str = _T("");

	// 現在時刻から求めているので引数時間から求めるように修正
	//tm t;
	//memset(&t, 0, sizeof(t));
	//time.GetGmtTm(&t);
	//str.Format(_T("%d, %02d:%02d:%02d"), t.tm_yday, time.GetHour(), time.GetMinute(), time.GetSecond());

	stServerItemData data;
	memset(&data, 0, sizeof(stServerItemData));
	SYSTEMTIME st;
	time.GetAsSystemTime(st);
	CCommBase::ConvertTime(&st, &data.systime);

	str.Format(_T("%d, %02d:%02d:%02d"), data.systime.wDay, time.GetHour(), time.GetMinute(), time.GetSecond());
	return str;
}


/*============================================================================*/
/*! CEquipmentRecSelectEquipment

-# 監視データ名から設備名を削除

@param  なし
@retval なし
*/
/*============================================================================*/
CString CEquipmentRecSelectEquipment::GetMonName(CString mon)
{
	CString str = _T("");

	int idx = mon.Find(_T("."));
	str = mon.Mid(idx + 1);
	return str;
}


/*============================================================================*/
/*! CEquipmentRecSelectEquipment

-# 出力ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentRecSelectEquipment::OnBnClickedEqiprecSeleqipOutputBtn()
{
	FILE *fp;

	// 選択項目取得
	int nCount = m_EquipList.GetSelCount();
	if (nCount <= 0)
	{
		AfxMessageBox(_T("ダンプ出力対象設備が選択されていません。"), MB_OK | MB_ICONEXCLAMATION);
	}

	CArray<int, int> aryListBoxSel;

	aryListBoxSel.SetSize(nCount);
	m_EquipList.GetSelItems(nCount, aryListBoxSel.GetData());


	CEquipmentRecDumpOutput* pParent = ((CEquipmentRecDumpOutput*)m_pParent);
	map<CString, vector<CString>> &EquipMonNameList = pParent->GetEquipMonNameList();

	// ダンプ開始時刻
	CTime dumpStart(pParent->m_StartTimeD.GetYear(), pParent->m_StartTimeD.GetMonth(), pParent->m_StartTimeD.GetDay(), pParent->m_StartTimeT.GetHour(), pParent->m_StartTimeT.GetMinute(), pParent->m_StartTimeT.GetSecond());
	CString strDumpStart = GetTotalDay(dumpStart);

	// ダンプ終了時刻
	CTime dumpEnd(pParent->m_EndTimeD.GetYear(), pParent->m_EndTimeD.GetMonth(), pParent->m_EndTimeD.GetDay(), pParent->m_EndTimeT.GetHour(), pParent->m_EndTimeT.GetMinute(), pParent->m_EndTimeT.GetSecond());
	CString strDumpEnd = GetTotalDay(dumpEnd);

	CString strEquip;
	CString filename;
	CString fullpath;
	TCHAR szwline[512];
	CStringA cbuf;


	CString strStation = mStationSimpleString[theApp.GetSelectStation()];
	CString ctrDir = mStationCtrlDir[theApp.GetSelectStation()];
	CString defDir = theApp.GetShareFilePath(eFileType_MonDump_Ftp, theApp.GetSelectStation());
	CString dir, pathTmp, ctrFulPath;

#if 1
	dir = defDir;
#else
	while (1)
	{
		dir = _T("");
		theApp.GetShellManager()->BrowseForFolder(dir, this, defDir, _T("出力フォルダを選択してください。"));

		if (dir == _T(""))
		{
			dir = defDir;
		}

		int idx = dir.Find(strStation.MakeLower());

		if (idx == -1)
		{
			CString msg;
			msg.Format(_T("%s以下のフォルダを指定してください。"), (LPCTSTR)strStation);
			AfxMessageBox(msg, MB_OK | MB_ICONEXCLAMATION);
			continue;
		}

		idx += strStation.GetLength();
		pathTmp = dir.Mid(idx);
		pathTmp.Replace(_T('\\'), _T('/'));
		break;
	}
#endif


	for (int i = 0; i < nCount; ++i)
	{
		int idx = aryListBoxSel.GetAt(i);
		m_EquipList.GetText(idx, strEquip);

		filename.Format(_T("mon_dump_%s.txt"), (LPCTSTR)strEquip);
		fullpath.Format(_T("%smon_dump_%s.txt"), (LPCTSTR)dir, (LPCTSTR)strEquip);
		ctrFulPath = ctrDir + pathTmp + filename;

		if ((_wfopen_s(&fp, fullpath, _T("wb")) != 0) || (fp == NULL))
		{
			CString mes;
			mes.Format(_T("ダンプ実行ファイルを開くことができません。\n%s"), (LPCTSTR)fullpath);
			AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
			return;
		}

		// ダンプ対象装置名
		wsprintf(szwline, _T("DEV             %s; %s\n"), (LPCTSTR)strEquip, (LPCTSTR)COMMENT_DEV);
		cbuf = CServerFile::ConvertEuc(szwline);
		fputs(cbuf, fp);

		// ダンプ開始時刻
		wsprintf(szwline, _T("START_TIME      %s; %s\n"), (LPCTSTR)strDumpStart, (LPCTSTR)COMMENTSTART_TIME);
		cbuf = CServerFile::ConvertEuc(szwline);
		fputs(cbuf, fp);

		// ダンプ終了時刻
		wsprintf(szwline, _T("STOP_TIME       %s; %s\n"), (LPCTSTR)strDumpEnd, (LPCTSTR)COMMENTSTOP_TIME);
		cbuf = CServerFile::ConvertEuc(szwline);
		fputs(cbuf, fp);

		vector<CString> &monList = EquipMonNameList[strEquip];
		for (int m = 0; m < monList.size(); ++m)
		{
			CString mon = monList[m];

			// ダンプ対象監視項目
			wsprintf(szwline, _T("WATCH           %s; %s\n"), (LPCTSTR)GetMonName(mon), (LPCTSTR)COMMENTWATCH);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp);
		}
		fclose(fp);

		// -----------------------------------------------------------------
		// シェル呼出

		// 検索精度
		CString seido;
		pParent->m_SearchTimeEdit.GetWindowTextW(seido);

		// ステータス
		int nStatus = pParent->m_StatusOutputCmb.GetCurSel();

		CString arg;
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = MONDUMP_SH;

//		arg.Format(_T("%s %s %s %d"), CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower(), filename, seido, nStatus);
		arg.Format(_T("%s %s %s %d"), (LPCTSTR)CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower(), (LPCTSTR)ctrFulPath, (LPCTSTR)seido, nStatus);
		stParam.strRemShellParam.Format(arg);

		int err = CSshShell::ExecShell(stParam);
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CEquipmentRecSelectEquipment"), _T("MON DUMP"), _T("debug"), strDebug, nLogEx::debug);
		}
	}
}
