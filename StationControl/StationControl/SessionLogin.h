#pragma once


// CSessionLogin ダイアログ

class CSessionLogin : public CDialogBase
{
	DECLARE_DYNAMIC(CSessionLogin)

public:
	CSessionLogin(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSessionLogin();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_SESSIONLOGIN };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString m_passwd;					// パスワード
	eSessionType m_sessionType;			// セッション

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void SetSessionType(eSessionType sessionType)	{ m_sessionType = sessionType; }
	eSessionType GetSessionType()	{ return m_sessionType; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	/* ------------------------------------------------------------------------------------ */
};
