// GridCtrl.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "GridCtrl.h"


// CGridCtrl

IMPLEMENT_DYNAMIC(CGridCtrl, CWnd)

CGridCtrl::CGridCtrl()
{
	mParent = NULL;
	mInitOver = FALSE;
	mNum = CPoint(mDivHMax, mDivVMax);
	mSel = CPoint(0, 0);
	mCaptured = FALSE;
	mMessage = WM_GRID_MESSAGE;
	mHideInvalidSel = FALSE;
	mAlignment = 0;

	mRect.SetRectEmpty();
}

CGridCtrl::~CGridCtrl()
{
}


BEGIN_MESSAGE_MAP(CGridCtrl, CWnd)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	//ON_WM_LBUTTONDOWN()
	//ON_WM_LBUTTONUP()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CGridCtrl ���b�Z�[�W �n���h���[
/*============================================================================*/
/*! �����O���b�h�R���g���[��
�����O���b�h�E�B���h�E�̍쐬

@param		pParent		�e�E�B���h�E�n���h��
@param		rect		�̈�

@retval BOOL
*/
/*============================================================================*/
BOOL CGridCtrl::Create(CWnd *pParent, CRect rect, UINT nID)
{
	if (!RegisterClass())
		return FALSE;

	mParent = pParent;
	mSel = CPoint(0, 0);
	mCaptured = FALSE;
	mInitOver = FALSE;
	if (CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParent, nID, NULL)){
		mInitOver = TRUE;
		return TRUE;
	}
	return FALSE;
}

/*============================================================================*/
/*! �����O���b�h�R���g���[��
�����O���b�h�E�B���h�E�̍쐬

@param		nX		����������
@param		nY		����������

@retval CSize
*/
/*============================================================================*/
CSize CGridCtrl::CalcSize(int nX, int nY)
{
	CSize cx;
	cx.cx = nX + 5;
	cx.cy = nY + 5;
	return cx;
}

/*============================================================================*/
/*! �����O���b�h�R���g���[��
�E�B���h�E�N���X��o�^

@param
@param

@retval BOOL
*/
/*============================================================================*/
BOOL CGridCtrl::RegisterClass()
{
	return TRUE;
	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = AfxWndProc;
	wc.hInstance = AfxGetInstanceHandle();
	wc.hbrBackground = ::CreateSolidBrush(COLOR_WINDOW);
	wc.hCursor = ::LoadCursor(AfxGetInstanceHandle(), IDC_ARROW);
	wc.lpszClassName = _T("GRIDWINDOW");
	return AfxRegisterClass(&wc);
}

/*============================================================================*/
/*! �����O���b�h�R���g���[��
�E�B���h�����

@param

@retval void
*/
/*============================================================================*/
void CGridCtrl::CloseWindow()
{
	//PostMessage(WM_CLOSE);
}

/*============================================================================*/
/*! �����O���b�h�R���g���[��
�O���b�h�Z���̗L��/�����𒲂ׂ�

@param		point		�m�F����Z���ʒu

@retval BOOL
*/
/*============================================================================*/
BOOL CGridCtrl::IsEnableGrid(CPoint point)
{
	__int64 selno = (__int64)(((point.y - 1) * mNum.x) + (point.x - 1));
	__int64 selbit = ((__int64)1 << selno);
	return TRUE;
}


int CGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �����ɓ���ȍ쐬�R�[�h��ǉ����Ă��������B

	return 0;
}

void CGridCtrl::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_INACTIVE)
		CloseWindow();
}

void CGridCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!mRect.PtInRect(point)){
		// �E�B���h�E�O�ŃN���b�N���ꂽ
		CloseWindow();
	}
	else{
		// �E�B���h�E���ŃN���b�N���ꂽ
		SetCapture();
		mCaptured = TRUE;
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CGridCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// �E�B���h�E�����
	CloseWindow();
	if (mCaptured){
		// �O���b�h�Z�����I�����ꂽ�̂ŁA�e�E�B���h�E�ɒʒm
		ReleaseCapture();
		if (mSel.x > 0 && mSel.y > 0){
			if (IsEnableGrid(mSel) == TRUE){
				UINT count = GetMaxGrid();
				mParent->PostMessage(mMessage, mSel.x, mSel.y);
			}
		}
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CGridCtrl::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}

void CGridCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	CloseWindow();
}

void CGridCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect);
	mRect = rect;
	mRect.InflateRect(-2, -2);
}

BOOL CGridCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(&rect, ::GetSysColor(COLOR_WINDOW));
	return TRUE;
}

