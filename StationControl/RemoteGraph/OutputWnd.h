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

#pragma once

#include "const.h"

/////////////////////////////////////////////////////////////////////////////
// COutputList ウィンドウ

class COutputList : public CListBox
{
// コンストラクション
public:
	COutputList();

// 実装
public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};


class COutputListCtrl : public CListCtrl
{
public:
	COutputListCtrl();
	virtual ~COutputListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
};

// CMFCOutputListCtrlEx

class CMFCOutputListCtrlEx : public CMFCListCtrl
{
	DECLARE_DYNAMIC(CMFCOutputListCtrlEx)

public:
	CMFCOutputListCtrlEx();
	virtual ~CMFCOutputListCtrlEx();

public:
	int	m_pos;

protected:
	DECLARE_MESSAGE_MAP()
};


// CMFCOutputTabCtrlEx

class CMFCOutputTabCtrlEx : public CMFCTabCtrl
{
	DECLARE_DYNAMIC(CMFCOutputTabCtrlEx)

public:
	CMFCOutputTabCtrlEx();
	virtual ~CMFCOutputTabCtrlEx();

#if 0
	virtual void FireChangeActiveTab(int nNewTab);
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


class COutputWnd : public CDockablePane
{
// コンストラクション
public:
	COutputWnd();

	void UpdateFonts();

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	void	AddTab(UINT pos);
	void	UpdateDataId(UINT pos);
	void	UpdateTab(UINT pos);
	void	DeleteTab(UINT pos);
	void	UpdateDataEx(UINT pos);
	int		AddItem(int pos, int nItem, int nSubItem, CString strItem, DWORD dwAll = 0);
	int	SearchTabIndex(CString str)
	{
		for (int i = 0; i < m_wndTabs.GetTabsNum(); i++)
		{
			CString	strTabName;
			m_wndTabs.GetTabLabel(i, strTabName);
			if( strTabName == str )
				return i;
		}

		return 0;
	}

protected:

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:

protected:


// 属性
protected:
	CMFCOutputTabCtrlEx	m_wndTabs;

	COutputList m_wndOutputBuild;
	COutputList m_wndOutputDebug;
	COutputList m_wndOutputFind;
	CRect	m_rectList;
	CMFCOutputListCtrlEx	m_ListOutput[MAX_VIEW];

	bool	m_listDisplayTab[MAX_VIEW];
	DWORD	m_listDataId[MAX_VIEW][MAX_TDATA];

protected:
	void FillBuildWindow();
	void FillDebugWindow();
	void FillFindWindow();

	void AdjustHorzScroll(CListBox& wndListBox);

// 実装
public:
	virtual ~COutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

