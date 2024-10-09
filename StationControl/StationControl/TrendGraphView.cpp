/*============================================================================*/
/*! TrendGraphView.cpp

-# �g�����h�O���t
*/
/*============================================================================*/
// TrendGraphView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "TrendGraphView.h"
#include "MainFrm.h"
#include "ChildFrm.h"

//#define _ALLWAYS_AUTO_SCALE 1

// CTrendGraphView

IMPLEMENT_DYNCREATE(CTrendGraphView, CView)

/*============================================================================*/
/*! TrendGraphView

-# �R���X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CTrendGraphView::CTrendGraphView()
: m_nWndTypeSet(eTrendWndSetType_AGC_RNG_EL)
, m_bIsInitialized(FALSE)
, m_MostOldTime1st(0)
, m_MostOldTime(0)
, m_MostNewTime(0)
{

}

/*============================================================================*/
/*! TrendGraphView

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CTrendGraphView::~CTrendGraphView()
{
}

BEGIN_MESSAGE_MAP(CTrendGraphView, CView)
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CTrendGraphView �`��

/*============================================================================*/
/*! TrendGraphView

-# �`�掞�ɌĂ΂��

@param  pDC�F�h�L�������g�̃f�o�C�X�R���e�L�X�g�|�C���^
@retval �Ȃ�
*/
/*============================================================================*/
void CTrendGraphView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
}


// CTrendGraphView �f�f

#ifdef _DEBUG
void CTrendGraphView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CTrendGraphView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
CStationControlDoc* CTrendGraphView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CTrendGraphView ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CTrendGraphView

-# �r���[����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
void CTrendGraphView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	GetDocument()->SetTitleEx();
}

