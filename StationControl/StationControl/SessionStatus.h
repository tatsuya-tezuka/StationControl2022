#pragma once

#include "SessionStatusTab.h"

// CSessionStatus ダイアログ

class CSessionStatus : public CDialogBase
{
	DECLARE_DYNAMIC(CSessionStatus)

public:
	CSessionStatus(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSessionStatus();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_SESSIONSTATUS };

	// 画面設定
	const int m_nTabLeftOffset = 15;			// タブウィンドウのLeftオフセット
	const int m_nTabTopOffset = 35;				// タブウィンドウのTopオフセット
	static const int m_nIntervalTime = 10000;	// 監視インターバルタイマ

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CTabCtrl m_tabCtrl;									// タブコントロール
	CSessionStatusTab	m_station[eSessionType_Num];	// 局
	UINT	m_nStation;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void AddStation(UINT nIndex, CString stationName);	// 局名を追加する
	void SetStation(UINT nStation)	{ m_nStation = nStation; }
	UINT GetStation()				{ return m_nStation; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

protected:
	void GetSessionStatus(BOOL bWaitComplete = TRUE, BOOL bForce = TRUE);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuClose();
	afx_msg void OnTcnSelchangeSessionstatusTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};