void CGridCtrl::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CDC memDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap = NULL;
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	pOldBitmap = memDC.SelectObject(&bitmap);

	CFont defFont, txtFont;
	CFont* oldFont = NULL;
	defFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (defFont.GetLogFont(&lf)){
		lf.lfWeight = FW_NORMAL;
		lf.lfHeight = 14;
		txtFont.CreateFontIndirect(&lf);
		oldFont = memDC.SelectObject(&txtFont);
	}

	COLORREF color = memDC.SetTextColor(mSelectColor);
	memDC.SetBkMode(TRANSPARENT);

	// �S�̘̂g��`��
	memDC.FillSolidRect(rect, ::GetSysColor(COLOR_WINDOW));
	rect.InflateRect(-1, -1);
	int x, y;

	y = mHeaderHeight;

	CRect textRect = rect;
	textRect.left += 5;
	textRect.top += 5;
	memDC.DrawText(mTitle, textRect, DT_SINGLELINE | DT_LEFT | DT_TOP);

	int sizeX, sizeY, h;
	sizeX = rect.Width() / mNum.x;
	sizeY = (rect.Height() - 20) / mNum.y;

	CString str;
	// �e�Z���̕`��
	for (int i = 0; i < mNum.y; i++, y += sizeY){
		x = 2;
		for (int j = 0; j < mNum.x; j++, x += sizeX){
			rect = CRect(x, y, x + sizeX, y + sizeY);
			rect.InflateRect(-1, -1);
			memDC.FillSolidRect(rect, ::GetSysColor(COLOR_BTNFACE));
			map<UINT, stStatusData>::iterator itr = mItemData.find((j << 16) | i);
			if (itr != mItemData.end()){
				memDC.FillSolidRect(rect, (*itr).second.color);
				CRect rc = rect;
				//h = memDC.DrawText((*itr).second.text, rc, DT_CENTER | DT_EDITCONTROL | DT_CALCRECT | DT_WORDBREAK);
				h = memDC.DrawText((*itr).second.text, rc, DT_CALCRECT);
				rc = rect;
				rc.top += h / 2;
				memDC.DrawText((*itr).second.text, rc, DT_CENTER);
			}
			//if (i < mSel.y && j < mSel.x){
			//	map<UINT, stStatusData>::iterator itr = mItemData.find((i<<16)|j);
			//	if (itr != mItemData.end()){
			//		memDC.DrawText((*itr).second.text, rect, DT_CENTER | DT_VCENTER);
			//	}
			//	memDC.FillSolidRect(rect, ::GetSysColor(COLOR_BTNHILIGHT));
			//	memDC.FillSolidRect(rect, ::GetSysColor(COLOR_BTNHILIGHT));
			//	memDC.Draw3dRect(&rect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNSHADOW));
			//}
		}
	}

	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SetTextColor(color);

	if (pOldBitmap != NULL)
		memDC.SelectObject(pOldBitmap);

	if (bitmap.m_hObject != NULL)
		bitmap.DeleteObject();

	defFont.DeleteObject();
	if (oldFont != NULL){
		memDC.SelectObject(oldFont);
		txtFont.DeleteObject();
	}
	memDC.DeleteDC();
}

void CGridCtrl::PostNcDestroy()
{
	CWnd::PostNcDestroy();
}

UINT CGridCtrl::GetMaxGrid()
{
	UINT count = 0;
	for (int i = 0; i < mNum.y; i++){
		for (int j = 0; j < mNum.x; j++){
			if (i < mSel.y && j < mSel.x){
				if (IsEnableGrid(CPoint(j + 1, i + 1)) == TRUE){
					if (mAlignment == 0){
						if (mNum.x >= ((i*mSel.x + j) + 1)){
							count++;
						}
					}
					else{
						if (mNum.y >= ((j*mSel.y + i) + 1)){
							count++;
						}
					}
				}
			}
		}
	}
	return count;
}

void CGridCtrl::AddItem(int x, int y, CString strItem, COLORREF color)
{
	UINT val = x << 16 | y;
	map<UINT, stStatusData>::iterator itr = mItemData.find(val);
	if (itr == mItemData.end()){
		stStatusData data;
		data.x = x;
		data.y = y;
		data.color = color;
		data.text = strItem;
		mItemData.insert(map<UINT, stStatusData>::value_type(val, data));
	}
	else{
		(*itr).second.x = x;
		(*itr).second.y = y;
		(*itr).second.color = color;
		(*itr).second.text = strItem;
	}
}


