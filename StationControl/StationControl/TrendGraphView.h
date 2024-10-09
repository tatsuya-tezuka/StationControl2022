#pragma once

#include "GraphWindow.h"
#include "StationControlDoc.h"

// CTrendGraphView ビュー

class CTrendGraphView : public CView
{
	DECLARE_DYNCREATE(CTrendGraphView)

public:
	CTrendGraphView();           // 動的生成で使用される protected コンストラクター
	virtual ~CTrendGraphView();

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
	CGraphWindow		m_Graph[eTrendWndType_Num];	// 各グラフ
	eTrendWndSetType	m_nWndTypeSet;				// ウィンドウタイプセット
	BOOL				m_bIsInitialized;			// 初期化済み
	time_t				m_MostOldTime1st;			// 初回のもっとも古い時刻
	time_t				m_MostOldTime;				// もっとも古い時刻
	time_t				m_MostNewTime;				// もっとも新しい時刻

	BOOL				m_bInit;					// 初期化

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	// 属性
public:
	CStationControlDoc* GetDocument() const;
	void SetWndTypeSet(eTrendWndSetType type)	{ m_nWndTypeSet = type; }
	eTrendWndSetType GetWndTypeSet()			{ return m_nWndTypeSet; }
	void GetGraphWndStartEnd(int& nStart, int& nEnd, BOOL& bEnaRR);
	void InitTrendWnd(eTrendWndSetType type);
	BOOL GetViewNameList(vector<CString>& listViewName);
	BOOL GetViewAllNameList(vector<CString>& listViewName);
	void AddViewParam();
	UINT ExchgPropertiesID2ViewID(UINT nPropertiesID);
	time_t	GetMostOldTime()	{ return m_MostOldTime; }
	time_t	GetMostNewTime()	{ return m_MostNewTime; }

	// レイアウト読み込み＆保存（設定情報）
	bool	Load(CArchive& ar);
	bool	Save(CArchive& ar);

protected:
	void UpdateGraph(BOOL bAll = FALSE);
	void SetGraphTime(int nStation);

public:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドされます。
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	/* ------------------------------------------------------------------------------------ */
};

#ifndef _DEBUG  // StationControlView.cpp のデバッグ バージョン
inline CStationControlDoc* CTrendGraphView::GetDocument() const
{
	return reinterpret_cast<CStationControlDoc*>(m_pDocument);
}
#endif
