#pragma once

#include <uxtheme.h>
#include <vssym32.h>
#pragma comment(lib,"uxtheme.lib")
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

enum eTimeLineWindowID{
	eTimeLineCtrl_AntEmg = (WM_USER + 1000),
	eTimeLineCtrl_SndStop,
	eTimeLineCtrl_SnapShot,
	eTimeLineCtrl_Sound,
	eTimeLineCtrl_Mute,
	eTimeLineCtrl_Help,
};

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
protected:
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

// CMFCCaptionBarEx

class CMFCCaptionBarEx : public CMFCCaptionBar
{
	DECLARE_DYNAMIC(CMFCCaptionBarEx)

public:
	CMFCCaptionBarEx();
	virtual ~CMFCCaptionBarEx();

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	CMFCButtonEx& GetSoundBtnCtrl() { return mButtonSound; }	// [サウンド]ボタン
	CMFCButtonEx& GetMuteBtnCtrl() { return mButtonMute; }	// [サウンド]ボタン


protected:
	int		OnDrawTime(CDC* pDC, bool bUtc, bool bJst);

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
protected:
	CMFCButtonEx	mButtonAnt;		// アンテナ強制停止
	CMFCButtonEx	mButtonSend;	// 送信機停止
	CMFCButtonEx	mButtonShot;	// [スクリーン]ボタン
	CMFCButtonEx	mButtonSound;	// [サウンド]ボタン
	CMFCButtonEx	mButtonMute;	// [ミュート]ボタン
	CMFCButtonEx	mButtonHelp;	// [？]ボタン

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnDrawButton(CDC* pDC, CRect rect, const CString& strButton, BOOL bEnabled);
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


