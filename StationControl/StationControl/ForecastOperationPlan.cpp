/*============================================================================*/
/*! ForecastOperationPlan.cpp

-# �\��l�ꗗ��ʁ@�^�p�v��
*/
/*============================================================================*/
// ForecastOperationPlan.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "ForecastOperationPlan.h"
#include "ForecastList.h"
#include "afxdialogex.h"

//----------------------------------------------------------------------
// �_�~�[�f�[�^

// �v��ꗗ�X�V�m�F����
#define  INTERVAL_REAL	1000						// �g�����h �\���J�n���ԁi�~���b�j

// CForecastOperationPlan �_�C�A���O

IMPLEMENT_DYNAMIC(CForecastOperationPlan, CDialogBase)

/*============================================================================*/
/*! CForecastOperationPlan

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CForecastOperationPlan::CForecastOperationPlan(CWnd* pParent /*=NULL*/)
	: CDialogBase(CForecastOperationPlan::IDD, pParent)
	, m_bUpperSort(TRUE)
	, m_nSelectedColumn(0)
	, m_nStationIndex(-1)
	, m_strSelectedStation(_T(""))
	, m_strSelectedSatellite(_T(""))
	, m_strSelectedPassId(_T(""))
	, m_strSelectedGetFrom(_T(""))
	, m_strSelectedFile(_T(""))
{

}

