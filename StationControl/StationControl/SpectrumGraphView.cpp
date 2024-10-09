/*============================================================================*/
/*! SessionStatusTab.cpp

-# �X�y�N�g�����\����ʁ@�O���t�r���[
*/
/*============================================================================*/
// SpectrumGraphView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "SpectrumGraphView.h"
#include "SpectrumView.h"


/* �F�w�� for original */
#define BK_COLOR			RGB(255, 255, 255)  /* �w�i */
#define GRID_LINE_COLOR     RGB(  0,   0, 128)  /* �O���b�h�� */
#define Y_FIG_COLOR         RGB( 34, 139,  34)	/* Y������ */
#define X_FIG_COLOR         RGB( 30, 144, 255)	/* X������*/
#define PLOT_COLOR          RGB(  0, 128,   0)  /* �v���b�g */
#define PEAK_COLOR          RGB(255, 255,   0)	/* �s�[�N�}�[�N */
#define TIME_COLOR          RGB(  0, 255, 255)	/* �����\�� */
/* �F�w�� for BLACK */
#define BK_COLOR_B			RGB(  0,   0,   0)	/* �w�i */
#define GRID_LINE_COLOR_B   RGB(128, 128, 128)	/* �O���b�h�� */
#define Y_FIG_COLOR_B       RGB(152, 251, 152)  /* Y������ */
#define X_FIG_COLOR_B       RGB(152, 251, 152)	/* X������*/
#define PLOT_COLOR_B        RGB(  0, 128,   0)	/* �v���b�g */
#define PEAK_COLOR_B        RGB(255, 255,   0)	/* �s�[�N�}�[�N */
#define TIME_COLOR_B        RGB(  0, 255, 255)	/* �����\�� */
/* �F�w�� for WHITE */
#define BK_COLOR_W			RGB(128, 128, 128)	/* �w�i */
#define GRID_LINE_COLOR_W   RGB( 30, 144, 255)  /* �O���b�h�� */
#define Y_FIG_COLOR_W       RGB( 34, 139,  34)  /* Y������ */
#define X_FIG_COLOR_W       RGB( 34, 139,  34)  /* X������*/
#define PLOT_COLOR_W        RGB(  0,   0,   0)	/* �v���b�g */
#define PEAK_COLOR_W        RGB(255, 255,   0)	/* �s�[�N�}�[�N */
#define TIME_COLOR_W        RGB(  0, 255, 255)	/* �����\�� */

#define TMPBUFSIZ       ( 256 ) /* �e���|�����o�b�t�@�T�C�Y */

#define SPECTRUM_VMAX 0
#define SPECTRUM_VMIN -999

extern CSpectrumViewData::BandWidth_t spectrum_gast_bandw[];

// CSpectrumGraphView

IMPLEMENT_DYNCREATE(CSpectrumGraphView, CView)

/*============================================================================*/
/*! CSpectrumGraphView

-# �R���X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSpectrumGraphView::CSpectrumGraphView()
{
	CFont defFont;
	CFont* oldFont = NULL;
	defFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (defFont.GetLogFont(&lf)){
		lf.lfWeight = FW_NORMAL;
		lf.lfHeight = 12;
		m_txtFont.CreateFontIndirect(&lf);
	}
}

/*============================================================================*/
/*! CSpectrumGraphView

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSpectrumGraphView::~CSpectrumGraphView()
{
	m_txtFont.DeleteObject();
}

BEGIN_MESSAGE_MAP(CSpectrumGraphView, CView)
	ON_WM_PAINT()
	ON_WM_MOUSEACTIVATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CSpectrumGraphView �`��

/*============================================================================*/
/*! CSpectrumGraphView

-# �`�掞�ɌĂ΂��

@param  pDC�F�h�L�������g�̃f�o�C�X�R���e�L�X�g�|�C���^
@retval �Ȃ�
*/
/*============================================================================*/
void CSpectrumGraphView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
}


// CSpectrumGraphView �f�f

#ifdef _DEBUG
void CSpectrumGraphView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CSpectrumGraphView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSpectrumGraphView ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CSpectrumGraphView

