/*============================================================================*/
// 監視情報テーブル
//   CDBAccess::GetObsname_t()で取得エラーとなる監視をテスト用に実装
/*============================================================================*/
// MonitorInfo.cpp : 実装ファイル

#include "stdafx.h"
#include "StationControl.h"
#include "MonitorInfo.h"
#include "SatelliteData.h"
#include "afxdialogex.h"
#include "EngValCnv.h"
#include <string.h>
#include <fstream>
#include <iostream>
#include <iomanip>

#ifdef MON_UT

#define COLUMN	4

// CMonitorListCtrl リスト

IMPLEMENT_DYNAMIC(CMonitorListCtrl, CListCtrl)

/*============================================================================*/
// コンストラクタ
/*============================================================================*/
CMonitorListCtrl::CMonitorListCtrl()
{

}

/*============================================================================*/
// デストラクタ
/*============================================================================*/
CMonitorListCtrl::~CMonitorListCtrl()
{
}

BEGIN_MESSAGE_MAP(CMonitorListCtrl, CListCtrl)
END_MESSAGE_MAP()

/*============================================================================*/
// リストの追加
/*============================================================================*/
int CMonitorListCtrl::AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam/*=0*/)
{
	LV_ITEM lv;

	memset(&lv, 0, sizeof(lv));
	lv.mask = LVIF_TEXT;
	lv.iItem = nItem;
	lv.iSubItem = nSubItem;
	if (nSubItem == 0)
	{
		lv.mask |= LVIF_PARAM;
		lv.lParam = dwParam;
	}
	lv.pszText = (LPTSTR)(LPCTSTR)strItem;
	lv.cchTextMax = strItem.GetLength();

	if(nSubItem == 0)
		return InsertItem(&lv);
	return SetItem(&lv);
}

// CMonitorInfo ダイアログ

IMPLEMENT_DYNAMIC(CMonitorInfo, CDialogBase)

/*============================================================================*/
// コンストラクタ
/*============================================================================*/
CMonitorInfo::CMonitorInfo(CWnd* pParent /*=NULL*/)
	: CDialogBase(CMonitorInfo::IDD, pParent)
{

}

/*============================================================================*/
// デストラクタ
/*============================================================================*/
CMonitorInfo::~CMonitorInfo()
{
}

void CMonitorInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MON_UT_LIST, m_MonitorList);
	DDX_Text(pDX, IDC_MON_UT_FNAME_EDIT, m_FileName);
}

BEGIN_MESSAGE_MAP(CMonitorInfo, CDialogBase)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_MON_UT_READ_BTN, &CMonitorInfo::OnBnClickedMonUtReadBtn)
	ON_BN_CLICKED(IDC_MON_UT_RENEW_BTN, &CMonitorInfo::OnBnClickedMonUtRenewBtn)
END_MESSAGE_MAP()

// CMonitorInfo メッセージ ハンドラー

