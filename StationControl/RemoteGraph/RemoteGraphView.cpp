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

// RemoteGraphView.cpp : CRemoteGraphView �N���X�̎���
//

#include "stdafx.h"
// SHARED_HANDLERS �́A�v���r���[�A�k���ŁA����ь����t�B���^�[ �n���h���[���������Ă��� ATL �v���W�F�N�g�Œ�`�ł��A
// ���̃v���W�F�N�g�Ƃ̃h�L�������g �R�[�h�̋��L���\�ɂ��܂��B
#ifndef SHARED_HANDLERS
#include "RemoteGraph.h"
#endif

#include "MainFrm.h"

#include "RemoteGraphDoc.h"
#include "RemoteGraphView.h"

#include <iostream>
#include <string>
#include <fstream> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// �Ȃ񂿂���ăf�[�^�p�C��
BOOL CRemoteGraphView::m_bUsedClassFirst = TRUE;
int CRemoteGraphView::m_initAngle[];


#if 0
UINT CRemoteGraphViewUpdateThread_Trend(LPVOID pParam)
{
	CRemoteGraphView	*pWnd = (CRemoteGraphView*)pParam;

	while (TRUE)
	{
		WaitForSingleObject(((CMainFrame*)(theApp.m_pMainWnd))->GetUpdateEvent()->m_hObject, INFINITE);

		if (pWnd->m_bUpdateThread == FALSE)
		{
			break;
		}

		// �Ď��f�[�^���X�V���ꂽ���߁A�r���[�ɍX�V��ʒm
		pWnd->UpdateGraph();
	}

	return 0;
}
#endif

// CRemoteGraphView

IMPLEMENT_DYNCREATE(CRemoteGraphView, CView)

BEGIN_MESSAGE_MAP(CRemoteGraphView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CRemoteGraphView �R���X�g���N�V����/�f�X�g���N�V����

CRemoteGraphView::CRemoteGraphView()
{
	// TODO: �\�z�R�[�h�������ɒǉ����܂��B

}

CRemoteGraphView::~CRemoteGraphView()
{
}

BOOL CRemoteGraphView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

// CRemoteGraphView �`��

void CRemoteGraphView::OnDraw(CDC* /*pDC*/)
{
	CRemoteGraphDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B
}

void CRemoteGraphView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRemoteGraphView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CRemoteGraphView �f�f

#ifdef _DEBUG
void CRemoteGraphView::AssertValid() const
{
	CView::AssertValid();
}

void CRemoteGraphView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRemoteGraphDoc* CRemoteGraphView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRemoteGraphDoc)));
	return (CRemoteGraphDoc*)m_pDocument;
}
#endif //_DEBUG



void CRemoteGraphView::CreateGraph(UINT nViewID, UINT nViewType, UINT nWindowNo)
{
	CRect rect;
	GetClientRect(&rect);
	m_Graph.SetViewID(nViewID);
	//m_nViewType = eGraphType_All;
	m_nViewType = nViewType;
	m_Graph.SetGraphType(m_nViewType);
	m_Graph.Create(WS_VISIBLE, rect, this, mCtrlTrend, nWindowNo);
	m_Graph.SetGridType(CGraphWindow::eGridType_DOT);

#if 0
	// �C�x���g�̃X���b�h�쐬
	CEvent m_evtUpdate = CEvent(FALSE, FALSE, _T("CRemoteGraphView_UpdateEvent"));
	m_bUpdateThread = TRUE;
	m_pUpdateThread = AfxBeginThread(CRemoteGraphViewUpdateThread_Trend, this);
#endif
}

void CRemoteGraphView::DeleteGraph()
{
#if 0
	m_bUpdateThread = FALSE;
#endif
	m_Graph.DestroyWindow();
#if 0
	delete m_pUpdateThread;
#endif
}

#if 0
void CRemoteGraphView::GetDummyData(int pos, float& data)
{
	if (pos == 0){
		for (int i = 0; i < sizeof(m_angle) / sizeof(m_angle[0]); ++i) {
			m_angle[i] = (m_angle[i] + 2 * (i + 1)) % 360;
			//fprintf_s(m_fp, "View(%d), pos(%d), m_angle[%d][%d]=%d\n", m_nViewType, pos, m_nViewType, i, m_angle[m_nViewType][i]);
		}
	}
	data = (float)((sin(m_angle[pos] * 3.1415926538 / 180.0) + 1) / 2 * (100 - 0) + 0);
	//fprintf_s(m_fp, "View(%d), pos(%d), data=%f\n", m_nViewType, pos, data);

	if (pos == 3){
		data = m_fLogData;
		if (m_fLogData == 0.0f)
			m_fLogData = 10.0f;
		else
			m_fLogData = 0.0f;
	}
}
#endif

void CRemoteGraphView::SetViewType(UINT val)
{
	m_nViewType = val;
	switch (val){
	case	eGraphType_OffAll:
	case	eGraphType_OffTrend:
		m_Graph.SetGraphType(m_nViewType);
		m_Graph.SetHzRange((double)CSatelliteData::mMaxDataCount);
		m_Graph.UpdateGraph();
		break;
	default:
		break;
	}
}


// CRemoteGraphView ���b�Z�[�W �n���h���[

