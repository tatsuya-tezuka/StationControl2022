/*============================================================================*/
/*! �^�p����

-#

@param
@retval

*/
/*============================================================================*/
// OperationHistoryView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "StationControlDoc.h"
#include "OperationHistoryView.h"


// COperationHistoryView

IMPLEMENT_DYNCREATE(COperationHistoryView, CHistroyView)

COperationHistoryView::COperationHistoryView()
{
	m_nHistoryType = eHistory_Operation;
}

COperationHistoryView::~COperationHistoryView()
{
}

BEGIN_MESSAGE_MAP(COperationHistoryView, CHistroyView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// COperationHistoryView �`��

void COperationHistoryView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
}


// COperationHistoryView �f�f

#ifdef _DEBUG
void COperationHistoryView::AssertValid() const
{
	CHistroyView::AssertValid();
}

#ifndef _WIN32_WCE
void COperationHistoryView::Dump(CDumpContext& dc) const
{
	CHistroyView::Dump(dc);
}
#endif
#endif //_DEBUG


// COperationHistoryView ���b�Z�[�W �n���h���[


void COperationHistoryView::OnInitialUpdate()
{
	m_nControlID = eOperationHistory;
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
void COperationHistoryView::AddHistroy()
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
void COperationHistoryView::AddHistroy(stHistoryData& data, bool bDataAdd/*=true*/)
{
	// ���M�����u�}�g�ǉ^�p�ǐ����u�v�̏ꍇ�̓��ʏ���
	if (data.equipment == eSenderItemMax){
		// ���M�����u�}�g�ǉ^�p�ǐ����u�v�ł���
		if (theApp.GetSelectStationMode() == eStation_1){
			data.station = theApp.GetSelectStation();
		}
	}
	else{
		if (theApp.GetSelectStationMode() == eStation_1 && theApp.GetSelectStation() != data.station){
			return;
		}
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
			// ���M�����u�}�g�ǉ^�p�ǐ����u�v�̏ꍇ�̓��ʏ���
			if (param == eSenderItemMax){
				if (theApp.GetSelectStationMode() == eStation_1){
					//bSet = false;
				}
				else{
					if (!(mFilterData.station & (1 << eStation_MAX)))
						bSet = false;
				}
				continue;
			}
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
				// ���M�����u�}�g�ǉ^�p�ǐ����u�v�̏ꍇ�̓��ʏ���
				if (data.equipment == eSenderItemMax){
					str.Format(_T("%s"), (LPCTSTR)mStationStringEx);
				}
				break;
			case eFilterItem_Equipment:
				if (data.equipment == eSenderItemMax){
					str = _T("");
				}
				else{
					str.Format(_T("%s"), (LPCTSTR)mHistEquipmentString[data.equipment]);
				}
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

void COperationHistoryView::OnSize(UINT nType, int cx, int cy)
{
	CHistroyView::OnSize(nType, cx, cy);
}


LRESULT COperationHistoryView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
