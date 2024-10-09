/*============================================================================*/
/*! CGraphWindow.cpp

-# �q���ߑ����
*/
/*============================================================================*/
#include "stdafx.h"
#include "StationControl.h"
#include "GraphWindow.h"
#include "TrendPropertiesDlg.h"
#include "TrendGraphView.h"
#include "IntgTimeDlg.h"

#include <math.h>
#include <float.h>

#define ID_SETTINGBTN		(WM_USER+8100)
#define ID_HGRIDONCHKBTN	(WM_USER+8101)
#define ID_SIGONCHKBTN		(WM_USER+8102)
#define ID_INTGBTN			(ID_SIGONCHKBTN + MAX_TDATA)

#define _GRAPHDRAW_KAIZEN 1

UINT CGraphWindow::mHScale[] = {
	60,
	300,
	600,
	1200,
	1800,
	3600,
	7200,
	14400,
	28800,
	43200,
	57600,
	72000,
	86400,
	60,
	60,
};

/*============================================================================*/
/*! CGraphWindow

-# �R���X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CGraphWindow::CGraphWindow()
: m_pointTop(0)
, m_dbMaxHzVal(1000)
, m_dbMinHzVal(0)
, m_dbHzRange(100)
, m_dbTmpHzRange(m_dbHzRange)
, m_nHorzDiv(mHDivNormal)
, m_DrawLines()
, m_ColorBk(RGB(236, 233, 216))
, m_ColorGrid(RGB(0, 0, 0))
, m_ColorText(RGB(0, 0, 0))
, m_ColorLighttGrid(RGB(128, 128, 128))
, m_nGraphType(eGraphType_Trend)
, m_nGrid(eGridType_NONE)
, m_nTrendInc(1)
, m_CursorPos(0)
, m_nTrandHScale(0)
, m_dTotalScaleLMax(-DBL_MAX)
, m_dTotalScaleLMin(DBL_MAX)
, m_dGridScaleLMax(0.0)
, m_dGridScaleLMin(0.0)
, m_nTrendDataPos(0)
, m_bViewDetail(TRUE)
, m_bFitYohochi(FALSE)
, m_bExitData(FALSE)
, m_bStopDraw(FALSE)
, m_StartTime(0)
, m_EndTime(0)
{
	// ������
	m_pParentWnd = NULL;
	m_nViewNo = 0;

	CFont defFont;
	CFont* oldFont = NULL;
	defFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (defFont.GetLogFont(&lf)){
		lf.lfWeight = FW_NORMAL;
		lf.lfHeight = 12;
		m_txtFontNormal.CreateFontIndirect(&lf);

//		lf.lfHeight = -10;
		m_txtFontSmall.CreateFontIndirect(&lf);
	}

	ClearCursor();
}

/*============================================================================*/
/*! CGraphWindow

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CGraphWindow::~CGraphWindow()
{
	m_txtFontSmall.DeleteObject();
	m_txtFontNormal.DeleteObject();
	
	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL)
		delete m_objects.GetNext(pos);

}

BEGIN_MESSAGE_MAP(CGraphWindow, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(ID_SETTINGBTN, &CGraphWindow::OnBnClickedSettingBtn)
	ON_BN_CLICKED(ID_HGRIDONCHKBTN, &CGraphWindow::OnBnClickedHGridOnBtn)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 0, &CGraphWindow::OnBnClickedSigOnBtn0)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 1, &CGraphWindow::OnBnClickedSigOnBtn1)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 2, &CGraphWindow::OnBnClickedSigOnBtn2)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 3, &CGraphWindow::OnBnClickedSigOnBtn3)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 4, &CGraphWindow::OnBnClickedSigOnBtn4)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 5, &CGraphWindow::OnBnClickedSigOnBtn5)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 6, &CGraphWindow::OnBnClickedSigOnBtn6)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 7, &CGraphWindow::OnBnClickedSigOnBtn7)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 8, &CGraphWindow::OnBnClickedSigOnBtn8)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 9, &CGraphWindow::OnBnClickedSigOnBtn9)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 10, &CGraphWindow::OnBnClickedSigOnBtn10)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 11, &CGraphWindow::OnBnClickedSigOnBtn11)
	ON_BN_CLICKED(ID_SIGONCHKBTN + 12, &CGraphWindow::OnBnClickedSigOnBtn12)
	ON_BN_CLICKED(ID_INTGBTN, &CGraphWindow::OnBnClickedIntgTimeBtn)
END_MESSAGE_MAP()


/*============================================================================*/
/*! �O���t�`��

-# �O���t�E�B���h�E�̍쐬

@param	dwStyle		�E�B���h�E�X�^�C��
@param	rect		�E�B���h�E�f�t�H���g�̈�
@param	pParentWnd	�e�E�B���h�E
@param	nID			�E�B���h�E���ʎq
@param	bVisibleSettingBtn	�ݒ�{�^���\��
@retval

*/
/*============================================================================*/
BOOL CGraphWindow::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, BOOL bVisibleSettingBtn /*= FALSE*/)
{
	BOOL result = FALSE;
	m_pParentWnd = pParentWnd;

	dwStyle |= WS_CHILD;
	if (m_nGraphType == eGraphType_OffTrend){
		dwStyle |= WS_HSCROLL;
	}

	// �E�B���h�E�̍쐬
	result = CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID, NULL);
	// ������
	m_nGrid = eGridType_NONE;	// �O���b�h�L��
	m_bLabel = FALSE;			// ���x���\��
	m_dbMaxHzVal = m_dbHzRange;
	if (bVisibleSettingBtn || theApp.GetSelectStationMode() == eStation_1)
		m_nHorzDiv = mHDivNormal;			// ������
	else
		m_nHorzDiv = mHDivSmall;			// ������

	m_bUseVAxis[LScale] = TRUE;			// �[���g�p
	m_bUseVAxis[RScale] = TRUE;			// �[���g�p
	m_nStvScale.Auto = TRUE;			// �[���̎����X�P�[��
	m_nStvScale.VScalLMax = 50.0;		// ���[���̍ő�l
	m_nStvScale.VScalLMin = -50.0;		// ���[���̍ŏ��l
	m_nStvScale.VScalRMax = 50.0;		// �E�[���̍ő�l
	m_nStvScale.VScalRMin = -50.0;		// �E�[���̍ŏ��l
	m_nStvScale.VSCalItvl = 5;			// �[��������

	if (m_nGraphType == eGraphType_OffTrend)
	{
		UpdateScrollInfo();
	}

	m_nTrendDataPos = 0;				// �g�����h�̊J�n��������̎��Ԉʒu

//	SetHzRange(mHScale[m1]);			// �������r���[�P�f�t�H���g
	SetHzRange(mHScale[m1] + 1);			// �������r���[�P�f�t�H���g

	m_nTrandHScale = 0;					// �c���X�P�[���̎��
	m_dTotalScaleLMax = -DBL_MAX;		// ���c���X�P�[�����
	m_dTotalScaleLMin = DBL_MAX;		// ���c���X�P�[������

	InitSatelliteDataSetting();

	// ���r���\��ON
	m_bHGridOn = TRUE;

	// �ݒ�{�^���\���̏ꍇ�́A�ʏ�̉q���ߑ��O���t
	if (bVisibleSettingBtn)
	{
		// �ݒ�{�^���\��
		CRect rcBtn = rect;
		rcBtn.right -= mBtnSettingMarginRight;
		rcBtn.left = rcBtn.right - mBtnWidth;
		rcBtn.top += mBtnSettingMarginTop;
		rcBtn.bottom = rcBtn.top + mBtnHeight;

		CString strBtn;
		if (strBtn.LoadStringW(IDS_TREND_SETTINGBTN) == FALSE)
			return FALSE;
		m_btnSetting.Create(strBtn, BS_PUSHBUTTON | WS_VISIBLE, rcBtn, this, ID_SETTINGBTN);
		m_btnSetting.SetFont(&m_txtFontNormal);

		// ���r���\��ON/OFF�`�F�b�N�{�b�N�X�\��
		rcBtn.left = rcBtn.right - mGridMarginRight + mChkBtnMarginRight;
		rcBtn.right = rcBtn.left + mChkBtnWidth;
		rcBtn.top += mBtnSettingMarginTop;
		rcBtn.bottom = rcBtn.top + mChkBtnHeight;
		strBtn;
		if (strBtn.LoadStringW(IDS_TREND_HGRID_ON) == FALSE)
			return FALSE;
		m_btnHGridChk.Create(strBtn, BS_CHECKBOX | BS_AUTOCHECKBOX | WS_VISIBLE, rcBtn, this, ID_HGRIDONCHKBTN);
		m_btnHGridChk.SetFont(&m_txtFontNormal);
		m_btnHGridChk.SetCheck(1);

		CRect rectGraphArea;
		rectGraphArea.SetRect(
			rect.left,
			rect.top + mGridMarginTop,
			rect.right,
			rect.bottom - mGridMarginBottom + 1);

		// �M���\���`�F�b�N�{�^���\��
		for (int i = 0; i < MAX_TDATA; i++)
		{
			CRect rcSigBtn = rectGraphArea;
			CString strSigBtn;
			CPoint ptSigName;
			ptSigName.x = rect.right - mGridMarginRight + mRightMargin + mLegendWeidth + mLegendLineStrDistance;
			ptSigName.y = rect.top + mTopMargin + mSigStrHeight * i;
			rcSigBtn.left = ptSigName.x;
			rcSigBtn.right = rcSigBtn.left + mChkBtnWidth - mHStrDistance;
			rcSigBtn.top = ptSigName.y;
			rcSigBtn.bottom = rcSigBtn.top + 12;

			if (m_DrawLines.size() > i)
			{
				strSigBtn = m_DrawLines[i].strGName;			
			}

			if (m_StationIndex == eStation_Usuda642)
			{
				// 54m
				m_txtSigName[i].Create(strSigBtn, SS_LEFTNOWORDWRAP, rcSigBtn, this, ID_SIGONCHKBTN + i);
				m_txtSigName[i].SetFont(&m_txtFontNormal);
			}
			else
			{
				// 54m�ȊO
				m_chkBtnViewSig[i].Create(strSigBtn, BS_CHECKBOX | BS_AUTOCHECKBOX, rcSigBtn, this, ID_SIGONCHKBTN + i);
				m_chkBtnViewSig[i].SetFont(&m_txtFontNormal);
				m_chkBtnViewSig[i].SetCheck(1);
			}
		}

	}
	// �T�}���O���t
	else
	{
		// �w�i�F�����F�ɕύX
		m_ColorBk = RGB(0, 0, 0);
		m_ColorGrid = RGB(128, 128, 128);
		m_ColorText = RGB(128, 128, 128);
		m_ColorLighttGrid = RGB(128, 128, 128);
	}

//	m_nIntgTime = 300;
	m_nIntgTime = -1;
	m_bExitData = FALSE;
	m_StartTime = 0;
	m_EndTime = 0;

	UpdateIntegKanshi();

	m_rectCurValArea = CRect(0, 0, 0, 0);

	ResetCursor(-1);

	AjustLayout();

	return result;
}


/*============================================================================*/
/*! �O���t�`��

-# �Ď�����ϕ����Ԃ̎擾�X�V

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CGraphWindow::UpdateIntegKanshi()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT sta = theApp.GetSelectStation();

	int nIntgTime;

	CString strKanshi;
	UINT nTermNo = theApp.GetTermNo();
	if (nTermNo > 0)
		nTermNo = nTermNo - 1;

	int nAreaNo = theApp.GetClientTerminal();
//	strKanshi.Format(_T("%s%02u"), NOTIFY_INTEG_MSG, nTermNo);
	strKanshi.Format(_T("%s%s%02u"), NOTIFY_INTEG_MSG, mAreaNameString[nAreaNo], nTermNo);
	CString str;
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, strKanshi);
	if (obs_adr == NULL)
	{
		nIntgTime = 0;
		str.Format(_T(""));
	}
	else if (obs_adr->l_inflg == INFLG_ON)
	{
		nIntgTime = (int)obs_adr->d_data;
		str.Format(_T("%d"), nIntgTime);
	}
	else
	{
		nIntgTime = 0;
		str.Format(_T(""));
	}

	if (m_nIntgTime != nIntgTime && m_btnIntg.GetSafeHwnd() != NULL)
	{
		m_nIntgTime = nIntgTime;
		m_btnIntg.SetWindowTextW(str);
		m_btnIntg.UpdateData(FALSE);
	}
}


/*============================================================================*/
/*! �O���t�`��

-# �O���t�E�B���h�E�̕`��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CGraphWindow::OnPaint()
{
	CRect rectClient;
	CRect rectGrid;
	CPaintDC dc(this);

//#ifdef _DEBUG
//	{
//		CString str;
//		str.Format(_T("%s(%d) :: Draw Graph GraphType = %u, ViewID = %u, "), __FILEW__, __LINE__, m_nGraphType, m_nPropertiesID);
//		CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("OnPaint"), _T("debug"), str, nLogEx::debug);
//	}
//#endif

	// �N���C�A���g�̈�̎擾
	GetClientRect(rectClient);

	CDC memDC;
	CBitmap bitm;
	// �`��c�b�̍쐬
	memDC.CreateCompatibleDC(&dc);

	// �`��p�r�b�g�}�b�v�̍쐬
	bitm.CreateCompatibleBitmap(&dc, rectClient.Width(), rectClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitm);

	// �o�b�N�O���E���h ���[�h��ݒ�
	memDC.SetBkMode(OPAQUE);
	// �w�i�F�̐ݒ�
	//	memDC.SetBkColor(m_ColorBk);
	COLORREF colBk = GetSysColor(COLOR_3DFACE);
	memDC.SetBkColor(colBk);
	// �f�o�C�X �R���e�L�X�g�̃r���[�|�[�g�̌��_��ݒ�
	memDC.SetViewportOrg(0, 0);

	memDC.SelectObject(&m_txtFontNormal);

	CBrush brush(colBk);
	// �S�̂�h��Ԃ�
	memDC.FillRect(rectClient, &brush);

	// �ڐ����\������̈悪�m�ۂł��邩���ׂ�
	m_bViewDetail = IsViewDetail(rectClient);

	if (m_bViewDetail)
	{
		// �ڍו\�����̎��̈�
		rectGrid.SetRect(
			rectClient.left + mGridMarginLeft,
			rectClient.top + mGridMarginTop,
			rectClient.right - mGridMarginRight,
			rectClient.bottom - mGridMarginBottom);
		// ���O���b�h���ڍו\��
		m_nHorzDiv = mHDivNormal;
	}
	else
	{
		// �ȈՕ\�����̎��̈�
		rectGrid = rectClient;
		if (theApp.GetSelectStationMode() == eStation_5)
		{
			// ���O���b�h���������̈�p�\��
			m_nHorzDiv = mHDivSmall;
		}
	}

	if (m_nGrid != eGridType_NONE)
	{
		if (m_nGraphType != eGraphType_Summary){
			DrawTrendName(memDC, rectClient);
		}
		if (DrawGrid(memDC, rectGrid) == TRUE)
		{
			// �O���t���̕`��
			if (!m_DrawLines.empty()){
				DrawGraph(memDC);
			}
		}
	}

	dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	bitm.DeleteObject(); // ADD 2011.08.25
	memDC.DeleteDC(); // ADD 2011.08.25
	brush.DeleteObject();
}

/*============================================================================*/
/*! �O���t�`��

-# �_���P�ʂ��f�o�C�X�P�ʂɕϊ�

@param X X�l
@param Y Y�l
@retval

*/
/*============================================================================*/
CPoint CGraphWindow::ConvPoint(double X, double Y, double max, double min)
{
	int nY;
	int nX;
	CRect rectClient;
	GetClientRect(rectClient);

	if (m_bViewDetail)
	{
		// ���̈�Ɠ����ɃT�C�Y�ύX
		rectClient.SetRect(
			rectClient.left + mGridMarginLeft,
			rectClient.top + mGridMarginTop,
			rectClient.right - mGridMarginRight,
			rectClient.bottom - mGridMarginBottom);
	}

	// �g�����h���ꏈ���i�ő偁�ŏ����l�j
	if (m_nGraphType == eGraphType_Trend || m_nGraphType == eGraphType_Summary){
		if (max == min && max == Y){
			if (m_bViewDetail)
				nX = (int)((rectClient.Width() * (X - m_pointTop)) / (m_dbHzRange - 1.0) + mGridMarginLeft);
			else
				nX = (int)((rectClient.Width() * (X - m_pointTop)) / (m_dbHzRange - 1.0));

			return CPoint(nX, rectClient.top + rectClient.Height() / 2);
		}
	}
	nY = (int)((rectClient.Height() * (Y - min)) / (max - min) + 0.5);
	if (m_bViewDetail)
		nX = (int)((rectClient.Width() * (X - m_pointTop)) / (m_dbHzRange - 1.0) + mGridMarginLeft);
	else
		nX = (int)((rectClient.Width() * (X - m_pointTop)) / (m_dbHzRange - 1.0));

	if (m_bViewDetail)
		return CPoint(nX, rectClient.Height() - nY + mGridMarginTop);
	else
		return CPoint(nX, rectClient.Height() - nY);
}
CPoint CGraphWindow::ConvPoint(double X)
{
	int nX;
	CRect rectClient;
	GetClientRect(rectClient);

	if (m_bViewDetail)
	{
		// ���̈�Ɠ����ɃT�C�Y�ύX
		rectClient.SetRect(
			rectClient.left + mGridMarginLeft,
			rectClient.top + mGridMarginTop,
			rectClient.right - mGridMarginRight,
			rectClient.bottom - mGridMarginBottom);
	}

	switch (m_nGraphType)
	{
	case eGraphType_All:
	case eGraphType_OffAll:
		if (m_bViewDetail)
		{
			nX = (int)((rectClient.Width() * (X - m_pointTop)) / m_dbTmpHzRange + mGridMarginLeft);
		}
		else
		{
			nX = (int)((rectClient.Width() * (X - m_pointTop)) / m_dbTmpHzRange);
		}
		break;
	case eGraphType_Trend:
	case eGraphType_OffTrend:
	case eGraphType_Summary:
	default:
		if (m_bViewDetail)
		{
			nX = (int)((rectClient.Width() * (X - m_pointTop)) / m_dbTmpHzRange + mGridMarginLeft);
		}
		else
		{
			nX = (int)((rectClient.Width() * (X - m_pointTop)) / m_dbTmpHzRange);
		}
		break;
	}
	return CPoint(nX, rectClient.Height());
}


/*============================================================================*/
/*! �O���t�`��

-# �g�����h�O���t���̕`��

@param	memDC		�h�L�������g
@param	RectClient	�`��̈�
@retval	TRUE:�g�����h�O���t����`�悵�� / FALSE:�g�����h�O���t����`�悵�Ȃ�����

*/
/*============================================================================*/
BOOL CGraphWindow::DrawTrendName(CDC& memDC, CRect& RectClient)
{
	// �g�����h�O���t�r���[���̕\��
	switch (m_nPropertiesID)
	{
	case eTrendWndType_AGC:
		if (m_strViewName.LoadString(IDS_TREND_AGCGRAPH) == FALSE)
			return FALSE;
		break;
	case eTrendWndType_RNG:
		if (m_strViewName.LoadString(IDS_TREND_RNGGRAPH) == FALSE)
			return FALSE;
		break;
	case eTrendWndType_RR:
		if (m_strViewName.LoadString(IDS_TREND_RRGRAPH) == FALSE)
			return FALSE;
		break;
	case eTrendWndType_EL:
		if (m_strViewName.LoadString(IDS_TREND_ELGRAPH) == FALSE)
			return FALSE;
		break;
	default:
		break;
	}

	memDC.SetTextColor(m_ColorGrid);
	memDC.SetBkMode(TRANSPARENT);
	CSize szStr;
	szStr = memDC.GetTextExtent(m_strViewName);
	memDC.TextOutW(RectClient.left + RectClient.Width() / 2 - szStr.cx / 2, RectClient.top + mViewWindowIDMarginTop, m_strViewName);

	if (m_nPropertiesID > eTrendWndType_AGC)
	{
		// �E�B���h�E�d�ؐ��̕`��
		CPen penSolid;
		penSolid.CreatePen(PS_SOLID, 1, m_ColorGrid);
		CPen* oldPen = memDC.SelectObject(&penSolid);

		memDC.MoveTo(RectClient.left, RectClient.top);
		memDC.LineTo(RectClient.right, RectClient.top);

		memDC.SelectObject(oldPen);
		penSolid.DeleteObject();
	}

	return TRUE;
}


