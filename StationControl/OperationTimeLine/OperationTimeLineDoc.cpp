
// OperationTimeLineDoc.cpp : COperationTimeLineDoc �N���X�̎���
//

#include "stdafx.h"
// SHARED_HANDLERS �́A�v���r���[�A�k���ŁA����ь����t�B���^�[ �n���h���[���������Ă��� ATL �v���W�F�N�g�Œ�`�ł��A
// ���̃v���W�F�N�g�Ƃ̃h�L�������g �R�[�h�̋��L���\�ɂ��܂��B
#ifndef SHARED_HANDLERS
#include "OperationTimeLine.h"
#endif

#include "OperationTimeLineDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COperationTimeLineDoc

IMPLEMENT_DYNCREATE(COperationTimeLineDoc, CDocument)

BEGIN_MESSAGE_MAP(COperationTimeLineDoc, CDocument)
END_MESSAGE_MAP()


// COperationTimeLineDoc �R���X�g���N�V����/�f�X�g���N�V����

COperationTimeLineDoc::COperationTimeLineDoc()
{
	m_strDefaultTitle.Empty();
}

COperationTimeLineDoc::~COperationTimeLineDoc()
{
}

BOOL COperationTimeLineDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B

	return TRUE;
}




// COperationTimeLineDoc �V���A����

void COperationTimeLineDoc::Serialize(CArchive& ar)
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
void COperationTimeLineDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void COperationTimeLineDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// �h�L�������g�̃f�[�^���猟���R���e���c��ݒ肵�܂��B
	// �R���e���c�̊e������ ";" �ŋ�؂�K�v������܂�

	// ��:      strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void COperationTimeLineDoc::SetSearchContent(const CString& value)
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

// COperationTimeLineDoc �f�f

#ifdef _DEBUG
void COperationTimeLineDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COperationTimeLineDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COperationTimeLineDoc �R�}���h

void COperationTimeLineDoc::SetTitle(LPCTSTR lpszTitle)
{
	CDocument::SetTitle(NULL);
}

void COperationTimeLineDoc::SetTitleEx(LPCTSTR lpszTitle)
{
	if (m_strDefaultTitle.IsEmpty() == true)
		m_strDefaultTitle = lpszTitle;
	CDocument::SetTitle(lpszTitle);
}

void COperationTimeLineDoc::ResetTitleEx()
{
	CDocument::SetTitle(m_strDefaultTitle);
}
