/*============================================================================*/
/*! BusinessDiary.cpp

-# 無線業務日誌一覧画面
*/
/*============================================================================*/
// BusinessDiary.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "BusinessDiary.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"
#include "BusinessDiaryFileNotify.h"
#include "WaitDialog.h"

#include "HookMsgBox.h"

#include <imagehlp.h>
#pragma comment(lib, "imagehlp.lib")


#define	PRINT_FORM_CSV_FILE	_T("REP_FORM_CSV.txt")

const char* mEndTime[3] = { "07:59:59", "15:59:59", "23:59:59" };

//----------------------------------------------------------------------
// CEditInListCtrl

IMPLEMENT_DYNAMIC(CDiaryListCtrl, CListCtrl)

/*============================================================================*/
/*! CDiaryListCtrl

-# コンストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CDiaryListCtrl::CDiaryListCtrl()
{
	mListNo = 0;
}

/*============================================================================*/
/*! CDiaryListCtrl

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CDiaryListCtrl::~CDiaryListCtrl()
{
}


BEGIN_MESSAGE_MAP(CDiaryListCtrl, CListCtrl)
	ON_NOTIFY(HDN_DIVIDERDBLCLICKA, 0, &CDiaryListCtrl::OnHdnDividerdblclick)
	ON_NOTIFY(HDN_DIVIDERDBLCLICKW, 0, &CDiaryListCtrl::OnHdnDividerdblclick)
	ON_NOTIFY(HDN_ENDTRACKA, 0, &CDiaryListCtrl::OnHdnEndtrack)
	ON_NOTIFY(HDN_ENDTRACKW, 0, &CDiaryListCtrl::OnHdnEndtrack)
END_MESSAGE_MAP()


/*============================================================================*/
/*! CDiaryListCtrl

-# コントロール通知のメッセージ マップを処理します。
   ヘッダーを固定にする。

@param  wParam	：メッセージがコントロールからそのメッセージを送信するコントロールを識別します。
@param  lParam	：通知コードと追加情報を含む通知メッセージ (NMHDR) の構造体へのポインター。
@param  pResult	：メッセージが処理されたとき結果を格納するコードする LRESULT の変数へのポインター。
@retval メッセージを処理した場合は1を返します; それ以外の場合は 0。
*/
/*============================================================================*/
//BOOL CDiaryListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
//{
//	HD_NOTIFY *phdn = (HD_NOTIFY *)lParam;
//	if (phdn->hdr.code == HDN_BEGINTRACK) {
//		// カラムを固定
////		if ((phdn->iItem == 0) || (phdn->iItem == 1) || (phdn->iItem == 2) || (phdn->iItem == 3)) 
//		{
//			*pResult = TRUE;
//			return(1);
//		}
//	}
//	return(0);
////	return CListCtrl::OnNotify(wParam, lParam, pResult);
//}
/*============================================================================*/
/*! 業務日誌

-# カラムの自動サイズ調整

@param	col		カラム位置のカラムサイズを自動に調整します
@retval

*/
/*============================================================================*/
void CDiaryListCtrl::AutoSizeColumns(int col /*=-1*/)
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);

	// create dummy
	int dummycol = pHeader->GetItemCount();
	InsertColumn(dummycol, _T("dummy"), LVCFMT_LEFT, 10);

	SetRedraw(false);
	int mincol = col < 0 ? 0 : col;
	int maxcol = col < 0 ? pHeader->GetItemCount() - 1 : col;
	for (col = mincol; col <= maxcol; col++){
		SetColumnWidth(col, LVSCW_AUTOSIZE);
		int wc1 = GetColumnWidth(col);
		SetColumnWidth(col, LVSCW_AUTOSIZE_USEHEADER);
		int wc2 = GetColumnWidth(col);
		int wc = MAX(75, MAX(wc1, wc2));
		if (col == 0)
			wc = 50;
		SetColumnWidth(col, wc);
	}
	// delete dummy
	DeleteColumn(dummycol);
	SetRedraw(true);
}
/*============================================================================*/
/*! 業務日誌

-# カラムの自動サイズ調整

@param	col		カラム位置のカラムサイズを自動に調整します
@retval

*/
/*============================================================================*/
void CDiaryListCtrl::ResetColumns(vector<int>& cols)
{
	//{ _T(""),                               _T("（主任）無線局従事者"), _T("無線設備操作者"),   _T("") };
	//{ _T("番号"), _T("運用日"), _T("天候"), _T("資格"), _T("氏名"),     _T("資格"), _T("氏名"), _T("相手局"), _T("延通信時間（分）"), _T("通信回数") };

	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int maxcol = pHeader->GetItemCount();
	if (maxcol > cols.size())
		return;

	if (maxcol == 4 && cols.size() == 10){
		// メインからサブ
		int width = cols[0] + cols[1] + cols[2];
		SetColumnWidth(0, width);
		width = cols[3] + cols[4];
		SetColumnWidth(1, width);
		width = cols[5] + cols[6];
		SetColumnWidth(2, width);
		width = cols[7] + cols[8] + cols[9];
		SetColumnWidth(3, width);
	}
	//if (maxcol == 10 && cols.size() == 4){
	//	// サブからメイン
	//	int width = cols[0];
	//	int wc = GetColumnWidth(2);
	//	if (width > wc){
	//		SetColumnWidth(2, 0);
	//		width -= wc;
	//		wc = GetColumnWidth(1);
	//		if (width > wc){
	//			SetColumnWidth(1, 0);
	//			width -= wc;
	//		}
	//		else{
	//		}
	//	}
	//	else{
	//		SetColumnWidth(2, wc - width);
	//	}
	//	width = cols[3] + cols[4];
	//	SetColumnWidth(1, width);
	//	width = cols[5] + cols[6];
	//	SetColumnWidth(2, width);
	//	width = cols[7] + cols[8] + cols[9];
	//	SetColumnWidth(3, width);
	//}
}

/*============================================================================*/
/*! 業務日誌

-# 業務日誌項目の登録

@param	nItem		アイテム番号
@param	nSubItem	サブアイテム番号
@param	strItem		設定文字列
@param	dwParam		ユーザ値
@retval

*/
/*============================================================================*/
int CDiaryListCtrl::AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam/*=0*/)
{
	LV_ITEM lv;
	memset(&lv, 0, sizeof(lv));
	lv.mask = LVIF_TEXT;
	lv.iItem = nItem;
	lv.iSubItem = nSubItem;
	if (nSubItem == 0){
		lv.mask |= LVIF_PARAM;
		lv.lParam = dwParam;
	}
	lv.pszText = (LPTSTR)(LPCTSTR)strItem;
	lv.cchTextMax = strItem.GetLength();

	if (nSubItem == 0)
		return InsertItem(&lv);
	return SetItem(&lv);
}


void CDiaryListCtrl::OnHdnDividerdblclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	mHeaderSize.clear();
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int maxcol = pHeader->GetItemCount();
	for (int col = 0; col < maxcol; col++){
		int wc = GetColumnWidth(col);
		mHeaderSize.push_back(wc);
	}
	GetParent()->PostMessage(WM_USER + 200, 0, mListNo);
	*pResult = 0;
}


void CDiaryListCtrl::OnHdnEndtrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	mHeaderSize.clear();
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int maxcol = pHeader->GetItemCount();
	for (int col = 0; col < maxcol; col++){
		int wc = GetColumnWidth(col);
		mHeaderSize.push_back(wc);
	}
	GetParent()->PostMessage(WM_USER + 200, 0, mListNo);
	*pResult = 0;
}


// CBusinessDiary ダイアログ

IMPLEMENT_DYNAMIC(CBusinessDiary, CDialogBase)

/*============================================================================*/
/*! CBusinessDiary

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CBusinessDiary::CBusinessDiary(CWnd* pParent /*=NULL*/)
: CDialogBase(CBusinessDiary::IDD, pParent)
	, m_DayTime(0)
{
	mbEdit = FALSE;
}

