#pragma once

#define		mSenderMessage	(WM_USER + 10)

typedef struct {
	UINT	id;
	CString	name;
	bool	chack;
	bool	cancel;
} stSenderData;

// CSenderButton

class CSenderButton : public CMFCMenuButton
{
	DECLARE_DYNAMIC(CSenderButton)

public:
	CSenderButton();
	virtual ~CSenderButton();

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
protected:
	CMenu			mMainMenu, mPopMenu;
	stSenderData	mData[eSenderItemMax+1];
	static HWND		m_hWnd;

public:
	UINT			mCancelID;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
protected:
	void	SetTitle();

public:
	void	Create();
	void	ResetMenu();
	UINT	GetStatus();
	void	SetStatus(UINT val);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//-----------------------------------------------------------------------------
};



// CColorCheckButton

class CColorCheckButton : public CButton
{
	DECLARE_DYNAMIC(CColorCheckButton)

public:
	CColorCheckButton();
	virtual ~CColorCheckButton();

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
protected:
	int		m_nStation;

public:

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
protected:

public:
	void	SetStation(int val)		{ m_nStation = val; }
	//-----------------------------------------------------------------------------

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};


