#pragma once

#include "FrequencyCtrl.h"
#include "FrequencyCtrlMon54.h"
#include "FrequencyCtrlCtrl54.h"
#include "FrequencyCtrlPre54.h"

// CFrequencyCtrl54 ダイアログ

class CFrequencyCtrl54 : public CFrequencyCtrl
{
	DECLARE_DYNAMIC(CFrequencyCtrl54)

public:
	CFrequencyCtrl54(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CFrequencyCtrl54();

// ダイアログ データ
	enum { IDD = IDD_DLG_FREQ_CTRL54 };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
private:
	CFrequencyCtrlMon54 		m_TabMon;
	CFrequencyCtrlCtrl54		m_TabCtrl;
	CFrequencyCtrlPre			m_TabPre;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeFreqCtrlTab(NMHDR *pNMHDR, LRESULT *pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	/* ------------------------------------------------------------------------------------ */
};
