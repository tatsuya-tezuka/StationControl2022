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
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	void	SetChildCaption(bool val)	{ mChildCaption = val; }
	CMFCButtonEx &GetSoundBtnCtrl() { return mButtonSound; }	// [サウンド]ボタン
	CMFCButtonEx &GetMuteBtnCtrl() { return mButtonMute; }	// [ミュート]ボタン
	UINT	GetStationNo() { return mStationNo; }
	void	SetStationNo(UINT val)	{ mStationNo = val; }


protected:
	int		OnDrawTime(CDC* pDC, bool bUtc, bool bJst);

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
protected:
	CMFCButtonEx	mButtonAnt;		// アンテナ強制停止
	CMFCButtonEx	mButtonSend;	// 送信機停止
	bool			mChildCaption;
	CMFCButtonEx	mButtonShot;	// [スクリーン]ボタン
	CMFCButtonEx	mButtonSound;	// [サウンド]ボタン
	CMFCButtonEx	mButtonMute;	// [ミュート]ボタン
	CMFCButtonEx	mButtonHelp;	// [？]ボタン
	UINT			mStationNo;		// 局番号

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnDrawButton(CDC* pDC, CRect rect, const CString& strButton, BOOL bEnabled);
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
