/*============================================================================*/
/*! OperationAccount.cpp

-# 運用アカウント設定
*/
/*============================================================================*/
// OperationAccount.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ControlManagement.h"
#include "OperationAccount.h"
#include "afxdialogex.h"


// COperationAccount ダイアログ

IMPLEMENT_DYNAMIC(COperationAccount, CDialogEx)

COperationAccount::COperationAccount(CWnd* pParent /*=NULL*/)
	: CDialogEx(COperationAccount::IDD, pParent)
{

}

COperationAccount::~COperationAccount()
{
}

void COperationAccount::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, mList);
}


BEGIN_MESSAGE_MAP(COperationAccount, CDialogEx)
	ON_NOTIFY(HDN_DIVIDERDBLCLICK, 0, &COperationAccount::OnHdnDividerdblclickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &COperationAccount::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &COperationAccount::OnBnClickedButton2)
END_MESSAGE_MAP()


// COperationAccount メッセージ ハンドラー


BOOL COperationAccount::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	DWORD  dwStyle = ListView_GetExtendedListViewStyle(mList.m_hWnd);
	//  拡張スタイルのセット
	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= /*LVS_EX_CHECKBOXES | */LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | 0x00010000/*LVS_EX_DOUBLEBUFFER*/;
	//  新しい拡張スタイルを設定
	ListView_SetExtendedListViewStyle(mList.m_hWnd, dwStyle);

	//=========================================================================
	// コントロールで使用するイメージリストの作成
	CDC* pdc = GetDC();
	HIMAGELIST hImageList = CreateImageList(pdc);
	m_ImageList.Attach(hImageList);
	mList.SetImageList(&m_ImageList, TVSIL_STATE);
	ReleaseDC(pdc);

	const CString mHeader[] = {
		_T("ユーザ名"), _T("内之浦20m局"), _T("内之浦34m局"), _T("臼田64m局"), _T("臼田54m局"),
	};
	for (int i = 0; i<sizeof(mHeader) / sizeof(mHeader[0]); i++){
		mList.InsertColumn(i, mHeader[i], LVCFMT_LEFT, 100);
	}

	vector<stUserInfo> mUserList;
	theApp.GetLocalUserList(mUserList);
	vector<stAccount> mAcList;

	bool	bUpdateFile = false;

	CFileAccess& ac = theApp.GetFileAccess();
	if (ac.ReadOperationAccount(mAcList) == false){
		// 運用アカウントファイルが存在しないので、ローカルユーザで初期化する
		mAccountList.clear();
		vector<stUserInfo>::iterator itr;
		for (itr = mUserList.begin(); itr != mUserList.end(); itr++){
			stAccount data;
			data.user = (*itr).user;
			data.usersid = (*itr).usersid;
			data.usc20 = false;
			data.usc34 = false;
			data.udsc64 = false;
			data.xdsc54 = false;
			data.enable = true;
			data.mycomputer = true;
			data.dummy = 0;
			mAccountList.push_back(data);
		}
	}
	else{
		// すでに運用アカウントが存在したので、ローカルユーザとの整合性チェックを行う
		vector<stAccount>::iterator itrdata;
		vector<stUserInfo>::iterator itr;
		mAccountList.clear();
		// ファイル内のデータを登録する
		for (itrdata = mAcList.begin(); itrdata != mAcList.end(); itrdata++){
			for (itr = mUserList.begin(); itr != mUserList.end(); itr++){
				if ((*itr).usersid.Mid(0, (*itr).usersid.GetLength() - 5) != (*itrdata).usersid.Mid(0, (*itr).usersid.GetLength() - 5)){
					break;
				}
				if ((*itr).usersid == (*itrdata).usersid)
					break;
			}
			if (itr == mUserList.end()){
				// ファイル内のユーザがWindows上に存在しないので
				// 登録しない
				bUpdateFile = true;
				continue;
			}
			stAccount data;
			data.user = (*itrdata).user;
			data.usersid = (*itrdata).usersid;
			data.usc20 = (*itrdata).usc20;
			data.usc34 = (*itrdata).usc34;
			data.udsc64 = (*itrdata).udsc64;
			data.xdsc54 = (*itrdata).xdsc54;
			data.enable = (*itrdata).enable;
			data.mycomputer = false;
			data.dummy = (*itrdata).dummy;
			mAccountList.push_back(data);
		}

		for (itr = mUserList.begin(); itr != mUserList.end(); itr++){
			for (itrdata = mAccountList.begin(); itrdata != mAccountList.end(); itrdata++){
				if ((*itr).user == (*itrdata).user && (*itr).usersid == (*itrdata).usersid){
					// ローカルユーザが存在する
					(*itrdata).mycomputer = true;
					break;
				}
			}
			if (itrdata == mAccountList.end()){
				// 新しいユーザが存在したので初期化して追加する
				stAccount data;
				data.user = (*itr).user;
				data.usersid = (*itr).usersid;
				data.usc20 = false;
				data.usc34 = false;
				data.udsc64 = false;
				data.xdsc54 = false;
				data.enable = true;
				data.mycomputer = true;
				data.dummy = 0;
				mAccountList.push_back(data);
			}
		}
	}

	if (bUpdateFile == true){
		// Windowsからユーザが削除されたので、アカウントファイルからも削除する
		CFileAccess& ac = theApp.GetFileAccess();
		ac.WriteOperationAccount(mAccountList);
	}

	vector<stAccount>::iterator itr;
	int pos = 0;
	int index;
	for (index=0,itr = mAccountList.begin(); itr != mAccountList.end(); index++,itr++){
		//LPARAM lParam = ((*itr).usc20 << 24) | ((*itr).usc34 << 16) | ((*itr).udsc64 << 8) | ((*itr).xdsc54 << 0);
		if ((*itr).enable == false)
			continue;
		LPARAM lParam = (((*itr).mycomputer<<24) | (index<<16) | (*itr).usc20 << 3) | ((*itr).usc34 << 2) | ((*itr).udsc64 << 1) | ((*itr).xdsc54 << 0);
		AddItem(pos, 0, (*itr).user, (DWORD)lParam);
		AddItem(pos, 1, _T(""));
		AddItem(pos, 2, _T(""));
		AddItem(pos, 3, _T(""));
		AddItem(pos, 4, _T(""));
		pos++;
	}

	AutoSizeColumns();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

