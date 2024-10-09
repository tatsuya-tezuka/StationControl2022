/*============================================================================*/
/*! ForecastRadarGraphView.cpp

-# �\��l�ꗗ��ʁ@���[�_�[�O���t
*/
/*============================================================================*/
// ForecastRadarGraphView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "ForecastRadarGraphView.h"


//----------------------------------------------------------------------
// ��`

// ����
#define  INTERVAL_NEW	0   						// ����(�āj�\���J�n���ԁi�~���b)
//#define  INTERVAL_REAL	6000						// �g�����h �\���J�n���ԁi�~���b�j
#define  INTERVAL_REAL	500							// �g�����h �\���J�n���ԁi�~���b�j

// �\���F
#define  GRPH_CLR    	RGB(236, 233, 216)  		// �O���t�̈�w�i�F
#define  FREM_CLR    	RGB(  0,   0,   0)   		// �O���t�g�\���F
#define  FREM_CLR2		RGB(255, 255, 255)			// �O���t�g�\���F2
#define  E80_CLR    	RGB(255, 215,   0)   		// �O���t�g�\���F
#define  GRID_CLR    	RGB(  0,   0,   0)			// �O���t�r���\���F	
#define  GRID_CLR2    	RGB(128, 128, 128)			// �O���t�r���\���F2	
#define  STRTIT_CLR  	RGB(  0,   0,   0)			// �^�C�g�������\���F
#define  STRCON_CLR  	RGB(  0,   0,   0)			// �Œ荀�ڕ����\���F
#define  STRVAR_CLR  	RGB(  0,   0,   0)			// �ύ��ڕ����\���F
#define  YOHO_AOS_CLR	RGB(  0,  11, 255)			// �O���t�`��F(�\��lAOS)
#define  YOHO_LOS_CLR	RGB(  0,   0, 255)			// �O���t�`��F(�\��lLOS)
#define  YOHO_CLR		RGB(255, 165,   0)			// �O���t�`��F(�\��l)
#define  TRND_CLR		RGB(  0, 128,   0)			// �O���t�`��F(�����l)	
#define  SKYL_CLR		RGB(144, 238, 144)			// �O���t�`��F(�X�J�C���C��)
//#define  RFTR_CLR		RGB(255, 192, 203)			// �O���t�`��F(�q�e�g���b�v)
#define  RFTR_CLR		RGB(255,   0,   0)			// �O���t�`��F(�q�e�g���b�v)
#define  OVER_CLR		RGB(255,   0,   0)			// �O���t�`��F(�͈̓I�[�o�[)

// ���̑���
#define  LINE_WID_FRAM_BOLD   2						// �O���t�g�̐��̑���(�����j
#define  LINE_WID_FRAM_BOLD2  3						// �O���t�g�̐��̑���(����2�j
#define  LINE_WID_FRAM   1							// �O���t�g�̐��̑���(�א��j
#define  LINE_WID_GRID   1							// �O���b�h�r���̑���

// ���W�A��
#define	RADCV	(3.1415926538/180.0)

// �����}�[�W��
#define TEXT_MARGIN				16		// �e�L�X�g�}�[�W��
#define ANGLE_MARGIN			28		// �p�x�}�[�W��
#define ANGLE_MARGIN_4STATION	14		// �p�x�}�[�W��

// �}��
#define EXAMPLE_AREA_WIDTH		50		// �}��e�L�X�g�G���A���i�P�ǃ��[�h�j
#define EXAMPLE_STATIONALL_AREA_WIDTH		150		// �}��e�L�X�g�G���A���i�S�ǃ��[�h�j
#define EXAMPLE_LINE_WIDTH		8		// �}�჉�C����
#define EXAMPLE_RIGHT_LINE_POS	50		// �E���C���ʒu
#define EXAMPLE_AREA_WIDTH2		100			// �}��e�L�X�g�G���A��2�i�P�ǃ��[�h�j
#define EXAMPLE_AREA_WIDTH3		70			// �}��e�L�X�g�G���A��3�i�v��ꗗ��ʁj
#define EXAMPLE_AREA_WIDTH4		100			// �}��e�L�X�g�G���A��4�i�\��l�ꗗ��ʁj

#define EXAMPLE_STATIONALL_LEFT_MARGIN		10		// �}��e�L�X�g�G���A���}�[�W���i�S�ǃ��[�h�j
#define EXAMPLE_STATIONALL_TOP_MARGIN		5		// �}��e�L�X�g�G���A��}�[�W���i�S�ǃ��[�h�j
#define RECT_STATIONALL_HEIGHT				50		// �̈�G���A��ʒu�i�S�ǃ��[�h�j
#define RECT_STATION_HEIGHT					40		// �̈�G���A��ʒu�i�P�ǃ��[�h�j
#define RECT_STATION_HEIGHT2				20		// �̈�G���A��ʒu�i�ꗗ��ʁj

// �p�x���� �\���ŏ��T�C�Y�ݒ�
#define VALUES_AREA_WIDTH					200		// �l�̕\���G���A�̉��T�C�Y
#define VALUES_AREA_HEGIHT					120		// �l�̕\���G���A�̏c�T�C�Y
#define ANGLEVIEW_MINIMUM_SZ				0		// �����菬�����ꍇ�́A�x���������\���Ƃ���
#define EXAMPLEVIEW_MINIMUM_SZ				180		// �����菬�����ꍇ�́A�}����\���Ƃ���
#define DIRVIEW_MINIMUM_SZ					140		// �����菬�����ꍇ�́A���ʂ��\���Ƃ���

// �l�̕\�̈ʒu�ݒ�
#define VALUES_AREA_HMARGIN					4		// �l�̕\�̉������}�[�W��
#define VALUES_AREA_VMARGIN					4		// �l�̕\�̏c�����}�[�W��
#define VALUES_AREA_ROW_H					20		// �l�̕\�̍s�̍����}�[�W��
#define VALUES_AREA_FORECAST_W				0.2		// �l�̕\�̗\��l�̕�
#define VALUES_AREA_TIME_W					0.4		// �l�̕\�̗\��l�̕�
#define VALUES_AREA_AZ_W					0.2		// �l�̕\�̗\��l�̕�
#define VALUES_AREA_EL_W					0.2		// �l�̕\�̗\��l�̕�

// �f�[�^��`
#define IGNORE_DATA		9999999999999.0		/* �����f�[�^�i�N���A�p�j */


// CForecastRadarGraphView

IMPLEMENT_DYNCREATE(CForecastRadarGraphView, CView)

/*============================================================================*/
/*! CForecastRadarGraphView

-# �R���X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CForecastRadarGraphView::CForecastRadarGraphView() 
: m_i_plot_flag(CTrackingRadarCtrlMonData::eOFF)
, m_nViewType(eRadarViewType_Station)
, m_nStationID(eStation_Usuda64)
, m_strPlanID(_T(""))
, m_tPassStart(0)
, m_tPassEnd(0)
, m_dummyCnt(0)
{
	CFont defFont;
	CFont* oldFont = NULL;
	defFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (defFont.GetLogFont(&lf)){
		lf.lfWeight = FW_NORMAL;
		lf.lfHeight = 12;
		m_txtFontNormal.CreateFontIndirect(&lf);

		lf.lfHeight = -8;
		m_txtFontSmall.CreateFontIndirect(&lf);
	}
}

/*============================================================================*/
/*! CForecastRadarGraphView

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CForecastRadarGraphView::~CForecastRadarGraphView()
{
	m_txtFontSmall.DeleteObject();
	m_txtFontNormal.DeleteObject();
}

BEGIN_MESSAGE_MAP(CForecastRadarGraphView, CView)
	ON_WM_MOUSEACTIVATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CForecastRadarGraphView �`��

/*============================================================================*/
/*! CForecastRadarGraphView

-# �`�掞�ɌĂ΂��

@param  pDC�F�h�L�������g�̃f�o�C�X�R���e�L�X�g�|�C���^
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastRadarGraphView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
}


// CForecastRadarGraphView �f�f

#ifdef _DEBUG
void CForecastRadarGraphView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CForecastRadarGraphView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForecastRadarGraphView ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CForecastRadarGraphView

-# ��A�N�e�B�u �E�B���h�E�Ƀ��[�U�[���}�E�X �{�^�����������Ƃ��ɌĂяo�����

@param  pDesktopWnd�F�A�N�e�B�u�ɂȂ�E�B���h�E�̃g�b�v���x���̐e�E�B���h�E�ւ̃|�C���^
@param	nHitTest�F�J�[�\���ʒu�e�X�g�̗̈�R�[�h
@param	message�F�}�E�X���b�Z�[�W�ԍ�
@retval �E�B���h�E�A�N�e�B�u/�j������
*/
/*============================================================================*/
int CForecastRadarGraphView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	return 0;
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# �`�掞�ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastRadarGraphView::OnPaint()
{
	CGraphMutex::Lock(eRadar);

	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		CGraphMutex::Unlock(eRadar);
		return;
	}

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
	COLORREF colBk = GetSysColor(COLOR_3DFACE);
	memDC.SetBkColor(colBk);

	CFont* oldFont = NULL;
	if (m_nViewType == eRadarViewType_Station && theApp.GetSelectStationMode() != eStation_1)
	{
		oldFont = memDC.SelectObject(&m_txtFontSmall);
	}
	else
	{
		oldFont = memDC.SelectObject(&m_txtFontNormal);
	}

	CBrush brush(colBk);
	// �S�̂�h��Ԃ�
	memDC.FillRect(rectClient, &brush);
	// �f�o�C�X �R���e�L�X�g�̃r���[�|�[�g�̌��_��ݒ�
	memDC.SetViewportOrg(0, 0);

	// ���[�_�[�g�`��
	DrawRadarFrame(memDC, rectClient, eDrawType_Before);
	// �X�J�C���C���`��
	DrawSkyline(memDC, rectClient);

	// RF�g���b�v�̓T�}����ʂ̂ݕ\��
	if (m_nViewType == eRadarViewType_Station)
	{
		// RF�g���b�v�`��
		DrawRfTrip(memDC, rectClient);
	}
	// �\��l�`��
	DrawForecast(memDC, rectClient);

	// �ǔ��󋵃O���t�ۑ��f�[�^�`�揈��(�\��l�ꗗ�͕`�悵�Ȃ��j
	if (m_nViewType != eRadarViewType_ForecastList &&
		pCtrlMonData->m_gst_dspinf.i_unyou_mode == CTrackingRadarCtrlMonData::eUNYOU_IN) {
		DrawTrakingSaveData(memDC, rectClient);
	}

	// ���[�_�[�g�`��
	DrawRadarFrame(memDC, rectClient, eDrawType_After);

	CGraphMutex::Unlock(eRadar);

	dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	bitm.DeleteObject();
	memDC.SelectObject(oldFont);

	memDC.DeleteDC();

	memDC.DeleteDC();
	brush.DeleteObject();
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# ���[�_�[�t���[���`��

