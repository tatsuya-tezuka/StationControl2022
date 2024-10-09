#include <Windows.h>
#include <afxwin.h >


//////////////////////////////////////////////////////////////////
// フックハンドル用の変数をグローバルで宣言する。
static HHOOK MyHookHandle;	// フックハンドル変数

// OKの文字列と、キャンセルの文字列
// ボタンの文字を変更したい場合はここを変更してください
static LPCWSTR strOK = L"実行";				// OKボタンの文字
static LPCWSTR strCancel = L"キャンセル";	// キャンセルボタンの文字

// プロトタイプ宣言
static LRESULT CALLBACK MsgBoxHookProc(int nCode, WPARAM wParam, LPARAM lParam);
static int AfxMessageBoxHooked(LPCSTR message, UINT nType);



////////////////////////////////////////////////////////////////////
//
//		メッセージボックス書き換えのための
//		フック関数（ローカルフック用）
//		※ グローバル関数である必要があります。
//		この関数の中を変えるときは慎重に行って下さい。
//
static LRESULT CALLBACK MsgBoxHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// コード判断
	if (nCode >= 0)
	{
		if (nCode == HCBT_ACTIVATE)
		{
			// OKボタン(IDOK)の内容を書き換える
			SetDlgItemText((HWND)wParam, IDOK, strOK);
			// キャンセルボタン(IDCANCEL)の内容を書き換える
			SetDlgItemText((HWND)wParam, IDCANCEL, strCancel);
			BOOL ret;
			// フック関数をアンインストール(フック解除！）
			ret = UnhookWindowsHookEx(MyHookHandle);

		}
	}
	// 次のフックに渡す
	return CallNextHookEx(MyHookHandle, nCode, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////
//
//		AfxMessageBoxHooked
//
static int AfxMessageBoxHooked(LPCTSTR message, UINT nType)
{
	// フック関数(MsgBoxHookProc)をインストールする SetWindowHookEx
	MyHookHandle = SetWindowsHookEx(WH_CBT, MsgBoxHookProc, NULL, GetCurrentThreadId());
	return (AfxMessageBox(message, nType));
}

/////////////////////////////////////////////////////////////////////
//
//		MessageBoxHooked
//
static int MessageBoxHooked(HWND handle, LPCTSTR message, LPCTSTR title, UINT nType)
{
	// フック関数(MsgBoxHookProc)をインストールする SetWindowHookEx
	MyHookHandle = SetWindowsHookEx(WH_CBT, MsgBoxHookProc, NULL, GetCurrentThreadId());
	return (MessageBox(handle, message, title, nType));
}

