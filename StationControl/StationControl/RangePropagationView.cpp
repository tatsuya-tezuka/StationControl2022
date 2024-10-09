/*============================================================================*/
/*! RangePropagationView.cpp

-# �����W�`���󋵃��j�^��ʁ@�`���O���t�r���[
*/
/*============================================================================*/
// RangePropagationView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "RangePropagationView.h"
#include "RangePropagationStatusMonitor.h"

//----------------------------------------------------------------------
// ��`

#define YOFFSET_RIGHT_ARROW		30
#define YOFFSET_LEFT_ARROW		56
#define XOFFSET_ARROW			20
#define YOFFSET_RIGHT_ARROW_STR	10
#define YOFFSET_LEFT_ARROW_STR	66

POINT   ptArrow[3]    = { 0, 0, 20, 7, 0, 14 };
POINT   ptArrowRvs[3] = { 20, 0, 0, 7, 20, 14 };

// CRangePropagationView

IMPLEMENT_DYNCREATE(CRangePropagationView, CView)

/*============================================================================*/
/*! CRangePropagationView

-# �R���X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CRangePropagationView::CRangePropagationView()
: m_colorStop(RGB(128, 128, 128))
, m_colorStart(RGB(0, 191, 255))
, m_colorText(RGB(0, 0, 0))
{
	m_txtFont.CreateStockObject(DEFAULT_GUI_FONT);
}

/*============================================================================*/
/*! CRangePropagationView

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CRangePropagationView::~CRangePropagationView()
{
}

BEGIN_MESSAGE_MAP(CRangePropagationView, CView)
	ON_WM_PAINT()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CRangePropagationView �`��

/*============================================================================*/
/*! CRangePropagationView

-# �`�掞�ɌĂ΂��

@param  pDC�F�h�L�������g�̃f�o�C�X�R���e�L�X�g�|�C���^
@retval �Ȃ�
*/
/*============================================================================*/
void CRangePropagationView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
}


// CRangePropagationView �f�f

#ifdef _DEBUG
void CRangePropagationView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CRangePropagationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRangePropagationView ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! CRangePropagationView

-# �O���b�h���̕`��

@param	memDC		�h�L�������g
@param	rectClient	�`��̈�
@retval	TRUE:�O���b�h����`�悵�� / FALSE:�O���b�h����`�悵�Ȃ�����

*/
/*============================================================================*/

BOOL CRangePropagationView::DrawArrow(CDC& memDC, CRect& rectClient)
{
	CPen penSolid;
	CPen penArrow;
	CBrush brush;
	CPen* oldPen;
	CBrush* oldBrush;
	COLORREF	color;
	POINT   points[3];

	CGraphMutex::Lock(eRngProp);

	// �`���f�[�^���擾
	CRangePropagation& rangePropagation = theApp.GetSatelliteData().GetRangePropagation();
	deque<CRangePropagation::structPropData>& dequeRngSnd = rangePropagation.GetRngSndData();
	deque<CRangePropagation::structPropData>& dequeRngRcv = rangePropagation.GetRngRcvData();
	UINT nStatus = rangePropagation.GetRngStatus();

	memDC.SetBkMode(TRANSPARENT);

	// ����
	if (nStatus == CRangePropagation::eStart && dequeRngSnd.empty() == false)
	{
		color = m_colorStart;
	}
	else
	{
		color = m_colorStop;
	}
	penSolid.CreatePen(PS_SOLID, 5, color);
	penArrow.CreatePen(PS_SOLID, 1, color);
	brush.CreateSolidBrush(color);
	oldPen = memDC.SelectObject(&penSolid);
	oldBrush = memDC.SelectObject(&brush);
	memDC.MoveTo(rectClient.left, rectClient.top + YOFFSET_RIGHT_ARROW);
	memDC.LineTo(rectClient.right - XOFFSET_ARROW, rectClient.top + YOFFSET_RIGHT_ARROW);
	memDC.SelectObject(&penArrow);
	for (int i = 0; i < 3; ++i)
	{
		points[i].x = ptArrow[i].x + rectClient.right - XOFFSET_ARROW;
		points[i].y = ptArrow[i].y + YOFFSET_RIGHT_ARROW - 7;
	}
	memDC.Polygon(points, 3);
	penSolid.DeleteObject();
	penArrow.DeleteObject();
	brush.DeleteObject();

	// �߂�
	if (nStatus == CRangePropagation::eStart && dequeRngRcv.empty() == false)
	{
		color = m_colorStart;
	}
	else
	{
		color = m_colorStop;
	}
	penSolid.CreatePen(PS_SOLID, 5, color);
	penArrow.CreatePen(PS_SOLID, 1, color);
	brush.CreateSolidBrush(color);
	memDC.SelectObject(&penSolid);
	memDC.SelectObject(&brush);
	memDC.MoveTo(rectClient.left + XOFFSET_ARROW, rectClient.top + YOFFSET_LEFT_ARROW);
	memDC.LineTo(rectClient.right, rectClient.top + YOFFSET_LEFT_ARROW);
	memDC.SelectObject(&penArrow);
	for (int i = 0; i < 3; ++i)
	{
		points[i].x = ptArrowRvs[i].x + rectClient.left;
		points[i].y = ptArrowRvs[i].y + YOFFSET_LEFT_ARROW - 7;
	}
	memDC.Polygon(points, 3);
	penSolid.DeleteObject();
	penArrow.DeleteObject();
	brush.DeleteObject();
	memDC.SelectObject(oldPen);
	memDC.SelectObject(oldBrush);

	CGraphMutex::Unlock(eRngProp);

	return TRUE;
}


