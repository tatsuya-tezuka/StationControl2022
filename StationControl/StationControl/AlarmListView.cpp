// AlarmListView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "AlarmListView.h"


// CAlarmListView
CCriticalSection	CAlarmListView::mSection;

IMPLEMENT_DYNCREATE(CAlarmListView, CView)

CAlarmListView::CAlarmListView()
{

}

CAlarmListView::~CAlarmListView()
{
}

CAlarmListView*	CAlarmListView::m_pInstance = NULL;
CAlarmListView*	CAlarmListView::Instance()
{
	if (m_pInstance == NULL){
		static CAlarmListView mInitializerInstance;
		m_pInstance = &mInitializerInstance;
	}
	return m_pInstance;
}

BEGIN_MESSAGE_MAP(CAlarmListView, CView)
END_MESSAGE_MAP()


// CAlarmListView �`��

void CAlarmListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
}


// CAlarmListView �f�f

#ifdef _DEBUG
void CAlarmListView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CAlarmListView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
CStationControlDoc* CAlarmListView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStationControlDoc)));
	return (CStationControlDoc*)m_pDocument;
}
#endif //_DEBUG


//============================================================================
/// @brief	���b�Z�[�W�R�[���o�b�N�֐�
/// @param	param	�t�B���^�f�[�^
/// @return
//============================================================================
void CALLBACK CAlarmListView::messageProc(stFilterData& param, UINT type)
{
	switch (type){
	case	eHistory_Alarm:
		CAlarmListView::Instance()->SetDisplayFilter(param);
		break;
	}
}

// CAlarmListView ���b�Z�[�W �n���h���[

void CAlarmListView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_pInstance = this;

	// �������X�g�̍쐬
	CreateAlarmList();

	// �^�u�^�o�[�̍쐬
	CRect rectClient;
	GetClientRect(rectClient);
	m_TabBar.Create(CMFCTabCtrl::STYLE_3D, rectClient, this, eAlarmList+1, CMFCTabCtrl::LOCATION_TOP);
	m_AlarmFilterDlg.SetOneStation(true);
	m_AlarmFilterDlg.SetHistoryType(eHistory_Alarm);
	m_AlarmFilterDlg.SetCallback(messageProc, eHistory_Alarm);
	m_AlarmFilterDlg.Create(IDD_DIALOG_FILTER_AL, &m_TabBar);
	m_AlarmFilterDlg.SetCallback(messageProc, eHistory_Alarm);
	m_TabBar.AddBar(&m_AlarmFilterDlg, _T("���A���^�C��"));
}

void CAlarmListView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_History.GetSafeHwnd()){
		m_History.SetWindowPos(NULL, -1, CMFCTabCtrlEx::mBarHeight, cx + 3, cy - CMFCTabCtrlEx::mBarHeight, /*SWP_NOMOVE | */SWP_NOACTIVATE | SWP_NOZORDER);
	}
	if (m_TabBar.GetSafeHwnd()){
		m_TabBar.SetWindowPos(NULL, -1, -1, cx, CMFCTabCtrlEx::mBarHeight + 10, /*SWP_NOMOVE | */SWP_NOACTIVATE | SWP_NOZORDER);
		m_TabBar.Resize(cx);
	}
}

/*============================================================================*/
/*! �ݔ��A���[���ꗗ

-# �������X�g�̍쐬

@param		rectClient		�N���C�A���g�̈�
@retval

*/
/*============================================================================*/
void CAlarmListView::CreateAlarmList()
{
	// �N���C�A���g�̈�̎擾
	CRect rectClient;
	GetClientRect(rectClient);

	m_History.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_SHOWSELALWAYS, rectClient, this, eAlarmList);
	// �g���X�^�C���̎擾
	DWORD  dwStyle = ListView_GetExtendedListViewStyle(m_History.m_hWnd);
	//  �g���X�^�C���̃Z�b�g
	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= LVS_EX_FULLROWSELECT/*|LVS_EX_GRIDLINES*/ | 0x00010000/*LVS_EX_DOUBLEBUFFER*/;
	//  �V�����g���X�^�C����ݒ�
	ListView_SetExtendedListViewStyle(m_History.m_hWnd, dwStyle);
	m_History.SetBkColor(RGB(0, 0, 0));
	m_History.InsertColumn(eFilterItemEq_Station, _T("���V�Y�U�S����"), LVCFMT_CENTER, 100);
	m_History.InsertColumn(eFilterItemEq_Time, _T("9999/99/99 99:99:99  "), LVCFMT_CENTER, 100);
	m_History.InsertColumn(eFilterItemEq_Equipment, _T("�ݔ����䑕�u�X�X"), LVCFMT_CENTER, 100);
	m_History.InsertColumn(eFilterItemEq_ImStatus, _T("�����@"), LVCFMT_CENTER, 100);
	m_History.InsertColumn(eFilterItemEq_Status, _T("POWER ON/OFF�@"), LVCFMT_CENTER, 100);
	m_History.InsertColumn(eFilterItemEq_MessageId, _T("�X�X�X�X�X�X"), LVCFMT_LEFT, 100);
	m_History.InsertColumn(eFilterItemEq_Message, _T("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"), LVCFMT_CENTER, 100);
	AutoSizeColumns();
}
/*============================================================================*/
/*! �ݔ��A���[���ꗗ

-# �J�����̎����T�C�Y����

@param	col		�J�����ʒu�̃J�����T�C�Y�������ɒ������܂�
@retval

*/
/*============================================================================*/
void CAlarmListView::AutoSizeColumns(int col /*=-1*/)
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)m_History.GetDlgItem(0);

	// create dummy
	int dummycol = pHeader->GetItemCount();
	m_History.InsertColumn(dummycol, _T("dummy"), LVCFMT_LEFT, 10);

	m_History.SetRedraw(false);
	int mincol = col < 0 ? 0 : col;
	int maxcol = col < 0 ? pHeader->GetItemCount() - 1 : col;
	for (col = mincol; col <= maxcol; col++){
		m_History.SetColumnWidth(col, LVSCW_AUTOSIZE);
		int wc1 = m_History.GetColumnWidth(col);
		m_History.SetColumnWidth(col, LVSCW_AUTOSIZE_USEHEADER);
		int wc2 = m_History.GetColumnWidth(col);
		int wc = MAX(MINCOLWIDTH, MAX(wc1, wc2));
		m_History.SetColumnWidth(col, wc);
	}
	// delete dummy
	m_History.DeleteColumn(dummycol);
	m_History.SetRedraw(true);
}