/*============================================================================*/
/*! CTrendGraphView

-# �g�����h�E�B���h�E����������

@param  type�F�E�B���h�E�Z�b�g�^�C�v 0:AGC / 1:RNG,RR / 2:AGC,RNG,EL
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
void CTrendGraphView::InitTrendWnd(eTrendWndSetType type)
{
	// �[���ԍ��擾����
	CString strTermNo;
	UINT nTermNo = theApp.GetTermNo();
	if (nTermNo > 0)
		nTermNo = nTermNo - 1;
	strTermNo.Format(_T("%02u"), nTermNo);

	SetWndTypeSet(type);

	for (int gno = 0; gno < eTrendWndType_Num; gno++){
		if (m_Graph[gno].GetSafeHwnd()){
			m_Graph[gno].DestroyWindow();
		}
	}

	// �e�O���t�\��
	CRect rect;
	GetClientRect(&rect);

	int nStart, nEnd;
	BOOL bEnaRR;
	GetGraphWndStartEnd(nStart, nEnd, bEnaRR);

	int nStation = theApp.GetSelectStation();

#ifdef _NOCOMM

	// �v���p�e�B�f�[�^��ݒ�
	// AGC/RNG/RR/EL������
	{
		vector<CString> listViewName;
		GetViewAllNameList(listViewName);

		for (int nPropertiesID = nStart; nPropertiesID <= nEnd; nPropertiesID++)
		{
			CPropertiesData prop = theApp.GetSatelliteData().GetTrendPropertiesData(nPropertiesID);

			// �E�B���h�E�|�C���^���R�s�[
			prop.wnd = (LPARAM)this;

			if (!prop.m_bSetted)
			{
				// �O���t�ԍ��R�s�[
				prop.nGraphIdx = nPropertiesID;
				UINT nViewID = nPropertiesID;
				prop.wndIdx = nViewID;						// �E�B���h�[�ԍ�
				prop.wndName = listViewName[nViewID];		// �E�B���h�[����

				prop.isRealTime = true;									// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
				prop.trandVScale.Auto = true;							// �c���� �I�[�g�X�P�[��
				prop.trandVScale.VScalLMax = initPropData.VScalLMax;	// �c���� �ő�l
				prop.trandVScale.VScalLMin = initPropData.VScalLMin;	// �c���� �ŏ��l
				//	prop.trandVScale.VScalRMax = initPropData.VScalRMax;	// �c���E �ő�l
				//	prop.trandVScale.VScalRMin = initPropData.VScalRMin;	// �c���E �ŏ��l
				prop.trandVScale.VSCalItvl = initPropData.VSCalItvl;	// �c���ڐ���̊Ԋu

				//		prop.trandHScale[0] = h24;			// �����X�P�[��(��)
				prop.trandHScale[0] = m1;			// �����X�P�[��
				//		prop.trandHScale[1] = m60;			// �����X�P�[��(��)

				for (int nSigNo = 0; nSigNo < MAX_TDATA; ++nSigNo)
				{
					if (mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSigName.Compare(UDSC64_AGC_XRX2_INTEG_SIG) == 0 ||
						mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSigName.Compare(UDSC64_AGC_XXTCR_INTEG_SIG) == 0 ||
						mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSigName.Compare(XDSC54_AGC_DSNMODEM_INTEG_SIG) == 0 ||
						mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSigName.Compare(USC34_AGC_INTEG_SIG) == 0)
					{
//						mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSigName = mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSigName + strTermNo;
						mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSigName = theApp.GetDataCollection().ReplaceAreaKanshiName(mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSigName) + strTermNo;
					}

					prop.dataName[nSigNo] = mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSigName;			// �f�[�^����
					prop.dataGName[nSigNo] = mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strSimpleSigName;	// �ȈՃf�[�^����
					prop.dataUnit[nSigNo] = mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].strUnit;			// �P��
					prop.nDecDidits[nSigNo] = mListSatelliteCaptureSig[nStation][nPropertiesID][nSigNo].nDecDidits;	// �����_�ȉ��̌���
					prop.isViewMode[nSigNo] = true;		// �\��On/Off
					prop.drawMode[nSigNo] = Line;		// �`�惂�[�h�iLine�ADots�AL&D�j
					prop.color[nSigNo] = colordef[nSigNo];	// �\���F
					prop.lineWidth[nSigNo] = 1;			// ����
					prop.dotSize[nSigNo] = Small;		// �h�b�g�T�C�Y
					prop.vscaleKind[nSigNo] = LScale;	// �c���X�P�[���I��
				}
				prop.m_bSetted = TRUE;
			}
			theApp.GetSatelliteData().SetTrendPropertiesData(nPropertiesID, prop);
		}
	}

	// AGC/RNG/RR/EL�̂����A�g���O���t�Z�b�g�̃r���[ID���Z�b�g
	{
//		vector<CString> listViewName;
//		GetViewNameList(listViewName);

//		int nStation = theApp.GetSelectStation();

		for (int nPropertiesID = nStart; nPropertiesID <= nEnd; nPropertiesID++)
		{
			if (nPropertiesID == eTrendWndType_RR && bEnaRR == FALSE)
				continue;

			CPropertiesData prop = theApp.GetSatelliteData().GetTrendPropertiesData(nPropertiesID);

			if (!prop.m_bSetted)
			{
				// �r���[ID�R�s�[
				UINT nViewID = ExchgPropertiesID2ViewID(nPropertiesID);
				prop.wndIdx = nViewID;						// �E�B���h�[�ԍ�
			}
			theApp.GetSatelliteData().SetTrendPropertiesData(nPropertiesID, prop);
		}
	}
#endif

	// �r���[����
	for (int nPropertiesID = nStart; nPropertiesID <= nEnd; nPropertiesID++)
	{
		if (nPropertiesID == eTrendWndType_RR && bEnaRR == FALSE)
			continue;

// test >>>
//		if (nPropertiesID != eTrendWndType_AGC)
//			continue;
// test <<<

		UINT nViewID = ExchgPropertiesID2ViewID(nPropertiesID);
		m_Graph[nPropertiesID].SetViewID(nViewID);
		m_Graph[nPropertiesID].SetPropertiesID(nPropertiesID);
		m_Graph[nPropertiesID].SetGraphType(eGraphType_Trend);
		m_Graph[nPropertiesID].SetStationIndex(nStation);
		m_Graph[nPropertiesID].Create(WS_VISIBLE, rect, this, eTrendCtrl, TRUE);
		m_Graph[nPropertiesID].SetGridType(CGraphWindow::eGridType_DOT);
	}

	// �r���[�Ƀp�����[�^�ǉ�
	AddViewParam();

	PostMessage(WM_SIZE);

	m_bIsInitialized = TRUE;
}


/*============================================================================*/
/*! CTrendGraphView

-# �T�C�Y�ύX���ɌĂ΂��

@param  nType
@param	cx
@param	cy
@retval TRUE�F���� / FALSE�F���s
*/
/*============================================================================*/
void CTrendGraphView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CRect rect, rect4;
	GetClientRect(&rect);
	if (m_nWndTypeSet == eTrendWndSetType_AGC_RNG_EL)
	{
		// �r���[�T�C�Y��AGC:4, RNG:3, EL:3�̊����ɂ���
		rect4 = rect;
		rect4.bottom = rect.bottom * 4 / 10;
		rect.bottom = rect.bottom * 3 / 10;
	}
	else
	{
		// �r���[�T�C�Y���ϓ��Ɋ���t����
		rect.bottom = rect.bottom / (m_nWndTypeSet + 1);
	}

	int nStart, nEnd;
	BOOL bEnaRR;
	GetGraphWndStartEnd(nStart, nEnd, bEnaRR);

