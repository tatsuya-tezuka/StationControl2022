#pragma once

#include "DataCollection.h"
#include "GraphWindow.h"
#include "SpectrumGraphView.h"

// CSummaryGraphCtrl

class CSummaryGraphCtrl : public CWnd
{
	DECLARE_DYNAMIC(CSummaryGraphCtrl)

public:
	CSummaryGraphCtrl();
	virtual ~CSummaryGraphCtrl();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	CWnd*		mParent;
	UINT		mID;								// ビューID
	UINT		mStationIndex;						// 局インデックス
	BOOL		mInitOver;
	CRect		mRect;
	CGraphWindow	m_Graph;						// グラフビュー
	CSpectrumGraphView*	m_pSpectrumView;			// スペクトラムビュー
	eSummaryWndSetType	m_nWndTypeSet;				// ウィンドウタイプセット
	int			m_nListMax;							// 項目数最大

	time_t				m_MostOldTime1st;			// 初回のもっとも古い時刻
	time_t				m_MostOldTime;				// もっとも古い時刻
	time_t				m_MostNewTime;				// もっとも新しい時刻

public:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	time_t	GetMostOldTime()	{ return m_MostOldTime; }
	time_t	GetMostNewTime()	{ return m_MostNewTime; }
	void UpdateGraph(BOOL bAll = FALSE);

	// レイアウト読み込み＆保存（設定情報）
	bool	Load(CArchive& ar);
	bool	Save(CArchive& ar);

protected:
	BOOL	RegisterClass();

	void SetWndTypeSet(eSummaryWndSetType type)	{ m_nWndTypeSet = type; }
	eSummaryWndSetType GetWndTypeSet()			{ return m_nWndTypeSet; }
	void InitTrendWnd(eSummaryWndSetType type);
	void InitSpectrumWnd(UINT nViewID);
	BOOL GetViewNameList(vector<CString>& listViewName);
	void AddViewParam();
	void SetGraphTime(int nStation);

public:
	BOOL	Create(CWnd *pParent, CRect rect, UINT nID, UINT nStaionID, UINT nParentID = 0);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	/* ------------------------------------------------------------------------------------ */
};


