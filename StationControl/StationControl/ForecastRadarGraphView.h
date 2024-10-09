#pragma once

#include "SatelliteData.h"

#define		ON	1
#define		OFF	0

// CForecastRadarGraphView �r���[

class CForecastRadarGraphView : public CView
{
	DECLARE_DYNCREATE(CForecastRadarGraphView)

protected:
	CForecastRadarGraphView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CForecastRadarGraphView();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	enum eDrawType
	{
		eDrawType_Before = 0,
		eDrawType_After
	};

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CTrackingRadarCtrlMonData	m_ctrlMonData;		// �^�p�󋵊Ď��f�[�^

	CPoint		m_ptCenter;			// �~�̒��S
	double		m_r;				// �~�̔��a
	int			m_i_plot_flag;		// �`�n�r�F�ł̃v���b�g���{�t���O

	int			m_i_into_flag = OFF;	/* �p�X�J�n�R�O���ȏ�O����R�O���ȓ��ɓ��������ǂ����̃t���O */
	long		m_l_latest_time = UNYOU_TIME + 1;	/* �O��̃J�E���g�_�E���l */

	int			m_nViewType;		// �\���^�C�v		
	int			m_nStationID;		// ��ID
	int			m_nCtrlMonDataID;	// �f�[�^ID

	int			m_dummyCnt;			// �_�~�[�J�E���^
//	time_t		m_dummyInitTime;	// �_�~�[��������

	CFont		m_txtFontSmall;		// �e�L�X�g�t�H���g�i�X���[���j
	CFont		m_txtFontNormal;	// �e�L�X�g�t�H���g�i�m�[�}���j

	CString		m_strPlanID;		// �v��ID
	__int64		m_tPassStart;		// �^�p�J�n����
	__int64		m_tPassEnd;			// �^�p�I������

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B

	void SetViewtype(int nViewType)			{ m_nViewType = nViewType; }
	void SetStationID(int nStationID)		{ m_nStationID = nStationID; }
	void Init(CString fpath, CString satellite, CString predId);
	void InitGtunyouData(CString fpath, CString& satellite, CString& predId);
	void InitDummyGtunyouData(CString fpath); 
	void SetPlanID(CString strPlanID, __int64 tPassStart, __int64 tPassEnd)		{ m_strPlanID = strPlanID; m_tPassStart = tPassStart; m_tPassEnd = tPassEnd; }

protected:
	BOOL DrawRadarFrame(CDC& memDC, CRect& rectClient);
	BOOL DrawRadarFrame(CDC& memDC, CRect& rectClient, int drawType);
	BOOL DrawSkyline(CDC& memDC, CRect& rectClient);
	BOOL DrawRfTrip(CDC& memDC, CRect& rectClient);
	BOOL DrawForecast(CDC& memDC, CRect& rectClient);
	BOOL DrawTrakingSaveData(CDC& memDC, CRect& rectClient);
	void GtazelToXY(double d_Az, double d_El, double d_CX, double d_CY, double d_RD, int* i_OX, int* i_OY);

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

protected:
	void AddDummyData();
	void UpdateGraphData();

public:
	virtual void OnInitialUpdate();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	/* ------------------------------------------------------------------------------------ */
};