// test >>>
//	nEnd = 0;
// test <<<

	for (int i = nStart; i <= nEnd; i++)
	{
		if (i == eTrendWndType_RR && bEnaRR == FALSE)
			continue;

		// AGC,RNG,EL�O���t�𓯎��\���̏ꍇ��AGC�O���t�̈�
		if (m_nWndTypeSet == eTrendWndSetType_AGC_RNG_EL && i == eTrendWndType_AGC)
		{
			// �O���t�̈�̃��T�C�Y
			if (m_Graph[i].GetSafeHwnd()){
				m_Graph[i].MoveWindow(&rect4);
				rect.OffsetRect(0, rect4.Height());
			}
		}
		// ����ȊO
		else
		{
			// �O���t�̈�̃��T�C�Y
			if (m_Graph[i].GetSafeHwnd()){
				m_Graph[i].MoveWindow(&rect);
				rect.OffsetRect(0, rect.Height());
			}
		}
	}
}


/*============================================================================*/
/*! CTrendGraphView

-# �r���[�ɍX�V��ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrendGraphView::UpdateGraph(BOOL bAll /*= FALSE*/)
{
	int nStart, nEnd;
	BOOL bEnaRR;
	GetGraphWndStartEnd(nStart, nEnd, bEnaRR);

	BOOL bFirst;
	BOOL bFitYoho;
// test >>>
//		nStart = 0;
//		nEnd = 0;
// test <<<

	// �`�悷�邩���ׂ�
	for (int i = nStart; i <= nEnd; i++)
	{
		if (m_Graph[i].GetSafeHwnd() != NULL)
		{
			if (m_Graph[i].GetStopDraw())
				return;
		}
	}

	// �X�V�f�[�^��ǉ�
	for (int i = nStart; i <= nEnd; i++)
	{
		if (m_Graph[i].GetSafeHwnd() != NULL)
		{
			m_Graph[i].AddSatelliteData();	// �\��l�ݒ���l��
		}
	}

	// �O���t�Ԃ̎������擾
	time_t t_old, t_new;
	bFirst = TRUE;
	bFitYoho = FALSE;
	for (int i = nStart; i <= nEnd; i++)
	{
		if (m_Graph[i].GetSafeHwnd() != NULL)
		{
			t_old = m_Graph[i].GetMostOldTime();
			if (m_Graph[i].GetFitYohochi())
				bFitYoho = TRUE;
			if (t_old == 0)
				continue;

			if (bFirst)
			{
				bFirst = FALSE;
				m_MostOldTime = t_old;
			}
			// �����Ƃ��Â������𓾂�
			if (t_old < m_MostOldTime)
			{
				m_MostOldTime = t_old;
			}
			t_new = m_Graph[i].GetMostNewTime();
			// �����Ƃ��V���������𓾂�
			if (t_new > m_MostNewTime)
			{
				m_MostNewTime = t_new;
			}
		}
	}

	// ����̂����Ƃ��Â����������������Â��Ȃ�Ȃ��悤�ɂ���
	if (m_MostOldTime < m_MostOldTime1st && bFitYoho == FALSE)
	{
		m_MostOldTime = m_MostOldTime1st;
	}


//#ifdef _DEBUG
//	{
//		CString	str;
//		str.Format(_T("m_MostOldTime = %I64d"), m_MostOldTime);
//		CLogTraceEx::Write(_T(""), _T("CTrendGraphView"), _T("UpdateGraph"), _T("debug"), str, nLogEx::debug);
//	}
//#endif

	// ���������W�ƃf�[�^�T�C�Y�����킹��
	time_t t_start, t_startTmp;
	t_start = 0;
	bFirst = TRUE;
	for (int i = nStart; i <= nEnd; i++)
	{
		if (m_Graph[i].GetSafeHwnd() != NULL)
		{
			t_startTmp = m_Graph[i].FitHRangeAndPPDataSize(m_MostOldTime, m_MostNewTime);

			if (t_startTmp == 0)
				continue;

			// ����̂����Ƃ��Â����������������Â��Ȃ�Ȃ��悤�ɂ���
			if (t_startTmp < m_MostOldTime1st && bFitYoho == FALSE)
			{
				t_startTmp = m_MostOldTime1st;
			}

			// �O���t�Ԃ̎��������킹��
			if (bFirst)
			{
				bFirst = FALSE;
				t_start = t_startTmp;
			}
			if (t_startTmp < t_start)
			{
				t_start = t_startTmp;
			}

#ifdef _ALLWAYS_AUTO_SCALE
			// �擪�̎����ύX���͍ő�l�ŏ��l��ݒ肵����
			if (t_startTmp != m_MostOldTime)
			{
				m_Graph[i].UpdateMaxMin();
			}
#endif /* _ALLWAYS_AUTO_SCALE */
		}
	}

	if (bFitYoho == FALSE)
	{
		// �Ď��̎����ɍ��킹��
		m_MostOldTime = t_start;
	}
	else
	{
		// �\��l�̎����ɍ��킹��
		t_start = m_MostOldTime;
	}

	for (int i = nStart; i <= nEnd; i++)
	{
		m_Graph[i].SetOldTime(m_MostOldTime);
	}

	if (t_start == 0)
	{
		SYSTEMTIME st_UTC;
		GetSystemTime(&st_UTC);
		t_start = CTime(st_UTC).GetTime();
	}

//#ifdef _DEBUG
//	{
//		CString	str;
//		str.Format(_T("t_start = %I64d"), t_start);
//		CLogTraceEx::Write(_T(""), _T("CTrendGraphView"), _T("UpdateGraph"), _T("debug"), str, nLogEx::debug);
//	}
//#endif

	// �Â��f�[�^���폜
	for (int i = nStart; i <= nEnd; i++)
	{
		if (m_Graph[i].GetSafeHwnd() != NULL)
		{
			m_Graph[i].SetStartTime(t_start);
			m_Graph[i].DeletePPData();
		}
	}

	// �Ď��f�[�^���X�V���ꂽ���߁A�O���t�r���[���X�V
	for (int i = nStart; i <= nEnd; i++)
	{
		if (m_Graph[i].GetSafeHwnd() != NULL)
		{
			// �`��ΏۂłȂ�
			if (i == eTrendWndType_RR && bEnaRR == FALSE)
				continue;

			// �O���t�r���[�X�V
			m_Graph[i].UpdateGraph(bAll);
		}
	}
}


