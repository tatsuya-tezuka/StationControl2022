#pragma once

#include "OperationTimeDialogBase.h"

// COperationEndTimeChg ダイアログ

class COperationEndTimeChg : public COperationTimeDialogBase
{
	DECLARE_DYNAMIC(COperationEndTimeChg)

public:
	COperationEndTimeChg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~COperationEndTimeChg();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_OPERATIONENDTIMECHG };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	void UpdatePlanData(UINT nStation, CString& strTime);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
