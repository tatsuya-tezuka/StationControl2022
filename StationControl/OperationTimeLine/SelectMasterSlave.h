#pragma once


// CSelectMasterSlave ダイアログ

class CSelectMasterSlave : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectMasterSlave)

public:
	CSelectMasterSlave(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSelectMasterSlave();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_MASTERSLAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	int mSelectMode;
};
