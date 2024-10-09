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
// COutputList �E�B���h�E

class COutputList : public CListBox
{
// �R���X�g���N�V����
public:
	COutputList();

// ����
public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};


class COutputListCtrl : public CListCtrl
{
public:
	COutputListCtrl();
	virtual ~COutputListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
};

// CMFCOutputListCtrlEx

class CMFCOutputListCtrlEx : public CMFCListCtrl
{
	DECLARE_DYNAMIC(CMFCOutputListCtrlEx)

public:
	CMFCOutputListCtrlEx();
	virtual ~CMFCOutputListCtrlEx();

public:
	int	m_pos;

protected:
	DECLARE_MESSAGE_MAP()
};


// CMFCOutputTabCtrlEx

class CMFCOutputTabCtrlEx : public CMFCTabCtrl
{
	DECLARE_DYNAMIC(CMFCOutputTabCtrlEx)

public:
	CMFCOutputTabCtrlEx();
	virtual ~CMFCOutputTabCtrlEx();

#if 0
	virtual void FireChangeActiveTab(int nNewTab);
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


class COutputWnd : public CDockablePane
{
// �R���X�g���N�V����
public:
	COutputWnd();

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
	CMFCOutputTabCtrlEx	m_wndTabs;

	COutputList m_wndOutputBuild;
	COutputList m_wndOutputDebug;
	COutputList m_wndOutputFind;
	CRect	m_rectList;
	CMFCOutputListCtrlEx	m_ListOutput[MAX_VIEW];

	bool	m_listDisplayTab[MAX_VIEW];
	DWORD	m_listDataId[MAX_VIEW][MAX_TDATA];

protected:
	void FillBuildWindow();
	void FillDebugWindow();
	void FillFindWindow();

	void AdjustHorzScroll(CListBox& wndListBox);

// ����
public:
	virtual ~COutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

