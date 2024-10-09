#pragma once

#include <uxtheme.h>
#include <vssym32.h>
#pragma comment(lib,"uxtheme.lib")
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;


// CGridButton
static const UINT mWidth = 80;
static const UINT mHeight = 28;
static const UINT mOffset = 1;
static const UINT mHCount = 6;
static const UINT mVCount = 9;

enum{
	eButtonStatus_Close = 0,	// LAN切断状態：[ CLOSE]		グレー
//	eButtonStatus_Off = 1,		// 電源OFF：	[  OFF ]		青
	eButtonStatus_Alaram = 1,	// アラーム：	[ ALARM]		赤
	eButtonStatus_Maint = 2,	// ワーニング：	[ MAINT]		黄
	eButtonStatus_Local = 3,	// ローカル：	[ LOCAL]		橙
	eButtonStatus_Remote = 4,	// REMOTE状態／その他：	[REMOTE] / [  ON  ] 緑
	eButtonStatus_None = 5,		// 設備なし
	eButtonStatus_Max = 5,		// 設備状況文字列定義数
	eButtonStatus_ColMax = 6,	// 設備の色数
};

static const COLORREF mStatusColor[] = {
	RGB(128, 128, 128),		// グレー
//	RGB(0, 0, 255),			// 青
	RGB(255, 0, 0),			// 赤
	RGB(255, 255, 0),		// 黄
	RGB(255, 165, 0),		// 橙
	RGB(0, 255, 0),			// 緑
	RGB(0, 64, 128),		// 設備無し
};

static const char* mEquipmentStatusRcvString[] =
{
	"[ CLOSE]",
//	"[  OFF ]",
	"[ ALARM]",
	"[ MAINT]",
	"[ LOCAL]",
	"[REMOTE]",
};

typedef struct {
	COLORREF	color;
	CPen		pen;
} stGridLineObject;
typedef struct {
	Color	upper;
	Color	lower;
	Color	hot;
} stGradientColor;

class CGridButton : public CMFCButton
{
	DECLARE_DYNAMIC(CGridButton)

public:
	CGridButton();
	virtual ~CGridButton();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	COLORREF	m_crEnable, m_crHot, m_crDisable, m_crInvert;
	stGradientColor	m_GColor[eButtonStatus_ColMax];
	bool		m_bGradient;
	bool		m_bControl;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	UINT	mStatus;
	CString	mText;

	// オーナー描画
	ULONG_PTR			m_gdiplusToken;
	GdiplusStartupInput	gdiplusStartupInput;
	float				m_gradientAngle;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	UINT	GetStatus()			{ return mStatus; }
	void	SetStatus(UINT val)
	{ 
		mStatus = val;
		SetTextColor(RGB(0, 0, 0));
		if (mText.IsEmpty() == false){
			//SetFaceColor(mStatusColor[mStatus]);
			Invalidate();
		}
	}
	void	SetText(CString val)	{ mText = val; SetWindowText(mText); }
	CString	GetText()				{ return mText; }
	bool	GetControl()			{ return m_bControl; }
protected:
	COLORREF	lightenColor(const COLORREF crColor, BYTE blend);
	COLORREF	darkenColor(const COLORREF crColor, BYTE blend);

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnFillBackground(CDC* pDC, const CRect& rectClient);
	virtual void OnDrawText(CDC* pDC, const CRect& rect, const CString& strText, UINT uiDTFlags, UINT uiState);
	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	/* ------------------------------------------------------------------------------------ */
};


