#pragma once

#include "MFCMenuBarEx.h"

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
	void	SetChildCaption(bool val)	{ mChildCaption = val; }
	CMFCButtonEx &GetSoundBtnCtrl() { return mButtonSound; }	// [�T�E���h]�{�^��
	CMFCButtonEx &GetMuteBtnCtrl() { return mButtonMute; }	// [�~���[�g]�{�^��
	UINT	GetStationNo() { return mStationNo; }
	void	SetStationNo(UINT val)	{ mStationNo = val; }


protected:
	int		OnDrawTime(CDC* pDC, bool bUtc, bool bJst);

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
protected:
	CMFCButtonEx	mButtonAnt;		// �A���e�i������~
	CMFCButtonEx	mButtonSend;	// ���M�@��~
	bool			mChildCaption;
	CMFCButtonEx	mButtonShot;	// [�X�N���[��]�{�^��
	CMFCButtonEx	mButtonSound;	// [�T�E���h]�{�^��
	CMFCButtonEx	mButtonMute;	// [�~���[�g]�{�^��
	CMFCButtonEx	mButtonHelp;	// [�H]�{�^��
	UINT			mStationNo;		// �ǔԍ�

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnDrawButton(CDC* pDC, CRect rect, const CString& strButton, BOOL bEnabled);
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
