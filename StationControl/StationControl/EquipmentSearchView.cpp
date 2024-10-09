// EquipmentSearchView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "EquipmentSearchView.h"


// CEquipmentSearchView

IMPLEMENT_DYNCREATE(CEquipmentSearchView, CHistroyView)

CEquipmentSearchView::CEquipmentSearchView()
{
	m_nHistoryType = eHistory_SearchEquipment;
}

CEquipmentSearchView::~CEquipmentSearchView()
{
}

BEGIN_MESSAGE_MAP(CEquipmentSearchView, CHistroyView)
END_MESSAGE_MAP()


// CEquipmentSearchView 描画

void CEquipmentSearchView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 描画コードをここに追加してください。
}


// CEquipmentSearchView 診断

#ifdef _DEBUG
void CEquipmentSearchView::AssertValid() const
{
	CHistroyView::AssertValid();
}

#ifndef _WIN32_WCE
void CEquipmentSearchView::Dump(CDumpContext& dc) const
{
	CHistroyView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEquipmentSearchView メッセージ ハンドラー

void CEquipmentSearchView::OnInitialUpdate()
{
	m_nControlID = eEquipmentSearch;
	m_nTabItem = eTabItem_Search;

	CHistroyView::OnInitialUpdate();

	GetDocument()->SetTitleEx();
}

/*============================================================================*/
/*! 運用履歴

-# 履歴情報の追加

@param
@retval

*/
/*============================================================================*/
void CEquipmentSearchView::AddHistroy()
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
/*! 運用履歴

-# 履歴情報の追加

@param	data		履歴情報
@param	bDataAdd	リスト用データへの追加
@retval

*/
/*============================================================================*/
void CEquipmentSearchView::AddHistroy(stHistoryData& data, bool bDataAdd/*=true*/)
{
	mSection.Lock();

	if (m_History.GetItemCount() == MAX_HISTORYSEARCH_DATA)
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
		case eFilterItemEq_Time:
			continue;
		case eFilterItemEq_Station:
			param = data.station;
			break;
		case eFilterItemEq_Equipment:
			param = data.equipment;
			break;
		case eFilterItemEq_ImStatus:
			param = data.status;
			break;
		case eFilterItemEq_Status:
			param = data.status;
			break;
		case eFilterItemEq_MessageId:
			param = data.message;
			break;
		case eFilterItemEq_Message:
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
			case eFilterItemEq_Time:
				str.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"), data.year, data.month, data.day, data.hour, data.minute, data.second);
				break;
			case eFilterItemEq_Station:
				str.Format(_T("%s"), (LPCTSTR)mStationString[data.station]);
				break;
			case eFilterItemEq_Equipment:
				str.Format(_T("%s"), (LPCTSTR)mHistEquipmentString[data.equipment]);
				break;
			case eFilterItemEq_ImStatus:
				str.Format(_T("%s"), (LPCTSTR)mHistImStatusString[data.imstatus]);
				break;
			case eFilterItemEq_Status:
				str.Format(_T("%s"), (LPCTSTR)mHistStatusString[data.status]);
				break;
			case eFilterItemEq_MessageId:
				str.Format(_T("%6d"), data.message);
				break;
			case eFilterItemEq_Message:
				str.Format(_T("%s"), (LPCTSTR)data.msgstring);
				break;
			default:
				continue;
			}
			AddItem(item, subitem, str, data.imstatus | 0x80000000);
		}
	}
	m_History.SetRedraw(true);
	mSection.Unlock();
}

void CEquipmentSearchView::OnSize(UINT nType, int cx, int cy)
{
	CHistroyView::OnSize(nType, cx, cy);
}


LRESULT CEquipmentSearchView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
