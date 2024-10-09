#pragma once


static const CString mEventDelText[] = {
	_T("アップリンク番号 %d を削除します\nよろしいですか？"),
	_T("CMD番号 %d を削除します\nよろしいですか？"),
	_T("RNG番号 %d を削除します\nよろしいですか？"),
};

enum{
	eEventDelModeUplink,
	eEventDelModeCmd,
	eEventDelModeRng,
};

// CEventDel ダイアログ

class CEventDel : public CDialogEx
{
	DECLARE_DYNAMIC(CEventDel)

public:
	CEventDel(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CEventDel();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_EVENTDEL };

	int		mMode;
	int		mTargetNo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CString m_strText;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
