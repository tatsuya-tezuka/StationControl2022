/*============================================================================*/
/*! CRangePropagationStatusMonitor.cpp

-# �����W�`���󋵃��j�^���
*/
/*============================================================================*/
// CRangePropagationStatusMonitor.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "RangePropagationStatusMonitor.h"
#include "RangePropagationView.h"
#include "afxdialogex.h"

#ifdef _DEBUG_RNGPROPDUMMY
UINT mRngAreaDummySelect = 2;
#endif /* _DEBUG_RNGPROPDUMMY */

// CRangePropagationStatusMonitor �_�C�A���O

IMPLEMENT_DYNAMIC(CRangePropagationStatusMonitor, CDialogBase)

/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CRangePropagationStatusMonitor::CRangePropagationStatusMonitor(CWnd* pParent /*=NULL*/)
	: CDialogBase(CRangePropagationStatusMonitor::IDD, pParent)
	, m_pView(NULL)
{
}

/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CRangePropagationStatusMonitor::~CRangePropagationStatusMonitor()
{
}

void CRangePropagationStatusMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RANGEPROPAGATION_AREA, m_viewArea);
	DDX_Control(pDX, IDC_RANGEPROP_LIST, m_detailList);
}


BEGIN_MESSAGE_MAP(CRangePropagationStatusMonitor, CDialogBase)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CRangePropagationStatusMonitor::OnMenuClose)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CRangePropagationStatusMonitor ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# �_�C�A���O����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
BOOL CRangePropagationStatusMonitor::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// �r���[�̍쐬
	CRect rect;
	m_viewArea.GetWindowRect(&rect);
	ScreenToClient(&rect);
	CRuntimeClass* pClass = RUNTIME_CLASS(CRangePropagationView);
	if (pClass == NULL)
	{
		return FALSE;
	}

	m_pView = (CRangePropagationView*)(pClass->CreateObject());
	if (m_pView != NULL)
	{
		m_pView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, 0, NULL);
		m_pView->SendMessage(WM_INITIALUPDATE);
	}

	InitStatus();

	// ���X�g������
	if (!InitList())
		return FALSE;

	// ���X�g�A�C�e���}��
	if (!InsertListItem())
		return FALSE;

	//��ɃX�N���[���o�[��\��������
	m_detailList.EnableScrollBarCtrl(SB_VERT, TRUE);

	UpdateWindow();

	SetTimer(1, 1000, NULL);

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# ���X�g�R���g���[��������

@param	�Ȃ�
@retval	TRUE:���������� / FALSE:���������s

*/
/*============================================================================*/
BOOL CRangePropagationStatusMonitor::InitList()
{
	// ���X�g���ڏ�����
	LVCOLUMN lvc;

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_FMT;

	lvc.iSubItem = 0;
	lvc.pszText = _T("No.");
	lvc.cx = m_nNoWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_detailList.InsertColumn(0, &lvc) < 0)
		return FALSE;

	lvc.iSubItem = 1;
	lvc.pszText = _T("RANGE NUMBER");
	lvc.cx = m_nRngNumWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_detailList.InsertColumn(1, &lvc) < 0)
		return FALSE;

	lvc.iSubItem = 2;
	lvc.pszText = _T("Tx TIME");
	lvc.cx = m_nTxTimeWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_detailList.InsertColumn(2, &lvc) < 0)
		return FALSE;

	lvc.iSubItem = 3;
	lvc.pszText = _T("SC TIME");
	lvc.cx = m_nScTimeWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_detailList.InsertColumn(3, &lvc) < 0)
		return FALSE;

	lvc.iSubItem = 4;
	lvc.pszText = _T("Rx TIME");
	lvc.cx = m_nRxTimeWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_detailList.InsertColumn(4, &lvc) < 0)
		return FALSE;

	m_detailList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	return TRUE;
}

/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# ���X�g�R���g���[���ɃA�C�e���}��

@param	�Ȃ�
@retval	TRUE:�}������ / FALSE:�}�����s

