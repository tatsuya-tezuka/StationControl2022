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

#include "ViewTree.h"
#include "MemMapList.h"

#ifdef _ENGDLL
#include "EngValCnv.h"
#else
#include "EngValConvApi.h"
#endif

using namespace std;

typedef struct {
	int			stno;
	CString		year;
	CString		month;
	CString		day;
} stSelectPath;

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// コンストラクション
public:
	CFileView();

	void AdjustLayout();
	void OnChangeVisualStyle();

// 属性
protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CViewTree			m_wndFileView;
	CImageList			m_FileViewImages;
	CFileViewToolBar	m_wndToolBar;
	CString				m_strHitItemName;
	stSelectPath		m_stHitItemPath;
	vector<CString>		m_MonDataFileList;		// 工学値変換対象のファイルリスト
	int					m_nActiveNo;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetActiveNo(int pos) { m_nActiveNo = pos; }
	int	GetActiveNo() { return m_nActiveNo; }
	void	SetHitItemName(CString str) { m_strHitItemName = str; }
	CString GetHitItemName() { return m_strHitItemName; }
	stSelectPath& GetHitItemPath() { return m_stHitItemPath; }
	void	FillFileView();

protected:
	void		CreateServerFileList(HTREEITEM hRoot, CString path, int nStationNo);
	bool		FindMonitorData(CString fname, int nStationNo, vector<CString>& outFileList);
	void		GetDataFileList(CString& path, vector<CString>& outFileList);
	bool		GetRangeTimeMonitorData(SYSTEMTIME& stdate, SYSTEMTIME& spdate, int nStationNo);
	int			GetStationNo(CString& name);
	void		ExpandAll(HTREEITEM hItem);
	HTREEITEM	FindItemText(HTREEITEM hRoot, CString& name);
	static void CALLBACK receiveCallBack(TCHAR* monName, time_t monTime, xobs_tbl_res_t* monData, bool bLeapSec);

// 実装
public:
	virtual ~CFileView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateOpen(CCmdUI *pCmdUI);
};

