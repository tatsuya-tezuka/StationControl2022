/*============================================================================*/
/*! ForecastValue.cpp

-# �\��l�ꗗ��ʁ@�\��l
*/
/*============================================================================*/
// ForecastValue.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "ForecastValue.h"
#include "ForecastList.h"
#include "afxdialogex.h"


// CForecastValue �_�C�A���O

IMPLEMENT_DYNAMIC(CForecastValue, CDialogBase)

/*============================================================================*/
/*! CForecastValue

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CForecastValue::CForecastValue(CWnd* pParent /*=NULL*/)
	: CDialogBase(CForecastValue::IDD, pParent)
	, m_bUpateView(FALSE)
	, m_bUpperSort(TRUE)
	, m_nSelectedColumn(0)
	, m_nPosEditCtrl(-1)
	, m_nStationIndex(-1)
	, m_strSelectedFilePath(_T(""))
{

}

/*============================================================================*/
/*! CForecastValue

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CForecastValue::~CForecastValue()
{
}

void CForecastValue::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FORECASTVALUE_LIST1, m_listForecastValue);
	DDX_Control(pDX, IDC_STATIC_FORECASTVALUE_GROUP_AREA, m_viewArea);
}


BEGIN_MESSAGE_MAP(CForecastValue, CDialogBase)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FORECASTVALUE_LIST1, &CForecastValue::OnLvnItemchangedForecastvalueList1)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CForecastValue::OnHdnItemclickForecastvalueList1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_FORECASTVALUE_LIST1, &CForecastValue::OnNMCustomdrawForecastvalueList1)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_FORECASTVALUE_LIST1, &CForecastValue::OnLvnEndlabeleditForecastvalueList1)
	ON_NOTIFY(NM_DBLCLK, IDC_FORECASTVALUE_LIST1, &CForecastValue::OnNMDblclkForecastvalueList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_FORECASTVALUE_LIST1, &CForecastValue::OnLvnColumnclickForecastvalueList1)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_FORECASTVALUE_LIST1, &CForecastValue::OnNMClickForecastvalueList1)
	ON_WM_VSCROLL()
	ON_NOTIFY(LVN_BEGINSCROLL, IDC_FORECASTVALUE_LIST1, &CForecastValue::OnLvnBeginScrollForecastvalueList1)
	ON_NOTIFY(HDN_ENDTRACK, 0, &CForecastValue::OnHdnEndtrackForecastvalueList1)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CForecastValue ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CForecastValue

-# ���X�g�R���g���[���A�C�e���ύX���ɌĂ΂��

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastValue::OnLvnItemchangedForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	*pResult = 0;
}


/*============================================================================*/
/*! CForecastValue

-# �_�C�A���O����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
BOOL CForecastValue::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_strSelectedFilePath = _T("");

	// ���X�g������
	if (!InitList())
		return FALSE;

	// ���X�g�A�C�e���}��
	if (!InsertListItem())
		return FALSE;

	// �O���[�v�r���[����
	if (!CreateGroupView())
		return FALSE;

	//��ɃX�N���[���o�[��\��������
	m_listForecastValue.EnableScrollBarCtrl(SB_VERT, TRUE);

	UpdateData(FALSE);
	UpdateWindow();

	m_listForecastValue.RedrawWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CForecastValue

-# ���X�g�R���g���[��������

@param	�Ȃ�
@retval	TRUE:���������� / FALSE:���������s

*/
/*============================================================================*/
BOOL CForecastValue::InitList()
{
	// ���X�g���ڏ�����
	LVCOLUMN lvc;
	TCHAR szMsg[256];

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_FMT;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_STATIONNAME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 0;
	lvc.pszText = szMsg;
	lvc.cx = m_nStationNameWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_listForecastValue.InsertColumn(0, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_SATELLITENAME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 1;
	lvc.pszText = szMsg;
	lvc.cx = m_nSatelliteNameWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_listForecastValue.InsertColumn(1, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_PASSID, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 2;
	lvc.pszText = szMsg;
	lvc.cx = m_nPassIDWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_listForecastValue.InsertColumn(2, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_DATETIME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 3;
	lvc.pszText = szMsg;
	lvc.cx = m_nDateWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_listForecastValue.InsertColumn(3, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_AZ, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 4;
	lvc.pszText = szMsg;
	lvc.cx = m_nAZWidth;
	lvc.fmt = LVCFMT_RIGHT;
	if (m_listForecastValue.InsertColumn(4, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_EL, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 5;
	lvc.pszText = szMsg;
	lvc.cx = m_nAZWidth;
	lvc.fmt = LVCFMT_RIGHT;
	if (m_listForecastValue.InsertColumn(5, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_DATETIME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 6;
	lvc.pszText = szMsg;
	lvc.cx = m_nDateWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_listForecastValue.InsertColumn(6, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_AZ, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 7;
	lvc.pszText = szMsg;
	lvc.cx = m_nAZWidth;
	lvc.fmt = LVCFMT_RIGHT;
	if (m_listForecastValue.InsertColumn(7, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_EL, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 8;
	lvc.pszText = szMsg;
	lvc.cx = m_nAZWidth;
	lvc.fmt = LVCFMT_RIGHT;
	if (m_listForecastValue.InsertColumn(8, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_DATETIME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 9;
	lvc.pszText = szMsg;
	lvc.cx = m_nDateWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_listForecastValue.InsertColumn(9, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_AZ, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 10;
	lvc.pszText = szMsg;
	lvc.cx = m_nAZWidth;
	lvc.fmt = LVCFMT_RIGHT;
	if (m_listForecastValue.InsertColumn(10, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_EL, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 11;
	lvc.pszText = szMsg;
	lvc.cx = m_nAZWidth;
	lvc.fmt = LVCFMT_RIGHT;
	if (m_listForecastValue.InsertColumn(11, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_OFFSET, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 12;
	lvc.pszText = szMsg;
	lvc.cx = m_nOffsetWidth;
	lvc.fmt = LVCFMT_RIGHT;
	if (m_listForecastValue.InsertColumn(12, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_FORMAT, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 13;
	lvc.pszText = szMsg;
	lvc.cx = m_nFormatWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_listForecastValue.InsertColumn(13, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_GETFROM, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 14;
	lvc.pszText = szMsg;
	lvc.cx = m_nGetStateWidth;
	lvc.fmt = LVCFMT_LEFT;
	if (m_listForecastValue.InsertColumn(14, &lvc) < 0)
		return FALSE;

	m_listForecastValue.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	return TRUE;
}

/*============================================================================*/
/*! CForecastValue

-# ���X�g�R���g���[���ɃA�C�e���}��

@param	�Ȃ�
@retval	TRUE:�}������ / FALSE:�}�����s

*/
/*============================================================================*/
BOOL CForecastValue::InsertListItem()
{
	CForecastList* pWnd = (CForecastList*)GetParent();
	if (pWnd == NULL)
		return FALSE;

	CGraphMutex::Lock(eFile);
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();

	SetStationIndex();
	SetSatelliteName();

	m_listForecastValue.DeleteAllItems();

	// �w�i�F���Z�b�g
	map<CString, COLORREF>	mapColor;
	int sz = sizeof(mStationSimpleString) / sizeof(mStationSimpleString[0]);
	for (int i = 0; i < sz; i++)
	{
//		mapColor.insert(map<CString, COLORREF>::value_type(mStationSimpleString[i], mStationColor[i]));
		mapColor.insert(map<CString, COLORREF>::value_type(mStationString[i], mStationColor[i]));
	}
	m_listForecastValue.SetColorMap(mapColor);

	// ���X�g�R���g���[���ɃA�C�e���}��
	LVITEM lvi;
	CString str;
	int i, index;
	CTime ctime;

	for (i = 0, index = 0; i < (int)listPred.size(); i++)
	{
		// �ǖ���ALL�܂��͈�v�̂Ƃ��Ƀ��X�g�ɒǉ�
//		str.Format(mStationSimpleString[m_nStationIndex]);
//		str = pWnd->GetSimpleStationName();
//		if (!(m_strStationName.CompareNoCase(_T("All")) == 0 || str.Compare(listPred[i].strStationName) == 0))
		if (!(m_strStationName.CompareNoCase(_T("All")) == 0 || m_strStationName.Compare(listPred[i].strStationName) == 0))
			continue;

		// �q������ALL�܂��͈�v�̂Ƃ��Ƀ��X�g�ɒǉ�
		if (!(m_strSatelliteName.CompareNoCase(_T("All")) == 0 ||
			m_strSatelliteName.CompareNoCase(listPred[i].strSatellite) == 0))
			continue;

		// ���X�g�R���g���[���ɓ��e�Z�b�g
		lvi.mask = LVIF_TEXT;

		lvi.iItem = i;
		lvi.iSubItem = 0;
		str.Format(listPred[i].strStationName);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if ((index = m_listForecastValue.InsertItem(&lvi)) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 1;
		str.Format(listPred[i].strSatellite);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 2;
		str.Format(listPred[i].strPassId);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 3;
		ctime = listPred[i].tAOS;
		str = ctime.Format("%Y-%m-%d %H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 4;
		str.Format(_T("%03.2f"), listPred[i].dAosAz);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 5;
		str.Format(_T("%03.2f"), listPred[i].dAosEl);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 6;
		ctime = listPred[i].tLOS;
		str = ctime.Format("%Y-%m-%d %H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 7;
		str.Format(_T("%03.2f"), listPred[i].dLosAz);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 8;
		str.Format(_T("%03.2f"), listPred[i].dLosEl);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 9;
		ctime = listPred[i].tMaxEl;
		str = ctime.Format("%Y-%m-%d %H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 10;
		str.Format(_T("%03.2f"), listPred[i].dMaxAz);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 11;
		str.Format(_T("%03.2f"), listPred[i].dMaxEl);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 12;
		long nOffset = labs(listPred[i].tOffset);
		if (listPred[i].tOffset >= 0)
		{
			str.Format(_T("%02d:%02d"), nOffset / 60, nOffset % 60);
		}
		else
		{
			str.Format(_T("-%02d:%02d"), nOffset / 60, nOffset % 60);
		}

		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 13;
		str.Format(listPred[i].strFormat);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 14;
		str.Format(listPred[i].strGetFrom);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listForecastValue.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}
	}
	CGraphMutex::Unlock(eFile);

	for (int i = 0; i < m_listForecastValue.GetItemCount(); i++) {
		m_listForecastValue.SetItemData(i, i);
	}

	return TRUE;
}

/*============================================================================*/
/*! CForecastValue

-# ���X�g�R���g���[���̃A�C�e�����X�V����

@param	�Ȃ�
@retval	TRUE:�X�V���� / FALSE:�X�V���s

*/
/*============================================================================*/
BOOL CForecastValue::UpdateListItem()
{
	int nItemCount = m_listForecastValue.GetItemCount();
	CRect rect;
	m_listForecastValue.GetItemRect(0, &rect, LVIR_BOUNDS);
	int index = m_listForecastValue.GetTopIndex();
	int offset = rect.Height() * index;
	CSize cs;
	cs.cx = 0;
	cs.cy = offset;

	// �A�C�e���}��
	InsertListItem();

	// �A�C�e�������ς���Ă��Ȃ��ꍇ�́A�L�^�����X�N���[���ʒu�ɃZ�b�g����
	if (nItemCount == m_listForecastValue.GetItemCount())
	{
		if (offset)
		{
			m_listForecastValue.Scroll(cs);
		}
	}

	CString strStation, strSatellite, strPassId, strPredPath;
	CGraphMutex::Lock(eFile);
	vector<stPredListData>::iterator itr;
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();

	BOOL bFind = FALSE;

	nItemCount = m_listForecastValue.GetItemCount();
	for (int i = 0; i < nItemCount && bFind == FALSE; i++)
	{
		strStation = m_listForecastValue.GetItemText(i, 0);
		strSatellite = m_listForecastValue.GetItemText(i, 1);
		strPassId = m_listForecastValue.GetItemText(i, 2);
		for (itr = listPred.begin(); itr != listPred.end() && bFind == FALSE; itr++)
		{
			if ((*itr).strStationName.Compare(strStation) == 0 &&
				(*itr).strSatellite.Compare(strSatellite) == 0 &&
				(*itr).strPassId.Compare(strPassId) == 0)
			{
				// �ǁA�q���A�\��lID����v
				if (((*itr).strPredPath).CompareNoCase(m_strSelectedFilePath) == 0)
				{
					// �t�@�C���p�X����v������A���X�g�R���g���[���I��
					if (m_listForecastValue.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED) == 0)
					{
						// ���X�g�I���ł��Ȃ������ꍇ�́A�t�@�C���p�X���N���A
						m_strSelectedFilePath = _T("");
					}

					bFind = TRUE;
				}
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	InitRadarGraphView();

	return TRUE;
}


/*============================================================================*/
/*! CForecastValue

-# �O���[�v�̃r���[�쐬

@param	�Ȃ�
@retval	TRUE:�}������ / FALSE:�}�����s

*/
/*============================================================================*/
BOOL CForecastValue::CreateGroupView()
{
	// �r���[�̍쐬
	CRect rect;
	m_viewArea.GetWindowRect(&rect);
	ScreenToClient(&rect);
	CRuntimeClass* pClass = RUNTIME_CLASS(CForecastValueGroupView);
	if (pClass == NULL)
	{
		return FALSE;
	}

	m_pView = (CForecastValueGroupView*)(pClass->CreateObject());
	if (m_pView != NULL)
	{
		m_pView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, 0, NULL);
		m_pView->SendMessage(WM_INITIALUPDATE);
	}

	return TRUE;
}


/*============================================================================*/
/*! CForecastValue

-# AOS�O���[�v��X�ʒu�擾

@param	stXPos�F�O���[�v�̊J�n�ʒu�A�I���ʒu
@retval	�Ȃ�

*/
/*============================================================================*/
void CForecastValue::GetAosXPos(structGroupXPos& stXPos)
{
	int xStart = 0;
	int xStop = 0;

	// AOS���O
	for (int i = 0; i < 3; ++i)
	{
		xStart += m_listForecastValue.GetColumnWidth(i);
	}
	
	// AOS�̕�
	for (int i = 3; i <6; ++i)
	{
		xStop += m_listForecastValue.GetColumnWidth(i);
	}

	stXPos.x1 = xStart;
	stXPos.x2 = xStart + xStop;
}

/*============================================================================*/
/*! CForecastValue

-# LOS�O���[�v��X�ʒu�擾

@param	stXPos�F�O���[�v�̊J�n�ʒu�A�I���ʒu
@retval	�Ȃ�

*/
/*============================================================================*/
void CForecastValue::GetLosXPos(structGroupXPos& stXPos)
{
	structGroupXPos stStartXPos;
	int xStop = 0;

	// LOS���O
	GetAosXPos(stStartXPos);

	// LOS�̕�
	for (int i = 6; i < 9; ++i)
	{
		xStop += m_listForecastValue.GetColumnWidth(i);
	}

	stXPos.x1 = stStartXPos.x2;
	stXPos.x2 = stStartXPos.x2 + xStop;
}

/*============================================================================*/
/*! CForecastValue

-# MAX EL�O���[�v��X�ʒu�擾

@param	stXPos�F�O���[�v�̊J�n�ʒu�A�I���ʒu
@retval	�Ȃ�

*/
/*============================================================================*/
void CForecastValue::GetMaxElXPos(structGroupXPos& stXPos)
{
	structGroupXPos stStartXPos;
	int xStop = 0;

	// MAX EL���O
	GetLosXPos(stStartXPos);

	// LOS�̕�
	for (int i = 9; i < 12; ++i)
	{
		xStop += m_listForecastValue.GetColumnWidth(i);
	}

	stXPos.x1 = stStartXPos.x2;
	stXPos.x2 = stStartXPos.x2 + xStop;
}


/*============================================================================*/
/*! CForecastValue

-# ���X�g�R���g���[���\�[�g���̃R�[���o�b�N�֐�

@param	lParam1		��r���A�C�e��
@param	lParam2		��r��A�C�e��
@param	lParamSort	�E�B���h�E�n���h��
@retval 0: ��v, -1:��r���̏��Ԃ��Ⴂ, 1:��r���̏��Ԃ�����

*/
/*============================================================================*/
int CALLBACK CForecastValue::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CForecastValue* dlg = (CForecastValue*)lParamSort;
	return dlg->CompareFunc(lParam1, lParam2);

}

/*============================================================================*/
/*! CForecastValue

-# ���X�g�R���g���[���\�[�g���̔�r�֐�

@param	lParam1		��r���A�C�e��
@param	lParam2		��r��A�C�e��
@retval 0: ��v, -1:��r���̏��Ԃ��Ⴂ, 1:��r���̏��Ԃ�����

*/
/*============================================================================*/
int CForecastValue::CompareFunc(LPARAM lParam1, LPARAM lParam2)
{
	CString str[2];
	LPARAM param[2] = { lParam1, lParam2 };

	// ���X�g�R���g���[�������r����A�C�e���擾
	for (int i = 0; i < 2; ++i)
	{
		LVFINDINFO fi = { 0 };
		fi.flags = LVFI_PARAM;
		fi.lParam = param[i];
		str[i] = m_listForecastValue.GetItemText(m_listForecastValue.FindItem(&fi), m_nSelectedColumn);
	}

	// ��r����
	int ret = 0;
	if (m_bUpperSort)
	{
		ret = str[0].CompareNoCase(str[1]);
	}
	else
	{
		ret = str[1].CompareNoCase(str[0]);
	}

	return ret;
}


/*============================================================================*/
/*! CForecastValue

-# �`�掞�ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastValue::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	// �`�惁�b�Z�[�W�� CDialogBase::OnPaint() ���Ăяo���Ȃ��ł��������B
}


/*============================================================================*/
/*! CForecastValue

-# �`�掞�ɌĂ΂�A�w��R���g���[���̔w�i��h��Ԃ��u���V�̃n���h����Ԃ�

@param	pDC�F�q�E�B���h�E�̃f�B�X�v���C�R���e�L�X�g�̃|�C���^
@param	pWnd�F�F���i�[���ꂽ�R���g���[���̃|�C���^
@param	nCtrlColor�F�R���g���[���̎��
@retval	�R���g���[���̔w�i��h��Ԃ��u���V�̃n���h��

*/
/*============================================================================*/
HBRUSH CForecastValue::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogBase::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG && pWnd == this)
	{
		hbr = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
	}

	// TODO: ����l���g�p�������Ȃ��ꍇ�͕ʂ̃u���V��Ԃ��܂��B
	return hbr;
}


/*============================================================================*/
/*! CForecastValue

-# ���X�g�R���g���[���̍��ڃN���b�N���ɌĂ΂��

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastValue::OnHdnItemclickForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	if (phdr == NULL)
		return;

	m_nSelectedColumn = phdr->iItem;

//	CancelOffsetTime();

	for (int i = 0; i < m_listForecastValue.GetItemCount(); i++) {
		m_listForecastValue.SetItemData(i, i);
	}
	m_listForecastValue.SortItems(&CForecastValue::CompareFunc, (LPARAM)this);

	m_bUpperSort = !m_bUpperSort;

	*pResult = 0;
}


/*============================================================================*/
/*! CForecastValue

-# ���X�g�R���g���[���̃J�X�^���`�掞�ɌĂ΂��

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastValue::OnNMCustomdrawForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	if (m_pView == NULL)
		return;

	m_pView->Invalidate();

	*pResult = 0;
}

/*============================================================================*/
/*! CForecastValue

-# �ǖ����ύX����Ă���ꍇ�ɋǖ����Z�b�g

@param	�Ȃ�
@retval TRUE: �ύX����Ă���B FALSE: �ύX����Ă��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastValue::SetStationName()
{
	CForecastList* pWnd = (CForecastList*)GetParent();
	if (pWnd == NULL)
	{
		return FALSE;
	}
	
	CString str = pWnd->GetStationName();
	// �ǖ����ύX���ꂽ�ꍇ
	if (m_strStationName.CompareNoCase(str) != 0)
	{
		// �ǖ����Z�b�g����
		m_strStationName = str;
		return TRUE;
	}

	return FALSE;
}

/*============================================================================*/
/*! CForecastValue

-# �ǃC���f�b�N�X���Z�b�g

@param	�Ȃ�
@retval TRUE: �擾�����B FALSE: �擾���s�B

*/
/*============================================================================*/
BOOL CForecastValue::SetStationIndex()
{
	SetStationName();

	CForecastList* pWnd = (CForecastList*)GetParent();
	if (pWnd == NULL)
	{
		return FALSE;
	}

	int nIndex = pWnd->GetStationIndex();
	if (pWnd->GetStationCount() > 1)
	{
		nIndex--;
	}
	m_nStationIndex = nIndex;

	return TRUE;
}


/*============================================================================*/
/*! CForecastValue

-# �q�������ύX����Ă���ꍇ�ɉq�������Z�b�g

@param	�Ȃ�
@retval TRUE: �ύX����Ă���B FALSE: �ύX����Ă��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastValue::SetSatelliteName()
{
	CForecastList* pWnd = (CForecastList*)GetParent();
	if (pWnd == NULL)
	{
		return FALSE;
	}

	CString str = pWnd->GetSatelliteName();
	// �q�������ύX���ꂽ�ꍇ
	if (m_strSatelliteName.CompareNoCase(str) != 0)
	{
		// �q�������Z�b�g����
		m_strSatelliteName = str;
		return TRUE;
	}

	return FALSE;
}


/*============================================================================*/
/*! CForecastValue

-# �I������Ă���\��l���X�g�̈�ԍŏ��̃t�@�C���p�X��Ԃ�

@param	strFilePath�F�t�@�C���p�X
@param	bChk�FTRUE:�I������Ă��邩�`�F�b�N���� / FALSE:�`�F�b�N���Ȃ�
@retval TRUE: �Ԃ��B FALSE: �Ԃ��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastValue::GetSelectedForecastFilePath(CString& strFilePath, BOOL bChk /*= FALSE*/)
{
	POSITION pos;
	int nItem;
	CString strStationName, strSatellite, strPassId;

	pos = m_listForecastValue.GetFirstSelectedItemPosition();
	if (bChk && !pos)
	{
		// �I������Ă��Ȃ����b�Z�[�W�\��
		CForecastList* pWnd = (CForecastList*)GetParent();
		if (pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->ViewUnSelectedMessage();
			return FALSE;
		}
	}

	CGraphMutex::Lock(eFile);
	vector<stPredListData>::iterator itr;
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();

	while (pos)
	{
		nItem = m_listForecastValue.GetNextSelectedItem(pos);
		strStationName = m_listForecastValue.GetItemText(nItem, 0);
		strSatellite = m_listForecastValue.GetItemText(nItem, 1);
		strPassId = m_listForecastValue.GetItemText(nItem, 2);
		for (itr = listPred.begin(); itr != listPred.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationName) == 0 &&
				(*itr).strSatellite.Compare(strSatellite) == 0 &&
				(*itr).strPassId.Compare(strPassId) == 0)
			{
				strFilePath = ((*itr).strPredPath);
				// �g���q��prd�̏ꍇ�Aanp�ɒu��������
				strFilePath.TrimRight(PRED_FILE_TYPE);
				strFilePath += ANP_FILE_TYPE;

				CGraphMutex::Unlock(eFile);
				return TRUE;
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	return FALSE;
}


/*============================================================================*/
/*! CForecastValue

-# �I������Ă���\��l���X�g�̈�ԍŏ��̋ǖ��ƃt�@�C���p�X��Ԃ�

@param	strStationName�F�ǖ�
@param	strFilePath�F�t�@�C���p�X
@param	nFormat�F0 �߉F��/ 1 �[�F��
@param	bChk�FTRUE:�I������Ă��邩�`�F�b�N���� / FALSE:�`�F�b�N���Ȃ�
@retval TRUE: �Ԃ��B FALSE: �Ԃ��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastValue::GetSelectedForecastStationNameAndFilePath(CString& strStationName, CString& strFilePath, int& nFormat, BOOL bChk /*= FALSE*/)
{
	POSITION pos;
	int nItem;
	CString strStationNameTmp, strSatellite, strPassId;

	pos = m_listForecastValue.GetFirstSelectedItemPosition();
	if (bChk && !pos)
	{
		// �I������Ă��Ȃ����b�Z�[�W�\��
		CForecastList* pWnd = (CForecastList*)GetParent();
		if (pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->ViewUnSelectedMessage();
			return FALSE;
		}
	}

	CGraphMutex::Lock(eFile);
	vector<stPredListData>::iterator itr;
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();

	CString strFormat = _T("");
	while (pos)
	{
		nItem = m_listForecastValue.GetNextSelectedItem(pos);
		strStationNameTmp = m_listForecastValue.GetItemText(nItem, 0);
		strSatellite = m_listForecastValue.GetItemText(nItem, 1);
		strPassId = m_listForecastValue.GetItemText(nItem, 2);
		strFormat = m_listForecastValue.GetItemText(nItem, 13);
		for (itr = listPred.begin(); itr != listPred.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationNameTmp) == 0 &&
				(*itr).strSatellite.Compare(strSatellite) == 0 &&
				(*itr).strPassId.Compare(strPassId) == 0)
			{
				strStationName = ((*itr).strStationName);
				CString strFileName = (*itr).strPassId;
				strFileName.Remove('-');
				strFilePath.Format(_T("%s\\%s.%s"), (LPCTSTR)(*itr).strSatellite, (LPCTSTR)strFileName, PRED_FILE_TYPE);

				if (strFormat.CompareNoCase(_T("�[�F��")) == 0)
				{
					nFormat = 1;
				}
				else
				{
					nFormat = 0;
				}

				CGraphMutex::Unlock(eFile);
				return TRUE;
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	return FALSE;
}


/*============================================================================*/
/*! CForecastValue

-# �I������Ă���\��l���X�g�̈�ԍŏ��̋ǖ��A�q�����A�p�XID�A�t���t�@�C���p�X��Ԃ�

@param	strStationName�F�ǖ�
@param	strSatelliteName�F�q����
@param	strPassId�F�p�XID
@param	strFilePath�F�t���t�@�C���p�X
@param	bChk�FTRUE:�I������Ă��邩�`�F�b�N���� / FALSE:�`�F�b�N���Ȃ�
@retval TRUE: �Ԃ��B FALSE: �Ԃ��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastValue::GetSelectedForecastStationNameAndFullFilePath(CString& strStationName, CString& strSatelliteName, CString& strPassId, CString& strFilePath, BOOL bChk /*= FALSE*/)
{
	POSITION pos;
	int nItem;
	CString strStationNameTmp, strSatelliteTmp, strPassIdTmp;

	pos = m_listForecastValue.GetFirstSelectedItemPosition();
	if (bChk && !pos)
	{
		// �I������Ă��Ȃ����b�Z�[�W�\��
		CForecastList* pWnd = (CForecastList*)GetParent();
		if (pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->ViewUnSelectedMessage();
			return FALSE;
		}
	}

	CGraphMutex::Lock(eFile);
	vector<stPredListData>::iterator itr;
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();

	while (pos)
	{
		nItem = m_listForecastValue.GetNextSelectedItem(pos);
		strStationNameTmp = m_listForecastValue.GetItemText(nItem, 0);
		strSatelliteTmp = m_listForecastValue.GetItemText(nItem, 1);
		strPassIdTmp = m_listForecastValue.GetItemText(nItem, 2);
		for (itr = listPred.begin(); itr != listPred.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationNameTmp) == 0 &&
				(*itr).strSatellite.Compare(strSatelliteTmp) == 0 &&
				(*itr).strPassId.Compare(strPassIdTmp) == 0)
			{
				strStationName = ((*itr).strStationName);
				strSatelliteName = ((*itr).strSatellite);
				strPassId = ((*itr).strPassId);
				strFilePath = ((*itr).strPredPath);
				CGraphMutex::Unlock(eFile);
				return TRUE;
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	return FALSE;
}


/*============================================================================*/
/*! CForecastValue

-# �I������Ă���\��l���X�g�̈�ԍŏ��̋ǖ��A�q�����A�p�XID�A�t���t�@�C���p�X��Ԃ�

@param	strStationName�F�ǖ�
@param	strSatelliteName�F�q����
@param	strPassId�F�p�XID
@param	strFilePath�F�t���t�@�C���p�X
@param	bChk�FTRUE:�I������Ă��邩�`�F�b�N���� / FALSE:�`�F�b�N���Ȃ�
@retval TRUE: �Ԃ��B FALSE: �Ԃ��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastValue::GetSelectedForecastStationNameAndFullFilePath(vector<stPredListData>& listPred, BOOL bChk /*= FALSE*/)
{
	POSITION pos;
	int nItem;
	CString strStationNameTmp, strSatelliteTmp, strPassIdTmp;

	BOOL bRet = FALSE;

	pos = m_listForecastValue.GetFirstSelectedItemPosition();
	if (bChk && !pos)
	{
		// �I������Ă��Ȃ����b�Z�[�W�\��
		CForecastList* pWnd = (CForecastList*)GetParent();
		if (pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->ViewUnSelectedMessage();
			return bRet;
		}
	}

	CGraphMutex::Lock(eFile);
	vector<stPredListData>::iterator itr;
	vector<stPredListData>& srcListPred = theApp.GetSatelliteData().GetPredList();

	while (pos)
	{
		nItem = m_listForecastValue.GetNextSelectedItem(pos);
		strStationNameTmp = m_listForecastValue.GetItemText(nItem, 0);
		strSatelliteTmp = m_listForecastValue.GetItemText(nItem, 1);
		strPassIdTmp = m_listForecastValue.GetItemText(nItem, 2);
		for (itr = srcListPred.begin(); itr != srcListPred.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationNameTmp) == 0 &&
				(*itr).strSatellite.Compare(strSatelliteTmp) == 0 &&
				(*itr).strPassId.Compare(strPassIdTmp) == 0)
			{
				listPred.push_back((*itr));
				bRet = TRUE;
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	return bRet;
}


/*============================================================================*/
/*! CForecastValue

-# �I������Ă���\��l���X�g�̈�ԍŏ��̃I�t�Z�b�g������Ԃ�

@param	offsetTime�F�I�t�Z�b�g����
@retval TRUE: �Ԃ��B FALSE: �Ԃ��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastValue::GetSelectedForecastOffsetTime(time_t& offsetTime)
{
	POSITION pos;
	int nItem;
	CString strStationNameTmp, strSatellite, strPassId;

	pos = m_listForecastValue.GetFirstSelectedItemPosition();
	if (!pos)
	{
		// �I������Ă��Ȃ����b�Z�[�W�\��
		CForecastList* pWnd = (CForecastList*)GetParent();
		if (pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->ViewUnSelectedMessage();
			return FALSE;
		}
	}

	CGraphMutex::Lock(eFile);
	vector<stPredListData>::iterator itr;
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();

	while (pos)
	{
		nItem = m_listForecastValue.GetNextSelectedItem(pos);
		strStationNameTmp = m_listForecastValue.GetItemText(nItem, 0);
		strSatellite = m_listForecastValue.GetItemText(nItem, 1);
		strPassId = m_listForecastValue.GetItemText(nItem, 2);
		for (itr = listPred.begin(); itr != listPred.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationNameTmp) == 0 &&
				(*itr).strSatellite.Compare(strSatellite) == 0 &&
				(*itr).strPassId.Compare(strPassId) == 0)
			{
				offsetTime = ((*itr).tOffset);
				CGraphMutex::Unlock(eFile);
				return TRUE;
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	return FALSE;
}


/*============================================================================*/
/*! CForecastValue

-# �\��l�ꗗ����ǖ����X�g���擾����

@param	listName�F�ǖ����X�g
@retval TRUE: �Ԃ��B FALSE: �Ԃ��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastValue::GetStationNameList(vector<CString>& listName)
{
	CGraphMutex::Lock(eFile);
	vector<stPredListData>::iterator itr;
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();
	vector<CString>::iterator itrName;

	listName.clear();

	for (itr = listPred.begin(); itr != listPred.end(); itr++)
	{
		// �ǖ������ɋǖ����X�g�ɂ��邩���ׂ�
		if (listName.empty())
		{
			listName.push_back((*itr).strStationName);
		}
		else
		{
			itrName = find(listName.begin(), listName.end(), (*itr).strStationName);
			if (itrName == listName.end())
			{
				// �����ꍇ�͋ǖ����X�g�ɒǉ�
				listName.push_back((*itr).strStationName);
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	if (listName.empty() == TRUE)
		return FALSE;

	return TRUE;
}


/*============================================================================*/
/*! CForecastValue

-# �\��l�ꗗ����q�������X�g���擾����

@param	listName�F�q�������X�g
@retval TRUE: �Ԃ��B FALSE: �Ԃ��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastValue::GetSatelliteNameList(vector<CString>& listName)
{
	CGraphMutex::Lock(eFile);
	vector<stPredListData>::iterator itr;
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();
	vector<CString>::iterator itrName;

	listName.clear();

	for (itr = listPred.begin(); itr != listPred.end(); itr++)
	{
		// �q���������ɉq�������X�g�ɂ��邩���ׂ�
		if (listName.empty())
		{
			listName.push_back((*itr).strSatellite);
		}
		else
		{
			itrName = find(listName.begin(), listName.end(), (*itr).strSatellite);
			if (itrName == listName.end())
			{
				// �����ꍇ�͋ǖ����X�g�ɒǉ�
				listName.push_back((*itr).strSatellite);
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	if (listName.empty() == TRUE)
		return FALSE;

	return TRUE;
}


/*============================================================================*/
/*! CForecastValue

-# �ǔ��O���t������������B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CForecastValue::InitRadarGraphView()
{
	// �I������Ă���\��l�̈�ԏ�̋ǖ��ƃt�@�C���p�X���擾
	CString strStationName, strSatellite, strPassId, strFilePath;
	
	if (GetSelectedForecastStationNameAndFullFilePath(strStationName, strSatellite, strPassId, strFilePath))
	{
		// �I������Ă���ꍇ�A�V�����t�@�C���p�X�擾
		m_strSelectedFilePath = strFilePath;
		CString strPredFileName = strPassId;
		strPredFileName.Remove('-');
		strFilePath.Format(_T("%s\\%s.%s"), (LPCTSTR)strSatellite, (LPCTSTR)strPredFileName, PRED_FILE_TYPE);		// �\��lID����Windows�Ǌǐ��T�[�o�̃t�@�C���p�X�����
	}
	else
	{
		// �I������Ă��Ȃ��ꍇ
		m_strSelectedFilePath = _T("");
		strFilePath = _T("");
	}

	// �ǖ�����ǔԍ��擾
	int nStation = CForecastList::GetStationIndex(strStationName);

	CForecastList* pWnd = (CForecastList*)GetParent();
	if (pWnd == NULL)
		return;

	CForecastRadarGraphView* pView = (CForecastRadarGraphView*)pWnd->GetView();
	if (pView == NULL || pView->GetSafeHwnd() == NULL)
		return;

	CString strFullPath;
	if (strFilePath.IsEmpty() || nStation < 0)
	{
		// ���I���������̓t�@�C���������ꍇ
		nStation = -1;
		strFullPath = _T("");
	}
	else
	{
		strFullPath = theApp.GetShareFilePath(eFileType_Pred_Folder, nStation) + strFilePath;
	}
	// �\��l�̒ǔ��O���t�ɂ���
	pView->SetViewtype(eRadarViewType_ForecastList);
	pView->SetStationID(nStation);
	pView->SetPlanID(_T(""), 0, 0);		// �\��l�ꗗ�Ȃ̂Ōv��̏��Ȃ�
	// �ǔ��O���t������������
	pView->Init(strFullPath, strSatellite, strPassId);
}


/*============================================================================*/
/*! CForecastValue

-# ���X�g�R���g���[���̍��ڕҏW�I���ɌĂ΂��B

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�

*/
/*============================================================================*/
void CForecastValue::OnLvnEndlabeleditForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	*pResult = 0;
}


/*============================================================================*/
/*! CForecastValue

-# ���X�g�R���g���[�����N���b�N���ɌĂ΂��B

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�

*/
/*============================================================================*/
void CForecastValue::OnNMClickForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	InitRadarGraphView();
}


/*============================================================================*/
/*! CForecastValue

-# ���X�g�R���g���[�����_�u���N���b�N���ɌĂ΂��B

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�

*/
/*============================================================================*/
void CForecastValue::OnNMDblclkForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult)
{

}


/*============================================================================*/
/*! CForecastValue

-# ���X�g�R���g���[���̗���N���b�N���ɌĂ΂��B

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�

*/
/*============================================================================*/
void CForecastValue::OnLvnColumnclickForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	*pResult = 0;
}


/*============================================================================*/
/*! CForecastValue

-# ���b�Z�[�W���f�B�X�p�b�`�����O�ɌĂ΂��B

@param  pMsg�F�ʒm���b�Z�[�W�|�C���^
@retval �Ȃ�

*/
/*============================================================================*/
BOOL CForecastValue::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B
	if (pMsg == NULL)
		return FALSE;

	return CDialogBase::PreTranslateMessage(pMsg);
}


/*============================================================================*/
/*! CForecastValue

-# �E�B���h�E�j�����ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CForecastValue::OnDestroy()
{
	CDialogBase::OnDestroy();
}


/*============================================================================*/
/*! CForecastValue

-# �X�N���[���J�n���ɌĂ΂��B

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�

*/
/*============================================================================*/
void CForecastValue::OnLvnBeginScrollForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// ���̋@�\�� Internet Explorer 5.5 ������ȍ~�̃o�[�W������K�v�Ƃ��܂��B
	// �V���{�� _WIN32_IE �� >= 0x0560 �ɂȂ�Ȃ���΂Ȃ�܂���B
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);
	// �����I�t�Z�b�g���͂��N���A
//	CancelOffsetTime();

	*pResult = 0;
}


/*============================================================================*/
/*! CForecastValue

-# �����I�t�Z�b�g�R���g���[�����t�H�[�J�X�����������̏���

@param  message	�F���b�Z�[�W�̎��ʎq
@param  wParam	�F���b�Z�[�W�̍ŏ��̃p�����[�^
@param  lParam	�F���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
@retval ��������
*/
/*============================================================================*/
LRESULT CForecastValue::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CDialogBase::WindowProc(message, wParam, lParam);
}


/*============================================================================*/
/*! CForecastValue

-# �敪���g���b�N�I�����ɌĂ΂��B

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�

*/
/*============================================================================*/
void CForecastValue::OnHdnEndtrackForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	if (m_pView == NULL)
		return;

	m_pView->Invalidate();

	*pResult = 0;
}

