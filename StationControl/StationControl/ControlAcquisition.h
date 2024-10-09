#pragma once


// CControlAcquisition ダイアログ

class CControlAcquisition : public CDialogEx
{
	DECLARE_DYNAMIC(CControlAcquisition)

public:
	CControlAcquisition(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CControlAcquisition();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CONTROL_ACQUISITION };

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
	CString		mGetPassword;
	__int64		mGetTimer;
	int			mClientTerminal;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	ResetControlFlag();
	int		GetClientTerminal()	{ return mClientTerminal; }

protected:
	int		GetControlType(int target);
	bool	CheckControl(int target, stControl& datast, stLocalControl& local);

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedMode();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CString m_strText;
	CString m_strPassword;
};
