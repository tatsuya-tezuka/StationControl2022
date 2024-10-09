/*============================================================================*/
/*! RdnsAgcDetailStep.cpp

-# 計画レディネスチェック-AGC校正ステップ数/積分時間設定画面
*/
/*============================================================================*/
// RdnsAgcDetailStep.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "RdnsAgcDetailStep.h"
#include "afxdialogex.h"

// CRdnsAgcDetailStep ダイアログ

IMPLEMENT_DYNAMIC(CRdnsAgcDetailStep, CDialogBase)

/*============================================================================*/
/*! CRdnsAgcDetailStep

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CRdnsAgcDetailStep::CRdnsAgcDetailStep(CWnd* pParent /*=NULL*/)
: CDialogBase(CRdnsAgcDetailStep::IDD, pParent)
{
}

/*============================================================================*/
/*! CRdnsAgcDetailStep

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CRdnsAgcDetailStep::~CRdnsAgcDetailStep()
{
}

void CRdnsAgcDetailStep::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RDNS_AGC_STEP_LIST, m_IntgListCtrl);
	DDX_Control(pDX, IDC_RDNS_AGC_STEP_INTG_EDIT, m_IntegEdit);
	DDX_Control(pDX, IDC_RDNS_AGC_STEP_PRF_CMB, m_CalibCmb);
}


BEGIN_MESSAGE_MAP(CRdnsAgcDetailStep, CDialogBase)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_RDNS_AGC_STEP_INTG_BTN, &CRdnsAgcDetailStep::OnBnClickedRdnsAgcStepIntgBtn)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_RDNS_AGC_STEP_PRF_CMB, &CRdnsAgcDetailStep::OnCbnSelchangeRdnsAgcStepPrfCmb)
	ON_BN_CLICKED(IDOK, &CRdnsAgcDetailStep::OnBnClickedOk)
END_MESSAGE_MAP()


// CRdnsAgcDetailStep メッセージ ハンドラー


int CRdnsAgcDetailStep::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


/*============================================================================*/
/*! CRdnsAgcDetailStep

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CRdnsAgcDetailStep::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// AGC校正テーブルファイル読み込み
	m_AgcTblDfltMap.clear();
	if (ReadAgcTblFile(TYPE_DFLT, m_AgcTblDfltMap) == FALSE)
	{
		EndDialog(IDCANCEL);
		return TRUE;
	}

	m_AgcTblCustMap.clear();
	if (ReadAgcTblFile(TYPE_CUST, m_AgcTblCustMap) == FALSE)
	{
		EndDialog(IDCANCEL);
		return TRUE;
	}

	// 校正値取得
	for (int i = 0; strStepComb[i] != _T(""); ++i)
	{
		m_CalibCmb.InsertString(-1, strStepComb[i]);
	}
	m_CalibCmb.SetCurSel(0);	// デフォルトは1step

	// 積分時間

	// List
	DWORD dwStyle = m_IntgListCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;
	m_IntgListCtrl.SetExtendedStyle(dwStyle);

	LVCOLUMN    lvc;
	int         i;
	TCHAR       caption[][32] = { _T("計測レベル[dBm]"), _T("デフォルト"), _T("カスタマイズ"), _T("積分時間[s]") };
	int			csize[] = {95, 70, 70, 80};
	const int   clmNum = sizeof caption / sizeof caption[0];
	int         err = 0;

	for (i = 0; i < clmNum; i++)
	{
		lvc.iSubItem	= i;
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.fmt			= LVCFMT_CENTER;
		lvc.pszText		= caption[i];
		lvc.cchTextMax = (int)wcslen(caption[i]);
		lvc.cx			= csize[i];
		m_IntgListCtrl.InsertColumn(i, &lvc);
	}

//#if 1
	int r = 0;
	LVITEM       lvi;
	stAgcTbl stCust;
	stAgcTbl stDflt;
	memset(&lvi, 0, sizeof(lvi));
	for (auto itr = m_AgcTblDfltMap.begin(); itr != m_AgcTblDfltMap.end(); ++itr)
	{
		CString att = itr->first;
		stDflt = itr->second;

		// 計測レベル
		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		lvi.lParam = (LPARAM)r;
		lvi.iItem		= r;
		lvi.iSubItem	= 0;
		lvi.pszText		= stDflt.att.GetBuffer();
		m_IntgListCtrl.InsertItem(&lvi);

		// デフォルト
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		lvi.iItem = r;
		lvi.iSubItem = 1;
		lvi.pszText = (LPWSTR)_T("");
		m_IntgListCtrl.SetItem(&lvi);

		// カスタム
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		lvi.iItem = r;
		lvi.iSubItem = 2;
		lvi.pszText = (LPWSTR)_T("");
		m_IntgListCtrl.SetItem(&lvi);

		// 積分時間
		stCust = m_AgcTblCustMap[att];
		lvi.mask = LVIF_TEXT;
		lvi.iItem = r;
		lvi.iSubItem = 3;
		lvi.pszText = stCust.intg.GetBuffer();
		//lvi.cchTextMax = stCust.intg.GetLength();
		m_IntgListCtrl.SetItem(&lvi);

		++r;
	}

	// リスト内チェックボックス作成
	m_IntgListCtrl.CreateCheckBox();

	r = 0;
	for (auto itr = m_AgcTblDfltMap.begin(); itr != m_AgcTblDfltMap.end(); ++itr)
	{
		CString att = itr->first;
		stDflt = itr->second;

		// デフォルト
		m_IntgListCtrl.SetItemData(r, 0);
		LPARAM param = 0;
		if (stDflt.enable == _T("ON"))
		{
			param |= 0x01;
		}

		// カスタマイズ
		stCust = m_AgcTblCustMap[att];
		if (stCust.enable == _T("ON"))
		{
			param |= 0x02;
		}
		m_IntgListCtrl.SetItemData(r, param);

		++r;
	}



//#else
//
//	LVITEM       lvi;
//	CString      str;
//	for (int r = 0; r < 21; ++r)
//	{
//		// 計測レベル
//		lvi.lParam		= (LPARAM)r;
//		lvi.iItem		= r;
//		lvi.iSubItem	= 0;
//		lvi.mask		= LVIF_TEXT;
//		str.Format(_T("%d"), -100 - r );
//		lvi.pszText		= str.GetBuffer();
//		m_IntgListCtrl.InsertItem(&lvi);
//
//		// 積分時間
//		lvi.lParam = (LPARAM)r;
//		lvi.iItem = r;
//		lvi.iSubItem = 3;
//		lvi.mask = LVIF_TEXT;
//		if (r < 13)
//		{
//			str.Format(_T("%d"), 1);
//		}
//		else if (r >= 13 && r < 20)
//		{
//			str.Format(_T("%d"), 3);
//		}
//		else
//		{
//			str.Format(_T("%d"), 5);
//		}
//		lvi.pszText = str.GetBuffer();
//		m_IntgListCtrl.SetItem(&lvi);
//	}
//
//	m_IntgListCtrl.CreateCheckBoxColumn(1, 21, FALSE);
//	m_IntgListCtrl.CreateCheckBoxColumn(2, 21);
//
//	// チェックボックスにチェック
//	OnCbnSelchangeRdnsAgcStepPrfCmb();
//#endif
	CenterWindowEx();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! CRdnsAgcDetailStep

-# 集計ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsAgcDetailStep::OnBnClickedRdnsAgcStepIntgBtn()
{
	int count = m_IntgListCtrl.GetItemCount();

	CString str = _T("");
	int custTime = 0;
	// カスタムの積分時間合計を計算
	for (int i = 0; i < count; ++i)
	{
		LPARAM param = m_IntgListCtrl.GetItemData(i);
		if (param & 0x02)
		{
			// 有効な積分時間を加算
			str = m_IntgListCtrl.GetItemText(i, 3);	// カスタム積分時間取得
			custTime += _wtoi(str);
		}
	}

	// デフォルトの積分時間合計を計算
	int deftTime = 0;
	for (auto itr = m_AgcTblDfltMap.begin(); itr != m_AgcTblDfltMap.end(); ++itr)
	{
		stAgcTbl st = itr->second;
		if (st.enable == _T("ON"))
		{
			// 有効な積分時間を加算
			deftTime += _wtoi(st.intg);	// ATT値に対応するデフォルト積分値取得
		}
	}

	str.Format(_T("%d"), custTime - deftTime);
	m_IntegEdit.SetWindowTextW(str);
}


/*============================================================================*/
/*! CRdnsAgcDetailStep

-# 設定ボタン処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsAgcDetailStep::OnBnClickedOk()
{
	// CUSTファイル作成
	WriteAgcTblFile();

//#if 0
//	// ファイルコピー（"_CUST"無し）
//	CString f_cust, f_lb;
//	CString folder = theApp.GetShareFilePath(eFileType_AGCCalib_Folder, theApp.GetSelectStation());
//	f_cust.Format(_T("%s%s\\agc_%s.tbl_%s_%s"), (LPCTSTR)folder, (LPCTSTR)m_Satellite, (LPCTSTR)m_AgcItem, (LPCTSTR)m_LoopBand, (LPCTSTR)TypeFile[TYPE_CUST]);
//	f_lb.Format(_T("%s%s\\agc_%s.tbl_%s"), (LPCTSTR)folder, (LPCTSTR)m_Satellite, (LPCTSTR)m_AgcItem, (LPCTSTR)m_LoopBand);
//
//	CopyFile(f_cust, f_lb, FALSE);
//#endif

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! CRdnsAgcDetailStep

-# 閉じるボタン処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsAgcDetailStep::OnClose()
{
	CDialogBase::OnClose();
}


/*============================================================================*/
/*! CRdnsAgcDetailStep

-# 校正値取得コンボボックス変更処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsAgcDetailStep::OnCbnSelchangeRdnsAgcStepPrfCmb()
{
	CString str;

	int idx = m_CalibCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}

	if ( (idx == STEP_1 )
		|| (idx == STEP_3)
		|| (idx == STEP_5)
		)
	{
		m_IntgListCtrl.CheckBoxCheckStep(2, STEP[idx]);
		m_CurrentStep = idx;
	}
	else if (idx == STEP_DEF)
	{
		SetDefaultValue();
	}
}

/*============================================================================*/
/*! CRdnsAgcDetailStep

-# リスト項目をDFLT状態にする。

@param  なし
@retval なし
*/
/*============================================================================*/
void CRdnsAgcDetailStep::SetDefaultValue()
{
	int row = 0;
	for (auto itr = m_AgcTblDfltMap.begin(); itr != m_AgcTblDfltMap.end(); ++itr)
	{
		CString att = itr->first;
		stAgcTbl st = itr->second;

		// カスタマイズをデフォルト値にする。
		LPARAM param = m_IntgListCtrl.GetItemData(row);
		if (st.enable == _T("ON"))
		{
			param = (param & ~0x02) | 0x02;
		}
		else
		{
			param = param & ~0x02;
		}
		m_IntgListCtrl.SetItemData(row, param);

		// 積分時間をデフォルト値にする。
		m_IntgListCtrl.SetItemText(row, 3, st.intg);

		++row;
	}
}