/*============================================================================*/
/*! �ݔ��A���[���ꗗ

-# �������ڂ̓o�^

@param	nItem		�A�C�e���ԍ�
@param	nSubItem	�T�u�A�C�e���ԍ�
@param	strItem		�ݒ蕶����
@param	dwParam		���[�U�l
@retval

*/
/*============================================================================*/
int CAlarmListView::AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam/*=0*/)
{
	LV_ITEM lv;
	memset(&lv, 0, sizeof(lv));
	lv.mask = LVIF_TEXT;
	lv.iItem = nItem;
	lv.iSubItem = nSubItem;
	if (nSubItem == 0){
		lv.mask |= LVIF_PARAM;
		lv.lParam = dwParam;
	}
	lv.pszText = (LPTSTR)(LPCTSTR)strItem;
	lv.cchTextMax = strItem.GetLength();

	if (nSubItem == 0)
		return m_History.InsertItem(&lv);
	return m_History.SetItem(&lv);
}

/*============================================================================*/
/*! �ݔ��A���[���ꗗ

-# �����t�B���^�̐ݒ�

@param	data		�������
@retval

*/
/*============================================================================*/
void CAlarmListView::SetDisplayFilter(stFilterData& data)
{
	memcpy(&mFilterData, &data, sizeof(stFilterData));
	CloseChildWindow(NULL);

	CGraphMutex::Lock(eData);
	mSection.Lock();
	m_History.SetRedraw(false);

	m_History.DeleteAllItems();

	deque<stHistoryData>::iterator itr;
	for (itr = mHistoryData.begin(); itr != mHistoryData.end(); itr++){
		// �t�B���^�\���Ȃ̂ŁA���ݑ��݂���f�[�^����\������̂ŁA��Q������false
		AddHistroy((*itr), false);
	}

	m_History.SetRedraw(true);
	mSection.Unlock();
	CGraphMutex::Unlock(eData);
}

bool CAlarmListView::_GetStation(stHistoryData& hdata, CString cbuf)
{
	CString str, temp;
	AfxExtractSubString(str, cbuf, eAlarmFileType_Station, ',');
	for (int i = 0; i < sizeof(mStationString) / sizeof(mStationString[0]); i++){
		if (str == mStationString[i]){
			hdata.station = (BYTE)i;
			return true;
		}
	}
	return false;
}

bool CAlarmListView::_GetEquipment(stHistoryData& hdata, CString cbuf)
{
	CString str, temp;
	AfxExtractSubString(str, cbuf, eAlarmFileType_Equipment, ',');
	for (int i = 0; i < sizeof(mHistEquipmentString) / sizeof(mHistEquipmentString[0]); i++){
		if (str == mHistEquipmentString[i]){
			hdata.equipment = (BYTE)i;
			return true;
		}
	}
	return false;
}

bool CAlarmListView::_GetImStatus(stHistoryData& hdata, CString cbuf)
{
	CString str, temp;
	AfxExtractSubString(str, cbuf, eAlarmFileType_ImStatus, ',');
	for (int i = 0; i < sizeof(mAlarmImStatusString) / sizeof(mAlarmImStatusString[0]); i++){
		if (str == mAlarmImStatusString[i]){
			hdata.imstatus = (BYTE)i;
			return true;
		}
	}
	return false;
}

bool CAlarmListView::_GetStatus(stHistoryData& hdata, CString cbuf)
{
	CString str, temp;
	AfxExtractSubString(str, cbuf, eAlarmFileType_Status, ',');
	for (int i = 0; i < sizeof(mAlarmStatusString) / sizeof(mAlarmStatusString[0]); i++){
		if (str == mAlarmStatusString[i]){
			hdata.status = (BYTE)i;
			return true;
		}
	}
	return false;
}

/*============================================================================*/
/*! �ݔ��A���[���ꗗ

-# �L�������̃`�F�b�N

@param	nFilterType		�^�C�v
@param	val				����
@retval

*/
/*============================================================================*/
bool CAlarmListView::IsDisplay(int nFilterType, DWORD val, UINT nStation)
{
	switch (nFilterType){
	case eFilterItemEq_Station:
		if (mFilterData.station & (1 << val))
			return true;
		break;
	case eFilterItemEq_Equipment:
		if (mFilterData.sender[nStation] == 0)
			return true;
		if (mFilterData.sender[nStation] & (1 << val))
			return true;
		break;
	case eFilterItemEq_ImStatus:
		if (mFilterData.imstatus & (1 << val))
			return true;
		break;
	case eFilterItemEq_Status:
		if (mFilterData.status & (1 << val))
			return true;
		break;
	default:
		break;
	}
	return false;
}
