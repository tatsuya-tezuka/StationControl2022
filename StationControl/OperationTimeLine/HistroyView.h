#pragma once

#include "HistoryList.h"
#include "HistoryFilterDlg.h"
#include "HistoryFilterEqDlg.h"
//#include "HistorySearchDlg.h"
//#include "HistorySearchEqDlg.h"

enum eTabItem{
	eTabItem_Filter = 0x01,
	eTabItem_Search = 0x02,
};

enum eFileTye{
	eFileType_Date,
	eFileType_Time,
	eFileType_Station,
	eFileType_Equipment,
	eFileType_Status,
	eFileType_MessageID,
};


// CHistroyView ビュー

class CHistroyView : public CView
{
	DECLARE_DYNCREATE(CHistroyView)

protected:
	CHistroyView();           // 動的生成で使用される protected コンストラクター
	virtual ~CHistroyView();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	static CHistroyView*	Instance(UINT type);
protected:
	static CHistroyView*	m_pInstance[eHistory_Max];
	static CCriticalSection	mSection;
	UINT				m_nHistoryType;
	UINT				m_nControlID;
	UINT				m_nTabItem;
	CHistoryList		m_History;
	CMFCTabCtrlEx		m_TabBar;
	CHistoryFilterDlg	m_FilterDlg;
	CHistoryFilterEqDlg	m_FilterDlgEq;
	//CHistorySearchDlg	m_SearchDlg;
	//CHistorySearchEqDlg	m_SearchDlgEq;
	stFilterData		mFilterData;
	deque<stHistoryData>	mHistoryData;
	deque<stHistoryData>	mHistoryHoldData; // スクロール停止中の保留データ
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	COperationTimeLineDoc* GetDocument() const;
	static void CALLBACK messageProc(stFilterData& param, UINT type);
	void	CloseChildWindow(MSG* pMsg)
	{
		m_TabBar.CloseChildWindow(pMsg);
	}
	void	SetDisplayFilter(stFilterData& data);
	void	SetDisplaySearch(int nFilterType, stFilterData& data);
	bool	IsDisplay(int nFilterType, DWORD val, UINT nStation);
	CString	CreateFilterString(stFilterData& data);
	virtual	UINT	GetHistoryType() { return m_nHistoryType; }
	virtual void	AddHistroy(){};
	virtual bool	AddHistroyEx(stHistoryData& data);
	virtual void	AddHistroy(stHistoryData& data, bool bDataAdd = true){};
	virtual bool	RestoreHoldData();
	virtual void	ClearHistroy(){ mHistoryData.clear(); };
protected:
	virtual void	CreateHistroyList();
	virtual void	AutoSizeColumns(int col = -1);
	virtual int		AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam = 0);
	/* ------------------------------------------------------------------------------------ */
	bool	_CheckHoldHistroy();
	bool	_GetDate(stHistoryData& hdata, CString cbuf);
	bool	_GetTime(stHistoryData& hdata, CString cbuf);
	bool	_GetStation(stHistoryData& hdata, CString cbuf);
	bool	_GetEquipment(stHistoryData& hdata, CString cbuf);
	bool	_GetStatus(stHistoryData& hdata, CString cbuf);
	bool	_GetMessageID(stHistoryData& hdata, CString cbuf);
	/* ------------------------------------------------------------------------------------ */

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
};

#ifndef _DEBUG  // StationControlView.cpp のデバッグ バージョン
inline COperationTimeLineDoc* CHistroyView::GetDocument() const
{
	return reinterpret_cast<COperationTimeLineDoc*>(m_pDocument);
}
#endif
