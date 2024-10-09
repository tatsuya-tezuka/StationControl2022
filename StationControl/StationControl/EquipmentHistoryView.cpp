/*============================================================================*/
/*! 制御履歴

-#

@param
@retval

*/
/*============================================================================*/
// EquipmentHistoryView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "EquipmentHistoryView.h"


// CEquipmentHistoryView

IMPLEMENT_DYNCREATE(CEquipmentHistoryView, CHistroyView)

CEquipmentHistoryView::CEquipmentHistoryView()
{
	m_nHistoryType = eHistory_Equipment;
}

CEquipmentHistoryView::~CEquipmentHistoryView()
{
}

BEGIN_MESSAGE_MAP(CEquipmentHistoryView, CHistroyView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CEquipmentHistoryView 描画

void CEquipmentHistoryView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 描画コードをここに追加してください。
}


// CEquipmentHistoryView 診断

#ifdef _DEBUG
void CEquipmentHistoryView::AssertValid() const
{
	CHistroyView::AssertValid();
}

#ifndef _WIN32_WCE
void CEquipmentHistoryView::Dump(CDumpContext& dc) const
{
	CHistroyView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEquipmentHistoryView メッセージ ハンドラー

void CEquipmentHistoryView::OnInitialUpdate()
{
	m_nControlID = eEquipmentHistory;
	m_nTabItem = eTabItem_Filter;

	CHistroyView::OnInitialUpdate();

	GetDocument()->SetTitleEx();
	m_History.SetParent(this);
}

/*============================================================================*/
/*! 運用履歴

-# 履歴情報の追加

@param
@retval

*/
/*============================================================================*/
void CEquipmentHistoryView::AddHistroy()
{
	CGraphMutex::Lock(eData);

	deque<stHistoryData>& data = theApp.GetDataCollection().GetEquipmentHistoryData();
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
void CEquipmentHistoryView::AddHistroy(stHistoryData& data, bool bDataAdd/* = true*/)
{
	if (theApp.GetSelectStationMode() == eStation_1 && theApp.GetSelectStation() != data.station){
		return;
	}

	mSection.Lock();

	if (bDataAdd == true){
		bool ret = CHistroyView::AddHistroyEx(data);
		if (ret == false){
			// リスト最大数＆スクロール停止中なので、データ保留を行う
			mSection.Unlock();
			return;
		}
	}

	// ここにくるのはデータ保留は開始されていない状態
	if (m_History.GetItemCount() == MAX_HISTORY_DATA){
		// ここでの最大数チェックはスクロール解除中しかこない
		if (m_History.GetSelectedCount() == 0){
			m_History.DeleteItem(0);
		}
	}

	if (bDataAdd == true){
		if (mHistoryData.size() == MAX_HISTORY_DATA){
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
			param = data.imstatus;
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
				if (data.message % 10 == 3)
				{
					// MAINT/NORMAL時
					str = _T("MAINT/NORMAL");
				}
				else
				{
					// それ以外
					str.Format(_T("%s"), (LPCTSTR)mEqHistStatusString[data.status]);
				}
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
	if (item == 0)
		AutoSizeColumns();

	if (m_History.GetSelectedCount() == 0){
		GetDocument()->SetTitleEx();
		m_History.EnsureVisible(item, FALSE);
	}
	else{
		CString str;
		str = GetDocument()->GetTitleEx();
		str += _T("[スクロール停止中]");
		GetDocument()->SetTitleEx(str);
	}
	m_History.SetRedraw(true);
	mSection.Unlock();
}


void CEquipmentHistoryView::OnSize(UINT nType, int cx, int cy)
{
	CHistroyView::OnSize(nType, cx, cy);
}


LRESULT CEquipmentHistoryView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case eMessage_ThreadNormal:
		AddHistroy();
		break;
	case eMessage_History:
	{
		if (m_History.GetSelectedCount() == 0){
			TRACE("### Start Auto Scroll\n");
			// スクロール停止の解除
			// 保留されているデータの追加
			mSection.Lock();
			RestoreHoldData();
			mSection.Unlock();
			// タイトル文字列変更
			GetDocument()->SetTitleEx();
		}
		else{
			TRACE("### Stop Auto Scroll\n");
			// タイトル文字列変更
			CString str;
			str = GetDocument()->GetTitleEx();
			str += _T("[スクロール停止中]");
			GetDocument()->SetTitleEx(str);
		}
		break;
	}

	default:
		return CHistroyView::WindowProc(message, wParam, lParam);
	}
	return TRUE;
}
