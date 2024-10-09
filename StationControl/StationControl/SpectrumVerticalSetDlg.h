#pragma once

#include "SpectrumVerticalSetCommonDlg.h"
#include "SpectrumView.h"

// CSpectrumVerticalSetDlg ダイアログ

class CSpectrumVerticalSetDlg : public CSpectrumVerticalSetCommonDlg
{
	DECLARE_DYNAMIC(CSpectrumVerticalSetDlg)

public:
	CSpectrumVerticalSetDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	CSpectrumVerticalSetDlg(CWnd* pParent, CSpectrumView* pSpectrumView);
	virtual ~CSpectrumVerticalSetDlg();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_SPECTRUMVERTICALSET };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CSpectrumView* m_pSpectrumView;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

//	BOOL SetVerticalValues();
//	void SetEnableVAxis(BOOL bEnable);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeSpectrumverticalsetCombo();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	/* ------------------------------------------------------------------------------------ */
};
