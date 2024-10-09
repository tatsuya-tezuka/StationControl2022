// TimeEvenHistoryView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "OperationTimeLineDoc.h"
#include "TimeEvenHistoryView.h"


// CTimeEvenHistoryView

IMPLEMENT_DYNCREATE(CTimeEvenHistoryView, CHistroyView)

CTimeEvenHistoryView::CTimeEvenHistoryView()
{
	m_nHistoryType = eHistory_Control;
}

CTimeEvenHistoryView::~CTimeEvenHistoryView()
{
}

BEGIN_MESSAGE_MAP(CTimeEvenHistoryView, CHistroyView)
END_MESSAGE_MAP()


// CTimeEvenHistoryView 描画

void CTimeEvenHistoryView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 描画コードをここに追加してください。
}


// CTimeEvenHistoryView 診断

#ifdef _DEBUG
void CTimeEvenHistoryView::AssertValid() const
{
	CHistroyView::AssertValid();
}

#ifndef _WIN32_WCE
void CTimeEvenHistoryView::Dump(CDumpContext& dc) const
{
	CHistroyView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTimeEvenHistoryView メッセージ ハンドラー

void CTimeEvenHistoryView::OnInitialUpdate()
{
	m_nControlID = eEventHistory;
	m_nTabItem = eTabItem_Filter;

	CHistroyView::OnInitialUpdate();

	GetDocument()->SetTitleEx(mTimeViewTitle[eTimeViewType_Event]);
	m_History.SetParent(this);
}

/*============================================================================*/
/*! 運用履歴

-# 履歴情報の追加

@param
@retval

*/
/*============================================================================*/
void CTimeEvenHistoryView::AddHistroy()
{
	CGraphMutex::Lock(eData);
	deque<stHistoryData>& data = theApp.GetEventHistoryData();
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
void CTimeEvenHistoryView::AddHistroy(stHistoryData& data, bool bDataAdd/* = true*/)
{
	if (theApp.GetSelectStation() != data.station){
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
		case eFilterItemEvent_Time:
			continue;
		case eFilterItemEvent_Station:
			param = data.station;
			break;
		case eFilterItemEvent_Status:
			param = data.status;
			break;
		case eFilterItemEvent_MessageId:
			param = data.message;
			break;
		case eFilterItemEvent_Message:
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
			case eFilterItemEvent_Time:
				str.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"), data.year, data.month, data.day, data.hour, data.minute, data.second);
				break;
			case eFilterItemEvent_Station:
				str.Format(_T("%s"), mStationString[data.station]);
				break;
			case eFilterItemEvent_Status:
				str.Format(_T("%s"), (LPCTSTR)mHistStatusString[data.status]);
				break;
			case eFilterItemEvent_MessageId:
				str.Format(_T("%6d"), data.message);
				break;
			case eFilterItemEvent_Message:
				str.Format(_T("%s"), data.msgstring);
				break;
			default:
				continue;
			}
			AddItem(item, subitem, str, data.status);
		}
	}
	if (item == 0)
		AutoSizeColumns();

	if (m_History.GetSelectedCount() == 0){
		GetDocument()->ResetTitleEx();
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


void CTimeEvenHistoryView::OnSize(UINT nType, int cx, int cy)
{
	CHistroyView::OnSize(nType, cx, cy);
}


LRESULT CTimeEvenHistoryView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case ID_PIPE_RECEIVE:
		AddHistroy();
		break;
	case eMessageTimeline_History:
	{
		if (m_History.GetSelectedCount() == 0){
			TRACE("### Start Auto Scroll\n");
			// スクロール停止の解除
			// 保留されているデータの追加
			mSection.Lock();
			RestoreHoldData();
			mSection.Unlock();
			// タイトル文字列変更
			GetDocument()->ResetTitleEx();
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
