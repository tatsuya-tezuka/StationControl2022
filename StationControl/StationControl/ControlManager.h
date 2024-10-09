#pragma once


// CControlManager ダイアログ

class CControlManager : public CDialogEx
{
	DECLARE_DYNAMIC(CControlManager)

public:
	CControlManager(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CControlManager();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CONTROL_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
