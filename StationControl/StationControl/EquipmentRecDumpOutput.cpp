/*============================================================================*/
/*! EquipmentRecDumpOutput.cpp

-# 設備記録 監視データダンプ画面
*/
/*============================================================================*/
// EquipmentRecDumpOutput.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "EquipmentRecDumpOutput.h"
#include "afxdialogex.h"
#include "MonSearchDlg.h"
#include "HookMsgBox.h"

// CEquipmentRecDumpOutput ダイアログ

IMPLEMENT_DYNAMIC(CEquipmentRecDumpOutput, CDialogBase)

CEquipmentRecDumpOutput::CEquipmentRecDumpOutput(CWnd* pParent /*=NULL*/)
: CDialogBase(CEquipmentRecDumpOutput::IDD, pParent)
	, m_strFileId(_T(""))
	, m_StartTimeD(0)
	, m_StartTimeT(0)
	, m_EndTimeD(0)
	, m_EndTimeT(0)
	, m_strDirectEntry(_T(""))
{

}

CEquipmentRecDumpOutput::~CEquipmentRecDumpOutput()
{
}

void CEquipmentRecDumpOutput::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EQIPREC_DUMPOUTPUT_EQIPSEL_CMB, m_EquipmentSelCmb);
	DDX_Text(pDX, IDC_EQIPREC_DUMPOUTPUT_FID_EDIT, m_strFileId);
	DDX_DateTimeCtrl(pDX, IDC_EQIPREC_DUMPOUTPUT_OSTIME_DAY_DP, m_StartTimeD);
	DDX_DateTimeCtrl(pDX, IDC_EQIPREC_DUMPOUTPUT_OSTIME_TIME_DP, m_StartTimeT);
	DDX_DateTimeCtrl(pDX, IDC_EQIPREC_DUMPOUTPUT_OETIME_DAY_DP, m_EndTimeD);
	DDX_DateTimeCtrl(pDX, IDC_EQIPREC_DUMPOUTPUT_OETIME_TIME_DP, m_EndTimeT);
	DDX_Control(pDX, IDC_EQIPREC_DUMPOUTPUT_SEARCH_EDIT, m_SearchTimeEdit);
	DDX_Control(pDX, IDC_EQIPREC_DUMPOUTPUT_STATUS_CMB, m_StatusOutputCmb);
	DDX_Control(pDX, IDC_EQIPREC_DUMPOUTPUT_MNTD_LIST, m_MonitoringDataList);
	DDX_Text(pDX, IDC_EQIPREC_DUMPOUTPUT_DIRECT_EDIT, m_strDirectEntry);
	DDX_Control(pDX, IDC_EQIPREC_DUMPOUTPUT_OSTIME_DAY_DP, m_StartTimeDTCtrl);
	DDX_Control(pDX, IDC_EQIPREC_DUMPOUTPUT_OETIME_DAY_DP, m_EndTimeDTCtrl);
	DDX_Control(pDX, IDC_EQIPREC_DUMPOUTPUT_FID_SAVE_BTN, m_SaveBtnCtrl);
	DDX_Control(pDX, IDC_EQIPREC_DUMPOUTPUT_OSTIME_TIME_DP, m_StartTimeTTCtrl);
	DDX_Control(pDX, IDC_EQIPREC_DUMPOUTPUT_OETIME_TIME_DP, m_EndTimeTTCtrl);
}


BEGIN_MESSAGE_MAP(CEquipmentRecDumpOutput, CDialogBase)
	ON_BN_CLICKED(IDC_EQIPREC_DUMPOUTPUT_FID_SAVE_BTN, &CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputFidSaveBtn)
	ON_BN_CLICKED(IDC_EQIPREC_DUMPOUTPUT_FID_READ_BTN, &CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputFidReadBtn)
	ON_BN_CLICKED(IDC_EQIPREC_DUMPOUTPUT_MNTD_UP_BTN, &CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputMntdUpBtn)
	ON_BN_CLICKED(IDC_EQIPREC_DUMPOUTPUT_MNTD_DWN_BTN, &CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputMntdDwnBtn)
	ON_BN_CLICKED(IDC_EQIPREC_DUMPOUTPUT_MNTD_DEL_BTN, &CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputMntdDelBtn)
	ON_BN_CLICKED(IDC_EQIPREC_DUMPOUTPUT_DIRECT_ADD_BTN, &CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputDirectAddBtn)
	ON_BN_CLICKED(IDC_EQIPREC_DUMPOUTPUT_EXEC_BTN, &CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputExecBtn)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CEquipmentRecDumpOutput::OnMenuCloseClose)
	ON_BN_CLICKED(IDC_EQIPREC_DUMPOUTPUT_DIRECT_SERCH_BTN, &CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputDirectSerchBtn)
	ON_CBN_SELCHANGE(IDC_EQIPREC_DUMPOUTPUT_EQIPSEL_CMB, &CEquipmentRecDumpOutput::OnCbnSelchangeEqiprecDumpoutputEqipselCmb)
	ON_EN_CHANGE(IDC_EQIPREC_DUMPOUTPUT_FID_EDIT, &CEquipmentRecDumpOutput::OnEnChangeEqiprecDumpoutputFidEdit)
