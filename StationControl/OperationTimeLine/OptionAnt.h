#pragma once


// COptionAnt ダイアログ

class COptionAnt : public CDialogEx
{
	DECLARE_DYNAMIC(COptionAnt)

public:
	COptionAnt(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~COptionAnt();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_OPTIONANT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	int m_nAntMode;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