/*============================================================================*/
/*! CBusinessDiary

-# デストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CBusinessDiary::~CBusinessDiary()
{
}

void CBusinessDiary::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DIARY_STLT_CMB, m_SatelliteCmb);
	DDX_DateTimeCtrl(pDX, IDC_DIARY_STLT_DP, m_DayTime);
	DDX_Control(pDX, IDC_DIARY_LIST, m_DiaryList);
	DDX_Control(pDX, IDC_DIARY_LIST2, m_DiaryList2);
	DDX_Control(pDX, IDC_DIARY_STLT_DP, mDate);
}


BEGIN_MESSAGE_MAP(CBusinessDiary, CDialogBase)
	ON_BN_CLICKED(IDC_DIARY_EDIT_BTN, &CBusinessDiary::OnBnClickedDiaryEditBtn)
	ON_BN_CLICKED(IDC_DIARY_PRNT_BTN, &CBusinessDiary::OnBnClickedDiaryPrntBtn)
	ON_BN_CLICKED(IDC_DIARY_CSV_BTN, &CBusinessDiary::OnBnClickedDiaryCsvBtn)
	ON_BN_CLICKED(IDC_DIARY_SINFO_BTN, &CBusinessDiary::OnBnClickedDiarySinfoBtn)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DIARY_STLT_DP, &CBusinessDiary::OnDtnDatetimechangeDiaryStltDp)
	ON_CBN_SELCHANGE(IDC_DIARY_STLT_CMB, &CBusinessDiary::OnCbnSelchangeDiaryStltCmb)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CBusinessDiary::OnMenuCloseClose)
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DIARY_LIST, &CBusinessDiary::OnLvnItemchangedDiaryList)
END_MESSAGE_MAP()


// CBusinessDiary メッセージ ハンドラー
/*============================================================================*/
/*! CBusinessDiary

-# ダイアログ初期化処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CBusinessDiary::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// パス中か確認
	if (IsPass() == TRUE)
	{
		// パス中は開かない
		EndDialog(IDCANCEL);
		return TRUE;  // return TRUE unless you set the focus to a control
	}

	// 無線業務日誌取得
#ifndef _DEBUG
//	if (Shell_GetRadiolog() == FALSE)
//	{
//		EndDialog(IDCANCEL);
//		return TRUE;  // return TRUE unless you set the focus to a control
//	}

	Shell_GetRadiolog();	// ファイルが存在しないケースもありえるため、シェルの戻りは見ない
#endif

	// RFファイルが正しいか確認
	if (CheckFormatRFFile() == FALSE)
	{
		EndDialog(IDCANCEL);
		return TRUE;
	}

	// RF情報ファイルの最終行にONがあるか確認
	if (CheckRFInfoFile() == TRUE)
	{
		EndDialog(IDCANCEL);
		return TRUE;  // return TRUE unless you set the focus to a control
	}

#if 0
	// 無線業務日誌ファイル確認
	CBusinessDiaryFileNotify dlg;
	if (dlg.DoModal() == IDCANCEL){
		EndDialog(IDCANCEL);
		return TRUE;  // return TRUE unless you set the focus to a control
	}
#endif

	//-----------------------------------------------------
	// 日付フォーマット変更
	mDate.SetFormat(_T("yyyy-MM-dd"));

	//-----------------------------------------------------
	// DiaryList2(一番上のヘッダ)
	DWORD dwStyle2 = m_DiaryList2.GetExtendedStyle();
	dwStyle2 |= LVS_EX_GRIDLINES;
	m_DiaryList2.SetExtendedStyle(dwStyle2);

	LVCOLUMN    lvc2;
	TCHAR       caption2[][32] = { _T(""), _T("（主任）無線局従事者"), _T("無線設備操作者"), _T("") };
	int			csize2[] = { 200, 170, 170, 260 };
	const int   clmNum2 = sizeof caption2 / sizeof caption2[0];

	for (int i = 0; i < clmNum2; i++)
	{
		lvc2.iSubItem = i;
		lvc2.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc2.fmt = LVCFMT_CENTER;
		lvc2.pszText = caption2[i];
		lvc2.cchTextMax = (int)wcslen(caption2[i]);
		lvc2.cx = csize2[i];
		m_DiaryList2.InsertColumn(i, &lvc2);
	}

	//-----------------------------------------------------
	// DiaryList
	DWORD dwStyle = m_DiaryList.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_DiaryList.SetExtendedStyle(dwStyle);

	LVCOLUMN    lvc;
	TCHAR       caption[][32] = { _T("番号"), _T("運用日"), _T("天候"), _T("資格"), _T("氏名"), _T("資格"), _T("氏名"), _T("相手局"), _T("延通信時間（分）"), _T("通信回数") };
	int			csize[] = { 50, 100, 50, 70, 100, 70, 100, 100, 100, 60};
	const int   clmNum = sizeof caption / sizeof caption[0];
	int         err = 0;

	for (int i = 0; i < clmNum; i++)
	{
		lvc.iSubItem = i;
		if (i == 8 || i == 9)
		{
			lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			lvc.fmt = LVCFMT_RIGHT;

		}
		else
		{
			lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			lvc.fmt = LVCFMT_CENTER;
		}
		lvc.pszText = caption[i];
		lvc.cchTextMax = (int)wcslen(caption[i]);
		lvc.cx = csize[i];
		m_DiaryList.InsertColumn(i, &lvc);
	}

	CreateDialyList();
	m_DiaryList.AutoSizeColumns();
	if (mSpaceList.size() > 1){
		// 衛星コンボボックスへの登録
		m_SatelliteCmb.ResetContent();
		vector<CString>::iterator itr;
		for (itr = mSpaceList.begin(); itr != mSpaceList.end(); itr++){
			m_SatelliteCmb.AddString((*itr));
		}
		m_SatelliteCmb.SetCurSel(0);

		// カレンダコントロールのカレント設定
		SYSTEMTIME date;
		memset(&date, 0, sizeof(SYSTEMTIME));
		CString str;

		if (mStartOpeTime.IsEmpty() == false)
		{
			AfxExtractSubString(str, mStartOpeTime, 0, '-');
			date.wYear = _wtoi(str);
			AfxExtractSubString(str, mStartOpeTime, 1, '-');
			date.wMonth = _wtoi(str);
			AfxExtractSubString(str, mStartOpeTime, 2, '-');
			date.wDay = _wtoi(str);
			mDate.SetTime(date);
		}

		// 日時フォーマット変更
		mDate.SetFormat(_T("yyyy-MM-dd"));
		CenterWindowEx();
	}

	m_DiaryList.mListNo = 0;
	m_DiaryList2.mListNo = 1;

	// メインからサブ
	CHeaderCtrl* pHeader = (CHeaderCtrl*)m_DiaryList.GetDlgItem(0);
	int maxcol = pHeader->GetItemCount();
	m_DiaryList.mHeaderSize.clear();
	for (int col = 0; col < maxcol; col++){
		int wc = m_DiaryList.GetColumnWidth(col);
		m_DiaryList.mHeaderSize.push_back(wc);
	}
	m_DiaryList2.ResetColumns(m_DiaryList.mHeaderSize);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! CBusinessDiary

-# ログダイアログ表示処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CBusinessDiary::UpdateDlg()
{
	return TRUE;
}


/*============================================================================*/
/*! CBusinessDiary

-# データリスト作成

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiary::CreateDialyList()
{
	mSpaceList.clear();
	mSpaceList.push_back(mAllText);
	mStartOpeTime.Empty();

	UINT nStation = theApp.GetSelectStation();
	if (theApp.GetSelectStationMode() == eStation_5)
		nStation = 0;
	CString filename = theApp.GetShareFilePath(eFileType_Diary_File, nStation);

	FILE *fp=NULL;

//	mDialyDataList.clear();
	vector<stDiary>	dialyDataList;
	char szDmy1[16] = { 0 };
	char szDmy2[16] = { 0 };
	char szDate[16] = { 0 };
	char szTime[16] = { 0 };
	char szCnt[16] = { 0 };
	char szDmy3[16] = { 0 };
	char szDmy4[16] = { 0 };

	TRY
	{
		// 日誌ファイルのオープン
		if (_wfopen_s(&fp, filename, _T("r")) != 0){
			// 日誌ファイルがなかった場合、ヘッダのみのファイルを作成する
			if (_wfopen_s(&fp, filename, _T("w")) != 0){
				char	szBuf[256];
				strcpy_s(m_szMakeDate, sizeof(m_szMakeDate), "1990-01-01 00:00:00");
				sprintf_s(szBuf, "Dmy1 Dmy2 %s %d Dmy3 Dmy4\n", m_szMakeDate, 0);
				fputs(szBuf, fp);
			}
		}
		else
		{
			char szline[2048];
			// ヘッダーから日時とレコード数取得
			fgets(szline, static_cast<unsigned>(_countof(szline)), fp);

			sscanf_s(szline, "%s %s %s %s %s %s %s\n"
				, szDmy1, static_cast<unsigned>(_countof(szDmy1))
				, szDmy2, static_cast<unsigned>(_countof(szDmy2))
				, szDate, static_cast<unsigned>(_countof(szDate))
				, szTime, static_cast<unsigned>(_countof(szTime))
				, szCnt, static_cast<unsigned>(_countof(szCnt))
				, szDmy3, static_cast<unsigned>(_countof(szDmy3))
				, szDmy4, static_cast<unsigned>(_countof(szDmy4)));

			memset(m_szMakeDate, '\0', sizeof(m_szMakeDate));
			strcpy_s(m_szMakeDate, sizeof(m_szMakeDate), szDate);
			strcat_s(m_szMakeDate, sizeof(m_szMakeDate), " ");
			strcat_s(m_szMakeDate, sizeof(m_szMakeDate), szTime);

			mStrRecCount = CString(szCnt);

			CString str, cbuf;
			BOOL bRead = TRUE;
			while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL){
				cbuf = CServerFile::ConvertUnicode(szline);
				cbuf = cbuf.Mid(0, cbuf.GetLength() - 1);	// 改行コード削除
				int i = 0;
				stDiary temp;
#if 1

				// 番号
				if (AfxExtractSubString(str, cbuf, i++, ','))
				{
#if 0
					if (str == _T("-"))
					{
						continue;
					}
#endif
					temp.no = str;
				}

				// 運用日
				if (AfxExtractSubString(str, cbuf, i++, ','))
				{
//					if (mStartOpeTime.IsEmpty() == true)
//						mStartOpeTime = str;
//					mEndOpeTime = str;
					temp.date = str;
				}
#else

				while (AfxExtractSubString(str, cbuf, i++, ',')){
					switch (i - 1){
					case	0:	// 番号
						temp.no = str;
						break;
					case	1:	// 運用日
						if (mStartOpeTime.IsEmpty() == true)
							mStartOpeTime = str;
						mEndOpeTime = str;
						temp.date = str;
						break;
					}

					if (i == 2)
						break;
				}
#endif

#if 1
				for (int u = 0; u < 3; ++u)
				{
					// 天候
					temp.data[u].weather = _T("-");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						temp.data[u].weather = str;
					}

					// 資格
					temp.data[u].capa1 = _T("-");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						temp.data[u].capa1 = str;
					}

					// 氏名
					temp.data[u].name1 = _T("-");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						temp.data[u].name1 = str;
					}

					// 資格
					temp.data[u].capa2 = _T("-");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						temp.data[u].capa2 = str;
					}

					// 氏名
					temp.data[u].name2 = _T("-");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						temp.data[u].name2 = str;
					}

					// 資格
					temp.data[u].capa3 = _T("-");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						temp.data[u].capa3 = str;
					}

					// 氏名
					temp.data[u].name3 = _T("-");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						temp.data[u].name3 = str;
					}

					// 相手局
					temp.data[u].station = _T("-");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						AddSpaceName(str);
						temp.data[u].station = str;
					}

					// 延通信時間
					temp.data[u].time = _T("0");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						temp.data[u].time = str;
					}

					// 通信回数
					temp.data[u].count = _T("0");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						temp.data[u].count = str;
					}
				}
#else
				int k = 0;
				while (AfxExtractSubString(str, cbuf, i++, ','))
				{
					switch ((i - 3) % 10){
					case	0:	// 天候
						temp.data[k].weather = str;
						break;
					case	1:	// 資格
						temp.data[k].capa1 = str;
						break;
					case	2:	// 氏名
						temp.data[k].name1 = str;
						break;
					case	3:	// 資格
						temp.data[k].capa2 = str;
						break;
					case	4:	// 氏名
						temp.data[k].name2 = str;
						break;
					case	5:	// 資格
						temp.data[k].capa3 = str;
						break;
					case	6:	// 氏名
						temp.data[k].name3 = str;
						break;
					case	7:	// 相手局
						AddSpaceName(str);
						temp.data[k].station = str;
						break;
					case	8:	// 延通信時間
						temp.data[k].time = str;
						break;
					case	9:	// 通信回数
						temp.data[k].count = str;
						k++;
						break;
					default:
						break;
					}

					if (i >= 32)
					{
						break;
					}
				}
#endif

#if 1
				// 備考1
				temp.info1 = _T("");
				if (AfxExtractSubString(str, cbuf, i++, ','))
				{
					temp.info1 = str;
				}

				// 備考2
				temp.info2 = _T("");
				if (AfxExtractSubString(str, cbuf, i++, ','))
				{
					temp.info2 = str;
				}

				// 備考3
				temp.info3 = _T("");
				if (AfxExtractSubString(str, cbuf, i++, ','))
				{
					temp.info3 = str;
				}

#else
				temp.info1 = _T("");
				temp.info2 = _T("");
				temp.info3 = _T("");

				while (AfxExtractSubString(str, cbuf, i++, ','))
				{
					switch (i - 1){
					case	32:	// 備考1
						temp.info1 = str;
						break;
					case	33:	// 備考2
						temp.info2 = str;
						break;
					case	34:	// 備考3
						temp.info3 = str;
						break;
					}
				}
#endif
//				mDialyDataList.push_back(temp);
				dialyDataList.push_back(temp);
			}
		}
	}CATCH_ALL(e){
	}
	END_CATCH_ALL

	if(fp != NULL)
		fclose(fp);

	// 現在時刻の時間帯を設定する
	GetTimeConf(NULL);
	// RF情報ファイル読み込み
	ReadRFInfoFile();
	// 無線業務日誌作成／更新処理
	MakeReport(dialyDataList);
	// 無線業務日誌をファイルに書き込み
	WriteDiaryData();

	if (mDialyDataList.size() > 0)
	{
		mStartOpeTime = mDialyDataList[0].date;
		mEndOpeTime = mDialyDataList[mDialyDataList.size() - 1].date;
	}

	// リスト表示
	DisplayList(mAllText);
}

/*============================================================================*/
/*! CBusinessDiary

-# リスト表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiary::DisplayList(CString name)
{
	CWaitCursor wait;

	m_DiaryList.SetRedraw(false);
	m_DiaryList.DeleteAllItems();

	mDialyDataListTmp.clear();

	// リスト表示
	vector<stDiary>::iterator itr;
	int item = 0;
	DWORD nExistTime[3];
	int nTime;
	int nCount;

	for (itr = mDialyDataList.begin(); itr != mDialyDataList.end(); itr++)
	{
		stDiary st = (*itr);
		if (name != mAllText)
		{
			if (name != st.data[0].station && name != st.data[1].station && name != st.data[2].station)
				continue;
		}
		int subitem = 0;

		// 番号が"-"の場合はリストしない
		if (st.no == _T("-"))
		{
			continue;
		}

		for (int nTimeIdx = 0; nTimeIdx < 3; nTimeIdx++)
		{
			nExistTime[nTimeIdx] = 0;
		}

		nTime = 0;
		nCount = 0;
		// 衛星名が入っているレコードを表示する
		for (int nTimeIdx = 0; nTimeIdx < 3; nTimeIdx++)
		{
			// 時間と回数は合計を表示する
			nTime += _ttoi(st.data[nTimeIdx].time);
			nCount += _ttoi(st.data[nTimeIdx].count);

			if (st.data[nTimeIdx].station != _T("") && st.data[nTimeIdx].station != _T("-"))
			{
				// 衛星がある
				nExistTime[nTimeIdx] |= 1 << 3;
			}
			if (st.data[nTimeIdx].time != _T("0") && st.data[nTimeIdx].count != _T("0"))
			{
				// 通信時間と通信回数が0でない
				nExistTime[nTimeIdx] |= 1 << 2;
			}
			if (st.data[nTimeIdx].weather.Find(_T("-")) < 0)
			{
				// 天候情報がある
				nExistTime[nTimeIdx] |= 1 << 1;
			}
			if ((st.data[nTimeIdx].capa1 != _T("") && st.data[nTimeIdx].capa1 != _T("-") && st.data[nTimeIdx].capa1 != _T("------")) ||
				(st.data[nTimeIdx].name1 != _T("") && st.data[nTimeIdx].name1 != _T("-") && st.data[nTimeIdx].name1 != _T("--------------")) ||
				(st.data[nTimeIdx].capa2 != _T("") && st.data[nTimeIdx].capa2 != _T("-") && st.data[nTimeIdx].capa2 != _T("------")) ||
				(st.data[nTimeIdx].name2 != _T("") && st.data[nTimeIdx].name2 != _T("-") && st.data[nTimeIdx].name2 != _T("--------------")))
			{
				// 資格、氏名情報がある
				nExistTime[nTimeIdx] |= 1 << 0;
			}
		}

		DWORD nCurTime = nExistTime[0];
		int nCurIdx = 0;		// 初期値は時間帯1
		for (int nTimeIdx = 0; nTimeIdx < 3; nTimeIdx++)
		{
			// 優先度が高い時間帯を探す
			if (nCurTime < nExistTime[nTimeIdx])
			{
				nCurTime = nExistTime[nTimeIdx];
				nCurIdx = nTimeIdx;
			}
		}

		m_DiaryList.AddItem(item, subitem++, st.no);
		m_DiaryList.AddItem(item, subitem++, st.date);
		stMember dst;
		ExchgNoneData2Long(st.data[nCurIdx], dst);
		m_DiaryList.AddItem(item, subitem++, dst.weather);
		m_DiaryList.AddItem(item, subitem++, dst.capa1);
		m_DiaryList.AddItem(item, subitem++, dst.name1);
		m_DiaryList.AddItem(item, subitem++, dst.capa2);
		m_DiaryList.AddItem(item, subitem++, dst.name2);
		m_DiaryList.AddItem(item, subitem++, dst.station);

		CString strTime, strCount;
		strTime.Format(_T("%d"), nTime);
		strCount.Format(_T("%d"), nCount);
		m_DiaryList.AddItem(item, subitem++, strTime);
		m_DiaryList.AddItem(item, subitem++, strCount);
		mDialyDataListTmp.push_back(st);
		item++;
	}

	if (m_DiaryList.GetItemCount() > 0){
		m_DiaryList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}
	m_DiaryList.SetRedraw(true);
}


/*============================================================================*/
/*! CBusinessDiary

-# 各項のデータ無しまたは'-'文字を'-'連続文字列に変換する

@param  src	変換前データ('-')
@param  dst	変換後データ('------')など。NULL時は変換前データに入れる
@retval なし
*/
/*============================================================================*/
void CBusinessDiary::ExchgNoneData2Long(stMember& src, stMember& dst)
{
	// weather
	if (src.weather == _T("") || src.weather == _T("-"))
	{
		dst.weather = _T("---");
	}
	else if (&src != &dst)
	{
		dst.weather = src.weather;
	}

	// capa1
	if (src.capa1 == _T("") || src.capa1 == _T("-"))
	{
		dst.capa1 = ("------");
	}
	else if (&src != &dst)
	{
		dst.capa1 = src.capa1;
	}

	// name1
	if (src.name1 == _T("") || src.name1 == _T("-"))
	{
		dst.name1 = _T("--------------");
	}
	else if (&src != &dst)
	{
		dst.name1 = src.name1;
	}

	// capa2
	if (src.capa2 == _T("") || src.capa2 == _T("-"))
	{
		dst.capa2 = ("------");
	}
	else if (&src != &dst)
	{
		dst.capa2 = src.capa2;
	}

	// name2
	if (src.name2 == _T("") || src.name2 == _T("-"))
	{
		dst.name2 = _T("--------------");
	}
	else if (&src != &dst)
	{
		dst.name2 = src.name2;
	}

	// capa3
	if (src.capa3 == _T("") || src.capa3 == _T("-"))
	{
		dst.capa3 = ("------");
	}
	else if (&src != &dst)
	{
		dst.capa3 = src.capa3;
	}

	// name3
	if (src.name3 == _T("") || src.name3 == _T("-"))
	{
		dst.name3 = _T("--------------");
	}
	else if (&src != &dst)
	{
		dst.name3 = src.name3;
	}

	// station
	if (src.station == _T("") || src.station == _T("-"))
	{
		dst.station = _T("------");
	}
	else if (&src != &dst)
	{
		dst.station = src.station;
	}

	// time
	if (src.time == _T("") || src.time == _T("-"))
	{
		dst.time = _T("0");
	}
	else if (&src != &dst)
	{
		dst.time = src.time;
	}

	// count
	if (src.count == _T("") || src.count == _T("-"))
	{
		dst.count = _T("0");
	}
	else if (&src != &dst)
	{
		dst.count = src.count;
	}
}