/*============================================================================*/
/*! CForecastOperationPlan

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CForecastOperationPlan::~CForecastOperationPlan()
{
}

void CForecastOperationPlan::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FORECASTOPERATIONPLAN_LIST1, m_listOperationPlan);
}


BEGIN_MESSAGE_MAP(CForecastOperationPlan, CDialogBase)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CForecastOperationPlan::OnHdnItemclickForecastoperationplanList1)
	ON_NOTIFY(NM_CLICK, IDC_FORECASTOPERATIONPLAN_LIST1, &CForecastOperationPlan::OnNMClickForecastoperationplanList1)
//	ON_WM_TIMER()
END_MESSAGE_MAP()


// CForecastOperationPlan ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CForecastOperationPlan

-# �_�C�A���O����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
BOOL CForecastOperationPlan::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_strSelectedStation = _T("");
	m_strSelectedSatellite = _T("");
	m_strSelectedPassId = _T("");
	m_strSelectedGetFrom = _T("");
	m_strSelectedFile = _T("");

	// ���X�g������
	if (!InitList())
		return FALSE;

	// ���X�g�A�C�e���}��
	if (!InsertListItem())
		return FALSE;

	//��ɃX�N���[���o�[��\��������
	m_listOperationPlan.EnableScrollBarCtrl(SB_VERT, TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CForecastOperationPlan

-# ���X�g�R���g���[��������

@param	�Ȃ�
@retval	TRUE:���������� / FALSE:���������s

*/
/*============================================================================*/
BOOL CForecastOperationPlan::InitList()
{
	LVCOLUMN lvc;
	TCHAR szMsg[256];

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTOPERATIONPLAN_STATIONNAME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 0;
	lvc.pszText = szMsg;
	lvc.cx = m_nStationNameWidth;
	if (m_listOperationPlan.InsertColumn(0, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTOPERATIONPLAN_SATELLITENAME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 1;
	lvc.pszText = szMsg;
	lvc.cx = m_nSatelliteNameWidth;
	if (m_listOperationPlan.InsertColumn(1, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTVALUE_PASSID, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 2;
	lvc.pszText = szMsg;
	lvc.cx = m_nPassIdWidth;
	if (m_listOperationPlan.InsertColumn(2, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTOPERATIONPLAN_PREPASSSTARTTIME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 3;
	lvc.pszText = szMsg;
	lvc.cx = m_prePassStartTimeWidth;
	if (m_listOperationPlan.InsertColumn(3, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTOPERATIONPLAN_PASSSTARTTIME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 4;
	lvc.pszText = szMsg;
	lvc.cx = m_passStartTimeWidth;
	if (m_listOperationPlan.InsertColumn(4, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTOPERATIONPLAN_PASSENDTIME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 5;
	lvc.pszText = szMsg;
	lvc.cx = m_passEndTimeWidth;
	if (m_listOperationPlan.InsertColumn(5, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTOPARETIONPLAN_CREATE, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 6;
	lvc.pszText = szMsg;
	lvc.cx = m_createTypeWidth;
	if (m_listOperationPlan.InsertColumn(6, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_FORECASTOPERATIONPLAN_OPERATIONTYPE, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 7;
	lvc.pszText = szMsg;
	lvc.cx = m_operationTypeWidth;
	if (m_listOperationPlan.InsertColumn(7, &lvc) < 0)
		return FALSE;

	m_listOperationPlan.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	return TRUE;
}

/*============================================================================*/
/*! CForecastOperationPlan

-# ���X�g�R���g���[���ɃA�C�e���}��

@param	�Ȃ�
@retval	TRUE:�}������ / FALSE:�}�����s

*/
/*============================================================================*/
BOOL CForecastOperationPlan::InsertListItem()
{
	CForecastList* pWnd = (CForecastList*)GetParent();
	if (pWnd == NULL)
	{
		return FALSE;
	}

	CGraphMutex::Lock(eFile);
	vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();
//	// �v��ꗗ���R�s�[
//	m_listPlanList = listPlan;

	SetStationIndex();
	SetSatelliteName();

	m_listOperationPlan.DeleteAllItems();

	LVITEM lvi;
	CString str;
	int i, index;
//	TCHAR szMsg[256];
	CTime ctime;

	// �w�i�F���Z�b�g
	map<CString, COLORREF>	mapColor;
	int sz = sizeof(mStationSimpleString) / sizeof(mStationSimpleString[0]);
	for (int i = 0; i < sz; i++)
	{
//		mapColor.insert(map<CString, COLORREF>::value_type(mStationSimpleString[i], mStationColor[i]));
		mapColor.insert(map<CString, COLORREF>::value_type(mStationString[i], mStationColor[i]));
	}
	m_listOperationPlan.SetColorMap(mapColor);

	// ���X�g�R���g���[���ɃA�C�e���}��
	for (i = 0, index = 0; i < (int)listPlan.size(); i++)
	{
		// �ǖ���ALL�܂��͈�v�̂Ƃ��Ƀ��X�g�ɒǉ�
//		str.Format(mStationSimpleString[m_nStationIndex]);
//		str = pWnd->GetSimpleStationName();
//		if (!(m_strStationName.CompareNoCase(_T("All")) == 0 || str.Compare(listPlan[i].strStationName) == 0))
		if (!(m_strStationName.CompareNoCase(_T("All")) == 0 || m_strStationName.Compare(listPlan[i].strStationName) == 0))
			continue;

		// �q������ALL�܂��͈�v�̂Ƃ��Ƀ��X�g�ɒǉ�
		if (!(m_strSatelliteName.CompareNoCase(_T("All")) == 0 ||
			m_strSatelliteName.CompareNoCase(listPlan[i].strSatelliteName) == 0))
			continue;

		// ���X�g�R���g���[���ɓ��e�Z�b�g
		lvi.mask = LVIF_TEXT;

		lvi.iItem = i;
		lvi.iSubItem = 0;
		str.Format(listPlan[i].strStationName);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if ((index = m_listOperationPlan.InsertItem(&lvi)) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 1;
		str.Format(listPlan[i].strSatelliteName);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listOperationPlan.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 2;
//		str.Format(+_T("%07u"), listPlan[i].m_nPlanId);
		str.Format(listPlan[i].strPassId);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listOperationPlan.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 3;
//		ctime = listPlan[i].tPrePassStart;
		if (listPlan[i].lOfsFlag == 1)
		{
			ctime = listPlan[i].tPrePassStart + listPlan[i].tOffset;
		}
		else
		{
			ctime = listPlan[i].tPrePassStart;
		}
		str = ctime.Format("%Y-%m-%d %H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listOperationPlan.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 4;
//		ctime = listPlan[i].tPassStart;
		if (listPlan[i].lOfsFlag == 1)
		{
			ctime = listPlan[i].tPassStart + listPlan[i].tOffset;
		}
		else
		{
			ctime = listPlan[i].tPassStart;
		}
		str = ctime.Format("%Y-%m-%d %H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listOperationPlan.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 5;
//		ctime = listPlan[i].tPassEnd;
		if (listPlan[i].lOfsFlag == 1)
		{
			ctime = listPlan[i].tPassEnd + listPlan[i].tOffset;
		}
		else
		{
			ctime = listPlan[i].tPassEnd;
		}
		str = ctime.Format("%Y-%m-%d %H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listOperationPlan.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 6;
		str.Format(listPlan[i].strGetFrom);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listOperationPlan.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}

		lvi.iItem = index;
		lvi.iSubItem = 7;
		str.Format(listPlan[i].strPlanType);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listOperationPlan.SetItem(&lvi) < 0){
			CGraphMutex::Unlock(eFile);
			return FALSE;
		}
	}
	CGraphMutex::Unlock(eFile);

	for (int i = 0; i < m_listOperationPlan.GetItemCount(); i++) {
		m_listOperationPlan.SetItemData(i, i);
	}

//	UpdateData(FALSE);
//	UpdateWindow();

	return TRUE;
}

/*============================================================================*/
/*! CForecastOperationPlan

-# ���X�g�R���g���[���̃A�C�e�����X�V����

@param	�Ȃ�
@retval	TRUE:�X�V���� / FALSE:�X�V���s

*/
/*============================================================================*/
BOOL CForecastOperationPlan::UpdateListItem()
{
	int nItemCount = m_listOperationPlan.GetItemCount();
	CRect rect;
	m_listOperationPlan.GetItemRect(0, &rect, LVIR_BOUNDS);
	int index = m_listOperationPlan.GetTopIndex();
	int offset = rect.Height() * index;
	CSize cs;
	cs.cx = 0;
	cs.cy = offset;

	// �A�C�e���}��
	InsertListItem();

	// �A�C�e�������ς���Ă��Ȃ��ꍇ�́A�L�^�����X�N���[���ʒu�ɃZ�b�g����
	if (nItemCount == m_listOperationPlan.GetItemCount())
	{
		if (offset)
		{
			m_listOperationPlan.Scroll(cs);
		}
	}

	CString strStation, strSatellite, strPassId, strGetFrom, strPredPath;
	CGraphMutex::Lock(eFile);
	vector<stPlanListData>::iterator itr;
	vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();

	BOOL bFind = FALSE;

	nItemCount = m_listOperationPlan.GetItemCount();
	for (int i = 0; i < nItemCount && bFind == FALSE; i++)
	{
		strStation = m_listOperationPlan.GetItemText(i, 0);
		strSatellite = m_listOperationPlan.GetItemText(i, 1);
		strPassId = m_listOperationPlan.GetItemText(i, 2);
		strGetFrom = m_listOperationPlan.GetItemText(i, 6);

		if (strStation.CompareNoCase(m_strSelectedStation) == 0 &&
			strSatellite.CompareNoCase(m_strSelectedSatellite) == 0 &&
			strPassId.CompareNoCase(m_strSelectedPassId) == 0 &&
			strGetFrom.CompareNoCase(m_strSelectedGetFrom) == 0)
		{
			// ���X�g�R���g���[���ƑI������Ă������̂���v
			for (itr = listPlan.begin(); itr != listPlan.end() && bFind == FALSE; itr++)
			{
				if ((*itr).strStationName.Compare(m_strSelectedStation) == 0 &&
					(*itr).strSatelliteName.Compare(m_strSelectedSatellite) == 0 &&
					(*itr).strPassId.Compare(m_strSelectedPassId) == 0 &&
					(*itr).strGetFrom.Compare(m_strSelectedGetFrom) == 0)
				{
					// �v��ꗗ�t�@�C���̋ǁA�q���A�v��ID�A�쐬���A�\��l�t�@�C������v
					if ((*itr).strStationName.Compare(m_strSelectedStation) == 0 &&
						(*itr).strSatelliteName.Compare(m_strSelectedSatellite) == 0 &&
						(*itr).strPassId.Compare(m_strSelectedPassId) == 0 &&
						(*itr).strGetFrom.Compare(m_strSelectedGetFrom) == 0 && 
						(*itr).strPredName.CompareNoCase(m_strSelectedFile) == 0)
					{
						// ���X�g�R���g���[���I��
						if (m_listOperationPlan.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED) == 0)
						{
							// ���X�g�I���ł��Ȃ������ꍇ�́A�t�@�C���p�X���N���A
							m_strSelectedFile = _T("");
						}

						bFind = TRUE;
					}
				}
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	InitRadarGraphView();

	return TRUE;
}


/*============================================================================*/
/*! CForecastOperationPlan

-# ���X�g�R���g���[���\�[�g���̃R�[���o�b�N�֐�

@param	lParam1		��r���A�C�e��
@param	lParam2		��r��A�C�e��
@param	lParamSort	�E�B���h�E�n���h��
@retval 0: ��v, -1:��r���̏��Ԃ��Ⴂ, 1:��r���̏��Ԃ�����

*/
/*============================================================================*/
int CALLBACK CForecastOperationPlan::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CForecastOperationPlan* dlg = (CForecastOperationPlan*)lParamSort;
	return dlg->CompareFunc(lParam1, lParam2);
}

/*============================================================================*/
/*! CForecastOperationPlan

-# ���X�g�R���g���[���\�[�g���̔�r�֐�

@param	lParam1		��r���A�C�e��
@param	lParam2		��r��A�C�e��
@retval 0: ��v, -1:��r���̏��Ԃ��Ⴂ, 1:��r���̏��Ԃ�����

*/
/*============================================================================*/
int CForecastOperationPlan::CompareFunc(LPARAM lParam1, LPARAM lParam2)
{
	CString str[2];
	LPARAM param[2] = { lParam1, lParam2 };

	// ���X�g�R���g���[�������r����A�C�e���擾
	for (int i = 0; i < 2; ++i)
	{
		LVFINDINFO fi = { 0 };
		fi.flags = LVFI_PARAM;
		fi.lParam = param[i];
		str[i] = m_listOperationPlan.GetItemText(m_listOperationPlan.FindItem(&fi), m_nSelectedColumn);
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
/*! CForecastOperationPlan

-# �`�掞�ɌĂ΂�A�w��R���g���[���̔w�i��h��Ԃ��u���V�̃n���h����Ԃ�

@param	pDC�F�q�E�B���h�E�̃f�B�X�v���C�R���e�L�X�g�̃|�C���^
@param	pWnd�F�F���i�[���ꂽ�R���g���[���̃|�C���^
@param	nCtrlColor�F�R���g���[���̎��
@retval	�R���g���[���̔w�i��h��Ԃ��u���V�̃n���h��

*/
/*============================================================================*/
HBRUSH CForecastOperationPlan::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
/*! CForecastOperationPlan

-# �ǖ����ύX����Ă���ꍇ�ɋǖ����Z�b�g

@param	�Ȃ�
@retval TRUE: �ύX����Ă���B FALSE: �ύX����Ă��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastOperationPlan::SetStationName()
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
/*! CForecastOperationPlan

-# �ǃC���f�b�N�X���Z�b�g

@param	�Ȃ�
@retval TRUE: �擾�����B FALSE: �擾���s�B

*/
/*============================================================================*/
BOOL CForecastOperationPlan::SetStationIndex()
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
/*! CForecastOperationPlan

-# �q�������ύX����Ă���ꍇ�ɉq�������Z�b�g

@retval TRUE: �ύX����Ă���B FALSE: �ύX����Ă��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastOperationPlan::SetSatelliteName()
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

#if 0
/*============================================================================*/
/*! CForecastOperationPlan

-# �I������Ă���^�p�v����폜����

@retval TRUE: �폜�����B FALSE: �폜���Ȃ��B

*/
/*============================================================================*/
BOOL CForecastOperationPlan::DeleteListItem()
{
	POSITION pos;
	int nItem;
	vector<stPlanListData>::iterator itr;
	BOOL ret = FALSE;
	CString strSatelliteName, strPassId, strGetFrom;

	vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();

	pos = m_listOperationPlan.GetFirstSelectedItemPosition();
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

	while (TRUE)
	{
		pos = m_listOperationPlan.GetFirstSelectedItemPosition();
		if (!pos)
			break;

		nItem = m_listOperationPlan.GetNextSelectedItem(pos);
		strSatelliteName = m_listOperationPlan.GetItemText(nItem, 1);
		strPassId = m_listOperationPlan.GetItemText(nItem, 2);
		strGetFrom = m_listOperationPlan.GetItemText(nItem, 6);
		for (itr = listPlan.begin(); itr != listPlan.end(); itr++)
		{
			// ��ʂ̃R���g���[���ƃ��X�g�ŉq�����ƃp�XID�̈�v�𒲂ׂ�
			if ((*itr).strSatelliteName.Compare(strSatelliteName) == 0 &&
				(*itr).strPassId.Compare(strPassId) == 0 &&
				(*itr).strGetFrom.Compare(strGetFrom) == 0)
			{
				listPlan.erase(itr);
				m_listOperationPlan.DeleteItem(nItem);
				ret = TRUE;
				break;
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	// �Ō�ɍ폜�����ʒu��I�����Ă���
	int size = m_listOperationPlan.GetItemCount();
	if (size > 0)
	{
		if (nItem >= size)
		{
			nItem = size - 1;
		}
		if (nItem >= 0)
		{
			m_listOperationPlan.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
		}
	}

	UpdateData(FALSE);
	UpdateWindow();

	return ret;
}
#endif

/*============================================================================*/
/*! CForecastOperationPlan

-# ���X�g�R���g���[���̍��ڃN���b�N���ɌĂ΂��

@retval TRUE: �폜�����B FALSE: �폜���Ȃ��B

*/
/*============================================================================*/
void CForecastOperationPlan::OnHdnItemclickForecastoperationplanList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	if (phdr == NULL)
		return;

	m_nSelectedColumn = phdr->iItem;

	for (int i = 0; i < m_listOperationPlan.GetItemCount(); i++) {
		m_listOperationPlan.SetItemData(i, i);
	}
	m_listOperationPlan.SortItems(&CForecastOperationPlan::CompareFunc, (LPARAM)this);

	m_bUpperSort = !m_bUpperSort;

	*pResult = 0;
}


/*============================================================================*/
/*! CForecastValue

-# �I������Ă���\��l���X�g�̈�ԍŏ��̃t�@�C���p�X��Ԃ�

@param	strFilePath�F�t�@�C���p�X
@retval TRUE: �Ԃ��B FALSE: �Ԃ��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastOperationPlan::GetSelectedPredFilePath(CString& strFilePath, BOOL bChk /*= FALSE*/)
{
	POSITION pos;
	int nItem;
	CString strStationName, strSatelliteName, strPassId, strGetFrom;

	pos = m_listOperationPlan.GetFirstSelectedItemPosition();
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
	vector<stPlanListData>::iterator itr;
	vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();

	while (pos)
	{
		nItem = m_listOperationPlan.GetNextSelectedItem(pos);
		strStationName = m_listOperationPlan.GetItemText(nItem, 0);
		strSatelliteName = m_listOperationPlan.GetItemText(nItem, 1);
		strPassId = m_listOperationPlan.GetItemText(nItem, 2);
		strGetFrom = m_listOperationPlan.GetItemText(nItem, 6);
		for (itr = listPlan.begin(); itr != listPlan.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationName) == 0 &&
				(*itr).strSatelliteName.Compare(strSatelliteName) == 0 &&
				(*itr).strPassId.Compare(strPassId) == 0 &&
				(*itr).strGetFrom.Compare(strGetFrom) == 0)
			{
				strFilePath = ((*itr).strPredName);
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

-# �I������Ă���\��l���X�g�̈�ԍŏ��̌v��ID�A�^�p�J�n�����A�^�p�I��������Ԃ��B

@param	strPlanID�F�v��ID
@param	tPassStart�F�^�p�J�n����
@param	tPassEnd�F�^�p�I������
@retval TRUE: �Ԃ��B FALSE: �Ԃ��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastOperationPlan::GetSelectedPlanID(CString& strPlanID, __int64& tPassStart, __int64& tPassEnd, BOOL bChk /*= FALSE*/)
{
	POSITION pos, pos2;
	int nItem;
	CString strStationName, strSatelliteName, strPassIdTmp, strGetFrom;

	pos = m_listOperationPlan.GetFirstSelectedItemPosition();
	pos2 = pos;
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
	vector<stPlanListData>::iterator itr;
	vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();

	// ��ɋǗ��ėD��ŋǁA�q���A�v��ID����v�𒲂ׂ�
	BOOL bDiffGetFrom = FALSE;
	while (pos)
	{
		nItem = m_listOperationPlan.GetNextSelectedItem(pos);
		strStationName = m_listOperationPlan.GetItemText(nItem, 0);
		strSatelliteName = m_listOperationPlan.GetItemText(nItem, 1);
		strPassIdTmp = m_listOperationPlan.GetItemText(nItem, 2);
		strGetFrom = m_listOperationPlan.GetItemText(nItem, 6);
		for (itr = listPlan.begin(); itr != listPlan.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationName) == 0 &&
				(*itr).strSatelliteName.Compare(strSatelliteName) == 0 &&
				(*itr).strPassId.Compare(strPassIdTmp) == 0)
			{
				if ((*itr).strGetFrom.Compare(strGetFrom) == 0)
				{
					// �ǁA�q���A�v��ID�AENP/�Ǌǐ��̈�v
					if ((*itr).strGetFrom.CompareNoCase(_T("ENP")) != 0)
					{
						// �Ǌǐ��̏ꍇ
						strPlanID = ((*itr).strPassId);
						tPassStart = ((*itr).tPassStart);
						tPassEnd = ((*itr).tPassEnd);
						CGraphMutex::Unlock(eFile);
						return TRUE;
					}
				}
				else
				{
					// �ǁA�q���A�v��ID�����������AENP/�Ǘ��ĈႢ����������
					bDiffGetFrom = TRUE;
				}
			}
		}
	}

	// �Ǘ��Č������ENP�ŋǁA�q���A�v��ID����v�𒲂ׂ�
	while (pos2)
	{
		nItem = m_listOperationPlan.GetNextSelectedItem(pos2);
		strStationName = m_listOperationPlan.GetItemText(nItem, 0);
		strSatelliteName = m_listOperationPlan.GetItemText(nItem, 1);
		strPassIdTmp = m_listOperationPlan.GetItemText(nItem, 2);
		strGetFrom = m_listOperationPlan.GetItemText(nItem, 6);
		for (itr = listPlan.begin(); itr != listPlan.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationName) == 0 &&
				(*itr).strSatelliteName.Compare(strSatelliteName) == 0 &&
				(*itr).strPassId.Compare(strPassIdTmp) == 0 &&
				(*itr).strGetFrom.Compare(strGetFrom) == 0)
			{
				if ((*itr).strGetFrom.CompareNoCase(_T("ENP")) == 0 && bDiffGetFrom == FALSE)
				{
					// ENP�ł��Ǌǐ��ɓ����v��ID���Ȃ������ꍇ
					strPlanID = ((*itr).strPassId);
					tPassStart = ((*itr).tPassStart);
					tPassEnd = ((*itr).tPassEnd);
					CGraphMutex::Unlock(eFile);
					return TRUE;
				}
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	return FALSE;
}


/*============================================================================*/
/*! CForecastValue

-# �I������Ă���v�惊�X�g�̈�ԍŏ��̋ǖ��A�q�����A�\��l�t�@�C���p�X��Ԃ�

@param	strStationName�F�ǖ�
@param	strSatelliteName�F�q����
@param	strPassId�F�v��ID
@param	strGetFrom�F�쐬��
@param	strFilePath�F�\��l�t�@�C���p�X
@retval TRUE: �Ԃ��B FALSE: �Ԃ��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastOperationPlan::GetSelectedStationNameAndPredFilePath(CString& strStationName, CString& strSatelliteName, CString& strPassId, 
	CString& strGetFrom, CString& strFilePath, BOOL bChk /*= FALSE*/)
{
	POSITION pos;
	int nItem;
	CString strStationNameCmp, strSatelliteNameCmp, strPassIdCmp, strGetFromCmp;

	pos = m_listOperationPlan.GetFirstSelectedItemPosition();
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
	vector<stPlanListData>::iterator itr;
	vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();

	while (pos)
	{
		nItem = m_listOperationPlan.GetNextSelectedItem(pos);
		strStationNameCmp = m_listOperationPlan.GetItemText(nItem, 0);
		strSatelliteNameCmp = m_listOperationPlan.GetItemText(nItem, 1);
		strPassIdCmp = m_listOperationPlan.GetItemText(nItem, 2);
		strGetFromCmp = m_listOperationPlan.GetItemText(nItem, 6);
		for (itr = listPlan.begin(); itr != listPlan.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationNameCmp) == 0 &&
				(*itr).strSatelliteName.Compare(strSatelliteNameCmp) == 0 &&
				(*itr).strPassId.Compare(strPassIdCmp) == 0 &&
				(*itr).strGetFrom.Compare(strGetFromCmp) == 0)
			{
				strStationName = ((*itr).strStationName);
				strSatelliteName = ((*itr).strSatelliteName);
				strPassId = ((*itr).strPassId);
				strGetFrom = ((*itr).strGetFrom);
				strFilePath = ((*itr).strPredName);
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

-# �I������Ă����ԍŏ��̋ǖ��A�q�����A�v��t�@�C�����A�擾���Ԃ�

@param	strStationName�F�ǖ�
@param	strSatelliteName�F�q����
@param	strFilePath�F�p�XID
@param	strGetFrom�F�擾��
@retval TRUE: �Ԃ��B FALSE: �Ԃ��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastOperationPlan::GetSelectedStationNameAndPlanFilePath(CString& strStationName, CString& strSatelliteName, CString& strFilePath, CString& strGetFrom, BOOL bChk /*= FALSE*/)
{
	POSITION pos;
	int nItem;
	CString strPassId;
	CString strSatelliteNameCmp;
	CString strStationNameCmp;
	CString strGetFromTmp;

	pos = m_listOperationPlan.GetFirstSelectedItemPosition();
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
	vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();
	vector<stPlanListData>::iterator itr;

	while (pos)
	{
		nItem = m_listOperationPlan.GetNextSelectedItem(pos);
		strStationNameCmp = m_listOperationPlan.GetItemText(nItem, 0);
		strSatelliteNameCmp = m_listOperationPlan.GetItemText(nItem, 1);
		strPassId = m_listOperationPlan.GetItemText(nItem, 2);
		strGetFromTmp = m_listOperationPlan.GetItemText(nItem, 6);
		for (itr = listPlan.begin(); itr != listPlan.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationNameCmp) == 0 &&
				(*itr).strSatelliteName.Compare(strSatelliteNameCmp) == 0 &&
				(*itr).strPassId.Compare(strPassId) == 0 &&
				(*itr).strGetFrom.Compare(strGetFromTmp) == 0)
			{
				strSatelliteName = ((*itr).strSatelliteName);
				strStationName = ((*itr).strStationName);
				// �v��t�@�C���p�X���쐬
				strFilePath.Format(_T("%s.%s"), (LPCTSTR)strPassId, PLAN_FILE_TYPE);
				strGetFrom = ((*itr).strGetFrom);
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

-# �I������Ă����ԍŏ��̋ǖ��A�q�����A�v��t�@�C�����A�擾���Ԃ�

@param	strStationName�F�ǖ�
@param	strSatelliteName�F�q����
@param	strFilePath�F�p�XID
@param	strGetFrom�F�擾��
@retval TRUE: �Ԃ��B FALSE: �Ԃ��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastOperationPlan::GetSelectedStationNameAndPlanFilePath(vector<stPlanListData>& listPlan, BOOL bChk /*= FALSE*/)
{
	POSITION pos;
	int nItem;
	CString strPassId;
	CString strSatelliteNameCmp;
	CString strStationNameCmp;
	CString strGetFromTmp;

	BOOL bRet = FALSE;

	pos = m_listOperationPlan.GetFirstSelectedItemPosition();
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
	vector<stPlanListData>& srcListPlan = theApp.GetSatelliteData().GetPlanList();
	vector<stPlanListData>::iterator itr;

	while (pos)
	{
		nItem = m_listOperationPlan.GetNextSelectedItem(pos);
		strStationNameCmp = m_listOperationPlan.GetItemText(nItem, 0);
		strSatelliteNameCmp = m_listOperationPlan.GetItemText(nItem, 1);
		strPassId = m_listOperationPlan.GetItemText(nItem, 2);
		strGetFromTmp = m_listOperationPlan.GetItemText(nItem, 6);
		for (itr = srcListPlan.begin(); itr != srcListPlan.end(); itr++)
		{
			if ((*itr).strStationName.Compare(strStationNameCmp) == 0 &&
				(*itr).strSatelliteName.Compare(strSatelliteNameCmp) == 0 &&
				(*itr).strPassId.Compare(strPassId) == 0 &&
				(*itr).strGetFrom.Compare(strGetFromTmp) == 0)
			{
				listPlan.push_back((*itr));
				// �v��t�@�C���p�X���쐬
				listPlan[listPlan.size() - 1].strPlanName.Format(_T("%s.%s"), (LPCTSTR)strPassId, PLAN_FILE_TYPE);
				bRet = TRUE;
			}
		}
	}
	CGraphMutex::Unlock(eFile);

	return bRet;
}


/*============================================================================*/
/*! CForecastValue

-# �ǖ��A�q�����A�\��l�p�XID���v��Ŏg���Ă��邩���ׂ�

@param	strStationName�F�ǖ�
@param	strSatelliteName�F�q����
@param	strPredPassId�F�\��l�p�XID
@retval TRUE: �g���Ă���B FALSE: �g���Ă��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastOperationPlan::IsUsedPlan(CString& strStationName, CString& strSatelliteName, CString& strPredPassId)
{
	CGraphMutex::Lock(eFile);
	vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();
	vector<stPlanListData>::iterator itr;

	CString strPredId;
	int idx = strPredPassId.Find(_T("-"));
	strPredId.Format(_T("%s%s"), (LPCTSTR)strPredPassId.Mid(0, idx), (LPCTSTR)strPredPassId.Mid(idx + 1, 4));

	for (itr = listPlan.begin(); itr != listPlan.end(); itr++)
	{
		if ((*itr).strStationName.Compare(strStationName) == 0 &&
			(*itr).strSatelliteName.Compare(strSatelliteName) == 0 &&
			(*itr).strPredName.IsEmpty() == false && 
			(*itr).strPredName.Find(strPredId) >= 0)
		{
			CGraphMutex::Unlock(eFile);
			return TRUE;
		}
	}
	CGraphMutex::Unlock(eFile);

	return FALSE;
}


/*============================================================================*/
/*! CForecastOperationPlan

-# �ǔ��O���t������������B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CForecastOperationPlan::InitRadarGraphView()
{
	// �I������Ă����ԏ�̌v��t�@�C���p�X���擾
	CString strStationName, strSatelliteName, strPassId, strGetFrom, strFilePath;
	if (GetSelectedStationNameAndPredFilePath(strStationName, strSatelliteName, strPassId, strGetFrom, strFilePath))
	{
		// �I������Ă���ꍇ�A�V�����t�@�C���p�X�擾
		m_strSelectedStation = strStationName;
		m_strSelectedSatellite = strSatelliteName;
		m_strSelectedPassId = strPassId;
		m_strSelectedGetFrom = strGetFrom;
		m_strSelectedFile = strFilePath;
	}
	else
	{
		// �I������Ă��Ȃ��ꍇ
		m_strSelectedStation = _T("");
		m_strSelectedSatellite = _T("");
		m_strSelectedPassId = _T("");
		m_strSelectedGetFrom = _T("");
		m_strSelectedFile = _T("");
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

	CString strPredId = strFilePath;
	CString strFullPath;
	if (strFilePath.IsEmpty() || strSatelliteName.IsEmpty() || nStation < 0)
	{
		// ���I���������̓t�@�C���������ꍇ
		nStation = -1;
		strFullPath = _T("");
		strPredId = _T("");
	}
	else
	{
		// �\��l�t�@�C���̃t���p�X�����
		strFullPath = theApp.GetShareFilePath(eFileType_Pred_Folder, nStation) + strSatelliteName + _T("\\") + strFilePath;
		// �\��l�t�@�C�����̊g���q���폜�A'-'��}�����A�\��lID�����
		AfxExtractSubString(strPredId, strFilePath, 0, '.');
		strPredId.Insert(6, '-');
	}

	// �\��l�̒ǔ��O���t�ɂ���
	pView->SetViewtype(eRadarViewType_OperationPlan);
	pView->SetStationID(nStation);

	// �v��ID�A�^�p�J�n�����A�^�p�I�������Z�b�g
	CString strPlanID;
	__int64 tPassStart, tPassEnd;
	if (GetSelectedPlanID(strPlanID, tPassStart, tPassEnd))
	{
		pView->SetPlanID(strPlanID, tPassStart, tPassEnd);	// �v�悪�݂�����
	}
	else
	{
		pView->SetPlanID(_T(""), 0, 0);		// �v�悪�݂���Ȃ�����
	}

	// �ǔ��O���t������������
	pView->Init(strFullPath, strSatelliteName, strPredId);
}


/*============================================================================*/
/*! CForecastOperationPlan

-# �v��ꗗ����ǖ����X�g���擾����

@param	listName�F�ǖ����X�g
@retval TRUE: �Ԃ��B FALSE: �Ԃ��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastOperationPlan::GetStationNameList(vector<CString>& listName)
{
	CGraphMutex::Lock(eFile);
	vector<stPredListData>::iterator itr;
	vector<stPredListData>& listPred = theApp.GetSatelliteData().GetPredList();
	vector<CString>::iterator itrName;

	listName.clear();

	for (itr = listPred.begin(); itr != listPred.end(); itr++)
	{
		// �ǖ������ɋǖ����X�g�ɂ��邩���ׂ�
		itrName = find(listName.begin(), listName.end(), (*itr).strStationName);
		if (itrName == listName.end())
		{
			// �����ꍇ�͋ǖ����X�g�ɒǉ�
			listName.push_back((*itr).strStationName);
		}
	}
	CGraphMutex::Unlock(eFile);

	if (listName.empty() == TRUE)
		return FALSE;

	return TRUE;
}


/*============================================================================*/
/*! CForecastOperationPlan

-# �v��ꗗ����q�������X�g���擾����

@param	listName�F�q�������X�g
@retval TRUE: �Ԃ��B FALSE: �Ԃ��Ȃ��B

*/
/*============================================================================*/
BOOL CForecastOperationPlan::GetSatelliteNameList(vector<CString>& listName)
{
	CGraphMutex::Lock(eFile);
	vector<stPlanListData>::iterator itr;
	vector<stPlanListData>& listPred = theApp.GetSatelliteData().GetPlanList();
	vector<CString>::iterator itrName;

	listName.clear();

	for (itr = listPred.begin(); itr != listPred.end(); itr++)
	{
		// �q���������ɉq�������X�g�ɂ��邩���ׂ�
		itrName = find(listName.begin(), listName.end(), (*itr).strSatelliteName);
		if (itrName == listName.end())
		{
			// �����ꍇ�͋ǖ����X�g�ɒǉ�
			listName.push_back((*itr).strSatelliteName);
		}
	}
	CGraphMutex::Unlock(eFile);

	if (listName.empty() == TRUE)
		return FALSE;

	return TRUE;
}


/*============================================================================*/
/*! CForecastOperationPlan

-# ���X�g�R���g���[�����N���b�N���ɌĂ΂��B

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�

*/
/*============================================================================*/
void CForecastOperationPlan::OnNMClickForecastoperationplanList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	InitRadarGraphView();
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
//	*pResult = 0;
}

