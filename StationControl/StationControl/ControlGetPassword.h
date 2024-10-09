#pragma once


// CControlGetPassword ダイアログ

class CControlGetPassword : public CDialogEx
{
	DECLARE_DYNAMIC(CControlGetPassword)

public:
	CControlGetPassword(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CControlGetPassword();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CONTROL_GETPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