/*============================================================================*/
/*! �O���t�`��

-# �O���b�h���̕`��

@param	memDC		�h�L�������g
@param	RectClient	�`��̈�
@retval	TRUE:�O���b�h����`�悵�� / FALSE:�O���b�h����`�悵�Ȃ�����

*/
/*============================================================================*/
BOOL CGraphWindow::DrawGrid(CDC& memDC, CRect& RectClient)
{
	// �O���b�h���̕`��
	CPen penLine;
	CPen penSolid;
	penLine.CreatePen((m_nGrid == eGridType_DOT) ? PS_DOT : PS_SOLID, 1, m_ColorGrid);
	penSolid.CreatePen(PS_SOLID, 1, m_ColorGrid);
	CPen* oldPen = memDC.SelectObject(&penLine);
	CFont* oldFont = memDC.SelectObject(&m_txtFontSmall);

	// �O���t������h��Ԃ�
	memDC.SetBkColor(m_ColorBk);
	CBrush brush(m_ColorBk);
	memDC.FillRect(RectClient, &brush);
	brush.DeleteObject();
	
	// �c���̎���\������̈悪�m�ۂł��邩���ׂ�
	int height = RectClient.Height();
	double oldYPos = -1.0;
	BOOL bRangeValid = TRUE;
	for (UINT n = 0; n <= (UINT)m_nStvScale.VSCalItvl; n++)
	{
		double yPos = (double)n * (height - 1) / m_nStvScale.VSCalItvl;
		if (yPos < oldYPos)
		{
			bRangeValid = FALSE;
		}
		oldYPos = yPos;
	}
	int nOldInipoint = -1;
	double dEndN = m_pointTop + m_dbHzRange + (m_dbHzRange / (double)m_nHorzDiv / 2);
	if (bRangeValid == TRUE)
	{
		for (double n1 = 0.0, n2 = 0.0;
			n1 < dEndN;
			n1 += (m_dbHzRange / (double)m_nHorzDiv), n2 += (m_dbTmpHzRange / (double)m_nHorzDiv))
		{

			int nInipoint = ConvPoint((double)n2).x;
			if (nInipoint < nOldInipoint)
			{
				bRangeValid = FALSE;
			}
			nOldInipoint = nInipoint;
		}
	}

	memDC.SetTextColor(m_ColorGrid);
	memDC.SetBkMode(TRANSPARENT);

	int nDecimalKeta = 0;

// test >>>
//	if (m_nPropertiesID == eTrendWndType_RNG)
//	{
//		int c = 0;
//	}
// test <<<


	if (m_nStvScale.VSCalItvl > 0 && bRangeValid == TRUE)
	{
		// �����`��
		double dVLRange;
		double dVLPos;
		double dVRRange;
		double dVRPos;
		double dVLStep;
		double dVRStep;
//++ 1�h�b�g���ꏈ�u >>>
		double dVLMin;
//++ 1�h�b�g���ꏈ�u <<<

		if (m_nStvScale.Auto)
		{
			double range;
#ifndef _GRAPHDRAW_KAIZEN			
			range = fabs(m_dTotalScaleLMax - m_dTotalScaleLMin);
			if (range == 0.0)
				range = fabs(m_dTotalScaleLMax);
			if (range == 0.0)
				range = 100.0;
			m_dGridScaleLMax = Ceil3Keta(m_dTotalScaleLMax + range * (double)AUTO_V_TOPBOTMARGIN, 1);
			m_dGridScaleLMin = Ceil3Keta(m_dTotalScaleLMin - range * (double)AUTO_V_TOPBOTMARGIN, 0);
#else
			// �ő�ŏ��������l�łȂ��ꍇ
			if (m_dTotalScaleLMax != -DBL_MAX && m_dTotalScaleLMin != DBL_MAX)
			{
				// �͈͒l���Z�b�g
				range = fabs(m_dTotalScaleLMax - m_dTotalScaleLMin);
			}
			else
			{
				// �f�t�H���g�Ŕ͈͒l100
				range = 100.0;
			}
			if (range == 0.0)
			{
				// �l���P�l�����Ȃ��ꍇ�͈̔͒l�Z�b�g
				range = fabs(m_dTotalScaleLMax) / 100.0;
			}
			if (range == 0.0)
			{
				// �ǂ����Ă��͈͒l��0�ɂȂ�ꍇ
				range = 100.0;
			}

			double rangeMargin = range * (double)AUTO_V_TOPBOTMARGIN;
			double rangeWidth = range + (double)AUTO_V_TOPBOTMARGIN * 2;
			m_dGridScaleLMax = CeilKeta(m_dTotalScaleLMax + rangeMargin, 1, rangeWidth);
			m_dGridScaleLMin = CeilKeta(m_dTotalScaleLMin - rangeMargin, 0, rangeWidth);
#endif /* _GRAPHDRAW_KAIZEN */
			if (m_dGridScaleLMax >= m_dGridScaleLMin)
			{
				dVLRange = m_dGridScaleLMax - m_dGridScaleLMin;
				dVLPos = m_dGridScaleLMax;
//++ 1�h�b�g���ꏈ�u >>>
				dVLMin = m_dGridScaleLMin;
//++ 1�h�b�g���ꏈ�u <<<
			}
			else
			{
				dVLRange = 0.0;
				dVLPos = 0.0;
//++ 1�h�b�g���ꏈ�u >>>
				dVLMin = 0.0;
//++ 1�h�b�g���ꏈ�u <<<
			}
		}
		else
		{
			m_dGridScaleLMax = m_nStvScale.VScalLMax;
			m_dGridScaleLMin = m_nStvScale.VScalLMin;
			dVLRange = m_nStvScale.VScalLMax - m_nStvScale.VScalLMin;
			dVLPos = m_nStvScale.VScalLMax;
//++ 1�h�b�g���ꏈ�u >>>
			dVLMin = m_nStvScale.VScalLMin;
//++ 1�h�b�g���ꏈ�u <<<
		}
		dVRRange = m_nStvScale.VScalRMax - m_nStvScale.VScalRMin;
		dVRPos = m_nStvScale.VScalRMax;
		dVLStep = fabs(dVLRange / m_nStvScale.VSCalItvl);
		dVRStep = fabs(dVRRange / m_nStvScale.VSCalItvl);
		int nStrLPos1 = -1;
		int nStrLPos2 = -1;
		int nStrRPos1 = -1;
		int nStrRPos2 = -1;

		CString strDigitMax = _T(" -###,###,###.###### ");
		CSize szDigitMax = memDC.GetTextExtent(strDigitMax);
		CString strLastDigit = _T("");
//-- 1�h�b�g���ꏈ�u >>>
//		double yPos;
//-- 1�h�b�g���ꏈ�u <<<
//++ 1�h�b�g���ꏈ�u >>>
		int yPos;
//++ 1�h�b�g���ꏈ�u <<<
		for (UINT n = 0; n <= (UINT)m_nStvScale.VSCalItvl; n++, dVLPos -= dVLStep, dVRPos -= dVRStep)
		{
			if (n == 0 || n == m_nStvScale.VSCalItvl)
				memDC.SelectObject(&penSolid);
			else
				memDC.SelectObject(&penLine);

			// ���r���̕\��ON
			if (n == 0 || n == m_nStvScale.VSCalItvl || m_bHGridOn)
			{
//-- 1�h�b�g���ꏈ�u >>>
//				yPos = (double)n * height / m_nStvScale.VSCalItvl;
//				memDC.MoveTo(RectClient.left, RectClient.top + (int)yPos);//double to int
//				memDC.LineTo(RectClient.right, RectClient.top + (int)yPos);//double to int
//-- 1�h�b�g���ꏈ�u <<<

//++ 1�h�b�g���ꏈ�u >>>
				// �������ƍ����̐ݒ�l���g�p����Y�l�v�Z
				double Y = (double)(m_nStvScale.VSCalItvl - n) * dVLStep + dVLMin;
				// Y�������W��0�̏ꍇ��Y�l���Ȃ����߁A�c���݂̂���v�Z����B
				// ����ȊO�̏ꍇ�́AConvPoint�֐���Y���W�v�Z�Ɠ�������
				int nY = (dVLRange == 0.0) ? ((int)((double)(m_nStvScale.VSCalItvl - n) * height / m_nStvScale.VSCalItvl + 0.5)) : ((int)((height * (Y - dVLMin)) / dVLRange + 0.5));
				yPos = height - nY + RectClient.top;
				memDC.MoveTo(RectClient.left, yPos);
				memDC.LineTo(RectClient.right, yPos);
//++ 1�h�b�g���ꏈ�u <<<
			}

			if (m_bViewDetail)
			{
				// Auto���A���c�����Auto�����̕\��
				if (m_nStvScale.Auto && n == 0)
				{
					long nVPos = (long)ceil(dVLPos);

					CString strDigit = _T("Auto");
					CSize sz = memDC.GetTextExtent(strDigit);

					// ���c�����l�����̊Ԃ�Auto����������
//-- 1�h�b�g���ꏈ�u >>>
//					memDC.TextOutW(RectClient.left + 5, RectClient.top + (int)(yPos + 0.5) - sz.cy, strDigit);
//-- 1�h�b�g���ꏈ�u <<<
//++ 1�h�b�g���ꏈ�u >>>
					memDC.TextOutW(RectClient.left + 5, yPos - sz.cy, strDigit);
//++ 1�h�b�g���ꏈ�u <<<
				}

				// �c���P�ʂ�\��
				if (n == 0)
				{
					// �v���p�e�B�̐ݒ�f�[�^���������[�v
					DRAWLINE::iterator itr;
					for (itr = m_DrawLines.begin(); itr != m_DrawLines.end(); itr++)
					{
						if ((*itr).second.strUnit.IsEmpty() == FALSE)
						{
							CString strDigit;
							strDigit.Format(_T("[%s]"), (LPCTSTR)(*itr).second.strUnit);
							CSize sz = memDC.GetTextExtent(strDigit);
							// ���c���̏�ɒP�ʕ���������
							memDC.TextOutW(RectClient.left - mUnitMarginLeft - sz.cx, RectClient.top - mUnitMarginTop, strDigit);

							// �c���̏����_�ȉ��̌����Z�b�g
							nDecimalKeta = (*itr).second.nDecDidits;
							break;
						}
					}
				}

				// ���r���̕\��ON
				if (n == 0 || n == m_nStvScale.VSCalItvl || m_bHGridOn)
				{
					// ���c���̒l�\��
					if (dVLStep != 0.0)
					{
						CString strDigit;
						strDigit = addCanma(dVLPos, nDecimalKeta);

						if (strLastDigit.CompareNoCase(strDigit) != 0)
						{
							// ���O�̖ڐ��萔�l���������v���Ȃ��ꍇ�ɐV�����ڐ��萔�l�\��
							CSize sz = memDC.GetTextExtent(strDigit);
	
//-- 1�h�b�g���ꏈ�u >>>
//							nStrLPos1 = RectClient.top + (int)(yPos + 0.5) - sz.cy;
//-- 1�h�b�g���ꏈ�u <<<
//++ 1�h�b�g���ꏈ�u >>>
							nStrLPos1 = yPos - sz.cy;
//++ 1�h�b�g���ꏈ�u <<<
							if (nStrLPos2 < 0 ||
								nStrLPos1 > nStrLPos2 + sz.cy)
							{
								nStrLPos2 = nStrLPos1;
								memDC.TextOutW(RectClient.left - sz.cx - mLabelLeftMargine, nStrLPos2, strDigit);
								strLastDigit = strDigit;
							}
						}
					}
				}
			}
		}
	}
	if (m_nHorzDiv > 0 && bRangeValid == TRUE)
	{
		// �c���`��
		memDC.SelectObject(&penLine);

		// �S�g�����h�c���i�c���̉�����h�~�j
		int nStrPos1 = -1;
		int nStrPos2 = -1;

		CTime		t = m_StartTime;
		CTime		prev_t;
		CString		strLastYmd;
		strLastYmd.Format(_T(""));
		for (double n1 = 0.0, n2 = 0.0;
			n1 < dEndN;
//			n1 += (m_dbHzRange / (double)m_nHorzDiv), n2 += (m_dbTmpHzRange / (double)m_nHorzDiv), prev_t = t, t += (time_t)(m_dbHzRange / (double)m_nHorzDiv))
			n1 += ((m_dbHzRange - 1)/ (double)m_nHorzDiv), n2 += (m_dbTmpHzRange / (double)m_nHorzDiv), prev_t = t, t += (time_t)((m_dbHzRange - 1)/ (double)m_nHorzDiv))
		{

			int nInipoint = ConvPoint((double)n2).x;
			memDC.MoveTo(nInipoint, RectClient.top);
			memDC.LineTo(nInipoint, RectClient.bottom);

			if (m_bViewDetail)
			{
				// �����̎����\��
				CString strYmd;
				CString strHMS;
				CSize szYmd;
				CSize szHMS;
				// ���[�̎����������́A�N�������ς��Ƃ��A�N������\��
				if (prev_t == t || prev_t.GetYear() != t.GetYear())
				{
					strYmd = t.Format("%Y-%m-%d");
				}
				else if (prev_t.GetMonth() != t.GetMonth() || prev_t.GetDay() != t.GetDay())
				{
					strYmd = t.Format("%m-%d");
				}
				else if (!strLastYmd.IsEmpty())
				{
					strYmd = strLastYmd;
					strLastYmd.Format(_T(""));
				}

				szYmd = memDC.GetTextExtent(strYmd);

				// �����b
				strHMS = t.Format("%H:%M:%S");
				szHMS = memDC.GetTextExtent(strHMS);

				// �傫��������̃T�C�Y���擾
				int xRange, xYmd;
				CSize szYmdDef = memDC.GetTextExtent(strYmd);

				if (szYmdDef.cx > szYmd.cx)
				{
					xYmd = szYmdDef.cx;
				}
				else
				{
					xYmd = szYmd.cx;
				}

				if (xYmd > szHMS.cx)
				{
					xRange = xYmd;
				}
				else
				{
					xRange = szHMS.cx;
				}

				nStrPos1 = nInipoint;
				if (nStrPos2 < 0 ||
					nStrPos1 > nStrPos2 + xRange + (long)mHStrDistance)
				{
					nStrPos2 = nStrPos1;
					memDC.TextOutW(nInipoint - xYmd, RectClient.bottom + 4, strYmd);
					memDC.TextOutW(nInipoint - szHMS.cx, RectClient.bottom + 4 + mTimeLabel2ndMargine, strHMS);
				}
				else if (!strYmd.IsEmpty())
				{
					// �N�������\���ł��Ȃ������ꍇ�́A���ɂ܂킷
					strLastYmd = strYmd;
				}
			}
		}
	}

	memDC.SelectObject(oldPen);
	memDC.SelectObject(oldFont);
	penLine.DeleteObject();
	penSolid.DeleteObject();

	return bRangeValid;
}

