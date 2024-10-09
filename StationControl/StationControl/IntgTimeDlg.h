#pragma once


// CIntgTimeDlg ダイアログ

class CIntgTimeDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CIntgTimeDlg)

public:
	CIntgTimeDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CIntgTimeDlg();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_INTGTIME };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	int m_nIntgTime;
	CEdit m_cedit;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void SetIntgTime(int time)	{ m_nIntgTime = time; }
	int GetIntgTime()			{ return m_nIntgTime; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	/* ------------------------------------------------------------------------------------ */
};
