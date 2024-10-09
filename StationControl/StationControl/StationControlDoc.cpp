
// StationControlDoc.cpp : CStationControlDoc クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
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


// CStationControlDoc コンストラクション/デストラクション

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

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。

	return TRUE;
}




// CStationControlDoc シリアル化

void CStationControlDoc::Serialize(CArchive& ar)
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
void CStationControlDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CStationControlDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ドキュメントのデータから検索コンテンツを設定します。
	// コンテンツの各部分は ";" で区切る必要があります

	// 例:      strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CStationControlDoc 診断

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


// CStationControlDoc コマンド


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
