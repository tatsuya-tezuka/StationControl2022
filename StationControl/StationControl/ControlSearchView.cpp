// ControlSearchView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "ControlSearchView.h"


// CControlSearchView

IMPLEMENT_DYNCREATE(CControlSearchView, CHistroyView)

CControlSearchView::CControlSearchView()
{
	m_nHistoryType = eHistory_SearchControl;
}

CControlSearchView::~CControlSearchView()
{
}

BEGIN_MESSAGE_MAP(CControlSearchView, CHistroyView)
END_MESSAGE_MAP()


// CControlSearchView �`��

void CControlSearchView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
}


// CControlSearchView �f�f

#ifdef _DEBUG
void CControlSearchView::AssertValid() const
{
	CHistroyView::AssertValid();
}

#ifndef _WIN32_WCE
void CControlSearchView::Dump(CDumpContext& dc) const
{
	CHistroyView::Dump(dc);
}
#endif
#endif //_DEBUG


// CControlSearchView ���b�Z�[�W �n���h���[

void CControlSearchView::OnInitialUpdate()
{
	m_nControlID = eOperationSearch;
	m_nTabItem = eTabItem_Search;

	CHistroyView::OnInitialUpdate();

	GetDocument()->SetTitleEx();
}

/*============================================================================*/
/*! �^�p����

-# �������̒ǉ�

@param
@retval

*/
/*============================================================================*/
void CControlSearchView::AddHistroy()
{
	CGraphMutex::Lock(eData);

	deque<stHistoryData>& data = theApp.GetDataCollection().GetOperationHistoryData();
	while (!data.empty())
	{
		stHistoryData newData = data.front();
		data.pop_front();
		AddHistroy(newData);
	}

	CGraphMutex::Unlock(eData);
}

/*============================================================================*/
/*! �^�p����

-# �������̒ǉ�

@param	data		�������
@param	bDataAdd	���X�g�p�f�[�^�ւ̒ǉ�
@retval

*/
/*============================================================================*/
void CControlSearchView::AddHistroy(stHistoryData& data, bool bDataAdd/*=true*/)
{
	mSection.Lock();

	if(m_History.GetItemCount() == MAX_HISTORYSEARCH_DATA)
		m_History.DeleteItem(0);
	if (bDataAdd == true){
		if (mHistoryData.size() == MAX_HISTORYSEARCH_DATA){
			mHistoryData.pop_front();
		}
		mHistoryData.push_back(data);
	}

	int item, subitem;
	item = m_History.GetItemCount();

	CHeaderCtrl* pHeader = (CHeaderCtrl*)m_History.GetDlgItem(0);
	m_History.SetRedraw(false);
	int maxcol = pHeader->GetItemCount();

	bool bSet = true;
	DWORD param;
	for (int col = 0; col <= maxcol; col++){
		CString str;
		switch (col){
		case eFilterItem_Time:
			continue;
		case eFilterItem_Station:
			param = data.station;
			break;
		case eFilterItem_Equipment:
			param = data.equipment;
			break;
		case eFilterItem_Status:
			param = data.status;
			break;
		case eFilterItem_MessageId:
			param = data.message;
			break;
		case eFilterItem_Message:
			continue;
		default:
			continue;
		}
		bSet = (bSet) && IsDisplay(col, param, data.station);
	}

	if (bSet == true){
		for (int col = 0; col <= maxcol; col++){
			subitem = col;
			CString str;
			switch (col){
			case eFilterItem_Time:
				str.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"), data.year, data.month, data.day, data.hour, data.minute, data.second);
				break;
			case eFilterItem_Station:
				str.Format(_T("%s"), (LPCTSTR)mStationString[data.station]);
				break;
			case eFilterItem_Equipment:
				str.Format(_T("%s"), (LPCTSTR)mHistEquipmentString[data.equipment]);
				break;
			case eFilterItem_Status:
				str.Format(_T("%s"), (LPCTSTR)mHistStatusString[data.status]);
				break;
			case eFilterItem_MessageId:
				str.Format(_T("%6d"), data.message);
				break;
			case eFilterItem_Message:
				str.Format(_T("%s"), (LPCTSTR)data.msgstring);
				break;
			default:
				continue;
			}
			AddItem(item, subitem, str, data.status);
		}
	}
	m_History.SetRedraw(true);
	mSection.Unlock();
}

void CControlSearchView::OnSize(UINT nType, int cx, int cy)
{
	CHistroyView::OnSize(nType, cx, cy);
}


LRESULT CControlSearchView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case eMessage_ThreadNormal:
		AddHistroy();
		break;

	default:
		return CHistroyView::WindowProc(message, wParam, lParam);
	}
	return TRUE;
}