/*============================================================================*/
/*! CRangePropagationView

-# �`���f�[�^�̕`��

@param	memDC		�h�L�������g
@param	rectClient	�`��̈�
@retval	TRUE:�O���b�h����`�悵�� / FALSE:�O���b�h����`�悵�Ȃ�����

*/
/*============================================================================*/
BOOL CRangePropagationView::DrawPropagation(CDC& memDC, CRect& rectClient)
{
	CFont* oldFont = memDC.SelectObject(&m_txtFont);
	memDC.SetTextColor(m_colorText);
	memDC.SetBkMode(TRANSPARENT);

	CGraphMutex::Lock(eRngProp);

	// �`���f�[�^���擾
	CRangePropagation& rangePropagation = theApp.GetSatelliteData().GetRangePropagation();
	deque<CRangePropagation::structPropData>& dequeRngSnd = rangePropagation.GetRngSndData();
	deque<CRangePropagation::structPropData>& dequeRngRcv = rangePropagation.GetRngRcvData();

	deque<CRangePropagation::structPropData>::iterator itr;

	int xpos;
	CString str;
	UINT nGeneMode = rangePropagation.GetGeneMode();
	UINT nRngNum = rangePropagation.GetRngNum();
	// �A���e�i->�q���Ԃ̉��ʒu
	for (itr = dequeRngSnd.begin(); itr != dequeRngSnd.end(); itr++)
	{
		// ���ۂ̋�������ʈʒu�ɕϊ�����
		xpos = (int)((rectClient.Width() - XOFFSET_ARROW * 3) * (*itr).pos / rangePropagation.GetRange());
		if (nGeneMode == eRngGeneration_Past)
		{
			str.Format(_T("%u>>"), (*itr).count);
		}
		else
		{
			str.Format(_T(">>"));
		}
		memDC.TextOutW(xpos + XOFFSET_ARROW, rectClient.top + YOFFSET_RIGHT_ARROW_STR, str);
	}

	// �A���e�i<-�q���Ԃ̉��ʒu
	for (itr = dequeRngRcv.begin(); itr != dequeRngRcv.end(); itr++)
	{
		// ���ۂ̋�������ʈʒu�ɕϊ�����
		xpos = (int)((rectClient.Width() - XOFFSET_ARROW * 3) * (*itr).pos / rangePropagation.GetRange());
		if (nGeneMode == eRngGeneration_Past)
		{
			str.Format(_T("<<%u"), (*itr).count);
		}
		else
		{
			str.Format(_T("<<"));
		}
		memDC.TextOutW(xpos + XOFFSET_ARROW, rectClient.top + YOFFSET_LEFT_ARROW_STR, str);
	}

	CGraphMutex::Unlock(eRngProp);

	memDC.SelectObject(oldFont);

	return TRUE;
}


/*============================================================================*/
/*! CRangePropagationView

-# �`�掞�ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRangePropagationView::OnPaint()
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
	COLORREF colorBk = GetSysColor(COLOR_3DFACE);
	memDC.SetBkColor(colorBk);
	CBrush brush(colorBk);
	// �S�̂�h��Ԃ�
	memDC.FillRect(rectClient, &brush);
	// �f�o�C�X �R���e�L�X�g�̃r���[�|�[�g�̌��_��ݒ�
	memDC.SetViewportOrg(0, 0);

	// ���̕`��
	DrawArrow(memDC, rectClient);

	// �`���f�[�^�̕`��
	DrawPropagation(memDC, rectClient);

	dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	bitm.DeleteObject();
	memDC.DeleteDC();
	brush.DeleteObject();
}


/*============================================================================*/
/*! CRangePropagationView

-# ��A�N�e�B�u �E�B���h�E�Ƀ��[�U�[���}�E�X �{�^�����������Ƃ��ɌĂяo�����

@param  pDesktopWnd�F�A�N�e�B�u�ɂȂ�E�B���h�E�̃g�b�v���x���̐e�E�B���h�E�ւ̃|�C���^
@param	nHitTest�F�J�[�\���ʒu�e�X�g�̗̈�R�[�h
@param	message�F�}�E�X���b�Z�[�W�ԍ�
@retval �E�B���h�E�A�N�e�B�u/�j������
*/
/*============================================================================*/
int CRangePropagationView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	return 0;
//	return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

#if 0
/*============================================================================*/
/*! CRangePropagationView

-# �w�萔�l���Q�Ɛ��l�Ɠ������܂�0�Ŗ��߂ĕ���������(4���܂őΉ��j

@param  �w�萔�l
@param  �Q�Ɛ��l
@param  ������
@retval �Ȃ�
*/
/*============================================================================*/
void CRangePropagationView::AddZeroString(UINT val, UINT refVal, CString& str)
{
	if (refVal >= 1000)
	{
		str.Format(_T("%04u>>"), val);
	}
	else if (refVal >= 100)
	{
		str.Format(_T("%03u>>"), val);
	}
	else if (refVal >= 10)
	{
		str.Format(_T("%02u>>"), val);
	}
	else
	{
		str.Format(_T("%01u>>"), val);
	}
}
#endif
