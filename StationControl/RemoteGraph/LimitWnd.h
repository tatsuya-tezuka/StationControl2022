// ���� MFC �T���v�� �\�[�X �R�[�h�ł́AMFC Microsoft Office Fluent ���[�U�[ �C���^�[�t�F�C�X 
// ("Fluent UI") �̎g�p���@�������܂��B���̃R�[�h�́AMFC C++ ���C�u���� �\�t�g�E�F�A�� 
// ��������Ă��� Microsoft Foundation Class ���t�@�����X����ъ֘A�d�q�h�L�������g��
// �⊮���邽�߂̎Q�l�����Ƃ��Ē񋟂���܂��B
// Fluent UI �𕡐��A�g�p�A�܂��͔z�z���邽�߂̃��C�Z���X�����͌ʂɗp�ӂ���Ă��܂��B
// Fluent UI ���C�Z���X �v���O�����̏ڍׂɂ��ẮAWeb �T�C�g
// http://go.microsoft.com/fwlink/?LinkId=238214 ���Q�Ƃ��Ă��������B
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#pragma once

#include "const.h"

/////////////////////////////////////////////////////////////////////////////
// CLimitList �E�B���h�E

class CLimitList : public CListBox
{
// �R���X�g���N�V����
public:
	CLimitList();

// ����
public:
	virtual ~CLimitList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewLimit();

	DECLARE_MESSAGE_MAP()
};


class CLimitListCtrl : public CListCtrl
{
public:
	CLimitListCtrl();
	virtual ~CLimitListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
};

// CMFCLimitListCtrlEx

class CMFCLimitListCtrlEx : public CMFCListCtrl
{
	DECLARE_DYNAMIC(CMFCLimitListCtrlEx)

public:
	CMFCLimitListCtrlEx();
	virtual ~CMFCLimitListCtrlEx();

public:
	int	m_pos;

protected:
	DECLARE_MESSAGE_MAP()
};


// CMFCLimitTabCtrlEx

class CMFCLimitTabCtrlEx : public CMFCTabCtrl
{
	DECLARE_DYNAMIC(CMFCLimitTabCtrlEx)

public:
	CMFCLimitTabCtrlEx();
	virtual ~CMFCLimitTabCtrlEx();

#if 0
	virtual void FireChangeActiveTab(int nNewTab);
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


class CLimitWnd : public CDockablePane
{
// �R���X�g���N�V����
public:
	CLimitWnd();

	void UpdateFonts();

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	void	AddTab(UINT pos);
	void	UpdateDataId(UINT pos);
	void	UpdateTab(UINT pos);
	void	DeleteTab(UINT pos);
	void	UpdateDataEx(UINT pos);
	int		AddItem(int pos, int nItem, int nSubItem, CString strItem, DWORD dwAll = 0);
	int	SearchTabIndex(CString str)
	{
		for (int i = 0; i < m_wndTabs.GetTabsNum(); i++)
		{
			CString	strTabName;
			m_wndTabs.GetTabLabel(i, strTabName);
			if( strTabName == str )
				return i;
		}

		return 0;
	}

protected:

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:

protected:


// ����
protected:
	CMFCLimitTabCtrlEx	m_wndTabs;

	CLimitList m_wndLimitBuild;
	CLimitList m_wndLimitDebug;
	CLimitList m_wndLimitFind;
	CRect	m_rectList;
	CMFCLimitListCtrlEx	m_ListLimit[MAX_VIEW];

	bool	m_listDisplayTab[MAX_VIEW];
	DWORD	m_listDataId[MAX_VIEW][MAX_TDATA];

protected:
	void FillBuildWindow();
	void FillDebugWindow();
	void FillFindWindow();

	void AdjustHorzScroll(CListBox& wndListBox);

// ����
public:
	virtual ~CLimitWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

