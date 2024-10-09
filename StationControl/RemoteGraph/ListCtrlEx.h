#pragma once

#include <vector>

using namespace std;

//=============================================================================
// CColorComboBox
//=============================================================================
//!< カラーコンボボックスコントロール
class CColorComboBox : public CComboBox
{
	// コンストラクション
public:
	CColorComboBox();

	// アトリビュート
protected:
	vector<COLORREF>	m_colorlist;
	COLORREF	m_color;
	COLORREF	m_colorOther;

public:

	// オペレーション
public:
	void		SetColorList(vector<CString> list);
	void		SetColorList(vector<COLORREF> list);
	vector<COLORREF>&	GetColorList() { return m_colorlist; }
	void		SetColor(COLORREF color)	{ m_color = color; }
	COLORREF	GetColor() { return m_color; }
	void		SetOtherColor(COLORREF color)	{ m_colorOther = color; }
	COLORREF	GetOtherColor() { return m_colorOther; }

	// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CColorComboBox)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// インプリメンテーション
public:
	virtual ~CColorComboBox();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CColorComboBox)
	// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

class CListComboEx : public CComboBox
{
	// コンストラクション
public:
	CListComboEx(CWnd* pmain, int iItem, int iSubItem, vector<CString>& itemList, int nSel, UINT id);

	// アトリビュート
public:
	CWnd*		m_pMain;
	UINT		m_uID;
	int			m_iItem;
	int 		m_iSubItem;
	vector<CString>	m_ItemList;
	int			m_nSel;
	BOOL		m_bESC;				// To indicate whether ESC key w
	// オペレーション
public:

	// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CListComboEx)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// インプリメンテーション
public:
	virtual ~CListComboEx();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CListComboEx)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNcDestroy();
	afx_msg void OnCloseup();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

// CListCtrlEx

class CListCtrlEx : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlEx)

public:
	CListCtrlEx();
	virtual ~CListCtrlEx();

	BOOL m_bColor;
	BOOL m_bModifyFont;

protected:
	COLORREF OnGetCellBkColor(int nRow, int nColum);

	BOOL GetCellRect(int iRow, int iCol, int nArea, CRect &rect);

public:
	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
	void	SetParent(CWnd* p) { m_pParent = p; }
	int		HitTestEx(CPoint &point, int *col);
	template <class T>
	CComboBox*	ShowInPlaceList(int nItem, int nCol, T& lstItems, int nSel);
	void SetCelColor(int cel, vector<COLORREF>& list) { m_ColorList = list; }
	COLORREF GetCelColorAt(int row);
	void SetIsDataSetting(int cel, vector<CString>& list);

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
protected:
	CWnd*						m_pParent;
	vector<CMFCColorButton*>	m_CBtnList;
	vector<COLORREF>			m_ColorList;
	vector<CString>				m_CombSelList;
	vector<CString>				m_DataNameList;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};


