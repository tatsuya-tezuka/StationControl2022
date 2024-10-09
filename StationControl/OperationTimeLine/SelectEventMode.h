#pragma once


// CSelectEventMode ダイアログ

class CSelectEventMode : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectEventMode)

public:
	CSelectEventMode(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSelectEventMode();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_EVENTMODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nEventMode;
	afx_msg void OnBnClickedOk();
};
