#pragma once


// CSelectLNADlg ダイアログ

class CSelectLNADlg : public CDialogBase
{
	DECLARE_DYNAMIC(CSelectLNADlg)

public:
	CSelectLNADlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSelectLNADlg();

// ダイアログ データ
	enum { IDD = IDD_DLG_SELECTLNA };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	enum
	{
		eLNA1 = 1,
		eLNA2,
	};

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
protected:
	UINT m_nSelLNA;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	void SetLNA(UINT lna)
	{
		if (lna < eLNA1 || lna > eLNA2)
			m_nSelLNA = eLNA1;
		else
			m_nSelLNA = lna;
	}

protected:
	void EnableExecButton();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedSelectLna1();
	afx_msg void OnBnClickedSelectLna2();
};
