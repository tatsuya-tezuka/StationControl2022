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
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	COLORREF	m_crEnable, m_crHot, m_crDisable;
	Color		m_crUpper, m_crLower, m_crLowerHot;
	bool		m_bGradient;
	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	bool		m_bButtonNormal;
protected:
	// �I�[�i�[�`��
	ULONG_PTR			m_gdiplusToken;
	GdiplusStartupInput	gdiplusStartupInput;
	float				m_gradientAngle;
	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
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
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	CMFCButtonEx& GetSoundBtnCtrl() { return mButtonSound; }	// [�T�E���h]�{�^��
	CMFCButtonEx& GetMuteBtnCtrl() { return mButtonMute; }	// [�T�E���h]�{�^��


protected:
	int		OnDrawTime(CDC* pDC, bool bUtc, bool bJst);

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
protected:
	CMFCButtonEx	mButtonAnt;		// �A���e�i������~
	CMFCButtonEx	mButtonSend;	// ���M�@��~
	CMFCButtonEx	mButtonShot;	// [�X�N���[��]�{�^��
	CMFCButtonEx	mButtonSound;	// [�T�E���h]�{�^��
	CMFCButtonEx	mButtonMute;	// [�~���[�g]�{�^��
	CMFCButtonEx	mButtonHelp;	// [�H]�{�^��

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnDrawButton(CDC* pDC, CRect rect, const CString& strButton, BOOL bEnabled);
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


