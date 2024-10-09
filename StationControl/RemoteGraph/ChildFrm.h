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

// ChildFrm.h : CChildFrame �N���X�̃C���^�[�t�F�C�X
//

#pragma once

#include "const.h"

using namespace std;

#if 0
typedef struct {
	int			wndIdx;				// �E�B���h�[�ԍ�
	CString		wndName;			// �E�B���h�[����

	CString		itemName;			// �f�[�^����
	bool		isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
	bool		isViewMode;			// �\��On/Off
	DrawMode	drawMode;			// �`�惂�[�h�iLine�ADots�AL&D�j
	COLORREF	color;				// �\���F
	DotSize		dotSize;			// �h�b�g�T�C�Y
	bool		isVAxisAutRange;	// �c���I�[�g�X�P�[��
	float		VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
	float		VAxisLabelIntvl;	// �c�����x���̊Ԋu
	float		VAxisMaxRange;		// �c���ő�l
	float		VAxisMinRange;		// �c���ŏ��l
	bool		isHAxisAutRange;	// �����I�[�g�X�P�[��
	AxisUnit	HAxisUnit;			// �����P�ʁi�b/��/���j
	float		HAxisScale;			// �����\���X�P�[��
	float		HAxisScaleIntvl;	// �����ڐ���̊Ԋu
	float		HAxisLabelIntvl;	// �������x���̊Ԋu
} dataParam;
#endif

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// ����
public:
	CSplitterWndEx	m_wndSplitter;
	UINT			m_nID;
	unsigned __int64	m_nZoomStart;
	int	m_nZoomTblNo;
	unsigned __int64	m_nCursor;

// ����
public:
	void	GetMinMaxValue(double& min, double& max);
	void	GetMinMaxValueEx(double& min, double& max);


// �I�[�o�[���C�h
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL IsReadOnly() { return TRUE/*FALSE*/; }	// �E�B���h�E�_�C�A���O�̕ۑ��{�^������������

// ����
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// �r���[�Ƀp�����[�^��ǉ�����
	BOOL AddViewParam(CPropertiesData* pParam);
	void CChildFrame::UpdateDataCollection(bool bClear);

public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

public:
	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnDestroy();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
