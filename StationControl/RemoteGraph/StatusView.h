#pragma once


// CMFCListCtrlEx

class CStatusList : public CMFCListCtrl
{
	DECLARE_DYNAMIC(CStatusList)

public:
	CStatusList();
	virtual ~CStatusList();

	BOOL m_bColor;
//	BOOL m_bModifyFont;

	void GetRealTimeStartString(CString& str);
	void GetRealTimeStopString(CString& str);

public:

protected:
	virtual COLORREF OnGetCellTextColor(int nRow, int nColum);
	virtual COLORREF OnGetCellBkColor(int nRow, int nColum);
//	virtual HFONT OnGetCellFont(int nRow, int nColum, DWORD dwData = 0);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};

// CStatusView

class CStatusView : public CDockablePane
{
	DECLARE_DYNAMIC(CStatusView)

public:
	CStatusView();
	virtual ~CStatusView();

	/* ------------------------------------------------------------------------------------ */
	/* ÉÅÉìÉoïœêî                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CFont			m_Font;
	CStatusList		m_wndStatus;

	/* ------------------------------------------------------------------------------------ */
	/* ÉÅÉìÉoä÷êî                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	UpdateStatus();
protected:
	void CStatusView::AutoSizeColumns(int col =-1);
	int AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam=0);
	void FillDebugWindow(CString& str);

	/* ------------------------------------------------------------------------------------ */

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
};