/*============================================================================*/
/*! CBusinessDiary

-# 各項のデータ無しまたは'-'連続文字列を'-'文字に変換する

@param  src	変換後データ('------')など
@param  dst	変換前データ('-')
@retval なし
*/
/*============================================================================*/
void CBusinessDiary::ExchgNoneData2Short(stMember& src, stMember& dst)
{
	// weather
	if (src.weather == _T("") || src.weather == _T("---"))
	{
		dst.weather = _T("-");
	}
	else if (&src != &dst)
	{
		dst.weather = src.weather;
	}

	// capa1
	if (src.capa1 == _T("") || src.capa1 == ("------"))
	{
		dst.capa1 = ("-");
	}
	else if (&src != &dst)
	{
		dst.capa1 = src.capa1;
	}

	// name1
	if (src.name1 == _T("") || src.name1 == _T("--------------"))
	{
		dst.name1 = _T("-");
	}
	else if (&src != &dst)
	{
		dst.name1 = src.name1;
	}

	// capa2
	if (src.capa2 == _T("") || src.capa2 == ("------"))
	{
		dst.capa2 = ("-");
	}
	else if (&src != &dst)
	{
		dst.capa2 = src.capa2;
	}

	// name2
	if (src.name2 == _T("") || src.name2 == _T("--------------"))
	{
		dst.name2 = _T("-");
	}
	else if (&src != &dst)
	{
		dst.name2 = src.name2;
	}

	// capa3
	if (src.capa3 == _T("") || src.capa3 == ("------"))
	{
		dst.capa3 = ("-");
	}
	else if (&src != &dst)
	{
		dst.capa3 = src.capa3;
	}

	// name3
	if (src.name3 == _T("") || src.name3 == _T("--------------"))
	{
		dst.name3 = _T("-");
	}
	else if (&src != &dst)
	{
		dst.name3 = src.name3;
	}

	// station
	if (src.station == _T("") || src.station == _T("------"))
	{
		dst.station = _T("-");
	}
	else if (&src != &dst)
	{
		dst.station = src.station;
	}

	// time
	if (src.time == _T("") || src.time == _T("-"))
	{
		dst.time = _T("0");
	}
	else if (&src != &dst)
	{
		dst.time = src.time;
	}

	// count
	if (src.count == _T("") || src.count == _T("-"))
	{
		dst.count = _T("0");
	}
	else if (&src != &dst)
	{
		dst.count = src.count;
	}
}