-# �r���[����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
void CSpectrumGraphView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_gl_bk_color_b = BK_COLOR_B; /* �w�i */
	m_gl_glid_line_color_b = GRID_LINE_COLOR_B; /* �O���b�h�� */
	m_gl_Y_fig_color_b = Y_FIG_COLOR_B; /* Y������ */
	m_gl_X_fig_color_b = X_FIG_COLOR_B; /* X������*/
	m_gl_plot_color_b = PLOT_COLOR_B; /* �v���b�g */
	m_gl_peak_color_b = PEAK_COLOR_B; /* �s�[�N�}�[�N */
	m_gl_time_color_b = TIME_COLOR_B; /* �����\�� */

	m_gl_bk_color_w = BK_COLOR_W; /* �w�i */
	m_gl_glid_line_color_w = GRID_LINE_COLOR_W; /* �O���b�h�� */
	m_gl_Y_fig_color_w = Y_FIG_COLOR_W; /* Y������ */
	m_gl_X_fig_color_w = X_FIG_COLOR_W; /* X������*/
	m_gl_plot_color_w = PLOT_COLOR_W; /* �v���b�g */
	m_gl_peak_color_w = PEAK_COLOR_W; /* �s�[�N�}�[�N */
	m_gl_time_color_w = TIME_COLOR_W; /* �����\�� */

	m_gl_bk_color = m_gl_bk_color_b; /* �w�i */
	m_gl_glid_line_color = m_gl_glid_line_color_b; /* �O���b�h�� */
	m_gl_Y_fig_color = m_gl_Y_fig_color_b; /* Y������ */
	m_gl_X_fig_color = m_gl_X_fig_color_b; /* X������*/
	m_gl_plot_color = m_gl_plot_color_b; /* �v���b�g */
	m_gl_peak_color = m_gl_peak_color_b; /* �s�[�N�}�[�N */
	m_gl_time_color = m_gl_time_color_b; /* �����\�� */
	
	m_nGrid = eGridType_DOT;

	m_dAutoVMax = -DBL_MAX;
	m_dAutoVMin = DBL_MAX;
}

/*============================================================================*/
/*! CSpectrumGraphView

-# �O���b�h���̕`��

@param	memDC		�h�L�������g
@param	rectClient	�`��̈�
@retval	TRUE:�`�悵�� / FALSE:�`�悵�Ȃ�����

*/
/*============================================================================*/

BOOL CSpectrumGraphView::DrawGrid(CDC& memDC, CRect& rectClient)
{
	// �O���b�h���̕`��
	CPen penLine;
	CPen penSolid;
	penLine.CreatePen((m_nGrid == eGridType_DOT) ? PS_DOT : PS_SOLID, 1, m_gl_glid_line_color);
	penSolid.CreatePen(PS_SOLID, 1, m_gl_glid_line_color);
	CPen* oldPen = memDC.SelectObject(&penLine);
	memDC.SelectObject(&m_txtFont);

	memDC.SetBkMode(TRANSPARENT);

	memDC.SetTextColor(RGB(0, 0, 0));

	CRect rectLine, rectText;
	rectLine = rectClient;
	rectText = rectClient;

	if (m_nGraphType == eSpectrumGraphType_Normal)
	{
		rectLine.left += m_nVAxisWidth;
		rectLine.top += m_nVAxisHeightMargin;
		rectLine.bottom -= m_nVAxisHeightMargin;
		rectText.right = rectLine.left;
	}
	rectText.right = rectLine.left;

	int height = rectLine.Height();
	int width = rectLine.Width();


	// �c���`��
	double xPos;
	memDC.SelectObject(&penLine);
	double d_x_grid = ((double)GRID_MAX_X - (double)GRID_MIN_X) / (double)GRID_NUM_X;
	m_nHorzDiv = (int)((double)width / d_x_grid);

	if (theApp.GetSelectStationMode() == eStation_5)
	{
		m_nHorzDiv /= 2;
	}
	for (UINT n = 0; n <= (UINT)m_nHorzDiv; n++)
	{
		if (n == 0 || n == m_nHorzDiv)
			memDC.SelectObject(&penSolid);
		else
			memDC.SelectObject(&penLine);
		xPos = (double)n * width / m_nHorzDiv;;
		memDC.MoveTo(rectLine.left + (int)xPos, rectLine.top);
		memDC.LineTo(rectLine.left + (int)xPos, rectLine.bottom);
	}


	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return FALSE;
	}

