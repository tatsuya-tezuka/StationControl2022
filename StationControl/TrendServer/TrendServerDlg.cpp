
// TrendServerDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "TrendServer.h"
#include "TrendServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTrendServerDlg ダイアログ



CTrendServerDlg::CTrendServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTrendServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTrendServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STATION, mListStation);
	DDX_Control(pDX, IDC_LIST_EQUIPMENT, mListEquipment);
	DDX_Control(pDX, IDC_LIST_SIGNAL, mListSignal);
	DDX_Control(pDX, IDC_RICHEDIT_MESSAGE, mMessage);
}

BEGIN_MESSAGE_MAP(CTrendServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CTrendServerDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CTrendServerDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


/*============================================================================*/
/*! サーバー

-# 制御サーバーの開始

@param

@retval

*/
/*============================================================================*/
bool CTrendServerDlg::_StartServer()
{
	vector<stDataList>& datalist = m_DataServer.GetDataList();
	// データリストの登録
	int cntStation = mListStation.GetItemCount();
	int cntEquipment = mListEquipment.GetItemCount();
	int cntSignal = mListSignal.GetItemCount();

	datalist.clear();
	int i, j, k;
	CString str;
	for (i = 0; i < cntStation; i++){
		if (mListStation.GetCheck(i) == FALSE)
			continue;
		for (j = 0; j < cntEquipment; j++){
			if (mListEquipment.GetCheck(j) == FALSE)
				continue;
			for (k = 0; k < cntSignal; k++){
				if (mListSignal.GetCheck(k) == FALSE)
					continue;
				str = mListStation.GetItemText(i, 0);
				str += _T(".");
				str += mListEquipment.GetItemText(j, 0);
				str += _T(".");
				str += mListSignal.GetItemText(k, 0);
				stDataList val;
				CStringA name(str);
				sprintf_s((char*)val.name, _MAX_NAME, "%s", name);
				CStringA unit(mDataUnitString[k]);
				sprintf_s((char*)val.unit, _MAX_UNIT, "%s", unit);
				datalist.push_back(val);
			}
		}
	}

	bool bSuccess = m_DataServer.Create(CCommServer::mDataPort);
	if (bSuccess){
		m_DataServer.SetMessageWnd(this);
		char buf[MAX_PATH];
		CString str;
		m_DataServer.GetLocalAddress(buf, MAX_PATH);
		str.Format(_T("=== Start Server : %s(%d)"), CString(buf), CCommServer::mDataPort);
		_AppendMessage(str);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_STATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_EQUIPMENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_SIGNAL)->EnableWindow(FALSE);
	}

	return bSuccess;
}

/*============================================================================*/
/*! サーバー

-# 制御サーバーの終了

@param

@retval

*/
/*============================================================================*/
bool CTrendServerDlg::_StopServer()
{
	bool bSuccess = m_DataServer.Release();
	if (bSuccess){
		_AppendMessage(_T("=== Stop Server"));
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_STATION)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_EQUIPMENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_SIGNAL)->EnableWindow(TRUE);
	}
	return true;
}

/*============================================================================*/
/*! サーバー

-# リストコントロールへの項目追加

@param

@retval

*/
/*============================================================================*/
int CTrendServerDlg::_AddItem(CListCtrl& list, int nItem, int nSubItem, CString strItem, DWORD dwParam/*=0*/)
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
		return list.InsertItem(&lv);
	return list.SetItem(&lv);
}
/*============================================================================*/
/*! サーバー

-# メッセージを画面に出力する

@param	msg		メッセージ
@param	level	表示レベル
@param	color	メッセージ色

@retval

*/
/*============================================================================*/
int CTrendServerDlg::_AppendMessage(CString msg, COLORREF color/* = RGB(0, 0, 0)*/)
{
	int nOldLines = 0, nNewLines = 0, nScroll = 0;
	long nInsertionPoint = 0;
	CHARFORMAT cf;

	if (mMessage.GetSafeHwnd() == NULL)
		return 0;


	nOldLines = mMessage.GetLineCount();

	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0;
	cf.crTextColor = color;

	nInsertionPoint = mMessage.GetWindowTextLength();
	mMessage.SetSel(nInsertionPoint, -1);

	mMessage.SetSelectionCharFormat(cf);

	msg += _T("\n");
	mMessage.ReplaceSel(msg);

	nNewLines = mMessage.GetLineCount();

	nScroll = nNewLines - nOldLines;
	mMessage.LineScroll(nScroll);

	return 0;
}


// CTrendServerDlg メッセージ ハンドラー

BOOL CTrendServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

	int pos;
	int i;
	DWORD  dwStyle = ListView_GetExtendedListViewStyle(mListStation.m_hWnd);
	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES;
	// 基地局の作成
	ListView_SetExtendedListViewStyle(mListStation.m_hWnd, dwStyle);
	for (i = 0; i<sizeof(mBaseStationString) / sizeof(mBaseStationString[0]); i++){
		pos = _AddItem(mListStation, i, 0, mBaseStationString[i], i);
		mListStation.SetCheck(pos);
	}
	// 設備の作成
	ListView_SetExtendedListViewStyle(mListEquipment.m_hWnd, dwStyle);
	for (i = 0; i<sizeof(mEquipmentString) / sizeof(mEquipmentString[0]); i++){
		pos = _AddItem(mListEquipment, i, 0, mEquipmentString[i], i);
		mListEquipment.SetCheck(pos);
	}
	// 基地局の作成
	ListView_SetExtendedListViewStyle(mListSignal.m_hWnd, dwStyle);
	for (i = 0; i<sizeof(mDataNameString) / sizeof(mDataNameString[0]); i++){
		pos = _AddItem(mListSignal, i, 0, mDataNameString[i], i);
		mListSignal.SetCheck(pos);
	}

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CTrendServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CTrendServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CTrendServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTrendServerDlg::OnBnClickedButtonStart()
{
	_StartServer();
}


void CTrendServerDlg::OnBnClickedButtonStop()
{
	_StopServer();
}


LRESULT CTrendServerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_ClientConnection:
	{
		CADDRLIST::iterator itr;
		itr = m_DataServer.GetClientAddrList().find((ULONG)lParam);
		if (itr != m_DataServer.GetClientAddrList().end()){
			CString str;
			str.Format(_T("# Connect Client : %s"), CString((*itr).second.ipClient));
			_AppendMessage(str);
		}
	}
	break;
	case	eMessage_ClientDisConnection:
	{
		CADDRLIST::iterator itr;
		itr = m_DataServer.GetClientAddrList().find((ULONG)lParam);
		if (itr != m_DataServer.GetClientAddrList().end()){
			CString str;
			str.Format(_T("# DisConnect Client : %s"), CString((*itr).second.ipClient));
			_AppendMessage(str);
		}
	}
	break;
	case	eMessage_ClientCommand:
	{
		CADDRLIST::iterator itr;
		itr = m_DataServer.GetClientAddrList().find((ULONG)lParam);
		if (itr != m_DataServer.GetClientAddrList().end()){
			CString str;
			str.Format(_T("- (%s) : Command(%d)"), CString((*itr).second.ipClient), wParam);
			_AppendMessage(str);
		}
	}
	break;
	default:
		return CDialogEx::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}