/*============================================================================*/
/*! CBusinessDiary

-# 衛星名の登録

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiary::AddSpaceName(CString name)
{
	if (name == _T("-"))
		return;
	vector<CString>::iterator itr;
	for (itr = mSpaceList.begin(); itr != mSpaceList.end(); itr++){
		if ((*itr) == name)
			return;
	}
	mSpaceList.push_back(name);
}

/*============================================================================*/
/*! CBusinessDiary

-# 衛星名の登録

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiary::JumpDate()
{
	SYSTEMTIME date;
	memset(&date, 0, sizeof(SYSTEMTIME));
	mDate.GetTime(&date);
	CTime ctime(date);
	CString str = ctime.Format("%Y-%m-%d");

	int count = m_DiaryList.GetItemCount();
	for (int i = 0; i < count; i++){
		CString text = m_DiaryList.GetItemText(i, 1);
		if (text == str){
			CRect rc;
			m_DiaryList.GetItemRect(0, &rc, LVIR_BOUNDS);// 行高さ
			int index = m_DiaryList.GetTopIndex();// 現在行
			if (i == index)
				return;
			int offset = rc.Height() * (i - index);// 行へオフセット計算
			CSize cs;
			cs.cx = 0;
			cs.cy = offset;
			if (offset) {
				m_DiaryList.Scroll(cs);
			}
			return;
		}
		else{
			DWORD t1 = _wtoi(text.Left(4)) << 16;
			t1 |= _wtoi(text.Mid(5, 2)) << 8;
			t1 |= _wtoi(text.Right(2));
			DWORD t2 = (date.wYear << 16) | (date.wMonth << 8) | date.wDay;
			if (t1 > t2){
				CRect rc;
				m_DiaryList.GetItemRect(0, &rc, LVIR_BOUNDS);// 行高さ
				int index = m_DiaryList.GetTopIndex();// 現在行
				int offset = rc.Height() * ((i-1) - index);// 行へオフセット計算
				CSize cs;
				cs.cx = 0;
				cs.cy = offset;
				if (offset) {
					m_DiaryList.Scroll(cs);
				}
				return;
			}
		}
	}
	m_DiaryList.EnsureVisible(count-1, FALSE);
}

/*============================================================================*/
/*! CBusinessDiary

-# 編集ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiary::OnBnClickedDiaryEditBtn()
{
	// 衛星名取得
	CString str;
	m_SatelliteCmb.GetLBText(m_SatelliteCmb.GetCurSel(), str);

	// 選択されている日誌取得
//	int idx = m_DiaryList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);

	int lidx = 0;
	int no = 0;
	int idx = -1;
	int count = 0;
	while ((idx = m_DiaryList.GetNextItem(idx, LVNI_ALL | LVNI_SELECTED)) != -1)
	{
		if (count == 0)
		{
			lidx = idx;
			CString strNo =	m_DiaryList.GetItemText(lidx, 0);
			no = atoi(CStringA(strNo));
		}
		else
		{
			m_DiaryList.SetItemState(idx, 0, LVIS_SELECTED);
		}
		count++;
	}

	// 編集ダイアログ表示
	CBusinessDiaryEdit dlg(this);
	dlg.SetSatellite(str);
	dlg.SetListPos(no);

	if (dlg.DoModal() != IDCANCEL)
	{
		CWaitCursor wait;
//		DisplayList(str);
		CreateDialyList();
	}
}


/*============================================================================*/
/*! CBusinessDiary

-# 印刷ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiary::OnBnClickedDiaryPrntBtn()
{
	CBusinessDiaryPrint dlg(this);

	SYSTEMTIME date;
	memset(&date, 0, sizeof(SYSTEMTIME));
	date.wYear = _wtoi(mStartOpeTime.Left(4));
	date.wMonth = _wtoi(mStartOpeTime.Mid(5, 2));
	date.wDay = _wtoi(mStartOpeTime.Right(2));
	CTime st = CTime(date);
	date.wYear = _wtoi(mEndOpeTime.Left(4));
	date.wMonth = _wtoi(mEndOpeTime.Mid(5, 2));
	date.wDay = _wtoi(mEndOpeTime.Right(2));
	CTime et = CTime(date);

	dlg.SetParameter(mSpaceList, st, et);
	dlg.DoModal();
}


/*============================================================================*/
/*! CBusinessDiary

-# CSV出力ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiary::OnBnClickedDiaryCsvBtn()
{
	UpdateData(TRUE);

	CTime ctime;
	mDate.GetTime(ctime);
	DWORD date = ctime.GetYear() << 16 | ctime.GetMonth() << 8 | ctime.GetDay();

	int i = m_SatelliteCmb.GetCurSel();
	CString mStation;
	m_SatelliteCmb.GetLBText(i, mStation);

#if 1
	FILE *fpform;
	FILE *fpcsv;

	UINT nStation = theApp.GetSelectStation();
	if (theApp.GetSelectStationMode() == eStation_5)
		nStation = 0;

	CString formDir = theApp.GetShareFilePath(eFileType_Diary_Form_Folder, nStation);
	CString csvDir = theApp.GetShareFilePath(eFileType_Diary_CSVOUT_Folder, nStation);

	int satCnt = 2;
	if(mStation == mAllText)
	{
		satCnt = m_SatelliteCmb.GetCount();
	}

	CString satname;
	char szRead[2048];
	char szWrite[2048];
	CString cbuf;
	CStringA wbuf;

	vector<CStringA> csvFormList;
	bool bOutput = false;

	for (int i = 1; i < satCnt; ++i)
	{
		csvFormList.clear();

		// 衛星名取得
		if (mStation == mAllText)
		{
			m_SatelliteCmb.GetLBText(i, satname);
		}
		else
		{
			m_SatelliteCmb.GetLBText(m_SatelliteCmb.GetCurSel(), satname);
		}

		CString strSatDir = _T("");

		// 衛星番号取得
		int satidx = GetIndexEqSatDBData(satname);
		if (satidx == -1)
		{
			strSatDir = NOT_SAT_DIR;
		}
		else
		{
			strSatDir = satname.MakeLower();
		}

		// CSV出力フォーマットファイルのフォルダがあるか確認
		CString strCSVFormFolder;
		strCSVFormFolder.Format(_T("%s%s\\"), (LPCTSTR)formDir, (LPCTSTR)strSatDir);
		if (PathIsDirectory(strCSVFormFolder) == FALSE)
		{
			MakeSureDirectoryPathExists((PCSTR)CStringA(strCSVFormFolder));
		}

		// CSV出力フォーマットファイル取得
		CString strCSVForm;
		strCSVForm.Format(_T("%s%s\\%s"), (LPCTSTR)formDir, (LPCTSTR)strSatDir, (LPCTSTR)PRINT_FORM_CSV_FILE);

		if (_wfopen_s(&fpform, strCSVForm, _T("r")) != 0)
		{
			CString mes;
			mes.Format(_T("以下のファイルが見つかりませんでした。\n%s"), (LPCTSTR)strCSVForm);
			AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
			continue;
		}

		while (fgets(szRead, static_cast<unsigned>(_countof(szRead)), fpform) != NULL)
		{
			cbuf = CServerFile::ConvertUnicode(szRead);
			csvFormList.push_back(CStringA(cbuf));
		}
		fclose(fpform);


		// 帳票書き込み

		stDiary st;
		int PrintYear = 0;
		int oldPrintYear = 0;
		int PrintMonth = 0;
		int oldPrintMonth = 0;
		int PrintDay;

		BOOL bOpen = FALSE;

		fpcsv = NULL;

		int dcnt = (int)mDialyDataListTmp.size();
		for (int d = 0; d < dcnt; ++d)
		{
			st = mDialyDataListTmp[d];
			if ( (satname.CompareNoCase(st.data[0].station) == 0) 
				|| (satname.CompareNoCase(st.data[1].station) == 0)
				|| (satname.CompareNoCase(st.data[2].station) == 0))
			{
				COleDateTime oleTime;
				oleTime.ParseDateTime(st.date);
				CTime cTime = CTime(oleTime.GetYear(), oleTime.GetMonth(), oleTime.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());

				// 年月日
				PrintYear = cTime.GetYear();
				PrintMonth = cTime.GetMonth();
				PrintDay = cTime.GetDay();

				// 曜日
				CString strDayOfWeek = DefDayOfWeek[cTime.GetDayOfWeek() - 1];

				if (!((PrintYear == oldPrintYear) && (PrintMonth == oldPrintMonth)))
				{
					if (fpcsv != NULL)
					{
						fclose(fpcsv);
						bOpen = FALSE;
					}

					CString strCsvFile;
					CString folder;
					folder.Format(_T("%s\\%s\\"), (LPCTSTR)csvDir, (LPCTSTR)strSatDir);
					if (PathFileExists(folder) == FALSE)	// 出力衛星フォルダが無い場合作成する。
					{
//						CreateDirectory(folder, NULL);
						MakeSureDirectoryPathExists((PCSTR)CStringA(folder));
					}

					if (satidx == -1)
					{
						strCsvFile.Format(_T("%s\\%s\\REP_%04d_%02d_%s.csv"), (LPCTSTR)csvDir, (LPCTSTR)strSatDir, PrintYear, PrintMonth, (LPCTSTR)satname);
					}
					else
					{
						strCsvFile.Format(_T("%s\\%s\\REP_%04d_%02d.csv"), (LPCTSTR)csvDir, (LPCTSTR)strSatDir, PrintYear, PrintMonth);
					}

					// CSV出力ファイルオープン
					if ((_wfopen_s(&fpcsv, strCsvFile, _T("w")) == 0) && (fpcsv != NULL))
					{
						// ヘッダー部書き込み
						sprintf_s(szWrite, "%s", (LPCSTR)csvFormList[0]);
						fputs(szWrite, fpcsv);

						sprintf_s(szWrite, "%s", (LPCSTR)csvFormList[1]);
						fputs(szWrite, fpcsv);

						sprintf_s(szWrite, csvFormList[2], (PrintYear - 1988));
						fputs(szWrite, fpcsv);

						sprintf_s(szWrite, csvFormList[3], PrintMonth);
						fputs(szWrite, fpcsv);

						sprintf_s(szWrite, "%s", (LPCSTR)csvFormList[4]);
						fputs(szWrite, fpcsv);

						bOpen = TRUE;
						bOutput = true;
					}
				}

				oldPrintYear = PrintYear;
				oldPrintMonth = PrintMonth;

				if (bOpen == TRUE)
				{
					for (int phaseT = 0; phaseT < 3; phaseT++)
					{
						// 各項のデータ無しまたは'-'文字を'-'連続文字列に変換する
						ExchgNoneData2Long(st.data[phaseT], st.data[phaseT]);
					}

					CString ref;
					AfxExtractSubString(ref, st.info3, 0, '\\');
					sprintf_s(szWrite, csvFormList[5], PrintDay, CStringA(strDayOfWeek), CStringA(st.data[0].weather), CStringA(st.data[0].capa1), CStringA(st.data[0].name1),
						CStringA(st.data[0].capa2), CStringA(st.data[0].name2), CStringA(st.data[0].station), _wtoi(st.data[0].time), _wtoi(st.data[0].count),
						CStringA(st.info1), CStringA(st.info2), CStringA(ref));
					fputs(szWrite, fpcsv);

					AfxExtractSubString(ref, st.info3, 1, '\\');
					sprintf_s(szWrite, csvFormList[6], CStringA(st.data[0].capa3), CStringA(st.data[0].name3), CStringA(ref));
					fputs(szWrite, fpcsv);

					AfxExtractSubString(ref, st.info3, 2, '\\');
					sprintf_s(szWrite, csvFormList[7], CStringA(st.data[1].weather), CStringA(st.data[1].capa1), CStringA(st.data[1].name1),
						CStringA(st.data[1].capa2), CStringA(st.data[1].name2), CStringA(st.data[1].station), _wtoi(st.data[1].time), _wtoi(st.data[1].count), CStringA(ref));
					fputs(szWrite, fpcsv);

					AfxExtractSubString(ref, st.info3, 3, '\\');
					sprintf_s(szWrite, csvFormList[8], CStringA(st.data[1].capa3), CStringA(st.data[1].name3), CStringA(ref));
					fputs(szWrite, fpcsv);

					AfxExtractSubString(ref, st.info3, 4, '\\');
					sprintf_s(szWrite, csvFormList[9], CStringA(st.data[2].weather), CStringA(st.data[2].capa1), CStringA(st.data[2].name1),
						CStringA(st.data[2].capa2), CStringA(st.data[2].name2), CStringA(st.data[2].station), _wtoi(st.data[2].time), _wtoi(st.data[2].count), CStringA(ref));
					fputs(szWrite, fpcsv);

					AfxExtractSubString(ref, st.info3, 5, '\\');
					sprintf_s(szWrite, csvFormList[10], CStringA(st.data[2].capa3), CStringA(st.data[2].name3), CStringA(ref));
					fputs(szWrite, fpcsv);
				}
			}
		}
		if(fpcsv != NULL)
		{
			fclose(fpcsv);
		}
	}

	if (bOutput)
	{
		AfxMessageBox(_T("CSVファイルを出力しました。"), MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		AfxMessageBox(_T("出力対象が見つかりませんでした。"), MB_OK | MB_ICONEXCLAMATION);
	}

#else
	UINT nStation = theApp.GetSelectStation();
	if (theApp.GetSelectStationMode() == eStation_5)
		nStation = 0;

	CString folder = theApp.GetShareFilePath(eFileType_Diary_CSVOUT_Folder, nStation);
	const TCHAR BASED_CODE szFilter[] = _T("CSV File(*.csv)|*.csv|All Files (*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("csv"), NULL, OFN_OVERWRITEPROMPT | OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);
	dlg.m_ofn.lpstrInitialDir = folder;
	if (dlg.DoModal() != IDOK)
		return;

	CString filenameout = dlg.GetPathName();

	FILE *fpout = NULL;

	TRY
	{
		// CSVファイルのオープン
		if (_wfopen_s(&fpout, filenameout, _T("w, ccs=UTF-8")) != 0){
			return;
		}

		CHeaderCtrl* pHeader = (CHeaderCtrl*)m_DiaryList.GetDlgItem(0);
		int maxcol = pHeader->GetItemCount();
		int count = m_DiaryList.GetItemCount();
		CString cbuf;
		CString text;
		for (int i = 0; i < count; i++){
			cbuf.Empty();
			for (int j = 0; j < maxcol; j++){
				text = m_DiaryList.GetItemText(i, j);
				if (cbuf.IsEmpty() == false){
					cbuf += _T(",");
				}
				cbuf += text;
			}
			cbuf += _T("\n");
			// 出力する
			fputws(cbuf, fpout);
		}
	}CATCH_ALL(e){
	}
	END_CATCH_ALL

		if (fpout != NULL)
			fclose(fpout);


#endif

}


/*============================================================================*/
/*! CBusinessDiary

-# 送信情報ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiary::OnBnClickedDiarySinfoBtn()
{
	CBusinessDiaryTInfo dlg;
#if 0
	SYSTEMTIME date;
	memset(&date, 0, sizeof(SYSTEMTIME));
	date.wYear = _wtoi(mStartOpeTime.Left(4));
	date.wMonth = _wtoi(mStartOpeTime.Mid(5, 2));
	date.wDay = _wtoi(mStartOpeTime.Right(2));
	CTime st = CTime(date);
	date.wYear = _wtoi(mEndOpeTime.Left(4));
	date.wMonth = _wtoi(mEndOpeTime.Mid(5, 2));
	date.wDay = _wtoi(mEndOpeTime.Right(2));
	CTime et = CTime(date);

	dlg.SetParameter(mSpaceList, st, et);
#endif
	dlg.DoModal();
}




void CBusinessDiary::OnDtnDatetimechangeDiaryStltDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	//JumpDate();
	PostMessage(WM_USER + 100, 0, 0);

	*pResult = 0;
}


void CBusinessDiary::OnCbnSelchangeDiaryStltCmb()
{
	UpdateData(TRUE);

	int i = m_SatelliteCmb.GetCurSel();
	CString str;
	m_SatelliteCmb.GetLBText(i, str);
	DisplayList(str);
}


/*============================================================================*/
/*! CBusinessDiary

-# ファイル-閉じるメニュー処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiary::OnMenuCloseClose()
{
	CWaitCursor wait;
	// 無線業務日誌をファイルに書き込み
	WriteDiaryData();
	// 無線業務日誌登録要求
	Shell_RegistRadiolog();

	OnOK();
}


LRESULT CBusinessDiary::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == (WM_USER + 100)){
		JumpDate();
		return TRUE;
	}
	if (message == (WM_USER + 200)){
		if (lParam == 0){
			// メインからサブ
			m_DiaryList2.ResetColumns(m_DiaryList.mHeaderSize);
		}
		if (lParam == 1){
			// サブからメイン
			m_DiaryList.ResetColumns(m_DiaryList2.mHeaderSize);
		}
		return TRUE;
	}

	return CDialogBase::WindowProc(message, wParam, lParam);
}

void CBusinessDiary::SelectListRow(int no)
{
	int sidx = -1;
	int idx;
	while ((sidx = m_DiaryList.GetNextItem(sidx, LVNI_ALL | LVNI_SELECTED)) != -1)
	{
		m_DiaryList.SetItemState(sidx, 0, LVIS_SELECTED);
	}

	int count = m_DiaryList.GetItemCount();
	for (idx = 0; idx < count; idx++)
	{
		CString strNo = m_DiaryList.GetItemText(idx, 0);
		if (strNo == _T("-"))
		{
			continue;
		}
		int searchNo = atoi(CStringA(strNo));
		if (searchNo == no)
		{
			break;
		}
	}

	m_DiaryList.SetItemState(idx, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_DiaryList.EnsureVisible(idx, FALSE);
}

/*============================================================================*/
/*! CBusinessDiary

-# 無線業務日誌取得シェルを殴る

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CBusinessDiary::Shell_GetRadiolog()
{
	BOOL ret = TRUE;
	CString arg = _T("");

#if 0
	// ファイルを削除する。
	CString filename = theApp.GetShareFilePath(eFileType_Diary_File, theApp.GetSelectStation());
	if (PathFileExistsW(filename) == TRUE)
	{
		DeleteFileW(filename);
	}
#endif

	// シェル実行
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = GET_RADIOLOG_SH;

	arg.Format(_T("%s"), (LPCTSTR)CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower());

	stParam.strRemShellParam.Format(arg);

	CDlgWaitCmd wait;
	wait.Open(_T("無線業務日誌"), _T("無線業務日誌取得中。\nしばらくお待ちください。"), false, true);
	int err = CSshShell::ExecShell(stParam);
	wait.Close();

	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("無線業務日誌取得"), _T("CBusinessDiary"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err == 0)
	{
		ret = TRUE;
	}
	else
	{
		AfxMessageBox(_T("無線業務日誌取得に失敗しました。"), MB_OK | MB_ICONWARNING);
		ret = FALSE;
	}

	return ret;
}

/*============================================================================*/
/*! CBusinessDiary

-# 無線業務日誌登録要求シェルを殴る

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CBusinessDiary::Shell_RegistRadiolog()
{
	CString arg = _T("");

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = REGIST_RADIOLOG_SH;

	arg.Format(_T("%s"), (LPCTSTR)CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower());

	stParam.strRemShellParam.Format(arg);

	int err = CSshShell::ExecShell(stParam);

	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("無線業務日誌登録要求"), _T("CBusinessDiary"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err != 0)
	{
		AfxMessageBox(_T("無線業務日誌登録に失敗しました。"), MB_OK | MB_ICONWARNING);
	}

	return TRUE;
}

/*============================================================================*/
/*! CBusinessDiary

-# パス中確認

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CBusinessDiary::IsPass()
{
	char	sz_status[256] = {0};

	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	// パス中は無線局業務日誌は開けない
	UINT sta = theApp.GetSelectStation();
	memset(sz_status, (char)NULL, sizeof(sz_status));

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(CTRL_PASS_INFO_NO));
	if (obs_adr != NULL)
	{
		strcpy_s(sz_status, sizeof(sz_status), obs_adr->sz_sts_name);
	}

	if ((strcmp(sz_status, CT_PASS_INFO_INIT) == 0) 			// 初期設定
		|| (strcmp(sz_status, CT_PASS_INFO_1WAY) == 0) 			// １ＷＡＹ捕捉中
		|| (strcmp(sz_status, CT_PASS_INFO_2WAY) == 0)			// ２ＷＡＹ捕捉中
		|| (strcmp(sz_status, CT_PASS_INFO_PASS_START1) == 0)	// パス開始(1WAY)
		|| (strcmp(sz_status, CT_PASS_INFO_PASS_START2) == 0)	// パス開始(2WAY)
		|| (strcmp(sz_status, CT_PASS_INFO_POL) == 0)			// 天頂通過監視領域内
		|| (strcmp(sz_status, CT_PASS_INFO_POSN) == 0)			// ANT POSN MODE 移行角検出
		|| (strcmp(sz_status, CT_PASS_INFO_EXTEND) == 0)		// パス延長
		)
	{
		CString title = _T("");
		GetWindowText(title);

		MessageBox(_T("パス中は無線業務日誌を表示できません。"), title, MB_OK | MB_ICONWARNING);
		return TRUE;
	}

	return FALSE;
}

/*============================================================================*/
/*! CBusinessDiary

-# RF情報ファイルの最終行に"ON"の有無を確認

@param  なし
@retval ON 有り TRUE or エラー時 / ON 無し FALSE;
*/
/*============================================================================*/
BOOL CBusinessDiary::CheckRFInfoFile()
{
	CString filename = theApp.GetShareFilePath(eFileType_Diary_Rf_File, theApp.GetSelectStation());

	BOOL ret = FALSE;
	FILE *fp = NULL;

	CString title = _T("");
	GetWindowText(title);

	TRY
	{
		// RF情報ファイルのオープン
		if ((_wfopen_s(&fp, filename, _T("r")) == 0) && (fp != NULL))
		{
			char szline[2048];
			CString cbuf = _T("");

			while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL)
			{
				cbuf = CServerFile::ConvertUnicode(szline);
			}

			if (cbuf.Find(_T("ON")) != -1)
			{
				MessageBox(_T("無線業務日誌編集禁止時間帯のため、\n無線業務日誌一覧画面を終了します。"), title, MB_OK | MB_ICONWARNING);
				ret = TRUE;
			}

			if (cbuf == _T(""))
			{
				MessageBox(_T("無線業務日誌が編集出来る時間帯か確認出来ないため、\n無線業務日誌一覧画面を終了します。"), title, MB_OK | MB_ICONWARNING);
				ret = TRUE;
			}
		}
		else
		{
			MessageBox(_T("無線業務日誌が編集出来る時間帯か確認出来ないため、\n無線業務日誌一覧画面を終了します。"), title, MB_OK | MB_ICONWARNING);
			ret = TRUE;
		}

	}CATCH_ALL(e){
	}
	END_CATCH_ALL

	if (fp != NULL)
		fclose(fp);

	return ret;
}

