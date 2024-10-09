// この MFC サンプル ソース コードでは、MFC Microsoft Office Fluent ユーザー インターフェイス 
// ("Fluent UI") の使用方法を示します。このコードは、MFC C++ ライブラリ ソフトウェアに 
// 同梱されている Microsoft Foundation Class リファレンスおよび関連電子ドキュメントを
// 補完するための参考資料として提供されます。
// Fluent UI を複製、使用、または配布するためのライセンス条項は個別に用意されています。
// Fluent UI ライセンス プログラムの詳細については、Web サイト
// http://go.microsoft.com/fwlink/?LinkId=238214 を参照してください。
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// RemoteGraphView.h : CRemoteGraphView クラスのインターフェイス
//

#pragma once

#include "GraphWindow.h"

class CRemoteGraphView : public CView
{
protected: // シリアル化からのみ作成します。
	CRemoteGraphView();
	DECLARE_DYNCREATE(CRemoteGraphView)

// 属性
public:
	CRemoteGraphDoc* GetDocument() const;

	CGraphWindow	m_Graph;
	UINT			m_nViewType;

// 操作
public:
	void	CreateGraph(UINT nViewID, UINT nViewType, UINT nWindowNo);
	void	DeleteGraph();
	void	SetViewType(UINT val);
//	void	ShowScroll(BOOL bShow)	{ if( m_Graph.GetSafeHwnd() ) m_Graph.ShowScrollBar(SB_HORZ, bShow); }
	void	CreateMinMaxValue()	{ if( m_Graph.GetSafeHwnd() ) m_Graph.CreateMinMaxValue(); }
	void	ResetRectangle(unsigned __int64 left, unsigned __int64 right)	{ if( m_Graph.GetSafeHwnd() ) m_Graph.ResetRectangle(m_Graph.ConvNoToX(left), m_Graph.ConvNoToX(right)); }
	void	ResetCursor(unsigned __int64 pos)	{ if( m_Graph.GetSafeHwnd() ) m_Graph.ResetCursor((pos == -1) ? -1 : m_Graph.ConvNoToX(pos)); }
	void	ResetDiffer()	{ if( m_Graph.GetSafeHwnd() ) m_Graph.ResetDiffer(true, true); }
	void	GetMinMaxValue(double& min, double& max)	{ if( m_Graph.GetSafeHwnd() ) m_Graph.GetMinMaxValue(min, max); }
	void	GetMinMaxValueEx(double& min, double& max)	{ if( m_Graph.GetSafeHwnd() ) m_Graph.GetMinMaxValueEx(min, max); }

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 実装
public:
	virtual ~CRemoteGraphView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int	m_angle[32];	// なんちゃってデータ用
	float	m_fLogData;
//	void	GetDummyData(int pos, float& data);

	static BOOL	m_bUsedClassFirst;
	static int m_initAngle[32];

#if 0
public:
	CEvent			m_evtUpdate;
	BOOL			m_bUpdateThread;
	CWinThread		*m_pUpdateThread;
#endif

public:
	BOOL	AddLine(UINT nID, CString strName, CString strGName, UINT nGraphKind, COLORREF crColor, DWORD nStyle, int nWidth, VScaleKind nVScaleKind, BOOL bVisible);
	BOOL	AddDot(UINT nI, CString strName, CString strGName, UINT nGraphKind, COLORREF crColor, int nDotSize, VScaleKind nVScaleKind, BOOL bVisible);
	BOOL	AddLineAndDot(UINT nID, CString strName, CString strGName, UINT nGraphKind, COLORREF crColor, DWORD nStyle, int nWidth, int nDotSize, VScaleKind nVScaleKind, BOOL bVisible);
	BOOL	AddAxis(stVScale nStvScale, HScale trandHScale);

	void	UpdateGraphWindow(int nViewNo, bool bClear);
	void	SetRealTime(BOOL val);

// 生成された、メッセージ割り当て関数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // RemoteGraphView.cpp のデバッグ バージョン
inline CRemoteGraphDoc* CRemoteGraphView::GetDocument() const
   { return reinterpret_cast<CRemoteGraphDoc*>(m_pDocument); }
#endif

