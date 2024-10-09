/*============================================================================*/
/*! SummaryGraphCtrl.cpp

-# �T�}���O���t
*/
/*============================================================================*/
// SummaryGraphCtrl.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "SummaryGraphCtrl.h"
#include "SummaryPropertiesDlg.h"

// CSummaryGraphCtrl

IMPLEMENT_DYNAMIC(CSummaryGraphCtrl, CWnd)

/*============================================================================*/
/*! CSummaryGraphCtrl

-# �R���X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSummaryGraphCtrl::CSummaryGraphCtrl()
{
	mParent = NULL;
	mInitOver = FALSE;
	mRect.SetRectEmpty();
	m_nListMax = MAX_SUMMARYTDATA;
	m_pSpectrumView = NULL;
}

/*============================================================================*/
/*! CSummaryGraphCtrl

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSummaryGraphCtrl::~CSummaryGraphCtrl()
{
}


BEGIN_MESSAGE_MAP(CSummaryGraphCtrl, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CSummaryGraphCtrl ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! CSummaryGraphCtrl
�E�B���h�E�̍쐬

@param	pParent�F�e�E�B���h�E�n���h��
@param	rect�F�̈�
@param	nID�F�E�B���h�EID
@retval TRUE�F���� / FALSE�F���s
*/
/*============================================================================*/
BOOL CSummaryGraphCtrl::Create(CWnd *pParent, CRect rect, UINT nID, UINT nStaionID, UINT nParentID/* = 0*/)
{
	if (!RegisterClass())
		return FALSE;

	mParent = pParent;
	mID = nID;
	mStationIndex = nStaionID;

	if (CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParent, nID, NULL)){
		mInitOver = TRUE;
		return TRUE;
	}
	return FALSE;
}

/*============================================================================*/
/*! CSummaryGraphCtrl
�E�B���h�E�N���X��o�^

@param	�Ȃ�
@retval TRUE�F���� / FALSE�F���s
*/
/*============================================================================*/
BOOL CSummaryGraphCtrl::RegisterClass()
{
	return TRUE;
	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = AfxWndProc;
	wc.hInstance = AfxGetInstanceHandle();
	wc.hbrBackground = ::CreateSolidBrush(COLOR_WINDOW);
	wc.hCursor = ::LoadCursor(AfxGetInstanceHandle(), IDC_ARROW);
	wc.lpszClassName = _T("SUMMARYGRAPH");
	return AfxRegisterClass(&wc);
}

/*============================================================================*/
/*! CSummaryGraphCtrl
�E�B���h�E�̍쐬

@param	lpCreateStruct
@retval	0�F���� / -1�F���s
*/
/*============================================================================*/
int CSummaryGraphCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

/*============================================================================*/
/*! CSummaryGraphCtrl

-# �T�C�Y�ύX���ɌĂ΂��

@param  nType
@param	cx
@param	cy
@retval TRUE�F���� / FALSE�F���s
*/
/*============================================================================*/
void CSummaryGraphCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect);
	mRect = rect;
	if (m_Graph.GetSafeHwnd()){
		m_Graph.MoveWindow(mRect);
	}
}