/*============================================================================*/
/*! CBusinessDiary

-# RFファイルのフォーマットチェック

@param  なし
@retval TRUE：正常 / FALSE：異常
*/
/*============================================================================*/
BOOL CBusinessDiary::CheckFormatRFFile()
{
	CString filename = theApp.GetShareFilePath(eFileType_Diary_Rf_File, theApp.GetSelectStation());

	BOOL ret = TRUE;
	FILE *fp = NULL;

	CString title = _T("");
	GetWindowText(title);

	TRY
	{
		// RF情報ファイルのオープン
		if ((_wfopen_s(&fp, filename, _T("r")) == 0) && (fp != NULL))
		{
			char szline[2048];
			CString cbuf = _T("");
			CString ccanma = _T("");
			CString cbufchk = _T("");

			while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL && ret == TRUE)
			{
				cbuf = CServerFile::ConvertUnicode(szline);
				// カンマの数チェック
				ccanma = cbuf;
				if (ccanma.Remove(_T(',')) != 4)
				{
					ret = FALSE;
				}
				// 文字数チェック
				for (int i = 0; i < 5 && ret == TRUE; i++)
				{
					AfxExtractSubString(cbufchk, cbuf, i, ',');
					if (cbufchk.GetLength() > 64)
					{
						ret = FALSE;
					}
				}
			}
		}
		else
		{
			CString strMsg;
			strMsg.Format(_T("RFファイルが開けないため、無線業務日誌一覧画面を終了します。\n%s"), filename);
			MessageBox(strMsg, title, MB_OK | MB_ICONWARNING);
			return FALSE;
		}

	}CATCH_ALL(e){
	}
	END_CATCH_ALL

	if (fp != NULL)
		fclose(fp);

	if (ret == FALSE)
	{
		CString strMsg;
		strMsg.Format(_T("RFファイルフォーマットが異なるため、無線業務日誌一覧画面を終了します。\n%s"), filename);
		MessageBox(strMsg, title, MB_OK | MB_ICONWARNING);
	}

	return ret;
}