#define	MINCOLWIDTH		80
template <class T> inline const T& MAX(const T& t1, const T& t2) {
	return t1 < t2 ? t2 : t1;
}
/*============================================================================*/
/*! 拡張リストコントロール

-# リストビューのカラム幅自動設定

@param	col	自動調整を行うカラム番号（-1の場合は全てのカラムを対象に自動調整を行う）

@retval void
*/
/*============================================================================*/
void COperationAccount::AutoSizeColumns(int col /*=-1*/)
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)mList.GetDlgItem(0);

	mList.SetRedraw(false);
	int dummycol = pHeader->GetItemCount();
	mList.InsertColumn(dummycol, _T("dummy"), LVCFMT_LEFT, 10);

	HDITEM hdi = { 0 };
	hdi.mask = HDI_FORMAT | HDI_IMAGE;
	pHeader->GetItem(0, &hdi);

	int nCol = col;
	int mincol = col < 0 ? 0 : col;
	int maxcol = col < 0 ? pHeader->GetItemCount() - 1 : col;
	for (col = mincol; col <= maxcol; col++){
		mList.SetColumnWidth(col, LVSCW_AUTOSIZE);
		int wc1 = mList.GetColumnWidth(col);
		mList.SetColumnWidth(col, LVSCW_AUTOSIZE_USEHEADER);
		int wc2 = mList.GetColumnWidth(col);
		int wc = MAX(MINCOLWIDTH, MAX(wc1, wc2));
		mList.SetColumnWidth(col, wc);
	}
	mList.DeleteColumn(dummycol);
	mList.SetRedraw(true);
}
/*============================================================================*/
/*! 拡張リストコントロール

-# リストビューへの項目設定

@param	nItem		アイテム番号
@param	nSubItem	サブアイテム番号
@param	strItem		アイテム文字列
@param	dwParam		ユーザ情報

@retval int
*/
/*============================================================================*/
int COperationAccount::AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam/*=0*/)
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
		return mList.InsertItem(&lv);
	return mList.SetItem(&lv);
}


void COperationAccount::OnHdnDividerdblclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	AutoSizeColumns();
	*pResult = 0;
}

