// DetailViewDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "DetailViewDlg.h"
#include "afxdialogex.h"

#include "SatelliteData.h"
#include "HookMsgBox.h"

// CDetailViewDlg ダイアログ

IMPLEMENT_DYNAMIC(CDetailViewDlg, CDialogBase)

CDetailViewDlg::CDetailViewDlg(CWnd* pParent /*=NULL*/)
: CDialogBase(CDetailViewDlg::IDD, pParent)
	, m_strSelectFile(_T(""))
{
	mTitleText.Empty();
	mSelectFileName.Empty();
	mSaveCx = mSaveCy = 0;
}

CDetailViewDlg::~CDetailViewDlg()
{
	if (mListFont.GetSafeHandle()){
		mListFont.DeleteObject();
	}
}

void CDetailViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_FILENAME, m_strSelectFile);
	DDX_Control(pDX, IDC_LIST_FILE, m_ListFile);
}


BEGIN_MESSAGE_MAP(CDetailViewDlg, CDialogBase)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_MENU_OPEN, &CDetailViewDlg::OnMenuOpen)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CDetailViewDlg::OnMenuClose)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDetailViewDlg メッセージ ハンドラー

void CDetailViewDlg::OnMenuOpen()
{
	// 現在のファイル名から拡張子を取得する
	CString strExt = mSelectFileName.Right(mSelectFileName.GetLength() - mSelectFileName.ReverseFind(_T('.')) - 1);
	CString folder = mSelectFileName.Mid(0, mSelectFileName.ReverseFind(_T('\\')));

	// ファイルを開く
	CString filter;
	if (strExt.CompareNoCase(_T("pln")) == 0)
	{
		filter = _T("Plan files (*.pln)|*.pln||");
	}
	else if (strExt.CompareNoCase(_T("prd")) == 0)
	{
		filter = _T("Pred files (*.prd)|*.prd||");
	}
	else if (strExt.CompareNoCase(_T("anp")) == 0)
	{
		filter = _T("Anp files (*.anp)|*.anp||");
	}

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	dlg.m_ofn.lpstrInitialDir = folder;
	int err = 0;

	if (dlg.DoModal() == IDOK)
	{
		CString strFilePathNew = dlg.GetPathName();
		CString strExtNew = strFilePathNew.Right(strFilePathNew.GetLength() - strFilePathNew.ReverseFind(_T('.')) - 1);
		// 拡張子が一致していること
		if (strExtNew.CompareNoCase(strExt) == 0)
		{
			mSelectFileName = strFilePathNew;
			if (mSelectFileName.IsEmpty() == false){
				m_strSelectFile = mSelectFileName;
				if (ReadFile(m_strSelectFile) == false)
				{
					m_strSelectFile = _T("ファイルエラー");
				}
				else
				{
					UpdateData(FALSE);
					AutoSizeColumns();
				}
			}
			else{
				m_strSelectFile = _T("ファイルエラー");
			}
		}
		else
		{
			MessageBox(_T("ファイル種類が違うため、開けませんでした"), MB_OK);
		}
	}

#if 1
	// 開くダイアログ表示後、メニューが白くなる事への対応
	DrawMenuBar();
#else
	ShowWindow(SW_HIDE);
	ShowWindow(SW_SHOW);
#endif
}

void CDetailViewDlg::OnMenuClose()
{
	//	OnOK();
	ShowWindow(SW_HIDE);
}

BOOL CDetailViewDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	mSaveCx = mSaveCy = 0;
	mControl.clear();

	SetControlInfo(IDC_STATIC_FILE, DVANCHORE_LEFTTOP);
	SetControlInfo(IDC_STATIC_FILENAME, DVANCHORE_LEFTTOP);
	SetControlInfo(IDC_LIST_FILE, DVANCHORE_LEFTTOP | DVRESIZE_BOTH);
	
	ModifyStyleEx(0, WS_EX_DLGMODALFRAME);