void CBusinessDiary::WriteDiaryData()
{
	UINT nStation = theApp.GetSelectStation();
	if (theApp.GetSelectStationMode() == eStation_5)
		nStation = 0;
	CString filename = theApp.GetShareFilePath(eFileType_Diary_File, nStation);

	FILE *fp = NULL;

	if (mbEdit)
	{
		// 無線業務日誌更新時にバックアップファイル作成
		SYSTEMTIME time;
		GetSystemTime(&time);
		CTime ctime(time);
		CString temp = ctime.Format("%Y%m%d%H%M%S");
		CString backfile;
		backfile.Format(_T("%s.%s"), (LPCTSTR)filename, (LPCTSTR)temp);
		CopyFile(filename, backfile, FALSE);
		mbEdit = FALSE;
	}

	CStringA cbuf;

	TRY
	{
		// 日誌ファイルのオープン
		if ((_wfopen_s(&fp, filename, _T("wb")) != 0) || (fp == NULL))
		{
			return;
		}

		TCHAR szline[2048];
		wsprintf(szline, _T("Dmy1 Dmy2 %s %s Dmy3 Dmy4\n"), (LPCTSTR)CString(m_szMakeDate), (LPCTSTR)mStrRecCount);
		cbuf = CServerFile::ConvertEuc(szline);
		// ヘッダー書き込み
		fputs(cbuf, fp);

		int count = (int)mDialyDataList.size();
		for (int i = 0; i < count; ++i)
		{
			wsprintf(szline, _T("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n"),
				(LPCTSTR)mDialyDataList[i].no,
				(LPCTSTR)mDialyDataList[i].date,
				(LPCTSTR)mDialyDataList[i].data[0].weather,
				(LPCTSTR)mDialyDataList[i].data[0].capa1,
				(LPCTSTR)mDialyDataList[i].data[0].name1,
				(LPCTSTR)mDialyDataList[i].data[0].capa2,
				(LPCTSTR)mDialyDataList[i].data[0].name2,
				(LPCTSTR)mDialyDataList[i].data[0].capa3,
				(LPCTSTR)mDialyDataList[i].data[0].name3,
				(LPCTSTR)mDialyDataList[i].data[0].station,
				(LPCTSTR)mDialyDataList[i].data[0].time,
				(LPCTSTR)mDialyDataList[i].data[0].count,
				(LPCTSTR)mDialyDataList[i].data[1].weather,
				(LPCTSTR)mDialyDataList[i].data[1].capa1,
				(LPCTSTR)mDialyDataList[i].data[1].name1,
				(LPCTSTR)mDialyDataList[i].data[1].capa2,
				(LPCTSTR)mDialyDataList[i].data[1].name2,
				(LPCTSTR)mDialyDataList[i].data[1].capa3,
				(LPCTSTR)mDialyDataList[i].data[1].name3,
				(LPCTSTR)mDialyDataList[i].data[1].station,
				(LPCTSTR)mDialyDataList[i].data[1].time,
				(LPCTSTR)mDialyDataList[i].data[1].count,
				(LPCTSTR)mDialyDataList[i].data[2].weather,
				(LPCTSTR)mDialyDataList[i].data[2].capa1,
				(LPCTSTR)mDialyDataList[i].data[2].name1,
				(LPCTSTR)mDialyDataList[i].data[2].capa2,
				(LPCTSTR)mDialyDataList[i].data[2].name2,
				(LPCTSTR)mDialyDataList[i].data[2].capa3,
				(LPCTSTR)mDialyDataList[i].data[2].name3,
				(LPCTSTR)mDialyDataList[i].data[2].station,
				(LPCTSTR)mDialyDataList[i].data[2].time,
				(LPCTSTR)mDialyDataList[i].data[2].count,
				(LPCTSTR)mDialyDataList[i].info1,
				(LPCTSTR)mDialyDataList[i].info2,
				(LPCTSTR)mDialyDataList[i].info3);
			cbuf = CServerFile::ConvertEuc(szline);
			fputs(cbuf, fp);
		}
	}CATCH_ALL(e){
	}
	END_CATCH_ALL

	if (fp != NULL)
		fclose(fp);
}