@param	memDC		�h�L�������g
@param	rectClient	�`��̈�
@retval	TRUE:�O���b�h����`�悵�� / FALSE:�O���b�h����`�悵�Ȃ�����

*/
/*============================================================================*/
BOOL CForecastRadarGraphView::DrawRadarFrame(CDC& memDC, CRect& rectClient, int drawType)
{
	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		return FALSE;
	}

	CPen penSolidBold;
	if (!penSolidBold.CreatePen(PS_SOLID, LINE_WID_FRAM_BOLD, pCtrlMonData->m_gst_dspinf.frem_color))
		return FALSE;
	CPen penSolidBold2;
	if (!penSolidBold2.CreatePen(PS_SOLID, LINE_WID_FRAM_BOLD2, pCtrlMonData->m_gst_dspinf.frem_color))
		return FALSE;
	CPen penSolid;
	if (!penSolid.CreatePen(PS_SOLID, LINE_WID_FRAM_BOLD, pCtrlMonData->m_gst_dspinf.frem_color2))
		return FALSE;
	CPen* oldPen = memDC.SelectObject(&penSolid);
	CBrush brushBk;
	brushBk.CreateSolidBrush(pCtrlMonData->m_gst_dspinf.grph_color);
	CBrush brush80;
	brush80.CreateSolidBrush(pCtrlMonData->m_gst_dspinf.e80_frem_color);
	CBrush* oldBrush = (CBrush*)memDC.SelectObject(&brushBk);

	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(pCtrlMonData->m_gst_dspinf.strcon_color);

	CRect rcCircle = rectClient;
	CRect rcValues;
	BOOL bViewValues = FALSE;
	BOOL bViewExample = FALSE;
	BOOL bViewStationModeAll = FALSE;

	// 1�ǃT�}���̏ꍇ
//	if (theApp.GetSelectStationMode() == eStation_1 && rcCircle.Width() >= VALUES_AREA_WIDTH && rcCircle.Height() >= EXAMPLEVIEW_MINIMUM_SZ + VALUES_AREA_HEGIHT)
	if (theApp.GetSelectStationMode() == eStation_1 && m_nViewType == eRadarViewType_Station)
	{
		bViewValues = TRUE;
		rcCircle.bottom -= VALUES_AREA_HEGIHT;
		rcValues = rectClient;
		rcValues.top = rcCircle.bottom;
	}

	// �ǃT�}���̏ꍇ
	if (m_nViewType == eRadarViewType_Station && theApp.GetSelectStationMode() != eStation_1)
	{
		rcCircle.bottom -= RECT_STATIONALL_HEIGHT;
		bViewStationModeAll = TRUE;
	}

	if (bViewStationModeAll || rcCircle.Width() >= EXAMPLEVIEW_MINIMUM_SZ && rcCircle.Height() >= EXAMPLEVIEW_MINIMUM_SZ)
	{
		bViewExample = TRUE;
		// 1�ǃ��[�h�܂��͈ꗗ���
		if (!bViewStationModeAll)
		{
			if (m_nViewType >= eRadarViewType_ForecastList)
			{
				// �c��������߂�
				rcCircle.bottom -= RECT_STATION_HEIGHT2;
			}
			else
			{
				// �c��������߂�
				rcCircle.bottom -= RECT_STATION_HEIGHT;
			}
		}
		// �����`�ɂ���
		if (rcCircle.Width() > rcCircle.Height())
		{
			// �����c�����傫���ꍇ�́A�����c�ɂ��킹��
			int sz = (rcCircle.Width() - rcCircle.Height()) / 2;
			rcCircle.left += sz;
			rcCircle.right -= sz;
		}
		else
		{
			// �c���������傫���ꍇ�́A�c�����ɂ��킹��
			int sz = (rcCircle.Height() - rcCircle.Width()) / 2;
			rcCircle.top += sz;
			rcCircle.bottom -= sz;
		}
	}

	CRect rc = rcCircle;

	// �~�O���b�h�̕`��
	int len = __min(rc.Width(), rc.Height());
	rc.right = rc.left + len;
	rc.bottom = rc.top + len;
	rc.left = rc.left + ((rcCircle.Width() - rc.Width()) / 2);
	rc.top = rc.top + ((rcCircle.Height() - rc.Height()) / 2);
	rc.right = rc.left + len;
	rc.bottom = rc.top + len;

	rc.DeflateRect(1, 1);
	CRect textrect = rc;

	if (rcCircle.Width() >= ANGLEVIEW_MINIMUM_SZ && rcCircle.Height() >= ANGLEVIEW_MINIMUM_SZ)
	{
		if (!bViewStationModeAll)
		{
			rc.DeflateRect(ANGLE_MARGIN, ANGLE_MARGIN);
		}
		else
		{
			rc.DeflateRect(ANGLE_MARGIN_4STATION, ANGLE_MARGIN_4STATION);
		}
	}

	if (drawType == eDrawType_Before)
	{
		memDC.Ellipse(rc);																									// 0�x�ʒu�h��Ԃ�
	}
	else
	{
		memDC.SelectObject(&penSolidBold);
		(CBrush*)memDC.SelectStockObject(NULL_BRUSH);
		memDC.Ellipse(rc);																									// 0�x�ʒu
	}
	
	memDC.SelectObject(&penSolid);

	if (drawType == eDrawType_After)
	{
		if (rcCircle.Width() >= ANGLEVIEW_MINIMUM_SZ && rcCircle.Height() >= ANGLEVIEW_MINIMUM_SZ)
		{
			memDC.TextOutW(rc.right, rc.top + (rc.bottom - rc.top) / 2 + 5, _T("0��"));
		}
	}

	CRect linerect = rc;
	rc.InflateRect(-((int)(linerect.Width() / 2.0 * (1.0 / 3.0))), -((int)(linerect.Height() / 2.0 * (1.0 / 3.0))));
	if (drawType == eDrawType_Before)
	{
		memDC.Ellipse(rc);																									// 30�x�ʒu
	}
	
	if (drawType == eDrawType_After)
	{
		if (rcCircle.Width() >= ANGLEVIEW_MINIMUM_SZ && rcCircle.Height() >= ANGLEVIEW_MINIMUM_SZ)
		{
			memDC.TextOutW(rc.right, rc.top + (rc.bottom - rc.top) / 2 + 5, _T("30��"));
		}
	}

	rc.InflateRect(-((int)(linerect.Width() / 2.0 * (1.0 / 3.0))), -((int)(linerect.Height() / 2.0 * (1.0 / 3.0))));
	if (drawType == eDrawType_Before)
	{
		memDC.Ellipse(rc);																									// 60�x�ʒu
	}
	
	if (drawType == eDrawType_After)
	{
		if (rcCircle.Width() >= ANGLEVIEW_MINIMUM_SZ && rcCircle.Height() >= ANGLEVIEW_MINIMUM_SZ)
		{
			memDC.TextOutW(rc.right, rc.top + (rc.bottom - rc.top) / 2 + 5, _T("60��"));
		}
	}

	rc = linerect;
	rc.InflateRect((int)(linerect.Width() / 2.0 / 9.0 - linerect.Width() / 2.0), (int)(linerect.Height() / 2.0 / 9.0 - linerect.Height() / 2.0));

	if (drawType == eDrawType_Before)
	{
		(CBrush*)memDC.SelectObject(&brush80);
		memDC.Ellipse(rc);																									// 80�x�ʒu																								;			
		(CBrush*)memDC.SelectStockObject(NULL_BRUSH);
		memDC.Ellipse(rc);																								;			
	}

	if (drawType == eDrawType_After)
	{
		if (rcCircle.Width() >= ANGLEVIEW_MINIMUM_SZ && rcCircle.Height() >= ANGLEVIEW_MINIMUM_SZ)
		{
			if (!bViewStationModeAll)
			{
				memDC.TextOutW(rc.right, rc.top + (rc.bottom - rc.top) / 2 + 5, _T("80��"));
			}
			else
			{
				memDC.TextOutW(rc.right - 5, rc.top + (rc.bottom - rc.top) / 2 + 5, _T("80��"));
			}
		}
	}

	CPen penLine;
	if (!penLine.CreatePen(PS_SOLID, LINE_WID_GRID, pCtrlMonData->m_gst_dspinf.grid_color))
		return FALSE;
	CPen penLine2;
	if (!penLine2.CreatePen(PS_SOLID, LINE_WID_GRID, pCtrlMonData->m_gst_dspinf.grid_color2))
		return FALSE;
	rc = linerect;
	double angle;
	double step = 30.0;
	double rad;
	double r;
	CPoint pt1, pt2, ptCenter;

	r = (rc.right - rc.left) / 2;
	ptCenter.x = rc.left + (rc.right - rc.left) / 2;
	ptCenter.y = rc.top + (rc.bottom - rc.top) / 2;
	m_r = r;
	m_ptCenter = ptCenter;

	if (drawType == eDrawType_After)
	{
		// �p�x�O���b�h�`��
		double r2 = r * 0.9;
		for (angle = 0.0; angle < 360.0; angle += step)
		{
			if (angle == 0.0 || angle == 90.0)
			{
				memDC.SelectObject(&penLine2);
				rad = 3.1415926538 * angle / 180.0;
				pt1.x = (int)(sin(rad) * r) + ptCenter.x;
				pt1.y = -(int)(cos(rad) * r) + ptCenter.y;
				memDC.MoveTo(pt1.x, pt1.y);
				rad = 3.1415926538 * (angle + 180.0) / 180.0;
				pt2.x = (int)(sin(rad) * r) + ptCenter.x;
				pt2.y = -(int)(cos(rad) * r) + ptCenter.y;
				memDC.LineTo(pt2.x, pt2.y);

				memDC.SelectObject(&penSolidBold2);
				rad = 3.1415926538 * angle / 180.0;
				pt1.x = (int)(sin(rad) * r / 18.0) + ptCenter.x;
				pt1.y = -(int)(cos(rad) * r / 18.0) + ptCenter.y;
				memDC.MoveTo(pt1.x, pt1.y);
				rad = 3.1415926538 * (angle + 180.0) / 180.0;
				pt2.x = (int)(sin(rad) * r / 18.0) + ptCenter.x;
				pt2.y = -(int)(cos(rad) * r / 18.0) + ptCenter.y;
				memDC.LineTo(pt2.x, pt2.y);
			}

			{
				memDC.SelectObject(&penLine);
				rad = 3.1415926538 * angle / 180.0;
				pt1.x = (int)(sin(rad) * r) + ptCenter.x;
				pt1.y = -(int)(cos(rad) * r) + ptCenter.y;
				memDC.MoveTo(pt1.x, pt1.y);
				pt2.x = (int)(sin(rad) * r2) + ptCenter.x;
				pt2.y = -(int)(cos(rad) * r2) + ptCenter.y;
				memDC.LineTo(pt2.x, pt2.y);
			}
		}

		if (rcCircle.Width() >= ANGLEVIEW_MINIMUM_SZ && rcCircle.Height() >= ANGLEVIEW_MINIMUM_SZ)
		{
			// �p�x���l�`��
			rc = textrect;
			if (!bViewStationModeAll)
				rc.InflateRect(-12, -12);
			else
				rc.InflateRect(-6, -6);

			r = (rc.right - rc.left) / 2;
			ptCenter.x = rc.left + (rc.right - rc.left) / 2;
			ptCenter.y = rc.top + (rc.bottom - rc.top) / 2;

			for (angle = 0.0; angle < 360.0; angle += step)
			{
				CString str;
				str.Format(_T("%.0f��"), angle);
				CSize szStr = memDC.GetTextExtent(str);
				rad = 3.1415926538 * angle / 180.0;
				pt1.x = (int)(sin(rad) * r) + ptCenter.x + 5;
				pt1.y = -(int)(cos(rad) * r) + ptCenter.y;
				memDC.TextOutW(pt1.x - szStr.cx / 2, pt1.y - szStr.cy / 2, str);
			}
		}
	}
	
	if (drawType == eDrawType_After)
	{
		if (bViewExample)
		{
			memDC.SelectObject(&m_txtFontNormal);

			// 1�ǃT�}�����
			if (bViewValues)
			{
				// �}��`��
				CString strMsg;
				textrect.left += 10;
				textrect.right = textrect.left + EXAMPLE_AREA_WIDTH;
				textrect.top = textrect.bottom + 10;
				textrect.bottom = textrect.top + TEXT_MARGIN;
				rc = textrect;
				CRect exampleLineRc = rc;
				rc.OffsetRect(EXAMPLE_LINE_WIDTH + 2, 0);

				// SKYLINE
				CPen penExampleSkyline;
				if (!penExampleSkyline.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.skyl_color))
					return FALSE;
				memDC.SelectObject(&penExampleSkyline);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				if (strMsg.LoadString(IDS_RADARGRAPH_SKYLINE) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				exampleLineRc.OffsetRect(EXAMPLE_AREA_WIDTH2, 0);

				// RF�g���b�v�̓T�}����ʂ̂ݕ\��
				CPen penExampleRftrip;
				if (m_nViewType == eRadarViewType_Station)
				{
					// RF-TRIP
					if (!penExampleRftrip.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.rftr_color))
						return FALSE;
					memDC.SelectObject(&penExampleRftrip);
					memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
					memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
					rc.OffsetRect(EXAMPLE_AREA_WIDTH2, 0);
					if (strMsg.LoadString(IDS_RADARGRAPH_RFTRIP) == FALSE)
						return FALSE;
					memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
					exampleLineRc.OffsetRect(EXAMPLE_AREA_WIDTH2, 0);
				}

				// �\��l
				CPen penExampleForecastValue;
				if (!penExampleForecastValue.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.yoho_color))
					return FALSE;
				memDC.SelectObject(&penExampleForecastValue);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				rc.OffsetRect(EXAMPLE_AREA_WIDTH2, 0);
				if (strMsg.LoadString(IDS_FORECASTLIST_FORECASTVALUE) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				rc = textrect;
				exampleLineRc = rc;
				exampleLineRc.OffsetRect(0, 14);

				// AOS
				CPen penExampleAosValue;
				if (!penExampleAosValue.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.aos_yoho_color))
					return FALSE;
				memDC.SelectObject(&penExampleAosValue);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				rc.OffsetRect(EXAMPLE_LINE_WIDTH + 2, 14);
				if (strMsg.LoadString(IDS_RADARGRAPH_AOS) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				exampleLineRc.OffsetRect(EXAMPLE_AREA_WIDTH2, 0);

				// �����l
				CPen penExampleActualValue;
				if (!penExampleActualValue.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.trnd_color))
					return FALSE;
				memDC.SelectObject(&penExampleActualValue);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				rc.OffsetRect(EXAMPLE_AREA_WIDTH2, 0);
				if (strMsg.LoadString(IDS_RADARGRAPH_ACTUAL_VALUE) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

				penExampleSkyline.DeleteObject();
				// RF�g���b�v�̓T�}����ʂ̂ݕ\��
				if (m_nViewType == eRadarViewType_Station)
				{
					penExampleRftrip.DeleteObject();
				}
				penExampleForecastValue.DeleteObject();
				penExampleAosValue.DeleteObject();
				penExampleActualValue.DeleteObject();
			}
			// 4�ǃT�}�����
			else if (bViewStationModeAll)
			{
				// �}��`��
				CString strMsg;
				rc = textrect;
				rc.left = rc.left + EXAMPLE_STATIONALL_LEFT_MARGIN;
				rc.right = rc.left + EXAMPLE_STATIONALL_AREA_WIDTH;
				rc.top = rc.bottom + EXAMPLE_STATIONALL_TOP_MARGIN;
				rc.bottom = rc.top + TEXT_MARGIN;
				CRect exampleLineRc = rc;
				rc.OffsetRect(EXAMPLE_LINE_WIDTH + 2, 0);

				// SKYLINE
				CPen penExampleSkyline;
				if (!penExampleSkyline.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.skyl_color))
					return FALSE;
				memDC.SelectObject(&penExampleSkyline);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				if (strMsg.LoadString(IDS_RADARGRAPH_SKYLINE) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				exampleLineRc.OffsetRect(0, 14);

				// RF-TRIP
				CPen penExampleRftrip;
				if (!penExampleRftrip.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.rftr_color))
					return FALSE;
				memDC.SelectObject(&penExampleRftrip);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				rc.OffsetRect(0, 14);
				if (strMsg.LoadString(IDS_RADARGRAPH_RFTRIP) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				exampleLineRc.OffsetRect(0, 14);

				// �\��l
				CPen penExampleForecastValue;
				if (!penExampleForecastValue.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.yoho_color))
					return FALSE;
				memDC.SelectObject(&penExampleForecastValue);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				rc.OffsetRect(0, 14);
				if (strMsg.LoadString(IDS_FORECASTLIST_FORECASTVALUE) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				exampleLineRc.OffsetRect(0, 14);

				rc = textrect;
				rc.left = rc.left + EXAMPLE_STATIONALL_LEFT_MARGIN;
				rc.right = rc.left + EXAMPLE_STATIONALL_AREA_WIDTH;
				rc.top = rc.bottom + EXAMPLE_STATIONALL_TOP_MARGIN;
				rc.bottom = rc.top + TEXT_MARGIN;
				exampleLineRc = rc;
				rc.OffsetRect(EXAMPLE_LINE_WIDTH + EXAMPLE_STATIONALL_AREA_WIDTH / 2 + 2, 0);
				exampleLineRc.OffsetRect(EXAMPLE_STATIONALL_AREA_WIDTH / 2, 0);

				// AOS
				CPen penExampleAosValue;
				if (!penExampleAosValue.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.aos_yoho_color))
					return FALSE;
				memDC.SelectObject(&penExampleAosValue);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				if (strMsg.LoadString(IDS_RADARGRAPH_AOS) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				exampleLineRc.OffsetRect(0, 14);

				// �����l
				CPen penExampleActualValue;
				if (!penExampleActualValue.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.trnd_color))
					return FALSE;
				memDC.SelectObject(&penExampleActualValue);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				rc.OffsetRect(0, 14);
				if (strMsg.LoadString(IDS_RADARGRAPH_ACTUAL_VALUE) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

				penExampleSkyline.DeleteObject();
				penExampleRftrip.DeleteObject();
				penExampleForecastValue.DeleteObject();
				penExampleAosValue.DeleteObject();
				penExampleActualValue.DeleteObject();
			}
			// �ꗗ���
			else
			{
				int nExapleWidth;
				if (m_nViewType == eRadarViewType_OperationPlan)
				{
					// �v��ꗗ�̏ꍇ
					nExapleWidth = EXAMPLE_AREA_WIDTH3;
				}
				else
				{
					// �\��l�ꗗ�̏ꍇ
					nExapleWidth = EXAMPLE_AREA_WIDTH4;
				}

				// �}��`��
				CString strMsg;
				textrect.left += 10;
				textrect.right = textrect.left + EXAMPLE_AREA_WIDTH;
				textrect.top = textrect.bottom + 10;
				textrect.bottom = textrect.top + TEXT_MARGIN;
				rc = textrect;
				CRect exampleLineRc = rc;
				rc.OffsetRect(EXAMPLE_LINE_WIDTH + 2, 0);

				// SKYLINE
				CPen penExampleSkyline;
				if (!penExampleSkyline.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.skyl_color))
					return FALSE;
				memDC.SelectObject(&penExampleSkyline);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				if (strMsg.LoadString(IDS_RADARGRAPH_SKYLINE) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				exampleLineRc.OffsetRect(nExapleWidth, 0);

				// RF�g���b�v�̓T�}����ʂ̂ݕ\��
				CPen penExampleRftrip;
				if (m_nViewType == eRadarViewType_Station)
				{
					// RF-TRIP
					if (!penExampleRftrip.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.rftr_color))
						return FALSE;
					memDC.SelectObject(&penExampleRftrip);
					memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
					memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
					rc.OffsetRect(nExapleWidth, 0);
					if (strMsg.LoadString(IDS_RADARGRAPH_RFTRIP) == FALSE)
						return FALSE;
					memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
					exampleLineRc.OffsetRect(nExapleWidth, 0);
				}

				// �\��l
				CPen penExampleForecastValue;
				if (!penExampleForecastValue.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.yoho_color))
					return FALSE;
				memDC.SelectObject(&penExampleForecastValue);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				rc.OffsetRect(nExapleWidth, 0);
				if (strMsg.LoadString(IDS_FORECASTLIST_FORECASTVALUE) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				exampleLineRc.OffsetRect(nExapleWidth, 0);

				// AOS
				CPen penExampleAosValue;
				if (!penExampleAosValue.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.aos_yoho_color))
					return FALSE;
				memDC.SelectObject(&penExampleAosValue);
				memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
				memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
				rc.OffsetRect(nExapleWidth, 0);
				if (strMsg.LoadString(IDS_RADARGRAPH_AOS) == FALSE)
					return FALSE;
				memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				exampleLineRc.OffsetRect(nExapleWidth, 0);

				// �����l�͌v��ꗗ�̂ݕ\��
				CPen penExampleActualValue;
				if (m_nViewType == eRadarViewType_OperationPlan)
				{
					// �����l
					if (!penExampleActualValue.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.trnd_color))
						return FALSE;
					memDC.SelectObject(&penExampleActualValue);
					memDC.MoveTo(exampleLineRc.left, exampleLineRc.top + exampleLineRc.Height() / 2);
					memDC.LineTo(exampleLineRc.left + EXAMPLE_LINE_WIDTH, exampleLineRc.top + exampleLineRc.Height() / 2);
					rc.OffsetRect(nExapleWidth, 0);
					if (strMsg.LoadString(IDS_RADARGRAPH_ACTUAL_VALUE) == FALSE)
						return FALSE;
					memDC.DrawText(strMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				}

				penExampleSkyline.DeleteObject();
				// RF�g���b�v�̓T�}����ʂ̂ݕ\��
				if (m_nViewType == eRadarViewType_Station)
				{
					penExampleRftrip.DeleteObject();
				}
				penExampleForecastValue.DeleteObject();
				penExampleAosValue.DeleteObject();
				if (m_nViewType == eRadarViewType_OperationPlan)
				{
					penExampleActualValue.DeleteObject();
				}
			}
		}

		// 1�ǃT�}�����
		if (bViewValues)
		{
			int x, y;

			// �l�̕\��
			// �\�g�`��
			rcValues.DeflateRect(VALUES_AREA_HMARGIN, VALUES_AREA_VMARGIN);
			memDC.SelectObject(&penLine);

			// �����`��
			for (int i = 0; i < 5; i++)
			{
				y = rcValues.top + i * VALUES_AREA_ROW_H;
				memDC.MoveTo(rcValues.left, y);
				memDC.LineTo(rcValues.right, y);
			}
			int y2 = rcValues.top + 5 * VALUES_AREA_ROW_H;
			memDC.MoveTo(rcValues.left, y2);
			memDC.LineTo(rcValues.left + (int)((double)rcValues.Width() * (VALUES_AREA_FORECAST_W + VALUES_AREA_TIME_W) + 2), y2);

			// �c���`��
			x = 0;
			memDC.MoveTo(rcValues.left + x, rcValues.top);
			memDC.LineTo(rcValues.left + x, y2);
			x += (int)((double)rcValues.Width() * VALUES_AREA_FORECAST_W + 1);
			memDC.MoveTo(rcValues.left + x, rcValues.top);
			memDC.LineTo(rcValues.left + x, y2);
			x += (int)((double)rcValues.Width() * VALUES_AREA_TIME_W + 1);
			memDC.MoveTo(rcValues.left + x, rcValues.top);
			memDC.LineTo(rcValues.left + x, y2);
			x += (int)((double)rcValues.Width() * VALUES_AREA_AZ_W);
			memDC.MoveTo(rcValues.left + x, rcValues.top);
			memDC.LineTo(rcValues.left + x, y);
			x += (int)((double)rcValues.Width() * VALUES_AREA_EL_W);
			memDC.MoveTo(rcValues.left + x, rcValues.top);
			memDC.LineTo(rcValues.left + x, y);

			CRect rcValuesTmp = rcValues;
			double dTime;
			char caTime[40];
			CString strMsg;
			CSize szStr;
			WCHAR wc[30];
			size_t chgNum;

			// ���ڕ`��
			{
				if (strMsg.LoadString(IDS_FORECASTLIST_FORECASTVALUE) == FALSE)
					return FALSE;
				rcValuesTmp.right = (int)((double)rcValues.Width() * VALUES_AREA_FORECAST_W + 1);
				rcValuesTmp.bottom = rcValuesTmp.top + VALUES_AREA_ROW_H;
				memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				if (strMsg.LoadString(IDS_RADARGRAPH_TIME) == FALSE)
					return FALSE;
				rcValuesTmp.left = rcValuesTmp.right;
				rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_TIME_W + 1);
				memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				if (strMsg.LoadString(IDS_RADARGRAPH_AZ) == FALSE)
					return FALSE;
				rcValuesTmp.left = rcValuesTmp.right;
				rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_AZ_W);
				memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				if (strMsg.LoadString(IDS_RADARGRAPH_EL) == FALSE)
					return FALSE;
				rcValuesTmp.left = rcValuesTmp.right;
				rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_EL_W);
				memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}

			// AOS���`��
			{
				if (strMsg.LoadString(IDS_RADARGRAPH_AOSINFO) == FALSE)
					return FALSE;
				rcValuesTmp = rcValues;
				rcValuesTmp.right = (int)((double)rcValues.Width() * VALUES_AREA_FORECAST_W + 1);
				rcValuesTmp.top = rcValuesTmp.top + VALUES_AREA_ROW_H;
				rcValuesTmp.bottom = rcValuesTmp.top + VALUES_AREA_ROW_H;
				memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				if (CSatelliteData::ub_asc_to_asctime(pCtrlMonData->m_gst_dspinf.ca_AOS_time, caTime) >= 0)
				{
					dTime = CSatelliteData::ub_asc_to_dtime(caTime);
					CTime t((__time64_t)dTime);
					strMsg = t.Format("%Y-%m-%d %H:%M:%S");
					rcValuesTmp.left = rcValuesTmp.right;
					rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_TIME_W + 1);
					memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

					mbstowcs_s(&chgNum, wc, 30, pCtrlMonData->m_gst_dspinf.ca_AOS_AZ, _TRUNCATE);
					strMsg.Format(_T("%s"), wc);
					rcValuesTmp.left = rcValuesTmp.right;
					rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_AZ_W);
					memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

					mbstowcs_s(&chgNum, wc, 30, pCtrlMonData->m_gst_dspinf.ca_AOS_EL, _TRUNCATE);
					strMsg.Format(_T("%s"), wc);
					rcValuesTmp.left = rcValuesTmp.right;
					rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_EL_W);
					memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
				}
			}

			// LOS���`��
			{
				if (strMsg.LoadString(IDS_RADARGRAPH_LOSINFO) == FALSE)
					return FALSE;
				rcValuesTmp = rcValues;
				rcValuesTmp.right = (int)((double)rcValues.Width() * VALUES_AREA_FORECAST_W + 1);
				rcValuesTmp.top = rcValuesTmp.top + VALUES_AREA_ROW_H * 2;
				rcValuesTmp.bottom = rcValuesTmp.top + VALUES_AREA_ROW_H;
				memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				if (CSatelliteData::ub_asc_to_asctime(pCtrlMonData->m_gst_dspinf.ca_LOS_time, caTime) >= 0)
				{
					dTime = CSatelliteData::ub_asc_to_dtime(caTime);
					CTime t((__time64_t)dTime);
					strMsg = t.Format("%Y-%m-%d %H:%M:%S");
					rcValuesTmp.left = rcValuesTmp.right;
					rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_TIME_W + 1);
					memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

					mbstowcs_s(&chgNum, wc, 30, pCtrlMonData->m_gst_dspinf.ca_LOS_AZ, _TRUNCATE);
					strMsg.Format(_T("%s"), wc);
					rcValuesTmp.left = rcValuesTmp.right;
					rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_AZ_W);
					memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

					mbstowcs_s(&chgNum, wc, 30, pCtrlMonData->m_gst_dspinf.ca_LOS_EL, _TRUNCATE);
					strMsg.Format(_T("%s"), wc);
					rcValuesTmp.left = rcValuesTmp.right;
					rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_EL_W);
					memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
				}
			}

			// �ő�p�`��
			{
				if (strMsg.LoadString(IDS_RADARGRAPH_MEL) == FALSE)
					return FALSE;
				rcValuesTmp = rcValues;
				rcValuesTmp.right = (int)((double)rcValues.Width() * VALUES_AREA_FORECAST_W + 1);
				rcValuesTmp.top = rcValuesTmp.top + VALUES_AREA_ROW_H * 3;
				rcValuesTmp.bottom = rcValuesTmp.top + VALUES_AREA_ROW_H;
				memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				if (CSatelliteData::ub_asc_to_asctime(pCtrlMonData->m_gst_dspinf.ca_MEL_time, caTime) >= 0)
				{
					dTime = CSatelliteData::ub_asc_to_dtime(caTime);
					CTime t((__time64_t)dTime);
					strMsg = t.Format("%Y-%m-%d %H:%M:%S");
					rcValuesTmp.left = rcValuesTmp.right;
					rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_TIME_W + 1);
					memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

					mbstowcs_s(&chgNum, wc, 30, pCtrlMonData->m_gst_dspinf.ca_MEL_AZ, _TRUNCATE);
					strMsg.Format(_T("%s"), wc);
					rcValuesTmp.left = rcValuesTmp.right;
					rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_AZ_W);
					memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

					mbstowcs_s(&chgNum, wc, 30, pCtrlMonData->m_gst_dspinf.ca_MEL_EL, _TRUNCATE);
					//strMsg.Format(_T("%s"), wc);
					strMsg.Format(_T("%s"), (LPCTSTR)CString(pCtrlMonData->m_gst_dspinf.ca_MEL_EL));
					rcValuesTmp.left = rcValuesTmp.right;
					rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_EL_W);
					memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
				}
			}

			// �����ԕ`��
			{
				if (strMsg.LoadString(IDS_RADARGRAPH_VISIBLETIME) == FALSE)
					return FALSE;
				rcValuesTmp = rcValues;
				rcValuesTmp.right = (int)((double)rcValues.Width() * VALUES_AREA_FORECAST_W + 1);
				rcValuesTmp.top = rcValuesTmp.top + VALUES_AREA_ROW_H * 4;
				rcValuesTmp.bottom = rcValuesTmp.top + VALUES_AREA_ROW_H;
				memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				mbstowcs_s(&chgNum, wc, 30, pCtrlMonData->m_gst_dspinf.ca_visible, _TRUNCATE);
				strMsg.Format(_T("%s"), wc);
				szStr = memDC.GetTextExtent(strMsg);
				rcValuesTmp.left = rcValuesTmp.right;
				rcValuesTmp.right = rcValuesTmp.left + (int)((double)rcValues.Width() * VALUES_AREA_TIME_W + 1);
				memDC.DrawText(strMsg, rcValuesTmp, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
			}
		}
	}

	memDC.SelectObject(oldBrush);
	memDC.SelectObject(oldPen);
	penSolidBold.DeleteObject();
	penSolidBold2.DeleteObject();
	penSolid.DeleteObject();
	penLine.DeleteObject();
	penLine2.DeleteObject();
	brushBk.DeleteObject();
	brush80.DeleteObject();

	return TRUE;
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# ����������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CForecastRadarGraphView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// ����������
	Init(_T(""), _T(""), _T(""));
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# ����������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CForecastRadarGraphView::Init(CString fpath, CString satellite, CString predId)
{
//	if (m_nStationID < 0)
//		return;

	// �r���[�^�C�v�Ƌ�ID����A�q���f�[�^���擾
	switch (m_nViewType)
	{
	case eRadarViewType_Station:
		// �ǂ̏ꍇ�͋�ID���Ď��f�[�^ID�ɂ���
		m_nCtrlMonDataID = m_nStationID;
		break;

	case eRadarViewType_ForecastList:
		// �\��l�ꗗ�̏ꍇ�͋�+1��ID�l�ɂ���
		m_nCtrlMonDataID = eStation_MAX;
		break;

	case eRadarViewType_OperationPlan:
	default:
		// �v��ꗗ�̏ꍇ�͋�+2��ID�l�ɂ���
		m_nCtrlMonDataID = eStation_MAX + 1;
		break;
	}

	CGraphMutex::Lock(eRadar);

	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		CGraphMutex::Unlock(eRadar);
		return;
	}

	// �r���[ID�Ƌǔԍ��Z�b�g
	pCtrlMonData->SetViewID(m_nCtrlMonDataID);
	pCtrlMonData->SetStationID(m_nStationID);
	// �L���[�̃N���A
	pCtrlMonData->ClearQeue();

	// �\���F�ݒ�
	//	pCtrlMonData->m_gst_dspinf.pixmap_color		= PIXMAP_CLR;
	pCtrlMonData->m_gst_dspinf.grph_color = GRPH_CLR;
	pCtrlMonData->m_gst_dspinf.frem_color = FREM_CLR;
	pCtrlMonData->m_gst_dspinf.frem_color2 = FREM_CLR2;
	pCtrlMonData->m_gst_dspinf.e80_frem_color = E80_CLR;
	pCtrlMonData->m_gst_dspinf.grid_color = GRID_CLR;
	pCtrlMonData->m_gst_dspinf.grid_color2 = GRID_CLR2;
	pCtrlMonData->m_gst_dspinf.strtit_color = STRTIT_CLR;
	pCtrlMonData->m_gst_dspinf.strcon_color = STRCON_CLR;
	pCtrlMonData->m_gst_dspinf.strvar_color = STRVAR_CLR;
	pCtrlMonData->m_gst_dspinf.aos_yoho_color = YOHO_AOS_CLR;
	pCtrlMonData->m_gst_dspinf.los_yoho_color = YOHO_LOS_CLR;
	pCtrlMonData->m_gst_dspinf.yoho_color = YOHO_CLR;
	pCtrlMonData->m_gst_dspinf.trnd_color = TRND_CLR;
	pCtrlMonData->m_gst_dspinf.skyl_color = SKYL_CLR;
	pCtrlMonData->m_gst_dspinf.rftr_color = RFTR_CLR;
	pCtrlMonData->m_gst_dspinf.over_color = OVER_CLR;