*/
/*============================================================================*/
BOOL CRangePropagationStatusMonitor::InsertListItem()
{
	CGraphMutex::Lock(eRngProp);

	// �`���f�[�^���擾
	CRangePropagation& rangePropagation = theApp.GetSatelliteData().GetRangePropagation();
	deque<CRangePropagation::structPropData>& dequeRngAll = rangePropagation.GetRngAllData();

	int dequeSize = (int)dequeRngAll.size();
	int oldListCtrlSize = m_detailList.GetItemCount();

	if (dequeSize == 0 && oldListCtrlSize == 0)
	{
		CGraphMutex::Unlock(eRngProp);
		return TRUE;
	}

	if (dequeSize == 0)
	{
		m_detailList.DeleteAllItems();
		CGraphMutex::Unlock(eRngProp);
		return TRUE;
	}

	if (dequeSize < oldListCtrlSize)
	{
		CGraphMutex::Unlock(eRngProp);
		return FALSE;
	}

	// �����W�g�[�����X�g�̐擪�C���f�b�N�X�ŁA���X�g�R���g���[���̐擪���猟�����A
	// �����W�g�[�����X�g����폜���ꂽ�������X�g�R���g���[������폜����
	CString strSearchCount;
	strSearchCount.Format(_T("%04u"), dequeRngAll.front().count);
	CString strCtrlCount;
	int nDelCount = 0;
	for (int i = 0; i < oldListCtrlSize; i++)
	{
		strCtrlCount = m_detailList.GetItemText(i, 1);
//		TRACE("###### Search delete string, dequeRngAll[0]:%04u\n", dequeRngAll.front().count);

		if (strCtrlCount.CompareNoCase(strSearchCount) == 0)
		{
			// �擪���璲�ׂ��J�E���g�l����v�̏ꍇ
//			TRACE("###### Hit delete string, dequeRngAll[0]:%04u\n", dequeRngAll.front().count);
			nDelCount = i;
			break;
		}
	}

	// ���X�g�R���g���[������s�v�����폜
	for (int i = 0; i < nDelCount && m_detailList.GetItemCount() > 0; i++)
	{
		m_detailList.DeleteItem(0);
	}

	// ���X�g�R���g���[���̎��̈ʒu���炪�ǉ��ʒu�ƂȂ�
	int newListCtrlSize = m_detailList.GetItemCount();
	if (dequeSize < newListCtrlSize)
	{
		CGraphMutex::Unlock(eRngProp);
		return FALSE;
	}

	if (dequeSize == newListCtrlSize)
	{
		CGraphMutex::Unlock(eRngProp);
		return TRUE;
	}

	LVITEM lvi;
	CString str;
	int index = 0;
	CTime ctime;

	TRACE("###### dequeSize:%04d, oldListCtrlSize:%04d, newListCtrlSize:%04d, nDelCount:%04d, count[%04d]:%04u\n", dequeSize, oldListCtrlSize, newListCtrlSize, nDelCount, newListCtrlSize, dequeRngAll[newListCtrlSize].count);

	// �����W�g�[�������������X�g�R���g���[���ɒǉ�
	for (int i = newListCtrlSize; i < dequeSize; i++)
	{
		// ���X�g�R���g���[���ɓ��e�Z�b�g
		lvi.mask = LVIF_TEXT;

		lvi.iItem = i;
		lvi.iSubItem = 0;
//		str.Format(_T("%04d"), i + 1);
		str.Format(_T("%04u"), dequeRngAll[i].count);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if ((index = m_detailList.InsertItem(&lvi)) < 0){
			CGraphMutex::Unlock(eRngProp);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 1;
		str.Format(_T("%04u"), dequeRngAll[i].count);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_detailList.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eRngProp);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 2;
		ctime = dequeRngAll[i].tStart;
		str = ctime.Format("%H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_detailList.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eRngProp);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 3;
		ctime = dequeRngAll[i].tStart + (time_t)(dequeRngAll[i].rtt / 2);
		str = ctime.Format("%H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_detailList.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eRngProp);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 4;
		ctime = dequeRngAll[i].tStart + (time_t)(dequeRngAll[i].rtt);
		str = ctime.Format("%H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_detailList.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eRngProp);
			return FALSE;
		}
	}
	m_detailList.EnsureVisible(index, FALSE);

	CGraphMutex::Unlock(eRngProp);

	return TRUE;
}


/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# �t�@�C�����j���[�̕��邩��Ăяo�����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CRangePropagationStatusMonitor::OnMenuClose()
{
	//	OnOK();
	ShowWindow(SW_HIDE);
}


/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# �^�C�}�C�x���g�ŌĂяo�����

@param	nIDEvent�F�C�x���gID
@retval	�Ȃ�

*/
/*============================================================================*/
void CRangePropagationStatusMonitor::OnTimer(UINT_PTR nIDEvent)
{
#ifdef _DEBUG_RNGPROPDUMMY
	theApp.GetSatelliteData().GetRangePropagation().CreateDummy();
#endif /* _DEBUG_RNGPROPDUMMY */

	// �X�e�[�^�X�X�V
	if (theApp.GetSelectStation() == eStation_Usuda642)
	{
		// 54m��
		UpdateStatus54();
	}
	else
	{
		// ����ȊO�̋�
		UpdateStatus();
	}

	// �ڍ׈ꗗ�쐬
	InsertListItem();

	// �r���[�X�V
	m_pView->Invalidate();

	CDialogBase::OnTimer(nIDEvent);
}


/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# �E�B���h�E�j�����ɌĂяo�����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CRangePropagationStatusMonitor::OnDestroy()
{
	CDialogBase::OnDestroy();

	KillTimer(1);
}

/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# �X�e�[�^�X�̍X�V

@param
@retval

*/
/*============================================================================*/
void CRangePropagationStatusMonitor::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

	UINT nStation = theApp.GetSelectStation();
	CRangePropagation& rangePropagation = theApp.GetSatelliteData().GetRangePropagation();
	UINT nTransMode = rangePropagation.GetTransMode();
	UINT nGeneMode = rangePropagation.GetGeneMode();
	UINT nStep = rangePropagation.GetStep();
	CTime& dataTime = rangePropagation.GetDataTime();
	const CString mXX_RNG_CNT = _T("CTRL.XX_RNG_GOT_CNT");
	const CString mRX_RNG_CNT = _T("CTRL.RX_RNG_GOT_CNT");

	int szControl = sizeof(mRngParam[nStation][nTransMode]) / sizeof(mRngParam[nStation][nTransMode][0]);
	for (int i = 0; i < szControl && mRngParam[nStation][nTransMode][i].id != 0; i++)
	{
		CString monitorString = mRngParam[nStation][nTransMode][i].nameK;
		CString str;
		stControlValue val;
		val.type = mRngParam[nStation][nTransMode][i].type;

		if (!monitorString.IsEmpty())
		{
			if (szControl && mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT8 && nTransMode != eRngTransMode_SS)
			{
				// RNG RCV COUNT�̒l�\���ł���S/S�łȂ��ꍇ
				if (nGeneMode == eRngGeneration_Past)
				{
					// �]���^
					obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, mXX_RNG_CNT);
				}
				else
				{
					// �Đ��^
					obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, mRX_RNG_CNT);
				}
			}
			else
			{
				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, monitorString);
#ifdef _DEBUG_RNGPROPDUMMY
				obs_adr = new CEngValCnv::obsname_t;
				sprintf_s(obs_adr->sz_sts_name, 64, ("%s"), (LPCSTR)mRngDummy[i].status);
				obs_adr->l_data = (long)mRngDummy[i].val;
				obs_adr->d_data = mRngDummy[i].val;
				obs_adr->l_inflg = INFLG_ON;
#endif
			}

			if (obs_adr == NULL)
			{
				continue;
			}
		}
		else
		{
			obs_adr = NULL;
		}

		switch (val.type){
		case	eControlStatus:
			if (obs_adr != NULL)
				str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT1)
			{
				// ANP FILE
//				str = str.Right(str.GetLength() - str.ReverseFind(_T('/')) - 1);
				str = theApp.GetSatelliteData().GetTrendData().GetAnpFileName(nStation);
			}
			else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT6 && nTransMode != eRngTransMode_SS)
			{
				// RNG FORMAT
				str = _T("PN_CODE");
			}
			break;
		case	eControlNumeric:
			if (obs_adr != NULL)
				str = CSatelliteData::AddCanma((double)obs_adr->l_data, 3);
			break;
		case	eControlDouble:
		{
			int keta;
			if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT10 ||
				mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT11)
				// RANGE, RANGE RATE
				keta = 3;
			else
				keta = 0;
			if (obs_adr != NULL)
				str = CSatelliteData::AddCanma(obs_adr->d_data, keta);
			break;
		}
		case	eControlTime:
			if (obs_adr != NULL)
				str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		default:
			break;
		}

		// DATE TIME
		if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT3)
		{
			theApp.GetCurrentUtcTime(dataTime);
			str = dataTime.Format(_T("%j %H:%M:%S"));
		}
		// RNG NUM
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT5)
		{
//			nRngNum = _wtol(str);
		}
		// RNG SEND COUNT
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT7)
		{
			// �X�e�b�v��4�ȉ��̏ꍇ
			if (nStep <= eRngStep_4)
			{
				// �����\�����Ȃ�
				str.Format(_T(" "));
			}
			else
			{
				UINT nSndCount = rangePropagation.GetSndCount();
				// �]���^���Đ��^��
				if (nGeneMode == eRngGeneration_Regene)
				{
					// �Đ��^�̏ꍇ�́A�����\�����Ȃ�
					str.Format(_T(" "));
				}
				else
				{
					// ���o�����g�[���̐���\��
					str.Format(_T("%u"), nSndCount);
				}
			}
		}
		// RNG RCV COUNT
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT8)
		{
			// �X�e�b�v��4�ȉ��̏ꍇ
			if (nStep <= eRngStep_4)
			{
				// �����\�����Ȃ�
				str.Format(_T(" "));
			}
		}
		// NEXT RCV TIME
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT9)
		{
			// �]���^���Đ��^��
			if (nGeneMode == eRngGeneration_Regene)
			{
				// �Đ��^�̏ꍇ�́A�����\�����Ȃ�
				str.Format(_T(" "));
			}
			else
			{
				// ��ʏ�ɕ����̃g�[�����\������Ă���ꍇ�A�ł��n��ǂɋ߂��g�[���̎�M������\��l����v�Z���\��
				// ��M���� = �A���e�i����̋��� / �����@+ ���ݎ���
				deque<CRangePropagation::structPropData>& dequeRngRcv = rangePropagation.GetRngRcvData();

				if (!dequeRngRcv.empty())
				{
					CTime nextRcvTime = (time_t)(dequeRngRcv[0].pos / V_C + (double)dataTime.GetTime());
					str = nextRcvTime.Format(_T("%H:%M:%S"));
				}
				else
				{
					str.Format(_T("  :  :  "));
				}
			}
		}

		// RTT
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT12)
		{
			double rtt = rangePropagation.GetRtt();
			if (rtt >= 0.0)
			{
				str = CSatelliteData::AddCanma(rtt, 9);
				str.Format(_T("%s"), (LPCTSTR)str);
			}
			else
			{
				str.Format(_T(" "));
			}
		}

		// �\��
		GetDlgItem(mRngParam[nStation][nTransMode][i].id)->SetWindowText(str);

