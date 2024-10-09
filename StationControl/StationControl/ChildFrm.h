
// ChildFrm.h : CChildFrame �N���X�̃C���^�[�t�F�C�X
//

#pragma once

#include "MFCCaptionBarEx.h"

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// ����
public:
	CMFCCaptionBarEx	m_wndCaption;
	CFont				mCaptionFont;
	UINT				mStation;
	int					mSizeX, mSizeY;

// ����
public:
	void	SetFrameMaxSize(int x, int y)	{ mSizeX = x; mSizeY = y; }

	void LoadRect(CArchive& ar, RECT& rect)
	{
		ar >> rect.left;
		ar >> rect.top;
		ar >> rect.right;
		ar >> rect.bottom;
	}
	void LoadPoint(CArchive& ar, POINT& point) const
	{
		ar >> point.x;
		ar >> point.y;
	}
	bool Load(CArchive& ar)
	{
		WINDOWPLACEMENT wp;
		memset(&wp, 0, sizeof(WINDOWPLACEMENT));
		wp.length = sizeof(WINDOWPLACEMENT);

		try
		{
			ar >> wp.flags;
			ar >> wp.showCmd;
			LoadPoint(ar, wp.ptMinPosition);
			LoadPoint(ar, wp.ptMaxPosition);
			LoadRect(ar, wp.rcNormalPosition);
			SetWindowPlacement(&wp);
		}
		catch (CArchiveException* e)
		{
			e->Delete();
			return false;
		}
		return true;
	}

	void SaveRect(CArchive& ar, RECT& rect) const
	{
		ar << rect.left;
		ar << rect.top;
		ar << rect.right;
		ar << rect.bottom;
	}
	void SavePoint(CArchive& ar, POINT& point) const
	{
		ar << point.x;
		ar << point.y;
	}
	bool Save(CArchive& ar) const
	{
		WINDOWPLACEMENT wp;
		memset(&wp, 0, sizeof(WINDOWPLACEMENT));
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(&wp);

		try
		{
			ar << wp.flags;
			ar << wp.showCmd;
			SavePoint(ar, wp.ptMinPosition);
			SavePoint(ar, wp.ptMaxPosition);
			SaveRect(ar, wp.rcNormalPosition);
		}
		catch (CArchiveException* e)
		{
			e->Delete();
			return false;
		}
		return true;
	}

// �I�[�o�[���C�h
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnClose();
};