END_MESSAGE_MAP()


// CEquipmentRecDumpOutput メッセージ ハンドラー


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# ダイアログ初期化処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CEquipmentRecDumpOutput::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# ダイアログ更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CEquipmentRecDumpOutput::UpdateDlg()
{
	// 現在時刻取得
	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	CTime time = CTime(timeSys);


	// 出力開始時刻
	m_StartTimeDTCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_StartTimeTTCtrl.SetFormat(_T("HH:mm:ss"));
	CTimeSpan    cTimeSpan(0, 1, 0, 0);

	m_StartTimeD = time - cTimeSpan;
	m_StartTimeT = time - cTimeSpan;

	// 出力終了時刻
	m_EndTimeDTCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_EndTimeTTCtrl.SetFormat(_T("HH:mm:ss"));
	m_EndTimeD = time;
	m_EndTimeT = time;

	// 検索精度
	m_SearchTimeEdit.SetWindowTextW(_T("0"));

	// ステータス出力方法
	m_StatusOutputCmb.ResetContent();
	m_StatusOutputCmb.InsertString(-1, _T("文字"));
	m_StatusOutputCmb.InsertString(-1, _T("数値"));
	m_StatusOutputCmb.SetCurSel(0);

	// 保存ボタンDisable
	m_SaveBtnCtrl.EnableWindow(FALSE);

	// 設備選択
	m_EquipmentSelCmb.ResetContent();

	// ファイル識別
	m_strFileId = _T("");

	// 監視データ
	m_MonitoringDataList.ResetContent();

	// データリスト
	m_EquipMonNameList.clear();

	UpdateData(FALSE);

	OnBnClickedEqiprecDumpoutputFidReadBtn();

	return TRUE;
}