#ifdef _DEBUG_RNGPROPDUMMY
		if (obs_adr != NULL)
			delete obs_adr;
#endif
	}
	UpdateData(FALSE);
}

#define CTRL_RNG_AREA_XX_PAST		"X/X(�]��)"		// CTRL.RNG_AREA��X/X�� �]���^������
#define CTRL_RNG_AREA_XX_REGENE		"X/X(�Đ�)"		// CTRL.RNG_AREA��X/X�� �Đ��^������
#define CTRL_RNG_AREA_XKA_PAST		"X/Ka(�]��)"	// CTRL.RNG_AREA��X/Ka�я]���^������
#define CTRL_RNG_AREA_XKA_REGENE	"X/Ka(�Đ�)"	// CTRL.RNG_AREA��X/Ka�эĐ��^������

/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# �X�e�[�^�X�̍X�V(54m)

@param
@retval

*/
/*============================================================================*/
void CRangePropagationStatusMonitor::UpdateStatus54()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

	UINT nStation = theApp.GetSelectStation();
	CRangePropagation& rangePropagation = theApp.GetSatelliteData().GetRangePropagation();
	// X��Ka�����ׂ�
	UINT nTransMode = eXmeasBandType_X;
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, JUDGE_BANDTYPE_STR);
	if (obs_adr != NULL)
	{
		CString str = CString(obs_adr->sz_sts_name);
		for (UINT i = 0; i < eXmeasBandType_Num; i++)
		{
			if (str.CompareNoCase(mBandTypeString[i]) == 0)
			{
				nTransMode = i;
				break;
			}
		}
	}

	UINT nGeneMode = rangePropagation.GetGeneMode();
	UINT nStep = rangePropagation.GetStep();
	CTime& dataTime = rangePropagation.GetDataTime();
	const CString mDX_RNG_CNT = _T("CTRL.DX_RNG_GOT_CNT");
	const CString mRX_RNG_CNT = _T("CTRL.RX_RNG_GOT_CNT");
	const CString mDKA_RNG_CNT = _T("CTRL.DKa_RNG_GOT_CNT");
	const CString mRKA_RNG_CNT = _T("CTRL.RKa_RNG_GOT_CNT");

