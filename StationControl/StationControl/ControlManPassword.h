#pragma once


// CControlManPassword ダイアログ

class CControlManPassword : public CDialogEx
{
	DECLARE_DYNAMIC(CControlManPassword)

public:
	CControlManPassword(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CControlManPassword();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CONTROL_MANPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
