#pragma once


// CTabAntenna ダイアログ

class CTabAntenna : public CDialogEx
{
	DECLARE_DYNCREATE(CTabAntenna)

public:
	CTabAntenna(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTabAntenna();

// ダイアログ データ
	enum { IDD = IDD_TAB_ANTENNA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
