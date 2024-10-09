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
	eButtonStatus_Close = 0,	// LAN�ؒf��ԁF[ CLOSE]		�O���[
//	eButtonStatus_Off = 1,		// �d��OFF�F	[  OFF ]		��
	eButtonStatus_Alaram = 1,	// �A���[���F	[ ALARM]		��
	eButtonStatus_Maint = 2,	// ���[�j���O�F	[ MAINT]		��
	eButtonStatus_Local = 3,	// ���[�J���F	[ LOCAL]		��
	eButtonStatus_Remote = 4,	// REMOTE��ԁ^���̑��F	[REMOTE] / [  ON  ] ��
	eButtonStatus_None = 5,		// �ݔ��Ȃ�
	eButtonStatus_Max = 5,		// �ݔ��󋵕������`��
	eButtonStatus_ColMax = 6,	// �ݔ��̐F��
};

static const COLORREF mStatusColor[] = {
	RGB(128, 128, 128),		// �O���[
//	RGB(0, 0, 255),			// ��
	RGB(255, 0, 0),			// ��
	RGB(255, 255, 0),		// ��
	RGB(255, 165, 0),		// ��
	RGB(0, 255, 0),			// ��
	RGB(0, 64, 128),		// �ݔ�����
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
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	COLORREF	m_crEnable, m_crHot, m_crDisable, m_crInvert;
	stGradientColor	m_GColor[eButtonStatus_ColMax];
	bool		m_bGradient;
	bool		m_bControl;
	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	UINT	mStatus;
	CString	mText;

	// �I�[�i�[�`��
	ULONG_PTR			m_gdiplusToken;
	GdiplusStartupInput	gdiplusStartupInput;
	float				m_gradientAngle;
	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
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


