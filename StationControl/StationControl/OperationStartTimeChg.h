#pragma once

#include "OperationTimeDialogBase.h"

// COperationStartTimeChg ダイアログ

class COperationStartTimeChg : public COperationTimeDialogBase
{
	DECLARE_DYNAMIC(COperationStartTimeChg)

public:
	COperationStartTimeChg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~COperationStartTimeChg();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_OPERATIONSTARTTIMECHG };

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
