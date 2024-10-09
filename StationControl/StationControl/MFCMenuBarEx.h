#pragma once


#include <uxtheme.h>
#include <vssym32.h>
#pragma comment(lib,"uxtheme.lib")
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

class CMFCButtonEx : public CMFCButton
{
	DECLARE_DYNAMIC(CMFCButtonEx)

public:
	CMFCButtonEx();
	virtual ~CMFCButtonEx();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	COLORREF	m_crEnable, m_crHot, m_crDisable;
	Color		m_crUpper, m_crLower, m_crLowerHot;
	bool		m_bGradient;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	bool		m_bButtonNormal;
protected :
	// オーナー描画
	ULONG_PTR			m_gdiplusToken;
	GdiplusStartupInput	gdiplusStartupInput;
	float				m_gradientAngle;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	CreateColors();
	void	ExecuteClicked(bool bPipe, int status);
protected:
	COLORREF	lightenColor(const COLORREF crColor, BYTE blend);
	COLORREF	darkenColor(const COLORREF crColor, BYTE blend);
	void		OnFillBackground(CDC* pDC, const CRect& rectClient);
	void		OnDraw(CDC* pDC, const CRect& rect, UINT uiState);
	void		OnDrawText(CDC* pDC, const CRect& rect, const CString& strText, UINT uiDTFlags, UINT uiState);

	/* ------------------------------------------------------------------------------------ */

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
public:
	afx_msg void OnBnClicked();
	virtual void PreSubclassWindow();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

// CMFCMenuBarEx

class CMFCMenuBarEx : public CMFCMenuBar
{
	DECLARE_DYNAMIC(CMFCMenuBarEx)

public:
	CMFCMenuBarEx();
	virtual ~CMFCMenuBarEx();

	CMFCButtonEx	mButtonAnt;		// アンテナ強制停止
	CMFCButtonEx	mButtonSend;	// 送信機停止

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual AFX_DOCK_TYPE GetDockingMode()const;
	virtual void SetPaneStyle(DWORD dwNewStyle);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCommandEmg();
	afx_msg void OnCommandStop();
	afx_msg void OnUpdateCommandEmg(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCommandStop(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