#ifdef _ENGVALCNV_TREND
	if (pCSectrumData->GetRetSpectrumData() == ERR_RETURN)
	{
		// �f�[�^���擾���͈ȍ~��`�悵�Ȃ�
		memDC.SelectObject(oldPen);
		penLine.DeleteObject();
		penSolid.DeleteObject();
		return FALSE;
	}
#endif /* _ENGVALCNV_TREND */


	// �����`��
	double max;
	double min;
	double val;
	long vStep = pCSectrumData->GetVerticalGrid();
	double dRange;

	if (pCSectrumData->GetAutoScale())
	{
		// �ő�ŏ���T��
		for (int i = SPECTRUM_NUM_MIN; i < SPECTRUM_NUM; i++)
		{
			if (pCSectrumData->m_gda_spectrum_data[i] > SPECTRUM_VMAX)
			{
				val = SPECTRUM_VMAX;
			}
			else if (pCSectrumData->m_gda_spectrum_data[i] < SPECTRUM_VMIN)
			{
				val = SPECTRUM_VMIN;
			}
			else
			{
				val = pCSectrumData->m_gda_spectrum_data[i] + pCSectrumData->m_stVScale.gd_y_offset_val + pCSectrumData->m_gd_y_offset_att;
			}

			if (val > m_dAutoVMax)
			{
				m_dAutoVMax = val;
			}

			if (val < m_dAutoVMin)
			{
				m_dAutoVMin = val;
			}
		}

		max = m_dAutoVMax;
		min = m_dAutoVMin;
		// �}�[�W���v�Z
		double dRangeTmp = fabs(max - min);
		if (dRangeTmp == 0.0)
		{
			dRangeTmp = (double)pCSectrumData->GetVerticalGrid();
		}
		max = max + (dRangeTmp *  m_dVAxisMaxMinMargin);
		min = min - (dRangeTmp *  m_dVAxisMaxMinMargin);

		dRange = fabs(max - min);

		if (dRange < (double)vStep)
		{
			vStep = (long)dRange;
		}

		m_nVerDiv = (int)ceil(dRange / (double)vStep);
		if (m_nVerDiv > m_nVerDivMax)
		{
			m_nVerDiv = m_nVerDivMax;
			vStep = (int)ceil(dRange / (double)m_nVerDiv);
		}
	}
	else
	{
		max = (double)pCSectrumData->GetVerticalMax();
		min = (double)pCSectrumData->GetVerticalMin();

		dRange = fabs(max - min);
		if (dRange > m_dVRange)
		{
			// �c���͈̓I�[�o�[
			dRange = m_dVRange;
		}

		m_nVerDiv = (int)ceil(dRange / (double)vStep);
		if (m_nVerDiv > m_nVerDivMax)
		{
			m_nVerDiv = m_nVerDivMax;
			vStep = (int)ceil(dRange / (double)m_nVerDiv);
		}
	}

	m_dVMax = max;
	m_dVMin = min;

	double yPos;
	long vVal = (long)floor(max);
	double offsetVal = max - (double)vVal;
	double yLastPos = -1.0;

	if (vStep != 0 && dRange != 0)
	{
		// �����`��
		for (UINT n = 0; n <= (UINT)m_nVerDiv && vVal >= (long)min; n++, vVal -= vStep)
		{
			memDC.SelectObject(&penLine);
			yPos = ((double)(n * vStep) + offsetVal) * (double)height / dRange;
			if (yPos > height)
			{
				break;
			}

			memDC.MoveTo(rectLine.left, rectLine.top + (int)yPos);
			memDC.LineTo(rectLine.right, rectLine.top + (int)yPos);

			if (m_nGraphType == eSpectrumGraphType_Normal)
			{
				// �ڐ���`��
				CString str;
				CSize szStr;
				str.Format(_T("%d"), vVal);
				szStr = memDC.GetTextExtent(str);
				if (yLastPos < 0.0 || yLastPos + szStr.cy + 2 < yPos)
				{
					memDC.TextOutW(rectText.right - 4 - szStr.cx, rectLine.top + (int)yPos - szStr.cy / 2, str);
					yLastPos = yPos;
				}
			}
		}
	}

	memDC.SelectObject(oldPen);
	penLine.DeleteObject();
	penSolid.DeleteObject();

	return TRUE;
}