//	// �_�~�[�f�[�^������
//	if (fpath.IsEmpty() && m_nViewType == eRadarViewType_Station)
//	{
//		fpath = theApp.GetShareFilePath(eFileType_Yoho_File, m_nStationID);
//	}
//	// �v��ꗗ�łȂ��ꍇ
//	else if (!fpath.IsEmpty())
	{
		// �g���q��prd�̏ꍇ�Aanp�ɒu��������
		fpath.TrimRight(PRED_FILE_TYPE);
		fpath += ANP_FILE_TYPE;
	}

	// �^�p�󋵊Ď��f�[�^������������
	InitGtunyouData(fpath, satellite, predId);

	CGraphMutex::Unlock(eRadar);

	// �O���t�f�[�^�X�V
	UpdateGraphData();

	// �^�C�}�o�^
	SetTimer(1, INTERVAL_REAL, NULL);

#if 0
// test >>>
	CRFTripAngleFile	rftrip;
	vector<double> listAzEl;
//	if (rftrip.ReadFile((UINT)m_nStationID, CString(_T("HAYABUSA2")), CString(_T("HPA1-100kw")), CString(_T("ABCD_BPSK_123")), listAzEl))
	if (rftrip.ReadFile((UINT)m_nStationID, CString(_T("HAYABUSA2")), CString(_T("HPA1-100kw")), CString(_T("")), listAzEl))
	{
		TRACE("Read RfTripFile OK\n");
	}
	else
	{
		TRACE("Read RfTripFile NG\n");
	}