/*============================================================================*/
/*! CBusinessDiary

-# 衛星固有DBより取得したデータより該当する衛星のインデックスを取得する

@param	str :衛星名
@retval	インデックス
*/
/*============================================================================*/
int CBusinessDiary::GetIndexEqSatDBData(CString str)
{
	int idx = -1;

	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	vector<CString> &saNameList = dba.GetSatNameList();

	int cnt = (int)saNameList.size();

	if (cnt == 0)
	{
		// サイズ0はデータベースオープンに失敗している。
		return -1;
	}

	for (int i = 0; i < cnt; ++i)
	{
		CString satname = saNameList[i];
		if (satname.CompareNoCase(str) == 0)
		{
			idx = i;
			break;
		}
	}

	return idx;
}


/*============================================================================*/
/*! CBusinessDiary

-# 無線業務日誌番号からリストインデックス位置を取得する

@param  無線業務日誌番号
@retval 無線業務日誌インデックス
*/
/*============================================================================*/
int CBusinessDiary::GetDiaryListIndex(int no)
{
	int idx;
	int searchNo;

	// 番号から無線業務日誌のインデックスを調べる
	for (idx = 0; idx < mDialyDataList.size(); idx++)
	{
		if (mDialyDataList[idx].no == _T("-"))
		{
			// -は何もしない
			continue;
		}

		searchNo = atoi(CStringA(mDialyDataList[idx].no));

		if (searchNo == no)
		{
			break;
		}
	}

	return idx;
}

/*============================================================================*/
/*! CBusinessDiary

-# 時間帯終了時刻取得処理	  
   
@param  日時
@retval 時間帯番号
*/
/*============================================================================*/
int CBusinessDiary::GetTimeConf(char *pszDate)
{
	int         i;
	time_t      lTime;
	struct tm   stTm;
	time_t      iTimeConf;
	char        szTimeConf[64];
	char        szBeforeTimeConf[64];

	char 		szNowTmpDate[64];

	memset(&stTm, 0, sizeof(stTm));
	memset(szTimeConf, '\0', sizeof(szTimeConf));
	memset(szNowTmpDate, '\0', sizeof(szNowTmpDate));

	if (pszDate == NULL) {
		time(&lTime);
//#ifdef _DEBUG		// ３分割時間エリアテスト
//		lTime = lTime + 16 * 3600;
//#endif
		if (gmtime_s(&stTm, &lTime) != 0)
		{
			return 0;
		}
		sprintf_s(szTimeConf, sizeof(szTimeConf), "%d-%02d-%02d %02d:%02d:%02d", stTm.tm_year + 1900, stTm.tm_mon + 1, stTm.tm_mday, stTm.tm_hour, stTm.tm_min, stTm.tm_sec);
		lTime = (time_t)CSatelliteData::ub_asc_to_dtime(szTimeConf);
	}
	else {
		lTime = (time_t)CSatelliteData::ub_asc_to_dtime(pszDate);
		if (localtime_s(&stTm, &lTime) != 0)
		{
			return 0;
		}
	}

	// 時間帯
	memset(szTimeConf, '\0', sizeof(szTimeConf));
	for (i = 0; i < 3; i++) {
		sprintf_s(szTimeConf, sizeof(szTimeConf), "%d-%02d-%02d %s", stTm.tm_year + 1900, stTm.tm_mon + 1, stTm.tm_mday, mEndTime[i]);
		iTimeConf = (time_t)CSatelliteData::ub_asc_to_dtime(szTimeConf);
		if (lTime < iTimeConf) {
			// 現在時刻が終わり時刻よりも前の場合
			if (i == 0) {
				// 第一時間ならば前日の時刻終わりにする
				if (pszDate == NULL) {
					sprintf_s(szTimeConf, sizeof(szTimeConf), "%d-%02d-%02d %s", stTm.tm_year + 1900, stTm.tm_mon + 1, stTm.tm_mday, mEndTime[2]);

					/* #2010-計算機換装2010_ctr CTR:HRTS-C-10-001-001対応 ↓ */
					/* 通算秒を一日戻す↓ */
					iTimeConf = (time_t)CSatelliteData::ub_asc_to_dtime(szTimeConf);
					iTimeConf -= 60 * 60 * 24;
					CSatelliteData::ub_dtime_to_asc((double)iTimeConf, szNowTmpDate);	/* ut 時刻情報をYYYY-MM-DD HH:MM:SS.ssss文字列に変換 */
					memset(m_szNowDate, '\0', sizeof(m_szNowDate));
					strncpy_s(m_szNowDate, sizeof(m_szNowDate), szNowTmpDate, 19);	/* 秒の少数点を削除 */
					/* #2010-計算機換装2010_ctr CTR:HRTS-C-10-001-001対応 ↑ */
				}
			}
			else {
				if (pszDate == NULL) {
					strcpy_s(m_szNowDate, sizeof(m_szNowDate), szBeforeTimeConf);
				}
			}
			break;
		}
		strcpy_s(szBeforeTimeConf, sizeof(szBeforeTimeConf), szTimeConf);
	}
	return(i);
}

/*============================================================================*/
/*! CBusinessDiary

-# 無線業務日誌作成／更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiary::MakeReport(vector<stDiary>& dialyDataList)
{
	time_t	iNowTime, iDateLimit, iRfSrchDate, iOnTime;
	char	szSrchDate[24];
	char    szOnDate[24], szOffDate[24];
	char	szCurDay[24];
	char    szBeforeDay[24];
	char    szRecord[512];
	int     iNo;
	size_t	pos = 0;
	bool	bAborRftList = false;

	vector<stSatRfInfo> satRfInfoList;

	memset(szSrchDate, '\0', sizeof(szSrchDate));
	memset(szOnDate, '\0', sizeof(szOnDate));
	memset(szOffDate, '\0', sizeof(szOffDate));
	memset(szCurDay, '\0', sizeof(szCurDay));
	memset(szBeforeDay, '\0', sizeof(szBeforeDay));
	memset(szRecord, '\0', sizeof(szRecord));
	satRfInfoList.clear();

	iNowTime = (time_t)CSatelliteData::ub_asc_to_dtime(m_szNowDate);	/* 現在の日付を通算秒に変換 */
	iDateLimit = iNowTime - ((mDelSpan + 1)*(60 * 60 * 24));			/* 現在の日付から保存間後の通算秒を算出 */

	iNo = 0;

	// 有効行をカウントしなおし、番号振りなおし
	for (int i = 0; i < dialyDataList.size(); i++)
	{
		// 残すデータである
		if (dialyDataList[i].no != _T("-"))
		{
			iNo++;
			dialyDataList[i].no.Format(_T("%d"), iNo);		// 番号振りなおし
		}
	}

	if (m_RfInfoList.size() > 0)
	{
		SetEditFlg(TRUE);								// RF情報を追加のため、日誌更新

		// 新しいRF情報を無線業務日誌に追加
		strncpy_s(szBeforeDay, sizeof(szBeforeDay), m_RfInfoList[0].szDate, 10);
		for (pos = 0; pos < m_RfInfoList.size(); pos++)
		{
			// 違う日付になったら
			memset(szCurDay, '\0', sizeof(szCurDay));
			strncpy_s(szCurDay, sizeof(szCurDay), m_RfInfoList[pos].szDate, 10);
			if (strcmp(szCurDay, szBeforeDay) && strlen(szOnDate) && strlen(szOffDate) &&
				CSatelliteData::ub_asc_to_dtime(szOnDate) <= CSatelliteData::ub_asc_to_dtime(szOffDate))
			{
				// 違う日付に変わったら、無線業務日誌に反映する
				for (int i = 0; i < satRfInfoList.size(); i++)
				{
					iNo++;

					stDiary temp;
					temp.no.Format(_T("%d"), iNo);
					temp.date.Format(_T("%s"), (LPCTSTR)CString(szBeforeDay));
					for (int u = 0; u < 3; ++u)
					{
						temp.data[u].weather = _T("-");
						temp.data[u].capa1 = _T("-");
						temp.data[u].name1 = _T("-");
						temp.data[u].capa2 = _T("-");
						temp.data[u].name2 = _T("-");
						temp.data[u].capa3 = _T("-");
						temp.data[u].name3 = _T("-");

						time_t	iaMin = satRfInfoList[i].iaTime[u] / 60;	// 秒を分にする
						if (satRfInfoList[i].iaTime[u] % 60)
						{
							iaMin++;
						}

						if (iaMin > 0)
						{
							temp.data[u].station = CString(satRfInfoList[i].szaSatName);
						}
						else
						{
							temp.data[u].station = _T("-");
						}
						temp.data[u].time.Format(_T("%I64d"), iaMin);
						temp.data[u].count.Format(_T("%d"), satRfInfoList[i].iaCnt[u]);
					}
					temp.info1 = _T("");
					temp.info2 = _T("");
					temp.info3 = _T("");

					dialyDataList.push_back(temp);
				}

				// 次の日付になるため初期化する
				memset(szBeforeDay, '\0', sizeof(szBeforeDay));
				strcpy_s(szBeforeDay, sizeof(szBeforeDay), szCurDay);
				memset(szOnDate, '\0', sizeof(szOnDate));
				memset(szOffDate, '\0', sizeof(szOffDate));
				satRfInfoList.clear();
			}

			// ON情報読み込み
			if (m_RfInfoList[pos].bOn)
			{
				strcpy_s(szOnDate, sizeof(szOnDate), m_RfInfoList[pos].szDate);
				iOnTime = (time_t)CSatelliteData::ub_asc_to_dtime(szOnDate);
				if (iNowTime < iOnTime)
				{
					bAborRftList = true;
					break;
				}
			}
			// OFF情報読み込み
			else
			{
				strcpy_s(szOffDate, sizeof(szOffDate), m_RfInfoList[pos].szDate);
				SetRfInfo(m_RfInfoList[pos].szSatName, m_RfInfoList[pos].szColim, m_RfInfoList[pos].szFreq, szOnDate, szOffDate, satRfInfoList);
			}
		}

		// 最後の日付のRF情報を日誌に反映する
		if (strlen(szBeforeDay))
		{
			for (int i = 0; i < satRfInfoList.size(); i++)
			{
				iNo++;

				stDiary temp;
				temp.no.Format(_T("%d"), iNo);
				temp.date.Format(_T("%s"), (LPCTSTR)CString(szBeforeDay));
				for (int u = 0; u < 3; ++u)
				{
					temp.data[u].weather = _T("-");
					temp.data[u].capa1 = _T("-");
					temp.data[u].name1 = _T("-");
					temp.data[u].capa2 = _T("-");
					temp.data[u].name2 = _T("-");
					temp.data[u].capa3 = _T("-");
					temp.data[u].name3 = _T("-");

					time_t	iaMin = satRfInfoList[i].iaTime[u] / 60;	// 秒を分にする
					if (satRfInfoList[i].iaTime[u] % 60)
					{
						iaMin++;
					}

					if (iaMin > 0)
					{
						temp.data[u].station = CString(satRfInfoList[i].szaSatName);
					}
					else
					{
						temp.data[u].station = _T("-");
					}
					temp.data[u].time.Format(_T("%I64d"), iaMin);
					temp.data[u].count.Format(_T("%d"), satRfInfoList[i].iaCnt[u]);
				}
				temp.info1 = _T("-");
				temp.info2 = _T("-");
				temp.info3 = _T("-");

				dialyDataList.push_back(temp);
			}
		}

		// 更新時間の更新
		size_t	size = m_RfInfoList.size();
		size_t	lastpos;
		bool	bUpdateMakeDate = false;
		if (bAborRftList == false)
		{
			// 新しいRF情報を全部追加時のリスト位置
			lastpos = size - 1;
			bUpdateMakeDate = true;
		}
		else if (pos > 0 && pos < size)
		{
			// 新しいRF情報を途中まで追加時のリスト位置
			lastpos = pos - 1;
			bUpdateMakeDate = true;
		}

		if (bUpdateMakeDate == true && lastpos >= 0 && lastpos < size)
		{
			time_t	iOffTime = (time_t)CSatelliteData::ub_asc_to_dtime(m_RfInfoList[lastpos].szDate);
			time_t	iHeaderTime = (time_t)CSatelliteData::ub_asc_to_dtime(m_szMakeDate);
			if (iHeaderTime < iNowTime)
			{
				if (iOffTime <= iNowTime)
				{
					strcpy_s(m_szMakeDate, sizeof(m_szMakeDate), m_szNowDate);
				}
				else
				{
					// 勤務跨ぎの場合は、RF情報ファイルの最後のOFF時刻にする
					strcpy_s(m_szMakeDate, sizeof(m_szMakeDate), m_RfInfoList[lastpos].szDate);
				}
			}
		}
	}

	mDialyDataList.clear();								// リストをいったん消す
	iNo = 0;

	// 保存期間内のデータのみ残す
	for (int i = 0; i < dialyDataList.size(); i++)
	{
		strcpy_s(szSrchDate, sizeof(szSrchDate), CStringA(dialyDataList[i].date));
		strcat_s(szSrchDate, sizeof(szSrchDate), " ");
		strcat_s(szSrchDate, sizeof(szSrchDate), "00:00:00");

		iRfSrchDate = (time_t)CSatelliteData::ub_asc_to_dtime(szSrchDate);	/* 日付を通算秒に変換 */
		if (iRfSrchDate > iDateLimit)
		{
			// 残すデータである
			if (dialyDataList[i].no != _T("-"))
			{
				iNo++;
				dialyDataList[i].no.Format(_T("%d"), iNo);		// 番号振りなおし
			}
			mDialyDataList.push_back(dialyDataList[i]);
		}
		else
		{
			// 削除データである
			SetEditFlg(TRUE);									// 日誌更新
		}
	}

	// 有効レコード数更新
	mStrRecCount.Format(_T("%d"), iNo);
}

