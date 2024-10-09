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

// RemoteGraphView.h : CRemoteGraphView �N���X�̃C���^�[�t�F�C�X
//

#pragma once

#include "GraphWindow.h"

class CRemoteGraphView : public CView
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CRemoteGraphView();
	DECLARE_DYNCREATE(CRemoteGraphView)

// ����
public:
	CRemoteGraphDoc* GetDocument() const;

	CGraphWindow	m_Graph;
	UINT			m_nViewType;

// ����
public:
	void	CreateGraph(UINT nViewID, UINT nViewType, UINT nWindowNo);
	void	DeleteGraph();
	void	SetViewType(UINT val);
//	void	ShowScroll(BOOL bShow)	{ if( m_Graph.GetSafeHwnd() ) m_Graph.ShowScrollBar(SB_HORZ, bShow); }
	void	CreateMinMaxValue()	{ if( m_Graph.GetSafeHwnd() ) m_Graph.CreateMinMaxValue(); }
	void	ResetRectangle(unsigned __int64 left, unsigned __int64 right)	{ if( m_Graph.GetSafeHwnd() ) m_Graph.ResetRectangle(m_Graph.ConvNoToX(left), m_Graph.ConvNoToX(right)); }
	void	ResetCursor(unsigned __int64 pos)	{ if( m_Graph.GetSafeHwnd() ) m_Graph.ResetCursor((pos == -1) ? -1 : m_Graph.ConvNoToX(pos)); }
	void	ResetDiffer()	{ if( m_Graph.GetSafeHwnd() ) m_Graph.ResetDiffer(true, true); }
	void	GetMinMaxValue(double& min, double& max)	{ if( m_Graph.GetSafeHwnd() ) m_Graph.GetMinMaxValue(min, max); }
	void	GetMinMaxValueEx(double& min, double& max)	{ if( m_Graph.GetSafeHwnd() ) m_Graph.GetMinMaxValueEx(min, max); }

// �I�[�o�[���C�h
public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
public:
	virtual ~CRemoteGraphView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int	m_angle[32];	// �Ȃ񂿂���ăf�[�^�p
	float	m_fLogData;
//	void	GetDummyData(int pos, float& data);

	static BOOL	m_bUsedClassFirst;
	static int m_initAngle[32];

#if 0
public:
	CEvent			m_evtUpdate;
	BOOL			m_bUpdateThread;
	CWinThread		*m_pUpdateThread;
#endif

public:
	BOOL	AddLine(UINT nID, CString strName, CString strGName, UINT nGraphKind, COLORREF crColor, DWORD nStyle, int nWidth, VScaleKind nVScaleKind, BOOL bVisible);
	BOOL	AddDot(UINT nI, CString strName, CString strGName, UINT nGraphKind, COLORREF crColor, int nDotSize, VScaleKind nVScaleKind, BOOL bVisible);
	BOOL	AddLineAndDot(UINT nID, CString strName, CString strGName, UINT nGraphKind, COLORREF crColor, DWORD nStyle, int nWidth, int nDotSize, VScaleKind nVScaleKind, BOOL bVisible);
	BOOL	AddAxis(stVScale nStvScale, HScale trandHScale);

	void	UpdateGraphWindow(int nViewNo, bool bClear);
	void	SetRealTime(BOOL val);

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // RemoteGraphView.cpp �̃f�o�b�O �o�[�W����
inline CRemoteGraphDoc* CRemoteGraphView::GetDocument() const
   { return reinterpret_cast<CRemoteGraphDoc*>(m_pDocument); }
#endif