void CRemoteGraphView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// �Ȃ񂿂���ėp�f�[�^�̏����f�[�^�l�̓N���X�ϐ��ŁA�S�g�����h�͈̓g�����h���킹�Ĉ��̂ݍ쐬
	if (m_bUsedClassFirst) {
		for (int i = 0; i < sizeof(m_angle) / sizeof(m_angle[0]); ++i) {
			m_initAngle[i] = rand() * 360 / RAND_MAX;
		}
		m_bUsedClassFirst = FALSE;
	}

	for (int i = 0; i < sizeof(m_angle) / sizeof(m_angle[0]); ++i) {
		m_angle[i] = m_initAngle[i];
	}

	m_fLogData = 0.0f;
}


void CRemoteGraphView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CRect rectCtrl;
	CRect rect;

	// �O���t�̈�̃��T�C�Y
	if (m_Graph.GetSafeHwnd()){
		m_Graph.GetWindowRect(&rectCtrl);
		ScreenToClient(rectCtrl);
		GetClientRect(&rect);
		rect.left = rectCtrl.left;
		rect.top = rectCtrl.top;
		//rect.bottom -= 50;
		m_Graph.MoveWindow(&rect);

		// �Y�[���g���X�V
		CWnd	*pWnd = GetParent();

		if( pWnd->GetSafeHwnd() != NULL )
		{
			pWnd = pWnd->GetParent();

			if( pWnd->GetSafeHwnd() != NULL )
			{
				// �Ď��f�[�^���X�V���ꂽ���߁A�r���[�ɍX�V��ʒm
				if( !m_Graph.GetRealTime() )
				{
					pWnd->PostMessage(eMessage_UpdateZoomRectangle, 0);
					pWnd->PostMessage(eMessage_UpdateCursorLine, 1);
				}
			}
		}
	}
}


BOOL CRemoteGraphView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CView::OnEraseBkgnd(pDC);
}

/*============================================================================*/
/*! �O���t�r���[

-# �I�����C���Ȃ�΍ĕ`�悷��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CRemoteGraphView::UpdateGraphWindow(int nViewNo, bool bClear)
{
//	if (((CMainFrame*)theApp.m_pMainWnd)->GetOnline() == TRUE || bForce == TRUE)
	//if (((CMainFrame*)theApp.m_pMainWnd)->GetOnline() == TRUE)
	{
		if (nViewNo >= 0)
			m_Graph.SetViewID(nViewNo);

		// Min/Max���쐬
		m_Graph.CreateMinMaxValue(bClear);

		// �ĕ`�悷��
		m_Graph.UpdateGraph();
	}
}

/*============================================================================*/
/*! �O���t�r���[

-# �I�����C���Ȃ�΍ĕ`�悷��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CRemoteGraphView::SetRealTime(BOOL val)
{
	m_Graph.SetRealTime(val);
}

BOOL CRemoteGraphView::AddLine(UINT nID, CString strName, CString strGName, UINT nGraphKind, COLORREF crColor, DWORD nStyle, int nWidth, VScaleKind nVScaleKind, BOOL bVisible)
{
	return m_Graph.AddLine(nID, strName, strGName, nGraphKind, crColor, nStyle, nWidth, nVScaleKind, bVisible);
}
BOOL CRemoteGraphView::AddDot(UINT nID, CString strName, CString strGName, UINT nGraphKind, COLORREF crColor, int nDotSize, VScaleKind nVScaleKind, BOOL bVisible)
{
	return m_Graph.AddDot(nID, strName, strGName, nGraphKind, crColor, nDotSize, nVScaleKind, bVisible);
}
BOOL CRemoteGraphView::AddLineAndDot(UINT nID, CString strName, CString strGName, UINT nGraphKind, COLORREF crColor, DWORD nStyle, int nWidth, int nDotSize, VScaleKind nVScaleKind, BOOL bVisible)
{
	return m_Graph.AddLineAndDot(nID, strName, strGName, nGraphKind, crColor, nStyle, nWidth, nDotSize, nVScaleKind, bVisible);
}
BOOL CRemoteGraphView::AddAxis(stVScale nStvScale, HScale trandHScale)
{
	return m_Graph.AddAxis(nStvScale, trandHScale);
}


void CRemoteGraphView::OnClose()
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CView::OnClose();
}

void CRemoteGraphView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch( nChar )
	{
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
	case VK_PRIOR:
	case VK_NEXT:
//		if( GetKeyState(VK_CONTROL) < 0 )
		{
			CWnd	*pWnd = GetParent();

			if( pWnd->GetSafeHwnd() != NULL )
			{
				pWnd = pWnd->GetParent();

				if( pWnd->GetSafeHwnd() != NULL )
					pWnd->PostMessage(eMessage_AnalysisKeyEvent, nChar);
			}
		}
		break;

	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CRemoteGraphView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if( GetKeyState(VK_CONTROL) < 0 )
	{
		CWnd	*pWnd = GetParent();

		if( pWnd->GetSafeHwnd() != NULL )
		{
			pWnd = pWnd->GetParent();

			if( pWnd->GetSafeHwnd() != NULL )
				pWnd->PostMessage(eMessage_AnalysisKeyEvent, (zDelta < 0) ? VK_DOWN : VK_UP);
		}
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