#ifdef _DEBUG_RNGPROPDUMMY
	CEngValCnv::obsname_t obs_adr_debug;
#endif /* _DEBUG_RNGPROPDUMMY */

	int szControl = sizeof(mRngParam[nStation][nTransMode]) / sizeof(mRngParam[nStation][nTransMode][0]);
	for (int i = 0; i < szControl && mRngParam[nStation][nTransMode][i].id != 0; i++)
	{
		CString monitorString = mRngParam[nStation][nTransMode][i].nameK;
		CString str;
		stControlValue val;
		val.type = mRngParam[nStation][nTransMode][i].type;

		if (!monitorString.IsEmpty())
		{
			if (szControl && mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT8)
			{
				// RNG RCV COUNT�̒l�\���̏ꍇ�AX��Ka�����ׂ�
				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, monitorString);
#ifdef _DEBUG_RNGPROPDUMMY
//				obs_adr = &obs_adr_debug;
//				sprintf_s(obs_adr->sz_sts_name, 64, ("%s"), mRngAreaDummy[mRngAreaDummySelect]);
//				obs_adr->l_data = 0;
//				obs_adr->d_data = 0;
//				obs_adr->l_inflg = INFLG_ON;
#endif /* _DEBUG_RNGPROPDUMMY */
				if (obs_adr == NULL)
				{
					continue;
				}
				size_t len = strlen(obs_adr->sz_sts_name);
				if (len == 0)
				{
					continue;
				}
				if (len == strlen(CTRL_RNG_AREA_XX_PAST) && strncmp(obs_adr->sz_sts_name, CTRL_RNG_AREA_XX_PAST, len) == 0)
				{
					// X/X(�]��)�̏ꍇ
					obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, mDX_RNG_CNT);
#ifdef _DEBUG_RNGPROPDUMMY
//					obs_adr = &obs_adr_debug;
//					sprintf_s(obs_adr->sz_sts_name, 64, (""));
//					obs_adr->l_data = 0;
//					obs_adr->d_data = 12;
//					obs_adr->l_inflg = INFLG_ON;
#endif /* _DEBUG_RNGPROPDUMMY */
				}
				else if (len == strlen(CTRL_RNG_AREA_XX_REGENE) && strncmp(obs_adr->sz_sts_name, CTRL_RNG_AREA_XX_REGENE, len) == 0)
				{
					// X/X(�Đ�)�̏ꍇ
					obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, mRX_RNG_CNT);
#ifdef _DEBUG_RNGPROPDUMMY
//					obs_adr = &obs_adr_debug;
//					sprintf_s(obs_adr->sz_sts_name, 64, (""));
//					obs_adr->l_data = 0;
//					obs_adr->d_data = 34;
//					obs_adr->l_inflg = INFLG_ON;
#endif /* _DEBUG_RNGPROPDUMMY */
				}
				else if (len == strlen(CTRL_RNG_AREA_XKA_PAST) && strncmp(obs_adr->sz_sts_name, CTRL_RNG_AREA_XKA_PAST, len) == 0)
				{
					// X/Ka(�]��)�̏ꍇ
					obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, mDKA_RNG_CNT);
