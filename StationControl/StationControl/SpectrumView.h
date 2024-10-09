#pragma once
#include "SpectrumCommonDlg.h"

// CSpectrumView ダイアログ

class CSpectrumView : public CSpectrumCommonDlg
{
	DECLARE_DYNAMIC(CSpectrumView)

public:
	CSpectrumView(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSpectrumView();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_SPECTRUMVIEW };


protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnStnClickedStaticSpectrumviewArea();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedSpectrumverticalsetButton();
	/* ------------------------------------------------------------------------------------ */
};