//	// システムコマンドSC_CLOSEの使用禁止
//	CMenu* pSysMenu = GetSystemMenu(FALSE);
//	if (pSysMenu)
//		pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

	if (mTitleText.IsEmpty() == false){
		SetWindowText(mTitleText);
	}

	CFont defFont;
	defFont.CreateStockObject(SYSTEM_FIXED_FONT);
	LOGFONT lf;
	if (defFont.GetLogFont(&lf)){
		//lf.lfWeight = FW_NORMAL;
		//lf.lfHeight = 24;

		if (mListFont.GetSafeHandle()){
			mListFont.DeleteObject();
		}
		mListFont.CreateFontIndirect(&lf);
	}

	// クライアント領域の取得
	CRect rectClient;
	GetClientRect(rectClient);

	// 拡張スタイルの取得
	DWORD  dwStyle = ListView_GetExtendedListViewStyle(m_ListFile.m_hWnd);
	//  拡張スタイルのセット
	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= LVS_EX_FULLROWSELECT/*|LVS_EX_GRIDLINES*/ | 0x00010000/*LVS_EX_DOUBLEBUFFER*/;
	//  新しい拡張スタイルを設定
	ListView_SetExtendedListViewStyle(m_ListFile.m_hWnd, dwStyle);
	m_ListFile.InsertColumn(0, _T("99999:"), LVCFMT_LEFT, 50);
	m_ListFile.InsertColumn(1, _T("WWWWWWWWWW"), LVCFMT_LEFT, 100);
	m_ListFile.SetBkColor(RGB(255, 255, 255));
	if (mListFont.GetSafeHandle()){
		m_ListFile.SetFont(&mListFont);
	}

	if (mSelectFileName.IsEmpty() == false){
		m_strSelectFile = mSelectFileName;
		if (ReadFile(m_strSelectFile) == false)
		{
			m_strSelectFile = _T("ファイルエラー");
		}
	}
	else{
		m_strSelectFile = _T("ファイルエラー");
	}
	UpdateData(FALSE);
	AutoSizeColumns();

	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! 詳細表示

-# カラムの自動サイズ調整

@param	col		カラム位置のカラムサイズを自動に調整します
@retval

*/
/*============================================================================*/
void CDetailViewDlg::AutoSizeColumns(int col /*=-1*/)
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)m_ListFile.GetDlgItem(0);

	m_ListFile.SetRedraw(false);
	int mincol = col < 0 ? 0 : col;
	int maxcol = col < 0 ? pHeader->GetItemCount() - 1 : col;
	for (col = mincol; col <= maxcol; col++){
		m_ListFile.SetColumnWidth(col, LVSCW_AUTOSIZE);
		int wc1 = m_ListFile.GetColumnWidth(col);
		m_ListFile.SetColumnWidth(col, LVSCW_AUTOSIZE_USEHEADER);
		int wc2 = m_ListFile.GetColumnWidth(col);
		int wc = MAX(50, MAX(wc1, wc2));
		m_ListFile.SetColumnWidth(col, wc);
	}
	m_ListFile.SetRedraw(true);
}

/*============================================================================*/
/*! 詳細表示

-# ファイル項目の登録

@param	nItem		アイテム番号
@param	nSubItem	サブアイテム番号
@param	strItem		設定文字列
@param	dwParam		ユーザ値
@retval

*/
/*============================================================================*/
int CDetailViewDlg::AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam/*=0*/)
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
		return m_ListFile.InsertItem(&lv);
	return m_ListFile.SetItem(&lv);
}

/*============================================================================*/
/*! 詳細表示

-# リサイズ時のコントロール情報を設定する

@param	id		コントロールID
@param	anchore	リサイズ時の識別子

@retval void
*/
/*============================================================================*/
void CDetailViewDlg::SetControlInfo(WORD id, WORD anchore)
{
	if (anchore == DVANCHORE_LEFTTOP)
		return;

	DWORD info = id | (anchore << 16);
	mControl.push_back(info);
}


void CDetailViewDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogBase::OnSize(nType, cx, cy);

	if (nType == SIZE_MINIMIZED)
		return;

	if (GetSafeHwnd() == NULL)
		return;

	if (mControl.size() == 0)
		return;

	if (mSaveCx == 0){
		mSaveCx = cx;
		mSaveCy = cy;
		return;
	}

	int dx = cx - mSaveCx;
	int dy = cy - mSaveCy;

	CRect rect, rectClient;
	GetClientRect(rectClient);

	CWnd *pWnd;
	DWORD info;
	WORD anchore;

	vector<DWORD>::iterator itr;
	for (itr = mControl.begin(); itr != mControl.end(); itr++){
		info = (*itr);
		pWnd = GetDlgItem(LOWORD(info));
		if (!pWnd){
			continue;
		}

		if (!HIWORD(info))
			continue;

		anchore = HIWORD(info);
		pWnd->GetWindowRect(rect);
		ScreenToClient(rect);

		if (anchore & DVRESIZE_HOR)
			rect.right += dx;
		else if (anchore & DVANCHORE_RIGHT)
			rect.OffsetRect(dx, 0);

		if (anchore & DVRESIZE_VER)
			rect.bottom += dy;
		else if (anchore & DVANCHORE_BOTTOM)
			rect.OffsetRect(0, dy);

		pWnd->MoveWindow(rect);
	}

	mSaveCx = cx;
	mSaveCy = cy;
}

/*============================================================================*/
/*! 詳細表示

-# ファイルの読み込み

@param	filename		読み込みファイル名
@retval

*/
/*============================================================================*/
bool CDetailViewDlg::ReadFile(CString filename)
{
	FILE* fp;
	bool bRead = false;

	m_ListFile.DeleteAllItems();

	if (_wfopen_s(&fp, filename, _T("r")) != 0)
	{
		return FALSE;
	}

	if (fp == NULL)
		return FALSE;

	TRY
	{
		bRead = true;

		char    szline[2048];	    /* レコード読み込みバッファ       */
		CString cbuf, str;
		int item = 0;
		while (!feof(fp))
		{
			/* １行読み込み */
			if (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) == NULL) {
				break;
			}
			else {
				cbuf = CServerFile::ConvertUnicode(szline);
				cbuf.TrimRight(_T("\n"));
				str.Format(_T("%04d:"), item + 1);
				AddItem(item, 0, str);
				AddItem(item, 1, cbuf);
				item++;
			}
		}
		fclose(fp);
	} CATCH_ALL(e){
		if (bRead == true)
			fclose(fp);
		return false;
	}
	END_CATCH_ALL

	return true;
}


/*============================================================================*/
/*! ColorListCtrl.cpp

-# 詳細リスト
*/
/*============================================================================*/
// ColorListCtrl.cpp : 実装ファイル
//


// CColorListCtrl

IMPLEMENT_DYNAMIC(CColorListCtrl, CListCtrl)

/*============================================================================*/
/*! CColorListCtrl

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CColorListCtrl::CColorListCtrl()
{

}

/*============================================================================*/
/*! CColorListCtrl

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CColorListCtrl::~CColorListCtrl()
{
}


BEGIN_MESSAGE_MAP(CColorListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CColorListCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()



// CColorListCtrl メッセージ ハンドラー

/*============================================================================*/
/*! CColorListCtrl

-# カスタムドロー時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし
*/
/*============================================================================*/
void CColorListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

	switch (lplvcd->nmcd.dwDrawStage) {
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
	{
		int		nItem = (int)lplvcd->nmcd.dwItemSpec;
		int		nSubItem = lplvcd->iSubItem;
		lplvcd->clrTextBk = RGB(255, 255, 255);
		lplvcd->clrText = RGB(0, 0, 0);
		*pResult = CDRF_DODEFAULT;
	}
	break;
	default:
		*pResult = CDRF_DODEFAULT;
		break;
	}
}


void CDetailViewDlg::PostNcDestroy()
{
	//delete this;

	CDialogBase::PostNcDestroy();
}


void CDetailViewDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 200;
	lpMMI->ptMinTrackSize.y = 200;

	CDialogBase::OnGetMinMaxInfo(lpMMI);
}


void CDetailViewDlg::OnClose()
{
	CString title;
	GetWindowText(title);

	if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("終了します。\nよろしいですか？"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return;

	//	OnOK();
	ShowWindow(SW_HIDE);
}

