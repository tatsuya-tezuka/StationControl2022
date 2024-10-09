#pragma once

typedef void (CALLBACK *SETUPDLGCALLBACK)(stFilterData& param, UINT type);

// CBarChild ダイアログ

class CBarChild : public CDialog
{
	DECLARE_DYNAMIC(CBarChild)

public:
	CBarChild(UINT IDD, CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CBarChild();

	static const UINT mStationMax = 4;
	static const UINT mStatusFlagMax = 4;
	static const UINT mImStatusKindMax = 3;
	static const UINT mStatusKindMax = 5;

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
protected:
	UINT		mMaxWidth, mMaxHeight;
	UINT		mActiveButton;

public:

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
protected:

public:
	virtual UINT	GetMaxWidth()	{ return mMaxWidth; }
	virtual UINT	GetMaxHeight()	{ return mMaxHeight; }
	virtual void	EnableControl(bool bEnable){};
	virtual UINT&	GetActiveButton() { return mActiveButton; }
	virtual void	ResetActiveButton() { mActiveButton = 0; }
	virtual void	ResetBanner(UINT maxWidth){};

	//-----------------------------------------------------------------------------

};


class CMFCTabInfo;

// CMFCTabCtrlEx

class CMFCTabCtrlEx : public CMFCTabCtrl
{
	friend class CMDIClientAreaWnd;

	DECLARE_DYNAMIC(CMFCTabCtrlEx)

public:
	CMFCTabCtrlEx();
	virtual ~CMFCTabCtrlEx();

	//-----------------------------------------------------------------------------
	//	定義
	//-----------------------------------------------------------------------------
	static const UINT mBarHeight = 22;
	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
protected:
	vector<CBarChild*>	mBarChildList;
	CWnd*		m_Parent;
	int			m_nTabShow;
	CRect		m_rectDefault;
	CString		m_strFilterText;
public:
	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
protected:
public:
	void	AddBar(CBarChild* pBar, CString str);
	void	Resize(int cx);
	void	TabChange(int pos, bool bShow = true);
	int		GetTabFromPointEx(CPoint& pt);
	void	CloseChildWindow(MSG* pMsg);
	UINT	GetMaxWidth();
	UINT	GetMaxHeight();
	void	SetFilterText(CString val)	{ m_strFilterText = val; Invalidate(); }
	//-----------------------------------------------------------------------------

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	virtual void Draw3DTab(CDC* pDC, CMFCTabInfo* pTab, BOOL bActive);
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnNcPaint();
};

//=============================================================================
// CGroupBoxEx
//=============================================================================
//!< 拡張グループボックスクラス
class CGroupBoxEx : public CButton
{
	DECLARE_DYNAMIC(CGroupBoxEx)

public:
	CGroupBoxEx();
	virtual ~CGroupBoxEx();

	enum eGroupType{
		eGroupType_None,
		eGroupType_Normal,
		eGroupType_Round,
		eGroupType_Box,
		eGroupType_BoxLine,
		eGroupType_Ribbon,
		eGroupType_Max,
	};

	void	SetTextColor(COLORREF color)	{ m_colorText = color; }
	void	SetBackColor(COLORREF color)	{ m_colorBack = color; }
	void	SetGroupType(eGroupType type)
	{
		m_typeGroup = type;
		if (type == eGroupType_Normal)
			m_colorBack = ::GetSysColor(COLOR_3DFACE);
	}
	void	SetRoundPoint(CPoint pt)		{ m_pointRound = pt; }
	void	SetFontBold(bool val)			{ m_bFontBold = val; }
	void	UpdateRect();

protected:
	CString		m_strText;
	CRect		m_rect;
	CPoint		m_pointRound;

	COLORREF	m_colorText;
	COLORREF	m_colorBack;
	eGroupType	m_typeGroup;
	bool		m_bFontBold;

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	afx_msg void OnPaint();
};
