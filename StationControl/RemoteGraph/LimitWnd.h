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
// CLimitList ウィンドウ

class CLimitList : public CListBox
{
// コンストラクション
public:
	CLimitList();

// 実装
public:
	virtual ~CLimitList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewLimit();

	DECLARE_MESSAGE_MAP()
};


class CLimitListCtrl : public CListCtrl
{
public:
	CLimitListCtrl();
	virtual ~CLimitListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
};

// CMFCLimitListCtrlEx

class CMFCLimitListCtrlEx : public CMFCListCtrl
{
	DECLARE_DYNAMIC(CMFCLimitListCtrlEx)

public:
	CMFCLimitListCtrlEx();
	virtual ~CMFCLimitListCtrlEx();

public:
	int	m_pos;

protected:
	DECLARE_MESSAGE_MAP()
};


// CMFCLimitTabCtrlEx

class CMFCLimitTabCtrlEx : public CMFCTabCtrl
{
	DECLARE_DYNAMIC(CMFCLimitTabCtrlEx)

public:
	CMFCLimitTabCtrlEx();
	virtual ~CMFCLimitTabCtrlEx();

#if 0
	virtual void FireChangeActiveTab(int nNewTab);
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


class CLimitWnd : public CDockablePane
{
// コンストラクション
public:
	CLimitWnd();

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
	CMFCLimitTabCtrlEx	m_wndTabs;

	CLimitList m_wndLimitBuild;
	CLimitList m_wndLimitDebug;
	CLimitList m_wndLimitFind;
	CRect	m_rectList;
	CMFCLimitListCtrlEx	m_ListLimit[MAX_VIEW];

	bool	m_listDisplayTab[MAX_VIEW];
	DWORD	m_listDataId[MAX_VIEW][MAX_TDATA];

protected:
	void FillBuildWindow();
	void FillDebugWindow();
	void FillFindWindow();

	void AdjustHorzScroll(CListBox& wndListBox);

// 実装
public:
	virtual ~CLimitWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