/*============================================================================*/
/*! CSpectrumGraphView

-# �X�y�N�g�����O���t�̕`��

@param	memDC		�h�L�������g
@param	rectClient	�`��̈�
@retval	TRUE�F�`�悵�� / FALSE:�`�悵�Ȃ�����

*/
/*============================================================================*/
int CSpectrumGraphView::DrawSpectrum(CDC& memDC, CRect& rectClient)
{
	long            l_x, l_y, l_x_peak, l_y_peak;
	double          d_low, d_upp, d_peak_freq, d_peak_level;
	int             i;

	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return FALSE;
	}

#ifdef _ENGVALCNV_TREND
	if (pCSectrumData->GetRetSpectrumData() == ERR_RETURN)
	{
		// �f�[�^���擾���͕`�悵�Ȃ�
		return FALSE;
	}
#endif /* _ENGVALCNV_TREND */

	HRGN	hRgn = CreateRectRgn(rectClient.left, rectClient.top, rectClient.right, rectClient.bottom);
	SelectClipRgn(memDC, hRgn);
	
	int width = rectClient.Width();
	int height = rectClient.Height();
	double	xStep = (double)width / (double)SPECTRUM_NUM;
	double	yRange = (double)height / (m_dVMax - m_dVMin);

	CPen penLine;
	CPen penPeak;
	penLine.CreatePen(PS_SOLID, 1, m_gl_plot_color);
	penPeak.CreatePen(PS_SOLID, 1, m_gl_peak_color);
	CPen* oldPen = memDC.SelectObject(&penLine);

	/* �X�y�N�g�����\�� */
	l_x_peak = -1;
	l_y_peak = -1;
	d_peak_freq = 0.0;
	d_peak_level = 0.0;

	d_low = 0.0;
	d_upp = 0.0;
	for (i = 0; spectrum_gast_bandw[i].sz_sts[0] != NULL; i++){
		if (spectrum_gast_bandw[i].sz_sts.Compare(pCSectrumData->m_gsz_bandw_sts) == 0) {
			d_low = pCSectrumData->m_gd_center_freq + spectrum_gast_bandw[i].d_low;
			d_upp = pCSectrumData->m_gd_center_freq + spectrum_gast_bandw[i].d_upp;
			break;
		}
	}

	for (i = SPECTRUM_NUM_MIN; i < SPECTRUM_NUM; i++){

		/* ���W�v�Z */
		l_x = (long)((double)rectClient.left + xStep * (double)i);
		l_y = (long)((double)rectClient.top + yRange
			* ((pCSectrumData->m_gda_spectrum_data[i] + pCSectrumData->m_stVScale.gd_y_offset_val + pCSectrumData->m_gd_y_offset_att - m_dVMax) * (-1.0)));

		if (l_y > rectClient.bottom){
			l_y = rectClient.bottom;
		}
		else if (l_y < 0){
			l_y = 0;
		}

		/* �v���b�g���[�v�P��ڈȊO�Ȃ�A���ŕ`�悷�� */
		if (i != 0) {
			memDC.LineTo(l_x, l_y);
		}
		else
		{
			memDC.MoveTo(l_x, l_y);
		}

		/* �s�[�N���W�ۑ� */
		if (pCSectrumData->m_gda_spectrum_data[i] == pCSectrumData->m_gd_spectrum_peak){
			/* �s�[�N�l�擾 */
			if ((d_low != 0.0) && (d_upp != 0.0)){
				l_x_peak = l_x;
				l_y_peak = l_y;
				d_peak_freq = d_low + ((d_upp - d_low) / (double)SPECTRUM_NUM) * (double)i;
				d_peak_level = pCSectrumData->m_gda_spectrum_data[i] + pCSectrumData->m_stVScale.gd_y_offset_val + pCSectrumData->m_gd_y_offset_att;
				pCSectrumData->m_gd_peak_level = d_peak_level;
			}
		}
	}
	/* �s�[�N�`�� */
	if ((l_x_peak != -1) && (l_y_peak != -1)){
		l_x = l_x_peak;
		l_y = l_y_peak;
		memDC.SelectObject(&penPeak);
		memDC.MoveTo((l_x + 5), l_y); memDC.LineTo(l_x, (l_y - 10));
		memDC.MoveTo(l_x, (l_y - 10)); memDC.LineTo((l_x - 5), l_y);
		memDC.MoveTo((l_x - 5), l_y); memDC.LineTo(l_x, (l_y + 10));
		memDC.MoveTo(l_x, (l_y + 10)); memDC.LineTo((l_x + 5), l_y);
		pCSectrumData->m_gd_peak_freq = d_peak_freq;
	}

	memDC.SelectObject(oldPen);
	penLine.DeleteObject();
	penPeak.DeleteObject();
	DeleteObject(hRgn);

	return (0);
}