// test <<<
#endif
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# �^�p�󋵊Ď��f�[�^������������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CForecastRadarGraphView::InitGtunyouData(CString fpath, CString& satellite, CString& predId)
{
	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		return;
	}

	pCtrlMonData->m_l_inflg = 0;	// �����ɂ��Ă���
	pCtrlMonData->m_gi_cmp1 = 1;
	pCtrlMonData->m_gi_cmp2 = 1;
	pCtrlMonData->m_gst_dspinf.i_dsp_type = TYPE_NEW;
	pCtrlMonData->m_gst_dspinf.i_unyou_mode = CTrackingRadarCtrlMonData::eUNYOU_IN;

	// ������
	wcscpy_s(pCtrlMonData->m_gst_dspinf.ca_fpath, 1024, fpath.GetBuffer());
	fpath.ReleaseBuffer();

	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_stime, 30, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_etime, 30, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_ftime, 30, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_plnno, 30, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_eisei, 100, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_AOS_time, 20, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_AOS_AZ, 9, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_AOS_EL, 8, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_LOS_time, 20, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_LOS_AZ, 9, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_LOS_EL, 8, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_MEL_time, 20, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_MEL_AZ, 9, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_MEL_EL, 8, "");
	strcpy_s(pCtrlMonData->m_gst_dspinf.ca_visible, 9, "");
	strcpy_s(pCtrlMonData->m_ca_yohouget, 30, "");

	if (m_nStationID < 0)
		return;

	// �\��l�ꗗ�A�v��ꗗ����擾���Ă����l���Z�b�g
	// �ǁA�q���A�\��l�t�@�C�����ƈ�v����\��l��\��l�ꗗ���X�g����T��
	CGraphMutex::Lock(eFile);
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();
	CTime ctime;
	CStringA strA;
	for (int i = 0; i < (int)listPred.size(); i++)
	{
		if (listPred[i].strStationName.CompareNoCase(mStationString[m_nStationID]) == 0 && 
			listPred[i].strSatellite.CompareNoCase(satellite) == 0 &&
			listPred[i].strPassId.CompareNoCase(predId) == 0)
		{
			pCtrlMonData->m_l_inflg = INFLG_ON;	// �f�[�^�L��

			// ������
			wcscpy_s(pCtrlMonData->m_gst_dspinf.ca_fpath, 1024, fpath.GetBuffer());
			fpath.ReleaseBuffer();

			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_plnno, 30, CStringA(m_strPlanID));							// �v��ꗗ��ʂ̏ꍇ�A�I�������v��̌v��ԍ����Z�b�g
			ctime = CTime(m_tPassStart);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_stime, 30, CStringA(ctime.Format("%Y-%m-%d %H:%M:%S")));	// �v��ꗗ��ʂ̏ꍇ�A�I�������v��̉^�p�J�n�������Z�b�g�i�v��ꗗ���X�g����擾�j
			ctime = CTime(m_tPassEnd);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_etime, 30, CStringA(ctime.Format("%Y-%m-%d %H:%M:%S")));	// �v��ꗗ��ʂ̏ꍇ�A�I�������v��̉^�p�I���������Z�b�g�i�v��ꗗ���X�g����擾�j

			// �\��l�t�@�C���X�V�����̎擾
			HANDLE hFile;
			CTime ftimestamp;
			hFile = CreateFile(fpath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				ftimestamp = CTime(0);
			}
			else
			{
				FILETIME cfTime, afTime, wfTime;
				GetFileTime(hFile, &cfTime, &afTime, &wfTime);
				CloseHandle(hFile);
				ftimestamp = CTime(wfTime);
			}
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_ftime, 30, CStringA(ftimestamp.Format("%Y-%m-%d %H:%M:%S")));	// �I�������\��l�t�@�C���^�C���X�^���v���Z�b�g�i�t�@�C������擾�j

			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_eisei, 100, CStringA(satellite));								// �q�����Z�b�g
			ctime = CTime(listPred[i].tAOS);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_AOS_time, 20, CStringA(ctime.Format("%Y-%m-%d %H:%M:%S")));		// AOS�����Z�b�g
			strA.Format("%3.4f", listPred[i].dAosAz);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_AOS_AZ, 9, strA);												// AOS AZ�Z�b�g
			strA.Format("%2.4f", listPred[i].dAosEl);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_AOS_EL, 8, strA);												// AOS EL�Z�b�g
			ctime = CTime(listPred[i].tLOS);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_LOS_time, 20, CStringA(ctime.Format("%Y-%m-%d %H:%M:%S")));		// LOS�����Z�b�g
			strA.Format("%3.4f", listPred[i].dLosAz);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_LOS_AZ, 9, strA);												// LOS AZ�Z�b�g
			strA.Format("%2.4f", listPred[i].dLosEl);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_LOS_EL, 8, strA);												// LOS EL�Z�b�g
			ctime = CTime(listPred[i].tMaxEl);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_MEL_time, 20, CStringA(ctime.Format("%Y-%m-%d %H:%M:%S")));		// MAX�����Z�b�g
			strA.Format("%3.4f", listPred[i].dMaxAz);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_MEL_AZ, 9, strA);												// MAX AZ�Z�b�g
			strA.Format("%2.4f", listPred[i].dMaxEl);
			strcpy_s(pCtrlMonData->m_gst_dspinf.ca_MEL_EL, 8, strA);												// MAX EL�Z�b�g

			break;
		}
	}
	CGraphMutex::Unlock(eFile);

	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	CTime t = timeSys;