/*============================================================================*/
/*! �O���t�`��

-# �J�[�\���`��

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::DrawCursor(CDC& memDC)
{
	CRect rect;
	GetClientRect(rect);

	CPoint	curPoint;

	switch (m_nGraphType)
	{
	case	eGraphType_OffTrend:
		{
			int pos = m_CursorPos - 1;
			DRAWLINE::const_iterator iterLine = m_DrawLines.begin();
			int endpos = (int)((m_dbHzRange + m_pointTop) / m_nTrendInc);
			pair<double, double> pnt;
			pnt.first = (double)((double)pos*m_nTrendInc); pnt.second = (double)0;
			curPoint = ConvPoint(pnt, (*iterLine).second.nMaxValue, (*iterLine).second.nMinValue);
			if (rect.right == curPoint.x){
				// �O���t�̈�̍ŉE�����ƃJ�[�\���ʒu�������ꍇ
				// �J�[�\���������Ȃ��Ȃ��Ă��܂��Ή�
				curPoint.x -= 1;
			}

			CPen		pen;
			COLORREF	cf = RGB(0, 255, 255);
			COLORREF	cfs = RGB(0, 0, 0);
			if (pen.CreatePen(PS_SOLID, 1, cfs)){
				CPen* pOldPen = memDC.SelectObject(&pen);
				pen.DeleteObject();
				memDC.MoveTo(curPoint.x + 1, rect.top);
				memDC.LineTo(curPoint.x + 1, rect.bottom);
				memDC.SelectObject(pOldPen);
			}
			if (pen.CreatePen(PS_SOLID, 1, cf)){
				CPen* pOldPen = memDC.SelectObject(&pen);
				memDC.MoveTo(curPoint.x, rect.top);
				memDC.LineTo(curPoint.x, rect.bottom);
				memDC.SelectObject(pOldPen);
				pen.DeleteObject();
			}
			break;
		}
	}
}

/*============================================================================*/
/*! �O���t�`��

-# �O���t���̕`��

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::DrawGraph(CDC& memDC)
{
	if (m_bIsRealTime == FALSE)
	{
		// �b�菈�u�B�I�����C���̏ꍇ�͕`�悵�Ȃ�
		//return;
	}

	switch (m_nGraphType){
	case	eGraphType_All:
	case	eGraphType_Trend:
	case	eGraphType_Summary:
		// �g�����h�O���t�̕`��
		DrawTrend(memDC);
		break;
	case	eGraphType_OffAll:
	case	eGraphType_OffTrend:
		// �g�����h�O���t�̕`��
		DrawOffTrend(memDC);
		break;
	default:
		// ��L�ȊO�O���t�̕`��
		return;
	}
}

/*============================================================================*/
/*! �O���t�`��

-# �g�����h�O���t���̕`��

@param	memDC	�f�o�C�X�R���e�L�X�g
@retval	�Ȃ�

*/
/*============================================================================*/
void CGraphWindow::DrawTrend(CDC& memDC)
{
	CRect rect, rectGrid;
	GetClientRect(rect);

	if (m_bViewDetail)
	{
		rect.SetRect(
			rect.left,
			rect.top + mGridMarginTop,
			rect.right,
			rect.bottom - mGridMarginBottom + 1);

		rectGrid = rect;
		rectGrid.left += mGridMarginLeft,
			rectGrid.right -= mGridMarginRight;
	}
	else
	{
		rectGrid = rect;
	}

	// ���[�W��������
	HRGN	hRgn = CreateRectRgn(rectGrid.left, rectGrid.top, rectGrid.right, rectGrid.bottom);
	SelectClipRgn(memDC, hRgn);

	CPoint pointDraw;
	CPen penLine, penCurValue;
	CPen* oldPen;
	CBrush brushDots, brushCurValue;
	CBrush* oldBrush;
	CFont* oldFont;
	CPoint pointLastSigName;

	penCurValue.CreatePen(PS_SOLID, 1, m_ColorGrid);
	brushCurValue.CreateSolidBrush(m_ColorLighttGrid);
	memDC.SetTextColor(m_ColorGrid);

	MutexLock();

	// �g�����h�`��
	for (int i = eSigSrcType_File; i >= eSigSrcType_EngExchange; i--)
	{
		// �f�[�^�����t�@�C���̏ꍇ�����Ƀg�����h�`�悷��
		DrawTrendContents(memDC, i);
	}

	// ���[�W��������
	SelectClipRgn(memDC, NULL);
	DeleteObject(hRgn);

	// �ϕ��\��
	BOOL bViewInteg = FALSE;
	UINT nStationID = theApp.GetSelectStation();
	// �T�}���O���t�łȂ��A���V�Y24m�łȂ��A����AGC�O���t�̏ꍇ
	if (m_nGraphType != eGraphType_Summary && nStationID != eStation_Uchinoura20 && m_nPropertiesID == eTrendWndType_AGC)
	{
		bViewInteg = TRUE;
	}

	// �}��`��
	if (m_bViewDetail)
	{
		// �t�H���g�̐ݒ�
		oldFont = memDC.SelectObject(&m_txtFontSmall);

		DRAWLINE::iterator itr;
		UINT nID;

		// �v���p�e�B�̐ݒ�f�[�^���������[�v
		BOOL bFirst = TRUE;
		for (itr = m_DrawLines.begin(); itr != m_DrawLines.end(); itr++)
		{
			// �f�[�^���������ꍇ�͕\�����Ȃ�
			if ((*itr).second.strName.IsEmpty() == TRUE)
			{
				continue;
			}

			nID = (*itr).first;
			structLine data = (*itr).second;

			// �\���ݒ�
			// ���F�̐ݒ�
			penLine.CreatePen((*itr).second.nStyle, 1, (*itr).second.crColor);
			oldPen = memDC.SelectObject(&penLine);

			// �~�F�̐ݒ�
			brushDots.CreateSolidBrush((*itr).second.crColor);
			oldBrush = memDC.SelectObject(&brushDots);
			int r;	// �~�i�_�j�̒��a

			// �f�[�^���\��
			//				memDC.SetTextColor((*itr).second.crColor);
			memDC.SetBkMode(TRANSPARENT);
			CString	strName;
			if ((*itr).second.strGName.IsEmpty() == TRUE)
			{
				strName = (*itr).second.strName;
			}
			else
			{
				strName = (*itr).second.strGName;
			}
			CSize szStr;
			szStr = memDC.GetTextExtent(strName);

			CPoint ptSigName;
			ptSigName.x = rect.right - mGridMarginRight + mRightMargin;
			ptSigName.y = rect.top + mTopMargin + mSigStrHeight * nID;

			// �ϕ��\���ł��A�P�c64m�A���V�Y34m���ꂼ��̐ϕ��M���ʒu�̏ꍇ�A�M���̊Ԃ��󂯂�
//			if (bViewInteg && (nStationID == eStation_Usuda64 && nID >= 1 || nStationID == eStation_Usuda642 && nID >= 1 || nStationID == eStation_Uchinoura34 && nID >= 11))
			if (bViewInteg && (nStationID == eStation_Usuda64 && nID >= 1 || nStationID == eStation_Uchinoura34 && nID >= 11))
			{
				ptSigName.y++;
			}

			pointLastSigName = ptSigName;

			switch ((*itr).second.nDrawKind)
			{
			case Line:
				// �}�����ŕ`��
				memDC.MoveTo(ptSigName.x, ptSigName.y + szStr.cy / 2);
				ptSigName.x += mLegendWeidth;
				memDC.LineTo(ptSigName.x, ptSigName.y + szStr.cy / 2);
				break;

			case Dots:
				// �}����~�ŕ`��
				r = (*itr).second.nDotSize / 2;
				memDC.Ellipse(ptSigName.x + mLegendWeidth / 2 - r, ptSigName.y + szStr.cy / 2 - r, ptSigName.x + mLegendWeidth / 2 + r, ptSigName.y + szStr.cy / 2 + r);
				ptSigName.x += mLegendWeidth;
				break;

			case LineAndDots:
				// �}�����Ɖ~�ŕ`��
				r = (*itr).second.nDotSize / 2;
				memDC.Ellipse(ptSigName.x + mLegendWeidth / 2 - r, ptSigName.y + szStr.cy / 2 - r, ptSigName.x + mLegendWeidth / 2 + r, ptSigName.y + szStr.cy / 2 + r);
				memDC.MoveTo(ptSigName.x, ptSigName.y + szStr.cy / 2);
				ptSigName.x += mLegendWeidth;
				memDC.LineTo(ptSigName.x, ptSigName.y + szStr.cy / 2);
				break;

			default:
				break;
			}

			ptSigName.x += mLegendLineStrDistance;
//			memDC.TextOutW(ptSigName.x, ptSigName.y, strName);

			// ���ݒl�`��
			CRect rectCurValue(rect.right - mHStrDistance - mCurValueWidth, ptSigName.y - mVStrDistance, rect.right - mHStrDistance, ptSigName.y + szStr.cy + mVStrDistance);
			memDC.SelectObject(&penCurValue);
			memDC.FillSolidRect(rectCurValue, COLOR_WHITE);
			memDC.FrameRect(rectCurValue, &brushCurValue);
			if (bFirst)
			{
				bFirst = FALSE;
				m_rectCurValArea = rectCurValue;
			}
			else
			{
				m_rectCurValArea.bottom = ptSigName.y + szStr.cy + mVStrDistance;
			}

			rectCurValue.left += mHStrDistance;
			rectCurValue.right -= mHStrDistance;
			memDC.DrawText(addCanma(m_curVal[nID], (*itr).second.nDecDidits), rectCurValue, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
			//				memDC.DrawText(addCanma(val, 0), rectCurValue, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

			memDC.SelectObject(oldPen);
			memDC.SelectObject(oldBrush);
			penLine.DeleteObject();
			brushDots.DeleteObject();
		}
		memDC.SelectObject(oldFont);
	}

	// �ϕ��\������ꍇ
	if (bViewInteg && pointLastSigName.x != 0)
	{
		// �ϕ����Ԑݒ�{�^���̐����\��
		// �t�H���g�̐ݒ�
		oldFont = memDC.SelectObject(&m_txtFontSmall);

		CRect rcIntgStr;
		rcIntgStr.left = pointLastSigName.x;
		rcIntgStr.right = rect.right;
		//		rcIntgStr.top = pointLastSigName.y + mSigStrHeight + mTopMargin;
		rcIntgStr.top = pointLastSigName.y + mSigStrHeight;
		rcIntgStr.bottom = rcIntgStr.top + mBtnHeight;
		CString strItegTime(_T("INTEG TIME"));
		memDC.DrawText(strItegTime, rcIntgStr, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

		CRect rectIntgFrame;
		rectIntgFrame = rcIntgStr;

		CSize sz = memDC.GetTextExtent(strItegTime);
		// �ϕ����Ԑݒ�{�^�����W�v�Z
		m_rectIntgBtn = rcIntgStr;
		m_rectIntgBtn.left += sz.cx + mBtnIntgMarginRight;
		m_rectIntgBtn.right = m_rectIntgBtn.left + mBtnWidth;
		if (m_btnIntg.GetSafeHwnd() == NULL)
		{
			// �ϕ����Ԑݒ�{�^���\��
			CString strBtn;
			if (m_nIntgTime > 0)
				strBtn.Format(_T("%d"), m_nIntgTime);
			m_btnIntg.Create(strBtn, BS_PUSHBUTTON | WS_VISIBLE, m_rectIntgBtn, this, ID_INTGBTN);
			m_btnIntg.SetFont(&m_txtFontNormal);
		}

		rcIntgStr = m_rectIntgBtn;
		rcIntgStr.left = rcIntgStr.right + mHStrDistance * 2;
		rcIntgStr.right += mIntgStrMarginWidth;
		memDC.DrawText(_T("[s]"), rcIntgStr, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

		memDC.SelectObject(oldFont);

		if (m_StationIndex == eStation_Usuda64 || m_StationIndex == eStation_Uchinoura34)
		{
			// 64m, 34m�͘g���`��
			rectIntgFrame.left -= mHStrDistance * 2;
			if (nStationID == eStation_Uchinoura34)
			{
				rectIntgFrame.top -= mIntegFrameTopOffsetUsc34;
			}
			else
			{
				rectIntgFrame.top -= mIntegFrameTopOffsetUdsc64;
			}
			rectIntgFrame.bottom += mIntegFrameBotOffset;
			memDC.SelectObject(&penCurValue);
			memDC.FrameRect(rectIntgFrame, &brushCurValue);
		}
	}

	penCurValue.DeleteObject();
	brushCurValue.DeleteObject();

	MutexUnlock();
}

/*============================================================================*/
/*! �O���t�`��

-# �g�����h�O���t���̕`��

@param	memDC�F�f�o�C�X�R���e�L�X�g
@param	nSrcType�F�f�[�^���^�C�v(0:�H�w�l�ϊ���f�[�^ / 1:�t�@�C���ǂݍ��݁j
@retval	�Ȃ�

*/
/*============================================================================*/
void CGraphWindow::DrawTrendContents(CDC& memDC, int nSrcType)
{
	DRAWLINE::iterator itr;

	CPoint pointDraw;
	CPen penLine;
	CPen* oldPen;
	CBrush brushDots;
	CBrush* oldBrush;
	CFont* oldFont;
	CPoint pointLastSigName;

	double val = 0.0;
	UINT nID;
	pair<double, double> pnt;

	int mSaveLogData;
	int mLogData;
	__int64 size;

	m_pointTop = 0.0;

	// �g�����h�̕`��
	CGraphMutex::Lock(eData);

	// �Ď��f�[�^���X�g�̎擾
	CSpaceData*	spaceDataList = theApp.GetDataCollection().GetSpaceData(m_StationIndex);
	CSpaceData*	sData;

	// �v���p�e�B�̐ݒ�f�[�^���������[�v
	for (itr = m_DrawLines.begin(); itr != m_DrawLines.end(); itr++)
	{
		// �f�[�^���������ꍇ�͕\�����Ȃ�
		if ((*itr).second.strName.IsEmpty() == TRUE)
		{
			continue;
		}

		// �f�[�^�C���f�b�N�X���Ȃ��ꍇ�͕\�����Ȃ�
		int nListIdx = (int)((*itr).second.nListIdx);
		if (nListIdx < 0)
		{
			continue;
		}

		nID = (*itr).first;
		structLine data = (*itr).second;
		// ��\���ݒ�Ȃ�Ε\�����Ȃ�
		if (data.bIsVisible == FALSE)
		{
			continue;
		}
		
		mSaveLogData = -1;
		mLogData = -1;

		sData = &spaceDataList[nListIdx];
		size = 0;

		// �f�[�^��������Ȃ��ꍇ
		if (sData == NULL)
		{
			// �\�����Ȃ�
			continue;
		}

		// �f�[�^����v�m�F
		if (sData->srcType != nSrcType)
		{
			// �\�����Ȃ�
			continue;
		}

		// �f�[�^�T�C�Y�̎擾
		size = (__int64)sData->value.size();
		if (size == 0)
		{
			// �Ď��f�[�^���Ȃ���Ε\�����Ȃ�
			continue;
		}

		// �L���[�ɖ����ꍇ�͉������Ȃ�
		if (m_TrendDataPP[nID].spaceData.empty() == TRUE)
		{
			continue;
		}

// test >>>
//		if (m_nPropertiesID == eTrendWndType_EL)
//		{
//			int c = 0;
//		}
// test <<<


		// �`��ON�̏ꍇ�ɃO���t�f�[�^��\������
		if (m_bIsDrawOn[nID] == TRUE)
		{
			__int64 pos = m_nTrendDataPos;
			size = (__int64)m_TrendDataPP[nID].spaceData.size();

			// �L���[�T�C�Y��茻�ݒl���傫���ꍇ�͕`�悵�Ȃ�
			if (pos >= size)
				continue;

			__int64 xpos = 0;
//			__int64 drawPos = -1;
			int saveX = -1;
			BOOL bIsLastMaxValue = FALSE;
			bool ppTrend = true;

			// �\���ݒ�
			// ���F�̐ݒ�
			penLine.CreatePen((*itr).second.nStyle, (*itr).second.nWidth, (*itr).second.crColor);
			oldPen = memDC.SelectObject(&penLine);

			// �~�F�̐ݒ�
			brushDots.CreateSolidBrush((*itr).second.crColor);
			oldBrush = memDC.SelectObject(&brushDots);
			int r;	// �~�i�_�j�̒��a

			// �t�H���g�̐ݒ�
			oldFont = memDC.SelectObject(&m_txtFontSmall);

			if (size > 0)
			{
				// �J�n������T��
				time_t posTime = CTime(m_TrendDataPP[nID].spaceData[pos].systime).GetTime();

#if 0
				// �񈳏k�̏ꍇ
				if (m_TrendDataPP[nID].spaceData[0].ppPair.count == 1)
				{
					// �J�n�������L���[�̎w��f�[�^�������傫���ꍇ
					if (m_StartTime > posTime)
					{
						// �J�n�����ɃI�t�Z�b�g��t��
						__int64 offset = m_StartTime - posTime;
						if (pos + offset < size)
						{
							pos += offset;
						}
						else
						{
							xpos -= offset;
						}

						// �J�n�O�̗L���f�[�^�������A�O���t�̈�̒[����[�܂ŕ\��
						while (pos > 0)
						{
							if ((char)m_TrendDataPP[nID].spaceData[pos].status[0] >= 0)
							{
								break;
							}
							pos--;
							xpos--;
						}
					}
					// �J�n�������L���[�̎w��f�[�^���������������ꍇ
					else if (m_StartTime < posTime)
					{
						// �J�n�����ɃI�t�Z�b�g��t��
						__int64 offset = posTime - m_StartTime;
						if (pos >= offset)
						{
							pos -= offset;
						}
						else
						{
							xpos += offset;
						}
						// �J�n�O�̗L���f�[�^�������A�O���t�̈�̒[����[�܂ŕ\��
						while (pos > 0)
						{
							if ((char)m_TrendDataPP[nID].spaceData[pos].status[0] >= 0)
							{
								break;
							}
							pos--;
							xpos--;
						}
					}
					else
					{
						// �J�n�O�̗L���f�[�^�������A�O���t�̈�̒[����[�܂ŕ\��
						while (pos > 0)
						{
							if ((char)m_TrendDataPP[nID].spaceData[pos].status[0] >= 0)
							{
								break;
							}
							pos--;
							xpos--;
						}
					}
				}
				// ���k�̏ꍇ
				else
#endif
				{
					time_t queStartTime = CTime(m_TrendDataPP[nID].spaceData[0].systime).GetTime();
					time_t queEndTime = CTime(m_TrendDataPP[nID].spaceData[size - 1].systime).GetTime();

					// �J�n�������L���[�̎w��f�[�^�������傫���ꍇ
					if (m_StartTime > posTime)
					{
						// �J�n�����ɃI�t�Z�b�g��t��
						__int64 offset = m_StartTime - posTime;
						// �L���[�͈͓��ɊJ�n����������ꍇ
						if (m_StartTime <= queEndTime)
						{
							// �J�n����pos�������ŒT��
							for (__int64 i = pos; i < size; i++)
							{
								if ((char)m_TrendDataPP[nID].spaceData[i].status[0] >= 0)
								{
									posTime = CTime(m_TrendDataPP[nID].spaceData[i].systime).GetTime();
									if (posTime > m_StartTime)
									{
										// �J�n�������z�����L���[�ʒu��������
										break;
									}
									else
									{
										pos = i;
									}
								}
							}
						}
						// �L���[�͈͓��ɊJ�n�������Ȃ��ꍇ
						else
						{
							xpos -= offset;
						}

						// �J�n�O�̗L���f�[�^�������A�O���t�̈�̒[����[�܂ŕ\��
						while (pos > 0)
						{
							if ((char)m_TrendDataPP[nID].spaceData[pos].status[0] >= 0)
							{
								break;
							}
							pos--;
							xpos--;
						}
					}
					// �J�n�������L���[�̎w��f�[�^���������������ꍇ
					else if (m_StartTime < posTime)
					{
						// �J�n�����ɃI�t�Z�b�g��t��
						__int64 offset = posTime - m_StartTime;
						// �L���[�͈͓��ɊJ�n����������ꍇ
						if (queStartTime <= m_StartTime)
						{
							// �J�n����pos�������ŒT��
							for (__int64 i = pos; i < size; i++)
							{
								if ((char)m_TrendDataPP[nID].spaceData[i].status[0] >= 0)
								{
									posTime = CTime(m_TrendDataPP[nID].spaceData[i].systime).GetTime();
									if (posTime > m_StartTime)
									{
										// �J�n�������z�����L���[�ʒu��������
										break;
									}
									else
									{
										pos = i;
									}
								}
							}
						}
						else
						{
							xpos += offset;
						}
						// �J�n�O�̗L���f�[�^�������A�O���t�̈�̒[����[�܂ŕ\��
						while (pos > 0)
						{
							if ((char)m_TrendDataPP[nID].spaceData[pos].status[0] >= 0)
							{
								break;
							}
							pos--;
							xpos--;
						}
					}
					else
					{
						// �J�n�O�̗L���f�[�^�������A�O���t�̈�̒[����[�܂ŕ\��
						while (pos > 0)
						{
							if ((char)m_TrendDataPP[nID].spaceData[pos].status[0] >= 0)
							{
								break;
							}
							pos--;
							xpos--;
						}
					}
				}
			}

			BOOL bFirst = TRUE;
			BOOL bSetCurVal = FALSE;

			// ���߂̒l���Z�b�g���Ă���
			val = m_curVal[nID];

			// ��ʂ�x�͈͂̂ݕ`��
			while (pos < size)
			{
				// ���̊Ď��f�[�^���玞���̌���
				time_t cur_t = CTime(m_TrendDataPP[nID].spaceData[pos].systime).GetTime();
				if (m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange)
				{
					while (xpos < (__int64)m_dbHzRange && m_StartTime + xpos < cur_t)
					{
						// ��������������΍�B������������܂�x�ʒu�����炷
						xpos++;
					}
				}
				else
				{
					xpos = cur_t - m_StartTime;
				}

//				// �E�B���h�E�͈͓���`�悵���甲����
//				//				if (xpos >= (int)m_dbHzRange)
//				if (drawPos >= (__int64)m_dbHzRange)
//				{
//					if (m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange)
//						break;
//				}

				// �\���\���
				if ((char)m_TrendDataPP[nID].spaceData[pos].status[0] >= 0)
				{
//					drawPos = xpos;

					// �񈳏k�\��
					if (m_TrendDataPP[nID].spaceData[0].ppPair.count == 1 && m_TrendDataPP[nID].spaceData[pos].ppPair.count == 1)
					{
						val = m_TrendDataPP[nID].spaceData[pos].ppPair.max;
						pnt.first = (double)xpos; pnt.second = val;
						pointDraw = ConvPoint(pnt, m_dGridScaleLMax, m_dGridScaleLMin);
						switch ((*itr).second.nDrawKind)
						{
						case Line:
							// ���̕`��
							if ((*itr).second.nGraphKind == 1)
							{
								if (xpos == 0 || pos == 0 ||
									m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange && (char)m_TrendDataPP[nID].spaceData[pos - 1].status[0] < 0 ||
									m_TrendDataPP[nID].srcType == eSigSrcType_File && bFirst)
								{
									memDC.MoveTo(pointDraw);
									bFirst = FALSE;
								}
								else
								{
									CPoint pt = pointDraw;
									pt.y = mSaveLogData;
									memDC.LineTo(pt);
									memDC.LineTo(pointDraw);
								}
								mSaveLogData = pointDraw.y;
							}
							else
							{
								if (xpos == 0 || pos == 0 ||
									m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange && (char)m_TrendDataPP[nID].spaceData[pos - 1].status[0] < 0 ||
									m_TrendDataPP[nID].srcType == eSigSrcType_File && bFirst)
								{
									memDC.MoveTo(pointDraw);
									bFirst = FALSE;
								}
								else
								{
									memDC.LineTo(pointDraw);
								}
							}

							break;

						case Dots:
							// �~�̕`��
							r = (*itr).second.nDotSize / 2;
							memDC.Ellipse(pointDraw.x - r, pointDraw.y - r, pointDraw.x + r, pointDraw.y + r);
							break;

						case LineAndDots:
							// ���̕`��
							if ((*itr).second.nGraphKind == 1)
							{
								if (xpos == 0 || pos == 0 ||
									m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange && (char)m_TrendDataPP[nID].spaceData[pos - 1].status[0] < 0 ||
									m_TrendDataPP[nID].srcType == eSigSrcType_File && bFirst)
								{
									memDC.MoveTo(pointDraw);
									bFirst = FALSE;
								}
								else
								{
									CPoint pt = pointDraw;
									pt.y = mSaveLogData;
									memDC.LineTo(pt);
									memDC.LineTo(pointDraw);
								}
								mSaveLogData = pointDraw.y;
							}
							else{
								if (xpos == 0 || pos == 0 ||
									m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange && (char)m_TrendDataPP[nID].spaceData[pos - 1].status[0] < 0 ||
									m_TrendDataPP[nID].srcType == eSigSrcType_File && bFirst)
								{
									memDC.MoveTo(pointDraw);
									bFirst = FALSE;
								}
								else
								{
									memDC.LineTo(pointDraw);
								}
							}

							// �~�̕`��
							r = (*itr).second.nDotSize / 2;
							memDC.Ellipse(pointDraw.x - r, pointDraw.y - r, pointDraw.x + r, pointDraw.y + r);
							break;

						default:
							// �����ɂ͗��Ȃ��B
							break;
						}
					}
					// ���k�\��
					else if (m_TrendDataPP[nID].spaceData[pos].ppPair.count > 1)
					{
						switch ((*itr).second.nDrawKind)
						{
						case Line:
							// ���̕`��
							if (xpos == 0 || pos == 0 ||
								m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange && (char)m_TrendDataPP[nID].spaceData[pos - 1].status[0] < 0 ||
								m_TrendDataPP[nID].srcType == eSigSrcType_File && bFirst)
							{
								PPLine1st();
								bFirst = FALSE;
							}
							else
							{
								// �O�񂪏㏸��
								if (bIsLastMaxValue == TRUE)
								{
									if (m_TrendDataPP[nID].spaceData[pos].ppPair.max >= m_TrendDataPP[nID].spaceData[pos - 1].ppPair.max)
									{
										// �㏸���̂���min����max
										PPLineMinToMax();
									}
									else
									{
										// ���~���̂���max����min
										PPLineMaxToMin();
									}
								}
								// �O�񂪉��~��
								else
								{
									if (m_TrendDataPP[nID].spaceData[pos].ppPair.min <= m_TrendDataPP[nID].spaceData[pos - 1].ppPair.min)
									{
										// ���~���̂���max����min
										PPLineMaxToMin();
									}
									else
									{
										// �㏸���̂���min����max
										PPLineMinToMax();
									}
								}
							}
							break;

						case Dots:
							// �~�̕`��
							PPEllipse();
							break;

						case LineAndDots:
							// ���̕`��
							if (xpos == 0 || pos == 0 ||
								m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange && (char)m_TrendDataPP[nID].spaceData[pos - 1].status[0] < 0 ||
								m_TrendDataPP[nID].srcType == eSigSrcType_File && bFirst)
							{
								PPLine1st();
								bFirst = FALSE;
							}
							else
							{
								// �O�񂪏㏸��
								if (bIsLastMaxValue == TRUE)
								{
									if (m_TrendDataPP[nID].spaceData[pos].ppPair.max >= m_TrendDataPP[nID].spaceData[pos - 1].ppPair.max)
									{
										// �㏸���̂���min����max
										PPLineMinToMax();
									}
									else
									{
										// ���~���̂���max����min
										PPLineMaxToMin();
									}
								}
								// �O�񂪉��~��
								else
								{
									if (m_TrendDataPP[nID].spaceData[pos].ppPair.min <= m_TrendDataPP[nID].spaceData[pos - 1].ppPair.min)
									{
										// ���~���̂���max����min
										PPLineMaxToMin();
									}
									else
									{
										// �㏸���̂���min����max
										PPLineMinToMax();
									}
								}
							}
							// �~�̕`��
							PPEllipse();
							break;

						default:
							// �����ɂ͗��Ȃ��B
							break;
						}
					}

					if (saveX >= 0 && saveX != pointDraw.x){
						ppTrend = false;
						//TRACE("### ppTrend!!!\n");
					}

					// �t�@�C���^�C�v�̃f�[�^����\���p�f�[�^�l�擾
					if (m_TrendDataPP[nID].srcType == eSigSrcType_File && bSetCurVal == FALSE && cur_t >= m_MostNewTime - m_nTrendDataPos)				
					{
						bSetCurVal = TRUE;
						m_curVal[nID] = val;
					}
				}

				saveX = pointDraw.x;
				pos++;
			}

			if (m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange)
			{
				m_curVal[nID] = val;
			}

			if (ppTrend == true && size > 1)
			{
				saveX = -1;
			}

			// ����f�[�^�̕`��
			memDC.SelectObject(oldPen);
			memDC.SelectObject(oldBrush);
			memDC.SelectObject(oldFont);
			penLine.DeleteObject();
			brushDots.DeleteObject();
		}
		else
		{
			m_bIsDrawOn[nID] = TRUE;
		}
	}

	CGraphMutex::Unlock(eData);
}

/*============================================================================*/
/*! �O���t�`��

-# �g�����h�O���t���̕`��

@param	memDC	�f�o�C�X�R���e�L�X�g
@retval	�Ȃ�

*/
/*============================================================================*/
void CGraphWindow::DrawOffTrend(CDC& memDC)
{
}

/*============================================================================*/
/*! �O���t�`��

-# �����f�[�^�̒ǉ�

@param nID		���C���h�c
@param crColor	���C���F
@param nStyle	���C���X�^�C��
@param nWidth	���C����
@param bVisible	���C���̕\��/��\��

@retval	bool

*/
/*============================================================================*/
BOOL CGraphWindow::AddLineToMap(UINT nID, structLine line)
{
//#ifdef _DEBUG
//	{
//		CString str;
//		str.Format(_T("%s(%d) :: Sig[%d], GraphType = %u, ViewID = %u"), __FILEW__, __LINE__, nID, m_nGraphType, m_nPropertiesID);
//		CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("AddLineToMap"), _T("debug"), str, nLogEx::debug);
//	}
//#endif

	MutexLock();
	CGraphMutex::Lock(eData);

	// �ő�l�A�ŏ��l��������
	if (m_DrawLines.find(nID) == m_DrawLines.end())
	{
		line.nMaxValue = m_dTotalScaleLMax;
		line.nMinValue = m_dTotalScaleLMin;
	}

	CSpaceData*	spaceDataList = theApp.GetDataCollection().GetSpaceData(m_StationIndex);

	// �Œ�X�P�[��
	// �܂��͎����X�P�[���i�񏉊��l�j
	if (m_DrawLines.find(nID) != m_DrawLines.end() &&
		(m_nStvScale.Auto == FALSE ||
		(spaceDataList[0].value.empty() == FALSE)))
	{
		line.nMaxValue = m_DrawLines[nID].nMaxValue;
		line.nMinValue = m_DrawLines[nID].nMinValue;
	}
	// �����X�P�[���i�����l�j
	else
	{
		line.nMaxValue = m_dTotalScaleLMax;
		line.nMinValue = m_dTotalScaleLMin;
	}

//	// �M�������قȂ鎞�ɃN���A
//	if (m_DrawLines[nID].strGName.Compare(line.strGName) != 0)
	{
//#ifdef _DEBUG
//		{
//			CString str;
//			str.Format(_T("%s(%d) :: Diff Sig[%d], GraphType = %u, ViewID = %u"), __FILEW__, __LINE__, nID, m_nGraphType, m_nPropertiesID);
//			CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("AddLineToMap"), _T("debug"), str, nLogEx::debug);
//		}
//#endif
//		line.nListIdx = -1;				// �q���f�[�^���X�g�̃C���f�b�N�X������
		line.nMaxValue = -DBL_MAX;		// �ő�l�N���A
		line.nMinValue = DBL_MAX;		// �ŏ��l�N���A

		m_nQueuLastTotalSize[nID] = 0;	// ���߂̃L���[�g�[�^���T�C�Y�N���A
		m_PPNumber[nID] = 0;			// ���k�ʒu�N���A
		m_PPTotalCount[nID] = 0;		// ���k���v���N���A
		m_TrendDataPP[nID].spaceData.clear();		// ���k���X�g�N���A
		m_TrendDataPP[nID].spaceData.shrink_to_fit();	// ���k���X�g�̃������N���A
		m_bIsDrawOn[nID] = TRUE;		// �`��ON
	}

	m_DrawLines[nID] = line;

	// ���c���̍ő�l�ŏ��l�X�V
	if (m_DrawLines[nID].nVScaleKind == LScale)
	{
		if (m_DrawLines[nID].nMaxValue > m_dTotalScaleLMax)
		{
			m_dTotalScaleLMax = m_DrawLines[nID].nMaxValue;
		}
		if (m_DrawLines[nID].nMinValue < m_dTotalScaleLMin)
		{
			m_dTotalScaleLMin = m_DrawLines[nID].nMinValue;
		}
	}

	m_bExitData = FALSE;

	CGraphMutex::Unlock(eData);
	MutexUnlock();

	CString	str;
	if (m_DrawLines[nID].strGName.IsEmpty() == false && m_DrawLines[nID].strName.IsEmpty() == false)
	{
		str = m_DrawLines[nID].strGName;
	}
	else if (m_DrawLines[nID].strName.IsEmpty() == false)
	{
		str = m_DrawLines[nID].strName;
	}
	else
	{
		str = _T("");
	}

	if (m_StationIndex == eStation_Usuda642)
	{
		// 54m
		if (m_txtSigName[nID].GetSafeHwnd() != NULL)
		{
			if (str.IsEmpty())
			{
				m_txtSigName[nID].ShowWindow(SW_HIDE);
			}
			else
			{
				m_txtSigName[nID].SetWindowTextW(str);
				m_txtSigName[nID].ShowWindow(SW_SHOW);
			}
		}
	}
	else
	{
		// 54m�ȊO
		if (m_chkBtnViewSig[nID].GetSafeHwnd() != NULL)
		{
			if (str.IsEmpty())
			{
				m_chkBtnViewSig[nID].ShowWindow(SW_HIDE);
			}
			else
			{
				m_chkBtnViewSig[nID].SetWindowTextW(str);
				m_chkBtnViewSig[nID].ShowWindow(SW_SHOW);
				CPropertiesData& prop = theApp.GetSatelliteData().GetTrendPropertiesData(m_nPropertiesID);
				if (prop.isViewMode[nID])
				{
					m_chkBtnViewSig[nID].SetCheck(BST_CHECKED);
				}
				else
				{
					m_chkBtnViewSig[nID].SetCheck(BST_UNCHECKED);
				}
			}
		}
	}
	return TRUE;
}

/*============================================================================*/
/*! �O���t�`��

-# �������f�[�^�̒ǉ�

@param nID		���C���h�c
@param pParam	�p�����[�^
@param nGraphKind	�O���t���
@param nStyle	���C���X�^�C��
@retval	bool

*/
/*============================================================================*/
BOOL CGraphWindow::AddLine(UINT nID, CPropertiesData* pParam, UINT nGraphKind, DWORD nStyle)
{
	// ���X�^�C�����ُ�̏ꍇ�̓f�t�H���g�ݒ�
	if (!((PS_STYLE_MASK)& nStyle))
		nStyle |= PS_SOLID;

	structLine line;

	line.nDrawKind = Line;
	line.bIsVisible = pParam->isViewMode[nID];
	line.crColor = pParam->color[nID];
	line.nStyle = nStyle;
	line.nWidth = pParam->lineWidth[nID];
	line.nDotSize = 0;
	line.nGraphKind = nGraphKind;
	line.strName = pParam->dataName[nID];
	line.strGName = pParam->dataGName[nID];
	line.strUnit = pParam->dataUnit[nID];
	line.nDecDidits = pParam->nDecDidits[nID];
	line.nVScaleKind = pParam->vscaleKind[nID];
	if (line.strName.IsEmpty())
	{
		line.nListIdx = -1;
	}
	else
	{
		line.nListIdx = theApp.GetDataCollection().GetGraphSigIndex(m_StationIndex, line.strName);
	}

	// ��ID���̏��}�b�v�Ƀ��C���ǉ�
	AddLineToMap(nID, line);

	return TRUE;
}
/*============================================================================*/
/*! �O���t�`��

-# �_�����f�[�^�̒ǉ�

@param nID		���C���h�c
@param pParam	�p�����[�^
@param nGraphKind	�O���t���
@retval	bool

*/
/*============================================================================*/
BOOL CGraphWindow::AddDot(UINT nID, CPropertiesData* pParam, UINT nGraphKind)
{
	// �_�X�^�C�����ُ�̏ꍇ�̓f�t�H���g�ݒ�
	if (!((PS_STYLE_MASK)& nGraphKind))
		nGraphKind |= PS_SOLID;

	structLine line;

	line.nDrawKind = Dots;
	line.bIsVisible = pParam->isViewMode[nID];
	line.crColor = pParam->color[nID];
	line.nStyle = PS_NULL;
	line.nWidth = 0;
	line.nDotSize = GetDotSize(pParam->dotSize[nID]);
	line.nGraphKind = nGraphKind;
	line.strName = pParam->dataName[nID];
	line.strGName = pParam->dataGName[nID];
	line.strUnit = pParam->dataUnit[nID];
	line.nDecDidits = pParam->nDecDidits[nID];
	line.nVScaleKind = pParam->vscaleKind[nID];
	if (line.strName.IsEmpty())
	{
		line.nListIdx = -1;
	}
	else
	{
		line.nListIdx = theApp.GetDataCollection().GetGraphSigIndex(m_StationIndex, line.strName);
	}

	// ��ID���̏��}�b�v�Ƀ��C���ǉ�
	AddLineToMap(nID, line);

	return TRUE;
}
/*============================================================================*/
/*! �O���t�`��

-# �����_�����f�[�^�̒ǉ�

@param nID		���C���h�c
@param pParam	�p�����[�^
@param nGraphKind	�O���t���
@param nStyle	���C���X�^�C��
@retval	bool

*/
/*============================================================================*/
BOOL CGraphWindow::AddLineAndDot(UINT nID, CPropertiesData* pParam, UINT nGraphKind, DWORD nStyle)
{
	// ���X�^�C�����ُ�̏ꍇ�̓f�t�H���g�ݒ�
	if (!((PS_STYLE_MASK)& nStyle))
		nStyle |= PS_SOLID;

	structLine line;

	line.nDrawKind = LineAndDots;
	line.bIsVisible = pParam->isViewMode[nID];
	line.crColor = pParam->color[nID];
	line.nStyle = nStyle;
	line.nWidth = pParam->lineWidth[nID];
	line.nDotSize = GetDotSize(pParam->dotSize[nID]);
	line.nGraphKind = nGraphKind;
	line.strName = pParam->dataName[nID];
	line.strGName = pParam->dataGName[nID];
	line.strUnit = pParam->dataUnit[nID];
	line.nDecDidits = pParam->nDecDidits[nID];
	line.nVScaleKind = pParam->vscaleKind[nID];
	if (line.strName.IsEmpty())
	{
		line.nListIdx = -1;
	}
	else
	{
		line.nListIdx = theApp.GetDataCollection().GetGraphSigIndex(m_StationIndex, line.strName);
	}

	// ��ID���̏��}�b�v�Ƀ��C���ǉ�
	AddLineToMap(nID, line);

	return TRUE;
}
/*============================================================================*/
/*! �O���t�`��

-# �c���̒ǉ�

@param nScaleKind	�c���X�P�[���I���i�E/���j
@param nStvScale	�X�P�[��
@param nVAxisScaleIntvl	�������̊Ԋu
@retval	bool

*/
/*============================================================================*/
BOOL CGraphWindow::AddAxis(stVScale nStvScale, HScale trandHScale)
{
	// �O��l���T���Ă���
	HScale		nOldTrandHScale;
	nOldTrandHScale = (HScale)m_nTrandHScale;

	m_nStvScale = nStvScale;
	if (m_nStvScale.Auto == TRUE ||
		m_nStvScale.VScalLMax > m_nStvScale.VScalLMin)
	{
		m_bUseVAxis[LScale] = TRUE;
	}
	else
	{
		m_bUseVAxis[LScale] = FALSE;
	}
	if (m_nStvScale.VScalRMax > m_nStvScale.VScalRMin)
	{
		m_bUseVAxis[RScale] = TRUE;
	}
	else
	{
		m_bUseVAxis[RScale] = FALSE;
	}

//	if (trandHScale == autoscale || m_nTrandHScale != trandHScale)
	if (m_nTrandHScale != trandHScale)
	{
		m_nTrendDataPos = 0;
	}

	m_nTrandHScale = (UINT)trandHScale;

	if (m_nTrandHScale == yohochiscale && theApp.GetSatelliteData().m_trendData.m_gst_dspinf.i_yohouti_umu == YOHOUTI_ARI)
	{
		double dbHzRange = theApp.GetSatelliteData().m_trendData.m_gst_dspinf.d_LOS_time - theApp.GetSatelliteData().m_trendData.m_gst_dspinf.d_AOS_time;
		if (dbHzRange > 0)
		{
			// �����̈�ݒ�
			SetHzRange(dbHzRange + 1);
		}
	}
	else
	{
		// �����̈�ݒ�
		SetHzRange(mHScale[trandHScale] + 1);
	}

	// �O��Ɖ����ݒ肪�ς�����ꍇ�͏�����
	if (nOldTrandHScale != (HScale)m_nTrandHScale)
	{
#ifdef _DEBUG
		{
			CString str;
			str.Format(_T("%s(%d) :: Change scale : GraphType = %u, ViewID = %u"), __FILEW__, __LINE__, m_nGraphType, m_nPropertiesID);
			CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("AddAxis"), _T("debug"), str, nLogEx::debug);
		}
#endif
		InitSatelliteDataSetting();
	}

	return TRUE;
}


/*============================================================================*/
/*! �O���t�`��

-# �����_�����f�[�^�̍폜

@param nID		���C���h�c

@retval

*/
/*============================================================================*/
//void CGraphWindow::ClearLine(UINT nID)
void CGraphWindow::ClearLineAndDot(UINT nID)
{
	MutexLock();
	if (nID == 0xffffffff){
		// �S�폜
		m_DrawLines.clear();
	}
	else{
		if (m_DrawLines.find(nID) != m_DrawLines.end()){
			m_DrawLines.erase(nID);
		}
	}
	MutexUnlock();
}
/*============================================================================*/
/*! �O���t�`��

-# �O���t�̍X�V

@param	bAll�FTRUE �S�X�V / FALSE �O���t�`��̂ݍX�V
@retval

*/
/*============================================================================*/
void CGraphWindow::UpdateGraph(BOOL bAll /*= FALSE*/)
{
	ClearCursor();
	Normalize();

	if (!bAll)
	{
		// �O���t�̂ݍĕ`��
		CRect rectClient;
		GetClientRect(rectClient);
		CRect rectGrid;
		if (m_bViewDetail)
		{
			// �ϕ��{�^���̊Ď��擾
			UpdateIntegKanshi();

			// �ڍו\�����̎��̈�
			rectGrid.SetRect(
				rectClient.left,
				rectClient.top,
				rectClient.right - mGridMarginRight,
				rectClient.bottom - mGridMarginBottom);
			InvalidateRect(rectGrid);
			rectClient.top = rectGrid.bottom;
			rectClient.right = rectGrid.right + mRightMargin;
			InvalidateRect(rectClient);
			InvalidateRect(m_rectCurValArea);
		}
		else
		{
			// �ȈՕ\�����̎��̈�
			rectGrid = rectClient;
			InvalidateRect(rectGrid);
		}
	}
	else
	{
		// �ϕ��{�^���̊Ď��擾
		UpdateIntegKanshi();

		// �S�ĕ`��
		Invalidate();
	}
}


void CGraphWindow::ResetCursor(int pos)
{
	CRect crect;
	GetClientRect(crect);

	CDrawTool::c_drawShape = line;
	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	if (pTool == NULL){
		CDrawTool::c_drawShape = selection;
		return;
	}

	if (pos < 0){
		Remove(objline);
		CRect r = crect;
		r.right = r.left;
		pTool->Create(this, r);
		CDrawTool::c_drawShape = selection;
		return;
	}

	Select(objline);

	pTool->MoveLineCursor(this, CPoint(crect.left + pos, crect.top));
	CDrawTool::c_drawShape = selection;
}

void CGraphWindow::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_nGraphType == eGraphType_All){
		if (nFlags & MK_CONTROL){
			CRect	rect;
			GetClientRect(rect);
			if (rect.PtInRect(point)){
				if (theApp.GetTrendScrollMode(m_nViewNo) == false){
					// ��̓��[�h�˓�
					theApp.SetTrendScrollMode(m_nViewNo, true);
					m_ptTrendMove = point;
					//SetCapture();
					//SetCursor(theApp.LoadStandardCursor(IDC_HAND));
				}
				else{
					// ��̓��[�h����
					//if (this->GetCapture() == this)
					{
						theApp.SetTrendScrollMode(m_nViewNo, false);
						//ReleaseCapture();
						//SetCursor(theApp.LoadStandardCursor(IDC_ARROW));
					}
				}
			}
		}
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CGraphWindow::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_nGraphType == eGraphType_All){
		if (theApp.GetTrendScrollMode(m_nViewNo) == true){
			CRect	rect;
			GetClientRect(rect);
//			if (rect.PtInRect(point) && m_nTrandHScale != autoscale){
			if (rect.PtInRect(point)){
				m_ptTrendMove = point;
			}
		}
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	if (m_nGraphType == eGraphType_OffAll){
		// �Y�[�����[�h���̏���
		if (nFlags & MK_CONTROL){
			// Ctrl�L�[�ƈꏏ�̃}�E�X�C�x���g�̓Y�[���g���쐬
			// �Y�[���J�[�\��
			Select(objrect);
			CDrawTool::c_drawShape = selection;
			// �}�E�X�J�[�\���ʒu���Y�[���g���ɂ��邩�̃`�F�b�N
			bool bsel = false;
			if (m_selection.GetCount() != 0){
				CDrawObject* pObj = m_selection.GetHead();
				if (pObj != NULL){
					CPoint local = point;
					ClientToDoc(local);
					int nHandle = pObj->HitTest(local, this, TRUE);
					if (nHandle == 0)
						bsel = true;
				}
			}
			if (bsel == false){
				// �����Y�[���g���I����ԂłȂ��̂ŁA�����Y�[���g���폜����B
				// ���̌�A�V�K�ɃY�[���g���쐬����
				Select(objrect);
				Remove(objrect);
				CDrawTool::c_drawShape = rect;
			}
			CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
			if (pTool != NULL){
				pTool->OnLButtonDown(this, nFlags, point);
			}
			return;
		}
		else{
			// �}�E�X�J�[�\���ʒu���J�[�\���g���ɂ��邩�̃`�F�b�N
			bool bsel = false;
			if (m_selection.GetCount() != 0){
				CDrawObject* pObj = m_selection.GetHead();
				if (pObj != NULL){
					CPoint local = point;
					ClientToDoc(local);
					int nHandle = pObj->HitTest(local, this, TRUE);
					if (nHandle == 0)
						bsel = true;
				}
			}
			if (bsel == false){
				// �J�[�\�쐬
				ResetCursor(point.x);
				return;
			}
			CDrawTool::c_drawShape = selection;
			CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
			if (pTool != NULL){
				pTool->OnLButtonDown(this, nFlags, point);
			}
			return;
		}
	}

	if (m_nGraphType == eGraphType_OffTrend){
		if (nFlags & MK_CONTROL && !(nFlags & MK_SHIFT)){
			int inc = GetTrendZoomInc();
			SetTrendZoomInc(inc * 2);
			OnZoom(true);
		}
		else if (nFlags & MK_CONTROL && nFlags & MK_SHIFT){
			int inc = GetTrendZoomInc();
			inc /= 2;
			if (inc == 0)	inc = 1;
			SetTrendZoomInc(inc);
			OnZoom(false);
		}
		else{
			SetFocus();
			int pos = (int)(m_pointTop / m_nTrendInc);
			if (pos != 0)
				pos--;
			int endpos = (int)((m_dbHzRange + m_pointTop) / m_nTrendInc);
			pair<double, double> pnt;
			CPoint pointDraw;
			DRAWLINE::const_iterator iterLine = m_DrawLines.begin();
			m_ZoomPos = CPoint(0, 0);
			while ((pos != endpos + 2) && (pos != (int)m_dbHzRange)){
				pnt.first = (double)((double)pos*m_nTrendInc); pnt.second = (double)0;
				pointDraw = ConvPoint(pnt, (*iterLine).second.nMaxValue, (*iterLine).second.nMinValue);
				if (pointDraw.x >= point.x){
					m_CursorPos = pos;
					if (m_CursorPos < 1)
						m_CursorPos = 1;
					//TRACE("@@@ Trend Cursor[%d:%d]\n", m_CursorPos, pos);
					Invalidate();
					break;
				}
				pos++;
			}
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}


void CGraphWindow::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_nGraphType == eGraphType_All){
		if (theApp.GetTrendScrollMode(m_nViewNo) == true){
			if (nFlags & MK_LBUTTON){
				CRect	rect;
				GetClientRect(rect);
				int size = (int)GetPPDataMaxSize();
				if (size != 0 && rect.PtInRect(point)){
					if (m_ptTrendMove.x >= point.x){
						// �E����
						m_nTrendDataPos += 1;
						if (size <= m_nTrendDataPos)
							m_nTrendDataPos = (size - 1);
					}
					else{
						// ������
						if ((int)((int)m_nTrendDataPos - 1) < 0)
							m_nTrendDataPos = 0;
						else
							m_nTrendDataPos -= 1;
					}
					UpdateGraph();
					m_ptTrendMove = point;
				}
			}
		}
		CWnd::OnMouseMove(nFlags, point);
		return;
	}

	CRect	rect;
	GetClientRect(rect);
	if (rect.left > point.x){
		point.x = rect.left;
	}
	if (rect.right < point.x){
		point.x = rect.right;
	}

	if (m_nGraphType == eGraphType_OffAll){
		CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
		if (pTool != NULL)
			pTool->OnMouseMove(this, nFlags, point);
		return;
	}

	CWnd::OnMouseMove(nFlags, point);
}


void CGraphWindow::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_nGraphType == eGraphType_All){
		CWnd::OnLButtonUp(nFlags, point);
		return;
	}
	if (m_nGraphType == eGraphType_OffAll){
		CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
		if (pTool != NULL)
			pTool->OnLButtonUp(this, nFlags, point);

		CDrawTool::c_drawShape = selection;
		return;
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CGraphWindow::DrawZoom(CDC* pDC, CRect drect)
{
	CRect rect = drect;
	//---------------------------------------------------------------------------
	//	�I�����ꂽ�̈�̘g���y���`��
	//---------------------------------------------------------------------------
	if (rect.right < 0)
		return;
	rect.DeflateRect(mZoomWidth - 2, mZoomWidth - 2);
	CPen pen;
	COLORREF cf = mZoomColor;
	if (pen.CreatePen(PS_SOLID, mZoomWidth, cf)){
		int old = pDC->SetROP2(R2_XORPEN);
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		if (rect.left >= rect.right - (LONG)mZoomWidth){
			pDC->MoveTo(rect.left, rect.top);
			pDC->LineTo(rect.left, rect.bottom);
		}
		else{
			pDC->MoveTo(rect.left, rect.top);
			pDC->LineTo(rect.left, rect.bottom);
			pDC->MoveTo(rect.right, rect.top);
			pDC->LineTo(rect.right, rect.bottom);

			pDC->MoveTo(rect.left, rect.top);
			pDC->LineTo(rect.right, rect.top);
			pDC->MoveTo(rect.left, rect.top);
			pDC->LineTo(rect.left, rect.top);
			pDC->MoveTo(rect.right, rect.top);
			pDC->LineTo(rect.right, rect.top);

			pDC->MoveTo(rect.left, rect.bottom);
			pDC->LineTo(rect.right, rect.bottom);
			pDC->MoveTo(rect.left, rect.bottom);
			pDC->LineTo(rect.left, rect.bottom);
			pDC->MoveTo(rect.right, rect.bottom);
			pDC->LineTo(rect.right, rect.bottom);
		}
		pDC->SetROP2(old);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}
}


void CGraphWindow::AjustLayout()
{
	if (m_btnSetting.GetSafeHwnd() != NULL)
	{
		// �ڍו\���̏ꍇ�̂ݐݒ�{�^���\��
		if (m_bViewDetail)
		{
			m_btnSetting.ShowWindow(SW_SHOW);
		}
		else
		{
			m_btnSetting.ShowWindow(SW_HIDE);
		}

		// �ݒ�{�^���ړ�
		CRect rcBtn;
		GetClientRect(rcBtn);
		rcBtn.right -= mBtnSettingMarginRight;
		rcBtn.left = rcBtn.right - mBtnWidth;
		rcBtn.top += mBtnSettingMarginTop;
		rcBtn.bottom = rcBtn.top + mBtnHeight;

		if (m_btnSetting.GetSafeHwnd() != NULL)
			m_btnSetting.MoveWindow(rcBtn);
	}

	if (m_btnHGridChk.GetSafeHwnd() != NULL)
	{
		// �ڍו\���̏ꍇ�̂݉��r���\��ON/OFF�`�F�b�N�{�b�N�X�\��
		if (m_bViewDetail)
		{
			m_btnHGridChk.ShowWindow(SW_SHOW);
		}
		else
		{
			m_btnHGridChk.ShowWindow(SW_HIDE);
		}

		// ���r���\��ON/OFF�`�F�b�N�{�b�N�X�ړ�
		CRect rcBtn;
		GetClientRect(rcBtn);
		rcBtn.left = rcBtn.right - mGridMarginRight + mChkBtnMarginRight;
		rcBtn.right = rcBtn.left + mChkBtnWidth;
		rcBtn.top += mBtnSettingMarginTop;
		rcBtn.bottom = rcBtn.top + mChkBtnHeight;

		if (m_btnHGridChk.GetSafeHwnd() != NULL)
			m_btnHGridChk.MoveWindow(rcBtn);
	}

	CRect rect;
	GetClientRect(rect);
	rect.SetRect(
		rect.left,
		rect.top + mGridMarginTop,
		rect.right,
		rect.bottom - mGridMarginBottom + 1);

	// �M���\���`�F�b�N�{�^���\��
	if (m_StationIndex == eStation_Usuda642)
	{
		// 54m
		for (int i = 0; i < MAX_TDATA; i++)
		{
			if (m_txtSigName[i].GetSafeHwnd() != NULL)
			{
				// �ڍו\���̏ꍇ�̂݉��r���\��ON/OFF�`�F�b�N�{�b�N�X�\��
				if (m_bViewDetail && m_DrawLines.size() > i && !m_DrawLines[i].strGName.IsEmpty())
				{
					m_txtSigName[i].ShowWindow(SW_SHOW);
				}
				else
				{
					m_txtSigName[i].ShowWindow(SW_HIDE);
				}

				// ���r���\��ON/OFF�`�F�b�N�{�b�N�X�ړ�
				CRect rcSigBtn = rect;
				CString strSigBtn;
				CPoint ptSigName;
				ptSigName.x = rcSigBtn.right - mGridMarginRight + mRightMargin + mLegendWeidth + mLegendLineStrDistance;
				ptSigName.y = rcSigBtn.top + mTopMargin + mSigStrHeight * i;
				rcSigBtn.left = ptSigName.x;
				rcSigBtn.right = rcSigBtn.left + mChkBtnWidth - mHStrDistance;
				rcSigBtn.top = ptSigName.y;
				rcSigBtn.bottom = rcSigBtn.top + 12;

				if (m_txtSigName[i].GetSafeHwnd() != NULL)
					m_txtSigName[i].MoveWindow(rcSigBtn);
			}
		}
	}
	else
	{
		// 54m�ȊO
		for (int i = 0; i < MAX_TDATA; i++)
		{
			if (m_chkBtnViewSig[i].GetSafeHwnd() != NULL)
			{
				// �ڍו\���̏ꍇ�̂݉��r���\��ON/OFF�`�F�b�N�{�b�N�X�\��
				if (m_bViewDetail && m_DrawLines.size() > i && !m_DrawLines[i].strGName.IsEmpty())
				{
					m_chkBtnViewSig[i].ShowWindow(SW_SHOW);
				}
				else
				{
					m_chkBtnViewSig[i].ShowWindow(SW_HIDE);
				}

				// ���r���\��ON/OFF�`�F�b�N�{�b�N�X�ړ�
				CRect rcSigBtn = rect;
				CString strSigBtn;
				CPoint ptSigName;
				ptSigName.x = rcSigBtn.right - mGridMarginRight + mRightMargin + mLegendWeidth + mLegendLineStrDistance;
				ptSigName.y = rcSigBtn.top + mTopMargin + mSigStrHeight * i;
				rcSigBtn.left = ptSigName.x;
				rcSigBtn.right = rcSigBtn.left + mChkBtnWidth - mHStrDistance;
				rcSigBtn.top = ptSigName.y;
				rcSigBtn.bottom = rcSigBtn.top + 12;

				if (m_chkBtnViewSig[i].GetSafeHwnd() != NULL)
					m_chkBtnViewSig[i].MoveWindow(rcSigBtn);
			}
		}
	}

	if (m_btnIntg.GetSafeHwnd() != NULL)
	{

		// �ڍו\���̏ꍇ�̂ݐϕ����ԃ{�^���\��
		if (m_bViewDetail)
		{
			m_btnIntg.ShowWindow(SW_SHOW);
		}
		else
		{
			m_btnIntg.ShowWindow(SW_HIDE);
		}

		// �ϕ����ԃ{�^���ړ�
		CRect rcBtn;
		GetClientRect(rcBtn);
		m_rectIntgBtn.right = rcBtn.right - mBtnIntgMarginRight;
		m_rectIntgBtn.left = m_rectIntgBtn.right - mBtnWidth;
		if (m_btnIntg.GetSafeHwnd() != NULL)
			m_btnIntg.MoveWindow(m_rectIntgBtn);
	}

	ClearCursor();
}



void CGraphWindow::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	AjustLayout();
}



