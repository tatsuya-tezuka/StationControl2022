// TimeEvenHistoryView.cpp : �����t�@�C��
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


// CTimeEvenHistoryView �`��

void CTimeEvenHistoryView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
}


// CTimeEvenHistoryView �f�f

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


// CTimeEvenHistoryView ���b�Z�[�W �n���h���[

void CTimeEvenHistoryView::OnInitialUpdate()
{
	m_nControlID = eEventHistory;
	m_nTabItem = eTabItem_Filter;

	CHistroyView::OnInitialUpdate();

	GetDocument()->SetTitleEx(mTimeViewTitle[eTimeViewType_Event]);
	m_History.SetParent(this);
}

/*============================================================================*/
/*! �^�p����

-# �������̒ǉ�

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
/*! �^�p����

-# �������̒ǉ�

@param	data		�������
@param	bDataAdd	���X�g�p�f�[�^�ւ̒ǉ�
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
			// ���X�g�ő吔���X�N���[����~���Ȃ̂ŁA�f�[�^�ۗ����s��
			mSection.Unlock();
			return;
		}
	}

	// �����ɂ���̂̓f�[�^�ۗ��͊J�n����Ă��Ȃ����
	if (m_History.GetItemCount() == MAX_HISTORY_DATA){
		// �����ł̍ő吔�`�F�b�N�̓X�N���[���������������Ȃ�
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
		str += _T("[�X�N���[����~��]");
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
			// �X�N���[����~�̉���
			// �ۗ�����Ă���f�[�^�̒ǉ�
			mSection.Lock();
			RestoreHoldData();
			mSection.Unlock();
			// �^�C�g��������ύX
			GetDocument()->ResetTitleEx();
		}
		else{
			TRACE("### Stop Auto Scroll\n");
			// �^�C�g��������ύX
			CString str;
			str = GetDocument()->GetTitleEx();
			str += _T("[�X�N���[����~��]");
			GetDocument()->SetTitleEx(str);
		}
		break;
	}

	default:
		return CHistroyView::WindowProc(message, wParam, lParam);
	}
	return TRUE;
}
