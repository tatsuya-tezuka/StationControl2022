#pragma once
#include "afxdtctl.h"
#include "atltime.h"
#include "FrequencyCtrlMon.h"

// CFrequencyCtrlMon54 ダイアログ


class CFrequencyCtrlMon54 : public CFrequencyCtrlMon
{
	DECLARE_DYNAMIC(CFrequencyCtrlMon54)

public:
	CFrequencyCtrlMon54(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CFrequencyCtrlMon54();

// ダイアログ データ
	enum { IDD = IDD_DLG_FREQ_CTRL_MONITOR54 };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
protected:

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	virtual BOOL UpdateDlg();

protected:
	void SetData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_strKaDC;
	CString m_strDCCOHMode;
};