#ifdef _DEBUG_RNGPROPDUMMY
//					obs_adr = &obs_adr_debug;
//					sprintf_s(obs_adr->sz_sts_name, 64, (""));
//					obs_adr->l_data = 0;
//					obs_adr->d_data = 56;
//					obs_adr->l_inflg = INFLG_ON;
#endif /* _DEBUG_RNGPROPDUMMY */
				}
				else if (len == strlen(CTRL_RNG_AREA_XKA_REGENE) && strncmp(obs_adr->sz_sts_name, CTRL_RNG_AREA_XKA_REGENE, len) == 0)
				{
					// X/Ka(�Đ�)�̏ꍇ
					obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, mRKA_RNG_CNT);
#ifdef _DEBUG_RNGPROPDUMMY
//					obs_adr = &obs_adr_debug;
//					sprintf_s(obs_adr->sz_sts_name, 64, (""));
//					obs_adr->l_data = 0;
//					obs_adr->d_data = 78;
//					obs_adr->l_inflg = INFLG_ON;
#endif /* _DEBUG_RNGPROPDUMMY */
				}
				else
				{
					CString strDebug;
					strDebug.Format(_T("%s(%d) :: CTRL_RNG_AREA(%s) miss match"), __FILEW__, __LINE__, (LPCTSTR)monitorString);
					CLogTraceEx::Write(_T(""), _T("CRangePropagationStatusMonitor"), _T("UpdateStatus54"), _T("debug"), strDebug, nLogEx::debug);
					continue;
				}
			}
			else
			{
				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, monitorString);