/*============================================================================*/
/*! �O���t�`��

-# �X�N���[�����̍X�V

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::UpdateScrollInfo()
{
	//if (m_nGraphType != eGraphType_OffTrend){
	//	return;
	//}

	int size = (int)GetPPDataMaxSize(); //�֐�������A�f�[�^�����郊�X�g�̃T�C�Y���擾
	if (size == 0)
	{
		return;
	}
#ifdef _DEBUG
	CString	str;
	str.Format(_T("m_nTrendDataPos = %I64d, size = %d"), m_nTrendDataPos, size);
	CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("UpdateScrollInfo"), _T("debug"), str, nLogEx::debug);
#endif

	SCROLLINFO si;

	si.cbSize = sizeof(SCROLLINFO);
	TRY
	{
		si.fMask = SIF_ALL;
		si.nPos = (int)m_nTrendDataPos;
		si.nMin = 0;
		si.nMax = size;
		si.nPage = mHScale[m_nTrandHScale];
	}CATCH_ALL(e){
		return;
	} END_CATCH_ALL

	SetScrollInfo(SB_HORZ, &si, TRUE);
	SendNewLimits();
}

/*============================================================================*/
/*! �O���t�`��

-# ���[���������s��

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::SendNewLimits()
{
	return;
	GetParent()->PostMessage(WM_NEWHORZLIMITS,
		(WPARAM)m_nTrendDataPos,
		(LPARAM)(m_nTrendDataPos + m_dbHzRange));
//	GetParent()->PostMessage(WM_NEWVERTLIMITS,
//		(WPARAM)m_dbMinValue, (LPARAM)m_dbMaxValue);
	GetParent()->PostMessage(WM_NEWVERTLIMITS,
		(WPARAM)GetVAxisMin(), (LPARAM)GetVAxisMax());
}


void CGraphWindow::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int size = (int)GetPPDataMaxSize();

	switch (nSBCode) {

	case SB_LINELEFT:
		if (m_nTrendDataPos < 1)
			return;
		m_nTrendDataPos -= 1;
		if (m_nTrendDataPos < 0)
			m_nTrendDataPos = 0;
		break;

	case SB_LINERIGHT:
		m_nTrendDataPos += 1;
		if (size <= m_nTrendDataPos)
			m_nTrendDataPos = (size - 1);
		break;

	case SB_PAGEUP:
		if (m_nTrendDataPos < 10)
			return;
		m_nTrendDataPos -= 10;
		if (m_nTrendDataPos < 0)
			m_nTrendDataPos = 0;
		break;

	case SB_PAGERIGHT:
		m_nTrendDataPos += 10;
		if (size <= m_nTrendDataPos)
			m_nTrendDataPos = (size - 1);
		break;

	case SB_THUMBTRACK:
		m_nTrendDataPos = nPos;
		if (size <= m_nTrendDataPos)
			m_nTrendDataPos = (size - 1);
		break;
	default:
		return;
	}

	Invalidate();
	UpdateScrollInfo();
//	UpdateWindow();

	//CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}
/*============================================================================*/
/*! �O���t�`��

-# ����ʒu�̍X�V

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::Normalize()
{
	//if (m_nGraphType != eGraphType_OffTrend){
	//	return;
	//}
	if (m_pointTop < m_dbMinHzVal)
		m_pointTop = m_dbMinHzVal;
	if (m_dbMaxHzVal < m_pointTop + m_dbHzRange)
		m_dbMaxHzVal = m_pointTop + m_dbHzRange;
}

/*============================================================================*/
/*! �O���t�`��

-# �Y�[���C�x���g

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::OnZoom(bool bZoomIn)
{
	if (m_nGraphType != eGraphType_OffTrend){
		return;
	}

	int pos = (int)(m_pointTop / m_nTrendInc);
	int endpos = (int)((m_dbHzRange + m_pointTop) / m_nTrendInc);
	int cpos = m_CursorPos - ((endpos - pos) / 2) + 1;
	m_pointTop = (double)((double)(m_CursorPos - 1)*m_nTrendInc) - (m_dbHzRange / 2.0);
	double dbTrendPos = (double)((double)cpos*m_nTrendInc);
	if (dbTrendPos < m_pointTop || dbTrendPos >(m_dbHzRange + m_pointTop)){
		// �J�[�\�����g�O�̂��߃X�N���[�������X�V����
		if (dbTrendPos < m_pointTop){
			m_pointTop = dbTrendPos;
		}
		if (dbTrendPos >(m_dbHzRange + m_pointTop)){
			m_pointTop = dbTrendPos - m_dbHzRange;
		}
	}
	if (m_pointTop < 0)
		m_pointTop = 0;

	//m_MaxHzVal = (double)(((size-1)*m_nTrendInc) + m_nHzRange * .1);
	m_dbMaxHzVal = (double)((double)((double)(m_dbHzRange - 1)*m_nTrendInc) + (double)m_dbHzRange * (double).1);
	Normalize();

	Invalidate();
	UpdateScrollInfo();
//	UpdateWindow();
}



void CGraphWindow::ClientToDoc(CPoint& point)
{
}

void CGraphWindow::ClientToDoc(CRect& rect)
{
	CRect rc;
	GetClientRect(rc);
	rect.top = rc.top;
	rect.bottom = rc.bottom;
	if (rc.left > rect.left)
		rect.left = rc.left;
	if (rc.right < rect.right)
		rect.right = rc.right;
}

void CGraphWindow::DocToClient(CPoint& point)
{
}

void CGraphWindow::DocToClient(CRect& rect)
{
	rect.NormalizeRect();
}

void CGraphWindow::InvalObj(CDrawObject* pObj)
{
	CRect rect = pObj->m_position;
	DocToClient(rect);
	rect.InflateRect(1, 1);

	InvalidateRect(rect, FALSE);
}

/*============================================================================*/
/*! �C���[�W�c�[��

-# ���ڂ̑I������

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::Deselect(CDrawObject* pObj)
{
	POSITION pos = m_selection.Find(pObj);
	if (pos != NULL){
		InvalObj(pObj);
		m_selection.RemoveAt(pos);
	}
}
/*============================================================================*/
/*! �C���[�W�c�[��

-# �I�u�W�F�N�g�̎擾

@param
@retval

*/
/*============================================================================*/
CDrawObject* CGraphWindow::ObjectAt(const CPoint& point)
{
	CRect rect(point, CSize(1, 1));
	POSITION pos = m_objects.GetTailPosition();
	while (pos != NULL)
	{
		CDrawObject* pObj = m_objects.GetPrev(pos);
		if (pObj->Intersects(rect))
			return pObj;
	}

	return NULL;
}

