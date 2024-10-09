// この MFC サンプル ソース コードでは、MFC Microsoft Office Fluent ユーザー インターフェイス 
// ("Fluent UI") の使用方法を示します。このコードは、MFC C++ ライブラリ ソフトウェアに 
// 同梱されている Microsoft Foundation Class リファレンスおよび関連電子ドキュメントを
// 補完するための参考資料として提供されます。
// Fluent UI を複製、使用、または配布するためのライセンス条項は個別に用意されています。
// Fluent UI ライセンス プログラムの詳細については、Web サイト
// http://go.microsoft.com/fwlink/?LinkId=238214 を参照してください。
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// RemoteGraphDoc.cpp : CRemoteGraphDoc クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
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
// ウィンドウ名称
const CString wndName[MAX_VIEW] = {
	_T("グラフ領域1"),
	_T("グラフ領域2"),
	_T("グラフ領域3"),
	_T("グラフ領域4"),
	_T("グラフ領域5"),
	_T("グラフ領域6"),
	_T("グラフ領域7"),
	_T("グラフ領域8")
};
#endif


// CRemoteGraphDoc

IMPLEMENT_DYNCREATE(CRemoteGraphDoc, CDocument)

BEGIN_MESSAGE_MAP(CRemoteGraphDoc, CDocument)
END_MESSAGE_MAP()


// CRemoteGraphDoc コンストラクション/デストラクション

CRemoteGraphDoc::CRemoteGraphDoc()
{
	// TODO: この位置に 1 度だけ呼ばれる構築用のコードを追加してください。

}

CRemoteGraphDoc::~CRemoteGraphDoc()
{
}

BOOL CRemoteGraphDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。


	return TRUE;
}




// CRemoteGraphDoc シリアル化

void CRemoteGraphDoc::Serialize(CArchive& ar)
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
void CRemoteGraphDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CRemoteGraphDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ドキュメントのデータから検索コンテンツを設定します。
	// コンテンツの各部分は ";" で区切る必要があります

	// 例:      strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CRemoteGraphDoc 診断

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
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	//	CDocument::SetTitle(lpszTitle);

	// ウィンドウ番号を取得する
	UINT nID = ((CMainFrame*)(theApp.m_pMainWnd))->GetCreateVewIdx();

	// ウィンドウ番号が8以上ならばウィンドウは開けないので戻る
	if (nID >= MAX_VIEW) {
		return;
	}
#if 0
//	// ウィンドウ番号に対応するパラメータ配列をアプリから取得
//	CPropertiesData* pParam = &theApp.GetPropertiesData(nID);
//	CDocument::SetTitle(pParam->wndName);

	// ウィンドウ番号に対応するグラフパラメータリストの取得
	CPropertiesData* pParam;
	// ダミーパラメータを取得
	pParam = new CPropertiesData();
	pParam->wndName = sampleParam[nID][0].wndName;

	CDocument::SetTitle(pParam->wndName);
	delete pParam;
#else
	// ウィンドウ番号に対応するグラフパラメータリストの取得
	//CPropertiesData* pParam;
	// ダミーパラメータを取得
	//pParam = new CPropertiesData();
	//pParam->wndName = wndName[nID];
	//CDocument::SetTitle(pParam->wndName);
	//delete pParam;
	CDocument::SetTitle(wndName[nID]);
#endif
}


// CRemoteGraphDoc コマンド


void CRemoteGraphDoc::OnCloseDocument()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	CDocument::OnCloseDocument();
}
