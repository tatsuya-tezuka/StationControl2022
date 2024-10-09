
// OperationTimeLineDoc.cpp : COperationTimeLineDoc クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
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


// COperationTimeLineDoc コンストラクション/デストラクション

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

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。

	return TRUE;
}




// COperationTimeLineDoc シリアル化

void COperationTimeLineDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 格納するコードをここに追加してください。
	}
	else
	{
		// TODO: 読み込むコードをここに追加してください。
	}
}

#ifdef SHARED_HANDLERS

//縮小版のサポート
void COperationTimeLineDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// このコードを変更してドキュメントのデータを描画します
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

// 検索ハンドラーのサポート
void COperationTimeLineDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ドキュメントのデータから検索コンテンツを設定します。
	// コンテンツの各部分は ";" で区切る必要があります

	// 例:      strSearchContent = _T("point;rectangle;circle;ole object;");
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

// COperationTimeLineDoc 診断

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


// COperationTimeLineDoc コマンド

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