BOOL CGraphWindow::IsSelected(const CObject* pDocItem) const
{
	CDrawObject* pDrawObj = (CDrawObject*)pDocItem;
	return m_selection.Find(pDrawObj) != NULL;
}

/*============================================================================*/
/*! �C���[�W�c�[��

-# ���ڂ̑I��

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::Select(CDrawObject* pObj, BOOL bAdd)
{
	if (!bAdd){
		OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		m_selection.RemoveAll();
	}

	if (pObj == NULL || IsSelected(pObj)){
		return;
	}

	m_selection.AddTail(pObj);
	InvalObj(pObj);
}

void CGraphWindow::Select(ObjectShape type)
{
	POSITION pt = m_objects.GetHeadPosition();
	while (pt != NULL){
		CDrawObject* pObj = m_objects.GetNext(pt);
		if (pObj->GetShape() == type){
			Select(pObj);
		}
	}
}

/*============================================================================*/
/*! �C���[�W�c�[��

-# ��`�ɂ�鍀�ڂ̑I��

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::SelectWithinRect(CRect rect, BOOL bAdd)
{
	if (!bAdd)
		Select(NULL);

	ClientToDoc(rect);

	CDrawObjList* pObList = &m_objects;
	POSITION posObj = pObList->GetHeadPosition();
	while (posObj != NULL){
		CDrawObject* pObj = pObList->GetNext(posObj);
		if (pObj->Intersects(rect))
			Select(pObj, TRUE);
	}
}

/*============================================================================*/
/*! �C���[�W�c�[��

-# �I�u�W�F�N�g�̒ǉ�

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::Add(CDrawObject* pObj)
{
	m_objects.AddTail(pObj);
	//pObj->m_pDocument = this;
}

/*============================================================================*/
/*! �C���[�W�c�[��

-# �I�u�W�F�N�g�̍폜

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::Remove(CDrawObject* pObj)
{
	POSITION pos = m_objects.Find(pObj);
	if (pos != NULL)
		m_objects.RemoveAt(pos);

	pos = m_selection.Find(pObj);
	if (pos != NULL){
		InvalObj(pObj);
		m_selection.RemoveAt(pos);
	}
}

void CGraphWindow::Remove(ObjectShape type)
{
	CDrawTool::c_drawShape = selection;
	if (type == objrect)
		CDrawTool::c_drawShape = rect;
	if (type == objline)
		CDrawTool::c_drawShape = line;

	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	if (pTool == NULL){
		CDrawTool::c_drawShape = selection;
		return;
	}

	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL){
		CDrawObject* pObj = m_objects.GetNext(pos);
		if (pObj->GetShape() == type){
			Remove(pObj);
			pObj->Remove();
		}
	}
}

/*============================================================================*/
/*! �C���[�W�c�[��

-# �I�u�W�F�N�g�̑S�폜

@param
@retval

*/
/*============================================================================*/
void CGraphWindow::RemoveAll(bool bInit)
{
	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL){
		CDrawObject* pObj = m_objects.GetNext(pos);
		if (bInit == false){
			UINT type = ((CDrawRect*)pObj)->GetShape();
			if (((CDrawRect*)pObj)->IsFunctionNone() == FALSE && ((CDrawRect*)pObj)->IsFunction() == FALSE)
				continue;
		}
		Remove(pObj);
		pObj->Remove();
	}
}
void CGraphWindow::OnUpdate(CView*, LPARAM lHint, CObject* pHint)
{
	switch (lHint)
	{
	case HINT_UPDATE_WINDOW:
		Invalidate(FALSE);
		break;

	case HINT_UPDATE_DRAWOBJ:
		InvalObj((CDrawObject*)pHint);
		break;

	case HINT_UPDATE_SELECTION:
	{
								  CDrawObjList* pList = pHint != NULL ?
									  (CDrawObjList*)pHint : &m_selection;
								  POSITION pos = pList->GetHeadPosition();
								  while (pos != NULL)
								  {
									  InvalObj(pList->GetNext(pos));
								  }
	}
		break;

	case HINT_DELETE_SELECTION:
		if (pHint != &m_selection)
		{
			CDrawObjList* pList = (CDrawObjList*)pHint;
			POSITION pos = pList->GetHeadPosition();
			while (pos != NULL)
			{
				CDrawObject* pObj = pList->GetNext(pos);
				InvalObj(pObj);
				Remove(pObj);
			}
		}
		break;

	default:
		ASSERT(FALSE);
		break;
	}
}
void CGraphWindow::DrawObj(CDC* pDC)
{
	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObject* pObj = m_objects.GetNext(pos);
		pObj->Draw(pDC, FALSE);
	}
}

