#pragma once


// CControlGrant ダイアログ

class CControlGrant : public CDialogEx
{
	DECLARE_DYNAMIC(CControlGrant)

public:
	CControlGrant(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CControlGrant();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CONTROL_GRANT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
