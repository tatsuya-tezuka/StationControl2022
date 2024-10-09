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

#include "ViewTree.h"
#include "MemMapList.h"

#ifdef _ENGDLL
#include "EngValCnv.h"
#else
#include "EngValConvApi.h"
#endif

using namespace std;

typedef struct {
	int			stno;
	CString		year;
	CString		month;
	CString		day;
} stSelectPath;

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// �R���X�g���N�V����
public:
	CFileView();

	void AdjustLayout();
	void OnChangeVisualStyle();

// ����
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CViewTree			m_wndFileView;
	CImageList			m_FileViewImages;
	CFileViewToolBar	m_wndToolBar;
	CString				m_strHitItemName;
	stSelectPath		m_stHitItemPath;
	vector<CString>		m_MonDataFileList;		// �H�w�l�ϊ��Ώۂ̃t�@�C�����X�g
	int					m_nActiveNo;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetActiveNo(int pos) { m_nActiveNo = pos; }
	int	GetActiveNo() { return m_nActiveNo; }
	void	SetHitItemName(CString str) { m_strHitItemName = str; }
	CString GetHitItemName() { return m_strHitItemName; }
	stSelectPath& GetHitItemPath() { return m_stHitItemPath; }
	void	FillFileView();

protected:
	void		CreateServerFileList(HTREEITEM hRoot, CString path, int nStationNo);
	bool		FindMonitorData(CString fname, int nStationNo, vector<CString>& outFileList);
	void		GetDataFileList(CString& path, vector<CString>& outFileList);
	bool		GetRangeTimeMonitorData(SYSTEMTIME& stdate, SYSTEMTIME& spdate, int nStationNo);
	int			GetStationNo(CString& name);
	void		ExpandAll(HTREEITEM hItem);
	HTREEITEM	FindItemText(HTREEITEM hRoot, CString& name);
	static void CALLBACK receiveCallBack(TCHAR* monName, time_t monTime, xobs_tbl_res_t* monData, bool bLeapSec);

// ����
public:
	virtual ~CFileView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateOpen(CCmdUI *pCmdUI);
};

