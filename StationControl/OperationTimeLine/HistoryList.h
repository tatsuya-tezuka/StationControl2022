#pragma once

// CHistoryList

#define		eMessageTimeline_History	(WM_USER + 100)

class CHistoryList : public CListCtrl
{
	DECLARE_DYNAMIC(CHistoryList)

public:
	CHistoryList();
	virtual ~CHistoryList();

	/* ------------------------------------------------------------------------------------ */
	/* íËã`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */
	/* ÉÅÉìÉoïœêî                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CWnd*	mParentWnd;
	/* ------------------------------------------------------------------------------------ */
	/* ÉÅÉìÉoä÷êî                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetParent(CWnd* p) { mParentWnd = p; }
protected:
	/* ------------------------------------------------------------------------------------ */

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
};