/*============================================================================*/
/*! CTrendGraphView

-# �O���t�������Ď��f�[�^����擾���ăZ�b�g����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrendGraphView::SetGraphTime(int nStation)
{
	if (m_MostOldTime1st == 0 || m_MostNewTime == 0)
	{
		time_t t;
		// �Ď��f�[�^���X�g�̎擾
		CGraphMutex::Lock(eData);

		CSpaceData*	spaceDataList = theApp.GetDataCollection().GetSpaceData(nStation);
		int size = theApp.GetDataCollection().GetGraphSigSize(nStation);
		for (int i = 0; i < size; i++)
		{
			if (spaceDataList[i].srcType == eSigSrcType_EngExchange && spaceDataList[i].value.empty() == false)
			{
				t = CTime(spaceDataList[i].value[0].systime).GetTime();
				if (t == 0)
					continue;

				if (t < m_MostOldTime1st)
				{
					m_MostOldTime1st = t;
					m_MostOldTime = t;
					m_MostNewTime = t;
				}
			}
		}

		CGraphMutex::Unlock(eData);
	}
}


/*============================================================================*/
/*! CTrendGraphView

-# �r���[�ɍX�V��ʒm

@param  message	�F���b�Z�[�W�̎��ʎq
@param  wParam	�F���b�Z�[�W�̍ŏ��̃p�����[�^
@param  lParam	�F���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
@retval ��������
*/
/*============================================================================*/
LRESULT CTrendGraphView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_ThreadNormal:
		// �Ď��f�[�^�X�V�A�O���t�r���[�X�V
		UpdateGraph();
		break;

	case	eMessage_UpdateForecastData:
		// ������
		InitTrendWnd(m_nWndTypeSet);
		break;

	case	eMessage_PropetyChange:
		// �p�����[�^�X�V�̂��߁A�p�����[�^��ǂݍ��݂ɍs��
		AddViewParam();
		break;

	case	eMessage_InitTrendWnd:
		// ������
		InitTrendWnd((eTrendWndSetType)wParam);
		break;

	case eMessage_NewEngValData:
		// �O���t�������擾���ăZ�b�g����
		SetGraphTime((int)wParam);

	default:
		return CView::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}


