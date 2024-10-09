#pragma once


// CControlLogin ダイアログ

class CControlLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CControlLogin)

public:
	CControlLogin(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CControlLogin();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CONTROL_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