/*============================================================================*/
/*! CBusinessDiary

-# RF ON/OFF時間設定処理

@param  
@retval 
*/
/*============================================================================*/
void CBusinessDiary::SetRfInfo(char *pszSatName, char *pszColim, char *pszFreq, char *pszOnDate, char *pszOffDate, vector<stSatRfInfo>& satRfInfoList)
{
	int     iOnTimeConf;
	time_t	iTime;

	iOnTimeConf = GetTimeConf(pszOnDate);
	iTime = (time_t)CSatelliteData::ub_asc_to_dtime(pszOffDate) - (time_t)CSatelliteData::ub_asc_to_dtime(pszOnDate);

	char	szSatName[24];
	memset(szSatName, '\0', sizeof(szSatName));

	if ((pszSatName[0] == ' ') || (strcmp(pszSatName, "NA") == 0))
	{
		strcpy_s(szSatName, sizeof(szSatName), pszFreq);
	}
	else
	{
		strcpy_s(szSatName, sizeof(szSatName), pszSatName);
	}

	// リストに衛星がすでにあればそのデータを使う、なければ追加
	for (int i = 0; i < satRfInfoList.size(); i++)
	{
		if (strcmp(satRfInfoList[i].szaSatName, szSatName) == 0)
		{
			// 衛星があったため、時間と回数を加算
			satRfInfoList[i].iaTime[iOnTimeConf] += iTime;
			satRfInfoList[i].iaCnt[iOnTimeConf]++;
			return;
		}
	}

	// 新しい衛星と時間、回数を追加
	stSatRfInfo rfInfo;
	memset(rfInfo.szaSatName, '\0', sizeof(rfInfo.szaSatName));
	for (int i = 0; i < 3; i++)
	{
		rfInfo.iaTime[i] = 0;
		rfInfo.iaCnt[i] = 0;
	}
	strcpy_s(rfInfo.szaSatName, sizeof(rfInfo.szaSatName), szSatName);
	rfInfo.iaTime[iOnTimeConf] = iTime;
	rfInfo.iaCnt[iOnTimeConf] = 1;
	satRfInfoList.push_back(rfInfo);
}

/*============================================================================*/
/*! CBusinessDiary

-# RF情報ファイルを読み込んでリストに展開

@param  なし
@retval true:追加した / false:追加しなかった
*/
/*============================================================================*/
bool CBusinessDiary::ReadRFInfoFile()
{
	CString filename = theApp.GetShareFilePath(eFileType_Diary_Rf_File, theApp.GetSelectStation());
	FILE *fp = NULL;
	bool	bAppend = false;

	m_RfInfoList.clear();

	TRY
	{
		// RF情報ファイルのオープン
		if ((_wfopen_s(&fp, filename, _T("r")) == 0) && (fp != NULL))
		{
			time_t     iOnTime, iAllReadyTime;

			iAllReadyTime = (time_t)CSatelliteData::ub_asc_to_dtime(m_szMakeDate);

			char szline[2048];
			CString cbuf = _T("");
			CString str;
			CStringA strA;
			stRfInfo rfInfo;
			memset(&rfInfo, 0, sizeof(rfInfo));

			while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL)
			{
				cbuf = CServerFile::ConvertUnicode(szline);
				cbuf.TrimRight(_T("\n"));

				if (AfxExtractSubString(str, cbuf, 0, ','))
				{
					// 日時
					strcpy_s(rfInfo.szDate, sizeof(rfInfo.szDate), CStringA(str));
				}

				if (AfxExtractSubString(str, cbuf, 1, ','))
				{
					// ON/OFF
					if (str == _T("ON"))
					{
						rfInfo.bOn = true;
					}
					else
					{
						rfInfo.bOn = false;
					}
				}

				if (AfxExtractSubString(str, cbuf, 2, ','))
				{
					// 周波数
					strcpy_s(rfInfo.szFreq, sizeof(rfInfo.szFreq), CStringA(str));
				}

				if (AfxExtractSubString(str, cbuf, 3, ','))
				{
					// 衛星名
					strcpy_s(rfInfo.szSatName, sizeof(rfInfo.szSatName), CStringA(str));
				}

				if (AfxExtractSubString(str, cbuf, 4, ','))
				{
					// COLIM
					strcpy_s(rfInfo.szColim, sizeof(rfInfo.szColim), CStringA(str));
				}

				// 時刻判定
				if (bAppend == false && rfInfo.bOn == true)
				{
					iOnTime = (time_t)CSatelliteData::ub_asc_to_dtime(rfInfo.szDate);
					if (iOnTime > iAllReadyTime)
					{
						bAppend = true;
					}
				}

				if (bAppend)
				{
					// 新しいRF情報を追加
					m_RfInfoList.push_back(rfInfo);
					// 新しい衛星名があれば追加
					AddSpaceName(CString(rfInfo.szSatName));
				}
			}

#if 0
			// 更新時間の更新
			size_t size = m_RfInfoList.size();
			if (size > 0)
			{
				time_t	iNowTime = (time_t)CSatelliteData::ub_asc_to_dtime(m_szNowDate);
				time_t	iOffTime = (time_t)CSatelliteData::ub_asc_to_dtime(m_RfInfoList[size - 1].szDate);
				time_t	iHeaderTime = (time_t)CSatelliteData::ub_asc_to_dtime(m_szMakeDate);
				if (iHeaderTime < iNowTime)
				{
					if (iOffTime <= iNowTime)
					{
						strcpy_s(m_szMakeDate, sizeof(m_szMakeDate), m_szNowDate);
					}
					else
					{
						// 勤務跨ぎの場合は、RF情報ファイルの最後のOFF時刻にする
						strcpy_s(m_szMakeDate, sizeof(m_szMakeDate), m_RfInfoList[size - 1].szDate);
					}
				}
			}
#endif
		}
	}CATCH_ALL(e){
	}
	END_CATCH_ALL

		if (fp != NULL)
			fclose(fp);

	return bAppend;
}


void CBusinessDiary::OnClose()
{
//	CDialogBase::OnClose();
	CString title;
	GetWindowText(title);

	if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("終了します。\nよろしいですか？"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return;

	CWaitCursor wait;
	// 無線業務日誌をファイルに書き込み
	WriteDiaryData();
	// 無線業務日誌登録要求
	Shell_RegistRadiolog();

	OnOK();
}


void CBusinessDiary::OnLvnItemchangedDiaryList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// ADD 2018.06.04 >>>
	GetDlgItem(IDC_DIARY_EDIT_BTN)->EnableWindow((m_DiaryList.GetSelectedCount() == 0)?FALSE:TRUE);
	// ADD 2018.06.04 <<<

	*pResult = 0;
}