//	m_dummyInitTime = t.GetTime();
	pCtrlMonData->m_i_unyou = 1;

#ifdef _DEBUG
	m_dummyCnt = 0;
#endif
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# �_�~�[�����l�f�[�^���쐬����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CForecastRadarGraphView::AddDummyData()
{
	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		return;
	}

	CGraphMutex::Lock(eRadar);

	int sz = (int)pCtrlMonData->m_queAzElForecast.size();
	static double offset = 0.0;
	static double inc = 0.2;

	if (m_dummyCnt == sz)
	{
		CGraphMutex::Unlock(eRadar);
		return;
	}

	if (m_dummyCnt < sz / 2)
	{
		offset = offset + inc;
	}
	else
	{
		offset = offset - inc;
	}
	Az_El	azel;
	azel.d_AZ = pCtrlMonData->m_queAzElForecast[m_dummyCnt].d_AZ + offset;
	azel.d_EL = pCtrlMonData->m_queAzElForecast[m_dummyCnt].d_EL + offset;
	pCtrlMonData->m_queAzElActual.push_back(azel);
	m_dummyCnt++;

	CGraphMutex::Unlock(eRadar);
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# �^�C�}�C�x���g�������ɌĂяo�����

@param	nIDEvent�F�C�x���gID
@retval	�Ȃ�

*/
/*============================================================================*/
void CForecastRadarGraphView::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(1);

	UpdateGraphData();

	SetTimer(1, INTERVAL_REAL, NULL);

	CView::OnTimer(nIDEvent);
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# �O���t�f�[�^�����X�V����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CForecastRadarGraphView::UpdateGraphData()
{
	CGraphMutex::Lock(eRadar);

	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		return;
	}

	// ���X�V
	pCtrlMonData->UpdateData();

	CGraphMutex::Unlock(eRadar);

	// �r���[�X�V
	Invalidate();
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# �X�J�C���C���`��

@param	memDC		�h�L�������g
@param	rectClient	�`��̈�
@retval	TRUE:�X�J�C���C����`�悵�� / FALSE:�X�J�C���C����`�悵�Ȃ�����

*/
/*============================================================================*/
BOOL CForecastRadarGraphView::DrawSkyline(CDC& memDC, CRect& rectClient)
{
	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		return FALSE;
	}

	CPen penSolid;
	if (!penSolid.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.skyl_color))
		return FALSE;
	CPen* oldPen = memDC.SelectObject(&penSolid);
	CBrush brush;
	brush.CreateSolidBrush(pCtrlMonData->m_gst_dspinf.skyl_color);
	CBrush* oldBrush = (CBrush*)memDC.SelectObject(&brush);

	int     i_X, i_Y;               /* �O���t�`����W             	  */
	POINT ptLast, ptLastCircle;
	POINT ptCur, ptCurCircle;
	POINT pt[4];
	double rad;

	for (int i = 0; i < pCtrlMonData->m_queAzElSkyline.size(); i++)
	{
		GtazelToXY(pCtrlMonData->m_queAzElSkyline[i].d_AZ, pCtrlMonData->m_queAzElSkyline[i].d_EL, (double)m_ptCenter.x,
			(double)m_ptCenter.y, m_r, &i_X, &i_Y);
		/* �O���t�`�� */
//		memDC.SetPixel(i_X, i_Y, pCtrlMonData->m_gst_dspinf.skyl_color);
		// �|���S��
		ptCur.x = i_X; ptCur.y = i_Y;
//		rad = atan2(-(ptCur.y - m_ptCenter.y) , (ptCur.x - m_ptCenter.x));
		rad = atan2((ptCur.x - m_ptCenter.x), -(ptCur.y - m_ptCenter.y));
//		if (rad < 0)
//		{
//			rad = rad + 2 * 3.1415926538;
//		}

		ptCurCircle.x = (int)(sin(rad) * m_r) + m_ptCenter.x;
		ptCurCircle.y = -(int)(cos(rad) * m_r) + m_ptCenter.y;

//		memDC.SetPixel(ptCurCircle.x, ptCurCircle.y, pCtrlMonData->m_gst_dspinf.skyl_color);

		if (i == 0)
		{
			ptLast = ptCur;
			ptLastCircle = ptCurCircle;
			continue;
		}

		pt[0] = ptLast;
		pt[1] = ptCur;
		pt[2] = ptCurCircle;
		pt[3] = ptLastCircle;

		memDC.Polygon(pt, 4);

		ptLast = ptCur;
		ptLastCircle = ptCurCircle;
	}

	memDC.SelectObject(oldBrush);
	memDC.SelectObject(oldPen);
	penSolid.DeleteObject();
	brush.DeleteObject();

	return TRUE;
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# RF�g���b�v�`��

@param	memDC		�h�L�������g
@param	rectClient	�`��̈�
@retval	TRUE:RF�g���b�v��`�悵�� / FALSE:RF�g���b�v��`�悵�Ȃ�����

*/
/*============================================================================*/
BOOL CForecastRadarGraphView::DrawRfTrip(CDC& memDC, CRect& rectClient)
#if 0
{
	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		return FALSE;
	}

	CPen penSolid;
	if (!penSolid.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.rftr_color))
		return FALSE;
	CPen* oldPen = memDC.SelectObject(&penSolid);

	int     i_X, i_Y;               /* �O���t�`����W             	  */
	int		i_X_0, i_Y_0;
	BOOL	bExistI0 = FALSE;
	for (int i = 0; i < pCtrlMonData->m_queAzElRftrip.size(); i++)
	{
		GtazelToXY(pCtrlMonData->m_queAzElRftrip[i].d_AZ, pCtrlMonData->m_queAzElRftrip[i].d_EL, (double)m_ptCenter.x,
			(double)m_ptCenter.y, m_r, &i_X, &i_Y);
		/* �O���t�`�� */
//		memDC.SetPixel(i_X, i_Y, pCtrlMonData->m_gst_dspinf.rftr_color);
		if (i == 0)
		{
			memDC.MoveTo(i_X, i_Y);
			i_X_0 = i_X;
			i_Y_0 = i_Y;
			bExistI0 = TRUE;
		}
		else
		{
			memDC.LineTo(i_X, i_Y);
		}
	}
	if (bExistI0)
		memDC.LineTo(i_X_0, i_Y_0);

	return TRUE;
}
#else
{
	if (m_nStationID < 0)
		return FALSE;

	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		return FALSE;
	}

	CPen penSolid;
	if (!penSolid.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.rftr_color))
		return FALSE;
	CPen* oldPen = memDC.SelectObject(&penSolid);

	int     i_X, i_Y;               /* �O���t�`����W             	  */
	int		i_X_0, i_Y_0;
	BOOL	bExistI0 = FALSE;

#if 0
	vector<Az_El> listAzEl;
	theApp.GetSatelliteData().GetRFTripAngleData(m_nStationID).GetAzEl(listAzEl);

	for (int i = 0; i < listAzEl.size(); i++)
	{
		GtazelToXY(listAzEl[i].d_AZ, listAzEl[i].d_EL, (double)m_ptCenter.x,
			(double)m_ptCenter.y, m_r, &i_X, &i_Y);
		/* �O���t�`�� */
		//		memDC.SetPixel(i_X, i_Y, pCtrlMonData->m_gst_dspinf.rftr_color);
		if (i == 0)
		{
			memDC.MoveTo(i_X, i_Y);
			i_X_0 = i_X;
			i_Y_0 = i_Y;
			bExistI0 = TRUE;
		}
		else
		{
			memDC.LineTo(i_X, i_Y);
		}
	}
	if (bExistI0)
		memDC.LineTo(i_X_0, i_Y_0);
#else

	vector<double> listAzEl;
	theApp.GetSatelliteData().GetRFTripAngleData(m_nStationID).GetAzEl(listAzEl);

	for (__int64 i = 0; i < (__int64)listAzEl.size(); i++)
	{
		GtazelToXY((double)i, listAzEl[i], (double)m_ptCenter.x,
			(double)m_ptCenter.y, m_r, &i_X, &i_Y);
		/* �O���t�`�� */
		//		memDC.SetPixel(i_X, i_Y, pCtrlMonData->m_gst_dspinf.rftr_color);
		if (i == 0)
		{
			memDC.MoveTo(i_X, i_Y);
			i_X_0 = i_X;
			i_Y_0 = i_Y;
			bExistI0 = TRUE;
		}
		else
		{
			memDC.LineTo(i_X, i_Y);
		}
	}
	if (bExistI0)
		memDC.LineTo(i_X_0, i_Y_0);

#endif

	return TRUE;
}
#endif