// �h�b�g�T�C�Y���擾����
int CGraphWindow::GetDotSize(int nDotSize)
{
	switch (nDotSize)
	{
	case Medium:
		return eDotSize_MEDIUM;
	case Large:
		return eDotSize_LARGE;
	case Small:
	default:
		return eDotSize_SMALL;
	}

	return 0;
}

/*============================================================================*/
/*! �O���t�`��

-# �c�����[�ƉE�[�ōő�l���擾

@param	�Ȃ�
@retval	�c���̍ő�l

*/
/*============================================================================*/
double CGraphWindow::GetVAxisMax()
{
	double ret = m_nStvScale.VScalLMax;

	if (m_bUseVAxis[RScale] == TRUE)
	{
		if (m_nStvScale.VScalLMax > m_nStvScale.VScalRMax)
		{
			ret = m_nStvScale.VScalLMax;
		}
		else
		{
			ret = m_nStvScale.VScalRMax;
		}
	}

	return ret;
}
/*============================================================================*/
/*! �O���t�`��

-# �c�����[�ƉE�[�ōŏ��l���擾

@param	�Ȃ�
@retval	�c���̍ŏ��l

*/
/*============================================================================*/
double CGraphWindow::GetVAxisMin()
{
	double ret = m_nStvScale.VScalLMin;

	if (m_bUseVAxis[RScale] == TRUE)
	{
		if (m_nStvScale.VScalLMin < m_nStvScale.VScalRMin)
		{
			ret = m_nStvScale.VScalLMin;
		}
		else
		{
			ret = m_nStvScale.VScalRMin;
		}
	}

	return ret;
}


/*============================================================================*/
/*! CGraphWindow

-# ���l�ɃJ���}��ǉ�

@param	pos�F�J���}��t���������l
@param	pointKeta�F�����_�ȉ��̌��� (�����_0�`6)
@retval	�J���}��t������̐��l

*/
/*============================================================================*/
CString CGraphWindow::addCanma(double pos, int pointKeta)
{
	int	i, j, keta, temp;
	int sign = 0;
	double tempPos = pos;
	char str[MAXDIGIT];

	if (isnan(pos)){
		// NaN (��)�̏ꍇ�͂O�ɂ���
		tempPos = 0.0;
	}

	// 0�ɋ߂��ꍇ�͂܂�߂�
	switch (pointKeta)
	{
	case 0:
		if (tempPos > -0.1 && tempPos < 0.1)
		{
			tempPos = 0.0;
		}
		break;
	case 1:
		if (tempPos > -0.01 && tempPos < 0.01)
		{
			tempPos = 0.0;
		}
		break;
	case 2:
		if (tempPos > -0.001 && tempPos < 0.001)
		{
			tempPos = 0.0;
		}
		break;
	case 3:
		if (tempPos > -0.0001 && tempPos < 0.0001)
		{
			tempPos = 0.0;
		}
		break;
	case 4:
		if (tempPos > -0.00001 && tempPos < 0.00001)
		{
			tempPos = 0.0;
		}
		break;
	case 5:
		if (tempPos > -0.000001 && tempPos < 0.000001)
		{
			tempPos = 0.0;
		}
		break;
	case 6:
		if (tempPos > -0.0000001 && tempPos < 0.0000001)
		{
			tempPos = 0.0;
		}
		break;
	default:
		break;
	}

	__int64 num = (__int64)tempPos;
	double dnum = fabs(tempPos - (double)num);
	CString strSyosuu;
	switch (pointKeta)
	{
	case 0:
		strSyosuu.Format(_T(""));
		break;
	case 1:
		strSyosuu.Format(_T("%.1f"), dnum);
		break;
	case 2:
		strSyosuu.Format(_T("%.2f"), dnum);
		break;
	case 3:
		strSyosuu.Format(_T("%.3f"), dnum);
		break;
	case 4:
		strSyosuu.Format(_T("%.4f"), dnum);
		break;
	case 5:
		strSyosuu.Format(_T("%.5f"), dnum);
		break;
	case 6:
		strSyosuu.Format(_T("%.6f"), dnum);
		break;
	default:
		strSyosuu.Format(_T(""));
		break;
	}

	__int64 kuriage = 0;
	if (strSyosuu.Left(1).Compare(_T("1")) == 0)
	{
		kuriage = 1;
	}

	strSyosuu.Delete(0);


	// �����𒲂ׂ�
	if (tempPos < 0)
	{
		sign = 1;
		num = -num;
	}

	// �J�グ�𑫂�
	num = num + kuriage;

	// �������ɃJ���}��t����
	i = 0;
	keta = 0;

	do
	{
		str[i++] = num % 10 + '0';
		keta++;
		num /= 10;
		if (keta % 3 == 0 && num != 0)
		{
			str[i++] = ',';
		}
	} while (num != 0 && i < sizeof(str));

	if (i >= MAXDIGIT)
	{
		return CString(_T("#,###,###,###,###.######"));
	}
	if (sign == 1)
	{
		if (i + 1 >= MAXDIGIT)
		{
			return CString(_T("#,###,###,###,###.######"));
		}
		str[i++] = '-';
	}

	str[i] = '\0';

	j = i - 1;
	for (i = 0; j > i; i++, j--)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
	}

	return CString(str) + strSyosuu;
}


/*============================================================================*/
/*! �O���t�`��

-# ���̐��l���ォ��3���ڂŐ؂�グ/�؂艺������

@param	���͐��l
@param	0:�؂艺�� / 0�ȊO:�؂�グ
@retval ���ʐ��l

*/
/*============================================================================*/
double CGraphWindow::Ceil3Keta(double pos, int dir)
{
	double tempPos = pos * 1000000000;	// �����_6���Ɛ؂�グ��3���ڕ������炷
	long long num = (long long)fabs(tempPos);
	int keta = 0;

	do
	{
		keta++;
		num /= 10;
	} while (num != 0);

	double sft = pow(10, keta - 3);
	tempPos /= sft;

	if (dir != 0)
	{
		tempPos = ceil(tempPos);
	}
	else
	{
		tempPos = floor(tempPos);
	}
	tempPos *= sft;
	tempPos /= 1000000000;

	return tempPos;
}


/*============================================================================*/
/*! �O���t�`��

-# ���̐��l���ォ��͈͒l/100�̌����Ő؂�グ/�؂艺������

@param	���͐��l
@param	0:�؂艺�� / 0�ȊO:�؂�グ
@param	�͈͒l
@retval ���ʐ��l

*/
/*============================================================================*/
double CGraphWindow::CeilKeta(double pos, int dir, double range)
{
	double tempPos = pos;
	double tempRange = range;
	double tempShift = 1.0;

	// �͈͒l�̌�������1���ȏ�̏ꍇ
	if (range >= 1.0 || range <= -1.0)
	{
		__int64 nNum = (__int64)fabs(tempRange);
		while (nNum != 0)
		{
			tempShift *= 10.0;		// �ŏ��P��
			nNum /= 10;				// �������邩
		}

		tempShift /= 100.0;	// �͈͒l/100�̌����𒲂ׂ�
		double sft = tempPos / tempShift;
		// �؂�グ�A�؂艺�������邩���ׂ�
		if (dir != 0)
		{
			sft = ceil(sft);
		}
		else
		{
			sft = floor(sft);
		}

		tempPos = sft * tempShift;
	}
	// �͈͒l�̌��������̏ꍇ
	else
	{
		double dNum = fabs(tempRange / 100);
		while (dNum < 1)
		{
			tempShift *= 10.0;		// �ŏ��P��(����j
			dNum *= 10.0;			// �������邩
		}

		// �������͂����Đ������Ə������ɕ�����
		double upper = fabs((double)((__int64)tempPos));
		double lower = fabs(tempPos) - upper;

		double sft = lower * tempShift;
		// ������t�����؂�グ�A�؂艺�������邩���ׂ�
		if (pos < 0)
		{
			sft = -sft;
		}
		if (dir != 0)
		{
			sft = ceil(sft);
		}
		else
		{
			sft = floor(sft);
		}

		// �������Ə������������A������t������
		tempPos = fabs(sft) / tempShift + upper;
		if (pos < 0)
		{
			tempPos = -tempPos;
		}
	}

	return tempPos;
}


/*============================================================================*/
/*! CDataCollection

-# PP���k�ϊ�

@param	�g���[�X�f�[�^�ԍ�
@param	�g�����h���k�f�[�^�L���[
@param	�Ď��f�[�^
@param	�ŏ��l�ő�l
@retval	�Ȃ�

*/
/*============================================================================*/
void CGraphWindow::ConvertPP(int trace, deque<stSpaceData>& trendPP, stSpaceData newData, structPPMinMax& stPPMinMax)
{
	__int64 number = m_PPNumber[trace];
	// �̈悪���k���E�T�C�Y�𒴂��Ă���A���k���X�g�������ς��̏ꍇ
	if ((m_dbHzRange > MAX_PPDATA_SIZE) && trendPP.size() == MAX_PPDATA_SIZE)
	{
		// ���k����K�v����
		if ((number + 1) == MAX_PPDATA_SIZE){
			// ���k�������I�[�ɒB����
			// �i�[����Ă���Ō�̃f�[�^�ƐV�����ǉ�����f�[�^�����k����
			trendPP[number].ppPair.count += newData.ppPair.count;
			if ((char)trendPP[number].status[0] >= 0 && (char)newData.status[0] >= 0)
			{
				trendPP[number].ppPair.max = __max(trendPP[number].ppPair.max, newData.ppPair.max);
				trendPP[number].ppPair.min = __min(trendPP[number].ppPair.min, newData.ppPair.min);
			}

			// �X�e�[�^�X���X�V����
			if ((char)trendPP[number].status[0] < 0 && (char)newData.status[0] >= 0)
			{
				trendPP[number].status[0] = newData.status[0];
				trendPP[number].ppPair.max = newData.ppPair.max;
				trendPP[number].ppPair.min = newData.ppPair.min;
			}

			// �ŏ��ő���v�Z
			CalcPPMinMaxWithCompareCount(trendPP, number, stPPMinMax);
			// �ʒu��0�N���A
			number = 0;
			// PP���k�L���[�ɓ��ꂽ�Ď��f�[�^�T�C�Y���v
			m_PPTotalCount[trace] += newData.ppPair.count;
		}
		else
		{
			// ���k�������I�[�ɒB���Ă��Ȃ�
			if (trendPP[MAX_PPDATA_SIZE - 2].ppPair.count != trendPP[MAX_PPDATA_SIZE - 1].ppPair.count)
			{
				trendPP[MAX_PPDATA_SIZE - 1].ppPair.count += newData.ppPair.count;
				if ((char)trendPP[MAX_PPDATA_SIZE - 1].status[0] >= 0 && (char)newData.status[0] >= 0)
				{
					trendPP[MAX_PPDATA_SIZE - 1].ppPair.max = __max(trendPP[MAX_PPDATA_SIZE - 1].ppPair.max, newData.ppPair.max);
					trendPP[MAX_PPDATA_SIZE - 1].ppPair.min = __min(trendPP[MAX_PPDATA_SIZE - 1].ppPair.min, newData.ppPair.min);
				}

				// �X�e�[�^�X���X�V����
				if ((char)trendPP[MAX_PPDATA_SIZE - 1].status[0] < 0 && (char)newData.status[0] >= 0)
				{
					trendPP[MAX_PPDATA_SIZE - 1].status[0] = newData.status[0];
					trendPP[MAX_PPDATA_SIZE - 1].ppPair.max = newData.ppPair.max;
					trendPP[MAX_PPDATA_SIZE - 1].ppPair.min = newData.ppPair.min;
				}

				// �ŏ��ő���v�Z
				CalcPPMinMaxWithCompareCount(trendPP, MAX_PPDATA_SIZE - 1, stPPMinMax);
				// PP���k�L���[�ɓ��ꂽ�Ď��f�[�^�T�C�Y���v
				m_PPTotalCount[trace] += newData.ppPair.count;
			}
			else
			{
				trendPP[number].ppPair.count += trendPP[number + 1].ppPair.count;
				if ((char)trendPP[number].status[0] >= 0 && (char)trendPP[number + 1].status[0] >= 0)
				{
					trendPP[number].ppPair.max = __max(trendPP[number].ppPair.max, trendPP[number + 1].ppPair.max);
					trendPP[number].ppPair.min = __min(trendPP[number].ppPair.min, trendPP[number + 1].ppPair.min);
				}
				// �X�e�[�^�X���X�V����
				if ((char)trendPP[number].status[0] < 0 && (char)trendPP[number + 1].status[0] >= 0)
				{
					trendPP[number].status[0] = trendPP[number + 1].status[0];
					trendPP[number].ppPair.max =  trendPP[number + 1].ppPair.max;
					trendPP[number].ppPair.min = trendPP[number + 1].ppPair.min;
				}

				number++;

				// ���k���s�����̂ň��k���ꂽ�f�[�^���폜����
				int pos = 0;
				deque<stSpaceData>::iterator itr;
				for (itr = trendPP.begin(); itr != trendPP.end(); itr++, pos++)
				{
					if (number == pos)
					{
						trendPP.erase(itr);
						break;
					}
				}
				trendPP.push_back(newData);

				__int64	nTailPos = trendPP.size() - 1;
				// �ŏ��ő���v�Z
				CalcPPMinMaxWithCompareCount(trendPP, nTailPos, stPPMinMax);
				// PP���k�L���[�ɓ��ꂽ�Ď��f�[�^�T�C�Y���v
				m_PPTotalCount[trace] += newData.ppPair.count;
			}
		}
	}
	// ���k���X�g����폜��̏���
	else if (trendPP.empty() == FALSE && trendPP[0].ppPair.count > 1)
	{
		// ���k�J�E���g���E����
		if ((number > 0 && trendPP[number - 1].ppPair.count != trendPP[number].ppPair.count) || number == 0)
		{
			// ���k���X�g�̖����̏ꍇ
			if (number == trendPP.size() - 1)
			{
				// �����ɐV�����f�[�^�����k�ǉ�
				trendPP[number].ppPair.count += newData.ppPair.count;
				if ((char)trendPP[number].status[0] >= 0 && (char)newData.status[0] >= 0)
				{
					trendPP[number].ppPair.max = __max(trendPP[number].ppPair.max, newData.ppPair.max);
					trendPP[number].ppPair.min = __min(trendPP[number].ppPair.min, newData.ppPair.min);
				}
				// �X�e�[�^�X���X�V����
				if ((char)trendPP[number].status[0] < 0 && (char)newData.status[0] >= 0)
				{
					trendPP[number].status[0] = newData.status[0];
					trendPP[number].ppPair.max = newData.ppPair.max;
					trendPP[number].ppPair.min = newData.ppPair.min;
				}

				// �ŏ��ő���v�Z
				CalcPPMinMaxWithCompareCount(trendPP, number, stPPMinMax);
				// PP���k�L���[�ɓ��ꂽ�Ď��f�[�^�T�C�Y���v
				m_PPTotalCount[trace] += newData.ppPair.count;
			}
			// ���k���X�g�̖����łȂ��ꍇ
			else
			{
				// ���k���X�g�̖����łȂ��ꍇ�́A���k���X�g�̖��������k�ς݂����f
				__int64	nEndPos = trendPP.size() - 1;
				if ((nEndPos > 0 && trendPP[nEndPos - 1].ppPair.count != trendPP[nEndPos].ppPair.count) || nEndPos == 0)
				{
					// �����ɐV�����f�[�^�����k�ǉ�
					trendPP[nEndPos].ppPair.count += newData.ppPair.count;
					if ((char)trendPP[nEndPos].status[0] >= 0 && (char)newData.status[0] >= 0)
					{
						trendPP[nEndPos].ppPair.max = __max(trendPP[nEndPos].ppPair.max, newData.ppPair.max);
						trendPP[nEndPos].ppPair.min = __min(trendPP[nEndPos].ppPair.min, newData.ppPair.min);
					}
					// �X�e�[�^�X���X�V����
					if ((char)trendPP[nEndPos].status[0] < 0 && (char)newData.status[0] >= 0)
					{
						trendPP[nEndPos].status[0] = newData.status[0];
						trendPP[nEndPos].ppPair.max = newData.ppPair.max;
						trendPP[nEndPos].ppPair.min = newData.ppPair.min;
					}

					// �ŏ��ő���v�Z
					CalcPPMinMaxWithCompareCount(trendPP, nEndPos, stPPMinMax);
					// PP���k�L���[�ɓ��ꂽ�Ď��f�[�^�T�C�Y���v
					m_PPTotalCount[trace] += newData.ppPair.count;
				}
				else
				{
					// �����f�[�^�̈��k
					trendPP[number].ppPair.count += trendPP[number + 1].ppPair.count;
					if ((char)trendPP[number].status[0] >= 0 && (char)trendPP[number + 1].status[0] >= 0)
					{
						trendPP[number].ppPair.max = __max(trendPP[number].ppPair.max, trendPP[number + 1].ppPair.max);
						trendPP[number].ppPair.min = __min(trendPP[number].ppPair.min, trendPP[number + 1].ppPair.min);
					}
					// �X�e�[�^�X���X�V����
					if ((char)trendPP[number].status[0] < 0 && (char)trendPP[number + 1].status[0] >= 0)
					{
						trendPP[number].status[0] = trendPP[number + 1].status[0];
						trendPP[number].ppPair.max = trendPP[number + 1].ppPair.max;
						trendPP[number].ppPair.min = trendPP[number + 1].ppPair.min;
					}
					number++;

					// ���k���s�����̂ň��k���ꂽ�f�[�^���폜����
					int pos = 0;
					deque<stSpaceData>::iterator itr;
					for (itr = trendPP.begin(); itr != trendPP.end(); itr++, pos++)
					{
						if (number == pos)
						{
							trendPP.erase(itr);
							break;
						}
					}
					// ���k���X�g�����ɐV�����f�[�^��ǉ�
					trendPP.push_back(newData);

					__int64	nTailPos = trendPP.size() - 1;
					// �ŏ��ő���v�Z
					CalcPPMinMaxWithCompareCount(trendPP, nTailPos, stPPMinMax);
					// PP���k�L���[�ɓ��ꂽ�Ď��f�[�^�T�C�Y���v
					m_PPTotalCount[trace] += newData.ppPair.count;
				}
			}
		}
		else
		{
			// ���k���X�g�����ɐV�����f�[�^��ǉ�
			trendPP.push_back(newData);

			__int64	nTailPos = trendPP.size() - 1;
			// �ŏ��ő���v�Z
			CalcPPMinMaxWithCompareCount(trendPP, nTailPos, stPPMinMax);
			// PP���k�L���[�ɓ��ꂽ�Ď��f�[�^�T�C�Y���v
			m_PPTotalCount[trace] += newData.ppPair.count;
			number++;
		}
	}
	// ���k����K�v�Ȃ�
	else
	{
		// ���k���X�g�����ɐV�����f�[�^��ǉ�
		trendPP.push_back(newData);

		__int64	nTailPos = trendPP.size() - 1;
		// �ŏ��ő���v�Z
		CalcPPMinMaxWithCompareCount(trendPP, nTailPos, stPPMinMax);
		// PP���k�L���[�ɓ��ꂽ�Ď��f�[�^�T�C�Y���v
		m_PPTotalCount[trace] += newData.ppPair.count;
	}

	m_PPNumber[trace] = number;
}


