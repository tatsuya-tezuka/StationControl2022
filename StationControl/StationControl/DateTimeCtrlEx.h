#pragma once


// CDateTimeCtrlEx

class CDateTimeCtrlEx : public CDateTimeCtrl
{
	DECLARE_DYNAMIC(CDateTimeCtrlEx)

public:
	CDateTimeCtrlEx();
	virtual ~CDateTimeCtrlEx();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CWnd* m_pPostToWnd;			// 通知先ウィンドウ

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void SetPostToWnd(CWnd* pWnd)	{ m_pPostToWnd = pWnd; }
	CWnd* GetPostToWnd()			{ return m_pPostToWnd; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	/* ------------------------------------------------------------------------------------ */
};


