/*============================================================================*/
/*! ForecastValueGroupView.cpp

-# �\��l�ꗗ��ʁ@�\��l���X�g�̃O���[�v�r���[
*/
/*============================================================================*/
// ForecastValueGroupView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "ForecastValueGroupView.h"
#include "ForecastValue.h"


// CForecastValueGroupView

IMPLEMENT_DYNCREATE(CForecastValueGroupView, CView)

CForecastValueGroupView::CForecastValueGroupView()
: m_colorText(RGB(0, 0, 0))
{
	m_txtFont.CreateStockObject(DEFAULT_GUI_FONT);
}

CForecastValueGroupView::~CForecastValueGroupView()
{
}

BEGIN_MESSAGE_MAP(CForecastValueGroupView, CView)
	ON_WM_PAINT()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CForecastValueGroupView �`��

void CForecastValueGroupView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
}


// CForecastValueGroupView �f�f

#ifdef _DEBUG
void CForecastValueGroupView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CForecastValueGroupView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForecastValueGroupView ���b�Z�[�W �n���h���[


void CForecastValueGroupView::OnPaint()
{
	CRect rectClient;
	CPaintDC dc(this); // device context for painting

	// �N���C�A���g�̈�̎擾
	GetClientRect(rectClient);

	CDC memDC;
	CBitmap bitm;
	// �`��DC�̍쐬
	memDC.CreateCompatibleDC(&dc);

	// �`��p�r�b�g�}�b�v�̍쐬
	bitm.CreateCompatibleBitmap(&dc, rectClient.Width(), rectClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitm);

	// �o�b�N�O���E���h ���[�h��ݒ�
	memDC.SetBkMode(OPAQUE);
	// �E�B���h�E�̔w�i�F���擾
	COLORREF colorBk = GetSysColor(COLOR_WINDOW);
	memDC.SetBkColor(colorBk);
	CBrush brush(colorBk);
	// �S�̂�h��Ԃ�
	memDC.FillRect(rectClient, &brush);
	// �f�o�C�X �R���e�L�X�g�̃r���[�|�[�g�̌��_��ݒ�
	memDC.SetViewportOrg(0, 0);

	// AOS, LOS, MAX EL�̃O���[�s���O�`��
	DrawGroup(memDC, rectClient);

	dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	bitm.DeleteObject();
	memDC.DeleteDC();
	brush.DeleteObject();
}

/*============================================================================*/
/*! �\��l�O���[�v�`��

-# �O���[�s���O�`��

@param	memDC		�h�L�������g
@param	rectClient	�`��̈�
@retval	TRUE:�O���b�h����`�悵�� / FALSE:�O���b�h����`�悵�Ȃ�����

*/
/*============================================================================*/
BOOL CForecastValueGroupView::DrawGroup(CDC& memDC, CRect& rectClient)
{
	CForecastValue* pWnd = (CForecastValue*)GetParent();
	if (pWnd == NULL)
	{
		return FALSE;
	}

	CFont* oldFont = memDC.SelectObject(&m_txtFont);
	memDC.SetTextColor(m_colorText);
	memDC.SetBkMode(TRANSPARENT);

	CForecastValue::structGroupXPos stXPos;
	CString str, strOut;
	CSize sz;

	// �e�̃��X�g�R���g���[������AOS�̕\���J�n�ʒu�ƏI���ʒu���擾
	pWnd->GetAosXPos(stXPos);
	str.Format(_T("AOS"));
	strOut = _T("");
	sz = memDC.GetTextExtent(str);

	while (sz.cx + m_nLeftOffset + m_nRightOffset < stXPos.x2 - stXPos.x1)
	{
		strOut = str;
		str = _T("-") + str + _T("-");
		sz = memDC.GetTextExtent(str);
	}

	memDC.TextOutW(rectClient.left + m_nLeftOffset + stXPos.x1, rectClient.top, strOut);

	// �e�̃��X�g�R���g���[������AOS�̕\���J�n�ʒu�ƏI���ʒu���擾
	pWnd->GetLosXPos(stXPos);
	str.Format(_T("LOS"));
	strOut = _T("");
	sz = memDC.GetTextExtent(str);

	while (sz.cx + m_nLeftOffset + m_nRightOffset < stXPos.x2 - stXPos.x1)
	{
		strOut = str;
		str = _T("-") + str + _T("-");
		sz = memDC.GetTextExtent(str);
	}

	memDC.TextOutW(rectClient.left + m_nLeftOffset + stXPos.x1, rectClient.top, strOut);

	// �e�̃��X�g�R���g���[������MAX EL�̕\���J�n�ʒu�ƏI���ʒu���擾
	pWnd->GetMaxElXPos(stXPos);
	str.Format(_T("MAX EL"));
	strOut = _T("");
	sz = memDC.GetTextExtent(str);

	while (sz.cx + m_nLeftOffset + m_nRightOffset < stXPos.x2 - stXPos.x1)
	{
		strOut = str;
		str = _T("-") + str + _T("-");
		sz = memDC.GetTextExtent(str);
	}

	memDC.TextOutW(rectClient.left + m_nLeftOffset + stXPos.x1, rectClient.top, strOut);

	memDC.SelectObject(oldFont);

	return TRUE;
}


int CForecastValueGroupView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	return 0;
//	return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