/*============================================================================*/
/*! CDataCollection

-# PP���k�f�[�^�𔼕��ɂ���

@param	�g���[�X�f�[�^�ԍ�
@param	�g�����h���k�f�[�^�L���[
@retval	�Ȃ�

*/
/*============================================================================*/
void CGraphWindow::DeleteHalfPP(int trace, deque<stSpaceData>& trendPP)
{
	// �ד��m�����k���Ă���
	__int64	number;
	__int64 oldSize = (__int64)trendPP.size();
	for (number = 0; number < (__int64)trendPP.size() - 1;)
	{
		trendPP[number].ppPair.count += trendPP[number + 1].ppPair.count;
		if ((char)trendPP[number].status[0] >= 0 && (char)trendPP[number + 1].status[0] >= 0)
		{
			trendPP[number].ppPair.max = __max(trendPP[number].ppPair.max, trendPP[number + 1].ppPair.max);
			trendPP[number].ppPair.min = __min(trendPP[number].ppPair.min, trendPP[number + 1].ppPair.min);
		}
		// �X�e�[�^�X���X�V����
		if ((char)trendPP[number].status[0] < 0 && (char)trendPP[number + 1].status[0] >= 0)
		{
			trendPP[number].status[0] = trendPP[number + 1].status[0];
			trendPP[number].ppPair.max = trendPP[number + 1].ppPair.max;
			trendPP[number].ppPair.min = trendPP[number + 1].ppPair.min;
		}
		number++;

		// ���k���ꂽ�f�[�^���폜����
		int pos = 0;
		deque<stSpaceData>::iterator itr;
		for (itr = trendPP.begin(); itr != trendPP.end(); itr++, pos++)
		{
			if (number == pos){
				trendPP.erase(itr);
				break;
			}
		}
	}

	if (number > 0)
	{
		if (oldSize % 2 == 0)
		{
			m_PPNumber[trace] = number - 1;
		}
		else
		{
			m_PPNumber[trace] = number;
		}
	}
	else
	{
		m_PPNumber[trace] = 0;
	}

	m_nTrendDataPos = 0;
}

/*============================================================================*/
/*! CDataCollection

-# PP���k�f�[�^�̓��̕���������

@param	�g���[�X�f�[�^�ԍ�
@param	�g�����h���k�f�[�^�L���[
@retval	�Ȃ�

*/
/*============================================================================*/
void CGraphWindow::DeleteHeadPP(int trace, deque<stSpaceData>& trendPP, int srcType)
{
	if (trendPP.empty() == FALSE)
	{
		// ���k���s�����̂ň��k���ꂽ�f�[�^���폜����
		deque<stSpaceData>::iterator itr = trendPP.begin();
		m_PPTotalCount[trace] -= (*itr).ppPair.count;
		trendPP.erase(itr);

		if (m_PPNumber[trace] > 0)
		{
			m_PPNumber[trace] -= 1;
		}

//#ifdef _DEBUG
//	{
//		if (m_TrendDataPP[trace].spaceData.size() > 0)
//		{
//			CString str;
//			str.Format(_T("%s(%d) :: Call delete headPP Sig[%d], GraphType = %u, ViewID = %u, cur_t = %I64d, size = %I64d"), __FILEW__, __LINE__, trace, m_nGraphType, m_nPropertiesID, CTime(m_TrendDataPP[trace].spaceData[m_TrendDataPP[trace].spaceData.size() - 1].systime).GetTime(), m_TrendDataPP[trace].spaceData.size());
//			CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("DeleteHeadPP"), _T("debug"), str, nLogEx::debug);
//
//		}
//	}
//#endif
	}

	m_nTrendDataPos = 0;

//	if (srcType == eSigSrcType_EngExchange)
//	{
//		deque<stSpaceData>::iterator itr = trendPP.begin();
//		if (itr != trendPP.end() && !m_bFitYohochi)
//		{
//			m_StartTime = CTime((*itr).systime).GetTime();
//		}
//	}
}


/*============================================================================*/
/*! CDataCollection

-# �q���f�[�^�ݒ������������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CGraphWindow::InitSatelliteDataSetting()
{
#ifdef _DEBUG
	{
		CString str;
		str.Format(_T("%s(%d) :: GraphType = %u, ViewID = %u"), __FILEW__, __LINE__, m_nGraphType, m_nPropertiesID);
		CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("InitSatelliteDataSetting"), _T("debug"), str, nLogEx::debug);
	}
#endif

	// �g�����h�̕`��
	MutexLock();

	for (int i = 0; i < MAX_TDATA; ++i)
	{
		m_nQueuLastTotalSize[i] = 0;	// ���߂̃L���[�g�[�^���T�C�Y�N���A
		m_PPNumber[i] = 0;				// ���k�ʒu�N���A
		m_PPTotalCount[i] = 0;			// ���k���v���N���A
		m_TrendDataPP[i].spaceData.clear();		// ���k���X�g�N���A
		m_TrendDataPP[i].spaceData.shrink_to_fit();	// ���k���X�g�̃������N���A
		m_bIsDrawOn[i] = TRUE;				// �`��ON

		m_curVal[i] = 0.0;
	}

	m_bFitYohochi = FALSE;				// �\��l�ɍ��킹��
	m_bStopDraw = FALSE;				// �`���~

	MutexUnlock();
}


/*============================================================================*/
/*! CDataCollection

-# ���k�ʒu�̗v�f�J�E���g�l�Ƃ��̑O�̗v�f�̃J�E���g�l���r���A�ŏ��l�ő�l���v�Z����

@param	�g�����h���k�f�[�^�L���[
@param	�L���[�̈ʒu
@param	�ŏ��l�ő�l
@retval

*/
/*============================================================================*/
void CGraphWindow::CalcPPMinMaxWithCompareCount(deque<stSpaceData>& trendPP, __int64 number, structPPMinMax& stPPMinMax)
{
	// count���������ꂽ��
	if (number == 0 || number > 0 && trendPP[number].ppPair.count == trendPP[number - 1].ppPair.count)
	{
		// �ŏ��ő���v�Z
		CalcPPMinMax(trendPP[number], stPPMinMax);
	}
}


/*============================================================================*/
/*! CDataCollection

-# �ŏ��l�ő�l���v�Z����

@param	�q���f�[�^
@param	�ŏ��l�ő�l
@retval	�Ȃ�

*/
/*============================================================================*/
void CGraphWindow::CalcPPMinMax(stSpaceData& sData, structPPMinMax& stPPMinMax)
{
// test >>>
//	if (m_nPropertiesID == eTrendWndType_RNG)
//	{
//		int c = 0;
//	}
// test <<<


	// �\���\���
	if ((char)sData.status[0] >= 0)
	{
		// �ő�l�A�ŏ��l�X�V
		if (sData.ppPair.max > stPPMinMax.stLine->nMaxValue)
		{
			stPPMinMax.stLine->nMaxValue = sData.ppPair.max;
			if (stPPMinMax.stLine->nVScaleKind == LScale &&
				stPPMinMax.stLine->nMaxValue > m_dTotalScaleLMax)
			{
				m_dTotalScaleLMax = stPPMinMax.stLine->nMaxValue;
				if (m_nStvScale.Auto == TRUE)
				{
					stPPMinMax.max = m_dTotalScaleLMax;
				}
			}
		}
		if (sData.ppPair.min < stPPMinMax.stLine->nMinValue)
		{
			stPPMinMax.stLine->nMinValue = sData.ppPair.min;
			if (stPPMinMax.stLine->nVScaleKind == LScale &&
				stPPMinMax.stLine->nMinValue < m_dTotalScaleLMin)
			{
				m_dTotalScaleLMin = stPPMinMax.stLine->nMinValue;
				if (m_nStvScale.Auto == TRUE)
				{
					stPPMinMax.min = m_dTotalScaleLMin;
				}
			}
		}
	}
}


/*============================================================================*/
/*! CDataCollection

-# PP���k�f�[�^�̍ő�T�C�Y��Ԃ�

@param	�Ȃ�
@retval	PP���k�f�[�^�̍ő�T�C�Y

*/
/*============================================================================*/
__int64 CGraphWindow::GetPPDataMaxSize()
{
	__int64 size = 0;

	// ���݂�������Ƃ��T�C�Y���傫�����k�f�[�^��T��
	for (int idx = 0; idx < MAX_TDATA; ++idx)
	{
		if ((__int64)m_TrendDataPP[idx].spaceData.size() > size)
		{
			size = m_TrendDataPP[idx].spaceData.size();
		}
	}

	return size;
}