/*============================================================================*/
/*! CTrendGraphView

-# �`�掞�ɌĂ΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CTrendGraphView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	// �`�惁�b�Z�[�W�� CView::OnPaint() ���Ăяo���Ȃ��ł��������B
}


/*============================================================================*/
/*! CTrendGraphView

-# �J�n�O���t�E�B���h�E�A�I���O���t�E�B���h�E�擾

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CTrendGraphView::GetGraphWndStartEnd(int& nStart, int& nEnd, BOOL& bEnaRR)
{
	switch (m_nWndTypeSet)
	{
	case eTrendWndSetType_AGC:
		// AGC
		nStart = eTrendWndType_AGC;
		nEnd = eTrendWndType_AGC;
		bEnaRR = FALSE;
		break;
	case eTrendWndSetType_RNG_RR:
		// RNG, RR
		nStart = eTrendWndType_RNG;
		nEnd = eTrendWndType_RR;
		bEnaRR = TRUE;
		break;
	case eTrendWndSetType_AGC_RNG_EL:
		// AGC, RNG, EL
	default:
		nStart = eTrendWndType_AGC;
		nEnd = eTrendWndType_EL;
		bEnaRR = FALSE;
		break;
	}
}


/*============================================================================*/
/*! CTrendGraphView

-# �O���t�r���[�����X�g�̎擾

@param	listViewName�F�r���[�����X�g
@retval	TRUE:�擾���� / FALSE:�擾���s

*/
/*============================================================================*/
BOOL CTrendGraphView::GetViewNameList(vector<CString>& listViewName)
{
	CString str;
	int type = GetWndTypeSet();

	switch (type)
	{
	case eTrendWndSetType_AGC:
		if (str.LoadString(IDS_TREND_AGCGRAPH) == FALSE)
			return FALSE;
		listViewName.push_back(str);
		break;

	case eTrendWndSetType_RNG_RR:
		if (str.LoadString(IDS_TREND_RNGGRAPH) == FALSE)
			return FALSE;
		listViewName.push_back(str);
		if (str.LoadString(IDS_TREND_RRGRAPH) == FALSE)
			return FALSE;
		listViewName.push_back(str);
		break;

	case eTrendWndSetType_AGC_RNG_EL:
		if (str.LoadString(IDS_TREND_AGCGRAPH) == FALSE)
			return FALSE;
		listViewName.push_back(str);
		if (str.LoadString(IDS_TREND_RNGGRAPH) == FALSE)
			return FALSE;
		listViewName.push_back(str);
		if (str.LoadString(IDS_TREND_ELGRAPH) == FALSE)
			return FALSE;
		listViewName.push_back(str);

		break;

	default:
		break;
	}

	return TRUE;
}


