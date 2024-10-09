#pragma once


// CSelectServerClient ダイアログ

class CSelectServerClient : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectServerClient)

public:
	CSelectServerClient(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSelectServerClient();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	int m_nMode;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
