#pragma once

// CHistoryList

typedef struct{
	WORD	year;			// �N
	BYTE	month;			// ��
	BYTE	day;			// ��
	BYTE	hour;			// ��
	BYTE	minute;			// ��
	BYTE	second;			// �b
	BYTE	station;		// ��
	BYTE	equipment;		// ���M��
	BYTE	imstatus;		// �ݔ��d�v�x
	BYTE	status;			// �X�e�[�^�X
	DWORD	message;		// ���b�Z�[�WID
	TCHAR	msgstring[1024];
	UINT	searchtype;
} stSearchHistoryData;

class SearchSort{
public:
	bool operator()(const stSearchHistoryData& left, const stSearchHistoryData& right) const {
		if (left.year >= right.year)
			return true;
		if (left.month >= right.month)
			return true;
		if (left.day >= right.day)
			return true;
		if (left.hour >= right.hour)
			return true;
		if (left.minute >= right.minute)
			return true;
		if (left.second >= right.second)
			return true;
		if (left.station >= right.station)
			return true;
		return false;
	}
};

class CHistoryList : public CListCtrl
{
	DECLARE_DYNAMIC(CHistoryList)

public:
	CHistoryList();
	virtual ~CHistoryList();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CWnd*	mParentWnd;
	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	CsvOutput(CString& strTitle);
	void	PrintOutput(CString& strTitle, CString& strFilter, int nFilterType, stFilterData& data);
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

class CHistoryPrint
{
public:
	CHistoryPrint(CDC* dc);
	virtual ~CHistoryPrint();
	void	CalcRect(CListCtrl* plist);
	void	SetTitle(CString& val, CString& valfilter) { mTitle = val; mFilterTitle = valfilter; }
	BOOL	StartPrinting(CListCtrl* plist);
	void	EndPrinting();
	void	PrintLine(const CString& str);
	void	PrintLine(const int line);
	void	PrintHeader(const CString& str);
	void	SetPrinterFont();
	CFont*	CreateFont(int nPoints, int lfWeight);
	int		GetPageNumber() { return mPageNumber; }
protected:
	CString	mTitle, mFilterTitle;
	int		mMaxCol;
	CListCtrl*	mpList;
	CDC*	mDc;
	int		mPageVMargin;
	int		mPageHMargin;
	int		mPageHeight;
	int		mPageWidth;
	int		mSaveVMargin;
	int		mLineHeight;
	BOOL	mPageStarted;
	BOOL	mDocStarted;
	int		mPageNumber;
	vector<int>		mWidthList;

	CFont*	mFontHeader;
	CFont*	mFontSubHeader;
	CFont*	mFontBody;
};