/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# 『保存』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputFidSaveBtn()
{
	UpdateData();

	FILE	*fp;
	TCHAR	sz_buff[512];
	CStringA cbuf;

	CString folder = theApp.GetShareFilePath(eFileType_MonDump_Def, theApp.GetSelectStation());

	CString		filter("ダンプ定義ファイル (*.txt)|*.txt||");
	CFileDialog	dlg(FALSE, NULL, m_strFileId, OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, filter);
	dlg.m_ofn.lpstrTitle = _T("ダンプ定義ファイル選択");
	dlg.m_ofn.lpstrInitialDir = folder;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	CString file = dlg.GetPathName();
	if (file.Find(_T(".txt")) == -1)
	{
		file = file + _T(".txt");
	}

	if ((_wfopen_s(&fp, file, _T("wb")) != 0) ||( fp == NULL))
	{
		CString mes;
		mes.Format(_T("ダンプ定義ファイルを開くことができません。\n%s"), (LPCTSTR)file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	CString str;
	for (int i = 0; i < m_EquipmentSelCmb.GetCount(); ++i)
	{
		m_EquipmentSelCmb.GetLBText(i, str);
		vector<CString> &monlist = m_EquipMonNameList[str];
		for (int i = 0; i < monlist.size(); ++i)
		{
			CString mon = monlist[i];
			wsprintf(sz_buff, _T("%s\n"), (LPCTSTR)mon);
			cbuf = CServerFile::ConvertEuc(sz_buff);
			fputs(cbuf, fp);
		}
	}
	fclose(fp);
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# 『読込』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputFidReadBtn()
{
	FILE	*fp;

	CString folder = theApp.GetShareFilePath(eFileType_MonDump_Def, theApp.GetSelectStation());

	CString	filter("ダンプ定義ファイル (*.txt)|*.txt||");
	CFileDialog	dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR, filter);
	dlg.m_ofn.lpstrTitle = _T("ダンプ定義ファイル選択");
	dlg.m_ofn.lpstrInitialDir = folder;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	CString file = dlg.GetPathName();

	if ((_wfopen_s(&fp, file, _T("r")) != 0) || (fp == NULL))
	{
		CString mes;
		mes.Format(_T("ダンプ定義ファイルを開くことができません。\n%s"), (LPCTSTR)file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return ;
	}

	m_EquipmentSelCmb.ResetContent();
	m_MonitoringDataList.ResetContent();
	m_EquipMonNameList.clear();

	char sz_line[512];
	CString cbuf;
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp) != NULL)
	{
		cbuf = CServerFile::ConvertUnicode(sz_line);
		cbuf = cbuf.Mid(0, cbuf.GetLength() - 1);	// 改行コード削除
		SetDlgData(CString(cbuf));
	}
	fclose(fp);


	// 設備選択更新
	m_EquipmentSelCmb.SetCurSel(0);
	
	// 監視データ更新
	CString equip;
	m_EquipmentSelCmb.GetLBText(0, equip);
	ResetList(equip);

	// ファイル識別更新
	m_strFileId = dlg.GetFileTitle();
	m_SaveBtnCtrl.EnableWindow(TRUE);

	UpdateData(FALSE);
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# 監視データリスト再設定

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::ResetMonMapList()
{
	CString str;
	CString equip;
	m_EquipmentSelCmb.GetWindowTextW(equip);

	vector<CString> monlist;
	int idx = m_MonitoringDataList.GetCount();
	if (idx == 0)
	{
		m_EquipMonNameList.erase(equip);
		m_EquipmentSelCmb.DeleteString(m_EquipmentSelCmb.FindStringExact(-1, equip));
	}
	else
	{
		for (int i = 0; i < idx; ++i)
		{
			m_MonitoringDataList.GetText(i, str);
			monlist.push_back(str);
		}
		m_EquipMonNameList[equip].swap(monlist);
	}
}
	
/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# 『1行上へ』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputMntdUpBtn()
{
	int nIndex = m_MonitoringDataList.GetCurSel();
	if (nIndex == -1 || nIndex == 0)
		return;

	CString str;
	m_MonitoringDataList.GetText(nIndex, str);
	m_MonitoringDataList.DeleteString(nIndex);
	nIndex = m_MonitoringDataList.InsertString(nIndex - 1, str);
	m_MonitoringDataList.SetCurSel(nIndex);

	ResetMonMapList();
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# 『1行下へ』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputMntdDwnBtn()
{
	int nIndex = m_MonitoringDataList.GetCurSel();
	if (nIndex == m_MonitoringDataList.GetCount() - 1)
		return;

	CString str;
	m_MonitoringDataList.GetText(nIndex, str);
	m_MonitoringDataList.DeleteString(nIndex);
	nIndex = m_MonitoringDataList.InsertString(nIndex + 1, str);
	m_MonitoringDataList.SetCurSel(nIndex);

	ResetMonMapList();
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# 『1行削除』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputMntdDelBtn()
{
	int nIndex = m_MonitoringDataList.GetCurSel();
	m_MonitoringDataList.DeleteString(nIndex);

	ResetMonMapList();
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# リスト更新

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::SetDlgData(CString mon)
{
	CString equip = _T("");
	int idx = mon.Find(_T("."));
	equip = mon.Mid(0, idx);
	if (m_EquipmentSelCmb.FindStringExact(-1, equip) == CB_ERR)
	{
		m_EquipmentSelCmb.InsertString(-1, equip);
	}
	m_EquipmentSelCmb.SelectStringEx(-1, equip);

	// 監視リストに追加
	if (m_EquipMonNameList.count(equip) == 0)
	{
		vector<CString> monlist;
		monlist.push_back(mon);
		m_EquipMonNameList[equip] = monlist;
	}
	else
	{
		BOOL bFind = FALSE;
		vector<CString> &monlist = m_EquipMonNameList[equip];
		for (int i = 0; i < monlist.size(); ++i)
		{
			if (monlist[i] == mon)
			{
				AfxMessageBox(_T("追加しようとした監視名は登録済みです。"), MB_OK | MB_ICONWARNING);
				bFind = TRUE;
				break;
			}
		}

		if (bFind == FALSE)
		{
			monlist.push_back(mon);
		}
	}
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# 『追加』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputDirectAddBtn()
{
	UpdateData();

	// 入力された監視データがDB無い場合は、メッセージ出力
	CSatelliteData& sd = theApp.GetSatelliteData();
	CDBAccess& dba = sd.GetDBAccessCls();

	BOOL bFind = FALSE;
	vector<CString>& mList = dba.GetMonNameList();
	int cnt = (int)mList.size();
	for (int i = 0; i < cnt; ++i)
	{
		if (m_strDirectEntry == mList[i])
		{
			bFind = TRUE;
			break;
		}
	}

	if (bFind == FALSE)
	{
		AfxMessageBox(_T("入力された監視名が不正です。"), MB_OK | MB_ICONWARNING);
		return;
	}

	// 設備選択コンボボックスに登録
	SetDlgData(m_strDirectEntry);

	CString equip = _T("");
	int idx = m_strDirectEntry.Find(_T("."));
	equip = m_strDirectEntry.Mid(0, idx);

	ResetList(equip);

	m_strDirectEntry = _T("");
	UpdateData(FALSE);
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# 『実行』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputExecBtn()
{
	UpdateData(TRUE);

	// 時刻チェック
	// 出力開始時刻
	CTime dumpStart(m_StartTimeD.GetYear(), m_StartTimeD.GetMonth(), m_StartTimeD.GetDay(), m_StartTimeT.GetHour(), m_StartTimeT.GetMinute(), m_StartTimeT.GetSecond());
	// 出力終了時刻
	CTime dumpEnd(m_EndTimeD.GetYear(), m_EndTimeD.GetMonth(), m_EndTimeD.GetDay(), m_EndTimeT.GetHour(), m_EndTimeT.GetMinute(), m_EndTimeT.GetSecond());
	if (dumpStart.GetTime() > dumpEnd.GetTime())
	{
		CString mes = _T("");
		CString strStartTime = dumpStart.Format(_T("%Y-%m-%d %H:%M:%S"));
		CString strEndTime = dumpEnd.Format(_T("%Y-%m-%d %H:%M:%S"));
		mes.Format(_T("出力終了時刻が出力開始時刻よりも過去のため、監視ダンプ出力できません。\n\n 出力開始時刻 :%s\n 出力終了時刻 :%s"), (LPCTSTR)strStartTime, (LPCTSTR)strEndTime);
		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
		return;
	}

	vector<CString> list;
	CString str;
	for (int i = 0; i < m_EquipmentSelCmb.GetCount(); ++i)
	{
		m_EquipmentSelCmb.GetLBText(i, str);
		list.push_back(str);
	}

	CEquipmentRecSelectEquipment dlg(this);
	dlg.m_EquipNameList.clear();
	dlg.m_EquipMonNameList.clear();
	dlg.m_EquipNameList = list;
	dlg.m_EquipMonNameList = m_EquipMonNameList;
	dlg.DoModal();
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# ファイル-閉じるメニュー処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnMenuCloseClose()
{
	//	OnOK();
	ShowWindow(SW_HIDE);
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# 『検索』ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputDirectSerchBtn()
{
	UpdateData();

	CMonSearchDlg dlg;
	dlg.m_strMonName = m_strDirectEntry;
	if (IDOK == dlg.DoModal())
	{
		m_strDirectEntry = dlg.m_MonNameSearch;
		UpdateData(FALSE);
	}
}

/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# リスト更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::ResetList(CString name)
{
//	m_strDirectEntry

	m_MonitoringDataList.SetRedraw(FALSE);
	m_MonitoringDataList.ResetContent();

#if 1
	vector<CString> &monlist = m_EquipMonNameList[name];
	for(int i = 0; i < monlist.size(); ++i)
	{
		m_MonitoringDataList.InsertString(-1, monlist[i]);
	}
	m_MonitoringDataList.SetRedraw(TRUE);

#else
	int cnt = (int)m_EquipNameListTemp.size();
	CString str;
	name.MakeLower();
	for (int i = 0; i < cnt; ++i)
	{
		if (name.IsEmpty() == false)
		{
			str = m_EquipNameListTemp[i];
			str.MakeLower();
			if (str.Find(name) == 0)
			{
				m_MonitoringDataList.InsertString(-1, m_EquipNameListTemp[i]);
			}
		}
		else
		{
			m_MonitoringDataList.InsertString(-1, m_EquipNameListTemp[i]);
		}
	}
	m_MonitoringDataList.SetRedraw(TRUE);
#endif
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# 設備選択コンボボックス変更処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnCbnSelchangeEqiprecDumpoutputEqipselCmb()
{
	CString str;

	int idx = m_EquipmentSelCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_EquipmentSelCmb.GetLBText(idx, str);
	ResetList(str);
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# ファイル識別 入力処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnEnChangeEqiprecDumpoutputFidEdit()
{
	UpdateData();

	if (m_strFileId.GetLength() != 0)
	{
		m_SaveBtnCtrl.EnableWindow(TRUE);
	}
	else
	{
		m_SaveBtnCtrl.EnableWindow(FALSE);
	}

	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogBase::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
}