/*============================================================================*/
/*! CTrendGraphView

-# �O���t�r���[���S���X�g�̎擾

@param	listViewName�F�r���[�����X�g
@retval	TRUE:�擾���� / FALSE:�擾���s

*/
/*============================================================================*/
BOOL CTrendGraphView::GetViewAllNameList(vector<CString>& listViewName)
{
	CString str;
	if (str.LoadString(IDS_TREND_AGCGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);
	if (str.LoadString(IDS_TREND_RNGGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);
	if (str.LoadString(IDS_TREND_RRGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);
	if (str.LoadString(IDS_TREND_ELGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	return TRUE;
}


/*============================================================================*/
/*! CTrendGraphView

-# �r���[�Ƀp�����[�^��ǉ�����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CTrendGraphView::AddViewParam()
{
	int nStart, nEnd;
	BOOL bEnaRR;
	GetGraphWndStartEnd(nStart, nEnd, bEnaRR);

	for (int nPropertiesID = nStart; nPropertiesID <= nEnd; nPropertiesID++)
	{
		if (nPropertiesID == eTrendWndType_RR && bEnaRR == FALSE)
			continue;

// test >>>
//		if (nPropertiesID != eTrendWndType_AGC)
//			continue;
// test <<<

		CPropertiesData* pParam = &theApp.GetSatelliteData().GetTrendPropertiesData(nPropertiesID);

//		m_Graph[nPropertiesID].SetStationIndex(theApp.GetSelectStation());
		m_Graph[nPropertiesID].SetRealTime(pParam->isRealTime);
		(void)m_Graph[nPropertiesID].AddAxis(pParam->trandVScale, pParam->trandHScale[0]);

		CString	strName;

		// �e�f�[�^�̃p�����[�^��ǉ�
		for (int i = 0; i < MAX_TDATA; ++i)
		{
			switch (pParam->drawMode[i])
			{
			case Line:
				(void)m_Graph[nPropertiesID].AddLine(i, pParam, 0, PS_SOLID);
				break;
			case Dots:
				(void)m_Graph[nPropertiesID].AddDot(i, pParam, 0);
				break;
			case LineAndDots:
				(void)m_Graph[nPropertiesID].AddLineAndDot(i, pParam, 0, PS_SOLID);
				break;
			default:
				// �������Ȃ�
				break;
			}
		}

	}

	// �J�[�\���҂��\��
	CWaitCursor wait;

	// �O���t�X�V
	UpdateGraph(TRUE);
}


/*============================================================================*/
/*! CTrendGraphView

-# �r���[ID���E�B���h�E�^�C�v�Z�b�g�ɂ���āA�r���[�ԍ��ɕϊ�����

@param	type�F�r���[ID
@retval	�r���[�ԍ�

*/
/*============================================================================*/
UINT CTrendGraphView::ExchgPropertiesID2ViewID(UINT nPropertiesID)
{
	UINT nViewID = nPropertiesID;

	// �O���t���r���[�̔ԍ��ɕϊ�
	switch (GetWndTypeSet())
	{
	case eTrendWndSetType_AGC:
		break;
	case eTrendWndSetType_RNG_RR:
		nViewID -= 1;
		break;
	case eTrendWndSetType_AGC_RNG_EL:
		if (nPropertiesID == eTrendWndType_EL)
			nViewID -= 1;
		break;
	default:
		break;
	}

	return nViewID;
}

/*============================================================================*/
/*! �ߑ��O���t

-# ���C�A�E�g�ǂݍ��݁i�ݒ�ǂݍ��݁j

@param  �Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
bool CTrendGraphView::Load(CArchive& ar)
{
	InitTrendWnd(m_nWndTypeSet);
	return true;
}

/*============================================================================*/
/*! �ߑ��O���t

-# ���C�A�E�g�ۑ��i�ݒ�ۑ��j

@param  �Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
bool CTrendGraphView::Save(CArchive& ar)
{
	return true;
}
