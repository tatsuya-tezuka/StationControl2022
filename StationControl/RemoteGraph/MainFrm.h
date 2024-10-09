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

// MainFrm.h : CMainFrame クラスのインターフェイス
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "LimitWnd.h"
#include "PropertiesWnd.h"
#include "StatusView.h"
//#include "DlgWait.h"

using namespace std;

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CWinThread	*m_pCheckDiskThread;
	bool	m_bCheckDiskStop;
protected:
	BOOL	m_bOnline;
	BOOL	m_bGraph[grapfmax];
	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
protected:
	void	OpenConfigFile(CString szFilePath);
	void	SaveConfigFile(CString szFilePath);

public:
	BOOL	GetOnline() { return m_bOnline; }
	void	OpenFile(CString strFileName);
	void	ShowChildFrame(int pos, BOOL bShow);
	BOOL	IsExistChildList(int pos);
	int		FindChildListPos(CWnd* pWnd);
	CWnd	*FindChildListWnd(int pos);
	int		GetCreateVewIdx() { return m_CreatingView; }
	UINT	GetActiveGraph() { return m_nActiveGraph; }

// 属性
public:

// 操作
public:
	CPropertiesWnd&		GetPropertiesWnd() { return m_wndProperties;}
	CFileView&			GetFileView() { return m_wndFileView; }

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 実装
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // コントロール バー用メンバー
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	CFileView			m_wndFileView;
	CClassView			m_wndClassView;
	COutputWnd			m_wndOutput;
	CLimitWnd			m_wndLimit;
	CPropertiesWnd		m_wndProperties;
	CStatusView			m_wndStatusView;

	map<UINT, LPARAM>	m_ChildWndList;
	UINT				m_nActiveGraph;
	int					m_CreatingView;

//	CDlgWait			*m_pDlgWait;

#if 0
	CEvent				m_evtUpdate;
#endif

public:
	CString				m_szCsvFile;

// 生成された、メッセージ割り当て関数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

	void UpdataeProperties(LPARAM lParam);
	void ResetChildList(UINT message, WPARAM wParam, LPARAM lParam);
	void PostMessageToChildList(UINT message, WPARAM wParam, LPARAM lParam);
	void SetTrendViewUpdateEvent();
	void	SetRealTimeRibbon();

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnButtonCsvOut();
	afx_msg void OnUpdateButtonCsvOut(CCmdUI *pCmdUI);
	afx_msg void OnButtonPrintOut();
	afx_msg void OnUpdateButtonPrintOut(CCmdUI *pCmdUI);
	afx_msg void OnButtonFileOpen();
	afx_msg void OnUpdateButtonFileOpen(CCmdUI *pCmdUI);
	afx_msg void OnButtonFileSave();
	afx_msg void OnUpdateButtonFileSave(CCmdUI *pCmdUI);
	afx_msg void OnButtonOnline();
	afx_msg void OnUpdateButtonOnline(CCmdUI *pCmdUI);
	afx_msg void OnButtonStation();
	afx_msg void OnUpdateButtonStation(CCmdUI *pCmdUI);

public:
	UINT GetChildWndListSize();
#if 0
	CEvent* GetUpdateEvent()	{ return &m_evtUpdate; };
#endif

	void PostMessageToOutput(UINT message, WPARAM wParam, LPARAM lParam)	{ m_wndOutput.PostMessage(message, wParam, lParam); }
	void PostMessageToLimit(UINT message, WPARAM wParam, LPARAM lParam)	{ m_wndLimit.PostMessage(message, wParam, lParam); }

public:
	afx_msg void OnCheckGraph1();
	afx_msg void OnCheckGraph2();
	afx_msg void OnCheckGraph3();
	afx_msg void OnCheckGraph4();
	afx_msg void OnCheckGraph5();
	afx_msg void OnCheckGraph6();
	afx_msg void OnCheckGraph7();
	afx_msg void OnCheckGraph8();
	afx_msg void OnCheckGraph9();
	afx_msg void OnUpdateCheckGraph1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCheckGraph2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCheckGraph3(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCheckGraph4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCheckGraph5(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCheckGraph6(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCheckGraph7(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCheckGraph8(CCmdUI *pCmdUI);
	afx_msg void OnViewProperties();
	afx_msg void OnUpdateViewProperties(CCmdUI *pCmdUI);
	afx_msg void OnButtonOnlineStop();
	afx_msg void OnUpdateButtonOnlineStop(CCmdUI *pCmdUI);
	afx_msg void OnViewDatalist();
	afx_msg void OnUpdateViewDatalist(CCmdUI *pCmdUI);
	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewOutput(CCmdUI *pCmdUI);
	afx_msg void OnViewLimit();
	afx_msg void OnUpdateViewLimit(CCmdUI *pCmdUI);
	afx_msg void OnViewStatus();
	afx_msg void OnUpdateViewStatus(CCmdUI *pCmdUI);
	afx_msg void OnBtnGraphClear1();
	afx_msg void OnBtnGraphClear2();
	afx_msg void OnBtnGraphClear3();
	afx_msg void OnBtnGraphClear4();
	afx_msg void OnBtnGraphClear5();
	afx_msg void OnBtnGraphClear6();
	afx_msg void OnBtnGraphClear7();
	afx_msg void OnBtnGraphClear8();
	afx_msg void OnUpdateBtnGraphClear1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBtnGraphClear2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBtnGraphClear3(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBtnGraphClear4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBtnGraphClear5(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBtnGraphClear6(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBtnGraphClear7(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBtnGraphClear8(CCmdUI *pCmdUI);
	afx_msg void OnClose();
};


