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

// RemoteGraphDoc.cpp : CRemoteGraphDoc �N���X�̎���
//

#include "stdafx.h"
// SHARED_HANDLERS �́A�v���r���[�A�k���ŁA����ь����t�B���^�[ �n���h���[���������Ă��� ATL �v���W�F�N�g�Œ�`�ł��A
// ���̃v���W�F�N�g�Ƃ̃h�L�������g �R�[�h�̋��L���\�ɂ��܂��B
#ifndef SHARED_HANDLERS
#include "RemoteGraph.h"
#endif

#include "RemoteGraphDoc.h"
#include "RemoteGraphView.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#if 0
extern const dataParam sampleParam[8][10];
#else
// �E�B���h�E����
const CString wndName[MAX_VIEW] = {
	_T("�O���t�̈�1"),
	_T("�O���t�̈�2"),
	_T("�O���t�̈�3"),
	_T("�O���t�̈�4"),
	_T("�O���t�̈�5"),
	_T("�O���t�̈�6"),
	_T("�O���t�̈�7"),
	_T("�O���t�̈�8")
};
#endif


// CRemoteGraphDoc

IMPLEMENT_DYNCREATE(CRemoteGraphDoc, CDocument)

BEGIN_MESSAGE_MAP(CRemoteGraphDoc, CDocument)
END_MESSAGE_MAP()


// CRemoteGraphDoc �R���X�g���N�V����/�f�X�g���N�V����

CRemoteGraphDoc::CRemoteGraphDoc()
{
	// TODO: ���̈ʒu�� 1 �x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CRemoteGraphDoc::~CRemoteGraphDoc()
{
}

BOOL CRemoteGraphDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B


	return TRUE;
}




// CRemoteGraphDoc �V���A����

void CRemoteGraphDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �i�[����R�[�h�������ɒǉ����Ă��������B
	}
	else
	{
		// TODO: �ǂݍ��ރR�[�h�������ɒǉ����Ă��������B
	}
}

#ifdef SHARED_HANDLERS

//�k���ł̃T�|�[�g
void CRemoteGraphDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ���̃R�[�h��ύX���ăh�L�������g�̃f�[�^��`�悵�܂�
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �����n���h���[�̃T�|�[�g
void CRemoteGraphDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// �h�L�������g�̃f�[�^���猟���R���e���c��ݒ肵�܂��B
	// �R���e���c�̊e������ ";" �ŋ�؂�K�v������܂�

	// ��:      strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CRemoteGraphDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CRemoteGraphDoc �f�f

#ifdef _DEBUG
void CRemoteGraphDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRemoteGraphDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


void CRemoteGraphDoc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	//	CDocument::SetTitle(lpszTitle);

	// �E�B���h�E�ԍ����擾����
	UINT nID = ((CMainFrame*)(theApp.m_pMainWnd))->GetCreateVewIdx();

	// �E�B���h�E�ԍ���8�ȏ�Ȃ�΃E�B���h�E�͊J���Ȃ��̂Ŗ߂�
	if (nID >= MAX_VIEW) {
		return;
	}
#if 0
//	// �E�B���h�E�ԍ��ɑΉ�����p�����[�^�z����A�v������擾
//	CPropertiesData* pParam = &theApp.GetPropertiesData(nID);
//	CDocument::SetTitle(pParam->wndName);

	// �E�B���h�E�ԍ��ɑΉ�����O���t�p�����[�^���X�g�̎擾
	CPropertiesData* pParam;
	// �_�~�[�p�����[�^���擾
	pParam = new CPropertiesData();
	pParam->wndName = sampleParam[nID][0].wndName;

	CDocument::SetTitle(pParam->wndName);
	delete pParam;
#else
	// �E�B���h�E�ԍ��ɑΉ�����O���t�p�����[�^���X�g�̎擾
	//CPropertiesData* pParam;
	// �_�~�[�p�����[�^���擾
	//pParam = new CPropertiesData();
	//pParam->wndName = wndName[nID];
	//CDocument::SetTitle(pParam->wndName);
	//delete pParam;
	CDocument::SetTitle(wndName[nID]);
#endif
}


// CRemoteGraphDoc �R�}���h


void CRemoteGraphDoc::OnCloseDocument()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	CDocument::OnCloseDocument();
}
