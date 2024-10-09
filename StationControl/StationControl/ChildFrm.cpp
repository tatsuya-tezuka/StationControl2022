
// ChildFrm.cpp : CChildFrame クラスの実装
//

#include "stdafx.h"
#include "StationControl.h"

#include "ChildFrm.h"
#include "HookMsgBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CChildFrame コンストラクション/デストラクション

CChildFrame::CChildFrame()
{
	mSizeX = mSizeY = 0;
}

CChildFrame::~CChildFrame()
{
	if (mCaptionFont.GetSafeHandle())
		mCaptionFont.DeleteObject();
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs を変更して、Window クラスまたはスタイルを変更します。
	//cs.style &= ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame 診断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame メッセージ ハンドラー


int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	mStation = theApp.GetOpenViewType();

	bool bCreate = false;
	if (theApp.GetSelectStationMode() == eStation_5){
		if (mStation >= eViewType5_EqOperation1 && mStation <= eViewType5_EqOperation5){
			bCreate = true;
			mStation -= eViewType5_EqOperation1;
		}
	}

	if (bCreate == true){
		m_wndCaption.SetChildCaption(true);
		if (!m_wndCaption.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, 1500, -1))
		{
			TRACE0("Failed to create caption bar\n");
			return FALSE;
		}

		m_wndCaption.SetStationNo(mStation);
		m_wndCaption.SetFlatBorder();
		m_wndCaption.SetFont(&afxGlobalData.fontBold);
		CString str = mStationString[mStation];
		m_wndCaption.m_clrBarText = RGB(0, 0, 0);
		m_wndCaption.m_clrBarBackground = mStationColor[mStation];
		m_wndCaption.SetText(str, CMFCCaptionBar::ALIGN_CENTER);

		CFont defFont;
		defFont.CreateStockObject(DEFAULT_GUI_FONT);
		LOGFONT lf;
		if (defFont.GetLogFont(&lf)){
			lf.lfWeight = FW_BOLD;
			lf.lfHeight = 24;
			mCaptionFont.CreateFontIndirect(&lf);
		}
		m_wndCaption.SetFont(&mCaptionFont);
	}

	return 0;
}


void CChildFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	if (mSizeX != 0 || mSizeY != 0){
		lpMMI->ptMinTrackSize.x = mSizeX;
		lpMMI->ptMinTrackSize.y = mSizeY;
	}

	CMDIChildWndEx::OnGetMinMaxInfo(lpMMI);
}


void CChildFrame::OnClose()
{
	CString title;
	GetWindowText(title);

	if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("終了します。\nよろしいですか？"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return;

	CMDIChildWndEx::OnClose();
}