/*============================================================================*/
// ダイアログ初期化処理
/*============================================================================*/
BOOL CMonitorInfo::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// MonitorList
	DWORD dwStyle = m_MonitorList.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;
	m_MonitorList.SetExtendedStyle(dwStyle);

	LVCOLUMN	lvc;
	TCHAR		caption[COLUMN][40] = { _T("監視名(sz_obsname)"), _T("整数値(l_data)"), _T("実数値(d_data)"), _T("ステータス(sz_sts_name)")  };
	int			csize[COLUMN] = { 170, 90, 130, 330 };

	for(int i=0; i<COLUMN; i++)
	{
		lvc.iSubItem = i;
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.fmt = LVCFMT_CENTER;
		lvc.pszText = caption[i];
		lvc.cchTextMax = (int)wcslen(caption[i]);
		lvc.cx = csize[i];
		m_MonitorList.InsertColumn(i, &lvc);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

/*============================================================================*/
// ダイアログ表示処理
/*============================================================================*/
BOOL CMonitorInfo::UpdateDlg()
{
	m_MonitorList.SetRedraw(true);

	return TRUE;
}

/*============================================================================*/
// CSV読込ボタン処理
/*============================================================================*/
void CMonitorInfo::OnBnClickedMonUtReadBtn()
{
	CString		filter("CSV Files (*.csv)|*.csv||");
	CFileDialog	selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	
	if (selDlg.DoModal() != IDOK)
	{
		return;
	}

	m_FileName = selDlg.GetPathName();
	DisplayList();

	UpdateData(FALSE);

	return;
}

/*============================================================================*/
// 監視情報更新ボタン処理
/*============================================================================*/
void CMonitorInfo::OnBnClickedMonUtRenewBtn()
{
	int i, j;
	CString text;
	TCHAR *pEnd;

	m_obsname_list.clear();

	CEngValCnv::obsname_t mon_data;

	for(i=0; i<m_MonitorList.GetItemCount(); i++)
	{
		memset(&mon_data, 0, sizeof(CEngValCnv::obsname_t));
		for(j=0; j<COLUMN; j++)
		{
			text = m_MonitorList.GetItemText(i, j);
			switch(j)
			{
				case 0:
					strcpy_s(mon_data.sz_obsname, CTLID_LEN, CStringA(text));
					break;
				case 1:
					mon_data.l_data = _tcstol(text, &pEnd, 10);
					break;
				case 2:
					mon_data.d_data = _tcstod(text, &pEnd);
					mon_data.ld_data = _tcstold(text, &pEnd);
					break;
				case 3:
					strcpy_s(mon_data.sz_sts_name, 64, CStringA(text));
					break;
			}
		}
		mon_data.l_inflg = INFLG_ON;	//データ有効フラグ 1=有効
		m_obsname_list.push_back(mon_data);

		theApp.UpdateControlCommand(theApp.GetSelectStation(), CString(mon_data.sz_obsname), CString(mon_data.sz_sts_name));
	}

	theApp.GetSatelliteData().GetDBAccessCls().SetMonUT(m_obsname_list);

	return;
}

/*============================================================================*/
// リスト表示
/*============================================================================*/
void CMonitorInfo::DisplayList()
{
	CStdioFile csvFile;
	CFileException ex;
	CString StrArray[COLUMN];
	CString szData, szToken;
	long l_data;
	double d_data;
	TCHAR *pEnd;
	int i;
	int cnt;
	int nCurPos;
	int row;

	char* back = setlocale(LC_ALL, "japanese");
	if(!csvFile.Open(m_FileName, CFile::modeRead, &ex))
	{
		CString mes;
		mes.Format(_T("ファイルが見つかりませんでした。\n%s"), (LPCTSTR)m_FileName);
		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
		return;
	}

	m_MonitorList.SetRedraw(false);
	m_MonitorList.DeleteAllItems();

	row = 0;
	// ファイルを1行ずつ読み込み
	while(csvFile.ReadString(szData) != NULL)
	{
		cnt = szData.Find(_T("#"));
		if(cnt >= 0)
		{
			continue;
		}

		nCurPos = 0;
		cnt = 0;
		for(i=0; i<COLUMN; i++)
		{
			// カンマで文字列を分割・抽出
			szToken = szData.Tokenize(_T(","), nCurPos);
			if(szToken.IsEmpty())
			{
				break;
			}
			StrArray[i] = szToken;
			cnt++;
		}

		pEnd = NULL;
		l_data = _tcstol(StrArray[1], &pEnd, 10);
		if(*pEnd == NULL)
		{
			//continue;
		}

		pEnd = NULL;
		d_data = _tcstod(StrArray[2], &pEnd);
		if(*pEnd == NULL)
		{
			//continue;
		}

		for(i=0; i<cnt; i++)
		{
			m_MonitorList.AddItem(row, i, StrArray[i]);
		}

		row++;
	}

	csvFile.Close();

	if (m_MonitorList.GetItemCount() > 0)
	{
		m_MonitorList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}
	m_MonitorList.SetRedraw(true);

	return;
}

#endif