/*============================================================================*/
/*! 設定リスト

-# ツリーコントロールで使用するイメージリストの作成

@param		pdc			デバイスコンテキスト

@retval	HIMAGELIST	イメージリスト
*/
/*============================================================================*/
HIMAGELIST COperationAccount::CreateImageList(CDC* pdc)
{
	typedef struct {
		UINT	nType;
		UINT	nStateId;
		UINT	nState;
	} stImageInfo;
	// ツリーコントロールではチェックボックスしか使用しないのでラジオボタンの作成は行わない
	static	const stImageInfo	mImageInfo[] = {
		{ 0, 0, 0 },
		{ BP_CHECKBOX, CBS_UNCHECKEDNORMAL, DFCS_BUTTONCHECK | DFCS_FLAT },
		{ BP_CHECKBOX, CBS_CHECKEDNORMAL, DFCS_BUTTONCHECK | DFCS_CHECKED | DFCS_FLAT },
		{ BP_CHECKBOX, CBS_MIXEDNORMAL, DFCS_BUTTON3STATE | DFCS_CHECKED | DFCS_FLAT },
	};

	int mBmpWidth, mBmpHeight;
	mBmpWidth = 16;
	mBmpHeight = 16;

	HIMAGELIST hImageList = 0;
	const int nBmpWidth = mBmpWidth;
	const int nBmpHeight = mBmpHeight;
	const int nImages = sizeof(mImageInfo) / sizeof(mImageInfo[0]);
	CDC memDC;
	if (memDC.CreateCompatibleDC(pdc) == FALSE)
		return NULL;
	CBitmap bmp;
	if (bmp.CreateCompatibleBitmap(pdc, nBmpWidth * nImages, nBmpHeight) == FALSE){
		memDC.DeleteDC();
		return NULL;
	}
	hImageList = ImageList_Create(nBmpWidth, nBmpHeight, ILC_COLOR32, nImages, 1);
	if (hImageList == NULL){
		bmp.DeleteObject();
		memDC.DeleteDC();
		return NULL;
	}

	RECT rectImage1 = { 0, 0, mBmpWidth - 1, mBmpHeight - 1 };

	HTHEME hTheme = OpenThemeData(NULL, L"BUTTON");
	CBitmap* pOldBmp = memDC.SelectObject(&bmp);
	memDC.FillSolidRect(0, 0, nBmpWidth*nImages, nBmpHeight, GetSysColor(COLOR_WINDOW));

	int nImageWidth = nBmpWidth - 2;
	int nImageHeight = nBmpHeight - 2;
	int nImageLeft = (nBmpWidth - nImageWidth) / 2;
	int nImageTop = (nBmpHeight - nImageHeight) / 2;

	RECT rectImage = { nImageLeft,
		nImageTop,
		nImageLeft + nImageWidth,
		nImageTop + nImageHeight };

	for (int i = 0; i<nImages; i++){
		memDC.FillSolidRect(0, 0, mBmpWidth, mBmpHeight, GetSysColor(COLOR_WINDOW));
		if (mImageInfo[i].nType == 0){
		}
		else{
			if (hTheme){
				rectImage1.left = rectImage.left - 1;
				rectImage1.right = rectImage1.left + nBmpWidth - 1;
				DrawThemeBackground(hTheme, memDC.m_hDC, mImageInfo[i].nType, mImageInfo[i].nStateId, &rectImage1, NULL);
				DrawThemeEdge(hTheme, memDC.m_hDC, mImageInfo[i].nType, mImageInfo[i].nStateId, &rectImage1, 0, BF_ADJUST | BF_RECT, NULL);
			}
			else{
				memDC.DrawFrameControl(&rectImage1, DFC_BUTTON, mImageInfo[i].nState);
			}
		}
		rectImage.left += nBmpWidth;
		rectImage.right += nBmpWidth;
	}
	if (hTheme){
		CloseThemeData(hTheme);
	}
	memDC.SelectObject(pOldBmp);

	// イメージリストへの追加
	ImageList_Add(hImageList, (HBITMAP)bmp.m_hObject, 0);

	bmp.DeleteObject();
	memDC.DeleteDC();
	return hImageList;
}


void COperationAccount::OnBnClickedButton1()
{
	vector<stAccount>::iterator itr;
	int pos = 0;
	for (itr = mAccountList.begin(); itr != mAccountList.end(); itr++){
		if ((*itr).enable == false)
			continue;
		LPARAM lParam = mList.GetItemData(pos);
		(*itr).usc20 = ((lParam >> 3) & 0x01) ? true : false;
		(*itr).usc34 = ((lParam >> 2) & 0x01) ? true : false;
		(*itr).udsc64 = ((lParam >> 1) & 0x01) ? true : false;
		(*itr).xdsc54 = ((lParam >> 0) & 0x01) ? true : false;
		pos++;
	}
	CFileAccess& ac = theApp.GetFileAccess();
	ac.WriteOperationAccount(mAccountList);

	vector<stAccount> mAcList;
	ac.ReadOperationAccount(mAcList);
}

void COperationAccount::OnBnClickedButton2()
{
	// 登録削除
	// 選択されているユーザを削除する
	CString msg;
	UINT uSelectedCount = mList.GetSelectedCount();
	int  nItem = -1;
	nItem = mList.GetNextItem(nItem, LVNI_SELECTED);
	CString user = mList.GetItemText(nItem, 0);
	msg.Format(_T("選択されたユーザ[%s]を運用アカウントから削除します。\nよろしいですか。"), (LPCTSTR)user);
	if (MessageBox(msg, 0, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO)
		return;

	// 削除処理
	LPARAM pos = mList.GetItemData(nItem);
	pos = (pos >> 16) & 0xff;
	mAccountList[pos].enable = false;
	mList.DeleteItem(nItem);
}
