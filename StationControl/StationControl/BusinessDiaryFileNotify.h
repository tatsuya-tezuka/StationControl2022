#pragma once


// CBusinessDiaryFileNotify ダイアログ

class CBusinessDiaryFileNotify : public CDialogEx
{
	DECLARE_DYNAMIC(CBusinessDiaryFileNotify)

public:
	CBusinessDiaryFileNotify(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CBusinessDiaryFileNotify();

// ダイアログ データ
	enum { IDD = IDD_DLG_DIARY_FILENOTIFY };

	CWinThread*		mpThread;

	bool	IsBusinessDiaryFile();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
