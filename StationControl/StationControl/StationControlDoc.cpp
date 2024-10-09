
// StationControlDoc.cpp : CStationControlDoc �N���X�̎���
//

#include "stdafx.h"
// SHARED_HANDLERS �́A�v���r���[�A�k���ŁA����ь����t�B���^�[ �n���h���[���������Ă��� ATL �v���W�F�N�g�Œ�`�ł��A
// ���̃v���W�F�N�g�Ƃ̃h�L�������g �R�[�h�̋��L���\�ɂ��܂��B
#ifndef SHARED_HANDLERS
#include "StationControl.h"
#endif

#include "StationControlDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CStationControlDoc

IMPLEMENT_DYNCREATE(CStationControlDoc, CDocument)

BEGIN_MESSAGE_MAP(CStationControlDoc, CDocument)
END_MESSAGE_MAP()


// CStationControlDoc �R���X�g���N�V����/�f�X�g���N�V����

CStationControlDoc::CStationControlDoc()
{
	m_nViewType = 0;
}

CStationControlDoc::~CStationControlDoc()
{
}

BOOL CStationControlDoc::OnNewDocument()
{
	m_nViewType = theApp.GetOpenViewType();

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B

	return TRUE;
}




// CStationControlDoc �V���A����

void CStationControlDoc::Serialize(CArchive& ar)
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
void CStationControlDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CStationControlDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// �h�L�������g�̃f�[�^���猟���R���e���c��ݒ肵�܂��B
	// �R���e���c�̊e������ ";" �ŋ�؂�K�v������܂�

	// ��:      strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CStationControlDoc::SetSearchContent(const CString& value)
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

// CStationControlDoc �f�f

#ifdef _DEBUG
void CStationControlDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStationControlDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CStationControlDoc �R�}���h


void CStationControlDoc::SetTitle(LPCTSTR lpszTitle)
{
	CString str = CString(lpszTitle);
	if ((int)m_nViewType >= 0)
		str = mViewTitle[m_nViewType];
	m_strTitle = str;
	CDocument::SetTitle(str);
}

void CStationControlDoc::SetTitleEx(LPCTSTR lpszTitle)
{
	m_strTitle = lpszTitle;
	CDocument::SetTitle(lpszTitle);
}

void CStationControlDoc::SetTitleEx(UINT id)
{
	CString str;
	if (str.LoadString(id) == false){
		str = _T("***");
	}
	m_strTitle = str;
	CDocument::SetTitle(str);
}

void CStationControlDoc::SetTitleEx()
{
	CString str = mViewTitle[m_nViewType];
	m_strTitle = str;
	CDocument::SetTitle(str);
}
