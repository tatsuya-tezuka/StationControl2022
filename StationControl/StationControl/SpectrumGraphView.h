#pragma once

/*******************************************************************************
* �f�[�^�v���b�g�֘A��`
*******************************************************************************/
#define GRID_MIN_X  (  60 )
#define GRID_MIN_Y  (  15 )
#define GRID_MAX_X  ( rectLine.Width() + GRID_MIN_X )
#define GRID_MAX_Y  ( rectLine.Height() + GRID_MIN_Y )
#define GRID_NUM_X  ( 10 )
#define GRID_NUM_Y  ( 16 )

// CSpectrumGraphView �r���[

class CSpectrumGraphView : public CView
{
	DECLARE_DYNCREATE(CSpectrumGraphView)

public:
	CSpectrumGraphView();
	virtual ~CSpectrumGraphView();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	enum eGridType{
		eGridType_DOT,
		eGridType_LINE,
		eGridType_NONE
	};

	static const int m_nVAxisWidth = 30;
	static const int m_nVAxisHeightMargin = 10;
	double m_dVAxisMaxMinMargin = 0.05;
	double m_dVRange = 1000.0;
	static const int m_nVerDivMax = 25;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	COLORREF    m_gl_bk_color;				/* �w�i */
	COLORREF    m_gl_glid_line_color;		/* �O���b�h�� */
	COLORREF    m_gl_Y_fig_color;			/* Y������ */
	COLORREF    m_gl_X_fig_color;			/* X������*/
	COLORREF    m_gl_plot_color;			/* �v���b�g */
	COLORREF    m_gl_peak_color;			/* �s�[�N�}�[�N */
	COLORREF    m_gl_time_color;			/* �����\�� */

	COLORREF    m_gl_bk_color_b;			/* �w�i */
	COLORREF    m_gl_glid_line_color_b;		/* �O���b�h�� */
	COLORREF    m_gl_Y_fig_color_b;			/* Y������ */
	COLORREF    m_gl_X_fig_color_b;			/* X������*/
	COLORREF    m_gl_plot_color_b;			/* �v���b�g */
	COLORREF    m_gl_peak_color_b;			/* �s�[�N�}�[�N */
	COLORREF    m_gl_time_color_b;			/* �����\�� */

	COLORREF    m_gl_bk_color_w;			/* �w�i */
	COLORREF    m_gl_glid_line_color_w;		/* �O���b�h�� */
	COLORREF    m_gl_Y_fig_color_w;			/* Y������ */
	COLORREF    m_gl_X_fig_color_w;			/* X������*/
	COLORREF    m_gl_plot_color_w;			/* �v���b�g */
	COLORREF    m_gl_peak_color_w;			/* �s�[�N�}�[�N */
	COLORREF    m_gl_time_color_w;			/* �����\�� */

protected:
	UINT		m_nHorzDiv;			// ��������
	UINT		m_nVerDiv;			// �c������
	int			m_nGrid;			// �O���b�h���

	long		m_gl_x_grid;		/* �O���b�h�ԊuX */
	long		m_gl_y_grid;		/* �O���b�h�ԊuY */

	CFont		m_txtFont;			// �e�L�X�g�t�H���g
	double		m_dAutoVMax;		// �I�[�g�X�P�[���c���ő�l
	double		m_dAutoVMin;		// �I�[�g�X�P�[���c���ŏ��l
	double		m_dVMax;			// �c���ő�l
	double		m_dVMin;			// �c���ŏ��l

	UINT		m_nGraphType;		// �O���t�^�C�v
	UINT		m_nStationIdx;		// �ǔԍ�
	UINT		m_nViewIdx;			// �r���[�C���f�b�N�X

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	void InitializeView(UINT nStationIdx, UINT nViewIdx, UINT nGraphType)	{
		m_nStationIdx = nStationIdx;
		m_nViewIdx = nViewIdx;
		m_nGraphType = nGraphType;
	}
	void SetStationIdx(UINT nStationIdx)	{ m_nStationIdx = nStationIdx; }
	void SetViewIdx(UINT nViewIdx)			{ m_nViewIdx = nViewIdx; }
	void SetGraphType(UINT nGraphType)		{ m_nGraphType = nGraphType; }
	void ClearAutoVMaxMin()
	{
		m_dAutoVMax = -DBL_MAX;
		m_dAutoVMin = DBL_MAX;
	}

public:
	CSpectrumViewData* GetSpectrumViewData();
	UINT GetGraphType()			{ return m_nGraphType; }

protected:
	BOOL DrawGrid(CDC& memDC, CRect& rectClient);
	BOOL DrawSpectrum(CDC& memDC, CRect& rectClient);

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnPaint();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	/* ------------------------------------------------------------------------------------ */
};


