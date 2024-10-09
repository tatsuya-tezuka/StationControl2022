#pragma once


// CTrsControlDetail ダイアログ

class CTrsControlDetail : public CDialogEx
{
	DECLARE_DYNAMIC(CTrsControlDetail)

public:
	CTrsControlDetail(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTrsControlDetail();

// ダイアログ データ
	enum { IDD = IDD_DLG_TRSCTRL_DETAIL };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
protected:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	// 制御名
	CString m_strCtrlName;
	int m_CtrlValue;
};
