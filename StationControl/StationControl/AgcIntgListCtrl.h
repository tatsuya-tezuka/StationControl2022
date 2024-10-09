#pragma once

using namespace std;

#define eMessage_AgcStepOutOFRange (WM_USER+3600)	// 積分時間範囲外

struct stLISTDATA
{
	int row;
	int col;
};

enum eListEditType
{
	eListEditType_Char,
	eListEditType_Float,
	eListEditType_Exponent,
	eListEditType_Int,
	eListEditType_UInt,
	eListEditType_VectorMag,
};

class CListEdit : public CEdit
{
	// Construction
public:
	CListEdit(int iItem, int iSubItem, CString sInitText);

	// Attributes
public:
	BOOL	m_bKeyReturn;
	BOOL    m_bKeyShift;
	UINT	m_nDecimal;
	UINT	m_nNumberLimit;
	UINT	m_nEditType;
	// Operations
public:
	void	SetDecimal(UINT len);
	BOOL	CheckDecimal();
	BOOL	CheckInput(UINT nChar);
	BOOL	CheckRange(UINT nChar = 0x00);
	void	SetNumberLimit(UINT len);
	void	SetEditType(UINT type)	{ m_nEditType = type; }
	void	SetMinMax(double min, double max)	{ m_fMin = min, m_fMax = max; }

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListEdit)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CListEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int		m_iItem;
	int		m_iSubItem;
	CString m_sInitText;
	BOOL    m_bESC;
	BOOL	m_bNotify;
	double	m_fMin;
	double	m_fMax;
	BOOL	m_bInputError;
	CString m_strCurVal;		// 現在の積分時間値を保持
};


class CEditInListCtrl : public CEdit
{
public:
	// 省略
	int m_row;
	int m_col;
	// 省略
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};

class CButtonInListCtrl : public CButton
{
	// コンストラクション
public:
	CButtonInListCtrl(CWnd* pmain, int iItem, int iSubItem, UINT id);

	// アトリビュート
public:
	CWnd*		m_pMain;
	UINT		m_uID;
	int			m_iItem;
	int 		m_iSubItem;
	int			m_nSel;

	// インプリメンテーション
public:
	virtual ~CButtonInListCtrl();

	// 省略
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClicked();
};


// CAgcIntgListCtrl
class CAgcIntgListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CAgcIntgListCtrl)

public:
	CAgcIntgListCtrl();
	virtual ~CAgcIntgListCtrl();

public:
	CEditInListCtrl m_Edit;
	CListEdit		*m_ListEdit;

	CImageList		m_ImageList;

	void CreateCheckBox();
	void CheckBoxCheckStep(int col, int step);
	void BeginEdit(int iItem, int iSubItem);

protected:
	int HitTestEx(CPoint &point, int *col);
	BOOL GetCellRect(int iRow, int iCol, int nArea, CRect &rect);

	HIMAGELIST	CreateImageList(CDC* pdc);

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