/*============================================================================*/
/*! CDataCollection

-# �Ď��f�[�^��ǉ����A�c���l�ƊĎ��f�[�^�̍ő�ŏ��l�����߂�

@param	�Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CGraphWindow::AddSatelliteData()
{
	DRAWLINE::iterator itr;
	deque<CSpaceData>::iterator trenditr;

	UINT nID;

	__int64 size;
	BOOL bSetFirstTime = TRUE;

	MutexLock();
	CGraphMutex::Lock(eData);

	// �Ď��f�[�^���X�g�̎擾
	CSpaceData*	spaceDataList = theApp.GetDataCollection().GetSpaceData(m_StationIndex);
	CSpaceData*	sData;

	time_t mostNewTime = m_MostNewTime;

	// �v���p�e�B�̐ݒ�f�[�^���������[�v
	for (itr = m_DrawLines.begin(); itr != m_DrawLines.end(); itr++)
	{
		// �f�[�^���������ꍇ�͕\�����Ȃ�
		if ((*itr).second.strName.IsEmpty() == TRUE)
		{
			continue;
		}

		// �f�[�^�C���f�b�N�X���Ȃ��ꍇ�͕\�����Ȃ�
		int nListIdx = (int)((*itr).second.nListIdx);
		if (nListIdx < 0)
		{
			continue;
		}

		nID = (*itr).first;
		structLine data = (*itr).second;
		// ��\���ݒ�Ȃ�Ε\�����Ȃ�
		if (data.bIsVisible == FALSE)
		{
			continue;
		}

		sData = NULL;
		size = 0;

		sData = &spaceDataList[nListIdx];

		// �f�[�^��������Ȃ��ꍇ
		if (sData == NULL)
		{
			continue;
		}

		// �f�[�^�T�C�Y�̎擾
		size = (__int64)sData->value.size();
		if (size == 0)
		{
			continue;
		}

//		double max, min, curMax, curMin;
		double max, min;
		// ���̍ő�ŏ�
		// ���c��
		if ((*itr).second.nVScaleKind == LScale)
		{
			// �����X�P�[�����̍ő�ŏ�
			if (m_nStvScale.Auto == TRUE)
			{
				max = m_dTotalScaleLMax;
				min = m_dTotalScaleLMin;
//				double range = max - min;
//				
//				if (range == 0.0)
//					range = fabs(max);
//				if (range == 0.0)
//					range = 100.0;
//				curMax = Ceil3Keta(max + range * (double)AUTO_V_TOPBOTMARGIN);
//				curMin = Ceil3Keta(min - range * (double)AUTO_V_TOPBOTMARGIN);
			}
			// �Œ�X�P�[�����̍ő�ŏ�
			else
			{
				max = m_nStvScale.VScalLMax;
				min = m_nStvScale.VScalLMin;
//				curMax = max;
//				curMin = min;
			}
		}
		// �E�c��
		else
		{
			// �Œ�X�P�[�����̍ő�ŏ�
			max = m_nStvScale.VScalRMax;
			min = m_nStvScale.VScalRMin;
//			curMax = max;
//			curMin = min;
		}

// test >>>
//		if (m_nPropertiesID == eTrendWndType_EL)
//		{
//			int c = 0;
//		}
// test <<<


		// ����̎�M�����̈ʒu�𒲂ׂ�
		__int64 pos;
		// ���܂ł̃f�[�^�T�C�Y����O��̃L���[�T�C�Y���������A�V������M�����f�[�^�T�C�Y�����k�T�C�Y�ɖ����Ȃ��ꍇ�A
		// �V������M�������f�[�^�ōő�ŏ��𒲂ׂ�B
		// ����ȊO�̏ꍇ�̓f�[�^�����k�j�����Ă��邽�߁A���k�f�[�^�̎n�߂���I���܂łōő�ŏ��𒲂ׂ�B
		if (size >= (sData->totalSize - m_nQueuLastTotalSize[nID]))
		{
			pos = size - (sData->totalSize - m_nQueuLastTotalSize[nID]);
		}
		else
		{
			pos = 0;
		}

#if 0
//		// �����������X�P�[���łȂ��ꍇ
//		if (m_nTrandHScale != autoscale)
		{
			// �V�K��M�f�[�^�������`��͈͂��傫���ꍇ
			if (size - pos > m_dbHzRange && sData->srcType == eSigSrcType_EngExchange)
			{
				// �`��͈͈ȏ�̃f�[�^�͕s�v�̂��߁A�f�[�^�̌�납��`��͈͂𒲂ׂȂ���
//				pos = sData->totalSize - (__int64)m_dbHzRange;
				pos = size - (__int64)m_dbHzRange;
				time_t search_t1 = CTime(sData->value[pos].systime).GetTime();
				time_t search_t2;
				// ���邤�΍�̂��߁A�O�̎����ɓ������������������ׂ�
				while (pos > 0)
				{
					search_t2 = CTime(sData->value[pos - 1].systime).GetTime();
					if (search_t2 < search_t1)
					{
						break;
					}
					pos--;
				}
			}
		}
#else
		// �V�K��M�f�[�^�̕s�v�f�[�^����
		if (sData->srcType == eSigSrcType_EngExchange && pos < size && size > 0)
		{
			time_t	rcv_startpos_t = CTime(sData->value[pos].systime).GetTime();
			time_t	rcv_endpos_t = CTime(sData->value[size - 1].systime).GetTime();
			time_t	rcv_range_t = rcv_endpos_t - rcv_startpos_t;
			time_t	rcv_halfpos_t = rcv_range_t / 2 + rcv_startpos_t;
			time_t	search_t;
//			time_t	start_t = m_EndTime - (time_t)m_dbHzRange;
//			time_t	start_t = m_MostNewTime - (time_t)(m_dbHzRange * 0.9);
			time_t	start_t;
			if (mostNewTime > 0)
			{
				start_t = mostNewTime - (time_t)(m_dbHzRange * 0.9);
			}
			else
			{
				start_t = rcv_endpos_t - (time_t)(m_dbHzRange * 0.9);
			}

			if (start_t < 0)
			{
				start_t = 0;
			}

			// �V�K��M�f�[�^�̊J�n������̃O���t�`��J�n�����ȉ��ŁA���I�����`��J�n�����ȏ�̏ꍇ
			if (rcv_startpos_t <= start_t && rcv_endpos_t >= start_t)
			{
				// �V�K��M�f�[�^�̐^�񒆂��A����̃O���t�`��J�n�����ȏ�̏ꍇ
				if (rcv_halfpos_t >= start_t)
				{
					// �`��͈͈ȏ�̃f�[�^�͕s�v�̂��߁A�f�[�^��O����`��͈͂𒲂ׂȂ���
					search_t = rcv_startpos_t;
					while (search_t < start_t && pos < size - 1)
					{
						pos++;
						search_t = CTime(sData->value[pos].systime).GetTime();
					}
				}
				// �V�K��M�f�[�^�̐^�񒆂��A����̃O���t�`��J�n������菬�����ꍇ
				else
				{
					// �`��͈͈ȏ�̃f�[�^�͕s�v�̂��߁A�f�[�^�̖�������`��͈͂𒲂ׂȂ���
					search_t = rcv_endpos_t;
					pos = size - 1;
					while (search_t > start_t && pos > 0)
					{
						pos--;
						search_t = CTime(sData->value[pos].systime).GetTime();
					}

					// ���邤�Ď��ƒZ�����Ď��΍�̂��߁A�O�̎����ɓ������������������ׂ�
					time_t search_t2;
					while (pos > 0)
					{
						search_t2 = CTime(sData->value[pos - 1].systime).GetTime();
						if (search_t2 < search_t)
						{
							break;
						}
						pos--;
					}
				}
			}
		}
#endif

		m_stPPMinMax.max = max;
		m_stPPMinMax.min = min;
//		m_stPPMinMax.curMax = curMax;
//		m_stPPMinMax.curMin = curMin;
		m_stPPMinMax.stLine = &data;

		m_TrendDataPP[nID].srcType = sData->srcType;

//#ifdef _DEBUG
//		{
//			CString str;
//			str.Format(_T("%s(%d) :: Sig[%d], GraphType = %u, ViewID = %u, pos = %I64d, size = %I64d"), __FILEW__, __LINE__, nID, m_nGraphType, m_nPropertiesID, pos, size);
//			CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("AddSatelliteData"), _T("debug"), str, nLogEx::debug);
//		}
//#endif

		while (pos < size)
		{
			if (size > 1 && m_TrendDataPP[nID].spaceData.empty() == FALSE && m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange)
			{
//#ifdef _DEBUG
//				{
//					CString str;
//					str.Format(_T("%s(%d) :: Check delete harf Sig[%d], GraphType = %u, ViewID = %u, pos = %I64d, size = %I64d"), __FILEW__, __LINE__, nID, m_nGraphType, m_nPropertiesID, pos, m_TrendDataPP[nID].spaceData.size());
//					CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("AddSatelliteData"), _T("debug"), str, nLogEx::debug);
//				}
//#endif
				// �f�[�^�T�C�Y���O���t�E�[�܂��͈��k���E�𒴂��Ă���ꍇ
				if (m_TrendDataPP[nID].spaceData.size() >= MAX_PPDATA_SIZE)
				{
					// ���k�f�[�^�𔼕��ɂ���
					DeleteHalfPP(nID, m_TrendDataPP[nID].spaceData);
				}
			}

			// ���k�f�[�^����
			ConvertPP(nID, m_TrendDataPP[nID].spaceData, sData->value[pos], m_stPPMinMax);

			m_bExitData = TRUE;

			pos++;
		}

		if (!m_TrendDataPP[nID].spaceData.empty())
		{
			// ���O�̃T�C�Y���T���Ă���
			m_nQueuLastTotalSize[nID] = sData->totalSize;
			if (m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange)
			{
				// �L���[�̍Ō�̈ʒu���擾
				m_nTrendDataEndPos = (__int64)m_TrendDataPP[nID].spaceData.size();
			}

			// �����Ƃ��Â������Ƃ����Ƃ��V��������
			time_t t_old = CTime(m_TrendDataPP[nID].spaceData[0].systime).GetTime();
			time_t t_new = CTime(m_TrendDataPP[nID].spaceData[m_TrendDataPP[nID].spaceData.size() - 1].systime).GetTime();

//#ifdef _DEBUG
//			{
//				CString str;
//				str.Format(_T("%s(%d) :: Sig(%d), t_old = %I64d, t_new = %I64d, GraphType = %u, ViewID = %u"), __FILEW__, __LINE__, nID, t_old, t_new, m_nGraphType, m_nPropertiesID);
//				CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("AddSatelliteData"), _T("debug"), str, nLogEx::debug);
//			}
//#endif

			// �Ď��f�[�^�܂��́A�\��l�ɍ��킹��ݒ莞�̗\��l�̏ꍇ�A�����擾
			if (m_TrendDataPP[nID].srcType == eSigSrcType_EngExchange)
			{
				// �O���t�ōŏ��̃f�[�^�͖������Ŏ����擾
				if (bSetFirstTime)
				{
					bSetFirstTime = FALSE;
					m_MostOldTime = t_old;
					m_MostNewTime = t_new;
				}
				// �ŏ��ȊO�͑召��r
				else
				{
					if (t_old < m_MostOldTime)
					{
						m_MostOldTime = t_old;
					}
					if (t_new > m_MostNewTime)
					{
						m_MostNewTime = t_new;
					}
				}
			}

		}
	}

	CGraphMutex::Unlock(eData);
	MutexUnlock();

	// �\��l����̐ݒ�ł́A�Ď��f�[�^��AOS���J�n�����ɂȂ�
	m_MostOldTime = SelectStartTime(m_MostOldTime);

//#ifdef _DEBUG
//	{
//		CString str;
//		str.Format(_T("%s(%d) :: m_MostOldTime = %I64d, m_MostNewTime = %I64d, GraphType = %u, ViewID = %u"), __FILEW__, __LINE__, m_MostOldTime, m_MostNewTime, m_nGraphType, m_nPropertiesID);
//		CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("AddSatelliteData"), _T("debug"), str, nLogEx::debug);
//	}
//#endif

}


/*============================================================================*/
/*! CDataCollection

-# �O���t�������W��PP���k�f�[�^�������킹��

@param	t_old�F�����Ƃ��Â��f�[�^�����i�q���ߑ��O���t���Ɏg�p�j
@param	t_new:�����Ƃ��V���������i�q���ߑ��O���t���Ɏg�p�j
@retval	�Ȃ�

*/
/*============================================================================*/
time_t CGraphWindow::FitHRangeAndPPDataSize(time_t t_old, time_t t_new)
{
	CGraphMutex::Lock(eData);

	time_t		size = 0;
	deque<stSpaceData>*	spaceDataQueu = &m_TrendDataPP[0].spaceData;
	int	idx = 0;

	// �ŐV�����ƈ�v����f�[�^��T��
	BOOL bFind = FALSE;
	for (int nSearchIdx = 0; nSearchIdx < MAX_TDATA; ++nSearchIdx)
	{
		size = m_TrendDataPP[nSearchIdx].spaceData.size();
		// �H�w�l�ϊ���f�[�^
		if (m_TrendDataPP[nSearchIdx].srcType == eSigSrcType_EngExchange && size > 0) 
		{
			if (t_new == CTime(m_TrendDataPP[nSearchIdx].spaceData[size - 1].systime).GetTime())
			{
				idx = nSearchIdx;
				spaceDataQueu = &m_TrendDataPP[nSearchIdx].spaceData;
				bFind = TRUE;
				break;
			}
		}
	}

	// �Ď��f�[�^�T�C�Y�̏�����
	m_MostOldTime = t_old;
	m_MostNewTime = t_new;

	if (bFind == FALSE)
	{
		CGraphMutex::Unlock(eData);
		return 0;
	}

	size = m_MostNewTime - m_MostOldTime;

	if (size > 0)
	{
		// ���k�L���[����łȂ��ꍇ
		if (spaceDataQueu->empty() == FALSE && m_TrendDataPP[idx].srcType == eSigSrcType_EngExchange)
		{
			// ���k�Ȃ��ŁA���E�c���ɓ��B�����ꍇ
			if (size >= m_dbHzRange)
			{
				// �Â��f�[�^��10%���폜����ʒu�ɍX�V
				if (theApp.GetTrendScrollMode(m_nViewNo) == false)
				{
					if (m_bFitYohochi)
					{
						m_bStopDraw = TRUE;
						CGraphMutex::Unlock(eData);
						return m_MostOldTime;
					}

					time_t mostOldTime = m_MostOldTime;
					mostOldTime = mostOldTime + size - (__int64)(m_dbHzRange * 0.9);
					// �f�[�^�ɋL�^����Ă��鎞����T��
					__int64 searchPos = 0;
					__int64 lastPos = (__int64)(*spaceDataQueu).size() - 1;
					time_t searchTime = CTime((*spaceDataQueu)[searchPos].systime).GetTime();
					while (TRUE)
					{
						if (searchTime == mostOldTime)
						{
							m_MostOldTime = searchTime;
							break;
						}

						if (searchPos >= lastPos)
						{
							m_MostOldTime = mostOldTime;
							break;
						}

						searchPos++;
						searchTime = CTime((*spaceDataQueu)[searchPos].systime).GetTime();
					}

					m_nTrendDataPos = 0;
#ifdef _DEBUG
					{
						CString	str;
						str.Format(_T("delete 90per : m_MostOldTime = %I64d, size = %I64d, GraphType = %u, ViewID = %u, m_nTrendDataPos = %I64d"), m_MostOldTime, size, m_nGraphType, m_nPropertiesID, m_nTrendDataPos);
						CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("FitHRangeAndPPDataSize"), _T("debug"), str, nLogEx::debug);
					}
#endif
//					if (m_nGraphType != eGraphType_Summary && m_bFitYohochi == TRUE)
//					{
//						m_bFitYohochi = FALSE;
//					}
				}
			}
		}
		// �f�[�^�������ꍇ
		else
		{
			CGraphMutex::Unlock(eData);
			return 0;
		}
	}

	CGraphMutex::Unlock(eData);

	return m_MostOldTime;
}


/*============================================================================*/
/*! CDataCollection

-# �J�n�������Â��f�[�^���폜����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CGraphWindow::DeletePPData()
{
	time_t cur_t;
	time_t aos_t = (time_t)theApp.GetSatelliteData().m_trendData.m_gst_dspinf.d_AOS_time;
	time_t los_t = (time_t)theApp.GetSatelliteData().m_trendData.m_gst_dspinf.d_LOS_time;
	for (int i = 0; i < MAX_TDATA; i++)
	{
		if (m_TrendDataPP[i].spaceData.empty())
			continue;

		while (m_TrendDataPP[i].spaceData.empty() == false)
		{
			// ���̎���
			cur_t = CTime(m_TrendDataPP[i].spaceData[0].systime).GetTime();
			// �\��l�̏ꍇ
			if (m_TrendDataPP[i].srcType == eSigSrcType_File && theApp.GetSatelliteData().m_trendData.m_gst_dspinf.i_yohouti_umu == YOHOUTI_ARI)
			{
				if (cur_t >= aos_t && m_StartTime <= los_t)
				{
					break;
				}
			}
			// �\��l�łȂ��ꍇ
			else
			{
				if (cur_t >= m_StartTime)
				{
					break;
				}
			}

//#ifdef _DEBUG
//			{
//				CString str;
//				str.Format(_T("%s(%d) :: Delete Sig[%d], time = %I64d, GraphType = %u, ViewID = %u"), __FILEW__, __LINE__, i, cur_t, m_nGraphType, m_nPropertiesID);
//				CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("DeletePPData"), _T("debug"), str, nLogEx::debug);
//			}
//#endif
			// ���k�f�[�^�̐擪���폜
			DeleteHeadPP(i, m_TrendDataPP[i].spaceData, m_TrendDataPP[i].srcType);
		}
	}
}


/*============================================================================*/
/*! CDataCollection

-# �q���ߑ��O���t�i�\��l����E�Ȃ��j�^�T�}���O���t�ɍ��킹���J�n������Ԃ�

@param	t�F�w�莞��
@retval	�J�n����

*/
/*============================================================================*/
time_t CGraphWindow::SelectStartTime(time_t t)
{
	time_t		start_t;
	BOOL		bFitYohoch = FALSE;

	// �q���ߑ��O���t�̏ꍇ
	if (m_nGraphType != eGraphType_Summary)
	{
		start_t = t;

		// ����\��l�ɍ��킹��ꍇ
		if (m_nTrandHScale == yohochiscale)
		{
			if (theApp.GetSatelliteData().m_trendData.m_gst_dspinf.i_yohouti_umu == YOHOUTI_ARI)
			{
				// �\��l����̐ݒ�ł́A�Ď��f�[�^��AOS���J�n�����ɂȂ�
//				if (start_t <= (time_t)theApp.GetSatelliteData().m_trendData.m_gst_dspinf.d_LOS_time)
				{
					start_t = (time_t)theApp.GetSatelliteData().m_trendData.m_gst_dspinf.d_AOS_time;
				}
				bFitYohoch = TRUE;

				if (start_t > (time_t)theApp.GetSatelliteData().m_trendData.m_gst_dspinf.d_LOS_time)
					m_bStopDraw = TRUE;
			}
		}
	}
	// �T�}���O���t�̏ꍇ
	else
	{
		start_t = m_MostOldTime;
	}

	m_bFitYohochi = bFitYohoch;

	return start_t;
}


/*============================================================================*/
/*! �O���t�`��

-# �ݒ�{�^���N���b�N���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSettingBtn()
{
#ifdef _DEBUG
	{
		CString str;
		str.Format(_T("%s(%d) :: Call Setting Btn, GraphType = %u, ViewID = %u"), __FILEW__, __LINE__, m_nGraphType, m_nPropertiesID);
		CLogTraceEx::Write(_T(""), _T("CGraphWindow"), _T("OnBnClickedSettingBtn"), _T("debug"), str, nLogEx::debug);
	}
#endif

	if (m_nGraphType != eGraphType_Summary)
	{
		CTrendGraphView* pView = (CTrendGraphView*)GetParent();
		if (pView == NULL)
			return;

		if (m_btnSetting.GetSafeHwnd() == NULL)
			return;

		// �g�����h�v���p�e�B�_�C�A���O���J���ʒm
		theApp.m_pMainWnd->PostMessage(eMessage_OpenTrendSetDlg, (WPARAM)m_nPropertiesID, (LPARAM)GetParent());
	}
}


/*============================================================================*/
/*! �O���t�`��

-# ���r���\��ONOFF�؂�ւ����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedHGridOnBtn()
{
	if (m_btnHGridChk.GetSafeHwnd() == NULL)
		return;

	m_bHGridOn = m_btnHGridChk.GetCheck();

	// �ĕ`��
	Invalidate();
}


/*============================================================================*/
/*! �O���t�`��

-# �M���\��ONOFF�؂�ւ����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CGraphWindow::BnClickedSigOnBtn(UINT nSig)
{
	if (m_nGraphType != eGraphType_Summary && m_StationIndex != eStation_Usuda642)
	{
		if (m_chkBtnViewSig[nSig].GetSafeHwnd() == NULL)
			return;

		CPropertiesData& prop = theApp.GetSatelliteData().GetTrendPropertiesData(m_nPropertiesID);
		if (m_chkBtnViewSig[nSig].GetCheck())
			prop.isViewMode[nSig] = true;
		else
			prop.isViewMode[nSig] = false;

		theApp.GetSatelliteData().SetTrendPropertiesData(m_nPropertiesID, prop);
		// �ύX��ʒm
		theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 0, (LPARAM)m_nViewID);
	}
}

/*============================================================================*/
/*! �O���t�`��

-# �M���\��ONOFF�؂�ւ����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn0()
{
	BnClickedSigOnBtn(0);
}

/*============================================================================*/
/*! �O���t�`��

-# �M���\��ONOFF�؂�ւ����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn1()
{
	BnClickedSigOnBtn(1);
}

/*============================================================================*/
/*! �O���t�`��

-# �M���\��ONOFF�؂�ւ����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn2()
{
	BnClickedSigOnBtn(2);
}

/*============================================================================*/
/*! �O���t�`��

-# �M���\��ONOFF�؂�ւ����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn3()
{
	BnClickedSigOnBtn(3);
}


/*============================================================================*/
/*! �O���t�`��

-# �M���\��ONOFF�؂�ւ����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn4()
{
	BnClickedSigOnBtn(4);
}


/*============================================================================*/
/*! �O���t�`��

-# �M���\��ONOFF�؂�ւ����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn5()
{
	BnClickedSigOnBtn(5);
}


/*============================================================================*/
/*! �O���t�`��

-# �M���\��ONOFF�؂�ւ����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn6()
{
	BnClickedSigOnBtn(6);
}


/*============================================================================*/
/*! �O���t�`��

-# �M���\��ONOFF�؂�ւ����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn7()
{
	BnClickedSigOnBtn(7);
}


/*============================================================================*/
/*! �O���t�`��

-# �M���\��ONOFF�؂�ւ����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn8()
{
	BnClickedSigOnBtn(8);
}


/*============================================================================*/
/*! �O���t�`��

-# �M���\��ONOFF�؂�ւ����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn9()
{
	BnClickedSigOnBtn(9);
}


/*============================================================================*/
/*! �O���t�`��

-# �M���\��ONOFF�؂�ւ����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn10()
{
	BnClickedSigOnBtn(10);
}


/*============================================================================*/
/*! �O���t�`��

-# �M���\��ONOFF�؂�ւ����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn11()
{
	BnClickedSigOnBtn(11);
}


/*============================================================================*/
/*! �O���t�`��

-# �M���\��ONOFF�؂�ւ����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedSigOnBtn12()
{
	BnClickedSigOnBtn(12);
}


/*============================================================================*/
/*! �O���t�`��

-# �ϕ����Ԑݒ�{�^���N���b�N���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CGraphWindow::OnBnClickedIntgTimeBtn()
{
	CTrendGraphView* pView = (CTrendGraphView*)GetParent();
	if (pView == NULL)
		return;

	if (m_btnSetting.GetSafeHwnd() == NULL)
		return;

	CIntgTimeDlg dlg;
	dlg.SetIntgTime(m_nIntgTime);
	if (dlg.DoModal() == IDOK)
	{
		// �ϕ��ݒ�{�^���̕\���X�V
//		m_nIntgTime = dlg.GetIntgTime();
		UpdateIntegKanshi();
	}
}


/*============================================================================*/
/*! �O���t�`��

-# �ڍו\������̈悪���邩���ׂ�

@param  rect�F�̈�
@retval TRUE�F�̈悪���� / FALSE�F�̈悪�Ȃ�
*/
/*============================================================================*/
BOOL CGraphWindow::IsViewDetail(CRect& rect)
{
	if (rect.Width() > mDrawDetailWidth && rect.Height() > mDrawDetailHeight)
	{
		return TRUE;
	}

	return FALSE;
}


/*============================================================================*/
/*! �O���t�`��

-# �ő�l�ŏ��l���X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CGraphWindow::UpdateMaxMin()
{
	UINT nID;
	DRAWLINE::iterator itr;
	deque<CSpaceData>::iterator trenditr;

	// �ő�l�ŏ��l���N���A
	m_dTotalScaleLMax = -DBL_MAX;
	m_dTotalScaleLMin = DBL_MAX;

	for (itr = m_DrawLines.begin(); itr != m_DrawLines.end(); itr++)
	{
		// �f�[�^���������ꍇ�͕\�����Ȃ�
		if ((*itr).second.strName.IsEmpty() == TRUE)
		{
			continue;
		}

		// �f�[�^�C���f�b�N�X���Ȃ��ꍇ�͕\�����Ȃ�
		int nListIdx = (int)((*itr).second.nListIdx);
		if (nListIdx < 0)
		{
			continue;
		}

		nID = (*itr).first;

		if (m_TrendDataPP[nID].spaceData.size() > 0)
		{
			CalcPPMinMaxWithCompareCount(m_TrendDataPP[nID].spaceData, nID, m_stPPMinMax);
		}
	}
}