/*============================================================================*/
/*! CSummaryGraphCtrl

-# �`�掞�ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CSummaryGraphCtrl::OnPaint()
{
	CPaintDC dc(this);

	return;
}


/*============================================================================*/
/*! CSummaryGraphCtrl

-# �r���[�Ƀp�����[�^��ǉ�����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSummaryGraphCtrl::AddViewParam()
{
	if (m_Graph.GetSafeHwnd() == NULL)
		return;

	int nViewID = mID - eSummaryCtrl;

	CPropertiesData* pParam = &theApp.GetSatelliteData().GetSummaryPropertiesData(mStationIndex, nViewID);

//	m_Graph.SetStationIndex(mStationIndex);
//
// test >>>
//	if (mStationIndex == eStation_Uchinoura20 && nViewID == 3)
//	{
//		int a = 0;
//	}
// test <<<

	m_Graph.SetRealTime(pParam->isRealTime);
	(void)m_Graph.AddAxis(pParam->trandVScale, pParam->trandHScale[0]);

	CString	strName;

	// �e�f�[�^�̃p�����[�^��ǉ�
	for (int i = 0; i < m_nListMax; ++i)
	{
		switch (pParam->drawMode[i])
		{
		case Line:
			(void)m_Graph.AddLine(i, pParam, 0, PS_SOLID);
			break;
		case Dots:
			(void)m_Graph.AddDot(i, pParam, 0);
			break;
		case LineAndDots:
			(void)m_Graph.AddLineAndDot(i, pParam, 0, PS_SOLID);
			break;
		default:
			// �������Ȃ�
			break;
		}
	}

	// �O���t�X�V
	UpdateGraph(TRUE);
}


/*============================================================================*/
/*! CSummaryGraphCtrl

-# �T�}���E�B���h�E����������

@param  type�F�E�B���h�E�Z�b�g�^�C�v 0:AGC / 1:RNG,RR / 2:AGC,RNG,EL
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
void CSummaryGraphCtrl::InitTrendWnd(eSummaryWndSetType type)
{
	if (m_pSpectrumView != NULL && m_pSpectrumView->GetSafeHwnd() != NULL)
	{
		m_pSpectrumView->ShowWindow(SW_HIDE);
	}

	if (m_Graph.GetSafeHwnd() != NULL)
	{
//		m_Graph.ShowWindow(SW_SHOW);
//		return;
		m_Graph.SendMessage(WM_CLOSE);
	}

	KillTimer(1);

	SetWndTypeSet(type);

	// �e�O���t�\��
	CRect rect;
	GetClientRect(&rect);

	int nViewID = mID - eSummaryCtrl;
	m_Graph.SetViewID(nViewID);
	m_Graph.SetGraphType(eGraphType_Summary);
	m_Graph.SetStationIndex(mStationIndex);
	m_Graph.Create(WS_VISIBLE, rect, this, mID + 4);
	m_Graph.SetGridType(CGraphWindow::eGridType_DOT);

	// �r���[�Ƀp�����[�^�ǉ�
	AddViewParam();

	PostMessage(WM_SIZE);
}


/*============================================================================*/
/*! CSummaryGraphCtrl

-# �X�y�N�g�����E�B���h�E����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
void CSummaryGraphCtrl::InitSpectrumWnd(UINT nViewID)
{
	CRect rectLast;
	if (m_Graph.GetSafeHwnd() != NULL)
	{
		m_Graph.ShowWindow(SW_HIDE);
	}

	// �e�O���t�\��
	CRect rect;
	GetClientRect(&rect);

	CRuntimeClass* pClass = RUNTIME_CLASS(CSpectrumGraphView);
	if (pClass == NULL)
	{
		return;
	}
	m_pSpectrumView = (CSpectrumGraphView*)(pClass->CreateObject());
	if (m_pSpectrumView != NULL)
	{
		m_pSpectrumView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, mID + 4, NULL);
		m_pSpectrumView->InitializeView(mStationIndex, nViewID, eSpectrumGraphType_Sammary);
		m_pSpectrumView->SendMessage(WM_INITIALUPDATE);
	}

	PostMessage(WM_SIZE);

	// �^�C�}�o�^
	SetTimer(1, SPECTRUM_INTERVAL, NULL);
}


/*============================================================================*/
/*! CTrendGraphView

-# �r���[�ɍX�V��ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CSummaryGraphCtrl::UpdateGraph(BOOL bAll /*= FALSE*/)
{
	if (m_Graph.GetSafeHwnd() != NULL)
	{
		// �X�V�f�[�^��ǉ�
		m_Graph.AddSatelliteData();	// �\��l�ݒ���l��

		// �O���t�Ԃ̎������擾
		time_t t_old, t_new;
		t_old = m_Graph.GetMostOldTime();
		t_new = m_Graph.GetMostNewTime();
		if (t_old != 0)
		{
			m_MostOldTime = t_old;
			m_MostNewTime = t_new;
		}

		// ����̂����Ƃ��Â����������������Â��Ȃ�Ȃ��悤�ɂ���
		if (m_MostOldTime < m_MostOldTime1st)
		{
			m_MostOldTime = m_MostOldTime1st;
		}

		time_t t_start;
		t_start = m_Graph.FitHRangeAndPPDataSize(m_MostOldTime, m_MostNewTime);
		// ����̂����Ƃ��Â����������������Â��Ȃ�Ȃ��悤�ɂ���
		if (t_start < m_MostOldTime1st)
		{
			t_start = m_MostOldTime1st;
		}

		// �Â��f�[�^���폜
		m_Graph.SetStartTime(t_start);
		m_Graph.DeletePPData();

		// �O���t�r���[�X�V
		m_Graph.UpdateGraph(bAll);
	}
}


