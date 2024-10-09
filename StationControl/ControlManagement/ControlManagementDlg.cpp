
// ControlManagementDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ControlManagement.h"
#include "ControlManagementDlg.h"
#include "afxdialogex.h"

#include "ControlManagementChangePasswordDlg.h"
#include "OperationAccount.h"
#include "ChangeControlDlg.h"
#include "ControlChangePasswordDlg.h"
#include "CancelControlDlg.h"

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


// CControlManagementDlg ダイアログ



CControlManagementDlg::CControlManagementDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlManagementDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CControlManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CControlManagementDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CControlManagementDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CControlManagementDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CControlManagementDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CControlManagementDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CControlManagementDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDCANCEL, &CControlManagementDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CControlManagementDlg メッセージ ハンドラー

BOOL CControlManagementDlg::OnInitDialog()
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

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CControlManagementDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CControlManagementDlg::OnPaint()
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
HCURSOR CControlManagementDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*============================================================================*/
/*! 制御権利管理

-# 制御権利変更

@param
@retval

*/
/*============================================================================*/
void CControlManagementDlg::OnBnClickedButton1()
{
	CChangeControlDlg dlg;
	if (dlg.DoModal() == IDCANCEL)
		return;
}

/*============================================================================*/
/*! 制御権利管理

-# 筑波局運用管制端末の制御権利解除

@param
@retval

*/
/*============================================================================*/
void CControlManagementDlg::OnBnClickedButton2()
{
	CCancelControlDlg dlg;
	if (dlg.DoModal() == IDCANCEL)
		return;
}

/*============================================================================*/
/*! 制御権利管理

-# 運用アカウント設定

@param
@retval

*/
/*============================================================================*/
void CControlManagementDlg::OnBnClickedButton3()
{
	COperationAccount dlg;
	if (dlg.DoModal() == IDCANCEL)
		return;
}

/*============================================================================*/
/*! 制御権利管理

-# 制御権利取得用パスワード変更

@param
@retval

*/
/*============================================================================*/
void CControlManagementDlg::OnBnClickedButton4()
{
	CControlChangePasswordDlg dlg;
	if (dlg.DoModal() == IDCANCEL)
		return;
}

/*============================================================================*/
/*! 制御権利管理

-# 管理パスワード変更

@param
@retval

*/
/*============================================================================*/
void CControlManagementDlg::OnBnClickedButton5()
{
	CControlManagementChangePasswordDlg dlg;
	if (dlg.DoModal() == IDCANCEL)
		return;
}

/*============================================================================*/
/*! 制御権利管理

-# 終了

@param
@retval

*/
/*============================================================================*/
void CControlManagementDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}