/*============================================================================*/
/*! CSpectrumGraphView

-# �X�y�N�g�����ݒ�f�[�^�擾

@param	�Ȃ�
@retval	�X�y�N�g�����ݒ�f�[�^

*/
/*============================================================================*/
CSpectrumViewData* CSpectrumGraphView::GetSpectrumViewData()
{
	CSpectrumViewData* pCSectrumData;
	if (m_nGraphType == eSpectrumGraphType_Normal)
	{
		pCSectrumData = &theApp.GetSatelliteData().GetSpectrumViewData();
	}
	else
	{
		pCSectrumData = &theApp.GetSatelliteData().GetSummarySpectrumViewData(m_nStationIdx, m_nViewIdx);
	}

	return pCSectrumData;
}


/*============================================================================*/
/*! CSpectrumGraphView

-# �O���t�E�B���h�E�̕`��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumGraphView::OnPaint()
{
	CRect rectClient, rectGraph;
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
	// �f�o�C�X �R���e�L�X�g�̃r���[�|�[�g�̌��_��ݒ�
	memDC.SetViewportOrg(0, 0);


	// �w�i��h��Ԃ�
	COLORREF colVAxisBk = GetSysColor(COLOR_3DFACE);
	memDC.SetBkColor(colVAxisBk);
	CBrush brushVAxis(colVAxisBk);
	memDC.FillRect(rectClient, &brushVAxis);

	// �O���t�w�i��h��Ԃ�
	rectGraph = rectClient;

	if (m_nGraphType == eSpectrumGraphType_Normal)
	{
		rectGraph.left += m_nVAxisWidth;
		rectGraph.top += m_nVAxisHeightMargin;
		rectGraph.bottom -= m_nVAxisHeightMargin;
	}
	memDC.SetBkColor(m_gl_bk_color);
	CBrush brush(m_gl_bk_color);
	memDC.FillRect(rectGraph, &brush);
	
	UpdateData(TRUE);

	// ���̕`��
	if (DrawGrid(memDC, rectClient) == TRUE)
	{
		// �O���t�f�[�^�`��
		(void)DrawSpectrum(memDC, rectGraph);
	}

	dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	bitm.DeleteObject();
	memDC.DeleteDC();
	brushVAxis.DeleteObject();
	brush.DeleteObject();
}


/*============================================================================*/
/*! CSpectrumGraphView

-# ��A�N�e�B�u �E�B���h�E�Ƀ��[�U�[���}�E�X �{�^�����������Ƃ��ɌĂяo�����

@param  pDesktopWnd�F�A�N�e�B�u�ɂȂ�E�B���h�E�̃g�b�v���x���̐e�E�B���h�E�ւ̃|�C���^
@param	nHitTest�F�J�[�\���ʒu�e�X�g�̗̈�R�[�h
@param	message�F�}�E�X���b�Z�[�W�ԍ�
@retval �E�B���h�E�A�N�e�B�u/�j������
*/
/*============================================================================*/
int CSpectrumGraphView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	return 0;
//	return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


/*============================================================================*/
/*! CSpectrumGraphView

-# �w�i�������ɌĂ΂��

@param  pDC�F�h�L�������g�̃f�o�C�X�R���e�L�X�g�|�C���^
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CSpectrumGraphView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

//	return CView::OnEraseBkgnd(pDC);
}