#ifdef _DEBUG_RNGPROPDUMMY
				obs_adr = &obs_adr_debug;
				sprintf_s(obs_adr->sz_sts_name, 64, ("%s"), (LPCSTR)mRngDummy[i].status);
				obs_adr->l_data = (long)mRngDummy[i].val;
				obs_adr->d_data = mRngDummy[i].val;
				obs_adr->l_inflg = INFLG_ON;
#endif /* _DEBUG_RNGPROPDUMMY */
			}

			if (obs_adr == NULL)
			{
				continue;
			}
		}
		else
		{
			obs_adr = NULL;
		}

		switch (val.type){
		case	eControlStatus:
			if (obs_adr != NULL)
				str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT1)
			{
				// ANP FILE
				//				str = str.Right(str.GetLength() - str.ReverseFind(_T('/')) - 1);
				str = theApp.GetSatelliteData().GetTrendData().GetAnpFileName(nStation);
			}
			else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT6)
			{
				// RNG FORMAT
				str = _T("PN_CODE");
			}
			break;
		case	eControlNumeric:
			if (obs_adr != NULL)
				str = CSatelliteData::AddCanma((double)obs_adr->l_data, 3);
			break;
		case	eControlDouble:
		{
			int keta;
			if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT10 ||
				mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT11)
				// RANGE, RANGE RATE
				keta = 3;
			else
				keta = 0;
			if (obs_adr != NULL)
				str = CSatelliteData::AddCanma(obs_adr->d_data, keta);
			break;
		}
		case	eControlTime:
			if (obs_adr != NULL)
				str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		default:
			break;
		}

		// DATE TIME
		if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT3)
		{
			theApp.GetCurrentUtcTime(dataTime);
			str = dataTime.Format(_T("%j %H:%M:%S"));
		}
		// RNG NUM
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT5)
		{
			//			nRngNum = _wtol(str);
		}
		// RNG SEND COUNT
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT7)
		{
			// �X�e�b�v��4�ȉ��̏ꍇ
			if (nStep <= eRngStep_4)
			{
				// �����\�����Ȃ�
				str.Format(_T(" "));
			}
			else
			{
				UINT nSndCount = rangePropagation.GetSndCount();
				// �]���^���Đ��^��
				if (nGeneMode == eRngGeneration_Regene)
				{
					// �Đ��^�̏ꍇ�́A�����\�����Ȃ�
					str.Format(_T(" "));
				}
				else
				{
					// ���o�����g�[���̐���\��
					str.Format(_T("%u"), nSndCount);
				}
			}
		}
		// RNG RCV COUNT
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT8)
		{
			// �X�e�b�v��4�ȉ��̏ꍇ
			if (nStep <= eRngStep_4)
			{
				// �����\�����Ȃ�
				str.Format(_T(" "));
			}
		}
		// NEXT RCV TIME
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT9)
		{
			// �]���^���Đ��^��
			if (nGeneMode == eRngGeneration_Regene)
			{
				// �Đ��^�̏ꍇ�́A�����\�����Ȃ�
				str.Format(_T(" "));
			}
			else
			{
				// ��ʏ�ɕ����̃g�[�����\������Ă���ꍇ�A�ł��n��ǂɋ߂��g�[���̎�M������\��l����v�Z���\��
				// ��M���� = �A���e�i����̋��� / �����@+ ���ݎ���
				deque<CRangePropagation::structPropData>& dequeRngRcv = rangePropagation.GetRngRcvData();

				if (!dequeRngRcv.empty())
				{
					CTime nextRcvTime = (time_t)(dequeRngRcv[0].pos / V_C + (double)dataTime.GetTime());
					str = nextRcvTime.Format(_T("%H:%M:%S"));
				}
				else
				{
					str.Format(_T("  :  :  "));
				}
			}
		}

		// RTT
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT12)
		{
			double rtt = rangePropagation.GetRtt();
			if (rtt >= 0.0)
			{
				str = CSatelliteData::AddCanma(rtt, 9);
				str.Format(_T("%s"), (LPCTSTR)str);
			}
			else
			{
				str.Format(_T(" "));
			}
		}

		// �\��
		GetDlgItem(mRngParam[nStation][nTransMode][i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! CRangePropagationStatusMonitor

-# �X�e�[�^�X������

@param
@retval

*/
/*============================================================================*/
void CRangePropagationStatusMonitor::InitStatus()
{
	UINT nStation = theApp.GetSelectStation();
	UINT nTransMode = eRngTransMode_SS;

	int szControl = sizeof(mRngParam[nStation][nTransMode]) / sizeof(mRngParam[nStation][nTransMode][0]);
	for (int i = 0; i < szControl && mRngParam[nStation][nTransMode][i].id != 0; i++)
	{
		CString str;
		stControlValue val;
		val.type = mRngParam[nStation][nTransMode][i].type;

		switch (val.type){
		case	eControlStatus:
			str.Format(_T(""));
			break;
		case	eControlNumeric:
			str.Format(_T(""));
			break;
		case	eControlDouble:
			str.Format(_T(""));
			break;
		case	eControlTime:
			str.Format(_T(""));
			break;
		default:
			break;
		}

		if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT3)
			str.Format(_T("      :  :  "));
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT9)
			str.Format(_T("  :  :  "));

		// RNG SEND COUNT
		else if (mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT7 ||
			mRngParam[nStation][nTransMode][i].id == IDC_RANGEPROPASTATUSMON_EDIT8)
		{
			// �����\�����Ȃ�
			str.Format(_T(" "));
		}

		if (str.IsEmpty())
			str.Format(_T(" "));
		GetDlgItem(mRngParam[nStation][nTransMode][i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}