/*============================================================================*/
/*! CTrendGraphView

-# �O���t�������Ď��f�[�^����擾���ăZ�b�g����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CSummaryGraphCtrl::SetGraphTime(int nStation)
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
/*! CSummaryGraphCtrl

-# �r���[�ɍX�V��ʒm

@param  message	�F���b�Z�[�W�̎��ʎq
@param  wParam	�F���b�Z�[�W�̍ŏ��̃p�����[�^
@param  lParam	�F���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
@retval ��������
*/
/*============================================================================*/
LRESULT CSummaryGraphCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	int nViewID = mID - eSummaryCtrl;
	CPropertiesData& prop = theApp.GetSatelliteData().GetSummaryPropertiesData(mStationIndex, nViewID);

	bool bSpectrum;
	if (prop.nGraphIdx == eSummaryWndSetType_XSP && mStationIndex != eStation_Uchinoura20 ||
		prop.nGraphIdx == eSummaryWndSetType_KASP && mStationIndex == eStation_Usuda642)
		bSpectrum = true;
	else
		bSpectrum = false;

	switch (message){
	case	eMessage_ThreadNormal:
		if (bSpectrum)
		{
		}
		else
		{
			// �Ď��f�[�^���X�V���ꂽ���߁A�r���[�ɍX�V��ʒm
			UpdateGraph();
		}
		break;

	case	eMessage_SummaryPropetyChange:
		if (bSpectrum)
		{
			// �p�����[�^�X�V�̂��߁A�p�����[�^��ǂݍ��݂ɍs��
			if (m_pSpectrumView != NULL)
			{
				m_pSpectrumView->ClearAutoVMaxMin();
				m_pSpectrumView->Invalidate();
			}
		}
		else
		{
			// �p�����[�^�X�V�̂��߁A�p�����[�^��ǂݍ��݂ɍs��
			AddViewParam();
		}
		break;

	case	eMessage_InitSummaryWnd:
	{
		if (bSpectrum)
		{
			// �~�j�X�y�N�g����������
			InitSpectrumWnd(nViewID);
		}
		else
		{
			// �~�j�g�����h������
			InitTrendWnd((eSummaryWndSetType)wParam);
		}
		break;
	}

	case eMessage_NewEngValData:
		// �O���t�������擾���ăZ�b�g����
		SetGraphTime((int)wParam);
		break;

	default:
		return CWnd::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}


/*============================================================================*/
/*! CSummaryGraphCtrl

-# �^�C�}�C�x���g�������ɌĂяo�����

@param	nIDEvent�F�C�x���gID
@retval	�Ȃ�

*/
/*============================================================================*/
void CSummaryGraphCtrl::OnTimer(UINT_PTR nIDEvent)
{
	CSpectrumViewData* pCSectrumData = &theApp.GetSatelliteData().GetSummarySpectrumViewData(mStationIndex, mID - eSummaryCtrl);
	if (pCSectrumData == NULL)
	{
		return;
	}

	/* �Ď��f�[�^�擾 */
	if (pCSectrumData->GetSpectrumData() != ERR_RETURN && m_pSpectrumView != NULL) 
	{
		// �r���[�`��
		m_pSpectrumView->Invalidate();
	}

	CWnd::OnTimer(nIDEvent);
}


/*============================================================================*/
/*! CSummaryGraphCtrl

-# �E�B���h�E�j�����ɌĂяo�����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSummaryGraphCtrl::OnDestroy()
{
	CWnd::OnDestroy();

	KillTimer(1);
}

/*============================================================================*/
/*! �T�}���O���t

-# ���C�A�E�g�ǂݍ��݁i�ݒ�ǂݍ��݁j

@param  �Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
bool CSummaryGraphCtrl::Load(CArchive& ar)
{
	int nViewID = mID - eSummaryCtrl;
	CPropertiesData& prop = theApp.GetSatelliteData().GetSummaryPropertiesData(mStationIndex, nViewID);
	if (prop.nGraphIdx == eSummaryWndSetType_XSP && mStationIndex != eStation_Uchinoura20 ||
		prop.nGraphIdx == eSummaryWndSetType_KASP && mStationIndex == eStation_Usuda642)
	{
		// �p�����[�^�X�V�̂��߁A�p�����[�^��ǂݍ��݂ɍs��
		if (m_pSpectrumView != NULL)
			m_pSpectrumView->Invalidate();
	}
	else
	{
		// �p�����[�^�X�V�̂��߁A�p�����[�^��ǂݍ��݂ɍs��
		AddViewParam();
	}
	return true;
}

/*============================================================================*/
/*! �T�}���O���t

-# ���C�A�E�g�ۑ��i�ݒ�ۑ��j

@param  �Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
bool CSummaryGraphCtrl::Save(CArchive& ar)
{
	return true;
}
