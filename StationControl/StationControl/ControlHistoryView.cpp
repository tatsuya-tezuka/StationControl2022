/*============================================================================*/
/*! ���䗚��

-#

@param
@retval

*/
/*============================================================================*/
// ControlHistoryView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "ControlHistoryView.h"


// CControlHistoryView

IMPLEMENT_DYNCREATE(CControlHistoryView, CHistroyView)

CControlHistoryView::CControlHistoryView()
{
	m_nHistoryType = eHistory_Control;
}

CControlHistoryView::~CControlHistoryView()
{
}

BEGIN_MESSAGE_MAP(CControlHistoryView, CHistroyView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CControlHistoryView �`��

void CControlHistoryView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
}


// CControlHistoryView �f�f

#ifdef _DEBUG
void CControlHistoryView::AssertValid() const
{
	CHistroyView::AssertValid();
}

#ifndef _WIN32_WCE
void CControlHistoryView::Dump(CDumpContext& dc) const
{
	CHistroyView::Dump(dc);
}
#endif
#endif //_DEBUG


// CControlHistoryView ���b�Z�[�W �n���h���[

void CControlHistoryView::OnInitialUpdate()
{
	m_nControlID = eControlHistory;
	m_nTabItem = eTabItem_Filter;

	CHistroyView::OnInitialUpdate();

	GetDocument()->SetTitleEx();
	m_History.SetParent(this);
}

/*============================================================================*/
/*! �^�p����

-# �������̒ǉ�

@param
@retval

*/
/*============================================================================*/
void CControlHistoryView::AddHistroy()
{
	CGraphMutex::Lock(eData);

	deque<stHistoryData>& data = theApp.GetDataCollection().GetControlHistoryData();
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
void CControlHistoryView::AddHistroy(stHistoryData& data, bool bDataAdd/* = true*/)
{
	if (theApp.GetSelectStationMode() == eStation_1 && theApp.GetSelectStation() != data.station){
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
	if (item == 0)
		AutoSizeColumns();

	if (m_History.GetSelectedCount() == 0){
		GetDocument()->SetTitleEx();
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


void CControlHistoryView::OnSize(UINT nType, int cx, int cy)
{
	CHistroyView::OnSize(nType, cx, cy);
}


LRESULT CControlHistoryView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case eMessage_ThreadNormal:
		AddHistroy();
		break;
	case eMessage_History:
	{
		if (m_History.GetSelectedCount() == 0){
			TRACE("### Start Auto Scroll\n");
			// �X�N���[����~�̉���
			// �ۗ�����Ă���f�[�^�̒ǉ�
			mSection.Lock();
			RestoreHoldData();
			mSection.Unlock();
			// �^�C�g��������ύX
			GetDocument()->SetTitleEx();
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