/*============================================================================*/
/*! CRdnsAgcDetailStep

-# AGC校正テーブルファイル読み込み

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CRdnsAgcDetailStep::ReadAgcTblFile(int type, map<CString, stAgcTbl> &map)
{
	FILE *fp = NULL;

	CString file;
	TCHAR sz_tmp1[20];
	TCHAR sz_tmp2[20];

	char sz_line[512];
	
	CString cbuf;
	CString str, tag;
	stAgcTbl st;

	CString folder = theApp.GetShareFilePath(eFileType_AGCCalib_Folder, theApp.GetSelectStation());
	file.Format(_T("%s%s\\agc_%s.tbl_%s_%s"), (LPCTSTR)folder, (LPCTSTR)m_Satellite, (LPCTSTR)m_AgcItem, (LPCTSTR)m_LoopBand, (LPCTSTR)TypeFile[type]);

	if (_wfopen_s(&fp, file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("AGC校正テーブルファイルを開くことができません。\n%s"), (LPCTSTR)file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp) != NULL)
	{
		cbuf = CServerFile::ConvertUnicode(sz_line);
		if (cbuf.Mid(0, 1) == '#' || cbuf.Mid(0, 1) == '\n')
		{
			continue;
		}

		// TAG
		AfxExtractSubString(str, cbuf, 0, ',');

		swscanf_s(str.GetBuffer(), _T("%s %s"),
			sz_tmp1, static_cast<unsigned>(_countof(sz_tmp1)),
			sz_tmp2, static_cast<unsigned>(_countof(sz_tmp2)));

		tag = CString(sz_tmp1);
		if (tag.Find(_T("MAIN_CAL")) == -1)
		{
			continue;
		}

		st.att = CString(sz_tmp2);

		// 校正値
		AfxExtractSubString(str, cbuf, 1, ',');
		str.TrimLeft(_T(" "));
		st.calib = str;

		// 上限許容値
		AfxExtractSubString(str, cbuf, 2, ',');
		str.TrimLeft(_T(" "));
		st.u_limit = str;

		// 下限許容値
		AfxExtractSubString(str, cbuf, 3, ',');
		str.TrimLeft(_T(" "));
		st.l_limit = str;

		// ATT増分
		AfxExtractSubString(str, cbuf, 4, ',');
		str.TrimLeft(_T(" "));
		st.incr = str;

		// 校正修正値
		AfxExtractSubString(str, cbuf, 5, ',');
		str.TrimLeft(_T(" "));
		st.corr = str;

		// 校正待ち時間
		AfxExtractSubString(str, cbuf, 6, ',');
		str.TrimLeft(_T(" "));
		st.wait = str;

		// 積分時間
		AfxExtractSubString(str, cbuf, 7, ',');
		str.TrimLeft(_T(" "));
		st.intg = str;

		// 有効無効
		AfxExtractSubString(str, cbuf, 8, ',');
		str.TrimLeft(_T(" "));
		int idx = str.Find(_T(";"));
		st.enable = str.Mid(0, idx);

		// コメント
		st.coment = str.Mid(idx+1);

		map[st.att] = st;
	}

	fclose(fp);

	return TRUE;
}

/*============================================================================*/
/*! CRdnsAgcDetailStep

-# AGC校正テーブルファイル読み込み

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CRdnsAgcDetailStep::WriteAgcTblFile()
{
	FILE *fp_cust = NULL;
	FILE *fp_dflt = NULL;

	char sz_rline[512];
	CString rbuf;
	TCHAR sz_buff[512];
	TCHAR sz_tmp1[20];
	TCHAR sz_tmp2[20];

	CStringA wbuf;
	TCHAR sz_wline[2048];


	CString f_cust;
	CString f_dflt;


	CString folder = theApp.GetShareFilePath(eFileType_AGCCalib_Folder, theApp.GetSelectStation());
	f_cust.Format(_T("%s%s\\agc_%s.tbl_%s_%s"), (LPCTSTR)folder, (LPCTSTR)m_Satellite, (LPCTSTR)m_AgcItem, (LPCTSTR)m_LoopBand, (LPCTSTR)TypeFile[TYPE_CUST]);
	f_dflt.Format(_T("%s%s\\agc_%s.tbl_%s_%s"), (LPCTSTR)folder, (LPCTSTR)m_Satellite, (LPCTSTR)m_AgcItem, (LPCTSTR)m_LoopBand, (LPCTSTR)TypeFile[TYPE_DFLT]);

	if (_wfopen_s(&fp_dflt, f_dflt, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("AGC校正テーブルファイルを開くことができません。\n%s"), (LPCTSTR)f_dflt);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (_wfopen_s(&fp_cust, f_cust, _T("wb")) != 0)
	{
		CString mes;
		mes.Format(_T("AGC校正テーブルファイルを開くことができません。\n%s"), (LPCTSTR)f_cust);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		fclose(fp_dflt);
		return FALSE;
	}

	SYSTEMTIME time;
	GetSystemTime(&time);
	CTime ctime(time);
	CString strTime;
	strTime = ctime.Format("%Y-%m-%d %H:%M:%S");

	while (fgets(sz_rline, static_cast<unsigned>(_countof(sz_rline)), fp_dflt) != NULL)
	{
		rbuf = CServerFile::ConvertUnicode(sz_rline);
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_rline));

		if (!wcsncmp(sz_buff, _T("#!HEAD:"), 7))
		{
			wsprintf(sz_wline, _T("#!HEAD: AGCTBL %s ---------- -- %s\n")
				, (LPCTSTR)strTime
				, (LPCTSTR)m_Satellite);
			wbuf = CServerFile::ConvertEuc(sz_wline);
			fputs(wbuf, fp_cust);
			continue;
		}
		else if (sz_buff[0] == '#' || sz_buff[0] == '\n')
		{
			wbuf = CServerFile::ConvertEuc(sz_buff);
			fputs(wbuf, fp_cust);
			continue;
		}

		CString str;
		// TAG
		AfxExtractSubString(str, rbuf, 0, ',');

		swscanf_s(str.GetBuffer(), _T("%s %s"),
			sz_tmp1, static_cast<unsigned>(_countof(sz_tmp1)),
			sz_tmp2, static_cast<unsigned>(_countof(sz_tmp2)));

		CString tag = CString(sz_tmp1);
		if (tag.Find(_T("MAIN_CAL")) == -1)
		{
			wbuf = CServerFile::ConvertEuc(sz_buff);
			fputs(wbuf, fp_cust);
			continue;
		}

		CString dAtt = CString(sz_tmp2);

		int size = m_IntgListCtrl.GetItemCount();
		for (int i = 0; i < size; ++i)
		{
			CString att = m_IntgListCtrl.GetItemText(i, 0);
			if (att == dAtt)
			{
				stAgcTbl &stCust = m_AgcTblCustMap[att];

				// カスタム
				LPARAM param = m_IntgListCtrl.GetItemData(i);
				CString enable = _T("OFF");
				if (param & 0x02)
				{
					enable = _T("ON");
				}

				// 積分時間
				CString iteg = m_IntgListCtrl.GetItemText(i, 3);

				wsprintf(sz_wline, _T("%s %5s, %8s, %10s, %10s, %7s, %10s, %10s, %8s, %8s;%s")
					, (LPCTSTR)tag
					, (LPCTSTR)att
					, (LPCTSTR)stCust.calib
					, (LPCTSTR)stCust.u_limit
					, (LPCTSTR)stCust.l_limit
					, (LPCTSTR)stCust.incr
					, (LPCTSTR)stCust.corr
					, (LPCTSTR)stCust.wait
					, (LPCTSTR)iteg
					, (LPCTSTR)enable
					, (LPCTSTR)stCust.coment);
				wbuf = CServerFile::ConvertEuc(sz_wline);
				fputs(wbuf, fp_cust);
			}
		}

	}

	fclose(fp_dflt);
	fclose(fp_cust);

	return TRUE;
}

/*============================================================================*/
/*! CRdnsAgcDetailStep

-# AGCステップ/積分時間の入力範囲値のメッセージ表示。

@param  message	：メッセージの識別子
@param  wParam	：メッセージの最初のパラメータ
@param  lParam	：メッセージの 2 番目のパラメータ
@retval 処理結果
*/
/*============================================================================*/
LRESULT CRdnsAgcDetailStep::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){

	case	eMessage_AgcStepOutOFRange:
		AfxMessageBox(_T("積分時間は1～300[秒]を入力してください。"), MB_OK | MB_ICONEXCLAMATION);
		break;

	default:
		return CDialogBase::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}