/*============================================================================*/
/*! CForecastRadarGraphView

-# �\��l�`��

@param	memDC		�h�L�������g
@param	rectClient	�`��̈�
@retval	TRUE:�\��l��`�悵�� / FALSE:�\��l��`�悵�Ȃ�����

*/
/*============================================================================*/
BOOL CForecastRadarGraphView::DrawForecast(CDC& memDC, CRect& rectClient)
{
	if (m_nStationID < 0)
		return FALSE;

	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		return FALSE;
	}

	CTrackingRadarCtrlMonData* pStationData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nStationID);
	if (pStationData == NULL)
	{
		return FALSE;
	}

	CPen penAos, penYoho, penYohoBold;
	if (!penAos.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.aos_yoho_color))
		return FALSE;
	if (!penYoho.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.yoho_color))
		return FALSE;
	if (!penYohoBold.CreatePen(PS_SOLID, LINE_WID_FRAM_BOLD2, pCtrlMonData->m_gst_dspinf.yoho_color))
		return FALSE;

	CPen* oldPen = memDC.SelectObject(&penAos);

	int     i_X, i_Y;					/* �O���t�`����W          	  */
	char	  ca_stime[40];				/* �p�X�^�p�J�n�����i�t���j   */
	char	  ca_etime[40];				/* �p�X�^�p�I�������i�t���j   */
	double	  d_Stime;					/* �p�X�^�p�J�n�����i�ώZ�b�j */
	double	  d_Etime;					/* �p�X�^�p�I�������i�ώZ�b�j */
	double    d_time;                   /* �O���t�f�[�^��������       */

	if (m_nViewType == eRadarViewType_OperationPlan || m_nViewType == eRadarViewType_ForecastList)
	{
		// �v��ꗗ�̏ꍇ
		/* �p�X�J�n�����i�ώZ�b�j�Z�o */
		CSatelliteData::ub_asc_to_asctime(pCtrlMonData->m_gst_dspinf.ca_stime, ca_stime);
		d_Stime = CSatelliteData::ub_asc_to_dtime(ca_stime);

		/* �p�X�I�������i�ώZ�b�j�Z�o */
		CSatelliteData::ub_asc_to_asctime(pCtrlMonData->m_gst_dspinf.ca_etime, ca_etime);
		d_Etime = CSatelliteData::ub_asc_to_dtime(ca_etime);
	}
	else
	{
		// �T�}����ʂ̏ꍇ
		/* �p�X�J�n�����i�ώZ�b�j�Z�o */
		CSatelliteData::ub_asc_to_asctime(pStationData->m_gst_dspinf.ca_stime, ca_stime);
		d_Stime = CSatelliteData::ub_asc_to_dtime(ca_stime);

		/* �p�X�I�������i�ώZ�b�j�Z�o */
		CSatelliteData::ub_asc_to_asctime(pStationData->m_gst_dspinf.ca_etime, ca_etime);
		d_Etime = CSatelliteData::ub_asc_to_dtime(ca_etime);
	}

	__int64 size = (__int64)pCtrlMonData->m_queAzElForecast.size();

	for (__int64 i = 0; i < size; i++)
	{
		/* �f�[�^�������� �f�d�s */
		d_time = pCtrlMonData->m_queAzElForecast[i].d_time;

		GtazelToXY(pCtrlMonData->m_queAzElForecast[i].d_AZ, pCtrlMonData->m_queAzElForecast[i].d_EL, (double)m_ptCenter.x,
			(double)m_ptCenter.y, m_r, &i_X, &i_Y);

		/* �v���b�g�t���O�̏����� */
		if (i == 0) m_i_plot_flag = CTrackingRadarCtrlMonData::eOFF;
		else {
			if (pCtrlMonData->m_queAzElForecast[i].d_aos >= 0.0 && pCtrlMonData->m_queAzElForecast[i].d_aos < 0.1) {
				memDC.SelectObject(&penAos);
				m_i_plot_flag = CTrackingRadarCtrlMonData::eON;
			}
			else {
				if (m_i_plot_flag == CTrackingRadarCtrlMonData::eOFF) {
					memDC.SelectObject(&penAos);
				}
				else {
					// �v��ꗗ�̒ǔ��O���t�̏ꍇ
//					if (m_nViewType == eRadarViewType_OperationPlan && d_time >= d_Stime && d_time <= d_Etime)
					if (m_nViewType != eRadarViewType_ForecastList && d_time >= d_Stime && d_time <= d_Etime)
					{
						memDC.SelectObject(&penYohoBold);
					}
					else
					{
						memDC.SelectObject(&penYoho);
					}
				}
				m_i_plot_flag = CTrackingRadarCtrlMonData::eCOMP;
			}
		}

		/* �v���b�g���[�v�P��ڈȊO�Ȃ�A�\��l����ŕ`�悷�� */
		if (i != 0) {
			memDC.LineTo(i_X, i_Y);
		}
		else
		{
			memDC.MoveTo(i_X, i_Y);
		}
	}

	memDC.SelectObject(oldPen);
	penAos.DeleteObject();
	penYoho.DeleteObject();
	penYohoBold.DeleteObject();

	return TRUE;
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# �ǔ��O���t�ۑ��f�[�^�`��

@param	memDC		�h�L�������g
@param	rectClient	�`��̈�
@retval	TRUE:�ǔ��O���t�ۑ��f�[�^��`�悵�� / FALSE:�ǔ��O���t�ۑ��f�[�^��`�悵�Ȃ�����

*/
/*============================================================================*/
BOOL CForecastRadarGraphView::DrawTrakingSaveData(CDC& memDC, CRect& rectClient)
{
	if (m_nStationID < 0)
		return FALSE;

	int nCtrlMonDataID;
	if (m_nViewType == eRadarViewType_OperationPlan)
	{
		// �v��ꗗ�̒ǔ��̏ꍇ�́A�\��l�t�@�C���p�X���ǂ̃J�����g�̃t�@�C���p�X�ƈ�v���A���v��ID��v���Ɏ����l��\��
		CTrackingRadarCtrlMonData* pOprListData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nCtrlMonDataID);
		CTrackingRadarCtrlMonData* pStationData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(m_nStationID);
		if (pOprListData == NULL || pStationData == NULL)
		{
			return FALSE;
		}

		CString strOprStationPlanID = CString(pStationData->m_gst_dspinf.ca_plnno);
		if (pOprListData->m_yohoFilePath.CompareNoCase(pStationData->m_yohoFilePath) == 0 &&
			m_strPlanID.CompareNoCase(strOprStationPlanID) == 0)
		{
			nCtrlMonDataID = m_nStationID;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		nCtrlMonDataID = m_nCtrlMonDataID;
	}

	CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(nCtrlMonDataID);
	if (pCtrlMonData == NULL)
	{
		return FALSE;
	}

//	char	  ca_stime[40];				/* �p�X�^�p�J�n�����i�t���j   */
//	char	  ca_etime[40];				/* �p�X�^�p�I�������i�t���j   */
//	double	  d_Stime;					/* �p�X�^�p�J�n�����i�ώZ�b�j */
//	double	  d_Etime;					/* �p�X�^�p�I�������i�ώZ�b�j */
	int       i_X, i_Y;                 /* �O���t�`����W             */
//	double    d_time;                   /* �O���t�f�[�^��������       */

//	/*----------*/
//	/* �������� */
//	/*----------*/
//	/* �p�X�J�n�����i�ώZ�b�j�Z�o */
//	CSatelliteData::ub_asc_to_asctime(pCtrlMonData->m_gst_dspinf.ca_stime, ca_stime);
//	d_Stime = CSatelliteData::ub_asc_to_dtime(ca_stime);
//
//	/* �p�X�I�������i�ώZ�b�j�Z�o */
//	CSatelliteData::ub_asc_to_asctime(pCtrlMonData->m_gst_dspinf.ca_etime, ca_etime);
//	d_Etime = CSatelliteData::ub_asc_to_dtime(ca_etime);

	/*------------*/
	/* �O���t�`�� */
	/*------------*/
	CPen penActual;
	if (!penActual.CreatePen(PS_SOLID, LINE_WID_FRAM, pCtrlMonData->m_gst_dspinf.trnd_color))
		return FALSE;

	CPen* oldPen = memDC.SelectObject(&penActual);

	__int64 size = (__int64)pCtrlMonData->m_queAzElActual.size();

	for (__int64 i = 0; i < size; i++)
	{
//		/* �f�[�^�������� �f�d�s */
//		d_time = pCtrlMonData->m_queAzElActual[i].d_time;
//
//#ifndef _DEBUG
//		/* �p�X�^�p�J�n�����O�̏ꍇ�`�悵�Ȃ� */
//		if (d_time < d_Stime || d_time > d_Etime){
//			continue;
//		}
//#endif
		if ((pCtrlMonData->m_queAzElActual[i].d_AZ != IGNORE_DATA) && (pCtrlMonData->m_queAzElActual[i].d_EL != IGNORE_DATA)) {

			/* �O���t�`����W�����߂� */
			GtazelToXY(pCtrlMonData->m_queAzElActual[i].d_AZ, pCtrlMonData->m_queAzElActual[i].d_EL,
				(double)m_ptCenter.x, (double)m_ptCenter.y,
				m_r, &i_X, &i_Y);
			if (i == 0)
			{
				memDC.MoveTo(i_X, i_Y);
			}
			else
			{
				memDC.LineTo(i_X, i_Y);
			}
		}
	}

	memDC.SelectObject(oldPen);
	penActual.DeleteObject();

	return TRUE;
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# �`�y�A�d�k���W�Z�o�֐�

@param	d_Az�F�`�y�l
@param	d_El�F�d�k�l
@param	d_CX�F�~�S�w���W
@param	d_CY�F�~�S�x���W
@param	d_RD�F�~�̔��a
@param	*i_OX�F�Z�o�w���W
@param	*i_OY�F�Z�o�x���W
@retval	�Ȃ�

*/
/*============================================================================*/
void CForecastRadarGraphView::GtazelToXY(double d_Az, double d_El, double d_CX, double d_CY, double d_RD, int* i_OX, int* i_OY)
{
	double		d_X, d_Y;

	d_X = d_CX + ((d_RD * ((90.0 - d_El) / 90.0)) * sin(d_Az * RADCV));
	d_Y = d_CY - ((d_RD * ((90.0 - d_El) / 90.0)) * cos(d_Az * RADCV));

	*i_OX = (int)d_X;
	*i_OY = (int)d_Y;
}


/*============================================================================*/
/*! CForecastRadarGraphView

-# �w�i�������ɌĂ΂��

@param  pDC�F�h�L�������g�̃f�o�C�X�R���e�L�X�g�|�C���^
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CForecastRadarGraphView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
